/**
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "Configuration.h"
#include <IfxCan.h>

#if MCM_CAN_TC39X_ENABLE == 1
#include "Mcmcan.h"
#include "SysSe/Bsp/Bsp.h"
#include <internal_fw_debug.h>
#include <_PinMap/IfxCan_PinMap.h>
#include <string.h>
#include <gpioexp.h>
#include "board.h"
#include "qci_udp_packet.h"
#include "qci_udp_client.h"
#include "os.h"
#include "pkg/com/com/inc/ee_com.h"
#include "pkg/com/com/inc/message_type.h"
#include <cangateway.h>
#include "Ifx_Lwip.h"
#include "MulticanCmds.h"
#include "Ifx_Console.h"


#pragma section ".bss_cpu1" awc1
static uint8 udp[UDP_ARRAY_SIZE] = {0};
uint32 rxData0[16] = {0};
uint32 rxData1[16] = {0};
uint32 rxData2[16] = {0};

queue_t can_queue0;
queue_t can_queue1;
queue_t can_queue2;


element_t can_elements0[CAN_QUEUE_ELEMENT_SIZE];
element_t can_elements1[CAN_QUEUE_ELEMENT_SIZE];
element_t can_elements2[CAN_QUEUE_ELEMENT_SIZE];

#pragma section

#pragma section ".bss_cpu5" awc5
uint32 rxData3[16] = {0};
queue_t can_queue3;
element_t can_elements3[CAN_QUEUE_ELEMENT_SIZE];
#pragma section

#pragma section ".bss_cpu3" awc3
uint32 rxData4[16] = {0};
queue_t can_queue4;
element_t can_elements4[CAN_QUEUE_ELEMENT_SIZE];
#pragma section

#pragma section ".bss_cpu4" awc4
uint32 rxData5[16] = {0};
queue_t can_queue5;
element_t can_elements5[CAN_QUEUE_ELEMENT_SIZE];
#pragma section

extern uint16 cangen_queue_index;
extern uint32 cangen_recv_queue_count[6];
extern uint16 cangen_queue_error_cnt[6];
extern boolean cangen_fix_count;
extern Ifx_Lwip g_Lwip;
extern CansetupConfig can_config[6];
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
//#define SHELL_NOT_ACTIVE   1

#define CAN_DEBUG_ENABLE 1

#if CAN_DEBUG_ENABLE == 1
#define CAN_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define CAN_DEBUG_PRINTF(x)
#endif

#define CAN_PRINTF_ALWAYS(x) DEBUG_PRINTF_ALWAYS(x)

/* Note: WAR: Resetting CAN_DEBUG_PRINTF to CAN_PRINTF_ALWAYS here
 * as we have disabled DEBUG_ENABLE in top level Confuguration.h
 */
#define CAN_DEBUG_PRINTF(x) CAN_PRINTF_ALWAYS(x)

#define CAN_RETRY_INTERNAL_MS  10
#define CAN_RETRY_TIMEOUT_S    1

#define NODE_OFFSET  0x1000

#define NODE0_RAM_OFFSET 0x0
#define NODE1_RAM_OFFSET (NODE0_RAM_OFFSET + 0x1000)
#define NODE2_RAM_OFFSET (NODE1_RAM_OFFSET + 0x1000)
#define NODE3_RAM_OFFSET (NODE2_RAM_OFFSET + 0x1000)
/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
volatile boolean node_a_lp = FALSE;
volatile boolean node_b_lp = FALSE;
volatile boolean node_cdef_lp = FALSE;
volatile boolean can_cnt_flag = FALSE;
volatile boolean fdMode = FALSE;
uint32 rxCnt[CAN_USED_NODES_NUM] = {0};

extern uint8 udp_host;
extern uint32 udp_ip[4];
extern struct udp_pcb *gw_sendback_pcb;
uint8 canMode = CAN_NORMAL_MODE, curFilterMode;
uint16 udpSeqNum = 1;


uint32 receive_time = 0xFFFFFFFF;

#if ISR_PROVIDER_MULTICAN == 0
    #if defined(__GNUC__)
    #pragma section ".bss_cpu0" awc0
    #endif
    #if defined(__TASKING__)
    #pragma section farbss "bss_cpu0"
    #pragma section fardata "data_cpu0"
    #endif
    #if defined(__DCC__)
    #pragma section DATA ".data_cpu0" ".bss_cpu0" far-absolute RW
    #endif
#elif ISR_PROVIDER_MULTICAN == 1
    #if defined(__GNUC__)
    #pragma section ".bss_cpu1" awc1
    #endif
    #if defined(__TASKING__)
    #pragma section farbss "bss_cpu1"
    #pragma section fardata "data_cpu1"
    #endif
    #if defined(__DCC__)
    #pragma section DATA ".data_cpu1" ".bss_cpu1" far-absolute RW
    #endif
#elif ISR_PROVIDER_MULTICAN == 2
    #if defined(__GNUC__)
    #pragma section ".bss_cpu2" awc2
    #endif
    #if defined(__TASKING__)
    #pragma section farbss "bss_cpu2"
    #pragma section fardata "data_cpu2"
    #endif
    #if defined(__DCC__)
    #pragma section DATA ".data_cpu2" ".bss_cpu2" far-absolute RW
    #endif
#else
#error "Set ISR_PROVIDER_MULTICAN to a valid value!"
#endif

App_Multican g_Multican; /**< \brief Multican global data */

#if defined(__GNUC__)
#pragma section
#endif
#if defined(__TASKING__)
#pragma section farbss restore
#pragma section fardata restore
#endif
#if defined(__DCC__)
#pragma section DATA RW
#endif

#define NODE0_RAM_OFFSET 0x0
#define NODE1_RAM_OFFSET (NODE0_RAM_OFFSET + 0x1000)
#define NODE2_RAM_OFFSET (NODE1_RAM_OFFSET + 0x1000)
#define NODE3_RAM_OFFSET (NODE2_RAM_OFFSET + 0x1000)

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/
boolean extendedFrame = FALSE;
uint32 fastBaudRate = 0;
uint32 samplepoint_standard = 8000;
uint32 samplepoint_fast = 8000;
uint8 tranceiver_delay_offset = 0;
uint32 noniso_mode = 0;
uint32 UDP_send_bytes = 0;
extern Ifx_CAN * can_used_module[CAN_USED_MODULES_NUM];
extern const t_node_config NODE_CONFIG[];
extern const uint32 FILTER_ID[6];
extern const uint32 FILTER_ID_F[6];
extern const e_phy NODE_PHY_MAPPING[];

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
extern IfxStdIf_DPipe *io_tmp;

static void IfxMultican_initNodes(uint32 baudrate)
{
    IfxCan_Can_NodeConfig nodeConfig[CAN_USED_NODES_NUM];
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

        /*Fifo setting start*/
        nodeConfig[i].txConfig.txMode = IfxCan_TxMode_fifo;    // see the Rx mode optins to select between Tx buffers and Tx fifo/Queue or the combination
        nodeConfig[i].txConfig.dedicatedTxBuffersNumber = 0;    // since Tx mode is chosen as only FIFO
        nodeConfig[i].txConfig.txFifoQueueSize = 20;
        nodeConfig[i].rxConfig.rxMode = IfxCan_RxMode_fifo0;    // see the Rx mode optins to select between Rx buffers and Rx fifos or the combination

        nodeConfig[i].rxConfig.rxFifo0Size = 20;
        nodeConfig[i].filterConfig.standardFilterForNonMatchingFrames = IfxCan_NonMatchingFrame_acceptToRxFifo1;
        nodeConfig[i].messageRAM.standardFilterListStartAddress = 0x0 + (uint32)node_ram_offset[NODE_CONFIG[i].node_id];
        nodeConfig[i].messageRAM.extendedFilterListStartAddress = 0x80 + (uint32)node_ram_offset[NODE_CONFIG[i].node_id];
        nodeConfig[i].messageRAM.rxFifo0StartAddress                    = 0xA20+ (uint32)node_ram_offset[NODE_CONFIG[i].node_id];

        nodeConfig[i].messageRAM.rxFifo1StartAddress                    = 0x200+ (uint32)node_ram_offset[NODE_CONFIG[i].node_id];
        nodeConfig[i].messageRAM.rxBuffersStartAddress = 0x800+ (uint32)node_ram_offset[NODE_CONFIG[i].node_id];
        nodeConfig[i].messageRAM.txEventFifoStartAddress            = 0x400+ (uint32)node_ram_offset[NODE_CONFIG[i].node_id];
        nodeConfig[i].messageRAM.txBuffersStartAddress                  = 0x440+ (uint32)node_ram_offset[NODE_CONFIG[i].node_id];
        nodeConfig[i].messageRAM.baseAddress = (uint32)(can_used_module[NODE_CONFIG[i].module_index]);
        /*Fifo setting end*/

        //CAN_PRINTF_ALWAYS(("offset is 0x%x\r\n", (NODE_CONFIG[i].node_id % 4) * 0x1000));
        //yc:  not needed       + NODE_OFFSET * (uint8)NODE_CONFIG[i].node_id;
        // enable the required interrupts with respective to group interrupts configuration at module level
        //nodeConfig[0].interruptConfig.messageStoredToDedicatedRxBufferEnabled = TRUE;
        nodeConfig[i].pins = NODE_CONFIG[i].pin;

        if(fdMode)
        {
            nodeConfig[i].frame.mode = IfxCan_FrameMode_fdLongAndFast;
            nodeConfig[i].rxConfig.rxBufferDataFieldSize = IfxCan_DataFieldSize_64;
            nodeConfig[i].rxConfig.rxFifo0DataFieldSize = IfxCan_DataFieldSize_64;
            nodeConfig[i].rxConfig.rxFifo1DataFieldSize = IfxCan_DataFieldSize_64;
            nodeConfig[i].txConfig.txBufferDataFieldSize = IfxCan_DataFieldSize_64;
            CAN_PRINTF_ALWAYS(("Enable FD mode in node \r\n"));

            nodeConfig[i].baudRate.baudrate = baudrate;
            nodeConfig[i].baudRate.samplePoint = samplepoint_standard;
            nodeConfig[i].fastBaudRate.baudrate = fastBaudRate;
            nodeConfig[i].fastBaudRate.samplePoint = samplepoint_fast;
            nodeConfig[i].fastBaudRate.tranceiverDelayOffset = tranceiver_delay_offset;
            CAN_PRINTF_ALWAYS(("baudrate is %d for standard,\r\n%d for data frame\r\n",
                    nodeConfig[i].baudRate.baudrate,
                    nodeConfig[i].fastBaudRate.baudrate));
            CAN_PRINTF_ALWAYS(("samplepoint is %d for standard,\r\n%d for data frame\r\n",
                                nodeConfig[i].baudRate.samplePoint,
                                nodeConfig[i].fastBaudRate.samplePoint));
            CAN_PRINTF_ALWAYS(("tranceiverDelayOffset is %d\r\n", nodeConfig[i].fastBaudRate.tranceiverDelayOffset));
        }
        else
        {
            nodeConfig[i].baudRate.baudrate = baudrate;
            nodeConfig[i].baudRate.samplePoint = samplepoint_standard;
        }

        IfxCan_Can_initNode(&g_Multican.drivers.node[i], &nodeConfig[i]);
        // CAN_DEBUG_PRINTF(("Initial CAN %d\r\n", i));
        IfxStdIf_DPipe_print(io_tmp, "Initial CAN %d\r\n", i);
    }
}

static void IfxMultican_initFilters(uint8 filterMode)
{
    IfxCan_Filter filter[CAN_USED_NODES_NUM];
    memset(filter, 0, sizeof(filter));
    uint8 i;
    for(i=0; i<CAN_USED_NODES_NUM; i++)
    {
#if 0
        // Initialize the filter structure
        filter[i].number = 0;
        filter[i].elementConfiguration = IfxCan_FilterElementConfiguration_storeInRxBuffer;
        filter[i].id1 = FILTER_ID[i];
        filter[i].rxBufferOffset = IfxCan_RxBufferId_0;
        IfxCan_Can_setStandardFilter(&g_Multican.drivers.node[i], &filter[i]);
#else
        // Initialize the filter structure
        // set filter0 for rxBuffer 0
        filter[i].number = 0;
        filter[i].elementConfiguration = IfxCan_FilterElementConfiguration_storeInRxFifo0;
        filter[i].type = IfxCan_FilterType_range;
        if (filterMode == CAN_BRAODCAST_FILTER){
            IfxStdIf_DPipe_print(io_tmp, "CAN %d enable broadcast_mode \r\n",i);
            filter[i].id1 = FILTER_ID[i];
            filter[i].id2 = FILTER_ID[i];
        } else if (filterMode == CAN_UNICAST_FILTER) {
            IfxStdIf_DPipe_print(io_tmp, "CAN %d disable broadcast_mode \r\n",i);
            filter[i].id1 = FILTER_ID_F[i];
            filter[i].id2 = FILTER_ID_F[i];
        }
        filter[i].rxBufferOffset = IfxCan_RxBufferId_0;
        IfxCan_Can_setStandardFilter(&g_Multican.drivers.node[i], &filter[i]);

        // set filter0 for rxBuffer 1
        filter[i].number = 1;
        filter[i].elementConfiguration = IfxCan_FilterElementConfiguration_storeInRxFifo0;
        filter[i].type = IfxCan_FilterType_range;
        filter[i].id1 =  0x7ff;
        filter[i].id2 =  0x7ff;
        filter[i].rxBufferOffset = IfxCan_RxBufferId_1;
        IfxCan_Can_setStandardFilter(&g_Multican.drivers.node[i], &filter[i]);
#endif
    }
    curFilterMode = filterMode;
}

void IfxMultican_initFilters_with_range(uint8 node_id, uint32 range1, uint32 range2, uint32 index, boolean set_ext_filter)
{
	if (!g_Multican.drivers.initialized){
		IfxStdIf_DPipe_print(io_tmp, "Error: need caninit first!\r\n");
		return;
	}
	IfxCan_Filter filter;

	filter.number = index;
	filter.elementConfiguration = IfxCan_FilterElementConfiguration_storeInRxFifo0;
	filter.type = IfxCan_FilterType_range;
	filter.id1 = range1;
	filter.id2 = range2;
	filter.rxBufferOffset = index;

#if CAN_EXT_ID_ENABLED == 1
	if(set_ext_filter)
	{
		IfxStdIf_DPipe_print(io_tmp, "Set CAN %s Extended filter from %d to %d in index %d\r\n",NODE_CONFIG[node_id].name,range1,range2,index);
		IfxCan_Can_setExtendedFilter(&g_Multican.drivers.node[node_id], &filter);
	}else{
		IfxStdIf_DPipe_print(io_tmp, "Set CAN %s Standard filter from %d to %d in index %d\r\n",NODE_CONFIG[node_id].name,range1,range2,index);
		IfxCan_Can_setStandardFilter(&g_Multican.drivers.node[node_id], &filter);
	}
#else
	IfxStdIf_DPipe_print(io_tmp, "Set CAN %s Standard filter from %d to %d in index %d\r\n",NODE_CONFIG[node_id].name,range1,range2,index);
	IfxCan_Can_setStandardFilter(&g_Multican.drivers.node[node_id], &filter);
#endif
}

void IfxMultican_disable_Filters(uint8 node_id, uint32 index, boolean set_ext_filter)
{
	if (!g_Multican.drivers.initialized){
		IfxStdIf_DPipe_print(io_tmp, "Error: need caninit first!\r\n");
		return;
	}
	IfxCan_Filter filter;

	filter.number = index; //IfxCan_RxBufferId_0 ~ 63
	filter.type = IfxCan_FilterType_none;
	filter.elementConfiguration = IfxCan_FilterElementConfiguration_disable;
#if CAN_EXT_ID_ENABLED == 1
	if(set_ext_filter)
	{
		IfxStdIf_DPipe_print(io_tmp, "Disable CAN %s Extended filter %d\r\n",NODE_CONFIG[node_id].name,index);
		IfxCan_Can_setExtendedFilter(&g_Multican.drivers.node[node_id], &filter);
	}else{
		IfxStdIf_DPipe_print(io_tmp, "Disable CAN %s Standard filter %d\r\n",NODE_CONFIG[node_id].name,index);
		IfxCan_Can_setStandardFilter(&g_Multican.drivers.node[node_id], &filter);
	}
#else
	IfxStdIf_DPipe_print(io_tmp, "Disable CAN %s Standard filter %d\r\n",NODE_CONFIG[node_id].name,index);
	IfxCan_Can_setStandardFilter(&g_Multican.drivers.node[node_id], &filter);
#endif
}

#define CAN_WAIT_TIME_MS    100
static boolean Wait_For_Node_Synchronized(uint8 i)
{
    uint32 count = 0;
    while (IfxCan_Can_isNodeSynchronized(&g_Multican.drivers.node[i]) != TRUE)
    {
        count++;
        waitTime(TimeConst[TIMER_INDEX_1MS]);
        if(count > CAN_WAIT_TIME_MS)
        {
            CAN_PRINTF_ALWAYS(("Error: Time out waiting for CAN node %d synchronized\r\n", i));
            return FALSE;
        }
    }
    return TRUE;
}

/** \brief Demo init API
 *
 * This function is called from main during initialization phase
 */
void IfxMultican_init(boolean loopbackMode, uint32 baudrate, boolean useInterrupt, uint8 filterMode)
{
    // create module configuration
//  CAN_DEBUG_PRINTF(("%s\r\n", __func__));

    //UNUSED(useInterrupt);
    UNUSED(loopbackMode);
    IfxCan_Can_Config canConfig[CAN_USED_MODULES_NUM];
    memset(canConfig, 0, sizeof(canConfig));
    uint8 i;
    for(i=0; i<CAN_USED_MODULES_NUM; i++)
    {
        IfxCan_Can_initModuleConfig(&canConfig[i], can_used_module[i]);
        // initialize module
        IfxCan_Can_initModule(&g_Multican.drivers.can[i], &canConfig[i]);
    }
    CAN_DEBUG_PRINTF(("Finish Initial Modules\r\n"));
if (useInterrupt){
    IfxMultican_init_with_interrupt(baudrate,filterMode);
}else{
    IfxMultican_initNodes(baudrate);
    IfxMultican_initFilters(filterMode);
}
    // set the control pins
#if CAN_NODE_3_ENABLE == 1
    can_enable_phy(PHY1);
#endif
#if CAN_NODE_2_ENABLE == 1
    can_enable_phy(PHY3);
#endif
#if (CAN_NODE_0_ENABLE == 1) || (CAN_NODE_1_ENABLE == 1) || \
    (CAN_NODE_4_ENABLE == 1) || (CAN_NODE_5_ENABLE == 1)
    can_enable_phy(PHY0);
#endif

    waitTime(100* TimeConst[TIMER_INDEX_1MS]);
    for(i=0; i<CAN_USED_NODES_NUM; i++)
    {
        Wait_For_Node_Synchronized(i);
    }

    node_a_lp = FALSE;
    node_b_lp = FALSE;
    node_cdef_lp = FALSE;

    CAN_DEBUG_PRINTF(("CAN is ready\r\n"));
    g_Multican.drivers.initialized = TRUE;
}

void sendCanUdp()
{
  ip_addr_t hostIpAddr;
  udp_msg_t msg;
  // Here we change to for loop six queue
int i;
for (i=0;i<6;i++){
  uint32 length, udpIdx = 0;
  boolean sendUdp = FALSE;
  while(!queue_is_empty(g_can_queue[i])){
    cangen_recv_queue_count[i] = cangen_recv_queue_count[i] + 1;
    element_t edata = queue_dequeue(g_can_queue[i]);
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
          //if (cangen_fix_count)
          {
            if ((rxData[0] != 0xAAAAAAAA) || (rxData[1] != 0x55555555)){
              cangen_queue_error_cnt[i] = cangen_queue_error_cnt[i] + 1;
              //IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc,"queue_dequeue_all[%d,%d,%d]: [%d]0x%X 0x%X"ENDL,bus_id,msg_id,msg_length,i,rxData[0], rxData[1]);
              break;
            }
          }
        }
#endif
    if (udpIdx > (UDP_ARRAY_SIZE-QCI_UDP_TOTAL_HEADER_LEN-64)) {
      printf("udp array  is full!\r\n");
      break;
    }
    length = QCI_UDP_TOTAL_HEADER_LEN + (edata.udp_content[16] | edata.udp_content[17]<<8);
    // UDP_HLEN and IP_HLEN would include in pbuf total length
    if (udpIdx+length+IP_HLEN+UDP_HLEN > (g_Lwip.netif.mtu-48)){
      // send
      IP4_ADDR(&hostIpAddr, udp_ip[0], udp_ip[1], udp_ip[2], udp_ip[3]);
      udp_connect(gw_sendback_pcb, &hostIpAddr, QCI_UDP_SERVER_PORT);
      qci_udp_multi_send(gw_sendback_pcb, udp, udpIdx);
      udpSeqNum++;
      memset(udp, udpIdx, 0);
      udpIdx = 0;
      // send lastone
      memcpy(&udp[0], edata.udp_content, length);
      udpIdx += length;
      qci_udp_multi_send(gw_sendback_pcb, udp, udpIdx);
      udpSeqNum++;
      memset(udp, udpIdx, 0);
      udpIdx = 0;
      continue;
    }
    memcpy(&udp[udpIdx], edata.udp_content, length);
    udpIdx += length;
    sendUdp = TRUE;
  }
  if (sendUdp) {
    IP4_ADDR(&hostIpAddr, udp_ip[0], udp_ip[1], udp_ip[2], udp_ip[3]);
    udp_connect(gw_sendback_pcb, &hostIpAddr, QCI_UDP_SERVER_PORT);
    qci_udp_multi_send(gw_sendback_pcb, udp, udpIdx);
    UDP_send_bytes = UDP_send_bytes + udpIdx;
    if(rt_sample.enabled == TRUE){
       rt_sample.point_e = now();       
    }
    udpSeqNum++;
    memset(udp, udpIdx, 0);
    udpIdx = 0;
  }
}

}

void can_to_udp(int canBusId, uint32 id, uint8 *data, uint8 length)
{
    struct qci_udp_packet udpPacket;
    udp_msg_t msg;

    // UDP packet format: Host => VIU
    udpPacket.header.packet_direct = toHOST;
    if (board.aurix_index == MAIN_AURIX)
        udpPacket.header.target_viu = AURIX1; //source
    else udpPacket.header.target_viu = AURIX2;
    udpPacket.header.target_mcu = udp_host;//target
    udpPacket.header.bus_type = typeCAN;
    udpPacket.info_len = QCI_UDP_INFO_LEN + length;
    if (!CheckQueueRem(senderUDPMsg)) udpPacket.info_code = MCU_queue_full;
    else udpPacket.info_code = MCU_scheduled_successfully | (CheckQueueRem(senderUDPMsg)-1);
    udpPacket.canid = id; //message id
    udpPacket.bus_id = canBusId;
    udpPacket.candlc = length;
    udpPacket.rs = 0;
    udpPacket.rs2 = udpSeqNum;

    memcpy(msg.udp_content, &udpPacket, QCI_UDP_TOTAL_HEADER_LEN);
    memcpy(&msg.udp_content[QCI_UDP_TOTAL_HEADER_LEN], data, length);
    if (CheckQueue(senderUDPMsg)) printf("UDP queue is full!\r\n");
    SendMessage(senderUDPMsg, &msg);
}

void print_Can_Rx_Counter(void)
{
    CAN_DEBUG_PRINTF(("Node a : %d, Node b : %d, Node c : %d\n", rxCnt[0], rxCnt[1], rxCnt[2]));
    CAN_DEBUG_PRINTF(("Node d : %d, Node e : %d, Node f : %d\n", rxCnt[3], rxCnt[4], rxCnt[5]));
}

void IfxMultican_PrintMsg(void)
{
    if(!g_Multican.drivers.initialized)
    {
        return;
    }

    // Initialise the message structure with default values, will be replaced by the received values
    IfxCan_Message rxMsg;

    // specify the rx buffer number where the data will be received based on the filter configuration
    //rxMsg.bufferNumber = 0;
    //IfxCan_RxBufferId buffer = IfxCan_RxBufferId_0;

    uint8 i;
    for(i=0; i<CAN_USED_NODES_NUM; i++)
    {
        // wait until node receives the data
        // if (IfxCan_Can_isNewDataReceived(&g_Multican.drivers.node[i], buffer))
        if (IfxCan_Can_getRxFifo0FillLevel(&g_Multican.drivers.node[i]) > 0)
        {
            if (canMode == CAN_NORMAL_MODE) {
                // will be replaced by the received values
                uint32 rxData[2];

                // Initialise the rx message structure with defualt values
                IfxCan_Can_initMessage(&rxMsg);
                rxMsg.readFromRxFifo0 = TRUE;   // enable reading the data from Rx FIFO 0

                // read message
                IfxCan_Can_readMessage(&g_Multican.drivers.node[i], &rxMsg, rxData);
                if (!cangen_enable){
                    CAN_DEBUG_PRINTF(("%s Receive data: 0x%x, 0x%x\r\n", NODE_CONFIG[i].name, rxData[0], rxData[1]));
                }
                if (can_cnt_flag) {
                    if (rxCnt[i] >= can_cnt_print){
                        can_cnt_flag = FALSE;
                        rxCnt[0] = rxCnt[1] = rxCnt[2] = rxCnt[3] = rxCnt[4] = rxCnt[5] = 0;
                        can_cnt_print = 0;
                        return;
                    }
                    print_Can_Rx_Counter();
                }
                rxCnt[i] ++;
                receive_time = (uint32)(now() / TimeConst_1us);
            } else {
                uint8 dlc[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64};
                uint8 udpPacket[QCI_UDP_TOTAL_HEADER_LEN + QCI_UDP_CAN_PAYLOAD_SIZE];
                uint8 length, nquadlets, pad = 0;
                uint32 rxData[16] = {0};

                // Initialise the rx message structure with defualt values
                IfxCan_Can_initMessage(&rxMsg);
                rxMsg.readFromRxFifo0 = TRUE;   // enable reading the data from Rx FIFO 0

                // read message
                IfxCan_Can_readMessage(&g_Multican.drivers.node[i], &rxMsg, rxData);

                length = dlc[rxMsg.dataLengthCode];
                nquadlets = length/4;
                pad = length%4;

                /*Ifx_Console_print(("%s Receive data: ", NODE_CONFIG[i].name));
                for(uint8 m=0; m<nquadlets; m++) {
                    Ifx_Console_print("0x%X ", rxData[m]);
                }*/
                if (pad != 0) {
                    if (pad == 1) rxData[nquadlets] = rxData[nquadlets] & 0xFF;
                    else if (pad == 2) rxData[nquadlets] = rxData[nquadlets] & 0xFFFF;
                    else if (pad == 3) rxData[nquadlets] = rxData[nquadlets] & 0xFFFFFF;
                    //Ifx_Console_print("0x%X ", rxData[nquadlets]);
                }
                //Ifx_Console_print("\r\n");

                can_to_udp(i, rxMsg.messageId, (uint8 *)rxData, length);
            }
        }
    }
}

void IfxMultican_EnableExtendedFrame(boolean isExtendedFrameEnable)
{
    if(isExtendedFrameEnable)
    {
        CAN_PRINTF_ALWAYS(("Enable Ext Frame mode\n"));
    }
    extendedFrame = isExtendedFrameEnable;
}

//boolean IfxMultican_IsExtendedFrameEnabled(void)
//{
//#if CAN_EXT_ID_ENABLED == 1
//        return TRUE;
//#else
//        return extendedFrame;
//#endif
//}

void IfxMultican_SetFDMode(boolean isFdMode, uint32 fast_baudrate)
{
    if(isFdMode)
    {
        CAN_PRINTF_ALWAYS(("Enable CAN FD mode at speed %d\n\r", fast_baudrate));
    }
    else
    {
        //CAN_PRINTF_ALWAYS(("Disable CAN FD mode\n\r"));
        IfxStdIf_DPipe_print(io_tmp, "Disable CAN FD mode\n\r");
    }
    fdMode = isFdMode;
    fastBaudRate = fast_baudrate;
}

void IfxMultican_SetSamplePoint(uint32 samplepoint1, uint32 samplepoint2)
{
    samplepoint_standard = samplepoint1;
    samplepoint_fast = samplepoint2;
}

void IfxMultican_SetTranceiverDelayOffset(uint8 tranceiverDelayOffset)
{
    tranceiver_delay_offset = tranceiverDelayOffset;
}


void IfxMultican_SetNonISOmode(uint8 noniso)
{
     noniso_mode = noniso;
}


/**
 * This function put CAN transceivers into low power mode
 */
void can_xceiver_lp(uint8 xceivernode)
{
    switch(xceivernode)
    {
    case 0:   // harness A
        node_a_lp = TRUE;
        break;
    case 1:   // harness B
        node_b_lp = TRUE;
        break;
    case 2:   // harness C, D, E, F
    case 3:
    case 4:
    case 5:
        node_cdef_lp = TRUE;
        break;
    default:
        // return if the value is out of range
        return;
    }
    can_set_phy_standby_state(NODE_PHY_MAPPING[xceivernode]);
}

/**
 * This function put CAN transceivers into normal mode
 */
void can_xceiver_normal(uint8 xceivernode)
{
    switch(xceivernode)
    {
    case 0:   // harness A
        node_a_lp = FALSE;
        break;
    case 1:   // harness B
        node_b_lp = FALSE;
        break;
    case 2:   // harness C, D, E, F
    case 3:
    case 4:
    case 5:
        node_cdef_lp = FALSE;
        break;
    default:
        // return if the value is out of range
        return;
    }
    can_set_phy_normal_state(NODE_PHY_MAPPING[xceivernode]);
}

void can_length_to_dlc(IfxCan_Message *txMsg, uint8 length)
{
    switch(length)
    {
    case 0:
        txMsg->dataLengthCode = IfxCan_DataLengthCode_0;
        break;
    case 1:
        txMsg->dataLengthCode = IfxCan_DataLengthCode_1;
        break;
    case 2:
        txMsg->dataLengthCode = IfxCan_DataLengthCode_2;
        break;
    case 3:
        txMsg->dataLengthCode = IfxCan_DataLengthCode_3;
        break;
    case 4:
        txMsg->dataLengthCode = IfxCan_DataLengthCode_4;
        break;
    case 5:
        txMsg->dataLengthCode = IfxCan_DataLengthCode_5;
        break;
    case 6:
        txMsg->dataLengthCode = IfxCan_DataLengthCode_6;
        break;
    case 7:
        txMsg->dataLengthCode = IfxCan_DataLengthCode_7;
        break;
    case 8:
        txMsg->dataLengthCode = IfxCan_DataLengthCode_8;
        break;
    case 12:
        txMsg->dataLengthCode = IfxCan_DataLengthCode_12;
        break;
    case 16:
        txMsg->dataLengthCode = IfxCan_DataLengthCode_16;
        break;
    case 20:
        txMsg->dataLengthCode = IfxCan_DataLengthCode_20;
        break;
    case 24:
        txMsg->dataLengthCode = IfxCan_DataLengthCode_24;
        break;
    case 32:
        txMsg->dataLengthCode = IfxCan_DataLengthCode_32;
        break;
    case 48:
        txMsg->dataLengthCode = IfxCan_DataLengthCode_48;
        break;
    case 64:
        txMsg->dataLengthCode = IfxCan_DataLengthCode_64;
        break;
    default:
        // return if the value is out of range
        return;
    }
}

void send_multican_message(uint32 node, uint32 id, uint32 dataLow, uint32 dataHigh, boolean rtr_frame)
{
    // we return directly if the MultiCan is not initialized
    if (g_Multican.drivers.initialized == FALSE)
    {
        Ifx_Console_print(ENDL"Please initialize CAN first with command caninit."ENDL);
        return;
    }

    //data that needs to be transmitted
    if(node >= CAN_USED_NODES_NUM)
    {
        CAN_DEBUG_PRINTF(("Node %d Exceed max support CAN node\r\n", node));
        return;
    }

    //if (id == 0 && curFilterMode != CAN_BRAODCAST_FILTER) {
    //  IfxMultican_initFilters(CAN_BRAODCAST_FILTER);
    //} else if (id > 0 && id < 7 && curFilterMode != CAN_UNICAST_FILTER) {
    //  IfxMultican_initFilters(CAN_UNICAST_FILTER);
    //}

#if 0
    if(id != FILTER_ID[node])
    {
        /* This is an warning for bring up only
         * we will remove it later */
        CAN_PRINTF_ALWAYS(("<Bring up Notice> ID may NOT match the filter id %d\r\n", FILTER_ID[node]));
    }
#endif
    uint32 txData[2];
    txData[0] = dataLow;
    txData[1] = dataHigh;

    // Initialise the message strcture with defualt values
    uint32 canTimeout = 0;
    IfxCan_Message txMsg;
    IfxCan_Can_initMessage(&txMsg);

    // change the relevent members of message structure
    txMsg.bufferNumber = 0;
    txMsg.messageId = id;
    txMsg.storeInTxFifoQueue = TRUE;
#if CAN_EXT_ID_ENABLED == 1
	if(can_config[node].is_id_ext)
	{
		txMsg.messageIdLength = IfxCan_MessageIdLength_extended;//[EXTENED ID test] set message id length
	}
#endif
    txMsg.dataLengthCode = IfxCan_DataLengthCode_8;
    if(can_config[node].is_fd)
    {
        //CAN_PRINTF_ALWAYS(("Prepare FD mode package\r\n"));
        txMsg.frameMode = IfxCan_FrameMode_fdLongAndFast;
    }

    //CAN_DEBUG_PRINTF(("Prepare to send the data\r\n"));
    // Transmit Data
    while (IfxCan_Can_sendMessage(&g_Multican.drivers.node[node], &txMsg, txData) == IfxCan_Status_notSentBusy)
    {
        if (CmdTest.flag && CmdTest.Mode == CAN_CMD_TEST_TP) {
            // break if buffer is full in throughput test
            break;
        }
        IfxStm_waitTicks(&MODULE_STM0, IfxStm_getTicksFromMilliseconds(&MODULE_STM0, 1));
        canTimeout++;
        if(canTimeout >= CAN_BUS_TIMEOUT)
        {
            CAN_DEBUG_PRINTF(("CAN Transmitting Data failed...\r\n"));
            break;
        }
    }
    //CAN_DEBUG_PRINTF(("Send out the data\r\n"));
}

void send_multican_message_gw(uint32 node, uint32 id, uint8 length, uint32* data, boolean rtr_frame)  //ycnew
{
    // we return directly if the MultiCan is not initialized
    if (g_Multican.drivers.initialized == FALSE)
    {
        Ifx_Console_print(ENDL"Please initialize CAN first with command caninit."ENDL);
        return;
    }
    
    //data that needs to be transmitted
    if(node >= CAN_USED_NODES_NUM)
    {
        CAN_DEBUG_PRINTF(("Node %d Exceed max support CAN node\r\n", node));
        return;
    }

    //if (id == 0 && curFilterMode != CAN_BRAODCAST_FILTER) {
    //  IfxMultican_initFilters(CAN_BRAODCAST_FILTER);
    //} else if (id > 0 && id < 7 && curFilterMode != CAN_UNICAST_FILTER) {
    //  IfxMultican_initFilters(CAN_UNICAST_FILTER);
    //}

    //if(id != FILTER_ID[node])
    //{
        /* This is an warning for bring up only
         * we will remove it later */
    //  CAN_PRINTF_ALWAYS(("<Bring up Notice> ID may NOT match the filter id %d\r\n", FILTER_ID[node]));
    //}
    // Initialise the message strcture with defualt values
    uint32 canTimeout = 0;
    IfxCan_Message txMsg;
    IfxCan_Can_initMessage(&txMsg);

    // change the relevent members of message structure
    txMsg.bufferNumber = 0;
    txMsg.messageId = id;
    txMsg.storeInTxFifoQueue = TRUE;
#if CAN_EXT_ID_ENABLED == 1
	if(can_config[node].is_id_ext)
	{
		txMsg.messageIdLength = IfxCan_MessageIdLength_extended;//[EXTENED ID test] set message id length
	}
#endif	
    can_length_to_dlc(&txMsg, length);
    if(can_config[node].is_fd)
    {
        //CAN_PRINTF_ALWAYS(("Prepare FD mode package\r\n"));
        txMsg.frameMode = IfxCan_FrameMode_fdLongAndFast;
    }
    if(rt_sample.enabled == TRUE){
       rt_sample.point_c = now();
    }
    //CAN_DEBUG_PRINTF(("Prepare to send the data\r\n"));
    // Transmit Data
    while( IfxCan_Can_sendMessage(&g_Multican.drivers.node[node], &txMsg, data) == IfxCan_Status_notSentBusy )
    {
        IfxStm_waitTicks(&MODULE_STM0, IfxStm_getTicksFromMilliseconds(&MODULE_STM0, 1));
        canTimeout++;
        if(canTimeout >= CAN_BUS_TIMEOUT)
        {
            CAN_DEBUG_PRINTF(("CAN Transmitting Data failed...\r\n"));
            break;
        }
    }
    //CAN_DEBUG_PRINTF(("[%s] Send out the data\r\n",__func__));
}

void run_canstress_test()  //ycnew
{
	if (!canstress_running)
	{
		IfxMultican_SetFDMode(FALSE, 0);
		IfxMultican_init(FALSE, 1000000, FALSE, TRUE);
		send_multican_message(0, 0, 0x55555555, 0x55555555, FALSE);
		send_multican_message(1, 0, 0x55555555, 0x55555555, FALSE);
		send_multican_message(2, 0, 0x55555555, 0x55555555, FALSE);
		send_multican_message(3, 0, 0x55555555, 0x55555555, FALSE);
		send_multican_message(4, 0, 0x55555555, 0x55555555, FALSE);
		send_multican_message(5, 0, 0x55555555, 0x55555555, FALSE);
		canstress_running = TRUE;
	}
}

//TODO: we can optimize it to be a table, right now keep it as it's for bring up
/* we should set en pin active, set stb pin de-active */
void can_enable_phy(e_phy phy_id)
{
    switch(phy_id)
    {
    case PHY0:
        /* There's no enable pin for PHY0 */
#ifdef CAN_PHY0_STB
        IfxPort_setPinLow(CAN_PHY0_STB.port, CAN_PHY0_STB.pinIndex);
        IfxPort_setPinModeOutput(CAN_PHY0_STB.port, CAN_PHY0_STB.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
#endif //endof  #ifdef CAN_PHY0_STB
#ifdef CAN_PHY0_STB_EXP
        if(board.aurix_index == MAIN_AURIX){
        SET_GPIOEXP_OUTPUT_LOW(CAN_PHY0_STB_EXP);
        SET_GPIOEXP_CONFIG_OUTPUT(CAN_PHY0_STB_EXP);
        }
#endif
        break;

    case PHY1:
#ifdef CAN_PHY1_EN
#if CAN_PHY1_ENABLE_HIGH_LEVEL == 0
        IfxPort_setPinLow(CAN_PHY1_EN.port, CAN_PHY1_EN.pinIndex);
#else
        IfxPort_setPinHigh(CAN_PHY1_EN.port, CAN_PHY1_EN.pinIndex);
#endif

        IfxPort_setPinHigh(CAN_PHY1_STB.port, CAN_PHY1_STB.pinIndex);
        IfxPort_setPinModeOutput(CAN_PHY1_EN.port, CAN_PHY1_EN.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
        IfxPort_setPinModeOutput(CAN_PHY1_STB.port, CAN_PHY1_STB.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
#endif //endof  #ifdef CAN_PHY1_EN
#ifdef CAN_PHY1_EN_EXP
        if(board.aurix_index == MAIN_AURIX){
        SET_GPIOEXP_OUTPUT_HIGH(CAN_PHY1_EN_EXP);
        SET_GPIOEXP_CONFIG_OUTPUT(CAN_PHY1_EN_EXP);
        }
#endif
#ifdef CAN_PHY1_STB_N_EXP
        if(board.aurix_index == MAIN_AURIX){
        SET_GPIOEXP_OUTPUT_HIGH(CAN_PHY1_STB_N_EXP);
        SET_GPIOEXP_CONFIG_OUTPUT(CAN_PHY1_STB_N_EXP);
        }
#endif
        break;

    case PHY3:
#ifdef CAN_PHY3_EN
#if CAN_PHY3_ENABLE_HIGH_LEVEL == 0
        IfxPort_setPinLow(CAN_PHY3_EN.port, CAN_PHY3_EN.pinIndex);
#else
        IfxPort_setPinHigh(CAN_PHY3_EN.port, CAN_PHY3_EN.pinIndex);
#endif
        IfxPort_setPinHigh(CAN_PHY3_STB.port, CAN_PHY3_STB.pinIndex);
        IfxPort_setPinModeOutput(CAN_PHY3_EN.port, CAN_PHY3_EN.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
        IfxPort_setPinModeOutput(CAN_PHY3_STB.port, CAN_PHY3_STB.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
#endif //endof #ifdef CAN_PHY3_EN
#ifdef CAN_PHY3_EN_EXP
        if(board.aurix_index == MAIN_AURIX){
        SET_GPIOEXP_OUTPUT_HIGH(CAN_PHY3_EN_EXP);
        SET_GPIOEXP_CONFIG_OUTPUT(CAN_PHY3_EN_EXP);
        }
#endif
#ifdef CAN_PHY3_STB_N_EXP
        if(board.aurix_index == MAIN_AURIX){
        SET_GPIOEXP_OUTPUT_HIGH(CAN_PHY3_STB_N_EXP);
        SET_GPIOEXP_CONFIG_OUTPUT(CAN_PHY3_STB_N_EXP);
        }
#endif
        break;

#if PG2_PREEVT == 1
    case X1_PHY:
        if(board.aurix_index == MAIN_AURIX){
        SET_GPIOEXP_OUTPUT_LOW(AURIX_XA_CAN_EN_N_EXP);
        SET_GPIOEXP_CONFIG_OUTPUT(AURIX_XA_CAN_EN_N_EXP);
        }
        break;
    case X2_PHY:
        if(board.aurix_index == MAIN_AURIX){
        SET_GPIOEXP_OUTPUT_LOW(AURIX_XB_CAN_EN_N_EXP);
        SET_GPIOEXP_CONFIG_OUTPUT(AURIX_XB_CAN_EN_N_EXP);
        }
        break;
    case P_PHY:
        if(board.project == E3550)
        {
            SET_GPIOEXP_OUTPUT_LOW(E3550_AURIX_P1_CAN_EN_N_EXP);
            SET_GPIOEXP_CONFIG_OUTPUT(E3550_AURIX_P1_CAN_EN_N_EXP);
        }
        break;
#else
    case TEGRA_PHY:
        IfxPort_setPinLow(CAN_TEG_EN_N.port, CAN_TEG_EN_N.pinIndex);
        IfxPort_setPinModeOutput(CAN_TEG_EN_N.port, CAN_TEG_EN_N.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
        break;
    case CVM_PHY:
        IfxPort_setPinLow(CAN_CVM_EN_N.port, CAN_CVM_EN_N.pinIndex);
        IfxPort_setPinModeOutput(CAN_CVM_EN_N.port, CAN_CVM_EN_N.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
        break;
#endif
    default:
        break;
    }
}

// we set can into standby state
void can_set_phy_standby_state(e_phy phy_id)
{
    switch(phy_id)
    {
    case PHY0:
#ifdef CAN_PHY0_STB
        IfxPort_setPinHigh(CAN_PHY0_STB.port, CAN_PHY0_STB.pinIndex);
        IfxPort_setPinModeOutput(CAN_PHY0_STB.port, CAN_PHY0_STB.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
#endif
#ifdef CAN_PHY0_STB_EXP
        if(board.aurix_index == MAIN_AURIX){
        SET_GPIOEXP_OUTPUT_HIGH(CAN_PHY0_STB_EXP);
        SET_GPIOEXP_CONFIG_OUTPUT(CAN_PHY0_STB_EXP);
        }
#endif
        break;
    case PHY1:
#ifdef CAN_PHY1_STB
        IfxPort_setPinLow(CAN_PHY1_STB.port, CAN_PHY1_STB.pinIndex);
        IfxPort_setPinModeOutput(CAN_PHY1_STB.port, CAN_PHY1_STB.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
#endif
#ifdef CAN_PHY1_STB_N_EXP
        if(board.aurix_index == MAIN_AURIX){
        SET_GPIOEXP_OUTPUT_LOW(CAN_PHY1_STB_N_EXP);
        SET_GPIOEXP_CONFIG_OUTPUT(CAN_PHY1_STB_N_EXP);
        }
#endif
        break;
    case PHY3:
#ifdef CAN_PHY3_STB
        IfxPort_setPinLow(CAN_PHY3_STB.port, CAN_PHY3_STB.pinIndex);
        IfxPort_setPinModeOutput(CAN_PHY3_STB.port, CAN_PHY3_STB.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
#endif
#ifdef CAN_PHY3_STB_N_EXP
        if(board.aurix_index == MAIN_AURIX){
        SET_GPIOEXP_OUTPUT_LOW(CAN_PHY3_STB_N_EXP);
        SET_GPIOEXP_CONFIG_OUTPUT(CAN_PHY3_STB_N_EXP);
        }
#endif
        break;

    /* Set Enable pin to de-active, which will set PHY into stanby */
#if PG2_PREEVT == 1
    case X1_PHY:
        if(board.aurix_index == MAIN_AURIX){
        SET_GPIOEXP_OUTPUT_LOW(AURIX_XA_CAN_EN_N_EXP);
        SET_GPIOEXP_CONFIG_OUTPUT(AURIX_XA_CAN_EN_N_EXP);
        }
        break;
    case X2_PHY:
        if(board.aurix_index == MAIN_AURIX){
        SET_GPIOEXP_OUTPUT_LOW(AURIX_XB_CAN_EN_N_EXP);
        SET_GPIOEXP_CONFIG_OUTPUT(AURIX_XB_CAN_EN_N_EXP);
        }
        break;
    case P_PHY:
        //need to check
        if(board.project == E3550)
        {
            SET_GPIOEXP_OUTPUT_LOW(E3550_AURIX_P1_CAN_EN_N_EXP);
            SET_GPIOEXP_CONFIG_OUTPUT(E3550_AURIX_P1_CAN_EN_N_EXP);
        }
        //raby original porting, but close now for sync code
//          SET_GPIOEXP_OUTPUT_LOW(E3550_AURIX_P1_CAN_EN_N_EXP);
//          SET_GPIOEXP_CONFIG_OUTPUT(E3550_AURIX_P1_CAN_EN_N_EXP);
        break;
#else
    case TEGRA_PHY:
        IfxPort_setPinHigh(CAN_TEG_EN_N.port, CAN_TEG_EN_N.pinIndex);
        IfxPort_setPinModeOutput(CAN_TEG_EN_N.port, CAN_TEG_EN_N.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
        break;
    case CVM_PHY:
        IfxPort_setPinHigh(CAN_CVM_EN_N.port, CAN_CVM_EN_N.pinIndex);
        IfxPort_setPinModeOutput(CAN_CVM_EN_N.port, CAN_CVM_EN_N.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
        break;
#endif
    default:
        break;
    }
}

// we set can out of standby state into normal state
void can_set_phy_normal_state(e_phy phy_id)
{
    switch(phy_id)
    {
    case PHY0:
#ifdef CAN_PHY0_STB
        IfxPort_setPinLow(CAN_PHY0_STB.port, CAN_PHY0_STB.pinIndex);
        IfxPort_setPinModeOutput(CAN_PHY0_STB.port, CAN_PHY0_STB.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
#endif
#ifdef CAN_PHY0_STB_EXP
        SET_GPIOEXP_OUTPUT_LOW(CAN_PHY0_STB_EXP);
        SET_GPIOEXP_CONFIG_OUTPUT(CAN_PHY0_STB_EXP);
#endif
        break;
    case PHY1:
#ifdef CAN_PHY1_STB
        IfxPort_setPinHigh(CAN_PHY1_STB.port, CAN_PHY1_STB.pinIndex);
        IfxPort_setPinModeOutput(CAN_PHY1_STB.port, CAN_PHY1_STB.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
#endif
#ifdef CAN_PHY1_STB_N_EXP
        SET_GPIOEXP_OUTPUT_HIGH(CAN_PHY1_STB_N_EXP);
        SET_GPIOEXP_CONFIG_OUTPUT(CAN_PHY1_STB_N_EXP);
#endif
        break;
    case PHY3:
#ifdef CAN_PHY3_STB
        IfxPort_setPinHigh(CAN_PHY3_STB.port, CAN_PHY3_STB.pinIndex);
        IfxPort_setPinModeOutput(CAN_PHY3_STB.port, CAN_PHY3_STB.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
#endif
#ifdef CAN_PHY3_STB_N_EXP
        SET_GPIOEXP_OUTPUT_HIGH(CAN_PHY3_STB_N_EXP);
        SET_GPIOEXP_CONFIG_OUTPUT(CAN_PHY3_STB_N_EXP);
#endif
        break;
    default:
        break;
    }
}

#endif  //endof #if MCM_CAN_TC39X_ENABLE == 1

#if 0
void sendCanUdp_from_UDPMsg()
{
    ip_addr_t hostIpAddr;
    udp_msg_t msg;
    uint8 udp[700] = {0};
    uint32 length, udpIdx = 0;
    boolean sendUdp = FALSE;

    while(ReceiveMessage(receiverUDPMsg, &msg) != E_COM_NOMSG) {
        if (udpIdx > (700-QCI_UDP_TOTAL_HEADER_LEN-64)) {
            printf("udp array  is full!\r\n");
            break;
        }
        length = QCI_UDP_TOTAL_HEADER_LEN + (msg.udp_content[16] | msg.udp_content[17]<<8);
        memcpy(&udp[udpIdx], msg.udp_content, length);
        udpIdx += length;
        sendUdp = TRUE;
    }

    if (sendUdp) {
        IP4_ADDR(&hostIpAddr, udp_ip[0], udp_ip[1], udp_ip[2], udp_ip[3]);
        udp_connect(gw_sendback_pcb, &hostIpAddr, QCI_UDP_SERVER_PORT);
        qci_udp_multi_send(gw_sendback_pcb, udp, udpIdx);
        udpSeqNum++;
        memset(udp, udpIdx, 0);
        udpIdx = 0;
    }
}
#endif
