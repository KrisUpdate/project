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

#include <stdio.h>
#include "EvadcAutoScan.h"
#include <Cpu/Std/IfxCpu.h>
#include "Configuration.h"
#include <internal_fw_debug.h>
#include "_Reg\IfxEvadc_reg.h"
#include "string.h"
#if PROJECT == PROJECT_PAGASUS2
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define EVADC_DEBUG_ENABLE 0

#if EVADC_DEBUG_ENABLE == 1
#define EVADC_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define EVADC_DEBUG_PRINTF(x)
#endif

#define EVADC_ALWAYS_PRINTF(x) DEBUG_PRINTF_ALWAYS(x)

#define KL30_CHAN_ID    0
/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/

#if ISR_PROVIDER_VADC == 0
	#if defined(__GNUC__)
	#pragma section ".bss_cpu0" awc0
	#endif
	#if defined(__TASKING__)
	#pragma section farbss "bss_cpu0"
	#pragma section fardata "data_cpu0"
	#endif
	#if defined(__DCC__)
	#pragma section DATA ".data_cpu0" ".bss_cpu0" far-absolute RW
	#endif
#elif ISR_PROVIDER_VADC == 1
	#if defined(__GNUC__)
	#pragma section ".bss_cpu1" awc1
	#endif
	#if defined(__TASKING__)
	#pragma section farbss "bss_cpu1"
	#pragma section fardata "data_cpu1"
	#endif
	#if defined(__DCC__)
	#pragma section DATA ".data_cpu1" ".bss_cpu1" far-absolute RW
	#endif
#elif ISR_PROVIDER_VADC == 2
	#if defined(__GNUC__)
	#pragma section ".bss_cpu2" awc2
	#endif
	#if defined(__TASKING__)
	#pragma section farbss "bss_cpu2"
	#pragma section fardata "data_cpu2"
	#endif
	#if defined(__DCC__)
	#pragma section DATA ".data_cpu2" ".bss_cpu2" far-absolute RW
	#endif
#else
#error "Set ISR_PROVIDER_VADC to a valid value!"
#endif

App_VadcAutoScan g_VadcAutoScan; /**< \brief Vadc information */

#if defined(__GNUC__)
#pragma section
#endif
#if defined(__TASKING__)
#pragma section farbss restore
#pragma section fardata restore
#endif
#if defined(__DCC__)
#pragma section DATA RW
#endif

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/
typedef struct
{
	IfxEvadc_GroupId group;
	uint8 channel;
}t_adc_group_channel;

static const IfxEvadc_GroupId USED_GROUP[]=
{
	IfxEvadc_GroupId_0,
	IfxEvadc_GroupId_1,
	IfxEvadc_GroupId_2,
	IfxEvadc_GroupId_3,
	IfxEvadc_GroupId_8,
};

static const t_adc_group_channel ADC_CHANNEL[]=
{
	{IfxEvadc_GroupId_0, 0},
	{IfxEvadc_GroupId_0, 1},
	{IfxEvadc_GroupId_0, 2},
	{IfxEvadc_GroupId_0, 3},
	{IfxEvadc_GroupId_0, 4},
	{IfxEvadc_GroupId_0, 5},
	{IfxEvadc_GroupId_0, 6},
	{IfxEvadc_GroupId_0, 7},
	{IfxEvadc_GroupId_1, 0},
	{IfxEvadc_GroupId_1, 1},
	{IfxEvadc_GroupId_1, 2},
	{IfxEvadc_GroupId_1, 3},
	{IfxEvadc_GroupId_1, 4},
	{IfxEvadc_GroupId_1, 5},
	{IfxEvadc_GroupId_1, 6},
	{IfxEvadc_GroupId_1, 7},
	{IfxEvadc_GroupId_2, 0},
	{IfxEvadc_GroupId_2, 1},
	{IfxEvadc_GroupId_2, 2},
	{IfxEvadc_GroupId_2, 3},
	{IfxEvadc_GroupId_2, 4},
	{IfxEvadc_GroupId_2, 5},
	{IfxEvadc_GroupId_2, 6},
	{IfxEvadc_GroupId_2, 7},
	{IfxEvadc_GroupId_3, 0},
	{IfxEvadc_GroupId_3, 1},
	{IfxEvadc_GroupId_3, 2},
	{IfxEvadc_GroupId_3, 3},
	{IfxEvadc_GroupId_3, 4},
	{IfxEvadc_GroupId_3, 5},
	{IfxEvadc_GroupId_3, 6},
	{IfxEvadc_GroupId_3, 7},
	{IfxEvadc_GroupId_8, 0},
	{IfxEvadc_GroupId_8, 1},
	{IfxEvadc_GroupId_8, 2},
	{IfxEvadc_GroupId_8, 3},
	{IfxEvadc_GroupId_8, 4},
	{IfxEvadc_GroupId_8, 5},
	{IfxEvadc_GroupId_8, 6},
	{IfxEvadc_GroupId_8, 7},
	{IfxEvadc_GroupId_8, 8},
	{IfxEvadc_GroupId_8, 9},
	{IfxEvadc_GroupId_8, 10},
	{IfxEvadc_GroupId_8, 11},
	{IfxEvadc_GroupId_8, 12},
	{IfxEvadc_GroupId_8, 13},
	{IfxEvadc_GroupId_8, 14},
	{IfxEvadc_GroupId_8, 15},
};
/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

/** \brief Demo init API
 *
 * This function is called from main during initialization phase
 */
void VadcAutoScan_init(void)
{
    /* create configuration */
    IfxEvadc_Adc_Config adcConfig;
    IfxEvadc_Adc_initModuleConfig(&adcConfig, &MODULE_EVADC);

    /* initialize module */
    IfxEvadc_Adc_initModule(&g_VadcAutoScan.vadc, &adcConfig);

    /* create group config */
    IfxEvadc_Adc_GroupConfig adcGroupConfig;
    IfxEvadc_Adc_initGroupConfig(&adcGroupConfig, &g_VadcAutoScan.vadc);

    /* start with group 0 */
    adcGroupConfig.groupId = IfxEvadc_GroupId_0;
    adcGroupConfig.master  = adcGroupConfig.groupId;
    /* new added for TC397 */
    adcGroupConfig.startupCalibration =TRUE;
    /* enable scan source */
    //adcGroupConfig.arbiter.requestSlotScanEnabled = TRUE;
    adcGroupConfig.arbiter.requestSlotQueue0Enabled = TRUE; // enable Queue0 mode
    adcGroupConfig.arbiter.requestSlotQueue1Enabled = TRUE; // enable Queue1 mode
    adcGroupConfig.arbiter.requestSlotQueue2Enabled = TRUE; // enable Queue2 mode
    // enable all gates in "always" mode (no edge detection)
    adcGroupConfig.queueRequest[0].triggerConfig.gatingMode = IfxEvadc_GatingMode_always;
    adcGroupConfig.queueRequest[1].triggerConfig.gatingMode = IfxEvadc_GatingMode_always;
    adcGroupConfig.queueRequest[2].triggerConfig.gatingMode = IfxEvadc_GatingMode_always;
    /* end of new added for TC397 */

    /* enable auto scan */
    //adcGroupConfig.scanRequest.autoscanEnabled = TRUE;

    /* enable all gates in "always" mode (no edge detection) */
    //adcGroupConfig.scanRequest.triggerConfig.gatingMode = IfxEvadc_GatingMode_always;

    /* initialize the groups */
    uint32 i;
    for(i=0; i<ARRAY_SIZE(USED_GROUP); i++)
    {
        /* start with group walking */
        adcGroupConfig.groupId = USED_GROUP[i];
        adcGroupConfig.master  = adcGroupConfig.groupId;
    	IfxEvadc_Adc_initGroup(&g_VadcAutoScan.adcGroup[i], &adcGroupConfig);
    	EVADC_DEBUG_PRINTF(("initial group %d\r\n",USED_GROUP[i]));

    }

    /* set the adc config based on board */
    VadcAutoScan_initConfig();
}

static uint32 get_group_index(uint32 group)
{
	uint32 i;
	for(i=0; i<ARRAY_SIZE(USED_GROUP); i++)
	{
		if(group == USED_GROUP[i])
			break;
	}
	return i;
}

/** \brief Demo run API
 *
 * This function is called from main
 */
void VadcAutoScan_run(void)
{
	uint32 i;
	IfxEvadc_GatingMode savedGate[ARRAY_SIZE(USED_GROUP)];
	IfxEvadc_GatingSource gatingSource[ARRAY_SIZE(USED_GROUP)];
	IfxEvadc_RequestSource requestSource = IfxEvadc_RequestSource_queue0;
	/* IMPORTANT: for deterministic results we have to disable the queue gate while filling the queue,
	 * otherwise results could be output in the wrong order
	 * Disable all the group gate here */
	for(i=0; i<ARRAY_SIZE(USED_GROUP); i++)
	{
		savedGate[i] =	IfxEvadc_getQueueSlotGatingMode(g_VadcAutoScan.adcGroup[i].group,
				requestSource);
		gatingSource[i] = IfxEvadc_getQueueSlotGatingSource(g_VadcAutoScan.adcGroup[i].group,
				requestSource);
		IfxEvadc_setQueueSlotGatingConfig(g_VadcAutoScan.adcGroup[i].group,
					gatingSource[i], IfxEvadc_GatingMode_disabled, requestSource);
	}

	EVADC_DEBUG_PRINTF(("channel num %d\r\n",ARRAY_SIZE(ADC_CHANNEL)));

	/* Initial Channel */
	// create channel config
	IfxEvadc_Adc_ChannelConfig adcChannelConfig;
	for(i=0; i<ARRAY_SIZE(ADC_CHANNEL); i++)
	{
		uint32 group_index = get_group_index(ADC_CHANNEL[i].group);
		if(group_index >= ARRAY_SIZE(USED_GROUP))
		{
			EVADC_ALWAYS_PRINTF(("Group config %d is not in the used group!! \r\n", ADC_CHANNEL[i].group));
			return;
		}
		IfxEvadc_Adc_initChannelConfig(&adcChannelConfig,
				&g_VadcAutoScan.adcGroup[group_index]);
		adcChannelConfig.channelId = (IfxEvadc_ChannelId)(ADC_CHANNEL[i].channel);
		adcChannelConfig.resultRegister = (IfxEvadc_ChannelResult)(ADC_CHANNEL[i].channel);
		// use result register #1 for all channels
		//adcChannelConfig.resultRegister = IfxEvadc_ChannelResult_1;
		/* Enable interrupt for group 0 channel 0*/
		if(i == KL30_CHAN_ID)
		{
			adcChannelConfig.channelPriority     = ISR_PRIORITY_ADC_CHANNEL;
			adcChannelConfig.channelSrcNr        = IfxEvadc_SrcNr_group0;
			adcChannelConfig.channelServProvider = IfxSrc_Tos_cpu0;
			adcChannelConfig.lowerBoundary       = IfxEvadc_BoundarySelection_group0,
			adcChannelConfig.upperBoundary       = IfxEvadc_BoundarySelection_group1,
			adcChannelConfig.boundaryMode        = IfxEvadc_BoundaryExtension_standard,
			adcChannelConfig.limitCheck = IfxEvadc_LimitCheck_eventIfOutsideArea;

			uint32 lower = 0;
			uint32 upper = 0xFFF;

			g_VadcAutoScan.adcGroup[i].group->BOUND.B.BOUNDARY0 = lower;
			g_VadcAutoScan.adcGroup[i].group->BOUND.B.BOUNDARY1 = upper;
		}
	    // initialize the channel
	    IfxEvadc_Adc_initChannel(&g_VadcAutoScan.adcChannel[i], &adcChannelConfig);

		if(i == KL30_CHAN_ID)
		{
			IfxEvadc_setDataModificationMode(g_VadcAutoScan.adcGroup[i].group,
					IfxEvadc_ChannelResult_0, IfxEvadc_DataModificationMode_standardDataReduction);
			IfxEvadc_setDataReductionControlMode(g_VadcAutoScan.adcGroup[i].group,
					IfxEvadc_ChannelResult_0, IfxEvadc_DataReductionControlMode_0);

		}

	    // Add channel to queue with refill enabled
	    IfxEvadc_Adc_addToQueue(&g_VadcAutoScan.adcChannel[i],
	    		requestSource, IFXEVADC_QUEUE_REFILL);

	    EVADC_DEBUG_PRINTF(("initial channel %d\r\n", i));
	}

	// restore previous gate config
	for(i=0; i<ARRAY_SIZE(USED_GROUP); i++)
	{
		IfxEvadc_setQueueSlotGatingConfig(g_VadcAutoScan.adcGroup[i].group,
				gatingSource[i], savedGate[i], requestSource);
		// start the Queue
		IfxEvadc_Adc_startQueue(&g_VadcAutoScan.adcGroup[i], requestSource);
	}
}

/** \brief Get ADC channel raw Result
 *
 *
 */
boolean VadcAutoScan_getResult(uint32 chnIx, uint32 *result_p)
{
	EVADC_DEBUG_PRINTF(("%s, chnix %d\r\n", __func__, chnIx));
	Ifx_EVADC_G_RES conversionResult;
	conversionResult = IfxEvadc_Adc_getResult(&g_VadcAutoScan.adcChannel[chnIx]);
	(*result_p) = conversionResult.B.RESULT;

	if(conversionResult.B.VF)
		return TRUE;
	else
		return FALSE;
}

uint32 VadcAutoScan_getVoltage_mV(uint32 chnIx)
{
	uint32 uiValue;
	uint32 mvVoltage;

	boolean valid_flag = VadcAutoScan_getResult(chnIx, &uiValue);
	mvVoltage = uiValue*5*1000/4095;

	if (valid_flag)
		return (mvVoltage);
	else
		return (0);
}

void VadcAutoScan_clear_kl30_interrupt_flag(void)
{
	IfxEvadc_clearChannelRequest(g_VadcAutoScan.adcGroup[KL30_CHAN_ID].group, 0);

}

void VadcAutoScan_set_kl30_low_thres(uint32 threshold_mv)
{
	uint32 lower = threshold_mv * 10/(49.9+10)*4095/5000;
	uint32 upper = 0xFFF;

	g_VadcAutoScan.adcGroup[KL30_CHAN_ID].group->BOUND.B.BOUNDARY0 = lower;
	g_VadcAutoScan.adcGroup[KL30_CHAN_ID].group->BOUND.B.BOUNDARY1 = upper;
	EVADC_ALWAYS_PRINTF(("Set low threshold: %d mv, raw value 0x%x \r\n",
			threshold_mv, lower));


}

void VadcAutoScan_set_kl30_high_thres(uint32 threshold_mv)
{
	uint32 lower = 0;
	uint32 upper = threshold_mv * 10/(49.9+10)*4095/5000;

	g_VadcAutoScan.adcGroup[KL30_CHAN_ID].group->BOUND.B.BOUNDARY0 = lower;
	g_VadcAutoScan.adcGroup[KL30_CHAN_ID].group->BOUND.B.BOUNDARY1 = upper;
	EVADC_ALWAYS_PRINTF(("Set high threshold: %d mv, raw value 0x%x \r\n",
			threshold_mv, upper));
}

static uint32 get_index_by_name(char* name, uint32 configIndex)
{
	uint32 index = 1; // default value is 1 if there's no finding.
	uint32 i;
	uint32 array_number = VadcAutoScan_getPinNumber();
	if(name == NULL)
	{
		index = configIndex;
	}
	else
	{
		for (i = 0; i < array_number; i++)
		{
			if(strcmp(IfxVadc_AN[i].railName, name) == 0)
			{
				index = i;
				EVADC_DEBUG_PRINTF(("find rail %s at index %d\r\n", name, index));
				break;
			}
		}
		if(i==array_number)
		{
			EVADC_ALWAYS_PRINTF(("Warning: Can not find the voltage name %s\r\n", name));
		}
	}
	return index;
}

uint32 VadcAutoScan_getCalCulatedVoltage_mV(char* name, uint32 configIndex)
{
	uint32 count, sel0, sel1;
	boolean valid_flag;
	float raw_value;
	uint32 uiValue, cal_mV;
	uint32 timeout = 0;
	/* Let's find the config array index */
	uint32 index = get_index_by_name(name, configIndex);

	if((IfxVadc_AN[index].groupShared & GROUP_ID_MASK) == 1)
	{
		//flip ADC mux selection
		count = (IfxVadc_AN[index].groupShared & GROUP_SEL_MASK) >> 4;
		sel0 = count & 0x1;
		sel1 = (count & 0x2) >> 1;
		if (sel0)
		{
			IfxPort_setPinHigh(ADC_MUX_CTRL_1.port, ADC_MUX_CTRL_1.pinIndex);
		}
		else
		{
			IfxPort_setPinLow(ADC_MUX_CTRL_1.port, ADC_MUX_CTRL_1.pinIndex);
		}
		if (sel1)
		{
			IfxPort_setPinHigh(ADC_MUX_CTRL_2.port, ADC_MUX_CTRL_2.pinIndex);
		}
		else
		{
			IfxPort_setPinLow(ADC_MUX_CTRL_2.port, ADC_MUX_CTRL_2.pinIndex);
		}
		waitTime(1*TimeConst[TIMER_INDEX_1MS]);
	}
	valid_flag = VadcAutoScan_getResult(IfxVadc_AN[index].pinIndex, &uiValue);
	while(!valid_flag)
	{ /* If ADC value is not valid, let's wait for a while */
		timeout++;
		if(timeout>10)
		{
			EVADC_ALWAYS_PRINTF(("Warning: TIME OUT, ADC value is not valid!\r\n"));
			return 0;
		}
		waitTime(1*TimeConst[TIMER_INDEX_1MS]);
		valid_flag = VadcAutoScan_getResult(IfxVadc_AN[index].pinIndex, &uiValue);
	}
	raw_value = (float)uiValue/4095*5;

	if ((IfxVadc_AN[index].adjustment2_start_value > 0)&& (raw_value > IfxVadc_AN[index].adjustment2_start_value))
	{

		cal_mV = (uint32)(((raw_value*((IfxVadc_AN[index].Rtop +IfxVadc_AN[index].Rbottom)/IfxVadc_AN[index].Rbottom)
					* IfxVadc_AN[index].multiplier2) + IfxVadc_AN[index].offset2) * 1000);
	}
	else
	{
		cal_mV = (uint32)(((raw_value*((IfxVadc_AN[index].Rtop +IfxVadc_AN[index].Rbottom)/IfxVadc_AN[index].Rbottom)
					* IfxVadc_AN[index].multiplier) + IfxVadc_AN[index].offset)*1000);
	}
	return cal_mV;
}


boolean VadcAutoScan_getGPIOState(char* name, uint32 configIndex)
{
	uint32 value = VadcAutoScan_getCalCulatedVoltage_mV(name, configIndex);
	if(value >= (3300 * 0.7))
	{ // high voltage
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

#endif //#if PROJECT == PROJECT_DRIVE_AX
