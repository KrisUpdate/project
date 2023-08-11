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
#include "VadcCmds.h"
#if PROJECT == PROJECT_PAGASUS2
#include "EvadcAutoScan.h"
#else
#include "VadcAutoScan.h"
#endif
#include "Configuration.h"
#include "Port/Std/IfxPort.h"
#include "SysSe/Bsp/Bsp.h"
#include "string.h"
#include "resource.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#if PROJECT != PROJECT_PAGASUS2
#define float_support
#else
#undef float_support
#endif
/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
boolean isDoShowVoltages = FALSE;
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
/** \brief Handle the 'showan' command
 *
 * \par Syntax
 *  - showan x y: show the values of ANx until ANy
 */
boolean Show_ANx_y(pchar args, void *data, IfxStdIf_DPipe *io)
{
	uint32 chnNrStart, chnNrEnd, uiValue, chnNr;
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : showan x"ENDL);
        IfxStdIf_DPipe_print(io, "           > Show the value of ANx"ENDL);
        return TRUE;
    }
	// first we get the first argument
    chnNrStart = 0;
	if (Ifx_Shell_parseUInt32(&args, &chnNrStart, FALSE) == FALSE)
   	{
   		IfxStdIf_DPipe_print(io, "Syntaxerror : invalid AN number %s"ENDL, args);
   		return TRUE;
   	}
    if (chnNrStart > 47)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid AN number %d"ENDL, chnNrStart);
        return TRUE;
    }
	// get the second argument if exist
    chnNrEnd = 0;
	if (Ifx_Shell_parseUInt32(&args, &chnNrEnd, FALSE) == FALSE)
   	{
	    chnNrEnd = chnNrStart;
   	}
    if (chnNrEnd > 47)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid AN number %d"ENDL, chnNrEnd);
        return TRUE;
    }

    for (chnNr = chnNrStart; chnNr <= chnNrEnd; chnNr++)
    {

    	boolean valid_flag = VadcAutoScan_getResult(chnNr, &uiValue);
#ifdef float_support
    	if (valid_flag)
    		IfxStdIf_DPipe_print(io, "AN%d : 0x%03X %04d %fV"ENDL, chnNr, uiValue, uiValue, 5.0f/4095.0f*(float)uiValue);
    	else
    		IfxStdIf_DPipe_print(io, "AN%d : 0x%03X %04d %fV (no VF bit)"ENDL, chnNr, uiValue, uiValue, 5.0f/4095.0f*(float)uiValue);
#else
    	uint32 mvVoltage = uiValue*5*1000/4095;

    	if (valid_flag)
			IfxStdIf_DPipe_print(io, "AN%d : 0x%03X %04d %d.%03dV"ENDL, chnNr, uiValue, uiValue, mvVoltage/1000, mvVoltage%1000);
		else
			IfxStdIf_DPipe_print(io, "AN%d : 0x%03X %04d %d.%03dV (no VF bit)"ENDL, chnNr, uiValue, uiValue, mvVoltage/1000, mvVoltage%1000);

#endif
    }

    return TRUE;
}

/** \brief Get the raw result of ANx channel
 *
 * \par chnNr
 *      the ANx channel
 * \return uint32
 *      return the raw result
 */
uint32 GetANxRawResult(uint8 chnNr)
{
	uint32 uiValue = 0;
	VadcAutoScan_getResult(chnNr, &uiValue);
	return uiValue;
}

/** \brief Handle the 'showvoltages' command
 *
 * \par Syntax
 *  - showvoltages show all voltages connected to VDAC pins
 */
uint32 current_value[4]; //test parameter
boolean show_voltage_log = TRUE;


void Set_ShowVoltages_log(boolean flag)
{
    show_voltage_log = flag;
    //IfxStdIf_DPipe_print(io_tmp, "show_voltage_log = %d"ENDL, show_voltage_log);
}

boolean ShowVoltages(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 uiValue, current_channel, chnNr;
    uint8 count,sel1,sel0;
    IfxPort_Pin * pPortPinL;
    IfxPort_Pin * pPortPinH;
    boolean adc_shared = FALSE;
    IfxPort_Pin *pPort;
    float raw_value;

    isDoShowVoltages = TRUE;

    count = 0;
    uint32 max_pin_number = VadcAutoScan_getPinNumber();
    for (chnNr = 0; chnNr < max_pin_number; chnNr++)
    {
    	adc_shared = FALSE;
/* If ADC_MUX_CTRL_3 is not defined, we don't need to check group 2 */
#if defined (ADC_MUX_CTRL_3) && defined (ADC_MUX_CTRL_4)
    	if((IfxVadc_AN[chnNr].groupShared & GROUP_ID_MASK) == 2)
		{
			adc_shared = TRUE;
			pPortPinL = &(ADC_MUX_CTRL_3);
			pPortPinH = &(ADC_MUX_CTRL_4);
		}
    	else if(IfxVadc_AN[chnNr].groupShared == 1)
#else
    	if((IfxVadc_AN[chnNr].groupShared & GROUP_ID_MASK) == 1)
#endif
    	{
			adc_shared = TRUE;
			pPortPinL = &(ADC_MUX_CTRL_1);
			pPortPinH = &(ADC_MUX_CTRL_2);
    	}
		if (adc_shared)
		{
			//flip ADC mux selection
			count = (IfxVadc_AN[chnNr].groupShared & GROUP_SEL_MASK) >> 4;
			sel0 = count & 0x1;
			sel1 = (count & 0x2) >> 1;
			if (sel0)
			{
				IfxPort_setPinHigh(pPortPinL->port, pPortPinL->pinIndex);
			}
			else
			{
				IfxPort_setPinLow(pPortPinL->port, pPortPinL->pinIndex);
			}
			if (sel1)
			{
				IfxPort_setPinHigh(pPortPinH->port, pPortPinH->pinIndex);
			}
			else
			{
				IfxPort_setPinLow(pPortPinH->port, pPortPinH->pinIndex);
			}
			waitTime(1*TimeConst[TIMER_INDEX_1MS]);
		}

		current_channel = IfxVadc_AN[chnNr].pinIndex;
		if(current_channel >= MAX_ANALOG_CHANNEL)
		{/* we support channel larger than max valid channel, to display some meaningful helper message */
			if(current_channel == DISPLAY_ONLY_CHANNEL)
			{
				if(show_voltage_log == TRUE)
				IfxStdIf_DPipe_print(io, "%s\r\n", IfxVadc_AN[chnNr].railName);
			}
			continue;
		}
		while(1)
		{
            sem_wait_until_get(SEM_ADC, __LINE__, __func__);
			boolean valid_flag = VadcAutoScan_getResult(current_channel, &uiValue);
            sem_release(SEM_ADC);
			if (valid_flag)
			{
				raw_value = (float)uiValue/4095*5;
				if((strcmp(IfxVadc_AN[chnNr].railName, "snn") == 0) ||
				   (strcmp(IfxVadc_AN[chnNr].railName, "SNN") == 0))
				{ /* we ignore the snn rail name */
					break;
				}
				if (IfxVadc_AN[chnNr].adjustment2_start_value > 0)
				{
					if (raw_value > IfxVadc_AN[chnNr].adjustment2_start_value)
					{
#ifdef float_support
						IfxStdIf_DPipe_print(io, "  %s = %.3f %s"ENDL, IfxVadc_AN[chnNr].railName,(raw_value*((IfxVadc_AN[chnNr].Rtop +IfxVadc_AN[chnNr].Rbottom)/IfxVadc_AN[chnNr].Rbottom) * IfxVadc_AN[chnNr].multiplier2) + IfxVadc_AN[chnNr].offset2,IfxVadc_AN[chnNr].unit);
#else
						uint32 mvVoltage =
								(uint32)(((raw_value*((IfxVadc_AN[chnNr].Rtop +IfxVadc_AN[chnNr].Rbottom)/IfxVadc_AN[chnNr].Rbottom)
										* IfxVadc_AN[chnNr].multiplier2) + IfxVadc_AN[chnNr].offset2) * 1000);
						if(show_voltage_log == TRUE)
						IfxStdIf_DPipe_print(io, "  %s = %d.%03d %s"ENDL,
								IfxVadc_AN[chnNr].railName,	mvVoltage/1000, mvVoltage%1000,	IfxVadc_AN[chnNr].unit);
#endif
						break;
					}
				}
#ifdef float_support
				IfxStdIf_DPipe_print(io, "  %s = %.3f %s"ENDL, IfxVadc_AN[chnNr].railName,(raw_value*((IfxVadc_AN[chnNr].Rtop +IfxVadc_AN[chnNr].Rbottom)/IfxVadc_AN[chnNr].Rbottom) * IfxVadc_AN[chnNr].multiplier) + IfxVadc_AN[chnNr].offset,IfxVadc_AN[chnNr].unit);
#else
				uint32 mvVoltage =
						(uint32)(((raw_value*((IfxVadc_AN[chnNr].Rtop +IfxVadc_AN[chnNr].Rbottom)/IfxVadc_AN[chnNr].Rbottom)
								* IfxVadc_AN[chnNr].multiplier) + IfxVadc_AN[chnNr].offset)*1000);
				if(show_voltage_log == TRUE)
				IfxStdIf_DPipe_print(io, "  %s = %d.%03d %s"ENDL, IfxVadc_AN[chnNr].railName,
						mvVoltage/1000, mvVoltage%1000, IfxVadc_AN[chnNr].unit);

				if (strcmp(IfxVadc_AN[chnNr].railName, "VBATSYS_ISENSE") == 0){
					current_value[0] = mvVoltage;
				}
				if (strcmp(IfxVadc_AN[chnNr].railName, "VBATTEG_ISENSE") == 0){
					current_value[1] = mvVoltage;
				}
				if (strcmp(IfxVadc_AN[chnNr].railName, "VBATSXMA_ISENSE") == 0){
					current_value[2] = mvVoltage;
				}
				if (strcmp(IfxVadc_AN[chnNr].railName, "VBATSXMB_ISENSE") == 0){
					current_value[3] = mvVoltage;
				}
#endif
				break;
			}
			else
			{
				if(show_voltage_log == TRUE)
					IfxStdIf_DPipe_print(io, "  chan %d flag is not set"ENDL, current_channel);
				break;
			}
		}
    }
    isDoShowVoltages = FALSE;
	
    return TRUE;
}


