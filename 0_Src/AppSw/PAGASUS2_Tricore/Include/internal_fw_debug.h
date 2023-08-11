/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef __INTERNAL_FW_DEBUG_H_
#define __INTERNAL_FW_DEBUG_H_

//TODO need to fix the way include interface file
#include "AsclinShellInterface.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#if DEBUG_ENABLE == 1
//#define DEBUG_PRINTF(x)  AsclinShellInterface_printf x
#define DEBUG_PRINTF(x)  AsclinShellInterface_printf x
#else
#define DEBUG_PRINTF(x)
#endif

//#define DEBUG_PRINTF_ALWAYS(x) AsclinShellInterface_printf x
#define DEBUG_PRINTF_BUFFER(x,y)  AsclinShellInterface_printbuffer((x), (y))
//#define DEBUG_PRINTF_BUFFER_32BIT(x,y)  AsclinShellInterface_printbuffer32bit((x), (y))

#define DEBUG_PRINTF_ALWAYS(x) AsclinShellInterface_printf x
//#define DEBUG_PRINTF_ALWAYS(x)
//#define DEBUG_PRINTF_ALWAYS(x) AsclinShellInterface_printf_test x

//#define DEBUG_PRINTF_BUFFER(x,y)
#define DEBUG_PRINTF_BUFFER_32BIT(x,y) AsclinShellInterface_printbuffer32bit((x), (y))


#endif  // __INTERNAL_FW_DEBUG_H_
