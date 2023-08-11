/**
 * \file Hssl.h
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
/* This hssl module implements an API to initialize and use the HSSL
 * communication module on AURIX 3G.
 * The implementation is not optimized in regard to performance
 * in favor of providing first users an initial starting point
 * for educational purpose.
 *
 * Both HSSL instances can be accessed.
 *
 * The API of this example implementations demonstrates three different
 * functionalities to you:
 *
 *            *** 1. Initialization ***
 * The initialization of the HSSL module is demonstrated.
 * To support also the other features of this example, user needs to provide
 * an address on which a 32 bit parameter can be received when RFC are received.
 * Furthermore an address within the address space of the remote device
 * needs to be provided which is used to set a 32 bit parameter when RFC is sent.
 *
 * Please see the API-description of the following function for details:
 * Hssl_initModule
 *
 *            *** 2. Remote function call ***
 * This functionality allows you to trigger a function call on the remote device.
 * It is demonstrated how a 32 bit parameter can be forwarded to the remote device
 * along with the RFC.
 *
 * For implementation this functionality the channel0 and channel1 of HSSL are
 * used internally and must not be used by the API for other purpose.
 *
 * Please see the API-description of the following functions for details:
 * Hssl_fctcall
 * Hssl_register_fctcall_cb
 *
 *            *** 3. request/response pairs ***
 * This function allows you to setup request/response pairs for the different
 * HSSL commands (WRITE/READ/READID/TRG).
 *
 * Please see the API-description of the following functions for details:
 * Hssl_frameReq
 * Hssl_getReadData
 * Hssl_waitchannelIdle
 *
 */

#ifndef IFXHSSL_H_
#define IFXHSSL_H_

/*******************************************************************************
**                             Includes                                       **
*******************************************************************************/
#include "Cpu/Std/IfxCpu.h"
#include "Cpu/Std/IfxCpu_Intrinsics.h"
#include "Src/Std/IfxSrc.h"
#include "IfxHssl_reg.h"
#include "IfxHssl_Hssl.h"

/*********************************************************************************
**                             Defines                                          **
*********************************************************************************/
#define HSSL_INSTANCE_COUNT 2

/*******************************************************************************
**                             Enumerations                                   **
*******************************************************************************/
typedef enum {
	Hssl_fctcall_idle,           /* RFC is idle, ready for next call */
	Hssl_fctcall_triggered,      /* RFC execution was triggered but not yet executed */
}Hssl_fctcall_state;

typedef enum {
	hssl_ok,                     /* generic success return code */
	hssl_failed,                 /* generic fail return code */
	hssl_timeout
}Hssl_return;


/*******************************************************************************
**                             Data Structures                                **
*******************************************************************************/
typedef struct
{
	IfxHssl_Hssl                hssl;						  /* HSSL module handle */
    uint32				        isr_base_prio_hssl;			  /* ISR base prio of this HSSL instance*/
    IfxHssl_Hssl_Channel        hsslChannel[4];				  /* HSSL channel handle */
    uint32				        *ptr_RFC_uint32_para_remote;  /* Address to set 32bit parameter data on remote side for RFC */
    uint32				        *ptr_RFC_uint32_para_local;   /* Address the remote side will use, to set 32bit parameter for RFC */
    volatile Hssl_fctcall_state fctcall_state_send;	   		  /* State of RFC */
    void				        (*cb_fctcall_uint32)(uint32); /* fct ptr to be called when RFC is triggered on this device */
} App_Hssl_instance;

/*******************************************************************************
**                             API Function prototypes                        **
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
 * */
Hssl_return Hssl_initModule(App_Hssl_instance *instance);

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
Hssl_return Hssl_frameReq(App_Hssl_instance *hssl_instance, IfxHssl_Hssl_FrameRequest frameRequest, IfxHssl_ChannelId ChId, IfxHssl_DataLength DatLen, uint32 Adr, uint32 Dat);

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
 * @return :  uint32 errorFlags: hscthssl_ok     - channel is idle; last request completed successfully
 *                               hscthssl_failed - channel is idle; last request had an error
 * */
Hssl_return Hssl_waitchannelIdle(App_Hssl_instance *hssl_instance, IfxHssl_ChannelId channelId);

/**
 * @Hssl_getReadData
 * Fetch received 32bit data of last read request from register.
 *
 * @input  :  - channel  : Pointer to HSSL channel data structure
 *
 * @return :  uint32 value received from last read request
 * */
uint32 Hssl_getReadData(IfxHssl_Hssl_Channel *channel);

/**
 * @Hssl_register_fctcall_cb
 * Register the callback which will be triggered by remote function calls of HSSL instance.
 *
 * @input  :  - hssl_instance  : Pointer to HSSL instance data structure
 *            - cb_fctptr      : Callback function pointer called for received remote function call
 * @return :  none
 * */
void Hssl_register_fctcall_cb(App_Hssl_instance *hssl_instance, void (*cb_fctptr)(uint32));

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
void Hssl_register_parameter_ptr(App_Hssl_instance *hssl_instance, uint32 ptr_uint32_local, uint32 ptr_uint32_remote);

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
Hssl_return Hssl_fctcall(App_Hssl_instance *hssl_instance, uint32 para);

#endif /* IFXHSSL_H_ */
