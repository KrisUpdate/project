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

#include "SysSe/Comm/Ifx_Shell.h"
#include "TegraQspiCmds.h"
#include "TegraQspi.h"
//#include "TacpQspi.h"
#include "Qspi/Std/IfxQspi.h"
#include "Configuration.h"
#include "Aurix2QspiSlave.h"
#include "string.h"
#include "board.h"
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
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
/** \brief Handle the 'tegrainit' command
 *
 * \par Syntax
 *  - tegrainit : initialize the qspi module for Tegra
 */
boolean tegraqspiinit(pchar args, void *data, IfxStdIf_DPipe *io)
{
	uint32 qspi_module_number;
	uint32 qspi_mode;
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
	{
	    IfxStdIf_DPipe_print(io, "Syntax     : tegrainit x y"ENDL);
	    IfxStdIf_DPipe_print(io, "           > Initialize the Qspi module x with mode y for Tegra"ENDL);
	}

	else
	{
		// now we get the first argument
		if (Ifx_Shell_parseUInt32(&args, &qspi_module_number, FALSE) == FALSE)
		{
			IfxStdIf_DPipe_print(io, "Syntaxerror : invalid qspi module %s"ENDL, args);
			return TRUE;
		}
		if (qspi_module_number > 4)
		{
			IfxStdIf_DPipe_print(io, "Syntaxerror : invalid qspi module %d"ENDL, qspi_module_number);
			return TRUE;
		}
		// now we get the second argument
		if (Ifx_Shell_parseUInt32(&args, &qspi_mode, FALSE) == FALSE)
		{
			IfxStdIf_DPipe_print(io, "Syntaxerror : invalid qspi mode %s"ENDL, args);
			return TRUE;
		}
		if (qspi_mode > 4)
		{
			IfxStdIf_DPipe_print(io, "Syntaxerror : invalid qspi mode %d"ENDL, qspi_mode);
			return TRUE;
		}
        IfxStdIf_DPipe_print(io, "TegraQspiMaster_init qspi_module_number=%d, qspi_mode=%d"ENDL, qspi_module_number,qspi_mode);
		TegraQspiMaster_init((uint8)qspi_module_number, (uint8)qspi_mode);
	}

	return TRUE;
}

/** \brief Handle the 'tegrawr' command
 *
 * \par Syntax
 *  - tegrawr a/b n ...: write and read tegra A/B with n bytes
 */
boolean tegrawriteread(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 count, uiCnt, uiCnt2, retval, slvNr;
    uint32 sendDatas[16];
    uint32 recDatas[16];
    memset(sendDatas, 0, 16);
    memset(recDatas, 0, 16);
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
    	if(board.project == P3570)
    	{
			IfxStdIf_DPipe_print(io, "Syntax     : tegrawr x1/x2/aurix2 n ..."ENDL);
			IfxStdIf_DPipe_print(io, "           > Write and read Tegra x1/x2/aurix2 with n 16-bits with master SPI"ENDL);
    	}
    	else
    	{
			IfxStdIf_DPipe_print(io, "Syntax     : tegrawr x1/x2/cvm n ..."ENDL);
			IfxStdIf_DPipe_print(io, "           > Write and read Tegra x1/x2/cvm with n 16-bits with master SPI"ENDL);
    	}
        IfxStdIf_DPipe_print(io, "Example    > tegrawr x1 2 0x0001 0x0203"ENDL);
        IfxStdIf_DPipe_print(io, "           > will write (2 16-bits) 4 bytes 0 1 2 3 to Xavier A via SPI"ENDL);
    }
    else
    {
        if ((Ifx_Shell_matchToken(&args, "x1") != FALSE) || (Ifx_Shell_matchToken(&args, "X1") != FALSE))
        {
            slvNr = 0;
        }
        else if ((Ifx_Shell_matchToken(&args, "x2") != FALSE) || (Ifx_Shell_matchToken(&args, "X2") != FALSE))
        {
            slvNr = 4;
        }
        else
        {

#if 0
        		if ((Ifx_Shell_matchToken(&args, "aurix2") != FALSE) || (Ifx_Shell_matchToken(&args, "AURIX2") != FALSE))
				{
					slvNr = 3;
				}
        		else
                {
                    IfxStdIf_DPipe_print(io, "Error : invalid parameter %s"ENDL, args);
                    return TRUE;
                }
#else
        	if(board.project == P3570)
        	{
        		if ((Ifx_Shell_matchToken(&args, "aurix2") != FALSE) || (Ifx_Shell_matchToken(&args, "AURIX2") != FALSE))
				{
					slvNr = 3;
				}
        		else
                {
                    IfxStdIf_DPipe_print(io, "Error : invalid parameter %s"ENDL, args);
                    return TRUE;
                }
        	}
        	else
        	{
        		if ((Ifx_Shell_matchToken(&args, "cvm") != FALSE) || (Ifx_Shell_matchToken(&args, "CVM") != FALSE))
        		{
        			slvNr = 3;
        		}
        		else
				{
					IfxStdIf_DPipe_print(io, "Error : invalid parameter %s"ENDL, args);
					return TRUE;
				}
        	}
#endif

        }
        // now we get the number of bytes to write/read
        if (Ifx_Shell_parseUInt32(&args, &count, FALSE) == FALSE)
        {
            IfxStdIf_DPipe_print(io, "Syntaxerror : invalid count %s"ENDL, args);
            return TRUE;
        }
        //TegraQspiMaster_init((uint8)slvNr);  //will be initialized at power up.
        while (count)
        {
            uiCnt = count;
            if (uiCnt > 16) uiCnt = 16;
            for (uiCnt2=0; uiCnt2<uiCnt; uiCnt2++)
            {
                if (Ifx_Shell_parseUInt32(&args, &sendDatas[uiCnt2], FALSE) == FALSE)
                {
                    IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value %s"ENDL, args);
                    return TRUE;
                }
            }
            retval = TegraQspiMaster_read_write(slvNr, uiCnt, &sendDatas[0], &recDatas[0]);
            if (retval)
            {
                IfxStdIf_DPipe_print(io, "Error : %d returned"ENDL, retval);
                return TRUE;
            }
#if PROJECT == PROJECT_PAGASUS2 && PG2_PREEVT
            if (slvNr == 0)
                IfxStdIf_DPipe_print(io, "Data returned by XA:"ENDL);
            else if (slvNr == 3)
                IfxStdIf_DPipe_print(io, "Data returned by CVM/AURIX2:"ENDL);
            else if (slvNr == 4)
                IfxStdIf_DPipe_print(io, "Data returned by XB:"ENDL);
#else
            if (slvNr == 1)
                IfxStdIf_DPipe_print(io, "Data returned by Tegra B:"ENDL);
            else
                IfxStdIf_DPipe_print(io, "Data returned by Tegra A:"ENDL);
#endif
            for (uiCnt2=0; uiCnt2<uiCnt; uiCnt2++)
                IfxStdIf_DPipe_print(io, "0x%02x  ", recDatas[uiCnt2]);
            IfxStdIf_DPipe_print(io, " "ENDL);
        	count -= uiCnt;
        }

    }
    return TRUE;
}

/** \brief Handle the 'spimastertest' command
 *
 * \par Syntax
 *  - spimastertest a/b: test with tegra A(/B) with n bytes with master SPI
 */
boolean spimastertest(pchar args, void *data, IfxStdIf_DPipe *io)
{
	const uint32 TEST_PATTERN[] = {0x5a5a, 0x5a5a, 0xa5a5, 0xa5a5, 0xffff, 0xffff, 0x0, 0x0};
	uint32 recDatas[ARRAY_SIZE(TEST_PATTERN)];
	uint32 count = ARRAY_SIZE(TEST_PATTERN);
	uint32 uiCnt=0, uiCnt2=0, retval=0, slvNr=0;
	uint32 qspi_mode;

	memset(recDatas, 0, ARRAY_SIZE(recDatas));
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
	{
		IfxStdIf_DPipe_print(io, "Syntax     : spimastertest [a/b] x"ENDL);
		IfxStdIf_DPipe_print(io, "           > test with tegra A(/B) with n bytes with master SPI"ENDL);
		IfxStdIf_DPipe_print(io, "           > with SPI mode x"ENDL);
		IfxStdIf_DPipe_print(io, "           > it works with mods test 217"ENDL);
	}
	else
	{
		// first we get the device selection
		if ((Ifx_Shell_matchToken(&args, "a") != FALSE) || (Ifx_Shell_matchToken(&args, "A") != FALSE))
		{
			slvNr=0;
		}
//#if PROJECT == PROJECT_DRIVE_PX2
		else if ((Ifx_Shell_matchToken(&args, "b") != FALSE) || (Ifx_Shell_matchToken(&args, "B") != FALSE))
		{
			slvNr=4;
		}
//#endif
		else
		{
			slvNr=0;
		}
		// now we get second argument mode
		if (Ifx_Shell_parseUInt32(&args, &qspi_mode, FALSE) == FALSE)
		{
			IfxStdIf_DPipe_print(io, "Syntaxerror : invalid qspi mode %s"ENDL, args);
			return TRUE;
		}
		if (qspi_mode > 3)
		{
			IfxStdIf_DPipe_print(io, "Syntaxerror : invalid qspi mode %d"ENDL, qspi_mode);
			return TRUE;
		}

		while (count)
		{
			uiCnt = count;
			if (uiCnt > QSPI_BUFFER_SIZE) uiCnt = QSPI_BUFFER_SIZE;

			TegraQspiMaster_init(slvNr, qspi_mode);
			retval = TegraQspiMaster_read_write(slvNr, uiCnt, &TEST_PATTERN[0], &recDatas[0]);
			if (retval)
			{
				IfxStdIf_DPipe_print(io, "Error : %d returned"ENDL, retval);
				return TRUE;
			}
			if (slvNr == 4)
				IfxStdIf_DPipe_print(io, "Data returned by Tegra B:"ENDL);
			else
				IfxStdIf_DPipe_print(io, "Data returned by Tegra A:"ENDL);
			for (uiCnt2=0; uiCnt2<uiCnt; uiCnt2++)
				IfxStdIf_DPipe_print(io, "0x%02x  ", recDatas[uiCnt2]);
			IfxStdIf_DPipe_print(io, " "ENDL);

			for (uiCnt2=0; uiCnt2<uiCnt; uiCnt2++)
			{
				if(recDatas[uiCnt2] != TEST_PATTERN[uiCnt2])
				{
					break;
				}
			}
			if(uiCnt2 == uiCnt)
			{
				IfxStdIf_DPipe_print(io, "Data Received is the same as the PATTERN, Test Pass"ENDL);
			}
			else
			{
				IfxStdIf_DPipe_print(io, "Data Received is NOT the same, receive 0x%x, expected 0x%x"ENDL,
						recDatas[uiCnt2], TEST_PATTERN[uiCnt2]);
			}
			count -= uiCnt;
		}
	}
	return TRUE;

}


/** \brief Handle the 'spislaveloopback' command
 *
 * \par Syntax
 *  - spislavetest : Start loopback test with Tegra with SPI Slave
 */
boolean spislaveloopback(pchar args, void *data, IfxStdIf_DPipe *io)
{
#if SPI_SLAVE_ENABLE == 1
	uint32 count = 10;
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
	{
		IfxStdIf_DPipe_print(io, "Syntax     : spislaveloopback [x]"ENDL);
		IfxStdIf_DPipe_print(io, "           > Start SPI Slave loopback test with Tegra (mods) for x bytes, default 10 bytes"ENDL);
		IfxStdIf_DPipe_print(io, "           > this is a blocking test, works with mods test 224"ENDL);
		IfxStdIf_DPipe_print(io, "           > it will possibly affect TACP over SPI (camera)"ENDL);
		IfxStdIf_DPipe_print(io, "           > so please reset Aurix after test command if you find TACP is not working properly"ENDL);
	}
	else
	{
		if (Ifx_Shell_parseUInt32(&args, &count, FALSE) == FALSE)
		{
			count  = 10;
		}

		IfxStdIf_DPipe_print(io, "it will possibly affect TACP over SPI (camera)"ENDL);
		IfxStdIf_DPipe_print(io, "so please reset Aurix after the command if you find TACP is not working properly"ENDL);

		tacpQspiEnabled = FALSE;

		if (IfxQspi_isModuleEnabled(&TACP_QSPI_SALVE_MODULE) == TRUE)
			IfxQspi_resetModule(&TACP_QSPI_SALVE_MODULE);

		TegraQspiSlave_init(4);
		TegraQspiSlave_loopback(count);

		TacpQspiInit();
	}
#else
	UNUSED(args);
	UNUSED(data);
	UNUSED(io);
	IfxStdIf_DPipe_print(io, "We don't support SPI slave commands in this project"ENDL);
#endif     //endof #if SPI_SLAVE_ENABLE == 1

	return TRUE;
}

/** \brief Handle the 'spislavewrite' command
 *
 * \par Syntax
 *  - spislavetest : Use spi slave to exchange data with Tegra
 */
boolean spislavewrite(pchar args, void *data, IfxStdIf_DPipe *io)
{
	uint32 count;
	uint32 sendDatas[16];
	uint32 recDatas[16];
	memset(sendDatas, 0, sizeof(sendDatas));
	memset(recDatas, 0, sizeof(recDatas));
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
	{
		IfxStdIf_DPipe_print(io, "Syntax     : spislavewrite n ..."ENDL);
		IfxStdIf_DPipe_print(io, "           > Use spi slave to exchange data with SPI master for n bytes"ENDL);
	}
	else
	{
		uint8 i;
		// now we get the number of bytes to write/read
		if (Ifx_Shell_parseUInt32(&args, &count, FALSE) == FALSE)
		{
			IfxStdIf_DPipe_print(io, "Syntaxerror : invalid count %s"ENDL, args);
			return TRUE;
		}
		for (i=0; i<count; i++)
		{
			if (Ifx_Shell_parseUInt32(&args, &sendDatas[i], FALSE) == FALSE)
			{
				IfxStdIf_DPipe_print(io, "Syntaxerror : invalid value %s"ENDL, args);
				return TRUE;
			}
		}

		A2_QspiSlave_read_write(count, sendDatas, recDatas);

		IfxStdIf_DPipe_print(io, "Data received from spi Mater:"ENDL);
		for (i=0; i<count; i++)
			IfxStdIf_DPipe_print(io, "0x%02x  ", recDatas[i]);

		IfxStdIf_DPipe_print(io, "Sent out data: "ENDL);
		for (i=0; i<count; i++)
			IfxStdIf_DPipe_print(io, "0x%02x  ", sendDatas[i]);
		IfxStdIf_DPipe_print(io, " "ENDL);
	}
	return TRUE;
}
