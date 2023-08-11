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

#include "SysSe/Comm/Ifx_Shell.h"
#include "HsslCmds.h"
#include "hssl_test.h"
#include "SysSe/Bsp/Bsp.h"
#include "Configuration.h"
#include "board.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

#define PRINT_SUCCESS_LOG		IfxStdIf_DPipe_print(io, "Command Executed"ENDL);
#define PRINT_FAIL_LOG			IfxStdIf_DPipe_print(io, "Command Fail "ENDL);

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

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

boolean hssltest(pchar args, void *data, IfxStdIf_DPipe * io)
{
	uint32 number;
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
	{
		 IfxStdIf_DPipe_print(io, "Syntax     : hssltest [x]" ENDL);
		 IfxStdIf_DPipe_print(io, "           > run the hssl loop back test between slave and master" ENDL);
		 IfxStdIf_DPipe_print(io, "           > x is the number of uint32 to send, starting from 0 to x" ENDL);
		 IfxStdIf_DPipe_print(io, "           > if x is not defined, default is 0xFF" ENDL);
	}
	else
	{
		if (Ifx_Shell_parseUInt32(&args, &number, FALSE) == FALSE)
		{
			number = 0xFF;
		}
		if (Ifx_Shell_matchToken(&args, "m") != FALSE)
		{
			hssl_demo_main_master(number);
		}
		else if(Ifx_Shell_matchToken(&args, "s") != FALSE)
		{
			hssl_demo_main_slave(number);
		}
		else
		{
			if(board.aurix_index == MAIN_AURIX)
				hssl_demo_main_master(number);
			else
				hssl_demo_main_slave(number);
		}
		PRINT_SUCCESS_LOG;
	}
	return TRUE;
}
