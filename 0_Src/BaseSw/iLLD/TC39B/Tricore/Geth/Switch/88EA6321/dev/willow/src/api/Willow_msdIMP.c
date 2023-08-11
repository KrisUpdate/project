#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"

/*******************************************************************************
* Willow_msdIMP.c
*
* DESCRIPTION:
*       API definitions for Marvell IMP related operation
*
* DEPENDENCIES:
*
* FILE REVISION NUMBER:
*******************************************************************************/

#include "Willow_msdIMP.h"
#include "Willow_msdApiInternal.h"
#include "Willow_msdHwAccess.h"
#include "Willow_msdDrvSwRegs.h"
#include "msdUtils.h"
#include "msdSem.h"
#include "msdHwAccess.h"

/****************************************************************************/
/* internal IMP related function declaration.                                    */
/****************************************************************************/
static MSD_STATUS Willow_IMP_Write
(
	IN	MSD_QD_DEV	*dev,
	IN	MSD_U16	pointer,
	IN	MSD_U16	data
);

static MSD_STATUS Willow_IMP_Read
(
	IN	MSD_QD_DEV	*dev,
	IN	MSD_U16	pointer,
	OUT	MSD_U16	*data
);

static MSD_STATUS Willow_IMP_De_Next
(
	IN	MSD_QD_DEV	*dev
);

static MSD_STATUS Willow_IMP_EXamine
(
	IN	MSD_QD_DEV	*dev
);

static MSD_STATUS Willow_IMP_Stop
(
	IN	MSD_QD_DEV	*dev
);

static MSD_STATUS Willow_waitBusyBitSelfClear
(
	IN	MSD_QD_DEV	*dev
);

static MSD_STATUS Willow_EEPROM_Write
(
	IN	MSD_QD_DEV	*dev,
	IN	MSD_U16	addr,
	IN	MSD_U8	data
);

static MSD_STATUS Willow_IMP_Download
(
	IN	MSD_QD_DEV	*dev,
	IN	MSD_U32	addr,
	IN	MSD_U32	datasize,
	IN	MSD_U8	*data
);

/*******************************************************************************
* Willow_impRun
*
* DESCRIPTION:
*       This routine is to run IMP with specified start address
*
*
* INPUTS:
*        addr - specified start address
*
* OUTPUTS:
*        None.
*
* RETURNS:
*       MSD_OK   - on success
*       MSD_FAIL - on error
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS Willow_impRun
(
	IN	MSD_QD_DEV	*dev,
	IN	MSD_U32	addr
)
{
	MSD_STATUS retVal;

	MSD_U16 pointer = 0x8;
	MSD_U16 data = 0x1;

	MSD_U16 pointer_AddrLo = 0x0A;
	MSD_U16 LowAddr = addr & 0xff;

	MSD_U16 pointer_AddrHi = 0x0B;
	MSD_U16 HighAddr = (addr & 0xff00) >> 8;

	MSD_DBG_INFO(("Willow_impRun Called.\n"));
	msdSemTake(dev->devNum,  dev->tblRegsSem, OS_WAIT_FOREVER);

	retVal = Willow_IMP_Stop(dev);
	if (retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_impRun Willow_IMP_Stop returned: %d.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->tblRegsSem);
		return MSD_FAIL;
	}

	retVal = Willow_IMP_Write(dev, pointer_AddrLo, LowAddr);
	retVal |= Willow_IMP_Write(dev, pointer_AddrHi, HighAddr);

	retVal |= Willow_IMP_EXamine(dev);

	if (retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_impRun Willow_IMP_EXamine returned: %d.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->tblRegsSem);
		return retVal;
	}

	retVal = Willow_IMP_Write(dev, pointer, data);

	if (retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_impRun Willow_IMP_Write returned: %d.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->tblRegsSem);
		return retVal;
	}

	msdSemGive(dev->devNum,  dev->tblRegsSem);
	MSD_DBG_INFO(("Willow_impRun Exit.\n"));

	return retVal;		
}

/*******************************************************************************
* Willow_impReset
*
* DESCRIPTION:
*       This routine is to reset imp
*
*
* INPUTS:
*        None.
*
* OUTPUTS:
*        None.
*
* RETURNS:
*       MSD_OK   - on success
*       MSD_FAIL - on error
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS Willow_impReset
(
	IN	MSD_QD_DEV	*dev
)
{
	MSD_STATUS retVal;
	MSD_U16 data;

	MSD_DBG_INFO(("Willow_impReset Called.\n"));

	msdSemTake(dev->devNum,  dev->tblRegsSem, OS_WAIT_FOREVER);

	/*Stop the CPU*/
	retVal = Willow_IMP_Stop(dev);
	if (retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_impReset Willow_IMP_Stop returned: %d.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->tblRegsSem);
		return retVal;
	}

	retVal = Willow_IMP_Write(dev, 0x2f, 0);
	if (retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_impReset Willow_IMP_Write returned: %d.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->tblRegsSem);
		return retVal;
	}

	retVal = Willow_IMP_Read(dev, 0x71, &data);
	if (retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_impReset Willow_IMP_Read returned: %d.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->tblRegsSem);
		return retVal;
	}

	data |= 0x40;
	retVal = Willow_IMP_Write(dev, 0x71, data);
	if (retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_impReset Willow_IMP_Write returned: %d.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->tblRegsSem);
		return retVal;
	}

	msdSemGive(dev->devNum,  dev->tblRegsSem);
	MSD_DBG_INFO(("Willow_impReset Exit.\n"));

	return retVal;
}

/*******************************************************************************
* Willow_impWriteEEPROM
*
* DESCRIPTION:
*       This routine is to write data to EEPROM
*
*
* INPUTS:
*        addr - EEPROM address
*        data - data to be written
*
* OUTPUTS:
*        None.
*
* RETURNS:
*       MSD_OK   - on success
*       MSD_FAIL - on error
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS Willow_impWriteEEPROM
(
	IN	MSD_QD_DEV	*dev,
	IN	MSD_U16	addr,
	IN	MSD_U8	data
)
{
	MSD_STATUS retVal;

	MSD_DBG_INFO(("Willow_impWriteEEPROM Called.\n"));

	msdSemTake(dev->devNum,  dev->tblRegsSem, OS_WAIT_FOREVER);

	retVal = Willow_IMP_Stop(dev);
	if (retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_impWriteEEPROM Willow_IMP_Stop returned: %d.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->tblRegsSem);
		return MSD_FAIL;
	}

	retVal = Willow_IMP_Write(dev, 0x2f, 0);
	if (retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_impWriteEEPROM Willow_IMP_Write 0x2f returned: %d.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->tblRegsSem);
		return retVal;
	}

	retVal = Willow_IMP_Write(dev, 0x70, 0x08);
	if (retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_impWriteEEPROM Willow_IMP_Write 0x70 returned: %d.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->tblRegsSem);
		return retVal;
	}

	retVal = Willow_EEPROM_Write(dev, addr, data);

	if (retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_impWriteEEPROM Willow_EEPROM_Write returned: %d.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->tblRegsSem);
		return MSD_FAIL;
	}

	msdSemGive(dev->devNum,  dev->tblRegsSem);
	MSD_DBG_INFO(("Willow_impWriteEEPROM Exit.\n"));

	return retVal;
}

/*******************************************************************************
* Willow_impLoadToEEPROM
*
* DESCRIPTION:
*       This routine is to load data to eeprom devices
*
*
* INPUTS:
*        datasize - data size
*        data - data to be written
*
* OUTPUTS:
*        None.
*
* RETURNS:
*       MSD_OK   - on success
*       MSD_FAIL - on error
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS Willow_impLoadToEEPROM
(
	IN	MSD_QD_DEV	*dev,
	IN	MSD_U16	dataSize,
	IN	MSD_U8*	data
)
{
	MSD_STATUS retVal;
	MSD_U16 Idx;

	MSD_DBG_INFO(("Willow_impLoadToEEPROM Called.\n"));

	msdSemTake(dev->devNum,  dev->tblRegsSem, OS_WAIT_FOREVER);

	retVal = Willow_IMP_Stop(dev);
	if (retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_impLoadToEEPROM Willow_IMP_Stop returned: %d.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->tblRegsSem);
		return MSD_FAIL;
	}

	retVal = Willow_IMP_Write(dev, 0x2f, 0);
	if (retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_impLoadToEEPROM Willow_IMP_Write 0x2f returned: %d.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->tblRegsSem);
		return retVal;
	}

	retVal = Willow_IMP_Write(dev, 0x70, 0x08);
	if (retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_impLoadToEEPROM Willow_IMP_Write 0x70 returned: %d.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->tblRegsSem);
		return retVal;
	}

	for (Idx = 0; Idx < dataSize; Idx++)
	{
		retVal = Willow_EEPROM_Write(dev, Idx, data[Idx]);
		if (retVal != MSD_OK)
		{
			MSD_DBG_ERROR(("Willow_impLoadToEEPROM Willow_EEPROM_Write returned: %d.\n", msdDisplayStatus(retVal)));
			msdSemGive(dev->devNum,  dev->tblRegsSem);
			return MSD_FAIL;
		}
	}
	retVal = Willow_waitBusyBitSelfClear(dev);
	if (retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_impLoadToEEPROM Willow_waitBusyBitSelfClear returned: %d.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->tblRegsSem);
		return MSD_FAIL;
	}

	msdSemGive(dev->devNum,  dev->tblRegsSem);
	MSD_DBG_INFO(("Willow_impLoadToEEPROM Exit.\n"));

	return retVal;
}

/*******************************************************************************
* Willow_impLoadToRAM
*
* DESCRIPTION:
*       This routine is to load data to memory
*
*
* INPUTS:
*        addr - start address
*        datasize - data size
*        data - data to be written
*
* OUTPUTS:
*        None.
*
* RETURNS:
*       MSD_OK   - on success
*       MSD_FAIL - on error
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS Willow_impLoadToRAM
(
	IN	MSD_QD_DEV	*dev,
	IN	MSD_U32	addr,
	IN	MSD_U32	dataSize,
	IN	MSD_U8*	data
)
{
	MSD_STATUS retVal;

	MSD_DBG_INFO(("Willow_impLoadToRAM Called.\n"));
	msdSemTake(dev->devNum,  dev->tblRegsSem, OS_WAIT_FOREVER);

	retVal = Willow_IMP_Stop(dev);

	/* work around*/
	retVal |= Willow_IMP_Write(dev, 0x2f, 0);
	retVal |= Willow_IMP_Write(dev, 0x70, 0x08);		
	if (retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_impLoadToRAM Willow_IMP_Write returned: %d.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->tblRegsSem);
		return MSD_FAIL;
	}

	retVal = Willow_IMP_Download(dev, addr, dataSize, data);
	if (retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_impLoadToRAM Willow_IMP_Download returned: %d.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->tblRegsSem);
		return MSD_FAIL;
	}

	msdSemGive(dev->devNum,  dev->tblRegsSem);
	MSD_DBG_INFO(("Willow_impLoadToRAM Exit.\n"));

	return retVal;
}


/****************************************************************************/
/* Internal functions.                                                  */
/****************************************************************************/
static MSD_STATUS Willow_IMP_Write
(
	IN	MSD_QD_DEV	*dev,
	IN	MSD_U16	pointer,
	IN	MSD_U16	data
)
{
	MSD_STATUS retVal;

	MSD_U8 regAddr = 0x13;
	data = 0x8000 | ((0x7F & pointer) << 8) | (data & 0xFF);

	retVal = msdSetAnyReg(dev->devNum,  WILLOW_GLOBAL2_DEV_ADDR, regAddr, data);
    if (retVal != MSD_OK)
    {
        msdRegisterAccessFlush(dev->devNum);
        return retVal;
    }
    retVal = msdRegisterAccessFlush(dev->devNum);
	return retVal;
}

static MSD_STATUS Willow_IMP_Read
(
	IN	MSD_QD_DEV	*dev,
	IN	MSD_U16	pointer,
	OUT	MSD_U16	*data
)
{
	MSD_STATUS retVal;
	MSD_U8 regAddr = 0x13;

	pointer = (pointer & 0x7f) << 8;
	retVal = msdSetAnyReg(dev->devNum,  WILLOW_GLOBAL2_DEV_ADDR, regAddr, pointer);
    if (retVal != MSD_OK)
    {
        msdRegisterAccessFlush(dev->devNum);
        return retVal;
    }
	retVal = msdGetAnyReg(dev->devNum,  WILLOW_GLOBAL2_DEV_ADDR, regAddr, data);
    if (retVal != MSD_OK)
    {
        msdRegisterAccessFlush(dev->devNum);
        return retVal;
    }
    retVal = msdRegisterAccessFlush(dev->devNum);
    *data = *data & 0xff;

	return retVal;
}

static MSD_STATUS Willow_IMP_De_Next
(
	IN	MSD_QD_DEV	*dev
)
{
	MSD_STATUS retVal;
	MSD_U16 pointer = 0x8;
	MSD_U16 data = 0x6;

	retVal = Willow_IMP_Write(dev, pointer, data);
	return retVal;
}

static MSD_STATUS Willow_IMP_EXamine
(
	IN	MSD_QD_DEV	*dev
)
{
	MSD_U16 pointer = 0x8;
	MSD_U16 data = 0x3;
	return Willow_IMP_Write(dev, pointer, data);
}

static MSD_STATUS Willow_IMP_Stop
(
	IN	MSD_QD_DEV	*dev
)
{
	MSD_STATUS retVal;

	MSD_U16 pointer = 0x8;
	MSD_U16 data = 0x2;

	retVal = Willow_IMP_Write(dev, pointer, data);

	pointer = 0x8;
	data = 0xf;
	Willow_IMP_Write(dev, pointer, data);

	Willow_IMP_Write(dev, 0x0A, 0x0);
	Willow_IMP_Write(dev, 0x0B, 0x0);
	Willow_IMP_EXamine(dev);

	return retVal;
}

static MSD_STATUS Willow_waitBusyBitSelfClear
(
	IN	MSD_QD_DEV	*dev
)
{
	MSD_STATUS retVal;

    retVal = Willow_msdWaitForBit(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, 20, 15, 0);
    if (retVal != MSD_OK)
    {
        msdRegisterAccessFlush(dev->devNum);
    }
    /* after this function the RMU flush function has to be called */
    /* it is intended that the wait command is still in the RMU buffer */
	return retVal;
}

static MSD_STATUS Willow_EEPROM_Write
(
	IN	MSD_QD_DEV	*dev,
	IN	MSD_U16	addr,
	IN	MSD_U8	data
)
{
	MSD_STATUS retVal;
	MSD_U16 reg14, reg15;

	retVal = Willow_waitBusyBitSelfClear(dev);
    /* if this functions returns a failure it has already done a flush */
	if (retVal != MSD_OK)
	{
		return MSD_FAIL;
	}

	reg15 = addr;
	retVal = msdSetAnyReg(dev->devNum,  WILLOW_GLOBAL2_DEV_ADDR, 0x15, reg15);
	if (retVal != MSD_OK)
	{
        msdRegisterAccessFlush(dev->devNum);
		return MSD_FAIL;
	}

	reg14 = 0x8000 | (3 << 12) | (1 << 10) | data;

	retVal = msdSetAnyReg(dev->devNum,  WILLOW_GLOBAL2_DEV_ADDR, 0x14, reg14);
	if (retVal != MSD_OK)
	{
        msdRegisterAccessFlush(dev->devNum);
		return MSD_FAIL;
	}
    retVal = msdRegisterAccessFlush(dev->devNum);
	return retVal;
}

static MSD_STATUS Willow_IMP_Download
(
	IN	MSD_QD_DEV	*dev,
	IN	MSD_U32	addr,
	IN	MSD_U32	datasize,
	IN	MSD_U8	*data
)
{
	MSD_STATUS retVal;
	MSD_U32 Idx;

	MSD_U16 pointer_AddrLo = 0x0A;
	MSD_U16 LowAddr = (MSD_U16)(addr & 0xff);

	MSD_U16 pointer_AddrHi = 0x0B;
	MSD_U16 HighAddr = (MSD_U16)((addr & 0xff00) >> 8);

	MSD_U16 pointer_WrData = 0x0c;

	retVal = Willow_IMP_Write(dev, pointer_AddrLo, LowAddr);
	retVal |= Willow_IMP_Write(dev, pointer_AddrHi, HighAddr);

	retVal |= Willow_IMP_EXamine(dev);

	if (retVal != MSD_OK)
	{
		return MSD_FAIL;
	}

	if (datasize > 0)
	{
		for (Idx = 0; Idx < datasize; Idx++)
		{
			Willow_IMP_Write(dev, pointer_WrData, (MSD_U16)data[Idx]);
			Willow_IMP_De_Next(dev);
		}
	}

	return MSD_OK;
}



/*******************************************************************************
* impStop
*
* DESCRIPTION:
*       This routine is to stop imp
*
*
* INPUTS:
*        None.
*
* OUTPUTS:
*        None.
*
* RETURNS:
*       MSD_OK   - on success
*       MSD_FAIL - on error
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS Willow_impStop
(
IN	MSD_QD_DEV	*dev
)
{
	MSD_STATUS retVal;

	MSD_DBG_INFO(("Willow_impStop Called.\n"));

	msdSemTake(dev->devNum,  dev->tblRegsSem, OS_WAIT_FOREVER);

	retVal = Willow_IMP_Stop(dev);

	if (retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_impStop Willow_IMP_Stop returned: %d.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum,  dev->tblRegsSem);
		return retVal;
	}

	msdSemGive(dev->devNum,  dev->tblRegsSem);
	MSD_DBG_INFO(("Willow_impStop Exit.\n"));

	return retVal;
}
#endif
