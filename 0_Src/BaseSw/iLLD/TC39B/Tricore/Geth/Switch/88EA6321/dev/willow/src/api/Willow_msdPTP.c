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
#include "Willow_msdApiInternal.h"
#include "Willow_msdHwAccess.h"
#include "Willow_msdDrvSwRegs.h"
#include "msdUtils.h"
#include "msdSem.h"
#include "msdHwAccess.h"

typedef enum
{
    WILLOW_MSD_AVB_BLOCK_PTP = 0x0,
    WILLOW_MSD_AVB_BLOCK_AVB_POLICY = 0x1,
    WILLOW_MSD_AVB_BLOCK_QAV = 0x2,
    WILLOW_MSD_AVB_BLOCK_QBV = 0x3
} WILLOW_MSD_AVB_BLOCK;

/*
* Typedef: enum WILLOW_MSD_PTP_OPERATION
*
* Description: Defines the PTP (Precise Time Protocol) Operation type
*
* Fields:
*      WILLOW_PTP_WRITE_DATA             - Write data to the PTP register
*      WILLOW_PTP_READ_DATA            - Read data from PTP register
*      WILLOW_PTP_READ_MULTIPLE_DATA    - Read multiple data from PTP register
*      WILLOW_PTP_READ_TIMESTAMP_DATA    - Read timestamp data from PTP register
*                    valid bit will be reset after read
*/
typedef enum
{
    WILLOW_PTP_WRITE_DATA = 0x3,
    WILLOW_PTP_READ_DATA = 0x4,
    WILLOW_PTP_READ_MULTIPLE_DATA = 0x6,
    WILLOW_PTP_READ_TIMESTAMP_DATA = 0x8,
} WILLOW_MSD_PTP_OPERATION;

/*
*  typedef: struct WILLOW_MSD_PTP_OP_DATA
*
*  Description: data required by PTP Operation
*
*  Fields:
*      ptpPort        - physical port of the device
*      ptpBlock    - AVB Block
*      ptpAddr     - register address
*      ptpData     - data for ptp register.
*      ptpMultiData- used for multiple read operation.
*      nData         - number of data to be read on multiple read operation.
*/
typedef struct
{
    MSD_U32    ptpPort;
    MSD_U32    ptpBlock;
    MSD_U32    ptpAddr;
    MSD_U32    ptpData;
    MSD_U32    ptpMultiData[MAX_PTP_CONSECUTIVE_READ];
    MSD_U32    nData;
} WILLOW_MSD_PTP_OP_DATA;


/******************************************************************************
* ptpOperationPerform
*
* DESCRIPTION:
*       This function accesses PTP Command Register and Data Register.
*
* INPUTS:
*       ptpOp      - The stats operation bits to be written into the stats
*                    operation register.
*
* OUTPUTS:
*       ptpData    - points to the data storage that the operation requires.
*
* RETURNS:
*       MSD_OK on success,
*       MSD_FAIL otherwise.
*
* COMMENTS:
*
******************************************************************************/
static MSD_STATUS ptpOperationPerform
(
    IN    MSD_QD_DEV             *dev,
    IN    WILLOW_MSD_PTP_OPERATION      ptpOp,
    INOUT WILLOW_MSD_PTP_OP_DATA        *opData
)
{
    MSD_STATUS       retVal;    /* Functions return value */
    MSD_U32             i;
	MSD_U16          ptpOperation;
	MSD_U16 data;

    msdSemTake(dev->devNum,dev->ptpRegsSem,OS_WAIT_FOREVER);

    /* Wait until the ptp in ready. */
    retVal = Willow_msdWaitForBit(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_AVB_COMMAND, 15, 0);
    if (retVal != MSD_OK)
    {
        msdRegisterAccessFlush(dev->devNum);
        msdSemGive(dev->devNum, dev->ptpRegsSem);
        return retVal;
    }

    /* Set the PTP Operation register */
    switch (ptpOp)
    {
        case WILLOW_PTP_WRITE_DATA:
		{
            data = (MSD_U16)opData->ptpData;
            retVal = msdSetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_AVB_DATA, data);
            if(retVal != MSD_OK)
            {
                msdRegisterAccessFlush(dev->devNum);
                msdSemGive(dev->devNum, dev->ptpRegsSem);
                return retVal;
            }

			ptpOperation = WILLOW_PTP_WRITE_DATA;
            ptpOperation &= 3;
            ptpOperation <<=13;
            data = (MSD_U16)((1 << 15) | (ptpOperation) | 
                    (opData->ptpPort << 8)    |
                    (opData->ptpBlock << 5)    |
                    (opData->ptpAddr & 0x1F));
            retVal = msdSetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR,
                WILLOW_QD_REG_AVB_COMMAND, data);
            if(retVal != MSD_OK)
            {
                msdRegisterAccessFlush(dev->devNum);
                msdSemGive(dev->devNum, dev->ptpRegsSem);
                return retVal;
            }
		}
		break;

        case WILLOW_PTP_READ_DATA:
		{
	        ptpOperation = WILLOW_PTP_READ_DATA;
            ptpOperation &= 3;
            ptpOperation <<=13;
            data = (MSD_U16)((1 << 15) | (ptpOperation) | 
                    (opData->ptpPort << 8)    |
                    (opData->ptpBlock << 5)    |
                    (opData->ptpAddr & 0x1F));
            retVal = msdSetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR,
                WILLOW_QD_REG_AVB_COMMAND, data);
            if(retVal != MSD_OK)
            {
                msdRegisterAccessFlush(dev->devNum);
                msdSemGive(dev->devNum, dev->ptpRegsSem);
                return retVal;
            }

            retVal = Willow_msdWaitForBit(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_AVB_COMMAND, 15, 0);
            if (retVal != MSD_OK)
            {
                msdRegisterAccessFlush(dev->devNum);
                msdSemGive(dev->devNum, dev->ptpRegsSem);
                return retVal;
            }

            retVal = msdGetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_AVB_DATA, &data);
			if(retVal != MSD_OK)
            {
                msdRegisterAccessFlush(dev->devNum);
                msdSemGive(dev->devNum, dev->ptpRegsSem);
                return retVal;
            }
            retVal = msdRegisterAccessFlush(dev->devNum);
            if (retVal != MSD_OK)
            {
                msdSemGive(dev->devNum, dev->ptpRegsSem);
                return retVal;
            }
            opData->ptpData = (MSD_U32)data;
		}
        break;

        case WILLOW_PTP_READ_MULTIPLE_DATA:
		{
	        ptpOperation = WILLOW_PTP_READ_MULTIPLE_DATA;
            ptpOperation &= 3;
            ptpOperation <<=13;
            data = (MSD_U16)((1 << 15) | (ptpOperation) | 
                    (opData->ptpPort << 8)    |
                    (opData->ptpBlock << 5)    |
                    (opData->ptpAddr & 0x1F));
            retVal = msdSetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_AVB_COMMAND, data);
            if(retVal != MSD_OK)
            {
                msdRegisterAccessFlush(dev->devNum);
                msdSemGive(dev->devNum, dev->ptpRegsSem);
                return retVal;
            }

            retVal = Willow_msdWaitForBit(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_AVB_COMMAND, 15, 0);
            if (retVal != MSD_OK)
            {
                msdRegisterAccessFlush(dev->devNum);
                msdSemGive(dev->devNum, dev->ptpRegsSem);
                return retVal;
            }

            for(i=0; i<opData->nData; i++)
            {
                retVal = msdGetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_AVB_DATA, &data);
                if(retVal != MSD_OK)
                {
                    msdRegisterAccessFlush(dev->devNum);
                    msdSemGive(dev->devNum, dev->ptpRegsSem);
                    return retVal;
                }
                retVal = msdRegisterAccessFlush(dev->devNum);
                if (retVal != MSD_OK)
                {
                    msdSemGive(dev->devNum, dev->ptpRegsSem);
                    return retVal;
                }
                opData->ptpMultiData[i] = (MSD_U32)data;
            }
		}
        break;

        case WILLOW_PTP_READ_TIMESTAMP_DATA:
		{
	        ptpOperation = WILLOW_PTP_READ_MULTIPLE_DATA;
            ptpOperation &= 3;
            ptpOperation <<=13;
            data = (MSD_U16)((1 << 15) | (ptpOperation) | 
                    (opData->ptpPort << 8)    |
                    (opData->ptpBlock << 5)    |
                    (opData->ptpAddr & 0x1F));
            retVal = msdSetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_AVB_COMMAND, data);
            if(retVal != MSD_OK)
            {
                msdRegisterAccessFlush(dev->devNum);
                msdSemGive(dev->devNum, dev->ptpRegsSem);
                return retVal;
            }

            retVal = Willow_msdWaitForBit(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_AVB_COMMAND, 15, 0);
            if (retVal != MSD_OK)
            {
                msdRegisterAccessFlush(dev->devNum);
                msdSemGive(dev->devNum, dev->ptpRegsSem);
                return retVal;
            }

            retVal = msdGetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_AVB_DATA, &data);
            if(retVal != MSD_OK)
            {
                msdRegisterAccessFlush(dev->devNum);
                msdSemGive(dev->devNum, dev->ptpRegsSem);
                return retVal;
            }
            retVal = msdRegisterAccessFlush(dev->devNum);
            if (retVal != MSD_OK)
            {
                msdSemGive(dev->devNum, dev->ptpRegsSem);
                return retVal;
            }
            opData->ptpMultiData[0] = (MSD_U32)data;

            if (!(data & 0x1))
            {
                /* valid bit is not set */
                msdSemGive(dev->devNum,dev->ptpRegsSem);
                return retVal;
            }

            for(i=1; i<opData->nData; i++)
            {
                retVal = msdGetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR,
                    WILLOW_QD_REG_AVB_DATA, &data);
                if (retVal != MSD_OK)
                {
                    msdRegisterAccessFlush(dev->devNum);
                    msdSemGive(dev->devNum, dev->ptpRegsSem);
                    return retVal;
                }
                retVal = msdRegisterAccessFlush(dev->devNum);
                if (retVal != MSD_OK)
                {
                    msdSemGive(dev->devNum, dev->ptpRegsSem);
                    return retVal;
                }
                opData->ptpMultiData[i] = (MSD_U32)data;
            }

            retVal = msdSetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_AVB_DATA, 0);
            if(retVal != MSD_OK)
            {
                msdRegisterAccessFlush(dev->devNum);
                msdSemGive(dev->devNum, dev->ptpRegsSem);
                return retVal;
            }

	        ptpOperation = WILLOW_PTP_WRITE_DATA;
            ptpOperation &= 3;
            ptpOperation <<=13;
            data = (MSD_U16)((1 << 15) | (ptpOperation) | 
                    (opData->ptpPort << 8)    |
                    (opData->ptpBlock << 5)    |
                    (opData->ptpAddr & 0x1F));
            retVal = msdSetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_AVB_COMMAND, data);
            if(retVal != MSD_OK)
            {
                msdRegisterAccessFlush(dev->devNum);
                msdSemGive(dev->devNum, dev->ptpRegsSem);
                return retVal;
            }
		}
        break;

        default:
		{
            msdRegisterAccessFlush(dev->devNum);
			msdSemGive(dev->devNum,dev->ptpRegsSem);
            return MSD_FAIL;
		}
    }

    retVal = Willow_msdWaitForBit(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, WILLOW_QD_REG_AVB_COMMAND, 15, 0);
    if (retVal != MSD_OK)
    {
        msdRegisterAccessFlush(dev->devNum);
        msdSemGive(dev->devNum, dev->ptpRegsSem);
        return retVal;
    }
    retVal = msdRegisterAccessFlush(dev->devNum);
    msdSemGive(dev->devNum,dev->ptpRegsSem);
    return retVal;
}

/*******************************************************************************
* gptpSetReg
*
* DESCRIPTION:
*       This routine writes data to PTP register.
*
* INPUTS:
*       port         - logical port number
*       regOffset    - register to be written
*        data        - data to be written
*
* OUTPUTS:
*        None.
*
* RETURNS:
*       MSD_OK      - on success
*       MSD_FAIL    - on error
*       MSD_BAD_PARAM  - bad parameter
*       MSD_NOT_SUPPORTED - if current device does not support this feature.
*
* COMMENTS:
*       None
*
*******************************************************************************/
static MSD_STATUS gptpSetReg
(
    IN  MSD_QD_DEV     *dev,
    IN  MSD_LPORT      port,
    IN  MSD_U32        regOffset,
    IN  MSD_U32        data
)
{
    MSD_STATUS           retVal;
    MSD_U32                hwPort;
    WILLOW_MSD_PTP_OPERATION    op;
    WILLOW_MSD_PTP_OP_DATA        opData;

    MSD_DBG_INFO(("gptpSetReg Called.\n"));

    /* translate logical port to physical port */
    if (port == 0x1F)
    {
        hwPort = port;
    }
    else
    {
        hwPort = MSD_LPORT_2_PORT(port);
        if (hwPort == MSD_INVALID_PORT)
        {
            MSD_DBG_ERROR(("Failed (Bad Port).\n"));
            return MSD_BAD_PARAM;
        }
    }

    if ((regOffset > 0x1F) || (data > 0xFFFF))
    {
        MSD_DBG_ERROR(("Failed (Invalid reg offset/data).\n"));
        return MSD_BAD_PARAM;
    }

    op = WILLOW_PTP_WRITE_DATA;
    opData.ptpPort = hwPort;
    opData.ptpAddr = regOffset;
    opData.ptpData = data;
    opData.ptpBlock = WILLOW_MSD_AVB_BLOCK_PTP;    /* PTP register space */

    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed (ptpOperationPerform).\n"));
        return MSD_FAIL;
    }

    MSD_DBG_INFO(("OK.\n"));
    return MSD_OK;
}

/*******************************************************************************
* gptpGetReg
*
* DESCRIPTION:
*       This routine reads PTP register.
*
* INPUTS:
*       port         - logical port number.
*       regOffset    - register to read
*
* OUTPUTS:
*        data        - register data
*
* RETURNS:
*       MSD_OK      - on success
*       MSD_FAIL    - on error
*       MSD_BAD_PARAM  - bad parameter
*       MSD_NOT_SUPPORTED - if current device does not support this feature.
*
* COMMENTS:
*       None
*
*******************************************************************************/
static MSD_STATUS gptpGetReg
(
    IN  MSD_QD_DEV     *dev,
    IN  MSD_LPORT	  port,
    IN  MSD_U32        regOffset,
    OUT MSD_U32        *data
)
{
    MSD_STATUS           retVal;
    MSD_U32              hwPort;
    WILLOW_MSD_PTP_OPERATION    op;
    WILLOW_MSD_PTP_OP_DATA      opData;

    /* translate logical port to physical port */
    if (port == 0x1F)
    {
        hwPort = port;
    }
    else
    {
        hwPort = MSD_LPORT_2_PORT(port);
        if (hwPort == MSD_INVALID_PORT)
        {
            MSD_DBG_ERROR(("Failed (Bad Port).\n"));
            return MSD_BAD_PARAM;
        }
    }

    if (regOffset > 0x1F)
    {
        MSD_DBG_ERROR(("Failed (Invalid reg offset).\n"));
        return MSD_BAD_PARAM;
    }

    op = WILLOW_PTP_READ_DATA;
    opData.ptpPort = hwPort;
    opData.ptpAddr = regOffset;
    opData.ptpBlock = WILLOW_MSD_AVB_BLOCK_PTP;    /* PTP register space */

    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed (ptpOperationPerform).\n"));
        return MSD_FAIL;
    }

    *data = opData.ptpData;

    return MSD_OK;
}

/*******************************************************************************
* gptpGlobalPointDataSet
*
* DESCRIPTION:
*        Set indirect register pointer control data from PTP Global registers.
*
* INPUTS:
*        port - logical port number.
*        point - Pointer to following index registers:
*            PTP_GLOBAL_CONFIG_PTP_MODE_REG
*            PTP_GLOBAL_CONFIG_PTP_EXTERNAL_TIME_FORM
*        data -  PTP Global configuration data write to the indirect register
*                pointed to by the point above.
*
* OUTPUTS:
*        None.
*
* RETURNS:
*
* COMMENTS:
*        None
*
*******************************************************************************/
static MSD_STATUS gptpGlobalPointDataSet
(
    IN  MSD_QD_DEV *dev,
    IN  MSD_U8 point,
    IN  MSD_U8 data
)
{
    MSD_STATUS retVal;
    MSD_U32 count, tmpData;

    msdSemTake(dev->devNum, dev->ptpModeSem, OS_WAIT_FOREVER);

    count = 5;
    tmpData = 0x8000;
    while (tmpData & 0x8000)
    {
        retVal = gptpGetReg(dev, 0x1F, 7, &tmpData);
        if (retVal != MSD_OK)
        {
            msdSemGive(dev->devNum, dev->ptpModeSem);
            return retVal;
        }
		if (--count == 0)
		{
			msdSemGive(dev->devNum, dev->ptpModeSem);
			return MSD_FAIL;
		}
    }

    tmpData = (MSD_U16)((1 << 15) | (point << 8) | data);

    retVal = gptpSetReg(dev, 0x1f, 7, tmpData);
    if (retVal != MSD_OK)
    {
        msdSemGive(dev->devNum, dev->ptpModeSem);
        return retVal;
    }

    msdSemGive(dev->devNum, dev->ptpModeSem);
    return retVal;
}

/*******************************************************************************
* gptpGlobalPointDataGet
*
* DESCRIPTION:
*        Get indirect register pointer control data from PTP Global registers.
*
* INPUTS:
*        point - Pointer to following index registers:
*            PTP_GLOBAL_CONFIG_PTP_MODE_REG
*            PTP_GLOBAL_CONFIG_PTP_EXTERNAL_TIME_FORM
* OUTPUTS:
*        data -  PTP Global configuration data read from the indirect register
*                pointed to by the point above.
*
* RETURNS:
*
* COMMENTS:
*        None
*
*******************************************************************************/
static MSD_STATUS gptpGlobalPointDataGet
(
    IN  MSD_QD_DEV *dev,
    IN  MSD_U8 point,
    OUT MSD_U8 *data
)
{
    MSD_STATUS retVal;
    MSD_U32 count, tmpData;

    msdSemTake(dev->devNum, dev->ptpModeSem, OS_WAIT_FOREVER);
    count = 5;

    tmpData = (MSD_U16)((point << 8) | 0);
    retVal = gptpSetReg(dev, 0x1f, 7, tmpData);
    if (retVal != MSD_OK)
    {
        msdSemGive(dev->devNum, dev->ptpModeSem);
        return retVal;
    }

    do {
        retVal = gptpGetReg(dev, 0x1f, 7, &tmpData);
        if (retVal != MSD_OK)
        {
            msdSemGive(dev->devNum, dev->ptpModeSem);
            return retVal;
        }
		if ((count--) == 0)
		{
			msdSemGive(dev->devNum, dev->ptpModeSem);
			return MSD_FAIL;
		}

    } while (tmpData & 0x8000);

    *data = (MSD_U8)tmpData;
    msdSemGive(dev->devNum, dev->ptpModeSem);

    return retVal;
}


/*******************************************************************************
* gptpTimeArraySet
*
* DESCRIPTION:
*       This function Store PTP time array Data.
*
* INPUTS:
*       storeOp   - Store compensation and store all
*       timeArrayIndex - Time array index
*       timeArray -    TOD load pointer
*                       10 byte ToD time,
*                       8 byte 1722 time,
*                       4 byte Compensation,
*                       1 byte Domain Number,
*                       And a 1 bit Clock Valid.
*
* RETURNS:
*
* COMMENTS:
*
*******************************************************************************/
static MSD_STATUS gptpTimeArrayStore
(
    IN  MSD_QD_DEV *dev,
    IN  MSD_U8 timeArrayIndex,
    IN  WILLOW_PTP_TOD_STORE_OPERATION storeOp,
    IN  WILLOW_PTP_TIME_ARRAY *timeArray
)
{
    MSD_STATUS retVal;  
    WILLOW_MSD_PTP_OP_DATA     opData;
    WILLOW_MSD_PTP_OPERATION   op;


    /*Load point Set*/
    opData.ptpBlock = 0;
    opData.ptpPort = 0x1F;    /* Global register */
    op = WILLOW_PTP_WRITE_DATA;
    opData.ptpAddr = 0x10;
    opData.ptpData = timeArray->todLoadPoint & 0xffff;
    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed writing TOD Load Point.\n"));
        return retVal;
    }
    opData.ptpAddr = 0x11;
    opData.ptpData = (timeArray->todLoadPoint >> 16) & 0xffff;
    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed reading TOD Load Point.\n"));
        return retVal;
    }

    /*Time array index, Clock Valid bit and domain number will be set when do the operation*/

    /*ToD Set*/
    opData.ptpBlock = 0;
    opData.ptpPort = 0x1F;    /* Global register */
    op = WILLOW_PTP_WRITE_DATA;

    opData.ptpAddr = 0x13;
    opData.ptpData = timeArray->todNanoseconds & 0xffff;
    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed writing ToD.\n"));
        return retVal;
    }
    opData.ptpAddr = 0x14;
    opData.ptpData = (timeArray->todNanoseconds >> 16) & 0xffff;
    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed writing ToD.\n"));
        return retVal;
    }
    opData.ptpAddr = 0x15;
    opData.ptpData = timeArray->todSecondsLow & 0xffff;
    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed writing ToD.\n"));
        return retVal;
    }
    opData.ptpAddr = 0x16;
    opData.ptpData = (timeArray->todSecondsLow >> 16) & 0xffff;
    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed writing ToD.\n"));
        return retVal;
    }
    opData.ptpAddr = 0x17;
    opData.ptpData = (timeArray->todSecondsHigh) & 0xffff;
    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed writing ToD.\n"));
        return retVal;
    }

    /*1722 time set*/
    opData.ptpBlock = 0;
    opData.ptpPort = 0x1F;    /* Global register */
    op = WILLOW_PTP_WRITE_DATA;

    opData.ptpAddr = 0x18;
    opData.ptpData = timeArray->Nanoseconds1722Low & 0xffff;
    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed writing 1722 ToD.\n"));
        return retVal;
    }
    opData.ptpAddr = 0x19;
    opData.ptpData = (timeArray->Nanoseconds1722Low >> 16) & 0xffff;
    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed writing 1722 ToD.\n"));
        return retVal;
    }
    opData.ptpAddr = 0x1a;
    opData.ptpData = (timeArray->Nanoseconds1722High) & 0xffff;
    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed writing 1722 ToD.\n"));
        return retVal;
    }
    opData.ptpAddr = 0x1b;
    opData.ptpData = (timeArray->Nanoseconds1722High >> 16) & 0xffff;
    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed writing 1722 ToD.\n"));
        return retVal;
    }

    /*Tod compensation set*/
    opData.ptpBlock = 0;
    opData.ptpPort = 0x1F;    /* Global register */
    op = WILLOW_PTP_WRITE_DATA;
    opData.ptpAddr = 0x1C;
    opData.ptpData = (timeArray->todCompensation & 0xffff);
    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed writing ToD compensation.\n"));
        return retVal;
    }

    opData.ptpAddr = 0x1D;
    opData.ptpData = ((timeArray->todCompensation >> 16) & 0xffff);
    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed writing ToD compensation.\n"));
        return retVal;
    }

    /*Store operation*/
    opData.ptpBlock = 0;
    opData.ptpPort = 0x1F;    /* Global register */
    op = WILLOW_PTP_WRITE_DATA;
    opData.ptpAddr = 0x12;
    opData.ptpData = ((1 << 15) |
        (storeOp << 12) |
        ((timeArrayIndex & 3) << 9) |
        (((timeArray->clkValid) & 1) << 8) |
        (timeArray->domainNumber & 0xff));

    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed writing ToD store operation control.\n"));
    }

    return retVal;
}

/*******************************************************************************
* gptpTimeArrayCapture
*
* DESCRIPTION:
*       This function Capture PTP time array Data.
*
* INPUTS:
*       timeArrayIndex - Time array index
*       timeArray -    TOD load pointer
*                       10 byte ToD time,
*                       8 byte 1722 time,
*                       4 byte Compensation,
*                       1 byte Domain Number,
*                       And a 1 bit Clock Valid.
*
* RETURNS:
*
* COMMENTS:
*
*******************************************************************************/
static MSD_STATUS gptpTimeArrayCapture
(
    IN  MSD_QD_DEV *dev,
    IN  MSD_U8 timeArrayIndex,
    OUT WILLOW_PTP_TIME_ARRAY	   *timeArray
)
{
    MSD_STATUS retVal; 
    WILLOW_MSD_PTP_OP_DATA   opData;
    WILLOW_MSD_PTP_OPERATION op;
    MSD_U32 tmpData;

    /*Get the origin operation register value*/
    opData.ptpBlock = 0;
    opData.ptpPort = 0x1F;    /* Global register */
    op = WILLOW_PTP_READ_DATA;
    opData.ptpAddr = 0x12;
    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed reading ToD operation control.\n"));
        return retVal;
    }
    tmpData = opData.ptpData;

    /*Capture operation*/
    opData.ptpBlock = 0;
    opData.ptpPort = 0x1F;    /* Global register */
    op = WILLOW_PTP_WRITE_DATA;
    opData.ptpAddr = 0x12;
    tmpData &= 0x89ff;
    opData.ptpData = ((1 << 15) |
        (4 << 12) |
        ((timeArrayIndex & 3) << 9)) |
        tmpData;
    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed writing ToD capture operation control.\n"));
        return retVal;
    }

    /*Get the Load Point*/
    opData.ptpBlock = 0;
    opData.ptpPort = 0x1F;    /* Global register */
    opData.ptpAddr = 0x10;
    op = WILLOW_PTP_READ_MULTIPLE_DATA;
    opData.nData = 2;

    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed reading TOD Load Point.\n"));
        return retVal;
    }

    timeArray->todLoadPoint = opData.ptpMultiData[1];
    timeArray->todLoadPoint <<= 16;
    timeArray->todLoadPoint |= (opData.ptpMultiData[0]);

    /*Get clock valid bit and domain number*/
    opData.ptpBlock = 0;
    opData.ptpPort = 0x1F;    /* Global register */
    op = WILLOW_PTP_READ_DATA;
    opData.ptpAddr = 0x12;
    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed reading ToD operation control.\n"));
        return retVal;
    }
    timeArray->clkValid = ((opData.ptpData >> 8) & 1) == 1 ? MSD_TRUE : MSD_FALSE;
    timeArray->domainNumber = (MSD_U8)(opData.ptpData & 0xff);

    /*Get the Tod time*/
    opData.ptpBlock = 0;
    opData.ptpPort = 0x1F;    /* Global register */
    op = WILLOW_PTP_READ_MULTIPLE_DATA;
    opData.nData = 4;
    opData.ptpAddr = 0x13;

    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed reading ToD.\n"));
        return retVal;
    }

    timeArray->todNanoseconds = opData.ptpMultiData[1] & 0xffff;
    timeArray->todNanoseconds <<= 16;
    timeArray->todNanoseconds |= (opData.ptpMultiData[0] & 0xffff);
    timeArray->todSecondsLow = opData.ptpMultiData[3] & 0xffff;
    timeArray->todSecondsLow <<= 16;
    timeArray->todSecondsLow |= (opData.ptpMultiData[2] & 0xffff);

    op = WILLOW_PTP_READ_DATA;
    opData.ptpAddr = 0x17;
    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed reading ToD.\n"));
        return retVal;
    }

    timeArray->todSecondsHigh = (MSD_U16)((opData.ptpData & 0xffff));

    /*Get 1722 time*/
    opData.ptpBlock = 0;
    opData.ptpPort = 0x1F;    /* Global register */
    op = WILLOW_PTP_READ_MULTIPLE_DATA;
    opData.nData = 4;
    opData.ptpAddr = 0x18;

    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed reading 1722 ToD.\n"));
        return retVal;
    }

    timeArray->Nanoseconds1722High = (opData.ptpMultiData[3] << 16) | opData.ptpMultiData[2];
    timeArray->Nanoseconds1722Low = (opData.ptpMultiData[1] << 16) | opData.ptpMultiData[0];

    /*Get compensation*/
    opData.ptpBlock = 0;
    opData.ptpPort = 0x1F;    /* Global register */
    op = WILLOW_PTP_READ_MULTIPLE_DATA;
    opData.nData = 2;
    opData.ptpAddr = 0x1C;

    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed reading ToD compensation.\n"));
        return retVal;
    }

    timeArray->todCompensation = (opData.ptpMultiData[1] & 0xffff) << 16;
    timeArray->todCompensation |= opData.ptpMultiData[0];

    return retVal;
}


MSD_STATUS Willow_gptpSetIntEnable
(
    IN MSD_QD_DEV* dev,
    IN MSD_LPORT port,
    IN WILLOW_PTP_PORT_INTERRUPT_EN *interruptEn
)
{
    MSD_STATUS  retval;
    MSD_U32 data = 0;

    MSD_DBG_INFO(("Willow_gptpSetIntEnable Called.\n"));

    retval = gptpGetReg(dev, port, 0x2, &data);
    if (retval != MSD_OK)
    {
        MSD_DBG_ERROR(("Get the PTP register error\n"));
        return retval;
    }

    data = data & (~3);
    data = data | (interruptEn->ptpArrIntEn ? 1 : 0) | ((interruptEn->ptpDepIntEn ? 1 : 0) << 1);
    retval = gptpSetReg(dev, port, 0x02, data);
    if (retval != MSD_OK)
    {
        MSD_DBG_ERROR(("Set the PTP register error\n"));
        return retval;
    }

    MSD_DBG_INFO(("Willow_gptpSetIntEnable Exit.\n"));
    return retval;
}
MSD_STATUS Willow_gptpGetIntEnable
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_LPORT port,
    OUT WILLOW_PTP_PORT_INTERRUPT_EN *interruptEn
)
{
    MSD_STATUS  retval;
    MSD_U32 data = 0;

    MSD_DBG_INFO(("Willow_gptpGetIntEnable Called.\n"));

    retval = gptpGetReg(dev, port, 0x2, &data);
    if (retval != MSD_OK)
    {
        MSD_DBG_ERROR(("Get the PTP register error\n"));
        return retval;
    }

    interruptEn->ptpArrIntEn = data & 0x1;
    interruptEn->ptpDepIntEn = (data & 0x02) >> 1;

    MSD_DBG_INFO(("Willow_gptpGetIntEnable Exit.\n"));
    return retval;
}

MSD_STATUS Willow_gptpSetProtPTPEn
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_LPORT port,
    IN  MSD_BOOL  en
)
{
    MSD_STATUS  retval;
    MSD_U32 data = 0;

    MSD_DBG_INFO(("Willow_gptpSetProtPTPEn Called.\n"));

    retval = gptpGetReg(dev, port, 0x0, &data);
    if (retval != MSD_OK)
    {
        MSD_DBG_ERROR(("Get the PTP register error\n"));
        return retval;
    }

    if (en)
        data &= ~0x1;
    else
        data |= 0x1;

    retval = gptpSetReg(dev, port, 0x0, data);
    if (retval != MSD_OK)
    {
        MSD_DBG_ERROR(("Set the PTP register error\n"));
        return retval;
    }

    MSD_DBG_INFO(("Willow_gptpSetProtPTPEn Exit.\n"));
    return retval;
}

MSD_STATUS Willow_gptpGetProtPTPEn
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_LPORT port,
    OUT MSD_BOOL  *en
)
{
    MSD_STATUS  retval;
    MSD_U32 data = 0;

    MSD_DBG_INFO(("Willow_gptpGetProtPTPEn Called.\n"));

    retval = gptpGetReg(dev, port, 0x0, &data);
    if (retval != MSD_OK)
    {
        MSD_DBG_ERROR(("Get the PTP register error\n"));
        return retval;
    }

    *en = (data & 0x1) ? MSD_FALSE : MSD_TRUE;

    MSD_DBG_INFO(("Willow_gptpGetProtPTPEn Exit.\n"));
    return retval;
}

MSD_STATUS Willow_gptpSetPortConfig
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_LPORT port,
    IN  WILLOW_PTP_PORT_CONFIG *ptpData
)
{
    MSD_STATUS retVal;
    MSD_U32 data = 0;
    MSD_DBG_INFO(("Willow_gptpSetPortConfig Called.\n"));

    if (ptpData->transSpec > 0xF)    /* 4 bits */
        return MSD_BAD_PARAM;

    if (ptpData->etJump > 0x1F)    /* 5 bits */
        return MSD_BAD_PARAM;

    if (ptpData->ipJump > 0x3F)    /* 6 bits */
        return MSD_BAD_PARAM;

    /* TransSpec, DisTSpecCheck, DisTSOverwrite bit, offset 0 */
    retVal = gptpGetReg(dev, port, 0x0, &data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Get the PTP register error\n"));
        return retVal;
    }
    data = (ptpData->transSpec << 12) | (data & 0x1) |
        ((ptpData->disTSpec ? 1 : 0) << 11) |
        ((ptpData->disTSOverwrite ? 1 : 0) << 1);

    retVal = gptpSetReg(dev, port, 0x0, data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Set the PTP register error\n"));
        return retVal;
    }

    /* setting etJump and ipJump, offset 1 */
    data = (ptpData->ipJump << 8) | ptpData->etJump;
    retVal = gptpSetReg(dev, port, 0x1, data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Set the PTP register error\n"));
        return retVal;
    }

    /* setting Int, offset 2 */
    retVal = gptpGetReg(dev, port, 0x2, &data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Get the PTP register error\n"));
        return retVal;
    }
    data &= 0x18;
    data |= (ptpData->ptpArrIntEn ? 1 : 0) |
        ((ptpData->ptpDepIntEn ? 1 : 0) << 1);
    data |= ((ptpData->arrTSMode & 0xff) << 8);
    data |= ((ptpData->filterAct ? 1 : 0) << 7); /* Filter 802.1 Act from LEDs */
    data |= ((ptpData->hwAccPtp ? 1 : 0) << 6);  /* Hardware Accelerated PTP */
    data |= ((ptpData->kpFramwSa ? 1 : 0) << 5); /* KS = Keep Frame SA */
    data |= ((ptpData->ExthwAcc ? 1 : 0) << 2); /*Exterbak Hardware Acceleration enable*/
    retVal = gptpSetReg(dev, port, 0x2, data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Set the PTP register error\n"));
        return retVal;
    }

    /* setting Led control, offset 3 */
    data = (ptpData->arrLedCtrl << 8) |
        (ptpData->depLedCtrl);
    retVal = gptpSetReg(dev, port, 0x3, data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Set the PTP register error\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gptpSetPortConfig Exit.\n"));
    return retVal;
}

MSD_STATUS Willow_gptpGetPortConfig
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_LPORT port,
    OUT WILLOW_PTP_PORT_CONFIG *ptpData
)
{
    MSD_STATUS retVal;
    MSD_U32 data = 0;
    MSD_DBG_INFO(("Willow_gptpGetPortConfig Called.\n"));

    /* TransSpec, DisTSpecCheck, DisTSOverwrite bit, offset 0 */
    retVal = gptpGetReg(dev, port, 0x0, &data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Get the PTP register error\n"));
        return retVal;
    }
    ptpData->transSpec = data >> 12;
    ptpData->disTSpec = ((data >> 11) & 0x1) ? MSD_TRUE : MSD_FALSE;
    ptpData->disTSOverwrite = ((data >> 1) & 0x1) ? MSD_TRUE : MSD_FALSE;

    /* getting MsgIDTSEn, offset 1 */
    retVal = gptpGetReg(dev, port, 0x1, &data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Get the PTP register error\n"));
        return retVal;
    }
    ptpData->ipJump = (data >> 8) & 0x3F;
    ptpData->etJump = data & 0x1F;

    /* getting TSArrPtr, offset 2 */
    retVal = gptpGetReg(dev, port, 0x2, &data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Get the PTP register error\n"));
        return retVal;
    }
    ptpData->ptpDepIntEn = ((data >> 1) & 0x1) ? MSD_TRUE : MSD_FALSE;
    ptpData->ptpArrIntEn = (data & 0x1) ? MSD_TRUE : MSD_FALSE;
    ptpData->arrTSMode = (data & 0xff00) >> 8;
    ptpData->filterAct = ((data >> 7) & 0x1) ? MSD_TRUE : MSD_FALSE;
    ptpData->hwAccPtp = ((data >> 6) & 0x1) ? MSD_TRUE : MSD_FALSE;
    ptpData->kpFramwSa = ((data >> 5) & 0x1) ? MSD_TRUE : MSD_FALSE;
    ptpData->ExthwAcc = ((data >> 2) & 0x1) ? MSD_TRUE : MSD_FALSE;

    /* getting Led control, offset 3 */
    retVal = gptpGetReg(dev, port, 0x3, &data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Get the PTP register error\n"));
        return retVal;
    }

    ptpData->arrLedCtrl = (data & 0xff00) >> 8;
    ptpData->depLedCtrl = data & 0xff;
    
    MSD_DBG_INFO(("Willow_gptpGetPortConfig Exit.\n"));
    return retVal;
}

MSD_STATUS Willow_gptpGetTimeStamp
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_LPORT    port,
    IN  WILLOW_PTP_TIME    timeToRead,
    OUT WILLOW_PTP_TS_STATUS    *ptpStatus
)
{
    MSD_STATUS retVal;
    MSD_U8 hwPort;
    WILLOW_MSD_PTP_OPERATION op;
    WILLOW_MSD_PTP_OP_DATA opData;
    MSD_U32 baseReg;

    MSD_DBG_INFO(("Willow_gptpGetTimeStamp Called.\n"));

    /* translate logical port to physical port */
    hwPort = MSD_LPORT_2_PORT(port);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    baseReg = 8;

    switch (timeToRead)
    {
    case WILLOW_PTP_ARR0_TIME:
        opData.ptpAddr = baseReg + 0;
        break;
    case WILLOW_PTP_ARR1_TIME:
        opData.ptpAddr = baseReg + 4;
        break;
    case WILLOW_PTP_DEP_TIME:
        opData.ptpAddr = baseReg + 8;
        break;
    default:
        MSD_DBG_ERROR(("Invalid time to be read\n"));
        return MSD_BAD_PARAM;
    }

    opData.ptpPort = hwPort;
    opData.ptpBlock = WILLOW_MSD_AVB_BLOCK_PTP;


    op = WILLOW_PTP_READ_MULTIPLE_DATA;
    opData.nData = 4;

    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed ptpOperation.\n"));
        return retVal;
    }

    ptpStatus->isValid = (opData.ptpMultiData[0] & 0x1) ? MSD_TRUE : MSD_FALSE;
    ptpStatus->status = (MSD_PTP_INT_STATUS)((opData.ptpMultiData[0] >> 1) & 0x3);
    ptpStatus->timeStamped = WILLOWMSD_PTP_BUILD_TIME(opData.ptpMultiData[2], opData.ptpMultiData[1]);
    ptpStatus->ptpSeqId = opData.ptpMultiData[3];

    MSD_DBG_INFO(("Willow_gptpGetTimeStamp Exit.\n"));
    return retVal;
}

MSD_STATUS Willow_gptpGetTSValidSt
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_LPORT    port,
    IN  MSD_PTP_TIME    timeToRead,
    OUT MSD_BOOL    *isValid
)
{
    MSD_STATUS  retVal;
    MSD_U32 data = 0;
    MSD_U32 baseReg;
    MSD_DBG_INFO(("Willow_gptpGetTSValidSt Called.\n"));

    baseReg = 8;

    switch (timeToRead)
    {
    case WILLOW_PTP_ARR0_TIME:
        baseReg = baseReg + 0;
        break;
    case WILLOW_PTP_ARR1_TIME:
        baseReg = baseReg + 4;
        break;
    case WILLOW_PTP_DEP_TIME:
        baseReg = baseReg + 8;
        break;
    default:
        MSD_DBG_ERROR(("Invalid time to get valid\n"));
        return MSD_BAD_PARAM;
    }

    retVal = gptpGetReg(dev, port, baseReg, &data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Get the PTP register error\n"));
        return retVal;
    }

    *isValid = (data & 0x1) ? MSD_TRUE : MSD_FALSE;
    MSD_DBG_INFO(("Willow_gptpGetTSValidSt Exit.\n"));
    return retVal;
}

MSD_STATUS Willow_gptpResetTimeStamp
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_LPORT    port,
    IN  MSD_PTP_TIME    timeToReset
)
{
    MSD_STATUS  retVal;
    MSD_U32 baseReg;
    int index;
    MSD_DBG_INFO(("Willow_gptpResetTimeStamp Called.\n"));

    switch (timeToReset)
    {
    case WILLOW_PTP_ARR0_TIME:
        baseReg = 8;
        break;
    case WILLOW_PTP_ARR1_TIME:
        baseReg = 12;
        break;
    case WILLOW_PTP_DEP_TIME:
        baseReg = 16;
        break;
    default:
        MSD_DBG_ERROR(("Invalid time to reset\n"));
        return MSD_BAD_PARAM;
    }

    for (index = 0; index < 4; index++)
    {
        MSD_U32 tempAddr = baseReg + index;
        retVal = gptpSetReg(dev, port, tempAddr, 0);
        if (retVal != MSD_OK)
        {
            MSD_DBG_ERROR(("Set the PTP register error\n"));
            return retVal;
        }
    }

    MSD_DBG_INFO(("Willow_gptpResetTimeStamp Exit.\n"));
    return retVal;
}

MSD_STATUS Willow_gptpSetGlobalConfig
(
    IN  MSD_QD_DEV* dev,
    IN  WILLOW_PTP_GLOBAL_CONFIG *ptpData
)
{
    MSD_STATUS retVal;
    WILLOW_MSD_PTP_OPERATION op;
    WILLOW_MSD_PTP_OP_DATA opData;
    MSD_DBG_INFO(("Willow_gptpResetTimeStamp Called.\n"));

    /* setting PTPEType, offset 0 */
    opData.ptpBlock = WILLOW_MSD_AVB_BLOCK_PTP;    /* PTP register space */
    opData.ptpPort = 0x1F;    /* Global register */
    op = WILLOW_PTP_WRITE_DATA;

    /* setting PTPEType, offset 0 */
    opData.ptpAddr = 0;
    opData.ptpData = ptpData->ptpEType;

    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_INFO(("Failed writing PTPEType.\n"));
        return retVal;
    }

    /* setting MsgIDTSEn, offset 1 */
    opData.ptpAddr = 1;
    opData.ptpData = ptpData->msgIdTSEn;

    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_INFO(("Failed writing MsgIDTSEn.\n"));
        return retVal;
    }

    /* setting TSArrPtr, offset 2 */
    opData.ptpAddr = 2;
    opData.ptpData = ptpData->tsArrPtr;

    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_INFO(("Failed writing TSArrPtr.\n"));
        return retVal;
    }
    
    MSD_DBG_INFO(("Willow_gptpResetTimeStamp Exit.\n"));
    return retVal;
}

MSD_STATUS Willow_gptpGetGlobalConfig
(
    IN  MSD_QD_DEV* dev,
    OUT WILLOW_PTP_GLOBAL_CONFIG    *ptpData
)
{
    MSD_STATUS retVal;
    WILLOW_MSD_PTP_OPERATION op;
    WILLOW_MSD_PTP_OP_DATA opData;
    MSD_DBG_INFO(("Willow_gptpGetGlobalConfig Called.\n"));

    opData.ptpBlock = WILLOW_MSD_AVB_BLOCK_PTP;    /* PTP register space */
    opData.ptpPort = 0x1F;    /* Global register */
    op = WILLOW_PTP_READ_DATA;

    /* getting PTPEType, offset 0 */
    opData.ptpAddr = 0;
    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_INFO(("Failed reading PTPEType.\n"));
        return retVal;
    }

    ptpData->ptpEType = opData.ptpData;

    /* getting MsgIDTSEn, offset 1 */
    opData.ptpAddr = 1;
    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_INFO(("Failed reading MsgIDTSEn.\n"));
        return retVal;
    }

    ptpData->msgIdTSEn = opData.ptpData;

    /* getting TSArrPtr, offset 2 */
    opData.ptpAddr = 2;
    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_INFO(("Failed reading TSArrPtr.\n"));
        return retVal;
    }

    ptpData->tsArrPtr = opData.ptpData;

    MSD_DBG_INFO(("Willow_gptpGetGlobalConfig Exit.\n"));
    return retVal;
}

MSD_STATUS Willow_gptpGetIntStatus
(
    IN  MSD_QD_DEV* dev,
    OUT WILLOW_PTP_INTERRUPT_STATUS *ptpIntSt
)
{
    MSD_STATUS retVal;
    WILLOW_MSD_PTP_OPERATION op;
    WILLOW_MSD_PTP_OP_DATA opData;
    MSD_DBG_INFO(("Willow_gptpGetIntStatus Called.\n"));

    opData.ptpBlock = 0;
    opData.ptpPort = 0x1F;    /* Global register */
    op = WILLOW_PTP_READ_DATA;
    opData.ptpAddr = 8;

    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed reading PTP status.\n"));
        return retVal;
    }

    ptpIntSt->ptpIntStPortVect = (opData.ptpData & ((1 << 11) - 1));
    ptpIntSt->ptpTrigGenInt = (opData.ptpData >> 15) & 1;
    ptpIntSt->ptpEventInt = (opData.ptpData >> 14) & 1;

    MSD_DBG_INFO(("Willow_gptpGetIntStatus Exit.\n"));
    return retVal;
}

MSD_STATUS Willow_gptpSetMeanPathDelay
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_LPORT portNum,
    IN  MSD_U16	delay
)
{
    MSD_STATUS retVal;
    MSD_DBG_INFO(("Willow_gptpSetMeanPathDelay Called.\n"));

    retVal = gptpSetReg(dev, portNum, 0x1c, delay);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Writing mean path delay error\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gptpSetMeanPathDelay Exit.\n"));
    return retVal;
}

MSD_STATUS Willow_gptpGetMeanPathDelay
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_LPORT portNum,
    OUT MSD_U16	*delay
)
{
    MSD_STATUS retVal;
    MSD_U32 data;
    MSD_DBG_INFO(("Willow_gptpGetMeanPathDelay Called.\n"));

    retVal = gptpGetReg(dev, portNum, 0x1c, &data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Reading mean path delay error\n"));
        return retVal;
    }

    *delay = (MSD_U16)data;

    MSD_DBG_INFO(("Willow_gptpGetMeanPathDelay Exit.\n"));
    return retVal;
}

MSD_STATUS Willow_gptpSetIgrPathDelayAsym
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_LPORT portNum,
    IN  MSD_U16	asym
)
{
    MSD_STATUS retVal;
    MSD_DBG_INFO(("Willow_gptpSetIgrPathDelayAsym Called.\n"));

    retVal = gptpSetReg(dev, portNum, 0x1d, asym);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Writing ingress path delay asym error\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gptpSetIgrPathDelayAsym Exit.\n"));
    return retVal;
}

MSD_STATUS Willow_gptpGetIgrPathDelayAsym
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_LPORT portNum,
    OUT MSD_U16	*asym
)
{
    MSD_STATUS retVal;
    MSD_U32 data;
    MSD_DBG_INFO(("Willow_gptpGetIgrPathDelayAsym Called.\n"));

    retVal = gptpGetReg(dev, portNum, 0x1d, &data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Reading ingress path delay asym error\n"));
        return retVal;
    }

    *asym = (MSD_U16)data;

    MSD_DBG_INFO(("Willow_gptpGetIgrPathDelayAsym Exit.\n"));
    return retVal;
}

MSD_STATUS Willow_gptpSetEgrPathDelayAsym
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_LPORT portNum,
    IN  MSD_U16	asym
)
{
    MSD_STATUS retVal;
    MSD_DBG_INFO(("Willow_gptpSetEgrPathDelayAsym Called.\n"));

    retVal = gptpSetReg(dev, portNum, 0x1e, asym);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Writing egress path delay asym error\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gptpSetEgrPathDelayAsym Exit.\n"));
    return retVal;
}

MSD_STATUS Willow_gptpGetEgrPathDelayAsym
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_LPORT portNum,
    OUT MSD_U16	*asym
)
{
    MSD_STATUS retVal;
    MSD_U32 data;
    MSD_DBG_INFO(("Willow_gptpGetEgrPathDelayAsym Called.\n"));

    retVal = gptpGetReg(dev, portNum, 0x1e, &data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Reading egress path delay asym error\n"));
        return retVal;
    }

    *asym = (MSD_U16)data;

    MSD_DBG_INFO(("Willow_gptpGetEgrPathDelayAsym Exit.\n"));
    return retVal;
}

MSD_STATUS Willow_gptpSetMode
(
    IN  MSD_QD_DEV* dev,
    IN  WILLOW_PTP_MODE_CONFIG *mode
)
{
    MSD_STATUS retVal;
    MSD_U8 data;

    MSD_DBG_INFO(("Willow_gptpSetMode Called.\n"));
    if ((retVal = gptpGlobalPointDataGet(dev, 0, &data)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed get PTP point/data.\n"));
        return retVal;
    }
    data &= 0xe0;
    data = (data |
        ((mode->altScheme & 1) << 4) |
        ((mode->grandMstr & 1) << 3) |
        ((mode->oneStep & 1) << 2) |
        (mode->PTPMode & 3));

    if ((retVal = gptpGlobalPointDataSet(dev, 0, data)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed set PTP point/data.\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gptpSetMode Exit.\n"));
    return retVal;
}

MSD_STATUS Willow_gptpGetMode
(
    IN  MSD_QD_DEV* dev,
    OUT WILLOW_PTP_MODE_CONFIG *mode
)
{
    MSD_STATUS retVal;
    MSD_U8 data;

    MSD_DBG_INFO(("Willow_gptpGetMode Called.\n"));
    if ((retVal = gptpGlobalPointDataGet(dev, 0, &data)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed get PTP point/data.\n"));
        return retVal;
    }

    mode->altScheme = (data >> 4) & 1;
    mode->grandMstr = (data >> 3) & 1;
    mode->oneStep = (data >> 2) & 1;
    mode->PTPMode = data & 3;

    MSD_DBG_INFO(("Willow_gptpGetMode Exit.\n"));
    return retVal;
}

MSD_STATUS Willow_gptpSetMasterSlave
(
    IN  MSD_QD_DEV* dev,
    IN  WILLOW_PTP_MASTER_SLAVE role
)
{
    MSD_STATUS retVal;
    MSD_U8 data;
    MSD_U8 tmp;

    MSD_DBG_INFO(("Willow_gptpSetMasterSlave Called.\n"));
    if (role == WILLOW_PTP_ROLE_SLAVE)
        tmp = 0;
    else if (role == WILLOW_PTP_ROLE_MASTER)
        tmp = 1;
    else
    {
        MSD_DBG_ERROR(("Invalid role\n"));
        return MSD_BAD_PARAM;
    }

    if ((retVal = gptpGlobalPointDataGet(dev, 0, &data)) !=MSD_OK)
    {
        MSD_DBG_ERROR(("Failed get PTP point/data.\n"));
        return retVal;
    }
    data &= 0xf7;
    data = data | (tmp << 3);

    if ((retVal = gptpGlobalPointDataSet(dev, 0, data)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed set PTP point/data.\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gptpSetMasterSlave Exit.\n"));
    return retVal;
}

MSD_STATUS Willow_gptpGetMasterSlave
(
    IN  MSD_QD_DEV* dev,
    IN  WILLOW_PTP_MASTER_SLAVE *role
)
{
    MSD_STATUS retVal;
    MSD_U8 data;

    MSD_DBG_INFO(("Willow_gptpGetMasterSlave Called.\n"));
    if ((retVal = gptpGlobalPointDataGet(dev, 0, &data)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Failed get PTP point/data.\n"));
        return retVal;
    }

    *role = (data >> 3) & 1;
    MSD_DBG_INFO(("Willow_gptpGetMasterSlave Exit.\n"));
    return retVal;
}

MSD_STATUS Willow_gptpGetTODBusyBitStatus
(
    IN  MSD_QD_DEV* dev,
    OUT MSD_BOOL	*busyStatus
)
{
    MSD_U32 tmpData;
    MSD_STATUS retVal;

    MSD_DBG_INFO(("Willow_gptpGetTODBusyBitStatus Called.\n"));

    if (busyStatus == NULL)
    {
        MSD_DBG_ERROR(("Input param busyStatus in Willow_gptpGetTODBusyBitStatus is NULL. \n"));
        return MSD_BAD_PARAM;
    }

    retVal = gptpGetReg(dev, 0x1f, 0x12, &tmpData);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Get the PTP Tod Busy bit Failed.\n"));
        return retVal;
    }

    *busyStatus = tmpData & 0x8000 ? MSD_TRUE : MSD_FALSE;

    MSD_DBG_INFO(("Willow_gptpGetTODBusyBitStatus Exit.\n"));
    return retVal;
}

MSD_STATUS Willow_gptpTODStoreCompensation
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_U8 timeArrayIndex,
    IN  WILLOW_PTP_TIME_ARRAY *timeArray
)
{
    MSD_STATUS retVal;

    MSD_DBG_INFO(("Willow_gptpTODStoreCompensation Called.\n"));

    /*check the timeArrayIndex*/
    if (!(timeArrayIndex >= 0 && timeArrayIndex < 4))
    {
        MSD_DBG_ERROR(("Time array index is over range.\n"));
        return MSD_BAD_PARAM;
    }
    msdSemTake(dev->devNum, dev->ptpTodSem, OS_WAIT_FOREVER);

    retVal = gptpTimeArrayStore(dev, timeArrayIndex, WILLOW_PTP_TOD_STORE_COMP, timeArray);
    if (retVal != MSD_OK)
    {
        msdSemGive(dev->devNum, dev->ptpTodSem);
        return retVal;
    }

    msdSemGive(dev->devNum, dev->ptpTodSem);
    MSD_DBG_INFO(("Willow_gptpTODStoreCompensation Exit.\n"));
    return retVal;
}

MSD_STATUS Willow_gptpTODStoreAll
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_U8 timeArrayIndex,
    IN  WILLOW_PTP_TIME_ARRAY *timeArray
)
{
    MSD_STATUS retVal;

    MSD_DBG_INFO(("Willow_gptpTODStoreAll Called.\n"));

    /*check the timeArrayIndex*/
    if (!(timeArrayIndex >= 0 && timeArrayIndex < 4))
    {
        MSD_DBG_ERROR(("Time array index is over range.\n"));
        return MSD_BAD_PARAM;
    }
    msdSemTake(dev->devNum, dev->ptpTodSem, OS_WAIT_FOREVER);

    retVal = gptpTimeArrayStore(dev, timeArrayIndex, WILLOW_PTP_TOD_STORE_ALL, timeArray);
    if (retVal != MSD_OK)
    {
        msdSemGive(dev->devNum, dev->ptpTodSem);
        return retVal;
    }

    msdSemGive(dev->devNum, dev->ptpTodSem);
    MSD_DBG_INFO(("Willow_gptpTODStoreAll Exit.\n"));
    return retVal;
}

MSD_STATUS Willow_gptpTODCaptureAll
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_U8 timeArrayIndex,
    OUT WILLOW_PTP_TIME_ARRAY *timeArray
)
{
    MSD_STATUS retVal;

    MSD_DBG_INFO(("Willow_gptpTODCaptureAll Called.\n"));
    /*check the timeArrayIndex*/
    if (!(timeArrayIndex >= 0 && timeArrayIndex < 4))
    {
        MSD_DBG_ERROR(("Time array index is over range.\n"));
        return MSD_BAD_PARAM;
    }
    msdSemTake(dev->devNum, dev->ptpTodSem, OS_WAIT_FOREVER);

    retVal = gptpTimeArrayCapture(dev, timeArrayIndex, timeArray);
    if (retVal != MSD_OK)
    {
        msdSemGive(dev->devNum, dev->ptpTodSem);
        return retVal;
    }

    msdSemGive(dev->devNum, dev->ptpTodSem);
    MSD_DBG_INFO(("Willow_gptpTODCaptureAll Exit.\n"));
    return retVal;
}

MSD_STATUS Willow_gptpSetPulse
(
    IN  MSD_QD_DEV* dev,
    IN  WILLOW_PTP_PULSE_STRUCT *pulsePara
)
{
    MSD_STATUS retVal;
    MSD_U32 data;
    MSD_DBG_INFO(("Willow_gptpSetPulse Called.\n"));

    data = ((((pulsePara->ptpPulseWidth) & 0xf) << 12) |
        (((pulsePara->ptp1ppsWidthRange) & 7) << 8) |
        (((pulsePara->ptp1ppsPhase) & 1) << 3) |
        (pulsePara->ptp1ppsSelect & 0x3));
    retVal = gptpSetReg(dev, 0x1f, 0x1e, data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Set 1PPS error\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gptpSetPulse Exit.\n"));
    return retVal;
}

MSD_STATUS Willow_gptpGetPulse
(
    IN  MSD_QD_DEV* dev,
    OUT WILLOW_PTP_PULSE_STRUCT *pulsePara
)
{
    MSD_STATUS retVal;
    MSD_U32 data;
    MSD_DBG_INFO(("Willow_gptpGetPulse Called.\n"));

    retVal = gptpGetReg(dev, 0x1f, 0x1e, &data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Get 1PPS error\n"));
        return retVal;
    }

    pulsePara->ptpPulseWidth = (MSD_U8)((data >> 12) & 0xF);
    pulsePara->ptp1ppsWidthRange = (MSD_U8)((data >> 8) & 7);
    pulsePara->ptp1ppsPhase = (MSD_U8)((data >> 3) & 1);
    pulsePara->ptp1ppsSelect = (MSD_U8)(data & 0x3);

    MSD_DBG_INFO(("Willow_gptpGetPulse Exit.\n"));
    return retVal;
}

MSD_STATUS Willow_gtaiGetPTPGlobalTime
(
    IN  MSD_QD_DEV *dev,
    OUT MSD_U32 *ptpTime
)
{
    MSD_STATUS retVal;
    WILLOW_MSD_PTP_OP_DATA   opData;
    WILLOW_MSD_PTP_OPERATION op;
    MSD_DBG_INFO(("Willow_gtaiGetPTPGlobalTime Called.\n"));

    opData.ptpBlock = 0x0;    /* PTP register space */
    op = WILLOW_PTP_READ_MULTIPLE_DATA;
    opData.ptpPort = 0x1E; /* TAI Global */
    opData.ptpAddr = 14;
    opData.nData = 2;

    if ((retVal = ptpOperationPerform(dev, op, &opData)) != MSD_OK)
    {
        MSD_DBG_INFO(("Failed reading PTP Global Time.\n"));
        return MSD_FAIL;
    }

    *ptpTime = (opData.ptpMultiData[1] << 16) | opData.ptpMultiData[0];

    MSD_DBG_INFO(("Willow_gtaiGetPTPGlobalTime Exit.\n"));
    return retVal;
}
#endif
