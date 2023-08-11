#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"

/********************************************************************************
* msdPolicy.c
*
* DESCRIPTION:
*       API definitions for Policy 
*
* DEPENDENCIES:
*
* FILE REVISION NUMBER:
*******************************************************************************/

#include "msdPolicy.h"
#include "msdApiTypes.h"
#include "msdUtils.h"

/*******************************************************************************
* msdPolicyMgmtCtrlSet
*
* DESCRIPTION:
*       Sets policy and mgmt control data.
*
* INPUTS:
*       portNum	- the logical port number.
*		point - to the Monitor and mgmt control register.
*           The Pointer bits are used to access the Index registers as follows:
*              0x00 to 0x0F: Polcy Trap/Mirror/Discard Policy Selections
*              0x10 to 0x1F: Policy Configurations
*              0x20 to 0x2F: Reserved DA's to CPU & Alt CPU Enables
*              0x30 to 0x3F: Trap/Mirror/CPU Port Settings
*       data  - Octet Data of the Policy Control Register referenced by the 
*               by the point above.
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
MSD_STATUS msdPolicyMgmtCtrlSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_U8		point,
IN MSD_U8		data
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicySetMgmtCtrl != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicySetMgmtCtrl(dev, portNum, point, data);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyMgmtCtrlGet
*
* DESCRIPTION:
*       Sets policy and mgmt control data.
*
* INPUTS:
*       portNum	- the logical port number.
*		point - to the Monitor and mgmt control register.
*           The Pointer bits are used to access the Index registers as follows:
*              0x00 to 0x0F: Polcy Trap/Mirror/Discard Policy Selections
*              0x10 to 0x1F: Policy Configurations
*              0x20 to 0x2F: Reserved DA's to CPU & Alt CPU Enables
*              0x30 to 0x3F: Trap/Mirror/CPU Port Settings
*
* OUTPUTS:
*       data  - Octet Data of the Policy Control Register referenced by the
*               by the point above.
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
MSD_STATUS msdPolicyMgmtCtrlGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
IN  MSD_U8		point,
OUT MSD_U8		*data
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicyGetMgmtCtrl != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicyGetMgmtCtrl(dev, portNum, point, data);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyCPUDestSet
*
* DESCRIPTION:
*       This routine sets CPU Destination Port. CPU Destination port indicates the
*       port number on this device where the CPU is connected (either directly or
*       indirectly through another Marvell switch device).
*
*       Many modes of frame processing need to know where the CPU is located.
*       These modes are:
*        1. When IGMP/MLD frame is received and Snooping is enabled
*        2. When the port is configured as a DSA port and it receives a To_CPU frame
*        3. When a Rsvd2CPU frame enters the port
*        4. When the port's SA Filtering mode is Drop to CPU
*        5. When any of the port's Policy Options trap the frame to the CPU
*        6. When the ingressing frame is an ARP and ARP mirroring is enabled in the
*           device
*		 7. When any of the port¡¯s Miss Traps occur that are enabled
*
*       In all cases, except for ARP, the frames that meet the enabled criteria
*       are mapped to the CPU Destination port, overriding where the frame would
*       normally go. In the case of ARP, the frame will be mapped normally and it
*       will also get copied to this port.
*       Frames that filtered or discarded will not be mapped to the CPU Destination
*       port with the exception of the Rsvd2CPU and DSA Tag cases.
*
*       If CPUDest = 0x1F, the remapped frames will be discarded, no ARP mirroring
*       will occur and ingressing To_CPU frames will be discarded.Use a value of 0x1E 
*		to map the frames to the internal CPU.
*
* INPUTS:
*       portNum	- the logical port number.
*		cpuPort - The CPU destination port.
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
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyCPUDestSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_LPORT    cpuPort
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicySetCpuDest != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicySetCpuDest(dev, portNum, cpuPort);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyCPUDestGet
*
* DESCRIPTION:
*       This routine gets CPU Destination Port. CPU Destination port indicates the
*       port number on this device where the CPU is connected (either directly or
*       indirectly through another Marvell switch device).
*
*       Many modes of frame processing need to know where the CPU is located.
*       These modes are:
*        1. When IGMP/MLD frame is received and Snooping is enabled
*        2. When the port is configured as a DSA port and it receives a To_CPU frame
*        3. When a Rsvd2CPU frame enters the port
*        4. When the port's SA Filtering mode is Drop to CPU
*        5. When any of the port's Policy Options trap the frame to the CPU
*        6. When the ingressing frame is an ARP and ARP mirroring is enabled in the
*           device
*		 7. When any of the port¡¯s Miss Traps occur that are enabled
*
*       In all cases, except for ARP, the frames that meet the enabled criteria
*       are mapped to the CPU Destination port, overriding where the frame would
*       normally go. In the case of ARP, the frame will be mapped normally and it
*       will also get copied to this port.
*       Frames that filtered or discarded will not be mapped to the CPU Destination
*       port with the exception of the Rsvd2CPU and DSA Tag cases.
*
*       If CPUDest = 0x1F, the remapped frames will be discarded, no ARP mirroring
*       will occur and ingressing To_CPU frames will be discarded.Use a value of 0x1E
*		to map the frames to the internal CPU.
*
* INPUTS:
*       port	- the logical port number.
*
* OUTPUTS:
*		cpuPort - The CPU destination port.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyCPUDestGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
OUT MSD_LPORT   *cpuPort
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicyGetCpuDest != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicyGetCpuDest(dev, portNum, cpuPort);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyIngressMonitorDestSet
*
* DESCRIPTION:
*       This routine sets Ingress Monitor Destination Port. Frames that are
*       targeted toward an Ingress Monitor Destination go out the port number
*       indicated in these bits. This includes frames received on a Marvell Tag port
*       with the Ingress Monitor type, and frames received on a Network port that
*       is enabled to be the Ingress Monitor Source Port.
*       If the Ingress Monitor Destination Port resides in this device these bits
*       should point to the Network port where these frames are to egress. If the
*       Ingress Monitor Destination Port resides in another device these bits
*       should point to the Marvell Tag port in this device that is used to get
*       to the device that contains the Ingress Monitor Destination Port.
*
* INPUTS:
*       portNum	- the logical port number.
*		imd		- The ingress monitor destination port.
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
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyIngressMonitorDestSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_LPORT	imd
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicySetIngressMonitorDest != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicySetIngressMonitorDest(dev, portNum, imd);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyIngressMonitorDestGet
*
* DESCRIPTION:
*       This routine gets Ingress Monitor Destination Port.
*
* INPUTS:
*       portNum	- the logical port number.
*
* OUTPUTS:
*		imd		- The ingress monitor destination port.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyIngressMonitorDestGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
OUT MSD_LPORT   *imd
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicyGetIngressMonitorDest != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicyGetIngressMonitorDest(dev, portNum, imd);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyRsvd2CpuEnablesSet
*
* DESCRIPTION:
*       This routine enable or disable trap reserved DA (01:80:C2:00:00:0x) to
*       CPU port.  When the Rsvd2Cpu(gsysSetRsvd2Cpu) is set to a one, the 16
*       reserved multicast DA addresses, whose bit in this register are also
*       set to a one, are treadted as MGMT frames. All the reserved DA's take
*       the form 01:80:C2:00:00:0x. When x = 0x0, bit 0 of this register is
*       tested. When x = 0x2, bit 2 of this field is tested and so on.
*		If the tested bit in this register is cleared to a zero, the frame will
*       be treated as a normal (non-MGMT) frame.
*
* INPUTS:
*       portNum	- the logical port number.
*       enBits  - bit vector of enabled Reserved Multicast.
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
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyRsvd2CpuEnablesSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_U16		enBits
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicySetRsvd2CpuEnables != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicySetRsvd2CpuEnables(dev, portNum, enBits);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyRsvd2CpuEnablesGet
*
* DESCRIPTION:
*       Gets the reserved DA enable bits. When the Rsvd2Cpu(gsysSetRsvd2Cpu) is
*       set to a one, the 16 reserved multicast DA addresses, whose bit in this
*       register are also set to a one, are treadted as MGMT frames. All the
*       reserved DA's take the form 01:80:C2:00:00:0x. When x = 0x0, bit 0 of
*       this register is tested. When x = 0x2, bit 2 of this field is tested and
*		so on.
*       If the tested bit in this register is cleared to a zero, the frame will
*       be treated as a normal (non-MGMT) frame.
*
* INPUTS:
*       portNum	- the logical port number.
*
* OUTPUTS:
*       enBits  - bit vector of enabled Reserved Multicast.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyRsvd2CpuEnablesGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
OUT MSD_U16   *enBits
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicyGetRsvd2CpuEnables != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicyGetRsvd2CpuEnables(dev, portNum, enBits);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyMGMTPriSet
*
* DESCRIPTION:
*       This routine sets the priority for Rsvd2CPU frames.These bits are used
*		as the PRI[2:0] bits on Rsvd2CPU frames.
*
* INPUTS:
*       portNum	- the logical port number.
*       pri		- PRI[2:0] bits (should be less than 8)
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
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyMGMTPriSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_U16		pri
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicySetMGMTPri != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicySetMGMTPri(dev, portNum, pri);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyMGMTPriGet
*
* DESCRIPTION:
*       This routine gets the priority for Rsvd2CPU frames. These bits are used
*		as the PRI[2:0] bits on Rsvd2CPU frames.
*
* INPUTS:
*       portNum	- the logical port number.
*
* OUTPUTS:
*       pri		- PRI[2:0] bits (should be less than 8)
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyMGMTPriGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
OUT MSD_U16     *pri
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicyGetMGMTPri != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicyGetMGMTPri(dev, portNum, pri);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyRouteDestSet
*
* DESCRIPTION:
*       This routine sets Route Destination Port. 
*		Frames that ingress a port that trigger a Route policy trap or mirror are
*		mapped (copied if action is a mirror) to this port as long as the frame is not
*		filtered or discarded. This function is disabled when this register is set to
*		0x1F. The RouteDest should point to the port that directs these frames to the
*		CPU that will process these frames. This target port should be a DSA Tag
*		port so the frames will egress with a To_CPU DSA Tag with a CPU Code of
*		Route (0x6).
*
* INPUTS:
*       portNum	- the logical port number.
*		routePort - The Route destination port.
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
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyRouteDestSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_LPORT    routePort
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicySetRouteDest != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicySetRouteDest(dev, portNum, routePort);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyRouteDestGet
*
* DESCRIPTION:
*       This routine gets Route Destination Port.
*		Frames that ingress a port that trigger a Route policy trap or mirror are
*		mapped (copied if action is a mirror) to this port as long as the frame is not
*		filtered or discarded. This function is disabled when this register is set to
*		0x1F. The RouteDest should point to the port that directs these frames to the
*		CPU that will process these frames. This target port should be a DSA Tag
*		port so the frames will egress with a To_CPU DSA Tag with a CPU Code of
*		Route (0x6).
*
* INPUTS:
*       portNum	- the logical port number.
*
* OUTPUTS:
*		routePort - The Route destination port.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyRouteDestGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
OUT MSD_LPORT   *routePort
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicyGetRouteDest != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicyGetRouteDest(dev, portNum, routePort);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyMirrorDestSet
*
* DESCRIPTION:
*       This routine sets Mirror Destination Port.
*       Frames that ingress a port that trigger a policy mirror are mapped (copied) to
*       this port as long as the frame is not filtered or discarded. This function is
*       disabled when this register is set to 0x1F. The MirrorDest should point to the
*       port that directs these frames to the CPU that will process these frames. This
*       target port should be a DSA Tag port so the frames will egress with a To_CPU
*       DSA Tag with a CPU Code of Policy Mirror.
*
* INPUTS:
*       portNum	- the logical port number.
*		mirrorPort - The mirror destination port.
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
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyMirrorDestSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_LPORT    mirrorPort
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicySetMirrorDest != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicySetMirrorDest(dev, portNum, mirrorPort);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyMirrorDestGet
*
* DESCRIPTION:
*       This routine gets Mirror Destination Port.
*       Frames that ingress a port that trigger a policy mirror are mapped (copied) to
*       this port as long as the frame is not filtered or discarded. This function is
*       disabled when this register is set to 0x1F. The MirrorDest should point to the
*       port that directs these frames to the CPU that will process these frames. This
*       target port should be a DSA Tag port so the frames will egress with a To_CPU
*       DSA Tag with a CPU Code of Policy Mirror.
*
* INPUTS:
*       portNum	- the logical port number.
*
* OUTPUTS:
*		mirrorPort - The mirror destination port.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyMirrorDestGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
OUT MSD_LPORT   *mirrorPort
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicyGetMirrorDest != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicyGetMirrorDest(dev, portNum, mirrorPort);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyIllegalIPAddrSet    
*
* DESCRIPTION:
*       This routine configures route policy action for Illegal IP Address. 
*       Illegal IP Addresses are:
*       For IPv4 SIP:
*       127.0.0.0 to 127.255.255.255 (Loopback addr or 127.0.0.0/8)
*       224.0.0.0 to 239.255.255.255 (Multicast ClassD or 224.0.0.0/4)
*       255.255.255.255 (Limited Broadcast addr)
*       For IPv4 DIP:
*       0.0.0.0 (Unspecified addr)
*       127.0.0.0 to 127.255.255.255 (Loopback addr or 127.0.0.0/8)
*       For IPv6 SIP:
*       0.0.0.0.0.0.0.1 (Loopback addr or ::1/128)
*       FF00.0.0.0.0.0.0.0 to all ones (Multicast addr or FF::/8)
*       For IPv6 DIP:
*       0.0.0.0.0.0.0.0 (Unspecified addr or ::/128)
*       0.0.0.0.0.0.0.1 (Loopback addr or ::1/128)
*
* INPUTS:
*       portNum	- the logical port number.
*		action  - Enumeration of the route policy action.
*			MSD_ROUTE_POLICY_ACTION_NORMAL				- Normal frame switching, do nothing.
*			MSD_ROUTE_POLICY_ACTION_MIRROR_MC_TRAP_UC	- For IP Unicast frame (DA MAC is unicast), trap to RouteDest;
*														  for IP multicast frames (DA MAC is multicast), mirror to RouteDest.
*			MSD_ROUTE_POLICY_ACTION_TRAP				- Trap frame to the RouteDest.
*			MSD_ROUTE_POLICY_ACTION_DISCARD				- Discard the frame.
*			MSD_ROUTE_POLICY_ACTION_BRIDGE				- Bridging only, no routing.
*			MSD_ROUTE_POLICY_ACTION_BRIDGE_MIRROR		- Bridge and Mirror to RouteDest.
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
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyIllegalIPAddrSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_ROUTE_POLICY_ACTION action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicySetIllegalIPAddr != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicySetIllegalIPAddr(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyIllegalIPAddrGet    
*
* DESCRIPTION:
*       This routine gets route policy action for Illegal IP Address.
*       Illegal IP Addresses are:
*       For IPv4 SIP:
*       127.0.0.0 to 127.255.255.255 (Loopback addr or 127.0.0.0/8)
*       224.0.0.0 to 239.255.255.255 (Multicast ClassD or 224.0.0.0/4)
*       255.255.255.255 (Limited Broadcast addr)
*       For IPv4 DIP:
*       0.0.0.0 (Unspecified addr)
*       127.0.0.0 to 127.255.255.255 (Loopback addr or 127.0.0.0/8)
*       For IPv6 SIP:
*       0.0.0.0.0.0.0.1 (Loopback addr or ::1/128)
*       FF00.0.0.0.0.0.0.0 to all ones (Multicast addr or FF::/8)
*       For IPv6 DIP:
*       0.0.0.0.0.0.0.0 (Unspecified addr or ::/128)
*       0.0.0.0.0.0.0.1 (Loopback addr or ::1/128)
*
* INPUTS:
*       portNum	- the logical port number.
*
* OUTPUTS:
*		action  - Enumeration of the route policy action.
*			MSD_ROUTE_POLICY_ACTION_NORMAL				- Normal frame switching, do nothing.
*			MSD_ROUTE_POLICY_ACTION_MIRROR_MC_TRAP_UC	- For IP Unicast frame (DA MAC is unicast), trap to RouteDest;
*														  for IP multicast frames (DA MAC is multicast), mirror to RouteDest.
*			MSD_ROUTE_POLICY_ACTION_TRAP				- Trap frame to the RouteDest.
*			MSD_ROUTE_POLICY_ACTION_DISCARD				- Discard the frame.
*			MSD_ROUTE_POLICY_ACTION_BRIDGE				- Bridging only, no routing.
*			MSD_ROUTE_POLICY_ACTION_BRIDGE_MIRROR		- Bridge and Mirror to RouteDest.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyIllegalIPAddrGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
OUT MSD_ROUTE_POLICY_ACTION   *action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicyGetIllegalIPAddr != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicyGetIllegalIPAddr(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyDipDaMismatchSet    
*
* DESCRIPTION:
*       This routine configures route policy action for DIP/DA mismatch. DIP/DA mismatch triggers are:
*       For IPv4:
*       If DA[40] = 0 and DIP[31:28] = 0xE
*       If DA[40] = 1 and DA <> Broadcast and:
*       DA[47:24] <> 01:00:5E or
*       DA[23] <> 0 or
*       DA[22:0] <> DIP[22:0] or
*       DIP[31:28] <> 0xE
*       For IPv6:
*       If DA[40] = 0 and DIP[127:120] = 0xFF
*       If DA[40] = 1 and DA <> Broadcast and:
*       DA[47:32] <> 33:33 or
*       DA[31:0] <> DIP[31:0] or
*       DIP[127:120] <> 0xFF
*
* INPUTS:
*       portNum	- the logical port number.
*		action  - Enumeration of the route policy action.
*			MSD_ROUTE_POLICY_ACTION_NORMAL				- Normal frame switching, do nothing.
*			MSD_ROUTE_POLICY_ACTION_MIRROR_MC_TRAP_UC	- For IP Unicast frame (DA MAC is unicast), trap to RouteDest;
*														  for IP multicast frames (DA MAC is multicast), mirror to RouteDest.
*			MSD_ROUTE_POLICY_ACTION_TRAP				- Trap frame to the RouteDest.
*			MSD_ROUTE_POLICY_ACTION_DISCARD				- Discard the frame.
*			MSD_ROUTE_POLICY_ACTION_BRIDGE				- Bridging only, no routing.
*			MSD_ROUTE_POLICY_ACTION_BRIDGE_MIRROR		- Bridge and Mirror to RouteDest.
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
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyDipDaMismatchSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_ROUTE_POLICY_ACTION  action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicySetDipDaMismatch != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicySetDipDaMismatch(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyDipDaMismatchGet    
*
* DESCRIPTION:
*       This routine gets route policy action for DIP/DA mismatch. DIP/DA mismatch triggers are:
*       For IPv4:
*       If DA[40] = 0 and DIP[31:28] = 0xE
*       If DA[40] = 1 and DA <> Broadcast and:
*       DA[47:24] <> 01:00:5E or
*       DA[23] <> 0 or
*       DA[22:0] <> DIP[22:0] or
*       DIP[31:28] <> 0xE
*       For IPv6:
*       If DA[40] = 0 and DIP[127:120] = 0xFF
*       If DA[40] = 1 and DA <> Broadcast and:
*       DA[47:32] <> 33:33 or
*       DA[31:0] <> DIP[31:0] or
*       DIP[127:120] <> 0xFF
*
* INPUTS:
*       portNum	- the logical port number.
*
* OUTPUTS:
*		action  - Enumeration of the route policy action.
*			MSD_ROUTE_POLICY_ACTION_NORMAL				- Normal frame switching, do nothing.
*			MSD_ROUTE_POLICY_ACTION_MIRROR_MC_TRAP_UC	- For IP Unicast frame (DA MAC is unicast), trap to RouteDest;
*														  for IP multicast frames (DA MAC is multicast), mirror to RouteDest.
*			MSD_ROUTE_POLICY_ACTION_TRAP				- Trap frame to the RouteDest.
*			MSD_ROUTE_POLICY_ACTION_DISCARD				- Discard the frame.
*			MSD_ROUTE_POLICY_ACTION_BRIDGE				- Bridging only, no routing.
*			MSD_ROUTE_POLICY_ACTION_BRIDGE_MIRROR		- Bridge and Mirror to RouteDest.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyDipDaMismatchGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
OUT MSD_ROUTE_POLICY_ACTION   *action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicyGetDipDaMismatch != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicyGetDipDaMismatch(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyZeroSIPSet    
*
* DESCRIPTION:
*       This routine configures route policy action for Zero SIP.
*
* INPUTS:
*       portNum	- the logical port number.
*		action  - Enumeration of the route policy action.
*			MSD_ROUTE_POLICY_ACTION_NORMAL				- Normal frame switching, do nothing.
*			MSD_ROUTE_POLICY_ACTION_MIRROR_MC_TRAP_UC	- For IP Unicast frame (DA MAC is unicast), trap to RouteDest;
*														  for IP multicast frames (DA MAC is multicast), mirror to RouteDest.
*			MSD_ROUTE_POLICY_ACTION_TRAP				- Trap frame to the RouteDest.
*			MSD_ROUTE_POLICY_ACTION_DISCARD				- Discard the frame.
*			MSD_ROUTE_POLICY_ACTION_BRIDGE				- Bridging only, no routing.
*			MSD_ROUTE_POLICY_ACTION_BRIDGE_MIRROR		- Bridge and Mirror to RouteDest.
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
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyZeroSIPSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_ROUTE_POLICY_ACTION  action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicySetZeroSIP != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicySetZeroSIP(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyZeroSIPGet    
*
* DESCRIPTION:
*       This routine gets route policy action for ZeroSIP. 
*
* INPUTS:
*       portNum	- the logical port number.
*
* OUTPUTS:
*		action  - Enumeration of the route policy action.
*			MSD_ROUTE_POLICY_ACTION_NORMAL				- Normal frame switching, do nothing.
*			MSD_ROUTE_POLICY_ACTION_MIRROR_MC_TRAP_UC	- For IP Unicast frame (DA MAC is unicast), trap to RouteDest;
*														  for IP multicast frames (DA MAC is multicast), mirror to RouteDest.
*			MSD_ROUTE_POLICY_ACTION_TRAP				- Trap frame to the RouteDest.
*			MSD_ROUTE_POLICY_ACTION_DISCARD				- Discard the frame.
*			MSD_ROUTE_POLICY_ACTION_BRIDGE				- Bridging only, no routing.
*			MSD_ROUTE_POLICY_ACTION_BRIDGE_MIRROR		- Bridge and Mirror to RouteDest.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyZeroSIPGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
OUT MSD_ROUTE_POLICY_ACTION   *action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicyGetZeroSIP != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicyGetZeroSIP(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyMcTtlExceededSet    
*
* DESCRIPTION:
*       This routine configures route policy action for multicast TTL/Hop-Limit Exceed.
*
* INPUTS:
*       portNum	- the logical port number.
*		action  - Enumeration of the route policy action.
*			MSD_ROUTE_POLICY_ACTION_NORMAL				- Normal frame switching, do nothing.
*			MSD_ROUTE_POLICY_ACTION_MIRROR_MC_TRAP_UC	- For IP Unicast frame (DA MAC is unicast), trap to RouteDest;
*														  for IP multicast frames (DA MAC is multicast), mirror to RouteDest.
*			MSD_ROUTE_POLICY_ACTION_TRAP				- Trap frame to the RouteDest.
*			MSD_ROUTE_POLICY_ACTION_DISCARD				- Discard the frame.
*			MSD_ROUTE_POLICY_ACTION_BRIDGE				- Bridging only, no routing.
*			MSD_ROUTE_POLICY_ACTION_BRIDGE_MIRROR		- Bridge and Mirror to RouteDest.
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
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyMcTtlExceededSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_ROUTE_POLICY_ACTION  action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicySetMcTtlExceeded != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicySetMcTtlExceeded(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyMcTtlExceededGet    
*
* DESCRIPTION:
*       This routine gets route policy action for multicast TTL/Hop-Limit Exceed. 
*
* INPUTS:
*       portNum	- the logical port number.
*
* OUTPUTS:
*		action  - Enumeration of the route policy action.
*			MSD_ROUTE_POLICY_ACTION_NORMAL				- Normal frame switching, do nothing.
*			MSD_ROUTE_POLICY_ACTION_MIRROR_MC_TRAP_UC	- For IP Unicast frame (DA MAC is unicast), trap to RouteDest;
*														  for IP multicast frames (DA MAC is multicast), mirror to RouteDest.
*			MSD_ROUTE_POLICY_ACTION_TRAP				- Trap frame to the RouteDest.
*			MSD_ROUTE_POLICY_ACTION_DISCARD				- Discard the frame.
*			MSD_ROUTE_POLICY_ACTION_BRIDGE				- Bridging only, no routing.
*			MSD_ROUTE_POLICY_ACTION_BRIDGE_MIRROR		- Bridge and Mirror to RouteDest.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyMcTtlExceededGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
OUT MSD_ROUTE_POLICY_ACTION   *action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicyGetMcTtlExceeded != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicyGetMcTtlExceeded(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyUcTtlExceededSet
*
* DESCRIPTION:
*       This routine configures route policy action for unicast TTL/Hop-Limit Exceed. 
*
* INPUTS:
*       portNum	- the logical port number.
*		action  - Enumeration of the route policy action.
*			MSD_ROUTE_POLICY_ACTION_NORMAL				- Normal frame switching, do nothing.
*			MSD_ROUTE_POLICY_ACTION_MIRROR_MC_TRAP_UC	- For IP Unicast frame (DA MAC is unicast), trap to RouteDest;
*														  for IP multicast frames (DA MAC is multicast), mirror to RouteDest.
*			MSD_ROUTE_POLICY_ACTION_TRAP				- Trap frame to the RouteDest.
*			MSD_ROUTE_POLICY_ACTION_DISCARD				- Discard the frame.
*			MSD_ROUTE_POLICY_ACTION_BRIDGE				- Bridging only, no routing.
*			MSD_ROUTE_POLICY_ACTION_BRIDGE_MIRROR		- Bridge and Mirror to RouteDest.
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
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyUcTtlExceededSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_ROUTE_POLICY_ACTION  action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicySetUcTtlExceeded != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicySetUcTtlExceeded(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyUcTtlExceededGet    
*
* DESCRIPTION:
*       This routine gets route policy action for unicast TTL/Hop-Limit Exceed. 
*
* INPUTS:
*       portNum	- the logical port number.
*
* OUTPUTS:
*		action  - Enumeration of the route policy action.
*			MSD_ROUTE_POLICY_ACTION_NORMAL				- Normal frame switching, do nothing.
*			MSD_ROUTE_POLICY_ACTION_MIRROR_MC_TRAP_UC	- For IP Unicast frame (DA MAC is unicast), trap to RouteDest;
*														  for IP multicast frames (DA MAC is multicast), mirror to RouteDest.
*			MSD_ROUTE_POLICY_ACTION_TRAP				- Trap frame to the RouteDest.
*			MSD_ROUTE_POLICY_ACTION_DISCARD				- Discard the frame.
*			MSD_ROUTE_POLICY_ACTION_BRIDGE				- Bridging only, no routing.
*			MSD_ROUTE_POLICY_ACTION_BRIDGE_MIRROR		- Bridge and Mirror to RouteDest.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyUcTtlExceededGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
OUT MSD_ROUTE_POLICY_ACTION   *action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicyGetUcTtlExceeded != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicyGetUcTtlExceeded(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPortTtlThresholdSet    
*
* DESCRIPTION:
*       This routine sets TTL/Hop-Limit Threshold. 
*
* INPUTS:
*       portNum	- the logical port number.
*		ttlThreshold  - Enumeration of the route policy TTL Threshold.
*			MSD_POLICY_TTL_THRESHOLD_1		- Check Packet¡¯s IPv4 TTL or IPv6 Hop-Limit ¡Ü 1.
*			MSD_POLICY_TTL_THRESHOLD_0		- Check Packet¡¯s IPv4 TTL or IPv6 Hop-Limit = 0.
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
*       None.
*
*******************************************************************************/
MSD_STATUS msdPortTtlThresholdSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_ROUTE_POLICY_TTL_THRESHOLD ttlThreshold
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gprtSetPortTtlThreshold != NULL)
		return dev->SwitchDevObj.POLICYObj.gprtSetPortTtlThreshold(dev, portNum, ttlThreshold);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPortTtlThresholdGet    
*
* DESCRIPTION:
*       This routine gets TTL/Hop-Limit Threshold. 
*
* INPUTS:
*       portNum	- the logical port number.
*
* OUTPUTS:
*		ttlThreshold  - Enumeration of the route policy TTL Threshold.
*			MSD_POLICY_TTL_THRESHOLD_1		- Check Packet¡¯s IPv4 TTL or IPv6 Hop-Limit ¡Ü 1.
*			MSD_POLICY_TTL_THRESHOLD_0		- Check Packet¡¯s IPv4 TTL or IPv6 Hop-Limit = 0.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdPortTtlThresholdGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
OUT MSD_ROUTE_POLICY_TTL_THRESHOLD *ttlThreshold
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gprtGetPortTtlThreshold != NULL)
		return dev->SwitchDevObj.POLICYObj.gprtGetPortTtlThreshold(dev, portNum, ttlThreshold);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyIpHdrErrSet    
*
* DESCRIPTION:
*       This routine configures route policy action for IP Header Error. An IP Header Error occurs 
*       when an IPv4 frame¡¯s IP Header or IPv6 frame¡¯s IP Header fails to pass any the following checks:
*       IPv4 Header Checksum
*       IPv4 Header Version = 4
*       IPv4 Header Length is ¡Ý5
*       IPv4 Header Length is ¡ÜTotal Length/4
*       IPv4 Header SIP ¡ÙDIP
*       IPv6 Header Version = 6
*       IPv6 Header SIP ¡ÙDIP
*
* INPUTS:
*       portNum	- the logical port number.
*		action  - Enumeration of the route policy action.
*			MSD_ROUTE_POLICY_ACTION_NORMAL				- Normal frame switching, do nothing.
*			MSD_ROUTE_POLICY_ACTION_MIRROR_MC_TRAP_UC	- For IP Unicast frame (DA MAC is unicast), trap to RouteDest;
*														  for IP multicast frames (DA MAC is multicast), mirror to RouteDest.
*			MSD_ROUTE_POLICY_ACTION_TRAP				- Trap frame to the RouteDest.
*			MSD_ROUTE_POLICY_ACTION_DISCARD				- Discard the frame.
*			MSD_ROUTE_POLICY_ACTION_BRIDGE				- Bridging only, no routing.
*			MSD_ROUTE_POLICY_ACTION_BRIDGE_MIRROR		- Bridge and Mirror to RouteDest.
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
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyIpHdrErrSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_ROUTE_POLICY_ACTION  action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicySetIpHdrErr != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicySetIpHdrErr(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyIpHdrErrGet
*
* DESCRIPTION:
*       This routine gets route policy action for IP Header Error. An IP Header Error occurs
*       when an IPv4 frame¡¯s IP Header or IPv6 frame¡¯s IP Header fails to pass any the following checks:
*       IPv4 Header Checksum
*       IPv4 Header Version = 4
*       IPv4 Header Length is ¡Ý5
*       IPv4 Header Length is ¡ÜTotal Length/4
*       IPv4 Header SIP ¡ÙDIP
*       IPv6 Header Version = 6
*       IPv6 Header SIP ¡ÙDIP
*
* INPUTS:
*       portNum	- the logical port number.
*
* OUTPUTS:
*		action  - Enumeration of the route policy action.
*			MSD_ROUTE_POLICY_ACTION_NORMAL				- Normal frame switching, do nothing.
*			MSD_ROUTE_POLICY_ACTION_MIRROR_MC_TRAP_UC	- For IP Unicast frame (DA MAC is unicast), trap to RouteDest;
*														  for IP multicast frames (DA MAC is multicast), mirror to RouteDest.
*			MSD_ROUTE_POLICY_ACTION_TRAP				- Trap frame to the RouteDest.
*			MSD_ROUTE_POLICY_ACTION_DISCARD				- Discard the frame.
*			MSD_ROUTE_POLICY_ACTION_BRIDGE				- Bridging only, no routing.
*			MSD_ROUTE_POLICY_ACTION_BRIDGE_MIRROR		- Bridge and Mirror to RouteDest.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyIpHdrErrGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
OUT MSD_ROUTE_POLICY_ACTION   *action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicyGetIpHdrErr != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicyGetIpHdrErr(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyTCPFinUrgPshSet
*
* DESCRIPTION:
*       This routine configures policy action for the port when receiving one TCP frame and 
*       frame¡¯s TCP FIN, URG and PSH flags are all equal to one and the TCP sequence number is zero.
*
* INPUTS:
*       portNum	- the logical port number.
*		action  - Enumeration of the Dos policy action.
*			MSD_POLICY_ACTION_NORMAL    - Normal frame switching, do nothing.
*			MSD_POLICY_ACTION_MIRROR	- Mirror frame to MirrorDest.
*			MSD_POLICY_ACTION_TRAP		- Trap frame to the CPUDest.
*			MSD_POLICY_ACTION_DISCARD	- Discard the frame.
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
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyTCPFinUrgPshSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_POLICY_ACTION  action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicySetTCPFinUrgPsh != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicySetTCPFinUrgPsh(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyTCPFinUrgPshGet    
*
* DESCRIPTION:
*       This routine gets policy action for the port when receiving the TCP frame and the frame¡¯s
*       TCP FIN, URG and PSH flags are all equal to one and the TCP sequence number is zero.
*
* INPUTS:
*       portNum	- the logical port number.
*
* OUTPUTS:
*		action  - Enumeration of the Dos policy action.
*			MSD_POLICY_ACTION_NORMAL    - Normal frame switching, do nothing.
*			MSD_POLICY_ACTION_MIRROR	- Mirror frame to MirrorDest.
*			MSD_POLICY_ACTION_TRAP		- Trap frame to the CPUDest.
*			MSD_POLICY_ACTION_DISCARD	- Discard the frame.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyTCPFinUrgPshGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
OUT MSD_POLICY_ACTION   *action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicyGetTCPFinUrgPsh != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicyGetTCPFinUrgPsh(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyTCPFlagsSeqNumSet    
*
* DESCRIPTION:
*       This routine configures policy action when receiving TCP frame and its 
*       TCP flags are all zero and TCP sequence number is also zero.
*
* INPUTS:
*       portNum	- the logical port number.
*		action  - Enumeration of the Dos policy action.
*			MSD_POLICY_ACTION_NORMAL    - Normal frame switching, do nothing.
*			MSD_POLICY_ACTION_MIRROR	- Mirror frame to MirrorDest.
*			MSD_POLICY_ACTION_TRAP		- Trap frame to the CPUDest.
*			MSD_POLICY_ACTION_DISCARD	- Discard the frame.
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
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyTCPFlagsSeqNumSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_POLICY_ACTION  action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicySetTCPFlagsSeqNum != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicySetTCPFlagsSeqNum(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyTCPFlagsSeqNumGet    
*
* DESCRIPTION:
*       This routine configures policy action when receiving TCP frame and its 
*       TCP flags are all zero and TCP sequence number is also zero.
*
* INPUTS:
*       portNum	- the logical port number.
*
* OUTPUTS:
*		action  - Enumeration of the Dos policy action.
*			MSD_POLICY_ACTION_NORMAL    - Normal frame switching, do nothing.
*			MSD_POLICY_ACTION_MIRROR	- Mirror frame to MirrorDest.
*			MSD_POLICY_ACTION_TRAP		- Trap frame to the CPUDest.
*			MSD_POLICY_ACTION_DISCARD	- Discard the frame.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyTCPFlagsSeqNumGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
OUT MSD_POLICY_ACTION   *action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicyGetTCPFlagsSeqNum != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicyGetTCPFlagsSeqNum(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyTCPSYNSet    
*
* DESCRIPTION:
*       This routine configures policy action when receiving TCP frame and its 
*       TCP SYN flag is set to a one and TCP Source Port is less than 1024.
*
* INPUTS:
*       portNum	- the logical port number.
*		action  - Enumeration of the Dos policy action.
*			MSD_POLICY_ACTION_NORMAL    - Normal frame switching, do nothing.
*			MSD_POLICY_ACTION_MIRROR	- Mirror frame to MirrorDest.
*			MSD_POLICY_ACTION_TRAP		- Trap frame to the CPUDest.
*			MSD_POLICY_ACTION_DISCARD	- Discard the frame.
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
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyTCPSYNSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_POLICY_ACTION  action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicySetTCPSYN != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicySetTCPSYN(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyTCPSYNGet    
*
* DESCRIPTION:
*       This routine gets policy action when receiving TCP frame and its 
*       TCP SYN flag is set to a one and TCP Source Port is less than 1024.
*
* INPUTS:
*       portNum	- the logical port number.
*
* OUTPUTS:
*		action  - Enumeration of the route policy action.
*		action  - Enumeration of the Dos policy action.
*			MSD_POLICY_ACTION_NORMAL    - Normal frame switching, do nothing.
*			MSD_POLICY_ACTION_MIRROR	- Mirror frame to MirrorDest.
*			MSD_POLICY_ACTION_TRAP		- Trap frame to the CPUDest.
*			MSD_POLICY_ACTION_DISCARD	- Discard the frame.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyTCPSYNGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
OUT MSD_POLICY_ACTION   *action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicyGetTCPSYN != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicyGetTCPSYN(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyTCPHSizeSet    
*
* DESCRIPTION:
*       This routine configures policy action when receiving TCP frame and its 
*       data offset field is less than the configured MinTCPHdrSize.
*
* INPUTS:
*       portNum	- the logical port number.
*		action  - Enumeration of the Dos policy action.
*			MSD_POLICY_ACTION_NORMAL    - Normal frame switching, do nothing.
*			MSD_POLICY_ACTION_MIRROR	- Mirror frame to MirrorDest.
*			MSD_POLICY_ACTION_TRAP		- Trap frame to the CPUDest.
*			MSD_POLICY_ACTION_DISCARD	- Discard the frame.
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
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyTCPHSizeSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_POLICY_ACTION  action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicySetTCPHSize != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicySetTCPHSize(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyTCPHSizeGet    
*
* DESCRIPTION:
*       This routine gets policy action when receiving TCP frame and its 
*       data offset field is less than the configured MinTCPHdrSize.
*
* INPUTS:
*       portNum	- the logical port number.
*
* OUTPUTS:
*		action  - Enumeration of the Dos policy action.
*			MSD_POLICY_ACTION_NORMAL    - Normal frame switching, do nothing.
*			MSD_POLICY_ACTION_MIRROR	- Mirror frame to MirrorDest.
*			MSD_POLICY_ACTION_TRAP		- Trap frame to the CPUDest.
*			MSD_POLICY_ACTION_DISCARD	- Discard the frame.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyTCPHSizeGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
OUT MSD_POLICY_ACTION   *action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicyGetTCPHSize != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicyGetTCPHSize(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPortTCPMinHdrSizeSet    
*
* DESCRIPTION:
*       This routine configures MinTCPHdrSize. 
*
* INPUTS:
*       portNum	- the logical port number.
*		minTCPHdrSize  - Minimum TCP Header Size.
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
*       None.
*
*******************************************************************************/
MSD_STATUS msdPortTCPMinHdrSizeSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_U8		minTCPHdrSize
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gprtSetPortTCPMinHdrSize != NULL)
		return dev->SwitchDevObj.POLICYObj.gprtSetPortTCPMinHdrSize(dev, portNum, minTCPHdrSize);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPortTCPMinHdrSizeGet    
*
* DESCRIPTION:
*       This routine gets MinTCPHdrSize. 
*
* INPUTS:
*       portNum	- the logical port number.
*
* OUTPUTS:
*		minTCPHdrSize  - Minimum TCP Header Size.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdPortTCPMinHdrSizeGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
OUT MSD_U8		*minTCPHdrSize
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gprtGetPortTCPMinHdrSize != NULL)
		return dev->SwitchDevObj.POLICYObj.gprtGetPortTCPMinHdrSize(dev, portNum, minTCPHdrSize);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyICMPSizeSet    
*
* DESCRIPTION:
*       This routine configures  policy action when receiving ICMP ping packet size exceeds
*		the configured MaxICMPSize.
*
* INPUTS:
*       portNum	- the logical port number.
*		action  - Enumeration of the Dos policy action.
*			MSD_POLICY_ACTION_NORMAL    - Normal frame switching, do nothing.
*			MSD_POLICY_ACTION_MIRROR	- Mirror frame to MirrorDest.
*			MSD_POLICY_ACTION_TRAP		- Trap frame to the CPUDest.
*			MSD_POLICY_ACTION_DISCARD	- Discard the frame.
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
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyICMPSizeSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_POLICY_ACTION action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicySetICMPSize != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicySetICMPSize(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyICMPSizeGet        
*
* DESCRIPTION:
*       This routine gets policy action when receiving ICMP ping packet size exceeds 
*		the configured MaxICMPSize. 
*
* INPUTS:
*       portNum	- the logical port number.
*
* OUTPUTS:
*		action  - Enumeration of the Dos policy action.
*			MSD_POLICY_ACTION_NORMAL    - Normal frame switching, do nothing.
*			MSD_POLICY_ACTION_MIRROR	- Mirror frame to MirrorDest.
*			MSD_POLICY_ACTION_TRAP		- Trap frame to the CPUDest.
*			MSD_POLICY_ACTION_DISCARD	- Discard the frame.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyICMPSizeGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
OUT MSD_POLICY_ACTION *action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicyGetICMPSize != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicyGetICMPSize(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPortICMPMaxSizeSet    
*
* DESCRIPTION:
*       This routine configures MaxICMPSize.
*
* INPUTS:
*       portNum	- the logical port number.
*		maxICMPSize  - Maximum ICMP Ping Packet Payload Size.
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
*       None.
*
*******************************************************************************/
MSD_STATUS msdPortICMPMaxSizeSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_U16		maxICMPSize
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gprtSetPortICMPMaxSize != NULL)
		return dev->SwitchDevObj.POLICYObj.gprtSetPortICMPMaxSize(dev, portNum, maxICMPSize);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPortICMPMaxSizeGet
*
* DESCRIPTION:
*       This routine gets Max ICMP Size. 
*
* INPUTS:
*       portNum	- the logical port number.
*
* OUTPUTS:
*		maxICMPSize  - Maximum ICMP Ping Packet Payload Size.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdPortICMPMaxSizeGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
OUT MSD_U16		*maxICMPSize
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gprtGetPortICMPMaxSize != NULL)
		return dev->SwitchDevObj.POLICYObj.gprtGetPortICMPMaxSize(dev, portNum, maxICMPSize);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyUDPPortSet    
*
* DESCRIPTION:
*       This routine configures policy action when receiving UDP frame with its 
*		Source Port equal to its Destination Port. 
*
* INPUTS:
*       portNum	- the logical port number.
*		action  - Enumeration of the Dos policy action.
*			MSD_POLICY_ACTION_NORMAL    - Normal frame switching, do nothing.
*			MSD_POLICY_ACTION_MIRROR	- Mirror frame to MirrorDest.
*			MSD_POLICY_ACTION_TRAP		- Trap frame to the CPUDest.
*			MSD_POLICY_ACTION_DISCARD	- Discard the frame.
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
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyUDPPortSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_POLICY_ACTION action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicySetUDPPort != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicySetUDPPort(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyUDPPortGet    
*
* DESCRIPTION:
*       This routine gets policy action when receiving UDP frame with its 
*		Source Port equal to its Destination Port. 
*
* INPUTS:
*       portNum	- the logical port number.
*
* OUTPUTS:
*		action  - Enumeration of the Dos policy action.
*			MSD_POLICY_ACTION_NORMAL    - Normal frame switching, do nothing.
*			MSD_POLICY_ACTION_MIRROR	- Mirror frame to MirrorDest.
*			MSD_POLICY_ACTION_TRAP		- Trap frame to the CPUDest.
*			MSD_POLICY_ACTION_DISCARD	- Discard the frame.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyUDPPortGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
OUT MSD_POLICY_ACTION *action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicyGetUDPPort != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicyGetUDPPort(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyTCPPortSet    
*
* DESCRIPTION:
*       This routine configures policy action when receiving TCP frame with its 
*		Source Port equal to its Destination Port. 
*
* INPUTS:
*       portNum	- the logical port number.
*		action  - Enumeration of the Dos policy action.
*			MSD_POLICY_ACTION_NORMAL    - Normal frame switching, do nothing.
*			MSD_POLICY_ACTION_MIRROR	- Mirror frame to MirrorDest.
*			MSD_POLICY_ACTION_TRAP		- Trap frame to the CPUDest.
*			MSD_POLICY_ACTION_DISCARD	- Discard the frame.
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
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyTCPPortSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_POLICY_ACTION action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicySetTCPPort != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicySetTCPPort(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyTCPPortGet    
*
* DESCRIPTION:
*       This routine gets policy action when receiving TCP frame with its 
*		Source Port equal to its Destination Port. 
*
* INPUTS:
*       portNum	- the logical port number.
*
* OUTPUTS:
*		action  - Enumeration of the Dos policy action.
*			MSD_POLICY_ACTION_NORMAL    - Normal frame switching, do nothing.
*			MSD_POLICY_ACTION_MIRROR	- Mirror frame to MirrorDest.
*			MSD_POLICY_ACTION_TRAP		- Trap frame to the CPUDest.
*			MSD_POLICY_ACTION_DISCARD	- Discard the frame.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyTCPPortGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
OUT MSD_POLICY_ACTION *action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicyGetTCPPort != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicyGetTCPPort(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyTCPSynFinSet    
*
* DESCRIPTION:
*       This routine configures policy action when receiving TCP frame and its 
*		TCP SYN and FIN flag is set to a one. 
*
* INPUTS:
*       portNum	- the logical port number.
*		action  - Enumeration of the Dos policy action.
*			MSD_POLICY_ACTION_NORMAL    - Normal frame switching, do nothing.
*			MSD_POLICY_ACTION_MIRROR	- Mirror frame to MirrorDest.
*			MSD_POLICY_ACTION_TRAP		- Trap frame to the CPUDest.
*			MSD_POLICY_ACTION_DISCARD	- Discard the frame.
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
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyTCPSynFinSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_POLICY_ACTION action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicySetTCPSynFin != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicySetTCPSynFin(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyTCPSynFinGet    
*
* DESCRIPTION:
*       This routine get policy action when receiving TCP frame and its
*		TCP SYN and FIN flag is set to a one. 
*
* INPUTS:
*       portNum	- the logical port number.
*
* OUTPUTS:
*		action  - Enumeration of the Dos policy action.
*			MSD_POLICY_ACTION_NORMAL    - Normal frame switching, do nothing.
*			MSD_POLICY_ACTION_MIRROR	- Mirror frame to MirrorDest.
*			MSD_POLICY_ACTION_TRAP		- Trap frame to the CPUDest.
*			MSD_POLICY_ACTION_DISCARD	- Discard the frame.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyTCPSynFinGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
OUT MSD_POLICY_ACTION *action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicyGetTCPSynFin != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicyGetTCPSynFin(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyTCPFragSet    
*
* DESCRIPTION:
*       This routine configures policy action when receiving TCP frame with 
*		an IP fragment offset equal to 1. 
*
* INPUTS:
*       portNum	- the logical port number.
*		action  - Enumeration of the Dos policy action.
*			MSD_POLICY_ACTION_NORMAL    - Normal frame switching, do nothing.
*			MSD_POLICY_ACTION_MIRROR	- Mirror frame to MirrorDest.
*			MSD_POLICY_ACTION_TRAP		- Trap frame to the CPUDest.
*			MSD_POLICY_ACTION_DISCARD	- Discard the frame.
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
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyTCPFragSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_POLICY_ACTION action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicySetTCPFrag != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicySetTCPFrag(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyTCPFragGet    
*
* DESCRIPTION:
*       This routine gets policy action when receiving TCP frame with 
*		an IP fragment offset equal to 1. 
*
* INPUTS:
*       portNum	- the logical port number.
*
* OUTPUTS:
*		action  - Enumeration of the Dos policy action.
*			MSD_POLICY_ACTION_NORMAL    - Normal frame switching, do nothing.
*			MSD_POLICY_ACTION_MIRROR	- Mirror frame to MirrorDest.
*			MSD_POLICY_ACTION_TRAP		- Trap frame to the CPUDest.
*			MSD_POLICY_ACTION_DISCARD	- Discard the frame.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyTCPFragGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
OUT MSD_POLICY_ACTION *action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicyGetTCPFrag != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicyGetTCPFrag(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyNearNonTPMRSet        
*
* DESCRIPTION:
*       This routine configures policy action when the ingressing frame is 
*		not E-Tagged, but contains a DA of 01:80:C2:00:00:03. 
*
* INPUTS:
*       portNum	- the logical port number.
*		action  - Enumeration of the PE policy action.
*			MSD_POLICY_ACTION_NORMAL    - Normal frame switching, do nothing.
*			MSD_POLICY_ACTION_MIRROR	- Mirror frame to MirrorDest.
*			MSD_POLICY_ACTION_TRAP		- Trap frame to the CPUDest.
*			MSD_POLICY_ACTION_DISCARD	- Discard the frame.
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
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyNearNonTPMRSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_POLICY_ACTION action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicySetNearNonTPMR != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicySetNearNonTPMR(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyNearNonTPMRGet    
*
* DESCRIPTION:
*       This routine gets policy action when the ingressing frame is 
*		not E-Tagged, but contains a DA of 01:80:C2:00:00:03. 
*
* INPUTS:
*       portNum	- the logical port number.
*
* OUTPUTS:
*		action  - Enumeration of the PE policy action.
*			MSD_POLICY_ACTION_NORMAL    - Normal frame switching, do nothing.
*			MSD_POLICY_ACTION_MIRROR	- Mirror frame to MirrorDest.
*			MSD_POLICY_ACTION_TRAP		- Trap frame to the CPUDest.
*			MSD_POLICY_ACTION_DISCARD	- Discard the frame.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyNearNonTPMRGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
OUT MSD_POLICY_ACTION *action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicyGetNearNonTPMR != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicyGetNearNonTPMR(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyETagSet    
*
* DESCRIPTION:
*       This routine configures policy action when the ingressing frame contains an E-Tagged. 
*
* INPUTS:
*       portNum	- the logical port number.
*		action  - Enumeration of the PE policy action.
*			MSD_POLICY_ACTION_NORMAL    - Normal frame switching, do nothing.
*			MSD_POLICY_ACTION_MIRROR	- Mirror frame to MirrorDest.
*			MSD_POLICY_ACTION_TRAP		- Trap frame to the CPUDest.
*			MSD_POLICY_ACTION_DISCARD	- Discard the frame.
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
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyETagSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_POLICY_ACTION action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicySetETag != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicySetETag(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyETagGet    
*
* DESCRIPTION:
*       This routine gets policy action when the ingressing frame is E-Tagged. 
*
* INPUTS:
*       portNum	- the logical port number.
*
* OUTPUTS:
*		action  - Enumeration of the PE policy action.
*			MSD_POLICY_ACTION_NORMAL    - Normal frame switching, do nothing.
*			MSD_POLICY_ACTION_MIRROR	- Mirror frame to MirrorDest.
*			MSD_POLICY_ACTION_TRAP		- Trap frame to the CPUDest.
*			MSD_POLICY_ACTION_DISCARD	- Discard the frame.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyETagGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
OUT MSD_POLICY_ACTION *action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicyGetETag != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicyGetETag(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyNonETagSet    
*
* DESCRIPTION:
*       This routine configures policy action when the ingressing frame does NOT contain E-Tagged. 
*
* INPUTS:
*       portNum	- the logical port number.
*		action  - Enumeration of the PE policy action.
*			MSD_POLICY_ACTION_NORMAL    - Normal frame switching, do nothing.
*			MSD_POLICY_ACTION_MIRROR	- Mirror frame to MirrorDest.
*			MSD_POLICY_ACTION_TRAP		- Trap frame to the CPUDest.
*			MSD_POLICY_ACTION_DISCARD	- Discard the frame.
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
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyNonETagSet
(
IN MSD_U8		devNum,
IN MSD_LPORT	portNum,
IN MSD_POLICY_ACTION action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicySetNonETag != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicySetNonETag(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}

/*******************************************************************************
* msdPolicyNonETagGet    
*
* DESCRIPTION:
*       This routine gets policy action when the ingressing frame does NOT contain E-Tagged. 
*
* INPUTS:
*       portNum	- the logical port number.
*
* OUTPUTS:
*		action  - Enumeration of the PE policy action.
*			MSD_POLICY_ACTION_NORMAL    - Normal frame switching, do nothing.
*			MSD_POLICY_ACTION_MIRROR	- Mirror frame to MirrorDest.
*			MSD_POLICY_ACTION_TRAP		- Trap frame to the CPUDest.
*			MSD_POLICY_ACTION_DISCARD	- Discard the frame.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None.
*
*******************************************************************************/
MSD_STATUS msdPolicyNonETagGet
(
IN  MSD_U8		devNum,
IN  MSD_LPORT	portNum,
OUT MSD_POLICY_ACTION *action
)
{
	MSD_QD_DEV* dev = sohoDevGet(devNum);
	if (NULL == dev)
	{
		MSD_DBG_ERROR(("Dev is NULL for devNum %d.\n", devNum));
		return MSD_FAIL;
	}

	if (dev->SwitchDevObj.POLICYObj.gpolicyGetNonETag != NULL)
		return dev->SwitchDevObj.POLICYObj.gpolicyGetNonETag(dev, portNum, action);
	else
		return MSD_NOT_SUPPORTED;
}
#endif /*#if FEATURE_MARVELL_6321*/
