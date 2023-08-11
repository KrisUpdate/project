/**
 * \file IfxEvadc_Adc.h
 * \brief EVADC ADC details
 * \ingroup IfxLld_Evadc
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
 * \defgroup IfxLld_Evadc_Adc_Usage How to use the EVADC ADC Interface driver?
 * \ingroup IfxLld_Evadc
 *
 * EVADC comprises of independent analog channels with Analog/Digital converters to convert analog input to discrete digital output.
 *
 * In the following sections it will be described, how to integrate the driver into the application framework.
 *
 * \section IfxLld_Evadc_Adc_Preparation Preparation
 * \subsection IfxLld_Evadc_Adc_Include Include Files
 *
 * Include following header file into your C code:
 * \code
 *
 * #include <Evadc/Adc/IfxEvadc_Adc.h>
 *
 * \endcode
 *
 * \subsection IfxLld_Evadc_Adc_Variables Variables
 * \code
 *
 * // EVADC handle
 * IfxEvadc_Adc evadc;
 * IfxEvadc_Adc_Group adcGroup;
 * \endcode
 *
 * \subsection IfxLld_Evadc_Adc_ModuleInitialisation Module Initialisation
 * The module initialisation can be done in the same function:
 * \code
 *     // create configuration
 *     IfxEvadc_Adc_Config adcConfig;
 *     IfxEvadc_Adc_initModuleConfig(&adcConfig, &MODULE_EVADC);
 *
 *     // initialize module
 *     // IfxEvadc_Adc evadc; // declared globally
 *     IfxEvadc_Adc_initModule(&evadc, &adcConfig);
 * \endcode
 *
 *
 * \subsection IfxLld_Evadc_Adc_GroupInitialisation Group Initialisation
 * The group initialisation can be done in the same function:
 * \code
 *
 * // create group config
 *     IfxEvadc_Adc_GroupConfig adcGroupConfig;
 *     IfxEvadc_Adc_initGroupConfig(&adcGroupConfig, &evadc);
 *
 *     // change group (default is GroupId_0, change to GroupId_3)
 *     adcGroupConfig.groupId = IfxEvadc_GroupId_3;
 *     adcGroupConfig.master = adcGroupConfig.groupId;
 *     adcGroupConfig.startupCalibration =TRUE;
 *
 *     // enable all arbiter request sources
 *     adcGroupConfig.arbiter.requestSlotQueue0Enabled           = TRUE; // enable Queue0 mode
 *     adcGroupConfig.arbiter.requestSlotQueue1Enabled           = TRUE; // enable Queue1 mode
 *     adcGroupConfig.arbiter.requestSlotQueue2Enabled           = TRUE; // enable Queue2 mode
 *
 *     // enable all gates in "always" mode (no edge detection)
 *     adcGroupConfig.queueRequest[0].triggerConfig.gatingMode = IfxEvadc_GatingMode_always;
 *     adcGroupConfig.queueRequest[1].triggerConfig.gatingMode = IfxEvadc_GatingMode_always;
 *     adcGroupConfig.queueRequest[2].triggerConfig.gatingMode = IfxEvadc_GatingMode_always;
 *
 *
 *
 *     // initialize the group
 *     //IfxEvadc_Adc_Group adcGroup;//declared globally
 *     IfxEvadc_Adc_initGroup(&adcGroup, &adcGroupConfig);
 *
 * \endcode
 *
 * \subsection IfxLld_Evadc_Adc_QueuedTransfers Queued Transfers
 * Now, EVADC is initialised. Here,Three channels are used for queued transfers
 * \code
 *     // IMPORTANT: for deterministic results we have to disable the queue gate
 *     // while filling the queue, otherwise results could be output in the wrong order
 *
 *         IfxEvadc_RequestSource requestSource = IfxEvadc_RequestSource_queue0;
 *
 *         IfxEvadc_GatingMode savedGate =  IfxEvadc_getQueueSlotGatingMode(adcGroup.group,requestSource);
 *         IfxEvadc_GatingSource gatingSource = IfxEvadc_getQueueSlotGatingSource(adcGroup.group, requestSource );
 *
 *          IfxEvadc_setQueueSlotGatingConfig(adcGroup.group, gatingSource, IfxEvadc_GatingMode_disabled, requestSource );
 *     // create channel config
 *     IfxEvadc_Adc_ChannelConfig adcChannelConfig[3];
 *     IfxEvadc_Adc_Channel adcChannel[3];
 *
 *     for(int chnIx=0; chnIx<3; ++chnIx) {
 *         IfxEvadc_Adc_initChannelConfig(&adcChannelConfig[chnIx], &adcGroup);
 *
 *         adcChannelConfig[chnIx].channelId = (IfxEvadc_ChannelId)(chnIx);
 *         adcChannelConfig[chnIx].resultRegister = IfxEvadc_ChannelResult_1; // use result register #1 for all channels
 *
 *         // initialize the channel
 *         IfxEvadc_Adc_initChannel(&adcChannel[chnIx], &adcChannelConfig[chnIx]);
 *
 *         // Add channel to queue with refill enabled
 *             IfxEvadc_Adc_addToQueue(&adcChannel[chnIx], requestSource, IFXEVADC_QUEUE_REFILL);
 *         }
 *
 *         // restore previous gate config
 *
 *          IfxEvadc_setQueueSlotGatingConfig(adcGroup.group, gatingSource, savedGate, requestSource );
 *
 *         // start the Queue
 *         IfxEvadc_Adc_startQueue(&adcGroup, requestSource); // just for the case that somebody copy&pastes the code - the queue has already been started in previous test
 *
 *         // get 10 results for all 3 channels and store in temporary buffer
 *         // (the usage of a buffer is required, since the print statements used by the checks take more time than the conversions)
 *         Ifx_EVADC_G_RES resultTrace[3*10];
 *         for(int i=0; i<3*10; ++i)
 *         {
 *             unsigned chnIx = i % 3;
 *
 *             // wait for valid result
 *             Ifx_EVADC_G_RES conversionResult;
 *             do {
 *                 conversionResult = IfxEvadc_Adc_getResult(&adcChannel[chnIx]);
 *             } while( !conversionResult.B.VF );
 *
 *             // store result
 *             resultTrace[i] = conversionResult;
 *         }
 *
 *         // stop the queue
 *         IfxEvadc_Adc_clearQueue(&adcGroup, requestSource);
 *
 *         // check results in buffer
 *  // ...
 *
 * \endcode
 *
 * \subsection IfxLld_Evadc_Adc_EmuxConfiguration External Multiplexer Configuration
 * External Configuration of 3 channels at channel 3
 *
 * \code
 *
 *       IfxEvadc_Adc_EmuxControl emuxConfig;
 *
 *         IfxEvadc_Adc_initExternalMultiplexerModeConfig(&emuxConfig,evadc);
 *
 *         emuxConfig.groupId           = IfxEvadc_GroupId_1;
 *         emuxConfig.channels          = (uint8)IfxEvadc_ChannelId_3;
 *         emuxConfig.startChannel   = IfxEvadc_EmuxSelectValue_2; // it will take 0 to 2 external channel
 *
 *         emuxConfig.sampleTimeControl = IfxEvadc_EmuxSampleTimeControl_always;
 *         emuxConfig.mode              = IfxEvadc_ExternalMultiplexerMode_steady;
 *
 *         IfxEvadc_Adc_EmuxPinConfig pinsConfig ={
 *  .pins={ &IfxEvadc_EMUX00_P02_6_OUT,
 *          &IfxEvadc_EMUX01_P02_7_OUT,
 *          &IfxEvadc_EMUX02_P02_8_OUT},
 *
 *
 *         .outputMode = IfxPort_OutputMode_pushPull,
 *         .padDriver  = IfxPort_PadDriver_cmosAutomotiveSpeed1
 *  };
 *         emuxConfig.emuxOutPinConfig = pinsConfig;
 *
 *         IfxEvadc_Adc_initExternalMultiplexerMode(evadc, &emuxConfig);
 *
 *         IfxEvadc_setEmuxGroupResolution(&evadc->G[emuxConfig.groupId], 0, IfxEvadc_ChannelResolution_12bit);
 *         IfxEvadc_setEmuxGroupSampletime(&evadc->G[emuxConfig.groupId], 0, 50000, 1.0e-6);
 *
 * \endcode
 *
 * \defgroup IfxLld_Evadc_Adc Interface Driver
 * \ingroup IfxLld_Evadc
 * \defgroup IfxLld_Evadc_Adc_DataStructures Data Structures
 * \ingroup IfxLld_Evadc_Adc
 * \defgroup IfxLld_Evadc_Adc_Module Module Functions
 * \ingroup IfxLld_Evadc_Adc
 * \defgroup IfxLld_Evadc_Adc_Group Group Functions
 * \ingroup IfxLld_Evadc_Adc
 * \defgroup IfxLld_Evadc_Adc_Channel Channel Functions
 * \ingroup IfxLld_Evadc_Adc
 * \defgroup IfxLld_Evadc_Adc_Queue Queue Functions
 * \ingroup IfxLld_Evadc_Adc
 * \defgroup IfxLld_Evadc_Adc_Clock Clock Functions
 * \ingroup IfxLld_Evadc_Adc
 * \defgroup IfxLld_Evadc_Adc_Interrupt Interrupt Functions
 * \ingroup IfxLld_Evadc_Adc
 * \defgroup IfxLld_Evadc_Adc_Variables Variables
 * \ingroup IfxLld_Evadc_Adc
 * \defgroup IfxLld_Evadc_Adc_Global Global Functions
 * \ingroup IfxLld_Evadc_Adc
 * \defgroup IfxLld_Evadc_Adc_Emux Emux Functions
 * \ingroup IfxLld_Evadc_Adc
 * \defgroup IfxLld_Evadc_Adc_FastCompare Fast Compare Functions
 * \ingroup IfxLld_Evadc_Adc
 */

#ifndef IFXEVADC_ADC_H
#define IFXEVADC_ADC_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "Evadc/Std/IfxEvadc.h"
#include "_Utilities/Ifx_Assert.h"

/******************************************************************************/
/*------------------------------Type Definitions------------------------------*/
/******************************************************************************/

typedef uint8 IfxEvadc_Adc_SYNCTR_STSEL;

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Evadc_Adc_DataStructures
 * \{ */
/** \brief EVADC handle data structure
 */
typedef struct
{
    Ifx_EVADC *evadc;       /**< \brief Specifies the pointer to the EVADC module registers */
} IfxEvadc_Adc;

/** \brief Gating/Trigger configuration structure
 */
typedef struct
{
    IfxEvadc_GatingSource  gatingSource;        /**< \brief Specifies used gate input for group */
    IfxEvadc_TriggerSource triggerSource;       /**< \brief Specifies used Trigger input for group */
    IfxEvadc_GatingMode    gatingMode;          /**< \brief Specifies gating mode. High level, Low Level  or Gating disabled */
    IfxEvadc_TriggerMode   triggerMode;         /**< \brief Specifies trigger mode. Rising, falling any edge leads to an trigger event */
} IfxEvadc_Adc_GatingTriggerConfig;

/** \} */

/** \addtogroup IfxLld_Evadc_Adc_DataStructures
 * \{ */
/** \brief Arbiter configuration structure.
 */
typedef struct
{
    IfxEvadc_ArbitrationRounds arbiterRoundLength;             /**< \brief Specifies arbiter round length. */
    boolean                    requestSlotQueue0Enabled;       /**< \brief request queue0 if enabled. */
    boolean                    requestSlotQueue1Enabled;       /**< \brief request queue1 if enabled. */
    boolean                    requestSlotQueue2Enabled;       /**< \brief request queue2 if enabled. */
} IfxEvadc_Adc_ArbiterConfig;

/** \brief Input class configuration structure
 */
typedef struct
{
    float32                                                sampleTime;                                /**< \brief Specifies the requested sample time for input class */
    IfxEvadc_ChannelNoiseReduction                         conversionMode;                            /**< \brief Specifies the conversion Mode, noise reduction levels */
    IfxEvadc_SpreadEarlySamplePointStandardConversionsMode samplePointStandard;
    IfxEvadc_AnalogInputPrechargeControl                   analogInputPrechargeControlStandard;
    float32                                                sampleTimeEMUX;
    IfxEvadc_ChannelNoiseReductionEMUX                     conversionModeEMUX;
    IfxEvadc_SpreadEarlySamplePointEMUXConversionsMode     samplePointEMUX;
    IfxEvadc_AnalogInputPrechargeControl                   analogInputPrechargeControlEMUX;
} IfxEvadc_Adc_ClassConfig;

typedef struct
{
    IfxEvadc_Emux_Out *pins[3];          /**< \brief Emux Pins configuration */
    IfxPort_OutputMode outputMode;       /**< \brief the pin output mode which should be configured */
    IfxPort_PadDriver  padDriver;        /**< \brief Pad driver */
} IfxEvadc_Adc_EmuxPinConfig;

/** \brief Group handle data structure
 */
typedef struct
{
    IfxEvadc_Adc     module;        /**< \brief The EVADC handle structure */
    Ifx_EVADC_G     *group;         /**< \brief Pointer to the group registers */
    IfxEvadc_GroupId groupId;       /**< \brief Specifies the group index */
} IfxEvadc_Adc_Group;

/** \brief Queue configuration structure
 */
typedef struct
{
    boolean                          flushQueueAfterInit;        /**< \brief Specifies if the queue is flushed after configuration */
    IfxEvadc_Adc_GatingTriggerConfig triggerConfig;              /**< \brief trigger and gating configuration. */
    IfxEvadc_RequestSlotPriority     requestSlotPrio;            /**< \brief priority of used  queue request slot. */
    IfxEvadc_RequestSlotStartMode    requestSlotStartMode;       /**< \brief start mode for request queue source. */
} IfxEvadc_Adc_QueueConfig;

/** \} */

/** \addtogroup IfxLld_Evadc_Adc_DataStructures
 * \{ */
/** \brief Channel handle data structure
 */
typedef struct
{
    IfxEvadc_ChannelId            channel;         /**< \brief Specifies the channel index */
    IfxEvadc_ChannelResult        resultreg;       /**< \brief Specifies allocated result register */
    IFX_CONST IfxEvadc_Adc_Group *group;           /**< \brief Specifies the group of the channel */
} IfxEvadc_Adc_Channel;

/** \brief Channel configuration structure
 */
typedef struct
{
    boolean                       globalResultUsage;              /**< \brief Specifies storage in global result register */
    boolean                       synchonize;                     /**< \brief Specifies synchronized conversion channel */
    boolean                       rightAlignedStorage;            /**< \brief Specifies result is right aligned */
    Ifx_Priority                  resultPriority;                 /**< \brief Interrupt priority of the result trigger interrupt, if 0 the interrupt is disable */
    Ifx_Priority                  channelPriority;                /**< \brief Interrupt priority of the channel trigger interrupt, if 0 the interrupt is disable */
    IfxSrc_Tos                    resultServProvider;             /**< \brief Interrupt service provider for the result trigger interrupt */
    IfxSrc_Tos                    channelServProvider;            /**< \brief Interrupt service provider for the channel trigger interrupt */
    IfxEvadc_SrcNr                resultSrcNr;                    /**< \brief Service node of the result trigger */
    IfxEvadc_SrcNr                channelSrcNr;                   /**< \brief Service node of the channel trigger */
    IfxEvadc_ChannelId            channelId;                      /**< \brief Specifies the channel index */
    IfxEvadc_InputClasses         inputClass;                     /**< \brief Specifies input class selection */
    IfxEvadc_ChannelReference     reference;                      /**< \brief Specifies Reference selection */
    IfxEvadc_ChannelResult        resultRegister;                 /**< \brief Specifies Result register selection */
    IfxEvadc_BoundarySelection    lowerBoundary;                  /**< \brief Specifies lower boundary selection */
    IfxEvadc_BoundarySelection    upperBoundary;                  /**< \brief Specifies upper boundary selection */
    IfxEvadc_BoundaryExtension    boundaryMode;                   /**< \brief Specifies Standard mode of fast compare mode */
    IfxEvadc_LimitCheck           limitCheck;                     /**< \brief Specifies boundary band selection upper/lower */
    IFX_CONST IfxEvadc_Adc_Group *group;                          /**< \brief Specifies pointer to the IfxEvadc_Adc_Group group handle */
    IfxEvadc_DataModificationMode dataModificationMode;           /**< \brief Specifies the Data Modification Mode.According to the value entered here, the meanings of values entered for dataReductionControlMode will mean either the number of results accumulated(GxRCRy.B.DMM=0) or filter coefficients(GxRCRy.B.DMM=1). */
    IfxEvadc_DataModificationMode dataReductionControlMode;       /**< \brief Depending on the valued of dataModificationMode, the meanings of values in this will differ.For details refer definition of IfxEvadc_DataReductionControlMode enum in IfxEvadc.h */
    IfxEvadc_WaitForRead          waitForReadMode;
    IfxEvadc_FifoMode             fifoMode;
} IfxEvadc_Adc_ChannelConfig;

/** \brief EVADC module configuration structure
 */
typedef struct
{
    Ifx_EVADC                         *evadc;                                                    /**< \brief Specifies the pointer to the EVADC module registers */
    float32                            digitalFrequency;                                         /**< \brief Specifies digital ADC Frequency */
    float32                            moduleFrequency;                                          /**< \brief ADC Module frequency */
    IfxEvadc_Adc_ClassConfig           globalInputClass[IFXEVADC_NUM_GLOBAL_INPUTCLASSES];       /**< \brief Specifies the global conversion settings one and two */
    IfxEvadc_AnalogClockGenerationMode analogClockGenerationMode;
    IfxEvadc_SupplyVoltageLevelControl supplyVoltage;
    IfxEvadc_StartupCalibration        startupCalibrationControl;
} IfxEvadc_Adc_Config;

/** \brief Emux Control Structure
 */
typedef struct
{
    Ifx_EVADC                       *evadc;                       /**< \brief Pointer to EVADC Module Space */
    IfxEvadc_ExternalMultiplexerMode mode;                        /**< \brief Specifies the External Multiplexer mode */
    IfxEvadc_EmuxSelectValue         startChannel;                /**< \brief specifies the external channel start value(EMUX[x:0])
                                                                   * x- specifies external channel number */
    IfxEvadc_EmuxCodingScheme        code;                        /**< \brief specifes binary/gray code */
    IfxEvadc_EmuxSampleTimeControl   sampleTimeControl;           /**< \brief specifies when to use sample time control */
    IfxEvadc_GroupId                 groupId;                     /**< \brief specifies groupId */
    uint8                            channels;                    /**< \brief specifies channel number */
    IfxEvadc_EmuxInterface           emuxInterface;               /**< \brief specifies the Emux interface */
    IfxEvadc_Adc_EmuxPinConfig       emuxOutPinConfig;            /**< \brief configure the emux output pin */
    IfxEvadc_ChannelSelectionStyle   channelSelectionStyle;       /**< \brief External Multiplexer Channel Selection Style */
} IfxEvadc_Adc_EmuxControl;

/** \brief Group configuration structure
 */
typedef struct
{
    IFX_CONST IfxEvadc_Adc                  *module;                                      /**< \brief Specifies pointer to the IfxEvadc_Adc module handle */
    IfxEvadc_GroupId                         groupId;                                     /**< \brief Specifies the group/kernel id */
    IfxEvadc_GroupId                         master;                                      /**< \brief Specifies the master group. If master is different from groupId, then the group is configured as slave. */
    IfxEvadc_Adc_ClassConfig                 inputClass[IFXEVADC_NUM_INPUTCLASSES];       /**< \brief Specifies conversion settings one and two */
    IfxEvadc_Adc_QueueConfig                 queueRequest[3];                             /**< \brief Specifies queue0 mode configuration */
    IfxEvadc_Adc_ArbiterConfig               arbiter;                                     /**< \brief Arbiter configuration structure. */
    float32                                  analogFrequency;                             /**< \brief specifies analog frequency in Hz */
    boolean                                  startupCalibration;                          /**< \brief Can be enabled to execute a startup calibration (disabled by default). */
    boolean                                  disablePostCalibration;                      /**< \brief Specifies if calibration after conversion (post calibration) should be disabled */
    IfxEvadc_InputClasses                    inputClasses;
    boolean                                  doubleClockForMSBConversionSelection;
    IfxEvadc_SampleSynchronization           sampleSynchronizationEnabled;
    IfxEvadc_AnalogClockSynchronizationDelay analogClockSynchronizationDelay;
    IfxEvadc_CalibrationSampleTimeControl    calibrationSampleTimeControlMode;
    IfxEvadc_ReferencePrechargeControl       referencePrechargeControlMode;
    boolean                                  referencePrechargeEnabled;
    boolean                                  inputBufferEnabled;
    IfxEvadc_IdlePrecharge                   idlePrechargeLevel;                          /**< \brief Voltage level to which sampling capacitor will be precharged when idle. */
    IfxEvadc_AnalogConverterMode             analogConverterMode;
} IfxEvadc_Adc_GroupConfig;

/** \} */

/** \addtogroup IfxLld_Evadc_Adc_FastCompare
 * \{ */
typedef struct
{
    IfxEvadc_Adc     module;                     /**< \brief The EVADC handle structure */
    Ifx_EVADC_FC    *fastCompareChannel;
    IfxEvadc_GroupId fastCompareChannelId;       /**< \brief Following values can be taken for fastCompareChannelId:
                                                  *
                                                  *     IfxEvadc_GroupId_12 for Fast compare channel 0
                                                  *     IfxEvadc_GroupId_13 for Fast compare channel 1
                                                  *     IfxEvadc_GroupId_14 for Fast compare channel 2
                                                  *     IfxEvadc_GroupId_15 for Fast compare channel 3
                                                  *     IfxEvadc_GroupId_16 for Fast compare channel 4
                                                  *     IfxEvadc_GroupId_17 for Fast compare channel 5
                                                  *     IfxEvadc_GroupId_18 for Fast compare channel 6
                                                  *     IfxEvadc_GroupId_19 for Fast compare channel 7 */
} IfxEvadc_Adc_FastCompareChannel;

/** \brief Configuring Fast Compare channel operations
 */
typedef struct
{
    IfxEvadc_Adc                                       *module;                                /**< \brief Pointer to EVADC module handle */
    IfxEvadc_GroupId                                    fastCompareChannelId;
    IfxEvadc_BoundaryFlagActivationMode                 boundaryFlagActivation;                /**< \brief Selects the behaviour of BoundaryFlag in FCBFL.B.BFA */
    IfxEvadc_BoundaryFlagInversionControl               boundaryFlagInversion;                 /**< \brief Select whether to use Boundary Flag(FCM.B.BFL) directly or in a inverted format */
    IfxEvadc_BoundaryFlagNodePointer                    boundaryFlagNodePointer;               /**< \brief Options for FCM.B.BFLNP to select Boundary Flag Node Pointer */
    IfxEvadc_BoundaryFlagSwControl                      boundaryFlagAction;                    /**< \brief Action to be performed on the boundary flag(BFL) */
    IfxEvadc_ChannelEventMode                           channelEventMode;                      /**< \brief Channel Event Mode for configuring generation of events for Fast Compare channels in FCCTRL.B.CHEVMODE */
    IfxEvadc_ClockDividerFactor                         clockDivider;                          /**< \brief Defines the frequency of the analog converter clock f_ADCI (base clock for conversion steps), derived from the peripheral clock. */
    IfxEvadc_ExternalTriggerPolarity                    externalTriggerPolarity;               /**< \brief External Trigger Polarity to be set in FCCTRL.B.XTPOL */
    IfxEvadc_FastCompareAnalogClockSynchronizationDelay delay;                                 /**< \brief Defines the delay of the analog clock in clocks after the sync signal in FCM.B.ACSD */
    IfxEvadc_FastCompareAnalogConverterControl          analogConverterControllerMode;         /**< \brief Settings for Analog Coverter Controller to be done in FCM.B.ANON */
    IfxEvadc_FastCompareAutomaticUpdate                 automaticUpdateMode;                   /**< \brief Defines the source of the value(s) in bitfield FCM.B.FCREF */
    IfxEvadc_FastCompareRunControl                      runMode;
    IfxEvadc_RampRunControl                             rampGenerationMode;                    /**< \brief Defines the run conditions for the ramp generation in FCM.B.RUNRAMP */
    IfxEvadc_FastCompareServiceRequestGeneration        serviceRequestGenerationEvent;         /**< \brief Criteria/mode settings for service request generation in FCM.B.SRG */
    IfxEvadc_GateOperatingMode                          gateOperatingMode;                     /**< \brief Gate Operating Mode to be filled in FCCTRL.B.GTMODE */
    IfxEvadc_RampDirection                              rampDirection;                         /**< \brief Ramp direction setting in FCM.B.FCRDIR */
    IfxEvadc_SampleSynchronization                      sampleTimingSynchronization;           /**< \brief Sample timing sync settings for FCM.B.SSE */
    IfxEvadc_TriggerOperatingMode                       externalTriggerOperatingMode;          /**< \brief Trigger operating mode in Fast Compare channels, to be entered in FCCTRL.B.XTMODE */
    uint8                                               additionalClockCycles;                 /**< \brief Number of additional clock cycles to be added to the minimum sample phase of 2 analog clock cycles. */
    IfxEvadc_ReferencePrechargeControl                  referenceInputPrechargeDuration;       /**< \brief Precharge duration for the reference input */
    IfxEvadc_AnalogInputPrechargeControl                analogInputPrechargeDuration;          /**< \brief Precharge duration for the analog input */
    uint8                                               triggerInterval;                       /**< \brief Defines the interval at which fast compare operations are triggered in steps of 16 * 1/f_ADC */
    uint16                                              referenceValue;                        /**< \brief The input level is compared to this value.
                                                                                                * The resulting reference level is (V_AREF / 1024) * FCM.B.FCREF */
    boolean                                             boundaryFlagValue;                     /**< \brief Defines the logic value that replaces the compare result while the gate input is inactive (low) in lock mode. */
} IfxEvadc_Adc_FastCompareChannelConfig;

/** \} */

/** \addtogroup IfxLld_Evadc_Adc_Module
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Reset the EVADC module
 * \param evadc pointer to the EVADC module
 * \return None
 *
 * Example Usage :\ref IfxLld_Evadc_Adc_Usage
 *
 */
IFX_INLINE void IfxEvadc_Adc_deInitModule(IfxEvadc_Adc *evadc);

/** \brief Get the current EVADC configuration (e.g. EVADC frequency)
 * \param evadc pointer to the EVADC module
 * \param config pointer to the EVADC module configuration
 * \param evadcG Pointer to the register Group
 * \return None
 *
 * Example Usage :\ref IfxLld_Evadc_Adc_Usage
 *
 */
IFX_INLINE void IfxEvadc_Adc_getModuleConfig(IfxEvadc_Adc *evadc, IfxEvadc_Adc_Config *config, Ifx_EVADC_G *evadcG);

/** \brief Get conversion result based on the Request Source. (Function does not care about the alignment)
 * value = raw * gain + offset
 * \param group pointer to the EVADC group
 * \param channel channel number
 * \param sourceType type of request source
 * \return scaled Conversion result
 *
 * \code
 *
 * // IMPORTANT: for deterministic results we have to disable the queue gate
 * // while filling the queue, otherwise results could be output in the wrong order
 * unsigned savedGate = adcGroup.module.evadc->G[adcGroup.groupId].Queue0.QMR0.B.ENGT;
 * adcGroup.module.evadc->G[adcGroup.groupId].Queue0.QMR0.B.ENGT = 0;
 *
 * // create channel config
 * IfxEvadc_Adc_ChannelConfig adcChannelConfig[3];
 * IfxEvadc_Adc_Channel adcChannel[3];
 *
 * for(int chnIx=0; chnIx<3; ++chnIx) {
 * IfxEvadc_Adc_initChannelConfig(&adcChannelConfig[chnIx], &adcGroup);
 *
 * adcChannelConfig[chnIx].channelId = (IfxEvadc_ChannelId)(chnIx);
 * adcChannelConfig[chnIx].resultRegister = IfxEvadc_ChannelResult_1; // use result register #1 for all channels
 *
 * // initialize the channel
 * IfxEvadc_Adc_initChannel(&adcChannel[chnIx], &adcChannelConfig[chnIx]);
 *
 * // Add channel to queue with refill enabled
 * IfxEvadc_Adc_addToQueue(&adcChannel[chnIx], IFXEVADC_QUEUE_REFILL,IfxEvadc_RequestSource_queue0);
 *
 * // restore previous gate config
 * adcGroup.module.evadc->G[adcGroup.groupId].Queue0.QMR0.B.ENGT = savedGate;
 *
 * // start the Queue
 * IfxEvadc_Adc_startQueue(&adcGroup,IfxEvadc_RequestSource_queue0); // just for the case that somebody copy&pastes the code - the queue has already been started in previous test
 *
 * // wait for valid result
 * Ifx_EVADC_G_RES resultChannel;
 * do {
 * resultChannel = IfxEvadc_Adc_getResultBasedOnRequestSource(&adcGroup, IfxEvadc_ChannelId2, IfxEvadc_RequestSource_queue0);
 * } while( !resultChannel.B.VF );
 * \endcode
 *
 */
IFX_INLINE Ifx_EVADC_G_RES IfxEvadc_Adc_getResultBasedOnRequestSource(IfxEvadc_Adc_Group *group, IfxEvadc_ChannelId channel, IfxEvadc_RequestSource sourceType);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Initialise the EVADC to run with the expected frequency and calibration
 * \param evadc pointer to the EVADC handle
 * \param config pointer to the EVADC configuration
 * \return IfxEvadc_Status
 *
 * For coding example see: \ref IfxLld_Evadc_Adc_Usage
 *
 */
IFX_EXTERN IfxEvadc_Status IfxEvadc_Adc_initModule(IfxEvadc_Adc *evadc, const IfxEvadc_Adc_Config *config);

/** \brief Initialise buffer with default EVADC configuration
 * \param config pointer to the EVADC module configuration
 * \param evadc pointer to the EVADC
 * \return None
 *
 * For coding example see: \ref IfxLld_Evadc_Adc_Usage
 *
 */
IFX_EXTERN void IfxEvadc_Adc_initModuleConfig(IfxEvadc_Adc_Config *config, Ifx_EVADC *evadc);

/** \} */

/** \addtogroup IfxLld_Evadc_Adc_Group
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Gets the current group module register address
 * \param group Group handle data structure
 * \return Group module register base address
 */
IFX_INLINE Ifx_EVADC *IfxEvadc_Adc_getEvadcFromGroup(const IfxEvadc_Adc_Group *group);

/** \brief Gets the current group register set
 * \param group Group handle data structure
 * \return Group register set
 */
IFX_INLINE Ifx_EVADC_G *IfxEvadc_Adc_getGroupRegsFromGroup(const IfxEvadc_Adc_Group *group);

/** \brief Get conversion result for the group
 * \param group pointer to the EVADC group
 * \param results pointer to scaled conversion results
 * \param resultOffset offset for the first result
 * \param numResults number of results
 * \return None
 *
 * \code
 * // create configuration
 * IfxEvadc_Adc_Config adcConfig;
 * IfxEvadc_Adc_initModuleConfig(&adcConfig, &MODULE_EVADC);
 *
 * // initialize module
 * IfxEvadc_Adc evadc;
 * IfxEvadc_Adc_initModule(&evadc, &adcConfig);
 *
 * // create group config
 * IfxEvadc_Adc_GroupConfig adcGroupConfig;
 * IfxEvadc_Adc_initGroupConfig(&adcGroupConfig, &evadc);
 *
 * // change group (default is GroupId0, change to GroupId2)
 * adcGroupConfig.groupId = IfxEvadc_GroupId2;
 * adcGroupConfig.master = adcGroupConfig.groupId;
 *
 * // enable gate in "always" mode (no edge detection)
 * adcGroupConfig.queueRequest[0].triggerConfig.gatingMode = IfxEvadc_GatingMode_always;
 *
 * // enable all arbiter request sources
 * adcGroupConfig.arbiter.requestSlotQueue0Enabled           = TRUE; // enable Queue0 mode
 * adcGroupConfig.arbiter.requestSlotQueue1Enabled           = TRUE; // enable Queue1 mode
 * adcGroupConfig.arbiter.requestSlotQueue2Enabled           = TRUE; // enable Queue2 mode
 *
 *     // initialize the group
 *     IfxEvadc_Adc_Group adcGroup;
 *     IfxEvadc_Adc_initGroup(&adcGroup, &adcGroupConfig);
 *
 *   // create channel config
 *         IfxEvadc_Adc_ChannelConfig adcChannelConfig;
 *         IfxEvadc_Adc_initChannelConfig(&adcChannelConfig, &adcGroup);
 *
 *
 *         // change channel (default is ChannelId_0, change to ChannelId_3)
 *         adcChannelConfig.channelId = IfxEvadc_ChannelId_3;
 *
 *         // initialize the channel
 *         IfxEvadc_Adc_Channel adcChannel;
 *         IfxEvadc_Adc_initChannel(&adcChannel, &adcChannelConfig);
 *
 *         // Add channel to queue with refill enabled
 *         IfxEvadc_Adc_addToQueue(&adcChannel, IfxEvadc_RequestSource_queue0, IFXEVADC_QUEUE_REFILL);
 *
 *         // start the Queue
 *         IfxEvadc_Adc_startQueue(&adcGroup, IfxEvadc_RequestSource_queue0);
 *
 *
 *         // get 10 results
 *         for(int i=0; i<10; ++i) {
 *             unsigned group = adcChannel.group->groupId;
 *             unsigned channel = adcChannel.channel;
 *
 *
 * // fetch the 2 results of conversion for group 0
 * Ifx_EVADC_G_RES results[10];
 * IfxEvadc_Adc_getGroupResult(&adcGroup, results, 0, 2);
 * \endcode
 *
 */
IFX_INLINE void IfxEvadc_Adc_getGroupResult(IfxEvadc_Adc_Group *group, Ifx_EVADC_G_RES *results, uint32 resultOffset, uint32 numResults);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Reset the EVADC group
 * \param group pointer to the EVADC group
 * \return None
 *
 * Example Usage :\ref IfxLld_Evadc_Adc_Usage
 *
 */
IFX_EXTERN void IfxEvadc_Adc_deInitGroup(IfxEvadc_Adc_Group *group);

/** \brief Get the current group configuration (e.g. evadc frequency)
 * \param group pointer to the EVADC group
 * \param config pointer to the EVADC group configuration
 * \return None
 *
 * Example Usage :\ref IfxLld_Evadc_Adc_Usage
 *
 */
IFX_EXTERN void IfxEvadc_Adc_getGroupConfig(IfxEvadc_Adc_Group *group, IfxEvadc_Adc_GroupConfig *config);

/** \brief Initialise the EVADC group (also  queue modes) Slave Groups must initialize first.
 * \param group Pointer to the EVADC group
 * \param config pointer to the EVADC group configuration
 * \return IfxEvadc_Status
 *
 * For coding example see: \ref IfxLld_Evadc_Adc_Usage
 *
 */
IFX_EXTERN IfxEvadc_Status IfxEvadc_Adc_initGroup(IfxEvadc_Adc_Group *group, const IfxEvadc_Adc_GroupConfig *config);

/** \brief Initialise buffer with default EVADC configuration
 * \param config pointer to the EVADC group configuration
 * \param evadc Pointer to the EVADC module
 * \return None
 *
 * For coding example see: \ref IfxLld_Evadc_Adc_Usage
 *
 */
IFX_EXTERN void IfxEvadc_Adc_initGroupConfig(IfxEvadc_Adc_GroupConfig *config, IfxEvadc_Adc *evadc);

/** \} */

/** \addtogroup IfxLld_Evadc_Adc_Channel
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief pointer to the EVADC channel
 * \param channel pointer to the EVADC channel
 * \param waitForReadMode wait for read mode enabled/disable
 * \return None
 *
 * For coding example see: \ref IfxEvadc_Adc_getResultBasedOnRequestSource
 *
 */
IFX_INLINE void IfxEvadc_Adc_configureWaitForReadMode(IfxEvadc_Adc_Channel *channel, boolean waitForReadMode);

/** \brief Get conversion result (Function does not care about the alignment)
 * \param channel pointer to the EVADC channel
 * \return scaled Conversion result
 *
 * For coding example see: \ref IfxLld_Evadc_Adc_Usage
 *
 */
IFX_INLINE Ifx_EVADC_G_RES IfxEvadc_Adc_getResult(IfxEvadc_Adc_Channel *channel);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Get the current channel configuration (e.g. sample settings)
 * \param channel pointer to the EVADC channel
 * \param config pointer to the EVADC channel configuration
 * \return None
 *
 * Example Usage :\ref IfxLld_Evadc_Adc_Usage
 *
 */
IFX_EXTERN void IfxEvadc_Adc_getChannelConfig(IfxEvadc_Adc_Channel *channel, IfxEvadc_Adc_ChannelConfig *config);

/** \brief Initialise one channel with given configuration
 * \param channel pointer to the EVADC channel
 * \param config pointer to the EVADC channel configuration
 * \return IfxEvadc_Status
 *
 * For coding example see: \ref IfxLld_Evadc_Adc_Usage
 *
 */
IFX_EXTERN IfxEvadc_Status IfxEvadc_Adc_initChannel(IfxEvadc_Adc_Channel *channel, const IfxEvadc_Adc_ChannelConfig *config);

/** \brief Initialise buffer with default channel configuration
 * \param config pointer to the EVADC channel configuration
 * \param group pointer to the EVADC group
 * \return None
 *
 * For coding example see: \ref IfxLld_Evadc_Adc_Usage
 *
 */
IFX_EXTERN void IfxEvadc_Adc_initChannelConfig(IfxEvadc_Adc_ChannelConfig *config, const IfxEvadc_Adc_Group *group);

/** \} */

/** \addtogroup IfxLld_Evadc_Adc_Queue
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Add an entry to the queue of a group for the specified channel with the following options set:
 *  refill
 *  source interrupt enable/disable
 * external trigger control
 * \param channel pointer to the EVADC channel
 * \param sourceType Request source Type(Queue 0,Queue 1,Queue 2)
 * \param options options for channel
 * \return None
 *
 * For coding example see: \ref IfxEvadc_Adc_getResult
 *
 */
IFX_INLINE void IfxEvadc_Adc_addToQueue(IfxEvadc_Adc_Channel *channel, IfxEvadc_RequestSource sourceType, uint32 options);

/** \brief Flush the contents of the queue of a group
 * \param group pointer to the EVADC group
 * \param sourceType Request source Type(Queue 0,Queue 1,Queue 2)
 * \return None
 *
 * For coding example see: \ref IfxEvadc_Adc_getResult
 *
 */
IFX_INLINE void IfxEvadc_Adc_clearQueue(IfxEvadc_Adc_Group *group, IfxEvadc_RequestSource sourceType);

/** \brief Gives the status of the Queue of a group by returning non zero value if the Queue is full
 * \param group pointer to the EVADC group
 * \param sourceType Request source Type(Queue 0,Queue 1,Queue 2)
 * \return Queue status
 *
 * For coding example see: \ref IfxEvadc_Adc_getResult
 *
 */
IFX_INLINE IfxEvadc_Status IfxEvadc_Adc_getQueueStatus(IfxEvadc_Adc_Group *group, IfxEvadc_RequestSource sourceType);

/** \brief Starts a queue of a group by generating a trigger event through software
 * \param group pointer to the EVADC group
 * \param sourceType Request source Type(Queue 0,Queue 1,Queue 2)
 * \return None
 *
 * For coding example see: \ref IfxEvadc_Adc_getResult
 *
 */
IFX_INLINE void IfxEvadc_Adc_startQueue(IfxEvadc_Adc_Group *group, IfxEvadc_RequestSource sourceType);

/** \} */

/** \addtogroup IfxLld_Evadc_Adc_Global
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief access function to enable/disable  wait for read mode for global result register
 * \param evadc pointer to the EVADC module
 * \param waitForReadMode wait for read mode enabled/disabled
 * \return None
 *
 * For coding example see: \ref IfxEvadc_Adc_getGlobalResult
 *
 */
IFX_INLINE void IfxEvadc_Adc_configureWaitForReadModeForGlobalResultRegister(IfxEvadc_Adc *evadc, boolean waitForReadMode);

/** \brief returns result stored in global result register
 * \param evadc pointer to the EVADC module
 * \return global result register
 *
 * \code
 * // create configuration
 * IfxEvadc_Adc_Config adcConfig;
 * IfxEvadc_Adc_initModuleConfig(&adcConfig, &MODULE_EVADC);
 *
 * // initialize module
 * IfxEvadc_Adc evadc;
 * IfxEvadc_Adc_initModule(&evadc, &adcConfig);
 *
 * // create group config
 * IfxEvadc_Adc_GroupConfig adcGroupConfig;
 * IfxEvadc_Adc_initGroupConfig(&adcGroupConfig, &evadc);
 *
 * // change group (default is GroupId_0, change to GroupId_3)
 * adcGroupConfig.groupId = IfxEvadc_GroupId_3;
 *
 * // IMPORTANT: usually we use the same group as master!
 * adcGroupConfig.master = adcGroupConfig.groupId;
 *
 * // initialize the group
 * IfxEvadc_Adc_Group adcGroup;
 * IfxEvadc_Adc_initGroup(&adcGroup, &adcGroupConfig);
 *
 * // create channel config
 * IfxEvadc_Adc_ChannelConfig adcChannelConfig2;
 * IfxEvadc_Adc_initChannelConfig(&adcChannelConfig2, &adcGroup);
 * adcChannelConfig2.backgroundChannel   = TRUE;
 * adcChannelConfig2.globalResultUsage   = TRUE;
 *
 * // change channel (default is ChannelId0, change to ChannelId2)
 * adcChannelConfig2.channelId = IfxEvadc_ChannelId2;
 *
 * // initialize the channel
 * IfxEvadc_Adc_Channel adcChannel2;
 * IfxEvadc_Adc_initChannel(&adcChannel2, &adcChannelConfig2);
 *
 * // create channel config
 * IfxEvadc_Adc_ChannelConfig adcChannelConfig5;
 * IfxEvadc_Adc_initChannelConfig(&adcChannelConfig5, &adcGroup);
 * adcChannelConfig5.backgroundChannel   = TRUE;
 * adcChannelConfig5.globalResultUsage   = TRUE;
 *
 * // change channel (default is ChannelId0, change to ChannelId5)
 * adcChannelConfig5.channelId = IfxEvadc_ChannelId5;
 *
 *
 * // initialize the channel
 * IfxEvadc_Adc_Channel adcChannel5;
 * IfxEvadc_Adc_initChannel(&adcChannel5, &adcChannelConfig5);
 *
 * uint32 channels = (1 << 5) | (1 << 2); // enable channel #5 and #2
 * uint32 mask = (1 << 7) | (1 << 5) | (1 << 2); // modify the selection for channel #7, #5 and #2; channel #7 will be disabled
 *
 * //configure wait for read mode
 * IfxEvadc_Adc_configureWaitForReadModeForGlobalResultRegister(&evadc, TRUE);
 *
 *      // Add channel to queue with refill enabled
 *             IfxEvadc_Adc_addToQueue(&adcChannel[chnIx],  IfxEvadc_RequestSource_queue0, IFXEVADC_QUEUE_REFILL);
 *
 *     // start the Queue
 *         IfxEvadc_Adc_startQueue(&adcGroup, IfxEvadc_RequestSource_queue0); // just for the case that somebody copy&pastes the code - the queue has already been started in previous test
 *
 *
 * // wait for valid result for channel 2
 * Ifx_EVADC_GLOB.RES resultChannel2;
 * do {
 * resultChannel2 = IfxEvadc_Adc_getGlobalResult(&evadc);
 * } while( !resultChannel2.B.VF );
 *
 * // wait for valid result for channel 5
 * Ifx_EVADC_GLOB.RES resultChannel5;
 * do {
 *     resultChannel5 = IfxEvadc_Adc_getGlobalResult(&evadc);
 * } while( !resultChannel5.B.VF );
 * \endcode
 *
 */
IFX_INLINE Ifx_EVADC_GLOB_RES IfxEvadc_Adc_getGlobalResult(IfxEvadc_Adc *evadc);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Configure global settings in the GLOBCFG register
 * \param evadc Pointer to EVADC SFRs
 * \param config pointer to the EVADC module configuration
 * \return None
 */
IFX_EXTERN void IfxEvadc_Adc_setGlobalConfigurations(Ifx_EVADC *evadc, const IfxEvadc_Adc_Config *config);

/** \} */

/** \addtogroup IfxLld_Evadc_Adc_Emux
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief initalise external multiplexer.
 * \param evadc Pointer to EVADC Module space
 * \param emuxControl speciifies EMUX configuration
 * \return None
 */
IFX_EXTERN void IfxEvadc_Adc_initExternalMultiplexerMode(Ifx_EVADC *evadc, const IfxEvadc_Adc_EmuxControl *emuxControl);

/** \brief initialise default configuration for external multiplexer
 * \param emuxConfig speciifies EMUX configuration
 * \param evadc pointer to EVADC module space
 * \return None
 */
IFX_EXTERN void IfxEvadc_Adc_initExternalMultiplexerModeConfig(IfxEvadc_Adc_EmuxControl *emuxConfig, Ifx_EVADC *evadc);

/** \} */

/** \addtogroup IfxLld_Evadc_Adc_FastCompare
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Initializing the Fast Compare channel.
 * \param fastCompareChannel Pointer to the EVADC Fast Compare Channel group
 * \param config Pointer to the EVADC Fast Compare Channel  configuration structure
 * \return None
 */
IFX_EXTERN void IfxEvadc_Adc_initFastCompareChannel(IfxEvadc_Adc_FastCompareChannel *fastCompareChannel, IfxEvadc_Adc_FastCompareChannelConfig *config);

/** \brief Initializing Fast Compare Channel config structure with default configurations
 * \param config pointer to the EVADC fast compare channel configuration structure
 * \param evadc Pointer to the EVADC module
 * \return None
 */
IFX_EXTERN void IfxEvadc_Adc_initFastCompareChannelConfig(IfxEvadc_Adc_FastCompareChannelConfig *config, IfxEvadc_Adc *evadc);

/** \} */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief set analog converter mode
 * \param evadc Pointer to module space
 * \param group Pointer to evadc group
 * \param analogConverterMode specifies analog convert mode
 * \return None
 */
IFX_EXTERN void IfxEvadc_Adc_setAnalogConvertControl(Ifx_EVADC *evadc, IfxEvadc_Adc_Group *group, IfxEvadc_AnalogConverterMode analogConverterMode);

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE void IfxEvadc_Adc_addToQueue(IfxEvadc_Adc_Channel *channel, IfxEvadc_RequestSource sourceType, uint32 options)
{
    IfxEvadc_addToQueue(channel->group->group, channel->channel, sourceType, options);
}


IFX_INLINE void IfxEvadc_Adc_clearQueue(IfxEvadc_Adc_Group *group, IfxEvadc_RequestSource sourceType)
{
    IfxEvadc_clearQueue(group->group, TRUE, sourceType);
}


IFX_INLINE void IfxEvadc_Adc_configureWaitForReadMode(IfxEvadc_Adc_Channel *channel, boolean waitForReadMode)
{
    IfxEvadc_configureWaitForReadMode(channel->group->group, channel->resultreg, waitForReadMode);
}


IFX_INLINE void IfxEvadc_Adc_configureWaitForReadModeForGlobalResultRegister(IfxEvadc_Adc *evadc, boolean waitForReadMode)
{
    IfxEvadc_configureWaitForReadModeForGlobalResultRegister(evadc->evadc, waitForReadMode);
}


IFX_INLINE void IfxEvadc_Adc_deInitModule(IfxEvadc_Adc *evadc)
{
    Ifx_EVADC *evadcSFR = evadc->evadc;

    IfxEvadc_resetModule(evadcSFR);
}


IFX_INLINE Ifx_EVADC *IfxEvadc_Adc_getEvadcFromGroup(const IfxEvadc_Adc_Group *group)
{
    return group->module.evadc;
}


IFX_INLINE Ifx_EVADC_GLOB_RES IfxEvadc_Adc_getGlobalResult(IfxEvadc_Adc *evadc)
{
    return IfxEvadc_getGlobalResult(evadc->evadc);
}


IFX_INLINE Ifx_EVADC_G *IfxEvadc_Adc_getGroupRegsFromGroup(const IfxEvadc_Adc_Group *group)
{
    return group->group;
}


IFX_INLINE void IfxEvadc_Adc_getGroupResult(IfxEvadc_Adc_Group *group, Ifx_EVADC_G_RES *results, uint32 resultOffset, uint32 numResults)
{
    IfxEvadc_getGroupResult(group->group, results, resultOffset, numResults);
}


IFX_INLINE void IfxEvadc_Adc_getModuleConfig(IfxEvadc_Adc *evadc, IfxEvadc_Adc_Config *config, Ifx_EVADC_G *evadcG)
{
    config->evadc                              = evadc->evadc;
    config->digitalFrequency                   = IfxEvadc_getAdcDigitalFrequency(evadc->evadc);
    config->globalInputClass[0].conversionMode = IfxEvadc_getGlobalConversionMode(evadc->evadc, 0);
    config->globalInputClass[1].conversionMode = IfxEvadc_getGlobalConversionMode(evadc->evadc, 1);
    config->moduleFrequency                    = IfxEvadc_getAdcModuleFrequency(evadc->evadc);
}


IFX_INLINE IfxEvadc_Status IfxEvadc_Adc_getQueueStatus(IfxEvadc_Adc_Group *group, IfxEvadc_RequestSource sourceType)
{
    return IfxEvadc_getQueueStatus(group->group, sourceType);
}


IFX_INLINE Ifx_EVADC_G_RES IfxEvadc_Adc_getResult(IfxEvadc_Adc_Channel *channel)
{
    return IfxEvadc_getResult(channel->group->group, channel->resultreg);
}


IFX_INLINE Ifx_EVADC_G_RES IfxEvadc_Adc_getResultBasedOnRequestSource(IfxEvadc_Adc_Group *group, IfxEvadc_ChannelId channel, IfxEvadc_RequestSource sourceType)
{
    return IfxEvadc_getResultBasedOnRequestSource(group->module.evadc, group->group, channel, sourceType);
}


IFX_INLINE void IfxEvadc_Adc_startQueue(IfxEvadc_Adc_Group *group, IfxEvadc_RequestSource sourceType)
{
    IfxEvadc_startQueue(group->group, sourceType);
}


#endif /* IFXEVADC_ADC_H */
