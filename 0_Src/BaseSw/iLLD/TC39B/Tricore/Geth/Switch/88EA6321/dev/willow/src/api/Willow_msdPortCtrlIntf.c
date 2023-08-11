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
MSD_STATUS Willow_gvlnSetPortVlanDot1qModeIntf
(
    IN MSD_QD_DEV        *dev,
    IN MSD_LPORT     port,
	IN MSD_8021Q_MODE    mode
)
{
	return Willow_gvlnSetPortVlanDot1qMode(dev, port, (WILLOW_MSD_8021Q_MODE)mode);
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
MSD_STATUS Willow_gvlnGetPortVlanDot1qModeIntf
(
    IN MSD_QD_DEV        *dev,
    IN  MSD_LPORT        port,
	OUT MSD_8021Q_MODE   *mode
)
{
	return Willow_gvlnGetPortVlanDot1qMode(dev, port, (WILLOW_MSD_8021Q_MODE*)mode);
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
MSD_STATUS Willow_gprtSetDiscardTaggedIntf
(
    IN MSD_QD_DEV    *dev,
    IN MSD_LPORT     port,
    IN MSD_BOOL        mode
)
{
	return Willow_gprtSetDiscardTagged(dev, port, mode);
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
MSD_STATUS Willow_gprtGetDiscardTaggedIntf
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT    port,
    OUT MSD_BOOL     *mode
)
{
	return Willow_gprtGetDiscardTagged(dev, port, mode);
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
MSD_STATUS Willow_gprtSetDiscardUntaggedIntf
(
    IN MSD_QD_DEV    *dev,
    IN MSD_LPORT     port,
    IN MSD_BOOL        mode
)
{
	return Willow_gprtSetDiscardUntagged(dev, port, mode);
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
MSD_STATUS Willow_gprtGetDiscardUntaggedIntf
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT    port,
    OUT MSD_BOOL     *mode
)
{
	return Willow_gprtGetDiscardUntagged(dev, port, mode);
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
MSD_STATUS Willow_gprtSetUnicastFloodBlockIntf
(
    IN  MSD_QD_DEV   *dev,
    IN MSD_LPORT     port,
    IN MSD_BOOL      mode
)
{
	return Willow_gprtSetUnicastFloodBlock(dev, port, mode);
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
MSD_STATUS Willow_gprtGetUnicastFloodBlockIntf
(
	IN  MSD_QD_DEV   *dev,
	IN  MSD_LPORT    port,
	OUT MSD_BOOL     *mode
)
{
	return Willow_gprtGetUnicastFloodBlock(dev, port, mode);
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
MSD_STATUS Willow_gprtSetMulticastFloodBlockIntf
(
    IN MSD_QD_DEV    *dev,
    IN MSD_LPORT     port,
    IN MSD_BOOL        mode
)
{
	return Willow_gprtSetMulticastFloodBlock(dev, port, mode);
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
MSD_STATUS Willow_gprtGetMulticastFloodBlockIntf
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT    port,
    OUT MSD_BOOL     *mode
)
{
	return Willow_gprtGetMulticastFloodBlock(dev, port, mode);
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
MSD_STATUS Willow_gprtSetDiscardBCastModeIntf
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT     port,
    IN  MSD_BOOL      en
)
{
	return Willow_gprtSetDiscardBCastMode(dev, port, en);
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
MSD_STATUS Willow_gprtGetDiscardBCastModeIntf
(
	IN  MSD_QD_DEV    *dev,
	IN  MSD_LPORT     port,
	OUT MSD_BOOL      *en
)
{
	return Willow_gprtGetDiscardBCastMode(dev, port, en);
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
*       mode - MSD_MTU_SIZE (1522, 2048, or 10240)
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
MSD_STATUS Willow_gprtSetJumboModeIntf
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT    port,
	IN  MSD_MTU_SIZE   mode
)
{
	return Willow_gprtSetJumboMode(dev, port, (WILLOW_MSD_MTU_SIZE)mode);
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
*       mode - MSD_MTU_SIZE (1522, 2048, or 10240)
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
MSD_STATUS Willow_gprtGetJumboModeIntf
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT    port,
	OUT MSD_MTU_SIZE   *mode
)
{
	return Willow_gprtGetJumboMode(dev, port, (WILLOW_MSD_MTU_SIZE*)mode);
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
*       mode - MSD_TRUE for disable or MSD_FALSE otherwise
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
MSD_STATUS Willow_gprtSetLearnEnableIntf
(
    IN MSD_QD_DEV    *dev,
    IN MSD_LPORT     port,
    IN MSD_BOOL      mode
)
{
	return Willow_gprtSetLearnEnable(dev, port, mode);
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
MSD_STATUS Willow_gprtGetLearnEnableIntf
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT     port,
    OUT MSD_BOOL      *mode
)
{
	return Willow_gprtGetLearnEnable(dev, port, mode);
}

MSD_STATUS Willow_gprtGetVTUPriorityOverrideIntf
(
IN  MSD_QD_DEV    *dev,
IN  MSD_LPORT     port,
OUT MSD_PRI_OVERRIDE  *mode
)
{
    return Willow_gprtGetVTUPriorityOverride(dev, port, (WILLOW_MSD_PRI_OVERRIDE*)mode);
}
MSD_STATUS Willow_gprtSetVTUPriorityOverrideIntf
(
IN  MSD_QD_DEV    *dev,
IN  MSD_LPORT     port,
OUT MSD_PRI_OVERRIDE  mode
)
{
    return Willow_gprtSetVTUPriorityOverride(dev, port, (WILLOW_MSD_PRI_OVERRIDE)mode);
}

MSD_STATUS Willow_gprtGetSAPriorityOverrideIntf
(
IN  MSD_QD_DEV    *dev,
IN  MSD_LPORT     port,
OUT MSD_PRI_OVERRIDE  *mode
)
{
    return Willow_gprtGetSAPriorityOverride(dev, port, (WILLOW_MSD_PRI_OVERRIDE*)mode);
}
MSD_STATUS Willow_gprtSetSAPriorityOverrideIntf
(
IN  MSD_QD_DEV    *dev,
IN  MSD_LPORT     port,
OUT MSD_PRI_OVERRIDE  mode
)
{
    return Willow_gprtSetSAPriorityOverride(dev, port, (WILLOW_MSD_PRI_OVERRIDE)mode);
}

MSD_STATUS Willow_gprtGetDAPriorityOverrideIntf
(
IN  MSD_QD_DEV    *dev,
IN  MSD_LPORT     port,
OUT MSD_PRI_OVERRIDE  *mode
)
{
    return Willow_gprtGetDAPriorityOverride(dev, port, (WILLOW_MSD_PRI_OVERRIDE*)mode);
}
MSD_STATUS Willow_gprtSetDAPriorityOverrideIntf
(
IN  MSD_QD_DEV    *dev,
IN  MSD_LPORT     port,
OUT MSD_PRI_OVERRIDE  mode
)
{
    return Willow_gprtSetDAPriorityOverride(dev, port, (WILLOW_MSD_PRI_OVERRIDE)mode);
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
MSD_STATUS Willow_gprtSetFlowCtrlIntf
(
	IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT    port,
    IN  MSD_BOOL     en,
	IN  MSD_PORT_FC_MODE      mode
)
{
	return Willow_gprtSetFlowCtrl(dev, port, en, (WILLOW_MSD_PORT_FC_MODE)mode);
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
MSD_STATUS Willow_gprtGetFlowCtrlIntf
(
	IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT    port,
    OUT MSD_BOOL     *en,
	OUT MSD_PORT_FC_MODE      *mode
)
{
	return Willow_gprtGetFlowCtrl(dev, port, en, (WILLOW_MSD_PORT_FC_MODE*)mode);
}

/*******************************************************************************
* Willow_gprtSetForceSpeed
*
* DESCRIPTION:
*       This routine forces switch MAC speed. 
*
* INPUTS:
*       port - the logical port number.
*       mode - MSD_PORT_FORCED_SPEED_MODE (10, 100, 200, 1000, 2.5g, 10g,or No Speed Force)
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
MSD_STATUS Willow_gprtSetForceSpeedIntf
(
    IN MSD_QD_DEV    *dev,
    IN MSD_LPORT     port,
	IN MSD_PORT_FORCED_SPEED_MODE  mode
)
{
	WILLOW_MSD_PORT_FORCED_SPEED_MODE willowMode;
	switch (mode)
	{
	case MSD_PORT_FORCE_SPEED_10M:
		willowMode = Willow_PORT_FORCE_SPEED_10M;
		break;
	case MSD_PORT_FORCE_SPEED_100M:
		willowMode = Willow_PORT_FORCE_SPEED_100M;
		break;
	case MSD_PORT_FORCE_SPEED_200M:
		willowMode = Willow_PORT_FORCE_SPEED_200M;
		break;
	case MSD_PORT_FORCE_SPEED_1000M:
		willowMode = Willow_PORT_FORCE_SPEED_1000M;
		break;
	case MSD_PORT_FORCE_SPEED_2_5G:
		willowMode = Willow_PORT_FORCE_SPEED_2_5G;
		break;
	case MSD_PORT_FORCE_SPEED_5G:
		return MSD_NOT_SUPPORTED;
	case MSD_PORT_FORCE_SPEED_10G:
		willowMode = Willow_PORT_FORCE_SPEED_10G;
		break;
	case MSD_PORT_DO_NOT_FORCE_SPEED:
		willowMode = Willow_PORT_DO_NOT_FORCE_SPEED;
		break;
	default:
		MSD_DBG_ERROR(("Failed (Bad Speed).\n"));
		return MSD_BAD_PARAM;
	}
	return Willow_gprtSetForceSpeed(dev, port, willowMode);
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
*       mode - MSD_PORT_FORCED_SPEED_MODE (10, 100, 200, 1000, 2.5g, 10g,or No Speed Force)
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
MSD_STATUS Willow_gprtGetForceSpeedIntf
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT     port,
	OUT MSD_PORT_FORCED_SPEED_MODE   *mode
)
{
	return Willow_gprtGetForceSpeed(dev, port, (WILLOW_MSD_PORT_FORCED_SPEED_MODE*)mode);
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
*                        kbRate - rate in kbps that should used with the MSD_ELIMIT_MODE of 
*                                MSD_ELIMIT_LAYER1,
*                                MSD_ELIMIT_LAYER2, or 
*                                MSD_ELIMIT_LAYER3 (see Willow_grcSetELimitMode)
*                            64kbps ~ 1Mbps    : increments of 64kbps,
*                            1Mbps ~ 100Mbps   : increments of 1Mbps, and
*                            100Mbps ~ 1000Mbps: increments of 10Mbps
*                            1Gbps ~ 5Gbps: increments of 100Mbps
*                            Therefore, the valid values are:
*                                64, 128, 192, 256, 320, 384,..., 960,
*                                1000, 2000, 3000, 4000, ..., 100000,
*                                110000, 120000, 130000, ..., 1000000
*                                1100000, 1200000, 1300000, ..., 5000000.
*                        fRate - frame per second that should used with MSD_ELIMIT_MODE of 
*                                MSD_PIRL_ELIMIT_FRAME
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
MSD_STATUS Willow_grcSetEgressRateIntf
(
    IN MSD_QD_DEV       *dev,
    IN MSD_LPORT        port,
    IN MSD_ELIMIT_MODE   mode,
    IN MSD_U32   rate
)
{
    return Willow_grcSetEgressRate(dev, port, (WILLOW_MSD_ELIMIT_MODE)mode, rate);
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
*                        kbRate - rate in kbps that should used with the MSD_ELIMIT_MODE of
*                                MSD_ELIMIT_LAYER1,
*                                MSD_ELIMIT_LAYER2, or
*                                MSD_ELIMIT_LAYER3 (see Willow_grcSetELimitMode)
*                            64kbps ~ 1Mbps    : increments of 64kbps,
*                            1Mbps ~ 100Mbps   : increments of 1Mbps, and
*                            100Mbps ~ 1000Mbps: increments of 10Mbps
*                            1Gbps ~ 5Gbps: increments of 100Mbps
*                            Therefore, the valid values are:
*                                64, 128, 192, 256, 320, 384,..., 960,
*                                1000, 2000, 3000, 4000, ..., 100000,
*                                110000, 120000, 130000, ..., 1000000
*                                1100000, 1200000, 1300000, ..., 5000000.
*                        fRate - frame per second that should used with MSD_ELIMIT_MODE of
*                                MSD_PIRL_ELIMIT_FRAME
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
MSD_STATUS Willow_grcGetEgressRateIntf
(
    IN MSD_QD_DEV		*dev,
    IN  MSD_LPORT		port,
    OUT MSD_ELIMIT_MODE   *mode,
    OUT MSD_U32   *rate
)
{
    return Willow_grcGetEgressRate(dev, port, (WILLOW_MSD_ELIMIT_MODE*)mode, rate);
}


/*******************************************************************************
* Willow_grcSetELimitMode
*
* DESCRIPTION:
*       This routine sets Egress Rate Limit counting mode.
*       The supported modes are as follows:
*            MSD_ELIMIT_FRAME -
*                Count the number of frames
*            MSD_ELIMIT_LAYER1 -
*                Count all Layer 1 bytes: 
*                Preamble (8bytes) + Frame's DA to CRC + IFG (12bytes)
*            MSD_ELIMIT_LAYER2 -
*                Count all Layer 2 bytes: Frame's DA to CRC
*            MSD_ELIMIT_LAYER3 -
*                Count all Layer 1 bytes: 
*                Frame's DA to CRC - 18 - 4 (if frame is tagged)
*
* INPUTS:
*       port - logical port number
*       mode - MSD_ELIMIT_MODE enum type
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
MSD_STATUS Willow_grcSetELimitModeIntf
(
    IN  MSD_QD_DEV			*dev,
    IN  MSD_LPORT			port,
	IN  MSD_ELIMIT_MODE      mode
)
{
	return Willow_grcSetELimitMode(dev, port, (WILLOW_MSD_ELIMIT_MODE)mode);
}


/*******************************************************************************
* Willow_grcGetELimitMode
*
* DESCRIPTION:
*       This routine Willow_gets Egress Rate Limit counting mode.
*       The supported modes are as follows:
*            MSD_ELIMIT_FRAME -
*                Count the number of frames
*            MSD_ELIMIT_LAYER1 -
*                Count all Layer 1 bytes: 
*                Preamble (8bytes) + Frame's DA to CRC + IFG (12bytes)
*            MSD_ELIMIT_LAYER2 -
*                Count all Layer 2 bytes: Frame's DA to CRC
*            MSD_ELIMIT_LAYER3 -
*                Count all Layer 1 bytes: 
*                Frame's DA to CRC - 18 - 4 (if frame is tagged)
*
* INPUTS:
*       port - logical port number
*
* OUTPUTS:
*       mode - MSD_ELIMIT_MODE enum type
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
MSD_STATUS Willow_grcGetELimitModeIntf
(
    IN  MSD_QD_DEV		*dev,
    IN  MSD_LPORT		port,
	OUT MSD_ELIMIT_MODE  *mode
)
{
	return Willow_grcGetELimitMode(dev, port, (WILLOW_MSD_ELIMIT_MODE*)mode);
}

MSD_STATUS Willow_gprtSetDuplexIntf
(
    IN  MSD_QD_DEV *dev,
    IN  MSD_LPORT  port,
    OUT MSD_PORT_FORCE_DUPLEX_MODE   mode
)
{
    return Willow_gprtSetDuplex(dev, port, (WILLOW_MSD_PORT_FORCE_DUPLEX_MODE)mode);
}

MSD_STATUS Willow_gprtGetDuplexIntf
(
    IN  MSD_QD_DEV *dev,
    IN  MSD_LPORT  port,
    OUT MSD_PORT_FORCE_DUPLEX_MODE   *mode
)
{
    return Willow_gprtGetDuplex(dev, port, (WILLOW_MSD_PORT_FORCE_DUPLEX_MODE*)mode);
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
MSD_STATUS Willow_gprtGetDuplexStatusIntf
(
	IN  MSD_QD_DEV *dev,
	IN  MSD_LPORT  port,
	OUT MSD_BOOL   *mode
)
{
	return Willow_gprtGetDuplexStatus(dev, port, mode);
}

MSD_STATUS Willow_gprtGetForceLinkIntf
(
IN  MSD_QD_DEV *dev,
IN  MSD_LPORT  port,
OUT MSD_PORT_FORCE_LINK_MODE   *mode
)
{
    return Willow_gprtGetForceLink(dev, port, (WILLOW_MSD_PORT_FORCE_LINK_MODE*)mode);
}

MSD_STATUS Willow_gprtSetForceLinkIntf
(
IN  MSD_QD_DEV *dev,
IN  MSD_LPORT  port,
OUT MSD_PORT_FORCE_LINK_MODE  mode
)
{
    return Willow_gprtSetForceLink(dev, port, (WILLOW_MSD_PORT_FORCE_LINK_MODE)mode);
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
*       mode - MSD_PORT_SPEED type.
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
MSD_STATUS Willow_gprtGetSpeedIntf
(
	IN  MSD_QD_DEV *dev,
	IN  MSD_LPORT  port,
	OUT MSD_PORT_SPEED   *speed
)
{
	return Willow_gprtGetSpeed(dev, port, (WILLOW_MSD_PORT_SPEED*)speed);
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
MSD_STATUS Willow_gprtGetFrameModeIntf
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
OUT MSD_FRAME_MODE  *mode
)
{
    return Willow_gprtGetFrameMode(dev, port, (WILLOW_MSD_FRAME_MODE*)mode);
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
MSD_STATUS Willow_gprtSetFrameModeIntf
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
IN  MSD_FRAME_MODE  mode
)
{
    return Willow_gprtSetFrameMode(dev, port, (WILLOW_MSD_FRAME_MODE)mode);
}

/*******************************************************************************
* Willow_gprtSetEgressModeIntf
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
MSD_STATUS Willow_gprtSetEgressModeIntf
(
IN MSD_QD_DEV*  dev,
IN MSD_LPORT  port,
IN MSD_EGRESS_MODE mode
)
{
	WILLOW_MSD_EGRESS_MODE WillowMode;
	switch (mode)
	{
		case MSD_EGRESS_MODE_UNMODIFIED:
			WillowMode = WILLOW_MSD_EGRESS_MODE_UNMODIFIED;
			break;
		case MSD_EGRESS_MODE_UNTAGGED:
			WillowMode = WILLOW_MSD_EGRESS_MODE_UNTAGGED;
			break;
		case MSD_EGRESS_MODE_TAGGED:
			WillowMode = WILLOW_MSD_EGRESS_MODE_TAGGED;
			break;
		default:
			MSD_DBG_ERROR(("Failed (Bad Mode).\n"));
			return MSD_BAD_PARAM;
	}
	return Willow_gprtSetEgressMode(dev, port, WillowMode);
}

/*******************************************************************************
* Willow_gprtGetEgressModeIntf
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
MSD_STATUS Willow_gprtGetEgressModeIntf
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
OUT MSD_EGRESS_MODE  *mode
)
{
	return Willow_gprtGetEgressMode(dev, port, (WILLOW_MSD_EGRESS_MODE *)mode);
}

MSD_STATUS Willow_gprtGetPortStateIntf
(
    IN  MSD_QD_DEV  *dev,
    IN  MSD_LPORT  port,
    OUT MSD_PORT_STP_STATE  *mode
)
{
    return Willow_gprtGetPortState(dev, port, (WILLOW_MSD_PORT_STP_STATE*)mode);
}

MSD_STATUS Willow_gprtSetPortStateIntf
(
    IN  MSD_QD_DEV  *dev,
    IN  MSD_LPORT  port,
    IN  MSD_PORT_STP_STATE  mode
)
{
    return Willow_gprtSetPortState(dev, port, (WILLOW_MSD_PORT_STP_STATE)mode);
}

MSD_STATUS Willow_gprtSetCutThrouthEnableIntf
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
IN  MSD_CUT_THROUGH  *cutThru
)
{
    return Willow_gprtSetCutThrouthEnable(dev, port, (WILLOW_CUT_THROUGH*)cutThru);
}

MSD_STATUS Willow_gprtGetCutThrouthEnableIntf
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT  port,
IN  MSD_CUT_THROUGH  *cutThru
)
{
    return Willow_gprtGetCutThrouthEnable(dev, port, (WILLOW_CUT_THROUGH*)cutThru);
}
#endif
