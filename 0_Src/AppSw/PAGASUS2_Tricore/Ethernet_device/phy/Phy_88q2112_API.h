/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#ifndef _PHY_88Q2112_API_H_
#define _PHY_88Q2112_API_H_

#include "Configuration.h"

/* PHY GE(1000) Operating Mode */
#define MRVL_88Q2112_MODE_LEGACY        0x06B0
#define MRVL_88Q2112_MODE_DEFAULT       0x0000
#define MRVL_88Q2112_MODE_MSK           0x07F0

/* Added by NVIDIA to use Macro for version number */
#define MRVL_88Q2112_Z1   0
#define MRVL_88Q2112_A0   1
#define MRVL_88Q2112_A1   2
#define MRVL_88Q2112_A2   3

/* Return the API version string */
char* get_api_version(void);

/* Get chip silicon version */
uint32 getRevNum(uint32 slaveaddr);

/* Q2112 Ax API 1.00
  Initial 100M speed */
void initQ2112Fe(uint32 slaveaddr);

/* Q2112 Ax API 1.00
  Initial 1G speed */
//void initQ2112Ge(uint32 slaveaddr);
void initQ2112Ge(uint32 slaveaddr, uint8 mode_sel);

//void initQ2112Ge_B2B(uint32 slaveaddr);
void initQ2112Ge_B2B(uint32 slaveaddr, uint8 mode_sel);

/* Q2112 Z1 API 1.10
  Initial 100M speed */
void initQ2112Z1Fe(uint32 slaveaddr);

/* Q2112 Z1 API 1.10
  Initial 100M speed */
void initQ2112Z1Ge(uint32 slaveaddr);

#endif /* _PHY_88Q2112_API_H_A */
