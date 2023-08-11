/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#ifndef VMON_H_
#define VMON_H_

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include <Cpu/Std/Ifx_Types.h>
#include "Configuration.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define VMON_PASS_VER			3	 // Pass version for VMONs

#define VMON_CID_REG		0x2D // The Register from which to read the CID

#if VMON_PASS_VER == 2
#define VMON1_CID		0x08
#define VMON2_CID		0x09
#define VMON3_CID		0x0A

#elif VMON_PASS_VER == 3
#define VMON1_CID		0x0B
#define VMON2_CID		0x0C
#define VMON3_CID		0x0D
#endif

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

boolean Vmon_init(void);
IFX_EXTERN boolean Vmon_showStatus(pchar args, void *data, IfxStdIf_DPipe *io);

#endif  // VMON_H_
