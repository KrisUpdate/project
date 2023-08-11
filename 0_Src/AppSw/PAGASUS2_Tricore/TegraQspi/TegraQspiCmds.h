/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef TEGRAQSPICMDS_H
#define TEGRAQSPICMDS_H 1

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

IFX_EXTERN boolean tegraqspiinit(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean tegrawriteread(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean spimastertest(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean spislaveloopback(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean spislavewrite(pchar args, void *data, IfxStdIf_DPipe *io);

#endif
