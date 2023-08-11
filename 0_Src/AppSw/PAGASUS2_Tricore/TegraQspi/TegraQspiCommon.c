/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#include "TegraQspi.h"
#include "Configuration.h"
#include <Qspi/SpiMaster/IfxQspi_SpiMaster.h>
#include <Qspi/SpiSlave/IfxQspi_SpiSlave.h>


static Ifx_QSPI * qspi_module_ptr[MAX_QSPI_MODULE] =
{
		&MODULE_QSPI0,
		&MODULE_QSPI1,
		&MODULE_QSPI2,
		&MODULE_QSPI3,
		&MODULE_QSPI4,
		&MODULE_QSPI5,
};

static const uint8 qspi_role[MAX_QSPI_MODULE] =
{
		SPI0_ROLE,
		SPI1_ROLE,
		SPI2_ROLE,
		SPI3_ROLE,
		SPI4_ROLE,
		SPI5_ROLE
};

static boolean is_qsip_init[MAX_QSPI_MODULE] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};


Ifx_QSPI * get_spi_module_ptr(uint8 qspi_module_number)
{
	if(qspi_module_number < MAX_QSPI_MODULE)
	{
		return qspi_module_ptr[qspi_module_number];
	}
	else
	{
		return NULL;
	}
}

boolean is_spi_module_enable(uint8 qspi_module_number)
{
	boolean ret = FALSE;
	if(qspi_module_number < MAX_QSPI_MODULE)
	{
		if((qspi_role[qspi_module_number] == SPI_MASTER) ||
		   (qspi_role[qspi_module_number] == SPI_SLAVE))
		{
			ret = TRUE;
		}
	}
	return ret;
}

void set_spi_module_init(uint8 qspi_module_number)
{
	if(qspi_module_number < MAX_QSPI_MODULE)
		is_qsip_init[qspi_module_number] = TRUE;
}

boolean is_spi_module_init(uint8 qspi_module_number)
{
	if(qspi_module_number < MAX_QSPI_MODULE)
		return is_qsip_init[qspi_module_number];
	else
		return FALSE;
}
