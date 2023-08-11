#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"

/********************************************************************************
* msdEcid.c
*
* DESCRIPTION:
*       API definitions for Bridge Port Extender 
*
* DEPENDENCIES:
*
* FILE REVISION NUMBER:
*******************************************************************************/

#include "msdEcid.h"
#include "msdApiTypes.h"
#include "msdUtils.h"

/*******************************************************************************
* msdBpeEnableSet
*
* DESCRIPTION:
*       This routine is used to configure the device for Port Extender mode or not. After enable this mode,
*       users can operate ECID table, such as add/delete/move entries, etc.
*
* INPUTS:
*       en    - MSD_TRUE: enable the feature for CPU operation, MSD_FALSE: otherwise.
*
* OUTPUTS:
*       None
*
* RETURNS:
*       MSD_OK - On success
*		MSD_FAIL - On error
*		MSD_BAD_PARAM - If invalid parameter is given
*		MSD_NOT_SUPPORTED - Device not support
*
* COMMENTS:
*       calling this API will cause the E-CID portion of the ATU to be flused, so it's best to call
*       this API during dirver initialization.
*
*******************************************************************************/
MSD_STATUS msdBpeEnableSet
(
    IN  MSD_U8   devNum,
    IN  MSD_BOOL en
)
{
    MSD_QD_DEV* dev = sohoDevGet(devNum);
    if (NULL == dev)
    {
        MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
        return MSD_FAIL;
    }
    if (dev->SwitchDevObj.ECIDObj.gBpeEnableSet != NULL)
        return dev->SwitchDevObj.ECIDObj.gBpeEnableSet(dev, en);
    else
        return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdBpeEnableGet
*
* DESCRIPTION:
*       This routine get the status if the device is configured Port Extender mode or not
*
* INPUTS:
*       None
*
* OUTPUTS:
*       en    - MSD_TRUE: enable the feature for CPU operation, MSD_FALSE: otherwise.
*
* RETURNS:
*       MSD_OK - On success
*		MSD_FAIL - On error
*		MSD_BAD_PARAM - If invalid parameter is given
*		MSD_NOT_SUPPORTED - Device not support
*
* COMMENTS:
*       none
*
*******************************************************************************/
MSD_STATUS msdBpeEnableGet
(
    IN  MSD_U8   devNum,
    OUT MSD_BOOL *en
)
{
    MSD_QD_DEV* dev = sohoDevGet(devNum);
    if (NULL == dev)
    {
        MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
        return MSD_FAIL;
    }
    if (dev->SwitchDevObj.ECIDObj.gBpeEnableGet != NULL)
        return dev->SwitchDevObj.ECIDObj.gBpeEnableGet(dev, en);
    else
        return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdEcidEntryAdd
*
* DESCRIPTION:
*       This routine creates a new entry in the ECID table
*
* INPUTS:
*       ecidEntry - ECID entry to be added to ECID table
*
* OUTPUTS:
*       None
*
* RETURNS:
*       MSD_OK - On success
*		MSD_FAIL - On error
*		MSD_BAD_PARAM - If invalid parameter is given
*		MSD_NOT_SUPPORTED - Device not support
*       MSD_FEATURE_NOT_ENABLE - The Bpe feature not been enable
*
* COMMENTS:
*       none
*
*******************************************************************************/
MSD_STATUS msdEcidEntryAdd
(
    IN  MSD_U8  devNum,
    IN  MSD_ECID_ENTRY *ecidEntry
)
{
    MSD_QD_DEV* dev = sohoDevGet(devNum);
    if (NULL == dev)
    {
        MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
        return MSD_FAIL;
    }
    if (dev->SwitchDevObj.ECIDObj.gecidAddEntry != NULL)
        return dev->SwitchDevObj.ECIDObj.gecidAddEntry(dev, ecidEntry);
    else
        return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdEcidEntryAllDelete
*
* DESCRIPTION:
*       This routine delete all entries in the ECID table
*
* INPUTS:
*
*
* OUTPUTS:
*       None
*
* RETURNS:
*       MSD_OK - On success
*		MSD_FAIL - On error
*		MSD_BAD_PARAM - If invalid parameter is given
*		MSD_NOT_SUPPORTED - Device not support
*       MSD_FEATURE_NOT_ENABLE - The Bpe feature not been enable
*
* COMMENTS:
*       none
*
*******************************************************************************/
MSD_STATUS msdEcidEntryAllDelete
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
    if (dev->SwitchDevObj.ECIDObj.gecidFlushAll != NULL)
        return dev->SwitchDevObj.ECIDObj.gecidFlushAll(dev);
    else
        return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdEcidEntryMove
*
* DESCRIPTION:
*       This routine moves entries from a port to another
*
* INPUTS:
*       moveFrom - port where moving frome
*       moveTo   - port where moving to
*
* OUTPUTS:
*       None
*
* RETURNS:
*       MSD_OK - On success
*		MSD_FAIL - On error
*		MSD_BAD_PARAM - If invalid parameter is given
*		MSD_NOT_SUPPORTED - Device not support
*       MSD_FEATURE_NOT_ENABLE - The Bpe feature not been enable
*
* COMMENTS:
*       none
*
*******************************************************************************/
MSD_STATUS msdEcidEntryMove
(
    IN  MSD_U8  devNum,
    IN  MSD_LPORT moveFrom,
    IN  MSD_LPORT moveTo
)
{
    MSD_QD_DEV* dev = sohoDevGet(devNum);
    if (NULL == dev)
    {
        MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
        return MSD_FAIL;
    }
    if (dev->SwitchDevObj.ECIDObj.gecidMove != NULL)
        return dev->SwitchDevObj.ECIDObj.gecidMove(dev, moveFrom, moveTo);
    else
        return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdEcidEntryDelete
*
* DESCRIPTION:
*       This routine deletes one entry of the specified group in the ECID table
*
* INPUTS:
*       group  - ecid group
*       ecid   - ecid base value
*
* OUTPUTS:
*       None
*
* RETURNS:
*       MSD_OK - On success
*		MSD_FAIL - On error
*		MSD_BAD_PARAM - If invalid parameter is given
*		MSD_NOT_SUPPORTED - Device not support
*       MSD_FEATURE_NOT_ENABLE - The Bpe feature not been enable
*
* COMMENTS:
*       none
*
*******************************************************************************/
MSD_STATUS msdEcidEntryDelete
(
    IN  MSD_U8  devNum,
    IN  MSD_U8  group,
    IN  MSD_U16 ecid
)
{
    MSD_QD_DEV* dev = sohoDevGet(devNum);
    if (NULL == dev)
    {
        MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
        return MSD_FAIL;
    }
    if (dev->SwitchDevObj.ECIDObj.gecidFlushEntry != NULL)
        return dev->SwitchDevObj.ECIDObj.gecidFlushEntry(dev, group, ecid);
    else
        return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdEcidEntryNextGet
*
* DESCRIPTION:
*       Gets the next valid ECID entry starting from the specified group and ecid value
*
* INPUTS:
*       group  - ecid group
*       ecid   - ecid base value
*
* OUTPUTS:
*       ecidEntry - valid ECID entry
*
* RETURNS:
*       MSD_OK - On success
*		MSD_FAIL - On error
*		MSD_BAD_PARAM - If invalid parameter is given
*       MSD_NO_SUCH - No valid entry exist
*		MSD_NOT_SUPPORTED - Device not support
*       MSD_FEATURE_NOT_ENABLE - The Bpe feature not been enable
*
* COMMENTS:
*       none
*
*******************************************************************************/
MSD_STATUS msdEcidEntryNextGet
(
    IN  MSD_U8  devNum,
    IN  MSD_U8  group,
    IN  MSD_U16 ecid,
    OUT MSD_ECID_ENTRY* ecidEntry
)
{
    MSD_QD_DEV* dev = sohoDevGet(devNum);
    if (NULL == dev)
    {
        MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
        return MSD_FAIL;
    }
    if (dev->SwitchDevObj.ECIDObj.gecidGetEcidEntryNext != NULL)
        return dev->SwitchDevObj.ECIDObj.gecidGetEcidEntryNext(dev, group, ecid, ecidEntry);
    else
        return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdEcidEntryFind
*
* DESCRIPTION:
*       This routine check if the specified ecid of one group exist in ECID table, if exists, return the valid ECID entry
*
* INPUTS:
*       group  - ecid group
*       ecid   - ecid base value
*
* OUTPUTS:
*       ecidEntry - valid ECID entry
*       found     - MSD_TRUE, if the appropriate entry exists
*
* RETURNS:
*       MSD_OK - On success
*		MSD_FAIL - On error
*		MSD_BAD_PARAM - If invalid parameter is given
*		MSD_NOT_SUPPORTED - Device not support
*       MSD_FEATURE_NOT_ENABLE - The Bpe feature not been enable
*
* COMMENTS:
*       none
*
*******************************************************************************/
MSD_STATUS msdEcidEntryFind
(
    IN  MSD_U8  devNum,
    IN  MSD_U8  group,
    IN  MSD_U16 ecid,
    OUT MSD_ECID_ENTRY* ecidEntry,
    OUT MSD_BOOL* found
)
{
    MSD_QD_DEV* dev = sohoDevGet(devNum);
    if (NULL == dev)
    {
        MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
        return MSD_FAIL;
    }
    if (dev->SwitchDevObj.ECIDObj.gecidFindEcidEntry != NULL)
        return dev->SwitchDevObj.ECIDObj.gecidFindEcidEntry(dev, group, ecid, ecidEntry, found);
    else
        return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdEcidEntryCountGet
*
* DESCRIPTION:
*       This routine counts all entries in the ECID table
*
* INPUTS:
*       None
*
* OUTPUTS:
*       count - The number of current valid entries
*
* RETURNS:
*       MSD_OK - On success
*		MSD_FAIL - On error
*		MSD_BAD_PARAM - If invalid parameter is given
*		MSD_NOT_SUPPORTED - Device not support
*       MSD_FEATURE_NOT_ENABLE - The Bpe feature not been enable
*
* COMMENTS:
*       none
*
*******************************************************************************/
MSD_STATUS msdEcidEntryCountGet
(
    IN  MSD_U8  devNum,
    OUT MSD_U32 *count
)
{
    MSD_QD_DEV* dev = sohoDevGet(devNum);
    if (NULL == dev)
    {
        MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
        return MSD_FAIL;
    }
    if (dev->SwitchDevObj.ECIDObj.gecidGetEntryCount != NULL)
        return dev->SwitchDevObj.ECIDObj.gecidGetEntryCount(dev, count);
    else
        return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdEcidDump
*
* DESCRIPTION:
*       This routine finds all valid ECID entries and print it out
*
* INPUTS:
*
*
* OUTPUTS:
*       None
*
* RETURNS:
*       MSD_OK - On success
*		MSD_FAIL - On error
*		MSD_BAD_PARAM - If invalid parameter is given
*		MSD_NOT_SUPPORTED - Device not support
*       MSD_FEATURE_NOT_ENABLE - The Bpe feature not been enable
*
* COMMENTS:
*       none
*
*******************************************************************************/
MSD_STATUS msdEcidDump
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
    if (dev->SwitchDevObj.ECIDObj.gecidDump != NULL)
        return dev->SwitchDevObj.ECIDObj.gecidDump(dev);
    else
        return MSD_NOT_SUPPORTED;
}
#endif /*#if FEATURE_MARVELL_6321*/
