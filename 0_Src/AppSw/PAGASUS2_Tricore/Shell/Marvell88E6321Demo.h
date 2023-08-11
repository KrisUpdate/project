#include "Configuration.h"
#if FEATURE_MARVELL_6321
#ifndef MARVELL6321DEMO_H
#define MARVELL6321DEMO_H 1

#include <Ifx_Types.h>
#include "msdApiTypes.h"
#include "msdSysConfig.h"

#define MARVELL_88Q5050_DEVICE_NEMBER 17
#define MARVELL_88EA6321_DEVICE_NEMBER 16
#define MARVELL_88EA6321_2_DEVICE_NEMBER 18
typedef struct {
  uint8 buf[128];
  uint32 len;
} rmu_payload;

MSD_STATUS msdBspRmuTxRx
(
	MSD_U8 *req_packet,
	MSD_U32 req_pktlen,
	MSD_U8 **rsp_packet,
	MSD_U32 *rsp_pktlen
);

MSD_STATUS init_88EA6321_RMU(MSD_U8 devNum);

#endif
#endif
