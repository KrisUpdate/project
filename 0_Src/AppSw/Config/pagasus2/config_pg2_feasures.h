#ifndef CONFIG_PG2_FEATURE
#define CONFIG_PG2_FEATURE

/* MCU Platform Options */
#define _AURIX_TC29X_        0
#define _AURIX_TC39X_        1


/* Set this Macro to 0 to remove setting for eval board (pin mux, led etc)
 * Remember needs to do a clean build */
#define START_KIT_ENABLE   0

/* Set this Macro to 0 for normal build
 * Use this macro to simplify bringup or to disable functions not needed
 * during bringup
 */
#define FIRST_TIME_BRINGUP 0

#define float_support

//SW Feature
#define DEBUG_ENABLE 0
//ex: LWIP
//
#define _OTA_PROD_FW                    0x01
#define _OTA_UPDATE_FW                0x02
#define ETHERNET_OTA_FW_TYPE    _OTA_PROD_FW

#define _DEFAULT                             0x01
#define _QPARTITION                        0x02
#define PARTITION_LAYOUT              _DEFAULT

/* define if normal CAN operation use interrupt (canstress is not affected) */
#if PROJECT == PROJECT_PAGASUS2
#define CAN_INTERRUPT_ENABLE     1
#else
#define CAN_INTERRUPT_ENABLE     1
#endif

#define CAN_DEBUG_MSG_ENABLE     1
#if ETHERNET_OTA_FW_TYPE == _OTA_PROD_FW
#define CAN_QUEUE_ELEMENT_SIZE 512
#elif ETHERNET_OTA_FW_TYPE == _OTA_UPDATE_FW
#define CAN_QUEUE_ELEMENT_SIZE 8
#endif
#define UDP_ARRAY_SIZE 16384

#define USE_WC_PATTERN   0

#if 0 // no need
/* define the initial defautl system mode, available options are
 * INT_MODE:	   Use internal, includes all the commands
 * CUSTOMER_MODE:  Use by customer, include only the commands for customers.
 * */
#define DEFAULT_SYSTEM_MODE     INT_MODE
#endif
//-----------------------------------------------------------------

//HW CONFIG

#define MESSAGE_DELAY_TIME_S    3

#define MUL_CAN_TC29X_ENABLE    0
#define MCM_CAN_TC39X_ENABLE    1

#define MCU_PLATFORM       _AURIX_TC39X_

#define CAN_NODE_0_ENABLE  1
#define CAN_NODE_1_ENABLE  1
#define CAN_NODE_2_ENABLE  1
#define CAN_NODE_3_ENABLE  1
#define CAN_NODE_4_ENABLE  1
#define CAN_NODE_5_ENABLE  1

#define CAN_PHY1_ENABLE_HIGH_LEVEL  1
#define CAN_PHY3_ENABLE_HIGH_LEVEL  1

#define CAN_EXT_ID_ENABLED          1 /*[EXTENED ID test] */

#define LIN_ENABLE         0
#define LIN_NODE_0_ENABLE  0
#define LIN_NODE_1_ENABLE  0
#define LIN_NODE_2_ENABLE  0
#define LIN_NODE_3_ENABLE  0

#define FLEXERAY_NODES     2

#define CAMERA_A_ENABLE    1
#define CAMERA_B_ENABLE    1
#define CAMERA_C_ENABLE    1
#define CAMERA_D_ENABLE    1
#define CAMERA_E_ENABLE    0
//#if DRIVE_AX_B0x == 1
#define CAMERA_CIM_ENABLE  1
//#else
//#define CAMERA_CIM_ENABLE  0
//#endif

#define EXPOSE_AURIX   0

#define ETHERNET_ENABLE   1
#if ETHERNET_ENABLE == 1
/* Ethernet Switch */
#define ETH_SWITCH_ENABLE               1    // Enable if there's Ethernet switch
#define ETH_MARVELL_SWITCH_ENABLE       1    // Enable if there's Marvell Ethernet switch
//#define HYPERION
//#if DRIVE_AX_B0x == 1
#define ETH_NXP_SWITCH_SJA1105_ENABLE   1    // Enable if there's NXP Ethernet switch SJA1105
//#else
//#define ETH_NXP_SWITCH_SJA1105_ENABLE   0    // Enable if there's NXP Ethernet switch SJA1105
//#endif

/* NXP switch features */
#if ETH_NXP_SWITCH_SJA1105_ENABLE == 1
/* No need to change these Macro */
#define _DEFAULT_OFFICIAL_DRV_CONFIG_   0    // Static config comes from official driver
#define _E3550_VLAN_ENABLE_CONFIG_      1    // minimal config for E3550 with VLAN enable
#define _E3550_VLAN_DISABLE_CONFIG_     2    // minimal config for E3550 withought VLAN enable

/* Change this macro to enable different configs */
#define SJA1105_STATIC_CONFIG          _E3550_VLAN_ENABLE_CONFIG_
#endif

/* Marvell 88e6321 switch features */
#if ETH_MARVELL_SWITCH_ENABLE == 1
/* Set it 1 to enable single chip mode for 88E6321, 88Q5050,
 * otherwise they're working in multi chip mode,
 * and some logic control pins will be n/a in multi chip mode
 * This is only supported in B0x source */
//#if DRIVE_AX_B0x == 1
/* By default we run multi-chip mode */
#define SINGLE_CHIP_ADDRESS_MODE_ENABLE            0
//#else
//#define SINGLE_CHIP_ADDRESS_MODE_ENABLE            1
//#endif

/* Enable the initial section for management configuration */
#define MARVELL_MANAGEMENT_CONFIG_ENABLE    	   0
/* Config if Marvell switch is in CPU mode or NO CPU mode */
#define MARVELL_CPU_MODE_ENABLE    	   			   1
/* When enable, we will not initil Marvell switch when boot */
#define MARVELL_SWITCH_DEBUG_ENABLE                0
/* Enable the Marvell Switch Vlan */
#define MARVELL_SWITCH_VLAN_ENABLE      		   1
/* Enable the Marvell Switch control on the forwarding */
#define MARVELL_SWITCH_FORWARDING_CONTROL_ENABLE   0
#if ETHERNET_OTA_FW_TYPE==_OTA_PROD_FW
#define FEATURE_MARVELL_6321                       1
#elif ETHERNET_OTA_FW_TYPE==_OTA_UPDATE_FW
#define FEATURE_MARVELL_6321                       0
#endif

#endif //endof #if ETH_MARVELL_SWITCH_ENABLE == 1

/* Ethernet PHY*/
#define ETH_PHY_ENABLE                  1    // Enable if there's Ethernet phy
#define BROAD_REACH_PHY_ENABLE          0    // Enable if there's Broad Reach phy
#define ETH_PHY_88Q2112_ENABLE          1    // Enable if there's Marvell phy 88Q2112
/* Ethernet General Features */
#define ETH_AUTO_START_ENABLE           1
#define ETH_IPERF_TEST_ENABLE           0
#define MAC_ADDRESS_MANUAL_SET          0
#define ETH_LWIP_DEBUG_ENABLE           1
#define MII    0
#define RMII   1
//#if START_KIT_ENABLE == 1
//#define ETH_INTERFACE    RMII
//#else
#define ETH_INTERFACE    MII
//#endif // endof #if START_KIT_ENABLE == 1

/* Define MDIO Slave address, it's for multi-chip mode configuration */
#if DRIVE_AX_A01 == 1
#define IFXETH_PHY_88Q2112_NUMBER 				  3
#define IFXETH_PHY_88Q2112_1_ADDR                 5  //U174
#define IFXETH_PHY_88Q2112_2_ADDR                 1  //U224
#define IFXETH_PHY_88Q2112_3_ADDR                 2  //U226
#define IFXETH_SWITCH_88E6321_1_ADDR			  0  //Single chip mode
#elif DRIVE_AX_B0x == PG2_PREEVT
/* In slave address, we need to pass the MDIO group as well
 * Format is group id: bit 24 ~ bit 31
 * mdio slave address: bit 0 ~ bit 23 */
#define GROUP_1                                   (1 << 24)
#define GROUP_2                                   (2 << 24)
#define GET_GROUP(x)                              (x >> 24)
#define SET_GROUP(x,y)							  (x|=(y<<24))
#define GET_MDIO_SLAVE_ADDRESS(x)                 (x & 0xfff)

/* P3479 */
#define IFXETH_PHY_88Q2112_NUMBER_P3479_A01 			8
#define IFXETH_PHY_88Q2112_NUMBER_P3479_A03             7
#define IFXETH_PHY_88Q2112_1_ADDR_P3479                 (1 | GROUP_1)  //I19, U474
#define IFXETH_PHY_88Q2112_2_ADDR_P3479                 (2 | GROUP_1)  //I13, U471
#define IFXETH_PHY_88Q2112_3_ADDR_P3479                 (3 | GROUP_1)  //I44, U559
#define IFXETH_PHY_88Q2112_4_ADDR_P3479                 (4 | GROUP_1)  //I45, U562
#define IFXETH_PHY_88Q2112_5_ADDR_P3479                 (1 | GROUP_2)  //I37, U477 // Move out from Aurix in A03
#define IFXETH_PHY_88Q2112_6_ADDR_P3479                 (2 | GROUP_2)  //I38, U480
#define IFXETH_PHY_88Q2112_7_ADDR_P3479                 (3 | GROUP_2)  //I67, U565
#define IFXETH_PHY_88Q2112_8_ADDR_P3479                 (4 | GROUP_2)  //I68, U568

/* E3550 */
#define IFXETH_PHY_88Q2112_NUMBER_E3550 			    7
#define IFXETH_PHY_88Q2112_0_ADDR_E3550                 0
#define IFXETH_PHY_88Q2112_1_ADDR_E3550                 1  //I15, U178
#define IFXETH_PHY_88Q2112_2_ADDR_E3550                 2  //I30, U181
#define IFXETH_PHY_88Q2112_3_ADDR_E3550                 3  //I13, U183
#define IFXETH_PHY_88Q2112_4_ADDR_E3550                 4  //I19, U185
#define IFXETH_PHY_88Q2112_5_ADDR_E3550                 5  //I37, U160
#define IFXETH_PHY_88Q2112_6_ADDR_E3550                 6  //I38, U162
#define IFXETH_PHY_88Q2112_7_ADDR_E3550                 7  //I11, U176
#define IFXETH_SWITCH_88E6321_1_ADDR			  0x10  //I84, U173
#define IFXETH_SWITCH_88E6321_2_ADDR			  0x12  //I67, U169
#define IFXETH_SWITCH_88Q5050_ADDR				  0x11  //U25

/* Define max number among all projects, for buffer usage */
#define PHY_88Q2112_MAX_NUMBER                    IFXETH_PHY_88Q2112_NUMBER_P3479_A01

#endif // endof #if DRIVE_AX_A01 == 1

/* AVB feature*/
#define FEATURE_AVB			0
#if FEATURE_AVB  == 1
#define FEATURE_PTP             1
#if FEATURE_PTP == 1
#define MAIN_AURIX_PTP_ENABLE 1
#define SECOND_AURIX_PTP_ENABLE 1
#endif// endof #if FEATURE_PTP == 1
#endif // endif #if FEATURE_AVB  ==1

#endif // endof #if ETHERNET_ENABLE == 1

#if 0
/* VADC config */
#define VDD_GRP1_ADCHANNEL 37
#define VDD_GRP2_ADCHANNEL 38
#define VDD_GRP3_ADCHANNEL 39
#define VDD_GRP4_ADCHANNEL 44

#define TVG1_I_SENSE_ADCHANNEL		32
#define TVG2_I_SENSE_ADCHANNEL		33
#define TVG3_I_SENSE_ADCHANNEL		34
#define TVG4_I_SENSE_ADCHANNEL		35
#define TVG5_I_SENSE_ADCHANNEL		36
#define TVG1_V_SENSE_ADCHANNEL		40
#define TVG2_V_SENSE_ADCHANNEL		41
#define TVG3_V_SENSE_ADCHANNEL		42
#define TVG4_V_SENSE_ADCHANNEL		43
#define TVG5_V_SENSE_ADCHANNEL		28
#endif

/* This is to monitor PMIC power to check if FTDI has shutdown the power
 * It needs VADC support
 * However, EVADC driver is not working very stable, we disable it for now in bring up*/
#define PMIC_MONITORING_ENABLE  0
#define AN_VDD_3V3           4
#define AN_VDD_3V3_B         6
#define AN_VDD_CPU_CHAN      16
#define AN_VDD_CPU_B_CHAN    20

/* SPI config */
#define  SPI_MASTER  0
#define  SPI_SLAVE   1
#define  SPI_UNUSED  0xFF
//#if DRIVE_AX_B0x == 1
#if 1
/* DRIVE_AX_B0x */
#define SPI0_ROLE    SPI_MASTER    // X1
#define SPI1_ROLE    SPI_MASTER	   // SJA1105
#define SPI2_ROLE    SPI_UNUSED
#define SPI3_ROLE    SPI_MASTER    // CVM
#define SPI4_ROLE    SPI_MASTER    // X2
#define SPI5_ROLE    SPI_UNUSED
#else
/* DRIVE_AX_A00 or DRIVE_AX_A01 */
#define SPI0_ROLE    SPI_MASTER
#define SPI1_ROLE    SPI_UNUSED
#define SPI2_ROLE    SPI_UNUSED
#define SPI3_ROLE    SPI_UNUSED
#define SPI4_ROLE    SPI_UNUSED
#define SPI5_ROLE    SPI_UNUSED
#endif

/* FAN config */
#define FAN_A_ENABLE  1
//#if DRIVE_AX_B0x == 1
#if 1
#define FAN_B_ENABLE  1
/* B01 only */
#define PULL_FAN_A_ENABLE  1
#define PULL_FAN_B_ENABLE  1
#else
#define FAN_B_ENABLE  0
#endif

/* Mechanism to measure FAN RPM
 * Enable it to use TIM
 * Disable it to use GPIO pulling */
#define FAN_TACH_TIM_MEASURE_ENABLE     1

/* Define the TLF35584 standby wake up source */
#define WAKE_UP_SOURCE_FR_A_ENABLE      1
#define WAKE_UP_SOURCE_FR_B_ENABLE      0
#define WAKE_UP_SOURCE_CAN_PHY1_ENABLE  1
#define WAKE_UP_SOURCE_CAN_PHY3_ENABLE  1
#define STANDBY_DELAY_MS  2

/****************************************************************************************************
 * Features
 * By enalbe/disable below Marco, we can crontol if this features are enbled/disable for the projects
 * for features that are enable, it will show up in the help commands list
 * otherwise it won't show up and can not be executed
 *****************************************************************************************************/

/* Select if we have SPI slave in Aurix */
#define SPI_SLAVE_ENABLE     0

#define SPI_TP_ENABLE        0
/* TSensor */
/* define if we need to enable the WAR solution for TMP411 to
 * disable the Remote temp when it's not used
 * it's not working for TMP451! */
#define TMP411_WAR_ENABLE   0

/* GPIO EXPANDER */
#define GPIO_EXPANDER_ENABLE    1
//#if DRIVE_AX_B0x == 1
/* 4 io expander (in main board) + 1 io expander (in CIM) */
/* For the io expander in CIM,
 * let's check how to handle it after CIM support is added*/
#define NUM_GPIO_EXP_DEVICES    4
//#else
//#define NUM_GPIO_EXP_DEVICES    2
//#endif

/* Enable to set I2C gpio to inactive state to save power
 * But we find out that it will affect I2C bus and io expander */
#define I2C_GPIO_POWER_SAVING_ENABLE   0
/* IO Expander Address */
#define GPIO_EXP_I2C_BUS 0x1
#define GPIO_EXP0_I2C_ADDR 0x74
#define GPIO_EXP1_I2C_ADDR 0x75
#define GPIO_EXP2_I2C_ADDR 0x76
#define GPIO_EXP3_I2C_ADDR 0x77

/* Source Mux Selection, e.g. fpdlink, gmsl, hdmi */
#define SOURCE_MUX_SELECTION_ENABLE   0

/* COMMON Macro */
#define ARRAY_SIZE(x)   (sizeof(x)/sizeof(x[0]))
#define UNUSED(x)   (void)(x)

#define DelayMs(x)  waitTime((x)*TimeConst[TIMER_INDEX_1MS]);
#define STANDBY_DELAY_MS  2

#if 0
/* Source Mux Selection, e.g. fpdlink, gmsl, hdmi */
#define SOURCE_MUX_SELECTION_ENABLE   0
#endif

/* Ethernet UDP command */
#define ETHERNET_UDP_CMD_ENABLE   1

#if 0 // no need
#if DRIVE_AX_A00 == 1
/* TEGRA UART enable */
#define TEGRA_UART_ENABLE   0
#elif DRIVE_AX_A01 == 1/* A01*/
/* TEGRA UART enable */
#define TEGRA_UART_ENABLE   1
#endif
#endif
/* COM Express Mode */
//#if DRIVE_AX_B0x == 1
#define CE_SUPPORT     1
//#else
//#define CE_SUPPORT     0
//#endif

/*Define canbus stress test for disable broadcast mode*/
#define CANBUS_STRESS_DISBM_ENABLE        0

#endif
