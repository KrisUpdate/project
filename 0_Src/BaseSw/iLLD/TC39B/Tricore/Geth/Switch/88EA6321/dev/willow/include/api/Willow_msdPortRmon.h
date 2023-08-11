#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"
/*******************************************************************************
* Willow_msdPortRmon.h
*
* DESCRIPTION:
*       API/Structure definitions for Marvell MIBS functionality.
*
* DEPENDENCIES:
*       None.
*
* FILE REVISION NUMBER:
*******************************************************************************/

#ifndef __Willow_msdPortRmon_h
#define __Willow_msdPortRmon_h

#include "msdApiTypes.h"
#include "msdSysConfig.h"
#include "msdUtils.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************/
/* Exported MIBS Types			 			                                   */
/****************************************************************************/
/*
 *  typedef: struct MSD_STATS_COUNTERS
 *
 *  Description: MIB counter
 *     
 */
#define WILLOW_MSD_TYPE_BANK 0x400
typedef enum
{
	/* Bank 0 */
    Willow_STATS_InGoodOctetsLo = 0,
    Willow_STATS_InGoodOctetsHi,
    Willow_STATS_InBadOctets,
    
    Willow_STATS_OutFCSErr,
    Willow_STATS_InUnicasts,
    Willow_STATS_Deferred,             /* offset 5 */
    Willow_STATS_InBroadcasts,
    Willow_STATS_InMulticasts,
    Willow_STATS_64Octets,
    Willow_STATS_127Octets,
    Willow_STATS_255Octets,            /* offset 10 */
    Willow_STATS_511Octets,
    Willow_STATS_1023Octets,
    Willow_STATS_MaxOctets,
    Willow_STATS_OutOctetsLo,
    Willow_STATS_OutOctetsHi,
    Willow_STATS_OutUnicasts,          /* offset 16 */
    Willow_STATS_Excessive,
    Willow_STATS_OutMulticasts,
    Willow_STATS_OutBroadcasts,
    Willow_STATS_Single,
    Willow_STATS_OutPause,
    Willow_STATS_InPause,
    Willow_STATS_Multiple,
    Willow_STATS_InUndersize,          /* offset 24 */
    Willow_STATS_InFragments,
    Willow_STATS_InOversize,
    Willow_STATS_InJabber,
    Willow_STATS_InRxErr,
    Willow_STATS_InFCSErr,
    Willow_STATS_Collisions,
    Willow_STATS_Late,                 /* offset 31 */
	/* Bank 1 */
    Willow_STATS_InDiscards      = WILLOW_MSD_TYPE_BANK+0x00,
    Willow_STATS_InFiltered      = WILLOW_MSD_TYPE_BANK+0x01,
    Willow_STATS_InAccepted      = WILLOW_MSD_TYPE_BANK+0x02,
    Willow_STATS_InBadAccepted   = WILLOW_MSD_TYPE_BANK+0x03,
    Willow_STATS_InGoodAvbClassA = WILLOW_MSD_TYPE_BANK+0x04,
    Willow_STATS_InGoodAvbClassB = WILLOW_MSD_TYPE_BANK+0x05,
    Willow_STATS_InBadAvbClassA  = WILLOW_MSD_TYPE_BANK+0x06,
    Willow_STATS_InBadAvbClassB  = WILLOW_MSD_TYPE_BANK+0x07,
    Willow_STATS_TCAMCounter0    = WILLOW_MSD_TYPE_BANK+0x08,
    Willow_STATS_TCAMCounter1    = WILLOW_MSD_TYPE_BANK+0x09,
    Willow_STATS_TCAMCounter2    = WILLOW_MSD_TYPE_BANK+0x0a,
    Willow_STATS_TCAMCounter3    = WILLOW_MSD_TYPE_BANK+0x0b,
    Willow_STATS_InDroppedAvbA   = WILLOW_MSD_TYPE_BANK+0x0c,
    Willow_STATS_InDroppedAvbB   = WILLOW_MSD_TYPE_BANK+0x0d, 
    Willow_STATS_InDaUnknown     = WILLOW_MSD_TYPE_BANK+0x0e,
    Willow_STATS_InMGMT          = WILLOW_MSD_TYPE_BANK+0x0f,
    Willow_STATS_OutQueue0       = WILLOW_MSD_TYPE_BANK+0x10,
    Willow_STATS_OutQueue1       = WILLOW_MSD_TYPE_BANK+0x11,
    Willow_STATS_OutQueue2       = WILLOW_MSD_TYPE_BANK+0x12,
    Willow_STATS_OutQueue3       = WILLOW_MSD_TYPE_BANK+0x13,
    Willow_STATS_OutQueue4       = WILLOW_MSD_TYPE_BANK+0x14,
    Willow_STATS_OutQueue5       = WILLOW_MSD_TYPE_BANK+0x15,
    Willow_STATS_OutQueue6       = WILLOW_MSD_TYPE_BANK+0x16,
    Willow_STATS_OutQueue7       = WILLOW_MSD_TYPE_BANK+0x17,
    Willow_STATS_OutCutThrough   = WILLOW_MSD_TYPE_BANK+0x18,
    Willow_STATS_OutOctetsA      = WILLOW_MSD_TYPE_BANK+0x1a,
    Willow_STATS_OutOctetsB      = WILLOW_MSD_TYPE_BANK+0x1b,
    Willow_STATS_OutYel          = WILLOW_MSD_TYPE_BANK+0x1c,
    Willow_STATS_OutDroppedYel   = WILLOW_MSD_TYPE_BANK+0x1d, 
    Willow_STATS_OutDiscards     = WILLOW_MSD_TYPE_BANK+0x1e, 
    Willow_STATS_OutMGMT         = WILLOW_MSD_TYPE_BANK+0x1f

} WILLOW_MSD_STATS_COUNTERS;

/*
 *  typedef: struct WILLOW_MSD_STATS_COUNTER_SET
 *
 *  Description: MIB Counter Set
 *     
 */
typedef struct _WILLOW_MSD_STATS_COUNTER_SET
{
	/* Bank 0 */
    MSD_U32    InGoodOctetsLo;     /* offset 0 */
    MSD_U32    InGoodOctetsHi;     /* offset 1 */
    MSD_U32    InBadOctets;        /* offset 2 */
    MSD_U32    OutFCSErr;          /* offset 3 */
    MSD_U32    InUnicasts;         /* offset 4 */
    MSD_U32    Deferred;           /* offset 5 */
    MSD_U32    InBroadcasts;       /* offset 6 */
    MSD_U32    InMulticasts;       /* offset 7 */
    /* 
        Histogram Counters : Rx Only, Tx Only, or both Rx and Tx 
        (refer to Histogram Mode) 
    */
    MSD_U32    Octets64;         /* 64 Octets, offset 8 */
    MSD_U32    Octets127;        /* 65 to 127 Octets, offset 9 */
    MSD_U32    Octets255;        /* 128 to 255 Octets, offset 10 */
    MSD_U32    Octets511;        /* 256 to 511 Octets, offset 11 */
    MSD_U32    Octets1023;       /* 512 to 1023 Octets, offset 12 */
    MSD_U32    OctetsMax;        /* 1024 to Max Octets, offset 13 */
    MSD_U32    OutOctetsLo;      /* offset 14 */
    MSD_U32    OutOctetsHi;      /* offset 15 */
    MSD_U32    OutUnicasts;      /* offset 16 */
    MSD_U32    Excessive;        /* offset 17 */
    MSD_U32    OutMulticasts;    /* offset 18 */
    MSD_U32    OutBroadcasts;    /* offset 19 */
    MSD_U32    Single;           /* offset 20 */

    MSD_U32    OutPause;         /* offset 21 */
    MSD_U32    InPause;          /* offset 22 */
    MSD_U32    Multiple;         /* offset 23 */
    MSD_U32    InUndersize;        /* offset 24 */
    MSD_U32    InFragments;        /* offset 25 */
    MSD_U32    InOversize;         /* offset 26 */
    MSD_U32    InJabber;           /* offset 27 */
    MSD_U32    InRxErr;          /* offset 28 */
    MSD_U32    InFCSErr;         /* offset 29 */
    MSD_U32    Collisions;       /* offset 30 */
    MSD_U32    Late;             /* offset 31 */
	/* Bank 1 */
    MSD_U32    InDiscards;       /* offset 0x00 */
    MSD_U32    InFiltered;       /* offset 0x01 */
    MSD_U32    InAccepted;       /* offset 0x02 */
    MSD_U32    InBadAccepted;    /* offset 0x03 */
    MSD_U32    InGoodAvbClassA;  /* offset 0x04 */
    MSD_U32    InGoodAvbClassB;  /* offset 0x05 */
    MSD_U32    InBadAvbClassA ;  /* offset 0x06 */
    MSD_U32    InBadAvbClassB ;  /* offset 0x07 */
    MSD_U32    TCAMCounter0;     /* offset 0x08 */
    MSD_U32    TCAMCounter1;     /* offset 0x09 */
    MSD_U32    TCAMCounter2;     /* offset 0x0a */
    MSD_U32    TCAMCounter3;     /* offset 0x0b */
    MSD_U32    InDroppedAvbA;    /* offset 0x0c */
    MSD_U32    InDroppedAvbB;    /* offset 0x0d */
    MSD_U32    InDaUnknown ;     /* offset 0x0e */
    MSD_U32    InMGMT;           /* offset 0x0f */
    MSD_U32    OutQueue0;        /* offset 0x10 */
    MSD_U32    OutQueue1;        /* offset 0x11 */
    MSD_U32    OutQueue2;        /* offset 0x12 */
    MSD_U32    OutQueue3;        /* offset 0x13 */
    MSD_U32    OutQueue4;        /* offset 0x14 */
    MSD_U32    OutQueue5;        /* offset 0x15 */
    MSD_U32    OutQueue6;        /* offset 0x16 */
    MSD_U32    OutQueue7;        /* offset 0x17 */
    MSD_U32    OutCutThrough;    /* offset 0x18 */
    MSD_U32    reserved_19 ;     /* offset 0x19 */
    MSD_U32    OutOctetsA;       /* offset 0x1a */
    MSD_U32    OutOctetsB;       /* offset 0x1b */
    MSD_U32    OutYel;           /* offset 0x1c */
    MSD_U32    OutDroppedYel;    /* offset 0x1d */
    MSD_U32    OutDiscards;      /* offset 0x1e */
    MSD_U32    OutMGMT;          /* offset 0x1f */

}WILLOW_MSD_STATS_COUNTER_SET;

/*
 * typedef: enum MSD_HISTOGRAM_MODE
 *
 * Description: Enumeration of the histogram counters mode.
 *
 * Enumerations:
 *   MSD_COUNT_RX_ONLY - In this mode, Rx Histogram Counters are counted.
 *   MSD_COUNT_TX_ONLY - In this mode, Tx Histogram Counters are counted.
 *   MSD_COUNT_RX_TX   - In this mode, Rx and Tx Histogram Counters are counted.
 */
typedef enum
{
	WILLOW_MSD_COUNT_RX_ONLY = 1,
	WILLOW_MSD_COUNT_TX_ONLY,
	WILLOW_MSD_COUNT_RX_TX
} WILLOW_MSD_HISTOGRAM_MODE;

/****************************************************************************/
/* Exported MIBS Functions		 			                                   */
/****************************************************************************/
/*******************************************************************************
* Willow_gstatsFlushAll
*
* DESCRIPTION:
*       Flush All RMON counters for all ports.
*
* INPUTS:
*       None.
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK   - on success
*       MSD_FAIL - on error
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS Willow_gstatsFlushAll
(
    IN MSD_QD_DEV* dev
);

/*******************************************************************************
* Willow_gstatsFlushPort
*
* DESCRIPTION:
*       Flush All RMON counters for a given port.
*
* INPUTS:
*       port - the logical port number.
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK - on success,
*       MSD_FAIL - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS Willow_gstatsFlushPort
(
    IN MSD_QD_DEV* dev,
    IN MSD_LPORT    port
);

/*******************************************************************************
* Willow_gstatsGetPortCounter
*
* DESCRIPTION:
*        This routine gets a specific counter of the given port
*
* INPUTS:
*        port - the logical port number.
*        counter - the counter which will be read
*
* OUTPUTS:
*        statsData - points to 32bit data storage for the MIB counter
*
* RETURNS:
*       MSD_OK - on success,
*       MSD_FAIL - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*        None
*
*******************************************************************************/
MSD_STATUS Willow_gstatsGetPortCounter
(
    IN  MSD_QD_DEV        *dev,
    IN  MSD_LPORT         port,
	IN  WILLOW_MSD_STATS_COUNTERS    counter,
    OUT MSD_U32            *statsData
);
MSD_STATUS Willow_gstatsGetPortCounterIntf
(
IN  MSD_QD_DEV        *dev,
IN  MSD_LPORT        port,
IN  MSD_STATS_COUNTERS    counter,
OUT MSD_U32            *statsData
);
/*******************************************************************************
* Willow_gstatsGetPortAllCounters
*
* DESCRIPTION:
*       This routine gets all RMON counters of the given port
*
* INPUTS:
*       port - the logical port number.
*
* OUTPUTS:
*       statsCounterSet - points to WILLOW_MSD_STATS_COUNTER_SET for the MIB counters
*
* RETURNS:
*       MSD_OK - on success,
*       MSD_FAIL - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS Willow_gstatsGetPortAllCounters
(
    IN  MSD_QD_DEV* dev,
    IN  MSD_LPORT        port,
	OUT WILLOW_MSD_STATS_COUNTER_SET    *statsCounterSet
);
MSD_STATUS Willow_gstatsGetPortAllCountersIntf
(
IN  MSD_QD_DEV               *dev,
IN  MSD_LPORT        port,
OUT MSD_STATS_COUNTER_SET    *statsCounterSet
);
/*******************************************************************************
* Willow_gstatsGetHistogramMode
*
* DESCRIPTION:
*        This routine gets the Histogram Counters Mode.
*
* INPUTS:
*        None.
*
* OUTPUTS:
*        mode - Histogram Mode (MSD_COUNT_RX_ONLY, MSD_COUNT_TX_ONLY, 
*                    and MSD_COUNT_RX_TX)
*
* RETURNS:
*       MSD_OK - on success,
*       MSD_FAIL - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS Willow_gstatsGetHistogramMode
(
    IN  MSD_QD_DEV   *dev,
	OUT WILLOW_MSD_HISTOGRAM_MODE    *mode
);
MSD_STATUS Willow_gstatsGetHistogramModeIntf
(
IN  MSD_QD_DEV   *dev,
OUT MSD_HISTOGRAM_MODE    *mode
);

/*******************************************************************************
* Willow_gstatsSetHistogramMode
*
* DESCRIPTION:
*        This routine sets the Histogram Counters Mode.
*
* INPUTS:
*        mode - Histogram Mode (MSD_COUNT_RX_ONLY, MSD_COUNT_TX_ONLY, 
*                    and MSD_COUNT_RX_TX)
*
* OUTPUTS:
*        None.
*
* RETURNS:
*       MSD_OK - on success,
*       MSD_FAIL - on error.
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*        None.
*
*******************************************************************************/
MSD_STATUS Willow_gstatsSetHistogramMode
(
    IN MSD_QD_DEV   *dev,
	IN WILLOW_MSD_HISTOGRAM_MODE        mode
);
MSD_STATUS Willow_gstatsSetHistogramModeIntf
(
IN MSD_QD_DEV   *dev,
IN MSD_HISTOGRAM_MODE        mode
);

/*******************************************************************************
* Willow_gmibDump
*
* DESCRIPTION:
*       Dumps MIB from the specified port.
*
* INPUTS:
*       devNum  - physical devie number
*       portNum - logical port number.
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
MSD_STATUS Willow_gmibDump
(
IN   MSD_QD_DEV      *dev,
IN  MSD_LPORT  portNum
);

#ifdef __cplusplus
}
#endif

#endif /* __Willow_msdPortRmon_h */
#endif
