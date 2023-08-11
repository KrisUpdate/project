/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#ifndef FAN_H_
#define FAN_H_

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/
typedef enum
{
	FAN_A_Tach_Pin = 0,
	FAN_B_Tach_Pin,
	PULL_FAN_A_B_Tach_Pin,
	Max_Tach_Pin_Number,
}e_fan_tach_pin;
/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
extern volatile uint32 time_ms_passed;

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
#define FAN_NUMBER 4

IFX_EXTERN uint32 rpm[FAN_NUMBER];
IFX_EXTERN boolean Show_FanRPM(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN void fan_detect_init(void);
IFX_EXTERN void fan_detect(void);

#endif /* FAN_H_ */
