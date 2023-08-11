/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#ifndef EVADCAUTOSCAN_H
#define EVADCAUTOSCAN_H 1

#include "Configuration.h"

#if PROJECT == PROJECT_PAGASUS2

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include <Evadc/Std/IfxEvadc.h>
#include <Evadc/Adc/IfxEvadc_Adc.h>

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define USED_GROUPS  6
#define NO_GROUP_SHARED  0xff
#define GROUP_ID_MASK     0x0F
#define GROUP_SEL_MASK    0xF0
#define MAX_ANALOG_CHANNEL   48
#define DISPLAY_ONLY_CHANNEL 0xFF
#define KL30_VBAT1_CHAN_ID	0
#define KL30_VBAT2_CHAN_ID	10

/* This is the array index in "IfxVadc_AN_P3570", used by P3570 only
 * */
#define P3570_A2_3V3_INDEX  15
#define P3570_A2_1V3_INDEX  16
/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/
typedef struct
{
    IfxEvadc_Adc vadc; /* VADC handle */
    IfxEvadc_Adc_Group adcGroup[USED_GROUPS];
    IfxEvadc_Adc_Channel adcChannel[USED_GROUPS*8];
} App_VadcAutoScan;

typedef struct
{
    uint8  pinIndex;
    /* Group status: it contains two part: group selection (MSB 4 bit) + group id (LSB 4 bit)
     * group selection (MSB 4 bit): define which selection it's in the mux
     * group id (LSB 4 bit): define to use which group selection pins:
     * 	  1 => ADC_MUX_CTRL_1, ADC_MUX_CTRL_2
     * 	  2 => ADC_MUX_CTRL_3, ADC_MUX_CTRL_4 */
    uint8 groupShared;
    float Rtop;
    float Rbottom;
    char railName[25];
    float multiplier;	//adjustment 1
    float offset;
    float adjustment2_start_value;
    float multiplier2;
    float offset2;
    char unit[8];
} IfxVadc_Pin;

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
IFX_EXTERN const IfxVadc_Pin* IfxVadc_AN;
IFX_EXTERN App_VadcAutoScan g_VadcAutoScan;
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
IFX_EXTERN void VadcAutoScan_init(void);
IFX_EXTERN void VadcAutoScan_run(void);
IFX_EXTERN boolean VadcAutoScan_getResult(uint32 chnIx, uint32 *result_p);
IFX_EXTERN uint32 VadcAutoScan_getPinNumber(void);
IFX_EXTERN void VadcAutoScan_initConfig(void);
IFX_EXTERN uint32 VadcAutoScan_getVoltage_mV(uint32 chnIx);
IFX_EXTERN void VadcAutoScan_clear_kl30_interrupt_flag(void);
IFX_EXTERN void VadcAutoScan_set_kl30_low_thres(uint32 threshold_mv);
IFX_EXTERN void VadcAutoScan_set_kl30_high_thres(uint32 threshold_mv);


/*\brief Get the voltage in mV after calculating. For example,
 *   if the ADC monitor SYS_5V, it should return value around 5000mV
 *\param: in: char*  name: this is the array rail name in IfxVadc_AN_E3550/P3570
 *\param: in: uint32 configIndex: Only used when "name" is NULL.
 *\                  This is the array index in IfxVadc_AN_E3550/P3570, not channel index
 *\param: uint32: return 0 if ADC result is not valid
 *\param:         return voltage value in mV if large than 0.
 */
IFX_EXTERN uint32 VadcAutoScan_getCalCulatedVoltage_mV(char* name, uint32 configIndex);

/*\brief Get the GPIO status high/low via ADC channel
 *\param: in: char*  name: this is the array rail name in IfxVadc_AN_E3550/P3570
 *\param: in: uint32 configIndex: Only used when "name" is NULL.
 *\                  This is the array index in IfxVadc_AN_E3550/P3570, not channel index
 *\param: boolean: return FALSE if GPIO is low
 *\param:          return TRUE if GPIO is high
 */
IFX_EXTERN boolean VadcAutoScan_getGPIOState(char* name, uint32 configIndex);

#endif //#if PROJECT == PROJECT_DRIVE_AX

#endif
//end of EVADCAUTOSCAN_H
