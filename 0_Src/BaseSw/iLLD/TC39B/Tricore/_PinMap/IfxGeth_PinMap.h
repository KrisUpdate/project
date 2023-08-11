/**
 * \file IfxGeth_PinMap.h
 * \brief GETH I/O map
 * \ingroup IfxLld_Geth
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
 * \defgroup IfxLld_Geth_pinmap GETH Pin Mapping
 * \ingroup IfxLld_Geth
 */

#ifndef IFXGETH_PINMAP_H
#define IFXGETH_PINMAP_H

#include <IfxGeth_reg.h>
#include <_Impl/IfxGeth_cfg.h>
#include <Port/Std/IfxPort.h>

/** \addtogroup IfxLld_Geth_pinmap
 * \{ */

/** \brief CRS pin mapping structure */
typedef const struct
{
    Ifx_GETH*          module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxGeth_Crs_In;

/** \brief CRSDV pin mapping structure */
typedef const struct
{
    Ifx_GETH*          module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxGeth_Crsdv_In;

/** \brief RXDV pin mapping structure */
typedef const struct
{
    Ifx_GETH*          module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxGeth_Rxdv_In;

/** \brief RXCTL pin mapping structure */
typedef const struct
{
    Ifx_GETH*          module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxGeth_Rxctl_In;

/** \brief GREFCLK pin mapping structure */
typedef const struct
{
    Ifx_GETH*          module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxGeth_Grefclk_In;

/** \brief REFCLK pin mapping structure */
typedef const struct
{
    Ifx_GETH*          module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxGeth_Refclk_In;

/** \brief RXCLK pin mapping structure */
typedef const struct
{
    Ifx_GETH*          module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxGeth_Rxclk_In;

/** \brief TXCLK pin mapping structure */
typedef const struct
{
    Ifx_GETH*          module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxGeth_Txclk_In;

/** \brief RXD pin mapping structure */
typedef const struct
{
    Ifx_GETH*          module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxGeth_Rxd_In;

/** \brief COL pin mapping structure */
typedef const struct
{
    Ifx_GETH*          module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxGeth_Col_In;

/** \brief MDC pin mapping structure */
typedef const struct
{
    Ifx_GETH*          module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    IfxPort_OutputIdx select;   /**< \brief Port control code */
} IfxGeth_Mdc_Out;

/** \brief MDIO pin mapping structure */
typedef const struct
{
    Ifx_GETH*          module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         inSelect; /**< \brief Input multiplexer value */
    IfxPort_OutputIdx outSelect;/**< \brief Port control code */
} IfxGeth_Mdio_InOut;

/** \brief TXD pin mapping structure */
typedef const struct
{
    Ifx_GETH*          module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    IfxPort_OutputIdx select;   /**< \brief Port control code */
} IfxGeth_Txd_Out;

/** \brief TXCLK pin mapping structure */
typedef const struct
{
    Ifx_GETH*          module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    IfxPort_OutputIdx select;   /**< \brief Port control code */
} IfxGeth_Txclk_Out;

/** \brief TXEN pin mapping structure */
typedef const struct
{
    Ifx_GETH*          module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    IfxPort_OutputIdx select;   /**< \brief Port control code */
} IfxGeth_Txen_Out;

/** \brief TXER pin mapping structure */
typedef const struct
{
    Ifx_GETH*          module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    IfxPort_OutputIdx select;   /**< \brief Port control code */
} IfxGeth_Txer_Out;

/** \brief TXCTL pin mapping structure */
typedef const struct
{
    Ifx_GETH*          module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    IfxPort_OutputIdx select;   /**< \brief Port control code */
} IfxGeth_Txctl_Out;

/** \brief RXER pin mapping structure */
typedef const struct
{
    Ifx_GETH*          module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxGeth_Rxer_In;

/** \brief PPS pin mapping structure */
typedef const struct
{
    Ifx_GETH*          module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    IfxPort_OutputIdx select;   /**< \brief Port control code */
} IfxGeth_Pps_Out;

IFX_EXTERN IfxGeth_Col_In IfxGeth_COL_P11_15_IN;  /**< \brief Collision MII */
IFX_EXTERN IfxGeth_Crs_In IfxGeth_CRSA_P11_14_IN;  /**< \brief Carrier Sense MII */
IFX_EXTERN IfxGeth_Crs_In IfxGeth_CRSB_P11_11_IN;  /**< \brief Carrier Sense MII */
IFX_EXTERN IfxGeth_Crsdv_In IfxGeth_CRSDVA_P11_11_IN;  /**< \brief Carrier Sense / Data Valid combi-signal for RMII */
IFX_EXTERN IfxGeth_Crsdv_In IfxGeth_CRSDVB_P11_14_IN;  /**< \brief Carrier Sense / Data Valid combi-signal for RMII */
IFX_EXTERN IfxGeth_Grefclk_In IfxGeth_GREFCLK_P11_5_IN;  /**< \brief Gigabit Reference Clock input for RGMII (125 MHz high precission) */
IFX_EXTERN IfxGeth_Mdc_Out IfxGeth_MDC_P02_8_OUT;  /**< \brief MDIO clock */
IFX_EXTERN IfxGeth_Mdc_Out IfxGeth_MDC_P12_0_OUT;  /**< \brief MDIO clock */
IFX_EXTERN IfxGeth_Mdc_Out IfxGeth_MDC_P21_2_OUT;  /**< \brief MDIO clock */
IFX_EXTERN IfxGeth_Mdio_InOut IfxGeth_MDIO_P00_0_INOUT;  /**< \brief MDIO Output */
IFX_EXTERN IfxGeth_Mdio_InOut IfxGeth_MDIO_P12_1_INOUT;  /**< \brief MDIO Output */
IFX_EXTERN IfxGeth_Mdio_InOut IfxGeth_MDIO_P21_3_INOUT;  /**< \brief MDIO Output */
IFX_EXTERN IfxGeth_Pps_Out IfxGeth_PPS_P14_4_OUT;  /**< \brief Pulse Per Second */
IFX_EXTERN IfxGeth_Refclk_In IfxGeth_REFCLKA_P11_12_IN;  /**< \brief Reference Clock input for RMII (50 MHz) */
IFX_EXTERN IfxGeth_Rxclk_In IfxGeth_RXCLKA_P11_12_IN;  /**< \brief Receive Clock MII */
IFX_EXTERN IfxGeth_Rxclk_In IfxGeth_RXCLKB_P11_4_IN;  /**< \brief Receive Clock MII */
IFX_EXTERN IfxGeth_Rxclk_In IfxGeth_RXCLKC_P12_0_IN;  /**< \brief Receive Clock MII */
IFX_EXTERN IfxGeth_Rxctl_In IfxGeth_RXCTLA_P11_11_IN;  /**< \brief Receive Control for RGMII */
IFX_EXTERN IfxGeth_Rxd_In IfxGeth_RXD0A_P11_10_IN;  /**< \brief Receive Data 0 MII, RMII and RGMII (RGMII can use RXD0A only) */
IFX_EXTERN IfxGeth_Rxd_In IfxGeth_RXD1A_P11_9_IN;  /**< \brief Receive Data 1 MII, RMII and RGMII (RGMII can use RXD1A only) */
IFX_EXTERN IfxGeth_Rxd_In IfxGeth_RXD2A_P11_8_IN;  /**< \brief Receive Data 2 MII and RGMII  (RGMII can use RXD2A only) */
IFX_EXTERN IfxGeth_Rxd_In IfxGeth_RXD3A_P11_7_IN;  /**< \brief Receive Data 3 MII and RGMII  (RGMII can use RXD3A only) */
IFX_EXTERN IfxGeth_Rxdv_In IfxGeth_RXDVA_P11_11_IN;  /**< \brief Receive Data Valid MII */
IFX_EXTERN IfxGeth_Rxdv_In IfxGeth_RXDVB_P11_14_IN;  /**< \brief Receive Data Valid MII */
IFX_EXTERN IfxGeth_Rxer_In IfxGeth_RXERA_P11_13_IN;  /**< \brief Receive Error MII */
IFX_EXTERN IfxGeth_Rxer_In IfxGeth_RXERB_P21_7_IN;  /**< \brief Receive Error MII */
IFX_EXTERN IfxGeth_Rxer_In IfxGeth_RXERC_P10_0_IN;  /**< \brief Receive Error MII */
IFX_EXTERN IfxGeth_Txclk_In IfxGeth_TXCLKA_P11_5_IN;  /**< \brief Transmit Clock Input for MII */
IFX_EXTERN IfxGeth_Txclk_In IfxGeth_TXCLKB_P11_12_IN;  /**< \brief Transmit Clock Input for MII */
IFX_EXTERN IfxGeth_Txclk_Out IfxGeth_TXCLK_P11_4_OUT;  /**< \brief Transmit Clock Output for RGMII */
IFX_EXTERN IfxGeth_Txctl_Out IfxGeth_TXCTL_P11_6_OUT;  /**< \brief GETH_TXEN 0: Transmit Control for RGMII */
IFX_EXTERN IfxGeth_Txd_Out IfxGeth_TXD0_P11_3_OUT;  /**< \brief Transmit Data */
IFX_EXTERN IfxGeth_Txd_Out IfxGeth_TXD1_P11_2_OUT;  /**< \brief Transmit Data */
IFX_EXTERN IfxGeth_Txd_Out IfxGeth_TXD2_P11_1_OUT;  /**< \brief Transmit Data */
IFX_EXTERN IfxGeth_Txd_Out IfxGeth_TXD3_P11_0_OUT;  /**< \brief Transmit Data */
IFX_EXTERN IfxGeth_Txen_Out IfxGeth_TXEN_P11_6_OUT;  /**< \brief Transmit Enable MII and RMII */
IFX_EXTERN IfxGeth_Txer_Out IfxGeth_TXER_P11_4_OUT;  /**< \brief Transmit Error MII */

/** \brief Table dimensions */
#define IFXGETH_PINMAP_NUM_MODULES 1
#define IFXGETH_PINMAP_COL_IN_NUM_ITEMS 1
#define IFXGETH_PINMAP_CRS_IN_NUM_ITEMS 2
#define IFXGETH_PINMAP_CRSDV_IN_NUM_ITEMS 2
#define IFXGETH_PINMAP_GREFCLK_IN_NUM_ITEMS 1
#define IFXGETH_PINMAP_MDC_OUT_NUM_ITEMS 3
#define IFXGETH_PINMAP_MDIO_INOUT_NUM_ITEMS 3
#define IFXGETH_PINMAP_PPS_OUT_NUM_ITEMS 1
#define IFXGETH_PINMAP_REFCLK_IN_NUM_ITEMS 1
#define IFXGETH_PINMAP_RXCLK_IN_NUM_ITEMS 3
#define IFXGETH_PINMAP_RXCTL_IN_NUM_ITEMS 1
#define IFXGETH_PINMAP_RXD_IN_NUM_ITEMS 4
#define IFXGETH_PINMAP_RXDV_IN_NUM_ITEMS 2
#define IFXGETH_PINMAP_RXER_IN_NUM_ITEMS 3
#define IFXGETH_PINMAP_TXCLK_IN_NUM_ITEMS 2
#define IFXGETH_PINMAP_TXCLK_OUT_NUM_ITEMS 1
#define IFXGETH_PINMAP_TXCTL_OUT_NUM_ITEMS 1
#define IFXGETH_PINMAP_TXD_OUT_NUM_ITEMS 4
#define IFXGETH_PINMAP_TXEN_OUT_NUM_ITEMS 1
#define IFXGETH_PINMAP_TXER_OUT_NUM_ITEMS 1


/** \brief IfxGeth_Col_In table */
IFX_EXTERN const IfxGeth_Col_In *IfxGeth_Col_In_pinTable[IFXGETH_PINMAP_NUM_MODULES][IFXGETH_PINMAP_COL_IN_NUM_ITEMS];

/** \brief IfxGeth_Crs_In table */
IFX_EXTERN const IfxGeth_Crs_In *IfxGeth_Crs_In_pinTable[IFXGETH_PINMAP_NUM_MODULES][IFXGETH_PINMAP_CRS_IN_NUM_ITEMS];

/** \brief IfxGeth_Crsdv_In table */
IFX_EXTERN const IfxGeth_Crsdv_In *IfxGeth_Crsdv_In_pinTable[IFXGETH_PINMAP_NUM_MODULES][IFXGETH_PINMAP_CRSDV_IN_NUM_ITEMS];

/** \brief IfxGeth_Grefclk_In table */
IFX_EXTERN const IfxGeth_Grefclk_In *IfxGeth_Grefclk_In_pinTable[IFXGETH_PINMAP_NUM_MODULES][IFXGETH_PINMAP_GREFCLK_IN_NUM_ITEMS];

/** \brief IfxGeth_Mdc_Out table */
IFX_EXTERN const IfxGeth_Mdc_Out *IfxGeth_Mdc_Out_pinTable[IFXGETH_PINMAP_NUM_MODULES][IFXGETH_PINMAP_MDC_OUT_NUM_ITEMS];

/** \brief IfxGeth_Mdio_InOut table */
IFX_EXTERN const IfxGeth_Mdio_InOut *IfxGeth_Mdio_InOut_pinTable[IFXGETH_PINMAP_NUM_MODULES][IFXGETH_PINMAP_MDIO_INOUT_NUM_ITEMS];

/** \brief IfxGeth_Pps_Out table */
IFX_EXTERN const IfxGeth_Pps_Out *IfxGeth_Pps_Out_pinTable[IFXGETH_PINMAP_NUM_MODULES][IFXGETH_PINMAP_PPS_OUT_NUM_ITEMS];

/** \brief IfxGeth_Refclk_In table */
IFX_EXTERN const IfxGeth_Refclk_In *IfxGeth_Refclk_In_pinTable[IFXGETH_PINMAP_NUM_MODULES][IFXGETH_PINMAP_REFCLK_IN_NUM_ITEMS];

/** \brief IfxGeth_Rxclk_In table */
IFX_EXTERN const IfxGeth_Rxclk_In *IfxGeth_Rxclk_In_pinTable[IFXGETH_PINMAP_NUM_MODULES][IFXGETH_PINMAP_RXCLK_IN_NUM_ITEMS];

/** \brief IfxGeth_Rxctl_In table */
IFX_EXTERN const IfxGeth_Rxctl_In *IfxGeth_Rxctl_In_pinTable[IFXGETH_PINMAP_NUM_MODULES][IFXGETH_PINMAP_RXCTL_IN_NUM_ITEMS];

/** \brief IfxGeth_Rxd_In table */
IFX_EXTERN const IfxGeth_Rxd_In *IfxGeth_Rxd_In_pinTable[IFXGETH_PINMAP_NUM_MODULES][IFXGETH_PINMAP_RXD_IN_NUM_ITEMS];

/** \brief IfxGeth_Rxdv_In table */
IFX_EXTERN const IfxGeth_Rxdv_In *IfxGeth_Rxdv_In_pinTable[IFXGETH_PINMAP_NUM_MODULES][IFXGETH_PINMAP_RXDV_IN_NUM_ITEMS];

/** \brief IfxGeth_Rxer_In table */
IFX_EXTERN const IfxGeth_Rxer_In *IfxGeth_Rxer_In_pinTable[IFXGETH_PINMAP_NUM_MODULES][IFXGETH_PINMAP_RXER_IN_NUM_ITEMS];

/** \brief IfxGeth_Txclk_In table */
IFX_EXTERN const IfxGeth_Txclk_In *IfxGeth_Txclk_In_pinTable[IFXGETH_PINMAP_NUM_MODULES][IFXGETH_PINMAP_TXCLK_IN_NUM_ITEMS];

/** \brief IfxGeth_Txclk_Out table */
IFX_EXTERN const IfxGeth_Txclk_Out *IfxGeth_Txclk_Out_pinTable[IFXGETH_PINMAP_NUM_MODULES][IFXGETH_PINMAP_TXCLK_OUT_NUM_ITEMS];

/** \brief IfxGeth_Txctl_Out table */
IFX_EXTERN const IfxGeth_Txctl_Out *IfxGeth_Txctl_Out_pinTable[IFXGETH_PINMAP_NUM_MODULES][IFXGETH_PINMAP_TXCTL_OUT_NUM_ITEMS];

/** \brief IfxGeth_Txd_Out table */
IFX_EXTERN const IfxGeth_Txd_Out *IfxGeth_Txd_Out_pinTable[IFXGETH_PINMAP_NUM_MODULES][IFXGETH_PINMAP_TXD_OUT_NUM_ITEMS];

/** \brief IfxGeth_Txen_Out table */
IFX_EXTERN const IfxGeth_Txen_Out *IfxGeth_Txen_Out_pinTable[IFXGETH_PINMAP_NUM_MODULES][IFXGETH_PINMAP_TXEN_OUT_NUM_ITEMS];

/** \brief IfxGeth_Txer_Out table */
IFX_EXTERN const IfxGeth_Txer_Out *IfxGeth_Txer_Out_pinTable[IFXGETH_PINMAP_NUM_MODULES][IFXGETH_PINMAP_TXER_OUT_NUM_ITEMS];

/** \} */

#endif /* IFXGETH_PINMAP_H */
