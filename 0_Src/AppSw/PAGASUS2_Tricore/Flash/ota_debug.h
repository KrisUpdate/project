/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#ifndef __OTA_DEBUG_H_
#define __OTA_DEBUG_H_

#include "internal_fw_debug.h"
#include "config_pg2_feasures.h"  
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define OTA_DEBUG_ENABLE 1

#if OTA_DEBUG_ENABLE == 1
#define OTA_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define OTA_DEBUG_PRINTF(x)
#endif

#define OTA_PRINTF_ALWAYS(x) DEBUG_PRINTF_ALWAYS(x)
#define PRINTF_BOOT(x) ({if(boot_msg_flag){DEBUG_PRINTF_ALWAYS(x);}})

#define OTA_UDP_APP_DEBUG_ENABLE 1

#if OTA_UDP_APP_DEBUG_ENABLE == 1


extern boolean boot_msg_flag;
#define OTA_UDP_APP_DEBUG_PRINTF(x)  OTA_PRINTF_ALWAYS(x)//DEBUG_PRINTF(x)
#define PRINTF_BOOT(x) ({if(boot_msg_flag){DEBUG_PRINTF_ALWAYS(x);}})

#else
#define OTA_UDP_APP_DEBUG_PRINTF(x)
#endif


#endif  // __OTA_DEBUG_H_
