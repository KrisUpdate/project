/**
 * \file FlashDemo.c
 * \brief Flash erase & program Demo
 *
 * \version iLLD_Demos_1_0_0_11_0
 * \copyright Copyright (c) 2014 Infineon Technologies AG. All rights reserved.
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

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "Platform_Types.h"
#include "Ifx_Types.h"
#include "IfxStdIf_DPipe.h"

#include "FlashDemo.h"
#include "IfxCpu.h"
#include "IfxFlash.h"

#define FLASH_DEMO_DEBUG_ENABLE 1

#if FLASH_DEMO_DEBUG_ENABLE == 1
#define FLASH_DEMO_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define FLASH_DEMO_DEBUG_PRINTF(x)
#endif

#define FLASH_DEMO_PRINTF_ALWAYS(x) DEBUG_PRINTF_ALWAYS(x)

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define FLASHDEMO_PFLASH_PAGE_LENGTH 0x20          /**< \brief P Flash Page Length */


#define FLASHDEMO_RELOC_START_ADDR   (0xC0000000U) /**< \brief Program & Erase routines relocation address */

/** \brief Length of Program & Erase routines
 */
#define FLASHDEMO_ERASESECTOR_LEN    (100)
#define FLASHDEMO_WAITUNBUSY_LEN     (100)
#define FLASHDEMO_ENTERPAGEMODE_LEN  (100)
#define FLASHDEMO_LOADPAGE2X32_LEN   (100)
#define FLASHDEMO_WRITEPAGE_LEN      (100)
#define FLASHDEMO_PFLASHERASE_LEN    (0x100)
#define FLASHDEMO_PFLASHPROGRAM_LEN  (0x10C)
/* Added by NVIDIA, we need to use one more function to get the flash type,
 * instead of making it fix */
#define FLASHDEMO_PFLASHTYPE_LEN     (0x100)

/** \brief Program & Erase routines relocation length */
#define FLASHDEMO_RELOC_LENGTH                                  \
    (FLASHDEMO_ERASESECTOR_LEN + FLASHDEMO_WAITUNBUSY_LEN +     \
     FLASHDEMO_ENTERPAGEMODE_LEN + FLASHDEMO_LOADPAGE2X32_LEN + \
     FLASHDEMO_WRITEPAGE_LEN + FLASHDEMO_PFLASHERASE_LEN +      \
     FLASHDEMO_PFLASHPROGRAM_LEN + FLASHDEMO_PFLASHTYPE_LEN)

/** \brief Start addresses of Program & Erase routines
 */
#define FLASHDEMO_ERASESECTOR_ADDR   (FLASHDEMO_RELOC_START_ADDR)
#define FLASHDEMO_WAITUNBUSY_ADDR    (FLASHDEMO_ERASESECTOR_ADDR + FLASHDEMO_ERASESECTOR_LEN)
#define FLASHDEMO_ENTERPAGEMODE_ADDR (FLASHDEMO_WAITUNBUSY_ADDR + FLASHDEMO_WAITUNBUSY_LEN)
#define FLASHDEMO_LOADPAGE2X32_ADDR  (FLASHDEMO_ENTERPAGEMODE_ADDR + FLASHDEMO_ENTERPAGEMODE_LEN)
#define FLASHDEMO_WRITEPAGE_ADDR     (FLASHDEMO_LOADPAGE2X32_ADDR + FLASHDEMO_LOADPAGE2X32_LEN)
#define FLASHDEMO_PFLASHERASE_ADDR   (FLASHDEMO_WRITEPAGE_ADDR + FLASHDEMO_WRITEPAGE_LEN)
#define FLASHDEMO_PFLASHPROGRAM_ADDR (FLASHDEMO_PFLASHERASE_ADDR + FLASHDEMO_PFLASHERASE_LEN)
#define FLASHDEMO_PFLASHTYPE_ADDR 	 (FLASHDEMO_PFLASHPROGRAM_ADDR + FLASHDEMO_PFLASHPROGRAM_LEN)


/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/

App_Pflash g_Pflash; /**< \brief PFlash global data */
//App_Dflash g_Dflash; /**< \brief DFlash global data */

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
static void PFlashErase(uint32 flash, uint32 sector_addr);
static void PFlashProgram(uint32 flash, uint32 sector_addr, uint32 start_page, uint32 no_of_pages, uint32 pattern);

static int PFlashGetType(uint32 sector_addr);

/* This function is to replace clib_flash.c clib_flash_type() for TC397*/
/* Please be notice that TC39X has 3M bytes for each PF,
 * while TC297 has 2M */
static int PFlashGetType(uint32 addr)
{
	if( (addr >= 0xA0000000) && (addr < 0xA0300000))
		return IfxFlash_FlashType_P0;

	if( (addr >= 0xA0300000) && (addr < 0xA0600000))
		return IfxFlash_FlashType_P1;

	if( (addr >= 0xA0600000) && (addr < 0xA0900000))
		return IfxFlash_FlashType_P2;

	if( (addr >= 0xA0900000) && (addr < 0xA0C00000))
		return IfxFlash_FlashType_P3;

	if( (addr >= 0xA0C00000) && (addr < 0xA0F00000))
		return IfxFlash_FlashType_P4;

	if( (addr >= 0xA0F00000) && (addr < 0xA1000000))
		return IfxFlash_FlashType_P5;

	return IfxFlash_FlashType_P0;
}



/** \brief PFlashErase
 *
 * This function will erase the P-Flash Sector
 * Note: This function shouldn't be executed from Flash it is trying to erase.
 * Recommended to execute the routine from PSPR memory
 */
static void PFlashErase(uint32 flash, uint32 sector_addr)
{

    uint16 endinitSfty_pw;
    int flash_type;
    endinitSfty_pw = IfxScuWdt_getSafetyWatchdogPasswordInline();

    flash_type = g_Pflash.command.pFlashType(sector_addr);
    /* Erase the sector */
    IfxScuWdt_clearSafetyEndinitInline(endinitSfty_pw);
    g_Pflash.command.eraseSector(sector_addr);
    IfxScuWdt_setSafetyEndinitInline(endinitSfty_pw);

    /* wait until unbusy */
    g_Pflash.command.waitUnbusy(flash, flash_type);
}


/** \brief PFlashProgram
 *
 * This function will program the P-Flash Sector
 * Note: This function shouldn't be executed from Flash it is trying to program.
 * Recommended to execute the routine from PSPR memory
 */
static void PFlashProgram(uint32 flash, uint32 sector_addr, uint32 start_page, uint32 no_of_pages, uint32 pattern)
{
    uint32 offset;
    uint32 page;
    uint16 endinitSfty_pw;
    int flash_type;
    endinitSfty_pw = IfxScuWdt_getSafetyWatchdogPasswordInline();

    flash_type = g_Pflash.command.pFlashType(sector_addr);
    /* program the given no of pages */
    for (page = start_page; page < no_of_pages; ++page)
    {
        uint32 pageAddr = sector_addr + page * FLASHDEMO_PFLASH_PAGE_LENGTH;
        g_Pflash.command.enterPageMode(pageAddr);

        /* wait until unbusy */
        g_Pflash.command.waitUnbusy(flash, flash_type);

        /* write 32 bytes (8 doublewords) into assembly buffer */
        for (offset = 0; offset < FLASHDEMO_PFLASH_PAGE_LENGTH; offset += 8)
        {
            g_Pflash.command.loadPage2X32(pageAddr, pattern, pattern);
        }

        /* write page */
        IfxScuWdt_clearSafetyEndinitInline(endinitSfty_pw);
        g_Pflash.command.writePage(pageAddr);
        IfxScuWdt_setSafetyEndinitInline(endinitSfty_pw);

        /* wait until unbusy */
        g_Pflash.command.waitUnbusy(flash, flash_type);
    }
}

void copyFlashRoutinesToPspr(void)
{
	/*
	 * Copy the below Flash routines to PSPR memory & assign a function pointer
	 * IfxFlash_eraseSector, IfxFlash_waitUnbusy, IfxFlash_enterPageMode, IfxFlash_loadPage2X32, IfxFlash_writePage
	 * PFlashErase, PFlashProgram
	 */
	memcpy((void *)FLASHDEMO_ERASESECTOR_ADDR, (const void *)IfxFlash_eraseSector, FLASHDEMO_ERASESECTOR_LEN);
	g_Pflash.command.eraseSector = (void *)FLASHDEMO_RELOC_START_ADDR;

	memcpy((void *)FLASHDEMO_WAITUNBUSY_ADDR, (const void *)IfxFlash_waitUnbusy, FLASHDEMO_WAITUNBUSY_LEN);
	g_Pflash.command.waitUnbusy = (void *)FLASHDEMO_WAITUNBUSY_ADDR;

	memcpy((void *)FLASHDEMO_ENTERPAGEMODE_ADDR, (const void *)IfxFlash_enterPageMode, FLASHDEMO_ENTERPAGEMODE_LEN);
	g_Pflash.command.enterPageMode = (void *)FLASHDEMO_ENTERPAGEMODE_ADDR;

	memcpy((void *)FLASHDEMO_LOADPAGE2X32_ADDR, (const void *)IfxFlash_loadPage2X32, FLASHDEMO_LOADPAGE2X32_LEN);
	g_Pflash.command.loadPage2X32 = (void *)FLASHDEMO_LOADPAGE2X32_ADDR;

	memcpy((void *)FLASHDEMO_WRITEPAGE_ADDR, (const void *)IfxFlash_writePage, FLASHDEMO_WRITEPAGE_LEN);
	g_Pflash.command.writePage = (void *)FLASHDEMO_WRITEPAGE_ADDR;

	memcpy((void *)FLASHDEMO_PFLASHERASE_ADDR, (const void *)PFlashErase, FLASHDEMO_PFLASHERASE_LEN);
	g_Pflash.command.pFlashErase = (void *)FLASHDEMO_PFLASHERASE_ADDR;

	memcpy((void *)FLASHDEMO_PFLASHPROGRAM_ADDR, (const void *)PFlashProgram, FLASHDEMO_PFLASHPROGRAM_LEN);
	g_Pflash.command.pFlashProgram = (void *)FLASHDEMO_PFLASHPROGRAM_ADDR;

	memcpy((void *)FLASHDEMO_PFLASHTYPE_ADDR, (const void *)PFlashGetType, FLASHDEMO_PFLASHTYPE_LEN);
	g_Pflash.command.pFlashType = (void *)FLASHDEMO_PFLASHTYPE_ADDR;
}

void PFlashWrite(IfxStdIf_DPipe *io, uint32 flash, uint32 sector_addr, uint32 start_page, uint32 no_of_pages, uint32 pattern)
{
    uint32  errors      = 0;
    uint32  offset;
    uint32  page        = 0;

    //IfxStdIf_DPipe_print(io, "Step 2"ENDL);
    /* disable interrupts */
    boolean interruptState = IfxCpu_disableInterrupts();

    /* Copy the erase and program routines to PSPR memory */
    copyFlashRoutinesToPspr();

    /* erase & program flash (execute from relocated memory)*/
    //IfxStdIf_DPipe_print(io, "Step 3"ENDL);
    g_Pflash.command.pFlashErase(flash, sector_addr);

    //IfxStdIf_DPipe_print(io, "Step 4"ENDL);
    g_Pflash.command.pFlashProgram(flash, sector_addr, start_page, no_of_pages, pattern);

    //IfxStdIf_DPipe_print(io, "Step 5"ENDL);
    /* enable interrupts again */
    IfxCpu_restoreInterrupts(interruptState);

    /* Verify the programmed data */
    for (page = start_page; page < no_of_pages; ++page)
    {
        uint32          pageAddr = sector_addr + page * FLASHDEMO_PFLASH_PAGE_LENGTH;
        volatile uint8 *addr     = (uint8 *)pageAddr;

        for (offset = 0; offset < FLASHDEMO_PFLASH_PAGE_LENGTH; offset += 8)
        {
            if (!((pattern == *((uint32 *)(addr + offset))) &&
                  (addr[offset + 4] == pattern)))
            {
                errors++;
            }
        }
    }

    //IfxStdIf_DPipe_print(io, "Step 6"ENDL);
    //IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, errors == 0);

    if (errors)
    {
    	//IfxStdIf_DPipe_print(io, "ERROR: error while P-Flash erase / program"ENDL);
        //printf("ERROR: error while P-Flash erase / program\n");
    }
    else
    {
    	//IfxStdIf_DPipe_print(io, "OK: P-Flash checks passed"ENDL);
        //printf("OK: P-Flash checks passed\n");
    }
}

uint32 PFlashReadByte(uint32 sector_addr, uint32 no_of_byte)
{
    //IfxStdIf_DPipe_print(io, "at 0x%08x value is 0x%08x"ENDL,sector_addr, *((uint32 *)(sector_addr)));
    uint32 v = *((uint32 *)(sector_addr));

    v &= 0xff<<8*(no_of_byte);
    v = v>>8*(no_of_byte);
    //IfxStdIf_DPipe_print(io, "0x%02x "ENDL, v);
    return v;
}

void PFlashRead(IfxStdIf_DPipe *io, uint32 flash, uint32 sector_addr, uint32 start_page, uint32 no_of_pages)
{
    uint32 offset;
    uint32 page;

    /* program the given no of pages */
    for (page = start_page; page < no_of_pages; ++page)
    {
        uint32 pageAddr = sector_addr + page * FLASHDEMO_PFLASH_PAGE_LENGTH;
        volatile uint8 *addr     = (uint8 *)pageAddr;

         for (offset = 0; offset < FLASHDEMO_PFLASH_PAGE_LENGTH; offset += 8)
         {
             IfxStdIf_DPipe_print(io, "0x%08x = 0x%08x"ENDL,pageAddr, *((uint32 *)(addr + offset)));
             IfxStdIf_DPipe_print(io, "0x%08x = 0x%08x"ENDL,pageAddr+4, *((uint32 *)(addr + offset + 4)));
             pageAddr = pageAddr + 8;
         }
    }
}

/* Below is the flash operation function that used by OTA */

void OTA_FLASH_ErasePage(uint32 sector_addr, uint32 size)
{
	 /* disable interrupts */
	boolean interruptState = IfxCpu_disableInterrupts();

	/* Copy the erase and program routines to PSPR memory */
	copyFlashRoutinesToPspr();
	if(size >= 0x4000)
	{
		do
		{
			g_Pflash.command.pFlashErase(0, sector_addr);
			sector_addr+= 0x4000;
			size-=0x4000;
		}while(size>=0x4000);
	}
	else
	{
		/* erase & program flash (execute from relocated memory)*/
		g_Pflash.command.pFlashErase(0, sector_addr);
	}
	IfxCpu_restoreInterrupts(interruptState);

}


void OTA_FLASH_ProgramFromBuffer(uint32 addr, uint32* buffer, uint32 size)
{
	int flash_type;
	uint32 offset;
	uint32 page;
	uint16 endinitSfty_pw;
	uint32* buffer_lp = buffer;
	 /* disable interrupts */
	boolean interruptState = IfxCpu_disableInterrupts();

	/* Copy the erase and program routines to PSPR memory */
	copyFlashRoutinesToPspr();

	endinitSfty_pw = IfxScuWdt_getSafetyWatchdogPasswordInline();

	flash_type = g_Pflash.command.pFlashType(addr);
	/* program the given no of pages */
	do
	{

		g_Pflash.command.enterPageMode(addr);

		/* wait until unbusy */
		g_Pflash.command.waitUnbusy(0, flash_type);

		/* write 32 bytes (8 doublewords) into assembly buffer */
		for (offset = 0; offset < FLASHDEMO_PFLASH_PAGE_LENGTH; offset += 8)
		{
			uint32 lword = (*buffer_lp);
			buffer_lp++;
			uint32 uword = (*buffer_lp);
			buffer_lp++;
			g_Pflash.command.loadPage2X32(addr, lword, uword);
		}

		/* write page */
		IfxScuWdt_clearSafetyEndinitInline(endinitSfty_pw);
		g_Pflash.command.writePage(addr);
		IfxScuWdt_setSafetyEndinitInline(endinitSfty_pw);

		/* wait until unbusy */
		g_Pflash.command.waitUnbusy(0, flash_type);

		addr+=FLASHDEMO_PFLASH_PAGE_LENGTH;
		size-=FLASHDEMO_PFLASH_PAGE_LENGTH;
	}while(size>0);


	IfxCpu_restoreInterrupts(interruptState);
}

void OTA_FLASH_Verify(uint32 sector_addr)
{

}

