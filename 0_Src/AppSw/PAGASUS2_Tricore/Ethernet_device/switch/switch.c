/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#include "switch.h"
#include "marvell_switch.h"
#include "switch_sja1105.h"

IFX_EXTERN void Ethernet_Switch_Initial(uint32 devices)
{
#if ETH_NXP_SWITCH_SJA1105_ENABLE == 1
	if((devices & switch_sja1105) != 0)
	{
		Switch_SJA1105_Initial();
	}
	if((devices & switch_sja1105_step2) != 0)
	{
		Switch_SJA1105_Step2_Init();
	}
#endif
#if ETH_MARVELL_SWITCH_ENABLE == 1 && MARVELL_SWITCH_DEBUG_ENABLE != 1
	if((devices & switch_88e6321) != 0)
	{
#if 1
		IfxEth_Marvell_Sw_Init(IFXETH_SWITCH_88E6321_1_ADDR);
		IfxEth_Marvell_Sw_Init(IFXETH_SWITCH_88E6321_2_ADDR);
#endif
	}
#endif

}
