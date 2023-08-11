/**
 * \file Cpu2_Main.c
 * \brief CPU2 functions.
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
#include "Cpu_Main.h"
#include "core_alive_check.h"
#include "MulticanCmds.h"
/******************************************************************************/
/*------------------------Inline Function Prototypes--------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
extern volatile boolean system_ready;
extern IfxCpu_syncEvent cpuSyncEvent;
#pragma section ".bss_cpu2" awc2
App_Cpu g_AppCpu2;
volatile uint32 cpu2_idle_counter=0;
volatile boolean eventFlag_1ms_stm_2 = FALSE;
volatile boolean eventFlag_10ms_stm_2 = FALSE;
volatile boolean eventFlag_100ms_stm_2 = FALSE;
volatile boolean eventFlag_1s_stm_2 = FALSE;

extern boolean is_ota_start;

#pragma section
/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

#if USE_WC_PATTERN == 1
extern boolean GLOBAL_RUN_WC_PATTERN_TEST;
extern sint32 GLOBAL_WC_PATTERN_TIME_IN_SEC;
extern boolean GLOBAL_WC_PATTERN_CPU2;
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

static void STM2_Initial(void)
{
    /* initialize STM0 */
    IfxStm_CompareConfig stmCompareConfig;
    /* suspend by debugger enabled */
    IfxStm_enableOcdsSuspend(&MODULE_STM2);
    /* constructor of configuration */
    IfxStm_initCompareConfig(&stmCompareConfig);
    /* change the comparator configuration */
    stmCompareConfig.triggerPriority      = ISR_PRIORITY_STM2_TICK;
    stmCompareConfig.comparatorInterrupt  = IfxStm_ComparatorInterrupt_ir0; /*User must select the interrupt output */
    stmCompareConfig.ticks                = IFX_CFG_STM2_TICKS_PER_MS*10;    /* we start after 10ms */
    stmCompareConfig.typeOfService        = IfxSrc_Tos_cpu2;

    /* Now Compare functionality is initialized */
    IfxStm_initCompare(&MODULE_STM2, &stmCompareConfig);
}

IFX_INTERRUPT (UsrIsr_Stm_2, 2, ISR_PRIORITY_STM2_TICK);

void UsrIsr_Stm_2(void)
{
    /* Set next 1ms scheduler tick alarm */
    IfxStm_increaseCompare(&MODULE_STM2, IfxStm_Comparator_0, IFX_CFG_STM2_TICKS_PER_MS);

    /* increment the ms value */
    g_AppCpu2.TickCount_1ms++;

    /* Enable the global interrupts of this CPU */
    IfxCpu_enableInterrupts();

    eventFlag_1ms_stm_2 = TRUE;

     /* action to do each 10ms */
    if ((g_AppCpu2.TickCount_1ms % 10) == 0)
    {
        eventFlag_10ms_stm_2 = TRUE;
    }
    /* action to do each 100ms */
    if ((g_AppCpu2.TickCount_1ms % 100) == 0)
    {
        eventFlag_100ms_stm_2 = TRUE;
    }
    if ((g_AppCpu2.TickCount_1ms % 1000) == 0)
    {
        eventFlag_1s_stm_2 = TRUE;
    }
}




/** \brief Main entry point for CPU2 */
void core2_main(void)
{

    IfxCpu_enableInterrupts();
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    g_AppCpu2.info.pllFreq = IfxScuCcu_getPllFrequency();
    g_AppCpu2.info.cpuFreq = IfxScuCcu_getCpuFrequency(IfxCpu_getCoreId());
    g_AppCpu2.info.sysFreq = IfxScuCcu_getSpbFrequency();
    g_AppCpu2.info.stmFreq = IfxStm_getFrequency(&MODULE_STM2);
    g_AppCpu2.TickCount_1ms = 0;

    while(!system_ready)
    {
        IfxCpu_emitEvent(&cpuSyncEvent);
        IfxCpu_waitEvent(&cpuSyncEvent, 1);
    }
    STM2_Initial();

    while (1)
    {
        if(is_ota_start != TRUE){
#if USE_WC_PATTERN == 1
            if(GLOBAL_RUN_WC_PATTERN_TEST && GLOBAL_WC_PATTERN_CPU2)
            {
                wc_start_trigger(IfxCpu_getCoreId(), IfxStm_getFrequency(&MODULE_STM2), Get_WC_Pattern_Time());
                GLOBAL_WC_PATTERN_CPU2 = FALSE;
            }
#endif
            while(system_ready == 0){
                continue;
            }


            /* CPU2 routine job */
            if(eventFlag_1ms_stm_2){
                core_raise_count(IfxCpu_getCoreId()); 
                cpu2_can_test();
                eventFlag_1ms_stm_2 = FALSE;
                
            }
            if(eventFlag_10ms_stm_2){
                eventFlag_10ms_stm_2 = FALSE;
            }
            if(eventFlag_100ms_stm_2){
                eventFlag_100ms_stm_2 = FALSE;
            }
            if(eventFlag_1s_stm_2){
                eventFlag_1s_stm_2 = FALSE;
            }
            cpu2_idle_counter++;
        }
    }
}
