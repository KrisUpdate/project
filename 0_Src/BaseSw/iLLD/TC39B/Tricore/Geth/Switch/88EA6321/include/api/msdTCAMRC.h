#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"
/*******************************************************************************
* msdTCAMRC.h
*
* DESCRIPTION:
*       API/Structure definitions for Marvell TCAM range check functionality.
*
* DEPENDENCIES:
*       None.
*
* FILE REVISION NUMBER:
*******************************************************************************/

#ifndef __msdTCAMRC_h
#define __msdTCAMRC_h

#include "msdApiTypes.h"
#include "msdSysConfig.h"

#ifdef __cplusplus
extern "C" {
#endif

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
);
/*******************************************************************************
* msdTcamRCAllDelete
*
* DESCRIPTION:
*       This routine is to flush all TCAM Range Check entries.Based on the function
*		of Entry Delete, set Page 2 Block 7 Reg 1 to 0x1F.
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
);
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
);
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
);
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
);
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
);
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
);

#ifdef __cplusplus
}
#endif

#endif /* __msdTCAMRC_h */
#endif /*#if FEATURE_MARVELL_6321*/
