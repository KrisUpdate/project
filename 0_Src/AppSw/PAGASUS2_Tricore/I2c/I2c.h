/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef _I2C_H
#define _I2C_H
/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include <Cpu/Std/Ifx_Types.h>

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define NUM_I2C_MODULE 		2
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
#define I2C_DEBUG_ENABLE 0

#if I2C_DEBUG_ENABLE == 1
#define I2C_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define I2C_DEBUG_PRINTF(x)
#endif

#define I2C_ALWAYS_PRINTF(x)  DEBUG_PRINTF_ALWAYS(x)

sint32 i2c_init (uint8 i2c_module, uint32 baudrate);
void i2c_update_gr(void);
boolean i2c_check_device(uint8 i2c_module, uint8 device_address);
uint8 i2c_write_bytes(uint8 i2c_module, uint8 device_address, uint8 address, uint32 cnt, uint8 *pData);
uint8 i2c_read_bytes(uint8 i2c_module, uint8 device_address, uint8 address, uint32 cnt, uint8 *pData, boolean use_reg_address);

#endif /* _I2C_H */
