#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"

/*******************************************************************************
* Pearl_msdEEPROM.c
*
* DESCRIPTION:
*       API definitions for Marvell EEPROM related operation
*
* DEPENDENCIES:
*
* FILE REVISION NUMBER:
*******************************************************************************/

#include "Pearl_msdEEPROM.h"
#include "Pearl_msdApiInternal.h"
#include "Pearl_msdHwAccess.h"
#include "Pearl_msdDrvSwRegs.h"
#include "msdUtils.h"
#include "msdSem.h"
#include "msdHwAccess.h"

/****************************************************************************/
/* internal EEPROM related function declaration.                            */
/****************************************************************************/
static MSD_STATUS Pearl_waitBusyBitSelfClear
(
IN	MSD_QD_DEV	*dev
);
static MSD_STATUS Pearl_EEPROM_Write
(
IN	MSD_QD_DEV	*dev,
IN	MSD_U16	addr,
IN	MSD_U16	data
);
static MSD_STATUS Pearl_EEPROM_Read
(
IN	MSD_QD_DEV	*dev,
IN	MSD_U16	addr,
OUT	MSD_U16	*data
);
static MSD_STATUS Pearl_checkWriteEnable
(
IN	MSD_QD_DEV	*dev
);

/*******************************************************************************
* Pearl_EEPROMWrite
*
* DESCRIPTION:
*       This routine is used to write one word to EEPROM
*
*
* INPUTS:
*      addr - EEPROM address
*      data - data to be written
*
* OUTPUTS:
*        None.
*
* RETURNS:
*       MSD_OK   - on success
*       MSD_FAIL - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS Pearl_EEPROMWrite
(
IN  MSD_QD_DEV *dev,
IN  MSD_U16  addr,
IN  MSD_U16  data
)
{
	MSD_STATUS retVal;
	MSD_DBG_INFO(("Pearl_EEPROMWrite Called.\n"));

	msdSemTake(dev->devNum, dev->tblRegsSem, OS_WAIT_FOREVER);

	retVal = Pearl_EEPROM_Write(dev, addr, data);

	if (retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Pearl_EEPROMWrite returned: %d.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum, dev->tblRegsSem);
		return MSD_FAIL;
	}

	msdSemGive(dev->devNum, dev->tblRegsSem);
	MSD_DBG_INFO(("Pearl_EEPROMWrite Exit.\n"));
	return retVal;
}

/*******************************************************************************
* Pearl_EEPROMRead
*
* DESCRIPTION:
*       This routine is used to read one word from EEPROM
*
*
* INPUTS:
*      addr - EEPROM address
*
* OUTPUTS:
*      data - EEPROM Data
*
* RETURNS:
*       MSD_OK   - on success
*       MSD_FAIL - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS Pearl_EEPROMRead
(
	IN  MSD_QD_DEV *dev,
	IN  MSD_U16  addr,
	OUT MSD_U16  *data
)
{
	MSD_STATUS retVal;
	MSD_DBG_INFO(("Pearl_EEPROMRead Called.\n"));

	msdSemTake(dev->devNum, dev->tblRegsSem, OS_WAIT_FOREVER);

	retVal = Pearl_EEPROM_Read(dev, addr, data);

	if (retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Pearl_EEPROMRead returned: %d.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum, dev->tblRegsSem);
		return MSD_FAIL;
	}

	msdSemGive(dev->devNum, dev->tblRegsSem);
	MSD_DBG_INFO(("Pearl_EEPROMRead Exit.\n"));
	return retVal;
}

/****************************************************************************/
/* Internal functions.                                                      */
/****************************************************************************/
static MSD_STATUS Pearl_waitBusyBitSelfClear
(
	IN	MSD_QD_DEV	*dev
)
{
	MSD_STATUS retVal;
	MSD_U16 data = 1;

	while (data == 1)
	{
		retVal = msdGetAnyRegField(dev->devNum, PEARL_GLOBAL2_DEV_ADDR, 20, 15, 1, &data);
		if (retVal != MSD_OK)
			return MSD_FAIL;
	}

	return MSD_OK;
}
static MSD_STATUS Pearl_checkWriteEnable
(
IN	MSD_QD_DEV	*dev
)
{
	MSD_STATUS retVal;
	MSD_U16 data = 0;

	retVal = msdGetAnyRegField(dev->devNum, PEARL_GLOBAL2_DEV_ADDR, 20, 10, 1, &data);
	if (retVal != MSD_OK)
	{
		return MSD_FAIL;
	}

	if (data != 1)
	{
		MSD_DBG_ERROR(("Pearl_checkWriteEnable returned: %d.\n", msdDisplayStatus(retVal)));
		return MSD_FAIL;
	}

	return MSD_OK;
}
static MSD_STATUS Pearl_EEPROM_Write
(
	IN	MSD_QD_DEV	*dev,
	IN	MSD_U16	addr,
	IN	MSD_U16	data
)
{
	MSD_STATUS retVal;
	MSD_U16 reg14, reg15;
	
	retVal = Pearl_checkWriteEnable(dev);
	if (retVal != MSD_OK)
	{
		return MSD_FAIL;
	}

	retVal = Pearl_waitBusyBitSelfClear(dev);
	if (retVal != MSD_OK)
	{
		return MSD_FAIL;
	}

	reg15 = data;
	retVal = msdSetAnyReg(dev->devNum, PEARL_GLOBAL2_DEV_ADDR, 0x15, reg15);
	if (retVal != MSD_OK)
	{
		return MSD_FAIL;
	}

	reg14 = 0x8000 | (3 << 12) | (addr & 0xff);

	retVal = msdSetAnyReg(dev->devNum, PEARL_GLOBAL2_DEV_ADDR, 0x14, reg14);
	if (retVal != MSD_OK)
	{
		return MSD_FAIL;
	}

	retVal = Pearl_waitBusyBitSelfClear(dev);
	if (retVal != MSD_OK)
	{
		return MSD_FAIL;
	}

	return retVal;
}
static MSD_STATUS Pearl_EEPROM_Read
(
	IN	MSD_QD_DEV	*dev,
	IN	MSD_U16	addr,
	OUT	MSD_U16	*data
)
{
	MSD_STATUS retVal;
	MSD_U16 reg14, tmpData;
	
	retVal = Pearl_checkWriteEnable(dev);
	if (retVal != MSD_OK)
	{
		return MSD_FAIL;
	}

	retVal = Pearl_waitBusyBitSelfClear(dev);
	if (retVal != MSD_OK)
	{
		return MSD_FAIL;
	}

	reg14 = 0x8000 | (4 << 12) | (addr & 0xff);

	retVal = msdSetAnyReg(dev->devNum, PEARL_GLOBAL2_DEV_ADDR, 0x14, reg14);
	if (retVal != MSD_OK)
	{
		return MSD_FAIL;
	}
	
	retVal = Pearl_waitBusyBitSelfClear(dev);
	if (retVal != MSD_OK)
	{
		return MSD_FAIL;
	}

	retVal = msdGetAnyReg(dev->devNum, PEARL_GLOBAL2_DEV_ADDR, 0x15, &tmpData);
	if (retVal != MSD_OK)
	{
		return MSD_FAIL;
	}

	*data = tmpData;
	
	return retVal;

}
#endif /*#if FEATURE_MARVELL_6321*/
