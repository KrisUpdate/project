#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"
/*******************************************************************************
* Willow_msdPortPrioMap.h
*
* DESCRIPTION:
*       API/Structure definitions for Marvell Port Priority Mapping Rules functionality.
*
* DEPENDENCIES:
*       None.
*
* FILE REVISION NUMBER:
*******************************************************************************/

#ifndef __Willow_msdPortPrioMap_h
#define __Willow_msdPortPrioMap_h

#include "msdApiTypes.h"
#include "msdSysConfig.h"


#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************/
/* Exported Port Control Types			 			                                   */
/****************************************************************************/

/*
 * Typedef: enum WILLOW_MSD_INITIAL_PRI
 *
 * Description: Defines Initial Priority
 *
 * Fields:
 *      WILLOW_MSD_INITIAL_PRI_DEFAULT	- Use Port defauts for Fpri, Qpri&Color.
 *      WILLOW_MSD_INITIAL_PRI_TAG		- Use Tag Priority.
 *      WILLOW_MSD_INITIAL_PRI_IP		- Use IP Priority.
 *		WILLOW_MSD_INITIAL_PRI_TAG_IP	- Use Tag&IP Priority.
 *                 
 */
typedef enum
{
    WILLOW_MSD_INITIAL_PRI_DEFAULT= 0,
    WILLOW_MSD_INITIAL_PRI_TAG,
    WILLOW_MSD_INITIAL_PRI_IP,
    WILLOW_MSD_INITIAL_PRI_TAG_IP
} WILLOW_MSD_INITIAL_PRI;

/* Supported Frame Priority Table set For IEEE priority mapping */
typedef enum
{
  WILLOW_MSD_IEEE_PRIORITY_TABLE_ING_PCP_2_F_Q_PRI,   /* 0 Ingress PCP to IEEE_FPRI & IEEE_QPRI mapping */
  WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_GREEN_FPRI_2_PCP, /* 0x1 Egress Green Frame FPRI to PCP mapping (non-AVB) */
  WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_YEL_FPRI_2_PCP,   /* 0x2 = Egress Yellow Frame FPRI to PCP mapping (non-AVB) */
  WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_AVB_FPRI_2_PCP,   /* 0x3 = Egress AVB Frame FPRI to PCP mapping */
  WILLOW_MSD_IEEE_PRIORITY_TABLE_RESERVED         ,   /* 0x4 = Reserved for future use */
  WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_GREEN_FPRI_2_DSCP,/* 0x5 = Egress Green Frame FPRI to DSCP mapping (non-AVB) */
  WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_YEL_FPRI_2_DSCP,  /* 0x6 = Egress Yellow Frame FPRI to DSCP mapping (non-AVB) */
  WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_AVB_FPRI_2_DSCP,  /* 0x7 = Egress AVB Frame FPRI to DSCP mapping */
} WILLOW_MSD_IEEE_PRIORITY_TABLE_SET;

/* Supported the format of each Table set For IEEE priority mapping */
typedef struct
{
	MSD_U8     ieeeYellow;
	MSD_U8     disIeeeQpri;
	MSD_U8     ieeeQpri;
	MSD_U8     disIeeeFpri;
	MSD_U8     ieeeFpri;
} WILLOW_MSD_IEEE_PRIORITY_TABLE_ING_PCP_2_F_Q_PRI_STRUCT;
typedef struct
{
	MSD_U8     disVIDMarking;
	MSD_U8     disPCPMarking;
	MSD_U8     framePCP;
} WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_GREEN_FPRI_2_PCP_STRUCT;
typedef struct
{
	MSD_U8     disVIDMarking;
	MSD_U8     disPCPMarking;
	MSD_U8     framePCP;
} WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_YEL_FPRI_2_PCP_STRUCT;
typedef struct
{
	MSD_U8     disVIDMarking;
	MSD_U8     disPCPMarking;
	MSD_U8     framePCP;
} WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_AVB_FPRI_2_PCP_STRUCT;
typedef struct
{
	MSD_U8     enDSCPMarking;
	MSD_U8     frameDSCP;
} WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_GREEN_FPRI_2_DSCP_STRUCT;
typedef struct
{
	MSD_U8     enDSCPMarking;
	MSD_U8     frameDSCP;
} WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_YEL_FPRI_2_DSCP_STRUCT;
typedef struct
{
	MSD_U8     enDSCPMarking;
	MSD_U8     frameDSCP;
} WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_AVB_FPRI_2_DSCP_STRUCT;

typedef union
{
  WILLOW_MSD_IEEE_PRIORITY_TABLE_ING_PCP_2_F_Q_PRI_STRUCT     ingTbl;
  WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_GREEN_FPRI_2_PCP_STRUCT   egGreeTblPcp;
  WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_YEL_FPRI_2_PCP_STRUCT     egYelTblPcp;
  WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_AVB_FPRI_2_PCP_STRUCT     egAvbTblPcp;
  WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_GREEN_FPRI_2_DSCP_STRUCT  egGreeTblDscp;
  WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_YEL_FPRI_2_DSCP_STRUCT    egYelTblDscp;
  WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_AVB_FPRI_2_DSCP_STRUCT    egAvbTbldscp;
} WILLOW_MSD_IEEE_PRIORITY_TABLE_STRUCT;

/****************************************************************************/
/* Exported Port Control Functions		 			                                   */
/****************************************************************************/

/*******************************************************************************
* Willow_gqosSetPrioMapRule
*
* DESCRIPTION:
*       This routine sets priority mapping rule(TagIfBoth bit).
*       If the current frame is both IEEE 802.3ac tagged and an IPv4 or IPv6, 
*       and UserPrioMap (for IEEE 802.3ac) and IPPrioMap (for IP frame) are 
*       enabled, then priority selection is made based on this setup.
*       If PrioMapRule is set to MSD_TRUE, UserPrioMap is used.
*       If PrioMapRule is reset to MSD_FALSE, IPPrioMap is used.
*
* INPUTS:
*       port - the logical port number.
*       mode - MSD_TRUE for user prio rule, MSD_FALSE for otherwise.
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
MSD_STATUS Willow_gqosSetPrioMapRule
(
    IN  MSD_QD_DEV  *dev,
    IN  MSD_LPORT   port,
    IN  MSD_BOOL    mode
);
MSD_STATUS Willow_gqosSetPrioMapRuleIntf
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT   port,
IN  MSD_BOOL    mode
);
/*******************************************************************************
* Willow_gqosGetPrioMapRule
*
* DESCRIPTION:
*       This routine gets priority mapping rule (TagIfBoth bit).
*        If the current frame is both IEEE 802.3ac tagged and an IPv4 or IPv6, 
*        and UserPrioMap (for IEEE 802.3ac) and IPPrioMap (for IP frame) are 
*        enabled, then priority selection is made based on this setup.
*        If PrioMapRule is set to MSD_TRUE, UserPrioMap is used.
*        If PrioMapRule is reset to MSD_FALSE, IPPrioMap is used.
*
* INPUTS:
*       port  - the logical port number.
*
* OUTPUTS:
*       mode - MSD_TRUE for user prio rule, MSD_FALSE for otherwise.
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
MSD_STATUS Willow_gqosGetPrioMapRule
(
    IN  MSD_QD_DEV *dev,
    IN  MSD_LPORT  port,
    OUT MSD_BOOL   *mode
);
MSD_STATUS Willow_gqosGetPrioMapRuleIntf
(
IN  MSD_QD_DEV *dev,
IN  MSD_LPORT  port,
OUT MSD_BOOL   *mode
);
/*******************************************************************************
* Willow_gprtSetInitialPri
*
* DESCRIPTION:
*       This routine set initial Priority assignment.
*       Each frame entering a port is assigned a Frame Priority(FPri) and 
*       a Queue Priority (QPri). This mode can be assigned as follows:
*            WILLOW_MSD_INITIAL_PRI_DEFAULT
*               
*            WILLOW_MSD_INITIAL_PRI_TAG
*                
*            WILLOW_MSD_INITIAL_PRI_IP
*                
*            WILLOW_MSD_INITIAL_PRI_TAG_IP
*                
*
* INPUTS:
*       port - the logical port number.
*       mode - WILLOW_MSD_INITIAL_PRI type
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
MSD_STATUS Willow_gprtSetInitialPri
(
    IN  MSD_QD_DEV		*dev,
    IN  MSD_LPORT		port,
    IN  WILLOW_MSD_INITIAL_PRI  mode
);
MSD_STATUS Willow_gprtSetInitialPriIntf
(
    IN  MSD_QD_DEV		*dev,
    IN  MSD_LPORT		port,
    IN  MSD_INITIAL_PRI  mode
);
/*******************************************************************************
* Willow_gprtGetInitialPri
*
* DESCRIPTION:
*       This routine gets initial Priority assignment.
*       Each frame entering a port is assigned a Frame Priority(FPri) and 
*       a Queue Priority (QPri). This mode can be assigned as follows:
*            WILLOW_MSD_INITIAL_PRI_DEFAULT
*               
*            WILLOW_MSD_INITIAL_PRI_TAG
*                
*            WILLOW_MSD_INITIAL_PRI_IP
*                
*            WILLOW_MSD_INITIAL_PRI_TAG_IP
*
* INPUTS:
*       port - the logical port number.
*
* OUTPUTS:
*       mode - WILLOW_MSD_INITIAL_PRI type
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
MSD_STATUS Willow_gprtGetInitialPri
(
    IN  MSD_QD_DEV		*dev,
    IN  MSD_LPORT		port,
    OUT WILLOW_MSD_INITIAL_PRI  *mode
);
MSD_STATUS Willow_gprtGetInitialPriIntf
(
    IN  MSD_QD_DEV		*dev,
    IN  MSD_LPORT		port,
    OUT MSD_INITIAL_PRI  *mode
);
/*******************************************************************************
* Willow_gprtSetDefQpri
*
* DESCRIPTION:
*       This routine Set Default queue priority.
*
* INPUTS:
*       port - the logical port number
*       defque - default queue priority (0~7)
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
MSD_STATUS Willow_gprtSetDefQpri
(
  IN  MSD_QD_DEV    *dev,
  IN  MSD_LPORT    port,
  IN  MSD_U8  	  defque
);
MSD_STATUS Willow_gprtSetDefQpriIntf
(
  IN  MSD_QD_DEV    *dev,
  IN  MSD_LPORT    port,
  IN  MSD_U8  	  defque
);
/*******************************************************************************
* Willow_gprtGetDefQpri
*
* DESCRIPTION:
*       This routine Get Default queue priority.
*
* INPUTS:
*       port - the logical port number
*
* OUTPUTS:
*       defque - default queue priority structure
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
MSD_STATUS Willow_gprtGetDefQpri
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT    port,
    OUT MSD_U8       *defque
);
MSD_STATUS Willow_gprtGetDefQpriIntf
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT    port,
    OUT MSD_U8       *defque
);
/*******************************************************************************
* Willow_gprtSetIEEEPrioMap
*
* DESCRIPTION:
*       This API set port IEEE Priority Mapping.
*
* INPUTS:
*       port		- the logical port number
*       table		- Frame Priority Table set
*						WILLOW_MSD_IEEE_PRIORITY_TABLE_ING_PCP_2_F_Q_PRI
*						WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_GREEN_FPRI_2_PCP
*						WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_YEL_FPRI_2_PCP
*						WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_AVB_FPRI_2_PCP
*						WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_GREEN_FPRI_2_DSCP
*						WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_YEL_FPRI_2_DSCP
*						WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_AVB_FPRI_2_DSCP
*       entryPtr		- Pointer to the desired entry of the IEEE Priority  
*						  Mapping table selected above
*       entryStruct	- Mapping data structure for the above selected entry.
*							WILLOW_MSD_IEEE_PRIORITY_TABLE_ING_PCP_2_F_Q_PRI_STRUCT;
*							WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_GREEN_FPRI_2_PCP_STRUCT;
*							WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_YEL_FPRI_2_PCP_STRUCT;
*							WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_AVB_FPRI_2_PCP_STRUCT;
*							WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_GREEN_FPRI_2_DSCP_STRUCT;
*							WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_YEL_FPRI_2_DSCP_STRUCT;
*							WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_AVB_FPRI_2_DSCP_STRUCT;
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
MSD_STATUS Willow_gprtSetIEEEPrioMap
(
    IN  MSD_QD_DEV	*dev,
    IN  MSD_LPORT	port,
    IN  WILLOW_MSD_IEEE_PRIORITY_TABLE_SET		table,
    IN  MSD_U8							entryPtr,
    IN  WILLOW_MSD_IEEE_PRIORITY_TABLE_STRUCT   *entryStruct
);
MSD_STATUS Willow_gprtSetIEEEPrioMapIntf
(
    IN  MSD_QD_DEV	*dev,
    IN  MSD_LPORT	port,
    IN  MSD_IEEE_PRIORITY_TABLE_SET		table,
    IN  MSD_U8							entryPtr,
    IN  MSD_IEEE_PRIORITY_TABLE_STRUCT   *entryStruct
);
/*******************************************************************************
* Willow_gprtGetIEEEPrioMap
*
* DESCRIPTION:
*       This API ge tport IEEE Priority Mapping.
*
* INPUTS:
*       port		- the logical port number
*       table		- Frame Priority Table set
*						WILLOW_MSD_IEEE_PRIORITY_TABLE_ING_PCP_2_F_Q_PRI
*						WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_GREEN_FPRI_2_PCP
*						WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_YEL_FPRI_2_PCP
*						WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_AVB_FPRI_2_PCP
*						WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_GREEN_FPRI_2_DSCP
*						WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_YEL_FPRI_2_DSCP
*						WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_AVB_FPRI_2_DSCP
*       entryPtr	- Pointer to the desired entry of the IEEE Priority  
*					  Mapping table selected above
*
* OUTPUTS:
*       entryStruct	- Mapping data structure for the above selected entry.
*						WILLOW_MSD_IEEE_PRIORITY_TABLE_ING_PCP_2_F_Q_PRI_STRUCT;
*						WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_GREEN_FPRI_2_PCP_STRUCT;
*						WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_YEL_FPRI_2_PCP_STRUCT;
*						WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_AVB_FPRI_2_PCP_STRUCT;
*						WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_GREEN_FPRI_2_DSCP_STRUCT;
*						WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_YEL_FPRI_2_DSCP_STRUCT;
*						WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_AVB_FPRI_2_DSCP_STRUCT;
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
MSD_STATUS Willow_gprtGetIEEEPrioMap
(
    IN  MSD_QD_DEV   *dev,
    IN  MSD_LPORT    port,
    IN  WILLOW_MSD_IEEE_PRIORITY_TABLE_SET      table,
    IN  MSD_U8							entryPtr,
    OUT WILLOW_MSD_IEEE_PRIORITY_TABLE_STRUCT    *entryStruct
);
MSD_STATUS Willow_gprtGetIEEEPrioMapIntf
(
    IN  MSD_QD_DEV   *dev,
    IN  MSD_LPORT    port,
    IN  MSD_IEEE_PRIORITY_TABLE_SET      table,
    IN  MSD_U8							entryPtr,
    OUT MSD_IEEE_PRIORITY_TABLE_STRUCT    *entryStruct
);

/*******************************************************************************
* Willow_gprtRewriteDscp
*
* DESCRIPTION:
*       Re-write ip dscp at egress based on the classification/trust settings in the switch. When this is
*       disabled, the packet is given qos treatment within the switch but the DSCP is not modified at the
*       egress. The L2 cos value gets modified as per the classification/trust settings in the switch
*       irrespective of this setting.
*
* INPUTS:
*       portNum - the logical port number
*       en      ¨C enable/disable dscp re-write at egress on the packet.
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
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtRewriteDscp
(
    IN  MSD_QD_DEV *dev,
    IN MSD_LPORT   portNum,
    IN MSD_BOOL    en
);
/*******************************************************************************
* Willow_gprtRewriteDscp
*
* DESCRIPTION:
*       Re-write ip dscp at egress based on the classification/trust settings in the switch. When this is
*       disabled, the packet is given qos treatment within the switch but the DSCP is not modified at the
*       egress. The L2 cos value gets modified as per the classification/trust settings in the switch
*       irrespective of this setting.
*
* INPUTS:
*       portNum - the logical port number
*       en      ¨C enable/disable dscp re-write at egress on the packet.
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
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtRewriteDscp
(
    IN  MSD_QD_DEV *dev,
    IN  MSD_LPORT   portNum,
    IN  MSD_BOOL    en
);
/*******************************************************************************
* Willow_gprtSetCosToQueue
*
* DESCRIPTION:
*       Update the COS to egress queue mapping.
*
* INPUTS:
*       portNum - the logical port number
*       cos     - Trusted or assigned cos for the packet
*       queue   - Physical queue to be mapped for trusted or assigned cos
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
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtSetCosToQueue
(
    IN  MSD_QD_DEV *dev,
    IN  MSD_LPORT  portNum,
    IN  MSD_U8     cos,
    IN  MSD_U8     queue
);

/*******************************************************************************
* Willow_gprtGetCosToQueue
*
* DESCRIPTION:
*       Get the physical queue to be mapped for trusted or assigned cos
* INPUTS:
*       portNum - the logical port number
*       cos     - Trusted or assigned cos for the packet
*
* OUTPUTS:
*       queue   - Physical queue to be mapped for trusted or assigned cos
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtGetCosToQueue
(
    IN  MSD_QD_DEV *dev,
    IN  MSD_LPORT  portNum,
    IN  MSD_U8     cos,
    OUT MSD_U8     *queue
);

/*******************************************************************************
* Willow_gprtSetCosToDscp
*
* DESCRIPTION:
*       Update the COS to DSCP mapping. This will be used for rewriting DSCP for
*       IPv4/IPv6 packets based on the assigned cos value.
*
* INPUTS:
*       portNum - the logical port number
*       cos     - Trusted or assigned cos for the packet
*       dscp    - frame differentiated services code point
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
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtSetCosToDscp
(
    IN  MSD_QD_DEV *dev,
    IN  MSD_LPORT   portNum,
    IN  MSD_U8      cos,
    IN  MSD_U8      dscp
);
/*******************************************************************************
* Willow_gprtGetCosToDscp
*
* DESCRIPTION:
*       Update the COS to DSCP mapping. This will be used for rewriting DSCP for
*       IPv4/IPv6 packets based on the assigned cos value.
*
* INPUTS:
*       portNum - the logical port number
*       cos     - Trusted or assigned cos for the packet
*
* OUTPUTS:
*       dscp    - frame differentiated services code point
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*		None.
*
*******************************************************************************/
MSD_STATUS Willow_gprtGetCosToDscp
(
    IN  MSD_QD_DEV *dev,
    IN  MSD_LPORT   portNum,
    IN  MSD_U8      cos,
    OUT MSD_U8      *dscp
);

/******************************************************************************
* Willow_gprtSetDefFpri
*
* Set port default priority.
*
******************************************************************************/
MSD_STATUS Willow_gprtSetDefFpri
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT    port,
    IN  MSD_U8  	  defque
);
/******************************************************************************
* Willow_gprtGetDefFpri
*
* Get port default priority.
*
******************************************************************************/
MSD_STATUS Willow_gprtGetDefFpri
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT     port,
    OUT MSD_U8  	   *defque
);

#ifdef __cplusplus
}
#endif

#endif /* __Willow_msdPortPrioMap_h */
#endif
