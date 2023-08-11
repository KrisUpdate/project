/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "Configuration.h"
#include "AppAscUartCmds.h"
#include "AppAscUart.h"
#include "gpioexp.h"
#include "board2board.h"

extern boolean is_b2b_test_status;
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/
boolean UartCmd_Init(pchar args, void *data, IfxStdIf_DPipe *io)
{
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
	{
		IfxStdIf_DPipe_print(io, "Syntax     : uartinit"ENDL);
		IfxStdIf_DPipe_print(io, "           > init the uart with Tegra"ENDL);
	}
	else
	{
		//UartApp_Init();
		is_b2b_test_status = TRUE;
	}
	return TRUE;
}

boolean UartCmd_Send(pchar args, void *data, IfxStdIf_DPipe *io)
{
	uint32 send_data = 0;
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
	{
		IfxStdIf_DPipe_print(io, "Syntax     : uartsend [x]"ENDL);
		IfxStdIf_DPipe_print(io, "           > send data x to uart, if x is not defined, send 0x55"ENDL);
	}
	else
	{
		if (Ifx_Shell_parseUInt32(&args, &send_data, FALSE) == FALSE)
		{
			send_data  = 0x55;
		}
		else
		{
			send_data = (uint8)send_data;
		}
		UartApp_BlockSend(send_data);
	}

	return TRUE;
}

boolean Test_Send(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 send_data = 0;
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : test [x]"ENDL);
        IfxStdIf_DPipe_print(io, "           > just for test"ENDL);
    }
    else
    {
        if (Ifx_Shell_matchToken(&args, "off") != FALSE)
        {
            IfxStdIf_DPipe_print(io, "test off "ENDL);
            B2B_send_uart_cmd(B2B_POLLING_CMD_BMC_PWROFF);
        }else if (Ifx_Shell_matchToken(&args, "on") != FALSE)
        {
            IfxStdIf_DPipe_print(io, "test on "ENDL);
            B2B_send_uart_cmd(B2B_POLLING_CMD_BMC_PWRON);
        }
        else if (Ifx_Shell_parseUInt32(&args, &send_data, FALSE) == TRUE)
        {
            IfxStdIf_DPipe_print(io, "test rev %d",send_data,ENDL);
            SET_GPIOEXP_OUTPUT_LOW(P3570_B2B_TX_PWR_EN_EXP);
            waitTime(send_data*TimeConst[TIMER_INDEX_1S]);
            
            SET_GPIOEXP_OUTPUT_HIGH(P3570_B2B_TX_PWR_EN_EXP);
            waitTime(send_data*TimeConst[TIMER_INDEX_1S]);
            
            SET_GPIOEXP_OUTPUT_LOW(P3570_B2B_TX_PWR_EN_EXP);
            IfxStdIf_DPipe_print(io, "test cmd run.."ENDL);
        }
    }
    return TRUE;
}

