#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"

/*******************************************************************************
* Willow_msdEEPROM.c
*
* DESCRIPTION:
*       API definitions for Marvell EEPROM related operation
*
* DEPENDENCIES:
*
* FILE REVISION NUMBER:
*******************************************************************************/

#include "Willow_msdEEPROM.h"
#include "Willow_msdApiInternal.h"
#include "Willow_msdHwAccess.h"
#include "Willow_msdDrvSwRegs.h"
#include "msdUtils.h"
#include "msdSem.h"
#include "msdHwAccess.h"

/****************************************************************************/
/* internal IMP related function declaration.                                    */
/****************************************************************************/
static MSD_STATUS Willow_waitBusyBitSelfClear
(
IN	MSD_QD_DEV	*dev
);
static MSD_STATUS Willow_EEPROM_Write
(
IN	MSD_QD_DEV	*dev,
IN	MSD_U16	addr,
IN	MSD_U16	data
);
static MSD_STATUS Willow_EEPROM_Read
(
IN	MSD_QD_DEV	*dev,
IN	MSD_U16	addr,
OUT	MSD_U16	*data
);

/*******************************************************************************
* Willow_EEPROMWrite
*
* DESCRIPTION:
*       This routine is used to write one byte to EEPROM
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
MSD_STATUS Willow_EEPROMWrite
(
IN  MSD_QD_DEV *dev,
IN  MSD_U16  addr,
IN  MSD_U16  data
)
{
	MSD_STATUS retVal;
	MSD_DBG_INFO(("Willow_EEPROMWrite Called.\n"));

	msdSemTake(dev->devNum, dev->tblRegsSem, OS_WAIT_FOREVER);

	retVal = Willow_EEPROM_Write(dev, addr, data);

	if (retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_EEPROMWrite returned: %d.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum, dev->tblRegsSem);
		return MSD_FAIL;
	}

	msdSemGive(dev->devNum, dev->tblRegsSem);
	MSD_DBG_INFO(("Willow_EEPROMWrite Exit.\n"));
	return retVal;
}

/*******************************************************************************
* Willow_EEPROMRead
*
* DESCRIPTION:
*       This routine is used to read one byte from EEPROM
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
MSD_STATUS Willow_EEPROMRead
(
IN  MSD_QD_DEV *dev,
IN  MSD_U16  addr,
OUT MSD_U16  *data
)
{
	MSD_STATUS retVal;
	MSD_DBG_INFO(("Willow_EEPROMRead Called.\n"));

	msdSemTake(dev->devNum, dev->tblRegsSem, OS_WAIT_FOREVER);

	retVal = Willow_EEPROM_Read(dev, addr, data);

	if (retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Willow_EEPROMRead returned: %d.\n", msdDisplayStatus(retVal)));
		msdSemGive(dev->devNum, dev->tblRegsSem);
		return MSD_FAIL;
	}

	msdSemGive(dev->devNum, dev->tblRegsSem);
	MSD_DBG_INFO(("Willow_EEPROMRead Exit.\n"));
	return retVal;
}

/****************************************************************************/
/* Internal functions.                                                  */
/****************************************************************************/
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
IN	MSD_U16	data
)
{
	MSD_STATUS retVal;
	MSD_U16 reg14, reg15;
	
	retVal = Willow_waitBusyBitSelfClear(dev);
	if (retVal != MSD_OK)
	{
		return MSD_FAIL;
	}

	reg15 = addr;
	retVal = msdSetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, 0x15, reg15);
	if (retVal != MSD_OK)
	{
        msdRegisterAccessFlush(dev->devNum);
		return MSD_FAIL;
	}

	reg14 = 0x8000 | (3 << 12) | (1 << 10) | (data & 0xff);

	retVal = msdSetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, 0x14, reg14);
	if (retVal != MSD_OK)
	{
        msdRegisterAccessFlush(dev->devNum);
		return MSD_FAIL;
	}

	retVal = Willow_waitBusyBitSelfClear(dev);
	if (retVal != MSD_OK)
	{
        msdRegisterAccessFlush(dev->devNum);
		return MSD_FAIL;
	}
    retVal = msdRegisterAccessFlush(dev->devNum);
	return retVal;
}
static MSD_STATUS Willow_EEPROM_Read
(
IN	MSD_QD_DEV	*dev,
IN	MSD_U16	addr,
OUT MSD_U16	*data
)
{
	MSD_STATUS retVal;
	MSD_U16 reg14, reg15, tmpData;
	
	reg15 = addr;
	retVal = msdSetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, 0x15, reg15);
	if (retVal != MSD_OK)
	{
        msdRegisterAccessFlush(dev->devNum);
		return MSD_FAIL;
	}

	retVal = Willow_waitBusyBitSelfClear(dev);
	if (retVal != MSD_OK)
	{
        msdRegisterAccessFlush(dev->devNum);
		return MSD_FAIL;
	}

	reg14 = 0x8000 | (4 << 12);

	retVal = msdSetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, 0x14, reg14);
	if (retVal != MSD_OK)
	{
        msdRegisterAccessFlush(dev->devNum);
		return MSD_FAIL;
	}
	retVal = Willow_waitBusyBitSelfClear(dev);
	if (retVal != MSD_OK)
	{
        msdRegisterAccessFlush(dev->devNum);
		return MSD_FAIL;
	}

	retVal = msdGetAnyReg(dev->devNum, WILLOW_GLOBAL2_DEV_ADDR, 0x14, &tmpData);
    if (retVal != MSD_OK)
    {
        msdRegisterAccessFlush(dev->devNum);
        return retVal;
    }
    retVal = msdRegisterAccessFlush(dev->devNum);
	*data = tmpData & 0xFF;
	return retVal;

}
#endif
