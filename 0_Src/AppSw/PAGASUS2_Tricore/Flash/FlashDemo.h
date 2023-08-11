/**
 * \file FlashDemo.h
 * \brief  Flash erase & program Demo
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
 *
 * \defgroup IfxLld_Demo_FlashDemo_SrcDoc_Main Demo Source
 * \ingroup IfxLld_Demo_FlashDemo_SrcDoc
 * \defgroup IfxLld_Demo_FlashDemo_SrcDoc_Main_Interrupt Interrupts
 * \ingroup IfxLld_Demo_FlashDemo_SrcDoc_Main
 */

#ifndef FLASHDEMO_H
#define FLASHDEMO_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
//#include <Ifx_Types.h>
//#include "IfxFlash.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/


/** \brief P Flash global data */
typedef struct
{
/** \brief Function pointers of Erase & program routines which are relocated to different memory (PSPR).
 *  The function pointers are used to execute the relocated routines
 */
    struct
    {
        void (*eraseSector)(uint32);
        uint8 (*waitUnbusy)(uint32, int);
        uint8 (*enterPageMode)(uint32);
        void (*loadPage2X32)(uint32, uint32, uint32);
        void (*writePage)(uint32);
        void (*pFlashErase)(uint32, uint32);
        void (*pFlashProgram)(uint32, uint32, uint32, uint32, uint32);
        int (*pFlashType)(uint32);
    }                  command;

    uint32             sector;        /**< \brief Flash sector to be erased & programmed */
    uint32             startPage;     /**< \brief Page from which programming starts */
    uint32             numberOfPages; /**< \brief No of pages to be programmed */
    //IfxFlash_FlashType flashType;     /**< \brief Flash type ussed for the erase/program */
} App_Pflash;

/** \brief D Flash global data */
typedef struct
{
    uint32             sector;              /**< \brief Flash sector to be erased & programmed */
    uint32             startPage;           /**< \brief Page from which programming starts */
    uint32             numberOfPages;       /**< \brief No of pages to be programmed */
    //IfxFlash_FlashType flashType;           /**< \brief Flash type ussed for the erase/program */
} App_Dflash;

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/

extern App_Pflash g_Pflash;
//IFX_EXTERN App_Dflash g_Dflash;


/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
void copyFlashRoutinesToPspr(void);
void PFlashWrite(IfxStdIf_DPipe *io, uint32 flash, uint32 sector_addr, uint32 start_page, uint32 no_of_pages, uint32 pattern);
void PFlashRead(IfxStdIf_DPipe *io, uint32 flash, uint32 sector_addr, uint32 start_page, uint32 no_of_pages);
void OTA_FLASH_ErasePage(uint32 sector_addr, uint32 size);
void OTA_FLASH_ProgramFromBuffer(uint32 addr, uint32* buffer, uint32 size);
void OTA_FLASH_Verify(uint32 sector_addr);
#endif