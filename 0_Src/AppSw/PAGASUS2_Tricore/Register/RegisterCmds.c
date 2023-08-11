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
#include "RegisterCmds.h"
#include "IfxPms_reg.h"
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
extern volatile boolean system_ready;
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
void aurix_reset(void);
/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
/** \brief Handle the 'regrd' command
 *
 * \par Syntax
 *  - regrd x [length]: read [length] double words starting from register address x
 */
boolean Register_Read(pchar args, void *data, IfxStdIf_DPipe *io)
{
	uint32 reg_offset,length,i;
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : regrd offset [length]"ENDL);
        IfxStdIf_DPipe_print(io, "           > read [length] double words starting from register address x"ENDL);
    }
    else
    {
    	if (Ifx_Shell_parseUInt32(&args, &reg_offset, FALSE) == FALSE)
       	{
       		IfxStdIf_DPipe_print(io, "Syntaxerror : invalid register offset %s"ENDL, args);
       		return TRUE;
       	}
        IfxStdIf_DPipe_print(io, "regrd offset= 0x%08X"ENDL, reg_offset);


    	if (Ifx_Shell_parseUInt32(&args, &length, FALSE) == FALSE)
    	{
        	length = 1;
    	}
        IfxStdIf_DPipe_print(io, "length= 0x%08X"ENDL, length);

    	for (i=0; i < length; i++)
    	{
    		IfxStdIf_DPipe_print(io, "0x%08x = 0x%08x"ENDL,reg_offset,*(uint32*)reg_offset);
    		reg_offset = reg_offset + 4;
    	}

    }
    return TRUE;
}

/** \brief Handle the 'regwr' command
 *
 * \par Syntax
 *  - regwr x value: write value to register address x
 */
boolean Register_Write(pchar args, void *data, IfxStdIf_DPipe *io)
{
    volatile uint32 reg_offset,value;
    uint16 password,i, length = 3;
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : regwr x value"ENDL);
        IfxStdIf_DPipe_print(io, "           > write value to register address x"ENDL);
        IfxStdIf_DPipe_print(io, "Notice:    > writing value to Aurix registers may break Aurix function, or cause it hang!"ENDL);
    }
    else
    {
    	if (Ifx_Shell_parseUInt32(&args, &reg_offset, FALSE) == FALSE)
       	{
       		IfxStdIf_DPipe_print(io, "Syntaxerror : invalid register offset %s"ENDL, args);
       		return TRUE;
       	}

    	if (Ifx_Shell_parseUInt32(&args, &value, FALSE) == FALSE)
    	{
       		IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value %s"ENDL, args);
       		return TRUE;
    	}
        IfxStdIf_DPipe_print(io, "regrd offset= 0x%08X"ENDL, reg_offset);
        IfxStdIf_DPipe_print(io, "value= 0x%08X"ENDL, value);




//      IfxStdIf_DPipe_print(io, "regrd offset= 0x%08X"ENDL, SWRSTCON);
//      IfxStdIf_DPipe_print(io, "length= 0x%08X"ENDL, value);

        password = IfxScuWdt_getCpuWatchdogPassword();

        IfxScuWdt_clearCpuEndinit(password);
        volatile uint8 *addr     = (uint8 *)reg_offset;
       *((uint32*)(addr)) = value;
        IfxScuWdt_setCpuEndinit(password);

    	for (i=0; i < length; i++)
    	{
    		IfxStdIf_DPipe_print(io, "dd 0x%08x = 0x%08x"ENDL,addr,*((uint32*)(addr)));
    		addr = addr + 4;
    	}

        IfxScuWdt_setSafetyEndinit(password);
        IfxStdIf_DPipe_print(io, "Done"ENDL);
    }
    return TRUE;
}

/** \brief Handle the 'aurixreset' command
 *
 * \par Syntax
 *  - aurixreset : reset Aurix Controller
 */
extern volatile boolean system_ready;
boolean aurixrst(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 value;
    uint16 password;
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : aurixreset"ENDL);
        IfxStdIf_DPipe_print(io, "           > reset Aurix controller"ENDL);
    }else if(Ifx_Shell_matchToken(&args, "pure") != FALSE){
            IfxStdIf_DPipe_print(io, "right now(aurixrst no power off)"ENDL);
			system_ready = 0;
			waitTime(500*TimeConst[TIMER_INDEX_1MS]); // this is just a placeholder value for delay
			aurix_reset();
    }
    else
    {
        System_PowerOff();
        //sem_wait_until_get(SEM_SPI_2, __LINE__, __func__);
        IfxStdIf_DPipe_print(io, "Command Executed"ENDL);
        system_ready = 0;
        waitTime(500*TimeConst[TIMER_INDEX_1MS]); // this is just a placeholder value for delay
        aurix_reset();

#if 0
    value = *(uint32*)(SWRSTCON);

        IfxStdIf_DPipe_print(io, "regrd offset= 0x%08X"ENDL, SWRSTCON);
        IfxStdIf_DPipe_print(io, "length= 0x%08X"ENDL, value);

    password = IfxScuWdt_getCpuWatchdogPassword();

    IfxScuWdt_clearCpuEndinit(password);
    *(uint32*)(SWRSTCON) = value | 0x2;
    IfxScuWdt_setCpuEndinit(password);


/*
    value = *(uint32*)(SWRSTCON);
    password = IfxScuWdt_getCpuWatchdogPassword();

    IfxScuWdt_clearCpuEndinit(password);
    *(uint32*)(SWRSTCON) = value | 0x2;
    IfxScuWdt_setCpuEndinit(password);
*/
#endif
    }

    return TRUE;
}

/** \brief Handle the 'uartflash' command
 *
 * \par Syntax
 *  - uartflash : prepare the board for uart flashing
 */
boolean uartflash(pchar args, void *data, IfxStdIf_DPipe *io)
{
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : uartflash"ENDL);
        IfxStdIf_DPipe_print(io, "           > prepare the board for uart flashing"ENDL);
    }
    else
    {
    	IfxStdIf_DPipe_print(io, "Configure the board for UART programming by toggling the programming switch"ENDL);
    	IfxStdIf_DPipe_print(io, "Connect target's UART connector to a host PC"ENDL);
    	IfxStdIf_DPipe_print(io, "This target will reset in 10 seconds"ENDL);
    	waitTime(10000*TimeConst[TIMER_INDEX_1MS]); // this is just a placeholder value for delay
    	uint32 value = *(uint32*)(SWRSTCON);
        uint16 password = IfxScuWdt_getSafetyWatchdogPassword();
        IfxScuWdt_clearSafetyEndinit(password);
       	*(uint32*)(SWRSTCON) = value | 0x2;
        IfxScuWdt_setSafetyEndinit(password);
    }

    return TRUE;
}

void ota_aurixrst()
{
	System_PowerOff();
	//sem_wait_until_get(SEM_SPI_2, __LINE__, __func__);
	system_ready = 0;

    waitTime(500*TimeConst[TIMER_INDEX_1MS]); // this is just a placeholder value for delay
    aurix_reset();
}

void aurix_reset(void)
{
    uint32 value;
    uint16 password;

#if 0
    /* It's not working for TC397, UART console is dead when we do software reset
    * we use ext watchdog for WAR now */
//    #include "../Power/TLF35584.h"
//    IfxTLF35584_unprotect_register();
//	IfxTLF35584_enable_window_watchdog();
//	IfxTLF35584_protect_register();
//	while(1);

	/* This is the software reset, but it's not used now */
    value = *(uint32*)(SWRSTCON);

//      IfxStdIf_DPipe_print(io, "regrd offset= 0x%08X"ENDL, SWRSTCON);
//      IfxStdIf_DPipe_print(io, "length= 0x%08X"ENDL, value);

    password = IfxScuWdt_getCpuWatchdogPassword();

    IfxScuWdt_clearCpuEndinit(password);
    *(uint32*)(SWRSTCON) = value | 0x2;
    IfxScuWdt_setCpuEndinit(password);
#else

    /* It's not working for TC397, UART console is dead when we do software reset
    * we use ext watchdog for WAR now */
    #include "../Power/TLF35584.h"
    IfxTLF35584_unprotect_register();
	IfxTLF35584_enable_window_watchdog();
	IfxTLF35584_protect_register();
	while(1);

	/* This is the software reset, but it's not used now */
    value = *(uint32*)(SWRSTCON);
    password = IfxScuWdt_getCpuWatchdogPassword();

    IfxScuWdt_clearCpuEndinit(password);
    *(uint32*)(SWRSTCON) = value | 0x2;
    IfxScuWdt_setCpuEndinit(password);
#endif
}


boolean Register_Read_keyword(pchar args, void *data, IfxStdIf_DPipe *io)
{
    /*TC397 parameter start*/
    double tc397_core_tmp, tc397_pms_tmp;
    /*TC397 parameter end*/

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : rdaurixreg temp"ENDL);
        IfxStdIf_DPipe_print(io, "           > read temp from aurix"ENDL);
    }
    else
    {
        if(Ifx_Shell_matchToken(&args, "temp") != FALSE)
        {
#if 0
            /*setting TC397 temperature start*/
            SCU_DTSCLIM.U = 0x0CD846D6;
            /*setting TC397 temperature end*/

            waitTime(500*TimeConst[TIMER_INDEX_1MS]); // this is just a placeholder value for delay

            /*TC397 tmp start*/
            tc397_tmp = (SCU_DTSCSTAT.U & 0x00000FFF) / 7.505 - 273.15;
            IfxStdIf_DPipe_print(io,"tc397_tmp, %f, degC\r\n", tc397_tmp);
            /*TC397 tmp end*/
#else
            if( 0 == SCU_DTSCLIM.B.EN ){
                //DTSC not enable, application reset needed
                SCU_DTSCLIM.U = 0x0CD846D6;
                IfxStdIf_DPipe_print(io,"DTSC not enable. doing application reset!\r\n");
                waitTime(500*TimeConst[TIMER_INDEX_1MS]); // this is just a placeholder value for delay
            }

            //1. read out Core Die Temperature Sensor value
            tc397_core_tmp = ((SCU_DTSCSTAT.U & 0x00000FFF) / 7.505) - 273.15;
            IfxStdIf_DPipe_print(io,"tc397 Core DTS: %f degC\r\n", tc397_core_tmp);

            //2. read out PMS Die Temperature Sensor value
            tc397_pms_tmp = ((PMS_DTSSTAT.U & 0x00000FFF) / 7.505) - 273.15;
            IfxStdIf_DPipe_print(io,"tc397 PMS DTS: %f degC\r\n", tc397_pms_tmp);
#endif
        }
        else
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid parameter %s"ENDL, args);
            IfxStdIf_DPipe_print(io, "Use rdreg ? to get help message"ENDL);
        }
    }
    return TRUE;
}
