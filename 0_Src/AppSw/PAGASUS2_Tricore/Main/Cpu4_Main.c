/**
 * \file Cpu4_Main.c
 * \brief CPU4 functions.
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
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include <AsclinShellInterface.h>
#include "TLF35584Cmds.h"
#include "Cpu_Main.h"
#include "Board.h"
#include "resource.h"
#include "core_alive_check.h"
#include "tegra_console.h"
#include <string.h>

extern volatile boolean system_ready;
extern IfxCpu_syncEvent cpuSyncEvent;
extern IfxStdIf_DPipe *io_tmp;
#pragma section ".bss_cpu4" awc4
App_Cpu g_AppCpu4;
volatile uint32 cpu4_idle_counter=0;
static volatile boolean eventFlag_1ms_stm_4 = FALSE;
static volatile boolean eventFlag_10ms_stm_4 = FALSE;
static volatile boolean eventFlag_100ms_stm_4 = FALSE;
static volatile boolean eventFlag_1s_stm_4 = FALSE;
extern boolean is_ota_start;
#pragma section


int pmic_section_id = FALSE;
#define PMIC_MONITOR_BUFFER_SIZE 20
//actually, 16 => ok
uint8 pmic_monitor_buffer[PMIC_MONITOR_BUFFER_SIZE];
void pmic_monitor_update(void)
{
    /* example
    "*PMIC*COUNT=0;"
    */
    uint8 tlfdata;
    int buffer_used = 0;

    tlfdata = IfxTLF35584_simplified_read_write(FALSE, (uint8)0x01, 0x00);
    buffer_used += snprintf(pmic_monitor_buffer+buffer_used, PMIC_MONITOR_BUFFER_SIZE-buffer_used,
                            "*PMIC*TLFDATA=%d;",tlfdata);
    //IfxStdIf_DPipe_print(io_tmp, "pmic len =[%d] "ENDL,buffer_used);
}


#if USE_WC_PATTERN == 1
extern boolean GLOBAL_RUN_WC_PATTERN_TEST;
extern sint32 GLOBAL_WC_PATTERN_TIME_IN_SEC;
extern boolean GLOBAL_WC_PATTERN_CPU4;
extern App_AsclinShellInterface g_AsclinShellInterface;
extern volatile boolean eventFlag_100ms_tlf_wd_printing;
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
	  //	  IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "[%d] i=%d, %d"ENDL,cpu_core_index,i,time_start);
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

static void STM4_Initial(void)
{
    /* initialize STM0 */
    IfxStm_CompareConfig stmCompareConfig;
    /* suspend by debugger enabled */
    IfxStm_enableOcdsSuspend(&MODULE_STM4);
    /* constructor of configuration */
    IfxStm_initCompareConfig(&stmCompareConfig);
    /* change the comparator configuration */
    stmCompareConfig.triggerPriority      = ISR_PRIORITY_STM4_TICK;
    stmCompareConfig.comparatorInterrupt  = IfxStm_ComparatorInterrupt_ir0; /*User must select the interrupt output */
    stmCompareConfig.ticks                = IFX_CFG_STM4_TICKS_PER_MS*10;    /* we start after 10ms */
    stmCompareConfig.typeOfService        = IfxSrc_Tos_cpu4;

    /* Now Compare functionality is initialized */
    IfxStm_initCompare(&MODULE_STM4, &stmCompareConfig);
}

IFX_INTERRUPT (UsrIsr_Stm_4, 4, ISR_PRIORITY_STM4_TICK);

void UsrIsr_Stm_4(void)
{
    /* Set next 1ms scheduler tick alarm */
    IfxStm_increaseCompare(&MODULE_STM4, IfxStm_Comparator_0, IFX_CFG_STM4_TICKS_PER_MS);

    /* increment the ms value */
    g_AppCpu4.TickCount_1ms++;

    /* Enable the global interrupts of this CPU */
    IfxCpu_enableInterrupts();

    eventFlag_1ms_stm_4 = TRUE;

     /* action to do each 10ms */
    if ((g_AppCpu4.TickCount_1ms % 10) == 0)
    {
    	eventFlag_10ms_stm_4 = TRUE;
    }
    /* action to do each 100ms */
    if ((g_AppCpu4.TickCount_1ms % 100) == 0)
    {
        eventFlag_100ms_stm_4 = TRUE;
        eventFlag_100ms_tlf_wd_printing = TRUE;
    }
    if ((g_AppCpu4.TickCount_1ms % 1000) == 0)
    {
        eventFlag_1s_stm_4 = TRUE;
    }
}
void core4_handle_monitor_update(void)
{
    if(!UdpMonitor_is_enabled()) return;
//IfxStdIf_DPipe_print(io_tmp,"11 core3_handle_monitor_update, %d \r\n", core3_seconds_count++);
    if(UdpMonitor_resource_is_ready(pmic_section_id) == FALSE){
        FUNC_TAKE_SEM(pmic_monitor_update, SEM_SPI_2, "pmic_monitor_update");
        UdpMonitor_notice(pmic_section_id);
    }
}


/** \brief Main entry point for CPU4  */
void core4_main(void)
{

    IfxCpu_enableInterrupts();
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    g_AppCpu4.info.pllFreq = IfxScuCcu_getPllFrequency();
    g_AppCpu4.info.cpuFreq = IfxScuCcu_getCpuFrequency(IfxCpu_getCoreId());
    g_AppCpu4.info.sysFreq = IfxScuCcu_getSpbFrequency();
    g_AppCpu4.info.stmFreq = IfxStm_getFrequency(&MODULE_STM4);
    g_AppCpu4.TickCount_1ms = 0;

    while(!system_ready)
    {
        IfxCpu_emitEvent(&cpuSyncEvent);
        IfxCpu_waitEvent(&cpuSyncEvent, 1);
    }

    pmic_section_id = UdpMonitor_registor(pmic_monitor_buffer);
    STM4_Initial();
    while (1)
    {
        if(is_ota_start != TRUE){

#if USE_WC_PATTERN == 1
            if(GLOBAL_RUN_WC_PATTERN_TEST && GLOBAL_WC_PATTERN_CPU4)
            {
                wc_start_trigger(IfxCpu_getCoreId(), IfxStm_getFrequency(&MODULE_STM4), Get_WC_Pattern_Time());
                GLOBAL_WC_PATTERN_CPU4 = FALSE;
            }
#endif
/*
        if(check_core_shell_enabled(IfxCpu_getCoreId()) == TRUE){
            Ifx_Shell_process(&g_AsclinShellInterface.shell);
        }
*/
            while(system_ready == 0){
                continue;
            }


            if (eventFlag_1ms_stm_4)
            {
                core_raise_count(IfxCpu_getCoreId()); 
                eventFlag_1ms_stm_4 = FALSE;
            }
            if (eventFlag_10ms_stm_4)
            {
            //just Core5(high priority) and Core4(low priority) implement emergency UART console
/*
            if(roll_call_cores_status_check(CORE5) == HANG_DECTION){
                roll_call_cores_status_check_all();
                core_shell_enable(IfxCpu_getCoreId());
            }
*/
                eventFlag_10ms_stm_4 = FALSE;
            }
            if (eventFlag_100ms_stm_4)
            {
                core4_handle_monitor_update();
                sem_wait_until_get(SEM_SPI_2, __LINE__, __func__);
                Print_TLF_WD_Status(&g_AsclinShellInterface.stdIf.asc);
                sem_release(SEM_SPI_2);
                eventFlag_100ms_stm_4 = FALSE;
            }
            if (eventFlag_1s_stm_4){
                if(board.aurix_index == MAIN_AURIX){
                    TLF_regular_check();
                }
                else{}
                eventFlag_1s_stm_4 = FALSE;
            }
            cpu4_idle_counter++;
        }
    }
}
