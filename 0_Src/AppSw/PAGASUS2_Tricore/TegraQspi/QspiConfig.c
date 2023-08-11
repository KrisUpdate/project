/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#include "Configuration.h"
#include "TegraQspi.h"
#include <Qspi/SpiMaster/IfxQspi_SpiMaster.h>
#include <Qspi/SpiSlave/IfxQspi_SpiSlave.h>

#if PROJECT == PROJECT_DRIVE_PX2 || (PROJECT == PROJECT_PAGASUS2 && (DRIVE_AX_A00 || DRIVE_AX_A01))
/* pin configuration */
const IfxQspi_SpiMaster_Pins qspi0_pins = {&AURIX_TEGRA_SPI_SCK, IfxPort_OutputMode_pushPull,   /* SCLK */
                                           &AURIX_TEGRA_SPI_MOSI,  IfxPort_OutputMode_pushPull, /* MTSR */
                                           &AURIX_TEGRA_SPI_MISO,  IfxPort_InputMode_pullDown,  /* MRST */
                                           IfxPort_PadDriver_ttlSpeed1		                    /* pad driver mode */
};

const IfxQspi_SpiMaster_Pins qspi1_pins = {&AURIX_TEGRA_B_SPI_SCK, IfxPort_OutputMode_pushPull,   /* SCLK */
                                           &AURIX_TEGRA_B_SPI_MOSI,  IfxPort_OutputMode_pushPull, /* MTSR */
                                           &AURIX_TEGRA_B_SPI_MISO,  IfxPort_InputMode_pullDown,  /* MRST */
                                           IfxPort_PadDriver_ttlSpeed1		                    /* pad driver mode */
};

const void * qspi_pins_ptr[MAX_QSPI_MODULE] =
{
		&qspi0_pins,
		&qspi1_pins,
		NULL,
		NULL,
		NULL,
		NULL,
};

/* CS pin configuration */
/* if we don't use SLSO pin, then we need to config this manually
 * It's a WAR and we should not support this
 * Keep here for reference only */
#ifndef TEGRA_USE_CHIPSELECT
/* our cs is not an SLSO, we disable the automatic, set to selected channel and to selected port pin */
IfxQspi_Slso_Out TEGRA_USE_CHIPSELECT = {&TEGRA_QSPI_MODULE, TEGRA_QSPI_CHANNEL, {&TEGRA_CS_PORT_MODULE, TEGRA_CS_PORT_PIN},  IfxPort_OutputIdx_general};
#define NO_AUTOMATIC_CS_SPI0
#endif
#ifndef TEGRA_B_USE_CHIPSELECT
/* our cs is not an SLSO, we disable the automatic, set to selected channel and to selected port pin */
IfxQspi_Slso_Out TEGRA_B_USE_CHIPSELECT = {&TEGRA_B_QSPI_MODULE, TEGRA_B_QSPI_CHANNEL, {&TEGRA_B_CS_PORT_MODULE, TEGRA_B_CS_PORT_PIN},  IfxPort_OutputIdx_general};
#define NO_AUTOMATIC_CS_B
#endif


const boolean is_cs_pin_automatic[MAX_QSPI_MODULE]=
{
		FALSE,
		FALSE,
		FALSE,
		FALSE,
		FALSE,
		FALSE,
};
const void * cs_pins_ptr[MAX_QSPI_MODULE]=
{
		&AURIX_TEGRA_SPI_CS,
		&AURIX_TEGRA_B_SPI_CS,
		NULL,
		NULL,
		NULL,
		NULL,
};

const uint8 SPI_BIT_WIDTH[MAX_QSPI_MODULE]=
{
		16,
		16,
		16,
		16,
		16,
		16
};


#elif PROJECT == PROJECT_PAGASUS2 && DRIVE_AX_B0x
//Using this section

/* pin configuration */
const IfxQspi_SpiMaster_Pins qspi0_pins = {&AURIX_X1_SPI_SCKR, IfxPort_OutputMode_pushPull,   /* SCLK */
                                           &AURIX_X1_SPI_MOSIR,  IfxPort_OutputMode_pushPull, /* MTSR */
                                           &AURIX_X1_SPI_MISOR,  IfxPort_InputMode_noPullDevice,  /* MRST */
                                           IfxPort_PadDriver_ttlSpeed1		                    /* pad driver mode */
};

const IfxQspi_SpiMaster_Pins qspi1_pins = {&SJA1105_SCK, IfxPort_OutputMode_pushPull,   /* SCLK */
                                           &SJA1105_SDI,  IfxPort_OutputMode_pushPull, /* MTSR */
                                           &SJA1105_SDO,  IfxPort_InputMode_noPullDevice,  /* MRST */
                                           IfxPort_PadDriver_ttlSpeed1		                    /* pad driver mode */
};

const IfxQspi_SpiMaster_Pins qspi3_pins = {&QSPI3_SPI_SCKR, IfxPort_OutputMode_pushPull,   /* SCLK */
                                           &QSPI3_SPI_MOSIR,  IfxPort_OutputMode_pushPull, /* MTSR */
                                           &QSPI3_SPI_MISOR,  IfxPort_InputMode_noPullDevice,  /* MRST */
                                           IfxPort_PadDriver_ttlSpeed1		                    /* pad driver mode */
};

const IfxQspi_SpiMaster_Pins qspi4_pins = {&AURIX_X2_SPI_SCKR, IfxPort_OutputMode_pushPull,   /* SCLK */
                                           &AURIX_X2_SPI_MOSIR,  IfxPort_OutputMode_pushPull, /* MTSR */
                                           &AURIX_X2_SPI_MISOR,  IfxPort_InputMode_noPullDevice,  /* MRST */
                                           IfxPort_PadDriver_ttlSpeed1		                    /* pad driver mode */
};


const void * qspi_pins_ptr[MAX_QSPI_MODULE] =
{
		&qspi0_pins,
		&qspi1_pins,
		NULL,
		&qspi3_pins,
		&qspi4_pins,
		NULL,
};

const boolean is_cs_pin_automatic[MAX_QSPI_MODULE]=
{
		TRUE,
		TRUE,
		FALSE,
		TRUE,
		TRUE,
		FALSE,
};
const void * cs_pins_ptr[MAX_QSPI_MODULE]=
{
		&AURIX_X1_SPI_CSR,
		&SJA1105_CSN,
		NULL,
		&QSPI3_SPI_SSR,
		&AURIX_X2_SPI_SSR,
		NULL,
};

const uint8 SPI_BIT_WIDTH[MAX_QSPI_MODULE]=
{
		16,
		32,   //SJA1105
		16,
		16,
		16,
		16
};



#elif PROJECT == PROJECT_AUTO_CRUISE

/* pin configuration */
const IfxQspi_SpiMaster_Pins qspi0_pins = {&AURIX_TEGRA_SPI_SCK, IfxPort_OutputMode_pushPull,   /* SCLK */
                                           &AURIX_TEGRA_SPI_MOSI,  IfxPort_OutputMode_pushPull, /* MTSR */
                                           &AURIX_TEGRA_SPI_MISO,  IfxPort_InputMode_pullDown,  /* MRST */
                                           IfxPort_PadDriver_ttlSpeed1		                    /* pad driver mode */
};

const IfxQspi_SpiSlave_Pins qspi4_pins = {
	&IfxQspi4_SCLKA_P33_11_IN, IfxPort_InputMode_pullDown,   // SCLK Pin
	&IfxQspi4_MTSRA_P33_12_IN, IfxPort_InputMode_pullDown,   // MTSR Pin
	&IfxQspi4_MRST_P33_13_OUT, IfxPort_OutputMode_pushPull,  // MRST Pin
	&IfxQspi4_SLSIA_P33_10_IN, IfxPort_InputMode_pullDown,   // SLSI Pin
	IfxPort_PadDriver_cmosAutomotiveSpeed3 // pad driver mode
};

const void * qspi_pins_ptr[MAX_QSPI_MODULE] =
{
		&qspi0_pins,
		NULL,
		NULL,
		NULL,
		&qspi4_pins,
		NULL,
};

/* CS pin configuration */
/* if we don't use SLSO pin, then we need to config this manually
 * It's a WAR and we should not support this
 * Keep here for reference only */
#ifndef TEGRA_USE_CHIPSELECT
/* our cs is not an SLSO, we disable the automatic, set to selected channel and to selected port pin */
IfxQspi_Slso_Out TEGRA_USE_CHIPSELECT = {&TEGRA_QSPI_MODULE, TEGRA_QSPI_CHANNEL, {&TEGRA_CS_PORT_MODULE, TEGRA_CS_PORT_PIN},  IfxPort_OutputIdx_general};
#define NO_AUTOMATIC_CS_SPI0
#endif

const boolean is_cs_pin_automatic[MAX_QSPI_MODULE]=
{
		FALSE,
		FALSE,
		FALSE,
		FALSE,
		FALSE,
		FALSE,
};

const void * cs_pins_ptr[MAX_QSPI_MODULE]=
{
		&AURIX_TEGRA_SPI_CS,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
};

const uint8 SPI_BIT_WIDTH[MAX_QSPI_MODULE]=
{
		16,
		16,
		16,
		16,
		16,
		16
};



#endif



