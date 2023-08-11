/**
 * \file TLF35584Cmds.h
 * \brief TLF35584 commands for AscLin Shell
 *
 * \version iLLD_Demos_1_0_0_5_0
 * \copyright Copyright (c) 2015 Infineon Technologies AG. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 *
 * Infineon Technologies AG (Infineon) is supplying this file for use
 * exclusively with Infineon's microcontroller products. This file can be freely
 * distributed within development tools that are supporting such microcontroller
 * products.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 */

#ifndef TLF35584CMDS_H
#define TLF35584CMDS_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "SysSe/Bsp/Bsp.h"
#include "Configuration.h"
#include "TLF35584.h"
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
extern volatile boolean eventFlag_100ms_tlf_wd_printing;
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

IFX_EXTERN boolean TLF_Standby(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean Show_TLF_Register(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean Write_TLF_Register(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN boolean TLF_Wwd_Test(pchar args, void *data, IfxStdIf_DPipe *io);
IFX_EXTERN void Print_TLF_WD_Status(IfxStdIf_DPipe *io);
IFX_EXTERN void TLF_set_wakeup_peripherals(void);
IFX_EXTERN void TLF_regular_check(void);
#endif
