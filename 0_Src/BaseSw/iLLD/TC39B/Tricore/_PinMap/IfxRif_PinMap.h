/**
 * \file IfxRif_PinMap.h
 * \brief RIF I/O map
 * \ingroup IfxLld_Rif
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
 * \defgroup IfxLld_Rif_pinmap RIF Pin Mapping
 * \ingroup IfxLld_Rif
 */

#ifndef IFXRIF_PINMAP_H
#define IFXRIF_PINMAP_H

#include <IfxRif_reg.h>
#include <_Impl/IfxRif_cfg.h>
#include <Port/Std/IfxPort.h>

/** \addtogroup IfxLld_Rif_pinmap
 * \{ */

/** \brief CLK pin mapping structure */
typedef const struct
{
    Ifx_RIF*          module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxRif_Clk_In;

/** \brief D pin mapping structure */
typedef const struct
{
    Ifx_RIF*          module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxRif_D_In;

/** \brief FR pin mapping structure */
typedef const struct
{
    Ifx_RIF*          module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxRif_Fr_In;

/** \brief ExRamp pin mapping structure */
typedef const struct
{
    Ifx_RIF*          module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxRif_ExRamp_In;


IFX_EXTERN IfxRif_ExRamp_In IfxRif_RAMP1B_P02_6_IN;  /**< \brief External RAMP B input */
IFX_EXTERN IfxRif_ExRamp_In IfxRif_RAMP1B_P10_8_IN;  /**< \brief External RAMP B input */

/** \brief Table dimensions */
#define IFXRIF_PINMAP_NUM_MODULES 2
#define IFXRIF_PINMAP_EXRAMP_IN_NUM_ITEMS 1


/** \brief IfxRif_ExRamp_In table */
IFX_EXTERN const IfxRif_ExRamp_In *IfxRif_ExRamp_In_pinTable[IFXRIF_PINMAP_NUM_MODULES][IFXRIF_PINMAP_EXRAMP_IN_NUM_ITEMS];

/** \} */

#endif /* IFXRIF_PINMAP_H */
