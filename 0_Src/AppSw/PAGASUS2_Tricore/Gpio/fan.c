/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */


#include "../Gpio_exp/gpioexp.h"
#include "Configuration.h"
#include "SysSe/Bsp/Bsp.h"
#include <_Impl/IfxPort_cfg.h>
#include "fan.h"
#include <internal_fw_debug.h>
#include "gtm.h"
#include "string.h"
#include "board.h"

#if FAN_TACH_TIM_MEASURE_ENABLE == 1

#define FAN_DEBUG_ENABLE 1

#if FAN_DEBUG_ENABLE == 1
#define FAN_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define FAN_DEBUG_PRINTF(x)
#endif

uint32 rpm[FAN_NUMBER];

#define SELECT_PULL_FAN_A   SET_GPIOEXP_OUTPUT_LOW(PULL_FAN_TACH_SEL_AB_EXP);\
							SET_GPIOEXP_CONFIG_OUTPUT(PULL_FAN_TACH_SEL_AB_EXP);

#define SELECT_PULL_FAN_B   SET_GPIOEXP_OUTPUT_HIGH(PULL_FAN_TACH_SEL_AB_EXP);\
							SET_GPIOEXP_CONFIG_OUTPUT(PULL_FAN_TACH_SEL_AB_EXP);

#define FAN_PHASE       2


boolean Show_FanRPM(pchar args, void *data, IfxStdIf_DPipe *io)
{
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
	{
		IfxStdIf_DPipe_print(io, "Syntax     : showfan [1/2/3/4]"ENDL);
		IfxStdIf_DPipe_print(io, "           > show all the fan rpm if not parameter"ENDL);
		IfxStdIf_DPipe_print(io, "           > 1: PUSH FAN A"ENDL);
		IfxStdIf_DPipe_print(io, "           > 2: PUSH FAN B"ENDL);
		IfxStdIf_DPipe_print(io, "           > 3: PULL FAN A"ENDL);
		IfxStdIf_DPipe_print(io, "           > 4: PULL FAN B"ENDL);
		return TRUE;
	}
	else
	{
		if (Ifx_Shell_matchToken(&args, "1") != FALSE)
		{
#if FAN_A_ENABLE == 1
			IfxStdIf_DPipe_print(io, "PUSH FAN A: The RPM is %d"ENDL, rpm[0]);
#endif
		}
		else if (Ifx_Shell_matchToken(&args, "2") != FALSE)
		{
#if FAN_B_ENABLE == 1
          IfxStdIf_DPipe_print(io, "PUSH FAN B: The RPM is %d"ENDL, rpm[1]);
			if(board.project != P3479)
			{
				IfxStdIf_DPipe_print(io, "PUSH FAN B: The RPM is %d"ENDL, rpm[1]);
			}
			else
			{
				IfxStdIf_DPipe_print(io, "There's PUSH FAN FOR Xavier B in 3479"ENDL);
			}
#endif
		}
		else if (Ifx_Shell_matchToken(&args, "3") != FALSE)
		{
#if PULL_FAN_A_ENABLE == 1
	IfxStdIf_DPipe_print(io, "PULL FAN A: The RPM is %d"ENDL, rpm[2]);
#endif
		}
		else if (Ifx_Shell_matchToken(&args, "4") != FALSE)
		{
#if PULL_FAN_B_ENABLE == 1

			if(board.project != P3479)
			{
				IfxStdIf_DPipe_print(io, "PULL FAN B: The RPM is %d"ENDL, rpm[3]);
			}
			else
			{
				IfxStdIf_DPipe_print(io, "There's PULL FAN FOR Xavier B in 3479"ENDL);
			}

#endif
		}
		else
		{
#if FAN_A_ENABLE == 1
			IfxStdIf_DPipe_print(io, "PUSH FAN A: The RPM is %d"ENDL, rpm[0]);
#endif
#if FAN_B_ENABLE == 1

			if(board.project != P3479)
			{
				IfxStdIf_DPipe_print(io, "PUSH FAN B: The RPM is %d"ENDL, rpm[1]);
			}

#endif
#if PULL_FAN_A_ENABLE == 1
			IfxStdIf_DPipe_print(io, "PULL FAN A: The RPM is %d"ENDL, rpm[2]);
#endif
#if PULL_FAN_B_ENABLE == 1

			if(board.project != P3479)
			{
				IfxStdIf_DPipe_print(io, "PULL FAN B: The RPM is %d"ENDL, rpm[3]);
			}

#endif
		}
	}
	return TRUE;
}

void fan_detect_init(void)
{
	GtmTim_init();
	memset(&rpm, 0, sizeof(rpm));
}

void fan_detect(void)
{
	static boolean is_pull_fan_a = TRUE;
	uint32 freq;

	if(GtmTim_getFreq(FAN_A_Tach_Pin, &freq) == TRUE)
		rpm[0] = ((float)freq) / FAN_PHASE * 60 ;
	else
		rpm[0] = 0;

	if(GtmTim_getFreq(FAN_B_Tach_Pin, &freq)==TRUE)
		rpm[1] = ((float)freq) / FAN_PHASE * 60 ;
	else
		rpm[1] = 0;

	if(is_pull_fan_a)
	{
		if(GtmTim_getFreq(PULL_FAN_A_B_Tach_Pin, &freq) == TRUE)
			rpm[2] = ((float)freq) / FAN_PHASE * 60 ;
		else
			rpm[2] = 0;
		is_pull_fan_a = FALSE;

		SELECT_PULL_FAN_B;
	}
	else
	{
		if(GtmTim_getFreq(PULL_FAN_A_B_Tach_Pin, &freq) == TRUE)
			rpm[3] = ((float)freq) / FAN_PHASE * 60 ;
		else
			rpm[3] = 0;
		is_pull_fan_a = TRUE;

		SELECT_PULL_FAN_A;
	}
}
#endif //#if FAN_TACH_TIM_MEASURE_ENABLE == 1
