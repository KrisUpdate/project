/**
 * \file IfxHspdm_PinMap.c
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
 */

#include "IfxHspdm_PinMap.h"

IfxHspdm_Bs_Out IfxHspdm0_BS0_OUT_P22_4_OUT = {&MODULE_HSPDM, {&MODULE_P22, 4}, IfxPort_OutputIdx_alt7};
IfxHspdm_Bs_Out IfxHspdm0_BS1_OUT_P22_5_OUT = {&MODULE_HSPDM, {&MODULE_P22, 5}, IfxPort_OutputIdx_alt7};
IfxHspdm_Mute_Out IfxHspdm0_MUTE_P22_3_OUT = {&MODULE_HSPDM, {&MODULE_P22, 3}, IfxPort_OutputIdx_alt7};


const IfxHspdm_Bs_Out *IfxHspdm_Bs_Out_pinTable[IFXHSPDM_PINMAP_NUM_MODULES][IFXHSPDM_PINMAP_NUM_0S][IFXHSPDM_PINMAP_BS_OUT_NUM_ITEMS] = {
    {
        {
            &IfxHspdm0_BS0_OUT_P22_4_OUT,
            &IfxHspdm0_BS1_OUT_P22_5_OUT
        }
    }
};

const IfxHspdm_Mute_Out *IfxHspdm_Mute_Out_pinTable[IFXHSPDM_PINMAP_NUM_MODULES][IFXHSPDM_PINMAP_NUM_0S][IFXHSPDM_PINMAP_MUTE_OUT_NUM_ITEMS] = {
    {
        {
            &IfxHspdm0_MUTE_P22_3_OUT
        }
    }
};
