/**
 * \file IfxCcu6_PwmBc.h
 * \brief CCU6 PWMBC details
 * \ingroup IfxLld_Ccu6
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
 * \defgroup IfxLld_Ccu6_PwmBc_Usage How to use the CCU6 PWMBC Interface driver?
 * \ingroup IfxLld_Ccu6_PwmBc
 *
 * The PWMBC interface driver provides a default CCU6 configuration specifically for running a Brush Less DC Motor using hall sensor inputs, by utilising both the timers
 *
 * After evaluating the hall input pattern at every T13 period match, new out pattern is given to the module after the phase delay at the T13 zero match
 *
 * User has to prepare the control table for the respective motor he/she is using,
 *
 * In the following sections it will be described, how to integrate the driver into the application framework.
 *
 *
 * \section IfxLld_Ccu6_PwmBc_Preparation Preparation
 *
 *
 * \subsection IfxLld_Ccu6_PwmBc_Include Include Files
 *
 * Include following header file into your C code:
 * \code
 *     #include <Ccu6/PwmBc/IfxCcu6_PwmBc.h>
 * \endcode
 *
 * \subsection IfxLld_Ccu6_PwmBc_Variables Variables
 *
 * Declare the PWMBC handle and the motor control table as global variables in your C code:
 *
 * \code
 *     // used globally
 *     static IfxCcu6_PwmBc pwmBc;
 *     // prepare the control table
 *     uint8 controlTable[6][3] = {{1, 5, 17} ,
 *                                 {2, 3, 36} ,
 *                                 {3, 1, 6} ,
 *                                 {4, 6, 18} ,
 *                                 {5, 4, 24} ,
 *                                 {6, 2, 9}};
 *     // to store value of the speed
 *     uint32 speed = 0;
 * \endcode
 *
 * \subsection IfxLld_Ccu6_PwmBc_Interrupt Interrupt Handler Installation
 *
 * See also \ref IfxLld_Cpu_Irq_Usage
 *
 * Define priority for the Interrrupt handler. This is normally done in the Ifx_IntPrioDef.h file:
 * \code
 *     // priorities are normally defined in Ifx_IntPrioDef.h
 *     #define IFX_INTPRIO_CCU6 1
 * \endcode
 *
 * Add the interrupt service routine to your C code.
 * \code
 *     IFX_INTERRUPT(ccu60ISR_PwmBc, 0, IFX_INTPRIO_CCU6)
 *     {
 *         //user code
 *     }
 * \endcode
 *
 * Finally install the interrupt handlers in your initialisation function:
 * \code
 *     // install interrupt handlers
 *     IfxCpu_Irq_installInterruptHandler(&ccu60ISR_PwmBc, IFX_INTPRIO_CCU6);
 *     IfxCpu_enableInterrupts();
 * \endcode
 *
 * \subsection IfxLld_Ccu6_PwmBc_Init Module Initialisation
 *
 * The module initialisation can be done in the same function. Here an example:
 *
 * \code
 *     // create configuration
 *     IfxCcu6_PwmBc_Config pwmBcConfig;
 *     IfxCcu6_PwmBc_initModuleConfig(&pwmBcConfig, &MODULE_CCU60);
 *
 *     // configure the frequencies of both the timers
 *     pwmBcConfig.base.t12Frequency = 400000;
 *     pwmBcConfig.base.t13Frequency = 400000;
 *
 *     // configure the periods of both the timers
 *     pwmBcConfig.base.t12Period = 100;
 *     pwmBcConfig.base.t13Period = 100;
 *
 *     // configure the phase delay and noise filter for hall input
 *     pwmBcConfig.base.phaseDelay = 20;
 *     pwmBcConfig.base.noiseFilter = 10;
 *
 *     // select the active state of the output
 *     pwmBcConfig.base.activeState = Ifx_ActiveState_high;
 *
 *     // configure timer12
 *     pwmBcConfig.timer12.countMode     = IfxCcu6_T12CountMode_edgeAligned;
 *     pwmBcConfig.timer12.counterValue     = 0;
 *
 *     // configure timer13
 *     pwmBcConfig.timer13.counterValue     = 0;
 *     pwmBcConfig.timer13.compareValue     = 100;
 *     // configure the syncronisation, in case of sync start with T12
 *     pwmBcConfig.timer13.t12SyncEvent     = IfxCcu6_T13TriggerEvent_onCC60RCompare;
 *     pwmBcConfig.timer13.t12SyncDirection = IfxCcu6_T13TriggerDirection_onT12CountingUp;
 *
 *     // configure hall sync event that starts the hall input pattern evaluation
 *     pwmBcConfig.hallSyncEvent       = IfxCcu6_HallSensorTriggerMode_t13PM,
 *
 *     // configure multi channel control
 *     pwmBcConfig.multiChannelControl.switchingSelect = IfxCcu6_MultiChannelSwitchingSelect_t12Channel1CompareMatch,
 *     pwmBcConfig.multiChannelControl.switchingSync   = IfxCcu6_MultiChannelSwitchingSync_t13ZeroMatch,
 *
 *     // pin configuration
 *     const IfxCcu6_PwmBc_Pins pins = {
 *         IfxCcu60_CC60_P02_0_OUT,	// CC60Out pin
 *         IfxCcu60_CC61_P02_2_OUT,	// CC61Out pin
 *         IfxCcu60_CC62_P02_4_OUT,	// CC62Out pin
 *         IfxCcu60_COUT60_P02_1_OUT,	// COUT60 pin
 *         IfxCcu60_COUT60_P02_3_OUT,	// COUT61 pin
 *         IfxCcu60_COUT60_P02_5_OUT,	// COUT62 pin
 *         NULL,	                        // COUT63 pin not used
 *
 *         IfxCcu60_COUT60_P02_6_OUT       // CCPOS0In pin
 *         IfxCcu60_COUT60_P02_7_OUT       // CCPOS1In pin
 *         IfxCcu60_COUT60_P02_8_OUT       // CCPOS2In pin
 *
 *         IfxPort_OutputMode_pushPull,
 *         IfxPort_PadDriver_cmosAutomotiveSpeed1,
 *         IfxPort_InputMode_pullUp
 *     };
 *     pwmBcConfig.pins = &pins;
 *
 *     // configure the interrupts
 *     pwmBcConfig.interrupt1.interruptSource = IfxCcu6_InterruptSource_cc61FallingEdge;
 *     pwmBcConfig.interrupt1.serviceRequest  = IfxCcu6_ServiceRequest_2;
 *     pwmBcConfig.interrupt1.priority        = IFX_INTRPRIO_CCU6;
 *     pwmBcConfig.interrupt1.typeOfService   = IfxSrc_Tos_cpu0;
 *
 *     // configure input and output triggers
 *     pwmBcConfig.trigger.t12ExtInputTrigger   = NULL_PTR;
 *     pwmBcConfig.trigger.t12ExtInputTriggerMode  = IfxCcu6_ExternalTriggerMode_risingEdge;
 *     pwmBcConfig.trigger.t13ExtInputTrigger   = NULL_PTR;
 *     pwmBcConfig.trigger.t13ExtInputTriggerMode  = IfxCcu6_ExternalTriggerMode_risingEdge;
 *     pwmBcConfig.trigger.t13InSyncWithT12     = FALSE;
 *
 *     pwmBcConfig.trigger.outputTriggerEnabled = TRUE;
 *     pwmBcConfig.trigger.outputLine           = IfxCcu6_TrigOut_0;
 *     pwmBcConfig.trigger.outputTrigger      = IfxCcu6_TrigSel_cout63;
 *
 *
 *     // initialize the module
 *     IfxCcu6_PwmBc_initModule(&pwmBc, &pwmBcConfig);
 * \endcode
 *
 *
 * The PwmBc is ready for use now!
 *
 *
 * \section IfxLld_Ccu6_PwmBc_ModulationControl Modulation Control
 *
 *
 * The PWMBC driver provides simple to use Motor Control functions
 *
 * This means: you can start, stop, the BLDC motor once the module is initialised with the appropriate configuration
 * You can optionally get the speed of the motor at any point of time
 *
 * Start the motor
 *
 * \code
 *     IfxCcu6_PwmBc_start(&pwmBc);
 * \endcode
 *
 * Stop the motor
 *
 * \code
 *     IfxCcu6_PwmBc_stop(&pwmBc);
 * \endcode
 *
 * Update the output pattern
 *
 * \code
 *     // prepare the control table
 *     // assumed to be declared globally
 *     uint8 controlTable[6][3] = {{1, 5, 17} ,
 *                                 {2, 3, 36} ,
 *                                 {3, 1, 6} ,
 *                                 {4, 6, 18} ,
 *                                 {5, 4, 24} ,
 *                                 {6, 2, 9}};
 *
 *     // update the output pattern
 *     IfxCcu6_PwmBc_updateHallPattern(&pwmBc, controlTable[6][3]);
 * \endcode
 *
 * Get the current motor speed
 * (can also be called in ISR)
 *
 * \code
 *     // assumed to be declared globally
 *     uint32 speed = 0;
 *
 *     // get the current motor speed
 *     speed = IfxCcu6_PwmBc_getMotorSpeed(&pwmBc);
 * \endcode
 *
 * \defgroup IfxLld_Ccu6_PwmBc PWMBC Interface driver
 * \ingroup IfxLld_Ccu6
 * \defgroup IfxLld_Ccu6_PwmBc_DataStructures Data Structures
 * \ingroup IfxLld_Ccu6_PwmBc
 * \defgroup IfxLld_Ccu6_PwmBc_Module_Initialize_Functions Module Initialize Functions
 * \ingroup IfxLld_Ccu6_PwmBc
 * \defgroup IfxLld_Ccu6_PwmBc_Modulation_Control_Functions Modulation Control Functions
 * \ingroup IfxLld_Ccu6_PwmBc
 * \defgroup IfxLld_Ccu6_PwmBc_Hall_Pattern_Functions Hall Pattern Functions
 * \ingroup IfxLld_Ccu6_PwmBc
 */

#ifndef IFXCCU6_PWMBC_H
#define IFXCCU6_PWMBC_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "Ccu6/Std/IfxCcu6.h"
#include "If/Ccu6If/PwmHl.h"

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Ccu6_PwmBc_DataStructures
 * \{ */
/** \brief Structure for interrupt configuration
 */
typedef struct
{
    IfxCcu6_InterruptSource source;               /**< \brief Interrupt source selection */
    IfxCcu6_ServiceRequest  serviceRequest;       /**< \brief Selection of service request outputs */
    uint16                  priority;             /**< \brief Interrupt priority */
    IfxSrc_Tos              typeOfService;        /**< \brief type of interrupt service */
} IfxCcu6_PwmBc_InterruptConfig;

/** \brief Structure for multichannel mode control
 */
typedef struct
{
    IfxCcu6_MultiChannelSwitchingSelect switchingSelect;       /**< \brief Trigger request source (next multi-channel event) for the shadow transfer MCM_ST from MCMPS to MCMP */
    IfxCcu6_MultiChannelSwitchingSync   switchingSync;         /**< \brief Synchronization selection for the shadow transfer event MCM_ST */
} IfxCcu6_PwmBc_MultiChannelControl;

/** \brief Structure for CCU6 output pin configuration
 */
typedef struct
{
    IFX_CONST IfxCcu6_Cc60_Out   *cc60Out;              /**< \brief CC60 compare output CC0 */
    IFX_CONST IfxCcu6_Cc61_Out   *cc61Out;              /**< \brief CC61 compare output CC1 */
    IFX_CONST IfxCcu6_Cc62_Out   *cc62Out;              /**< \brief CC62 compare output CC2 */
    IFX_CONST IfxCcu6_Cout60_Out *cout60;               /**< \brief CC60 compare output COUT0 */
    IFX_CONST IfxCcu6_Cout61_Out *cout61;               /**< \brief CC61 compare output COUT1 */
    IFX_CONST IfxCcu6_Cout62_Out *cout62;               /**< \brief CC62 compare output COUT2 */
    IFX_CONST IfxCcu6_Cout63_Out *cout63;               /**< \brief CC63 compare output COUT3 */
    IFX_CONST IfxCcu6_Ccpos0_In  *ccpos0;               /**< \brief CCPOS0 input signal */
    IFX_CONST IfxCcu6_Ccpos1_In  *ccpos1;               /**< \brief CCPOS2 input signal */
    IFX_CONST IfxCcu6_Ccpos2_In  *ccpos2;               /**< \brief CCPOS2 input signal */
    IfxPort_OutputMode            outputMode;           /**< \brief The pin output mode which should be configured */
    IfxPort_PadDriver             pinDriver;            /**< \brief pad driver */
    IfxPort_InputMode             inputMode;            /**< \brief The CCPOSx pin input mode which should be configured */
    IFX_CONST IfxCcu6_T12hr_In   *t12hr;                /**< \brief T12HR input signal */
    IFX_CONST IfxCcu6_T13hr_In   *t13hr;                /**< \brief T13HR input signal */
    IfxPort_InputMode             t1xhrInputMode;       /**< \brief The T1xHR pin input mode which should be configured */
} IfxCcu6_PwmBc_Pins;

/** \brief Structure for Timer 12
 */
typedef struct
{
    IfxCcu6_T12CountMode countMode;          /**< \brief Operating mode of Timer 12 */
    uint16               counterValue;       /**< \brief 16-bit counter value of Timer12 */
} IfxCcu6_PwmBc_Timer12;

/** \brief Structure for Timer 13
 */
typedef struct
{
    uint16                      counterValue;           /**< \brief 16-bit counter value of Timer13 */
    uint16                      compareValue;           /**< \brief 16-bit comapre value of Timer12 */
    IfxCcu6_T13TriggerEvent     t12SyncEvent;           /**< \brief T12 sync trigger event to start T13 */
    IfxCcu6_T13TriggerDirection t12SyncDirection;       /**< \brief Additional information to control trigger event selection */
} IfxCcu6_PwmBc_Timer13;

/** \brief Configuration structure for external triggers
 */
typedef struct
{
    IfxCcu6_T12hr_In           *t12ExtInputTrigger;           /**< \brief External input signal selection for timer 12 */
    IfxCcu6_ExternalTriggerMode t12ExtInputTriggerMode;       /**< \brief Event of signal T12HR that can set the run bit T12R by HW */
    IfxCcu6_T13hr_In           *t13ExtInputTrigger;           /**< \brief External input signal selection for timer 13 */
    IfxCcu6_ExternalTriggerMode t13ExtInputTriggerMode;       /**< \brief Event of signal T13HR that can set the run bit T13R by HW */
    boolean                     t13InSyncWithT12;             /**< \brief Selection of Timer 13 start in sync with T12 */
    boolean                     outputTriggerEnabled;         /**< \brief Output trigger connection to VADC enable / disable choice */
    IfxCcu6_TrigOut             outputLine;                   /**< \brief Trigger out selection */
    IfxCcu6_TrigSel             outputTrigger;                /**< \brief Trigger selection */
} IfxCcu6_PwmBc_TriggerConfig;

/** \} */

/** \addtogroup IfxLld_Ccu6_PwmBc_DataStructures
 * \{ */
/** \brief Module handle
 */
typedef struct
{
    PwmHl                       base;                   /**< \brief Base PWMHL object */
    Ifx_CCU6                   *ccu6;                   /**< \brief Pointer to the base of CCU6 registers */
    IfxCcu6_PwmBc_TriggerConfig trigger;                /**< \brief Structure for trigger configuration */
    uint8                       hallPatternIndex;       /**< \brief Hall pattern index of motor control table */
} IfxCcu6_PwmBc;

/** \brief Configuration structure of the module
 */
typedef struct
{
    PwmHl_Config                      base;                      /**< \brief Base configuration */
    Ifx_CCU6                         *ccu6;                      /**< \brief Pointer to the base of CCU6 registers */
    IfxCcu6_PwmBc_Timer12             timer12;                   /**< \brief Structure for Timer 12 */
    IfxCcu6_PwmBc_Timer13             timer13;                   /**< \brief Structure for Timer 13 */
    uint8                             hallPatternIndex;          /**< \brief Hall pattern index of motor control table */
    IfxCcu6_HallSensorTriggerMode     hallSyncEvent;             /**< \brief Hall pattern evaluation trigger selection (HSYNC) */
    IfxCcu6_PwmBc_MultiChannelControl multiChannelControl;       /**< \brief Structure for multichannel mode control */
    IfxCcu6_PwmBc_Pins               *pins;                      /**< \brief Structure for CCU6 output pin configuration */
    IfxCcu6_PwmBc_InterruptConfig     interrupt1;                /**< \brief Structure for first interrupt configuration */
    IfxCcu6_PwmBc_InterruptConfig     interrupt2;                /**< \brief Structure for second interrupt configuration */
    IfxCcu6_PwmBc_InterruptConfig     interrupt3;                /**< \brief Structure for third interrupt configuration */
    IfxCcu6_PwmBc_InterruptConfig     interrupt4;                /**< \brief Structure for fourth interrupt configuration */
    IfxCcu6_PwmBc_TriggerConfig       trigger;                   /**< \brief Structure for trigger configuration */
} IfxCcu6_PwmBc_Config;

/** \} */

/** \addtogroup IfxLld_Ccu6_PwmBc_Module_Initialize_Functions
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Initialises the module with default configuration
 * \param pwmBc Module handle
 * \param config Configuration structure of the module
 * \return None
 *
 * A coding example can be found in \ref IfxLld_Ccu6_PwmBc_Usage
 *
 */
IFX_EXTERN void IfxCcu6_PwmBc_initModule(IfxCcu6_PwmBc *pwmBc, const IfxCcu6_PwmBc_Config *config);

/** \brief Fills the config structure with default values
 * \param config Configuration structure of the module
 * \param ccu6 Pointer to the base of CCU6 registers
 * \return None
 *
 * A coding example can be found in \ref IfxLld_Ccu6_PwmBc_Usage
 *
 */
IFX_EXTERN void IfxCcu6_PwmBc_initModuleConfig(IfxCcu6_PwmBc_Config *config, Ifx_CCU6 *ccu6);

/** \} */

/** \addtogroup IfxLld_Ccu6_PwmBc_Modulation_Control_Functions
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Starts the Hall sensored based PWM
 * \param pwmBc Module handle
 * \return None
 *
 * A coding example can be found in \ref IfxLld_Ccu6_PwmBc_Usage
 *
 */
IFX_EXTERN void IfxCcu6_PwmBc_start(IfxCcu6_PwmBc *pwmBc);

/** \brief Stops the Hall sensored based PWM
 * \param pwmBc Module handle
 * \return None
 *
 * A coding example can be found in \ref IfxLld_Ccu6_PwmBc_Usage
 *
 */
IFX_EXTERN void IfxCcu6_PwmBc_stop(IfxCcu6_PwmBc *pwmBc);

/** \} */

/** \addtogroup IfxLld_Ccu6_PwmBc_Hall_Pattern_Functions
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief returns the current motor speed
 * \param pwmBc Module handle
 * \return speed
 *
 * A coding example can be found in \ref IfxLld_Ccu6_PwmBc_Usage
 *
 */
IFX_EXTERN uint32 IfxCcu6_PwmBc_getMotorSpeed(IfxCcu6_PwmBc *pwmBc);

/** \brief Updates the Hall pattern
 * \param pwmBc Module handle
 * \param controlTable Motor Control table
 * \return None
 *
 * A coding example can be found in \ref IfxLld_Ccu6_PwmBc_Usage
 *
 */
IFX_EXTERN void IfxCcu6_PwmBc_updateHallPattern(IfxCcu6_PwmBc * pwmBc, uint8 controlTable[6][3]);

/** \} */

#endif /* IFXCCU6_PWMBC_H */
