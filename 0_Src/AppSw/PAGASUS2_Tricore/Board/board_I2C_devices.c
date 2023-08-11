/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */


#include "string.h"
#include "stdio.h"
#include "board_I2C_devices.h"
#include "board.h"
#include "internal_fw_debug.h"

/* Initial the i2c device address to in-valid value */
t_i2c_device i2c_tmp451_1 = {0, 0xff};
t_i2c_device i2c_tmp451_2 = {0, 0xff};
t_i2c_device i2c_tmp451_3 = {0, 0xff};
t_i2c_device i2c_tmp451_4 = {0, 0xff};
t_i2c_device i2c_tmp451_5 = {0, 0xff};
t_i2c_device i2c_xa_vmon_1 = {0, 0xff};
t_i2c_device i2c_xa_vmon_2 = {0, 0xff};
t_i2c_device i2c_xa_vmon_3 = {0, 0xff};
t_i2c_device i2c_xb_vmon_1 = {0, 0xff};
t_i2c_device i2c_xb_vmon_2 = {0, 0xff};
t_i2c_device i2c_xb_vmon_3 = {0, 0xff};
t_i2c_device i2c_ina_1 = {0, 0xff};
t_i2c_device i2c_ina_2 = {0, 0xff};
t_i2c_device i2c_cim_rom = {0, 0xff};
t_i2c_device i2c_cim_ioexp = {0, 0xff};
t_i2c_device i2c_cim_vmon = {0, 0xff};
t_i2c_device i2c_cim_lsw_svc = {0, 0xff};
t_i2c_device i2c_cim_lsw_cvc = {0, 0xff};
t_i2c_device i2c_cim_lsw_lrc_mrr = {0, 0xff};
t_i2c_device i2c_cim_lsw_lrr = {0, 0xff};

void load_i2c_devices_address(void)
{
	switch(board.project)
	{
	case P3570:
		if(board.aurix_index == MAIN_AURIX)
		{
			//TMP Sensor moved to i2c bus1 in P3570
			i2c_tmp451_1.bus =1;
			i2c_tmp451_1.address = 0x4c;
			i2c_tmp451_2.bus =1;
			i2c_tmp451_2.address = 0x49;
			i2c_tmp451_3.bus =1;
			i2c_tmp451_3.address = 0x4e;

			i2c_xa_vmon_1.bus =0;
			i2c_xa_vmon_1.address = 0x48;
			i2c_xa_vmon_2.bus =0;
			i2c_xa_vmon_2.address = 0x58;
			i2c_xa_vmon_3.bus =0;
			i2c_xa_vmon_3.address = 0x28;

			i2c_xb_vmon_1.bus =1;
			i2c_xb_vmon_1.address = 0x48;
			i2c_xb_vmon_2.bus =1;
			i2c_xb_vmon_2.address = 0x58;
			i2c_xb_vmon_3.bus =1;
			i2c_xb_vmon_3.address = 0x28;

			i2c_ina_1.bus =0;
			i2c_ina_1.address = 0x40;
			i2c_ina_2.bus =0;
			i2c_ina_2.address = 0x41;

			i2c_cim_ioexp.bus =0;
			i2c_cim_ioexp.address = 0x74;
			//i2c_cim_vmon.bus =0;
			//i2c_cim_vmon.address = 0x30;
			//two lsw address are changed in P3570
			i2c_cim_lsw_svc.bus =0;
			i2c_cim_lsw_svc.address = 0x2C;
			i2c_cim_lsw_cvc.bus =0;
			i2c_cim_lsw_cvc.address = 0x2D;
			i2c_cim_lsw_lrc_mrr.bus =0;
			i2c_cim_lsw_lrc_mrr.address = 0x2B;
			i2c_cim_lsw_lrr.bus =0;
			i2c_cim_lsw_lrr.address = 0x2A;
		}
		else
		{
            if(board.pcb_version >= PG2_A02){
                i2c_tmp451_1.bus =1;
                i2c_tmp451_1.address = 0x4c;
                i2c_tmp451_2.bus =1;
                i2c_tmp451_2.address = 0x49;
                i2c_tmp451_3.bus =1;
                i2c_tmp451_3.address = 0x4e;
                i2c_tmp451_4.bus =0;
                i2c_tmp451_4.address = 0x4c;
                i2c_tmp451_5.bus =0;
                i2c_tmp451_5.address = 0x49;
            }
		}
		break;
	case E3550:
	case P3479:
		/* assign the i2c address for E3550 and P3479 together,
		 * even though P3479 has less devices */
		i2c_tmp451_1.bus =0;
		i2c_tmp451_1.address = 0x4c;

		i2c_xa_vmon_1.bus =0;
		i2c_xa_vmon_1.address = 0x48;
		i2c_xa_vmon_2.bus =0;
		i2c_xa_vmon_2.address = 0x58;
		i2c_xa_vmon_3.bus =0;
		i2c_xa_vmon_3.address = 0x28;

		i2c_xb_vmon_1.bus =1;
		i2c_xb_vmon_1.address = 0x48;
		i2c_xb_vmon_2.bus =1;
		i2c_xb_vmon_2.address = 0x58;
		i2c_xb_vmon_3.bus =1;
		i2c_xb_vmon_3.address = 0x28;

		i2c_ina_1.bus =0;
		i2c_ina_1.address = 0x40;
		i2c_ina_2.bus =0;
		i2c_ina_2.address = 0x41;

		i2c_cim_rom.bus =0;
		i2c_cim_rom.address = 0x50;
		i2c_cim_ioexp.bus =0;
		i2c_cim_ioexp.address = 0x74;
		i2c_cim_vmon.bus =0;
		i2c_cim_vmon.address = 0x30;
		i2c_cim_lsw_svc.bus =0;
		i2c_cim_lsw_svc.address = 0x28;
		i2c_cim_lsw_cvc.bus =0;
		i2c_cim_lsw_cvc.address = 0x29;
		i2c_cim_lsw_lrc_mrr.bus =0;
		i2c_cim_lsw_lrc_mrr.address = 0x2A;
		i2c_cim_lsw_lrr.bus =0;
		i2c_cim_lsw_lrr.address = 0x2B;
		break;
	default:
		break;
	}

}
