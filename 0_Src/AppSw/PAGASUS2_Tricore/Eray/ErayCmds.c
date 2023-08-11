/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "Configuration.h"
#include "IfxPort.h"
#include "Gpioexp.h"
#include "ErayNormalDemo.h"
#include "SysSe/Comm/Ifx_Shell.h"
#include "ErayCmds.h"
#include "Eray.h"
#include <gpioexp.h>
#include <string.h>
#include "qci_udp_packet.h"
#include "qci_udp_client.h"
#include "Ifx_Lwip.h"
#include "board.h"
#include "resource.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
volatile boolean eraystress_test = FALSE;
uint8 erayUdpMode = ERAY_NONE;
uint32 erayreceive_flag = ERAY_PRINT_ALWAYS;

extern Eray_run_mode g_Eray_run_mode;
extern Ifx_Lwip g_Lwip;
extern boolean set_ip_flag;
extern uint8 udp_host;
extern uint16 udpSeqNum;
extern uint32 udp_ip[4];
extern struct udp_pcb *gw_sendback_pcb;
extern IfxEray_Header headerSample1[NUM_OF_SLOT_MAX_S1];
extern IfxEray_Header headerSample2[NUM_OF_SLOT_MAX_S2];
extern IfxEray_SlotConfig slotSample1[NUM_OF_SLOT_MAX_S1];
extern IfxEray_SlotConfig slotSample2[NUM_OF_SLOT_MAX_S2];
extern rxheader_list_t rxheader_list[NUM_OF_SLOT_MAX_S2];
extern txheader_list_t txheader_list[NUM_OF_SLOT_MAX_S1];
extern uint8 g_eraynoslotsample1;
extern uint8 g_eraynoslotsample2;
extern eraytxdata_t g_eraytxdata[NUM_OF_SLOT_MAX_S1];
extern eray_app_state g_eray_app_state;
extern uint8 g_eraytx_num;
static boolean isFirstStartup[ERAY_SAMPLE_CONFIG] ={TRUE,TRUE};
static boolean isFirstTxConfig[ERAY_SAMPLE_CONFIG] ={TRUE,TRUE};
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
/** \brief Handle the 'erayinit' command
 *
 * \par Syntax
 *  - erayinit : initialize the eray modules
 */
boolean erayinit(pchar args, void *data, IfxStdIf_DPipe *io)
{
    boolean monitor = FALSE;
    boolean loopback = FALSE;
    uint32 frame_id = FRMAE_ID_INGORE;
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : erayinit t/r/lb [id x]"ENDL);
        IfxStdIf_DPipe_print(io, "           > Initialize the Eray modules in transmit/receive/loopback mode"ENDL);
        IfxStdIf_DPipe_print(io, "           > Optional: set frame id to x, if not set, use default frame id"ENDL);
    }
    else
    {
        if (Ifx_Shell_matchToken(&args, "t") != FALSE)
        {
            monitor = FALSE;
            loopback = FALSE;
        }
        else
        {
            if (Ifx_Shell_matchToken(&args, "r") != FALSE)
            {
                monitor = TRUE;
                loopback = FALSE;
                if (Ifx_Shell_parseUInt32(&args, &erayreceive_flag, FALSE) == TRUE)
               {
                    if (erayreceive_flag == ERAY_PRINT_ONCE)
                        IfxStdIf_DPipe_print(io, "USE erayreceive_flag"ENDL, args);
                    else
                    {
                        erayreceive_flag = ERAY_PRINT_ALWAYS;
                        IfxStdIf_DPipe_print(io, "Disable erayreceive_flag"ENDL, args);
                    }
                }
            }
            else
            {
                if (Ifx_Shell_matchToken(&args, "lb") != FALSE)
                {
                    monitor = FALSE;
                    loopback = TRUE;
                }
                else
                {
                    IfxStdIf_DPipe_print(io, "Syntaxerror : invalid mode %s"ENDL, args);
                    return TRUE;
                }
            }
        }
        if (Ifx_Shell_parseUInt32(&args, &frame_id, FALSE) == TRUE)
        {
            if((frame_id < FRAME_ID_MIN) || (frame_id > FRMAE_ID_MAX))
            {
                IfxStdIf_DPipe_print(io, "Invalid Frame ID 0x%x, will use default frame ID"ENDL, frame_id);
            }
        }
        else
        {
            frame_id = FRMAE_ID_INGORE;
        }
        erayUdpMode = ERAY_NONE;
        g_Eray_run_mode=ERAY_TEST_MODE;
        IfxErayTestMode_init(monitor, loopback, frame_id, IfxSrc_Tos_cpu0);
    }
    return TRUE;
}

/** \brief Handle the 'eraysend' command
 *
 * \par Syntax
 *  - eraysend : send data to eray
 */
boolean eraysend(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 datas[2];

	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : eraysend x y"ENDL);
        IfxStdIf_DPipe_print(io, "           > Send values x and y on Eray channel A and B"ENDL);
    }
    else
    {
        if (g_ErayTestMode.initialized == FALSE)
        {
       		IfxStdIf_DPipe_print(io, "Initialize Eray first with command erayinit t"ENDL);
       	    return TRUE;
        }
        if (IfxEray_getPocState(g_ErayTestMode.eray.eray) != 0xE)
        {
       		IfxStdIf_DPipe_print(io, "Initialize Eray first with command erayinit t"ENDL);
       	    return TRUE;
        }
    	// now we get the first data argument
      	if (Ifx_Shell_parseUInt32(&args, &datas[0], FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value x %s"ENDL, args);
            return TRUE;
      	}
    	// now we get the second data argument
      	if (Ifx_Shell_parseUInt32(&args, &datas[1], FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value y %s"ENDL, args);
            return TRUE;
      	}
       	send_eray_frame(&datas[0], 2);
    }

    return TRUE;
}

/** \brief Handle the 'eraytest' command
 *
 * \par Syntax
 *  - eraytest ab/ba: test the Eray A to B connected
 */
boolean eraytest(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 retval;

	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : eraytest ab/ba"ENDL);
        IfxStdIf_DPipe_print(io, "           > Test the Eray A to B or B to A connection"ENDL);
    }
    else
    {
        if (g_ErayTestMode.initialized == FALSE)
        {
       		IfxStdIf_DPipe_print(io, "Initialize Eray first with command erayinit lb"ENDL);
       	    return TRUE;
        }
        if (IfxEray_getPocState(g_ErayTestMode.eray.eray) != 0xD)
        {
       		IfxStdIf_DPipe_print(io, "Initialize Eray first with command erayinit lb"ENDL);
       	    return TRUE;
        }
        if ((Ifx_Shell_matchToken(&args, "ab") != FALSE) || (Ifx_Shell_matchToken(&args, "AB") != FALSE))
        {
           	retval = testeray(0);
        }
        else
        {
            if ((Ifx_Shell_matchToken(&args, "ba") != FALSE) || (Ifx_Shell_matchToken(&args, "BA") != FALSE))
            {
               	retval = testeray(1);
            }
            else
            {
            	IfxStdIf_DPipe_print(io, "Syntaxerror : invalid test mode %s"ENDL, args);
            	return TRUE;
            }
        }
       	if (retval)
       		IfxStdIf_DPipe_print(io, "Error occured - return value: %d"ENDL, retval);
       	else
       		IfxStdIf_DPipe_print(io, "Test Passed"ENDL);
    }

    return TRUE;
}

boolean eraytoudp(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 datas[2];

	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : eraytoudp m x y"ENDL);
        IfxStdIf_DPipe_print(io, "           > Set Aurix as master mode and send values x and y on Eray channel A and B"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax     : eraytoudp s"ENDL);
        IfxStdIf_DPipe_print(io, "           > Set Aurix as slave mode "ENDL);
    }
    else
    {
    	if (Ifx_Shell_matchToken(&args, "m") != FALSE)
		{
	    	if (g_Lwip.initialized != TRUE) {
					IfxStdIf_DPipe_print(io, "lwip doesn't init!"ENDL);
					return TRUE;
	    	}
	    	if (set_ip_flag == FALSE)
	    	{
					IfxStdIf_DPipe_print(io, "Set UDP server IP first with command setudpip"ENDL);
					return TRUE;
	    	}
	      	if (Ifx_Shell_parseUInt32(&args, &datas[0], FALSE) == FALSE)
	        {
	            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value x %s"ENDL, args);
	            return TRUE;
	      	}
	    	// now we get the second data argument
	      	if (Ifx_Shell_parseUInt32(&args, &datas[1], FALSE) == FALSE)
	        {
	            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value y %s"ENDL, args);
	            return TRUE;
	      	}
			IfxErayTestMode_init(FALSE, FALSE, FRMAE_ID_INGORE, IfxSrc_Tos_cpu0);
			erayUdpMode = ERAY_MASTER;
			send_eray_frame(&datas[0], 2);
			IfxErayTestMode_init(TRUE, FALSE, FRMAE_ID_INGORE, IfxSrc_Tos_cpu0);
		}
		else if (Ifx_Shell_matchToken(&args, "s") != FALSE)
		{
			IfxErayTestMode_init(TRUE, FALSE, FRMAE_ID_INGORE, IfxSrc_Tos_cpu0);
			erayUdpMode = ERAY_SLAVE;
		}
    }
    return TRUE;
}

/** \brief Handle the 'eraywakeup' command
 *
 * \par Syntax
 *  - eraywakeup : send flexray wake sequence from Flexray A
 */
boolean send_Eray_Wakeup(pchar args, void *data, IfxStdIf_DPipe *io)
{
	boolean stb_orig_state, enable_orig_state, bus_enable_orig_state;
	uint8 txd_orig_dir,tx_en_orig_dir;
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : eraywakeup"ENDL);
        IfxStdIf_DPipe_print(io, "           > Send FlexRay wake sequence from FlexRay A"ENDL);
    }
	else
	{
#ifdef FR_A_STB
		stb_orig_state = IfxPort_getPinState(FR_A_STB.port, FR_A_STB.pinIndex);
#endif
#ifdef FR_A_STB_N_EXP
		stb_orig_state = READ_GPIOEXP_STATE(FR_A_STB_N_EXP);
#endif
		enable_orig_state = IfxPort_getPinState(FR_A_EN.port, FR_A_EN.pinIndex);
#ifdef FLEXRAY_ENABLE
		bus_enable_orig_state = IfxPort_getPinState(FLEXRAY_ENABLE.port, FLEXRAY_ENABLE.pinIndex);
#endif
#ifdef FLEXRAY_ENABLE_N
		bus_enable_orig_state = IfxPort_getPinState(FLEXRAY_ENABLE_N.port, FLEXRAY_ENABLE_N.pinIndex);
#endif
		txd_orig_dir = IfxPort_getPinDirection(FR_A_TXD.pin.port, FR_A_TXD.pin.pinIndex);
		tx_en_orig_dir = IfxPort_getPinDirection(FR_A_TXEN.pin.port, FR_A_TXEN.pin.pinIndex);

		IfxPort_setPinHigh(FR_A_TXD.pin.port, FR_A_TXD.pin.pinIndex);
		IfxPort_setPinModeOutput(FR_A_TXD.pin.port, FR_A_TXD.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

	    IfxPort_setPinHigh(FR_A_EN.port, FR_A_EN.pinIndex);
#ifdef FR_A_STB
		IfxPort_setPinHigh(FR_A_STB.port, FR_A_STB.pinIndex);
#endif
#ifdef FR_A_STB_N_EXP
		SET_GPIOEXP_OUTPUT_HIGH(FR_A_STB_N_EXP);
		SET_GPIOEXP_CONFIG_OUTPUT(FR_A_STB_N_EXP);
#endif
#ifdef FLEXRAY_ENABLE
		IfxPort_setPinLow(FLEXRAY_ENABLE.port, FLEXRAY_ENABLE.pinIndex);
#endif
#ifdef FLEXRAY_ENABLE_N
		IfxPort_setPinLow(FLEXRAY_ENABLE_N.port, FLEXRAY_ENABLE_N.pinIndex);
#endif
		IfxPort_setPinLow(FR_A_TXEN.pin.port, FR_A_TXEN.pin.pinIndex);
		IfxPort_setPinModeOutput(FR_A_TXEN.pin.port, FR_A_TXEN.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
		waitTime(1*TimeConst[TIMER_INDEX_1MS]);

		IfxPort_setPinLow(FR_A_TXD.pin.port, FR_A_TXD.pin.pinIndex);
		waitTime(35*TimeConst[TIMER_INDEX_100NS]);
		IfxPort_setPinHigh(FR_A_TXD.pin.port, FR_A_TXD.pin.pinIndex);
		waitTime(35*TimeConst[TIMER_INDEX_100NS]);
		IfxPort_setPinLow(FR_A_TXD.pin.port, FR_A_TXD.pin.pinIndex);
		waitTime(35*TimeConst[TIMER_INDEX_100NS]);
		IfxPort_setPinHigh(FR_A_TXD.pin.port, FR_A_TXD.pin.pinIndex);
		waitTime(35*TimeConst[TIMER_INDEX_100NS]);

		IfxPort_setPinHigh(FR_A_TXEN.pin.port, FR_A_TXEN.pin.pinIndex);

		if (bus_enable_orig_state)
		{
#ifdef FLEXRAY_ENABLE
			IfxPort_setPinHigh(FLEXRAY_ENABLE.port, FLEXRAY_ENABLE.pinIndex);
#endif
#ifdef FLEXRAY_ENABLE_N
			IfxPort_setPinHigh(FLEXRAY_ENABLE_N.port, FLEXRAY_ENABLE_N.pinIndex);
#endif
		}

		if (!stb_orig_state)
		{
#ifdef FR_A_STB
			IfxPort_setPinLow(FR_A_STB.port, FR_A_STB.pinIndex);
#endif
#ifdef FR_A_STB_N_EXP
			SET_GPIOEXP_OUTPUT_LOW(FR_A_STB_N_EXP);
			SET_GPIOEXP_CONFIG_OUTPUT(FR_A_STB_N_EXP);
#endif
			waitTime(1*TimeConst[TIMER_INDEX_1MS]);
		}

		if (!enable_orig_state)
		{
			IfxPort_setPinLow(FR_A_EN.port, FR_A_EN.pinIndex);
		}

		if(txd_orig_dir == (IfxPort_OutputIdx_alt6 >>3) )  //it was initialized as FlexRay before
		{
			IfxPort_setPinModeOutput(FR_A_TXEN.pin.port, FR_A_TXEN.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_alt6);
			IfxPort_setPinModeOutput(FR_A_TXD.pin.port, FR_A_TXD.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_alt6);
		}
	}
}

/** \brief Handle the 'eraystress' command
 *
 * \par Syntax
 *  - eraystress : send FlexRay traffic continuously on FlexRayA and FlexRayB
 */
boolean eraystress(pchar args, void *data, IfxStdIf_DPipe *io)
{

	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : eraystress"ENDL);
        IfxStdIf_DPipe_print(io, "           > send FlexRay traffic continuously on FlexRayA and FlexRayB"ENDL);
    }
    else
    {
    	eraystress_test = TRUE;
    }
    return TRUE;
}

void eray_to_udp(uint8 *data, uint8 ch, uint16 length, uint8 *udpPacket)
{
	struct qci_udp_packet *g_ErayUdpPacket = (struct qci_udp_packet *)udpPacket;
	ip_addr_t hostIpAddr;

	// UDP packet format: Host => VIU
	g_ErayUdpPacket->header.packet_direct = toHOST;
	if (board.aurix_index == MAIN_AURIX)
		g_ErayUdpPacket->header.target_viu =AURIX1; //source	
	else g_ErayUdpPacket->header.target_viu =AURIX2;
	g_ErayUdpPacket->header.target_mcu = udp_host; // don't care in case: Host => VIU
	g_ErayUdpPacket->header.bus_type = typeERAY;
	g_ErayUdpPacket->info_len = QCI_UDP_INFO_LEN + length;
	g_ErayUdpPacket->info_code = 0; //should be 0s if this Packet is sent by VCU to VIU
	g_ErayUdpPacket->canid = 0;
	g_ErayUdpPacket->bus_id = ch;
	g_ErayUdpPacket->candlc = length; //bytes
	g_ErayUdpPacket->rs = 0;
	g_ErayUdpPacket->rs2 = udpSeqNum++;

	memcpy(g_ErayUdpPacket->payload, data, length);
	IP4_ADDR(&hostIpAddr, udp_ip[0], udp_ip[1], udp_ip[2], udp_ip[3]);
	udp_connect(gw_sendback_pcb, &hostIpAddr, QCI_UDP_SERVER_PORT);
	qci_udp_multi_send(gw_sendback_pcb, udpPacket, QCI_UDP_TOTAL_HEADER_LEN + length);
}

/** \brief Print a message from Eray if available
 *
 */
void Print_ErayTestModeMsg(IfxStdIf_DPipe *io)
{
    boolean recvA = FALSE, recvB = FALSE;
    // we return directly if the Eray is not initialized
    if (g_ErayTestMode.initialized == FALSE) 
        return;
    //    IfxStdIf_DPipe_print(io, "Print_ErayMsg "ENDL);
    if (g_ErayTestMode.ndat_A == TRUE)
    {
        /*print the data received in all communication cycles*/
        if(erayreceive_flag==ERAY_PRINT_ONCE){
            IfxStdIf_DPipe_print(io, "Eray data packet received on channel A: 0x%08x 0x%08x"ENDL, g_ErayTestMode.receivedData_A[0], g_ErayTestMode.receivedData_A[1]);
            erayreceive_flag = ERAY_PRINT_OFF;
            // print our prompt
#ifdef IFX_SHELL_PROMPT
           IfxStdIf_DPipe_print(io, IFX_SHELL_PROMPT);
#else
#ifdef IFX_CFG_SHELL_PROMPT
            IfxStdIf_DPipe_print(io, IFX_CFG_SHELL_PROMPT);
#endif
#endif
        }else if(erayreceive_flag==ERAY_PRINT_ALWAYS){
            IfxStdIf_DPipe_print(io, "Eray data packet received on channel A: 0x%08x 0x%08x"ENDL, g_ErayTestMode.receivedData_A[0], g_ErayTestMode.receivedData_A[1]);
#ifdef IFX_SHELL_PROMPT
            IfxStdIf_DPipe_print(io, IFX_SHELL_PROMPT);
#else
#ifdef IFX_CFG_SHELL_PROMPT
            IfxStdIf_DPipe_print(io, IFX_CFG_SHELL_PROMPT);
#endif
#endif
        }
        g_ErayTestMode.ndat_A = FALSE;
        if (g_ErayTestMode.errors)
        {
            IfxStdIf_DPipe_print(io, "ERROR: %d Errors occured during communication"ENDL, g_ErayTestMode.errors);
        }
        if (erayUdpMode != ERAY_NONE) recvA = TRUE;
    }

    if (g_ErayTestMode.ndat_B == TRUE)
    {
        IfxStdIf_DPipe_print(io, " "ENDL);
        /*print the data received in all communication cycles*/
        IfxStdIf_DPipe_print(io, "Eray data packet received on channel B: 0x%08x 0x%08x"ENDL, g_ErayTestMode.receivedData_B[0], g_ErayTestMode.receivedData_B[1]);
        g_ErayTestMode.ndat_B = FALSE;
        if (g_ErayTestMode.errors)
        {
            IfxStdIf_DPipe_print(io, "ERROR: %d Errors occured during communication"ENDL, g_ErayTestMode.errors);
        }
        if (erayUdpMode != ERAY_NONE) recvB = TRUE;
        // print our prompt
#ifdef IFX_SHELL_PROMPT
        IfxStdIf_DPipe_print(io, IFX_SHELL_PROMPT);
#else
#ifdef IFX_CFG_SHELL_PROMPT
        IfxStdIf_DPipe_print(io, IFX_CFG_SHELL_PROMPT);
#endif
#endif
    }

    if (recvA || recvB) {
        if (erayUdpMode == ERAY_MASTER) {
            uint8 udpPacket[QCI_UDP_TOTAL_HEADER_LEN + QCI_UDP_ERAY_PAYLOAD_SIZE];
            if (recvA && recvB) {
                boolean theSame = TRUE;
                for (int m = 0; m< 2; m++) {
                    if (g_ErayTestMode.receivedData_A[m] != g_ErayTestMode.receivedData_B[m]) {
                        theSame = FALSE;
                        break;
                    }
                }
                if (theSame) {
                    eray_to_udp((uint8 *)g_ErayTestMode.receivedData_A, ERAY_RECV_CHAB, 8, udpPacket);
                    recvA = recvB = FALSE;
                }
            }

            if (recvA)
                eray_to_udp((uint8 *)g_ErayTestMode.receivedData_A, ERAY_RECV_CHA, 8, udpPacket);
            if (recvB)
                eray_to_udp((uint8 *)g_ErayTestMode.receivedData_B, ERAY_RECV_CHB, 8, udpPacket);
        } 
        else if (erayUdpMode == ERAY_SLAVE)
        {
            IfxErayTestMode_init(FALSE, FALSE, FRMAE_ID_INGORE, IfxSrc_Tos_cpu0);
            if (recvA && recvB) {
                boolean theSame = TRUE;
                for (int m = 0; m< 2; m++) {
                    if (g_ErayTestMode.receivedData_A[m] != g_ErayTestMode.receivedData_B[m]) {
                        theSame = FALSE;
                        break;
                    }
                }
                if (theSame) {
                    send_eray_frame(g_ErayTestMode.receivedData_A, 2);
                    recvA = recvB = FALSE;
                }
            }

            if (recvA)
                send_eray_frame(g_ErayTestMode.receivedData_A, 2);
            if (recvB)
                send_eray_frame(g_ErayTestMode.receivedData_B, 2);
            IfxErayTestMode_init(TRUE, FALSE, FRMAE_ID_INGORE, IfxSrc_Tos_cpu0);
        }
    }
}

boolean AppShell_eray_test(pchar args, void *data, IfxStdIf_DPipe *io){
    uint32 var;
     if (Ifx_Shell_matchToken(&args, "s") != FALSE)
    {
        showErayState();
        return TRUE;
    }

    if (Ifx_Shell_parseUInt32(&args, &var, FALSE) == TRUE){
        showErayState();
        IfxStdIf_DPipe_print(io, "eray test variale is %d "ENDL, var);
        IfxEray_Eray_changePocState(&g_Eray, var);
        showErayState();
    }
}

boolean AppShell_eray_config(pchar args, void *data, IfxStdIf_DPipe *io){
    erayconfig_t erayconfig;
    memset(&erayconfig,0,sizeof(erayconfig_t));
    //IfxStdIf_DPipe_print(io, "REV:: %s"ENDL,args);
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax: erayconfig  s1/s2  t  frameid  cyclecode  data[0]  data[1]"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax: erayconfig  s1/s2  r  frameid  cyclecode  "ENDL);
        //IfxStdIf_DPipe_print(io, "Syntax: erayconfig  s1/s2  d  frameid  data[0]  data[1]"ENDL);
    }
    else
    {
        //erayconfig sample transmit/receive/set_t_data frameid cycle 
        //erayconfig  s1/s2            t/r                             3         3     

        if (Ifx_Shell_matchToken(&args, "s1") != FALSE)
        {
            //using headerSample1 as default slot setting 
            g_Eray_run_mode=ERAY_NORMAL_MODE_TEST_S1;
            //parserHeader(&headerSample1,&slotSample1,g_eraynoslotsample1);
        }else if (Ifx_Shell_matchToken(&args, "s2") != FALSE){
            //using headerSample2 as default slot setting
            g_Eray_run_mode=ERAY_NORMAL_MODE_TEST_S2;
            //parserHeader(&headerSample2,&slotSample2,g_eraynoslotsample2);
        }else if (Ifx_Shell_matchToken(&args, "l") != FALSE){
            uint32 data;
            if (Ifx_Shell_parseUInt32(&args, &data, FALSE) == TRUE){
                IfxStdIf_DPipe_print(io, "config payload length is %d bytes "ENDL,g_eraytx_num);
                //byte transfer two-byte num
                g_eraytx_num=(data+1)/2;
            }
            return TRUE;
        }else{
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value, only s1/s2 "ENDL);
            return TRUE;
        }

        if (Ifx_Shell_matchToken(&args, "t") != FALSE){
            erayconfig.bufferDirection = IfxEray_BufferDirection_transmit;
            //IfxStdIf_DPipe_print(io, "bufferDirection  is %d "ENDL,erayconfig.bufferDirection);
        }else if (Ifx_Shell_matchToken(&args, "r") != FALSE){
            erayconfig.bufferDirection = IfxEray_BufferDirection_receive;
            //IfxStdIf_DPipe_print(io, "bufferDirection  is %d "ENDL,erayconfig.bufferDirection);
        }else{
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value, only t/r or d"ENDL);
            return TRUE;
        }

        if (Ifx_Shell_parseUInt32(&args, &erayconfig.frameId, FALSE) == TRUE){
            //IfxStdIf_DPipe_print(io, "frame id is %d "ENDL,erayconfig.frameId);
        }
        if (Ifx_Shell_parseUInt32(&args, &erayconfig.cycle, FALSE) == TRUE){
            //IfxStdIf_DPipe_print(io, "cyclecode is %d "ENDL,erayconfig.cycle);
        }

        if(g_Eray_run_mode==ERAY_NORMAL_MODE_TEST_S1){
            int k;
            if(erayconfig.bufferDirection==IfxEray_BufferDirection_transmit&&isFirstTxConfig[ERAY_S1]){
                erayconfig.list_index=0;
                isFirstTxConfig[ERAY_S1] = FALSE;
                IfxStdIf_DPipe_print(io, "frame ID %d, define t(1)/r(0) %d and cycle count %d "ENDL,erayconfig.frameId,erayconfig.bufferDirection,erayconfig.cycle);
            }else{
                k=g_eraynoslotsample1-1;
                IfxStdIf_DPipe_print(io, "s1 slot number is %d "ENDL,g_eraynoslotsample1);
                for (int i=0; i<g_eraynoslotsample1; i++){
                    if(erayconfig.frameId==headerSample1[i].frameId){
                        erayconfig.list_index=i;
                        IfxStdIf_DPipe_print(io, "frame ID %d exist, define t(1)/r(0) %d and cycle count %d "ENDL,erayconfig.frameId,erayconfig.bufferDirection,erayconfig.cycle);
                        //IfxStdIf_DPipe_print(io, "found frame id is %d, defined and reconfig"ENDL,erayconfig.frameId);
                        break;
                    }
                    if(i==k){
                        if(g_eraynoslotsample1<NUM_OF_SLOT_MAX_S1){
                            erayconfig.list_index=g_eraynoslotsample1;
                            g_eraynoslotsample1=g_eraynoslotsample1+1;
                            IfxStdIf_DPipe_print(io, "frame ID %d, define t(1)/r(0) %d and cycle count %d "ENDL,erayconfig.frameId,erayconfig.bufferDirection,erayconfig.cycle);
                        }else{
                            IfxStdIf_DPipe_print(io, "sample1 use slot number is over maximun."ENDL);
                            return TRUE;
                        }
                    }
                }
            }
            headerSample1[erayconfig.list_index].bufferDirection=erayconfig.bufferDirection;
            headerSample1[erayconfig.list_index].frameId=erayconfig.frameId;
            headerSample1[erayconfig.list_index].cycleCode=erayconfig.cycle;
            headerSample1[erayconfig.list_index].channelAFiltered=TRUE;
            headerSample1[erayconfig.list_index].channelBFiltered=TRUE;
            headerSample1[erayconfig.list_index].transmitPayloadIndicatior=FALSE;
            headerSample1[erayconfig.list_index].transmissionMode=IfxEray_TransmissionMode_continuous;
            headerSample1[erayconfig.list_index].bufferServiceEnabled=TRUE;
            headerSample1[erayconfig.list_index].payloadLength=DATA_PAYLOAD_LENGTH;
            headerSample1[erayconfig.list_index].dataPointer=0x20+erayconfig.list_index*0x20;
            if(erayconfig.bufferDirection == IfxEray_BufferDirection_transmit&&isFirstStartup[ERAY_S1]==TRUE){
                headerSample1[erayconfig.list_index].startupFrameIndicator=TRUE;
                headerSample1[erayconfig.list_index].syncFrameIndicator=TRUE;
                isFirstStartup[ERAY_S1]=FALSE;
                IfxStdIf_DPipe_print(io, "config startup & sync."ENDL);
            }else{
                headerSample1[erayconfig.list_index].startupFrameIndicator=FALSE;
                headerSample1[erayconfig.list_index].syncFrameIndicator=FALSE;
            }
            slotSample1[erayconfig.list_index].dataTransfered=TRUE;
            slotSample1[erayconfig.list_index].headerTransfered=TRUE;
            slotSample1[erayconfig.list_index].transferRequested=TRUE;
            slotSample1[erayconfig.list_index].bufferIndex=erayconfig.list_index;
            g_eraytxdata[erayconfig.list_index].frameId=erayconfig.frameId;
        }else if(g_Eray_run_mode==ERAY_NORMAL_MODE_TEST_S2){
            int k;
            if(erayconfig.bufferDirection==IfxEray_BufferDirection_transmit&&isFirstTxConfig[ERAY_S2]){
                erayconfig.list_index=0;
                isFirstTxConfig[ERAY_S2] = FALSE;
                IfxStdIf_DPipe_print(io, "frame ID %d, define t(1)/r(0) %d and cycle count %d "ENDL,erayconfig.frameId,erayconfig.bufferDirection,erayconfig.cycle);
            }else{
                k=g_eraynoslotsample2-1;
                IfxStdIf_DPipe_print(io, "s2 slot number is %d "ENDL,g_eraynoslotsample2);
                for (int i=0; i<g_eraynoslotsample2; i++){
                    if(erayconfig.frameId==headerSample2[i].frameId){
                        erayconfig.list_index=i;
                        IfxStdIf_DPipe_print(io, "frame ID %d exist, define t(1)/r(0) %d, and cycle count %d "ENDL,erayconfig.frameId,erayconfig.bufferDirection,erayconfig.cycle);
                        //IfxStdIf_DPipe_print(io, "found frame id is %d, defined and reconfig"ENDL,erayconfig.frameId);
                        break;
                    }
                    if(i==k){
                        if(g_eraynoslotsample2<NUM_OF_SLOT_MAX_S2){
                            erayconfig.list_index=g_eraynoslotsample2;
                            g_eraynoslotsample2=g_eraynoslotsample2+1;
                            IfxStdIf_DPipe_print(io, "frame ID %d, define t(1)/r(0) %d and cycle count %d "ENDL,erayconfig.frameId,erayconfig.bufferDirection,erayconfig.cycle);
                        }else{
                            IfxStdIf_DPipe_print(io, "sample2 use slot number is over maximun."ENDL);
                            return TRUE;
                        }
                    }
                }
            }
            headerSample2[erayconfig.list_index].bufferDirection=erayconfig.bufferDirection;
            headerSample2[erayconfig.list_index].frameId=erayconfig.frameId;
            headerSample2[erayconfig.list_index].cycleCode=erayconfig.cycle;
            headerSample2[erayconfig.list_index].channelAFiltered=TRUE;
            headerSample2[erayconfig.list_index].channelBFiltered=TRUE;
            headerSample2[erayconfig.list_index].transmitPayloadIndicatior=FALSE;
            headerSample2[erayconfig.list_index].transmissionMode=IfxEray_TransmissionMode_continuous;
            headerSample2[erayconfig.list_index].bufferServiceEnabled=TRUE;
            headerSample2[erayconfig.list_index].payloadLength=DATA_PAYLOAD_LENGTH;
            headerSample2[erayconfig.list_index].dataPointer=0x20+erayconfig.list_index*0x20;
            if(erayconfig.bufferDirection == IfxEray_BufferDirection_transmit&&isFirstStartup[ERAY_S2]==TRUE){
                headerSample2[erayconfig.list_index].startupFrameIndicator=TRUE;
                headerSample2[erayconfig.list_index].syncFrameIndicator=TRUE;
                isFirstStartup[ERAY_S2]=FALSE;
                IfxStdIf_DPipe_print(io, "config startup & sync."ENDL);
            }else{
                headerSample2[erayconfig.list_index].startupFrameIndicator=FALSE;
                headerSample2[erayconfig.list_index].syncFrameIndicator=FALSE;
            }
            slotSample2[erayconfig.list_index].dataTransfered=TRUE;
            slotSample2[erayconfig.list_index].headerTransfered=TRUE;
            slotSample2[erayconfig.list_index].transferRequested=TRUE;
            slotSample2[erayconfig.list_index].bufferIndex=erayconfig.list_index;
            g_eraytxdata[erayconfig.list_index].frameId=erayconfig.frameId;
        }
    }
    return TRUE;
}

boolean AppShell_eray(pchar args, void *data, IfxStdIf_DPipe *io)
{
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        printf ("Syntax: eray s1/s2  ; // run eray table1/table2"ENDL);
        printf ("Syntax: eray default  ; // load eray default table1 and table2"ENDL);
        printf ("Syntax: eray show  ; // show eray table1 and table2"ENDL);
        printf ("Syntax: eray clear  ; // clear eray table1 and table2"ENDL);
        printf (""ENDL);
    }
    else
    {
            // enable transcevier
            IfxPort_setPinHigh(FR_A_EN.port, FR_A_EN.pinIndex);
#ifdef FR_A_STB
            IfxPort_setPinHigh(FR_A_STB.port, FR_A_STB.pinIndex);
#endif
#ifdef FR_A_STB_N_EXP
            SET_GPIOEXP_OUTPUT_HIGH(FR_A_STB_N_EXP);
            SET_GPIOEXP_CONFIG_OUTPUT(FR_A_STB_N_EXP);
#endif
            IfxPort_setPinHigh(FR_B_EN.port, FR_B_EN.pinIndex);
#ifdef FR_B_STB
            IfxPort_setPinHigh(FR_B_STB.port, FR_B_STB.pinIndex);
#endif
#ifdef FR_B_STB_N_EXP
            SET_GPIOEXP_OUTPUT_HIGH(FR_B_STB_N_EXP);
            SET_GPIOEXP_CONFIG_OUTPUT(FR_B_STB_N_EXP);
#endif
#ifdef FLEXRAY_ENABLE
            IfxPort_setPinLow(FLEXRAY_ENABLE.port, FLEXRAY_ENABLE.pinIndex);
#endif
#ifdef FLEXRAY_ENABLE_N
            IfxPort_setPinLow(FLEXRAY_ENABLE_N.port, FLEXRAY_ENABLE_N.pinIndex);
#endif
        if (Ifx_Shell_matchToken(&args, "s1") != FALSE)
        {
            g_Eray_run_mode=ERAY_NORMAL_MODE_TEST_S1;
            IfxEray_Init();
        }else if (Ifx_Shell_matchToken(&args, "s2") != FALSE)
        {
            g_Eray_run_mode=ERAY_NORMAL_MODE_TEST_S2;
            IfxEray_Init();
        }else if (Ifx_Shell_matchToken(&args, "default") != FALSE)
        {
            load_default_eray_config();
        }else if (Ifx_Shell_matchToken(&args, "show") != FALSE)
        {
            showErayTable();
        }else if (Ifx_Shell_matchToken(&args, "clear") != FALSE)
        {
            clearErayTable();
        }else{
            IfxStdIf_DPipe_print(io, "invalid error, please use eray ? to inquire "ENDL);
        }
        
    }
    return TRUE;
}

