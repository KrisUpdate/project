#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"

/********************************************************************************
* msdTCAMRC.c
*
* DESCRIPTION:
*       API definitions for TCAM range check
*
* DEPENDENCIES:
*
* FILE REVISION NUMBER:
*******************************************************************************/

#include "msdTCAMRC.h"
#include "msdApiTypes.h"
#include "msdUtils.h"

/*******************************************************************************
* msdTcamRCEntryAdd
*
* DESCRIPTION:
*       This routine loads a Range Check entry.
*
* INPUTS:
*       devNum	     - Device number
*		rcEntryNum   - Pointer to the desired entry of RC table
*		rcData       - Range Check entry Data
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS msdTcamRCEntryAdd
(
IN  MSD_U8  devNum,
IN  MSD_U32  rcEntryNum,
IN  MSD_TCAM_RC_DATA *rcData
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.TCAMRCObj.gtcamRCLoadEntry != NULL)
		return dev->SwitchDevObj.TCAMRCObj.gtcamRCLoadEntry(dev, rcEntryNum, rcData);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdTcamRCAllDelete
*
* DESCRIPTION:
*       This routine is to flush all TCAM Range Check entries.Based on the function 
*		of Entry Delete. Set TCAM Page2 Block7 Reg1 to 0x1F.
*
* INPUTS:
*       devNum - physical device number
*
* OUTPUTS:
*        None.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS msdTcamRCAllDelete
(
IN  MSD_U8  devNum
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.TCAMRCObj.gtcamRCFlushAllEntry != NULL)
		return dev->SwitchDevObj.TCAMRCObj.gtcamRCFlushAllEntry(dev);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdTcamRCEntryDelete
*
* DESCRIPTION:
*       This routine is to flush a single Range Checker entry. 
*
*
* INPUTS:
*       devNum      - physical device number
*       rcEntryNum  - pointer to the desired entry of RC table
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS msdTcamRCEntryDelete
(
IN  MSD_U8  devNum,
IN  MSD_U32 rcEntryNum
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.TCAMRCObj.gtcamRCFlushEntry != NULL)
		return dev->SwitchDevObj.TCAMRCObj.gtcamRCFlushEntry(dev, rcEntryNum);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdTcamRCEntryRead
*
* DESCRIPTION:
*       This routine reads the TCAM RC entry.
*
*
* INPUTS:
*       devNum      - physical device number
*       rcEntryNum  - pointer to the desired entry of RC table
*
* OUTPUTS:
*       rcData     - RC entry Data
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS msdTcamRCEntryRead
(
IN  MSD_U8  devNum,
IN  MSD_U32 rcEntryNum,
OUT MSD_TCAM_RC_DATA  *rcData
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.TCAMRCObj.gtcamRCReadEntry != NULL)
		return dev->SwitchDevObj.TCAMRCObj.gtcamRCReadEntry(dev, rcEntryNum, rcData);
	else
		return MSD_NOT_SUPPORTED;
}
/*******************************************************************************
* msdTcamRCEntryGetNext
*
* DESCRIPTION:
*       This routine finds the next higher RC Entry number that is valid starting 
*		from the specified RC entry.
*
* INPUTS:
*       devNum      - physical device number
*       rcEntryNum  - pointer to the desired entry of RC table
*
* OUTPUTS:
*       rcEntryNum  - pointer to the desired entry of RC table
*       rcData      - RC entry Data
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_NO_SUCH - no valid entry exist
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS msdTcamRCEntryGetNext
(
IN    MSD_U8  devNum,
INOUT MSD_U32 *rcEntryNum,
OUT   MSD_TCAM_RC_DATA *rcData
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.TCAMRCObj.gtcamRCGetNextEntry != NULL)
		return dev->SwitchDevObj.TCAMRCObj.gtcamRCGetNextEntry(dev, rcEntryNum, rcData);
	else
		return MSD_NOT_SUPPORTED;
}
/*******************************************************************************
* msdTcamRCEntryFind
*
* DESCRIPTION:
*       Finds the specified valid RC entry in ingress RC table.
*
* INPUTS:
*       devNum      - physical device number
*       rcEntryNum  - pointer to the desired entry of RC table
*
* OUTPUTS:
*       rcData      - RC entry Data
*       found    - MSD_TRUE, if the appropriate entry exists.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None
*******************************************************************************/
MSD_STATUS msdTcamRCEntryFind
(
IN  MSD_U8  devNum,
IN	MSD_U32 rcEntryNum,
OUT	MSD_TCAM_RC_DATA  *rcData,
OUT	MSD_BOOL *found
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.TCAMRCObj.gtcamRCFindEntry != NULL)
		return dev->SwitchDevObj.TCAMRCObj.gtcamRCFindEntry(dev, rcEntryNum, rcData, found);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdTcamRCDump
*
* DESCRIPTION:
*       Finds all valid RC entries and print it out.
*
* INPUTS:
*       devNum  - physical devie number
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS msdTcamRCDump
(
IN  MSD_U8  devNum
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.TCAMRCObj.gtcamRCEntryDump != NULL)
		return dev->SwitchDevObj.TCAMRCObj.gtcamRCEntryDump(dev);
	else
		return MSD_NOT_SUPPORTED;
}
#endif /*#if FEATURE_MARVELL_6321*/
