/**
 * \file HsslHsct.h
 *
 *  Copyright (c) 2018 Infineon Technologies AG. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 *
 * Infineon Technologies AG (Infineon) is supplying this file for use
 * exclusively with Infineon's microcontroller products. This file can be freely
 * distributed within development tools that are supporting such microcontroller
 * products.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 */


/*******************************************************************************
**                             Description                                    **
*******************************************************************************/
/* This hscthssl module implements an API to initialize and use the HSCT/HSSL
 * communication module on AURIX 3G.
 * The implementation is not optimized in regard to performance
 * in favor of providing first users an initial starting point
 * for educational purpose.
 *
 * Both HSCT/HSSL instances can be accessed.
 *
 * The hscthssl module is implemented on top of two other lower layer modules:
 * 1. hsct.c / hsct.h  HSCT implementation
 * 2. hssl.c / hssl.h  HSSL implementation
 *
 * The API of this example implementations demonstrates four different
 * features to you:
 *
 *            *** 1. Initialization ***
 * For initialization you just need to define if the device shall operate
 * in master or slave role.
 *
 * Please see the API-description of the following function for details:
 * HsctHssl_init
 *
 *            *** 2. Remote function call ***
 * This feature allows you to trigger function calls on the remote device.
 * For this purpose you can register on the receiver side up to 256 different
 * callback pointers to 256 different function call IDs.
 * On the sending side you trigger these function calls using the corresponding
 * function call ID.
 *
 * For implementation this functionality the channel0 and channel1 of HSSL are
 * used internally and not available for direct use by the API.
 *
 * Please see the API-description of the following functions for details:
 * HsctHssl_register_cb
 * HsctHssl_fctcall
 *
 *            *** 3. Streaming ***
 * This feature allows you to trigger streaming from the local initiator
 * to the remote target.
 *
 * For implementation this functionality the channel2 of HSSL is
 * used internally and not available for direct use by the API.
 *
 * Please see the API-description of the following functions for details:
 * HsctHssl_startstreaming
 * HsctHssl_waitstreamingdone
 *
 *            *** 4. request/response pairs ***
 * This feature allows you to setup request/response pairs for the different
 * HSSL commands (WRITE/READ/READID/TRG) on channel3 only.
 * The feature is limited to channel3 only, because all other channels
 * are already occupied for other purpose.
 *
 * Please see the API-description of the following functions for details:
 * HsctHssl_frameReq
 * HsctHssl_getReadData
 * HsctHssl_waitchannelIdle
 *
 */
#ifndef IFXHSSLHSCT_H_
#define IFXHSSLHSCT_H_

/*******************************************************************************
**                             Includes                                       **
*******************************************************************************/
#include "IfxHssl.h"
#include "Hsct.h"
#include "Hssl.h"


/*******************************************************************************
**                             Defines                                        **
*******************************************************************************/

/*******************************************************************************
*                              Global Variable Declarations                   **
*******************************************************************************/
extern App_Hssl_instance hssl_instance_0, hssl_instance_1;
extern App_Hsct_instance hsct_instance_0, hsct_instance_1;

/*******************************************************************************
**                             Enumerations                                   **
*******************************************************************************/

/*******************************************************************************
**                             Data Structures                                **
*******************************************************************************/
typedef enum {
	hscthssl_ok,		   /* generic success return code */
	hscthssl_failed,	   /* generic fail return code */
	hscthssl_timeout,
	hscthssl_ch_busy,      /* generic channel busy return code */
	hscthssl_setup_err     /* Target setup error */

}HsctHssl_return;

typedef const struct {
    Ifx_HSCT 	      *module; 	/**< \brief Base address */
    IfxPort_Pin        pin;		/**< \brief Port pin */
    IfxPort_OutputMode mode;    /**< \brief Port output mode */
    IfxPort_OutputIdx  idx;     /**< \brief Port output index */
    IfxPort_PadDriver  driver;	/**< \brief Port pad driver */
} IfxHsct_Out;

typedef const struct {
    Ifx_HSCT    *module;		/**< \brief Base address */
    IfxPort_Pin  pin;			/**< \brief Port pin */
    IfxPort_InputMode mode;     /**< \brief Port input mode */
    IfxPort_PadDriver driver;   /**< \brief Port pad driver */
} IfxHsct_In;

/*******************************************************************************
**                             API Function prototypes                        **
*******************************************************************************/

/**
 * @HsctHssl_init
 * Initializes and enables both HSSL/HSCT module instances including ports
 * (internal 100 Ohm LVDS termination) in master or slave interface mode.
 *
 * @input  :  - interfaceMode  : Defines master/slave mode of the device
 *                                       IfxHssl_InterfaceMode_master
 *                                       IfxHssl_InterfaceMode_slave
 *
 * @return :  none
 * */
void HsctHssl_init(IfxHssl_InterfaceMode interfaceMode);

/**
 * @HsctHssl_register_cb
 * Register RFC callback which gets called for a certain function call ID.
 *
 * @input  :  - hssl_instance  : Pointer to HSSL instance data structure
 *            - fctcall_ID     : Function call ID for which the callback is registered
 *                                       IfxHssl_InterfaceMode_master
 *                                       IfxHssl_InterfaceMode_slave
 *            - cb_fctptr      : Callback pointer which is registered for the function call ID
 *
 * @return :  none
 * */
void HsctHssl_register_cb(App_Hssl_instance *hssl_instance, uint8 fctcall_ID, void (*cb_fctptr)(void));

/**
 * @HsctHssl_fctcall
 * Trigger RFC on the remote side.
 * On the remote side the callback function which was registered for the function call ID
 * will be called.
 *
 * @input  :  - hssl_instance  : Pointer to HSSL instance data structure
 *            - fctcall_ID     : Function call ID for which the callback is registered
 *            - trials         : number of trials in case of failure
 *
 * @return :  hscthssl_ok      - functional call is triggered on remote device
 *            hscthssl_timeout - trigger sent, but no confirmation received from remote device
 *            hscthssl_failed  - functional call was not triggered on remote device
 * */
HsctHssl_return HsctHssl_fctcall(App_Hssl_instance *hssl_instance, uint8 fctcall_ID, uint32 trials);

/**
 * @Hssl_frameReq
 * Start request/response pair as initiator on channel 3.
 *
 * @input  :  - hssl_instance  : Pointer to HSSL instance data structure
 *            - frameRequest   : Type of request
 *                                       IfxHssl_Hssl_FrameRequest_readFrame
 *                                       IfxHssl_Hssl_FrameRequest_writeFrame
 *                                       IfxHssl_Hssl_FrameRequest_triggerFrame
 *                                       IfxHssl_Hssl_FrameRequest_readId
 *            - DatLen         : Data length
 *                                       IfxHssl_DataLength_8bit
 *                                       IfxHssl_DataLength_16bit
 *                                       IfxHssl_DataLength_32bit
 *            - Adr            : Address
 *            - Dat            : Data
 *            - blocking       : TRUE  call will be blocking until request/response pair has finished
 *                               FALSE call will return immediately while request is still ongoing
 *                                     see also HsctHssl_waitchannelIdle
 *            - trials         : only used in blocking mode
 *                               number of trials in case of error on request/response pair
 *                               before returning
 *
 * @return :  hscthssl_ok           - request started successfully   (if blocking set FALSE)
 *                                    see also Hssl_waitchannelIdle
 *                                  - request finalized successfully (if blocking set TRUE)
 *            hscthssl_failed       - request finalized with error
 *            hscthssl_ch_busy      - channel 3 already busy
 *                                    request/response pair ongoing
 *
 * */
HsctHssl_return HsctHssl_frameReq(App_Hssl_instance *hssl_instance, IfxHssl_Hssl_FrameRequest frameRequest, IfxHssl_DataLength DatLen, uint32 Adr, uint32 Dat, boolean blocking, uint32 trials);


/**
 * @HsctHssl_waitchannelIdle
 * Wait until channel 3 becomes idle and ready to be used for another request
 * Return code indicates an error on the last request executed before becoming idle
 *
 * @input  :  - hssl_instance  : Pointer to HSSL instance data structure
 *
 * @return :  hcthssl_ok      - channel is now idle; last request completed successfully
 *            hscthssl_failed - channel is now idle; last request had an error
 * */
HsctHssl_return HsctHssl_waitchannelIdle(App_Hssl_instance *hssl_instance);

/**
 * @HsctHsslHssl_getReadData
 * Fetch received 32bit data of last read request of channel 3 from register.
 *
 * @input  :  - hssl_instance  : Pointer to HSSL instance data structure
 *
 * @return :  uint32 value received from last read request on ch3
 * */
uint32 HsctHssl_getReadData(App_Hssl_instance *hssl_instance);

/**
 * @HsctHssl_startstreaming
 * Start streaming of data from this device to the remote device.
 * Remote device (target) is setup using channel 3.
 * Streaming with HsctHssl_startstreaming must only be started when
 * there is no other streaming already ongoing/started.
 * See also HsctHssl_waitstreamingdone.
 *
 * Optionally,
 *        1. the function can be blocking until streaming is finished.
 *        or
 *        2. the function returns immediately even while streaming still is ongoing
 *           (e.g. to start streaming on the other HSSL-instance in parallel)
 *           In this case you have to make sure to call HsctHssl_waitstreamingdone
 *           before another streaming is started again on this HSSL instance here.
 *
 * @input  :  - hssl_instance   : Pointer to HSSL instance data structure
 *            - source_adr      : Source start address on this (=initiator) device
 *            - destination_adr : Destination start address on the target device
 *            - frame_cnt       : Defines the amount of data for streaming (multiple of 32 bytes)
 *            - blocking        : TRUE  call will be blocking until streaming has finished
 *                                FALSE call will return immediately while streaming is still ongoing
 *                                      see also HsctHssl_waitstreamingdone
 *            - trials          : only used in blocking mode
 *                                number of trials in case of errors before returning
 *
 * @return :  hscthssl_ok       : Streaming started successfully   (if blocking was set FALSE)
 *                                Streaming finalized successfully (if blocking was set TRUE)
 *            hscthssl_setup_err: Target setup error (on ch3)
 *            hscthssl_failed   : Streaming error (only if blocking was set to TRUE)
 *
 * */
HsctHssl_return HsctHssl_startstreaming(App_Hssl_instance *hssl_instance, uint32 source_adr, uint32 destination_adr, uint32 frame_cnt, boolean blocking, uint32 trials);

/**
 * @HsctHssl_waitstreamingdone
 * Waits until the streaming of this HSSL instance has finished.
 *
 * @input  :  - hssl_instance   : Pointer to HSSL instance data structure
 *
 * @return :  hscthssl_ok      : Streaming finalized successfully
 *            hscthssl_failed  : Streaming finalized with error
 *
 * */
HsctHssl_return HsctHssl_waitstreamingdone(App_Hssl_instance *hssl_instance);


void HsctLoopback_test(uint32 testNumber);

#endif

