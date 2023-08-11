/**
 * \file TLF35584Cmds.c
 * \brief TLF35584 commands for AscLin Shell
 *
 * \version iLLD_Demos_1_0_0_5_0
 * \copyright Copyright (c) 2015 Infineon Technologies AG. All rights reserved.
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

#include "SysSe/Comm/Ifx_Shell.h"
#include "TLF35584Cmds.h"
#if MUL_CAN_TC29X_ENABLE == 1
#include "../Multican/Multican.h"
#endif
#if MCM_CAN_TC39X_ENABLE == 1
#include "Mcmcan.h"
#endif
#include <gpioexp.h>
#include "AsclinShellInterface.h"
#include "resource.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define STANDBY_MODE_MAX_COUNTING 300

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/
IFX_EXTERN App_AsclinShellInterface g_AsclinShellInterface;

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
volatile boolean eventFlag_100ms_tlf_wd_printing = FALSE;
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/
uint32 standby_defer_in_sec = 0;
boolean tlf_cnting_semaphore = 0;
/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
/** \brief Handle the 'showtlf' command
 *
 * \par Syntax
 *  - showtlf x: show the value of TLF register x
 */
boolean Show_TLF_Register(pchar args, void *data, IfxStdIf_DPipe *io)
{
	uint32 regNr;
	uint8 uiValue;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : showtlf x"ENDL);
        IfxStdIf_DPipe_print(io, "           > Show the value of TLF register x"ENDL);
        return TRUE;
    }
	// first we get the first argument
    regNr = 0;
	if (Ifx_Shell_parseUInt32(&args, &regNr, FALSE) == FALSE)
   	{
   		IfxStdIf_DPipe_print(io, "Syntaxerror : invalid register number %s"ENDL, args);
   		return TRUE;
   	}
    if (regNr > 0x3A)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid register number %d"ENDL, regNr);
        return TRUE;
    }
    sem_wait_until_get(SEM_SPI_2, __LINE__, __func__);
    uiValue = IfxTLF35584_simplified_read_write(FALSE, (uint8)regNr, 0x00);
    sem_release(SEM_SPI_2);
    IfxStdIf_DPipe_print(io, "Reg 0x%02X:  0x%02X"ENDL, regNr, uiValue);

    return TRUE;
}

/*  \brief Handle the 'writetlf' command
 *
 *  \par Syntax
 *  - tlfwrite x y: write value y to the register x
 */
boolean Write_TLF_Register(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 regNr, value;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : tlfwrite x y"ENDL);
        IfxStdIf_DPipe_print(io, "           > Write the value y to TLF register x"ENDL);
        return TRUE;
    }

    // first argument
    regNr = 0;
    if (Ifx_Shell_parseUInt32(&args, &regNr, FALSE) == FALSE)
    {
	IfxStdIf_DPipe_print(io, "Syntaxerror : invalid register number %s"ENDL, args);
	return TRUE;
    }
    if (regNr > 0x3A)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid register number %d"ENDL, regNr);
        return TRUE;
    }

    // second argument
    if (Ifx_Shell_parseUInt32(&args, &value, FALSE) == FALSE)
    {
	IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value %s"ENDL, args);
	return TRUE;
    }

    // write value
    sem_wait_until_get(SEM_SPI_2, __LINE__, __func__);
    IfxTLF35584_simplified_read_write(TRUE, (uint8)regNr, (uint8)value);
    sem_release(SEM_SPI_2);
    return TRUE;
}
boolean TLF_Wwd_Test(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 delay;
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : tlfwwdtest delay"ENDL);
        IfxStdIf_DPipe_print(io, "           > enable TLF Window Watchdog monitoring"ENDL);
        IfxStdIf_DPipe_print(io, "           > and service Window Watchdog after specified delay in ms"ENDL);
        return TRUE;
    }
	else
	{
		if (Ifx_Shell_parseUInt32(&args, &delay, FALSE) == FALSE)
	   	{
			delay = 3000;
	   	}
		IfxStdIf_DPipe_print(io, "Service TLF Window Watchdog after %d ms delay "ENDL, delay);
		IfxStdIf_DPipe_print(io, "Aurix will reset if WWD Error Counter reaches 9 "ENDL);
        FUNCS_TAKE_SEM(IfxTLF35584_unprotect_register, IfxTLF35584_enable_window_watchdog, IfxTLF35584_protect_register, SEM_SPI_2);
		waitTime(delay*TimeConst[TIMER_INDEX_1MS]); // this is just a placeholder value for delay
		wwd_enable = TRUE;
	}
    return TRUE;
}

/** \brief Handle the 'standby' command
 *
 * \par Syntax
 *  - standby : switch the TLF35584 to standby state if possible (wake not activ)
 */

void TLF_set_wakeup_peripherals(void)
{
    	//first make sure Inhibit from CAN phy and Flexray phy are OFF before going into standby
    	//we put them into sleep mode so that the Inhibit pin will be float (low by pull-down)
#if WAKE_UP_SOURCE_FR_A_ENABLE == 1
     	IfxPort_setPinHigh(FR_A_EN.port, FR_A_EN.pinIndex);
    	IfxPort_setPinModeOutput(FR_A_EN.port, FR_A_EN.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
#ifdef FR_A_STB
    	IfxPort_setPinLow(FR_A_STB.port, FR_A_STB.pinIndex);
    	IfxPort_setPinModeOutput(FR_A_STB.port, FR_A_STB.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
#endif
#ifdef FR_A_STB_N_EXP
    	SET_GPIOEXP_OUTPUT_LOW(FR_A_STB_N_EXP);
    	SET_GPIOEXP_CONFIG_OUTPUT(FR_A_STB_N_EXP);
#endif
#endif
#if WAKE_UP_SOURCE_FR_B_ENABLE == 1
     	IfxPort_setPinHigh(FR_B_EN.port, FR_B_EN.pinIndex);
    	IfxPort_setPinModeOutput(FR_B_EN.port, FR_B_EN.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    	IfxPort_setPinLow(FR_B_STB.port, FR_B_STB.pinIndex);
    	IfxPort_setPinModeOutput(FR_B_STB.port, FR_B_STB.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
#endif
#if WAKE_UP_SOURCE_CAN_PHY1_ENABLE == 1
    	can_enable_phy(PHY1);
    	waitTime(2*TimeConst[TIMER_INDEX_1MS]); // this is just a placeholder value for delay
#ifdef CAN_PHY1_STB
    	IfxPort_setPinLow(CAN_PHY1_STB.port, CAN_PHY1_STB.pinIndex);
    	IfxPort_setPinModeOutput(CAN_PHY1_STB.port, CAN_PHY1_STB.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
#endif
#ifdef CAN_PHY1_STB_N_EXP
       	SET_GPIOEXP_OUTPUT_LOW(CAN_PHY1_STB_N_EXP);
        SET_GPIOEXP_CONFIG_OUTPUT(CAN_PHY1_STB_N_EXP);
#endif

#endif
#if WAKE_UP_SOURCE_CAN_PHY3_ENABLE == 1
    	can_enable_phy(PHY3);
    	waitTime(2*TimeConst[TIMER_INDEX_1MS]); // this is just a placeholder value for delay
#ifdef CAN_PHY3_STB
    	IfxPort_setPinLow(CAN_PHY3_STB.port, CAN_PHY3_STB.pinIndex);
    	IfxPort_setPinModeOutput(CAN_PHY3_STB.port, CAN_PHY3_STB.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
#endif
#ifdef CAN_PHY3_STB_N_EXP
    	SET_GPIOEXP_OUTPUT_LOW(CAN_PHY3_STB_N_EXP);
    	SET_GPIOEXP_CONFIG_OUTPUT(CAN_PHY3_STB_N_EXP);
#endif
#endif
    	/* in ACR, we need to wait longer time for KL15 voltage to drop */
    	waitTime(STANDBY_DELAY_MS * TimeConst[TIMER_INDEX_1MS]);
    	
}

boolean TLF_get_standby_counting(void)
{
  return tlf_cnting_semaphore;
}

void TLF_set_standby_counting(void)
{
  tlf_cnting_semaphore = TRUE;
}

void TLF_regular_check(void)
{
    static unsigned char counting_sec = 0;
    //do standby mode check
    if(TLF_get_standby_counting() != FALSE)
    {
        counting_sec++;
        //calibration
        IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "counting [%d]"ENDL,counting_sec);
        if(counting_sec >= standby_defer_in_sec){
            IfxStdIf_DPipe_print(&g_AsclinShellInterface.stdIf.asc, "%ds is up, do standby"ENDL, standby_defer_in_sec);
            TLF_set_wakeup_peripherals();
            FUNC_TAKE_SEM(IfxTLF35584_goto_standby_state, SEM_SPI_2, "IfxTLF35584_goto_standby_state");
        }
    }
}

extern volatile boolean system_ready;
boolean TLF_Standby(pchar args, void *data, IfxStdIf_DPipe *io)
{
    unsigned int seconds = 0;
    Ifx_DateTime rt_standby;
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : standby"ENDL);
        IfxStdIf_DPipe_print(io, "           > Switch TLF35584 to standby state"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax     : standby s 30"ENDL);
        IfxStdIf_DPipe_print(io, "           > Switch TLF35584 to standby state after 30s secconds"ENDL);
    }
    else
    {
        if (Ifx_Shell_matchToken(&args, "s") != FALSE)
        {
            if (Ifx_Shell_parseUInt32(&args, &standby_defer_in_sec, FALSE) == FALSE){
                standby_defer_in_sec = STANDBY_MODE_MAX_COUNTING;
            }
            if(standby_defer_in_sec > STANDBY_MODE_MAX_COUNTING) standby_defer_in_sec = STANDBY_MODE_MAX_COUNTING;
            IfxStdIf_DPipe_print(io, "Counting now"ENDL);
            IfxStdIf_DPipe_print(io, "Standby mode defer second is %d"ENDL, standby_defer_in_sec);
            TLF_set_standby_counting();
        }else if(Ifx_Shell_matchToken(&args, "pure") != FALSE){
            IfxStdIf_DPipe_print(io, "right now(no power off)"ENDL);
            TLF_set_wakeup_peripherals();
            FUNC_TAKE_SEM(IfxTLF35584_goto_standby_state, SEM_SPI_2, "IfxTLF35584_goto_standby_state");
        }else{
            IfxStdIf_DPipe_print(io, "right now"ENDL);
            system_ready = 0;
            waitTime(500*TimeConst[TIMER_INDEX_1MS]); // this is just a placeholder value for delay
            TLF_set_wakeup_peripherals();
			System_PowerOff();
			waitTime(100*TimeConst[TIMER_INDEX_1MS]);
            FUNC_TAKE_SEM(IfxTLF35584_goto_standby_state, SEM_SPI_2, "IfxTLF35584_goto_standby_state");
        }
    }
    return TRUE;
}

void Print_TLF_WD_Status(IfxStdIf_DPipe *io)
{
	static uint8 wwd_err_count_prev=0;
	uint8 wwd_err_count;
	static uint32 elapse_100msec=0;
	static uint32 elapse_min=0;
	if(wwd_enable)
	{
		if (eventFlag_100ms_tlf_wd_printing)
		{
	    	wwd_err_count = IfxTLF35584_simplified_read_write(FALSE, 0x29, 0x00);

	    	if (wwd_err_count != wwd_err_count_prev)
	    	{
	    		IfxStdIf_DPipe_print(io, ENDL"WWD Error Counter:%d\r\n", wwd_err_count);
	    		wwd_err_count_prev = wwd_err_count;
	    	}
			elapse_100msec++;
			if ((elapse_100msec % 600)  == 0)
			{	//print every minutes
				elapse_min = elapse_100msec / 600;
				IfxStdIf_DPipe_print(io, ENDL"WWD Test Elapse Time:  %d min\r\n", elapse_min);

			}
			eventFlag_100ms_tlf_wd_printing = FALSE;
		}

	}
}

