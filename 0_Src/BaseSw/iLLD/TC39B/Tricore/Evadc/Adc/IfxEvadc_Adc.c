/**
 * \file IfxEvadc_Adc.c
 * \brief EVADC ADC details
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

#include "IfxEvadc_Adc.h"

/** \addtogroup IfxLld_Evadc_Adc_Group
 * \{ */
/******************************************************************************/
/*------------------------Inline Function Prototypes--------------------------*/
/******************************************************************************/

/** \brief Gets the master id
 * \param slave Index of the group
 * \param masterIndex master kernel index
 * \return Master Group Id
 */
IFX_INLINE IfxEvadc_GroupId IfxEvadc_Adc_getMasterId(IfxEvadc_GroupId slave, IfxEvadc_Adc_SYNCTR_STSEL masterIndex);

/** \brief Gets the current master kernel index.
 * \param slave Index of the group
 * \param master Index of the group
 * \return current master kernel index
 */
IFX_INLINE IfxEvadc_Adc_SYNCTR_STSEL IfxEvadc_Adc_getMasterKernelIndex(IfxEvadc_GroupId slave, IfxEvadc_GroupId master);

/** \} */

/** \addtogroup IfxLld_Evadc_Adc_Variables
 * \{ */

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

IFX_STATIC IFX_CONST IfxEvadc_Adc_SYNCTR_STSEL IfxEvadc_Adc_masterIndex[IFXEVADC_NUM_ADC_CAL_GROUPS][IFXEVADC_NUM_ADC_CAL_GROUPS] = {
    /* 0  1  2  3  4  5  6  7  8  9  10 11   */
    {0, 1, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0},   /* Grp 0 */
    {1, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0},   /* Grp 1 */
    {1, 2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0},   /* Grp 2 */
    {1, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* Grp 3 */
    {0, 0, 0, 0, 0, 1, 2, 3, 0, 0, 0, 0},   /* Grp 4 */
    {0, 0, 0, 0, 1, 0, 2, 3, 0, 0, 0, 0},   /* Grp 5 */
    {0, 0, 0, 0, 1, 2, 0, 3, 0, 0, 0, 0},   /* Grp 6 */
    {0, 0, 0, 0, 1, 2, 3, 0, 0, 0, 0, 0},   /* Grp 7 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3},   /* Grp 8 */
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2, 3},   /* Grp 9 */
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 3},   /* Grp 10*/
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 0}    /* Grp 11*/
};

/** \} */

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE IfxEvadc_GroupId IfxEvadc_Adc_getMasterId(IfxEvadc_GroupId slave, IfxEvadc_Adc_SYNCTR_STSEL masterIndex)
{
    uint8            i, idxOffset;
    IfxEvadc_GroupId masterId = slave;

    if (masterIndex == 0)
    {
        masterId = slave;
    }
    else
    {
        idxOffset = (slave < 4) ? 0 : 4;    /* 4 is the index in the above IfxEvadc_Adc_masterIndex about which symmetry is observed */

        for (i = 0; i < 4; i++)
        {
            if (IfxEvadc_Adc_masterIndex[slave][i + idxOffset] == masterIndex)
            {
                return (IfxEvadc_GroupId)(i + idxOffset);
            }
        }
    }

    return masterId;
}


IFX_INLINE IfxEvadc_Adc_SYNCTR_STSEL IfxEvadc_Adc_getMasterKernelIndex(IfxEvadc_GroupId slave, IfxEvadc_GroupId master)
{
    return IfxEvadc_Adc_masterIndex[slave][master];
}


/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

void IfxEvadc_Adc_deInitGroup(IfxEvadc_Adc_Group *group)
{
    Ifx_EVADC       *evadc      = IfxEvadc_Adc_getEvadcFromGroup(group);
    Ifx_EVADC_G     *evadcG     = IfxEvadc_Adc_getGroupRegsFromGroup(group);
    /* Get group index */
    IfxEvadc_GroupId groupIndex = group->groupId;

    /* Request Access to configuration registers */
    IfxEvadc_enableAccess(evadc, (IfxEvadc_Protection)(IfxEvadc_Protection_initGroup0 + groupIndex));
    IfxEvadc_resetGroup(evadcG);
    IfxEvadc_disableAccess(evadc, (IfxEvadc_Protection)(IfxEvadc_Protection_initGroup0 + groupIndex));
}


void IfxEvadc_Adc_getChannelConfig(IfxEvadc_Adc_Channel *channel, IfxEvadc_Adc_ChannelConfig *config)
{
    Ifx_EVADC_G       *evadcG       = IfxEvadc_Adc_getGroupRegsFromGroup(channel->group);
    IfxEvadc_ChannelId channelIndex = channel->channel;

    config->channelId = channel->channel;
    config->group     = channel->group;

    Ifx_EVADC_G_CHCTR tempChctr;
    tempChctr.U                 = 0;
    tempChctr                   = IfxEvadc_getChannelControlConfig(evadcG, channelIndex);

    config->inputClass          = (IfxEvadc_InputClasses)tempChctr.B.ICLSEL;
    config->reference           = (IfxEvadc_ChannelReference)tempChctr.B.REFSEL;
    config->resultRegister      = (IfxEvadc_ChannelResult)tempChctr.B.RESREG;
    config->globalResultUsage   = tempChctr.B.RESTGT;
    config->lowerBoundary       = (IfxEvadc_BoundarySelection)tempChctr.B.BNDSELL;
    config->upperBoundary       = (IfxEvadc_BoundarySelection)tempChctr.B.BNDSELU;
    config->boundaryMode        = (IfxEvadc_BoundaryExtension)tempChctr.B.BNDSELX;
    config->limitCheck          = (IfxEvadc_LimitCheck)tempChctr.B.CHEVMODE;
    config->synchonize          = tempChctr.B.SYNC;
    config->rightAlignedStorage = tempChctr.B.RESPOS;

    uint32           channelServiceRequestNodePtr;
    /* Get Channel index */
    IfxEvadc_GroupId groupIndex = channel->group->groupId;

    if (config->channelId < IfxEvadc_ChannelId_8)
    {
        channelServiceRequestNodePtr = ((IfxEvadc_getChannelServiceRequestNodePointer0(evadcG)).U >> (channel->channel * 4)) & 0xF;
    }
    else
    {
        channelServiceRequestNodePtr = ((IfxEvadc_getChannelServiceRequestNodePointer1(evadcG)).U >> ((channel->channel - IfxEvadc_ChannelId_8) * 4)) & 0xF;
    }

    volatile Ifx_SRC_SRCR *src = IfxEvadc_getSrcAddress(groupIndex, (IfxEvadc_SrcNr)channelServiceRequestNodePtr);

    if (src->B.SRE == 1)
    {
        config->channelSrcNr        = (IfxEvadc_SrcNr)channelServiceRequestNodePtr;
        config->channelPriority     = (Ifx_Priority)src->B.SRPN;
        config->channelServProvider = (IfxSrc_Tos)src->B.TOS;
    }
    else
    {
        config->channelSrcNr        = (IfxEvadc_SrcNr)0;
        config->channelPriority     = (Ifx_Priority)0;
        config->channelServProvider = (IfxSrc_Tos)0;
    }

    uint32 resultServiceRequestNodePtr;

    if (config->resultRegister < IfxEvadc_ChannelResult_8)
    {
        resultServiceRequestNodePtr = ((IfxEvadc_getChannelResultServiceRequestNodePointer0(evadcG)).U >> (channel->channel * 4)) & 0xF;
    }
    else
    {
        resultServiceRequestNodePtr = ((IfxEvadc_getChannelResultServiceRequestNodePointer1(evadcG)).U >> ((channel->channel - IfxEvadc_ChannelResult_8) * 4)) & 0xF;
    }

    src = IfxEvadc_getSrcAddress(groupIndex, (IfxEvadc_SrcNr)resultServiceRequestNodePtr);

    if (src->B.SRE == 1)
    {
        config->resultSrcNr        = (IfxEvadc_SrcNr)resultServiceRequestNodePtr;
        config->resultPriority     = (Ifx_Priority)src->B.SRPN;
        config->resultServProvider = (IfxSrc_Tos)src->B.TOS;
    }
    else
    {
        config->resultSrcNr        = (IfxEvadc_SrcNr)0;
        config->resultPriority     = (Ifx_Priority)0;
        config->resultServProvider = (IfxSrc_Tos)0;
    }
}


void IfxEvadc_Adc_getGroupConfig(IfxEvadc_Adc_Group *group, IfxEvadc_Adc_GroupConfig *config)
{
    uint8        inputClassNum;
    Ifx_EVADC_G *evadcG          = group->group;
    float32      analogFrequency = IfxEvadc_getAdcAnalogFrequency(evadcG);

    config->groupId = group->groupId;
    config->module  = &group->module;

    for (inputClassNum = 0; inputClassNum < IFXEVADC_NUM_INPUTCLASSES; inputClassNum++)
    {
        config->inputClass[inputClassNum].conversionMode = IfxEvadc_getGroupConversionMode(evadcG, inputClassNum);
        config->inputClass[inputClassNum].sampleTime     = IfxEvadc_getGroupSampleTime(evadcG, inputClassNum, analogFrequency);
    }

    if (IfxEvadc_isRequestQueueSlotEnabled(evadcG, IfxEvadc_RequestSource_queue0) == TRUE)
    {
        config->arbiter.requestSlotQueue0Enabled            = TRUE;
        config->queueRequest[0].requestSlotPrio             = IfxEvadc_getQueueSlotPriority(evadcG, IfxEvadc_RequestSource_queue0);
        config->queueRequest[0].requestSlotStartMode        = IfxEvadc_getQueueSlotStartMode(evadcG, IfxEvadc_RequestSource_queue0);

        config->queueRequest[0].triggerConfig.triggerSource = IfxEvadc_getQueueSlotTriggerInput(evadcG, IfxEvadc_RequestSource_queue0);
        config->queueRequest[0].triggerConfig.triggerMode   = IfxEvadc_getQueueSlotTriggerMode(evadcG, IfxEvadc_RequestSource_queue0);

        if (config->queueRequest[0].triggerConfig.triggerSource != IfxEvadc_TriggerSource_15)
        {
            config->queueRequest[0].triggerConfig.gatingMode = IfxEvadc_getQueueSlotGatingMode(evadcG, IfxEvadc_RequestSource_queue0);
        }
        else if (config->queueRequest[0].triggerConfig.triggerMode != IfxEvadc_TriggerMode_noExternalTrigger)
        {
            config->queueRequest[0].triggerConfig.gatingMode = IfxEvadc_GatingMode_always;
        }

        config->queueRequest[0].triggerConfig.gatingSource = IfxEvadc_getQueueSlotGatingSource(evadcG, IfxEvadc_RequestSource_queue0);
        config->queueRequest[0].flushQueueAfterInit        = FALSE;
    }
    else
    {
        config->queueRequest[0].flushQueueAfterInit         = FALSE;
        config->queueRequest[0].triggerConfig.gatingMode    = IfxEvadc_GatingMode_disabled;
        config->queueRequest[0].triggerConfig.gatingSource  = IfxEvadc_GatingSource_0;        /* Use CCU6061 TRIG0 */
        config->queueRequest[0].triggerConfig.triggerMode   = IfxEvadc_TriggerMode_noExternalTrigger;
        config->queueRequest[0].triggerConfig.triggerSource = IfxEvadc_TriggerSource_0;       /* Trigger source taken from Gating Input */
    }

    if (IfxEvadc_isRequestQueueSlotEnabled(evadcG, IfxEvadc_RequestSource_queue1) == TRUE)
    {
        config->arbiter.requestSlotQueue1Enabled            = TRUE;
        config->queueRequest[1].requestSlotPrio             = IfxEvadc_getQueueSlotPriority(evadcG, IfxEvadc_RequestSource_queue1);
        config->queueRequest[1].requestSlotStartMode        = IfxEvadc_getQueueSlotStartMode(evadcG, IfxEvadc_RequestSource_queue1);

        config->queueRequest[1].triggerConfig.triggerSource = IfxEvadc_getQueueSlotTriggerInput(evadcG, IfxEvadc_RequestSource_queue1);
        config->queueRequest[1].triggerConfig.triggerMode   = IfxEvadc_getQueueSlotTriggerMode(evadcG, IfxEvadc_RequestSource_queue1);

        if (config->queueRequest[1].triggerConfig.triggerSource != IfxEvadc_TriggerSource_15)
        {
            config->queueRequest[1].triggerConfig.gatingMode = IfxEvadc_getQueueSlotGatingMode(evadcG, IfxEvadc_RequestSource_queue1);
        }
        else if (config->queueRequest[1].triggerConfig.triggerMode != IfxEvadc_TriggerMode_noExternalTrigger)
        {
            config->queueRequest[1].triggerConfig.gatingMode = IfxEvadc_GatingMode_always;
        }

        config->queueRequest[1].triggerConfig.gatingSource = IfxEvadc_getQueueSlotGatingSource(evadcG, IfxEvadc_RequestSource_queue1);
        config->queueRequest[1].flushQueueAfterInit        = FALSE;
    }
    else
    {
        config->queueRequest[1].flushQueueAfterInit         = FALSE;
        config->queueRequest[1].triggerConfig.gatingMode    = IfxEvadc_GatingMode_disabled;
        config->queueRequest[1].triggerConfig.gatingSource  = IfxEvadc_GatingSource_0;        /* Use CCU6061 TRIG0 */
        config->queueRequest[1].triggerConfig.triggerMode   = IfxEvadc_TriggerMode_noExternalTrigger;
        config->queueRequest[1].triggerConfig.triggerSource = IfxEvadc_TriggerSource_0;       /* Trigger source taken from Gating Input */
    }

    if (IfxEvadc_isRequestQueueSlotEnabled(evadcG, IfxEvadc_RequestSource_queue2) == TRUE)
    {
        config->arbiter.requestSlotQueue2Enabled            = TRUE;
        config->queueRequest[2].requestSlotPrio             = IfxEvadc_getQueueSlotPriority(evadcG, IfxEvadc_RequestSource_queue2);
        config->queueRequest[2].requestSlotStartMode        = IfxEvadc_getQueueSlotStartMode(evadcG, IfxEvadc_RequestSource_queue2);

        config->queueRequest[2].triggerConfig.triggerSource = IfxEvadc_getQueueSlotTriggerInput(evadcG, IfxEvadc_RequestSource_queue2);
        config->queueRequest[2].triggerConfig.triggerMode   = IfxEvadc_getQueueSlotTriggerMode(evadcG, IfxEvadc_RequestSource_queue2);

        if (config->queueRequest[2].triggerConfig.triggerSource != IfxEvadc_TriggerSource_15)
        {
            config->queueRequest[2].triggerConfig.gatingMode = IfxEvadc_getQueueSlotGatingMode(evadcG, IfxEvadc_RequestSource_queue2);
        }
        else if (config->queueRequest[2].triggerConfig.triggerMode != IfxEvadc_TriggerMode_noExternalTrigger)
        {
            config->queueRequest[2].triggerConfig.gatingMode = IfxEvadc_GatingMode_always;
        }

        config->queueRequest[2].triggerConfig.gatingSource = IfxEvadc_getQueueSlotGatingSource(evadcG, IfxEvadc_RequestSource_queue2);
        config->queueRequest[2].flushQueueAfterInit        = FALSE;
    }
    else
    {
        config->queueRequest[2].flushQueueAfterInit         = FALSE;
        config->queueRequest[2].triggerConfig.gatingMode    = IfxEvadc_GatingMode_disabled;
        config->queueRequest[2].triggerConfig.gatingSource  = IfxEvadc_GatingSource_0;        /* Use CCU6061 TRIG0 */
        config->queueRequest[2].triggerConfig.triggerMode   = IfxEvadc_TriggerMode_noExternalTrigger;
        config->queueRequest[2].triggerConfig.triggerSource = IfxEvadc_TriggerSource_0;       /* Trigger source taken from Gating Input */
    }

    config->master                 = IfxEvadc_Adc_getMasterId(group->groupId, IfxEvadc_getMasterIndex(evadcG));
    config->disablePostCalibration = ((IfxEvadc_getAnalogFunctionConfiguration(evadcG)).U >> (IFX_EVADC_G_ANCFG_DPCAL_OFF)) & 0x1;
}


IfxEvadc_Status IfxEvadc_Adc_initChannel(IfxEvadc_Adc_Channel *channel, const IfxEvadc_Adc_ChannelConfig *config)
{
    IfxEvadc_Status    Status = IfxEvadc_Status_noError;
    Ifx_EVADC         *evadc  = IfxEvadc_Adc_getEvadcFromGroup(config->group);
    Ifx_EVADC_G       *evadcG = IfxEvadc_Adc_getGroupRegsFromGroup(config->group);

    channel->group = config->group;
    IfxEvadc_GroupId   groupIndex   = channel->group->groupId;
    IfxEvadc_ChannelId channelIndex = config->channelId;

    /* Request Access to configuration registers */
    IfxEvadc_enableAccess(evadc, (IfxEvadc_Protection)(IfxEvadc_Protection_channelControl0 + groupIndex));

    /* Configure Channel */
    {
        IfxEvadc_setReferenceInput(evadcG, channelIndex, config->reference);
        IfxEvadc_storeGroupResult(evadcG, channelIndex, config->resultRegister);
        IfxEvadc_setLowerBoundary(evadcG, channelIndex, config->lowerBoundary);
        IfxEvadc_setUpperBoundary(evadcG, channelIndex, config->upperBoundary);
        IfxEvadc_setSyncRequest(evadcG, channelIndex, config->synchonize);
        IfxEvadc_setChannelInputClass(evadcG, channelIndex, config->inputClass);
        IfxEvadc_setChannelLimitCheckMode(evadcG, channelIndex, config->limitCheck);
        IfxEvadc_setResultPosition(evadcG, channelIndex, config->rightAlignedStorage);
        IfxEvadc_setBoundaryMode(evadcG, channelIndex, config->boundaryMode);
    }

    if (config->channelPriority > 0)
    {
        volatile Ifx_SRC_SRCR *src = IfxEvadc_getSrcAddress(groupIndex, config->channelSrcNr);

        if (config->channelId < IfxEvadc_ChannelId_8)
        {
            IfxEvadc_setChannelEventNodePointer0(evadcG, config->channelSrcNr, channel->channel);
        }
        else
        {
            IfxEvadc_setChannelEventNodePointer1(evadcG, config->channelSrcNr, channel->channel);
        }

        IfxEvadc_clearChannelRequest(evadcG, config->channelId);
        IfxSrc_init(src, config->channelServProvider, config->channelPriority);
        IfxSrc_enable(src);
    }
    else
    {
        /* do nothing */
        /* FIXME disable the interrupt generation? */
    }

    if (config->resultPriority > 0)
    {
        volatile Ifx_SRC_SRCR *src = IfxEvadc_getSrcAddress(groupIndex, config->resultSrcNr);
        IfxEvadc_enableServiceRequest(evadcG, config->resultRegister);

        if (config->resultRegister < IfxEvadc_ChannelResult_8)
        {
            IfxEvadc_setResultNodeEventPointer0(evadcG, config->resultSrcNr, config->resultRegister);
        }
        else
        {
            IfxEvadc_setResultNodeEventPointer1(evadcG, config->resultSrcNr, config->resultRegister);
        }

        IfxEvadc_clearAllResultRequests(evadcG);
        IfxSrc_init(src, config->resultServProvider, config->resultPriority);
        IfxSrc_enable(src);
    }
    else
    {
        /* do nothing */
        /* FIXME disable the interrupt generation? */
    }

    IfxEvadc_disableAccess(evadc, (IfxEvadc_Protection)(IfxEvadc_Protection_channelControl0 + groupIndex));
    /* Software initialization */
    channel->resultreg = config->resultRegister;
    channel->channel   = config->channelId;

    return Status;
}


void IfxEvadc_Adc_initChannelConfig(IfxEvadc_Adc_ChannelConfig *config, const IfxEvadc_Adc_Group *group)
{
    static const IfxEvadc_Adc_ChannelConfig IfxEvadc_Adc_defaultChannelConfig = {
        .channelId           = IfxEvadc_ChannelId_0,
        .group               = NULL_PTR,
        .inputClass          = IfxEvadc_InputClasses_group0,
        .reference           = IfxEvadc_ChannelReference_standard,
        .resultRegister      = IfxEvadc_ChannelResult_0,
        .globalResultUsage   = FALSE,
        .lowerBoundary       = IfxEvadc_BoundarySelection_group0,
        .upperBoundary       = IfxEvadc_BoundarySelection_group0,
        .boundaryMode        = IfxEvadc_BoundaryExtension_standard,
        .limitCheck          = IfxEvadc_LimitCheck_noCheck,
        .synchonize          = FALSE,
        .rightAlignedStorage = FALSE,
        .resultPriority      = 0,
        .resultSrcNr         = IfxEvadc_SrcNr_group0,
        .resultServProvider  = IfxSrc_Tos_cpu0,
        .channelPriority     = 0,
        .channelSrcNr        = IfxEvadc_SrcNr_group0,
        .channelServProvider = IfxSrc_Tos_cpu0
    };
    *config       = IfxEvadc_Adc_defaultChannelConfig;
    config->group = group;
}


void IfxEvadc_Adc_initExternalMultiplexerMode(Ifx_EVADC *evadc, const IfxEvadc_Adc_EmuxControl *emuxControl)
{
    uint8        count  = 0;
    Ifx_EVADC_G *evadcG = &evadc->G[emuxControl->groupId];
    IfxEvadc_setEmuxInterfaceForGroup(evadc, emuxControl->emuxInterface, emuxControl->groupId);

    for (count = 0; count < 3; count++)
    {
        if (emuxControl->emuxOutPinConfig.pins[count] != NULL_PTR)
        {
            IfxEvadc_initEmuxPin(emuxControl->emuxOutPinConfig.pins[count], emuxControl->emuxOutPinConfig.outputMode, emuxControl->emuxOutPinConfig.padDriver);
        }
    }

    IfxEvadc_configExternalMultiplexerMode(evadc, evadcG, emuxControl->mode, emuxControl->channels, emuxControl->startChannel, emuxControl->code, emuxControl->sampleTimeControl, emuxControl->channelSelectionStyle);
}


void IfxEvadc_Adc_initExternalMultiplexerModeConfig(IfxEvadc_Adc_EmuxControl *emuxConfig, Ifx_EVADC *evadc)
{
    emuxConfig->evadc                 = evadc;
    emuxConfig->channels              = 0;
    emuxConfig->groupId               = IfxEvadc_GroupId_0;
    emuxConfig->emuxInterface         = IfxEvadc_EmuxInterface_0;
    emuxConfig->startChannel          = IfxEvadc_EmuxSelectValue_0;
    emuxConfig->code                  = IfxEvadc_EmuxCodingScheme_binary;
    emuxConfig->sampleTimeControl     = IfxEvadc_EmuxSampleTimeControl_settingChanges;
    emuxConfig->mode                  = IfxEvadc_ExternalMultiplexerMode_softwareControl;
    emuxConfig->channelSelectionStyle = IfxEvadc_ChannelSelectionStyle_channelNumber;
    IfxEvadc_Adc_EmuxPinConfig defaultConfig = {
        .pins       = {NULL_PTR},
        .outputMode = IfxPort_OutputMode_pushPull,
        .padDriver  = (IfxPort_PadDriver)0,
    };

    emuxConfig->emuxOutPinConfig = defaultConfig;
}


void IfxEvadc_Adc_initFastCompareChannel(IfxEvadc_Adc_FastCompareChannel *fastCompareChannel, IfxEvadc_Adc_FastCompareChannelConfig *config)
{
    Ifx_EVADC       *evadc                   = config->module->evadc;

    IfxEvadc_GroupId fastCompareChannelIndex = (IfxEvadc_GroupId)(config->fastCompareChannelId - 12);
    fastCompareChannel->fastCompareChannelId = fastCompareChannelIndex;

    Ifx_EVADC_FC    *evadcFcSFR = &evadc->FC[fastCompareChannelIndex];

    fastCompareChannel->fastCompareChannel = evadcFcSFR;
    fastCompareChannel->module             = *config->module;

    Ifx_EVADC_FC_FCCTRL tempFCCTRL;

    tempFCCTRL.U          = evadcFcSFR->FCCTRL.U;

    tempFCCTRL.B.CPWC     = 1; /* Enabling write permissions */
    tempFCCTRL.B.DIVA     = config->clockDivider;
    tempFCCTRL.B.CHEVMODE = config->channelEventMode;
    tempFCCTRL.B.AIPF     = config->analogInputPrechargeDuration;

    tempFCCTRL.B.RPE      = config->referenceInputPrechargeDuration;

    tempFCCTRL.B.STCF     = config->additionalClockCycles; /* No additional clock cycles in sample phase to be added*/
    tempFCCTRL.B.XTWC     = 1;                             /* Enabling write permissions */
    tempFCCTRL.B.GTMODE   = config->gateOperatingMode;
    tempFCCTRL.B.XTPOL    = config->externalTriggerPolarity;
    tempFCCTRL.B.XTMODE   = config->externalTriggerOperatingMode;

    evadcFcSFR->FCCTRL.U  = tempFCCTRL.U;

    Ifx_EVADC_FC_FCM tempFCM;

    tempFCM.U         = evadcFcSFR->FCM.U;

    tempFCM.B.FCMWC   = 1; /* Enabling write permissions */

    tempFCM.B.SSE     = config->sampleTimingSynchronization;
    tempFCM.B.ACSD    = config->delay;

    tempFCM.B.AUE     = config->automaticUpdateMode;
    tempFCM.B.SRG     = config->serviceRequestGenerationEvent;
    tempFCM.B.FCTRIV  = config->triggerInterval;
    tempFCM.B.ANON    = config->analogConverterControllerMode;
    tempFCM.B.FCRDIR  = config->rampDirection;
    tempFCM.B.RUNRAMP = config->rampGenerationMode;
    tempFCM.B.RUNCOMP = config->runMode;

    evadcFcSFR->FCM.U = tempFCM.U;

    IfxEvadc_disableFastCompareBoundaryFlag(evadcFcSFR); /* Disabling the Boundary Flag to configure it*/
    IfxEvadc_setFastCompareReferenceValue(evadcFcSFR, config->referenceValue);
    IfxEvadc_selectFastCompareBoundaryFlagActivationMode(evadcFcSFR, config->boundaryFlagActivation);
    IfxEvadc_setFastCompareBoundaryFlagInversion(evadcFcSFR, config->boundaryFlagInversion);
    IfxEvadc_setFastCompareBoundaryFlagSwControl(evadcFcSFR, config->boundaryFlagAction);
    IfxEvadc_selectFastCompareBoundaryFlagNodePointer(evadcFcSFR, config->boundaryFlagNodePointer);
    IfxEvadc_enableFastCompareBoundaryFlag(evadcFcSFR); /* Enabling the Boundary Flag after configuring it*/
}


void IfxEvadc_Adc_initFastCompareChannelConfig(IfxEvadc_Adc_FastCompareChannelConfig *config, IfxEvadc_Adc *evadc)
{
    config->module                          = evadc;
    config->fastCompareChannelId            = IfxEvadc_GroupId_12; /*Fast compare channel 0 */
    config->boundaryFlagActivation          = IfxEvadc_BoundaryFlagActivationMode_0;
    config->boundaryFlagInversion           = IfxEvadc_BoundaryFlagInversionControl_direct;
    config->boundaryFlagNodePointer         = IfxEvadc_BoundaryFlagNodePointer_commonBoundaryFlagOutput0;
    config->boundaryFlagAction              = IfxEvadc_BoundaryFlagSwControl_noAction;
    config->channelEventMode                = IfxEvadc_ChannelEventMode_aboveOrBelowCompareValue;
    config->clockDivider                    = IfxEvadc_ClockDividerFactor_maxFrequency;
    config->externalTriggerPolarity         = IfxEvadc_ExternalTriggerPolarity_direct;
    config->analogConverterControllerMode   = IfxEvadc_FastCompareAnalogConverterControl_normal;
    config->automaticUpdateMode             = IfxEvadc_FastCompareAutomaticUpdate_sw;
    config->runMode                         = IfxEvadc_FastCompareRunControl_stop;
    config->rampGenerationMode              = IfxEvadc_RampRunControl_stop;
    config->serviceRequestGenerationEvent   = IfxEvadc_FastCompareServiceRequestGeneration_off;
    config->gateOperatingMode               = IfxEvadc_GateOperatingMode_noGateFunction;
    config->rampDirection                   = IfxEvadc_RampDirection_down;
    config->externalTriggerOperatingMode    = IfxEvadc_TriggerOperatingMode_noExternalTrigger;
    config->additionalClockCycles           = 0;
    config->referenceInputPrechargeDuration = IfxEvadc_ReferencePrechargeControl_singleClockPhase;
    config->delay                           = IfxEvadc_FastCompareAnalogClockSynchronizationDelay_0;
    config->sampleTimingSynchronization     = IfxEvadc_SampleSynchronization_off;
    config->analogInputPrechargeDuration    = IfxEvadc_AnalogInputPrechargeControl_noPrecharge;
    config->triggerInterval                 = 0; /*Interval set as 16 clock cycles*/
    config->referenceValue                  = 512;
    config->boundaryFlagValue               = FALSE;
}


IfxEvadc_Status IfxEvadc_Adc_initGroup(IfxEvadc_Adc_Group *group, const IfxEvadc_Adc_GroupConfig *config)
{
    IfxEvadc_Status status = IfxEvadc_Status_noError;
    Ifx_EVADC      *evadc  = config->module->evadc;
    Ifx_EVADC_G    *evadcG = &evadc->G[config->groupId];
    float32         analogFrequency;

    /* check for write access */
    group->group   = evadcG;
    group->module  = *config->module;
    IfxEvadc_GroupId groupIndex = config->groupId;
    group->groupId = groupIndex;
    uint8            inputClassNum;

    IfxEvadc_enableAccess(evadc, (IfxEvadc_Protection)(IfxEvadc_Protection_initGroup0 + groupIndex));

    IfxEvadc_selectDoubleClockForMSBConversion(&evadc->G[groupIndex], groupIndex, (IfxEvadc_MsbConversionTime)config->doubleClockForMSBConversionSelection);
    IfxEvadc_setSampleSynchronizationMode(&evadc->G[groupIndex], groupIndex, config->sampleSynchronizationEnabled);
    IfxEvadc_setAnalogClockSynchronizationDelay(&evadc->G[groupIndex], groupIndex, config->analogClockSynchronizationDelay);
    IfxEvadc_disablePostCalibration(&evadc->G[groupIndex], groupIndex, config->disablePostCalibration);
    IfxEvadc_setCalibrationSampleTime(&evadc->G[groupIndex], groupIndex, config->calibrationSampleTimeControlMode);
    IfxEvadc_setReferencePrechargeDuration(&evadc->G[groupIndex], groupIndex, config->referencePrechargeControlMode);

    if (config->referencePrechargeEnabled)
    {
        IfxEvadc_enableReferencePrecharge(&evadc->G[groupIndex], groupIndex);
    }
    else
    {
        IfxEvadc_disableReferencePrecharge(&evadc->G[groupIndex], groupIndex);
    }

    if (config->inputBufferEnabled)
    {
        IfxEvadc_enableInputBuffer(&evadc->G[groupIndex], groupIndex);
    }
    else
    {
        IfxEvadc_disableInputBuffer(&evadc->G[groupIndex], groupIndex);
    }

    IfxEvadc_selectIdlePrechargeLevel(&evadc->G[groupIndex], groupIndex, config->idlePrechargeLevel);
    IfxEvadc_setAnalogConvertControl(&evadc->G[groupIndex], config->analogConverterMode);

    IfxEvadc_disableAccess(evadc, (IfxEvadc_Protection)(IfxEvadc_Protection_initGroup0 + groupIndex));

    /* Set Analog Frequency */
    if (IfxEvadc_initializeFAdcI(evadc, config->analogFrequency, group->group) == 0)
    {
        return IfxEvadc_Status_notInitialised;
    }
    else
    {
        /* do nothing */
    }

    analogFrequency = IfxEvadc_getAdcAnalogFrequency(group->group);

    if (config->startupCalibration == TRUE)
    {
        /* execute calibration */
        IfxEvadc_startupCalibration(evadc);
    }

    /* Request Access to configuration registers */
    IfxEvadc_enableAccess(evadc, (IfxEvadc_Protection)(IfxEvadc_Protection_initGroup0 + groupIndex));

    if (config->arbiter.requestSlotQueue0Enabled == TRUE)
    {
        /* Enable Arbiter slot, set Priority and start mode */
        IfxEvadc_setArbiterPriority(evadcG, config->arbiter.requestSlotQueue0Enabled, config->queueRequest[0].requestSlotPrio, config->queueRequest[0].requestSlotStartMode, IfxEvadc_RequestSource_queue0);
    }
    else
    {
        /* Disable the slot */
        IfxEvadc_setArbiterPriority(evadcG, FALSE, IfxEvadc_RequestSlotPriority_lowest, IfxEvadc_RequestSlotStartMode_waitForStart, IfxEvadc_RequestSource_queue0);
    }

    if (config->arbiter.requestSlotQueue1Enabled == TRUE)
    {
        /* Enable Arbiter slot, set Priority and start mode */
        IfxEvadc_setArbiterPriority(evadcG, config->arbiter.requestSlotQueue1Enabled, config->queueRequest[1].requestSlotPrio, config->queueRequest[1].requestSlotStartMode, IfxEvadc_RequestSource_queue1);
    }
    else
    {
        /* Disable the slot */
        IfxEvadc_setArbiterPriority(evadcG, FALSE, IfxEvadc_RequestSlotPriority_lowest, IfxEvadc_RequestSlotStartMode_waitForStart, IfxEvadc_RequestSource_queue1);
    }

    if (config->arbiter.requestSlotQueue2Enabled == TRUE)
    {
        /* Enable Arbiter slot, set Priority and start mode */
        IfxEvadc_setArbiterPriority(evadcG, config->arbiter.requestSlotQueue2Enabled, config->queueRequest[2].requestSlotPrio, config->queueRequest[2].requestSlotStartMode, IfxEvadc_RequestSource_queue2);
    }
    else
    {
        /* Disable the slot */
        IfxEvadc_setArbiterPriority(evadcG, FALSE, IfxEvadc_RequestSlotPriority_lowest, IfxEvadc_RequestSlotStartMode_waitForStart, IfxEvadc_RequestSource_queue2);
    }

    /* master slave configuration */
    if (config->master != groupIndex)
    {
        uint8 masterIndex = IfxEvadc_Adc_getMasterKernelIndex(groupIndex, config->master);
        IfxEvadc_setMasterIndex(evadcG, masterIndex);
    }
    else
    {
        /* do nothing */
    }

    /* Setup arbiter */
    /* Turn off the group during initialization, see UM for sync mode */
    IfxEvadc_setAnalogConvertControl(evadcG, IfxEvadc_AnalogConverterMode_off);

    /* Setup queue0 request if enabled */
    if (config->arbiter.requestSlotQueue0Enabled == TRUE)
    {
        const IfxEvadc_Adc_QueueConfig *queueSlot = &config->queueRequest[0];

        /* configure external Trigger if enabled */
        if (queueSlot->triggerConfig.triggerMode != IfxEvadc_TriggerMode_noExternalTrigger)
        {
            /* enable external trigger */
            IfxEvadc_enableQueueSlotExternalTrigger(evadcG, IfxEvadc_RequestSource_queue0);
            IfxEvadc_setQueueSlotTriggerOperatingConfig(evadcG, queueSlot->triggerConfig.triggerMode, queueSlot->triggerConfig.triggerSource, IfxEvadc_RequestSource_queue0);

            /* if last input is used the trigger input selection is extend by Gating inputs */
            if (queueSlot->triggerConfig.triggerSource == IfxEvadc_TriggerSource_15)
            {
                IfxEvadc_setQueueSlotGatingConfig(evadcG, queueSlot->triggerConfig.gatingSource, IfxEvadc_GatingMode_always, IfxEvadc_RequestSource_queue0);
            }
            else
            {
                /* do nothing, gating is configured later */
            }
        }
        else
        {
            /* disable external trigger */
            IfxEvadc_disableQueueSlotExternalTrigger(evadcG, IfxEvadc_RequestSource_queue0);
        }

        /* configure Gating if enabled */
        if ((queueSlot->triggerConfig.triggerSource != IfxEvadc_TriggerSource_15))
        {
            IfxEvadc_setQueueSlotGatingConfig(evadcG, queueSlot->triggerConfig.gatingSource, queueSlot->triggerConfig.gatingMode, IfxEvadc_RequestSource_queue0);
        }
        else
        {
            /* do nothing, handled by trigger settings */
        }

        IfxEvadc_clearQueue(evadcG, (queueSlot->flushQueueAfterInit) ? 1 : 0, IfxEvadc_RequestSource_queue0);
    }
    else
    {
        /* do nothing */
    }

    if (config->arbiter.requestSlotQueue1Enabled == TRUE)
    {
        const IfxEvadc_Adc_QueueConfig *queueSlot = &config->queueRequest[1];

        /* configure external Trigger if enabled */
        if (queueSlot->triggerConfig.triggerMode != IfxEvadc_TriggerMode_noExternalTrigger)
        {
            /* enable external trigger */
            IfxEvadc_enableQueueSlotExternalTrigger(evadcG, IfxEvadc_RequestSource_queue1);
            IfxEvadc_setQueueSlotTriggerOperatingConfig(evadcG, queueSlot->triggerConfig.triggerMode, queueSlot->triggerConfig.triggerSource, IfxEvadc_RequestSource_queue1);

            /* if last input is used the trigger input selection is extend by Gating inputs */
            if (queueSlot->triggerConfig.triggerSource == IfxEvadc_TriggerSource_15)
            {
                IfxEvadc_setQueueSlotGatingConfig(evadcG, queueSlot->triggerConfig.gatingSource, IfxEvadc_GatingMode_always, IfxEvadc_RequestSource_queue1);
            }
            else
            {
                /* do nothing, gating is configured later */
            }
        }
        else
        {
            /* disable external trigger */
            IfxEvadc_disableQueueSlotExternalTrigger(evadcG, IfxEvadc_RequestSource_queue1);
        }

        /* configure Gating if enabled */
        if ((queueSlot->triggerConfig.triggerSource != IfxEvadc_TriggerSource_15))
        {
            IfxEvadc_setQueueSlotGatingConfig(evadcG, queueSlot->triggerConfig.gatingSource, queueSlot->triggerConfig.gatingMode, IfxEvadc_RequestSource_queue1);
        }
        else
        {
            /* do nothing, handled by trigger settings */
        }

        IfxEvadc_clearQueue(evadcG, (queueSlot->flushQueueAfterInit) ? 1 : 0, IfxEvadc_RequestSource_queue1);
    }
    else
    {
        /* do nothing */
    }

    if (config->arbiter.requestSlotQueue2Enabled == TRUE)
    {
        const IfxEvadc_Adc_QueueConfig *queueSlot = &config->queueRequest[2];

        /* configure external Trigger if enabled */
        if (queueSlot->triggerConfig.triggerMode != IfxEvadc_TriggerMode_noExternalTrigger)
        {
            /* enable external trigger */
            IfxEvadc_enableQueueSlotExternalTrigger(evadcG, IfxEvadc_RequestSource_queue2);
            IfxEvadc_setQueueSlotTriggerOperatingConfig(evadcG, queueSlot->triggerConfig.triggerMode, queueSlot->triggerConfig.triggerSource, IfxEvadc_RequestSource_queue2);

            /* if last input is used the trigger input selection is extend by Gating inputs */
            if (queueSlot->triggerConfig.triggerSource == IfxEvadc_TriggerSource_15)
            {
                IfxEvadc_setQueueSlotGatingConfig(evadcG, queueSlot->triggerConfig.gatingSource, IfxEvadc_GatingMode_always, IfxEvadc_RequestSource_queue2);
            }
            else
            {
                /* do nothing, gating is configured later */
            }
        }
        else
        {
            /* disable external trigger */
            IfxEvadc_disableQueueSlotExternalTrigger(evadcG, IfxEvadc_RequestSource_queue2);
        }

        /* configure Gating if enabled */
        if ((queueSlot->triggerConfig.triggerSource != IfxEvadc_TriggerSource_15))
        {
            IfxEvadc_setQueueSlotGatingConfig(evadcG, queueSlot->triggerConfig.gatingSource, queueSlot->triggerConfig.gatingMode, IfxEvadc_RequestSource_queue2);
        }
        else
        {
            /* do nothing, handled by trigger settings */
        }

        IfxEvadc_clearQueue(evadcG, (queueSlot->flushQueueAfterInit) ? 1 : 0, IfxEvadc_RequestSource_queue2);
    }
    else
    {
        /* do nothing */
    }

    /* turn on group after initialisation, only in master mode */
    IfxEvadc_AnalogConverterMode convertMode = (config->master == groupIndex) ? IfxEvadc_AnalogConverterMode_normalOperation : IfxEvadc_AnalogConverterMode_off;
    IfxEvadc_setAnalogConvertControl(evadcG, convertMode);

    /*  Post Calibration */
    IfxEvadc_disablePostCalibration(evadcG, groupIndex, config->disablePostCalibration);

    for (inputClassNum = 0; inputClassNum < IFXEVADC_NUM_INPUTCLASSES; inputClassNum++)
    {
        /* configure Group input class registers */
        IfxEvadc_setGroupConversionMode(evadcG, inputClassNum, config->inputClass[inputClassNum].conversionMode);
        /* Calculate Sample time ticks */
        IfxEvadc_setGroupSampleTime(evadcG, inputClassNum, analogFrequency, config->inputClass[inputClassNum].sampleTime);
    }

    IfxEvadc_disableAccess(evadc, (IfxEvadc_Protection)(IfxEvadc_Protection_initGroup0 + groupIndex));

    return status;
}


void IfxEvadc_Adc_initGroupConfig(IfxEvadc_Adc_GroupConfig *config, IfxEvadc_Adc *evadc)
{
    static const IfxEvadc_Adc_GroupConfig IfxEvadc_Adc_defaultGroupConfig = {
        .arbiter                             = {
            .arbiterRoundLength       = IfxEvadc_ArbitrationRounds_4_slots,
            .requestSlotQueue0Enabled = FALSE,
            .requestSlotQueue1Enabled = FALSE,
            .requestSlotQueue2Enabled = FALSE,
        },

        .queueRequest                        = {
            {
                .flushQueueAfterInit         = TRUE,
                .triggerConfig.gatingMode    = IfxEvadc_GatingMode_disabled,
                .triggerConfig.gatingSource  = IfxEvadc_GatingSource_0,
                .triggerConfig.triggerMode   = IfxEvadc_TriggerMode_noExternalTrigger,
                .triggerConfig.triggerSource = IfxEvadc_TriggerSource_0,
                .requestSlotPrio             = IfxEvadc_RequestSlotPriority_low,
                .requestSlotStartMode        = IfxEvadc_RequestSlotStartMode_waitForStart,
            },

            {
                .flushQueueAfterInit         = TRUE,
                .triggerConfig.gatingMode    = IfxEvadc_GatingMode_disabled,
                .triggerConfig.gatingSource  = IfxEvadc_GatingSource_0,
                .triggerConfig.triggerMode   = IfxEvadc_TriggerMode_noExternalTrigger,
                .triggerConfig.triggerSource = IfxEvadc_TriggerSource_0,
                .requestSlotPrio             = IfxEvadc_RequestSlotPriority_low,
                .requestSlotStartMode        = IfxEvadc_RequestSlotStartMode_waitForStart,
            },

            {
                .flushQueueAfterInit         = TRUE,
                .triggerConfig.gatingMode    = IfxEvadc_GatingMode_disabled,
                .triggerConfig.gatingSource  = IfxEvadc_GatingSource_0,
                .triggerConfig.triggerMode   = IfxEvadc_TriggerMode_noExternalTrigger,
                .triggerConfig.triggerSource = IfxEvadc_TriggerSource_0,
                .requestSlotPrio             = IfxEvadc_RequestSlotPriority_low,
                .requestSlotStartMode        = IfxEvadc_RequestSlotStartMode_waitForStart,
            }
        },
        .inputClass[0].conversionMode = IfxEvadc_ChannelNoiseReduction_standardConversion,
        .inputClass[0].sampleTime     = 1.0e-6,                           /* Set sample time to 1us */
        .inputClass[1].conversionMode = IfxEvadc_ChannelNoiseReduction_standardConversion,
        .inputClass[1].sampleTime     = 1.0e-6,                           /* Set sample time to 1us */
        .analogFrequency              = IFXEVADC_DEFAULT_ANALOG_FREQ,
        .startupCalibration           = FALSE,
    };

    *config                                      = IfxEvadc_Adc_defaultGroupConfig;
    config->groupId                              = IfxEvadc_GroupId_0;
    config->module                               = evadc;
    config->master                               = config->groupId;
    config->disablePostCalibration               = FALSE;

    config->doubleClockForMSBConversionSelection = FALSE;
    config->sampleSynchronizationEnabled         = IfxEvadc_SampleSynchronization_off;
    config->analogClockSynchronizationDelay      = IfxEvadc_AnalogClockSynchronizationDelay_0;
    config->calibrationSampleTimeControlMode     = IfxEvadc_CalibrationSampleTimeControl_4;
    config->referencePrechargeControlMode        = IfxEvadc_ReferencePrechargeControl_singleClockPhase;
    config->referencePrechargeEnabled            = FALSE;
    config->inputBufferEnabled                   = FALSE;
    config->idlePrechargeLevel                   = IfxEvadc_IdlePrecharge_currentLevel;
    config->analogConverterMode                  = IfxEvadc_AnalogConverterMode_normalOperation;
}


IfxEvadc_Status IfxEvadc_Adc_initModule(IfxEvadc_Adc *evadc, const IfxEvadc_Adc_Config *config)
{
    IfxEvadc_Status status   = IfxEvadc_Status_noError;
    Ifx_EVADC      *evadcSFR = config->evadc;
    evadc->evadc = evadcSFR;
    uint8           inputClassNum;

    /* Enable EVADC kernel clock */
    IfxEvadc_enableModule(evadcSFR);

    /* Set digital Frequency */
    IfxEvadc_initializeFAdcD(evadcSFR, config->digitalFrequency);

    /* configure Global input class registers */
    for (inputClassNum = 0; inputClassNum < IFXEVADC_NUM_GLOBAL_INPUTCLASSES; inputClassNum++)
    {
        /* configure ADC channel  conversion mode  */
        IfxEvadc_setGlobalConversionMode(evadcSFR, inputClassNum, config->globalInputClass[inputClassNum].conversionMode);
    }

    IfxEvadc_Adc_setGlobalConfigurations(evadcSFR, config);

    return status;
}


void IfxEvadc_Adc_initModuleConfig(IfxEvadc_Adc_Config *config, Ifx_EVADC *evadc)
{
    config->evadc                              = evadc;
    config->moduleFrequency                    = 100000000;
    config->digitalFrequency                   = 100000000;
    config->globalInputClass[0].conversionMode = IfxEvadc_ChannelNoiseReduction_standardConversion;
    config->globalInputClass[1].conversionMode = IfxEvadc_ChannelNoiseReduction_standardConversion;
}


void IfxEvadc_Adc_setAnalogConvertControl(Ifx_EVADC *evadc, IfxEvadc_Adc_Group *group, IfxEvadc_AnalogConverterMode analogConverterMode)
{
    IfxEvadc_GroupId groupIndex = group->groupId;
    IfxEvadc_enableAccess(evadc, (IfxEvadc_Protection)(IfxEvadc_Protection_initGroup0 + groupIndex));
    IfxEvadc_setAnalogConvertControl(&evadc->G[groupIndex], analogConverterMode);
    IfxEvadc_disableAccess(evadc, (IfxEvadc_Protection)(IfxEvadc_Protection_initGroup0 + groupIndex));
}


void IfxEvadc_Adc_setGlobalConfigurations(Ifx_EVADC *evadc, const IfxEvadc_Adc_Config *config)
{
    Ifx_EVADC_GLOBCFG tempGLOBCFG;
    tempGLOBCFG.U        = evadc->GLOBCFG.U;
    tempGLOBCFG.B.CPWC   = 1; //Write Access to SUPLEV and USC
    tempGLOBCFG.B.USC    = config->analogClockGenerationMode;
    tempGLOBCFG.B.SUPLEV = config->supplyVoltage;
    tempGLOBCFG.B.SUCAL  = config->startupCalibrationControl;
    evadc->GLOBCFG.U     = tempGLOBCFG.U;
}
