/**
 * \file IfxPsi5s_Psi5s.h
 * \brief PSI5S PSI5S details
 * \ingroup IfxLld_Psi5s
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
 * \defgroup IfxLld_Psi5s_Psi5s_Usage How to use the PSI5S PSI5S Interface driver?
 * \ingroup IfxLld_Psi5s
 *
 * PSI5S defines a current loop based serial communication link typically used to connect airbag sensors or other peripheral devices.Data transmission and configuration of the sensor can be done by modulation of the Sync Pulses.
 *
 * In the following sections it will be described, how to integrate the driver into the application framework.
 *
 * \section IfxLld_Psi5s_Psi5s_Preparation Preparation
 * \subsection IfxLld_Psi5s_Psi5s_Include Include Files
 *
 * Include following header file into your C code:
 * \code
 *
 * #include <Psi5s/Psi5s/IfxPsi5s_Psi5s.h>
 *
 * \endcode
 *
 * \subsection IfxLld_Psi5s_Psi5s_Variables Variables
 *
 * \code
 *
 * // PSI5S handle
 * IfxPsi5s_Psi5s psi5s;
 *
 * \endcode
 *
 * \subsection IfxLld_Psi5s_Psi5s_initModule Module Initialisation
 * \code
 *
 * // create module config
 * IfxPsi5s_Psi5s_Config psi5sModuleConfig;
 * IfxPsi5s_Psi5s_initModuleConfig(&psi5sModuleConfig, &MODULE_PSI5S);
 * IfxPsi5s_Psi5s_initModule(&psi5s, &psi5sModuleConfig);
 *
 * \endcode
 *
 * \subsection IfxLld_Psi5s_Psi5s_initChannel Channel Initialisation
 * \code
 *
 * // create channel config
 * IfxPsi5s_Psi5s_ChannelConfig psi5sChannelConfig;
 * IfxPsi5s_Psi5s_initChannelConfig(&psi5sChannelConfig, &psi5s);
 * // change channel (default is channel 0, change to channel 4)
 * psi5sChannelConfig.channelId = IfxPsi5s_ChannelId_4;
 *
 * // change payloadlength (default is 0, i.e., no frame expecte; change to 8)
 * for(int slot=0; slot<6; ++slot)
 * {
 *    psi5sChannelConfig.receiveControl.payloadLength[slot] = 8;
 * }
 * // change channel (default is channel 0, change to channel 4)
 * psi5sChannelConfig.channelId = IfxPsi5s_ChannelId_4;
 *
 * // initialize the channel
 * IfxPsi5s_Psi5s_Channel psi5sChannel;
 * IfxPsi5s_Psi5s_initChannel(&psi5sChannel, &psi5sChannelConfig);
 *
 * \endcode
 *
 * Now, PSI5S is ready.  Then, call the below APIs to do operation.
 *
 * \subsection IfxLld_Psi5s_Psi5s_sendChannelData   Send Channel Data
 * \code
 * uint32 channels = (1 << IfxPsi5s_ChannelId_4); // enable channel 4
 * uint32 mask = (1 << IfxPsi5s_ChannelId_4); // modify the selection for channel 4
 *
 * // enable the channel trigger counter
 * IfxPsi5s_Psi5s_enableDisableChannelTriggerCounters(&psi5s, channels, mask);
 *
 * // enable the channel
 * IfxPsi5s_Psi5s_enableDisableChannels(&psi5s, channels, mask);
 *
 * // start ASC interface
 * IfxPsi5s_Psi5s_startAscTransactions(&psi5s);
 *
 * uint32 data = 0x5;
 * // send data
 * if(IfxPsi5s_Psi5s_sendChannelData(&psi5sChannel, data))
 * {
 * // wait till data transfer is completed
 * while(psi5s.psi5s->INTSTAT[IfxPsi5s_ChannelId_4].B.TPI == 0);
 * }
 *
 * \endcode
 *
 * \subsection IfxLld_Psi5s_Psi5s_readFrame  Read Channel Frame
 * \code
 * // enable ASC receiver
 * IfxPsi5s_Psi5s_enableAscReceiver(&psi5s);
 *
 * // start ASC interface
 * IfxPsi5s_Psi5s_startAscTransactions(&psi5s);
 *
 * // add the code below to initiate the sensor to transmit frames
 * // end of code to initiate the sensor to transmit frames
 *
 * IfxPsi5s_Psi5s_Frame frame;
 *
 * while(!IfxPsi5s_Psi5s_getReadFrameStatus(psi5,IfxPsi5s_ChannelId_4));
 *
 * IfxPsi5s_Psi5s_readFrame(&psi5sChannel, &frame))
 *
 * \endcode
 *
 * \defgroup IfxLld_Psi5s_Psi5s PSI5S
 * \ingroup IfxLld_Psi5s
 * \defgroup IfxLld_Psi5s_Psi5s_Structures Data Structures
 * \ingroup IfxLld_Psi5s_Psi5s
 * \defgroup IfxLld_Psi5s_Psi5s_Module Module Initialise Functions
 * \ingroup IfxLld_Psi5s_Psi5s
 * \defgroup IfxLld_Psi5s_Psi5s_Channel Channel Functions
 * \ingroup IfxLld_Psi5s_Psi5s
 * \defgroup IfxLld_Psi5s_Psi5s_Utility Utility Functions
 * \ingroup IfxLld_Psi5s_Psi5s
 * \defgroup IfxLld_Psi5s_Psi5s_Interrupt Interrupt configuration Function
 * \ingroup IfxLld_Psi5s_Psi5s
 */

#ifndef IFXPSI5S_PSI5S_H
#define IFXPSI5S_PSI5S_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "Psi5s/Std/IfxPsi5s.h"
#include "Scu/Std/IfxScuWdt.h"

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Psi5s_Psi5s_Structures
 * \{ */
/** \brief Clock configuration data structure
 */
typedef struct
{
    uint32               frequency;       /**< \brief Specifies the frequency for the clock */
    IfxPsi5s_DividerMode mode;            /**< \brief Specifies the mode of division for the clock */
    IfxPsi5s_ClockType   type;            /**< \brief Specifies the type of clock (fracDiv / timestamp) */
} IfxPsi5s_Psi5s_Clock;

/** \brief Received individual bits
 */
typedef struct
{
    uint8 xcrc : 6;                            /**< \brief Received XCRC */
    uint8 xcrcError : 1;                       /**< \brief XCRC error flag */
    uint8 crc : 3;                             /**< \brief Received CRC */
    uint8 crcError : 1;                        /**< \brief CRC error flag */
    uint8 errorFlag0 : 1;                      /**< \brief Error signalling flag 0 */
    uint8 errorFlag1 : 1;                      /**< \brief Error signalling flag 1 */
    uint8 headerErrorFlag : 1;                 /**< \brief Header error signalling flag */
    uint8 ascParityErrorFlag : 1;              /**< \brief ASC parity error flag */
    uint8 ascFramingErrorFlag : 1;             /**< \brief ASC framing error flag */
    uint8 ascOverrunErrorFlag : 1;             /**< \brief ASC overrun error flag */
    uint8 watchdogTimeoutErrorFlag : 1;        /**< \brief Watchdog timeout error flag */
    uint8 receiveBufferOverflowFlag : 1;       /**< \brief Receive buffer overflow flag */
    uint8 frameId : 3;                         /**< \brief Frame ID */
    uint8 channelId : 3;                       /**< \brief Channel ID */
    uint8 actualUartFrameCount : 3;            /**< \brief UART frames actually received */
    uint8 packetFrameCount : 4;                /**< \brief Packet frame count */
} IfxPsi5s_Psi5s_ReceivedBits;

/** \brief Receive data structure with different segments of data
 */
typedef struct
{
    uint32 readData : 28;          /**< \brief Received data */
    uint8  packetFrameCount;       /**< \brief Packet frame count */
} IfxPsi5s_Psi5s_ReceivedData;

/** \brief Received timestamp contents structure
 */
typedef struct
{
    uint8  packetFrameCount : 4;       /**< \brief Packet frame count */
    uint32 timestamp : 24;             /**< \brief Received data */
} IfxPsi5s_Psi5s_Timestamp;

/** \} */

/** \addtogroup IfxLld_Psi5s_Psi5s_Structures
 * \{ */
/** \brief PSI5S handle data structure
 */
typedef struct
{
    Ifx_PSI5S *psi5s;       /**< \brief Specifies the pointer to the PSI5S module registers */
} IfxPsi5s_Psi5s;

/** \brief PSI5S module configuration structure
 */
typedef struct
{
    boolean                       parityCheckEnabled;                /**< \brief Specifies whether parity check is enabled or not */
    boolean                       framingCheckEnabled;               /**< \brief Specifies whether framing check is enabled or not */
    boolean                       overrunCheckEnabled;               /**< \brief Specifies whether overrun check is enabled or not */
    boolean                       fractionalDividerEnabled;          /**< \brief Specifies whether fractional divider is enabled or not */
    boolean                       receiverOddParityEnabled;          /**< \brief Specifies whether receiver parity check should be for even or odd */
    boolean                       transmitterOddParityEnabled;       /**< \brief Specifies whether transmit parity generation should be even or odd */
    uint32                        baudrateFrequency;                 /**< \brief Specifies the baudrate frequency */
    IfxPsi5s_AscStopBits          stopBits;                          /**< \brief Specifies the number of stop bits */
    IfxPsi5s_AscMode              receiveMode;                       /**< \brief Specifies the mode of operation for ASC receiver */
    IfxPsi5s_AscBaudratePrescalar baudrateSelection;                 /**< \brief Specifies whether divide-by-2 or divide-by-3 is selected for baudrate prescalar */
    IfxPsi5s_AscMode              transmitMode;                      /**< \brief Specifies the mode of operation for ASC transmitter */
    IfxPsi5s_Psi5s_Clock          clockOutput;                       /**< \brief Specifies the output clock properties */
    IfxPsi5s_LoopBackMode         loopbackEnabled;                   /**< \brief Specifies whether loopback is enabled or not */
} IfxPsi5s_Psi5s_AscConfig;

/** \brief channel trigger configuration structure
 */
typedef struct
{
    uint32 channelTriggerValue;         /**< \brief Specifies the channel trigger value CTV */
    uint32 channelTriggerCounter;       /**< \brief Specifies the channel trigger counter */
} IfxPsi5s_Psi5s_ChannelTrigger;

/** \brief PSI5S global control configuration structure
 */
typedef struct
{
    boolean ascOnlyMode;                              /**< \brief Specifies if the module is in ASC only mode */
    boolean crcErrorConsideredForRSI;                 /**< \brief Specifies whether CRCI is considered for RSI assertion */
    boolean xcrcErrorConsideredForRSI;                /**< \brief Specifies whether XCRCI is considered for RSI assertion */
    boolean transmitErrorConsideredForRSI;            /**< \brief Specifies whether TEI is considered for RSI assertion */
    boolean parityErrorConsideredForRSI;              /**< \brief Specifies whether PE is considered for RSI assertion */
    boolean framingErrorConsideredForRSI;             /**< \brief Specifies whether FE is considered for RSI assertion */
    boolean overrunErrorConsideredForRSI;             /**< \brief Specifies whether OE is considered for RSI assertion */
    boolean receiveBufferErrorConsideredForRSI;       /**< \brief Specifies whether RBI is considered for RSI assertion */
    boolean headerErrorConsideredForRSI;              /**< \brief Specifies whether HDI is considered for RSI assertion */
    uint32  idleTime;                                 /**< \brief Specifies the number of stop bits in addition to last UART frame that is required for start of frame detection */
} IfxPsi5s_Psi5s_GlobalControlConfig;

/** \brief Structure for PSI5S pin configuration
 */
typedef struct
{
    IFX_CONST IfxPsi5s_Rx_In   *rx;              /**< \brief PSI5S Rx pin */
    IfxPort_InputMode           rxMode;          /**< \brief Rx pin as input */
    IFX_CONST IfxPsi5s_Tx_Out  *tx;              /**< \brief PSI5S Tx pin */
    IfxPort_OutputMode          txMode;          /**< \brief Tx as output */
    IFX_CONST IfxPsi5s_Clk_Out *clk;             /**< \brief PSI5S Clk Pin */
    IfxPort_OutputMode          clkMode;         /**< \brief Clk as output */
    IfxPort_PadDriver           pinDriver;       /**< \brief pad driver */
} IfxPsi5s_Psi5s_Pins;

/** \brief Sync pulse generation configuration structure
 */
typedef struct
{
    uint32               codeforZero;                  /**< \brief Specifies the code used to represent '0' - referred as TXCMD */
    uint32               codeforOne;                   /**< \brief Specifies the code used to represent '1' - referred as ATXCMD */
    IfxPsi5s_TimeBase    timeBaseSelect;               /**< \brief Specifies the clock source for CTV as internal or external */
    IfxPsi5s_Trigger     externalTimeBaseSelect;       /**< \brief Specifies the clock source for CTV in the case of external */
    IfxPsi5s_TriggerType periodicOrExternal;           /**< \brief Specifies whether periodic trigger or external trigger or bypass is selected */
    IfxPsi5s_Trigger     externalTriggerSelect;        /**< \brief Specifies the trigger source in case of external triggerring */
} IfxPsi5s_Psi5s_PulseGeneration;

/** \brief Receive control configuration structure - covers control fields from RCRA, RCRB and NFC registers
 */
typedef struct
{
    boolean                       timestampEnabled;                         /**< \brief Specifies whether the timestamp is enabled or not */
    uint32                        payloadLength[IFXPSI5S_NUM_SLOTS];        /**< \brief Specifies the payload length to be received for each slot */
    IfxPsi5s_CrcOrParity          crcOrParity[IFXPSI5S_NUM_SLOTS];          /**< \brief Specifies the crc or parity selection for the slots 0 to 5 */
    IfxPsi5s_TimestampRegister    timestampSelect;                          /**< \brief Specifies the timestamp register selection for pulses */
    IfxPsi5s_TimestampTrigger     timestampTriggerSelect;                   /**< \brief Specifies the timestamp register selection for pulses */
    IfxPsi5s_FrameId              frameIdSelect;                            /**< \brief Specifies if frame ID is updated from frame header or is a rolling number 0..5 copied from FCNT */
    IfxPsi5s_WatchdogTimerMode    watchdogTimerModeSelect;                  /**< \brief Specifies the watchdog timer restart is wrt frame or sync pulse reception */
    IfxPsi5s_UartFrameCount       uartFrameCount[IFXPSI5S_NUM_SLOTS];       /**< \brief Specifies the expected number of UART frames per packet frame for each slot */
    IfxPsi5s_NumberExpectedFrames numberOfFramesExpected;                   /**< \brief Specifies the number of psi5s frames expected */
} IfxPsi5s_Psi5s_ReceiveControl;

/** \brief "Received data" data structure
 */
typedef union
{
    uint32                      rdr;                /**< \brief received data with frame count */
    IfxPsi5s_Psi5s_ReceivedData receivedData;       /**< \brief Receive data structure with different segments of data */
} IfxPsi5s_Psi5s_ReceiveData;

/** \brief Receiver status data structure
 */
typedef union
{
    uint32                      rds;                /**< \brief received status data. */
    IfxPsi5s_Psi5s_ReceivedBits receivedBits;       /**< \brief Received individual bits */
} IfxPsi5s_Psi5s_ReceiveStatus;

/** \brief Receiver timestamp data structure
 */
typedef union
{
    uint32                   tsm;             /**< \brief received timestamp along with frame count */
    IfxPsi5s_Psi5s_Timestamp timeStamp;       /**< \brief Received timestamp contents structure */
} IfxPsi5s_Psi5s_ReceiveTimestamp;

/** \brief Timestamp configuration data structure
 */
typedef struct
{
    IfxPsi5s_Trigger  externalTimeBaseSelect;       /**< \brief Specifies the clock base for counter CTS in the case of external */
    IfxPsi5s_TimeBase timeBaseSelect;               /**< \brief Specifies the clock source for CTS as internal or external */
} IfxPsi5s_Psi5s_TimeStampConfig;

/** \brief Transmit control configuration structure
 */
typedef struct
{
    boolean                   bitStuffControl;                      /**< \brief Specifies whether the bit stuffing is turned on or not */
    boolean                   crcGenerationControl;                 /**< \brief Specifies whether the crc generation is turned on or not */
    boolean                   startSequenceGenerationControl;       /**< \brief Specifies whether the start sequence generation is turned on or not */
    uint32                    payloadLength;                        /**< \brief Specifies the payload length to be sent */
    IfxPsi5s_EnhancedProtocol enhancedProtocolSelection;            /**< \brief Specifies whether the enhanced protocol is selected or not */
} IfxPsi5s_Psi5s_TransmitControl;

/** \} */

/** \addtogroup IfxLld_Psi5s_Psi5s_Structures
 * \{ */
/** \brief Channel handle data structure
 */
typedef struct
{
    IfxPsi5s_Psi5s    *module;          /**< \brief The PSI5S handle structure */
    IfxPsi5s_ChannelId channelId;       /**< \brief Specifies the channel index */
} IfxPsi5s_Psi5s_Channel;

/** \brief Channel configuration structure
 */
typedef struct
{
    IFX_CONST IfxPsi5s_Psi5s      *module;                   /**< \brief Specifies pointer to the IfxPsi5s_Psi5s module handle */
    uint32                         watchdogTimerLimit;       /**< \brief Specifies the watchdog timer limit for each of the slots 0 to 6 */
    IfxPsi5s_ChannelId             channelId;                /**< \brief Specifies the channel index */
    IfxPsi5s_Psi5s_PulseGeneration pulseGeneration;          /**< \brief Specifies the configuration for sync pulse generation */
    IfxPsi5s_Psi5s_ChannelTrigger  channelTrigger;           /**< \brief Specifies the configuration for channel trigger */
    IfxPsi5s_Psi5s_ReceiveControl  receiveControl;           /**< \brief Specifies the configuration for reception */
    IfxPsi5s_Psi5s_TransmitControl sendControl;              /**< \brief Specifies the configuration for transmission */
} IfxPsi5s_Psi5s_ChannelConfig;

/** \brief PSI5S module configuration structure
 */
typedef struct
{
    Ifx_PSI5S                         *module;                    /**< \brief Specifies the pointer to the PSI5S module registers */
    IfxPsi5s_Psi5s_Clock               fracDiv;                   /**< \brief Specifies fractionally divided clock properties */
    IfxPsi5s_Psi5s_Clock               timestampClock;            /**< \brief Specifies the time stamp clock properties */
    IfxPsi5s_Psi5s_TimeStampConfig     timestampCounterA;         /**< \brief Specifies the time stamp counter A properties */
    IfxPsi5s_Psi5s_TimeStampConfig     timestampCounterB;         /**< \brief Specifies the time stamp counter B properties */
    IfxPsi5s_Psi5s_AscConfig           ascConfig;                 /**< \brief Specifies the configuration for ASC */
    IfxPsi5s_Psi5s_GlobalControlConfig globalControlConfig;       /**< \brief Specifies the global control configuration */
    IFX_CONST IfxPsi5s_Psi5s_Pins     *pins;                      /**< \brief structure for PSI5S pin configuration */
} IfxPsi5s_Psi5s_Config;

/** \brief Psi5s frame data structure
 */
typedef struct
{
    IfxPsi5s_Psi5s_ReceiveData      data;            /**< \brief Received data */
    IfxPsi5s_Psi5s_ReceiveStatus    status;          /**< \brief Receiver status */
    IfxPsi5s_Psi5s_ReceiveTimestamp timestamp;       /**< \brief Receiver timestamp */
} IfxPsi5s_Psi5s_Frame;

/** \} */

/** \addtogroup IfxLld_Psi5s_Psi5s_Module
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief De-initialise the PSI5S module
 * \param psi5s pointer to the PSI5S module
 * \return None
 */
IFX_EXTERN void IfxPsi5s_Psi5s_deInitModule(IfxPsi5s_Psi5s *psi5s);

/** \brief Initialise the PSI5S with the supplied configureation
 * \param psi5s pointer to the PSI5S module
 * \param config pointer to the PSI5S configuration
 * \return TRUE if valid otherwise FALSE
 *
 * Usage Example: \ref IfxLld_Psi5s_Psi5s_Usage
 *
 */
IFX_EXTERN boolean IfxPsi5s_Psi5s_initModule(IfxPsi5s_Psi5s *psi5s, const IfxPsi5s_Psi5s_Config *config);

/** \brief Initialise buffer with default PSI5S configuration
 * \param config pointer to the PSI5S module configuration buffer
 * \param psi5s pointer to the PSI5S register space
 * \return None
 *
 * Usage Example: \ref IfxLld_Psi5s_Psi5s_Usage
 *
 */
IFX_EXTERN void IfxPsi5s_Psi5s_initModuleConfig(IfxPsi5s_Psi5s_Config *config, Ifx_PSI5S *psi5s);

/** \} */

/** \addtogroup IfxLld_Psi5s_Psi5s_Channel
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Enable ASC interface receiver
 * \param psi5s pointer to the PSI5S module
 * \return None
 *
 * Usage Example: \ref IfxLld_Psi5s_Psi5s_Usage
 *
 */
IFX_INLINE void IfxPsi5s_Psi5s_enableAscReceiver(IfxPsi5s_Psi5s *psi5s);

/** \brief Access function to enable/disable any combination of channel trigger counters selected by mask parameter
 * \param psi5s pointer to the PSI5S module
 * \param channels specifies the channel trigger counters which should be enabled/disabled
 * \param mask specifies the channel trigger counters which should be modified
 * \return None
 *
 * Usage Example: \ref IfxLld_Psi5s_Psi5s_Usage
 *
 */
IFX_INLINE void IfxPsi5s_Psi5s_enableDisableChannelTriggerCounters(IfxPsi5s_Psi5s *psi5s, uint32 channels, uint32 mask);

/** \brief Access function to enable/disable any combination of channels selected by mask parameter
 * \param psi5s pointer to the PSI5S module
 * \param channels specifies the channels which should be enabled/disabled
 * \param mask specifies the channels which should be modified
 * \return None
 *
 * Usage Example: \ref IfxLld_Psi5s_Psi5s_Usage
 *
 */
IFX_INLINE void IfxPsi5s_Psi5s_enableDisableChannels(IfxPsi5s_Psi5s *psi5s, uint32 channels, uint32 mask);

/** \brief Start ASC transactions
 * \param psi5s pointer to the PSI5S module
 * \return None
 *
 * Usage Example: \ref IfxLld_Psi5s_Psi5s_Usage
 *
 */
IFX_INLINE void IfxPsi5s_Psi5s_startAscTransactions(IfxPsi5s_Psi5s *psi5s);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Initialize the channel with the supplied configuration
 * \param channel pointer to the PSI5S channel
 * \param config pointer to the PSI5S channel configuration
 * \return TRUE on success & FALSE if configuration not valid (e.g. missing resource)
 *
 * Usage Example: \ref IfxLld_Psi5s_Psi5s_Usage
 *
 */
IFX_EXTERN boolean IfxPsi5s_Psi5s_initChannel(IfxPsi5s_Psi5s_Channel *channel, const IfxPsi5s_Psi5s_ChannelConfig *config);

/** \brief Initialise buffer with default channel configuration
 * \param config pointer to the PSI5S channel configuration
 * \param psi5s pointer to the PSI5S module
 * \return None
 *
 * Usage Example: \ref IfxLld_Psi5s_Psi5s_Usage
 *
 */
IFX_EXTERN void IfxPsi5s_Psi5s_initChannelConfig(IfxPsi5s_Psi5s_ChannelConfig *config, IfxPsi5s_Psi5s *psi5s);

/** \brief Get the received psi5s frame for the channel
 * \param channel pointer to the PSI5S module
 * \param frame pointer to the PSI5S frame buffer
 * \return None
 *
 * Usage Example: \ref IfxLld_Psi5s_Psi5s_Usage
 *
 */
IFX_EXTERN void IfxPsi5s_Psi5s_readFrame(IfxPsi5s_Psi5s_Channel *channel, IfxPsi5s_Psi5s_Frame *frame);

/** \brief Transmit the data through the channel
 * \param channel pointer to the PSI5S channel
 * \param data data to be sent
 * \return TRUE if Sends data otherwise FALSE
 *
 * Usage Example: \ref IfxLld_Psi5s_Psi5s_Usage
 *
 */
IFX_EXTERN boolean IfxPsi5s_Psi5s_sendChannelData(IfxPsi5s_Psi5s_Channel *channel, uint32 data);

/** \} */

/** \addtogroup IfxLld_Psi5s_Psi5s_Utility
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Enable PSI5S kernel
 * \param psi5s pointer to the base of PSI5S register space
 * \return None
 */
IFX_EXTERN void IfxPsi5s_Psi5s_enableModule(Ifx_PSI5S *psi5s);

/** \brief get Baud Rate
 * \param psi5s Pointer to PSI5S Module space
 * \param ascConfig Pointer to ASC configuration structure
 * \return None
 */
IFX_EXTERN void IfxPsi5s_Psi5s_getBaudrate(Ifx_PSI5S *psi5s, IfxPsi5s_Psi5s_AscConfig *ascConfig);

/** \brief Reset PSI5S kernel
 * \param psi5s pointer to the base of PSI5S registers
 * \return None
 */
IFX_EXTERN void IfxPsi5s_Psi5s_resetModule(Ifx_PSI5S *psi5s);

/** \} */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Get the received psi5s frame for the channel
 * \param psi5s pointer to the PSI5S module
 * \param channelId specifies channelID
 * \return return the status of Frame
 */
IFX_INLINE boolean IfxPsi5s_Psi5s_getReadFrameStatus(IfxPsi5s_Psi5s *psi5s, IfxPsi5s_ChannelId channelId);

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE void IfxPsi5s_Psi5s_enableAscReceiver(IfxPsi5s_Psi5s *psi5s)
{
    IfxPsi5s_enableAscReceiver(psi5s->psi5s);
}


IFX_INLINE void IfxPsi5s_Psi5s_enableDisableChannelTriggerCounters(IfxPsi5s_Psi5s *psi5s, uint32 channels, uint32 mask)
{
    IfxPsi5s_enableDisableChannelTriggerCounters(psi5s->psi5s, channels, mask);
}


IFX_INLINE void IfxPsi5s_Psi5s_enableDisableChannels(IfxPsi5s_Psi5s *psi5s, uint32 channels, uint32 mask)
{
    IfxPsi5s_enableDisableChannels(psi5s->psi5s, channels, mask);
}


IFX_INLINE void IfxPsi5s_Psi5s_startAscTransactions(IfxPsi5s_Psi5s *psi5s)
{
    IfxPsi5s_startAscTransactions(psi5s->psi5s);
}


IFX_INLINE boolean IfxPsi5s_Psi5s_getReadFrameStatus(IfxPsi5s_Psi5s *psi5s, IfxPsi5s_ChannelId channelId)
{
    boolean flag = 0;
    flag = IfxPsi5s_getReadFrameStatus(psi5s->psi5s, channelId);
    return flag;
}


#endif /* IFXPSI5S_PSI5S_H */
