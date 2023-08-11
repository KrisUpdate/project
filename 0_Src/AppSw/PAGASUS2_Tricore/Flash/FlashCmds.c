/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
/*
#include <stdio.h>
#include <string.h>
#include <Cpu/Std/IfxCpu.h>
#include "Configuration.h"
#include "SysSe/Bsp/Bsp.h"
#include "SysSe/Comm/Ifx_Shell.h"
#include "FlashCmd.h"
#include "FlashDemo.h"
#include "ota.h"

#define CRC_SECTOR_ADDR				 0xA0600000
#define CRC_CORRUPT_PATTERN			 0xA060ABCD
*/
#include "Configuration.h"
#include "Platform_Types.h"
#include "Ifx_Types.h"
#include "IfxStdIf_DPipe.h"
#include "resource.h"
#if PARTITION_LAYOUT == _QPARTITION
#define CRC_SECTOR_ADDR                          0xA0C00000
#define CRC_SECTOR_CORRECT_VALUE        0xA0600000
#else
#define CRC_SECTOR_ADDR                           0xA0600000
#define CRC_SECTOR_CORRECT_VALUE         0xA0600000
#endif
#define CRC_CORRUPT_PATTERN                  0xA060ABCD

extern boolean is_ota_start;
/*
boolean pflash_write(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 sector_addr,start_page,no_of_pages,pattern;
    uint32 flash=0;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : pflash_write sector_addr start_page no_of_pages pattern"ENDL);
        IfxStdIf_DPipe_print(io, "           > write 32*no_of_pages bytes(pattern) from sector_addr"ENDL);
    }
    else
    {
        if (Ifx_Shell_parseUInt32(&args, &sector_addr, FALSE) == FALSE)
        {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid sector address %s"ENDL, args);
                return TRUE;
        }

        if (Ifx_Shell_parseUInt32(&args, &start_page, FALSE) == FALSE)
        {
                start_page = 1;
        }

        if (Ifx_Shell_parseUInt32(&args, &no_of_pages, FALSE) == FALSE)
        {
                no_of_pages = 1;
        }
        if (Ifx_Shell_parseUInt32(&args, &pattern, FALSE) == FALSE)
        {
                pattern = 1;
        }
        IfxStdIf_DPipe_print(io, "Running : pflash_write 0x%08x %d %d"ENDL, sector_addr, start_page, no_of_pages);
#if MCU_PLATFORM == _AURIX_TC29X_
        IfxStdIf_DPipe_print(io, "flash %d flash_type %d pattern 0x%08x"ENDL, flash, clib_flash_type(sector_addr), pattern);
#elif MCU_PLATFORM == _AURIX_TC39X_
        IfxStdIf_DPipe_print(io, "flash %d pattern 0x%08x"ENDL, flash, pattern);
#endif
        PFlashWrite(io,flash,sector_addr,start_page,no_of_pages, pattern);
    }
    return TRUE;
}

boolean pflash_read(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 sector_addr,start_page,no_of_pages;
    uint32 flash=0;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : pflash_read sector_addr start_page no_of_pages"ENDL);
        IfxStdIf_DPipe_print(io, "           > read 32*no_of_pages from sector_addr"ENDL);
    }
    else
    {
        if (Ifx_Shell_parseUInt32(&args, &sector_addr, FALSE) == FALSE)
        {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid sector address %s"ENDL, args);
                return TRUE;
        }

        if (Ifx_Shell_parseUInt32(&args, &start_page, FALSE) == FALSE)
        {
                start_page = 1;
        }

        if (Ifx_Shell_parseUInt32(&args, &no_of_pages, FALSE) == FALSE)
        {
                no_of_pages = 1;
        }
        IfxStdIf_DPipe_print(io, "Running : pflash_read 0x%08x %d %d"ENDL, sector_addr, start_page, no_of_pages);
#if MCU_PLATFORM == _AURIX_TC29X_
        IfxStdIf_DPipe_print(io, "flash %d flash_type %d"ENDL, flash, clib_flash_type(sector_addr));
#elif MCU_PLATFORM == _AURIX_TC39X_
        IfxStdIf_DPipe_print(io, "flash %d"ENDL, flash);
#endif
        PFlashRead(io,flash,sector_addr,start_page,no_of_pages);

    }
    return TRUE;
}
*/
/*
boolean pflash_jump(pchar args, void *data, IfxStdIf_DPipe *io)
{
    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : jump"ENDL);
        IfxStdIf_DPipe_print(io, "           > jump to update fw starting at 0xA0D00000"ENDL);
    }
    else
    {
        IfxStdIf_DPipe_print(io, "Running : Jump to update fw at 0xA0D00000"ENDL);

        __asm__ volatile ("ji %0"::"a"(0xA0D00000));
    }
    return TRUE;
}
*/


boolean pflash_write(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 sector_addr,start_page,no_of_pages,pattern;
    uint32 flash=0;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : pflash_write sector_addr start_page no_of_pages pattern"ENDL);
        IfxStdIf_DPipe_print(io, "           > write 32*no_of_pages bytes(pattern) from sector_addr"ENDL);
    }
    else
    {
        if (Ifx_Shell_parseUInt32(&args, &sector_addr, FALSE) == FALSE)
        {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid sector address %s"ENDL, args);
                return TRUE;
        }

        if (Ifx_Shell_parseUInt32(&args, &start_page, FALSE) == FALSE)
        {
                start_page = 1;
        }

        if (Ifx_Shell_parseUInt32(&args, &no_of_pages, FALSE) == FALSE)
        {
                no_of_pages = 1;
        }
        if (Ifx_Shell_parseUInt32(&args, &pattern, FALSE) == FALSE)
        {
                pattern = 1;
        }
        IfxStdIf_DPipe_print(io, "Running : pflash_write 0x%08x %d %d"ENDL, sector_addr, start_page, no_of_pages);

        IfxStdIf_DPipe_print(io, "flash %d pattern 0x%08x"ENDL, flash, pattern);

        PFlashWrite(io,flash,sector_addr,start_page,no_of_pages, pattern);
    }
    return TRUE;
}

boolean pflash_read(pchar args, void *data, IfxStdIf_DPipe *io)
{
    uint32 sector_addr,start_page,no_of_pages;
    uint32 flash=0;

    if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : pflash_read sector_addr start_page no_of_pages"ENDL);
        IfxStdIf_DPipe_print(io, "           > read 32*no_of_pages from sector_addr"ENDL);
    }
    else
    {
        if (Ifx_Shell_parseUInt32(&args, &sector_addr, FALSE) == FALSE)
        {
                IfxStdIf_DPipe_print(io, "Syntaxerror : invalid sector address %s"ENDL, args);
                return TRUE;
        }

        if (Ifx_Shell_parseUInt32(&args, &start_page, FALSE) == FALSE)
        {
                start_page = 1;
        }

        if (Ifx_Shell_parseUInt32(&args, &no_of_pages, FALSE) == FALSE)
        {
                no_of_pages = 1;
        }
        IfxStdIf_DPipe_print(io, "Running : pflash_read 0x%08x %d %d"ENDL, sector_addr, start_page, no_of_pages);
        IfxStdIf_DPipe_print(io, "flash %d"ENDL, flash);
        PFlashRead(io,flash,sector_addr,start_page,no_of_pages);

    }
    return TRUE;
}

boolean SwitchToProdFw(pchar args, void *data, IfxStdIf_DPipe *io)
{
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : switchtopfw"ENDL);
        IfxStdIf_DPipe_print(io, "           > Switch to Porduction FW"ENDL);
    }
    else
    {
    	PFlashWrite(io,0,CRC_SECTOR_ADDR,0,1, CRC_SECTOR_CORRECT_VALUE);
        sem_wait_until_get(SEM_SPI_2, __LINE__, __func__);
    	ota_aurixrst();
        sem_release(SEM_SPI_2);
    }

    return TRUE;
}

boolean SwitchToUpdFw(pchar args, void *data, IfxStdIf_DPipe *io)
{
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : switchtoufw"ENDL);
        IfxStdIf_DPipe_print(io, "           > Switch to Update FW"ENDL);
    }
    else
    {
    
        is_ota_start = TRUE;
    	PFlashWrite(io,0,CRC_SECTOR_ADDR,0,1, CRC_CORRUPT_PATTERN);
        sem_wait_until_get(SEM_SPI_2, __LINE__, __func__);
    	ota_aurixrst();
        sem_release(SEM_SPI_2);
    }

    return TRUE;
}


boolean ota_config(pchar args, void *data, IfxStdIf_DPipe *io)
{
	if (Ifx_Shell_matchToken(&args, "?") != FALSE)
    {
        IfxStdIf_DPipe_print(io, "Syntax     : ota enable/disable"ENDL);
        IfxStdIf_DPipe_print(io, "           > Enable/disable OTA function "ENDL);
    }
    else
    {
    	if (Ifx_Shell_matchToken(&args, "enable") != FALSE)
    	{
            //is_ota_enabled = TRUE;
            SetOtaConfig(TRUE);
    	}
    	else if (Ifx_Shell_matchToken(&args, "disable") != FALSE)
    	{
            //is_ota_enabled = FALSE;
            SetOtaConfig(FALSE);
    	}
    	else
    	{
    		if(GetOtaConfig())
    			IfxStdIf_DPipe_print(io, "OTA feature is enabled"ENDL);
    		else
    			IfxStdIf_DPipe_print(io, "OTA feature is disabled"ENDL);
    	}
    }

    return TRUE;
}

void pflash_correct_from_ota()
{
	PFlashWrite(NULL,0,CRC_SECTOR_ADDR,0,1, CRC_SECTOR_CORRECT_VALUE);
}

void pflash_CORRUPT_from_ota()
{
	PFlashWrite(NULL,0,CRC_SECTOR_ADDR,0,1, CRC_CORRUPT_PATTERN);
}

