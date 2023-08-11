#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"

/********************************************************************************
* msdApi.h
*
* DESCRIPTION:
*       API definitions for QuarterDeck Device
*
* DEPENDENCIES:
*
* FILE REVISION NUMBER:
*
*******************************************************************************/

#ifndef __msdApi_h
#define __msdApi_h
#ifdef __cplusplus
extern "C" {
#endif

/* minimum need header files*/
#include "msdApiTypes.h"
#include "msdHwAccess.h"
#include "msdSem.h"
#include "msdSysConfig.h"
#include "msdUtils.h"

/* add modules whatever you needed */
#include "msdArp.h"
#include "msdBrgFdb.h"
#include "msdEcid.h"
#include "msdAdvVct.h"
#include "msdBrgStu.h"
#include "msdBrgVtu.h"
#include "msdIMP.h"
#include "msdEEPROM.h"
#include "msdPhyCtrl.h"
#include "msdPIRL.h"
#include "msdPortCtrl.h"
#include "msdQosMap.h"
#include "msdPortRmon.h"
#include "msdPolicy.h"
#include "msdQueueCtrl.h"
#include "msdSysCtrl.h"
#include "msdTCAM.h"
#include "msdTCAMRC.h"
#include "msdTCAMRCComp.h"
#include "msdRMU.h"
#include "msdQav.h"
#include "msdPTP.h"
#include "msdTrunk.h"
#include "msdLed.h"
#ifdef __cplusplus
}
#endif

#endif /* __msdApi_h */
#endif /*#if FEATURE_MARVELL_6321*/
