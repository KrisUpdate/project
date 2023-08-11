/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */


#include "SysSe/Comm/Ifx_Shell.h"
#include <I2c.h>
#include "gpioexp.h"
#include "SysSe/Bsp/Bsp.h"
#include "gpioexpcmds.h"
#include <internal_fw_debug.h>
#include "resource_state_manage.h"
#include "resource.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define GPIO_EXP_CMD_DEBUG_ENABLE 0

#if GPIO_EXP_CMD_DEBUG_ENABLE == 1
#define GPIO_EXP_CMD_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define GPIO_EXP_CMD_DEBUG_PRINTF(x)
#endif

static const uint8 IO_EXPANDER_ADDRESS[] =
{
//#if DRIVE_AX_B0x == 1
GPIO_EXP0_I2C_ADDR,
GPIO_EXP1_I2C_ADDR,
GPIO_EXP2_I2C_ADDR,
GPIO_EXP3_I2C_ADDR
//#else /* ACH, DDPX 0.75 */
//GPIO_EXP0_I2C_ADDR,
//GPIO_EXP1_I2C_ADDR
//#endif
};

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
static void print_address_help_message(IfxStdIf_DPipe *io)
{
//#if DRIVE_AX_B0x == 1
    IfxStdIf_DPipe_print(io, "           > device 0 => slave addr 0x74 "ENDL);
    IfxStdIf_DPipe_print(io, "           > device 1 => slave addr 0x75 "ENDL);
    IfxStdIf_DPipe_print(io, "           > device 2 => slave addr 0x76 "ENDL);
    IfxStdIf_DPipe_print(io, "           > device 3 => slave addr 0x77 "ENDL);
//#else
//    IfxStdIf_DPipe_print(io, "           > device 0 => slave addr 0x74 "ENDL);
//    IfxStdIf_DPipe_print(io, "           > device 1 => slave addr 0x77 "ENDL);
//#endif
}

boolean Show_ExpPortState_all(pchar args, void *data, IfxStdIf_DPipe *io)
{
	char l_args[10];
	uint8 device = 0;
	uint8 port = 0;
	uint8 pin = 0;
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
	{
		IfxStdIf_DPipe_print(io, "Syntax     : showportexpall"ENDL);
		IfxStdIf_DPipe_print(io, "           > show all the state of GPIO expansion device"ENDL);
		return TRUE;
	}
	else
	{
		for(device = 0; device <NUM_GPIO_EXP_DEVICES; device++)
		{
			for(port =0; port <2; port++)
			{
				for(pin=0; pin<8; pin++)
				{
					sprintf(l_args, "%d %d %d", device, port, pin);
					AsclinShellInterface_execute_cmd("showportexp", l_args);
				}
			}
		}
	}

	return TRUE;
}

/** \brief Handle the 'showportexp' command
 *
 * \par Syntax
 *  - showportexp x y z: show the state of expansion device x group y port z
 */
boolean Show_ExpPortState(pchar args, void *data, IfxStdIf_DPipe *io)
{
	uint32 uiPort, uiDevice, uiGroup;
	uint8 i2caddress;
	boolean direction, current_state;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : showportexp x y z"ENDL);
        IfxStdIf_DPipe_print(io, "           > show the state of GPIO expansion device x group y port z"ENDL);
        print_address_help_message(io);
        return TRUE;
    }
	// first we get the first argument
    uiDevice = 0;
    uiPort = 0;

	if (Ifx_Shell_parseUInt32(&args, &uiDevice, FALSE) == FALSE)
   	{
   		IfxStdIf_DPipe_print(io, "Syntaxerror : invalid device %s"ENDL, args);
   		return TRUE;
   	}
	if (uiDevice > NUM_GPIO_EXP_DEVICES - 1)
	{
   		IfxStdIf_DPipe_print(io, "Syntaxerror : invalid device %s"ENDL, args);
   		return TRUE;
	}
	// now we get the second argument
	if (Ifx_Shell_parseUInt32(&args, &uiGroup, FALSE) == FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid group %s"ENDL, args);
        return TRUE;
    }
    if (uiGroup > 1)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid group %d"ENDL, uiGroup);
        return TRUE;
    }

	// now we get the 3rd argument
	if (Ifx_Shell_parseUInt32(&args, &uiPort, FALSE) == FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid port %s"ENDL, args);
        return TRUE;
    }
    if (uiPort > 7)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid port %d"ENDL, uiPort);
        return TRUE;
    }
    i2caddress = IO_EXPANDER_ADDRESS[uiDevice];
    IfxStdIf_DPipe_print(io, "EXP %d.%d.%d: ", uiDevice, uiGroup, uiPort);
    direction = readConfig(GPIO_EXP_I2C_BUS, i2caddress, (uint8)uiGroup, (uint8)uiPort);
    if (direction)
    {
        IfxStdIf_DPipe_print(io, "Direction = IN; ");
    }
    else
    {
        IfxStdIf_DPipe_print(io, "Direction = OUT; ");
    }
    current_state = readState(GPIO_EXP_I2C_BUS,i2caddress, (uint8)uiGroup, (uint8)uiPort);
    if (current_state)
    {
    	IfxStdIf_DPipe_print(io, "State = HIGH "ENDL);
    }
    else
    {
    	IfxStdIf_DPipe_print(io, "State = LOW "ENDL);
    }

    return TRUE;
}

/** \brief Handle the 'configportexp' command
 *
 * \par Syntax
 *  - configportexp x y z: configure the GPIO expansion device x group y port z to input/output
 */
boolean Config_ExpPortState(pchar args, void *data, IfxStdIf_DPipe *io)
{
	uint32 uiPort, uiDevice, uiGroup;
	uint8 i2caddress;
	boolean direction, current_state, value;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : configportexp x y z in"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax     : configportexp x y z out"ENDL);
        IfxStdIf_DPipe_print(io, "           > configure the GPIO expansion device x group y port z to input/output"ENDL);
        print_address_help_message(io);
        return TRUE;
    }
	// first we get the first argument
    uiDevice = 0;
    uiPort = 0;

	if (Ifx_Shell_parseUInt32(&args, &uiDevice, FALSE) == FALSE)
   	{
   		IfxStdIf_DPipe_print(io, "Syntaxerror : invalid device %s"ENDL, args);
   		return TRUE;
   	}
	if (uiDevice > NUM_GPIO_EXP_DEVICES - 1)
	{
   		IfxStdIf_DPipe_print(io, "Syntaxerror : invalid device %s"ENDL, args);
   		return TRUE;
	}
	// now we get the second argument
	if (Ifx_Shell_parseUInt32(&args, &uiGroup, FALSE) == FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid group %s"ENDL, args);
        return TRUE;
    }
    if (uiGroup > 2)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid group %d"ENDL, uiGroup);
        return TRUE;
    }

	// now we get the 3rd argument
	if (Ifx_Shell_parseUInt32(&args, &uiPort, FALSE) == FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid port %s"ENDL, args);
        return TRUE;
    }
    if (uiPort > 16)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid port %d"ENDL, uiPort);
        return TRUE;
    }
	// now we get the last argument
    if (Ifx_Shell_matchToken(&args, "in") != FALSE)
    {
    	value = 1;
    }
    else
    {
    	if (Ifx_Shell_matchToken(&args, "out") != FALSE)
    	{
    		value = 0;
    	}
    	else
    	{
    		IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value %s"ENDL, args);
    	}
    }
    i2caddress = IO_EXPANDER_ADDRESS[uiDevice];
    setConfig(GPIO_EXP_I2C_BUS, i2caddress, (uint8)uiGroup, (uint8)uiPort,value);
    return TRUE;
}

/** \brief Handle the 'setportexp' command
 *
 * \par Syntax
 *  - setportexp x y z: configure the GPIO expansion device x group y port z to input/output
 */
boolean SetOutput_ExpPortState(pchar args, void *data, IfxStdIf_DPipe *io)
{
	uint32 uiPort, uiDevice, uiGroup, value;
	uint8 i2caddress;
	boolean direction, current_state;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : setportexp x y z 0"ENDL);
        IfxStdIf_DPipe_print(io, "Syntax     : setportexp x y z 1"ENDL);
        IfxStdIf_DPipe_print(io, "           > set output of GPIO expansion device x group y port z to 0 or 1"ENDL);
        print_address_help_message(io);
        return TRUE;
    }
	// first we get the first argument
    uiDevice = 0;
    uiPort = 0;

	if (Ifx_Shell_parseUInt32(&args, &uiDevice, FALSE) == FALSE)
   	{
   		IfxStdIf_DPipe_print(io, "Syntaxerror : invalid device %s"ENDL, args);
   		return TRUE;
   	}
	if (uiDevice > NUM_GPIO_EXP_DEVICES - 1)
	{
   		IfxStdIf_DPipe_print(io, "Syntaxerror : invalid device %s"ENDL, args);
   		return TRUE;
	}
	// now we get the second argument
	if (Ifx_Shell_parseUInt32(&args, &uiGroup, FALSE) == FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid group %s"ENDL, args);
        return TRUE;
    }
    if (uiGroup > 2)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid group %d"ENDL, uiGroup);
        return TRUE;
    }

	// now we get the 3rd argument
	if (Ifx_Shell_parseUInt32(&args, &uiPort, FALSE) == FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid port %s"ENDL, args);
        return TRUE;
    }
    if (uiPort > 16)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid port %d"ENDL, uiPort);
        return TRUE;
    }
	// now we get the last argument
	if (Ifx_Shell_parseUInt32(&args, &value, FALSE) == FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value %s"ENDL, args);
        return TRUE;
    }
	if (value >1)
	{
        IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value %s"ENDL, args);
        return TRUE;
	}

	i2caddress = IO_EXPANDER_ADDRESS[uiDevice];
    setOutputState(GPIO_EXP_I2C_BUS, i2caddress, (uint8)uiGroup, (uint8)uiPort,(boolean)value);
    return TRUE;
}

#if SOURCE_MUX_SELECTION_ENABLE == 1
/**
 *
 * \par Syntax
 *  - fpdlsource a/b: select FPDLink Source from Tegra A/B
 */
boolean fpdlSource(pchar args, void *data, IfxStdIf_DPipe *io)
{
	IfxPort_Pin *pPort;
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : fpdlsource a/b"ENDL);
        IfxStdIf_DPipe_print(io, "           > select FPDLink Source from Tegra A/B"ENDL);
    }
    else
    {
    	// first we get the device selection
        if ((Ifx_Shell_matchToken(&args, "a") != FALSE) || (Ifx_Shell_matchToken(&args, "A") != FALSE))
        {
        	Resource_SetFpdlState(TEGRA_A);
        }
        else if ((Ifx_Shell_matchToken(&args, "b") != FALSE) || (Ifx_Shell_matchToken(&args, "B") != FALSE))
        {
        	Resource_SetFpdlState(TEGRA_B);
        }
        else
		{
			IfxStdIf_DPipe_print(io, "Error : invalid tegra %s"ENDL, args);
			return TRUE;
		}
        IfxStdIf_DPipe_print(io, "Command Executed"ENDL);
	}
    return TRUE;
}

/** \brief Handle the 'pexcontrol' command
 *
 * \par Syntax
 *  - pexcontrol x...:switch muxes for PEX switch ref clock, reset, and I2C to be source by Tegra x
 */
boolean pexcontrol(pchar args, void *data, IfxStdIf_DPipe *io)
{
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
	{
		IfxStdIf_DPipe_print(io, "Syntax     : pexcontrol a/b"ENDL);
		IfxStdIf_DPipe_print(io, "           > switch muxes for PEX switch ref clock, reset, and I2C to be source A/B"ENDL);
	}
	else
	{
		if ((Ifx_Shell_matchToken(&args, "a") != FALSE) || (Ifx_Shell_matchToken(&args, "A") != FALSE))
		{
			Resource_SetPciState(TEGRA_A);

		}
		else if ((Ifx_Shell_matchToken(&args, "b") != FALSE) || (Ifx_Shell_matchToken(&args, "B") != FALSE))
		{
			Resource_SetPciState(TEGRA_B);
		}
		else
		{
			IfxStdIf_DPipe_print(io, "Error : invalid tegra %s"ENDL, args);
			return TRUE;
		}
	}
	return TRUE;
}

/** \brief Handle the 'switchpcie' command
 *
 * \par Syntax
 *  - switchpcie a b milliseconds   // Switch PEX REFCLK from Tegra A to Tegra B with a delay of milliseconds
 */
/*
 * -make sure configportexp 1 1 0 out // Make sure I2C GPIO expander port P10 is set as an output. Sure this is your default state now.
-make sure configportexp 1 1 1 out // Make sure I2C GPIO expander port P11 is set as an output. Sure this is your default state now.

- tegrareset "a/b" h   //  Put Tegra "a or b' in a HOLD reset state to TURN OFF PEX REFCLK
- delay milliseconds   // Hopefully milliseconds is ENOUGH resolution? May need useconds?
- setportexp 1 1 0 "0/1"   // Throw the PEX REFCLK switch to Tegra "a or b"
-setportexp 1 1 1 "0/1"    // Go ahead and also set PEX RESET to Tegra "a or b"
- delay 500+ms         // Give it some time to see if Tegra "a/b" kernel hangs.
- tegrareset "a/b"    // release the HOLD on Tegra reset
- print out who has pex REFCLK/RESET control.
*/
boolean switchpcie(pchar args, void *data, IfxStdIf_DPipe *io)
{
	boolean isToTegraA = TRUE;
	uint32 us;
	uint32 i;
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
	{
		IfxStdIf_DPipe_print(io, "Syntax     : switchpcie a us"ENDL);
		IfxStdIf_DPipe_print(io, "           > switch PEX REFCLK to Tegra a with a delay of us milliseconds"ENDL);
	}
	else
	{
		if ((Ifx_Shell_matchToken(&args, "a") != FALSE) || (Ifx_Shell_matchToken(&args, "A") != FALSE))
		{
			isToTegraA = TRUE;
		}
		else if ((Ifx_Shell_matchToken(&args, "b") != FALSE) || (Ifx_Shell_matchToken(&args, "B") != FALSE))
		{
			isToTegraA = FALSE;
		}
		else
		{
			IfxStdIf_DPipe_print(io, "Error : invalid tegra %s"ENDL, args);
			return TRUE;
		}

		if (Ifx_Shell_parseUInt32(&args, &us, FALSE) == FALSE)
		{
			IfxStdIf_DPipe_print(io, "Syntaxerror : invalid delay time %s"ENDL, args);
			return TRUE;
		}

		AsclinShellInterface_execute_cmd("configportexp", "1 1 0 out");
		AsclinShellInterface_execute_cmd("configportexp", "1 1 1 out");

		if(isToTegraA)
		{
			AsclinShellInterface_execute_cmd("tegrareset", "b h");
			GPIO_EXP_CMD_DEBUG_PRINTF(("Delay %d us\n", us));
			waitTime(us * TimeConst[TIMER_INDEX_1US]);
			AsclinShellInterface_execute_cmd("setportexp", "1 1 0 0");
			AsclinShellInterface_execute_cmd("setportexp", "1 1 1 0");
			GPIO_EXP_CMD_DEBUG_PRINTF(("Delay 500 ms\n"));
			waitTime(500 * TimeConst[TIMER_INDEX_1MS]);
			AsclinShellInterface_execute_cmd("tegrareset", "b");
			IfxStdIf_DPipe_print(io, "Command Executed, switch pex ref to tegra A with %d us delay"ENDL, us);
		}
		else
		{
			AsclinShellInterface_execute_cmd("tegrareset", "a h");
			GPIO_EXP_CMD_DEBUG_PRINTF(("Delay %d us\n", us));
			waitTime(us * TimeConst[TIMER_INDEX_1US]);
			AsclinShellInterface_execute_cmd("setportexp", "1 1 0 1");
			AsclinShellInterface_execute_cmd("setportexp", "1 1 1 1");
			GPIO_EXP_CMD_DEBUG_PRINTF(("Delay 500 ms\n"));
			waitTime(500 * TimeConst[TIMER_INDEX_1MS]);
			AsclinShellInterface_execute_cmd("tegrareset", "a");
			IfxStdIf_DPipe_print(io, "Command Executed, switch pex ref to tegra B with %d us delay"ENDL, us);
		}
	}


	return TRUE;
}
#endif //#if SOURCE_MUX_SELECTION_ENABLE == 1
