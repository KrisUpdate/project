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

#include "Gpio.h"
#include "Configuration.h"
#include "Port/Std/IfxPort.h"
#include "SysSe/Bsp/Bsp.h"
#include <internal_fw_debug.h>
//#include "resource_state_manage.h"
//#include "../Vadc/VadcCmds.h"
#include "GpioCmds.h"
#include "ethernet.h"
#include <Mcmcan.h>
//#include "Vadc/EvadcAutoScan.h"
//#include "I2c/I2cRom.h"
//#include "I2c/Vmon.h"
#include "board.h"
#include <stdarg.h>
//#include "Tsensor.h"
#include "Gpio_ctrl_3570.h"
#include "TegraQspi.h"
//#include "CIM/CIM.h"
#include "board2board.h"
#include "system_monitor.h"
#include "resource.h"
#if ISR_PROVIDER_ASC_SHELL == 0
// include shell if assign to CPU0
#include "AsclinShellInterface.h"
#endif

#if PROJECT == PROJECT_PAGASUS2
//#if (DRIVE_AX_A00 == 1) || (DRIVE_AX_A01 == 1)
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define GPIO_DEBUG_ENABLE 1

#if GPIO_DEBUG_ENABLE == 1
#define GPIO_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define GPIO_DEBUG_PRINTF(x)
#endif

#define GPIO_ALWAYS_PRINTF(x) DEBUG_PRINTF_ALWAYS(x)
#define VDD_3V3_THRESHOLD    ((float)2.31)
#define VDD_CPU_THRESHOLD    ((float)0.58)
#define DGPU_THERM_ALERT_THRESHOLD ((float)0.7)
//Due to sometimes check PG timeout error, we change it from 2000 to 8000 
#define TIMEOUT_CNTR_PG_1MS      (8000)

#define VBAT_SXMA_CHANNEL           22
#define VBAT_SXMB_CHANNEL           23
#define DGPU_THERM_ALERT_CHANNEL    47

/* Macro used by 3570 */
#define TEGRA_OWN_DGPU_MODE   0 //no use
#define X86_OWN_DGPU_MODE     3

#define TEGRA_POWER_RETRY 3
typedef enum
{
	poweron_state,
	poweroff_state,
	max_state
}e_system_state;

static e_system_state system_state = poweroff_state;

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
volatile boolean brd_alert = TRUE;
volatile boolean brd_alert_2 = TRUE;
volatile boolean brd_alert_3 = TRUE;
volatile boolean brd_alert_4 = TRUE;
volatile boolean brd_alert_5 = TRUE;
volatile boolean tegra_a_alert = TRUE;
volatile boolean tegra_b_alert = TRUE;

volatile boolean dgpu_a_alert = TRUE;  // DGPU1
volatile boolean dgpu_b_alert = TRUE;  // DGPU2


volatile boolean brd_therm = TRUE;
volatile boolean tegra_a_therm = TRUE;
volatile boolean tegra_b_therm = TRUE;
#if 0
//For E3550
//volatile boolean tegra_c_therm = TRUE; // P1
/* P3479-A04 */
//volatile boolean ovr_cpu_thwn = TRUE; // FDMF5833 CPU OVR
//volatile boolean ovr_gpu_thwn = TRUE; // FDMF5833 GPU OVR
//volatile boolean ovr_soc_thwn = TRUE; // FDMF5833 SOC OVR
//volatile boolean ovr_cv_thwn  = TRUE; // FDMF5833 CV OVR
#endif
/* P3570 */
volatile boolean xa_ovr_thwn = TRUE;
volatile boolean xb_ovr_thwn  = TRUE;

volatile boolean pmic_a_on = FALSE;
volatile boolean pmic_b_on = FALSE;
volatile boolean pmic_c_on = FALSE;    // P1
volatile boolean dgpu_a_on = FALSE;	   // DGPU1
volatile boolean dgpu_b_on = FALSE;    // DGPU2
volatile boolean whole_board_off = TRUE;

/*monitor the VDD3V3 apply to tegras*/
volatile boolean vdd3v3_off = FALSE;

/* rework level or BOM sku */
char pcb_variant = 'A';
/* board pcb version x00, x01*/
uint8 pcb_version = 0;
/* board pcb version A0x, B0x*/
char pcb_spin = 'A';

extern boolean is_b2b_x86_v3nc;

static boolean g_poweroff_flag = FALSE;
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
static boolean Check_PGoodSignals(uint8 count, ...)
{
	uint8 i;
	uint32 time_out_count = 0;
	IoExpPort_Pin* pgood_signal_ptr;
    va_list ap;
    va_start(ap, count);
    for(i=0; i<count; i++)
    {
    	pgood_signal_ptr = va_arg(ap, IoExpPort_Pin*);
    	while (!(READ_GPIOEXP_STATE((*pgood_signal_ptr))))
		{
    		waitTime(1*TimeConst[TIMER_INDEX_1MS]);	// this is just a placeholder value for delay
    		time_out_count ++;
			if (time_out_count >= TIMEOUT_CNTR_PG_1MS)
			{
				return FALSE;
			}
		}
    	time_out_count = 0;
    }
    return TRUE;
}

#if 0
static void SetOutputs_to_System_PowerOff_State_3550_3479(void)
{
	// Set AO_CAN ports
    GPIO_ALWAYS_PRINTF(("SetOutputs_to_System_PowerOff_State_3550_3479.\r\n")); 
    return;
#if CAN_NODE_1_ENABLE == 1
	IfxPort_setPinHigh(CAN_PHY2_TXD.pin.port, CAN_PHY2_TXD.pin.pinIndex);
	IfxPort_setPinModeOutput(CAN_PHY2_TXD.pin.port, CAN_PHY2_TXD.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
#endif

#if CAN_NODE_3_ENABLE == 1
	IfxPort_setPinHigh(CAN_PHY1_TXD.pin.port, CAN_PHY1_TXD.pin.pinIndex);
	IfxPort_setPinModeOutput(CAN_PHY1_TXD.pin.port, CAN_PHY1_TXD.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
	SET_GPIOEXP_OUTPUT_LOW(CAN_PHY1_EN_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(CAN_PHY1_EN_EXP);
	can_set_phy_standby_state(PHY1);
#endif

#if CAN_NODE_2_ENABLE == 1
	IfxPort_setPinHigh(CAN_PHY3_TXD.pin.port, CAN_PHY3_TXD.pin.pinIndex);
	IfxPort_setPinModeOutput(CAN_PHY3_TXD.pin.port, CAN_PHY3_TXD.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
	SET_GPIOEXP_OUTPUT_LOW(CAN_PHY3_EN_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(CAN_PHY3_EN_EXP);
	can_set_phy_standby_state(PHY3);
#endif

#if CAN_NODE_0_ENABLE == 1
	IfxPort_setPinHigh(CAN0_TXD.pin.port, CAN0_TXD.pin.pinIndex);
	IfxPort_setPinModeOutput(CAN0_TXD.pin.port, CAN0_TXD.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
#endif

#if CAN_NODE_4_ENABLE == 1
	IfxPort_setPinHigh(CANR0_TXD.pin.port, CANR0_TXD.pin.pinIndex);
	IfxPort_setPinModeOutput(CANR0_TXD.pin.port, CANR0_TXD.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
#endif

#ifdef CAN_PHY0_STB_EXP
	can_set_phy_standby_state(PHY0);
#endif

#if CAN_NODE_5_ENABLE == 1
	IfxPort_setPinHigh(CANR1_TXD.pin.port, CANR1_TXD.pin.pinIndex);
	IfxPort_setPinModeOutput(CANR1_TXD.pin.port, CANR1_TXD.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
#endif

	// Set CAN ports
	can_set_phy_standby_state(X1_PHY);
	can_set_phy_standby_state(X2_PHY);
	can_set_phy_standby_state(P_PHY);

	// Set Ethernet ports
	IfxPort_setPinLow(PHY_TXD3.pin.port, PHY_TXD3.pin.pinIndex);
	IfxPort_setPinModeOutput(PHY_TXD3.pin.port, PHY_TXD3.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(PHY_TXD2.pin.port, PHY_TXD2.pin.pinIndex);
	IfxPort_setPinModeOutput(PHY_TXD2.pin.port, PHY_TXD2.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(PHY_TXD1.pin.port, PHY_TXD1.pin.pinIndex);
	IfxPort_setPinModeOutput(PHY_TXD1.pin.port, PHY_TXD1.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(PHY_TXD0.pin.port, PHY_TXD0.pin.pinIndex);
	IfxPort_setPinModeOutput(PHY_TXD0.pin.port, PHY_TXD0.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(PHY_TXEN.pin.port, PHY_TXEN.pin.pinIndex);
	IfxPort_setPinModeOutput(PHY_TXEN.pin.port, PHY_TXEN.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	IfxPort_setPinModeInput(PHY_RGMII_RXD0.pin.port, PHY_RGMII_RXD0.pin.pinIndex,IfxPort_Mode_inputPullDown);
	IfxPort_setPinModeInput(PHY_RGMII_RX_CTL.pin.port, PHY_RGMII_RX_CTL.pin.pinIndex,IfxPort_Mode_inputPullDown);
	IfxPort_setPinModeInput(PHY_REFCLK.pin.port, PHY_REFCLK.pin.pinIndex,IfxPort_Mode_inputPullDown);
	IfxPort_setPinModeInput(PHY_RGMII_TX_CLKR.pin.port, PHY_RGMII_TX_CLKR.pin.pinIndex,IfxPort_Mode_inputPullDown);
	IfxPort_setPinModeInput(PHY_RGMII_RXD3.pin.port, PHY_RGMII_RXD3.pin.pinIndex,IfxPort_Mode_inputPullDown);
	IfxPort_setPinModeInput(PHY_RGMII_RXD2.pin.port, PHY_RGMII_RXD2.pin.pinIndex,IfxPort_Mode_inputPullDown);
	IfxPort_setPinModeInput(PHY_RGMII_RXD1.pin.port, PHY_RGMII_RXD1.pin.pinIndex,IfxPort_Mode_inputPullDown);

	// Set Fan ports
#if FAN_A_ENABLE == 1
	IfxPort_setPinLow(AURIX_X1_FANCTL.port, AURIX_X1_FANCTL.pinIndex);
	IfxPort_setPinModeOutput(AURIX_X1_FANCTL.port, AURIX_X1_FANCTL.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
#endif
#if FAN_B_ENABLE == 1
	IfxPort_setPinLow(AURIX_X2_FANCTL.port, AURIX_X2_FANCTL.pinIndex);
	IfxPort_setPinModeOutput(AURIX_X2_FANCTL.port, AURIX_X2_FANCTL.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
#endif
#if PULL_FAN_A_ENABLE == 1 || PULL_FAN_B_ENABLE == 1
	SET_GPIOEXP_OUTPUT_LOW(PULL_FAN_TACH_SEL_AB_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(PULL_FAN_TACH_SEL_AB_EXP);

	IfxPort_setPinLow(E3550_ARX_X1_FAN2_PWM_OVERRIDE.port, E3550_ARX_X1_FAN2_PWM_OVERRIDE.pinIndex);
	IfxPort_setPinModeOutput(E3550_ARX_X1_FAN2_PWM_OVERRIDE.port, E3550_ARX_X1_FAN2_PWM_OVERRIDE.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(E3550_ARX_X2_FAN2_PWM_OVERRIDE.port, E3550_ARX_X2_FAN2_PWM_OVERRIDE.pinIndex);
	IfxPort_setPinModeOutput(E3550_ARX_X2_FAN2_PWM_OVERRIDE.port, E3550_ARX_X2_FAN2_PWM_OVERRIDE.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
#endif

	// Set AO_FlexRay ports
	IfxPort_setPinLow(FR_A_TXD.pin.port, FR_A_TXD.pin.pinIndex);
	IfxPort_setPinModeOutput(FR_A_TXD.pin.port, FR_A_TXD.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(FR_B_TXD.pin.port, FR_B_TXD.pin.pinIndex);
	IfxPort_setPinModeOutput(FR_B_TXD.pin.port, FR_B_TXD.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(FR_A_EN.port, FR_A_EN.pinIndex);
	IfxPort_setPinModeOutput(FR_A_EN.port, FR_A_EN.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(FR_B_EN.port, FR_B_EN.pinIndex);
	IfxPort_setPinModeOutput(FR_B_EN.port, FR_B_EN.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	// we need to keep FLAY TXEN High all the time, bug 1835760
	IfxPort_setPinHigh(FR_A_TXEN.pin.port, FR_A_TXEN.pin.pinIndex);
	IfxPort_setPinModeOutput(FR_A_TXEN.pin.port, FR_A_TXEN.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	// we need to keep FLAY TXEN High all the time, bug 1835760
	IfxPort_setPinHigh(FR_B_TXEN.pin.port, FR_B_TXEN.pin.pinIndex);
	IfxPort_setPinModeOutput(FR_B_TXEN.pin.port, FR_B_TXEN.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	SET_GPIOEXP_OUTPUT_LOW(FR_A_STB_N_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(FR_A_STB_N_EXP);

	SET_GPIOEXP_OUTPUT_LOW(FR_B_STB_N_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(FR_B_STB_N_EXP);

	IfxPort_setPinLow(FLEXRAY_ENABLE_N.port, FLEXRAY_ENABLE_N.pinIndex);
	IfxPort_setPinModeOutput(FLEXRAY_ENABLE_N.port, FLEXRAY_ENABLE_N.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

#if I2C_GPIO_POWER_SAVING_ENABLE == 1
	// Set I2c ports
	IfxPort_setPinLow(AURIX_I2C_SCL.pin.port, AURIX_I2C_SCL.pin.pinIndex);
	IfxPort_setPinModeOutput(AURIX_I2C_SCL.pin.port, AURIX_I2C_SCL.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(AURIX_I2C1_SCL.pin.port, AURIX_I2C1_SCL.pin.pinIndex);
	IfxPort_setPinModeOutput(AURIX_I2C1_SCL.pin.port, AURIX_I2C1_SCL.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	/* I2C_SDA is bidirectional, we set to open drain output */
	IfxPort_setPinLow(AURIX_I2C_SDA.pin.port, AURIX_I2C_SDA.pin.pinIndex);
	IfxPort_setPinModeOutput(AURIX_I2C_SDA.pin.port, AURIX_I2C_SDA.pin.pinIndex, IfxPort_OutputMode_openDrain, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(AURIX_I2C1_SDA.pin.port, AURIX_I2C1_SDA.pin.pinIndex);
	IfxPort_setPinModeOutput(AURIX_I2C1_SDA.pin.port, AURIX_I2C1_SDA.pin.pinIndex, IfxPort_OutputMode_openDrain, IfxPort_OutputIdx_general);
#endif
	// Interrupt pins
	IfxPort_setPinModeInput(XA_VMON_INT_N.port, XA_VMON_INT_N.pinIndex,IfxPort_Mode_inputPullDown);
	IfxPort_setPinModeInput(XB_VMON_INT_N.port, XB_VMON_INT_N.pinIndex,IfxPort_Mode_inputPullDown);
	IfxPort_setPinModeInput(E3550_AURIX_CIM_INT_N.port, E3550_AURIX_CIM_INT_N.pinIndex,IfxPort_Mode_inputPullDown);
	IfxPort_setPinLow(E3550_CIM_PWR_INT_N.port, E3550_CIM_PWR_INT_N.pinIndex);
	IfxPort_setPinModeOutput(E3550_CIM_PWR_INT_N.port, E3550_CIM_PWR_INT_N.pinIndex, IfxPort_OutputMode_openDrain, IfxPort_OutputIdx_general);

	// Set MDIO ports
	IfxPort_setPinLow(PHY_MDC.pin.port, PHY_MDC.pin.pinIndex);
	IfxPort_setPinModeOutput(PHY_MDC.pin.port, PHY_MDC.pin.pinIndex, IfxPort_OutputMode_openDrain, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(PHY_MDIO.pin.port, PHY_MDIO.pin.pinIndex);
	IfxPort_setPinModeOutput(PHY_MDIO.pin.port, PHY_MDIO.pin.pinIndex, IfxPort_OutputMode_openDrain, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex);
	IfxPort_setPinModeOutput(ARX_ETHSW_PHY_MDIO_CTRL.port, ARX_ETHSW_PHY_MDIO_CTRL.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	SET_GPIOEXP_OUTPUT_LOW(ARX_ETH_2112_GRP1_MDC_CTRL_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETH_2112_GRP1_MDC_CTRL_EXP);

	SET_GPIOEXP_OUTPUT_LOW(ARX_ETH_2112_GRP2_MDC_CTRL_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETH_2112_GRP2_MDC_CTRL_EXP);

	SET_GPIOEXP_OUTPUT_LOW(ARX_ETH_Q5050_MDC_CTRL_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETH_Q5050_MDC_CTRL_EXP);

	SET_GPIOEXP_OUTPUT_LOW(ARX_ETH_EA6321_1_MDC_CTRL_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETH_EA6321_1_MDC_CTRL_EXP);

	// Set AO_Misc ports
	// LED1, LED2
	IfxPort_setPinHigh(E3550_AURIX_LED1.port, E3550_AURIX_LED1.pinIndex);
	IfxPort_setPinModeOutput(E3550_AURIX_LED1.port, E3550_AURIX_LED1.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
	IfxPort_setPinHigh(AURIX_LED2.port, AURIX_LED2.pinIndex);
	IfxPort_setPinModeOutput(AURIX_LED2.port, AURIX_LED2.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
	// Harnes GPIO
	IfxPort_setPinLow(AHRN_GPO0.port, AHRN_GPO0.pinIndex);
	IfxPort_setPinModeOutput(AHRN_GPO0.port, AHRN_GPO0.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
	// ADC voltage sensing mux
	IfxPort_setPinLow(ADC_MUX_CTRL_1.port, ADC_MUX_CTRL_1.pinIndex);
	IfxPort_setPinModeOutput(ADC_MUX_CTRL_1.port, ADC_MUX_CTRL_1.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
	IfxPort_setPinLow(ADC_MUX_CTRL_2.port, ADC_MUX_CTRL_2.pinIndex);
	IfxPort_setPinModeOutput(ADC_MUX_CTRL_2.port, ADC_MUX_CTRL_2.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	// Set Misc ports
	SET_GPIOEXP_OUTPUT_LOW(E3550_DBG_FTDI_EN_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(E3550_DBG_FTDI_EN_EXP);

	SET_GPIOEXP_OUTPUT_LOW(E3550_ARX_TRUST_PLATFORM_CTRL_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(E3550_ARX_TRUST_PLATFORM_CTRL_EXP);

	// Set PEX control ports
	IfxPort_setPinLow(E3550_ARX_DGPU1_MUX_CTRL.port, E3550_ARX_DGPU1_MUX_CTRL.pinIndex);
	IfxPort_setPinModeOutput(E3550_ARX_DGPU1_MUX_CTRL.port, E3550_ARX_DGPU1_MUX_CTRL.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(E3550_ARX_DGPU2_MUX_CTRL.port, E3550_ARX_DGPU2_MUX_CTRL.pinIndex);
	IfxPort_setPinModeOutput(E3550_ARX_DGPU2_MUX_CTRL.port, E3550_ARX_DGPU2_MUX_CTRL.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	SET_GPIOEXP_OUTPUT_LOW(E3550_ARX_PEXSW_UART_CTRL_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(E3550_ARX_PEXSW_UART_CTRL_EXP);

	// Set Power ports
	IfxPort_setPinLow(VBAT_SYS_PWR_ON.port, VBAT_SYS_PWR_ON.pinIndex);
	IfxPort_setPinModeOutput(VBAT_SYS_PWR_ON.port, VBAT_SYS_PWR_ON.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(E3550_AURIX_P1_WAKE.port, E3550_AURIX_P1_WAKE.pinIndex);
	IfxPort_setPinModeOutput(E3550_AURIX_P1_WAKE.port, E3550_AURIX_P1_WAKE.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(AURIX_X1_WAKE.port, AURIX_X1_WAKE.pinIndex);
	IfxPort_setPinModeOutput(AURIX_X1_WAKE.port, AURIX_X1_WAKE.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(AURIX_X2_WAKE.port, AURIX_X2_WAKE.pinIndex);
	IfxPort_setPinModeOutput(AURIX_X2_WAKE.port, AURIX_X2_WAKE.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(VBAT_TEG_PWR_ON.port, VBAT_TEG_PWR_ON.pinIndex);
	IfxPort_setPinModeOutput(VBAT_TEG_PWR_ON.port, VBAT_TEG_PWR_ON.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(E3550_AURIX_P1_PWR_ON_R.port, E3550_AURIX_P1_PWR_ON_R.pinIndex);
	IfxPort_setPinModeOutput(E3550_AURIX_P1_PWR_ON_R.port, E3550_AURIX_P1_PWR_ON_R.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(X1_PMIC_POWER_ON_R.port, X1_PMIC_POWER_ON_R.pinIndex);
	IfxPort_setPinModeOutput(X1_PMIC_POWER_ON_R.port, X1_PMIC_POWER_ON_R.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(X2_PMIC_POWER_ON_R.port, X2_PMIC_POWER_ON_R.pinIndex);
	IfxPort_setPinModeOutput(X2_PMIC_POWER_ON_R.port, X2_PMIC_POWER_ON_R.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	SET_GPIOEXP_OUTPUT_LOW(E3550_CIM_PWR_ON_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(E3550_CIM_PWR_ON_EXP);

	SET_GPIOEXP_OUTPUT_LOW(E3550_CE_PREREG_EN_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(E3550_CE_PREREG_EN_EXP);

	SET_GPIOEXP_OUTPUT_LOW(ARX_SXMA_VBAT_EN_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(ARX_SXMA_VBAT_EN_EXP);

	SET_GPIOEXP_OUTPUT_LOW(ARX_SXMB_VBAT_EN_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(ARX_SXMB_VBAT_EN_EXP);

	SET_GPIOEXP_OUTPUT_LOW(E3550_COMEXP_PWR_EN_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(E3550_COMEXP_PWR_EN_EXP);

	// Set recovery ports
	IfxPort_setPinLow(X1_FORCE_RCVR_R_N.port, X1_FORCE_RCVR_R_N.pinIndex);
	IfxPort_setPinModeOutput(X1_FORCE_RCVR_R_N.port, X1_FORCE_RCVR_R_N.pinIndex, IfxPort_OutputMode_openDrain, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(X2_FORCE_RCVR_R_N.port, X2_FORCE_RCVR_R_N.pinIndex);
	IfxPort_setPinModeOutput(X2_FORCE_RCVR_R_N.port, X2_FORCE_RCVR_R_N.pinIndex, IfxPort_OutputMode_openDrain, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(E3550_P1_FORCE_RCVR_N.port, E3550_P1_FORCE_RCVR_N.pinIndex);
	IfxPort_setPinModeOutput(E3550_P1_FORCE_RCVR_N.port, E3550_P1_FORCE_RCVR_N.pinIndex, IfxPort_OutputMode_openDrain, IfxPort_OutputIdx_general);

	SET_GPIOEXP_OUTPUT_LOW(ARX_PEXSW1_BOOT_REC_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(ARX_PEXSW1_BOOT_REC_EXP);

	SET_GPIOEXP_OUTPUT_LOW(ARX_PEXSW2_BOOT_REC_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(ARX_PEXSW2_BOOT_REC_EXP);

	// Set reset ports
	IfxPort_setPinLow(SJA1105_RSTN.port, SJA1105_RSTN.pinIndex);
	IfxPort_setPinModeOutput(SJA1105_RSTN.port, SJA1105_RSTN.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(AURIX_XA_RST_N.port, AURIX_XA_RST_N.pinIndex);
	IfxPort_setPinModeOutput(AURIX_XA_RST_N.port, AURIX_XA_RST_N.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(E3550_ESATA_PEX_RST_N.port, E3550_ESATA_PEX_RST_N.pinIndex);
	IfxPort_setPinModeOutput(E3550_ESATA_PEX_RST_N.port, E3550_ESATA_PEX_RST_N.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(E3550_AURIX_P1_RST_N.port, E3550_AURIX_P1_RST_N.pinIndex);
	IfxPort_setPinModeOutput(E3550_AURIX_P1_RST_N.port, E3550_AURIX_P1_RST_N.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(E3550_AQC107_1_PEX_RESET_N.port, E3550_AQC107_1_PEX_RESET_N.pinIndex);
	IfxPort_setPinModeOutput(E3550_AQC107_1_PEX_RESET_N.port, E3550_AQC107_1_PEX_RESET_N.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(E3550_AQC107_2_PEX_RESET_N.port, E3550_AQC107_2_PEX_RESET_N.pinIndex);
	IfxPort_setPinModeOutput(E3550_AQC107_2_PEX_RESET_N.port, E3550_AQC107_2_PEX_RESET_N.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	IfxPort_setPinLow(AURIX_XB_RST_N.port, AURIX_XB_RST_N.pinIndex);
	IfxPort_setPinModeOutput(AURIX_XB_RST_N.port, AURIX_XB_RST_N.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	SET_GPIOEXP_OUTPUT_LOW(CIM_RST_N_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(CIM_RST_N_EXP);

	SET_GPIOEXP_OUTPUT_LOW(E3550_PEXSW2_RESET_OUTPUT_N_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(E3550_PEXSW2_RESET_OUTPUT_N_EXP);

	SET_GPIOEXP_OUTPUT_LOW(ARX_ETHSW_PHY_GRP1_RESET_N_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETHSW_PHY_GRP1_RESET_N_EXP);

	SET_GPIOEXP_OUTPUT_LOW(ARX_ETHSW_PHY_GRP2_RESET_N_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(ARX_ETHSW_PHY_GRP2_RESET_N_EXP);

	SET_GPIOEXP_OUTPUT_LOW(E3550_COMEXP_RESET_CTRL_N_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(E3550_COMEXP_RESET_CTRL_N_EXP);

	SET_GPIOEXP_OUTPUT_LOW(E3550_ARX_PM8534_RESET_N_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(E3550_ARX_PM8534_RESET_N_EXP);

	SET_GPIOEXP_OUTPUT_LOW(E3550_ARX_AQR107_RST_N_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(E3550_ARX_AQR107_RST_N_EXP);

	SET_GPIOEXP_OUTPUT_LOW(E3550_M2_RESET_N_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(E3550_M2_RESET_N_EXP);

	if(board.project == P3479 && board.pcb_spin == 'A'
			&& board.pcb_version >= 3)
	{
		SET_GPIOEXP_OUTPUT_LOW(P3479_LAN7431_RESET_N_EXP);
		SET_GPIOEXP_CONFIG_OUTPUT(P3479_LAN7431_RESET_N_EXP);
	}

	// Set SPI ports
	// SJA1105
	IfxPort_setPinLow(SJA1105_CSN.pin.port, SJA1105_CSN.pin.pinIndex);
	IfxPort_setPinModeOutput(SJA1105_CSN.pin.port, SJA1105_CSN.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
	IfxPort_setPinModeInput(SJA1105_SDO.pin.port, SJA1105_SDO.pin.pinIndex,IfxPort_Mode_inputPullDown);
	IfxPort_setPinLow(SJA1105_SCK.pin.port, SJA1105_SCK.pin.pinIndex);
	IfxPort_setPinModeOutput(SJA1105_SCK.pin.port, SJA1105_SCK.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
	IfxPort_setPinLow(SJA1105_SDI.pin.port, SJA1105_SDI.pin.pinIndex);
	IfxPort_setPinModeOutput(SJA1105_SDI.pin.port, SJA1105_SDI.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
	//P1 SPI
	IfxPort_setPinLow(QSPI3_SPI_SSR.pin.port, QSPI3_SPI_SSR.pin.pinIndex);
	IfxPort_setPinModeOutput(QSPI3_SPI_SSR.pin.port, QSPI3_SPI_SSR.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
	IfxPort_setPinLow(QSPI3_SPI_MOSIR.pin.port, QSPI3_SPI_MOSIR.pin.pinIndex);
	IfxPort_setPinModeOutput(QSPI3_SPI_MOSIR.pin.port, QSPI3_SPI_MOSIR.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
	IfxPort_setPinModeInput(QSPI3_SPI_MISOR.pin.port, QSPI3_SPI_MISOR.pin.pinIndex,IfxPort_Mode_inputPullDown);  //pull down is required to prevent backdrive
	IfxPort_setPinLow(QSPI3_SPI_SCKR.pin.port, QSPI3_SPI_SCKR.pin.pinIndex);
	IfxPort_setPinModeOutput(QSPI3_SPI_SCKR.pin.port, QSPI3_SPI_SCKR.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
	//TLF (nothing to do here)
	// X1 SPI
	IfxPort_setPinLow(AURIX_X1_SPI_SCKR.pin.port, AURIX_X1_SPI_SCKR.pin.pinIndex);
	IfxPort_setPinModeOutput(AURIX_X1_SPI_SCKR.pin.port, AURIX_X1_SPI_SCKR.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
	IfxPort_setPinModeInput(AURIX_X1_SPI_MISOR.pin.port,AURIX_X1_SPI_MISOR.pin.pinIndex,IfxPort_Mode_inputPullDown);  //pull down is required to prevent backdrive
	IfxPort_setPinLow(AURIX_X1_SPI_CSR.pin.port, AURIX_X1_SPI_CSR.pin.pinIndex);
	IfxPort_setPinModeOutput(AURIX_X1_SPI_CSR.pin.port, AURIX_X1_SPI_CSR.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
	IfxPort_setPinLow(AURIX_X1_SPI_MOSIR.pin.port, AURIX_X1_SPI_MOSIR.pin.pinIndex);
	IfxPort_setPinModeOutput(AURIX_X1_SPI_MOSIR.pin.port, AURIX_X1_SPI_MOSIR.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
	//X2 SPI
	IfxPort_setPinLow(AURIX_X2_SPI_MOSIR.pin.port, AURIX_X2_SPI_MOSIR.pin.pinIndex);
	IfxPort_setPinModeOutput(AURIX_X2_SPI_MOSIR.pin.port, AURIX_X2_SPI_MOSIR.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
	IfxPort_setPinModeInput(AURIX_X2_SPI_MISOR.pin.port,AURIX_X2_SPI_MISOR.pin.pinIndex,IfxPort_Mode_inputPullDown);  //pull down is required to prevent backdrive
	IfxPort_setPinLow(AURIX_X2_SPI_SSR.pin.port, AURIX_X2_SPI_SSR.pin.pinIndex);
	IfxPort_setPinModeOutput(AURIX_X2_SPI_SSR.pin.port, AURIX_X2_SPI_SSR.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
	IfxPort_setPinLow(AURIX_X2_SPI_SCKR.pin.port, AURIX_X2_SPI_SCKR.pin.pinIndex);
	IfxPort_setPinModeOutput(AURIX_X2_SPI_SCKR.pin.port, AURIX_X2_SPI_SCKR.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
	// SPI handshake pins
	IfxPort_setPinLow(E3550_AURIX_P1_SPI_HS_1.port, E3550_AURIX_P1_SPI_HS_1.pinIndex);
	IfxPort_setPinModeOutput(E3550_AURIX_P1_SPI_HS_1.port, E3550_AURIX_P1_SPI_HS_1.pinIndex, IfxPort_OutputMode_openDrain, IfxPort_OutputIdx_general);
	IfxPort_setPinLow(AURIX_X1_SPI_HS_1.port, AURIX_X1_SPI_HS_1.pinIndex);
	IfxPort_setPinModeOutput(AURIX_X1_SPI_HS_1.port, AURIX_X1_SPI_HS_1.pinIndex, IfxPort_OutputMode_openDrain, IfxPort_OutputIdx_general);

	/* for IVI */
	IfxPort_setPinModeInput(AURIX_X1_SPI_HS_2.port, AURIX_X1_SPI_HS_2.pinIndex,IfxPort_Mode_inputNoPullDevice);
	IfxPort_setPinModeInput(AURIX_X2_SPI_HS_2.port, AURIX_X2_SPI_HS_2.pinIndex,IfxPort_Mode_inputNoPullDevice);

	IfxPort_setPinLow(AURIX_X2_SPI_HS_1.port, AURIX_X2_SPI_HS_1.pinIndex);
	IfxPort_setPinModeOutput(AURIX_X2_SPI_HS_1.port, AURIX_X2_SPI_HS_1.pinIndex, IfxPort_OutputMode_openDrain, IfxPort_OutputIdx_general);

	// Set Strap ports
	IfxPort_setPinLow(E3550_ARX_PM8534_BOOT_INDEX.port, E3550_ARX_PM8534_BOOT_INDEX.pinIndex);
	IfxPort_setPinModeOutput(E3550_ARX_PM8534_BOOT_INDEX.port, E3550_ARX_PM8534_BOOT_INDEX.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
	SET_GPIOEXP_OUTPUT_LOW(X1_BSTRP0_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(X1_BSTRP0_EXP);
	SET_GPIOEXP_OUTPUT_LOW(X1_BSTRP1_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(X1_BSTRP1_EXP);
	SET_GPIOEXP_OUTPUT_LOW(X1_BSTRP2_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(X1_BSTRP2_EXP);
	SET_GPIOEXP_OUTPUT_LOW(X2_BSTRP0_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(X2_BSTRP0_EXP);
	SET_GPIOEXP_OUTPUT_LOW(X2_BSTRP1_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(X2_BSTRP1_EXP);
	SET_GPIOEXP_OUTPUT_LOW(X2_BSTRP2_EXP);
	SET_GPIOEXP_CONFIG_OUTPUT(X2_BSTRP2_EXP);

	// Thermal
    /* Share this for E3550 and P3479, we can set the pin even it's not used in P3479*/
	IfxPort_setPinModeInput(E3550_P1_THERM_SHDN_N.port, E3550_P1_THERM_SHDN_N.pinIndex,IfxPort_Mode_inputNoPullDevice);
	IfxPort_setPinModeInput(E3550_P1_THERM_ALERT_3V3_N.port, E3550_P1_THERM_ALERT_3V3_N.pinIndex,IfxPort_Mode_inputPullDown);
	IfxPort_setPinModeInput(E3550_X1_THERM_SHDN_3V3_N.port, E3550_X1_THERM_SHDN_3V3_N.pinIndex,IfxPort_Mode_inputPullDown);
	IfxPort_setPinModeInput(E3550_BRD_THERM_ALERT_N.port, E3550_BRD_THERM_ALERT_N.pinIndex,IfxPort_Mode_inputNoPullDevice);
	/* set it with pull down is to save power as the sensor is not working during power off state */
	IfxPort_setPinModeInput(X2_THERM_ALERT_3V3_N.port, X2_THERM_ALERT_3V3_N.pinIndex,IfxPort_Mode_inputPullDown);
	IfxPort_setPinModeInput(X1_THERM_ALERT_3V3_N.port, X1_THERM_ALERT_3V3_N.pinIndex,IfxPort_Mode_inputPullDown);
	IfxPort_setPinModeInput(X2_THERM_SHDN_3V3_R_N.port, X2_THERM_SHDN_3V3_R_N.pinIndex,IfxPort_Mode_inputPullDown);
	/* board thermal sensor is always on, so always set it input no pull-up/pull-down
	 * It's better to set it input, as the GPIO default state is control by the HWCFG strap,
	 * it could be input pull-high */
	IfxPort_setPinModeInput(BRD_THERM_SHDN_N.port, BRD_THERM_SHDN_N.pinIndex,IfxPort_Mode_inputNoPullDevice);
}

//endof SetOutputs_to_System_PowerOff_State()


/** \brief configure/set all outputs to System Inactive State
 *
 * This function set the outputs to needed state for System Inactive State
 * System Inactive State is to be entered when Power to the Tegra have been enabled
 * prior to releasing reset
 * Power Sequence:  Poweroff - Enable Power - Inactive - Release Reset
 */
static void SetOutputs_to_System_Inactive_State_3550_3479(void)
{
	uint8 boot_media_ta;
	uint8 boot_media_tb;
    GPIO_ALWAYS_PRINTF(("SetOutputs_to_System_Inactive_State_3550_3479.\r\n")); 
    return 1;
	// Set Fan ports
#if FAN_A_ENABLE == 1
	IfxPort_setPinHigh(AURIX_X1_FANCTL.port, AURIX_X1_FANCTL.pinIndex);
#endif
#if FAN_B_ENABLE == 1
	IfxPort_setPinHigh(AURIX_X2_FANCTL.port, AURIX_X2_FANCTL.pinIndex);
#endif

#if PULL_FAN_A_ENABLE == 1 || PULL_FAN_B_ENABLE == 1
	IfxPort_setPinHigh(E3550_ARX_X1_FAN2_PWM_OVERRIDE.port, E3550_ARX_X1_FAN2_PWM_OVERRIDE.pinIndex);
	IfxPort_setPinModeOutput(E3550_ARX_X1_FAN2_PWM_OVERRIDE.port, E3550_ARX_X1_FAN2_PWM_OVERRIDE.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	IfxPort_setPinHigh(E3550_ARX_X2_FAN2_PWM_OVERRIDE.port, E3550_ARX_X2_FAN2_PWM_OVERRIDE.pinIndex);
	IfxPort_setPinModeOutput(E3550_ARX_X2_FAN2_PWM_OVERRIDE.port, E3550_ARX_X2_FAN2_PWM_OVERRIDE.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
#endif

	// set Eray ports
	IfxPort_setPinHigh(FLEXRAY_ENABLE_N.port, FLEXRAY_ENABLE_N.pinIndex);
	SET_GPIOEXP_OUTPUT_HIGH(FR_A_STB_N_EXP);
	SET_GPIOEXP_OUTPUT_HIGH(FR_B_STB_N_EXP);

	// Interrupt
	IfxPort_setPinModeInput(E3550_CIM_PWR_INT_N.port, E3550_CIM_PWR_INT_N.pinIndex,IfxPort_Mode_inputNoPullDevice);

	// Set Mdio
	SET_GPIOEXP_OUTPUT_HIGH(ARX_ETH_2112_GRP2_MDC_CTRL_EXP);
	SET_GPIOEXP_OUTPUT_HIGH(ARX_ETH_Q5050_MDC_CTRL_EXP);

	// power
	IfxPort_setPinHigh(VBAT_SYS_PWR_ON.port, VBAT_SYS_PWR_ON.pinIndex);
	IfxPort_setPinHigh(E3550_AURIX_P1_WAKE.port, E3550_AURIX_P1_WAKE.pinIndex);
	IfxPort_setPinLow(AURIX_X1_WAKE.port, AURIX_X1_WAKE.pinIndex);
	IfxPort_setPinLow(AURIX_X2_WAKE.port, AURIX_X2_WAKE.pinIndex);
	IfxPort_setPinHigh(VBAT_TEG_PWR_ON.port, VBAT_TEG_PWR_ON.pinIndex);
	IfxPort_setPinHigh(X1_PMIC_POWER_ON_R.port, X1_PMIC_POWER_ON_R.pinIndex);
	IfxPort_setPinHigh(X2_PMIC_POWER_ON_R.port, X2_PMIC_POWER_ON_R.pinIndex);
	SET_GPIOEXP_OUTPUT_HIGH(ARX_SXMA_VBAT_EN_EXP);
	SET_GPIOEXP_OUTPUT_HIGH(ARX_SXMB_VBAT_EN_EXP);

	if(board.project == E3550)
	{ /* E3550, we will detect 2nd harness cable and print out warning for now */
		if(!READ_GPIOEXP_STATE(E3550_HRN2_DET_EXP))
		{
			GPIO_ALWAYS_PRINTF(("Detect 2nd Harness is not connected.\r\n"
					"If you are using DGPU, please contact your hardware representative to get 2nd harness cable\r\n"));
		}
	}

	//recovery
	IfxPort_setPinHigh(X1_FORCE_RCVR_R_N.port, X1_FORCE_RCVR_R_N.pinIndex);
	IfxPort_setPinHigh(X2_FORCE_RCVR_R_N.port, X2_FORCE_RCVR_R_N.pinIndex);
	IfxPort_setPinHigh(E3550_P1_FORCE_RCVR_N.port, E3550_P1_FORCE_RCVR_N.pinIndex);
	SET_GPIOEXP_OUTPUT_HIGH(ARX_PEXSW1_BOOT_REC_EXP);
	SET_GPIOEXP_OUTPUT_HIGH(ARX_PEXSW2_BOOT_REC_EXP);

	// Spi
	IfxPort_setPinHigh(SJA1105_CSN.pin.port, SJA1105_CSN.pin.pinIndex);
	IfxPort_setPinHigh(QSPI3_SPI_SSR.pin.port, QSPI3_SPI_SSR.pin.pinIndex);
	IfxPort_setPinHigh(AURIX_X1_SPI_CSR.pin.port, AURIX_X1_SPI_CSR.pin.pinIndex);
	IfxPort_setPinHigh(AURIX_X2_SPI_SSR.pin.port, AURIX_X2_SPI_SSR.pin.pinIndex);

	// boot strap for Qspi (001) default
	/* Read boot medium from inforom, if inforom is unflashed or no valid
	 * "sys" header on inforom, it will return default (Qspi) medium
	 * boot_media :  Medium
	 *    0       :   UFS
	 *    1       :   Qspi
	 *    2       :   Emmc
	 */
	i2crom_read_boot_media(&boot_media_ta, &boot_media_tb);
	switch(boot_media_ta)
	{
	case 0:
		//strap value = 5
		SET_GPIOEXP_OUTPUT_HIGH(X1_BSTRP0_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X1_BSTRP1_EXP);
		SET_GPIOEXP_OUTPUT_HIGH(X1_BSTRP2_EXP);
		GPIO_ALWAYS_PRINTF(("Configured BootStraps for X1 as UFS\n"));
		break;
	case 1:
		//strap value = 1
		SET_GPIOEXP_OUTPUT_HIGH(X1_BSTRP0_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X1_BSTRP1_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X1_BSTRP2_EXP);
		GPIO_ALWAYS_PRINTF(("Configured BootStraps for X1 as QSPI\n"));
		break;
	case 2:
		//strap value = 0
		SET_GPIOEXP_OUTPUT_LOW(X1_BSTRP0_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X1_BSTRP1_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X1_BSTRP2_EXP);
		GPIO_ALWAYS_PRINTF(("Configured BootStraps for X1 as EMMC\n"));
		break;
	default:
		//strap value = 1
		SET_GPIOEXP_OUTPUT_HIGH(X1_BSTRP0_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X1_BSTRP1_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X1_BSTRP2_EXP);
		GPIO_ALWAYS_PRINTF(("Configured BootStraps for X1 as QSPI\n"));
		break;
	}
	switch(boot_media_tb)
	{
	case 0:
		//strap value = 5
		SET_GPIOEXP_OUTPUT_HIGH(X2_BSTRP0_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X2_BSTRP1_EXP);
		SET_GPIOEXP_OUTPUT_HIGH(X2_BSTRP2_EXP);
		GPIO_ALWAYS_PRINTF(("Configured BootStraps for X2 as UFS\n"));
		break;
	case 1:
		//strap value = 1
		SET_GPIOEXP_OUTPUT_HIGH(X2_BSTRP0_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X2_BSTRP1_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X2_BSTRP2_EXP);
		GPIO_ALWAYS_PRINTF(("Configured BootStraps for X2 as QSPI\n"));
		break;
	case 2:
		//strap value = 0
		SET_GPIOEXP_OUTPUT_LOW(X2_BSTRP0_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X2_BSTRP1_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X2_BSTRP2_EXP);
		GPIO_ALWAYS_PRINTF(("Configured BootStraps for X2 as EMMC\n"));
		break;
	default:
		//strap value = 1
		SET_GPIOEXP_OUTPUT_HIGH(X2_BSTRP0_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X2_BSTRP1_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X2_BSTRP2_EXP);
		GPIO_ALWAYS_PRINTF(("Configured BootStraps for X2 as QSPI\n"));
		break;
	}

	IfxPort_setPinModeInput(E3550_P1_THERM_ALERT_3V3_N.port, E3550_P1_THERM_ALERT_3V3_N.pinIndex,IfxPort_Mode_inputNoPullDevice);
	IfxPort_setPinModeInput(E3550_P1_THERM_SHDN_N.port, E3550_P1_THERM_SHDN_N.pinIndex,IfxPort_Mode_inputNoPullDevice);
	IfxPort_setPinModeInput(E3550_X1_THERM_SHDN_3V3_N.port, E3550_X1_THERM_SHDN_3V3_N.pinIndex,IfxPort_Mode_inputNoPullDevice);

	// Thermal
	IfxPort_setPinModeInput(X1_THERM_ALERT_3V3_N.port, X1_THERM_ALERT_3V3_N.pinIndex,IfxPort_Mode_inputNoPullDevice);
	IfxPort_setPinModeInput(X2_THERM_ALERT_3V3_N.port, X2_THERM_ALERT_3V3_N.pinIndex,IfxPort_Mode_inputNoPullDevice);
	IfxPort_setPinModeInput(X2_THERM_SHDN_3V3_R_N.port, X2_THERM_SHDN_3V3_R_N.pinIndex,IfxPort_Mode_inputNoPullDevice);

	/* We need to set MDIO open-drain, high, to work with the level-shiffter */
	IfxPort_setPinHigh(PHY_MDIO.pin.port, PHY_MDIO.pin.pinIndex);
	IfxPort_setPinModeOutput(PHY_MDIO.pin.port, PHY_MDIO.pin.pinIndex, IfxPort_OutputMode_openDrain, IfxPort_OutputIdx_general);

}

static boolean System_PowerOff_3550_3479(void)
{
	boolean cim_sgood;
    GPIO_ALWAYS_PRINTF(("System_PowerOff_3550_3479.\r\n")); 
    return 1;
	if(system_state == poweroff_state)
	{
		GPIO_ALWAYS_PRINTF(("The board has been powered off.\r\n"));
		return FALSE;
	}
	/* Turn off CIM power */
	cim_sgood = CIM_Check_SGood_Status();
	if (cim_sgood)
	{
		CIM_Power_Control(FALSE);
	}
	else
	{
		GPIO_DEBUG_PRINTF(("System_PowerOff: CIM module not detected"));
	}

	/* stop the ethernet */
	Ethernet_Stop();

	// Trigger all resets
	IfxPort_setPinLow(SJA1105_RSTN.port, SJA1105_RSTN.pinIndex);
	IfxPort_setPinLow(AURIX_XA_RST_N.port, AURIX_XA_RST_N.pinIndex);

	IfxPort_setPinLow(AURIX_XB_RST_N.port, AURIX_XB_RST_N.pinIndex);
	SET_GPIOEXP_OUTPUT_LOW(CIM_RST_N_EXP);

	SET_GPIOEXP_OUTPUT_LOW(ARX_ETHSW_PHY_GRP1_RESET_N_EXP);
	SET_GPIOEXP_OUTPUT_LOW(ARX_ETHSW_PHY_GRP2_RESET_N_EXP);

	SET_GPIOEXP_OUTPUT_LOW(E3550_ARX_PM8534_RESET_N_EXP);

	if((board.project == E3550) || (board.project == P3479))
	{
		IfxPort_setPinLow(E3550_ESATA_PEX_RST_N.port, E3550_ESATA_PEX_RST_N.pinIndex);
		IfxPort_setPinLow(E3550_AURIX_P1_RST_N.port, E3550_AURIX_P1_RST_N.pinIndex);
		IfxPort_setPinLow(E3550_AQC107_1_PEX_RESET_N.port, E3550_AQC107_1_PEX_RESET_N.pinIndex);
		IfxPort_setPinLow(E3550_AQC107_2_PEX_RESET_N.port, E3550_AQC107_2_PEX_RESET_N.pinIndex);
		SET_GPIOEXP_OUTPUT_LOW(E3550_PEXSW2_RESET_OUTPUT_N_EXP);
		SET_GPIOEXP_OUTPUT_LOW(E3550_COMEXP_RESET_CTRL_N_EXP);
		SET_GPIOEXP_OUTPUT_LOW(E3550_ARX_AQR107_RST_N_EXP);
		SET_GPIOEXP_OUTPUT_LOW(E3550_M2_RESET_N_EXP);
	}

	// power off ports

	if(board.project == E3550)
	{
		SET_GPIOEXP_OUTPUT_LOW(E3550_CIM_PWR_ON_EXP);
		SET_GPIOEXP_OUTPUT_LOW(E3550_COMEXP_PWR_EN_EXP);
	}

	waitTime(50*TimeConst[TIMER_INDEX_1MS]);	// this is just a placeholder value for delay
	if(board.project == E3550)
	{
		SET_GPIOEXP_OUTPUT_LOW(E3550_CE_PREREG_EN_EXP);
	}
	IfxPort_setPinLow(VBAT_SYS_PWR_ON.port, VBAT_SYS_PWR_ON.pinIndex);

	if(board.project == E3550)
	{
		IfxPort_setPinLow(E3550_AURIX_P1_PWR_ON_R.port, E3550_AURIX_P1_PWR_ON_R.pinIndex);
	}
	IfxPort_setPinLow(X1_PMIC_POWER_ON_R.port, X1_PMIC_POWER_ON_R.pinIndex);
	if(board.project == E3550)
		IfxPort_setPinLow(X2_PMIC_POWER_ON_R.port, X2_PMIC_POWER_ON_R.pinIndex);

	waitTime(10*TimeConst[TIMER_INDEX_1MS]);	// this is just a placeholder value for delay
	if(board.project == E3550)
	{
		SET_GPIOEXP_OUTPUT_LOW(ARX_SXMA_VBAT_EN_EXP);
		SET_GPIOEXP_OUTPUT_LOW(ARX_SXMB_VBAT_EN_EXP);
	}

	IfxPort_setPinLow(VBAT_TEG_PWR_ON.port, VBAT_TEG_PWR_ON.pinIndex);

	// Execute poweroff states on all outputs
	SetOutputs_to_System_PowerOff_State();
	GPIO_DEBUG_PRINTF(("SetOutputs_to_System_PowerOff_State\n"));

	pmic_a_on = FALSE;
	pmic_b_on = FALSE;
	pmic_c_on = FALSE;
	dgpu_a_on = FALSE;
	dgpu_b_on = FALSE;
	whole_board_off = TRUE;

	GPIO_ALWAYS_PRINTF(("Power off the system\n"));
	system_state = poweroff_state;
	return TRUE;
}

#endif




static boolean System_PowerOff_3570(void)
{
    GPIO_ALWAYS_PRINTF(("System_PowerOff_3570.\r\n"));
	if(system_state == poweroff_state)
	{
		GPIO_ALWAYS_PRINTF(("The board has been powered off.\r\n"));
		return FALSE;
	}
	//for VW80000 test
	g_poweroff_flag = TRUE;

	/* stop the ethernet */
	Ethernet_Stop();
	// Trigger all resets
	CIM_Power_Control(FALSE);
    SET_GPIOEXP_OUTPUT_LOW(P3570_NVME_RSTB);
	IfxPort_setPinLow(SJA1105_RSTN.port, SJA1105_RSTN.pinIndex);
	SET_GPIOEXP_OUTPUT_LOW(ARX_ETHSW_PHY_GRP1_RESET_N_EXP);
	SET_GPIOEXP_OUTPUT_LOW(ARX_ETHSW_PHY_GRP2_RESET_N_EXP);
	IfxPort_setPinLow(P3570_LAN7431_RST_N.port, P3570_LAN7431_RST_N.pinIndex);
	IfxPort_setPinLow(AURIX_XA_RST_N.port, AURIX_XA_RST_N.pinIndex);
	IfxPort_setPinLow(AURIX_XB_RST_N.port, AURIX_XB_RST_N.pinIndex);
	SET_GPIOEXP_OUTPUT_LOW(P3570_PEXSW1_RESET_N_EXP);
	IfxPort_setPinLow(P3570_AURIX_NVME_PEX_RESET_N.port, P3570_AURIX_NVME_PEX_RESET_N.pinIndex);
	IfxPort_setPinLow(P3570_AURIX2_RST_N.port, P3570_AURIX2_RST_N.pinIndex);
	SET_GPIOEXP_OUTPUT_LOW(P3570_B2B_TX_SYS_RST_N_EXP);
	//TODO: remove it
	waitTime(50*TimeConst[TIMER_INDEX_1MS]);     // 50: Actually be 100ms sometimes.

	//power off 2nd board
	SET_GPIOEXP_OUTPUT_LOW(P3570_B2B_TX_PWR_EN_EXP);

	//power off AURIX2
	IfxPort_setPinLow(P3570_AURIX2_PWR_ON.port, P3570_AURIX2_PWR_ON.pinIndex);

	//power off SYS devices
	IfxPort_setPinLow(VBAT_SYS_PWR_ON.port, VBAT_SYS_PWR_ON.pinIndex);

	//power off Tegras
	IfxPort_setPinLow(X1_PMIC_POWER_ON_R.port, X1_PMIC_POWER_ON_R.pinIndex);
	IfxPort_setPinLow(X2_PMIC_POWER_ON_R.port, X2_PMIC_POWER_ON_R.pinIndex);

	waitTime(10*TimeConst[TIMER_INDEX_1MS]);
	//power off SXMs
	SET_GPIOEXP_OUTPUT_LOW(ARX_SXMA_VBAT_EN_EXP);
	SET_GPIOEXP_OUTPUT_LOW(ARX_SXMB_VBAT_EN_EXP);

	IfxPort_setPinLow(VBAT_TEG_PWR_ON.port, VBAT_TEG_PWR_ON.pinIndex);
	// Execute poweroff states on all outputs
	SetOutputs_to_System_PowerOff_State();
	GPIO_DEBUG_PRINTF(("SetOutputs_to_System_PowerOff_State\r\n"));

	pmic_a_on = FALSE;
	pmic_b_on = FALSE;
	pmic_c_on = FALSE;
	dgpu_a_on = FALSE;
	dgpu_b_on = FALSE;
	whole_board_off = TRUE;

	GPIO_ALWAYS_PRINTF(("Power off the system\r\n"));
	system_state = poweroff_state;

	//for VW80000 test
	g_poweroff_flag = FALSE;
	return TRUE;
}
#if 0
static boolean System_PowerOn_3550_3479(boolean isForcePoweron)
{
	uint32 count = 0;
	uint32 ce_mode = 0;
	uint32 sxm_a_mV = 0;
	uint32 sxm_b_mV = 0;
	boolean cim_sgood;

    return 1;
    GPIO_ALWAYS_PRINTF(("System_PowerOn_3550_3479.\r\n"));
	if(system_state == poweron_state)
	{
		GPIO_ALWAYS_PRINTF(("The board has been powered on.\r\n"));
		GPIO_ALWAYS_PRINTF(("Please run \"poweroff\" first if you want to do a power cycle\r\n"));
		return FALSE;
	}
	// Execute power-off state
	// all our ports are tristate, we set the pins to the needed level for System PowerOff State
	SetOutputs_to_System_PowerOff_State();
	GPIO_DEBUG_PRINTF(("SetOutputs_to_System_PowerOff_State\n"));

	// Power up rails
	IfxPort_setPinHigh(VBAT_TEG_PWR_ON.port, VBAT_TEG_PWR_ON.pinIndex);

	//TODO: check if all the waiting functions can be move into one functions,
	//      as they're all similar waiting for PG signals, e.g.
	//      wait_power_good_signals(power_good_signals_array_p, size);
	if (!isForcePoweron)
	{
		if(board.project == E3550)
		{
			if(Check_PGoodSignals(3, (&XA_5V_PG), (&XB_5V_PG), (&E3550_P1_5V_PG)) == FALSE)
			{
				GPIO_ALWAYS_PRINTF(("System_PowerOn: Reading PG status: Timeout Error!\n"));
				GPIO_ALWAYS_PRINTF(("XA_5V_PG/XB_5V_PG/P1_5V_PG\r\n"));
				return FALSE;
			}
		}
		else
		{ /* P3479 */
			if(Check_PGoodSignals(1, (&XA_5V_PG)) == FALSE)
			{
				GPIO_ALWAYS_PRINTF(("System_PowerOn: Reading PG status: Timeout Error!\n"));
				GPIO_ALWAYS_PRINTF(("XA_5V_PG\r\n"));
				return FALSE;
			}
		}
	}
	waitTime(50*TimeConst[TIMER_INDEX_1MS]);

	// Check CVM Present status
	if(board.project == E3550)
	{
		if (!READ_GPIOEXP_STATE(E3550_P1_CVM_PRSNT_N_EXP))
		{
			// CVM is present, power on CVM
			IfxPort_setPinHigh(E3550_AURIX_P1_PWR_ON_R.port, E3550_AURIX_P1_PWR_ON_R.pinIndex);
			IfxPort_setPinModeOutput(E3550_AURIX_P1_PWR_ON_R.port, E3550_AURIX_P1_PWR_ON_R.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
		}
		else
		{ /* specific set it to low when CVM is not there */
			IfxPort_setPinLow(E3550_AURIX_P1_PWR_ON_R.port, E3550_AURIX_P1_PWR_ON_R.pinIndex);
			IfxPort_setPinModeOutput(E3550_AURIX_P1_PWR_ON_R.port, E3550_AURIX_P1_PWR_ON_R.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
		}
	}

	// Power on X1 and X2
	IfxPort_setPinHigh(X1_PMIC_POWER_ON_R.port, X1_PMIC_POWER_ON_R.pinIndex);
	if(board.project == E3550)
		IfxPort_setPinHigh(X2_PMIC_POWER_ON_R.port, X2_PMIC_POWER_ON_R.pinIndex);

	if (!isForcePoweron)
	{
		if(Check_PGoodSignals(1, (&X1_XA_VDD_SOC_XA_VDD_DDR2_XA_VDD_DDRQ_PG)) == FALSE)
		{
			GPIO_ALWAYS_PRINTF(("System_PowerOn: Reading PG status: Timeout Error!\n"));
			GPIO_ALWAYS_PRINTF(("X1_XA_VDD_SOC_XA_VDD_DDR2_XA_VDD_DDRQ_PG\r\n"));
			return FALSE;
		}

		if(board.project == E3550)
		{
			if(Check_PGoodSignals(1, (&X2_XB_VDD_SOC_XB_VDD_DDR2_XB_VDD_DDRQ_PG)) == FALSE)
			{
				GPIO_ALWAYS_PRINTF(("System_PowerOn: Reading PG status: Timeout Error!\n"));
				GPIO_ALWAYS_PRINTF(("X2_XB_VDD_SOC_XB_VDD_DDR2_XB_VDD_DDRQ_PG\r\n"));
				return FALSE;
			}
		}
	}

	IfxPort_setPinHigh(VBAT_SYS_PWR_ON.port, VBAT_SYS_PWR_ON.pinIndex);

	if (!isForcePoweron)
	{
		if(board.project == E3550)
		{
			if(Check_PGoodSignals(4,
					(&SYS_5V_PG),
					(&SYS_0V85_SYS_1V0_SYS_1V2_PG),
					(&SYS_3V3_SYS_1V8_SYS_0V9_PG1),
					(&SYS_3V3_SYS_1V8_SYS_1V5_SYS_1V1_SYS_0V9_PG2)) == FALSE)
			{
				GPIO_ALWAYS_PRINTF(("System_PowerOn: Reading PG status: Timeout Error!\n"));
				GPIO_ALWAYS_PRINTF(("SYS_5V_PG, SYS_0V85_SYS_1V0_SYS_1V2_PG,\r\n"));
				GPIO_ALWAYS_PRINTF(("SYS_3V3_SYS_1V8_SYS_0V9_PG1, SYS_3V3_SYS_1V8_SYS_1V5_SYS_1V1_SYS_0V9_PG2\r\n"));
				return FALSE;
			}
		}
		else
		{ /* P3479 */
			if ((board.pcb_spin == 'A') && (board.pcb_version >= 3))
			{ /* A03 has no SYS_5V_PG */
				if(Check_PGoodSignals(2,
						(&SYS_0V85_SYS_1V0_SYS_1V2_PG),
						(&SYS_3V3_SYS_1V8_SYS_0V9_PG1)) == FALSE)
				{
					GPIO_ALWAYS_PRINTF(("System_PowerOn: Reading PG status: Timeout Error!\n"));
					GPIO_ALWAYS_PRINTF(("SYS_0V85_SYS_1V0_SYS_1V2_PG,\r\n"));
					GPIO_ALWAYS_PRINTF(("SYS_3V3_SYS_1V8_SYS_0V9_PG1\r\n"));
					return FALSE;
				}
			}
			else
			{
				if(Check_PGoodSignals(3,
						(&SYS_5V_PG),
						(&SYS_0V85_SYS_1V0_SYS_1V2_PG),
						(&SYS_3V3_SYS_1V8_SYS_0V9_PG1)) == FALSE)
				{
					GPIO_ALWAYS_PRINTF(("System_PowerOn: Reading PG status: Timeout Error!\n"));
					GPIO_ALWAYS_PRINTF(("SYS_5V_PG, SYS_0V85_SYS_1V0_SYS_1V2_PG,\r\n"));
					GPIO_ALWAYS_PRINTF(("SYS_3V3_SYS_1V8_SYS_0V9_PG1\r\n"));
					return FALSE;
				}
			}

		}
	}

	waitTime(30*TimeConst[TIMER_INDEX_1MS]);	// this is just a placeholder value for delay

	// Execute inactive state
	GPIO_DEBUG_PRINTF(("SetOutputs_to_System_Inactive_State\n"));
	/* boot strap is set inside the func */
	SetOutputs_to_System_Inactive_State();	//System Inactive State is to be entered when Power to the Tegra have been enabled

	/* QSPI0 connects to Tegra A on DAX (0.7, 0.75, 0.9)
       Initialise QSPI0 as master, with mode 1 default */
	TegraQspiMaster_init(0, 1);
#if PROJECT == PROJECT_DRIVE_AX && DRIVE_AX_B0x
	/* Additionally, also init QSPI1 (SJA1105),  QSPI3 (CVM)and QSPI4 (Tegra B), with mode 1 default */
	TegraQspiMaster_init(1, 1);
	if(board.project == E3550)
	{
		TegraQspiMaster_init(3, 1);
		TegraQspiMaster_init(4, 1);
	}
#endif
	/* Check VMONs for P3570 and P3479-A03 */
	if(board.project == P3570 || (board.project == P3479 && board.pcb_spin == 'A' && board.pcb_version >= 3))
	{
		if(!Vmon_init())
			GPIO_ALWAYS_PRINTF(("VMON initialization failed !!\n"));
	}

	if(board.project == E3550)
	{
		/* If inforom SYS header is programmed, i2crom_read_ce_mode() returns
		 * the value read from inforom, else it returns default CE_Mode = 0
		 */
		ce_mode = i2crom_read_ce_mode();
		GPIO_ALWAYS_PRINTF(("System Powering on in CE_MODE = %d ...\n", ce_mode));

		/*   CE_MODE Definition:
		 *   0:  Both Tegra Control DGPUs
		 *   1:  COM Express control DGPU A, X2 control DGPU B
		 *   2:  COM Express control DGPU B, X1 control DGPU A
		 *   3:  COM Express control DGPU A and DGPU B
		 */

	#if CE_SUPPORT == 1
		if ((ce_mode > 0) && (!(IfxPort_getPinState(E3550_COMEXP_PRESENT_DET_GND_N.port, E3550_COMEXP_PRESENT_DET_GND_N.pinIndex))))
		{
			if (ce_mode == 1)
			{
				IfxPort_setPinHigh(E3550_ARX_DGPU1_MUX_CTRL.port, E3550_ARX_DGPU1_MUX_CTRL.pinIndex);
				IfxPort_setPinLow(E3550_ARX_DGPU2_MUX_CTRL.port, E3550_ARX_DGPU2_MUX_CTRL.pinIndex);
			}
			else if (ce_mode == 2)
			{
				IfxPort_setPinLow(E3550_ARX_DGPU1_MUX_CTRL.port, E3550_ARX_DGPU1_MUX_CTRL.pinIndex);
				IfxPort_setPinHigh(E3550_ARX_DGPU2_MUX_CTRL.port, E3550_ARX_DGPU2_MUX_CTRL.pinIndex);
			}
			else if (ce_mode == 3)
			{
				IfxPort_setPinHigh(E3550_ARX_DGPU1_MUX_CTRL.port, E3550_ARX_DGPU1_MUX_CTRL.pinIndex);
				IfxPort_setPinHigh(E3550_ARX_DGPU2_MUX_CTRL.port, E3550_ARX_DGPU2_MUX_CTRL.pinIndex);
			}
		}
		else
	#endif
		{
			/* Either CE not supported or CE_MODE = 0 */
			IfxPort_setPinLow(E3550_ARX_DGPU1_MUX_CTRL.port, E3550_ARX_DGPU1_MUX_CTRL.pinIndex);
			IfxPort_setPinLow(E3550_ARX_DGPU2_MUX_CTRL.port, E3550_ARX_DGPU2_MUX_CTRL.pinIndex);

		}
	}// endof if(board.project == E3550)

	// Release resets
	//Exit Inactive State, Entering Active State with release of reset
	IfxPort_setPinHigh(SJA1105_RSTN.port, SJA1105_RSTN.pinIndex);
	SET_GPIOEXP_OUTPUT_HIGH(ARX_ETHSW_PHY_GRP1_RESET_N_EXP);
	SET_GPIOEXP_OUTPUT_HIGH(ARX_ETHSW_PHY_GRP2_RESET_N_EXP);
	if(board.project == E3550)
	{
		SET_GPIOEXP_OUTPUT_HIGH(E3550_M2_RESET_N_EXP);
		SET_GPIOEXP_OUTPUT_HIGH(E3550_ARX_PM8534_RESET_N_EXP);
		waitTime(60*TimeConst[TIMER_INDEX_1MS]);	// for PEX Switch
	}
	if(board.project == P3479 && board.pcb_spin == 'A' && board.pcb_version >= 3) // P3479-A03 has LAN7431
		SET_GPIOEXP_OUTPUT_HIGH(P3479_LAN7431_RESET_N_EXP);
	if(board.project == P3570)
		IfxPort_setPinHigh(P3570_LAN7431_RST_N.port, P3570_LAN7431_RST_N.pinIndex);
	if(board.project == E3550)
	{
		IfxPort_setPinHigh(E3550_AQC107_1_PEX_RESET_N.port, E3550_AQC107_1_PEX_RESET_N.pinIndex);
		IfxPort_setPinHigh(E3550_AQC107_2_PEX_RESET_N.port, E3550_AQC107_2_PEX_RESET_N.pinIndex);
		IfxPort_setPinHigh(E3550_ESATA_PEX_RST_N.port, E3550_ESATA_PEX_RST_N.pinIndex);
	}
	IfxPort_setPinHigh(AURIX_XA_RST_N.port, AURIX_XA_RST_N.pinIndex);
	if((board.project == E3550)||(board.project == P3570))
	{
		IfxPort_setPinHigh(AURIX_XB_RST_N.port, AURIX_XB_RST_N.pinIndex);
	}
	if(board.project == E3550)
	{
		IfxPort_setPinHigh(E3550_AURIX_P1_RST_N.port, E3550_AURIX_P1_RST_N.pinIndex);
	}
	if(board.project == E3550)
	{
#if CE_SUPPORT == 1
		if (!(IfxPort_getPinState(E3550_COMEXP_PRESENT_DET_GND_N.port, E3550_COMEXP_PRESENT_DET_GND_N.pinIndex)))
		{
			if (ce_mode == 1)
			{
				sxm_a_mV = VadcAutoScan_getVoltage_mV(VBAT_SXMA_CHANNEL);
				count = 0;
				while ((!sxm_a_mV))
				{
					waitTime(1*TimeConst[TIMER_INDEX_1S]);	// this is just a placeholder value for delay
					count ++;
					if (count >= TIMEOUT_CNTR_SXM_1S)
					{
						GPIO_ALWAYS_PRINTF(("System_PowerOn: Reading VBAT_SXMA Timeout Error!\n"));
						return FALSE;
					}
				}
			}
			else if (ce_mode == 2)
			{
				sxm_b_mV = VadcAutoScan_getVoltage_mV(VBAT_SXMB_CHANNEL);
				count = 0;
				while ((!sxm_b_mV))
				{
					waitTime(1*TimeConst[TIMER_INDEX_1S]);	// this is just a placeholder value for delay
					count ++;
					if (count >= TIMEOUT_CNTR_SXM_1S)
					{
						GPIO_ALWAYS_PRINTF(("System_PowerOn: Reading VBAT_SXMB Timeout Error!\n"));
						return FALSE;
					}
				}
			}
			else if (ce_mode == 3)
			{
				sxm_a_mV = VadcAutoScan_getVoltage_mV(VBAT_SXMA_CHANNEL);
				sxm_b_mV = VadcAutoScan_getVoltage_mV(VBAT_SXMB_CHANNEL);

				count = 0;
				while ((!sxm_a_mV) && (!sxm_b_mV))
				{
					waitTime(1*TimeConst[TIMER_INDEX_1S]);	// this is just a placeholder value for delay
					count ++;
					if (count >= TIMEOUT_CNTR_SXM_1S)
					{
						GPIO_ALWAYS_PRINTF(("System_PowerOn: Reading VBAT_SXMA/VBAT_SXMB Timeout Error!\n"));
						return FALSE;
					}
				}
			}

			SET_GPIOEXP_OUTPUT_HIGH(E3550_CE_PREREG_EN_EXP);

			SET_GPIOEXP_OUTPUT_HIGH(E3550_COMEXP_PWR_EN_EXP);
			SET_GPIOEXP_OUTPUT_HIGH(E3550_PEXSW2_RESET_OUTPUT_N_EXP);

			waitTime(60*TimeConst[TIMER_INDEX_1MS]);	// this is just a placeholder value for delay

			SET_GPIOEXP_OUTPUT_HIGH(E3550_ARX_TRUST_PLATFORM_CTRL_EXP);
			// Tristate by configuring IOExpander pin as input
			SET_GPIOEXP_CONFIG_INPUT(E3550_ARX_AQR107_RST_N_EXP);
			SET_GPIOEXP_OUTPUT_HIGH(E3550_COMEXP_RESET_CTRL_N_EXP);


		} //endof if (!(IfxPort_getPinState(COMEXP_PRESENT_DET_GND_N.port, COMEXP_PRESENT_DET_GND_N.pinIndex)))
	} //endof if(board.project == E3550)
#endif

	// Active state completed

	pmic_a_on = TRUE;
	pmic_b_on = TRUE;
	pmic_c_on = TRUE;
	whole_board_off = FALSE;
	if((board.pcb_spin == 'B') && (board.pcb_version > 1))
	{
		if(VadcAutoScan_getVoltage_mV(VBAT_SXMA_CHANNEL))
			dgpu_a_on = TRUE;
		else
			dgpu_a_on = FALSE;

		if(VadcAutoScan_getVoltage_mV(VBAT_SXMB_CHANNEL))
			dgpu_b_on = TRUE;
		else
			dgpu_b_on = FALSE;
	}
#if ETH_AUTO_START_ENABLE == 1
	/* we change it to static ip  10.42.0.146 at start up */
	t_ip_addr ip;
	// main Aurix 10.42.0.146
	ip.ipaddr1 = 10;
	ip.ipaddr2 = 42;
	ip.ipaddr3 = 0;
	ip.ipaddr4 = 146;
	ip.mask1 = 255;
	ip.mask2 = 255;
	ip.mask3 = 255;
	ip.mask4 = 0;
	Ethernet_Init(STATIC_IP, &ip);
#endif
	if(board.project == E3550)
	{
		toggle_levelShifter(); //WAR for CVM level shifter
	}
	/* Turn on CIM power */
	cim_sgood = CIM_Check_SGood_Status();
	if (cim_sgood)
	{
		CIM_Power_Control(TRUE);
	}
	else
	{
		GPIO_DEBUG_PRINTF(("System_PowerOn: CIM module not detected"));
	}

	i2c_update_gr();
	GPIO_ALWAYS_PRINTF(("Power on the system\n"));
	system_state = poweron_state;
	return TRUE;
}
#endif

boolean check_pin_status(void)
{
   if(Check_PGoodSignals(1, (&P3570_B2B_PG_in)) == TRUE){
     return TRUE;
   }else{
     return FALSE;
   }
}

void pull_poweron()
{
    IfxPort_setPinLow(VBAT_TEG_PWR_ON.port, VBAT_TEG_PWR_ON.pinIndex);
    waitTime(50*TimeConst[TIMER_INDEX_1MS]);
    IfxPort_setPinHigh(VBAT_TEG_PWR_ON.port, VBAT_TEG_PWR_ON.pinIndex);
}

static boolean System_PowerOn_3570(boolean isForcePoweron)
{
	boolean B2B_status = FALSE;
	boolean b2b_prst_det1, b2b_prst_det2;
	uint32 b2b_brd_id = 0, dgpu_mode = 0;
	uint8 tegra_pwr_retry = 0;
    GPIO_ALWAYS_PRINTF(("System_PowerOn_3570.\r\n"));
	if(system_state == poweron_state)
	{
		GPIO_ALWAYS_PRINTF(("The board has been powered on.\r\n"));
		GPIO_ALWAYS_PRINTF(("Please run \"poweroff\" first if you want to do a power cycle\r\n"));
		return FALSE;
	}
	// Execute power-off state
	// all our ports are tristate, we set the pins to the needed level for System PowerOff State
	SetOutputs_to_System_PowerOff_State();

	/* SYS power up */
	IfxPort_setPinHigh(VBAT_SYS_PWR_ON.port, VBAT_SYS_PWR_ON.pinIndex);
	if (!isForcePoweron)
	{
		if(Check_PGoodSignals(4,
				(&SYS_5V_PG),
				(&SYS_0V85_SYS_1V0_SYS_1V2_PG),
				(&SYS_3V3_SYS_1V8_SYS_0V9_PG1),
				(&SYS_3V3_SYS_1V8_SYS_1V5_SYS_1V1_SYS_0V9_PG2)) == FALSE)
		{
			GPIO_ALWAYS_PRINTF(("System_PowerOn: Reading PG status: Timeout Error!\n"));
			GPIO_ALWAYS_PRINTF(("SYS_5V_PG, SYS_0V85_SYS_1V0_SYS_1V2_PG,\r\n"));
			GPIO_ALWAYS_PRINTF(("SYS_3V3_SYS_1V8_SYS_0V9_PG1, SYS_3V3_SYS_1V8_SYS_1V5_SYS_1V1_SYS_0V9_PG2\r\n"));
			return FALSE;
		}
	}

	/* Tegra power up */
    IfxPort_setPinHigh(VBAT_TEG_PWR_ON.port, VBAT_TEG_PWR_ON.pinIndex);
    if (!isForcePoweron)
    {
        if(board.pcb_version <= PG2_A01){
            for(tegra_pwr_retry=1;tegra_pwr_retry<=TEGRA_POWER_RETRY;tegra_pwr_retry++){
                if(Check_PGoodSignals(2, (&XA_5V_PG), (&XB_5V_PG))){
                    GPIO_ALWAYS_PRINTF(("Tegra power up \r\n"));
                    break;
                }
                else
                {
                    GPIO_ALWAYS_PRINTF(("Error: Tegra power low, retry: %d\r\n", tegra_pwr_retry));
                    pull_poweron();
                }

            }
            if(tegra_pwr_retry == TEGRA_POWER_RETRY)
            {
                GPIO_ALWAYS_PRINTF(("Tegra power up fail after retry %d times. \r\n", tegra_pwr_retry));
                return FALSE;
            }
        }
        else
        {
            if(!Check_PGoodSignals(2, (&XA_5V_PG), (&XB_5V_PG)))
            {
                GPIO_ALWAYS_PRINTF(("Error: Tegra power low"));
                return FALSE;
            }
        }
    }
    // When PCB_VER>=A03, P3570_NVME_RSTB will output High
    if (board.pcb_version >= PG2_A03)
        SET_GPIOEXP_OUTPUT_HIGH(P3570_NVME_RSTB);
    else
        SET_GPIOEXP_OUTPUT_LOW(P3570_NVME_RSTB);

    //TODO:remove this delay
    //waitTime(50*TimeConst[TIMER_INDEX_1MS]);
	// Power on X1
	IfxPort_setPinHigh(X1_PMIC_POWER_ON_R.port, X1_PMIC_POWER_ON_R.pinIndex);
	SET_GPIOEXP_OUTPUT_HIGH(ARX_SXMA_VBAT_EN_EXP);
	IfxPort_setPinLow(AURIX_XA_RST_N.port, AURIX_XA_RST_N.pinIndex);
	// Power on X2
	IfxPort_setPinHigh(X2_PMIC_POWER_ON_R.port, X2_PMIC_POWER_ON_R.pinIndex);
	SET_GPIOEXP_OUTPUT_HIGH(ARX_SXMB_VBAT_EN_EXP);
	IfxPort_setPinLow(AURIX_XB_RST_N.port, AURIX_XB_RST_N.pinIndex);
	if (!isForcePoweron)
	{
		if(Check_PGoodSignals(1, (&X1_XA_VDD_SOC_XA_VDD_DDR2_XA_VDD_DDRQ_PG)) == FALSE)
		{
			GPIO_ALWAYS_PRINTF(("System_PowerOn: Reading PG status: Timeout Error!\n"));
			GPIO_ALWAYS_PRINTF(("X1_PG\r\n"));
			return FALSE;
		}

		if(Check_PGoodSignals(1, (&X2_XB_VDD_SOC_XB_VDD_DDR2_XB_VDD_DDRQ_PG)) == FALSE)
		{
			GPIO_ALWAYS_PRINTF(("System_PowerOn: Reading PG status: Timeout Error!\n"));
			GPIO_ALWAYS_PRINTF(("X2_PG\r\n"));
			return FALSE;
		}
	}
	//TODO:remove this delay
	waitTime(30*TimeConst[TIMER_INDEX_1MS]);

	/* Aurix2 Power up */
	IfxPort_setPinHigh(P3570_AURIX2_PWR_ON.port, P3570_AURIX2_PWR_ON.pinIndex);
	IfxPort_setPinLow(P3570_AURIX2_RST_N.port, P3570_AURIX2_RST_N.pinIndex);
	/* AURIX2_3V3 = 3.3 V
	 * AURIX2_1V3 = 1.2 V
	 * and then
	 * AURIX2_Status = 1;
	 * */
	uint32 time_out_count = 0;
	uint32 voltage = VadcAutoScan_getCalCulatedVoltage_mV("AURIX2_3V3", 0);
	uint32 min = 3300 * (1 - 0.15);
	uint32 max = 3300 * (1 + 0.15);
	while((voltage < min) || (voltage > max))
	{
		waitTime(1*TimeConst[TIMER_INDEX_1MS]);
		time_out_count++;
		if (time_out_count >= TIMEOUT_CNTR_PG_1MS)
		{
			GPIO_ALWAYS_PRINTF(("Aurix2_3V3 is out of range, value detect: %d mV\n", voltage));
			break;
		}
		voltage = VadcAutoScan_getCalCulatedVoltage_mV("AURIX2_3V3", 0);
	}

	time_out_count = 0;
	voltage = VadcAutoScan_getCalCulatedVoltage_mV("AURIX2_1V3", 0);
	min = 1300 * (1 - 0.15);
	max = 1300 * (1 + 0.15);
	while((voltage < min) || (voltage > max))
	{
		waitTime(1*TimeConst[TIMER_INDEX_1MS]);
		time_out_count++;
		if (time_out_count >= TIMEOUT_CNTR_PG_1MS)
		{
			GPIO_ALWAYS_PRINTF(("Aurix2_1V3 is out of range, value detect: %d mV\n", voltage));
			break;
		}
		voltage = VadcAutoScan_getCalCulatedVoltage_mV("AURIX2_1V3", 0);
	}

	time_out_count = 0;
	voltage = VadcAutoScan_getCalCulatedVoltage_mV("KL30_VBAT2", 0);
	min = 12000 * (1 - 0.15);
	max = 12000 * (1 + 0.15);
    GPIO_ALWAYS_PRINTF(("KL30_VBAT2 = %dmV\n", voltage));
	if((voltage > min) || (voltage < max))
	{
		voltage = VadcAutoScan_getCalCulatedVoltage_mV("KL30_VBAT1", 0);
		GPIO_ALWAYS_PRINTF(("KL30_VBAT2 is on, and check KL30_VBAT1 = %dmV\n", voltage));
		if(voltage < 1000){
			GPIO_ALWAYS_PRINTF(("KL30_VBAT2 is on, and set flag: KL30_VBAT1 = %d mV\n", voltage));
			set_backup_power(BACKUP_POWER);
		}
	}
	/* 2nd Board Power up */
    if(b2b_is_board_dectected())
	{ // if 2nd Board is connected. keep it false for now
	  // Read board ID using ADC
		b2b_brd_id = b2b_get_board_id();

		B2B_status = TRUE;
		switch(b2b_brd_id)
		{
		case BRD_ID_NV_3570:
			/* 3570 boot up individually, No need to control */
                GPIO_ALWAYS_PRINTF(("Detect another NV 3570 is connected\r\n"));
			break;
		case BRD_ID_X86_V3DA:
			/* Need to power up X86 and check its Power good signal */
                GPIO_ALWAYS_PRINTF(("Detect X86 V3DA is connected\r\n"));
			break;
            case BRD_ID_X86_V3NC:
                is_b2b_x86_v3nc = TRUE;
                GPIO_ALWAYS_PRINTF(("Detect X86 V3NC is connected\r\n"));
                break;
		default:
			GPIO_ALWAYS_PRINTF(("Detect board is connected but ID 0x%x is unknown\r\n", b2b_brd_id));
			B2B_status = FALSE;
			break;
		}
        b2b_default_config(b2b_brd_id);
	}
	else
	{
		B2B_status = FALSE;
	}
	waitTime(30*TimeConst[TIMER_INDEX_1MS]);

	// Execute inactive state
	GPIO_DEBUG_PRINTF(("SetOutputs_to_System_Inactive_State\r\n"));
	/* boot strap is set inside the func */
	SetOutputs_to_System_Inactive_State();	//System Inactive State is to be entered when Power to the Tegra have been enabled

	/* QSPI0 connects to Tegra A on DAX (0.7, 0.75, 0.9)
       Initialise QSPI0 as master, with mode 1 default */
	/* QSPI0: XA
	 * QSPI1 SJA1105
	 * QSPI3 AURIX2
	 * QSPI4 XB*/
	TegraQspiMaster_init(0, 1);
	TegraQspiMaster_init(1, 1);
	TegraQspiMaster_init(3, 1);
	TegraQspiMaster_init(4, 1);

	/* Check VMONs IDs */
	if(!Vmon_init())
		GPIO_ALWAYS_PRINTF(("VMON initialization failed !!\n"));

	/* Boot Peripherals */
	// Release resets
	IfxPort_setPinHigh(SJA1105_RSTN.port, SJA1105_RSTN.pinIndex);
	SET_GPIOEXP_OUTPUT_HIGH(ARX_ETHSW_PHY_GRP1_RESET_N_EXP);
	SET_GPIOEXP_OUTPUT_HIGH(ARX_ETHSW_PHY_GRP2_RESET_N_EXP);
	IfxPort_setPinHigh(P3570_LAN7431_RST_N.port, P3570_LAN7431_RST_N.pinIndex);
	SET_GPIOEXP_OUTPUT_HIGH(P3570_PEXSW1_RESET_N_EXP);
	//SET_GPIOEXP_OUTPUT_HIGH(CIM_RST_N_EXP);
	CIM_Power_Control(TRUE);

	// wait TBD ms for PEXSW LTSSM
	waitTime(100*TimeConst[TIMER_INDEX_1MS]);
	IfxPort_setPinHigh(P3570_AURIX_NVME_PEX_RESET_N.port, P3570_AURIX_NVME_PEX_RESET_N.pinIndex);

	/* Boot Tegra */
	//Select DGPU
	if(B2B_status)
	{
		/* P3570 stores the dgpu mode same location as ce mode in 3550
		 * If inforom SYS header is programmed, i2crom_read_ce_mode() returns
		 * the value read from inforom, else it returns default DGPU_MODE = 0
		 */
		/*   DGPU MODE Definition:
		 *   0:  Tegra Control DGPUs
		 *   1:  X86 control DGPUs
		 */
//		dgpu_mode = i2crom_read_ce_mode();
//		GPIO_ALWAYS_PRINTF(("System Powering on in DGPU MODE = %d ...\r\n", dgpu_mode));

        if((b2b_brd_id == BRD_ID_X86_V3DA)||((b2b_brd_id == BRD_ID_X86_V3NC)))
		{
	        // x86 own dgpu
			GPIO_ALWAYS_PRINTF(("PwrON B2B: X86 owns DGPU\r\n"));
			IfxPort_setPinHigh(P3570_ARX_DGPU_MUX_CTRL.port, P3570_ARX_DGPU_MUX_CTRL.pinIndex);
		}
		else
		{ // Xavier own the dgpu
			GPIO_ALWAYS_PRINTF(("PwrON B2B: Xavier owns DGPU\r\n"));
			IfxPort_setPinLow(P3570_ARX_DGPU_MUX_CTRL.port, P3570_ARX_DGPU_MUX_CTRL.pinIndex);
		}
	}
	else
	{
		// Xavier own the dgpu
		GPIO_ALWAYS_PRINTF(("PwrON: Xavier owns DGPU\r\n"));
		IfxPort_setPinLow(P3570_ARX_DGPU_MUX_CTRL.port, P3570_ARX_DGPU_MUX_CTRL.pinIndex);
	}
	// Release Xavier
	IfxPort_setPinHigh(AURIX_XA_RST_N.port, AURIX_XA_RST_N.pinIndex);
	IfxPort_setPinHigh(AURIX_XB_RST_N.port, AURIX_XB_RST_N.pinIndex);

	/* Boot Aurix2 */
	IfxPort_setPinHigh(P3570_AURIX2_RST_N.port, P3570_AURIX2_RST_N.pinIndex);

	// Active state completed
	pmic_a_on = TRUE;
	pmic_b_on = TRUE;
	pmic_c_on = TRUE;
	whole_board_off = FALSE;

	//TODO: do we need this for P3570?
	if(VadcAutoScan_getVoltage_mV(VBAT_SXMA_CHANNEL))
		dgpu_a_on = TRUE;
	else
		dgpu_a_on = FALSE;

	if(VadcAutoScan_getVoltage_mV(VBAT_SXMB_CHANNEL))
		dgpu_b_on = TRUE;
	else
		dgpu_b_on = FALSE;

//    if((B2B_status == FALSE) || (B2B_status == TRUE && b2b_brd_id == BRD_ID_NV_3570)){
#if ETH_AUTO_START_ENABLE == 1
	/* we change it to static ip  10.42.0.146 at start up */
	t_ip_addr ip;
	// main Aurix 10.42.0.146
	ip.ipaddr1 = 10;
	ip.ipaddr2 = 42;
	ip.ipaddr3 = 0;
	ip.ipaddr4 = 146;
	ip.mask1 = 255;
	ip.mask2 = 255;
	ip.mask3 = 255;
	ip.mask4 = 0;
	
	Ethernet_Init(STATIC_IP, &ip);
#endif
//    }

	/* Boot X86 board */
	if(B2B_status)
	{
		// Wait TBD ms for tegras to fully boot
		waitTime(100*TimeConst[TIMER_INDEX_1MS]);
		SET_GPIOEXP_OUTPUT_HIGH(P3570_B2B_TX_SYS_RST_N_EXP);
		GPIO_ALWAYS_PRINTF(("Boot 2nd board\r\n"));
	}
#if 0
	if(b2b_is_board_dectected())
	{
	    b2b_brd_id = b2b_get_board_id();
		switch(b2b_brd_id)
		{
		case BRD_ID_X86:
			GPIO_ALWAYS_PRINTF(("Phy init: Detect X86 is connected\r\n"));
			//
			//AURIX_B2B_GPO_1
			GPIO_ALWAYS_PRINTF(("Phy init: Set RST to Lo\r\n"));
			//setportexp 3 1 4 0
			setOutputState(GPIO_EXP_I2C_BUS, 0x77, 1, 4, 0);
			//wait 100ms
			waitTime(100*TimeConst[TIMER_INDEX_1MS]);
			GPIO_ALWAYS_PRINTF(("~~~~~~ Do b2b phy init ~~~~ \r\n"));
			IfxEth_B2BPhy_88q2112_init();
			GPIO_ALWAYS_PRINTF(("~~~~~~ Finish b2b phy init ~~~~ \r\n"));
			break;
		default:
			GPIO_ALWAYS_PRINTF(("Phy init: Detect board is connected but ID 0x%x \r\n", b2b_brd_id));
			break;
		}
	}
#endif
	GPIO_ALWAYS_PRINTF(("Power on the P3570 system\r\n"));
	system_state = poweron_state;
	return TRUE;
}


/** \brief configure/set all outputs to System Power Off State
 *
 * This function set the outputs to needed state for System Power Off State
 * First we set the output to the needed level then we switch to general output to drive the pin
 * with this we are independent of the actual settings
 */
void SetOutputs_to_System_PowerOff_State(void)
{
    GPIO_ALWAYS_PRINTF(("SetOutputs_to_System_PowerOff_State.\r\n"));
    SetOutputs_to_System_State(P2); 
}

/** \brief configure/set all outputs to System Inactive State
 *
 * This function set the outputs to needed state for System Inactive State
 * System Inactive State is to be entered when Power to the Tegra have been enabled
 * prior to releasing reset
 * Power Sequence:  Poweroff - Enable Power - Inactive - Release Reset
 */
void SetOutputs_to_System_Inactive_State(void)
{
    GPIO_ALWAYS_PRINTF(("SetOutputs_to_System_Inactive_State+.\r\n"));
//	if((board.project == E3550) || (board.project == P3479))
//	{
//		SetOutputs_to_System_Inactive_State_3550_3479();
//	}
//	else if(board.project == P3570)
//	{
		SetOutputs_to_System_State(IN_ACTIVE);
//	}
//	else
//	{
//		GPIO_ALWAYS_PRINTF(("Error: can not detect board type, info ROM content may be broken\r\n"));
//	}
}

boolean System_PowerOff()
{
    GPIO_ALWAYS_PRINTF(("System_PowerOff.\r\n"));        
    return System_PowerOff_3570();
}
boolean System_PowerOn(boolean isForcePoweron)
{
    GPIO_ALWAYS_PRINTF(("System_PowerOn.\r\n"));
    return System_PowerOn_3570(isForcePoweron);
}


boolean Second_Aurix_Boot(void)
{
	A2_QspiSlave_init();
#if ETH_AUTO_START_ENABLE == 1
	t_ip_addr ip;
	// 2nd Aurix 10.42.0.147
	ip.ipaddr1 = 10;
	ip.ipaddr2 = 42;
	ip.ipaddr3 = 0;
	ip.ipaddr4 = 147;
	ip.mask1 = 255;
	ip.mask2 = 255;
	ip.mask3 = 255;
	ip.mask4 = 0;
	Ethernet_Init(STATIC_IP, &ip);
#endif
	return TRUE;
}


static void Monitor_Vdd3v3_Status(void)
{
	/* add vdd 3v3 monitoring */
	volatile uint32 value4 = GetANxRawResult(AN_VDD_3V3);
	volatile uint32 value6 = GetANxRawResult(AN_VDD_3V3_B);
	static uint32 vdd3v3_threshold_adc_value = VDD_3V3_THRESHOLD * 4095 / 5;
	//GPIO_ALWAYS_PRINTF(("value4: %d, value6: %d threshold: %d\n",
	//	value4, value6, vdd3v3_threshold_adc_value));
	if((value4 < vdd3v3_threshold_adc_value) && (value6 < vdd3v3_threshold_adc_value))
	{
#if GPIO_DEBUG_ENABLE == 1
		if(!vdd3v3_off)
		{
			GPIO_DEBUG_PRINTF(("Monitor that vdd3v3 is off\n"));
			GPIO_DEBUG_PRINTF(("value4: %d, value6: %d threshold: %d\n",
				value4, value6, vdd3v3_threshold_adc_value));
		}
#endif
		vdd3v3_off = TRUE;
	}
	else
	{
#if GPIO_DEBUG_ENABLE == 1
		if(vdd3v3_off)
		{
			GPIO_DEBUG_PRINTF(("Monitor that vdd3v3 is on\n"));
			GPIO_DEBUG_PRINTF(("value4: %d, value6: %d threshold: %d\n",
				value4, value6, vdd3v3_threshold_adc_value));
		}
#endif
		vdd3v3_off = FALSE;
	}
}


void Monitor_System_Status(void)
{
	volatile uint32 value16 = GetANxRawResult(AN_VDD_CPU_CHAN);
	volatile uint32 value20 = GetANxRawResult(AN_VDD_CPU_B_CHAN);
	static uint32 threshold_adc_value = VDD_CPU_THRESHOLD * 4095 / 5;
	if(value16 < threshold_adc_value)
	{
		//GPIO_DEBUG_PRINTF(("Monitor that pmic_a is power off, set pmic_a_on FALSE\n"));
		pmic_a_on = FALSE;
	}
	else
	{
		pmic_a_on = TRUE;
	}
	if(value20 < threshold_adc_value)
	{
		//GPIO_DEBUG_PRINTF(("Monitor that pmic_b is power off, set pmic_b_on FALSE\n"));
		pmic_b_on = FALSE;
	}
	else
	{
		pmic_b_on = TRUE;
	}
	if (!pmic_a_on){
		whole_board_off = TRUE;
	}
	else
	{
		whole_board_off = FALSE;
	}

	Monitor_Vdd3v3_Status();
}


static boolean Is_State_Low_Assert(boolean pre_state, boolean cur_state)
{
	if( (pre_state) && (!cur_state))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


void Monitor_Therm_Status(void)
{
	static boolean pre_brd_therm = TRUE;
	static boolean pre_tegra_a_therm = TRUE;
	static boolean pre_tegra_b_therm = TRUE;
	static boolean pre_tegra_c_therm = TRUE;
	static boolean pre_brd_alert = TRUE;
	static boolean pre_brd_alert_2 = TRUE;
	static boolean pre_brd_alert_3 = TRUE;
	static boolean pre_tegra_a_alert = TRUE;
	static boolean pre_tegra_b_alert = TRUE;
	static boolean pre_tegra_c_alert = TRUE;
	static boolean pre_dgpu_a_alert = TRUE;
	static boolean pre_dgpu_b_alert = TRUE;
	/* P3479-A04 */
	static boolean pre_ovr_cpu_thwn = TRUE;
	static boolean pre_ovr_gpu_thwn = TRUE;
	static boolean pre_ovr_soc_thwn = TRUE;
	static boolean pre_ovr_cv_thwn = TRUE;
	/* P3570 */
	static boolean pre_xa_ovr_thwn = TRUE;
	static boolean pre_xb_ovr_thwn = TRUE;

	static uint32 dgpu_alert_threshold = DGPU_THERM_ALERT_THRESHOLD * 4095 / 5;

	IfxPort_Pin *pPort;

    //for VW80000 test
    if(g_poweroff_flag == FALSE){

        waitTime(TimeConst[TIMER_INDEX_1MS]); // this is just a placeholder value for delay

	    /* Therma for board */
	    pPort = &(BRD_THERM_SHDN_N);
	    brd_therm = IfxPort_getPinState(pPort->port, pPort->pinIndex);

	    /* Therma for Xavier A */
	    if(pmic_a_on)
	    {
		    if(board.project == P3570)
		    {
			    pPort = &(P3570_X1_THERM_SHDN_3V3_N);
		    }
//		    else
//		    {
//			    pPort = &(E3550_X1_THERM_SHDN_3V3_N);
//		    }
		    tegra_a_therm = IfxPort_getPinState(pPort->port, pPort->pinIndex);
		    pPort = &(X1_THERM_ALERT_3V3_N);
		    tegra_a_alert = IfxPort_getPinState(pPort->port, pPort->pinIndex);
	    }
	    else
	    {
		    //if pmic a is off tegra_a_therm/alert is N/A
		    tegra_a_therm = TRUE;
		    tegra_a_alert = TRUE;
	    }

	    if((board.project == E3550) || (board.project == P3570))
	    {
		    /* Therma for Xavier B */
		    if(pmic_b_on)
		    {
			    pPort = &(X2_THERM_SHDN_3V3_R_N);
			    tegra_b_therm = IfxPort_getPinState(pPort->port, pPort->pinIndex);
			    pPort = &(X2_THERM_ALERT_3V3_N);
			    tegra_b_alert = IfxPort_getPinState(pPort->port, pPort->pinIndex);
		    }
		    else
		    {
			    //if pmic b is off, tegra_b_therm/alert is N/A
			    tegra_b_therm = TRUE;
			    tegra_b_alert = TRUE;
		    }

		    /* Thermal for DGPU A */
		    if(dgpu_a_on)
		    {
			    IfxPort_setPinLow(ADC_MUX_CTRL_1.port, ADC_MUX_CTRL_1.pinIndex);
			    IfxPort_setPinLow(ADC_MUX_CTRL_2.port, ADC_MUX_CTRL_2.pinIndex);
			    if(GetANxRawResult(DGPU_THERM_ALERT_CHANNEL) < dgpu_alert_threshold)
				    dgpu_a_alert = FALSE;
			    else
				    dgpu_a_alert = TRUE;
		    }
		    else
		    {
			    dgpu_a_alert = TRUE;
		    }

		    /* Thermal for DGPU B */
		    if(dgpu_b_on)
		    {
			    IfxPort_setPinLow(ADC_MUX_CTRL_1.port, ADC_MUX_CTRL_1.pinIndex);
			    IfxPort_setPinHigh(ADC_MUX_CTRL_2.port, ADC_MUX_CTRL_2.pinIndex);
			    if(GetANxRawResult(DGPU_THERM_ALERT_CHANNEL) < dgpu_alert_threshold)
				    dgpu_b_alert = FALSE;
			    else
				    dgpu_b_alert = TRUE;
		    }
		    else
		    {
			    dgpu_b_alert = TRUE;
		    }
	    }
#if 0
	if(board.project == E3550)
	{
		/* Therma for Xavier C/ CVM */
		/* if CVM is present, check therm status */
		if (!READ_GPIOEXP_STATE(E3550_P1_CVM_PRSNT_N_EXP))
		{
			if(pmic_c_on)
			{
				pPort = &(E3550_P1_THERM_SHDN_N);
				tegra_c_therm = IfxPort_getPinState(pPort->port, pPort->pinIndex);
				pPort = &(E3550_P1_THERM_ALERT_3V3_N);
				tegra_c_alert = IfxPort_getPinState(pPort->port, pPort->pinIndex);
			}
			else
			{
				//if pmic c is off, tegra_c_therm/alert is N/A
				tegra_c_therm = TRUE;
				tegra_c_alert = TRUE;
			}
		}
	}
	//OVR thermal
	if(board.project == P3479)
	{
		if(board.pcb_spin == 'A' && board.pcb_version >= 3)
		{
			ovr_cpu_thwn = READ_GPIOEXP_STATE(P3479_OVR_CPU_THWN_N_EXP);
			ovr_gpu_thwn = READ_GPIOEXP_STATE(P3479_OVR_GPU_THWN_N_EXP);
			ovr_soc_thwn = READ_GPIOEXP_STATE(P3479_OVR_SOC_THWN_N_EXP);
			ovr_cv_thwn = READ_GPIOEXP_STATE(P3479_OVR_CV_THWN_N_EXP);
		}
#endif
//	else if (board.project == P3570)
	    {
		    xa_ovr_thwn = READ_GPIOEXP_STATE(P3570_XA_OVR_THWN_N_EXP);
		    xb_ovr_thwn = READ_GPIOEXP_STATE(P3570_XB_OVR_THWN_N_EXP);
	    }

	    /* board alert */
	    if(!whole_board_off)
	    {
		    if(board.project == P3570)
		    {
			    brd_alert = READ_GPIOEXP_STATE(P3570_BRD_THERM_ALERT_N_EXP_1);
			    brd_alert_2 = READ_GPIOEXP_STATE(P3570_BRD_THERM_ALERT_N_EXP_2);
			    brd_alert_3 = READ_GPIOEXP_STATE(P3570_BRD_THERM_ALERT_N_EXP_3);

			    clearTempsensors_status();
		    }
		    else
		    {
			    pPort = &(E3550_BRD_THERM_ALERT_N);
			    brd_alert = IfxPort_getPinState(pPort->port, pPort->pinIndex);
			    clearTempsensors_status();
		    }
	    }
	    else
	    {
		    //if both pmic is off, poweroff state requirement dictates that i2c is off, therefore Alert cannot be serviced and Alert pin status is N/A.
		    brd_alert = TRUE;
		    brd_alert_2 = TRUE;
		    brd_alert_3 = TRUE;
	    }


	    /* print out the therm status only once when it's asserted */
	    /* board thermal */
	    if(Is_State_Low_Assert(pre_brd_therm, brd_therm))
	    {
		    display_thermal_status = TRUE;
		    AsclinShellInterface_qualDisplay(TRUE);
	    }
	    pre_brd_therm = brd_therm;

	    if(Is_State_Low_Assert(pre_brd_alert, brd_alert))
	    {
		    display_thermal_status = TRUE;
		    AsclinShellInterface_qualDisplay(TRUE);
	    }
	    pre_brd_alert = brd_alert;
	    if(board.project == P3570)
   	    {
		    if(Is_State_Low_Assert(pre_brd_alert_2, brd_alert_2))
		    {
			    display_thermal_status = TRUE;
			    AsclinShellInterface_qualDisplay(TRUE);
		    }
		    pre_brd_alert_2 = brd_alert_2;

		    if(Is_State_Low_Assert(pre_brd_alert_3, brd_alert_3))
		    {
			    display_thermal_status = TRUE;
			    AsclinShellInterface_qualDisplay(TRUE);
		    }
		    pre_brd_alert_3 = brd_alert_3;
	    }

	    /* XA thermal */
	    if(Is_State_Low_Assert(pre_tegra_a_therm, tegra_a_therm))
	    {
		    display_thermal_status = TRUE;
		    AsclinShellInterface_qualDisplay(TRUE);
	    }
	    pre_tegra_a_therm = tegra_a_therm;
	    if(Is_State_Low_Assert(pre_tegra_a_alert, tegra_a_alert))
	    {
		    display_thermal_status = TRUE;
		    /* print the message at once if qual log is enable */
		    AsclinShellInterface_qualDisplay(TRUE);
	    }
	    pre_tegra_a_alert = tegra_a_alert;

	    // XB, dgpu
	    if((board.project == E3550) || (board.project == P3570))
   	    {
		    if(Is_State_Low_Assert(pre_tegra_b_therm, tegra_b_therm))
		    {
			    display_thermal_status = TRUE;
			    AsclinShellInterface_qualDisplay(TRUE);
		    }
		    pre_tegra_b_therm = tegra_b_therm;

	  	    if(Is_State_Low_Assert(pre_tegra_b_alert, tegra_b_alert))
		    {
			    display_thermal_status = TRUE;
			    AsclinShellInterface_qualDisplay(TRUE);
		    }
		    pre_tegra_b_alert = tegra_b_alert;

		    if(Is_State_Low_Assert(pre_dgpu_a_alert, dgpu_a_alert))
		    {
			    display_thermal_status = TRUE;
			    AsclinShellInterface_qualDisplay(TRUE);
		    }
		    pre_dgpu_a_alert = dgpu_a_alert;

		    if(Is_State_Low_Assert(pre_dgpu_b_alert, dgpu_b_alert))
		    {
			    display_thermal_status = TRUE;
			    AsclinShellInterface_qualDisplay(TRUE);
		    }
		    pre_dgpu_b_alert = dgpu_b_alert;
	    }
#if 0
	if(board.project == E3550)
	{ // CVM/P2888
		/* if CVM is present, Trigger therm status if CVM alert/shdn assert  */
		if (!READ_GPIOEXP_STATE(E3550_P1_CVM_PRSNT_N_EXP))
		{
			if(Is_State_Low_Assert(pre_tegra_c_therm, tegra_c_therm))
			{
				display_thermal_status = TRUE;
				AsclinShellInterface_qualDisplay(TRUE);
			}
			pre_tegra_c_therm = tegra_c_therm;

			if(Is_State_Low_Assert(pre_tegra_c_alert, tegra_c_alert))
			{
				display_thermal_status = TRUE;
				AsclinShellInterface_qualDisplay(TRUE);
			}
			pre_tegra_c_alert = tegra_c_alert;
		}
	}
	//OVR
	if(board.project == P3479 && board.pcb_spin == 'A' && board.pcb_version >= 3)
	{
		if(Is_State_Low_Assert(pre_ovr_cpu_thwn, ovr_cpu_thwn))
		{
			display_thermal_status = TRUE;
			AsclinShellInterface_qualDisplay(TRUE);
		}
		if(Is_State_Low_Assert(pre_ovr_gpu_thwn, ovr_gpu_thwn))
		{
			display_thermal_status = TRUE;
			AsclinShellInterface_qualDisplay(TRUE);
		}
		if(Is_State_Low_Assert(pre_ovr_soc_thwn, ovr_soc_thwn))
		{
			display_thermal_status = TRUE;
			AsclinShellInterface_qualDisplay(TRUE);
		}
		if(Is_State_Low_Assert(pre_ovr_cv_thwn, ovr_cv_thwn))
		{
			display_thermal_status = TRUE;
			AsclinShellInterface_qualDisplay(TRUE);
		}
		pre_ovr_cpu_thwn = ovr_cpu_thwn;
		pre_ovr_gpu_thwn = ovr_gpu_thwn;
		pre_ovr_soc_thwn = ovr_soc_thwn;
		pre_ovr_cv_thwn = ovr_cv_thwn;
	}
#endif
//	else if (board.project == P3570)
	    {
		    if(Is_State_Low_Assert(pre_xa_ovr_thwn, xa_ovr_thwn))
		    {
			    display_thermal_status = TRUE;
			    AsclinShellInterface_qualDisplay(TRUE);
		    }
		    pre_xa_ovr_thwn = xa_ovr_thwn;
		    if(Is_State_Low_Assert(pre_xb_ovr_thwn, xb_ovr_thwn))
		    {
			    display_thermal_status = TRUE;
			    AsclinShellInterface_qualDisplay(TRUE);
		    }
		    pre_xb_ovr_thwn = xb_ovr_thwn;
	    }
    }
}


/*
#define AURIX2_THERM_ALERT_1  IfxPort_P33_0
#define AURIX2_THERM_ALERT_2  IfxPort_P33_1
#define AURIX2_THERM_ALERT_3  IfxPort_P33_2
#define AURIX2_THERM_ALERT_4  IfxPort_P33_5
#define AURIX2_THERM_ALERT_5  IfxPort_P33_6
*/
void Monitor_Aurix2_Therm_Status(void)
{
    static boolean pre_brd_alert = TRUE;
    static boolean pre_brd_alert_2 = TRUE;
    static boolean pre_brd_alert_3 = TRUE;
    static boolean pre_brd_alert_4 = TRUE;
    static boolean pre_brd_alert_5 = TRUE;

    IfxPort_Pin *pPort;

    /* board alert */
    if(!whole_board_off)
    {
        pPort = &(AURIX2_THERM_ALERT_1);
        brd_alert = IfxPort_getPinState(pPort->port, pPort->pinIndex);
        pPort = &(AURIX2_THERM_ALERT_2);
        brd_alert_2 = IfxPort_getPinState(pPort->port, pPort->pinIndex);
        pPort = &(AURIX2_THERM_ALERT_3);
        brd_alert_3 = IfxPort_getPinState(pPort->port, pPort->pinIndex);
        pPort = &(AURIX2_THERM_ALERT_4);
        brd_alert_4 = IfxPort_getPinState(pPort->port, pPort->pinIndex);
        pPort = &(AURIX2_THERM_ALERT_5);
        brd_alert_5 = IfxPort_getPinState(pPort->port, pPort->pinIndex);

        clearAurx2Tempsensors_status();
    }
    else
    {
        //if both pmic is off, poweroff state requirement dictates that i2c is off, therefore Alert cannot be serviced and Alert pin status is N/A.
        brd_alert = TRUE;
        brd_alert_2 = TRUE;
        brd_alert_3 = TRUE;
        brd_alert_4 = TRUE;
        brd_alert_5 = TRUE;
    }

    if(Is_State_Low_Assert(pre_brd_alert, brd_alert))
    {
        display_thermal_status = TRUE;
        AsclinShellInterface_qualDisplay(TRUE);
    }
    pre_brd_alert = brd_alert;

    if(Is_State_Low_Assert(pre_brd_alert_2, brd_alert_2))
    {
        display_thermal_status = TRUE;
        AsclinShellInterface_qualDisplay(TRUE);
    }
    pre_brd_alert_2 = brd_alert_2;

    if(Is_State_Low_Assert(pre_brd_alert_3, brd_alert_3))
    {
        display_thermal_status = TRUE;
        AsclinShellInterface_qualDisplay(TRUE);
    }
    pre_brd_alert_3 = brd_alert_3;

    if(Is_State_Low_Assert(pre_brd_alert_4, brd_alert_4))
    {
        display_thermal_status = TRUE;
        AsclinShellInterface_qualDisplay(TRUE);
    }
    pre_brd_alert_3 = brd_alert_3;

    if(Is_State_Low_Assert(pre_brd_alert_5, brd_alert_5))
    {
        display_thermal_status = TRUE;
        AsclinShellInterface_qualDisplay(TRUE);
    }
    pre_brd_alert_3 = brd_alert_3;

}



void execute_thermal_shutdown(void)
{
	/* need to shutdown the power if therm happen, and print the message even qual mode is not enable */

	/* Note: System_PowerOff() powers off complete board.
	 * Individual tegra power control is not supported.
	 */
	if (!brd_therm)
	{
		GPIO_ALWAYS_PRINTF(("brd_therm We're going to power off the system due to thermal Alert/Shutdown\r\n"));
		display_thermal_status = TRUE;
        //No HW
		Print_Thermal_Status(&g_AsclinShellInterface.stdIf.asc);
        //I2C1 + I2C0
		System_PowerOff();
        sem_wait_until_get(SEM_SPI_2, __LINE__, __func__);
        waitTime(500*TimeConst[TIMER_INDEX_1MS]);
        aurix_reset();
	}
	if (!tegra_a_therm)
	{
		GPIO_ALWAYS_PRINTF(("tegra_a_therm We're going to power off the system due to thermal Alert/Shutdown\r\n"));
		display_thermal_status = TRUE;
		Print_Thermal_Status(&g_AsclinShellInterface.stdIf.asc);
        //I2C1 + I2C0
		System_PowerOff();
        sem_wait_until_get(SEM_SPI_2, __LINE__, __func__);
        waitTime(500*TimeConst[TIMER_INDEX_1MS]); 
        aurix_reset();
	}
	if((board.project == E3550) || (board.project == P3570))
	{
		if (!tegra_b_therm)
		{
			GPIO_ALWAYS_PRINTF(("We're going to power off the system due to thermal Alert/Shutdown\r\n"));
			display_thermal_status = TRUE;
			Print_Thermal_Status(&g_AsclinShellInterface.stdIf.asc);
            //I2C1 + I2C0
			System_PowerOff();
            sem_wait_until_get(SEM_SPI_2, __LINE__, __func__);
            waitTime(500*TimeConst[TIMER_INDEX_1MS]);
            aurix_reset();
		}
	}
}


// Toggle the bidirectional level shifter
// ONLY needed for E3550
void toggle_levelShifter()
{
	// WAR for CVM Level shifter
	IfxPort_setPinModeOutput(E3550_P1_THERM_ALERT_3V3_N.port, E3550_P1_THERM_ALERT_3V3_N.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
	IfxPort_setPinHigh(E3550_P1_THERM_ALERT_3V3_N.port, E3550_P1_THERM_ALERT_3V3_N.pinIndex);
	IfxPort_setPinModeInput(E3550_P1_THERM_ALERT_3V3_N.port, E3550_P1_THERM_ALERT_3V3_N.pinIndex, IfxPort_InputMode_noPullDevice);

	IfxPort_setPinModeOutput(E3550_P1_SOC_PWR_REQ_3V3.port, E3550_P1_SOC_PWR_REQ_3V3.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
	IfxPort_setPinHigh(E3550_P1_SOC_PWR_REQ_3V3.port, E3550_P1_SOC_PWR_REQ_3V3.pinIndex);
	IfxPort_setPinModeInput(E3550_P1_SOC_PWR_REQ_3V3.port, E3550_P1_SOC_PWR_REQ_3V3.pinIndex, IfxPort_InputMode_noPullDevice);

	IfxPort_setPinModeOutput(E3550_P1_SYS_SAFE_3V3_N.port, E3550_P1_SYS_SAFE_3V3_N.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
	IfxPort_setPinHigh(E3550_P1_SYS_SAFE_3V3_N.port, E3550_P1_SYS_SAFE_3V3_N.pinIndex);
	IfxPort_setPinModeInput(E3550_P1_SYS_SAFE_3V3_N.port, E3550_P1_SYS_SAFE_3V3_N.pinIndex, IfxPort_InputMode_noPullDevice);
}

boolean is_PowerOff()
{
	if(system_state == poweroff_state)
		return TRUE;
	return FALSE;
}

//There're no SKU strapping in B0X
void Initial_Sku_Setting(void)
{

}

// endof #if (DRIVE_AX_A00 == 1) || (DRIVE_AX_A01 == 1)
#endif //#if PROJECT == PROJECT_DRIVE_AX
