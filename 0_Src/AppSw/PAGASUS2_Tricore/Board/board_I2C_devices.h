/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#ifndef _BOARD_I2C_DEVICES_H
#define _BOARD_I2C_DEVICES_H

#include "Configuration.h"

typedef struct
{
	uint8 bus;
	uint8 address;
}t_i2c_device;

extern t_i2c_device i2c_tmp451_1;
extern t_i2c_device i2c_tmp451_2;
extern t_i2c_device i2c_tmp451_3;
extern t_i2c_device i2c_tmp451_4;
extern t_i2c_device i2c_tmp451_5;
extern t_i2c_device i2c_xa_vmon_1;
extern t_i2c_device i2c_xa_vmon_2;
extern t_i2c_device i2c_xa_vmon_3;
extern t_i2c_device i2c_xb_vmon_1;
extern t_i2c_device i2c_xb_vmon_2;
extern t_i2c_device i2c_xb_vmon_3;
extern t_i2c_device i2c_ina_1;
extern t_i2c_device i2c_ina_2;
extern t_i2c_device i2c_cim_rom;
extern t_i2c_device i2c_cim_ioexp;
extern t_i2c_device i2c_cim_vmon;
extern t_i2c_device i2c_cim_lsw_svc;
extern t_i2c_device i2c_cim_lsw_cvc;
extern t_i2c_device i2c_cim_lsw_lrc_mrr;
extern t_i2c_device i2c_cim_lsw_lrr;

void load_i2c_devices_address(void);

#endif //endof _BOARD_I2C_DEVICES_H
