/**
 * \file IfxSpulckstp_regdef.h
 * \brief
 * \copyright Copyright (c) 2018 Infineon Technologies AG. All rights reserved.
 *
 *
 * Version: TC39XB_UM_V1.0.0.R0
 * Specification: TC3xx User Manual V1.0.0
 * MAY BE CHANGED BY USER [yes/no]: No
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
 * \defgroup IfxSfr_Spulckstp_Registers Spulckstp Registers
 * \ingroup IfxSfr
 * 
 * \defgroup IfxSfr_Spulckstp_Registers_Bitfields Bitfields
 * \ingroup IfxSfr_Spulckstp_Registers
 * 
 * \defgroup IfxSfr_Spulckstp_Registers_union Register unions
 * \ingroup IfxSfr_Spulckstp_Registers
 * 
 * \defgroup IfxSfr_Spulckstp_Registers_struct Memory map
 * \ingroup IfxSfr_Spulckstp_Registers
 */
#ifndef IFXSPULCKSTP_REGDEF_H
#define IFXSPULCKSTP_REGDEF_H 1
/******************************************************************************/
#include "Ifx_TypesReg.h"
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/


/** \addtogroup IfxSfr_Spulckstp_Registers_Bitfields
 * \{  */
/** \brief Access Enable Register 0 */
typedef struct _Ifx_SPULCKSTP_ACCEN0_Bits
{
    Ifx_UReg_32Bit EN0:1;             /**< \brief [0:0] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN1:1;             /**< \brief [1:1] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN2:1;             /**< \brief [2:2] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN3:1;             /**< \brief [3:3] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN4:1;             /**< \brief [4:4] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN5:1;             /**< \brief [5:5] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN6:1;             /**< \brief [6:6] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN7:1;             /**< \brief [7:7] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN8:1;             /**< \brief [8:8] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN9:1;             /**< \brief [9:9] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN10:1;            /**< \brief [10:10] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN11:1;            /**< \brief [11:11] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN12:1;            /**< \brief [12:12] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN13:1;            /**< \brief [13:13] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN14:1;            /**< \brief [14:14] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN15:1;            /**< \brief [15:15] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN16:1;            /**< \brief [16:16] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN17:1;            /**< \brief [17:17] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN18:1;            /**< \brief [18:18] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN19:1;            /**< \brief [19:19] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN20:1;            /**< \brief [20:20] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN21:1;            /**< \brief [21:21] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN22:1;            /**< \brief [22:22] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN23:1;            /**< \brief [23:23] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN24:1;            /**< \brief [24:24] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN25:1;            /**< \brief [25:25] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN26:1;            /**< \brief [26:26] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN27:1;            /**< \brief [27:27] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN28:1;            /**< \brief [28:28] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN29:1;            /**< \brief [29:29] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN30:1;            /**< \brief [30:30] Access Enable for Master TAG ID x (rw) */
    Ifx_UReg_32Bit EN31:1;            /**< \brief [31:31] Access Enable for Master TAG ID x (rw) */
} Ifx_SPULCKSTP_ACCEN0_Bits;

/** \brief Access Enable Register 1 */
typedef struct _Ifx_SPULCKSTP_ACCEN1_Bits
{
    Ifx_UReg_32Bit reserved_0:32;     /**< \brief [31:0] \internal Reserved */
} Ifx_SPULCKSTP_ACCEN1_Bits;

/** \brief Clock Control */
typedef struct _Ifx_SPULCKSTP_CLC_Bits
{
    Ifx_UReg_32Bit DISR:1;            /**< \brief [0:0] Disable Request (rw) */
    Ifx_UReg_32Bit DISS:1;            /**< \brief [1:1] Disable Status (rh) */
    Ifx_UReg_32Bit RES:1;             /**< \brief [2:2] Reserved (r) */
    Ifx_UReg_32Bit EDIS:1;            /**< \brief [3:3] Sleep Mode Enable Control (rw) */
    Ifx_UReg_32Bit reserved_4:28;     /**< \brief [31:4] \internal Reserved */
} Ifx_SPULCKSTP_CLC_Bits;

/** \brief SPU Lockstep Control */
typedef struct _Ifx_SPULCKSTP_CTRL_Bits
{
    Ifx_UReg_32Bit LSEN0:2;           /**< \brief [1:0] Lockstep Comparator x Enable (rw) */
    Ifx_UReg_32Bit LSEN1:2;           /**< \brief [3:2] Lockstep Comparator x Enable (rw) */
    Ifx_UReg_32Bit LSEN2:2;           /**< \brief [5:4] Lockstep Comparator x Enable (rw) */
    Ifx_UReg_32Bit LSEN3:2;           /**< \brief [7:6] Lockstep Comparator x Enable (rw) */
    Ifx_UReg_32Bit reserved_8:8;      /**< \brief [15:8] \internal Reserved */
    Ifx_UReg_32Bit ERRDIS:2;          /**< \brief [17:16] Error Disable (rw) */
    Ifx_UReg_32Bit MS:2;              /**< \brief [19:18] Mirror SPU0 (rw) */
    Ifx_UReg_32Bit reserved_20:12;    /**< \brief [31:20] \internal Reserved */
} Ifx_SPULCKSTP_CTRL_Bits;

/** \brief Error Clear */
typedef struct _Ifx_SPULCKSTP_ERRCLR_Bits
{
    Ifx_UReg_32Bit CLR0:2;            /**< \brief [1:0] Clear Lockstep Comparator x Fail Flag (rwh) */
    Ifx_UReg_32Bit CLR1:2;            /**< \brief [3:2] Clear Lockstep Comparator x Fail Flag (rwh) */
    Ifx_UReg_32Bit CLR2:2;            /**< \brief [5:4] Clear Lockstep Comparator x Fail Flag (rwh) */
    Ifx_UReg_32Bit CLR3:2;            /**< \brief [7:6] Clear Lockstep Comparator x Fail Flag (rwh) */
    Ifx_UReg_32Bit reserved_8:8;      /**< \brief [15:8] \internal Reserved */
    Ifx_UReg_32Bit RCLR0:2;           /**< \brief [17:16] Clear Lockstep Comparator x Redundancy Fail Flag (rwh) */
    Ifx_UReg_32Bit RCLR1:2;           /**< \brief [19:18] Clear Lockstep Comparator x Redundancy Fail Flag (rwh) */
    Ifx_UReg_32Bit RCLR2:2;           /**< \brief [21:20] Clear Lockstep Comparator x Redundancy Fail Flag (rwh) */
    Ifx_UReg_32Bit RCLR3:2;           /**< \brief [23:22] Clear Lockstep Comparator x Redundancy Fail Flag (rwh) */
    Ifx_UReg_32Bit reserved_24:8;     /**< \brief [31:24] \internal Reserved */
} Ifx_SPULCKSTP_ERRCLR_Bits;

/** \brief Error Monitoring Register */
typedef struct _Ifx_SPULCKSTP_ERROR_Bits
{
    Ifx_UReg_32Bit LCFAIL0:2;         /**< \brief [1:0] Lockstep Comparator x Fail (rh) */
    Ifx_UReg_32Bit LCFAIL1:2;         /**< \brief [3:2] Lockstep Comparator x Fail (rh) */
    Ifx_UReg_32Bit LCFAIL2:2;         /**< \brief [5:4] Lockstep Comparator x Fail (rh) */
    Ifx_UReg_32Bit LCFAIL3:2;         /**< \brief [7:6] Lockstep Comparator x Fail (rh) */
    Ifx_UReg_32Bit reserved_8:8;      /**< \brief [15:8] \internal Reserved */
    Ifx_UReg_32Bit LCRFAIL0:2;        /**< \brief [17:16] Lockstep Comparator x Redundancy Fail (rh) */
    Ifx_UReg_32Bit LCRFAIL1:2;        /**< \brief [19:18] Lockstep Comparator x Redundancy Fail (rh) */
    Ifx_UReg_32Bit LCRFAIL2:2;        /**< \brief [21:20] Lockstep Comparator x Redundancy Fail (rh) */
    Ifx_UReg_32Bit LCRFAIL3:2;        /**< \brief [23:22] Lockstep Comparator x Redundancy Fail (rh) */
    Ifx_UReg_32Bit reserved_24:8;     /**< \brief [31:24] \internal Reserved */
} Ifx_SPULCKSTP_ERROR_Bits;

/** \brief Module Identification Register */
typedef struct _Ifx_SPULCKSTP_MODID_Bits
{
    Ifx_UReg_32Bit MOD_REV:8;         /**< \brief [7:0] Module Revision Number (r) */
    Ifx_UReg_32Bit MOD_TYPE:8;        /**< \brief [15:8] Module Type (r) */
    Ifx_UReg_32Bit MOD_NUMBER:16;     /**< \brief [31:16] Module Number Value (r) */
} Ifx_SPULCKSTP_MODID_Bits;

/** \brief SPU Control */
typedef struct _Ifx_SPULCKSTP_SPUCTRL_Bits
{
    Ifx_UReg_32Bit TRIGGER:1;         /**< \brief [0:0] SPU Trigger (rwh) */
    Ifx_UReg_32Bit reserved_1:31;     /**< \brief [31:1] \internal Reserved */
} Ifx_SPULCKSTP_SPUCTRL_Bits;

/** \brief Alarm Test Register */
typedef struct _Ifx_SPULCKSTP_TEST_Bits
{
    Ifx_UReg_32Bit LSTST0:2;          /**< \brief [1:0] Test Lockstep Comparator x Alarm (rwh) */
    Ifx_UReg_32Bit LSTST1:2;          /**< \brief [3:2] Test Lockstep Comparator x Alarm (rwh) */
    Ifx_UReg_32Bit LSTST2:2;          /**< \brief [5:4] Test Lockstep Comparator x Alarm (rwh) */
    Ifx_UReg_32Bit LSTST3:2;          /**< \brief [7:6] Test Lockstep Comparator x Alarm (rwh) */
    Ifx_UReg_32Bit reserved_8:24;     /**< \brief [31:8] \internal Reserved */
} Ifx_SPULCKSTP_TEST_Bits;

/** \}  */
/******************************************************************************/
/******************************************************************************/
/** \addtogroup IfxSfr_spulckstp_Registers_union
 * \{   */
/** \brief Access Enable Register 0   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_SPULCKSTP_ACCEN0_Bits B;      /**< \brief Bitfield access */
} Ifx_SPULCKSTP_ACCEN0;

/** \brief Access Enable Register 1   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_SPULCKSTP_ACCEN1_Bits B;      /**< \brief Bitfield access */
} Ifx_SPULCKSTP_ACCEN1;

/** \brief Clock Control   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_SPULCKSTP_CLC_Bits B;         /**< \brief Bitfield access */
} Ifx_SPULCKSTP_CLC;

/** \brief SPU Lockstep Control   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_SPULCKSTP_CTRL_Bits B;        /**< \brief Bitfield access */
} Ifx_SPULCKSTP_CTRL;

/** \brief Error Clear   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_SPULCKSTP_ERRCLR_Bits B;      /**< \brief Bitfield access */
} Ifx_SPULCKSTP_ERRCLR;

/** \brief Error Monitoring Register   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_SPULCKSTP_ERROR_Bits B;       /**< \brief Bitfield access */
} Ifx_SPULCKSTP_ERROR;

/** \brief Module Identification Register   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_SPULCKSTP_MODID_Bits B;       /**< \brief Bitfield access */
} Ifx_SPULCKSTP_MODID;

/** \brief SPU Control   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_SPULCKSTP_SPUCTRL_Bits B;     /**< \brief Bitfield access */
} Ifx_SPULCKSTP_SPUCTRL;

/** \brief Alarm Test Register   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_SPULCKSTP_TEST_Bits B;        /**< \brief Bitfield access */
} Ifx_SPULCKSTP_TEST;

/** \}  */

/******************************************************************************/
/** \addtogroup IfxSfr_Spulckstp_Registers_struct
 * \{  */
/******************************************************************************/
/** \name Object L0
 * \{  */

/** \brief SPULCKSTP object */
typedef volatile struct _Ifx_SPULCKSTP
{
       Ifx_SPULCKSTP_CLC                   CLC;                    /**< \brief 0, Clock Control*/
       Ifx_SPULCKSTP_MODID                 MODID;                  /**< \brief 4, Module Identification Register*/
       Ifx_UReg_8Bit                       reserved_8[8];          /**< \brief 8, \internal Reserved */
       Ifx_SPULCKSTP_CTRL                  CTRL;                   /**< \brief 10, SPU Lockstep Control*/
       Ifx_UReg_8Bit                       reserved_14[4];         /**< \brief 14, \internal Reserved */
       Ifx_SPULCKSTP_ERROR                 ERROR;                  /**< \brief 18, Error Monitoring Register*/
       Ifx_SPULCKSTP_ERRCLR                ERRCLR;                 /**< \brief 1C, Error Clear*/
       Ifx_SPULCKSTP_TEST                  TEST;                   /**< \brief 20, Alarm Test Register*/
       Ifx_SPULCKSTP_SPUCTRL               SPUCTRL;                /**< \brief 24, SPU Control*/
       Ifx_UReg_8Bit                       reserved_28[188];       /**< \brief 28, \internal Reserved */
       Ifx_SPULCKSTP_ACCEN0                ACCEN0;                 /**< \brief E4, Access Enable Register 0*/
       Ifx_SPULCKSTP_ACCEN1                ACCEN1;                 /**< \brief E8, Access Enable Register 1*/
       Ifx_UReg_8Bit                       reserved_EC[20];        /**< \brief EC, \internal Reserved */
} Ifx_SPULCKSTP;

/** \}  */
/******************************************************************************/
/** \}  */


/******************************************************************************/

/******************************************************************************/

#endif /* IFXSPULCKSTP_REGDEF_H */
