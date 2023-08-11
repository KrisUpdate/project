/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#ifndef CIM_CMDS_H_
#define CIM_CMDS_H_


/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "SysSe/Comm/Ifx_Shell.h"
#include "Configuration.h"

#if CAMERA_CIM_ENABLE == 1

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
boolean CIM_Power_Control_Cmd(pchar args, void *data, IfxStdIf_DPipe *io);
boolean CIM_LSW_Control_Cmd(pchar args, void *data, IfxStdIf_DPipe *io);
boolean CIM_Aggregator_Control_Cmd(pchar args, void *data, IfxStdIf_DPipe *io);
boolean CIM_FrameSync_Control_Cmd(pchar args, void *data, IfxStdIf_DPipe *io);
boolean CIM_SGood_Status_Cmd(pchar args, void *data, IfxStdIf_DPipe *io);
boolean CIM_Power_Test_Cmd(pchar args, void *data, IfxStdIf_DPipe *io);

IFX_EXTERN boolean ReadCamVolt(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean ReadCamCurr(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean ReadCamVolCurr(pchar args, void *data, IfxStdIf_DPipe *io);

#endif //CAMERA_CIM_ENABLE == 1

#endif  // CIM_CMDS_H_

