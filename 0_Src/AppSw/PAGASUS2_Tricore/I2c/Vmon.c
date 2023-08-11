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
#include "Vmon.h"
#include "I2c.h"
#include "SysSe/Bsp/Bsp.h"
#include "SysSe/Comm/Ifx_Shell.h"
#include <Configuration.h>
#include "Gpio.h"
#include "board.h"
#include "internal_fw_debug.h"
#include "board_I2C_devices.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define VMON_DEBUG_ENABLE 1

#if VMON_DEBUG_ENABLE == 1
#define VMON_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define VMON_DEBUG_PRINTF(x)
#endif

#define VMON_ALWAYS_PRINTF(x)  DEBUG_PRINTF_ALWAYS(x)

#define XA_VMON_CTRL	AURIX_XA_RST_N
#define XB_VMON_CTRL	AURIX_XB_RST_N

#define AURIX_XA_VMON_ACCESS	(!IfxPort_getPinState(XA_VMON_CTRL.port, XA_VMON_CTRL.pinIndex))
#define AURIX_XB_VMON_ACCESS	(!IfxPort_getPinState(XB_VMON_CTRL.port, XB_VMON_CTRL.pinIndex))

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/
typedef struct
{
        uint8 bus;
        uint8 i2c_addr;
        uint8 cid;
        uint8 expected_cid;
}t_vmon;

/* Define the table, i2c address will be updated based on board during run time */
static t_vmon VMON_TABLE[]=
{
	{0, 0xff, 0, VMON1_CID},
	{0, 0xff, 0, VMON2_CID},
	{0, 0xff, 0, VMON3_CID},

	{0, 0xff, 0, VMON1_CID},
	{0, 0xff, 0, VMON2_CID},
	{0, 0xff, 0, VMON3_CID},
};

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/
static uint8 vmon_count = 0;

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

/** \brief check if VMON present at address and has the correct CID
 *	takes a pointer to the t_vmon struct
 *	returns TRUE if the device is present and has valid CID, FALSE otherwise
 */
boolean Vmon_checkDevice(t_vmon* vmon_p)
{
	uint32 bytesleft;
	if(!i2c_check_device(vmon_p->bus, vmon_p->i2c_addr))
	{
		VMON_ALWAYS_PRINTF(("VMON not found on BUS: %d, ADDR: 0x%02x !!!! \n",
				vmon_p->bus, vmon_p->i2c_addr));
		return FALSE;
	}
	bytesleft = i2c_read_bytes(vmon_p->bus, vmon_p->i2c_addr, VMON_CID_REG, 1, &(vmon_p->cid), 1);
	if(vmon_p->cid != vmon_p->expected_cid)
	{
		VMON_ALWAYS_PRINTF(("CID Mismatch !!! on BUS: %d, ADDR: 0x%02x, Expected: 0x%02x, Read Back: 0x%02x\n",
				vmon_p->bus, vmon_p->i2c_addr, vmon_p->expected_cid, vmon_p->cid));
		return FALSE;
	}
	return TRUE;
}

static void load_vmon_i2c_address(void)
{
	VMON_TABLE[0].bus = i2c_xa_vmon_1.bus;
	VMON_TABLE[0].i2c_addr = i2c_xa_vmon_1.address;
	VMON_TABLE[1].bus = i2c_xa_vmon_2.bus;
	VMON_TABLE[1].i2c_addr = i2c_xa_vmon_2.address;
	VMON_TABLE[2].bus = i2c_xa_vmon_3.bus;
	VMON_TABLE[2].i2c_addr = i2c_xa_vmon_3.address;

	VMON_TABLE[3].bus = i2c_xb_vmon_1.bus;
	VMON_TABLE[3].i2c_addr = i2c_xb_vmon_1.address;
	VMON_TABLE[4].bus = i2c_xb_vmon_2.bus;
	VMON_TABLE[4].i2c_addr = i2c_xb_vmon_2.address;
	VMON_TABLE[5].bus = i2c_xb_vmon_3.bus;
	VMON_TABLE[5].i2c_addr = i2c_xb_vmon_3.address;
}

/** \brief VMON initialization
 *
 * TODO: This function initializes the VMONs
 * For Bringup, just verify and print the CIDs of the VMONs
 */
boolean Vmon_init(void)
{
	uint8 i;
	boolean check_cid = TRUE;

	load_vmon_i2c_address();

#if 0
	if(board.project == P3570)
		vmon_count = sizeof(VMON_TABLE)/sizeof(VMON_TABLE[0]);
	else if(board.project == P3479)
		vmon_count = sizeof(VMON_TABLE)/sizeof(VMON_TABLE[0]) / 2;
#else
vmon_count = sizeof(VMON_TABLE)/sizeof(VMON_TABLE[0]);
#endif
	for(i=0; i<vmon_count; i++)
	{
		check_cid &= Vmon_checkDevice(&VMON_TABLE[i]);
	}
	return check_cid;
}

/** \brief Handle the 'vmonstatus' command
 *
 * \par Syntax
 *  - vmonstatus : display the status of VMONs
 */
boolean Vmon_showStatus(pchar args, void *data, IfxStdIf_DPipe *io)
{
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
	{
		IfxStdIf_DPipe_print(io, "Syntax    : vmonstatus"ENDL);
		IfxStdIf_DPipe_print(io, "           > print the status of VMONs"ENDL);
	}
	else
	{
		uint8 i;
		if(vmon_count == 0)
		{
			IfxStdIf_DPipe_print(io, "No VMONs found for board!!"ENDL);
			return TRUE;
		}
        IfxStdIf_DPipe_print(io, "vmonstatus test"ENDL);
		if(AURIX_XA_VMON_ACCESS)
			IfxStdIf_DPipe_print(io, "Aurix has XA VMON access"ENDL);
		else
			IfxStdIf_DPipe_print(io, "Xavier has XA VMON access"ENDL);

		if(board.project == P3570)
		{
			if(AURIX_XB_VMON_ACCESS)
				IfxStdIf_DPipe_print(io, "Aurix has XB VMON access"ENDL);
			else
				IfxStdIf_DPipe_print(io, "Xavier has XB VMON access"ENDL);
		}

		for(i=0; i<vmon_count; i++)
		{
			IfxStdIf_DPipe_print(io, "BUS: %d, ADDR: 0x%02x, Expected CID: 0x%02x, Read Back CID: 0x%02x"ENDL,
					VMON_TABLE[i].bus, VMON_TABLE[i].i2c_addr, VMON_TABLE[i].expected_cid, VMON_TABLE[i].cid);
		}
	}
	return TRUE;
}
