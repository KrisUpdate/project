/**
 * \file IfxAsclin_cfg.c
 * \brief ASCLIN on-chip implementation data
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

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "IfxAsclin_cfg.h"

/******************************************************************************/
/*-----------------------Exported Variables/Constants-------------------------*/
/******************************************************************************/

IFX_CONST IfxModule_IndexMap IfxAsclin_cfg_indexMap[IFXASCLIN_NUM_MODULES] = {
    {&MODULE_ASCLIN0,  (uint32)IfxAsclin_Index_0 },
    {&MODULE_ASCLIN1,  (uint32)IfxAsclin_Index_1 },
    {&MODULE_ASCLIN2,  (uint32)IfxAsclin_Index_2 },
    {&MODULE_ASCLIN3,  (uint32)IfxAsclin_Index_3 },
    {&MODULE_ASCLIN4,  (uint32)IfxAsclin_Index_4 },
    {&MODULE_ASCLIN5,  (uint32)IfxAsclin_Index_5 },
    {&MODULE_ASCLIN6,  (uint32)IfxAsclin_Index_6 },
    {&MODULE_ASCLIN7,  (uint32)IfxAsclin_Index_7 },
    {&MODULE_ASCLIN8,  (uint32)IfxAsclin_Index_8 },
    {&MODULE_ASCLIN9,  (uint32)IfxAsclin_Index_9 },
    {&MODULE_ASCLIN10, (uint32)IfxAsclin_Index_10},
    {&MODULE_ASCLIN11, (uint32)IfxAsclin_Index_11},
};
