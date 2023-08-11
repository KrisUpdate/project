/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#ifndef APPASCUART_H
#define APPASCUART_H

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "Configuration.h"
#include "SysSe/Bsp/Bsp.h"
#include "Asclin/Asc/IfxAsclin_Asc.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define UART_BAUD_RATE_19200                   19200
#define UART_BAUD_RATE_115200                115200

#define UART_PACKET_HEADER1_V3NC              0x7b
#define UART_PACKET_HEADER2_V3NC              0x7c
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
IFX_EXTERN void UartApp_Init(uint32);
void UartApp_BlockSend(uint8 byte);
uint8 ReadB2BTest(uint8 v);
void ClearB2BTest(void);
void B2BUartService(void);
#endif // end of #define APPASCUART_H

