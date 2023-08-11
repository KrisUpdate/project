/**
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
#include "SysSe/Comm/Ifx_Shell.h"
#include "MulticanCmds.h"
#include <Configuration.h>
#include "Mcmcan.h"
#include <gpioexp.h>
#include "board.h"
#include "Ifx_Lwip.h"
#include "cangateway.h"
#include "resource.h"
#include "Internal_fw_debug.h"



extern uint32 rxCnt[CAN_USED_NODES_NUM];
extern uint32 udp_count;
extern uint32 mess_count;
extern uint16 udpSeqNum;


#if MCM_CAN_TC39X_ENABLE == 1

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

App_Multican_CmdTest CmdTest;

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
static boolean canwaketest_a = FALSE;
static boolean canwaketest_b = FALSE;
static boolean canwaketest_c = FALSE;
static boolean canwaketest_d = FALSE;
static boolean canwaketest_e = FALSE;
static boolean canwaketest_f = FALSE;

static boolean isCanE_EchoEnable = FALSE;
static boolean isCanF_EchoEnable = FALSE;
extern uint8 canMode;
extern const uint32 FILTER_ID_F[];
boolean set_ip_flag = FALSE;
uint32 udp_ip[4] = {0, 0, 0, 0};
extern Ifx_Lwip g_Lwip;

extern IfxStdIf_DPipe *io_tmp;
static void can_echo_message(uint8 node, uint32 id, uint32 data0, uint32 data1);
static boolean cansend_conti_enable = FALSE;
static uint32 cansend_conti_node = 0;
#define INVALID_CAN_NODE      0xFF

CansetupConfig can_config[6];
volatile boolean show_Canbus_log;
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

static uint8 multican_get_node_input(pchar * args_ptr)
{
	uint8 node;
	// now we get the first argument (node number)
	if ((Ifx_Shell_matchToken(args_ptr, "a") != FALSE) || (Ifx_Shell_matchToken(args_ptr, "A") != FALSE))
	{

		node = 0;
	}
	else
	{
		if ((Ifx_Shell_matchToken(args_ptr, "b") != FALSE) || (Ifx_Shell_matchToken(args_ptr, "B") != FALSE))
		{
			node = 1;
		}
		else
		{
			if ((Ifx_Shell_matchToken(args_ptr, "c") != FALSE) || (Ifx_Shell_matchToken(args_ptr, "C") != FALSE))
			{
				node = 2;
			}
			else
			{
				if ((Ifx_Shell_matchToken(args_ptr, "d") != FALSE) || (Ifx_Shell_matchToken(args_ptr, "D") != FALSE))
				{
					node = 3;
				}
				else
				{
					if ((Ifx_Shell_matchToken(args_ptr, "e") != FALSE) || (Ifx_Shell_matchToken(args_ptr, "E") != FALSE))
					{
						node = 4;
					}
					else
					{
						if ((Ifx_Shell_matchToken(args_ptr, "f") != FALSE) || (Ifx_Shell_matchToken(args_ptr, "F") != FALSE))
						{
							node = 5;
						}
						else
						{
							node = INVALID_CAN_NODE;
						}
					}
				}
			}
		}
	}
	return node;
}

/** \brief Handle the 'caninit' command
 *
 * \par Syntax
 *  - caninit : initialize the multican modules and nodes
 */
boolean multicaninit(pchar args, void *data, IfxStdIf_DPipe *io)
{
	uint32 baudrate = 0, fast_baudrate = 0;
	uint32 samplepoint_a = 8000, samplepoint_b = 8000;
	uint32 tranceiverDelayOffset = 0;
	uint8 filterMode = CAN_BRAODCAST_FILTER;
	boolean useInterrupt;
	boolean isLoopback = FALSE;
	boolean isFdMode = FALSE;
	boolean isNonISO = FALSE;
    int i = 0;

	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        //IfxStdIf_DPipe_print(io, "Syntax     : caninit x [lb] [ext] [fd y] [samplepoint a b]"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax     : caninit x [lb] [fd y] [disbm] [samplepoint a b] [delay d] [noniso]"ENDL);
        IfxStdIf_DPipe_print(io, "           > Initialize the Multican modules and nodes with x baud [loopback]"ENDL);
        //IfxStdIf_DPipe_print(io, "           > If ext is given, then will enable extended Frame mode"ENDL);
        IfxStdIf_DPipe_print(io, "           > If fd is given, then CAN will be work in FD mode with BRS (Bit Rate Switch) enable"ENDL);
        IfxStdIf_DPipe_print(io, "           > In FD mode, arbitration phase is using x baudrate, data phase baudrate use y baudrate "ENDL);
        IfxStdIf_DPipe_print(io, "           > If disbm is given, then will disable broadcast mode"ENDL);
        IfxStdIf_DPipe_print(io, "           > If samplepoint is given, we will set the sample point a b"ENDL);
        IfxStdIf_DPipe_print(io, "           > Range = [0, 10000] resp. [0%, 100%) of the total bit time."ENDL);
        IfxStdIf_DPipe_print(io, "           > a is for standard package"ENDL);
        IfxStdIf_DPipe_print(io, "           > b is for data package if FD mode is enabled"ENDL);
        IfxStdIf_DPipe_print(io, "           > For 5M baudrate in FD mode, transceiver compensation delay is needed"ENDL);
        IfxStdIf_DPipe_print(io, "           > d is for compensation delay offset between 0~127"ENDL);
        IfxStdIf_DPipe_print(io, "           > If noniso is given, the can fd is initialized with non iso mode"ENDL);
    }
    else
    {
    	// now we get the first argument (baudrate)
    	if (Ifx_Shell_parseUInt32(&args, &baudrate, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid node %s"ENDL, args);
            return TRUE;
        }
#if CAN_INTERRUPT_ENABLE == 1
    	useInterrupt = TRUE;
#else
    	useInterrupt = FALSE;
#endif
    	/* analyze the options for different input order */
    	if (Ifx_Shell_matchToken(&args, "lb") != FALSE)
    	{
    		isLoopback = TRUE;
    		//if (Ifx_Shell_matchToken(&args, "ext") != FALSE)
    		//{
    		//	isExtFrame = TRUE;
    		//}
    	}
    	else if (Ifx_Shell_matchToken(&args, "ext") != FALSE)
    	{
    		//isExtFrame = TRUE;
    		if (Ifx_Shell_matchToken(&args, "lb") != FALSE)
    		{
    			isLoopback = TRUE;
    		}
    	}
    	else if (Ifx_Shell_matchToken(&args, "fd") != FALSE)
    	{
    		isFdMode = TRUE;
			if (Ifx_Shell_parseUInt32(&args, &fast_baudrate, FALSE) == FALSE)
			{
				IfxStdIf_DPipe_print(io, "Syntaxerror : invalid fast baudrate"ENDL);
				return TRUE;
			}
    	}

		if (Ifx_Shell_matchToken(&args, "disbm") != FALSE){
            filterMode = CAN_UNICAST_FILTER;
        }

        if (Ifx_Shell_matchToken(&args, "noniso") != FALSE){
            isNonISO = 1;
            IfxStdIf_DPipe_print(io, "noniso = %d"ENDL, isNonISO);
        }else{
            isNonISO = 0;
            IfxStdIf_DPipe_print(io, "noniso = %d"ENDL, isNonISO);
        }

        if (Ifx_Shell_matchToken(&args, "samplepoint") != FALSE)
        {
            if (Ifx_Shell_parseUInt32(&args, &samplepoint_a, FALSE) == FALSE)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid sample point for standard package"ENDL);
                return TRUE;
            }
            if (Ifx_Shell_parseUInt32(&args, &samplepoint_b, FALSE) == FALSE)
            {
                samplepoint_b = 8000;
            }
            IfxStdIf_DPipe_print(io, "Receive samplepoint parameter"ENDL);
        }

    	if (Ifx_Shell_matchToken(&args, "delay") != FALSE){
    		if (Ifx_Shell_parseUInt32(&args, &tranceiverDelayOffset, FALSE) == FALSE){
    			IfxStdIf_DPipe_print(io, "Syntaxerror : invalid delay for standard package"ENDL);
    			return TRUE;
    		}
    		if (tranceiverDelayOffset > 127){
    			IfxStdIf_DPipe_print(io, "Syntaxerror : invalid delay offset, the valid range is 0~127"ENDL);
    			return TRUE;
    		}
    		IfxStdIf_DPipe_print(io, "Receive delay parameter"ENDL);
    	}

    	//IfxStdIf_DPipe_print(io, "loopback %d, extframe: %d"ENDL, isLoopback, isExtFrame);
    	IfxMultican_SetFDMode(isFdMode, fast_baudrate);
    	IfxMultican_SetSamplePoint(samplepoint_a, samplepoint_b);
    	IfxMultican_SetTranceiverDelayOffset((uint8)tranceiverDelayOffset);
        IfxMultican_SetNonISOmode(isNonISO);
    	IfxMultican_init(isLoopback, baudrate, useInterrupt, filterMode);
        for (i=0;i<6;i++){
          can_config[i].classic_baudrate = baudrate;
          can_config[i].is_fd = isFdMode;
          can_config[i].fd_baudrate = fast_baudrate;
          can_config[i].delay = tranceiverDelayOffset;
          can_config[i].samplepoint_standard = samplepoint_a;
          can_config[i].samplepoint_fast = samplepoint_b;
          can_config[i].noniso = isNonISO;
		  can_config[i].is_id_ext = FALSE;
        }
        Multican_CmdTestInitial();
    }
    return TRUE;
}

extern uint32 UDP_send_bytes;
boolean show_cansetup(pchar args, void *data, IfxStdIf_DPipe *io)
{
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : showcansetup"ENDL);
        IfxStdIf_DPipe_print(io, "           > show cansetup status"ENDL);
    }
	else
    {
	    if (g_Multican.drivers.initialized == FALSE)
	    {
	        IfxStdIf_DPipe_print(io, "Initialize CAN first with command caninit"ENDL);
            return TRUE;
	    }
	    int i;
        if (can_config[i].is_fd){
            IfxStdIf_DPipe_print(io, "   Ch    ClassBR   FD_BR    ClassSP   FD_SP  FDdelay  NonISO TxEXT"ENDL);
        }else{
            IfxStdIf_DPipe_print(io, "   Ch    ClassBR   FD_BR    ClassSP   FD_SP  FDdelay  NonISO TxEXT"ENDL);  
        }
        for (i=0;i<6;i++){
            if (can_config[i].is_fd){
              IfxStdIf_DPipe_print(io, "%01d-CANFD",i+1);
              IfxStdIf_DPipe_print(io, ", %7d",can_config[i].classic_baudrate);
              IfxStdIf_DPipe_print(io, ", %7d",can_config[i].fd_baudrate);
              IfxStdIf_DPipe_print(io, ", %7d",can_config[i].samplepoint_standard);
              IfxStdIf_DPipe_print(io, ", %7d",can_config[i].samplepoint_fast);
              IfxStdIf_DPipe_print(io, ",  %3d",can_config[i].delay);
              IfxStdIf_DPipe_print(io, ",      %1d",can_config[i].noniso);
			  IfxStdIf_DPipe_print(io, ",    %1d"ENDL,can_config[i].is_id_ext);
            }else{
              IfxStdIf_DPipe_print(io, "%01d-Class",i+1);
              IfxStdIf_DPipe_print(io, ", %7d",can_config[i].classic_baudrate);
              IfxStdIf_DPipe_print(io, ",        ");
              IfxStdIf_DPipe_print(io, ", %7d",can_config[i].samplepoint_standard);
			  IfxStdIf_DPipe_print(io, ",        ");
			  IfxStdIf_DPipe_print(io, ",     ");
			  IfxStdIf_DPipe_print(io, ",       ");
			  IfxStdIf_DPipe_print(io, ",    %1d"ENDL,can_config[i].is_id_ext);
            }
        }
		if (Ifx_Shell_matchToken(&args, "idmode") != FALSE)
        {
            //IfxStdIf_DPipe_print(io, "Transmit setting 0(std)/1(ext) ID mode:"ENDL);
			//for (i=0;i<6;i++){
			//	  IfxStdIf_DPipe_print(io, "%01d-CANFD",i+1);
			//	  IfxStdIf_DPipe_print(io, ", %7d"ENDL,can_config[i].is_id_ext);
			//}
            
            IfxStdIf_DPipe_print(io, ENDL"Rx Standard ID Filter setting:"ENDL);
            IfxStdIf_DPipe_print(io, "   Ch      index_0_range      index_1_range "ENDL);
			for (i=0;i<6;i++){
				  IfxStdIf_DPipe_print(io, "    %01d",i+1);
				  IfxStdIf_DPipe_print(io, ", %7d",can_config[i].std_filter[0][0]);
				  IfxStdIf_DPipe_print(io, ", %7d",can_config[i].std_filter[0][1]);
				  IfxStdIf_DPipe_print(io, ", %7d",can_config[i].std_filter[1][0]);
				  IfxStdIf_DPipe_print(io, ", %7d"ENDL,can_config[i].std_filter[1][1]);
			}
            IfxStdIf_DPipe_print(io, ENDL"Rx Extended ID Filter setting:"ENDL);
            IfxStdIf_DPipe_print(io, "   Ch      index_0_range      index_1_range "ENDL);
			for (i=0;i<6;i++){
				  IfxStdIf_DPipe_print(io, "    %01d",i+1);
				  IfxStdIf_DPipe_print(io, ", %7d",can_config[i].ext_filter[0][0]);
				  IfxStdIf_DPipe_print(io, ", %7d",can_config[i].ext_filter[0][1]);
				  IfxStdIf_DPipe_print(io, ", %7d",can_config[i].ext_filter[1][0]);
				  IfxStdIf_DPipe_print(io, ", %7d"ENDL,can_config[i].ext_filter[1][1]);
			}

        }

    }
    IfxStdIf_DPipe_print(io, "receive: %d\r\n"ENDL,rxCnt[0]);
    IfxStdIf_DPipe_print(io, "receive1: %d\r\n"ENDL,rxCnt[1]);
    IfxStdIf_DPipe_print(io, "receive2: %d\r\n"ENDL,rxCnt[2]);
    IfxStdIf_DPipe_print(io, "receive3: %d\r\n"ENDL,rxCnt[3]);
    IfxStdIf_DPipe_print(io, "receive4: %d\r\n"ENDL,rxCnt[4]);
    IfxStdIf_DPipe_print(io, "receive5: %d\r\n"ENDL,rxCnt[5]);
    IfxStdIf_DPipe_print(io, "Receive_Udp_count: %d\r\n"ENDL,udp_count);
    IfxStdIf_DPipe_print(io, "Transfer_Udp_count: %d\r\n"ENDL,udpSeqNum);
    IfxStdIf_DPipe_print(io, "Transfer_Udp_bytes: %d\r\n"ENDL,UDP_send_bytes);
    IfxStdIf_DPipe_print(io, "message_count: %d\r\n"ENDL,mess_count);
    
	return TRUE;
}

boolean cansetup(pchar args, void *data, IfxStdIf_DPipe *io)
{
	uint32 node;
	uint32 baudrate = 0, fast_baudrate = 0;
	boolean useInterrupt;
	boolean isFdMode = FALSE;
	uint32 tranceiverDelayOffset = 0;
	uint32 samplepoint_a = 8000, samplepoint_b = 8000;
    boolean isNonISO = FALSE;
	boolean isext = FALSE;

	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : cansetup ch x [fd y][samplepoint a b][delay d][noniso][stdid/extid]"ENDL);
        IfxStdIf_DPipe_print(io, "           > Setup Multican node ch(a-f) with x baud"ENDL);
        IfxStdIf_DPipe_print(io, "           > If fd is given, then CAN will be work in FD mode with BRS (Bit Rate Switch) enable"ENDL);
        IfxStdIf_DPipe_print(io, "           > In FD mode, arbitration phase is using x baudrate, data phase baudrate use y baudrate "ENDL);
        IfxStdIf_DPipe_print(io, "           > If samplepoint is given, we will set the sample point a b"ENDL);
        IfxStdIf_DPipe_print(io, "           > a is for standard package"ENDL);
        IfxStdIf_DPipe_print(io, "           > b is for data package if FD mode is enabled"ENDL);
        IfxStdIf_DPipe_print(io, "           > For 5M baudrate in FD mode, transceiver compensation delay is needed"ENDL);
        IfxStdIf_DPipe_print(io, "           > d is for compensation delay offset between 0~127"ENDL);
        IfxStdIf_DPipe_print(io, "           > If noniso is given, the can fd is initialized with non iso mode"ENDL);
        IfxStdIf_DPipe_print(io, "           > If extid is given, the can id is initialized with extened id mode"ENDL);
    }
	else
    {
	    if (g_Multican.drivers.initialized == FALSE)
	    {
	        IfxStdIf_DPipe_print(io, "Initialize CAN first with command caninit"ENDL);
            return TRUE;
	    }
	    // now we get the first argument (node number)
	    node = multican_get_node_input(&args);
	    if(node == INVALID_CAN_NODE)
	    {
	      IfxStdIf_DPipe_print(io, "Syntaxerror : invalid node"ENDL);
	      return TRUE;
	    }
        if (Ifx_Shell_parseUInt32(&args, &baudrate, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid baudrate %s"ENDL, args);
            return TRUE;
        }
        if (Ifx_Shell_matchToken(&args, "fd") != FALSE)
        {
            isFdMode = TRUE;
			if (Ifx_Shell_parseUInt32(&args, &fast_baudrate, FALSE) == FALSE)
			{
				IfxStdIf_DPipe_print(io, "Syntaxerror : invalid fast baudrate"ENDL);
				return TRUE;
			}
        }
        if (Ifx_Shell_matchToken(&args, "samplepoint") != FALSE)
        {
            if (Ifx_Shell_parseUInt32(&args, &samplepoint_a, FALSE) == FALSE)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid sample point for standard package"ENDL);
                return TRUE;
            }
            if (Ifx_Shell_parseUInt32(&args, &samplepoint_b, FALSE) == FALSE)
            {
                samplepoint_b = 8000;
            }
            IfxStdIf_DPipe_print(io, "Receive samplepoint parameter"ENDL);
        }
        if (Ifx_Shell_matchToken(&args, "delay") != FALSE)
        {
          if (Ifx_Shell_parseUInt32(&args, &tranceiverDelayOffset, FALSE) == FALSE){
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid delay for standard package"ENDL);
            return TRUE;
          }
          if (tranceiverDelayOffset > 127){
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid delay offset, the valid range is 0~127"ENDL);
            return TRUE;
          }
        }

        if (Ifx_Shell_matchToken(&args, "noniso") != FALSE){
            isNonISO = 1;
            IfxStdIf_DPipe_print(io, "noniso = %d"ENDL, isNonISO);
        }else{
            isNonISO = 0;
            IfxStdIf_DPipe_print(io, "noniso = %d"ENDL, isNonISO);
        }

        if (Ifx_Shell_matchToken(&args, "stdid") != FALSE){
	        isext = FALSE;
	        IfxStdIf_DPipe_print(io, "set standard id mode "ENDL);
        }else if (Ifx_Shell_matchToken(&args, "extid") != FALSE){
	        isext = TRUE;
	        IfxStdIf_DPipe_print(io, "set extended id mode"ENDL);
        }

#if CAN_INTERRUPT_ENABLE == 1
        useInterrupt = TRUE;
#else
        useInterrupt = FALSE;
#endif
      if (useInterrupt)
      {
        can_config[node].classic_baudrate = baudrate;
        can_config[node].is_fd = isFdMode;
        can_config[node].fd_baudrate = fast_baudrate;
        can_config[node].delay = tranceiverDelayOffset;
        can_config[node].samplepoint_standard = samplepoint_a;
        can_config[node].samplepoint_fast = samplepoint_b;
        can_config[node].noniso = isNonISO;
		can_config[node].is_id_ext = isext;
        IfxMultican_SetNonISOmode(isNonISO);
        IfxMultican_initNode_with_interrupt(node, baudrate, isFdMode, fast_baudrate, tranceiverDelayOffset, samplepoint_a, samplepoint_b);
      }else{
        IfxStdIf_DPipe_print(io, "Syntaxerror : not support without interrupt mode!"ENDL);
      }
    }
	return TRUE;
}

/** \brief Handle the 'cansend' command
 *
 * \par Syntax
 *  - cansend x id a b : aend a and b on Multican node x with id
 */
boolean multicansend(pchar args, void *data, IfxStdIf_DPipe *io)  //ycnew
{
    uint32 node, id, dataLow, dataHigh;
    boolean rtr_frame = FALSE;
    boolean b_extid = FALSE;/* [EXTENED ID test] */

	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : cansend x id a b"ENDL);
        IfxStdIf_DPipe_print(io, "           > Send values a and b on Multican node x with id"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax     : cansend x id r"ENDL);
        IfxStdIf_DPipe_print(io, "           > Send RTR frame on Multican node x with id"ENDL);
    }
    else
    {
        // we return directly if the MultiCan is not initialized
        if (g_Multican.drivers.initialized == FALSE)
        {
       		IfxStdIf_DPipe_print(io, "Initialize CAN first with command caninit"ENDL);
       	    return TRUE;
        }
    	// now we get the first argument (node number)
        node = multican_get_node_input(&args);
        if(node == INVALID_CAN_NODE)
        {
        	IfxStdIf_DPipe_print(io, "Syntaxerror : invalid node"ENDL);
        	return TRUE;
        }

    	if (Ifx_Shell_parseUInt32(&args, &id, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid id %s"ENDL, args);
            return TRUE;
        }
		if ((Ifx_Shell_matchToken(&args, "r") != FALSE) || (Ifx_Shell_matchToken(&args, "R") != FALSE))
		{
			rtr_frame = TRUE;
			dataLow = 0;
			dataHigh = 0;
		}
		else
		{
			rtr_frame = FALSE;
			if (Ifx_Shell_parseUInt32(&args, &dataLow, FALSE) == FALSE)
			{
				IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value %s"ENDL, args);
				return TRUE;
			}
			if (Ifx_Shell_parseUInt32(&args, &dataHigh, FALSE) == FALSE)
			{
				IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value %s"ENDL, args);
				return TRUE;
			}
		}
		/* [EXTENED ID test] start */
//		if ( Ifx_Shell_matchToken(&args, "ext") != FALSE )
//		{
//#if CAN_EXT_ID_ENABLED == 1
//			b_extid = TRUE;
//#else
//			IfxStdIf_DPipe_print(io, "Syntaxerror : This AURIX version did not support extended id mode."ENDL);
//#endif
//		}
		/* [EXTENED ID test] end */
		canMode = CAN_NORMAL_MODE;
		send_multican_message(node, id, dataLow, dataHigh, rtr_frame);
    }

    return TRUE;
}

/** \brief Handle the 'canprintrx' command
 *
 * \par Syntax
 *  - canprintrx : set CANBUS RX log flag
 */
boolean set_canbus_rx_show(pchar args, void *data, IfxStdIf_DPipe *io)
{
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : canprintrx enable/disable"ENDL);
        IfxStdIf_DPipe_print(io, "           > Show CANBUS RX message"ENDL);
    }
    else
    {
        if (Ifx_Shell_matchToken(&args, "enable") != FALSE)
        {
            queue_dequeue_all_print_flag = TRUE;
        }
        else if (Ifx_Shell_matchToken(&args, "disable") != FALSE)
        {
            queue_dequeue_all_print_flag = FALSE;
        }
        else
        {
            IfxStdIf_DPipe_print(io, "invalid parameter."ENDL);
        }
    }
    return TRUE;
}


/** \brief Handle the 'canlp' command
 *
 * \par Syntax
 *  - canlp a/b/c/d/e/f : put can node a/b/c/d/e/f into low power mode
 */
boolean canlp(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 node;

	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : canlp a/b/c/d/e/f"ENDL);
        IfxStdIf_DPipe_print(io, "           > Put can node a/b/c/d/e/f into low power mode"ENDL);
        IfxStdIf_DPipe_print(io, "           > can node a/b/c/d/e/f maybe different based on projects"ENDL);
    }
    else
    {
        // we return directly if the MultiCan is not initialized
        if (g_Multican.drivers.initialized == FALSE)
        {
       		IfxStdIf_DPipe_print(io, "Initialize CAN first with command caninit"ENDL);
       	    return TRUE;
        }

        // now we get the first argument (node number)
		node = multican_get_node_input(&args);
		if(node == INVALID_CAN_NODE)
		{
			IfxStdIf_DPipe_print(io, "Syntaxerror : invalid node"ENDL);
			return TRUE;
		}
        can_xceiver_lp(node);
    }

    return TRUE;
}

/**
 *
 * \par Syntax
 *  - canwaketest a/b/c/d/e/f on/off: repeatedly put can node in low power and display wakeup if received
 */

boolean CANWakeTest(pchar args, void *data, IfxStdIf_DPipe *io)
{
	uint32 node;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : canwaketest a/b/c/d/e/f/off"ENDL);
        IfxStdIf_DPipe_print(io, "           > a/b/c/d/e/f:  repeatedly put can node in low power and display wakeup if received"ENDL);
        IfxStdIf_DPipe_print(io, "           > can node a/b/c/d/e/f maybe different based on projects"ENDL);
        IfxStdIf_DPipe_print(io, "           > off:  disable the test"ENDL);
    }
    else
    {
        // we return directly if the MultiCan is not initialized
        if (g_Multican.drivers.initialized == FALSE)
        {
       		IfxStdIf_DPipe_print(io, "Initialize CAN first with command caninit"ENDL);
       	    return TRUE;
        }
    	// now we get the first argument (node number)
        if ((Ifx_Shell_matchToken(&args, "a") != FALSE) || (Ifx_Shell_matchToken(&args, "A") != FALSE))
        {
        	canwaketest_a = TRUE;
        	node = 0;
        }
        else
        {
            if ((Ifx_Shell_matchToken(&args, "b") != FALSE) || (Ifx_Shell_matchToken(&args, "B") != FALSE))
            {

            	canwaketest_b = TRUE;
            	node = 1;
            }
            else
            {
                if ((Ifx_Shell_matchToken(&args, "c") != FALSE) || (Ifx_Shell_matchToken(&args, "C") != FALSE))
                {
                	canwaketest_c = TRUE;
                	node = 2;
                }
                else
                {
                    if ((Ifx_Shell_matchToken(&args, "d") != FALSE) || (Ifx_Shell_matchToken(&args, "D") != FALSE))
                    {
                    	canwaketest_d = TRUE;
                    	node = 3;
                    }
                    else
                    {
                        if ((Ifx_Shell_matchToken(&args, "e") != FALSE) || (Ifx_Shell_matchToken(&args, "E") != FALSE))
                        {
                        	canwaketest_e = TRUE;
                        	node = 4;
                        }
                        else
                        {
                            if ((Ifx_Shell_matchToken(&args, "f") != FALSE) || (Ifx_Shell_matchToken(&args, "F") != FALSE))
                            {
                            	canwaketest_f = TRUE;
                            	node = 5;
                            }
                            else
                            {
                            	if ((Ifx_Shell_matchToken(&args, "off") != FALSE) || (Ifx_Shell_matchToken(&args, "OFF") != FALSE))
                            	{
                            		canwaketest_a = FALSE;
                            		canwaketest_b = FALSE;
                            		canwaketest_c = FALSE;
                            		canwaketest_d = FALSE;
                            		canwaketest_e = FALSE;
                            		canwaketest_f = FALSE;
                            		can_xceiver_normal(0);
                            		can_xceiver_normal(1);
                            		can_xceiver_normal(2);
                            		can_xceiver_normal(3);
                            		can_xceiver_normal(4);
                            		can_xceiver_normal(5);
                            		return TRUE;
                            	}
                            	else
                            	{
                            		IfxStdIf_DPipe_print(io, "Syntaxerror : invalid node/selection %s"ENDL, args);
                            		return TRUE;
                            	}
                            }
                        }
                    }
                }
            }
        }
    }
    can_xceiver_lp(node);
    return TRUE;
}


/** \brief Handle the 'canwakeup' command
 *
 * \par Syntax
 *  - canwakeup : send can wake sequence from CAN A
 */
boolean send_CAN_Wakeup(pchar args, void *data, IfxStdIf_DPipe *io)
{
	boolean stb_orig_state, enable_orig_state;
	uint8 txd_orig_dir;
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : canwakeup"ENDL);
        IfxStdIf_DPipe_print(io, "           > Send can wake sequence from CAN A"ENDL);
    }
	else
	{
#ifdef CAN_PHY1_STB
		stb_orig_state = IfxPort_getPinState(CAN_PHY1_STB.port, CAN_PHY1_STB.pinIndex);
#endif
#ifdef CAN_PHY1_STB_N_EXP
		stb_orig_state = READ_GPIOEXP_STATE(CAN_PHY1_STB_N_EXP);
#endif
#ifdef CAN_PHY1_EN
		enable_orig_state = IfxPort_getPinState(CAN_PHY1_EN.port, CAN_PHY1_EN.pinIndex);
#endif
#ifdef CAN_PHY1_EN_EXP
		enable_orig_state = READ_GPIOEXP_STATE(CAN_PHY1_EN_EXP);
#endif
		txd_orig_dir = IfxPort_getPinDirection(CAN_PHY1_TXD.pin.port, CAN_PHY1_TXD.pin.pinIndex);

		IfxPort_setPinHigh(CAN_PHY1_TXD.pin.port, CAN_PHY1_TXD.pin.pinIndex);
		IfxPort_setPinModeOutput(CAN_PHY1_TXD.pin.port, CAN_PHY1_TXD.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

		can_enable_phy(PHY1);
		waitTime(1*TimeConst[TIMER_INDEX_1MS]);

		waitTime(35*TimeConst[TIMER_INDEX_100NS]);
		IfxPort_setPinLow(CAN_PHY1_TXD.pin.port, CAN_PHY1_TXD.pin.pinIndex);
		waitTime(35*TimeConst[TIMER_INDEX_100NS]);
		IfxPort_setPinHigh(CAN_PHY1_TXD.pin.port, CAN_PHY1_TXD.pin.pinIndex);
		waitTime(35*TimeConst[TIMER_INDEX_100NS]);
		IfxPort_setPinLow(CAN_PHY1_TXD.pin.port, CAN_PHY1_TXD.pin.pinIndex);
		waitTime(35*TimeConst[TIMER_INDEX_100NS]);
		IfxPort_setPinHigh(CAN_PHY1_TXD.pin.port, CAN_PHY1_TXD.pin.pinIndex);
		waitTime(35*TimeConst[TIMER_INDEX_100NS]);

		if (!stb_orig_state)
		{
#ifdef CAN_PHY1_STB
			IfxPort_setPinLow(CAN_PHY1_STB.port, CAN_PHY1_STB.pinIndex);
#endif
#ifdef CAN_PHY1_STB_N_EXP
			SET_GPIOEXP_OUTPUT_LOW(CAN_PHY1_STB_N_EXP);
			SET_GPIOEXP_CONFIG_OUTPUT(CAN_PHY1_STB_N_EXP);
#endif
			waitTime(1*TimeConst[TIMER_INDEX_1MS]);
		}

		if (!enable_orig_state)
		{
#ifdef CAN_PHY1_EN
			IfxPort_setPinLow(CAN_PHY1_EN.port, CAN_PHY1_EN.pinIndex);
#endif
#ifdef CAN_PHY1_EN_EXP
			SET_GPIOEXP_OUTPUT_LOW(CAN_PHY1_EN_EXP);
			SET_GPIOEXP_CONFIG_OUTPUT(CAN_PHY1_EN_EXP);
#endif
		}

		if(txd_orig_dir == (IfxPort_OutputIdx_alt5 >>3) )  //it was initialized as can before
		{
#ifdef CAN_PHY1_STB
			IfxPort_setPinLow(CAN_PHY1_STB.port, CAN_PHY1_STB.pinIndex);
#endif
#ifdef CAN_PHY1_STB_N_EXP
			SET_GPIOEXP_OUTPUT_LOW(CAN_PHY1_STB_N_EXP);
			SET_GPIOEXP_CONFIG_OUTPUT(CAN_PHY1_STB_N_EXP);
#endif
			waitTime(1*TimeConst[TIMER_INDEX_1MS]);
			IfxPort_setPinModeOutput(CAN_PHY1_TXD.pin.port, CAN_PHY1_TXD.pin.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_alt5);
			waitTime(1*TimeConst[TIMER_INDEX_1MS]);
#ifdef CAN_PHY1_STB
			IfxPort_setPinHigh(CAN_PHY1_STB.port, CAN_PHY1_STB.pinIndex);
#endif
#ifdef CAN_PHY1_STB_N_EXP
			SET_GPIOEXP_OUTPUT_HIGH(CAN_PHY1_STB_N_EXP);
			SET_GPIOEXP_CONFIG_OUTPUT(CAN_PHY1_STB_N_EXP);
#endif
		}
	}
	return TRUE;
}

boolean setudpip(pchar args, void *data, IfxStdIf_DPipe *io)
{
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax: setudpip xxx xxx xxx xxx"ENDL);
        IfxStdIf_DPipe_print(io, "           > Set UDP server IP"ENDL);
    } else {
		for(int i=0; i<4; i++) {
			if (Ifx_Shell_parseUInt32(&args, &udp_ip[i], FALSE) == FALSE)
			{
					IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value %s"ENDL, args);
					return TRUE;
			}
		}
		set_ip_flag = TRUE;
    }
	return TRUE;
}

boolean cantoudp(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 node, id, nbytes;
    boolean rtr_frame = FALSE;

	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : cantoudp x id n a b c..."ENDL);
        IfxStdIf_DPipe_print(io, "           > Send values a b c ... (total n bytes) on Multican node x with id and send to UDP"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax     : cantoudp x id r"ENDL);
        IfxStdIf_DPipe_print(io, "           > Send RTR frame on Multican node x with id"ENDL);
    }
    else
    {
    	// we return directly if the MultiCan is not initialized
    	if (g_Multican.drivers.initialized == FALSE)
    	{
       		IfxStdIf_DPipe_print(io, "Initialize CAN first with command caninit"ENDL);
       	    return TRUE;
    	}
    	if (g_Lwip.initialized != TRUE) {
				IfxStdIf_DPipe_print(io, "lwip doesn't init!"ENDL);
				return TRUE;
    	}
    	if (set_ip_flag == FALSE)
    	{
				IfxStdIf_DPipe_print(io, "Set UDP server IP first with command setudpip"ENDL);
				return TRUE;
    	}
    	// now we get the first argument (node number)
    	node = multican_get_node_input(&args);
    	if(node == INVALID_CAN_NODE)
    	{
        	IfxStdIf_DPipe_print(io, "Syntaxerror : invalid node"ENDL);
        	return TRUE;
    	}
    	if (Ifx_Shell_parseUInt32(&args, &id, FALSE) == FALSE)
    	{
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid id %s"ENDL, args);
            return TRUE;
    	}
    	canMode = CAN_GATEWAY_MODE;
    	if ((Ifx_Shell_matchToken(&args, "r") != FALSE) || (Ifx_Shell_matchToken(&args, "R") != FALSE))
    	{
    		rtr_frame = TRUE;
    		send_multican_message_gw(node, id, 0, NULL, rtr_frame);
    	}
    	else
    	{
    		rtr_frame = FALSE;
    		if (Ifx_Shell_parseUInt32(&args, &nbytes, FALSE) == FALSE)
    		{
    			IfxStdIf_DPipe_print(io, "Syntaxerror : invalid length %s"ENDL, args);
				return TRUE;
    		}
    		if (nbytes > 64 || nbytes < 0) {
				IfxStdIf_DPipe_print(io, "Syntaxerror : invalid length %s"ENDL, args);
				return TRUE;
    		}

    		if (nbytes == 0) {
				send_multican_message_gw(node, id, nbytes, NULL, rtr_frame);
    		} else if (nbytes>0 && nbytes <=4) {
				uint32 txData;
				Ifx_Shell_parseUInt32(&args, &txData, FALSE);
				send_multican_message_gw(node, id, nbytes, &txData, rtr_frame);
    		} else {
				uint8 nquadlets = nbytes/4;
				if (nbytes%4 != 0) nquadlets++;
				uint32 txData[nquadlets];
				for(int m=0; m<nquadlets; m++) {
					if (Ifx_Shell_parseUInt32(&args, &txData[m], FALSE) == FALSE)
					{
						IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value %s"ENDL, args);
						return TRUE;
					}
				}
				send_multican_message_gw(node, id, nbytes, txData, rtr_frame);
    		}
    	}
	}
    return TRUE;
}

/** \brief Print a message from Multican if available
 *
 */
void Print_MulticanMsg(IfxStdIf_DPipe *io)
{
    IfxMultican_PrintMsg();
}

/** \brief Handle the 'canstress' command
 *
 * \par Syntax
 *  - canstress : send can traffic continuously CANA-CANF
 */
boolean canstress(pchar args, void *data, IfxStdIf_DPipe *io)
{
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : canstress"ENDL);
        IfxStdIf_DPipe_print(io, "           > send can traffic continuously on CANA-CANF"ENDL);
    }
    else
    {
    	IfxStdIf_DPipe_print(io, "CAN stress replies on CAN re-send mechanism"ENDL);
    	IfxStdIf_DPipe_print(io, "Please keep all Aurix CAN nodes open and not connected during CAN stressing"ENDL);
    	canMode = CAN_NORMAL_MODE;
    	run_canstress_test();
    }
    return TRUE;
}

static void can_echo_message(uint8 node, uint32 id, uint32 data0, uint32 data1)
{
	/* we only echo on the node "E", "F" (can node 0 and 1) in DPX2 */
	if(((node == 0) && isCanE_EchoEnable) ||
	   ((node == 1) && isCanF_EchoEnable))
	{
		send_multican_message(node, id, data0, data1, FALSE);
	}
}

/** \brief Handle the 'canecho' command
 *
 * \par Syntax
 *  - canecho : enable/disable can echo in CANE-CANF, for test with Tegra
 */
boolean canecho(pchar args, void *data, IfxStdIf_DPipe *io)
{
	boolean * can_echo_ptr = NULL;
	char can_node = 'e';
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : canecho e/f on/off"ENDL);
        IfxStdIf_DPipe_print(io, "           > enable/disable can echo in CANE-CANF, for test with Tegra"ENDL);
    }
    else
    {
    	if (Ifx_Shell_matchToken(&args, "e") != FALSE)
    	{
    		can_echo_ptr = &isCanE_EchoEnable;
    		can_node = 'E';
    	}
    	else if (Ifx_Shell_matchToken(&args, "f") != FALSE)
    	{
    		can_echo_ptr = &isCanF_EchoEnable;
    		can_node = 'F';
    	}
    	else
		{
			IfxStdIf_DPipe_print(io, "Syntaxerror : invalid parameter %s"ENDL, args);
			return TRUE;
		}

    	if (Ifx_Shell_matchToken(&args, "on") != FALSE)
		{
    		IfxStdIf_DPipe_print(io, "Enable CAN echo in CAN node %c with Tegra"ENDL, can_node);
    		(*can_echo_ptr) = TRUE;
		}
		else if (Ifx_Shell_matchToken(&args, "off") != FALSE)
		{
			IfxStdIf_DPipe_print(io, "Disable CAN echo in CAN node %c with Tegra"ENDL, can_node);
			(*can_echo_ptr) = FALSE;
		}
		else
		{
			IfxStdIf_DPipe_print(io, "Syntaxerror : invalid parameter %s"ENDL, args);
			return TRUE;
		}
    }
    return TRUE;
}

void Print_CAN_Wakeup_detection(IfxStdIf_DPipe *io)
{
	IfxPort_Pin *pPort;
	if (node_a_lp)
	{
#if CAN_NODE_3_ENABLE == 1
		pPort = &(CAN_PHY1_RXD.pin);
		if (IfxPort_getPinState(pPort->port, pPort->pinIndex) == FALSE)
		{
	        IfxStdIf_DPipe_print(io, "Wakeup detected on Node A"ENDL);
	        can_xceiver_normal(0);
	        if (canwaketest_a)
	        {
	        	waitTime(2*TimeConst[TIMER_INDEX_1MS]); // this is just a placeholder value for delay
	        	can_xceiver_lp(0);
	        }
		}
#endif
	}
	if (node_b_lp)
	{
#if CAN_NODE_2_ENABLE == 1
		pPort = &(CAN_PHY3_RXD.pin);
		if (IfxPort_getPinState(pPort->port, pPort->pinIndex) == FALSE)
		{
	        IfxStdIf_DPipe_print(io, "Wakeup detected on Node B"ENDL);
	        can_xceiver_normal(1);
	        if (canwaketest_b)
	        {
	        	waitTime(2*TimeConst[TIMER_INDEX_1MS]); // this is just a placeholder value for delay
	        	can_xceiver_lp(1);
	        }
		}
#endif
	}

	if (node_cdef_lp)
	{
#if CAN_NODE_5_ENABLE == 1
		pPort = &(CANR1_RXD.pin);
		if (IfxPort_getPinState(pPort->port, pPort->pinIndex) == FALSE)
		{
	        IfxStdIf_DPipe_print(io, "Wakeup detected on Node C"ENDL);
	        can_xceiver_normal(2);
	        if (canwaketest_c)
	        {
	        	waitTime(2*TimeConst[TIMER_INDEX_1MS]); // this is just a placeholder value for delay
                can_xceiver_lp(2);
	        }
		}
#endif
#if CAN_NODE_4_ENABLE == 1
		pPort = &(CANR0_RXD.pin);
		if (IfxPort_getPinState(pPort->port, pPort->pinIndex) == FALSE)
		{
	        IfxStdIf_DPipe_print(io, "Wakeup detected on Node D"ENDL);
	        can_xceiver_normal(3);
	        if (canwaketest_d)
	        {
	        	waitTime(2*TimeConst[TIMER_INDEX_1MS]); // this is just a placeholder value for delay
	        	can_xceiver_lp(3);
	        }
		}
#endif
#if CAN_NODE_0_ENABLE == 1
		pPort = &(CAN0_RXD.pin);
		if (IfxPort_getPinState(pPort->port, pPort->pinIndex) == FALSE)
		{
	        IfxStdIf_DPipe_print(io, "Wakeup detected on Node E"ENDL);
	        can_xceiver_normal(4);
	        if (canwaketest_e)
	        {
	        	waitTime(2*TimeConst[TIMER_INDEX_1MS]); // this is just a placeholder value for delay
	        	can_xceiver_lp(4);
	        }

		}
#endif
#if CAN_NODE_1_ENABLE == 1
		pPort = &(CAN_PHY2_RXD.pin);
		if (IfxPort_getPinState(pPort->port, pPort->pinIndex) == FALSE)
		{
	        IfxStdIf_DPipe_print(io, "Wakeup detected on Node F"ENDL);
	        can_xceiver_normal(5);
	        if (canwaketest_f)
	        {
	        	waitTime(2*TimeConst[TIMER_INDEX_1MS]); // this is just a placeholder value for delay
	        	can_xceiver_lp(5);
	        }
		}
#endif
	}
}

/** \brief Handle the 'cancnt' command
 *
 * \par Syntax
 *  - cancnt : Show can receive counter.
 *  - cancnt on/off : On/Off show counter when receive data.
 */
uint32 can_cnt_print = 0;
boolean cancnt(pchar args, void *data, IfxStdIf_DPipe *io)
{
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "cancnt on X / off"ENDL);
    } else {
		if (Ifx_Shell_matchToken(&args, "on") != FALSE)
		{
			can_cnt_flag = TRUE;
			if (Ifx_Shell_parseUInt32(&args, &can_cnt_print, FALSE) == FALSE)
	        {
	            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid cnt %s"ENDL, args);
	            return TRUE;
	        }
		} else if (Ifx_Shell_matchToken(&args, "off") != FALSE) {
			can_cnt_flag = FALSE;
			can_cnt_print = 0;
		} else {
			can_cnt_flag = TRUE;
			IfxMultican_PrintMsg();
			can_cnt_flag = FALSE;
		}
	}
	return TRUE;
}


// Send stress frame. (a->b, b->c, c->d, d->e, e->f, f->a)
void canstress_run()
{
	uint32 txData[2] = {0xA, 0x1};
	uint32 node, id;
	uint8 NODE_LIMIT = 6;
	const uint32 *filter_id;
	filter_id = FILTER_ID_F;
	for(uint8 i = 0; i < NODE_LIMIT; i++) {
		node = i;
		if(i >= 5)
			id = filter_id[0];
		else
			id = filter_id[i+1];

		txData[0] = id;
		txData[1] = node;
		if(!fdMode)
			send_multican_message(node, id, txData[0], txData[1], FALSE);
		else {
			uint32 fdData[3] = {0xABCD1234, 0xAABBCCDD, 0x12345678};
			send_multican_message_gw(node, id, 12, fdData, FALSE);
		}
	}
}

/** \brief Handle the 'canst' command
 *
 * \par Syntax
 *  - canst : send can stress on standard mode
 */
boolean canstress_H(pchar args, void *data, IfxStdIf_DPipe *io)
{
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "canstress"ENDL);
    } else {
		// a->b, b->c, ... , f->a
    	if (can_cnt_flag)
    		canstress_run();
    	else
    		IfxStdIf_DPipe_print(io, "Please key 'cancnt on x'"ENDL);
	}
	return TRUE;
}

/** \brief Handle the 'canfilter' command
 *
 * \par Syntax
 *  - canfilter : set can filter
 */
boolean canfilter(pchar args, void *data, IfxStdIf_DPipe *io)
{
  if (Ifx_Shell_matchToken(&args, "?") != FALSE)
  {
    IfxStdIf_DPipe_print(io, "Syntax     : canfilter x range r1 r2 [index] [ext]"ENDL);
	IfxStdIf_DPipe_print(io, "           > Set filter index range from r1 to r2 of the node x "ENDL);
	IfxStdIf_DPipe_print(io, "           > index default is 0, it could be 0~63"ENDL);
	IfxStdIf_DPipe_print(io, "           > If ext is given, then will set extended filter"ENDL);
	IfxStdIf_DPipe_print(io, "Syntax     : canfilter x disable [index] [ext]"ENDL);
	IfxStdIf_DPipe_print(io, "           > Disable filter index of the node x "ENDL);
  } else {
    uint32 node_id;
    node_id = multican_get_node_input(&args);
    if(node_id == INVALID_CAN_NODE)
    {
      IfxStdIf_DPipe_print(io, "Syntaxerror : invalid node"ENDL);
      return TRUE;
	}
    if (Ifx_Shell_matchToken(&args, "range") != FALSE)
    {
      uint32 range1, range2, index;
      boolean b_extid = FALSE;/* [EXTENED ID test] */
      if (Ifx_Shell_parseUInt32(&args, &range1, FALSE) == FALSE)
      {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid range1 %s"ENDL, args);
        return TRUE;
      }
      if (Ifx_Shell_parseUInt32(&args, &range2, FALSE) == FALSE)
      {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid range2 %s"ENDL, args);
        return TRUE;
      }
      if (Ifx_Shell_parseUInt32(&args, &index, FALSE) == FALSE)
      {
    	  index = 0;
      }
      /* [EXTENED ID test] start */
      if ( Ifx_Shell_matchToken(&args, "extid") != FALSE )
      {
#if CAN_EXT_ID_ENABLED == 1
          b_extid = TRUE;
#else
          IfxStdIf_DPipe_print(io, "Syntaxerror : This AURIX version did not support extended id mode."ENDL);
#endif
      }
	  if(b_extid){
	      //ext_filter[index 0,1][range]
		  can_config[node_id].ext_filter[index][0]=range1;
		  can_config[node_id].ext_filter[index][1]=range2;

	  }else{
	      //std_filter[index 0,1][range]
		  can_config[node_id].std_filter[index][0]=range1;
		  can_config[node_id].std_filter[index][1]=range2;
	  }
      /* [EXTENED ID test] end */
	  //IfxStdIf_DPipe_print(io, "Node %d Set filter id from %d to %d"ENDL, node_id, range1, range2);
	  IfxMultican_initFilters_with_range(node_id, range1, range2, index, b_extid);
    }else if (Ifx_Shell_matchToken(&args, "disable") != FALSE){
        uint32 index;
        boolean b_extid = FALSE;/* [EXTENED ID test] */
        if (Ifx_Shell_parseUInt32(&args, &index, FALSE) == FALSE)
        {
            index = 0;
        }
        if ( Ifx_Shell_matchToken(&args, "extid") != FALSE )
        {
#if CAN_EXT_ID_ENABLED == 1
            b_extid = TRUE;
#else
            IfxStdIf_DPipe_print(io, "Syntaxerror : This AURIX version did not support extended id mode."ENDL);
#endif
        }
        IfxMultican_disable_Filters(node_id, index, b_extid);
    }else{
      IfxStdIf_DPipe_print(io, "Syntaxerror : invalid parameter %s"ENDL, args);
    }
  }
  return TRUE;
}

void cansend_conti_set(boolean enable, uint32 node)
{
    cansend_conti_enable = enable;
    cansend_conti_node = node;
}
void cansend_conti_do(void)
{
    if(cansend_conti_enable == FALSE)
        return;
    Ifx_Console_print(ENDL"Can Send Contininously Test: CAN %d send data"ENDL, cansend_conti_node);
    send_multican_message(cansend_conti_node-1, 0, 0x55555555, 0x55555555, TRUE);
}

boolean multicansend_test(pchar args, void *data, IfxStdIf_DPipe *io)  //ycnew
{
    uint32 node;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : cancontinuous start x"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax     : cancontinuous stop"ENDL);
    }
    else
    {
        // we return directly if the MultiCan is not initialized
        if (g_Multican.drivers.initialized == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Initialize CAN first with command caninit"ENDL);
            return TRUE;
        }
        if (Ifx_Shell_matchToken(&args, "start") == TRUE)
        {
            if (Ifx_Shell_parseUInt32(&args, &node, FALSE) == FALSE)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid node"ENDL);
                return TRUE;
            }
            if (node > 6 || node < 1)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : node have to be 1 ~ 6"ENDL);
                return TRUE;
            }
            cansend_conti_set(TRUE, node);
        }
        else if (Ifx_Shell_matchToken(&args, "stop") == TRUE)
        {
            IfxStdIf_DPipe_print(io, "STOP CANBUS TEST"ENDL);
            cansend_conti_set(FALSE, 0);
        }
        else
        {
            IfxStdIf_DPipe_print(io, "Please key cancontinuous start x or cancontinuous stop"ENDL);
            return TRUE;
        }
    }
    return TRUE;
}

boolean can_measure(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 parameter;
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : canmeasure rt 0"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax     : canmeasure rt 1"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax     : canmeasure cnt 2000"ENDL);
    }
    else
    {
        // we return directly if the MultiCan is not initialized
        if (g_Multican.drivers.initialized == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Initialize CAN first with command caninit"ENDL);
            return TRUE;
        }
        if (Ifx_Shell_matchToken(&args, "rt") == TRUE)
        {
            if (Ifx_Shell_parseUInt32(&args, &parameter, FALSE) == FALSE)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid node"ENDL);
                return TRUE;
            }
            if(parameter > 2 || parameter < 0)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : only support 0 or 1"ENDL);
                return TRUE;
            } 
            rt_sample.enabled = parameter;
        }
        else if (Ifx_Shell_matchToken(&args, "cnt") == TRUE)
        {
            if (Ifx_Shell_parseUInt32(&args, &parameter, FALSE) == FALSE)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid node"ENDL);
                return TRUE;
            }
            if(parameter > 10000 || parameter < 0)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : support below 10000 "ENDL);
                return TRUE;
            } 
            rt_sample.tx_target_cnt = parameter;
            rt_sample.tx_cnt = 0;
            rt_sample.tx_start = 0;
            rt_sample.tx_end = 0;
            return TRUE;
        }
        else if (Ifx_Shell_matchToken(&args, "print") == TRUE)
        {
            IfxStdIf_DPipe_print(io, "tx_count=%d"ENDL, rt_sample.tx_cnt);
            printf("%llu %llu start: end\r\n", rt_sample.tx_start, rt_sample.tx_end);
            printf("%llu %llu %llu %llu  b,c,d,e\r\n", rt_sample.point_b, rt_sample.point_c, rt_sample.point_d,rt_sample.point_e);
            rt_sample.tx_cnt = 0;
            rt_sample.tx_start = 0;
            rt_sample.tx_end = 0;
            return TRUE;
        }
        else
        {
            IfxStdIf_DPipe_print(io, "Please key canmeasure start or canmeasure stop"ENDL);
            return TRUE;
        }
    }
}


#endif //#if MCM_CAN_TC39X_ENABLE == 1

void clearCanCount(void)
{
    for(uint8 i = 0; i < 6; i++) {
        rxCnt[i] = 0;
    }
}

/** \brief Handle the 'canclear' command
 *
 * \par Syntax
 *  - canclear : Clear can receive counter.
 */
boolean canclear(pchar args, void *data, IfxStdIf_DPipe *io)
{
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax : canclear"ENDL);
		IfxStdIf_DPipe_print(io, "       > Clear rx count."ENDL);
    } else {
		clearCanCount();
		IfxStdIf_DPipe_print(io, "Clear success!"ENDL);
	}
	return TRUE;
}

/** \brief Handle the 'canthroughput' command
 *
 * \par Syntax
 *  - canthroughput : Start throughput test. Send maximum frame on bus.
 */
boolean canthroughput(pchar args, void *data, IfxStdIf_DPipe *io)
{
    if (Ifx_Shell_matchToken(&args, "?") != FALSE) {
        IfxStdIf_DPipe_print(io, "Syntax      : canthroughput test n m length t"ENDL);
        IfxStdIf_DPipe_print(io, "            > Use n node sending max frame for t seconds to m node."ENDL);
        IfxStdIf_DPipe_print(io, "            > (n, m) = a ~ f"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax      : canthroughput result"ENDL);
        IfxStdIf_DPipe_print(io, "            > Show throughput test result."ENDL);
        IfxStdIf_DPipe_print(io, "Syntax      : canthroughput setflag"ENDL);
        IfxStdIf_DPipe_print(io, "            > Show show_Canbus_log."ENDL);
    } else {
        // we return directly if the MultiCan is not initialized
        if (g_Multican.drivers.initialized == FALSE) {
            IfxStdIf_DPipe_print(io, "Initialize CAN first with command caninit"ENDL);
            return TRUE;
        }
        if (Ifx_Shell_matchToken(&args, "result") != FALSE) {
            // print_Can_Rx_Counter(CAN_MAX_THROUGHPUT);
            Multican_printTpResult();
            return TRUE;
        } else if (Ifx_Shell_matchToken(&args, "test") != FALSE) {
            CmdTest.txNode = multican_get_node_input(&args);
            if(CmdTest.txNode == INVALID_CAN_NODE){
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid node"ENDL);
                return TRUE;
            }
            CmdTest.rxNode = multican_get_node_input(&args);
            if(CmdTest.rxNode == INVALID_CAN_NODE){
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid node"ENDL);
                return TRUE;
            }
//            CmdTest.rxNode += 1;
            if (Ifx_Shell_parseUInt32(&args, &CmdTest.DataLength, FALSE) == FALSE) {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid number %s"ENDL, args);
                return TRUE;
            }
            if (Ifx_Shell_parseUInt32(&args, &CmdTest.LimitTime, FALSE) == FALSE){
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid time %s"ENDL, args);
                return TRUE;
            } 
            IfxStdIf_DPipe_print(io, "Start throughput test for %d sec."ENDL, CmdTest.LimitTime);
            Multican_setCanCmdTestMode(CAN_CMD_TEST_TP);
        }else if (Ifx_Shell_matchToken(&args, "setflag") != FALSE) {
            // print_Can_Rx_Counter(CAN_MAX_THROUGHPUT);
            show_Canbus_log = TRUE;
            return TRUE;
        }
        else {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid parameter %s"ENDL, args);
            return TRUE;
        }
    }
    return TRUE;
}

/* Called by caninit function */
void Multican_CmdTestInitial(void)
{
    CmdTest.Mode = CAN_CMD_TEST_STRESS;
    CmdTest.txNode = 0;
    CmdTest.rxNode = 0;
    CmdTest.LoopCount = 6;
    CmdTest.DataLength = 8;
    CmdTest.StartTime = 0;
    CmdTest.EndTime = 0xFFFFFFFF;
    CmdTest.LimitTime = 0;
    CmdTest.Cnt = 0;
    CmdTest.flag = FALSE;

    for (uint8 j = 0; j < 16; j++) {
        if (j % 2 == 0) CmdTest.CanData[j] = 0x55555555;
        else            CmdTest.CanData[j] = 0xAAAAAAAA;
    }
}

/* Setting command test mode, initial parameter */
void Multican_setCanCmdTestMode(App_Multican_CmdTestMode mode)
{
    CmdTest.Mode = mode;
    if (CmdTest.Mode == CAN_CMD_TEST_TP) {
        CmdTest.StartTime = (uint32)(now() / TimeConst_1us);
//        CmdTest.StartTime = (uint32)(now());
        CmdTest.EndTime = 0;
        CmdTest.LimitTime = CmdTest.LimitTime * 1000000; 

        CmdTest.Cnt = 0;
//        show_Canbus_log = FALSE;
        can_cnt_flag = FALSE;
        clearCanCount();
        CmdTest.flag = TRUE;
    } else if(CmdTest.Mode == CAN_CMD_TEST_STRESS){
    
    }
	else {
        //CAN_CMD_SHOWLOG
         show_Canbus_log = TRUE;
    }
}


/* Print throughput result */
void Multican_printTpResult(void)
{
    CmdTest.EndTime = receive_time;
    uint32 frame_count = rxCnt[(CmdTest.rxNode)];
    uint32 cost_time = CmdTest.EndTime - CmdTest.StartTime;
    uint32 throughput = (float)frame_count / cost_time * 1000000;
    IfxStdIf_DPipe_print(io_tmp, "Throughput Test Result:"ENDL);
    IfxStdIf_DPipe_print(io_tmp, "Cost time  : %d.%03d s"ENDL, (uint32)(cost_time / 1000000), (uint32)(cost_time/1000) % 1000);
    IfxStdIf_DPipe_print(io_tmp, "Get frame  : %d f"ENDL, frame_count);
    IfxStdIf_DPipe_print(io_tmp, "Throughput : %d f/s"ENDL, throughput);
}

/* Main command test function */
void Multican_CmdTest(void)
{
    /* Check CAN */
    if (g_Multican.drivers.initialized == FALSE) {
         return;
    }
    /* Check flag, activated  by command */
    if (CmdTest.flag) {
        CmdTest.Cnt ++;
        switch(CmdTest.Mode)
        {
/*
            case CAN_CMD_TEST_STRESS:
                for (uint8 i = 0; i < CAN_USED_NODES_NUM; i++) {
                    CmdTest.txNode = i;
                    CmdTest.rxNode = (i + 1) % CAN_USED_NODES_NUM;
                    send_multican_message(CmdTest.txNode, CmdTest.rxNode, CmdTest.DataLength, CmdTest.CanData, FALSE, fdMode);
                }
                CmdTest.flag = FALSE;
                break;
            case CAN_CMD_TEST_LOOP:
                for (uint8 i = 0; i < CmdTest.LoopCount; i++) {
                    CmdTest.txNode = i % CAN_USED_NODES_NUM;
                    CmdTest.rxNode = (i + 1) % CAN_USED_NODES_NUM;
                    send_multican_message(CmdTest.txNode, CmdTest.rxNode, CmdTest.DataLength, CmdTest.CanData, FALSE, fdMode);
                }
                break;
            case CAN_CMD_TEST_SINGLE:
                for (uint8 i = 0; i < CmdTest.LoopCount; i++) {
                    send_multican_message(CmdTest.txNode, CmdTest.rxNode, CmdTest.DataLength, CmdTest.CanData, FALSE, fdMode);
                }
                break;
*/
            case CAN_CMD_TEST_TP:
                for (uint8 i = 0; i < 32; i++) {
                    send_multican_message(CmdTest.txNode, CmdTest.rxNode+1, CmdTest.DataLength, CmdTest.CanData, FALSE);    
                }
                break;
        }

        /* Print through msg every 5 second */
        if (CmdTest.Cnt % 5000 == 0) {
            IfxStdIf_DPipe_print(io_tmp, "Running CAN command task ..."ENDL);
            if (CmdTest.Mode == CAN_CMD_TEST_TP) {
                Multican_printTpResult();
            } else {
                //print_Can_Rx_Counter();
            }
        }

        /* Check limit */
        switch(CmdTest.Mode)
        {
#if 0
            /* Use count to check */
            case CAN_CMD_TEST_LOOP:
            case CAN_CMD_TEST_SINGLE:
                if (CmdTest.Cnt >= CmdTest.LimitTime) {
                    CmdTest.flag = FALSE;
                    IfxStdIf_DPipe_print(io_tmp, "Finish test!"ENDL);
                }
                break;
#endif
            /* Use cost time to check */
            case CAN_CMD_TEST_TP:
                if ((((uint32)(now() / TimeConst_1us)) - CmdTest.StartTime) >= CmdTest.LimitTime) {
                    CmdTest.flag = FALSE;
                    IfxStdIf_DPipe_print(io_tmp, "Finish throughput test!"ENDL);
                }
                break;
            default:
                break;
        }
    }
}



volatile uint32 mess_count_case1[16]={0};
volatile uint32 mess_count_case2[21]={0};
volatile uint32 mess_count_case3[6]={0};
volatile uint32 mess_count_case4[6]={0};
volatile uint32 mess_count_case5[4]={0};


volatile boolean cpu2_start_send_can = FALSE;
volatile boolean cpu1_start_send_can = FALSE;

volatile uint32 time_count_cpu1 = 0;
volatile uint32 time_count_cpu2 = 0;
volatile uint32 total_time = 0;
static uint8 data_64[64]={0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,
                                    0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,0xB0,
                                    0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,
                                    0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,0xC0,
                                    0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,
                                    0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,0xD0,
                                    0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,
                                    0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,0xE0};
static uint8 data_8[8]={0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8};
static uint8 data_16[16]={0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,
                                    0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,0xB0};
static uint8 data_20[20]={0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,
                                    0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,0xB0,
                                    0xB1,0xB2,0xB3,0xB4};


/** \brief Handle the 'cantestcase' command
 *
 * \par Syntax
 *  - multicansend_test x id a b : aend a and b on Multican node x with id
 */
boolean cantestcase(pchar args, void *data, IfxStdIf_DPipe *io)  //ycnew
{
    uint32 mode,time;
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : cantestcase a b"ENDL);
        IfxStdIf_DPipe_print(io, "           > Send CAN Data for a ms and b times per ms"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax     : multicansend R"ENDL);
        IfxStdIf_DPipe_print(io, "           > try to send a 64bit data"ENDL);
        UDP_send_bytes = 0;
    }
    else if(Ifx_Shell_matchToken(&args, "R") != FALSE)
    {
        static uint8 data[64]={   0x55,0x55,0x55,0x55, 0x55,0x55,0x55,0x55, 0x55,0x55,0x55,0x55, 0x55,0x55,0x55,0x55, 
                    0x55,0x55,0x55,0x55, 0x55,0x55,0x55,0x55, 0x55,0x55,0x55,0x55, 0x55,0x55,0x55,0x55,
                    0x55,0x55,0x55,0x55, 0x55,0x55,0x55,0x55, 0x55,0x55,0x55,0x55, 0x55,0x55,0x55,0x55,
                    0x55,0x55,0x55,0x55, 0x55,0x55,0x55,0x55, 0x55,0x55,0x55,0x55, 0x55,0x55,0x55,0x55};
        
        send_multican_message_gw(0, 0, 64, (uint32 *) &data[0], FALSE);
    }
    else 
    {
        if(Ifx_Shell_parseUInt32(&args, &time, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid time %s"ENDL, args);
            return TRUE;
        }
        
        cpu1_start_send_can = TRUE;
        cpu2_start_send_can = TRUE;
        total_time = time;
    }
     return TRUE;
}   

void cpu2_can_test(){
    if(board.aurix_index == MAIN_AURIX){
        
        if(cpu2_start_send_can)
        {
            if(time_count_cpu2%1100 == 0)
            {
                send_multican_message_gw(0, 51, 8, (uint32 *) &data_8[0], FALSE);
                send_multican_message_gw(0, 52, 8, (uint32 *) &data_8[0], FALSE);

                mess_count_case2[20]=mess_count_case2[20]+2;
            }
            if(time_count_cpu2%1000 == 0)
            {

                send_multican_message_gw(0, 53, 8, (uint32 *) &data_8[0], FALSE);
                send_multican_message_gw(0, 54, 8, (uint32 *) &data_8[0], FALSE);
                for(int g = 56;g<67;g++)
                {
                    send_multican_message_gw(0, g, 8, (uint32 *) &data_8[0], FALSE);
                }
                send_multican_message_gw(1, 22, 8, (uint32 *) &data_8[0], FALSE);
                mess_count_case2[19]=mess_count_case2[19]+13;
                mess_count_case3[5]=mess_count_case3[5]+1;
            }
            if(time_count_cpu2%920 == 0)
            {

                send_multican_message_gw(0, 50, 8, (uint32 *) &data_8[0], FALSE);
                mess_count_case2[18]++;
            }
            if(time_count_cpu2%840 == 0)
            {

                send_multican_message_gw(0, 49, 8, (uint32 *) &data_8[0], FALSE);
                mess_count_case2[17]++;
            }
            if(time_count_cpu2%800 == 0)
            {
                send_multican_message_gw(0, 47, 8, (uint32 *) &data_8[0], FALSE);
                mess_count_case2[16]++;
            }
            if(time_count_cpu2%440 == 0)
            {
                send_multican_message_gw(0, 46, 8, (uint32 *) &data_8[0], FALSE);
                mess_count_case2[15]++;
            }
            if(time_count_cpu2%400 == 0)
            {
                send_multican_message_gw(0, 44, 8, (uint32 *) &data_8[0], FALSE);
                mess_count_case2[14]++;
            }
            if(time_count_cpu2%380 == 0)
            {
                send_multican_message_gw(0, 45, 8, (uint32 *) &data_8[0], FALSE);
                mess_count_case2[13]++;
            }
            if(time_count_cpu2%300 == 0)
            {
                send_multican_message_gw(0, 42, 8, (uint32 *) &data_8[0], FALSE);
                mess_count_case2[12]++;
            }
            if(time_count_cpu2%280 == 0)
            {
                send_multican_message_gw(0, 41, 8, (uint32 *) &data_8[0], FALSE);
                mess_count_case2[11]++;
            }
            if(time_count_cpu2%275 == 0)
            {
                send_multican_message_gw(0, 40, 8, (uint32 *) &data_8[0], FALSE);
                mess_count_case2[10]++;
            }
            if(time_count_cpu2%260 == 0)
            {
                send_multican_message_gw(0, 38, 8, (uint32 *) &data_8[0], FALSE);
                send_multican_message_gw(0, 39, 8, (uint32 *) &data_8[0], FALSE);
                mess_count_case2[9]=mess_count_case2[9]+2;
            }
            if(time_count_cpu2%160 == 0)
            {
                send_multican_message_gw(0, 43, 8, (uint32 *) &data_8[0], FALSE);
                send_multican_message_gw(1, 21, 8, (uint32 *) &data_8[0], FALSE);
                mess_count_case2[8]++;
                mess_count_case3[4]=mess_count_case3[4]+1;
            }
            if(time_count_cpu2%95 == 0)
            {
                send_multican_message_gw(0, 29, 8, (uint32 *) &data_8[0], FALSE);
                // send_multican_message_gw(0, 95, 8, (uint32 *) &data_8[0], FALSE);
                mess_count_case2[7]=mess_count_case2[7]+1;
            }

            
            if(time_count_cpu2%50 == 0)
            {
                send_multican_message_gw(0, 32, 8, (uint32 *) &data_8[0], FALSE);
                // send_multican_message_gw(0, 98, 8, (uint32 *) &data_8[0], FALSE);
                mess_count_case2[6]= mess_count_case2[6]+1;
            }
            if(time_count_cpu2%45 == 0)
            {
                // for(int i=0;i<67;i=i+66)
                // {
                send_multican_message_gw(0, 18, 8, (uint32 *) &data_8[0], FALSE);
                send_multican_message_gw(0, 21, 8, (uint32 *) &data_8[0], FALSE);
                send_multican_message_gw(0, 31, 8, (uint32 *) &data_8[0], FALSE);
                // }
                mess_count_case2[5]=mess_count_case2[5]+3;
            }
            if(time_count_cpu2%40 == 0)
            {
                // for(int i=0;i<67;i=i+66)
                // {
                send_multican_message_gw(0, 19, 8, (uint32 *) &data_8[0], FALSE);
                send_multican_message_gw(0, 20, 8, (uint32 *) &data_8[0], FALSE);
                // }
                mess_count_case2[4]=mess_count_case2[4]+2;
            }
            if(time_count_cpu2%30 == 0)
            {
                
                send_multican_message_gw(0, 24, 8, (uint32 *) &data_8[0], FALSE);
                // send_multican_message_gw(0, 90, 8, (uint32 *) &data_8[0], FALSE);
                mess_count_case2[3] = mess_count_case2[3]+1;
            }
            
            if(time_count_cpu2%25 == 0)
            {
                // for(int i =0;i<47;i=i+23)
                // {
                    send_multican_message_gw(1, 10, 20, (uint32 *) &data_20[0], FALSE);
                // }    
                mess_count_case3[3]=mess_count_case3[3]+1;

            }
            if(time_count_cpu2%20 == 0)
            {
                // for(int i=0;i<67;i=i+66)
                // {
                send_multican_message_gw(0, 13, 8, (uint32 *) &data_8[0], FALSE);
                send_multican_message_gw(0, 16, 8, (uint32 *) &data_8[0], FALSE);
                send_multican_message_gw(0, 17, 8, (uint32 *) &data_8[0], FALSE);
                send_multican_message_gw(0, 22, 8, (uint32 *) &data_8[0], FALSE);
                send_multican_message_gw(0, 25, 8, (uint32 *) &data_8[0], FALSE);
                send_multican_message_gw(0, 27, 8, (uint32 *) &data_8[0], FALSE);
                // }
                // for(int i =0;i<47;i=i+23)
                // {
                send_multican_message_gw(1, 8, 16, (uint32 *) &data_16[0], FALSE);
                // }
                mess_count_case2[2]=mess_count_case2[2]+6;
                mess_count_case3[2]=mess_count_case3[2]+1;
            }
            if(time_count_cpu2%15 == 0)
            {
                // for(int i=0;i<67;i=i+66)
                // {
                    send_multican_message_gw(0, 10, 8, (uint32 *) &data_8[0], FALSE);
                    send_multican_message_gw(0, 11, 8, (uint32 *) &data_8[0], FALSE);
                    send_multican_message_gw(0, 23, 8, (uint32 *) &data_8[0], FALSE);
                // }
                
                mess_count_case2[1]=mess_count_case2[1]+3;
            }
            if((time_count_cpu2-1)%10 == 0)
            {
                for(int g = 4;g<10;g++)
                {
                    send_multican_message_gw(0, g, 8, (uint32 *) &data_8[0], FALSE);
                }
                // for(int g = 70;g<76;g++)
                // {
                    //send_multican_message_gw(0, g, 8, (uint32 *) &data_8[0], FALSE);
                // }
                // for(int i =0;i<47;i=i+23)
                // {
                    send_multican_message_gw(1, 2, 20, (uint32 *) &data_20[0], FALSE);
                    send_multican_message_gw(1, 3, 64, (uint32 *) &data_64[0], FALSE);
                    send_multican_message_gw(1, 7, 8, (uint32 *) &data_8[0], FALSE);
                // }
        
                mess_count_case2[0]=mess_count_case2[0]+6;
                mess_count_case3[1]= mess_count_case3[1]+3;
            }
            
             if((time_count_cpu2-1)%5 == 0)
            {
                // for(int i =0;i<47;i=i+23)
                // {
                send_multican_message_gw(1, 1, 20, (uint32 *) &data_20[0], FALSE);
                // }
                mess_count_case3[0]= mess_count_case3[0]+1;
            }
            if(time_count_cpu2-1==total_time)
            {
                int total_0 = 0;
                int total_1 = 0;
                for(int b = 0;b < 21; b++)
                {
                    total_0 = total_0 + mess_count_case2[b];
                    Ifx_Console_print("count 0 %d: %d\r\n", b,mess_count_case2[b]);
                }
                Ifx_Console_print("Total count 0: %d\r\n",total_0);
                total_0 = 0;
                for(int b = 0;b < 21; b++)
                {
                    mess_count_case2[b]=0;
                }
                for(int b = 0;b < 6; b++)
                {
                    total_1 = total_1 + mess_count_case3[b];
                    Ifx_Console_print("count 1 %d: %d\r\n", b,mess_count_case3[b]);
                }
                Ifx_Console_print("Total count 1: %d\r\n",total_1);
                total_1 = 0;
                for(int b = 0;b < 6; b++)
                {
                    mess_count_case3[b]=0;
                }
                //Ifx_Console_print("total package count: %d\r\n", total_package_send);
                time_count_cpu2 = 0;
                cpu2_start_send_can = FALSE;
            }

            ++time_count_cpu2;
        }
        
    }
    

}
void cpu1_can_test(){
    int add[3]={0,28,54};
	if(board.aurix_index == MAIN_AURIX){
        
	        if(cpu1_start_send_can)
        {
            if(time_count_cpu1%1000 == 0)
            {
                for(int g = 43;g<66;g++)
                {
                    send_multican_message_gw(3, g, 8, (uint32 *) &data_8[0], FALSE);
                }
                send_multican_message_gw(3, 21, 8, (uint32 *) &data_8[0], FALSE);
                send_multican_message_gw(4, 28, 8, (uint32 *) &data_8[0], FALSE);
                send_multican_message_gw(2, 1, 64, (uint32 *) &data_64[0], FALSE);
                send_multican_message_gw(2, 8, 64, (uint32 *) &data_64[0], FALSE);
                send_multican_message_gw(2, 15, 64, (uint32 *) &data_64[0], FALSE);
                mess_count_case4[5]++;
                mess_count_case5[3] = mess_count_case5[3]+3;
                mess_count_case1[15] = mess_count_case1[15]+24;
            }
            if(time_count_cpu1%800 == 0)
            {
                send_multican_message_gw(3, 42, 8, (uint32 *) &data_8[0], FALSE);
                mess_count_case1[14]++;
            }
            if(time_count_cpu1%400 == 0)
            {
                send_multican_message_gw(3, 40, 8, (uint32 *) &data_8[0], FALSE);            
                mess_count_case1[13]++;
            }
            if(time_count_cpu1%380 == 0)
            {
                send_multican_message_gw(3, 41, 8, (uint32 *) &data_8[0], FALSE);
                mess_count_case1[12]++;
            }
            if(time_count_cpu1%160 == 0)
            {
                send_multican_message_gw(3, 36, 8, (uint32 *) &data_8[0], FALSE);
                send_multican_message_gw(3, 37, 8, (uint32 *) &data_8[0], FALSE);
                send_multican_message_gw(3, 38, 8, (uint32 *) &data_8[0], FALSE);
                mess_count_case1[11] = mess_count_case1[11] + 3;
            }
            if(time_count_cpu1%110 == 0)
            {
                send_multican_message_gw(3, 35, 8, (uint32 *) &data_8[0], FALSE);
                mess_count_case1[10]++;
            }
            if(time_count_cpu1%80 == 0)
            {
                // for(int i=0;i<3;i++)
                // {
                send_multican_message_gw(4, 16, 16, (uint32 *) &data_16[0], FALSE);
                // }
                mess_count_case4[4]=mess_count_case4[4]+1;
            }
            if(time_count_cpu1%60 == 0)
            {
                // for(int i=0;i<67;i=i+66)
                // {
                    send_multican_message_gw(3, 34, 8, (uint32 *) &data_8[0], FALSE);
                // }
                mess_count_case1[9]=mess_count_case1[9]+1;
            }
            if(time_count_cpu1%50 == 0)
            {
                send_multican_message_gw(2, 3, 64, (uint32 *) &data_64[0], FALSE);
                send_multican_message_gw(2, 10, 64, (uint32 *) &data_64[0], FALSE);
                send_multican_message_gw(2, 17, 64, (uint32 *) &data_64[0], FALSE);
                mess_count_case5[2] = mess_count_case5[2]+3;
            }
            if(time_count_cpu1%45 == 0)
            {
                // for(int i=0;i<67;i=i+66)
                // {
                    for(int g = 29;g<34;g++)
                    {
                        send_multican_message_gw(3, g, 8, (uint32 *) &data_8[0], FALSE);
                    }
                // }
                mess_count_case1[8] = mess_count_case1[8] + 5;
            }
            if(time_count_cpu1%40 == 0)
            {
                // for(int i=0;i<3;i++)
                // {
                send_multican_message_gw(4, 13, 8, (uint32 *) &data_8[0], FALSE);
                // }
                // for(int i=0;i<67;i=i+66)
                // {
                    for(int g = 24;g<29;g++)
                    {
                        send_multican_message_gw(3, g, 8, (uint32 *) &data_8[0], FALSE);
                    }
                // }
                send_multican_message_gw(3, 39, 8, (uint32 *) &data_8[0], FALSE);
                mess_count_case4[3]=mess_count_case4[3]+1;
                mess_count_case1[7] = mess_count_case1[7]+6;
            }
            if(time_count_cpu1%35 == 0)
            {
                // for(int i=0;i<68;i=i+67)
                // {
                    send_multican_message_gw(3, 20, 8, (uint32 *) &data_8[0], FALSE);
                    send_multican_message_gw(3, 22, 8, (uint32 *) &data_8[0], FALSE);
                // }
                send_multican_message_gw(3, 23, 8, (uint32 *) &data_8[0], FALSE);
                // send_multican_message_gw(3, 88, 8, (uint32 *) &data_8[0], FALSE);
                mess_count_case1[6] = mess_count_case1[6]+3;
            }
            if(time_count_cpu1%30 == 0)
            {
                // for(int i=0;i<68;i=i+67)
                // {
                    for(int g = 15;g<20;g++)
                    {
                        send_multican_message_gw(3, g, 8, (uint32 *) &data_8[0], FALSE);
                    } 
                // }
                
                mess_count_case1[5] = mess_count_case1[5]+5;
            }
            if(time_count_cpu1%25 == 0)
            {
                // for(int i=0;i<3;i++)
                // {
                    send_multican_message_gw(4, 12, 20, (uint32 *) &data_20[0], FALSE);
                // }           
                send_multican_message_gw(3, 14, 8, (uint32 *) &data_8[0], FALSE);
                // send_multican_message_gw(3, 81, 8, (uint32 *) &data_8[0], FALSE);
        
               mess_count_case4[2]=mess_count_case4[2]+1;
               mess_count_case1[4]=mess_count_case1[4]+1;

            }

            if(time_count_cpu1%20 == 0)
            {
                send_multican_message_gw(2, 5, 64, (uint32 *) &data_64[0], FALSE);
                send_multican_message_gw(2, 12, 64, (uint32 *) &data_64[0], FALSE);
                send_multican_message_gw(2, 19, 64, (uint32 *) &data_64[0], FALSE);
                // for(int i=0;i<68;i=i+67)
                // {
                    send_multican_message_gw(3, 11, 8, (uint32 *) &data_8[0], FALSE);
                    send_multican_message_gw(3, 12, 8, (uint32 *) &data_8[0], FALSE);
                    send_multican_message_gw(3, 13, 8, (uint32 *) &data_8[0], FALSE);
                // }
                mess_count_case5[1] = mess_count_case5[1]+3;
                mess_count_case1[3] = mess_count_case1[3]+3;
            }

            if(time_count_cpu1%15 == 0)
            {
                // for(int i=0;i<68;i=i+67)
                // {
                    send_multican_message_gw(3, 9, 8, (uint32 *) &data_8[0], FALSE);
                    send_multican_message_gw(3, 10, 8, (uint32 *) &data_8[0], FALSE);
                // }
                mess_count_case1[2] = mess_count_case1[2]+2;
            }

            if((time_count_cpu1-1)%10 == 0)
            {
                // for(int i=0;i<3;i++)
                // {
                send_multican_message_gw(4, 2, 8, (uint32 *) &data_8[0], FALSE);
                send_multican_message_gw(4, 3, 16, (uint32 *) &data_16[0], FALSE);
                send_multican_message_gw(4, 6, 64, (uint32 *) &data_64[0], FALSE);
                send_multican_message_gw(4, 7, 20, (uint32 *) &data_20[0], FALSE);
                send_multican_message_gw(4, 10, 8, (uint32 *) &data_8[0], FALSE);
                // }
                send_multican_message_gw(2, 2, 8, (uint32 *) &data_64[0], FALSE);
                send_multican_message_gw(2, 4, 20, (uint32 *) &data_20[0], FALSE);
                send_multican_message_gw(2, 9, 64, (uint32 *) &data_64[0], FALSE);
                send_multican_message_gw(2, 11, 20, (uint32 *) &data_20[0], FALSE);
                send_multican_message_gw(2, 16, 64, (uint32 *) &data_64[0], FALSE);
                send_multican_message_gw(2, 18, 20, (uint32 *) &data_20[0], FALSE);
                // for(int i=0;i<68;i=i+67)
                // {
                    for(int g = 2;g<9;g++)
                    {
                        send_multican_message_gw(3, g, 8, (uint32 *) &data_8[0], FALSE);
                    }
                // }
                
                mess_count_case4[1]= mess_count_case4[1]+5;
                mess_count_case5[0] = mess_count_case5[0]+6;
                mess_count_case1[1] = mess_count_case1[1]+7;
            }
            
             if((time_count_cpu1-1)%5 == 0)
            {
                // for(int i=0;i<3;i++)
                // {
                    send_multican_message_gw(4, 1, 8, (uint32 *) &data_8[0], FALSE);
                // }
                // for(int i=0;i<68;i=i+67)
                // {
                    send_multican_message_gw(3, 0, 8, (uint32 *) &data_8[0], FALSE);
                    send_multican_message_gw(3, 1, 8, (uint32 *) &data_8[0], FALSE);
                // }
                
                mess_count_case4[0] = mess_count_case4[0]+1;
                mess_count_case1[0] = mess_count_case1[0]+2;
            }
            if(time_count_cpu1==total_time)
            {
                int total_4 = 0;
                int total_2 = 0;
                int total_1 = 0;
                 for(int b = 0;b < 6; b++)
                {
                    total_4 = total_4 + mess_count_case4[b];
                    Ifx_Console_print("count 4 %d: %d\r\n", b,mess_count_case4[b]);
                }
                for(int a = 0;a < 4; a++)
                {
                    total_2 = total_2 + mess_count_case5[a];
                    Ifx_Console_print("count 2 %d: %d\r\n", a,mess_count_case5[a]);
                }
                for(int a = 0;a < 16; a++)
                {
                    total_1 = total_1 + mess_count_case1[a];
                    Ifx_Console_print("count 3 %d: %d\r\n", a,mess_count_case1[a]);
                }
                Ifx_Console_print("Total count 4: %d\r\n",total_4);
                Ifx_Console_print("Total count 2: %d\r\n",total_2);
                Ifx_Console_print("Total count 3: %d\r\n",total_1);
                for(int b = 0;b < 6; b++)
                {
                    mess_count_case4[b]=0;
                }
                for(int b = 0;b < 4; b++)
                {
                    mess_count_case5[b]=0;
                }
                for(int b = 0;b < 16; b++)
                {
                    mess_count_case1[b]=0;
                }
                total_4 = 0;
                total_2 = 0;
                total_1 = 0;
                time_count_cpu1 = 0;
                cpu1_start_send_can = FALSE;
            }

            ++time_count_cpu1;
        }
	    }
}
