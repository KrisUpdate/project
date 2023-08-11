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

#define HSSL_ALWAYS_PRINTF(x) DEBUG_PRINTF_ALWAYS(x)


#define DUMMY_WAIT_TIME 1000


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
	state_process_5_6_7,
	state_process_8,
	state_process_9,
	state_finalize_sequence,
} process_states;

/* Trigger variables */
volatile uint32 trigger_S2M_syncseqSLAVE_initialized = 0;
volatile uint32 trigger_S2M_start_sequ = 0;
volatile uint32 trigger_S2M_NCIB_transfer_finished = 0;
volatile uint32 trigger_S2M_DBFB_transfer_finished = 0;
volatile uint32 trigger_S2M_prep3rdFFT_transfer_finished = 0;
volatile uint32 trigger_S2M_3rdFFTDoA_transfer_finished = 0;
volatile uint32 trigger_S2M_syncseqSLAVE_finished = 0;

/* Callback functions for RFC */
void cb_S2M_syncseqSLAVE_initialized(void){
	trigger_S2M_syncseqSLAVE_initialized = 1;}
void cb_S2M_start_sequ(void){
	trigger_S2M_start_sequ = 1;}
void cb_S2M_NCIB_transfer_finished(void){
	trigger_S2M_NCIB_transfer_finished = 1;}
void cb_S2M_DBFB_transfer_finished(void){
	trigger_S2M_DBFB_transfer_finished = 1;}
void cb_S2M_prep3rdFFT_transfer_finished(void){
	trigger_S2M_prep3rdFFT_transfer_finished = 1;}
void cb_S2M_3rdFFTDoA_transfer_finished(void){
	trigger_S2M_3rdFFTDoA_transfer_finished = 1;}
void cb_S2M_syncseqSLAVE_finished(void){
	trigger_S2M_syncseqSLAVE_finished = 1;}

#if 1
/* Reset all trigger variables */
void S2M_reset_all_triggers (void)
{
	trigger_S2M_syncseqSLAVE_initialized = 0;
	trigger_S2M_NCIB_transfer_finished = 0;
	trigger_S2M_DBFB_transfer_finished = 0;
	trigger_S2M_prep3rdFFT_transfer_finished = 0;
	trigger_S2M_3rdFFTDoA_transfer_finished = 0;
	trigger_S2M_syncseqSLAVE_finished = 0;
}


/* Wait for trigger variable to be set including timeout */
HsctHssl_return S2M_wait_for_trigger (volatile uint32 *trigger, uint32 timeout)
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
HsctHssl_return S2M_streaming_both_instances (uint32 start_adr_hssl0, uint32 dest_adr_hssl0, uint32 frame_cnt_hssl0,
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


void hssl_demo_main_master(uint32 number)
{
	HSSL_ALWAYS_PRINTF(("Hssl Test Master Start\r\n"));
	uint32 wait_loop;
	process_states process_state = state_reset_connection;
	uint32 first_init = 0;

	{
		switch (process_state)
		{
			case state_reset_connection:
			{
				/* Reset all status triggers which are set from remote within callbacks */
				S2M_reset_all_triggers();

				/* Trigger_S2M_start_sequence is handled separately from all other triggers,
				 * to make sure it is reset before sequence is restarted inside state "state_finalize_sequence".
				 * For the initial startup it is also must be reset here.
				 */
				trigger_S2M_start_sequ = 0;

				/* Initialize HSCT/HSSL
				 * implicitly slave is resetted*/
				HsctHssl_init(IfxHssl_InterfaceMode_master);

				HsctLoopback_test(number);

#if RFC_TEST_ENABLE == 1
				/* register all callbacks for function calls from slave side */
				HsctHssl_register_cb(&hssl_instance_0, fctcall_S2M_syncseqSLAVE_initialized,cb_S2M_syncseqSLAVE_initialized);
				HsctHssl_register_cb(&hssl_instance_0, fctcall_S2M_start_sequ, cb_S2M_start_sequ);
				HsctHssl_register_cb(&hssl_instance_0, fctcall_S2M_NCIB_transfer_finished, cb_S2M_NCIB_transfer_finished);
				HsctHssl_register_cb(&hssl_instance_0, fctcall_S2M_DBFB_transfer_finished, cb_S2M_DBFB_transfer_finished);
				HsctHssl_register_cb(&hssl_instance_0, fctcall_S2M_prep3rdFFT_transfer_finished, cb_S2M_prep3rdFFT_transfer_finished);
				HsctHssl_register_cb(&hssl_instance_0, fctcall_S2M_3rdFFTDoA_transfer_finished, cb_S2M_3rdFFTDoA_transfer_finished);
				HsctHssl_register_cb(&hssl_instance_0, fctcall_S2M_syncseqSLAVE_finished, cb_S2M_syncseqSLAVE_finished);

				/* signal "master initialized" to slave until slave is also initialized */
				HSSL_ALWAYS_PRINTF(("Master: Sending signal to slave\r\n"));
				do
				{
					HsctHssl_fctcall(&hssl_instance_0, fctcall_M2S_syncseqMASTER_initialized, 10);
				} while (trigger_S2M_syncseqSLAVE_initialized == 0);

				HSSL_ALWAYS_PRINTF(("Master: Receive signal from slave\r\n"));
#endif
				/* the demo will transfer next state,
				 *   process_state = state_start_sequ;
				 * but we will just use this state to finish the test. */
			}
			break;
			case state_start_sequ:
			{
				/* Heartbeat to indicate cyclic process is ongoing */
				IfxPort_togglePin(&MODULE_P13,0);

				/* Reset all triggers from remote */
				S2M_reset_all_triggers();

				/* Signal "sequence start" to slave  */
				if (HsctHssl_fctcall(&hssl_instance_0, fctcall_M2S_start_sequ, 3)!=hscthssl_ok)
				{
					/* Exceptional RFC error not covered by BER < 10^-12
					 * For recovery go back to state when starting from reset */
					process_state = state_reset_connection;
					break;
				}

				/* Wait for slave to signal sequence ready to start */
				if (S2M_wait_for_trigger (&trigger_S2M_start_sequ, 0xffffff/*timeout*/) != hscthssl_ok)
				{
					/* Exception: Slave no more responsive
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
				/* Dummy implementation: Master processes 1 to 4 */
				for (wait_loop = 0; wait_loop < DUMMY_WAIT_TIME; wait_loop++);

				/* Master signals to slave: Ready to receive NCIB data */
				if (HsctHssl_fctcall(&hssl_instance_0, fctcall_M2S_start_NCIB_transfer, 3)!=hscthssl_ok)
				{
					/* Exceptional RFC error not covered by BER < 10^-12
					 * For recovery go back to state when starting from reset */
					process_state = state_reset_connection;
					break;
				}

				/* Wait for slave to signal transfer is finished */
				if (S2M_wait_for_trigger (&trigger_S2M_NCIB_transfer_finished, 0xffffff/*timeout*/) != hscthssl_ok)
				{
					/* Exception: Slave no more responsive
					 * For recovery go back to state when starting from reset */
					process_state = state_reset_connection;
					break;
				}
				/* transfer next state */
				process_state = state_process_5_6_7;

			}
			break;
			case state_process_5_6_7:
			{
				/* Dummy implementation: Master processes 5 to 7 */
				for (wait_loop = 0; wait_loop < DUMMY_WAIT_TIME; wait_loop++);

				/* Master signals slave: Ready to receive DBFB data */
				if (HsctHssl_fctcall(&hssl_instance_0, fctcall_M2S_start_DBFB_transfer, 3)!=hscthssl_ok)
				{
					/* Exceptional RFC error not covered by BER < 10^-12
					 * For recovery go back to state when starting from reset */
					process_state = state_reset_connection;
					break;
				}

				/* Wait for slave to signal transfer is finished */
				if (S2M_wait_for_trigger (&trigger_S2M_DBFB_transfer_finished, 0xffffff/*timeout*/) != hscthssl_ok)
				{
					/* Exception: Slave no more responsive
					 * For recovery go back to state when starting from reset */
					process_state = state_reset_connection;
					break;
				}

				/* transfer next state */
				process_state = state_process_8;
			}
			break;
			case state_process_8:
			{
				/* Dummy implementation: Master processes 8 */
				for (wait_loop = 0; wait_loop < DUMMY_WAIT_TIME; wait_loop++);

				/* Dummy transfer of 2MByte on both instances in parallel: DBFBfinal_data */
				if (S2M_streaming_both_instances (0xb9000000 /*src address instance 0*/,
											  0xb9100000 /*dest address instance 0*/, 32768 /*1MByte*/,
											  0xb9100000 /*src address instance 1*/,
											  0xb9200000 /*dest address instance 1*/, 32768 /*1MByte*/) != hscthssl_ok )
				{
					/* Exceptional streaming error not covered by BER < 10^-12
					 * For recovery go back to state when starting from reset */
					process_state = state_reset_connection;
					break;
				}
				/* Both channels finalized streaming successfully */
				/* Master signals slave: Transfer DBFBfinal finished, now prep3rdFFT can start */
				if (HsctHssl_fctcall(&hssl_instance_0, fctcall_M2S_start_prep3rdFFT_transfer, 3)!=hscthssl_ok)
				{
					/* Exceptional RFC error not covered by BER < 10^-12
					 * For recovery go back to state when starting from reset */
					process_state = state_reset_connection;
					break;
				}

				/* Wait for slave to finalize transfer of prep3rdFFT data */
				if (S2M_wait_for_trigger (&trigger_S2M_prep3rdFFT_transfer_finished, 0xffffff/*timeout*/) != hscthssl_ok)
				{
					/* Exception: Slave no more responsive
					 * For recovery go back to state when starting from reset */
					process_state = state_reset_connection;
					break;
				}
				/* transfer next state */
				process_state = state_process_9;
			}
			break;
			case state_process_9:
			{
				/* Dummy implementation: Master processes 9 */
				for (wait_loop = 0; wait_loop < DUMMY_WAIT_TIME; wait_loop++);

				/* Master signals slave: process 3rd FFT for DoA finished; request 3rdFFT for DoA data from slave */
				if (HsctHssl_fctcall(&hssl_instance_0, fctcall_M2S_start_3rdFFTDoA_transfer, 3)!=hscthssl_ok)
				{
					/* Exceptional RFC error not covered by BER < 10^-12
					 * For recovery go back to state when starting from reset */
					process_state = state_reset_connection;
					break;
				}
				/* Wait for slave to provide slave 3rdFFT data */
				if (S2M_wait_for_trigger (&trigger_S2M_3rdFFTDoA_transfer_finished, 0xffffff/*timeout*/) != hscthssl_ok)
				{
					/* Exception: Slave no more responsive
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
				trigger_S2M_start_sequ = 0;

				/* Dummy implementation: Master final process: Clustering, tracking */
				for (wait_loop = 0; wait_loop < DUMMY_WAIT_TIME; wait_loop++);

				/* Master signals to slave: sequence finished; start next */
				if (HsctHssl_fctcall(&hssl_instance_0, fctcall_M2S_syncseqMASTER_finished, 3)!=hscthssl_ok)
				{
					/* Exceptional RFC error not covered by BER < 10^-12
					 * For recovery go back to state when starting from reset */
					process_state = state_reset_connection;
					break;
				}

				/* Wait for slave to signal sequence is finished */
				if (S2M_wait_for_trigger (&trigger_S2M_syncseqSLAVE_finished, 0xffffff/*timeout*/) != hscthssl_ok)
				{
					/* Exception: Slave no more responsive
					 * For recovery go back to state when starting from reset */
					process_state = state_reset_connection;
					break;
				}

				/* transfer next state */
				process_state = state_start_sequ;
			}
			break;
		}
	}
}
#endif
