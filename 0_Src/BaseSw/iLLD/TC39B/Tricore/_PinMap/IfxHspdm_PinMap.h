/**
 * \file IfxHspdm_PinMap.h
 * \brief HSPDM I/O map
 * \ingroup IfxLld_Hspdm
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
 * \defgroup IfxLld_Hspdm_pinmap HSPDM Pin Mapping
 * \ingroup IfxLld_Hspdm
 */

#ifndef IFXHSPDM_PINMAP_H
#define IFXHSPDM_PINMAP_H

#include <IfxHspdm_reg.h>
#include <_Impl/IfxHspdm_cfg.h>
#include <Port/Std/IfxPort.h>

/** \addtogroup IfxLld_Hspdm_pinmap
 * \{ */

/** \brief HSPDM BS pin mapping structure */
typedef const struct
{
    Ifx_HSPDM*         module;    /**< \brief Base address */
    IfxPort_Pin       pin;       /**< \brief Port pin */
    IfxPort_OutputIdx select;    /**< \brief Port control code */
} IfxHspdm_Bs_Out;

/** \brief HSPDM MUTE pin mapping structure */
typedef const struct
{
    Ifx_HSPDM*         module;    /**< \brief Base address */
    IfxPort_Pin       pin;       /**< \brief Port pin */
    IfxPort_OutputIdx select;    /**< \brief Port control code */
} IfxHspdm_Mute_Out;

IFX_EXTERN IfxHspdm_Bs_Out IfxHspdm0_BS0_OUT_P22_4_OUT;  /**< \brief Bit stream 0 output to the pad */
IFX_EXTERN IfxHspdm_Bs_Out IfxHspdm0_BS1_OUT_P22_5_OUT;  /**< \brief Bit stream 1 output to the pad */
IFX_EXTERN IfxHspdm_Mute_Out IfxHspdm0_MUTE_P22_3_OUT;  /**< \brief Mute output from the micro controller which could be used to control an external Transmitter */

/** \brief Table dimensions */
#define IFXHSPDM_PINMAP_NUM_MODULES 1
#define IFXHSPDM_PINMAP_NUM_0S 1
#define IFXHSPDM_PINMAP_BS_OUT_NUM_ITEMS 2
#define IFXHSPDM_PINMAP_MUTE_OUT_NUM_ITEMS 1


/** \brief IfxHspdm_Bs_Out table */
IFX_EXTERN const IfxHspdm_Bs_Out *IfxHspdm_Bs_Out_pinTable[IFXHSPDM_PINMAP_NUM_MODULES][IFXHSPDM_PINMAP_NUM_0S][IFXHSPDM_PINMAP_BS_OUT_NUM_ITEMS];

/** \brief IfxHspdm_Mute_Out table */
IFX_EXTERN const IfxHspdm_Mute_Out *IfxHspdm_Mute_Out_pinTable[IFXHSPDM_PINMAP_NUM_MODULES][IFXHSPDM_PINMAP_NUM_0S][IFXHSPDM_PINMAP_MUTE_OUT_NUM_ITEMS];

/** \} */

#endif /* IFXHSPDM_PINMAP_H */
