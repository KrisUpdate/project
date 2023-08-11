#include "Platform_Types.h"
#include "Ifx_Types.h"
#include "IfxStdIf_DPipe.h"
#include "ota.h"
#include "pbuf.h"
#include "Ip_addr.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define BOUND_ANY_IP_ENABLE       0
#if OLD_OTA_ENABLE == 1
#define OTA_UDP_PORT                     50000
#else
#define OTA_UDP_PORT                     5001
#endif
#define TIME_TICK_MS                      100
#define LOOP_BACK_TEST_ENABLE   0

/* set 1 to enable separate Tegra detection, need protocol update */
#define TEGRA_SEPERATE_ENABLE   0

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/
static rec_ptr ota_receive_cb = NULL;
static boolean isEthEnable = FALSE;
static boolean isResume = FALSE;
static ip_addr_t dest_ip;
static uint16 dest_port;
static boolean is_dest_known = FALSE;
static struct udp_pcb *ota_udp_object_ptr;

void OTA_TegraEthComm_init(void)
{
    OTA_UDP_APP_DEBUG_PRINTF(("Initializing the OTA ethernet communication between Tegra and Aurix\r\n"));
    isEthEnable = TRUE;
}

void OTA_TegraEthComm_resume(void)
{
    OTA_UDP_APP_DEBUG_PRINTF(("Resuming the OTA ethernet communication between Tegra and Aurix\r\n"));
    isResume = TRUE;
}

void OTA_TegraEthComm_routine(void)
{
    if(!isEthEnable)
        return;
    static boolean initial = FALSE;
    if((!initial) && isResume)
    {
        OTA_PRINTF_ALWAYS(("OTA_TegraEthComm: it may initial the UDP again, please report it to MCU owner\n\r"));
    }
    if(!initial)
    {
        /* Checking the dhcp ip status and start the UDP after it's ready */
        if(Ethernet_Is_Ip_Bound())
        {
            uint32 ip = 0;
            if(Ethernet_get_ip(&ip))
            {
                OTA_DEBUG_PRINTF(("OTA_TegraEthComm: Get ip address is 0x%x\n\r", ip));
            }
            else
            {
                OTA_DEBUG_PRINTF(("OTA_TegraEthComm: Fail to get the bound ip\n\r"));
            }
            initial = TRUE;
            udp_app_init_ota(ip);
//#if ETHERNET_OTA_ENABLE == 1
            udp_app_register_rec_ota_cb(ota_handle_state);
//#endif
            OTA_DEBUG_PRINTF(("Tegra<->Aurix OTA ethernet communication is ready\n"));
        }
    }
    /* we don't need initial again if it's a resume process */
    if(isResume)
    {
        if(Ethernet_Is_Ip_Bound())
        {
            OTA_DEBUG_PRINTF(("Tegra<->Aurix OTA ethernet communication is ready\n"));
            isResume = FALSE;
        }
    }
}
