/**
 * \file IfxStm_regdef.h
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
 * \defgroup IfxSfr_Stm_Registers Stm Registers
 * \ingroup IfxSfr
 * 
 * \defgroup IfxSfr_Stm_Registers_Bitfields Bitfields
 * \ingroup IfxSfr_Stm_Registers
 * 
 * \defgroup IfxSfr_Stm_Registers_union Register unions
 * \ingroup IfxSfr_Stm_Registers
 * 
 * \defgroup IfxSfr_Stm_Registers_struct Memory map
 * \ingroup IfxSfr_Stm_Registers
 */
#ifndef IFXSTM_REGDEF_H
#define IFXSTM_REGDEF_H 1
/******************************************************************************/
#include "Ifx_TypesReg.h"
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/


/** \addtogroup IfxSfr_Stm_Registers_Bitfields
 * \{  */
/** \brief Access Enable Register 0 */
typedef struct _Ifx_STM_ACCEN0_Bits
{
    Ifx_UReg_32Bit EN0:1;             /**< \brief [0:0] Access Enable for Master TAG ID 0 - EN0 (rw) */
    Ifx_UReg_32Bit EN1:1;             /**< \brief [1:1] Access Enable for Master TAG ID 1 - EN1 (rw) */
    Ifx_UReg_32Bit EN2:1;             /**< \brief [2:2] Access Enable for Master TAG ID 2 - EN2 (rw) */
    Ifx_UReg_32Bit EN3:1;             /**< \brief [3:3] Access Enable for Master TAG ID 3 - EN3 (rw) */
    Ifx_UReg_32Bit EN4:1;             /**< \brief [4:4] Access Enable for Master TAG ID 4 - EN4 (rw) */
    Ifx_UReg_32Bit EN5:1;             /**< \brief [5:5] Access Enable for Master TAG ID 5 - EN5 (rw) */
    Ifx_UReg_32Bit EN6:1;             /**< \brief [6:6] Access Enable for Master TAG ID 6 - EN6 (rw) */
    Ifx_UReg_32Bit EN7:1;             /**< \brief [7:7] Access Enable for Master TAG ID 7 - EN7 (rw) */
    Ifx_UReg_32Bit EN8:1;             /**< \brief [8:8] Access Enable for Master TAG ID 8 - EN8 (rw) */
    Ifx_UReg_32Bit EN9:1;             /**< \brief [9:9] Access Enable for Master TAG ID 9 - EN9 (rw) */
    Ifx_UReg_32Bit EN10:1;            /**< \brief [10:10] Access Enable for Master TAG ID 10 - EN10 (rw) */
    Ifx_UReg_32Bit EN11:1;            /**< \brief [11:11] Access Enable for Master TAG ID 11 - EN11 (rw) */
    Ifx_UReg_32Bit EN12:1;            /**< \brief [12:12] Access Enable for Master TAG ID 12 - EN12 (rw) */
    Ifx_UReg_32Bit EN13:1;            /**< \brief [13:13] Access Enable for Master TAG ID 13 - EN13 (rw) */
    Ifx_UReg_32Bit EN14:1;            /**< \brief [14:14] Access Enable for Master TAG ID 14 - EN14 (rw) */
    Ifx_UReg_32Bit EN15:1;            /**< \brief [15:15] Access Enable for Master TAG ID 15 - EN15 (rw) */
    Ifx_UReg_32Bit EN16:1;            /**< \brief [16:16] Access Enable for Master TAG ID 16 - EN16 (rw) */
    Ifx_UReg_32Bit EN17:1;            /**< \brief [17:17] Access Enable for Master TAG ID 17 - EN17 (rw) */
    Ifx_UReg_32Bit EN18:1;            /**< \brief [18:18] Access Enable for Master TAG ID 18 - EN18 (rw) */
    Ifx_UReg_32Bit EN19:1;            /**< \brief [19:19] Access Enable for Master TAG ID 19 - EN19 (rw) */
    Ifx_UReg_32Bit EN20:1;            /**< \brief [20:20] Access Enable for Master TAG ID 20 - EN20 (rw) */
    Ifx_UReg_32Bit EN21:1;            /**< \brief [21:21] Access Enable for Master TAG ID 21 - EN21 (rw) */
    Ifx_UReg_32Bit EN22:1;            /**< \brief [22:22] Access Enable for Master TAG ID 22 - EN22 (rw) */
    Ifx_UReg_32Bit EN23:1;            /**< \brief [23:23] Access Enable for Master TAG ID 23 - EN23 (rw) */
    Ifx_UReg_32Bit EN24:1;            /**< \brief [24:24] Access Enable for Master TAG ID 24 - EN24 (rw) */
    Ifx_UReg_32Bit EN25:1;            /**< \brief [25:25] Access Enable for Master TAG ID 25 - EN25 (rw) */
    Ifx_UReg_32Bit EN26:1;            /**< \brief [26:26] Access Enable for Master TAG ID 26 - EN26 (rw) */
    Ifx_UReg_32Bit EN27:1;            /**< \brief [27:27] Access Enable for Master TAG ID 27 - EN27 (rw) */
    Ifx_UReg_32Bit EN28:1;            /**< \brief [28:28] Access Enable for Master TAG ID 28 - EN28 (rw) */
    Ifx_UReg_32Bit EN29:1;            /**< \brief [29:29] Access Enable for Master TAG ID 29 - EN29 (rw) */
    Ifx_UReg_32Bit EN30:1;            /**< \brief [30:30] Access Enable for Master TAG ID 30 - EN30 (rw) */
    Ifx_UReg_32Bit EN31:1;            /**< \brief [31:31] Access Enable for Master TAG ID 31 - EN31 (rw) */
} Ifx_STM_ACCEN0_Bits;

/** \brief Access Enable Register 1 */
typedef struct _Ifx_STM_ACCEN1_Bits
{
    Ifx_UReg_32Bit reserved_0:32;     /**< \brief [31:0] \internal Reserved */
} Ifx_STM_ACCEN1_Bits;

/** \brief Timer Capture Register */
typedef struct _Ifx_STM_CAP_Bits
{
    Ifx_UReg_32Bit STMCAP_63_32:32;    /**< \brief [31:0] Captured System Timer Bits [63:32] - STMCAP[63:32] (rh) */
} Ifx_STM_CAP_Bits;

/** \brief Timer Capture Register Second View */
typedef struct _Ifx_STM_CAPSV_Bits
{
    Ifx_UReg_32Bit STMCAP_63_32:32;    /**< \brief [31:0] Captured System Timer Bits [63:32] - STMCAP[63:32] (rh) */
} Ifx_STM_CAPSV_Bits;

/** \brief Clock Control Register */
typedef struct _Ifx_STM_CLC_Bits
{
    Ifx_UReg_32Bit DISR:1;            /**< \brief [0:0] Module Disable Request Bit - DISR (rw) */
    Ifx_UReg_32Bit DISS:1;            /**< \brief [1:1] Module Disable Status Bit - DISS (r) */
    Ifx_UReg_32Bit reserved_2:1;      /**< \brief [2:2] \internal Reserved */
    Ifx_UReg_32Bit EDIS:1;            /**< \brief [3:3] Sleep Mode Enable Control - EDIS (rw) */
    Ifx_UReg_32Bit reserved_4:28;     /**< \brief [31:4] \internal Reserved */
} Ifx_STM_CLC_Bits;

/** \brief Compare Match Control Register */
typedef struct _Ifx_STM_CMCON_Bits
{
    Ifx_UReg_32Bit MSIZE0:5;          /**< \brief [4:0] Compare Register Size for CMP0 - MSIZE0 (rw) */
    Ifx_UReg_32Bit reserved_5:3;      /**< \brief [7:5] \internal Reserved */
    Ifx_UReg_32Bit MSTART0:5;         /**< \brief [12:8] Start Bit Location for CMP0 - MSTART0 (rw) */
    Ifx_UReg_32Bit reserved_13:3;     /**< \brief [15:13] \internal Reserved */
    Ifx_UReg_32Bit MSIZE1:5;          /**< \brief [20:16] Compare Register Size for CMP1 - MSIZE1 (rw) */
    Ifx_UReg_32Bit reserved_21:3;     /**< \brief [23:21] \internal Reserved */
    Ifx_UReg_32Bit MSTART1:5;         /**< \brief [28:24] Start Bit Location for CMP1 - MSTART1 (rw) */
    Ifx_UReg_32Bit reserved_29:3;     /**< \brief [31:29] \internal Reserved */
} Ifx_STM_CMCON_Bits;

/** \brief Compare Register ${x} */
typedef struct _Ifx_STM_CMP_Bits
{
    Ifx_UReg_32Bit CMPVAL:32;         /**< \brief [31:0] Compare Value of Compare Register x - CMPVAL (rw) */
} Ifx_STM_CMP_Bits;

/** \brief Interrupt Control Register */
typedef struct _Ifx_STM_ICR_Bits
{
    Ifx_UReg_32Bit CMP0EN:1;          /**< \brief [0:0] Compare Register CMP0 Interrupt Enable Control - CMP0EN (rw) */
    Ifx_UReg_32Bit CMP0IR:1;          /**< \brief [1:1] Compare Register CMP0 Interrupt Request Flag - CMP0IR (rh) */
    Ifx_UReg_32Bit CMP0OS:1;          /**< \brief [2:2] Compare Register CMP0 Interrupt Output Selection - CMP0OS (rw) */
    Ifx_UReg_32Bit reserved_3:1;      /**< \brief [3:3] \internal Reserved */
    Ifx_UReg_32Bit CMP1EN:1;          /**< \brief [4:4] Compare Register CMP1 Interrupt Enable Control - CMP1EN (rw) */
    Ifx_UReg_32Bit CMP1IR:1;          /**< \brief [5:5] Compare Register CMP1 Interrupt Request Flag - CMP1IR (rh) */
    Ifx_UReg_32Bit CMP1OS:1;          /**< \brief [6:6] Compare Register CMP1 Interrupt Output Selection - CMP1OS (rw) */
    Ifx_UReg_32Bit reserved_7:25;     /**< \brief [31:7] \internal Reserved */
} Ifx_STM_ICR_Bits;

/** \brief Module Identification Register */
typedef struct _Ifx_STM_ID_Bits
{
    Ifx_UReg_32Bit MODREV:8;          /**< \brief [7:0] Module Revision Number - MODREV (r) */
    Ifx_UReg_32Bit MODTYPE:8;         /**< \brief [15:8] Module Type - MODTYPE (r) */
    Ifx_UReg_32Bit MODNUM:16;         /**< \brief [31:16] Module Number Value - MODNUM (r) */
} Ifx_STM_ID_Bits;

/** \brief Interrupt Set/Clear Register */
typedef struct _Ifx_STM_ISCR_Bits
{
    Ifx_UReg_32Bit CMP0IRR:1;         /**< \brief [0:0] Reset Compare Register CMP0 Interrupt Flag - CMP0IRR (w) */
    Ifx_UReg_32Bit CMP0IRS:1;         /**< \brief [1:1] Set Compare Register CMP0 Interrupt Flag - CMP0IRS (w) */
    Ifx_UReg_32Bit CMP1IRR:1;         /**< \brief [2:2] Reset Compare Register CMP1 Interrupt Flag - CMP1IRR (w) */
    Ifx_UReg_32Bit CMP1IRS:1;         /**< \brief [3:3] Set Compare Register CMP1 Interrupt Flag - CMP1IRS (w) */
    Ifx_UReg_32Bit reserved_4:28;     /**< \brief [31:4] \internal Reserved */
} Ifx_STM_ISCR_Bits;

/** \brief Kernel Reset Register 0 */
typedef struct _Ifx_STM_KRST0_Bits
{
    Ifx_UReg_32Bit RST:1;             /**< \brief [0:0] Kernel Reset - RST (rwh) */
    Ifx_UReg_32Bit RSTSTAT:1;         /**< \brief [1:1] Kernel Reset Status - RSTSTAT (rh) */
    Ifx_UReg_32Bit reserved_2:30;     /**< \brief [31:2] \internal Reserved */
} Ifx_STM_KRST0_Bits;

/** \brief Kernel Reset Register 1 */
typedef struct _Ifx_STM_KRST1_Bits
{
    Ifx_UReg_32Bit RST:1;             /**< \brief [0:0] Kernel Reset - RST (rwh) */
    Ifx_UReg_32Bit reserved_1:31;     /**< \brief [31:1] \internal Reserved */
} Ifx_STM_KRST1_Bits;

/** \brief Kernel Reset Status Clear Register */
typedef struct _Ifx_STM_KRSTCLR_Bits
{
    Ifx_UReg_32Bit CLR:1;             /**< \brief [0:0] Kernel Reset Status Clear - CLR (w) */
    Ifx_UReg_32Bit reserved_1:31;     /**< \brief [31:1] \internal Reserved */
} Ifx_STM_KRSTCLR_Bits;

/** \brief OCDS Control and Status Register */
typedef struct _Ifx_STM_OCS_Bits
{
    Ifx_UReg_32Bit reserved_0:3;      /**< \brief [2:0] \internal Reserved */
    Ifx_UReg_32Bit reserved_3:21;     /**< \brief [23:3] \internal Reserved */
    Ifx_UReg_32Bit SUS:4;             /**< \brief [27:24] OCDS Suspend Control - SUS (rw) */
    Ifx_UReg_32Bit SUS_P:1;           /**< \brief [28:28] SUS Write Protection - SUS_P (w) */
    Ifx_UReg_32Bit SUSSTA:1;          /**< \brief [29:29] Suspend State - SUSSTA (rh) */
    Ifx_UReg_32Bit reserved_30:2;     /**< \brief [31:30] \internal Reserved */
} Ifx_STM_OCS_Bits;

/** \brief Timer Register 0 */
typedef struct _Ifx_STM_TIM0_Bits
{
    Ifx_UReg_32Bit STM_31_0:32;       /**< \brief [31:0] System Timer Bits [31:0] - STM[31:0] (r) */
} Ifx_STM_TIM0_Bits;

/** \brief Timer Register 0 Second View */
typedef struct _Ifx_STM_TIM0SV_Bits
{
    Ifx_UReg_32Bit STM_31_0:32;       /**< \brief [31:0] System Timer Bits [31:0] - STM[31:0] (r) */
} Ifx_STM_TIM0SV_Bits;

/** \brief Timer Register 1 */
typedef struct _Ifx_STM_TIM1_Bits
{
    Ifx_UReg_32Bit STM_35_4:32;       /**< \brief [31:0] System Timer Bits [35:4] - STM[35:4] (r) */
} Ifx_STM_TIM1_Bits;

/** \brief Timer Register 2 */
typedef struct _Ifx_STM_TIM2_Bits
{
    Ifx_UReg_32Bit STM_39_8:32;       /**< \brief [31:0] System Timer Bits [39:8] - STM[39:8] (r) */
} Ifx_STM_TIM2_Bits;

/** \brief Timer Register 3 */
typedef struct _Ifx_STM_TIM3_Bits
{
    Ifx_UReg_32Bit STM_43_12:32;      /**< \brief [31:0] System Timer Bits [43:12] - STM[43:12] (r) */
} Ifx_STM_TIM3_Bits;

/** \brief Timer Register 4 */
typedef struct _Ifx_STM_TIM4_Bits
{
    Ifx_UReg_32Bit STM_47_16:32;      /**< \brief [31:0] System Timer Bits [47:16] - STM[47:16] (r) */
} Ifx_STM_TIM4_Bits;

/** \brief Timer Register 5 */
typedef struct _Ifx_STM_TIM5_Bits
{
    Ifx_UReg_32Bit STM_51_20:32;      /**< \brief [31:0] System Timer Bits [51:20] - STM[51:20] (r) */
} Ifx_STM_TIM5_Bits;

/** \brief Timer Register 6 */
typedef struct _Ifx_STM_TIM6_Bits
{
    Ifx_UReg_32Bit STM_63_32:32;      /**< \brief [31:0] System Timer Bits [63:32] - STM[63:32] (r) */
} Ifx_STM_TIM6_Bits;

/** \}  */
/******************************************************************************/
/******************************************************************************/
/** \addtogroup IfxSfr_stm_Registers_union
 * \{   */
/** \brief Access Enable Register 0   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_STM_ACCEN0_Bits B;            /**< \brief Bitfield access */
} Ifx_STM_ACCEN0;

/** \brief Access Enable Register 1   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_STM_ACCEN1_Bits B;            /**< \brief Bitfield access */
} Ifx_STM_ACCEN1;

/** \brief Timer Capture Register   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_STM_CAP_Bits B;               /**< \brief Bitfield access */
} Ifx_STM_CAP;

/** \brief Timer Capture Register Second View   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_STM_CAPSV_Bits B;             /**< \brief Bitfield access */
} Ifx_STM_CAPSV;

/** \brief Clock Control Register   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_STM_CLC_Bits B;               /**< \brief Bitfield access */
} Ifx_STM_CLC;

/** \brief Compare Match Control Register   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_STM_CMCON_Bits B;             /**< \brief Bitfield access */
} Ifx_STM_CMCON;

/** \brief Compare Register ${x}   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_STM_CMP_Bits B;               /**< \brief Bitfield access */
} Ifx_STM_CMP;

/** \brief Interrupt Control Register   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_STM_ICR_Bits B;               /**< \brief Bitfield access */
} Ifx_STM_ICR;

/** \brief Module Identification Register   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_STM_ID_Bits B;                /**< \brief Bitfield access */
} Ifx_STM_ID;

/** \brief Interrupt Set/Clear Register   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_STM_ISCR_Bits B;              /**< \brief Bitfield access */
} Ifx_STM_ISCR;

/** \brief Kernel Reset Register 0   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_STM_KRST0_Bits B;             /**< \brief Bitfield access */
} Ifx_STM_KRST0;

/** \brief Kernel Reset Register 1   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_STM_KRST1_Bits B;             /**< \brief Bitfield access */
} Ifx_STM_KRST1;

/** \brief Kernel Reset Status Clear Register   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_STM_KRSTCLR_Bits B;           /**< \brief Bitfield access */
} Ifx_STM_KRSTCLR;

/** \brief OCDS Control and Status Register   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_STM_OCS_Bits B;               /**< \brief Bitfield access */
} Ifx_STM_OCS;

/** \brief Timer Register 0   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_STM_TIM0_Bits B;              /**< \brief Bitfield access */
} Ifx_STM_TIM0;

/** \brief Timer Register 0 Second View   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_STM_TIM0SV_Bits B;            /**< \brief Bitfield access */
} Ifx_STM_TIM0SV;

/** \brief Timer Register 1   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_STM_TIM1_Bits B;              /**< \brief Bitfield access */
} Ifx_STM_TIM1;

/** \brief Timer Register 2   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_STM_TIM2_Bits B;              /**< \brief Bitfield access */
} Ifx_STM_TIM2;

/** \brief Timer Register 3   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_STM_TIM3_Bits B;              /**< \brief Bitfield access */
} Ifx_STM_TIM3;

/** \brief Timer Register 4   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_STM_TIM4_Bits B;              /**< \brief Bitfield access */
} Ifx_STM_TIM4;

/** \brief Timer Register 5   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_STM_TIM5_Bits B;              /**< \brief Bitfield access */
} Ifx_STM_TIM5;

/** \brief Timer Register 6   */
typedef union
{
    Ifx_UReg_32Bit U;                 /**< \brief Unsigned access */
    Ifx_SReg_32Bit I;                 /**< \brief Signed access */
    Ifx_STM_TIM6_Bits B;              /**< \brief Bitfield access */
} Ifx_STM_TIM6;

/** \}  */

/******************************************************************************/
/** \addtogroup IfxSfr_Stm_Registers_struct
 * \{  */
/******************************************************************************/
/** \name Object L0
 * \{  */

/** \brief STM object */
typedef volatile struct _Ifx_STM
{
       Ifx_STM_CLC                         CLC;                    /**< \brief 0, Clock Control Register*/
       Ifx_UReg_8Bit                       reserved_4[4];          /**< \brief 4, \internal Reserved */
       Ifx_STM_ID                          ID;                     /**< \brief 8, Module Identification Register*/
       Ifx_UReg_8Bit                       reserved_C[4];          /**< \brief C, \internal Reserved */
       Ifx_STM_TIM0                        TIM0;                   /**< \brief 10, Timer Register 0*/
       Ifx_STM_TIM1                        TIM1;                   /**< \brief 14, Timer Register 1*/
       Ifx_STM_TIM2                        TIM2;                   /**< \brief 18, Timer Register 2*/
       Ifx_STM_TIM3                        TIM3;                   /**< \brief 1C, Timer Register 3*/
       Ifx_STM_TIM4                        TIM4;                   /**< \brief 20, Timer Register 4*/
       Ifx_STM_TIM5                        TIM5;                   /**< \brief 24, Timer Register 5*/
       Ifx_STM_TIM6                        TIM6;                   /**< \brief 28, Timer Register 6*/
       Ifx_STM_CAP                         CAP;                    /**< \brief 2C, Timer Capture Register*/
       Ifx_STM_CMP                         CMP[2];                 /**< \brief 30, Compare Register ${x}*/
       Ifx_STM_CMCON                       CMCON;                  /**< \brief 38, Compare Match Control Register*/
       Ifx_STM_ICR                         ICR;                    /**< \brief 3C, Interrupt Control Register*/
       Ifx_STM_ISCR                        ISCR;                   /**< \brief 40, Interrupt Set/Clear Register*/
       Ifx_UReg_8Bit                       reserved_44[12];        /**< \brief 44, \internal Reserved */
       Ifx_STM_TIM0SV                      TIM0SV;                 /**< \brief 50, Timer Register 0 Second View*/
       Ifx_STM_CAPSV                       CAPSV;                  /**< \brief 54, Timer Capture Register Second View*/
       Ifx_UReg_8Bit                       reserved_58[144];       /**< \brief 58, \internal Reserved */
       Ifx_STM_OCS                         OCS;                    /**< \brief E8, OCDS Control and Status Register*/
       Ifx_STM_KRSTCLR                     KRSTCLR;                /**< \brief EC, Kernel Reset Status Clear Register*/
       Ifx_STM_KRST1                       KRST1;                  /**< \brief F0, Kernel Reset Register 1*/
       Ifx_STM_KRST0                       KRST0;                  /**< \brief F4, Kernel Reset Register 0*/
       Ifx_STM_ACCEN1                      ACCEN1;                 /**< \brief F8, Access Enable Register 1*/
       Ifx_STM_ACCEN0                      ACCEN0;                 /**< \brief FC, Access Enable Register 0*/
} Ifx_STM;

/** \}  */
/******************************************************************************/
/** \}  */


/******************************************************************************/

/******************************************************************************/

#endif /* IFXSTM_REGDEF_H */
