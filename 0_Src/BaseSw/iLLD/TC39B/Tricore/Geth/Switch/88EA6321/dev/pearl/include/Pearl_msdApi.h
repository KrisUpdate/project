#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"
/********************************************************************************
* Pearl_msdApi.h
*
* DESCRIPTION:
*       API definitions for QuarterDeck Device
*
* DEPENDENCIES:
*
* FILE REVISION NUMBER:
*
*******************************************************************************/

#ifndef __Pearl_msdApi_h
#define __Pearl_msdApi_h
#ifdef __cplusplus
extern "C" {
#endif

/* minimum need header files*/

/* add modules whatever you needed */
#include "Pearl_msdBrgFdb.h"
#include "Pearl_msdVct.h"
#include "Pearl_msdBrgVtu.h"
#include "Pearl_msdBrgStu.h"
#include "Pearl_msdPIRL.h"
#include "Pearl_msdPortRmon.h"
#include "Pearl_msdTCAM.h"
#include "Pearl_msdRMU.h"
#include "Pearl_msdPhyCtrl.h"
#include "Pearl_msdPortPrioMap.h"
#include "Pearl_msdPortCtrl.h"
#include "Pearl_msdSysCtrl.h"
#include "Pearl_msdQav.h"
#include "Pearl_msdPTP.h"
#include "Pearl_msdQueueCtrl.h"
#include "Pearl_msdLed.h"
#include "Pearl_msdEEPROM.h"
#include "Pearl_msdHwAccess.h"
#ifdef __cplusplus
}
#endif

#endif /* __Pearl_msdApi_h */
#endif /*#if FEATURE_MARVELL_6321*/
