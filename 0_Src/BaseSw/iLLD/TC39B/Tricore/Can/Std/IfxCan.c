/**
 * \file IfxCan.c
 * \brief CAN  basic functionality
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

#include "IfxCan.h"
#include "IfxCan_bf.h"

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

void IfxCan_Node_clearRxBufferNewDataFlag(Ifx_CAN_N *node, IfxCan_RxBufferId rxBufferId)
{
    uint32 value;

    if (rxBufferId < IfxCan_RxBufferId_32)
    {
        value         = (1U << rxBufferId);
        node->NDAT1.U = value;
    }
    else
    {
        value         = (1U << (rxBufferId - 32));
        node->NDAT2.U = value;
    }
}


uint32 IfxCan_Node_getDataLengthFromCode(Ifx_CAN_N *node, IfxCan_DataLengthCode dataLengthCode)
{
    uint32 numBytes;

    if ((dataLengthCode <= IfxCan_DataLengthCode_64) && (dataLengthCode > IfxCan_DataLengthCode_24))
    {
        numBytes = ((uint32)dataLengthCode - 11) * 16;
    }
    else if ((dataLengthCode <= IfxCan_DataLengthCode_24) && (dataLengthCode > IfxCan_DataLengthCode_8))
    {
        numBytes = ((uint32)dataLengthCode - 6) * 4;
    }
    else
    {
        numBytes = (uint32)dataLengthCode;
    }

    return (numBytes + 3) / 4;
}


Ifx_CAN_EXTMSG *IfxCan_Node_getExtendedFilterElementAddress(Ifx_CAN_N *node, uint32 ramBaseAddress, uint16 extendedFilterListStartAddress, uint8 filterNumber)
{
    uint32 extendedFilterElementAddress, filterIndex;

    filterIndex                  = filterNumber * 8;
    extendedFilterElementAddress = ramBaseAddress + extendedFilterListStartAddress + filterIndex;

    return (Ifx_CAN_EXTMSG *)extendedFilterElementAddress;
}


IfxCan_FrameMode IfxCan_Node_getFrameMode(Ifx_CAN_N *node, Ifx_CAN_RXMSG *rxBufferElement)
{
    IfxCan_FrameMode frameMode;

    /* if CAN FD long frame is been selected */
    if (rxBufferElement->R1.B.FDF)
    {
        /* if bitrate switch is been set */
        if (rxBufferElement->R1.B.BRS)
        {
            frameMode = IfxCan_FrameMode_fdLongAndFast;
        }
        /* if bitrate switch is not been set */
        else
        {
            frameMode = IfxCan_FrameMode_fdLong;
        }
    }
    /* if CAN FD long frame is not been selected */
    else
    {
        frameMode = IfxCan_FrameMode_standard;
    }

    return frameMode;
}


IfxCan_FrameMode IfxCan_Node_getFrameModeFromTxEventFifo(Ifx_CAN_N *node, Ifx_CAN_TXEVENT *txEventFifoElement)
{
    IfxCan_FrameMode frameMode;

    /* if CAN FD long frame is been selected */
    if (txEventFifoElement->E1.B.FDF)
    {
        /* if bitrate switch is been set */
        if (txEventFifoElement->E1.B.BRS)
        {
            frameMode = IfxCan_FrameMode_fdLongAndFast;
        }
        /* if bitrate switch is not been set */
        else
        {
            frameMode = IfxCan_FrameMode_fdLong;
        }
    }
    /* if CAN FD long frame is not been selected */
    else
    {
        frameMode = IfxCan_FrameMode_standard;
    }

    return frameMode;
}


uint8 IfxCan_Node_getRxBufferDataFieldSize(Ifx_CAN_N *node)
{
    IfxCan_DataFieldSize sizeCode = (IfxCan_DataFieldSize)node->RX.ESC.B.RBDS;

    uint8                size;

    if (sizeCode < IfxCan_DataFieldSize_32)
    {
        size = ((uint8)sizeCode + 2) * 4;
    }
    else
    {
        size = ((uint8)sizeCode - 3) * 16;
    }

    return size;
}


Ifx_CAN_RXMSG *IfxCan_Node_getRxBufferElementAddress(Ifx_CAN_N *node, uint32 ramBaseAddress, uint16 rxBuffersStartAddress, IfxCan_RxBufferId rxBufferNumber)
{
    uint16 rxBufferIndex, rxBufferSize, numOfDataBytes, numOfconfigBytes = 8;

    numOfDataBytes = IfxCan_Node_getRxBufferDataFieldSize(node);
    rxBufferSize   = numOfconfigBytes + numOfDataBytes;
    rxBufferIndex  = rxBufferSize * ((uint16)rxBufferNumber);
    uint32 rxBufferElementAddress = ramBaseAddress + rxBuffersStartAddress + rxBufferIndex;

    return (Ifx_CAN_RXMSG *)rxBufferElementAddress;
}


uint8 IfxCan_Node_getRxFifo0DataFieldSize(Ifx_CAN_N *node)
{
    IfxCan_DataFieldSize sizeCode = (IfxCan_DataFieldSize)node->RX.ESC.B.F0DS;

    uint8                size;

    if (sizeCode < IfxCan_DataFieldSize_32)
    {
        size = ((uint8)sizeCode + 2) * 4;
    }
    else
    {
        size = ((uint8)sizeCode - 3) * 16;
    }

    return size;
}


Ifx_CAN_RXMSG *IfxCan_Node_getRxFifo0ElementAddress(Ifx_CAN_N *node, uint32 ramBaseAddress, uint16 rxFifo0StartAddress, IfxCan_RxBufferId rxBufferNumber)
{
    uint16 rxFifoElementIndex, rxFifoElementSize, numOfDataBytes, numOfconfigBytes = 8;

    numOfDataBytes     = IfxCan_Node_getRxFifo0DataFieldSize(node);
    rxFifoElementSize  = numOfconfigBytes + numOfDataBytes;
    rxFifoElementIndex = rxFifoElementSize * ((uint16)rxBufferNumber);
    uint32 rxFifoElementAddress = ramBaseAddress + rxFifo0StartAddress + rxFifoElementIndex;

    return (Ifx_CAN_RXMSG *)rxFifoElementAddress;
}


uint8 IfxCan_Node_getRxFifo1DataFieldSize(Ifx_CAN_N *node)
{
    IfxCan_DataFieldSize sizeCode = (IfxCan_DataFieldSize)node->RX.ESC.B.F1DS;

    uint8                size;

    if (sizeCode < IfxCan_DataFieldSize_32)
    {
        size = ((uint8)sizeCode + 2) * 4;
    }
    else
    {
        size = ((uint8)sizeCode - 3) * 16;
    }

    return size;
}


Ifx_CAN_RXMSG *IfxCan_Node_getRxFifo1ElementAddress(Ifx_CAN_N *node, uint32 ramBaseAddress, uint16 rxFifo1StartAddress, IfxCan_RxBufferId rxBufferNumber)
{
    uint16 rxFifoElementIndex, rxFifoElementSize, numOfDataBytes, numOfconfigBytes = 8;

    numOfDataBytes     = IfxCan_Node_getRxFifo1DataFieldSize(node);
    rxFifoElementSize  = numOfconfigBytes + numOfDataBytes;
    rxFifoElementIndex = rxFifoElementSize * ((uint16)rxBufferNumber);
    uint32 rxFifoElementAddress = ramBaseAddress + rxFifo1StartAddress + rxFifoElementIndex;

    return (Ifx_CAN_RXMSG *)rxFifoElementAddress;
}


Ifx_CAN_STDMSG *IfxCan_Node_getStandardFilterElementAddress(Ifx_CAN_N *node, uint32 ramBaseAddress, uint16 standardFilterListStartAddress, uint8 filterNumber)
{
    uint32 standardFilterElementAddress, filterIndex;

    filterIndex                  = filterNumber * 4;
    standardFilterElementAddress = ramBaseAddress + standardFilterListStartAddress + filterIndex;

    return (Ifx_CAN_STDMSG *)standardFilterElementAddress;
}


uint8 IfxCan_Node_getTxBufferDataFieldSize(Ifx_CAN_N *node)
{
    IfxCan_DataFieldSize sizeCode = (IfxCan_DataFieldSize)node->TX.ESC.B.TBDS;

    uint8                size;

    if (sizeCode < IfxCan_DataFieldSize_32)
    {
        size = ((uint8)sizeCode + 2) * 4;
    }
    else
    {
        size = ((uint8)sizeCode - 3) * 16;
    }

    return size;
}


Ifx_CAN_TXMSG *IfxCan_Node_getTxBufferElementAddress(Ifx_CAN_N *node, uint32 ramBaseAddress, uint16 txBuffersStartAddress, IfxCan_TxBufferId txBufferNumber)
{
    uint16 txBufferIndex, txBufferSize, numOfDataBytes, numOfConfigBytes = 8;

    numOfDataBytes = IfxCan_Node_getTxBufferDataFieldSize(node);
    txBufferSize   = numOfConfigBytes + numOfDataBytes;
    txBufferIndex  = txBufferSize * ((uint16)txBufferNumber);

    uint32 txBufferElementAddress = ramBaseAddress + txBuffersStartAddress + txBufferIndex;
    return (Ifx_CAN_TXMSG *)txBufferElementAddress;
}


Ifx_CAN_TXEVENT *IfxCan_Node_getTxEventFifoElementAddress(Ifx_CAN_N *node, uint32 ramBaseAddress, uint16 txEventFifoStartAddress, uint8 txEventFifoNumber)
{
    uint32 txEventFifoElementAddress, txEventFifoIndex;

    txEventFifoIndex          = txEventFifoNumber * 8;
    txEventFifoElementAddress = ramBaseAddress + txEventFifoStartAddress + txEventFifoIndex;

    return (Ifx_CAN_TXEVENT *)txEventFifoElementAddress;
}


boolean IfxCan_Node_initRxPin(Ifx_CAN_N *node, IfxCan_Rxd_In *rxd, IfxPort_InputMode mode, IfxPort_PadDriver padDriver)
{
    IfxPort_setPinModeInput(rxd->pin.port, rxd->pin.pinIndex, mode);
    IfxPort_setPinPadDriver(rxd->pin.port, rxd->pin.pinIndex, padDriver);
    node->NPCR.B.RXSEL = rxd->select;

    return TRUE;
}


boolean IfxCan_Node_initTxPin(Ifx_CAN_N *node, IfxCan_Txd_Out *txd, IfxPort_OutputMode mode, IfxPort_PadDriver padDriver)
{
    IfxPort_setPinModeOutput(txd->pin.port, txd->pin.pinIndex, mode, txd->select);
    IfxPort_setPinPadDriver(txd->pin.port, txd->pin.pinIndex, padDriver);

    return TRUE;
}


boolean IfxCan_Node_isRxBufferNewDataUpdated(Ifx_CAN_N *node, IfxCan_RxBufferId rxBufferId)
{
    uint32  mask;
    boolean tempVar;

    if (rxBufferId < IfxCan_RxBufferId_32)
    {
        mask    = (1U << rxBufferId);
        tempVar = (boolean)(node->NDAT1.U & mask);
        return tempVar;
    }
    else
    {
        mask    = (1U << (rxBufferId - 32));
        tempVar = (boolean)(node->NDAT2.U & mask);
        return tempVar;
    }
}


void IfxCan_Node_readData(Ifx_CAN_N *node, Ifx_CAN_RXMSG *rxBufferElement, IfxCan_DataLengthCode dataLengthCode, uint32 *data)
{
    uint32  i;
    uint32 *destinationAddress = (uint32 *)rxBufferElement + 2;
    /* get number of data bytes from data length code (DLC) */
    uint32  length             = IfxCan_Node_getDataLengthFromCode(node, dataLengthCode);

    /* read data from the data section of Rx Buffer element  */
    for (i = 0; i < length; i++)
    {
        data[i] = destinationAddress[i];
    }
}


void IfxCan_Node_setBitTiming(Ifx_CAN_N *node, float32 moduleFreq, uint32 baudrate, uint16 samplePoint, uint16 syncJumpWidth)
{
    sint32  maxBRP   = IFX_CAN_N_NBTP_NBRP_MSK + 1;
    sint32  minBRP   = 1;
    sint32  maxTSEG1 = IFX_CAN_N_NBTP_NTSEG1_MSK + 1;
    sint32  minTSEG1 = 3;
    sint32  maxTSEG2 = IFX_CAN_N_NBTP_NTSEG2_MSK + 1;
    sint32  minTSEG2 = 2;
    sint32  maxTBAUD = 0;
    sint32  minTBAUD = 8;

    sint32  tempBRP, tempSJW, tempTSEG1, tempTBAUD;
    sint32  bestBRP   = 1, bestSJW = 1, bestTBAUD = 8, bestTSEG1 = 3, bestTSEG2 = 2;
    float32 bestError = 0;

    /*
     * Bit timing & sampling
     * Tq = (BRP+1)/Fcan if DIV8 = 0
     * Tq = 8*(BRP+1)/Fcan if DIV8 = 1
     * TSync = 1.Tq
     * TSeg1 = (TSEG1+1)*Tq                >= 3Tq
     * TSeg2 = (TSEG2+1)*Tq                >= 2Tq
     * Bit Time = TSync + TSeg1 + TSeg2    >= 8Tq
     *
     * Resynchronization:
     *
     * Tsjw = (SJW + 1)*Tq
     * TSeg1 >= Tsjw + Tprop
     * TSeg2 >= Tsjw
     */

    /* search for best baudrate */
    bestError = (float32)baudrate;
    maxTBAUD  = maxTSEG1 + maxTSEG2 + 1;

    for (tempBRP = 1; tempBRP <= maxBRP; tempBRP++)
    {
        float32 Fquanta = moduleFreq / tempBRP;
        tempTBAUD = Fquanta / baudrate;

        if (tempTBAUD == 0)
        {
            break; /* to avoid division by 0 */
        }

        float32 tempBaudrate = Fquanta / tempTBAUD;
        float32 error        = __absf(tempBaudrate - baudrate);

        if (tempTBAUD < minTBAUD)
        {
            break;  /* below the minimum allowed limits, break is required otherwise TSEG1 and TSEG2 may result in negitive values */
        }

        if ((tempTBAUD <= maxTBAUD) && (bestError >= error))
        {
            bestBRP   = tempBRP;
            bestTBAUD = tempTBAUD;
            bestError = error;

            if ((tempTBAUD <= 20) && (error < 0.1))
            {
                break;      /* optimal condition */
            }
        }
    }

    if ((bestBRP == 0) && (tempBRP == (maxBRP + 1)))
    {
        bestBRP   = maxBRP;
        bestTBAUD = maxTBAUD;

        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, FALSE);
    }

    if ((bestBRP == 0) && (tempTBAUD < minTBAUD))
    {
        bestBRP   = minBRP;
        bestTBAUD = minTBAUD;

        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, FALSE);
    }

    /* search for best sample point */
    bestError = samplePoint * 0.25; /* 25% tolerance in sample point as max error */

    if (bestTBAUD < maxTSEG1)
    {
        maxTSEG1 = bestTBAUD;
    }

    bestTSEG1 = maxTSEG1;

    for (tempTSEG1 = maxTSEG1; tempTSEG1 >= minTSEG1; tempTSEG1--)
    {
        sint32 tempSamplePoint = ((tempTSEG1 + 1) * 10000) / bestTBAUD;
        sint32 error           = __abs(tempSamplePoint - samplePoint);

        if (bestError > error)
        {
            bestTSEG1 = tempTSEG1;
            bestError = (float)error;
        }

        if (tempSamplePoint < samplePoint)
        {
            /*least possible error */
            break;  /* least possible error has already occured */
        }
    }

    bestTSEG2 = bestTBAUD - bestTSEG1 - 1;

    if (bestTSEG2 > maxTSEG2)
    {
        bestTSEG2 = maxTSEG2;

        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, FALSE);
    }

    if (bestTSEG2 < minTSEG2)
    {
        bestTSEG2 = minTSEG2;

        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, FALSE);
    }

    /* search for best SJW */
    bestError = 10000;

    for (tempSJW = 1; tempSJW <= bestTSEG2; tempSJW++)
    {
        sint32 tempSyncJumpWidth = (tempSJW * 10000) / bestTBAUD;
        sint32 error             = __abs(tempSyncJumpWidth - syncJumpWidth);

        if (bestError > error)
        {
            bestSJW   = tempSJW;
            bestError = (float)error;
        }
    }

    {
        node->NBTP.B.NBRP   = bestBRP - 1;
        node->NBTP.B.NSJW   = bestSJW - 1;
        node->NBTP.B.NTSEG1 = bestTSEG1 - 1;
        node->NBTP.B.NTSEG2 = bestTSEG2 - 1;
    }
}


void IfxCan_Node_setBitTimingValues(Ifx_CAN_N *node, uint16 syncJumpWidth, uint8 timeSegment2, uint8 timeSegment1, uint16 prescaler)
{
    Ifx_CAN_N_NBTP nbtp;
    nbtp.U        = node->NBTP.U;
    nbtp.B.NSJW   = syncJumpWidth;
    nbtp.B.NTSEG1 = timeSegment1;
    nbtp.B.NTSEG2 = timeSegment2;
    nbtp.B.NBRP   = prescaler;

    node->NBTP.U  = nbtp.U;
}


void IfxCan_Node_setFastBitTiming(Ifx_CAN_N *node, float32 moduleFreq, uint32 baudrate, uint16 samplePoint, uint16 syncJumpWidth)
{
    sint32  maxBRP   = IFX_CAN_N_DBTP_DBRP_MSK + 1;
    sint32  minBRP   = 1;
    sint32  maxTSEG1 = IFX_CAN_N_DBTP_DTSEG1_MSK + 1;
    sint32  minTSEG1 = 3;
    sint32  maxTSEG2 = IFX_CAN_N_DBTP_DTSEG2_MSK + 1;
    sint32  minTSEG2 = 2;
    sint32  maxTBAUD = 0;
    sint32  minTBAUD = 8;

    sint32  tempBRP, tempSJW, tempTSEG1, tempTBAUD;
    sint32  bestBRP   = 1, bestSJW = 1, bestTBAUD = 8, bestTSEG1 = 3, bestTSEG2 = 2;
    float32 bestError = 0;

    /*
     * Bit timing & sampling
     * Tq = (BRP+1)/Fcan if DIV8 = 0
     * Tq = 8*(BRP+1)/Fcan if DIV8 = 1
     * TSync = 1.Tq
     * TSeg1 = (TSEG1+1)*Tq                >= 3Tq
     * TSeg2 = (TSEG2+1)*Tq                >= 2Tq
     * Bit Time = TSync + TSeg1 + TSeg2    >= 8Tq
     *
     * Resynchronization:
     *
     * Tsjw = (SJW + 1)*Tq
     * TSeg1 >= Tsjw + Tprop
     * TSeg2 >= Tsjw
     */

    /* search for best baudrate */
    bestError = (float32)baudrate;
    maxTBAUD  = maxTSEG1 + maxTSEG2 + 1;

    for (tempBRP = 1; tempBRP <= maxBRP; tempBRP++)
    {
        float32 Fquanta = moduleFreq / tempBRP;
        tempTBAUD = Fquanta / baudrate;

        if (tempTBAUD == 0)
        {
            break; /* to avoid division by 0 */
        }

        float32 tempBaudrate = Fquanta / tempTBAUD;
        float32 error        = __absf(tempBaudrate - baudrate);

        if (tempTBAUD < minTBAUD)
        {
            break;  /* below the minimum allowed limits, break is required otherwise TSEG1 and TSEG2 may result in negitive values */
        }

        if ((tempTBAUD <= maxTBAUD) && (bestError >= error))
        {
            bestBRP   = tempBRP;
            bestTBAUD = tempTBAUD;
            bestError = error;

            if ((tempTBAUD <= 20) && (error < 0.1))
            {
                break;      /* optimal condition */
            }
        }
    }

    if ((bestBRP == 0) && (tempBRP == (maxBRP + 1)))
    {
        bestBRP   = maxBRP;
        bestTBAUD = maxTBAUD;

        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, FALSE);
    }

    if ((bestBRP == 0) && (tempTBAUD < minTBAUD))
    {
        bestBRP   = minBRP;
        bestTBAUD = minTBAUD;

        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, FALSE);
    }

    /* search for best sample point */
    bestError = samplePoint * 0.25; /* 25% tolerance in sample point as max error */

    if (bestTBAUD < maxTSEG1)
    {
        maxTSEG1 = bestTBAUD;
    }

    bestTSEG1 = maxTSEG1;

    for (tempTSEG1 = maxTSEG1; tempTSEG1 >= minTSEG1; tempTSEG1--)
    {
        sint32 tempSamplePoint = ((tempTSEG1 + 1) * 10000) / bestTBAUD;
        sint32 error           = __abs(tempSamplePoint - samplePoint);

        if (bestError > error)
        {
            bestTSEG1 = tempTSEG1;
            bestError = (float)error;
        }

        if (tempSamplePoint < samplePoint)
        {
            break;  /* least possible error has already occured */
        }
    }

    bestTSEG2 = bestTBAUD - bestTSEG1 - 1;

    if (bestTSEG2 > maxTSEG2)
    {
        bestTSEG2 = maxTSEG2;

        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, FALSE);
    }

    if (bestTSEG2 < minTSEG2)
    {
        bestTSEG2 = minTSEG2;

        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, FALSE);
    }

    /* search for best SJW */
    bestError = 10000;

    for (tempSJW = 1; tempSJW <= bestTSEG2; tempSJW++)
    {
        sint32 tempSyncJumpWidth = (tempSJW * 10000) / bestTBAUD;
        sint32 error             = __abs(tempSyncJumpWidth - syncJumpWidth);

        if (bestError > error)
        {
            bestSJW   = tempSJW;
            bestError = (float)error;
        }
    }

    {
        node->DBTP.B.DBRP   = bestBRP - 1;
        node->DBTP.B.DSJW   = bestSJW - 1;
        node->DBTP.B.DTSEG1 = bestTSEG1 - 1;
        node->DBTP.B.DTSEG2 = bestTSEG2 - 1;
    }
}


void IfxCan_Node_setFastBitTimingValues(Ifx_CAN_N *node, uint16 syncJumpWidth, uint8 timeSegment2, uint8 timeSegment1, uint16 prescaler)
{
    Ifx_CAN_N_DBTP dbtp;
    dbtp.U        = node->DBTP.U;
    dbtp.B.DSJW   = syncJumpWidth;
    dbtp.B.DTSEG1 = timeSegment1;
    dbtp.B.DTSEG2 = timeSegment2;
    dbtp.B.DBRP   = prescaler;

    node->DBTP.U  = dbtp.U;
}


void IfxCan_Node_setFrameMode(Ifx_CAN_N *node, IfxCan_FrameMode frameMode)
{
    switch (frameMode)
    {
    case IfxCan_FrameMode_standard:
        node->CCCR.B.FDOE = 0;
        node->CCCR.B.BRSE = 0;
        break;
    case IfxCan_FrameMode_fdLong:
        node->CCCR.B.FDOE = 1;
        node->CCCR.B.BRSE = 0;
        break;
    case IfxCan_FrameMode_fdLongAndFast:
        node->CCCR.B.FDOE = 1;
        node->CCCR.B.BRSE = 1;
        break;
    }
}


void IfxCan_Node_setFrameModeRequest(Ifx_CAN_N *node, Ifx_CAN_TXMSG *txBufferElement, IfxCan_FrameMode frameMode)
{
    switch (frameMode)
    {
    case IfxCan_FrameMode_standard:
        txBufferElement->T1.B.FDF = 0;
        txBufferElement->T1.B.BRS = 0;
        break;
    case IfxCan_FrameMode_fdLong:
        txBufferElement->T1.B.FDF = 1;
        txBufferElement->T1.B.BRS = 0;
        break;
    case IfxCan_FrameMode_fdLongAndFast:
        txBufferElement->T1.B.FDF = 1;
        txBufferElement->T1.B.BRS = 1;
        break;
    }
}


void IfxCan_Node_setGroupInterruptLine(Ifx_CAN_N *node, IfxCan_InterruptGroup interruptGroup, IfxCan_InterruptLine interruptLine)
{
    if (interruptGroup <= IfxCan_InterruptGroup_loi)
    {
        node->GRINT1.U |= (interruptLine) << (interruptGroup * 4);
    }
    else
    {
        node->GRINT2.U |= (interruptLine) << ((interruptGroup % 8) * 4);
    }
}


void IfxCan_Node_setInterruptLine(Ifx_CAN_N *node, IfxCan_Interrupt interrupt, IfxCan_InterruptLine interruptLine)
{
    switch (interrupt)
    {
    case IfxCan_Interrupt_rxFifo0NewMessage:
        node->GRINT2.B.RXF0N = interruptLine;
        break;
    case IfxCan_Interrupt_rxFifo0WatermarkReached:
    case IfxCan_Interrupt_rxFifo1WatermarkReached:
    case IfxCan_Interrupt_txEventFifoWatermarkReached:
        node->GRINT1.B.WATI = interruptLine;
        break;
    case IfxCan_Interrupt_rxFifo0Full:
        node->GRINT2.B.RXF0F = interruptLine;
        break;
    case IfxCan_Interrupt_rxFifo0MessageLost:
    case IfxCan_Interrupt_rxFifo1MessageLost:
    case IfxCan_Interrupt_txEventFifoEventLost:
    case IfxCan_Interrupt_timestampWraparound:
    case IfxCan_Interrupt_errorPassive:
    case IfxCan_Interrupt_warningStatus:
        node->GRINT1.B.ALRT = interruptLine;
        break;
    case IfxCan_Interrupt_rxFifo1NewMessage:
        node->GRINT2.B.RXF1N = interruptLine;
        break;
    case IfxCan_Interrupt_rxFifo1Full:
        node->GRINT2.B.RXF1F = interruptLine;
        break;
    case IfxCan_Interrupt_highPriorityMessage:
        node->GRINT1.B.HPE = interruptLine;
        break;
    case IfxCan_Interrupt_transmissionCompleted:
    case IfxCan_Interrupt_transmissionCancellationFinished:
        node->GRINT2.B.TRACO = interruptLine;
        break;
    case IfxCan_Interrupt_txFifoEmpty:
        node->GRINT2.B.TRAQ = interruptLine;
        break;
    case IfxCan_Interrupt_txEventFifoNewEntry:
    case IfxCan_Interrupt_txEventFifoFull:
        node->GRINT1.B.TEFIFO = interruptLine;
        break;
    case IfxCan_Interrupt_messageRAMAccessFailure:
    case IfxCan_Interrupt_bitErrorCorrected:
    case IfxCan_Interrupt_bitErrorUncorrected:
    case IfxCan_Interrupt_watchdog:
        node->GRINT1.B.MOER = interruptLine;
        break;
    case IfxCan_Interrupt_timeoutOccurred:
        node->GRINT2.B.RETI = interruptLine;
        break;
    case IfxCan_Interrupt_messageStoredToDedicatedRxBuffer:
        node->GRINT2.B.REINT = interruptLine;
        break;
    case IfxCan_Interrupt_errorLoggingOverflow:
        node->GRINT1.B.SAFE = interruptLine;
        break;
    case IfxCan_Interrupt_busOffStatus:
        node->GRINT1.B.BOFF = interruptLine;
        break;
    case IfxCan_Interrupt_protocolErrorArbitration:
    case IfxCan_Interrupt_protocolErrorData:
    case IfxCan_Interrupt_AccessToReservedAddress:
        node->GRINT1.B.LOI = interruptLine;
        break;
    default:
        break;
    }
}


void IfxCan_Node_writeData(Ifx_CAN_N *node, Ifx_CAN_TXMSG *txBufferElement, IfxCan_DataLengthCode dataLengthCode, uint32 *data)
{
    uint32  i;

    uint32 *destinationAddress = (uint32 *)txBufferElement + 2;
    /* get number of data bytes from data length code (DLC) */
    uint32  length             = IfxCan_Node_getDataLengthFromCode(node, dataLengthCode);

    /* write data into the data section of Tx Buffer element  */
    for (i = 0; i < length; i++)
    {
        destinationAddress[i] = data[i];
    }
}


void IfxCan_disableModule(Ifx_CAN *can)
{
    uint16 passwd = IfxScuWdt_getCpuWatchdogPassword();
    IfxScuWdt_clearCpuEndinit(passwd);

    /*Disable module */
    can->CLC.B.DISR = 1U;

    /*Wait until module is disabled*/
    while (IfxCan_isModuleEnabled(can) == TRUE)
    {}

    IfxScuWdt_setCpuEndinit(passwd);
}


void IfxCan_enableModule(Ifx_CAN *can)
{
    uint16 passwd = IfxScuWdt_getCpuWatchdogPassword();
    IfxScuWdt_clearCpuEndinit(passwd);

    /*Enable module, disregard Sleep Mode request */
    can->CLC.B.DISR = 0U;

    /*Wait until module is enabled*/
    while (IfxCan_isModuleEnabled(can) == FALSE)
    {}

    IfxScuWdt_setCpuEndinit(passwd);
}


Ifx_CAN *IfxCan_getAddress(IfxCan_Index can)
{
    Ifx_CAN *module;

    if (can < IFXCAN_NUM_MODULES)
    {
        module = (Ifx_CAN *)IfxCan_cfg_indexMap[can].module;
    }
    else
    {
        module = NULL_PTR;
    }

    return module;
}


IfxCan_Index IfxCan_getIndex(Ifx_CAN *can)
{
    uint32       index;
    IfxCan_Index result;

    result = IfxCan_Index_none;

    for (index = 0; index < IFXCAN_NUM_MODULES; index++)
    {
        if (IfxCan_cfg_indexMap[index].module == can)
        {
            result = (IfxCan_Index)IfxCan_cfg_indexMap[index].index;
            break;
        }
    }

    return result;
}


float32 IfxCan_getModuleFrequency(Ifx_CAN *can)
{
    float32 moduleFreq = 0;

    moduleFreq = IfxScuCcu_getMcanFrequency();

    return moduleFreq;
}


volatile Ifx_SRC_SRCR *IfxCan_getSrcPointer(Ifx_CAN *can, IfxCan_InterruptLine interruptLine)
{
    IfxCan_Index canIndex = IfxCan_getIndex(can);
    return &(MODULE_SRC.CAN.CAN[canIndex].INT[interruptLine]);
}


void IfxCan_resetModule(Ifx_CAN *can)
{
    uint16 passwd = IfxScuWdt_getCpuWatchdogPassword();

    IfxScuWdt_clearCpuEndinit(passwd);
    can->KRST0.B.RST = 1;           /* Only if both Kernel reset bits are set a reset is executed */
    can->KRST1.B.RST = 1;
    IfxScuWdt_setCpuEndinit(passwd);

    while (0 == can->KRST0.B.RSTSTAT)   /* Wait until reset is executed */

    {}

    IfxScuWdt_clearCpuEndinit(passwd);
    can->KRSTCLR.B.CLR = 1;         /* Clear Kernel reset status bit */
    IfxScuWdt_setCpuEndinit(passwd);
}


void IfxCan_setClockSource(Ifx_CAN *can, IfxCan_ClockSelect clockSelect, IfxCan_ClockSource clockSource)
{
    Ifx_CAN_MCR mcr;

    /* enable CCCE and CI */
    mcr.U      = can->MCR.U;
    mcr.B.CCCE = 1;
    mcr.B.CI   = 1;
    can->MCR.U = mcr.U;

    /* select clock */
    switch (clockSelect)
    {
    case IfxCan_ClockSelect_0:
        mcr.B.CLKSEL0 = clockSource;
        break;
    case IfxCan_ClockSelect_1:
        mcr.B.CLKSEL1 = clockSource;
        break;
    case IfxCan_ClockSelect_2:
        mcr.B.CLKSEL2 = clockSource;
        break;
    case IfxCan_ClockSelect_3:
        mcr.B.CLKSEL3 = clockSource;
        break;
    }

    can->MCR.U = mcr.U;

    /* disable CCCE and CI */
    mcr.B.CCCE = 0;
    mcr.B.CI   = 0;
    can->MCR.U = mcr.U;
}
