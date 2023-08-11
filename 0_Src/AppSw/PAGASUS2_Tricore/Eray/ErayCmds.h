/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#ifndef ERAYCMDS_H
#define ERAYCMDS_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "SysSe/Bsp/Bsp.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/
typedef enum{
    ERAY_PRINT_ALWAYS = 0,
    ERAY_PRINT_ONCE = 1,
    ERAY_PRINT_OFF = 2
}eray_rev_flag_enum_t;
/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
extern volatile boolean eraystress_test;

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
IFX_EXTERN boolean erayinit(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean eraysend(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean eraytest(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean eraytoudp(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean send_Eray_Wakeup(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean eraystress(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN void Print_ErayTestModeMsg(IfxStdIf_DPipe *io);
boolean AppShell_eray(pchar args, void *data, IfxStdIf_DPipe *io);
boolean AppShell_eray_config(pchar args, void *data, IfxStdIf_DPipe *io);
boolean AppShell_eray_test(pchar args, void *data, IfxStdIf_DPipe *io);
#endif
