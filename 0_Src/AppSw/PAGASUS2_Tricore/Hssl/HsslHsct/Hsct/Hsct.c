/**
 * \file Hsct.c
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

#define HSCT_ALWAYS_PRINTF(x) DEBUG_PRINTF_ALWAYS(x)

/*******************************************************************************
**                             Defines                                        **
*******************************************************************************/
IFX_INTERRUPT(ISR_Hsct_Master0, ISR_PROVIDER_HSCT, ISR_PRIORITY_HSCT0);
IFX_INTERRUPT(ISR_Hsct_Master1, ISR_PROVIDER_HSCT, ISR_PRIORITY_HSCT1);

/*******************************************************************************
* 							   Global Variable Declarations                   **
*******************************************************************************/
IfxHssl_Hsct_Config  hsctConfig;

/*******************************************************************************
* 							   Local Variable Declarations                    **
*******************************************************************************/
static App_Hsct_instance *hsct_instance[HSCT_INSTANCE_COUNT];

/*******************************************************************************
* 							   Internal Function implementation               **
*******************************************************************************/
/**
 * @Hsct_Master_WaitPoll_pingAnswerReceived
 * Only to be called on master
 * Wait until ping answer from slave was received.
 *
 * @input  :  - hsct         : Pointer to HSCT registers
 *              timeout      : wait timeout (typically 0xffff)
 *
 * @return :  - status 	hsct_ok      success
 *                      hsct_timeout no ping answer received within timeout
 * */
   #if 1
Hsct_return Hsct_Master_WaitPoll_pingAnswerReceived(Ifx_HSCT *hsct, uint32 timeout)
{
	/* Wait with timeout until ping answer was received */
	while((!(IfxHssl_getHsctInterruptFlagStatus(hsct, IfxHssl_Hsct_InterruptSource_pingAnswerReceived)))&&(timeout>0))
		timeout--;

	/* Clear PAR interrupt flag */
	IfxHssl_clearHsctInterruptFlag(hsct, IfxHssl_Hsct_InterruptSource_pingAnswerReceived);

	/* Success if timeout did not reach zero */
	if (timeout==0)
		return hsct_timeout;
	else
		return hsct_ok;
}

/**
 * @Hsct_Master_WaitPoll_interfaceControlFrameSend
 * Only to be called on master
 * Wait until interface control frame was sent by master.
 *
 * @input  :  - hsct         : Pointer to HSCT registers
 *              timeout      : wait timeout (typically 0xffff)
 *
 * @return :  - status 	hsct_ok      success
 *                      hsct_timeout IFC not sent within timeout
 * */

Hsct_return Hsct_Master_WaitPoll_interfaceControlFrameSend(Ifx_HSCT *hsct, uint32 timeout)
{
	/* Wait with timeout until interface control frame was sent */
	while((!(IfxHssl_getHsctInterruptFlagStatus(hsct, IfxHssl_Hsct_InterruptSource_interfaceControlFrameSend)))&&(timeout>0))
		timeout--;

	/* Clear IFCS interrupt flag */
	IfxHssl_clearHsctInterruptFlag(hsct, IfxHssl_Hsct_InterruptSource_interfaceControlFrameSend);

	/* Success if timeout did not reach zero */
	if (timeout==0)
		return hsct_timeout;
	else
		return hsct_ok;
}

/**
 * @Hsct_Master_setSpeed
 * Only to be called on master
 * Set speed mode on master.
 *
 * @input  :  - hsct         : Pointer to HSCT registers
 *              link_speed   : tx_lowspeed
 *                             rx_lowspeed
 *                             tx_highspeed
 *                             rx_highspeed
 *
 * @return :  none
 * */

void Hsct_Master_setSpeed(Ifx_HSCT *hsct, Hsct_LinkSpeed link_speed){
	switch(link_speed) {

		case tx_lowspeed  : hsct->IFCTRL.B.MTXSPEED = 0; // set tx link to low speed
		                    break;
		case rx_lowspeed  : hsct->IFCTRL.B.MRXSPEED = 0; // set rx link to low speed
				            break;
		case tx_highspeed : hsct->IFCTRL.B.MTXSPEED = 2; // set tx link to high speed
				            break;
		case rx_highspeed : hsct->IFCTRL.B.MRXSPEED = 2; // set rx link to high speed
				            break;

	}
}
#endif
/**
 * @ISR_Hsct
 * Generic Interrupt Service Routine of HSCT module
 * Check if an USM message was received.
 * In case of null message received, respond with the pre-defined message
 * on null messages for this HSCT instance.
 * Otherwise this message is stored as the response to a null message
 * of the other HSCT instance.
 *
 * @input  :  - instance  : Pointer to HSCT instance data structure
 *
 * @return :  none
 *
 * */
void ISR_Hsct(App_Hsct_instance *hsct_instance)
{
	uint32 receivedUSM;

	if(hsct_instance_0.test_mode_enable == FALSE)
	{
		/* Interrupt triggered by received USM ? */
		if (IfxHssl_getHsctInterruptFlagStatus(hsct_instance->hsct.hsct, IfxHssl_Hsct_InterruptSource_UnsolicitedMessageReceived))
		{
			/* New USM received */
			IfxHssl_clearHsctInterruptFlag(hsct_instance->hsct.hsct, IfxHssl_Hsct_InterruptSource_UnsolicitedMessageReceived);
			/* Fetch received USM */
			receivedUSM = IfxHssl_getHsctUnsolicitedStatusMessage(hsct_instance->hsct.hsct);
			/* "zero-message" received ?*/
			if ( receivedUSM == 0x0 )
			{
				/* zero message received
				 * --> respond with the user defined USM message */
				IfxHssl_sendHsctUnsolicitedStatusMessage(hsct_instance->hsct.hsct, hsct_instance->usm_zero_message_response);
			}
			else
			{
				/* non-zero message received
				 * --> store received USM as the "zero-message" response
				 *     of the remote device
				 */
				hsct_instance->usm_zero_message_response_received = receivedUSM;
			}
		}
	}
}

/**
 * @ISR_Hsct_Master0
 * Explicit Interrupt Service Routine of HSCT0.
 * Used to forward call to Generic Interrupt Service ISR_Hsct Routine of HSCT module
 *
 * @input  : none
 *
 * @return :  none
 *
 * */
void ISR_Hsct_Master0(void){
	ISR_Hsct (hsct_instance[0]);
}

/**
 * @ISR_Hsct_Master1
 * Explicit Interrupt Service Routine of HSCT1.
 * Used to forward call to Generic Interrupt Service Routine of HSCT module
 *
 * @input  : none
 *
 * @return :  none
 *
 * */
void ISR_Hsct_Master1(void){
	ISR_Hsct (hsct_instance[1]);
}

/**
 * @Hsct_Master_CheckLink
 * Only to be called on master
 * Sends ping to slave and waits for ping answer.
 *
 * @input  :  - hsct_module  : Pointer to HSCT instance data structure
 *              trials       : number of tries before function returns if no
 *                             ping answer can be received
 *
 * @return :  - status 	hsct_ok      success
 *                      hsct_timeout no ping answer received within timeout
 * */
   #if 1
Hsct_return Hsct_Master_CheckLink(IfxHssl_Hsct *hsct_module,uint32 trials){
	Hsct_return status=hsct_failed;

	/* Check link (ping) makes only sense for master */
	if (!(hsct_module->hsct->INIT.B.IFM))
	{
		while ( (trials > 0) && (status!=hsct_ok))
		{
			trials--;
			IfxHssl_Hssl_sendControlCommand(hsct_module, send_ping);
			status = Hsct_Master_WaitPoll_pingAnswerReceived(hsct_module->hsct,0xffff);
		}
	}
	return status;
}

/**
 * @Hsct_Master_TestSpeed
 * Only to be called on master
 * Sets master speed mode to the specified setting and checks if slave answers to pings.
 *
 * @input  :  - hsct_module  : Pointer to HSCT instance data structure
 *
 * @return :  - status 	hsct_ok      success
 *                      hsct_timeout ping answer from slave side timed out
 *                      hsct_failed  failed (only to be called on master)
 * */

Hsct_return Hsct_Master_TestSpeed(IfxHssl_Hsct *hsct_module, Hsct_LinkSpeed tx_speed, Hsct_LinkSpeed rx_speed ){
	/* Test speed modes makes only sense for master */
	if (!(hsct_module->hsct->INIT.B.IFM))
	{
		/* Set tx/rx speed on master */
		Hsct_Master_setSpeed(hsct_module->hsct, tx_speed);
		Hsct_Master_setSpeed(hsct_module->hsct, rx_speed);

		/* send IFC to make sure slave tx is enabled */
		IfxHssl_Hssl_sendControlCommand(hsct_module, enable_slave_tx);
		Hsct_Master_WaitPoll_interfaceControlFrameSend(hsct_module->hsct, 0xffffffff);

		/* check if ping answer can be received (max. 3 trials) */
		return Hsct_Master_CheckLink(hsct_module, 3);
	}
	return hsct_failed;
}

/**
 * @Hsct_Master_ScanLinkSpeed
 * Only to be called on master
 * Aligns speed mode of master to current slave speed mode until correct ping answer
 * can be received.
 *
 * @input  :  - hsct_module  : Pointer to HSCT instance data structure
 *
 * @return :  - status 	hsct_ok      success
 *                      hsct_failed  no valid speed mode found
 * */

Hsct_return Hsct_Master_ScanLinkSpeed(IfxHssl_Hsct  *hsct_module){
	/* Scan makes only sense for master */
	if (!(hsct_module->hsct->INIT.B.IFM))
	{
		/* Check all possible speed modes and return if success */
		if (Hsct_Master_TestSpeed(hsct_module, tx_highspeed,rx_highspeed)==hsct_ok)
			return hsct_ok;

		if (Hsct_Master_TestSpeed(hsct_module, tx_lowspeed,rx_lowspeed)==hsct_ok)
			return hsct_ok;

		if (Hsct_Master_TestSpeed(hsct_module, tx_lowspeed,rx_highspeed)==hsct_ok)
			return hsct_ok;

		if (Hsct_Master_TestSpeed(hsct_module, tx_highspeed,rx_lowspeed)==hsct_ok)
			return hsct_ok;
	}
	return hsct_failed;
}

/**
 * @Hsct_enableLVDStermination
 * Enable LVDS termination on HSCT1/0 ports.
 *
 * @input  :  none
 *
 * @return :  none
 *
 *
 * */

void Hsct_enableLVDStermination(void)
{
	uint16 passwd = IfxScuWdt_getCpuWatchdogPassword();
	IfxScuWdt_clearCpuEndinit(passwd);

	/* Enable termination of port P21.0/P21.1 */
	MODULE_P21.LPCR[0].B.TERM=1;

	/* Enable termination of port P21.2/P21.3 */
	MODULE_P21.LPCR[1].B.TERM=1;

 	IfxScuWdt_setCpuEndinit(passwd);
}

/*******************************************************************************
**                             API Function implementation                    **
*******************************************************************************/
/**
 * @Hsct_initModule
 * Initializes and enables the HSCT module including ports (internal 100 Ohm LVDS termination)
 * For proper initialisation specify
 * 	- reference to HSCT-register base address
 * 	- USM-response value on zero-requests
 * 	- ISR base prio
 * 	- interface mode (master/slave)
 * inside App_Hsct_instance input parameter
 *
 * @input  :  - instance  : Pointer to HSCT instance data structure
 *
 * @return :  - status 	hsct_ok      success
 *                      hsct_failed  failed
 *
 * */

Hsct_return Hsct_initModule(App_Hsct_instance *instance) {

	/* USM zero message response must not be zero */
	//if (instance->usm_zero_message_response==0)
	//	return hsct_failed;
	//if (instance->usm_zero_message_response_received!=0)
	//	return hsct_failed;

	/* Store instance pointer for use in generic ISR handling */
	if (instance->hsct.hsct==&MODULE_HSCT0)
		hsct_instance[0]=instance;
	else
	{
		if (instance->hsct.hsct==&MODULE_HSCT1)
			hsct_instance[1]=instance;
		else
			return hsct_failed;
	}

	/* Initialize the configuration structure of iLLD */
	IfxHssl_Hssl_initHsctModuleConfig(&hsctConfig, instance->hsct.hsct);

	/* Set interface mode */
	hsctConfig.interfaceMode=instance->interfaceMode;

	/* Initialize HSCT module */
	IfxHssl_Hssl_initHsctModule(&(instance->hsct), &hsctConfig);
	Hsct_enableLVDStermination();

	/* Enable HSCT interrupt for USM */
	IfxHssl_clearHsctInterruptFlag (instance->hsct.hsct,IfxHssl_Hsct_InterruptSource_UnsolicitedMessageReceived);

	IfxHssl_enableHsctInterruptFlag(instance->hsct.hsct,IfxHssl_Hsct_InterruptSource_UnsolicitedMessageReceived);

	/* Enable global HSCT interrupt */
	IfxHssl_enableHsctInterrupt(instance->hsct.hsct, IfxSrc_Tos_cpu0, instance->isr_base_prio_hsct);

	return hsct_ok;
}

/**
 * @Hsct_getUSMresponse
 *
 * Sends a zero-value USM message and polls until answer is received.
 * Returns the received answer from remote side.
 *
 * @input  :  - instance  : Pointer to HSCT instance data structure
 *
 * @return :  - received USM message
 *              (zero of no connection to remote side could be established)
 * */

uint32 Hsct_getUSMresponse(App_Hsct_instance *instance)
{
	uint32 timeout = 100;

	/* Align speed mode of master */
	Hsct_Master_ScanLinkSpeed( &(instance->hsct) );

	/* Reset last USM response received */
	instance->usm_zero_message_response_received = 0;

    /* Send zero message to trigger the sending of zero message response from remote side ...*/
	IfxHssl_sendHsctUnsolicitedStatusMessage(instance->hsct.hsct, 0x0 /*zero*/);

	/* ... wait until zero message response is received or time out */
	while ( ((--timeout)>0) && (instance->usm_zero_message_response_received == 0 ))
	{
		waitTime(1*TimeConst[TIMER_INDEX_1MS]);
	}

	return instance->usm_zero_message_response_received;
}
#endif
/**
 * @Hsct_Master_LowSpeed
 * Only to be called on master
 * Aligns speed mode of master to current slave speed mode until correct ping answer
 * can be received.
 * Afterwards sets master and slave speed to low speed on RX and TX.
 *
 * @input  :  - instance  : Pointer to HSCT instance data structure
 *
 * @return :  - status 	hsct_ok      success
 *                      hsct_timeout ping to slave timed out
 *                      hsct_failed  failed
 * */
Hsct_return Hsct_Master_LowSpeed(App_Hsct_instance *instance)
{
	IfxHssl_Hsct *hsct_module = &instance->hsct;

	/* Align master speed mode to current slave speed mode */
	if ( (Hsct_Master_ScanLinkSpeed(hsct_module) != hsct_ok) )
		return hsct_failed;

	/* Send IFC: slave tx to low speed */
	IfxHssl_Hssl_sendControlCommand(hsct_module, ls_slave_to_master);
	Hsct_Master_WaitPoll_interfaceControlFrameSend(hsct_module->hsct, 0xffffffff);

	/* Set master rx to low speed */
	Hsct_Master_setSpeed(hsct_module->hsct, rx_lowspeed);

	/* Send IFC: slave rx to low speed */
	IfxHssl_Hssl_sendControlCommand(hsct_module, ls_master_to_slave);
	Hsct_Master_WaitPoll_interfaceControlFrameSend(hsct_module->hsct, 0xffffffff);

	uint32 i;
	/* Wait loop needed to not interrupt sending of interface command
	 * see also "Section 35.7.3.3.3 Interface Control"
	 * inside table "Processing Times of Interface Control"
	 * */
	for (i=0; i<5000;i++);

	/* Set master tx to low speed */
	Hsct_Master_setSpeed(hsct_module->hsct, tx_lowspeed);

	/* Check if speed setting succeeded */
	return Hsct_Master_CheckLink (hsct_module, 5);
}

/**
 * @Hsct_Master_HighSpeed
 * Only to be called on master
 * Aligns speed mode of master to current slave speed mode until correct ping answer
 * can be received.
 * Afterwards sets master and slave speed to high speed on RX and TX.
 *
 * @input  :  - instance  : Pointer to HSCT instance data structure
 *
 * @return :  - status 	hsct_ok      success
 *                      hsct_timeout ping to slave side timed out for all speed modes
 *                      hsct_failed  failed
 * */
 #if 1
Hsct_return Hsct_Master_HighSpeed(App_Hsct_instance *instance)
{
	IfxHssl_Hsct *hsct_module = &instance->hsct;

	/* Align master speed mode to current slave speed mode*/
	if ( (Hsct_Master_ScanLinkSpeed(hsct_module) != hsct_ok) )
		return hsct_failed;

	/* Send IFC: Set slave tx to high speed */
	IfxHssl_Hssl_sendControlCommand(hsct_module, hs_slave_to_master);
	Hsct_Master_WaitPoll_interfaceControlFrameSend(hsct_module->hsct, 0xffffffff);

	/* Set master rx to high speed */
	Hsct_Master_setSpeed(hsct_module->hsct, rx_highspeed);

	/* Send IFC: Set slave rx to high speed */
	IfxHssl_Hssl_sendControlCommand(hsct_module, hs_master_to_slave);
	Hsct_Master_WaitPoll_interfaceControlFrameSend(hsct_module->hsct, 0xffffffff);

	volatile uint32 i;
	/* Wait loop needed to not interrupt sending of interface command
	 * see also "Section 35.7.3.3.3 Interface Control"
	 * inside table "Processing Times of Interface Control"
	 * */
	for (i=0; i<5000;i++);

	/* Set master tx to high speed */
	Hsct_Master_setSpeed(hsct_module->hsct, tx_highspeed);

	/* Check if speed setting succeeded */
	return Hsct_Master_CheckLink (hsct_module, 5);
}
#endif
/**
 * @Hsct_isLinkActive
 * Checks active link on USM response from remote side.
 * Sends a zero-value USM message and polls until answer is received or timeout triggered.
 * Stores the received answer from remote side inside HSCT instance structure.
 *
 * @input  :  - instance  : Pointer to HSCT instance data structure
 *            - timeout   : timeout value
 *
 * @return :  - status 	hsct_ok      success
 *                      hsct_timeout USM response from remote side timed out
 *                      hsct_failed  failed
 * */
Hsct_return Hsct_isLinkActive(App_Hsct_instance *instance, uint32 timeout)
{
	/* Hsct not already intitialized */
	if (instance->hsct.hsct == 0)
		return hsct_failed;

	/* Reset last USM response received */
	instance->usm_zero_message_response_received = 0;

    /* Send zero message to trigger sending of zero message response from remote side ...*/
	IfxHssl_sendHsctUnsolicitedStatusMessage(instance->hsct.hsct, 0x0 /*zero*/);

	/* ... wait until zero message response is received or time out */
	while ( ((--timeout)>0) && (instance->usm_zero_message_response_received == 0 ));

	if (timeout==0)
		return hsct_failed;

	return hsct_ok;
}




#if 1
boolean Hsct_testLookback(App_Hsct_instance *instance, uint32 testNumber)
{
	boolean ret = TRUE;
	/* Interrupt triggered by received USM ? */
	if (IfxHssl_getHsctInterruptFlagStatus(instance->hsct.hsct, IfxHssl_Hsct_InterruptSource_UnsolicitedMessageReceived))
	{
		/* New USM received */
		IfxHssl_clearHsctInterruptFlag(instance->hsct.hsct, IfxHssl_Hsct_InterruptSource_UnsolicitedMessageReceived);
		/* Fetch received USM */
		uint32 receivedUSM = IfxHssl_getHsctUnsolicitedStatusMessage(instance->hsct.hsct);

		if(instance->interfaceMode == IfxHssl_InterfaceMode_slave)
		{
			instance->usm_zero_message_response_received = receivedUSM;
			instance->usm_zero_message_response = receivedUSM;
			HSCT_ALWAYS_PRINTF(("Loopback: rec 0x%x and respond 0x%x back\r\n",
								instance->usm_zero_message_response_received,
								instance->usm_zero_message_response));
			/* Respond with what ever we received */
			IfxHssl_sendHsctUnsolicitedStatusMessage(instance->hsct.hsct, instance->usm_zero_message_response);
		}
		else
		{
			instance->usm_zero_message_response_received = receivedUSM;
			if(instance->usm_zero_message_response_received == instance->usm_zero_message_response)
			{
				HSCT_ALWAYS_PRINTF(("Result match, Sent 0x%x, rec 0x%x\r\n",
						instance->usm_zero_message_response,
						instance->usm_zero_message_response_received));
				if(instance->usm_zero_message_response < testNumber)
				{
					instance->usm_zero_message_response++;
					waitTime(10*TimeConst[TIMER_INDEX_1MS]);
					IfxHssl_sendHsctUnsolicitedStatusMessage(instance->hsct.hsct, instance->usm_zero_message_response);
				}
			}
			else
			{
				HSCT_ALWAYS_PRINTF(("Result Mismatch! Sent 0x%x, rec 0x%x\r\n",
						instance->usm_zero_message_response,
						instance->usm_zero_message_response_received));
				HSCT_ALWAYS_PRINTF(("Test fails and stops! \r\n"));
				ret = FALSE;
			}
		}
	}

	if(instance->interfaceMode == IfxHssl_InterfaceMode_master)
	{
		if(instance->usm_zero_message_response_received == 0)
		{ /* First of the test, need to send the data from master to trigger the test */

			/* Align speed mode of master */
			Hsct_Master_ScanLinkSpeed( &(instance->hsct) );

			/* Reset last USM response received */
			instance->usm_zero_message_response_received = 0;

			instance->usm_zero_message_response = 1;

			/* Send zero message to trigger the sending of zero message response from remote side ...*/
			IfxHssl_sendHsctUnsolicitedStatusMessage(instance->hsct.hsct, instance->usm_zero_message_response /*zero*/);

			waitTime(1*TimeConst[TIMER_INDEX_1MS]);
		}
	}
	else
	{ /* Slave does not need to do anything to start test*/

	}
	return ret;
}
#endif
