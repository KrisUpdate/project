/**
 * \file IfxCan_Can.c
 * \brief CAN CAN details
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

#include "IfxCan_Can.h"
#include "Mcmcan.h"

extern CansetupConfig can_config[6];

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

void IfxCan_Can_initMessage(IfxCan_Message *message)
{
    IfxCan_Message defaultMessage = {
        .bufferNumber          = 0,
        .messageId             = 0x0,
        .remoteTransmitRequest = 0,
        .messageIdLength       = IfxCan_MessageIdLength_standard,
        .errorStateIndicator   = 0,
        .dataLengthCode        = IfxCan_DataLengthCode_8,
        .frameMode             = IfxCan_FrameMode_standard,
        .txEventFifoControl    = 0,
        .storeInTxFifoQueue    = FALSE,
        .readFromRxFifo0       = FALSE,
        .readFromRxFifo1       = FALSE
    };
    for(int i; i<6; i++){
        if(can_config[i].is_fd==TRUE){
			defaultMessage.frameMode = IfxCan_FrameMode_fdLongAndFast;
		}
	}
    /* Default Configuration */
    *message = defaultMessage;
}


void IfxCan_Can_initModule(IfxCan_Can *can, IfxCan_Can_Config *config)
{
    can->can = config->can;

    // if module is not enebled
    if (IfxCan_isModuleEnabled(can->can) != TRUE)
    {
        // Enable module, disregard Sleep Mode request
        IfxCan_enableModule(can->can);
    }
}


void IfxCan_Can_initModuleConfig(IfxCan_Can_Config *config, Ifx_CAN *can)
{
    const IfxCan_Can_Config defaultConfig = {
        .can = NULL_PTR
    };

    /* Default Configuration */
    *config = defaultConfig;

    /* take over module pointer */
    config->can = can;
}


boolean IfxCan_Can_initNode(IfxCan_Can_Node *node, const IfxCan_Can_NodeConfig *config)
{
    Ifx_CAN   *canSfr  = config->can;
    node->can = canSfr;
    boolean    status  = 0;

    Ifx_CAN_N *nodeSfr = IfxCan_getNodePointer(canSfr, config->nodeId);
    node->node       = nodeSfr;

    node->messageRAM = config->messageRAM;

    /* initialise the clock for the selected node */
    IfxCan_ClockSelect clockSelect = (IfxCan_ClockSelect)config->nodeId;
    IfxCan_setClockSource(canSfr, clockSelect, config->clockSource);

    /* enable configuration change CCCR.CCE = 1, CCCR.INIT = 1 */
    IfxCan_Node_enableConfigurationChange(nodeSfr);

    float32 moduleFreq = IfxCan_getModuleFrequency(canSfr);

    /* set baudrate */
    if (config->calculateBitTimingValues)
    {
        IfxCan_Node_setBitTiming(nodeSfr, moduleFreq, config->baudRate.baudrate, config->baudRate.samplePoint, config->baudRate.syncJumpWidth);
    }
    else
    {
        IfxCan_Node_setBitTimingValues(nodeSfr, config->baudRate.syncJumpWidth, config->baudRate.timeSegment2, config->baudRate.timeSegment1, config->baudRate.prescaler);
    }

    /* for CAN FD frames, set fast baudrate */
    if (config->frame.mode != IfxCan_FrameMode_standard)
    {
        if (config->calculateBitTimingValues)
        {
            IfxCan_Node_setFastBitTiming(nodeSfr, moduleFreq, config->fastBaudRate.baudrate, config->fastBaudRate.samplePoint, config->fastBaudRate.syncJumpWidth);
        }
        else
        {
            IfxCan_Node_setFastBitTimingValues(nodeSfr, config->baudRate.syncJumpWidth, config->baudRate.timeSegment2, config->baudRate.timeSegment1, config->baudRate.prescaler);
        }

        /* set transceiver delay compensation offset */
        if (config->fastBaudRate.tranceiverDelayOffset != 0)
        {
            IfxCan_Node_setTransceiverDelayCompensationOffset(nodeSfr, config->fastBaudRate.tranceiverDelayOffset);
        }
    }

    /* transmit frame configuration */

    if ((config->frame.type == IfxCan_FrameType_transmit) ||
        (config->frame.type == IfxCan_FrameType_transmitAndReceive) ||
        (config->frame.type == IfxCan_FrameType_remoteRequest) ||
        (config->frame.type == IfxCan_FrameType_remoteAnswer))
    {
        uint32 id; /* used for enabling transmit interrupts for individual Tx buffers */

        /* set Tx element data field size */
        IfxCan_Node_setTxBufferDataFieldSize(nodeSfr, config->txConfig.txBufferDataFieldSize);

        /* set Tx buffers start address in the Message RAM */
        IfxCan_Node_setTxBuffersStartAddress(nodeSfr, config->messageRAM.txBuffersStartAddress);

        if ((config->txConfig.txMode == IfxCan_TxMode_dedicatedBuffers) ||
            (config->txConfig.txMode == IfxCan_TxMode_sharedFifo) ||
            (config->txConfig.txMode == IfxCan_TxMode_sharedQueue))
        {
            /* dedicated TX buffers operation */
            /* select number of Tx buffers */
            IfxCan_Node_setDedicatedTxBuffersNumber(nodeSfr, config->txConfig.dedicatedTxBuffersNumber);

            /* shared operation */
            if ((config->txConfig.txMode == IfxCan_TxMode_sharedFifo) || (config->txConfig.txMode == IfxCan_TxMode_sharedQueue))
            {
                if (config->txConfig.txMode == IfxCan_TxMode_sharedFifo)
                {
                    /* set Tx FIFO mode */
                    IfxCan_Node_setTransmitFifoQueueMode(nodeSfr, IfxCan_TxMode_fifo);
                }

                if (config->txConfig.txMode == IfxCan_TxMode_sharedQueue)
                {
                    /* set Tx Queue mode */
                    IfxCan_Node_setTransmitFifoQueueMode(nodeSfr, IfxCan_TxMode_queue);
                }

                /* select number of Tx buffers to be used as Tx FIFO/Queue */
                IfxCan_Node_setTransmitFifoQueueSize(nodeSfr, config->txConfig.txFifoQueueSize);
            }

            /* enable transmission interrupt on selected buffers */
            for (id = 0; id < (config->txConfig.dedicatedTxBuffersNumber + config->txConfig.txFifoQueueSize); ++id)
            {
                IfxCan_Node_enableTxBufferTransmissionInterrupt(nodeSfr, (IfxCan_TxBufferId)id);
            }
        }

        /* TX FIFO/Queue operation */
        else if ((config->txConfig.txMode == IfxCan_TxMode_fifo) || (config->txConfig.txMode == IfxCan_TxMode_queue))
        {
            /* set Tx FIFO/Queue mode */
            IfxCan_Node_setTransmitFifoQueueMode(nodeSfr, config->txConfig.txMode);

            /* select number of Tx buffers to be used as Tx FIFO/Queue */
            IfxCan_Node_setTransmitFifoQueueSize(nodeSfr, config->txConfig.txFifoQueueSize);

            /* enable transmission interrupt on selected buffers */
            for (id = 0; id < config->txConfig.txFifoQueueSize; ++id)
            {
                IfxCan_Node_enableTxBufferTransmissionInterrupt(nodeSfr, (IfxCan_TxBufferId)id);
            }
        }
        /* wrong selection */
        else
        {
            IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, 0);
        }

        /* set the Tx event fifo size and start address if selected */
        if ((config->txConfig.txEventFifoSize > 0) && (config->txConfig.txEventFifoSize <= 32))
        {
            IfxCan_Node_setTxEventFifoStartAddress(nodeSfr, config->messageRAM.txEventFifoStartAddress);
            IfxCan_Node_setTxEventFifoSize(nodeSfr, config->txConfig.txEventFifoSize);
        }
        else /* maximum number of configurable Tx Event FIFO elements is 32 */
        {
            IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, config->txConfig.txEventFifoSize <= 32);
        }

        /* enable CAN frame mode of transmission Standard or CAN FD Long or FD Long and Fast frames*/
        IfxCan_Node_setFrameMode(nodeSfr, config->frame.mode);
    }

    /* receive frame configuration */

    if ((config->frame.type == IfxCan_FrameType_receive) ||
        (config->frame.type == IfxCan_FrameType_transmitAndReceive) ||
        (config->frame.type == IfxCan_FrameType_remoteAnswer))
    {
        if ((config->rxConfig.rxMode == IfxCan_RxMode_dedicatedBuffers) ||
            (config->rxConfig.rxMode == IfxCan_RxMode_sharedFifo0) ||
            (config->rxConfig.rxMode == IfxCan_RxMode_sharedFifo1) ||
            (config->rxConfig.rxMode == IfxCan_RxMode_sharedAll))
        {
            /* set Rx buffer data length */
            IfxCan_Node_setRxBufferDataFieldSize(nodeSfr, config->rxConfig.rxBufferDataFieldSize);

            /* set Rx buffers start address in the Message RAM */
            IfxCan_Node_setRxBuffersStartAddress(nodeSfr, config->messageRAM.rxBuffersStartAddress);
        }

        if ((config->rxConfig.rxMode == IfxCan_RxMode_fifo0) ||
            (config->rxConfig.rxMode == IfxCan_RxMode_sharedFifo0) ||
            (config->rxConfig.rxMode == IfxCan_RxMode_sharedAll))
        {
            /* set Rx FIFO 0 data length */
            IfxCan_Node_setRxFifo0DataFieldSize(nodeSfr, config->rxConfig.rxFifo0DataFieldSize);

            /* set Rx FIFO 0 start address in the Message RAM */
            IfxCan_Node_setRxFifo0StartAddress(nodeSfr, config->messageRAM.rxFifo0StartAddress);

            /* set Rx FIFO 0 size */
            IfxCan_Node_setRxFifo0Size(nodeSfr, config->rxConfig.rxFifo0Size);

            /* set Rx FIFO 0 operating mode */
            IfxCan_Node_setRxFifo0OperatingMode(nodeSfr, config->rxConfig.rxFifo0OperatingMode);

            /* set Rx FIFO 0 watermark level */
            IfxCan_Node_setRxFifo0WatermarkLevel(nodeSfr, config->rxConfig.rxFifo0WatermarkLevel);
        }

        if ((config->rxConfig.rxMode == IfxCan_RxMode_fifo1) ||
            (config->rxConfig.rxMode == IfxCan_RxMode_sharedFifo1) ||
            (config->rxConfig.rxMode == IfxCan_RxMode_sharedAll))
        {
            /* set Rx FIFO 1 data length */
            IfxCan_Node_setRxFifo1DataFieldSize(nodeSfr, config->rxConfig.rxFifo1DataFieldSize);

            /* set Rx FIFO 1 start address in the Message RAM */
            IfxCan_Node_setRxFifo1StartAddress(nodeSfr, config->messageRAM.rxFifo1StartAddress);

            /* set Rx FIFO 1 size */
            IfxCan_Node_setRxFifo1Size(nodeSfr, config->rxConfig.rxFifo1Size);

            /* set Rx FIFO 1 operating mode */
            IfxCan_Node_setRxFifo1OperatingMode(nodeSfr, config->rxConfig.rxFifo1OperatingMode);

            /* set Rx FIFO 1 watermark level */
            IfxCan_Node_setRxFifo1WatermarkLevel(nodeSfr, config->rxConfig.rxFifo1WatermarkLevel);
        }

        /* enable CAN frame mode of transmission */
        IfxCan_Node_setFrameMode(nodeSfr, config->frame.mode);

        /* filter configuration */

        if ((config->filterConfig.messageIdLength == IfxCan_MessageIdLength_standard) ||
            (config->filterConfig.messageIdLength == IfxCan_MessageIdLength_both))
        {
            IfxCan_Node_setStandardFilterListStartAddress(nodeSfr, config->messageRAM.standardFilterListStartAddress);
            IfxCan_Node_setStandardFilterListSize(nodeSfr, config->filterConfig.standardListSize);
            IfxCan_Node_configureStandardFilterForNonMatchingFrames(nodeSfr, config->filterConfig.standardFilterForNonMatchingFrames);

            if (config->filterConfig.rejectRemoteFramesWithStandardId == 1U)
            {
                IfxCan_Node_rejectRemoteFramesWithStandardId(nodeSfr);
            }
        }

        if ((config->filterConfig.messageIdLength == IfxCan_MessageIdLength_extended) ||
            (config->filterConfig.messageIdLength == IfxCan_MessageIdLength_both))
        {
            IfxCan_Node_setExtendedFilterListStartAddress(nodeSfr, config->messageRAM.extendedFilterListStartAddress);
            IfxCan_Node_setExtendedFilterListSize(nodeSfr, config->filterConfig.extendedListSize);
            IfxCan_Node_configureExtendedFilterForNonMatchingFrames(nodeSfr, config->filterConfig.extendedFilterForNonMatchingFrames);

            if (config->filterConfig.rejectRemoteFramesWithExtendedId == 1U)
            {
                IfxCan_Node_rejectRemoteFramesWithExtendedId(nodeSfr);
            }
        }
    }

    /* pins initialisation */

    if (config->pins != NULL_PTR)
    {
        if (config->pins->txPin != NULL_PTR)
        {
            status |= (IfxCan_Node_initTxPin(nodeSfr, config->pins->txPin, config->pins->txPinMode, config->pins->padDriver));
        }

        if (config->pins->rxPin != NULL_PTR)
        {
            status |= (IfxCan_Node_initRxPin(nodeSfr, config->pins->rxPin, config->pins->rxPinMode, config->pins->padDriver));
        }
    }

    /* enable internal virtual CAN bus loopback mode if selected */
    if (config->busLoopbackEnabled)
    {
        IfxCan_Node_enableLoopbackMode(nodeSfr);
    }

    // interrupt groups configuration
    volatile Ifx_SRC_SRCR *srcPointer;

    if (config->interruptConfig.tefifo.priority > 0)
    {
        IfxCan_Node_setGroupInterruptLine(nodeSfr, IfxCan_InterruptGroup_tefifo, config->interruptConfig.tefifo.interruptLine);
        srcPointer = IfxCan_getSrcPointer(config->can, config->interruptConfig.tefifo.interruptLine);
        IfxSrc_init(srcPointer, config->interruptConfig.tefifo.typeOfService, config->interruptConfig.tefifo.priority);
        IfxSrc_enable(srcPointer);
    }

    if (config->interruptConfig.hpe.priority > 0)
    {
        IfxCan_Node_setGroupInterruptLine(nodeSfr, IfxCan_InterruptGroup_hpe, config->interruptConfig.hpe.interruptLine);
        srcPointer = IfxCan_getSrcPointer(config->can, config->interruptConfig.hpe.interruptLine);
        IfxSrc_init(srcPointer, config->interruptConfig.hpe.typeOfService, config->interruptConfig.hpe.priority);
        IfxSrc_enable(srcPointer);
    }

    if (config->interruptConfig.wati.priority > 0)
    {
        IfxCan_Node_setGroupInterruptLine(nodeSfr, IfxCan_InterruptGroup_wati, config->interruptConfig.wati.interruptLine);
        srcPointer = IfxCan_getSrcPointer(config->can, config->interruptConfig.wati.interruptLine);
        IfxSrc_init(srcPointer, config->interruptConfig.wati.typeOfService, config->interruptConfig.wati.priority);
        IfxSrc_enable(srcPointer);
    }

    if (config->interruptConfig.alrt.priority > 0)
    {
        IfxCan_Node_setGroupInterruptLine(nodeSfr, IfxCan_InterruptGroup_alrt, config->interruptConfig.alrt.interruptLine);
        srcPointer = IfxCan_getSrcPointer(config->can, config->interruptConfig.alrt.interruptLine);
        IfxSrc_init(srcPointer, config->interruptConfig.alrt.typeOfService, config->interruptConfig.alrt.priority);
        IfxSrc_enable(srcPointer);
    }

    if (config->interruptConfig.moer.priority > 0)
    {
        IfxCan_Node_setGroupInterruptLine(nodeSfr, IfxCan_InterruptGroup_moer, config->interruptConfig.moer.interruptLine);
        srcPointer = IfxCan_getSrcPointer(config->can, config->interruptConfig.moer.interruptLine);
        IfxSrc_init(srcPointer, config->interruptConfig.moer.typeOfService, config->interruptConfig.moer.priority);
        IfxSrc_enable(srcPointer);
    }

    if (config->interruptConfig.safe.priority > 0)
    {
        IfxCan_Node_setGroupInterruptLine(nodeSfr, IfxCan_InterruptGroup_safe, config->interruptConfig.safe.interruptLine);
        srcPointer = IfxCan_getSrcPointer(config->can, config->interruptConfig.safe.interruptLine);
        IfxSrc_init(srcPointer, config->interruptConfig.safe.typeOfService, config->interruptConfig.safe.priority);
        IfxSrc_enable(srcPointer);
    }

    if (config->interruptConfig.boff.priority > 0)
    {
        IfxCan_Node_setGroupInterruptLine(nodeSfr, IfxCan_InterruptGroup_boff, config->interruptConfig.boff.interruptLine);
        srcPointer = IfxCan_getSrcPointer(config->can, config->interruptConfig.boff.interruptLine);
        IfxSrc_init(srcPointer, config->interruptConfig.boff.typeOfService, config->interruptConfig.boff.priority);
        IfxSrc_enable(srcPointer);
    }

    if (config->interruptConfig.loi.priority > 0)
    {
        IfxCan_Node_setGroupInterruptLine(nodeSfr, IfxCan_InterruptGroup_loi, config->interruptConfig.loi.interruptLine);
        srcPointer = IfxCan_getSrcPointer(config->can, config->interruptConfig.loi.interruptLine);
        IfxSrc_init(srcPointer, config->interruptConfig.loi.typeOfService, config->interruptConfig.loi.priority);
        IfxSrc_enable(srcPointer);
    }

    if (config->interruptConfig.reint.priority > 0)
    {
        IfxCan_Node_setGroupInterruptLine(nodeSfr, IfxCan_InterruptGroup_reint, config->interruptConfig.reint.interruptLine);
        srcPointer = IfxCan_getSrcPointer(config->can, config->interruptConfig.reint.interruptLine);
        IfxSrc_init(srcPointer, config->interruptConfig.reint.typeOfService, config->interruptConfig.reint.priority);
        IfxSrc_enable(srcPointer);
    }

    if (config->interruptConfig.rxf1f.priority > 0)
    {
        IfxCan_Node_setGroupInterruptLine(nodeSfr, IfxCan_InterruptGroup_rxf1f, config->interruptConfig.rxf1f.interruptLine);
        srcPointer = IfxCan_getSrcPointer(config->can, config->interruptConfig.rxf1f.interruptLine);
        IfxSrc_init(srcPointer, config->interruptConfig.rxf1f.typeOfService, config->interruptConfig.rxf1f.priority);
        IfxSrc_enable(srcPointer);
    }

    if (config->interruptConfig.rxf0f.priority > 0)
    {
        IfxCan_Node_setGroupInterruptLine(nodeSfr, IfxCan_InterruptGroup_rxf0f, config->interruptConfig.rxf0f.interruptLine);
        srcPointer = IfxCan_getSrcPointer(config->can, config->interruptConfig.rxf0f.interruptLine);
        IfxSrc_init(srcPointer, config->interruptConfig.rxf0f.typeOfService, config->interruptConfig.rxf0f.priority);
        IfxSrc_enable(srcPointer);
    }

    if (config->interruptConfig.rxf1n.priority > 0)
    {
        IfxCan_Node_setGroupInterruptLine(nodeSfr, IfxCan_InterruptGroup_rxf1n, config->interruptConfig.rxf1n.interruptLine);
        srcPointer = IfxCan_getSrcPointer(config->can, config->interruptConfig.rxf1n.interruptLine);
        IfxSrc_init(srcPointer, config->interruptConfig.rxf1n.typeOfService, config->interruptConfig.rxf1n.priority);
        IfxSrc_enable(srcPointer);
    }

    if (config->interruptConfig.rxf0n.priority > 0)
    {
        IfxCan_Node_setGroupInterruptLine(nodeSfr, IfxCan_InterruptGroup_rxf0n, config->interruptConfig.rxf0n.interruptLine);
        srcPointer = IfxCan_getSrcPointer(config->can, config->interruptConfig.rxf0n.interruptLine);
        IfxSrc_init(srcPointer, config->interruptConfig.rxf0n.typeOfService, config->interruptConfig.rxf0n.priority);
        IfxSrc_enable(srcPointer);
    }

    if (config->interruptConfig.reti.priority > 0)
    {
        IfxCan_Node_setGroupInterruptLine(nodeSfr, IfxCan_InterruptGroup_reti, config->interruptConfig.reti.interruptLine);
        srcPointer = IfxCan_getSrcPointer(config->can, config->interruptConfig.reti.interruptLine);
        IfxSrc_init(srcPointer, config->interruptConfig.reti.typeOfService, config->interruptConfig.reti.priority);
        IfxSrc_enable(srcPointer);
    }

    if (config->interruptConfig.traq.priority > 0)
    {
        IfxCan_Node_setGroupInterruptLine(nodeSfr, IfxCan_InterruptGroup_traq, config->interruptConfig.traq.interruptLine);
        srcPointer = IfxCan_getSrcPointer(config->can, config->interruptConfig.traq.interruptLine);
        IfxSrc_init(srcPointer, config->interruptConfig.traq.typeOfService, config->interruptConfig.traq.priority);
        IfxSrc_enable(srcPointer);
    }

    if (config->interruptConfig.traco.priority > 0)
    {
        IfxCan_Node_setGroupInterruptLine(nodeSfr, IfxCan_InterruptGroup_traco, config->interruptConfig.traco.interruptLine);
        srcPointer = IfxCan_getSrcPointer(config->can, config->interruptConfig.traco.interruptLine);
        IfxSrc_init(srcPointer, config->interruptConfig.traco.typeOfService, config->interruptConfig.traco.priority);
        IfxSrc_enable(srcPointer);
    }

    /* enable the selected interrupts */
    if (config->interruptConfig.rxFifo0NewMessageEnabled)
    {
        IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_rxFifo0NewMessage);
    }

    if (config->interruptConfig.rxFifo0WatermarkEnabled)
    {
        IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_rxFifo0WatermarkReached);
    }

    if (config->interruptConfig.rxFifo0FullEnabled)
    {
        IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_rxFifo0Full);
    }

    if (config->interruptConfig.rxFifo0MessageLostEnabled)
    {
        IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_rxFifo0MessageLost);
    }

    if (config->interruptConfig.rxFifo1NewMessageEnabled)
    {
        IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_rxFifo1NewMessage);
    }

    if (config->interruptConfig.rxFifo1WatermarkEnabled)
    {
        IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_rxFifo1WatermarkReached);
    }

    if (config->interruptConfig.rxFifo1FullEnabled)
    {
        IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_rxFifo1Full);
    }

    if (config->interruptConfig.rxFifo1MessageLostEnabled)
    {
        IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_rxFifo1MessageLost);
    }

    if (config->interruptConfig.highPriorityMessageEnabled)
    {
        IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_highPriorityMessage);
    }

    if (config->interruptConfig.transmissionCompletedEnabled)
    {
        IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_transmissionCompleted);
    }

    if (config->interruptConfig.transmissionCancellationFinishedEnabled)
    {
        IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_transmissionCancellationFinished);
    }

    if (config->interruptConfig.txFifoEmptyEnabled)
    {
        IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_txFifoEmpty);
    }

    if (config->interruptConfig.txEventFifoNewEntryEnabled)
    {
        IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_txEventFifoNewEntry);
    }

    if (config->interruptConfig.txEventFifoWatermarkEnabled)
    {
        IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_txEventFifoWatermarkReached);
    }

    if (config->interruptConfig.txEventFifoFullEnabled)
    {
        IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_txEventFifoFull);
    }

    if (config->interruptConfig.txEventFifoEventLostEnabled)
    {
        IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_txEventFifoEventLost);
    }

    if (config->interruptConfig.timestampWraparoundEnabled)
    {
        IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_timestampWraparound);
    }

    if (config->interruptConfig.messageRAMAccessFailureEnabled)
    {
        IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_messageRAMAccessFailure);
    }

    if (config->interruptConfig.timeoutOccurredEnabled)
    {
        IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_timeoutOccurred);
    }

    if (config->interruptConfig.messageStoredToDedicatedRxBufferEnabled)
    {
        IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_messageStoredToDedicatedRxBuffer);
    }

    if (config->interruptConfig.errorLoggingOverflowEnabled)
    {
        IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_errorLoggingOverflow);
    }

    if (config->interruptConfig.errorPassiveEnabled)
    {
        IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_errorPassive);
    }

    if (config->interruptConfig.warningStatusEnabled)
    {
        IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_warningStatus);
    }

    if (config->interruptConfig.busOffStatusEnabled)
    {
        IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_busOffStatus);
    }

    if (config->interruptConfig.watchdogEnabled)
    {
        IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_watchdog);
    }

    if (config->interruptConfig.protocolErrorArbitrationEnabled)
    {
        IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_protocolErrorArbitration);
    }

    if (config->interruptConfig.protocolErrorDataEnabled)
    {
        IfxCan_Node_enableInterrupt(nodeSfr, IfxCan_Interrupt_protocolErrorData);
    }

    /* disable configuration change CCCR.CCE = 0 */
    IfxCan_Node_disableConfigurationChange(nodeSfr);

    return status;
}


void IfxCan_Can_initNodeConfig(IfxCan_Can_NodeConfig *config, IfxCan_Can *can)
{
    const IfxCan_Can_NodeConfig defaultConfig = {
        .can         = NULL_PTR,
        .nodeId      = IfxCan_NodeId_0,
        .clockSource = IfxCan_ClockSource_both,
        .frame       = {
            .type = IfxCan_FrameType_receive,
            .mode = IfxCan_FrameMode_standard
        },
        .baudRate                                    = {
            .baudrate      = 500000,
            .samplePoint   = 8000,
            .syncJumpWidth = 3,
            .prescaler     = 0,
            .timeSegment1  = 3,
            .timeSegment2  = 10
        },
        .fastBaudRate                                = {
            .baudrate              = 1000000,
            .samplePoint           = 8000,
            .syncJumpWidth         = 3,
            .prescaler             = 1,
            .timeSegment1          = 3,
            .timeSegment2          = 10,
            .tranceiverDelayOffset = 0
        },
        .txConfig                                    = {
            .txMode                   = IfxCan_TxMode_dedicatedBuffers,
            .dedicatedTxBuffersNumber = 2,
            .txFifoQueueSize          = 0,
            .txBufferDataFieldSize    = IfxCan_DataFieldSize_8,
            .txEventFifoSize          = 0
        },
        .filterConfig                                = {
            .messageIdLength                    = IfxCan_MessageIdLength_standard,
            .standardListSize                   = 2,
            .extendedListSize                   = 0,
            .rejectRemoteFramesWithStandardId   = 0,
            .rejectRemoteFramesWithExtendedId   = 0,
            .standardFilterForNonMatchingFrames = IfxCan_NonMatchingFrame_acceptToRxFifo0,
            .extendedFilterForNonMatchingFrames = IfxCan_NonMatchingFrame_acceptToRxFifo0
        },
        .rxConfig                                    = {
            .rxMode                = IfxCan_RxMode_dedicatedBuffers,
            .rxBufferDataFieldSize = IfxCan_DataFieldSize_8,
            .rxFifo0DataFieldSize  = IfxCan_DataFieldSize_8,
            .rxFifo1DataFieldSize  = IfxCan_DataFieldSize_8,
            .rxFifo0OperatingMode  = IfxCan_RxFifoMode_blocking,
            .rxFifo1OperatingMode  = IfxCan_RxFifoMode_blocking,
            .rxFifo0WatermarkLevel = 0,
            .rxFifo1WatermarkLevel = 0,
            .rxFifo0Size           = 0,
            .rxFifo1Size           = 0
        },
        .messageRAM                                  = {
            .baseAddress                    = (uint32)&MODULE_CAN0,
            .standardFilterListStartAddress = 0x0,
            .extendedFilterListStartAddress = 0x80,
            .rxFifo0StartAddress            = 0x100,
            .rxFifo1StartAddress            = 0x200,
            .rxBuffersStartAddress          = 0x300,
            .txEventFifoStartAddress        = 0x400,
            .txBuffersStartAddress          = 0x440
        },
        .interruptConfig                             = {
            .rxFifo0NewMessageEnabled                = FALSE,
            .rxFifo0WatermarkEnabled                 = FALSE,
            .rxFifo0FullEnabled                      = FALSE,
            .rxFifo0MessageLostEnabled               = FALSE,
            .rxFifo1NewMessageEnabled                = FALSE,
            .rxFifo1WatermarkEnabled                 = FALSE,
            .rxFifo1FullEnabled                      = FALSE,
            .rxFifo1MessageLostEnabled               = FALSE,
            .highPriorityMessageEnabled              = FALSE,
            .transmissionCompletedEnabled            = FALSE,
            .transmissionCancellationFinishedEnabled = FALSE,
            .txFifoEmptyEnabled                      = FALSE,
            .txEventFifoNewEntryEnabled              = FALSE,
            .txEventFifoWatermarkEnabled             = FALSE,
            .txEventFifoFullEnabled                  = FALSE,
            .txEventFifoEventLostEnabled             = FALSE,
            .timestampWraparoundEnabled              = FALSE,
            .messageRAMAccessFailureEnabled          = FALSE,
            .timeoutOccurredEnabled                  = FALSE,
            .messageStoredToDedicatedRxBufferEnabled = FALSE,
            .errorLoggingOverflowEnabled             = FALSE,
            .errorPassiveEnabled                     = FALSE,
            .warningStatusEnabled                    = FALSE,
            .busOffStatusEnabled                     = FALSE,
            .watchdogEnabled                         = FALSE,
            .protocolErrorArbitrationEnabled         = FALSE,
            .protocolErrorDataEnabled                = FALSE,
            .tefifo                                  = {
                .interruptLine = IfxCan_InterruptLine_0,
                .priority      = 0,
                .typeOfService = IfxSrc_Tos_cpu0
            },
            .hpe                                     = {
                .interruptLine = IfxCan_InterruptLine_0,
                .priority      = 0,
                .typeOfService = IfxSrc_Tos_cpu0
            },
            .wati                                    = {
                .interruptLine = IfxCan_InterruptLine_0,
                .priority      = 0,
                .typeOfService = IfxSrc_Tos_cpu0
            },
            .alrt                                    = {
                .interruptLine = IfxCan_InterruptLine_0,
                .priority      = 0,
                .typeOfService = IfxSrc_Tos_cpu0
            },
            .moer                                    = {
                .interruptLine = IfxCan_InterruptLine_0,
                .priority      = 0,
                .typeOfService = IfxSrc_Tos_cpu0
            },
            .safe                                    = {
                .interruptLine = IfxCan_InterruptLine_0,
                .priority      = 0,
                .typeOfService = IfxSrc_Tos_cpu0
            },
            .boff                                    = {
                .interruptLine = IfxCan_InterruptLine_0,
                .priority      = 0,
                .typeOfService = IfxSrc_Tos_cpu0
            },
            .loi                                     = {
                .interruptLine = IfxCan_InterruptLine_0,
                .priority      = 0,
                .typeOfService = IfxSrc_Tos_cpu0
            },
            .reint                                   = {
                .interruptLine = IfxCan_InterruptLine_0,
                .priority      = 0,
                .typeOfService = IfxSrc_Tos_cpu0
            },
            .rxf1f                                   = {
                .interruptLine = IfxCan_InterruptLine_0,
                .priority      = 0,
                .typeOfService = IfxSrc_Tos_cpu0
            },
            .rxf0f                                   = {
                .interruptLine = IfxCan_InterruptLine_0,
                .priority      = 0,
                .typeOfService = IfxSrc_Tos_cpu0
            },
            .rxf1n                                   = {
                .interruptLine = IfxCan_InterruptLine_0,
                .priority      = 0,
                .typeOfService = IfxSrc_Tos_cpu0
            },
            .rxf0n                                   = {
                .interruptLine = IfxCan_InterruptLine_0,
                .priority      = 0,
                .typeOfService = IfxSrc_Tos_cpu0
            },
            .reti                                    = {
                .interruptLine = IfxCan_InterruptLine_0,
                .priority      = 0,
                .typeOfService = IfxSrc_Tos_cpu0
            },
            .traq                                    = {
                .interruptLine = IfxCan_InterruptLine_0,
                .priority      = 0,
                .typeOfService = IfxSrc_Tos_cpu0
            },
            .traco                                   = {
                .interruptLine = IfxCan_InterruptLine_0,
                .priority      = 0,
                .typeOfService = IfxSrc_Tos_cpu0
            }
        },
        .pins                     = NULL_PTR,
        .busLoopbackEnabled       = FALSE,
        .calculateBitTimingValues = TRUE
    };

    /* Default Configuration */
    *config = defaultConfig;

    /* take over module pointer */
    config->can = can->can;
}


void IfxCan_Can_readMessage(IfxCan_Can_Node *node, IfxCan_Message *message, uint32 *data)
{
    IfxCan_RxBufferId bufferId = IfxCan_RxBufferId_0;
    Ifx_CAN_RXMSG    *rxBufferElement;

    if ((message->readFromRxFifo0) || (message->readFromRxFifo1))
    {
        if (message->readFromRxFifo0)
        {
            /* get the Tx FIFO 0 ELement address */
            bufferId        = IfxCan_Node_getRxFifo0GetIndex(node->node);
            rxBufferElement = IfxCan_Node_getRxFifo0ElementAddress(node->node, node->messageRAM.baseAddress, node->messageRAM.rxFifo0StartAddress, bufferId);
        }
        else
        {
            /* get the Tx FIFO 1 ELement address */
            bufferId        = IfxCan_Node_getRxFifo1GetIndex(node->node);
            rxBufferElement = IfxCan_Node_getRxFifo1ElementAddress(node->node, node->messageRAM.baseAddress, node->messageRAM.rxFifo1StartAddress, bufferId);
        }
    }
    else
    {
        /* get the Rx Bufer ELement address */
        bufferId        = (IfxCan_RxBufferId)message->bufferNumber;
        rxBufferElement = IfxCan_Node_getRxBufferElementAddress(node->node, node->messageRAM.baseAddress, node->messageRAM.rxBuffersStartAddress, bufferId);
    }
    message->messageIdLength = (IfxCan_MessageIdLength)rxBufferElement->R0.B.XTD;/* [EXTENED ID test] */

    /*get message ID */
    message->messageId = IfxCan_Node_getMesssageId(node->node, rxBufferElement);

    /* get data length code*/
    message->dataLengthCode = (IfxCan_DataLengthCode)IfxCan_Node_getDataLengthCode(node->node, rxBufferElement);

    /* get CAN frame mode of operation */
    message->frameMode = IfxCan_Node_getFrameMode(node->node, rxBufferElement);

    /* read data */
    IfxCan_Node_readData(node->node, rxBufferElement, message->dataLengthCode, data);

    /* write acknowledgement index incase of FIFO */
    if (message->readFromRxFifo0)
    {
        IfxCan_Node_setRxFifo0AcknowledgeIndex(node->node, bufferId);
    }
    else if (message->readFromRxFifo1)
    {
        IfxCan_Node_setRxFifo1AcknowledgeIndex(node->node, bufferId);
    }

    /* clear newdata flag after reading */
    IfxCan_Node_clearRxBufferNewDataFlag(node->node, bufferId);
}


IfxCan_Status IfxCan_Can_sendMessage(IfxCan_Can_Node *node, IfxCan_Message *message, uint32 *data)
{
    IfxCan_Status     status   = IfxCan_Status_ok;

    IfxCan_TxBufferId bufferId = IfxCan_TxBufferId_0;

    if (!message->storeInTxFifoQueue)
    {
        bufferId = (IfxCan_TxBufferId)message->bufferNumber;
    }
    else
    {
        bufferId = IfxCan_Node_getTxFifoQueuePutIndex(node->node);
    }

    if (IfxCan_Can_isTxBufferRequestPending(node, bufferId) == 1)
    {                       /* previous message was not transferred, e.g. due to busy bus, BUS-OFF or others */
        status = IfxCan_Status_notSentBusy;
    }
    else
    {
        /* get the Tx Bufer ELement address */
        Ifx_CAN_TXMSG *txBufferElement = IfxCan_Node_getTxBufferElementAddress(node->node, node->messageRAM.baseAddress, node->messageRAM.txBuffersStartAddress, bufferId);

        /*set message Id (ID and XTD) */
        IfxCan_Node_setMesssageId(node->node, txBufferElement, message->messageId, message->messageIdLength);

        /* set TX FIFO Event control (EFC) and Message Marker (MM) if Tx Event Fifo is chosen */
        if (message->txEventFifoControl == TRUE)
        {
            IfxCan_Node_setTxEventFifoControl(node->node, txBufferElement, message->txEventFifoControl);
            IfxCan_Node_setMessageMarker(node->node, txBufferElement, bufferId);
        }

        /* set Remote Transmit request if selected (RTR)*/
        IfxCan_Node_setRemoteTransmitRequest(node->node, txBufferElement, message->remoteTransmitRequest);

        /* set Error State Indicator if selected (ESI)*/
        if ((message->frameMode == IfxCan_FrameMode_fdLong) || (message->frameMode == IfxCan_FrameMode_fdLongAndFast))
        {
            IfxCan_Node_setErrorStateIndicator(node->node, txBufferElement, message->errorStateIndicator);
        }

        /* set data length code (DLC) */
        IfxCan_Node_setDataLengthCode(node->node, txBufferElement, message->dataLengthCode);

        /* write data (DBx) */
        IfxCan_Node_writeData(node->node, txBufferElement, message->dataLengthCode, data);

        /* set CAN frame mode request (FDF and BRS) */
        IfxCan_Node_setFrameModeRequest(node->node, txBufferElement, message->frameMode);

        /*set transmit request */
        IfxCan_Node_setTxBufferAddRequest(node->node, bufferId);
    }

    return status;
}


void IfxCan_Can_setExtendedFilter(IfxCan_Can_Node *node, IfxCan_Filter *filter)
{
    /* get the Extended filter element address */
    Ifx_CAN_EXTMSG *extendedFilterElement = IfxCan_Node_getExtendedFilterElementAddress(node->node, node->messageRAM.baseAddress, node->messageRAM.extendedFilterListStartAddress, filter->number);

    /* enable configuration change CCCR.CCE = 1, CCCR.INIT = 1 */
    IfxCan_Node_enableConfigurationChange(node->node);

    if (filter->elementConfiguration != IfxCan_FilterElementConfiguration_storeInRxBuffer)
    {
        IfxCan_Node_setExtendedFilterId2(node->node, extendedFilterElement, filter->id2);
    }
    else
    {
        IfxCan_Node_setExtendedFilterRxBufferOffset(node->node, extendedFilterElement, filter->rxBufferOffset);
    }

    IfxCan_Node_setExtendedFilterId1(node->node, extendedFilterElement, filter->id1);
    IfxCan_Node_setExtendedFilterConfiguration(node->node, extendedFilterElement, filter->elementConfiguration);
    IfxCan_Node_setExtendedFilterType(node->node, extendedFilterElement, filter->type);

    /* disable configuration change CCCR.CCE = 0, CCCR.INIT = 0 */
    IfxCan_Node_disableConfigurationChange(node->node);
}


void IfxCan_Can_setStandardFilter(IfxCan_Can_Node *node, IfxCan_Filter *filter)
{
    /* get the standard filter element address */
    Ifx_CAN_STDMSG *standardFilterElement = IfxCan_Node_getStandardFilterElementAddress(node->node, node->messageRAM.baseAddress, node->messageRAM.standardFilterListStartAddress, filter->number);

    /* enable configuration change CCCR.CCE = 1, CCCR.INIT = 1 */
    IfxCan_Node_enableConfigurationChange(node->node);

    if (filter->elementConfiguration != IfxCan_FilterElementConfiguration_storeInRxBuffer)
    {
        IfxCan_Node_setStandardFilterId2(node->node, standardFilterElement, filter->id2);
    }
    else
    {
        IfxCan_Node_setStandardFilterRxBufferOffset(node->node, standardFilterElement, filter->rxBufferOffset);
    }

    IfxCan_Node_setStandardFilterId1(node->node, standardFilterElement, filter->id1);
    IfxCan_Node_setStandardFilterConfiguration(node->node, standardFilterElement, filter->elementConfiguration);
    IfxCan_Node_setStandardFilterType(node->node, standardFilterElement, filter->type);

    /* disable configuration change CCCR.CCE = 0, CCCR.INIT = 0 */
    IfxCan_Node_disableConfigurationChange(node->node);
}
