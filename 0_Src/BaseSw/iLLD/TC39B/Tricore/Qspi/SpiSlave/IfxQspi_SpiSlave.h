/**
 * \file IfxQspi_SpiSlave.h
 * \brief QSPI SPISLAVE details
 * \ingroup IfxLld_Qspi
 *
 * \version iLLD_1_0_1_9_0
 * \copyright Copyright (c) 2018 Infineon Technologies AG. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 *
 * Infineon Technologies AG (Infineon) is supplying this file for use
 * exclusively with Infineon's microcontroller products. This file can be freely
 * distributed within development tools that are supporting such microcontroller
 * products.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * \defgroup IfxLld_Qspi_SpiSlave_Usage How to use the SPI Slave Interface driver?
 * \ingroup IfxLld_Qspi
 *
 * The SPI Slave interface driver provides a default QSPI configuration for a bidirectional serial communication of data words.
 *
 * Data transactions are buffered by the hardware based FIFOs. Incoming and outgoing data is transfered in background from/to the QSPI peripheral by interrupt service handlers, which are part of this driver as well. This allows a nonblocking communication without stalling the thread(s) from where data is sent and received.
 * Optionally Dma can be used for data transfers. Only the interrupt configuration and Module initialisation are different when dma is used.
 *
 * In the following sections it will be described, how to integrate the driver into the application framework.
 *
 * \section IfxLld_Qspi_SpiSlave_Preparation Preparation
 * \subsection IfxLld_Qspi_SpiSlave_Include Include Files
 *
 * Include following header file into your C code:
 * \code
 * #include <Qspi/SpiSlave/IfxQspi_SpiSlave.h>
 * \endcode
 *
 * \subsection IfxLld_Qspi_SpiSlave_Variables Variables
 *
 * Declare QSPI module handle:
 *
 * \code
 * IfxQspi_SpiSlave spi;
 * \endcode
 *
 * In addition, declare global transmit and receive buffers for the data transfers:
 * \code
 * #define SPI_BUFFER_SIZE 8
 * uint8	spiTxBuffer[SPI_BUFFER_SIZE];
 * uint8	spiRxBuffer[SPI_BUFFER_SIZE];
 * \endcode
 *
 * \subsection IfxLld_Qspi_SpiSlave_Interrupt Interrupt Handler Installation (without dma use)
 *
 * See also \ref IfxLld_Cpu_Irq_Usage
 *
 * Define priorities for the Interrrupt handlers. This is normally done in the Ifx_IntPrioDef.h file:
 * \code
 * // priorities are normally defined in Ifx_IntPrioDef.h
 *  #define IFX_INTPRIO_QSPI2_TX  1
 *  #define IFX_INTPRIO_QSPI2_RX  2
 *  #define IFX_INTPRIO_QSPI2_ER  5
 * \endcode
 *
 * Add the interrupt service routines to your C code. They have to call the QSPI interrupt handlers by passing the spi handle:
 * \code
 * IFX_INTERRUPT(qspi2TxISR, 0, IFX_INTPRIO_QSPI2_TX)
 * {
 *     IfxQspi_SpiSlave_isrTransmit(&spi);
 * }
 *
 * IFX_INTERRUPT(qspi2RxISR, 0, IFX_INTPRIO_QSPI2_RX)
 * {
 *     IfxQspi_SpiSlave_isrReceive(&spi);
 * }
 *
 * IFX_INTERRUPT(qspi2ErISR, 0, IFX_INTPRIO_QSPI2_ER)
 * {
 *     IfxQspi_SpiSlave_isrError(&spi);
 *     // Process errors. Eg: parity Error is checked below
 *     if( spi.errorFlags.parityError == 1)
 *     {
 *         // Parity Error
 *     }
 * }
 * \endcode
 *
 * Finally install the interrupt handlers in your initialisation function:
 * \code
 *     // install interrupt handlers
 *     IfxCpu_Irq_installInterruptHandler(&qspi2TxISR, IFX_INTPRIO_QSPI2_TX);
 *     IfxCpu_Irq_installInterruptHandler(&qspi2RxISR, IFX_INTPRIO_QSPI2_RX);
 *     IfxCpu_Irq_installInterruptHandler(&qspi2ErISR, IFX_INTPRIO_QSPI2_ER);
 *     IfxCpu_enableInterrupts();
 * \endcode
 *
 * \subsection IfxLld_Qspi_SpiSlave_Interrupt_dma Interrupt Handler Installation (with dma use)
 *
 * See also \ref IfxLld_Cpu_Irq_Usage
 *
 * Define priorities for the Interrrupt handlers. This is normally done in the Ifx_IntPrioDef.h file:
 * \code
 * // priorities are normally defined in Ifx_IntPrioDef.h
 * // qspi priorities
 * #define IFX_INTPRIO_QSPI2_TX  3 // DMA channel 3
 * #define IFX_INTPRIO_QSPI2_RX  4 // DMA channel 4
 * #define IFX_INTPRIO_QSPI2_ER  0x31
 * // dma priorities
 * #define IFX_INTPRIO_DMA_CH3  12
 * #define IFX_INTPRIO_DMA_CH4  13
 * \endcode
 *
 * Add the interrupt service routines to your C code. They have to call the QSPI interrupt handlers by passing the spi handle:
 * \code
 * IFX_INTERRUPT(qspi2DmaTxISR, 0, IFX_INTPRIO_DMA_CH3)
 * {
 *   IfxQspi_SpiSlave_isrDmaTransmit(&spi);
 *
 * }
 *
 * IFX_INTERRUPT(qspi2DmaRxISR, 0, IFX_INTPRIO_DMA_CH4)
 * {
 *   IfxQspi_SpiSlave_isrDmaReceive(&spi);
 * }
 *
 * IFX_INTERRUPT(qspi2ErISR, 0, IFX_INTPRIO_QSPI2_ER)
 * {
 *     IfxQspi_SpiSlave_isrError(&spi);
 *     // Process errors. Eg: parity Error is checked below
 *     if( spi.errorFlags.parityError == 1)
 *     {
 *         // Parity Error
 *     }
 *
 * }
 * \endcode
 *
 * Finally install the interrupt handlers in your initialisation function:
 * \code
 *     // install interrupt handlers
 *     IfxCpu_Irq_installInterruptHandler(&qspi2DmaTxISR, IFX_INTPRIO_DMA_CH3);
 *     IfxCpu_Irq_installInterruptHandler(&qspi2DmaRxISR, IFX_INTPRIO_DMA_CH4);
 *     IfxCpu_Irq_installInterruptHandler(&qspi2ErISR, IFX_INTPRIO_QSPI2_ER);
 *     IfxCpu_enableInterrupts();
 * \endcode
 *
 * \subsection IfxLld_Qspi_SpiSlave_Init Module Initialisation (without dma use)
 *
 * The module initialisation can be done in the same function.
 *
 * Here an example for slave mode:
 * \code
 *     // create module config
 *     IfxQspi_SpiSlave_Config spiSlaveConfig;
 *     IfxQspi_SpiSlave_initModuleConfig(&spiSlaveConfig, &MODULE_QSPI2);
 *
 *     // set the maximum baudrate
 *     spiSlaveConfig.base.maximumBaudrate  = 10000000;
 *
 *     // ISR priorities and interrupt target
 *     spiSlaveConfig.base.txPriority       = IFX_INTPRIO_QSPI2_TX;
 *     spiSlaveConfig.base.rxPriority       = IFX_INTPRIO_QSPI2_RX;
 *     spiSlaveConfig.base.erPriority       = IFX_INTPRIO_QSPI2_ER;
 *     spiSlaveConfig.base.isrProvider      = IfxCpu_Irq_getTos(IfxCpu_getCoreIndex());
 *
 *     // pin configuration
 *     const IfxQspi_SpiSlave_Pins slavePins = {
 *         &IfxQspi2_SCLKA_P15_3_IN, IfxPort_InputMode_pullDown,   // SCLK Pin
 *         &IfxQspi2_MTSRA_P15_5_IN, IfxPort_InputMode_pullDown,   // MTSR Pin
 *         &IfxQspi2_MRST_P15_7_OUT, IfxPort_OutputMode_pushPull,  // MRST Pin
 *         &IfxQspi2_SLSIA_P15_2_IN, IfxPort_InputMode_pullDown,   // SLSI Pin
 *         IfxPort_PadDriver_cmosAutomotiveSpeed3 // pad driver mode
 *     };
 *     spiSlaveConfig.pins = &slavePins;
 *
 *     // initialize module
 *     //IfxQspi_Spi spi; // defined globally
 *     IfxQspi_SpiSlave_initModule(&spi, &spiSlaveConfig);
 * \endcode
 *
 * \subsection IfxLld_Qspi_SpiSlave_Init_dma Module Initialisation (with dma use)
 *
 * The module initialisation can be done in the same function.
 *
 * Here an example for slave mode:
 * \code
 *     // create module config
 *     IfxQspi_SpiSlave_Config spiSlaveConfig;
 *     IfxQspi_SpiSlave_initModuleConfig(&spiSlaveConfig, &MODULE_QSPI2);
 *
 *     // set the maximum baudrate
 *     spiSlaveConfig.base.maximumBaudrate  = 10000000;
 *
 *     // ISR priorities and interrupt target (with dma usage)
 *     spiSlaveConfig.base.txPriority       = IFX_INTPRIO_DMA_CH3;
 *     spiSlaveConfig.base.rxPriority       = IFX_INTPRIO_DMA_CH4;
 *     spiSlaveConfig.base.erPriority       = IFX_INTPRIO_QSPI2_ER;
 *
 *       spiSlaveConfig.dma.txDmaChannelId = IfxDma_ChannelId_3;
 *        spiSlaveConfig.dma.rxDmaChannelId = IfxDma_ChannelId_4;
 *        spiSlaveConfig.dma.useDma = 1;
 *
 *     // pin configuration
 *     const IfxQspi_SpiSlave_Pins slavePins = {
 *         &IfxQspi2_SCLKA_P15_3_IN, IfxPort_InputMode_pullDown,   // SCLK Pin
 *         &IfxQspi2_MTSRA_P15_5_IN, IfxPort_InputMode_pullDown,   // MTSR Pin
 *         &IfxQspi2_MRST_P15_7_OUT, IfxPort_OutputMode_pushPull,  // MRST Pin
 *         &IfxQspi2_SLSIA_P15_2_IN, IfxPort_InputMode_pullDown,   // SLSI Pin
 *         IfxPort_PadDriver_cmosAutomotiveSpeed3 // pad driver mode
 *     };
 *     spiSlaveConfig.pins = &slavePins;
 *
 *     // initialize module
 *     //IfxQspi_Spi spi; // defined globally
 *     IfxQspi_SpiSlave_initModule(&spi, &spiSlaveConfig);
 * \endcode
 *
 * The QSPI is ready for use now!
 *
 *
 * \section IfxLld_Qspi_SpiSlave_DataTransfers Data Transfers
 *
 * In following examples we assume, that following buffers are declared globally:
 * \code
 * // declared somewhere globally
 * #define SPI_BUFFER_SIZE 8
 * uint8 spiTxBuffer[SPI_BUFFER_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8 };
 * uint8 spiRxBuffer[SPI_BUFFER_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0 };
 * \endcode
 *
 * Sending and Receiving a data stream:
 * \code
 *      int i = 0;
 *     // wait until transfer of previous data stream is finished
 *     while( IfxQspi_SpiSlave_getStatus(&spi) == SpiIf_Status_busy );
 *
 *     // send/receive new stream
 *     IfxQspi_SpiSlave_exchange(&spi, &spiTxBuffer[i], &spiRxBuffer[i], SPI_BUFFER_SIZE);
 * \endcode
 *
 * Send only, discard received data:
 * \code
 *
 *     // wait until transfer of previous data stream is finished
 *     while( IfxQspi_SpiSlave_getStatus(&spi) == SpiIf_Status_busy );
 *
 *     // send new stream
 *     IfxQspi_SpiSlave_exchange(&spi, &spiTxBuffer[i], NULL_PTR, SPI_BUFFER_SIZE);
 * \endcode
 *
 * Receive only, send all-1
 * \code
 *     // wait until transfer of previous data stream is finished
 *     while( IfxQspi_SpiSlave_getStatus(&spi) == SpiIf_Status_busy );
 *
 *     // receive new stream
 *     IfxQspi_SpiSlave_exchange(&spi, NULL_PTR, &spiRxBuffer[i], SPI_BUFFER_SIZE);
 * \endcode
 *
 * \defgroup IfxLld_Qspi_SpiSlave SPI Slave Driver
 * \ingroup IfxLld_Qspi
 * \defgroup IfxLld_Qspi_SpiSlave_DataStructures Data Structures
 * \ingroup IfxLld_Qspi_SpiSlave
 * \defgroup IfxLld_Qspi_SpiSlave_Module Module Functions
 * \ingroup IfxLld_Qspi_SpiSlave
 * \defgroup IfxLld_Qspi_SpiSlave_Support Support Functions
 * \ingroup IfxLld_Qspi_SpiSlave
 * \defgroup IfxLld_Qspi_SpiSlave_Com Communication
 * \ingroup IfxLld_Qspi_SpiSlave
 * \defgroup IfxLld_Qspi_SpiSlave_InterruptFunctions Interrupt Functions
 * \ingroup IfxLld_Qspi_SpiSlave
 * \defgroup IfxLld_Qspi_SpiSlave_DirectFifo Direct FIFO Access
 * \ingroup IfxLld_Qspi_SpiSlave
 */

#ifndef IFXQSPI_SPISLAVE_H
#define IFXQSPI_SPISLAVE_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "Cpu/Irq/IfxCpu_Irq.h"
#include "Dma/Dma/IfxDma_Dma.h"
#include "Qspi/Std/IfxQspi.h"
#include "Scu/Std/IfxScuWdt.h"

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Qspi_SpiSlave_DataStructures
 * \{ */
/** \brief Dma handle
 */
typedef struct
{
    IfxDma_Dma_Channel rxDmaChannel;         /**< \brief receive DMA channel handle */
    IfxDma_Dma_Channel txDmaChannel;         /**< \brief transmit DMA channel handle */
    IfxDma_ChannelId   rxDmaChannelId;       /**< \brief DMA channel no for the Spi recieve */
    IfxDma_ChannelId   txDmaChannelId;       /**< \brief DMA channel no for the Spi transmit */
    boolean            useDma;               /**< \brief use Dma for Data transfers */
} IfxQspi_SpiSlave_Dma;

/** \brief Dma configuration
 */
typedef struct
{
    IfxDma_ChannelId rxDmaChannelId;       /**< \brief DMA channel no for the Spi receive */
    IfxDma_ChannelId txDmaChannelId;       /**< \brief DMA channel no for the Spi transmit */
    boolean          useDma;               /**< \brief use Dma for Data transfers */
} IfxQspi_SpiSlave_DmaConfig;

/** \brief Qspi Slave Mode Error Flags
 */
typedef struct
{
    uint16 parityError : 1;                   /**< \brief [0:0] Parity Error */
    uint16 configurationError : 1;            /**< \brief [1:1] Configuration Error */
    uint16 baudrateError : 1;                 /**< \brief [2:2] baudrate Error */
    uint16 txFifoOverflowError : 1;           /**< \brief [3:3] TxFifo Overflow Error */
    uint16 txFifoUnderflowError : 1;          /**< \brief [4:4] TxFifo underflow Error */
    uint16 rxFifoOverflowError : 1;           /**< \brief [5:5] RxFifo Overflow Error */
    uint16 rxFifoUnderflowError : 1;          /**< \brief [6:6] RxFifo underflow Error */
    uint16 expectTimeoutError : 1;            /**< \brief [7:7] Expect Timeout Error */
    uint16 slsiMisplacedInactivation : 1;     /**< \brief [8:8] SLSI misplaced inactivation (slave mode) */
} IfxQspi_SpiSlave_ErrorFlags;

/** \brief Slave pin IO configuration structure
 */
typedef struct
{
    IFX_CONST IfxQspi_Sclk_In  *sclk;            /**< \brief Pointer to SLCK in pin */
    IfxPort_InputMode           sclkMode;        /**< \brief The SCLK pin input mode */
    IFX_CONST IfxQspi_Mtsr_In  *mtsr;            /**< \brief Pointer to MTSR in pin */
    IfxPort_InputMode           mtsrMode;        /**< \brief The MTSR pin input mode */
    IFX_CONST IfxQspi_Mrst_Out *mrst;            /**< \brief Pointer to MRST out pin */
    IfxPort_OutputMode          mrstMode;        /**< \brief The MRST pin output mode */
    IFX_CONST IfxQspi_Slsi_In  *slsi;            /**< \brief Pointer to SLSI in pin */
    IfxPort_InputMode           slsiMode;        /**< \brief The SLSI pin input mode */
    IfxPort_PadDriver           pinDriver;       /**< \brief The pad driver mode which should be configured */
} IfxQspi_SpiSlave_Pins;

/** \brief Configures the SPI Protocol
 */
typedef struct
{
    SpiIf_ClockPolarity clockPolarity;       /**< \brief Specifies the clock polarity */
    SpiIf_ShiftClock    shiftClock;          /**< \brief Specifies the clock phase */
    SpiIf_DataHeading   dataHeading;         /**< \brief Specifies MSB or LSB first */
    uint8               dataWidth;           /**< \brief range 2 .. 32 bits (note 2 = 2-bits, 3 = 3-bits ... */
    Ifx_ParityMode      parityMode;          /**< \brief Specifies the parity mode */
} IfxQspi_SpiSlave_Protocol;

/** \} */

/** \addtogroup IfxLld_Qspi_SpiSlave_DataStructures
 * \{ */
/** \brief Module handle data structure
 */
typedef struct
{
    SpiIf                       base;             /**< \brief Module SPI interface handle */
    Ifx_QSPI                   *qspi;             /**< \brief Pointer to QSPI module registers */
    uint8                       dataWidth;        /**< \brief Number of bits which will be written into the FIFO */
    SpiIf_Job                   rxJob;            /**< \brief Rx Stream which has been received */
    SpiIf_Job                   txJob;            /**< \brief Tx Stream which should be sent */
    boolean                     onTransfer;       /**< \brief set to TRUE during ongoing transfer */
    IfxQspi_SpiSlave_Dma        dma;              /**< \brief Dma handle */
    IfxQspi_SpiSlave_ErrorFlags errorFlags;       /**< \brief Spi Slave Error Flags */
} IfxQspi_SpiSlave;

/** \brief Module configuration structure
 */
typedef struct
{
    SpiIf_Config                     base;                             /**< \brief SPI interface configuration structure */
    Ifx_QSPI                        *qspi;                             /**< \brief Pointer to QSPI module registers */
    boolean                          allowSleepMode;                   /**< \brief Specifies module sleep mode */
    boolean                          pauseOnBaudrateSpikeErrors;       /**< \brief Specifies module pause on baudrate or spike errors */
    IfxQspi_PauseRunTransition       pauseRunTransition;               /**< \brief Specifies module run or pause mode */
    IfxQspi_TxFifoInt                txFifoThreshold;                  /**< \brief Specifies the TXFIFO interrupt threshold */
    IfxQspi_RxFifoInt                rxFifoThreshold;                  /**< \brief Specifies the RXFIFO interrupt threshold */
    IFX_CONST IfxQspi_SpiSlave_Pins *pins;                             /**< \brief structure for QSPI Slave pins */
    IfxQspi_SpiSlave_Protocol        protocol;
    IfxQspi_SpiSlave_DmaConfig       dma;                              /**< \brief Dma configuration */
    IfxQspi_FifoMode                 txFifoMode;                       /**< \brief Specifies the transfer FIFO mode. */
    IfxQspi_FifoMode                 rxFifoMode;                       /**< \brief Specifies the receive FIFO mode */
} IfxQspi_SpiSlave_Config;

/** \} */

/** \addtogroup IfxLld_Qspi_SpiSlave_Module
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Initialises the module
 * \param handle Module handle
 * \param config Predefined configuration structure of the module
 * \return None
 *
 * Usage example: see \ref IfxLld_Qspi_SpiSlave_Usage
 *
 */
IFX_EXTERN void IfxQspi_SpiSlave_initModule(IfxQspi_SpiSlave *handle, const IfxQspi_SpiSlave_Config *config);

/** \brief Fills the config structure with default values
 * \param config Configuration structure which should be initialized.
 * \param qspi pointer to QSPI registers
 * \return None
 *
 * Usage example: see \ref IfxLld_Qspi_SpiSlave_Usage
 *
 */
IFX_EXTERN void IfxQspi_SpiSlave_initModuleConfig(IfxQspi_SpiSlave_Config *config, Ifx_QSPI *qspi);

/** \} */

/** \addtogroup IfxLld_Qspi_SpiSlave_Com
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Exchanges data between source and data
 * \param handle Module handle
 * \param src Source of data. Can be set to NULL_PTR if nothing to receive (transmit only)
 * \param dest Destination to which to be sent. Can be set to NULL_PTR if nothing to transmit (receive only) - in this case, all-1 will be sent.
 * \param count Number of data in pending
 * \return Status of exchange of data
 *
 * Usage example: see \ref IfxLld_Qspi_SpiSlave_Usage
 *
 */
IFX_EXTERN SpiIf_Status IfxQspi_SpiSlave_exchange(IfxQspi_SpiSlave *handle, const void *src, void *dest, Ifx_SizeT count);

/** \brief Gets the transmission status
 * \param handle Module handle
 * \return Transmission status
 *
 * Usage example: see \ref IfxLld_Qspi_SpiSlave_Usage
 *
 */
IFX_EXTERN SpiIf_Status IfxQspi_SpiSlave_getStatus(IfxQspi_SpiSlave *handle);

/** \} */

/** \addtogroup IfxLld_Qspi_SpiSlave_InterruptFunctions
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Dma receive interrupt handler
 * \param qspiHandle Module handle
 * \return None
 */
IFX_EXTERN void IfxQspi_SpiSlave_isrDmaReceive(IfxQspi_SpiSlave *qspiHandle);

/** \brief Transmit interrupt handler
 * \param qspiHandle Module handle
 * \return None
 */
IFX_EXTERN void IfxQspi_SpiSlave_isrDmaTransmit(IfxQspi_SpiSlave *qspiHandle);

/** \brief Error Interrupt handler
 * \param handle Module handle
 * \return None
 */
IFX_EXTERN void IfxQspi_SpiSlave_isrError(IfxQspi_SpiSlave *handle);

/** \brief Receive Interrupt handler
 * \param handle Module handle
 * \return None
 */
IFX_EXTERN void IfxQspi_SpiSlave_isrReceive(IfxQspi_SpiSlave *handle);

/** \brief Transmit interrupt handler
 * \param handle Module handle
 * \return None
 */
IFX_EXTERN void IfxQspi_SpiSlave_isrTransmit(IfxQspi_SpiSlave *handle);

/** \} */

/** \addtogroup IfxLld_Qspi_SpiSlave_DirectFifo
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Reads data or status in RxFIFO
 * \param handle QSpi Slave handle
 * \return Data or Status in RxFIFO
 */
IFX_INLINE uint32 IfxQspi_SpiSlave_readReceiveFifo(IfxQspi_SpiSlave *handle);

/** \brief Writes the data to TxFIFO
 * \param handle QSpi slave handle
 * \param data Data to be entered in Tx FIFO
 * \return None
 */
IFX_INLINE void IfxQspi_SpiSlave_writeTransmitFifo(IfxQspi_SpiSlave *handle, uint32 data);

/** \} */

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE uint32 IfxQspi_SpiSlave_readReceiveFifo(IfxQspi_SpiSlave *handle)
{
    Ifx_QSPI *qspiSFR = handle->qspi;
    uint32    data    = IfxQspi_readReceiveFifo(qspiSFR);
    return data;
}


IFX_INLINE void IfxQspi_SpiSlave_writeTransmitFifo(IfxQspi_SpiSlave *handle, uint32 data)
{
    IfxQspi_writeTransmitFifo(handle->qspi, data);
}


#endif /* IFXQSPI_SPISLAVE_H */
