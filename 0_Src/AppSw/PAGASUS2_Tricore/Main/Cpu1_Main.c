/**
 * \file Cpu1_Main.c
 * \brief Main function definition for Cpu core 1 .
 *
 * \copyright Copyright (c) 2018 Infineon Technologies AG. All rights reserved.
 *
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

#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"

#include "tegra_console.h"
#include <AsclinShellInterface.h>
#include "Cpu_Main.h"
#include "IfxPms_reg.h"
#include "resource.h"
#include "core_alive_check.h"
#include "MulticanCmds.h"
extern IfxStdIf_DPipe *io_tmp;
//App_Cpu g_AppCpu1; /**< \brief CPU 1 global data */
static volatile boolean eventFlag_1ms_stm_1 = FALSE;
static volatile boolean eventFlag_10ms_stm_1 = FALSE;
static volatile boolean eventFlag_100ms_stm_1 = FALSE;
static volatile boolean eventFlag_1s_stm_1 = FALSE;
extern IfxCpu_syncEvent cpuSyncEvent;
boolean cpu1SyncError;
extern boolean is_ota_start;
extern volatile boolean system_ready;

#pragma section ".bss_cpu1" awc1
App_Cpu g_AppCpu1;
volatile uint32 cpu1_idle_counter=0;
char monitor_buffer[2000];  // store monitor data

#include "EvadcAutoScan.h"
int adc_section_id = FALSE;
//#define VOLTAGES_MONITOR_BUFFER_SIZE 620
#define VOLTAGES_MONITOR_BUFFER_SIZE 1600
volatile uint8 voltages_monitor_buffer[VOLTAGES_MONITOR_BUFFER_SIZE];
volatile int voltages_monitor_buffer_lens = 0;
//Aurix2, len = 49
#pragma section


#if USE_WC_PATTERN == 1
extern boolean GLOBAL_RUN_WC_PATTERN_TEST;
extern sint32 GLOBAL_WC_PATTERN_TIME_IN_SEC;
extern boolean GLOBAL_WC_PATTERN_CPU1;
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
	//IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "wc_start_trigger:%d,%f,%d"ENDL, cpu_core_index,STM_CLK,time_seconds);
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
	// easy case time is between start and end
	//IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "wc_start_trigger start %d"ENDL, cpu_core_index);
	//IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "wc_start_trigger time_start=%d"ENDL, time_start);
	//IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "wc_start_trigger time_end=%d"ENDL, time_end);
	int i = 0;
	while (time_end >= time_start)
	{
	  i++;
	  //if ((i % 1000) == 0)
	  //  IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "[%d] i=%d, %d"ENDL,cpu_core_index,i,time_start);
	  pattern ();
	  time += ref_pattern_time_adjustment;
	  time_start = tmp_ticks + time;
	}
	IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "[%d] end"ENDL,cpu_core_index);
	return 1;
}

static sint32 Get_WC_Pattern_Time(){
	return GLOBAL_WC_PATTERN_TIME_IN_SEC;
}
#endif


static void STM1_Initial(void)
{
    /* initialize STM0 */
    IfxStm_CompareConfig stmCompareConfig;
    /* suspend by debugger enabled */
    IfxStm_enableOcdsSuspend(&MODULE_STM1);
    /* constructor of configuration */
    IfxStm_initCompareConfig(&stmCompareConfig);
    /* change the comparator configuration */
    stmCompareConfig.triggerPriority      = ISR_PRIORITY_STM1_TICK;
    stmCompareConfig.comparatorInterrupt  = IfxStm_ComparatorInterrupt_ir0; /*User must select the interrupt output */
    stmCompareConfig.ticks                = IFX_CFG_STM1_TICKS_PER_MS*10;    /* we start after 10ms */
    stmCompareConfig.typeOfService        = IfxSrc_Tos_cpu1;

    /* Now Compare functionality is initialized */
    IfxStm_initCompare(&MODULE_STM1, &stmCompareConfig);
}

IFX_INTERRUPT (UsrIsr_Stm_1, 1, ISR_PRIORITY_STM1_TICK);

void UsrIsr_Stm_1(void)
{
    /* Set next 1ms scheduler tick alarm */
    IfxStm_increaseCompare(&MODULE_STM1, IfxStm_Comparator_0, IFX_CFG_STM1_TICKS_PER_MS);

    /* increment the ms value */
    g_AppCpu1.TickCount_1ms++;

    /* Enable the global interrupts of this CPU */
    IfxCpu_enableInterrupts();

    eventFlag_1ms_stm_1 = TRUE;

     /* action to do each 10ms */
    if ((g_AppCpu1.TickCount_1ms % 10) == 0)
    {
    	eventFlag_10ms_stm_1 = TRUE;
    }
    /* action to do each 100ms */
    if ((g_AppCpu1.TickCount_1ms % 100) == 0)
    {
    	eventFlag_100ms_stm_1 = TRUE;
    }
    if ((g_AppCpu1.TickCount_1ms % 1000) == 0)
    {
        eventFlag_1s_stm_1 = TRUE;
    }
}

int aurix_section_id = FALSE;
#define AURIX_DATA_MONITOR_BUFFER_SIZE 60
//AurixA, actually, 29
uint8 aurix_data_monitor_buffer[AURIX_DATA_MONITOR_BUFFER_SIZE];
void aurix_data_monitor_update(void)
{
    /* example
    "*PMIC*COUNT=0;"
    */
    int buffer_used = 0;
    double tc397_core_tmp, tc397_pms_tmp;

    //1. read out Core Die Temperature Sensor value
    tc397_core_tmp = ((SCU_DTSCSTAT.U & 0x00000FFF) / 7.505) - 273.15;
//    IfxStdIf_DPipe_print(io_tmp,"tc397 Core DTS: %f degC\r\n", tc397_core_tmp);

    //2. read out PMS Die Temperature Sensor value
    tc397_pms_tmp = ((PMS_DTSSTAT.U & 0x00000FFF) / 7.505) - 273.15;
//    IfxStdIf_DPipe_print(io_tmp,"tc397 PMS DTS: %f degC\r\n", tc397_pms_tmp);
    /*TC397 tmp end*/

    buffer_used += snprintf(aurix_data_monitor_buffer+buffer_used, AURIX_DATA_MONITOR_BUFFER_SIZE-buffer_used,
                            "*AUX*CTMP=%.3f;PTMP=%.3f;",tc397_core_tmp,tc397_pms_tmp);
//    IfxStdIf_DPipe_print(io_tmp, "aurixdata len =[%d] "ENDL,buffer_used);
}


/*
#define MONITOR_BUF_LEN 10
int monitor_section_id;
uint8 monitor_buffer[MONITOR_BUF_LEN];
uint32 monitor_section_count = 0;
*/

void voltages_monitor_update(void)
{
    static int buffer_used = 0;

    uint32 uiValue, current_channel, chnNr;
    uint8 count,sel1,sel0;
    IfxPort_Pin * pPortPinL;
    IfxPort_Pin * pPortPinH;
    boolean adc_shared = FALSE;
    char voltage_item[6][10] = {"INPUTS:", "AURIX:", "SYSTEM:", "Tegra A:", "Tegra B:", "B2B:"};
    float raw_value;

    count = 0;
    buffer_used = 0;
    uint32 max_pin_number = VadcAutoScan_getPinNumber();
    for (chnNr = 0; chnNr < max_pin_number; chnNr++)
    {

        adc_shared = FALSE;
/* If ADC_MUX_CTRL_3 is not defined, we don't need to check group 2 */
#if defined (ADC_MUX_CTRL_3) && defined (ADC_MUX_CTRL_4)
        if((IfxVadc_AN[chnNr].groupShared & GROUP_ID_MASK) == 2)
        {
            adc_shared = TRUE;
            pPortPinL = &(ADC_MUX_CTRL_3);
            pPortPinH = &(ADC_MUX_CTRL_4);
        }
        else if(IfxVadc_AN[chnNr].groupShared == 1)
#else
        if((IfxVadc_AN[chnNr].groupShared & GROUP_ID_MASK) == 1)
#endif
        {
            adc_shared = TRUE;
            pPortPinL = &(ADC_MUX_CTRL_1);
            pPortPinH = &(ADC_MUX_CTRL_2);
        }
        if (adc_shared)
        {
            //flip ADC mux selection
            count = (IfxVadc_AN[chnNr].groupShared & GROUP_SEL_MASK) >> 4;
            sel0 = count & 0x1;
            sel1 = (count & 0x2) >> 1;
            if (sel0)
            {
                IfxPort_setPinHigh(pPortPinL->port, pPortPinL->pinIndex);
            }
            else
            {
                IfxPort_setPinLow(pPortPinL->port, pPortPinL->pinIndex);
            }
            if (sel1)
            {
                IfxPort_setPinHigh(pPortPinH->port, pPortPinH->pinIndex);
            }
            else
            {
                IfxPort_setPinLow(pPortPinH->port, pPortPinH->pinIndex);
            }
            waitTime(1*TimeConst[TIMER_INDEX_1MS]);
        }

        current_channel = IfxVadc_AN[chnNr].pinIndex;
        if(current_channel >= MAX_ANALOG_CHANNEL)
        {/* we support channel larger than max valid channel, to display some meaningful helper message */
            if(current_channel == DISPLAY_ONLY_CHANNEL)
            {
                buffer_used += snprintf(voltages_monitor_buffer+buffer_used, VOLTAGES_MONITOR_BUFFER_SIZE-buffer_used, "*%s*", IfxVadc_AN[chnNr].railName);
            }
            continue;
        }
        while(1)
        {
            sem_wait_until_get(SEM_ADC, __LINE__, __func__);
            boolean valid_flag = VadcAutoScan_getResult(current_channel, &uiValue);
            sem_release(SEM_ADC);
            if (valid_flag)
            {
                raw_value = (float)uiValue/4095*5;
                if((strcmp(IfxVadc_AN[chnNr].railName, "snn") == 0) ||
                    (strcmp(IfxVadc_AN[chnNr].railName, "SNN") == 0))
                { /* we ignore the snn rail name */
                    break;
                }
                if (IfxVadc_AN[chnNr].adjustment2_start_value > 0)
                {
                    if (raw_value > IfxVadc_AN[chnNr].adjustment2_start_value)
                    {
                        uint32 mvVoltage =
                                (uint32)(((raw_value*((IfxVadc_AN[chnNr].Rtop +IfxVadc_AN[chnNr].Rbottom)/IfxVadc_AN[chnNr].Rbottom)
                                        * IfxVadc_AN[chnNr].multiplier2) + IfxVadc_AN[chnNr].offset2) * 1000);
                            buffer_used += snprintf(voltages_monitor_buffer+buffer_used, VOLTAGES_MONITOR_BUFFER_SIZE-buffer_used,"%s=%d.%03d %s;"ENDL,
                                IfxVadc_AN[chnNr].railName, mvVoltage/1000, mvVoltage%1000, IfxVadc_AN[chnNr].unit);
                        break;
                    }
                }

                uint32 mvVoltage =
                        (uint32)(((raw_value*((IfxVadc_AN[chnNr].Rtop +IfxVadc_AN[chnNr].Rbottom)/IfxVadc_AN[chnNr].Rbottom)
                                * IfxVadc_AN[chnNr].multiplier) + IfxVadc_AN[chnNr].offset)*1000);

#if 1
                buffer_used += snprintf(voltages_monitor_buffer+buffer_used, VOLTAGES_MONITOR_BUFFER_SIZE-buffer_used,"%s=%d.%03d;",
                    IfxVadc_AN[chnNr].railName, mvVoltage/1000, mvVoltage%1000);
#else
                /*INPUTS and aurix*/
                if (IfxVadc_AN[chnNr].pinIndex < 32 && IfxVadc_AN[chnNr].pinIndex > -1 )
                {
                    buffer_used += snprintf(voltages_monitor_buffer+buffer_used, VOLTAGES_MONITOR_BUFFER_SIZE-buffer_used,"%s=%d.%03d;",
                        IfxVadc_AN[chnNr].railName, mvVoltage/1000, mvVoltage%1000);
                }
                /*B2B*/
                //if (IfxVadc_AN[chnNr].pinIndex < 39 && IfxVadc_AN[chnNr].pinIndex > 33)
                if (IfxVadc_AN[chnNr].pinIndex < 35 && IfxVadc_AN[chnNr].pinIndex > 33)
                {
                    buffer_used += snprintf(voltages_monitor_buffer+buffer_used, VOLTAGES_MONITOR_BUFFER_SIZE-buffer_used,"%s=%d.%03d;",
                        IfxVadc_AN[chnNr].railName, mvVoltage/1000, mvVoltage%1000);
                }
#endif
                break;
            }
            else
            {
                //IfxStdIf_DPipe_print(io, "  chan %d flag is not set"ENDL, current_channel);
                break;
            }
        }
    }
    voltages_monitor_buffer_lens = buffer_used;
//    IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "adc len =[%d] "ENDL,voltages_monitor_buffer_lens);
}


static void core1_handle_monitor_update(void)
{
    if(!UdpMonitor_is_enabled()) return;
//IfxStdIf_DPipe_print(io_tmp,"11 core3_handle_monitor_update, %d \r\n", core3_seconds_count++);

    if(UdpMonitor_resource_is_ready(aurix_section_id) == FALSE){
        aurix_data_monitor_update();
        UdpMonitor_notice(aurix_section_id);
    }


    if(adc_section_id == FALSE){
        voltages_monitor_update();
        adc_section_id = TRUE;
    }
}

void core1_main (void)
{

    /*
     * !!WATCHDOG1 IS DISABLED HERE!!
     * Enable the watchdog in the demo if it is required and also service the watchdog periodically
     * */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());

    /* Initialise the application state */
    g_AppCpu1.info.pllFreq = IfxScuCcu_getPllFrequency();
    g_AppCpu1.info.cpuFreq = IfxScuCcu_getCpuFrequency(IfxCpu_getCoreId());
    g_AppCpu1.info.sysFreq = IfxScuCcu_getSpbFrequency();
    g_AppCpu1.info.stmFreq = IfxStm_getFrequency(&MODULE_STM1);
    g_AppCpu1.TickCount_1ms = 0;
    IfxCpu_enableInterrupts();

    while(!system_ready)
    {
        IfxCpu_emitEvent(&cpuSyncEvent);
        cpu1SyncError = IfxCpu_waitEvent(&cpuSyncEvent, 1);
    }
    aurix_section_id = UdpMonitor_registor(aurix_data_monitor_buffer);
    STM1_Initial();

    while(1)
    {
        if(is_ota_start != TRUE){
#if USE_WC_PATTERN == 1
            if(GLOBAL_RUN_WC_PATTERN_TEST && GLOBAL_WC_PATTERN_CPU1)
            {
                wc_start_trigger(IfxCpu_getCoreId(), IfxStm_getFrequency(&MODULE_STM1), Get_WC_Pattern_Time());
                GLOBAL_WC_PATTERN_CPU1 = FALSE;
            }
#endif

            while(system_ready == 0){
                continue;
            }


            if (eventFlag_1ms_stm_1)
            {
                core_raise_count(IfxCpu_getCoreId()); 
                eventFlag_1ms_stm_1 = FALSE;
                cpu1_can_test();
            }
            if (eventFlag_10ms_stm_1)
            {
                eventFlag_10ms_stm_1 = FALSE;
            }
            if (eventFlag_100ms_stm_1)
            {
                core1_handle_monitor_update();
                eventFlag_100ms_stm_1 = FALSE;
            }
            if (eventFlag_1s_stm_1)
            {
                eventFlag_1s_stm_1 = FALSE;
            }
            cpu1_idle_counter++;
        }
    }
}
