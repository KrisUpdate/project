#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"
/********************************************************************************
* msdDevObj.h
*
* DESCRIPTION:
*       API/Structure definitions for Marvell Device Object initialize.
*
* DEPENDENCIES:
*       None.
*
* FILE REVISION NUMBER:
*******************************************************************************/

#ifndef __msdDevObj_h
#define __msdDevObj_h

#include "msdApiTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

MSD_STATUS InitDevObj(MSD_QD_DEV *dev);

#ifdef __cplusplus
}
#endif
#endif /* __msdDevObj_h */
#endif /*#if FEATURE_MARVELL_6321*/
