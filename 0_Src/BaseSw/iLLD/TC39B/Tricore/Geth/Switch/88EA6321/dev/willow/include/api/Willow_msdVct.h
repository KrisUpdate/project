#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"
/*******************************************************************************
* Willow_msdPhy.h
*
* DESCRIPTION:
*       API definitions for Marvell Phy functionality.
*
* DEPENDENCIES:
*       None.
*
* FILE REVISION NUMBER:
*******************************************************************************/

#ifndef __Willow_msdPhy_h
#define __Willow_msdPhy_h

#include "msdApiTypes.h"
#include "msdSysConfig.h"
#include "msdUtils.h"
#ifdef __cplusplus
extern "C" {
#endif

/* Definition for Advance Virtual Cable Test */

/*
 * typedef: enum MSD_RX_CHANNEL
 *
 * Description: Enumeration of Receiver Channel Assignment
 *
 * Enumerations:
 *        WILLOW_MSD_CHANNEL_A   - Channel A
 *        WILLOW_MSD_CHANNEL_B   - Channel B
 *        WILLOW_MSD_CHANNEL_C   - Channel C
 *        WILLOW_MSD_CHANNEL_D   - Channel D
 */
typedef enum
{
    WILLOW_MSD_CHANNEL_A,
    WILLOW_MSD_CHANNEL_B,
    WILLOW_MSD_CHANNEL_C,
    WILLOW_MSD_CHANNEL_D
} WILLOW_MSD_RX_CHANNEL;

/*
 * typedef: enum MSD_POLARITY_STATUS
 *
 * Description: Enumeration of polarity status
 *
 * Enumerations:
 *      WILLOW_MSD_POSITIVE    - positive polarity
 *      WILLOW_MSD_NEGATIVE    - negative polarity
 */
typedef enum
{
    WILLOW_MSD_POSITIVE,
    WILLOW_MSD_NEGATIVE

} WILLOW_MSD_POLARITY_STATUS;

#define WILLOW_MSD_MDI_PAIR_NUM         4    /* (1,2),(3,6),(4,5),(7,8) */
#define WILLOW_MSD_CHANNEL_PAIR_NUM     2    /* (channel A,B),(channel C,D) */

/*
 * typedef: enum  WILLOW_MSD_ADV_VCT_TRANS_CHAN_SEL
 *
 * Description: Enumeration of Advanced VCT Transmitter channel select
 *
 * Enumerations:
 *         WILLOW_MSD_ADV_VCT_NO_CROSSPAIR - Transmitter channel select is 000
 *         WILLOW_MSD_ADV_VCT_CROSSPAIR    - Transmitter channelselect is 100/101/110/111
 */
typedef enum
{
    /* Advanced VCT Mode */
     WILLOW_MSD_ADV_VCT_TCS_NO_CROSSPAIR        = 0,
     WILLOW_MSD_ADV_VCT_TCS_CROSSPAIR_0            = 0x4,
     WILLOW_MSD_ADV_VCT_TCS_CROSSPAIR_1            = 0x5,
     WILLOW_MSD_ADV_VCT_TCS_CROSSPAIR_2            = 0x6,
     WILLOW_MSD_ADV_VCT_TCS_CROSSPAIR_3            = 0x7
} WILLOW_MSD_ADV_VCT_TRANS_CHAN_SEL;


typedef enum
{
    /* Advanced VCT Mode */
     WILLOW_MSD_ADV_VCT_SAVG_2        = 0,
     WILLOW_MSD_ADV_VCT_SAVG_4        = 1,
     WILLOW_MSD_ADV_VCT_SAVG_8        = 2,
     WILLOW_MSD_ADV_VCT_SAVG_16        = 3,
     WILLOW_MSD_ADV_VCT_SAVG_32        = 4,
     WILLOW_MSD_ADV_VCT_SAVG_64        = 5,
     WILLOW_MSD_ADV_VCT_SAVG_128    = 6,
     WILLOW_MSD_ADV_VCT_SAVG_256    = 7
}  WILLOW_MSD_ADV_VCT_SAMPLE_AVG;

typedef enum
{
    /* Advanced VCT Mode */
     WILLOW_MSD_ADV_VCT_MAX_PEAK        =0x00,
     WILLOW_MSD_ADV_VCT_FIRST_PEAK        =0x01,
     WILLOW_MSD_ADV_VCT_OFFSET            =0x02,
     WILLOW_MSD_ADV_VCT_SAMPLE_POINT    =0x03
#if 0
    /* Advanced VCT Transmiter Channel Select */
     WILLOW_MSD_ADV_VCT_NO_CROSSPAIR    =0x04,  /* 23_5 bit 13:11 = 000 */
     WILLOW_MSD_ADV_VCT_CROSSPAIR        =0x08   /* 23_5 bit 13:11 = 100, 101, 110, 111 */
#endif
}  WILLOW_MSD_ADV_VCT_MOD;

typedef unsigned int  WILLOW_MSD_ADV_VCT_PEAKDET_HYST;

/*
 * typedef: enum  WILLOW_MSD_ADV_VCT_MODE
 *
 * Description: Enumeration of Advanced VCT Mode and Transmitter channel select
 *
 * Enumerations:
 *       WILLOW_MSD_ADV_VCT_FIRST_PEAK   - first peak above a certain threshold is reported.
 *       WILLOW_MSD_ADV_VCT_MAX_PEAK     - maximum peak above a certain threshold is reported.
 *       WILLOW_MSD_ADV_VCT_OFFSE         - offset
 *       WILLOW_MSD_ADV_VCT_SAMPLE_POINT - sample point
 */
typedef struct
{
     WILLOW_MSD_ADV_VCT_MOD                    mode;
     WILLOW_MSD_ADV_VCT_TRANS_CHAN_SEL      transChanSel;
     WILLOW_MSD_ADV_VCT_SAMPLE_AVG            sampleAvg;
     WILLOW_MSD_ADV_VCT_PEAKDET_HYST        peakDetHyst;
    MSD_16                            samplePointDist; 
}  WILLOW_MSD_ADV_VCT_MODE;

/*
 * typedef: enum  WILLOW_MSD_ADV_VCT_STATUS
 *
 * Description: Enumeration of Advanced VCT status
 *
 * Enumerations:
 *       WILLOW_MSD_ADV_VCT_FAIL     - advanced virtual cable test failed.
 *                             cable lengh cannot be determined.
 *       WILLOW_MSD_ADV_VCT_NORMAL   - normal cable.
 *                             cable lengh may not be determined.
 *       WILLOW_MSD_ADV_VCT_IMP_GREATER_THAN_115 - impedance mismatch > 115 ohms
 *                             cable lengh is valid.
 *       WILLOW_MSD_ADV_VCT_IMP_LESS_THAN_85 - impedance mismatch < 85 ohms
 *                             cable lengh is valid.
 *       WILLOW_MSD_ADV_VCT_OPEN      - cable open
 *                             cable lengh is valid.
 *       WILLOW_MSD_ADV_VCT_SHORT      - cable shorted
 *                             cable lengh is valid.
 *       WILLOW_MSD_ADV_VCT_CROSS_PAIR_SHORT - cross pair short.
 *                             cable lengh for each channel is valid.
 */
typedef enum
{
     WILLOW_MSD_ADV_VCT_FAIL,
     WILLOW_MSD_ADV_VCT_NORMAL,
     WILLOW_MSD_ADV_VCT_IMP_GREATER_THAN_115,
     WILLOW_MSD_ADV_VCT_IMP_LESS_THAN_85,
     WILLOW_MSD_ADV_VCT_OPEN,
     WILLOW_MSD_ADV_VCT_SHORT,
     WILLOW_MSD_ADV_VCT_CROSS_PAIR_SHORT
}  WILLOW_MSD_ADV_VCT_STATUS;


/*
 * typedef: struct MSD_CROSS_PAIR_LIST
 *
 * Description: strucuture for cross pair short channels.
 *
 * Fields:
 *      channel - cross pair short channel list
 *                channel[i] is MSD_TRUE if the channel[i] is cross pair short
 *                with the current channel under test.
 *      dist2fault - estimated distance to the shorted location.
 *                   valid only if related channel (above) is MSD_TRUE.
 */
typedef struct _WILLOW_MSD_CROSS_SHORT_LIST
{
    MSD_BOOL    channel[WILLOW_MSD_MDI_PAIR_NUM];
    MSD_16     dist2fault[WILLOW_MSD_MDI_PAIR_NUM];
} WILLOW_MSD_CROSS_SHORT_LIST;

/*
 * typedef: struct  WILLOW_MSD_ADV_CABLE_STATUS
 *
 * Description: strucuture for advanced cable status.
 *
 * Fields:
 *      cableStatus - VCT cable status for each channel.
 *      crossShort  - cross pair short list for each channel. 
 *                    Valid only if relative cableStatus is  WILLOW_MSD_ADV_VCT_CROSS_PAIR_SHORT.
 *      dist2fault  - estimated distance to fault for each channel.
 *                    Valid if relative cableStatus is one of the followings:
 *                       WILLOW_MSD_ADV_VCT_NORMAL
 *                       WILLOW_MSD_ADV_VCT_IMP_GREATER_THAN_115
 *                       WILLOW_MSD_ADV_VCT_IMP_LESS_THAN_85,
 *                       WILLOW_MSD_ADV_VCT_OPEN, or
 *                         WILLOW_MSD_ADV_VCT_SHORT
 *      offset  - The offset seen at the receiver.
 *      sampleAmp  - returned amplitude for sample point.
 */
typedef struct
{
	WILLOW_MSD_ADV_VCT_STATUS   cableStatus[WILLOW_MSD_MDI_PAIR_NUM];
    union {
		WILLOW_MSD_CROSS_SHORT_LIST crossShort;
        MSD_16     dist2fault;
        MSD_16     offset;
        MSD_16     sampleAmp;
	}u[WILLOW_MSD_MDI_PAIR_NUM];
}  WILLOW_MSD_ADV_CABLE_STATUS;

/*
 * typedef: struct  WILLOW_MSD_ADV_EXTENDED_STATUS
 *
 * Description: Currently the 1000Base-T PCS can determine the cable polarity
 *         on pairs A,B,C,D; crossover on pairs A,B and C,D; and skew among 
 *        the pares. These status enhance the capability of the virtual cable tester
 *
 * Fields:
 *      isValid        - MSD_TRUE if this structure have valid information, 
 *                       MSD_FALSE otherwise.
 *                      It is valid only if 1000BASE-T Link is up.
 *      pairSwap    - Receive channel assignement
 *      pairPolarity- WILLOW_MSD_POSITIVE, if polarity is positive,
 *                    WILLOW_MSD_NEGATIVE, otherwise
 *      pairSkew    - pair skew in units of ns
 *      cableLen    - cable length based on DSP
 */
typedef struct
{
    MSD_BOOL						isValid;
	WILLOW_MSD_RX_CHANNEL		pairSwap[WILLOW_MSD_MDI_PAIR_NUM];
	WILLOW_MSD_POLARITY_STATUS	pairPolarity[WILLOW_MSD_MDI_PAIR_NUM];
	MSD_U32						pairSkew[WILLOW_MSD_MDI_PAIR_NUM];
	MSD_U32						cableLen[WILLOW_MSD_MDI_PAIR_NUM];
}  WILLOW_MSD_ADV_EXTENDED_STATUS;

typedef struct _Willow_SW_VCT_REGISTER
{
    MSD_U8    page;
    MSD_U8    regOffset;
} Willow_SW_VCT_REGISTER;


/*******************************************************************************
*  Willow_gvctGetAdvCableDiag
*
* DESCRIPTION:
*       This routine perform the advanced virtual cable test for the requested
*       port and returns the the status per MDI pair.
*
* INPUTS:
*       port - logical port number.
*       mode - advance VCT mode (either First Peak or Maximum Peak)
*
* OUTPUTS:
*       cableStatus - the port copper cable status.
*
* RETURNS:
*       MSD_OK   - on success
*       MSD_FAIL - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS  Willow_gvctGetAdvCableDiag
(
    IN  MSD_QD_DEV *dev,
    IN  MSD_LPORT        port,
    IN   WILLOW_MSD_ADV_VCT_MODE mode,
    OUT  WILLOW_MSD_ADV_CABLE_STATUS *cableStatus
);

MSD_STATUS  Willow_gvctGetAdvCableDiagIntf
(
	IN  MSD_QD_DEV *dev,
	IN  MSD_LPORT        port,
	IN   MSD_ADV_VCT_MODE mode,
	OUT  MSD_ADV_CABLE_STATUS *cableStatus
);
/*******************************************************************************
*  Willow_gvctGetAdvExtendedStatus
*
* DESCRIPTION:
*        This routine retrieves extended cable status, such as Pair Poloarity,
*        Pair Swap, and Pair Skew. Note that this routine will be success only
*        if 1000Base-T Link is up.
*        DSP based cable length is also provided.
*
* INPUTS:
*       dev  - pointer to GT driver structure returned from mdLoadDriver
*       port - logical port number.
*
* OUTPUTS:
*       extendedStatus - the extended cable status.
*
* RETURNS:
*       MSD_OK   - on success
*       MSD_FAIL - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*        None
*
*******************************************************************************/
MSD_STATUS  Willow_gvctGetAdvExtendedStatus
(
    IN  MSD_QD_DEV     *dev,
    IN  MSD_LPORT   port,
    OUT  WILLOW_MSD_ADV_EXTENDED_STATUS *extendedStatus
);
MSD_STATUS Willow_gvctGetAdvExtendedStatusIntf
(
	IN  MSD_QD_DEV     *dev,
	IN  MSD_LPORT   port,
	OUT MSD_ADV_EXTENDED_STATUS *extendedStatus
);
#ifdef __cplusplus
}
#endif

#endif /* __Willow_msdVct_h */
#endif
