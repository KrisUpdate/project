/**
 * \file Cpu3_Main.c
 * \brief CPU3 functions.
 *
 * \version disabled
 * \copyright Copyright (c) 2018 Infineon Technologies AG. All rights reserved.
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
#include <stdio.h>
#include <string.h>
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include <AsclinShellInterface.h>
#include "Cpu_Main.h"
#include <TsensorCmds.h>
#include <PSensorCmds.h>
#include "Board.h"
#include "Gpio.h"
#include "fan.h"
#include "Ifx_Console.h"
#include "IfxPms_reg.h"
#include "ErayNormalDemo.h"
#include "resource.h"
#include "core_alive_check.h"
#include "board_I2C_devices.h"
#include "gpioexp.h"
#include "Gpio_ctrl_3570.h"
#include "Psensor.h"
#include "Tsensor.h"
#include "TsensorCmds.h"
#include "Phy_88q2112.h"
#include "marvell_switch.h"
#include "cangateway.h"
#include "CIM.h"
#include "I2c.h"
#include "Mcmcan.h"
#include "Eray.h"
#include "VadcCmds.h"
#include "tegra_console.h"

extern volatile boolean system_ready;
extern IfxCpu_syncEvent cpuSyncEvent;
extern IfxStdIf_DPipe *io_tmp;
extern Eray_run_mode g_Eray_run_mode;
#pragma section ".bss_cpu3" awc3
App_Cpu g_AppCpu3;
static volatile boolean eventFlag_1ms_stm_3 = FALSE;
static volatile boolean eventFlag_10ms_stm_3 = FALSE;
static volatile boolean eventFlag_100ms_stm_3 = FALSE;
static volatile boolean eventFlag_1s_stm_3 = FALSE;
volatile uint32 cpu3_idle_counter=0;
extern boolean do_main_aurix_vibration;
extern boolean is_ota_start;
int thermal_section_id = FALSE;
#define THERMAL_MONITOR_BUFFER_SIZE 151
//actually, AurixA, 126 => ok
//actually, AurixA, 150 => ok
uint8 thermal_monitor_buffer[THERMAL_MONITOR_BUFFER_SIZE];
extern boolean disable_udp_cmd_flag;

int power_good_section_id = FALSE;
#define POWER_GOOD_MONITOR_BUFFER_SIZE 285
//actually, 284 => ok
uint8 PowerGood_monitor_buffer[POWER_GOOD_MONITOR_BUFFER_SIZE];

int ina3221_section_id = FALSE;
#define INA3221_MONITOR_BUFFER_SIZE 140
//actually, 101 => ok
uint8 ina3221_monitor_buffer[INA3221_MONITOR_BUFFER_SIZE];

int eth_section_id = FALSE;
//#define ETH_MONITOR_BUFFER_SIZE 220
#define ETH_MONITOR_BUFFER_SIZE 250 
uint8 eth_monitor_buffer[ETH_MONITOR_BUFFER_SIZE];
//actually, 222 => ok

int fan_section_id = FALSE;
#define FAN_MONITOR_BUFFER_SIZE 60
uint8 fan_monitor_buffer[FAN_MONITOR_BUFFER_SIZE];
//actually, 21 => ok

int idrom_section_id = FALSE;
#define IDROM_MONITOR_BUFFER_SIZE 32
uint8 idrom_monitor_buffer[IDROM_MONITOR_BUFFER_SIZE];
//actually, 29 => ok

int max20087_section_id = FALSE;
#define MAX20087_MONITOR_BUFFER_SIZE 84
//actually, 83 => ok
uint8 max20087_monitor_buffer[MAX20087_MONITOR_BUFFER_SIZE];
#pragma section

extern boolean isDoShowVoltages;
extern boolean isDoOTARST;

void read_camera_poc(void);

void thermal_monitor_update(void)
{
    /* example
    "*TEMP_BOARD*LOCAL=28.69;ALERT=H;SHUTDOWN=H;"
    "*TEMP_XA*ALERT=H;SHUTDOWN=H;"
    "*TEMP_FDMF5833*CPU_ALERT=H;GPU_ALERT=H;SOC_ALERT=H;CV_ALERT=H;"
    */

    int buffer_used = 0;

    if(board.aurix_index == MAIN_AURIX){
        /*i2c_tmp451_1 parameter start*/
        float board_local_1 = readTsensorLocal(&i2c_tmp451_1);
        float board_remote = readTsensorRemote(&i2c_tmp451_1);
        IfxPort_Pin *shdn_ptr;
        shdn_ptr = &BRD_THERM_SHDN_N;
        boolean board_shutdown = IfxPort_getPinState(shdn_ptr->port, shdn_ptr->pinIndex);
        IoExpPort_Pin* Therm_Alert_1_ptr;
        Therm_Alert_1_ptr = &P3570_BRD_THERM_ALERT_N_EXP_1;
        boolean board_Therm_Alert_1 = READ_GPIOEXP_STATE((*Therm_Alert_1_ptr));
        buffer_used += snprintf(thermal_monitor_buffer+buffer_used, THERMAL_MONITOR_BUFFER_SIZE-buffer_used,
                                "*TEMP_BOARD*LOCAL_1=%.2f;REMOTE=%.2f;THERM_ALERT_1=%c;SHUTDOWN=%c;",
                                board_local_1, board_remote, board_Therm_Alert_1?'H':'L', board_shutdown?'H':'L');
        /*i2c_tmp451_1 parameter end*/

        /*i2c_tmp451_2 parameter start*/
        float board_local_2 = readTsensorLocal(&i2c_tmp451_2);
        IoExpPort_Pin* Therm_Alert_2_ptr;
        Therm_Alert_2_ptr = &P3570_BRD_THERM_ALERT_N_EXP_2;
        boolean board_Therm_Alert_2 = READ_GPIOEXP_STATE((*Therm_Alert_2_ptr));
        buffer_used += snprintf(thermal_monitor_buffer+buffer_used, THERMAL_MONITOR_BUFFER_SIZE-buffer_used,
                                "LOCAL_2=%.2f;THERM_ALERT_2=%c;",
                                board_local_2, board_Therm_Alert_2?'H':'L');
        /*i2c_tmp451_2 parameter end*/

        /*i2c_tmp451_3 parameter start*/
        float board_local_3 = readTsensorLocal(&i2c_tmp451_3);
        IoExpPort_Pin* Therm_Alert_3_ptr;
        Therm_Alert_3_ptr = &P3570_BRD_THERM_ALERT_N_EXP_3;
        boolean board_Therm_Alert_3 = READ_GPIOEXP_STATE((*Therm_Alert_3_ptr));
        buffer_used += snprintf(thermal_monitor_buffer+buffer_used, THERMAL_MONITOR_BUFFER_SIZE-buffer_used,
                                "LOCAL_3=%.2f;THERM_ALERT_3=%c;",
                                board_local_3, board_Therm_Alert_3?'H':'L');
        /*i2c_tmp451_3 parameter start*/
//        IfxStdIf_DPipe_print(io_tmp, "thermal len =[%d] "ENDL,buffer_used);

    }else{
        if(board.pcb_version >= PG2_A02){
            IfxPort_Pin *pPort;
            float board_local;
            boolean brd_alert;

            pPort = &(AURIX2_THERM_ALERT_1);
            board_local = readTsensorLocal(&i2c_tmp451_1);
            brd_alert = IfxPort_getPinState(pPort->port, pPort->pinIndex);
            buffer_used += snprintf(thermal_monitor_buffer+buffer_used, THERMAL_MONITOR_BUFFER_SIZE-buffer_used,
                                    "LOCAL_1=%.2f;THERM_ALERT_1=%c;",
                                    board_local, brd_alert?'H':'L');

            pPort = &(AURIX2_THERM_ALERT_2);
            board_local = readTsensorLocal(&i2c_tmp451_2);
            brd_alert = IfxPort_getPinState(pPort->port, pPort->pinIndex);
            buffer_used += snprintf(thermal_monitor_buffer+buffer_used, THERMAL_MONITOR_BUFFER_SIZE-buffer_used,
                                    "LOCAL_2=%.2f;THERM_ALERT_2=%c;",
                                    board_local, brd_alert?'H':'L');

            pPort = &(AURIX2_THERM_ALERT_3);
            board_local = readTsensorLocal(&i2c_tmp451_3);
            brd_alert = IfxPort_getPinState(pPort->port, pPort->pinIndex);
            buffer_used += snprintf(thermal_monitor_buffer+buffer_used, THERMAL_MONITOR_BUFFER_SIZE-buffer_used,
                                    "LOCAL_3=%.2f;THERM_ALERT_3=%c;",
                                    board_local, brd_alert?'H':'L');

            pPort = &(AURIX2_THERM_ALERT_4);
            board_local = readTsensorLocal(&i2c_tmp451_4);
            brd_alert = IfxPort_getPinState(pPort->port, pPort->pinIndex);
            buffer_used += snprintf(thermal_monitor_buffer+buffer_used, THERMAL_MONITOR_BUFFER_SIZE-buffer_used,
                                    "LOCAL_4=%.2f;THERM_ALERT_4=%c;",
                                    board_local, brd_alert?'H':'L');

            pPort = &(AURIX2_THERM_ALERT_5);
            board_local = readTsensorLocal(&i2c_tmp451_5);
            brd_alert = IfxPort_getPinState(pPort->port, pPort->pinIndex);
            buffer_used += snprintf(thermal_monitor_buffer+buffer_used, THERMAL_MONITOR_BUFFER_SIZE-buffer_used,
                                    "LOCAL_5=%.2f;THERM_ALERT_5=%c;",
                                    board_local, brd_alert?'H':'L');

//            IfxStdIf_DPipe_print(io_tmp, "thermal len =[%d] "ENDL,buffer_used);
        }
    }
}

static t_pgood_signals PAGASUS2_PGOOD_SIGNALS[]=
{
    {&IoExpPort_A74_P00, "XA_5V_PG", TRUE},
    {&IoExpPort_A74_P01, "N/A", FALSE},
    {&IoExpPort_A74_P02, "X1_PG/XA_VDD_[SOC/DDR2/DDRQ]_PG", TRUE},
    {&IoExpPort_A74_P03, "XA_VDD_[CPU/GPU/CV]_PG", TRUE},
    {&IoExpPort_A74_P04, "XB_5V_PG", TRUE},
    {&IoExpPort_A74_P05, "N/A", FALSE},
    {&IoExpPort_A74_P06, "X2_PG/XB_VDD_[SOC/DDR2/DDRQ]_PG", TRUE},
    {&IoExpPort_A74_P07, "XB_VDD_[CPU/GPU/CV]_PG", TRUE},

    {&IoExpPort_A74_P10, "SYS_5V_PG", TRUE},
    {&IoExpPort_A74_P11, "SYS_[0V85/1V0/1V2]_PG(A00) or CAM_1V2_PG(A01)", TRUE},
    {&IoExpPort_A74_P12, "SYS_[3V3/1V8/0V9]_PG1(A00) or CAM_1V2_PG(A01)", TRUE},
    {&IoExpPort_A74_P13, "SYS_[3V3/1V8/1V5/1V1/0V9]_PG2", TRUE},
    {&IoExpPort_A74_P14, "N/A", FALSE},
    {&IoExpPort_A74_P15, "N/A", FALSE},
    {&IoExpPort_A74_P16, "N/A", FALSE},
    {&IoExpPort_A74_P17, "N/A", FALSE},
};

void powergood_monitor_update(void)
{
    /* example
    "*PG*XA_5V=H;X1=H;VDD_SOC=H;VDD_DDR2=H;VDD_DDRQ=H;VDD_CPU=H;VDD_GPU=H;VDD_CV=H;XA_PREREG=L;XA_RTC=H;NW2=H;HIA_POC=H;SPA2_HIA_CIM=H;"
    */
    int buffer_used = 0;

    t_pgood_signals* ptr = &PAGASUS2_PGOOD_SIGNALS[0];
    uint8 i;
    boolean current_state = FALSE;
    buffer_used += snprintf(PowerGood_monitor_buffer+buffer_used, POWER_GOOD_MONITOR_BUFFER_SIZE-buffer_used,"*PG*");
    for(i=0; i<16; i++)
    {
        if(ptr[i].is_connected)
        {
            current_state = READ_GPIOEXP_STATE((*(ptr[i].pin)));
            if (current_state == TRUE){
                buffer_used += snprintf(PowerGood_monitor_buffer+buffer_used, POWER_GOOD_MONITOR_BUFFER_SIZE-buffer_used,
                        "%s=H;", ptr[i].name);
            }
            else{
                buffer_used += snprintf(PowerGood_monitor_buffer+buffer_used, POWER_GOOD_MONITOR_BUFFER_SIZE-buffer_used,
                        "%s=L;", ptr[i].name);
            }
        }
    }
    //IfxStdIf_DPipe_print(io_tmp, "powergood len =[%d] "ENDL,buffer_used);
}



static t_channel_display INA3221_CONFIG[6]=
{
    {&i2c_ina_1, 1, "XA_5V"},
    {&i2c_ina_1, 2, "XA_PREREG"},
    {&i2c_ina_1, 3, "SXMA_PREREG"},

    {&i2c_ina_2, 1, "XB_5V"},
    {&i2c_ina_2, 2, "XB_PREREG"},
    {&i2c_ina_2, 3, "SXMB_PREREG"},
};

void ina3221_monitor_update(void)
{
    int buffer_used = 0;
    float vshunt_ch, vbus_ch, current_ch, power_ch;
    uint8 i, channel_number;

    channel_number=sizeof(INA3221_CONFIG)/sizeof(INA3221_CONFIG[0]);

    buffer_used += snprintf(ina3221_monitor_buffer+buffer_used, INA3221_MONITOR_BUFFER_SIZE-buffer_used,"*INA3221*");
    for(i=0; i<channel_number; i++)
    {
        readPowerSensorINA3221(INA3221_CONFIG[i].dev_ptr, INA3221_CONFIG[i].index, &vshunt_ch, &vbus_ch, &current_ch, &power_ch);
        buffer_used += snprintf(ina3221_monitor_buffer+buffer_used, INA3221_MONITOR_BUFFER_SIZE-buffer_used,
            "%s=%.3f;", INA3221_CONFIG[i].name, power_ch);
    }
    //IfxStdIf_DPipe_print(io_tmp, "ina3221 len =[%d] "ENDL,buffer_used);
}

void max20087_monitor_update(void)
{
    int buffer_used = 0;
    float camvolData[4];
    float camcurrData[4];
    char devicename[5];
    char* name = devicename;

    buffer_used += snprintf(max20087_monitor_buffer+buffer_used, MAX20087_MONITOR_BUFFER_SIZE-buffer_used,"*POC*");

    //U1AA
    readCamCurrent(&i2c_cim_lsw_svc, camcurrData);
    readCamVoltage(&i2c_cim_lsw_svc, camvolData);
    name = "U1AA";
    buffer_used += snprintf(max20087_monitor_buffer+buffer_used, MAX20087_MONITOR_BUFFER_SIZE-buffer_used,
            "%s=%.3fV,%.3fA;",name, camvolData[0], camcurrData[0]);
    //U1AB
    readCamCurrent(&i2c_cim_lsw_cvc, camcurrData);
    readCamVoltage(&i2c_cim_lsw_cvc, camvolData);
    name = "U1AB";
    buffer_used += snprintf(max20087_monitor_buffer+buffer_used, MAX20087_MONITOR_BUFFER_SIZE-buffer_used,
            "%s=%.3fV,%.3fA;",name, camvolData[1], camcurrData[1]);
    //U1AC
    readCamCurrent(&i2c_cim_lsw_lrc_mrr, camcurrData);
    readCamVoltage(&i2c_cim_lsw_lrc_mrr, camvolData);
    name = "U1AC";
    buffer_used += snprintf(max20087_monitor_buffer+buffer_used, MAX20087_MONITOR_BUFFER_SIZE-buffer_used,
            "%s=%.3fV,%.3fA;",name, camvolData[2], camcurrData[2]);

    //U1AD
    readCamCurrent(&i2c_cim_lsw_lrr, camcurrData);
    readCamVoltage(&i2c_cim_lsw_lrc_mrr, camvolData);
    name = "U1AD";
    buffer_used += snprintf(max20087_monitor_buffer+buffer_used, MAX20087_MONITOR_BUFFER_SIZE-buffer_used,
            "%s=%.3fV,%.3fA;",name, camvolData[3], camcurrData[3]);

    //IfxStdIf_DPipe_print(io_tmp, "max20087 len =[%d] "ENDL,buffer_used);
}


void eth_monitor_update(void)
{
    uint32 value;
    uint8 i, temp;
    uint32 tmp_bit;
    uint32 tmp1_threshold, tmp2_threshold, tmp1_value, tmp2_value;
    int buffer_used = 0;

    buffer_used += snprintf(eth_monitor_buffer+buffer_used, ETH_MONITOR_BUFFER_SIZE-buffer_used,"*ETH_MONITOR*");

    /*88Q2112 TMP monitor start*/
    for (i = 0x01; i < 0x08; i++){
        IfxEth_Phy_88q2112_write_mdio_reg(i, 0x03, 0x8042, 0x4d00);
        IfxEth_Phy_88q2112_read_mdio_reg(i, 0x03, 0x8042, &value);
        waitTime(2*TimeConst[TIMER_INDEX_1MS]);
        if(value > 75){
        temp = (uint8)value -75;
        }else{
        temp = 0;
        }

        switch(i){
            case 0x01:
                buffer_used += snprintf(eth_monitor_buffer+buffer_used, ETH_MONITOR_BUFFER_SIZE-buffer_used,
                    "88Q2112(U178)=%d;", temp);
                break;
            case 0x02:
                buffer_used += snprintf(eth_monitor_buffer+buffer_used, ETH_MONITOR_BUFFER_SIZE-buffer_used,
                    "88Q2112(U181)=%d;", temp);
                break;
            case 0x03:
                buffer_used += snprintf(eth_monitor_buffer+buffer_used, ETH_MONITOR_BUFFER_SIZE-buffer_used,
                    "88Q2112(U183)=%d;", temp);
                break;
            case 0x04:
                buffer_used += snprintf(eth_monitor_buffer+buffer_used, ETH_MONITOR_BUFFER_SIZE-buffer_used,
                    "88Q2112(U185)=%d;", temp);
                break;
            case 0x05:
                buffer_used += snprintf(eth_monitor_buffer+buffer_used, ETH_MONITOR_BUFFER_SIZE-buffer_used,
                    "88Q2112(U160)=%d;", temp);
                break;
            case 0x06:
                buffer_used += snprintf(eth_monitor_buffer+buffer_used, ETH_MONITOR_BUFFER_SIZE-buffer_used,
                    "88Q2112(U162)=%d;", temp);
                break;
            case 0x07:
                buffer_used += snprintf(eth_monitor_buffer+buffer_used, ETH_MONITOR_BUFFER_SIZE-buffer_used,
                    "88Q2112(U176)=%d;", temp);
                break;
        }
    }
    /*88Q2112 TMP monitor end*/
#if 1
    /*88E6321 TMP monitor start */
    /*setting page address*/
    IfxEth_Sw_88E88E6321_write_phy_reg(0x10, 0x03, 0x16, 0x06);
    IfxEth_Sw_88E88E6321_write_phy_reg(0x12, 0x03, 0x16, 0x06);

    /*read tmp value*/
    IfxEth_Sw_88E88E6321_read_phy_reg(0x10, 0x03, 0x1A, &tmp_bit);
    tmp1_threshold = (uint8)(tmp_bit >> 8) * 5 - 25;
    IfxEth_Sw_88E88E6321_read_phy_reg(0x10, 0x03, 0x1B, &tmp_bit);
    tmp1_value = (uint8)(tmp_bit & 0x000000FF) - 25;
    buffer_used += snprintf(eth_monitor_buffer+buffer_used, ETH_MONITOR_BUFFER_SIZE-buffer_used,
        "88E6321(U173)=%d;", tmp1_value);

    IfxEth_Sw_88E88E6321_read_phy_reg(0x12, 0x03, 0x1A, &tmp_bit);
    tmp2_threshold = (uint8)(tmp_bit >> 8) * 5 - 25;
    IfxEth_Sw_88E88E6321_read_phy_reg(0x12, 0x03, 0x1B, &tmp_bit);
    tmp2_value = (uint8)(tmp_bit & 0x000000FF) - 25;
    buffer_used += snprintf(eth_monitor_buffer+buffer_used, ETH_MONITOR_BUFFER_SIZE-buffer_used,
        "88E6321(U169)=%d;", tmp2_value);
    /*88E6321 TMP monitor end */

    /*88Q5050 TMP monitor start */
    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x861b);
    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8023);
    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x4d00);
    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8423);
    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8c23);
    IfxEth_Marvell_Switch_read_reg(0x11, 0x1c, 0x19, &tmp_bit);
    temp = (uint8)(tmp_bit) -75;
    buffer_used += snprintf(eth_monitor_buffer+buffer_used, ETH_MONITOR_BUFFER_SIZE-buffer_used,
        "88Q5050p1=%d;", temp);

    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x861b);
    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8043);
    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x4d00);
    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8443);
    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8c43);
    IfxEth_Marvell_Switch_read_reg(0x11, 0x1c, 0x19, &tmp_bit);
    temp = (uint8)(tmp_bit) -75;
    buffer_used += snprintf(eth_monitor_buffer+buffer_used, ETH_MONITOR_BUFFER_SIZE-buffer_used,
        "88Q5050p2=%d;", temp);

    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x861b);
    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8063);
    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x4d00);
    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8463);
    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8c63);
    IfxEth_Marvell_Switch_read_reg(0x11, 0x1c, 0x19, &tmp_bit);
    temp = (uint8)(tmp_bit) -75;
    buffer_used += snprintf(eth_monitor_buffer+buffer_used, ETH_MONITOR_BUFFER_SIZE-buffer_used,
        "88Q5050p3=%d;", temp);

    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x861b);
    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8083);
    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x4d00);
    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8483);
    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8c83);
    IfxEth_Marvell_Switch_read_reg(0x11, 0x1c, 0x19, &tmp_bit);
    temp = (uint8)(tmp_bit) -75;
    buffer_used += snprintf(eth_monitor_buffer+buffer_used, ETH_MONITOR_BUFFER_SIZE-buffer_used,
        "88Q5050p4=%d;", temp);
#endif
    //IfxStdIf_DPipe_print(io_tmp, "eth len =[%d] "ENDL,buffer_used);
    /*88Q5050 TMP monitor end */
}


void fan_monitor_update(void)
{
    int buffer_used = 0;
    /*
    result example "*FAN*1=111;2=222;3=222;4=222;"
    */
    buffer_used += snprintf(fan_monitor_buffer, FAN_MONITOR_BUFFER_SIZE,
             "*FAN*1=%d;2=%d;3=%d;4=%d;",
             (int)rpm[0], (int)rpm[1], (int)rpm[2], (int)rpm[3]);
    //IfxStdIf_DPipe_print(io_tmp, "fan len =[%d] "ENDL, buffer_used);
}


void idrom_monitor_update(void)
{
    //interface: I2C
    /* example
    "*IDROM*0x00=0x4d;
    */
    int buffer_used = 0;
    uint8 i2c_module_number, devAddr, address;
    uint32 uiCnt, bytesleft;
    uint8 datas[2];

    memset(datas,0,2);
    i2c_module_number=0;//V31C has bus 0 only
    devAddr=0x54;//idrom slave addr
    address=68;//read register 0x00
    uiCnt=1;//read count
    bytesleft = i2c_read_bytes(i2c_module_number,devAddr, address, uiCnt, &datas[0],1);

    if( 0 == bytesleft){
        //successful
        buffer_used += snprintf(idrom_monitor_buffer+buffer_used, IDROM_MONITOR_BUFFER_SIZE-buffer_used, "*IDROM*PCBHB=0x%02x;",datas[0]);
    }else{
        //failed
        buffer_used += snprintf(idrom_monitor_buffer+buffer_used, IDROM_MONITOR_BUFFER_SIZE-buffer_used, "*IDROM*PCBHB=failed;");
    }

    address++;
    bytesleft = i2c_read_bytes(i2c_module_number,devAddr, address, uiCnt, &datas[1],1);

    if( 0 == bytesleft){
        //successful
        buffer_used += snprintf(idrom_monitor_buffer+buffer_used, IDROM_MONITOR_BUFFER_SIZE-buffer_used, "PCBLB=0x%02x;",datas[1]);
    }else{
        //failed
        buffer_used += snprintf(idrom_monitor_buffer+buffer_used, IDROM_MONITOR_BUFFER_SIZE-buffer_used, "PCBLB=failed;");
    }
    //IfxStdIf_DPipe_print(io_tmp, "idrom len =[%d] "ENDL,buffer_used);
}



#if USE_WC_PATTERN == 1
extern boolean GLOBAL_RUN_WC_PATTERN_TEST;
extern sint32 GLOBAL_WC_PATTERN_TIME_IN_SEC;
extern boolean GLOBAL_WC_PATTERN_CPU3;
extern App_AsclinShellInterface g_AsclinShellInterface;
static __inline__ void special_pattern (void *pmem)
{
/* *INDENT-OFF* */
  __asm__ __volatile__(
  "  mov.aa %%a15,%0 \n"
  "  movh   %%d15,HI:0xAAAAAAAA \n"
  "  addi   %%d15,%%d15,LO:0xAAAAAAAA \n"
  "  st.w   [%%a15+],%%d15 \n"
  "  st.w   [%%a15+],%%d15 \n"
  "  movh   %%d15,HI:0x55555555 \n"
  "  addi   %%d15,%%d15,LO:0x55555555 \n"
  "  st.w   [%%a15+],%%d15 \n"
  "  st.w   [%%a15+],%%d15 \n"
  "  movh   %%d15,HI:0x33333333 \n"
  "  addi   %%d15,%%d15,LO:0x33333333 \n"
  "  st.w   [%%a15+],%%d15 \n"
  "  st.w   [%%a15+],%%d15 \n"
  "  movh   %%d15,HI:0xcccccccc \n"
  "  addi   %%d15,%%d15,LO:0xcccccccc \n"
  "  st.w   [%%a15+],%%d15 \n"
  "  st.w   [%%a15+],%%d15 \n"
  "  movh   %%d15,HI:0x66666666 \n"
  "  addi   %%d15,%%d15,LO:0x66666666 \n"
  "  st.w   [%%a15+],%%d15 \n"
  "  st.w   [%%a15+],%%d15 \n"
  "  movh   %%d15,HI:0x99999999 \n"
  "  addi   %%d15,%%d15,LO:0x99999999 \n"
  "  st.w   [%%a15+],%%d15 \n"
  "  st.w   [%%a15+],%%d15 \n"
  "  movh   %%d15,HI:0xffffffff \n"
  "  addi   %%d15,%%d15,LO:0xffffffff \n"
  "  st.w   [%%a15+],%%d15 \n"
  "  st.w   [%%a15+],%%d15 \n"
  "  movh   %%d15,HI:0x00000000 \n"
  "  addi   %%d15,%%d15,LO:0x00000000 \n"
  "  st.w   [%%a15+],%%d15 \n"
  "  st.w   [%%a15+],%%d15 \n"
  "  mov.u  %%d0,0 \n"
  "  mov.u  %%d1,0 \n"
  "  mov.u  %%d2,0 \n"
  "  mov.u  %%d3,0 \n"
  "  mov.u  %%d4,0 \n"
  "  mov.u  %%d5,0 \n"
  "  mov.u  %%d6,0 \n"
  "  mov.u  %%d7,0 \n"
  "  mov.u  %%d8,0 \n"
  "  mov.u  %%d9,0 \n"
  "  mov.aa %%a15,%0 \n"
  "  movh   %%d15,HI:0x0000 \n"
  "  addi   %%d15,%%d15,LO:0x0100 \n"
  "  mov.a  %%a2,%%d15 \n"
  " .align 4   # align 2^4 bytes\n"
  "_power_loop: \n"
  "  madd.h %%e0,%%e8,%%d7,%%d6UL,1 \n"
  "  ld.d   %%e2,[%%a15]0x8 \n"
  "  madd.h %%e0,%%e8,%%d5,%%d4UL,1 \n"
  "  ld.d   %%e4,[%%a15]0x10 \n"
  "  madd.h %%e0,%%e8,%%d7,%%d6UL,1 \n"
  "  ld.d   %%e2,[%%a15]0x18 \n"
  "  madd.h %%e0,%%e8,%%d5,%%d4UL,1 \n"
  "  ld.d   %%e4,[%%a15]0x20 \n"
  "  madd.h %%e0,%%e8,%%d7,%%d6UL,1 \n"
  "  ld.d   %%e2,[%%a15]0x28 \n"
  "  madd.h %%e0,%%e8,%%d5,%%d4UL,1 \n"
  "  ld.d   %%e4,[%%a15]0x30 \n"
  "  madd.h %%e0,%%e8,%%d7,%%d6UL,1 \n"
  "  ld.d   %%e2,[%%a15]0x38 \n"
  "  loop   %%a2,_power_loop \n": : "a" (pmem):"a15", "d15", "a2", "d0", "d1", "d3", "d2", "d5", "d4", "d7", "d6", "d8", "d9");
/* *INDENT-ON* */
}
static void pattern(void)
{
    float64 *pmem;
    float64 wc_array[0x10];     //we need just an array of 0x10 32bit 64bit aligned values
    // This will be optimized, important is that pmem is 64bit aligned
    pmem = (float64 *) & wc_array[0];
    if ((((uint32) & wc_array[0]) & 0x00000007) != 0)
        pmem += 4;
    special_pattern (pmem);
}
static uint32 ref_pattern_time_adjustment = 10;
static sint32 wc_start_trigger(uint32 cpu_core_index, float32 STM_CLK, sint32 time_seconds)
{
    float32 tmp = 1.1;
    uint32 tmp_ticks;
    sint32 time_start;
    sint32 time_end;
    sint32 time;
    tmp = (float32) STM_CLK;
    tmp = 1 / tmp;
    tmp = ( (float32) 900 * (float32) 1000.0f / ((float32) tmp * (float32) 1e9f));
    tmp_ticks = (uint32) tmp;
    time_start = tmp_ticks * ref_pattern_time_adjustment;
    time = 0;
    time_end = (time_start + (tmp_ticks * time_seconds * ref_pattern_time_adjustment));
    int i = 0;
    while (time_end >= time_start)
    {
      i++;
      //if ((i % 1000) == 0)
      //      IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "[%d] i=%d, %d"ENDL,cpu_core_index,i,time_start);
      pattern ();
      time += ref_pattern_time_adjustment;
      time_start = tmp_ticks + time;
    }
    //IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "[%d] end"ENDL,cpu_core_index);
    return 1;
}

static sint32 Get_WC_Pattern_Time(){
    return GLOBAL_WC_PATTERN_TIME_IN_SEC;
}
#endif


extern uint32 current_value[4];
extern boolean do_main_aurix_thermal_test;
extern boolean do_sec_aurix_thermal_test;
extern boolean do_main_aurix_thermal_can_flex;
extern boolean do_main_aurix_thermal_eth;

void data_test();

void Main_Aurix_thermal_test_init(void)
{
    uint8 i;

    for (i = 0x01; i < 0x08; i++){
        IfxEth_Phy_88q2112_write_mdio_reg(i, 0x03, 0x8042, 0x4d00);
    }
    IfxMultican_SetFDMode(FALSE, 1000000);
    IfxMultican_SetSamplePoint(8000, 8000);
    IfxMultican_init(FALSE, 1000000, TRUE, FALSE);
    queue_dequeue_all_print_flag = TRUE; //enable canbus rx log
    if(g_Eray_run_mode==ERAY_TEST_MODE){
        IfxErayTestMode_init(FALSE, FALSE, 0xFFFF, IfxSrc_Tos_cpu0); /*Flexray TX initial*/
    }
    IfxStdIf_DPipe_print(io_tmp,"Initial Flexray TX \r\n");
    /*GMSL initial*/
    #if 0
    uint8 datas[1] = {0x1f};
    Resource_SetCameraAggState(CAMERA_MODULE_A, CAMERA_ON);
    Resource_SetCameraAggState(CAMERA_MODULE_B, CAMERA_ON);
    Resource_SetCameraAggState(CAMERA_MODULE_C, CAMERA_ON);
    Resource_SetCameraAggState(CAMERA_MODULE_D, CAMERA_ON);
    i2c_write_bytes(0, 0x2A, 0x01, 1, &datas[0]);
    i2c_write_bytes(0, 0x2B, 0x01, 1, &datas[0]);
    i2c_write_bytes(0, 0x2C, 0x01, 1, &datas[0]);
    i2c_write_bytes(0, 0x2D, 0x01, 1, &datas[0]);
    #endif
}

boolean check_thermal_main_aurix_scenario(void)
{
    if(do_main_aurix_thermal_test == FALSE)
        return FALSE;
    else
        return TRUE;
}

boolean check_thermal_sec_aurix_scenario(void)
{
    if(do_sec_aurix_thermal_test == FALSE)
        return FALSE;
    else
        return TRUE;
}


void Main_Aurix_thermal_counting(void)
{
    static uint8 initial_count = 0;
    static uint32 monitor_count = 1;
    static uint32 vib_count=0;

    initial_count++;
    if (initial_count > 10){
        if (disable_udp_cmd_flag == FALSE){
            IfxStdIf_DPipe_print(io_tmp, "monitor_count, %d \r\n", monitor_count);
            monitor_count++;
        //if(!do_main_aurix_vibration){
            //read_camera_poc();
        //}
        }else if(disable_udp_cmd_flag == TRUE){
            if((monitor_count%12)==0){
                vib_count++;
                IfxStdIf_DPipe_print(io_tmp, "monitor_count, %d \r\n", vib_count);
            }
        }
    }
    if(monitor_count>0xff) monitor_count = 10;
}

void Main_Aurix_thermal_test(void)
{
    /*setting TC397 temperature start*/
    SCU_DTSCLIM.U = 0x0CD846D6;
    /*setting TC397 temperature end*/

    static uint8 initial_count = 0;
//    static uint32 monitor_count = 1;
    /*TC397 parameter start*/
    double tc397_core_tmp, tc397_pms_tmp;
    uint32 value;
    uint8 i;
    sint8 temp88Q2112[7];
    sint8 temp88Q5050[4];

    pchar args_test;

    char current_item[4][25] = {"VBATSYS_ISENSE", "VBATTEG_ISENSE", "VBATSXMA_ISENSE", "VBATSXMB_ISENSE"};

    uint32 tmp_bit;
    uint32 tmp1_threshold, tmp2_threshold;
    sint32 tmp1_value, tmp2_value;
    static uint32_t status=0;
    if(!do_main_aurix_vibration){
        uint32 eray_data[2] = {0x55555555, 0x55555555};
        //IfxStdIf_DPipe_print(io_tmp,"xxxxxtest status is: %d\r\n", status);
        initial_count++;
        if (initial_count > 10){
            /*TMP451 monitor start*/
                //sem_wait_until_get(SEM_I2C_1, __LINE__, "Main_Aurix_thermal_test");
                ReadTsensor(io_tmp);
                //sem_release(SEM_I2C_1);
                /*TMP451 monitor end*/
                /*power monitor start*/
                //sem_wait_until_get(SEM_I2C_0, __LINE__, "Main_Aurix_thermal_test");
                Read_PowerMsgINA3221(io_tmp);
                //sem_release(SEM_I2C_0);
                /*power monitor start*/
                /*showvoltages monitor start*/
                ShowVoltages(args_test, &data_test, &io_tmp);
                for (i = 0; i < 4; i++)
                    IfxStdIf_DPipe_print(io_tmp,"%s, %d.%03d, A \r\n", current_item[i], current_value[i]/1000, current_value[i]%1000);
                /*showvoltages monitor end*/

        //1. read out Core Die Temperature Sensor value
        tc397_core_tmp = ((SCU_DTSCSTAT.U & 0x00000FFF) / 7.505) - 273.15;
        //2. read out PMS Die Temperature Sensor value
        tc397_pms_tmp = ((PMS_DTSSTAT.U & 0x00000FFF) / 7.505) - 273.15;
        IfxStdIf_DPipe_print(io_tmp,"tc397 DTS Core: %f, PMS: %f\r\n", tc397_core_tmp, tc397_pms_tmp);

        if (do_main_aurix_thermal_eth == TRUE)
        {
            /*88Q2112 TMP monitor start*/
            for (i = 0x01; i < 0x08; i++){
                IfxEth_Phy_88q2112_read_mdio_reg(i, 0x03, 0x8042, &value);
                temp88Q2112[i-1] = (sint8)((uint8)value -75);
            }
            IfxStdIf_DPipe_print(io_tmp,"88Q2112(U178)%d (U181)%d (U183)%d (U185)%d\r\n", temp88Q2112[0],temp88Q2112[1],temp88Q2112[2],temp88Q2112[3]);
            IfxStdIf_DPipe_print(io_tmp,"88Q2112(U160)%d (U162)%d (U176)%d\r\n", temp88Q2112[4],temp88Q2112[5],temp88Q2112[6]);
            /*88Q2112 TMP monitor end*/

            /*88E6321 TMP monitor start */
            /*setting page address*/
            IfxEth_Sw_88E88E6321_write_phy_reg(0x10, 0x03, 0x16, 0x06);
            IfxEth_Sw_88E88E6321_write_phy_reg(0x12, 0x03, 0x16, 0x06);

            /*read tmp value*/
            IfxEth_Sw_88E88E6321_read_phy_reg(0x10, 0x03, 0x1A, &tmp_bit);
            tmp1_threshold = (uint8)(tmp_bit >> 8) * 5 - 25;
            IfxEth_Sw_88E88E6321_read_phy_reg(0x10, 0x03, 0x1B, &tmp_bit);
            tmp1_value = (sint32)((uint8)(tmp_bit & 0x000000FF) - 25);

            IfxEth_Sw_88E88E6321_read_phy_reg(0x12, 0x03, 0x1A, &tmp_bit);
            tmp2_threshold = (uint8)(tmp_bit >> 8) * 5 - 25;
            IfxEth_Sw_88E88E6321_read_phy_reg(0x12, 0x03, 0x1B, &tmp_bit);
            tmp2_value = (sint32)((uint8)(tmp_bit & 0x000000FF) - 25);
            IfxStdIf_DPipe_print(io_tmp,"88E6321_1(U173)%d (U169)%d\r\n", tmp1_value, tmp2_value);
            /*88E6321 TMP monitor end */

            /*88Q5050 TMP monitor start */
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x861b);
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8023);
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x4d00);
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8423);
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8c23);
            IfxEth_Marvell_Switch_read_reg(0x11, 0x1c, 0x19, &tmp_bit);
            temp88Q5050[0] = (sint8)((uint8)(tmp_bit) -75);

            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x861b);
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8043);
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x4d00);
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8443);
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8c43);
            IfxEth_Marvell_Switch_read_reg(0x11, 0x1c, 0x19, &tmp_bit);
            temp88Q5050[1] = (sint8)((uint8)(tmp_bit) -75);

            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x861b);
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8063);
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x4d00);
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8463);
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8c63);
            IfxEth_Marvell_Switch_read_reg(0x11, 0x1c, 0x19, &tmp_bit);
            temp88Q5050[2] = (sint8)((uint8)(tmp_bit) -75);

            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x861b);
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8083);
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x4d00);
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8483);
            IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8c83);
            IfxEth_Marvell_Switch_read_reg(0x11, 0x1c, 0x19, &tmp_bit);
            temp88Q5050[3] = (sint8)((uint8)(tmp_bit) -75);
            IfxStdIf_DPipe_print(io_tmp,"88Q5050 %d %d %d %d degC\r\n", temp88Q5050[0],temp88Q5050[1],temp88Q5050[2],temp88Q5050[3]);
            /*88Q5050 TMP monitor end */
        }
        if (do_main_aurix_thermal_can_flex == TRUE)
        {
            /*CANBUS1 loopback start*/
            send_multican_message(0, 0, 0x55555555, 0x55555555, TRUE);
            //IfxStdIf_DPipe_print(io_tmp,"CAN1 set data \r\n");
            /*CANBUS1 loopback end*/

                    /*Flexray test start*/
                   if(g_Eray_run_mode==ERAY_TEST_MODE){
                        send_eray_frame(&eray_data[0], 2);
                        IfxStdIf_DPipe_print(io_tmp,"Eray Channel A send data: 0x%X, 0x%X \r\n", eray_data[0], eray_data[1]);
                    }
                    /*Flexray test end*/
                }
                //Switch_SJA1105_Register_Read(1, 0x100A00, &val);
                //Switch_SJA1105_Register_Read(1, 0x18, &val);
                //MAIN_ALWAYS_PRINTF(("Switch_SJA1105_Register_0x18_Read = 0x%04x \r\n", val));
                //monitor_count++;

        }
        if (initial_count == 0xff)
            initial_count = 11;
    }if(do_main_aurix_vibration){
        status++;
        //if (status>=12) status=0;
        if (status>=8) status=0;
        uint32 eray_data[2] = {0x55555555, 0x55555555};
        //IfxStdIf_DPipe_print(io_tmp,"xxxxxtest status is: %d\r\n", status);
        initial_count++;
        if (initial_count > 10){
            /*TMP451 monitor start*/
            if(status==0){
                //sem_wait_until_get(SEM_I2C_1, __LINE__, "Main_Aurix_thermal_test");
                ReadTsensor(io_tmp);
                //sem_release(SEM_I2C_1);
                /*TMP451 monitor end*/
            }
            if(status==1){
                /*power monitor start*/
                //sem_wait_until_get(SEM_I2C_0, __LINE__, "Main_Aurix_thermal_test");
                Read_PowerMsgINA3221(io_tmp);
                //sem_release(SEM_I2C_0);
                /*power monitor start*/
            }
            if(status==2){
                /*showvoltages monitor start*/
                ShowVoltages(args_test, &data_test, &io_tmp);
                for (i = 0; i < 4; i++)
                    IfxStdIf_DPipe_print(io_tmp,"%s, %d.%03d, A \r\n", current_item[i], current_value[i]/1000, current_value[i]%1000);
                /*showvoltages monitor end*/

                //1. read out Core Die Temperature Sensor value
                tc397_core_tmp = ((SCU_DTSCSTAT.U & 0x00000FFF) / 7.505) - 273.15;
                //2. read out PMS Die Temperature Sensor value
                tc397_pms_tmp = ((PMS_DTSSTAT.U & 0x00000FFF) / 7.505) - 273.15;
                IfxStdIf_DPipe_print(io_tmp,"tc397 DTS Core: %f, PMS: %f\r\n", tc397_core_tmp, tc397_pms_tmp);
            }
            if (do_main_aurix_thermal_eth == TRUE)
            {
                if(status==3){
                    /*88Q2112 TMP monitor start*/
                    for (i = 0x01; i < 0x08; i++){
                        //sem_wait_until_get(SEM_I2C_1, __LINE__, "Main_Aurix_thermal_test");
                        IfxEth_Phy_88q2112_read_mdio_reg(i, 0x03, 0x8042, &value);
                        //sem_release(SEM_I2C_1);  
                        temp88Q2112[i-1] = (sint8)((uint8)value -75);
                    }
                    IfxStdIf_DPipe_print(io_tmp,"88Q2112(U178)%d (U181)%d (U183)%d (U185)%d\r\n", temp88Q2112[0],temp88Q2112[1],temp88Q2112[2],temp88Q2112[3]);
                    IfxStdIf_DPipe_print(io_tmp,"88Q2112(U160)%d (U162)%d (U176)%d\r\n", temp88Q2112[4],temp88Q2112[5],temp88Q2112[6]);
                }
                /*88Q2112 TMP monitor end*/
                if(status==4){
                    /*88E6321 TMP monitor start */
                    /*setting page address*/
                    //sem_wait_until_get(SEM_I2C_1, __LINE__, "Main_Aurix_thermal_test");
                    IfxEth_Sw_88E88E6321_write_phy_reg(0x10, 0x03, 0x16, 0x06);
                    IfxEth_Sw_88E88E6321_write_phy_reg(0x12, 0x03, 0x16, 0x06);

                    /*read tmp value*/
                    IfxEth_Sw_88E88E6321_read_phy_reg(0x10, 0x03, 0x1A, &tmp_bit);
                    tmp1_threshold = (uint8)(tmp_bit >> 8) * 5 - 25;
                    IfxEth_Sw_88E88E6321_read_phy_reg(0x10, 0x03, 0x1B, &tmp_bit);
                    tmp1_value = (sint32)((uint8)(tmp_bit & 0x000000FF) - 25);

                    IfxEth_Sw_88E88E6321_read_phy_reg(0x12, 0x03, 0x1A, &tmp_bit);
                    tmp2_threshold = (uint8)(tmp_bit >> 8) * 5 - 25;
                    IfxEth_Sw_88E88E6321_read_phy_reg(0x12, 0x03, 0x1B, &tmp_bit);
                    tmp2_value = (sint32)((uint8)(tmp_bit & 0x000000FF) - 25);
                    IfxStdIf_DPipe_print(io_tmp,"88E6321_1(U173)%d (U169)%d\r\n", tmp1_value, tmp2_value);
                    /*88E6321 TMP monitor end */
                }
                if(status==5){
                    /*88Q5050 TMP monitor start */
                    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x861b);
                    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8023);
                    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x4d00);
                    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8423);
                    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8c23);
                    IfxEth_Marvell_Switch_read_reg(0x11, 0x1c, 0x19, &tmp_bit);
                    temp88Q5050[0] = (sint8)((uint8)(tmp_bit) -75);

                    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x861b);
                    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8043);
                    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x4d00);
                    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8443);
                    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8c43);
                    IfxEth_Marvell_Switch_read_reg(0x11, 0x1c, 0x19, &tmp_bit);
                    temp88Q5050[1] = (sint8)((uint8)(tmp_bit) -75);
					IfxStdIf_DPipe_print(io_tmp,"88Q5050 %d %d degC\r\n", temp88Q5050[0],temp88Q5050[1]);
                }
                if(status==6){
                    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x861b);
                    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8063);
                    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x4d00);
                    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8463);
                    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8c63);
                    IfxEth_Marvell_Switch_read_reg(0x11, 0x1c, 0x19, &tmp_bit);
                    temp88Q5050[2] = (sint8)((uint8)(tmp_bit) -75);

                    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x861b);
                    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8083);
                    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x19, 0x4d00);
                    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8483);
                    IfxEth_Marvell_Switch_write_reg(0x11, 0x1c, 0x18, 0x8c83);
                    IfxEth_Marvell_Switch_read_reg(0x11, 0x1c, 0x19, &tmp_bit);
                    //sem_release(SEM_I2C_1);  
                    temp88Q5050[3] = (sint8)((uint8)(tmp_bit) -75);
                    IfxStdIf_DPipe_print(io_tmp,"88Q5050 %d %d degC\r\n", temp88Q5050[2],temp88Q5050[3]);
                    /*88Q5050 TMP monitor end */
                }
            }
            if(status==7){
                if (do_main_aurix_thermal_can_flex == TRUE)
                {
                    /*CANBUS1 loopback start*/
                    send_multican_message(0, 0, 0x55555555, 0x55555555, TRUE);
                    //IfxStdIf_DPipe_print(io_tmp,"CAN1 set data \r\n");
                    /*CANBUS1 loopback end*/

                    /*Flexray test start*/
                   if(g_Eray_run_mode==ERAY_TEST_MODE){
                        send_eray_frame(&eray_data[0], 2);
                        IfxStdIf_DPipe_print(io_tmp,"Eray Channel A send data: 0x%X, 0x%X \r\n", eray_data[0], eray_data[1]);
                    }
                    /*Flexray test end*/
                }
                //Switch_SJA1105_Register_Read(1, 0x100A00, &val);
                //Switch_SJA1105_Register_Read(1, 0x18, &val);
                //MAIN_ALWAYS_PRINTF(("Switch_SJA1105_Register_0x18_Read = 0x%04x \r\n", val));
                //monitor_count++;
            }
            /*if(status==8){
                //read_camera_poc();
                AsclinShellInterface_execute_cmd("readcamvoltcurr", "0");
            }
            if(status==9){    AsclinShellInterface_execute_cmd("readcamvoltcurr", "1");}
            if(status==10){    AsclinShellInterface_execute_cmd("readcamvoltcurr", "2");}
            if(status==11){    AsclinShellInterface_execute_cmd("readcamvoltcurr", "3");}*/
        }
        if (initial_count == 0xff)
            initial_count = 11;
    }
}

void Second_Aurix_thermal_test(void)
{
    static uint8 initial_count = 0;
    static uint32 monitor_count = 1;
    /*TC397 parameter start*/
    double tc397_core_tmp, tc397_pms_tmp;

    /*setting TC397 temperature start*/
    SCU_DTSCLIM.U = 0x0CD846D6;
    /*setting TC397 temperature end*/

#if 1
    initial_count++;
    if (initial_count > 10){
        IfxStdIf_DPipe_print(io_tmp,"monitor_count, %d \r\n", monitor_count);

        /*TMP451 monitor start*/
        ReadTsensor(io_tmp);
        /*TMP451 monitor end*/

       /*TC397 tmp start*/
        //1. read out Core Die Temperature Sensor value
        tc397_core_tmp = ((SCU_DTSCSTAT.U & 0x00000FFF) / 7.505) - 273.15;
        IfxStdIf_DPipe_print(io_tmp,"tc397 Core DTS: %f degC\r\n", tc397_core_tmp);

        //2. read out PMS Die Temperature Sensor value
        tc397_pms_tmp = ((PMS_DTSSTAT.U & 0x00000FFF) / 7.505) - 273.15;
        IfxStdIf_DPipe_print(io_tmp,"tc397 PMS DTS: %f degC\r\n", tc397_pms_tmp);
        /*TC397 tmp end*/
       /*TC397 tmp end*/
       monitor_count++;
    }
    if (initial_count == 0xff)
       initial_count = 11;
#endif
}


static void STM3_Initial(void)
{
    /* initialize STM0 */
    IfxStm_CompareConfig stmCompareConfig;
    /* suspend by debugger enabled */
    IfxStm_enableOcdsSuspend(&MODULE_STM3);
    /* constructor of configuration */
    IfxStm_initCompareConfig(&stmCompareConfig);
    /* change the comparator configuration */
    stmCompareConfig.triggerPriority      = ISR_PRIORITY_STM3_TICK;
    stmCompareConfig.comparatorInterrupt  = IfxStm_ComparatorInterrupt_ir0; /*User must select the interrupt output */
    stmCompareConfig.ticks                = IFX_CFG_STM3_TICKS_PER_MS*10;    /* we start after 10ms */
    stmCompareConfig.typeOfService        = IfxSrc_Tos_cpu3;

    /* Now Compare functionality is initialized */
    IfxStm_initCompare(&MODULE_STM3, &stmCompareConfig);
}

IFX_INTERRUPT (UsrIsr_Stm_3, 3, ISR_PRIORITY_STM3_TICK);
void UsrIsr_Stm_3(void)
{
    /* Set next 1ms scheduler tick alarm */
    IfxStm_increaseCompare(&MODULE_STM3, IfxStm_Comparator_0, IFX_CFG_STM3_TICKS_PER_MS);

    /* increment the ms value */
    g_AppCpu3.TickCount_1ms++;

    /* Enable the global interrupts of this CPU */
    IfxCpu_enableInterrupts();

    eventFlag_1ms_stm_3 = TRUE;

     /* action to do each 10ms */
    if ((g_AppCpu3.TickCount_1ms % 10) == 0)
    {
        eventFlag_10ms_stm_3 = TRUE;
    }
     /* action to do each 10ms */
    /* action to do each 100ms */
    if ((g_AppCpu3.TickCount_1ms % 100) == 0)
    {
        eventFlag_100ms_stm_3 = TRUE;
    }
    if ((g_AppCpu3.TickCount_1ms % 1000) == 0)
    {
        eventFlag_1s_stm_3 = TRUE;
    }
}
/** \brief Main entry point for CPU3 */

#define MONITOR_FUNCTION(id, update_content_func) \
       do { \
          if (UdpMonitor_resource_is_ready(id) == FALSE){\
              update_content_func();  \
              UdpMonitor_notice(id); \
          } \
       } while(0)

void core3_handle_monitor_update(void)
{
    if(!UdpMonitor_is_enabled()) return;
//IfxStdIf_DPipe_print(io_tmp,"11 core3_handle_monitor_update, %d \r\n", core3_seconds_count++);
    if(board.aurix_index == MAIN_AURIX){

        //I2C1
        MONITOR_FUNCTION(eth_section_id, eth_monitor_update);

        //No HW
        MONITOR_FUNCTION(fan_section_id, fan_monitor_update);

        //I2C1
        MONITOR_FUNCTION(power_good_section_id, powergood_monitor_update);

        //I2C0
        MONITOR_FUNCTION(ina3221_section_id, ina3221_monitor_update);
        //I2C1
        MONITOR_FUNCTION(thermal_section_id,thermal_monitor_update);
        //I2C0
        MONITOR_FUNCTION(max20087_section_id, max20087_monitor_update);
    }else{
        if(board.pcb_version >= PG2_A02){
            //I2C1 + I2C0
            MONITOR_FUNCTION(thermal_section_id, thermal_monitor_update);
        }
    }
    //I2C0
    MONITOR_FUNCTION(idrom_section_id, idrom_monitor_update);
}


void read_camera_poc(void)
{
    AsclinShellInterface_execute_cmd("readcamvoltcurr", "0");
    AsclinShellInterface_execute_cmd("readcamvoltcurr", "1");
    AsclinShellInterface_execute_cmd("readcamvoltcurr", "2");
    AsclinShellInterface_execute_cmd("readcamvoltcurr", "3");
}

void core3_main(void)
{

    IfxCpu_enableInterrupts();
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    g_AppCpu3.info.pllFreq = IfxScuCcu_getPllFrequency();
    g_AppCpu3.info.cpuFreq = IfxScuCcu_getCpuFrequency(IfxCpu_getCoreId());
    g_AppCpu3.info.sysFreq = IfxScuCcu_getSpbFrequency();
    g_AppCpu3.info.stmFreq = IfxStm_getFrequency(&MODULE_STM3);
    g_AppCpu3.TickCount_1ms = 0;

    while(!system_ready)
    {
        IfxCpu_emitEvent(&cpuSyncEvent);
        IfxCpu_waitEvent(&cpuSyncEvent, 1);
    }

    if(board.aurix_index == MAIN_AURIX)
    {
        thermal_section_id = UdpMonitor_registor(thermal_monitor_buffer);
        power_good_section_id = UdpMonitor_registor(PowerGood_monitor_buffer);
        ina3221_section_id = UdpMonitor_registor(ina3221_monitor_buffer);
        eth_section_id = UdpMonitor_registor(eth_monitor_buffer);
        fan_section_id = UdpMonitor_registor(fan_monitor_buffer);
        max20087_section_id = UdpMonitor_registor(max20087_monitor_buffer);
        fan_detect_init(); 
    }else{
        if(board.pcb_version >= PG2_A02){
            thermal_section_id = UdpMonitor_registor(thermal_monitor_buffer);
        }
    }

    idrom_section_id = UdpMonitor_registor(idrom_monitor_buffer);
    
    STM3_Initial();
    while (1)
    {
        if(is_ota_start == TRUE){
            IfxCpu_emitEvent(&cpuSyncEvent);
            IfxCpu_waitEvent(&cpuSyncEvent, 500);
        }
		if(isDoOTARST == TRUE){
			waitTime(500*TimeConst[TIMER_INDEX_1MS]); // this is just a placeholder value for delay
            IfxStdIf_DPipe_print(io_tmp,"reoot. \r\n");
			aurixrst();
		}

        while(system_ready == 0){
            continue;
        }

    if(is_ota_start != TRUE){
#if USE_WC_PATTERN == 1
            if(GLOBAL_RUN_WC_PATTERN_TEST && GLOBAL_WC_PATTERN_CPU3)
            {
                wc_start_trigger(IfxCpu_getCoreId(), IfxStm_getFrequency(&MODULE_STM3), Get_WC_Pattern_Time());
                GLOBAL_WC_PATTERN_CPU3 = FALSE;
            }
#endif
            #if 1
            if(board.aurix_index == MAIN_AURIX){

                // Core 3 stop heart beat when Long Run test @ 2020/11/09
                // Move from Core3 10ms
                /*I2C bus0*/ Print_PowerMsgINA3221(&g_AsclinShellInterface.stdIf.asc);

                /*I2C bus1*/ Print_TemperatureMsg(&g_AsclinShellInterface.stdIf.asc);
            }
            #endif

            cpu3_idle_counter++;
            if (eventFlag_1ms_stm_3)
            {
                core_raise_count(IfxCpu_getCoreId());
                eventFlag_1ms_stm_3 = FALSE;
            }
            if (eventFlag_10ms_stm_3)
            {
                if(board.aurix_index == MAIN_AURIX){
                    /*I2C bus1*/
                    if(isDoShowVoltages==FALSE)
                        Monitor_Therm_Status();

                    /*I2C bus1+bus0*/ execute_thermal_shutdown();
                }
                else{
                    if(board.pcb_version >= PG2_A02){
                        /*I2C bus0 + bus1*/Print_TemperatureMsg(&g_AsclinShellInterface.stdIf.asc);
                        Monitor_Aurix2_Therm_Status();
                    }
                }

                //sem inside the function for more easily debugging
                core3_handle_monitor_update();
                eventFlag_10ms_stm_3 = FALSE;
            }
            if (eventFlag_100ms_stm_3)
            {
#if FAN_TACH_TIM_MEASURE_ENABLE == 1
                //for Main/Second Aurix => need to check, Aurix2 with IOexpander?
                if(board.aurix_index == MAIN_AURIX){
                    fan_detect();
                }
#endif
                eventFlag_100ms_stm_3 = FALSE;
                //IfxStdIf_DPipe_print(io_tmp,"11 core3_seconds_count, %d \r\n", core3_seconds_count++);
            }
            if (eventFlag_1s_stm_3)
            {
                eventFlag_1s_stm_3 = FALSE;
            }
        }
    }
}
