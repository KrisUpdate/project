/**
 * \file ErayDemo.c
 * \brief Eray Demo
 *
 * \version iLLD_Demos_1_0_0_11_0
 * \copyright Copyright (c) 2014 Infineon Technologies AG. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 *
 * Infineon Technologies AG (Infineon) is supplying this file for use
 * exclusively with Infineon's microcontroller products. This file can be freely
 * distributed within development tools that are supporting such microcontroller
 * products.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "Configuration.h"
#include "ErayNormalDemo.h"
#include "Ifx_Console.h"
#include <internal_fw_debug.h>
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "ConfigurationIsr.h"
#include "Eray.h"
#include "IfxEray_reg.h"
#include "ip_addr.h"
#include "qci_udp_packet.h"
#include "board.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define printf Ifx_Console_print
#define ERAYNORMAL_ALWAYS_PRINTF(x)  DEBUG_PRINTF_ALWAYS(x)
#define TRANSFER_NUM DATA_PAYLOAD_LENGTH
/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
#pragma section ".bss_cpu1" awc1
App_Eray g_Eray; /**< \brief Eray global data */
#pragma section 
IFX_EXTERN App_ErayTestMode g_ErayTestMode;
uint32 rx_data[1] = {0xDEADBEEF};
eray_app_state g_eray_app_state = ERAY_APP_STATE_IDLE;
uint8 g_eraynoslotsample1 = DEFAULT_NUM_S1;
uint8 g_eraynoslotsample2 = DEFAULT_NUM_S2;
Eray_run_mode g_Eray_run_mode;
uint8 g_eraytx_num = TRANSFER_NUM;

rxheader_list_t rxheader_list[NUM_OF_SLOT_MAX_S2];
txheader_list_t txheader_list[NUM_OF_SLOT_MAX_S1];

extern uint8 udp_host;
extern uint16 udpSeqNum;
extern uint32 udp_ip[4];
extern struct udp_pcb *gw_sendback_pcb;

#if 0
eraytxdata_t g_eraytxdata[NUM_OF_SLOT_MAX_S1]={
                {1,0x0101,0x0102,0x0103,0x0104,0x0105,0x0106,0x0107,0x0108,0x0109,0x010a,0x010b,0x010c,0x011d,0x010e,0x010f,0x0110,0x0111,0x0112,0x0113,0x0114,0x0115,0x0116,0x0117,0x0118},
                {2,0x0201,0x0202,0x0203,0x0204,0x0205,0x0206,0x0207,0x0208,0x0209,0x020a,0x020b,0x020c,0x020d,0x020e,0x020f,0x0210,0x0211,0x0212,0x0213,0x0214,0x0215,0x0216,0x0217,0x0218},
                {3,0x0301,0x0302,0x0303,0x0304,0x0305,0x0306,0x0307,0x0308,0x0309,0x030a,0x030b,0x030c,0x030d,0x030e,0x030f,0x0310,0x0311,0x0312,0x0313,0x0314,0x0315,0x0316,0x0317,0x0318},
                {4,0x0401,0x0402,0x0403,0x0404,0x0405,0x0406,0x0407,0x0408,0x0409,0x040a,0x040b,0x040c,0x040d,0x040e,0x040f,0x0410,0x0411,0x0412,0x0413,0x0414,0x0415,0x0416,0x0417,0x0418},
                {32,0x2001,0x2002,0x2003,0x2004,0x2005,0x2006,0x2007,0x2008,0x2009,0x200a,0x200b,0x200c,0x200d,0x200e,0x200f,0x2010,0x2011,0x2012,0x2013,0x2014,0x2015,0x2016,0x2017,0x2018},
                {48,0x3001,0x3002,0x3003,0x3004,0x3005,0x3006,0x3007,0x3008,0x3009,0x300a,0x300b,0x300c,0x300d,0x300e,0x300f,0x3010,0x3011,0x3012,0x3013,0x3014,0x3015,0x3016,0x3017,0x3018},
                {64,0x4001,0x4002,0x4003,0x4004,0x4005,0x4006,0x4007,0x4008,0x4009,0x400a,0x400b,0x400c,0x400d,0x400e,0x400f,0x4010,0x4011,0x4012,0x4013,0x4014,0x4015,0x4016,0x4017,0x4018},
                {20,0x1401,0x1402,0x1403,0x1404,0x1405,0x1406,0x1407,0x1408,0x1409,0x140a,0x140b,0x140c,0x140d,0x140e,0x140f,0x1410,0x1411,0x1412,0x1413,0x1414,0x1415,0x1416,0x1417,0x1418},
        };*/

IfxEray_Header    headerSample2[NUM_OF_SLOT_MAX_S2] = {
        /* fid  cyc cha    chb               buffer direction              ppit                   transmission mode               mbi                pl                      dp   startup   sync*/
            {2,  0,  TRUE, TRUE, IfxEray_BufferDirection_receive,  FALSE,  IfxEray_TransmissionMode_continuous, TRUE, DATA_PAYLOAD_LENGTH, 0x20, FALSE,  FALSE},
            {1,  0,  TRUE, TRUE, IfxEray_BufferDirection_transmit, FALSE,  IfxEray_TransmissionMode_continuous, TRUE, DATA_PAYLOAD_LENGTH, 0x40, TRUE,   TRUE},
            {3,  3,  TRUE, TRUE, IfxEray_BufferDirection_transmit,  FALSE,  IfxEray_TransmissionMode_continuous, TRUE, DATA_PAYLOAD_LENGTH, 0x60, FALSE,  FALSE},
            {4,  4,  TRUE, TRUE, IfxEray_BufferDirection_receive,  FALSE,  IfxEray_TransmissionMode_continuous, TRUE, DATA_PAYLOAD_LENGTH, 0x80, FALSE,  FALSE},
            //{61, 8,  TRUE, TRUE, IfxEray_BufferDirection_receive,  FALSE,  IfxEray_TransmissionMode_continuous, TRUE, DATA_PAYLOAD_LENGTH, 0xA0, FALSE,  FALSE},
        };
        /* slot buffer transmit configuration */
IfxEray_SlotConfig slotSample2[NUM_OF_SLOT_MAX_S2] = {
            /* header data   stxrh  bufferNum */
            {TRUE, TRUE,  TRUE,  1},
            {TRUE, TRUE,  TRUE,  0},
            {TRUE, TRUE,  TRUE,  2},
            {TRUE, TRUE,  TRUE,  3},
            //{TRUE, TRUE,  TRUE,  4},
        };

IfxEray_Header headerSample1[NUM_OF_SLOT_MAX_S1] = {
        /*  fid cyc cha    chb                 buffer direction            ppit                transmission mode                  mbi                    pl                  dp    startup  sync */
            {2,  0,  TRUE, TRUE, IfxEray_BufferDirection_transmit, FALSE,  IfxEray_TransmissionMode_continuous, TRUE, DATA_PAYLOAD_LENGTH, 0x20, TRUE,   TRUE},
            {1,  0,  TRUE, TRUE, IfxEray_BufferDirection_receive,  FALSE,  IfxEray_TransmissionMode_continuous, TRUE, DATA_PAYLOAD_LENGTH, 0x40, FALSE,  FALSE},
            {3,  3,  TRUE, TRUE, IfxEray_BufferDirection_receive, FALSE,  IfxEray_TransmissionMode_continuous, TRUE, DATA_PAYLOAD_LENGTH, 0x60, FALSE,  FALSE},
            {4,  4,  TRUE, TRUE, IfxEray_BufferDirection_transmit, FALSE,  IfxEray_TransmissionMode_continuous, TRUE, DATA_PAYLOAD_LENGTH, 0x80, FALSE,  FALSE},
            //{61, 8,  TRUE, TRUE, IfxEray_BufferDirection_transmit, FALSE,  IfxEray_TransmissionMode_continuous, TRUE, DATA_PAYLOAD_LENGTH, 0xA0, FALSE,  FALSE},
        };

IfxEray_SlotConfig slotSample1[NUM_OF_SLOT_MAX_S1] = {
        /*  header data   stxrh  bufferNum */
            {TRUE, TRUE,  TRUE,  0},
            {TRUE, TRUE,  TRUE,  1},
            {TRUE, TRUE,  TRUE,  2},
            {TRUE, TRUE,  TRUE,  3},
            //{TRUE, TRUE,  TRUE,  4},
        };
#else
eraytxdata_t g_eraytxdata[NUM_OF_SLOT_MAX_S1]={
                {1,0x0101,0x0102,0x0103,0x0104,0x0105,0x0106,0x0107,0x0108,0x0109,0x010a,0x010b,0x010c,0x011d,0x010e,0x010f,0x0110,0x0111,0x0112,0x0113,0x0114,0x0115,0x0116,0x0117,0x0118},
                {2,0x0201,0x0202,0x0203,0x0204,0x0205,0x0206,0x0207,0x0208,0x0209,0x020a,0x020b,0x020c,0x020d,0x020e,0x020f,0x0210,0x0211,0x0212,0x0213,0x0214,0x0215,0x0216,0x0217,0x0218},
                {3,0x0301,0x0302,0x0303,0x0304,0x0305,0x0306,0x0307,0x0308,0x0309,0x030a,0x030b,0x030c,0x030d,0x030e,0x030f,0x0310,0x0311,0x0312,0x0313,0x0314,0x0315,0x0316,0x0317,0x0318},
                {4,0x0401,0x0402,0x0403,0x0404,0x0405,0x0406,0x0407,0x0408,0x0409,0x040a,0x040b,0x040c,0x040d,0x040e,0x040f,0x0410,0x0411,0x0412,0x0413,0x0414,0x0415,0x0416,0x0417,0x0418},
                {32,0x2001,0x2002,0x2003,0x2004,0x2005,0x2006,0x2007,0x2008,0x2009,0x200a,0x200b,0x200c,0x200d,0x200e,0x200f,0x2010,0x2011,0x2012,0x2013,0x2014,0x2015,0x2016,0x2017,0x2018},
                {48,0x3001,0x3002,0x3003,0x3004,0x3005,0x3006,0x3007,0x3008,0x3009,0x300a,0x300b,0x300c,0x300d,0x300e,0x300f,0x3010,0x3011,0x3012,0x3013,0x3014,0x3015,0x3016,0x3017,0x3018},
                {64,0x4001,0x4002,0x4003,0x4004,0x4005,0x4006,0x4007,0x4008,0x4009,0x400a,0x400b,0x400c,0x400d,0x400e,0x400f,0x4010,0x4011,0x4012,0x4013,0x4014,0x4015,0x4016,0x4017,0x4018},
                {20,0x1401,0x1402,0x1403,0x1404,0x1405,0x1406,0x1407,0x1408,0x1409,0x140a,0x140b,0x140c,0x140d,0x140e,0x140f,0x1410,0x1411,0x1412,0x1413,0x1414,0x1415,0x1416,0x1417,0x1418},
        };
IfxEray_Header    headerSample2[NUM_OF_SLOT_MAX_S2];
IfxEray_SlotConfig slotSample2[NUM_OF_SLOT_MAX_S2];
IfxEray_Header headerSample1[NUM_OF_SLOT_MAX_S1];
IfxEray_SlotConfig slotSample1[NUM_OF_SLOT_MAX_S1];
#endif
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
void ERAY_irqInt0Handler(void);
void ERAY_irqInt1Handler(void);
void ERAY_irqNdat1Handler(void);
void IfxEray_Init(void);
void IfxEray_Run(void);
/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
IFX_INTERRUPT(ERAY_irqInt0Handler, 0, ISR_PRIORITY_ERAY_INT0);
IFX_INTERRUPT(ERAY_irqInt1Handler, 0, ISR_PRIORITY_ERAY_INT1);
IFX_INTERRUPT(ERAY_irqNdat1Handler, 0, ISR_PRIORITY_ERAY_NDAT1);

/*void update_eray_send_data(uint16 fid,uint16 *pdata , uint16 length){
    int i=0;
    while(g_eraytxdata[i].frameId!=0){
        if(g_eraytxdata[i].frameId==fid){
            for (int k=0; k<length; k++){
                g_eraytxdata[i].txdata[k]=pdata[k];
                DEBUG_PRINTF_ALWAYS(("get data:0x%x \r\n",g_eraytxdata[i].txdata[k]));
            }
        }
        i++;
    }
}*/
boolean load_default_eray_config(void)
{
    g_eraynoslotsample1=4;
    g_eraynoslotsample2=4;
    eraytxdata_t tmp_eraytxdata[NUM_OF_SLOT_MAX_S1]={
        {1,0x0101,0x0102,0x0103,0x0104,0x0105,0x0106,0x0107,0x0108,0x0109,0x010a,0x010b,0x010c,0x011d,0x010e,0x010f,0x0110,0x0111,0x0112,0x0113,0x0114,0x0115,0x0116,0x0117,0x0118},
        {2,0x0201,0x0202,0x0203,0x0204,0x0205,0x0206,0x0207,0x0208,0x0209,0x020a,0x020b,0x020c,0x020d,0x020e,0x020f,0x0210,0x0211,0x0212,0x0213,0x0214,0x0215,0x0216,0x0217,0x0218},
        {3,0x0301,0x0302,0x0303,0x0304,0x0305,0x0306,0x0307,0x0308,0x0309,0x030a,0x030b,0x030c,0x030d,0x030e,0x030f,0x0310,0x0311,0x0312,0x0313,0x0314,0x0315,0x0316,0x0317,0x0318},
        {4,0x0401,0x0402,0x0403,0x0404,0x0405,0x0406,0x0407,0x0408,0x0409,0x040a,0x040b,0x040c,0x040d,0x040e,0x040f,0x0410,0x0411,0x0412,0x0413,0x0414,0x0415,0x0416,0x0417,0x0418},
        {32,0x2001,0x2002,0x2003,0x2004,0x2005,0x2006,0x2007,0x2008,0x2009,0x200a,0x200b,0x200c,0x200d,0x200e,0x200f,0x2010,0x2011,0x2012,0x2013,0x2014,0x2015,0x2016,0x2017,0x2018},
        {48,0x3001,0x3002,0x3003,0x3004,0x3005,0x3006,0x3007,0x3008,0x3009,0x300a,0x300b,0x300c,0x300d,0x300e,0x300f,0x3010,0x3011,0x3012,0x3013,0x3014,0x3015,0x3016,0x3017,0x3018},
        {64,0x4001,0x4002,0x4003,0x4004,0x4005,0x4006,0x4007,0x4008,0x4009,0x400a,0x400b,0x400c,0x400d,0x400e,0x400f,0x4010,0x4011,0x4012,0x4013,0x4014,0x4015,0x4016,0x4017,0x4018},
        {20,0x1401,0x1402,0x1403,0x1404,0x1405,0x1406,0x1407,0x1408,0x1409,0x140a,0x140b,0x140c,0x140d,0x140e,0x140f,0x1410,0x1411,0x1412,0x1413,0x1414,0x1415,0x1416,0x1417,0x1418},
    };
    memcpy(&g_eraytxdata,&tmp_eraytxdata,sizeof(tmp_eraytxdata));

    IfxEray_Header    tmp_headerSample2[NUM_OF_SLOT_MAX_S2] = {
        /* fid  cyc cha    chb               buffer direction              ppit                   transmission mode               mbi                pl                      dp   startup   sync*/
        {2,  0,  TRUE, TRUE, IfxEray_BufferDirection_receive,  FALSE,  IfxEray_TransmissionMode_continuous, TRUE, DATA_PAYLOAD_LENGTH, 0x20, FALSE,  FALSE},
        {1,  0,  TRUE, TRUE, IfxEray_BufferDirection_transmit, FALSE,  IfxEray_TransmissionMode_continuous, TRUE, DATA_PAYLOAD_LENGTH, 0x40, TRUE,   TRUE},
        {3,  3,  TRUE, TRUE, IfxEray_BufferDirection_transmit,  FALSE,  IfxEray_TransmissionMode_continuous, TRUE, DATA_PAYLOAD_LENGTH, 0x60, FALSE,  FALSE},
        {4,  4,  TRUE, TRUE, IfxEray_BufferDirection_receive,  FALSE,  IfxEray_TransmissionMode_continuous, TRUE, DATA_PAYLOAD_LENGTH, 0x80, FALSE,  FALSE},
        //{61, 8,  TRUE, TRUE, IfxEray_BufferDirection_receive,  FALSE,  IfxEray_TransmissionMode_continuous, TRUE, DATA_PAYLOAD_LENGTH, 0xA0, FALSE,  FALSE},
    };
    memcpy(&headerSample2,&tmp_headerSample2,sizeof(tmp_headerSample2));

    /* slot buffer transmit configuration */
    IfxEray_SlotConfig tmp_slotSample2[NUM_OF_SLOT_MAX_S2] = {
        /* header data   stxrh  bufferNum */
        {TRUE, TRUE,  TRUE,  1},
        {TRUE, TRUE,  TRUE,  0},
        {TRUE, TRUE,  TRUE,  2},
        {TRUE, TRUE,  TRUE,  3},
        //{TRUE, TRUE,  TRUE,  4},
    };
    memcpy(&slotSample2,&tmp_slotSample2,sizeof(tmp_slotSample2));

    IfxEray_Header tmp_headerSample1[NUM_OF_SLOT_MAX_S1] = {
        /*  fid cyc cha    chb                 buffer direction            ppit                transmission mode                  mbi                    pl                  dp    startup  sync */
        {2,  0,  TRUE, TRUE, IfxEray_BufferDirection_transmit, FALSE,  IfxEray_TransmissionMode_continuous, TRUE, DATA_PAYLOAD_LENGTH, 0x20, TRUE,   TRUE},
        {1,  0,  TRUE, TRUE, IfxEray_BufferDirection_receive,  FALSE,  IfxEray_TransmissionMode_continuous, TRUE, DATA_PAYLOAD_LENGTH, 0x40, FALSE,  FALSE},
        {3,  3,  TRUE, TRUE, IfxEray_BufferDirection_receive, FALSE,  IfxEray_TransmissionMode_continuous, TRUE, DATA_PAYLOAD_LENGTH, 0x60, FALSE,  FALSE},
        {4,  4,  TRUE, TRUE, IfxEray_BufferDirection_transmit, FALSE,  IfxEray_TransmissionMode_continuous, TRUE, DATA_PAYLOAD_LENGTH, 0x80, FALSE,  FALSE},
        //{61, 8,  TRUE, TRUE, IfxEray_BufferDirection_transmit, FALSE,  IfxEray_TransmissionMode_continuous, TRUE, DATA_PAYLOAD_LENGTH, 0xA0, FALSE,  FALSE},
    };
    memcpy(&headerSample1,&tmp_headerSample1,sizeof(tmp_headerSample1));

    IfxEray_SlotConfig tmp_slotSample1[NUM_OF_SLOT_MAX_S1] = {
        /*  header data   stxrh  bufferNum */
        {TRUE, TRUE,  TRUE,  0},
        {TRUE, TRUE,  TRUE,  1},
        {TRUE, TRUE,  TRUE,  2},
        {TRUE, TRUE,  TRUE,  3},
        //{TRUE, TRUE,  TRUE,  4},
    };
    memcpy(&slotSample1,&tmp_slotSample1,sizeof(tmp_slotSample1));
    return TRUE;
}

void update_eray_send_data(struct qci_udp_packet *packet){
    int i=0,j=0;
    uint16 fid;
    fid=packet->frameid_h<<8|packet->frameid_l;
    while(g_eraytxdata[i].frameId!=0){
        if(g_eraytxdata[i].frameId==fid){
            for (int k=0; k<packet->eraylc; k=k+2){
                g_eraytxdata[i].txdata[j]=packet->flash_data[k+1]<<8|packet->flash_data[k];
                //DEBUG_PRINTF_ALWAYS(("i = %d, get data:0x%x \r\n",i,g_eraytxdata[i].txdata[j]));
                j++;
            }
        }
        i++;
    }
}
void eraynormal_to_udp(uint8 *data, uint8 ch, uint16 length, uint8 *udpPacket, uint8 cyclecnt,uint16 fid)
{
    struct qci_udp_packet *g_ErayUdpPacket = (struct qci_udp_packet *)udpPacket;
    ip_addr_t hostIpAddr;

    length=length*2;
    // UDP packet format: Host => VIU
    g_ErayUdpPacket->header.packet_direct = toHOST;
    if (board.aurix_index == MAIN_AURIX)
        g_ErayUdpPacket->header.target_viu =AURIX1; //source	
    else g_ErayUdpPacket->header.target_viu =AURIX2;
        g_ErayUdpPacket->header.target_mcu = udp_host; // don't care in case: Host => VIU
    g_ErayUdpPacket->header.bus_type = typeERAY;
    g_ErayUdpPacket->info_len = QCI_UDP_INFO_LEN + length;
    g_ErayUdpPacket->info_code = 0; //should be 0s if this Packet is sent by VCU to VIU
    //g_ErayUdpPacket->canid = 0;
    g_ErayUdpPacket->bus_id = ch;
    g_ErayUdpPacket->candlc = length; //bytes
    g_ErayUdpPacket->rs = 0;
    g_ErayUdpPacket->rs2 = udpSeqNum++;
    g_ErayUdpPacket->cyclecnt=cyclecnt;
    g_ErayUdpPacket->frameid_h=fid>>8;
    g_ErayUdpPacket->frameid_l=fid;
    if(g_Eray_run_mode==ERAY_NORMAL_MODE_TEST_S1)
        g_ErayUdpPacket->sample=1;
    else if(g_Eray_run_mode==ERAY_NORMAL_MODE_TEST_S2)
        g_ErayUdpPacket->sample=2;

    memcpy(g_ErayUdpPacket->payload, data, length);
    IP4_ADDR(&hostIpAddr, udp_ip[0], udp_ip[1], udp_ip[2], udp_ip[3]);
    udp_connect(gw_sendback_pcb, &hostIpAddr, QCI_UDP_SERVER_PORT);
    qci_udp_multi_send(gw_sendback_pcb, udpPacket, QCI_UDP_TOTAL_HEADER_LEN + length);
}

/** \brief Print a message from Eray if available
 *
 */
void Print_ErayNormalModeMsg(IfxStdIf_DPipe *io)
{
    uint8 udpPacket[QCI_UDP_TOTAL_HEADER_LEN + QCI_UDP_ERAY_PAYLOAD_SIZE];
    if(g_Eray.isUpdate){
        //ERAYNORMAL_ALWAYS_PRINTF(("print eray normal msg to udp. \r\n"));
        for(int i=0; i<NUM_OF_SLOT_MAX; i++){
            if(rxheader_list[i].frameId!=0){
                for(int j=0;j<NUMBER_ERAY_COMM_CYCLES;j++){
                    //eraynormal_to_udp((uint8 *)&g_Eray.receivedData[j][1][0], ERAY_NORMAL_RECV, g_eraytx_num, udpPacket,j,rxheader_list[i].frameId);
                    eraynormal_to_udp((uint8 *)&g_Eray.receivedData[j][rxheader_list[i].bufferindex][0], ERAY_NORMAL_RECV, 6, udpPacket,j,rxheader_list[i].frameId);
                    waitTime(10*TimeConst[TIMER_INDEX_1MS]); // this is just a placeholder value for delay
                    //DEBUG_PRINTF_ALWAYS(("send to udp %d  \r\n",j));
                }
            }
        }
        //eray_to_udp((uint8 *)g_ErayTestMode.receivedData_A, ERAY_RECV_CHAB, 8, udpPacket);
        //for(int n=0;n<g_eraytx_num;n++){
        //    DEBUG_PRINTF_ALWAYS(("0x%x  \r\n",g_Eray.receivedData[1][1][n]));
        //}
        g_Eray.isUpdate=FALSE;
    }
}

void eray_poc_state_monitor(void){

    /*if (g_Eray.ndatCount>=NUMBER_ERAY_COMM_CYCLES){
        g_eray_app_state=ERAY_APP_STATE_FINISH_COMMUNICATION;
    }*/
    if(g_eray_app_state == ERAY_APP_STATE_WAIT_ACTIVE_OR_PASSIVE){
    /* wait until communication Controller enters NORMAL_ACTIVE or NORMAL_PASSIVE state, exit if communication Controller enters HALT state. */
        IfxEray_PocState pocState;

        pocState = IfxEray_Eray_getPocState(&g_Eray.eray);
        if ((pocState == IfxEray_PocState_normalActive) || (pocState == IfxEray_PocState_normalPassive))
        {
            while (!IfxEray_Eray_setPocAllSlots(&g_Eray.eray))
            {
                printf("erayinit set all slots enter to active or passive...\r\n");
            }
            g_eray_app_state=ERAY_APP_STATE_ACTIVE_OR_PASSIVE;
        }
    }/*else if (g_eray_app_state ==ERAY_APP_STATE_FINISH_COMMUNICATION){
        g_eray_app_state = ERAY_APP_STATE_WAIT_ACTIVE_OR_PASSIVE;
        if (g_Eray_run_mode==ERAY_NORMAL_MODE_TEST_S2){
            AsclinShellInterface_execute_cmd("erayinit", "rx");
        }else if(g_Eray_run_mode==ERAY_NORMAL_MODE_TEST_S1){
            AsclinShellInterface_execute_cmd("erayinit", "tx");
        }
        DEBUG_PRINTF_ALWAYS(("eray enter reset state \r\n"));
    }*/
}

void parserHeader(IfxEray_Header *pheader, IfxEray_SlotConfig *pslot, uint8 num){
    int j=0;
    int k=0;
    for(int i=0;i<num;i++){
        if((pheader+i)->bufferDirection==IfxEray_BufferDirection_receive){
            //DEBUG_PRINTF_ALWAYS(("parser receive is %d \r\n", i));
            //DEBUG_PRINTF_ALWAYS(("parser frameid is %d \r\n", (pheader+i)->frameId));
            rxheader_list[j].enable=TRUE;
            rxheader_list[j].frameId=(pheader+i)->frameId;
            rxheader_list[j].bufferindex=(pslot+i)->bufferIndex;
            j++;
        }else if((pheader+i)->bufferDirection==IfxEray_BufferDirection_transmit){
            //DEBUG_PRINTF_ALWAYS(("parser transmit is %d \r\n", i));
            txheader_list[k].enable=TRUE;
            txheader_list[k].frameId=(pheader+i)->frameId;
            //DEBUG_PRINTF_ALWAYS(("parser frameId is %d, %d \r\n", k, (pheader+i)->frameId));
            txheader_list[k].bufferindex=(pslot+i)->bufferIndex;
            //DEBUG_PRINTF_ALWAYS(("parser bufferIndex is %d, %d \r\n", k, (pslot+i)->bufferIndex));
            //txheader_list[k].txdata[0]=256*(k+1);
            //txheader_list[k].txdata[1]=256*(k+1)+1;
            //find eray tx default frame id and data to assign into tx send list data
            for (int index=0;index<num;index++){
                if(txheader_list[k].frameId==g_eraytxdata[index].frameId){
                    for (int m=0;m<g_eraytx_num;m++){
                        txheader_list[k].txdata[m]=g_eraytxdata[index].txdata[m];
                        //DEBUG_PRINTF_ALWAYS(("i=%d, tx data [%d] = 0x%x \r\n",i,m,g_eraytxdata[i].txdata[m]));
                    }
                }
            }
            k++;
        }else{
            DEBUG_PRINTF_ALWAYS(("this isn't tx or rx config. \r\n"));
        }
    }
}

void clearErayTable(void){
     DEBUG_PRINTF_ALWAYS(("Clear S1 \r\n"));
     memset(&headerSample1, 0, sizeof(headerSample1));
     memset(&slotSample1, 0, sizeof(slotSample1));
     DEBUG_PRINTF_ALWAYS(("Clear S2 \r\n"));
     memset(&headerSample2, 0, sizeof(headerSample2));
     memset(&slotSample2, 0, sizeof(slotSample2));
}

void showErayTable(void){
    DEBUG_PRINTF_ALWAYS(("S1 Table \r\n"));
    DEBUG_PRINTF_ALWAYS(("Slot    FrameID    T(1)/R(0)    Cycle\r\n"));
    for(int i=0;i<NUM_OF_SLOT_MAX_S1;i++){
        DEBUG_PRINTF_ALWAYS(("[%d]:       %d          %d          %d\r\n",i,headerSample1[i].frameId,headerSample1[i].bufferDirection,headerSample1[i].cycleCode));
    }
    DEBUG_PRINTF_ALWAYS(("S2 Table \r\n"));
    DEBUG_PRINTF_ALWAYS(("Slot    FrameID    T(1)/R(0)    Cycle\r\n"));
    for(int i=0;i<NUM_OF_SLOT_MAX_S2;i++){
        DEBUG_PRINTF_ALWAYS(("[%d]:       %d          %d          %d\r\n",i,headerSample2[i].frameId,headerSample2[i].bufferDirection,headerSample2[i].cycleCode));
    }
}

void showErayState(){
    if(g_eray_app_state!=ERAY_APP_STATE_IDLE){
        DEBUG_PRINTF_ALWAYS(("Eray POC state is %d \r\n",IfxEray_Eray_getPocState(&g_Eray.eray)));
    }
}
/** \brief Handle Eray INT 0 service line interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_ERAY_INT0
 * \isrPriority \ref ISR_PRIORITY_ERAY_INT0
 *
 */
void ERAY_irqInt0Handler(void)
{
    if(g_Eray_run_mode==ERAY_NORMAL_MODE_TEST_S2||g_Eray_run_mode==ERAY_NORMAL_MODE_TEST_S1){
        /*Get the error interrupt flags*/
        Ifx_ERAY_EIR ErrIntStat = IfxEray_Eray_getErrorInterrupts(&g_Eray.eray);
        /* Get the status interrupt flags*/
        Ifx_ERAY_SIR StatusIntStat = IfxEray_Eray_getStatusInterrupts(&g_Eray.eray);
        if (StatusIntStat.B.SDS)
        {
            /* SDS must be cleared for getting the next interrupt   */
            IfxEray_Eray_clearStatusFlag(&g_Eray.eray, IfxEray_ClearStatusFlag_sds);
        }
        if (StatusIntStat.B.CYCS)
        {
            /* CYCS must be cleared for getting the next interrupt   */
            IfxEray_Eray_clearStatusFlag(&g_Eray.eray, IfxEray_ClearStatusFlag_cycs);
        }
        if (StatusIntStat.B.SUCS)
        {
            /* SUCS must be cleared for getting the next interrupt   */
            IfxEray_Eray_clearStatusFlag(&g_Eray.eray, IfxEray_ClearStatusFlag_sucs);
        }
        if (StatusIntStat.B.TIBC)
        {
            /* TIBC must be cleared for getting the next interrupt   */
            IfxEray_Eray_clearStatusFlag(&g_Eray.eray, IfxEray_ClearStatusFlag_tibc);
        }
        if (ErrIntStat.B.MHF)
        {
            /* clear flag MHF */
            IfxEray_Eray_clearErrorFlag(&g_Eray.eray, IfxEray_ClearErrorFlag_mhf);
        }
    }
    else if(g_Eray_run_mode==ERAY_TEST_MODE){
        /*Get the error interrupt flags*/
        Ifx_ERAY_EIR ErrIntStat = IfxEray_Eray_getErrorInterrupts(&g_ErayTestMode.eray);
        /* Get the status interrupt flags*/
        Ifx_ERAY_SIR StatusIntStat = IfxEray_Eray_getStatusInterrupts(&g_ErayTestMode.eray);
        if (StatusIntStat.B.SDS)
        {
            /* SDS must be cleared for getting the next interrupt   */
            IfxEray_Eray_clearStatusFlag(&g_ErayTestMode.eray, IfxEray_ClearStatusFlag_sds);
        }
        if (StatusIntStat.B.CYCS)
        {
            /* CYCS must be cleared for getting the next interrupt   */
            IfxEray_Eray_clearStatusFlag(&g_ErayTestMode.eray, IfxEray_ClearStatusFlag_cycs);
        }
        if (StatusIntStat.B.SUCS)
        {
            /* SUCS must be cleared for getting the next interrupt   */
            IfxEray_Eray_clearStatusFlag(&g_ErayTestMode.eray, IfxEray_ClearStatusFlag_sucs);
        }
        if (StatusIntStat.B.TIBC)
        {
            /* TIBC must be cleared for getting the next interrupt   */
            IfxEray_Eray_clearStatusFlag(&g_ErayTestMode.eray, IfxEray_ClearStatusFlag_tibc);
        }
        if (StatusIntStat.B.TXI)
        {
            /* TXI must be cleared for getting the next interrupt   */
            IfxEray_Eray_clearStatusFlag(&g_ErayTestMode.eray, IfxEray_ClearStatusFlag_txi);
        }
        if (ErrIntStat.B.MHF)
        {
            g_ErayTestMode.errors++;
            /* clear flag MHF */
            IfxEray_Eray_clearErrorFlag(&g_ErayTestMode.eray, IfxEray_ClearErrorFlag_mhf);
        }
    }
}

/** \brief Handle Eray INT 1 service line interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_ERAY_INT1
 * \isrPriority \ref ISR_PRIORITY_ERAY_INT1
 *
 */
void ERAY_irqInt1Handler(void)
{
    if(g_Eray_run_mode==ERAY_NORMAL_MODE_TEST_S2||g_Eray_run_mode==ERAY_NORMAL_MODE_TEST_S1){
        /*Get the status interrupt flags*/
        Ifx_ERAY_SIR StatusIntStat = IfxEray_Eray_getStatusInterrupts(&g_Eray.eray);
        if (StatusIntStat.B.TOBC)
        {
            /* TOBC must be cleared for getting the next interrupt */
            IfxEray_Eray_clearStatusFlag(&g_Eray.eray, IfxEray_ClearStatusFlag_tobc);
        }
    }else if(g_Eray_run_mode==ERAY_TEST_MODE)
    {
        /*Get the status interrupt flags*/
        Ifx_ERAY_SIR StatusIntStat = IfxEray_Eray_getStatusInterrupts(&g_ErayTestMode.eray);
        if (StatusIntStat.B.TOBC)
        {
            /* TOBC must be cleared for getting the next interrupt */
            IfxEray_Eray_clearStatusFlag(&g_ErayTestMode.eray, IfxEray_ClearStatusFlag_tobc);
        }
    }
}


/** \brief Handle Eray New Data 1 service line interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_ERAY_NDAT1
 * \isrPriority \ref ISR_PRIORITY_ERAY_NDAT1
 *
 *
 * ERAY_irqNdat1Handler update data to be sent in every slot
 *
 */

void ERAY_irqNdat1Handler(void)
{
    if(g_Eray_run_mode==ERAY_NORMAL_MODE_TEST_S2||g_Eray_run_mode==ERAY_NORMAL_MODE_TEST_S1){
        IfxEray_Eray_ReceiveControl config;
        if (g_Eray.ndatCount < NUMBER_ERAY_COMM_CYCLES)
        {
            for(int i=0; i<NUM_OF_SLOT_MAX; i++){
                if(rxheader_list[i].enable==TRUE){
                    if (IfxEray_Eray_getNewDataInterruptStatus(&g_Eray.eray, rxheader_list[i].bufferindex) == 1){
                        config.headerReceived   = FALSE;
                        config.dataReceived     = TRUE;
                        config.receiveRequested = TRUE;
                        config.swapRequested    = TRUE;
                        config.bufferIndex      = rxheader_list[i].bufferindex;
                        IfxEray_Eray_receiveFrame(&g_Eray.eray, &config);
                        IfxEray_Eray_readData(&g_Eray.eray, &g_Eray.receivedData[g_Eray.ndatCount][rxheader_list[i].bufferindex][0], g_eraytx_num);
                        //DEBUG_PRINTF_ALWAYS(("%d, %d\r\n", g_Eray.ndatCount, rxheader_list[i].bufferindex));
                        //DEBUG_PRINTF_ALWAYS(("cycle %d, frameID %d:  0x%x  0x%x\r\n",g_Eray.ndatCount, rxheader_list[i].frameId, 
                        //    g_Eray.receivedData[g_Eray.ndatCount][rxheader_list[i].bufferindex][0], 
                        //    g_Eray.receivedData[g_Eray.ndatCount][rxheader_list[i].bufferindex][1]));
                        //DEBUG_PRINTF_ALWAYS(("cycle %d, frameID %d:  \r\n",g_Eray.ndatCount, rxheader_list[i].frameId));
                        //for(int n=0;n<g_eraytx_num;n++){
                        //DEBUG_PRINTF_ALWAYS(("0x%x  \r\n", g_Eray.receivedData[g_Eray.ndatCount][rxheader_list[i].bufferindex][g_eraytx_num-1]));
                        //}
                    }
                }else
                    break;
            }
            for(int i=0; i<NUM_OF_SLOT_MAX; i++){
                //DEBUG_PRINTF_ALWAYS(("send %d th \r\n",i));
                if(txheader_list[i].enable==TRUE){
                    //DEBUG_PRINTF_ALWAYS(("sending %d th \r\n",i));
                    IfxEray_Eray_writeData(&g_Eray.eray, &txheader_list[i].txdata[0], g_eraytx_num);
                    //DEBUG_PRINTF_ALWAYS(("slot %d send :0x%x \r\n",txheader_list[i].bufferindex,txheader_list[i].txdata[0]));
                    //DEBUG_PRINTF_ALWAYS(("slot %d send :0x%x \r\n",txheader_list[i].bufferindex,txheader_list[i].txdata[1]));
                    IfxEray_Eray_TransmitControl config;
                    config.headerTransfered  = FALSE;
                    config.dataTransfered    = TRUE;
                    config.transferRequested = TRUE;
                    config.bufferIndex       = txheader_list[i].bufferindex;
                    IfxEray_Eray_transmitFrame(&g_Eray.eray, &config);
                    //txheader_list[i].txdata[0] = txheader_list[i].txdata[1]+1;
                    //txheader_list[i].txdata[1] = txheader_list[i].txdata[0]+1;
                }else
                    break;
            }
            g_Eray.ndatCount++;
            if(g_Eray.ndatCount>=NUMBER_ERAY_COMM_CYCLES){
                g_Eray.isUpdate=TRUE;
                //g_Eray.ndatCount=0;
            }
        }
    }
    else if(g_Eray_run_mode==ERAY_TEST_MODE)
    {
        /* message received channel A */
        if (IfxEray_Eray_getNewDataInterruptStatus(&g_ErayTestMode.eray, RECEIVE_BUFFER_A_INDEX) == 1)
        {
            IfxEray_Eray_ReceiveControl config;
            config.headerReceived   = FALSE;
            config.dataReceived     = TRUE;
            config.receiveRequested = TRUE;
            config.swapRequested    = TRUE;
            config.bufferIndex      = RECEIVE_BUFFER_A_INDEX;
            IfxEray_Eray_receiveFrame(&g_ErayTestMode.eray, &config);
            IfxEray_Eray_readData(&g_ErayTestMode.eray, &g_ErayTestMode.receivedData_A[0], 2 * 2);
            g_ErayTestMode.ndat_A = TRUE;
        }
        /* message received channel B */
        if (IfxEray_Eray_getNewDataInterruptStatus(&g_ErayTestMode.eray, RECEIVE_BUFFER_B_INDEX) == 1)
        {
            IfxEray_Eray_ReceiveControl config;
            config.headerReceived   = FALSE;
            config.dataReceived     = TRUE;
            config.receiveRequested = TRUE;
            config.swapRequested    = TRUE;
            config.bufferIndex      = RECEIVE_BUFFER_B_INDEX;
            IfxEray_Eray_receiveFrame(&g_ErayTestMode.eray, &config);
            IfxEray_Eray_readData(&g_ErayTestMode.eray, &g_ErayTestMode.receivedData_B[0], 2 * 2);
            g_ErayTestMode.ndat_B = TRUE;
        }
    }
}

void IfxEray_Init(void)
{
    /* disable interrupts */
    boolean interruptState = IfxCpu_disableInterrupts();
    int     i;

    g_Eray.ndatCount = 0;

    /* create default module config */
    IfxEray_Eray_initModuleConfig(&g_Eray.erayModuleConfig, &MODULE_ERAY0);
    

    /* set interrupt priorities */
    g_Eray.erayModuleConfig.interrupt.int0Priority  = ISR_PRIORITY_ERAY_INT0;
    g_Eray.erayModuleConfig.interrupt.int1Priority  = ISR_PRIORITY_ERAY_INT1;
    g_Eray.erayModuleConfig.interrupt.ndat1Priority = ISR_PRIORITY_ERAY_NDAT1;

    /* initialize ERAY module */
    IfxEray_Eray_initModule(&g_Eray.eray, &g_Eray.erayModuleConfig);

    /* create ERAY node default configuration */
    IfxEray_Eray_Node_initConfig(&g_Eray.nodeConfig);

    /* ERAY node suppled configurations */
    /* Messages*/
    g_Eray.nodeConfig.messageRAMConfig.frameIdFilter		  = 0x7FF;
    if (g_Eray_run_mode==ERAY_NORMAL_MODE_TEST_S2){
        g_Eray.nodeConfig.messageRAMConfig.numberOfMessageBuffers = g_eraynoslotsample2;
    }else{
        g_Eray.nodeConfig.messageRAMConfig.numberOfMessageBuffers = g_eraynoslotsample1;
    }
    g_Eray.nodeConfig.messageRAMConfig.firstDynamicBuffer     = 0x80;
    g_Eray.nodeConfig.messageRAMConfig.fifoBufferStartIndex   = 0x80;

    g_Eray.nodeConfig.controllerConfig.staticFramepayload     = DATA_PAYLOAD_LENGTH;
    g_Eray.nodeConfig.controllerConfig.gtuConfig.gtu01Config.microticksPerCycle = 0x30D40;
    g_Eray.nodeConfig.controllerConfig.gtuConfig.gtu02Config.macroticksPerCycle = 0x1388;
//    g_Eray.nodeConfig.controllerConfig.gtuConfig.gtu07Config.staticSlotLength = 0x01;
    g_Eray.nodeConfig.controllerConfig.gtuConfig.gtu07Config.staticSlotsCount   = 0x40;
//    g_Eray.nodeConfig.controllerConfig.gtuConfig.gtu08Config.dynamicSlotLength  = 0x00;
//    g_Eray.nodeConfig.controllerConfig.gtuConfig.gtu08Config.dynamicSlotCount   = 0x00;

    //=======================================================================================================
    //     Frame header parameters
    //=======================================================================================================
    //     * fid = Frame ID
    //	   * cyc = Cycle counter
    //     * cha/chb = channel A/B
    //     * buffer direction = transmit/receive (selects buffer as a transmit buffer or as a receive buffer)
    //     * ppit = transmitPayloadIndicatior
    //     * transmission Mode = Single or Continuous
    //     * mbi= Buffer Service Request Enable
    //     * pl = Payload (length of data section)
    //	   * dp = pointer to the data section of message buffer in RAM
    //     * startup = startup frame is indicated or not
    //	   * sync = sync frame is indicated or not
    //=======================================================================================================
    if (g_Eray_run_mode==ERAY_NORMAL_MODE_TEST_S2){
        for (i = 0; i < g_eraynoslotsample2; i++)
        {
            g_Eray.nodeConfig.messageRAMConfig.header[i]      = &headerSample2[i];
            g_Eray.nodeConfig.messageRAMConfig.slotControl[i] = &slotSample2[i];
            g_Eray.nodeConfig.messageRAMConfig.data[i]        = NULL_PTR;
        }
        parserHeader(&headerSample2,&slotSample2,g_eraynoslotsample2);
    }else if(g_Eray_run_mode==ERAY_NORMAL_MODE_TEST_S1){
        for (i = 0; i < g_eraynoslotsample1; i++)
        {
            g_Eray.nodeConfig.messageRAMConfig.header[i]      = &headerSample1[i];
            g_Eray.nodeConfig.messageRAMConfig.slotControl[i] = &slotSample1[i];
            g_Eray.nodeConfig.messageRAMConfig.data[i]        = NULL_PTR;
        }
        parserHeader(&headerSample1,&slotSample1,g_eraynoslotsample1);
    }
    /* channel A pins */
    const IfxEray_Eray_NodeA nodeAPins = {
        IfxPort_InputMode_pullDown,
        IfxPort_OutputMode_pushPull,
        IfxPort_PadDriver_cmosAutomotiveSpeed3,
        IfxPort_OutputMode_pushPull,
        &IfxEray0_RXDA2_P02_1_IN,//&IfxEray0_RXDA0_P14_8_IN,   //&IfxEray0_RXDA2_P02_1_IN,
        &IfxEray0_TXDA_P02_0_OUT,//&IfxEray0_TXDA_P14_10_OUT, //&IfxEray0_TXDA_P02_0_OUT,
        &IfxEray0_TXENA_P02_4_OUT//&IfxEray0_TXENA_P14_9_OUT   //&IfxEray0_TXENA_P02_4_OUT
    };
    /* channel B pins */
    const IfxEray_Eray_NodeB nodeBPins = {
        IfxPort_InputMode_pullDown,
        IfxPort_OutputMode_pushPull,
        IfxPort_PadDriver_cmosAutomotiveSpeed3,
        IfxPort_OutputMode_pushPull,
        &IfxEray0_RXDB2_P02_3_IN,//&IfxEray0_RXDB0_P14_7_IN,
        &IfxEray0_TXDB_P02_2_OUT,//&IfxEray0_TXDB_P14_5_OUT,
        &IfxEray0_TXENB_P02_5_OUT//&IfxEray0_TXENB_P14_6_OUT
    };

    const IfxEray_Eray_Pins  pins = {(IfxEray_Eray_NodeA *)&nodeAPins, (IfxEray_Eray_NodeB *)&nodeBPins};
    g_Eray.nodeConfig.pins = (IfxEray_Eray_Pins *)&pins;

    /* ERAY node initialization with supplied configuration */
    IfxEray_Eray_Node_init(&g_Eray.eray, &g_Eray.nodeConfig);

    /* ALL NDAT interrupts are routed to SRC_ERAYNDAT1 interrupt service line */
    IfxEray_Eray_setNewDataInterruptDestination(&g_Eray.eray, 0, 1); // New Data #0 -> NDAT1
    IfxEray_Eray_setNewDataInterruptDestination(&g_Eray.eray, 1, 1); // New Data #1 -> NDAT1  //+++++destination

    /* enable interrupts again */
    IfxCpu_restoreInterrupts(interruptState);
   // printf("RX Ready for transfer...\r\n");
    /* wait for wake-up pattern on default configured channel ( IfxEray_Channel_a )*/
//    while (IfxEray_Eray_getWakeupPatternReceivedChannel(&g_Eray.eray) != IfxEray_WakeupChannel_b)
//    {}
    //printf("RX Ready for transfer a...\r\n");

    //=======================================================================================================
    // Communication Controller is in Ready State
    //=======================================================================================================

    /* allow node to cold-start and wait until cold-start is successful*/
    while (!IfxEray_Eray_allowColdStart(&g_Eray.eray))
    {}
    //printf("RX Ready for transfer b...\r\n");

    //=======================================================================================================
    // Communication Controller is in cold-start resolution
    //=======================================================================================================

    /* start the communication in cluster and wait until communication is successful */
    while (!IfxEray_Eray_startCommunication(&g_Eray.eray))
    {}
    //printf("RX Ready for transfer c...\r\n");
    //if (g_Eray_run_mode==ERAY_NORMAL_MODE_TEST_S2){
    //    printf("Sample2 Ready for transfer...\r\n");
    //}else if(g_Eray_run_mode==ERAY_NORMAL_MODE_TEST_S1){
    //    printf("Sample1 Ready for transfer...\r\n");
    //}
    g_eray_app_state = ERAY_APP_STATE_WAIT_ACTIVE_OR_PASSIVE;
}

void IfxEray_Run(void)
{
    unsigned prevReceivedDataCounter = 0;

    /* wait until all data received in all cycles */
    while (1)
    {
        if (g_Eray.ndatCount != prevReceivedDataCounter)
        {
            prevReceivedDataCounter = g_Eray.ndatCount;
            printf("transfer counter: %d\r\n", prevReceivedDataCounter);
            //printf("SCCA : %d \r\n",ERAY0_SCV.B.SCCA);
            //printf("SCCB : %d \r\n",ERAY0_SCV.B.SCCB);
            if (g_Eray.ndatCount >= NUMBER_ERAY_COMM_CYCLES)
            {
                g_Eray.ndatCount = 0;
                break;
            }
        }
    }
}

