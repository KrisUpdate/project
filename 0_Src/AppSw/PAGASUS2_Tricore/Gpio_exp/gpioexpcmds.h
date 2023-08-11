/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef GPIOEXPCMDS_H_
#define GPIOEXPCMDS_H_
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

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
IFX_EXTERN boolean Show_ExpPortState_all(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean Show_ExpPortState(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean Config_ExpPortState(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean SetOutput_ExpPortState(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean fpdlSource(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean pexcontrol(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean switchpcie(pchar args, void *data, IfxStdIf_DPipe *io);
#endif /* GPIOEXPCMDS_H_ */
