/**
 * \file IfxEmem_cfg.h
 * \brief EMEM on-chip implementation data
 * \ingroup IfxLld_Emem
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
 * \defgroup IfxLld_Emem EMEM
 * \ingroup IfxLld
 * \defgroup IfxLld_Emem_Impl Implementation
 * \ingroup IfxLld_Emem
 * \defgroup IfxLld_Emem_Std Standard Driver
 * \ingroup IfxLld_Emem
 */

#ifndef IFXEMEM_CFG_H
#define IFXEMEM_CFG_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "Cpu/Std/Ifx_Types.h"
#include "IfxEmem_mpu_reg.h"
#include "IfxEmem_reg.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

#define IFXEMEM_START_ADDR_CPU_CACHED 0x99000000UL

#define IFXEMEM_START_ADDR_CPU        0xB9000000UL

#define IFXEMEM_START_ADDR_BBB        (0xB9000000UL)

#define IFXEMEM_SIZE                  (0x400000UL)

#define IFXEMEM_NUM_MODULES           (1)

#define IFXEMEM_NUM_MPU_MODULES       (4)

/** \brief Emem bus size in bytes
 */
#define IFXEMEM_BUS_SIZE              (256 / 8)

/******************************************************************************/
/*-------------------------------Enumerations---------------------------------*/
/******************************************************************************/

/** \brief List of the available EMEM MPU instances
 */
typedef enum
{
    IfxEmem_MpuIndex_0    = 0,  /**< \brief EMEM Module MPU 0 */
    IfxEmem_MpuIndex_1    = 1,  /**< \brief EMEM Module MPU 1 */
    IfxEmem_MpuIndex_2    = 2,  /**< \brief EMEM Module MPU 2 */
    IfxEmem_MpuIndex_3    = 3,  /**< \brief EMEM Module MPU 3 */
    IfxEmem_MpuIndex_none = -1  /**< \brief Not selected */
} IfxEmem_MpuIndex;

/******************************************************************************/
/*-------------------Global Exported Variables/Constants----------------------*/
/******************************************************************************/

IFX_EXTERN IFX_CONST IfxModule_IndexMap IfxEmem_cfg_indexMap[IFXEMEM_NUM_MPU_MODULES];

#endif /* IFXEMEM_CFG_H */
