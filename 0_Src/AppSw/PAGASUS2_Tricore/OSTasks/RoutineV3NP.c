#include "RoutineV3NP.h"
#include "Routine.h"
#include "Platform_Types.h"
#include "board.h"
#include "resource.h"
#include "core_alive_check.h"
#if CANBUS_STRESS_DISBM_ENABLE == 1
#include "Mcmcan.h"
#include "MulticanCmds.h"
#endif
#include <AsclinShellInterface.h>
#include <cangateway.h>
#include "ErayNormalDemo.h"
#include "system_monitor.h"
#include "ota.h"
#include "MulticanCmds.h"
#include "core_alive_check.h"


static uint8 delay_count = 0;
static unsigned char B2B_start_count = 0;
extern Eray_run_mode g_Eray_run_mode;
extern volatile boolean can_cnt_flag;
#define B2B_INIT_WAIT_SEC 23
#define B2B_FINISHED_FLAG 99
extern boolean is_b2b_x86_v3nc;
extern boolean is_b2b_pg;
#if USE_WC_PATTERN == 1
extern boolean GLOBAL_RUN_WC_PATTERN_TEST;
extern sint32 GLOBAL_WC_PATTERN_TIME_IN_SEC;
extern boolean GLOBAL_WC_PATTERN_CPU0;
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
static void always_main(void)
{
    if(g_Eray_run_mode==ERAY_NORMAL_MODE_TEST_S1||g_Eray_run_mode==ERAY_NORMAL_MODE_TEST_S2){
       eray_poc_state_monitor();
    }
    // update pmic status by the power voltage // old coomment says "not support"
// #if PMIC_MONITORING_ENABLE == 1
//     Monitor_System_Status();
// #endif

    //move to os task  // #if0 in old code, don't know why
// #if ISR_PROVIDER_ETHERNET == 0
//     Ifx_Lwip_pollTimerFlags();
//     Ifx_Lwip_pollReceiveFlags();
// #endif

#if USE_WC_PATTERN == 1
    if(GLOBAL_RUN_WC_PATTERN_TEST && GLOBAL_WC_PATTERN_CPU0)
    {
        wc_start_trigger(IfxCpu_getCoreId(), IfxStm_getFrequency(&MODULE_STM0), Get_WC_Pattern_Time());
        GLOBAL_WC_PATTERN_CPU0 = FALSE;
    }
#endif
}

static void every_1ms_main(void)
{
    core_raise_count(IfxCpu_getCoreId());
    sem_wait_until_get(SEM_ADC, __LINE__, __func__);
    System_Monitor_KL30_Routine();
    sem_release(SEM_ADC);
}

static void every_10ms_main(void)
{
#if CANBUS_STRESS_DISBM_ENABLE == 1
    if (can_cnt_flag)
        canstress_run();
#endif
}

static void every_100ms_main(void)
{
    // toogle_Led
    IfxPort_togglePin(AURIX_LED2.port, AURIX_LED2.pinIndex);
    TegraEthComm_routine();
    OTA_TegraEthComm_routine();
}

static void every_1s_main(void)
{
    cansend_conti_do();
    if(B2B_start_count <= B2B_INIT_WAIT_SEC){
        B2B_start_count++;
    }else{
        if(B2B_start_count != B2B_FINISHED_FLAG){
            if(!is_b2b_x86_v3nc){
                AsclinShellInterface_execute_cmd("b2b", "config");
            }else if(is_b2b_x86_v3nc){
                is_b2b_pg=b2b_get_board_powergood_status();
                IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "B2B Power Good is %d "ENDL,is_b2b_pg);
            }
            B2B_start_count = B2B_FINISHED_FLAG;
        }
    }
    if(delay_count < MESSAGE_DELAY_TIME_S)
    {
        delay_count++;
    }
    else if(delay_count == MESSAGE_DELAY_TIME_S)
    {
        // AsclinShellInterface_Print_Delay_Message's implemetation is marked #if0 So it is useless, just comment it
        // AsclinShellInterface_Print_Delay_Message();
        report_error_log();
        delay_count = 0xff;
    }

    if(board.aurix_index == MAIN_AURIX){
        if(TRUE == check_thermal_main_aurix_scenario()){
            Main_Aurix_thermal_counting();
//         sem_wait_until_get(SEM_I2C_0, __LINE__, "Main_Aurix_thermal_test");
//         sem_wait_until_get(SEM_I2C_1, __LINE__, "Main_Aurix_thermal_test");
            Main_Aurix_thermal_test();
//         sem_release(SEM_I2C_1);
//         sem_release(SEM_I2C_0);
        }
    }else{
        if(TRUE == check_thermal_sec_aurix_scenario()){
            sem_wait_until_get(SEM_I2C_0, __LINE__, "every_1s_second");
            sem_wait_until_get(SEM_I2C_1, __LINE__, "every_1s_second");
            Second_Aurix_thermal_test();
            sem_release(SEM_I2C_1);
            sem_release(SEM_I2C_0);
        }
    }
}


static void always_second(void)
{
    // #if0 in old code, don't know why
#if 0
//move to os task
#if ISR_PROVIDER_ETHERNET == 0
        Ifx_Lwip_pollTimerFlags();
        Ifx_Lwip_pollReceiveFlags();
#endif
#endif
}

static void every_1ms_second(void)
{
    core_raise_count(IfxCpu_getCoreId());
    if (cangen_enable){
        cansendtest();
    }
}
static void every_10ms_second(void)
{
#if CANBUS_STRESS_DISBM_ENABLE == 1
    if (can_cnt_flag)
        canstress_run();
#endif
}
static void every_100ms_second(void)
{
    IfxPort_togglePin(AURIX_LED2.port, AURIX_LED2.pinIndex);
    TegraEthComm_routine();
    OTA_TegraEthComm_routine();
}
static void every_1s_second(void)
{
    cansend_conti_do();
    if(delay_count < MESSAGE_DELAY_TIME_S)
    {
        delay_count++;
    }
    else if(delay_count == MESSAGE_DELAY_TIME_S)
    {
        report_error_log();
        delay_count = 0xff;
    }
    if(TRUE == check_thermal_sec_aurix_scenario()){
        Second_Aurix_thermal_test();
    }
}

void RoutineV3NP_init(routine_v3np_select_t select)
{
    routine_func_set_t func_set;
    if(select==ROUTINE_V3NP_SELECT_AURIX_SECOND){
        func_set.always = always_second;
        func_set.every_1ms = every_1ms_second;
        func_set.every_10ms = every_10ms_second;
        func_set.every_100ms = every_100ms_second;
        func_set.every_1s = every_1s_second;
    }else{
        func_set.always = always_main;
        func_set.every_1ms = every_1ms_main;
        func_set.every_10ms = every_10ms_main;
        func_set.every_100ms = every_100ms_main;
        func_set.every_1s = every_1s_main;
    }
    Routine_bind(func_set);
}

