/**
 * \file Cpu0_Main.c
 * \brief System initialisation and main program implementation.
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

//#include "Cpu_Main.h"   /* FIXME: Cpu_main.h */

#include "GethBasicDemo.h"
#include "Ifx_Types.h"
#include "SysSe/Bsp/Bsp.h"
#include <Stm/Std/IfxStm.h>
#include "IfxCpu.h"
#include "Cpu0_Main.h"
#if FEATURE_AVB
#include "maap.h"
#endif
#include "gpio.h"
#include "Tsensor.h"
#include "TsensorCmds.h"
#include "PSensorCmds.h"
#include "I2c.h"
#include "I2cCmds.h"

#include "IfxScuWdt.h"
#include "SysSe/Time/Ifx_DateTime.h"
#include "Configuration.h"
#include "TLF35584Cmds.h"
#include "board.h"
#include "IfxPms_reg.h"
#include "IfxScu_reg.h"
#include "resource_state_manage.h"
//add
#if ISR_PROVIDER_ASC_SHELL == 0
// include shell if assign to CPU0
#include "AsclinShellInterface.h"
#endif


#include <internal_fw_debug.h>
#include "fan.h"
#include "AsclinShellInterface.h"
#include "RoutineV3NP.h"
#include "config_pg2_feasures.h"
#include "EvadcAutoScan.h"
#include "AppAscUart.h"
#include "Tsensor.h"
#include "resource.h"
#include "PSensor.h"
#include "system_monitor.h"

/******************************************************************************/
/*------------------------Inline Function Prototypes--------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define MAIN_DEBUG_ENABLE 1

#if MAIN_DEBUG_ENABLE == 1
#define MAIN_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define MAIN_DEBUG_PRINTF(x)
#endif
boolean boot_msg_flag=FALSE;
#define MAIN_ALWAYS_PRINTF(x)  DEBUG_PRINTF_ALWAYS(x)
#define PRINTF_BOOT(x)  ({if(boot_msg_flag){DEBUG_PRINTF_ALWAYS(x);}})

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/
IfxCpu_syncEvent cpuSyncEvent=0;
/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
App_Cpu0 g_AppCpu0; /**< \brief CPU 0 global data */
volatile uint32 cpu0_idle_counter=0;
volatile boolean eventFlag_1ms_stm_0 = FALSE;
volatile boolean eventFlag_10ms_stm_0 = FALSE;
volatile boolean eventFlag_100ms_stm_0 = FALSE;
volatile boolean eventFlag_1s_stm_0 = FALSE;
//LWIP
#include "Ifx_Lwip.h"
extern eth_addr_t ethAddr;
extern Ifx_Lwip g_Lwip;

volatile boolean system_ready = FALSE;
extern boolean is_b2b_x86_v3nc;
/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
static void Main_Aurix_loop(void);
static void Second_Aurix_loop(void);
/** \brief Main entry point after CPU boot-up.
 *
 *  It initialise the system and enter the endless loop that handles the demo
 */
static void STM_Initial(void);
static void Residual_Voltage_check(void);
extern e_AurixIndex detect_Aurix_index(void);

int core0_main(void)
{
    system_ready = FALSE;
    /* ensure that XTAL1 configured for X MHz */    
    uint32 fosc = IFX_CFG_SCU_XTAL_FREQUENCY;    
    uint32 fpll = IFX_CFG_SCU_PLL_FREQUENCY;
    /*
     * !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdog in the demo if it is required and also service the watchdog periodically
     * */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

    /* Initialise the application state */
    g_AppCpu0.info.pllFreq = IfxScuCcu_getPllFrequency();
    g_AppCpu0.info.cpuFreq = IfxScuCcu_getCpuFrequency(IfxCpu_getCoreIndex());
    g_AppCpu0.info.sysFreq = IfxScuCcu_getSpbFrequency();
    g_AppCpu0.info.stmFreq = IfxStm_getFrequency(&MODULE_STM0);
    g_AppCpu0.TickCount_1ms = 0;
    /* Enable the global interrupts of this CPU */
    IfxCpu_enableInterrupts();
    board.aurix_index = detect_Aurix_index();
#if ISR_PROVIDER_ASC_SHELL == 0
    /* Com port shell init */
    AsclinShellInterface_init();
#endif
    // ###PRINT COMMIT### // don't remove this line since release utility replaces this to print commit hash in daily build
    //Ifx_Console_print
    printf("\r\nBuilt at DATE %s, TIME %s\r\n", __DATE__, __TIME__);    
    printf("PLL initialized: Normal mode, fosc: %ld MHz & fpll: %ld MHz\r\n", fosc / 1000000, fpll / 1000000);    
    printf("PLL freq: %f MHz\r\n", g_AppCpu0.info.pllFreq / 1000000);    
    printf("CPU freq: %f MHz\r\n", g_AppCpu0.info.cpuFreq / 1000000);    
    printf("SPB freq: %f MHz\r\n",  IfxScuCcu_getSpbFrequency() / 1000000);    
    printf("SRI freq: %f MHz\r\n",  IfxScuCcu_getSriFrequency() / 1000000);    
    printf("STM freq: %f MHz\r\n", g_AppCpu0.info.stmFreq / 1000000);
    printf("SYS freq: %f MHz\r\n", g_AppCpu0.info.sysFreq / 1000000);

    // Enable I2c modules
    // TODO: use a functions to hold all the necessary MCU Peripherals initial
    i2c_init(0, 400000);
    i2c_init(1, 400000);

    IfxTLF35584_init();
    IfxTLF35584_unprotect_register();
    IfxTLF35584_disable_window_watchdog();
    IfxTLF35584_disable_err_pin_monitor();
    IfxTLF35584_protect_register();

    load_board_config();

    SetOutputs_to_System_PowerOff_State();

    if(board.aurix_index == MAIN_AURIX)
    {
        Main_Aurix_loop();
    }
    else
    {
        Second_Aurix_loop();
    }

    /* Enable Erika OS */
    system_ready = TRUE;
    void Ifx_OSTask_ApplicationInit(void);
    Ifx_OSTask_ApplicationInit();

    /* Cpu sync event wait*/
    IfxCpu_emitEvent(&cpuSyncEvent);
    IfxCpu_waitEvent(&cpuSyncEvent, 1);

    /* background endless loop */
    while (TRUE)
    {
        //AsclinShellInterface_run(); /* Move to background task */
        
        REGRESSION_RUN_STOP_PASS;
    }

    return 0;
}


static void Main_Aurix_loop(void)
{
    MAIN_ALWAYS_PRINTF(("Binary is running in Main Aurix\r\n"));
#if ISR_PROVIDER_VADC == 0
    // init vadc if assign to CPU0
    VadcAutoScan_init();
#endif
    STM_Initial();
#if ISR_PROVIDER_VADC == 0
    // start vadc autoscan if assign to CPU0
    VadcAutoScan_run();
#endif
    
    Residual_Voltage_check();

    /* Switch TLF to normal state */
    /* here we wait until or first interrupt from STM occurs */
    while (g_AppCpu0.TickCount_1ms == 0);
    /* now we had wait more than 10ms, we can switch to normal state */
    IfxTLF35584_goto_normal_state();

    booting_system();

    /*Init Temp sensor */
    initTempsensors();

    /*Init Power sensor */
    initPowersensors();

    if(is_b2b_x86_v3nc)
        UartApp_Init(UART_BAUD_RATE_19200);
    else
        UartApp_Init(UART_BAUD_RATE_115200);

    System_Monitor_KL30_Init();
    RoutineV3NP_init(ROUTINE_V3NP_SELECT_AURIX_MAIN);
}

static void Second_Aurix_loop(void)
{
    MAIN_ALWAYS_PRINTF(("Binary is running in Second Aurix\r\n"));
#if ISR_PROVIDER_VADC == 0
    // init vadc if assign to CPU0
    VadcAutoScan_init();
#endif
    STM_Initial();
#if ISR_PROVIDER_VADC == 0
    // start vadc autoscan if assign to CPU0
    VadcAutoScan_run();
#endif
    /* Switch TLF to normal state */
    /* here we wait until or first interrupt from STM occurs */
    while (g_AppCpu0.TickCount_1ms == 0);
    /* now we had wait more than 10ms, we can switch to normal state */
    IfxTLF35584_goto_normal_state();
    /*Init Temp sensor */
    if(board.pcb_version >= PG2_A02)
        initAurix2Tempsensors();

    Second_Aurix_Boot();
//    count = 0;
    RoutineV3NP_init(ROUTINE_V3NP_SELECT_AURIX_SECOND);
}

static void STM_Initial(void)
{
    /* initialize STM0 */
    IfxStm_CompareConfig stmCompareConfig;
    /* suspend by debugger enabled */
    IfxStm_enableOcdsSuspend(&MODULE_STM0);
    /* constructor of configuration */
    IfxStm_initCompareConfig(&stmCompareConfig);
    /* change the comparator configuration */
    stmCompareConfig.triggerPriority      = ISR_PRIORITY_STM0_TICK;
    stmCompareConfig.comparatorInterrupt  = IfxStm_ComparatorInterrupt_ir0; /*User must select the interrupt output */
    stmCompareConfig.ticks                = IFX_CFG_STM0_TICKS_PER_MS*10;    /* we start after 10ms */
    stmCompareConfig.typeOfService        = IfxSrc_Tos_cpu0;

    /* Now Compare functionality is initialized */
    IfxStm_initCompare(&MODULE_STM0, &stmCompareConfig);
}

static void Residual_Voltage_check(void)
{
	uint32 count, fail_count;
    uint32 millivoltages;

    /* wait for discharge */
    waitTime(100 * TimeConst[TIMER_INDEX_1MS]);

    count = 0;
    fail_count = 0;
    while(count < 3)
	{
        millivoltages = VadcAutoScan_getCalCulatedVoltage_mV("VBAT_SYS", 0);
        if(millivoltages >= 1800)
        {
            if(fail_count >= 50)
            {
                MAIN_ALWAYS_PRINTF(("%s is out of range, value detect: %d mV\r\n", "VBAT_SYS", millivoltages));
                fail_count = 0;
            }
            fail_count++;
            waitTime(100 * TimeConst[TIMER_INDEX_1MS]);
        }
        else
        {
            // MAIN_ALWAYS_PRINTF(("%s value detect: %d mV\r\n", "VBAT_SYS", millivoltages));
            waitTime(1 * TimeConst[TIMER_INDEX_1MS]);
            count++;
        }
	}
    
    count = 0;
    fail_count = 0;
    while(count < 3)
	{
        millivoltages = VadcAutoScan_getCalCulatedVoltage_mV("VBAT_TEG", 0);
        if(millivoltages >= 1800)
        {
            if(fail_count >= 50)
            {
                MAIN_ALWAYS_PRINTF(("%s is out of range, value detect: %d mV\r\n", "VBAT_TEG", millivoltages));
                fail_count = 0;
            }
            fail_count++;
            waitTime(100 * TimeConst[TIMER_INDEX_1MS]);
        }
        else
        {
            // MAIN_ALWAYS_PRINTF(("%s value detect: %d mV\r\n", "VBAT_TEG", millivoltages));
            waitTime(1 * TimeConst[TIMER_INDEX_1MS]);
            count++;
        }
	}
}


IFX_INTERRUPT (UsrIsr_Stm_0, 0, ISR_PRIORITY_STM0_TICK);

void UsrIsr_Stm_0(void)
{
    /* Set next 1ms scheduler tick alarm */
    IfxStm_increaseCompare(&MODULE_STM0, IfxStm_Comparator_0, IFX_CFG_STM0_TICKS_PER_MS);

    /* increment the ms value */
    g_AppCpu0.TickCount_1ms++;

#if FAN_TACH_TIM_MEASURE_ENABLE == 0
    time_ms_passed++;
#endif

    random_seed++;
    /* action to do each 1ms */
#if ISR_PROVIDER_ETHERNET == 0
//    Ifx_Lwip_onTimerTick();
#endif

    /* Enable the global interrupts of this CPU */
    IfxCpu_enableInterrupts();

    eventFlag_10ms_temp_printing = TRUE;
    eventFlag_1ms_power_printing = TRUE;

    eventFlag_1ms_stm_0 = TRUE;

     /* action to do each 10ms */
    if ((g_AppCpu0.TickCount_1ms % 10) == 0)
    {
        //MAIN_ALWAYS_PRINTF(("1ms\r\n"));
        eventFlag_10ms_stm_0 = TRUE;
    }
    /* action to do each 100ms */
    if ((g_AppCpu0.TickCount_1ms % 100) == 0)
    {
        //MAIN_ALWAYS_PRINTF(("0.1s\r\n"));
        eventFlag_1s_stm_0 = TRUE;
        eventFlag_100ms_stm_0 = TRUE;
        eventFlag_100ms_tlf_wd_printing = TRUE;
    }

    if ((g_AppCpu0.TickCount_1ms % 1000) == 0)
    {
        PRINTF_BOOT(("1s\r\n"));
        eventFlag_1s_stm_0 = TRUE;
    }
}

/** \} */


