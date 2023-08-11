/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef GPIOCMDS_H
#define GPIOCMDS_H 1

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
extern volatile boolean display_thermal_status;
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
IFX_EXTERN boolean Show_PortState(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean Show_PortState_all(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean Set_PortState(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean Config_Port(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean System_PowerOff_Cmd(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean System_InActive(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean System_PowerOn_Cmd(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean TegraRecovery(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean TegraReset(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean Aurix2Reset(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean Aurix2Recovery(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean Aurix2Flash(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean CamPower(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean HDMIASource(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean GMSLSource(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean GMSLCable(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean HDMIBSource(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean ShowPgood(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean ThermStatus(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN void Print_Thermal_Status(IfxStdIf_DPipe *io);
#if PROJECT == PROJECT_PAGASUS2 && PG2_PREEVT
IFX_EXTERN boolean SetBootStrap(pchar args, void *data, IfxStdIf_DPipe *io);
#endif
#endif
