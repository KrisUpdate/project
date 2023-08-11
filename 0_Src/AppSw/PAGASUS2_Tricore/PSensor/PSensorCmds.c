/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#include "SysSe/Comm/Ifx_Shell.h"
#include <I2c.h>
#include "PSensor.h"
#include "SysSe/Bsp/Bsp.h"
#include "PSensorCmds.h"
#include "board.h"
#include "board_I2C_devices.h"
#include "Resource.h"
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
volatile boolean display_power = FALSE;
volatile boolean eventFlag_1ms_power_printing = FALSE;
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
/** \brief Handle the 'rdpower' command
 *
 * \par Syntax
 *  - rdpower [c/loop] [interval]:  read board power for [loop] times or continuously with interval in between read
 */
boolean ReadPower(pchar args, void *data, IfxStdIf_DPipe *io)
{
    display_power_loop_count = 0;
    pchar temp_args;

	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : rdpower [loop] "ENDL);
        IfxStdIf_DPipe_print(io, "           > possible format is"ENDL);
        IfxStdIf_DPipe_print(io, "           > rdpower [loop]"ENDL);
        IfxStdIf_DPipe_print(io, "           >> read board power for [loop] times, if [loop] is not given, it will read once"ENDL);
        IfxStdIf_DPipe_print(io, "           > rdpower c [x] "ENDL);
        IfxStdIf_DPipe_print(io, "           >> continuously read board power with interval x ms in between"ENDL);
        IfxStdIf_DPipe_print(io, "           >> use loff to stop it"ENDL);
    }
	else
	{
/*
		if ((board.pcb_spin == 'A') && (board.pcb_version >= 3))
		{ // in P3479-A03, INA3221 is removed, so this command is not supported
			IfxStdIf_DPipe_print(io, "Detect board is P3479-A03 onwards"ENDL);
			IfxStdIf_DPipe_print(io, "INA3221 is removed since P3479-A03\r\nSo \"rdpower\" command is not supported"ENDL);
			return TRUE;
		}
*/
		temp_args = args;
		if ((Ifx_Shell_matchToken(&args, "c") != FALSE) || (Ifx_Shell_matchToken(&args, "C") != FALSE))
		{
			display_power_continuous = TRUE;
		}
		else
		{
			display_power_continuous = FALSE;
			args = temp_args;
			if (Ifx_Shell_parseUInt32(&args, &display_power_loop_count, FALSE) == FALSE)
			{
				display_power_loop_count = 1;
			}
		}

		if (Ifx_Shell_parseUInt32(&args, &display_power_interval, FALSE) == FALSE)
		{
			display_power_interval = 1000;
		}
		if (display_power_interval==0)
		{
			display_power_interval = 1;
		}
		display_power = TRUE;
	}
    return TRUE;
}

static t_channel_display CHANNEL_CONFIG[6]=
{
	{&i2c_ina_1, 1, "XA_5V"},
	{&i2c_ina_1, 2, "XA_PREREG"},
	{&i2c_ina_1, 3, "SXMA_PREREG"},

	{&i2c_ina_2, 1, "XB_5V"},
	{&i2c_ina_2, 2, "XB_PREREG"},
	{&i2c_ina_2, 3, "SXMB_PREREG"},
};


void Read_PowerMsgINA3221(IfxStdIf_DPipe *io)
{
	float vshunt_ch, vbus_ch, current_ch, power_ch;
	uint8 i, channel_number;

	channel_number=sizeof(CHANNEL_CONFIG)/sizeof(CHANNEL_CONFIG[0]);

	for(i=0; i<channel_number; i++)
	{
		readPowerSensorINA3221(CHANNEL_CONFIG[i].dev_ptr, CHANNEL_CONFIG[i].index, &vshunt_ch, &vbus_ch, &current_ch, &power_ch);
		IfxStdIf_DPipe_print(io, "%-13s: Power(W):  %.3f"ENDL, CHANNEL_CONFIG[i].name, power_ch);
	}
}

void Print_PowerMsgINA3221(IfxStdIf_DPipe *io)
{
	float vshunt_ch, vbus_ch, current_ch, power_ch;

	static uint32 loop_count=0;
	static uint32 interval_count=0;
	uint8 i, channel_number;
//    display_power_interval = 0;
//    display_power_continuous = 0;

	if(display_power)
	{
//        IfxStdIf_DPipe_print(io, "Print_PowerMsgINA3221, eventFlag_1ms_power_printing=%d"ENDL, eventFlag_1ms_power_printing);
		if(eventFlag_1ms_power_printing)
		{
			eventFlag_1ms_power_printing = FALSE;
			if (interval_count == display_power_interval)
			{
                IfxStdIf_DPipe_print(io, "Print_PowerMsgINA3221, interval_count =%d"ENDL, interval_count);
#if 0
                channel_number=sizeof(CHANNEL_CONFIG)/sizeof(CHANNEL_CONFIG[0]);
#else
				if(board.project == E3550)
					channel_number=sizeof(CHANNEL_CONFIG)/sizeof(CHANNEL_CONFIG[0]);
				else
				{
					channel_number=sizeof(CHANNEL_CONFIG)/sizeof(CHANNEL_CONFIG[0]);
					//CHANNEL_CONFIG[2].name = "SYS_5V";
				}
#endif
				for(i=0; i<channel_number; i++)
				{
					readPowerSensorINA3221(CHANNEL_CONFIG[i].dev_ptr, CHANNEL_CONFIG[i].index,
							&vshunt_ch, &vbus_ch, &current_ch, &power_ch);

					IfxStdIf_DPipe_print(io, ENDL"CHANNEL %d: %-13s: Shunt(mV):  %.3f  Voltage(V):  %.3f",
							CHANNEL_CONFIG[i].index, CHANNEL_CONFIG[i].name, (vshunt_ch*1000), vbus_ch);
					IfxStdIf_DPipe_print(io, ENDL"                          Current(A):  %.3f  Power(W):  %.3f",
							current_ch, power_ch);
				}

				interval_count = 0;


#if 0
                IfxStdIf_DPipe_print(io, ENDL);
#else
				if(!display_power_continuous)
				{
					loop_count++;
					if (loop_count==display_power_loop_count)
					{
						display_power=FALSE;
						loop_count=0;
						IfxStdIf_DPipe_print(io, ENDL);
					}
				}
#endif
			}
			interval_count++;
		}

	}
}

#if PROJECT == PROJECT_DRIVE_PX2 || PROJECT == PROJECT_AUTO_CRUISE
void Print_PowerMsgINA220(IfxStdIf_DPipe *io)
{
	float power;
	float voltage;
	float shuntvoltage;
	float current;
	static uint32 loop_count=0;
	static uint32 interval_count=0;

	if(display_power)
	{
		if(eventFlag_1ms_power_printing)
		{
			eventFlag_1ms_power_printing = FALSE;
			if (interval_count == display_power_interval)
			{
				readPowerSensorINA220(INA220_0_I2C_ADDR, &shuntvoltage, &current, &voltage, &power);
				IfxStdIf_DPipe_print(io, ENDL"PMIC A: Shunt(mV):  %.3f  Voltage(V):  %.3f  Current(A):  %.3f  Power(W):  %.3f", shuntvoltage,voltage,current,power);
				readPowerSensorINA220(INA220_1_I2C_ADDR, &shuntvoltage, &current, &voltage, &power);
				IfxStdIf_DPipe_print(io, ENDL"PMIC B: Shunt(mV):  %.3f  Voltage(V):  %.3f  Current(A):  %.3f  Power(W):  %.3f", shuntvoltage,voltage,current,power);
				interval_count = 0;

				if(!display_power_continuous)
				{
					loop_count++;
					if (loop_count==display_power_loop_count)
					{
						display_power=FALSE;
						loop_count=0;
						IfxStdIf_DPipe_print(io, ENDL);
					}
				}
			}
			interval_count++;
		}

	}
}
#endif
