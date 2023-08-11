/**
 * \file OSTasks.c
 * \brief OS task definitions.
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
#if FEATURE_AVB
#include "ptp_glue.h"
#endif
#include "IfxStm.h"
#include "IfxStm_reg.h"
#include "TriboardLed.h"
#include "os.h"
#include "Ifx_Lwip.h"   /* LWIP */
#include "Bsp.h"
#include "internal_fw_debug.h"
#include "Routine.h"
#include "board.h"
#include "Mcmcan.h"
#include "Configuration.h"
#include "ErayNormalDemo.h"
#include "cangateway.h"

#define OS_TICK_IN_SECONDS (0.0001)
extern boolean boot_msg_flag;
#define PRINTF_BOOT(x)       ({if(boot_msg_flag){DEBUG_PRINTF_ALWAYS(x);}})
extern boolean is_b2b_test_status;

#if (defined(__OO_ECC1__) || defined(__OO_ECC2__))
#define Ifx_OSTask_ClearEvent(x) ClearEvent(x)
#define Ifx_OSTask_WaitEvent(x) WaitEvent(x)
#define Ifx_OSTask_SetEvent(x,y) SetEvent(x,y)
#endif
#if (defined(__OO_BCC1__) || defined(__OO_BCC2__))
#define Ifx_OSTask_ClearEvent(x)
#define Ifx_OSTask_WaitEvent(x)
#define Ifx_OSTask_SetEvent(x,y)
#endif

#if FEATURE_PTP
extern boolean PTP_enable;
#endif

//extern eth_addr_t ethAddr;
extern Ifx_Lwip g_Lwip;
//extern boolean can_1722_flag;
uint32 bmsr_old;
boolean phyconn = FALSE;

extern volatile boolean eventFlag_1ms_power_printing;

extern volatile boolean eventFlag_1ms_stm_0;
extern volatile boolean eventFlag_10ms_stm_0;
extern volatile boolean eventFlag_100ms_stm_0;
extern volatile boolean eventFlag_1s_stm_0;
extern uint8 canMode;
extern Eray_run_mode g_Eray_run_mode;
extern boolean is_ota_start;
void Ifx_OSTask_ApplicationInit(void)
{
    StartOS(TRICORE_CPU);
}


/*Define an ISR category 2 to trigger OS ticks.
 * Category 1 ISR is not allowed here because call of
 * OS service "CounterTick" is restricted*/
IFX_INTERRUPT_CATEGORY2(Ifx_OSTask_OSTickIsr,0,10)
{
    uint32 stmTicks;
    stmTicks= (uint32)(OS_TICK_IN_SECONDS * IfxStm_getFrequency (&MODULE_STM0));
    IfxStm_updateCompare (&MODULE_STM0, IfxStm_Comparator_0, IfxStm_getCompare (&MODULE_STM0, IfxStm_Comparator_0) + stmTicks);
    __enable ();
    CounterTick(IFX_OSTASK_COUNTER);

//    TriboardLed_serviceDimmer(); //TODO : Application call
}

/*Define an ISR category 2 to trigger events.
 * Category 2 ISR needed here because of the call restriction
 * with SetEvent API*/
IFX_INTERRUPT_CATEGORY2(Ifx_OSTask_BlinkLedIsr,0,11)
{
    uint32 stmTicks;
    stmTicks= (uint32)(TRIBOARD_LED_INTERVAL_SECONDS * IfxStm_getFrequency (&MODULE_STM0));
    IfxStm_updateCompare (&MODULE_STM0, IfxStm_Comparator_1, IfxStm_getCompare (&MODULE_STM0, IfxStm_Comparator_1) + stmTicks);
    __enable ();

    Ifx_OSTask_SetEvent (IFX_OSTASK_EVENT1, 1);
}

/*Initialize theSTM 0 and set the comparator 0 to trigger OS counter*/
void Ifx_OSTask_initStm0Ticks (void)
{
    IfxStm_CompareConfig stmCompareConfig;

    IfxStm_enableOcdsSuspend (&MODULE_STM0);

    //Call the constructor of configuration
    IfxStm_initCompareConfig (&stmCompareConfig);
    //Modify only the number of ticks and enable the trigger output
    stmCompareConfig.ticks = 100;   /*Interrupt after 100 ticks from now */
    stmCompareConfig.triggerPriority = 10;
    stmCompareConfig.typeOfService= IfxSrc_Tos_cpu0;

    //Now Compare functionality is initialized
    IfxStm_initCompare (&MODULE_STM0, &stmCompareConfig);
}

/*Initialize theSTM 0 and set the comparator 1 to trigger an interrupt
 * which will in turn trigger event 1*/
void Ifx_OSTask_initBlinkyLedFunction (void)
{
    IfxStm_CompareConfig stmCompareConfig;

    //Call the constructor of configuration
    IfxStm_initCompareConfig (&stmCompareConfig);
    //Select comparator 1
    stmCompareConfig.comparator=IfxStm_Comparator_1;
    stmCompareConfig.comparatorInterrupt=IfxStm_ComparatorInterrupt_ir1;
    //Modify only the number of ticks and enable the trigger output
    stmCompareConfig.ticks = 100;   /*Interrupt after 100 ticks from now */
    stmCompareConfig.triggerPriority = 11;
    stmCompareConfig.typeOfService= IfxSrc_Tos_cpu0;

    //Now Compare functionality is initialized
    IfxStm_initCompare (&MODULE_STM0, &stmCompareConfig);
}

uint32 Ifx_OSTask_Event1_Count;
TASK(IFX_OSTASK_EVENT1)
{
    while (1)
    {
        Ifx_OSTask_ClearEvent ( 1 );
        Ifx_OSTask_WaitEvent ( 1 );
        {
            /*Call your event 1 related functions here*/
            //TriboardLed_sweepGlow(); //TODO : Application call
        }
    }
    TerminateTask();
}

uint32 Ifx_OSTask_Event2_Count;
TASK(IFX_OSTASK_EVENT2)
{
    Ifx_OSTask_Event2_Count++;
    {
        /*Call your event 2 related functions here*/
    }
    TerminateTask();
}

uint32 Ifx_OSTask_Event3_Count;
TASK(IFX_OSTASK_EVENT3)
{
    Ifx_OSTask_Event3_Count++;
    {
        /*Call your event 3 related functions here*/
    }
    TerminateTask();
}

uint32 Ifx_OSTask_1ms_Count;
static volatile uint8 lock_udp_polling;
TASK(IFX_OSTASK_1MS)
{
    Ifx_OSTask_1ms_Count++;

    eventFlag_1ms_power_printing = TRUE;

    if( (Ifx_OSTask_1ms_Count % 5000) == 0){
        //printf("1ms time test\r\n");
    }
    eventFlag_1ms_stm_0 = TRUE;

    /* action to do each 10ms */
    if ((Ifx_OSTask_1ms_Count % 10) == 0)
    {
        eventFlag_10ms_stm_0 = TRUE;
    }
    /* action to do each 100ms */
    if ((Ifx_OSTask_1ms_Count % 100) == 0)
    {
        eventFlag_100ms_stm_0 = TRUE;
    }

    if ((Ifx_OSTask_1ms_Count % 1000) == 0)
    {
        eventFlag_1s_stm_0 = TRUE;
    }

    {
#if FEATURE_PTP
      if (PTP_enable) {
        /*Call your 1ms functions here*/
#if MAIN_AURIX_PTP_ENABLE && !SECOND_AURIX_PTP_ENABLE
        if(board.aurix_index == MAIN_AURIX)
        	ptpGluePeriodic();
#elif !MAIN_AURIX_PTP_ENABLE && SECOND_AURIX_PTP_ENABLE
        if(board.aurix_index == SECOND_AURIX)
        	ptpGluePeriodic();
#elif MAIN_AURIX_PTP_ENABLE && SECOND_AURIX_PTP_ENABLE
    		ptpGluePeriodic();
#endif
      }
#endif
    }

    {
        if ((Ifx_OSTask_1ms_Count % 4) == 0){
            if (canMode == CAN_NORMAL_MODE){
                queue_dequeue_all(g_can_queue);
            }
            else
                sendCanUdp();
        }
    }
    Multican_CmdTest();

    {
        if(lock_udp_polling == 0)
        {
            lock_udp_polling = 1;
            if(g_Lwip.initialized == TRUE)
            //&&!is_ota_start)
            {
                Ifx_Lwip_pollTimerFlags();
                Ifx_Lwip_pollReceiveFlags();
                sys_check_timeouts();
            }
            lock_udp_polling = 0;
        }
    }

    TerminateTask();
}

IFX_EXTERN App_AsclinShellInterface g_AsclinShellInterface;
uint32 Ifx_OSTask_5ms_Count;
TASK(IFX_OSTASK_5MS)
{
    //printf("5ms time test\r\n");
    Ifx_OSTask_5ms_Count++;
    TerminateTask();
}

uint32 Ifx_OSTask_10ms_Count;
TASK(IFX_OSTASK_10MS)
{
    Ifx_OSTask_10ms_Count++;
    {
        /*Call your 10ms functions here*/
    }
    TerminateTask();
}

uint32 Ifx_OSTask_20ms_Count;
TASK(IFX_OSTASK_20MS)
{
    Ifx_OSTask_20ms_Count++;
    {
        /*Call your 20ms functions here*/
    }
    TerminateTask();
}

uint32 Ifx_OSTask_50ms_Count;
TASK(IFX_OSTASK_50MS)
{
    //DEBUG_PRINTF_ALWAYS(("test \r\n"));
    Ifx_OSTask_50ms_Count++;
    {
        /*Call your 100ms functions here*/
        //Ifx_OSTask_SetEvent (IFX_OSTASK_EVENT1, 1);
    }
    TerminateTask();
}

IFX_EXTERN Ifx_TickTime TimeConst[TIMER_COUNT];
uint32 Ifx_OSTask_100ms_Count;
TASK(IFX_OSTASK_100MS)
{
    Ifx_OSTask_100ms_Count++;
    

    if((Ifx_OSTask_100ms_Count % 10) == 0){
        B2BUartService();
    }
    {
        /*Call your 100ms functions here*/

        //TriboardLed_toggleDimSwitch(); //TODO : Application call
#if 0
        if(Ifx_OSTask_100ms_Count % 10 == 0)
        {
            /* Detect Ethernet link status */
            bmsr = IFXETH_EPHY_RTL8211FG_mdio_read_reg(1, RTL8211FD_BMSR, 0) ;

	     if(bmsr_old != bmsr){
		 Ifx_Console_print("bmsr %x ,LWIP init:%x\r\n",bmsr,g_Lwip.initialized);
		 bmsr_old = bmsr;
             }

            if((bmsr& 0x04) &&(bmsr&0x08)&&(bmsr&0x20))
            {
                phyconn = TRUE;

                if(g_Lwip.initialized == FALSE)
                {
                    /*Call your event 2 related functions here*/
                    Ifx_Console_print("Do Geth demo init"ENDL);
                    /* Geth init */
                    GethBasicDemo_init();
                    Ifx_Console_print("Do Geth demo init done"ENDL);

                    /* LWIP init*/
                    Ifx_Lwip_init(ethAddr);
                    #if AVB_NUM_SOURCES != 0  //Enable Talker task
                        ActivateTask(IFX_OSTASK_AVB_1722_TALKER);
                    #endif
                    #if AVB_NUM_SINKS != 0 //Enable Listener task
                        ActivateTask(IFX_OSTASK_AVB_1722_LISTENER);
                    #endif
						/* SRPDemo init*/
						SRPDemo_init();
						/* SRP init*/
						{
							mrp_init(ethAddr.addr);
							srp_domain_init();
							avb_mvrp_init();
							avb_mrp_init = 1;
						}
			/* Message queue init*/
			StartCOM(EE_COM_MODE_A);
			InitMessage(receiverTalkerMsg, &talker_msg);
			InitMessage(receiverListenerMsg, &listener_msg);
                    /* MAAP TEST*/
                    avb_1722_maap_init();
                    unsigned int serial_num = 0; //TODO: Alvin will fix it
                    avb_1722_1_aecp_aem_init(serial_num);
                    // 1722.1
                    avb_1722_1_task();
                    Ifx_Console_print("avb 1722_1 done"ENDL);
                    // Talker/Listener reg.
                    avb_interface_initialise();
                    Ifx_Console_print("[dummy]talker/listener reg done"ENDL);
                }
            }
            else
            {
                phyconn = FALSE;
            }

            //if(disconn then conn)
            if(prv_phyconn == FALSE && phyconn == TRUE)
            {
                //Do MAAP
                if (AVB_1722_1_TALKER_ENABLED == TRUE) // only talker
                {
                    Ifx_Console_print("[******] Do MAAP %d->%d \n", prv_phyconn, phyconn);
                    if(avb_1722_maap_get_base_address(addr_base)!=-1)
                    {
                        avb_1722_maap_request_addresses(1,addr_base);
                    }else{
                        avb_1722_maap_request_addresses(1,NULL);
                    }
                }
                srp_domain_join();
            }
            //store
            prv_phyconn = phyconn;
        }
#endif
    }
    TerminateTask();
}

uint8 canDataX[64];
uint32 Ifx_OSTask_Background_Count;
TASK(IFX_OSTASK_BACKGROUND)
{
    Ifx_OSTask_Background_Count++;
    /*Add your code here*/

    while(1)
    {
        if(lock_udp_polling == 0) 
        {
            lock_udp_polling = 1;
            if(g_Lwip.initialized == TRUE)
            //&&is_ota_start == TRUE)
            {
                Ifx_Lwip_pollTimerFlags();
                Ifx_Lwip_pollReceiveFlags();
                sys_check_timeouts();
            }
            lock_udp_polling = 0;
        }

        Routine_do();
        AsclinShellInterface_run();
    }
    TerminateTask();
}

TASK(IFX_OSTASK_INIT)
{
    uint32 bmsr;
    __enable();

    /*Add your initialization code here*/
    ActivateTask(IFX_OSTASK_BACKGROUND);

    /*  Alarms are Auto started with the same parameters as below. Code below is not necessary.
     * You could also implement this if you want to change the parameters of alarms
     *//*
        SetRelAlarm(IFX_OSTASK_ALARM_1MS,5,10);
        SetRelAlarm(IFX_OSTASK_ALARM_5MS,25,50);
        SetRelAlarm(IFX_OSTASK_ALARM_10MS,50,100);
        SetRelAlarm(IFX_OSTASK_ALARM_20MS,100,200);
        SetRelAlarm(IFX_OSTASK_ALARM_50MS,250,500);
        SetRelAlarm(IFX_OSTASK_ALARM_100MS,500,1000);
    */
    queue_init(g_can_queue);
    //g_can_queue = queue_new(20);
    Ifx_OSTask_initStm0Ticks ();
    //Ifx_OSTask_initBlinkyLedFunction();
    //TriboardLed_init(); //TODO : Application call
    ActivateTask(IFX_OSTASK_100MS);
    ActivateTask(IFX_OSTASK_EVENT1);
    ActivateTask(IFX_OSTASK_1MS);
    PRINTF_BOOT(("Do Geth demo init"ENDL));
    TerminateTask();
}

TASK(IFX_OSTASK_AVB_1722_TALKER)
{
#if FEATURE_AVB
	Task_AVB_1722_talker();
#endif
};
TASK(IFX_OSTASK_AVB_1722_LISTENER) {
#if FEATURE_AVB
	Task_AVB_1722_listener();
#endif

};

TASK(IFX_OSTASK_SPIROM){}
