/**
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
#include "Configuration.h"
#include <Qspi/SpiMaster/IfxQspi_SpiMaster.h>
#include <Qspi/SpiSlave/IfxQspi_SpiSlave.h>
#include "TegraQspiCommon.h"
#include <internal_fw_debug.h>
#include "board.h"
#include "Aurix2QspiSlave.h"

#define SPI_DEBUG_ENABLE 0

#if SPI_DEBUG_ENABLE == 1
#define SPI_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define SPI_DEBUG_PRINTF(x)
#endif

#define SPI_ALWAYS_PRINTF(x)  DEBUG_PRINTF_ALWAYS(x)


#if PROJECT == PROJECT_PAGASUS2
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define QSPI_MAX_BAUDRATE          50000000  // maximum baudrate which is possible to get a small time quantum
// DMA confiuration paramaters - Disabled
// uncomment line for using DMA and define the DMA channels for this QSPIX, say QSPI0/1/2/3/4
//#define QSPIX_USE_DMA
//#define DMA_CH_QSPIX_TX
//#define DMA_CH_QSPIX_RX
//#define QSPIX_TRANSMIT_CALLBACK
//#define QSPIX_RECEIVE_CALLBACK

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/
typedef struct
{
    uint32 spiTxBuffer[QSPI_BUFFER_SIZE];                               /**< \brief Qspi Transmit buffer, 16 bit datawidth need 16 bit buffer */
    uint32 spiRxBuffer[QSPI_BUFFER_SIZE];                               /**< \brief Qspi receive buffer, 16 bit datawidth need 16 bit buffer */
} AppQspi_Buffer;

/** \brief QspiCpu global data */
typedef struct
{
    AppQspi_Buffer qspiBuffer;                              /**< \brief Qspi buffer */
    struct
    {
        IfxQspi_SpiMaster         spiMaster;                /**< \brief Spi Master handle */
        IfxQspi_SpiMaster_Channel spiMasterChannel;      /**< \brief Spi Master Channel handle */
    }drivers;
}  App_Qspi_Cpu;

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

/* allocate enough space for all QSPI modules */
App_Qspi_Cpu g_Qspi_Cpu[MAX_QSPI_MODULE];

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
#ifdef QSPI0_TRANSMIT_CALLBACK
    extern void QSPI0_TRANSMIT_CALLBACK(void);
#endif
#ifdef QSPI0_RECEIVE_CALLBACK
    extern void QSPI0_RECEIVE_CALLBACK(void);
#endif

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/
/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
#if (SPI0_ROLE == SPI_MASTER) || (SPI0_ROLE == SPI_SLAVE)
IFX_INTERRUPT(ISR_qspi_0_Tx, 0, ISR_PRIORITY_QSPI_0_TX);
IFX_INTERRUPT(ISR_qspi_0_Rx, 0, ISR_PRIORITY_QSPI_0_RX);
IFX_INTERRUPT(ISR_qspi_0_Er, 0, ISR_PRIORITY_QSPI_0_ER);
#endif

#if (SPI1_ROLE == SPI_MASTER) || (SPI1_ROLE == SPI_SLAVE)
IFX_INTERRUPT(ISR_qspi_1_Tx, 0, ISR_PRIORITY_QSPI_1_TX);
IFX_INTERRUPT(ISR_qspi_1_Rx, 0, ISR_PRIORITY_QSPI_1_RX);
IFX_INTERRUPT(ISR_qspi_1_Er, 0, ISR_PRIORITY_QSPI_1_ER);
#endif

#if (SPI2_ROLE == SPI_MASTER) || (SPI2_ROLE == SPI_SLAVE)
IFX_INTERRUPT(ISR_qspi_2_Tx, 0, ISR_PRIORITY_QSPI_2_TX);
IFX_INTERRUPT(ISR_qspi_2_Rx, 0, ISR_PRIORITY_QSPI_2_RX);
IFX_INTERRUPT(ISR_qspi_2_Er, 0, ISR_PRIORITY_QSPI_2_ER);
#endif

#if (SPI3_ROLE == SPI_MASTER) || (SPI3_ROLE == SPI_SLAVE)
IFX_INTERRUPT(ISR_qspi_3_Tx, 0, ISR_PRIORITY_QSPI_3_TX);
IFX_INTERRUPT(ISR_qspi_3_Rx, 0, ISR_PRIORITY_QSPI_3_RX);
IFX_INTERRUPT(ISR_qspi_3_Er, 0, ISR_PRIORITY_QSPI_3_ER);
#endif

#if (SPI4_ROLE == SPI_MASTER) || (SPI4_ROLE == SPI_SLAVE)
IFX_INTERRUPT(ISR_qspi_4_Tx, 0, ISR_PRIORITY_QSPI_4_TX);
IFX_INTERRUPT(ISR_qspi_4_Rx, 0, ISR_PRIORITY_QSPI_4_RX);
IFX_INTERRUPT(ISR_qspi_4_Er, 0, ISR_PRIORITY_QSPI_4_ER);
#endif

#if (SPI5_ROLE == SPI_MASTER) || (SPI5_ROLE == SPI_SLAVE)
IFX_INTERRUPT(ISR_qspi_5_Tx, 0, ISR_PRIORITY_QSPI_5_TX);
IFX_INTERRUPT(ISR_qspi_5_Rx, 0, ISR_PRIORITY_QSPI_5_RX);
IFX_INTERRUPT(ISR_qspi_5_Er, 0, ISR_PRIORITY_QSPI_5_ER);
#endif

#if (SPI0_ROLE == SPI_MASTER) || (SPI0_ROLE == SPI_SLAVE)
/** \brief Handle qspi0_Tx interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_QSPI_TEGRA
 * \isrPriority \ref ISR_PRIORITY_QSPI_TEGRA
 *
 */
void ISR_qspi_0_Tx(void)
{
    IfxCpu_enableInterrupts();
#ifdef QSPI0_USE_DMA
    IfxQspi_SpiMaster_isrDmaTransmit(&g_Qspi_Cpu[0].drivers.spiMaster);
#else
    IfxQspi_SpiMaster_isrTransmit(&g_Qspi_Cpu[0].drivers.spiMaster);
#endif

#ifdef QSPI0_TRANSMIT_CALLBACK
    QSPI0_TRANSMIT_CALLBACK();
#endif
}

/** \brief Handle qspi_Rx interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_QSPI_TEGRA
 * \isrPriority \ref ISR_PRIORITY_QSPI_TEGRA
 *
 */
void ISR_qspi_0_Rx(void)
{
    IfxCpu_enableInterrupts();
#ifdef QSPI0_USE_DMA
    IfxQspi_SpiMaster_isrDmaReceive(&g_Qspi_Cpu[0].drivers.spiMaster);
#else
    IfxQspi_SpiMaster_isrReceive(&g_Qspi_Cpu[0].drivers.spiMaster);
#endif
#ifdef QSPI0_RECEIVE_CALLBACK
    QSPI0_RECEIVE_CALLBACK();
#endif
}

/** \brief Handle qspi0_Er interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_QSPI_TEGRA
 * \isrPriority \ref ISR_PRIORITY_QSPI_TEGRA
 *
 */
void ISR_qspi_0_Er(void)
{
    IfxCpu_enableInterrupts();
    IfxQspi_SpiMaster_isrError(&g_Qspi_Cpu[0].drivers.spiMaster);
}
#endif


#if (SPI1_ROLE == SPI_MASTER)
/** \brief Handle qspi1_Tx interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_QSPI_TEGRA
 * \isrPriority \ref ISR_PRIORITY_QSPI_TEGRA
 *
 */
void ISR_qspi_1_Tx(void)
{
    IfxCpu_enableInterrupts();
#ifdef QSPI1_USE_DMA
    IfxQspi_SpiMaster_isrDmaTransmit(&g_Qspi_Cpu[1].drivers.spiMaster);
#else
    IfxQspi_SpiMaster_isrTransmit(&g_Qspi_Cpu[1].drivers.spiMaster);
#endif

#ifdef QSPI1_TRANSMIT_CALLBACK
    QSPI1_TRANSMIT_CALLBACK();
#endif
}

/** \brief Handle qspi1_Rx interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_QSPI_TEGRA
 * \isrPriority \ref ISR_PRIORITY_QSPI_TEGRA
 *
 */
void ISR_qspi_1_Rx(void)
{
    IfxCpu_enableInterrupts();
#ifdef QSPI1_USE_DMA
    IfxQspi_SpiMaster_isrDmaReceive(&g_Qspi_Cpu[1].drivers.spiMaster);
#else
    IfxQspi_SpiMaster_isrReceive(&g_Qspi_Cpu[1].drivers.spiMaster);
#endif
#ifdef QSPI1_RECEIVE_CALLBACK
    QSPI1_RECEIVE_CALLBACK();
#endif
}

/** \brief Handle qspi1_Er interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_QSPI_TEGRA
 * \isrPriority \ref ISR_PRIORITY_QSPI_TEGRA
 *
 */
void ISR_qspi_1_Er(void)
{
    IfxCpu_enableInterrupts();
    IfxQspi_SpiMaster_isrError(&g_Qspi_Cpu[1].drivers.spiMaster);
}
#endif


#if (SPI3_ROLE == SPI_MASTER) || (SPI3_ROLE == SPI_SLAVE)
/** \brief Handle qspi3_Tx interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_QSPI_TEGRA
 * \isrPriority \ref ISR_PRIORITY_QSPI_TEGRA
 *
 */
void ISR_qspi_3_Tx(void)
{
    IfxCpu_enableInterrupts();
    if((board.project == P3570) && (board.aurix_index == SECOND_AURIX))
    {
    	IfxQspi_SpiSlave_isrTransmit(&aurix2_slave_spi3);
    }
    else
    {
#ifdef QSPI3_USE_DMA
		IfxQspi_SpiMaster_isrDmaTransmit(&g_Qspi_Cpu[3].drivers.spiMaster);
#else
		IfxQspi_SpiMaster_isrTransmit(&g_Qspi_Cpu[3].drivers.spiMaster);
#endif

#ifdef QSPI3_TRANSMIT_CALLBACK
		QSPI3_TRANSMIT_CALLBACK();
#endif
    }
}

/** \brief Handle qspi3_Rx interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_QSPI_TEGRA
 * \isrPriority \ref ISR_PRIORITY_QSPI_TEGRA
 *
 */
void ISR_qspi_3_Rx(void)
{
    IfxCpu_enableInterrupts();
    if((board.project == P3570) && (board.aurix_index == SECOND_AURIX))
    {
    	IfxQspi_SpiSlave_isrReceive(&aurix2_slave_spi3);
    }
    else
    {
#ifdef QSPI3_USE_DMA
    	IfxQspi_SpiMaster_isrDmaReceive(&g_Qspi_Cpu[3].drivers.spiMaster);
#else
    	IfxQspi_SpiMaster_isrReceive(&g_Qspi_Cpu[3].drivers.spiMaster);
#endif
#ifdef QSPI3_RECEIVE_CALLBACK
    	QSPI3_RECEIVE_CALLBACK();
#endif
    }
}

/** \brief Handle qspi3_Er interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_QSPI_TEGRA
 * \isrPriority \ref ISR_PRIORITY_QSPI_TEGRA
 *
 */
void ISR_qspi_3_Er(void)
{
    IfxCpu_enableInterrupts();
    if((board.project == P3570) && (board.aurix_index == SECOND_AURIX))
    {
    	IfxQspi_SpiSlave_isrError(&aurix2_slave_spi3);
    }
    else
    {
    	IfxQspi_SpiMaster_isrError(&g_Qspi_Cpu[3].drivers.spiMaster);
    }
}
#endif

#if (SPI4_ROLE == SPI_MASTER) || (SPI4_ROLE == SPI_SLAVE)
/** \brief Handle qspi4_Tx interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_QSPI_TEGRA
 * \isrPriority \ref ISR_PRIORITY_QSPI_TEGRA
 *
 */
void ISR_qspi_4_Tx(void)
{
    IfxCpu_enableInterrupts();
#ifdef QSPI4_USE_DMA
    IfxQspi_SpiMaster_isrDmaTransmit(&g_Qspi_Cpu[4].drivers.spiMaster);
#else
    IfxQspi_SpiMaster_isrTransmit(&g_Qspi_Cpu[4].drivers.spiMaster);
#endif

#ifdef QSPI4_TRANSMIT_CALLBACK
    QSPI4_TRANSMIT_CALLBACK();
#endif
}

/** \brief Handle qspi4_Rx interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_QSPI_TEGRA
 * \isrPriority \ref ISR_PRIORITY_QSPI_TEGRA
 *
 */
void ISR_qspi_4_Rx(void)
{
    IfxCpu_enableInterrupts();
#ifdef QSPI4_USE_DMA
    IfxQspi_SpiMaster_isrDmaReceive(&g_Qspi_Cpu[4].drivers.spiMaster);
#else
    IfxQspi_SpiMaster_isrReceive(&g_Qspi_Cpu[4].drivers.spiMaster);
#endif
#ifdef QSPI4_RECEIVE_CALLBACK
    QSPI4_RECEIVE_CALLBACK();
#endif
}

/** \brief Handle qspi4_Er interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_QSPI_TEGRA
 * \isrPriority \ref ISR_PRIORITY_QSPI_TEGRA
 *
 */
void ISR_qspi_4_Er(void)
{
    IfxCpu_enableInterrupts();
    IfxQspi_SpiMaster_isrError(&g_Qspi_Cpu[4].drivers.spiMaster);
}
#endif




/** \brief QSPI initialization
 *
 * This function initializes Qspi0 in master mode and setup the two channels
 */
void TegraQspiMaster_init(uint8 qspi_module_number, uint8 qspi_mode)
{
	Ifx_QSPI * qspi_module;
	IfxQspi_SpiMaster_Pins *qspi_pins;
	IfxQspi_Slso_Out * tegra_chip_select;
	uint8 module_autoCS = 0;

	if(!is_spi_module_enable(qspi_module_number))
	{
		/* if the spi is not enabled, then let's skip it */
		return;
	}

    /* disable interrupts */
    boolean interruptState = IfxCpu_disableInterrupts();

    IfxQspi_SpiMaster_Config        spiMasterConfig;

    /* re-define the module config based on module number */
    qspi_module = get_spi_module_ptr(qspi_module_number);
    qspi_pins = (IfxQspi_SpiMaster_Pins *) qspi_pins_ptr[qspi_module_number];
    tegra_chip_select = (IfxQspi_Slso_Out *)cs_pins_ptr[qspi_module_number];
    module_autoCS = is_cs_pin_automatic[qspi_module_number] ? 1 : 0;

   	IfxQspi_SpiMaster_initModuleConfig(&spiMasterConfig, qspi_module);
    spiMasterConfig.pins = qspi_pins;
    /* set the maximum baudrate */
    spiMasterConfig.base.maximumBaudrate = QSPI_MAX_BAUDRATE;

    /* ISR priorities and interrupt target */
    switch (qspi_module_number)
    {
    case 0:
        spiMasterConfig.base.txPriority  = ISR_PRIORITY_QSPI_0_TX;
        spiMasterConfig.base.rxPriority  = ISR_PRIORITY_QSPI_0_RX;
        spiMasterConfig.base.erPriority  = ISR_PRIORITY_QSPI_0_ER;
        break;
    case 1:
        spiMasterConfig.base.txPriority  = ISR_PRIORITY_QSPI_1_TX;
        spiMasterConfig.base.rxPriority  = ISR_PRIORITY_QSPI_1_RX;
        spiMasterConfig.base.erPriority  = ISR_PRIORITY_QSPI_1_ER;
        break;
    case 2:
        spiMasterConfig.base.txPriority  = ISR_PRIORITY_QSPI2_TX;
        spiMasterConfig.base.rxPriority  = ISR_PRIORITY_QSPI2_RX;
        spiMasterConfig.base.erPriority  = ISR_PRIORITY_QSPI2_ER;
        break;
    case 3:
        spiMasterConfig.base.txPriority  = ISR_PRIORITY_QSPI_3_TX;
        spiMasterConfig.base.rxPriority  = ISR_PRIORITY_QSPI_3_RX;
        spiMasterConfig.base.erPriority  = ISR_PRIORITY_QSPI_3_ER;
        break;
    case 4:
        spiMasterConfig.base.txPriority  = ISR_PRIORITY_QSPI_4_TX;
        spiMasterConfig.base.rxPriority  = ISR_PRIORITY_QSPI_4_RX;
        spiMasterConfig.base.erPriority  = ISR_PRIORITY_QSPI_4_ER;
        break;
    default:
        spiMasterConfig.base.txPriority  = ISR_PRIORITY_QSPI_0_TX;
        spiMasterConfig.base.rxPriority  = ISR_PRIORITY_QSPI_0_RX;
        spiMasterConfig.base.erPriority  = ISR_PRIORITY_QSPI_0_ER;
        break;
    }

    spiMasterConfig.base.isrProvider = ISR_PROVIDER_QSPI_TEGRA;

#ifdef QSPI0_USE_DMA
    // DMA configuration
    spiMasterConfig.dma.txDmaChannelId = DMA_CH_QSPI0_TX;
    spiMasterConfig.dma.rxDmaChannelId = DMA_CH_QSPI0_RX;
    spiMasterConfig.dma.useDma = 1;
#endif


    /* initialize module */
    IfxQspi_SpiMaster_initModule(&g_Qspi_Cpu[qspi_module_number].drivers.spiMaster, &spiMasterConfig);
    /* set the MRST_input also to the selected pad driver mode if needed */
    if (qspi_pins->mrst != NULL_PTR)
    	IfxPort_setPinPadDriver(qspi_pins->mrst->pin.port, qspi_pins->mrst->pin.pinIndex, qspi_pins->pinDriver);

    IfxQspi_SpiMaster_ChannelConfig spiMasterChannelConfig;

    // setup channel for Tegra
    {
        /* create channel config */
#if defined(__DCC__)
   		// bug on DCC not all bits in mode are cleared
   		memset(&spiMasterChannelConfig, 0, sizeof(spiMasterChannelConfig));
#endif
        IfxQspi_SpiMaster_initChannelConfig(&spiMasterChannelConfig,
            &g_Qspi_Cpu[qspi_module_number].drivers.spiMaster);

        /* set the baudrate for this channel */
        spiMasterChannelConfig.base.baudrate = 5000000;

        /* set the transfer data width */
        spiMasterChannelConfig.base.mode.dataWidth = SPI_BIT_WIDTH[qspi_module_number];

        spiMasterChannelConfig.base.mode.csTrailDelay = 2;
        spiMasterChannelConfig.base.mode.csInactiveDelay = 2;
        switch(qspi_mode)
        {
        case 0:
            spiMasterChannelConfig.base.mode.clockPolarity = SpiIf_ClockPolarity_idleLow;
            spiMasterChannelConfig.base.mode.shiftClock = SpiIf_ShiftClock_shiftTransmitDataOnTrailingEdge;
            break;

        case 1:
            spiMasterChannelConfig.base.mode.clockPolarity = SpiIf_ClockPolarity_idleLow;
            spiMasterChannelConfig.base.mode.shiftClock = SpiIf_ShiftClock_shiftTransmitDataOnLeadingEdge;
            break;

        case 2:
            spiMasterChannelConfig.base.mode.clockPolarity = SpiIf_ClockPolarity_idleHigh;
            spiMasterChannelConfig.base.mode.shiftClock = SpiIf_ShiftClock_shiftTransmitDataOnTrailingEdge;
            break;

        case 3:
            spiMasterChannelConfig.base.mode.clockPolarity = SpiIf_ClockPolarity_idleHigh;
            spiMasterChannelConfig.base.mode.shiftClock = SpiIf_ShiftClock_shiftTransmitDataOnLeadingEdge;
            break;
        default:
            spiMasterChannelConfig.base.mode.clockPolarity = SpiIf_ClockPolarity_idleLow;
            spiMasterChannelConfig.base.mode.shiftClock = SpiIf_ShiftClock_shiftTransmitDataOnLeadingEdge;
            break;
        }

        spiMasterChannelConfig.base.mode.csLeadDelay = SpiIf_SlsoTiming_4;

        spiMasterChannelConfig.base.mode.autoCS = module_autoCS;

        const IfxQspi_SpiMaster_Output slsOutput = {tegra_chip_select,
                                                    IfxPort_OutputMode_pushPull,
                                                    IfxPort_PadDriver_cmosAutomotiveSpeed1};

        spiMasterChannelConfig.sls.output.pin    = slsOutput.pin;
        spiMasterChannelConfig.sls.output.mode   = slsOutput.mode;
        spiMasterChannelConfig.sls.output.driver = slsOutput.driver;

        /* initialize channel */
        IfxQspi_SpiMaster_initChannel(&g_Qspi_Cpu[qspi_module_number].drivers.spiMasterChannel,
            &spiMasterChannelConfig);
    }

    /* init tx buffer area */
	g_Qspi_Cpu[qspi_module_number].qspiBuffer.spiTxBuffer[0] = 0;
    g_Qspi_Cpu[qspi_module_number].qspiBuffer.spiRxBuffer[0] = 0;

    /* enable interrupts again */
    IfxCpu_restoreInterrupts(interruptState);

    set_spi_module_init(qspi_module_number);
}

uint32 TegraQspiMaster_read_write(uint32 slvNr, uint32 count, uint32 *pSendData, uint32 *pRecData)
{
    uint32 uiCnt;

    if (slvNr >= MAX_QSPI_MODULE) return 0x1;
    if (count > QSPI_BUFFER_SIZE) return 0x2;
    if(!is_spi_module_init(slvNr))
    {/* If spi is not initial, we print out error and return */
    	SPI_ALWAYS_PRINTF(("!!\r\nERROR: SPI is not initial, run command \"tegrawr\" to initial it before using!!\r\n"));
    	return 0x3;
    }

    for (uiCnt=0; uiCnt<count; uiCnt++)	g_Qspi_Cpu[slvNr].qspiBuffer.spiTxBuffer[uiCnt] = *pSendData++;

    while (IfxQspi_SpiMaster_getStatus(&g_Qspi_Cpu[slvNr].drivers.spiMasterChannel) == SpiIf_Status_busy)  {};

    IfxQspi_SpiMaster_exchange(&g_Qspi_Cpu[slvNr].drivers.spiMasterChannel, &g_Qspi_Cpu[slvNr].qspiBuffer.spiTxBuffer[0],
        &g_Qspi_Cpu[slvNr].qspiBuffer.spiRxBuffer[0], count);

    /* we wait until our values are read from Qspi */
    while (IfxQspi_SpiMaster_getStatus(&g_Qspi_Cpu[slvNr].drivers.spiMasterChannel) == SpiIf_Status_busy)  {};

    for (uiCnt=0; uiCnt<count; uiCnt++)	*pRecData++ = g_Qspi_Cpu[slvNr].qspiBuffer.spiRxBuffer[uiCnt];

    return 0;

}
#endif
