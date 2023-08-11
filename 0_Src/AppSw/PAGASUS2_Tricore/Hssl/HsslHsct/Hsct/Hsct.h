/**
 * \file Hsct.h
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

#ifndef IFXHSCT_H_
#define IFXHSCT_H_

/*******************************************************************************
**                             Description                                    **
*******************************************************************************/
/* This hsct module implements an API to initialize and use the HSCT
 * communication module on AURIX 3G.
 * The implementation is not optimized in regard to performance
 * in favor of providing first users an initial starting point
 * for educational purpose.
 *
 * Both HSCT instances can be accessed.
 *
 * The API of this example implementations demonstrates four different
 * functionalities to you:
 *
 *            *** 1. Initialization ***
 * The initialization of the HSCT module is demonstrated in master or slave mode.
 * A predefined non-zero USM response (which will be automatically responded to
 * every zero message within the HSCT-ISR) can be defined during initialization.
 *
 * Please see the API-description of the following function for details:
 * Hsct_initModule
 *
 *            *** 2. USM messaging ***
 * Unsolicited messaging is the only way to receive messages on the HSSL/HSCT
 * interface without memory being involved.
 * For this purpose you can define a non-zero response during initialization
 * which will automatically be responded to every zero-message received.
 * You can fetch the response of your remote device with the following function.
 * The same mechanism is used to check if the link is still active.
 * Despite ping is only available on devices in master role, this mechanism is
 * avaialble to devices in both roles: master/slave.
 *
 * Please see the API-description of the following functions for details:
 * Hsct_getUSMresponse
 * Hsct_isLinkActive
 *
 *            *** 3. Speed modes ***
 * HSCT can operate in different speed modes.
 * This API allows to set highspeed/lowspeed mode by master.
 * For convenience of usage, first master aligns to the current speed mode of
 * the slave device before switching he links to highspeed/lowspeed.
 *
 * Please see the API-description of the following functions for details:
 * Hsct_Master_HighSpeed
 * Hsct_Master_LowSpeed
 *
 */


/*******************************************************************************
**                                 Includes                                   **
*******************************************************************************/
#include "Cpu/Std/IfxCpu.h"
#include "Cpu/Std/IfxCpu_Intrinsics.h"
#include "Src/Std/IfxSrc.h"
#include "IfxHsct_reg.h"
#include "IfxScuWdt.h"
#include "IfxSrc_reg.h"
#include "ifxScuWdt.h"
#include "Port/Std/IfxPort.h"
#include "IfxScuWdt.h"
#include "_Impl/IfxHssl_cfg.h"
#include "Port/Std/IfxPort.h"
#include "Hssl.h"
#include "IfxHssl_Hssl.h"


/*********************************************************************************
**                             Defines                                          **
*********************************************************************************/
#define HSCT_INSTANCE_COUNT 2

/*******************************************************************************
**                             Enumerations                                   **
*********************************************************************************/
typedef enum {
	ls_slave_to_master  = 0x20,			/* lowspeed for RX_Link on slave side */
	ls_master_to_slave  = 0x08,			/* lowspeed for TX_Link on slave side */
	hs_slave_to_master  = 0x80,			/* highspeed for RX_Link on slave side */
	hs_master_to_slave  = 0x10,			/* highspeed for TX_Link on slave side */
	start_clk_mul_slave = 0x02,			/* start PLL on slave side */
	stop_clk_mul_slave  = 0x04,			/* stop PLL on slave side */
	enable_slave_tx     = 0x31,			/* enable slave transmitter */
	disable_slave_tx    = 0x32,			/* disable slave transmitter */
	send_ping           = 0x00			/* trigger ping */
}Hsct_Ifc;

typedef enum {
	tx_lowspeed  = 0,					/* lowspeed for TX_Link on master side */
	rx_lowspeed  = 1,					/* lowspeed for RX_Link on master side */
	tx_highspeed = 2,					/* highspeed for TX_Link on master side */
	rx_highspeed = 3					/* highspeed for RX_Link on master side */
}Hsct_LinkSpeed;

typedef enum {
	hsct_ok,							/* generic success return code */
	hsct_timeout,						/* generic fail (timeout) return code */
	hsct_failed							/* generic fail return code */
}Hsct_return;

/*********************************************************************************
**                           Data Structures                                    **
*********************************************************************************/
typedef struct
{
	IfxHssl_Hsct          hsct;									/* HSCT module handle */
	IfxHssl_InterfaceMode interfaceMode;						/* interface mode (master/slave) */
    uint32				  isr_base_prio_hsct;					/* ISR base prio of this HSCT instance */
    uint32				  usm_zero_message_response;			/* USM response this device sends on received USM zero messages */
    uint32				  usm_zero_message_response_received;	/* USM this device received from remote side on the last USM zero message sent */
    boolean               test_mode_enable;
} App_Hsct_instance;


/*********************************************************************************
**                             API Function prototypes                          **
*********************************************************************************/

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
 * */
Hsct_return Hsct_initModule(App_Hsct_instance *instance);

/**
 * @Hsct_getUSMresponse
 * Master only: Aligns speed mode of master to current slave speed mode until correct ping answer
 * can be received
 *
 * Sends a zero-value USM message and polls until answer is received.
 * Returns the received answer from remote side.
 *
 * @input  :  - instance  : Pointer to HSCT instance data structure
 *
 * @return :  - received USM message
 *              (zero of no connection to remote side could be established)
 * */
uint32 Hsct_getUSMresponse(App_Hsct_instance *instance);

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
 *                      hsct_timeout ping to slave side timed out for all speed modes
 *                      hsct_failed  failed
 * */
Hsct_return Hsct_Master_LowSpeed(App_Hsct_instance *instance);

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
Hsct_return Hsct_Master_HighSpeed(App_Hsct_instance *instance);

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
Hsct_return Hsct_isLinkActive(App_Hsct_instance *instance, uint32 timeout);


boolean Hsct_testLookback(App_Hsct_instance *instance, uint32 testNumber);

#endif

