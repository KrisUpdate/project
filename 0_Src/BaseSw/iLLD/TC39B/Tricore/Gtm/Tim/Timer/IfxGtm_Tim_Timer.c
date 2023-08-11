/**
 * \file IfxGtm_Tim_Timer.c
 * \brief GTM TIMER details
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

#include "IfxGtm_Tim_Timer.h"
#include "_Utilities/Ifx_Assert.h"

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

void IfxGtm_Tim_Timer_initChannel(IfxGtm_Tim_Timer *driver, IfxGtm_Tim_Timer_Config *config)
{
    driver->gtm      = &MODULE_GTM;
    driver->timIndex = config->timIndex;

    driver->tim      = &driver->gtm->TIM[config->timIndex];
    driver->channel  = config->channel;

    Ifx_GTM_TIM_CH *channel = IfxGtm_Tim_getChannel(driver->tim, driver->channel);

    IfxGtm_Tim_Ch_setControl(channel, config->channelControl);

    if (config->enableAuxInputSourceSelect)
    {
        IfxGtm_Tim_Ch_setAuxInputSourceSelection(driver->gtm, driver->timIndex, driver->channel);
    }

    IfxGtm_Tim_Ch_setRisingEdgeFilterTime(channel, config->raisingEdgeFilterTime);
    IfxGtm_Tim_Ch_setFallingEdgeFilterTime(channel, config->fallingEdgeFilterTime);
    IfxGtm_Tim_Ch_setInputSourceSelection(driver->tim, driver->channel, config->inputSourceSelect);

    if (config->irqEnable)
    {
        IfxGtm_Tim_Ch_setNotificationMode(channel, config->irqMode);
        IfxGtm_Tim_Ch_setNotification(channel, config->irqType);
    }

    if ((config->channelControl.mode == IfxGtm_Tim_Mode_inputPrescaler) || (config->channelControl.mode == IfxGtm_Tim_Mode_bitCompression))
    {
        IfxGtm_Tim_Ch_setShadowCounter(channel, config->shadowCounter);
    }
}


void IfxGtm_Tim_Timer_initChannelConfig(IfxGtm_Tim_Timer_Config *config, Ifx_GTM *gtm)
{
    const IfxGtm_Tim_Timer_Config IfxGtm_Tim_Timer_defaultChannelConfig = {
        .timIndex       = IfxGtm_Tim_0,
        .channel        = IfxGtm_Tim_Ch_0,
        .channelControl = {        //
            .enable                       = 1,
            .mode                         = IfxGtm_Tim_Mode_pwmMeasurement,
            .enableOneShotMode            = 0,
            .enableAruRouting             = 0,
            .channelInputControl          = 0,
            .gpr0Sel                      = IfxGtm_Tim_GprSel_tbuTs0,
            .gpr1Sel                      = IfxGtm_Tim_GprSel_tbuTs0,
            .cntsSel                      = IfxGtm_Tim_CntsSel_cntReg,
            .signalLevelControl           = 0,
            .ignoreSignalLevel            = 0,
            .enableCounterReset           = 0,
            .enableFilter                 = 0,
            .filterCounterFrequency       = IfxGtm_Tim_FilterCounterFreqSel_cmuClk0,
            .enableExternalCaptureMode    = 0,
            .filterModeRisingEdge         = IfxGtm_Tim_FilterMode_immediateEdgePropagation,
            .filterCounterModeRisingEdge  = IfxGtm_Tim_FilterCounter_upDown,
            .filterModeFallingEdge        = IfxGtm_Tim_FilterMode_immediateEdgePropagation,
            .filterCounterModeFallingEdge = IfxGtm_Tim_FilterCounter_upDown,
            .clkSel                       = IfxGtm_Cmu_Clk_0,
            .extendEdgeCounterOverflow    = 0,
            .egpr0Sel                     = 0,
            .egpr1Sel                     = 0,
            .timeoutControl               = IfxGtm_Tim_Timeout_disabled,
        },
        .raisingEdgeFilterTime = 0,
        .fallingEdgeFilterTime = 0,
        .shadowCounter         = 0,
        .irqEnable             = 0,
        .irqMode               = IfxGtm_IrqMode_level,
        .irqType               = IfxGtm_Tim_IrqType_newVal,
        .inputSourceSelect     = {
            .mode  = 0,
            .value = 0,
        },
        .enableAuxInputSourceSelect       = 0,
    };

    *config = IfxGtm_Tim_Timer_defaultChannelConfig;
}
