/*
 * Gpio_ctrl_3570.c
 *
 *  Created on: Mar 4, 2019
 *      Author: bradf
 */

#include "Port/Std/IfxPort.h"
#include "Configuration.h"
#include "Gpio_ctrl_3570.h"
#include "gpioexp.h"
#include "I2cRom.h"
#include "internal_fw_debug.h"
#include "Mcmcan.h"

#define OUT_PP    IfxPort_OutputMode_pushPull
#define OUT_OD    IfxPort_OutputMode_openDrain
#define IN_NP     IfxPort_Mode_inputNoPullDevice
#define IN_PD     IfxPort_Mode_inputPullDown
#define IN_PU     IfxPort_Mode_inputPullUp

#define GPIO_CTRL_ALWAYS_PRINTF(x) DEBUG_PRINTF_ALWAYS(x)

/*
 * TRUE: set P3570_AURIX2_RCVR_N to low at power on
 * FALSE: set P3570_AURIX2_RCVR_N to high at power off
 * bug 2568557
 * */
boolean is_aurix2_recy_on = FALSE;


typedef enum
{
	ARX = 0,
	EXP,
}e_pin_type;

typedef enum
{
	IN = 0,
	OUT,
}e_direction;

typedef enum
{
	L = 0,  //LOW
	H,   // HIGH
	X,   //UN-DEFINED VALUE, ignore it.
}e_value;

#define N_A   0xFF

typedef struct
{
	e_pin_type type;
	void* pin_p;  // IfxPort_Pin* or IoExpPort_Pin*
	e_direction dir;
	e_value values[MAX_STATE]; // P2, INACTIVE, P0 and SC7
	IfxPort_OutputMode output_mode;
	IfxPort_Mode mode;
}t_gpio_config;


static t_gpio_config GPIO_CONFIG_3570[]=
{//  TYPE     PIN                 DIR P2 INAC P0 SC7 OUT-MODE INPUT-MODE
	// U47, SLAVE 0x74
	{EXP, &P3570_B2B_PG_OUT,      OUT, {L, L, L, L}, N_A, N_A},
	// U248, SLAVE 0x75
	{EXP, &CAN_PHY1_EN_EXP,       OUT, {L, L, X, X}, N_A, N_A},
	{EXP, &CAN_PHY1_STB_N_EXP,    OUT, {L, L, X, X}, N_A, N_A},
	{EXP, &CAN_PHY3_EN_EXP,       OUT, {L, L, X, X}, N_A, N_A},
	{EXP, &CAN_PHY3_STB_N_EXP,    OUT, {L, L, X, X}, N_A, N_A},
	{EXP, &CAN_PHY0_STB_EXP,      OUT, {H, H, X, X}, N_A, N_A},
	{EXP, &AURIX_XA_CAN_EN_N_EXP, OUT, {L, H, X, L}, N_A, N_A},
	{EXP, &AURIX_XB_CAN_EN_N_EXP, OUT, {L, H, X, L}, N_A, N_A},
	{EXP, &FR_A_STB_N_EXP,        OUT, {L, H, X, X}, N_A, N_A},
	{EXP, &FR_B_STB_N_EXP,        OUT, {L, H, X, X}, N_A, N_A},
    {EXP, &P3570_NVME_RSTB,       OUT, {L, H, X, X}, N_A, N_A},
	// U261, SLAVE 0x76
	{EXP, &X1_BSTRP0_EXP,         OUT, {L, H, X, L}, N_A, N_A},
	{EXP, &X1_BSTRP1_EXP,         OUT, {L, L, X, L}, N_A, N_A},
	{EXP, &X1_BSTRP2_EXP,         OUT, {L, L, X, L}, N_A, N_A},
	{EXP, &P3570_B2B_TX_PWR_EN_EXP,OUT,{L, L, H, H}, N_A, N_A},
	{EXP, &X2_BSTRP0_EXP,         OUT, {L, H, X, L}, N_A, N_A},
	{EXP, &X2_BSTRP1_EXP,         OUT, {L, L, X, L}, N_A, N_A},
	{EXP, &X2_BSTRP2_EXP,         OUT, {L, L, X, L}, N_A, N_A},
	{EXP, &P3570_B2B_TX_SYS_RST_N_EXP, OUT, {L, L, H, L}, N_A, N_A},
	{EXP, &CIM_RST_N_EXP,         OUT, {L, L, X, L}, N_A, N_A},
	{EXP, &P3570_X1_FAN2_PWM_OVERRIDE_EXP, OUT, {X, X, X, X}, N_A, N_A},
	{EXP, &P3570_X2_FAN2_PWM_OVERRIDE_EXP, OUT, {X, X, X, X}, N_A, N_A},
	{EXP, &P3570_PEXSW1_VQPS_OTP_EN_EXP,   OUT, {L, H, X, H}, N_A, N_A},
	{EXP, &ARX_SXMA_VBAT_EN_EXP,  OUT, {L, H, H, L}, N_A, N_A},
	{EXP, &ARX_SXMB_VBAT_EN_EXP,  OUT, {L, H, H, L}, N_A, N_A},
	// U332, SLAVE 0x77
	{EXP, &ARX_ETH_2112_GRP1_MDC_CTRL_EXP, OUT, {L, L, X, L}, N_A, N_A},
	{EXP, &ARX_ETH_2112_GRP2_MDC_CTRL_EXP, OUT, {L, H, H, L}, N_A, N_A},
	{EXP, &ARX_ETH_Q5050_MDC_CTRL_EXP,     OUT, {L, H, H, L}, N_A, N_A},
	{EXP, &ARX_ETHSW_PHY_GRP1_RESET_N_EXP, OUT, {L, L, H, L}, N_A, N_A},
	{EXP, &ARX_ETHSW_PHY_GRP2_RESET_N_EXP, OUT, {L, L, H, L}, N_A, N_A},
	{EXP, &ARX_ETH_EA6321_1_MDC_CTRL_EXP,  OUT, {L, L, L, L}, N_A, N_A},
	{EXP, &PULL_FAN_TACH_SEL_AB_EXP, OUT, {X, X, X, X}, N_A, N_A},
	{EXP, &ARX_PEXSW1_BOOT_REC_EXP, OUT, {L, H, H, L}, N_A, N_A},
	{EXP, &ARX_PEXSW2_BOOT_REC_EXP, OUT, {L, H, H, L}, N_A, N_A},
	{EXP, &P3570_B2B_GPO_1_EXP, OUT, {L, L, X, L}, N_A, N_A},
	{EXP, &P3570_PEXSW1_RESET_N_EXP, OUT, {L, L, H, L}, N_A, N_A},
	{EXP, &P3570_B2B_GPO_2_EXP, OUT, {L, L, X, L}, N_A, N_A},
	//P00
	{ARX, (IfxPort_Pin*)&CAN_PHY2_TXD.pin,         OUT, {H, H, X, X}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&CAN_PHY1_TXD.pin,         OUT, {H, H, X, X}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&CAN0_TXD.pin,             OUT, {H, H, X, X}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&P3570_ARX_DGPU_MUX_CTRL,  OUT, {L, L, X, L}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&P3570_ARX_PM40052_BOOT_INDEX1, OUT, {L, L, L, L}, OUT_PP, N_A},
	//P01
	{ARX, (IfxPort_Pin*)&CANR1_TXD.pin,            OUT, {H, H, X, X}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&P3570_AURIX_3_IOEXP_INT,  OUT, {L, X, X, L}, OUT_PP, N_A},
	//P02
	{ARX, (IfxPort_Pin*)&FR_A_TXD.pin,  OUT, {L, L, X, X}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&FR_B_TXD.pin,  OUT, {L, L, X, X}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&FR_A_TXEN.pin, OUT, {H, H, X, X}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&FR_B_TXEN.pin, OUT, {H, H, X, X}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&P3570_ARX_PM40052_BOOT_INDEX0,  OUT, {L, L, L, L}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&FR_A_EN,   OUT, {L, L, X, X}, OUT_PP, N_A},
	//P10
	{ARX, (IfxPort_Pin*)&SJA1105_CSN.pin,  OUT, {L, H, X, L}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&SJA1105_SDO.pin,  IN,  {X, X, X, X}, N_A, IN_PD},
	{ARX, (IfxPort_Pin*)&SJA1105_SCK.pin,  OUT, {L, L, X, L}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&SJA1105_SDI.pin,  OUT, {L, L, X, L}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&FR_B_EN,      OUT, {L, L, X, X}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&QSPI3_SPI_SSR.pin,    OUT, {L, H, X, L}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&QSPI3_SPI_MOSIR.pin,  OUT, {L, L, X, L}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&QSPI3_SPI_MISOR.pin,  IN,  {X, X, X, X}, N_A, IN_PD},
	{ARX, (IfxPort_Pin*)&QSPI3_SPI_SCKR.pin,   OUT, {L, L, X, L}, OUT_PP, N_A},

	//P11
	{ARX, (IfxPort_Pin*)&PHY_TXD3.pin,  OUT, {L, L, X, L}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&PHY_TXD2.pin,  OUT, {L, L, X, L}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&PHY_TXD1.pin,  OUT, {L, L, X, L}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&PHY_TXD0.pin,  OUT, {L, L, X, L}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&PHY_RGMII_TX_CLKR.pin, IN, {X, X, X, X}, N_A, IN_PD},
	{ARX, (IfxPort_Pin*)&PHY_TXEN.pin,  OUT, {L, L, X, L}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&PHY_RGMII_RXD3.pin,  IN, {X, X, X, X}, N_A, IN_PD},
	{ARX, (IfxPort_Pin*)&PHY_RGMII_RXD2.pin,  IN, {X, X, X, X}, N_A, IN_PD},
	{ARX, (IfxPort_Pin*)&PHY_RGMII_RXD1.pin,  IN, {X, X, X, X}, N_A, IN_PD},
	{ARX, (IfxPort_Pin*)&PHY_RGMII_RXD0.pin,  IN, {X, X, X, X}, N_A, IN_PD},
	{ARX, (IfxPort_Pin*)&PHY_RGMII_RX_CTL.pin,IN, {X, X, X, X}, N_A, IN_PD},
	{ARX, (IfxPort_Pin*)&PHY_REFCLK.pin,      IN, {X, X, X, X}, N_A, IN_PD},
	{ARX, (IfxPort_Pin*)&SJA1105_RSTN,  OUT, {L, L, H, L}, OUT_PP, N_A},

	//P12
	{ARX, (IfxPort_Pin*)&PHY_MDC.pin,  OUT, {L, L, X, L}, OUT_OD, N_A},
	/* We need to set MDIO open-drain, high in inactive, to work with the level-shiffter */
	{ARX, (IfxPort_Pin*)&PHY_MDIO.pin, OUT, {L, H, X, L}, OUT_OD, N_A},
	//P13
	{ARX, (IfxPort_Pin*)&VBAT_SYS_PWR_ON,  OUT, {L, H, H, H}, OUT_PP, N_A},
	//P14
	{ARX, (IfxPort_Pin*)&CANR0_TXD.pin,    OUT, {H, H, X, X}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&CAN_PHY3_TXD.pin, OUT, {H, H, X, X}, OUT_PP, N_A},
	//P15
	//TODO: is it correct to set UART pin to low?
	{ARX, (IfxPort_Pin*)&P3570_AURIX_B2B_UART_TX.pin,  OUT, {L, L, X, X}, OUT_PP, N_A},
	//{ARX, (IfxPort_Pin*)&P3570_AURIX2_RCVR_N,          OUT, {H, H, X, L}, OUT_OD, N_A},
	{ARX, (IfxPort_Pin*)&X2_THERM_ALERT_3V3_N,         IN,  {X, X, X, X}, N_A, IN_PD},
	{ARX, (IfxPort_Pin*)&FLEXRAY_ENABLE_N,             OUT, {L, H, X, X}, OUT_PP, N_A},
	//P20
	//{ARX, (IfxPort_Pin*)&P3570_AURIX1_HSSL_SYSCLK_OUT_R.pin,  OUT, {L, X, X, X}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&AURIX_X1_SPI_HS_1,  OUT, {L, X, X, X}, OUT_OD, N_A},
	{ARX, (IfxPort_Pin*)&AURIX_X1_SPI_HS_2,  IN,  {X, X, X, X}, N_A, IN_NP},
	{ARX, (IfxPort_Pin*)&AURIX_X2_SPI_HS_2,  IN,  {X, X, X, X}, N_A, IN_NP},
	{ARX, (IfxPort_Pin*)&AURIX_X2_SPI_HS_1,  OUT, {L, X, X, X}, OUT_OD, N_A},
	{ARX, (IfxPort_Pin*)&AURIX_X1_SPI_SCKR.pin, OUT, {L, L, X, L}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&AURIX_X1_SPI_MISOR.pin,IN,  {X, X, X, X}, N_A, IN_PD},
	{ARX, (IfxPort_Pin*)&AURIX_X1_SPI_CSR.pin,  OUT, {L, H, X, L}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&AURIX_X1_SPI_MOSIR.pin,OUT, {L, L, X, L}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&AURIX_LED2, OUT,         {H, X, X, X}, OUT_PP, N_A},
	//P21
	//"P3570_X1_THERM_SHDN_3V3_N" is USB Hub reset in E3550, comment it for now to verify in 3550
	//TODO: uncomment it for P3570 bring up
	//{ARX, (IfxPort_Pin*)&P3570_X1_THERM_SHDN_3V3_N,  IN, {X, X, X, X}, N_A, IN_PD},
	{ARX, (IfxPort_Pin*)&X1_THERM_ALERT_3V3_N,       IN, {X, X, X, X}, N_A, IN_PD},
	//{ARX, (IfxPort_Pin*)&P3570_AURIX1_HSSL_RXDN_R.pin,  OUT, {X, X, X, X}, OUT_PP, N_A},
	//{ARX, (IfxPort_Pin*)&P3570_AURIX1_HSSL_RXDP_R.pin,  OUT, {X, X, X, X}, OUT_PP, N_A},
	//{ARX, (IfxPort_Pin*)&P3570_AURIX1_HSSL_TXDN_R.pin,  OUT, {X, X, X, X}, OUT_PP, N_A},
	//{ARX, (IfxPort_Pin*)&P3570_AURIX1_HSSL_TXDP_R.pin,  OUT, {X, X, X, X}, OUT_PP, N_A},
	//P22

	{ARX, (IfxPort_Pin*)&AURIX_X2_SPI_MOSIR.pin,OUT, {L, L, X, L}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&AURIX_X2_SPI_MISOR.pin,IN,  {X, X, X, X}, N_A, IN_PD},
	{ARX, (IfxPort_Pin*)&AURIX_X2_SPI_SSR.pin,  OUT, {L, H, X, L}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&AURIX_X2_SPI_SCKR.pin, OUT, {L, L, X, L}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&ARX_ETHSW_PHY_MDIO_CTRL, OUT, {L, L, X, L}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&AHRN_GPO0,  OUT, {L, L, X, X}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&ADC_MUX_CTRL_1,  OUT, {L, L, X, X}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&ADC_MUX_CTRL_2,  OUT, {L, L, X, X}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&P3570_AURIX_PEXSW_ROM_WP_EN,  OUT, {L, X, X, H}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&P3570_AURIX_PCIECON_DRV_EN_N,  OUT, {L, L, X, X}, OUT_OD, N_A},
	//P23
	{ARX, (IfxPort_Pin*)&P3570_AURIX_PCIECON_PEX_RESET_N,  OUT, {L, H, H, L}, OUT_OD, N_A},
	{ARX, (IfxPort_Pin*)&AURIX_X1_WAKE,  OUT, {L, H, H, L}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&AURIX_X2_WAKE,  OUT, {L, H, H, L}, OUT_PP, N_A},
	//P32
	{ARX, (IfxPort_Pin*)&AURIX_X1_FANCTL,  OUT, {L, H, X, L}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&AURIX_X2_FANCTL,  OUT, {L, H, X, L}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&P3570_AURIX_NVME_PEX_RESET_N,  OUT, {L, H, H, L}, OUT_OD, N_A},
	//P33
	{ARX, (IfxPort_Pin*)&VBAT_TEG_PWR_ON,  OUT, {L, H, H, H}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&X1_FORCE_RCVR_R_N,  OUT, {L, H, X, L}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&X1_PMIC_POWER_ON_R,  OUT, {L, H, H, H}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&BRD_THERM_SHDN_N,  IN, {X, X, X, X}, N_A, IN_NP},
	{ARX, (IfxPort_Pin*)&X2_FORCE_RCVR_R_N,  OUT, {L, H, X, L}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&X2_PMIC_POWER_ON_R,  OUT, {L, H, H, H}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&AURIX_XB_RST_N,  OUT, {L, L, H, H}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&AURIX_XA_RST_N,  OUT, {L, L, H, H}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&P3570_LAN7431_RST_N,  OUT, {L, L, H, L}, OUT_OD, N_A},
	{ARX, (IfxPort_Pin*)&P3570_AURIX1_TO_AURIX2_HS,  IN, {X, X, X, X}, N_A, IN_NP},
	{ARX, (IfxPort_Pin*)&P3570_AURIX2_TO_AURIX1_HS,  IN, {X, X, X, X}, N_A, IN_NP},
	{ARX, (IfxPort_Pin*)&P3570_AURIX2_PWR_ON,  OUT, {L, H, H, L}, OUT_PP, N_A},
	{ARX, (IfxPort_Pin*)&P3570_AURIX2_RST_N,  OUT, {L, L, H, L}, OUT_OD, N_A},
	//P34
	{ARX, (IfxPort_Pin*)&X2_THERM_SHDN_3V3_R_N,  IN, {X, X, X, X}, N_A, IN_PD},
};

static void extra_config_p2_state(void)
{
	if(is_aurix2_recy_on)
	{
		IfxPort_setPinLow(P3570_AURIX2_RCVR_N.port, P3570_AURIX2_RCVR_N.pinIndex);
		GPIO_CTRL_ALWAYS_PRINTF(("Aurix2 recovery pin low \r\n"));
	}
	else
	{
		IfxPort_setPinHigh(P3570_AURIX2_RCVR_N.port, P3570_AURIX2_RCVR_N.pinIndex);
		GPIO_CTRL_ALWAYS_PRINTF(("Aurix2 recovery pin high \r\n"));
	}
	IfxPort_setPinModeOutput(P3570_AURIX2_RCVR_N.port, P3570_AURIX2_RCVR_N.pinIndex,
			IfxPort_OutputMode_openDrain, IfxPort_OutputIdx_general);

	/* CAN, keep the same as 3550 */
	can_set_phy_standby_state(PHY1);
	can_set_phy_standby_state(PHY3);
	can_set_phy_standby_state(PHY0);
	can_set_phy_standby_state(X1_PHY);
	can_set_phy_standby_state(X2_PHY);
}

static void extra_config_inactive_state(void)
{
	uint8 boot_media_ta = 1;
	uint8 boot_media_tb = 1;
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
        if(board.aurix_index == MAIN_AURIX){
		SET_GPIOEXP_OUTPUT_HIGH(X1_BSTRP0_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X1_BSTRP1_EXP);
		SET_GPIOEXP_OUTPUT_HIGH(X1_BSTRP2_EXP);
        }
		GPIO_CTRL_ALWAYS_PRINTF(("Configured BootStraps for X1 as UFS\n\r"));
		break;
	case 1:
		//strap value = 1
        if(board.aurix_index == MAIN_AURIX){
		SET_GPIOEXP_OUTPUT_HIGH(X1_BSTRP0_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X1_BSTRP1_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X1_BSTRP2_EXP);
        }
		GPIO_CTRL_ALWAYS_PRINTF(("Configured BootStraps for X1 as QSPI\n\r"));
		break;
	case 2:
		//strap value = 0
        if(board.aurix_index == MAIN_AURIX){
		SET_GPIOEXP_OUTPUT_LOW(X1_BSTRP0_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X1_BSTRP1_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X1_BSTRP2_EXP);
        }
		GPIO_CTRL_ALWAYS_PRINTF(("Configured BootStraps for X1 as EMMC\n\r"));
		break;
	default:
		//strap value = 1
        if(board.aurix_index == MAIN_AURIX){
		SET_GPIOEXP_OUTPUT_HIGH(X1_BSTRP0_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X1_BSTRP1_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X1_BSTRP2_EXP);
        }
		GPIO_CTRL_ALWAYS_PRINTF(("Configured BootStraps for X1 as QSPI\n\r"));
		break;
	}
	switch(boot_media_tb)
	{
	case 0:
		//strap value = 5
        if(board.aurix_index == MAIN_AURIX){
		SET_GPIOEXP_OUTPUT_HIGH(X2_BSTRP0_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X2_BSTRP1_EXP);
		SET_GPIOEXP_OUTPUT_HIGH(X2_BSTRP2_EXP);
        }
		GPIO_CTRL_ALWAYS_PRINTF(("Configured BootStraps for X2 as UFS\n\r"));
		break;
	case 1:
		//strap value = 1
        if(board.aurix_index == MAIN_AURIX){
		SET_GPIOEXP_OUTPUT_HIGH(X2_BSTRP0_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X2_BSTRP1_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X2_BSTRP2_EXP);
        }
		GPIO_CTRL_ALWAYS_PRINTF(("Configured BootStraps for X2 as QSPI\n\r"));
		break;
	case 2:
		//strap value = 0
        if(board.aurix_index == MAIN_AURIX){
		SET_GPIOEXP_OUTPUT_LOW(X2_BSTRP0_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X2_BSTRP1_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X2_BSTRP2_EXP);
        }
		GPIO_CTRL_ALWAYS_PRINTF(("Configured BootStraps for X2 as EMMC\n\r"));
		break;
	default:
		//strap value = 1
        if(board.aurix_index == MAIN_AURIX){
		SET_GPIOEXP_OUTPUT_HIGH(X2_BSTRP0_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X2_BSTRP1_EXP);
		SET_GPIOEXP_OUTPUT_LOW(X2_BSTRP2_EXP);
        }
		GPIO_CTRL_ALWAYS_PRINTF(("Configured BootStraps for X2 as QSPI\n\r"));
		break;
	}

	/* CAN, keep the same as 3550
	 * in 3550 it enable the X1/X2 CAN PHY after powering up */
	can_enable_phy(X1_PHY);
	can_enable_phy(X2_PHY);
}

void SetOutputs_to_System_State(t_system_state state)
{
	uint32 i;
	uint32 pin_num = sizeof(GPIO_CONFIG_3570)/sizeof(t_gpio_config);
	IoExpPort_Pin* exp_pin_p;
	IfxPort_Pin* arx_pin_p;
	for(i=0; i<pin_num; i++)
	{
        if(board.aurix_index == MAIN_AURIX){
		if(GPIO_CONFIG_3570[i].type == EXP)
		{
			if(GPIO_CONFIG_3570[i].dir == OUT)
			{
				exp_pin_p = (IoExpPort_Pin*)GPIO_CONFIG_3570[i].pin_p;
				if(GPIO_CONFIG_3570[i].values[state] == L)
				{
					SET_GPIOEXP_OUTPUT_LOW((*exp_pin_p));
				}
				else if(GPIO_CONFIG_3570[i].values[state] == H)
				{
					SET_GPIOEXP_OUTPUT_HIGH((*exp_pin_p));
				}
				/*initial P3570_NVME_RSTB for PG2_A03 start*/
                if ((board.pcb_version >= PG2_A03) && (exp_pin_p == &P3570_NVME_RSTB))
                {
                    if (state == IN_ACTIVE)
                        SET_GPIOEXP_OUTPUT_HIGH((*exp_pin_p));
                    else if (state == P2)
                        SET_GPIOEXP_OUTPUT_LOW((*exp_pin_p));
                }
                /*initial P3570_NVME_RSTB for PG2_A03 end*/
				SET_GPIOEXP_CONFIG_OUTPUT((*exp_pin_p));
			}
			else{}; // IO EXPANDER IO is input by default
		}
        }
        if(GPIO_CONFIG_3570[i].type == ARX)
		{
			arx_pin_p = (IfxPort_Pin*)GPIO_CONFIG_3570[i].pin_p;
			if(GPIO_CONFIG_3570[i].dir == OUT)
			{
				if(GPIO_CONFIG_3570[i].values[state] == L)
				{
					IfxPort_setPinLow(arx_pin_p->port, arx_pin_p->pinIndex);
				}
				else if(GPIO_CONFIG_3570[i].values[state] == H)
				{
					IfxPort_setPinHigh(arx_pin_p->port, arx_pin_p->pinIndex);
				}
				IfxPort_setPinModeOutput(arx_pin_p->port, arx_pin_p->pinIndex,
						GPIO_CONFIG_3570[i].output_mode, IfxPort_OutputIdx_general);
			}
			else
			{
				IfxPort_setPinModeInput(arx_pin_p->port, arx_pin_p->pinIndex, GPIO_CONFIG_3570[i].mode);
			}
		}
	}
	if(state == P2)
		extra_config_p2_state();
	if(state == IN_ACTIVE)
		extra_config_inactive_state();
}

