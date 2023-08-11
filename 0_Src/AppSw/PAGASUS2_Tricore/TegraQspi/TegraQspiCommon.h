/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

Ifx_QSPI * get_spi_module_ptr(uint8 qspi_module_number);
boolean is_spi_module_enable(uint8 qspi_module_number);
void set_spi_module_init(uint8 qspi_module_number);
boolean is_spi_module_init(uint8 qspi_module_number);
