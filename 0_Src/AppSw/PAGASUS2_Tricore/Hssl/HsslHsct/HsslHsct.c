/**
 * \file HsslHsct.c
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
**                             Includes                                       **
*******************************************************************************/
#include "HsslHsct.h"
#include "internal_fw_debug.h"
/*******************************************************************************
**                             Defines                                        **
*******************************************************************************/
#define HSSLHSCT_ALWAYS_PRINTF(x) DEBUG_PRINTF_ALWAYS(x)
/*******************************************************************************
* 							   Global Variable Declarations                   **
*******************************************************************************/
/* HSCT port configurations for both instances */
static IfxHsct_In  IfxHsct0_Rx_P_IN    = {&MODULE_HSCT0, {&MODULE_P21, 3}, IfxPort_InputMode_noPullDevice, IfxPort_PadDriver_ttl3v3Speed1};
static IfxHsct_In  IfxHsct0_Rx_N_IN    = {&MODULE_HSCT0, {&MODULE_P21, 2}, IfxPort_InputMode_noPullDevice, IfxPort_PadDriver_ttl3v3Speed1};
static IfxHsct_Out IfxHsct0_Tx_P_OUT   = {&MODULE_HSCT0, {&MODULE_P21, 5}, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general, IfxPort_PadDriver_ttl3v3Speed1};
static IfxHsct_Out IfxHsct0_Tx_N_OUT   = {&MODULE_HSCT0, {&MODULE_P21, 4}, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general, IfxPort_PadDriver_ttl3v3Speed1};

static IfxHsct_In  IfxHsct_SysClk_IN  = {&MODULE_HSCT0, {&MODULE_P20, 0}, IfxPort_InputMode_noPullDevice, IfxPort_PadDriver_ttl3v3Speed1};
static IfxHsct_Out IfxHsct_SysClk_OUT = {&MODULE_HSCT0, {&MODULE_P20, 0}, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_alt5, IfxPort_PadDriver_ttl3v3Speed1};

/* HSSL/HSCT instances */
App_Hssl_instance hssl_instance_0;
App_Hsct_instance hsct_instance_0;

/*******************************************************************************
* 							   Local Variable Declarations                    **
*******************************************************************************/
static uint32 local_fct_parameter_instance_0 = 0;
static void (*fctcall_fct_ptr[256][2])(void);

/*******************************************************************************
* 							   Internal Function implementation               **
*******************************************************************************/

/**
 * @Hsct_InitializeLVDS
 * Initialize all LVDS specific settings of both HSCT0/HSCT1 instances.
 *
 * @input  :  none
 *
 * @return :  none
 * */
  #if 1
void HsctHssl_InitializeLVDS(void) {
	uint16 passwd = IfxScuWdt_getCpuWatchdogPassword();
	IfxScuWdt_clearCpuEndinit(passwd);
	IfxHsct0_Rx_P_IN.pin.port->LPCR[1].B.REN_CTRL     = 1;
	IfxHsct0_Rx_P_IN.pin.port->LPCR[1].B.RX_EN        = 1;
	IfxHsct0_Rx_P_IN.pin.port->LPCR[1].B.TERM         = 1;
	IfxHsct0_Rx_P_IN.pin.port->LPCR[1].B.LVDSM        = 0;
	IfxHsct0_Rx_P_IN.pin.port->LPCR[1].B.PS           = 0;

	IfxHsct0_Tx_P_OUT.pin.port->LPCR[2].B.TX_PWDPD    = 0;
	IfxHsct0_Tx_P_OUT.pin.port->LPCR[2].B.TX_PD       = 0;
	IfxHsct0_Tx_P_OUT.pin.port->LPCR[2].B.TX_EN       = 1;
	IfxHsct0_Tx_P_OUT.pin.port->LPCR[2].B.TEN_CTRL    = 1;
	IfxHsct0_Tx_P_OUT.pin.port->LPCR[2].B.PS          = 0;

 	IfxScuWdt_setCpuEndinit(passwd);
}

/**
 * @HsctHssl_InitializeAllPorts
 * Initialize all ports of both HSCT0/HSCT1 instances.
 *
 * @input  :  none
 *
 * @return :  none
 * */

void HsctHssl_InitializeAllPorts(IfxHssl_InterfaceMode interfaceMode)
{
	IfxPort_setPinModeInput(IfxHsct0_Rx_P_IN.pin.port, IfxHsct0_Rx_P_IN.pin.pinIndex, IfxHsct0_Rx_P_IN.mode);
	IfxPort_setPinModeInput(IfxHsct0_Rx_N_IN.pin.port, IfxHsct0_Rx_N_IN.pin.pinIndex, IfxHsct0_Rx_N_IN.mode);
	IfxPort_setPinModeOutput(IfxHsct0_Tx_P_OUT.pin.port, IfxHsct0_Tx_P_OUT.pin.pinIndex, IfxHsct0_Tx_P_OUT.mode, IfxHsct0_Tx_P_OUT.idx);
	IfxPort_setPinModeOutput(IfxHsct0_Tx_N_OUT.pin.port, IfxHsct0_Tx_N_OUT.pin.pinIndex, IfxHsct0_Tx_N_OUT.mode, IfxHsct0_Tx_N_OUT.idx);

	IfxPort_setPinPadDriver(IfxHsct0_Rx_P_IN.pin.port, IfxHsct0_Rx_P_IN.pin.pinIndex, IfxHsct0_Rx_P_IN.driver);
	IfxPort_setPinPadDriver(IfxHsct0_Rx_N_IN.pin.port, IfxHsct0_Rx_N_IN.pin.pinIndex, IfxHsct0_Rx_N_IN.driver);
	IfxPort_setPinPadDriver(IfxHsct0_Tx_P_OUT.pin.port, IfxHsct0_Tx_P_OUT.pin.pinIndex, IfxHsct0_Tx_P_OUT.driver);
	IfxPort_setPinPadDriver(IfxHsct0_Tx_N_OUT.pin.port, IfxHsct0_Tx_N_OUT.pin.pinIndex, IfxHsct0_Tx_N_OUT.driver);

	HsctHssl_InitializeLVDS();

	if (interfaceMode==IfxHssl_InterfaceMode_master) {
		IfxPort_setPinPadDriver(IfxHsct_SysClk_OUT.pin.port, IfxHsct_SysClk_OUT.pin.pinIndex, IfxHsct_SysClk_OUT.driver);
		IfxPort_setPinModeOutput(IfxHsct_SysClk_OUT.pin.port, IfxHsct_SysClk_OUT.pin.pinIndex, IfxHsct_SysClk_OUT.mode, IfxHsct_SysClk_OUT.idx);
	}
	else {
		IfxPort_setPinPadDriver(IfxHsct_SysClk_IN.pin.port, IfxHsct_SysClk_IN.pin.pinIndex, IfxHsct_SysClk_IN.driver);
		IfxPort_setPinModeInput(IfxHsct_SysClk_IN.pin.port, IfxHsct_SysClk_IN.pin.pinIndex, IfxHsct_SysClk_IN.mode);
	}
}

/**
 * @HsctHssl_instance0_fctcall_cb
 * RFC callback registered to HSSL instance 0.
 * Depending on the parameter the call is forwarded to corresponding function
 * registered inside the function pointer array of the instance.
 * See also API function HsctHssl_register_cb.
 *
 * @input  :  - para  : 32-bit parameter received by RFC
 *
 * @return :  none
 * */
static void HsctHssl_instance0_fctcall_cb(uint32 para)
{
	/* RFC call from remote side received on HSSL instance 0
	 * Check if a void (*fctcall_fct_ptr)(void) is registered to the 32 bit parameter
	 * and if yes, forward the call to the function.
	 */
	if (para < 256)
	{
			if (fctcall_fct_ptr[para][0] != 0)
				fctcall_fct_ptr[para][0]();
	}
	return;
}
#endif

/**
 * @HsctHssl_instance1_fctcall_cb
 * RFC callback registered to HSSL instance 1.
 * Depending on the parameter the call is forwarded to corresponding function
 * registered inside the function pointer array of the instance.
 * See also API function HsctHssl_register_cb.
 *
 * @input  :  - para  : 32-bit parameter received by RFC
 *
 * @return :  none
 * */
static void HsctHssl_instance1_fctcall_cb(uint32 para)
{
	/* RFC call from remote side received on HSSL instance 1
	 * Check if a void (*fctcall_fct_ptr)(void) is registered to the 32 bit parameter
	 * and if yes, forward the call to the function.
	 */
	if (para < 256)
	{
			if (fctcall_fct_ptr[para][1] != 0)
				fctcall_fct_ptr[para][1]();
	}
	return;
}

/**
 * @HsctHssl_startstreaming_intern
 * Start streaming of data from this device to the remote device.
 * Remote device (target) is setup using channel 3.
 * Streaming with HsctHssl_startstreaming_intern must only be started when
 * there is no other streaming already ongoing/started.
 * See also HsctHssl_waitstreamingdone.
 *
 * Optionally,
 *        1. the function can be blocking until streaming is finished.
 *        or
 *        2. the function returns immediately even while streaming still is ongoing
 *           (e.g. to start streaming on the other HSSL-instance in parallel)
 *           In this case you have to make sure to call IfxHsctHssl_waitstreamingdone
 *           before another streaming is started again on this HSSL instance here.
 *
 * @input  :  - hssl_instance   : Pointer to HSSL instance data structure
 *            - source_adr      : Source start address on this (=initiator) device
 *            - destination_adr : Destination start address on the target device
 *            - frame_cnt       : Defines the amount of data for streaming (multiple of 32 bytes)
 *            - blocking        : TRUE  call will be blocking until streaming has finished
 *                                FALSE call will return immediately while streaming is still ongoing
 *                                      see also HsctHssl_waitstreamingdone
 *
 * @return :  hscthssl_ok       : Streaming started successfully   (if blocking was set FALSE)
 *                                Streaming finalized successfully (if blocking was set TRUE)
 *            hscthssl_setup_err: Target setup error (on ch3)
 *            hscthssl_failed   : Streaming error (only if blocking was set to TRUE)
 *
 * */
  #if 1
static HsctHssl_return HsctHssl_startstreaming_intern(App_Hssl_instance *hssl_instance, uint32 source_adr, uint32 destination_adr, uint32 frame_cnt, boolean blocking)
{
	Ifx_HSSL_TS_FC readTSFC_reg;
	Ifx_HSSL_MFLAGSSET mflags;
	mflags.U = 0;
	mflags.B.TSES = 0x1;

	/* Set CFG register on target for preparation of next streaming */
	if (HsctHssl_frameReq(hssl_instance, IfxHssl_Command_writeFrame, IfxHssl_DataLength_32bit, (uint32)&hssl_instance->hssl.hssl->CFG.U, (uint32)0x00073FFF, TRUE, 3) != hscthssl_ok)
		return hscthssl_setup_err;

	/* Set destination address inside register on target for preparation of next streaming */
	if (HsctHssl_frameReq(hssl_instance, IfxHssl_Command_writeFrame, IfxHssl_DataLength_32bit, (uint32)&hssl_instance->hssl.hssl->TS.SA[0].U, destination_adr, TRUE, 3) != hscthssl_ok)
		return hscthssl_setup_err;

	/* Precautious meassurement on target because: !!! Last streaming might have failed/interrupted !!!
	 * Read TSFC and set CURCOUNT equal to RELCOUNT before resetting TSE
	 * for details see also  section 35.3.7 Memory Block Transfer Modes of the Stream Channel of user manual */
	if (HsctHssl_frameReq(hssl_instance, IfxHssl_Command_readFrame, IfxHssl_DataLength_32bit, (uint32)&hssl_instance->hssl.hssl->TS.FC.U, 0, TRUE, 3) != hscthssl_ok)
		return hscthssl_setup_err;
	readTSFC_reg.U = HsctHssl_getReadData(hssl_instance);
	readTSFC_reg.B.RELCOUNT = readTSFC_reg.B.CURCOUNT;
	if (HsctHssl_frameReq(hssl_instance, IfxHssl_Command_writeFrame, IfxHssl_DataLength_32bit, (uint32)&hssl_instance->hssl.hssl->TS.FC.U, readTSFC_reg.U, TRUE, 3) != hscthssl_ok)
		return hscthssl_setup_err;

	if (HsctHssl_frameReq(hssl_instance, IfxHssl_Command_writeFrame, IfxHssl_DataLength_32bit, (uint32)&hssl_instance->hssl.hssl->MFLAGSCL.U, mflags.U, TRUE, 3) != hscthssl_ok)
		return hscthssl_setup_err;

	/* Set frame count inside register on target for preparation of next streaming */
	if (HsctHssl_frameReq(hssl_instance, IfxHssl_Command_writeFrame, IfxHssl_DataLength_32bit, (uint32)&hssl_instance->hssl.hssl->TS.FC.U, frame_cnt, TRUE, 3) != hscthssl_ok)
		return hscthssl_setup_err;

	/* Set/Enable target for streaming for preparation of next streaming */
	if (HsctHssl_frameReq(hssl_instance, IfxHssl_Command_writeFrame, IfxHssl_DataLength_32bit, (uint32)&hssl_instance->hssl.hssl->MFLAGSSET.U, mflags.U, TRUE, 3) != hscthssl_ok)
		return hscthssl_setup_err;

	/* Start streaming by using iLLD */
	hssl_instance->hsslChannel[IfxHssl_ChannelId_2].streamingModeOn = TRUE;
	IfxHssl_Hssl_writeStream(&hssl_instance->hssl, (uint32 *)source_adr, frame_cnt);

	/* Shall we wait until streaming is finished ?*/
	if (blocking == TRUE)
	{
		/* Wait until streaming is finished and return final status */
		return HsctHssl_waitstreamingdone(hssl_instance);
	}
	else
	{
		/* Streaming started successfully and is still ongoing */
		return hscthssl_ok;
	}
}


/*******************************************************************************
* 							   API functions                                  **
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

void HsctHssl_init(IfxHssl_InterfaceMode interfaceMode)
{
    /* Initialise hsct instances */
    hsct_instance_0.hsct.hsct=&MODULE_HSCT0;

    hsct_instance_0.isr_base_prio_hsct = ISR_PRIORITY_HSCT0;

    hsct_instance_0.interfaceMode=interfaceMode;
    hsct_instance_0.test_mode_enable = FALSE;

    hsct_instance_0.usm_zero_message_response = (uint32) &(local_fct_parameter_instance_0);
    hsct_instance_0.usm_zero_message_response_received = 0;

    HSSLHSCT_ALWAYS_PRINTF(("Init Hsct\r\n"));
    Hsct_initModule(&hsct_instance_0);

    /* Initialise hssl instances */
    hssl_instance_0.hssl.hssl=&MODULE_HSSL0;

    hssl_instance_0.isr_base_prio_hssl = ISR_PRIORITY_HSSL0_BASE_PRIO;

    HSSLHSCT_ALWAYS_PRINTF(("Init hssl\r\n"));
	Hssl_initModule(&hssl_instance_0);

	/* Initialise RFC */
	while(hsct_instance_0.usm_zero_message_response_received == 0)
		Hsct_getUSMresponse(&hsct_instance_0);

	Hssl_register_parameter_ptr(&hssl_instance_0,
			                    (uint32) &(local_fct_parameter_instance_0),
								hsct_instance_0.usm_zero_message_response_received);
	int i;
	for (i=0 ; i<=255 ; i++)
	{
		fctcall_fct_ptr[i][0]=0;
		fctcall_fct_ptr[i][1]=0;
	}

	Hssl_register_fctcall_cb(&hssl_instance_0, HsctHssl_instance0_fctcall_cb);

	HsctHssl_InitializeAllPorts(interfaceMode);

	if(Hsct_Master_HighSpeed(&hsct_instance_0) == hsct_ok)
	{
		HSSLHSCT_ALWAYS_PRINTF(("Master: switch to High speed\r\n"));
	}
	else
	{
		if(hsct_instance_0.interfaceMode == IfxHssl_InterfaceMode_master)
		{
			HSSLHSCT_ALWAYS_PRINTF(("Master: FAILed to switch to High speed\r\n"));
		}
	}
	waitTime(1000*TimeConst[TIMER_INDEX_1MS]);
}

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
void HsctHssl_register_cb(App_Hssl_instance *hssl_instance, uint8 fctcall_ID, void (*cb_fctptr)(void))
{
	/* Register handlers inside function pointer array void (*fctcall_fct_ptr)(void) for RFC */
	if ((hssl_instance->hssl.hssl)==&MODULE_HSSL0)
		fctcall_fct_ptr[fctcall_ID][0]=cb_fctptr;
	else
		fctcall_fct_ptr[fctcall_ID][1]=cb_fctptr;
}

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
HsctHssl_return HsctHssl_fctcall(App_Hssl_instance *hssl_instance, uint8 fctcall_ID, uint32 trials)
{
	Hssl_return status = hssl_failed;
	/* Trigger RFC to the remote side */
	while ((status != hssl_ok) && (trials > 0))
	{
		status = Hssl_fctcall(hssl_instance, fctcall_ID);
		waitTime(100*TimeConst[TIMER_INDEX_1MS]);
		trials--;
	}

	if (status == hssl_ok)
		return hscthssl_ok;

	if (status == hssl_timeout)
		return hscthssl_timeout;

	return hscthssl_failed;
}

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
 *                                       IfxHssl_Hssl_FrameRequest_noAction
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
HsctHssl_return HsctHssl_frameReq(App_Hssl_instance *hssl_instance, IfxHssl_Hssl_FrameRequest frameRequest,
		                          IfxHssl_DataLength DatLen, uint32 Adr, uint32 Dat, boolean blocking, uint32 trials){
	Hssl_return status = hssl_failed;

	if (blocking == FALSE)
	{
		if (Hssl_frameReq(hssl_instance, frameRequest, IfxHssl_ChannelId_3, DatLen, Adr, Dat) != hssl_ok)
			return hscthssl_ch_busy;
		else
			return hscthssl_ok;
	}

	while ((trials>0) && (status!= hssl_ok))
	{
		if (Hssl_frameReq(hssl_instance, frameRequest, IfxHssl_ChannelId_3, DatLen, Adr, Dat) != hssl_ok)
			return hscthssl_ch_busy;
		status = Hssl_waitchannelIdle(hssl_instance, IfxHssl_ChannelId_3);
		trials--;
	}

	if (status == hssl_failed)
		return hscthssl_failed;
	else
		return hscthssl_ok;
}
#endif

/**
 * @HsctHssl_waitchannelIdle
 * Wait until channel 3 becomes idle and ready to be used for the another request
 * Return code indicates an error on the last request executed before becoming idle
 *
 * @input  :  - hssl_instance  : Pointer to HSSL instance data structure
 *
 * @return :  hcthssl_ok      - channel is now idle; last request completed successfully
 *            hscthssl_failed - channel is now idle; last request had an error
 * */
HsctHssl_return HsctHssl_waitchannelIdle(App_Hssl_instance *hssl_instance)
{
	Hssl_return status = Hssl_waitchannelIdle(hssl_instance, IfxHssl_ChannelId_3);

	if (status == hssl_failed)
		return hscthssl_failed;
	else
		return hscthssl_ok;
}

/**
 * @HsctHsslHssl_getReadData
 * Fetch received 32bit data of last read request of channel 3 from register.
 *
 * @input  :  - hssl_instance  : Pointer to HSSL instance data structure
 *
 * @return :  uint32 value received from last read request on ch3
 * */
  #if 1
uint32 HsctHssl_getReadData(App_Hssl_instance *hssl_instance)
{
	return Hssl_getReadData(&hssl_instance->hsslChannel[IfxHssl_ChannelId_3]);
}

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
 *           In this case you have to make sure to call IfxHsctHssl_waitstreamingdone
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

HsctHssl_return HsctHssl_startstreaming(App_Hssl_instance *hssl_instance, uint32 source_adr, uint32 destination_adr, uint32 relcnt, boolean blocking, uint32 trials)
{
	HsctHssl_return status = hscthssl_failed;

	if (blocking==FALSE)
		return HsctHssl_startstreaming_intern(hssl_instance, source_adr, destination_adr, relcnt, blocking);

	while ((trials>0) && (status!= hscthssl_ok))
	{
		status = HsctHssl_startstreaming_intern(hssl_instance, source_adr, destination_adr, relcnt, blocking);
		trials--;
	}
	return status;
}

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
HsctHssl_return HsctHssl_waitstreamingdone(App_Hssl_instance *hssl_instance)
{
	/* Wait until streaming is finished and return final status */
	Hssl_return status = Hssl_waitchannelIdle(hssl_instance, IfxHssl_ChannelId_2);

	if (status == hssl_failed)
		return hscthssl_failed;
	else
		return hscthssl_ok;
}


void HsctLoopback_test(uint32 testNumber)
{
	HSSLHSCT_ALWAYS_PRINTF(("Start loop back test with Hsct \r\n"));
	boolean test_result = TRUE;
	hsct_instance_0.test_mode_enable = TRUE;
    hsct_instance_0.usm_zero_message_response = 0;
    hsct_instance_0.usm_zero_message_response_received = 0;
	while(hsct_instance_0.usm_zero_message_response_received != testNumber)
	{
		if(Hsct_testLookback(&hsct_instance_0, testNumber) == FALSE)
		{
			test_result = FALSE;
			break;
		}
	}
	if(hsct_instance_0.interfaceMode == IfxHssl_InterfaceMode_master)
	{
		if(test_result)
		{
			HSSLHSCT_ALWAYS_PRINTF(("HSSL Loop Back Test Pass\r\n"));
		}
		else
		{
			HSSLHSCT_ALWAYS_PRINTF(("HSSL Loop Back Test Fail\r\n"));
		}
	}
	else
	{
		HSSLHSCT_ALWAYS_PRINTF(("HSSL Loop Back Test End. Please check result in master\r\n"));
	}
	hsct_instance_0.test_mode_enable = FALSE;
}
#endif
