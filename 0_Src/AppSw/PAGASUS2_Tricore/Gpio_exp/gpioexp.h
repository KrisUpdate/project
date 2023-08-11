/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef GPIOEXP_H_
#define GPIOEXP_H_
/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include <Cpu/Std/Ifx_Types.h>
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

//INA220 registers
#define GPIO_EXP_CONFIGURATION_REGISTER_HI	 				0x07
#define GPIO_EXP_POLARITY_INVERSION_REGISTER_HI	 			0x05
#define GPIO_EXP_OUTPUT_REGISTER_HI 						0x03
#define GPIO_EXP_INPUT_REGISTER_HI		 					0x01
#define GPIO_EXP_CONFIGURATION_REGISTER_LO	 				0x06
#define GPIO_EXP_POLARITY_INVERSION_REGISTER_LO	 			0x04
#define GPIO_EXP_OUTPUT_REGISTER_LO 						0x02
#define GPIO_EXP_INPUT_REGISTER_LO		 					0x00
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
void SetExpOutputs_to_System_PowerOff_State(void);
void SetExpOutputs_to_System_Inactive_State(void);
// return 1 if input, return 0 if output
boolean readConfig(uint8 i2cbus, uint8 devAddr, uint8 group, uint8 port);
boolean readState(uint8 i2cbus, uint8 devAddr, uint8 group, uint8 port);
// setInput = TRUE: set input;  setInput = FALSE: set output
void setConfig(uint8 i2cbus, uint8 devAddr, uint8 group, uint8 port, boolean setInput);
void setOutputState(uint8 i2cbus, uint8 devAddr, uint8 group, uint8 port, boolean state);

#define SET_GPIOEXP_CONFIG_OUTPUT(x) setConfig(x.i2cbus, x.devAddr, x.group, x.port, FALSE)
#define SET_GPIOEXP_CONFIG_INPUT(x) setConfig(x.i2cbus, x.devAddr, x.group, x.port, TRUE)
#define READ_GPIOEXP_CONFIG(x) readConfig(x.i2cbus, x.devAddr, x.group, x.port)
#define READ_GPIOEXP_STATE(x)  readState(x.i2cbus, x.devAddr, x.group, x.port)
#define SET_GPIOEXP_OUTPUT_HIGH(x) setOutputState(x.i2cbus, x.devAddr, x.group, x.port, TRUE)
#define SET_GPIOEXP_OUTPUT_LOW(x)  setOutputState(x.i2cbus, x.devAddr, x.group, x.port, FALSE)

#endif /* GPIOEXP_H_ */
