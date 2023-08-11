/**
 * \file IfxHssl_Hssl.h
 * \brief HSSL HSSL details
 * \ingroup IfxLld_Hssl
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
 *
 * \defgroup IfxLld_Hssl_Hssl_Usage How to use the HSSL Interface driver?
 * \ingroup IfxLld_Hssl
 *
 * The HSSL interface driver provides a default HSSL/HSCT configuration for point to point communication at two transfer speeds, 5MBaud (low speed) and 320MBaud (high speed).
 * It also supports streaming transfers of data a memory block at both low and high speeds.
 *
 * In the following sections it will be described, how to integrate the driver into the application framework.
 *
 * \section IfxLld_Hssl_Hssl_Preparation Preparation
 * \subsection IfxLld_Hssl_Hssl_Include Include Files
 *
 * Include following header file into your C code:
 * \code
 *     #include <Hssl/Hssl/IfxHssl_Hssl.h>
 * \endcode
 *
 * \subsection IfxLld_Hssl_Hssl_Variables Variables
 *
 * Declare the HSSL handle, HSCT handle and channel array as global variables in your C code:
 *
 * \code
 *     // used globally
 *     static IfxHssl_Hssl hssl;
 *     static IfxHssl_Hsct hsct;
 *     IfxHssl_Hssl_Channel hsslChannel[4];
 *     __attribute__ ((aligned(256))) uint32 txData[80]; // needs to be declared globally in case of streaming transfers
 * \endcode
 *
 * \subsection IfxLld_Hssl_Hssl_InitModule Module Initialisation
 *
 * The module initialisation can be done as follows:
 *
 * \code
 *     // create module config
 *     IfxHssl_Hsct_Config hsctConfig;
 *     IfxHssl_Hssl_initHsctModuleConfig(&hsctConfig, &MODULE_HSCT);
 *
 *     // select the interface mode (in case of slave)
 *     hsctConfig.interfaceMode = IfxHssl_InterfaceMode_slave;
 *
 *     // select the high speed mode if required
 *     hsctConfig.highSpeedMode = TRUE;
 *
 *     // initialize module
 *     //IfxHssl_Hsct hsct; // defined globally
 *     IfxHssl_Hssl_initHsctModule(&hsct, &hsctConfig);
 *
 *     // create module config
 *     IfxHssl_Hssl_Config hsslConfig;
 *     IfxHssl_Hssl_initHsslModuleConfig(&hsslConfig, &MODULE_HSSL);
 *
 *     //IfxHssl_Hssl hssl; // defined globally
 *     IfxHssl_Hssl_initHsslModule(&hssl, &hsslConfig);
 * \endcode
 *
 * \subsection IfxLld_Hssl_Hssl_InitChannel Channel Initialisation
 *
 * The Channel initialisation can be done as follows:
 *
 * \code
 *     // create HSSL channel config
 *     IfxHssl_Hssl_ChannelConfig hsslChannelConfig;
 *     IfxHssl_Hssl_initChannelConfig(&hsslChannelConfig, &hssl, &hsct);
 *
 *     // initialize the channels
 *     // IfxHssl_Hssl_Channel hsslChannel[4]; // defined globally
 *     for(int i=0; i<4; ++i)
 *     {
 *         hsslChannelConfig.channelId = (IfxHssl_ChannelId)i;
 *         IfxHssl_Hssl_initChannel(&hsslChannel[i], &hsslChannelConfig);
 *     }
 * \endcode
 *
 * \subsection IfxLld_Hssl_Hssl_LinkSpeed Changing HSCT link speed
 *
 * The link speed change can be done as follows
 *
 * As per the above settings the HSCT master is in High speed reception and transmission.
 *
 * The pseudo code for setting the target to High speed is as below
 *
 * \code
 *
 *      // Set the Tx link speed of the master to Low speed
 *      IfxHssl_setHsctTxLinkSpeed(&hsct , IfxHssl_MasterModeTxSpeed_lowSpeed);
 *
 *      // Enable reception at the Slave
 *      IfxHssl_Hssl_sendControlCommand(&hsct,IfxHssl_ControlCommand_enableReception);
 *
 *      // Send interface commmand to change Rx to High speed at the target
 *      IfxHssl_Hssl_sendControlCommand(&hsct,IfxHssl_ControlCommand_highSpeedReception);
 *
 *      // Send interface commmand to change Tx to High speed at the target
 *      IfxHssl_Hssl_sendControlCommand(&hsct,IfxHssl_ControlCommand_highSpeedTransmission);
 *
 *      // Set the Tx link speed of the master to High speed
 *      IfxHssl_setHsctTxLinkSpeed(&hsct , IfxHssl_MasterModeTxSpeed_highSpeed);
 *
 * \endcode
 *
 * The pseudo code for setitng the target to Low speed from High speed (ie. if hsctConfig.highSpeedMode is set to TRUE).
 *
 * \code
 *
 *      // Set the Tx link speed of master to Low speed
 *      IfxHssl_setHsctTxLinkSpeed(&hsct , IfxHssl_MasterModeTxSpeed_lowSpeed);
 *
 *      // Send interface command to change Rx to low speed at the target
 *      IfxHssl_Hssl_sendControlCommand(&hsct,IfxHssl_ControlCommand_lowSpeedReception);
 *
 *      // Set the Rx link speed of master to Low speed
 *      IfxHssl_setHsctRxLinkSpeed(&hsct, IfxHssl_MasterModeRxSpeed_lowSpeed);
 *
 *      // Send interface command to change Tx to low speed at the target
 *      IfxHssl_Hssl_sendControlCommand(&hsct,IfxHssl_ControlCommand_lowSpeedTransmission);
 *
 * \endcode
 *
 * The HSSL is ready for use now!
 *
 *
 * \section IfxLld_Hssl_Hssl_DataTransfers Data Transfers
 * \subsection IfxLld_Hssl_Hssl_SimpleTransfers Simple Transfers
 *
 * The HSSL driver provides simple to use data transfer functions,
 *
 * It supports direct writing of 8/16/32 bit data from the initiator into a target's register, as well as reading a value from the target
 *
 * \code
 *     // write some data to remote location:
 *     IfxHssl_Hssl_write(&hsslChannel[0], 0x70000000, 0x12345678, IfxHssl_DataLength_32bit);
 *
 *     // wait for the acknowledgement
 *     while( IfxHssl_Hssl_waitAcknowledge(&hsslChannel[0]) != IfxHssl_Hssl_Status_ok )
 *     {
 *         if( IfxHssl_Hssl_waitAcknowledge(&hsslChannel[0]) == IfxHssl_Hssl_Status_error )
 *           {
 *               IfxHssl_Hssl_checkErrors(&hssl);
 *               break;
 *           }
 *     }
 * \endcode
 *
 * A simple to use receive function is available as well.
 *
 * \code
 *     // read some data from remote location:
 *     IfxHssl_Hssl_read(&hsslChannel[0], 0x70000000, IfxHssl_DataLength_32bit);
 *
 *     // wait for the acknowledgement
 *     while( IfxHssl_Hssl_waitAcknowledge(&hsslChannel[0]) != IfxHssl_Hssl_Status_ok )
 *     {
 *         if( IfxHssl_Hssl_waitAcknowledge(&hsslChannel[0]) == IfxHssl_Hssl_Status_error )
 *           {
 *               IfxHssl_Hssl_checkErrors(&hssl);
 *               break;
 *           }
 *     }
 *
 *     // read data from the register
 *     uint32 dataL = IfxHssl_Hssl_getReadData(&hsslChannel[0]);
 * \endcode
 *
 * \subsection IfxLld_Hssl_Hssl_StreamingTransfers Streaming Transfers
 *
 * HSSL driver also supports streaming transfers of data as a memory block at both low and high speeds.
 *
 * Preparing the target for streaming with the desired memory location where the data needs to be transfered
 *
 * \code
 *     // choose a channel other than channel2 for register access //
 *     // prepare streaming of single memory block //
 *     IfxHssl_Hssl_prepareStream(&hsslChannel[0], 0x70000000, 10);
 * \endcode
 *
 * Stream the memory block
 *
 * Usage Example:
 * \code
 *     // __attribute__ ((aligned(256))) uint32 txData[80]; // expected to be declared globally
 *
 *     // for single block streaming transfer //
 *     // change the txData address to global address before passing it to the API
 *
 *     // IfxHssl_Hssl_writeStream(&hssl, (uint32 *)IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), (uint32)txData),  10);
 *
 *     IfxHssl_Hssl_writeStream(&hssl, txData, 10);
 *
 *     // wait until the streaming is finished
 *     while( IfxHssl_Hssl_waitAcknowledge(&hsslChannel[2]) != IfxHssl_Hssl_Status_ok )
 *     {}
 * \endcode
 *
 * \subsection IfxLld_Hssl_Hssl_DMAOperatedCommandQueues DMA Operated Command Queues
 *
 * It makes sense to do this from outside the driver, by initialising the DMA after HSSL, and send command queues through linked lists
 * here is an example of how to use DMA operated command queues.
 *
 *
 * Include following header files into your C code:
 * \code
 *     #include <Hssl/Hssl/IfxHssl_Hssl.h>
 *     #include <Dma/Dma/IfxDma_Dma.h>
 *     #include <Scu/Std/IfxScuWdt.h>
 * \endcode
 *
 * Declare the HSSL handle, HSCT handle, HSSL channel handle and DMA channel handle as global variables in your C code:
 *
 * \code
 *     // used globally
 *     static IfxHssl_Hssl hssl;
 *     static IfxHssl_Hsct hsct;
 *     IfxHssl_Hssl_Channel hsslChannel;
 *     // DMA channel handle
 *     IfxDma_Dma_Channel chn;
 *
 *     // Linked List storage
 *     // IMPORTANT: it has to be aligned to an 64bit address, otherwise DMA can't read it
 *     #define NUM_LINKED_LIST_ITEMS 3
 *     __attribute__ ((aligned(64))) Ifx_DMA_CH linkedList[NUM_LINKED_LIST_ITEMS] ;
 *     // transfer these values to channel 0 : HSSL_IWD0, HSSL_ICON0, HSSL_IRWA0 registers via linked lists
 *     #define NUM_TRANSFERED_WORDS 3
 *     // three write command queues with different data and to different addresses on slave
 *     uint32 sourceBuffer[NUM_LINKED_LIST_ITEMS][NUM_TRANSFERED_WORDS] = {
 *         {0xC0CAC01A, 0xFF0A0000, 0x70000000},
 *         {0xBA5EBA11, 0xFF0A0000, 0x70000010},
 *         {0xDEADBEEF, 0xFF0A0000, 0x70000020}
 *     };
 *
 *     // three command queues to the same channel 0
 *     const uint32 destinationAddresses[NUM_LINKED_LIST_ITEMS] = {
 *         (uint32)&HSSL_I0_IWD,
 *         (uint32)&HSSL_I0_IWD,
 *         (uint32)&HSSL_I0_IWD
 *     };
 * \endcode
 *
 * Initialise the hssl module, see \ref IfxLld_Hssl_Hssl_InitModule
 *
 * Initialise the hssl channel
 *
 * \code
 *     // create HSSL channel config
 *     IfxHssl_Hssl_ChannelConfig hsslChannelConfig;
 *     IfxHssl_Hssl_initChannelConfig(&hsslChannelConfig, &hssl, &hsct);
 *
 *     // initialize the channel 0
 *     // IfxHssl_Hssl_Channel hsslChannel; // defined globally
 *     hsslChannelConfig.channelId = 0;
 *     IfxHssl_Hssl_initChannel(&hsslChannel, &hsslChannelConfig);
 * \endcode
 *
 * Build a linked list
 *
 * \code
 *     // create module config
 *     IfxDma_Dma_Config dmaConfig;
 *     IfxDma_Dma_initModuleConfig(&dmaConfig, &MODULE_DMA);
 *     // initialize module
 *     IfxDma_Dma dma;
 *     IfxDma_Dma_initModule(&dma, &dmaConfig);
 *     // initial channel configuration
 *     IfxDma_Dma_ChannelConfig cfg;
 *     IfxDma_Dma_initChannelConfig(&cfg, &dma);
 *     // following settings are used by all transactions
 *     cfg.transferCount = NUM_TRANSFERED_WORDS;
 *     cfg.requestMode = IfxDma_ChannelRequestMode_completeTransactionPerRequest;
 *     cfg.moveSize = IfxDma_ChannelMoveSize_32bit;
 *     cfg.shadowControl = IfxDma_ChannelShadow_linkedList;
 *     // generate linked list items
 *     for(int i=0; i<NUM_LINKED_LIST_ITEMS; ++i) {
 *         cfg.sourceAddress = IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), &sourceBuffer[i]);
 *         cfg.destinationAddress = destinationAddresses[i];
 *         // address to next transaction set
 *         cfg.shadowAddress = IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), (uint32)&linkedList[(i + 1) % NUM_LINKED_LIST_ITEMS]);
 *         // transfer first transaction set into DMA channel
 *         if( i == 0 ) {
 *             IfxDma_Dma_initChannel(&chn, &cfg);
 *         }
 *         // transfer into linked list storage
 *         IfxDma_Dma_initLinkedListEntry((void *)&linkedList[i], &cfg);
 *         if( i == 0 ) {
 *             // - trigger channel interrupt once the first transaction set has been loaded (again) into DMA channel
 *             linkedList[i].CHCSR.B.SIT = 1;
 *         } else {
 *          // - activate SCH (transaction request) for each entry, expect for the first one (linked list terminated here)
 *             linkedList[i].CHCSR.B.SCH = 1;
 *         }
 *     }
 * \endcode
 *
 * The transfer can be started via software with:
 *
 * \code
 *     // clear service request flag
 *     (IfxDma_Dma_getSrcPointer(&chn))->B.CLRR = 1;
 *     // start linked list transaction
 *     IfxDma_Dma_startChannelTransaction(&chn);
 *     // wait for service request which is triggered at the end of linked list transfers
 *     while( !(IfxDma_Dma_getSrcPointer(&chn))->B.SRR );
 * \endcode
 *
 * \subsection IfxLld_Hssl_Hssl_Interrupts Interrupts usage
 *
 * Interrupts can be enabled from the application by using the APIs provided in the driver,
 * there are APIs available in the driver to enable, disable, clear and read the status of interrupt falgs, along with these,\n
 * APIs to enable the the enterrupts are also available.
 *
 * here is an example of how to use the interrupts in HSSL driver.
 *
 * Using HSCT interrupts
 *
 * After initialising HSCT module \ref IfxLld_Hssl_Hssl_InitModule
 *
 * choose what flags needs to be enabled for HSCT interrupt and call the following function once for each flag by choosing the right flag as parameter,\n
 * and after choosing all the flags needed, enable the HSCT interrupt with desired type of service and priority.
 *
 * \code
 *     Ifx_HSCT *hsct = &MODULE_HSCT;
 *     IfxHssl_enableHsctInterruptFlag(hsct, IfxHssl_Hsct_InterruptSource_headerError);
 *     IfxHssl_enableHsctInterruptFlag(hsct, IfxHssl_Hsct_InterruptSource_payloadError);
 *     IfxHssl_enableHsctInterruptFlag(hsct, IfxHssl_Hsct_InterruptSource_commandError);
 *     IfxHssl_enableHsctInterruptFlag(hsct, IfxHssl_Hsct_InterruptSource_speedModeSwitchError);
 *
 *     // enable the HSCT interrupt
 *     IfxHssl_enableHsctInterrupt(hsct, IfxSrc_Tos_cpu0, 4);
 * \endcode
 *
 * Using HSSl interrupts
 *
 * HSSL global error interrupt (EXI)
 *
 * After initilaising HSSL module \ref IfxLld_Hssl_Hssl_InitModule
 *
 * choose what flags needs to be enabled for HSSL global error (EXI) interrupt and call the following function once for each flag by choosing the right flag as parameter,\n
 * and after choosing all the flags needed, enable the HSSL EXI interrupt with desired type of service and priority
 *
 * \code
 *     Ifx_HSSL *hssl = &MODULE_HSSL;
 *     IfxHssl_enableHsslGlobalErrorInterruptFlag(hssl, IfxHssl_Hssl_EXIInterruptSource_busAccessError);
 *     IfxHssl_enableHsslGlobalErrorInterruptFlag(hssl, IfxHssl_Hssl_EXIInterruptSource_dataLengthError);
 *     IfxHssl_enableHsslGlobalErrorInterruptFlag(hssl, IfxHssl_Hssl_EXIInterruptSource_crcError);
 *
 *
 *     // enable the HSSL EXI interrupt
 *     IfxHssl_enableHsslEXIInterrupt(hssl, IfxSrc_Tos_cpu0, 6);
 * \endcode
 *
 * HSSL channel specific error interrupt (ERR)
 *
 * After initilaising HSSL channel \ref IfxLld_Hssl_Hssl_InitChannel
 *
 * choose what flags needs to be enabled for HSSL channel specific error (ERR) interrupt and call the following function once for each flag by choosing the right flag as parameter,\n
 * and after choosing all the flags needed, enable the HSSL ERR interrupt with desired channel, type of service and priority
 *
 * \code
 *     Ifx_HSSL *hssl = &MODULE_HSSL;
 *     IfxHssl_enableHsslChannelErrorInterruptFlag(hssl, channelId_0, IfxHssl_Hssl_ERRInterruptSource_transactionTagError);
 *     IfxHssl_enableHsslChannelErrorInterruptFlag(hssl, channelId_0, IfxHssl_Hssl_ERRInterruptSource_timeoutError);
 *
 *     // enable the HSSL ERR interrupt
 *     IfxHssl_enableHsslERRInterrupt(hssl, channelId_0, IfxSrc_Tos_cpu0, 8);
 * \endcode
 *
 * you can define the ISR of your own and service the interrupt, please refer to more general usage of interrupts \ref IfxLld_Cpu_Irq_Usage
 *
 * some additional APIs to clear, disable interrupt flags and get flag status are also available.
 *
 * \defgroup IfxLld_Hssl_Hssl HSSL
 * \ingroup IfxLld_Hssl
 * \defgroup IfxLld_Hssl_Hssl_DataStructures Data Structures
 * \ingroup IfxLld_Hssl_Hssl
 * \defgroup IfxLld_Hssl_Hssl_Enumerations Enumerations
 * \ingroup IfxLld_Hssl_Hssl
 * \defgroup IfxLld_Hssl_Hssl_ModuleFunctions Module Functions
 * \ingroup IfxLld_Hssl_Hssl
 * \defgroup IfxLld_Hssl_Hssl_ChannelFunctions Channel Functions
 * \ingroup IfxLld_Hssl_Hssl
 * \defgroup IfxLld_Hssl_Hssl_SimpleCom Simple Communication
 * \ingroup IfxLld_Hssl_Hssl
 * \defgroup IfxLld_Hssl_Hssl_ErrorHandling Error Handling
 * \ingroup IfxLld_Hssl_Hssl
 * \defgroup IfxLld_Hssl_Hssl_StreamingCom Streaming Communication
 * \ingroup IfxLld_Hssl_Hssl
 */

#ifndef IFXHSSL_HSSL_H
#define IFXHSSL_HSSL_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "Hssl/Std/IfxHssl.h"
#include "Port/Std/IfxPort.h"

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Hssl_Hssl_Enumerations
 * \{ */
/** \brief frame request selection
 */
typedef enum
{
    IfxHssl_Hssl_FrameRequest_readFrame    = 1,  /**< \brief read frame rquest */
    IfxHssl_Hssl_FrameRequest_writeFrame   = 2,  /**< \brief write frame rquest */
    IfxHssl_Hssl_FrameRequest_triggerFrame = 3,  /**< \brief trigger frame rquest */
    IfxHssl_Hssl_FrameRequest_readId       = 4,  /**< \brief read id request */
    IfxHssl_Hssl_FrameRequest_noAction     = 5   /**< \brief no action */
} IfxHssl_Hssl_FrameRequest;

/** \brief module status
 */
typedef enum
{
    IfxHssl_Hssl_Status_ok    = 0, /**< \brief status ok */
    IfxHssl_Hssl_Status_busy  = 1, /**< \brief status busy */
    IfxHssl_Hssl_Status_error = 2  /**< \brief status error */
} IfxHssl_Hssl_Status;

/** \} */

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Hssl_Hssl_DataStructures
 * \{ */
/** \brief structure for access windows
 */
typedef struct
{
    uint32 start;       /**< \brief start of the access */
    uint32 end;         /**< \brief end of the access */
} IfxHssl_Hssl_AccessWindow;

/** \} */

/** \brief structure for error flags
 */
typedef struct
{
    uint8 notAcknowledgeError : 1;        /**< \brief not acknowledge error / tag error */
    uint8 transactionTagError : 1;        /**< \brief transaction tag error */
    uint8 timeoutError : 1;               /**< \brief timeout error */
    uint8 unexpectedError : 1;            /**< \brief unexpected type of frame error */
    uint8 memoryAccessViolation : 1;      /**< \brief memory access violation */
    uint8 busAccessError : 1;             /**< \brief SRI/SPB bus access error */
    uint8 channelNumberCodeError : 1;     /**< \brief PHY inconsistency error 1 (channel number code error) */
    uint8 dataLengthError : 1;            /**< \brief PHY inconsistency error 2 (data length error) */
    uint8 crcError : 1;                   /**< \brief CRC error */
} IfxHssl_Hssl_errorFlags;

/** \addtogroup IfxLld_Hssl_Hssl_DataStructures
 * \{ */
/** \brief HSSL Handle
 */
typedef struct
{
    Ifx_HSSL               *hssl;             /**< \brief pointer to HSSL register */
    IfxHssl_Hssl_errorFlags errorFlags;       /**< \brief structure for error flags */
    boolean                 loopBack;         /**< \brief loop back (enable / disable) for streaming transfers within the microcontroller */
} IfxHssl_Hssl;

/** \brief channel handle
 */
typedef struct
{
    Ifx_HSSL                 *hssl;                      /**< \brief pointer to HSSL registers */
    Ifx_HSCT                 *hsct;                      /**< \brief pointer to HSCT registers */
    IfxHssl_ChannelId         channelId;                 /**< \brief channel number (id) */
    IfxHssl_Hssl_FrameRequest currentFrameRequest;       /**< \brief current frame request */
    IfxHssl_StreamingMode     streamingMode;             /**< \brief streaming mode selection ( single / continuous ) */
    boolean                   loopBack;                  /**< \brief loopback (enable / disable) for streaming transfers within the microcontroller */
    boolean                   streamingModeOn;           /**< \brief streaming mode or command mode */
} IfxHssl_Hssl_Channel;

/** \brief configuration structure for channel
 */
typedef struct
{
    Ifx_HSSL             *hssl;                /**< \brief pointer to HSSL registers */
    Ifx_HSCT             *hsct;                /**< \brief pointer to HSCT registers */
    IfxHssl_ChannelId     channelId;           /**< \brief channel number (id) */
    IfxHssl_StreamingMode streamingMode;       /**< \brief streaming mode selection ( single / continuous ) */
    boolean               loopBack;            /**< \brief loop back (enable / disable) for streaming transfers within the microcontroller */
} IfxHssl_Hssl_ChannelConfig;

/** \brief configuration structure of the HSSL module
 */
typedef struct
{
    Ifx_HSSL                 *hssl;                /**< \brief pointer to HSSL registers */
    IfxHssl_Hssl_AccessWindow accessWindow0;       /**< \brief access window of channel 0 */
    IfxHssl_Hssl_AccessWindow accessWindow1;       /**< \brief access window of channel 1 */
    IfxHssl_Hssl_AccessWindow accessWindow2;       /**< \brief access window of channel 2 */
    IfxHssl_Hssl_AccessWindow accessWindow3;       /**< \brief access window of channel 3 */
    uint16                    preDivider;          /**< \brief Defines the down-scaled module clock to be used by all channel timeout timers */
} IfxHssl_Hssl_Config;

/** \} */

/** \addtogroup IfxLld_Hssl_Hssl_ModuleFunctions
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Initialises the module
 * \param hsct HSCT Handle
 * \param config configuration structure of the HSCT module
 * \return None
 *
 * A coding example can be found in \ref IfxLld_Hssl_Hssl_Usage
 *
 */
IFX_EXTERN void IfxHssl_Hssl_initHsctModule(IfxHssl_Hsct *hsct, const IfxHssl_Hsct_Config *config);

/** \brief Fills the config structure with default values
 * \param config configuration structure of the HSCT module
 * \param hsct pointer to HSCT register
 * \return None
 *
 * A coding example can be found in \ref IfxLld_Hssl_Hssl_Usage
 *
 */
IFX_EXTERN void IfxHssl_Hssl_initHsctModuleConfig(IfxHssl_Hsct_Config *config, Ifx_HSCT *hsct);

/** \brief Initialises the Hssl module
 * \param hssl HSSL handle
 * \param config configuration structure of the module
 * \return None
 *
 * A coding example can be found in \ref IfxLld_Hssl_Hssl_Usage
 *
 */
IFX_EXTERN void IfxHssl_Hssl_initHsslModule(IfxHssl_Hssl *hssl, const IfxHssl_Hssl_Config *config);

/** \brief Fills the config structure with default values
 * \param config configuration structure of the module
 * \param hssl pointer to HSSL registers
 * \return None
 *
 * A coding example can be found in \ref IfxLld_Hssl_Hssl_Usage
 *
 */
IFX_EXTERN void IfxHssl_Hssl_initHsslModuleConfig(IfxHssl_Hssl_Config *config, Ifx_HSSL *hssl);

/** \} */

/** \addtogroup IfxLld_Hssl_Hssl_ChannelFunctions
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Initialises the channel
 * \param channel channel handle
 * \param channelConfig configuration structure for channel
 * \return None
 *
 * A coding example can be found in \ref IfxLld_Hssl_Hssl_Usage
 *
 */
IFX_EXTERN void IfxHssl_Hssl_initChannel(IfxHssl_Hssl_Channel *channel, const IfxHssl_Hssl_ChannelConfig *channelConfig);

/** \brief Fills the channel config structure with default values
 * \param channelConfig configuration structure for channel
 * \param hssl HSSL Handle
 * \param hsct HSCT Handle
 * \return None
 *
 * A coding example can be found in \ref IfxLld_Hssl_Hssl_Usage
 *
 */
IFX_EXTERN void IfxHssl_Hssl_initChannelConfig(IfxHssl_Hssl_ChannelConfig *channelConfig, IfxHssl_Hssl *hssl, IfxHssl_Hsct *hsct);

/** \} */

/** \addtogroup IfxLld_Hssl_Hssl_SimpleCom
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief reads and returs the data
 * \param channel channel handle
 * \return data
 *
 * A coding example can be found in \ref IfxLld_Hssl_Hssl_Usage
 *
 */
IFX_INLINE uint32 IfxHssl_Hssl_getReadData(IfxHssl_Hssl_Channel *channel);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Initiates read request
 * \param channel channel handle
 * \param address address of the location from where the data is to be read
 * \param dataLength length of the data
 * \return module status (ok, busy, error)
 *
 * A coding example can be found in \ref IfxLld_Hssl_Hssl_Usage
 *
 */
IFX_EXTERN IfxHssl_Hssl_Status IfxHssl_Hssl_read(IfxHssl_Hssl_Channel *channel, uint32 address, IfxHssl_DataLength dataLength);

/** \brief sends a predefined command from master to slave
 * \param hsct HSCT Handle
 * \param command command value
 * \return None
 *
 * Usage Example:
 * \code
 *     // enable slave Tx channel (Rx disable to Rx low peed) //
 *     IfxHssl_Hssl_sendControlCommand(&channel, IfxHssl_ControlCommand_enableReception);
 * \endcode
 *
 */
IFX_EXTERN void IfxHssl_Hssl_sendControlCommand(IfxHssl_Hsct *hsct, uint8 command);

/** \brief serves the frame request (read, write, trigger frame and read id)
 * \param channel channel handle
 * \param frameRequest frame request
 * \param address address of the location (to be written into / read from)
 * \param data data to be written
 * \param dataLength length of the data
 * \return module status (ok, busy, error)
 */
IFX_EXTERN IfxHssl_Hssl_Status IfxHssl_Hssl_singleFrameRequest(IfxHssl_Hssl_Channel *channel, IfxHssl_Hssl_FrameRequest frameRequest, uint32 address, uint32 data, IfxHssl_DataLength dataLength);

/** \brief waits until the current transaction is done
 * \param channel channel handle
 * \return module status (ok, busy, error)
 *
 * A coding example can be found in \ref IfxLld_Hssl_Hssl_Usage
 *
 */
IFX_EXTERN IfxHssl_Hssl_Status IfxHssl_Hssl_waitAcknowledge(IfxHssl_Hssl_Channel *channel);

/** \brief writes single frame of data into the specified address
 * \param channel channel handle
 * \param address address of the location where the data is to be written
 * \param data data that needs to be written
 * \param dataLength length of the data (8, 16, 32 bit)
 * \return module status (ok, busy, error)
 *
 * A coding example can be found in \ref IfxLld_Hssl_Hssl_Usage
 *
 */
IFX_EXTERN IfxHssl_Hssl_Status IfxHssl_Hssl_write(IfxHssl_Hssl_Channel *channel, uint32 address, uint32 data, IfxHssl_DataLength dataLength);

/** \} */

/** \addtogroup IfxLld_Hssl_Hssl_ErrorHandling
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief stores the status of errors in the respective members of the error flags structure
 * \param hssl HSSL Handle
 * \return None
 */
IFX_EXTERN void IfxHssl_Hssl_checkErrors(IfxHssl_Hssl *hssl);

/** \brief clears the status of members in the error flags structure
 * \param hssl HSSL Handle
 * \return None
 */
IFX_EXTERN void IfxHssl_Hssl_clearErrorFlags(IfxHssl_Hssl *hssl);

/** \brief a simple software delay
 * \param hsct HSCT Handle
 * \return None
 */
IFX_EXTERN void IfxHssl_Hssl_delay(IfxHssl_Hsct *hsct);

/** \} */

/** \addtogroup IfxLld_Hssl_Hssl_StreamingCom
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Prepares the target device for streaming
 * \param channel channel handle
 * \param slaveTargetAddress address of the location on target device where the data needs to be transfered
 * \param count Frame count (length of the data in the memory as 256 bytes per frame)
 * \return module status (ok, busy, error)
 *
 * A coding example can be found in \ref IfxLld_Hssl_Hssl_Usage
 *
 */
IFX_EXTERN IfxHssl_Hssl_Status IfxHssl_Hssl_prepareStream(IfxHssl_Hssl_Channel *channel, uint32 slaveTargetAddress, Ifx_SizeT count);

/** \brief transfers one memory block of data
 * useful for transfering huge data from one location to another and between devices.
 * NOTE: This function should be called only for IfxHssl_ChannelId_2
 * \param hssl HSSL handle
 * \param data starting address of the location to be read from (memory block 0 / HSSL_ISSA0)
 * \param count Frame count (length of the data in the memory as 256 bytes per frame)
 * \return module status (ok, busy, error)
 *
 * A coding example can be found in \ref IfxLld_Hssl_Hssl_Usage
 *
 */
IFX_EXTERN IfxHssl_Hssl_Status IfxHssl_Hssl_writeStream(IfxHssl_Hssl *hssl, uint32 *data, Ifx_SizeT count);

/** \} */

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE uint32 IfxHssl_Hssl_getReadData(IfxHssl_Hssl_Channel *channel)
{
    Ifx_HSSL_I *hsslI = (Ifx_HSSL_I *)&channel->hssl->I[channel->channelId];
    return hsslI->IRD.U;    // retutn the data read from the data register
}


#endif /* IFXHSSL_HSSL_H */
