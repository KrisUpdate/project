/**
 * \file IfxRif_Rif.c
 * \brief RIF RIF details
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

#include "IfxRif_Rif.h"

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

void IfxRif_Rif_initModule(IfxRif_Rif *rif, IfxRif_Rif_Config *config)
{
    Ifx_RIF *rifSFR = config->rif; /* pointer to RIF registers */
    rif->rif           = rifSFR;   /* take over register pointer to module handler */

    rif->numOfChannels = config->numOfChannels;

    /* enable module if it hasn't been enabled already */
    if (IfxRif_isModuleEnabled(rifSFR) == FALSE)
    {
        IfxRif_enableModule(rifSFR);
    }

    /* --- Deserializer configuration if the external ADC are used --- */

    if (config->connectedAdc == IfxRif_Adc_external)
    {
        IfxRif_setClockPolarity(rifSFR, config->deserializer.clockPolarity);
        IfxRif_setFramePolarity(rifSFR, config->deserializer.framePolarity);
        IfxRif_setDataPolarity0(rifSFR, config->deserializer.dataPolarity0);
        IfxRif_setDataPolarity1(rifSFR, config->deserializer.dataPolarity1);
        IfxRif_setDataPolarity2(rifSFR, config->deserializer.dataPolarity2);
        IfxRif_setDataPolarity3(rifSFR, config->deserializer.dataPolarity3);

        IfxRif_setDataLength(rifSFR, config->data.length);
        IfxRif_setDataFormat(rifSFR, config->data.format);
        IfxRif_setShiftDirection(rifSFR, config->data.shiftDirection);

        if (config->deserializer.calibrationEnable != FALSE)
        {
            IfxRif_enableCalibration(rifSFR);
        }
    }

    /* --- FIFO and Lane Management configuration --- */

    IfxRif_enableFifos(rifSFR, config->numOfChannels);

    IfxRif_setFlmMode(rifSFR, config->data.flmMode);
    IfxRif_setFullSwapMode(rifSFR, config->data.fullSwapMode);

    /* CRC enable / disable for both internal and external ADC */
    if (config->data.crcEnable != FALSE)
    {
        IfxRif_enableCrc(rifSFR);
    }

    /* --- Radar State Machine Configuration --- */

    if (config->connectedAdc == IfxRif_Adc_external)
    {     /* if External ADCs are selected */
        IfxRif_enableExternalAdc(rifSFR);
        IfxRif_setValidDataSamplesNumber(rifSFR, config->rsm.numOfValidSamples);

        if (config->fwdg.threshold > 0)
        {
            IfxRif_setFrameWatchdogThreshold(rifSFR, config->fwdg.threshold);
        }
    }
    else
    {    /* if Internal ADCs are selected */
        IfxRif_enableInternalAdc(rifSFR);

        /*       IfxRif_setRamp1PhaseLength(rifSFR, config->rsm.ramp1Phase);
         *     IfxRif_setRamp3PhaseLength(rifSFR, config->rsm.ramp3Phase);
         *     IfxRif_setWaitPhaseLength(rifSFR, config->rsm.waitPhase);
         *     IfxRif_setPausePhasePrescaler(rifSFR, config->rsm.pausePrescaler);
         *     IfxRif_setPausePhaseLength(rifSFR, config->rsm.pausePhase);
         */
        if (config->rsm.ramp1SignalEnable == TRUE)
        {
            IfxRif_setRamp1SignalInput(rifSFR, config->rsm.ramp1SignalInput);
            IfxRif_setRamp1SignalPolarity(rifSFR, config->rsm.ramp1SignalPolarity);
            IfxRif_enableRamp1Signal(rifSFR, TRUE);
        }

        IfxRif_setChirpLength(rifSFR, config->rsm.rampsPerChirp);
    }

    /* --- DMI configuration --- */

    IfxRif_setDataAlignment(rifSFR, config->data.alignment);

    /* --- LVDS configuration --- */

    IfxRif_setFrameLvdsPadControl(rifSFR, config->lvds.frameControl);
    IfxRif_setClockLvdsPadControl(rifSFR, config->lvds.clockControl);
    IfxRif_setData0LvdsPadControl(rifSFR, config->lvds.data0Control);
    IfxRif_setData1LvdsPadControl(rifSFR, config->lvds.data1Control);
    IfxRif_setData2LvdsPadControl(rifSFR, config->lvds.data2Control);
    IfxRif_setData3LvdsPadControl(rifSFR, config->lvds.data3Control);
    IfxRif_setCommonLvdsPadControl(rifSFR, config->lvds.commonControl);
    IfxRif_setRtermTrimmingValue(rifSFR, config->lvds.rtermTrimmingValue);
    IfxRif_setLvdsBiasDistributorPowerDownMode(rifSFR, config->lvds.biasDistributorPowerDownMode);
    IfxRif_setLvdsBiasDistributor5VMode(rifSFR, config->lvds.biasDistributor5VMode);

    /* --- Interrupt configuration --- */

    if (config->interrupt.intPriority > 0)
    {
        volatile Ifx_SRC_SRCR *src;
        src = IfxRif_getSrcPointerInt(rifSFR);
        IfxSrc_init(src, config->interrupt.typeOfService, config->interrupt.intPriority);

        if (config->interrupt.calibrationEndEnable)
        {
            IfxRif_enableInterrupt(rifSFR, IfxRif_Interrupt_calibrationEnd);
        }

        if (config->interrupt.frameWatchdogOverflowEnable)
        {
            IfxRif_enableInterrupt(rifSFR, IfxRif_Interrupt_frameWatchdogOverflow);
        }

        if (config->interrupt.rampEndEnable)
        {
            IfxRif_enableInterrupt(rifSFR, IfxRif_Interrupt_rampEnd);
        }

        if (config->interrupt.chirpEndEnable)
        {
            IfxRif_enableInterrupt(rifSFR, IfxRif_Interrupt_chirpEnd);
        }

        if (config->interrupt.ramp1StartEnable)
        {
            IfxRif_enableInterrupt(rifSFR, IfxRif_Interrupt_ramp1Start);
        }

        IfxSrc_enable(src);
    }

    if (config->interrupt.errPriority > 0)
    {
        volatile Ifx_SRC_SRCR *src;
        src = IfxRif_getSrcPointerErr(rifSFR);
        IfxSrc_init(src, config->interrupt.typeOfService, config->interrupt.errPriority);

        if (config->interrupt.crcErrorOnLine0Enable)
        {
            IfxRif_enableInterrupt(rifSFR, IfxRif_Interrupt_crcErrorOnLine0);
        }

        if (config->interrupt.crcErrorOnLine1Enable)
        {
            IfxRif_enableInterrupt(rifSFR, IfxRif_Interrupt_crcErrorOnLine1);
        }

        if (config->interrupt.crcErrorOnLine2Enable)
        {
            IfxRif_enableInterrupt(rifSFR, IfxRif_Interrupt_crcErrorOnLine2);
        }

        if (config->interrupt.crcErrorOnLine3Enable)
        {
            IfxRif_enableInterrupt(rifSFR, IfxRif_Interrupt_crcErrorOnLine3);
        }

        if (config->interrupt.ramp1ErrorEnable)
        {
            IfxRif_enableInterrupt(rifSFR, IfxRif_Interrupt_ramp1Error);
        }

        IfxSrc_enable(src);
    }

    /* Lockstep enable /disable */
    IfxRif_enableLockstep(rifSFR, config->rsm.lockstepEnable);
}


void IfxRif_Rif_initModuleConfig(IfxRif_Rif_Config *config, Ifx_RIF *rif)
{
    const IfxRif_Rif_Config defaultConfig = {
        .connectedAdc  = IfxRif_Adc_internal,
        .numOfChannels = 4,
        .deserializer  = {
            .clockPolarity     = IfxRif_ClockPolarity_default,
            .framePolarity     = IfxRif_FramePolarity_default,
            .dataPolarity0     = IfxRif_DataPolarity_default,
            .dataPolarity1     = IfxRif_DataPolarity_default,
            .dataPolarity2     = IfxRif_DataPolarity_default,
            .dataPolarity3     = IfxRif_DataPolarity_default,
            .calibrationEnable = FALSE
        },
        .data                             = {
            .length         = IfxRif_DataLength_16bit,
            .format         = IfxRif_DataFormat_unsigned,
            .shiftDirection = IfxRif_ShiftDirection_lsbFirst,
            .alignment      = IfxRif_DataAlignment_left,
            .flmMode        = IfxRif_FlmMode_direct,
            .fullSwapMode   = IfxRif_FullSwapMode_direct,
            .crcEnable      = FALSE
        },
        .fwdg                             = {
            .threshold                    = 0
        },
        .rsm                              = {
            .ramp1SignalEnable   = FALSE,
            .ramp1SignalInput    = IfxRif_Ramp1SignalInput_0,
            .ramp1SignalPolarity = IfxRif_Ramp1SignalPolarity_lowActive,
            .rampsPerChirp       = 128,
            .numOfValidSamples   = 128,
            .lockstepEnable      = FALSE
        },
        .lvds                             = {
            .frameControl                 = IfxRif_LvdsPadControl_frameClock | IfxRif_LvdsPadControl_rterm,
            .clockControl                 = IfxRif_LvdsPadControl_frameClock | IfxRif_LvdsPadControl_rterm,
            .data0Control                 = IfxRif_LvdsPadControl_frameClock | IfxRif_LvdsPadControl_rterm,
            .data1Control                 = IfxRif_LvdsPadControl_frameClock | IfxRif_LvdsPadControl_rterm,
            .data2Control                 = IfxRif_LvdsPadControl_frameClock | IfxRif_LvdsPadControl_rterm,
            .data3Control                 = IfxRif_LvdsPadControl_frameClock | IfxRif_LvdsPadControl_rterm,
            .commonControl                = IfxRif_CommonLvdsPadControl_5vMode,
            .rtermTrimmingValue           = 0x3,
            .biasDistributorPowerDownMode = IfxRif_LvdsBiasDistributorMode_active,
            .biasDistributor5VMode        = TRUE
        },
        .interrupt                        = {
            .calibrationEndEnable        = FALSE,
            .frameWatchdogOverflowEnable = FALSE,
            .rampEndEnable               = FALSE,
            .chirpEndEnable              = FALSE,
            .crcErrorOnLine0Enable       = FALSE,
            .crcErrorOnLine1Enable       = FALSE,
            .crcErrorOnLine2Enable       = FALSE,
            .crcErrorOnLine3Enable       = FALSE,
            .ramp1ErrorEnable            = FALSE,
            .chirp1ErrorEnable           = FALSE,
            .ramp1StartEnable            = FALSE,
            .intPriority                 = 0,
            .errPriority                 = 0,
            .typeOfService               = IfxSrc_Tos_cpu0
        }
    };

    /* Default Configuration */
    *config = defaultConfig;

    /* take over module pointer */
    config->rif = rif;
}


void IfxRif_Rif_isrError(IfxRif_Rif *rif)
{
    Ifx_RIF *rifSFR = rif->rif;

    if (IfxRif_getInterruptFlagStatus(rifSFR, IfxRif_Interrupt_crcErrorOnLine0))
    {
        IfxRif_clearInterruptFlag(rifSFR, IfxRif_Interrupt_crcErrorOnLine0);
        rif->status.crcErrorOnLine0 = 1;
        rif->status.crcNoError      = 0;
    }

    if (IfxRif_getInterruptFlagStatus(rifSFR, IfxRif_Interrupt_crcErrorOnLine1))
    {
        IfxRif_clearInterruptFlag(rifSFR, IfxRif_Interrupt_crcErrorOnLine1);
        rif->status.crcErrorOnLine1 = 1;
        rif->status.crcNoError      = 0;
    }

    if (IfxRif_getInterruptFlagStatus(rifSFR, IfxRif_Interrupt_crcErrorOnLine2))
    {
        IfxRif_clearInterruptFlag(rifSFR, IfxRif_Interrupt_crcErrorOnLine2);
        rif->status.crcErrorOnLine2 = 1;
        rif->status.crcNoError      = 0;
    }

    if (IfxRif_getInterruptFlagStatus(rifSFR, IfxRif_Interrupt_crcErrorOnLine3))
    {
        IfxRif_clearInterruptFlag(rifSFR, IfxRif_Interrupt_crcErrorOnLine3);
        rif->status.crcErrorOnLine3 = 1;
        rif->status.crcNoError      = 0;
    }

    if (IfxRif_getInterruptFlagStatus(rifSFR, IfxRif_Interrupt_ramp1Error))
    {
        IfxRif_clearInterruptFlag(rifSFR, IfxRif_Interrupt_ramp1Error);
        rif->status.ramp1Error = 1;
    }

    if (IfxRif_getInterruptFlagStatus(rifSFR, IfxRif_Interrupt_chirp1Error))
    {
        IfxRif_clearInterruptFlag(rifSFR, IfxRif_Interrupt_chirp1Error);
        rif->status.chirp1Error = 1;
    }
}


void IfxRif_Rif_isrInterrupt(IfxRif_Rif *rif)
{
    Ifx_RIF *rifSFR = rif->rif;

    if (IfxRif_getInterruptFlagStatus(rifSFR, IfxRif_Interrupt_calibrationEnd))
    {
        IfxRif_clearInterruptFlag(rifSFR, IfxRif_Interrupt_calibrationEnd);
        rif->status.calibrationEnd = 1;
    }

    if (IfxRif_getInterruptFlagStatus(rifSFR, IfxRif_Interrupt_frameWatchdogOverflow))
    {
        IfxRif_clearInterruptFlag(rifSFR, IfxRif_Interrupt_frameWatchdogOverflow);
        rif->status.numOfRamps++;
    }

    if (IfxRif_getInterruptFlagStatus(rifSFR, IfxRif_Interrupt_rampEnd))
    {
        IfxRif_clearInterruptFlag(rifSFR, IfxRif_Interrupt_rampEnd);
        rif->status.numOfRamps++;
    }

    if (IfxRif_getInterruptFlagStatus(rifSFR, IfxRif_Interrupt_chirpEnd))
    {
        IfxRif_clearInterruptFlag(rifSFR, IfxRif_Interrupt_chirpEnd);
        rif->status.chirpEnd = 1;
        rif->status.numOfChirps++;
    }

    if (IfxRif_getInterruptFlagStatus(rifSFR, IfxRif_Interrupt_ramp1Start))
    {
        IfxRif_clearInterruptFlag(rifSFR, IfxRif_Interrupt_ramp1Start);
    }
}


void IfxRif_Rif_startDeserializers(IfxRif_Rif *rif)
{
    Ifx_RIF *rifSFR = rif->rif;

    switch (rif->numOfChannels)
    {
    case 1:
        IfxRif_enableDeserializer(rifSFR, IfxRif_DeserializerId_0);
        break;
    case 2:
        IfxRif_enableDeserializer(rifSFR, IfxRif_DeserializerId_0);
        IfxRif_enableDeserializer(rifSFR, IfxRif_DeserializerId_1);
        break;
    case 3:
    case 4:
        IfxRif_enableDeserializer(rifSFR, IfxRif_DeserializerId_0);
        IfxRif_enableDeserializer(rifSFR, IfxRif_DeserializerId_1);
        IfxRif_enableDeserializer(rifSFR, IfxRif_DeserializerId_2);
        IfxRif_enableDeserializer(rifSFR, IfxRif_DeserializerId_3);
        break;
    default:
        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, FALSE); /* wrong selection  */
        break;
    }
}


void IfxRif_Rif_stopDeserializers(IfxRif_Rif *rif)
{
    Ifx_RIF *rifSFR = rif->rif;

    switch (rif->numOfChannels)
    {
    case 1:
        IfxRif_disableDeserializer(rifSFR, IfxRif_DeserializerId_0);
        break;
    case 2:
        IfxRif_disableDeserializer(rifSFR, IfxRif_DeserializerId_0);
        IfxRif_disableDeserializer(rifSFR, IfxRif_DeserializerId_1);
        break;
    case 3:
    case 4:
        IfxRif_disableDeserializer(rifSFR, IfxRif_DeserializerId_0);
        IfxRif_disableDeserializer(rifSFR, IfxRif_DeserializerId_1);
        IfxRif_disableDeserializer(rifSFR, IfxRif_DeserializerId_2);
        IfxRif_disableDeserializer(rifSFR, IfxRif_DeserializerId_3);
        break;
    default:
        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, FALSE); /* wrong selection  */
        break;
    }
}
