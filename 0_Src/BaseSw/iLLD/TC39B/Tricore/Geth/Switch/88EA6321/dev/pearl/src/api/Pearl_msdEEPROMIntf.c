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

/*******************************************************************************
* Pearl_EEPROMWrite
*
* DESCRIPTION:
*       This routine is used to write one word EEPROM data
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
MSD_STATUS Pearl_EEPROMWriteIntf
(
IN  MSD_QD_DEV *dev,
IN  MSD_U16  addr,
IN  MSD_U16  data
)
{
	return Pearl_EEPROMWrite(dev, addr, data);
}
/*******************************************************************************
* Pearl_EEPROMRead
*
* DESCRIPTION:
*       This routine is used to read one word EEPROM data
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
MSD_STATUS Pearl_EEPROMReadIntf
(
IN  MSD_QD_DEV *dev,
IN  MSD_U16  addr,
OUT  MSD_U16  *data
)
{
	return Pearl_EEPROMRead(dev, addr, data);
}
#endif /*#if FEATURE_MARVELL_6321*/
