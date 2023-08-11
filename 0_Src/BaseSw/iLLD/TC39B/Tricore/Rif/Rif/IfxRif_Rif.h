/**
 * \file IfxRif_Rif.h
 * \brief RIF RIF details
 * \ingroup IfxLld_Rif
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
 * \defgroup IfxLld_Rif_Rif_Usage How to use the RIF RIF Interface driver?
 * \ingroup IfxLld_Rif
 *
 * The Rif interface driver provides a default RIF configuration for both internal and external ADC s use cases,
 *
 * All the configuration parameters can be changed/modified according to the need
 *
 * In the following sections it will be described, how to integrate the driver into the application framework.
 *
 * \section IfxLld_Rif_Rif_Preparation Preparation
 * \subsection IfxLld_Rif_Rif_Include Include Files
 *
 * Include following header file into your C code:
 * \code
 *     #include <Rif/Rif/IfxRif_Rif.h>
 * \endcode
 *
 * \subsection IfxLld_Rif_Rif_Variables Variables
 *
 * Declare the RIF module handle as global variable in your C code:
 *
 * \code
 *     // used globally
 *     static IfxRif_Rif rif0;
 * \endcode
 *
 * All the required interrupts can be configured for either Radar use case, internal or external ADCs in the interrupt configuration,
 *
 * \subsection IfxLld_Rif_Rif_Interrupt Interrupt Handler Installation
 *
 * See also \ref IfxLld_Cpu_Irq_Usage
 *
 * Define priorities for the Interrrupt handlers. This is normally done in the Ifx_IntPrioDef.h file:
 * \code
 *     // priorities are normally defined in Ifx_IntPrioDef.h
 *     #define IFX_INTPRIO_RIF0_INT  4
 *     #define IFX_INTPRIO_RIF0_ERR  8
 * \endcode
 *
 * Add the interrupt service routines to your C code. They have to call the RIF interrupt handlers by passing the rif module handle:
 * \code
 *     IFX_INTERRUPT(rif0INTISR, 0, IFX_INTPRIO_RIF0_INT)
 *     {
 *         IfxRif_Rif_isrInterrupt(&rif0);
 *     }
 *
 *     IFX_INTERRUPT(rif0ERRISR, 0, IFX_INTPRIO_RIF0_ERR)
 *     {
 *         IfxRif_Rif_isrError(&rif0);
 *     }
 * \endcode
 *
 * Finally install the interrupt handlers in your initialisation function:
 * \code
 *     // install interrupt handlers
 *     IfxCpu_Irq_installInterruptHandler(&rif0INTISR, IFX_INTPRIO_RIF0_INT);
 *     IfxCpu_Irq_installInterruptHandler(&rif0ERRISR, IFX_INTPRIO_RIF0_ERR);
 *     IfxCpu_enableInterrupts();
 * \endcode
 *
 * \subsection IfxLld_Rif_Rif_Init Module Initialisation
 *
 * The module initialisation can be done in the same function. Here an example:
 * \code
 *     // create module config
 *     IfxRif_Rif_Config rifConfig;
 *     IfxRif_Rif_initModuleConfig(&rifConfig, &MODULE_RIF0);
 *
 *     // choose the Type of ADC used and number of channels used
 *     rifConfig.connectedAdc = IfxRif_Adc_external;
 *     rifConfig.numOfChannels = 4;
 *
 *     // deserialiser configuration
 *     rifConfig.deserializer.clockPolarity = IfxRif_ClockPolarity_default;
 *     rifConfig.deserializer.framePolarity = IfxRif_FramePolarity_inverted;
 *     rifConfig.deserializer.dataPolarity0 = IfxRif_DataPolarity_default;
 *     rifConfig.deserializer.dataPolarity1 = IfxRif_DataPolarity_default;
 *     rifConfig.deserializer.dataPolarity2 = IfxRif_DataPolarity_default;
 *     rifConfig.deserializer.dataPolarity3 = IfxRif_DataPolarity_default;
 *     rifConfig.deserializer.calibrationEnable = FALSE;
 *
 *     // enable / disable CRC of input data
 *     rifConfig.data.crcEnable = TRUE;
 *
 *     // set frame Watchdog configuration
 *     rifConfig.fwdg.threshold = 128;
 *
 *     // Radar State machine Configuration
 *     rifConfig.rsm.numOfValidSamples    = 128;
 *
 *     // enable / disable synchronous start of two RIF modules
 *     rifConfig.rsm.lockstepEnable = FALSE;
 *
 *     // set LVDS configuration
 *     rifConfig.lvds.frameControl                 = IfxRif_LvdsPadControl_frameClock | IfxRif_LvdsPadControl_rterm;
 *     rifConfig.lvds.clockControl                  = IfxRif_LvdsPadControl_frameClock | IfxRif_LvdsPadControl_rterm;
 *     rifConfig.lvds.data0Control                 = IfxRif_LvdsPadControl_frameClock | IfxRif_LvdsPadControl_rterm;
 *     rifConfig.lvds.data1Control                 = IfxRif_LvdsPadControl_frameClock | IfxRif_LvdsPadControl_rterm;
 *     rifConfig.lvds.data2Control                 = IfxRif_LvdsPadControl_frameClock | IfxRif_LvdsPadControl_rterm;
 *     rifConfig.lvds.data3Control                 = IfxRif_LvdsPadControl_frameClock | IfxRif_LvdsPadControl_rterm;
 *     rifConfig.lvds.commonControl            = IfxRif_CommonLvdsPadControl_5vMode;
 *     rifConfig.lvds.rtermTrimmingValue      = 0x3;
 *     rifConfig.lvds.biasDistributorPowerDownMode = IfxRif_LvdsBiasDistributorMode_active;
 *     rifConfig.lvds.biasDistributor5VMode        = TRUE;
 *
 *     // set interrupt configuration based on the selected ADCs (internal / external)
 *     rifConfig.interrupt.intPriority = IFX_INTPRIO_RIF0_INT;
 *     rifConfig.interrupt.errPriority = IFX_INTPRIO_RIF0_ERR;
 *     rifConfig.interrupt.chirpEndEnable = TRUE;
 *     rifConfig.interrupt.crcErrorOnLine0Enable = TRUE;
 *
 *     //ifxRif_Rif rif0; //defined globally
 *     IfxRif_Rif_initModule(&rif0, &rifConfig);
 *
 * \endcode
 *
 * The RIF is ready for use now!
 *
 *
 * \section IfxLld_Rif_Rif_DataProcessing Data Processing
 *
 * \subsection IfxLld_Rif_Rif_StartDesrializer Start Deserializer
 *
 * The RIF driver provides dynamic control of start and stop of quad deserializer channels indiavidually based on the number of channels od ADC selected in the initialisation configuration
 *
 * This means: you can start and stop quad deserilizer unit when using external ADCs
 * \code
 *     IfxRif_Rif_startDeserializers(&rif0);
 * \endcode
 *
 * \code
 *     IfxRif_Rif_stopDeserializers(&rif0);
 * \endcode
 *
 * \subsection IfxLld_Rif_Rif_Status Status
 *
 * Status of all the necessary paramters of the current operation cycle independent of selected ADCs (internal / external ), can be found under the status structure of the module handle,
 * which include status of the interrupt flags, number of ramps and chirps
 * which will be updated by the respective ISRs
 *
 * Status of the previous Calibration can also be retrieved by the following
 *
 * \code
 *     IfxRif_Rif_getCalibrationStatus(&rif0);
 * \endcode
 *
 * \defgroup IfxLld_Rif_Rif RIF
 * \ingroup IfxLld_Rif
 * \defgroup IfxLld_Rif_Rif_DataStructures Data Structures
 * \ingroup IfxLld_Rif_Rif
 * \defgroup IfxLld_Rif_Rif_InitialisationFunctions Initialisation Functions
 * \ingroup IfxLld_Rif_Rif
 * \defgroup IfxLld_Rif_Rif_DeserializerFunctions Deserializer Functions
 * \ingroup IfxLld_Rif_Rif
 * \defgroup IfxLld_Rif_Rif_InterruptFunctions Interrupt Functions
 * \ingroup IfxLld_Rif_Rif
 */

#ifndef IFXRIF_RIF_H
#define IFXRIF_RIF_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "Rif/Std/IfxRif.h"

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Rif_Rif_DataStructures
 * \{ */
/** \brief Structure for data formating unit configuration
 */
typedef struct
{
    IfxRif_DataLength     length;               /**< \brief Input data length from deserializer to the quad processor */
    IfxRif_DataFormat     format;               /**< \brief Input data format from deserializer to the qaud processor */
    IfxRif_ShiftDirection shiftDirection;       /**< \brief Input data shift direction from deserializer to the quad processor */
    IfxRif_DataAlignment  alignment;            /**< \brief Output data alignment from the qaud processor to DMI */
    IfxRif_FlmMode        flmMode;              /**< \brief FLM mode of operation */
    IfxRif_FullSwapMode   fullSwapMode;         /**< \brief FLM mode of operation */
    boolean               crcEnable;            /**< \brief CRC enable / disable choice */
} IfxRif_Rif_DataConfig;

/** \brief Structure for deserializer configuration
 */
typedef struct
{
    IfxRif_ClockPolarity clockPolarity;           /**< \brief Polarity of the clock signal from deserializer */
    IfxRif_FramePolarity framePolarity;           /**< \brief Polarity of the frame signal from deserializer */
    IfxRif_DataPolarity  dataPolarity0;           /**< \brief Polarity of the data signal from deserializer */
    IfxRif_DataPolarity  dataPolarity1;           /**< \brief Polarity of the data signal from deserializer */
    IfxRif_DataPolarity  dataPolarity2;           /**< \brief Polarity of the data signal from deserializer */
    IfxRif_DataPolarity  dataPolarity3;           /**< \brief Polarity of the data signal from deserializer */
    boolean              calibrationEnable;       /**< \brief Calibration enable / disable choice */
} IfxRif_Rif_DeserializerConfig;

/** \brief Structure for interrupt configuration
 */
typedef struct
{
    boolean    calibrationEndEnable;              /**< \brief Calibration End interrupt enable / disable choice */
    boolean    frameWatchdogOverflowEnable;       /**< \brief Frame Watchdog Overflow interrupt enable / disable choice */
    boolean    rampEndEnable;                     /**< \brief Ramp End interrupt enable / disable choice */
    boolean    chirpEndEnable;                    /**< \brief Chirp End interrupt enable / disable choice */
    boolean    crcErrorOnLine0Enable;             /**< \brief CRC Error interrupt on Line 0 enable / disable choice */
    boolean    crcErrorOnLine1Enable;             /**< \brief CRC Error interrupt on Line 1 enable / disable choice */
    boolean    crcErrorOnLine2Enable;             /**< \brief CRC Error interrupt on Line 2 enable / disable choice */
    boolean    crcErrorOnLine3Enable;             /**< \brief CRC Error interrupt on Line 3 enable / disable choice */
    boolean    ramp1ErrorEnable;                  /**< \brief Ramp1 Error interrupt enable / disable choice */
    boolean    chirp1ErrorEnable;                 /**< \brief Chirp1 Error interrupt enable / disable choice */
    boolean    ramp1StartEnable;                  /**< \brief Ramp1 Start interrupt enable / disable choice */
    uint16     intPriority;                       /**< \brief Interrupt priority of the RIF module */
    uint16     errPriority;                       /**< \brief Error interrupt priority of the RIF module */
    IfxSrc_Tos typeOfService;                     /**< \brief Type of interrupt service */
} IfxRif_Rif_InterruptConfig;

/** \brief Structure for LVDS PAD control configuration
 */
typedef struct
{
    uint8                          frameControl;                       /**< \brief Frame LVDS Pad Control */
    uint8                          clockControl;                       /**< \brief Clock LVDS Pad Control */
    uint8                          data0Control;                       /**< \brief DATA0 LVDS Pad Control */
    uint8                          data1Control;                       /**< \brief DATA1 LVDS Pad Control */
    uint8                          data2Control;                       /**< \brief DATA2 LVDS Pad Control */
    uint8                          data3Control;                       /**< \brief DATA3 LVDS Pad Control */
    IfxRif_CommonLvdsPadControl    commonControl;                      /**< \brief Miscellaneous Common LVDS Pad Control */
    uint8                          rtermTrimmingValue;                 /**< \brief Termination Resistor Trimming value */
    IfxRif_LvdsBiasDistributorMode biasDistributorPowerDownMode;       /**< \brief LVDS Bias Distributor Power Down mode */
    boolean                        biasDistributor5VMode;              /**< \brief Enable / Disable 5V Mode for LVDS Bias Distributor */
} IfxRif_Rif_LvdsConfig;

/** \brief Structure for radar state machine configuration
 */
typedef struct
{
    boolean                    ramp1SignalEnable;         /**< \brief RAMP1 Signal enable /disable choice. */
    IfxRif_Ramp1SignalInput    ramp1SignalInput;          /**< \brief RAMP1 Signal input selection. */
    IfxRif_Ramp1SignalPolarity ramp1SignalPolarity;       /**< \brief RAMP1 Signal polarity. */
    uint8                      rampsPerChirp;             /**< \brief Number of ramps per chirp, in the range of 1 to 256 */
    uint16                     numOfValidSamples;         /**< \brief Number of valid samples, in the range of 1 to 2048 samples */
    boolean                    lockstepEnable;            /**< \brief Lockstep enable /disable choice, synchronous delivery of ADC samples from two RIFs */
} IfxRif_Rif_RadarStateMachineConfig;

/** \brief Structure for necessary status information
 */
typedef struct
{
    boolean calibrationEnd;        /**< \brief Calibration End state info */
    boolean chirpEnd;              /**< \brief End of the chirp status info */
    boolean crcErrorOnLine0;       /**< \brief CRC error status info on Line 0 */
    boolean crcErrorOnLine1;       /**< \brief CRC error status info on Line 1 */
    boolean crcErrorOnLine2;       /**< \brief CRC error status info on Line 2 */
    boolean crcErrorOnLine3;       /**< \brief CRC error status info on Line 3 */
    boolean crcNoError;            /**< \brief CRC no error status info on all lines */
    uint16  numOfRamps;            /**< \brief Number of ramps info */
    uint16  numOfChirps;           /**< \brief Number of chirps info */
    boolean ramp1Error;            /**< \brief RAMP1 Error status */
    boolean chirp1Error;           /**< \brief CHIRP1 Error status */
} IfxRif_Rif_Status;

/** \brief Structure for frame watchdog configuration
 */
typedef struct
{
    uint16 threshold;       /**< \brief Threshold value of the frame watchdog timer */
} IfxRif_Rif_frameWatchdogConfig;

/** \} */

/** \addtogroup IfxLld_Rif_Rif_DataStructures
 * \{ */
/** \brief Configuration structure of the module
 */
typedef struct
{
    Ifx_RIF                           *rif;                 /**< \brief Specifies the pointer to the base of RIF registers */
    IfxRif_Adc                         connectedAdc;        /**< \brief ADC used, internal / external */
    uint8                              numOfChannels;       /**< \brief Number of ADC channels used, Internal or External ADCs */
    IfxRif_Rif_DeserializerConfig      deserializer;        /**< \brief Structure for deserializer configuration */
    IfxRif_Rif_DataConfig              data;                /**< \brief Structure for data formating unit configuration */
    IfxRif_Rif_frameWatchdogConfig     fwdg;                /**< \brief Structure for frame watchdog configuration */
    IfxRif_Rif_RadarStateMachineConfig rsm;                 /**< \brief Structure for radar state machine configuration */
    IfxRif_Rif_LvdsConfig              lvds;                /**< \brief Structure for LVDS PAD control configuration */
    IfxRif_Rif_InterruptConfig         interrupt;           /**< \brief Structure for interrupt configuration */
} IfxRif_Rif_Config;

/** \} */

/** \brief Structure for module handle
 */
typedef struct
{
    Ifx_RIF          *rif;                 /**< \brief Specifies the pointer to the base of RIF registers */
    IfxRif_Adc        connectedAdc;        /**< \brief ADC used, internal / external */
    uint8             numOfChannels;       /**< \brief Number of ADC channels used, Internal or External ADCs */
    IfxRif_Rif_Status status;              /**< \brief Structure for necessary status information */
} IfxRif_Rif;

/** \addtogroup IfxLld_Rif_Rif_InitialisationFunctions
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Initialises the module with default configuration
 * \param rif Module handle
 * \param config Configuration structure of the module, will be filled with default values
 * \return None
 *
 * \code
 *     // create configuration
 *     IfxRif_Rif_Config rifConfig;
 *     IfxRif_Rif_initModuleConfig(&rifConfig, &MODULE_RIF0);
 *
 *     // change configuration
 *     rifConfig.connectedADC = IfxRif_Adc_external;
 *     rifConfig.numOfChannels = 4;
 *
 *     rifConfig.deserializer.calibrationEnable = TRUE;
 *
 *     rifConfig.data.crcEnable = TRUE;
 *
 *     rifConfig.fwdg.enable = TRUE;
 *     rifConfig.fwdg.threshold = 128;
 *
 *     rifConfig.rsm.lockstepEnable = FALSE;
 *
 *     rifConfig.interrupt.intPriority = 4;
 *     rifConfig.interrupt.errPriority = 8;
 *     rifConfig.interrupt.crcErrorOnLine0Enable = TRUE;
 *     rifConfig.interrupt.crcErrorOnLine1Enable = FALSE;
 *     rifConfig.interrupt.crcErrorOnLine2Enable = TRUE;
 *     rifConfig.interrupt.crcErrorOnLine3Enable = FALSE;
 *     rifConfig.interrupt.chirpEndEnable = TRUE;
 *
 *     \\ifxRif_Rif rif0; //defined globally
 *     IfxRif_Rif_initModule(&rif0, &rifConfig);
 *
 * \endcode
 *
 */
IFX_EXTERN void IfxRif_Rif_initModule(IfxRif_Rif *rif, IfxRif_Rif_Config *config);

/** \brief Fills the config structure with default values
 * \param config Configuration structure of the module, will be filled with default values
 * \param rif Specifies the pointer to the base of RIF registers
 * \return None
 *
 * A coding example can be found in \ref IfxRif_Rif_initModule
 *
 */
IFX_EXTERN void IfxRif_Rif_initModuleConfig(IfxRif_Rif_Config *config, Ifx_RIF *rif);

/** \} */

/** \addtogroup IfxLld_Rif_Rif_DeserializerFunctions
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Returns the status of the latest timing calibration sequence
 * \param rif Module handle
 * \return Calibration status (fail / success)
 *
 * \code
 *     \\IfxRif_Rif rif0; //defined globally
 *
 *     IfxRif_Rif_getCalibrationStatus(&rif0);
 * \endcode
 *
 */
IFX_INLINE IfxRif_CalibrationStatus IfxRif_Rif_getCalibrationStatus(IfxRif_Rif *rif);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Starts the quad deserializer, enables the individual deseralizers per selected ADC channels
 * \param rif Module handle
 * \return None
 *
 * \code
 *     \\IfxRif_Rif rif0; //defined globally
 *
 *     IfxRif_Rif_startDeserializers(&rif0);
 * \endcode
 *
 */
IFX_EXTERN void IfxRif_Rif_startDeserializers(IfxRif_Rif *rif);

/** \brief Stops the quad deserializer, disables the individual deseralizers per selected ADC channels
 * \param rif Module handle
 * \return None
 *
 * \code
 *     \\IfxRif_Rif rif0; //defined globally
 *
 *     IfxRif_Rif_stopDeserializers(&rif0);
 * \endcode
 *
 */
IFX_EXTERN void IfxRif_Rif_stopDeserializers(IfxRif_Rif *rif);

/** \} */

/** \addtogroup IfxLld_Rif_Rif_InterruptFunctions
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief ISR error routine
 * \param rif Module handle
 * \return None
 *
 * \code
 *     //define priorities for the Interrrupt handler
 *     #define IFX_INTPRIO_RIF0_ERR  8
 *
 *     //add the interrupt service routine
 *     IFX_INTERRUPT(rif0ERRISR, 0, IFX_INTPRIO_RIF0_ERR)
 *     {
 *         IfxRif_Rif_isrError(&rif0);
 *     }
 *
 *     // install interrupt handlers
 *     IfxCpu_Irq_installInterruptHandler(&rif0ERRISR, IFX_INTPRIO_RIF0_ERR);
 *     IfxCpu_enableInterrupts();
 * \endcode
 *
 */
IFX_EXTERN void IfxRif_Rif_isrError(IfxRif_Rif *rif);

/** \brief ISR interrupt routine
 * \param rif Module handle
 * \return None
 *
 * \code
 *     //define priorities for the Interrrupt handler
 *     #define IFX_INTPRIO_RIF0_INT  4
 *
 *     //add the interrupt service routine
 *     IFX_INTERRUPT(rif0INTISR, 0, IFX_INTPRIO_RIF0_INT)
 *     {
 *         IfxRif_Rif_isrInterrupt(&rif0);
 *     }
 *
 *     // install interrupt handlers
 *     IfxCpu_Irq_installInterruptHandler(&rif0INTISR, IFX_INTPRIO_RIF0_INT);
 *     IfxCpu_enableInterrupts();
 * \endcode
 *
 */
IFX_EXTERN void IfxRif_Rif_isrInterrupt(IfxRif_Rif *rif);

/** \} */

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE IfxRif_CalibrationStatus IfxRif_Rif_getCalibrationStatus(IfxRif_Rif *rif)
{
    return IfxRif_getCalibrationStatus(rif->rif);
}


#endif /* IFXRIF_RIF_H */
