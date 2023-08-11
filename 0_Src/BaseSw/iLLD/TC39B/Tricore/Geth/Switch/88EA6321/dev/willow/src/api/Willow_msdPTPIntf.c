#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"

/******************************************************************************
* Willow_msdPTP.c
*
* DESCRIPTION:
*       API definitions for Precise Time Protocol logic
*
* DEPENDENCIES:
*
* FILE REVISION NUMBER:
******************************************************************************/
#include "Willow_msdPTP.h"

MSD_STATUS Willow_gptpSetIntEnableIntf
(
    IN MSD_QD_DEV* dev,
    IN MSD_LPORT port,
    IN MSD_PTP_PORT_INTERRUPT_EN *interruptEn
)
{
    WILLOW_PTP_PORT_INTERRUPT_EN tempIntEn;

    if (interruptEn == NULL)
    {
        MSD_DBG_ERROR(("Input param PTP_PORT_INTERRUPT_EN in Willow_gptpSetIntEnableIntf is NULL. \n"));
        return MSD_BAD_PARAM;
    }

    tempIntEn.ptpArrIntEn = interruptEn->ptpArrIntEn;
    tempIntEn.ptpDepIntEn = interruptEn->ptpDepIntEn;

    return Willow_gptpSetIntEnable(dev, port, &tempIntEn);
}

MSD_STATUS Willow_gptpGetIntEnableIntf
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_LPORT port,
    OUT MSD_PTP_PORT_INTERRUPT_EN *interruptEn
)
{
    MSD_STATUS retVal;
    WILLOW_PTP_PORT_INTERRUPT_EN tempIntEn;

    if (interruptEn == NULL)
    {
        MSD_DBG_ERROR(("param PTP_PORT_INTERRUPT_EN in Willow_gptpGetIntEnableIntf is NULL. \n"));
        return MSD_BAD_PARAM;
    }

    retVal = Willow_gptpGetIntEnable(dev, port, &tempIntEn);
    if (MSD_OK != retVal)
        return retVal;

    interruptEn->ptpArrIntEn = tempIntEn.ptpArrIntEn;
    interruptEn->ptpDepIntEn = tempIntEn.ptpDepIntEn;

    return retVal;
}

MSD_STATUS Willow_gptpSetPortConfigIntf
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_LPORT port,
    IN  MSD_PTP_PORT_CONFIG *ptpData
)
{
    WILLOW_PTP_PORT_CONFIG tempData;

    if (ptpData == NULL)
    {
        MSD_DBG_ERROR(("Input param PTP_PORT_CONFIG in Willow_gptpSetPortConfigIntf is NULL. \n"));
        return MSD_BAD_PARAM;
    }

    tempData.arrLedCtrl = ptpData->arrLedCtrl;
    tempData.arrTSMode = ptpData->arrTSMode;
    tempData.depLedCtrl = ptpData->depLedCtrl;
    tempData.disTSOverwrite = ptpData->disTSOverwrite;
    tempData.disTSpec = ptpData->disTSpec;
    tempData.etJump = ptpData->etJump;
    tempData.filterAct = ptpData->filterAct;
    tempData.hwAccPtp = ptpData->hwAccPtp;
    tempData.ExthwAcc = ptpData->ExthwAcc;
    tempData.ipJump = ptpData->ipJump;
    tempData.kpFramwSa = ptpData->kpFramwSa;
    tempData.ptpArrIntEn = ptpData->ptpArrIntEn;
    tempData.ptpDepIntEn = ptpData->ptpDepIntEn;
    tempData.transSpec = ptpData->transSpec;

    return Willow_gptpSetPortConfig(dev, port, &tempData);
}

MSD_STATUS Willow_gptpGetPortConfigIntf
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_LPORT port,
    OUT MSD_PTP_PORT_CONFIG *ptpData
)
{
    MSD_STATUS retVal;
    WILLOW_PTP_PORT_CONFIG tempData;

    if (ptpData == NULL)
    {
        MSD_DBG_ERROR(("param PTP_PORT_CONFIG in Willow_gptpGetPortConfigIntf is NULL. \n"));
        return MSD_BAD_PARAM;
    }

    retVal = Willow_gptpGetPortConfig(dev, port, &tempData);
    if (retVal != MSD_OK)
        return retVal;

    ptpData->arrLedCtrl = tempData.arrLedCtrl;
    ptpData->arrTSMode = tempData.arrTSMode;
    ptpData->depLedCtrl = tempData.depLedCtrl;
    ptpData->disTSOverwrite = tempData.disTSOverwrite;
    ptpData->disTSpec = tempData.disTSpec;
    ptpData->etJump = tempData.etJump;
    ptpData->filterAct = tempData.filterAct;
    ptpData->hwAccPtp = tempData.hwAccPtp;
    ptpData->ExthwAcc = tempData.ExthwAcc;
    ptpData->ipJump = tempData.ipJump;
    ptpData->kpFramwSa = tempData.kpFramwSa;
    ptpData->ptpArrIntEn = tempData.ptpArrIntEn;
    ptpData->ptpDepIntEn = tempData.ptpDepIntEn;
    ptpData->transSpec = tempData.transSpec;

    return retVal;
}

MSD_STATUS Willow_gptpGetTimeStampIntf
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_LPORT    port,
    IN  MSD_PTP_TIME    timeToRead,
    OUT MSD_PTP_TS_STATUS    *ptpStatus
)
{
    MSD_STATUS retVal;
    WILLOW_PTP_TS_STATUS tempStatus;

    if (ptpStatus == NULL)
    {
        MSD_DBG_ERROR(("param PTP_TS_STATUS in Willow_gptpGetTimeStampIntf is NULL. \n"));
        return MSD_BAD_PARAM;
    }

    retVal = Willow_gptpGetTimeStamp(dev, port, (WILLOW_PTP_TIME)timeToRead, &tempStatus);
    if (retVal != MSD_OK)
        return retVal;

    ptpStatus->isValid = tempStatus.isValid;
    ptpStatus->ptpSeqId = tempStatus.ptpSeqId;
    ptpStatus->status = tempStatus.status;
    ptpStatus->timeStamped = tempStatus.timeStamped;

    return retVal;
}

MSD_STATUS Willow_gptpSetGlobalConfigIntf
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_PTP_GLOBAL_CONFIG    *ptpData
)
{
    WILLOW_PTP_GLOBAL_CONFIG tempData;

    if (ptpData == NULL)
    {
        MSD_DBG_ERROR(("param PTP_GLOBAL_CONFIG in Willow_gptpSetGlobalConfigIntf is NULL. \n"));
        return MSD_BAD_PARAM;
    }

    tempData.msgIdTSEn = ptpData->msgIdTSEn;
    tempData.ptpEType = ptpData->ptpEType;
    tempData.tsArrPtr = ptpData->tsArrPtr;

    return Willow_gptpSetGlobalConfig(dev, &tempData);
}

MSD_STATUS Willow_gptpGetGlobalConfigIntf
(
    IN  MSD_QD_DEV* dev,
    OUT MSD_PTP_GLOBAL_CONFIG    *ptpData
)
{
    MSD_STATUS retVal;
    WILLOW_PTP_GLOBAL_CONFIG tempData;

    if (ptpData == NULL)
    {
        MSD_DBG_ERROR(("param PTP_GLOBAL_CONFIG in Willow_gptpGetGlobalConfigIntf is NULL. \n"));
        return MSD_BAD_PARAM;
    }

    retVal = Willow_gptpGetGlobalConfig(dev, &tempData);
    if (retVal != MSD_OK)
        return retVal;

    ptpData->msgIdTSEn = tempData.msgIdTSEn;
    ptpData->ptpEType = tempData.ptpEType;
    ptpData->tsArrPtr = tempData.tsArrPtr;

    return retVal;
}

MSD_STATUS Willow_gptpGetIntStatusIntf
(
    IN  MSD_QD_DEV* dev,
    OUT MSD_PTP_INTERRUPT_STATUS *ptpIntSt
)
{
    MSD_STATUS retVal;
    WILLOW_PTP_INTERRUPT_STATUS tempIntSt;

    if (ptpIntSt == NULL)
    {
        MSD_DBG_ERROR(("param PTP_INTERRUPT_STATUS in Willow_gptpGetIntStatusIntf is NULL. \n"));
        return MSD_BAD_PARAM;
    }

    retVal = Willow_gptpGetIntStatus(dev, &tempIntSt);
    if (retVal != MSD_OK)
        return retVal;

    ptpIntSt->ptpEventInt = tempIntSt.ptpEventInt;
    ptpIntSt->ptpIntStPortVect = tempIntSt.ptpIntStPortVect;
    ptpIntSt->ptpTrigGenInt = tempIntSt.ptpTrigGenInt;

    return retVal;
}

MSD_STATUS Willow_gptpSetModeIntf
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_PTP_MODE_CONFIG *mode
)
{
    MSD_STATUS retVal;
    WILLOW_PTP_MODE_CONFIG tempMode;

    if (mode == NULL)
    {
        MSD_DBG_ERROR(("param PTP_MODE_CONFIG in Willow_gptpSetModeIntf is NULL. \n"));
        return MSD_BAD_PARAM;
    }

    tempMode.altScheme = mode->altScheme;
    tempMode.grandMstr = mode->grandMstr;
    tempMode.oneStep = mode->oneStep;
    tempMode.PTPMode = mode->PTPMode;

    retVal = Willow_gptpSetMode(dev, &tempMode);

    return retVal;
}

MSD_STATUS Willow_gptpGetModeIntf
(
    IN  MSD_QD_DEV* dev,
    OUT MSD_PTP_MODE_CONFIG *mode
)
{
    MSD_STATUS retVal;
    WILLOW_PTP_MODE_CONFIG tempMode;

    if (mode == NULL)
    {
        MSD_DBG_ERROR(("param PTP_MODE_CONFIG in Willow_gptpGetModeIntf is NULL. \n"));
        return MSD_BAD_PARAM;
    }

    retVal = Willow_gptpGetMode(dev, &tempMode);
    if (retVal != MSD_OK)
        return retVal;

    mode->altScheme = tempMode.altScheme;
    mode->grandMstr = tempMode.grandMstr;
    mode->oneStep = tempMode.oneStep;
    mode->PTPMode = tempMode.PTPMode;

    return retVal;
}

MSD_STATUS Willow_gptpSetMasterSlaveIntf
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_PTP_MASTER_SLAVE role
)
{
    return Willow_gptpSetMasterSlave(dev, (WILLOW_PTP_MASTER_SLAVE)role);
}

MSD_STATUS Willow_gptpGetMasterSlaveIntf
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_PTP_MASTER_SLAVE *role
)
{
    return Willow_gptpGetMasterSlave(dev, (WILLOW_PTP_MASTER_SLAVE*)role);
}

MSD_STATUS Willow_gptpTODStoreCompensationIntf
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_U8 timeArrayIndex,
    IN  MSD_PTP_TIME_ARRAY *timeArray
)
{
    WILLOW_PTP_TIME_ARRAY tmpArray;

    if (timeArray == NULL)
    {
        MSD_DBG_ERROR(("param PTP_TIME_ARRAY in Willow_gptpTODStoreCompensationIntf is NULL. \n"));
        return MSD_BAD_PARAM;
    }

    tmpArray.clkValid = timeArray->clkValid;
    tmpArray.domainNumber = timeArray->domainNumber;
    tmpArray.Nanoseconds1722High = timeArray->Nanoseconds1722High;
    tmpArray.Nanoseconds1722Low = timeArray->Nanoseconds1722Low;
    tmpArray.todCompensation = timeArray->todCompensation;
    tmpArray.todLoadPoint = timeArray->todLoadPoint;
    tmpArray.todNanoseconds = timeArray->todNanoseconds;
    tmpArray.todSecondsHigh = timeArray->todSecondsHigh;
    tmpArray.todSecondsLow = timeArray->todSecondsLow;
        
    return Willow_gptpTODStoreCompensation(dev, timeArrayIndex, &tmpArray);
}

MSD_STATUS Willow_gptpTODStoreAllIntf
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_U8 timeArrayIndex,
    IN  MSD_PTP_TIME_ARRAY *timeArray
)
{
    WILLOW_PTP_TIME_ARRAY tmpArray;

    if (timeArray == NULL)
    {
        MSD_DBG_ERROR(("param PTP_TIME_ARRAY in Willow_gptpTODStoreAllIntf is NULL. \n"));
        return MSD_BAD_PARAM;
    }

    tmpArray.clkValid = timeArray->clkValid;
    tmpArray.domainNumber = timeArray->domainNumber;
    tmpArray.Nanoseconds1722High = timeArray->Nanoseconds1722High;
    tmpArray.Nanoseconds1722Low = timeArray->Nanoseconds1722Low;
    tmpArray.todCompensation = timeArray->todCompensation;
    tmpArray.todLoadPoint = timeArray->todLoadPoint;
    tmpArray.todNanoseconds = timeArray->todNanoseconds;
    tmpArray.todSecondsHigh = timeArray->todSecondsHigh;
    tmpArray.todSecondsLow = timeArray->todSecondsLow;

    return Willow_gptpTODStoreAll(dev, timeArrayIndex, &tmpArray);
}

MSD_STATUS Willow_gptpTODCaptureAllIntf
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_U8 timeArrayIndex,
    OUT MSD_PTP_TIME_ARRAY *timeArray
)
{
    MSD_STATUS retVal;
    WILLOW_PTP_TIME_ARRAY tmpArray;

    if (timeArray == NULL)
    {
        MSD_DBG_ERROR(("param PTP_TIME_ARRAY in Willow_gptpTODStoreAllIntf is NULL. \n"));
        return MSD_BAD_PARAM;
    }

    retVal = Willow_gptpTODCaptureAll(dev, timeArrayIndex, &tmpArray);
    if (retVal != MSD_OK)
        return retVal;

    timeArray->clkValid = tmpArray.clkValid;
    timeArray->domainNumber = tmpArray.domainNumber;
    timeArray->Nanoseconds1722High = tmpArray.Nanoseconds1722High;
    timeArray->Nanoseconds1722Low = tmpArray.Nanoseconds1722Low;
    timeArray->todCompensation = tmpArray.todCompensation;
    timeArray->todLoadPoint = tmpArray.todLoadPoint;
    timeArray->todNanoseconds = tmpArray.todNanoseconds;
    timeArray->todSecondsHigh = tmpArray.todSecondsHigh;
    timeArray->todSecondsLow = tmpArray.todSecondsLow;

    return retVal;
}

MSD_STATUS Willow_gptpSetPulseIntf
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_PTP_PULSE_STRUCT *pulsePara
)
{
    WILLOW_PTP_PULSE_STRUCT tmpData;

    if (pulsePara == NULL)
    {
        MSD_DBG_ERROR(("param PTP_PULSE_STRUCT in Willow_gptpSetPulseIntf is NULL. \n"));
        return MSD_BAD_PARAM;
    }

    tmpData.ptp1ppsPhase = pulsePara->ptp1ppsPhase;
    tmpData.ptp1ppsSelect = pulsePara->ptp1ppsSelect;
    tmpData.ptp1ppsWidthRange = pulsePara->ptp1ppsWidthRange;
    tmpData.ptpPulseWidth = pulsePara->ptpPulseWidth;

    return Willow_gptpSetPulse(dev, &tmpData);
}

MSD_STATUS Willow_gptpGetPulseIntf
(
    IN  MSD_QD_DEV* dev,
    OUT MSD_PTP_PULSE_STRUCT *pulsePara
)
{
    MSD_STATUS retVal;
    WILLOW_PTP_PULSE_STRUCT tmpData;

    if (pulsePara == NULL)
    {
        MSD_DBG_ERROR(("param PTP_PULSE_STRUCT in msdPtpPulseGetIntf is NULL. \n"));
        return MSD_BAD_PARAM;
    }

    retVal = Willow_gptpGetPulse(dev, &tmpData);

    pulsePara->ptp1ppsPhase = tmpData.ptp1ppsPhase;
    pulsePara->ptp1ppsSelect = tmpData.ptp1ppsSelect;
    pulsePara->ptp1ppsWidthRange = tmpData.ptp1ppsWidthRange;
    pulsePara->ptpPulseWidth = tmpData.ptpPulseWidth;

    return retVal;
}
#endif
