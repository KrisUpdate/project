/**
 * \file IfxGtm_cfg.c
 * \brief GTM on-chip implementation data
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

#include "IfxGtm_cfg.h"

/******************************************************************************/
/*-----------------------Exported Variables/Constants-------------------------*/
/******************************************************************************/

IFX_CONST IfxModule_IndexMap IfxGtm_spe_indexMap[IFXGTM_NUM_SPE_OBJECTS] = {
    {&(MODULE_GTM.SPE[0]), (uint32)IfxGtm_Spe_0},
    {&(MODULE_GTM.SPE[1]), (uint32)IfxGtm_Spe_1},
    {&(MODULE_GTM.SPE[2]), (uint32)IfxGtm_Spe_2},
    {&(MODULE_GTM.SPE[3]), (uint32)IfxGtm_Spe_3},
    {&(MODULE_GTM.SPE[4]), (uint32)IfxGtm_Spe_4},
    {&(MODULE_GTM.SPE[5]), (uint32)IfxGtm_Spe_5}
};
