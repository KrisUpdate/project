/**
 * \file IfxEdsadc.c
 * \brief EDSADC  basic functionality
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

#include "IfxEdsadc.h"

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

void IfxEdsadc_disableModule(Ifx_EDSADC *edsadc)
{
    uint16 passwd = IfxScuWdt_getCpuWatchdogPassword();
    IfxScuWdt_clearCpuEndinit(passwd);
    edsadc->CLC.B.DISR = 1;

    IfxScuWdt_setCpuEndinit(passwd);
}


void IfxEdsadc_enableModule(Ifx_EDSADC *edsadc)
{
    uint16 passwd = IfxScuWdt_getCpuWatchdogPassword();
    IfxScuWdt_clearCpuEndinit(passwd);
    edsadc->CLC.U = 0x00000000;

    if (edsadc->CLC.U)
    {}                       /* sync access */

    IfxScuWdt_setCpuEndinit(passwd);
}


volatile Ifx_SRC_SRCR *IfxEdsadc_getAuxSrc(Ifx_EDSADC *edsadc, IfxEdsadc_ChannelId channel)
{
    return &(MODULE_SRC.DSADC.DSADC[channel].SRA);
}


float32 IfxEdsadc_getIntegratorOutFreq(Ifx_EDSADC *edsadc, IfxEdsadc_ChannelId channel)
{
    float32             frequency = IfxEdsadc_getMainCombOutFreq(edsadc, channel);

    frequency = frequency / ((float32)1.0 + edsadc->CH[channel].IWCTR.B.NVALINT);
    Ifx_EDSADC_CH_FCFGM fcfgm = edsadc->CH[channel].FCFGM;

    if (fcfgm.B.FIR0EN != 0)
    {
        frequency = frequency / 2;
    }

    if (fcfgm.B.FIR1EN != 0)
    {
        frequency = frequency / 2;
    }

    return frequency;
}


float32 IfxEdsadc_getMainCombOutFreq(Ifx_EDSADC *edsadc, IfxEdsadc_ChannelId channel)
{
    float32 frequency = IfxEdsadc_getModulatorClockFreq(edsadc, channel);

    return frequency / ((float32)1.0 + edsadc->CH[channel].FCFGC.B.CFMDF);
}


float32 IfxEdsadc_getMainGroupDelay(Ifx_EDSADC *edsadc, IfxEdsadc_ChannelId channel)
{
    Ifx_EDSADC_CH_FCFGC fcfgc  = edsadc->CH[channel].FCFGC;
    Ifx_EDSADC_CH_FCFGM fcfgm  = edsadc->CH[channel].FCFGM;
    Ifx_EDSADC_CH_DICFG dicfg  = edsadc->CH[channel].DICFG;
    Ifx_EDSADC_CH_IWCTR iwctr  = edsadc->CH[channel].IWCTR;
    float32             tsMod  = 1.0 / IfxEdsadc_getModulatorClockFreq(edsadc, channel);
    float32             tsPre  = (fcfgm.B.PFEN == 1) ? (tsMod * 1.5) : (tsMod * (1 + fcfgm.B.PFEN));
    float32             tsComb = tsPre * (fcfgc.B.CFMDF);
    float32             tsFir0 = tsComb * (1 + fcfgm.B.FIR0EN);
    float32             tsFir1 = tsFir0 * (1 + fcfgm.B.FIR1EN);

    float32             gdDsA  = 7 * tsMod;
    uint32              combN  = fcfgc.B.CFMDF;
    float32             gdPre  = (fcfgm.B.PFEN == 1) ? tsPre : 0;
    float32             gdComb = tsPre * (1.5 * (combN - 1));
    float32             gdFir0 = (fcfgm.B.FIR0EN != 0) ? (3.5 * tsComb) + 3 : 0;
    float32             gdFir1 = (fcfgm.B.FIR1EN != 0) ? (27 * tsFir0) + tsFir0 + 5 : 0;
    float32             gdInt  = (dicfg.B.ITRMODE == IfxEdsadc_IntegratorTrigger_bypassed) ? 0 : (0.5 * (iwctr.B.NVALINT + 1) * tsFir1);

    return gdDsA + gdPre + gdComb + gdFir0 + gdFir1 + gdInt;
}


volatile Ifx_SRC_SRCR *IfxEdsadc_getMainSrc(Ifx_EDSADC *edsadc, IfxEdsadc_ChannelId channel)
{
    return &(MODULE_SRC.DSADC.DSADC[channel].SRM);
}


float32 IfxEdsadc_getModulatorClockFreq(Ifx_EDSADC *edsadc, IfxEdsadc_ChannelId channel)
{
    float32 sourceFreq = IfxEdsadc_getModulatorInputClockFreq(edsadc);

    return sourceFreq / (((edsadc->CH[channel].MODCFG.B.DIVM + 1) * 2) + 2);
}


float32 IfxEdsadc_getModulatorInputClockFreq(Ifx_EDSADC *edsadc)
{
    float32 result;

#ifdef IFX_CFG_AURIX_SCUCCU_USED
    result = IfxScuCcu_getSpbFrequency();
#else
    result = IfxScuCcu_getAdcFrequency();
#endif

    return result;
}


void IfxEdsadc_resetModule(Ifx_EDSADC *edsadc)
{
    uint16 passwd = IfxScuWdt_getCpuWatchdogPassword();

    IfxScuWdt_clearCpuEndinit(passwd);
    edsadc->KRST0.B.RST = 1;            /* Only if both Kernel reset bits are set a reset is executed */
    edsadc->KRST1.B.RST = 1;
    IfxScuWdt_setCpuEndinit(passwd);

    while (0 == edsadc->KRST0.B.RSTSTAT)    /* Wait until reset is executed */

    {}

    IfxScuWdt_clearCpuEndinit(passwd);
    edsadc->KRSTCLR.B.CLR = 1;          /* Clear Kernel reset status bit */
    IfxScuWdt_setCpuEndinit(passwd);
}


void IfxEdsadc_setCommonModeVoltage(Ifx_EDSADC *edsadc, IfxEdsadc_ChannelId channel, IfxEdsadc_FractionalRefVoltageSelection voltage, IfxEdsadc_FractionalRefVoltage refMode, IfxEdsadc_HalfSupplyVoltage hsvMode, IfxEdsadc_CommonModeHoldVoltage cmhvMode, uint8 positiveInput, uint8 negativeInput)
{
    Ifx_EDSADC_CH_VCM tempVCM;
    tempVCM.U                 = edsadc->CH[channel].VCM.U;
    tempVCM.B.VREFXSEL        = voltage;
    tempVCM.B.VXON            = refMode;
    tempVCM.U                |= positiveInput << IFX_EDSADC_CH_VCM_INPVC0_OFF;
    tempVCM.U                |= negativeInput << IFX_EDSADC_CH_VCM_INNVC0_OFF;
    edsadc->CH[channel].VCM.U = tempVCM.U;
}


void IfxEdsadc_setGainCorrectionFactor(Ifx_EDSADC *edsadc, IfxEdsadc_ChannelId channel, uint16 gainFactor, uint8 cicShift)
{
    Ifx_EDSADC_CH_GAINCORR tempGAINCORR;

    tempGAINCORR.U                 = edsadc->CH[channel].GAINCORR.U;
    tempGAINCORR.B.GAINFACTOR      = gainFactor;
    tempGAINCORR.B.CICSHIFT        = cicShift;
    edsadc->CH[channel].GAINCORR.U = tempGAINCORR.U;
}


void IfxEdsadc_setTimeStampCounter(Ifx_EDSADC *edsadc, IfxEdsadc_ChannelId channel, IfxEdsadc_TimeStampCounterClockSelection timeStampCounterClock, IfxEdsadc_TimeStampCounterControl runControl, IfxEdsadc_AnalogMux analogMuxCopy)
{
    Ifx_EDSADC_CH_TSCNT tempTSCNT;
    tempTSCNT.U                 = edsadc->CH[channel].TSCNT.U;
    tempTSCNT.B.TSCLK           = timeStampCounterClock;
    tempTSCNT.B.TSCRUN          = runControl;
    tempTSCNT.B.AMXCOPY         = analogMuxCopy;
    edsadc->CH[channel].TSCNT.U = tempTSCNT.U;
}


void IfxEdsadc_setGainControl(Ifx_EDSADC *edsadc, IfxEdsadc_ChannelId channel, uint16 gainFactor, uint8 cicShift, uint8 cicDecimationRate)
{
    Ifx_EDSADC_CH_GAINCTR tempGAINCTR;

    tempGAINCTR.U                 = edsadc->CH[channel].GAINCTR.U;
    tempGAINCTR.B.GAINFACTOR      = gainFactor;
    tempGAINCTR.B.CICSHIFT        = cicShift;
    tempGAINCTR.B.CICDEC          = cicDecimationRate;
    edsadc->CH[channel].GAINCTR.U = tempGAINCTR.U;
}


void IfxEdsadc_setGainCalibration(Ifx_EDSADC *edsadc, IfxEdsadc_ChannelId channel, uint16 calFactor, uint16 calTarget)
{
    Ifx_EDSADC_CH_GAINCAL tempGAINCAL;
    tempGAINCAL.U                 = edsadc->CH[channel].GAINCAL.U;
    tempGAINCAL.B.CALFACTOR       = calFactor;
    tempGAINCAL.B.CALTARGET       = calTarget;
    edsadc->CH[channel].GAINCAL.U = tempGAINCAL.U;
}


void IfxEdsadc_configureModulator(Ifx_EDSADC *edsadc, IfxEdsadc_ChannelId channel, IfxEdsadc_ModulatorConfig *config)
{
    Ifx_EDSADC_CH_MODCFG modcfg;

    modcfg.U                     = 0;

    modcfg.B.INCFGP              = config->positiveInput;
    modcfg.B.INCFGN              = config->negativeInput;
    modcfg.B.GAINSEL             = config->inputGain;
    modcfg.B.INSEL               = config->inputPin;
    modcfg.B.INMAC               = config->inputMuxActionControl;
    modcfg.B.INMODE              = config->triggerEvent;
    modcfg.B.INCWC               = 1; // enable write access for these bitfields
    modcfg.B.DIVM                = config->modulatorClockPeriod;
    modcfg.B.ACSD                = config->analogClockSyncDelay;
    modcfg.B.DITHEN              = config->ditheringEnabled;
    modcfg.B.IREN                = config->integratorResetEnabled;
    modcfg.B.MMWC                = 1; // enable write access for these bitfields

    edsadc->CH[channel].MODCFG.U = modcfg.U;
}
