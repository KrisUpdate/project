/**
 * \file IfxCpu_Bmhd.c
 * \brief This file contains the Bmhd for Aurix Plus.
 *
 * \copyright Copyright (c) 2018 Infineon Technologies AG. All rights reserved.
 *
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
 */
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Configuration.h"
#include "Ifx_Ssw.h"
#include "Platform_Types.h"


/*******************************************************************************
**                      Type definitions                                     **
*******************************************************************************/


/*******************************************************************************
**                              BMHD constants                                **
*******************************************************************************/
#if PARTITION_LAYOUT == _QPARTITION
#if defined(__GNUC__)
#pragma section
#pragma section ".bmhd_0_orig" a
#endif
#if defined(__TASKING__)
#pragma section farrom "bmhd_0_orig"
#endif
#if defined(__DCC__)
#pragma section CONST ".bmhd_0_orig" far-absolute R
#endif
#if defined(__ghs__)
#pragma ghs section rodata= ".bmhd_0_orig"
#endif

/* This point to the ABMHD */
const Ifx_Ssw_Bmhd bmhd_0_orig=
{
    0x00FC,         /**< \brief 0x000: .bmi: Boot Mode Index (BMI)*/
    0xB359,         /**< \brief 0x002: .bmhdid: Boot Mode Header ID (CODE) = B359H*/
    0xA0C04000,     /**< \brief 0x004: .stad: User Code start address*/
    0x2A6FDF55,     /**< \brief 0x008: .crc: Check Result for the BMI Header (offset 000H - 007H)*/
    0xd59020aa,     /**< \brief 0x00C: .crcInv: Inverted Check Result for the BMI Header (offset 000H - 007H)*/
    {
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x010: Reserved (0x010 - 0x01F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x020: Reserved (0x020 - 0x02F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x030: Reserved (0x030 - 0x03F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x040: Reserved (0x040 - 0x04F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x050: Reserved (0x050 - 0x05F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x060: Reserved (0x060 - 0x06F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x070: Reserved (0x070 - 0x07F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x080: Reserved (0x080 - 0x08F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x090: Reserved (0x090 - 0x09F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0A0: Reserved (0x0A0 - 0x0AF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0B0: Reserved (0x0B0 - 0x0BF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0C0: Reserved (0x0C0 - 0x0CF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0D0: Reserved (0x0D0 - 0x0DF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0E0: Reserved (0x0E0 - 0x0EF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000         /**< \brief 0x0F0: Reserved (0x0F0 - 0x0FF) */
    },
    {
        0x00000000, /**< \brief 0x100: .pw0: Password protection word 0 (lsw) */
        0x00000000, /**< \brief 0x104: .pw1: Password protection word 1 */
        0x00000000, /**< \brief 0x108: .pw2: Password protection word 2 */
        0x00000000, /**< \brief 0x10C: .pw3: Password protection word 3 */
        0x00000000, /**< \brief 0x110: .pw4: Password protection word 4 */
        0x00000000, /**< \brief 0x114: .pw5: Password protection word 5 */
        0x00000000, /**< \brief 0x118: .pw6: Password protection word 6 */
        0x00000000, /**< \brief 0x11C: .pw7: Password protection word 7 (msw) */
    },
    {
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x120: Reserved (0x120 - 0x12F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x130: Reserved (0x130 - 0x13F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x140: Reserved (0x140 - 0x14F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x150: Reserved (0x150 - 0x15F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x160: Reserved (0x160 - 0x16F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x170: Reserved (0x170 - 0x17F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x180: Reserved (0x180 - 0x18F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x190: Reserved (0x190 - 0x19F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1A0: Reserved (0x1A0 - 0x1AF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1B0: Reserved (0x1B0 - 0x1BF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1C0: Reserved (0x1C0 - 0x1CF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1D0: Reserved (0x1D0 - 0x1DF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000         /**< \brief 0x1E0: Reserved (0x1E0 - 0x1EF) */
    },
    0x43211234      /**< \brief 0x1F0: .confirmation: 32-bit CODE, (always same)*/
};

/* we need to put this in 0xA0604000, which bmhd_0_orig points to*/
#if defined(__GNUC__)
#pragma section
#pragma section ".abmhd_0"
#endif
#if defined(__TASKING__)
#pragma section farrom "abmhd_0"
#endif
#if defined(__DCC__)
#pragma section CONST ".abmhd_0" far-absolute R
#endif

/* This ABMHD will point to production f/w if the crc is correct */
const uint32 ABMHD_PROD[] = {
	0xA0000000u,                 //0xA0000000u,                 /* STADBM first user code at 0x8000 0000h */
    0xFA7CB359u,                 /* BMI = 0070h BMHDID = B359h */
    0xa0c00000u,                 /* ChkStart */
	0xa0c00000u,                 /* ChkEnd */
	0x24f80d39u,                 /* CRCrange */
	0xdb07f2c6u,                 /* !CRCrange */
	0xEB52F46Fu,                 /* CRChead */
	0x14ad0b90u                  /* !CRChead */
};

/* we need to put this in 0xA0604000, which bmhd_0_orig points to*/
#if defined(__GNUC__)
#pragma section
#pragma section ".abmhd_check_address_pfls7"
#endif
#if defined(__TASKING__)
#pragma section farrom "abmhd_check_address_pfls7"
#endif
#if defined(__DCC__)
#pragma section CONST ".abmhd_check_address_pfls7" far-absolute R
#endif

const uint32 ABMHD_CHECK_VALUE_PFLS7 = {0xA0600000};


/* BMHD_0_COPY point to update F/W */
#if defined(__GNUC__)
#pragma section
#pragma section ".bmhd_0_copy" a
#endif
#if defined(__TASKING__)
#pragma section farrom "bmhd_0_copy"
#endif
#if defined(__DCC__)
#pragma section CONST ".bmhd_0_copy" far-absolute R
#endif
#if defined(__ghs__)
#pragma ghs section rodata= ".bmhd_0_copy"
#endif
const Ifx_Ssw_Bmhd bmhd_0_copy=
{
    0x00FE,         /**< \brief 0x000: .bmi: Boot Mode Index (BMI)*/
    0xB359,         /**< \brief 0x002: .bmhdid: Boot Mode Header ID (CODE) = B359H*/
    0xA0B00000,     /**< \brief 0x004: .stad: User Code start address*/
    0xF427AB60,     /**< \brief 0x008: .crc: Check Result for the BMI Header (offset 000H - 007H)*/
    0x0bd8549f,     /**< \brief 0x00C: .crcInv: Inverted Check Result for the BMI Header (offset 000H - 007H)*/
    {
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x010: Reserved (0x010 - 0x01F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x020: Reserved (0x020 - 0x02F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x030: Reserved (0x030 - 0x03F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x040: Reserved (0x040 - 0x04F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x050: Reserved (0x050 - 0x05F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x060: Reserved (0x060 - 0x06F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x070: Reserved (0x070 - 0x07F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x080: Reserved (0x080 - 0x08F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x090: Reserved (0x090 - 0x09F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0A0: Reserved (0x0A0 - 0x0AF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0B0: Reserved (0x0B0 - 0x0BF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0C0: Reserved (0x0C0 - 0x0CF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0D0: Reserved (0x0D0 - 0x0DF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0E0: Reserved (0x0E0 - 0x0EF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000         /**< \brief 0x0F0: Reserved (0x0F0 - 0x0FF) */
    },
    {
        0x00000000, /**< \brief 0x100: .pw0: Password protection word 0 (lsw) */
        0x00000000, /**< \brief 0x104: .pw1: Password protection word 1 */
        0x00000000, /**< \brief 0x108: .pw2: Password protection word 2 */
        0x00000000, /**< \brief 0x10C: .pw3: Password protection word 3 */
        0x00000000, /**< \brief 0x110: .pw4: Password protection word 4 */
        0x00000000, /**< \brief 0x114: .pw5: Password protection word 5 */
        0x00000000, /**< \brief 0x118: .pw6: Password protection word 6 */
        0x00000000, /**< \brief 0x11C: .pw7: Password protection word 7 (msw) */
    },
    {
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x120: Reserved (0x120 - 0x12F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x130: Reserved (0x120 - 0x13F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x140: Reserved (0x120 - 0x14F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x150: Reserved (0x120 - 0x15F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x160: Reserved (0x120 - 0x16F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x170: Reserved (0x120 - 0x17F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x180: Reserved (0x120 - 0x18F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x190: Reserved (0x120 - 0x19F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1A0: Reserved (0x120 - 0x1AF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1B0: Reserved (0x120 - 0x1BF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1C0: Reserved (0x120 - 0x1CF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1D0: Reserved (0x120 - 0x1DF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1E0: Reserved (0x120 - 0x1EF) */
    },
    0x43211234      /**< \brief 0x1F0: .confirmation: 32-bit CODE, (always same)*/
};

/* rest of the BMHD should point to prod F/W as back up*/
#if defined(__GNUC__)
#pragma section
#pragma section ".bmhd_1_orig" a
#endif
#if defined(__TASKING__)
#pragma section farrom "bmhd_1_orig"
#endif
#if defined(__DCC__)
#pragma section CONST ".bmhd_1_orig" far-absolute R
#endif
#if defined(__ghs__)
#pragma ghs section rodata= ".bmhd_1_orig"
#endif
const Ifx_Ssw_Bmhd bmhd_1_orig=
{
	0x00FE,         /**< \brief 0x000: .bmi: Boot Mode Index (BMI)*/
	0xB359,         /**< \brief 0x002: .bmhdid: Boot Mode Header ID (CODE) = B359H*/
	0xA0B00000,     /**< \brief 0x004: .stad: User Code start address*/
	0xF427AB60,     /**< \brief 0x008: .crc: Check Result for the BMI Header (offset 000H - 007H)*/
	0x0bd8549f,     /**< \brief 0x00C: .crcInv: Inverted Check Result for the BMI Header (offset 000H - 007H)*/
	{
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x010: Reserved (0x010 - 0x01F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x020: Reserved (0x020 - 0x02F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x030: Reserved (0x030 - 0x03F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x040: Reserved (0x040 - 0x04F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x050: Reserved (0x050 - 0x05F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x060: Reserved (0x060 - 0x06F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x070: Reserved (0x070 - 0x07F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x080: Reserved (0x080 - 0x08F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x090: Reserved (0x090 - 0x09F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0A0: Reserved (0x0A0 - 0x0AF) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0B0: Reserved (0x0B0 - 0x0BF) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0C0: Reserved (0x0C0 - 0x0CF) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0D0: Reserved (0x0D0 - 0x0DF) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0E0: Reserved (0x0E0 - 0x0EF) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000         /**< \brief 0x0F0: Reserved (0x0F0 - 0x0FF) */
	},
	{
		0x00000000, /**< \brief 0x100: .pw0: Password protection word 0 (lsw) */
		0x00000000, /**< \brief 0x104: .pw1: Password protection word 1 */
		0x00000000, /**< \brief 0x108: .pw2: Password protection word 2 */
		0x00000000, /**< \brief 0x10C: .pw3: Password protection word 3 */
		0x00000000, /**< \brief 0x110: .pw4: Password protection word 4 */
		0x00000000, /**< \brief 0x114: .pw5: Password protection word 5 */
		0x00000000, /**< \brief 0x118: .pw6: Password protection word 6 */
		0x00000000, /**< \brief 0x11C: .pw7: Password protection word 7 (msw) */
	},
	{
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x120: Reserved (0x120 - 0x12F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x130: Reserved (0x120 - 0x13F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x140: Reserved (0x120 - 0x14F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x150: Reserved (0x120 - 0x15F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x160: Reserved (0x120 - 0x16F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x170: Reserved (0x120 - 0x17F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x180: Reserved (0x120 - 0x18F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x190: Reserved (0x120 - 0x19F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1A0: Reserved (0x120 - 0x1AF) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1B0: Reserved (0x120 - 0x1BF) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1C0: Reserved (0x120 - 0x1CF) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1D0: Reserved (0x120 - 0x1DF) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1E0: Reserved (0x120 - 0x1EF) */
	},
	0x43211234      /**< \brief 0x1F0: .confirmation: 32-bit CODE, (always same)*/
};

#else

#if defined(__GNUC__)
#pragma section
#pragma section ".bmhd_0_orig" a
#endif
#if defined(__TASKING__)
#pragma section farrom "bmhd_0_orig"
#endif
#if defined(__DCC__)
#pragma section CONST ".bmhd_0_orig" far-absolute R
#endif
#if defined(__ghs__)
#pragma ghs section rodata= ".bmhd_0_orig"
#endif
/* This point to the ABMHD */
const Ifx_Ssw_Bmhd bmhd_0_orig=
{
    0x00FC,         /**< \brief 0x000: .bmi: Boot Mode Index (BMI)*/
    0xB359,         /**< \brief 0x002: .bmhdid: Boot Mode Header ID (CODE) = B359H*/
    0xA0604000,     /**< \brief 0x004: .stad: User Code start address*/
    0xf3178235,     /**< \brief 0x008: .crc: Check Result for the BMI Header (offset 000H - 007H)*/
    0x0ce87dca,     /**< \brief 0x00C: .crcInv: Inverted Check Result for the BMI Header (offset 000H - 007H)*/
    {
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x010: Reserved (0x010 - 0x01F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x020: Reserved (0x020 - 0x02F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x030: Reserved (0x030 - 0x03F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x040: Reserved (0x040 - 0x04F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x050: Reserved (0x050 - 0x05F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x060: Reserved (0x060 - 0x06F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x070: Reserved (0x070 - 0x07F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x080: Reserved (0x080 - 0x08F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x090: Reserved (0x090 - 0x09F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0A0: Reserved (0x0A0 - 0x0AF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0B0: Reserved (0x0B0 - 0x0BF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0C0: Reserved (0x0C0 - 0x0CF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0D0: Reserved (0x0D0 - 0x0DF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0E0: Reserved (0x0E0 - 0x0EF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000         /**< \brief 0x0F0: Reserved (0x0F0 - 0x0FF) */
    },
    {
        0x00000000, /**< \brief 0x100: .pw0: Password protection word 0 (lsw) */
        0x00000000, /**< \brief 0x104: .pw1: Password protection word 1 */
        0x00000000, /**< \brief 0x108: .pw2: Password protection word 2 */
        0x00000000, /**< \brief 0x10C: .pw3: Password protection word 3 */
        0x00000000, /**< \brief 0x110: .pw4: Password protection word 4 */
        0x00000000, /**< \brief 0x114: .pw5: Password protection word 5 */
        0x00000000, /**< \brief 0x118: .pw6: Password protection word 6 */
        0x00000000, /**< \brief 0x11C: .pw7: Password protection word 7 (msw) */
    },
    {
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x120: Reserved (0x120 - 0x12F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x130: Reserved (0x130 - 0x13F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x140: Reserved (0x140 - 0x14F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x150: Reserved (0x150 - 0x15F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x160: Reserved (0x160 - 0x16F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x170: Reserved (0x170 - 0x17F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x180: Reserved (0x180 - 0x18F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x190: Reserved (0x190 - 0x19F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1A0: Reserved (0x1A0 - 0x1AF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1B0: Reserved (0x1B0 - 0x1BF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1C0: Reserved (0x1C0 - 0x1CF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1D0: Reserved (0x1D0 - 0x1DF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000         /**< \brief 0x1E0: Reserved (0x1E0 - 0x1EF) */
    },
    0x43211234      /**< \brief 0x1F0: .confirmation: 32-bit CODE, (always same)*/
};

/* we need to put this in 0xA0604000, which bmhd_0_orig points to*/
#if defined(__GNUC__)
#pragma section
#pragma section ".abmhd_0"
#endif
#if defined(__TASKING__)
#pragma section farrom "abmhd_0"
#endif
#if defined(__DCC__)
#pragma section CONST ".abmhd_0" far-absolute R
#endif

/* This ABMHD will point to production f/w if the crc is correct */
const uint32 ABMHD_PROD[] = {
	0xA0000000u,                 /* STADBM first user code at 0x8000 0000h */
    0xFA7CB359u,                 /* BMI = 0070h BMHDID = B359h */
    0xa0600000u,                 /* ChkStart */
	0xa0600000u,                 /* ChkEnd */
	0x24f80d39u,                 /* CRCrange */
	0xdb07f2c6u,                 /* !CRCrange */
	0xdc6d8002,                 /* CRChead */
	0x23927ffd                  /* !CRChead */
};

/* we need to put this in 0xA0604000, which bmhd_0_orig points to*/
#if defined(__GNUC__)
#pragma section
#pragma section ".abmhd_check_address"
#endif
#if defined(__TASKING__)
#pragma section farrom "abmhd_check_address"
#endif
#if defined(__DCC__)
#pragma section CONST ".abmhd_check_address" far-absolute R
#endif

const uint32 ABMHD_CHECK_VALUE = {0xA0600000};


/* BMHD_0_COPY point to update F/W */
#if defined(__GNUC__)
#pragma section
#pragma section ".bmhd_0_copy" a
#endif
#if defined(__TASKING__)
#pragma section farrom "bmhd_0_copy"
#endif
#if defined(__DCC__)
#pragma section CONST ".bmhd_0_copy" far-absolute R
#endif
#if defined(__ghs__)
#pragma ghs section rodata= ".bmhd_0_copy"
#endif
const Ifx_Ssw_Bmhd bmhd_0_copy=
{
    0x00FE,         /**< \brief 0x000: .bmi: Boot Mode Index (BMI)*/
    0xB359,         /**< \brief 0x002: .bmhdid: Boot Mode Header ID (CODE) = B359H*/
    0xA0D00000,     /**< \brief 0x004: .stad: User Code start address*/
    0xbcf06040,     /**< \brief 0x008: .crc: Check Result for the BMI Header (offset 000H - 007H)*/
    0x430f9fbf,     /**< \brief 0x00C: .crcInv: Inverted Check Result for the BMI Header (offset 000H - 007H)*/
    {
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x010: Reserved (0x010 - 0x01F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x020: Reserved (0x020 - 0x02F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x030: Reserved (0x030 - 0x03F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x040: Reserved (0x040 - 0x04F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x050: Reserved (0x050 - 0x05F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x060: Reserved (0x060 - 0x06F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x070: Reserved (0x070 - 0x07F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x080: Reserved (0x080 - 0x08F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x090: Reserved (0x090 - 0x09F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0A0: Reserved (0x0A0 - 0x0AF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0B0: Reserved (0x0B0 - 0x0BF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0C0: Reserved (0x0C0 - 0x0CF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0D0: Reserved (0x0D0 - 0x0DF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0E0: Reserved (0x0E0 - 0x0EF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000         /**< \brief 0x0F0: Reserved (0x0F0 - 0x0FF) */
    },
    {
        0x00000000, /**< \brief 0x100: .pw0: Password protection word 0 (lsw) */
        0x00000000, /**< \brief 0x104: .pw1: Password protection word 1 */
        0x00000000, /**< \brief 0x108: .pw2: Password protection word 2 */
        0x00000000, /**< \brief 0x10C: .pw3: Password protection word 3 */
        0x00000000, /**< \brief 0x110: .pw4: Password protection word 4 */
        0x00000000, /**< \brief 0x114: .pw5: Password protection word 5 */
        0x00000000, /**< \brief 0x118: .pw6: Password protection word 6 */
        0x00000000, /**< \brief 0x11C: .pw7: Password protection word 7 (msw) */
    },
    {
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x120: Reserved (0x120 - 0x12F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x130: Reserved (0x120 - 0x13F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x140: Reserved (0x120 - 0x14F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x150: Reserved (0x120 - 0x15F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x160: Reserved (0x120 - 0x16F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x170: Reserved (0x120 - 0x17F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x180: Reserved (0x120 - 0x18F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x190: Reserved (0x120 - 0x19F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1A0: Reserved (0x120 - 0x1AF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1B0: Reserved (0x120 - 0x1BF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1C0: Reserved (0x120 - 0x1CF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1D0: Reserved (0x120 - 0x1DF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1E0: Reserved (0x120 - 0x1EF) */
    },
    0x43211234      /**< \brief 0x1F0: .confirmation: 32-bit CODE, (always same)*/
};

/* rest of the BMHD should point to prod F/W as back up*/
#if defined(__GNUC__)
#pragma section
#pragma section ".bmhd_1_orig" a
#endif
#if defined(__TASKING__)
#pragma section farrom "bmhd_1_orig"
#endif
#if defined(__DCC__)
#pragma section CONST ".bmhd_1_orig" far-absolute R
#endif
#if defined(__ghs__)
#pragma ghs section rodata= ".bmhd_1_orig"
#endif
const Ifx_Ssw_Bmhd bmhd_1_orig=
{
	0x00FE,         /**< \brief 0x000: .bmi: Boot Mode Index (BMI)*/
	0xB359,         /**< \brief 0x002: .bmhdid: Boot Mode Header ID (CODE) = B359H*/
	0xA0D00000,     /**< \brief 0x004: .stad: User Code start address*/
	0xbcf06040,     /**< \brief 0x008: .crc: Check Result for the BMI Header (offset 000H - 007H)*/
	0x430f9fbf,     /**< \brief 0x00C: .crcInv: Inverted Check Result for the BMI Header (offset 000H - 007H)*/
	{
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x010: Reserved (0x010 - 0x01F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x020: Reserved (0x020 - 0x02F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x030: Reserved (0x030 - 0x03F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x040: Reserved (0x040 - 0x04F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x050: Reserved (0x050 - 0x05F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x060: Reserved (0x060 - 0x06F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x070: Reserved (0x070 - 0x07F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x080: Reserved (0x080 - 0x08F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x090: Reserved (0x090 - 0x09F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0A0: Reserved (0x0A0 - 0x0AF) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0B0: Reserved (0x0B0 - 0x0BF) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0C0: Reserved (0x0C0 - 0x0CF) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0D0: Reserved (0x0D0 - 0x0DF) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0E0: Reserved (0x0E0 - 0x0EF) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000         /**< \brief 0x0F0: Reserved (0x0F0 - 0x0FF) */
	},
	{
		0x00000000, /**< \brief 0x100: .pw0: Password protection word 0 (lsw) */
		0x00000000, /**< \brief 0x104: .pw1: Password protection word 1 */
		0x00000000, /**< \brief 0x108: .pw2: Password protection word 2 */
		0x00000000, /**< \brief 0x10C: .pw3: Password protection word 3 */
		0x00000000, /**< \brief 0x110: .pw4: Password protection word 4 */
		0x00000000, /**< \brief 0x114: .pw5: Password protection word 5 */
		0x00000000, /**< \brief 0x118: .pw6: Password protection word 6 */
		0x00000000, /**< \brief 0x11C: .pw7: Password protection word 7 (msw) */
	},
	{
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x120: Reserved (0x120 - 0x12F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x130: Reserved (0x120 - 0x13F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x140: Reserved (0x120 - 0x14F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x150: Reserved (0x120 - 0x15F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x160: Reserved (0x120 - 0x16F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x170: Reserved (0x120 - 0x17F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x180: Reserved (0x120 - 0x18F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x190: Reserved (0x120 - 0x19F) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1A0: Reserved (0x120 - 0x1AF) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1B0: Reserved (0x120 - 0x1BF) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1C0: Reserved (0x120 - 0x1CF) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1D0: Reserved (0x120 - 0x1DF) */
		0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1E0: Reserved (0x120 - 0x1EF) */
	},
	0x43211234      /**< \brief 0x1F0: .confirmation: 32-bit CODE, (always same)*/
};


#endif


#if defined(__GNUC__)
#pragma section
#pragma section ".bmhd_1_copy" a
#endif
#if defined(__TASKING__)
#pragma section farrom "bmhd_1_copy"
#endif
#if defined(__DCC__)
#pragma section CONST ".bmhd_1_copy" far-absolute R
#endif
#if defined(__ghs__)
#pragma ghs section rodata= ".bmhd_1_copy"
#endif
const Ifx_Ssw_Bmhd bmhd_1_copy=
{
    0x00FE,         /**< \brief 0x000: .bmi: Boot Mode Index (BMI)*/
    0xB359,         /**< \brief 0x002: .bmhdid: Boot Mode Header ID (CODE) = B359H*/
    0xA0000000,     /**< \brief 0x004: .stad: User Code start address*/
    0x31795570,     /**< \brief 0x008: .crc: Check Result for the BMI Header (offset 000H - 007H)*/
    0xCE86AA8F,     /**< \brief 0x00C: .crcInv: Inverted Check Result for the BMI Header (offset 000H - 007H)*/
    {
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x010: Reserved (0x010 - 0x01F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x020: Reserved (0x020 - 0x02F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x030: Reserved (0x030 - 0x03F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x040: Reserved (0x040 - 0x04F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x050: Reserved (0x050 - 0x05F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x060: Reserved (0x060 - 0x06F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x070: Reserved (0x070 - 0x07F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x080: Reserved (0x080 - 0x08F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x090: Reserved (0x090 - 0x09F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0A0: Reserved (0x0A0 - 0x0AF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0B0: Reserved (0x0B0 - 0x0BF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0C0: Reserved (0x0C0 - 0x0CF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0D0: Reserved (0x0D0 - 0x0DF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0E0: Reserved (0x0E0 - 0x0EF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000         /**< \brief 0x0F0: Reserved (0x0F0 - 0x0FF) */
    },
    {
        0x00000000, /**< \brief 0x100: .pw0: Password protection word 0 (lsw) */
        0x00000000, /**< \brief 0x104: .pw1: Password protection word 1 */
        0x00000000, /**< \brief 0x108: .pw2: Password protection word 2 */
        0x00000000, /**< \brief 0x10C: .pw3: Password protection word 3 */
        0x00000000, /**< \brief 0x110: .pw4: Password protection word 4 */
        0x00000000, /**< \brief 0x114: .pw5: Password protection word 5 */
        0x00000000, /**< \brief 0x118: .pw6: Password protection word 6 */
        0x00000000, /**< \brief 0x11C: .pw7: Password protection word 7 (msw) */
    },
    {
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x120: Reserved (0x120 - 0x12F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x130: Reserved (0x120 - 0x13F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x140: Reserved (0x120 - 0x14F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x150: Reserved (0x120 - 0x15F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x160: Reserved (0x120 - 0x16F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x170: Reserved (0x120 - 0x17F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x180: Reserved (0x120 - 0x18F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x190: Reserved (0x120 - 0x19F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1A0: Reserved (0x120 - 0x1AF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1B0: Reserved (0x120 - 0x1BF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1C0: Reserved (0x120 - 0x1CF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1D0: Reserved (0x120 - 0x1DF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1E0: Reserved (0x120 - 0x1EF) */
    },
    0x43211234      /**< \brief 0x1F0: .confirmation: 32-bit CODE, (always same)*/
};

#if defined(__GNUC__)
#pragma section
#pragma section ".bmhd_2_orig" a
#endif
#if defined(__TASKING__)
#pragma section farrom "bmhd_2_orig"
#endif
#if defined(__DCC__)
#pragma section CONST ".bmhd_2_orig" far-absolute R
#endif
#if defined(__ghs__)
#pragma ghs section rodata= ".bmhd_2_orig"
#endif
const Ifx_Ssw_Bmhd bmhd_2_orig=
{
    0x00FE,         /**< \brief 0x000: .bmi: Boot Mode Index (BMI)*/
    0xB359,         /**< \brief 0x002: .bmhdid: Boot Mode Header ID (CODE) = B359H*/
    0xA0000000,     /**< \brief 0x004: .stad: User Code start address*/
    0x31795570,     /**< \brief 0x008: .crc: Check Result for the BMI Header (offset 000H - 007H)*/
    0xCE86AA8F,     /**< \brief 0x00C: .crcInv: Inverted Check Result for the BMI Header (offset 000H - 007H)*/
    {
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x010: Reserved (0x010 - 0x01F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x020: Reserved (0x020 - 0x02F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x030: Reserved (0x030 - 0x03F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x040: Reserved (0x040 - 0x04F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x050: Reserved (0x050 - 0x05F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x060: Reserved (0x060 - 0x06F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x070: Reserved (0x070 - 0x07F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x080: Reserved (0x080 - 0x08F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x090: Reserved (0x090 - 0x09F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0A0: Reserved (0x0A0 - 0x0AF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0B0: Reserved (0x0B0 - 0x0BF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0C0: Reserved (0x0C0 - 0x0CF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0D0: Reserved (0x0D0 - 0x0DF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0E0: Reserved (0x0E0 - 0x0EF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000         /**< \brief 0x0F0: Reserved (0x0F0 - 0x0FF) */
    },
    {
        0x00000000, /**< \brief 0x100: .pw0: Password protection word 0 (lsw) */
        0x00000000, /**< \brief 0x104: .pw1: Password protection word 1 */
        0x00000000, /**< \brief 0x108: .pw2: Password protection word 2 */
        0x00000000, /**< \brief 0x10C: .pw3: Password protection word 3 */
        0x00000000, /**< \brief 0x110: .pw4: Password protection word 4 */
        0x00000000, /**< \brief 0x114: .pw5: Password protection word 5 */
        0x00000000, /**< \brief 0x118: .pw6: Password protection word 6 */
        0x00000000, /**< \brief 0x11C: .pw7: Password protection word 7 (msw) */
    },
    {
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x120: Reserved (0x120 - 0x12F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x130: Reserved (0x130 - 0x13F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x140: Reserved (0x140 - 0x14F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x150: Reserved (0x150 - 0x15F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x160: Reserved (0x160 - 0x16F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x170: Reserved (0x170 - 0x17F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x180: Reserved (0x180 - 0x18F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x190: Reserved (0x190 - 0x19F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1A0: Reserved (0x1A0 - 0x1AF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1B0: Reserved (0x1B0 - 0x1BF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1C0: Reserved (0x1C0 - 0x1CF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1D0: Reserved (0x1D0 - 0x1DF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000         /**< \brief 0x1E0: Reserved (0x1E0 - 0x1EF) */
    },
    0x43211234      /**< \brief 0x1F0: .confirmation: 32-bit CODE, (always same)*/
};

#if defined(__GNUC__)
#pragma section
#pragma section ".bmhd_2_copy" a
#endif
#if defined(__TASKING__)
#pragma section farrom "bmhd_2_copy"
#endif
#if defined(__DCC__)
#pragma section CONST ".bmhd_2_copy" far-absolute R
#endif
#if defined(__ghs__)
#pragma ghs section rodata= ".bmhd_2_copy"
#endif
const Ifx_Ssw_Bmhd bmhd_2_copy=
{
    0x00FE,         /**< \brief 0x000: .bmi: Boot Mode Index (BMI)*/
    0xB359,         /**< \brief 0x002: .bmhdid: Boot Mode Header ID (CODE) = B359H*/
    0xA0000000,     /**< \brief 0x004: .stad: User Code start address*/
    0x31795570,     /**< \brief 0x008: .crc: Check Result for the BMI Header (offset 000H - 007H)*/
    0xCE86AA8F,     /**< \brief 0x00C: .crcInv: Inverted Check Result for the BMI Header (offset 000H - 007H)*/
    {
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x010: Reserved (0x010 - 0x01F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x020: Reserved (0x020 - 0x02F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x030: Reserved (0x030 - 0x03F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x040: Reserved (0x040 - 0x04F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x050: Reserved (0x050 - 0x05F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x060: Reserved (0x060 - 0x06F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x070: Reserved (0x070 - 0x07F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x080: Reserved (0x080 - 0x08F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x090: Reserved (0x090 - 0x09F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0A0: Reserved (0x0A0 - 0x0AF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0B0: Reserved (0x0B0 - 0x0BF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0C0: Reserved (0x0C0 - 0x0CF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0D0: Reserved (0x0D0 - 0x0DF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0E0: Reserved (0x0E0 - 0x0EF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000         /**< \brief 0x0F0: Reserved (0x0F0 - 0x0FF) */
    },
    {
        0x00000000, /**< \brief 0x100: .pw0: Password protection word 0 (lsw) */
        0x00000000, /**< \brief 0x104: .pw1: Password protection word 1 */
        0x00000000, /**< \brief 0x108: .pw2: Password protection word 2 */
        0x00000000, /**< \brief 0x10C: .pw3: Password protection word 3 */
        0x00000000, /**< \brief 0x110: .pw4: Password protection word 4 */
        0x00000000, /**< \brief 0x114: .pw5: Password protection word 5 */
        0x00000000, /**< \brief 0x118: .pw6: Password protection word 6 */
        0x00000000, /**< \brief 0x11C: .pw7: Password protection word 7 (msw) */
    },
    {
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x120: Reserved (0x120 - 0x12F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x130: Reserved (0x120 - 0x13F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x140: Reserved (0x120 - 0x14F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x150: Reserved (0x120 - 0x15F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x160: Reserved (0x120 - 0x16F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x170: Reserved (0x120 - 0x17F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x180: Reserved (0x120 - 0x18F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x190: Reserved (0x120 - 0x19F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1A0: Reserved (0x120 - 0x1AF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1B0: Reserved (0x120 - 0x1BF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1C0: Reserved (0x120 - 0x1CF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1D0: Reserved (0x120 - 0x1DF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1E0: Reserved (0x120 - 0x1EF) */
    },
    0x43211234      /**< \brief 0x1F0: .confirmation: 32-bit CODE, (always same)*/
};

/*Infineon confirm that, if all BHMD are erased by mistake, the chip will be dead
 * Suggestion from Infineon is that do not use all the BMHD in code
 * Right now we're using:
 * BHMD_0_ORIG => ABMHD
 * BHMD_0_COPY => UPDATE_FW
 * BHMD_1_ORIG/COPY => PROD F/W
 * BHMD_2_ORIG/COPY => PROD F/W
 * BHMD_3_ORIG/COPY: NOT USED
 */
#ifdef BMHD_3_ENABLE

#if defined(__GNUC__)
#pragma section
#pragma section ".bmhd_3_orig" a
#endif
#if defined(__TASKING__)
#pragma section farrom "bmhd_3_orig"
#endif
#if defined(__DCC__)
#pragma section CONST ".bmhd_3_orig" far-absolute R
#endif
#if defined(__ghs__)
#pragma ghs section rodata= ".bmhd_3_orig"
#endif
const Ifx_Ssw_Bmhd bmhd_3_orig=
{
    0x00FE,         /**< \brief 0x000: .bmi: Boot Mode Index (BMI)*/
    0xB359,         /**< \brief 0x002: .bmhdid: Boot Mode Header ID (CODE) = B359H*/
    0xA0000000,     /**< \brief 0x004: .stad: User Code start address*/
    0x31795570,     /**< \brief 0x008: .crc: Check Result for the BMI Header (offset 000H - 007H)*/
    0xCE86AA8F,     /**< \brief 0x00C: .crcInv: Inverted Check Result for the BMI Header (offset 000H - 007H)*/
    {
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x010: Reserved (0x010 - 0x01F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x020: Reserved (0x020 - 0x02F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x030: Reserved (0x030 - 0x03F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x040: Reserved (0x040 - 0x04F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x050: Reserved (0x050 - 0x05F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x060: Reserved (0x060 - 0x06F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x070: Reserved (0x070 - 0x07F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x080: Reserved (0x080 - 0x08F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x090: Reserved (0x090 - 0x09F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0A0: Reserved (0x0A0 - 0x0AF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0B0: Reserved (0x0B0 - 0x0BF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0C0: Reserved (0x0C0 - 0x0CF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0D0: Reserved (0x0D0 - 0x0DF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0E0: Reserved (0x0E0 - 0x0EF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000         /**< \brief 0x0F0: Reserved (0x0F0 - 0x0FF) */
    },
    {
        0x00000000, /**< \brief 0x100: .pw0: Password protection word 0 (lsw) */
        0x00000000, /**< \brief 0x104: .pw1: Password protection word 1 */
        0x00000000, /**< \brief 0x108: .pw2: Password protection word 2 */
        0x00000000, /**< \brief 0x10C: .pw3: Password protection word 3 */
        0x00000000, /**< \brief 0x110: .pw4: Password protection word 4 */
        0x00000000, /**< \brief 0x114: .pw5: Password protection word 5 */
        0x00000000, /**< \brief 0x118: .pw6: Password protection word 6 */
        0x00000000, /**< \brief 0x11C: .pw7: Password protection word 7 (msw) */
    },
    {
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x120: Reserved (0x120 - 0x12F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x130: Reserved (0x130 - 0x13F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x140: Reserved (0x140 - 0x14F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x150: Reserved (0x150 - 0x15F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x160: Reserved (0x160 - 0x16F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x170: Reserved (0x170 - 0x17F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x180: Reserved (0x180 - 0x18F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x190: Reserved (0x190 - 0x19F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1A0: Reserved (0x1A0 - 0x1AF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1B0: Reserved (0x1B0 - 0x1BF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1C0: Reserved (0x1C0 - 0x1CF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1D0: Reserved (0x1D0 - 0x1DF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000         /**< \brief 0x1E0: Reserved (0x1E0 - 0x1EF) */
    },
    0x43211234      /**< \brief 0x1F0: .confirmation: 32-bit CODE, (always same)*/
};

#if defined(__GNUC__)
#pragma section
#pragma section ".bmhd_3_copy" a
#endif
#if defined(__TASKING__)
#pragma section farrom "bmhd_3_copy"
#endif
#if defined(__DCC__)
#pragma section CONST ".bmhd_3_copy" far-absolute R
#endif
#if defined(__ghs__)
#pragma ghs section rodata= ".bmhd_3_copy"
#endif
const Ifx_Ssw_Bmhd bmhd_3_copy=
{
    0x00FE,         /**< \brief 0x000: .bmi: Boot Mode Index (BMI)*/
    0xB359,         /**< \brief 0x002: .bmhdid: Boot Mode Header ID (CODE) = B359H*/
    0xA0000000,     /**< \brief 0x004: .stad: User Code start address*/
    0x31795570,     /**< \brief 0x008: .crc: Check Result for the BMI Header (offset 000H - 007H)*/
    0xCE86AA8F,     /**< \brief 0x00C: .crcInv: Inverted Check Result for the BMI Header (offset 000H - 007H)*/
    {
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x010: Reserved (0x010 - 0x01F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x020: Reserved (0x020 - 0x02F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x030: Reserved (0x030 - 0x03F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x040: Reserved (0x040 - 0x04F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x050: Reserved (0x050 - 0x05F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x060: Reserved (0x060 - 0x06F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x070: Reserved (0x070 - 0x07F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x080: Reserved (0x080 - 0x08F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x090: Reserved (0x090 - 0x09F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0A0: Reserved (0x0A0 - 0x0AF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0B0: Reserved (0x0B0 - 0x0BF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0C0: Reserved (0x0C0 - 0x0CF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0D0: Reserved (0x0D0 - 0x0DF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x0E0: Reserved (0x0E0 - 0x0EF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000         /**< \brief 0x0F0: Reserved (0x0F0 - 0x0FF) */
    },
    {
        0x00000000, /**< \brief 0x100: .pw0: Password protection word 0 (lsw) */
        0x00000000, /**< \brief 0x104: .pw1: Password protection word 1 */
        0x00000000, /**< \brief 0x108: .pw2: Password protection word 2 */
        0x00000000, /**< \brief 0x10C: .pw3: Password protection word 3 */
        0x00000000, /**< \brief 0x110: .pw4: Password protection word 4 */
        0x00000000, /**< \brief 0x114: .pw5: Password protection word 5 */
        0x00000000, /**< \brief 0x118: .pw6: Password protection word 6 */
        0x00000000, /**< \brief 0x11C: .pw7: Password protection word 7 (msw) */
    },
    {
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x120: Reserved (0x120 - 0x12F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x130: Reserved (0x120 - 0x13F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x140: Reserved (0x120 - 0x14F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x150: Reserved (0x120 - 0x15F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x160: Reserved (0x120 - 0x16F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x170: Reserved (0x120 - 0x17F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x180: Reserved (0x120 - 0x18F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x190: Reserved (0x120 - 0x19F) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1A0: Reserved (0x120 - 0x1AF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1B0: Reserved (0x120 - 0x1BF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1C0: Reserved (0x120 - 0x1CF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1D0: Reserved (0x120 - 0x1DF) */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,        /**< \brief 0x1E0: Reserved (0x120 - 0x1EF) */
    },
    0x43211234      /**< \brief 0x1F0: .confirmation: 32-bit CODE, (always same)*/
};

#endif //#ifdef BMHD_3_ENABLE

#if defined(__GNUC__)
#pragma section
#endif
