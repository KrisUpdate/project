#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"

/********************************************************************************
* Willow_msdPortPrioMap.c
*
* DESCRIPTION:
*       API definitions for Port Priority Mapping Rules control
*
* DEPENDENCIES:
*
* FILE REVISION NUMBER:
*******************************************************************************/

#include "Willow_msdPortPrioMap.h"
#include "msdUtils.h"
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
MSD_STATUS Willow_gqosSetPrioMapRuleIntf
(
IN  MSD_QD_DEV  *dev,
IN  MSD_LPORT   port,
IN  MSD_BOOL    mode
)
{
	return Willow_gqosSetPrioMapRule(dev, port, mode);
}
/*******************************************************************************
* Willow_gqosGetPrioMapRule
*
* DESCRIPTION:
*       This routine Willow_gets priority mapping rule (TagIfBoth bit).
*       If the current frame is both IEEE 802.3ac tagged and an IPv4 or IPv6, 
*       and UserPrioMap (for IEEE 802.3ac) and IPPrioMap (for IP frame) are 
*       enabled, then priority selection is made based on this setup.
*       If PrioMapRule is set to MSD_TRUE, UserPrioMap is used.
*       If PrioMapRule is reset to MSD_FALSE, IPPrioMap is used.
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
MSD_STATUS Willow_gqosGetPrioMapRuleIntf
(
    IN  MSD_QD_DEV *dev,
    IN  MSD_LPORT  port,
    OUT MSD_BOOL   *mode
)
{
	return Willow_gqosGetPrioMapRule(dev, port, mode);
}

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
MSD_STATUS Willow_gprtSetInitialPriIntf
(
    IN  MSD_QD_DEV		*dev,
    IN  MSD_LPORT		port,
    IN  MSD_INITIAL_PRI  mode
)
{
	return Willow_gprtSetInitialPri(dev, port, (WILLOW_MSD_INITIAL_PRI)mode);
}

/*******************************************************************************
* Willow_gprtGetInitialPri
*
* DESCRIPTION:
*       This routine Willow_gets initial Priority assignment.
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
MSD_STATUS Willow_gprtGetInitialPriIntf
(
    IN  MSD_QD_DEV		*dev,
    IN  MSD_LPORT		port,
    OUT MSD_INITIAL_PRI  *mode
)
{
	return Willow_gprtGetInitialPri(dev, port, (WILLOW_MSD_INITIAL_PRI*)mode);
}

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
MSD_STATUS Willow_gprtSetDefQpriIntf
(
  IN  MSD_QD_DEV    *dev,
  IN  MSD_LPORT    port,
  IN  MSD_U8  	  defque
)
{
	return Willow_gprtSetDefQpri(dev, port, defque);
}

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
MSD_STATUS Willow_gprtGetDefQpriIntf
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_LPORT    port,
    OUT MSD_U8       *defque
)
{
	return Willow_gprtGetDefQpri(dev, port, defque);
}

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
MSD_STATUS Willow_gprtSetIEEEPrioMapIntf
(
    IN  MSD_QD_DEV	*dev,
    IN  MSD_LPORT	port,
    IN  MSD_IEEE_PRIORITY_TABLE_SET		table,
    IN  MSD_U8							entryPtr,
    IN  MSD_IEEE_PRIORITY_TABLE_STRUCT   *entryStruct
)
{
	WILLOW_MSD_IEEE_PRIORITY_TABLE_STRUCT entry;
	WILLOW_MSD_IEEE_PRIORITY_TABLE_SET willowTable;

	if (NULL == entryStruct)
	{
		MSD_DBG_ERROR(("Input param MSD_IEEE_PRIORITY_TABLE_STRUCT in Willow_gprtSetIEEEPrioMapIntf is NULL. \n"));
		return MSD_BAD_PARAM;
	}

	switch (table)
	{
	case MSD_IEEE_PRIORITY_TABLE_ING_PCP_2_F_Q_PRI:
		willowTable = WILLOW_MSD_IEEE_PRIORITY_TABLE_ING_PCP_2_F_Q_PRI;
		break;
	case MSD_IEEE_PRIORITY_TABLE_EG_GREEN_FPRI_2_PCP:
		willowTable = WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_GREEN_FPRI_2_PCP;
		break;
	case MSD_IEEE_PRIORITY_TABLE_EG_YEL_FPRI_2_PCP:
		willowTable = WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_YEL_FPRI_2_PCP;
		break;
	case MSD_IEEE_PRIORITY_TABLE_EG_AVB_FPRI_2_PCP:
		willowTable = WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_AVB_FPRI_2_PCP;
		break;
	case MSD_IEEE_PRIORITY_TABLE_ING_PCP_2_F_Q_PRI_DEI_1:
		return MSD_NOT_SUPPORTED;
	case MSD_IEEE_PRIORITY_TABLE_EG_GREEN_FPRI_2_DSCP:
		willowTable = WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_GREEN_FPRI_2_DSCP;
		break;
	case MSD_IEEE_PRIORITY_TABLE_EG_YEL_FPRI_2_DSCP:
		willowTable = WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_YEL_FPRI_2_DSCP;
		break;
	case MSD_IEEE_PRIORITY_TABLE_EG_AVB_FPRI_2_DSCP:
		willowTable = WILLOW_MSD_IEEE_PRIORITY_TABLE_EG_AVB_FPRI_2_DSCP;
		break;
	default:
		return MSD_NOT_SUPPORTED;
	}

	entry.ingTbl.disIeeeFpri = entryStruct->ingTbl.disIeeeFpri;
	entry.ingTbl.disIeeeQpri = entryStruct->ingTbl.disIeeeQpri;
	entry.ingTbl.ieeeFpri = entryStruct->ingTbl.ieeeFpri;
	entry.ingTbl.ieeeQpri = entryStruct->ingTbl.ieeeQpri;
	entry.ingTbl.ieeeYellow = entryStruct->ingTbl.ieeeYellow;

	return Willow_gprtSetIEEEPrioMap(dev, port, willowTable, entryPtr, &entry);
}


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
MSD_STATUS Willow_gprtGetIEEEPrioMapIntf
(
    IN  MSD_QD_DEV   *dev,
    IN  MSD_LPORT    port,
    IN  MSD_IEEE_PRIORITY_TABLE_SET      table,
    IN  MSD_U8							entryPtr,
    OUT MSD_IEEE_PRIORITY_TABLE_STRUCT    *entryStruct
)
{
	WILLOW_MSD_IEEE_PRIORITY_TABLE_STRUCT entry;
	MSD_STATUS    retVal;

	msdMemSet((void*)(&entry), 0, sizeof(WILLOW_MSD_IEEE_PRIORITY_TABLE_STRUCT));
	retVal = Willow_gprtGetIEEEPrioMap(dev, port, (WILLOW_MSD_IEEE_PRIORITY_TABLE_SET)table, entryPtr, &entry);
	if (MSD_OK != retVal)
		return retVal;

	if (NULL == entryStruct)
	{
		MSD_DBG_ERROR(("Input param MSD_IEEE_PRIORITY_TABLE_STRUCT in Willow_gprtSetIEEEPrioMapIntf is NULL. \n"));
		return MSD_BAD_PARAM;
	}

	msdMemSet((void*)entryStruct, 0, sizeof(MSD_IEEE_PRIORITY_TABLE_STRUCT));
	entryStruct->ingTbl.disIeeeFpri = entry.ingTbl.disIeeeFpri;
	entryStruct->ingTbl.disIeeeQpri = entry.ingTbl.disIeeeQpri;
	entryStruct->ingTbl.ieeeFpri = entry.ingTbl.ieeeFpri;
	entryStruct->ingTbl.ieeeQpri = entry.ingTbl.ieeeQpri;
	entryStruct->ingTbl.ieeeYellow = entry.ingTbl.ieeeYellow;

	return retVal;
}
#endif
