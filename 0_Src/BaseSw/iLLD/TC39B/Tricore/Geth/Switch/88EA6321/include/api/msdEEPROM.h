#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"
/*******************************************************************************
* msdEEPROM.h
*
* DESCRIPTION:
*       API/Structure definitions for Marvell EEPROM functionality.
*
* DEPENDENCIES:
*       None.
*
* FILE REVISION NUMBER:
*******************************************************************************/

#ifndef __msdEEPROM_h
#define __msdEEPROM_h

#include "msdApiTypes.h"
#include "msdSysConfig.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
* msdEEPROMWrite
*
* DESCRIPTION:
*       This routine is used to write EEPROM data
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
);

/*******************************************************************************
* msdEEPROMRead
*
* DESCRIPTION:
*       This routine is used to read EEPROM data
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
);

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
);

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
);

#ifdef __cplusplus
}
#endif

#endif /* __msdEEPROM_h */
#endif /*#if FEATURE_MARVELL_6321*/
