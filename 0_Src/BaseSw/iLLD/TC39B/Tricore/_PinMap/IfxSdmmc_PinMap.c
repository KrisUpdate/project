/**
 * \file IfxSdmmc_PinMap.c
 * \brief SDMMC I/O map
 * \ingroup IfxLld_Sdmmc
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

#include "IfxSdmmc_PinMap.h"

IfxSdmmc_Clk_Out IfxSdmmc0_CLK_P15_1_OUT = {&MODULE_SDMMC0, {&MODULE_P15, 1}, IfxPort_OutputIdx_alt7};
IfxSdmmc_Cmd_InOut IfxSdmmc0_CMD_P15_3_INOUT = {&MODULE_SDMMC0, {&MODULE_P15, 3}, Ifx_RxSel_a, IfxPort_OutputIdx_general};
IfxSdmmc_Dat0_InOut IfxSdmmc0_DAT0_P20_7_INOUT = {&MODULE_SDMMC0, {&MODULE_P20, 7}, Ifx_RxSel_a, IfxPort_OutputIdx_general};
IfxSdmmc_Dat1_InOut IfxSdmmc0_DAT1_P20_8_INOUT = {&MODULE_SDMMC0, {&MODULE_P20, 8}, Ifx_RxSel_a, IfxPort_OutputIdx_general};
IfxSdmmc_Dat2_InOut IfxSdmmc0_DAT2_P20_10_INOUT = {&MODULE_SDMMC0, {&MODULE_P20,10}, Ifx_RxSel_a, IfxPort_OutputIdx_general};
IfxSdmmc_Dat3_InOut IfxSdmmc0_DAT3_P20_11_INOUT = {&MODULE_SDMMC0, {&MODULE_P20,11}, Ifx_RxSel_a, IfxPort_OutputIdx_general};
IfxSdmmc_Dat4_InOut IfxSdmmc0_DAT4_P20_12_INOUT = {&MODULE_SDMMC0, {&MODULE_P20,12}, Ifx_RxSel_a, IfxPort_OutputIdx_general};
IfxSdmmc_Dat5_InOut IfxSdmmc0_DAT5_P20_13_INOUT = {&MODULE_SDMMC0, {&MODULE_P20,13}, Ifx_RxSel_a, IfxPort_OutputIdx_general};
IfxSdmmc_Dat6_InOut IfxSdmmc0_DAT6_P20_14_INOUT = {&MODULE_SDMMC0, {&MODULE_P20,14}, Ifx_RxSel_a, IfxPort_OutputIdx_general};
IfxSdmmc_Dat7_InOut IfxSdmmc0_DAT7_P15_0_INOUT = {&MODULE_SDMMC0, {&MODULE_P15, 0}, Ifx_RxSel_a, IfxPort_OutputIdx_general};


const IfxSdmmc_Clk_Out *IfxSdmmc_Clk_Out_pinTable[IFXSDMMC_PINMAP_NUM_MODULES][IFXSDMMC_PINMAP_CLK_OUT_NUM_ITEMS] = {
    {
        &IfxSdmmc0_CLK_P15_1_OUT
    }
};

const IfxSdmmc_Cmd_InOut *IfxSdmmc_Cmd_InOut_pinTable[IFXSDMMC_PINMAP_NUM_MODULES][IFXSDMMC_PINMAP_CMD_INOUT_NUM_ITEMS] = {
    {
        &IfxSdmmc0_CMD_P15_3_INOUT
    }
};

const IfxSdmmc_Dat0_InOut *IfxSdmmc_Dat0_InOut_pinTable[IFXSDMMC_PINMAP_NUM_MODULES][IFXSDMMC_PINMAP_DAT0_INOUT_NUM_ITEMS] = {
    {
        &IfxSdmmc0_DAT0_P20_7_INOUT
    }
};

const IfxSdmmc_Dat1_InOut *IfxSdmmc_Dat1_InOut_pinTable[IFXSDMMC_PINMAP_NUM_MODULES][IFXSDMMC_PINMAP_DAT1_INOUT_NUM_ITEMS] = {
    {
        &IfxSdmmc0_DAT1_P20_8_INOUT
    }
};

const IfxSdmmc_Dat2_InOut *IfxSdmmc_Dat2_InOut_pinTable[IFXSDMMC_PINMAP_NUM_MODULES][IFXSDMMC_PINMAP_DAT2_INOUT_NUM_ITEMS] = {
    {
        &IfxSdmmc0_DAT2_P20_10_INOUT
    }
};

const IfxSdmmc_Dat3_InOut *IfxSdmmc_Dat3_InOut_pinTable[IFXSDMMC_PINMAP_NUM_MODULES][IFXSDMMC_PINMAP_DAT3_INOUT_NUM_ITEMS] = {
    {
        &IfxSdmmc0_DAT3_P20_11_INOUT
    }
};

const IfxSdmmc_Dat4_InOut *IfxSdmmc_Dat4_InOut_pinTable[IFXSDMMC_PINMAP_NUM_MODULES][IFXSDMMC_PINMAP_DAT4_INOUT_NUM_ITEMS] = {
    {
        &IfxSdmmc0_DAT4_P20_12_INOUT
    }
};

const IfxSdmmc_Dat5_InOut *IfxSdmmc_Dat5_InOut_pinTable[IFXSDMMC_PINMAP_NUM_MODULES][IFXSDMMC_PINMAP_DAT5_INOUT_NUM_ITEMS] = {
    {
        &IfxSdmmc0_DAT5_P20_13_INOUT
    }
};

const IfxSdmmc_Dat6_InOut *IfxSdmmc_Dat6_InOut_pinTable[IFXSDMMC_PINMAP_NUM_MODULES][IFXSDMMC_PINMAP_DAT6_INOUT_NUM_ITEMS] = {
    {
        &IfxSdmmc0_DAT6_P20_14_INOUT
    }
};

const IfxSdmmc_Dat7_InOut *IfxSdmmc_Dat7_InOut_pinTable[IFXSDMMC_PINMAP_NUM_MODULES][IFXSDMMC_PINMAP_DAT7_INOUT_NUM_ITEMS] = {
    {
        &IfxSdmmc0_DAT7_P15_0_INOUT
    }
};
