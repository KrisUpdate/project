#include "SysSe/Bsp/Bsp.h"
#include <cangateway.h>
#include <Configuration.h>
#include <AsclinShellInterface.h>
#include <ConfigurationIsr_pagasus2.h>
#include "Mcmcan.h"
#include "MulticanCmds.h"
#include "os.h"
#include "pkg/com/com/inc/ee_com.h"
#include "pkg/com/com/inc/message_type.h"
#include <stdlib.h>
#include "board.h"
#include "qci_udp_packet.h"
#include "config_pg2_feasures.h"

extern App_Multican g_Multican;
extern const t_node_config NODE_CONFIG[];
extern Ifx_CAN * can_used_module[CAN_USED_MODULES_NUM];
extern uint8 canMode;

extern uint32 rxData0[16];
extern uint32 rxData1[16];
extern uint32 rxData2[16];
extern uint32 rxData3[16];
extern uint32 rxData4[16];
extern uint32 rxData5[16];
extern uint32 rxCnt[CAN_USED_NODES_NUM];


#define NODE0_RAM_OFFSET 0x0
#define NODE1_RAM_OFFSET (NODE0_RAM_OFFSET + 0x1000)
#define NODE2_RAM_OFFSET (NODE1_RAM_OFFSET + 0x1000)
#define NODE3_RAM_OFFSET (NODE2_RAM_OFFSET + 0x1000)
#define CAN_WAIT_TIME_MS    100

volatile boolean cangen_enable = FALSE;
//boolean caninit_flag = FALSE;
IFX_EXTERN App_AsclinShellInterface g_AsclinShellInterface;
IfxCan_Can can[3];
IfxCan_Can_Node canNode[CAN_PORT_NUM];
boolean queue_dequeue_all_print_flag = FALSE;

extern boolean extendedFrame;
extern uint32 fastBaudRate;
extern uint32 samplepoint_standard;
extern uint32 samplepoint_fast;
extern uint8 tranceiver_delay_offset;
extern uint32 noniso_mode;
// Here for cangen parameter
// stm2cnt greater than cangen_time_ms is terminal condition
// cnt use for can data payload automatically increment.
// can_num_per_ms define how many frames per millisecond
static uint32 stm2cnt = 0;
static uint32 cangen_time_ms = 1000;
static uint32 can_num_per_ms = 1;
static uint32 cangen_node_id = 0;
static uint32 cangen_msg_id = 1;
static uint32 cangen_recv_count_Can0 = 0;
static uint32 cangen_recv_count_Can1 = 0;
static uint32 cangen_recv_count_Can2 = 0;
static uint32 cangen_recv_count_Can3 = 0;
static uint32 cangen_recv_count_Can4 = 0;
static uint32 cangen_recv_count_Can5 = 0;
static uint16 cangen_index=1;
uint16 cangen_queue_index=1;
static uint16 cangen_cancnt = 0;
uint32 cangen_recv_queue_count[6] = {0};
static uint16 cangen_error_cnt0=0;
static uint16 cangen_error_cnt1=0;
static uint16 cangen_error_cnt2=0;
static uint16 cangen_error_cnt3=0;
static uint16 cangen_error_cnt4=0;
static uint16 cangen_error_cnt5=0;
uint16 cangen_queue_error_cnt[6]={0};
boolean cangen_fix_count;
boolean cangen_dual_test;
static uint32 cangen_nbytes=8;

round_trip_measure_t rt_sample;
struct{
    //int cancan_index;
    //Ifx_CAN *can;
    //IfxCan_NodeId    nodeId;
    //IfxCan_Txd_Out    *txPin;
    //IfxCan_Rxd_In     *rxPin;
    //uint32 canFilterId;
    Ifx_Priority rxPriority;
    IfxCan_InterruptLine rxInterruptLine;
    IfxSrc_Tos tos_num;
    //boolean       bDataIn;
}V33_Can_Int[CAN_USED_NODES_NUM] = {
       {/*0,&MODULE_CAN0, IfxCan_NodeId_3, &IfxCan_TXD03_P00_2_OUT, &IfxCan_RXD03A_P00_3_IN, 0,*/ISR_PRIORITY_CAN0_RX, IfxCan_InterruptLine_0,IfxSrc_Tos_cpu1/*, FALSE*/},
       {/*2,&MODULE_CAN2, IfxCan_NodeId_3, &IfxCan_TXD23_P14_9_OUT, &IfxCan_RXD23A_P14_10_IN, 1,*/ISR_PRIORITY_CAN1_RX, IfxCan_InterruptLine_1,IfxSrc_Tos_cpu1/*, FALSE*/},
       {/*0,&MODULE_CAN0, IfxCan_NodeId_1, &IfxCan_TXD01_P01_3_OUT, &IfxCan_RXD01C_P01_4_IN, 2,*/ISR_PRIORITY_CAN2_RX, IfxCan_InterruptLine_2,IfxSrc_Tos_cpu2/*, FALSE*/},
       {/*1,&MODULE_CAN1, IfxCan_NodeId_3, &IfxCan_TXD13_P14_6_OUT, &IfxCan_RXD13A_P14_7_IN, 3,*/ISR_PRIORITY_CAN3_RX, IfxCan_InterruptLine_3,IfxSrc_Tos_cpu2/*, FALSE*/},
       {/*1,&MODULE_CAN1, IfxCan_NodeId_1, &IfxCan_TXD11_P00_4_OUT, &IfxCan_RXD11B_P00_5_IN, 4,*/ISR_PRIORITY_CAN4_RX, IfxCan_InterruptLine_4,IfxSrc_Tos_cpu5/*, FALSE*/},
       {/*1,&MODULE_CAN1, IfxCan_NodeId_0, &IfxCan_TXD10_P00_0_OUT, &IfxCan_RXD10A_P00_1_IN, 5,*/ISR_PRIORITY_CAN5_RX, IfxCan_InterruptLine_5,IfxSrc_Tos_cpu5/*, FALSE*/},
};

const uint32 CanGateway_FILTER_ID[6]=
{
    0, //CAN_PHY_A
    0, //CAN_PHY_B
    0, //CAN_PHY_C
    0, //CAN_PHY_D
    0, //CAN_PHY_E
    0, //CAN_PHY_F
};

/* define the filter ID for each nodes (0~5) */
const uint32 CanGateway_FILTER_ID_F[6]=
{
    1, //CAN_PHY_A
    2, //CAN_PHY_B
    3, //CAN_PHY_C
    4, //CAN_PHY_D
    5, //CAN_PHY_E
    6, //CAN_PHY_F
};

static uint8 dlc[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64};

queue_t* g_can_queue[6];
extern queue_t can_queue0;
extern queue_t can_queue1;
extern queue_t can_queue2;
extern queue_t can_queue3;
extern queue_t can_queue4;
extern queue_t can_queue5;

extern element_t can_elements0[CAN_QUEUE_ELEMENT_SIZE];
extern element_t can_elements1[CAN_QUEUE_ELEMENT_SIZE];
extern element_t can_elements2[CAN_QUEUE_ELEMENT_SIZE];
extern element_t can_elements3[CAN_QUEUE_ELEMENT_SIZE];
extern element_t can_elements4[CAN_QUEUE_ELEMENT_SIZE];
extern element_t can_elements5[CAN_QUEUE_ELEMENT_SIZE];

extern uint8 udp_host;
extern uint16 udpSeqNum;

void convert_can_to_edata(queue_t* p_can_queue, int canBusId, uint32 id, uint8 *data, uint8 length,element_t* edata)
{
    struct qci_udp_packet udpPacket;
    // UDP packet format: Host => VIU
    udpPacket.header.packet_direct = toHOST;
    if (board.aurix_index == MAIN_AURIX)
            udpPacket.header.target_viu = AURIX1; //source
    else udpPacket.header.target_viu = AURIX2;
    udpPacket.header.target_mcu = udp_host;//target
    udpPacket.header.bus_type = typeCAN;
    udpPacket.info_len = QCI_UDP_INFO_LEN + length;
    //!CheckQueueRem(senderUDPMsg)
    if (queue_is_full(p_can_queue)) udpPacket.info_code = MCU_queue_full;
    else udpPacket.info_code = MCU_scheduled_successfully | (p_can_queue->capacity-queue_get_size(p_can_queue));
    udpPacket.info_code = MCU_scheduled_successfully;
    udpPacket.canid = id; //message id
    udpPacket.bus_id = canBusId;
    udpPacket.candlc = length;
    udpPacket.rs = 0;
    udpPacket.rs2 = udpSeqNum;
    memcpy(edata->udp_content, &udpPacket, QCI_UDP_TOTAL_HEADER_LEN);
    memcpy(&(edata->udp_content[QCI_UDP_TOTAL_HEADER_LEN]), data, length);
}

IFX_INTERRUPT(CAN_irqRxCan0Handler, 1, ISR_PRIORITY_CAN0_RX)
{
#if CANGATEWAYDEBUG == 1
    cangen_recv_count_Can0 = cangen_recv_count_Can0 + 1;
#endif
    IfxCan_Message rxMsg;
    uint8 length;
    IfxCan_Can_initMessage(&rxMsg);
    rxMsg.readFromRxFifo0 = TRUE;
    rxMsg.bufferNumber = 0;
    //while (IfxCan_Node_isRxBufferNewDataUpdated(g_CanBasic.drivers.canNode[i].node, (IfxCan_RxBufferId)rxMsg.bufferNumber) == 0) {}
    IfxCan_Can_readMessage(&g_Multican.drivers.node[0], &rxMsg, rxData0);
    //IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"messageid: %d/n"ENDL,rxMsg.messageId);
    length = dlc[rxMsg.dataLengthCode];
#if CANGATEWAYDEBUG == 1
    //if (cangen_fix_count)
    {
      uint8 nquadlets = length/8;
      int i;
      for (i=0;i<nquadlets;i++){
        uint32 rxData[2];
        rxData[0]   = rxData0[i*2];
        rxData[1]   = rxData0[(i*2)+1];
        //IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"queue_dequeue_all[%d]: 0x%X 0x%X"ENDL,i,rxData[0], rxData[1]);
        if ((rxData[0] != 0xAAAAAAAA) || (rxData[1] != 0x55555555)){
          cangen_error_cnt0 = cangen_error_cnt0 + 1;
          break;
        }
      }
    }
#endif
    element_t edata;
    convert_can_to_edata(g_can_queue[0],0, rxMsg.messageId, (uint8 *)rxData0, length,&edata);
    //IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"CAN_irqRxCan0Handler push:%d"ENDL,(edata.udp_content[19] << 8) | edata.udp_content[18]);
    queue_enqueue(g_can_queue[0], edata);
    //can_to_udp(0, rxMsg.messageId, (uint8 *)rxData, length);
    /* Clear transmission completed interrupt flag */


    rxCnt[0]++;
    // calculate throughput time
    receive_time = (uint32)(now() / TimeConst_1us);

    IfxCan_Node_clearInterruptFlag(g_Multican.drivers.node[0].node, IfxCan_Interrupt_rxFifo0NewMessage);
    if(rt_sample.enabled == TRUE){
       rt_sample.point_d = now();
    }
}

IFX_INTERRUPT(CAN_irqRxCan1Handler, 1, ISR_PRIORITY_CAN1_RX)
{
#if CANGATEWAYDEBUG == 1
    cangen_recv_count_Can1 = cangen_recv_count_Can1 + 1;
#endif
    IfxCan_Message rxMsg;
    uint8 length;
    IfxCan_Can_initMessage(&rxMsg);
    rxMsg.readFromRxFifo0 = TRUE;
    rxMsg.bufferNumber = 0;
    IfxCan_Can_readMessage(&g_Multican.drivers.node[1], &rxMsg, rxData1);
    //IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"messageid: %d/n"ENDL,rxMsg.messageId);
    length = dlc[rxMsg.dataLengthCode];
#if CANGATEWAYDEBUG == 1
    //if (cangen_fix_count)
    {
      uint8 nquadlets = length/8;
      int i;
      for (i=0;i<nquadlets;i++){
        uint32 rxData[2];
        rxData[0]   = rxData1[i*2];
        rxData[1]   = rxData1[(i*2)+1];
        //IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"queue_dequeue_all[%d]: 0x%X 0x%X"ENDL,i,rxData[0], rxData[1]);
        if ((rxData[0] != 0xAAAAAAAA) || (rxData[1] != 0x55555555)){
          cangen_error_cnt1 = cangen_error_cnt1 + 1;
          break;
        }
      }
    }
#endif
    element_t edata;
    convert_can_to_edata(g_can_queue[1],1, rxMsg.messageId, (uint8 *)rxData1, length,&edata);
    queue_enqueue(g_can_queue[1], edata);

    rxCnt[1]++;
    // calculate throughput time
    receive_time = (uint32)(now() / TimeConst_1us);

    IfxCan_Node_clearInterruptFlag(g_Multican.drivers.node[1].node, IfxCan_Interrupt_rxFifo0NewMessage);
    if(rt_sample.enabled == TRUE){
       rt_sample.point_d = now();
    }
}

IFX_INTERRUPT(CAN_irqRxCan2Handler, 2, ISR_PRIORITY_CAN2_RX)
{
#if CANGATEWAYDEBUG == 1
    cangen_recv_count_Can2 = cangen_recv_count_Can2 + 1;
#endif
    IfxCan_Message rxMsg;
    uint8 length;
    IfxCan_Can_initMessage(&rxMsg);
    rxMsg.readFromRxFifo0 = TRUE;
    rxMsg.bufferNumber = 0;
    IfxCan_Can_readMessage(&g_Multican.drivers.node[2], &rxMsg, rxData2);
    //IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"messageid: %d/n"ENDL,rxMsg.messageId);
    length = dlc[rxMsg.dataLengthCode];
#if CANGATEWAYDEBUG == 1
    //if (cangen_fix_count)
    {
      uint8 nquadlets = length/8;
      int i;
      for (i=0;i<nquadlets;i++){
        uint32 rxData[2];
        rxData[0]   = rxData2[i*2];
        rxData[1]   = rxData2[(i*2)+1];
        //IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"queue_dequeue_all[%d]: 0x%X 0x%X"ENDL,i,rxData[0], rxData[1]);
        if ((rxData[0] != 0xAAAAAAAA) || (rxData[1] != 0x55555555)){
          cangen_error_cnt2 = cangen_error_cnt2 + 1;
          break;
        }
      }
    }
#endif
    element_t edata;
    convert_can_to_edata(g_can_queue[2],2, rxMsg.messageId, (uint8 *)rxData2, length,&edata);
    queue_enqueue(g_can_queue[2], edata);

    rxCnt[2]++;
    // calculate throughput time
    receive_time = (uint32)(now() / TimeConst_1us);

    IfxCan_Node_clearInterruptFlag(g_Multican.drivers.node[2].node, IfxCan_Interrupt_rxFifo0NewMessage);
    if(rt_sample.enabled == TRUE){
       rt_sample.point_d = now();
    }
}

IFX_INTERRUPT(CAN_irqRxCan3Handler, 2, ISR_PRIORITY_CAN3_RX)
{
#if CANGATEWAYDEBUG == 1
    cangen_recv_count_Can3 = cangen_recv_count_Can3 + 1;
#endif
    IfxCan_Message rxMsg;
    uint8 length;
    IfxCan_Can_initMessage(&rxMsg);
    rxMsg.readFromRxFifo0 = TRUE;
    rxMsg.bufferNumber = 0;
    IfxCan_Can_readMessage(&g_Multican.drivers.node[3], &rxMsg, rxData3);
    //IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"messageid: %d/n"ENDL,rxMsg.messageId);
    length = dlc[rxMsg.dataLengthCode];
#if CANGATEWAYDEBUG == 1
    //if (cangen_fix_count)
    {
      uint8 nquadlets = length/8;
      int i;
      for (i=0;i<nquadlets;i++){
        uint32 rxData[2];
        rxData[0]   = rxData3[i*2];
        rxData[1]   = rxData3[(i*2)+1];
        //IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"queue_dequeue_all[%d]: 0x%X 0x%X"ENDL,i,rxData[0], rxData[1]);
        if ((rxData[0] != 0xAAAAAAAA) || (rxData[1] != 0x55555555)){
          cangen_error_cnt3 = cangen_error_cnt3 + 1;
          break;
        }
      }
    }
#endif
    element_t edata;
    convert_can_to_edata(g_can_queue[3],3, rxMsg.messageId, (uint8 *)rxData3, length,&edata);
    queue_enqueue(g_can_queue[3], edata);

    rxCnt[3]++;
    // calculate throughput time
    receive_time = (uint32)(now() / TimeConst_1us);

    IfxCan_Node_clearInterruptFlag(g_Multican.drivers.node[3].node, IfxCan_Interrupt_rxFifo0NewMessage);
    if(rt_sample.enabled == TRUE){
       rt_sample.point_d = now();
    }
}

IFX_INTERRUPT(CAN_irqRxCan4Handler, 5, ISR_PRIORITY_CAN4_RX)
{
#if CANGATEWAYDEBUG == 1
    cangen_recv_count_Can4 = cangen_recv_count_Can4 + 1;
#endif
    IfxCan_Message rxMsg;
    uint8 length;
    IfxCan_Can_initMessage(&rxMsg);
    rxMsg.readFromRxFifo0 = TRUE;
    rxMsg.bufferNumber = 0;
    IfxCan_Can_readMessage(&g_Multican.drivers.node[4], &rxMsg, rxData4);
    //IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"messageid: %d/n"ENDL,rxMsg.messageId);
    length = dlc[rxMsg.dataLengthCode];
#if CANGATEWAYDEBUG == 1
    //if (cangen_fix_count)
    {
      uint8 nquadlets = length/8;
      int i;
      for (i=0;i<nquadlets;i++){
        uint32 rxData[2];
        rxData[0]   = rxData4[i*2];
        rxData[1]   = rxData4[(i*2)+1];
        //IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"queue_dequeue_all[%d]: 0x%X 0x%X"ENDL,i,rxData[0], rxData[1]);
        if ((rxData[0] != 0xAAAAAAAA) || (rxData[1] != 0x55555555)){
          cangen_error_cnt4 = cangen_error_cnt4 + 1;
          break;
        }
      }
    }
#endif
    element_t edata;
    convert_can_to_edata(g_can_queue[4],4, rxMsg.messageId, (uint8 *)rxData4, length,&edata);
    queue_enqueue(g_can_queue[4], edata);

    rxCnt[4]++;
    // calculate throughput time
    receive_time = (uint32)(now() / TimeConst_1us);

    IfxCan_Node_clearInterruptFlag(g_Multican.drivers.node[4].node, IfxCan_Interrupt_rxFifo0NewMessage);
    if(rt_sample.enabled == TRUE){
       rt_sample.point_d = now();
    }
}

IFX_INTERRUPT(CAN_irqRxCan5Handler, 5, ISR_PRIORITY_CAN5_RX)
{
#if CANGATEWAYDEBUG == 1
    cangen_recv_count_Can5 = cangen_recv_count_Can5 + 1;
#endif
    IfxCan_Message rxMsg;
    uint8 length;
    IfxCan_Can_initMessage(&rxMsg);
    rxMsg.readFromRxFifo0 = TRUE;
    rxMsg.bufferNumber = 0;
    IfxCan_Can_readMessage(&g_Multican.drivers.node[5], &rxMsg, rxData5);
    //IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"messageid: %d/n"ENDL,rxMsg.messageId);
    length = dlc[rxMsg.dataLengthCode];
#if CANGATEWAYDEBUG == 1
    //if (cangen_fix_count)
    {
      uint8 nquadlets = length/8;
      int i;
      for (i=0;i<nquadlets;i++){
        uint32 rxData[2];
        rxData[0]   = rxData5[i*2];
        rxData[1]   = rxData5[(i*2)+1];
        //IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"queue_dequeue_all[%d]: 0x%X 0x%X"ENDL,i,rxData[0], rxData[1]);
        if ((rxData[0] != 0xAAAAAAAA) || (rxData[1] != 0x55555555)){
          cangen_error_cnt5 = cangen_error_cnt5 + 1;
          break;
        }
      }
    }
#endif
    element_t edata;
    convert_can_to_edata(g_can_queue[5],5, rxMsg.messageId, (uint8 *)rxData5, length,&edata);
    queue_enqueue(g_can_queue[5], edata);

    rxCnt[5]++;
    // calculate throughput time
    receive_time = (uint32)(now() / TimeConst_1us);

    IfxCan_Node_clearInterruptFlag(g_Multican.drivers.node[5].node, IfxCan_Interrupt_rxFifo0NewMessage);
    if(rt_sample.enabled == TRUE){
       rt_sample.point_d = now();
    }
}

#if 0
static boolean Wait_For_Node_Synchronized(uint8 i)
{
    uint32 count = 0;
    while (IfxCan_Can_isNodeSynchronized(&canNode[i]) != TRUE)
    {
        count++;
        waitTime(TimeConst[TIMER_INDEX_1MS]);
        if(count > CAN_WAIT_TIME_MS)
        {
            //CAN_PRINTF_ALWAYS(("Error: Time out waiting for CAN node %d synchronized\r\n", i));
            return FALSE;
        }
    }
    return TRUE;
}

static void STM2_Initial(void)
{
    /* initialize STM2 */
    IfxStm_CompareConfig stmCompareConfig;
    /* suspend by debugger enabled */
    IfxStm_enableOcdsSuspend(&MODULE_STM2);
    /* constructor of configuration */
    IfxStm_initCompareConfig(&stmCompareConfig);
    /* change the comparator configuration */
    stmCompareConfig.triggerPriority      = ISR_PRIORITY_STM2_TICK;
    stmCompareConfig.comparatorInterrupt  = IfxStm_ComparatorInterrupt_ir0; /*User must select the interrupt output */
    stmCompareConfig.ticks                = IFX_CFG_STM2_TICKS_PER_MS*10;    /* we start after 10ms */
    stmCompareConfig.typeOfService        = IfxSrc_Tos_cpu2;
    /* Now Compare functionality is initialized */
    IfxStm_initCompare(&MODULE_STM2, &stmCompareConfig);
}

IFX_INTERRUPT (UsrIsr_Stm_2, 2, ISR_PRIORITY_STM2_TICK);
void UsrIsr_Stm_2(void)
{
    stm2cnt = stm2cnt + 1;
    /* Set next 1ms scheduler tick alarm */
    if (stm2cnt >= cangen_time_ms){
        IfxStm_disableComparatorInterrupt(&MODULE_STM2,  IfxStm_Comparator_0);
        IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"cangen finish"ENDL);
        stm2cnt = 0;
    }else{
        IfxStm_increaseCompare(&MODULE_STM2, IfxStm_Comparator_0, IFX_CFG_STM2_TICKS_PER_MS);
    }
    IfxCan_Message rxMsg;
    uint32 rxData[64] = {0};
    uint8 length;
    IfxCan_Can_initMessage(&rxMsg);
    rxMsg.readFromRxFifo0 = TRUE;
    rxMsg.bufferNumber = 0;
    //while (IfxCan_Node_isRxBufferNewDataUpdated(g_CanBasic.drivers.canNode[i].node, (IfxCan_RxBufferId)rxMsg.bufferNumber) == 0) {}
    //IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"CAN_irqRxCan0Handler id:%d, 0x%02X,0x%02X"ENDL,rxMsg.messageId, rxData[0],rxData[1]);
    //IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"CAN_irqRxCan0Handler 0x%X, 0x%X"ENDL,rxData[0],rxData[1]);
    int i;
    for (i=0;i<can_num_per_ms;i++){
      rxData[0] = 0xAAAAAA00; //| cnt;
      rxData[1] = 0x55555555;
      send_multican_message(cangen_node_id, cangen_msg_id, rxData[0], rxData[1], FALSE);
      //cnt = cnt + 1;
    }
}
#endif

static void enable_cansendtest(boolean is_fix_count, boolean is_dual){
    cangen_enable = TRUE;
    cangen_fix_count = is_fix_count;
    cangen_dual_test = is_dual;
    IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"cangen_fix_count %d, cangen_dual_test %d"ENDL,cangen_fix_count,cangen_dual_test);
}

void cansendtest(void)
{
    stm2cnt = stm2cnt + 1;
    if (cangen_time_ms != 0){
        if (stm2cnt > cangen_time_ms){
            cangen_enable = FALSE;
            stm2cnt = 0;
            cangen_cancnt = 0;
            IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"cansendtest finish"ENDL);
            return;
        }
    }
    IfxCan_Message rxMsg;
    uint32 rxData[16] = {0};
    IfxCan_Can_initMessage(&rxMsg);
    rxMsg.readFromRxFifo0 = TRUE;
    rxMsg.bufferNumber = 0;
    uint8 nquadlets = cangen_nbytes/4;
    int i;
    rxData[1] = 0x55555555;
    for (i=2;i<16;i++){
        rxData[i] = 0xAAAAAAAA;
        i++;
        rxData[i] = 0x55555555;
    }

    for (i=0;i<can_num_per_ms;i++){
      cangen_cancnt = cangen_cancnt + 1;
      if (cangen_fix_count){
        rxData[0] = 0xAAAAAAAA; //0xAAAA0000 | cangen_cancnt;
      }else{
        rxData[0] = 0xAAAA0000 | cangen_cancnt;
      }
      if (cangen_dual_test){
          send_multican_message_gw(0, 1, cangen_nbytes, &rxData, FALSE);
          send_multican_message_gw(1, 6, cangen_nbytes, &rxData, FALSE);
          send_multican_message_gw(2, 3, cangen_nbytes, &rxData, FALSE);
          send_multican_message_gw(3, 4, cangen_nbytes, &rxData, FALSE);
          send_multican_message_gw(4, 5, cangen_nbytes, &rxData, FALSE);
          send_multican_message_gw(5, 2, cangen_nbytes, &rxData, FALSE);
      }else{
          send_multican_message_gw(cangen_node_id, cangen_msg_id, cangen_nbytes, &rxData, FALSE);
      }
    }
}
boolean Shell_cangateway_cangen(pchar args, void *data, IfxStdIf_DPipe *io)
{
  if (Ifx_Shell_matchToken(&args, "?") != FALSE)
  {
    IfxStdIf_DPipe_print(io, "Syntax     : cangen node id num time_ms nbytes [fix/dual/fixdual]"ENDL);
    IfxStdIf_DPipe_print(io, "           > ex: cangen 0 1 4 10000 8"ENDL);
    IfxStdIf_DPipe_print(io, "           > can data generate for gateway"ENDL);
  }else{
#if CANGATEWAYDEBUG == 1
    if (Ifx_Shell_matchToken(&args, "show") != FALSE)
    {
      IfxStdIf_DPipe_print(io, "cangen_int_count0: %d/%d"ENDL,cangen_error_cnt0,cangen_recv_count_Can0);
      IfxStdIf_DPipe_print(io, "cangen_int_count1: %d/%d"ENDL,cangen_error_cnt1,cangen_recv_count_Can1);
      IfxStdIf_DPipe_print(io, "cangen_int_count2: %d/%d"ENDL,cangen_error_cnt2,cangen_recv_count_Can2);
      IfxStdIf_DPipe_print(io, "cangen_int_count3: %d/%d"ENDL,cangen_error_cnt3,cangen_recv_count_Can3);
      IfxStdIf_DPipe_print(io, "cangen_int_count4: %d/%d"ENDL,cangen_error_cnt4,cangen_recv_count_Can4);
      IfxStdIf_DPipe_print(io, "cangen_int_count5: %d/%d"ENDL,cangen_error_cnt5,cangen_recv_count_Can5);
      IfxStdIf_DPipe_print(io, "cangen_queue_count0: %d/%d"ENDL,cangen_queue_error_cnt[0],cangen_recv_queue_count[0]);
      IfxStdIf_DPipe_print(io, "cangen_queue_count1: %d/%d"ENDL,cangen_queue_error_cnt[1],cangen_recv_queue_count[1]);
      IfxStdIf_DPipe_print(io, "cangen_queue_count2: %d/%d"ENDL,cangen_queue_error_cnt[2],cangen_recv_queue_count[2]);
      IfxStdIf_DPipe_print(io, "cangen_queue_count3: %d/%d"ENDL,cangen_queue_error_cnt[3],cangen_recv_queue_count[3]);
      IfxStdIf_DPipe_print(io, "cangen_queue_count4: %d/%d"ENDL,cangen_queue_error_cnt[4],cangen_recv_queue_count[4]);
      IfxStdIf_DPipe_print(io, "cangen_queue_count5: %d/%d"ENDL,cangen_queue_error_cnt[5],cangen_recv_queue_count[5]);
      //int i;
      //for (i=0;i<10;i++){
      //  IfxStdIf_DPipe_print(io, "queue: %d"ENDL,g_can_queue->elements[i].udp_content[18]);
      //}
      IfxStdIf_DPipe_print(io, "queue0->tail: %d"ENDL,g_can_queue[0]->tail);
      IfxStdIf_DPipe_print(io, "queue0->head: %d"ENDL,g_can_queue[0]->head);
      return 1;
    }
    if (Ifx_Shell_matchToken(&args, "reset") != FALSE)
    {
      cangen_recv_count_Can0 = 0;
      cangen_recv_count_Can1 = 0;
      cangen_recv_count_Can2 = 0;
      cangen_recv_count_Can3 = 0;
      cangen_recv_count_Can4 = 0;
      cangen_recv_count_Can5 = 0;
      cangen_recv_queue_count[0] = 0;
      cangen_recv_queue_count[1] = 0;
      cangen_recv_queue_count[2] = 0;
      cangen_recv_queue_count[3] = 0;
      cangen_recv_queue_count[4] = 0;
      cangen_recv_queue_count[5] = 0;
      cangen_error_cnt0 = 0;
      cangen_error_cnt1 = 0;
      cangen_error_cnt2 = 0;
      cangen_error_cnt3 = 0;
      cangen_error_cnt4 = 0;
      cangen_error_cnt5 = 0;
      cangen_index = 1;
      cangen_queue_error_cnt[0] = 0;
      cangen_queue_error_cnt[1] = 0;
      cangen_queue_error_cnt[2] = 0;
      cangen_queue_error_cnt[3] = 0;
      cangen_queue_error_cnt[4] = 0;
      cangen_queue_error_cnt[5] = 0;
      cangen_queue_index = 1;
      cangen_nbytes = 8;
      //g_can_queue = queue_init();
      IfxStdIf_DPipe_print(io, "Reset cangen_recv_count"ENDL);
      return 1;
    }
#endif
    if (Ifx_Shell_parseSInt32(&args, &cangen_node_id) == FALSE)
    {
      IfxStdIf_DPipe_print(io, "Error     :  node"ENDL);
      return 1;
    }
    if (Ifx_Shell_parseSInt32(&args, &cangen_msg_id) == FALSE)
    {
      IfxStdIf_DPipe_print(io, "Error     :  id"ENDL);
      return 1;
    }
    if (Ifx_Shell_parseSInt32(&args, &can_num_per_ms) == FALSE)
    {
      IfxStdIf_DPipe_print(io, "Error     :  num"ENDL);
      return 1;
    }
    if (Ifx_Shell_parseSInt32(&args, &cangen_time_ms) == FALSE)
    {
      IfxStdIf_DPipe_print(io, "Error     :  time_ms"ENDL);
      return 1;
    }
    if (Ifx_Shell_parseSInt32(&args, &cangen_nbytes) == FALSE)
    {
      IfxStdIf_DPipe_print(io, "Error     :  nbytes"ENDL);
      return 1;
    }
    if (Ifx_Shell_matchToken(&args, "fix") != FALSE)
        enable_cansendtest(TRUE, FALSE);
    else{
        if (Ifx_Shell_matchToken(&args, "dual") != FALSE){
          enable_cansendtest(FALSE,TRUE);
        }else{
            if (Ifx_Shell_matchToken(&args, "fixdual") != FALSE){
              enable_cansendtest(TRUE,TRUE);
            }else
              enable_cansendtest(FALSE,FALSE);
        }
    }
  }
  return 1;
}

void queue_print_element(element_t *edata)
{
    uint8 bus_id = 0;
    bus_id = (uint8)edata->udp_content[8];
    uint32 msg_id = 0;
    msg_id = (uint32)((edata->udp_content[15] << 24) | (edata->udp_content[14] << 16) | (edata->udp_content[13] << 8) | edata->udp_content[12]);
    uint16 msg_length = 0;
    msg_length = (uint16)((edata->udp_content[17] << 8) | edata->udp_content[16]);
    if (msg_length == 8){
      uint32 doubleword_data0 = (uint32)((edata->udp_content[21] << 24) | edata->udp_content[20] << 16 |(edata->udp_content[19] << 8) | edata->udp_content[18]);
      uint32 doubleword_data1 = (uint32)((edata->udp_content[25] << 24) | edata->udp_content[24] << 16 |(edata->udp_content[23] << 8) | edata->udp_content[22]);
      IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"%s Receive data: 0x%x, 0x%x \r\n", NODE_CONFIG[bus_id].name, doubleword_data0, doubleword_data1);
    }else{
      IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"%s length %d: \r\n", NODE_CONFIG[bus_id].name, msg_length);
      int i;
      for (i=0;i<msg_length;i++)
        IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"0x%02X ", edata->udp_content[18+i]);
        IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc," \r\n");
    }
}

void queue_dequeue_all(queue_t ** pself)
{
    int i;
    for (i=0;i<6;i++){
        while(!queue_is_empty(pself[i]))
        {
            cangen_recv_queue_count[i] = cangen_recv_queue_count[i] + 1;
            element_t edata = queue_dequeue(pself[i]);
            if(queue_dequeue_all_print_flag)
            {
                queue_print_element(&edata);
            }
            #if CANGATEWAYDEBUG == 1
                uint8 bus_id = 0;
                bus_id = (uint8)edata.udp_content[8];
                uint32 msg_id = 0;
                msg_id = (uint32)((edata.udp_content[15] << 24) | (edata.udp_content[14] << 16) | (edata.udp_content[13] << 8) | edata.udp_content[12]);
                uint16 msg_length = 0;
                msg_length = (uint16)((edata.udp_content[17] << 8) | edata.udp_content[16]);
                uint8 nquadlets = msg_length/8;
                int i;
                for (i=0;i<nquadlets;i++){
                    uint32 rxData[2];
                    rxData[0] = edata.udp_content[18+(i*8)] | edata.udp_content[19+(i*8)] << 8 | edata.udp_content[20+(i*8)] << 16 |edata.udp_content[21+(i*8)] << 24 ;
                    rxData[1] = edata.udp_content[22+(i*8)] | edata.udp_content[23+(i*8)] << 8 | edata.udp_content[24+(i*8)] << 16 |edata.udp_content[25+(i*8)] << 24 ;
                    //IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"queue_dequeue_all[%d]: 0x%X 0x%X"ENDL,i,rxData[0], rxData[1]);
                    if ((rxData[0] != 0xAAAAAAAA) || (rxData[1] != 0x55555555)){
                        cangen_queue_error_cnt[i] = cangen_queue_error_cnt[i] + 1;
                        //IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"queue_dequeue_all[%d,%d,%d]: [%d]0x%X 0x%X"ENDL,bus_id,msg_id,msg_length,i,rxData[0], rxData[1]);
                        break;
                    }
                }
            #endif
         }
    }
}

void initISO_Mode_Config(IfxCan_Can_NodeConfig *config)
{
    Ifx_CAN   *canSfr  = config->can;
    Ifx_CAN_N *nodeSfr = &(canSfr->N[config->nodeId]);

    /* initialise the clock for the selected node */
    IfxCan_ClockSelect clockSelect = (IfxCan_ClockSelect)config->nodeId;
    IfxCan_setClockSource(canSfr, clockSelect, config->clockSource);

    IfxCan_Node_enableConfigurationChange(nodeSfr);
    if(noniso_mode == 0){
        nodeSfr->CCCR.B.NISO = 0;
    }else{
        nodeSfr->CCCR.B.NISO = 1;
    }
    IfxCan_Node_disableConfigurationChange(nodeSfr);
    IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"##### noniso=%d #### \r\n", nodeSfr->CCCR.B.NISO);
    IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"##### noniso=%d #### \r\n", noniso_mode);
}


void IfxMultican_initNode_with_interrupt(uint32 node, uint32 classic_baudrate, boolean is_fdmode, uint32 fd_baudrate, uint32 delay_offset, uint32 samplepoint_a, uint32 samplepoint_b)
{
    IfxCan_Can_NodeConfig nodeConfig[6];
    memset(nodeConfig, 0, sizeof(nodeConfig));
	uint8 i;
	uint32 node_ram_offset[4] = {
	    NODE0_RAM_OFFSET,
	    NODE1_RAM_OFFSET,
	    NODE2_RAM_OFFSET,
	    NODE3_RAM_OFFSET,
	};
	i = node;
	{
	        IfxCan_Can_initNodeConfig(&nodeConfig[i], &g_Multican.drivers.can[NODE_CONFIG[i].module_index]);
	        // initialize Node 0
	        nodeConfig[i].nodeId = NODE_CONFIG[i].node_id;
	        nodeConfig[i].clockSource = IfxCan_ClockSource_both;
	        nodeConfig[i].frame.type = IfxCan_FrameType_transmitAndReceive;
	        // kokob3: IfxCan_TxMode_fifo & IfxCan_TxMode_dedicatedBuffers will block if no Rx
	        //nodeConfig.txConfig.txMode = IfxCan_TxMode_fifo;
	        /*Fifo setting start*/
	        nodeConfig[i].txConfig.txMode = IfxCan_TxMode_queue;    // see the Rx mode optins to select between Tx buffers and Tx fifo/Queue or the combination
	        nodeConfig[i].txConfig.dedicatedTxBuffersNumber = 0;    // since Tx mode is chosen as only FIFO
	        nodeConfig[i].txConfig.txFifoQueueSize = 20;
	        nodeConfig[i].rxConfig.rxMode = IfxCan_RxMode_fifo0;    // see the Rx mode optins to select between Rx buffers and Rx fifos or the combination

	        nodeConfig[i].rxConfig.rxFifo0Size = 20;

	        nodeConfig[i].filterConfig.standardFilterForNonMatchingFrames = IfxCan_NonMatchingFrame_acceptToRxFifo1;
#if CAN_EXT_ID_ENABLED == 1/* [EXTENED ID test] */
        nodeConfig[i].filterConfig.messageIdLength = IfxCan_MessageIdLength_both;
        nodeConfig[i].filterConfig.extendedListSize = 2;
        nodeConfig[i].filterConfig.extendedFilterForNonMatchingFrames = IfxCan_NonMatchingFrame_acceptToRxFifo1;
#endif
	        nodeConfig[i].interruptConfig.rxFifo0NewMessageEnabled = TRUE;
	        nodeConfig[i].interruptConfig.rxf0n.priority = V33_Can_Int[i].rxPriority;//ISR_PRIORITY_CANx_RX;
	        nodeConfig[i].interruptConfig.rxf0n.interruptLine = V33_Can_Int[i].rxInterruptLine;//IfxCan_InterruptLine_x;
	        nodeConfig[i].interruptConfig.rxf0n.typeOfService = V33_Can_Int[i].tos_num;

	        nodeConfig[i].messageRAM.standardFilterListStartAddress = 0x0 + (uint32)node_ram_offset[NODE_CONFIG[i].node_id];
	        nodeConfig[i].messageRAM.extendedFilterListStartAddress = 0x80 + (uint32)node_ram_offset[NODE_CONFIG[i].node_id];
	        nodeConfig[i].messageRAM.rxFifo0StartAddress                    = 0xA20+ (uint32)node_ram_offset[NODE_CONFIG[i].node_id];
	        nodeConfig[i].messageRAM.rxFifo1StartAddress                    = 0x200+ (uint32)node_ram_offset[NODE_CONFIG[i].node_id];
	        nodeConfig[i].messageRAM.rxBuffersStartAddress                  = 0x300+ (uint32)node_ram_offset[NODE_CONFIG[i].node_id];

	        nodeConfig[i].messageRAM.txEventFifoStartAddress            = 0x400+ (uint32)node_ram_offset[NODE_CONFIG[i].node_id];
	        nodeConfig[i].messageRAM.txBuffersStartAddress                  = 0x440+ (uint32)node_ram_offset[NODE_CONFIG[i].node_id];

	        nodeConfig[i].messageRAM.baseAddress = (uint32)(can_used_module[NODE_CONFIG[i].module_index]);//(uint32)(can_used_module[NODE_CONFIG[i].module_index]);
	        /*Fifo setting end*/
	        //CAN_PRINTF_ALWAYS(("offset is 0x%x\r\n", (NODE_CONFIG[i].node_id % 4) * 0x1000));
	        //yc:  not needed       + NODE_OFFSET * (uint8)NODE_CONFIG[i].node_id;
	        // enable the required interrupts with respective to group interrupts configuration at module level
	        //nodeConfig[0].interruptConfig.messageStoredToDedicatedRxBufferEnabled = TRUE;
	        nodeConfig[i].pins = NODE_CONFIG[i].pin;
	        if(is_fdmode)
	        {
	            nodeConfig[i].frame.mode = IfxCan_FrameMode_fdLongAndFast;
	            nodeConfig[i].rxConfig.rxBufferDataFieldSize = IfxCan_DataFieldSize_64;
	            nodeConfig[i].rxConfig.rxFifo0DataFieldSize = IfxCan_DataFieldSize_64;
	            nodeConfig[i].rxConfig.rxFifo1DataFieldSize = IfxCan_DataFieldSize_64;
	            nodeConfig[i].txConfig.txBufferDataFieldSize = IfxCan_DataFieldSize_64;
	            IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"Enable FD mode in node \r\n");
	            nodeConfig[i].baudRate.baudrate = classic_baudrate;
	            nodeConfig[i].baudRate.samplePoint = samplepoint_a;
	            nodeConfig[i].fastBaudRate.baudrate = fd_baudrate;
	            nodeConfig[i].fastBaudRate.samplePoint = samplepoint_b;
	            nodeConfig[i].fastBaudRate.tranceiverDelayOffset = delay_offset;

                initISO_Mode_Config(&nodeConfig[i]);
	            IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"baudrate is %d for standard,\r\n%d for data frame\r\n",
	                    nodeConfig[i].baudRate.baudrate,
	                    nodeConfig[i].fastBaudRate.baudrate);
	            IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"samplepoint is %d for standard,\r\n%d for data frame\r\n",
	                                nodeConfig[i].baudRate.samplePoint,
	                                nodeConfig[i].fastBaudRate.samplePoint);
	            IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"tranceiverDelayOffset is %d\r\n", nodeConfig[i].fastBaudRate.tranceiverDelayOffset);

	        }else{
	            nodeConfig[i].baudRate.baudrate = classic_baudrate;
	            nodeConfig[i].baudRate.samplePoint = samplepoint_a;
	        }
	        IfxCan_Can_initNode(&g_Multican.drivers.node[i], &nodeConfig[i]);
	        IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"IfxCan_Can_initNode %d with interrupt"ENDL,i);
	}

}


static void IfxMultican_initNodes_with_interrupt(uint32 baudrate)
{
    IfxCan_Can_NodeConfig nodeConfig[6];
    memset(nodeConfig, 0, sizeof(nodeConfig));

    uint8 i;
    uint32 node_ram_offset[4] = {
        NODE0_RAM_OFFSET,
        NODE1_RAM_OFFSET,
        NODE2_RAM_OFFSET,
        NODE3_RAM_OFFSET,
    };
    for(i=0; i<CAN_USED_NODES_NUM; i++)
    {
        IfxCan_Can_initNodeConfig(&nodeConfig[i], &g_Multican.drivers.can[NODE_CONFIG[i].module_index]);
        // initialize Node 0
        nodeConfig[i].nodeId = NODE_CONFIG[i].node_id;
        nodeConfig[i].clockSource = IfxCan_ClockSource_both;
        nodeConfig[i].frame.type = IfxCan_FrameType_transmitAndReceive;
        // kokob3: IfxCan_TxMode_fifo & IfxCan_TxMode_dedicatedBuffers will block if no Rx
        //nodeConfig.txConfig.txMode = IfxCan_TxMode_fifo;
        /*Fifo setting start*/
        nodeConfig[i].txConfig.txMode = IfxCan_TxMode_queue;    // see the Rx mode optins to select between Tx buffers and Tx fifo/Queue or the combination
        nodeConfig[i].txConfig.dedicatedTxBuffersNumber = 0;    // since Tx mode is chosen as only FIFO
        nodeConfig[i].txConfig.txFifoQueueSize = 20;
        nodeConfig[i].rxConfig.rxMode = IfxCan_RxMode_fifo0;    // see the Rx mode optins to select between Rx buffers and Rx fifos or the combination

        nodeConfig[i].rxConfig.rxFifo0Size = 20;

        nodeConfig[i].filterConfig.standardFilterForNonMatchingFrames = IfxCan_NonMatchingFrame_acceptToRxFifo1;
#if CAN_EXT_ID_ENABLED == 1/* [EXTENED ID test] */
				nodeConfig[i].filterConfig.messageIdLength = IfxCan_MessageIdLength_both;
				nodeConfig[i].filterConfig.extendedListSize = 2;
				nodeConfig[i].filterConfig.extendedFilterForNonMatchingFrames = IfxCan_NonMatchingFrame_acceptToRxFifo1;
#endif

        //nodeConfig[i].rxConfig.rxMode = IfxCan_RxMode_fifo0;    // see the Rx mode optins to select between Rx buffers and Rx fifos or the combination
        //nodeConfig[i].rxConfig.rxFifo0Size = 64;
        //nodeConfig[i].filterConfig.standardFilterForNonMatchingFrames = IfxCan_NonMatchingFrame_acceptToRxFifo1;
        //nodeConfig[i].interruptConfig.messageStoredToDedicatedRxBufferEnabled = TRUE;
        nodeConfig[i].interruptConfig.rxFifo0NewMessageEnabled = TRUE;
        nodeConfig[i].interruptConfig.rxf0n.priority = V33_Can_Int[i].rxPriority;//ISR_PRIORITY_CANx_RX;
        nodeConfig[i].interruptConfig.rxf0n.interruptLine = V33_Can_Int[i].rxInterruptLine;//IfxCan_InterruptLine_x;
        nodeConfig[i].interruptConfig.rxf0n.typeOfService = V33_Can_Int[i].tos_num;

        nodeConfig[i].messageRAM.standardFilterListStartAddress = 0x0 + (uint32)node_ram_offset[NODE_CONFIG[i].node_id];
        nodeConfig[i].messageRAM.extendedFilterListStartAddress = 0x80 + (uint32)node_ram_offset[NODE_CONFIG[i].node_id];

        nodeConfig[i].messageRAM.rxFifo0StartAddress                    = 0xA20+ (uint32)node_ram_offset[NODE_CONFIG[i].node_id];

        nodeConfig[i].messageRAM.rxFifo1StartAddress                    = 0x200+ (uint32)node_ram_offset[NODE_CONFIG[i].node_id];
        nodeConfig[i].messageRAM.rxBuffersStartAddress = 0x800+ (uint32)node_ram_offset[NODE_CONFIG[i].node_id];
        nodeConfig[i].messageRAM.txEventFifoStartAddress            = 0x400+ (uint32)node_ram_offset[NODE_CONFIG[i].node_id];
        nodeConfig[i].messageRAM.txBuffersStartAddress                  = 0x440+ (uint32)node_ram_offset[NODE_CONFIG[i].node_id];
        nodeConfig[i].messageRAM.baseAddress = (uint32)(can_used_module[NODE_CONFIG[i].module_index]);//(uint32)(can_used_module[NODE_CONFIG[i].module_index]);
        /*Fifo setting end*/

        //CAN_PRINTF_ALWAYS(("offset is 0x%x\r\n", (NODE_CONFIG[i].node_id % 4) * 0x1000));
        //yc:  not needed       + NODE_OFFSET * (uint8)NODE_CONFIG[i].node_id;
        // enable the required interrupts with respective to group interrupts configuration at module level
        //nodeConfig[0].interruptConfig.messageStoredToDedicatedRxBufferEnabled = TRUE;
        nodeConfig[i].pins = NODE_CONFIG[i].pin;
#if 0
        IfxCan_Can_Pins pins;
        pins.rxPin = V33_Can[i].rxPin;
        pins.rxPinMode = IfxPort_InputMode_pullUp;
        pins.txPin = V33_Can[i].txPin;
        pins.txPinMode = IfxPort_OutputMode_pushPull;
        pins.padDriver = IfxPort_PadDriver_cmosAutomotiveSpeed2;
        nodeConfig[i].pins = &pins;
#endif
        if(fdMode)
        {
            nodeConfig[i].frame.mode = IfxCan_FrameMode_fdLongAndFast;
            nodeConfig[i].rxConfig.rxBufferDataFieldSize = IfxCan_DataFieldSize_64;
            nodeConfig[i].rxConfig.rxFifo0DataFieldSize = IfxCan_DataFieldSize_64;
            nodeConfig[i].rxConfig.rxFifo1DataFieldSize = IfxCan_DataFieldSize_64;
            nodeConfig[i].txConfig.txBufferDataFieldSize = IfxCan_DataFieldSize_64;
            IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"Enable FD mode in node \r\n");
            nodeConfig[i].baudRate.baudrate = baudrate;
            nodeConfig[i].baudRate.samplePoint = samplepoint_standard;
            nodeConfig[i].fastBaudRate.baudrate = fastBaudRate;
            nodeConfig[i].fastBaudRate.samplePoint = samplepoint_fast;
            nodeConfig[i].fastBaudRate.tranceiverDelayOffset = tranceiver_delay_offset;

            initISO_Mode_Config(&nodeConfig[i]);
            IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"baudrate is %d for standard,\r\n%d for data frame\r\n",
                    nodeConfig[i].baudRate.baudrate,
                    nodeConfig[i].fastBaudRate.baudrate);
            IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"samplepoint is %d for standard,\r\n%d for data frame\r\n",
                                nodeConfig[i].baudRate.samplePoint,
                                nodeConfig[i].fastBaudRate.samplePoint);
            IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"tranceiverDelayOffset is %d\r\n", nodeConfig[i].fastBaudRate.tranceiverDelayOffset);
        }
        else
        {
            nodeConfig[i].baudRate.baudrate = baudrate;
            nodeConfig[i].baudRate.samplePoint = samplepoint_standard;
        }
        IfxCan_Can_initNode(&g_Multican.drivers.node[i], &nodeConfig[i]);
        IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"IfxCan_Can_initNode with interrupt"ENDL);

    }
}

static void IfxMultican_initFilters_with_interrupt(uint8 filterMode)
{
    IfxCan_Filter filter[6];
    memset(filter, 0, sizeof(filter));
    uint8 i;
    for(i=0; i<CAN_USED_NODES_NUM; i++)
    {
        // Initialize the filter structure
        // set filter0 for rxBuffer 0
        filter[i].number = 0;
        filter[i].elementConfiguration = IfxCan_FilterElementConfiguration_storeInRxFifo0;//IfxCan_FilterElementConfiguration_storeInRxFifo0;
        filter[i].type = IfxCan_FilterType_range;
        if (filterMode == 0){//CAN_BRAODCAST_FILTER
            IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"CAN %d enable broadcast_mode"ENDL,i);
            filter[i].id1 = CanGateway_FILTER_ID[i];
            filter[i].id2 = CanGateway_FILTER_ID[i];
        } else if (filterMode == 1) { //CAN_UNICAST_FILTER
            IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"CAN %d disable broadcast_mode"ENDL,i);
            filter[i].id1 = CanGateway_FILTER_ID_F[i];
            filter[i].id2 = CanGateway_FILTER_ID_F[i];
        }
        filter[i].rxBufferOffset = IfxCan_RxBufferId_0;
        IfxCan_Can_setStandardFilter(&g_Multican.drivers.node[i], &filter[i]);
#if CAN_EXT_ID_ENABLED == 1/*[EXTENED ID test] */
        IfxCan_Can_setExtendedFilter(&g_Multican.drivers.node[i], &filter[i]);
#endif

        // set filter0 for rxBuffer 1
        filter[i].number = 1;
        filter[i].elementConfiguration = IfxCan_FilterElementConfiguration_storeInRxFifo0;//IfxCan_FilterElementConfiguration_storeInRxFifo0;
        filter[i].type = IfxCan_FilterType_range;
        filter[i].id1 =  0x7ff;
        filter[i].id2 =  0x7ff;
        filter[i].rxBufferOffset = IfxCan_RxBufferId_1;
        IfxCan_Can_setStandardFilter(&g_Multican.drivers.node[i], &filter[i]);
#if CAN_EXT_ID_ENABLED == 1/*[EXTENED ID test] */
        IfxCan_Can_setExtendedFilter(&g_Multican.drivers.node[i], &filter[i]);
#endif
    }
}

void IfxMultican_init_with_interrupt(uint32 baudrate,uint8 filterMode)
{
  // comment the IfxCpu_disableInterrupts() and IfxCpu_restoreInterrupts() due the OSTask would delay about 20s
  //boolean interruptState = IfxCpu_disableInterrupts();
  IfxMultican_initNodes_with_interrupt(baudrate);
  IfxMultican_initFilters_with_interrupt(filterMode);
  //IfxCpu_restoreInterrupts(interruptState);
}

void queue_init(queue_t** p_can_queue)
{
    can_queue0.elements = &(can_elements0[0]);
    can_queue0.capacity = CAN_QUEUE_ELEMENT_SIZE;
    can_queue0.head = 0;
    can_queue0.tail = 0;
    can_queue1.elements = &(can_elements1[0]);
    can_queue1.capacity = CAN_QUEUE_ELEMENT_SIZE;
    can_queue1.head = 0;
    can_queue1.tail = 0;
    can_queue2.elements = &(can_elements2[0]);
    can_queue2.capacity = CAN_QUEUE_ELEMENT_SIZE;
    can_queue2.head = 0;
    can_queue2.tail = 0;
    can_queue3.elements = &(can_elements3[0]);
    can_queue3.capacity = CAN_QUEUE_ELEMENT_SIZE;
    can_queue3.head = 0;
    can_queue3.tail = 0;
    can_queue4.elements = &(can_elements4[0]);
    can_queue4.capacity = CAN_QUEUE_ELEMENT_SIZE;
    can_queue4.head = 0;
    can_queue4.tail = 0;
    can_queue5.elements = &(can_elements5[0]);
    can_queue5.capacity = CAN_QUEUE_ELEMENT_SIZE;
    can_queue5.head = 0;
    can_queue5.tail = 0;
    p_can_queue[0] = &can_queue0;
    p_can_queue[1] = &can_queue1;
    p_can_queue[2] = &can_queue2;
    p_can_queue[3] = &can_queue3;
    p_can_queue[4] = &can_queue4;
    p_can_queue[5] = &can_queue5;
}

queue_t * queue_new(int c)
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return q;

    q->capacity = c;
    q->head = 0;
    q->tail = 0;
    q->elements = malloc(q->capacity * sizeof(element_t));
    if (!(q->elements)) {
        free(q);
        q = NULL;
        return q;
    }
    return q;
}

void queue_delete(void *self)
{
    if (!self)
        return;
    element_t *elements = ((queue_t *) self)->elements;
    if (elements)
        free(((queue_t *) self)->elements);
    free(self);
}

unsigned int queue_get_size(const queue_t * self)
{
  if (!self)
      return 0;
  if ((self->tail - self->head) < 0){
    return self->tail - self->head+self->capacity;
  }else
    return (self->tail - self->head);
}

boolean queue_is_empty(const queue_t *self)
{
    //assert(self);
    if (!self){
        return TRUE;
    }
    return queue_get_size(self) == 0;
}

boolean queue_is_full(const queue_t *self)
{
    //assert(self);
    if (!self){
        return TRUE;
    }
    return queue_get_size(self) == self->capacity;
}

// return head pointer element
element_t queue_peek(const queue_t *self)
{
    //assert(!queue_is_empty(self));
    if (queue_is_empty(self)){
        element_t e;
        return e;
    }
    return self->elements[self->head];
}

// 1) move tail, 2) put element data in queue
boolean queue_enqueue(queue_t *self, element_t data)
{
    //if queue is too small, we expand first
    //if (!queue_expand(self)) {
    //    return FALSE;
    //}
    if (queue_get_size(self) == self->capacity) {
        IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"queue_enqueue error full"ENDL);
        return FALSE;
    }
    //self->elements[self->tail] = data;
    int i;
    for (i=0;i<UDP_CONTENT_SIZE;i++){
         self->elements[self->tail].udp_content[i] = data.udp_content[i];
    }
    self->tail = (self->tail + 1) % self->capacity;
    return TRUE;
}

element_t queue_dequeue(queue_t *self)
{
    //assert(!queue_is_empty(self));
    if (queue_is_empty(self)){
        IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"queue_dequeue error is empty"ENDL);
        element_t e;
        return e;
    }
    // pop up the head element
    element_t popped = self->elements[self->head];
    // move head
    self->head = (self->head + 1) % self->capacity;
    return popped;
}

void can_latency_measure(void)
{
    rt_sample.point_b = now();
    if(rt_sample.tx_target_cnt){
        if(rt_sample.tx_cnt == 0){
            rt_sample.tx_start = now();
        }

        if(rt_sample.tx_cnt == rt_sample.tx_target_cnt - 1){
            rt_sample.tx_end = now();
        }
        rt_sample.tx_cnt++;
    }
}
