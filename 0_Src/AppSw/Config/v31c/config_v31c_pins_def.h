#ifndef CONFIG_V31C_PINS_DEF
#define CONFIG_V31C_PINS_DEF

#define TC397B
//
#include <_PinMap/IfxAsclin_PinMap.h>
#include <_PinMap/IfxEray_PinMap.h>
#ifdef TC397B
#include <_PinMap/IfxGeth_PinMap.h>
#else
#include <_PinMap/IfxEth_PinMap.h>
#endif
#include <_PinMap/IfxI2c_PinMap.h>
#include <_PinMap/IfxCan_PinMap.h>
#include <_PinMap/IfxPort_PinMap.h>
#include <_PinMap/IfxQspi_PinMap.h>
#include <gpioexp_pinmap.h>

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
/* postfix naming format:
 * N: means it's low active
 * EXP: means it's GPIO IO Expander */


/*******************************/
/************ CAN **************/
/*******************************/
//HARNESS_A
#define CAN_PHY1_TXD            IfxCan_TXD03_P00_2_OUT
#define CAN_PHY1_RXD            IfxCan_RXD03A_P00_3_IN
//HARNESS_B
#define CAN_PHY3_TXD            IfxCan_TXD23_P14_9_OUT
#define CAN_PHY3_RXD            IfxCan_RXD23A_P14_10_IN
//HARNESS_C
#define CANR1_TXD               IfxCan_TXD01_P01_3_OUT
#define CANR1_RXD               IfxCan_RXD01C_P01_4_IN
//HARNESS_D
#define CANR0_TXD               IfxCan_TXD13_P14_6_OUT
#define CANR0_RXD               IfxCan_RXD13A_P14_7_IN
//HARNESS_E
#define CAN0_TXD                IfxCan_TXD11_P00_4_OUT
#define CAN0_RXD                IfxCan_RXD11B_P00_5_IN
//HARNESS_F
#define CAN_PHY2_TXD            IfxCan_TXD10_P00_0_OUT
#define CAN_PHY2_RXD            IfxCan_RXD10A_P00_1_IN

#define CAN_PHY1_EN_EXP         IoExpPort_A75_P00
#define CAN_PHY1_ERR_N          IfxPort_P00_9      //low_acitve
/* standby pin, set CAN mo  dule to low power mode when active */
#define CAN_PHY1_STB_N_EXP      IoExpPort_A75_P01  //low_acitve

//CAN_PHY0_STB = CAN_PHYCDEF_STB
#define CAN_PHY0_STB_EXP        IoExpPort_A75_P04  //high_active
#define CAN_PHY3_EN_EXP         IoExpPort_A75_P02
#define CAN_PHY3_ERR_N          IfxPort_P14_8      //low_acitve
#define CAN_PHY3_STB_N_EXP      IoExpPort_A75_P03  //low_acitve

//control on X1/X2/P CAN
#define AURIX_XA_CAN_EN_N_EXP   IoExpPort_A75_P05
#define AURIX_XB_CAN_EN_N_EXP   IoExpPort_A75_P06
#define AURIX_P1_CAN_EN_N_EXP   IoExpPort_A75_P07

/*******************************/
/******** Detection ************/
/*******************************/


/*******************************/
/********* Ethernet ************/
/*******************************/
/* MII pins definition */
/** =============================
 * ===define in GethBasicDemo.c===
   ==============================*/
//#define PHY_TXD3                    IfxGeth_TXD3_P11_0_OUT
//#define PHY_TXD2                    IfxGeth_TXD2_P11_1_OUT
//#define PHY_TXD1                    IfxGeth_TXD1_P11_2_OUT
//#define PHY_TXD0                    IfxGeth_TXD0_P11_3_OUT
//#define SNN_AURIX_ETH_RGMII_TXCLK   IfxPort_P11_4
//#define PHY_TXEN                    IfxGeth_TXEN_P11_6_OUT
//#define PHY_RGMII_RXD3              IfxGeth_RXD3A_P11_7_IN
//#define PHY_RGMII_RXD2              IfxGeth_RXD2A_P11_8_IN
//#define PHY_RGMII_RXD1              IfxGeth_RXD1A_P11_9_IN
//#define PHY_RGMII_RXD0              IfxGeth_RXD0A_P11_10_IN
//#define PHY_RGMII_RX_CTL            IfxGeth_RXDVA_P11_11_IN


/*******************************/
/************ FAN **************/
/*******************************/
/* PUSH FAN */
/* high: Tegra control (good state)
 * Low:  MAX PWM (bad state) */
#define AURIX_X1_FANCTL             IfxPort_P02_9
#define AURIX_X2_FANCTL             IfxPort_P32_2

#define FAN1_GTM_TACH_PIN           IfxGtm_TIM3_4_P02_10_IN//IfxGtm_TIM3_4_TIN117_P02_10_IN
#define FAN2_GTM_TACH_PIN           IfxGtm_TIM0_7_P02_11_IN//IfxGtm_TIM0_7_TIN118_P02_11_IN

/* PULL FAN (B01) */
/* high: Tegra control (good state)
 * Low:  MAX PWM (bad state) */
/* PWM Overwrite mechanism:
 * Aurix PWM duty High: Pull FAN PWM voltage will be driven low by Tegra PWM pin
 *     (when Tegra is in reset and PWM pin is driven low by pull-down resister)
 * Aurix PWM duty Low: Pull FAN PWM voltage will be driven high
 * Hence if circuit works, Aurix PWM is reversed
 * */
#define ARX_X1_FAN2_PWM_OVERRIDE    IfxPort_P15_0
#define ARX_X2_FAN2_PWM_OVERRIDE    IfxPort_P15_1

#define PULL_FAN_TACH_SEL_AB_EXP    IoExpPort_A77_P10


/*******************************/
/*********** FlexRay ***********/
/*******************************/
#define FR_A_TXD                    IfxEray0_TXDA_P02_0_OUT
#define FR_A_RXD                    IfxEray0_RXDA2_P02_1_IN
#define FR_A_TXEN                   IfxEray0_TXENA_P02_4_OUT
#define FR_A_RXEN                   IfxPort_P02_6
#define FR_A_ERR_N                  IfxPort_P02_8 //low_active
#define FR_A_STB_N_EXP              IoExpPort_A75_P10 //low_active
#define FR_A_EN                     IfxPort_P02_10

#define FR_B_TXD                    IfxEray0_TXDB_P02_2_OUT
#define FR_B_RXD                    IfxEray0_RXDB2_P02_3_IN
#define FR_B_TXEN                   IfxEray0_TXENB_P02_5_OUT
#define FR_B_RXEN                   IfxPort_P02_7
#define FR_B_ERR_N                  IfxPort_P22_5    //low_active
#define FR_B_STB_N_EXP              IoExpPort_A75_P11   //low_active
#define FR_B_EN                     IfxPort_P10_4
//#define FLEXRAY_ENABLE_N            IfxPort_P15_8    //low_active
#define FLEXRAY_A_WAKE_N            IfxPort_P00_12

/*******************************/
/************* I2C *************/
/*******************************/
#define AURIX_I2C1_SDA              IfxI2c1_SDA_P11_13_INOUT
#define AURIX_I2C1_SCL              IfxI2c1_SCL_P11_14_INOUT

#define AURIX_I2C_SCL               IfxI2c0_SCL_P13_1_INOUT
#define AURIX_I2C_SDA               IfxI2c0_SDA_P13_2_INOUT

/*******************************/
/********** Interrupt **********/
/*******************************/
//I/O Expander IRQ
#define XA_VMON_INT_N               IfxPort_P01_5
#define HIA_POC_LOAD_SWITCH_INT1    IfxPort_P32_5
#define HIA_POC_LOAD_SWITCH_INT2    IfxPort_P32_6
#define CIM_PWR_INT_N               IfxPort_P32_7
#define INT_TLF35584                IfxPort_P33_4

/*******************************/
/************* MDIO ************/
/*******************************/
/* MDIO/MDC */
#define PHY_MDC                     IfxGeth_MDC_P12_0_OUT
#define PHY_MDIO                    IfxGeth_MDIO_P12_1_INOUT

/* MDIO control logic */



/*******************************/
/************* MISC ************/
/*******************************/
// AURIX Led pins of version B00
#define AURIX_LED1                  IfxPort_P20_3
#define AURIX_LED2                  IfxPort_P20_1

#define AURIX_WDT_TLF35584          IfxPort_P33_3
#define TLF35584_ERR                IfxPort_P33_8

/***********************************/
/********** PEX Control ************/
/***********************************/


/***********************************/
/************* PGood ***************/
/***********************************/
#define SPA2_X1_PG                                     IfxPort_P00_0
#define SPA2_VDD_SOC_PG                                IfxPort_P00_1
#define SPA2_VDD_DDR2_PG                               IfxPort_P00_2
#define SPA2_VDD_DDRQ_PG                               IfxPort_P00_3
#define SPA2_VDD_CPU_PG                                IfxPort_P00_4
#define SPA2_VDD_GPU_PG                                IfxPort_P00_5
#define SPA2_VDD_CV_PG                                 IfxPort_P00_6
#define SPA2_XA_5V_PG                                  IfxPort_P00_7
#define SPA2_XA_PREREG_PG                              IfxPort_P00_8
#define SPA2_XA_RTC_PG                                 IfxPort_P00_9
#if SPA2_HIA == 1 /*20190924,sync with source code*/
#define SPA2_NW2_POWER_GOOD                            IfxPort_P15_7
#else
#define SPA2_NW1_POWER_GOOD                            IfxPort_P15_7
#endif
#define HIA_POC_PWR_GOOD                               IfxPort_P22_5
#define SPA2_HIA_CIM_PG                                IfxPort_P22_6

#define SGA_BAT1_FAULT                                 IfxPort_P02_0
#define SGA_BAT2_FAULT                                 IfxPort_P02_1
#define HIB_BAT1_FAULT                                 IfxPort_P02_2
#define HIB_BAT2_FAULT                                 IfxPort_P02_3
#define HIA_BAT1_FAULT                                 IfxPort_P02_4
#define HIA_BAT2_FAULT                                 IfxPort_P02_5
#define HIA_BAT_OFF                                    IfxPort_P02_6


/***********************************/
/************* Power ***************/
/***********************************/
#define VBAT_SYS_PWR_ON             IfxPort_P13_0
#define X1_PMU_SHDN_REQ_3V3         IfxPort_P23_1
#define AURIX_X1_WAKE               IfxPort_P23_6
#define VBAT_TEG_PWR_ON             IfxPort_P33_0
#define X1_PMIC_POWER_ON_R          IfxPort_P33_2
#define XA_SOC_PWR_EN_3V3           IfxPort_P23_2

#define HIA_CIM_PWR_ON				IfxPort_P22_4
#define HIA_POC_LOAD_SWITCH_EN1     IfxPort_P32_3
#define HIA_POC_LOAD_SWITCH_EN2     IfxPort_P32_4


/************************************/
/************* Recovery *************/
/************************************/
#define X1_FORCE_RCVR_R_N           IfxPort_P33_1


/************************************/
/************** Reset ***************/
/************************************/
#if SPA2_HIB == 1
#define SJA1105_TDL_TOGGLE              IfxPort_P22_9/* nvidia 5190 drop5 */
#endif
#define SJA1105_RSTN                    IfxPort_P11_15
#define AURIX_XA_RST_N                  IfxPort_P33_10
#define HIA_PHY_RESET                   IfxPort_P15_6


/************************************/
/*********** Safe State *************/
/************************************/
#define X1_SAFE_STATE_3V3_R         IfxPort_P23_5
#define TLF35584_SS1                IfxPort_P15_8

/************************************/
/************** SPI *****************/
/************************************/
#if SPA2_HIA == 1 /*20190924,sync with source code*/
#define SJA1105_CSN                 IfxQspi1_SLSO0_P20_8_OUT //IfxPort_P20_8
#define SJA1105_1_CSN               IfxQspi1_SLSO1_P20_9_OUT //IfxPort_P20_8
#else
#define SJA1105_CSN                 IfxQspi1_SLSO1_P20_9_OUT //IfxQspi1_SLSO0_P20_8_OUT
#define SJA1105_1_CSN               IfxQspi1_SLSO0_P20_8_OUT //IfxQspi1_SLSO1_P20_9_OUT
#endif
#define SJA1105_SDO                 IfxQspi1_MRSTA_P10_1_IN  //MISO
#define SJA1105_SCK                 IfxQspi1_SCLK_P10_2_OUT
#define SJA1105_SDI                 IfxQspi1_MTSR_P10_3_OUT  //MOSI

// Xavier 1 SPI
#define AURIX_X1_SPI_SCKR           IfxQspi0_SCLK_P20_11_OUT
#define AURIX_X1_SPI_MISOR          IfxQspi0_MRSTA_P20_12_IN
#define AURIX_X1_SPI_CSR            IfxQspi0_SLSO2_P20_13_OUT
#define AURIX_X1_SPI_MOSIR          IfxQspi0_MTSR_P20_14_OUT

//SPI Handshake
#define AURIX_X1_SPI_HS_1           IfxPort_P20_6
#define AURIX_X1_SPI_HS_2           IfxPort_P20_10
#define AURIX_X2_SPI_HS_2           IfxPort_P20_8

#define AURIX_TLF_SPI_CS            IfxQspi2_SLSO0_P15_2_OUT
#define AURIX_TLF_SPI_MOSI          IfxQspi2_MTSR_P15_5_OUT
#define AURIX_TLF_SPI_MISO          IfxQspi2_MRSTA_P15_4_IN
#define AURIX_TLF_SPI_SCK           IfxQspi2_SCLK_P15_3_OUT
#define TLF_USE_CHIPSELECT          AURIX_TLF_SPI_CS

/* Update section for SPI Between HIA and HIB */
#if SPA2_HIA == 1
#define AURIX_HIB_HIA_SPI_SCK         	IfxQspi4_SCLKB_P22_3_IN
#define AURIX_HIB_HIA_SPI_MOSI        	IfxQspi4_MTSRB_P22_0_IN
#define AURIX_HIB_HIA_SPI_MISO        	IfxQspi4_MRST_P22_1_OUT
#define AURIX_HIB_HIA_SPI_CS          	IfxQspi4_SLSIB_P22_2_IN
#else
#define AURIX_HIA_HIB_SPI_SCK         	IfxQspi4_SCLK_P22_3_OUT
#define AURIX_HIA_HIB_SPI_MOSI        	IfxQspi4_MTSR_P22_0_OUT
#define AURIX_HIA_HIB_SPI_MISO        	IfxQspi4_MRSTB_P22_1_IN
#define AURIX_HIA_HIB_SPI_CS          	IfxQspi4_SLSO3_P22_2_OUT
#endif

//HIA HIB gpio handshake
#define AURIX_HS_2                      IfxPort_P20_7

/************************************/
/************* Strap ****************/
/************************************/

/************************************/
/************* Thermal **************/
/************************************/
#define X1_THERM_ALERT_3V3_N        IfxPort_P23_4
#define X1_THERM_SHDN_3V3_N         IfxPort_P23_3

#define BRD_THERM_ALERT             IfxPort_P33_6
#define BRD_THERM_SHDN              IfxPort_P33_5

#define SPA2_OVR_CPU_THWN_N         IfxPort_P22_7
#define SPA2_OVR_GPU_THWN_N         IfxPort_P22_8
#define SPA2_OVR_SOC_THWN_N         IfxPort_P22_9
#define SPA2_OVR_CV_THWN_N          IfxPort_P22_10

/************************************/
/************** UART ***************/
/************************************/
#define AURIX_DBG_UART_TX           IfxAsclin0_TX_P14_0_OUT
#define AURIX_DBG_UART_RX           IfxAsclin0_RXA_P14_1_IN

#define AURIX_TEGRA_UART_TX         IfxAsclin1_TX_P15_0_OUT
#define AURIX_TEGRA_UART_RX         IfxAsclin1_RXA_P15_1_IN

/************************************/
/************** CIM *****************/
/************************************/
/* Note: These are CIM specific pins. While there is only
 * one type of CIM now, there may be more in future. These
 * definitions for FRSYNC/PWDN will need update dynamically
 * after reading CIM ID from inforom */
//V31C did not support CIM FRSYNC/PWDN


#endif /** #ifndef CONFIG_V31C_PINS_DEF */
