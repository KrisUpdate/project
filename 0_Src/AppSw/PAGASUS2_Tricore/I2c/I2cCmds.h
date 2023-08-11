/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef I2CCMDS_H
#define I2CCMDS_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "Configuration.h"

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
extern volatile uint32 random_seed;
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
IFX_EXTERN boolean i2cinit(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean i2cscan(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean i2cread(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean i2cwrite(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean i2cromtest(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean i2cromrw(pchar args, void *data, IfxStdIf_DPipe *io);
//#if (PROJECT == PROJECT_DRIVE_AX) && (DRIVE_AX_B0x == 1)
#if 0
IFX_EXTERN boolean i2cromreadwritecemode(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean i2cromreadcimrom(pchar args, void *data, IfxStdIf_DPipe *io);
#endif
#endif
