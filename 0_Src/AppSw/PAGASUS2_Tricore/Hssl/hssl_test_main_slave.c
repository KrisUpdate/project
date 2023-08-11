/**
 * \file Cpu0_Main.c
 * \brief Main function definition for Cpu core 0 .
 *
 * \copyright Copyright (c) 2017 Infineon Technologies AG. All rights reserved.
 *
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
 *
 */
/* This is from Infineon HSSL demo */

#include "Configuration.h"
#include "internal_fw_debug.h"
#include "Emem/IfxEmem_init.h"
#include "HsslHsct/HsslHsct.h"

#define HSSL_SLAVE_ALWAYS_PRINTF(x) DEBUG_PRINTF_ALWAYS(x)

#define WAIT_TIME 1000

/* All function call IDs from slave to master device */
typedef enum
{
    fctcall_S2M_syncseqSLAVE_initialized,
    fctcall_S2M_start_sequ,
    fctcall_S2M_NCIB_transfer_finished,
    fctcall_S2M_DBFB_transfer_finished,
    fctcall_S2M_prep3rdFFT_transfer_finished,
    fctcall_S2M_3rdFFTDoA_transfer_finished,
    fctcall_S2M_syncseqSLAVE_finished,
} Hssl_fctcall_S2M_ID;

/* All function call IDs from master to slave device */
typedef enum
{
    fctcall_M2S_syncseqMASTER_initialized,
    fctcall_M2S_start_sequ,
    fctcall_M2S_start_NCIB_transfer,
    fctcall_M2S_start_DBFB_transfer,
    fctcall_M2S_start_prep3rdFFT_transfer,
    fctcall_M2S_start_3rdFFTDoA_transfer,
    fctcall_M2S_syncseqMASTER_finished,
} Hssl_fctcall_M2S_ID;

/* Processing states */
typedef enum
{
	state_reset_connection,
	state_start_sequ,
	state_process_1_2_3_4,
	state_process_5,
	state_idle,
	state_process_6,
	state_finalize_sequence,
} process_states;

/* Trigger variables */
volatile uint32 trigger_M2S_syncseqMASTER_initialized = 0;
volatile uint32 trigger_M2S_start_sequ = 0;
volatile uint32 trigger_M2S_start_NCIB_transfer = 0;
volatile uint32 trigger_M2S_start_DBFB_transfer = 0;
volatile uint32 trigger_M2S_start_prep3rdFFT_transfer = 0;
volatile uint32 trigger_M2S_start_3rdFFTDoA_transfer = 0;
volatile uint32 trigger_M2S_syncseqMASTER_finished = 0;

/* Callback functions for RFC */
void cb_M2S_syncseqMASTER_initialized(void){
	trigger_M2S_syncseqMASTER_initialized = 1;}
void cb_M2S_start_sequ(void){
	trigger_M2S_start_sequ = 1;}
void cb_M2S_start_NCIB_transfer(void){
	trigger_M2S_start_NCIB_transfer = 1;}
void cb_M2S_start_DBFB_transfer(void){
	trigger_M2S_start_DBFB_transfer = 1;}
void cb_M2S_start_prep3rdFFT_transfer(void){
	trigger_M2S_start_prep3rdFFT_transfer = 1;}
void cb_M2S_start_3rdFFTDoA_transfer(void){
	trigger_M2S_start_3rdFFTDoA_transfer = 1;}
void cb_M2S_syncseqMASTER_finished(void){
	trigger_M2S_syncseqMASTER_finished = 1;}

/* Reset all trigger variables */
#if 1
void M2S_reset_all_triggers (void)
{
	trigger_M2S_syncseqMASTER_initialized = 0;
	trigger_M2S_start_NCIB_transfer = 0;
	trigger_M2S_start_DBFB_transfer = 0;
	trigger_M2S_start_prep3rdFFT_transfer = 0;
	trigger_M2S_start_3rdFFTDoA_transfer = 0;
	trigger_M2S_syncseqMASTER_finished = 0;
}


/* Wait for trigger variable to be set including timeout */
HsctHssl_return M2S_wait_for_trigger (volatile uint32 *trigger, uint32 timeout)
{
    while ((timeout > 0 ) && (*trigger == 0) )
    {
    	timeout--;
    }

    if (*trigger == 0)
    	return hscthssl_timeout;
    else
    	return hscthssl_ok;
}

/* Start streaming on both instances in parallel */
HsctHssl_return M2S_streaming_both_instances (uint32 start_adr_hssl0, uint32 dest_adr_hssl0, uint32 frame_cnt_hssl0,
		                                  uint32 start_adr_hssl1, uint32 dest_adr_hssl1, uint32 frame_cnt_hssl1)
{
	/* First start streaming on instance 0 non-blocking */
	if ( HsctHssl_startstreaming (&hssl_instance_0, start_adr_hssl0, dest_adr_hssl0, frame_cnt_hssl0, FALSE, 1) != hscthssl_ok )
	{
		/* Exceptional streaming error not covered by BER < 10^-12 */
		return hscthssl_failed;
	}

	/* Streaming on instance 1 has finished with success,
	 * Wait for and check instance 0 now...
	 */
	if (HsctHssl_waitstreamingdone(&hssl_instance_0) != hscthssl_ok)
	{
		/* Streaming on instance 0 has finished with failure
		 * Two re-trials on instance 0 now. As instance 1 already has finished with success, this time we can start in blocking mode.
		 */
		if ( HsctHssl_startstreaming (&hssl_instance_0, start_adr_hssl0, dest_adr_hssl0, frame_cnt_hssl0, TRUE, 2) != hscthssl_ok )
		{
			/* Exceptional streaming error not covered by BER < 10^-12 */
			return hscthssl_failed;
		}
	}
	return hscthssl_ok;
}


void hssl_demo_main_slave(uint32 number)
{

	HSSL_SLAVE_ALWAYS_PRINTF(("Hssl Test Slave Start\r\n"));
	uint32 wait_loop;
	process_states process_state = state_reset_connection;

	{
		switch (process_state)
		{
			case state_reset_connection:
			{
				/* Reset all status triggers which are set from remote within callbacks */
				M2S_reset_all_triggers();

				/* trigger_M2S_start_sequence is handled separately from all other triggers,
				 * and reset before the sequence gets restarted inside state "state_finalize_sequence".
				 * For the initial startup it is also must be reset here.
				 */
				trigger_M2S_start_sequ = 0;

				/* Initialize HSCT/HSSL */
				HsctHssl_init(IfxHssl_InterfaceMode_slave);

				/* Run the loopback test */
				HsctLoopback_test(number);
#if RFC_TEST_ENABLE == 1
				/* register all callbacks for function calls from master side */
				HsctHssl_register_cb(&hssl_instance_0, fctcall_M2S_syncseqMASTER_initialized, cb_M2S_syncseqMASTER_initialized);
				HsctHssl_register_cb(&hssl_instance_0, fctcall_M2S_start_sequ, cb_M2S_start_sequ);
				HsctHssl_register_cb(&hssl_instance_0, fctcall_M2S_start_NCIB_transfer, cb_M2S_start_NCIB_transfer);
				HsctHssl_register_cb(&hssl_instance_0, fctcall_M2S_start_DBFB_transfer, cb_M2S_start_DBFB_transfer);
				HsctHssl_register_cb(&hssl_instance_0, fctcall_M2S_start_prep3rdFFT_transfer, cb_M2S_start_prep3rdFFT_transfer);
				HsctHssl_register_cb(&hssl_instance_0, fctcall_M2S_start_3rdFFTDoA_transfer, cb_M2S_start_3rdFFTDoA_transfer);
				HsctHssl_register_cb(&hssl_instance_0, fctcall_M2S_syncseqMASTER_finished, cb_M2S_syncseqMASTER_finished);

				HSSL_SLAVE_ALWAYS_PRINTF(("Slave: Waiting for signal from master\r\n"));
				/* wait for master signalling "initialized" */
				while(trigger_M2S_syncseqMASTER_initialized==0);

				HSSL_SLAVE_ALWAYS_PRINTF(("Slave: Receive signal from master\r\n"));
				/* respond to master. slave is also initialized */
				HsctHssl_fctcall(&hssl_instance_0,fctcall_S2M_syncseqSLAVE_initialized, 3);

				HSSL_SLAVE_ALWAYS_PRINTF(("Slave: Sent signal to master\r\n"));
#endif
				/* the demo will transfer next state,
				 *    process_state = state_start_sequ;
			    * but we will just use this state to finish the test. */
				process_state = state_start_sequ;
			}
			break;
			case state_start_sequ:
			{
				/* Heartbeat to indicate cyclic process is ongoing */
				IfxPort_togglePin(&MODULE_P13,0);

				/* Reset all triggers from remote */
				M2S_reset_all_triggers();

				/* Signal "sequence start" to master  */
				if ( HsctHssl_fctcall(&hssl_instance_0, fctcall_S2M_start_sequ, 3)!=hscthssl_ok)
				{
					/* Exceptional RFC error not covered by BER < 10^-12
					 * For recovery go back to state when starting from reset */
					process_state = state_reset_connection;
					break;
				}

				/* Wait for master to signal sequence ready to start */
				if (M2S_wait_for_trigger (&trigger_M2S_start_sequ, 0xffffff/*timeout*/) != hscthssl_ok)
				{
					/* Exception: master no more responsive
					 * For recovery go back to state when starting from reset */
					process_state = state_reset_connection;
					break;
				}

				/* transfer next state */
				process_state = state_process_1_2_3_4;
			}
			break;
			case state_process_1_2_3_4:
			{
				/* Dummy implementation: Slave processes 1 to 4 */
				for (wait_loop = 0; wait_loop<WAIT_TIME; wait_loop++);

				/* Wait for master to signal NCIB streaming transfer can start */
				if (M2S_wait_for_trigger (&trigger_M2S_start_NCIB_transfer, 0xffffff/*timeout*/) != hscthssl_ok)
				{
					/* Exception: master no more responsive
					 * For recovery go back to state when starting from reset */
					process_state = state_reset_connection;
					break;
				}

				/* Dummy transfer of 2MByte on both instances in parallel: NCIB data */
				if (M2S_streaming_both_instances (0xb9000000 /*src address instance 0*/,
											  0xb9100000 /*dest address instance 0*/, 32768 /*1MByte*/,
											  0xb9100000 /*src address instance 1*/,
											  0xb9200000 /*dest address instance 1*/, 32768 /*1MByte*/)
											  != hscthssl_ok )
				{
					/* Exceptional streaming error not covered by BER < 10^-12
					 * For recovery go back to state when starting from reset */
					process_state = state_reset_connection;
					break;
				}

				/* Signal NCIB transfer finished to master  */
				if (HsctHssl_fctcall(&hssl_instance_0, fctcall_S2M_NCIB_transfer_finished, 3)!=hscthssl_ok)
				{
					/* Exceptional RFC error not covered by BER < 10^-12
					 * For recovery go back to state when starting from reset */
					process_state = state_reset_connection;
					break;
				}

				/* transfer next state */
				process_state = state_process_5;
			}
			break;
			case state_process_5:
			{
				/* Dummy implementation: Slave processes 5 */
				for (wait_loop = 0; wait_loop<WAIT_TIME; wait_loop++);

				/* Wait for master to signal DBFB streaming transfer can start */
				if (M2S_wait_for_trigger (&trigger_M2S_start_DBFB_transfer, 0xffffff/*timeout*/) != hscthssl_ok)
				{
					/* Exception: master no more responsive
					 * For recovery go back to state when starting from reset */
					process_state = state_reset_connection;
					break;
				}

				/* transfer next state */
				process_state = state_idle;
			}
			break;
			case state_idle:
			{

				/* Dummy transfer of 2MByte on both instances in parallel: DBFB data */
				if (M2S_streaming_both_instances (0xb9000000 /*src address instance 0*/,
											  0xb9100000 /*dest address instance 0*/, 32768 /*1MByte*/,
											  0xb9100000 /*src address instance 1*/,
											  0xb9200000 /*dest address instance 1*/, 32768 /*1MByte*/) != hscthssl_ok )
				{
					/* Exceptional streaming error not covered by BER < 10^-12
					 * For recovery go back to state when starting from reset */
					process_state = state_reset_connection;
					break;
				}

				/* Signal DBFB transfer finished to master  */
				if (HsctHssl_fctcall(&hssl_instance_0, fctcall_S2M_DBFB_transfer_finished, 3)!=hscthssl_ok)
				{
					/* Exceptional RFC error not covered by BER < 10^-12
					 * For recovery go back to state when starting from reset */
					process_state = state_reset_connection;
					break;
				}

				/* Wait for master requesting prep3rdFFT data
				 * (implicitly signaling DBFB final data transfer has finished)
				 * */
				if (M2S_wait_for_trigger (&trigger_M2S_start_prep3rdFFT_transfer, 0xffffff/*timeout*/) != hscthssl_ok)
				{
					/* Exception: master no more responsive
					 * For recovery go back to state when starting from reset */
					process_state = state_reset_connection;
					break;
				}

				/* transfer next state */
				process_state = state_process_6;
			}
			break;
			case state_process_6:
			{
				/* Dummy transfer of 2MByte on both instances in parallel: prep3rdFFT data */
				if (M2S_streaming_both_instances (0xb9000000 /*src address instance 0*/,
											  0xb9100000 /*dest address instance 0*/, 32768 /*1MByte*/,
											  0xb9100000 /*src address instance 1*/,
											  0xb9200000 /*dest address instance 1*/, 32768 /*1MByte*/) != hscthssl_ok )
				{
					/* Exceptional RFC error not covered by BER < 10^-12
					 * For recovery go back to state when starting from reset */
					process_state = state_reset_connection;
					break;
				}

				/* Both channels finalized streaming successfully */
				/* Signal to master: transfer prep3rdFFT data finalized */
				if (HsctHssl_fctcall(&hssl_instance_0, fctcall_S2M_prep3rdFFT_transfer_finished, 3)!=hscthssl_ok)
				{
					/* Exceptional RFC error not covered by BER < 10^-12
					 * For recovery go back to state when starting from reset */
					process_state = state_reset_connection;
					break;
				}

				/* Dummy implementation: Slave processes 6 */
				for (wait_loop = 0; wait_loop<WAIT_TIME; wait_loop++);

				/* Wait for master requesting prep3rdFFT data
				 * (implicitly signaling DBFB final data transfer has finished)
				 * */
				if (M2S_wait_for_trigger (&trigger_M2S_start_3rdFFTDoA_transfer, 0xffffff/*timeout*/) != hscthssl_ok)
				{
					/* Exception: master no more responsive
					 * For recovery go back to state when starting from reset */
					process_state = state_reset_connection;
					break;
				}

				/* transfer next state */
				process_state = state_finalize_sequence;
			}
			break;
			case state_finalize_sequence:
			{
				/* Reset trigger variable to synchronize start of next sequence before finalizing this sequence*/
				trigger_M2S_start_sequ = 0;

				/* Dummy transfer of 2MByte on both instances in parallel: 3rdFFTDoA data */
				if (M2S_streaming_both_instances (0xb9000000 /*src address instance 0*/,
											  0xb9100000 /*dest address instance 0*/, 32768 /*1MByte*/,
											  0xb9100000 /*src address instance 1*/,
											  0xb9200000 /*dest address instance 1*/, 32768 /*1MByte*/) != hscthssl_ok )
				{
					/* Exceptional streaming error not covered by BER < 10^-12
					 * For recovery go back to state when starting from reset */
					process_state = state_reset_connection;
					break;
				}

				/* Signal to master: transfer 3rdFFTDoA data finalized */
				if (HsctHssl_fctcall(&hssl_instance_0, fctcall_S2M_3rdFFTDoA_transfer_finished, 3)!=hscthssl_ok)
				{
					/* Exceptional RFC error not covered by BER < 10^-12
					 * For recovery go back to state when starting from reset */
					process_state = state_reset_connection;
					break;
				}

				/* Slave signals to master: sequence finished; start next */
				if (HsctHssl_fctcall(&hssl_instance_0, fctcall_S2M_syncseqSLAVE_finished, 3)!=hscthssl_ok)
				{
					/* Exceptional RFC error not covered by BER < 10^-12
					 * For recovery go back to state when starting from reset */
					process_state = state_reset_connection;
					break;
				}

				/* Wait for master to signal sequence is finished */
				if (M2S_wait_for_trigger (&trigger_M2S_syncseqMASTER_finished, 0xffffff/*timeout*/) != hscthssl_ok)
				{
					/* Exception: master no more responsive
					 * For recovery go back to state when starting from reset */
					process_state = state_reset_connection;
					break;
				}

				/* Both channels finalized streaming successfully */
				/* transfer next state */
				process_state = state_start_sequ;
			}
			break;
		}
	}
}
#endif
