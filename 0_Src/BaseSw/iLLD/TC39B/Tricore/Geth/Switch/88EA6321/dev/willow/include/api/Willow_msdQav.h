#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"

/******************************************************************************
* Willow_msdQav.h
*
* DESCRIPTION:
*       API definitions for Qav
*
* DEPENDENCIES:
*
* FILE REVISION NUMBER:
******************************************************************************/

#ifndef __Willow_msdQav_h
#define __Willow_msdQav_h

#include "msdApiTypes.h"
#include "msdSysConfig.h"
#include "msdUtils.h"

#ifdef __cplusplus
extern "C" {
#endif

#undef USE_SINGLE_READ
#define MAX_PTP_CONSECUTIVE_READ    4

MSD_STATUS Willow_gqavSetPortQpriXRate
(
    IN  MSD_QD_DEV     *dev,
    IN    MSD_LPORT    port,
    IN  MSD_U8        queue,
    IN  MSD_U32        rate
);

MSD_STATUS Willow_gqavGetPortQpriXRate
(
    IN  MSD_QD_DEV     *dev,
    IN    MSD_LPORT    port,
    IN  MSD_U8        queue,
    OUT MSD_U32        *rate
);

MSD_STATUS Willow_gqavSetPortQpriXHiLimit
(
    IN  MSD_QD_DEV     *dev,
    IN    MSD_LPORT    port,
    IN  MSD_U8        queue,
    IN  MSD_U16        hiLimit
);

MSD_STATUS Willow_gqavGetPortQpriXHiLimit
(
    IN  MSD_QD_DEV     *dev,
    IN    MSD_LPORT    port,
    IN  MSD_U8        queue,
    OUT MSD_U16        *hiLimit
);


#ifdef __cplusplus
}
#endif

#endif /* __Willow_msdQav_h */
#endif
