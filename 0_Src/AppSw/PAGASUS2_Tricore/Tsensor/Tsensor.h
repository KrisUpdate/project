/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef TSENSOR_H_
#define TSENSOR_H_
/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include <Cpu/Std/Ifx_Types.h>
#include "board_I2C_devices.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
//TMP411 Registers
#define TMP411_LOCAL_TEMP_HBYTE_READ	 				0x00
#define TMP411_LOCAL_TEMP_LBYTE_READ	 				0x15
#define TMP411_REMOTE_TEMP_HBYTE_READ	 				0x01
#define TMP411_REMOTE_TEMP_LBYTE_READ	 				0x10
#define TMP411_STATUS_READ 								0x02
#define TMP411_CONFIGURATION_READ	 					0x03
#define TMP411_CONFIGURATION_WRITE	 					0x09
#define TMP411_CONSECUTIVE_ALERT						0x22

#define TMP411_LOCAL_TEMP_HIGH_LIMIT_HBYTE_READ 		0x05
#define TMP411_LOCAL_TEMP_HIGH_LIMIT_HBYTE_WRITE 		0x0B
#define TMP411_LOCAL_TEMP_HIGH_LIMIT_LBYTE		 		0x16
#define TMP411_LOCAL_TEMP_LOW_LIMIT_HBYTE_READ 			0x06
#define TMP411_LOCAL_TEMP_LOW_LIMIT_HBYTE_WRITE 		0x0C
#define TMP411_LOCAL_TEMP_LOW_LIMIT_LBYTE		 		0x17
#define TMP411_LOCAL_THERM_LIMIT						0x20

#define TMP411_REMOTE_TEMP_HIGH_LIMIT_HBYTE_READ 		0x07
#define TMP411_REMOTE_TEMP_HIGH_LIMIT_HBYTE_WRITE 		0x0D
#define TMP411_REMOTE_TEMP_HIGH_LIMIT_LBYTE		 		0x13
#define TMP411_REMOTE_TEMP_LOW_LIMIT_HBYTE_READ 		0x08
#define TMP411_REMOTE_TEMP_LOW_LIMIT_HBYTE_WRITE 		0x0E
#define TMP411_REMOTE_TEMP_LOW_LIMIT_LBYTE		 		0x14
#define TMP411_REMOTE_THERM_LIMIT						0x19

//TMP411 Register Values
#define TMP411_LOCAL_TEMP_HIGH_LIMIT_VALUES_DEGREE_C 			100//110  //may need to modify later
#define TMP411_LOCAL_TEMP_LOW_LIMIT_VALUES_DEGREE_C 			-45  //may need to modify later
#define TMP411_LOCAL_TEMP_THERM_LIMIT_VALUES_DEGREE_C 			105//115  //may need to modify later

#define TMP411_LOCAL_TEMP_HIGH_LIMIT_VALUES_REG 		TMP411_LOCAL_TEMP_HIGH_LIMIT_VALUES_DEGREE_C + 64
#define TMP411_LOCAL_TEMP_LOW_LIMIT_VALUES_REG 			TMP411_LOCAL_TEMP_LOW_LIMIT_VALUES_DEGREE_C + 64
#define TMP411_LOCAL_TEMP_THERM_LIMIT_VALUES_REG 		TMP411_LOCAL_TEMP_THERM_LIMIT_VALUES_DEGREE_C + 64

//Remote is not used but limit needs to be defined to avoid false alert or therm event
#define TMP411_REMOTE_TEMP_HIGH_LIMIT_VALUES_DEGREE_C 			150
#define TMP411_REMOTE_TEMP_LOW_LIMIT_VALUES_DEGREE_C 			-55
#define TMP411_REMOTE_TEMP_THERM_LIMIT_VALUES_DEGREE_C 			150

#define TMP411_REMOTE_TEMP_HIGH_LIMIT_VALUES_REG	 			TMP411_REMOTE_TEMP_HIGH_LIMIT_VALUES_DEGREE_C + 64
#define TMP411_REMOTE_TEMP_LOW_LIMIT_VALUES_REG 				TMP411_REMOTE_TEMP_LOW_LIMIT_VALUES_DEGREE_C + 64
#define TMP411_REMOTE_TEMP_THERM_LIMIT_VALUES_REG	 			TMP411_REMOTE_TEMP_THERM_LIMIT_VALUES_DEGREE_C + 64

#define  TMP411_CONFIGURATION_RANGE_BIT_MASK          (0x1<<2)
#define  TMP411_CONFIGURATION_ALERT_BIT_MASK          (0x1<<5)
#define  TMP411_CONFIGURATION_SD_BIT_MASK      	  	  (0x1<<6)
#define  TMP411_CONFIGURATION_MASK1_BIT_MASK      	  (0x1<<7)

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
static boolean tempsensor_initialized = FALSE;
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
void initTempsensors(void);
void initAurix2Tempsensors(void);
void clearTempsensors_status(void);
void clearAurx2Tempsensors_status(void);
float readTsensorLocal(t_i2c_device* dev_ptr);
float readTsensorRemote(t_i2c_device* dev_ptr);
void testTsensor(t_i2c_device* dev_ptr, uint32 loops);
void testAurix2Tsensor(t_i2c_device* dev_ptr, uint32 loops);


#endif /* TSENSOR_H_ */
