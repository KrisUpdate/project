/**
 * \file IfxEdsadc_Edsadc.c
 * \brief EDSADC EDSADC details
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

#include "IfxEdsadc_Edsadc.h"

/** \addtogroup IfxLld_Edsadc_Edsadc_Module
 * \{ */

/******************************************************************************/
/*-----------------------Private Function Prototypes--------------------------*/
/******************************************************************************/

/** \brief calculate division factor for modulator clock period
 * \param sourceFreq Source frequency
 * \param targetFreq Target frequency
 * \return division factor
 */
IFX_STATIC sint32 IfxEdsadc_Edsadc_calcDIVM(float32 sourceFreq, float32 *targetFreq);

/** \} */

/******************************************************************************/
/*-----------------------Private Function Prototypes--------------------------*/
/******************************************************************************/

/** \brief calculate division factor for pwm patteren mode
 * \param sourceFreq Source Frequency
 * \param targetFreq Target Frequency
 * \return Division Factor
 */
IFX_STATIC sint32 IfxEdsadc_Edsadc_calcDIVCG(float32 sourceFreq, float32 *targetFreq);

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

IFX_STATIC sint32 IfxEdsadc_Edsadc_calcDIVM(float32 sourceFreq, float32 *targetFreq)
{
    float32 bestError = 10e6;
    sint32  bestDiv   = 4, i;

    for (i = 4; i <= 32; i += 2)
    {
        float32 freq  = sourceFreq / i;
        float32 error = __absf(freq - *targetFreq);

        if (__leqf(error, bestError))
        {
            bestError = error;
            bestDiv   = i;

            if (!__neqf(error, 0))
            {
                break;
            }
        }
    }

    *targetFreq = sourceFreq / bestDiv;

    return (bestDiv / 2) - 2;
}


void IfxEdsadc_Edsadc_initCarrierGen(IfxEdsadc_Edsadc *edsadc, IfxEdsadc_Edsadc_CarrierGenConfig *config)
{
    Ifx_EDSADC_CGCFG cgcfg;

    cgcfg.U = 0;

    float32          sourceFreq = IfxEdsadc_getModulatorInputClockFreq(edsadc->edsadc);
    float32          targetFreq = config->frequency;
    cgcfg.B.DIVCG         = IfxEdsadc_Edsadc_calcDIVCG(sourceFreq / (32 * 32), &targetFreq);
    cgcfg.B.SIGPOL        = (config->inverted == FALSE) ? 0 : 1;
    cgcfg.B.BREV          = (config->bitReversed == FALSE) ? 0 : 1;
    cgcfg.B.CGMOD         = config->carrierWaveformMode;

    edsadc->edsadc->CGCFG = cgcfg;

    const IfxEdsadc_Cgpwm_Out *pinPos = config->pinPos;

    if (pinPos != NULL_PTR)
    {                           /* Initialise positive carrier pin */
        IfxEdsadc_initCgPwmPin(pinPos, config->pinMode, config->pinDriver);
    }

    const IfxEdsadc_Cgpwm_Out *pinNeg = config->pinNeg;

    if (pinNeg != NULL_PTR)
    {                           /* Initialise negative carrier pin */
        IfxEdsadc_initCgPwmPin(pinNeg, config->pinMode, config->pinDriver);
    }
}


void IfxEdsadc_Edsadc_initChannel(IfxEdsadc_Edsadc_Channel *channel, const IfxEdsadc_Edsadc_ChannelConfig *config)
{
    Ifx_EDSADC *edsadc = config->module;

    channel->channelId = config->channelId;
    channel->module    = edsadc;
    channel->channel   = (Ifx_EDSADC_CH *)&edsadc->CH[config->channelId];

    IfxEdsadc_Edsadc_initModulator(channel, &config->modulator);
    IfxEdsadc_Edsadc_initDemodulator(channel, &config->demodulator);
    IfxEdsadc_Edsadc_initCombFilter(channel, &config->combFilter);
    IfxEdsadc_Edsadc_initFirFilter(channel, &config->firFilter);
    IfxEdsadc_Edsadc_initIntegrator(channel, &config->integrator);
    IfxEdsadc_Edsadc_initRectifier(channel, &config->rectifier);
    IfxEdsadc_Edsadc_setGainCorrectionFactor(edsadc, channel->channelId, config->gainFactor, config->cicShift);

    IfxEdsadc_Edsadc_initTimeStampCounter(edsadc, channel->channelId, &config->timeStamp);
    IfxEdsadc_Edsadc_initCommonModeVoltage(edsadc, channel->channelId, &config->commonMode);
    IfxEdsadc_setGainControl(edsadc, channel->channelId, config->gainCalibrationConfig.gainFactor, config->gainCalibrationConfig.cicShift, config->gainCalibrationConfig.cicDecimationRate);
    IfxEdsadc_setGainCalibration(edsadc, channel->channelId, config->gainCalibrationConfig.calFactor, config->gainCalibrationConfig.calTarget);

    const IfxEdsadc_Edsadc_ChannelPins *pins = config->channelPins;

    if (pins != NULL_PTR)
    {
        const IfxEdsadc_Dsn_In *dsn = pins->dsn;

        if (dsn != NULL_PTR)
        {
            IfxEdsadc_initDsnPin(dsn, pins->dsnMode, pins->pinDriver);
        }

        const IfxEdsadc_Dsp_In *dsp = pins->dsp;

        if (dsp != NULL_PTR)
        {
            IfxEdsadc_initDspPin(dsp, pins->dspMode, pins->pinDriver);
        }

        const IfxEdsadc_Cin_In *cin = pins->cin;

        if (cin != NULL_PTR)
        {
            IfxEdsadc_initCinPin(cin, pins->cinMode, pins->pinDriver);
        }

        const IfxEdsadc_Din_In *din = pins->din;

        if (din != NULL_PTR)
        {
            IfxEdsadc_initDinPin(din, pins->dinMode, pins->pinDriver);
        }

        const IfxEdsadc_Itr_In *itr = pins->itr;

        if (itr != NULL_PTR)
        {
            IfxEdsadc_initItrPin(itr, pins->itrMode, pins->pinDriver);
        }
    }
}


void IfxEdsadc_Edsadc_initChannelConfig(IfxEdsadc_Edsadc_ChannelConfig *config, IfxEdsadc_Edsadc *edsadc)
{
    const IfxEdsadc_Edsadc_ChannelConfig IfxEdsadc_Edsadc_defaultChannelConfig = {
        .channelId = IfxEdsadc_ChannelId_0,
        .firFilter = {
            .serviceRequest            = IfxEdsadc_MainServiceRequest_everyNewResult,
            .auxServiceRequest         = IfxEdsadc_AuxServiceRequest_never,
            .serviceRequestLevel       = IfxEdsadc_ServiceRequestFifolevel_1,
            .eventSelect               = IfxEdsadc_EventSelect_everyNewResult,
            .offsetCompensation        = IfxEdsadc_OffsetCompensationFilter_disabled,
            .fir0Enabled               = TRUE,
            .fir1Enabled               = TRUE,
            .rippleCompensationEnabled = FALSE,
            .fir1DecimateRate          = FALSE,
            .cicMode                   = FALSE,
            .prefilterEnabled          = TRUE,
            .calibrationTrigger        = FALSE,
            .calibrationControl        = FALSE,
            .overshootCompensation     = IfxEdsadc_OvershootCompensation_disabled
        },
        .modulator                      = {
            .modulatorClockFreq     = 10.0e6,
            .inputGain              = IfxEdsadc_InputGain_factor1,
            .analogClockSyncDelay   = IfxEdsadc_AnalogClockSyncDelay_noDelay,
            .positiveInput          = IfxEdsadc_InputConfig_inputPin,
            .negativeInput          = IfxEdsadc_InputConfig_inputPin,
            .inputPin               = IfxEdsadc_InputPin_a,
            .ditheringEnabled       = IfxEdsadc_dithering_disabled,
            .integratorResetEnabled = FALSE,
            .inputMuxActionControl  = FALSE,
            .triggerEvent           = IfxEdsadc_InputMuxControlMode_softwareControl
        },
        .demodulator                    = {
            .triggerInput       = IfxEdsadc_TriggerInput_a,
            .inputDataSource    = IfxEdsadc_InputDataSource_directInputA,
            .dataStream         = IfxEdsadc_DataStream_onChipModulator,
            .sampleClockSource  = IfxEdsadc_SampleClockSource_internal,
            .integrationTrigger = IfxEdsadc_IntegratorTrigger_bypassed,
            .timestampTrigger   = IfxEdsadc_TimestampTrigger_noTrigger,
            .dataReadMode       = IfxEdsadc_DataRead_timestampOnHighbit,
            .timeStampMode      = FALSE,
            .resultDisplay      = IfxEdsadc_ResultDisplayMode_signed
        },
        .integrator                     = {
            .windowSize        = IfxEdsadc_IntegrationWindowSize_internalControl,
            .discardCount      = 0,
            .integrationCount  = 20,
            .integrationCycles = 1,
            .shiftControl      = IfxEdsadc_IntegrationShiftControl_0,
            .fcRestartControl  = IfxEdsadc_RestartFilterChain_on
        },
        .rectifier                      = {
            .signDelay  = 0,
            .signPeriod = 0,
            .signSource = IfxEdsadc_RectifierSignSource_onChipGenerator,
            .enabled    = FALSE
        },
        .combFilter                     = {
            .decimationFactor = 36,
            .startValue       = 36
        },
        .timeStamp                      = {
            .timeStampCounterClock      = IfxEdsadc_TimeStampCounterClockSelection_div1,
            .timeStampCounterRunControl = IfxEdsadc_TimeStampCounterControl_off,
            .analogMuxCopy              = IfxEdsadc_AnalogMux_doNotCopyAmx
        },
        .commonMode                     = {
            .fractionalRefVoltage  = IfxEdsadc_FractionalRefVoltageSelection_div2,
            .refVoltageEnable      = IfxEdsadc_FractionalRefVoltage_disable,
            .halfSupplyVoltge      = IfxEdsadc_HalfSupplyVoltage_disable,
            .commonModeHoldVoltage = IfxEdsadc_CommonModeHoldVoltage_disable,
            .positiveInput         = 0,
            .negativeInput         = 0
        },
        .gainFactor            = 8090,
        .cicShift              = 16,
        .channelPins           = NULL_PTR,
        .gainCalibrationConfig = {
            .gainFactor        = 5759,
            .cicShift          = 27,
            .cicDecimationRate = 6,
            .calFactor         = 4096,
            .calTarget         = 32000
        }
    };
    *config = IfxEdsadc_Edsadc_defaultChannelConfig;

    if (edsadc != NULL_PTR)
    {
        config->module = edsadc->edsadc;
    }
    else
    {
        config->module = NULL_PTR;
    }
}


void IfxEdsadc_Edsadc_initCombFilter(IfxEdsadc_Edsadc_Channel *channel, const IfxEdsadc_Edsadc_CombFilterConfig *config)
{
    Ifx_EDSADC_CH_FCFGC fcfgc;

    fcfgc.U                   = 0;

    fcfgc.B.CFMDF             = config->decimationFactor - 1;
    fcfgc.B.CFMSV             = config->startValue - 1;

    (channel->channel)->FCFGC = fcfgc;
}


void IfxEdsadc_Edsadc_initDemodulator(IfxEdsadc_Edsadc_Channel *channel, const IfxEdsadc_Edsadc_DemodulatorConfig *config)
{
    Ifx_EDSADC_CH_DICFG dicfg;

    dicfg.U                   = 0;

    dicfg.B.DSS               = config->dataStream;
    dicfg.B.DSRCEX            = config->inputDataSource;
    dicfg.B.DSWC              = 1; // enable write access for this bitfield

    dicfg.B.ITRMODE           = config->integrationTrigger;
    dicfg.B.TSTRMODE          = config->timestampTrigger;
    dicfg.B.TRSEL             = config->triggerInput;
    dicfg.B.DRM               = config->dataReadMode;
    dicfg.B.TSM               = config->timeStampMode;

    dicfg.B.CSRCEX            = config->sampleClockSource;
    dicfg.B.RDM               = config->resultDisplay;
    dicfg.B.MSWC              = 1; // enable write access for these bitfields

    (channel->channel)->DICFG = dicfg;
}


void IfxEdsadc_Edsadc_initFirFilter(IfxEdsadc_Edsadc_Channel *channel, const IfxEdsadc_Edsadc_FirFilterConfig *config)
{
    Ifx_EDSADC_CH_FCFGM fcfgm;
    Ifx_EDSADC_CH_RFC   rfc;
    fcfgm.U                   = 0;
    rfc.U                     = 0;

    fcfgm.B.FIR0EN            = (config->fir0Enabled != FALSE) ? 1 : 0;
    fcfgm.B.FIR1EN            = (config->fir1Enabled != FALSE) ? 1 : 0;
    fcfgm.B.OCEN              = (config->offsetCompensation != FALSE) ? 1 : 0;
    fcfgm.B.OVCEN             = config->overshootCompensation;
    fcfgm.B.FIR1DEC           = config->fir1DecimateRate;
    fcfgm.B.CICMOD            = config->cicMode;
    fcfgm.B.PFEN              = config->prefilterEnabled;
    fcfgm.B.FMWC              = 1;
    fcfgm.B.SRGM              = config->serviceRequest;
    fcfgm.B.SRGA              = config->auxServiceRequest;
    fcfgm.B.ESEL              = config->eventSelect;
    fcfgm.B.CALIB             = config->calibrationTrigger;
    fcfgm.B.AUTOCAL           = config->calibrationControl;
    fcfgm.B.CSRWC             = 1;

    (channel->channel)->FCFGM = fcfgm;
    rfc.B.SRLVL               = config->serviceRequestLevel;
    channel->channel->RFC     = rfc;
}


void IfxEdsadc_Edsadc_initIntegrator(IfxEdsadc_Edsadc_Channel *channel, const IfxEdsadc_Edsadc_IntegratorConfig *config)
{
    Ifx_EDSADC_CH_IWCTR iwctr;

    iwctr.U                   = 0;

    iwctr.B.REPVAL            = config->integrationCycles - 1;
    iwctr.B.NVALDIS           = config->discardCount;
    iwctr.B.IWS               = config->windowSize;
    iwctr.B.NVALINT           = config->integrationCount - 1;
    iwctr.B.ISC               = config->shiftControl;
    iwctr.B.FRC               = config->fcRestartControl;

    (channel->channel)->IWCTR = iwctr;
}


void IfxEdsadc_Edsadc_initModulator(IfxEdsadc_Edsadc_Channel *channel, const IfxEdsadc_Edsadc_ModulatorConfig *config)
{
    Ifx_EDSADC_CH_MODCFG modcfg;

    modcfg.U         = 0;

    modcfg.B.INCFGP  = config->positiveInput;
    modcfg.B.INCFGN  = config->negativeInput;
    modcfg.B.GAINSEL = config->inputGain;
    modcfg.B.INSEL   = config->inputPin;
    modcfg.B.INMAC   = config->inputMuxActionControl;
    modcfg.B.INMODE  = config->triggerEvent;
    modcfg.B.INCWC   = 1;       // enable write access for these bitfields

    float32 targetFreq = config->modulatorClockFreq;
    float32 sourceFreq = IfxEdsadc_getModulatorInputClockFreq(channel->module);
    modcfg.B.DIVM              = IfxEdsadc_Edsadc_calcDIVM(sourceFreq, &targetFreq);
    modcfg.B.ACSD              = config->analogClockSyncDelay;
    modcfg.B.DITHEN            = config->ditheringEnabled;

    modcfg.B.IREN              = config->integratorResetEnabled;
    modcfg.B.MMWC              = 1;  // enable write access for these bitfields

    (channel->channel)->MODCFG = modcfg;
}


void IfxEdsadc_Edsadc_initModule(IfxEdsadc_Edsadc *edsadc, IfxEdsadc_Edsadc_Config *config)
{
    Ifx_EDSADC *edsadcSFR = config->edsadc;

    edsadc->edsadc = edsadcSFR;

    {
        if (edsadcSFR->CLC.U != 0)
        {
            uint16 passwd = IfxScuWdt_getCpuWatchdogPassword();
            IfxScuWdt_clearCpuEndinit(passwd);
            edsadcSFR->CLC.U = 0x00000000;

            if (edsadcSFR->CLC.U)
            {}                   // sync access

            IfxScuWdt_setCpuEndinit(passwd);
        }

        {
            Ifx_EDSADC_GLOBCFG globcfg;
            globcfg.U            = edsadcSFR->GLOBCFG.U;

            globcfg.B.CPWC       = 1;
            globcfg.B.DITRIM     = config->ditheringIntensity;
            globcfg.B.USC        = config->modulatorClockMode;
            globcfg.B.SUPLEV     = config->supplyVoltageLevel;

            edsadcSFR->GLOBCFG.U = globcfg.U;
        }
    }
}


void IfxEdsadc_Edsadc_initModuleConfig(IfxEdsadc_Edsadc_Config *config, Ifx_EDSADC *edsadc)
{
    const IfxEdsadc_Edsadc_Config IfxEdsadc_Edsadc_defaultConfig = {
        .ditheringIntensity = IfxEdsadc_DitheringIntensity_minimum,
        .modulatorClockMode = IfxEdsadc_ModulatorClockGeneration_syncMode,
        .supplyVoltageLevel = IfxEdsadc_SupplyVoltageLevel_automaticControl
    };

    *config        = IfxEdsadc_Edsadc_defaultConfig;
    config->edsadc = edsadc;
}


void IfxEdsadc_Edsadc_initRectifier(IfxEdsadc_Edsadc_Channel *channel, const IfxEdsadc_Edsadc_RectifierConfig *config)
{
    Ifx_EDSADC_CH_RECTCFG rect;

    rect.U                             = channel->module->CH[channel->channelId].RECTCFG.U;
    rect.B.RFEN                        = config->enabled;
    rect.B.SSRC                        = config->signSource;
    (channel->channel)->RECTCFG        = rect;
    (channel->channel)->CGSYNC.B.SDPOS = config->signDelay;
    (channel->channel)->CGSYNC.B.SDNEG = config->signDelay + (config->signPeriod / 2);
}


IFX_STATIC sint32 IfxEdsadc_Edsadc_calcDIVCG(float32 sourceFreq, float32 *targetFreq)
{
    float32 bestError = 10e6;
    sint32  bestDiv   = 2, i;

    for (i = 2; i <= 32; i += 2)
    {
        float32 freq  = sourceFreq / i;
        float32 error = __absf(freq - *targetFreq);

        if (__leqf(error, bestError))
        {
            bestError = error;
            bestDiv   = i;

            if (!__neqf(error, 0))
            {
                break;
            }
        }
    }

    *targetFreq = sourceFreq / bestDiv;

    return (bestDiv / 2) - 1;
}


void IfxEdsadc_Edsadc_initCarrierGenConfig(IfxEdsadc_Edsadc_CarrierGenConfig *config)
{
    config->bitReversed         = FALSE;
    config->carrierWaveformMode = IfxEdsadc_CarrierWaveformMode_sine;
    config->frequency           = 10000;
    config->inverted            = FALSE;
    config->pinDriver           = IfxPort_PadDriver_cmosAutomotiveSpeed1;
    config->pinMode             = IfxPort_OutputMode_pushPull;
    config->pinNeg              = NULL_PTR;
    config->pinPos              = NULL_PTR;
}
