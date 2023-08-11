/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#ifndef CIM_H_
#define CIM_H_


/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include <Cpu/Std/Ifx_Types.h>
#include "Configuration.h"
#include "board_I2C_devices.h"

#if CAMERA_CIM_ENABLE == 1

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define CIM_LSW_MAX20087_MASK_REG          0x00
#define CIM_LSW_MAX20087_CONFIG_REG        0x01
#define CIM_LSW_MAX20087_ID_REG            0x02
#define CIM_LSW_MAX20087_CONFIG_EN_MASK    0x0F

#define MASK_REGISTER_COMMAND          0x00
#define CONFIG_REGISTER_COMMAND        0X01
#define ID_REGISTER_COMMAND            0X02
#define STAT1_REGISTER_COMMAND         0x03
#define STAT2_1_REGISTER_COMMAND       0X04
#define STAT2_2_REGISTER_COMMAND       0X04
#define ADC1_REGISTER_COMMAND          0X06
#define ADC2_REGISTER_COMMAND          0X07
#define ADC3_REGISTER_COMMAND          0X08
#define ADC4_REGISTER_COMMAND          0X09

#define VOLTAGE_CONFIG_SETTING_COMMAND 0X7f
#define CURRENT_CONFIG_SETTING_COMMAND 0X3f


#define NUM_CAM_MONITOR 3
#define NUM_ADC_UNITS   4

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

boolean CIM_Power_Control(boolean powerEnable);
boolean CIM_LSW_Control(t_i2c_device* dev_ptr, uint8 devChannel, boolean powerEnable);
boolean CIM_Check_SGood_Status();
void readCamVoltage(t_i2c_device* dev_ptr, float* camData);
void readCamCurrent(t_i2c_device* dev_ptr, float* camData);
#endif //CAMERA_CIM_ENABLE == 1

#endif  // CIM_H_
