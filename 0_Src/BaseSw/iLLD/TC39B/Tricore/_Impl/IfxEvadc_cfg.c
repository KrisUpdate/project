/**
 * \file IfxEvadc_cfg.c
 * \brief EVADC on-chip implementation data
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
 */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "IfxEvadc_cfg.h"

/******************************************************************************/
/*-----------------------Exported Variables/Constants-------------------------*/
/******************************************************************************/

volatile Ifx_SRC_SRCR *IfxEvadc_cfg_srcAddresses[(IFXEVADC_NUM_ADC_PRIMARY_GROUPS * 4) + (IFXEVADC_NUM_ADC_SECONDARY_GROUPS * 4) + (IFXEVADC_NUM_FAST_COMPARE_CHANNELS * 4) + (IFXEVADC_NUM_ADC_COMMON_GROUPS * 4)] = {
/* Primary Group 0*/
    &SRC_VADCG0SR0,
    &SRC_VADCG0SR1,
    &SRC_VADCG0SR2,
    &SRC_VADCG0SR3,
/* Primary Group 1*/
    &SRC_VADCG1SR0,
    &SRC_VADCG1SR1,
    &SRC_VADCG1SR2,
    &SRC_VADCG1SR3,
/* Primary Group 2*/
    &SRC_VADCG2SR0,
    &SRC_VADCG2SR1,
    &SRC_VADCG2SR2,
    &SRC_VADCG2SR3,
/* Primary Group 3*/
    &SRC_VADCG3SR0,
    &SRC_VADCG3SR1,
    &SRC_VADCG3SR2,
    &SRC_VADCG3SR3,
/* Primary Group 4*/
    &SRC_VADCG4SR0,
    &SRC_VADCG4SR1,
    &SRC_VADCG4SR2,
    &SRC_VADCG4SR3,
/* Primary Group 5*/
    &SRC_VADCG5SR0,
    &SRC_VADCG5SR1,
    &SRC_VADCG5SR2,
    &SRC_VADCG5SR3,
/* Primary Group 6*/
    &SRC_VADCG6SR0,
    &SRC_VADCG6SR1,
    &SRC_VADCG6SR2,
    &SRC_VADCG6SR3,
/* Primary Group 7*/
    &SRC_VADCG7SR0,
    &SRC_VADCG7SR1,
    &SRC_VADCG7SR2,
    &SRC_VADCG7SR3,

/* Secondary Group 0*/
    &SRC_VADCG8SR0,
    &SRC_VADCG8SR1,
    &SRC_VADCG8SR2,
    &SRC_VADCG8SR3,
/* Secondary Group 1*/
    &SRC_VADCG9SR0,
    &SRC_VADCG9SR1,
    &SRC_VADCG9SR2,
    &SRC_VADCG9SR3,
/* Secondary Group 2*/
    &SRC_VADCG10SR0,
    &SRC_VADCG10SR1,
    &SRC_VADCG10SR2,
    &SRC_VADCG10SR3,
/* Secondary Group 3*/
    &SRC_VADCG11SR0,
    &SRC_VADCG11SR1,
    &SRC_VADCG11SR2,
    &SRC_VADCG11SR3,

/* Fast Compare Channel 0*/
    &SRC_VADCFC0SR0,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
/* Fast Compare Channel 1*/
    &SRC_VADCFC1SR0,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
/* Fast Compare Channel 2*/
    &SRC_VADCFC2SR0,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
/* Fast Compare Channel 3*/
    &SRC_VADCFC3SR0,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
/* Fast Compare Channel 4*/
    &SRC_VADCFC4SR0,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
/* Fast Compare Channel 5*/
    &SRC_VADCFC5SR0,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
/* Fast Compare Channel 6*/
    &SRC_VADCFC6SR0,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
/* Fast Compare Channel 7*/
    &SRC_VADCFC7SR0,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,

/*Common Group 0*/
    &SRC_VADCCG0SR0,
    &SRC_VADCCG0SR1,
    &SRC_VADCCG0SR2,
    &SRC_VADCCG0SR3,
/*Common Group 1*/
    &SRC_VADCCG1SR0,
    &SRC_VADCCG1SR1,
    &SRC_VADCCG1SR2,
    &SRC_VADCCG1SR3
};
