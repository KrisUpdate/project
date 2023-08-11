/**
 * \file IfxGeth_Eth.c
 * \brief GETH ETH details
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

#include "IfxGeth_Eth.h"
#include "Configuration.h"
/******************************************************************************/
/*-----------------------Exported Variables/Constants-------------------------*/
/******************************************************************************/

IfxGeth_RxDescrList IfxGeth_Eth_rxDescrList[IFXGETH_NUM_MODULES][IFXGETH_NUM_RX_CHANNELS];

IfxGeth_TxDescrList IfxGeth_Eth_txDescrList[IFXGETH_NUM_MODULES][IFXGETH_NUM_TX_CHANNELS];

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

void IfxGeth_Eth_configureDMA(IfxGeth_Eth *geth, IfxGeth_Eth_DmaConfig *dmaConfig)
{
    uint32 txChannelIndex, rxChannelIndex, channelIndex;
    uint32 numOfChannels = (dmaConfig->numOfTxChannels >= dmaConfig->numOfRxChannels) ? dmaConfig->numOfTxChannels : dmaConfig->numOfRxChannels;

    /* DMA top level configuration */
    IfxGeth_dma_setAddressAlignedBeats(geth->gethSFR, dmaConfig->addressAlignedBeatsEnabled);
    IfxGeth_dma_setFixedBurst(geth->gethSFR, dmaConfig->fixedBurstEnabled);
    IfxGeth_dma_setMixedBurst(geth->gethSFR, dmaConfig->mixedBurstEnabled);

    /* TX Channels(s) configuration */
    IfxGeth_Eth_stopTransmitters(geth, dmaConfig->numOfTxChannels);

    for (txChannelIndex = 0; txChannelIndex < dmaConfig->numOfTxChannels; txChannelIndex++)
    {
        IfxGeth_dma_setTxMaxBurstLength(geth->gethSFR, dmaConfig->txChannel[txChannelIndex].channelId, dmaConfig->txChannel[txChannelIndex].maxBurstLength);
        IfxGeth_Eth_initTransmitDescriptors(geth, &dmaConfig->txChannel[txChannelIndex]);
    }

    /* RX Channels(s) configuration */
    for (rxChannelIndex = 0; rxChannelIndex < dmaConfig->numOfRxChannels; rxChannelIndex++)
    {
        IfxGeth_dma_setRxMaxBurstLength(geth->gethSFR, dmaConfig->rxChannel[rxChannelIndex].channelId, dmaConfig->rxChannel[rxChannelIndex].maxBurstLength);
        IfxGeth_Eth_initReceiveDescriptors(geth, &dmaConfig->rxChannel[rxChannelIndex]);
    }

    /* initialise the selected interrupts */
    for (channelIndex = 0; channelIndex < numOfChannels; channelIndex++)
    {
        IfxGeth_dma_clearAllInterruptFlags(geth->gethSFR, dmaConfig->txInterrupt[channelIndex].channelId);

        /* Transmit interrupts */
        if (dmaConfig->txInterrupt[channelIndex].priority > 0)
        {
            IfxGeth_dma_enableInterrupt(geth->gethSFR, dmaConfig->txInterrupt[channelIndex].channelId, IfxGeth_DmaInterruptFlag_transmitInterrupt);

            volatile Ifx_SRC_SRCR *srcSFR;
            srcSFR = IfxGeth_getSrcPointer(geth->gethSFR, (IfxGeth_ServiceRequest)((uint32)IfxGeth_ServiceRequest_2 + (uint32)dmaConfig->txInterrupt[channelIndex].channelId));
            IfxSrc_init(srcSFR, dmaConfig->txInterrupt[channelIndex].provider, dmaConfig->txInterrupt[channelIndex].priority);
            IfxSrc_enable(srcSFR);
        }

        /* Receive interrupts */
        if (dmaConfig->rxInterrupt[channelIndex].priority > 0)
        {
            IfxGeth_dma_enableInterrupt(geth->gethSFR, dmaConfig->rxInterrupt[channelIndex].channelId, IfxGeth_DmaInterruptFlag_receiveInterrupt);

            volatile Ifx_SRC_SRCR *srcSFR;
            srcSFR = IfxGeth_getSrcPointer(geth->gethSFR, (IfxGeth_ServiceRequest)((uint32)IfxGeth_ServiceRequest_6 + (uint32)dmaConfig->rxInterrupt[channelIndex].channelId));
            IfxSrc_init(srcSFR, dmaConfig->rxInterrupt[channelIndex].provider, dmaConfig->rxInterrupt[channelIndex].priority);
            IfxSrc_enable(srcSFR);
        }
    }
}


void IfxGeth_Eth_configureMTL(IfxGeth_Eth *geth, IfxGeth_Eth_MtlConfig *mtlConfig)
{
    uint32 txQueueIndex, rxQueueIndex, queueIndex;
    uint32 numOfQueues = (mtlConfig->numOfTxQueues >= mtlConfig->numOfRxQueues) ? mtlConfig->numOfTxQueues : mtlConfig->numOfRxQueues;

    /* clear all flags */
    for (queueIndex = 0; queueIndex < numOfQueues; queueIndex++)
    {
        IfxGeth_mtl_clearAllInterruptFlags(geth->gethSFR, (IfxGeth_MtlQueue)queueIndex);
    }

    /* TX Queue(s) configuration */
    if (mtlConfig->numOfTxQueues > 1)
    {
        IfxGeth_mtl_setTxSchedulingAlgorithm(geth->gethSFR, mtlConfig->txSchedulingAlgorithm);
    }

    for (txQueueIndex = 0; txQueueIndex < mtlConfig->numOfTxQueues; txQueueIndex++)
    {
        IfxGeth_mtl_setTxQueueSize(geth->gethSFR, (IfxGeth_TxMtlQueue)txQueueIndex, mtlConfig->txQueue[txQueueIndex].txQueueSize);
        IfxGeth_mtl_setTxStoreAndForward(geth->gethSFR, (IfxGeth_TxMtlQueue)txQueueIndex, mtlConfig->txQueue[txQueueIndex].storeAndForward);
        IfxGeth_mtl_enableTxQueue(geth->gethSFR, (IfxGeth_TxMtlQueue)txQueueIndex);
#if PROJECT == PROJECT_TRIBOARD
        IfxGeth_mtl_setTxETSControl(geth->gethSFR, (IfxGeth_TxMtlQueue)txQueueIndex);
        IfxGeth_mtl_setTxCreditParam(geth->gethSFR, (IfxGeth_TxMtlQueue)txQueueIndex);
#else

#endif
        if (mtlConfig->txQueue[txQueueIndex].txQueueUnderflowInterruptEnabled)
        {
            IfxGeth_mtl_enableInterrupt(geth->gethSFR, (IfxGeth_MtlQueue)txQueueIndex, IfxGeth_MtlInterruptFlag_txQueueUnderflow);
        }
    }

    /* RX Queue(s) configuration */
    if (mtlConfig->numOfRxQueues > 1)
    {
        IfxGeth_mtl_setRxArbitrationAlgorithm(geth->gethSFR, mtlConfig->rxArbitrationAlgorithm);
    }

    for (rxQueueIndex = 0; rxQueueIndex < mtlConfig->numOfRxQueues; rxQueueIndex++)
    {
        IfxGeth_mtl_setRxQueueSize(geth->gethSFR, (IfxGeth_RxMtlQueue)rxQueueIndex, mtlConfig->rxQueue[rxQueueIndex].rxQueueSize);
        IfxGeth_mtl_setRxStoreAndForward(geth->gethSFR, (IfxGeth_RxMtlQueue)rxQueueIndex, mtlConfig->rxQueue[rxQueueIndex].storeAndForward);
        IfxGeth_mtl_setRxForwardErrorPacket(geth->gethSFR, (IfxGeth_RxMtlQueue)rxQueueIndex, mtlConfig->rxQueue[rxQueueIndex].forwardErrorPacket);
        IfxGeth_mtl_setRxForwardUndersizedGoodPacket(geth->gethSFR, (IfxGeth_RxMtlQueue)rxQueueIndex, mtlConfig->rxQueue[rxQueueIndex].forwardUndersizedGoodPacket);

        if (mtlConfig->rxQueue[rxQueueIndex].daBasedDmaChannelEnabled != 0)
        {
            IfxGeth_mtl_setRxQueueForDaBasedDmaChannel(geth->gethSFR, (IfxGeth_RxMtlQueue)rxQueueIndex, TRUE);
        }
        else
        {
            IfxGeth_mtl_setRxQueueDmaChannelMapping(geth->gethSFR, (IfxGeth_RxMtlQueue)rxQueueIndex, mtlConfig->rxQueue[rxQueueIndex].rxDmaChannelMap);
        }

        IfxGeth_mtl_enableRxQueue(geth->gethSFR, (IfxGeth_RxMtlQueue)rxQueueIndex);

        if (mtlConfig->rxQueue[rxQueueIndex].rxQueueOverflowInterruptEnabled)
        {
            IfxGeth_mtl_enableInterrupt(geth->gethSFR, (IfxGeth_MtlQueue)rxQueueIndex, IfxGeth_MtlInterruptFlag_rxQueueOverflow);
        }
    }

    if (mtlConfig->interrupt.priority > 0)
    {
        volatile Ifx_SRC_SRCR *srcSFR;
        srcSFR = IfxGeth_getSrcPointer(geth->gethSFR, mtlConfig->interrupt.serviceRequest);
        IfxSrc_init(srcSFR, mtlConfig->interrupt.provider, mtlConfig->interrupt.priority);
        IfxSrc_enable(srcSFR);
    }
}


void IfxGeth_Eth_configureMacCore(IfxGeth_Eth *geth, IfxGeth_Eth_MacConfig *macConfig)
{
    /* MAC configuration */
    IfxGeth_mac_setDuplexMode(geth->gethSFR, macConfig->duplexMode);
    IfxGeth_mac_setLineSpeed(geth->gethSFR, macConfig->lineSpeed);
    IfxGeth_mac_setPreambleLength(geth->gethSFR, IfxGeth_PreambleLength_7Bytes);

    IfxGeth_mac_setMaxPacketSize(geth->gethSFR, macConfig->maxPacketSize);

    IfxGeth_mac_setCrcStripping(geth->gethSFR, FALSE, FALSE);
    IfxGeth_mac_setCrcChecking(geth->gethSFR, FALSE);

    IfxGeth_mac_setLoopbackMode(geth->gethSFR, macConfig->loopbackMode);

    /* packet Filter Configuration */
    IfxGeth_mac_setPromiscuousMode(geth->gethSFR, FALSE);
    IfxGeth_mac_setAllMulticastPassing(geth->gethSFR, TRUE);

    /* set MAC Address */
    IfxGeth_mac_setMacAddress(geth->gethSFR, macConfig->macAddress);
}


void IfxGeth_Eth_freeReceiveBuffer(IfxGeth_Eth *geth, IfxGeth_RxDmaChannel channelId)
{
    volatile IfxGeth_RxDescr *descr = IfxGeth_Eth_getActualRxDescriptor(geth, channelId);

    IfxGeth_RxDescr3          rdes3;
    rdes3.U        = 0;
    rdes3.R.BUF1V  = 1; /* buffer 1 valid */
    rdes3.R.BUF2V  = 0; /* buffer 2 not valid */
    rdes3.R.IOC    = 1; /* interrupt enabled */
    rdes3.R.OWN    = 1; /* owned by DMA */
    descr->RDES3.U = rdes3.U;
    IfxGeth_Eth_shuffleRxDescriptor(geth, channelId);
}


void *IfxGeth_Eth_getReceiveBuffer(IfxGeth_Eth *geth, IfxGeth_RxDmaChannel channelId)
{
    void                     *result = 0;
    volatile IfxGeth_RxDescr *descr;

    if (IfxGeth_Eth_isRxDataAvailable(geth, channelId))
    {
        geth->rxChannel[channelId].rxCount++;
        descr  = IfxGeth_Eth_getActualRxDescriptor(geth, channelId);
        result = (void *)(descr->RDES0.U);
    }

    IfxGeth_Eth_wakeupReceiver(geth, channelId);

    return result;
}


void *IfxGeth_Eth_getTransmitBuffer(IfxGeth_Eth *geth, IfxGeth_TxDmaChannel channelId)
{
    void                     *buffer = NULL_PTR;
    volatile IfxGeth_TxDescr *descr  = IfxGeth_Eth_getActualTxDescriptor(geth, channelId);

    // check descriptor / buffer is free.
    if (descr->TDES3.R.OWN == 0)
    {
        buffer = ((void *)descr->TDES0.U);
    }

    return buffer;
}


void IfxGeth_Eth_initModule(IfxGeth_Eth *geth, IfxGeth_Eth_Config *config)
{
    Ifx_GETH *gethSFR = config->gethSFR;
    geth->gethSFR = gethSFR;

#if 1//nvidia 5160 patch
    /* Clean up GPCTL */
    gethSFR->GPCTL.U = 0x0;

    gethSFR->SKEWCTL.B.RXCFG = 0;
    gethSFR->SKEWCTL.B.TXCFG = 0;
#endif

    /* Enable Module */
    IfxGeth_enableModule(gethSFR);

    /* Set up the pins */
    if (config->phyInterfaceMode == IfxGeth_PhyInterfaceMode_rmii)
    {
        if (config->pins.rmiiPins != NULL_PTR)
        {
            IfxGeth_Eth_setupRmiiOutputPins(geth, config->pins.rmiiPins);
            IfxGeth_Eth_setupRmiiInputPins(geth, config->pins.rmiiPins);
        }
    }
    else if (config->phyInterfaceMode == IfxGeth_PhyInterfaceMode_rgmii)
    {
        if (config->pins.rgmiiPins != NULL_PTR)
        {
            IfxGeth_Eth_setupRgmiiOutputPins(geth, config->pins.rgmiiPins);
            IfxGeth_Eth_setupRgmiiInputPins(geth, config->pins.rgmiiPins);
        }
    }
    else
    {
        if (config->pins.miiPins != NULL_PTR)
        {
            IfxGeth_Eth_setupMiiOutputPins(geth, config->pins.miiPins);
            IfxGeth_Eth_setupMiiInputPins(geth, config->pins.miiPins);
        }
    }

    /* reset the Module */
    IfxGeth_resetModule(gethSFR);
    /* select the Phy Interface Mode */
    IfxGeth_setPhyInterfaceMode(gethSFR, config->phyInterfaceMode);
    IfxGeth_dma_applySoftwareReset(gethSFR);

    /* wait until reset is finished or timeout. */
    {
        uint32 timeout = 0;

        while ((IfxGeth_dma_isSoftwareResetDone(gethSFR) == 0) && (timeout < IFXGETH_MAX_TIMEOUT_VALUE))
        {
            timeout++;
        }
    }

    /* Configure MAC Core */
    IfxGeth_Eth_configureMacCore(geth, &config->mac);

    /* Configure MTL */
    IfxGeth_Eth_configureMTL(geth, &config->mtl);

    /* Configure DMA */
    IfxGeth_Eth_configureDMA(geth, &config->dma);
}


void IfxGeth_Eth_initModuleConfig(IfxGeth_Eth_Config *config, Ifx_GETH *gethSFR)
{
    IfxGeth_Index            gethIndex;
    gethIndex = IfxGeth_getIndex(gethSFR);
    const IfxGeth_Eth_Config defaultConfig = {
        .gethSFR          = NULL_PTR,
        .phyInterfaceMode = IfxGeth_PhyInterfaceMode_rmii,
        .pins             = {
            .rmiiPins  = NULL_PTR,
            .rgmiiPins = NULL_PTR,
            .miiPins   = NULL_PTR,
        },
        .mac                                             = {
            .duplexMode    = IfxGeth_DuplexMode_fullDuplex,
            .lineSpeed     = IfxGeth_LineSpeed_100Mbps,
            .loopbackMode  = IfxGeth_LoopbackMode_disable,
            .maxPacketSize = 1518,
            .macAddress    = {0},
        },
        .mtl                                             = {
            .numOfTxQueues                               = 1,
            .txSchedulingAlgorithm                       = IfxGeth_TxSchedulingAlgorithm_wrr,
            .numOfRxQueues                               = 1,
            .rxArbitrationAlgorithm                      = IfxGeth_RxArbitrationAlgorithm_sp,

            .txQueue[0].storeAndForward                  = FALSE,
            .txQueue[0].txQueueSize                      = IfxGeth_QueueSize_256Bytes,
            .txQueue[0].txQueueUnderflowInterruptEnabled = FALSE,

            .txQueue[1].storeAndForward                  = FALSE,
            .txQueue[1].txQueueSize                      = IfxGeth_QueueSize_256Bytes,
            .txQueue[1].txQueueUnderflowInterruptEnabled = FALSE,

            .txQueue[2].storeAndForward                  = FALSE,
            .txQueue[2].txQueueSize                      = IfxGeth_QueueSize_256Bytes,
            .txQueue[2].txQueueUnderflowInterruptEnabled = FALSE,

            .txQueue[3].storeAndForward                  = FALSE,
            .txQueue[3].txQueueSize                      = IfxGeth_QueueSize_256Bytes,
            .txQueue[3].txQueueUnderflowInterruptEnabled = FALSE,

            .rxQueue[0].storeAndForward                  = FALSE,
            .rxQueue[0].rxQueueSize                      = IfxGeth_QueueSize_256Bytes,
            .rxQueue[0].forwardErrorPacket               = FALSE,
            .rxQueue[0].forwardUndersizedGoodPacket      = FALSE,
            .rxQueue[0].daBasedDmaChannelEnabled         = FALSE,
            .rxQueue[0].rxDmaChannelMap                  = IfxGeth_RxDmaChannel_0,
            .rxQueue[0].rxQueueOverflowInterruptEnabled  = FALSE,

            .rxQueue[1].storeAndForward                  = FALSE,
            .rxQueue[1].rxQueueSize                      = IfxGeth_QueueSize_256Bytes,
            .rxQueue[1].forwardErrorPacket               = FALSE,
            .rxQueue[1].forwardUndersizedGoodPacket      = FALSE,
            .rxQueue[1].daBasedDmaChannelEnabled         = FALSE,
            .rxQueue[1].rxDmaChannelMap                  = IfxGeth_RxDmaChannel_1,
            .rxQueue[1].rxQueueOverflowInterruptEnabled  = FALSE,

            .rxQueue[2].storeAndForward                  = FALSE,
            .rxQueue[2].rxQueueSize                      = IfxGeth_QueueSize_256Bytes,
            .rxQueue[2].forwardErrorPacket               = FALSE,
            .rxQueue[2].forwardUndersizedGoodPacket      = FALSE,
            .rxQueue[2].daBasedDmaChannelEnabled         = FALSE,
            .rxQueue[2].rxDmaChannelMap                  = IfxGeth_RxDmaChannel_2,
            .rxQueue[2].rxQueueOverflowInterruptEnabled  = FALSE,

            .rxQueue[3].storeAndForward                  = FALSE,
            .rxQueue[3].rxQueueSize                      = IfxGeth_QueueSize_256Bytes,
            .rxQueue[3].forwardErrorPacket               = FALSE,
            .rxQueue[3].forwardUndersizedGoodPacket      = FALSE,
            .rxQueue[3].daBasedDmaChannelEnabled         = FALSE,
            .rxQueue[3].rxDmaChannelMap                  = IfxGeth_RxDmaChannel_3,
            .rxQueue[3].rxQueueOverflowInterruptEnabled  = FALSE,

            .interrupt                                   = {
                .serviceRequest = IfxGeth_ServiceRequest_1,
                .priority       = 0,
                .provider       = IfxSrc_Tos_cpu0
            }
        },
        .dma                                             = {
            .numOfTxChannels                    = 1,
            .numOfRxChannels                    = 1,
            .addressAlignedBeatsEnabled         = FALSE,
            .fixedBurstEnabled                  = FALSE,
            .mixedBurstEnabled                  = FALSE,

            .txChannel[0].channelId             = IfxGeth_TxDmaChannel_0,
            .txChannel[0].maxBurstLength        = IfxGeth_DmaBurstLength_0,
            .txChannel[0].txDescrList           = &IfxGeth_Eth_txDescrList[gethIndex][0],
            .txChannel[0].txBuffer1StartAddress = NULL_PTR,
            .txChannel[0].txBuffer1Size         = 256,

            .txChannel[1].channelId             = IfxGeth_TxDmaChannel_1,
            .txChannel[1].maxBurstLength        = IfxGeth_DmaBurstLength_0,
            .txChannel[1].txDescrList           = &IfxGeth_Eth_txDescrList[gethIndex][1],
            .txChannel[1].txBuffer1StartAddress = NULL_PTR,
            .txChannel[1].txBuffer1Size         = 256,

            .txChannel[2].channelId             = IfxGeth_TxDmaChannel_2,
            .txChannel[2].maxBurstLength        = IfxGeth_DmaBurstLength_0,
            .txChannel[2].txDescrList           = &IfxGeth_Eth_txDescrList[gethIndex][2],
            .txChannel[2].txBuffer1StartAddress = NULL_PTR,
            .txChannel[2].txBuffer1Size         = 256,

            .txChannel[3].channelId             = IfxGeth_TxDmaChannel_3,
            .txChannel[3].maxBurstLength        = IfxGeth_DmaBurstLength_0,
            .txChannel[3].txDescrList           = &IfxGeth_Eth_txDescrList[gethIndex][3],
            .txChannel[3].txBuffer1StartAddress = NULL_PTR,
            .txChannel[3].txBuffer1Size         = 256,

            .rxChannel[0].channelId             = IfxGeth_RxDmaChannel_0,
            .rxChannel[0].maxBurstLength        = IfxGeth_DmaBurstLength_0,
            .rxChannel[0].rxDescrList           = &IfxGeth_Eth_rxDescrList[gethIndex][0],
            .rxChannel[0].rxBuffer1StartAddress = NULL_PTR,
            .rxChannel[0].rxBuffer1Size         = 256,

            .rxChannel[1].channelId             = IfxGeth_RxDmaChannel_1,
            .rxChannel[1].maxBurstLength        = IfxGeth_DmaBurstLength_0,
            .rxChannel[1].rxDescrList           = &IfxGeth_Eth_rxDescrList[gethIndex][1],
            .rxChannel[1].rxBuffer1StartAddress = NULL_PTR,
            .rxChannel[1].rxBuffer1Size         = 256,

            .rxChannel[2].channelId             = IfxGeth_RxDmaChannel_2,
            .rxChannel[2].maxBurstLength        = IfxGeth_DmaBurstLength_0,
            .rxChannel[2].rxDescrList           = &IfxGeth_Eth_rxDescrList[gethIndex][2],
            .rxChannel[2].rxBuffer1StartAddress = NULL_PTR,
            .rxChannel[2].rxBuffer1Size         = 256,

            .rxChannel[3].channelId             = IfxGeth_RxDmaChannel_3,
            .rxChannel[3].maxBurstLength        = IfxGeth_DmaBurstLength_0,
            .rxChannel[3].rxDescrList           = &IfxGeth_Eth_rxDescrList[gethIndex][3],
            .rxChannel[3].rxBuffer1StartAddress = NULL_PTR,
            .rxChannel[3].rxBuffer1Size         = 256,

            .txInterrupt[0].channelId           = IfxGeth_DmaChannel_0,
            .txInterrupt[0].priority            = 0,
            .txInterrupt[0].provider            = IfxSrc_Tos_cpu0,

            .txInterrupt[1].channelId           = IfxGeth_DmaChannel_1,
            .txInterrupt[1].priority            = 0,
            .txInterrupt[1].provider            = IfxSrc_Tos_cpu0,

            .txInterrupt[2].channelId           = IfxGeth_DmaChannel_2,
            .txInterrupt[2].priority            = 0,
            .txInterrupt[2].provider            = IfxSrc_Tos_cpu0,

            .txInterrupt[3].channelId           = IfxGeth_DmaChannel_3,
            .txInterrupt[3].priority            = 0,
            .txInterrupt[3].provider            = IfxSrc_Tos_cpu0,

            .rxInterrupt[0].channelId           = IfxGeth_DmaChannel_0,
            .rxInterrupt[0].priority            = 0,
            .rxInterrupt[0].provider            = IfxSrc_Tos_cpu0,

            .rxInterrupt[1].channelId           = IfxGeth_DmaChannel_1,
            .rxInterrupt[1].priority            = 0,
            .rxInterrupt[1].provider            = IfxSrc_Tos_cpu0,

            .rxInterrupt[2].channelId           = IfxGeth_DmaChannel_2,
            .rxInterrupt[2].priority            = 0,
            .rxInterrupt[2].provider            = IfxSrc_Tos_cpu0,

            .rxInterrupt[3].channelId           = IfxGeth_DmaChannel_3,
            .rxInterrupt[3].priority            = 0,
            .rxInterrupt[3].provider            = IfxSrc_Tos_cpu0,
        }
    };

    /* Default Configuration */
    *config = defaultConfig;

    /* take over module pointer */
    config->gethSFR = gethSFR;
}


void IfxGeth_Eth_initReceiveDescriptors(IfxGeth_Eth *geth, IfxGeth_Eth_RxChannelConfig *config)
{
    int                       i;
    uint32                    buffer1StartAddress = (uint32)config->rxBuffer1StartAddress;

    IfxGeth_RxDmaChannel      channelId           = config->channelId;
    geth->rxChannel[channelId].channelId   = channelId;
    geth->rxChannel[channelId].rxDescrList = config->rxDescrList;

    volatile IfxGeth_RxDescr *descr = IfxGeth_Eth_getBaseRxDescriptor(geth, channelId);

    geth->rxChannel[channelId].rxDescrPtr = descr;
    geth->rxChannel[channelId].curDescIdx = 0;

    IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, (config->rxBuffer1Size) % 4 == 0);

    /* Initialize descriptors in ring mode */
    for (i = 0; i < IFXGETH_MAX_RX_DESCRIPTORS; i++)
    {
        descr->RDES0.U       = (uint32)(config->rxBuffer1Size * i) + buffer1StartAddress;
        descr->RDES2.U       = 0; /* buffer2 not used */

        descr->RDES3.R.BUF1V = 1; /* buffer 1 valid */
        descr->RDES3.R.BUF2V = 0; /* buffer 2 not valid */
        descr->RDES3.R.IOC   = 1; /* interrupt enabled */
        descr->RDES3.R.OWN   = 1; /* owned by DMA */

        descr                = &descr[1];
    }

    /* rest the current pointer to base pointer in the handle */
    geth->rxChannel[channelId].rxDescrPtr = IfxGeth_Eth_getBaseRxDescriptor(geth, channelId);

    /* set the buffer size */
    IfxGeth_dma_setRxBufferSize(geth->gethSFR, channelId, config->rxBuffer1Size);

    IfxGeth_dma_setRxDescriptorListAddress(geth->gethSFR, channelId, (uint32)IfxGeth_Eth_getBaseRxDescriptor(geth, channelId));
    IfxGeth_dma_setRxDescriptorTailPointer(geth->gethSFR, channelId, (uint32)descr);
    IfxGeth_dma_setRxDescriptorRingLength(geth->gethSFR, channelId, (IFXGETH_MAX_RX_DESCRIPTORS - 1));
}


void IfxGeth_Eth_initTransmitDescriptors(IfxGeth_Eth *geth, IfxGeth_Eth_TxChannelConfig *config)
{
    int                  i;
    uint32               buffer1StartAddress = (uint32)config->txBuffer1StartAddress;

    IfxGeth_TxDmaChannel channelId           = config->channelId;
    geth->txChannel[channelId].channelId   = channelId;
    geth->txChannel[channelId].txDescrList = config->txDescrList;
    geth->txChannel[channelId].txBuf1Size  = (uint16)config->txBuffer1Size;

    volatile IfxGeth_TxDescr *descr = IfxGeth_Eth_getBaseTxDescriptor(geth, channelId);

    geth->txChannel[channelId].txDescrPtr = descr;
    geth->txChannel[channelId].curDescIdx = 0;

    IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, (config->txBuffer1Size) % 4 == 0);

    /* Initialize descriptors in ring mode */
    for (i = 0; i < IFXGETH_MAX_TX_DESCRIPTORS; i++)
    {
        descr->TDES0.U           = (uint32)(config->txBuffer1Size * i) + buffer1StartAddress;
        descr->TDES1.U           = 0; /* buffer2 not used */

        descr->TDES2.R.B1L       = config->txBuffer1Size;
        descr->TDES2.R.VTIR      = 0; /* do not use VLAN tag */
        descr->TDES2.R.B2L       = 0; /* buffer2 not used */
        descr->TDES2.R.TTSE_TMWD = 0; /* timestamp not used */
        descr->TDES2.R.IOC       = 0; /* interrupt disabled */

        /* TDES3 will be configured while trasmitting each packet */

        descr = &descr[1];
    }

    /* rest the current pointer to base pointer in the handle */
    geth->txChannel[channelId].txDescrPtr = IfxGeth_Eth_getBaseTxDescriptor(geth, channelId);

    IfxGeth_dma_setTxDescriptorListAddress(geth->gethSFR, channelId, (uint32)IfxGeth_Eth_getBaseTxDescriptor(geth, channelId));
    IfxGeth_dma_setTxDescriptorRingLength(geth->gethSFR, channelId, (IFXGETH_MAX_TX_DESCRIPTORS - 1));
}


void IfxGeth_Eth_sendFrame(IfxGeth_Eth *geth, IfxGeth_Eth_FrameConfig *config)
{
    IfxGeth_Eth_sendTransmitBuffer(geth, config->packetLength, config->channelId);
}


void IfxGeth_Eth_sendTransmitBuffer(IfxGeth_Eth *geth, uint32 packetLength, IfxGeth_TxDmaChannel channelId)
{
    uint32                    i;
    volatile IfxGeth_TxDescr *firstDescr       = IfxGeth_Eth_getActualTxDescriptor(geth, channelId);
    volatile IfxGeth_TxDescr *descr            = firstDescr;
    volatile IfxGeth_TxDescr *nextDescr        = &firstDescr[1];
    uint32                    bufferLength     = geth->txChannel[channelId].txBuf1Size; /* get the configured buffer length */
    /* calculate the number of descriptors needed for the frame based on buffer length */
    uint32                    numOfDescriptors = packetLength / bufferLength;

    if (packetLength % bufferLength)
    {
        numOfDescriptors += 1;
    }

    /* configure the first descriptor */
    firstDescr->TDES3.R.FL_TPL  = packetLength; /* total length of the packet */
    firstDescr->TDES3.R.TSE     = 0;            /* TCP Segmentation Disable */
    firstDescr->TDES3.R.CIC_TPL = 3;
    firstDescr->TDES3.R.SAIC    = 0;            /* Source Address insertion disabled */
    firstDescr->TDES3.R.CPC     = 0;            /* CRC and PAD insertion enabled */

    /* configure every other descriptor including first descriptor for the frame transmission */
    for (i = 0; i < numOfDescriptors; i++)
    {
        if (i == (numOfDescriptors - 1))
        {
            descr->TDES3.R.LD  = 1;                                              /* last descriptor of the frame */
            descr->TDES2.R.IOC = 1;                                              /* last descriptor of the frame set IOC */
            descr->TDES3.R.FD  = 0;
            descr->TDES2.R.B1L = packetLength;
        }
        else
        {
            descr->TDES3.R.LD  = 0;
            descr->TDES3.R.FD  = 0;
            descr->TDES2.R.IOC = 0;                                          /* Clear the IOC bits for intermeditate buffers */
            descr->TDES2.R.B1L = geth->txChannel[channelId].txBuf1Size;
            packetLength      -= bufferLength;
        }

        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, descr->TDES3.R.OWN != 1U);           /* Assert if buffers are not available for transfer */
        descr->TDES3.R.OWN = 1U;                                                 /* release to DMA */
        IfxGeth_Eth_shuffleTxDescriptor(geth, channelId);
        descr              = IfxGeth_Eth_getActualTxDescriptor(geth, channelId); /* update the descr pointer */
        nextDescr          = descr;
    }

    firstDescr->TDES3.R.FD                = 1;          /* first descriptor of the frame */
    geth->txChannel[channelId].txDescrPtr = firstDescr; /* point to first descriptor to initiate the transfer */
    IfxGeth_dma_setTxDescriptorTailPointer(geth->gethSFR, channelId, (uint32)nextDescr);
    IfxGeth_Eth_wakeupTransmitter(geth, channelId);     /* initialte the transfer */
    geth->txChannel[channelId].txDescrPtr = nextDescr;  /* update the handle pointer to next descriptor */

    geth->txChannel[channelId].txCount++;
}


void IfxGeth_Eth_setupMiiInputPins(IfxGeth_Eth *geth, const IfxGeth_Eth_MiiPins *miiPins)
{
    IfxPort_InputMode   mode       = IfxPort_InputMode_noPullDevice;
    IfxPort_PadDriver   speedGrade = IfxPort_PadDriver_cmosAutomotiveSpeed1;

    IfxGeth_Crs_In     *crs        = miiPins->crs;
    IfxGeth_Col_In     *col        = miiPins->col;
    IfxGeth_Txclk_In   *txClk      = miiPins->txClk;
    IfxGeth_Rxclk_In   *rxClk      = miiPins->rxClk;
    IfxGeth_Rxdv_In    *rxDv       = miiPins->rxDv;
    IfxGeth_Rxer_In    *rxEr       = miiPins->rxEr;
    IfxGeth_Rxd_In     *rxd0       = miiPins->rxd0;
    IfxGeth_Rxd_In     *rxd1       = miiPins->rxd1;
    IfxGeth_Rxd_In     *rxd2       = miiPins->rxd2;
    IfxGeth_Rxd_In     *rxd3       = miiPins->rxd3;
    IfxGeth_Mdio_InOut *mdio       = miiPins->mdio;

    geth->gethSFR->GPCTL.B.ALTI0  = mdio->inSelect;
    geth->gethSFR->GPCTL.B.ALTI1  = rxClk->select;
    geth->gethSFR->GPCTL.B.ALTI2  = crs->select;
    geth->gethSFR->GPCTL.B.ALTI3  = col->select;
    geth->gethSFR->GPCTL.B.ALTI4  = rxDv->select;
    geth->gethSFR->GPCTL.B.ALTI5  = rxEr->select;
    geth->gethSFR->GPCTL.B.ALTI6  = rxd0->select;
    geth->gethSFR->GPCTL.B.ALTI7  = rxd1->select;
    geth->gethSFR->GPCTL.B.ALTI8  = rxd2->select;
    geth->gethSFR->GPCTL.B.ALTI9  = rxd3->select;
    geth->gethSFR->GPCTL.B.ALTI10 = txClk->select;

    IfxPort_setPinControllerSelection(crs->pin.port, crs->pin.pinIndex);
    IfxPort_setPinControllerSelection(col->pin.port, col->pin.pinIndex);
    IfxPort_setPinControllerSelection(txClk->pin.port, txClk->pin.pinIndex);
    IfxPort_setPinControllerSelection(rxClk->pin.port, rxClk->pin.pinIndex);
    IfxPort_setPinControllerSelection(rxDv->pin.port, rxDv->pin.pinIndex);
    IfxPort_setPinControllerSelection(rxEr->pin.port, rxEr->pin.pinIndex);
    IfxPort_setPinControllerSelection(rxd0->pin.port, rxd0->pin.pinIndex);
    IfxPort_setPinControllerSelection(rxd1->pin.port, rxd1->pin.pinIndex);
    IfxPort_setPinControllerSelection(rxd2->pin.port, rxd2->pin.pinIndex);
    IfxPort_setPinControllerSelection(rxd3->pin.port, rxd3->pin.pinIndex);

    IfxPort_setPinModeInput(crs->pin.port, crs->pin.pinIndex, mode);
    IfxPort_setPinModeInput(col->pin.port, col->pin.pinIndex, mode);
    IfxPort_setPinModeInput(txClk->pin.port, txClk->pin.pinIndex, mode);
    IfxPort_setPinModeInput(rxClk->pin.port, rxClk->pin.pinIndex, mode);
    IfxPort_setPinModeInput(rxDv->pin.port, rxDv->pin.pinIndex, mode);
    IfxPort_setPinModeInput(rxEr->pin.port, rxEr->pin.pinIndex, mode);
    IfxPort_setPinModeInput(rxd0->pin.port, rxd0->pin.pinIndex, mode);
    IfxPort_setPinModeInput(rxd1->pin.port, rxd1->pin.pinIndex, mode);
    IfxPort_setPinModeInput(rxd2->pin.port, rxd2->pin.pinIndex, mode);
    IfxPort_setPinModeInput(rxd3->pin.port, rxd3->pin.pinIndex, mode);

    IfxPort_setPinPadDriver(crs->pin.port, crs->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(col->pin.port, col->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(txClk->pin.port, txClk->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(rxClk->pin.port, rxClk->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(rxDv->pin.port, rxDv->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(rxEr->pin.port, rxEr->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(rxd0->pin.port, rxd0->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(rxd1->pin.port, rxd1->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(rxd2->pin.port, rxd2->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(rxd3->pin.port, rxd3->pin.pinIndex, speedGrade);
}


void IfxGeth_Eth_setupMiiOutputPins(IfxGeth_Eth *geth, const IfxGeth_Eth_MiiPins *miiPins)
{
    IfxPort_OutputMode mode       = IfxPort_OutputMode_pushPull;
    IfxPort_PadDriver  speedGrade = IfxPort_PadDriver_cmosAutomotiveSpeed1;

    IfxGeth_Txen_Out  *txEn       = miiPins->txEn;
    IfxGeth_Txer_Out  *txEr       = miiPins->txEr;
    IfxGeth_Txd_Out   *txd0       = miiPins->txd0;
    IfxGeth_Txd_Out   *txd1       = miiPins->txd1;
    IfxGeth_Txd_Out   *txd2       = miiPins->txd2;
    IfxGeth_Txd_Out   *txd3       = miiPins->txd3;
    IfxGeth_Mdc_Out   *mdc        = miiPins->mdc;

    IfxPort_setPinControllerSelection(mdc->pin.port, mdc->pin.pinIndex);
    IfxPort_setPinControllerSelection(txEn->pin.port, txEn->pin.pinIndex);
    IfxPort_setPinControllerSelection(txEr->pin.port, txEr->pin.pinIndex);
    IfxPort_setPinControllerSelection(txd0->pin.port, txd0->pin.pinIndex);
    IfxPort_setPinControllerSelection(txd1->pin.port, txd1->pin.pinIndex);
    IfxPort_setPinControllerSelection(txd2->pin.port, txd2->pin.pinIndex);
    IfxPort_setPinControllerSelection(txd3->pin.port, txd3->pin.pinIndex);

    IfxPort_setPinModeOutput(mdc->pin.port, mdc->pin.pinIndex, mode, mdc->select);
    IfxPort_setPinModeOutput(txEn->pin.port, txEn->pin.pinIndex, mode, txEn->select);
    IfxPort_setPinModeOutput(txEr->pin.port, txEr->pin.pinIndex, mode, txEr->select);
    IfxPort_setPinModeOutput(txd0->pin.port, txd0->pin.pinIndex, mode, txd0->select);
    IfxPort_setPinModeOutput(txd1->pin.port, txd1->pin.pinIndex, mode, txd1->select);
    IfxPort_setPinModeOutput(txd2->pin.port, txd2->pin.pinIndex, mode, txd2->select);
    IfxPort_setPinModeOutput(txd3->pin.port, txd3->pin.pinIndex, mode, txd3->select);

    IfxPort_setPinPadDriver(mdc->pin.port, mdc->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(txEn->pin.port, txEn->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(txEr->pin.port, txEr->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(txd0->pin.port, txd0->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(txd1->pin.port, txd1->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(txd2->pin.port, txd2->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(txd3->pin.port, txd3->pin.pinIndex, speedGrade);
}


void IfxGeth_Eth_setupRgmiiInputPins(IfxGeth_Eth *geth, const IfxGeth_Eth_RgmiiPins *rgmiiPins)
{
    IfxPort_InputMode   mode       = IfxPort_InputMode_noPullDevice;
    IfxPort_PadDriver   speedGrade = IfxPort_PadDriver_cmosAutomotiveSpeed4;

    IfxGeth_Rxclk_In   *rxClk      = rgmiiPins->rxClk;
    IfxGeth_Rxctl_In   *rxCtl      = rgmiiPins->rxCtl;
    IfxGeth_Rxd_In     *rxd0       = rgmiiPins->rxd0;
    IfxGeth_Rxd_In     *rxd1       = rgmiiPins->rxd1;
    IfxGeth_Rxd_In     *rxd2       = rgmiiPins->rxd2;
    IfxGeth_Rxd_In     *rxd3       = rgmiiPins->rxd3;
    IfxGeth_Grefclk_In *grefClk    = rgmiiPins->grefClk;
    IfxGeth_Mdio_InOut *mdio       = rgmiiPins->mdio;

    geth->gethSFR->GPCTL.B.ALTI0 = mdio->inSelect;
    geth->gethSFR->GPCTL.B.ALTI1 = rxClk->select;
    geth->gethSFR->GPCTL.B.ALTI4 = rxCtl->select;
    geth->gethSFR->GPCTL.B.ALTI6 = rxd0->select;
    geth->gethSFR->GPCTL.B.ALTI7 = rxd1->select;
    geth->gethSFR->GPCTL.B.ALTI8 = rxd2->select;
    geth->gethSFR->GPCTL.B.ALTI9 = rxd3->select;

    IfxPort_setPinControllerSelection(rxClk->pin.port, rxClk->pin.pinIndex);
    IfxPort_setPinControllerSelection(rxCtl->pin.port, rxCtl->pin.pinIndex);
    IfxPort_setPinControllerSelection(rxd0->pin.port, rxd0->pin.pinIndex);
    IfxPort_setPinControllerSelection(rxd1->pin.port, rxd1->pin.pinIndex);
    IfxPort_setPinControllerSelection(rxd2->pin.port, rxd2->pin.pinIndex);
    IfxPort_setPinControllerSelection(rxd3->pin.port, rxd3->pin.pinIndex);
    IfxPort_setPinControllerSelection(grefClk->pin.port, grefClk->pin.pinIndex);

    IfxPort_setPinModeInput(rxClk->pin.port, rxClk->pin.pinIndex, mode);
    IfxPort_setPinModeInput(rxCtl->pin.port, rxCtl->pin.pinIndex, mode);
    IfxPort_setPinModeInput(rxd0->pin.port, rxd0->pin.pinIndex, mode);
    IfxPort_setPinModeInput(rxd1->pin.port, rxd1->pin.pinIndex, mode);
    IfxPort_setPinModeInput(rxd2->pin.port, rxd2->pin.pinIndex, mode);
    IfxPort_setPinModeInput(rxd3->pin.port, rxd3->pin.pinIndex, mode);
    IfxPort_setPinModeInput(grefClk->pin.port, grefClk->pin.pinIndex, mode);

    IfxPort_setPinPadDriver(rxClk->pin.port, rxClk->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(rxCtl->pin.port, rxCtl->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(rxd0->pin.port, rxd0->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(rxd1->pin.port, rxd1->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(rxd2->pin.port, rxd2->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(rxd3->pin.port, rxd3->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(grefClk->pin.port, grefClk->pin.pinIndex, speedGrade);
}


void IfxGeth_Eth_setupRgmiiOutputPins(IfxGeth_Eth *geth, const IfxGeth_Eth_RgmiiPins *rgmiiPins)
{
    IfxPort_OutputMode  mode       = IfxPort_OutputMode_pushPull;
    IfxPort_PadDriver   speedGrade = IfxPort_PadDriver_cmosAutomotiveSpeed4;

    IfxGeth_Txclk_Out  *txClk      = rgmiiPins->txClk;
    IfxGeth_Txctl_Out  *txCtl      = rgmiiPins->txCtl;
    IfxGeth_Txd_Out    *txd0       = rgmiiPins->txd0;
    IfxGeth_Txd_Out    *txd1       = rgmiiPins->txd1;
    IfxGeth_Txd_Out    *txd2       = rgmiiPins->txd2;
    IfxGeth_Txd_Out    *txd3       = rgmiiPins->txd3;
    IfxGeth_Mdc_Out    *mdc        = rgmiiPins->mdc;
    IfxGeth_Mdio_InOut *mdio       = rgmiiPins->mdio;

    IfxPort_setPinControllerSelection(mdc->pin.port, mdc->pin.pinIndex);
    IfxPort_setPinControllerSelection(mdio->pin.port, mdio->pin.pinIndex);
    IfxPort_setPinControllerSelection(txClk->pin.port, txClk->pin.pinIndex);
    IfxPort_setPinControllerSelection(txCtl->pin.port, txCtl->pin.pinIndex);
    IfxPort_setPinControllerSelection(txd0->pin.port, txd0->pin.pinIndex);
    IfxPort_setPinControllerSelection(txd1->pin.port, txd1->pin.pinIndex);
    IfxPort_setPinControllerSelection(txd2->pin.port, txd2->pin.pinIndex);
    IfxPort_setPinControllerSelection(txd3->pin.port, txd3->pin.pinIndex);

    IfxPort_setPinModeOutput(mdc->pin.port, mdc->pin.pinIndex, mode, mdc->select);
    IfxPort_setPinModeOutput(txClk->pin.port, txClk->pin.pinIndex, mode, txClk->select);
    IfxPort_setPinModeOutput(txCtl->pin.port, txCtl->pin.pinIndex, mode, txCtl->select);
    IfxPort_setPinModeOutput(txd0->pin.port, txd0->pin.pinIndex, mode, txd0->select);
    IfxPort_setPinModeOutput(txd1->pin.port, txd1->pin.pinIndex, mode, txd1->select);
    IfxPort_setPinModeOutput(txd2->pin.port, txd2->pin.pinIndex, mode, txd2->select);
    IfxPort_setPinModeOutput(txd3->pin.port, txd3->pin.pinIndex, mode, txd3->select);

    /*  For MDIO, when P21.1 is used it should be configured as output */
    if ((mdio->pin.port == (&MODULE_P21)) && (mdio->pin.pinIndex == 1))
    {
        IfxPort_setPinModeOutput(mdio->pin.port, mdio->pin.pinIndex, mode, mdio->outSelect);
    }

    IfxPort_setPinPadDriver(mdc->pin.port, mdc->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(txClk->pin.port, txClk->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(txCtl->pin.port, txCtl->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(txd0->pin.port, txd0->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(txd1->pin.port, txd1->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(txd2->pin.port, txd2->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(txd3->pin.port, txd3->pin.pinIndex, speedGrade);
}


void IfxGeth_Eth_setupRmiiInputPins(IfxGeth_Eth *geth, const IfxGeth_Eth_RmiiPins *rmiiPins)
{
    IfxPort_InputMode   mode       = IfxPort_InputMode_noPullDevice;
    IfxPort_PadDriver   speedGrade = IfxPort_PadDriver_cmosAutomotiveSpeed1;

    IfxGeth_Crsdv_In   *crsDiv     = rmiiPins->crsDiv;
    IfxGeth_Refclk_In  *refClk     = rmiiPins->refClk;
    IfxGeth_Rxd_In     *rxd0       = rmiiPins->rxd0;
    IfxGeth_Rxd_In     *rxd1       = rmiiPins->rxd1;
    IfxGeth_Mdio_InOut *mdio       = rmiiPins->mdio;

    geth->gethSFR->GPCTL.B.ALTI0 = mdio->inSelect;
    geth->gethSFR->GPCTL.B.ALTI1 = refClk->select;
    geth->gethSFR->GPCTL.B.ALTI4 = crsDiv->select;
    geth->gethSFR->GPCTL.B.ALTI6 = rxd0->select;
    geth->gethSFR->GPCTL.B.ALTI7 = rxd1->select;

    IfxPort_setPinControllerSelection(crsDiv->pin.port, crsDiv->pin.pinIndex);
    IfxPort_setPinControllerSelection(refClk->pin.port, refClk->pin.pinIndex);
    IfxPort_setPinControllerSelection(rxd0->pin.port, rxd0->pin.pinIndex);
    IfxPort_setPinControllerSelection(rxd1->pin.port, rxd1->pin.pinIndex);

    IfxPort_setPinModeInput(crsDiv->pin.port, crsDiv->pin.pinIndex, mode);
    IfxPort_setPinModeInput(refClk->pin.port, refClk->pin.pinIndex, mode);
    IfxPort_setPinModeInput(rxd0->pin.port, rxd0->pin.pinIndex, mode);
    IfxPort_setPinModeInput(rxd1->pin.port, rxd1->pin.pinIndex, mode);

    IfxPort_setPinPadDriver(crsDiv->pin.port, crsDiv->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(refClk->pin.port, refClk->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(rxd0->pin.port, rxd0->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(rxd1->pin.port, rxd1->pin.pinIndex, speedGrade);
}


void IfxGeth_Eth_setupRmiiOutputPins(IfxGeth_Eth *geth, const IfxGeth_Eth_RmiiPins *rmiiPins)
{
    IfxPort_OutputMode  mode       = IfxPort_OutputMode_pushPull;
    IfxPort_PadDriver   speedGrade = IfxPort_PadDriver_cmosAutomotiveSpeed1;

    IfxGeth_Mdc_Out    *mdc        = rmiiPins->mdc;
    IfxGeth_Mdio_InOut *mdio       = rmiiPins->mdio;
    IfxGeth_Txen_Out   *txen       = rmiiPins->txEn;
    IfxGeth_Txd_Out    *txd0       = rmiiPins->txd0;
    IfxGeth_Txd_Out    *txd1       = rmiiPins->txd1;

    IfxPort_setPinControllerSelection(mdc->pin.port, mdc->pin.pinIndex);
    IfxPort_setPinControllerSelection(txen->pin.port, txen->pin.pinIndex);
    IfxPort_setPinControllerSelection(txd0->pin.port, txd0->pin.pinIndex);
    IfxPort_setPinControllerSelection(txd1->pin.port, txd1->pin.pinIndex);

    IfxPort_setPinModeOutput(mdc->pin.port, mdc->pin.pinIndex, mode, mdc->select);
    IfxPort_setPinModeOutput(txen->pin.port, txen->pin.pinIndex, mode, txen->select);
    IfxPort_setPinModeOutput(txd0->pin.port, txd0->pin.pinIndex, mode, txd0->select);
    IfxPort_setPinModeOutput(txd1->pin.port, txd1->pin.pinIndex, mode, txd1->select);

    /*  For MDIO, when P21.1 is used it should be configured as output */
    if ((mdio->pin.port == (&MODULE_P21)) && (mdio->pin.pinIndex == 1))
    {
        IfxPort_setPinModeOutput(mdio->pin.port, mdio->pin.pinIndex, mode, mdio->outSelect);
    }

    IfxPort_setPinPadDriver(mdc->pin.port, mdc->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(txen->pin.port, txen->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(txd0->pin.port, txd0->pin.pinIndex, speedGrade);
    IfxPort_setPinPadDriver(txd1->pin.port, txd1->pin.pinIndex, speedGrade);
}


void IfxGeth_Eth_shuffleRxDescriptor(IfxGeth_Eth *geth, IfxGeth_RxDmaChannel channelId)
{
    volatile IfxGeth_RxDescr *currentDescr = geth->rxChannel[channelId].rxDescrPtr;
    volatile IfxGeth_RxDescr *lastDescr    = &geth->rxChannel[channelId].rxDescrList->descr[IFXGETH_MAX_RX_DESCRIPTORS - 1];

    if (currentDescr == lastDescr)
    {
        /* wrap around the descriptors */
        geth->rxChannel[channelId].rxDescrPtr = IfxGeth_Eth_getBaseRxDescriptor(geth, channelId);
        geth->rxChannel[channelId].curDescIdx = 0;
    }
    else
    {
        /* point to the next descriptor */
        geth->rxChannel[channelId].rxDescrPtr = &geth->rxChannel[channelId].rxDescrPtr[1];
        geth->rxChannel[channelId].curDescIdx +=1;
    }
}


void IfxGeth_Eth_shuffleTxDescriptor(IfxGeth_Eth *geth, IfxGeth_TxDmaChannel channelId)
{
    volatile IfxGeth_TxDescr *currentDescr = geth->txChannel[channelId].txDescrPtr;
    volatile IfxGeth_TxDescr *lastDescr    = &geth->txChannel[channelId].txDescrList->descr[IFXGETH_MAX_TX_DESCRIPTORS - 1];

    if (currentDescr == lastDescr)
    {
        /* wrap around the descriptors */
        geth->txChannel[channelId].txDescrPtr = IfxGeth_Eth_getBaseTxDescriptor(geth, channelId);
        geth->txChannel[channelId].curDescIdx = 0;
    }
    else
    {
        /* point to the next descriptor */
        geth->txChannel[channelId].txDescrPtr = &geth->txChannel[channelId].txDescrPtr[1];
        geth->txChannel[channelId].curDescIdx +=1;
    }
}


void IfxGeth_Eth_startReceiver(IfxGeth_Eth *geth, IfxGeth_RxDmaChannel channelId)
{
    IfxGeth_mac_enableReceiver(geth->gethSFR);
    IfxGeth_dma_startReceiver(geth->gethSFR, channelId);
}


void IfxGeth_Eth_startReceivers(IfxGeth_Eth *geth, uint32 numOfChannels)
{
    uint32 i;
    IfxGeth_mac_enableReceiver(geth->gethSFR);

    for (i = 0; i < numOfChannels; i++)
    {
        IfxGeth_dma_startReceiver(geth->gethSFR, (IfxGeth_RxDmaChannel)i);
    }
}


void IfxGeth_Eth_startTransmitter(IfxGeth_Eth *geth, IfxGeth_TxDmaChannel channelId)
{
    IfxGeth_mac_enableTransmitter(geth->gethSFR);
    IfxGeth_dma_startTransmitter(geth->gethSFR, channelId);
}


void IfxGeth_Eth_startTransmitters(IfxGeth_Eth *geth, uint32 numOfChannels)
{
    uint32 i;
    IfxGeth_mac_enableTransmitter(geth->gethSFR);

    for (i = 0; i < numOfChannels; i++)
    {
        IfxGeth_dma_startTransmitter(geth->gethSFR, (IfxGeth_TxDmaChannel)i);
    }
}


void IfxGeth_Eth_stopTransmitters(IfxGeth_Eth *geth, uint32 numOfChannels)
{
    uint32 i;
    IfxGeth_mac_disableTransmitter(geth->gethSFR);

    for (i = 0; i < numOfChannels; i++)
    {
        IfxGeth_dma_stopTransmitter(geth->gethSFR, (IfxGeth_TxDmaChannel)i);
    }
}


void IfxGeth_Eth_wakeupReceiver(IfxGeth_Eth *geth, IfxGeth_RxDmaChannel channelId)
{
    /* check if receiver suspended */
    if (IfxGeth_dma_isInterruptFlagSet(geth->gethSFR, (IfxGeth_DmaChannel)channelId, IfxGeth_DmaInterruptFlag_receiveStopped))
    {
        /* check if receive buffer unavailable */
        if (IfxGeth_dma_isInterruptFlagSet(geth->gethSFR, (IfxGeth_DmaChannel)channelId, IfxGeth_DmaInterruptFlag_receiveBufferUnavailable))
        {
            IfxGeth_dma_clearInterruptFlag(geth->gethSFR, (IfxGeth_DmaChannel)channelId, IfxGeth_DmaInterruptFlag_receiveBufferUnavailable);
        }

        IfxGeth_Eth_startReceiver(geth, channelId);
    }
}


void IfxGeth_Eth_wakeupTransmitter(IfxGeth_Eth *geth, IfxGeth_TxDmaChannel channelId)
{
    /* check if transmitter suspended */
    if (IfxGeth_dma_isInterruptFlagSet(geth->gethSFR, (IfxGeth_DmaChannel)channelId, IfxGeth_DmaInterruptFlag_transmitStopped))
    {
        /* check if transmit buffer unavailable */
        if (IfxGeth_dma_isInterruptFlagSet(geth->gethSFR, (IfxGeth_DmaChannel)channelId, IfxGeth_DmaInterruptFlag_transmitBufferUnavailable))
        {
            IfxGeth_dma_clearInterruptFlag(geth->gethSFR, (IfxGeth_DmaChannel)channelId, IfxGeth_DmaInterruptFlag_transmitBufferUnavailable);
        }

        /* check MTL underflow flag */
        if (IfxGeth_mtl_isInterruptFlagSet(geth->gethSFR, (IfxGeth_MtlQueue)channelId, IfxGeth_MtlInterruptFlag_txQueueUnderflow))
        {
            IfxGeth_mtl_clearInterruptFlag(geth->gethSFR, (IfxGeth_MtlQueue)channelId, IfxGeth_MtlInterruptFlag_txQueueUnderflow);
        }

        IfxGeth_Eth_startTransmitter(geth, channelId);
    }
}


void IfxGeth_Eth_writeHeader(IfxGeth_Eth *geth, uint8 *txBuffer, uint8 *destinationAddress, uint8 *sourceAddress, uint32 payloadLength)
{
    uint32 i;

    /* Destination Address */
    for (i = 0; i < 6; i++)
    {
        *txBuffer++ = *destinationAddress++;
    }

    /* Source Address */
    for (i = 0; i < 6; i++)
    {
        *txBuffer++ = *sourceAddress++;
    }

    /* packet size */
    *txBuffer++ = (uint8)(payloadLength / 256);
    *txBuffer   = (uint8)(payloadLength % 256);
}
