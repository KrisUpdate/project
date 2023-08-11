#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "msdCopyright.h"
/*******************************************************************************
* msdQueueCtrl.h
*
* DESCRIPTION:
*       API definitions for Marvell Queue Control.
*
* DEPENDENCIES:
*       None.
*
* FILE REVISION NUMBER:
*******************************************************************************/

#ifndef __msdQueueCtrl_h
#define __msdQueueCtrl_h

#include "msdApiTypes.h"
#include "msdSysConfig.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
* msdPortQueueCtrlGet
*
* DESCRIPTION:
*        Gets Queue control data from the Queue Control register.
*        The register(pointer) of Queue control are:
*            MSD_QUEUE_CFG_PORT_SCHEDULE
*            MSD_QUEUE_CFG_FILTER_Q_EN
*            MSD_QUEUE_CFG_PORT_SCRATCH_REG,     (acratch0 - 1 inc: 1)
*            MSD_QUEUE_CFG_H_Q_LIMIT_REG         (Q0 - Q7 inc: 1)
*            MSD_QUEUE_CFG_Y_Q_LIMIT_REG         (Q0 - Q7 inc: 1)
*            MSD_QUEUE_CFG_PFC_THRESHOLD_BEGIN   (Q0 - Q7 inc: 2)
*            MSD_QUEUE_CFG_PFC_THRESHOLD_END     (Q0 - Q7 inc: 2)
*            MSD_QUEUE_CFG_IN_Q_COUNT            (Q0 - Q7 inc: 1)
*            MSD_QUEUE_CFG_FC_THRESHOLD_BEGIN
*			 MSD_QUEUE_CFG_FC_THRESHOLD_END
*            MSD_QUEUE_CFG_IN_PORT_COUNT
*
* INPUTS:
*       devNum  - physical devie number
*       portNum - logical port number.
*       point - Pointer to the Queue control register.
*
* OUTPUTS:
*       data  - Queue Control data written to the register
*                pointed to by the point above.
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
MSD_STATUS msdPortQueueCtrlGet
(
    IN  MSD_U8  devNum,
    IN  MSD_LPORT  portNum,
    IN  MSD_U8  point,
    OUT MSD_U8  *data
);

/*******************************************************************************
* msdPortQueueCtrlSet
*
* DESCRIPTION:
*        Sets Queue control data to the Queue Control register.
*        The register(pointer) of Queue control are:
*            MSD_QUEUE_CFG_PORT_SCHEDULE
*            MSD_QUEUE_CFG_FILTER_Q_EN
*            MSD_QUEUE_CFG_PORT_SCRATCH_REG,     (acratch0 - 1 inc: 1)
*            MSD_QUEUE_CFG_H_Q_LIMIT_REG         (Q0 - Q7 inc: 1)
*            MSD_QUEUE_CFG_Y_Q_LIMIT_REG         (Q0 - Q7 inc: 1)
*            MSD_QUEUE_CFG_PFC_THRESHOLD_BEGIN   (Q0 - Q7 inc: 2)
*            MSD_QUEUE_CFG_PFC_THRESHOLD_END     (Q0 - Q7 inc: 2)
*            MSD_QUEUE_CFG_IN_Q_COUNT            (Q0 - Q7 inc: 1)
*            MSD_QUEUE_CFG_FC_THRESHOLD_BEGIN
*			 MSD_QUEUE_CFG_FC_THRESHOLD_END
*            MSD_QUEUE_CFG_IN_PORT_COUNT
*
* INPUTS:
*       devNum  - physical devie number
*       portNum - logical port number.
*       point - Pointer to the Queue control register.
*       data  - Queue Control data written to the register
*                pointed to by the point above.
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
MSD_STATUS msdPortQueueCtrlSet
(
    IN  MSD_U8  devNum,
    IN  MSD_LPORT  portNum,
    IN  MSD_U8  point,
    IN  MSD_U8  data
);

/*******************************************************************************
* msdPortSchedGet
*
* DESCRIPTION:
*        This routine gets Port Scheduling Mode.
*        When usePortSched is enablied, this mode is used to select the Queue
*        controller's scheduling on the port
*
* INPUTS:
*       devNum  - physical devie number
*       portNum - logical port number.
*
* OUTPUTS:
*       mode - MSD_PORT_SCHED_MODE enum type
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
MSD_STATUS msdPortSchedGet
(
    IN  MSD_U8  devNum,
    IN  MSD_LPORT  portNum,
    OUT MSD_PORT_SCHED_MODE  *mode
);

/*******************************************************************************
* msdPortSchedSet
*
* DESCRIPTION:
*        This routine sets Port Scheduling Mode.
*        When usePortSched is enabled, this mode is used to select the Queue
*        controller's scheduling on the port
*
* INPUTS:
*       devNum  - physical devie number
*       portNum - logical port number.
*       mode - MSD_PORT_SCHED_MODE enum type
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
MSD_STATUS msdPortSchedSet
(
    IN  MSD_U8  devNum,
    IN  MSD_LPORT  portNum,
    IN  MSD_PORT_SCHED_MODE  mode
);

/*******************************************************************************
* msdSysQoSWeightGet
*
* DESCRIPTION:
*       This routine gets round robin weights.
*       Each port has 4/8 output Queues. Queue 3/7 has the highest priority
*       Queue 0 has the lowest priority. When a scheduling mode of port is
*       configured as Weighted Round Robin queuing mode, the access sequece of
*       Queue is 3,2,3,1,3,2,3,0,3,2,3,1,3,2,3 by default.
*       (Queue is 7,6,5,7,1,6,7,4 by default. That is after 6390.)
*       This routine retrieves the access sequence of the Queue.
*
* INPUTS:
*       None.
*
* OUTPUTS:
*       weight - access sequence of the queue
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
MSD_STATUS msdSysQoSWeightGet
(
    IN  MSD_U8  devNum,
    OUT MSD_QoS_WEIGHT  *weight
);

/*******************************************************************************
* msdSysQoSWeightSet
*
* DESCRIPTION:
*       This routine sets round robin weights.
*       Each port has 4/8 output Queues. Queue 3/7 has the highest priority
*       Queue 0 has the lowest priority. When a scheduling mode of port is
*       configured as Weighted Round Robin queuing mode, the access sequece of
*       Queue is 3,2,3,1,3,2,3,0,3,2,3,1,3,2,3 by default.
*       (Queue is 7,6,5,7,1,6,7,4 by default. That is after 6390.)
*       This sequence can be configured with this API.
*
* INPUTS:
*       weight - access sequence of the queue
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
MSD_STATUS msdSysQoSWeightSet
(
    IN  MSD_U8  devNum,
    IN  MSD_QoS_WEIGHT  *weight
);

#ifdef __cplusplus
}
#endif

#endif /* __msdQueueCtrl_h */
#endif /*#if FEATURE_MARVELL_6321*/
