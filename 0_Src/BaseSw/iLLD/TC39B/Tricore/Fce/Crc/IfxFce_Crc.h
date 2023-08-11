/**
 * \file IfxFce_Crc.h
 * \brief FCE CRC details
 * \ingroup IfxLld_Fce
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
 * \defgroup IfxLld_Fce_Crc_Usage How to use the FCE CRC Interface driver?
 * \ingroup IfxLld_Fce
 *
 *
 * FCE gives CRC message signatures. Kernel 0-3 can be used for CRC calculation
 *
 * In the following sections it will be described, how to integrate the driver into the application framework.
 *
 * \section IfxLld_Fce_Fce_Preparation Preparation
 * \subsection IfxLld_Fce_Fce_Include Include Files
 *
 * Include following header file into your C code:
 *
 * \code
 * #include <Fce/Crc/IfxFce_Crc.h>
 *
 * \endcode
 *
 * \subsection IfxLld_Fce_Crc_Variables Variables
 *
 * Declare the FCE and CRC handles as global variable in your C code:
 * \code
 * // used globally
 * static IfxFce_Crc fce;
 * static IfxFce_Crc_Crc fceCrc32_0;
 *
 *
 * \endcode
 *
 * \subsection IfxLld_Fce_Crc_Interrupt Interrupt Handler Installation
 *
 * See also \ref IfxLld_Cpu_Irq_Usage
 *
 * Define priorities for the Interrrupt handler. This is normally done in the Ifx_IntPrioDef.h file:
 * \code
 * // priorities are normally defined in Ifx_IntPrioDef.h
 * #define IFX_INTPRIO_FCE  1
 * \endcode
 *
 * Add the interrupt service routine to your C code which should do the error flag handling:
 * \code
 * IFX_INTERRUPT(fceISR, 0, IFX_INTPRIO_FCE)
 * {
 *     {
 *         Ifx_FCE_IN_STS interruptStatus = IfxFce_Crc_getInterruptStatus(&fceCrc32_0);
 *         if (interruptStatus.U )
 *         {
 *          if ( interruptStatus.B.CMF == 1)
 *          {
 *                 // CRC Mismatch Error...!
 *          }
 *          if( interruptStatus.B.CEF == 1)
 *          {
 *                 // Configuration Error...!
 *          }
 *          if ( interruptStatus.B.LEF == 1)
 *          {
 *                 // Length Error...!
 *          }
 *          if ( interruptStatus.B.BEF == 1)
 *          {
 *                 // Bus Error...!
 *          }
 *         }
 *         IfxFce_Crc_clearErrorFlags(&fceCrc32_0);
 *     }
 *
 * }
 * \endcode
 *
 * Finally install the interrupt handler in your initialisation function:
 * \code
 *     // install interrupt handler
 *     IfxCpu_Irq_installInterruptHandler(fceISR, IFX_INTPRIO_FCE);
 *     IfxCpu_enableInterrupts();
 * \endcode
 *
 * \subsection IfxLld_Fce_Fce_InitCrc Module Initialisation
 *
 * The module initialisation can be done in the same function. Here an example for all CRC kernels:
 * \code
 *     // create module config
 *     IfxFce_Crc_Config fceConfig;
 *     IfxFce_Crc_initModuleConfig(&fceConfig, &MODULE_FCE);
 *
 *     // ISR priorities and interrupt target
 *     fceConfig.isrPriority = IFX_INTPRIO_FCE;
 *     fceConfig.isrTypeOfService = IfxCpu_Irq_getTos( IfxCpu_getCoreIndex());
 *
 *     // initialize module
 *     //IfxFce_Crc fce; // defined globally
 *     IfxFce_Crc_initModule(&fce, &fceConfig);
 *
 *     // initialize CRC kernels
 *     IfxFce_Crc_CrcConfig crcConfig;
 *     IfxFce_Crc_initCrcConfig(&crcConfig, &fce);
 *
 *
 *     crcConfig.crcKernel= IfxFce_CrcKernel_3;
 *     IfxFce_Crc_initCrc(&fceCrc32_1, &crcConfig);
 *
 *
 *     crcConfig.useDma       = TRUE; // Enable Dma transfer
 *     crcConfig.fceChannelId = IfxDma_ChannelId_0;
 *
 *
 * \endcode
 *
 *
 * \subsection IfxLld_Fce_Fce_ErrorIRQ Error Interrupt on CRC mismatch
 *
 * By default, all error interrupts aside from the CRC mismatch interrupt (CMF) are enabled.
 *
 * In order to use the CMF interrupt as well, the appr. error flag has to be enabled during configuration:
 * \code
 *     // initialize CRC kernel with CRC check enabled
 *     IfxFce_Crc_CrcConfig crcConfig;
 *     IfxFce_Crc_initCrcConfig(&crcConfig, &fce);
 *     crcConfig.enabledInterrupts.crcMismatch = TRUE;
 *     IfxFce_Crc_initCrc(&fceCrc32_0, &crcConfig);
 * \endcode
 *
 * And the expected CRC value has to be written into the expectedCrc field of the handle before the CRC calculation is started.
 *
 * \code
 *     {
 *         // Expected CRC:
 *         fceCrc32_0.expectedCrc = 0xd95def75;
 *
 *         // do calculation
 *         uint32 fceCrc = IfxFce_Crc_calculateCrc(&fceCrc32_0, checkData, CHECK_DATA_SIZE, 0x00000000);
 *
 *         // we can expect, that an error interrupt has been triggered if the CRC over checkData didn't match.
 *     }
 * \endcode
 *
 * \subsection IfxLld_Fce_Fce_calculateCrc CRC Calculation
 *
 * Now, all 4 FCE kernels are configured and can be used to calculate CRC values.
 *
 * In following examples, CRC is calculated over following array:
 * \code
 * #define CHECK_DATA_SIZE 25  // declare this portion globally
 *
 * // if Dma is enabled, align memory to 32 byte
 *
 * uint32 checkData[CHECK_DATA_SIZE] = {
 *     0xbe9957bb,
 *     0x1c706c1e,
 *     0x14c3db3f,
 *     0x7fb17a93,
 *     0xb0d9d5a7,
 *     0x768093e0,
 *     0x88b206a0,
 *     0xc51299e4,
 *     0xe8a97d48,
 *     0x89367f27,
 *     0x70095984,
 *     0xec030f75,
 *     0xdc22f8d4,
 *     0xd951407b,
 *     0x34ae18c6,
 *     0x4d47ba7d,
 *     0x0e2e4622,
 *     0x4a2e90d3,
 *     0xdaec3752,
 *     0xcd3ed11c,
 *     0x36b416b7,
 *     0x8ea28658,
 *     0xdd37eee3,
 *     0x23928b62,
 *     0x84eb4b22,
 * };
 * \endcode
 *
 *
 * \defgroup IfxLld_Fce_Crc CRC
 * \ingroup IfxLld_Fce
 * \defgroup IfxLld_Fce_Crc_DataStructures Data Structures
 * \ingroup IfxLld_Fce_Crc
 * \defgroup IfxLld_Fce_Crc_Module Module Initialise Functions
 * \ingroup IfxLld_Fce_Crc
 * \defgroup IfxLld_Fce_Crc_Operative CRC Operation Functions
 * \ingroup IfxLld_Fce_Crc
 * \defgroup IfxLld_Fce_Crc_Interrupt Interrupt Status Function
 * \ingroup IfxLld_Fce_Crc
 */

#ifndef IFXFCE_CRC_H
#define IFXFCE_CRC_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "Fce/Std/IfxFce.h"
#include "Cpu/Irq/IfxCpu_Irq.h"
#include "Dma/Dma/IfxDma_Dma.h"

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Fce_Crc_DataStructures
 * \{ */
/** \brief Specifies the interrupt enable structure
 */
typedef struct
{
    boolean crcMismatch;       /**< \brief Specifies enabel/disable of CRC mismatch interrupt */
    boolean configError;       /**< \brief Specifies enabel/disable of configuration error interrupt */
    boolean lengthError;       /**< \brief Specifies enabel/disable of length error interrupt */
    boolean busError;          /**< \brief Specifies enabel/disable of bus error interrupt */
} IfxFce_Crc_EnabledInterrupts;

/** \} */

/** \addtogroup IfxLld_Fce_Crc_DataStructures
 * \{ */
/** \brief FCE Module Handle;
 */
typedef struct
{
    Ifx_FCE *fce;       /**< \brief Pointer to FCE registers */
} IfxFce_Crc;

/** \brief Configuration structure of the module
 */
typedef struct
{
    Ifx_FCE   *fce;                    /**< \brief Pointer to FCE registers */
    uint16     isrPriority;            /**< \brief interrupt priority */
    IfxSrc_Tos isrTypeOfService;       /**< \brief type of interrupt service */
} IfxFce_Crc_Config;

/** \brief Specifies the pointer to FCE module handler
 */
typedef struct
{
    Ifx_FCE           *fce;                 /**< \brief Specifies pointer to FCE module registers */
    uint32             expectedCrc;         /**< \brief Specifies the expected CRC to be compared with resulted. */
    IfxDma_Dma_Channel fceDmaChannel;       /**< \brief Dma channel handle for Fce. */
    boolean            useDma;              /**< \brief Variable indicates Dma usage in Fce. */
    IfxFce_CrcChannel  crcChannel;          /**< \brief Specifies the Crc channel */
    IfxFce_CrcKernel   crcKernel;           /**< \brief Specifies the Crc kernel */
} IfxFce_Crc_Crc;

/** \brief Specifies the module configuration structure
 */
typedef struct
{
    Ifx_FCE                     *fce;                             /**< \brief Specifies pointer to FCE module registers */
    boolean                      crcCheckCompared;                /**< \brief Specifies whether CRC check comparision is enabled or not */
    boolean                      automaticLengthReload;           /**< \brief Specifies the enable/disable of automatic length reload */
    boolean                      dataByteReflectionEnabled;       /**< \brief Specifies enable/disable of input data byte wise reflection */
    boolean                      crc32BitReflectionEnabled;       /**< \brief Specifies enable/disable of CRC 32-bit wise reflection */
    boolean                      swapOrderOfBytes;                /**< \brief Swaps the order of the bytes in the IR input register */
    uint32                       expectedCrc;                     /**< \brief Specifies the expected CRC to be compared with resulted. */
    boolean                      crcResultInverted;               /**< \brief Specifies the XOR valueto get the final CRC */
    IfxFce_Crc_EnabledInterrupts enabledInterrupts;               /**< \brief Specifies the interrupt enable structure */
    boolean                      useDma;                          /**< \brief Specifies whether Dma is used or not. */
    IfxDma_ChannelId             fceChannelId;                    /**< \brief Select the Dma channel for Fce operation */
    IfxFce_CrcChannel            crcChannel;                      /**< \brief Specifies the Crc Channel */
    IfxFce_CrcKernel             crcKernel;                       /**< \brief Specifies the Crc kernel */
} IfxFce_Crc_CrcConfig;

/** \} */

/** \addtogroup IfxLld_Fce_Crc_DataStructures
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Calculate the XORed CRC value and returns it. It takes the precomputed XORed and reversed.
 * \param fce Specifies the pointer to FCE module handler
 * \param crcData Length of the input data block
 * \param crcDataLength Length of the input data block
 * \param crcStartValue start value for CRC calculation
 * \return Final CRC after XORed with XOR value.
 */
IFX_EXTERN uint32 IfxFce_Crc_calculateCrc(IfxFce_Crc_Crc *fce, const uint32 *crcData, uint16 crcDataLength, uint32 crcStartValue);

/** \} */

/** \addtogroup IfxLld_Fce_Crc_Module
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Deinitialises the FCE module
 * \param fce Specifies the pointer to FCE module handler
 * \return None
 *
 * Reset the module
 *
 * \code
 * IfxFce_Crc fce;
 * //Deinitialise the module
 * IfxFce_Crc_deInitModule(&fce);
 * \endcode
 *
 */
IFX_EXTERN void IfxFce_Crc_deInitModule(IfxFce_Crc_Crc *fce);

/** \brief Initialise the FCE module for CRC computation according to CRC mode
 * \param fceCrc Specifies the pointer to CRC handle
 * \param crcConfig Specifies the FCE CRC configuration structure
 * \return None
 *
 * Usage Example: see \ref IfxLld_Fce_Crc_Usage
 *
 */
IFX_EXTERN void IfxFce_Crc_initCrc(IfxFce_Crc_Crc *fceCrc, const IfxFce_Crc_CrcConfig *crcConfig);

/** \brief Initialises the default CRC configuration buffer
 * \param crcConfig Specifies the FCE CRC configuration structure
 * \param fce Pointer to the FCE module handle
 * \return None
 *
 * Usage Example: see \ref IfxLld_Fce_Crc_Usage
 *
 */
IFX_EXTERN void IfxFce_Crc_initCrcConfig(IfxFce_Crc_CrcConfig *crcConfig, IfxFce_Crc *fce);

/** \brief Usage Example: see \ref IfxLld_Fce_Crc_Usage
 * \param fce module handle
 * \param config predefined configuration structure of the module
 * \return None
 */
IFX_EXTERN void IfxFce_Crc_initModule(IfxFce_Crc *fce, const IfxFce_Crc_Config *config);

/** \brief Fills the config structure with default values
 * \param config configuration structure of the module
 * \param fce pointer to FCE registers
 * \return None
 *
 * Usage Example: see \ref IfxLld_Fce_Crc_Usage
 *
 */
IFX_EXTERN void IfxFce_Crc_initModuleConfig(IfxFce_Crc_Config *config, Ifx_FCE *fce);

/** \} */

/** \addtogroup IfxLld_Fce_Crc_Interrupt
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Clears the error flags
 * \param fce Specifies the pointer to FCE module handler
 * \return None
 *
 * Usage Example: see \ref IfxLld_Fce_Crc_Usage
 *
 */
IFX_EXTERN void IfxFce_Crc_clearErrorFlags(IfxFce_Crc_Crc *fce);

/** \} */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Gets the current CRC interrupt status.
 * \param fce Specifies the pointer to FCE module handler
 * \return Current interrupt status
 */
IFX_EXTERN Ifx_FCE_IN_STS IfxFce_Crc_getInterruptStatus(IfxFce_Crc_Crc *fce);

#endif /* IFXFCE_CRC_H */
