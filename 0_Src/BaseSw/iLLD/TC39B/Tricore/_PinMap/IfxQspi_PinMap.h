/**
 * \file IfxQspi_PinMap.h
 * \brief QSPI I/O map
 * \ingroup IfxLld_Qspi
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
 * \defgroup IfxLld_Qspi_pinmap QSPI Pin Mapping
 * \ingroup IfxLld_Qspi
 */

#ifndef IFXQSPI_PINMAP_H
#define IFXQSPI_PINMAP_H

#include <IfxQspi_reg.h>
#include <_Impl/IfxQspi_cfg.h>
#include <Port/Std/IfxPort.h>

/** \addtogroup IfxLld_Qspi_pinmap
 * \{ */

/** \brief MRST pin mapping structure */
typedef const struct
{
    Ifx_QSPI*         module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxQspi_Mrst_In;

/** \brief MTSR pin mapping structure */
typedef const struct
{
    Ifx_QSPI*         module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxQspi_Mtsr_In;

/** \brief SCLK pin mapping structure */
typedef const struct
{
    Ifx_QSPI*         module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxQspi_Sclk_In;

/** \brief SLSI pin mapping structure */
typedef const struct
{
    Ifx_QSPI*         module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxQspi_Slsi_In;

/** \brief HSICIN pin mapping structure */
typedef const struct
{
    Ifx_QSPI*         module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxQspi_Hsicin_In;

/** \brief MRST pin mapping structure */
typedef const struct
{
    Ifx_QSPI*         module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    IfxPort_OutputIdx select;   /**< \brief Port control code */
} IfxQspi_Mrst_Out;

/** \brief MTSR pin mapping structure */
typedef const struct
{
    Ifx_QSPI*         module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    IfxPort_OutputIdx select;   /**< \brief Port control code */
} IfxQspi_Mtsr_Out;

/** \brief SCLK pin mapping structure */
typedef const struct
{
    Ifx_QSPI*         module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    IfxPort_OutputIdx select;   /**< \brief Port control code */
} IfxQspi_Sclk_Out;

/** \brief SLSO pin mapping structure */
typedef const struct
{
    Ifx_QSPI*         module;   /**< \brief Base address */
    sint32            slsoNr;   /**< \brief Slave Select */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    IfxPort_OutputIdx select;   /**< \brief Port control code */
} IfxQspi_Slso_Out;

IFX_EXTERN IfxQspi_Hsicin_In IfxQspi2_HSICINA_P15_2_IN;  /**< \brief Highspeed capture channel */
IFX_EXTERN IfxQspi_Hsicin_In IfxQspi2_HSICINB_P15_3_IN;  /**< \brief Highspeed capture channel */
IFX_EXTERN IfxQspi_Hsicin_In IfxQspi3_HSICINA_P33_9_IN;  /**< \brief Highspeed capture channel */
IFX_EXTERN IfxQspi_Hsicin_In IfxQspi3_HSICINB_P33_10_IN;  /**< \brief Highspeed capture channel */
IFX_EXTERN IfxQspi_Mrst_In IfxQspi0_MRSTA_P20_12_IN;  /**< \brief Master SPI data input */
IFX_EXTERN IfxQspi_Mrst_In IfxQspi0_MRSTB_P22_9_IN;  /**< \brief Master SPI data input */
IFX_EXTERN IfxQspi_Mrst_In IfxQspi0_MRSTC_P22_6_IN;  /**< \brief Master SPI data input */
IFX_EXTERN IfxQspi_Mrst_In IfxQspi1_MRSTA_P10_1_IN;  /**< \brief Master SPI data input */
IFX_EXTERN IfxQspi_Mrst_In IfxQspi1_MRSTB_P11_3_IN;  /**< \brief Master SPI data input */
IFX_EXTERN IfxQspi_Mrst_In IfxQspi2_MRSTA_P15_4_IN;  /**< \brief Master SPI data input */
IFX_EXTERN IfxQspi_Mrst_In IfxQspi2_MRSTB_P15_7_IN;  /**< \brief Master SPI data input */
IFX_EXTERN IfxQspi_Mrst_In IfxQspi2_MRSTCN_P21_2_IN;  /**< \brief Master SPI data input (LVDS N line) */
IFX_EXTERN IfxQspi_Mrst_In IfxQspi2_MRSTCP_P21_3_IN;  /**< \brief Master SPI data input (LVDS P line) */
IFX_EXTERN IfxQspi_Mrst_In IfxQspi2_MRSTD_P34_4_IN;  /**< \brief Master SPI data input */
IFX_EXTERN IfxQspi_Mrst_In IfxQspi2_MRSTE_P15_2_IN;  /**< \brief Master SPI data input */
IFX_EXTERN IfxQspi_Mrst_In IfxQspi2_MRSTFN_P14_9_IN;  /**< \brief Master SPI data input (LVDS N line) */
IFX_EXTERN IfxQspi_Mrst_In IfxQspi2_MRSTFP_P14_10_IN;  /**< \brief Master SPI data input (LVDS P line) */
IFX_EXTERN IfxQspi_Mrst_In IfxQspi3_MRSTA_P02_5_IN;  /**< \brief Master SPI data input */
IFX_EXTERN IfxQspi_Mrst_In IfxQspi3_MRSTB_P10_7_IN;  /**< \brief Master SPI data input */
IFX_EXTERN IfxQspi_Mrst_In IfxQspi3_MRSTC_P01_5_IN;  /**< \brief Master SPI data input */
IFX_EXTERN IfxQspi_Mrst_In IfxQspi4_MRSTA_P33_13_IN;  /**< \brief Master SPI data input */
IFX_EXTERN IfxQspi_Mrst_In IfxQspi4_MRSTB_P22_1_IN;  /**< \brief Master SPI data input */
IFX_EXTERN IfxQspi_Mrst_In IfxQspi4_MRSTCN_P21_2_IN;  /**< \brief Master SPI data input (LVDS N line) */
IFX_EXTERN IfxQspi_Mrst_In IfxQspi4_MRSTCP_P21_3_IN;  /**< \brief Master SPI data input (LVDS P line) */
IFX_EXTERN IfxQspi_Mrst_In IfxQspi4_MRSTDN_P21_0_IN;  /**< \brief Master SPI data input (LVDS N line) */
IFX_EXTERN IfxQspi_Mrst_In IfxQspi4_MRSTDP_P21_1_IN;  /**< \brief Master SPI data input (LVDS P line) */
IFX_EXTERN IfxQspi_Mrst_In IfxQspi5_MRSTA_P15_10_IN;  /**< \brief Master SPI data input */
IFX_EXTERN IfxQspi_Mrst_In IfxQspi5_MRSTB_P14_5_IN;  /**< \brief Master SPI data input */
IFX_EXTERN IfxQspi_Mrst_Out IfxQspi0_MRST_P20_12_OUT;  /**< \brief Slave SPI data output */
IFX_EXTERN IfxQspi_Mrst_Out IfxQspi0_MRST_P22_6_OUT;  /**< \brief Slave SPI data output */
IFX_EXTERN IfxQspi_Mrst_Out IfxQspi0_MRST_P22_9_OUT;  /**< \brief Slave SPI data output */
IFX_EXTERN IfxQspi_Mrst_Out IfxQspi1_MRST_P10_1_OUT;  /**< \brief Slave SPI data output */
IFX_EXTERN IfxQspi_Mrst_Out IfxQspi1_MRST_P10_6_OUT;  /**< \brief Slave SPI data output */
IFX_EXTERN IfxQspi_Mrst_Out IfxQspi1_MRST_P11_3_OUT;  /**< \brief Slave SPI data output */
IFX_EXTERN IfxQspi_Mrst_Out IfxQspi2_MRST_P15_4_OUT;  /**< \brief Slave SPI data output */
IFX_EXTERN IfxQspi_Mrst_Out IfxQspi2_MRST_P15_7_OUT;  /**< \brief Slave SPI data output */
IFX_EXTERN IfxQspi_Mrst_Out IfxQspi2_MRST_P34_4_OUT;  /**< \brief Slave SPI data output */
IFX_EXTERN IfxQspi_Mrst_Out IfxQspi3_MRST_P01_5_OUT;  /**< \brief Slave SPI data output */
IFX_EXTERN IfxQspi_Mrst_Out IfxQspi3_MRST_P02_5_OUT;  /**< \brief Slave SPI data output */
IFX_EXTERN IfxQspi_Mrst_Out IfxQspi3_MRST_P10_7_OUT;  /**< \brief Slave SPI data output */
IFX_EXTERN IfxQspi_Mrst_Out IfxQspi4_MRST_P22_1_OUT;  /**< \brief Slave SPI data output */
IFX_EXTERN IfxQspi_Mrst_Out IfxQspi4_MRST_P33_13_OUT;  /**< \brief Slave SPI data output */
IFX_EXTERN IfxQspi_Mrst_Out IfxQspi5_MRST_P14_5_OUT;  /**< \brief Slave SPI data output */
IFX_EXTERN IfxQspi_Mrst_Out IfxQspi5_MRST_P15_10_OUT;  /**< \brief Slave SPI data output */
IFX_EXTERN IfxQspi_Mtsr_In IfxQspi0_MTSRA_P20_14_IN;  /**< \brief Slave SPI data input */
IFX_EXTERN IfxQspi_Mtsr_In IfxQspi0_MTSRB_P22_10_IN;  /**< \brief Slave SPI data input */
IFX_EXTERN IfxQspi_Mtsr_In IfxQspi0_MTSRC_P22_5_IN;  /**< \brief Slave SPI data input */
IFX_EXTERN IfxQspi_Mtsr_In IfxQspi1_MTSRA_P10_3_IN;  /**< \brief Slave SPI data input */
IFX_EXTERN IfxQspi_Mtsr_In IfxQspi1_MTSRB_P11_9_IN;  /**< \brief Slave SPI data input */
IFX_EXTERN IfxQspi_Mtsr_In IfxQspi1_MTSRC_P10_4_IN;  /**< \brief Slave SPI data input */
IFX_EXTERN IfxQspi_Mtsr_In IfxQspi2_MTSRA_P15_5_IN;  /**< \brief Slave SPI data input */
IFX_EXTERN IfxQspi_Mtsr_In IfxQspi2_MTSRB_P15_6_IN;  /**< \brief Slave SPI data input */
IFX_EXTERN IfxQspi_Mtsr_In IfxQspi2_MTSRD_P34_5_IN;  /**< \brief Slave SPI data input */
IFX_EXTERN IfxQspi_Mtsr_In IfxQspi3_MTSRA_P02_6_IN;  /**< \brief Slave SPI data input */
IFX_EXTERN IfxQspi_Mtsr_In IfxQspi3_MTSRB_P10_6_IN;  /**< \brief Slave SPI data input */
IFX_EXTERN IfxQspi_Mtsr_In IfxQspi3_MTSRC_P01_6_IN;  /**< \brief Slave SPI data input */
IFX_EXTERN IfxQspi_Mtsr_In IfxQspi4_MTSRA_P33_12_IN;  /**< \brief Slave SPI data input */
IFX_EXTERN IfxQspi_Mtsr_In IfxQspi4_MTSRB_P22_0_IN;  /**< \brief Slave SPI data input */
IFX_EXTERN IfxQspi_Mtsr_In IfxQspi5_MTSRA_P15_14_IN;  /**< \brief Slave SPI data input */
IFX_EXTERN IfxQspi_Mtsr_In IfxQspi5_MTSRB_P14_6_IN;  /**< \brief Slave SPI data input */
IFX_EXTERN IfxQspi_Mtsr_Out IfxQspi0_MTSR_P20_12_OUT;  /**< \brief Master SPI data output */
IFX_EXTERN IfxQspi_Mtsr_Out IfxQspi0_MTSR_P20_14_OUT;  /**< \brief Master SPI data output */
IFX_EXTERN IfxQspi_Mtsr_Out IfxQspi0_MTSR_P22_10_OUT;  /**< \brief Master SPI data output */
IFX_EXTERN IfxQspi_Mtsr_Out IfxQspi0_MTSR_P22_5_OUT;  /**< \brief Master SPI data output */
IFX_EXTERN IfxQspi_Mtsr_Out IfxQspi1_MTSR_P10_1_OUT;  /**< \brief Master SPI data output */
IFX_EXTERN IfxQspi_Mtsr_Out IfxQspi1_MTSR_P10_3_OUT;  /**< \brief Master SPI data output */
IFX_EXTERN IfxQspi_Mtsr_Out IfxQspi1_MTSR_P10_4_OUT;  /**< \brief Master SPI data output */
IFX_EXTERN IfxQspi_Mtsr_Out IfxQspi1_MTSR_P11_9_OUT;  /**< \brief Master SPI data output */
IFX_EXTERN IfxQspi_Mtsr_Out IfxQspi2_MTSRN_P13_2_OUT;  /**< \brief Master SPI data output (LVDS N line) */
IFX_EXTERN IfxQspi_Mtsr_Out IfxQspi2_MTSRP_P13_3_OUT;  /**< \brief Master SPI data output (LVDS P line) */
IFX_EXTERN IfxQspi_Mtsr_Out IfxQspi2_MTSR_P15_5_OUT;  /**< \brief Master SPI data output */
IFX_EXTERN IfxQspi_Mtsr_Out IfxQspi2_MTSR_P15_6_OUT;  /**< \brief Master SPI data output */
IFX_EXTERN IfxQspi_Mtsr_Out IfxQspi2_MTSR_P34_5_OUT;  /**< \brief Master SPI data output */
IFX_EXTERN IfxQspi_Mtsr_Out IfxQspi3_MTSR_P01_6_OUT;  /**< \brief Master SPI data output */
IFX_EXTERN IfxQspi_Mtsr_Out IfxQspi3_MTSR_P02_6_OUT;  /**< \brief Master SPI data output */
IFX_EXTERN IfxQspi_Mtsr_Out IfxQspi3_MTSR_P10_6_OUT;  /**< \brief Master SPI data output */
IFX_EXTERN IfxQspi_Mtsr_Out IfxQspi4_MTSRN_P22_2_OUT;  /**< \brief Master SPI data output (LVDS N line) */
IFX_EXTERN IfxQspi_Mtsr_Out IfxQspi4_MTSRP_P22_3_OUT;  /**< \brief Master SPI data output (LVDS P line) */
IFX_EXTERN IfxQspi_Mtsr_Out IfxQspi4_MTSR_P22_0_OUT;  /**< \brief Master SPI data output */
IFX_EXTERN IfxQspi_Mtsr_Out IfxQspi4_MTSR_P33_12_OUT;  /**< \brief Master SPI data output */
IFX_EXTERN IfxQspi_Mtsr_Out IfxQspi5_MTSR_P14_6_OUT;  /**< \brief Master SPI data output */
IFX_EXTERN IfxQspi_Mtsr_Out IfxQspi5_MTSR_P15_14_OUT;  /**< \brief Master SPI data output */
IFX_EXTERN IfxQspi_Sclk_In IfxQspi0_SCLKA_P20_11_IN;  /**< \brief Slave SPI clock inputs */
IFX_EXTERN IfxQspi_Sclk_In IfxQspi0_SCLKB_P22_8_IN;  /**< \brief Slave SPI clock inputs */
IFX_EXTERN IfxQspi_Sclk_In IfxQspi0_SCLKC_P22_7_IN;  /**< \brief Slave SPI clock inputs */
IFX_EXTERN IfxQspi_Sclk_In IfxQspi1_SCLKA_P10_2_IN;  /**< \brief Slave SPI clock inputs */
IFX_EXTERN IfxQspi_Sclk_In IfxQspi1_SCLKB_P11_6_IN;  /**< \brief Slave SPI clock inputs */
IFX_EXTERN IfxQspi_Sclk_In IfxQspi2_SCLKA_P15_3_IN;  /**< \brief Slave SPI clock inputs */
IFX_EXTERN IfxQspi_Sclk_In IfxQspi2_SCLKB_P15_8_IN;  /**< \brief Slave SPI clock inputs */
IFX_EXTERN IfxQspi_Sclk_In IfxQspi2_SCLKD_P33_14_IN;  /**< \brief Slave SPI clock inputs */
IFX_EXTERN IfxQspi_Sclk_In IfxQspi3_SCLKA_P02_7_IN;  /**< \brief Slave SPI clock inputs */
IFX_EXTERN IfxQspi_Sclk_In IfxQspi3_SCLKB_P10_8_IN;  /**< \brief Slave SPI clock inputs */
IFX_EXTERN IfxQspi_Sclk_In IfxQspi3_SCLKC_P01_7_IN;  /**< \brief Slave SPI clock inputs */
IFX_EXTERN IfxQspi_Sclk_In IfxQspi4_SCLKA_P33_11_IN;  /**< \brief Slave SPI clock inputs */
IFX_EXTERN IfxQspi_Sclk_In IfxQspi4_SCLKB_P22_3_IN;  /**< \brief Slave SPI clock inputs */
IFX_EXTERN IfxQspi_Sclk_In IfxQspi5_SCLKA_P15_15_IN;  /**< \brief Slave SPI clock inputs */
IFX_EXTERN IfxQspi_Sclk_In IfxQspi5_SCLKB_P14_13_IN;  /**< \brief Slave SPI clock inputs */
IFX_EXTERN IfxQspi_Sclk_Out IfxQspi0_SCLK_P20_11_OUT;  /**< \brief Master SPI clock output */
IFX_EXTERN IfxQspi_Sclk_Out IfxQspi0_SCLK_P20_13_OUT;  /**< \brief Master SPI clock output */
IFX_EXTERN IfxQspi_Sclk_Out IfxQspi0_SCLK_P22_7_OUT;  /**< \brief Master SPI clock output */
IFX_EXTERN IfxQspi_Sclk_Out IfxQspi0_SCLK_P22_8_OUT;  /**< \brief Master SPI clock output */
IFX_EXTERN IfxQspi_Sclk_Out IfxQspi1_SCLK_P10_2_OUT;  /**< \brief Master SPI clock output */
IFX_EXTERN IfxQspi_Sclk_Out IfxQspi1_SCLK_P11_6_OUT;  /**< \brief Master SPI clock output */
IFX_EXTERN IfxQspi_Sclk_Out IfxQspi2_SCLKN_P13_0_OUT;  /**< \brief Master SPI clock output (LVDS N line) */
IFX_EXTERN IfxQspi_Sclk_Out IfxQspi2_SCLKP_P13_1_OUT;  /**< \brief Master SPI clock output (LVDS P line) */
IFX_EXTERN IfxQspi_Sclk_Out IfxQspi2_SCLK_P15_3_OUT;  /**< \brief Master SPI clock output */
IFX_EXTERN IfxQspi_Sclk_Out IfxQspi2_SCLK_P15_6_OUT;  /**< \brief Master SPI clock output */
IFX_EXTERN IfxQspi_Sclk_Out IfxQspi2_SCLK_P15_8_OUT;  /**< \brief Master SPI clock output */
IFX_EXTERN IfxQspi_Sclk_Out IfxQspi2_SCLK_P33_14_OUT;  /**< \brief Master SPI clock output */
IFX_EXTERN IfxQspi_Sclk_Out IfxQspi2_SCLK_P33_1_OUT;  /**< \brief Master SPI clock output */
IFX_EXTERN IfxQspi_Sclk_Out IfxQspi3_SCLK_P01_7_OUT;  /**< \brief Master SPI clock output */
IFX_EXTERN IfxQspi_Sclk_Out IfxQspi3_SCLK_P02_7_OUT;  /**< \brief Master SPI clock output */
IFX_EXTERN IfxQspi_Sclk_Out IfxQspi3_SCLK_P10_8_OUT;  /**< \brief Master SPI clock output */
IFX_EXTERN IfxQspi_Sclk_Out IfxQspi4_SCLKN_P22_0_OUT;  /**< \brief Master SPI clock output (LVDS N line) */
IFX_EXTERN IfxQspi_Sclk_Out IfxQspi4_SCLKP_P22_1_OUT;  /**< \brief Master SPI clock output (LVDS P line) */
IFX_EXTERN IfxQspi_Sclk_Out IfxQspi4_SCLK_P22_3_OUT;  /**< \brief Master SPI clock output */
IFX_EXTERN IfxQspi_Sclk_Out IfxQspi4_SCLK_P33_11_OUT;  /**< \brief Master SPI clock output */
IFX_EXTERN IfxQspi_Sclk_Out IfxQspi5_SCLK_P14_10_OUT;  /**< \brief Master SPI clock output */
IFX_EXTERN IfxQspi_Sclk_Out IfxQspi5_SCLK_P14_13_OUT;  /**< \brief Master SPI clock output */
IFX_EXTERN IfxQspi_Sclk_Out IfxQspi5_SCLK_P15_15_OUT;  /**< \brief Master SPI clock output */
IFX_EXTERN IfxQspi_Slsi_In IfxQspi0_SLSIA_P20_13_IN;  /**< \brief Slave select input */
IFX_EXTERN IfxQspi_Slsi_In IfxQspi0_SLSIB_P20_9_IN;  /**< \brief Slave select input */
IFX_EXTERN IfxQspi_Slsi_In IfxQspi1_SLSIA_P11_10_IN;  /**< \brief Slave select input */
IFX_EXTERN IfxQspi_Slsi_In IfxQspi2_SLSIA_P15_2_IN;  /**< \brief Slave select input */
IFX_EXTERN IfxQspi_Slsi_In IfxQspi2_SLSIB_P15_1_IN;  /**< \brief Slave select input */
IFX_EXTERN IfxQspi_Slsi_In IfxQspi3_SLSIA_P02_4_IN;  /**< \brief Slave select input */
IFX_EXTERN IfxQspi_Slsi_In IfxQspi3_SLSIB_P01_3_IN;  /**< \brief Slave select input */
IFX_EXTERN IfxQspi_Slsi_In IfxQspi4_SLSIA_P33_10_IN;  /**< \brief Slave select input */
IFX_EXTERN IfxQspi_Slsi_In IfxQspi4_SLSIB_P22_2_IN;  /**< \brief Slave select input */
IFX_EXTERN IfxQspi_Slsi_In IfxQspi5_SLSIA_P15_11_IN;  /**< \brief Slave select input */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi0_SLSO0_P20_8_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi0_SLSO10_P22_11_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi0_SLSO11_P23_6_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi0_SLSO12_P22_4_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi0_SLSO13_P15_0_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi0_SLSO1_P20_9_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi0_SLSO2_P20_13_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi0_SLSO3_P11_10_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi0_SLSO4_P11_11_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi0_SLSO5_P11_2_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi0_SLSO6_P20_10_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi0_SLSO7_P33_5_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi0_SLSO8_P20_6_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi0_SLSO9_P20_3_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi1_SLSO0_P20_8_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi1_SLSO10_P10_0_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi1_SLSO1_P20_9_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi1_SLSO2_P20_13_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi1_SLSO3_P11_10_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi1_SLSO4_P11_11_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi1_SLSO5_P11_2_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi1_SLSO6_P33_10_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi1_SLSO7_P33_5_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi1_SLSO8_P10_4_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi1_SLSO9_P10_5_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi2_SLSO0_P15_2_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi2_SLSO10_P33_2_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi2_SLSO10_P34_3_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi2_SLSO11_P33_15_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi2_SLSO11_P33_6_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi2_SLSO12_P32_6_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi2_SLSO12_P33_4_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi2_SLSO1_P14_2_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi2_SLSO2_P14_6_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi2_SLSO3_P14_3_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi2_SLSO4_P14_7_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi2_SLSO5_P15_1_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi2_SLSO6_P33_13_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi2_SLSO7_P20_10_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi2_SLSO8_P20_6_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi2_SLSO9_P20_3_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi3_SLSO0_P02_4_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi3_SLSO10_P01_4_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi3_SLSO1_P02_0_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi3_SLSO2_P02_1_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi3_SLSO3_P00_5_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi3_SLSO3_P02_2_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi3_SLSO4_P00_2_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi3_SLSO4_P02_3_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi3_SLSO5_P02_12_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi3_SLSO5_P02_8_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi3_SLSO6_P00_8_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi3_SLSO6_P02_15_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi3_SLSO7_P00_9_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi3_SLSO7_P02_13_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi3_SLSO8_P10_5_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi3_SLSO9_P01_3_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi4_SLSO0_P33_10_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi4_SLSO1_P33_9_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi4_SLSO2_P33_3_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi4_SLSO2_P33_8_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi4_SLSO3_P22_2_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi4_SLSO4_P02_12_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi4_SLSO4_P23_5_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi4_SLSO5_P02_15_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi4_SLSO5_P23_4_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi4_SLSO6_P02_13_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi4_SLSO6_P23_1_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi4_SLSO7_P02_1_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi4_SLSO7_P33_7_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi5_SLSO0_P15_13_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi5_SLSO1_P15_12_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi5_SLSO2_P15_11_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi5_SLSO3_P15_6_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi5_SLSO4_P13_14_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi5_SLSO5_P13_9_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi5_SLSO6_P14_12_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi5_SLSO7_P14_14_OUT;  /**< \brief Master slave select output */
IFX_EXTERN IfxQspi_Slso_Out IfxQspi5_SLSO8_P14_15_OUT;  /**< \brief Master slave select output */

/** \brief Table dimensions */
#define IFXQSPI_PINMAP_NUM_MODULES 6
#define IFXQSPI_PINMAP_NUM_SLAVESELECTS 14
#define IFXQSPI_PINMAP_HSICIN_IN_NUM_ITEMS 2
#define IFXQSPI_PINMAP_MRST_IN_NUM_ITEMS 6
#define IFXQSPI_PINMAP_MRST_OUT_NUM_ITEMS 3
#define IFXQSPI_PINMAP_MTSR_IN_NUM_ITEMS 4
#define IFXQSPI_PINMAP_MTSR_OUT_NUM_ITEMS 5
#define IFXQSPI_PINMAP_SCLK_IN_NUM_ITEMS 4
#define IFXQSPI_PINMAP_SCLK_OUT_NUM_ITEMS 7
#define IFXQSPI_PINMAP_SLSI_IN_NUM_ITEMS 2
#define IFXQSPI_PINMAP_SLSO_OUT_NUM_ITEMS 2


/** \brief IfxQspi_Hsicin_In table */
IFX_EXTERN const IfxQspi_Hsicin_In *IfxQspi_Hsicin_In_pinTable[IFXQSPI_PINMAP_NUM_MODULES][IFXQSPI_PINMAP_HSICIN_IN_NUM_ITEMS];

/** \brief IfxQspi_Mrst_In table */
IFX_EXTERN const IfxQspi_Mrst_In *IfxQspi_Mrst_In_pinTable[IFXQSPI_PINMAP_NUM_MODULES][IFXQSPI_PINMAP_MRST_IN_NUM_ITEMS];

/** \brief IfxQspi_Mrst_Out table */
IFX_EXTERN const IfxQspi_Mrst_Out *IfxQspi_Mrst_Out_pinTable[IFXQSPI_PINMAP_NUM_MODULES][IFXQSPI_PINMAP_MRST_OUT_NUM_ITEMS];

/** \brief IfxQspi_Mtsr_In table */
IFX_EXTERN const IfxQspi_Mtsr_In *IfxQspi_Mtsr_In_pinTable[IFXQSPI_PINMAP_NUM_MODULES][IFXQSPI_PINMAP_MTSR_IN_NUM_ITEMS];

/** \brief IfxQspi_Mtsr_Out table */
IFX_EXTERN const IfxQspi_Mtsr_Out *IfxQspi_Mtsr_Out_pinTable[IFXQSPI_PINMAP_NUM_MODULES][IFXQSPI_PINMAP_MTSR_OUT_NUM_ITEMS];

/** \brief IfxQspi_Sclk_In table */
IFX_EXTERN const IfxQspi_Sclk_In *IfxQspi_Sclk_In_pinTable[IFXQSPI_PINMAP_NUM_MODULES][IFXQSPI_PINMAP_SCLK_IN_NUM_ITEMS];

/** \brief IfxQspi_Sclk_Out table */
IFX_EXTERN const IfxQspi_Sclk_Out *IfxQspi_Sclk_Out_pinTable[IFXQSPI_PINMAP_NUM_MODULES][IFXQSPI_PINMAP_SCLK_OUT_NUM_ITEMS];

/** \brief IfxQspi_Slsi_In table */
IFX_EXTERN const IfxQspi_Slsi_In *IfxQspi_Slsi_In_pinTable[IFXQSPI_PINMAP_NUM_MODULES][IFXQSPI_PINMAP_SLSI_IN_NUM_ITEMS];

/** \brief IfxQspi_Slso_Out table */
IFX_EXTERN const IfxQspi_Slso_Out *IfxQspi_Slso_Out_pinTable[IFXQSPI_PINMAP_NUM_MODULES][IFXQSPI_PINMAP_NUM_SLAVESELECTS][IFXQSPI_PINMAP_SLSO_OUT_NUM_ITEMS];

/** \} */

#endif /* IFXQSPI_PINMAP_H */
