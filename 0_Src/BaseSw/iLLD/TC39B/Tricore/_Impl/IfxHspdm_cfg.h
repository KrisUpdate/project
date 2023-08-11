/**
 * \file IfxHspdm_cfg.h
 * \brief HSPDM on-chip implementation data
 * \ingroup IfxLld_Hspdm
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
 * \defgroup IfxLld_Hspdm HSPDM
 * \ingroup IfxLld
 * \defgroup IfxLld_Hspdm_Impl Implementation
 * \ingroup IfxLld_Hspdm
 * \defgroup IfxLld_Hspdm_Std Standard Driver
 * \ingroup IfxLld_Hspdm
 */

#ifndef IFXHSPDM_CFG_H
#define IFXHSPDM_CFG_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "Cpu/Std/Ifx_Types.h"
#include "IfxHspdm_reg.h"
#include "IfxHspdm_regdef.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

#define IFXHSPDM_NUM_MODULES     (1)

/** \brief maximum buffer size of HSPDM is 8KB.
 */
#define IFXHSPDM_BUFFER_MAX_SIZE (0x2000u)

/******************************************************************************/
/*-------------------------------Enumerations---------------------------------*/
/******************************************************************************/

/** \brief hardware trigger source selection
 */
typedef enum
{
    IfxHspdm_HwTriggerSource_CCU6 = 0  /**< \brief CCU6 is trigger device */
} IfxHspdm_HwTriggerSource;

#endif /* IFXHSPDM_CFG_H */
