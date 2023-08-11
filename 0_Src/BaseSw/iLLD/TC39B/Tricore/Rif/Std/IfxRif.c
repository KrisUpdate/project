/**
 * \file IfxRif.c
 * \brief RIF  basic functionality
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

#include "IfxRif.h"

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

void IfxRif_disableModule(Ifx_RIF *rif)
{
    uint16 password = IfxScuWdt_getCpuWatchdogPassword();
    IfxScuWdt_clearCpuEndinit(password);
    rif->CLC.B.DISR = 1U;
    IfxScuWdt_setCpuEndinit(password);
}


void IfxRif_enableFifos(Ifx_RIF *rif, uint8 count)
{
    IfxRif_disableAllFifos(rif);

    switch (count)
    {
    case 0:     /*All FIFOs disabled*/
        break;
    case 1:
        IfxRif_enableFifo(rif, IfxRif_FifoId_0);
        break;
    case 2:
        IfxRif_enableFifo(rif, IfxRif_FifoId_0);
        IfxRif_enableFifo(rif, IfxRif_FifoId_1);
        break;
    case 3:
    case 4:
        IfxRif_enableFifo(rif, IfxRif_FifoId_0);
        IfxRif_enableFifo(rif, IfxRif_FifoId_1);
        IfxRif_enableFifo(rif, IfxRif_FifoId_2);
        IfxRif_enableFifo(rif, IfxRif_FifoId_3);
        break;
    default:
        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, FALSE); /* wrong selection  */
        break;
    }
}


void IfxRif_enableModule(Ifx_RIF *rif)
{
    uint16 password = IfxScuWdt_getCpuWatchdogPassword();
    IfxScuWdt_clearCpuEndinit(password);
    rif->CLC.B.DISR = 0U;
    IfxScuWdt_setCpuEndinit(password);

    /* Wait until module is enabled */
    while (IfxRif_isModuleEnabled(rif) == FALSE)
    {}
}


Ifx_RIF *IfxRif_getAddress(IfxRif_Index rif)
{
    Ifx_RIF *module;

    if (rif < IFXRIF_NUM_MODULES)
    {
        module = (Ifx_RIF *)IfxRif_cfg_indexMap[rif].module;
    }
    else
    {
        module = NULL_PTR;
    }

    return module;
}


IfxRif_Index IfxRif_getIndex(Ifx_RIF *rif)
{
    uint32       index;
    IfxRif_Index result;

    result = IfxRif_Index_none;

    for (index = 0; index < IFXRIF_NUM_MODULES; index++)
    {
        if (IfxRif_cfg_indexMap[index].module == rif)
        {
            result = (IfxRif_Index)IfxRif_cfg_indexMap[index].index;
            break;
        }
    }

    return result;
}


volatile Ifx_SRC_SRCR *IfxRif_getSrcPointerErr(Ifx_RIF *rif)
{
    return &MODULE_SRC.RIF.RIF[IfxRif_getIndex(rif)].ERR;
}


volatile Ifx_SRC_SRCR *IfxRif_getSrcPointerInt(Ifx_RIF *rif)
{
    return &MODULE_SRC.RIF.RIF[IfxRif_getIndex(rif)].INT;
}


void IfxRif_disableFifo(Ifx_RIF *rif, IfxRif_FifoId fifoId)
{
    uint32 setValue = ~(1U << fifoId);
    rif->DMI.U = (rif->DMI.U & setValue);
}


void IfxRif_disableAllFifos(Ifx_RIF *rif)
{
    rif->DMI.U = 0U;
}
