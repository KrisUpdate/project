#ifndef CONFIG_V3NA_PINS_DEF
#define CONFIG_V3NA_PINS_DEF
#include <_PinMap/IfxAsclin_PinMap.h>
#include <_PinMap/IfxEray_PinMap.h>
#include <_PinMap/IfxGeth_PinMap.h>
#include <_PinMap/IfxI2c_PinMap.h>
#include <_PinMap/IfxCan_PinMap.h>
#include <_PinMap/IfxPort_PinMap.h>
#include <_PinMap/IfxQspi_PinMap.h>

#include <gpioexp_pinmap.h>

#define TC397B

//From AX.h
//new in DAX, control on Tegra/CVM CAN

// U271, Bus1, SLAVE 0x74
#define XA_5V_PG                                       IoExpPort_A74_P00
#define XA_PREREG_PG                                   IoExpPort_A74_P01
#define X1_XA_VDD_SOC_XA_VDD_DDR2_XA_VDD_DDRQ_PG       IoExpPort_A74_P02
#define XA_VDD_CPU_XA_VDD_GPU_XA_VDD_CV_PG             IoExpPort_A74_P03
#define SYS_5V_PG                                      IoExpPort_A74_P10  //use on A02 or before
#define SYS_3V3_2_EN                                   IoExpPort_A74_P10
#define SYS_0V85_SYS_1V2_PG                            IoExpPort_A74_P11
#define SYS_3V3_SYS_2V1_PG1                            IoExpPort_A74_P12
#define CIM_PWR_GD                                     IoExpPort_A74_P16

// U272, Bus1, SLAVE 0x75
/* PH1,PH3 = TLE9252VSK  -> Normal:(EN=H,STB=H) ; Standby:(EN=L,STB=L) */
/* PH0,XA  = TLE9251VLE  -> Normal:(STB=L)      ; Standby:(STB=H) */
/* FlexRay = TLE9221SX   -> Normal:(EN=H,STB=H) ; Standby:(EN=L,STB=L) */
#define CAN_PHY1_EN_EXP                   IoExpPort_A75_P00
#define CAN_PHY1_STB_N_EXP                IoExpPort_A75_P01
#define CAN_PHY3_EN_EXP                   IoExpPort_A75_P02
#define CAN_PHY3_STB_N_EXP                IoExpPort_A75_P03
#define CAN_PHY0_STB_EXP                  IoExpPort_A75_P04  //CAN_PHY0_STB = CAN_PHYCDEF_STB
#define AURIX_XA_CAN_EN_N_EXP             IoExpPort_A75_P05
#define FR_A_STB_N_EXP                    IoExpPort_A75_P10
#define FR_B_STB_N_EXP                    IoExpPort_A75_P11
/* Valid for P3479-a03 or later only */
#define P3479_OVR_CPU_THWN_N_EXP          IoExpPort_A75_P14
#define P3479_OVR_GPU_THWN_N_EXP          IoExpPort_A75_P15
#define P3479_OVR_SOC_THWN_N_EXP          IoExpPort_A75_P16
#define P3479_OVR_CV_THWN_N_EXP           IoExpPort_A75_P17

// U2731, Bus1, SLAVE 0x76
#define X1_BSTRP0_EXP                     IoExpPort_A76_P00
#define X1_BSTRP1_EXP                     IoExpPort_A76_P01
#define X1_BSTRP2_EXP                     IoExpPort_A76_P02
#define CIM_RST_N_EXP                     IoExpPort_A76_P10
#define CIM_PWR_ON_EXP                    IoExpPort_A76_P11
#define CIM_PRSNT_N_EXP                   IoExpPort_A76_P12
#define CIM_SEAT_GOOD_EXP                 IoExpPort_A76_P13

// U270, Bus1, SLAVE 0x77
#define TPS7A1601_PG_3V3                  IoExpPort_A77_P00  //use on EVT2, no use before EVT2
#define ARX_ETHSW_PHY_GRP1_RESET_N_EXP    IoExpPort_A77_P04
#define ARX_ETHSW_PHY_GRP2_RESET_N_EXP    IoExpPort_A77_P05
#define P3479_LAN7431_RESET_N_EXP         IoExpPort_A77_P16  //P3479-A03
#define ARX_ETH_2112_GRP1_MDC_CTRL_EXP    IoExpPort_A77_P01  //TODO: No use on P3479! Modify ethernet driver.
#define ARX_ETH_2112_GRP2_MDC_CTRL_EXP    IoExpPort_A77_P02  //TODO: No use on P3479! Modify ethernet driver.
#define ARX_ETH_Q5050_MDC_CTRL_EXP        IoExpPort_A77_P03  //TODO: No use on P3479! Modify ethernet driver.
#define ARX_ETH_EA6321_1_MDC_CTRL_EXP     IoExpPort_A77_P06  //TODO: No use on P3479! Modify ethernet driver.

// U293, Bus0, SLAVE 0x74, CIM Ioexpander
#define SV_FRSYNC_EN_N                IoExpPort_CIM_A74_P00
#define CV_FRSYNC_EN_N                IoExpPort_CIM_A74_P01
#define LRC_FRSYNC_EN_N               IoExpPort_CIM_A74_P02
#define LRR_FRSYNC_EN_N               IoExpPort_CIM_A74_P03  //TODO: No use on P3479! Modify CIM driver.
#define SV_PWRDN_N                    IoExpPort_CIM_A74_P04
#define CV_PWRDN_N                    IoExpPort_CIM_A74_P05
#define LRC_PWRDN_N                   IoExpPort_CIM_A74_P06
#define LRR_PWRDN_N                   IoExpPort_CIM_A74_P07  //TODO: No use on P3479! Modify CIM driver.
#define SV_LSW_INT_N                  IoExpPort_CIM_A74_P10
#define CV_LSW_INT_N                  IoExpPort_CIM_A74_P11
#define LRC_LSW_INT_N                 IoExpPort_CIM_A74_P12
#define TEGRA_SEL                     IoExpPort_CIM_A74_P16  //TODO: No use on P3479! Modify CIM driver.
#define FRSYNC_SEL                    IoExpPort_CIM_A74_P17  //TODO: No use on P3479! Modify CIM driver.

/*******************************************************/
/***************** Aurix IOs P00 ***********************/
/*******************************************************/
//Note P00.0 in circuit
//HARNESS_F
#define CAN_PHY2_TXD            IfxCan_TXD10_P00_0_OUT
#define CAN_PHY2_RXD            IfxCan_RXD10A_P00_1_IN
//HARNESS_A
#define CAN_PHY1_TXD            IfxCan_TXD03_P00_2_OUT
#define CAN_PHY1_RXD            IfxCan_RXD03A_P00_3_IN
//HARNESS_E
#define CAN0_TXD                IfxCan_TXD11_P00_4_OUT
#define CAN0_RXD                IfxCan_RXD11B_P00_5_IN
#define LS1028_PS_3V3_EN                 IfxPort_P00_6
#define LS1028_RESET_REQ                 IfxPort_P00_7  //used on preEVT, EVT1
#define AURIX_LS1028_CAN_EN_N            IfxPort_P00_8
#define CAN_PHY1_ERR_N                   IfxPort_P00_9  //low_acitve
#define I2C_GPIO_EXP_IRQ                 IfxPort_P00_10
#define AURIX_S32G_CAN_EN_N              IfxPort_P00_11
#define FLEXRAY_A_WAKE_N                 IfxPort_P00_12

/*******************************************************/
/***************** Aurix IOs P01 ***********************/
/*******************************************************/
//HARNESS_C
#define CANR1_TXD               IfxCan_TXD01_P01_3_OUT
#define CANR1_RXD               IfxCan_RXD01C_P01_4_IN
#define XA_VMON_INT_N                    IfxPort_P01_5
#define XA_RTC_PG                        IfxPort_P01_6
#define I2C_GPIO_EXP2_IRQ                IfxPort_P01_7

/*******************************************************/
/***************** Aurix IOs P02 ***********************/
/*******************************************************/
#define FR_A_TXD                 IfxEray0_TXDA_P02_0_OUT
#define FR_A_RXD                 IfxEray0_RXDA2_P02_1_IN
#define FR_B_TXD                 IfxEray0_TXDB_P02_2_OUT
#define FR_B_RXD                 IfxEray0_RXDB2_P02_3_IN
#define FR_A_TXEN                IfxEray0_TXENA_P02_4_OUT
#define FR_B_TXEN                IfxEray0_TXENB_P02_5_OUT
#define FR_A_RXEN                          IfxPort_P02_6
#define FR_B_RXEN                          IfxPort_P02_7
#define FR_A_ERR_N                         IfxPort_P02_8
#define AURIX_FAN3_TACH                    IfxPort_P02_9
#define FR_A_EN                            IfxPort_P02_10
#define AURIX_FAN2_TACH                    IfxPort_P02_11

/*******************************************************/
/***************** Aurix IOs P10 ***********************/
/*******************************************************/
#define SJA1105_CSN              IfxQspi1_SLSO10_P10_0_OUT
#define SJA1105_SDO              IfxQspi1_MRSTA_P10_1_IN  //MISO
#define SJA1105_SCK              IfxQspi1_SCLK_P10_2_OUT
#define SJA1105_SDI              IfxQspi1_MTSR_P10_3_OUT  //MOSI
#define QSPI3_CSN                IfxQspi3_SLSO8_P10_5_OUT
#define QSPI3_SDI                IfxQspi3_MTSR_P10_6_OUT  //MOSI
#define QSPI3_SDO                IfxQspi3_MRSTB_P10_7_IN  //MISO
#define QSPI3_SCLK               IfxQspi3_SCLK_P10_8_OUT

#define FR_B_EN                            IfxPort_P10_4
#define EVT2_LS1028_RESET_REQ              IfxPort_P10_7  //use after EVT2
#define PVT_LS1028_RESET_REQ              IfxPort_P20_3  //use after PVT

/*******************************************************/
/***************** Aurix IOs P11 ***********************/
/*******************************************************/
/* TC397B MII pins definition */
#define PHY_TXD3                    IfxGeth_TXD3_P11_0_OUT
#define PHY_TXD2                    IfxGeth_TXD2_P11_1_OUT
#define PHY_TXD1                    IfxGeth_TXD1_P11_2_OUT
#define PHY_TXD0                    IfxGeth_TXD0_P11_3_OUT
//Not used:  TXERR for A-Step device in MII mode, but we're not using TXERR
#define AURIX_ETH_RGMII_TXCLK       IfxPort_P11_4
//AURIX_ETH_MII_TXCLK_RGMII_REFCLK, TXCLK Input for A-Step device in MII mode
#define PHY_RGMII_TX_CLKR           IfxGeth_TXCLKA_P11_5_IN
#define PHY_TXEN                    IfxGeth_TXEN_P11_6_OUT
#define PHY_RGMII_RXD3              IfxGeth_RXD3A_P11_7_IN
#define PHY_RGMII_RXD2              IfxGeth_RXD2A_P11_8_IN
#define PHY_RGMII_RXD1              IfxGeth_RXD1A_P11_9_IN
#define PHY_RGMII_RXD0              IfxGeth_RXD0A_P11_10_IN
#define PHY_RGMII_RX_CTL            IfxGeth_RXDVA_P11_11_IN  // AURIX_ETH_MII_RX_CTL
#define PHY_REFCLK                  IfxGeth_RXCLKA_P11_12_IN // RX CLK
#define AURIX_I2C1_SDA              IfxI2c1_SDA_P11_13_INOUT
#define AURIX_I2C1_SCL              IfxI2c1_SCL_P11_14_INOUT
#define SJA1105_RSTN                IfxPort_P11_15
#define TPS7A1601_EN                IfxPort_P23_3

/*******************************************************/
/***************** Aurix IOs P12 ***********************/
/*******************************************************/
//TODO: No use on P3479! Modify Eth_Devices driver.
#define PHY_MDC                     IfxGeth_MDC_P12_0_OUT
#define PHY_MDIO                    IfxGeth_MDIO_P12_1_INOUT

/*******************************************************/
/***************** Aurix IOs P13 ***********************/
/*******************************************************/
#define VBAT_SYS_PWR_ON             IfxPort_P13_0
#define AURIX_I2C_SCL               IfxI2c0_SCL_P13_1_INOUT
#define AURIX_I2C_SDA               IfxI2c0_SDA_P13_2_INOUT
#define LIN1_SLEEP                  IfxPort_P13_3

/*******************************************************/
/***************** Aurix IOs P14 ***********************/
/*******************************************************/
#define AURIX_DBG_UART_TX           IfxAsclin0_TX_P14_0_OUT
#define AURIX_DBG_UART_RX           IfxAsclin0_RXA_P14_1_IN
//HARNESS_D
#define CANR0_TXD                   IfxCan_TXD13_P14_6_OUT
#define CANR0_RXD                   IfxCan_RXD13A_P14_7_IN
#define CAN_PHY3_ERR_N              IfxPort_P14_8
//HARNESS_B
#define CAN_PHY3_TXD                IfxCan_TXD23_P14_9_OUT
#define CAN_PHY3_RXD                IfxCan_RXD23A_P14_10_IN

/*******************************************************/
/***************** Aurix IOs P15 ***********************/
/*******************************************************/
/* PULL FAN (B01) */
/* high: Tegra control (good state)
 * Low:  MAX PWM (bad state) */
/* PWM Overwrite mechanism:
 * Aurix PWM duty High: Pull FAN PWM voltage will be driven low by Tegra PWM pin
 *     (when Tegra is in reset and PWM pin is driven low by pull-down resister)
 * Aurix PWM duty Low: Pull FAN PWM voltage will be driven high
 * Hence if circuit works, Aurix PWM is reversed
 * */
#define AURIX_FAN2_CTRL             IfxPort_P15_0
#define CAN_PHY7_RXD                IfxCan_RXD02A_P15_1_IN
#define AURIX_TLF_SPI_CS            IfxQspi2_SLSO0_P15_2_OUT
#define AURIX_TLF_SPI_SCK           IfxQspi2_SCLK_P15_3_OUT
#define AURIX_TLF_SPI_MISO          IfxQspi2_MRSTA_P15_4_IN
#define AURIX_TLF_SPI_MOSI          IfxQspi2_MTSR_P15_5_OUT
#define FLEXRAY_ENABLE_N            IfxPort_P15_8
#define TLF_USE_CHIPSELECT          AURIX_TLF_SPI_CS

/*******************************************************/
/***************** Aurix IOs P20 ***********************/
/*******************************************************/
#define BRD_THERM_ALERT_N           IfxPort_P20_0
#define AURIX_LED2                  IfxPort_P20_1
#define AURIX_TESTMODE_IN           IfxPort_P20_2
#define DS160OR410_1_READ_EN_N      IfxPort_P20_3
#define DS160OR410_1_ALL_DONE_N     IfxPort_P20_6
#define CAN_PHY8_RXD                IfxCan_RXD00B_P20_7_IN
#define CAN_PHY8_TXD                IfxCan_TXD00_P20_8_OUT
#define AURIX_X1_SPI_HS_1           IfxPort_P20_9
#define AURIX_X1_SPI_HS_2           IfxPort_P20_10
// Xavier 1 SPI
#define AURIX_X1_SPI_SCKR           IfxQspi0_SCLK_P20_11_OUT
#define AURIX_X1_SPI_MISOR          IfxQspi0_MRSTA_P20_12_IN
#define AURIX_X1_SPI_CSR            IfxQspi0_SLSO2_P20_13_OUT
#define AURIX_X1_SPI_MOSIR          IfxQspi0_MTSR_P20_14_OUT
/*******************************************************/
/***************** Aurix IOs P21 ***********************/
/*******************************************************/
#define LS1028_PORESET              IfxPort_P21_0
#define X1_THERM_ALERT_3V3_N        IfxPort_P21_1
#define X1_THERM_SHDN_3V3_N         IfxPort_P21_2
#define AURIX_CIM_INT_N             IfxPort_P21_3
#define CIM_PWR_INT_N               IfxPort_P21_4
#define AURIX_LED1                  IfxPort_P21_5

/*******************************************************/
/***************** Aurix IOs P22 ***********************/
/*******************************************************/
#define SPI_TC397_4_SDI             IfxQspi4_MTSR_P22_0_OUT
#define SPI_TC397_4_SDO             IfxQspi4_MRSTB_P22_1_IN
#define SPI_TC397_4_CSN             IfxQspi4_SLSO3_P22_2_OUT
#define SPI_TC397_4_SCK             IfxQspi4_SCLK_P22_3_OUT
#define FR_B_ERR_N                  IfxPort_P22_5
// harness GPIO
#define AHRN_GPO0                   IfxPort_P22_6
#define AHRN_GPI0                   IfxPort_P22_7
#define DS160OR410_2_READ_EN_N      IfxPort_P22_8
#define DS160OR410_2_ALL_DONE_N     IfxPort_P22_9
//voltage sensing mux
#define ADC_MUX_CTRL_1              IfxPort_P22_8 //TODO: Remove?
#define ADC_MUX_CTRL_2              IfxPort_P22_9 //TODO: Remove?
#define ARX_ETHSW_PHY_MDIO_CTRL     IfxPort_P22_4 //TODO: No use on P3479! Modify Eth_Device driver.
#define CUST_ID_0                   IfxPort_P22_10
#define CUST_ID_1                   IfxPort_P22_11

/*******************************************************/
/***************** Aurix IOs P23 ***********************/
/*******************************************************/
#define X1_PMU_RST_OUT_N            IfxPort_P23_0
#define X1_PMU_SHDN_REQ_3V3         IfxPort_P23_1
#define AURIX_PPS                   IfxPort_P23_4
#define AURIX_X1_WAKE               IfxPort_P23_6

/*******************************************************/
/***************** Aurix IOs P32 ***********************/
/*******************************************************/ 
#define AURIX_1V3_VGATE_L           IfxPort_P32_0
#define AURIX_1V3_VGATE_H           IfxPort_P32_1
/* PUSH FAN */
/* high: Tegra control (good state)
 * Low:  MAX PWM (bad state) */
#define AURIX_X1_FANCTL             IfxPort_P32_3
#define XA_FAN_TACH_3V3             IfxPort_P32_4
#define CAN_PHY7_TXD                IfxCan_TXD02_P32_5_OUT
#define AURIX_FAN3_CTRL             IfxPort_P32_7

/*******************************************************/
/***************** Aurix IOs P33 ***********************/
/*******************************************************/
#define VBAT_TEG_PWR_ON             IfxPort_P33_0
#define X1_FORCE_RCVR_R_N           IfxPort_P33_1 //OD output
#define X1_PMIC_POWER_ON_R          IfxPort_P33_2
#define AURIX_WDT_TLF35584          IfxPort_P33_3
#define INT_TLF35584                IfxPort_P33_4
#define BRD_THERM_SHDN_N            IfxPort_P33_5
#define TLF35584_ERR                IfxPort_P33_8
#define LIN2_SLEEP                  IfxPort_P33_9
#define AURIX_XA_RST_N              IfxPort_P33_10
#define MCU_HW_ID_0                 IfxPort_P33_11
#define MCU_HW_ID_1                 IfxPort_P33_12
#define MCU_HW_ID_2                 IfxPort_P33_13
#define MCU_HW_ID_3                 IfxPort_P33_15
#define LS1028_BOOT_SEL             IfxPort_P33_14

/*******************************************************/
/***************** Aurix IOs P34 ***********************/
/*******************************************************/
#define XA_SOC_PWR_EN_3V3           IfxPort_P34_1
#define X1_SAFE_STATE_3V3_R         IfxPort_P34_3

/*******************************************************/
/***************** Aurix EVADC *************************/
/*******************************************************/
#define KL30_VBAT1_SENSE  		IfxEvadc_G0CH0_AN0_IN
#define KL30_POWER_SENSE      	IfxEvadc_G0CH1_AN1_IN
#define KL15_POWER_SENSE		IfxEvadc_G0CH2_AN2_IN
#define AURIX_5V_SENSE			IfxEvadc_G0CH3_AN3_IN
#define CAN1_5V_SENSE			IfxEvadc_G0CH4_AN4_IN
#define FR_5V_SENSE				IfxEvadc_G0CH5_AN5_IN
#define AURIX_3V3				IfxEvadc_G0CH6_AN6_IN
#define AURIX_1V3				IfxEvadc_G0CH7_AN7_IN
#define CAN2_5V_SENSE			IfxEvadc_G1CH0_AN8_IN
#define HRNS_GPI_SENSE			IfxEvadc_G1CH1_AN9_IN
#define KL30_VBAT2_SENSE  		IfxEvadc_G1CH2_AN10_IN
#define SNN_ADC_G1CH3      		IfxEvadc_G1CH3_AN11_IN
#define SNN_ADC_G1CH4 			IfxEvadc_G1CH4_AN12_IN
#define SNN_ADC_G1CH5			IfxEvadc_G1CH5_AN13_IN
#define SNN_ADC_G1CH6           IfxEvadc_G1CH6_AN14_IN
#define SNN_ADC_G1CH7           IfxEvadc_G1CH7_AN15_IN
#define VBATSYS_ISENSE          IfxEvadc_G2CH0_AN16_IN
#define VBATTEG_ISENSE          IfxEvadc_G2CH1_P40_10_IN //AN17
#define VBAT_SYS_ISENSE         IfxEvadc_G2CH2_P40_11_IN //AN18
#define SNN_VBATSXMB_ISENSE     IfxEvadc_G2CH3_P40_12_IN //AN19
#define VBATSYS_VSENSE          IfxEvadc_G2CH4_AN20_IN
#define VBATTEG_VSENSE          IfxEvadc_G2CH5_AN21_IN
#define VBAT_SYS_VSENSE         IfxEvadc_G2CH6_AN22_IN
#define SNN_VBATSXMB_VSENSE     IfxEvadc_G2CH7_AN23_IN
#define XA_PREREG_SENSE         IfxEvadc_G3CH0_P40_0_IN //AN24
#define XA_5V_SENSE             IfxEvadc_G3CH1_P40_1_IN //AN25
#define VDD_FAN_SENSE_R         IfxEvadc_G3CH2_P40_2_IN //AN26
#define VDD_6V0_PREREG_SENSE_R  IfxEvadc_G3CH3_P40_3_IN //AN27
#define VBOOST_5V_SENSE_R       IfxEvadc_G3CH4_P40_13_IN //AN28
#define SNN_ADC_G3CH5           IfxEvadc_G3CH5_P40_14_IN //AN29
#define SNN_ADC_G3CH6           IfxEvadc_G3CH6_AN30_IN
#define SNN_ADC_G3CH7           IfxEvadc_G3CH7_AN31_IN
#define SNN_ADC_G8CH0           IfxEvadc_G8CH0_P40_4_IN //AN32
#define SNN_ADC_G8CH1           IfxEvadc_G8CH1_P40_5_IN //AN33
#define SYS_2V1                 IfxEvadc_G8CH2_AN34_IN
#define SYS_1V2                 IfxEvadc_G8CH3_AN35_IN
#define SNN_ADC_G8CH4           IfxEvadc_G8CH4_P40_6_IN //AN36
#define SYS_0V85                IfxEvadc_G8CH5_P40_7_IN //AN37
#define SNN_ADC_G8CH6           IfxEvadc_G8CH6_P40_8_IN //AN38
#define SNN_ADC_G8CH7           IfxEvadc_G8CH7_P40_9_IN //AN39
#define SYS_5V_SNSE             IfxEvadc_G8CH8_AN40_IN //AN40
#define SNN_ADC_G8CH9           IfxEvadc_G8CH9_AN41_IN
#define SNN_ADC_G8CH10          IfxEvadc_G8CH10_AN42_IN
#define SYS_3V3_1_SNSE          IfxEvadc_G8CH11_AN43_IN
#define SYS_1V8                 IfxEvadc_G8CH12_AN44_IN
#define SYS_3V3_2_SNSE          IfxEvadc_G8CH13_AN45_IN
#define SNN_ADC_AN46            IfxEvadc_G8CH14_AN46_IN
#define SYS_0V9                 IfxEvadc_G8CH15_AN47_IN

#endif
