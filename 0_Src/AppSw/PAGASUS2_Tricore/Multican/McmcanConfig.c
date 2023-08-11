/*
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
#if MCM_CAN_TC39X_ENABLE == 1
#include "Mcmcan.h"

Ifx_CAN * can_used_module[CAN_USED_MODULES_NUM]=
{
	&MODULE_CAN0,
	&MODULE_CAN1,
	&MODULE_CAN2
};

const IfxCan_Can_Pins CAN_A_PIN =
{
		&CAN_PHY1_TXD,
		IfxPort_OutputMode_pushPull,
		&CAN_PHY1_RXD,
		IfxPort_InputMode_pullUp,
		IfxPort_PadDriver_cmosAutomotiveSpeed1,
};

const IfxCan_Can_Pins CAN_B_PIN =
{
		&CAN_PHY3_TXD,
		IfxPort_OutputMode_pushPull,
		&CAN_PHY3_RXD,
		IfxPort_InputMode_pullUp,
		IfxPort_PadDriver_cmosAutomotiveSpeed1,
};

const IfxCan_Can_Pins CAN_C_PIN =
{
		&CANR1_TXD,
		IfxPort_OutputMode_pushPull,
		&CANR1_RXD,
		IfxPort_InputMode_pullUp,
		IfxPort_PadDriver_cmosAutomotiveSpeed1,
};

const IfxCan_Can_Pins CAN_D_PIN =
{
		&CANR0_TXD,
		IfxPort_OutputMode_pushPull,
		&CANR0_RXD,
		IfxPort_InputMode_pullUp,
		IfxPort_PadDriver_cmosAutomotiveSpeed1,
};

const IfxCan_Can_Pins CAN_E_PIN =
{
		&CAN0_TXD,
		IfxPort_OutputMode_pushPull,
		&CAN0_RXD,
		IfxPort_InputMode_pullUp,
		IfxPort_PadDriver_cmosAutomotiveSpeed1,
};

const IfxCan_Can_Pins CAN_F_PIN =
{
		&CAN_PHY2_TXD,
		IfxPort_OutputMode_pushPull,
		&CAN_PHY2_RXD,
		IfxPort_InputMode_pullUp,
		IfxPort_PadDriver_cmosAutomotiveSpeed1,
};
#if START_KIT_ENABLE == 1
const IfxCan_Can_Pins START_KIT_CAN_00_PIN =
{
		&CAN_00_TXD,
		IfxPort_OutputMode_pushPull,
		&CAN_00_RXD,
		IfxPort_InputMode_pullUp,
		IfxPort_PadDriver_cmosAutomotiveSpeed1,
};

const IfxCan_Can_Pins START_KIT_CAN_10_PIN =
{
		&CAN_10_TXD,
		IfxPort_OutputMode_pushPull,
		&CAN_10_RXD,
		IfxPort_InputMode_pullUp,
		IfxPort_PadDriver_cmosAutomotiveSpeed1,
};
#endif

const t_node_config NODE_CONFIG[]=
{
	{0, IfxCan_NodeId_3, &CAN_A_PIN, "CAN_A"}, //CAN_PHY_A
	{2, IfxCan_NodeId_3, &CAN_B_PIN, "CAN_B"}, //CAN_PHY_B
	{0, IfxCan_NodeId_1, &CAN_C_PIN, "CAN_C"}, //CAN_PHY_C
	{1, IfxCan_NodeId_3, &CAN_D_PIN, "CAN_D"}, //CAN_PHY_D
#if START_KIT_ENABLE == 1
	{0, IfxCan_NodeId_0, &START_KIT_CAN_00_PIN, "START_KIT_CAN_00"}, //START_KIT_00
	{1, IfxCan_NodeId_0, &START_KIT_CAN_10_PIN, "START_KIT_CAN_10"}, //START_KIT_10
#else
	{1, IfxCan_NodeId_1, &CAN_E_PIN, "CAN_E"}, //CAN_PHY_E
	{1, IfxCan_NodeId_0, &CAN_F_PIN, "CAN_F"}, //CAN_PHY_F
#endif
};

// config the CAN HARNESS A-F mapping to real PHY transceiver ID
// index is node ( 0=A, 1=B ...)
// value is the phy id
const e_phy NODE_PHY_MAPPING[]=
{
	PHY1,			//harness A
	PHY3,			//harness B
	PHY0,			//harness C
	PHY0,			//harness D
	PHY0,			//harness E
	PHY0			//harness F
};

/* define the filter ID for each nodes */
const uint32 FILTER_ID[6]=
{
	0, //CAN_PHY_A
	0, //CAN_PHY_B
	0, //CAN_PHY_C
	0, //CAN_PHY_D
	0, //CAN_PHY_E
	0, //CAN_PHY_F
};

/* define the filter ID for each nodes (0~5) */
const uint32 FILTER_ID_F[6]=
{
	1, //CAN_PHY_A
	2, //CAN_PHY_B
	3, //CAN_PHY_C
	4, //CAN_PHY_D
	5, //CAN_PHY_E
	6, //CAN_PHY_F
};

#endif  //endof #if MCM_CAN_TC39X_ENABLE == 1
