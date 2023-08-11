/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#ifndef _ERROR_REPORT_H
#define _ERROR_REPORT_H

#include "Configuration.h"


#define INVALID_ROM_CONTENT_ERR       0
#define INVALID_SJA1105_DEVICE_ERR    1
#define SJA1105_INIT_FAIL_ERR         2
#define INVAILD_88E6321_DEVICE_ERR    3
#define INVAILD_MAC_ADDRESS_ERR       4

void set_error_flag(uint32 error_flag);
void report_error_log(void);

#endif //endof _ERROR_REPORT_H
