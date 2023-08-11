#ifndef CONFIG_V31C_FEATURE
#define CONFIG_V31C_FEATURE

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
#define DEBUG_ENABLE 1
//ex: LWIP
//
/* define if normal CAN operation use interrupt (canstress is not affected) */
#define CAN_INTERRUPT_ENABLE     0

// Enable UDP_Echo server on HI
#define TEGRA_UDP_ECHO_SERVER       1

#define AURIX_A_STEP            0
#define AURIX_B_STEP            0x10

/* The UART console shows up very later in B0x, due to the reason that
 * FTDI chip is power by system power.
 * We need to delay message so that it can show up later when the console is ready
 * Enable this macro to show the delay message (e.g. important warning) */
#define DELAY_MESSAGE_SHOW_UP_ENABLE  0
#define MESSAGE_DELAY_TIME_S    3
//-----------------------------------------------------------------
//HW CONFIG
#define MUL_CAN_TC29X_ENABLE    0
#define MCM_CAN_TC39X_ENABLE    0/*20190924,sync with source code*/

#define MCU_PLATFORM       _AURIX_TC39X_

#define CAN_NODE_0_ENABLE  0 /*20190924,sync with source code*/
#define CAN_NODE_1_ENABLE  0 /*20190924,sync with source code*/
#define CAN_NODE_2_ENABLE  0 /*20190924,sync with source code*/
#define CAN_NODE_3_ENABLE  0 /*20190924,sync with source code*/
#define CAN_NODE_4_ENABLE  0 /*20190924,sync with source code*/
#define CAN_NODE_5_ENABLE  0 /*20190924,sync with source code*/

#define CAN_PHY1_ENABLE_HIGH_LEVEL  0 /*20190924,sync with source code*/
#define CAN_PHY3_ENABLE_HIGH_LEVEL  0 /*20190924,sync with source code*/

#define CAN_EXT_ID_ENABLED          0

#define LIN_ENABLE         0
#define LIN_NODE_0_ENABLE  0
#define LIN_NODE_1_ENABLE  0
#define LIN_NODE_2_ENABLE  0
#define LIN_NODE_3_ENABLE  0

#define FLEXERAY_NODES     0 /*20190924,sync with source code*/

#if SPA2_HIA == 1
#define CAMERA_CIM_ENABLE  1
#define CAMERA_A_ENABLE    1
#define CAMERA_B_ENABLE    1
#define CAMERA_C_ENABLE    0 /*20190924,sync with source code*/
#define CAMERA_D_ENABLE    0 /*20190924,sync with source code*/
#define CAMERA_E_ENABLE    0
#else
#define CAMERA_CIM_ENABLE  0
#define CAMERA_A_ENABLE    0
#define CAMERA_B_ENABLE    0
#define CAMERA_C_ENABLE    0
#define CAMERA_D_ENABLE    0
#define CAMERA_E_ENABLE    0
#endif

#define ETHERNET_ENABLE   1
#if ETHERNET_ENABLE == 1
/* Ethernet Switch */
#define ETH_SWITCH_ENABLE               1    // Enable if there's Ethernet switch
//#define HYPERION

/* NXP switch features */
#define ETH_NXP_SWITCH_SJA1105_ENABLE   1    // Enable if there's NXP Ethernet switch SJA1105
#if ETH_NXP_SWITCH_SJA1105_ENABLE == 1
/* No need to change these Macro */
#define _DEFAULT_OFFICIAL_DRV_CONFIG_   0    // Static config comes from official driver
#define _E3550_VLAN_ENABLE_CONFIG_      1    // minimal config for E3550 with VLAN enable
#define _E3550_VLAN_DISABLE_CONFIG_     2    // minimal config for E3550 withought VLAN enable

/* Change this macro to enable different configs */
#define SJA1105_STATIC_CONFIG          _E3550_VLAN_ENABLE_CONFIG_
#endif

/* Marvell 88e6321 switch features */
#define ETH_MARVELL_SWITCH_ENABLE       0    // Enable if there's Marvell Ethernet switch
#if ETH_MARVELL_SWITCH_ENABLE == 1
/* Set it 1 to enable single chip mode for 88E6321, 88Q5050,
 * otherwise they're working in multi chip mode,
 * and some logic control pins will be n/a in multi chip mode
 * This is only supported in B0x source */

/* By default we run multi-chip mode */
#define SINGLE_CHIP_ADDRESS_MODE_ENABLE            0
/* Enable the initial section for management configuration */
#define MARVELL_MANAGEMENT_CONFIG_ENABLE    	   0
/* Config if Marvell switch is in CPU mode or NO CPU mode */
#define MARVELL_CPU_MODE_ENABLE    	   			   0 /*20190924,sync with source code*/
/* When enable, we will not initil Marvell switch when boot */
#define MARVELL_SWITCH_DEBUG_ENABLE                0
/* Enable the Marvell Switch Vlan */
#define MARVELL_SWITCH_VLAN_ENABLE      		   0 /*20190924,sync with source code*/
/* Enable the Marvell Switch control on the forwarding */
#define MARVELL_SWITCH_FORWARDING_CONTROL_ENABLE   0

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

#define IFXETH_SWITCH_88E6321_1_ADDR			  0x10  //no use but keep it for sharing function
#define IFXETH_SWITCH_88E6321_2_ADDR			  0x12  //no use but keep it for sharing function
#define IFXETH_SWITCH_88Q5050_ADDR				  0x11  //no use but keep it for sharing function

/* P3461 */
#define IFXETH_PHY_88Q2110_NUMBER_P3461 			    4
#define IFXETH_PHY_88Q2110_1_ADDR_P3461                 1  /* I229 / I229 */
#define IFXETH_PHY_88Q2110_2_ADDR_P3461                 2  /* I230 / I230 */
#define IFXETH_PHY_88Q2110_3_ADDR_P3461                 4  /* I75  / I75 */
#define IFXETH_PHY_88Q2110_4_ADDR_P3461                 6  /* I76  / I76 */

/* Define max number among all projects, for buffer usage */
#define PHY_88Q2112_MAX_NUMBER                    IFXETH_PHY_88Q2110_NUMBER_P3461

#endif // endof #if ETHERNET_ENABLE == 1


/* VADC config */
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

#define SPI0_ROLE    SPI_MASTER    // X1
#define SPI1_ROLE    SPI_MASTER	   // SJA1105
#define SPI2_ROLE    SPI_MASTER
#define SPI3_ROLE    SPI_MASTER    // CVM
//above spi is controlled in TegraQspi folder
#define SPI4_ROLE    SPI_UNUSED    // hia <-> hib, porting in HiaHibCom folder
#define SPI5_ROLE    SPI_UNUSED

/* FAN config */
#if SPA2_HIA == 1
#define FAN_A_ENABLE  1
#define FAN_B_ENABLE  1
#else
#define FAN_A_ENABLE  0
#define FAN_B_ENABLE  0
#endif
/* B01 only */
#define PULL_FAN_A_ENABLE  0 /*20190924,sync with source code*/
#define PULL_FAN_B_ENABLE  0 /*20190924,sync with source code*/


/* Mechanism to measure FAN RPM
 * Enable it to use TIM
 * Disable it to use GPIO pulling */
#define FAN_TACH_TIM_MEASURE_ENABLE     1

/* Define the TLF35584 standby wake up source */
#define WAKE_UP_SOURCE_FR_A_ENABLE      0 /*20190924,sync with source code*/
#define WAKE_UP_SOURCE_FR_B_ENABLE      0
#define WAKE_UP_SOURCE_CAN_PHY1_ENABLE  0 /*20190924,sync with source code*/
#define WAKE_UP_SOURCE_CAN_PHY3_ENABLE  0 /*20190924,sync with source code*/

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
#define GPIO_EXPANDER_ENABLE    1  // v31c should not have GPIO_EXPANDER //sync with source code
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

#define INFOROM_GENERATE_IMAGE_ENABLE  1  /** if inforom data goes wrong, this is a method to recovery quickly. */

/* Source Mux Selection, e.g. fpdlink, gmsl, hdmi */
#define SOURCE_MUX_SELECTION_ENABLE   0

/* COMMON Macro */
#define ARRAY_SIZE(x)   (sizeof(x)/sizeof(x[0]))
#define UNUSED(x)   (void)(x)

#define DelayMs(x)  waitTime((x)*TimeConst[TIMER_INDEX_1MS]);


/* Ethernet OTA enable */
#define ETHERNET_OTA_ENABLE  1
#define _OTA_PROD_FW            0x01
#define _OTA_UPDATE_FW          0x02
#define ETHERNET_OTA_FW_TYPE    _OTA_PROD_FW
//#define ETHERNET_OTA_FW_TYPE    _OTA_UPDATE_FW
#define _DEFAULT                0x01
#define _QPARTITION             0x02
#define PARTITION_LAYOUT        _DEFAULT

/* Ethernet UDP command */
#define ETHERNET_UDP_CMD_ENABLE   1


/* TEGRA UART enable */
#define TEGRA_UART_ENABLE   1 /*20190924,sync with source code*/



#endif
