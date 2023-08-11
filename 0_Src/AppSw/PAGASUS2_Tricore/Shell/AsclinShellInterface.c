/**
 * \file AsclinShellInterface.c
 * \brief ASCLIN Shell interface demo
 *
 * \version disabled
 * \copyright Copyright (c) 2014 Infineon Technologies AG. All rights reserved.
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
 */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "AsclinShellInterface.h"
#include "internal_fw_debug.h"
#if FEATURE_AVB
#include "maap.h"
#endif
#include "GethBasicDemo.h"
#include "SRPDemo.h"
#include "ping.h"
#include "SysSe/Time/Ifx_DateTime.h"
#include "IfxLldVersion.h"
#include "_Impl/IfxGlobal_cfg.h"
#include "Cpu_Main.h"
#include "Eray.h"
#include "Mcmcan.h"
#include "Phy_88q2112.h"
#include "marvell_switch.h"
#include "VadcCmds.h"
#include "EvadcAutoScan.h"
#include "i2crom_drv.h"

//#include "Cpu0_Main.h"
#include <I2cCmds.h>
#include <Vmon.h>
#include <TsensorCmds.h>
#include <PSensorCmds.h>
#include <gpioexpcmds.h>

#include <TLF35584Cmds.h>

#include <GpioCmds.h>
#include "Gpio.h"
#include <VadcCmds.h>
#include <ErayCmds.h>
#include <RegisterCmds.h>


#include <MulticanCmds.h>
#include <TegraQspiCmds.h>
#include <EthernetCmds.h>
#include "TLF35584.h"
#include "CIMcmds.h"
#include "AppAscUartCmds.h"
#include "fan.h"
#include "IfxStdIf_DPipe.h"
//#include "_Utilities/Ifx_Assert.h"
#include "system_monitor.h"
#include "HsslCmds.h"
#include "board2board.h"

#include <string.h>
#include "board.h"
//#include <stdio.h>
#include <stdarg.h>
#include "Ifx_Console.h"
#include "config_pg2_feasures.h"
#include "FlashCmds.h"
#include "Routine.h"
#include "IfxPms_reg.h"
#include "ErayCmds.h"
#include "resource.h"
#include "core_alive_check.h"


#if FEATURE_PTP
#include "ptp_api.h"
#endif
#if FEATURE_MARVELL_6321
#include "msdApiTypes.h"
#include "msdSysConfig.h"
#include "msdRMU.h"
#include "Marvell88E6321Demo.h"
#include "Pearl_msdRMU.h"
boolean AppShell_rmu(pchar args, void *data, IfxStdIf_DPipe *io);
#endif
#include <cangateway.h>
#include "ErayNormalDemo.h"
#include "tegra_console.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

#define SHELL_HELP_DESCRIPTION_TEXT                                 \
    "     : Display command list."ENDL                              \
    "           A command followed by a question mark '?' will"ENDL \
    "           show the command syntax"

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
#if 0
#if ISR_PROVIDER_ASC_SHELL == 0
    #if defined(__GNUC__)
    #pragma section ".bss_cpu0" awc0
    #endif
    #if defined(__TASKING__)
    #pragma section farbss "bss_cpu0"
    #pragma section fardata "data_cpu0"
    #endif
    #if defined(__DCC__)
    #pragma section DATA ".data_cpu0" ".bss_cpu0" far-absolute RW
    #endif
#elif ISR_PROVIDER_ASC_SHELL == 1
    #if defined(__GNUC__)
    #pragma section ".bss_cpu1" awc1
    #endif
    #if defined(__TASKING__)
    #pragma section farbss "bss_cpu1"
    #pragma section fardata "data_cpu1"
    #endif
    #if defined(__DCC__)
    #pragma section DATA ".data_cpu1" ".bss_cpu1" far-absolute RW
    #endif
#elif ISR_PROVIDER_ASC_SHELL == 2
    #if defined(__GNUC__)
    #pragma section ".bss_cpu2" awc2
    #endif
    #if defined(__TASKING__)
    #pragma section farbss "bss_cpu2"
    #pragma section fardata "data_cpu2"
    #endif
    #if defined(__DCC__)
    #pragma section DATA ".data_cpu2" ".bss_cpu2" far-absolute RW
    #endif
#else
#error "Set ISR_PROVIDER_ASC_SHELL to a valid value!"
#endif
#endif
App_AsclinShellInterface g_AsclinShellInterface; /**< \brief Demo information */
extern boolean can_1722_flag = FALSE;
extern Eray_run_mode g_Eray_run_mode;

static uint32 g_tx_num[5],g_rx_num[5];
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

boolean AppShell_status(pchar args, void *data, IfxStdIf_DPipe *io);


boolean Stop_continuous_logging(pchar args, void *data, IfxStdIf_DPipe *io);

static boolean Stop_thermal_logging(pchar args, void *data, IfxStdIf_DPipe *io);
static boolean Enable_thermal_logging(pchar args, void *data, IfxStdIf_DPipe *io);

static boolean Shell_echo(pchar args, void *data, IfxStdIf_DPipe *io);
static boolean Shell_udp_echo(pchar args, void *data, IfxStdIf_DPipe *io);
static boolean Show_Menu_All(pchar args, void *data, IfxStdIf_DPipe *io);
static boolean Marvell_5050_tool(pchar args, void *data, IfxStdIf_DPipe *io);
static boolean Shell_version(pchar args, void *data, IfxStdIf_DPipe *io);
static boolean Shell_revision(pchar args, void *data, IfxStdIf_DPipe *io);
static boolean Shell_load_board_config(pchar args, void *data, IfxStdIf_DPipe *io);

boolean AppShell_info(pchar args, void *data, IfxStdIf_DPipe *io);
boolean AppShell_geth(pchar args, void *data, IfxStdIf_DPipe *io);
boolean AppShell_maap(pchar args, void *data, IfxStdIf_DPipe *io);
boolean AppShell_can(pchar args, void *data, IfxStdIf_DPipe *io);
boolean AppShell_geth(pchar args, void *data, IfxStdIf_DPipe *io);
static boolean do_thermal(pchar args, void *data, IfxStdIf_DPipe *io);
#if FEATURE_PTP
boolean AppShell_ptp(pchar args, void *data, IfxStdIf_DPipe *io);
boolean AppShell_ptpon(pchar args, void *data, IfxStdIf_DPipe *io);
#endif

extern  volatile uint32 cpu0_idle_counter;
unsigned char Backup_pwr_start_count = 0;

#define BACKUP_PWR_UP_SEC 10
#define BACKUP_PWR_FINISHED_FLAG 99

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/** \brief Application shell command list */
#if 0
const Ifx_Shell_Command APPSHELL_COMMANDS_MAIN_AURIX[] = {
    //=====================================================================================================
    //Cmds to from Aurix itself
    {"rdaurixreg",   "read aurix internal reg by keyword",        &g_AsclinShellInterface.shell, &Register_Read_keyword},
    {"tegrarecovery", ": Set Tegra X1/X2/CVM in recovery mode",   &g_AsclinShellInterface,       &TegraRecovery},
    {"tegrareset", "   : Reset Tegra X1/X2/CVM",   &g_AsclinShellInterface,       &TegraReset},
    {"showport", "     : Show the state of Px.y",   &g_AsclinShellInterface,       &Show_PortState},
    {"showportall", "     : Show the state of Px.y",   &g_AsclinShellInterface,       &Show_PortState_all},
    {"setport", "      : Set the state of Px.y to state z (0/1)",   &g_AsclinShellInterface,       &Set_PortState},
    {"configport", "   : Configure Px.y to input or output",   &g_AsclinShellInterface,       &Config_Port},
    {"regrd", "        : regrd x [length]: read [length] starting from register address x",   &g_AsclinShellInterface, &Register_Read},
    {"regwr", "        : regwr x value: write value to register address x",   &g_AsclinShellInterface, &Register_Write},
    {"showan", "       : Show the value(s) of ANx [to y]",   &g_AsclinShellInterface,       &Show_ANx_y},
    {"showvoltages", " : Display all voltages",   &g_AsclinShellInterface,       &ShowVoltages},
    {"showfan", "      : show the fan RPM",   &g_AsclinShellInterface,       &Show_FanRPM},
    {"ping", "         : Send a ping to any address or to the gateway",   &g_AsclinShellInterface,       &ethernetping},
    {"netstat", "      : Show status of ethernet",   &g_AsclinShellInterface,       &ethernetstat,      },
    {"mdioinit", "     : read PHY/Switch Register through MDIO/ interface",   &g_AsclinShellInterface,       &mdioinit},
    {"aurix2reset", "   : Reset Aurix2",   &g_AsclinShellInterface,       &Aurix2Reset},
    {"aurix2recovery", "   : Set Aurix2 in recovery mode",   &g_AsclinShellInterface,       &Aurix2Recovery},
    {"kl30config",   "kl30config",        &g_AsclinShellInterface.shell, &System_Monitor_Config_Cmd},
    //=====================================================================================================
    //Cmds to handshake with other devices
    {"hssltest", "     : Run hssl test",   &g_AsclinShellInterface,       &hssltest },
    {"tegrainit", "    : Initialize the Qspi module for Tegra",   &g_AsclinShellInterface,       &tegraqspiinit},
    {"tegrawr", "      : Write and read tegra X1/X2/CVM with n bytes with master SPI",   &g_AsclinShellInterface,       &tegrawriteread},
    {"b2brun", "          : show temperature of board to board", &g_AsclinShellInterface,       &B2b_run},
    //{"b2bcmd", "          : send command to board to board", &g_AsclinShellInterface,       &b2b_cmd_nc},
#if DEBUG_ENABLE == 1
    /* it's used with old mods test, should be abandon but we will keep it here for now */
    {"spimastertest", ": test with tegra A(/B) with n bytes with master SPI",   &g_AsclinShellInterface,       &spimastertest},
#endif
    //=====================================================================================================
    //complex, involves all HW ex: system on/off (I2C0, I2C1, SPI 0~4)
    {"poweroff", "     : Set the outputs to system power off state",   &g_AsclinShellInterface,       &System_PowerOff_Cmd},
    {"poweron", "      : Turn main power on and release Tegra resets",   &g_AsclinShellInterface,       &System_PowerOn_Cmd},
    {"aurix2flash", "   : WAR: Prepare Aurix2 into recovery mode",   &g_AsclinShellInterface,       &Aurix2Flash},    
    {"aurixreset", "   : reset Aurix",   &g_AsclinShellInterface,       &aurixrst},
    //with I2C rom, ethernet init
/*test*/    {"ethinit", "      : Initialize the Ethernet module and the Phy",   &g_AsclinShellInterface,       &ethernetinit,     },
    //=====================================================================================================
    //Cmds to involve I2C1 and MDIO
    {"mdioset", "      : set mdio logic to access specific device",   &g_AsclinShellInterface,              &mdioset},
    {"mdiord", "       : read PHY/Switch Register through MDIO/ interface",   &g_AsclinShellInterface,       &mdiord},
    {"mdiowr", "       : write value to PHY/Switch Register through MDIO interface",   &g_AsclinShellInterface,       &mdiowr},
#if ETH_SWITCH_ENABLE == 1
    {"rdswphy", "      : read PHY/Switch Register",   &g_AsclinShellInterface,       &readSWitchPhy},
    {"wrswphy", "      : write to PHY/Switch Register",   &g_AsclinShellInterface,       &writeSWitchPhy}, 
#endif
#if ETH_PHY_88Q2112_ENABLE == 1 
    {"ethphyinit", "    : init various ethernet phy",   &g_AsclinShellInterface.shell,       &ethphyinit}, 
    {"ethphyrd", "      : read PHY Register",           &g_AsclinShellInterface,             &ethphyread},
    {"ethphywr", "      : write to PHY Register",       &g_AsclinShellInterface,             &ethphywrite},
#endif
    //Cmds to involve SPI, I2C1 and MDIO
     //spi and i2c1
    {"ethswrd", "      : read the ethernet switch register",   &g_AsclinShellInterface,     &ethswitchread},
    {"ethswwr", "      : write the ethernet switch register",  &g_AsclinShellInterface,     &ethswitchwrite},
    //=====================================================================================================
    //Cmds to I2C0, I2C1
    {"i2cinit", "      : init i2c module",   &g_AsclinShellInterface,       &i2cinit},
    {"i2cscan", "      : Scan i2c module for devices",   &g_AsclinShellInterface,       &i2cscan},
    {"i2cread", "      : Read from i2c module x device y at offset z n bytes",   &g_AsclinShellInterface,       &i2cread},
    {"i2cwrite", "     : Write to i2c module x device y at offset z n bytes",   &g_AsclinShellInterface,       &i2cwrite},
    //=====================================================================================================
    //Cmds to mainly for I2C devices
    //I2C0
    {"cimpower", "     : turn on/off Camera Interface Module",  &g_AsclinShellInterface,    &CIM_Power_Control_Cmd},
    {"cimcampower", "  : turn on/off Camera Unit LSW power",   &g_AsclinShellInterface,    &CIM_LSW_Control_Cmd},
    {"cimaggpower", "  : turn on/off Camera Group Aggregator",  &g_AsclinShellInterface,    &CIM_Aggregator_Control_Cmd},
    {"cimfsync", "     : control the Camera frame synce",  &g_AsclinShellInterface,    &CIM_FrameSync_Control_Cmd},
    {"cimseatgood", "  : Show CIM Presence and seat good status",  &g_AsclinShellInterface,    &CIM_SGood_Status_Cmd},
    {"i2cromtest", "   : run the i2c read write test on ROM M24C02",   &g_AsclinShellInterface,       &i2cromtest},
    {"inforom", "      : Operation on InfoROM M24C02",   &g_AsclinShellInterface,       &i2cromrw},
    {"rdpower", "      : rdpower [c/loop] [interval]:  read board power for [loop] times\n\r or continuously with interval in between read", &g_AsclinShellInterface, &ReadPower}, 
    {"vmonstatus", "   : Print the status of VMONs",   &g_AsclinShellInterface,       &Vmon_showStatus},
    {"readcamvolt",   "     : read camera voltage",   &g_AsclinShellInterface,       &ReadCamVolt},
    {"readcamcurr",   "     : read camera current",   &g_AsclinShellInterface,       &ReadCamCurr},
    {"readcamvoltcurr",   "     : read camera current",   &g_AsclinShellInterface,       &ReadCamVolCurr},
    {"ignchksta"      ,   "     : Read Ignition Check Feature Enablement setting"          ,   &g_AsclinShellInterface,       &IgnitionChkEngage_Sta},
    {"ignchkset"      ,   "     : Setup Ignition Check Feature Enablement setting"         ,   &g_AsclinShellInterface,       &IgnitionChkEngage_Set},
    {"ethphysta"      ,   "     : Read Ethernet PHY work mode & Master/Slave setting"      ,   &g_AsclinShellInterface,       &EthPHYModeMS_Sta     },
    {"ethphymodeset"  ,   "     : Setup Ethernet PHY work mode setting"                    ,   &g_AsclinShellInterface,       &EthPHYMode_Set       },
    {"ethphymsset"    ,   "     : Setup Ethernet PHY Master/Slave setting"                 ,   &g_AsclinShellInterface,       &EthPHYMS_Set         },

    //I2C1
#if GPIO_EXPANDER_ENABLE == 1
    {"showportexp", "  : Show the state of GPIO expansion port",   &g_AsclinShellInterface,       &Show_ExpPortState},
    {"showportexpall",": show all the state of GPIO expansion device",   &g_AsclinShellInterface,       &Show_ExpPortState_all},
    {"configportexp", ": Configure GPIO expansion port as input/output",   &g_AsclinShellInterface,       &Config_ExpPortState},
    {"setportexp", "   : Set output of GPIO expansion port to 0 or 1",   &g_AsclinShellInterface,       &SetOutput_ExpPortState},
#endif
    {"pgoodstatus", "  : Show Power Good Status",   &g_AsclinShellInterface,       &ShowPgood },
    {"loff", "         : stop continuous loop logging",   &g_AsclinShellInterface,       &Stop_continuous_logging},
    {"toff", "         : stop thermal logging",   &g_AsclinShellInterface,       &Stop_thermal_logging},
    {"ton", "          : enable thermal logging", &g_AsclinShellInterface,       &Enable_thermal_logging},
    {"rdtemp", "       : rdtemp [c/loop] [interval]:  read board temperature for [loop]\n\r               times or continuously with interval in between read", &g_AsclinShellInterface,       &ReadTemp},
    {"thermstatus", "  : Show thermal Status",   &g_AsclinShellInterface,       &ThermStatus},
    {"thermtest", "    : Test thermal sensor alert/shutdown feature",   &g_AsclinShellInterface,       &ThermTest},
    //=====================================================================================================
    //Cmds to PMIC, involve I2C1, SPI2, I2C0
    {"standby", "      : Switch TLF35584 to standby state",   &g_AsclinShellInterface,       &TLF_Standby},    
    {"showtlf", "      : Show the value of TLF register x",   &g_AsclinShellInterface,       &Show_TLF_Register},
    {"tlfwrite", "      : Write to the TLF register x",   &g_AsclinShellInterface,       &Write_TLF_Register},
    {"tlfwwdtest", "   : Test TLF Window Watchdog",   &g_AsclinShellInterface,       &TLF_Wwd_Test},
    //=====================================================================================================
    // Cmds to OTA
    {"switchtopfw", "  : Switch to Production fw",   &g_AsclinShellInterface,       &SwitchToProdFw,       },
    {"switchtoufw", "  : Switch to Update fw",   &g_AsclinShellInterface,       &SwitchToUpdFw,       },
/*test*/    {"pflashwrite", "  : test Pflash read/write/erase",   &g_AsclinShellInterface,       &pflash_write,   },
/*test*/    {"pflashread", "   : Pflash read sector",   &g_AsclinShellInterface,       &pflash_read,   },
/*test*/    {"ota", "          : enable/disable OTA",   &g_AsclinShellInterface,       &ota_config,       },
    //=====================================================================================================
    // CAN and FlexRay
    //I2C1{
    {"erayinit", "     : Initialize the Eray modules in transmit/receive/loopback mode",   &g_AsclinShellInterface,       &erayinit},
    {"eray",         "  : Run eRay normal mode",               &g_AsclinShellInterface,       &AppShell_eray,     },
    {"eraywakeup", "   : Send flexray wake sequence from FlexRay A",   &g_AsclinShellInterface,       &send_Eray_Wakeup},
    //}
    {"eraytest", "     : Test the Eray A to B or B to A connection",   &g_AsclinShellInterface,       &eraytest},
    {"eraysend", "     : Send values x and y to Eray channel A and B and sent by UDP",   &g_AsclinShellInterface,       &eraysend},
    {"eraytoudp", "     : Set Aurix as master/slave mode",   &g_AsclinShellInterface,       &eraytoudp},
    {"erayconfig", "  : Config eRay normal mode slot",      &g_AsclinShellInterface,       &AppShell_eray_config,     },
    {"eraytest", "  :    eRay debug",                             &g_AsclinShellInterface,       &AppShell_eray_test,     },
    //  {"eraystress", "   : Send flexray traffic continuously on FlexRay A and B",   &g_AsclinShellInterface,       &eraystress,  INT_MODE},
    //I2C1{
    {"caninit", "      : Initialize the Multican modules and nodes with baudrate x",   &g_AsclinShellInterface,       &multicaninit},
    {"cansetup", "      : Setup Multican modules and nodes with baudrate x after initialize",   &g_AsclinShellInterface,       &cansetup},
    {"showcansetup", "      : Show cansetup status",   &g_AsclinShellInterface,       &show_cansetup},
    {"canlp", "        : Put can node into low power mode",   &g_AsclinShellInterface,       &canlp},
    {"canwaketest", "  : repeatedly put can node in low power and display wakeup if received",   &g_AsclinShellInterface,       &CANWakeTest},
    {"canwakeup", "    : Send can wake sequence from CAN A",   &g_AsclinShellInterface,       &send_CAN_Wakeup},
    //}
    {"cansend", "      : Send values a and b on Multican node x with id",   &g_AsclinShellInterface,       &multicansend},
    {"canprintrx", "   : Show CANBUS RX message",   &g_AsclinShellInterface,       &set_canbus_rx_show},
    {"cancontinuous", "  : Multican node x send data continuously",   &g_AsclinShellInterface,       &multicansend_test},
    //should be ok, 1. check CAN standby pin or 2. HW measurement
    {"canst", "     : CAN stress test", &g_AsclinShellInterface,       &canstress_H},
    {"cancnt", "        : on/off CAN count log", &g_AsclinShellInterface,       &cancnt},
    {"cantoudp", "  : Test CAN UDP function",   &g_AsclinShellInterface,       &cantoudp},
    {"setudpip", "  : Set UDP Server IP",   &g_AsclinShellInterface,       &setudpip},
    {"canfilter", "  : Set Can filter ID", &g_AsclinShellInterface,       &canfilter},
    {"cangen",   " : Show recv count from the Main Aurix",        &g_AsclinShellInterface.shell, &Shell_cangateway_cangen},
    {"canmeasure",   " : measure latency",        &g_AsclinShellInterface.shell, &can_measure},
    {"canclear", "     : clear CAN rx count", &g_AsclinShellInterface,       &canclear},
    {"canthroughput", ": Test throughput and get result.", &g_AsclinShellInterface,       &canthroughput},

	//=====================================================================================================
    // Cmds to B2B functionality, invol
    // need gear I2C1
/*test*/    {"b2b", "          : board to board status and control", &g_AsclinShellInterface,       &B2b_cmd},
/*test*/    {"uartinit", "     : init the uart with Tegra",   &g_AsclinShellInterface.shell,       &UartCmd_Init},
/*test*/    {"uartsend", "     : send the uart data",   &g_AsclinShellInterface.shell,       &UartCmd_Send},
/*test*/    {"test", "     : send test function",   &g_AsclinShellInterface.shell,       &Test_Send},
    //=====================================================================================================
    // Cmds to trigger test scenarios
    {"dothermal", "    : do thermal test",   &g_AsclinShellInterface.shell,       &do_thermal},
    {"loadsim",   "switch loadsim set and clear",        &g_AsclinShellInterface.shell, &ethswitchloadsim},
    {"exposeaurix",   "switch expose aurix set and clear",        &g_AsclinShellInterface.shell, &ethswitchexposeaurix},
/*test*/    {"swpakcnt", ": display the how many packet is send or received on each switch ports",  &g_AsclinShellInterface,     &swpakcnt},
#if ETH_IPERF_TEST_ENABLE == 1
    {"iperf", "        : start the iperf bridge",   &g_AsclinShellInterface,       &ethernetiperf,       },
#endif
#if USE_WC_PATTERN == 1
    {"wc_pattern",   " : Worst Case Pattern with a high IPC Rate",        &g_AsclinShellInterface.shell, &Start_WC_Pattern},
#endif
    //=====================================================================================================
    // Cmds to AVB related
    {"geth", "  : Run gigabit ethernet test",   &g_AsclinShellInterface,       &AppShell_geth,     },
    {"can", "  : Send can packet",  &g_AsclinShellInterface,       &AppShell_can,     },
#if MAIN_AURIX_PTP_ENABLE
    {"ptp", "     : PTP commands",   &g_AsclinShellInterface,       &AppShell_ptp},
    {"ptpoff", "     : Turn off PTP",   &g_AsclinShellInterface,       &AppShell_ptpoff},
#endif
    //=====================================================================================================
    //no support any more
    //{"bootstrap", "    : Configure strap settings for boot medium (Not support now)",   &g_AsclinShellInterface.shell,       &SetBootStrap},
    //{"nonvlanconfig", ": Enable non VLAN tegra-Aurix communication and disable VLAN#200 on aurix port",  &g_AsclinShellInterface,     &nonVlanConfig},
    //=====================================================================================================
    // Cmds to show system status or information
    {"shellecho", "    : enable/disable the shell echo",   &g_AsclinShellInterface.shell,       &Shell_echo},
    {"version", "      : show the firmware version",   &g_AsclinShellInterface.shell,       &Shell_version},
    {"revision", "     : show the develope version",   &g_AsclinShellInterface.shell,       &Shell_revision},
    {"info",   "     : Show the welcome screen",   &g_AsclinShellInterface,       &AppShell_info,      },
    {"status", "   : Show the application status", &g_AsclinShellInterface,       &AppShell_status,    },
    {"help",   SHELL_HELP_DESCRIPTION_TEXT,        &g_AsclinShellInterface.shell, &Ifx_Shell_showHelp },
    IFX_SHELL_COMMAND_LIST_END
};



/** Shell Command for 2nd Aurix */
const Ifx_Shell_Command APPSHELL_COMMANDS_2ND_AURIX[] = {
    //=====================================================================================================
    //Cmds to from Aurix itself
    {"aurixreset", "   : reset Aurix",   &g_AsclinShellInterface,       &aurixrst},
    {"showport", "     : Show the state of Px.y",   &g_AsclinShellInterface,       &Show_PortState},
    {"showportall", "  : Show the state of Px.y",   &g_AsclinShellInterface,       &Show_PortState_all},
    {"setport", "      : Set the state of Px.y to state z (0/1)",   &g_AsclinShellInterface,       &Set_PortState},
    {"configport", "   : Configure Px.y to input or output",   &g_AsclinShellInterface,       &Config_Port},
    {"regrd", "        : regrd x [length]: read [length] starting from register address x",   &g_AsclinShellInterface, &Register_Read}, 
/*test*/   {"regwr", "        : regwr x value: write value to register address x",   &g_AsclinShellInterface, &Register_Write},
    {"showvoltages", " : Display all voltages",   &g_AsclinShellInterface,       &ShowVoltages},
    {"ping", "         : Send a ping to any address or to the gateway",   &g_AsclinShellInterface,       &ethernetping},
    {"netstat", "      : Show tatus of ethernet",   &g_AsclinShellInterface,       &ethernetstat},
    /* SPI */
    {"spislavewrite", "    : Use spi slave to exchange data with Tegra",   &g_AsclinShellInterface,       &spislavewrite},
    //=====================================================================================================
    //complex, involves all HW ex: system on/off (I2C0, I2C1, SPI 0~4)
    /* RGMII */
/*test*/   {"ethinit", "      : Initialize the Ethernet module and the Phy",   &g_AsclinShellInterface,       &ethernetinit},
    //=====================================================================================================
    // Cmds to OTA
    {"switchtopfw", "  : Switch to Production fw",   &g_AsclinShellInterface,       &SwitchToProdFw,       },
    {"switchtoufw", "  : Switch to Update fw",   &g_AsclinShellInterface,       &SwitchToUpdFw,       },
    {"ota", "          : enable/disable OTA",   &g_AsclinShellInterface,       &ota_config,       },
    //=====================================================================================================
    // CAN and FlexRay
    {"caninit", "      : Initialize the Multican modules and nodes with baudrate x",   &g_AsclinShellInterface,       &multicaninit},
    {"cansetup", "      : Setup Multican modules and nodes with baudrate x after initialize",   &g_AsclinShellInterface,       &cansetup},
    {"showcansetup", "      : Show cansetup status",   &g_AsclinShellInterface,       &show_cansetup},
    {"cansend", "      : Send values a and b on Multican node x with id",   &g_AsclinShellInterface,       &multicansend},
    {"canprintrx", "   : Show CANBUS RX message",   &g_AsclinShellInterface,       &set_canbus_rx_show},
    {"cancontinuous", "  : Multican node x send data continuously",   &g_AsclinShellInterface,       &multicansend_test},
    {"canlp", "        : Put can node into low power mode",   &g_AsclinShellInterface,       &canlp},
    {"canwaketest", "  : repeatedly put can node in low power and display wakeup if received",   &g_AsclinShellInterface,       &CANWakeTest},
    {"canwakeup", "    : Send can wake sequence from CAN A",   &g_AsclinShellInterface,       &send_CAN_Wakeup},
    {"canstress", "    : send can traffic continuously on CANA-CANF",   &g_AsclinShellInterface,       &canstress},
    {"canst", "     : CAN stress test", &g_AsclinShellInterface,       &canstress_H},
    {"cancnt", "        : on/off CAN count log", &g_AsclinShellInterface,       &cancnt},
    {"cantoudp", "  : Test CAN UDP function",   &g_AsclinShellInterface,       &cantoudp},
    {"setudpip", "  : Set UDP Host IP", &g_AsclinShellInterface,       &setudpip},
    {"canfilter", "  : Set Can filter ID", &g_AsclinShellInterface,       &canfilter},
    {"hssltest", "     : Run hssl test",   &g_AsclinShellInterface,       &hssltest},
    {"cangen",   " : Generate Can bus data from the Second Aurix",        &g_AsclinShellInterface.shell, &Shell_cangateway_cangen},
    //=====================================================================================================
    //Cmds to I2C0, I2C1
    {"i2cinit", "      : init i2c module",   &g_AsclinShellInterface,       &i2cinit},
    {"i2cscan", "      : Scan i2c module for devices",   &g_AsclinShellInterface,       &i2cscan},
    {"i2cread", "      : Read from i2c module x device y at offset z n bytes",   &g_AsclinShellInterface,       &i2cread},
    {"i2cwrite", "     : Write to i2c module x device y at offset z n bytes",   &g_AsclinShellInterface,       &i2cwrite},
    //Cmds to mainly for I2C devices
    //I2C0
    {"i2cromtest", "   : run the i2c read write test on ROM M24C02",   &g_AsclinShellInterface,       &i2cromtest},
    {"inforom", "      : Operation on InfoROM M24C02",   &g_AsclinShellInterface,       &i2cromrw},
    //I2C1
    {"loff", "         : stop continuous loop logging",   &g_AsclinShellInterface,       &Stop_continuous_logging},
    {"rdtemp", "       : rdtemp [c/loop] [interval]:  read board temperature for [loop]\n\r               times or continuously with interval in between read", &g_AsclinShellInterface,       &ReadTemp},
    //I2C1 + I2c0
    {"thermtest", "    : Test thermal sensor alert/shutdown feature",   &g_AsclinShellInterface,       &Aurix2ThermTest},
    //=====================================================================================================
    //Cmds to PMIC, involve I2C1, SPI2, I2C0
    {"showtlf", "      : Show the value of TLF register x",   &g_AsclinShellInterface,       &Show_TLF_Register},
    {"tlfwrite", "      : Write to the TLF register x",   &g_AsclinShellInterface,       &Write_TLF_Register},
    {"tlfwwdtest", "   : Test TLF Window Watchdog",   &g_AsclinShellInterface,       &TLF_Wwd_Test},
    //=====================================================================================================
    // Cmds to trigger test scenarios
    {"dothermal", "    : do thermal test",   &g_AsclinShellInterface.shell,       &do_thermal},
#if USE_WC_PATTERN == 1
    {"wc_pattern",   " : Worst Case Pattern with a high IPC Rate",        &g_AsclinShellInterface.shell, &Start_WC_Pattern},
#endif
    //=====================================================================================================
    // Cmds to AVB related
#if SECOND_AURIX_PTP_ENABLE
    {"ptp", "     : PTP commands",   &g_AsclinShellInterface,       &AppShell_ptp},
    {"ptpoff", "     : turn off PTP",   &g_AsclinShellInterface,       &AppShell_ptpoff},
#endif
#if FEATURE_MARVELL_6321
    {"rmu","      : Aurix2 controls 6321 via RMU",   &g_AsclinShellInterface,       &AppShell_rmu},
#endif
    //=====================================================================================================
    // Cmds to show system status or information
    {"status", "       : Show the application status", &g_AsclinShellInterface,       &AppShell_status},
    {"version", "      : show the firmware version",   &g_AsclinShellInterface.shell,       &Shell_version},
    {"revision", "     : show the develope version",   &g_AsclinShellInterface.shell,       &Shell_revision},
    {"help",   SHELL_HELP_DESCRIPTION_TEXT,        &g_AsclinShellInterface.shell, &Ifx_Shell_showHelp},
    IFX_SHELL_COMMAND_LIST_END
};


#else

const Ifx_Shell_Command APPSHELL_COMMANDS_MAIN_AURIX[] = {
    //=====================================================================================================
    //Cmds to from Aurix itself
    {"rdaurixreg"       , ": Read aurix internal reg by keyword"                                        , &g_AsclinShellInterface.shell    , &Register_Read_keyword      , USER_TYPE_ADMINISTRATOR},
    {"tegrarecovery"    , ": Set Tegra X1/X2/CVM in recovery mode"                                      , &g_AsclinShellInterface          , &TegraRecovery              , USER_TYPE_ADMINISTRATOR},
    {"tegrareset"       , "   : Reset Tegra X1/X2/CVM"                                                  , &g_AsclinShellInterface          , &TegraReset                 , USER_TYPE_ADMINISTRATOR},
    {"showport"         , "     : Show the state of Px.y"                                               , &g_AsclinShellInterface          , &Show_PortState             , USER_TYPE_USER         },
    {"showportall"      , "     : Show the state of Px.y"                                               , &g_AsclinShellInterface          , &Show_PortState_all         , USER_TYPE_ADMINISTRATOR},
    {"setport"          , "      : Set the state of Px.y to state z (0/1)"                              , &g_AsclinShellInterface          , &Set_PortState              , USER_TYPE_ADMINISTRATOR},
    {"configport"       , "   : Configure Px.y to input or output"                                      , &g_AsclinShellInterface          , &Config_Port                , USER_TYPE_ADMINISTRATOR},
    {"regrd"            , "        : regrd x [length]: read [length] starting from register address x"  , &g_AsclinShellInterface          , &Register_Read              , USER_TYPE_ADMINISTRATOR},
    {"regwr"            , "        : regwr x value: write value to register address x"                  , &g_AsclinShellInterface          , &Register_Write             , USER_TYPE_ADMINISTRATOR},
    {"showan"           , "       : Show the value(s) of ANx [to y]"                                    , &g_AsclinShellInterface          , &Show_ANx_y                 , USER_TYPE_ADMINISTRATOR},
    {"showvoltages"     , " : Display all voltages"                                                     , &g_AsclinShellInterface          , &ShowVoltages               , USER_TYPE_ADMINISTRATOR},
    {"showfan"          , "      : show the fan RPM"                                                    , &g_AsclinShellInterface          , &Show_FanRPM                , USER_TYPE_ADMINISTRATOR},
    {"ping"             , "         : Send a ping to any address or to the gateway"                     , &g_AsclinShellInterface          , &ethernetping               , USER_TYPE_USER         },
    {"netstat"          , "      : Show status of ethernet"                                             , &g_AsclinShellInterface          , &ethernetstat               , USER_TYPE_ADMINISTRATOR},
    {"mdioinit"         , "     : read PHY/Switch Register through MDIO/ interface"                     , &g_AsclinShellInterface          , &mdioinit                   , USER_TYPE_ADMINISTRATOR},
    {"aurix2reset"      , "   : Reset Aurix2"                                                           , &g_AsclinShellInterface          , &Aurix2Reset                , USER_TYPE_USER         },
    {"aurix2recovery"   , "   : Set Aurix2 in recovery mode"                                            , &g_AsclinShellInterface          , &Aurix2Recovery             , USER_TYPE_ADMINISTRATOR},
    {"kl30config"       , ": kl30config"                                                                , &g_AsclinShellInterface.shell    , &System_Monitor_Config_Cmd  , USER_TYPE_ADMINISTRATOR},
    //=====================================================================================================
    //Cmds to handshake with other devices
    {"hssltest"         , "     : Run hssl test"                                                        , &g_AsclinShellInterface          , &hssltest                   , USER_TYPE_ADMINISTRATOR},
    {"tegrainit"        , "    : Initialize the Qspi module for Tegra"                                  , &g_AsclinShellInterface          , &tegraqspiinit              , USER_TYPE_ADMINISTRATOR},
    {"tegrawr"          , "      : Write and read tegra X1/X2/CVM with n bytes with master SPI"         , &g_AsclinShellInterface          , &tegrawriteread             , USER_TYPE_ADMINISTRATOR},
    {"b2brun"           , "          : show temperature of board to board"                              , &g_AsclinShellInterface          , &B2b_run                    , USER_TYPE_USER         },
    //{"b2bcmd"         , "          : send command to board to board"                                  , &g_AsclinShellInterface          , &b2b_cmd_nc                 ,},
    #if DEBUG_ENABLE == 1
    /* it's used with old mods test, should be abandon but we will keep it here for now */
    {"spimastertest"    , ": test with tegra A(/B) with n bytes with master SPI"                        , &g_AsclinShellInterface          , &spimastertest              , USER_TYPE_ADMINISTRATOR},
    #endif
    //=====================================================================================================
    //complex, involves all HW ex: system on/off (I2C0, I2C1, SPI 0~4)
    {"poweroff"         , "     : Set the outputs to system power off state"                            , &g_AsclinShellInterface          , &System_PowerOff_Cmd        , USER_TYPE_USER         },
    {"poweron"          , "      : Turn main power on and release Tegra resets"                         , &g_AsclinShellInterface          , &System_PowerOn_Cmd         , USER_TYPE_USER         },
    {"aurix2flash"      , "   : WAR: Prepare Aurix2 into recovery mode"                                 , &g_AsclinShellInterface          , &Aurix2Flash                , USER_TYPE_USER         },
    {"aurixreset"       , "   : reset Aurix"                                                            , &g_AsclinShellInterface          , &aurixrst                   , USER_TYPE_USER         },
    //with I2C rom, ethernet init
/*test*/    {"ethinit"  , "      : Initialize the Ethernet module and the Phy"                          , &g_AsclinShellInterface          , &ethernetinit               , USER_TYPE_ADMINISTRATOR},
    //=====================================================================================================
    //Cmds to involve I2C1 and MDIO
    {"mdioset"          , "      : set mdio logic to access specific device"                            , &g_AsclinShellInterface          , &mdioset                    , USER_TYPE_ADMINISTRATOR},
    {"mdiord"           , "       : read PHY/Switch Register through MDIO/ interface"                   , &g_AsclinShellInterface          , &mdiord                     , USER_TYPE_ADMINISTRATOR},
    {"mdiowr"           , "       : write value to PHY/Switch Register through MDIO interface"          , &g_AsclinShellInterface          , &mdiowr                     , USER_TYPE_ADMINISTRATOR},
#if ETH_SWITCH_ENABLE == 1
    {"rdswphy"          , "      : read PHY/Switch Register"                                            , &g_AsclinShellInterface          , &readSWitchPhy              , USER_TYPE_ADMINISTRATOR},
    {"wrswphy"          , "      : write to PHY/Switch Register"                                        , &g_AsclinShellInterface          , &writeSWitchPhy             , USER_TYPE_ADMINISTRATOR},
#endif
#if ETH_PHY_88Q2112_ENABLE == 1
    {"ethphyinit"       , "    : init various ethernet phy"                                             , &g_AsclinShellInterface.shell    , &ethphyinit                 , USER_TYPE_ADMINISTRATOR},
    {"ethphyrd"         , "      : read PHY Register"                                                   , &g_AsclinShellInterface          , &ethphyread                 , USER_TYPE_ADMINISTRATOR},
    {"ethphywr"         , "      : write to PHY Register"                                               , &g_AsclinShellInterface          , &ethphywrite                , USER_TYPE_ADMINISTRATOR},
#endif
    //Cmds to involve SPI, I2C1 and MDIO
     //spi and i2c1
    {"ethswrd"          , "      : read the ethernet switch register"                                   , &g_AsclinShellInterface          , &ethswitchread              , USER_TYPE_USER         },
    {"ethswwr"          , "      : write the ethernet switch register"                                  , &g_AsclinShellInterface          , &ethswitchwrite             , USER_TYPE_USER         },
    //=====================================================================================================
    //Cmds to I2C0, I2C1
    {"i2cinit"          , "      : init i2c module"                                                     , &g_AsclinShellInterface          , &i2cinit                    , USER_TYPE_ADMINISTRATOR},
    {"i2cscan"          , "      : Scan i2c module for devices"                                         , &g_AsclinShellInterface          , &i2cscan                    , USER_TYPE_ADMINISTRATOR},
    {"i2cread"          , "      : Read from i2c module x device y at offset z n bytes"                 , &g_AsclinShellInterface          , &i2cread                    , USER_TYPE_ADMINISTRATOR},
    {"i2cwrite"         , "     : Write to i2c module x device y at offset z n bytes"                   , &g_AsclinShellInterface          , &i2cwrite                   , USER_TYPE_ADMINISTRATOR},
    //=====================================================================================================
    //Cmds to mainly for I2C devices
    //I2C0
    {"cimpower"         , "     : turn on/off Camera Interface Module"                                  , &g_AsclinShellInterface          , &CIM_Power_Control_Cmd      , USER_TYPE_ADMINISTRATOR},
    {"cimcampower"      , "  : turn on/off Camera Unit LSW power"                                       , &g_AsclinShellInterface          , &CIM_LSW_Control_Cmd        , USER_TYPE_ADMINISTRATOR},
    {"cimaggpower"      , "  : turn on/off Camera Group Aggregator"                                     , &g_AsclinShellInterface          , &CIM_Aggregator_Control_Cmd , USER_TYPE_ADMINISTRATOR},
    {"cimfsync"         , "     : control the Camera frame synce"                                       , &g_AsclinShellInterface          , &CIM_FrameSync_Control_Cmd  , USER_TYPE_ADMINISTRATOR},
    {"cimseatgood"      , "  : Show CIM Presence and seat good status"                                  , &g_AsclinShellInterface          , &CIM_SGood_Status_Cmd       , USER_TYPE_ADMINISTRATOR},
    {"cimpowertest"      , "  : turn on/off all CIM power for test"                                  , &g_AsclinShellInterface          , &CIM_Power_Test_Cmd       , USER_TYPE_ADMINISTRATOR},
    {"i2cromtest"       , "   : run the i2c read write test on ROM M24C02"                              , &g_AsclinShellInterface          , &i2cromtest                 , USER_TYPE_ADMINISTRATOR},
    {"inforom"          , "      : Operation on InfoROM M24C02"                                         , &g_AsclinShellInterface          , &i2cromrw                   , USER_TYPE_ADMINISTRATOR},
    {"rdpower"          , "      : rdpower [c/loop] [interval]:  read board power for [loop] times\n\r or continuously with interval in between read", &g_AsclinShellInterface, &ReadPower, USER_TYPE_USER         },
    {"vmonstatus"       , "   : Print the status of VMONs"                                              , &g_AsclinShellInterface          , &Vmon_showStatus            , USER_TYPE_USER         },
    {"readcamvolt"      , "     : read camera voltage"                                                  , &g_AsclinShellInterface          , &ReadCamVolt                , USER_TYPE_ADMINISTRATOR},
    {"readcamcurr"      , "     : read camera current"                                                  , &g_AsclinShellInterface          , &ReadCamCurr                , USER_TYPE_ADMINISTRATOR},
    {"readcamvoltcurr"  , "     : read camera current"                                                  , &g_AsclinShellInterface          , &ReadCamVolCurr             , USER_TYPE_ADMINISTRATOR},
    {"ignchksta"        , "     : Read Ignition Check Feature Enablement setting"                       , &g_AsclinShellInterface          , &IgnitionChkEngage_Sta      , USER_TYPE_USER         },
    {"ignchkset"        , "     : Setup Ignition Check Feature Enablement setting"                      , &g_AsclinShellInterface          , &IgnitionChkEngage_Set      , USER_TYPE_USER         },
    {"ethphysta"        , "     : Read Ethernet PHY work mode & Master/Slave setting"                   , &g_AsclinShellInterface          , &EthPHYModeMS_Sta           , USER_TYPE_USER         },
    {"ethphymodeset"    , "     : Setup Ethernet PHY work mode setting"                                 , &g_AsclinShellInterface          , &EthPHYMode_Set             , USER_TYPE_ADMINISTRATOR},
    {"ethphymsset"      , "     : Setup Ethernet PHY Master/Slave setting"                              , &g_AsclinShellInterface          , &EthPHYMS_Set               , USER_TYPE_USER         },

    //I2C1
#if GPIO_EXPANDER_ENABLE == 1
    {"showportexp"      , "  : Show the state of GPIO expansion port"                                   , &g_AsclinShellInterface          , &Show_ExpPortState          , USER_TYPE_ADMINISTRATOR},
    {"showportexpall"   , ": show all the state of GPIO expansion device"                               , &g_AsclinShellInterface          , &Show_ExpPortState_all      , USER_TYPE_ADMINISTRATOR},
    {"configportexp"    , ": Configure GPIO expansion port as input/output"                             , &g_AsclinShellInterface          , &Config_ExpPortState        , USER_TYPE_ADMINISTRATOR},
    {"setportexp"       , "   : Set output of GPIO expansion port to 0 or 1"                            , &g_AsclinShellInterface          , &SetOutput_ExpPortState     , USER_TYPE_ADMINISTRATOR},
#endif
    {"pgoodstatus"      , "  : Show Power Good Status"                                                  , &g_AsclinShellInterface          , &ShowPgood                  , USER_TYPE_USER         },
    {"loff"             , "         : stop continuous loop logging"                                     , &g_AsclinShellInterface          , &Stop_continuous_logging    , USER_TYPE_ADMINISTRATOR},
    {"toff"             , "         : stop thermal logging"                                             , &g_AsclinShellInterface          , &Stop_thermal_logging       , USER_TYPE_ADMINISTRATOR},
    {"ton"              , "          : enable thermal logging"                                          , &g_AsclinShellInterface          , &Enable_thermal_logging     , USER_TYPE_ADMINISTRATOR},
    {"rdtemp"           , "       : rdtemp [c/loop] [interval]:  read board temperature for [loop]\n\r               times or continuously with interval in between read", &g_AsclinShellInterface, &ReadTemp, USER_TYPE_ADMINISTRATOR},
    {"thermstatus"      , "  : Show thermal Status"                                                     , &g_AsclinShellInterface          , &ThermStatus                , USER_TYPE_USER         },
    {"thermtest"        , "    : Test thermal sensor alert/shutdown feature"                            , &g_AsclinShellInterface          , &ThermTest                  , USER_TYPE_ADMINISTRATOR},
    //=====================================================================================================
    //Cmds to PMIC, involve I2C1, SPI2, I2C0
    {"standby"          , "      : Switch TLF35584 to standby state"                                    , &g_AsclinShellInterface          , &TLF_Standby                , USER_TYPE_USER         },
    {"showtlf"          , "      : Show the value of TLF register x"                                    , &g_AsclinShellInterface          , &Show_TLF_Register          , USER_TYPE_ADMINISTRATOR},
    {"tlfwrite"         , "      : Write to the TLF register x"                                         , &g_AsclinShellInterface          , &Write_TLF_Register         , USER_TYPE_ADMINISTRATOR},
    {"tlfwwdtest"       , "   : Test TLF Window Watchdog"                                               , &g_AsclinShellInterface          , &TLF_Wwd_Test               , USER_TYPE_ADMINISTRATOR},
    //=====================================================================================================
    // Cmds to OTA
    {"switchtopfw"      , "  : Switch to Production fw"                                                 , &g_AsclinShellInterface          , &SwitchToProdFw             , USER_TYPE_USER         },
    {"switchtoufw"      , "  : Switch to Update fw"                                                     , &g_AsclinShellInterface          , &SwitchToUpdFw              , USER_TYPE_USER         },
/*test*/    {"pflashwrite", "  : test Pflash read/write/erase"                                          , &g_AsclinShellInterface          , &pflash_write               , USER_TYPE_ADMINISTRATOR},
/*test*/    {"pflashread" , "   : Pflash read sector"                                                   , &g_AsclinShellInterface          , &pflash_read                , USER_TYPE_ADMINISTRATOR},
/*test*/    {"ota"        , "          : enable/disable OTA"                                            , &g_AsclinShellInterface          , &ota_config                 , USER_TYPE_ADMINISTRATOR},
    //=====================================================================================================
    // CAN and FlexRay
    //I2C1{
    {"erayinit"         , "     : Initialize the Eray modules in transmit/receive/loopback mode"        , &g_AsclinShellInterface          , &erayinit                   , USER_TYPE_USER         },
    {"eray"             , "  : Run eRay normal mode"                                                    , &g_AsclinShellInterface          , &AppShell_eray              , USER_TYPE_USER         },
    {"eraywakeup"       , "   : Send flexray wake sequence from FlexRay A"                              , &g_AsclinShellInterface          , &send_Eray_Wakeup           , USER_TYPE_ADMINISTRATOR},
    //}
    {"eraytest"         , "     : Test the Eray A to B or B to A connection"                            , &g_AsclinShellInterface          , &eraytest                   , USER_TYPE_ADMINISTRATOR},
    {"eraysend"         , "     : Send values x and y to Eray channel A and B and sent by UDP"          , &g_AsclinShellInterface          , &eraysend                   , USER_TYPE_USER         },
    {"eraytoudp"        , "     : Set Aurix as master/slave mode"                                       , &g_AsclinShellInterface          , &eraytoudp                  , USER_TYPE_ADMINISTRATOR},
    {"erayconfig"       , "  : Config eRay normal mode slot"                                            , &g_AsclinShellInterface          , &AppShell_eray_config       , USER_TYPE_ADMINISTRATOR},
    {"eraytest"         , "  :    eRay debug"                                                           , &g_AsclinShellInterface          , &AppShell_eray_test         , USER_TYPE_USER         },
    //  {"eraystress"   , "   : Send flexray traffic continuously on FlexRay A and B"                   , &g_AsclinShellInterface          , &eraystress,  INT_MODE      ,},
    //I2C1{
    {"caninit"          , "      : Initialize the Multican modules and nodes with baudrate x"           , &g_AsclinShellInterface          , &multicaninit               , USER_TYPE_USER         },
    {"cansetup"         , "      : Setup Multican modules and nodes with baudrate x after initialize"   , &g_AsclinShellInterface          , &cansetup                   , USER_TYPE_USER         },
    {"showcansetup"     , "      : Show cansetup status"                                                , &g_AsclinShellInterface          , &show_cansetup              , USER_TYPE_USER         },
    {"canlp"            , "        : Put can node into low power mode"                                  , &g_AsclinShellInterface          , &canlp                      , USER_TYPE_ADMINISTRATOR},
    {"canwaketest"      , "  : repeatedly put can node in low power and display wakeup if received"     , &g_AsclinShellInterface          , &CANWakeTest                , USER_TYPE_ADMINISTRATOR},
    {"canwakeup"        , "    : Send can wake sequence from CAN A"                                     , &g_AsclinShellInterface          , &send_CAN_Wakeup            , USER_TYPE_ADMINISTRATOR},
    //}
    {"cansend"          , "      : Send values a and b on Multican node x with id"                      , &g_AsclinShellInterface          , &multicansend               , USER_TYPE_USER         },
    {"canprintrx"       , "   : Show CANBUS RX message"                                                 , &g_AsclinShellInterface          , &set_canbus_rx_show         , USER_TYPE_USER         },
    {"cancontinuous"    , "  : Multican node x send data continuously"                                  , &g_AsclinShellInterface          , &multicansend_test          , USER_TYPE_ADMINISTRATOR},
    //should be ok, 1. check CAN standby pin or 2. HW measurement
    {"canst"            , "     : CAN stress test"                                                      , &g_AsclinShellInterface          , &canstress_H                , USER_TYPE_ADMINISTRATOR},
    {"cancnt"           , "        : on/off CAN count log"                                              , &g_AsclinShellInterface          , &cancnt                     , USER_TYPE_ADMINISTRATOR},
    {"cantoudp"         , "  : Test CAN UDP function"                                                   , &g_AsclinShellInterface          , &cantoudp                   , USER_TYPE_USER         },
    {"setudpip"         , "  : Set UDP Server IP"                                                       , &g_AsclinShellInterface          , &setudpip                   , USER_TYPE_USER         },
    {"canfilter"        , "  : Set Can filter ID"                                                       , &g_AsclinShellInterface          , &canfilter                  , USER_TYPE_USER         },
    {"cangen"           , " : Show recv count from the Main Aurix"                                      , &g_AsclinShellInterface.shell    , &Shell_cangateway_cangen    , USER_TYPE_USER         },
    {"canmeasure"       , " : measure latency"                                                          , &g_AsclinShellInterface.shell    , &can_measure                , USER_TYPE_ADMINISTRATOR},
    {"canclear"         , "     : clear CAN rx count"                                                   , &g_AsclinShellInterface          , &canclear                   , USER_TYPE_ADMINISTRATOR},
    {"canthroughput"    , ": Test throughput and get result."                                           , &g_AsclinShellInterface          , &canthroughput              , USER_TYPE_USER         },
    {"cantestcase"     , " : Test can case"                                                        , &g_AsclinShellInterface          , &cantestcase                , USER_TYPE_ADMINISTRATOR},
    //=====================================================================================================
    // Cmds to B2B functionality, invol
    // need gear I2C1
/*test*/    {"b2b"      , "          : board to board status and control"                               , &g_AsclinShellInterface          , &B2b_cmd                    , USER_TYPE_USER         },
/*test*/    {"uartinit" , "     : init the uart with Tegra"                                             , &g_AsclinShellInterface.shell    , &UartCmd_Init               , USER_TYPE_USER         },
/*test*/    {"uartsend" , "     : send the uart data"                                                   , &g_AsclinShellInterface.shell    , &UartCmd_Send               , USER_TYPE_USER         },
/*test*/    {"test"     , "     : send test function"                                                   , &g_AsclinShellInterface.shell    , &Test_Send                  , USER_TYPE_ADMINISTRATOR},
    //=====================================================================================================
    // Cmds to trigger test scenarios
    {"dothermal"        , "    : do thermal test"                                                       , &g_AsclinShellInterface.shell    , &do_thermal                 , USER_TYPE_ADMINISTRATOR},
    {"loadsim"          , ": switch loadsim set and clear"                                              , &g_AsclinShellInterface.shell    , &ethswitchloadsim           , USER_TYPE_ADMINISTRATOR},
    {"exposeaurix"      , ": switch expose aurix set and clear"                                         , &g_AsclinShellInterface.shell    , &ethswitchexposeaurix       , USER_TYPE_USER         },
/*test*/    {"swpakcnt" , ": display the how many packet is send or received on each switch ports"      , &g_AsclinShellInterface          , &swpakcnt                   , USER_TYPE_ADMINISTRATOR},
#if ETH_IPERF_TEST_ENABLE == 1
    {"iperf"            , "        : start the iperf bridge"                                            , &g_AsclinShellInterface          , &ethernetiperf              , USER_TYPE_ADMINISTRATOR},
#endif
#if USE_WC_PATTERN == 1
    {"wc_pattern"       , " : Worst Case Pattern with a high IPC Rate"                                  , &g_AsclinShellInterface.shell    , &Start_WC_Pattern           , USER_TYPE_ADMINISTRATOR},
#endif
    //=====================================================================================================
    // Cmds to AVB related
    {"geth"             , "  : Run gigabit ethernet test"                                               , &g_AsclinShellInterface          , &AppShell_geth              , USER_TYPE_ADMINISTRATOR},
    {"can"              , "  : Send can packet"                                                         , &g_AsclinShellInterface          , &AppShell_can               , USER_TYPE_ADMINISTRATOR},
#if MAIN_AURIX_PTP_ENABLE
    {"ptp"              , "     : PTP commands"                                                         , &g_AsclinShellInterface          , &AppShell_ptp               , USER_TYPE_USER         },
    {"ptpon"            , "     : Turn on PTP"                                                          , &g_AsclinShellInterface          , &AppShell_ptpon            , USER_TYPE_USER         },
#endif
    {"switchtool"      , "    : read/write marvell 5050"                                                , &g_AsclinShellInterface.shell    , &Marvell_5050_tool          , USER_TYPE_ADMINISTRATOR},
    //=====================================================================================================
    //no support any more
    //{"bootstrap"      , "    : Configure strap settings for boot medium (Not support now)"            , &g_AsclinShellInterface.shell    , &SetBootStrap               ,},
    //{"nonvlanconfig"  , ": Enable non VLAN tegra-Aurix communication and disable VLAN#200 on aurix port", &g_AsclinShellInterface        , &nonVlanConfig              ,},
    //=====================================================================================================
    // Cmds to show system status or information
    {"shellecho"        , "    : enable/disable the shell echo"                                         , &g_AsclinShellInterface.shell    , &Shell_echo                 , USER_TYPE_USER         },
    {"shelludpecho"     , "    : enable/disable UDP command echo"                                       , &g_AsclinShellInterface.shell    , &Shell_udp_echo             , USER_TYPE_ADMINISTRATOR},
    {"showmenuall"      , "    : enable/disable to show all menu items"                                 , &g_AsclinShellInterface.shell    , &Show_Menu_All              , USER_TYPE_ADMINISTRATOR},
    {"version"          , "      : show the firmware version"                                           , &g_AsclinShellInterface.shell    , &Shell_version              , USER_TYPE_USER         },
    {"revision"         , "     : show the develope version"                                            , &g_AsclinShellInterface.shell    , &Shell_revision             , USER_TYPE_USER         },
    {"info"             , "     : Show the welcome screen"                                              , &g_AsclinShellInterface          , &AppShell_info              , USER_TYPE_USER         },
    {"status"           , "   : Show the application status"                                            , &g_AsclinShellInterface          , &AppShell_status            , USER_TYPE_USER         },
    {"help"             , SHELL_HELP_DESCRIPTION_TEXT                                                   , &g_AsclinShellInterface.shell    , &Ifx_Shell_showHelp         , USER_TYPE_USER         },
    IFX_SHELL_COMMAND_LIST_END
};



/** Shell Command for 2nd Aurix */
const Ifx_Shell_Command APPSHELL_COMMANDS_2ND_AURIX[] = {
    //=====================================================================================================
    //Cmds to from Aurix itself
    {"aurixreset"       , "   : reset Aurix"                                                            , &g_AsclinShellInterface          , &aurixrst                   , USER_TYPE_USER         },
    {"showport"         , "     : Show the state of Px.y"                                               , &g_AsclinShellInterface          , &Show_PortState             , USER_TYPE_USER         },
    {"showportall"      , "  : Show the state of Px.y"                                                  , &g_AsclinShellInterface          , &Show_PortState_all         , USER_TYPE_USER         },
    {"setport"          , "      : Set the state of Px.y to state z (0/1)"                              , &g_AsclinShellInterface          , &Set_PortState              , USER_TYPE_ADMINISTRATOR},
    {"configport"       , "   : Configure Px.y to input or output"                                      , &g_AsclinShellInterface          , &Config_Port                , USER_TYPE_ADMINISTRATOR},
    {"regrd"            , "        : regrd x [length]: read [length] starting from register address x"  , &g_AsclinShellInterface          , &Register_Read              , USER_TYPE_ADMINISTRATOR},
/*test*/   {"regwr"     , "        : regwr x value: write value to register address x"                  , &g_AsclinShellInterface          , &Register_Write             , USER_TYPE_ADMINISTRATOR},
    {"showvoltages"     , " : Display all voltages"                                                     , &g_AsclinShellInterface          , &ShowVoltages               , USER_TYPE_ADMINISTRATOR},
    {"ping"             , "         : Send a ping to any address or to the gateway"                     , &g_AsclinShellInterface          , &ethernetping               , USER_TYPE_USER         },
    {"netstat"          , "      : Show tatus of ethernet"                                              , &g_AsclinShellInterface          , &ethernetstat               , USER_TYPE_ADMINISTRATOR},
    /* SPI */
    {"spislavewrite"    , "    : Use spi slave to exchange data with Tegra"                             , &g_AsclinShellInterface          , &spislavewrite              , USER_TYPE_ADMINISTRATOR},
    //=====================================================================================================
    //complex, involves all HW ex: system on/off (I2C0, I2C1, SPI 0~4)
    /* RGMII */
/*test*/   {"ethinit"   , "      : Initialize the Ethernet module and the Phy"                          , &g_AsclinShellInterface          , &ethernetinit               , USER_TYPE_ADMINISTRATOR},
    //=====================================================================================================
    // Cmds to OTA
    {"switchtopfw"      , "  : Switch to Production fw"                                                 , &g_AsclinShellInterface          , &SwitchToProdFw             , USER_TYPE_USER         },
    {"switchtoufw"      , "  : Switch to Update fw"                                                     , &g_AsclinShellInterface          , &SwitchToUpdFw              , USER_TYPE_USER         },
    {"ota"              , "          : enable/disable OTA"                                              , &g_AsclinShellInterface          , &ota_config                 , USER_TYPE_ADMINISTRATOR},
    //=====================================================================================================
    // CAN and FlexRay
    {"caninit"          , "      : Initialize the Multican modules and nodes with baudrate x"           , &g_AsclinShellInterface          , &multicaninit               , USER_TYPE_USER         },
    {"cansetup"         , "      : Setup Multican modules and nodes with baudrate x after initialize"   , &g_AsclinShellInterface          , &cansetup                   , USER_TYPE_USER         },
    {"showcansetup"     , "      : Show cansetup status"                                                , &g_AsclinShellInterface          , &show_cansetup              , USER_TYPE_USER         },
    {"cansend"          , "      : Send values a and b on Multican node x with id"                      , &g_AsclinShellInterface          , &multicansend               , USER_TYPE_USER         },
    {"canprintrx"       , "   : Show CANBUS RX message"                                                 , &g_AsclinShellInterface          , &set_canbus_rx_show         , USER_TYPE_USER         },
    {"cancontinuous"    , "  : Multican node x send data continuously"                                  , &g_AsclinShellInterface          , &multicansend_test          , USER_TYPE_ADMINISTRATOR},
    {"canlp"            , "        : Put can node into low power mode"                                  , &g_AsclinShellInterface          , &canlp                      , USER_TYPE_ADMINISTRATOR},
    {"canwaketest"      , "  : repeatedly put can node in low power and display wakeup if received"     , &g_AsclinShellInterface          , &CANWakeTest                , USER_TYPE_ADMINISTRATOR},
    {"canwakeup"        , "    : Send can wake sequence from CAN A"                                     , &g_AsclinShellInterface          , &send_CAN_Wakeup            , USER_TYPE_ADMINISTRATOR},
    {"canstress"        , "    : send can traffic continuously on CANA-CANF"                            , &g_AsclinShellInterface          , &canstress                  , USER_TYPE_ADMINISTRATOR},
    {"canst"            , "     : CAN stress test"                                                      , &g_AsclinShellInterface          , &canstress_H                , USER_TYPE_ADMINISTRATOR},
    {"cancnt"           , "        : on/off CAN count log"                                              , &g_AsclinShellInterface          , &cancnt                     , USER_TYPE_ADMINISTRATOR},
    {"cantoudp"         , "  : Test CAN UDP function"                                                   , &g_AsclinShellInterface          , &cantoudp                   , USER_TYPE_USER         },
    {"setudpip"         , "  : Set UDP Host IP"                                                         , &g_AsclinShellInterface          , &setudpip                   , USER_TYPE_USER         },
    {"canfilter"        , "  : Set Can filter ID"                                                       , &g_AsclinShellInterface          , &canfilter                  , USER_TYPE_USER         },
    {"hssltest"         , "     : Run hssl test"                                                        , &g_AsclinShellInterface          , &hssltest                   , USER_TYPE_ADMINISTRATOR},
    {"cangen"           ,   " : Generate Can bus data from the Second Aurix"                            , &g_AsclinShellInterface.shell    , &Shell_cangateway_cangen    , USER_TYPE_USER         },
	{"cantestcase"     , " : Test can case"                                                        , &g_AsclinShellInterface          , &cantestcase                , USER_TYPE_ADMINISTRATOR},
	//=====================================================================================================
    //Cmds to I2C0, I2C1
    {"i2cinit"          , "      : init i2c module"                                                     , &g_AsclinShellInterface          , &i2cinit                    , USER_TYPE_ADMINISTRATOR},
    {"i2cscan"          , "      : Scan i2c module for devices"                                         , &g_AsclinShellInterface          , &i2cscan                    , USER_TYPE_ADMINISTRATOR},
    {"i2cread"          , "      : Read from i2c module x device y at offset z n bytes"                 , &g_AsclinShellInterface          , &i2cread                    , USER_TYPE_ADMINISTRATOR},
    {"i2cwrite"         , "     : Write to i2c module x device y at offset z n bytes"                   , &g_AsclinShellInterface          , &i2cwrite                   , USER_TYPE_ADMINISTRATOR},
    //Cmds to mainly for I2C devices
    //I2C0
    {"i2cromtest"       , "   : run the i2c read write test on ROM M24C02"                              , &g_AsclinShellInterface          , &i2cromtest                 , USER_TYPE_ADMINISTRATOR},
    {"inforom"          , "      : Operation on InfoROM M24C02"                                         , &g_AsclinShellInterface          , &i2cromrw                   , USER_TYPE_ADMINISTRATOR},
    //I2C1
    {"loff"             , "         : stop continuous loop logging"                                     , &g_AsclinShellInterface          , &Stop_continuous_logging    , USER_TYPE_ADMINISTRATOR},
    {"rdtemp"           , "       : rdtemp [c/loop] [interval]:  read board temperature for [loop]\n\r               times or continuously with interval in between read", &g_AsclinShellInterface, &ReadTemp, USER_TYPE_USER         },
    //I2C1 + I2c0
    {"thermtest"        , "    : Test thermal sensor alert/shutdown feature"                            , &g_AsclinShellInterface          , &Aurix2ThermTest            , USER_TYPE_ADMINISTRATOR},
    //=====================================================================================================
    //Cmds to PMIC, involve I2C1, SPI2, I2C0
    {"showtlf"          , "      : Show the value of TLF register x"                                    , &g_AsclinShellInterface          , &Show_TLF_Register          , USER_TYPE_ADMINISTRATOR},
    {"tlfwrite"         , "      : Write to the TLF register x"                                         , &g_AsclinShellInterface          , &Write_TLF_Register         , USER_TYPE_ADMINISTRATOR},
    {"tlfwwdtest"       , "   : Test TLF Window Watchdog"                                               , &g_AsclinShellInterface          , &TLF_Wwd_Test               , USER_TYPE_ADMINISTRATOR},
    //=====================================================================================================
    // Cmds to trigger test scenarios
    {"dothermal"        , "    : do thermal test"                                                       , &g_AsclinShellInterface.shell    , &do_thermal                 , USER_TYPE_ADMINISTRATOR},
#if USE_WC_PATTERN == 1
    {"wc_pattern"       , " : Worst Case Pattern with a high IPC Rate"                                  , &g_AsclinShellInterface.shell    , &Start_WC_Pattern           , USER_TYPE_ADMINISTRATOR},
#endif
    //=====================================================================================================
    // Cmds to AVB related
#if SECOND_AURIX_PTP_ENABLE
    {"ptp"              , "     : PTP commands"                                                         , &g_AsclinShellInterface          , &AppShell_ptp               , USER_TYPE_USER         },
    {"ptpon"            , "     : turn on PTP"                                                          , &g_AsclinShellInterface          , &AppShell_ptpon            , USER_TYPE_USER         },
#endif
#if FEATURE_MARVELL_6321
    {"rmu"              ,"      : Aurix2 controls 6321 via RMU"                                         , &g_AsclinShellInterface          , &AppShell_rmu               , USER_TYPE_ADMINISTRATOR},
#endif
    //=====================================================================================================
    // Cmds to show system status or information
    {"status"           , "       : Show the application status"                                        , &g_AsclinShellInterface          , &AppShell_status            , USER_TYPE_USER         },
    {"shelludpecho"     , "    : enable/disable UDP command echo"                                       , &g_AsclinShellInterface.shell    , &Shell_udp_echo             , USER_TYPE_ADMINISTRATOR},
    {"showmenuall"      , "    : enable/disable to show all menu items"                                 , &g_AsclinShellInterface.shell    , &Show_Menu_All              , USER_TYPE_ADMINISTRATOR},
    {"version"          , "      : show the firmware version"                                           , &g_AsclinShellInterface.shell    , &Shell_version              , USER_TYPE_USER         },
    {"revision"         , "     : show the develope version"                                            , &g_AsclinShellInterface.shell    , &Shell_revision             , USER_TYPE_USER         },
    {"help"             , SHELL_HELP_DESCRIPTION_TEXT                                                   , &g_AsclinShellInterface.shell    , &Ifx_Shell_showHelp         , USER_TYPE_USER         },
    IFX_SHELL_COMMAND_LIST_END
};

#endif





typedef struct
{
    volatile boolean isEnable;
    uint8 counter;
}t_qual_control;

static t_qual_control qual_control =
{
        .isEnable = FALSE,
        .counter = 0,
};

#define THERM_STATUS_INTERNAL_S     3
/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
/** \addtogroup IfxLld_Demo_AsclinShellInterface_SrcDoc_Main_Interrupt
 * \{ */

/** \name Interrupts for Serial interface
 * \{ */

IFX_INTERRUPT(ISR_Asc_Shell_rx, 0, ISR_PRIORITY_ASC_SHELL_RX);
IFX_INTERRUPT(ISR_Asc_Shell_tx, 0, ISR_PRIORITY_ASC_SHELL_TX);
IFX_INTERRUPT(ISR_Asc_Shell_ex, 0, ISR_PRIORITY_ASC_SHELL_EX);

/** \} */

/** \} */

/** \brief Handle Asc0 Rx interrupt
 *
 * \isrProvider \ref ISR_PROVIDER_ASC_0
 * \isrPriority \ref ISR_PRIORITY_ASC_0_RX
 *
 * - Configuration of this interrupt is done by Asc_If_init()
 * - This interrupt is raised each time a data have been received on the serial interface.
 *   and Asc_If_receiveIrq() will be called
 */
void ISR_Asc_Shell_rx(void)
{
    IfxCpu_enableInterrupts();
    IfxStdIf_DPipe_onReceive(&g_AsclinShellInterface.stdIf.asc);
}


/** \brief Handle Asc0 Tx interrupt
 *
 * \isrProvider \ref ISR_PROVIDER_ASC_0
 * \isrPriority \ref ISR_PRIORITY_ASC_0_TX
 *
 * - Configuration of this interrupt is done by Asc_If_init()
 * - This interrupt is raised each time the serial interface transmit buffer get empty
 *   and Asc_If_transmitIrq() will be called
 */
void ISR_Asc_Shell_tx(void)
{
    IfxCpu_enableInterrupts();
    IfxStdIf_DPipe_onTransmit(&g_AsclinShellInterface.stdIf.asc);
}


/** \brief Handle Asc0 Ex interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_ASC_0
 * \isrPriority \ref ISR_PRIORITY_ASC_0_EX
 *
 * - Configuration of this interrupt is done by Asc_If_init()
 * - This interrupt is raised each time an error occurs on the serial interface and
 *   Asc_If_errorIrq() will be called.
 */
void ISR_Asc_Shell_ex(void)
{
    IfxCpu_enableInterrupts();
    IfxStdIf_DPipe_onError(&g_AsclinShellInterface.stdIf.asc);
}


/** \addtogroup IfxLld_Demo_AsclinShellInterface_SrcDoc_Main_Interrupt
 * \{ */

/** Send the welcome screen to the Terminal
 *
 */
IfxStdIf_DPipe *io_tmp;
void welcomeScreen(App_AsclinShellInterface *app, IfxStdIf_DPipe *io)
{
    io_tmp = io;
    IfxStdIf_DPipe_print(io, ENDL ""ENDL);
    IfxStdIf_DPipe_print(io, "******************************************************************************"ENDL);
    IfxStdIf_DPipe_print(io, "*******                                                                *******"ENDL);
    IfxStdIf_DPipe_print(io, "*******  Infineon "IFXGLOBAL_DERIVATIVE_NAME " uC                                             *******"ENDL);
    IfxStdIf_DPipe_print(io, "******************************************************************************"ENDL);
    IfxStdIf_DPipe_print(io, "*******  Software Version Date : %4X/%2X/%2X                            *******"ENDL, app->info.srcRevDate >> 16, (app->info.srcRevDate >> 8) & 0xFF, (app->info.srcRevDate & 0xFF));
    IfxStdIf_DPipe_print(io, "*******  Software Version      : %2d.%02d                                 *******"ENDL, (app->info.srcRev >> 8) & 0xFF, (app->info.srcRev >> 0) & 0xFF);
    IfxStdIf_DPipe_print(io, "*******  iLLD version          : %d.%d.%d.%d.%d                             *******"ENDL, IFX_LLD_VERSION_GENERATION, IFX_LLD_VERSION_MAJOR, IFX_LLD_VERSION_MAJOR_UPDATE, IFX_LLD_VERSION_MINOR, IFX_LLD_VERSION_REVISION);
    IfxStdIf_DPipe_print(io, "*******  "COMPILER_NAME " Compiler      : %ld.%1dr%1d                                 *******"ENDL, (app->info.compilerVer >> 16) & 0xFF, (app->info.compilerVer >> 8) & 0xFF, (app->info.compilerVer >> 0) & 0xFF);
    IfxStdIf_DPipe_print(io, "******************************************************************************"ENDL);
}


/** \brief Handle the 'info' command.
 *
 * \par Syntax
 * - info : Show application information */
boolean AppShell_info(pchar args, void *data, IfxStdIf_DPipe *io)
{
    App_AsclinShellInterface *app = (App_AsclinShellInterface *)data;

    welcomeScreen(app, io);
    return TRUE;
}


/** \brief Handle the 'status' command
 *
 * \par Syntax
 *  - status : Show the application internal state
 */
boolean AppShell_status(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint8 i = 0;
    uint32  coreid, action = 0;
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : status"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax     : status heartbeat"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax     : status semreport"ENDL);
        IfxStdIf_DPipe_print(io, "           > Show the application internal state"ENDL);
    }
    else if (Ifx_Shell_matchToken(&args, "heartbeat") != FALSE)
    {
        for(i = 0; i < 6 ; i++){
            IfxStdIf_DPipe_print(io, "Core%d heartbeat:[%d], active:%d, debug:%d, shell=%d"ENDL, 
                                      i, core_show_count(i), core_show_healthy(i), core_show_debug(i), (int)check_core_shell_enabled(i));
        }
    }
    else if (Ifx_Shell_matchToken(&args, "debug") != FALSE)
    {
        if (Ifx_Shell_parseUInt32(&args, &coreid, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid core id: %s"ENDL, args);
            return TRUE;
        }
        if (coreid > 6 )
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid  core id %s"ENDL, args);
            return TRUE;
        }
        if (Ifx_Shell_parseUInt32(&args, &action, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid action: %s"ENDL, args);
            return TRUE;
        }
        if (action > 1 )
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid action %s"ENDL, args);
            return TRUE;
        }
        core_simulate_hang(coreid, (boolean)action);
        IfxStdIf_DPipe_print(io, "Set coreid: %d as %d"ENDL, coreid, action);
    }
    else if (Ifx_Shell_matchToken(&args, "semreport") != FALSE)
    {
        sem_report_status(io);
    }
    else
    {
        Ifx_DateTime rt;
        DateTime_get(&rt);
        IfxStdIf_DPipe_print(io, "Real-time: %02d:%02d:%02d"ENDL, rt.hours, rt.minutes, rt.seconds);
        IfxStdIf_DPipe_print(io, "CPU Frequency: %ld Hz"ENDL, (sint32)g_AppCpu0.info.cpuFreq);
        IfxStdIf_DPipe_print(io, "SYS Frequency: %ld Hz"ENDL, (sint32)g_AppCpu0.info.sysFreq);
        IfxStdIf_DPipe_print(io, "STM Frequency: %ld Hz"ENDL, (sint32)g_AppCpu0.info.stmFreq);
        IfxStdIf_DPipe_print(io, "Alive: %02d:%02d:%02d"ENDL, rt.hours, rt.minutes, rt.seconds);
        IfxStdIf_DPipe_print(io, "CPU0 Frequency: %ld Hz"ENDL, (sint32)g_AppCpu0.info.cpuFreq);
        IfxStdIf_DPipe_print(io, "STM0 Frequency: %ld Hz"ENDL, (sint32)g_AppCpu0.info.stmFreq);
/*
        IfxStdIf_DPipe_print(io, "CPU1 Frequency: %ld Hz"ENDL, (sint32)g_AppCpu1.info.cpuFreq);
        IfxStdIf_DPipe_print(io, "STM1 Frequency: %ld Hz"ENDL, (sint32)g_AppCpu1.info.stmFreq);
        IfxStdIf_DPipe_print(io, "CPU2 Frequency: %ld Hz"ENDL, (sint32)g_AppCpu2.info.cpuFreq);
        IfxStdIf_DPipe_print(io, "STM2 Frequency: %ld Hz"ENDL, (sint32)g_AppCpu2.info.stmFreq);    }
*/

    }
    return TRUE;
}


/** \brief Handle the 'status' command
 *
 * \par Syntax
 *  - coff : Show the application internal state
 */
boolean Stop_continuous_logging(pchar args, void *data, IfxStdIf_DPipe *io)
{
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : loff"ENDL);
        IfxStdIf_DPipe_print(io, "           > stop continuous loop logging of these commands"ENDL);
        IfxStdIf_DPipe_print(io, "           > rdtemp"ENDL);
    }
    else
    {
        display_temperature = FALSE;
        display_power = FALSE;
    }

    return TRUE;
}

/** \brief Stop thermal printout
 *
 *  Temp solution to stop thermal printout as it's anoying
 */
static boolean Stop_thermal_logging(pchar args, void *data, IfxStdIf_DPipe *io)
{
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : toff"ENDL);
        IfxStdIf_DPipe_print(io, "           > stop thermal logging print out"ENDL);
    }
    else
    {
        qual_control.isEnable = FALSE;
    }
    return TRUE;
}

static boolean Enable_thermal_logging(pchar args, void *data, IfxStdIf_DPipe *io)
{
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : ton"ENDL);
        IfxStdIf_DPipe_print(io, "           > enable thermal logging print out"ENDL);
    }
    else
    {
        qual_control.isEnable = TRUE;
    }
    return TRUE;
}


boolean AppShell_geth(pchar args, void *data, IfxStdIf_DPipe *io)
{
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax: geth run          ; run gigabit ethernet loopback test"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax: geth mdio         ; run phy mdio test"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax: geth vct          ; run phy vct test"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax: geth checklink    ; run phy checklink test"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax: geth oui          ; get phy oui "ENDL);
        IfxStdIf_DPipe_print(io, "Syntax: geth r [1:6] [devAddr] [regAddr]; read phy register "ENDL);
        IfxStdIf_DPipe_print(io, "Syntax: geth setmaster [1:6] [0:1]; set phy master or slave "ENDL);
        IfxStdIf_DPipe_print(io, "Syntax: geth getmaster    ; get phy master or slave "ENDL);
        IfxStdIf_DPipe_print(io, "Syntax: geth getspeed     ; get phy speed 1= 1000 0 =100 "ENDL);
        IfxStdIf_DPipe_print(io, "Syntax: geth setlp [1:6] [lp type] [0:1]  ; set phy loopback mode "ENDL);
        IfxStdIf_DPipe_print(io, "Syntax: geth phy [1:6] [1:6]  ; run phy test master and slave "ENDL);
        IfxStdIf_DPipe_print(io, "Syntax: geth packetgen [1:6]  ; run phy packet generator "ENDL);
        IfxStdIf_DPipe_print(io, "Syntax: geth bufdump          ; dump rx buffer "ENDL);
        IfxStdIf_DPipe_print(io, "Syntax: geth pktdbg [0:1] [0:1] ; enable or disable tx/rx buffer "ENDL);
        IfxStdIf_DPipe_print(io, "Syntax: geth sqi  ; get phy SQI level  "ENDL);
        IfxStdIf_DPipe_print(io, "Syntax: geth cqi  ; get phy CQI reading  "ENDL);
        IfxStdIf_DPipe_print(io, "Syntax: geth ping  ; ping"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax: geth qci_udp  ; qci_udp client test"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax: geth testmode [0:6]  ; PHY testmode 1~6 ,0 normal"ENDL);
    }
    else
    {
        if (Ifx_Shell_matchToken(&args, "run") != FALSE) {
//          Ifx_Console_print("geth run"ENDL);
            IfxStdIf_DPipe_print(io, "geth run"ENDL);
            GethBasicDemo_run();
//          Ifx_Console_print("geth run done"ENDL);
            IfxStdIf_DPipe_print(io, "geth run done"ENDL);
        }else if (Ifx_Shell_matchToken(&args, "ping") != FALSE)  {
//          Ifx_Console_print("Got geth ip"ENDL);
            IfxStdIf_DPipe_print(io, "Got geth ip"ENDL);
            ping_send_now();
        }
    }
    return TRUE;
}

boolean AppShell_srp(pchar args, void *data, IfxStdIf_DPipe *io)
{
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax: srp talker            ; run simple talker"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax: srp listener          ; run simple listener"ENDL);
    }
    else
    {
        if (Ifx_Shell_matchToken(&args, "talker") != FALSE) {
//          Ifx_Console_print("srp talker running"ENDL);
            IfxStdIf_DPipe_print(io, "srp talker running"ENDL);
            SRPDemo_run(1);
        }else if (Ifx_Shell_matchToken(&args, "listener") != FALSE)  {
//          Ifx_Console_print("srp listener running"ENDL);
            IfxStdIf_DPipe_print(io, "srp listener running"ENDL);
            SRPDemo_run(0);
        }
    }
    return TRUE;

}

boolean AppShell_maap(pchar args, void *data, IfxStdIf_DPipe *io)
{
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax: maap run          ; run maap"ENDL);
    }
    else
    {
        if (Ifx_Shell_matchToken(&args, "run") != FALSE) {
//          Ifx_Console_print("maap testing"ENDL);
            IfxStdIf_DPipe_print(io, "maap testing"ENDL);
            {
                unsigned char addr_base[6];

                if(avb_1722_maap_get_base_address(addr_base)!=-1)
                {
                  avb_1722_maap_request_addresses(1,addr_base);
                }else{
                  avb_1722_maap_request_addresses(1,NULL);
                }
            }
        }
    }
    return TRUE;

}

boolean AppShell_can(pchar args, void *data, IfxStdIf_DPipe *io)
{
    if (Ifx_Shell_matchToken(&args, "?") != FALSE) {
        IfxStdIf_DPipe_print(io, "Syntax: can on            ; start sending can packet"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax: can off           ; stop sending can packet"ENDL);
    } else {
        if (Ifx_Shell_matchToken(&args, "on") != FALSE) {
            IfxStdIf_DPipe_print(io, "start can test"ENDL);
            can_1722_flag = TRUE;
        } else if (Ifx_Shell_matchToken(&args, "off") != FALSE) {
            can_1722_flag = FALSE;
        }
    }
    return TRUE;
}

/** Initialise the serial interafce
 *
 */
void initSerialInterface(void)
{
    {   /** - Serial interface */
        IfxAsclin_Asc_Config config;
        IfxAsclin_Asc_initModuleConfig(&config, &MODULE_ASCLIN0);
        config.baudrate.baudrate             = CFG_ASC_SHELL_BAUDRATE;
        config.baudrate.oversampling         = IfxAsclin_OversamplingFactor_16;
        config.bitTiming.medianFilter        = IfxAsclin_SamplesPerBit_three;
        config.bitTiming.samplePointPosition = IfxAsclin_SamplePointPosition_8;
        config.interrupt.rxPriority          = ISR_PRIORITY_ASC_SHELL_RX;
        config.interrupt.txPriority          = ISR_PRIORITY_ASC_SHELL_TX;
        config.interrupt.erPriority          = ISR_PRIORITY_ASC_SHELL_EX;
        config.interrupt.typeOfService       = ISR_PROVIDER_ASC_SHELL;
        IfxAsclin_Asc_Pins ascPins = {
            .cts       = NULL_PTR,
            .ctsMode   = IfxPort_InputMode_noPullDevice,
            .rx        = &ASC_SHELL_RX_PIN,
            .rxMode    = IfxPort_InputMode_noPullDevice,
            .rts       = NULL_PTR,
            .rtsMode   = IfxPort_OutputMode_pushPull,
            .tx        = &ASC_SHELL_TX_PIN,
            .txMode    = IfxPort_OutputMode_pushPull,
            .pinDriver = IfxPort_PadDriver_cmosAutomotiveSpeed1
        };
        config.pins         = &ascPins;
        config.rxBuffer     = g_AsclinShellInterface.ascBuffer.rx;
        config.txBuffer     = g_AsclinShellInterface.ascBuffer.tx;
        config.txBufferSize = CFG_ASC_TX_BUFFER_SIZE;
        config.rxBufferSize = CFG_ASC_RX_BUFFER_SIZE;
        IfxAsclin_Asc_initModule(&g_AsclinShellInterface.drivers.asc, &config);

        /* Connect the standard asc interface to the device driver*/
        IfxAsclin_Asc_stdIfDPipeInit(&g_AsclinShellInterface.stdIf.asc, &g_AsclinShellInterface.drivers.asc);
    }

    /* Ifx_Console initialisation */
    Ifx_Console_init(&g_AsclinShellInterface.stdIf.asc);

    /* Assert initialisation */
    Ifx_Assert_setStandardIo(&g_AsclinShellInterface.stdIf.asc);
}

static void print_project_information(void)
{
    IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, ENDL"DDPX Aurix Serial Console"ENDL);
    IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "%s"ENDL, board.project_string);
    if(board.project == P3570)
    { // in P3570 we have dual Aurix, need to print the Aurix index.
        if(board.aurix_index == MAIN_AURIX)
        {
            IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "Main Aurix"ENDL);
        }
        else
        {
            IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "Second Aurix"ENDL);
        }
    }
    if (TLF_B_Step)
    {
        IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "with TLF35584 B/C-Step"ENDL);
    }
    else
    {
        IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "with TLF35584 A-Step"ENDL);
    }
}

static void print_version(void)
{
#if PROJECT == PROJECT_PAGASUS2
#if ETHERNET_OTA_FW_TYPE == _OTA_UPDATE_FW
    IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "UPDATE SW Version %2d.%02d.%02d"ENDL,
                UPDATE_SW_MAJOR, UPDATE_SW_MINOR, SW_BUGFIX);
#else
    IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "SW Version %2d.%02d.%02d"ENDL,
                SW_MAJOR, SW_MINOR, SW_BUGFIX);
#endif
#endif

       IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "Partition %2d"ENDL,PARTITION_LAYOUT);
    /* add string to align with PDK AFW for 5.1.3 release */
//  char aurix_ver[100];
//  Get_Aurix_Version(aurix_ver);
//  IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "%s"ENDL, aurix_ver);

#if DEBUG_VERSION_ENABLE == 1
    IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "Patch %d"ENDL, SW_BUILD);
#endif

    uint8 Mcu_Chip_Rev = SCU_CHIPID.B.CHREV;
    if(Mcu_Chip_Rev<0x10)
    {
        IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "TC397 Step A"ENDL);
    }
    else if((Mcu_Chip_Rev >= 0x10) && (Mcu_Chip_Rev < 0x20))
    {
        IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "TC397 Step B%c"ENDL, 'A'+ (Mcu_Chip_Rev-0x10));
    }
    else
    {
        IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "Error: Unknown Aurix step"ENDL);
    }
}


static void print_revision(void)
{
    if(board.project == P3570){
        IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "For Aurix NP"ENDL);
        IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "Date              : %4X/%2X/%2X"ENDL, SW_REVISION_DATE >> 16, (SW_REVISION_DATE >> 8) & 0xFF, (SW_REVISION_DATE & 0xFF));
        IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "Software Revision : %2d.%02d"ENDL, (SW_REVISION >> 8) & 0xFF, (SW_REVISION >> 0) & 0xFF);
        IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "iLLD version      : %d.%d.%d.%d.%d"ENDL, IFX_LLD_VERSION_GENERATION, IFX_LLD_VERSION_MAJOR, IFX_LLD_VERSION_MAJOR_UPDATE, IFX_LLD_VERSION_MINOR, IFX_LLD_VERSION_REVISION);
        if(board.aurix_index == MAIN_AURIX)
            IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "Main Aurix"ENDL);
        else
            IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "Second Aurix"ENDL);

        if(board.pcb_version != PG2_NULL){  
            if(board.pcb_version <= 0x09 && board.pcb_version >= 0x00)
                IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "HW: A0%d\r\n",board.pcb_version);
            else
                IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "HW: A%d\r\n",board.pcb_version);
        }	
    }
    else
        IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "Not support for this board.project, %d"ENDL,board.project);
}


static boolean Shell_version(pchar args, void *data, IfxStdIf_DPipe *io)
{

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : show the firmware version"ENDL);
    }
    else
    {
         print_project_information();
         print_version();
         IfxStdIf_DPipe_print(io, "It's an internal version(REF v305)."ENDL);
    }
    return TRUE;
}
static boolean Shell_revision(pchar args, void *data, IfxStdIf_DPipe *io)
{
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : show the revision"ENDL);
    }
    else
    {
         print_revision();
    }
    return TRUE;
}

boolean do_main_aurix_thermal_eth = 0;
boolean do_main_aurix_thermal_test = 0;
boolean do_sec_aurix_thermal_test = 0;
boolean do_main_aurix_thermal_can_flex = 0;
boolean do_main_aurix_vibration = 0;
void turn_on_camera_power(void)
{
    //Initial Camera POC
    AsclinShellInterface_execute_cmd("cimpower", "on");
    AsclinShellInterface_execute_cmd("cimcampower", "a0 on");
    AsclinShellInterface_execute_cmd("cimcampower", "a1 on");
    AsclinShellInterface_execute_cmd("cimcampower", "a2 on");
    AsclinShellInterface_execute_cmd("cimcampower", "a3 on");
    AsclinShellInterface_execute_cmd("cimcampower", "b0 on");
    AsclinShellInterface_execute_cmd("cimcampower", "b1 on");
    AsclinShellInterface_execute_cmd("cimcampower", "b2 on");
    AsclinShellInterface_execute_cmd("cimcampower", "b3 on");
    AsclinShellInterface_execute_cmd("cimcampower", "c0 on");
    AsclinShellInterface_execute_cmd("cimcampower", "c1 on");
    AsclinShellInterface_execute_cmd("cimcampower", "c2 on");
    AsclinShellInterface_execute_cmd("cimcampower", "c3 on");
    AsclinShellInterface_execute_cmd("cimcampower", "d0 on");
    AsclinShellInterface_execute_cmd("cimcampower", "d1 on");
    AsclinShellInterface_execute_cmd("cimcampower", "d2 on");
    AsclinShellInterface_execute_cmd("cimcampower", "d3 on");
    AsclinShellInterface_execute_cmd("cimaggpower", "a on");
    AsclinShellInterface_execute_cmd("cimaggpower", "b on");
    AsclinShellInterface_execute_cmd("cimaggpower", "c on");
    AsclinShellInterface_execute_cmd("cimaggpower", "d on");
}

void Set_Main_Aurix_thermal_test(unsigned char flag)
{
    do_main_aurix_thermal_test = flag;
    if (do_main_aurix_thermal_test == TRUE){
        //TodoI2C0
        Main_Aurix_thermal_test_init();
//        turn_on_camera_power();
    }
    //IfxStdIf_DPipe_print(io_tmp,"do_main_aurix_thermal_test=%d \r\n", do_main_aurix_thermal_test);
}

void Set_Second_Aurix_thermal_test(unsigned char flag)
{
    do_sec_aurix_thermal_test = flag;
    if (do_sec_aurix_thermal_test == TRUE)
        SCU_DTSCLIM.U = 0x0CD846D6; //initial TC397 temperature
    //IfxStdIf_DPipe_print(io_tmp,"Set_Second_Aurix_thermal_test=%d \r\n", do_sec_aurix_thermal_test);
}
void Set_Main_Aurix_thermal_test_eth(unsigned char flag)
{
    do_main_aurix_thermal_eth = flag;
    //IfxStdIf_DPipe_print(io_tmp,"do_main_aurix_thermal_eth=%d \r\n", do_main_aurix_thermal_eth);
}
void Set_Main_Aurix_thermal_test_can_flex(unsigned char flag)
{
    do_main_aurix_thermal_can_flex = flag;
    //IfxStdIf_DPipe_print(io_tmp,"do_main_aurix_thermal_can_flex=%d \r\n", do_main_aurix_thermal_can_flex);
}

void Set_Main_Aurix_vibration_test(unsigned char flag)
{
    do_main_aurix_vibration = flag;
    //IfxStdIf_DPipe_print(io_tmp,"do_main_aurix_thermal_can_flex=%d \r\n", do_main_aurix_thermal_can_flex);
}
void data_test(){

}

void disable_thermal_flag(void)
{
    //IfxStdIf_DPipe *io;
    //IfxStdIf_DPipe_print(io, "Disable Thermal Flag"ENDL);
    Set_ShowVoltages_log(TRUE);
    Set_Main_Aurix_thermal_test(FALSE);
    Set_Second_Aurix_thermal_test(FALSE);
    Set_Main_Aurix_thermal_test_eth(FALSE);
    Set_Main_Aurix_thermal_test_can_flex(FALSE);
    can_cnt_flag = FALSE;
}

static boolean do_thermal(pchar args, void *data, IfxStdIf_DPipe *io)
{
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : dothermal a1 1 1 1"ENDL);
        IfxStdIf_DPipe_print(io, "           : dothermal a1 1 1 0"ENDL);
        IfxStdIf_DPipe_print(io, "           : dothermal a1 1 0 1"ENDL);
        IfxStdIf_DPipe_print(io, "           : dothermal a1 1 0 0"ENDL);
        IfxStdIf_DPipe_print(io, "           : dothermal a1 0"ENDL);
        IfxStdIf_DPipe_print(io, "           : dothermal a2 1"ENDL);
        IfxStdIf_DPipe_print(io, "           : dothermal a2 0"ENDL);
        IfxStdIf_DPipe_print(io, "           : dothermal platform thermal_flag eth_flag can_flex_flag"ENDL);
    }
    else
    {
        if (Ifx_Shell_matchToken(&args, "a1") != FALSE)
        {
            if (Ifx_Shell_matchToken(&args, "1") != FALSE)
            {
                IfxStdIf_DPipe_print(io, "do a1 thermal test"ENDL);
                Set_ShowVoltages_log(FALSE);
                Set_Main_Aurix_thermal_test(TRUE);

                /*therm_eth_test start*/
                if (Ifx_Shell_matchToken(&args, "1") != FALSE)
                {
                    IfxStdIf_DPipe_print(io, "Open eth_flag"ENDL);
                    Set_Main_Aurix_thermal_test_eth(TRUE);

                    /*therm_can_flex_test start*/
                    if (Ifx_Shell_matchToken(&args, "1") != FALSE)
                    {
                        IfxStdIf_DPipe_print(io, "Open can_flex_flag"ENDL);
                        Set_Main_Aurix_thermal_test_can_flex(TRUE);

                        /*for vibratiion test decrease test frequence because of camiera i2c0 mismatc*/
                        if (Ifx_Shell_matchToken(&args, "1") != FALSE)
                        {
                            IfxStdIf_DPipe_print(io, "Open vibration_test_flag"ENDL);
                            Set_Main_Aurix_vibration_test(TRUE);
                        }
                    }
                    else if (Ifx_Shell_matchToken(&args, "0") != FALSE)
                    {
                        IfxStdIf_DPipe_print(io, "Close can_flex_flag"ENDL);
                        Set_Main_Aurix_thermal_test_can_flex(FALSE);
                    }
                    else
                    {
                        IfxStdIf_DPipe_print(io, "Syntaxerror : can_flex_flag only key 0 or 1"ENDL);
                        disable_thermal_flag();
                        return TRUE;
                    }
                    /*therm_can_flex_test end*/
                }
                else if (Ifx_Shell_matchToken(&args, "0") != FALSE)
                {
                    IfxStdIf_DPipe_print(io, "Close eth_flag"ENDL);
                    Set_Main_Aurix_thermal_test_eth(FALSE);

                    /*therm_can_flex_test start*/
                    if (Ifx_Shell_matchToken(&args, "1") != FALSE)
                    {
                        IfxStdIf_DPipe_print(io, "Open can_flex_flag"ENDL);
                        Set_Main_Aurix_thermal_test_can_flex(TRUE);
                    }
                    else if (Ifx_Shell_matchToken(&args, "0") != FALSE)
                    {
                        IfxStdIf_DPipe_print(io, "Close can_flex_flag"ENDL);
                        Set_Main_Aurix_thermal_test_can_flex(FALSE);
                    }
                    else
                    {
                        IfxStdIf_DPipe_print(io, "Syntaxerror : can_flex_flag only key 0 or 1"ENDL);
                        disable_thermal_flag();
                        return TRUE;
                    }
                    /*therm_can_flex_test end*/
                }
                else
                {
                    IfxStdIf_DPipe_print(io, "Syntaxerror : eth_flag only key 0 or 1"ENDL);
                    disable_thermal_flag();
                    return TRUE;
                }
                /*therm_eth_test end*/
            }
            else if (Ifx_Shell_matchToken(&args, "0") != FALSE)
            {
                IfxStdIf_DPipe_print(io, "cancel thermal test"ENDL);
                Set_ShowVoltages_log(TRUE);
                Set_Main_Aurix_thermal_test(FALSE);
                Set_Main_Aurix_thermal_test_eth(FALSE);
                Set_Main_Aurix_thermal_test_can_flex(FALSE);
                return TRUE;
            }
            else
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : thermal_flag only key 0 or 1"ENDL);
                disable_thermal_flag();
                return TRUE;
            }
            /*therm_test end*/
        }
        else if (Ifx_Shell_matchToken(&args, "a2") != FALSE)
        {
            if (Ifx_Shell_matchToken(&args, "1") != FALSE)
            {
                IfxStdIf_DPipe_print(io, "do a2 thermal test"ENDL);
                Set_Second_Aurix_thermal_test(TRUE);
            }
            else if (Ifx_Shell_matchToken(&args, "0") != FALSE)
            {
                IfxStdIf_DPipe_print(io, "cancel thermal test"ENDL);
                Set_Second_Aurix_thermal_test(FALSE);
            }
            else
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : thermal_flag only key 0 or 1"ENDL);
                disable_thermal_flag();
                return TRUE;
            }
        }
        else
        {
            IfxStdIf_DPipe_print(io, "default cancel thermal test"ENDL);
            disable_thermal_flag();
            return TRUE;
        }
    }
    return TRUE;
}


#if FEATURE_PTP
boolean PTP_enable = FALSE;
boolean AppShell_ptp(pchar args, void *data, IfxStdIf_DPipe *io)
{
   typedef enum {STATUS, MASTER, SLAVE, USAGE} cmdType;
   cmdType cmd = STATUS;
   int32_t initSyncPer, operSyncPer, initPdelayPer, operPdelayPer;
    //    ptpPortRole role = PTP_SLAVE;
   if (Ifx_Shell_matchToken(&args, "?") != FALSE) cmd = USAGE;
   else if (Ifx_Shell_matchToken(&args, "m") != FALSE) cmd = MASTER;
   else if (Ifx_Shell_matchToken(&args, "s") != FALSE) cmd = SLAVE;
   switch(cmd) {
   case USAGE:
      Ifx_Console_print("Change configuration"ENDL);
      Ifx_Console_print("\tptp <m/s> [<initSync> <operSync> <initPdelay> <operPdelay>]"ENDL);
      Ifx_Console_print("Print status"ENDL);
      Ifx_Console_print("\tptp"ENDL);
      break;
   case STATUS:
      ptpApiPrintStatus();
      break;
   case MASTER:
    //   role = PTP_MASTER;
      ptp_role = PTP_MASTER;
      break;
   case SLAVE:
    //   if (Ifx_Shell_parseSInt32(&args, &initSyncPer) == FALSE) initSyncPer = -3;
    //   if (Ifx_Shell_parseSInt32(&args, &operSyncPer) == FALSE) operSyncPer = -3;
    //   if (Ifx_Shell_parseSInt32(&args, &initPdelayPer) == FALSE) initPdelayPer = 0;
    //   if (Ifx_Shell_parseSInt32(&args, &operPdelayPer) == FALSE) operPdelayPer = 1;
    //   ptpApiReset(role, initSyncPer, operSyncPer, initPdelayPer, operPdelayPer);
      ptp_role = PTP_SLAVE;
      break;
   }
   return TRUE;
}

boolean AppShell_ptpon(pchar args, void *data, IfxStdIf_DPipe *io)
{

   if (Ifx_Shell_matchToken(&args, "?") != FALSE)
   {
      IfxStdIf_DPipe_print(io, "Syntax     : Turn on PTP"ENDL);
   }
   else
   {
      PTP_enable = TRUE;
   }
   return TRUE;
}
#endif

#if FEATURE_MARVELL_6321
boolean AppShell_rmu(pchar args, void *data, IfxStdIf_DPipe *io)
{
    MSD_U32 devAddr, regAddr, devnum;
    MSD_U32 regData;
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax: rmu init <devnum> ; devnum 0:88EA6321 1:88Q5050"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax: rmu ID <devnum>   ; get ID"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax: rmu read <devnum> <devAddr> <regAddr>; read register"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax: rmu write <devnum> <devAddr> <regAddr> <regData>; write register"ENDL);
    }
    else
    {
        if (Ifx_Shell_matchToken(&args, "init") != FALSE) {
          if (args[0] == '0'){
            init_88EA6321_RMU(MARVELL_88EA6321_DEVICE_NEMBER);
          }else if (args[0] == '1'){
            init_88EA6321_RMU(MARVELL_88Q5050_DEVICE_NEMBER);
          }
        }else if (Ifx_Shell_matchToken(&args, "ID") != FALSE) {
         MSD_U16 id=0;
          if (args[0] == '0'){
            msdRMUGetID(MARVELL_88EA6321_DEVICE_NEMBER,&id);
          }else if (args[0] == '1'){
            msdRMUGetID(MARVELL_88Q5050_DEVICE_NEMBER,&id);
          }
          Ifx_Console_print("id = 0x%x\n\r",id);
        }else if (Ifx_Shell_matchToken(&args, "read") != FALSE){
                 if (*Ifx_Shell_skipWhitespace(args) == '\0') {
                   return TRUE;
                 }
                 if (Ifx_Shell_parseUInt32(&args, &devnum, FALSE) != TRUE) {
                  printf("Err: Could not parse <devnum>!"ENDL);
                  return TRUE;
                 }
                 if (Ifx_Shell_parseUInt32(&args, &devAddr, FALSE) != TRUE) {
                  printf("Err: Could not parse <devAddr>!"ENDL);
                  return TRUE;
                 }
                 if (Ifx_Shell_parseUInt32(&args, &regAddr, FALSE) != TRUE) {
                  printf("Err: Could not parse <regAddr>!"ENDL);
                  return TRUE;
                 }
                 if(devnum==0){
                   MSD_QD_DEV* dev = sohoDevGet(MARVELL_88EA6321_DEVICE_NEMBER);
                   Pearl_msdRmuReadRegister(dev,devAddr,regAddr,&regData);
                 }else if(devnum==1){
                   MSD_QD_DEV* dev = sohoDevGet(MARVELL_88Q5050_DEVICE_NEMBER);
             Willow_msdRmuReadRegister(dev,devAddr,regAddr,&regData);
                 }

                 printf("0x%02x,0x%02x,0x%04x"ENDL, devAddr,regAddr,regData);
               }else if (Ifx_Shell_matchToken(&args, "write") != FALSE){
                 if (*Ifx_Shell_skipWhitespace(args) == '\0') {
                   return TRUE;
                 }
                 if (Ifx_Shell_parseUInt32(&args, &devnum, FALSE) != TRUE) {
                  printf("Err: Could not parse <devnum>!"ENDL);
                  return TRUE;
                 }
                 if (Ifx_Shell_parseUInt32(&args, &devAddr, FALSE) != TRUE) {
                  printf("Err: Could not parse <devAddr>!"ENDL);
                  return TRUE;
                 }
                 if (*Ifx_Shell_skipWhitespace(args) == '\0') {
                   return TRUE;
                 }
                 if (Ifx_Shell_parseUInt32(&args, &regAddr, FALSE) != TRUE) {
                  printf("Err: Could not parse <regAddr>!"ENDL);
                  return TRUE;
                 }
                 if (*Ifx_Shell_skipWhitespace(args) == '\0') {
                   return TRUE;
                 }
                 if (Ifx_Shell_parseUInt32(&args, &regData, FALSE) != TRUE) {
                  printf("Err: Could not parse <regData>!"ENDL);
                  return TRUE;
                 }
                 if(devnum==0){
                   MSD_QD_DEV* dev = sohoDevGet(MARVELL_88EA6321_DEVICE_NEMBER);
                   Pearl_msdRmuWriteRegister(dev,devAddr,regAddr,regData);
                 }else if(devnum==1){
                   MSD_QD_DEV* dev = sohoDevGet(MARVELL_88Q5050_DEVICE_NEMBER);
             Willow_msdRmuWriteRegister(dev,devAddr,regAddr,regData);
                 }
                  printf("0x%02x,0x%02x,0x%04x"ENDL, devAddr,regAddr,regData);
               }

      }
    return TRUE;
}
#endif
static boolean Shell_load_board_config(pchar args, void *data, IfxStdIf_DPipe *io)
{
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : loadboardconfig [set 3550/3479/3570]"ENDL);
        IfxStdIf_DPipe_print(io, "           > loadboardconfig"ENDL);
        IfxStdIf_DPipe_print(io, "            >> load the board config from I2C ROM if no option parameter is given"ENDL);
        IfxStdIf_DPipe_print(io, "           > loadboardconfig set 3550/3479"ENDL);
        IfxStdIf_DPipe_print(io, "            >> force to use 3550/3479 "ENDL);
        IfxStdIf_DPipe_print(io, "            >> Use it only if your ROM data is not correct, "ENDL);
        IfxStdIf_DPipe_print(io, "            >> This is a transient setting and reboot will wipe-out this setting"ENDL);
    }
    else
    {
        if (Ifx_Shell_matchToken(&args, "set") != FALSE)
        {
            if (Ifx_Shell_matchToken(&args, "3550") != FALSE)
            {
                IfxStdIf_DPipe_print(io, "force board to be E3550"ENDL);
                force_board_config(E3550);
            }
            else if (Ifx_Shell_matchToken(&args, "3479") != FALSE)
            {
                IfxStdIf_DPipe_print(io, "force board to be P3479"ENDL);
                force_board_config(P3479);
            }
            else if (Ifx_Shell_matchToken(&args, "3570") != FALSE)
            {
                IfxStdIf_DPipe_print(io, "force board to be P3570-A01"ENDL);
                force_board_config(P3570);
            }
            else
            {
                IfxStdIf_DPipe_print(io, "Error : invalid parameter %s"ENDL, args);
            }
        }
        else
        {
            load_board_config();
        }
    }
    return TRUE;
}

static boolean Shell_echo(pchar args, void *data, IfxStdIf_DPipe *io)
{
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : shellecho on/off"ENDL);
        IfxStdIf_DPipe_print(io, "           > enable/disable shell echo"ENDL);
    }
    else
    {
        if (Ifx_Shell_matchToken(&args, "on") != FALSE)
        {
            g_AsclinShellInterface.shell.control.echo = TRUE;
        }
        else if (Ifx_Shell_matchToken(&args, "off") != FALSE)
        {
            g_AsclinShellInterface.shell.control.echo = FALSE;
            IfxStdIf_DPipe_print(io, "Shell echo is turned off, please use \"shellecho on\" to turn it on"ENDL);
        }
        else
        {
            IfxStdIf_DPipe_print(io, "Error : invalid parameter %s"ENDL, args);
        }

    }
    return TRUE;
}


static boolean Shell_udp_echo(pchar args, void *data, IfxStdIf_DPipe *io)
{
    boolean enable = FALSE;
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : shelludpecho [enable/disable]"ENDL);
        IfxStdIf_DPipe_print(io, "           : enable/disable UDP command echo"ENDL);
    }
    else
    {
            if (Ifx_Shell_matchToken(&args, "enable") != FALSE)
            {
                enable = TRUE;
            }
            else if (Ifx_Shell_matchToken(&args, "disable") != FALSE)
            {
                enable = FALSE;
            }
            print_udp_cmd_ctrl(enable);
    }
    return TRUE;
}


static boolean Marvell_5050_tool(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 value[10][15],value1,value2,value3,value4;
    uint32 value_6321_1[10][16];
    uint32 value_6321_2[10][16];
    uint32 fid_mac[8][6];//fid_mac[8][6];
    uint32 dev_id=0;
    uint32 port_id=0;
    uint32 tx_num = 0;
    uint32 rx_num = 0;
    uint32 drop_array[256];
    memset(drop_array, 0, 256);    
    uint32 port_config,port;
    uint32 default_read_config_1=0xc000;//0xd000 capture all counters for a port //0xc000; read a capture or direct counter
    uint32 default_read_config_2=0xc400;//0xc400;
    uint32 val, devaddr, count, slaveaddr;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax: mav5050 "ENDL);
        IfxStdIf_DPipe_print(io, "      > items"ENDL);
    }
    else
    {
        if (Ifx_Shell_matchToken(&args, "mib") != FALSE)
        {

            //read SJA1105
			Switch_SJA1105_Device_Register_Read(0, 1, 0x402, &tx_num);
			Switch_SJA1105_Device_Register_Read(0, 1, 0x406, &rx_num);
			Switch_SJA1105_Device_Register_Read(0, 8, 0x408, drop_array);
			IfxStdIf_DPipe_print(io, "SJA1105 internal counter"ENDL);
			IfxStdIf_DPipe_print(io, "          INCOUNTER      OUTCOUNTER      DROP"ENDL);
			//IfxStdIf_DPipe_print(io, "SJA1105S Switch_1 P0[tx:%d, rx:%d], drop:", tx_num, rx_num);

			IfxStdIf_DPipe_print(io, "port[0]:  %d,            %d            ", rx_num-g_rx_num[0], tx_num-g_tx_num[0]);

			DEBUG_PRINTF_BUFFER_32BIT(drop_array, 8);//list drop packet

			Switch_SJA1105_Device_Register_Read(0, 1, 0x412, &tx_num);
			Switch_SJA1105_Device_Register_Read(0, 1, 0x416, &rx_num);
			Switch_SJA1105_Device_Register_Read(0, 8, 0x418, drop_array);
			//IfxStdIf_DPipe_print(io, "SJA1105S Switch_1 P1[tx:%d, rx:%d], drop:", tx_num, rx_num);

			IfxStdIf_DPipe_print(io, "port[1]:  %d,            %d            ", rx_num-g_rx_num[1], tx_num-g_tx_num[1]);

			DEBUG_PRINTF_BUFFER_32BIT(drop_array, 8);//list drop packet

			Switch_SJA1105_Device_Register_Read(0, 1, 0x422, &tx_num);
			Switch_SJA1105_Device_Register_Read(0, 1, 0x426, &rx_num);
			Switch_SJA1105_Device_Register_Read(0, 8, 0x428, drop_array);
			//IfxStdIf_DPipe_print(io, "SJA1105S Switch_1 P2[tx:%d, rx:%d], drop:", tx_num, rx_num);

			IfxStdIf_DPipe_print(io, "port[2]:  %d,            %d            ", rx_num-g_rx_num[2], tx_num-g_tx_num[2]);			

			DEBUG_PRINTF_BUFFER_32BIT(drop_array, 8);//list drop packet

			Switch_SJA1105_Device_Register_Read(0, 1, 0x432, &tx_num);
			Switch_SJA1105_Device_Register_Read(0, 1, 0x436, &rx_num);
			Switch_SJA1105_Device_Register_Read(0, 8, 0x438, drop_array);
			//IfxStdIf_DPipe_print(io, "SJA1105S Switch_1 P3[tx:%d, rx:%d], drop:", tx_num, rx_num);

			IfxStdIf_DPipe_print(io, "port[3]:  %d,            %d            ", rx_num-g_rx_num[3], tx_num-g_tx_num[3]);			

			DEBUG_PRINTF_BUFFER_32BIT(drop_array, 8);//list drop packet

			Switch_SJA1105_Device_Register_Read(0, 1, 0x442, &tx_num);
			Switch_SJA1105_Device_Register_Read(0, 1, 0x446, &rx_num);
			Switch_SJA1105_Device_Register_Read(0, 8, 0x448, drop_array);
			//IfxStdIf_DPipe_print(io, "SJA1105S Switch_1 P4[tx:%d, rx:%d], drop:", tx_num, rx_num);

			IfxStdIf_DPipe_print(io, "port[4]:  %d,            %d            ", rx_num-g_rx_num[4], tx_num-g_tx_num[4]);			

			DEBUG_PRINTF_BUFFER_32BIT(drop_array, 8);//list drop packet

            for (int port=PORT_0;port<=PORT_8;port++){
                // capture all counters for a port ;
                port_config=0xd000;
                port_config=0<<5|port_config;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1d, port_config);

                
                port_config=default_read_config_1;
                port_config=port<<5|port_config;
                port_config=port_config|0x00;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1f, &value2);
                //IfxStdIf_DPipe_print(io, "Show Marvell 88Q5050 MIB"ENDL);
                //IfxStdIf_DPipe_print(io, "incounter: %d, %d"ENDL, value1, value2);
                value[port][INCOUNTER]=value1<<15|value2;
                //IfxStdIf_DPipe_print(io, "incounter: %d"ENDL, value[port][INCOUNTER]);

                port_config=default_read_config_1;
                port_config=port<<5|port_config;
                port_config=port_config|0x0E;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1f, &value2);
                value[port][OUTCOUNTER]=value1<<15|value2;
                //IfxStdIf_DPipe_print(io, "outcounter: %d"ENDL, value[port][OUTCOUNTER]);

                port_config=default_read_config_1;
                port_config=port<<5|port_config;
                port_config=port_config|0x04;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1f, &value2);
                value[port][INUNICAST]=value1<<15|value2;
                //IfxStdIf_DPipe_print(io, "inUnicast: %d"ENDL, value[port][INUNICAST]);

                port_config=default_read_config_1;
                port_config=port<<5|port_config;
                port_config=port_config|0x06;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1f, &value2);
                value[port][INBROCAST]=value1<<15|value2;
                //IfxStdIf_DPipe_print(io, "inBrocast: %d"ENDL, value[port][INBROCAST]);

                port_config=default_read_config_1;
                port_config=port<<5|port_config;
                port_config=port_config|0x07;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1f, &value2);
                value[port][INMULTICAST]=value1<<15|value2;
                //IfxStdIf_DPipe_print(io, "inMulticast: %d"ENDL, value[port][INMULTICAST]);
                
                port_config=default_read_config_1;
                port_config=port<<5|port_config;
                port_config=port_config|0x10;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1f, &value2);
                value[port][OUTUNICAST]=value1<<15|value2;
                //IfxStdIf_DPipe_print(io, "inUnicast: %d"ENDL, value[port][INUNICAST]);

                port_config=default_read_config_1;
                port_config=port<<5|port_config;
                port_config=port_config|0x13;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1f, &value2);
                value[port][OUTBROCAST]=value1<<15|value2;
                //IfxStdIf_DPipe_print(io, "inBrocast: %d"ENDL, value[port][INBROCAST]);

                port_config=default_read_config_1;
                port_config=port<<5|port_config;
                port_config=port_config|0x12;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1f, &value2);
                value[port][OUTMULTICAST]=value1<<15|value2;

                port_config=default_read_config_1;
                port_config=port<<5|port_config;
                port_config=port_config|0x18;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1f, &value2);
                value[port][INUNDERSIZE]=value1<<15|value2;

                port_config=default_read_config_1;
                port_config=port<<5|port_config;
                port_config=port_config|0x19;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1f, &value2);
                value[port][INFRAGMENT]=value1<<15|value2;

                port_config=default_read_config_1;
                port_config=port<<5|port_config;
                port_config=port_config|0x1A;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1f, &value2);
                value[port][INOVERSIZE]=value1<<15|value2;

                port_config=default_read_config_1;
                port_config=port<<5|port_config;
                port_config=port_config|0x1B;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1f, &value2);
                value[port][INJABBER]=value1<<15|value2;

                port_config=default_read_config_2;
                port_config=port<<5|port_config;
                port_config=port_config|0x00;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1f, &value2);
                value[port][INDISCARDS]=value1<<15|value2;

                port_config=default_read_config_2;
                port_config=port<<5|port_config;
                port_config=port_config|0x1D;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1f, &value2);
                value[port][OUTDROPPS]=value1<<15|value2;

                port_config=default_read_config_2;
                port_config=port<<5|port_config;
                port_config=port_config|0x1E;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1f, &value2);
                value[port][OUTDISCARDS]=value1<<15|value2;
            }
            IfxStdIf_DPipe_print(io, "88Q5050 internal counter"ENDL);
            //IfxStdIf_DPipe_print(io, "         INCOUNTER      OUTCOUNTER      INUNICAST      INBRODCAST      INMULTICAST      OUTUNICAST      OUTBROCAST      OUTMULTICAST      INDISCARDS      OUTDROPPS      OUTDISCARDS"ENDL);
            //for (int port=PORT_0;port<=PORT_8;port++){
            //    IfxStdIf_DPipe_print(io, "port[%d]: %d,             %d,             %d,             %d,             %d,             %d,             %d,             %d,             %d,             %d,             %d"ENDL,port-1, value[port][INCOUNTER], value[port][OUTCOUNTER], value[port][INUNICAST], value[port][INBROCAST], value[port][INMULTICAST], value[port][OUTUNICAST], value[port][OUTBROCAST], value[port][OUTMULTICAST], value[port][INDISCARDS], value[port][OUTDROPPS], value[port][OUTDISCARDS]);
            //}
            //IfxStdIf_DPipe_print(io, "         INCOUNTER      OUTCOUNTER        INDISCARDS      OUTDROPPS      OUTDISCARDS         INUNDERSIZE         INFRAGMENT         INOVERSIZE           INJABBER"ENDL);
            //for (int port=PORT_0;port<=PORT_8;port++){
            //    IfxStdIf_DPipe_print(io, "port[%d]: %d,             %d,             %d,             %d,             %d,               %d,                %d,           %d,          %d"ENDL,port-1, value[port][INCOUNTER], value[port][OUTCOUNTER], value[port][INDISCARDS], value[port][OUTDROPPS], value[port][OUTDISCARDS],value[port][INUNDERSIZE],value[port][INFRAGMENT],value[port][INOVERSIZE],value[port][INJABBER]);
            //}
            IfxStdIf_DPipe_print(io, "         INCOUNTER      OUTCOUNTER      INDISCARDS      OUTDROPPS      OUTDISCARDS"ENDL);
            for (int port=PORT_0;port<=PORT_8;port++){
                IfxStdIf_DPipe_print(io, "port[%d]: %d,             %d,             %d,             %d,             %d"ENDL,port-1, value[port][INCOUNTER], value[port][OUTCOUNTER], value[port][INDISCARDS], value[port][OUTDROPPS], value[port][OUTDISCARDS]);
            }
/*            //5050 flow control
			IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x3, 0x01, 0xC3);
			//IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x3, 0x01, 0x03);

            IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x03, 0x00, &value1);
			IfxStdIf_DPipe_print(io, "port[3] 0x00: flow control : %d"ENDL, value1);
            IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x03, 0x01, &value1);
			IfxStdIf_DPipe_print(io, "port[3] 0x01: %d"ENDL, value1);			

			IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x6, 0x01, 0xC3);
			//IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x3, 0x01, 0x03);
            IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x06, 0x00, &value1);
			IfxStdIf_DPipe_print(io, "port[6] 0x00: flow control : %d"ENDL, value1);
            IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x06, 0x01, &value1);
			IfxStdIf_DPipe_print(io, "port[6] 0x01: %d"ENDL, value1);


			IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x7, 0x01, 0x20F2);
			//IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x7, 0x01, 0x2032);
			IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x07, 0x00, &value1);
			IfxStdIf_DPipe_print(io, "port[7] 0x00: flow control : %d"ENDL, value1 );
			IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x07, 0x01, &value1);
			IfxStdIf_DPipe_print(io, "port[7] 0x01: %d"ENDL, value1 );
*/

            for (int port=PORT_0;port<=PORT_6;port++){
                // capture all counters for a port ;
                //port_config=0xcc00;
                //port_config=0<<5|port_config;
                //value1=0;
                //value2=0;
                //IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1d, port_config);

                port_config=0xcc00; //0xcc00 bank_0 0xce00 bank_1
                port_config=port<<5|port_config;
                port_config=port_config|0x00;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1f, &value2);
                //IfxStdIf_DPipe_print(io, "Show Marvell 88Q5050 MIB"ENDL);
                //IfxStdIf_DPipe_print(io, "incounter: %d, %d"ENDL, value1, value2);
                value_6321_1[port][INCOUNTER_6321]=value1<<15|value2;
                //IfxStdIf_DPipe_print(io, "incounter: %d"ENDL, value[port][INCOUNTER]);

                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x0E;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1f, &value2);
                value_6321_1[port][OUTCOUNTER_6321]=value1<<15|value2;

                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x04;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1f, &value2);
                value_6321_1[port][INUNICAST_6321]=value1<<15|value2;
                //IfxStdIf_DPipe_print(io, "inUnicast: %d"ENDL, value[port][INUNICAST]);

                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x06;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1f, &value2);
                value_6321_1[port][INBROCAST_6321]=value1<<15|value2;
                //IfxStdIf_DPipe_print(io, "inBrocast: %d"ENDL, value[port][INBROCAST]);

                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x07;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1f, &value2);
                value_6321_1[port][INMULTICAST_6321]=value1<<15|value2;
                //IfxStdIf_DPipe_print(io, "inMulticast: %d"ENDL, value[port][INMULTICAST]);
                
                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x10;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1f, &value2);
                value_6321_1[port][OUTUNICAST_6321]=value1<<15|value2;
                //IfxStdIf_DPipe_print(io, "inUnicast: %d"ENDL, value[port][INUNICAST]);

                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x13;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1f, &value2);
                value_6321_1[port][OUTBROCAST_6321]=value1<<15|value2;
                //IfxStdIf_DPipe_print(io, "inBrocast: %d"ENDL, value[port][INBROCAST]);

                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x12;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1f, &value2);
                value_6321_1[port][OUTMULTICAST_6321]=value1<<15|value2;

                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x18;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1f, &value2);
                value_6321_1[port][INUNDERSIZE_6321]=value1<<15|value2;

                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x19;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1f, &value2);
                value_6321_1[port][INFRAGMENT_6321]=value1<<15|value2;

                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x1A;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1f, &value2);
                value_6321_1[port][INOVERSIZE_6321]=value1<<15|value2;

                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x1B;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1f, &value2);
                value_6321_1[port][INJABBER_6321]=value1<<15|value2;

                port_config=0xce00; //for bank 2
                port_config=port<<5|port_config;
                port_config=port_config|0x00;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1f, &value2);
                value_6321_1[port][INDISCARDS_6321]=value1<<15|value2;

                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x1C;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1f, &value2);
                value_6321_1[port][INRXERR_6321]=value1<<15|value2;

                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x1C;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1f, &value2);
                value_6321_1[port][INFCSERR_6321]=value1<<15|value2;

                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x03;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1f, &value2);
                value_6321_1[port][OUTFCSERR_6321]=value1<<15|value2;
            }
            IfxStdIf_DPipe_print(io, "88E6321_1  internal counter"ENDL);
            //IfxStdIf_DPipe_print(io, "         INCOUNTER      OUTCOUNTER      INUNICAST      INBRODCAST      INMULTICAST      OUTUNICAST      OUTBROCAST      OUTMULTICAST      INDISCARDS      INRXERR        INFCSERR           OUTFCSERR"ENDL);
            //for (int port=PORT_0;port<=PORT_6;port++){
            //    IfxStdIf_DPipe_print(io, "port[%d]: %d,             %d,             %d,             %d,             %d,             %d,             %d,             %d,             %d,             %d,             %d              %d"ENDL,port-1, value_6321_1[port][INCOUNTER_6321], value_6321_1[port][OUTCOUNTER_6321], value_6321_1[port][INUNICAST_6321], value_6321_1[port][INBROCAST_6321], value_6321_1[port][INMULTICAST_6321], value_6321_1[port][OUTUNICAST_6321], value_6321_1[port][OUTBROCAST_6321], value_6321_1[port][OUTMULTICAST_6321], value_6321_1[port][INDISCARDS_6321], value_6321_1[port][INRXERR_6321], value_6321_1[port][INFCSERR_6321], value_6321_1[port][OUTFCSERR_6321]);
            //}
            //IfxStdIf_DPipe_print(io, "         INCOUNTER      OUTCOUNTER         INDISCARDS      INRXERR        INFCSERR           OUTFCSERR         INUNDERSIZE         INFRAGMENT         INOVERSIZE           INJABBER"ENDL);
            //for (int port=PORT_0;port<=PORT_6;port++){
            //    IfxStdIf_DPipe_print(io, "port[%d]: %d,             %d,             %d,             %d,             %d,               %d,               %d,             %d,                 %d,             %d"ENDL,port-1, value_6321_1[port][INCOUNTER_6321], value_6321_1[port][OUTCOUNTER_6321], value_6321_1[port][INDISCARDS_6321], value_6321_1[port][INRXERR_6321], value_6321_1[port][INFCSERR_6321], value_6321_1[port][OUTFCSERR_6321],value_6321_1[port][INUNDERSIZE],value_6321_1[port][INFRAGMENT],value_6321_1[port][INOVERSIZE],value_6321_1[port][INJABBER]);
            //}
            IfxStdIf_DPipe_print(io, "         INCOUNTER      OUTCOUNTER      INDISCARDS      INRXERR        INFCSERR           OUTFCSERR"ENDL);
            for (int port=PORT_0;port<=PORT_6;port++){
                IfxStdIf_DPipe_print(io, "port[%d]: %d,             %d,             %d,             %d,             %d,               %d"ENDL,port-1, value_6321_1[port][INCOUNTER_6321], value_6321_1[port][OUTCOUNTER_6321], value_6321_1[port][INDISCARDS_6321], value_6321_1[port][INRXERR_6321], value_6321_1[port][INFCSERR_6321], value_6321_1[port][OUTFCSERR_6321]);
            }
            for (int port=PORT_0;port<=PORT_6;port++){
                // capture all counters for a port ;
                //port_config=0xcc00;
                //port_config=0<<5|port_config;
                //value1=0;
                //value2=0;
                //IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1d, port_config);

                port_config=0xcc00; //0xcc00 bank_0 0xce00 bank_1
                port_config=port<<5|port_config;
                port_config=port_config|0x00;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1f, &value2);
                //IfxStdIf_DPipe_print(io, "Show Marvell 88Q5050 MIB"ENDL);
                //IfxStdIf_DPipe_print(io, "incounter: %d, %d"ENDL, value1, value2);
                value_6321_2[port][INCOUNTER_6321]=value1<<15|value2;
                //IfxStdIf_DPipe_print(io, "incounter: %d"ENDL, value[port][INCOUNTER]);

                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x0E;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1f, &value2);
                value_6321_2[port][OUTCOUNTER_6321]=value1<<15|value2;

                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x04;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1f, &value2);
                value_6321_2[port][INUNICAST_6321]=value1<<15|value2;
                //IfxStdIf_DPipe_print(io, "inUnicast: %d"ENDL, value[port][INUNICAST]);

                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x06;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1f, &value2);
                value_6321_2[port][INBROCAST_6321]=value1<<15|value2;
                //IfxStdIf_DPipe_print(io, "inBrocast: %d"ENDL, value[port][INBROCAST]);

                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x07;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1f, &value2);
                value_6321_2[port][INMULTICAST_6321]=value1<<15|value2;
                //IfxStdIf_DPipe_print(io, "inMulticast: %d"ENDL, value[port][INMULTICAST]);
                
                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x10;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1f, &value2);
                value_6321_2[port][OUTUNICAST_6321]=value1<<15|value2;
                //IfxStdIf_DPipe_print(io, "inUnicast: %d"ENDL, value[port][INUNICAST]);

                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x13;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1f, &value2);
                value_6321_2[port][OUTBROCAST_6321]=value1<<15|value2;
                //IfxStdIf_DPipe_print(io, "inBrocast: %d"ENDL, value[port][INBROCAST]);

                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x12;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1f, &value2);
                value_6321_2[port][OUTMULTICAST_6321]=value1<<15|value2;

                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x18;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1f, &value2);
                value_6321_2[port][INUNDERSIZE_6321]=value1<<15|value2;

                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x19;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1f, &value2);
                value_6321_2[port][INFRAGMENT_6321]=value1<<15|value2;

                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x1A;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1f, &value2);
                value_6321_2[port][INOVERSIZE_6321]=value1<<15|value2;

                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x1B;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1f, &value2);
                value_6321_2[port][INJABBER_6321]=value1<<15|value2;

                port_config=0xce00; //for bank 2
                port_config=port<<5|port_config;
                port_config=port_config|0x00;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1f, &value2);
                value_6321_2[port][INDISCARDS_6321]=value1<<15|value2;

                port_config=0xce00; //for bank 2
                port_config=port<<5|port_config;
                port_config=port_config|0x00;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1f, &value2);
                value_6321_2[port][INDISCARDS_6321]=value1<<15|value2;

                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x1C;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1f, &value2);
                value_6321_2[port][INRXERR_6321]=value1<<15|value2;

                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x1C;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1f, &value2);
                value_6321_2[port][INFCSERR_6321]=value1<<15|value2;

                port_config=0xcc00;
                port_config=port<<5|port_config;
                port_config=port_config|0x03;
                value1=0;
                value2=0;
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1d, port_config);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1e, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1f, &value2);
                value_6321_2[port][OUTFCSERR_6321]=value1<<15|value2;
            }
            IfxStdIf_DPipe_print(io, "88E6321_2  internal counter"ENDL);
            //IfxStdIf_DPipe_print(io, "         INCOUNTER      OUTCOUNTER      INUNICAST      INBRODCAST      INMULTICAST      OUTUNICAST      OUTBROCAST      OUTMULTICAST      INDISCARDS      INRXERR        INFCSERR           OUTFCSERR"ENDL);
            //for (int port=PORT_0;port<=PORT_6;port++){
            //    IfxStdIf_DPipe_print(io, "port[%d]: %d,             %d,             %d,             %d,             %d,             %d,             %d,             %d,             %d,             %d,             %d              %d"ENDL,port-1, value_6321_2[port][INCOUNTER_6321], value_6321_2[port][OUTCOUNTER_6321], value_6321_2[port][INUNICAST_6321], value_6321_2[port][INBROCAST_6321], value_6321_2[port][INMULTICAST_6321], value_6321_2[port][OUTUNICAST_6321], value_6321_2[port][OUTBROCAST_6321], value_6321_2[port][OUTMULTICAST_6321], value_6321_2[port][INDISCARDS_6321], value_6321_2[port][INRXERR_6321], value_6321_2[port][INFCSERR_6321], value_6321_2[port][OUTFCSERR_6321]);
            //}
            //IfxStdIf_DPipe_print(io, "         INCOUNTER      OUTCOUNTER         INDISCARDS      INRXERR        INFCSERR           OUTFCSERR         INUNDERSIZE         INFRAGMENT         INOVERSIZE           INJABBER"ENDL);
            //for (int port=PORT_0;port<=PORT_6;port++){
            //    IfxStdIf_DPipe_print(io, "port[%d]: %d,             %d,             %d,             %d,             %d,             %d,                     %d,             %d,                %d,             %d"ENDL,port-1, value_6321_2[port][INCOUNTER_6321], value_6321_2[port][OUTCOUNTER_6321], value_6321_2[port][INDISCARDS_6321], value_6321_2[port][INRXERR_6321], value_6321_2[port][INFCSERR_6321], value_6321_2[port][OUTFCSERR_6321],value_6321_2[port][INUNDERSIZE],value_6321_2[port][INFRAGMENT],value_6321_2[port][INOVERSIZE],value_6321_2[port][INJABBER]);
            //}
            IfxStdIf_DPipe_print(io, "         INCOUNTER      OUTCOUNTER      INDISCARDS      INRXERR        INFCSERR           OUTFCSERR"ENDL);
            for (int port=PORT_0;port<=PORT_6;port++){
                IfxStdIf_DPipe_print(io, "port[%d]: %d,             %d,             %d,             %d,             %d,             %d"ENDL,port-1, value_6321_2[port][INCOUNTER_6321], value_6321_2[port][OUTCOUNTER_6321], value_6321_2[port][INDISCARDS_6321], value_6321_2[port][INRXERR_6321], value_6321_2[port][INFCSERR_6321], value_6321_2[port][OUTFCSERR_6321]);
            }
            //for (int port=0;port<=8;port++){
            //    IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x04, 115);
            //    IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x04, &value1);
            //    IfxStdIf_DPipe_print(io, "port:  %d"ENDL,value1);
            //}
        }
        else if (Ifx_Shell_matchToken(&args, "fid") != FALSE)
        {
            IfxStdIf_DPipe_print(io, "Marvell 88Q5050 FID database"ENDL);
            value1=0;
            value2=0;
            value3=0;
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x0b, 0xb100);
            IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x0d, &value1);
            IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x0e, &value2);
            IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x0f, &value3);
            fid_mac[0][0]=(value1>>8)&0xFF;
            fid_mac[0][1]=value1&0xFF;
            fid_mac[0][2]=(value2>>8)&0xFF;
            fid_mac[0][3]=value2&0xFF;
            fid_mac[0][4]=(value3>>8)&0xFF;
            fid_mac[0][5]=value3&0xFF;

            IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x0c, &value4);
            value4=(value4>>4)&0x01ff;
            IfxStdIf_DPipe_print(io, "FID[0]  %x:%x:%x:%x:%x:%x  PortVec  0x%x"ENDL,fid_mac[0][0],fid_mac[0][1],fid_mac[0][2],fid_mac[0][3],fid_mac[0][4],fid_mac[0][5],value4);

            for(int i=1;i<=7;i++){
                IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x0b, 0xc100);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x0d, &value1);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x0e, &value2);
                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x0f, &value3);
                fid_mac[i][0]=(value1>>8)&0xFF;
                fid_mac[i][1]=value1&0xFF;
                fid_mac[i][2]=(value2>>8)&0xFF;
                fid_mac[i][3]=value2&0xFF;
                fid_mac[i][4]=(value3>>8)&0xFF;
                fid_mac[i][5]=value3&0xFF;

                IfxEth_Marvell_Switch_read_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x0c, &value4);
                            value4=(value4>>4)&0x01ff;
                IfxStdIf_DPipe_print(io, "FID[%d]  %x:%x:%x:%x:%x:%x  PortVec  0x%x"ENDL,i,fid_mac[i][0],fid_mac[i][1],fid_mac[i][2],fid_mac[i][3],fid_mac[i][4],fid_mac[i][5],value4);
            }
        }
		else if(Ifx_Shell_matchToken(&args, "vlan") != FALSE){
#if 0
			slaveaddr = IFXETH_SWITCH_88E6321_2_ADDR;
			        /****************  Disable all ports ********************/
            for(devaddr=0x10; devaddr<=0x16; devaddr++)
            {
                IfxEth_Marvell_Switch_read_reg(slaveaddr, devaddr, 0x04, &val);
                val &= 0xFFFC;
                IfxEth_Marvell_Switch_write_reg(slaveaddr, devaddr, 0x04, val);
            }
			/*
            //VLAN11, P2,P3 is member
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x07, 0x1133); // P2,P3 is member
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x08, 0x3333);
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x06, 0x100B); // 11 VID and valid bit[12] =1
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x02, 0x000B); // FID = VID
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x05, 0xB000); // program VTU
            //IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x12, 0x08, 0x2480); // Enable 802.1Q on P2 (mode Fallback) //allow both tagged and un-tagged
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x13, 0x08, 0x2480); // Enable 802.1Q on P3 (mode Fallback) //allow both tagged and un-tagged
            //IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x14, 0x08, 0x2480); // Enable 802.1Q on P4 (mode Fallback) //allow both tagged and un-tagged
            //IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x12, 0x07, 0x100B); // P2 : Set default VID = 11 and force defaultVID =1
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x13, 0x07, 0x100B); // P3 : Set default VID = 11 and force defaultVID =1
            //IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x14, 0x07, 0x100B); // P4 : Set default VID = 11 and force defaultVID =1
            */
            //VLAN11, P0,P4 is member
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x07, 0x3332); // P0 is member
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x08, 0x3332); // p4 is member
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x06, 0x100B); // VID=11(set VLAN ID as 11) and enable valid bit[12] =1
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x02, 0x000B); // FID = VID
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x05, 0xB000); // program above VTU config data to 88E6321
            
            //IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x10, 0x08, 0x2480); // Enable 802.1Q on P0 (mode Fallback) //allow both tagged and un-tagged
			IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x10, 0x08, 0x2c80); //check mode
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x10, 0x07, 0x100B); // P0 : Set default VID = 11 and force defaultVID =1
			//IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x10, 0x07, 0x100c);//VID = 12

	        // packet forwarding P0 : P4
            IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x10, 0x06, &val);
            val = 0x10;
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x10, 0x06, val);

            // packet forwarding P4 : P0
            IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x14, 0x06, &val);
            val = 0x01;
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x14, 0x06, val);

            count = 0;
            while(val & 0x8000)
            {
                waitTime(TimeConst[TIMER_INDEX_1MS]);
                IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x1B, 0x05, &val);
                count++;
                if(count>100)
                {
                    IfxStdIf_DPipe_print(io,"Waiting for 88E6321 VTU not busy fail\r\n");
                    break;
                }
            }

			/****************  Enable all ports ********************/
            for(devaddr=0x10; devaddr<=0x16; devaddr++)
            {
                IfxEth_Marvell_Switch_read_reg(slaveaddr, devaddr, 0x04, &val);
                val |= 0x0003;
                IfxEth_Marvell_Switch_write_reg(slaveaddr, devaddr, 0x04, val);
            }

		    slaveaddr = IFXETH_SWITCH_88E6321_1_ADDR;
			/****************  Disable all ports ********************/
			for(devaddr=0x10; devaddr<=0x16; devaddr++)
			{
				IfxEth_Marvell_Switch_read_reg(slaveaddr, devaddr, 0x04, &val);
				val &= 0xFFFC;
				IfxEth_Marvell_Switch_write_reg(slaveaddr, devaddr, 0x04, val);
			}		
			//VLAN11, P0,P4 is member
			IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x07, 0x3332); // P0 is member
			IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x08, 0x3332); // p4 is member
			IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x06, 0x100B); // VID=11(set VLAN ID as 11) and enable valid bit[12] =1
			IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x02, 0x000B); // FID = VID
			IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x05, 0xB000); // program above VTU config data to 88E6321
					
			//IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x10, 0x08, 0x2480); // Enable 802.1Q on P0 (mode Fallback) //allow both tagged and un-tagged
			//IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x10, 0x07, 0x100B); // P0 : Set default VID = 11 and force defaultVID =1
		
			// packet forwarding P0 : P4
			IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x10, 0x06, &val);
			val = 0x10;
			IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x10, 0x06, val);
		
			// packet forwarding P4 : P0
			IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x14, 0x06, &val);
			val = 0x01;
			IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x14, 0x06, val);
		
			count = 0;
			while(val & 0x8000)
			{
				waitTime(TimeConst[TIMER_INDEX_1MS]);
				IfxEth_Marvell_Switch_read_reg(slaveaddr, 0x1B, 0x05, &val);
				count++;
				if(count>100)
				{
					IfxStdIf_DPipe_print(io,"Waiting for 88E6321 VTU not busy fail\r\n");
					break;
				}
			}
		
			/****************  Enable all ports ********************/
			for(devaddr=0x10; devaddr<=0x16; devaddr++)
			{
				IfxEth_Marvell_Switch_read_reg(slaveaddr, devaddr, 0x04, &val);
				val |= 0x0003;
				IfxEth_Marvell_Switch_write_reg(slaveaddr, devaddr, 0x04, val);
			}
            //======================================================//

			slaveaddr = IFXETH_SWITCH_88Q5050_ADDR;
            //set 88Q5050
			/****************  Disable all ports ********************/
			for(devaddr=0x0; devaddr<=0x08; devaddr++)
			{
				IfxEth_Marvell_Switch_read_reg(slaveaddr, devaddr, 0x04, &val);
				val &= 0xFFFC;
				IfxEth_Marvell_Switch_write_reg(slaveaddr, devaddr, 0x04, val);
			}	
			
            //VLAN11, P6,P7 is member
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x07, 0x3333); // P0~p3 aren't member
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x08, 0x2233); // p6,p7 are member
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x06, 0x100B); // VID=11(set VLAN ID as 11) and enable valid bit[12] =1
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x02, 0x000B); // FID = VID
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x1B, 0x05, 0xB000); // program above VTU config data to 88Q5050
            
            //IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x6, 0x08, 0x2480); // Enable 802.1Q on P6 (mode Fallback) //allow both tagged and un-tagged
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x6, 0x08, 0x2c80); // Enable 802.1Q on P6 (mode check) 
            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x6, 0x07, 0x100B); // P6 : Set default VID = 11 and force defaultVID =1

            IfxEth_Marvell_Switch_write_reg(slaveaddr, 0x7, 0x08, 0x2c80); // Enable 802.1Q on P7 (mode check) 

			
		    // Set 88Q5050 forwarding P7 ---> P6,P3
			port = 7;
			IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x48);
			// Set 88Q5050 forwarding P6 ---> P7
			port = 6;
			IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x80);
		    // Set 88Q5050 forwarding P3 ---> P7
			port = 3;
			IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, port, 0x6, 0x80);		

			/****************  Enable all ports ********************/
			for(devaddr=0x0; devaddr<=0x08; devaddr++)
			{
				IfxEth_Marvell_Switch_read_reg(slaveaddr, devaddr, 0x04, &val);
				val |= 0x0003;
				IfxEth_Marvell_Switch_write_reg(slaveaddr, devaddr, 0x04, val);
			}			
#endif
		}
        else if (Ifx_Shell_matchToken(&args, "clear") != FALSE)
        {
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88Q5050_ADDR, 0x1b, 0x1d, 0x9120);
            IfxStdIf_DPipe_print(io, "Clear Marvell 88Q5050 MIB"ENDL);
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_1_ADDR, 0x1b, 0x1d, 0x9120);
            IfxStdIf_DPipe_print(io, "Clear Marvell 88E6321_1 MIB"ENDL);
            IfxEth_Marvell_Switch_write_reg(IFXETH_SWITCH_88E6321_2_ADDR, 0x1b, 0x1d, 0x9120);
            IfxStdIf_DPipe_print(io, "Clear Marvell 88E6321_2 MIB"ENDL);

			Switch_SJA1105_Device_Register_Read(0, 1, 0x402, &g_tx_num[0]);
			Switch_SJA1105_Device_Register_Read(0, 1, 0x406, &g_rx_num[0]);
			Switch_SJA1105_Device_Register_Read(0, 1, 0x412, &g_tx_num[1]);
			Switch_SJA1105_Device_Register_Read(0, 1, 0x416, &g_rx_num[1]);
			Switch_SJA1105_Device_Register_Read(0, 1, 0x422, &g_tx_num[2]);
			Switch_SJA1105_Device_Register_Read(0, 1, 0x426, &g_rx_num[2]);
			Switch_SJA1105_Device_Register_Read(0, 1, 0x432, &g_tx_num[3]);
			Switch_SJA1105_Device_Register_Read(0, 1, 0x436, &g_rx_num[3]);
			Switch_SJA1105_Device_Register_Read(0, 1, 0x442, &g_tx_num[4]);
			Switch_SJA1105_Device_Register_Read(0, 1, 0x446, &g_rx_num[4]);
			IfxStdIf_DPipe_print(io, "Clear SJA1105 MIB"ENDL);


        }
        else
        {
            IfxStdIf_DPipe_print(io, "Error : invalid parameter %s"ENDL, args);
        }

    }
    return TRUE;
}

static boolean Show_Menu_All(pchar args, void *data, IfxStdIf_DPipe *io)
{
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax: showmenuall on/off"ENDL);
        IfxStdIf_DPipe_print(io, "      > enable/disable to show all menu items"ENDL);
    }
    else
    {
        if (Ifx_Shell_matchToken(&args, "on") != FALSE)
        {
            g_AsclinShellInterface.user_type= USER_TYPE_ADMINISTRATOR;
            IfxStdIf_DPipe_print(io, "All menu items is turned on, please use \"showmenuall off\" to turn it off"ENDL);
        }
        else if (Ifx_Shell_matchToken(&args, "off") != FALSE)
        {
            g_AsclinShellInterface.user_type= USER_TYPE_DEFAULT;
            IfxStdIf_DPipe_print(io, "All menu items is turned off, please use \"showmenuall on\" to turn it on"ENDL);
        }
        else
        {
            IfxStdIf_DPipe_print(io, "Error : invalid parameter %s"ENDL, args);
        }

    }
    return TRUE;
}


#if 1
void AsclinShellInterface_init(void)
{
    /** - Initialise the time constants */
    initTime();

    /** - Initialise the serial interface and the console */
    initSerialInterface();

    g_AsclinShellInterface.info.srcRev      = SW_REVISION;
    g_AsclinShellInterface.info.srcRevDate  = SW_REVISION_DATE;
#if defined(__TASKING__)
    g_AsclinShellInterface.info.compilerVer = SW_COMPILER_VERSION;
#else
    g_AsclinShellInterface.info.compilerVer = 0;
#endif

    g_AsclinShellInterface.user_type= USER_TYPE_DEFAULT;


    /** - Enable the global interrupts of this CPU */
    restoreInterrupts(TRUE);

    Ifx_SizeT count = 52;

//    Ifx_Console_print(ENDL);

    /** - Simple print using IfxAsclin_Asc_write API */
    IfxAsclin_Asc_write(&g_AsclinShellInterface.drivers.asc, "Hello world!  => print using IfxAsclin_Asc_write()\n", &count, TIME_INFINITE);

    /** - Simple print using Ifx_Console_print API */
//    Ifx_Console_print(ENDL "Hello world!  => print using Ifx_Console_print()"ENDL);

    /** - Simple print using IfxStdIf_DPipe_print API */
    IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, ENDL "Hello world!  => print using IfxStdIf_DPipe_print()"ENDL);

    /** - Show the welcome screen using the standard DPipe interface */
    welcomeScreen(&g_AsclinShellInterface, &g_AsclinShellInterface.stdIf.asc);

    /** - Initialise the shell interface  */
    {
        Ifx_Shell_Config config;
        Ifx_Shell_initConfig(&config);
        config.standardIo     = &g_AsclinShellInterface.stdIf.asc;
        if(board.aurix_index == MAIN_AURIX)
            config.commandList[0] = &APPSHELL_COMMANDS_MAIN_AURIX[0];
        else
            config.commandList[0] = &APPSHELL_COMMANDS_2ND_AURIX[0];

        Ifx_Shell_init(&g_AsclinShellInterface.shell, &config);
    }

//    Ifx_Console_print(ENDL "Enter 'help' to see the available commands"ENDL);
}

void AsclinShellInterface_log(void)
{
    IfxStdIf_DPipe_print(io_tmp, "test UART log %x, %x, %x"ENDL, 0x99,0x88,0x77);
}


void AsclinShellInterface_printf_test(const char *format, ...)
{
    {
        // make sure that shell is ready
        IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, format);
    }
}
#endif

void AsclinShellInterface_run_main_aurix(void)
{
    if(g_Eray_run_mode==ERAY_TEST_MODE){
        Print_ErayTestModeMsg(&g_AsclinShellInterface.stdIf.asc);
    }else if(g_Eray_run_mode==ERAY_NORMAL_MODE_TEST_S1||g_Eray_run_mode==ERAY_NORMAL_MODE_TEST_S2){
         Print_ErayNormalModeMsg(&g_AsclinShellInterface.stdIf.asc);
    }

    Print_CAN_Wakeup_detection(&g_AsclinShellInterface.stdIf.asc);
    AsclinShellInterface_Print_Delay_Message();
}

void AsclinShellInterface_run_second_aurix(void)
{
    //no functions
}

void AsclinShellInterface_run(void)
{
//    if(check_core_shell_enabled(CORE0)){
        Ifx_Shell_process(&g_AsclinShellInterface.shell);        
//    }

    if(board.aurix_index == MAIN_AURIX)
        AsclinShellInterface_run_main_aurix();
    else
        AsclinShellInterface_run_second_aurix();
    cpu0_idle_counter++;

#if CAN_INTERRUPT_ENABLE == 0
    Print_MulticanMsg(&g_AsclinShellInterface.stdIf.asc);
#endif
}

void AsclinShellInterface_qualDisplay(boolean is_no_delay)
{
    if(qual_control.isEnable)
    {
        qual_control.counter ++;
        if(((qual_control.counter % THERM_STATUS_INTERNAL_S) == 0 ) || (is_no_delay))
        {
            qual_control.counter = 0;
            Print_Thermal_Status(&g_AsclinShellInterface.stdIf.asc);
        }
    }
    else
    { /* if qual log is not enable, then clear it to FALSE to avoid it print out log later when it's on */
        display_thermal_status = FALSE;
    }
}

void AsclinShellInterface_execute_cmd(pchar commandLine, pchar arg)
{
    uint8 i = 0;
    uint8 buffer[100];
#if 0
    if((commandLine != NULL_PTR) && (arg != NULL_PTR))
        DEBUG_PRINTF(("%s %s\n",commandLine, arg));
#endif
    const Ifx_Shell_Command* cmd_ptr;
    if(board.aurix_index == MAIN_AURIX)
    {
        cmd_ptr = &APPSHELL_COMMANDS_MAIN_AURIX[0];
    }
    else
    {
        cmd_ptr = &APPSHELL_COMMANDS_2ND_AURIX[0];
    }
    while(cmd_ptr[i].commandLine != NULL_PTR)
    {
        if(strcmp(cmd_ptr[i].commandLine, commandLine) == 0)
        {
            cmd_ptr[i].call(arg, buffer, &g_AsclinShellInterface.stdIf.asc);
            break;
        }
        i++;
    }
#if 0
    if(cmd_ptr[i].commandLine == NULL_PTR)
    {
        DEBUG_PRINTF(("Can not execute the command %s\n",commandLine));
    }
#endif
}

void AsclinShellInterface_Print_Delay_Message(void)
{
#if DELAY_MESSAGE_SHOW_UP_ENABLE == 1
    print_project_information();
    print_version();
    Ifx_Console_print(ENDL "Enter 'help' to see the available commands"ENDL);
    Ifx_Console_print("Due to the UART console comes up very late,\r\n"
            "you may miss the power on message from Aurix\r\n");
#endif
}


#define MAXCHARS 256

void AsclinShellInterface_printf(const char *format, ...)
{
#if 1
    char    str[MAXCHARS + 4];
    /* do not print if shell is not initial */
//  if(!isShellInitial)
//      return;
    va_list args;
    va_start(args, format);
    vsnprintf(str, MAXCHARS, format, args);
    va_end(args);
    {
        // make sure that shell is ready
        IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "%s\r", (uint8 *)&str[0]);
    }
#else

        char      message[STDIF_DPIPE_MAX_PRINT_SIZE + 1];
        Ifx_SizeT count;
        va_list   args;
        va_start(args, format);
        vsprintf((char *)message, format, args);
        va_end(args);
        count = (Ifx_SizeT)strlen(message);
        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, count < STDIF_DPIPE_MAX_PRINT_SIZE);
        //return
        IfxStdIf_DPipe_write(&g_AsclinShellInterface.stdIf.asc, (void *)message, &count, TIME_INFINITE);
#endif

}



void AsclinShellInterface_printbuffer(uint8* buffer, uint16 size)
{
    uint16 i;
    char buffer_str[256*4];
    memset(buffer_str, 0, sizeof(buffer_str));
    for(i = 0; i<size; i++)
    {
        if(i%10 == 0)
        {
            if(i > 0){
                IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "%s"ENDL, buffer_str);
                memset(buffer_str, 0, sizeof(buffer_str));
            }
            sprintf(buffer_str, "%02X", (uint8)buffer[i]);
        }else{
            sprintf(buffer_str, "%s %02X", buffer_str, (uint8)buffer[i]);
        }
    }
    IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "%s"ENDL, buffer_str);
}

void AsclinShellInterface_printbuffer32bit(uint32* buffer, uint16 size)
{
    uint16 i;
    char buffer_str[256*4];
    memset(buffer_str, 0, sizeof(buffer_str));
    for(i = 0; i<size; i++)
    {
        if((i != 0) && (i%8 == 0))
        {
            IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "%s"ENDL, buffer_str);
            memset(buffer_str, 0, sizeof(buffer_str));
        }
        sprintf(buffer_str, "%s0x%08x ", buffer_str, (uint32)buffer[i]);
        //strcat(buffer_str, byte_str);
    }
    IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "%s"ENDL, buffer_str);
}

