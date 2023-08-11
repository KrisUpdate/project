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

#include "TegraQspi.h"
#include "TegraQspiCommon.h"
#include "Aurix2QspiSlave.h"
#include <internal_fw_debug.h>
#include "string.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#if SPI_SLV_DEBUG_ENABLE == 1
#define SPI_SLV_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define SPI_SLV_DEBUG_PRINTF(x)
#endif

#define SPI_SLV_PRINTF_ALWAYS(x) DEBUG_PRINTF_ALWAYS(x)

#define QSPI_MAX_BAUDRATE          50000000  // maximum baudrate which is possible to get a small time quantum

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
#if ISR_PROVIDER_QSPI_TEGRA == 0
	#if defined(__GNUC__)
	#pragma section ".bss_cpu0" awc0
	#endif
	#if defined(__TASKING__)
	#pragma section farbss "bss_cpu0"
	#pragma section fardata "data_cpu0"
	#endif
	#if defined(__DCC__)
	#pragma section DATA ".data_cpu0" ".bss_cpu0" far-absolute RW
	#endif
#elif ISR_PROVIDER_QSPI_TEGRA == 1
	#if defined(__GNUC__)
	#pragma section ".bss_cpu1" awc1
	#endif
	#if defined(__TASKING__)
	#pragma section farbss "bss_cpu1"
	#pragma section fardata "data_cpu1"
	#endif
	#if defined(__DCC__)
	#pragma section DATA ".data_cpu1" ".bss_cpu1" far-absolute RW
	#endif
#elif ISR_PROVIDER_QSPI_TEGRA == 2
	#if defined(__GNUC__)
	#pragma section ".bss_cpu2" awc2
	#endif
	#if defined(__TASKING__)
	#pragma section farbss "bss_cpu2"
	#pragma section fardata "data_cpu2"
	#endif
	#if defined(__DCC__)
	#pragma section DATA ".data_cpu2" ".bss_cpu2" far-absolute RW
	#endif
#else
#error "Set ISR_PROVIDER_QSPI_TEGRA to a valid value!"
#endif

#if defined(__GNUC__)
#pragma section
#endif
#if defined(__TASKING__)
#pragma section farbss restore
#pragma section fardata restore
#endif
#if defined(__DCC__)
#pragma section DATA RW
#endif
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/
/* pin configuration */

// define the QSPI slave module
IfxQspi_SpiSlave aurix2_slave_spi3;

/* the buffer */
#define SPI_BUFFER_SIZE 8
 uint32 spiTxBuffer[SPI_BUFFER_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8 };
 uint32 spiRxBuffer[SPI_BUFFER_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0 };

 static const IfxQspi_SpiSlave_Pins qspi3_slave_pins = {
	&IfxQspi3_SCLKB_P10_8_IN, IfxPort_InputMode_pullDown,   // SCLK Pin
	&IfxQspi3_MTSRB_P10_6_IN, IfxPort_InputMode_pullDown,   // MTSR Pin
	&IfxQspi3_MRST_P10_7_OUT, IfxPort_OutputMode_pushPull,  // MRST Pin
	&IfxQspi3_SLSIA_P02_4_IN, IfxPort_InputMode_pullDown,   // SLSI Pin
	IfxPort_PadDriver_cmosAutomotiveSpeed3 // pad driver mode
};

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

static void FlushRxBuffer(void)
{
	uint8 recFifoLevel = IfxQspi_getReceiveFifoLevel(aurix2_slave_spi3.qspi);
	SPI_SLV_PRINTF_ALWAYS(("fifo level is %d\n", recFifoLevel));
	if(recFifoLevel > 0)
{ 
		IfxQspi_requestReset(aurix2_slave_spi3.qspi, IfxQspi_Reset_stateMachineAndFifo);
		waitTime(10*TimeConst[TIMER_INDEX_1MS]);
		recFifoLevel = IfxQspi_getReceiveFifoLevel(aurix2_slave_spi3.qspi);
		SPI_SLV_PRINTF_ALWAYS(("after flush, fifo level is %d\n", recFifoLevel));
	}
}

void A2_QspiSlave_init(void)
{
	/* disable interrupts */
	boolean interruptState = IfxCpu_disableInterrupts();

	// create module config
	IfxQspi_SpiSlave_Config spiSlaveConfig;
	IfxQspi_SpiSlave_initModuleConfig(&spiSlaveConfig, &MODULE_QSPI3);
	// set the maximum baudrate
	spiSlaveConfig.base.maximumBaudrate  = QSPI_MAX_BAUDRATE;
	// ISR priorities and interrupt target
	spiSlaveConfig.base.txPriority       = ISR_PRIORITY_QSPI_3_TX;
	spiSlaveConfig.base.rxPriority       = ISR_PRIORITY_QSPI_3_RX;
	spiSlaveConfig.base.erPriority       = ISR_PRIORITY_QSPI_3_ER;
	spiSlaveConfig.base.isrProvider      = 0;
	spiSlaveConfig.protocol.dataWidth = SPI_BIT_WIDTH[3];  //16
	spiSlaveConfig.protocol.clockPolarity = SpiIf_ClockPolarity_idleLow;
	spiSlaveConfig.protocol.shiftClock    = SpiIf_ShiftClock_shiftTransmitDataOnLeadingEdge;

	spiSlaveConfig.pins = &qspi3_slave_pins;

	// initialize module
	//IfxQspi_Spi aurix2_slave_spi3; // defined globally
	IfxQspi_SpiSlave_initModule(&aurix2_slave_spi3, &spiSlaveConfig);

	/* enable interrupts again */
	IfxCpu_restoreInterrupts(interruptState);
}

void A2_QspiSlave_loopback(uint16 count)
{
	memset(spiRxBuffer, 0, ARRAY_SIZE(spiRxBuffer));
	SPI_SLV_PRINTF_ALWAYS(("Start SPI Slave loopback test\n"));
	SPI_SLV_PRINTF_ALWAYS(("SPI 4 is config as slave, mode 1, will blocking waiting for the SPI data\n"));
	SPI_SLV_PRINTF_ALWAYS(("After transfer %d bytes, the test will exit\n", count));
	SPI_SLV_PRINTF_ALWAYS(("Please run your mods test 224\n"));

	FlushRxBuffer();

	while(count > 0)
	{
		while( IfxQspi_SpiSlave_getStatus(&aurix2_slave_spi3) == SpiIf_Status_busy );
		IfxQspi_SpiSlave_exchange(&aurix2_slave_spi3, &spiTxBuffer[0], &spiRxBuffer[0], 4);
		while( IfxQspi_SpiSlave_getStatus(&aurix2_slave_spi3) == SpiIf_Status_busy );
		if((spiRxBuffer[0] | spiRxBuffer[1] | spiRxBuffer[2] | spiRxBuffer[3]) != 0)
		{
			memcpy(spiTxBuffer, spiRxBuffer, 4);
		}
		else
		{ /* MODS test will send the dummy byte (0) to read data, we need to ignore receiving these dummy byte */
			count --;
		}
	}
}

boolean A2_QspiSlave_read_write(uint32 count, uint32 *pSendData, uint32 *pRecData)
{
	FlushRxBuffer();
	if(count > SPI_BUFFER_SIZE)
	{
		count = SPI_BUFFER_SIZE;
	}
	memset(spiRxBuffer, 0, sizeof(spiRxBuffer));
	memset(spiTxBuffer, 0, sizeof(spiTxBuffer));
	memcpy(spiTxBuffer, pSendData, count*4);
	while( IfxQspi_SpiSlave_getStatus(&aurix2_slave_spi3) == SpiIf_Status_busy );
	IfxQspi_SpiSlave_exchange(&aurix2_slave_spi3, &spiTxBuffer[0], &spiRxBuffer[0], count);
	while( IfxQspi_SpiSlave_getStatus(&aurix2_slave_spi3) == SpiIf_Status_busy );
	memcpy(pRecData, spiRxBuffer, count*4);
/*
 * In Mods driver, it will send another dummy bytes just for reading
 *
	while( IfxQspi_SpiSlave_getStatus(&aurix2_slave_spi3) == SpiIf_Status_busy );
	IfxQspi_SpiSlave_exchange(&aurix2_slave_spi3, &spiTxBuffer[0], &spiRxBuffer[0], count);
	while( IfxQspi_SpiSlave_getStatus(&aurix2_slave_spi3) == SpiIf_Status_busy );
*/
	return TRUE;
}

