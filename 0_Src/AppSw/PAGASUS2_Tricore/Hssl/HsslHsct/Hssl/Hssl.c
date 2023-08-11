/**
 * \file Hssl.c
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
**                                 Includes                                   **
*******************************************************************************/
#include "Hssl.h"
#include "IfxHssl_Hssl.h"
#include "Port/Std/IfxPort.h"
#include "Hsct.h"
#include "Configuration.h"

/*******************************************************************************
**                             Defines                                        **
*******************************************************************************/
IFX_INTERRUPT(ISR_Hssl0_Ch1_trg, ISR_PROVIDER_HSCT, ISR_PRIORITY_CH1_TRG);
IFX_INTERRUPT(ISR_Hssl0_Ch0_trg, ISR_PROVIDER_HSCT, ISR_PRIORITY_CH0_TRG);

/*******************************************************************************
* 							   Global Variable Declarations                   **
*******************************************************************************/

/*******************************************************************************
* 							   Local Variable Declarations                    **
*******************************************************************************/
static App_Hssl_instance *hssl_instance[HSSL_INSTANCE_COUNT];

/*******************************************************************************
* 							   Internal Function implementation               **
*******************************************************************************/

/**
 * @ISR_Hssl_Ch1_trg
 * Generic Interrupt Service Routine of HSSL module for CH1 TRG
 * Called when RFC is triggered from remote side.
 * Uses the 32bit parameter of the call from the remote side to call callback.
 * Sents confirmation (CH0 TRG) to remote device to indicate execution of the function
 * to caller.
 *
 *
 * @input  :  - instance  : Pointer to HSCT instance data structure
 *
 * @return :  none
 *
 * */
void ISR_Hssl_Ch1_trg(App_Hssl_instance *hssl_instance)
{
	uint32 para, trials = 3;
	Hssl_return status = hssl_failed;

	/* Fetch local parameter (which was set by remote side) */
	para = *hssl_instance->ptr_RFC_uint32_para_local;

	/* ... confirm to remote side parameter was fetched and RFC was called ...
	 * Latest after three times failure (with BER<10^-12), there is something seriously wrong with the connection.
	 * So we give up. Remote side will notice this via timeout and can initiate recovery.
	 */
	while (( status != hssl_ok) || (trials == 0))
	{
		Hssl_frameReq(hssl_instance, IfxHssl_Command_triggerFrame, IfxHssl_ChannelId_0, IfxHssl_DataLength_32bit, 0, 0);
		status = Hssl_waitchannelIdle(hssl_instance, IfxHssl_ChannelId_0);
		trials--;
	}
	/* ... check if callback pointer was registered and confirmation succeeded ... */
	if ( (hssl_instance->cb_fctcall_uint32!=0) && (trials>0) )
	{
		/* ... call callback registered on this device for RFC with parameter*/
		hssl_instance->cb_fctcall_uint32( para );
	}

	return;
}

/**
 * @ISR_Hssl0_Ch1_trg
 * Explicit Interrupt Service Routine of HSSL module 0 for CH1 TRG
 * Used to forward call to Generic Interrupt Service ISR_Hssl_Ch1_trg Routine of HSSL module
 *
 * @input  : none
 *
 * @return :  none
 *
 * */
void ISR_Hssl0_Ch1_trg(){
	/* Call generic ISR handler */
	ISR_Hssl_Ch1_trg(hssl_instance[0]);
}

/**
 * @ISR_Hssl_Ch0_trg
 * Generic Interrupt Service Routine of this HSSL instance for CH0 TRG.
 * Gets called when RFC has arrived on remote side.
 * Sets the state of send RFC to idle (ready to send next call).
 *
 * @input  :  - instance  : Pointer to HSCT instance data structure
 *
 * @return :  none
 *
 * */
void ISR_Hssl_Ch0_trg(App_Hssl_instance *hssl_instance)
{
	/* Server confirmed RFC is triggered. Set state back to idle */
	hssl_instance->fctcall_state_send = Hssl_fctcall_idle;
	return;
}

/**
 * @ISR_Hssl0_Ch0_trg
 * Explicit Interrupt Service Routine of HSSL module 0 for CH0 TRG
 * Gets called when RFC has arrived on remote side.
 * Sets the state of send RFC to idle (ready to send next call).
 *
 * @input  : none
 *
 * @return :  none
 *
 * */
void ISR_Hssl0_Ch0_trg(){
	/* Call generic ISR handler */
	ISR_Hssl_Ch0_trg(hssl_instance[0]);
}


/**
 * @Hssl_getChannelErrorFlags
 * Get and reset all error flags for HSSL channel.
 *
 * @input  :  - hssl_instance  : Pointer to HSSL instance data structure
 *            - channelId      : HSSL channel
 *                                   IfxHssl_ChannelId_0
 *                                   IfxHssl_ChannelId_1
 *                                   IfxHssl_ChannelId_2
 *                                   IfxHssl_ChannelId_3
 *
 * @return :  uint32 errorFlags: Channel error flags
 * */
  #if 1
static uint32 Hssl_getChannelErrorFlags(App_Hssl_instance *hssl_instance, IfxHssl_ChannelId channelId)
{
	uint32 errorFlags;

	errorFlags = IfxHssl_getHsslChannelErrorInterruptFlagStatus(hssl_instance->hssl.hssl,IfxHssl_Hssl_ERRInterruptSource_notAcknowledgeError,channelId)|
			     IfxHssl_getHsslChannelErrorInterruptFlagStatus(hssl_instance->hssl.hssl,IfxHssl_Hssl_ERRInterruptSource_transactionTagError,channelId)|
			     IfxHssl_getHsslChannelErrorInterruptFlagStatus(hssl_instance->hssl.hssl,IfxHssl_Hssl_ERRInterruptSource_timeoutError,channelId)|
			     IfxHssl_getHsslChannelErrorInterruptFlagStatus(hssl_instance->hssl.hssl,IfxHssl_Hssl_ERRInterruptSource_unexpectedError,channelId);

	IfxHssl_clearHsslChannelErrorInterruptFlag(hssl_instance->hssl.hssl, IfxHssl_Hssl_ERRInterruptSource_notAcknowledgeError, channelId);
	IfxHssl_clearHsslChannelErrorInterruptFlag(hssl_instance->hssl.hssl, IfxHssl_Hssl_ERRInterruptSource_transactionTagError, channelId);
	IfxHssl_clearHsslChannelErrorInterruptFlag(hssl_instance->hssl.hssl, IfxHssl_Hssl_ERRInterruptSource_timeoutError, channelId);
	IfxHssl_clearHsslChannelErrorInterruptFlag(hssl_instance->hssl.hssl, IfxHssl_Hssl_ERRInterruptSource_unexpectedError, channelId);

	return errorFlags;
}

/**
 * @Hssl_frameReq_wait
 * Start a new request/response pair as initiator and waits until request has finished.
 *
 * @input  :  - hssl_instance  : Pointer to HSSL instance data structure
 *            - frameRequest   : Type of request
 *                                       IfxHssl_Hssl_FrameRequest_readFrame
 *                                       IfxHssl_Hssl_FrameRequest_writeFrame
 *                                       IfxHssl_Hssl_FrameRequest_triggerFrame
 *                                       IfxHssl_Hssl_FrameRequest_readId
 *                                       IfxHssl_Hssl_FrameRequest_noAction
 *            - ChId           : Channel ID
 *                                       IfxHssl_ChannelId_0
 *                                       IfxHssl_ChannelId_1
 *                                       IfxHssl_ChannelId_2
 *                                       IfxHssl_ChannelId_3
 *            - DatLen         : Data length
 *                                       IfxHssl_DataLength_8bit
 *                                       IfxHssl_DataLength_16bit
 *                                       IfxHssl_DataLength_32bit
 *            - Adr            : Address
 *            - Dat            : Data
 *
 * @return :  uint32 errorFlags: hssl_ok     - frame request sent
 *                               hssl_failed - frame request not sent (channel was busy)
 * */

Hssl_return Hssl_frameReq_wait (App_Hssl_instance *hssl_instance, IfxHssl_Hssl_FrameRequest frameRequest,
		                        IfxHssl_ChannelId ChId, IfxHssl_DataLength DatLen, uint32 Adr, uint32 Dat){

	if (IfxHssl_Hssl_singleFrameRequest(&(hssl_instance->hsslChannel[ChId]), frameRequest, Adr, Dat, DatLen)!=IfxHssl_Hssl_Status_ok)
		return hssl_failed;

	if (Hssl_waitchannelIdle(hssl_instance, ChId) != hssl_ok)
		return hssl_failed;

	return hssl_ok;
}
#endif

/*******************************************************************************
**                             API Function implementation                    **
*******************************************************************************/
/**
 * @Hssl_initModule
 * Initializes and enables the HSSL module
 * For proper initialisation specify
 * 	- reference to HSSL-register base address
 * 	- address of local 32bit function parameter to pass parameter to remote side (RFC)
 * 	- address of remote 32bit function parameter to fetch parameter from remote side (RFC)
 * 	- ISR base prio
 * inside App_Hssl_instance input parameter
 *
 * @input  :  - instance  : Pointer to HSCT instance data structure
 *
 * @return :  - status 	hssl_ok      success
 *                      hssl_failed  failed
 * */
Hssl_return Hssl_initModule(App_Hssl_instance *instance){
	IfxHssl_Hssl_Config hsslconfig;

	/* Store instance pointer for use in generic ISR handler */
	if (instance->hssl.hssl==&MODULE_HSSL0)
		hssl_instance[0]=instance;
	else
	{
		if (instance->hssl.hssl==&MODULE_HSSL1)
			hssl_instance[1]=instance;
		else
			return hssl_failed;
	}

	/* Initialise the configuration structure of iLLD */
	IfxHssl_Hssl_initHsslModuleConfig(&hsslconfig, instance->hssl.hssl);

	/* Initialize module internal states for RFC */
	instance->fctcall_state_send = Hssl_fctcall_idle;
	instance->ptr_RFC_uint32_para_local = 0;
	instance->ptr_RFC_uint32_para_remote = 0;
	instance->cb_fctcall_uint32 = 0;

	/* Initialise HSSL module */
	IfxHssl_Hssl_initHsslModule(&instance->hssl, &hsslconfig);

	uint32 i;
	/* Initialise channel data structures used by iLLD */
    for (i = 0; i < 4; ++i)
    {
    	instance->hsslChannel[i].hssl = instance->hssl.hssl;
    	instance->hsslChannel[i].channelId = (IfxHssl_ChannelId)i;
    	instance->hsslChannel[i].currentFrameRequest = IfxHssl_Hssl_FrameRequest_noAction;
    	instance->hsslChannel[i].loopBack = FALSE;
    	instance->hsslChannel[i].streamingMode = IfxHssl_StreamingMode_single;
    	instance->hsslChannel[i].streamingModeOn = FALSE;
    }

    /* Enable HSSL interrupt in use */
	IfxHssl_enableHsslTRGInterrupt (instance->hssl.hssl, IfxHssl_ChannelId_1, IfxSrc_Tos_cpu0, ISR_PRIORITY_CH1_TRG);
	IfxHssl_enableHsslTRGInterrupt (instance->hssl.hssl, IfxHssl_ChannelId_0, IfxSrc_Tos_cpu0, ISR_PRIORITY_CH0_TRG);

	return hssl_ok;
}

/**
 * @Hssl_frameReq
 * Start a new request/response pair as initiator.
 *
 * @input  :  - hssl_instance  : Pointer to HSSL instance data structure
 *            - frameRequest   : Type of request
 *                                       IfxHssl_Hssl_FrameRequest_readFrame
 *                                       IfxHssl_Hssl_FrameRequest_writeFrame
 *                                       IfxHssl_Hssl_FrameRequest_triggerFrame
 *                                       IfxHssl_Hssl_FrameRequest_readId
 *                                       IfxHssl_Hssl_FrameRequest_noAction
 *            - ChId           : Channel ID
 *                                       IfxHssl_ChannelId_0
 *                                       IfxHssl_ChannelId_1
 *                                       IfxHssl_ChannelId_2
 *                                       IfxHssl_ChannelId_3
 *            - DatLen         : Data length
 *                                       IfxHssl_DataLength_8bit
 *                                       IfxHssl_DataLength_16bit
 *                                       IfxHssl_DataLength_32bit
 *            - Adr            : Address
 *            - Dat            : Data
 *
 * @return :  uint32 errorFlags: hssl_ok     - frame request sent
 *                               hssl_failed - frame request not sent (channel was busy)
 * */
  #if 1
Hssl_return Hssl_frameReq(App_Hssl_instance *hssl_instance, IfxHssl_Hssl_FrameRequest frameRequest, IfxHssl_ChannelId ChId, IfxHssl_DataLength DatLen, uint32 Adr, uint32 Dat){

	if (IfxHssl_Hssl_singleFrameRequest(&(hssl_instance->hsslChannel[ChId]), frameRequest, Adr, Dat, DatLen)!=IfxHssl_Hssl_Status_ok)
		return hssl_failed;

	return hssl_ok;
}

/**
 * @Hssl_waitchannelIdle
 * Wait until channel becomes idle and ready to be used for the another request
 * Return code indicates an error on the last request executed before becoming idle
 *
 * @input  :  - hssl_instance  : Pointer to HSSL instance data structure
 *            - channelId      : HSSL channel
 *                                   IfxHssl_ChannelId_0
 *                                   IfxHssl_ChannelId_1
 *                                   IfxHssl_ChannelId_2
 *                                   IfxHssl_ChannelId_3
 *
 * @return :  uint32 errorFlags: hssl_ok     - channel is idle; last request completed successfully
 *                               hssl_failed - channel is idle; last request had an error
 * */

Hssl_return Hssl_waitchannelIdle(App_Hssl_instance *hssl_instance, IfxHssl_ChannelId channelId)
{

	/* Current iLLD does not check for BSY-flag of channel inside ICON for non-streaming mode.
	 * However, this is the proposed way to check for ongoing non-streaming request/response pairs.
	 * Therefore as a workaround the check is implemented here before proceeding with
	 * IfxHssl_Hssl_waitAcknowledge inside iLLD.
	 */
	if (hssl_instance->hsslChannel[channelId].streamingModeOn!=TRUE)
	{
		while(hssl_instance->hssl.hssl->I[channelId].ICON.B.BSY);
	}

	/* Proceed with iLLD to administrate iLLD internal states */
	while ( IfxHssl_Hssl_waitAcknowledge(&(hssl_instance->hsslChannel[channelId])) == IfxHssl_Hssl_Status_busy );

	/* Check for erros on hssl layer (hsct error are propagated to this layer by HW) */
	if (Hssl_getChannelErrorFlags(hssl_instance, channelId) != 0)
			return hssl_failed;
	return hssl_ok;
}

/**
 * @Hssl_getReadData
 * Fetch received 32bit data of last read request from register.
 *
 * @input  :  - channel  : Pointer to HSSL channel data structure
 *
 * @return :  uint32 value received from last read request
 * */


uint32 Hssl_getReadData(IfxHssl_Hssl_Channel *channel)
{
	return IfxHssl_Hssl_getReadData(channel);
}

/**
 * @Hssl_register_fctcall_cb
 * Register the callback which will be triggered by remote function calls of HSSL instance.
 *
 * @input  :  - hssl_instance  : Pointer to HSSL instance data structure
 *            - cb_fctptr      : Callback function pointer called for received remote function call
 * @return :  none
 * */

void Hssl_register_fctcall_cb(App_Hssl_instance *hssl_instance, void (*cb_fctptr)(uint32))
{
	/* Store callback function pointer for later use */
	hssl_instance->cb_fctcall_uint32 = cb_fctptr;
	return;
}

/**
 * @Hssl_register_parameter_ptr
 * Register
 * 1. the local address for incoming RFCs from remote side and
 * 2. the remote address for outgoing RFCs to remote side
 *
 * @input  :  - hssl_instance      : Pointer to HSSL instance data structure
 *            - ptr_uint32_local   : Address of the 32bit parameter on the local side  (incoming RFCs)
 *            - ptr_uint32_remote  : Address of the 32bit aprameter on the remote side (outgoing RFCs)
 *
 * @return :  none
 * */

void Hssl_register_parameter_ptr(App_Hssl_instance *hssl_instance, uint32 ptr_uint32_local, uint32 ptr_uint32_remote)
{
	/* Store pointers to share parameters for use during RFC */
	hssl_instance->ptr_RFC_uint32_para_local  = (uint32*) ptr_uint32_local;
	hssl_instance->ptr_RFC_uint32_para_remote = (uint32*) ptr_uint32_remote;
	return;
}

/**
 * @Hssl_fctcall
 * Trigger remote function call by HSSL instance.
 *
 * @input  :  - hssl_instance  : Pointer to HSSL instance data structure
 *            - para           : 32bit parameter passed to remote function
 *
 * @return :  hssl_ok        - functional call is triggered on remote device
 *            hssl_failed    - functional call was not triggered on remote device
 *            hssl_timeout   - trigger sent, but no confirmation received from remote device
 * */
Hssl_return Hssl_fctcall(App_Hssl_instance *hssl_instance, uint32 para)
{
	uint32 timeout = 100000;
	/* Check if already another RFC is still ongoing */
	if (hssl_instance->fctcall_state_send != Hssl_fctcall_idle)
		return hssl_failed;

	/* Check if pointer to write parameter on remote side has been set */
	if ( hssl_instance->ptr_RFC_uint32_para_remote == 0 )
		return hssl_failed;

	/* Send 32bit parameter for RFC to be fetched by remote side  */
    if ( Hssl_frameReq_wait(hssl_instance, IfxHssl_Command_writeFrame, IfxHssl_ChannelId_1, IfxHssl_DataLength_32bit,
    		           (uint32)hssl_instance->ptr_RFC_uint32_para_remote, para) != hssl_ok)
    	return hssl_failed;

	/* Trigger RFC on remote side  */
	hssl_instance->fctcall_state_send = Hssl_fctcall_triggered;
    if ( Hssl_frameReq_wait(hssl_instance, IfxHssl_Command_triggerFrame, IfxHssl_ChannelId_1, IfxHssl_DataLength_32bit, 0, 0) != hssl_ok)
    {	/* Trigger to remote side has failed, set send state back to idle */
    	hssl_instance->fctcall_state_send = Hssl_fctcall_idle;
    	return hssl_failed;;
    }

    /* Wait for confirmation of remote device or timeout */
	while((hssl_instance->fctcall_state_send == Hssl_fctcall_triggered) && (--timeout>0));

    /* Check if confirmation was received within timeout */
	if (timeout==0)
	{
		/* No confirmation received in time. Return back to idle state. */
		hssl_instance->fctcall_state_send = Hssl_fctcall_idle;
		return hssl_timeout;
	}
	else
	{
		/* RFC succeeded. */
		return hssl_ok;
	}
}
#endif