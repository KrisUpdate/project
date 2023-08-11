#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"
/*******************************************************************************
* Willow_msdIMP.h
*
* DESCRIPTION:
*       API/Structure definitions for Marvell IMP functionality.
*
* DEPENDENCIES:
*       None.
*
* FILE REVISION NUMBER:
*******************************************************************************/

#ifndef __Willow_msdIMP_h
#define __Willow_msdIMP_h

#include "msdApiTypes.h"
#include "msdSysConfig.h"

#ifdef __cplusplus
extern "C" {
#endif

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
);
MSD_STATUS Willow_impRunIntf
(
IN	MSD_QD_DEV	*dev,
IN	MSD_U32	addr
);
/*******************************************************************************
* Willow_impReset
*
* DESCRIPTION:
*       This routine is to reset Willow_imp
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
);
MSD_STATUS Willow_impResetIntf
(
IN	MSD_QD_DEV	*dev
);
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
);
MSD_STATUS Willow_impWriteEEPROMIntf
(
IN	MSD_QD_DEV	*dev,
IN	MSD_U16	addr,
IN	MSD_U8	data
);
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
);
MSD_STATUS Willow_impLoadToEEPROMIntf
(
IN	MSD_QD_DEV	*dev,
IN	MSD_U16	dataSize,
IN	MSD_U8*	data
);
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
);
MSD_STATUS Willow_impLoadToRAMIntf
(
IN	MSD_QD_DEV	*dev,
IN	MSD_U32	addr,
IN	MSD_U32	dataSize,
IN	MSD_U8*	data
);
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
);
MSD_STATUS Willow_impStopIntf
(
IN	MSD_QD_DEV	*dev
);
#ifdef __cplusplus
}
#endif

#endif /* __Willow_msdIMP_h */
#endif
