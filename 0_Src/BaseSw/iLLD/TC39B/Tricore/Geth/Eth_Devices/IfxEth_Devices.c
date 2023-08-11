/**
 * \file IfxEth_Devices.c
 * \brief ETH DEVICES details
 *
 * \version iLLD_1_0_0_5_0
 * \copyright Copyright (c) 2013 Infineon Technologies AG. All rights reserved.
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
 *
 */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include <Configuration.h>
#if PROJECT == PROJECT_PAGASUS2 || PROJECT == PROJECT_V31C || PROJECT == PROJECT_V3NA
#include <Geth/Eth_Devices/IfxEth_Devices.h>
#include <internal_fw_debug.h>

#include <gpioexp.h>
#include <gpioexp_pinmap.h>
#include <_PinMap/IfxGeth_PinMap.h>
//#include <Ethernet/switch/marvell_switch.h>
#include <marvell_switch.h>
#include "I2cRom.h"
#include "board.h"
#include "mdio_gpio.h"
#include "Geth/Std/IfxGeth.h"

/******************************************************************************/
/*----------------------------------Macros------------------------------------*/
/******************************************************************************/
#define ETH_ILLD_DRV_DEBUG_ENABLE 0//[hint] v31c will cause booting issue when enable flag

#if ETH_ILLD_DRV_DEBUG_ENABLE == 1
#define ETH_ILLD_DRV_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define ETH_ILLD_DRV_DEBUG_PRINTF(x)
#endif

#define ETH_ILLD_DRV_ALWAYS_PRINTF(x)  DEBUG_PRINTF_ALWAYS(x)


#define MDIO_LOGIC_OUTPUT_DELAY_MS   1

/******************************************************************************/
/*-----------------------Exported Variables/Constants-------------------------*/
/******************************************************************************/

uint32 IfxEth_IsModuleInitDone = 0;
uint32 IfxEth_Phy_Id_1 = 0;
uint32 IfxEth_Phy_Id_2= 0;

static boolean is_mdio_up = FALSE;
static boolean is_mdio_gpio_enable = FALSE;
static uint32 prev_slave_address = 0xFFFF;

/******************************************************************************/
/*---------------------------- static Function  -------------------------------*/
/******************************************************************************/
//static void IfxEth_Cfg_Aurix_MDC_P3479(uint32 slaveaddr);
//static void IfxEth_Cfg_Aurix_MDC_E3550(uint32 slaveaddr);

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

uint32 IfxEth_Module_Init(void)
{

	ETH_ILLD_DRV_DEBUG_PRINTF(("%s\r\n", __func__));
#if START_KIT_ENABLE == 1
	IfxEth_Phy_Pef7071_init();
#else
	IFXGETH_PHY_WAIT_GMII_READY();
    /*TODO This affect the UDP receiving, why?*/
    GETH_GPCTL.B.ALTI3 = 0x3; // ETHCOLD
    GETH_GPCTL.B.ALTI5 = 0x3; // ETHRXERD

    //Phy inside Marvell switch is not accessible.  Phy is configured with strapping resistors
#endif
    IfxEth_IsModuleInitDone = 1;

    return IfxEth_IsModuleInitDone;

}


boolean IfxEth_Phy_link(void)
{
	uint32 value;
    boolean linkEstablished = FALSE;

#if START_KIT_ENABLE == 1
    return IfxEth_Phy_Pef7071_link();
#else
    if (IfxEth_IsModuleInitDone)
    {
#if BROAD_REACH_PHY_ENABLE == 1
		IfxEth_read_mdio_reg(IFXETH_BROADR_PHY_SMI_DEVICE_ADDR, IFXETH_BROADR_PHY_LRE_STATUS_REGISTER_ADDR, &value);
		linkEstablished = ((value & IFXETH_BROADR_PHY_LINK_STATUS_BIT) !=0) ? TRUE : FALSE;
#elif ETH_SWITCH_ENABLE == 1
		//TODO: we should check the link value of SJA1105 in DDPX
		linkEstablished = IfxEth_Marvell_Sw_Is_Link(IFXETH_SWITCH_88E6321_1_ADDR);
#else
		#error "Please define Aurix phy since Marvell Switch, and Broad reach are not enable"
#endif
		}
    return linkEstablished;
#endif
}

#if 1 //Disable MDIOinit for now
void IfxEth_mdioinit(void)
{

}
#else
static uint32 empty_init(void)
{
	return 1;
}

static boolean empty_link(void)
{
	return TRUE;
}

/** \brief Asc information */
typedef struct
{
    struct
    {
        IfxEth eth;                      /**< \brief ETH interface */
    }drivers;
} App_Eth;


void IfxEth_mdioinit(void)
{
#ifdef TC397B
	const IfxEth_MiiPins mii_pins = {NULL, //&IfxEth_CRSA_P11_14_IN,                /* CRSDIV NULL */
									 NULL, //&IfxEth_COL_P11_15_IN,                /* COL  null*/
	                                 &IfxGeth_TXCLKA_P11_5_IN,  /* TXCLK */
									 &IfxGeth_RXCLKA_P11_12_IN,         /* RXCLK */
	                                 &IfxGeth_RXDVA_P11_11_IN,   /* RXDV */
									 NULL, //&IfxEth_RXERA_P11_13_IN,                /* RXER null*/
	                                 &IfxGEth_RXD0A_P11_10_IN,     /* RXD0 */
	                                 &IfxGEth_RXD1A_P11_9_IN,     /* RXD1 */
	                                 &IfxGEth_RXD2A_P11_8_IN,     /* RXD2 */
	                                 &IfxGEth_RXD3A_P11_7_IN,     /* RXD3 */
	                                 &IfxGeth_MDC_P12_0_OUT,            /* MDC */
	                                 &IfxGEth_MDIO_P12_1_INOUT,           /* MDIO */
	                                 &IfxGeth_TXEN_P11_6_OUT,           /* TXEN */
									 NULL, //&IfxEth_TXER_P11_4_OUT,                /* TXER null */
	                                 &IfxGEth_TXD0_P11_3_OUT,           /* TXD0 */
	                                 &IfxGEth_TXD1_P11_2_OUT,           /* TXD1 */
	                                 &IfxGEth_TXD2_P11_1_OUT,           /* TXD2 */
	                                 &IfxGEth_TXD3_P11_0_OUT,           /* TXD3 */
	};
	const IfxEth_RmiiPins rmii_pins = {&IfxGEth_CRSDVA_P11_11_IN,     /* CRSDIV */
	                                   &IfxGEth_REFCLKA_P11_12_IN,     /* REFCLK */
	                                   &IfxGEth_RXD0A_P11_10_IN,       /* RXD0 */
	                                   &IfxGEth_RXD1A_P11_9_IN,       /* RXD1 */
	                                   &IfxGeth_MDC_P12_0_OUT,        /* MDC */
	                                   &IfxGEth_MDIO_P12_1_INOUT,       /* MDIO */
	                                   &IfxGEth_TXD0_P11_3_OUT,       /* TXD0 */
	                                   &IfxGEth_TXD1_P11_2_OUT,       /* TXD1 */
	                                   &IfxGEth_TXEN_P11_6_OUT        /* TXEN */
	};
#else //TC397A
	const IfxEth_MiiPins mii_pins = {NULL,                /* CRSDIV */
	                                 NULL,                /* COL */
	                                 &IfxEth_TXCLKA_P11_5_IN,  /* TXCLK */
									 &IfxEth_RXCLKA_P11_12_IN,         /* RXCLK */
	                                 &IfxEth_RXDVA_P11_11_IN,   /* RXDV */
	                                 NULL,                /* RXER */
	                                 &IfxEth_RXD0A_P11_10_IN,     /* RXD0 */
	                                 &IfxEth_RXD1A_P11_9_IN,     /* RXD1 */
	                                 &IfxEth_RXD2A_P11_8_IN,     /* RXD2 */
	                                 &IfxEth_RXD3A_P11_7_IN,     /* RXD3 */
	                                 //&IfxEth_MDC_P12_0_OUT,            /* MDC */
	                                 //&IfxEth_MDIO_P12_1_INOUT,           /* MDIO */
	                                 &IfxEth_TXEN_P11_6_OUT,           /* TXEN */
	                                 NULL,                /* TXER */
	                                 &IfxEth_TXD0_P11_3_OUT,           /* TXD0 */
	                                 &IfxEth_TXD1_P11_2_OUT,           /* TXD1 */
	                                 &IfxEth_TXD2_P11_1_OUT,           /* TXD2 */
	                                 &IfxEth_TXD3_P11_0_OUT,           /* TXD3 */
	};

	const IfxEth_RmiiPins rmii_pins = {&IfxEth_CRSDVA_P11_11_IN,     /* CRSDIV */
	                                   &IfxEth_REFCLKA_P11_12_IN,     /* REFCLK */
	                                   &IfxEth_RXD0A_P11_10_IN,       /* RXD0 */
	                                   &IfxEth_RXD1A_P11_9_IN,       /* RXD1 */
	                                   &IfxEth_MDC_P12_0_OUT,        /* MDC */
	                                   &IfxEth_MDIO_P12_1_INOUT,       /* MDIO */
	                                   &IfxEth_TXD0_P11_3_OUT,       /* TXD0 */
	                                   &IfxEth_TXD1_P11_2_OUT,       /* TXD1 */
	                                   &IfxEth_TXEN_P11_6_OUT        /* TXEN */
	};
#endif

	App_Eth g_Eth; /**< \brief Demo information */

	IfxEth_Config   config;
	IfxEth_initConfig(&config, &MODULE_ETH);

	config.phyInterfaceMode = IfxEth_PhyInterfaceMode_rmii;
	config.rmiiPins = &rmii_pins;
	config.phyInit = &empty_init;
	config.phyLink = &empty_link;

	IfxEth_init(&g_Eth.drivers.eth, &config);

	/* activate loopback */
	IfxEth_setLoopbackMode(&g_Eth.drivers.eth, 1);

	/* and enable transmitter/receiver */
	IfxEth_startTransmitter(&g_Eth.drivers.eth);
	IfxEth_startReceiver(&g_Eth.drivers.eth);
	is_mdio_up = TRUE;
	ETH_ILLD_DRV_ALWAYS_PRINTF(("init mdio module\r\n"));
}
#endif

void IfxEth_mdio_set_simulating(boolean enable)
{
	if(enable)
	{
		ETH_ILLD_DRV_ALWAYS_PRINTF(("enable mdio gpio\r\n"));
		init_MDIO();
	}
	else
	{
		ETH_ILLD_DRV_ALWAYS_PRINTF(("disable mdio gpio\r\n"));
		deinit_MDIO();
	}
	is_mdio_gpio_enable = enable;

}


void IfxEth_read_mdio_clause_22(uint32 slaveaddr, uint32 regaddr, uint32 *pdata)
{

	if(is_mdio_gpio_enable)
		(*pdata) = clause22_read(slaveaddr, regaddr);
	else
	{
		IfxGeth_phy_Clause22_readMDIORegister(slaveaddr, regaddr, pdata);
	}

}


void IfxEth_write_mdio_clause_22(uint32 slaveaddr, uint32 regaddr, uint32 data)
{
	if(is_mdio_gpio_enable)
		clause22_write(slaveaddr, regaddr, data);
	else
	{
		IfxGeth_Phy_Clause22_writeMDIORegister(slaveaddr, regaddr, data);
	}
}


/* use clause 22 to read clause 45 register */
void IfxEth_read_mdio_clause_45(uint32 slaveaddr, uint32 devaddr, uint32 regaddr, uint32 *pdata)
{
	if(is_mdio_gpio_enable)
	{
		(*pdata) = clause45_read(slaveaddr, devaddr, regaddr);
	}
	else
	{
		uint32 regdata = devaddr;
		uint32 value;
		IfxEth_write_mdio_clause_22(slaveaddr, 13, regdata);

		regdata = regaddr;
		IfxEth_write_mdio_clause_22(slaveaddr, 14, regdata);

		regdata = 0x4000 + devaddr;
		IfxEth_write_mdio_clause_22(slaveaddr, 13, regdata);

		IfxEth_read_mdio_clause_22(slaveaddr, 14, &value);

		*pdata = value;
	}
}


void IfxEth_write_mdio_clause_45(uint32 slaveaddr, uint32 devaddr, uint32 regaddr, uint32 data)
{
	if(is_mdio_gpio_enable)
	{
		clause45_write(slaveaddr, devaddr, regaddr, data);
	}
	else
	{
		uint32 regdata = devaddr;

		IfxEth_write_mdio_clause_22(slaveaddr, 13, regdata);

		regdata = regaddr;
		IfxEth_write_mdio_clause_22(slaveaddr, 14, regdata);

		regdata = 0x4000 + devaddr;
		IfxEth_write_mdio_clause_22(slaveaddr, 13, regdata);

		IfxEth_write_mdio_clause_22(slaveaddr, 14, data);
	}
}


static boolean Is_Marvell_Switch(uint32 slaveaddr)
{
	boolean ret = FALSE;
#ifdef IFXETH_SWITCH_88E6321_1_ADDR
	if(slaveaddr == IFXETH_SWITCH_88E6321_1_ADDR)
	{
		ret = TRUE;
	}
#endif
#ifdef IFXETH_SWITCH_88E6321_2_ADDR
	if(slaveaddr == IFXETH_SWITCH_88E6321_2_ADDR)
	{
		ret = TRUE;
	}
#endif
#ifdef IFXETH_SWITCH_88Q5050_ADDR
	if(slaveaddr == IFXETH_SWITCH_88Q5050_ADDR)
	{
		ret = TRUE;
	}
#endif
	return ret;
}


void IfxEth_read_mdio_reg(uint32 slaveaddr, uint32 layeraddr, uint32 regaddr, uint32 *pdata)
{

	// the parameter pass to the mdio function
	uint32 mdio_slave_address = 0;

	/* Step 1: config the MDC control logic */
	IfxEth_Cfg_Aurix_MDC(slaveaddr);

	/* Step 2: if it's Marvell switch, we need to handle singal-chip/multi-chip mode separately */
	if(Is_Marvell_Switch(slaveaddr))
	{
#if SINGLE_CHIP_ADDRESS_MODE_ENABLE == 1
		/* according to Marvell spec, when switch is in single chip mode, its slave address is 0,
		 * and it will response to the layer address directly, so we will pass layer address in MDIO/MDC */
		mdio_slave_address = layeraddr;
#else
		/* when switch is in multi chip mode, it has unique slave address,
		 * and it will response to the slave address only as common device does,
		 * so we will pass slave address in MDIO/MDC */
		mdio_slave_address = slaveaddr;
#endif
	}
	else
	{
		mdio_slave_address = slaveaddr;
	}
	/* step 3: call MDIO driver to send data via MDIO */
	IfxEth_read_mdio_clause_22(mdio_slave_address, regaddr, pdata);

	ETH_ILLD_DRV_DEBUG_PRINTF(("%s, slaveaddr 0x%x, layeraddr 0x%x, regaddr 0x%x, data 0x%x\n",
				__func__, slaveaddr, layeraddr, regaddr, *pdata));
}

void IfxEth_write_mdio_reg(uint32 slaveaddr, uint32 layeraddr, uint32 regaddr, uint32 data)
{
	ETH_ILLD_DRV_DEBUG_PRINTF(("%s, slaveaddr 0x%x, layeraddr 0x%x, regaddr 0x%x, data 0x%x\n",
				__func__, slaveaddr, layeraddr, regaddr, data));

	// the parameter pass to the mdio function
	uint32 mdio_slave_address = 0;

	/* Step 1: config the MDC control logic */
	IfxEth_Cfg_Aurix_MDC(slaveaddr);

	/* Step 2: if it's Marvell switch, we need to handle singal-chip/multi-chip mode separately */
	/* Step 2: if it's Marvell switch, we need to handle singal-chip/multi-chip mode separately */
	if(Is_Marvell_Switch(slaveaddr))
	{
#if SINGLE_CHIP_ADDRESS_MODE_ENABLE == 1
		/* according to Marvell spec, when switch is in single chip mode, its slave address is 0,
		 * and it will response to the layer address directly, so we will pass layer address in MDIO/MDC */
		mdio_slave_address = layeraddr;
#else
		/* when switch is in multi chip mode, it has unique slave address,
		 * and it will response to the slave address only as common device does,
		 * so we will pass slave address in MDIO/MDC */
		mdio_slave_address = slaveaddr;
#endif
	}
	else
	{
		mdio_slave_address = slaveaddr;
	}
	/* step 3: call MDIO driver to send data via MDIO */
	IfxEth_write_mdio_clause_22(mdio_slave_address, regaddr, data);
}

IFX_EXTERN void IfxEth_Cfg_Aurix_MDC_Reset()
{
	prev_slave_address = 0xFFFF;
}

#if PROJECT != PROJECT_V31C
IFX_EXTERN void IfxEth_Cfg_Aurix_MDC_5050()
{
	if(prev_slave_address == IFXETH_SWITCH_88Q5050_ADDR)
	{ /* if it's the same slave address, we don't need to toggle GPIO again */
		return;
	}
	IfxPort_setPinHigh(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex);
	IfxPort_setPinModeOutput(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
#if SINGLE_CHIP_ADDRESS_MODE_ENABLE == 1
	SET_GPIOEXP_OUTPUT_HIGH(ARX_ETH_EA6321_1_MDC_CTRL_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETH_EA6321_1_MDC_CTRL_EXP);

	SET_GPIOEXP_OUTPUT_LOW(ARX_ETH_2112_GRP2_MDC_CTRL_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETH_2112_GRP2_MDC_CTRL_EXP);
#endif
	SET_GPIOEXP_OUTPUT_HIGH(ARX_ETH_Q5050_MDC_CTRL_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETH_Q5050_MDC_CTRL_EXP);

	DelayMs(MDIO_LOGIC_OUTPUT_DELAY_MS);
	prev_slave_address = IFXETH_SWITCH_88Q5050_ADDR;
}


IFX_EXTERN void IfxEth_Cfg_Aurix_MDC_107()
{
	// ARX_ETHSW_PHY_MDIO_CTRL = 0
	IfxPort_setPinLow(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex);
	IfxPort_setPinModeOutput(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	// ARX_ETH_2112_GRP1_MDC_CTRL = 1
	SET_GPIOEXP_OUTPUT_HIGH(ARX_ETH_2112_GRP1_MDC_CTRL_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETH_2112_GRP1_MDC_CTRL_EXP);

	DelayMs(MDIO_LOGIC_OUTPUT_DELAY_MS);
	/* AQR107 address is the same as one PHY 88Q2112
	 * to keep it simple, just reset the previuos slave address after AQR107 is accessed
	 * This might affect AQR107 flashing efficient
	 * However, it's not POR */
	IfxEth_Cfg_Aurix_MDC_Reset();
}
#endif

#if (PROJECT ==  PROJECT_V3NA)
static void IfxEth_Cfg_Aurix_MDC_P3479(uint32 slaveaddr)
{
	switch(slaveaddr)
	{
	case IFXETH_PHY_88Q2112_1_ADDR_P3479:
	case IFXETH_PHY_88Q2112_2_ADDR_P3479:
	case IFXETH_PHY_88Q2112_3_ADDR_P3479:
	case IFXETH_PHY_88Q2112_4_ADDR_P3479:
		//ARX_ETHSW_PHY_MDIO_CTRL = 0
		//ARX_ETH_2112_GRP1_MDC_CTRL = 1
		//ARX_ETH_EA6321_1_MDC_CTRL = N/A
		//ARX_ETH_2112_GRP2_MDC_CTRL = N/A
		IfxPort_setPinLow(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex);
		IfxPort_setPinModeOutput(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

		SET_GPIOEXP_OUTPUT_HIGH(ARX_ETH_2112_GRP1_MDC_CTRL_EXP);
		SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETH_2112_GRP1_MDC_CTRL_EXP);

		break;

	case IFXETH_PHY_88Q2112_5_ADDR_P3479:
	case IFXETH_PHY_88Q2112_6_ADDR_P3479:
	case IFXETH_PHY_88Q2112_7_ADDR_P3479:
	case IFXETH_PHY_88Q2112_8_ADDR_P3479:
		//ARX_ETHSW_PHY_MDIO_CTRL = 1
		//ARX_ETH_2112_GRP1_MDC_CTRL = N/A
		//ARX_ETH_EA6321_1_MDC_CTRL = N/A
		//ARX_ETH_2112_GRP2_MDC_CTRL = 1
		IfxPort_setPinHigh(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex);
		IfxPort_setPinModeOutput(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

		SET_GPIOEXP_OUTPUT_HIGH(ARX_ETH_2112_GRP2_MDC_CTRL_EXP);
		SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETH_2112_GRP2_MDC_CTRL_EXP);

		break;
	case IFXETH_SWITCH_88E6321_1_ADDR:   // 0x10, U456 (P3479-A00)  U33 (P3479-A01)
		//A00: ARX_ETHSW_PHY_MDIO_CTRL = 1    Group 2
		//A01: ARX_ETHSW_PHY_MDIO_CTRL = 0    Group 1
		//ARX_ETH_2112_GRP1_MDC_CTRL = N/A
		//ARX_ETH_EA6321_1_MDC_CTRL = 0
		//ARX_ETH_2112_GRP2_MDC_CTRL = N/A
		if ((board.pcb_spin == 'A') && (board.pcb_version == 0))
		{ // A00
			IfxPort_setPinHigh(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex);
		}
		else
		{ // A01
			IfxPort_setPinLow(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex);
		}
		IfxPort_setPinModeOutput(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

		SET_GPIOEXP_OUTPUT_LOW(ARX_ETH_EA6321_1_MDC_CTRL_EXP);
		SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETH_EA6321_1_MDC_CTRL_EXP);


		break;
	case IFXETH_SWITCH_88E6321_2_ADDR:   // 0x12 U452 (P3479-A00)  U37 (P3479-A01)
		//A00: ARX_ETHSW_PHY_MDIO_CTRL = 0    Group 1
		//A01: ARX_ETHSW_PHY_MDIO_CTRL = 1    Group 2
		//ARX_ETH_2112_GRP1_MDC_CTRL = N/A
		//ARX_ETH_EA6321_1_MDC_CTRL = 0
		//ARX_ETH_2112_GRP2_MDC_CTRL = N/A
		if ((board.pcb_spin == 'A') && (board.pcb_version == 0))
		{ // A00
			IfxPort_setPinLow(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex);
		}
		else
		{ // A01
			IfxPort_setPinHigh(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex);
		}
		IfxPort_setPinModeOutput(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

		SET_GPIOEXP_OUTPUT_LOW(ARX_ETH_EA6321_1_MDC_CTRL_EXP);
		SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETH_EA6321_1_MDC_CTRL_EXP);
		break;
	default:
		ETH_ILLD_DRV_ALWAYS_PRINTF(("%s: un-expected slave address is set\r\n", __func__));
		break;

	}
#if ETH_ILLD_DRV_DEBUG_ENABLE == 1
	ETH_ILLD_DRV_DEBUG_PRINTF(("ARX_ETHSW_PHY_MDIO_CTRL = %d\r\n", IfxPort_getPinState(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex)));
	ETH_ILLD_DRV_DEBUG_PRINTF(("ARX_ETH_2112_GRP1_MDC_CTRL = %d\r\n", READ_GPIOEXP_STATE(ARX_ETH_2112_GRP1_MDC_CTRL_EXP)));
	ETH_ILLD_DRV_DEBUG_PRINTF(("ARX_ETH_EA6321_1_MDC_CTRL = %d\r\n", READ_GPIOEXP_STATE(ARX_ETH_EA6321_1_MDC_CTRL_EXP)));
	ETH_ILLD_DRV_DEBUG_PRINTF(("ARX_ETH_2112_GRP2_MDC_CTRL = %d\r\n", READ_GPIOEXP_STATE(ARX_ETH_2112_GRP2_MDC_CTRL_EXP)));
#endif
}
#endif /*(PROJECT ==  PROJECT_V3NA)*/

#if (PROJECT ==  PROJECT_PAGASUS2)
/* E3550-B0x, Pegasus */
static void IfxEth_Cfg_Aurix_MDC_E3550(uint32 slaveaddr)
{
	switch(slaveaddr & 0xfff)
	{
	case IFXETH_PHY_88Q2112_1_ADDR_E3550:
	case IFXETH_PHY_88Q2112_2_ADDR_E3550:
	case IFXETH_PHY_88Q2112_3_ADDR_E3550:
	case IFXETH_PHY_88Q2112_4_ADDR_E3550:
	case IFXETH_PHY_88Q2112_7_ADDR_E3550:
		//ARX_ETHSW_PHY_MDIO_CTRL = 0
		//ARX_ETH_2112_GRP1_MDC_CTRL = 1
		//ARX_ETH_EA6321_1_MDC_CTRL = N/A
		//ARX_ETH_2112_GRP2_MDC_CTRL = N/A
		//ARX_ETH_Q5050_MDC_CTRL = N/A
		IfxPort_setPinLow(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex);
		IfxPort_setPinModeOutput(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

		SET_GPIOEXP_OUTPUT_HIGH(ARX_ETH_2112_GRP1_MDC_CTRL_EXP);
		SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETH_2112_GRP1_MDC_CTRL_EXP);

		break;

	case IFXETH_PHY_88Q2112_5_ADDR_E3550:
	case IFXETH_PHY_88Q2112_6_ADDR_E3550:
		//ARX_ETHSW_PHY_MDIO_CTRL = 1
		//ARX_ETH_2112_GRP1_MDC_CTRL = N/A
		//ARX_ETH_EA6321_1_MDC_CTRL = 1 (Multi_chip: N/A)
		//ARX_ETH_2112_GRP2_MDC_CTRL = 1
		//ARX_ETH_Q5050_MDC_CTRL = 0 (Multi_chip: N/A)
		IfxPort_setPinHigh(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex);
		IfxPort_setPinModeOutput(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

#if SINGLE_CHIP_ADDRESS_MODE_ENABLE == 1
		SET_GPIOEXP_OUTPUT_HIGH(ARX_ETH_EA6321_1_MDC_CTRL_EXP);
		SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETH_EA6321_1_MDC_CTRL_EXP);
#endif
		SET_GPIOEXP_OUTPUT_HIGH(ARX_ETH_2112_GRP2_MDC_CTRL_EXP);
		SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETH_2112_GRP2_MDC_CTRL_EXP);
#if SINGLE_CHIP_ADDRESS_MODE_ENABLE == 1
		SET_GPIOEXP_OUTPUT_LOW(ARX_ETH_Q5050_MDC_CTRL_EXP);
		SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETH_Q5050_MDC_CTRL_EXP);
#endif
		break;
	case IFXETH_SWITCH_88E6321_2_ADDR:   // 0x12 I67, U169
		//ARX_ETHSW_PHY_MDIO_CTRL = 0
		//ARX_ETH_2112_GRP1_MDC_CTRL = 0
		//ARX_ETH_EA6321_1_MDC_CTRL = N/A
		//ARX_ETH_2112_GRP2_MDC_CTRL = N/A
		//ARX_ETH_Q5050_MDC_CTRL = N/A
		IfxPort_setPinLow(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex);
		IfxPort_setPinModeOutput(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

		if ((board.pcb_spin == 'B') && (board.pcb_version == 0))
		{ /* B00 */
			SET_GPIOEXP_OUTPUT_HIGH(ARX_ETH_2112_GRP1_MDC_CTRL_EXP);
		}
		else
		{ // B01 or later
			SET_GPIOEXP_OUTPUT_LOW(ARX_ETH_2112_GRP1_MDC_CTRL_EXP);
		}

		SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETH_2112_GRP1_MDC_CTRL_EXP);
		break;
	case IFXETH_SWITCH_88E6321_1_ADDR:   // 0x10 I84, U173
		//ARX_ETHSW_PHY_MDIO_CTRL = 1
		//ARX_ETH_2112_GRP1_MDC_CTRL = N/A
		//ARX_ETH_EA6321_1_MDC_CTRL = 0
		//ARX_ETH_2112_GRP2_MDC_CTRL = 0 (Multi_chip: N/A)
		//ARX_ETH_Q5050_MDC_CTRL = 0 (Multi_chip: N/A)
		IfxPort_setPinHigh(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex);
		IfxPort_setPinModeOutput(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

		SET_GPIOEXP_OUTPUT_LOW(ARX_ETH_EA6321_1_MDC_CTRL_EXP);
		SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETH_EA6321_1_MDC_CTRL_EXP);
#if SINGLE_CHIP_ADDRESS_MODE_ENABLE == 1
		SET_GPIOEXP_OUTPUT_LOW(ARX_ETH_2112_GRP2_MDC_CTRL_EXP);
		SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETH_2112_GRP2_MDC_CTRL_EXP);

		SET_GPIOEXP_OUTPUT_LOW(ARX_ETH_Q5050_MDC_CTRL_EXP);
		SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETH_Q5050_MDC_CTRL_EXP);
#endif
		break;
	case IFXETH_PHY_88Q2112_0_ADDR_E3550:
	case IFXETH_SWITCH_88Q5050_ADDR:
		IfxEth_Cfg_Aurix_MDC_5050();
		break;
	default:
		ETH_ILLD_DRV_ALWAYS_PRINTF(("%s: un-expected slave address is set\r\n", __func__));
		break;

	}
#if ETH_ILLD_DRV_DEBUG_ENABLE == 1
	ETH_ILLD_DRV_DEBUG_PRINTF(("ARX_ETHSW_PHY_MDIO_CTRL = %d\r\n", IfxPort_getPinState(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex)));
	ETH_ILLD_DRV_DEBUG_PRINTF(("ARX_ETH_2112_GRP1_MDC_CTRL = %d\r\n", READ_GPIOEXP_STATE(ARX_ETH_2112_GRP1_MDC_CTRL_EXP)));
	ETH_ILLD_DRV_DEBUG_PRINTF(("ARX_ETH_EA6321_1_MDC_CTRL = %d\r\n", READ_GPIOEXP_STATE(ARX_ETH_EA6321_1_MDC_CTRL_EXP)));
	ETH_ILLD_DRV_DEBUG_PRINTF(("ARX_ETH_2112_GRP2_MDC_CTRL = %d\r\n", READ_GPIOEXP_STATE(ARX_ETH_2112_GRP2_MDC_CTRL_EXP)));
	ETH_ILLD_DRV_DEBUG_PRINTF(("ARX_ETH_Q5050_MDC_CTRL = %d\r\n", READ_GPIOEXP_STATE(ARX_ETH_Q5050_MDC_CTRL_EXP)));
#endif
}
#endif /*(PROJECT ==  PROJECT_PAGASUS2)*/

// General function pointing to different funcs based on project
IFX_EXTERN void IfxEth_Cfg_Aurix_MDC(uint32 slaveaddr)
{
	if(prev_slave_address == slaveaddr)
	{ /* if it's the same slave address, we don't need to toggle GPIO again */
		return;
	}

#if PROJECT == PROJECT_V3NA
	// For V3NA we skip the function
	//IfxEth_Cfg_Aurix_MDC_P3479(slaveaddr);
#elif PROJECT == PROJECT_V31C
	//do nothing
#elif PROJECT == PROJECT_PAGASUS2
	IfxEth_Cfg_Aurix_MDC_E3550(slaveaddr);
#else
	ETH_ILLD_DRV_ALWAYS_PRINTF(("%s: un-expected project is defined\r\n", __func__));
#endif

	DelayMs(MDIO_LOGIC_OUTPUT_DELAY_MS);
	prev_slave_address = slaveaddr;
}

#endif /*PROJECT == PROJECT_PAGASUS2 || PROJECT == PROJECT_V31C || PROJECT == PROJECT_V3NA*/

