/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "PSensor.h"
#include "SysSe/Bsp/Bsp.h"
#include <Configuration.h>

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

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

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/
/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
static void initPowersensorINA3221(t_i2c_device* dev_ptr)
{
	uint8 value[2];
	uint8 chmask;

	/* Enable all 3 channels */
	i2c_read_bytes(dev_ptr->bus, dev_ptr->address, INA_3221_CONFIGURATION_REGISTER, 2, &value, 1);
	chmask = (INA_3221_CHANNEL_1_ENABLE | INA_3221_CHANNEL_2_ENABLE | INA_3221_CHANNEL_3_ENABLE) << INA_3221_CHANNEL_SHIFT;
	value[0] &= ~INA_3221_CHANNEL_MASK;
	value[0] |= chmask & INA_3221_CHANNEL_MASK;
	i2c_write_bytes(dev_ptr->bus, dev_ptr->address, INA_3221_CONFIGURATION_REGISTER, 2, &value[0]);
}


void initPowersensors(void)
{
	initPowersensorINA3221(&i2c_ina_1);
	if(board.project != P3479)
	{ // P3479 does not have 2nd INA3221
		initPowersensorINA3221(&i2c_ina_2);
	}
}


void readPowerSensorINA3221(t_i2c_device* dev_ptr, uint8 channel_id, float *vshunt_ch1, float *vbus_ch1, float *current_ch1, float *power_ch1)
{
	uint8 value[2];
	float shunt_current, calibrated_current;
	uint8 shunt_volt_reg_offset = INA_3221_SHUNT_VOLTAGE_CHANNEL_1_REGISTER + (channel_id -1)*2;
	uint8 bus_volt_reg_offset = INA_3221_BUS_VOLTAGE_CHANNEL_1_REGISTER + (channel_id -1)*2;

	i2c_read_bytes(dev_ptr->bus, dev_ptr->address, shunt_volt_reg_offset, 2, &value, 1);
	*vshunt_ch1 = ((((value[0] << 8) | value[1]) & INA_3221_VOLTAGE_REGISTER_MASK) >> 3)*INA_3221_SHUNT_LSB;

	i2c_read_bytes(dev_ptr->bus, dev_ptr->address, bus_volt_reg_offset, 2, &value, 1);
	*vbus_ch1 = ((((value[0] << 8) | value[1]) & INA_3221_VOLTAGE_REGISTER_MASK) >> 3)*INA_3221_VOLTAGE_LSB;

	/* Calibration
	 * calibrated_bus_voltage (V) = bus_voltage
	 * current (A):  shunt_voltage /shunt_resistor / 1000 (as shunt_voltage is mv)
     * E3550: calibrated_current(A) = 1.00068* current  -0.03006
     * P3479: calibrated_current(A) = 1.00052* current  -0.01049
    */
#if 0
	shunt_current = (*vshunt_ch1) / SHUNT_RESISTOR / 1000;
#else
	if (channel_id == 3)
		shunt_current = (*vshunt_ch1) / SHUNT_RESISTOR_DGPU / 1000;
	else
		shunt_current = (*vshunt_ch1) / SHUNT_RESISTOR / 1000;
#endif

	if(board.project == E3550)
		calibrated_current = 1.00068 * shunt_current - 0.03006;
	else{
        // This board for 3570, and its configuration is similar with E3550
		//calibrated_current = 1.00052 * shunt_current - 0.01049;
		calibrated_current = 1.00068 * shunt_current - 0.03006;
	}

	*current_ch1 = calibrated_current;
	*power_ch1 = (*current_ch1) * (*vbus_ch1);
}

/* In TOT we don't support DPX2 and ACH anymore, so keep this source here only */
#if PROJECT == PROJECT_DRIVE_PX2 || PROJECT == PROJECT_AUTO_CRUISE
static void initPowersensorINA220(t_i2c_device* dev_ptr)
{
	uint32 bytesleft;
	uint8 value[2];

	value[0] = ((CALIBRATION_VALUE & 0xff00) >> 8);
	value[1] = (CALIBRATION_VALUE & 0xff);
	bytesleft = i2c_write_bytes(dev_ptr->bus, dev_ptr->address, INA_220_CALIBRATION_REGISTER, 2, &value[0]);
	if (dev_ptr->address == INA220_0_I2C_ADDR)
	{
		powersensor_0_initialized = TRUE;
	}
	if (dev_ptr->address == INA220_1_I2C_ADDR)
	{
		powersensor_1_initialized = TRUE;
	}
}
void readPowerSensorINA220(t_i2c_device* dev_ptr, float *shuntvoltage, float *current, float *voltage, float *power)
{
	uint32 bytesleft;
	uint8 value[2];
#if 0
    while(1)
    {
		bytesleft = i2c_read_bytes(INA220_I2C_BUS,devAddr, INA_220_BUS_VOLTAGE_REGISTER, 2, &value,1);
		if (value & INA_220_BUS_VOLTAGE_REGISTER_CNVR)
			break;
    }
#endif
    bytesleft = i2c_read_bytes(dev_ptr->bus, dev_ptr->address, INA_220_BUS_VOLTAGE_REGISTER, 2, &value,1);
    *voltage = ((((value[0]<<8)|value[1])&INA_220_BUS_VOLTAGE_REGISTER_VOLTAGE)>>3)*VOLTAGE_LSB;
   // *voltage = ((((value[0]<<8)|value[1])&INA_220_BUS_VOLTAGE_REGISTER_VOLTAGE)>>3);
    bytesleft = i2c_read_bytes(dev_ptr->bus, dev_ptr->address, INA_220_SHUNT_VOLTAGE_REGISTER, 2, &value,1);
	*shuntvoltage = ((value[0]<<8)|value[1])*SHUNT_LSB;  //in mV

	bytesleft = i2c_read_bytes(dev_ptr->bus, dev_ptr->address, INA_220_CURRENT_REGISTER, 2, &value,1);
	*current = ((value[0]<<8)|value[1])*CURRENT_LSB;

	bytesleft = i2c_read_bytes(dev_ptr->bus, dev_ptr->address, INA_220_POWER_REGISTER, 2, &value,1);
	*power = ((value[0]<<8)|value[1])*POWER_LSB;
}
#endif


