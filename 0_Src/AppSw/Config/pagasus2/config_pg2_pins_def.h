#ifndef CONFIG_PG2_PINS_DEF
#define CONFIG_PG2_PINS_DEF
#include <_PinMap/IfxAsclin_PinMap.h>
#include <_PinMap/IfxEray_PinMap.h>
#include <_PinMap/IfxGeth_PinMap.h>
#include <_PinMap/IfxI2c_PinMap.h>
#include <_PinMap/IfxCan_PinMap.h>
#include <_PinMap/IfxPort_PinMap.h>
#include <_PinMap/IfxQspi_PinMap.h>
#include <gpioexp_pinmap.h>
//#include <IfxPort_PinMap.h>

#define TC397B

//From AX.h
//new in DAX, control on Tegra/CVM CAN

// U47, SLAVE 0x74
#define XA_5V_PG                                       IoExpPort_A74_P00
//#define E3550_XA_PREREG_PG                             IoExpPort_A74_P01
#define P3570_B2B_PG_OUT        					   IoExpPort_A74_P01
#define X1_XA_VDD_SOC_XA_VDD_DDR2_XA_VDD_DDRQ_PG       IoExpPort_A74_P02
//#define XA_VDD_CPU_XA_VDD_GPU_XA_VDD_CV_PG             IoExpPort_A74_P03  //no use
#define XB_5V_PG                                       IoExpPort_A74_P04
//#define E3550_XB_PREREG_PG                             IoExpPort_A74_P05
#define P3570_B2B_PG_in								   IoExpPort_A74_P05
#define X2_XB_VDD_SOC_XB_VDD_DDR2_XB_VDD_DDRQ_PG       IoExpPort_A74_P06
//#define XB_VDD_CPU_XB_VDD_GPU_XB_VDD_CV_PG             IoExpPort_A74_P07  //no use
#define SYS_5V_PG                                      IoExpPort_A74_P10
#define SYS_0V85_SYS_1V0_SYS_1V2_PG                    IoExpPort_A74_P11
#define SYS_3V3_SYS_1V8_SYS_0V9_PG1                    IoExpPort_A74_P12
#define SYS_3V3_SYS_1V8_SYS_1V5_SYS_1V1_SYS_0V9_PG2    IoExpPort_A74_P13

//#define E3550_P1_5V_PG                                 IoExpPort_A74_P14
//#define E3550_P1_PREREG_PG                             IoExpPort_A74_P15
//#define E3550_CIM_PWR_GD                               IoExpPort_A74_P16
//#define E3550_CE_PREREG_PG                             IoExpPort_A74_P17

//#define P3570_A74_IOEXP_INT_TEST                       IoExpPort_A74_P14
#define P3570_BRD_THERM_ALERT_N_EXP_1   			   IoExpPort_A74_P15
#define P3570_BRD_THERM_ALERT_N_EXP_2  				   IoExpPort_A74_P16
#define P3570_BRD_THERM_ALERT_N_EXP_3  				   IoExpPort_A74_P17


// U248, SLAVE 0x75
#define CAN_PHY1_EN_EXP               IoExpPort_A75_P00
/* standby pin, set CAN mo  dule to low power mode when active */
#define CAN_PHY1_STB_N_EXP            IoExpPort_A75_P01  //low_acitve
#define CAN_PHY3_EN_EXP               IoExpPort_A75_P02
#define CAN_PHY3_STB_N_EXP            IoExpPort_A75_P03  //low_acitve
//CAN_PHY0_STB = CAN_PHYCDEF_STB
#define CAN_PHY0_STB_EXP              IoExpPort_A75_P04  //high_active
//control on X1/X2/P CAN
#define AURIX_XA_CAN_EN_N_EXP         IoExpPort_A75_P05
#define AURIX_XB_CAN_EN_N_EXP         IoExpPort_A75_P06
#define E3550_AURIX_P1_CAN_EN_N_EXP   IoExpPort_A75_P07               
#define P3570_I2C_GPIO_EXP4_IRQ       IoExpPort_A75_P07                //no use
#define FR_A_STB_N_EXP                IoExpPort_A75_P10   //low_active
#define FR_B_STB_N_EXP                IoExpPort_A75_P11   //low_active
#define P3570_NVME_RSTB               IoExpPort_A75_P13

//Note: for E3550
//#define E3550_P1_CVM_PRSNT_N_EXP      IoExpPort_A75_P12
//#define E3550_CVM_ID_0_EXP            IoExpPort_A75_P13
//#define E3550_CVM_ID_1_EXP            IoExpPort_A75_P14
//#define E3550_CVM_ID_2_EXP            IoExpPort_A75_P15
//#define E3550_CVM_ID_3_EXP            IoExpPort_A75_P16
//#define E3550_CVM_ID_4_EXP            IoExpPort_A75_P17

#define P3570_AURIX_IOEXP_INT_N_EXP   IoExpPort_A75_P12
//raby: need to turn on where it is in code
#define P3570_XA_OVR_THWN_N_EXP       IoExpPort_A75_P16
#define P3570_XB_OVR_THWN_N_EXP       IoExpPort_A75_P17



/* Valid for P3479-a03 or later only */
//#define P3479_OVR_CPU_THWN_N_EXP      IoExpPort_A75_P14
//#define P3479_OVR_GPU_THWN_N_EXP      IoExpPort_A75_P15
//#define P3479_OVR_SOC_THWN_N_EXP      IoExpPort_A75_P16
//#define P3479_OVR_CV_THWN_N_EXP       IoExpPort_A75_P17

// U261, SLAVE 0x76
#define X1_BSTRP0_EXP                 IoExpPort_A76_P00
#define X1_BSTRP1_EXP                 IoExpPort_A76_P01
#define X1_BSTRP2_EXP                 IoExpPort_A76_P02
//#define E3550_DBG_FTDI_EN_EXP         IoExpPort_A76_P03
#define P3570_B2B_TX_PWR_EN_EXP       IoExpPort_A76_P03
#define X2_BSTRP0_EXP                 IoExpPort_A76_P04
#define X2_BSTRP1_EXP                 IoExpPort_A76_P05
#define X2_BSTRP2_EXP                 IoExpPort_A76_P06
//#define E3550_ARX_PEXSW_UART_CTRL_EXP IoExpPort_A76_P07
#define P3570_B2B_TX_SYS_RST_N_EXP    IoExpPort_A76_P07
#define CIM_RST_N_EXP                 IoExpPort_A76_P10
//#define E3550_CIM_PWR_ON_EXP          IoExpPort_A76_P11
#define P3570_X1_FAN2_PWM_OVERRIDE_EXP IoExpPort_A76_P11
#define E3550_CIM_PRSNT_N_EXP         IoExpPort_A76_P12
#define P3570_X2_FAN2_PWM_OVERRIDE_EXP IoExpPort_A76_P12
#define E3550_CIM_SEAT_GOOD_EXP       IoExpPort_A76_P13
//#define E3550_HRN2_DET_EXP            IoExpPort_A76_P14
#define P3570_PEXSW1_VQPS_OTP_EN_EXP  IoExpPort_A76_P14
//#define E3550_CE_PREREG_EN_EXP        IoExpPort_A76_P15
#define P3570_AURIX_IOEXP_76_INT_EXP  IoExpPort_A76_P15
#define ARX_SXMA_VBAT_EN_EXP          IoExpPort_A76_P16
#define ARX_SXMB_VBAT_EN_EXP          IoExpPort_A76_P17

// U332, SLAVE 0x77
//#define E3550_PEXSW2_RESET_OUTPUT_N_EXP   IoExpPort_A77_P00
#define P3570_B2B_RX_SYS_RST_N_EXP        IoExpPort_A77_P00
#define ARX_ETH_2112_GRP1_MDC_CTRL_EXP    IoExpPort_A77_P01
#define ARX_ETH_2112_GRP2_MDC_CTRL_EXP    IoExpPort_A77_P02
#define ARX_ETH_Q5050_MDC_CTRL_EXP        IoExpPort_A77_P03
#define ARX_ETHSW_PHY_GRP1_RESET_N_EXP    IoExpPort_A77_P04
#define ARX_ETHSW_PHY_GRP2_RESET_N_EXP    IoExpPort_A77_P05
#define ARX_ETH_EA6321_1_MDC_CTRL_EXP     IoExpPort_A77_P06
//#define E3550_ARX_TRUST_PLATFORM_CTRL_EXP IoExpPort_A77_P07
#define P3570_AURIX_IOEXP_77_INT_EXP      IoExpPort_A77_P07
//Low: XA pull FAN, High: XB pull FAN
#define PULL_FAN_TACH_SEL_AB_EXP    	  IoExpPort_A77_P10
#define ARX_PEXSW1_BOOT_REC_EXP           IoExpPort_A77_P11
#define ARX_PEXSW2_BOOT_REC_EXP           IoExpPort_A77_P12
//#define E3550_COMEXP_PWR_EN_EXP           IoExpPort_A77_P13
#define P3570_B2B_GPI_1_EXP               IoExpPort_A77_P13
//#define E3550_COMEXP_RESET_CTRL_N_EXP     IoExpPort_A77_P14
#define P3570_B2B_GPO_1_EXP               IoExpPort_A77_P14
//#define E3550_ARX_PM8534_RESET_N_EXP      IoExpPort_A77_P15
#define P3570_PEXSW1_RESET_N_EXP          IoExpPort_A77_P15

//#define E3550_ARX_AQR107_RST_N_EXP        IoExpPort_A77_P16 // E3550
//#define P3479_LAN7431_RESET_N_EXP	      IoExpPort_A77_P16 // P3479-A03
#define P3570_B2B_GPI_2_EXP               IoExpPort_A77_P16 // P3570
//#define E3550_M2_RESET_N_EXP              IoExpPort_A77_P17
#define P3570_B2B_GPO_2_EXP               IoExpPort_A77_P17


// U667, SLAVE 0x74, CIM Ioexpander
#define SV_FRSYNC_EN_N              IoExpPort_CIM_A74_P00
#define CV_FRSYNC_EN_N              IoExpPort_CIM_A74_P01
#define LRC_FRSYNC_EN_N             IoExpPort_CIM_A74_P02
#define LRR_FRSYNC_EN_N             IoExpPort_CIM_A74_P03
#define SV_PWRDN_N                  IoExpPort_CIM_A74_P04
#define CV_PWRDN_N                  IoExpPort_CIM_A74_P05
#define LRC_PWRDN_N                 IoExpPort_CIM_A74_P06
#define LRR_PWRDN_N                 IoExpPort_CIM_A74_P07
#define TEGRA_SEL                   IoExpPort_CIM_A74_P16
#define FRSYNC_SEL                  IoExpPort_CIM_A74_P17
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

//#define E3550_ARX_DGPU1_MUX_CTRL          IfxPort_P00_6
//#define E3550_ARX_DGPU2_MUX_CTRL          IfxPort_P00_7
//#define E3550_P1_THERM_SHDN_N             IfxPort_P00_8

#define P3570_ARX_DGPU_MUX_CTRL           IfxPort_P00_6
//#define P3570_PIC_AURIX_INT_N       	  IfxPort_P00_7
//#define P3570_I2C_GPIO_EXP2_IRQ           IfxPort_P00_8

#define CAN_PHY1_ERR_N         			  IfxPort_P00_9      //low_acitve
//#define I2C_GPIO_EXP_IRQ            	  IfxPort_P00_10
#define P3570_ARX_PM40052_BOOT_INDEX1     IfxPort_P00_11
#define FLEXRAY_A_WAKE_N            	  IfxPort_P00_12

/*******************************************************/
/***************** Aurix IOs P01 ***********************/
/*******************************************************/
//HARNESS_C
#define CANR1_TXD               IfxCan_TXD01_P01_3_OUT
#define CANR1_RXD               IfxCan_RXD01C_P01_4_IN
//#define XA_VMON_INT_N               	  IfxPort_P01_5
//#define XB_VMON_INT_N               	  IfxPort_P01_6
//#define E3550_P1_VM_INT_3V3_N             IfxPort_P01_7
//#define E3550_P1_VM_INT_3V3_N             IfxPort_P01_7
#define P3570_AURIX_3_IOEXP_INT           IfxPort_P01_7

/*******************************************************/
/***************** Aurix IOs P02 ***********************/
/*******************************************************/
#define FR_A_TXD                    IfxEray0_TXDA_P02_0_OUT
#define FR_A_RXD                    IfxEray0_RXDA2_P02_1_IN
#define FR_B_TXD                    IfxEray0_TXDB_P02_2_OUT
#define FR_B_RXD                    IfxEray0_RXDB2_P02_3_IN

#define FR_A_TXEN                   IfxEray0_TXENA_P02_4_OUT
#define AURIX2_QSPI3_SPI_SSR        IfxQspi3_SLSIA_P02_4_IN

#define FR_B_TXEN                   IfxEray0_TXENB_P02_5_OUT
#define FR_A_RXEN                   		IfxPort_P02_6
#define FR_B_RXEN                   		IfxPort_P02_7
#define FR_A_ERR_N                  		IfxPort_P02_8
//#define E3550_ARX_PM8534_BOOT_INDEX         IfxPort_P02_9
#define P3570_ARX_PM40052_BOOT_INDEX0       IfxPort_P02_9
#define FR_A_EN                     		IfxPort_P02_10
#define XA_XB_PULL_FAN2_TACH        	    IfxPort_P02_11

/*******************************************************/
/***************** Aurix IOs P10 ***********************/
/*******************************************************/
#define SJA1105_CSN                 IfxQspi1_SLSO10_P10_0_OUT
#define SJA1105_SDO                 IfxQspi1_MRSTA_P10_1_IN  //MISO
#define SJA1105_SCK                 IfxQspi1_SCLK_P10_2_OUT
#define SJA1105_SDI                 IfxQspi1_MTSR_P10_3_OUT  //MOSI

#define FR_B_EN                     IfxPort_P10_4
// E3550: Parker, P3570: connected between Aurix2
#define QSPI3_SPI_SSR            IfxQspi3_SLSO8_P10_5_OUT  // MAIN Aurix only
#define QSPI3_SPI_MOSIR          IfxQspi3_MTSR_P10_6_OUT
#define QSPI3_SPI_MISOR          IfxQspi3_MRSTB_P10_7_IN
#define QSPI3_SPI_SCKR           IfxQspi3_SCLK_P10_8_OUT


/*******************************************************/
/***************** Aurix IOs P11 ***********************/
/*******************************************************/
#if 1
/* TC397B MII pins definition */
#define PHY_TXD3                    IfxGeth_TXD3_P11_0_OUT
#define PHY_TXD2                    IfxGeth_TXD2_P11_1_OUT
#define PHY_TXD1                    IfxGeth_TXD1_P11_2_OUT
#define PHY_TXD0                    IfxGeth_TXD0_P11_3_OUT
//Not used:  TXERR for A-Step device in MII mode, but we're not using TXERR
#define SNN_AURIX_ETH_RGMII_TXCLK   IfxPort_P11_4
//AURIX_ETH_MII_TXCLK_RGMII_REFCLK, TXCLK Input for A-Step device in MII mode
#define PHY_RGMII_TX_CLKR           IfxGeth_TXCLKA_P11_5_IN
#define PHY_TXEN                    IfxGeth_TXEN_P11_6_OUT
#define PHY_RGMII_RXD3              IfxGeth_RXD3A_P11_7_IN
#define PHY_RGMII_RXD2              IfxGeth_RXD2A_P11_8_IN
#define PHY_RGMII_RXD1              IfxGeth_RXD1A_P11_9_IN
#define PHY_RGMII_RXD0              IfxGeth_RXD0A_P11_10_IN
#define PHY_RGMII_RX_CTL            IfxGeth_RXDVA_P11_11_IN  // AURIX_ETH_MII_RX_CTL
/* RX CLK */
#define PHY_REFCLK                  IfxGeth_RXCLKA_P11_12_IN
#endif

#define AURIX_I2C1_SDA              IfxI2c1_SDA_P11_13_INOUT
#define AURIX_I2C1_SCL              IfxI2c1_SCL_P11_14_INOUT
#define SJA1105_RSTN                IfxPort_P11_15

/*******************************************************/
/***************** Aurix IOs P12 ***********************/
/*******************************************************/
#ifdef TC397B
#define PHY_MDC                     IfxGeth_MDC_P12_0_OUT
#define PHY_MDIO                    IfxGeth_MDIO_P12_1_INOUT
#else
#define PHY_MDC                     IfxEth_MDC_P12_0_OUT
#define PHY_MDIO                    IfxEth_MDIO_P12_1_INOUT
#endif

/*******************************************************/
/***************** Aurix IOs P13 ***********************/
/*******************************************************/
#define VBAT_SYS_PWR_ON             	  IfxPort_P13_0
#define AURIX_I2C_SCL               	  IfxI2c0_SCL_P13_1_INOUT
#define AURIX_I2C_SDA               	  IfxI2c0_SDA_P13_2_INOUT
#define E3550_COMEXP_PRESENT_DET_GND_N    IfxPort_P13_3
#define P3570_AURIX_ID                    IfxPort_P13_3

/*******************************************************/
/***************** Aurix IOs P14 ***********************/
/*******************************************************/
#define AURIX_DBG_UART_TX           IfxAsclin0_TX_P14_0_OUT
#define AURIX_DBG_UART_RX           IfxAsclin0_RXA_P14_1_IN
//#define AURIX_HWCFG2                IfxPort_P14_2
//#define AURIX_HWCFG3                IfxPort_P14_3
//#define AURIX_HWCFG6                IfxPort_P14_4
//#define AURIX_HWCFG1                IfxPort_P14_5
//HARNESS_D
#define CANR0_TXD               	IfxCan_TXD13_P14_6_OUT
#define CANR0_RXD               	IfxCan_RXD13A_P14_7_IN
#define CAN_PHY3_ERR_N          	IfxPort_P14_8
//HARNESS_B
#define CAN_PHY3_TXD            	IfxCan_TXD23_P14_9_OUT
#define CAN_PHY3_RXD            	IfxCan_RXD23A_P14_10_IN

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
//#define E3550_ARX_X1_FAN2_PWM_OVERRIDE    IfxPort_P15_0   //moved to A76_P11 in P3570
//#define E3550_ARX_X2_FAN2_PWM_OVERRIDE    IfxPort_P15_1   //moved to A76_P12 in P3570

#define P3570_AURIX_B2B_UART_TX			  IfxAsclin1_TX_P15_0_OUT
#define P3570_AURIX_B2B_UART_RX		      IfxAsclin1_RXA_P15_1_IN

#define AURIX_TLF_SPI_CS            	  IfxQspi2_SLSO0_P15_2_OUT
#define AURIX_TLF_SPI_SCK           	  IfxQspi2_SCLK_P15_3_OUT
#define AURIX_TLF_SPI_MISO          	  IfxQspi2_MRSTA_P15_4_IN
#define AURIX_TLF_SPI_MOSI          	  IfxQspi2_MTSR_P15_5_OUT
#define TLF_USE_CHIPSELECT          	  AURIX_TLF_SPI_CS

#define E3550_P1_FORCE_RCVR_N             IfxPort_P15_6
#define P3570_AURIX2_RCVR_N		    	  IfxPort_P15_6
#define X2_THERM_ALERT_3V3_N              IfxPort_P15_7
#define FLEXRAY_ENABLE_N                  IfxPort_P15_8

/*******************************************************/
/***************** Aurix IOs P20 ***********************/
/*******************************************************/
#define E3550_BRD_THERM_ALERT_N     		IfxPort_P20_0
#define P3570_AURIX1_HSSL_SYSCLK_OUT_R		//TBD: IfxPort_P20_0
#define AURIX_LED2                  		IfxPort_P20_1
#define AURIX_TESTMODE_IN           		IfxPort_P20_2
//#define E3550_AURIX_P1_SPI_HS_1             IfxPort_P20_3
//#define P3570_SS1_TLF35584_2                IfxPort_P20_3
//#define E3550_P1_BPMP_SHDN_REQ              IfxPort_P20_6
#define P3570_AURIX_CIM_INT_N               IfxPort_P20_6
#define AURIX_X2_SPI_HS_1           		IfxPort_P20_7
#define AURIX_X2_SPI_HS_2           		IfxPort_P20_8
#define AURIX_X1_SPI_HS_1           		IfxPort_P20_9
#define AURIX_X1_SPI_HS_2           		IfxPort_P20_10
// Xavier 1 SPI
#define AURIX_X1_SPI_SCKR           		IfxQspi0_SCLK_P20_11_OUT
#define AURIX_X1_SPI_MISOR          		IfxQspi0_MRSTA_P20_12_IN
#define AURIX_X1_SPI_CSR            		IfxQspi0_SLSO2_P20_13_OUT
#define AURIX_X1_SPI_MOSIR          		IfxQspi0_MTSR_P20_14_OUT
/*******************************************************/
/***************** Aurix IOs P21 ***********************/
/*******************************************************/
//#define E3550_ARX_USBHUB_RST_N            	IfxPort_P21_0
#define P3570_X1_THERM_SHDN_3V3_N   		IfxPort_P21_0
#define X1_THERM_ALERT_3V3_N        		IfxPort_P21_1
//#define E3550_X1_THERM_SHDN_3V3_N   		IfxPort_P21_2
//#define E3550_AURIX_CIM_INT_N               IfxPort_P21_3
//#define E3550_CIM_PWR_INT_N                 IfxPort_P21_4
#define P3570_AURIX1_HSSL_RXDN_R            //IfxPort_P21_2
#define P3570_AURIX1_HSSL_RXDP_R            //IfxPort_P21_3
#define P3570_AURIX1_HSSL_TXDN_R            //IfxPort_P21_4
#define P3570_AURIX1_HSSL_TXDP_R            //IfxPort_P21_5

#define P3570_AURIX2_HSSL_TXDN_R            //IfxPort_P21_2
#define P3570_AURIX2_HSSL_TXDP_R            //IfxPort_P21_3
#define P3570_AURIX2_HSSL_RXDN_R            //IfxPort_P21_4
#define P3570_AURIX2_HSSL_RXDP_R            //IfxPort_P21_5
//#define E3550_AURIX_LED1                  	IfxPort_P21_5
/*******************************************************/
/***************** Aurix IOs P22 ***********************/
/*******************************************************/
#define AURIX_X2_SPI_MOSIR          	IfxQspi4_MTSR_P22_0_OUT
#define AURIX_X2_SPI_MISOR          	IfxQspi4_MRSTB_P22_1_IN
#define AURIX_X2_SPI_SSR            	IfxQspi4_SLSO3_P22_2_OUT
#define AURIX_X2_SPI_SCKR           	IfxQspi4_SCLK_P22_3_OUT
#define ARX_ETHSW_PHY_MDIO_CTRL         IfxPort_P22_4
//#define FR_B_ERR_N                  	IfxPort_P22_5
// harness GPIO
#define AHRN_GPO0                   	IfxPort_P22_6
#define AHRN_GPI0                   	IfxPort_P22_7
//voltage sensing mux
#define ADC_MUX_CTRL_1              	IfxPort_P22_8
#define ADC_MUX_CTRL_2              	IfxPort_P22_9
#define E3550_P1_THERM_ALERT_3V3_N      IfxPort_P22_10
#define P3570_AURIX_PEXSW_ROM_WP_EN     IfxPort_P22_10
#define E3550_P1_SYS_SAFE_3V3_N         IfxPort_P22_11
#define P3570_AURIX_PCIECON_DRV_EN_N    IfxPort_P22_11 //OD output

/*******************************************************/
/***************** Aurix IOs P23 ***********************/
/*******************************************************/
//#define X1_PMU_RST_OUT_N                IfxPort_P23_0  //no use
//#define X1_PMU_SHDN_REQ_3V3         	IfxPort_P23_1    //no use
//#define X2_PMU_RST_OUT_N                IfxPort_P23_2  //no use
//#define X2_PMU_SHDN_REQ_3V3         	IfxPort_P23_3    //no use
//#define AURIX_PPS                   	IfxPort_P23_4 
//#define E3550_AURIX_P1_WAKE             IfxPort_P23_5
#define P3570_AURIX_PCIECON_PEX_RESET_N IfxPort_P23_5 //OD output
#define AURIX_X1_WAKE               	IfxPort_P23_6
#define AURIX_X2_WAKE               	IfxPort_P23_7

/*******************************************************/
/***************** Aurix IOs P32 ***********************/
/*******************************************************/ 
//efine AURIX_1V3_VGATE_1N          IfxPort_P32_0
//efine AURIX_1V3_VGATE_1P          IfxPort_P32_1
/* PUSH FAN */
/* high: Tegra control (good state)
 * Low:  MAX PWM (bad state) */
#define AURIX_X2_FANCTL             IfxPort_P32_2
#define AURIX_X1_FANCTL             IfxPort_P32_3
//efine XA_FAN_TACH_3V3             IfxPort_P32_4
//efine XB_FAN_TACH_3V3             IfxPort_P32_5
//no use{
#define INA3221_AURIX_WARN          IfxPort_P32_6
//}
#define E3550_P1_SOC_PWR_REQ_3V3    IfxPort_P32_7
#define P3570_AURIX_NVME_PEX_RESET_N IfxPort_P32_7  //OD output

/*******************************************************/
/***************** Aurix IOs P33 ***********************/
/*******************************************************/
#define VBAT_TEG_PWR_ON             IfxPort_P33_0
#define X1_FORCE_RCVR_R_N           IfxPort_P33_1   //OD output
#define X1_PMIC_POWER_ON_R          IfxPort_P33_2
//#define AURIX_WDT_TLF35584          IfxPort_P33_3 //no use
//#define INT_TLF35584                IfxPort_P33_4 //no use
#define BRD_THERM_SHDN_N            IfxPort_P33_5
#define X2_FORCE_RCVR_R_N           IfxPort_P33_6    //OD output
#define X2_PMIC_POWER_ON_R          IfxPort_P33_7
//#define TLF35584_ERR                IfxPort_P33_8 //no use
#define AURIX_XB_RST_N              IfxPort_P33_9
#define AURIX_XA_RST_N              IfxPort_P33_10

//#define E3550_ESATA_PEX_RST_N       IfxPort_P33_11
//#define E3550_AURIX_P1_PWR_ON_R     IfxPort_P33_12
#define E3550_AURIX_P1_RST_N        IfxPort_P33_13
//#define E3550_AQC107_1_PEX_RESET_N  IfxPort_P33_14
//#define E3550_AQC107_2_PEX_RESET_N  IfxPort_P33_15
#define P3570_LAN7431_RST_N			IfxPort_P33_11    //OD output
#define P3570_AURIX1_TO_AURIX2_HS   IfxPort_P33_12
#define P3570_AURIX2_TO_AURIX1_HS   IfxPort_P33_13
#define P3570_AURIX2_PWR_ON		    IfxPort_P33_14
#define P3570_AURIX2_RST_N			IfxPort_P33_15    //OD output

/*******************************************************/
/***************** Aurix IOs P34 ***********************/
/*******************************************************/
//#define XA_SOC_PWR_EN_3V3           IfxPort_P34_1
//#define XB_SOC_PWR_EN_3V3           IfxPort_P34_2
//#define X1_SAFE_STATE_3V3_R         IfxPort_P34_3
//#define X2_SAFE_STATE_3V3_R         IfxPort_P34_4
#define X2_THERM_SHDN_3V3_R_N       IfxPort_P34_5
#endif
