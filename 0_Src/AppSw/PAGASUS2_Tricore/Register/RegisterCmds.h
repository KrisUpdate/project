/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef REGISTERCMDS_H_
#define REGISTERCMDS_H_
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

IFX_EXTERN boolean Register_Read(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean Register_Write(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean aurixrst(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean uartflash(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN void ota_aurixrst(void);
IFX_EXTERN boolean Register_Read_keyword(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN void aurix_reset(void);

#define SWRSTCON 0XF0036060u

#endif /* REGISTERCMDS_H_ */
