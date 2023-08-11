#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"
/*******************************************************************************
* Willow_msdTCAM.h
*
* DESCRIPTION:
*       API/Structure definitions for Marvell TCAM functionality.
*
* DEPENDENCIES:
*       None.
*
* FILE REVISION NUMBER:
*******************************************************************************/

#ifndef __Willow_msdTCAM_h
#define __Willow_msdTCAM_h

#include "msdApiTypes.h"
#include "msdSysConfig.h"
#include "msdUtils.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************/
/* Exported TCAM Types			 			                                   */
/****************************************************************************/

#define WILLOW_MAX_IGR_TCAM_ENTRY 256

typedef enum
{
    WILLOW_MSD_TCAM_MODE_DISABLE,
    WILLOW_MSD_TCAM_MODE_ENABLE_48,
    WILLOW_MSD_TCAM_MODE_ENABLE_96,
} WILLOW_MSD_TCAM_MODE;

typedef struct {
	/* Key & Mask */
    MSD_U8        frameType;
    MSD_U8        frameTypeMask;
    MSD_U16       spv;
    MSD_U16       spvMask;
    MSD_U8        ppri;
    MSD_U8        ppriMask;
    MSD_U16       pvid;
    MSD_U16       pvidMask;
	
	/* Ethernet Frame Content*/
    MSD_U8        frameOctet[48];
    MSD_U8        frameOctetMask[48];

	/* Ingress Action */
    MSD_U8        continu;
    MSD_U8        interrupt;
    MSD_U8        IncTcamCtr;
    MSD_BOOL      vidOverride;
    MSD_U16       vidData;

    MSD_U8        nextId;
    MSD_BOOL      qpriOverride;
    MSD_U8        qpriData;
    MSD_BOOL      fpriOverride;
    MSD_U8        fpriData;

    MSD_U8        dpvSF;
    MSD_U32       dpvData;

    MSD_U8        dpvMode;
    MSD_U8        colorMode;
    MSD_BOOL      vtuPageOverride;
    MSD_U8        vtuPage;
    MSD_U8        unKnownFilter;
    MSD_U8        egActPoint;

    MSD_BOOL      ldBalanceOverride;
    MSD_U8        ldBalanceData;
    MSD_BOOL      DSCPOverride;
    MSD_U8        DSCP;

    MSD_BOOL      factionOverride;
    MSD_U16       factionData;
}  WILLOW_MSD_TCAM_DATA;

typedef struct
{
    MSD_BOOL      frameModeOverride;
    MSD_U8        frameMode;
    MSD_BOOL      tagModeOverride;
    MSD_U8        tagMode;
    MSD_U8        daMode;
    MSD_U8        saMode;

    MSD_BOOL      egVidModeOverride;
    MSD_U8        egVidMode;
    MSD_U16       egVidData;

    MSD_U8        egDSCPMode;
    MSD_U8        egDSCP;
    MSD_BOOL      egfpriModeOverride;
    MSD_U8        egfpriMode;
    MSD_U8        egEC;
    MSD_U8        egFPRI;
	
}  WILLOW_MSD_TCAM_EGR_DATA;

/*Typedef: struct WILLOW_MSD_TCAM_ENT_KEY
*
* Description : TCAM entry data for key/mask.
*
* Fields :
*frameType - Frame type
*      frameType - Frame type
*      spv  - Source Port Vector
*      ppri - Provider pri
*      pvid - Provider VID
*
*/
typedef struct
{
    MSD_U8       frameType;
    MSD_U16      spv;
    MSD_U8       ppri;
    MSD_U16      pvid;
} WILLOW_MSD_TCAM_ENT_KEY;

/*Typedef: struct WILLOW_MSD_TCAM_ENT_ACT
*
* Description : TCAM action data for advance setting
*
* Fields :
*      interrupt - Interrupt on a TCAM hit
*      IncTcamCtr - Increment the port¡¯s TCAM Counter
*      vidOverride - VID Override Enable
*      vidData - VID Override Data
*      nextId - Next Index
*      qpriOverride - QPRI Override Enable
*      qpriData - QPRI Override Data
*      fpriOverride - FPRI Override Enable
*      fpriData - FPRI Override Data
*      dpvSF - Source Port Filter
*      dpvData - DPV Data
*      dpvMode - DPV Mode
*      colorMode - Color Mode
*      vtuPageOverride - VTU Page Override
*      vtuPage - VTU Page
*      unKnownFilter - Unknown Filtering
*      egActPoint - Egress Action Pointer
*      ldBalanceOverride - Load Balance Override Enable
*      ldBalanceData - Load Balance Override Data
*      DSCPOverride - DSCP Override Enable
*      DSCP - DSCP Data
*      factionOverride - Frame Action Override Enable
*      factionData - Frame Action Data
*/
typedef struct
{
    MSD_U8        interrupt;
    MSD_U8        IncTcamCtr;
    MSD_BOOL      vidOverride;
    MSD_U16       vidData;

    MSD_BOOL      qpriOverride;
    MSD_U8        qpriData;
    MSD_BOOL      fpriOverride;
    MSD_U8        fpriData;

    MSD_U8        dpvSF;
    MSD_U32       dpvData;

    MSD_U8        dpvMode;
    MSD_U8        colorMode;
    MSD_BOOL      vtuPageOverride;
    MSD_U8        vtuPage;
    MSD_U8        unKnownFilter;
    MSD_U8        egActPoint;

    MSD_BOOL      ldBalanceOverride;
    MSD_U8        ldBalanceData;
    MSD_BOOL      DSCPOverride;
    MSD_U8        DSCP;

    MSD_BOOL      factionOverride;
    MSD_U16       factionData;
} WILLOW_MSD_TCAM_ENT_ACT;
/****************************************************************************/
/* Exported TCAM Functions		 			                                   */
/****************************************************************************/

/*******************************************************************************
*  Willow_gtcamFlushAll
*
* DESCRIPTION:
*       This routine is to flush all entries. A Flush All command will initialize 
*       TCAM Pages 0 and 1, offsets 0x02 to 0x1B to 0x0000, and TCAM Page 2 offset
*       0x02 to 0x1B to 0x0000 for all TCAM entries with the exception that TCAM 
*       Page 0 offset 0x02 will be initialized to 0x00FF.
*
*
* INPUTS:
*        None.
*
* OUTPUTS:
*        None.
*
* RETURNS:
*       MSD_OK   - on success
*       MSD_FAIL - on error
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS  Willow_gtcamFlushAll
(
    IN  MSD_QD_DEV     *dev
);

/*******************************************************************************
*  Willow_gtcamFlushEntry
*
* DESCRIPTION:
*       This routine is to flush a single entry. A Flush a single TCAM entry command 
*       will write the same values to a TCAM entry as a Flush All command, but it is
*       done to the selected single TCAM entry only.
*
*
* INPUTS:
*       tcamPointer - pointer to the desired entry of TCAM (0 ~ 255)
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK      - on success
*       MSD_FAIL    - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS  Willow_gtcamFlushEntry
(
    IN  MSD_QD_DEV     *dev,
    IN  MSD_U32        tcamPointer
);

/*******************************************************************************
*  Willow_gtcamLoadEntry
*
* DESCRIPTION:
*       This routine loads a TCAM entry.
*       The load sequence of TCAM entry is critical. Each TCAM entry is made up of
*       3 pages of data. All 3 pages need to loaded in a particular order for the TCAM 
*       to operate correctly while frames are flowing through the switch. 
*       If the entry is currently valid, it must first be flushed. Then page 2 needs 
*       to be loaded first, followed by page 1 and then finally page 0. 
*       Each page load requires its own write TCAMOp with these TCAM page bits set 
*       accordingly. 
*
* INPUTS:
*        tcamPointer - pointer to the desired entry of TCAM (0 ~ 255)
*        tcamData    - Tcam entry Data
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK      - on success
*       MSD_FAIL    - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS  Willow_gtcamLoadEntry
(
    IN  MSD_QD_DEV     *dev,
    IN  MSD_U32        tcamPointer,
    IN   WILLOW_MSD_TCAM_DATA  *tcamData
);
MSD_STATUS  Willow_gtcamLoadEntryIntf
(
    IN  MSD_QD_DEV     *dev,
    IN  MSD_U32        tcamPointer,
    IN  MSD_TCAM_DATA  *tcamData
);
/*******************************************************************************
*  Willow_gtcamGetNextTCAMData
*
* DESCRIPTION:
*       This routine finds the next higher TCAM Entry number that is valid (i.e.,
*       any entry whose Page 0 offset 0x02 is not equal to 0x00FF). The TCAM Entry 
*       register (bits 7:0) is used as the TCAM entry to start from. To find 
*       the lowest number TCAM Entry that is valid, start the Get Next operation 
*       with TCAM Entry set to 0xFF.
*
* INPUTS:
*        tcamPointer - pointer to the desired entry of TCAM (0 ~ 255)
*
* OUTPUTS:
*        tcamPointer - next pointer entry of TCAM (0 ~ 255)
*        tcamData    - Tcam entry Data
*
* RETURNS:
*       MSD_OK      - on success
*       MSD_FAIL    - on error
*       MSD_NO_SUCH - no more entries.
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS  Willow_gtcamGetNextTCAMData
(
    IN     MSD_QD_DEV     *dev,
    INOUT  MSD_U32        *tcamPointer,
    OUT     WILLOW_MSD_TCAM_DATA  *tcamData
);
MSD_STATUS Willow_gtcamGetNextTCAMDataIntf
(
    IN     MSD_QD_DEV     *dev,
    INOUT  MSD_U32        *tcamPointer,
    OUT    MSD_TCAM_DATA  *tcamData
);
/*******************************************************************************
*  Willow_gtcamReadTCAMData
*
* DESCRIPTION:
*       This routine reads the TCAM offsets 0x02 to 0x1B registers with 
*       the data found in the TCAM entry and its TCAM page pointed to by the TCAM
*       entry and TCAM page bits of this register (bits 7:0 and 11:10 respectively.
*
*
* INPUTS:
*       tcamPointer - pointer to the desired entry of TCAM (0 ~ 255)
*
* OUTPUTS:
*       tcamData    - Tcam entry Data
*
* RETURNS:
*       MSD_OK      - on success
*       MSD_FAIL    - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS  Willow_gtcamReadTCAMData
(
    IN  MSD_QD_DEV     *dev,
    IN  MSD_U32        tcamPointer,
    OUT  WILLOW_MSD_TCAM_DATA  *tcamData
);
MSD_STATUS  Willow_gtcamReadTCAMDataIntf
(
    IN  MSD_QD_DEV     *dev,
    IN  MSD_U32        tcamPointer,
    OUT  MSD_TCAM_DATA  *tcamData
);

/*******************************************************************************
*  Willow_gtcamFindEntry
*
* DESCRIPTION:
*       Find the specified valid tcam entry in ingress TCAM Table.
*
* INPUTS:
*       tcamPointer - the tcam entry index to search.
*
* OUTPUTS:
*       found    - MSD_TRUE, if the appropriate entry exists.
*       tcamData - the entry parameters.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error or entry does not exist.
*       MSD_NO_SUCH - no more entries.
*       MSD_BAD_PARAM    - on bad parameter
*
* COMMENTS:
*       None
*******************************************************************************/
MSD_STATUS  Willow_gtcamFindEntry
(
    IN	MSD_QD_DEV     *dev,
    IN	MSD_U32        tcamPointer,
    OUT	 WILLOW_MSD_TCAM_DATA  *tcamData,
	OUT	MSD_BOOL		  *found
);
MSD_STATUS  Willow_gtcamFindEntryIntf
(
    IN	MSD_QD_DEV     *dev,
    IN	MSD_U32        tcamPointer,
    OUT	MSD_TCAM_DATA  *tcamData,
    OUT	MSD_BOOL		  *found
);
/*******************************************************************************
* Willow_gtcamSetMode
*
* DESCRIPTION:
*       Set the TCAM Mode, The following mode is supported:
*               TCAM_MODE_DISABLE, TCAM_MODE_ENABLE_48, TCAM_MODE_ENABLE_96
*       Note: Do not chage the value while frames are flowing into this port,
*             You must first put the port in the Disable Port State, then change
*             TCAM mode, and then re-enable the port's Port State again.
*
* INPUTS:
*       portNum  - the logical port number
*       mode     - tcam mode to be set
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
*       None
*******************************************************************************/
MSD_STATUS Willow_gtcamSetMode
(
    IN MSD_QD_DEV *dev,
    IN MSD_LPORT portNum,
    IN WILLOW_MSD_TCAM_MODE mode
);
MSD_STATUS Willow_gtcamSetModeIntf
(
    IN MSD_QD_DEV *dev,
    IN MSD_LPORT portNum,
    IN MSD_TCAM_MODE mode
);
/*******************************************************************************
* Willow_gtcamGetMode
*
* DESCRIPTION:
*       Set the TCAM Mode, The following mode is supported:
*               TCAM_MODE_DISABLE, TCAM_MODE_ENABLE_48, TCAM_MODE_ENABLE_96
*
* INPUTS:
*       portNum  - the logical port number
*
* OUTPUTS:
*       mode     - get back tcam mode
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
MSD_STATUS Willow_gtcamGetMode
(
    IN  MSD_QD_DEV *dev,
    IN  MSD_LPORT  portNum,
    OUT WILLOW_MSD_TCAM_MODE *mode
);
MSD_STATUS Willow_gtcamGetModeIntf
(
    IN  MSD_QD_DEV *dev,
    IN  MSD_LPORT  portNum,
    OUT MSD_TCAM_MODE *mode
);
/*******************************************************************************
* Willow_gtcamAddEntryPort
*
* DESCRIPTION:
*       Add a port to an existing tcam entry. If the tcam entry does not exist
*       the API should return MSD_NO_SUCH
*
* INPUTS:
*       devNum   - physical device number
*       tcamPointer - pointer to the desired entry of TCAM (Willow:0 ~ 255, Topaz:0~127)
*       portNum  - the logical port number
*
* OUTPUTS:
*       None
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_NO_SUCH - the entry is not exist
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None
*******************************************************************************/
MSD_STATUS Willow_gtcamAddEntryPort
(
    IN MSD_QD_DEV *dev,
    IN MSD_U32   tcamPointer,
    IN MSD_LPORT portNum
);
/*******************************************************************************
* Willow_gtcamRemoveEntryPort
*
* DESCRIPTION:
*       Remove a port to an existing tcam entry. If the tcam entry does not exist
*       the API should return MSD_NO_SUCH
*
* INPUTS:
*       devNum   - physical device number
*       tcamPointer - pointer to the desired entry of TCAM (Willow:0 ~ 255, Topaz:0~127)
*       portNum  - the logical port number
*
* OUTPUTS:
*       None
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_NO_SUCH - the entry is not exist
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not supported
*
* COMMENTS:
*       None
*******************************************************************************/
MSD_STATUS Willow_gtcamRemoveEntryPort
(
    IN MSD_QD_DEV *dev,
    IN MSD_U32   tcamPointer,
    IN MSD_LPORT portNum
);
/*******************************************************************************
*  Willow_gtcamEgrFlushEntry
*
* DESCRIPTION:
*       This routine is to flush a single egress entry for a particular port. A
*       Flush a single Egress TCAM entry command will write the same values to a 
*       Egress TCAM entry as a Flush All command, but it is done to the selected 
*		single egress TCAM entry of the selected single port only.
*
*
* INPUTS:
*       port        - switch port
*       tcamPointer - pointer to the desired entry of TCAM (1 ~ 63)
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK      - on success
*       MSD_FAIL    - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS  Willow_gtcamEgrFlushEntry
(
    IN  MSD_QD_DEV     *dev,
    IN  MSD_LPORT      port,
    IN  MSD_U32        tcamPointer
);
MSD_STATUS  Willow_gtcamEgrFlushEntryIntf
(
    IN  MSD_QD_DEV     *dev,
    IN  MSD_LPORT      port,
    IN  MSD_U32        tcamPointer
);
/*******************************************************************************
*  Willow_gtcamEgrFlushEntryAllPorts
*
* DESCRIPTION:
*       This routine is to flush a single egress entry for all switch ports. 
*
* INPUTS:
*       tcamPointer - pointer to the desired entry of TCAM (1 ~ 63)
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK      - on success
*       MSD_FAIL    - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS  Willow_gtcamEgrFlushEntryAllPorts
(
    IN  MSD_QD_DEV     *dev,
    IN  MSD_U32        tcamPointer
);
MSD_STATUS  Willow_gtcamEgrFlushEntryAllPortsIntf
(
    IN  MSD_QD_DEV     *dev,
    IN  MSD_U32        tcamPointer
);
/*******************************************************************************
*  Willow_gtcamEgrLoadEntry
*
* DESCRIPTION:
*       This routine loads a single egress TCAM entry for a specific port.
*
* INPUTS:
*       port        - switch port
*       tcamPointer - pointer to the desired entry of TCAM (1 ~ 63)
*       tcamData    - Tcam entry Data
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK      - on success
*       MSD_FAIL    - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS  Willow_gtcamEgrLoadEntry
(
    IN  MSD_QD_DEV     *dev,
    IN  MSD_LPORT      port,
    IN  MSD_U32        tcamPointer,
    IN   WILLOW_MSD_TCAM_EGR_DATA  *tcamData
);
MSD_STATUS  Willow_gtcamEgrLoadEntryIntf
(
    IN  MSD_QD_DEV     *dev,
    IN  MSD_LPORT      port,
    IN  MSD_U32        tcamPointer,
    IN  MSD_TCAM_EGR_DATA  *tcamData
);
/*******************************************************************************
*  Willow_gtcamEgrGetNextTCAMData
*
* DESCRIPTION:
*       This routine finds the next higher Egress TCAM Entry number that is valid 
*       (i.e.,any entry that is non-zero). The TCAM Entry register (bits 5:0) is
*       used as the Egress TCAM entry to start from. To find the lowest number
*       Egress TCAM Entry that is valid, start the Get Next operation with Egress
*       TCAM Entry set to 0x3F.
*
* INPUTS:
*		 port		 - switch port
*        tcamPointer - pointer to start search TCAM 
*
* OUTPUTS:
*        tcamPointer - next pointer entry of TCAM 
*        tcamData    - Tcam entry Data
*
* RETURNS:
*       MSD_OK      - on success
*       MSD_FAIL    - on error
*       MSD_NO_SUCH - no more entries.
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS  Willow_gtcamEgrGetNextTCAMData
(
    IN     MSD_QD_DEV     *dev,
    IN	   MSD_LPORT      port,
    INOUT  MSD_U32        *tcamPointer,
    OUT     WILLOW_MSD_TCAM_EGR_DATA  *tcamData
);
MSD_STATUS  Willow_gtcamEgrGetNextTCAMDataIntf
(
    IN     MSD_QD_DEV     *dev,
    IN	   MSD_LPORT      port,
    INOUT  MSD_U32        *tcamPointer,
    OUT    MSD_TCAM_EGR_DATA  *tcamData
);
/*******************************************************************************
*  Willow_gtcamEgrReadTCAMData
*
* DESCRIPTION:
*       This routine reads a single Egress TCAM entry for a specific port.
*
*
* INPUTS:
*       port        - switch port
*       tcamPointer - pointer to the desired entry of TCAM (1 ~ 63)
*
* OUTPUTS:
*       tcamData    - Tcam entry Data
*
* RETURNS:
*       MSD_OK      - on success
*       MSD_FAIL    - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS  Willow_gtcamEgrReadTCAMData
(
    IN  MSD_QD_DEV     *dev,
    IN	MSD_LPORT      port,
    IN  MSD_U32        tcamPointer,
    OUT  WILLOW_MSD_TCAM_EGR_DATA  *tcamData
);
MSD_STATUS  Willow_gtcamEgrReadTCAMDataIntf
(
    IN  MSD_QD_DEV     *dev,
    IN	MSD_LPORT      port,
    IN  MSD_U32        tcamPointer,
    OUT MSD_TCAM_EGR_DATA  *tcamData
);
/*******************************************************************************
* Willow_gtcamDump
*
* DESCRIPTION:
*       Finds all Tcams and print it out.
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
MSD_STATUS Willow_gtcamDump
(
    IN    MSD_QD_DEV           *dev
);

/*******************************************************************************
* Willow_gtcamAdvConfig
*
* DESCRIPTION:
*       This routine sets tcam entry to match standard IPv4/IPv6, TCP/UDP packets
*       with the given header data and mask.
*
* INPUTS:
*       devNum  - physical devie number
*       pktType - Enumeration of TCAM mapping packet type
*       tcamPointer  - tcam entry number
*       tcamPointer2 - tcam entry2 number for cascade
*       keyMaskPtr   - mask pointer for TCAM entry key
*       keyPtr       - data pointer for TCAM entry key
*       maskPtr      - mask pointer for packet unit
*       patternPtr   - data pointer for packet unit
*       actionPtr    - action pionter
*
* OUTPUTS:
*       entry2Used - MSD_TRUE: two tcam entries been used, MSD_FALSE: only tcamPointer been used, tcamPointer2 not used.
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
MSD_STATUS Willow_gtcamAdvConfig
(
    IN  MSD_QD_DEV *dev,
    IN  MSD_TCAM_PKT_TYPE pktType,
    IN  MSD_U32 tcamPointer,
    IN  MSD_U32 tcamPointer2,
    IN  WILLOW_MSD_TCAM_ENT_KEY *keyMaskPtr,
    IN  WILLOW_MSD_TCAM_ENT_KEY *keyPtr,
    IN  MSD_TCAM_ENT_PKT_UNT *maskPtr,
    IN  MSD_TCAM_ENT_PKT_UNT *patternPtr,
    IN  WILLOW_MSD_TCAM_ENT_ACT *actionPtr,
    OUT MSD_BOOL *entry2Used
);
MSD_STATUS Willow_gtcamAdvConfigIntf
(
    IN  MSD_QD_DEV *dev,
    IN  MSD_TCAM_PKT_TYPE pktType,
    IN  MSD_U32 tcamPointer,
    IN  MSD_U32 tcamPointer2,
    IN  MSD_TCAM_ENT_KEY *keyMaskPtr,
    IN  MSD_TCAM_ENT_KEY *keyPtr,
    IN  MSD_TCAM_ENT_PKT_UNT *maskPtr,
    IN  MSD_TCAM_ENT_PKT_UNT *patternPtr,
    IN  MSD_TCAM_ENT_ACT *actionPtr,
    OUT MSD_BOOL *entry2Used
);
#ifdef __cplusplus
}
#endif

#endif /* __Willow_msdTCAM_h */
#endif
