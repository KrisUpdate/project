/**
 * \file IfxRif_cfg.h
 * \brief RIF on-chip implementation data
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
 *
 *
 * \defgroup IfxLld_Rif RIF
 * \ingroup IfxLld
 * \defgroup IfxLld_Rif_Impl Implementation
 * \ingroup IfxLld_Rif
 * \defgroup IfxLld_Rif_Std Standard Driver
 * \ingroup IfxLld_Rif
 * \defgroup IfxLld_Rif_Impl_Enumerations Enumerations
 * \ingroup IfxLld_Rif_Impl
 * \defgroup IfxLld_Rif_Impl_Variables Global Variables
 * \ingroup IfxLld_Rif_Impl
 */

#ifndef IFXRIF_CFG_H
#define IFXRIF_CFG_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "Cpu/Std/Ifx_Types.h"
#include "IfxRif_reg.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

#define IFXRIF_NUM_MODULES (2)

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Rif_Impl_Enumerations
 * \{ */
/** \brief List of the available RIF resources
 */
typedef enum
{
    IfxRif_Index_none = -1,  /**< \brief Not Selected */
    IfxRif_Index_0    = 0,   /**< \brief RIF index 0  */
    IfxRif_Index_1           /**< \brief RIF index 1  */
} IfxRif_Index;

/** \} */

/** \addtogroup IfxLld_Rif_Impl_Variables
 * \{ */

/******************************************************************************/
/*-------------------Global Exported Variables/Constants----------------------*/
/******************************************************************************/

IFX_EXTERN IfxModule_IndexMap IfxRif_cfg_indexMap[IFXRIF_NUM_MODULES];

/** \} */

#endif /* IFXRIF_CFG_H */
