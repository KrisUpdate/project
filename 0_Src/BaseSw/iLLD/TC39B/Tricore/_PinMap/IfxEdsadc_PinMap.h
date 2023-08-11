/**
 * \file IfxEdsadc_PinMap.h
 * \brief EDSADC I/O map
 * \ingroup IfxLld_Edsadc
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
 * \defgroup IfxLld_Edsadc_pinmap EDSADC Pin Mapping
 * \ingroup IfxLld_Edsadc
 */

#ifndef IFXEDSADC_PINMAP_H
#define IFXEDSADC_PINMAP_H

#include <IfxEdsadc_reg.h>
#include <_Impl/IfxEdsadc_cfg.h>
#include <Port/Std/IfxPort.h>

/** \addtogroup IfxLld_Edsadc_pinmap
 * \{ */

/** \brief DS negative pin mapping structure */
typedef const struct
{
    Ifx_EDSADC*       module;    /**< \brief Base address */
    uint8             channel;   /**< \brief Channel number */
    IfxPort_Pin       pin;       /**< \brief Port pin */
    Ifx_RxSel         select;    /**< \brief Input multiplexer value */
} IfxEdsadc_Dsn_In;

/** \brief DS positive pin mapping structure */
typedef const struct
{
    Ifx_EDSADC*       module;   /**< \brief Base address */
    uint8             channel;  /**< \brief Channel number */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxEdsadc_Dsp_In;

/** \brief CIN pin mapping structure */
typedef const struct
{
    Ifx_EDSADC*       module;   /**< \brief Base address */
    uint8             channel;  /**< \brief Channel number */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxEdsadc_Cin_In;

/** \brief DIN pin mapping structure */
typedef const struct
{
    Ifx_EDSADC*       module;   /**< \brief Base address */
    uint8             channel;  /**< \brief Channel number */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxEdsadc_Din_In;

/** \brief DIN pin mapping structure */
typedef const struct
{
    Ifx_EDSADC*       module;   /**< \brief Base address */
    uint8             channel;  /**< \brief Channel number */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxEdsadc_Dscin_In;

/** \brief DIN pin mapping structure */
typedef const struct
{
    Ifx_EDSADC*       module;   /**< \brief Base address */
    uint8             channel;  /**< \brief Channel number */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxEdsadc_Dsdin_In;

/** \brief ITR pin mapping structure */
typedef const struct
{
    Ifx_EDSADC*       module;   /**< \brief Base address */
    uint8             channel;  /**< \brief Channel number */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxEdsadc_Itr_In;

/** \brief CGPWM pin mapping structure */
typedef const struct
{
    Ifx_EDSADC*       module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    IfxPort_OutputIdx select;   /**< \brief Port control code */
} IfxEdsadc_Cgpwm_Out;

/** \brief SG pin mapping structure */
typedef const struct
{
    Ifx_EDSADC*       module;   /**< \brief Base address */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    Ifx_RxSel         select;   /**< \brief Input multiplexer value */
} IfxEdsadc_Sg_In;

/** \brief COUT pin mapping structure */
typedef const struct
{
    Ifx_EDSADC*       module;   /**< \brief Base address */
    uint8             channel;  /**< \brief Channel number */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    IfxPort_OutputIdx select;   /**< \brief Port control code */
} IfxEdsadc_Dscout_Out;

/** \brief COUT pin mapping structure */
typedef const struct
{
    Ifx_EDSADC*       module;   /**< \brief Base address */
    uint8             channel;  /**< \brief Channel number */
    IfxPort_Pin       pin;      /**< \brief Port pin */
    IfxPort_OutputIdx select;   /**< \brief Port control code */
} IfxEdsadc_Cout_Out;

IFX_EXTERN IfxEdsadc_Cgpwm_Out IfxEdsadc_CGPWMN_P00_5_OUT;  /**< \brief Negative carrier generator output */
IFX_EXTERN IfxEdsadc_Cgpwm_Out IfxEdsadc_CGPWMN_P02_0_OUT;  /**< \brief Negative carrier generator output */
IFX_EXTERN IfxEdsadc_Cgpwm_Out IfxEdsadc_CGPWMN_P33_11_OUT;  /**< \brief Negative carrier generator output */
IFX_EXTERN IfxEdsadc_Cgpwm_Out IfxEdsadc_CGPWMP_P00_6_OUT;  /**< \brief Positive carrier generator output */
IFX_EXTERN IfxEdsadc_Cgpwm_Out IfxEdsadc_CGPWMP_P02_1_OUT;  /**< \brief Positive carrier generator output */
IFX_EXTERN IfxEdsadc_Cgpwm_Out IfxEdsadc_CGPWMP_P33_12_OUT;  /**< \brief Positive carrier generator output */
IFX_EXTERN IfxEdsadc_Dscin_In IfxEdsadc_DSCIN0A_P00_11_IN;  /**< \brief Modulator clock input, channel 0 */
IFX_EXTERN IfxEdsadc_Dscin_In IfxEdsadc_DSCIN0B_P33_5_IN;  /**< \brief Modulator clock input, channel 0 */
IFX_EXTERN IfxEdsadc_Dscin_In IfxEdsadc_DSCIN10A_P01_13_IN;  /**< \brief Modulator clock input, channel 10 */
IFX_EXTERN IfxEdsadc_Dscin_In IfxEdsadc_DSCIN11A_P01_15_IN;  /**< \brief Modulator clock input, channel 11 */
IFX_EXTERN IfxEdsadc_Dscin_In IfxEdsadc_DSCIN12A_P02_13_IN;  /**< \brief Modulator clock input, channel 12 */
IFX_EXTERN IfxEdsadc_Dscin_In IfxEdsadc_DSCIN13A_P02_15_IN;  /**< \brief Modulator clock input, channel 13 */
IFX_EXTERN IfxEdsadc_Dscin_In IfxEdsadc_DSCIN1A_P00_9_IN;  /**< \brief Modulator clock input, channel 1 */
IFX_EXTERN IfxEdsadc_Dscin_In IfxEdsadc_DSCIN1B_P33_3_IN;  /**< \brief Modulator clock input, channel 1 */
IFX_EXTERN IfxEdsadc_Dscin_In IfxEdsadc_DSCIN2A_P00_5_IN;  /**< \brief Modulator clock input, channel 2 */
IFX_EXTERN IfxEdsadc_Dscin_In IfxEdsadc_DSCIN2B_P33_1_IN;  /**< \brief Modulator clock input, channel 2 */
IFX_EXTERN IfxEdsadc_Dscin_In IfxEdsadc_DSCIN3A_P00_3_IN;  /**< \brief Modulator clock input, channel 3 */
IFX_EXTERN IfxEdsadc_Dscin_In IfxEdsadc_DSCIN3B_P02_7_IN;  /**< \brief Modulator clock input, channel 3 */
IFX_EXTERN IfxEdsadc_Dscin_In IfxEdsadc_DSCIN4A_P00_7_IN;  /**< \brief Modulator clock input, channel 4 */
IFX_EXTERN IfxEdsadc_Dscin_In IfxEdsadc_DSCIN4B_P02_5_IN;  /**< \brief Modulator clock input, channel 4 */
IFX_EXTERN IfxEdsadc_Dscin_In IfxEdsadc_DSCIN5A_P00_1_IN;  /**< \brief Modulator clock input, channel 5 */
IFX_EXTERN IfxEdsadc_Dscin_In IfxEdsadc_DSCIN5B_P02_3_IN;  /**< \brief Modulator clock input, channel 5 */
IFX_EXTERN IfxEdsadc_Dscin_In IfxEdsadc_DSCIN6A_P00_14_IN;  /**< \brief Modulator clock input, channel 6 */
IFX_EXTERN IfxEdsadc_Dscin_In IfxEdsadc_DSCIN7A_P01_2_IN;  /**< \brief Modulator clock input, channel 7 */
IFX_EXTERN IfxEdsadc_Dscin_In IfxEdsadc_DSCIN7B_P00_1_IN;  /**< \brief Modulator clock input, channel 7 */
IFX_EXTERN IfxEdsadc_Dscin_In IfxEdsadc_DSCIN8A_P01_5_IN;  /**< \brief Modulator clock input, channel 8 */
IFX_EXTERN IfxEdsadc_Dscin_In IfxEdsadc_DSCIN9A_P01_9_IN;  /**< \brief Modulator clock input, channel 9 */
IFX_EXTERN IfxEdsadc_Dscout_Out IfxEdsadc_DSCOUT0_P00_11_OUT;  /**< \brief Modulator clock output */
IFX_EXTERN IfxEdsadc_Dscout_Out IfxEdsadc_DSCOUT0_P33_5_OUT;  /**< \brief Modulator clock output */
IFX_EXTERN IfxEdsadc_Dscout_Out IfxEdsadc_DSCOUT10_P01_13_OUT;  /**< \brief Modulator clock output */
IFX_EXTERN IfxEdsadc_Dscout_Out IfxEdsadc_DSCOUT11_P01_15_OUT;  /**< \brief Modulator clock output */
IFX_EXTERN IfxEdsadc_Dscout_Out IfxEdsadc_DSCOUT12_P02_13_OUT;  /**< \brief Modulator clock output */
IFX_EXTERN IfxEdsadc_Dscout_Out IfxEdsadc_DSCOUT13_P02_15_OUT;  /**< \brief Modulator clock output */
IFX_EXTERN IfxEdsadc_Dscout_Out IfxEdsadc_DSCOUT1_P00_9_OUT;  /**< \brief Modulator clock output */
IFX_EXTERN IfxEdsadc_Dscout_Out IfxEdsadc_DSCOUT1_P33_3_OUT;  /**< \brief Modulator clock output */
IFX_EXTERN IfxEdsadc_Dscout_Out IfxEdsadc_DSCOUT2_P00_5_OUT;  /**< \brief Modulator clock output */
IFX_EXTERN IfxEdsadc_Dscout_Out IfxEdsadc_DSCOUT2_P33_1_OUT;  /**< \brief Modulator clock output */
IFX_EXTERN IfxEdsadc_Dscout_Out IfxEdsadc_DSCOUT3_P00_3_OUT;  /**< \brief Modulator clock output */
IFX_EXTERN IfxEdsadc_Dscout_Out IfxEdsadc_DSCOUT3_P02_7_OUT;  /**< \brief Modulator clock output */
IFX_EXTERN IfxEdsadc_Dscout_Out IfxEdsadc_DSCOUT4_P00_7_OUT;  /**< \brief Modulator clock output */
IFX_EXTERN IfxEdsadc_Dscout_Out IfxEdsadc_DSCOUT4_P02_5_OUT;  /**< \brief Modulator clock output */
IFX_EXTERN IfxEdsadc_Dscout_Out IfxEdsadc_DSCOUT5_P00_1_OUT;  /**< \brief Modulator clock output */
IFX_EXTERN IfxEdsadc_Dscout_Out IfxEdsadc_DSCOUT5_P02_3_OUT;  /**< \brief Modulator clock output */
IFX_EXTERN IfxEdsadc_Dscout_Out IfxEdsadc_DSCOUT6_P00_14_OUT;  /**< \brief Modulator clock output */
IFX_EXTERN IfxEdsadc_Dscout_Out IfxEdsadc_DSCOUT7_P00_1_OUT;  /**< \brief Modulator clock output */
IFX_EXTERN IfxEdsadc_Dscout_Out IfxEdsadc_DSCOUT7_P01_2_OUT;  /**< \brief Modulator clock output */
IFX_EXTERN IfxEdsadc_Dscout_Out IfxEdsadc_DSCOUT8_P01_5_OUT;  /**< \brief Modulator clock output */
IFX_EXTERN IfxEdsadc_Dscout_Out IfxEdsadc_DSCOUT9_P01_9_OUT;  /**< \brief Modulator clock output */
IFX_EXTERN IfxEdsadc_Dsdin_In IfxEdsadc_DSDIN0A_P00_12_IN;  /**< \brief Digital datastream input, channel 0 */
IFX_EXTERN IfxEdsadc_Dsdin_In IfxEdsadc_DSDIN0B_P33_6_IN;  /**< \brief Digital datastream input, channel 0 */
IFX_EXTERN IfxEdsadc_Dsdin_In IfxEdsadc_DSDIN10A_P01_12_IN;  /**< \brief Digital datastream input, channel 10 */
IFX_EXTERN IfxEdsadc_Dsdin_In IfxEdsadc_DSDIN11A_P01_14_IN;  /**< \brief Digital datastream input, channel 11 */
IFX_EXTERN IfxEdsadc_Dsdin_In IfxEdsadc_DSDIN12A_P02_12_IN;  /**< \brief Digital datastream input, channel 12 */
IFX_EXTERN IfxEdsadc_Dsdin_In IfxEdsadc_DSDIN13A_P02_14_IN;  /**< \brief Digital datastream input, channel 13 */
IFX_EXTERN IfxEdsadc_Dsdin_In IfxEdsadc_DSDIN1A_P00_10_IN;  /**< \brief Digital datastream input, channel 1 */
IFX_EXTERN IfxEdsadc_Dsdin_In IfxEdsadc_DSDIN1B_P33_4_IN;  /**< \brief Digital datastream input, channel 1 */
IFX_EXTERN IfxEdsadc_Dsdin_In IfxEdsadc_DSDIN2A_P00_6_IN;  /**< \brief Digital datastream input, channel 2 */
IFX_EXTERN IfxEdsadc_Dsdin_In IfxEdsadc_DSDIN2B_P33_2_IN;  /**< \brief Digital datastream input, channel 2 */
IFX_EXTERN IfxEdsadc_Dsdin_In IfxEdsadc_DSDIN3A_P00_4_IN;  /**< \brief Digital datastream input, channel 3 */
IFX_EXTERN IfxEdsadc_Dsdin_In IfxEdsadc_DSDIN3B_P02_8_IN;  /**< \brief Digital datastream input, channel 3 */
IFX_EXTERN IfxEdsadc_Dsdin_In IfxEdsadc_DSDIN4A_P00_8_IN;  /**< \brief Digital datastream input, channel 4 */
IFX_EXTERN IfxEdsadc_Dsdin_In IfxEdsadc_DSDIN4B_P02_6_IN;  /**< \brief Digital datastream input, channel 4 */
IFX_EXTERN IfxEdsadc_Dsdin_In IfxEdsadc_DSDIN5A_P00_2_IN;  /**< \brief Digital datastream input, channel 5 */
IFX_EXTERN IfxEdsadc_Dsdin_In IfxEdsadc_DSDIN5B_P02_4_IN;  /**< \brief Digital datastream input, channel 5 */
IFX_EXTERN IfxEdsadc_Dsdin_In IfxEdsadc_DSDIN6A_P00_13_IN;  /**< \brief Digital datastream input, channel 6 */
IFX_EXTERN IfxEdsadc_Dsdin_In IfxEdsadc_DSDIN7A_P01_15_IN;  /**< \brief Digital datastream input, channel 7 */
IFX_EXTERN IfxEdsadc_Dsdin_In IfxEdsadc_DSDIN7B_P00_2_IN;  /**< \brief Digital datastream input, channel 7 */
IFX_EXTERN IfxEdsadc_Dsdin_In IfxEdsadc_DSDIN8A_P01_6_IN;  /**< \brief Digital datastream input, channel 8 */
IFX_EXTERN IfxEdsadc_Dsdin_In IfxEdsadc_DSDIN9A_P01_8_IN;  /**< \brief Digital datastream input, channel 9 */
IFX_EXTERN IfxEdsadc_Dsn_In IfxEdsadc_DS0NA_AN3_IN;  /**< \brief Negative analog input channel 0, pin A */
IFX_EXTERN IfxEdsadc_Dsn_In IfxEdsadc_DS0NB_AN13_IN;  /**< \brief Negative analog input channel 0, pin B */
IFX_EXTERN IfxEdsadc_Dsn_In IfxEdsadc_DS10NA_AN59_IN;  /**< \brief Negative analog input channel 10, pin A */
IFX_EXTERN IfxEdsadc_Dsn_In IfxEdsadc_DS11NA_P40_15_IN;  /**< \brief Negative analog input channel 11, pin A */
IFX_EXTERN IfxEdsadc_Dsn_In IfxEdsadc_DS12NA_P41_3_IN;  /**< \brief Negative analog input channel 12, pin A */
IFX_EXTERN IfxEdsadc_Dsn_In IfxEdsadc_DS13NA_AN73_IN;  /**< \brief Negative analog input channel 13, pin A */
IFX_EXTERN IfxEdsadc_Dsn_In IfxEdsadc_DS1NA_P40_7_IN;  /**< \brief Negative analog input channel 1, pin A */
IFX_EXTERN IfxEdsadc_Dsn_In IfxEdsadc_DS1NB_P40_9_IN;  /**< \brief Negative analog input channel 1, pin B */
IFX_EXTERN IfxEdsadc_Dsn_In IfxEdsadc_DS1NC_AN45_IN;  /**< \brief Negative analog input channel 1, pin C */
IFX_EXTERN IfxEdsadc_Dsn_In IfxEdsadc_DS1ND_AN47_IN;  /**< \brief Negative analog input channel 1, pin D */
IFX_EXTERN IfxEdsadc_Dsn_In IfxEdsadc_DS2NA_AN21_IN;  /**< \brief Negative analog input channel 2, pin A */
IFX_EXTERN IfxEdsadc_Dsn_In IfxEdsadc_DS2NB_P40_1_IN;  /**< \brief Negative analog input channel 2, pin B */
IFX_EXTERN IfxEdsadc_Dsn_In IfxEdsadc_DS3NA_AN1_IN;  /**< \brief Negative analog input channel 3, pin A */
IFX_EXTERN IfxEdsadc_Dsn_In IfxEdsadc_DS3NB_AN15_IN;  /**< \brief Negative analog input channel 3, pin N */
IFX_EXTERN IfxEdsadc_Dsn_In IfxEdsadc_DS4NA_P00_7_IN;  /**< \brief Negative analog input channel 4, pin A */
IFX_EXTERN IfxEdsadc_Dsn_In IfxEdsadc_DS4NB_P00_9_IN;  /**< \brief Negative analog input channel 4, pin B */
IFX_EXTERN IfxEdsadc_Dsn_In IfxEdsadc_DS5NA_P00_1_IN;  /**< \brief Negative analog input channel 5, pin A */
IFX_EXTERN IfxEdsadc_Dsn_In IfxEdsadc_DS5NB_P00_3_IN;  /**< \brief Negative analog input channel 5, pin B */
IFX_EXTERN IfxEdsadc_Dsn_In IfxEdsadc_DS6NA_AN53_IN;  /**< \brief Negative analog input channel 6, pin A */
IFX_EXTERN IfxEdsadc_Dsn_In IfxEdsadc_DS6NB_P41_5_IN;  /**< \brief Negative analog input channel 6, pin B */
IFX_EXTERN IfxEdsadc_Dsn_In IfxEdsadc_DS7NA_AN61_IN;  /**< \brief Negative analog input channel 7, pin A */
IFX_EXTERN IfxEdsadc_Dsn_In IfxEdsadc_DS7NB_P41_7_IN;  /**< \brief Negative analog input channel 7, pin B */
IFX_EXTERN IfxEdsadc_Dsn_In IfxEdsadc_DS8NA_P41_1_IN;  /**< \brief Negative analog input channel 8, pin A */
IFX_EXTERN IfxEdsadc_Dsn_In IfxEdsadc_DS9NA_AN51_IN;  /**< \brief Negative analog input channel 9, pin A */
IFX_EXTERN IfxEdsadc_Dsn_In IfxEdsadc_DS9NB_P41_3_IN;  /**< \brief Negative analog input channel 9, pin B */
IFX_EXTERN IfxEdsadc_Dsp_In IfxEdsadc_DS0PA_AN2_IN;  /**< \brief Positive analog input channel 0, pin A */
IFX_EXTERN IfxEdsadc_Dsp_In IfxEdsadc_DS0PB_AN12_IN;  /**< \brief Positive analog input channel 0, pin B */
IFX_EXTERN IfxEdsadc_Dsp_In IfxEdsadc_DS10PA_AN58_IN;  /**< \brief Positive analog input channel 10, pin A */
IFX_EXTERN IfxEdsadc_Dsp_In IfxEdsadc_DS11PA_AN66_IN;  /**< \brief Positive analog input channel 11, pin A */
IFX_EXTERN IfxEdsadc_Dsp_In IfxEdsadc_DS12PA_P41_2_IN;  /**< \brief Positive analog input channel 12, pin A */
IFX_EXTERN IfxEdsadc_Dsp_In IfxEdsadc_DS13PA_AN72_IN;  /**< \brief Positive analog input channel 13, pin A */
IFX_EXTERN IfxEdsadc_Dsp_In IfxEdsadc_DS1PA_P40_6_IN;  /**< \brief Positive analog input channel 1, pin A */
IFX_EXTERN IfxEdsadc_Dsp_In IfxEdsadc_DS1PB_P40_8_IN;  /**< \brief Positive analog input channel 1, pin B */
IFX_EXTERN IfxEdsadc_Dsp_In IfxEdsadc_DS1PC_AN44_IN;  /**< \brief Positive analog input channel 1, pin C */
IFX_EXTERN IfxEdsadc_Dsp_In IfxEdsadc_DS1PD_AN46_IN;  /**< \brief Positive analog input channel 1, pin D */
IFX_EXTERN IfxEdsadc_Dsp_In IfxEdsadc_DS2PA_AN20_IN;  /**< \brief Positive analog input channel 2, pin A */
IFX_EXTERN IfxEdsadc_Dsp_In IfxEdsadc_DS2PB_P40_0_IN;  /**< \brief Positive analog input channel 2, pin B */
IFX_EXTERN IfxEdsadc_Dsp_In IfxEdsadc_DS3PA_AN0_IN;  /**< \brief Positive analog input channel 3, pin A */
IFX_EXTERN IfxEdsadc_Dsp_In IfxEdsadc_DS3PB_AN14_IN;  /**< \brief Positive analog input channel 3, pin B */
IFX_EXTERN IfxEdsadc_Dsp_In IfxEdsadc_DS4PA_P00_8_IN;  /**< \brief Positive analog input channel 4, pin A */
IFX_EXTERN IfxEdsadc_Dsp_In IfxEdsadc_DS4PB_P00_10_IN;  /**< \brief Positive analog input channel 4, pin B */
IFX_EXTERN IfxEdsadc_Dsp_In IfxEdsadc_DS5PA_P00_2_IN;  /**< \brief Positive analog input channel 5, pin A */
IFX_EXTERN IfxEdsadc_Dsp_In IfxEdsadc_DS5PB_P00_4_IN;  /**< \brief Positive analog input channel 5, pin B */
IFX_EXTERN IfxEdsadc_Dsp_In IfxEdsadc_DS6PA_AN52_IN;  /**< \brief Positive analog input channel 6, pin A */
IFX_EXTERN IfxEdsadc_Dsp_In IfxEdsadc_DS6PB_P41_4_IN;  /**< \brief Positive analog input channel 6, pin B */
IFX_EXTERN IfxEdsadc_Dsp_In IfxEdsadc_DS7PA_AN60_IN;  /**< \brief Positive analog input channel 7, pin A */
IFX_EXTERN IfxEdsadc_Dsp_In IfxEdsadc_DS7PB_P41_6_IN;  /**< \brief Positive analog input channel 7, pin B */
IFX_EXTERN IfxEdsadc_Dsp_In IfxEdsadc_DS8PA_P41_0_IN;  /**< \brief Positive analog input channel 8, pin A */
IFX_EXTERN IfxEdsadc_Dsp_In IfxEdsadc_DS9PA_AN50_IN;  /**< \brief Positive analog input channel 9, pin A */
IFX_EXTERN IfxEdsadc_Dsp_In IfxEdsadc_DS9PB_P41_2_IN;  /**< \brief Positive analog input channel 9, pin B */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR0E_P33_0_IN;  /**< \brief Trigger/Gate input, channel 0 */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR0F_P33_4_IN;  /**< \brief Trigger/Gate input, channel 0 */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR10E_P01_13_IN;  /**< \brief Trigger/Gate input, channel 10 */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR10F_P01_12_IN;  /**< \brief Trigger/Gate input, channel 10 */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR11E_P01_15_IN;  /**< \brief Trigger/Gate input, channel 11 */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR11F_P01_14_IN;  /**< \brief Trigger/Gate input, channel 11 */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR12E_P02_13_IN;  /**< \brief Trigger/Gate input, channel 12 */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR12F_P02_12_IN;  /**< \brief Trigger/Gate input, channel 12 */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR13E_P02_15_IN;  /**< \brief Trigger/Gate input, channel 13 */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR13F_P02_14_IN;  /**< \brief Trigger/Gate input, channel 13 */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR1E_P33_1_IN;  /**< \brief Trigger/Gate input, channel 1 */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR1F_P33_5_IN;  /**< \brief Trigger/Gate input, channel 1 */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR2E_P33_2_IN;  /**< \brief Trigger/Gate input, channel 2 */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR2F_P33_6_IN;  /**< \brief Trigger/Gate input, channel 2 */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR3E_P02_8_IN;  /**< \brief Trigger/Gate input, channel 3 */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR3F_P00_9_IN;  /**< \brief Trigger/Gate input, channel 3 */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR4E_P02_7_IN;  /**< \brief Trigger/Gate input, channel 4 */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR4F_P00_6_IN;  /**< \brief Trigger/Gate input, channel 4 */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR5E_P02_6_IN;  /**< \brief Trigger/Gate input, channel 5 */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR5F_P00_3_IN;  /**< \brief Trigger/Gate input, channel 5 */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR6E_P01_0_IN;  /**< \brief Trigger/Gate input, channel 6 */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR6F_P00_15_IN;  /**< \brief Trigger/Gate input, channel 6 */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR7E_P01_4_IN;  /**< \brief Trigger/Gate input, channel 7 */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR7F_P01_3_IN;  /**< \brief Trigger/Gate input, channel 7 */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR8E_P01_1_IN;  /**< \brief Trigger/Gate input, channel 8 */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR8F_P01_7_IN;  /**< \brief Trigger/Gate input, channel 8 */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR9E_P01_11_IN;  /**< \brief Trigger/Gate input, channel 9 */
IFX_EXTERN IfxEdsadc_Itr_In IfxEdsadc_ITR9F_P01_10_IN;  /**< \brief Trigger/Gate input, channel 9 */
IFX_EXTERN IfxEdsadc_Sg_In IfxEdsadc_SGNA_P00_4_IN;  /**< \brief Carrier sign signal input */
IFX_EXTERN IfxEdsadc_Sg_In IfxEdsadc_SGNB_P33_13_IN;  /**< \brief Carrier sign signal input */

/** \brief Table dimensions */
#define IFXEDSADC_PINMAP_NUM_MODULES 1
#define IFXEDSADC_PINMAP_NUM_CHANNELS 14
#define IFXEDSADC_PINMAP_CGPWM_OUT_NUM_ITEMS 6
#define IFXEDSADC_PINMAP_DSCIN_IN_NUM_ITEMS 2
#define IFXEDSADC_PINMAP_DSCOUT_OUT_NUM_ITEMS 2
#define IFXEDSADC_PINMAP_DSDIN_IN_NUM_ITEMS 2
#define IFXEDSADC_PINMAP_DSN_IN_NUM_ITEMS 4
#define IFXEDSADC_PINMAP_DSP_IN_NUM_ITEMS 4
#define IFXEDSADC_PINMAP_ITR_IN_NUM_ITEMS 6
#define IFXEDSADC_PINMAP_SG_IN_NUM_ITEMS 2


/** \brief IfxEdsadc_Cgpwm_Out table */
IFX_EXTERN const IfxEdsadc_Cgpwm_Out *IfxEdsadc_Cgpwm_Out_pinTable[IFXEDSADC_PINMAP_NUM_MODULES][IFXEDSADC_PINMAP_CGPWM_OUT_NUM_ITEMS];

/** \brief IfxEdsadc_Dscin_In table */
IFX_EXTERN const IfxEdsadc_Dscin_In *IfxEdsadc_Dscin_In_pinTable[IFXEDSADC_PINMAP_NUM_MODULES][IFXEDSADC_PINMAP_NUM_CHANNELS][IFXEDSADC_PINMAP_DSCIN_IN_NUM_ITEMS];

/** \brief IfxEdsadc_Dscout_Out table */
IFX_EXTERN const IfxEdsadc_Dscout_Out *IfxEdsadc_Dscout_Out_pinTable[IFXEDSADC_PINMAP_NUM_MODULES][IFXEDSADC_PINMAP_NUM_CHANNELS][IFXEDSADC_PINMAP_DSCOUT_OUT_NUM_ITEMS];

/** \brief IfxEdsadc_Dsdin_In table */
IFX_EXTERN const IfxEdsadc_Dsdin_In *IfxEdsadc_Dsdin_In_pinTable[IFXEDSADC_PINMAP_NUM_MODULES][IFXEDSADC_PINMAP_NUM_CHANNELS][IFXEDSADC_PINMAP_DSDIN_IN_NUM_ITEMS];

/** \brief IfxEdsadc_Dsn_In table */
IFX_EXTERN const IfxEdsadc_Dsn_In *IfxEdsadc_Dsn_In_pinTable[IFXEDSADC_PINMAP_NUM_MODULES][IFXEDSADC_PINMAP_NUM_CHANNELS][IFXEDSADC_PINMAP_DSN_IN_NUM_ITEMS];

/** \brief IfxEdsadc_Dsp_In table */
IFX_EXTERN const IfxEdsadc_Dsp_In *IfxEdsadc_Dsp_In_pinTable[IFXEDSADC_PINMAP_NUM_MODULES][IFXEDSADC_PINMAP_NUM_CHANNELS][IFXEDSADC_PINMAP_DSP_IN_NUM_ITEMS];

/** \brief IfxEdsadc_Itr_In table */
IFX_EXTERN const IfxEdsadc_Itr_In *IfxEdsadc_Itr_In_pinTable[IFXEDSADC_PINMAP_NUM_MODULES][IFXEDSADC_PINMAP_NUM_CHANNELS][IFXEDSADC_PINMAP_ITR_IN_NUM_ITEMS];

/** \brief IfxEdsadc_Sg_In table */
IFX_EXTERN const IfxEdsadc_Sg_In *IfxEdsadc_Sg_In_pinTable[IFXEDSADC_PINMAP_NUM_MODULES][IFXEDSADC_PINMAP_SG_IN_NUM_ITEMS];

/** \} */

#endif /* IFXEDSADC_PINMAP_H */
