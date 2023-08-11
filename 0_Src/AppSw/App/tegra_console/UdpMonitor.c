#include "Configuration.h"

#if (PROJECT == PROJECT_PAGASUS2)
#include "tegra_console.h"
#include "Board.h"
#include "string.h"
#include "udp.h"
#include "AsclinShellInterface.h" // for debug

#define UDP_MONITOR_DEBUG_ENABLE 1

#if UDP_MONITOR_DEBUG_ENABLE == 1
#define UDP_MONITOR_DEBUG_PRINTF(x) AsclinShellInterface_printf x
#else
#define UDP_MONITOR_DEBUG_PRINTF(x)
#endif

extern char monitor_buffer[2000];
#define RESPONSE_BUF_SIZE 30
#define SECTION_NUM_MAX 20

typedef enum{
    UM_CMD_UNKNOWN,
    UM_CMD_START,
    UM_CMD_STOP,
    UM_CMD_GET
} udp_monitor_command_t;

typedef enum{
    UM_STATE_NO_INIT,
    UM_STATE_ENABLE,
    UM_STATE_DISABLE
} udp_monitor_state_t;

typedef struct{
    char *src;
    boolean ready;
} section_t;

static udp_monitor_state_t monitor_state = UM_STATE_NO_INIT;
static section_t sections[SECTION_NUM_MAX];
static int section_num = 0;

//static char monitor_buffer[MONITOR_BUF_SIZE];  // store monitor data
static char response_buffer[RESPONSE_BUF_SIZE];  // response for monitor data not available

static char *response_ptr;
static int response_len = 0;

static uint32 package_count = 0;

static struct udp_pcb *tegra_console_monitor_ptr = NULL;

extern int adc_section_id;
//#define VOLTAGES_MONITOR_BUFFER_SIZE 620
#define VOLTAGES_MONITOR_BUFFER_SIZE 1600
extern volatile uint8 voltages_monitor_buffer[VOLTAGES_MONITOR_BUFFER_SIZE];
extern volatile int voltages_monitor_buffer_lens;
int monitor_buffer_update()
{
    int i, buffer_used=0;

    memset(monitor_buffer, 0x00, 2000);

    for(i=0; i<section_num; i++){
        buffer_used += snprintf(monitor_buffer+buffer_used, 2000-buffer_used, "%s", sections[i].src);
        if(buffer_used >= 2000){
            UDP_MONITOR_DEBUG_PRINTF(("not enough buffer"));
            return 0;
        }
    }
//    buffer_used += snprintf(monitor_buffer+buffer_used, MONITOR_BUF_SIZE-buffer_used, "#####");
    return buffer_used;
}

boolean is_all_ready()
{
    int i;
    for(i=0; i<section_num; i++){
        if(!sections[i].ready) return FALSE;
    }
    return TRUE;
}

void handle_request(struct udp_pcb *pcb, ip_addr_t *addr, u16_t port, udp_monitor_command_t command)
{
    volatile uint32 left;
    switch(command){
        case UM_CMD_START:
            monitor_state = UM_STATE_ENABLE;
            response_len = snprintf(response_buffer, RESPONSE_BUF_SIZE, "#####%s#####", "START");
            response_ptr = response_buffer;
            break;
        case UM_CMD_STOP:
            monitor_state = UM_STATE_DISABLE;
            response_len = snprintf(response_buffer, RESPONSE_BUF_SIZE, "#####%s#####", "STOP");
            response_ptr = response_buffer;
            break;
        case UM_CMD_GET:
            if(monitor_state != UM_STATE_ENABLE){
                response_len = snprintf(response_buffer, RESPONSE_BUF_SIZE, "#####%s#####", "NOT_ENABLED");
                response_ptr = response_buffer;
                break;
            }
            if(is_all_ready()){
                response_len = snprintf(response_buffer, 2000, "#####%u", package_count++);
                response_ptr = response_buffer;
                monitor_response_send(pcb, addr, port, response_len, response_ptr);

                if(adc_section_id == TRUE){
                    // suppose that voltages_monitor_buffer_lens <= 2000
                    if(voltages_monitor_buffer_lens <= 500){
                        monitor_response_send(pcb, addr, port, voltages_monitor_buffer_lens, &voltages_monitor_buffer[0]);
                    }
                    else{
                        monitor_response_send(pcb, addr, port, 500, &voltages_monitor_buffer);
                        monitor_response_send(pcb, addr, port, 500, &voltages_monitor_buffer[500]);
                        monitor_response_send(pcb, addr, port, voltages_monitor_buffer_lens - 1000, &voltages_monitor_buffer[1000]);
                    }
                    adc_section_id = FALSE;
                }

                response_len = monitor_buffer_update();
                response_ptr = monitor_buffer;                
                monitor_response_send(pcb, addr, port, response_len, response_ptr);
                UdpMonitor_resource_reset();
                response_len = snprintf(response_buffer, RESPONSE_BUF_SIZE, "#####");
                response_ptr = response_buffer;
            }else if(package_count == 0){
                response_len = snprintf(response_buffer, RESPONSE_BUF_SIZE, "#####%s#####", "NOT_READY");
                response_ptr = response_buffer;
            }
            break;
        default:
            response_len = snprintf(response_buffer, RESPONSE_BUF_SIZE, "#####%s#####", "UNKNOWN_COMMAND");
            response_ptr = response_buffer;
            break;
    }
    monitor_response_send(pcb, addr, port, response_len, response_ptr);
}

void monitor_response_send(struct udp_pcb * udp_ptr, ip_addr_t* ip_ptr, uint16 port, int buf_len, char *buf)
{
    struct pbuf *udp_buf = pbuf_alloc(PBUF_TRANSPORT, buf_len, PBUF_RAM);
    if(udp_buf == NULL)
    {
        UDP_MONITOR_DEBUG_PRINTF(("Can not alloc the buffer, len = %d\n\r", buf_len));
    }
    else
    {
        udp_buf->payload = buf;
        udp_sendto(udp_ptr, udp_buf, ip_ptr, port);
        pbuf_free(udp_buf);
    }
}

void UdpMonitor_resource_reset(void)
{
    unsigned char i;
    for(i = 0; i< section_num; i++)
      sections[i].ready = FALSE;
}

boolean UdpMonitor_resource_is_ready(int section_id)
{
    return sections[section_id].ready;
}

static void tegra_console_monitor_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p,
        ip_addr_t *addr, u16_t port)
{
    LWIP_UNUSED_ARG(arg);
    LWIP_UNUSED_ARG(pcb);
    char empty_arg[] = " ";
    char *firstlf;

    udp_monitor_command_t command;
    UDP_MONITOR_DEBUG_PRINTF(("UDP Monitor Server Rec: ip address is 0x%x, port %d\n\r", *addr, port));
    if((port != TEGRA_A_CLIENT_PORT) &&
       (port != TEGRA_B_CLIENT_PORT))
    { /* we don't care message from other ports */
        return;
    }
    if(p->payload != NULL)
    {
        firstlf = strchr((char*)p->payload,'\n');
        *firstlf = '\0';
        UDP_MONITOR_DEBUG_PRINTF(("UDP Monitor Received>%s\n\r", p->payload));

        char* cmd = NULL;
        char* arg = NULL;
        if(strchr(p->payload, ' ') != NULL)
        { /* we have parameter */
            cmd = strtok((char*)p->payload, " ");
            arg = strtok(NULL, "\n");
        }
        else
        {
            cmd = strtok((char*)p->payload, "\n");
        }
        if(cmd != NULL)
        {
            if(arg == NULL)
            {
                arg = empty_arg;
            }

            if( memcmp(cmd,"GET",3)==0 ){
                command = UM_CMD_GET;
            } else if( memcmp(cmd,"START",5)==0 ){
                command = UM_CMD_START;
            }else if( memcmp(cmd,"STOP",4)==0 ){
                command = UM_CMD_STOP;
            }else{
                command = UM_CMD_UNKNOWN;
            }
            handle_request(pcb, addr, port, command);
        }
    }
}


void UdpMonitor_init(void)
{
    tegra_console_init_udp(&tegra_console_monitor_ptr, TEGRA_CONSOLE_MONITOR_PORT, tegra_console_monitor_recv);
    monitor_state = UM_STATE_DISABLE;
}

int UdpMonitor_registor(char *src)
{
    if(section_num >= SECTION_NUM_MAX)
        return -1;
    int section_id = section_num;
    section_num++;

    sections[section_id].src = src;
    sections[section_id].ready = FALSE;
    return section_id;
}

void UdpMonitor_notice(int section_id)
{
    sections[section_id].ready = TRUE;
}

boolean UdpMonitor_is_enabled()
{
    return monitor_state == UM_STATE_ENABLE;
}

#elif (PROJECT == PROJECT_V31C)
#include "tegra_console.h"
#include "Board.h"
#include "string.h"
#include "udp.h"
#include "AsclinShellInterface.h" /* for debug */
#include "PeripheralMonitor.h"

#define UDP_MONITOR_DEBUG_ENABLE 1

#if UDP_MONITOR_DEBUG_ENABLE == 1
#define UDP_MONITOR_DEBUG_PRINTF(x) AsclinShellInterface_printf x
#else
#define UDP_MONITOR_DEBUG_PRINTF(x)
#endif

#define MONITOR_BUF_SIZE 1000
#define RESPONSE_BUF_SIZE 30
#define SECTION_NUM_MAX 20

#define HEARTBEAT_BUF_SIZE 50

typedef enum{
    UM_CMD_UNKNOWN,
    UM_CMD_START,
    UM_CMD_STOP,
    UM_CMD_GET
} udp_monitor_command_t;

typedef enum{
    UM_STATE_NO_INIT,
    UM_STATE_ENABLE,
    UM_STATE_DISABLE
} udp_monitor_state_t;


static udp_monitor_state_t monitor_state = UM_STATE_NO_INIT;
static boolean sections[SECTION_NUM_MAX];
static uint8 section_num = 0;/* how many sections */

static char monitor_buffer[MONITOR_BUF_SIZE+1];  /* store monitor data */
static char response_buffer[RESPONSE_BUF_SIZE];  /* response for monitor data not available */
static char hb_buf[HEARTBEAT_BUF_SIZE+1];        /* heartbeat message buffer */

static char *response_ptr;
static int response_len = 0;

static struct udp_pcb *tegra_console_monitor_ptr = NULL;/* peripheral monitor pcb */
static struct udp_pcb *tegra_heartbeat_ptr = NULL;  /* heartbeat pcb */
ip_addr_t tegra_heartbeat_dst_ip;/* hpa ip */
ip_addr_t sga_heartbeat_dst_ip;/* sga ip */

boolean is_all_ready()
{
    for(uint8 i=0; i<section_num; i++){
        if(!sections[i]) return FALSE;
    }
    return TRUE;
}

void handle_request(udp_monitor_command_t command)
{
    switch(command){
        case UM_CMD_START:
            monitor_state = UM_STATE_ENABLE;
            response_len = snprintf(response_buffer, RESPONSE_BUF_SIZE, "#####%s#####", "START");
            response_ptr = response_buffer;
            break;
        case UM_CMD_STOP:
            monitor_state = UM_STATE_DISABLE;
            response_len = snprintf(response_buffer, RESPONSE_BUF_SIZE, "#####%s#####", "STOP");
            response_ptr = response_buffer;
            break;
        case UM_CMD_GET:
            if(monitor_state != UM_STATE_ENABLE){
                response_len = snprintf(response_buffer, RESPONSE_BUF_SIZE, "#####%s#####", "NOT_ENABLED");
                response_ptr = response_buffer;
                break;
            }
            if(is_all_ready()){
                memset( monitor_buffer, 0x00, MONITOR_BUF_SIZE+1 );
                response_len = peripheral_monitor_buf_update( monitor_buffer, MONITOR_BUF_SIZE );
                response_ptr = monitor_buffer;
            }else{
                response_len = snprintf(response_buffer, RESPONSE_BUF_SIZE, "#####%s#####", "NOT_READY");
                response_ptr = response_buffer;
            }
            break;
        default:
            response_len = snprintf(response_buffer, RESPONSE_BUF_SIZE, "#####%s#####", "UNKNOWN_COMMAND");
            response_ptr = response_buffer;
            break;
    }
}

void monitor_response_send(struct udp_pcb * udp_ptr, ip_addr_t* ip_ptr, uint16 port)
{
    struct pbuf *udp_buf = pbuf_alloc(PBUF_TRANSPORT, response_len, PBUF_RAM);
    if(udp_buf == NULL)
    {
        UDP_MONITOR_DEBUG_PRINTF(("Can not alloc the buffer\n\r"));
    }
    else
    {
        //UDP_MONITOR_DEBUG_PRINTF(("[%s]\n\r", response_ptr));
        udp_buf->payload = response_ptr;
        udp_sendto(udp_ptr, udp_buf, ip_ptr, port);
        pbuf_free(udp_buf);
    }
}

static void tegra_console_monitor_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p,
        ip_addr_t *addr, u16_t port)
{
    LWIP_UNUSED_ARG(arg);
    LWIP_UNUSED_ARG(pcb);
    char empty_arg[] = " ";
    char *firstlf;

    udp_monitor_command_t command;
    UDP_MONITOR_DEBUG_PRINTF(("UDP Monitor Server Rec: ip address is 0x%x, port %d\n\r", *addr, port));
    if((port != TEGRA_B_CLIENT_PORT) &&
       (port != TEGRA_B_CLIENT_RETRY_PORT))
    { /* we don't care message from other ports */
        return;
    }
    if(p->payload != NULL)
    {
        firstlf = strchr((char*)p->payload,'\n');
        *firstlf = '\0';
        UDP_MONITOR_DEBUG_PRINTF(("UDP Monitor Received>%s\n\r", p->payload));

        char* cmd = NULL;
        char* arg = NULL;
        if(strchr(p->payload, ' ') != NULL)
        { /* we have parameter */
            cmd = strtok((char*)p->payload, " ");
            arg = strtok(NULL, "\n");
        }
        else
        {
            cmd = strtok((char*)p->payload, "\n");
        }
        if(cmd != NULL)
        {
            if(arg == NULL)
            {
                arg = empty_arg;
            }

            if( memcmp(cmd,"GET",3)==0 ){
                command = UM_CMD_GET;
            } else if( memcmp(cmd,"START",5)==0 ){
                command = UM_CMD_START;
            }else if( memcmp(cmd,"STOP",4)==0 ){
                command = UM_CMD_STOP;
            }else{
                command = UM_CMD_UNKNOWN;
            }
            handle_request(command);
            monitor_response_send(pcb, addr, port);
        }
    }
}

void tegra_udp_heartbeat_client_send(uint32 heartbeat)
{
    /* send heartbeat */
    if (tegra_heartbeat_ptr!=NULL)
    {
        /* format: "'op_mode' 'local_temp' remote_temp 'heartbeat' 'phy1_temp' 'phy2_temp' 'phy3_temp' 'phy4_temp'" */
        uint32 hb_len =0;
        memset(hb_buf, 0x00, HEARTBEAT_BUF_SIZE+1);
        hb_len = peripheral_heartbeat_buf_update(hb_buf, HEARTBEAT_BUF_SIZE, heartbeat);

        //UDP_MONITOR_DEBUG_PRINTF(("send heartbeat[%s][%d]\n\r", hb_buf, hb_len));

        struct pbuf *udp_buf = pbuf_alloc(PBUF_TRANSPORT, hb_len, PBUF_RAM);

        if(udp_buf == NULL)
        {
            UDP_MONITOR_DEBUG_PRINTF(("Can not alloc the buffer\n\r"));
        }
        else
        {
            udp_buf->payload = hb_buf;
            udp_sendto(tegra_heartbeat_ptr, udp_buf, &tegra_heartbeat_dst_ip, TEGRA_HEARTBEAT_DST_PORT);/* send to Xavier */
            udp_sendto(tegra_heartbeat_ptr, udp_buf, &sga_heartbeat_dst_ip, TEGRA_HEARTBEAT_DST_PORT);/* send to SGA */
            pbuf_free(udp_buf);
        }
    }
}
void tegra_udp_heartbeat_client_init(void)
{
    UDP_MONITOR_DEBUG_PRINTF(("  tegra_udp_heartbeat_client_init. dst port[%d]\n\r", TEGRA_HEARTBEAT_DST_PORT));

    /*assign destination IP address */
    IP4_ADDR(&tegra_heartbeat_dst_ip, 10,42,0,28);/* set Xavier IP */
    IP4_ADDR(&sga_heartbeat_dst_ip, 10,42,0,20);/* set SGA IP */

    /*Create a new UDP control block */
    tegra_heartbeat_ptr = udp_new();

}

void UdpMonitor_init(void)
{
    tegra_console_init_udp(&tegra_console_monitor_ptr, TEGRA_CONSOLE_MONITOR_PORT, tegra_console_monitor_recv);
    monitor_state = UM_STATE_DISABLE;
    section_num = peripheral_get_sections();
    if(section_num > SECTION_NUM_MAX)
    {
        UDP_MONITOR_DEBUG_PRINTF(("UDP Monitor: Too many sections[%d]\n\r", section_num));
        section_num = SECTION_NUM_MAX;
    }
}

int UdpMonitor_registor(char *src)
{
    /* V31C no needed */
	return 0;
}

void UdpMonitor_notice(int section_id)
{
    sections[section_id] = TRUE;
}

boolean UdpMonitor_is_enabled()
{
    return monitor_state == UM_STATE_ENABLE;
}

#elif (PROJECT == PROJECT_TRIBOARD)
//
#elif (PROJECT == PROJECT_V3NA)
//
#else
//
#endif
