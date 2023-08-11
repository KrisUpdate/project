/**
 * \file IfxEbu_Dram.h
 * \brief EBU DRAM details
 * \ingroup IfxLld_Ebu
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
 * \defgroup IfxLld_Ebu_Dram_Usage How to use the DRAM Driver?
 * \ingroup IfxLld_Ebu
 *
 * The DRAM interface driver provides a default EBU configuration for communicating with external SDRAM devices such as MT48H8M32LF and HY52V651620B.
 *
 * \section IfxLld_Ebu_Dram_Preparation Preparation
 * \subsection IfxLld_Ebu_Dram_Include Include Files
 *
 * Include following header file into your C code:
 * \code
 * #include <Ebu/Dram/IfxEbu_Dram.h>
 * \endcode
 *
 * \subsection IfxLld_Ebu_Dram_Init_MT48 MT48H8M32LF Initialisation
 * The module initialisation can be done as shown in the example
 *
 * This will configure EBU for MT48H8M32LF device
 *
 * \code
 *     IfxEbu_Dram_Config cfg;
 *     IfxEbu_Dram_initMemoryConfig(&cfg, &MODULE_EBU0);
 *     cfg.memoryRegionConfig.baseAddress = 0xa4000000; // specify noncached segment A, driver will also enable the cached segment 8
 * \endcode
 *
 * The device is used as 16bit device
 * \code
 *     cfg.sdramDevice = IfxEbu_Dram_SDRAMDevice_8Mx16;
 * \endcode
 *
 * By default Mobile SDRAM settings are not configured hence we need to configure them
 * \code
 *     cfg.sdramModConfig.extendedBankSelect = IfxEbu_ExtendedOperationBankSelect_2;
 *     cfg.sdramModConfig.extendedOperationMode = 0x18;
 *     IfxEbu_Dram dram;
 *     IfxEbu_Dram_initMemory(&dram, &cfg);
 * \endcode
 *
 * \subsection IfxLld_Ebu_Dram_Init_HY52 HY52V65120B Initialisation
 *
 * Inoder to configure HY52V65120B on CS0 instead of MT48H8M32LF then do the following
 * \code
 *     IfxEbu_Dram_Config cfg;
 *     IfxEbu_Dram_initMemoryConfig(&cfg, &MODULE_EBU0);
 *     cfg.memoryRegionConfig.baseAddress = 0xa4000000; // specify noncached segment A, driver will also enable the cached segment 8
 *     cfg.sdramDevice = IfxEbu_Dram_SDRAMDevice_4Mx16;
 *     IfxEbu_Dram dram;
 *     IfxEbu_Dram_initMemory(&dram, &cfg);
 * \endcode
 *
 * SDRAM interface layer will configure the BANK and AWIDTH settings based on the size of the MEMORY device.
 *
 * \defgroup IfxLld_Ebu_Dram DRAM Driver
 * \ingroup IfxLld_Ebu
 * \defgroup IfxLld_Ebu_Dram_DataStructures Data Structures
 * \ingroup IfxLld_Ebu_Dram
 * \defgroup IfxLld_Ebu_Dram_Module Module Functions
 * \ingroup IfxLld_Ebu_Dram
 * \defgroup IfxLld_Ebu_Dram_Enum Enumerations
 * \ingroup IfxLld_Ebu_Dram
 */

#ifndef IFXEBU_DRAM_H
#define IFXEBU_DRAM_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "Ebu/Std/IfxEbu.h"
#include "SysSe/Bsp/Bsp.h"
#include "Port/Std/IfxPort.h"
#include "Scu/Std/IfxScuWdt.h"

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Ebu_Dram_Enum
 * \{ */
typedef enum
{
    IfxEbu_Dram_SDRAMDevice_64Mx16bit = 0,  /**< \brief  */
    IfxEbu_Dram_SDRAMDevice_32Mx16bit,      /**< \brief  */
    IfxEbu_Dram_SDRAMDevice_16Mx16,
    IfxEbu_Dram_SDRAMDevice_8Mx16,
    IfxEbu_Dram_SDRAMDevice_4Mx16,
    IfxEbu_Dram_SDRAMDevice_1Mx16,          /**< \brief In this case only one Bank Address will be used by the External Device */
    IfxEbu_Dram_SDRAMDevice_32Mx32,
    IfxEbu_Dram_SDRAMDevice_16Mx32,
    IfxEbu_Dram_SDRAMDevice_8Mx32,
    IfxEbu_Dram_SDRAMDevice_4Mx32
} IfxEbu_Dram_SDRAMDevice;

/** \} */

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Ebu_Dram_DataStructures
 * \{ */
typedef struct
{
    Ifx_EBU          *ebu;
    IfxEbu_ChipSelect chipSelect;
    uint32            baseAddress;
} IfxEbu_Dram;

typedef struct
{
    Ifx_EBU                    *module;
    IfxEbu_ChipSelect           chipSelect;
    IfxEbu_ReadConfig           syncReadConfig;
    IfxEbu_WriteConfig          syncWriteConfig;
    IfxEbu_ReadConfig           asyncReadConfig;
    IfxEbu_WriteConfig          asyncWriteConfig;
    IfxEbu_ReadAccessParameter  syncReadAccessParameter;
    IfxEbu_WriteAccessParameter syncWriteAccessParameter;
    IfxEbu_ReadAccessParameter  asyncReadAccessParameter;
    IfxEbu_WriteAccessParameter asyncWriteAccessParameter;
    IfxEbu_Dram_SDRAMDevice     sdramDevice;
    IfxEbu_SDRAMModConfig       sdramModConfig;
    IfxEbu_SDRAMRefreshConfig   sdramRefreshConfig;
    IfxEbu_SDRAMControlConfig   sdramControlConfig;
    IfxEbu_MemoryRegionConfig   memoryRegionConfig;
    IfxEbu_ExternalClockRatio   externalClockRatio;
    IfxEbu_ModuleConfig         moduleConfig;
} IfxEbu_Dram_Config;

/** \} */

/** \addtogroup IfxLld_Ebu_Dram_Module
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/**
 * \return None
 */
IFX_EXTERN void IfxEbu_Dram_initMemory(IfxEbu_Dram *dram, const IfxEbu_Dram_Config *config);

/**
 * \return None
 */
IFX_EXTERN void IfxEbu_Dram_initMemoryConfig(IfxEbu_Dram_Config *config, Ifx_EBU *ebu);

/** \} */

#endif /* IFXEBU_DRAM_H */
