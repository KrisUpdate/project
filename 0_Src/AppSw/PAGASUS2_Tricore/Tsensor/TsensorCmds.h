/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef TSENSORCMDS_H_
#define TSENSORCMDS_H_
/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

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
extern volatile boolean display_temperature;
static boolean display_temp_continuous = FALSE;
static uint32 display_temperature_loop_count;
static uint32 display_temperature_interval;
extern volatile boolean eventFlag_10ms_temp_printing;
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

IFX_EXTERN boolean ReadTemp(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean ThermTest(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean Aurix2ThermTest(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN void Print_TemperatureMsg(IfxStdIf_DPipe *io);
IFX_EXTERN void ReadTsensor(IfxStdIf_DPipe *io);


#endif /* TSENSORCMDS_H_ */
