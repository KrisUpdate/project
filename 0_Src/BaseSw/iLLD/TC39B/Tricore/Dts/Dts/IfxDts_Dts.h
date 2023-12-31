/**
 * \file IfxDts_Dts.h
 * \brief DTS DTS details
 * \ingroup IfxLld_Dts
 *
 * \version iLLD_1_0_1_9_0
 * \copyright Copyright (c) 2017 Infineon Technologies AG. All rights reserved.
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
 * \defgroup IfxLld_Dts_Dts_Usage How to use the DTS Interface driver?
 * \ingroup IfxLld_Dts
 *
 * DTS gives the die-temperature result which will decoded to standard temperature  value. Minimum first two temperature results are to be ignored to get reliable temperature.
 *
 * In the following sections it will be described, how to integrate the driver into the application framework.
 *
 * \section IfxLld_Dts_Dts_Preparation Preparation
 * \subsection IfxLld_Dts_Dts_Include Include Files
 *
 * Include following header file into your C code:
 *
 * \code
 * #include <Dts/Dts/IfxDts_Dts.h>
 * \endcode
 *
 * \subsection IfxLld_Dts_Dts_Interrupt Interrupt Handler Installation
 *
 * See also \ref IfxLld_Cpu_Irq_Usage
 *
 * Define priorities for the Interrrupt handler. This is normally done in the Ifx_IntPrioDef.h file:
 * \code
 * // priorities are normally defined in Ifx_IntPrioDef.h
 * #define IFX_INTPRIO_DTS  1
 * \endcode
 *
 * Add the interrupt service routine to your C code which should do the error flag handling:
 * \code
 * IFX_INTERRUPT(dtsISR, 0, IFX_INTPRIO_DTS)
 * {
 *      // DTS finished temperature measurement
 *
 *      uint16 temperatureValue = IfxDts_Dts_getTemperatureValue();
 *      // here we could do something with the value...
 * }
 * \endcode
 *
 * Finally install the interrupt handler in your initialisation function:
 * \code
 *     // install interrupt handler
 *     IfxCpu_Irq_installInterruptHandler(&dtsISR, IFX_INTPRIO_DTS);
 *     IfxCpu_enableInterrupts();
 * \endcode
 *
 * \subsection IfxLld_Dts_Dts_Module DTS Module Initialisation
 *
 * The module initialisation can be done in the same function:
 * \code
 *     // Get the default configuration
 *     IfxDts_Dts_Config dtsConfig;
 *     IfxDts_Dts_initModuleConfig(&dtsConfig);
 *
 *     // Adapt the default configuration if required
 *     dtsConfig.isrPriority = IFX_INTPRIO_DTS;
 *     dtsConfig.isrTypeOfService = IfxCpu_Irq_getTos(IfxCpu_getCoreIndex());
 *     dtsConfig.lowerTemperatureLimit = -35;  // SMU alarm if temperature value is below this Celsius value
 *     dtsConfig.upperTemperatureLimit = 150;  // SMU alarm if temperature value is above this Celsius value
 *
 *     // Module initialisation
 *     IfxDts_Dts_initModule(&dtsConfig);
 * \endcode
 *
 * \subsection IfxLld_Dts_Dts_IrqBased IRQ based measurements
 *
 * Whenever the interrupt handler is called, a new measurment result is available and could be processed further (e.g. for statistical analysis).
 *
 * \code
 * IFX_INTERRUPT(dtsISR, 0, IFX_INTPRIO_DTS)
 * {
 *      // DTS finished temperature measurement
 *
 *      uint16 temperatureValue = IfxDts_Dts_getTemperatureValue();
 *      // here we could do something with the value...
 * }
 * \endcode
 *
 *
 * \subsection IfxLld_Dts_Dts_NonIrqBased Measurements without IRQs
 *
 * If no interrupt routine should be used, a new measurement result can be requested the following way:
 *
 * \code
 *     uint16 temperatureValue = IfxDts_Dts_getTemperatureValue();
 * \endcode
 *
 *
 * Alternatively (and this is the normal usage), the DTS could be periodically started from a timer routine, e.g. each mS. The conversion itself takes 100 uS maximum.
 *
 *
 * \subsection IfxLld_Dts_Dts_Converted Temperature conversion
 *
 * Following function returns the actual temperature in Celsius:
 *
 * \code
 *    float32 temperature = IfxDts_Dts_getTemperatureCelsius();
 * \endcode
 *
 * \defgroup IfxLld_Dts_Dts DTS
 * \ingroup IfxLld_Dts
 * \defgroup IfxLld_Dts_Dts_Structures Data Structures
 * \ingroup IfxLld_Dts_Dts
 * \defgroup IfxLld_Dts_Dts_Module Module Initialisation functions
 * \ingroup IfxLld_Dts_Dts
 * \defgroup IfxLld_Dts_Dts_Sensor Sensor Functions
 * \ingroup IfxLld_Dts_Dts
 * \defgroup IfxLld_Dts_Dts_Conversion Conversion Functions
 * \ingroup IfxLld_Dts_Dts
 */

#ifndef IFXDTS_DTS_H
#define IFXDTS_DTS_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "Dts/Std/IfxDts.h"
#include "Cpu/Std/IfxCpu.h"

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Dts_Dts_Structures
 * \{ */
/** \brief DTS module configuration structure
 */
typedef struct
{
    boolean sensorControlDisabled;          /**< \brief MODULE_SCU.DTSCON.SCLK, specifies the control register lock except MODULE_SCU.DTSCON.START. */
    float32 lowerTemperatureLimit;          /**< \brief Specifies the lower temperature limit compared against die temperature in Celsius
                                             *
                                             * A SMU will be triggered if the measurement result is below this limit */
    float32 upperTemperatureLimit;          /**< \brief Specifies the upper temperature limit compared against die temperature in Celsius.
                                             *
                                             * A SMU will be triggered if the measurement result is above this limit */
    uint16     isrPriority;                 /**< \brief interrupt priority */
    IfxSrc_Tos isrTypeOfService;            /**< \brief type of interrupt service */
} IfxDts_Dts_Config;

/** \} */

/** \addtogroup IfxLld_Dts_Dts_Module
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Initialise the DTS with supplied configuration.
 * \param config pointer to module configuration structure
 * \return None
 *
 * Usage Example : \ref IfxLld_Dts_Dts_Usage
 *
 */
IFX_EXTERN void IfxDts_Dts_initModule(const IfxDts_Dts_Config *config);

/** \brief Intialises the module configuration buffer with default configuration.
 * \param config pointer to module configuration structure
 * \return None
 *
 * Usage Example : \ref IfxLld_Dts_Dts_Usage
 *
 */
IFX_EXTERN void IfxDts_Dts_initModuleConfig(IfxDts_Dts_Config *config);

/** \} */

/** \addtogroup IfxLld_Dts_Dts_Sensor
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Returns the converted temperature in Celsius
 * \return The temperature based on the DTS temperature value
 *
 * Usage Example : \ref IfxLld_Dts_Dts_Usage
 *
 */
IFX_INLINE float32 IfxDts_Dts_getTemperatureCelsius(void);

/** \brief Returns the unconverted temperature measurement result
 * \return current sensor measured result.
 *
 * Usage Example : \ref IfxLld_Dts_Dts_Usage
 *
 */
IFX_INLINE uint16 IfxDts_Dts_getTemperatureValue(void);

/** \} */

/** \addtogroup IfxLld_Dts_Dts_Conversion
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Converts a temperature value in Celsius to DTS value
 * \param temperatureValue the temperature in Celsius
 * \return the appr. DTS value
 */
IFX_EXTERN uint16 IfxDts_Dts_convertFromCelsius(float32 temperatureValue);

/** \brief Converts the measurement value returned from DTS to Celsius
 * \param dtsValue measurement value returned from DTS
 * \return temperature in Celsius
 */
IFX_EXTERN float32 IfxDts_Dts_convertToCelsius(uint16 dtsValue);

/** \} */

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE float32 IfxDts_Dts_getTemperatureCelsius(void)
{
    return IfxDts_Dts_convertToCelsius(IfxDts_getTemperatureValue());
}


IFX_INLINE uint16 IfxDts_Dts_getTemperatureValue(void)
{
    return (uint16)IfxDts_getTemperatureValue();
}


#endif /* IFXDTS_DTS_H */
