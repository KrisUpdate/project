/**
 * \file IfxGtm_Spe.c
 * \brief GTM  basic functionality
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
 *
 */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "IfxGtm_Spe.h"

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

void IfxGtm_Spe_disableInterrupt(Ifx_GTM_SPE *spe, IfxGtm_Spe_InterruptSignal interrupt)
{
    uint32 irqen_temp = spe->IRQ.EN.U;
    irqen_temp    = (irqen_temp ^ ((uint32)1 << interrupt));
    spe->IRQ.EN.U = irqen_temp;
}


void IfxGtm_Spe_enableErrorInterrupt(Ifx_GTM_SPE *spe, IfxGtm_Spe_InterruptSignal interrupt, boolean enabled)
{
    uint32 irqen_temp = spe->EIRQ_EN.U;
    irqen_temp     = (irqen_temp | ((uint32)1 << interrupt)) & ((uint32)enabled << interrupt);
    spe->EIRQ_EN.U = irqen_temp;
}


void IfxGtm_Spe_enableInput(Ifx_GTM_SPE *spe, IfxGtm_Spe_InputSignal inputSignal, boolean enabled)
{
    switch (enabled)
    {
    case TRUE:
    {
        spe->CTRL_STAT.U |= ((uint32)enabled << ((uint32)inputSignal + IFX_GTM_SPE_CTRL_STAT_SIE0_OFF));
        break;
    }
    case FALSE:
        spe->CTRL_STAT.U &= ((uint32)enabled << ((uint32)inputSignal + IFX_GTM_SPE_CTRL_STAT_SIE0_OFF));
        break;
    }
}


void IfxGtm_Spe_enableInterrupt(Ifx_GTM_SPE *spe, IfxGtm_Spe_InterruptSignal interrupt)
{
    uint32 irqen_temp = spe->IRQ.EN.U;
    irqen_temp    = (irqen_temp | ((uint32)1 << interrupt));
    spe->IRQ.EN.U = irqen_temp;
}


uint32 IfxGtm_Spe_getFastShutoffLevel(Ifx_GTM_SPE *spe, IfxGtm_Spe_OutChannel channel)
{
    uint32 fsol_temp = (uint32)(spe->CTRL_STAT.B.FSOL);
    fsol_temp = (fsol_temp & (uint32)(1 << (uint32)channel)) >> (uint32)channel;
    return (uint32)fsol_temp;
}


boolean IfxGtm_Spe_getInterruptStatus(Ifx_GTM_SPE *spe, IfxGtm_Spe_InterruptSignal interrupt)
{
    return (((spe->IRQ.NOTIFY.U) >> interrupt) & (uint32)1) == (uint32)1;
}


uint32 IfxGtm_Spe_getRevCompare(Ifx_GTM_SPE *spe)
{
    return (uint32)(spe->REV_CMP.B.REV_CMP);
}


void IfxGtm_Spe_selectTrigger(Ifx_GTM_SPE *spe, IfxGtm_Spe_TriggerSource trigger)
{
    Ifx_GTM_SPE_CTRL_STAT ctrl_stat;
    ctrl_stat.U           = spe->CTRL_STAT.U;

    ctrl_stat.B.TRIG_SEL  = IFXGTM_SPE_GET_TRIGSEL(trigger);
    ctrl_stat.B.ETRIG_SEL = IFXGTM_SPE_GET_ETRIG(trigger);

    spe->CTRL_STAT.U      = ctrl_stat.U;
}


void IfxGtm_Spe_setAllInputPatterns(Ifx_GTM_SPE *spe, IfxGtm_Spe_InputPattern *patPtr)
{
    uint32 spepat_temp = spe->PAT.U;

    spepat_temp = IFXGTM_SPE_GETALLPATTERNS(patPtr);

    spe->PAT.U  = spepat_temp;
}


void IfxGtm_Spe_setInputPattern(Ifx_GTM_SPE *spe, IfxGtm_Spe_InputPattern *patPtr, IfxGtm_Spe_PatternIndex patIndex)
{
    uint32 pattern     = patPtr->U;
    uint32 spepat_temp = spe->PAT.U;

    spepat_temp = IFXGTM_SPE_SETSINGLEPATTERN(spepat_temp, pattern, patIndex);

    spe->PAT.U  = spepat_temp;
}


void IfxGtm_Spe_setInterrupt(Ifx_GTM_SPE *spe, IfxGtm_Spe_InterruptSignal interrupt)
{
    uint32 forceirq_temp = spe->IRQ.FORCINT.U;
    forceirq_temp      = (forceirq_temp | ((uint32)1 << interrupt));
    spe->IRQ.FORCINT.U = forceirq_temp;
}


void IfxGtm_Spe_setOutputPattern(Ifx_GTM_SPE *spe, IfxGtm_Spe_OutputPattern *patPtr, IfxGtm_Spe_PatternIndex patIndex)
{
    spe->OUT_PAT[patIndex].U = patPtr->U;
}


void IfxGtm_Spe_setRevCompare(Ifx_GTM_SPE *spe, uint32 count)
{
    spe->REV_CMP.B.REV_CMP = count;
}
