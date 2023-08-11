/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef PSENSORCMDS_H_
#define PSENSORCMDS_H_
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
extern volatile boolean display_power;
static boolean display_power_continuous = FALSE;
static uint32 display_power_loop_count;
static uint32 display_power_interval;
extern volatile boolean eventFlag_1ms_power_printing;
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

IFX_EXTERN boolean ReadPower(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN void Print_PowerMsgINA220(IfxStdIf_DPipe *io);
IFX_EXTERN void Print_PowerMsgINA3221(IfxStdIf_DPipe *io);
IFX_EXTERN void Read_PowerMsgINA3221(IfxStdIf_DPipe *io);
#endif /* PSENSORCMDS_H_ */
