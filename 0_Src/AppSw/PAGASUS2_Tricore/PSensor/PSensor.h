/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef PSENSOR_H_
#define PSENSOR_H_
/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include <Cpu/Std/Ifx_Types.h>
#include <Configuration.h>
#include "board.h"
#include "board_I2C_devices.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

//INA220 registers
#define INA_220_CONFIGURATION_REGISTER	 				0x00
#define INA_220_SHUNT_VOLTAGE_REGISTER	 				0x01
#define INA_220_BUS_VOLTAGE_REGISTER 					0x02
#define INA_220_POWER_REGISTER		 					0x03
#define INA_220_CURRENT_REGISTER		 				0x04
#define INA_220_CALIBRATION_REGISTER		 			0x05

//#define CALIBRATION_VALUE		19172  //after first calibration
#define CALIBRATION_VALUE		19813  //after optional second calibration at
#define SHUNT_LSB				0.01
#define VOLTAGE_LSB				0.004
#define CURRENT_LSB				0.0010682
#define POWER_LSB				0.021364

#define INA_220_BUS_VOLTAGE_REGISTER_CNVR	    0b0000000000000010
#define INA_220_BUS_VOLTAGE_REGISTER_VOLTAGE 	0b1111111111111000

/* Macros for INA3221 */
#define INA3221_I2C_BUS                 0x00
#define INA3221_0_I2C_ADDR              0x40
#define INA3221_1_I2C_ADDR				0x41

//INA3221 registers
#define INA_3221_CONFIGURATION_REGISTER                         0x00
#define INA_3221_SHUNT_VOLTAGE_CHANNEL_1_REGISTER               0x01
#define INA_3221_BUS_VOLTAGE_CHANNEL_1_REGISTER                 0x02
#define INA_3221_SHUNT_VOLTAGE_CHANNEL_2_REGISTER               0x03
#define INA_3221_BUS_VOLTAGE_CHANNEL_2_REGISTER                 0x04
#define INA_3221_SHUNT_VOLTAGE_CHANNEL_3_REGISTER               0x05
#define INA_3221_BUS_VOLTAGE_CHANNEL_3_REGISTER                 0x06
#define INA_3221_CHANNEL_1_CRIT_ALERT                           0x07
#define INA_3221_CHANNEL_1_WARN_ALERT                           0x08
#define INA_3221_CHANNEL_2_CRIT_ALERT                           0x09
#define INA_3221_CHANNEL_2_WARN_ALERT                           0x0A
#define INA_3221_CHANNEL_3_CRIT_ALERT                           0x0B
#define INA_3221_CHANNEL_3_WARN_ALERT                           0x0C
#define INA_3221_SHUNT_VOLTAGE_SUM                              0X0D
#define INA_3221_SHUNT_VOLTATE_SUM_LIMIT                        0x0E
#define INA_3221_MASK_ENABLE                                    0x0F
#define INA_3221_POWER_VALID_UPPER_LIMIT                        0x10
#define INA_3221_POWER_VALID_LOWER_LIMIT                        0x11
#define INA_3221_MANUFACTURER_ID                                0xFE
#define INA_3221_DIE_ID                                         0xFF

#define INA_3221_CHANNEL_1_ENABLE                 0x04
#define INA_3221_CHANNEL_2_ENABLE                 0x02
#define INA_3221_CHANNEL_3_ENABLE                 0x01
#define INA_3221_CHANNEL_MASK                     0x70
#define INA_3221_CHANNEL_SHIFT                    0x04
#define INA_3221_VOLTAGE_REGISTER_MASK            0x7FF8 //0b1111111111111000

#define INA_3221_VOLTAGE_LSB		0.008
#define INA_3221_SHUNT_LSB			0.04

#define SHUNT_RESISTOR   0.002  //2 mOhm
#define SHUNT_RESISTOR_DGPU   0.0005  //0.5 mOhm

#define SYS_5V_TEG_SHUNT				0.001
#define SYS_PREREG_TEG_SHUNT			0.001
#define CVM_PREREG_SHUNT				0.001


/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
typedef struct
{
	t_i2c_device* dev_ptr;
	uint8 index;
	char* name;
}t_channel_display;
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
void initPowersensors(void);
void readPowerSensorINA3221(t_i2c_device* dev_ptr, uint8 channel_id,
	float *vshunt_ch1, float *vbus_ch1, float *current_ch1, float *power_ch1);
#endif /* PSENSOR_H_ */
