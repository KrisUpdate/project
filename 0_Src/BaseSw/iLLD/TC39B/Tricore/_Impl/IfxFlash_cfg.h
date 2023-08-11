/**
 * \file IfxFlash_cfg.h
 * \brief FLASH on-chip implementation data
 * \ingroup IfxLld_Flash
 *
 * \version iLLD_1_0_1_9_0
 * \copyright Copyright (c) 2018 Infineon Technologies AG. All rights reserved.
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
 * \defgroup IfxLld_Flash FLASH
 * \ingroup IfxLld
 * \defgroup IfxLld_Flash_Impl Implementation
 * \ingroup IfxLld_Flash
 * \defgroup IfxLld_Flash_Std Standard Driver
 * \ingroup IfxLld_Flash
 */

#ifndef IFXFLASH_CFG_H
#define IFXFLASH_CFG_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "Cpu/Std/IfxCpu_Intrinsics.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/** \brief number of flash modules.
 * This macro may be deprecated in future. Use IFXFLASH_NUM_MODULES instead.
 */
#define IFXFLASH_NUM_FLASH_MODULES                       (1)

/** \brief base address for general command sequences
 */
#define IFXFLASH_CMD_BASE_ADDRESS                        (0xaf000000)

/** \brief offset between command areas (in DFlash range)
 */
#define IFXFLASH_CMD_BASE_OFFSET                         (0x00100000)

#define IFXFLASH_DFLASH_BANKS                            (2)

/** \brief Dflash burst length
 */
#define IFXFLASH_DFLASH_BURST_LENGTH                     (0x20)

#define IFXFLASH_DFLASH_END                              (IFXFLASH_DFLASH_START + IFXFLASH_DFLASH_SIZE - 1)

/** \brief CFS logical sector count
 */
#define IFXFLASH_DFLASH_NUM_CFS_LOG_SECTORS              (16)

/** \brief
 */
#define IFXFLASH_DFLASH_NUM_HSM_LOG_SECTORS              (32)

#define IFXFLASH_DFLASH_NUM_LOG_SECTORS                  (256 + 32)

/** \brief Phy sector for DF
 */
#define IFXFLASH_DFLASH_NUM_PHYSICAL_SECTORS             (1)

#define IFXFLASH_DFLASH_NUM_UCB_LOG_SECTORS              (48)

/** \brief 8 bytes
 */
#define IFXFLASH_DFLASH_PAGE_LENGTH                      (8)

#define IFXFLASH_DFLASH_SIZE                             (IFXFLASH_DFLASH_NUM_LOG_SECTORS * 0x1000)

#define IFXFLASH_DFLASH_START                            (0xaf000000)

#define IFXFLASH_DFLASH_WORDLINE_LENGTH                  (512)

/** \brief
 */
#define IFXFLASH_ERROR_TRACKING_MAX_CORRECTABLE_ERRORS   (10)

/** \brief
 */
#define IFXFLASH_ERROR_TRACKING_MAX_UNCORRECTABLE_ERRORS (1)

/** \brief number of flash modules
 */
#define IFXFLASH_NUM_MODULES                             (1)

#define IFXFLASH_PFLASH_BANKS                            (6)

/** \brief P flash burst length
 */
#define IFXFLASH_PFLASH_BURST_LENGTH                     (0x100)

/** \brief p flash end
 */
#define IFXFLASH_PFLASH_END                              (IFXFLASH_PFLASH_START + IFXFLASH_PFLASH_SIZE - 1)

/** \brief
 */
#define IFXFLASH_PFLASH_NUM_LOG_SECTORS                  ((192 * 5) + 64)

#define IFXFLASH_PFLASH_NUM_PHYSICAL_SECTORS             (3 * 5 + 1)

/** \brief offset between PMU PFlash ranges
 */
#define IFXFLASH_PFLASH_OFFSET                           (0x00800000)

/** \brief PF0 end
 */
#define IFXFLASH_PFLASH_P0_END                           (IFXFLASH_PFLASH_P0_START + IFXFLASH_PFLASH_P0_SIZE - 1)

/** \brief PF0 size
 */
#define IFXFLASH_PFLASH_P0_SIZE                          (0x300000)

/** \brief PF0 start
 */
#define IFXFLASH_PFLASH_P0_START                         (0xa0000000)

/** \brief PF1 end
 */
#define IFXFLASH_PFLASH_P1_END                           (IFXFLASH_PFLASH_P1_START + IFXFLASH_PFLASH_P1_SIZE - 1)

/** \brief PF1 size
 */
#define IFXFLASH_PFLASH_P1_SIZE                          (0x300000)

/** \brief PF1 start
 */
#define IFXFLASH_PFLASH_P1_START                         (IFXFLASH_PFLASH_P0_START + IFXFLASH_PFLASH_P0_SIZE)

/** \brief PF2 end
 */
#define IFXFLASH_PFLASH_P2_END                           (IFXFLASH_PFLASH_P2_START + IFXFLASH_PFLASH_P2_SIZE - 1)

/** \brief PF2 size
 */
#define IFXFLASH_PFLASH_P2_SIZE                          (0x300000)

/** \brief PF2 start
 */
#define IFXFLASH_PFLASH_P2_START                         (IFXFLASH_PFLASH_P1_START + IFXFLASH_PFLASH_P1_SIZE)

/** \brief PF3 end
 */
#define IFXFLASH_PFLASH_P3_END                           (IFXFLASH_PFLASH_P3_START + IFXFLASH_PFLASH_P3_SIZE - 1)

/** \brief PF3 size
 */
#define IFXFLASH_PFLASH_P3_SIZE                          (0x300000)

/** \brief PF3 start
 */
#define IFXFLASH_PFLASH_P3_START                         (IFXFLASH_PFLASH_P2_START + IFXFLASH_PFLASH_P2_SIZE)

/** \brief PF4 end
 */
#define IFXFLASH_PFLASH_P4_END                           (IFXFLASH_PFLASH_P4_START + IFXFLASH_PFLASH_P4_SIZE - 1)

/** \brief PF4 size
 */
#define IFXFLASH_PFLASH_P4_SIZE                          (0x300000)

/** \brief PF4 start
 */
#define IFXFLASH_PFLASH_P4_START                         (IFXFLASH_PFLASH_P3_START + IFXFLASH_PFLASH_P3_SIZE)

/** \brief PF5 end
 */
#define IFXFLASH_PFLASH_P5_END                           (IFXFLASH_PFLASH_P5_START + IFXFLASH_PFLASH_P5_SIZE - 1)

/** \brief PF5 size
 */
#define IFXFLASH_PFLASH_P5_SIZE                          (0x100000)

/** \brief PF5 start
 */
#define IFXFLASH_PFLASH_P5_START                         (IFXFLASH_PFLASH_P4_START + IFXFLASH_PFLASH_P4_SIZE)

#define IFXFLASH_PFLASH_PAGE_LENGTH                      (32)

/** \brief p flash size
 */
#define IFXFLASH_PFLASH_SIZE                             (0x01000000)

/** \brief p flash start
 */
#define IFXFLASH_PFLASH_START                            (0xa0000000)

#define IFXFLASH_PFLASH_WORDLINE_LENGTH                  (1024)

/******************************************************************************/
/*-------------------------------Enumerations---------------------------------*/
/******************************************************************************/

/** \brief
 */
typedef enum
{
    IfxFlash_FlashType_D0 = 0,  /**< \brief data flash #0 */
    IfxFlash_FlashType_D1 = 1,  /**< \brief data flash #1 */
    IfxFlash_FlashType_P0 = 2,  /**< \brief program flash #0 */
    IfxFlash_FlashType_P1 = 3,  /**< \brief program flash #1 */
    IfxFlash_FlashType_P2 = 4,  /**< \brief program flash #2 */
    IfxFlash_FlashType_P3 = 5,  /**< \brief program flash #3 */
    IfxFlash_FlashType_P4 = 6,  /**< \brief program flash #4 */
    IfxFlash_FlashType_P5 = 7,  /**< \brief program flash #5 */
    IfxFlash_FlashType_Fa = 16  /**< \brief program flash array */
} IfxFlash_FlashType;

/** \brief user configuration block type
 */
typedef enum
{
    IfxFlash_UcbType_ucbPflash = 16,  /**< \brief UCB for PFlash */
    IfxFlash_UcbType_ucbDflash = 17,  /**< \brief UCB for DFlash */
    IfxFlash_UcbType_ucbDebug  = 18,  /**< \brief UCB for debug */
    IfxFlash_UcbType_ucbRetest = 15   /**< \brief UCB for Re test */
} IfxFlash_UcbType;

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \brief contains start and end address of sectors
 */
typedef struct
{
    uint32 start;       /**< \brief start address of sector */
    uint32 end;         /**< \brief end address of sector */
} IfxFlash_flashSector;

/******************************************************************************/
/*-------------------Global Exported Variables/Constants----------------------*/
/******************************************************************************/

/** \brief
 */
IFX_EXTERN IFX_CONST IfxFlash_flashSector IfxFlash_dFlashTableCfsLog[IFXFLASH_DFLASH_NUM_CFS_LOG_SECTORS];

IFX_EXTERN IFX_CONST IfxFlash_flashSector IfxFlash_dFlashTableEepLog[IFXFLASH_DFLASH_NUM_LOG_SECTORS];

/** \brief
 */
IFX_EXTERN IFX_CONST IfxFlash_flashSector IfxFlash_dFlashTableHsmLog[IFXFLASH_DFLASH_NUM_HSM_LOG_SECTORS];

IFX_EXTERN IFX_CONST IfxFlash_flashSector IfxFlash_dFlashTablePhys[IFXFLASH_DFLASH_NUM_PHYSICAL_SECTORS];

/** \brief
 */
IFX_EXTERN IFX_CONST IfxFlash_flashSector IfxFlash_dFlashTableUcbLog[IFXFLASH_DFLASH_NUM_UCB_LOG_SECTORS];

IFX_EXTERN IFX_CONST IfxFlash_flashSector IfxFlash_pFlashTableLog[IFXFLASH_PFLASH_NUM_LOG_SECTORS];

IFX_EXTERN IFX_CONST IfxFlash_flashSector IfxFlash_pFlashTablePhys[IFXFLASH_PFLASH_NUM_PHYSICAL_SECTORS];

#endif /* IFXFLASH_CFG_H */
