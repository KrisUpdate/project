/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef _MCMCAN_H
#define _MCMCAN_H

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "Configuration.h"
#if MCM_CAN_TC39X_ENABLE == 1
#include <Can/Can/IfxCan_Can.h>

#define CAN_PORT_NUM 6
#define CAN_BUS_TIMEOUT 200 //ms
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define CAN_USED_MODULES_NUM           (3)
#define CAN_USED_NODES_NUM    		   (6)

#define CAN_NORMAL_MODE	    0
#define CAN_GATEWAY_MODE	1

#define CAN_BRAODCAST_FILTER	0
#define CAN_UNICAST_FILTER	1

#define CAN_FILTER_INDEX_NUM  2
#define CAN_FILTER_RANGE_NUM  2

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/
typedef enum
{
	PHY0,
	PHY1,
	PHY3,
#if PG2_PREEVT == 1
	X1_PHY,
	X2_PHY,
	P_PHY,
#else
	TEGRA_PHY,
	CVM_PHY,
#endif
	MAX_PHY,
}e_phy;

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
typedef struct
{
	uint8 module_index;
	IfxCan_NodeId node_id;
	const IfxCan_Can_Pins* pin;
	const char * name;
}t_node_config;

#if CAN_INTERRUPT_ENABLE == 1
typedef struct
{
	volatile boolean isDataLost;
	uint16 cnt;			/**< \brief CAN received message number */
	volatile uint16 valid;	    /**< \brief CAN received message valid */
	uint32 id[16];      /**< \brief CAN received id */
	uint32 data0[16];   /**< \brief CAN received data 0 */
	uint32 data1[16];   /**< \brief CAN received data 1 */
} Can_RecMsg;
#endif

typedef struct
{
    struct
    {
    	IfxCan_Can can[CAN_USED_MODULES_NUM];
    	IfxCan_Can_Node node[CAN_USED_NODES_NUM];
 //       IfxMultican_Can        can;                  /**< \brief CAN driver handle */
 //       IfxMultican_Can        canr;                 /**< \brief CANR driver handle */
 //       IfxMultican_Can_Node   canNode[6];           /**< \brief CAN Source Node 0..6 */
 //       IfxMultican_Can_MsgObj canRecMsgObj[6];      /**< \brief CAN Receive Message object */
 //      IfxMultican_Can_MsgObj canTransmitMsgObj[6]; /**< \brief CAN Transmit Message object */
        boolean                initialized;          /**< \brief CAN Initialized if set */
    }drivers;
#if CAN_INTERRUPT_ENABLE == 1
	boolean                ndat[CAN_USED_NODES_NUM];              /**< \brief CAN received data (new data) */
    uint32                 receivedId[CAN_USED_NODES_NUM];        /**< \brief CAN last received id */
    uint32                 receivedData0[CAN_USED_NODES_NUM];     /**< \brief CAN last received data 0 */
    uint32                 receivedData1[CAN_USED_NODES_NUM];     /**< \brief CAN last received data 1 */
    Can_RecMsg             Can_Rec[CAN_USED_NODES_NUM];           /**< \brief CAN received data struct */
#endif
} App_Multican;

typedef struct _CansetupConfig
{
	uint32 classic_baudrate;
	boolean is_fd;
	uint32 fd_baudrate;
	uint32 delay;
    uint32 samplepoint_standard;
    uint32 samplepoint_fast;
    uint32 noniso;
    uint32 std_filter[CAN_FILTER_INDEX_NUM][CAN_FILTER_RANGE_NUM];//[index 0,1][range]
    uint32 ext_filter[CAN_FILTER_INDEX_NUM][CAN_FILTER_RANGE_NUM];//[index 0,1][range]
    char is_id_ext;
}CansetupConfig;

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
IFX_EXTERN App_Multican g_Multican;
extern volatile boolean node_a_lp;
extern volatile boolean node_b_lp;
extern volatile boolean node_cdef_lp;
extern volatile boolean can_cnt_flag;
extern volatile boolean fdMode;
extern uint32 can_cnt_print;
static boolean canstress_running = FALSE;

extern uint32 receive_time;
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
IFX_EXTERN void IfxMultican_init(boolean loopbackMode, uint32 baudrate, boolean useInterrupt, uint8 fileterMode);
IFX_EXTERN void can_to_udp(int canBusId, uint32 id, uint8 *data, uint8 length);
IFX_EXTERN void IfxMultican_PrintMsg(void);
IFX_EXTERN void IfxMultican_EnableExtendedFrame(boolean isExtendedFrameEnable);
IFX_EXTERN boolean IfxMultican_IsExtendedFrameEnabled(void);
IFX_EXTERN void IfxMultican_SetFDMode(boolean isFdMode, uint32 fast_baudrate);
IFX_EXTERN void IfxMultican_SetSamplePoint(uint32 samplepoint1, uint32 samplepoint2);
IFX_EXTERN void IfxMultican_SetTranceiverDelayOffset(uint8 tranceiverDelayOffset);
IFX_EXTERN void IfxMultican_SetNonISOmode(uint8 noniso);
IFX_EXTERN void send_multican_message(uint32 node, uint32 id, uint32 dataLow, uint32 dataHigh, boolean rtr_frame);
IFX_EXTERN void send_multican_message_gw(uint32 node, uint32 id, uint8 length, uint32* data, boolean rtr_frame);
IFX_EXTERN void can_xceiver_lp(uint8 xceivernode);
IFX_EXTERN void can_xceiver_normal(uint8 xceivernode);
IFX_EXTERN void run_canstress_test();
IFX_EXTERN void QciCan_Rx_Wrap();
IFX_EXTERN void IfxMultican_initFilters_with_range(uint8 node_id, uint32 range1, uint32 range2, uint32 index, boolean set_ext_filter);//[EXTENED ID test]
IFX_EXTERN void IfxMultican_disable_Filters(uint8 node_id, uint32 index, boolean set_ext_filter);//[EXTENED ID test]
/*\brief Enable CAN PHY transceivers
 *\param: in: e_phy phy_id: PHY ID
 *\return void
 */
IFX_EXTERN void can_enable_phy(e_phy phy_id);

/*\brief Set CAN PHY transceivers into stanby state
 *\param: in: e_phy phy_id: PHY ID
 *\return void
 */
IFX_EXTERN void can_set_phy_standby_state(e_phy phy_id);


/*\brief Set CAN PHY transceivers into normal state (out from standby)
 *\param: in: e_phy phy_id: PHY ID
 *\return void
 *\note: As Tegra PHY transceivers standby pin is not connected with Aurix,
 *\note: This function doesn't support Tegra PHY
 *\note: To set Tegra PHY into normal state, use can_enable_phy()
 */
IFX_EXTERN void can_set_phy_normal_state(e_phy phy_id);

/*TRIBOARD*/
IFX_EXTERN void QciCan_Rx_Wrap();
#endif //#if MCM_CAN_TC39X_ENABLE == 1
#endif /*_MCMCAN_H*/
