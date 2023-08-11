/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#include "phy.h"
#if ETH_PHY_88Q2112_ENABLE == 1
#include "Phy_88q2112.h"
#endif
#include "internal_fw_debug.h"
#include "board.h"

extern boolean boot_msg_flag;
#define PHY_PRINTF_ALWAYS(x)         DEBUG_PRINTF_ALWAYS(x)
#define PRINTF_BOOT(x)       ({if(boot_msg_flag){DEBUG_PRINTF_ALWAYS(x);}})

void Phy_88EA1512_init()
{
	uint32 phyaddr = 0x03; //Fixme : This is with SPC-3
	IfxEth_Cfg_Aurix_MDC_5050();

	// Program 88EA1512 in SGMII to Copper mode
	IfxEth_write_mdio_clause_22(phyaddr, 0x16, 0x12); //Set PHY page 18
	IfxEth_write_mdio_clause_22(phyaddr, 0x14, 0x8001); // 1512: General control register 1: Set mode = 0b01 (SGMII-Copper)
}

void B2B_Ethernet_Phy_Re_Initial(void)
{
  uint8 i;
	PHY_PRINTF_ALWAYS(("B2B_Ethernet_Phy_Re_Initial \r\n"));
	for(i=5;i<=6;i++){
		PHY_PRINTF_ALWAYS(("B2B_Ethernet_Phy_Re_Initial %d \r\n",i));
		IfxEth_Cfg_Aurix_MDC(i);
	  IfxEth_Phy_88q2112_b2b_re_init(i);	
	}
}


#if 1
void Ethernet_Phy_Initial(uint32 devices)
{
	uint8 i;
#if ETH_PHY_88Q2112_ENABLE == 1
	uint32 phy_number = get_88q2112_number();
	if((devices & phy_88q2112) != 0)
	{
		PRINTF_BOOT(("Initial 88Q2112\r\n"));
		for(i = IFXETH_PHY_88Q2112_1_PHYID; i <= phy_number; i++)
		{
			IfxEth_Phy_88q2112_init(i);
		}
		if(board.project == E3550)
			Phy_88EA1512_init();
	}
#endif
}
#endif

void Ethernet_Phy_DeInitial(uint32 devices)
{
	uint8 i;
#if ETH_PHY_88Q2112_ENABLE == 1
	uint32 phy_number = get_88q2112_number();
	if((devices & phy_88q2112) != 0)
	{
		for(i = IFXETH_PHY_88Q2112_1_PHYID; i <= phy_number; i++)
		{
			IfxEth_Phy_88q2112_deinit(i);
		}
	}
#endif
}
