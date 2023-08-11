/**
 * \file IfxEray_PinMap.h
 * \brief ERAY I/O map
 * \ingroup IfxLld_Eray
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
 * \defgroup IfxLld_Eray_pinmap ERAY Pin Mapping
 * \ingroup IfxLld_Eray
 */

#ifndef IFXERAY_PINMAP_H
#define IFXERAY_PINMAP_H

#include <IfxEray_reg.h>
#include <_Impl/IfxEray_cfg.h>
#include <Port/Std/IfxPort.h>

/** \addtogroup IfxLld_Eray_pinmap
 * \{ */

/** \brief RXD pin mapping structure */
typedef const struct
{
    Ifx_ERAY*         module;   /**< \brief Base address */
    IfxEray_NodeId    nodeId;   /**< \brief Node ID */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxEray_Rxd_In;

/** \brief TXD pin mapping structure */
typedef const struct
{
    Ifx_ERAY*         module;   /**< \brief Base address */
    IfxEray_NodeId    nodeId;   /**< \brief Node ID */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    IfxPort_OutputIdx select;   /**< \brief Port control code */
} IfxEray_Txd_Out;

/** \brief TXEN pin mapping structure */
typedef const struct
{
    Ifx_ERAY*         module;   /**< \brief Base address */
    IfxEray_NodeId    nodeId;   /**< \brief Node ID */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    IfxPort_OutputIdx select;   /**< \brief Port control code */
} IfxEray_Txen_Out;

IFX_EXTERN IfxEray_Rxd_In IfxEray0_RXDA0_P14_8_IN;  /**< \brief Receive Channel A0 */
IFX_EXTERN IfxEray_Rxd_In IfxEray0_RXDA1_P11_9_IN;  /**< \brief Receive Channel A1 */
IFX_EXTERN IfxEray_Rxd_In IfxEray0_RXDA2_P02_1_IN;  /**< \brief Receive Channel A2 */
IFX_EXTERN IfxEray_Rxd_In IfxEray0_RXDA3_P14_1_IN;  /**< \brief Receive Channel A3 */
IFX_EXTERN IfxEray_Rxd_In IfxEray0_RXDB0_P14_7_IN;  /**< \brief Receive Channel B0 */
IFX_EXTERN IfxEray_Rxd_In IfxEray0_RXDB1_P11_10_IN;  /**< \brief Receive Channel B1 */
IFX_EXTERN IfxEray_Rxd_In IfxEray0_RXDB2_P02_3_IN;  /**< \brief Receive Channel B2 */
IFX_EXTERN IfxEray_Rxd_In IfxEray0_RXDB3_P14_1_IN;  /**< \brief Receive Channel B3 */
IFX_EXTERN IfxEray_Rxd_In IfxEray1_RXDA0_P14_8_IN;  /**< \brief Receive Channel A0 */
IFX_EXTERN IfxEray_Rxd_In IfxEray1_RXDA1_P01_1_IN;  /**< \brief Receive Channel A1 */
IFX_EXTERN IfxEray_Rxd_In IfxEray1_RXDB0_P14_7_IN;  /**< \brief Receive Channel B0 */
IFX_EXTERN IfxEray_Rxd_In IfxEray1_RXDB1_P01_8_IN;  /**< \brief Receive Channel B1 */
IFX_EXTERN IfxEray_Txd_Out IfxEray0_TXDA_P02_0_OUT;  /**< \brief Transmit Channel A */
IFX_EXTERN IfxEray_Txd_Out IfxEray0_TXDA_P11_3_OUT;  /**< \brief Transmit Channel A */
IFX_EXTERN IfxEray_Txd_Out IfxEray0_TXDA_P14_0_OUT;  /**< \brief Transmit Channel A */
IFX_EXTERN IfxEray_Txd_Out IfxEray0_TXDA_P14_10_OUT;  /**< \brief Transmit Channel A */
IFX_EXTERN IfxEray_Txd_Out IfxEray0_TXDB_P02_2_OUT;  /**< \brief Transmit Channel B */
IFX_EXTERN IfxEray_Txd_Out IfxEray0_TXDB_P11_12_OUT;  /**< \brief Transmit Channel B */
IFX_EXTERN IfxEray_Txd_Out IfxEray0_TXDB_P14_0_OUT;  /**< \brief Transmit Channel B */
IFX_EXTERN IfxEray_Txd_Out IfxEray0_TXDB_P14_5_OUT;  /**< \brief Transmit Channel B */
IFX_EXTERN IfxEray_Txd_Out IfxEray1_TXDA_P01_12_OUT;  /**< \brief Transmit Channel A */
IFX_EXTERN IfxEray_Txd_Out IfxEray1_TXDA_P14_10_OUT;  /**< \brief Transmit Channel A */
IFX_EXTERN IfxEray_Txd_Out IfxEray1_TXDB_P01_13_OUT;  /**< \brief Transmit Channel B */
IFX_EXTERN IfxEray_Txd_Out IfxEray1_TXDB_P14_5_OUT;  /**< \brief Transmit Channel B */
IFX_EXTERN IfxEray_Txen_Out IfxEray0_TXENA_P02_4_OUT;  /**< \brief Transmit Enable Channel A */
IFX_EXTERN IfxEray_Txen_Out IfxEray0_TXENA_P11_6_OUT;  /**< \brief Transmit Enable Channel A */
IFX_EXTERN IfxEray_Txen_Out IfxEray0_TXENA_P14_9_OUT;  /**< \brief Transmit Enable Channel A */
IFX_EXTERN IfxEray_Txen_Out IfxEray0_TXENB_P02_5_OUT;  /**< \brief Transmit Enable Channel B */
IFX_EXTERN IfxEray_Txen_Out IfxEray0_TXENB_P11_11_OUT;  /**< \brief Transmit Enable Channel B */
IFX_EXTERN IfxEray_Txen_Out IfxEray0_TXENB_P11_6_OUT;  /**< \brief Transmit Enable Channel B */
IFX_EXTERN IfxEray_Txen_Out IfxEray0_TXENB_P14_6_OUT;  /**< \brief Transmit Enable Channel B */
IFX_EXTERN IfxEray_Txen_Out IfxEray0_TXENB_P14_9_OUT;  /**< \brief Transmit Enable Channel B */
IFX_EXTERN IfxEray_Txen_Out IfxEray1_TXENA_P01_14_OUT;  /**< \brief Transmit Enable Channel A */
IFX_EXTERN IfxEray_Txen_Out IfxEray1_TXENA_P14_9_OUT;  /**< \brief Transmit Enable Channel A */
IFX_EXTERN IfxEray_Txen_Out IfxEray1_TXENB_P02_15_OUT;  /**< \brief Transmit Enable Channel B */
IFX_EXTERN IfxEray_Txen_Out IfxEray1_TXENB_P14_6_OUT;  /**< \brief Transmit Enable Channel B */

/** \brief Table dimensions */
#define IFXERAY_PINMAP_NUM_MODULES 2
#define IFXERAY_PINMAP_NUM_NODES 2
#define IFXERAY_PINMAP_RXD_IN_NUM_ITEMS 4
#define IFXERAY_PINMAP_TXD_OUT_NUM_ITEMS 4
#define IFXERAY_PINMAP_TXEN_OUT_NUM_ITEMS 5


/** \brief IfxEray_Rxd_In table */
IFX_EXTERN const IfxEray_Rxd_In *IfxEray_Rxd_In_pinTable[IFXERAY_PINMAP_NUM_MODULES][IFXERAY_PINMAP_NUM_NODES][IFXERAY_PINMAP_RXD_IN_NUM_ITEMS];

/** \brief IfxEray_Txd_Out table */
IFX_EXTERN const IfxEray_Txd_Out *IfxEray_Txd_Out_pinTable[IFXERAY_PINMAP_NUM_MODULES][IFXERAY_PINMAP_NUM_NODES][IFXERAY_PINMAP_TXD_OUT_NUM_ITEMS];

/** \brief IfxEray_Txen_Out table */
IFX_EXTERN const IfxEray_Txen_Out *IfxEray_Txen_Out_pinTable[IFXERAY_PINMAP_NUM_MODULES][IFXERAY_PINMAP_NUM_NODES][IFXERAY_PINMAP_TXEN_OUT_NUM_ITEMS];

/** \} */

#endif /* IFXERAY_PINMAP_H */
