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
#include "SysSe/Bsp/Bsp.h"
#include "SysSe/Comm/Ifx_Shell.h"
#include <_Impl/IfxPort_cfg.h>
#include "GpioCmds.h"
#include "Gpio.h"
#include "resource_state_manage.h"
#include "board.h"
#include "Gpio_ctrl_3570.h"
#include "TegraQspi.h"
#include "resource.h"
//#if PROJECT == PROJECT_DRIVE_AX
//#if DRIVE_AX_B0x == 1
#if 1
#if 1
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
/* we need consistent output log for the command result (for automation) */
#define PRINT_SUCCESSFUL_LOG  IfxStdIf_DPipe_print(io, "Command Executed"ENDL);
#define PRINT_FAIL_LOG        IfxStdIf_DPipe_print(io, "Command Fail "ENDL);
/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
volatile boolean display_thermal_status = FALSE;
boolean recovery_flag = FALSE;
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
/** \brief Handle the 'showport' command
 *
 * \par Syntax
 *  - showport x y: show the state of Px.y
 */

boolean Show_PortState(pchar args, void *data, IfxStdIf_DPipe *io)
{
	Ifx_P *pModule;
	uint32 uiPort, uiPin, uiCnt;
	uint8 pDir;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : showport x y"ENDL);
        IfxStdIf_DPipe_print(io, "           > Show the state of port x.y"ENDL);
        return TRUE;
    }
	// first we get the first argument
    uiPort = 0;
    uiPin = 0;
	if (Ifx_Shell_parseUInt32(&args, &uiPort, FALSE) == FALSE)
   	{
   		IfxStdIf_DPipe_print(io, "Syntaxerror : invalid port %s"ENDL, args);
   		return TRUE;
   	}
   	for (uiCnt=0; uiCnt<IFXPORT_NUM_MODULES; uiCnt++)
   	{
   		if (IfxPort_cfg_indexMap[uiCnt].index == uiPort)
   		{
   			pModule = IfxPort_cfg_indexMap[uiCnt].module;
   			break;
   		}
    }
    if (uiCnt == IFXPORT_NUM_MODULES)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid port %d"ENDL, uiPort);
        return TRUE;
    }
	// now we get the second argument
	if (Ifx_Shell_parseUInt32(&args, &uiPin, FALSE) == FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid pin %s"ENDL, args);
        return TRUE;
    }
    if (uiPin > 15)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid pin %d"ENDL, uiPin);
        return TRUE;
    }

    IfxStdIf_DPipe_print(io, "P%02d.%d: ", uiPort, uiPin);
    // we add this for dax tc397
    pDir = IfxPort_getPinDirection(pModule, uiPin);
    if (pDir & 0x10)
    {
        IfxStdIf_DPipe_print(io, "Direction = OUT; ");
        if (pDir & 0x8)
        {
        	IfxStdIf_DPipe_print(io, "Open Drain; ");
        }
        else
        {
        	IfxStdIf_DPipe_print(io, "Push Pull; ");
        }
        if (!(pDir & 0x7))
        {
        	IfxStdIf_DPipe_print(io, "General-purpose output; ");
        }
        else
        {
        	IfxStdIf_DPipe_print(io, "Alternate output; %d ",pDir&0x7);
        }

    }
    else
    {
        IfxStdIf_DPipe_print(io, "Direction = IN; ");
        if ((pDir & 0x3) == 0x1)
        {
        	IfxStdIf_DPipe_print(io, "PD; ");
        }
        if ((pDir & 0x3) == 0x2)
        {
        	IfxStdIf_DPipe_print(io, "PU; ");
        }

    }
    if (IfxPort_getPinState(pModule, uiPin) == TRUE)
        IfxStdIf_DPipe_print(io, "State = HIGH "ENDL);
    else
        IfxStdIf_DPipe_print(io, "State = LOW "ENDL);

    return TRUE;
}

boolean Show_PortState_all(pchar args, void *data, IfxStdIf_DPipe *io)
{
	char l_args[10];
	uint8 port = 0;
	uint8 pin = 0;
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
	{
		IfxStdIf_DPipe_print(io, "Syntax     : showportall"ENDL);
		IfxStdIf_DPipe_print(io, "           > show all the state of GPIO device"ENDL);
		return TRUE;
	}
	else
	{
//		for(device = 0; device <NUM_GPIO_EXP_DEVICES; device++)
		{
			for(port =0; port <1; port++)
			{
				for(pin=0; pin<=12; pin++)
				{
					sprintf(l_args, "%d %d",port, pin);
					AsclinShellInterface_execute_cmd("showport", l_args);
				}
			}


			for(port =1; port <2; port++)
			{
				for(pin=3; pin<=7; pin++)
				{
					sprintf(l_args, "%d %d",port, pin);
					AsclinShellInterface_execute_cmd("showport", l_args);
				}
			}

			for(port =2; port <3; port++)
			{
				for(pin=0; pin<=11; pin++)
				{
					sprintf(l_args, "%d %d",port, pin);
					AsclinShellInterface_execute_cmd("showport", l_args);
				}
			}		

			for(port =10; port <11; port++)
			{
				for(pin=0; pin<=8; pin++)
				{
					sprintf(l_args, "%d %d",port, pin);
					AsclinShellInterface_execute_cmd("showport", l_args);
				}
			}	


			for(port =11; port <12; port++)
			{
				for(pin=0; pin<=15; pin++)
				{
					sprintf(l_args, "%d %d",port, pin);
					AsclinShellInterface_execute_cmd("showport", l_args);
				}
			}	

			for(port =12; port <13; port++)
			{
				for(pin=0; pin<=1; pin++)
				{
					sprintf(l_args, "%d %d",port, pin);
					AsclinShellInterface_execute_cmd("showport", l_args);
				}
			}

			for(port =13; port <14; port++)
			{
				for(pin=0; pin<=3; pin++)
				{
					sprintf(l_args, "%d %d",port, pin);
					AsclinShellInterface_execute_cmd("showport", l_args);
				}
			}

			for(port =14; port <15; port++)
			{
				for(pin=0; pin<=10; pin++)
				{
					sprintf(l_args, "%d %d",port, pin);
					AsclinShellInterface_execute_cmd("showport", l_args);
				}
			}
			for(port =15; port <16; port++)
			{
				for(pin=0; pin<=8; pin++)
				{
					sprintf(l_args, "%d %d",port, pin);
					AsclinShellInterface_execute_cmd("showport", l_args);
				}
			}

			for(port =20; port <21; port++)
			{
				for(pin=0; pin<=14; pin++)
				{
					sprintf(l_args, "%d %d",port, pin);
					AsclinShellInterface_execute_cmd("showport", l_args);
				}
			}


			for(port =21; port <22; port++)
			{
				for(pin=0; pin<=5; pin++)
				{
					sprintf(l_args, "%d %d",port, pin);
					AsclinShellInterface_execute_cmd("showport", l_args);
				}
			}		

			for(port =22; port <23; port++)
			{
				for(pin=0; pin<=11; pin++)
				{
					sprintf(l_args, "%d %d",port, pin);
					AsclinShellInterface_execute_cmd("showport", l_args);
				}
			}	

			for(port =23; port <24; port++)
			{
				for(pin=0; pin<=7; pin++)
				{
					sprintf(l_args, "%d %d",port, pin);
					AsclinShellInterface_execute_cmd("showport", l_args);
				}
			}	

			for(port =32; port <33; port++)
			{
				for(pin=0; pin<=7; pin++)
				{
					sprintf(l_args, "%d %d",port, pin);
					AsclinShellInterface_execute_cmd("showport", l_args);
				}
			}	

			for(port =34; port <35; port++)
			{
				for(pin=0; pin<=5; pin++)
				{
					sprintf(l_args, "%d %d",port, pin);
					AsclinShellInterface_execute_cmd("showport", l_args);
				}
			}	
		}
	}

	return TRUE;
}



/** \brief Handle the 'setport' command
 *
 * \par Syntax
 *  - setport x y z: set the state of Px.y to state z (0/1)
 */
boolean Set_PortState(pchar args, void *data, IfxStdIf_DPipe *io)
{
	Ifx_P *pModule;
	uint32 uiPort, uiPin, uiValue, uiCnt;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : setport x y z"ENDL);
        IfxStdIf_DPipe_print(io, "           > Set the state of Px.y to state z (0/1)"ENDL);
        return TRUE;
    }
	// first we get the first argument
    uiPort = 0;
    uiPin = 0;
	if (Ifx_Shell_parseUInt32(&args, &uiPort, FALSE) == FALSE)
   	{
   		IfxStdIf_DPipe_print(io, "Syntaxerror : invalid port %s"ENDL, args);
   		return TRUE;
   	}
   	for (uiCnt=0; uiCnt<IFXPORT_NUM_MODULES; uiCnt++)
   	{
   		if (IfxPort_cfg_indexMap[uiCnt].index == uiPort)
   		{
   			pModule = IfxPort_cfg_indexMap[uiCnt].module;
   			break;
   		}
    }
    if (uiCnt == IFXPORT_NUM_MODULES)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid port %d"ENDL, uiPort);
        return TRUE;
    }
	// now we get the second argument
	if (Ifx_Shell_parseUInt32(&args, &uiPin, FALSE) == FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid pin %s"ENDL, args);
        return TRUE;
    }
    if (uiPin > 15)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid pin %d"ENDL, uiPin);
        return TRUE;
    }

	// now we get the third argument
	if (Ifx_Shell_parseUInt32(&args, &uiValue, FALSE) == FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value %s"ENDL, args);
        return TRUE;
    }
    if (uiValue > 1)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value %d (only 0 and 1)"ENDL, uiValue);
        return TRUE;
    }

    if (uiValue == 0)
    	IfxPort_setPinLow(pModule, uiPin);
    else
    	IfxPort_setPinHigh(pModule, uiPin);

    return TRUE;
}

/** \brief Handle the 'configport' command
 *
 * \par Syntax
 *  - configport x y in %/pu/pd : configure Px.y to input with pull device if specified
 *  - configport x y out z %/od: configure Px.y to output z (0=general,1..7=alternate) and open drain if specified
 */


boolean Config_Port(pchar args, void *data, IfxStdIf_DPipe *io)
{
	Ifx_P *pModule;
	uint32 uiPort, uiPin, uiValue, uiCnt;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : configport x y in [pu/pd] [pdr]"ENDL);
        IfxStdIf_DPipe_print(io, "           > Configure Px.y to input with pull device (optional)"ENDL);
        IfxStdIf_DPipe_print(io, "             and set pdr value (optional)"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax     : configport x y out z [od] [pdr]"ENDL);
        IfxStdIf_DPipe_print(io, "           > Configure Px.y to output z (0=gen,1..7=alt) and open drain (optional)"ENDL);
        IfxStdIf_DPipe_print(io, "             and set pdr value (optional)"ENDL);
        return TRUE;
    }
	// first we get the first argument
    uiPort = 0;
    uiPin = 0;
	if (Ifx_Shell_parseUInt32(&args, &uiPort, FALSE) == FALSE)
   	{
   		IfxStdIf_DPipe_print(io, "Syntaxerror : invalid port %s"ENDL, args);
   		return TRUE;
   	}
   	for (uiCnt=0; uiCnt<IFXPORT_NUM_MODULES; uiCnt++)
   	{
   		if (IfxPort_cfg_indexMap[uiCnt].index == uiPort)
   		{
   			pModule = IfxPort_cfg_indexMap[uiCnt].module;
   			break;
   		}
    }
    if (uiCnt == IFXPORT_NUM_MODULES)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid port %d"ENDL, uiPort);
        return TRUE;
    }
	// now we get the second argument
	if (Ifx_Shell_parseUInt32(&args, &uiPin, FALSE) == FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid pin %s"ENDL, args);
        return TRUE;
    }
    if (uiPin > 15)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid pin %d"ENDL, uiPin);
        return TRUE;
    }

	// now we get the third argument
    if (Ifx_Shell_matchToken(&args, "in") != FALSE)
    {
    	// we specify an input
    	IfxPort_InputMode mode;

    	mode = IfxPort_InputMode_noPullDevice;
    	// check if pu specified
        if (Ifx_Shell_matchToken(&args, "pu") != FALSE) mode = IfxPort_InputMode_pullUp;
        else
            if (Ifx_Shell_matchToken(&args, "pd") != FALSE) mode = IfxPort_InputMode_pullDown;

        IfxPort_setPinModeInput(pModule, uiPin, mode);

    	if (Ifx_Shell_parseUInt32(&args, &uiValue, FALSE) != FALSE)
    	{
    		if (uiValue < 12) IfxPort_setPinPadDriver(pModule, uiPin, uiValue);
    		else IfxStdIf_DPipe_print(io, "invalid pdr value %s (pdr not set)"ENDL, uiValue);
    	}
    }
    else
    {
        if (Ifx_Shell_matchToken(&args, "out") != FALSE)
        {
        	IfxPort_OutputIdx index;
        	if (Ifx_Shell_parseUInt32(&args, &uiValue, FALSE) == FALSE)
            {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid out value %s"ENDL, args);
                return TRUE;
            }
        	switch (uiValue)
        	{
        	case 0: index = IfxPort_OutputIdx_general;
        	        break;
        	case 1: index = IfxPort_OutputIdx_alt1;
        	        break;
        	case 2: index = IfxPort_OutputIdx_alt2;
        	        break;
        	case 3: index = IfxPort_OutputIdx_alt3;
        	        break;
        	case 4: index = IfxPort_OutputIdx_alt4;
        	        break;
        	case 5: index = IfxPort_OutputIdx_alt5;
        	        break;
        	case 6: index = IfxPort_OutputIdx_alt6;
        	        break;
        	case 7: index = IfxPort_OutputIdx_alt7;
        	        break;
        	default:
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid out value %d"ENDL, uiValue);
                return TRUE;

        	}

            if (Ifx_Shell_matchToken(&args, "od") != FALSE)
            	IfxPort_setPinModeOutput(pModule, uiPin, IfxPort_OutputMode_openDrain, index);
            else
            	IfxPort_setPinModeOutput(pModule, uiPin, IfxPort_OutputMode_pushPull, index);

            if (Ifx_Shell_parseUInt32(&args, &uiValue, FALSE) != FALSE)
        	{
        		if (uiValue < 12) IfxPort_setPinPadDriver(pModule, uiPin, uiValue);
        		else IfxStdIf_DPipe_print(io, "invalid pdr value %s (pdr not set)"ENDL, uiValue);
        	}
        }
        else
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid type (only in and out) %s"ENDL, args);
            return TRUE;
        }
    }

    return TRUE;
}

/** \brief Handle the 'poweroff' command
 *
 * \par Syntax
 *  - poweroff : set the outputs to system power off state
 */
boolean System_PowerOff_Cmd(pchar args, void *data, IfxStdIf_DPipe *io)
{
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : poweroff"ENDL);
        IfxStdIf_DPipe_print(io, "           > Set outputs to system power off state"ENDL);
    }
    else
    {
        sem_wait_until_get(SEM_SPI_2, __LINE__, __func__);
        if(System_PowerOff() == TRUE)
        {
            PRINT_SUCCESSFUL_LOG;
        }
        else
        {
            PRINT_FAIL_LOG;
        }
        sem_release(SEM_SPI_2);
    }

    return TRUE;
}


/** \brief Handle the 'poweron' command
 *
 * \par Syntax
 *  - poweron : turn main power on and release Tegra Resets
 */
boolean System_PowerOn_Cmd(pchar args, void *data, IfxStdIf_DPipe *io)
{
	boolean isForcePoweron = FALSE;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : poweron [f]"ENDL);
        IfxStdIf_DPipe_print(io, "           > Turn main power on and release Tegra resets"ENDL);
        IfxStdIf_DPipe_print(io, "           > Optionally, use f to force poweron without checking Power Good"ENDL);
    }
    else
    {
        if (Ifx_Shell_matchToken(&args, "f") != FALSE)
        {
            isForcePoweron = TRUE;
        }
        sem_wait_until_get(SEM_SPI_2, __LINE__, __func__);
        //need to measure time
        if(System_PowerOn(isForcePoweron) == TRUE)
        {
            PRINT_SUCCESSFUL_LOG;
        }
        else
        {
            PRINT_FAIL_LOG;
        }
        sem_release(SEM_SPI_2);
    }
    return TRUE;
}
/**
 *
 * \par Syntax
 *  - recovery a/b on/off : set Tegra A/B recovery mode on/off
 */

boolean TegraRecovery(pchar args, void *data, IfxStdIf_DPipe *io)
{
	IfxPort_Pin *pPort;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : tegrarecovery x1/x2/cvm on/off"ENDL);
        IfxStdIf_DPipe_print(io, "           > Set Tegra recovery mode on/off for x1/x2/cvm"ENDL);
    }
    else
    {
        // first we get the device selection
        if ((Ifx_Shell_matchToken(&args, "x1") != FALSE) || (Ifx_Shell_matchToken(&args, "X1") != FALSE))
        {
            pPort = &(X1_FORCE_RCVR_R_N);
        }
        else if ((Ifx_Shell_matchToken(&args, "x2") != FALSE) || (Ifx_Shell_matchToken(&args, "X2") != FALSE))
        {
            pPort = &(X2_FORCE_RCVR_R_N);
        }
        else if ((Ifx_Shell_matchToken(&args, "cvm") != FALSE) || (Ifx_Shell_matchToken(&args, "CVM") != FALSE))
        {

        	if(board.project == E3550)
        	{
        		pPort = &(E3550_P1_FORCE_RCVR_N);
            }
        	else
        	{
        		IfxStdIf_DPipe_print(io, "Error : invalid tegra %s"ENDL, args);
				PRINT_FAIL_LOG;
				return TRUE;
        	}

        }
        else
        {
            IfxStdIf_DPipe_print(io, "Error : invalid tegra %s"ENDL, args);
            PRINT_FAIL_LOG;
            return TRUE;
        }

        if (Ifx_Shell_matchToken(&args, "on") != FALSE)
        {
            IfxPort_setPinLow(pPort->port, pPort->pinIndex); // Recovery ON
        }
        else
        {
            if (Ifx_Shell_matchToken(&args, "off") != FALSE)
            {
                IfxPort_setPinHigh(pPort->port, pPort->pinIndex); // Recovery OFF
            }
            else
            {
                IfxStdIf_DPipe_print(io, "Error : invalid recovery state %s"ENDL, args);
                PRINT_FAIL_LOG;
                return TRUE;
            }
        }
        PRINT_SUCCESSFUL_LOG;
    }
    return TRUE;
}
/**
 *
 * \par Syntax
 *  - tegrareset a/b: reset Tegra A/B
 */


boolean TegraReset(pchar args, void *data, IfxStdIf_DPipe *io)
{
	boolean holdreset = FALSE;
	boolean cvmflag = FALSE;
	IfxPort_Pin *pPort;

	if(is_PowerOff())
	{
		IfxStdIf_DPipe_print(io, "System powered off!"ENDL);
		PRINT_FAIL_LOG;
		return FALSE;
	}
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : tegrareset x1/x2/cvm [h]"ENDL);
        IfxStdIf_DPipe_print(io, "           > reset Tegra x1/x2/cvm "ENDL);
        IfxStdIf_DPipe_print(io, "             and hold reset low (optional)"ENDL);
    }
    else
    {
        // first we get the device selection
        if ((Ifx_Shell_matchToken(&args, "x1") != FALSE) || (Ifx_Shell_matchToken(&args, "X1") != FALSE))
        {
            pPort = &(AURIX_XA_RST_N);
        }
        else if ((Ifx_Shell_matchToken(&args, "x2") != FALSE) || (Ifx_Shell_matchToken(&args, "X2") != FALSE))
        {
            pPort = &(AURIX_XB_RST_N);
        }
        else if ((Ifx_Shell_matchToken(&args, "cvm") != FALSE) || (Ifx_Shell_matchToken(&args, "CVM") != FALSE))
        {

        	if(board.project == E3550)
        	{
        		pPort = &(E3550_AURIX_P1_RST_N);
        		cvmflag = TRUE;
        	}
        	else
        	{
        		IfxStdIf_DPipe_print(io, "Error : invalid tegra %s"ENDL, args);
        		PRINT_FAIL_LOG;
        		return TRUE;
        	}

        }
        else
        {
            IfxStdIf_DPipe_print(io, "Error : invalid tegra %s"ENDL, args);
            PRINT_FAIL_LOG;
            return TRUE;
        }

        if (Ifx_Shell_matchToken(&args, "h") != FALSE)
        {
            holdreset = TRUE;
            if(cvmflag)
                pmic_c_on = FALSE; // CVM reset pin connected to reset of PMIC
        }

        IfxPort_setPinLow(pPort->port, pPort->pinIndex);
        if(!holdreset)
        {
            waitTime(500*TimeConst[TIMER_INDEX_1MS]); // this is just a placeholder value for delay
            IfxPort_setPinHigh(pPort->port, pPort->pinIndex);
            if(cvmflag)
            {
                pmic_c_on = TRUE;
                waitTime(200*TimeConst[TIMER_INDEX_1MS]);
                toggle_levelShifter();
            }
        }
        PRINT_SUCCESSFUL_LOG;
    }
    return TRUE;
}

/**
 *
 * \par Syntax
 *  - aurix2reset : reset the second aurix
 */
boolean Aurix2Reset(pchar args, void *data, IfxStdIf_DPipe *io)
{
    boolean holdreset = FALSE;
    IfxPort_Pin *pPort;

    if(board.project != P3570)
    {
	IfxStdIf_DPipe_print(io, "No Second Aurix on board!"ENDL);
	PRINT_FAIL_LOG;
	return TRUE;
    }

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : aurix2reset [h]"ENDL);
        IfxStdIf_DPipe_print(io, "           > reset second aurix "ENDL);
        IfxStdIf_DPipe_print(io, "             and hold reset low (optional)"ENDL);
    }
    else
    {
	if (Ifx_Shell_matchToken(&args, "h") != FALSE)
		holdreset = TRUE;
	pPort = &(P3570_AURIX2_RST_N);
	IfxPort_setPinLow(pPort->port, pPort->pinIndex);
	if(!holdreset)
	{
		waitTime(500*TimeConst[TIMER_INDEX_1MS]); // this is just a placeholder value for delay
		IfxPort_setPinHigh(pPort->port, pPort->pinIndex);
	}
	PRINT_SUCCESSFUL_LOG;
    }
    return TRUE;
}

/**
 *
 * \par Syntax
 *  - aurix2recovery : set second aurix recovery mode on/off
 */
boolean Aurix2Recovery(pchar args, void *data, IfxStdIf_DPipe *io)
{
    IfxPort_Pin *pPort;

    if(board.project != P3570)
    {
        IfxStdIf_DPipe_print(io, "No Second Aurix on board!"ENDL);
        PRINT_FAIL_LOG;
        return TRUE;
    }
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : aurix2recovery on/off"ENDL);
        IfxStdIf_DPipe_print(io, "           > Set Second Aurix recovery mode on/off"ENDL);
    }
    else
    {
    	pPort = &(P3570_AURIX2_RCVR_N);
        if (Ifx_Shell_matchToken(&args, "on") != FALSE)
        {
            IfxPort_setPinLow(pPort->port, pPort->pinIndex); // Recovery ON
            is_aurix2_recy_on = TRUE;
        }
        else if (Ifx_Shell_matchToken(&args, "off") != FALSE)
        {
            IfxPort_setPinHigh(pPort->port, pPort->pinIndex); // Recovery OFF
            is_aurix2_recy_on = FALSE;
        }
        else
        {
            IfxStdIf_DPipe_print(io, "Error : invalid recovery state %s"ENDL, args);
            PRINT_FAIL_LOG;
            return TRUE;
        }
    }
    PRINT_SUCCESSFUL_LOG;
    return TRUE;
}

/**
 *
 * \par Syntax
 *  - Aurix2Flash : WAR to prepare aurix2 into recovery mode
 */
boolean Aurix2Flash(pchar args, void *data, IfxStdIf_DPipe *io)
{
    if(board.project != P3570)
    {
        IfxStdIf_DPipe_print(io, "No Second Aurix on board!"ENDL);
        PRINT_FAIL_LOG;
        return TRUE;
    }
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : aurix2flash"ENDL);
        IfxStdIf_DPipe_print(io, "           > WAR: Prepare Aurix2 into recovery mode"ENDL);

    }
    else
    {
    	IfxStdIf_DPipe_print(io, "Notice: this command will power off the whole board!"ENDL);
    	IfxStdIf_DPipe_print(io, "After back drive is fix, this command will not be supported"ENDL);

    	IfxPort_setPinLow(P3570_AURIX2_RCVR_N.port, P3570_AURIX2_RCVR_N.pinIndex);
    	is_aurix2_recy_on = TRUE;
    	System_PowerOff();
    	waitTime(100*TimeConst[TIMER_INDEX_1MS]);

    	IfxPort_setPinHigh(P3570_AURIX2_PWR_ON.port, P3570_AURIX2_PWR_ON.pinIndex);
    	waitTime(10*TimeConst[TIMER_INDEX_1MS]);
    	IfxPort_setPinHigh(VBAT_SYS_PWR_ON.port, VBAT_SYS_PWR_ON.pinIndex);
    	waitTime(10*TimeConst[TIMER_INDEX_1MS]);
    	IfxPort_setPinHigh(P3570_AURIX2_RST_N.port, P3570_AURIX2_RST_N.pinIndex);
    	waitTime(10*TimeConst[TIMER_INDEX_1MS]);
    	is_aurix2_recy_on = FALSE;
    	IfxStdIf_DPipe_print(io, "aurix2 is in recovery mode"ENDL);
    	IfxStdIf_DPipe_print(io, "Please run \"aurixreset\" to recover the board after flashing aurix2"ENDL);
    }
    PRINT_SUCCESSFUL_LOG;
    return TRUE;
}

#if 0

/**
 *
 * \par Syntax
 *  - campower a/b/c on/off: turn on/off group A/B/C camera Power
 */
boolean CamPower(pchar args, void *data, IfxStdIf_DPipe *io)
{
	eCameraModule cam_module;
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : campower a/b/c/d/e on/off"ENDL);
        IfxStdIf_DPipe_print(io, "           > turn on/off Group A/B/C/D/E Camera Power Enable"ENDL);
    }
    else
    {
	// first we get the device selection
        if ((Ifx_Shell_matchToken(&args, "a") != FALSE) || (Ifx_Shell_matchToken(&args, "A") != FALSE))
        {
#if CAMERA_A_ENABLE == 1
		cam_module = CAMERA_MODULE_A;
#else
		IfxStdIf_DPipe_print(io, "Error : invalid Group A"ENDL);
		return TRUE;
#endif
        }
        else if ((Ifx_Shell_matchToken(&args, "b") != FALSE) || (Ifx_Shell_matchToken(&args, "B") != FALSE))
        {
#if CAMERA_B_ENABLE == 1
		cam_module = CAMERA_MODULE_B;
#else
		IfxStdIf_DPipe_print(io, "Error : invalid Group B"ENDL);
		return TRUE;
#endif
        }
        else if ((Ifx_Shell_matchToken(&args, "c") != FALSE) || (Ifx_Shell_matchToken(&args, "C") != FALSE))
        {
#if CAMERA_C_ENABLE == 1
		cam_module = CAMERA_MODULE_C;
#else
		IfxStdIf_DPipe_print(io, "Error : invalid Group C"ENDL);
		return TRUE;
#endif

        }
        else if ((Ifx_Shell_matchToken(&args, "d") != FALSE) || (Ifx_Shell_matchToken(&args, "D") != FALSE))
        {
#if CAMERA_D_ENABLE == 1
		cam_module = CAMERA_MODULE_D;
#else
		IfxStdIf_DPipe_print(io, "Error : invalid Group C"ENDL);
		return TRUE;
#endif

        }
        else if ((Ifx_Shell_matchToken(&args, "e") != FALSE) || (Ifx_Shell_matchToken(&args, "E") != FALSE))
        {
#if CAMERA_E_ENABLE == 1
		cam_module = CAMERA_MODULE_E;
#else
		IfxStdIf_DPipe_print(io, "Error : invalid Group E"ENDL);
		return TRUE;
#endif
        }
        else
        {
        	IfxStdIf_DPipe_print(io, "Error : invalid Group %s"ENDL, args);
        	return TRUE;
        }

		if (Ifx_Shell_matchToken(&args, "on") != FALSE)
		{
			Resource_SetCameraState(cam_module, CAMERA_ON);
		}
		else if (Ifx_Shell_matchToken(&args, "off") != FALSE)
		{
			Resource_SetCameraState(cam_module, CAMERA_OFF);
		}
		else
		{
			IfxStdIf_DPipe_print(io, "Error : invalid Camera Power state %s"ENDL, args);
			return TRUE;
		}
		IfxStdIf_DPipe_print(io, "Command Executed"ENDL);
	}
    return TRUE;
}
#endif


//U47
static t_pgood_signals P3570_PGOOD_SIGNALS[]=
{
	{&IoExpPort_A74_P00, "XA_5V_PG", TRUE},
	{&IoExpPort_A74_P01, "N/A", FALSE},
	{&IoExpPort_A74_P02, "X1_PG/XA_VDD_[SOC/DDR2/DDRQ]_PG", TRUE},
	{&IoExpPort_A74_P03, "XA_VDD_[CPU/GPU/CV]_PG", TRUE},
	{&IoExpPort_A74_P04, "XB_5V_PG", TRUE},
	{&IoExpPort_A74_P05, "N/A", FALSE},
	{&IoExpPort_A74_P06, "X2_PG/XB_VDD_[SOC/DDR2/DDRQ]_PG", TRUE},
	{&IoExpPort_A74_P07, "XB_VDD_[CPU/GPU/CV]_PG", TRUE},

	{&IoExpPort_A74_P10, "SYS_5V_PG", TRUE},
	{&IoExpPort_A74_P11, "SYS_[0V85/1V0/1V2]_PG(A00) or CAM_1V2_PG(A01)", TRUE},
	{&IoExpPort_A74_P12, "SYS_[3V3/1V8/0V9]_PG1(A00) or CAM_1V2_PG(A01)", TRUE},
	{&IoExpPort_A74_P13, "SYS_[3V3/1V8/1V5/1V1/0V9]_PG2", TRUE},
	{&IoExpPort_A74_P14, "N/A", FALSE},
	{&IoExpPort_A74_P15, "N/A", FALSE},
	{&IoExpPort_A74_P16, "N/A", FALSE},
	{&IoExpPort_A74_P17, "N/A", FALSE},
};


/** \brief Handle the 'ShowPgood' command
 *
 * \par Syntax
 *  - pgood status - Show Power Good Status
 */
boolean ShowPgood(pchar args, void *data, IfxStdIf_DPipe *io)
{
	t_pgood_signals* ptr;
	uint8 i;
	boolean current_state = FALSE;

    ptr = &P3570_PGOOD_SIGNALS[0];


	for(i=0; i<16; i++)
	{
		if(ptr[i].is_connected)
		{
			current_state = READ_GPIOEXP_STATE((*(ptr[i].pin)));
			if (current_state == TRUE)
				IfxStdIf_DPipe_print(io, "%s = HIGH "ENDL, ptr[i].name);
			else
				IfxStdIf_DPipe_print(io, "%s = LOW "ENDL, ptr[i].name);
		}
	}
    return TRUE;
}


/** \brief Handle the 'thermstatus' command
 *
 * \par Syntax
 *  - thermstatus - Show Thermal Status
 */
boolean ThermStatus(pchar args, void *data, IfxStdIf_DPipe *io)
{

	display_thermal_status = TRUE;
	Print_Thermal_Status(io);
    return TRUE;
}


/** \brief Handle the 'inactive' command
 *
 * \par Syntax
 *  - inactive : set the outputs to system inactive state
 */
boolean System_InActive(pchar args, void *data, IfxStdIf_DPipe *io)
{
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : inactive"ENDL);
        IfxStdIf_DPipe_print(io, "           > Set outputs to system inactive state"ENDL);
    }
    else
    {
        SetOutputs_to_System_Inactive_State();
    }

    return TRUE;
}

void Print_Thermal_Status(IfxStdIf_DPipe *io)
{
	IfxPort_Pin *pPort;

	if(display_thermal_status)
	{
		IfxStdIf_DPipe_print(io, ENDL);
		//Read BRD_THERM_ALERT
		if(! whole_board_off)
		{
				if (brd_alert == TRUE)
					IfxStdIf_DPipe_print(io, "Board Thermal Alert 1 = HIGH "ENDL);
				else
					IfxStdIf_DPipe_print(io, "Board Thermal Alert 1 = LOW "ENDL);
				if (brd_alert_2 == TRUE)
					IfxStdIf_DPipe_print(io, "Board Thermal Alert 2 = HIGH "ENDL);
				else
					IfxStdIf_DPipe_print(io, "Board Thermal Alert 2 = LOW "ENDL);
				if (brd_alert_3 == TRUE)
					IfxStdIf_DPipe_print(io, "Board Thermal Alert 3 = HIGH "ENDL);
				else
					IfxStdIf_DPipe_print(io, "Board Thermal Alert 3 = LOW "ENDL);
		}
		else
		{
			IfxStdIf_DPipe_print(io, "Board Thermal Alert = N/A "ENDL);
		}

		//Check BRD_THERM_SHDN
		if (brd_therm == TRUE)
			IfxStdIf_DPipe_print(io, "Board Thermal Shutdown = HIGH "ENDL);
		else
			IfxStdIf_DPipe_print(io, "Board Thermal Shutdown = LOW "ENDL);

		/* Xavier A */
		if(pmic_a_on)
		{
			//Read X1_THERM_ALERT
			if (tegra_a_alert == TRUE)
				IfxStdIf_DPipe_print(io, "Xavier A Thermal Alert = HIGH "ENDL);
			else
				IfxStdIf_DPipe_print(io, "Xavier A Thermal Alert = LOW "ENDL);
			//Read X1_THERM_SHDN
			if (tegra_a_therm == TRUE)
				IfxStdIf_DPipe_print(io, "Xavier A Thermal Shutdown = HIGH "ENDL);
			else
				IfxStdIf_DPipe_print(io, "Xavier A Thermal Shutdown = LOW "ENDL);
		}
		else
		{
			IfxStdIf_DPipe_print(io, "Xavier A Thermal Alert = N/A "ENDL);
			IfxStdIf_DPipe_print(io, "Xavier A Thermal Shutdown = N/A "ENDL);
		}

		if((board.project == E3550) || (board.project == P3570))
		{
			/* Xavier B */
			if(pmic_b_on)
			{
				//Read X2_THERM_ALERT
				if (tegra_b_alert == TRUE)
					IfxStdIf_DPipe_print(io, "Xavier B Thermal Alert = HIGH "ENDL);
				else
					IfxStdIf_DPipe_print(io, "Xavier B Thermal Alert = LOW "ENDL);
				//Read X2_THERM_SHDN
				if (tegra_b_therm == TRUE)
					IfxStdIf_DPipe_print(io, "Xavier B Thermal Shutdown = HIGH "ENDL);
				else
					IfxStdIf_DPipe_print(io, "Xavier B Thermal Shutdown = LOW "ENDL);
			}
			else
			{
				IfxStdIf_DPipe_print(io, "Xavier B Thermal Alert = N/A "ENDL);
				IfxStdIf_DPipe_print(io, "Xavier B Thermal Shutdown = N/A "ENDL);
			}
			/* DGPU A THERM_ALERT */
			if(dgpu_a_on)
			{
				if(dgpu_a_alert == TRUE)
					IfxStdIf_DPipe_print(io, "DGPU A Thermal Alert = HIGH "ENDL);
				else
					IfxStdIf_DPipe_print(io, "DGPU A Thermal Alert = LOW "ENDL);
			}
			else
			{
				IfxStdIf_DPipe_print(io, "DGPU A Thermal Alert = N/A "ENDL);
			}

			/* DGPU B THERM_ALERT */
			if(dgpu_b_on)
			{
				if(dgpu_b_alert == TRUE)
					IfxStdIf_DPipe_print(io, "DGPU B Thermal Alert = HIGH "ENDL);
				else
					IfxStdIf_DPipe_print(io, "DGPU B Thermal Alert = LOW "ENDL);
			}
			else
			{
				IfxStdIf_DPipe_print(io, "DGPU B Thermal Alert = N/A "ENDL);
			}
		}

		{
			if(xa_ovr_thwn == TRUE)
				IfxStdIf_DPipe_print(io, "OVR Xavier A Thermal Alert = HIGH "ENDL);
			else
				IfxStdIf_DPipe_print(io, "OVR Xavier A Thermal Alert = LOW "ENDL);

			if(xb_ovr_thwn == TRUE)
				IfxStdIf_DPipe_print(io, "OVR Xavier B Thermal Alert = HIGH "ENDL);
			else
				IfxStdIf_DPipe_print(io, "OVR Xavier B Thermal Alert = LOW "ENDL);
		}

		display_thermal_status = FALSE;
	}
}

/**
 *
 * \par Syntax
 *  - bootstrap x1/x2 qspi/emmc/ufs: Configure strap settings for boot medium
 */
boolean SetBootStrap(pchar args, void *data, IfxStdIf_DPipe *io)
{
	IfxPort_Pin *pPort;
	uint32 strap_value = 1; // Strap value as per schematic
	uint8 boot_media_ta;    // Boot Medium (TA) value as per IDrom spec
	uint8 boot_media_tb;    // Boot Medium (TB) value as per IDrom spec
	IoExpPort_Pin strap0, strap1, strap2;

	/* Possible values for strap_value and boot_media_tx
	 * boot_media_tx :  Medium   : strap_value
	 *      0        :   UFS     :     5
	 *      1        :   Qspi    :     1
	 *      2        :   Emmc    :     0
	 */

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : bootstrap x1/x2 qspi/emmc/ufs"ENDL);
        IfxStdIf_DPipe_print(io, "           > Configure strap settings for boot medium for x1/x2"ENDL);
        IfxStdIf_DPipe_print(io, "             as qspi/emmc/ufs"ENDL);
    }
    else
    {
        i2crom_read_boot_media(&boot_media_ta, &boot_media_tb);

        // first we get the device selection
        if ((Ifx_Shell_matchToken(&args, "x1") != FALSE) || (Ifx_Shell_matchToken(&args, "X1") != FALSE))
        {
            strap0 = X1_BSTRP0_EXP;
            strap1 = X1_BSTRP1_EXP;
            strap2 = X1_BSTRP2_EXP;
            // get second argument
            if ((Ifx_Shell_matchToken(&args, "qspi") != FALSE) || (Ifx_Shell_matchToken(&args, "QSPI") != FALSE))
            {
                strap_value = 1;
                boot_media_ta = 1;
            }
            else if ((Ifx_Shell_matchToken(&args, "emmc") != FALSE) || (Ifx_Shell_matchToken(&args, "EMMC") != FALSE))
            {
                strap_value = 0;
                boot_media_ta = 2;
            }
            else if ((Ifx_Shell_matchToken(&args, "ufs") != FALSE) || (Ifx_Shell_matchToken(&args, "UFS") != FALSE))
            {
                strap_value = 5;
                boot_media_ta = 0;
            }
            else
            {
                IfxStdIf_DPipe_print(io, "Error : invalid boot medium for TA %s"ENDL, args);
                return TRUE;
            }
        }
        else if ((Ifx_Shell_matchToken(&args, "x2") != FALSE) || (Ifx_Shell_matchToken(&args, "X2") != FALSE))
        {
            strap0 = X2_BSTRP0_EXP;
            strap1 = X2_BSTRP1_EXP;
            strap2 = X2_BSTRP2_EXP;
            // get second argument
            if ((Ifx_Shell_matchToken(&args, "qspi") != FALSE) || (Ifx_Shell_matchToken(&args, "QSPI") != FALSE))
            {
                strap_value = 1;
                boot_media_tb = 1;
            }
            else if ((Ifx_Shell_matchToken(&args, "emmc") != FALSE) || (Ifx_Shell_matchToken(&args, "EMMC") != FALSE))
            {
                strap_value = 0;
                boot_media_tb = 2;
            }
            else if ((Ifx_Shell_matchToken(&args, "ufs") != FALSE) || (Ifx_Shell_matchToken(&args, "UFS") != FALSE))
            {
                strap_value = 5;
                boot_media_tb = 0;
            }
            else
            {
                IfxStdIf_DPipe_print(io, "Error : invalid boot medium for TB %s"ENDL, args);
                return TRUE;
            }

        }
        else
        {
            IfxStdIf_DPipe_print(io, "Error : invalid tegra %s"ENDL, args);
            return TRUE;
        }

        switch(strap_value)
        {
        case 0:
            SET_GPIOEXP_OUTPUT_LOW(strap0);
            SET_GPIOEXP_OUTPUT_LOW(strap1);
            SET_GPIOEXP_OUTPUT_LOW(strap2);
            break;
        case 1:
            SET_GPIOEXP_OUTPUT_HIGH(strap0);
            SET_GPIOEXP_OUTPUT_LOW(strap1);
            SET_GPIOEXP_OUTPUT_LOW(strap2);
            break;
        case 5:
            SET_GPIOEXP_OUTPUT_HIGH(strap0);
            SET_GPIOEXP_OUTPUT_LOW(strap1);
            SET_GPIOEXP_OUTPUT_HIGH(strap2);
            break;
        default:
            SET_GPIOEXP_OUTPUT_HIGH(strap0);
            SET_GPIOEXP_OUTPUT_LOW(strap1);
            SET_GPIOEXP_OUTPUT_LOW(strap2);
            break;
        }

        /* Save boot medium in inforom */
        i2crom_write_boot_media(boot_media_ta, boot_media_tb);
	}
    return TRUE;
}

#endif //endof #if DRIVE_AX_B0x == 1
#endif //#if PROJECT == PROJECT_DRIVE_AX
