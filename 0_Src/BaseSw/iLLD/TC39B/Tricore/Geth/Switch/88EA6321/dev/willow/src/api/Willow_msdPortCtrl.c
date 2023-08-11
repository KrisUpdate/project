#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"

/********************************************************************************
* Willow_msdPortCtrl.c
*
* DESCRIPTION:
*       API implementation for switch port common control.
*
* DEPENDENCIES:
*
* FILE REVISION NUMBER:
*******************************************************************************/

#include "Willow_msdPortCtrl.h"
#include "Willow_msdApiInternal.h"
#include "Willow_msdHwAccess.h"
#include "Willow_msdDrvSwRegs.h"
#include "msdUtils.h"
#include "msdSem.h"
#include "msdHwAccess.h"


#define WILLOW_MSD_GET_RATE_LIMIT_PER_FRAME(_frames, _dec)    \
        ((_frames)?(1000000000 / (16 * (_frames)) + ((1000000000 % (16 * (_frames)) > (16*(_frames)/2))?1:0)):0)

#define WILLOW_MSD_GET_RATE_LIMIT_PER_BYTE(_kbps, _dec)    \
        ((_kbps)?((8000000*(_dec)) / (16 * (_kbps)) + ((8000000*(_dec)) % (16 * (_kbps))?1:0)):0)

static MSD_STATUS Willow_writeFlowCtrlReg
(
	IN MSD_QD_DEV	*dev, 
	IN MSD_LPORT	port,
	IN MSD_U8	pointer,
	IN MSD_U8	data
);
static MSD_STATUS Willow_readFlowCtrlReg
(
	IN MSD_QD_DEV	*dev,
	IN MSD_LPORT	port,
	IN MSD_U8	pointer,
	OUT MSD_U8	*data
);


/*******************************************************************************
* Willow_gvlnSetPortVid
*
* DESCRIPTION:
*       This routine Set the port default vlan id.
*
* INPUTS:
*       port - logical port number to set.
*       vid  - the port vlan id.
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gvlnSetPortVid
(
    IN MSD_QD_DEV    *dev,
    IN MSD_LPORT     port,
    IN MSD_U16       vid
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           phyPort;        /* Physical port.               */
	MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gvlnSetPortVid Called.\n"));
    phyPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, phyPort);
	if (phyPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}	

    if (vid > 0xFFF)
    {
        MSD_DBG_ERROR(("Bad vid. It should be within [0, 0xFFF]\n"));
        return MSD_BAD_PARAM;
    }

    retVal = msdSetAnyRegField(dev->devNum, phyAddr,WILLOW_QD_REG_PVID,0,12, vid);
    /* this will cause a flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to write WILLOW_QD_REG_PVID Register.\n"));
        return retVal;
    }
    MSD_DBG_INFO(("Willow_gvlnSetPortVid Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gvlnGetPortVid
*
* DESCRIPTION:
*       This routine Get the port default vlan id.
*
* INPUTS:
*       port - logical port number to set.
*
* OUTPUTS:
*       vid  - the port vlan id.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gvlnGetPortVid
(
	IN MSD_QD_DEV *dev,
	IN  MSD_LPORT port,
	OUT MSD_U16   *vid
)
{
	MSD_STATUS       retVal;         /* Functions return value.      */
	MSD_U16          data;           /* The register's read data.    */
	MSD_U8           phyPort;        /* Physical port.               */
	MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gvlnGetPortVid Called.\n"));

	phyPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, phyPort);
	if (phyPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}

	retVal = msdGetAnyRegField(dev->devNum,  phyAddr, WILLOW_QD_REG_PVID, 0, 12, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PVID Register.\n"));
        return retVal;
    }

    *vid = data;

    MSD_DBG_INFO(("Willow_gvlnGetPortVid Exit.\n"));
	return MSD_OK;
}

/********************************************************************
* Willow_gvlnSetPortVlanDot1qMode
*
* DESCRIPTION:
*       This routine sets the IEEE 802.1q mode for this port
*
* INPUTS:
*       port    - logical port number to set.
*       mode     - 802.1q mode for this port 
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gvlnSetPortVlanDot1qMode
(
    IN MSD_QD_DEV        *dev,
    IN MSD_LPORT     port,
    IN WILLOW_MSD_8021Q_MODE    mode
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           phyPort;        /* Physical port.               */
	MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gvlnSetPortVlanDot1qMode Called.\n"));

    phyPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, phyPort);
	if (phyPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}
    
    switch (mode)
    {
    case WILLOW_MSD_DISABLE:
    case WILLOW_MSD_FALLBACK:
    case WILLOW_MSD_CHECK:
    case WILLOW_MSD_SECURE:
        break;
    default:
        MSD_DBG_ERROR(("Failed (Bad Mode).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = msdSetAnyRegField(dev->devNum, phyAddr,WILLOW_QD_REG_PORT_CONTROL2,10,2,(MSD_U16)mode );
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to write WILLOW_QD_REG_PORT_CONTROL2 Register.\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gvlnSetPortVlanDot1qMode Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gvlnGetPortVlanDot1qMode
*
* DESCRIPTION:
*       This routine Willow_gets the IEEE 802.1q mode for this port.
*
* INPUTS:
*       port     - logical port number to Willow_get.
*
* OUTPUTS:
*       mode     - 802.1q mode for this port 
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gvlnGetPortVlanDot1qMode
(
    IN MSD_QD_DEV        *dev,
    IN  MSD_LPORT        port,
    OUT WILLOW_MSD_8021Q_MODE   *mode
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U16          data;           /* The register's read data.    */
    MSD_U8           phyPort;        /* Physical port.               */
	MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gvlnGetPortVlanDot1qMode Called.\n"));

    phyPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, phyPort);
	if (phyPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}

    if(mode == NULL)
    {
        MSD_DBG_ERROR(("Failed(bad mode input).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = msdGetAnyRegField(dev->devNum, phyAddr,WILLOW_QD_REG_PORT_CONTROL2,10,2, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PORT_CONTROL2 Register.\n"));
        return retVal;
    }

    *mode = data;
    MSD_DBG_INFO(("Willow_gvlnGetPortVlanDot1qMode Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtSetDiscardTagged
*
* DESCRIPTION:
*       When this bit is set to a one, all non-MGMT frames that are processed as 
*       Tagged will be discarded as they enter this switch port. Priority only 
*       tagged frames (with a VID of 0x000) are considered tagged.
*
* INPUTS:
*       port - the logical port number.
*       mode - MSD_TRUE to discard tagged frame, MSD_FALSE otherwise
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtSetDiscardTagged
(
    IN MSD_QD_DEV    *dev,
    IN MSD_LPORT     port,
    IN MSD_BOOL        mode
)
{
    MSD_U16          data;           
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
	MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetDiscardTagged Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
	if (hwPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}

    /* translate BOOL to binary */
    MSD_BOOL_2_BIT(mode, data);

    /* Set DiscardTagged. */
    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL2, 9, 1, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to write WILLOW_QD_REG_PORT_CONTROL2 Register.\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetDiscardTagged Exit.\n"));
    return retVal;
}

/*******************************************************************************
* Willow_gprtGetDiscardTagged
*
* DESCRIPTION:
*       This routine Willow_gets DiscardTagged bit for the given port
*
* INPUTS:
*       port  - the logical port number.
*
* OUTPUTS:
*       mode  - MSD_TRUE if DiscardTagged bit is set, MSD_FALSE otherwise
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtGetDiscardTagged
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT    port,
    OUT MSD_BOOL     *mode
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
	MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtGetDiscardTagged Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
	if (hwPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}

    /* Get the DiscardTagged. */
    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL2, 9, 1, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PORT_CONTROL2 Register.\n"));
        return retVal;
    }
    MSD_BIT_2_BOOL(data, *mode);

    MSD_DBG_INFO(("Willow_gprtGetDiscardTagged Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtSetDiscardUntagged
*
* DESCRIPTION:
*       When this bit is set to a one, all non-MGMT frames that are processed as 
*       Untagged will be discarded as they enter this switch port. Priority only 
*       tagged frames (with a VID of 0x000) are considered tagged.
*
* INPUTS:
*       port - the logical port number.
*       mode - MSD_TRUE to discard untagged frame, MSD_FALSE otherwise
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtSetDiscardUntagged
(
    IN MSD_QD_DEV    *dev,
    IN MSD_LPORT     port,
    IN MSD_BOOL        mode
)
{
    MSD_U16          data;           
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
	MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetDiscardUntagged Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
	if (hwPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}

    /* translate BOOL to binary */
    MSD_BOOL_2_BIT(mode, data);

    /* Set DiscardUnTagged. */
    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL2, 8, 1, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to write WILLOW_QD_REG_PORT_CONTROL2 Register.\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetDiscardUntagged Exit.\n"));
    return retVal;
}

/*******************************************************************************
* Willow_gprtGetDiscardUntagged
*
* DESCRIPTION:
*       This routine Willow_gets DiscardUntagged bit for the given port
*
* INPUTS:
*       port  - the logical port number.
*
* OUTPUTS:
*       mode  - MSD_TRUE if DiscardUntagged bit is set, MSD_FALSE otherwise
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtGetDiscardUntagged
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT    port,
    OUT MSD_BOOL     *mode
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
	MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtGetDiscardUnTagged Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
	if (hwPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}

    /* Get the DiscardUnTagged. */
    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL2, 8, 1, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PORT_CONTROL2 Register.\n"));
        return retVal;
    }

    MSD_BIT_2_BOOL(data, *mode);

    MSD_DBG_INFO(("Willow_gprtGetDiscardUnTagged Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtSetUnicastFloodBlock
*
* DESCRIPTION:
*       This routine set Forward Unknown mode of a switch port. 
*       When this mode is set to MSD_TRUE, normal switch operation occurs.
*       When this mode is set to MSD_FALSE, unicast frame with unknown DA addresses
*       will not egress out this port.
*
* INPUTS:
*       port - the logical port number.
*       mode - MSD_TRUE for normal switch operation or MSD_FALSE to do not egress out the unknown DA unicast frames
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtSetUnicastFloodBlock
(
    IN  MSD_QD_DEV   *dev,
    IN MSD_LPORT     port,
    IN MSD_BOOL      mode
)
{
    MSD_U16          data;           
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
	MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetUnicastFloodBlock Called.\n"));

    /* translate BOOL to binary */
    MSD_BOOL_2_BIT(mode, data);
    data ^= 1;

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
	if (hwPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}

    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL, 2, 1, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to write WILLOW_QD_REG_PORT_CONTROL Register.\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetUnicastFloodBlock Exit.\n"));
    return retVal;
}

/*******************************************************************************
* Willow_gprtGetUnicastFloodBlock
*
* DESCRIPTION:
*       This routine Willow_gets Forward Unknown mode of a switch port.
*       When this mode is set to MSD_TRUE, normal switch operation occurs.
*       When this mode is set to MSD_FALSE, unicast frame with unknown DA addresses
*       will not egress out this port.
*
* INPUTS:
*       port  - the logical port number.
*
* OUTPUTS:
*       mode - MSD_TRUE for normal switch operation or MSD_FALSE to do not egress out the unknown DA unicast frames
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtGetUnicastFloodBlock
(
	IN  MSD_QD_DEV   *dev,
	IN  MSD_LPORT    port,
	OUT MSD_BOOL     *mode
)
{
	MSD_STATUS       retVal;         /* Functions return value.      */
	MSD_U8           hwPort;         /* the physical port number     */
	MSD_U16          data;           /* to keep the read valve       */
	MSD_U8			phyAddr;

	MSD_DBG_INFO(("Willow_gprtGetUnicastFloodBlock Called.\n"));

	/* translate LPORT to hardware port */
	hwPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
	if (hwPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL, 2, 1, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PORT_CONTROL Register.\n"));
        return retVal;
    }
    data ^= 1;

    /* translate binary to BOOL  */
    MSD_BIT_2_BOOL(data, *mode);

    MSD_DBG_INFO(("Willow_gprtGetUnicastFloodBlock Exit.\n"));
	return MSD_OK;
}

/*******************************************************************************
* Willow_gprtSetMulticastFloodBlock
*
* DESCRIPTION:
*       When this bit is set to a one, normal switch operation will occurs and 
*       multicast frames with unknown DA addresses are allowed to egress out this 
*       port (assuming the VLAN settings allow the frame to egress this port too).
*       When this bit is cleared to a zero, multicast frames with unknown DA 
*       addresses will not egress out this port.
*
* INPUTS:
*       port - the logical port number.
*       mode - MSD_TRUE for normal switch operation or MSD_FALSE to do not egress out the unknown DA multicast frames
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtSetMulticastFloodBlock
(
    IN MSD_QD_DEV    *dev,
    IN MSD_LPORT     port,
    IN MSD_BOOL        mode
)
{
    MSD_U16          data;           
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
	MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetMulticastFloodBlock Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
	if(hwPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}

    /* translate BOOL to binary */
    MSD_BOOL_2_BIT(mode, data);

    data ^= 1;

    /* Set DefaultForward. */
    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL, 3, 1, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to write WILLOW_QD_REG_PORT_CONTROL Register.\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetMulticastFloodBlock Exit.\n"));
    return retVal;
}

/*******************************************************************************
* Willow_gprtGetMulticastFloodBlock
*
* DESCRIPTION:
*       This routine Willow_gets DefaultForward bit for the given port
*
* INPUTS:
*       port  - the logical port number.
*
* OUTPUTS:
*       mode - MSD_TRUE for normal switch operation or MSD_FALSE to do not egress out the unknown DA multicast frames
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtGetMulticastFloodBlock
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT    port,
    OUT MSD_BOOL     *mode
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
	MSD_U8			phyAddr;
    MSD_U16          data;           /* to keep the read valve       */

    MSD_DBG_INFO(("Willow_gprtGetMulticastFloodBlock Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
	if(hwPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}

    /* Get the DefaultForward. */
    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL, 3, 1, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PORT_CONTROL Register.\n"));
        return retVal;
    }

    data ^= 1;

    MSD_BIT_2_BOOL(data, *mode);

    MSD_DBG_INFO(("Willow_gprtGetMulticastFloodBlock Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtSetDiscardBCastMode
*
* DESCRIPTION:
*       This routine sets the Discard Broadcast mode.
*       If the mode is enabled, all the broadcast frames to the given port will 
*       be discarded.
*
* INPUTS:
*       port - logical port number
*       en - MSD_TRUE, to enable the mode,
*             MSD_FALSE, otherwise.
*
* OUTPUTS:
*       None
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtSetDiscardBCastMode
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT     port,
    IN  MSD_BOOL      en
)
{
    MSD_U16          data;           /* Used to poll the data */
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
	MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetDiscardBCastMode Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
	if (hwPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}

    /* translate BOOL to binary */
    MSD_BOOL_2_BIT(en, data);

    retVal = msdSetAnyRegField(dev->devNum, phyAddr, 0x19, 13, 1, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to write Register, phyAddr: %x, regAddr:%x.\n", phyAddr, 0x19));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetDiscardBCastMode Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtGetDiscardBCastMode
*
* DESCRIPTION:
*       This routine Willow_gets the Discard Broadcast Mode. If the mode is enabled,
*       all the broadcast frames to the given port will be discarded.
*
* INPUTS:
*       port - logical port number
*
* OUTPUTS:
*       en - MSD_TRUE, if enabled,
*             MSD_FALSE, otherwise.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtGetDiscardBCastMode
(
	IN  MSD_QD_DEV    *dev,
	IN  MSD_LPORT     port,
	OUT MSD_BOOL      *en
)
{
	MSD_U16          data;
	MSD_STATUS       retVal;         /* Functions return value.      */
	MSD_U8           hwPort;         /* the physical port number     */
	MSD_U8			phyAddr;

	MSD_DBG_INFO(("Willow_gprtGetDiscardBCastMode Called.\n"));

	/* translate LPORT to hardware port */
	hwPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
	if (hwPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}

	data = 0;

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, 0x19, 13, 1, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read Register, phyAddr: %x, regAddr:%x.\n", phyAddr, 0x19));
        return retVal;
    }

	MSD_BIT_2_BOOL(data, *en);

    MSD_DBG_INFO(("Willow_gprtGetDiscardBCastMode Exit.\n"));
	return MSD_OK;
}

/*******************************************************************************
* Willow_gprtSetIGMPSnoopEnable
*
* DESCRIPTION:
*       This routine sets the Enable/disable trapping all IPV4 IGMP packets to 
*       the CPU,based on their ingress port.
*
* INPUTS:
*       port - logical port number
*       en - MSD_TRUE, to enable the mode,
*             MSD_FALSE, otherwise.
*
* OUTPUTS:
*       None
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtSetIGMPSnoopEnable
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT     port,
    IN  MSD_BOOL      en
)
{
    MSD_U16          data;           /* Used to poll the data */
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetIGMPSnoopEnable Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    /* translate BOOL to binary */
    MSD_BOOL_2_BIT(en, data);

    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL, 10, 1, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to write WILLOW_QD_REG_CT_CONTROL Register.\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetIGMPSnoopEnable Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtGetIGMPSnoopEnable
*
* DESCRIPTION:
*       This routine gets the Enable/disable trapping all IPV4 IGMP packets to
*       the CPU,based on their ingress port.
*
* INPUTS:
*       port - logical port number
*
* OUTPUTS:
*       en - MSD_TRUE, if enabled,
*             MSD_FALSE, otherwise.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtGetIGMPSnoopEnable
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT     port,
    OUT MSD_BOOL      *en
)
{
    MSD_U16          data;
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtGetIGMPSnoopEnable Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    data = 0;

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL, 10, 1, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PORT_CONTROL Register.\n"));
        return retVal;
    }

    MSD_BIT_2_BOOL(data, *en);

    MSD_DBG_INFO(("Willow_gprtGetIGMPSnoopEnable Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtGetHeaderMode
*
* DESCRIPTION:
*       This routine gets the ingress and egress header mode of a switch 
*       port. 
*
* INPUTS:
*       port - logical port number
*
* OUTPUTS:
*       en - MSD_TRUE, if enabled,
*             MSD_FALSE, otherwise.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtGetHeaderMode
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT     port,
    OUT MSD_BOOL      *en
)
{
    MSD_U16          data;
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtGetHeaderMode Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    data = 0;

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL, 11, 1, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PORT_CONTROL Register.\n"));
        return retVal;
    }

    MSD_BIT_2_BOOL(data, *en);

    MSD_DBG_INFO(("Willow_gprtGetHeaderMode Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtSetHeaderMode
*
* DESCRIPTION:
*       This routine sets the ingress and egress header mode of a switch 
*       port. To be used only for port connected to cpu mainly used for layer3
*       32bit alignment.
*
* INPUTS:
*       port - logical port number
*       en - MSD_TRUE, to enable the mode,
*             MSD_FALSE, otherwise.
*
* OUTPUTS:
*       None
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtSetHeaderMode
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT     port,
    IN  MSD_BOOL      en
)
{
    MSD_U16          data;           /* Used to poll the data */
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetHeaderMode Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    /* translate BOOL to binary */
    MSD_BOOL_2_BIT(en, data);

    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL, 11, 1, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to write WILLOW_QD_REG_PORT_CONTROL Register.\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetHeaderMode Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtGetDropOnLock
*
* DESCRIPTION:
*       This routine gets the DropOnLock mode of specified port on specified device
*
* INPUTS:
*       port - logical port number
*
* OUTPUTS:
*       en - MSD_TRUE, if enabled,
*             MSD_FALSE, otherwise.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtGetDropOnLock
(
IN  MSD_QD_DEV    *dev,
IN  MSD_LPORT     port,
OUT MSD_BOOL      *en
)
{
    MSD_U16          data;
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtGetDropOnLock Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    data = 0;

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL, 14, 1, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PORT_CONTROL Register.\n"));
        return retVal;
    }

    MSD_BIT_2_BOOL(data, *en);

    MSD_DBG_INFO(("Willow_gprtGetDropOnLock Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtSetDropOnLock
*
* DESCRIPTION:
*       This routine sets the Drop on Lock. When set to one, Ingress frames
*       will be discarded if their SA field is not in the ATU's address database
*       of specified port on specified device
*
* INPUTS:
*       port - logical port number
*       en - MSD_TRUE, to enable the mode,
*             MSD_FALSE, otherwise.
*
* OUTPUTS:
*       None
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtSetDropOnLock
(
IN  MSD_QD_DEV    *dev,
IN  MSD_LPORT     port,
IN  MSD_BOOL      en
)
{
    MSD_U16          data;           /* Used to poll the data */
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetDropOnLock Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    /* translate BOOL to binary */
    MSD_BOOL_2_BIT(en, data);

    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL, 14, 2, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to write WILLOW_QD_REG_PORT_CONTROL Register.\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetDropOnLock Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtSetJumboMode
*
* DESCRIPTION:
*       This routine Set the max frame size allowed to be received and transmitted
*       from or to a given port.
*
* INPUTS:
*       port - the logical port number
*       mode - WILLOW_MSD_MTU_SIZE (1522, 2048, or 10240)
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtSetJumboMode
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT    port,
    IN  WILLOW_MSD_MTU_SIZE   mode
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
	MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetJumboMode Called.\n"));

    if (mode > WILLOW_MSD_MTU_SIZE_10240)
    {
        MSD_DBG_ERROR(("Bad Parameter\n"));
        return MSD_BAD_PARAM;
    }

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
	if (hwPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
    }

    switch (mode)
    {
    case WILLOW_MSD_MTU_SIZE_1522:
    case WILLOW_MSD_MTU_SIZE_2048:
    case WILLOW_MSD_MTU_SIZE_10240:
        break;
    default:
        MSD_DBG_ERROR(("Failed (Bad Mode).\n"));
        return MSD_BAD_PARAM;
    }

    /* Set the Jumbo Fram Size bit.               */
    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL2, 12, 2, (MSD_U16)mode);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to write WILLOW_QD_REG_PORT_CONTROL2 Register.\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetJumboMode Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtGetJumboMode
*
* DESCRIPTION:
*       This routine Willow_gets the max frame size allowed to be received and transmitted
*       from or to a given port.
*
* INPUTS:
*       port  - the logical port number.
*
* OUTPUTS:
*       mode - WILLOW_MSD_MTU_SIZE (1522, 2048, or 10240)
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtGetJumboMode
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT    port,
    OUT WILLOW_MSD_MTU_SIZE   *mode
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
	MSD_U8			phyAddr;
    MSD_U16          data;           /* to keep the read valve       */

    MSD_DBG_INFO(("Willow_gsysGetJumboMode Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
	if (hwPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}

    /* Get Jumbo Frame Mode.            */
    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL2, 12, 2, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PORT_CONTROL2 Register.\n"));
        return retVal;
    }

    *mode = (WILLOW_MSD_MTU_SIZE)data;

    MSD_DBG_INFO(("Willow_gsysGetJumboMode Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtSetLearnEnable
*
* DESCRIPTION:
*       This routine enables/disables automatic learning of new source MAC
*       addresses on the given port ingress
*
* INPUTS:
*       port - the logical port number.
*       mode - MSD_TRUE for enable or MSD_FALSE otherwise
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtSetLearnEnable
(
    IN MSD_QD_DEV    *dev,
    IN MSD_LPORT     port,
    IN MSD_BOOL      mode
)
{          
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
	MSD_U8			phyAddr;
    MSD_U16          pav;

    MSD_DBG_INFO(("Willow_gprtSetLearnEnable Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
	if(hwPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}

	/* Set the port's PAV to all zeros */
    if (mode)
        pav = (MSD_U16)(1 << port);
    else
        pav = 0;

    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PAV, 0, 11, pav);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to write WILLOW_QD_REG_PAV Register.\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetLearnEnable Exit.\n"));
    return retVal;
}

/*******************************************************************************
* Willow_gprtGetLearnEnable
*
* DESCRIPTION:
*       This routine Willow_gets LearnDisable setup
*
* INPUTS:
*       port  - the logical port number.
*
* OUTPUTS:
*       mode - MSD_TRUE: Learning disabled on the given port ingress frames,
*                 MSD_FALSE otherwise
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtGetLearnEnable
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT     port,
    OUT MSD_BOOL      *mode
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
	MSD_U8			phyAddr;
    MSD_U16          data;           /* to keep the read valve       */
    MSD_U16          pav;

    MSD_DBG_INFO(("Willow_gprtGetLearnEnable Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
	if(hwPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PAV, 0, 11, &pav);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PAV Register.\n"));
        return retVal;
    }
    if (pav)
		data = 1;
	else
		data = 0;

    /* translate binary to BOOL  */
    MSD_BIT_2_BOOL(data, *mode);

    MSD_DBG_INFO(("Willow_gprtGetLearnEnable Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtGetVTUPriorityOverride
(
IN  MSD_QD_DEV    *dev,
IN  MSD_LPORT     port,
OUT WILLOW_MSD_PRI_OVERRIDE  *mode
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8			phyAddr;
    MSD_U16          data;           /* to keep the read valve       */

    MSD_DBG_INFO(("Willow_gprtGetVTUPriorityOverride Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PRI_OVERRIDE, 10, 2, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PRI_OVERRIDE Register.\n"));
        return retVal;
    }

    *mode = (WILLOW_MSD_PRI_OVERRIDE)data;

    MSD_DBG_INFO(("Willow_gprtGetVTUPriorityOverride Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtSetVTUPriorityOverride
(
IN  MSD_QD_DEV    *dev,
IN  MSD_LPORT     port,
IN  WILLOW_MSD_PRI_OVERRIDE  mode
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8			phyAddr;
    MSD_U16          data;           /* to keep the read valve       */

    MSD_DBG_INFO(("Willow_gprtSetVTUPriorityOverride Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    switch (mode)
    {
    case WILLOW_PRI_OVERRIDE_NONE:
    case WILLOW_PRI_OVERRIDE_FRAME:
    case WILLOW_PRI_OVERRIDE_QUEUE:
    case WILLOW_PRI_OVERRIDE_FRAME_QUEUE:
        break;
    default:
        MSD_DBG_ERROR(("Failed (Bad Mode).\n"));
        return MSD_BAD_PARAM;
    }

    data = (MSD_U16)mode;
    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PRI_OVERRIDE, 10, 2, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PRI_OVERRIDE Register.\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetVTUPriorityOverride Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtGetSAPriorityOverride
(
IN  MSD_QD_DEV    *dev,
IN  MSD_LPORT     port,
OUT WILLOW_MSD_PRI_OVERRIDE  *mode
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8			phyAddr;
    MSD_U16          data;           /* to keep the read valve       */

    MSD_DBG_INFO(("Willow_gprtGetSAPriorityOverride Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PRI_OVERRIDE, 12, 2, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PRI_OVERRIDE.\n"));
        return retVal;
    }

    *mode = (WILLOW_MSD_PRI_OVERRIDE)data;

    MSD_DBG_INFO(("Willow_gprtGetSAPriorityOverride Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtSetSAPriorityOverride
(
IN  MSD_QD_DEV    *dev,
IN  MSD_LPORT     port,
IN  WILLOW_MSD_PRI_OVERRIDE  mode
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8			phyAddr;
    MSD_U16          data;           /* to keep the read valve       */

    MSD_DBG_INFO(("Willow_gprtSetSAPriorityOverride Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    switch (mode)
    {
    case WILLOW_PRI_OVERRIDE_NONE:
    case WILLOW_PRI_OVERRIDE_FRAME:
    case WILLOW_PRI_OVERRIDE_QUEUE:
    case WILLOW_PRI_OVERRIDE_FRAME_QUEUE:
        break;
    default:
        MSD_DBG_ERROR(("Failed (Bad Mode).\n"));
        return MSD_BAD_PARAM;
    }

    data = (MSD_U16)mode;
    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PRI_OVERRIDE, 12, 2, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PRI_OVERRIDE Register.\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetSAPriorityOverride Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtGetDAPriorityOverride
(
IN  MSD_QD_DEV    *dev,
IN  MSD_LPORT     port,
OUT WILLOW_MSD_PRI_OVERRIDE  *mode
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8			phyAddr;
    MSD_U16          data;           /* to keep the read valve       */

    MSD_DBG_INFO(("Willow_gprtGetDAPriorityOverride Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PRI_OVERRIDE, 14, 2, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PRI_OVERRIDE.\n"));
        return retVal;
    }

    *mode = (WILLOW_MSD_PRI_OVERRIDE)data;

    MSD_DBG_INFO(("Willow_gprtGetDAPriorityOverride Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtSetDAPriorityOverride
(
IN  MSD_QD_DEV    *dev,
IN  MSD_LPORT     port,
IN  WILLOW_MSD_PRI_OVERRIDE  mode
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8			phyAddr;
    MSD_U16          data;           /* to keep the read valve       */

    MSD_DBG_INFO(("Willow_gprtSetDAPriorityOverride Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    switch (mode)
    {
    case WILLOW_PRI_OVERRIDE_NONE:
    case WILLOW_PRI_OVERRIDE_FRAME:
    case WILLOW_PRI_OVERRIDE_QUEUE:
    case WILLOW_PRI_OVERRIDE_FRAME_QUEUE:
        break;
    default:
        MSD_DBG_ERROR(("Failed (Bad Mode).\n"));
        return MSD_BAD_PARAM;
    }

    data = (MSD_U16)mode;
    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PRI_OVERRIDE, 14, 2, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PRI_OVERRIDE Register.\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetDAPriorityOverride Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtGetMessagePort
*
* DESCRIPTION:
*       This routine gets message port mode for specific port on a specified device.
*
* INPUTS:
*       devNum  - physical devie number
*       portNum - logical port number
*
* OUTPUTS:
*       en - MSD_TRUE: enable Message Port, MSD_FALSE: otherwise
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*		None
*
*******************************************************************************/
MSD_STATUS Willow_gprtGetMessagePort
(
IN  MSD_QD_DEV    *dev,
IN  MSD_LPORT     port,
OUT MSD_BOOL  *en
)
{
    MSD_U16          data;
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtGetMessagePort Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL1, 15, 1, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PORT_CONTROL1 Register.\n"));
        return retVal;
    }

    MSD_BIT_2_BOOL(data, *en);

    MSD_DBG_INFO(("Willow_gprtGetMessagePort Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtSetMessagePort
*
* DESCRIPTION:
*       This routine Set message port for specific port on a specific device.
*       Learning and learn refresh message frames will be sent out to this port.
*
* INPUTS:
*       devNum  - physical devie number
*       portNum - logical port number
*       en - MSD_TRUE: enable Message Port, MSD_FALSE: otherwise
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
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtSetMessagePort
(
IN  MSD_QD_DEV    *dev,
IN  MSD_LPORT     port,
IN  MSD_BOOL  en
)
{
    MSD_U16          data;
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetMessagePort Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    MSD_BOOL_2_BIT(en, data);

    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL1, 15, 1, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to write WILLOW_QD_REG_PORT_CONTROL1 Register.\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetMessagePort Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtGetEtherType
*
* DESCRIPTION:
*       This routine gets the port's special Ether Type. This Ether Type is used
*       for Policy (see gprtSetPolicy API) and FrameMode (see gprtSetFrameMode
*       API) of specified port on specified device
*
* INPUTS:
*       devNum  - physical devie number
*       portNum - logical port number
*
* OUTPUTS:
*       eType - Ethernet type
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*		None
*
*******************************************************************************/
MSD_STATUS Willow_gprtGetEtherType
(
IN  MSD_QD_DEV    *dev,
IN  MSD_LPORT     port,
OUT MSD_U16  *eType
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtGetEtherType Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = msdGetAnyReg(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_ETH_TYPE, eType);
    if (retVal != MSD_OK)
    {
        msdRegisterAccessFlush(dev->devNum);
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PORT_ETH_TYPE Register.\n"));
        return retVal;
    }
    retVal = msdRegisterAccessFlush(dev->devNum);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to flush RMU for WILLOW_QD_REG_PORT_ETH_TYPE Register.\n"));
        return retVal;
    }
    MSD_DBG_INFO(("Willow_gprtGetEtherType Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtSetEtherType
*
* DESCRIPTION:
*       This routine sets the port's special Ether Type. This Ether Type is used
*       for Policy (see gprtSetPolicy API) and FrameMode (see gprtSetFrameMode API)
*       of specified port on specified device
*
* INPUTS:
*       devNum  - physical devie number
*       portNum - logical port number
*       eType - Ethernet type
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
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtSetEtherType
(
IN  MSD_QD_DEV    *dev,
IN  MSD_LPORT     port,
IN  MSD_U16  eType
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetEtherType Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = msdSetAnyReg(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_ETH_TYPE, eType);
    if (retVal != MSD_OK)
    {
        msdRegisterAccessFlush(dev->devNum);
        MSD_DBG_ERROR(("ERROR to write WILLOW_QD_REG_PORT_ETH_TYPE Register.\n"));
        return retVal;
    }
    retVal = msdRegisterAccessFlush(dev->devNum);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to flush RMU for WILLOW_QD_REG_PORT_ETH_TYPE Register.\n"));
        return retVal;
    }
    MSD_DBG_INFO(("Willow_gprtSetEtherType Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtGetAllowVidZero
*
* DESCRIPTION:
*       This routine Get allow VID of Zero mode of specified port on specified device
*
* INPUTS:
*       devNum  - physical devie number
*       portNum - logical port number
*
* OUTPUTS:
*       en - MSD_TRUE: allow VID of zero, MSD_FALSE: otherwise
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*		None
*
*******************************************************************************/
MSD_STATUS Willow_gprtGetAllowVidZero
(
IN  MSD_QD_DEV    *dev,
IN  MSD_LPORT     port,
OUT MSD_BOOL  *en
)
{
    MSD_U16          data;
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtGetAllowVidZero Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL2, 3, 1, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PORT_CONTROL2 Register.\n"));
        return retVal;
    }

    MSD_BIT_2_BOOL(data, *en);

    MSD_DBG_INFO(("Willow_gprtGetAllowVidZero Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtSetAllowVidZero
*
* DESCRIPTION:
*       This routine Set allow VID of Zero of specified port on specified device
*
* INPUTS:
*       devNum  - physical devie number
*       portNum - logical port number
*       en - MSD_TRUE: allow VID of zero, MSD_FALSE: otherwise
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
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtSetAllowVidZero
(
IN  MSD_QD_DEV    *dev,
IN  MSD_LPORT     port,
IN  MSD_BOOL  en
)
{
    MSD_U16          data;
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetAllowVidZero Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    MSD_BOOL_2_BIT(en, data);

    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL2, 3, 1, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to write WILLOW_QD_REG_PORT_CONTROL2 Register.\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetAllowVidZero Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtSetTrunkPort
*
* DESCRIPTION:
*       This function enables/disables and sets the trunk ID.
*        
* INPUTS:
*       port - the logical port number.
*       en - MSD_TRUE to make the port be a member of a trunk with the given trunkId.
*             MSD_FALSE, otherwise.
*       trunkId - valid ID is 0 ~ 31
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtSetTrunkPort
(
    IN MSD_QD_DEV    *dev,
    IN MSD_LPORT     port,
    IN MSD_BOOL      en,
    IN MSD_U32       trunkId
)
{
    MSD_U16          data;           
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
	MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetTrunkPort Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
	if(hwPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}

    /* translate BOOL to binary */
    MSD_BOOL_2_BIT(en, data);

    if(en == MSD_TRUE)
    {
        /* need to enable trunk. so check the trunkId */
        if (!WILLOW_IS_TRUNK_ID_VALID(dev, trunkId))
        {
            MSD_DBG_ERROR(("Failed(bad trunkId).\n"));
            return MSD_BAD_PARAM;
        }

        /* Set TrunkId. */
        retVal = msdSetAnyRegField(dev->devNum,  phyAddr, WILLOW_QD_REG_PORT_CONTROL1, 8, 5, (MSD_U16)trunkId);
        /* this will cause an automatic flush operation */
        if (retVal != MSD_OK)
        {
            MSD_DBG_ERROR(("ERROR to write WILLOW_QD_REG_PORT_CONTROL1 Register.\n"));
            return retVal;
        }
    }

    /* Set TrunkPort bit. */
    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL1, 14, 1, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to write WILLOW_QD_REG_PORT_CONTROL1 Register.\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetTrunkPort Exit.\n"));
    return retVal;
}

/*******************************************************************************
* Willow_gprtGetTrunkPort
*
* DESCRIPTION:
*       This function returns trunk state of the port.
*       When trunk is disabled, trunkId field won't have valid value.
*        
* INPUTS:
*       port - the logical port number.
*
* OUTPUTS:
*       en - MSD_TRUE, if the port is a member of a trunk,
*             MSD_FALSE, otherwise.
*       trunkId - 0 ~ 31, valid only if en is MSD_TRUE
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtGetTrunkPort
(
    IN MSD_QD_DEV    *dev,
    IN MSD_LPORT     port,
    OUT MSD_BOOL     *en,
    OUT MSD_U32        *trunkId
)
{
    MSD_U16          data;           
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
	MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtGetTrunkPort Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
	if(hwPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}

    data = 0;
    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL1, 14, 1, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PORT_CONTROL1 Register.\n"));
        return retVal;
    }

    MSD_BIT_2_BOOL(data, *en);

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL1, 8, 5, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PORT_CONTROL1 Register.\n"));
        return retVal;
    }

    *trunkId = (MSD_U32)data;

    MSD_DBG_INFO(("Willow_gprtGetTrunkPort Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtSetFlowCtrl
*
* DESCRIPTION:
*       This routine enable/disable port flow control and set flow control mode
*          mode - PORT_FC_TX_RX_ENABLED,
*                 PORT_RX_ONLY,
*                 PORT_TX_ONLY,
*                 PORT_PFC_ENABLED
*
* INPUTS:
*       port - the logical port number.
*		en - enable/disable the flow control
*       mode - flow control mode
*
* OUTPUTS:
*       None
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*        
* COMMENTS:
*        Set ForcedFC=1, FCValue = enable/disable, FCMode
*
*******************************************************************************/
MSD_STATUS Willow_gprtSetFlowCtrl
(
	IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT    port,
    IN  MSD_BOOL     en,
	IN  WILLOW_MSD_PORT_FC_MODE      mode
)
{
	MSD_STATUS       retVal;         /* Functions return value.      */
	MSD_U8			tmpData;

    MSD_DBG_INFO(("Willow_gprtSetFlowCtrl Called.\n"));

    switch (mode)
    {
    case Willow_PORT_FC_TX_RX_ENABLED:
    case Willow_PORT_TX_ONLY:
    case Willow_PORT_RX_ONLY:
    case Willow_PORT_PFC_ENABLED:
        break;
    default:
        MSD_DBG_ERROR(("Failed (Bad Mode).\n"));
        return MSD_BAD_PARAM;
    }

	retVal = Willow_readFlowCtrlReg(dev, port, 0x10, &tmpData);
	if(retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Willow_readFlowCtrlReg port:%x, reg:%x, returned: %s.\n", port, 0x10, msdDisplayStatus(retVal)));
        return retVal;
	}

	tmpData &= ~0x07;
	tmpData = (MSD_U8)(tmpData | (1 << 3 ) |((en == MSD_TRUE?1:0) << 2) | mode);

	retVal = Willow_writeFlowCtrlReg(dev, port, 0x10, tmpData);
	if(retVal != MSD_OK)
	{
        MSD_DBG_ERROR(("Willow_writeFlowCtrlReg port:%x, reg:%x, returned: %s.\n", port, 0x10, msdDisplayStatus(retVal)));
        return retVal;
	}

    MSD_DBG_INFO(("Willow_gprtSetFlowCtrl Exit.\n"));
	return retVal;
}

/*******************************************************************************
* Willow_gprtGetFlowCtrl
*
* DESCRIPTION:
*       This routine Willow_get switch port flow control enable/disable status and return 
*       flow control mode
*
* INPUTS:
*       port - the logical port number.
*
* OUTPUTS:
*		en - enable/disable the flow control
*       mode - flow control mode
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*        
*******************************************************************************/
MSD_STATUS Willow_gprtGetFlowCtrl
(
	IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT    port,
    OUT MSD_BOOL     *en,
	OUT WILLOW_MSD_PORT_FC_MODE      *mode
)
{
	MSD_U8			tmpData;
    MSD_STATUS       retVal;

    MSD_DBG_INFO(("Willow_gprtGetFlowCtrl Called.\n"));

	retVal = Willow_readFlowCtrlReg(dev, port, 0x10, &tmpData);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Willow_readFlowCtrlReg port:%x, reg:%x, returned: %s.\n", port, 0x10, msdDisplayStatus(retVal)));
        return retVal;
    }

	*en = (tmpData >> 2) & 0x1;
	*mode = (WILLOW_MSD_PORT_FC_MODE)(tmpData & 0x3); 

    MSD_DBG_INFO(("Willow_gprtGetFlowCtrl Exit.\n"));
	return retVal;
}

/*******************************************************************************
* Willow_gprtSetEgressMonitorSource
*
* DESCRIPTION:
*       When this bit is cleared to a zero, normal network switching occurs.
*       When this bit is set to a one, any frame that egresses out this port will
*       also be sent to the EgressMonitorDest Port
*
* INPUTS:
*       port - the logical port number.
*       mode - MSD_TRUE to set EgressMonitorSource, MSD_FALSE otherwise
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtSetEgressMonitorSource
(
    IN MSD_QD_DEV    *dev,
    IN MSD_LPORT     port,
    IN MSD_BOOL        mode
)
{
    MSD_U16          data;           
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
	MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetEgressMonitorSource Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
	if(hwPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}
    /* translate BOOL to binary */
    MSD_BOOL_2_BIT(mode, data);

    /* Set EgressMonitorSource. */
	retVal = msdSetAnyRegField(dev->devNum,  phyAddr, WILLOW_QD_REG_PORT_CONTROL2, 5, 1, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Set EgressMonitorSource bit error\n"));
        return retVal;
    }
    
    MSD_DBG_INFO(("Willow_gprtSetEgressMonitorSource Exit.\n"));
    return retVal;
}

/*******************************************************************************
* Willow_gprtGetEgressMonitorSource
*
* DESCRIPTION:
*       This routine Willow_gets EgressMonitorSource bit for the given port
*
* INPUTS:
*       port  - the logical port number.
*
* OUTPUTS:
*       mode  - MSD_TRUE if EgressMonitorSource bit is set, MSD_FALSE otherwise
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtGetEgressMonitorSource
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT    port,
    OUT MSD_BOOL     *mode
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
	MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtGetEgressMonitorSource Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
	if(hwPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}

    /* Get the EgressMonitorSource. */
    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL2, 5, 1, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Get EgressMonitorSource bit error\n"));
        return retVal;
    }

    MSD_BIT_2_BOOL(data, *mode);

    MSD_DBG_INFO(("Willow_gprtGetEgressMonitorSource Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtSetIngressMonitorSource
*
* DESCRIPTION:
*       When this be is cleared to a zero, normal network switching occurs.
*       When this bit is set to a one, any frame that ingresses in this port will
*       also be sent to the IngressMonitorDest Port
*
* INPUTS:
*       port - the logical port number.
*       mode - MSD_TRUE to set IngressMonitorSource, MSD_FALSE otherwise
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtSetIngressMonitorSource
(
    IN MSD_QD_DEV    *dev,
    IN MSD_LPORT     port,
    IN MSD_BOOL        mode
)
{
    MSD_U16          data;           
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
	MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetIngressMonitorSource Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
	if(hwPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}

    /* translate BOOL to binary */
    MSD_BOOL_2_BIT(mode, data);

    /* Set IngressMonitorSource. */
    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL2, 4, 1, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Set IngressMonitorSource bit error\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetIngressMonitorSource Exit.\n"));
    return retVal;
}

/*******************************************************************************
* Willow_gprtGetIngressMonitorSource
*
* DESCRIPTION:
*       This routine Willow_gets IngressMonitorSource bit for the given port
*
* INPUTS:
*       port  - the logical port number.
*
* OUTPUTS:
*       mode  - MSD_TRUE if IngressMonitorSource bit is set, MSD_FALSE otherwise
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtGetIngressMonitorSource
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT    port,
    OUT MSD_BOOL     *mode
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
	MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtGetIngressMonitorSource Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
	if(hwPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}

    /* Get the IngressMonitorSource. */
    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL2, 4, 1, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Get IngressMonitorSource bit error\n"));
        return retVal;
    }

    MSD_BIT_2_BOOL(data, *mode);

    MSD_DBG_INFO(("Willow_gprtGetIngressMonitorSource Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_grcSetEgressRate
*
* DESCRIPTION:
*       This routine sets the port's egress data limit.
*        
*
* INPUTS:
*       port      - logical port number.
*       rateType  - egress data rate limit (MSD_ERATE_TYPE union type). 
*                    union type is used to support multiple devices with the
*                    different formats of egress rate.
*                    MSD_ERATE_TYPE has the following fields:
*                        kbRate - rate in kbps that should used with the WILLOW_MSD_ELIMIT_MODE of 
*                                WILLOW_MSD_ELIMIT_LAYER1,
*                                WILLOW_MSD_ELIMIT_LAYER2, or 
*                                WILLOW_MSD_ELIMIT_LAYER3 (see Willow_grcSetELimitMode)
*                            64kbps ~ 1Mbps    : increments of 64kbps,
*                            1Mbps ~ 100Mbps   : increments of 1Mbps, and
*                            100Mbps ~ 1000Mbps: increments of 10Mbps
*                            1Gbps ~ 5Gbps: increments of 100Mbps
*                            Therefore, the valid values are:
*                                64, 128, 192, 256, 320, 384,..., 960,
*                                1000, 2000, 3000, 4000, ..., 100000,
*                                110000, 120000, 130000, ..., 1000000
*                                1100000, 1200000, 1300000, ..., 5000000.
*                        fRate - frame per second that should used with WILLOW_MSD_ELIMIT_MODE of 
*                                WILLOW_MSD_PIRL_ELIMIT_FRAME
*                            Valid values are between 3815 and 14880000
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_grcSetEgressRate
(
    IN MSD_QD_DEV       *dev,
    IN MSD_LPORT        port,
    IN WILLOW_MSD_ELIMIT_MODE mode,
    IN MSD_U32	rate
)
{
    MSD_STATUS    retVal;         /* Functions return value.      */
    MSD_U16        data;
    MSD_U32        eDec;
    MSD_U8        hwPort,phyAddr;        /* Physical port.               */

    MSD_DBG_INFO(("Willow_grcSetEgressRate Called.\n"));

    hwPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
	if(hwPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}

    switch (mode)
    {
    case WILLOW_MSD_ELIMIT_FRAME:
    case WILLOW_MSD_ELIMIT_LAYER1:
    case WILLOW_MSD_ELIMIT_LAYER2:
    case WILLOW_MSD_ELIMIT_LAYER3:
        break;
    default:
        MSD_DBG_ERROR(("Failed (Bad Mode).\n"));
        return MSD_BAD_PARAM;
    }

    if((retVal = Willow_grcSetELimitMode(dev,port,mode)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Willow_grcSetELimitMode returned: %s.\n", msdDisplayStatus(retVal)));
        return retVal;
    }

    if (mode == WILLOW_MSD_ELIMIT_FRAME)    
    {
        if (rate == 0) /* disable egress rate limit */
        {
            eDec = 0;
            data = 0;
        }
        else if((rate < 3815)  || (rate > 14880000))
        {
            MSD_DBG_ERROR(("Failed (Bad Rate).\n"));
            return MSD_BAD_PARAM;
        }
        else
        {
            eDec = 1;
            data = (MSD_U16)WILLOW_MSD_GET_RATE_LIMIT_PER_FRAME(rate,eDec);
        }
    }
    else
    {
        if(rate == 0)
        {
            eDec = 0;
        }
        else if(rate < 1000)    /* less than 1Mbps */
        {
            /* it should be divided by 64 */
            if (rate % 64)
            {
                MSD_DBG_ERROR(("Failed (Bad Rate).\n"));
                return MSD_BAD_PARAM;
            }
            eDec = rate/64;
        }
        else if(rate <= 100000)    /* less than or equal to 100Mbps */
        {
            /* it should be divided by 1000 */
            if (rate % 1000)
            {
                MSD_DBG_ERROR(("Failed (Bad Rate).\n"));
                return MSD_BAD_PARAM;
            }
            eDec = rate/1000;
        }
        else if(rate <= 1000000)    /* less than or equal to 1000Mbps */
        {
            /* it should be divided by 10000 */
            if (rate % 10000)
            {
                MSD_DBG_ERROR(("Failed (Bad Rate).\n"));
                return MSD_BAD_PARAM;
            }
            eDec = rate/10000;
        }
        else if(rate <= 5000000)    /* less than or equal to 10Gbps */
        {
            /* it should be divided by 100000 */
            if (rate % 100000)
            {
                MSD_DBG_ERROR(("Failed (Bad Rate).\n"));
                return MSD_BAD_PARAM;
            }
            eDec = rate/100000;
        }
        else
        {
            MSD_DBG_ERROR(("Failed (Bad Rate).\n"));
            return MSD_BAD_PARAM;
        }

        if(rate == 0)
        {
            data = 0;
        }
        else
        {
            data = (MSD_U16)WILLOW_MSD_GET_RATE_LIMIT_PER_BYTE(rate,eDec);
        }
    }

	retVal = msdSetAnyRegField(dev->devNum,  phyAddr, WILLOW_QD_REG_EGRESS_RATE_CTRL, 0, 7, (MSD_U16)eDec);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to write WILLOW_QD_REG_EGRESS_RATE_CTRL Register.\n"));
        return retVal;
    }

	retVal = msdSetAnyRegField(dev->devNum,  phyAddr, WILLOW_QD_REG_EGRESS_RATE_CTRL2, 0, 14, (MSD_U16)data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to write WILLOW_QD_REG_EGRESS_RATE_CTRL2 Register.\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_grcSetEgressRate Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_grcGetEgressRate
*
* DESCRIPTION:
*       This routine Willow_gets the port's egress data limit.
*
* INPUTS:
*       port    - logical port number.
*
* OUTPUTS:
*       rateType  - egress data rate limit (MSD_ERATE_TYPE union type).
*                    union type is used to support multiple devices with the
*                    different formats of egress rate.
*                    MSD_ERATE_TYPE has the following fields:
*                        kbRate - rate in kbps that should used with the WILLOW_MSD_ELIMIT_MODE of
*                                WILLOW_MSD_ELIMIT_LAYER1,
*                                WILLOW_MSD_ELIMIT_LAYER2, or
*                                WILLOW_MSD_ELIMIT_LAYER3 (see Willow_grcSetELimitMode)
*                            64kbps ~ 1Mbps    : increments of 64kbps,
*                            1Mbps ~ 100Mbps   : increments of 1Mbps, and
*                            100Mbps ~ 1000Mbps: increments of 10Mbps
*                            1Gbps ~ 5Gbps: increments of 100Mbps
*                            Therefore, the valid values are:
*                                64, 128, 192, 256, 320, 384,..., 960,
*                                1000, 2000, 3000, 4000, ..., 100000,
*                                110000, 120000, 130000, ..., 1000000
*                                1100000, 1200000, 1300000, ..., 5000000.
*                        fRate - frame per second that should used with WILLOW_MSD_ELIMIT_MODE of
*                                WILLOW_MSD_PIRL_ELIMIT_FRAME
*                            Valid values are between 3815 and 14880000
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_grcGetEgressRate
(
    IN MSD_QD_DEV		*dev,
    IN  MSD_LPORT		port,
    OUT WILLOW_MSD_ELIMIT_MODE *mode,
	OUT MSD_U32	*rate
)
{
    MSD_STATUS    retVal;         /* Functions return value.      */
    MSD_U16        eRate, eDec;
    MSD_U8        hwPort,phyAddr;        /* Physical port.               */

    MSD_DBG_INFO(("Willow_grcGetEgressRate Called.\n"));

    hwPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
	if(hwPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}

    if((retVal = Willow_grcGetELimitMode(dev,port,mode)) != MSD_OK)
    {
        MSD_DBG_ERROR(("Willow_grcGetELimitMode returned: %s.\n", msdDisplayStatus(retVal)));
        return retVal;
    }

    retVal = msdGetAnyRegField(dev->devNum, phyAddr,WILLOW_QD_REG_EGRESS_RATE_CTRL,0,7,&eDec);
    /* this will cause an automatic flush operation */
    if(retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_EGRESS_RATE_CTRL Register.\n"));
        return retVal;
    }

    retVal = msdGetAnyRegField(dev->devNum, phyAddr,WILLOW_QD_REG_EGRESS_RATE_CTRL2,0,14,&eRate );
    /* this will cause an automatic flush operation */
    if(retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_EGRESS_RATE_CTRL2 Register.\n"));
        return retVal;
    }

    if (*mode == WILLOW_MSD_ELIMIT_FRAME)    
    {
        *rate = WILLOW_MSD_GET_RATE_LIMIT_PER_FRAME(eRate, eDec);
    }
    else
    {
        /* Count Per Byte */
        *rate = WILLOW_MSD_GET_RATE_LIMIT_PER_BYTE(eRate, eDec);
    }

    MSD_DBG_INFO(("Willow_grcGetEgressRate Exit.\n"));
    return MSD_OK;
}


/*******************************************************************************
* Willow_grcSetELimitMode
*
* DESCRIPTION:
*       This routine sets Egress Rate Limit counting mode.
*       The supported modes are as follows:
*            WILLOW_MSD_ELIMIT_FRAME -
*                Count the number of frames
*            WILLOW_MSD_ELIMIT_LAYER1 -
*                Count all Layer 1 bytes: 
*                Preamble (8bytes) + Frame's DA to CRC + IFG (12bytes)
*            WILLOW_MSD_ELIMIT_LAYER2 -
*                Count all Layer 2 bytes: Frame's DA to CRC
*            WILLOW_MSD_ELIMIT_LAYER3 -
*                Count all Layer 1 bytes: 
*                Frame's DA to CRC - 18 - 4 (if frame is tagged)
*
* INPUTS:
*       port - logical port number
*       mode - WILLOW_MSD_ELIMIT_MODE enum type
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None. 
*
*******************************************************************************/
MSD_STATUS Willow_grcSetELimitMode
(
    IN  MSD_QD_DEV			*dev,
    IN  MSD_LPORT			port,
    IN  WILLOW_MSD_ELIMIT_MODE      mode
)
{
    MSD_U16            data;
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;        /* Physical port.               */
	MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_grcSetELimitMode Called.\n"));

    hwPort = MSD_LPORT_2_PORT(port);
	if(hwPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}

	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    data = (MSD_U16)mode & 0x3;

    /* Set the Elimit mode.            */
    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_EGRESS_RATE_CTRL2, 14, 2, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_EGRESS_RATE_CTRL2 Register.\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_grcSetELimitMode Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_grcGetELimitMode
*
* DESCRIPTION:
*       This routine Willow_gets Egress Rate Limit counting mode.
*       The supported modes are as follows:
*            WILLOW_MSD_ELIMIT_FRAME -
*                Count the number of frames
*            WILLOW_MSD_ELIMIT_LAYER1 -
*                Count all Layer 1 bytes: 
*                Preamble (8bytes) + Frame's DA to CRC + IFG (12bytes)
*            WILLOW_MSD_ELIMIT_LAYER2 -
*                Count all Layer 2 bytes: Frame's DA to CRC
*            WILLOW_MSD_ELIMIT_LAYER3 -
*                Count all Layer 1 bytes: 
*                Frame's DA to CRC - 18 - 4 (if frame is tagged)
*
* INPUTS:
*       port - logical port number
*
* OUTPUTS:
*       mode - WILLOW_MSD_ELIMIT_MODE enum type
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_grcGetELimitMode
(
    IN  MSD_QD_DEV		*dev,
    IN  MSD_LPORT		port,
    OUT WILLOW_MSD_ELIMIT_MODE  *mode
)
{
    MSD_U16            data;
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;        /* Physical port.               */
	MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_grcGetELimitMode Called.\n"));

    hwPort = MSD_LPORT_2_PORT(port);
	if(hwPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);

    /* Get the Elimit mode.            */
    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_EGRESS_RATE_CTRL2, 14, 2, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_EGRESS_RATE_CTRL2 Register.\n"));
        return retVal;
    }

    *mode = data;

    MSD_DBG_INFO(("Willow_grcGetELimitMode Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtSetDuplex
(
    IN  MSD_QD_DEV *dev,
    IN  MSD_LPORT  port,
    OUT WILLOW_MSD_PORT_FORCE_DUPLEX_MODE   mode
)
{
    MSD_U16          data;
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetDuplex Called.\n"));
    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);

    switch (mode)
    {
    case WILLOW_MSD_DUPLEX_HALF:
        data = 0x1;
        break;
    case WILLOW_MSD_DUPLEX_FULL:
        data = 0x3;
        break;
    case WILLOW_MSD_DUPLEX_AUTO:
        data = 0x0;
        break;
    default:
        MSD_DBG_ERROR(("Failed (Bad mode).\n"));
        return MSD_BAD_PARAM;
        break;
    }

    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PHY_CONTROL, 2, 2, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PORT_STATUS Register.\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetDuplex Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtGetDuplex
(
    IN  MSD_QD_DEV *dev,
    IN  MSD_LPORT  port,
    OUT WILLOW_MSD_PORT_FORCE_DUPLEX_MODE  *mode
)
{
    MSD_U16          data;
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtGetDuplex Called.\n"));
    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PHY_CONTROL, 2, 2, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PORT_STATUS Register.\n"));
        return retVal;
    }

    switch (data)
    {
    case 0x1:
        *mode = WILLOW_MSD_DUPLEX_HALF;
        break;
    case 0x3:
        *mode = WILLOW_MSD_DUPLEX_FULL;
        break;
    case 0:
    case 0x2:
        *mode = WILLOW_MSD_DUPLEX_AUTO;
        break;
    default:
        MSD_DBG_ERROR(("Failed (Bad mode).\n"));
        return MSD_BAD_PARAM;
        break;
    }


    MSD_DBG_INFO(("Willow_gprtGetDuplex Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtGetDuplexStatus
*
* DESCRIPTION:
*       This routine retrives the port duplex mode.
*
* INPUTS:
*       port - the logical port number.
*
* OUTPUTS:
*       mode - MSD_TRUE for Full-duplex  or MSD_FALSE for Half-duplex
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtGetDuplexStatus
(
	IN  MSD_QD_DEV *dev,
	IN  MSD_LPORT  port,
	OUT MSD_BOOL   *mode
)
{
	MSD_U16          data;
	MSD_STATUS       retVal;         /* Functions return value.      */
	MSD_U8           hwPort;         /* the physical port number     */
	MSD_U8			phyAddr;
	
	MSD_DBG_INFO(("Willow_gprtGetDuplexStatus Called.\n"));
	/* translate LPORT to hardware port */
	hwPort = MSD_LPORT_2_PORT(port);
	if (hwPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);

	/* Get the force flow control bit.  */
	retVal = msdGetAnyRegField(dev->devNum,  phyAddr, WILLOW_QD_REG_PORT_STATUS, 10, 1, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PORT_STATUS Register.\n"));
        return retVal;
    }

	/* translate binary to BOOL  */
	MSD_BIT_2_BOOL(data, *mode);

    MSD_DBG_INFO(("Willow_gprtGetDuplexStatus Exit.\n"));
	return MSD_OK;
}

MSD_STATUS Willow_gprtGetForceLink
(
    IN  MSD_QD_DEV *dev,
    IN  MSD_LPORT  port,
    OUT WILLOW_MSD_PORT_FORCE_LINK_MODE   *mode
)
{
    MSD_U16          data;
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtGetForceLink Called.\n"));
    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PHY_CONTROL, 4, 2, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PORT_STATUS Register.\n"));
        return retVal;
    }

    if ((data & 0x1) == 0)
        *mode = WILLOW_PORT_DO_NOT_FORCE_LINK;
    else if ((data & 0x3) == 3)
        *mode = WILLOW_PORT_FORCE_LINK_UP;
    else if ((data & 0x3) == 1)
        *mode = WILLOW_PORT_FORCE_LINK_DOWN;

    MSD_DBG_INFO(("Willow_gprtGetForceLink Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtSetForceLink
(
IN  MSD_QD_DEV *dev,
IN  MSD_LPORT  port,
OUT WILLOW_MSD_PORT_FORCE_LINK_MODE  mode
)
{
    MSD_U16          data;
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetForceLink Called.\n"));
    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);

    switch (mode)
    {
    case WILLOW_PORT_DO_NOT_FORCE_LINK:
        data = 0;
        break;
    case WILLOW_PORT_FORCE_LINK_UP:
        data = 3;
        break;
    case WILLOW_PORT_FORCE_LINK_DOWN:
        data = 1;
        break;
    default:
        MSD_DBG_ERROR(("Failed (Bad mpde).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PHY_CONTROL, 4, 2, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PORT_STATUS Register.\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetForceLink Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtGetLinkState
*
* DESCRIPTION:
*       This routine retrives the link state.
*
* INPUTS:
*       port - the logical port number.
*
* OUTPUTS:
*       state - MSD_TRUE for Up  or MSD_FALSE for Down
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtGetLinkState
(
	IN  MSD_QD_DEV *dev,
	IN  MSD_LPORT  port,
	OUT MSD_BOOL   *state
)
{
	MSD_U16          data;           /* Used to poll the SWReset bit */
	MSD_STATUS       retVal;         /* Functions return value.      */
	MSD_U8           hwPort;         /* the physical port number     */
	MSD_U8			phyAddr;
	
	MSD_DBG_INFO(("Willow_gprtGetLinkState Called.\n"));
	/* translate LPORT to hardware port */
	hwPort = MSD_LPORT_2_PORT(port);
	if (hwPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);

	/* Get the force flow control bit.  */
	retVal = msdGetAnyRegField(dev->devNum,  phyAddr, WILLOW_QD_REG_PORT_STATUS, 11, 1, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PORT_STATUS Register.\n"));
        return retVal;
    }

	/* translate binary to BOOL  */
	MSD_BIT_2_BOOL(data, *state);

    MSD_DBG_INFO(("Willow_gprtGetLinkState Exit.\n"));
	return MSD_OK;
}

/*******************************************************************************
* Willow_gprtSetForceSpeed
*
* DESCRIPTION:
*       This routine forces switch MAC speed.
*
* INPUTS:
*       port - the logical port number.
*       mode - WILLOW_MSD_PORT_FORCED_SPEED_MODE (10, 100, 200, 1000, 2.5g, 10g,or No Speed Force)
*
* OUTPUTS:
*       None
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtSetForceSpeed
(
IN MSD_QD_DEV    *dev,
IN MSD_LPORT     port,
IN WILLOW_MSD_PORT_FORCED_SPEED_MODE  mode
)
{
    MSD_STATUS       retVal;
    MSD_U8           hwPort;
    MSD_U8			phyAddr;
    MSD_U16			data1;
    MSD_U16			data2;
    MSD_U16			linkStatus;

    MSD_DBG_INFO(("Willow_gprtSetForceSpeed Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

	/* Set the ForceSpeed bit.  */
	if (mode == Willow_PORT_DO_NOT_FORCE_SPEED)
	{
		/*Get the force link status and then force link down*/
		if ((retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PHY_CONTROL, 4, 2, &linkStatus)) != MSD_OK)
        /* this will cause an automatic flush operation */
		{
			MSD_DBG_ERROR(("Get link status Failed\n"));
			return retVal;
		}
		if ((retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PHY_CONTROL, 4, 2, 1)) != MSD_OK)
        /* this will cause an automatic flush operation */
        {
			MSD_DBG_ERROR(("Force Link down failed\n"));
			return retVal;
		}

		if ((retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PHY_CONTROL, 13, 1, 0)) != MSD_OK)
        /* this will cause an automatic flush operation */
        {
			MSD_DBG_ERROR(("Set Force Speed bit error\n"));
			return retVal;
		}
	}
	else
	{
		switch (mode)
		{
		case Willow_PORT_FORCE_SPEED_10M:
			data1 = 0;
			data2 = 0;
			break;
		case Willow_PORT_FORCE_SPEED_100M:
			data1 = 0;
			data2 = 1;
			break;
		case Willow_PORT_FORCE_SPEED_200M:
			if (port != 0)
			{
				MSD_DBG_ERROR(("Failed (Bad Port), Only port 0 support 200M.\n"));
				return MSD_NOT_SUPPORTED;
			}
			data1 = 1;
			data2 = 1;
			break;
		case Willow_PORT_FORCE_SPEED_1000M:
			data1 = 0;
			data2 = 2;
			break;
		case Willow_PORT_FORCE_SPEED_2_5G:
			if ((port != 9) && (port != 10))
			{
				MSD_DBG_ERROR(("Failed (Bad Port), Only port 9, 10 support 2.5G.\n"));
				return MSD_NOT_SUPPORTED;
			}
			data1 = 1;
			data2 = 3;
			break;
		case Willow_PORT_FORCE_SPEED_10G:
			if ((port != 9) && (port != 10))
			{
				MSD_DBG_ERROR(("Failed (Bad Port), Only port 9, 10 support 10G.\n"));
				return MSD_NOT_SUPPORTED;
			}
			data1 = 0;
			data2 = 3;
			break;
		default:
			MSD_DBG_ERROR(("Failed (Bad Speed).\n"));
			return MSD_BAD_PARAM;
		}

		/*Get the force link status and then force link down*/
		if ((retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PHY_CONTROL, 4, 2, &linkStatus)) != MSD_OK)
        /* this will cause an automatic flush operation */
		{
			MSD_DBG_ERROR(("Get link status Failed\n"));
			return retVal;
		}
		if ((retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PHY_CONTROL, 4, 2, 1)) != MSD_OK)
        /* this will cause an automatic flush operation */
		{
			MSD_DBG_ERROR(("Force Link down failed\n"));
			return retVal;
		}

		/* Set the ForceSpeed bit.  */
		if ((retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PHY_CONTROL, 13, 1, 1)) != MSD_OK)
        /* this will cause an automatic flush operation */
		{
			MSD_DBG_ERROR(("Set Force Speed bit error\n"));
			return retVal;
		}

		if ((retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PHY_CONTROL, 12, 1, data1)) != MSD_OK)
        /* this will cause an automatic flush operation */
		{
			MSD_DBG_ERROR(("Set Alternate Speed bit error\n"));
			return retVal;
		}
		if ((retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PHY_CONTROL, 0, 2, data2)) != MSD_OK)
        /* this will cause an automatic flush operation */
		{
			MSD_DBG_ERROR(("Set Speed bit error\n"));
			return retVal;
		}
	}

    /*Set back the force link status*/
    if ((retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PHY_CONTROL, 4, 2, linkStatus)) != MSD_OK)
    /* this will cause an automatic flush operation */
    {
        MSD_DBG_ERROR(("Set back force link status error\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetForceSpeed Called.\n"));
    return retVal;
}

/*******************************************************************************
* Willow_gprtGetForceSpeed
*
* DESCRIPTION:
*       This routine retrieves switch MAC Force Speed value
*
* INPUTS:
*       port - the logical port number.
*
* OUTPUTS:
*       mode - WILLOW_MSD_PORT_FORCED_SPEED_MODE (10, 100, 200, 1000, 2.5g, 10g,or No Speed Force)
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtGetForceSpeed
(
IN  MSD_QD_DEV    *dev,
IN  MSD_LPORT     port,
OUT WILLOW_MSD_PORT_FORCED_SPEED_MODE   *mode
)
{
    MSD_U16          data1;
    MSD_U16          data2;
    MSD_STATUS       retVal;
    MSD_U8           hwPort;
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtGetForceSpeed Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    /* Get the ForceSpeed bit.  */
    if ((retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PHY_CONTROL, 13, 1, &data1)) != MSD_OK)
    /* this will cause an automatic flush operation */
    {
        MSD_DBG_ERROR(("Get force Speed bit error\n"));
        return retVal;
    }

    if (data1 == 0)
    {
        *mode = Willow_PORT_DO_NOT_FORCE_SPEED;
    }
    else
    {
        if ((retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PHY_CONTROL, 12, 1, &data1)) != MSD_OK)
        /* this will cause an automatic flush operation */
        {
            MSD_DBG_ERROR(("Get Alternate Speed bit error\n"));
            return retVal;
        }
        if ((retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PHY_CONTROL, 0, 2, &data2)) != MSD_OK)
        /* this will cause an automatic flush operation */
        {
            MSD_DBG_ERROR(("Get Speed bit error\n"));
            return retVal;
        }
        if (data1 == 1)
        {
            switch (data2)
            {
            case 0:
                *mode = Willow_PORT_FORCE_SPEED_10M;
                break;
            case 1:
                *mode = Willow_PORT_FORCE_SPEED_200M;
                break;
            case 2:
                *mode = Willow_PORT_FORCE_SPEED_1000M;
                break;
            case 3:
                *mode = Willow_PORT_FORCE_SPEED_2_5G;
                break;
            default:
                MSD_DBG_ERROR(("Get invalid speed from hardware\n."));
                return MSD_FAIL;
            }
        }
        else
        {
            switch (data2)
            {
            case 0:
                *mode = Willow_PORT_FORCE_SPEED_10M;
                break;
            case 1:
                *mode = Willow_PORT_FORCE_SPEED_100M;
                break;
            case 2:
                *mode = Willow_PORT_FORCE_SPEED_1000M;
                break;
            case 3:
                *mode = Willow_PORT_FORCE_SPEED_10G;
                break;
            default:
                MSD_DBG_ERROR(("Get invalid speed from hardware\n."));
                return MSD_FAIL;
            }
        }
    }

    MSD_DBG_INFO(("Willow_gprtGetForceSpeed Exit.\n"));
    return retVal;
}

/*******************************************************************************
* Willow_gprtGetSpeed
*
* DESCRIPTION:
*       This routine retrives the port speed.
*
* INPUTS:
*       port - the logical port number.
*
* OUTPUTS:
*       mode - WILLOW_MSD_PORT_SPEED type.
*                (PORT_SPEED_10_MBPS,PORT_SPEED_100_MBPS, PORT_SPEED_1000_MBPS,
*                etc.)
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtGetSpeed
(
	IN  MSD_QD_DEV *dev,
	IN  MSD_LPORT  port,
	OUT WILLOW_MSD_PORT_SPEED   *speed
)
{
	MSD_U16          data, speedEx;
	MSD_STATUS       retVal;         /* Functions return value.      */
	MSD_U8           hwPort;         /* the physical port number     */
	MSD_U8			phyAddr;

	MSD_DBG_INFO(("Willow_gprtGetSpeed Called.\n"));	

	/* translate LPORT to hardware port */
	hwPort = MSD_LPORT_2_PORT(port);
	if (hwPort == MSD_INVALID_PORT)
	{
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);

	retVal = msdGetAnyReg(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_STATUS, &data);
    if (retVal != MSD_OK)
    {
        msdRegisterAccessFlush(dev->devNum);
        MSD_DBG_ERROR(("ERROR to read WILLOW_QD_REG_PORT_STATUS Register.\n"));
        return retVal;
    }
    retVal = msdRegisterAccessFlush(dev->devNum);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("ERROR to flush read WILLOW_QD_REG_PORT_STATUS Register.\n"));
        return retVal;
    }

	/* Reg0_13 + Reg0[9:8]
		000 - 10M
		001 - 100M
		101 - 200M
		010 - 1000M
		011 - 10G
		111 - 2.5G
	*/
	speedEx = (MSD_U16)(((data & 0x2000) >> 11) | ((data & 0x300) >> 8));

	switch (speedEx)
	{
		case 0:
			*speed = Willow_PORT_SPEED_10_MBPS;
			break;
		case 1:
			*speed = Willow_PORT_SPEED_100_MBPS;
			break;
		case 5:
			*speed = Willow_PORT_SPEED_200_MBPS;
			break;
		case 2:
			*speed = Willow_PORT_SPEED_1000_MBPS;
			break;
		case 7:
			*speed = Willow_PORT_SPEED_2_5_GBPS;
			break;
		case 3:
			*speed = Willow_PORT_SPEED_10_GBPS;
			break;
		default:
			*speed = Willow_PORT_SPEED_UNKNOWN;
			break;
	}

    MSD_DBG_INFO(("Willow_gprtGetSpeed Called.\n"));
	return MSD_OK;
}


/*******************************************************************************
* Willow_gprtGetVlanPorts
*
* DESCRIPTION:
*       this routine Get port based vlan table of a specified port
*
* INPUTS:
*       devNum  - physical devie number
*       portNum - logical port number
*
* OUTPUTS:
*       memPorts - vlan ports
*       memPortsLen - number of vlan ports
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtGetVlanPorts
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
OUT MSD_LPORT  *memPorts,
OUT MSD_U8  *memPortsLen
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
    MSD_U8			phyAddr;
    MSD_U8           i;
    MSD_32           portLen = 0;

    MSD_DBG_INFO(("Willow_gprtGetVlanPorts Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_VLAN_MAP, 0, 11, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Get WILLOW_QD_REG_PORT_VLAN_MAP register error\n"));
        return retVal;
    }
    for (i = 0; i < dev->numOfPorts; i++)
    {
        if (data & (1 << i))
        {
            memPorts[portLen++] = i;
        }
    }

    *memPortsLen = (MSD_U8)portLen;

    MSD_DBG_INFO(("Willow_gprtGetVlanPorts Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* msdPortVlanPortsSet
*
* DESCRIPTION:
*       this routine Set port based vlan table of a specified port
*
* INPUTS:
*       devNum  - physical devie number
*       portNum - logical port number
*       memPorts - vlan ports to set
*       memPortsLen - number of vlan ports
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
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtSetVlanPorts
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
IN  MSD_LPORT  *memPorts,
IN  MSD_U8  memPortsLen
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
    MSD_U8			phyAddr;
    MSD_U8           i;

    MSD_DBG_INFO(("Willow_gprtSetVlanPorts Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    if (memPortsLen > dev->numOfPorts)
    {
        MSD_DBG_ERROR(("Failed (Bad num of vlan ports).\n"));
        return MSD_BAD_PARAM;
    }

    data = 0;
    for (i = 0; i < memPortsLen; i++)
    {
        if (memPorts[i] < dev->numOfPorts)
            data |= (1 << memPorts[i]);
        else
        {
            MSD_DBG_ERROR(("Failed (Bad member port).\n"));
            return MSD_BAD_PARAM;
        }
    }

    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_VLAN_MAP, 0, 11, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Set WILLOW_QD_REG_PORT_VLAN_MAP register error\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetVlanPorts Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtGetFrameMode
*
* DESCRIPTION:
*       this routine Get frame mode of a specified port
*
* INPUTS:
*       port - logical port number
*
* OUTPUTS:
*       mode - MSD_FRAME_MODE
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtGetFrameMode
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
OUT WILLOW_MSD_FRAME_MODE  *mode
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtGetFrameMode Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL, 8, 2, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Get WILLOW_QD_REG_PORT_CONTROL register error\n"));
        return retVal;
    }

    *mode = (WILLOW_MSD_FRAME_MODE)data;

    MSD_DBG_INFO(("Willow_gprtGetFrameMode Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtSetFrameMode
*
* DESCRIPTION:
*       this routine Set frame mode of a specified port
*
* INPUTS:
*       port - logical port number
*       mode - MSD_FRAME_MODE
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
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtSetFrameMode
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
IN  WILLOW_MSD_FRAME_MODE  mode
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetFrameMode Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    switch (mode)
    {
    case WILLOW_FRAME_MODE_NORMAL :
    case WILLOW_FRAME_MODE_DSA:
    case WILLOW_FRAME_MODE_PROVIDER:
    case WILLOW_FRAME_MODE_ETHER_TYPE_DSA:
        break;
    default:
        MSD_DBG_ERROR(("Failed (Bad mode).\n"));
        return MSD_BAD_PARAM;
    }

    data = (MSD_U16)mode;

    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL, 8, 2, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Set WILLOW_QD_REG_PORT_CONTROL register error\n"));
        return retVal;
    }


    MSD_DBG_INFO(("Willow_gprtSetFrameMode Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtSetEgressMode
*
* DESCRIPTION:
*       this routine Set egress mode of a specified port
*
* INPUTS:
*       port - logical port number
*       mode - MSD_EGRESS_MODE
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
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtSetEgressMode
(
	IN MSD_QD_DEV*  dev,
	IN MSD_LPORT  port,
	IN WILLOW_MSD_EGRESS_MODE mode
)
{
	MSD_STATUS       retVal;         /* Functions return value.      */
	MSD_U8           hwPort;         /* the physical port number     */
	MSD_U16          data;           /* to keep the read valve       */
	MSD_U8			phyAddr;

	MSD_DBG_INFO(("Willow_gprtSetEgressMode Called.\n"));

	/* translate LPORT to hardware port */
	hwPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
	if (hwPort == MSD_INVALID_PORT)
	{
		MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}

	switch (mode)
	{
	case WILLOW_MSD_EGRESS_MODE_UNMODIFIED:
	case WILLOW_MSD_EGRESS_MODE_UNTAGGED:
	case WILLOW_MSD_EGRESS_MODE_TAGGED:
		break;
	default:
		MSD_DBG_ERROR(("Failed (Bad mode).\n"));
		return MSD_BAD_PARAM;
	}

	data = (MSD_U16)mode;

	retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL, 12, 2, data);
    /* this will cause an automatic flush operation */
	if (retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Set WILLOW_QD_REG_PORT_CONTROL register error\n"));
		return retVal;
	}


	MSD_DBG_INFO(("Willow_gprtSetEgressMode Exit.\n"));
	return MSD_OK;
}

/*******************************************************************************
* Willow_gprtGetEgressMode
*
* DESCRIPTION:
*       this routine Get egress mode of a specified port
*
* INPUTS:
*       port - logical port number
*
* OUTPUTS:
*       mode - MSD_EGRESS_MODE
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtGetEgressMode
(
	IN  MSD_QD_DEV  *dev,
	IN  MSD_LPORT  port,
	OUT WILLOW_MSD_EGRESS_MODE  *mode
)
{
	MSD_STATUS       retVal;         /* Functions return value.      */
	MSD_U8           hwPort;         /* the physical port number     */
	MSD_U16          data;           /* to keep the read valve       */
	MSD_U8			phyAddr;

	MSD_DBG_INFO(("Willow_gprtGetEgressMode Called.\n"));

	/* translate LPORT to hardware port */
	hwPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
	if (hwPort == MSD_INVALID_PORT)
	{
		MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}

	retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL, 12, 2, &data);
    /* this will cause an automatic flush operation */
	if (retVal != MSD_OK)
	{
		MSD_DBG_ERROR(("Get WILLOW_QD_REG_PORT_CONTROL register error\n"));
		return retVal;
	}

	*mode = (WILLOW_MSD_EGRESS_MODE)data;

	MSD_DBG_INFO(("Willow_gprtGetEgressMode Exit.\n"));
	return MSD_OK;
}

/*******************************************************************************
* Willow_gprtGetPortState
*
* DESCRIPTION:
*       this routine Get port state of a specified port
*
* INPUTS:
*       port - logical port number
*
* OUTPUTS:
*       mode - WILLOW_PORT_STP_STATE
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtGetPortState
(
    IN  MSD_QD_DEV  *dev,
    IN  MSD_LPORT  port,
    OUT WILLOW_MSD_PORT_STP_STATE  *mode
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtGetPortState Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL, 0, 2, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Get WILLOW_QD_REG_PORT_CONTROL register error\n"));
        return retVal;
    }

    *mode = (WILLOW_MSD_PORT_STP_STATE)data;

    MSD_DBG_INFO(("Willow_gprtGetPortState Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Willow_gprtSetPortState
*
* DESCRIPTION:
*       this routine Set port state of a specified port
*
* INPUTS:
*       port - logical port number
*       mode - WILLOW_PORT_STP_STATE
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
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtSetPortState
(
    IN  MSD_QD_DEV  *dev,
    IN  MSD_LPORT  port,
    IN  WILLOW_MSD_PORT_STP_STATE  mode
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetPortState Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    switch (mode)
    {
    case WILLOW_MSD_PORT_DISABLE:
    case WILLOW_MSD_PORT_BLOCKING:
    case WILLOW_MSD_PORT_LEARNING:
    case WILLOW_MSD_PORT_FORWARDING:
        break;
    default:
        MSD_DBG_ERROR(("Failed (Bad mode).\n"));
        return MSD_BAD_PARAM;
    }

    data = (MSD_U16)mode;

    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL, 0, 2, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Set WILLOW_QD_REG_PORT_CONTROL register error\n"));
        return retVal;
    }


    MSD_DBG_INFO(("Willow_gprtSetPortState Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtSetCutThrouthEnable
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
IN  WILLOW_CUT_THROUGH  *cutThru
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetCutThrouthEnable Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    data = ((cutThru->enable & 0x1) << 8) | cutThru->cutThruQueue;

    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_CT_CONTROL, 0, 9, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Set WILLOW_QD_REG_CT_CONTROL register error\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetCutThrouthEnable Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtGetCutThrouthEnable
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
OUT  WILLOW_CUT_THROUGH  *cutThru
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtGetCutThrouthEnable Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_CT_CONTROL, 0, 9, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Set WILLOW_QD_REG_CT_CONTROL register error\n"));
        return retVal;
    }

    cutThru->enable = (data >> 8) & 0x1;
    cutThru->cutThruQueue = data & 0xff;

    MSD_DBG_INFO(("Willow_gprtGetCutThrouthEnable Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtSetIgnoreFcs
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
IN  MSD_BOOL  en
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetIgnoreFcs Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    /* translate BOOL to binary */
    MSD_BOOL_2_BIT(en, data);

    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL2, 15, 1, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Set WILLOW_QD_REG_PORT_CONTROL2 register error\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetIgnoreFcs Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtGetIgnoreFcs
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
OUT MSD_BOOL  *en
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtGetIgnoreFcs Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL2, 15, 1, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Set WILLOW_QD_REG_PORT_CONTROL2 register error\n"));
        return retVal;
    }

    MSD_BIT_2_BOOL(data, *en);

    MSD_DBG_INFO(("Willow_gprtGetIgnoreFcs Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtSetAllowBadFcs
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
IN  MSD_BOOL  en
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetAllowBadFcs Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    /* translate BOOL to binary */
    MSD_BOOL_2_BIT(en, data);

    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL2, 14, 1, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Set WILLOW_QD_REG_PORT_CONTROL2 register error\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetAllowBadFcs Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtGetAllowBadFcs
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
OUT MSD_BOOL  *en
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtGetAllowBadFcs Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL2, 14, 1, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Set WILLOW_QD_REG_PORT_CONTROL2 register error\n"));
        return retVal;
    }

    MSD_BIT_2_BOOL(data, *en);

    MSD_DBG_INFO(("Willow_gprtGetAllowBadFcs Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtSetPauseLimitOut
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
IN  MSD_U16  limit
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */

    MSD_DBG_INFO(("Willow_gprtSetPauseLimitOut Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = Willow_writeFlowCtrlReg(dev, port, 0x1, (MSD_U8)limit);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Willow_writeFlowCtrlReg port:%x, reg:%x, returned: %s.\n", port, 0x1, msdDisplayStatus(retVal)));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetPauseLimitOut Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtGetPauseLimitOut
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
OUT MSD_U16  *limit
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */

    MSD_DBG_INFO(("Willow_gprtGetPauseLimitOut Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = Willow_readFlowCtrlReg(dev, port, 0x1, (MSD_U8*)limit);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Willow_readFlowCtrlReg port:%x, reg:%x, returned: %s.\n", port, 0x1, msdDisplayStatus(retVal)));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtGetPauseLimitOut Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtSetPauseLimitIn
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
IN  MSD_U16  limit
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */

    MSD_DBG_INFO(("Willow_gprtSetPauseLimitIn Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = Willow_writeFlowCtrlReg(dev, port, 0x0, (MSD_U8)limit);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Willow_writeFlowCtrlReg port:%x, reg:%x, returned: %s.\n", port, 0x0, msdDisplayStatus(retVal)));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetPauseLimitIn Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtGetPauseLimitIn
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
OUT MSD_U16  *limit
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */

    MSD_DBG_INFO(("Willow_gprtGetPauseLimitOut Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = Willow_readFlowCtrlReg(dev, port, 0x0, (MSD_U8*)limit);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Willow_readFlowCtrlReg port:%x, reg:%x, returned: %s.\n", port, 0x0, msdDisplayStatus(retVal)));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtGetPauseLimitOut Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtSetQueueToPause
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
IN  MSD_U8  queues
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */

    MSD_DBG_INFO(("Willow_gprtSetQueueToPause Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = Willow_writeFlowCtrlReg(dev, port, 0x17, (MSD_U8)queues);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Willow_writeFlowCtrlReg port:%x, reg:%x, returned: %s.\n", port, 0x17, msdDisplayStatus(retVal)));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetQueueToPause Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtGetQueueToPause
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
OUT MSD_U8  *queues
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */

    MSD_DBG_INFO(("Willow_gprtGetQueueToPause Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = Willow_readFlowCtrlReg(dev, port, 0x17, (MSD_U8*)queues);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Willow_readFlowCtrlReg port:%x, reg:%x, returned: %s.\n", port, 0x17, msdDisplayStatus(retVal)));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtGetQueueToPause Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtSetRxPriorityFlowControlEnable
(
IN  MSD_QD_DEV  *dev,
IN MSD_LPORT port,
IN MSD_U8 pfcPri,
IN MSD_BOOL en
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8           pointer;
    MSD_U8           data, data1;

    MSD_DBG_INFO(("Willow_gprtSetRxPriorityFlowControlEnable Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    if (pfcPri > 0x7)
    {
        MSD_DBG_ERROR(("Failed (Bad pfcPri), pfcPri : %u.\n", pfcPri));
        return MSD_BAD_PARAM;
    }

    pointer = (MSD_U8)(pfcPri / 2 + 0x18);

    retVal = Willow_readFlowCtrlReg(dev, port, pointer, &data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Willow_readFlowCtrlReg port:%x, reg:%x, returned: %s.\n", port, pointer, msdDisplayStatus(retVal)));
        return retVal;
    }

    MSD_BOOL_2_BIT(en, data1);
	if (pfcPri % 2)
    {
        data &= 0x7f;
        data |= (data1 << 7);
    }
    else
    {
        data &= 0xf7;
        data |= (data1 << 3);
    }

    retVal = Willow_writeFlowCtrlReg(dev, port, pointer, (MSD_U8)data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Willow_writeFlowCtrlReg port:%x, reg:%x, returned: %s.\n", port, pointer, msdDisplayStatus(retVal)));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetRxPriorityFlowControlEnable Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtGetRxPriorityFlowControlEnable
(
IN  MSD_QD_DEV  *dev,
IN MSD_LPORT port,
IN MSD_U8 pfcPri,
OUT MSD_BOOL *en
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8           pointer;
    MSD_U8           bitMask;
    MSD_U8           data;

    MSD_DBG_INFO(("Willow_gprtGetRxPriorityFlowControlEnable Called.\n"));


    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    if (pfcPri > 0x7)
    {
        MSD_DBG_ERROR(("Failed (Bad pfcPri), pfcPri : %u.\n", pfcPri));
        return MSD_BAD_PARAM;
    }

    pointer = (MSD_U8)(pfcPri / 2 + 0x18);

	if (pfcPri % 2)
        bitMask = 0x80;
    else
        bitMask = 0x08;

    retVal = Willow_readFlowCtrlReg(dev, port, pointer, &data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Willow_readFlowCtrlReg port:%x, reg:%x, returned: %s.\n", port, pointer, msdDisplayStatus(retVal)));
        return retVal;
    }

    if ((data & bitMask) != 0)
        *en = MSD_TRUE;
    else
        *en = MSD_FALSE;

    MSD_DBG_INFO(("Willow_gprtGetRxPriorityFlowControlEnable Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtSetRxPriorityFlowControlToQueue
(
IN  MSD_QD_DEV  *dev,
IN MSD_LPORT port,
IN MSD_U8 pfcPri,
IN MSD_U8 queue
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8           pointer;
    MSD_U8           data;

    MSD_DBG_INFO(("Willow_gprtSetRxPriorityFlowControlToQueue Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    if (pfcPri > 0x7)
    {
        MSD_DBG_ERROR(("Failed (Bad pfcPri), pfcPri : %u.\n", pfcPri));
        return MSD_BAD_PARAM;
    }

	if (queue >= WILLOW_MAX_NUM_OF_QUEUES)
    {
        MSD_DBG_ERROR(("Failed (Bad queue), queue : %u.\n", queue));
        return MSD_BAD_PARAM;
    }

    pointer = (MSD_U8)(pfcPri / 2 + 0x18);

    retVal = Willow_readFlowCtrlReg(dev, port, pointer, &data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Willow_readFlowCtrlReg port:%x, reg:%x, returned: %s.\n", port, pointer, msdDisplayStatus(retVal)));
        return retVal;
    }
	if (pfcPri % 2)
    {
        data &= 0x8f;
        data |= (queue << 4);
    }
    else
    {
        data &= 0xf8;
        data |= queue;
    }

    retVal = Willow_writeFlowCtrlReg(dev, port, pointer, (MSD_U8)data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Willow_writeFlowCtrlReg port:%x, reg:%x, returned: %s.\n", port, pointer, msdDisplayStatus(retVal)));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetRxPriorityFlowControlToQueue Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtGetRxPriorityFlowControlToQueue
(
IN  MSD_QD_DEV  *dev,
IN MSD_LPORT port,
IN MSD_U8 pfcPri,
OUT MSD_U8 *queue
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8           pointer;
    MSD_U8           data;

    MSD_DBG_INFO(("Willow_gprtGetRxPriorityFlowControlToQueue Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    if (pfcPri > 0x7)
    {
        MSD_DBG_ERROR(("Failed (Bad pfcPri), pfcPri : %u.\n", pfcPri));
        return MSD_BAD_PARAM;
    }

    pointer = (MSD_U8)(pfcPri / 2 + 0x18);

    retVal = Willow_readFlowCtrlReg(dev, port, pointer, &data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Willow_readFlowCtrlReg port:%x, reg:%x, returned: %s.\n", port, pointer, msdDisplayStatus(retVal)));
        return retVal;
    }
	if (pfcPri % 2)
    {
        *queue = (data >> 4) & 0x7;
    }
    else
    {
        *queue = data & 0x7;
    }

    MSD_DBG_INFO(("Willow_gprtGetRxPriorityFlowControlToQueue Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtSetTxPriorityFlowControlEnable
(
IN  MSD_QD_DEV  *dev,
IN MSD_LPORT port,
IN MSD_U8 pri,
IN MSD_BOOL en
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8           pointer;
    MSD_U8           data, data1;

    MSD_DBG_INFO(("Willow_gprtSetTxPriorityFlowControlEnable Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    if (pri > 0x7)
    {
        MSD_DBG_ERROR(("Failed (Bad pri), pri : %u.\n", pri));
        return MSD_BAD_PARAM;
    }

    pointer = (MSD_U8)(pri / 2 + 0x1c);

    retVal = Willow_readFlowCtrlReg(dev, port, pointer, &data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Willow_readFlowCtrlReg port:%x, reg:%x, returned: %s.\n", port, pointer, msdDisplayStatus(retVal)));
        return retVal;
    }

    MSD_BOOL_2_BIT(en, data1);
	if (pri % 2)
    {
        data &= 0x7f;
        data |= (data1 << 7);
    }
    else
    {
        data &= 0xf7;
        data |= (data1 << 3);
    }

    retVal = Willow_writeFlowCtrlReg(dev, port, pointer, (MSD_U8)data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Willow_writeFlowCtrlReg port:%x, reg:%x, returned: %s.\n", port, pointer, msdDisplayStatus(retVal)));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetTxPriorityFlowControlEnable Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtGetTxPriorityFlowControlEnable
(
IN  MSD_QD_DEV  *dev,
IN MSD_LPORT port,
IN MSD_U8 pri,
OUT MSD_BOOL *en
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8           pointer;
    MSD_U8           bitMask;
    MSD_U8           data;

    MSD_DBG_INFO(("Willow_gprtGetTxPriorityFlowControlEnable Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    if (pri > 0x7)
    {
        MSD_DBG_ERROR(("Failed (Bad pri), pri : %u.\n", pri));
        return MSD_BAD_PARAM;
    }

    pointer = (MSD_U8)(pri / 2 + 0x1c);

	if (pri % 2)
        bitMask = 0x80;
    else
        bitMask = 0x08;

    retVal = Willow_readFlowCtrlReg(dev, port, pointer, &data);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Willow_readFlowCtrlReg port:%x, reg:%x, returned: %s.\n", port, pointer, msdDisplayStatus(retVal)));
        return retVal;
    }

    if ((data & bitMask) != 0)
        *en = MSD_TRUE;
    else
        *en = MSD_FALSE;

    MSD_DBG_INFO(("Willow_gprtGetTxPriorityFlowControlEnable Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtGetOutQueueSize
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
IN  MSD_U8 queue,
OUT MSD_U16  *count
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtGetOutQueueSize Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

	if (queue >= WILLOW_MAX_NUM_OF_QUEUES)
    {
        MSD_DBG_ERROR(("Failed (Bad queue), queue : %u.\n", queue));
        return MSD_BAD_PARAM;
    }

    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_Q_COUNTER, 12, 4, queue);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Set WILLOW_QD_REG_Q_COUNTER register error\n"));
        return retVal;
    }

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_Q_COUNTER, 0, 9, count);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Get WILLOW_QD_REG_Q_COUNTER register error\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtGetOutQueueSize Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtSetMapDA
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
IN  MSD_BOOL  en
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetMapDA Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    /* translate BOOL to binary */
    MSD_BOOL_2_BIT(en, data);

    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL2, 7, 1, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Set WILLOW_QD_REG_PORT_CONTROL2 register error\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetMapDA Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtGetMapDA
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
OUT MSD_BOOL  *en
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtGetMapDA Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL2, 7, 1, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Get WILLOW_QD_REG_PORT_CONTROL2 register error\n"));
        return retVal;
    }

    MSD_BIT_2_BOOL(data, *en);

    MSD_DBG_INFO(("Willow_gprtGetMapDA Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtSetPortLocked
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
IN  MSD_BOOL  en
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetPortLocked Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    /* translate BOOL to binary */
    MSD_BOOL_2_BIT(en, data);

    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PAV, 13, 1, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Set WILLOW_QD_REG_PAV register error\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetPortLocked Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtGetPortLocked
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
OUT MSD_BOOL  *en
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtGetPortLocked Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PAV, 13, 1, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Get WILLOW_QD_REG_PAV register error\n"));
        return retVal;
    }

    MSD_BIT_2_BOOL(data, *en);

    MSD_DBG_INFO(("Willow_gprtGetPortLocked Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtSetHoldAt1
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
IN  MSD_BOOL  en
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetHoldAt1 Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    /* translate BOOL to binary */
    MSD_BOOL_2_BIT(en, data);

    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PAV, 15, 1, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Set WILLOW_QD_REG_PAV register error\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetHoldAt1 Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtGetHoldAt1
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
OUT MSD_BOOL  *en
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtGetHoldAt1 Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PAV, 15, 1, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Get WILLOW_QD_REG_PAV register error\n"));
        return retVal;
    }

    MSD_BIT_2_BOOL(data, *en);

    MSD_DBG_INFO(("Willow_gprtGetHoldAt1 Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtSetInt0nAgeOut
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
IN  MSD_BOOL  en
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetInt0nAgeOut Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    /* translate BOOL to binary */
    MSD_BOOL_2_BIT(en, data);

    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PAV, 14, 1, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Set WILLOW_QD_REG_PAV register error\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetInt0nAgeOut Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtGetInt0nAgeOut
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
OUT MSD_BOOL  *en
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtGetInt0nAgeOut Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PAV, 14, 1, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Get WILLOW_QD_REG_PAV register error\n"));
        return retVal;
    }

    MSD_BIT_2_BOOL(data, *en);

    MSD_DBG_INFO(("Willow_gprtGetInt0nAgeOut Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtSetRefreshLocked
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
IN  MSD_BOOL  en
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetRefreshLocked Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    /* translate BOOL to binary */
    MSD_BOOL_2_BIT(en, data);

    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PAV, 11, 1, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Set WILLOW_QD_REG_PAV register error\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetRefreshLocked Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtGetRefreshLocked
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
OUT MSD_BOOL  *en
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtGetRefreshLocked Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PAV, 11, 1, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Get WILLOW_QD_REG_PAV register error\n"));
        return retVal;
    }

    MSD_BIT_2_BOOL(data, *en);

    MSD_DBG_INFO(("Willow_gprtGetRefreshLocked Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtSetIgnoreWrongData
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
IN  MSD_BOOL  en
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetIgnoreWrongData Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    /* translate BOOL to binary */
    MSD_BOOL_2_BIT(en, data);

    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PAV, 12, 1, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Set WILLOW_QD_REG_PAV register error\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetIgnoreWrongData Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtGetIgnoreWrongData
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
OUT MSD_BOOL  *en
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtGetIgnoreWrongData Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PAV, 12, 1, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Get WILLOW_QD_REG_PAV register error\n"));
        return retVal;
    }

    MSD_BIT_2_BOOL(data, *en);

    MSD_DBG_INFO(("Willow_gprtGetIgnoreWrongData Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtSetARPMirror
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
IN  MSD_BOOL  en
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtSetARPMirror Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    /* translate BOOL to binary */
    MSD_BOOL_2_BIT(en, data);

    retVal = msdSetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL2, 6, 1, data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Set WILLOW_QD_REG_PORT_CONTROL2 register error\n"));
        return retVal;
    }

    MSD_DBG_INFO(("Willow_gprtSetARPMirror Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtGetARPMirror
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
OUT MSD_BOOL  *en
)
{
    MSD_STATUS       retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U16          data;           /* to keep the read valve       */
    MSD_U8			phyAddr;

    MSD_DBG_INFO(("Willow_gprtGetARPMirror Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
    phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        MSD_DBG_ERROR(("Failed (Bad Port).\n"));
        return MSD_BAD_PARAM;
    }

    retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_PORT_CONTROL2, 6, 1, &data);
    /* this will cause an automatic flush operation */
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Get WILLOW_QD_REG_PORT_CONTROL2 register error\n"));
        return retVal;
    }

    MSD_BIT_2_BOOL(data, *en);

    MSD_DBG_INFO(("Willow_gprtGetARPMirror Exit.\n"));
    return MSD_OK;
}

MSD_STATUS Willow_gprtRegDump
(
IN  MSD_QD_DEV    *dev,
IN  MSD_LPORT  port
)
{
	MSD_STATUS       retVal;         /* Functions return value.      */
	MSD_U16	data[32];
	MSD_U8   hwPort;         /* the physical port number     */
	MSD_U8	phyAddr;
	MSD_U8 i = 0;
	char desc[32][48] =
	{
		{ "Port Status Register(0x0)" },
		{ "Physical Control Register(0x1)" },
		{ "Jamming Control Register(0x2)" },
		{ "Switch Identifier Register(0x3)" },
		{ "Port Control Register(0x4)" },
		{ "Port Control 1(0x5)" },
		{ "Port Based VLAN Map(0x6)" },
		{ "Default Port VLAN ID & Priority(0x7)" },
		{ "Port Control 2 Register(0x8)" },
		{ "Egress Rate Control(0x9)" },
		{ "Egress Rate Control 2(0xa)" },
		{ "Port Association Vector(0xb)" },
		{ "Port ATU Control(0xc)" },
		{ "Override Register(0xd)" },
		{ "Policy Control Register(0xe)" },
		{ "Port E Type(0xf)" },
		{ "Reserved(0x10)" },
		{ "Reserved(0x11)" },
		{ "Reserved(0x12)" },
		{ "Reserved(0x13)" },
		{ "Reserved(0x14)" },
		{ "Reserved(0x15)" },
		{ "LED Control(0x16)" },
		{ "IP Priority Mapping Table(0x17)" },
		{ "Port IEEE Priority Remapping Registers(0x18)" },
		{ "Port Control 3 Register(0x19)" },
		{ "Reserved(0x1a)" },
		{ "Queue Counter Registers(0x1b)" },
		{ "Port Queue Control register(0x1c)" },
		{ "Reserved(0x1d)" },
		{ "Cut Through Register(0x1e)" },
		{ "Debug Counter(0x1f)" }
	};

	MSD_DBG_INFO(("Willow_gprtRegDump Called.\n"));
	/* translate LPORT to hardware port */
	hwPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
	if (hwPort == MSD_INVALID_PORT)
	{
		MSD_DBG_ERROR(("Failed (Bad Port).\n"));
		return MSD_BAD_PARAM;
	}

	MSG(("\n------------------------------------------------------\n"));
	for (i = 0; i < 32; i++)
	{
		retVal = msdGetAnyReg(dev->devNum, phyAddr, i, &data[i]);
		if (retVal != MSD_OK)
		{
            msdRegisterAccessFlush(dev->devNum);
			MSD_DBG_ERROR(("msdGetAnyReg returned: %s.\n", msdDisplayStatus(retVal)));
			return retVal;
		}
	}
    retVal = msdRegisterAccessFlush(dev->devNum);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("msdRegisterAccessFlush returned: %s.\n", msdDisplayStatus(retVal)));
        return retVal;
    }
    for (i = 0; i < 32; i++)
    {
        MSG(("%-48s%04x\n", desc[i], data[i]));
    }
    MSD_DBG_INFO(("Willow_gprtRegDump Exit.\n"));
	return MSD_OK;
}

/****************************************************************************/
/* Internal functions.                                                      */
/****************************************************************************/
static MSD_STATUS Willow_writeFlowCtrlReg
(
	IN MSD_QD_DEV	*dev, 
	IN MSD_LPORT	port,
	IN MSD_U8	pointer,
	IN MSD_U8	data
)
{
	MSD_STATUS    retVal;         /* Functions return value.      */
    MSD_U8        hwPort;         /* the physical port number     */
	MSD_U8		 phyAddr;
    MSD_U16   count, tmpData;
    MSD_DBG_INFO(("Willow_writeFlowCtrlReg Called.\n"));

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
	if(hwPort == MSD_INVALID_PORT)
	{
		return MSD_BAD_PARAM;
	}

    msdSemTake(dev->devNum, dev->tblRegsSem,OS_WAIT_FOREVER);

    count = 5;
    tmpData = 1;
    while(tmpData ==1)
    {
		retVal = msdGetAnyRegField(dev->devNum,  phyAddr, WILLOW_QD_REG_LIMIT_PAUSE_CONTROL, 15, 1, &tmpData);
        /* this will cause an automatic flush operation */
        if (retVal != MSD_OK)
		{
			msdSemGive(dev->devNum, dev->tblRegsSem);
			return retVal;
		}
		if ((count--) == 0)
		{
			msdSemGive(dev->devNum,  dev->tblRegsSem);
			return MSD_FAIL;
		}
    }

	tmpData =  (MSD_U16)((1 << 15) | ((pointer&0x7F) << 8) | (MSD_U16)data);

	retVal = msdSetAnyReg(dev->devNum,  phyAddr, WILLOW_QD_REG_LIMIT_PAUSE_CONTROL, tmpData);
	if(retVal != MSD_OK)
	{
        msdRegisterAccessFlush(dev->devNum);
        MSD_DBG_INFO(("Failed.\n"));
		msdSemGive(dev->devNum, dev->tblRegsSem);
		return retVal;
	}
    retVal = msdRegisterAccessFlush(dev->devNum);
    if (retVal != MSD_OK)
    {
        MSD_DBG_INFO(("Failed.\n"));
    }
    return retVal;
}

static MSD_STATUS Willow_readFlowCtrlReg
(
	IN MSD_QD_DEV	*dev,
	IN MSD_LPORT	port,
	IN MSD_U8	pointer,
	OUT MSD_U8	*data
)
{
	MSD_STATUS    retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
	MSD_U8			phyAddr;
    MSD_U16            tmpData;
    int count;

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(port);
	phyAddr = WILLOW_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
	if(hwPort == MSD_INVALID_PORT)
	{
		return MSD_BAD_PARAM;
	}

    msdSemTake(dev->devNum, dev->tblRegsSem,OS_WAIT_FOREVER);

    count = 0x10;
    tmpData = 1;
    while (tmpData == 1)
    {
        retVal = msdGetAnyRegField(dev->devNum, phyAddr, WILLOW_QD_REG_LIMIT_PAUSE_CONTROL, 15, 1, &tmpData);
        /* this will cause an automatic flush operation */
        if (retVal != MSD_OK)
        {
            msdSemGive(dev->devNum, dev->tblRegsSem);
            return retVal;
        }
        if ((count--) == 0)
        {
            msdSemGive(dev->devNum, dev->tblRegsSem);
            return MSD_FAIL;
        }
    }

	tmpData =  (MSD_U16)(((pointer&0x7F) << 8) | 0);
	retVal = msdSetAnyReg(dev->devNum,  phyAddr, WILLOW_QD_REG_LIMIT_PAUSE_CONTROL, tmpData);
	if(retVal != MSD_OK)
	{
        msdRegisterAccessFlush(dev->devNum);
	    msdSemGive(dev->devNum, dev->tblRegsSem);
	    return retVal;
	}

    count = 0x10;
    do {
		retVal = msdGetAnyReg(dev->devNum,  phyAddr, WILLOW_QD_REG_LIMIT_PAUSE_CONTROL, &tmpData);
        if(retVal != MSD_OK)
        {
            msdRegisterAccessFlush(dev->devNum);
            msdSemGive(dev->devNum, dev->tblRegsSem);
			return retVal;
        }
        retVal = msdRegisterAccessFlush(dev->devNum);
        if (retVal != MSD_OK)
        {
            msdSemGive(dev->devNum, dev->tblRegsSem);
            return retVal;
        }
        if ((count--) == 0)
		{
			msdSemGive(dev->devNum, dev->tblRegsSem);
			return MSD_FAIL;
		}
    } while (tmpData&0x8000);

	*data = (MSD_U8)(tmpData & 0xff);
    msdSemGive(dev->devNum, dev->tblRegsSem);

    return retVal;
}
#endif
