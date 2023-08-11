/**
 * \file IfxRif_PinMap.c
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
 */

#include "IfxRif_PinMap.h"

IfxRif_ExRamp_In IfxRif_RAMP1B_P02_6_IN = {&MODULE_RIF0, {&MODULE_P02, 6}, Ifx_RxSel_a};
IfxRif_ExRamp_In IfxRif_RAMP1B_P10_8_IN = {&MODULE_RIF1, {&MODULE_P10, 8}, Ifx_RxSel_a};


const IfxRif_ExRamp_In *IfxRif_ExRamp_In_pinTable[IFXRIF_PINMAP_NUM_MODULES][IFXRIF_PINMAP_EXRAMP_IN_NUM_ITEMS] = {
    {
        &IfxRif_RAMP1B_P02_6_IN
    },
    {
        &IfxRif_RAMP1B_P10_8_IN
    }
};
