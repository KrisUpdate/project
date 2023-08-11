#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"

/********************************************************************************
* msdEEPROM.c
*
* DESCRIPTION:
*       API definitions for EEPROM 
*
* DEPENDENCIES:
*
* FILE REVISION NUMBER:
*******************************************************************************/

#include "msdEEPROM.h"
#include "msdApiTypes.h"
#include "msdUtils.h"

/*******************************************************************************
* msdEEPROMWrite
*
* DESCRIPTION:
*       This routine is used to write data to EEPROM
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
MSD_STATUS msdEEPROMWrite
(
	IN  MSD_U8		devNum,
	IN  MSD_U16		addr,
	IN  MSD_U16		data
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.EEPROMObj.geepromWrite != NULL)
		return dev->SwitchDevObj.EEPROMObj.geepromWrite(dev, addr, data);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdEEPROMRead
*
* DESCRIPTION:
*       This routine is used to read one data from EEPROM
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
MSD_STATUS msdEEPROMRead
(
	IN  MSD_U8		devNum,
	IN  MSD_U16		addr,
	OUT MSD_U16		*data
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.EEPROMObj.geepromRead != NULL)
		return dev->SwitchDevObj.EEPROMObj.geepromRead(dev, addr, data);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdEEPROMChipSelSet
*
* DESCRIPTION:
*       This routine is used to select one external EERPOM chip to operation
*
*
* INPUTS:
*      chipSelect - selected EEPROM chip number
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
MSD_STATUS msdEEPROMChipSelSet
(
	IN  MSD_U8  devNum,
	IN  MSD_U8  chipSelect
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.EEPROMObj.geepromSetMChipSel != NULL)
		return dev->SwitchDevObj.EEPROMObj.geepromSetMChipSel(dev, chipSelect);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdEEPROMChipSelGet
*
* DESCRIPTION:
*       This routine is used to get which external EERPOM chip is actively been operated on
*
*
* INPUTS:
*      None
*
* OUTPUTS:
*      chipSelect - selected EEPROM chip number
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
MSD_STATUS msdEEPROMChipSelGet
(
	IN  MSD_U8		devNum,
	OUT  MSD_U8		*chipSelect
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.EEPROMObj.geepromGetMChipSel != NULL)
		return dev->SwitchDevObj.EEPROMObj.geepromGetMChipSel(dev, chipSelect);
	else
		return MSD_NOT_SUPPORTED;
}
#endif /*#if FEATURE_MARVELL_6321*/
