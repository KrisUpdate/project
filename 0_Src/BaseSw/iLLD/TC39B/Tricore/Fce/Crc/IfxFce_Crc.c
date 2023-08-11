/**
 * \file IfxFce_Crc.c
 * \brief FCE CRC details
 *
 * \version iLLD_1_0_1_9_0
 * \copyright Copyright (c) 2017 Infineon Technologies AG. All rights reserved.
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

#include "IfxFce_Crc.h"

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

uint32 IfxFce_Crc_calculateCrc(IfxFce_Crc_Crc *fce, const uint32 *crcData, uint16 crcDataLength, uint32 crcStartValue)
{
    Ifx_FCE          *fceSFR     = fce->fce;
    IfxFce_CrcChannel crcChannel = fce->crcChannel;
    uint32            inputDataCounter;
    uint32            crcResultValue;
    uint32           *dataPtr = (uint32 *)crcData;

    /*set the Legth*/
    IfxFce_setChannelCrcLength(fce->fce, crcChannel, crcDataLength);

    /*set the expected CRC*/
    IfxFce_setExpectedCrc(fce->fce, crcChannel, fce->expectedCrc);

    /*Configure CRC register*/
    IfxFce_setCrcstartValue(fce->fce, crcChannel, crcStartValue);

    volatile Ifx_FCE_IN_IR *InputData = &fceSFR->IN[fce->crcChannel].IR;

    if (fce->useDma == TRUE)
    {
        IfxDma_ChannelIncrementStep dmaIncrementStep;
        IfxDma_ChannelMoveSize      dmaChannelsize;

        if ((fce->crcKernel == IfxFce_CrcKernel_0) || (fce->crcKernel == IfxFce_CrcKernel_1))
        {
            dmaIncrementStep = IfxDma_ChannelIncrementStep_1;
            dmaChannelsize   = IfxDma_ChannelMoveSize_32bit;
        }
        else if (fce->crcKernel == IfxFce_CrcKernel_2)
        {
            dmaIncrementStep = IfxDma_ChannelIncrementStep_2;
            dmaChannelsize   = IfxDma_ChannelMoveSize_16bit;
        }
        else
        {
            dmaIncrementStep = IfxDma_ChannelIncrementStep_4;
            dmaChannelsize   = IfxDma_ChannelMoveSize_8bit;
        }

        IfxDma_setChannelSourceAddress(fce->fceDmaChannel.dma, fce->fceDmaChannel.channelId, (void *)IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreIndex(), crcData));
        IfxDma_setChannelDestinationAddress(fce->fceDmaChannel.dma, fce->fceDmaChannel.channelId, (void *)&InputData->U);
        IfxDma_setChannelTransferCount(fce->fceDmaChannel.dma, fce->fceDmaChannel.channelId, crcDataLength);
        IfxDma_setChannelMoveSize(fce->fceDmaChannel.dma, fce->fceDmaChannel.channelId, dmaChannelsize);
        IfxDma_setChannelDestinationIncrementStep(fce->fceDmaChannel.dma, fce->fceDmaChannel.channelId, dmaIncrementStep,
            IfxDma_ChannelIncrementDirection_positive, IfxDma_ChannelIncrementCircular_4);

        IfxDma_Dma_startChannelTransaction(&fce->fceDmaChannel);

        while (IfxDma_Dma_isChannelTransactionPending(&fce->fceDmaChannel) == TRUE)
        {}
    }
    else
    {
        /* input in INIT register */
        for (inputDataCounter = 0; inputDataCounter < crcDataLength; ++inputDataCounter)
        {
            InputData->U = *(dataPtr++);
        }
    }

    crcResultValue = fceSFR->IN[fce->crcChannel].RES.U;

    return crcResultValue;
}


void IfxFce_Crc_clearErrorFlags(IfxFce_Crc_Crc *fce)
{
    IfxFce_clearCrcErrorFlags(fce->fce, fce->crcChannel);
}


void IfxFce_Crc_deInitModule(IfxFce_Crc_Crc *fce)
{
    IfxFce_resetModule(fce->fce);
}


Ifx_FCE_IN_STS IfxFce_Crc_getInterruptStatus(IfxFce_Crc_Crc *fce)
{
    return IfxFce_getCrcInterruptStatus(fce->fce, fce->crcChannel);
}


void IfxFce_Crc_initCrc(IfxFce_Crc_Crc *fceCrc, const IfxFce_Crc_CrcConfig *crcConfig)
{
    fceCrc->fce = crcConfig->fce;
    Ifx_FCE *fceSFR = crcConfig->fce;

    fceCrc->expectedCrc = crcConfig->expectedCrc;
    fceCrc->crcChannel  = crcConfig->crcChannel;
    fceCrc->crcKernel   = crcConfig->crcKernel;

    uint16         password = IfxScuWdt_getCpuWatchdogPassword();
    IfxScuWdt_clearCpuEndinit(password);

    Ifx_FCE_IN_CFG tempCFG;

    tempCFG.U                               = 0;
    tempCFG.B.CMI                           = crcConfig->enabledInterrupts.crcMismatch;
    tempCFG.B.CEI                           = crcConfig->enabledInterrupts.configError;
    tempCFG.B.LEI                           = crcConfig->enabledInterrupts.lengthError;
    tempCFG.B.BEI                           = crcConfig->enabledInterrupts.busError;
    tempCFG.B.CCE                           = crcConfig->crcCheckCompared;
    tempCFG.B.ALR                           = crcConfig->automaticLengthReload;
    tempCFG.B.REFIN                         = crcConfig->dataByteReflectionEnabled;
    tempCFG.B.REFOUT                        = crcConfig->crc32BitReflectionEnabled;
    tempCFG.B.XSEL                          = crcConfig->crcResultInverted;
    tempCFG.B.BYTESWAP                      = crcConfig->swapOrderOfBytes;
    tempCFG.B.KERNEL                        = crcConfig->crcKernel;

    fceSFR->IN[crcConfig->crcChannel].CFG.U = tempCFG.U;

    IfxScuWdt_setCpuEndinit(password);

    fceCrc->useDma = crcConfig->useDma;

    if (fceCrc->useDma == TRUE)
    {
        Ifx_DMA                 *dmaSFR = &MODULE_DMA;
        IfxDma_Dma               dma;
        IfxDma_Dma_createModuleHandle(&dma, dmaSFR);

        IfxDma_Dma_ChannelConfig dmaChannelCfg;
        IfxDma_Dma_initChannelConfig(&dmaChannelCfg, &dma);

        dmaChannelCfg.channelId                        = crcConfig->fceChannelId;
        dmaChannelCfg.requestMode                      = IfxDma_ChannelRequestMode_completeTransactionPerRequest;
        dmaChannelCfg.operationMode                    = IfxDma_ChannelOperationMode_continuous;
        dmaChannelCfg.destinationAddressCircularRange  = IfxDma_ChannelIncrementCircular_4,
        dmaChannelCfg.destinationCircularBufferEnabled = TRUE,
        IfxDma_Dma_initChannel(&fceCrc->fceDmaChannel, &dmaChannelCfg);
    }
}


void IfxFce_Crc_initCrcConfig(IfxFce_Crc_CrcConfig *crcConfig, IfxFce_Crc *fce)
{
    crcConfig->fce                           = fce->fce;
    crcConfig->crcKernel                     = IfxFce_CrcKernel_0;
    crcConfig->crcChannel                    = IfxFce_CrcChannel_0;
    crcConfig->crcCheckCompared              = TRUE;
    crcConfig->automaticLengthReload         = FALSE;
    crcConfig->dataByteReflectionEnabled     = TRUE;
    crcConfig->crc32BitReflectionEnabled     = TRUE;
    crcConfig->swapOrderOfBytes              = FALSE;
    crcConfig->crcResultInverted             = TRUE;
    crcConfig->enabledInterrupts.crcMismatch = FALSE; // enable if CRC is already known
    crcConfig->enabledInterrupts.configError = TRUE;
    crcConfig->enabledInterrupts.lengthError = TRUE;
    crcConfig->enabledInterrupts.busError    = TRUE;
    crcConfig->useDma                        = FALSE;
    crcConfig->fceChannelId                  = IfxDma_ChannelId_none;
}


void IfxFce_Crc_initModule(IfxFce_Crc *fce, const IfxFce_Crc_Config *config)
{
    fce->fce = config->fce;
    Ifx_FCE               *fceSFR = config->fce;

    IfxFce_enableModule(fceSFR);

    volatile Ifx_SRC_SRCR *src = IfxFce_getSrcPointer(fceSFR);
    IfxSrc_init(src, config->isrTypeOfService, config->isrPriority);
    IfxSrc_enable(src);
}


void IfxFce_Crc_initModuleConfig(IfxFce_Crc_Config *config, Ifx_FCE *fce)
{
    config->fce              = fce;
    config->isrPriority      = 0;
    config->isrTypeOfService = IfxSrc_Tos_cpu0;
}
