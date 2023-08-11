#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"
/*******************************************************************************
* msdBrgStu.h
*
* DESCRIPTION:
*       API/Structure definitions for Marvell STU functionality.
*
* DEPENDENCIES:
*       None.
*
* FILE REVISION NUMBER:
*******************************************************************************/

#ifndef __msdBrgStu_h
#define __msdBrgStu_h

#include "msdApiTypes.h"
#include "msdSysConfig.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
* msdStuEntryFind
*
* DESCRIPTION:
*       Find STU entry for a specific SID, it will return the entry, if found,
*       along with its associated data
*
* INPUTS:
*       devNum - physical device number
*       sid    - SID ID, valid value is 0 ~ 63
*
* OUTPUTS:
*       stuEntry - pointer to the STU entry structure
*       found    - 1/0 for found / not found
*
* RETURNS:
*       MSD_OK      - on success
*       MSD_FAIL    - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*
*******************************************************************************/
MSD_STATUS msdStuEntryFind
(
    IN  MSD_U8  devNum,
    IN  MSD_U16 sid,
    OUT MSD_STU_ENTRY *stuEntry,
    OUT MSD_BOOL *found
);

/*******************************************************************************
* msdStuEntryAdd
*
* DESCRIPTION:
*       Creates or update the entry in STU table based on user input.
*
* INPUTS:
*       stuEntry  - stu entry to insert to the STU.
*
* OUTPUTS:
*       None
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*
*******************************************************************************/
MSD_STATUS msdStuEntryAdd
(
    IN MSD_U8 devNum,
    IN MSD_STU_ENTRY *stuEntry
);

/*******************************************************************************
* msdStuEntryNextGet
*
* DESCRIPTION:
*       Gets next valid STU entry from the specified SID.
*
* INPUTS:
*       sid  - the SID to start the search
*
* OUTPUTS:
*       stuEntry - next valid stu entry
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*       MSD_NO_SUCH - no more entries
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS msdStuEntryNextGet
(
	IN  MSD_U8 devNum,
	IN  MSD_U16 sid,
	OUT MSD_STU_ENTRY *stuEntry
);

/*******************************************************************************
* msdStuEntryDelete
*
* DESCRIPTION:
*       Deletes STU entry specified by SID.
*
* INPUTS:
*       devNum - physical device number
*       sid    - SID ID, valid value is 0 ~ 63
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK      - on success
*       MSD_FAIL    - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdStuEntryDelete
(
    IN  MSD_U8  devNum,
    IN  MSD_U16 sid
);

/*******************************************************************************
* msdStuEntryCountGet
*
* DESCRIPTION:
*       Gets number of valid entries in the STU table
*
* INPUTS:
*       devNum - physical device number
*
* OUTPUTS:
*       count  - number of valid entries
*
* RETURNS:
*       MSD_OK      - on success
*       MSD_FAIL    - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*
*******************************************************************************/
MSD_STATUS msdStuEntryCountGet
(
    IN  MSD_U8  devNum,
    OUT MSD_U16 *count
);

/*******************************************************************************
* msdStuDump
*
* DESCRIPTION:
*       Finds all STU Entries and print it out.
*
* INPUTS:
*       devNum - physical device number
*
* OUTPUTS:
*       None
*
* RETURNS:
*       MSD_OK      - on success
*       MSD_FAIL    - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*
*******************************************************************************/
MSD_STATUS msdStuDump
(
    IN  MSD_U8  devNum
);
#ifdef __cplusplus
}
#endif

#endif /* __msdBrgStu_h */
#endif /*#if FEATURE_MARVELL_6321*/
