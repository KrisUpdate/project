/*
 * qci_udp_packet.c
 *
 *  Created on: 2019�~2��13��
 *      Author: 95111301
 */

#include <string.h>
#include "Ifx_Console.h"
#include "qci_udp_packet.h"
#include "qci_udp_client.h"
#include "lwip/opt.h"
#include "os.h"
#include "AsclinShellInterface.h"
#include "IfxScuRcu.h"
#include "Configuration.h"
#include "Project_definition.h"
#if PROJECT == PROJECT_PAGASUS2
#include <cangateway.h>
#include "resource.h"
#endif
#if PROJECT == PROJECT_TRIBOARD
#include "CanBasicDemo.h"
#include "AsclinLinDemo.h"
#endif

#if PROJECT == PROJECT_PAGASUS2 || PROJECT == PROJECT_V3NA
extern struct udp_pcb *gw_sendback_pcb;
#include "board.h"
#include "eray.h"
#include "Mcmcan.h"
extern uint8 canMode, erayUdpMode;
extern volatile boolean show_Canbus_log;
#endif

#define printf Ifx_Console_print
uint8 udp_host = 0;
extern uint8 g_eraytx_num;
void dump_qci_udp_packet(struct qci_udp_packet *packet)
{
	uint8 idx = 0 ;
	printf("%s\r\n",__func__);
	printf("\r\nheader \r\n");
	printf("direction 0x%02X \tviu 0x%02X mcu 0x%02X\tbus type %d\r\n",
											packet->header.packet_direct,
											packet->header.target_viu,
											packet->header.target_mcu,
											packet->header.bus_type);
	printf("info \r\n");
	printf("info len %d\tinfo code %d\tbus id %d\trs %d\tcan id 0x%08X\tcan data len %d \r\n",
											packet->info_len,
											packet->info_code,
											packet->bus_id,
											packet->rs,
											packet->canid,
											packet->candlc);
	printf("payload \r\n");
	for(idx = 0; idx < packet->candlc; idx++)
		printf("0x%02X\t",packet->payload[idx]);
	printf("\r\n");

}

#if PROJECT == PROJECT_PAGASUS2 || PROJECT == PROJECT_V3NA
void exchange_role(struct qci_udp_packet *packet, uint16 info_code)
{
	uint8 source;
	packet->header.packet_direct = toHOST;
	source = packet->header.target_viu;
	packet->header.target_viu = packet->header.target_mcu;
	packet->header.target_mcu = source;
	packet->info_code = info_code;
}

uint16 check_recv_udp(struct qci_udp_packet *packet)
{
	if (packet->header.packet_direct != toVIU) {
		exchange_role(packet, MCU_received_invalid_direction_code);
		return 1;
	}
	if (packet->header.target_mcu == AURIX1) {
		if (board.aurix_index != MAIN_AURIX) {
			exchange_role(packet, MCU_received_invalid_VIU_ID);
			return 1;
		}
		if (packet->header.bus_type != typeCAN && packet->header.bus_type != typeERAY) {
			exchange_role(packet, MCU_received_invalid_bus_type);
			return 1;
		}
	} else if (packet->header.target_mcu == AURIX2) {
		if (board.aurix_index != SECOND_AURIX) {
			exchange_role(packet, MCU_received_invalid_VIU_ID);
			return 1;
		}
		if (packet->header.bus_type != typeCAN) {
			exchange_role(packet, MCU_received_invalid_bus_type);
			return 1;
		}
	} else {
			exchange_role(packet, MCU_received_invalid_VIU_ID);
			return 1;
	}

	if (packet->header.bus_type == typeCAN) {
		if (packet->bus_id < 0 || packet->bus_id > 5) {
			exchange_role(packet, MCU_received_invalid_BUS_ID);
			return 1;
		}
		if (packet->candlc != 0 && packet->candlc != 1 && packet->candlc != 2 && packet->candlc != 3 &&\
			packet->candlc != 4 && packet->candlc != 5 && packet->candlc != 6 && packet->candlc != 7 &&\
			packet->candlc != 8 && packet->candlc != 12 && packet->candlc != 16 && packet->candlc != 20 &&\
			packet->candlc != 24 && packet->candlc != 32 && packet->candlc != 48 && packet->candlc != 64) {
			exchange_role(packet, MCU_received_invalid_msg_length);
			return 1;
		}
	} else if (packet->header.bus_type == typeERAY) {
		if (!(packet->bus_id == 1 || packet->bus_id == 0x10)) { //TBD
			exchange_role(packet, MCU_received_invalid_BUS_ID);
			return 1;
		}
		if ((packet->candlc%2) != 0 || packet->candlc < 0 || packet->candlc > 254) {
			exchange_role(packet, MCU_received_invalid_msg_length);
			return 1;
		}
	}
	packet->info_code = MCU_scheduled_successfully;
	return 0;
}
#endif

void parse_qci_udp_packet(ip_addr_t *addr, struct qci_udp_packet *packet)
{
	//printf("parse success\r\n");
#if PROJECT == PROJECT_PAGASUS2
    uint8 bufdata[8];
#endif
#if PROJECT != PROJECT_V3NA
  #if PROJECT != PROJECT_PAGASUS2
	dump_qci_udp_packet(packet);
  #endif
#endif

    if(packet->header.packet_direct == toHOST){
		//printf("toHOST missing\r\n");
        LWIP_DEBUGF(QCI_UDP_SERVICE_DEBUG, ("to toHOST, discard frame\r\n"));
        return ;
    }
	//printf("bus_type: %d\r\n", packet->header.bus_type);
    switch (packet->header.bus_type)
    {
        case typeCAN:
        {
			//printf("typeCAN\r\n");
#if PROJECT== PROJECT_TRIBOARD
        Ifx_Console_print("[UDP] recv can msg, send it from CAN(%d) with ID(%x)\r\n", packet->bus_id,  packet->canid);
        udp_host = packet->header.target_viu;
        QciCan_VCU_Send_To_CanNode(packet->bus_id,  packet->canid, packet->candlc, packet->payload);
#elif PROJECT== PROJECT_PAGASUS2 || PROJECT== PROJECT_V3NA
#if PROJECT== PROJECT_PAGASUS2
        if(rt_sample.enabled == TRUE){
            can_latency_measure();
        }
#else
        if (show_Canbus_log) {
            Ifx_Console_print("[UDP] send CAN msg right away\r\n");
        }
#endif

        if (check_recv_udp(packet)) {
            printf("[UDP] error udp format, info_code is 0x%X, return it!\r\n", packet->info_code);
            udp_connect(gw_sendback_pcb, addr, QCI_UDP_SERVER_PORT);
            qci_udp_multi_send(gw_sendback_pcb, (uint8 *)packet, QCI_UDP_TOTAL_HEADER_LEN + packet->candlc);
        } else {
            canMode = CAN_GATEWAY_MODE;
            udp_host = packet->header.target_viu;
#if PROJECT== PROJECT_PAGASUS2
            if (show_Canbus_log) {
                Ifx_Console_print("GW: bus:0x%x, msgid:0x%x, lc:0x%x\r\n", packet->bus_id, packet->canid, packet->candlc);
            }
            if(packet->candlc <= 8){
                //do memory alignment because the CAN illd expects a word alignment address to handle payload copy
                //printf("length: %d\r\n",packet->candlc);
				memcpy(bufdata, packet->payload, 8);
                send_multican_message_gw(packet->bus_id, packet->canid, packet->candlc, (uint32 *)bufdata, FALSE);
            }else{
				//printf("length: %d\r\n",packet->candlc);
                send_multican_message_gw(packet->bus_id, packet->canid, packet->candlc,  (uint32 *)&packet->payload, FALSE);
            }
            
#else
            send_multican_message(packet->bus_id, packet->canid, packet->candlc, (uint32 *)&packet->payload, FALSE, fdMode);
#endif
        }
#endif
        break;
    	}
    	case typeLIN:
    	{
			//printf("typeLIN\r\n");
#if PROJECT== PROJECT_TRIBOARD
			uint32 readData[2];
			udp_host = packet->header.target_viu;
    		QciLin_VCU_Send_To_LinBus(packet->bus_id, 0, packet->candlc, packet->payload);
    		QciLin_VCU_Read_From_LinBus(packet->bus_id, packet->canid, packet->candlc, (uint8 *)readData);
    		printf("[UDP] read from LIN(%d) with ID(0x%X): 0x%X, 0x%X\r\n", packet->bus_id, packet->canid, readData[0], readData[1]);
#endif
    		break;
    	}
	case typeERAY:
	{
		//printf("typeERAY\r\n");
#if (PROJECT == PROJECT_PAGASUS2)|| PROJECT == PROJECT_V3NA
    	//Ifx_Console_print("[UDP] send eray msg right away\r\n");
    	if (check_recv_udp(packet)) {
			printf("[UDP] error udp format, info_code is 0x%X, return it!\r\n", packet->info_code);
			udp_connect(gw_sendback_pcb, addr, QCI_UDP_SERVER_PORT);
			qci_udp_multi_send(gw_sendback_pcb, (uint8 *)packet, QCI_UDP_TOTAL_HEADER_LEN + packet->candlc);
    	} else {
             if(packet->bus_id!=typeERAY_NORMALMODE){
#if PROJECT == PROJECT_V3NA
			IfxEray_Init_testmode(FALSE, FALSE, FRMAE_ID_INGORE, IfxSrc_Tos_cpu0);
#elif PROJECT== PROJECT_PAGASUS2
            IfxErayTestMode_init(FALSE, FALSE, FRMAE_ID_INGORE, IfxSrc_Tos_cpu0);
#else
			IfxErayTestMode_init(FALSE, FALSE, FRMAE_ID_INGORE, IfxSrc_Tos_cpu0);
#endif
			erayUdpMode = ERAY_MASTER;
			udp_host = packet->header.target_viu;
			send_eray_frame((uint32 *)&packet->payload, 2);
#if PROJECT == PROJECT_V3NA
			IfxEray_Init_testmode(TRUE, FALSE, FRMAE_ID_INGORE, IfxSrc_Tos_cpu0);
#elif PROJECT== PROJECT_PAGASUS2
            IfxErayTestMode_init(TRUE, FALSE, FRMAE_ID_INGORE, IfxSrc_Tos_cpu0);
#else
			IfxErayTestMode_init(TRUE, FALSE, FRMAE_ID_INGORE, IfxSrc_Tos_cpu0);
#endif
            }
#if PROJECT == PROJECT_PAGASUS2
    	//Ifx_Console_print("[UDP] send eray msg right away\r\n");
    	if (check_recv_udp(packet)) {
			printf("[UDP] error udp format, info_code is 0x%X, return it!\r\n", packet->info_code);
			udp_connect(gw_sendback_pcb, addr, QCI_UDP_SERVER_PORT);
			qci_udp_multi_send(gw_sendback_pcb, (uint8 *)packet, QCI_UDP_TOTAL_HEADER_LEN + packet->candlc);
    	} else {
            if(packet->bus_id==typeERAY_NORMALMODE){
                //printf("qci parser udp message id :0x%x \r\n",packet->canid);
                //printf("qci parser udp message id sample:0x%x \r\n",packet->sample);
                //printf("qci parser udp message id frameid_h:0x%x \r\n",packet->frameid_h);
                //printf("qci parser udp message id frameid_l:0x%x \r\n",packet->frameid_l);
                //printf("qci parser udp message id cyclecnt:0x%x \r\n",packet->cyclecnt);
                //printf("qci parser udp eray length:0x%x \r\n",packet->eraylc);
            }
            
            update_eray_send_data(packet);
            g_eraytx_num = packet->eraylc/2;
            char s[20];
            //sprintf(s,"s%d d %d %d %d ",packet->sample, packet->frameid_l,packet->payload[0],packet->payload[1]);
            //AsclinShellInterface_execute_cmd("erayconfig", s);
            sprintf(s,"s%d",packet->sample);
            AsclinShellInterface_execute_cmd("eray", s);
       }
#endif
    	}
#else
		uint32 * data;
		data = (uint32 *)&packet->payload;
		printf("[UDP] recv eray: 0x%X 0x%X\r\n", *data, *(data+1));
#if PROJECT!= PROJECT_V31C
#if FEATURE_ERAY_TESTMODE
                erayUdpMode = ERAY_MASTER;
                send_eray_frame(data, packet->candlc/2); //bytes to words
#endif
#endif
#endif
		break;
	}
    	default:
		//printf("default\r\n");
		break;
    }
}
