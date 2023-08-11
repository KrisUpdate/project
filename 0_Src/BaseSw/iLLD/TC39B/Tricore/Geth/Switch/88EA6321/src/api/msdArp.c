#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"

/********************************************************************************
* msdArp.c
*
* DESCRIPTION:
*       API definitions for ARP table 
*
* DEPENDENCIES:
*
* FILE REVISION NUMBER:
*******************************************************************************/

#include "msdArp.h"
#include "msdApiTypes.h"
#include "msdUtils.h"

/*******************************************************************************
* msdArpMcEntryAdd
*
* DESCRIPTION:
*       This routine loads an ARP entry for multicast.
*
* INPUTS:
*       devNum			- physical device number
*       arpEntryNum		- Pointer to the desired entry of ARP 
*       arpMcData		- For multicast, this data becomes the value for the number
*						  of frames to duplicate for each egress port as follows:
*						  arpMcData[0] = P0_DUP
*						  arpMcData[1] = P1_DUP
*						  arpMcData[2] = P2_DUP
*						  arpMcData[3] = P3_DUP
*						  arpMcData[4] = P4_DUP
*						  arpMcData[5] = P5_DUP
*						  arpMcData[6] = P6_DUP
*						  arpMcData[7] = P7_DUP
*						  arpMcData[8] = P8_DUP
*						  arpMcData[9] = P9_DUP
*						  arpMcData[10] = P10_DUP
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
MSD_STATUS msdArpMcEntryAdd
(
IN  MSD_U8   devNum,
IN  MSD_U32  arpEntryNum,
IN  MSD_ARP_MC_DATA  *arpMcData
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.ARPObj.garpLoadMcEntry != NULL)
		return dev->SwitchDevObj.ARPObj.garpLoadMcEntry(dev, arpEntryNum, arpMcData);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdArpUcEntryAdd
*
* DESCRIPTION:
*       This routine loads an ARP entry for unicast.
*
* INPUTS:
*       devNum			- physical device number
*       arpEntryNum		- Pointer to the desired entry of ARP
*       arpUcData		- For unicast, this field is the ARP DA MAC address which is 
*						  use to replace DA field of the Unicast routing frame.
*						  arpUcData[0]:
*										RouteDA[47:40]
*						  arpUcData[1]:
*										RouteDA[39:32]
*						  arpUcData[2]:
*										RouteDA[31:24]
*						  arpUcData[3]:
*										RouteDA[23:16]
*						  arpUcData[4]:
*										RouteDA[15:8]
*						  arpUcData[5]:
*										RouteDA[7:0]
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
MSD_STATUS msdArpUcEntryAdd
(
IN  MSD_U8   devNum,
IN  MSD_U32  arpEntryNum,
IN  MSD_ARP_UC_DATA  *arpUcData
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.ARPObj.garpLoadUcEntry != NULL)
		return dev->SwitchDevObj.ARPObj.garpLoadUcEntry(dev, arpEntryNum, arpUcData);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdArpEntryDelete
*
* DESCRIPTION:
*       This routine delete an ARP entry.
*
* INPUTS:
*       devNum			- physical device number
*       arpEntryNum		- Pointer to the desired entry of ARP
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
MSD_STATUS msdArpEntryDelete
(
IN  MSD_U8  devNum,
IN  MSD_U32  arpEntryNum
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.ARPObj.garpFlushEntry != NULL)
		return dev->SwitchDevObj.ARPObj.garpFlushEntry(dev, arpEntryNum);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdArpAllDelete
*
* DESCRIPTION:
*       This routine is to flush all entries. 
*
* INPUTS:
*       devNum			- physical device number
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
MSD_STATUS msdArpAllDelete
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

	if (dev->SwitchDevObj.ARPObj.garpFlushAll != NULL)
		return dev->SwitchDevObj.ARPObj.garpFlushAll(dev);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdArpEntryRead
*
* DESCRIPTION:
*       This routine reads the ARP entry.
*
* INPUTS:
*       devNum			- physical device number
*       arpEntryNum		- Pointer to the desired entry of ARP
*       arpData			- ARP entry Data
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
MSD_STATUS msdArpEntryRead
(
IN  MSD_U8  devNum,
IN  MSD_U32  arpEntryNum,
OUT MSD_ARP_DATA  *arpData
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.ARPObj.garpReadEntry != NULL)
		return dev->SwitchDevObj.ARPObj.garpReadEntry(dev, arpEntryNum, arpData);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdArpEntryGetNext
*
* DESCRIPTION:
*       This routine finds the next higher ARP Entry number that is valid starting 
*		from the specified ARP entry.
*
* INPUTS:
*       devNum			- physical device number
*       arpEntryNum		- Pointer to the desired entry of ARP
*
* OUTPUTS:
*       arpEntryNum		- Pointer to the desired entry of ARP
*       arpData			- ARP entry Data
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
MSD_STATUS msdArpEntryGetNext
(
IN		MSD_U8  devNum,
INOUT   MSD_U32  *arpEntryNum,
OUT		MSD_ARP_DATA  *arpData
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.ARPObj.garpGetNextEntry != NULL)
		return dev->SwitchDevObj.ARPObj.garpGetNextEntry(dev, arpEntryNum, arpData);
	else
		return MSD_NOT_SUPPORTED;
}
#endif /*#if FEATURE_MARVELL_6321*/
