/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#include "error_report.h"
#include "internal_fw_debug.h"
#if ISR_PROVIDER_ASC_SHELL == 0
// include shell if assign to CPU0
#include "AsclinShellInterface.h"
#endif

#define ERROR_ALWAYS_PRINTF(x)  DEBUG_PRINTF_ALWAYS(x)

uint32 error = 0;

#define MAX_ERROR_NUM  5

static const char * string_buffer[MAX_ERROR_NUM]=
{
	"INFO ROM content is invalid! run command \"loadboardconfig\" for detail",
	"NXP SJA1105 device id is not expected",
	"Fail to Init SJA1105",
	"MARVELL 88E6321 device id is not expected",
	"MAC address reading from I2C ROM are all 0, please check I2C ROM flashing",
};

static boolean is_error_flag_set(uint32 error_flag);

void set_error_flag(uint32 error_flag)
{
	error |= (0x1 << error_flag);
}

void report_error_log(void)
{
	uint8 i;
	if(error == 0)
		return;
	for(i=0; i<MAX_ERROR_NUM; i++)
	{
		if(is_error_flag_set(i))
		{
			ERROR_ALWAYS_PRINTF(("\r\nError: %s!!\r\n", string_buffer[i]));
		}
	}
	error = 0;
}

static boolean is_error_flag_set(uint32 error_flag)
{
	if((error & (0x1 << error_flag)) != 0)
		return TRUE;
	else
		return FALSE;
}
