/**
 * \file IfxCan_reg.h
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
 * \defgroup IfxSfr_Can_Registers_Cfg Can address
 * \ingroup IfxSfr_Can_Registers
 * 
 * \defgroup IfxSfr_Can_Registers_Cfg_BaseAddress Base address
 * \ingroup IfxSfr_Can_Registers_Cfg
 *
 * \defgroup IfxSfr_Can_Registers_Cfg_Can0 2-CAN0
 * \ingroup IfxSfr_Can_Registers_Cfg
 *
 * \defgroup IfxSfr_Can_Registers_Cfg_Can1 2-CAN1
 * \ingroup IfxSfr_Can_Registers_Cfg
 *
 * \defgroup IfxSfr_Can_Registers_Cfg_Can2 2-CAN2
 * \ingroup IfxSfr_Can_Registers_Cfg
 *
 *
 */
#ifndef IFXCAN_REG_H
#define IFXCAN_REG_H 1
/******************************************************************************/
#include "IfxCan_regdef.h"
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/

/** \addtogroup IfxSfr_Can_Registers_Cfg_BaseAddress
 * \{  */

/** \brief CAN object */
#define MODULE_CAN0 /*lint --e(923, 9078)*/ ((*(Ifx_CAN*)0xF0200000u))
#define MODULE_CAN1 /*lint --e(923, 9078)*/ ((*(Ifx_CAN*)0xF0210000u))
#define MODULE_CAN2 /*lint --e(923, 9078)*/ ((*(Ifx_CAN*)0xF0220000u))
/** \}  */


/******************************************************************************/
/******************************************************************************/
/** \addtogroup IfxSfr_Can_Registers_Cfg_Can0
 * \{  */
/** \brief 0, Embedded SRAM for messages */
#define CAN0_RAM ((void*)0xF0200000u)
#define CAN0_RAM_SIZE (0x8000u)
/** \brief 0, CAN Clock Control Register */
#define CAN0_CLC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_CLC*)0xF0208000u)

/** \brief 8, Module Identification Register */
#define CAN0_ID /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_ID*)0xF0208008u)

/** \brief 30, Module Control Register */
#define CAN0_MCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_MCR*)0xF0208030u)

/** \brief 34, Buffer receive address and transmit address */
#define CAN0_BUFADR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_BUFADR*)0xF0208034u)

/** \brief 40, Measure Control Register */
#define CAN0_MECR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_MECR*)0xF0208040u)

/** \brief 44, Measure Status Register */
#define CAN0_MESTAT /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_MESTAT*)0xF0208044u)

/** \brief DC, Access Enable Register Control 0 */
#define CAN0_ACCENCTR0 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_ACCENCTR0*)0xF02080DCu)

/** \brief E8, OCDS Control and Status */
#define CAN0_OCS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_OCS*)0xF02080E8u)

/** \brief EC, Kernel Reset Status Clear Register */
#define CAN0_KRSTCLR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_KRSTCLR*)0xF02080ECu)

/** \brief F0, Kernel Reset Register 1 */
#define CAN0_KRST1 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_KRST1*)0xF02080F0u)

/** \brief F4, Kernel Reset Register 0 */
#define CAN0_KRST0 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_KRST0*)0xF02080F4u)

/** \brief FC, Access Enable Register 0 */
#define CAN0_ACCEN0 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_ACCEN0*)0xF02080FCu)

/** \brief 100, Access Enable Register CAN Node 0 0 */
#define CAN0_N0_ACCENNODE0 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ACCENNODE0*)0xF0208100u)
/** Alias (User Manual Name) for CAN0_N0_ACCENNODE0.
* To use register names with standard convension, please use CAN0_N0_ACCENNODE0.
*/
#define CAN0_ACCENNODE00 (CAN0_N0_ACCENNODE0)

/** \brief 108, Start Address Node 0 */
#define CAN0_N0_STARTADR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_STARTADR*)0xF0208108u)
/** Alias (User Manual Name) for CAN0_N0_STARTADR.
* To use register names with standard convension, please use CAN0_N0_STARTADR.
*/
#define CAN0_STARTADR0 (CAN0_N0_STARTADR)

/** \brief 10C, End Address Node 0 */
#define CAN0_N0_ENDADR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ENDADR*)0xF020810Cu)
/** Alias (User Manual Name) for CAN0_N0_ENDADR.
* To use register names with standard convension, please use CAN0_N0_ENDADR.
*/
#define CAN0_ENDADR0 (CAN0_N0_ENDADR)

/** \brief 110, Interrupt Signalling Register 0 */
#define CAN0_N0_ISREG /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ISREG*)0xF0208110u)
/** Alias (User Manual Name) for CAN0_N0_ISREG.
* To use register names with standard convension, please use CAN0_N0_ISREG.
*/
#define CAN0_ISREG0 (CAN0_N0_ISREG)

/** \brief 114, Interrupt routing for Groups 1 0 */
#define CAN0_N0_GRINT1 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GRINT1*)0xF0208114u)
/** Alias (User Manual Name) for CAN0_N0_GRINT1.
* To use register names with standard convension, please use CAN0_N0_GRINT1.
*/
#define CAN0_GRINT10 (CAN0_N0_GRINT1)

/** \brief 118, Interrupt routing for Groups 2 0 */
#define CAN0_N0_GRINT2 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GRINT2*)0xF0208118u)
/** Alias (User Manual Name) for CAN0_N0_GRINT2.
* To use register names with standard convension, please use CAN0_N0_GRINT2.
*/
#define CAN0_GRINT20 (CAN0_N0_GRINT2)

/** \brief 120, Node 0 Timer Clock Control Register */
#define CAN0_N0_NT_CCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_CCR*)0xF0208120u)
/** Alias (User Manual Name) for CAN0_N0_NT_CCR.
* To use register names with standard convension, please use CAN0_N0_NT_CCR.
*/
#define CAN0_NTCCR0 (CAN0_N0_NT_CCR)

/** \brief 124, Node 0 Timer A Transmit Trigger Register */
#define CAN0_N0_NT_ATTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_ATTR*)0xF0208124u)
/** Alias (User Manual Name) for CAN0_N0_NT_ATTR.
* To use register names with standard convension, please use CAN0_N0_NT_ATTR.
*/
#define CAN0_NTATTR0 (CAN0_N0_NT_ATTR)

/** \brief 128, Node 0 Timer B Transmit Trigger Register */
#define CAN0_N0_NT_BTTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_BTTR*)0xF0208128u)
/** Alias (User Manual Name) for CAN0_N0_NT_BTTR.
* To use register names with standard convension, please use CAN0_N0_NT_BTTR.
*/
#define CAN0_NTBTTR0 (CAN0_N0_NT_BTTR)

/** \brief 12C, Node 0 Timer C Transmit Trigger Register */
#define CAN0_N0_NT_CTTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_CTTR*)0xF020812Cu)
/** Alias (User Manual Name) for CAN0_N0_NT_CTTR.
* To use register names with standard convension, please use CAN0_N0_NT_CTTR.
*/
#define CAN0_NTCTTR0 (CAN0_N0_NT_CTTR)

/** \brief 130, Node 0 Timer Receive Timeout Register */
#define CAN0_N0_NT_RTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_RTR*)0xF0208130u)
/** Alias (User Manual Name) for CAN0_N0_NT_RTR.
* To use register names with standard convension, please use CAN0_N0_NT_RTR.
*/
#define CAN0_NTRTR0 (CAN0_N0_NT_RTR)

/** \brief 140, Node 0 Port Control Register */
#define CAN0_N0_NPCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NPCR*)0xF0208140u)
/** Alias (User Manual Name) for CAN0_N0_NPCR.
* To use register names with standard convension, please use CAN0_N0_NPCR.
*/
#define CAN0_NPCR0 (CAN0_N0_NPCR)

/** \brief 1F0, Time Trigger Control Register */
#define CAN0_N0_TTCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TTCR*)0xF02081F0u)
/** Alias (User Manual Name) for CAN0_N0_TTCR.
* To use register names with standard convension, please use CAN0_N0_TTCR.
*/
#define CAN0_TTCR0 (CAN0_N0_TTCR)

/** \brief 200, Core Release Register 0 */
#define CAN0_N0_CREL /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_CREL*)0xF0208200u)
/** Alias (User Manual Name) for CAN0_N0_CREL.
* To use register names with standard convension, please use CAN0_N0_CREL.
*/
#define CAN0_CREL0 (CAN0_N0_CREL)

/** \brief 204, Endian Register 0 */
#define CAN0_N0_ENDN /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ENDN*)0xF0208204u)
/** Alias (User Manual Name) for CAN0_N0_ENDN.
* To use register names with standard convension, please use CAN0_N0_ENDN.
*/
#define CAN0_ENDN0 (CAN0_N0_ENDN)

/** \brief 20C, Data Bit Timing & Prescaler Register 0 */
#define CAN0_N0_DBTP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_DBTP*)0xF020820Cu)
/** Alias (User Manual Name) for CAN0_N0_DBTP.
* To use register names with standard convension, please use CAN0_N0_DBTP.
*/
#define CAN0_DBTP0 (CAN0_N0_DBTP)

/** \brief 210, Test Register 0 */
#define CAN0_N0_TEST /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TEST*)0xF0208210u)
/** Alias (User Manual Name) for CAN0_N0_TEST.
* To use register names with standard convension, please use CAN0_N0_TEST.
*/
#define CAN0_TEST0 (CAN0_N0_TEST)

/** \brief 214, RAM Watchdog 0 */
#define CAN0_N0_RWD /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RWD*)0xF0208214u)
/** Alias (User Manual Name) for CAN0_N0_RWD.
* To use register names with standard convension, please use CAN0_N0_RWD.
*/
#define CAN0_RWD0 (CAN0_N0_RWD)

/** \brief 218, CC Control Register 0 */
#define CAN0_N0_CCCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_CCCR*)0xF0208218u)
/** Alias (User Manual Name) for CAN0_N0_CCCR.
* To use register names with standard convension, please use CAN0_N0_CCCR.
*/
#define CAN0_CCCR0 (CAN0_N0_CCCR)

/** \brief 21C, Nominal Bit Timing & Prescaler Register 0 */
#define CAN0_N0_NBTP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NBTP*)0xF020821Cu)
/** Alias (User Manual Name) for CAN0_N0_NBTP.
* To use register names with standard convension, please use CAN0_N0_NBTP.
*/
#define CAN0_NBTP0 (CAN0_N0_NBTP)

/** \brief 220, Timestamp Counter Configuration 0 */
#define CAN0_N0_TSCC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TSCC*)0xF0208220u)
/** Alias (User Manual Name) for CAN0_N0_TSCC.
* To use register names with standard convension, please use CAN0_N0_TSCC.
*/
#define CAN0_TSCC0 (CAN0_N0_TSCC)

/** \brief 224, Timestamp Counter Value 0 */
#define CAN0_N0_TSCV /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TSCV*)0xF0208224u)
/** Alias (User Manual Name) for CAN0_N0_TSCV.
* To use register names with standard convension, please use CAN0_N0_TSCV.
*/
#define CAN0_TSCV0 (CAN0_N0_TSCV)

/** \brief 228, Timeout Counter Configuration 0 */
#define CAN0_N0_TOCC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TOCC*)0xF0208228u)
/** Alias (User Manual Name) for CAN0_N0_TOCC.
* To use register names with standard convension, please use CAN0_N0_TOCC.
*/
#define CAN0_TOCC0 (CAN0_N0_TOCC)

/** \brief 22C, Timeout Counter Value 0 */
#define CAN0_N0_TOCV /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TOCV*)0xF020822Cu)
/** Alias (User Manual Name) for CAN0_N0_TOCV.
* To use register names with standard convension, please use CAN0_N0_TOCV.
*/
#define CAN0_TOCV0 (CAN0_N0_TOCV)

/** \brief 240, Error Counter Register 0 */
#define CAN0_N0_ECR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ECR*)0xF0208240u)
/** Alias (User Manual Name) for CAN0_N0_ECR.
* To use register names with standard convension, please use CAN0_N0_ECR.
*/
#define CAN0_ECR0 (CAN0_N0_ECR)

/** \brief 244, Protocol Status Register 0 */
#define CAN0_N0_PSR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_PSR*)0xF0208244u)
/** Alias (User Manual Name) for CAN0_N0_PSR.
* To use register names with standard convension, please use CAN0_N0_PSR.
*/
#define CAN0_PSR0 (CAN0_N0_PSR)

/** \brief 248, Transmitter Delay Compensation Register 0 */
#define CAN0_N0_TDCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TDCR*)0xF0208248u)
/** Alias (User Manual Name) for CAN0_N0_TDCR.
* To use register names with standard convension, please use CAN0_N0_TDCR.
*/
#define CAN0_TDCR0 (CAN0_N0_TDCR)

/** \brief 250, Interrupt Register 0 */
#define CAN0_N0_IR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_IR*)0xF0208250u)
/** Alias (User Manual Name) for CAN0_N0_IR.
* To use register names with standard convension, please use CAN0_N0_IR.
*/
#define CAN0_IR0 (CAN0_N0_IR)

/** \brief 254, Interrupt Enable 0 */
#define CAN0_N0_IE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_IE*)0xF0208254u)
/** Alias (User Manual Name) for CAN0_N0_IE.
* To use register names with standard convension, please use CAN0_N0_IE.
*/
#define CAN0_IE0 (CAN0_N0_IE)

/** \brief 280, Global Filter Configuration 0 */
#define CAN0_N0_GFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GFC*)0xF0208280u)
/** Alias (User Manual Name) for CAN0_N0_GFC.
* To use register names with standard convension, please use CAN0_N0_GFC.
*/
#define CAN0_GFC0 (CAN0_N0_GFC)

/** \brief 284, Standard ID Filter Configuration 0 */
#define CAN0_N0_SIDFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_SIDFC*)0xF0208284u)
/** Alias (User Manual Name) for CAN0_N0_SIDFC.
* To use register names with standard convension, please use CAN0_N0_SIDFC.
*/
#define CAN0_SIDFC0 (CAN0_N0_SIDFC)

/** \brief 288, Extended ID Filter Configuration 0 */
#define CAN0_N0_XIDFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_XIDFC*)0xF0208288u)
/** Alias (User Manual Name) for CAN0_N0_XIDFC.
* To use register names with standard convension, please use CAN0_N0_XIDFC.
*/
#define CAN0_XIDFC0 (CAN0_N0_XIDFC)

/** \brief 290, Extended ID AND Mask 0 */
#define CAN0_N0_XIDAM /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_XIDAM*)0xF0208290u)
/** Alias (User Manual Name) for CAN0_N0_XIDAM.
* To use register names with standard convension, please use CAN0_N0_XIDAM.
*/
#define CAN0_XIDAM0 (CAN0_N0_XIDAM)

/** \brief 294, High Priority Message Status 0 */
#define CAN0_N0_HPMS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_HPMS*)0xF0208294u)
/** Alias (User Manual Name) for CAN0_N0_HPMS.
* To use register names with standard convension, please use CAN0_N0_HPMS.
*/
#define CAN0_HPMS0 (CAN0_N0_HPMS)

/** \brief 298, New Data 1 0 */
#define CAN0_N0_NDAT1 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NDAT1*)0xF0208298u)
/** Alias (User Manual Name) for CAN0_N0_NDAT1.
* To use register names with standard convension, please use CAN0_N0_NDAT1.
*/
#define CAN0_NDAT10 (CAN0_N0_NDAT1)

/** \brief 29C, New Data 2 0 */
#define CAN0_N0_NDAT2 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NDAT2*)0xF020829Cu)
/** Alias (User Manual Name) for CAN0_N0_NDAT2.
* To use register names with standard convension, please use CAN0_N0_NDAT2.
*/
#define CAN0_NDAT20 (CAN0_N0_NDAT2)

/** \brief 2A0, Rx FIFO 0 Configuration 0 */
#define CAN0_N0_RX_F0C /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0C*)0xF02082A0u)
/** Alias (User Manual Name) for CAN0_N0_RX_F0C.
* To use register names with standard convension, please use CAN0_N0_RX_F0C.
*/
#define CAN0_RXF0C0 (CAN0_N0_RX_F0C)

/** \brief 2A4, Rx FIFO 0 Status 0 */
#define CAN0_N0_RX_F0S /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0S*)0xF02082A4u)
/** Alias (User Manual Name) for CAN0_N0_RX_F0S.
* To use register names with standard convension, please use CAN0_N0_RX_F0S.
*/
#define CAN0_RXF0S0 (CAN0_N0_RX_F0S)

/** \brief 2A8, Rx FIFO 0 Acknowledge 0 */
#define CAN0_N0_RX_F0A /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0A*)0xF02082A8u)
/** Alias (User Manual Name) for CAN0_N0_RX_F0A.
* To use register names with standard convension, please use CAN0_N0_RX_F0A.
*/
#define CAN0_RXF0A0 (CAN0_N0_RX_F0A)

/** \brief 2AC, Rx Buffer Configuration 0 */
#define CAN0_N0_RX_BC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_BC*)0xF02082ACu)
/** Alias (User Manual Name) for CAN0_N0_RX_BC.
* To use register names with standard convension, please use CAN0_N0_RX_BC.
*/
#define CAN0_RXBC0 (CAN0_N0_RX_BC)

/** \brief 2B0, Rx FIFO 1 Configuration 0 */
#define CAN0_N0_RX_F1C /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1C*)0xF02082B0u)
/** Alias (User Manual Name) for CAN0_N0_RX_F1C.
* To use register names with standard convension, please use CAN0_N0_RX_F1C.
*/
#define CAN0_RXF1C0 (CAN0_N0_RX_F1C)

/** \brief 2B4, Rx FIFO 1 Status 0 */
#define CAN0_N0_RX_F1S /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1S*)0xF02082B4u)
/** Alias (User Manual Name) for CAN0_N0_RX_F1S.
* To use register names with standard convension, please use CAN0_N0_RX_F1S.
*/
#define CAN0_RXF1S0 (CAN0_N0_RX_F1S)

/** \brief 2B8, Rx FIFO 1 Acknowledge 0 */
#define CAN0_N0_RX_F1A /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1A*)0xF02082B8u)
/** Alias (User Manual Name) for CAN0_N0_RX_F1A.
* To use register names with standard convension, please use CAN0_N0_RX_F1A.
*/
#define CAN0_RXF1A0 (CAN0_N0_RX_F1A)

/** \brief 2BC, Rx Buffer/FIFO Element Size Configuration 0 */
#define CAN0_N0_RX_ESC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_ESC*)0xF02082BCu)
/** Alias (User Manual Name) for CAN0_N0_RX_ESC.
* To use register names with standard convension, please use CAN0_N0_RX_ESC.
*/
#define CAN0_RXESC0 (CAN0_N0_RX_ESC)

/** \brief 2C0, Tx Buffer Configuration 0 */
#define CAN0_N0_TX_BC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BC*)0xF02082C0u)
/** Alias (User Manual Name) for CAN0_N0_TX_BC.
* To use register names with standard convension, please use CAN0_N0_TX_BC.
*/
#define CAN0_TXBC0 (CAN0_N0_TX_BC)

/** \brief 2C4, Tx FIFO/Queue Status 0 */
#define CAN0_N0_TX_FQS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_FQS*)0xF02082C4u)
/** Alias (User Manual Name) for CAN0_N0_TX_FQS.
* To use register names with standard convension, please use CAN0_N0_TX_FQS.
*/
#define CAN0_TXFQS0 (CAN0_N0_TX_FQS)

/** \brief 2C8, Tx Buffer Element Size Configuration 0 */
#define CAN0_N0_TX_ESC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_ESC*)0xF02082C8u)
/** Alias (User Manual Name) for CAN0_N0_TX_ESC.
* To use register names with standard convension, please use CAN0_N0_TX_ESC.
*/
#define CAN0_TXESC0 (CAN0_N0_TX_ESC)

/** \brief 2CC, Tx Buffer Request Pending 0 */
#define CAN0_N0_TX_BRP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BRP*)0xF02082CCu)
/** Alias (User Manual Name) for CAN0_N0_TX_BRP.
* To use register names with standard convension, please use CAN0_N0_TX_BRP.
*/
#define CAN0_TXBRP0 (CAN0_N0_TX_BRP)

/** \brief 2D0, Tx Buffer Add Request 0 */
#define CAN0_N0_TX_BAR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BAR*)0xF02082D0u)
/** Alias (User Manual Name) for CAN0_N0_TX_BAR.
* To use register names with standard convension, please use CAN0_N0_TX_BAR.
*/
#define CAN0_TXBAR0 (CAN0_N0_TX_BAR)

/** \brief 2D4, Tx Buffer Cancellation Request 0 */
#define CAN0_N0_TX_BCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCR*)0xF02082D4u)
/** Alias (User Manual Name) for CAN0_N0_TX_BCR.
* To use register names with standard convension, please use CAN0_N0_TX_BCR.
*/
#define CAN0_TXBCR0 (CAN0_N0_TX_BCR)

/** \brief 2D8, Tx Buffer Transmission Occurred 0 */
#define CAN0_N0_TX_BTO /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BTO*)0xF02082D8u)
/** Alias (User Manual Name) for CAN0_N0_TX_BTO.
* To use register names with standard convension, please use CAN0_N0_TX_BTO.
*/
#define CAN0_TXBTO0 (CAN0_N0_TX_BTO)

/** \brief 2DC, Tx Buffer Cancellation Finished 0 */
#define CAN0_N0_TX_BCF /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCF*)0xF02082DCu)
/** Alias (User Manual Name) for CAN0_N0_TX_BCF.
* To use register names with standard convension, please use CAN0_N0_TX_BCF.
*/
#define CAN0_TXBCF0 (CAN0_N0_TX_BCF)

/** \brief 2E0, Tx Buffer Transmission Interrupt Enable 0 */
#define CAN0_N0_TX_BTIE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BTIE*)0xF02082E0u)
/** Alias (User Manual Name) for CAN0_N0_TX_BTIE.
* To use register names with standard convension, please use CAN0_N0_TX_BTIE.
*/
#define CAN0_TXBTIE0 (CAN0_N0_TX_BTIE)

/** \brief 2E4, Tx Buffer Cancellation Finished Interrupt Enable 0 */
#define CAN0_N0_TX_BCIE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCIE*)0xF02082E4u)
/** Alias (User Manual Name) for CAN0_N0_TX_BCIE.
* To use register names with standard convension, please use CAN0_N0_TX_BCIE.
*/
#define CAN0_TXBCIE0 (CAN0_N0_TX_BCIE)

/** \brief 2F0, Tx Event FIFO Configuration 0 */
#define CAN0_N0_TX_EFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFC*)0xF02082F0u)
/** Alias (User Manual Name) for CAN0_N0_TX_EFC.
* To use register names with standard convension, please use CAN0_N0_TX_EFC.
*/
#define CAN0_TXEFC0 (CAN0_N0_TX_EFC)

/** \brief 2F4, Tx Event FIFO Status 0 */
#define CAN0_N0_TX_EFS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFS*)0xF02082F4u)
/** Alias (User Manual Name) for CAN0_N0_TX_EFS.
* To use register names with standard convension, please use CAN0_N0_TX_EFS.
*/
#define CAN0_TXEFS0 (CAN0_N0_TX_EFS)

/** \brief 2F8, Tx Event FIFO Acknowledge 0 */
#define CAN0_N0_TX_EFA /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFA*)0xF02082F8u)
/** Alias (User Manual Name) for CAN0_N0_TX_EFA.
* To use register names with standard convension, please use CAN0_N0_TX_EFA.
*/
#define CAN0_TXEFA0 (CAN0_N0_TX_EFA)

/** \brief 300, TT Trigger Memory Configuration 0 */
#define CAN0_N0_TT_TMC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TT_TMC*)0xF0208300u)
/** Alias (User Manual Name) for CAN0_N0_TT_TMC.
* To use register names with standard convension, please use CAN0_N0_TT_TMC.
*/
#define CAN0_TTTMC0 (CAN0_N0_TT_TMC)

/** \brief 304, TT Reference Message Configuration 0 */
#define CAN0_N0_TT_RMC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TT_RMC*)0xF0208304u)
/** Alias (User Manual Name) for CAN0_N0_TT_RMC.
* To use register names with standard convension, please use CAN0_N0_TT_RMC.
*/
#define CAN0_TTRMC0 (CAN0_N0_TT_RMC)

/** \brief 308, TT Operation Configuration 0 */
#define CAN0_N0_TT_OCF /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TT_OCF*)0xF0208308u)
/** Alias (User Manual Name) for CAN0_N0_TT_OCF.
* To use register names with standard convension, please use CAN0_N0_TT_OCF.
*/
#define CAN0_TTOCF0 (CAN0_N0_TT_OCF)

/** \brief 30C, TT Matrix Limits 0 */
#define CAN0_N0_TT_MLM /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TT_MLM*)0xF020830Cu)
/** Alias (User Manual Name) for CAN0_N0_TT_MLM.
* To use register names with standard convension, please use CAN0_N0_TT_MLM.
*/
#define CAN0_TTMLM0 (CAN0_N0_TT_MLM)

/** \brief 310, TUR Configuration 0 */
#define CAN0_N0_TT_TURCF /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TT_TURCF*)0xF0208310u)
/** Alias (User Manual Name) for CAN0_N0_TT_TURCF.
* To use register names with standard convension, please use CAN0_N0_TT_TURCF.
*/
#define CAN0_TURCF0 (CAN0_N0_TT_TURCF)

/** \brief 314, TT Operation Control 0 */
#define CAN0_N0_TT_OCN /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TT_OCN*)0xF0208314u)
/** Alias (User Manual Name) for CAN0_N0_TT_OCN.
* To use register names with standard convension, please use CAN0_N0_TT_OCN.
*/
#define CAN0_TTOCN0 (CAN0_N0_TT_OCN)

/** \brief 318, TT Global Time Preset 0 */
#define CAN0_N0_TT_GTP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TT_GTP*)0xF0208318u)
/** Alias (User Manual Name) for CAN0_N0_TT_GTP.
* To use register names with standard convension, please use CAN0_N0_TT_GTP.
*/
#define CAN0_TTGTP0 (CAN0_N0_TT_GTP)

/** \brief 31C, TT Time Mark 0 */
#define CAN0_N0_TT_TMK /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TT_TMK*)0xF020831Cu)
/** Alias (User Manual Name) for CAN0_N0_TT_TMK.
* To use register names with standard convension, please use CAN0_N0_TT_TMK.
*/
#define CAN0_TTTMK0 (CAN0_N0_TT_TMK)

/** \brief 320, TT Interrupt Register 0 */
#define CAN0_N0_TT_IR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TT_IR*)0xF0208320u)
/** Alias (User Manual Name) for CAN0_N0_TT_IR.
* To use register names with standard convension, please use CAN0_N0_TT_IR.
*/
#define CAN0_TTIR0 (CAN0_N0_TT_IR)

/** \brief 324, TT Interrupt Enable 0 */
#define CAN0_N0_TT_IE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TT_IE*)0xF0208324u)
/** Alias (User Manual Name) for CAN0_N0_TT_IE.
* To use register names with standard convension, please use CAN0_N0_TT_IE.
*/
#define CAN0_TTIE0 (CAN0_N0_TT_IE)

/** \brief 32C, TT Operation Status 0 */
#define CAN0_N0_TT_OST /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TT_OST*)0xF020832Cu)
/** Alias (User Manual Name) for CAN0_N0_TT_OST.
* To use register names with standard convension, please use CAN0_N0_TT_OST.
*/
#define CAN0_TTOST0 (CAN0_N0_TT_OST)

/** \brief 330, TUR Numerator Actual 0 */
#define CAN0_N0_TT_TURNA /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TT_TURNA*)0xF0208330u)
/** Alias (User Manual Name) for CAN0_N0_TT_TURNA.
* To use register names with standard convension, please use CAN0_N0_TT_TURNA.
*/
#define CAN0_TURNA0 (CAN0_N0_TT_TURNA)

/** \brief 334, TT Local & Global Time 0 */
#define CAN0_N0_TT_LGT /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TT_LGT*)0xF0208334u)
/** Alias (User Manual Name) for CAN0_N0_TT_LGT.
* To use register names with standard convension, please use CAN0_N0_TT_LGT.
*/
#define CAN0_TTLGT0 (CAN0_N0_TT_LGT)

/** \brief 338, TT Cycle Time & Count 0 */
#define CAN0_N0_TT_CTC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TT_CTC*)0xF0208338u)
/** Alias (User Manual Name) for CAN0_N0_TT_CTC.
* To use register names with standard convension, please use CAN0_N0_TT_CTC.
*/
#define CAN0_TTCTC0 (CAN0_N0_TT_CTC)

/** \brief 33C, TT Capture Time 0 */
#define CAN0_N0_TT_CPT /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TT_CPT*)0xF020833Cu)
/** Alias (User Manual Name) for CAN0_N0_TT_CPT.
* To use register names with standard convension, please use CAN0_N0_TT_CPT.
*/
#define CAN0_TTCPT0 (CAN0_N0_TT_CPT)

/** \brief 340, TT Cycle Sync Mark 0 */
#define CAN0_N0_TT_CSM /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TT_CSM*)0xF0208340u)
/** Alias (User Manual Name) for CAN0_N0_TT_CSM.
* To use register names with standard convension, please use CAN0_N0_TT_CSM.
*/
#define CAN0_TTCSM0 (CAN0_N0_TT_CSM)

/** \brief 500, Access Enable Register CAN Node 1 0 */
#define CAN0_N1_ACCENNODE0 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ACCENNODE0*)0xF0208500u)
/** Alias (User Manual Name) for CAN0_N1_ACCENNODE0.
* To use register names with standard convension, please use CAN0_N1_ACCENNODE0.
*/
#define CAN0_ACCENNODE10 (CAN0_N1_ACCENNODE0)

/** \brief 508, Start Address Node 1 */
#define CAN0_N1_STARTADR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_STARTADR*)0xF0208508u)
/** Alias (User Manual Name) for CAN0_N1_STARTADR.
* To use register names with standard convension, please use CAN0_N1_STARTADR.
*/
#define CAN0_STARTADR1 (CAN0_N1_STARTADR)

/** \brief 50C, End Address Node 1 */
#define CAN0_N1_ENDADR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ENDADR*)0xF020850Cu)
/** Alias (User Manual Name) for CAN0_N1_ENDADR.
* To use register names with standard convension, please use CAN0_N1_ENDADR.
*/
#define CAN0_ENDADR1 (CAN0_N1_ENDADR)

/** \brief 510, Interrupt Signalling Register 1 */
#define CAN0_N1_ISREG /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ISREG*)0xF0208510u)
/** Alias (User Manual Name) for CAN0_N1_ISREG.
* To use register names with standard convension, please use CAN0_N1_ISREG.
*/
#define CAN0_ISREG1 (CAN0_N1_ISREG)

/** \brief 514, Interrupt routing for Groups 1 1 */
#define CAN0_N1_GRINT1 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GRINT1*)0xF0208514u)
/** Alias (User Manual Name) for CAN0_N1_GRINT1.
* To use register names with standard convension, please use CAN0_N1_GRINT1.
*/
#define CAN0_GRINT11 (CAN0_N1_GRINT1)

/** \brief 518, Interrupt routing for Groups 2 1 */
#define CAN0_N1_GRINT2 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GRINT2*)0xF0208518u)
/** Alias (User Manual Name) for CAN0_N1_GRINT2.
* To use register names with standard convension, please use CAN0_N1_GRINT2.
*/
#define CAN0_GRINT21 (CAN0_N1_GRINT2)

/** \brief 520, Node 1 Timer Clock Control Register */
#define CAN0_N1_NT_CCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_CCR*)0xF0208520u)
/** Alias (User Manual Name) for CAN0_N1_NT_CCR.
* To use register names with standard convension, please use CAN0_N1_NT_CCR.
*/
#define CAN0_NTCCR1 (CAN0_N1_NT_CCR)

/** \brief 524, Node 1 Timer A Transmit Trigger Register */
#define CAN0_N1_NT_ATTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_ATTR*)0xF0208524u)
/** Alias (User Manual Name) for CAN0_N1_NT_ATTR.
* To use register names with standard convension, please use CAN0_N1_NT_ATTR.
*/
#define CAN0_NTATTR1 (CAN0_N1_NT_ATTR)

/** \brief 528, Node 1 Timer B Transmit Trigger Register */
#define CAN0_N1_NT_BTTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_BTTR*)0xF0208528u)
/** Alias (User Manual Name) for CAN0_N1_NT_BTTR.
* To use register names with standard convension, please use CAN0_N1_NT_BTTR.
*/
#define CAN0_NTBTTR1 (CAN0_N1_NT_BTTR)

/** \brief 52C, Node 1 Timer C Transmit Trigger Register */
#define CAN0_N1_NT_CTTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_CTTR*)0xF020852Cu)
/** Alias (User Manual Name) for CAN0_N1_NT_CTTR.
* To use register names with standard convension, please use CAN0_N1_NT_CTTR.
*/
#define CAN0_NTCTTR1 (CAN0_N1_NT_CTTR)

/** \brief 530, Node 1 Timer Receive Timeout Register */
#define CAN0_N1_NT_RTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_RTR*)0xF0208530u)
/** Alias (User Manual Name) for CAN0_N1_NT_RTR.
* To use register names with standard convension, please use CAN0_N1_NT_RTR.
*/
#define CAN0_NTRTR1 (CAN0_N1_NT_RTR)

/** \brief 540, Node 1 Port Control Register */
#define CAN0_N1_NPCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NPCR*)0xF0208540u)
/** Alias (User Manual Name) for CAN0_N1_NPCR.
* To use register names with standard convension, please use CAN0_N1_NPCR.
*/
#define CAN0_NPCR1 (CAN0_N1_NPCR)

/** \brief 600, Core Release Register 1 */
#define CAN0_N1_CREL /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_CREL*)0xF0208600u)
/** Alias (User Manual Name) for CAN0_N1_CREL.
* To use register names with standard convension, please use CAN0_N1_CREL.
*/
#define CAN0_CREL1 (CAN0_N1_CREL)

/** \brief 604, Endian Register 1 */
#define CAN0_N1_ENDN /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ENDN*)0xF0208604u)
/** Alias (User Manual Name) for CAN0_N1_ENDN.
* To use register names with standard convension, please use CAN0_N1_ENDN.
*/
#define CAN0_ENDN1 (CAN0_N1_ENDN)

/** \brief 60C, Data Bit Timing & Prescaler Register 1 */
#define CAN0_N1_DBTP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_DBTP*)0xF020860Cu)
/** Alias (User Manual Name) for CAN0_N1_DBTP.
* To use register names with standard convension, please use CAN0_N1_DBTP.
*/
#define CAN0_DBTP1 (CAN0_N1_DBTP)

/** \brief 610, Test Register 1 */
#define CAN0_N1_TEST /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TEST*)0xF0208610u)
/** Alias (User Manual Name) for CAN0_N1_TEST.
* To use register names with standard convension, please use CAN0_N1_TEST.
*/
#define CAN0_TEST1 (CAN0_N1_TEST)

/** \brief 614, RAM Watchdog 1 */
#define CAN0_N1_RWD /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RWD*)0xF0208614u)
/** Alias (User Manual Name) for CAN0_N1_RWD.
* To use register names with standard convension, please use CAN0_N1_RWD.
*/
#define CAN0_RWD1 (CAN0_N1_RWD)

/** \brief 618, CC Control Register 1 */
#define CAN0_N1_CCCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_CCCR*)0xF0208618u)
/** Alias (User Manual Name) for CAN0_N1_CCCR.
* To use register names with standard convension, please use CAN0_N1_CCCR.
*/
#define CAN0_CCCR1 (CAN0_N1_CCCR)

/** \brief 61C, Nominal Bit Timing & Prescaler Register 1 */
#define CAN0_N1_NBTP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NBTP*)0xF020861Cu)
/** Alias (User Manual Name) for CAN0_N1_NBTP.
* To use register names with standard convension, please use CAN0_N1_NBTP.
*/
#define CAN0_NBTP1 (CAN0_N1_NBTP)

/** \brief 620, Timestamp Counter Configuration 1 */
#define CAN0_N1_TSCC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TSCC*)0xF0208620u)
/** Alias (User Manual Name) for CAN0_N1_TSCC.
* To use register names with standard convension, please use CAN0_N1_TSCC.
*/
#define CAN0_TSCC1 (CAN0_N1_TSCC)

/** \brief 624, Timestamp Counter Value 1 */
#define CAN0_N1_TSCV /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TSCV*)0xF0208624u)
/** Alias (User Manual Name) for CAN0_N1_TSCV.
* To use register names with standard convension, please use CAN0_N1_TSCV.
*/
#define CAN0_TSCV1 (CAN0_N1_TSCV)

/** \brief 628, Timeout Counter Configuration 1 */
#define CAN0_N1_TOCC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TOCC*)0xF0208628u)
/** Alias (User Manual Name) for CAN0_N1_TOCC.
* To use register names with standard convension, please use CAN0_N1_TOCC.
*/
#define CAN0_TOCC1 (CAN0_N1_TOCC)

/** \brief 62C, Timeout Counter Value 1 */
#define CAN0_N1_TOCV /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TOCV*)0xF020862Cu)
/** Alias (User Manual Name) for CAN0_N1_TOCV.
* To use register names with standard convension, please use CAN0_N1_TOCV.
*/
#define CAN0_TOCV1 (CAN0_N1_TOCV)

/** \brief 640, Error Counter Register 1 */
#define CAN0_N1_ECR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ECR*)0xF0208640u)
/** Alias (User Manual Name) for CAN0_N1_ECR.
* To use register names with standard convension, please use CAN0_N1_ECR.
*/
#define CAN0_ECR1 (CAN0_N1_ECR)

/** \brief 644, Protocol Status Register 1 */
#define CAN0_N1_PSR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_PSR*)0xF0208644u)
/** Alias (User Manual Name) for CAN0_N1_PSR.
* To use register names with standard convension, please use CAN0_N1_PSR.
*/
#define CAN0_PSR1 (CAN0_N1_PSR)

/** \brief 648, Transmitter Delay Compensation Register 1 */
#define CAN0_N1_TDCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TDCR*)0xF0208648u)
/** Alias (User Manual Name) for CAN0_N1_TDCR.
* To use register names with standard convension, please use CAN0_N1_TDCR.
*/
#define CAN0_TDCR1 (CAN0_N1_TDCR)

/** \brief 650, Interrupt Register 1 */
#define CAN0_N1_IR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_IR*)0xF0208650u)
/** Alias (User Manual Name) for CAN0_N1_IR.
* To use register names with standard convension, please use CAN0_N1_IR.
*/
#define CAN0_IR1 (CAN0_N1_IR)

/** \brief 654, Interrupt Enable 1 */
#define CAN0_N1_IE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_IE*)0xF0208654u)
/** Alias (User Manual Name) for CAN0_N1_IE.
* To use register names with standard convension, please use CAN0_N1_IE.
*/
#define CAN0_IE1 (CAN0_N1_IE)

/** \brief 680, Global Filter Configuration 1 */
#define CAN0_N1_GFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GFC*)0xF0208680u)
/** Alias (User Manual Name) for CAN0_N1_GFC.
* To use register names with standard convension, please use CAN0_N1_GFC.
*/
#define CAN0_GFC1 (CAN0_N1_GFC)

/** \brief 684, Standard ID Filter Configuration 1 */
#define CAN0_N1_SIDFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_SIDFC*)0xF0208684u)
/** Alias (User Manual Name) for CAN0_N1_SIDFC.
* To use register names with standard convension, please use CAN0_N1_SIDFC.
*/
#define CAN0_SIDFC1 (CAN0_N1_SIDFC)

/** \brief 688, Extended ID Filter Configuration 1 */
#define CAN0_N1_XIDFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_XIDFC*)0xF0208688u)
/** Alias (User Manual Name) for CAN0_N1_XIDFC.
* To use register names with standard convension, please use CAN0_N1_XIDFC.
*/
#define CAN0_XIDFC1 (CAN0_N1_XIDFC)

/** \brief 690, Extended ID AND Mask 1 */
#define CAN0_N1_XIDAM /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_XIDAM*)0xF0208690u)
/** Alias (User Manual Name) for CAN0_N1_XIDAM.
* To use register names with standard convension, please use CAN0_N1_XIDAM.
*/
#define CAN0_XIDAM1 (CAN0_N1_XIDAM)

/** \brief 694, High Priority Message Status 1 */
#define CAN0_N1_HPMS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_HPMS*)0xF0208694u)
/** Alias (User Manual Name) for CAN0_N1_HPMS.
* To use register names with standard convension, please use CAN0_N1_HPMS.
*/
#define CAN0_HPMS1 (CAN0_N1_HPMS)

/** \brief 698, New Data 1 1 */
#define CAN0_N1_NDAT1 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NDAT1*)0xF0208698u)
/** Alias (User Manual Name) for CAN0_N1_NDAT1.
* To use register names with standard convension, please use CAN0_N1_NDAT1.
*/
#define CAN0_NDAT11 (CAN0_N1_NDAT1)

/** \brief 69C, New Data 2 1 */
#define CAN0_N1_NDAT2 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NDAT2*)0xF020869Cu)
/** Alias (User Manual Name) for CAN0_N1_NDAT2.
* To use register names with standard convension, please use CAN0_N1_NDAT2.
*/
#define CAN0_NDAT21 (CAN0_N1_NDAT2)

/** \brief 6A0, Rx FIFO 0 Configuration 1 */
#define CAN0_N1_RX_F0C /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0C*)0xF02086A0u)
/** Alias (User Manual Name) for CAN0_N1_RX_F0C.
* To use register names with standard convension, please use CAN0_N1_RX_F0C.
*/
#define CAN0_RXF0C1 (CAN0_N1_RX_F0C)

/** \brief 6A4, Rx FIFO 0 Status 1 */
#define CAN0_N1_RX_F0S /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0S*)0xF02086A4u)
/** Alias (User Manual Name) for CAN0_N1_RX_F0S.
* To use register names with standard convension, please use CAN0_N1_RX_F0S.
*/
#define CAN0_RXF0S1 (CAN0_N1_RX_F0S)

/** \brief 6A8, Rx FIFO 0 Acknowledge 1 */
#define CAN0_N1_RX_F0A /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0A*)0xF02086A8u)
/** Alias (User Manual Name) for CAN0_N1_RX_F0A.
* To use register names with standard convension, please use CAN0_N1_RX_F0A.
*/
#define CAN0_RXF0A1 (CAN0_N1_RX_F0A)

/** \brief 6AC, Rx Buffer Configuration 1 */
#define CAN0_N1_RX_BC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_BC*)0xF02086ACu)
/** Alias (User Manual Name) for CAN0_N1_RX_BC.
* To use register names with standard convension, please use CAN0_N1_RX_BC.
*/
#define CAN0_RXBC1 (CAN0_N1_RX_BC)

/** \brief 6B0, Rx FIFO 1 Configuration 1 */
#define CAN0_N1_RX_F1C /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1C*)0xF02086B0u)
/** Alias (User Manual Name) for CAN0_N1_RX_F1C.
* To use register names with standard convension, please use CAN0_N1_RX_F1C.
*/
#define CAN0_RXF1C1 (CAN0_N1_RX_F1C)

/** \brief 6B4, Rx FIFO 1 Status 1 */
#define CAN0_N1_RX_F1S /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1S*)0xF02086B4u)
/** Alias (User Manual Name) for CAN0_N1_RX_F1S.
* To use register names with standard convension, please use CAN0_N1_RX_F1S.
*/
#define CAN0_RXF1S1 (CAN0_N1_RX_F1S)

/** \brief 6B8, Rx FIFO 1 Acknowledge 1 */
#define CAN0_N1_RX_F1A /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1A*)0xF02086B8u)
/** Alias (User Manual Name) for CAN0_N1_RX_F1A.
* To use register names with standard convension, please use CAN0_N1_RX_F1A.
*/
#define CAN0_RXF1A1 (CAN0_N1_RX_F1A)

/** \brief 6BC, Rx Buffer/FIFO Element Size Configuration 1 */
#define CAN0_N1_RX_ESC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_ESC*)0xF02086BCu)
/** Alias (User Manual Name) for CAN0_N1_RX_ESC.
* To use register names with standard convension, please use CAN0_N1_RX_ESC.
*/
#define CAN0_RXESC1 (CAN0_N1_RX_ESC)

/** \brief 6C0, Tx Buffer Configuration 1 */
#define CAN0_N1_TX_BC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BC*)0xF02086C0u)
/** Alias (User Manual Name) for CAN0_N1_TX_BC.
* To use register names with standard convension, please use CAN0_N1_TX_BC.
*/
#define CAN0_TXBC1 (CAN0_N1_TX_BC)

/** \brief 6C4, Tx FIFO/Queue Status 1 */
#define CAN0_N1_TX_FQS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_FQS*)0xF02086C4u)
/** Alias (User Manual Name) for CAN0_N1_TX_FQS.
* To use register names with standard convension, please use CAN0_N1_TX_FQS.
*/
#define CAN0_TXFQS1 (CAN0_N1_TX_FQS)

/** \brief 6C8, Tx Buffer Element Size Configuration 1 */
#define CAN0_N1_TX_ESC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_ESC*)0xF02086C8u)
/** Alias (User Manual Name) for CAN0_N1_TX_ESC.
* To use register names with standard convension, please use CAN0_N1_TX_ESC.
*/
#define CAN0_TXESC1 (CAN0_N1_TX_ESC)

/** \brief 6CC, Tx Buffer Request Pending 1 */
#define CAN0_N1_TX_BRP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BRP*)0xF02086CCu)
/** Alias (User Manual Name) for CAN0_N1_TX_BRP.
* To use register names with standard convension, please use CAN0_N1_TX_BRP.
*/
#define CAN0_TXBRP1 (CAN0_N1_TX_BRP)

/** \brief 6D0, Tx Buffer Add Request 1 */
#define CAN0_N1_TX_BAR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BAR*)0xF02086D0u)
/** Alias (User Manual Name) for CAN0_N1_TX_BAR.
* To use register names with standard convension, please use CAN0_N1_TX_BAR.
*/
#define CAN0_TXBAR1 (CAN0_N1_TX_BAR)

/** \brief 6D4, Tx Buffer Cancellation Request 1 */
#define CAN0_N1_TX_BCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCR*)0xF02086D4u)
/** Alias (User Manual Name) for CAN0_N1_TX_BCR.
* To use register names with standard convension, please use CAN0_N1_TX_BCR.
*/
#define CAN0_TXBCR1 (CAN0_N1_TX_BCR)

/** \brief 6D8, Tx Buffer Transmission Occurred 1 */
#define CAN0_N1_TX_BTO /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BTO*)0xF02086D8u)
/** Alias (User Manual Name) for CAN0_N1_TX_BTO.
* To use register names with standard convension, please use CAN0_N1_TX_BTO.
*/
#define CAN0_TXBTO1 (CAN0_N1_TX_BTO)

/** \brief 6DC, Tx Buffer Cancellation Finished 1 */
#define CAN0_N1_TX_BCF /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCF*)0xF02086DCu)
/** Alias (User Manual Name) for CAN0_N1_TX_BCF.
* To use register names with standard convension, please use CAN0_N1_TX_BCF.
*/
#define CAN0_TXBCF1 (CAN0_N1_TX_BCF)

/** \brief 6E0, Tx Buffer Transmission Interrupt Enable 1 */
#define CAN0_N1_TX_BTIE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BTIE*)0xF02086E0u)
/** Alias (User Manual Name) for CAN0_N1_TX_BTIE.
* To use register names with standard convension, please use CAN0_N1_TX_BTIE.
*/
#define CAN0_TXBTIE1 (CAN0_N1_TX_BTIE)

/** \brief 6E4, Tx Buffer Cancellation Finished Interrupt Enable 1 */
#define CAN0_N1_TX_BCIE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCIE*)0xF02086E4u)
/** Alias (User Manual Name) for CAN0_N1_TX_BCIE.
* To use register names with standard convension, please use CAN0_N1_TX_BCIE.
*/
#define CAN0_TXBCIE1 (CAN0_N1_TX_BCIE)

/** \brief 6F0, Tx Event FIFO Configuration 1 */
#define CAN0_N1_TX_EFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFC*)0xF02086F0u)
/** Alias (User Manual Name) for CAN0_N1_TX_EFC.
* To use register names with standard convension, please use CAN0_N1_TX_EFC.
*/
#define CAN0_TXEFC1 (CAN0_N1_TX_EFC)

/** \brief 6F4, Tx Event FIFO Status 1 */
#define CAN0_N1_TX_EFS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFS*)0xF02086F4u)
/** Alias (User Manual Name) for CAN0_N1_TX_EFS.
* To use register names with standard convension, please use CAN0_N1_TX_EFS.
*/
#define CAN0_TXEFS1 (CAN0_N1_TX_EFS)

/** \brief 6F8, Tx Event FIFO Acknowledge 1 */
#define CAN0_N1_TX_EFA /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFA*)0xF02086F8u)
/** Alias (User Manual Name) for CAN0_N1_TX_EFA.
* To use register names with standard convension, please use CAN0_N1_TX_EFA.
*/
#define CAN0_TXEFA1 (CAN0_N1_TX_EFA)

/** \brief 900, Access Enable Register CAN Node 2 0 */
#define CAN0_N2_ACCENNODE0 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ACCENNODE0*)0xF0208900u)
/** Alias (User Manual Name) for CAN0_N2_ACCENNODE0.
* To use register names with standard convension, please use CAN0_N2_ACCENNODE0.
*/
#define CAN0_ACCENNODE20 (CAN0_N2_ACCENNODE0)

/** \brief 908, Start Address Node 2 */
#define CAN0_N2_STARTADR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_STARTADR*)0xF0208908u)
/** Alias (User Manual Name) for CAN0_N2_STARTADR.
* To use register names with standard convension, please use CAN0_N2_STARTADR.
*/
#define CAN0_STARTADR2 (CAN0_N2_STARTADR)

/** \brief 90C, End Address Node 2 */
#define CAN0_N2_ENDADR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ENDADR*)0xF020890Cu)
/** Alias (User Manual Name) for CAN0_N2_ENDADR.
* To use register names with standard convension, please use CAN0_N2_ENDADR.
*/
#define CAN0_ENDADR2 (CAN0_N2_ENDADR)

/** \brief 910, Interrupt Signalling Register 2 */
#define CAN0_N2_ISREG /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ISREG*)0xF0208910u)
/** Alias (User Manual Name) for CAN0_N2_ISREG.
* To use register names with standard convension, please use CAN0_N2_ISREG.
*/
#define CAN0_ISREG2 (CAN0_N2_ISREG)

/** \brief 914, Interrupt routing for Groups 1 2 */
#define CAN0_N2_GRINT1 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GRINT1*)0xF0208914u)
/** Alias (User Manual Name) for CAN0_N2_GRINT1.
* To use register names with standard convension, please use CAN0_N2_GRINT1.
*/
#define CAN0_GRINT12 (CAN0_N2_GRINT1)

/** \brief 918, Interrupt routing for Groups 2 2 */
#define CAN0_N2_GRINT2 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GRINT2*)0xF0208918u)
/** Alias (User Manual Name) for CAN0_N2_GRINT2.
* To use register names with standard convension, please use CAN0_N2_GRINT2.
*/
#define CAN0_GRINT22 (CAN0_N2_GRINT2)

/** \brief 920, Node 2 Timer Clock Control Register */
#define CAN0_N2_NT_CCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_CCR*)0xF0208920u)
/** Alias (User Manual Name) for CAN0_N2_NT_CCR.
* To use register names with standard convension, please use CAN0_N2_NT_CCR.
*/
#define CAN0_NTCCR2 (CAN0_N2_NT_CCR)

/** \brief 924, Node 2 Timer A Transmit Trigger Register */
#define CAN0_N2_NT_ATTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_ATTR*)0xF0208924u)
/** Alias (User Manual Name) for CAN0_N2_NT_ATTR.
* To use register names with standard convension, please use CAN0_N2_NT_ATTR.
*/
#define CAN0_NTATTR2 (CAN0_N2_NT_ATTR)

/** \brief 928, Node 2 Timer B Transmit Trigger Register */
#define CAN0_N2_NT_BTTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_BTTR*)0xF0208928u)
/** Alias (User Manual Name) for CAN0_N2_NT_BTTR.
* To use register names with standard convension, please use CAN0_N2_NT_BTTR.
*/
#define CAN0_NTBTTR2 (CAN0_N2_NT_BTTR)

/** \brief 92C, Node 2 Timer C Transmit Trigger Register */
#define CAN0_N2_NT_CTTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_CTTR*)0xF020892Cu)
/** Alias (User Manual Name) for CAN0_N2_NT_CTTR.
* To use register names with standard convension, please use CAN0_N2_NT_CTTR.
*/
#define CAN0_NTCTTR2 (CAN0_N2_NT_CTTR)

/** \brief 930, Node 2 Timer Receive Timeout Register */
#define CAN0_N2_NT_RTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_RTR*)0xF0208930u)
/** Alias (User Manual Name) for CAN0_N2_NT_RTR.
* To use register names with standard convension, please use CAN0_N2_NT_RTR.
*/
#define CAN0_NTRTR2 (CAN0_N2_NT_RTR)

/** \brief 940, Node 2 Port Control Register */
#define CAN0_N2_NPCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NPCR*)0xF0208940u)
/** Alias (User Manual Name) for CAN0_N2_NPCR.
* To use register names with standard convension, please use CAN0_N2_NPCR.
*/
#define CAN0_NPCR2 (CAN0_N2_NPCR)

/** \brief A00, Core Release Register 2 */
#define CAN0_N2_CREL /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_CREL*)0xF0208A00u)
/** Alias (User Manual Name) for CAN0_N2_CREL.
* To use register names with standard convension, please use CAN0_N2_CREL.
*/
#define CAN0_CREL2 (CAN0_N2_CREL)

/** \brief A04, Endian Register 2 */
#define CAN0_N2_ENDN /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ENDN*)0xF0208A04u)
/** Alias (User Manual Name) for CAN0_N2_ENDN.
* To use register names with standard convension, please use CAN0_N2_ENDN.
*/
#define CAN0_ENDN2 (CAN0_N2_ENDN)

/** \brief A0C, Data Bit Timing & Prescaler Register 2 */
#define CAN0_N2_DBTP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_DBTP*)0xF0208A0Cu)
/** Alias (User Manual Name) for CAN0_N2_DBTP.
* To use register names with standard convension, please use CAN0_N2_DBTP.
*/
#define CAN0_DBTP2 (CAN0_N2_DBTP)

/** \brief A10, Test Register 2 */
#define CAN0_N2_TEST /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TEST*)0xF0208A10u)
/** Alias (User Manual Name) for CAN0_N2_TEST.
* To use register names with standard convension, please use CAN0_N2_TEST.
*/
#define CAN0_TEST2 (CAN0_N2_TEST)

/** \brief A14, RAM Watchdog 2 */
#define CAN0_N2_RWD /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RWD*)0xF0208A14u)
/** Alias (User Manual Name) for CAN0_N2_RWD.
* To use register names with standard convension, please use CAN0_N2_RWD.
*/
#define CAN0_RWD2 (CAN0_N2_RWD)

/** \brief A18, CC Control Register 2 */
#define CAN0_N2_CCCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_CCCR*)0xF0208A18u)
/** Alias (User Manual Name) for CAN0_N2_CCCR.
* To use register names with standard convension, please use CAN0_N2_CCCR.
*/
#define CAN0_CCCR2 (CAN0_N2_CCCR)

/** \brief A1C, Nominal Bit Timing & Prescaler Register 2 */
#define CAN0_N2_NBTP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NBTP*)0xF0208A1Cu)
/** Alias (User Manual Name) for CAN0_N2_NBTP.
* To use register names with standard convension, please use CAN0_N2_NBTP.
*/
#define CAN0_NBTP2 (CAN0_N2_NBTP)

/** \brief A20, Timestamp Counter Configuration 2 */
#define CAN0_N2_TSCC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TSCC*)0xF0208A20u)
/** Alias (User Manual Name) for CAN0_N2_TSCC.
* To use register names with standard convension, please use CAN0_N2_TSCC.
*/
#define CAN0_TSCC2 (CAN0_N2_TSCC)

/** \brief A24, Timestamp Counter Value 2 */
#define CAN0_N2_TSCV /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TSCV*)0xF0208A24u)
/** Alias (User Manual Name) for CAN0_N2_TSCV.
* To use register names with standard convension, please use CAN0_N2_TSCV.
*/
#define CAN0_TSCV2 (CAN0_N2_TSCV)

/** \brief A28, Timeout Counter Configuration 2 */
#define CAN0_N2_TOCC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TOCC*)0xF0208A28u)
/** Alias (User Manual Name) for CAN0_N2_TOCC.
* To use register names with standard convension, please use CAN0_N2_TOCC.
*/
#define CAN0_TOCC2 (CAN0_N2_TOCC)

/** \brief A2C, Timeout Counter Value 2 */
#define CAN0_N2_TOCV /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TOCV*)0xF0208A2Cu)
/** Alias (User Manual Name) for CAN0_N2_TOCV.
* To use register names with standard convension, please use CAN0_N2_TOCV.
*/
#define CAN0_TOCV2 (CAN0_N2_TOCV)

/** \brief A40, Error Counter Register 2 */
#define CAN0_N2_ECR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ECR*)0xF0208A40u)
/** Alias (User Manual Name) for CAN0_N2_ECR.
* To use register names with standard convension, please use CAN0_N2_ECR.
*/
#define CAN0_ECR2 (CAN0_N2_ECR)

/** \brief A44, Protocol Status Register 2 */
#define CAN0_N2_PSR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_PSR*)0xF0208A44u)
/** Alias (User Manual Name) for CAN0_N2_PSR.
* To use register names with standard convension, please use CAN0_N2_PSR.
*/
#define CAN0_PSR2 (CAN0_N2_PSR)

/** \brief A48, Transmitter Delay Compensation Register 2 */
#define CAN0_N2_TDCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TDCR*)0xF0208A48u)
/** Alias (User Manual Name) for CAN0_N2_TDCR.
* To use register names with standard convension, please use CAN0_N2_TDCR.
*/
#define CAN0_TDCR2 (CAN0_N2_TDCR)

/** \brief A50, Interrupt Register 2 */
#define CAN0_N2_IR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_IR*)0xF0208A50u)
/** Alias (User Manual Name) for CAN0_N2_IR.
* To use register names with standard convension, please use CAN0_N2_IR.
*/
#define CAN0_IR2 (CAN0_N2_IR)

/** \brief A54, Interrupt Enable 2 */
#define CAN0_N2_IE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_IE*)0xF0208A54u)
/** Alias (User Manual Name) for CAN0_N2_IE.
* To use register names with standard convension, please use CAN0_N2_IE.
*/
#define CAN0_IE2 (CAN0_N2_IE)

/** \brief A80, Global Filter Configuration 2 */
#define CAN0_N2_GFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GFC*)0xF0208A80u)
/** Alias (User Manual Name) for CAN0_N2_GFC.
* To use register names with standard convension, please use CAN0_N2_GFC.
*/
#define CAN0_GFC2 (CAN0_N2_GFC)

/** \brief A84, Standard ID Filter Configuration 2 */
#define CAN0_N2_SIDFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_SIDFC*)0xF0208A84u)
/** Alias (User Manual Name) for CAN0_N2_SIDFC.
* To use register names with standard convension, please use CAN0_N2_SIDFC.
*/
#define CAN0_SIDFC2 (CAN0_N2_SIDFC)

/** \brief A88, Extended ID Filter Configuration 2 */
#define CAN0_N2_XIDFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_XIDFC*)0xF0208A88u)
/** Alias (User Manual Name) for CAN0_N2_XIDFC.
* To use register names with standard convension, please use CAN0_N2_XIDFC.
*/
#define CAN0_XIDFC2 (CAN0_N2_XIDFC)

/** \brief A90, Extended ID AND Mask 2 */
#define CAN0_N2_XIDAM /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_XIDAM*)0xF0208A90u)
/** Alias (User Manual Name) for CAN0_N2_XIDAM.
* To use register names with standard convension, please use CAN0_N2_XIDAM.
*/
#define CAN0_XIDAM2 (CAN0_N2_XIDAM)

/** \brief A94, High Priority Message Status 2 */
#define CAN0_N2_HPMS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_HPMS*)0xF0208A94u)
/** Alias (User Manual Name) for CAN0_N2_HPMS.
* To use register names with standard convension, please use CAN0_N2_HPMS.
*/
#define CAN0_HPMS2 (CAN0_N2_HPMS)

/** \brief A98, New Data 1 2 */
#define CAN0_N2_NDAT1 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NDAT1*)0xF0208A98u)
/** Alias (User Manual Name) for CAN0_N2_NDAT1.
* To use register names with standard convension, please use CAN0_N2_NDAT1.
*/
#define CAN0_NDAT12 (CAN0_N2_NDAT1)

/** \brief A9C, New Data 2 2 */
#define CAN0_N2_NDAT2 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NDAT2*)0xF0208A9Cu)
/** Alias (User Manual Name) for CAN0_N2_NDAT2.
* To use register names with standard convension, please use CAN0_N2_NDAT2.
*/
#define CAN0_NDAT22 (CAN0_N2_NDAT2)

/** \brief AA0, Rx FIFO 0 Configuration 2 */
#define CAN0_N2_RX_F0C /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0C*)0xF0208AA0u)
/** Alias (User Manual Name) for CAN0_N2_RX_F0C.
* To use register names with standard convension, please use CAN0_N2_RX_F0C.
*/
#define CAN0_RXF0C2 (CAN0_N2_RX_F0C)

/** \brief AA4, Rx FIFO 0 Status 2 */
#define CAN0_N2_RX_F0S /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0S*)0xF0208AA4u)
/** Alias (User Manual Name) for CAN0_N2_RX_F0S.
* To use register names with standard convension, please use CAN0_N2_RX_F0S.
*/
#define CAN0_RXF0S2 (CAN0_N2_RX_F0S)

/** \brief AA8, Rx FIFO 0 Acknowledge 2 */
#define CAN0_N2_RX_F0A /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0A*)0xF0208AA8u)
/** Alias (User Manual Name) for CAN0_N2_RX_F0A.
* To use register names with standard convension, please use CAN0_N2_RX_F0A.
*/
#define CAN0_RXF0A2 (CAN0_N2_RX_F0A)

/** \brief AAC, Rx Buffer Configuration 2 */
#define CAN0_N2_RX_BC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_BC*)0xF0208AACu)
/** Alias (User Manual Name) for CAN0_N2_RX_BC.
* To use register names with standard convension, please use CAN0_N2_RX_BC.
*/
#define CAN0_RXBC2 (CAN0_N2_RX_BC)

/** \brief AB0, Rx FIFO 1 Configuration 2 */
#define CAN0_N2_RX_F1C /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1C*)0xF0208AB0u)
/** Alias (User Manual Name) for CAN0_N2_RX_F1C.
* To use register names with standard convension, please use CAN0_N2_RX_F1C.
*/
#define CAN0_RXF1C2 (CAN0_N2_RX_F1C)

/** \brief AB4, Rx FIFO 1 Status 2 */
#define CAN0_N2_RX_F1S /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1S*)0xF0208AB4u)
/** Alias (User Manual Name) for CAN0_N2_RX_F1S.
* To use register names with standard convension, please use CAN0_N2_RX_F1S.
*/
#define CAN0_RXF1S2 (CAN0_N2_RX_F1S)

/** \brief AB8, Rx FIFO 1 Acknowledge 2 */
#define CAN0_N2_RX_F1A /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1A*)0xF0208AB8u)
/** Alias (User Manual Name) for CAN0_N2_RX_F1A.
* To use register names with standard convension, please use CAN0_N2_RX_F1A.
*/
#define CAN0_RXF1A2 (CAN0_N2_RX_F1A)

/** \brief ABC, Rx Buffer/FIFO Element Size Configuration 2 */
#define CAN0_N2_RX_ESC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_ESC*)0xF0208ABCu)
/** Alias (User Manual Name) for CAN0_N2_RX_ESC.
* To use register names with standard convension, please use CAN0_N2_RX_ESC.
*/
#define CAN0_RXESC2 (CAN0_N2_RX_ESC)

/** \brief AC0, Tx Buffer Configuration 2 */
#define CAN0_N2_TX_BC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BC*)0xF0208AC0u)
/** Alias (User Manual Name) for CAN0_N2_TX_BC.
* To use register names with standard convension, please use CAN0_N2_TX_BC.
*/
#define CAN0_TXBC2 (CAN0_N2_TX_BC)

/** \brief AC4, Tx FIFO/Queue Status 2 */
#define CAN0_N2_TX_FQS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_FQS*)0xF0208AC4u)
/** Alias (User Manual Name) for CAN0_N2_TX_FQS.
* To use register names with standard convension, please use CAN0_N2_TX_FQS.
*/
#define CAN0_TXFQS2 (CAN0_N2_TX_FQS)

/** \brief AC8, Tx Buffer Element Size Configuration 2 */
#define CAN0_N2_TX_ESC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_ESC*)0xF0208AC8u)
/** Alias (User Manual Name) for CAN0_N2_TX_ESC.
* To use register names with standard convension, please use CAN0_N2_TX_ESC.
*/
#define CAN0_TXESC2 (CAN0_N2_TX_ESC)

/** \brief ACC, Tx Buffer Request Pending 2 */
#define CAN0_N2_TX_BRP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BRP*)0xF0208ACCu)
/** Alias (User Manual Name) for CAN0_N2_TX_BRP.
* To use register names with standard convension, please use CAN0_N2_TX_BRP.
*/
#define CAN0_TXBRP2 (CAN0_N2_TX_BRP)

/** \brief AD0, Tx Buffer Add Request 2 */
#define CAN0_N2_TX_BAR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BAR*)0xF0208AD0u)
/** Alias (User Manual Name) for CAN0_N2_TX_BAR.
* To use register names with standard convension, please use CAN0_N2_TX_BAR.
*/
#define CAN0_TXBAR2 (CAN0_N2_TX_BAR)

/** \brief AD4, Tx Buffer Cancellation Request 2 */
#define CAN0_N2_TX_BCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCR*)0xF0208AD4u)
/** Alias (User Manual Name) for CAN0_N2_TX_BCR.
* To use register names with standard convension, please use CAN0_N2_TX_BCR.
*/
#define CAN0_TXBCR2 (CAN0_N2_TX_BCR)

/** \brief AD8, Tx Buffer Transmission Occurred 2 */
#define CAN0_N2_TX_BTO /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BTO*)0xF0208AD8u)
/** Alias (User Manual Name) for CAN0_N2_TX_BTO.
* To use register names with standard convension, please use CAN0_N2_TX_BTO.
*/
#define CAN0_TXBTO2 (CAN0_N2_TX_BTO)

/** \brief ADC, Tx Buffer Cancellation Finished 2 */
#define CAN0_N2_TX_BCF /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCF*)0xF0208ADCu)
/** Alias (User Manual Name) for CAN0_N2_TX_BCF.
* To use register names with standard convension, please use CAN0_N2_TX_BCF.
*/
#define CAN0_TXBCF2 (CAN0_N2_TX_BCF)

/** \brief AE0, Tx Buffer Transmission Interrupt Enable 2 */
#define CAN0_N2_TX_BTIE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BTIE*)0xF0208AE0u)
/** Alias (User Manual Name) for CAN0_N2_TX_BTIE.
* To use register names with standard convension, please use CAN0_N2_TX_BTIE.
*/
#define CAN0_TXBTIE2 (CAN0_N2_TX_BTIE)

/** \brief AE4, Tx Buffer Cancellation Finished Interrupt Enable 2 */
#define CAN0_N2_TX_BCIE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCIE*)0xF0208AE4u)
/** Alias (User Manual Name) for CAN0_N2_TX_BCIE.
* To use register names with standard convension, please use CAN0_N2_TX_BCIE.
*/
#define CAN0_TXBCIE2 (CAN0_N2_TX_BCIE)

/** \brief AF0, Tx Event FIFO Configuration 2 */
#define CAN0_N2_TX_EFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFC*)0xF0208AF0u)
/** Alias (User Manual Name) for CAN0_N2_TX_EFC.
* To use register names with standard convension, please use CAN0_N2_TX_EFC.
*/
#define CAN0_TXEFC2 (CAN0_N2_TX_EFC)

/** \brief AF4, Tx Event FIFO Status 2 */
#define CAN0_N2_TX_EFS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFS*)0xF0208AF4u)
/** Alias (User Manual Name) for CAN0_N2_TX_EFS.
* To use register names with standard convension, please use CAN0_N2_TX_EFS.
*/
#define CAN0_TXEFS2 (CAN0_N2_TX_EFS)

/** \brief AF8, Tx Event FIFO Acknowledge 2 */
#define CAN0_N2_TX_EFA /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFA*)0xF0208AF8u)
/** Alias (User Manual Name) for CAN0_N2_TX_EFA.
* To use register names with standard convension, please use CAN0_N2_TX_EFA.
*/
#define CAN0_TXEFA2 (CAN0_N2_TX_EFA)

/** \brief D00, Access Enable Register CAN Node 3 0 */
#define CAN0_N3_ACCENNODE0 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ACCENNODE0*)0xF0208D00u)
/** Alias (User Manual Name) for CAN0_N3_ACCENNODE0.
* To use register names with standard convension, please use CAN0_N3_ACCENNODE0.
*/
#define CAN0_ACCENNODE30 (CAN0_N3_ACCENNODE0)

/** \brief D08, Start Address Node 3 */
#define CAN0_N3_STARTADR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_STARTADR*)0xF0208D08u)
/** Alias (User Manual Name) for CAN0_N3_STARTADR.
* To use register names with standard convension, please use CAN0_N3_STARTADR.
*/
#define CAN0_STARTADR3 (CAN0_N3_STARTADR)

/** \brief D0C, End Address Node 3 */
#define CAN0_N3_ENDADR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ENDADR*)0xF0208D0Cu)
/** Alias (User Manual Name) for CAN0_N3_ENDADR.
* To use register names with standard convension, please use CAN0_N3_ENDADR.
*/
#define CAN0_ENDADR3 (CAN0_N3_ENDADR)

/** \brief D10, Interrupt Signalling Register 3 */
#define CAN0_N3_ISREG /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ISREG*)0xF0208D10u)
/** Alias (User Manual Name) for CAN0_N3_ISREG.
* To use register names with standard convension, please use CAN0_N3_ISREG.
*/
#define CAN0_ISREG3 (CAN0_N3_ISREG)

/** \brief D14, Interrupt routing for Groups 1 3 */
#define CAN0_N3_GRINT1 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GRINT1*)0xF0208D14u)
/** Alias (User Manual Name) for CAN0_N3_GRINT1.
* To use register names with standard convension, please use CAN0_N3_GRINT1.
*/
#define CAN0_GRINT13 (CAN0_N3_GRINT1)

/** \brief D18, Interrupt routing for Groups 2 3 */
#define CAN0_N3_GRINT2 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GRINT2*)0xF0208D18u)
/** Alias (User Manual Name) for CAN0_N3_GRINT2.
* To use register names with standard convension, please use CAN0_N3_GRINT2.
*/
#define CAN0_GRINT23 (CAN0_N3_GRINT2)

/** \brief D20, Node 3 Timer Clock Control Register */
#define CAN0_N3_NT_CCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_CCR*)0xF0208D20u)
/** Alias (User Manual Name) for CAN0_N3_NT_CCR.
* To use register names with standard convension, please use CAN0_N3_NT_CCR.
*/
#define CAN0_NTCCR3 (CAN0_N3_NT_CCR)

/** \brief D24, Node 3 Timer A Transmit Trigger Register */
#define CAN0_N3_NT_ATTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_ATTR*)0xF0208D24u)
/** Alias (User Manual Name) for CAN0_N3_NT_ATTR.
* To use register names with standard convension, please use CAN0_N3_NT_ATTR.
*/
#define CAN0_NTATTR3 (CAN0_N3_NT_ATTR)

/** \brief D28, Node 3 Timer B Transmit Trigger Register */
#define CAN0_N3_NT_BTTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_BTTR*)0xF0208D28u)
/** Alias (User Manual Name) for CAN0_N3_NT_BTTR.
* To use register names with standard convension, please use CAN0_N3_NT_BTTR.
*/
#define CAN0_NTBTTR3 (CAN0_N3_NT_BTTR)

/** \brief D2C, Node 3 Timer C Transmit Trigger Register */
#define CAN0_N3_NT_CTTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_CTTR*)0xF0208D2Cu)
/** Alias (User Manual Name) for CAN0_N3_NT_CTTR.
* To use register names with standard convension, please use CAN0_N3_NT_CTTR.
*/
#define CAN0_NTCTTR3 (CAN0_N3_NT_CTTR)

/** \brief D30, Node 3 Timer Receive Timeout Register */
#define CAN0_N3_NT_RTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_RTR*)0xF0208D30u)
/** Alias (User Manual Name) for CAN0_N3_NT_RTR.
* To use register names with standard convension, please use CAN0_N3_NT_RTR.
*/
#define CAN0_NTRTR3 (CAN0_N3_NT_RTR)

/** \brief D40, Node 3 Port Control Register */
#define CAN0_N3_NPCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NPCR*)0xF0208D40u)
/** Alias (User Manual Name) for CAN0_N3_NPCR.
* To use register names with standard convension, please use CAN0_N3_NPCR.
*/
#define CAN0_NPCR3 (CAN0_N3_NPCR)

/** \brief E00, Core Release Register 3 */
#define CAN0_N3_CREL /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_CREL*)0xF0208E00u)
/** Alias (User Manual Name) for CAN0_N3_CREL.
* To use register names with standard convension, please use CAN0_N3_CREL.
*/
#define CAN0_CREL3 (CAN0_N3_CREL)

/** \brief E04, Endian Register 3 */
#define CAN0_N3_ENDN /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ENDN*)0xF0208E04u)
/** Alias (User Manual Name) for CAN0_N3_ENDN.
* To use register names with standard convension, please use CAN0_N3_ENDN.
*/
#define CAN0_ENDN3 (CAN0_N3_ENDN)

/** \brief E0C, Data Bit Timing & Prescaler Register 3 */
#define CAN0_N3_DBTP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_DBTP*)0xF0208E0Cu)
/** Alias (User Manual Name) for CAN0_N3_DBTP.
* To use register names with standard convension, please use CAN0_N3_DBTP.
*/
#define CAN0_DBTP3 (CAN0_N3_DBTP)

/** \brief E10, Test Register 3 */
#define CAN0_N3_TEST /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TEST*)0xF0208E10u)
/** Alias (User Manual Name) for CAN0_N3_TEST.
* To use register names with standard convension, please use CAN0_N3_TEST.
*/
#define CAN0_TEST3 (CAN0_N3_TEST)

/** \brief E14, RAM Watchdog 3 */
#define CAN0_N3_RWD /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RWD*)0xF0208E14u)
/** Alias (User Manual Name) for CAN0_N3_RWD.
* To use register names with standard convension, please use CAN0_N3_RWD.
*/
#define CAN0_RWD3 (CAN0_N3_RWD)

/** \brief E18, CC Control Register 3 */
#define CAN0_N3_CCCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_CCCR*)0xF0208E18u)
/** Alias (User Manual Name) for CAN0_N3_CCCR.
* To use register names with standard convension, please use CAN0_N3_CCCR.
*/
#define CAN0_CCCR3 (CAN0_N3_CCCR)

/** \brief E1C, Nominal Bit Timing & Prescaler Register 3 */
#define CAN0_N3_NBTP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NBTP*)0xF0208E1Cu)
/** Alias (User Manual Name) for CAN0_N3_NBTP.
* To use register names with standard convension, please use CAN0_N3_NBTP.
*/
#define CAN0_NBTP3 (CAN0_N3_NBTP)

/** \brief E20, Timestamp Counter Configuration 3 */
#define CAN0_N3_TSCC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TSCC*)0xF0208E20u)
/** Alias (User Manual Name) for CAN0_N3_TSCC.
* To use register names with standard convension, please use CAN0_N3_TSCC.
*/
#define CAN0_TSCC3 (CAN0_N3_TSCC)

/** \brief E24, Timestamp Counter Value 3 */
#define CAN0_N3_TSCV /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TSCV*)0xF0208E24u)
/** Alias (User Manual Name) for CAN0_N3_TSCV.
* To use register names with standard convension, please use CAN0_N3_TSCV.
*/
#define CAN0_TSCV3 (CAN0_N3_TSCV)

/** \brief E28, Timeout Counter Configuration 3 */
#define CAN0_N3_TOCC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TOCC*)0xF0208E28u)
/** Alias (User Manual Name) for CAN0_N3_TOCC.
* To use register names with standard convension, please use CAN0_N3_TOCC.
*/
#define CAN0_TOCC3 (CAN0_N3_TOCC)

/** \brief E2C, Timeout Counter Value 3 */
#define CAN0_N3_TOCV /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TOCV*)0xF0208E2Cu)
/** Alias (User Manual Name) for CAN0_N3_TOCV.
* To use register names with standard convension, please use CAN0_N3_TOCV.
*/
#define CAN0_TOCV3 (CAN0_N3_TOCV)

/** \brief E40, Error Counter Register 3 */
#define CAN0_N3_ECR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ECR*)0xF0208E40u)
/** Alias (User Manual Name) for CAN0_N3_ECR.
* To use register names with standard convension, please use CAN0_N3_ECR.
*/
#define CAN0_ECR3 (CAN0_N3_ECR)

/** \brief E44, Protocol Status Register 3 */
#define CAN0_N3_PSR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_PSR*)0xF0208E44u)
/** Alias (User Manual Name) for CAN0_N3_PSR.
* To use register names with standard convension, please use CAN0_N3_PSR.
*/
#define CAN0_PSR3 (CAN0_N3_PSR)

/** \brief E48, Transmitter Delay Compensation Register 3 */
#define CAN0_N3_TDCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TDCR*)0xF0208E48u)
/** Alias (User Manual Name) for CAN0_N3_TDCR.
* To use register names with standard convension, please use CAN0_N3_TDCR.
*/
#define CAN0_TDCR3 (CAN0_N3_TDCR)

/** \brief E50, Interrupt Register 3 */
#define CAN0_N3_IR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_IR*)0xF0208E50u)
/** Alias (User Manual Name) for CAN0_N3_IR.
* To use register names with standard convension, please use CAN0_N3_IR.
*/
#define CAN0_IR3 (CAN0_N3_IR)

/** \brief E54, Interrupt Enable 3 */
#define CAN0_N3_IE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_IE*)0xF0208E54u)
/** Alias (User Manual Name) for CAN0_N3_IE.
* To use register names with standard convension, please use CAN0_N3_IE.
*/
#define CAN0_IE3 (CAN0_N3_IE)

/** \brief E80, Global Filter Configuration 3 */
#define CAN0_N3_GFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GFC*)0xF0208E80u)
/** Alias (User Manual Name) for CAN0_N3_GFC.
* To use register names with standard convension, please use CAN0_N3_GFC.
*/
#define CAN0_GFC3 (CAN0_N3_GFC)

/** \brief E84, Standard ID Filter Configuration 3 */
#define CAN0_N3_SIDFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_SIDFC*)0xF0208E84u)
/** Alias (User Manual Name) for CAN0_N3_SIDFC.
* To use register names with standard convension, please use CAN0_N3_SIDFC.
*/
#define CAN0_SIDFC3 (CAN0_N3_SIDFC)

/** \brief E88, Extended ID Filter Configuration 3 */
#define CAN0_N3_XIDFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_XIDFC*)0xF0208E88u)
/** Alias (User Manual Name) for CAN0_N3_XIDFC.
* To use register names with standard convension, please use CAN0_N3_XIDFC.
*/
#define CAN0_XIDFC3 (CAN0_N3_XIDFC)

/** \brief E90, Extended ID AND Mask 3 */
#define CAN0_N3_XIDAM /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_XIDAM*)0xF0208E90u)
/** Alias (User Manual Name) for CAN0_N3_XIDAM.
* To use register names with standard convension, please use CAN0_N3_XIDAM.
*/
#define CAN0_XIDAM3 (CAN0_N3_XIDAM)

/** \brief E94, High Priority Message Status 3 */
#define CAN0_N3_HPMS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_HPMS*)0xF0208E94u)
/** Alias (User Manual Name) for CAN0_N3_HPMS.
* To use register names with standard convension, please use CAN0_N3_HPMS.
*/
#define CAN0_HPMS3 (CAN0_N3_HPMS)

/** \brief E98, New Data 1 3 */
#define CAN0_N3_NDAT1 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NDAT1*)0xF0208E98u)
/** Alias (User Manual Name) for CAN0_N3_NDAT1.
* To use register names with standard convension, please use CAN0_N3_NDAT1.
*/
#define CAN0_NDAT13 (CAN0_N3_NDAT1)

/** \brief E9C, New Data 2 3 */
#define CAN0_N3_NDAT2 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NDAT2*)0xF0208E9Cu)
/** Alias (User Manual Name) for CAN0_N3_NDAT2.
* To use register names with standard convension, please use CAN0_N3_NDAT2.
*/
#define CAN0_NDAT23 (CAN0_N3_NDAT2)

/** \brief EA0, Rx FIFO 0 Configuration 3 */
#define CAN0_N3_RX_F0C /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0C*)0xF0208EA0u)
/** Alias (User Manual Name) for CAN0_N3_RX_F0C.
* To use register names with standard convension, please use CAN0_N3_RX_F0C.
*/
#define CAN0_RXF0C3 (CAN0_N3_RX_F0C)

/** \brief EA4, Rx FIFO 0 Status 3 */
#define CAN0_N3_RX_F0S /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0S*)0xF0208EA4u)
/** Alias (User Manual Name) for CAN0_N3_RX_F0S.
* To use register names with standard convension, please use CAN0_N3_RX_F0S.
*/
#define CAN0_RXF0S3 (CAN0_N3_RX_F0S)

/** \brief EA8, Rx FIFO 0 Acknowledge 3 */
#define CAN0_N3_RX_F0A /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0A*)0xF0208EA8u)
/** Alias (User Manual Name) for CAN0_N3_RX_F0A.
* To use register names with standard convension, please use CAN0_N3_RX_F0A.
*/
#define CAN0_RXF0A3 (CAN0_N3_RX_F0A)

/** \brief EAC, Rx Buffer Configuration 3 */
#define CAN0_N3_RX_BC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_BC*)0xF0208EACu)
/** Alias (User Manual Name) for CAN0_N3_RX_BC.
* To use register names with standard convension, please use CAN0_N3_RX_BC.
*/
#define CAN0_RXBC3 (CAN0_N3_RX_BC)

/** \brief EB0, Rx FIFO 1 Configuration 3 */
#define CAN0_N3_RX_F1C /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1C*)0xF0208EB0u)
/** Alias (User Manual Name) for CAN0_N3_RX_F1C.
* To use register names with standard convension, please use CAN0_N3_RX_F1C.
*/
#define CAN0_RXF1C3 (CAN0_N3_RX_F1C)

/** \brief EB4, Rx FIFO 1 Status 3 */
#define CAN0_N3_RX_F1S /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1S*)0xF0208EB4u)
/** Alias (User Manual Name) for CAN0_N3_RX_F1S.
* To use register names with standard convension, please use CAN0_N3_RX_F1S.
*/
#define CAN0_RXF1S3 (CAN0_N3_RX_F1S)

/** \brief EB8, Rx FIFO 1 Acknowledge 3 */
#define CAN0_N3_RX_F1A /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1A*)0xF0208EB8u)
/** Alias (User Manual Name) for CAN0_N3_RX_F1A.
* To use register names with standard convension, please use CAN0_N3_RX_F1A.
*/
#define CAN0_RXF1A3 (CAN0_N3_RX_F1A)

/** \brief EBC, Rx Buffer/FIFO Element Size Configuration 3 */
#define CAN0_N3_RX_ESC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_ESC*)0xF0208EBCu)
/** Alias (User Manual Name) for CAN0_N3_RX_ESC.
* To use register names with standard convension, please use CAN0_N3_RX_ESC.
*/
#define CAN0_RXESC3 (CAN0_N3_RX_ESC)

/** \brief EC0, Tx Buffer Configuration 3 */
#define CAN0_N3_TX_BC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BC*)0xF0208EC0u)
/** Alias (User Manual Name) for CAN0_N3_TX_BC.
* To use register names with standard convension, please use CAN0_N3_TX_BC.
*/
#define CAN0_TXBC3 (CAN0_N3_TX_BC)

/** \brief EC4, Tx FIFO/Queue Status 3 */
#define CAN0_N3_TX_FQS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_FQS*)0xF0208EC4u)
/** Alias (User Manual Name) for CAN0_N3_TX_FQS.
* To use register names with standard convension, please use CAN0_N3_TX_FQS.
*/
#define CAN0_TXFQS3 (CAN0_N3_TX_FQS)

/** \brief EC8, Tx Buffer Element Size Configuration 3 */
#define CAN0_N3_TX_ESC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_ESC*)0xF0208EC8u)
/** Alias (User Manual Name) for CAN0_N3_TX_ESC.
* To use register names with standard convension, please use CAN0_N3_TX_ESC.
*/
#define CAN0_TXESC3 (CAN0_N3_TX_ESC)

/** \brief ECC, Tx Buffer Request Pending 3 */
#define CAN0_N3_TX_BRP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BRP*)0xF0208ECCu)
/** Alias (User Manual Name) for CAN0_N3_TX_BRP.
* To use register names with standard convension, please use CAN0_N3_TX_BRP.
*/
#define CAN0_TXBRP3 (CAN0_N3_TX_BRP)

/** \brief ED0, Tx Buffer Add Request 3 */
#define CAN0_N3_TX_BAR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BAR*)0xF0208ED0u)
/** Alias (User Manual Name) for CAN0_N3_TX_BAR.
* To use register names with standard convension, please use CAN0_N3_TX_BAR.
*/
#define CAN0_TXBAR3 (CAN0_N3_TX_BAR)

/** \brief ED4, Tx Buffer Cancellation Request 3 */
#define CAN0_N3_TX_BCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCR*)0xF0208ED4u)
/** Alias (User Manual Name) for CAN0_N3_TX_BCR.
* To use register names with standard convension, please use CAN0_N3_TX_BCR.
*/
#define CAN0_TXBCR3 (CAN0_N3_TX_BCR)

/** \brief ED8, Tx Buffer Transmission Occurred 3 */
#define CAN0_N3_TX_BTO /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BTO*)0xF0208ED8u)
/** Alias (User Manual Name) for CAN0_N3_TX_BTO.
* To use register names with standard convension, please use CAN0_N3_TX_BTO.
*/
#define CAN0_TXBTO3 (CAN0_N3_TX_BTO)

/** \brief EDC, Tx Buffer Cancellation Finished 3 */
#define CAN0_N3_TX_BCF /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCF*)0xF0208EDCu)
/** Alias (User Manual Name) for CAN0_N3_TX_BCF.
* To use register names with standard convension, please use CAN0_N3_TX_BCF.
*/
#define CAN0_TXBCF3 (CAN0_N3_TX_BCF)

/** \brief EE0, Tx Buffer Transmission Interrupt Enable 3 */
#define CAN0_N3_TX_BTIE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BTIE*)0xF0208EE0u)
/** Alias (User Manual Name) for CAN0_N3_TX_BTIE.
* To use register names with standard convension, please use CAN0_N3_TX_BTIE.
*/
#define CAN0_TXBTIE3 (CAN0_N3_TX_BTIE)

/** \brief EE4, Tx Buffer Cancellation Finished Interrupt Enable 3 */
#define CAN0_N3_TX_BCIE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCIE*)0xF0208EE4u)
/** Alias (User Manual Name) for CAN0_N3_TX_BCIE.
* To use register names with standard convension, please use CAN0_N3_TX_BCIE.
*/
#define CAN0_TXBCIE3 (CAN0_N3_TX_BCIE)

/** \brief EF0, Tx Event FIFO Configuration 3 */
#define CAN0_N3_TX_EFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFC*)0xF0208EF0u)
/** Alias (User Manual Name) for CAN0_N3_TX_EFC.
* To use register names with standard convension, please use CAN0_N3_TX_EFC.
*/
#define CAN0_TXEFC3 (CAN0_N3_TX_EFC)

/** \brief EF4, Tx Event FIFO Status 3 */
#define CAN0_N3_TX_EFS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFS*)0xF0208EF4u)
/** Alias (User Manual Name) for CAN0_N3_TX_EFS.
* To use register names with standard convension, please use CAN0_N3_TX_EFS.
*/
#define CAN0_TXEFS3 (CAN0_N3_TX_EFS)

/** \brief EF8, Tx Event FIFO Acknowledge 3 */
#define CAN0_N3_TX_EFA /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFA*)0xF0208EF8u)
/** Alias (User Manual Name) for CAN0_N3_TX_EFA.
* To use register names with standard convension, please use CAN0_N3_TX_EFA.
*/
#define CAN0_TXEFA3 (CAN0_N3_TX_EFA)

/******************************************************************************/
/******************************************************************************/
/** \addtogroup IfxSfr_Can_Registers_Cfg_Can1
 * \{  */
/** \brief 0, Embedded SRAM for messages */
#define CAN1_RAM ((void*)0xF0210000u)
#define CAN1_RAM_SIZE (0x4000u)
/** \brief 0, CAN Clock Control Register */
#define CAN1_CLC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_CLC*)0xF0218000u)

/** \brief 8, Module Identification Register */
#define CAN1_ID /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_ID*)0xF0218008u)

/** \brief 30, Module Control Register */
#define CAN1_MCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_MCR*)0xF0218030u)

/** \brief DC, Access Enable Register Control 0 */
#define CAN1_ACCENCTR0 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_ACCENCTR0*)0xF02180DCu)

/** \brief E8, OCDS Control and Status */
#define CAN1_OCS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_OCS*)0xF02180E8u)

/** \brief EC, Kernel Reset Status Clear Register */
#define CAN1_KRSTCLR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_KRSTCLR*)0xF02180ECu)

/** \brief F0, Kernel Reset Register 1 */
#define CAN1_KRST1 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_KRST1*)0xF02180F0u)

/** \brief F4, Kernel Reset Register 0 */
#define CAN1_KRST0 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_KRST0*)0xF02180F4u)

/** \brief FC, Access Enable Register 0 */
#define CAN1_ACCEN0 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_ACCEN0*)0xF02180FCu)

/** \brief 100, Access Enable Register CAN Node 0 0 */
#define CAN1_N0_ACCENNODE0 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ACCENNODE0*)0xF0218100u)
/** Alias (User Manual Name) for CAN1_N0_ACCENNODE0.
* To use register names with standard convension, please use CAN1_N0_ACCENNODE0.
*/
#define CAN1_ACCENNODE00 (CAN1_N0_ACCENNODE0)

/** \brief 108, Start Address Node 0 */
#define CAN1_N0_STARTADR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_STARTADR*)0xF0218108u)
/** Alias (User Manual Name) for CAN1_N0_STARTADR.
* To use register names with standard convension, please use CAN1_N0_STARTADR.
*/
#define CAN1_STARTADR0 (CAN1_N0_STARTADR)

/** \brief 10C, End Address Node 0 */
#define CAN1_N0_ENDADR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ENDADR*)0xF021810Cu)
/** Alias (User Manual Name) for CAN1_N0_ENDADR.
* To use register names with standard convension, please use CAN1_N0_ENDADR.
*/
#define CAN1_ENDADR0 (CAN1_N0_ENDADR)

/** \brief 110, Interrupt Signalling Register 0 */
#define CAN1_N0_ISREG /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ISREG*)0xF0218110u)
/** Alias (User Manual Name) for CAN1_N0_ISREG.
* To use register names with standard convension, please use CAN1_N0_ISREG.
*/
#define CAN1_ISREG0 (CAN1_N0_ISREG)

/** \brief 114, Interrupt routing for Groups 1 0 */
#define CAN1_N0_GRINT1 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GRINT1*)0xF0218114u)
/** Alias (User Manual Name) for CAN1_N0_GRINT1.
* To use register names with standard convension, please use CAN1_N0_GRINT1.
*/
#define CAN1_GRINT10 (CAN1_N0_GRINT1)

/** \brief 118, Interrupt routing for Groups 2 0 */
#define CAN1_N0_GRINT2 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GRINT2*)0xF0218118u)
/** Alias (User Manual Name) for CAN1_N0_GRINT2.
* To use register names with standard convension, please use CAN1_N0_GRINT2.
*/
#define CAN1_GRINT20 (CAN1_N0_GRINT2)

/** \brief 120, Node 0 Timer Clock Control Register */
#define CAN1_N0_NT_CCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_CCR*)0xF0218120u)
/** Alias (User Manual Name) for CAN1_N0_NT_CCR.
* To use register names with standard convension, please use CAN1_N0_NT_CCR.
*/
#define CAN1_NTCCR0 (CAN1_N0_NT_CCR)

/** \brief 124, Node 0 Timer A Transmit Trigger Register */
#define CAN1_N0_NT_ATTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_ATTR*)0xF0218124u)
/** Alias (User Manual Name) for CAN1_N0_NT_ATTR.
* To use register names with standard convension, please use CAN1_N0_NT_ATTR.
*/
#define CAN1_NTATTR0 (CAN1_N0_NT_ATTR)

/** \brief 128, Node 0 Timer B Transmit Trigger Register */
#define CAN1_N0_NT_BTTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_BTTR*)0xF0218128u)
/** Alias (User Manual Name) for CAN1_N0_NT_BTTR.
* To use register names with standard convension, please use CAN1_N0_NT_BTTR.
*/
#define CAN1_NTBTTR0 (CAN1_N0_NT_BTTR)

/** \brief 12C, Node 0 Timer C Transmit Trigger Register */
#define CAN1_N0_NT_CTTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_CTTR*)0xF021812Cu)
/** Alias (User Manual Name) for CAN1_N0_NT_CTTR.
* To use register names with standard convension, please use CAN1_N0_NT_CTTR.
*/
#define CAN1_NTCTTR0 (CAN1_N0_NT_CTTR)

/** \brief 130, Node 0 Timer Receive Timeout Register */
#define CAN1_N0_NT_RTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_RTR*)0xF0218130u)
/** Alias (User Manual Name) for CAN1_N0_NT_RTR.
* To use register names with standard convension, please use CAN1_N0_NT_RTR.
*/
#define CAN1_NTRTR0 (CAN1_N0_NT_RTR)

/** \brief 140, Node 0 Port Control Register */
#define CAN1_N0_NPCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NPCR*)0xF0218140u)
/** Alias (User Manual Name) for CAN1_N0_NPCR.
* To use register names with standard convension, please use CAN1_N0_NPCR.
*/
#define CAN1_NPCR0 (CAN1_N0_NPCR)

/** \brief 200, Core Release Register 0 */
#define CAN1_N0_CREL /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_CREL*)0xF0218200u)
/** Alias (User Manual Name) for CAN1_N0_CREL.
* To use register names with standard convension, please use CAN1_N0_CREL.
*/
#define CAN1_CREL0 (CAN1_N0_CREL)

/** \brief 204, Endian Register 0 */
#define CAN1_N0_ENDN /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ENDN*)0xF0218204u)
/** Alias (User Manual Name) for CAN1_N0_ENDN.
* To use register names with standard convension, please use CAN1_N0_ENDN.
*/
#define CAN1_ENDN0 (CAN1_N0_ENDN)

/** \brief 20C, Data Bit Timing & Prescaler Register 0 */
#define CAN1_N0_DBTP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_DBTP*)0xF021820Cu)
/** Alias (User Manual Name) for CAN1_N0_DBTP.
* To use register names with standard convension, please use CAN1_N0_DBTP.
*/
#define CAN1_DBTP0 (CAN1_N0_DBTP)

/** \brief 210, Test Register 0 */
#define CAN1_N0_TEST /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TEST*)0xF0218210u)
/** Alias (User Manual Name) for CAN1_N0_TEST.
* To use register names with standard convension, please use CAN1_N0_TEST.
*/
#define CAN1_TEST0 (CAN1_N0_TEST)

/** \brief 214, RAM Watchdog 0 */
#define CAN1_N0_RWD /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RWD*)0xF0218214u)
/** Alias (User Manual Name) for CAN1_N0_RWD.
* To use register names with standard convension, please use CAN1_N0_RWD.
*/
#define CAN1_RWD0 (CAN1_N0_RWD)

/** \brief 218, CC Control Register 0 */
#define CAN1_N0_CCCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_CCCR*)0xF0218218u)
/** Alias (User Manual Name) for CAN1_N0_CCCR.
* To use register names with standard convension, please use CAN1_N0_CCCR.
*/
#define CAN1_CCCR0 (CAN1_N0_CCCR)

/** \brief 21C, Nominal Bit Timing & Prescaler Register 0 */
#define CAN1_N0_NBTP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NBTP*)0xF021821Cu)
/** Alias (User Manual Name) for CAN1_N0_NBTP.
* To use register names with standard convension, please use CAN1_N0_NBTP.
*/
#define CAN1_NBTP0 (CAN1_N0_NBTP)

/** \brief 220, Timestamp Counter Configuration 0 */
#define CAN1_N0_TSCC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TSCC*)0xF0218220u)
/** Alias (User Manual Name) for CAN1_N0_TSCC.
* To use register names with standard convension, please use CAN1_N0_TSCC.
*/
#define CAN1_TSCC0 (CAN1_N0_TSCC)

/** \brief 224, Timestamp Counter Value 0 */
#define CAN1_N0_TSCV /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TSCV*)0xF0218224u)
/** Alias (User Manual Name) for CAN1_N0_TSCV.
* To use register names with standard convension, please use CAN1_N0_TSCV.
*/
#define CAN1_TSCV0 (CAN1_N0_TSCV)

/** \brief 228, Timeout Counter Configuration 0 */
#define CAN1_N0_TOCC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TOCC*)0xF0218228u)
/** Alias (User Manual Name) for CAN1_N0_TOCC.
* To use register names with standard convension, please use CAN1_N0_TOCC.
*/
#define CAN1_TOCC0 (CAN1_N0_TOCC)

/** \brief 22C, Timeout Counter Value 0 */
#define CAN1_N0_TOCV /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TOCV*)0xF021822Cu)
/** Alias (User Manual Name) for CAN1_N0_TOCV.
* To use register names with standard convension, please use CAN1_N0_TOCV.
*/
#define CAN1_TOCV0 (CAN1_N0_TOCV)

/** \brief 240, Error Counter Register 0 */
#define CAN1_N0_ECR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ECR*)0xF0218240u)
/** Alias (User Manual Name) for CAN1_N0_ECR.
* To use register names with standard convension, please use CAN1_N0_ECR.
*/
#define CAN1_ECR0 (CAN1_N0_ECR)

/** \brief 244, Protocol Status Register 0 */
#define CAN1_N0_PSR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_PSR*)0xF0218244u)
/** Alias (User Manual Name) for CAN1_N0_PSR.
* To use register names with standard convension, please use CAN1_N0_PSR.
*/
#define CAN1_PSR0 (CAN1_N0_PSR)

/** \brief 248, Transmitter Delay Compensation Register 0 */
#define CAN1_N0_TDCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TDCR*)0xF0218248u)
/** Alias (User Manual Name) for CAN1_N0_TDCR.
* To use register names with standard convension, please use CAN1_N0_TDCR.
*/
#define CAN1_TDCR0 (CAN1_N0_TDCR)

/** \brief 250, Interrupt Register 0 */
#define CAN1_N0_IR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_IR*)0xF0218250u)
/** Alias (User Manual Name) for CAN1_N0_IR.
* To use register names with standard convension, please use CAN1_N0_IR.
*/
#define CAN1_IR0 (CAN1_N0_IR)

/** \brief 254, Interrupt Enable 0 */
#define CAN1_N0_IE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_IE*)0xF0218254u)
/** Alias (User Manual Name) for CAN1_N0_IE.
* To use register names with standard convension, please use CAN1_N0_IE.
*/
#define CAN1_IE0 (CAN1_N0_IE)

/** \brief 280, Global Filter Configuration 0 */
#define CAN1_N0_GFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GFC*)0xF0218280u)
/** Alias (User Manual Name) for CAN1_N0_GFC.
* To use register names with standard convension, please use CAN1_N0_GFC.
*/
#define CAN1_GFC0 (CAN1_N0_GFC)

/** \brief 284, Standard ID Filter Configuration 0 */
#define CAN1_N0_SIDFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_SIDFC*)0xF0218284u)
/** Alias (User Manual Name) for CAN1_N0_SIDFC.
* To use register names with standard convension, please use CAN1_N0_SIDFC.
*/
#define CAN1_SIDFC0 (CAN1_N0_SIDFC)

/** \brief 288, Extended ID Filter Configuration 0 */
#define CAN1_N0_XIDFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_XIDFC*)0xF0218288u)
/** Alias (User Manual Name) for CAN1_N0_XIDFC.
* To use register names with standard convension, please use CAN1_N0_XIDFC.
*/
#define CAN1_XIDFC0 (CAN1_N0_XIDFC)

/** \brief 290, Extended ID AND Mask 0 */
#define CAN1_N0_XIDAM /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_XIDAM*)0xF0218290u)
/** Alias (User Manual Name) for CAN1_N0_XIDAM.
* To use register names with standard convension, please use CAN1_N0_XIDAM.
*/
#define CAN1_XIDAM0 (CAN1_N0_XIDAM)

/** \brief 294, High Priority Message Status 0 */
#define CAN1_N0_HPMS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_HPMS*)0xF0218294u)
/** Alias (User Manual Name) for CAN1_N0_HPMS.
* To use register names with standard convension, please use CAN1_N0_HPMS.
*/
#define CAN1_HPMS0 (CAN1_N0_HPMS)

/** \brief 298, New Data 1 0 */
#define CAN1_N0_NDAT1 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NDAT1*)0xF0218298u)
/** Alias (User Manual Name) for CAN1_N0_NDAT1.
* To use register names with standard convension, please use CAN1_N0_NDAT1.
*/
#define CAN1_NDAT10 (CAN1_N0_NDAT1)

/** \brief 29C, New Data 2 0 */
#define CAN1_N0_NDAT2 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NDAT2*)0xF021829Cu)
/** Alias (User Manual Name) for CAN1_N0_NDAT2.
* To use register names with standard convension, please use CAN1_N0_NDAT2.
*/
#define CAN1_NDAT20 (CAN1_N0_NDAT2)

/** \brief 2A0, Rx FIFO 0 Configuration 0 */
#define CAN1_N0_RX_F0C /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0C*)0xF02182A0u)
/** Alias (User Manual Name) for CAN1_N0_RX_F0C.
* To use register names with standard convension, please use CAN1_N0_RX_F0C.
*/
#define CAN1_RXF0C0 (CAN1_N0_RX_F0C)

/** \brief 2A4, Rx FIFO 0 Status 0 */
#define CAN1_N0_RX_F0S /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0S*)0xF02182A4u)
/** Alias (User Manual Name) for CAN1_N0_RX_F0S.
* To use register names with standard convension, please use CAN1_N0_RX_F0S.
*/
#define CAN1_RXF0S0 (CAN1_N0_RX_F0S)

/** \brief 2A8, Rx FIFO 0 Acknowledge 0 */
#define CAN1_N0_RX_F0A /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0A*)0xF02182A8u)
/** Alias (User Manual Name) for CAN1_N0_RX_F0A.
* To use register names with standard convension, please use CAN1_N0_RX_F0A.
*/
#define CAN1_RXF0A0 (CAN1_N0_RX_F0A)

/** \brief 2AC, Rx Buffer Configuration 0 */
#define CAN1_N0_RX_BC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_BC*)0xF02182ACu)
/** Alias (User Manual Name) for CAN1_N0_RX_BC.
* To use register names with standard convension, please use CAN1_N0_RX_BC.
*/
#define CAN1_RXBC0 (CAN1_N0_RX_BC)

/** \brief 2B0, Rx FIFO 1 Configuration 0 */
#define CAN1_N0_RX_F1C /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1C*)0xF02182B0u)
/** Alias (User Manual Name) for CAN1_N0_RX_F1C.
* To use register names with standard convension, please use CAN1_N0_RX_F1C.
*/
#define CAN1_RXF1C0 (CAN1_N0_RX_F1C)

/** \brief 2B4, Rx FIFO 1 Status 0 */
#define CAN1_N0_RX_F1S /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1S*)0xF02182B4u)
/** Alias (User Manual Name) for CAN1_N0_RX_F1S.
* To use register names with standard convension, please use CAN1_N0_RX_F1S.
*/
#define CAN1_RXF1S0 (CAN1_N0_RX_F1S)

/** \brief 2B8, Rx FIFO 1 Acknowledge 0 */
#define CAN1_N0_RX_F1A /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1A*)0xF02182B8u)
/** Alias (User Manual Name) for CAN1_N0_RX_F1A.
* To use register names with standard convension, please use CAN1_N0_RX_F1A.
*/
#define CAN1_RXF1A0 (CAN1_N0_RX_F1A)

/** \brief 2BC, Rx Buffer/FIFO Element Size Configuration 0 */
#define CAN1_N0_RX_ESC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_ESC*)0xF02182BCu)
/** Alias (User Manual Name) for CAN1_N0_RX_ESC.
* To use register names with standard convension, please use CAN1_N0_RX_ESC.
*/
#define CAN1_RXESC0 (CAN1_N0_RX_ESC)

/** \brief 2C0, Tx Buffer Configuration 0 */
#define CAN1_N0_TX_BC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BC*)0xF02182C0u)
/** Alias (User Manual Name) for CAN1_N0_TX_BC.
* To use register names with standard convension, please use CAN1_N0_TX_BC.
*/
#define CAN1_TXBC0 (CAN1_N0_TX_BC)

/** \brief 2C4, Tx FIFO/Queue Status 0 */
#define CAN1_N0_TX_FQS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_FQS*)0xF02182C4u)
/** Alias (User Manual Name) for CAN1_N0_TX_FQS.
* To use register names with standard convension, please use CAN1_N0_TX_FQS.
*/
#define CAN1_TXFQS0 (CAN1_N0_TX_FQS)

/** \brief 2C8, Tx Buffer Element Size Configuration 0 */
#define CAN1_N0_TX_ESC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_ESC*)0xF02182C8u)
/** Alias (User Manual Name) for CAN1_N0_TX_ESC.
* To use register names with standard convension, please use CAN1_N0_TX_ESC.
*/
#define CAN1_TXESC0 (CAN1_N0_TX_ESC)

/** \brief 2CC, Tx Buffer Request Pending 0 */
#define CAN1_N0_TX_BRP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BRP*)0xF02182CCu)
/** Alias (User Manual Name) for CAN1_N0_TX_BRP.
* To use register names with standard convension, please use CAN1_N0_TX_BRP.
*/
#define CAN1_TXBRP0 (CAN1_N0_TX_BRP)

/** \brief 2D0, Tx Buffer Add Request 0 */
#define CAN1_N0_TX_BAR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BAR*)0xF02182D0u)
/** Alias (User Manual Name) for CAN1_N0_TX_BAR.
* To use register names with standard convension, please use CAN1_N0_TX_BAR.
*/
#define CAN1_TXBAR0 (CAN1_N0_TX_BAR)

/** \brief 2D4, Tx Buffer Cancellation Request 0 */
#define CAN1_N0_TX_BCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCR*)0xF02182D4u)
/** Alias (User Manual Name) for CAN1_N0_TX_BCR.
* To use register names with standard convension, please use CAN1_N0_TX_BCR.
*/
#define CAN1_TXBCR0 (CAN1_N0_TX_BCR)

/** \brief 2D8, Tx Buffer Transmission Occurred 0 */
#define CAN1_N0_TX_BTO /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BTO*)0xF02182D8u)
/** Alias (User Manual Name) for CAN1_N0_TX_BTO.
* To use register names with standard convension, please use CAN1_N0_TX_BTO.
*/
#define CAN1_TXBTO0 (CAN1_N0_TX_BTO)

/** \brief 2DC, Tx Buffer Cancellation Finished 0 */
#define CAN1_N0_TX_BCF /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCF*)0xF02182DCu)
/** Alias (User Manual Name) for CAN1_N0_TX_BCF.
* To use register names with standard convension, please use CAN1_N0_TX_BCF.
*/
#define CAN1_TXBCF0 (CAN1_N0_TX_BCF)

/** \brief 2E0, Tx Buffer Transmission Interrupt Enable 0 */
#define CAN1_N0_TX_BTIE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BTIE*)0xF02182E0u)
/** Alias (User Manual Name) for CAN1_N0_TX_BTIE.
* To use register names with standard convension, please use CAN1_N0_TX_BTIE.
*/
#define CAN1_TXBTIE0 (CAN1_N0_TX_BTIE)

/** \brief 2E4, Tx Buffer Cancellation Finished Interrupt Enable 0 */
#define CAN1_N0_TX_BCIE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCIE*)0xF02182E4u)
/** Alias (User Manual Name) for CAN1_N0_TX_BCIE.
* To use register names with standard convension, please use CAN1_N0_TX_BCIE.
*/
#define CAN1_TXBCIE0 (CAN1_N0_TX_BCIE)

/** \brief 2F0, Tx Event FIFO Configuration 0 */
#define CAN1_N0_TX_EFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFC*)0xF02182F0u)
/** Alias (User Manual Name) for CAN1_N0_TX_EFC.
* To use register names with standard convension, please use CAN1_N0_TX_EFC.
*/
#define CAN1_TXEFC0 (CAN1_N0_TX_EFC)

/** \brief 2F4, Tx Event FIFO Status 0 */
#define CAN1_N0_TX_EFS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFS*)0xF02182F4u)
/** Alias (User Manual Name) for CAN1_N0_TX_EFS.
* To use register names with standard convension, please use CAN1_N0_TX_EFS.
*/
#define CAN1_TXEFS0 (CAN1_N0_TX_EFS)

/** \brief 2F8, Tx Event FIFO Acknowledge 0 */
#define CAN1_N0_TX_EFA /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFA*)0xF02182F8u)
/** Alias (User Manual Name) for CAN1_N0_TX_EFA.
* To use register names with standard convension, please use CAN1_N0_TX_EFA.
*/
#define CAN1_TXEFA0 (CAN1_N0_TX_EFA)

/** \brief 500, Access Enable Register CAN Node 1 0 */
#define CAN1_N1_ACCENNODE0 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ACCENNODE0*)0xF0218500u)
/** Alias (User Manual Name) for CAN1_N1_ACCENNODE0.
* To use register names with standard convension, please use CAN1_N1_ACCENNODE0.
*/
#define CAN1_ACCENNODE10 (CAN1_N1_ACCENNODE0)

/** \brief 508, Start Address Node 1 */
#define CAN1_N1_STARTADR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_STARTADR*)0xF0218508u)
/** Alias (User Manual Name) for CAN1_N1_STARTADR.
* To use register names with standard convension, please use CAN1_N1_STARTADR.
*/
#define CAN1_STARTADR1 (CAN1_N1_STARTADR)

/** \brief 50C, End Address Node 1 */
#define CAN1_N1_ENDADR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ENDADR*)0xF021850Cu)
/** Alias (User Manual Name) for CAN1_N1_ENDADR.
* To use register names with standard convension, please use CAN1_N1_ENDADR.
*/
#define CAN1_ENDADR1 (CAN1_N1_ENDADR)

/** \brief 510, Interrupt Signalling Register 1 */
#define CAN1_N1_ISREG /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ISREG*)0xF0218510u)
/** Alias (User Manual Name) for CAN1_N1_ISREG.
* To use register names with standard convension, please use CAN1_N1_ISREG.
*/
#define CAN1_ISREG1 (CAN1_N1_ISREG)

/** \brief 514, Interrupt routing for Groups 1 1 */
#define CAN1_N1_GRINT1 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GRINT1*)0xF0218514u)
/** Alias (User Manual Name) for CAN1_N1_GRINT1.
* To use register names with standard convension, please use CAN1_N1_GRINT1.
*/
#define CAN1_GRINT11 (CAN1_N1_GRINT1)

/** \brief 518, Interrupt routing for Groups 2 1 */
#define CAN1_N1_GRINT2 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GRINT2*)0xF0218518u)
/** Alias (User Manual Name) for CAN1_N1_GRINT2.
* To use register names with standard convension, please use CAN1_N1_GRINT2.
*/
#define CAN1_GRINT21 (CAN1_N1_GRINT2)

/** \brief 520, Node 1 Timer Clock Control Register */
#define CAN1_N1_NT_CCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_CCR*)0xF0218520u)
/** Alias (User Manual Name) for CAN1_N1_NT_CCR.
* To use register names with standard convension, please use CAN1_N1_NT_CCR.
*/
#define CAN1_NTCCR1 (CAN1_N1_NT_CCR)

/** \brief 524, Node 1 Timer A Transmit Trigger Register */
#define CAN1_N1_NT_ATTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_ATTR*)0xF0218524u)
/** Alias (User Manual Name) for CAN1_N1_NT_ATTR.
* To use register names with standard convension, please use CAN1_N1_NT_ATTR.
*/
#define CAN1_NTATTR1 (CAN1_N1_NT_ATTR)

/** \brief 528, Node 1 Timer B Transmit Trigger Register */
#define CAN1_N1_NT_BTTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_BTTR*)0xF0218528u)
/** Alias (User Manual Name) for CAN1_N1_NT_BTTR.
* To use register names with standard convension, please use CAN1_N1_NT_BTTR.
*/
#define CAN1_NTBTTR1 (CAN1_N1_NT_BTTR)

/** \brief 52C, Node 1 Timer C Transmit Trigger Register */
#define CAN1_N1_NT_CTTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_CTTR*)0xF021852Cu)
/** Alias (User Manual Name) for CAN1_N1_NT_CTTR.
* To use register names with standard convension, please use CAN1_N1_NT_CTTR.
*/
#define CAN1_NTCTTR1 (CAN1_N1_NT_CTTR)

/** \brief 530, Node 1 Timer Receive Timeout Register */
#define CAN1_N1_NT_RTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_RTR*)0xF0218530u)
/** Alias (User Manual Name) for CAN1_N1_NT_RTR.
* To use register names with standard convension, please use CAN1_N1_NT_RTR.
*/
#define CAN1_NTRTR1 (CAN1_N1_NT_RTR)

/** \brief 540, Node 1 Port Control Register */
#define CAN1_N1_NPCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NPCR*)0xF0218540u)
/** Alias (User Manual Name) for CAN1_N1_NPCR.
* To use register names with standard convension, please use CAN1_N1_NPCR.
*/
#define CAN1_NPCR1 (CAN1_N1_NPCR)

/** \brief 600, Core Release Register 1 */
#define CAN1_N1_CREL /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_CREL*)0xF0218600u)
/** Alias (User Manual Name) for CAN1_N1_CREL.
* To use register names with standard convension, please use CAN1_N1_CREL.
*/
#define CAN1_CREL1 (CAN1_N1_CREL)

/** \brief 604, Endian Register 1 */
#define CAN1_N1_ENDN /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ENDN*)0xF0218604u)
/** Alias (User Manual Name) for CAN1_N1_ENDN.
* To use register names with standard convension, please use CAN1_N1_ENDN.
*/
#define CAN1_ENDN1 (CAN1_N1_ENDN)

/** \brief 60C, Data Bit Timing & Prescaler Register 1 */
#define CAN1_N1_DBTP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_DBTP*)0xF021860Cu)
/** Alias (User Manual Name) for CAN1_N1_DBTP.
* To use register names with standard convension, please use CAN1_N1_DBTP.
*/
#define CAN1_DBTP1 (CAN1_N1_DBTP)

/** \brief 610, Test Register 1 */
#define CAN1_N1_TEST /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TEST*)0xF0218610u)
/** Alias (User Manual Name) for CAN1_N1_TEST.
* To use register names with standard convension, please use CAN1_N1_TEST.
*/
#define CAN1_TEST1 (CAN1_N1_TEST)

/** \brief 614, RAM Watchdog 1 */
#define CAN1_N1_RWD /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RWD*)0xF0218614u)
/** Alias (User Manual Name) for CAN1_N1_RWD.
* To use register names with standard convension, please use CAN1_N1_RWD.
*/
#define CAN1_RWD1 (CAN1_N1_RWD)

/** \brief 618, CC Control Register 1 */
#define CAN1_N1_CCCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_CCCR*)0xF0218618u)
/** Alias (User Manual Name) for CAN1_N1_CCCR.
* To use register names with standard convension, please use CAN1_N1_CCCR.
*/
#define CAN1_CCCR1 (CAN1_N1_CCCR)

/** \brief 61C, Nominal Bit Timing & Prescaler Register 1 */
#define CAN1_N1_NBTP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NBTP*)0xF021861Cu)
/** Alias (User Manual Name) for CAN1_N1_NBTP.
* To use register names with standard convension, please use CAN1_N1_NBTP.
*/
#define CAN1_NBTP1 (CAN1_N1_NBTP)

/** \brief 620, Timestamp Counter Configuration 1 */
#define CAN1_N1_TSCC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TSCC*)0xF0218620u)
/** Alias (User Manual Name) for CAN1_N1_TSCC.
* To use register names with standard convension, please use CAN1_N1_TSCC.
*/
#define CAN1_TSCC1 (CAN1_N1_TSCC)

/** \brief 624, Timestamp Counter Value 1 */
#define CAN1_N1_TSCV /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TSCV*)0xF0218624u)
/** Alias (User Manual Name) for CAN1_N1_TSCV.
* To use register names with standard convension, please use CAN1_N1_TSCV.
*/
#define CAN1_TSCV1 (CAN1_N1_TSCV)

/** \brief 628, Timeout Counter Configuration 1 */
#define CAN1_N1_TOCC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TOCC*)0xF0218628u)
/** Alias (User Manual Name) for CAN1_N1_TOCC.
* To use register names with standard convension, please use CAN1_N1_TOCC.
*/
#define CAN1_TOCC1 (CAN1_N1_TOCC)

/** \brief 62C, Timeout Counter Value 1 */
#define CAN1_N1_TOCV /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TOCV*)0xF021862Cu)
/** Alias (User Manual Name) for CAN1_N1_TOCV.
* To use register names with standard convension, please use CAN1_N1_TOCV.
*/
#define CAN1_TOCV1 (CAN1_N1_TOCV)

/** \brief 640, Error Counter Register 1 */
#define CAN1_N1_ECR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ECR*)0xF0218640u)
/** Alias (User Manual Name) for CAN1_N1_ECR.
* To use register names with standard convension, please use CAN1_N1_ECR.
*/
#define CAN1_ECR1 (CAN1_N1_ECR)

/** \brief 644, Protocol Status Register 1 */
#define CAN1_N1_PSR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_PSR*)0xF0218644u)
/** Alias (User Manual Name) for CAN1_N1_PSR.
* To use register names with standard convension, please use CAN1_N1_PSR.
*/
#define CAN1_PSR1 (CAN1_N1_PSR)

/** \brief 648, Transmitter Delay Compensation Register 1 */
#define CAN1_N1_TDCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TDCR*)0xF0218648u)
/** Alias (User Manual Name) for CAN1_N1_TDCR.
* To use register names with standard convension, please use CAN1_N1_TDCR.
*/
#define CAN1_TDCR1 (CAN1_N1_TDCR)

/** \brief 650, Interrupt Register 1 */
#define CAN1_N1_IR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_IR*)0xF0218650u)
/** Alias (User Manual Name) for CAN1_N1_IR.
* To use register names with standard convension, please use CAN1_N1_IR.
*/
#define CAN1_IR1 (CAN1_N1_IR)

/** \brief 654, Interrupt Enable 1 */
#define CAN1_N1_IE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_IE*)0xF0218654u)
/** Alias (User Manual Name) for CAN1_N1_IE.
* To use register names with standard convension, please use CAN1_N1_IE.
*/
#define CAN1_IE1 (CAN1_N1_IE)

/** \brief 680, Global Filter Configuration 1 */
#define CAN1_N1_GFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GFC*)0xF0218680u)
/** Alias (User Manual Name) for CAN1_N1_GFC.
* To use register names with standard convension, please use CAN1_N1_GFC.
*/
#define CAN1_GFC1 (CAN1_N1_GFC)

/** \brief 684, Standard ID Filter Configuration 1 */
#define CAN1_N1_SIDFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_SIDFC*)0xF0218684u)
/** Alias (User Manual Name) for CAN1_N1_SIDFC.
* To use register names with standard convension, please use CAN1_N1_SIDFC.
*/
#define CAN1_SIDFC1 (CAN1_N1_SIDFC)

/** \brief 688, Extended ID Filter Configuration 1 */
#define CAN1_N1_XIDFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_XIDFC*)0xF0218688u)
/** Alias (User Manual Name) for CAN1_N1_XIDFC.
* To use register names with standard convension, please use CAN1_N1_XIDFC.
*/
#define CAN1_XIDFC1 (CAN1_N1_XIDFC)

/** \brief 690, Extended ID AND Mask 1 */
#define CAN1_N1_XIDAM /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_XIDAM*)0xF0218690u)
/** Alias (User Manual Name) for CAN1_N1_XIDAM.
* To use register names with standard convension, please use CAN1_N1_XIDAM.
*/
#define CAN1_XIDAM1 (CAN1_N1_XIDAM)

/** \brief 694, High Priority Message Status 1 */
#define CAN1_N1_HPMS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_HPMS*)0xF0218694u)
/** Alias (User Manual Name) for CAN1_N1_HPMS.
* To use register names with standard convension, please use CAN1_N1_HPMS.
*/
#define CAN1_HPMS1 (CAN1_N1_HPMS)

/** \brief 698, New Data 1 1 */
#define CAN1_N1_NDAT1 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NDAT1*)0xF0218698u)
/** Alias (User Manual Name) for CAN1_N1_NDAT1.
* To use register names with standard convension, please use CAN1_N1_NDAT1.
*/
#define CAN1_NDAT11 (CAN1_N1_NDAT1)

/** \brief 69C, New Data 2 1 */
#define CAN1_N1_NDAT2 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NDAT2*)0xF021869Cu)
/** Alias (User Manual Name) for CAN1_N1_NDAT2.
* To use register names with standard convension, please use CAN1_N1_NDAT2.
*/
#define CAN1_NDAT21 (CAN1_N1_NDAT2)

/** \brief 6A0, Rx FIFO 0 Configuration 1 */
#define CAN1_N1_RX_F0C /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0C*)0xF02186A0u)
/** Alias (User Manual Name) for CAN1_N1_RX_F0C.
* To use register names with standard convension, please use CAN1_N1_RX_F0C.
*/
#define CAN1_RXF0C1 (CAN1_N1_RX_F0C)

/** \brief 6A4, Rx FIFO 0 Status 1 */
#define CAN1_N1_RX_F0S /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0S*)0xF02186A4u)
/** Alias (User Manual Name) for CAN1_N1_RX_F0S.
* To use register names with standard convension, please use CAN1_N1_RX_F0S.
*/
#define CAN1_RXF0S1 (CAN1_N1_RX_F0S)

/** \brief 6A8, Rx FIFO 0 Acknowledge 1 */
#define CAN1_N1_RX_F0A /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0A*)0xF02186A8u)
/** Alias (User Manual Name) for CAN1_N1_RX_F0A.
* To use register names with standard convension, please use CAN1_N1_RX_F0A.
*/
#define CAN1_RXF0A1 (CAN1_N1_RX_F0A)

/** \brief 6AC, Rx Buffer Configuration 1 */
#define CAN1_N1_RX_BC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_BC*)0xF02186ACu)
/** Alias (User Manual Name) for CAN1_N1_RX_BC.
* To use register names with standard convension, please use CAN1_N1_RX_BC.
*/
#define CAN1_RXBC1 (CAN1_N1_RX_BC)

/** \brief 6B0, Rx FIFO 1 Configuration 1 */
#define CAN1_N1_RX_F1C /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1C*)0xF02186B0u)
/** Alias (User Manual Name) for CAN1_N1_RX_F1C.
* To use register names with standard convension, please use CAN1_N1_RX_F1C.
*/
#define CAN1_RXF1C1 (CAN1_N1_RX_F1C)

/** \brief 6B4, Rx FIFO 1 Status 1 */
#define CAN1_N1_RX_F1S /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1S*)0xF02186B4u)
/** Alias (User Manual Name) for CAN1_N1_RX_F1S.
* To use register names with standard convension, please use CAN1_N1_RX_F1S.
*/
#define CAN1_RXF1S1 (CAN1_N1_RX_F1S)

/** \brief 6B8, Rx FIFO 1 Acknowledge 1 */
#define CAN1_N1_RX_F1A /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1A*)0xF02186B8u)
/** Alias (User Manual Name) for CAN1_N1_RX_F1A.
* To use register names with standard convension, please use CAN1_N1_RX_F1A.
*/
#define CAN1_RXF1A1 (CAN1_N1_RX_F1A)

/** \brief 6BC, Rx Buffer/FIFO Element Size Configuration 1 */
#define CAN1_N1_RX_ESC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_ESC*)0xF02186BCu)
/** Alias (User Manual Name) for CAN1_N1_RX_ESC.
* To use register names with standard convension, please use CAN1_N1_RX_ESC.
*/
#define CAN1_RXESC1 (CAN1_N1_RX_ESC)

/** \brief 6C0, Tx Buffer Configuration 1 */
#define CAN1_N1_TX_BC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BC*)0xF02186C0u)
/** Alias (User Manual Name) for CAN1_N1_TX_BC.
* To use register names with standard convension, please use CAN1_N1_TX_BC.
*/
#define CAN1_TXBC1 (CAN1_N1_TX_BC)

/** \brief 6C4, Tx FIFO/Queue Status 1 */
#define CAN1_N1_TX_FQS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_FQS*)0xF02186C4u)
/** Alias (User Manual Name) for CAN1_N1_TX_FQS.
* To use register names with standard convension, please use CAN1_N1_TX_FQS.
*/
#define CAN1_TXFQS1 (CAN1_N1_TX_FQS)

/** \brief 6C8, Tx Buffer Element Size Configuration 1 */
#define CAN1_N1_TX_ESC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_ESC*)0xF02186C8u)
/** Alias (User Manual Name) for CAN1_N1_TX_ESC.
* To use register names with standard convension, please use CAN1_N1_TX_ESC.
*/
#define CAN1_TXESC1 (CAN1_N1_TX_ESC)

/** \brief 6CC, Tx Buffer Request Pending 1 */
#define CAN1_N1_TX_BRP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BRP*)0xF02186CCu)
/** Alias (User Manual Name) for CAN1_N1_TX_BRP.
* To use register names with standard convension, please use CAN1_N1_TX_BRP.
*/
#define CAN1_TXBRP1 (CAN1_N1_TX_BRP)

/** \brief 6D0, Tx Buffer Add Request 1 */
#define CAN1_N1_TX_BAR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BAR*)0xF02186D0u)
/** Alias (User Manual Name) for CAN1_N1_TX_BAR.
* To use register names with standard convension, please use CAN1_N1_TX_BAR.
*/
#define CAN1_TXBAR1 (CAN1_N1_TX_BAR)

/** \brief 6D4, Tx Buffer Cancellation Request 1 */
#define CAN1_N1_TX_BCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCR*)0xF02186D4u)
/** Alias (User Manual Name) for CAN1_N1_TX_BCR.
* To use register names with standard convension, please use CAN1_N1_TX_BCR.
*/
#define CAN1_TXBCR1 (CAN1_N1_TX_BCR)

/** \brief 6D8, Tx Buffer Transmission Occurred 1 */
#define CAN1_N1_TX_BTO /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BTO*)0xF02186D8u)
/** Alias (User Manual Name) for CAN1_N1_TX_BTO.
* To use register names with standard convension, please use CAN1_N1_TX_BTO.
*/
#define CAN1_TXBTO1 (CAN1_N1_TX_BTO)

/** \brief 6DC, Tx Buffer Cancellation Finished 1 */
#define CAN1_N1_TX_BCF /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCF*)0xF02186DCu)
/** Alias (User Manual Name) for CAN1_N1_TX_BCF.
* To use register names with standard convension, please use CAN1_N1_TX_BCF.
*/
#define CAN1_TXBCF1 (CAN1_N1_TX_BCF)

/** \brief 6E0, Tx Buffer Transmission Interrupt Enable 1 */
#define CAN1_N1_TX_BTIE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BTIE*)0xF02186E0u)
/** Alias (User Manual Name) for CAN1_N1_TX_BTIE.
* To use register names with standard convension, please use CAN1_N1_TX_BTIE.
*/
#define CAN1_TXBTIE1 (CAN1_N1_TX_BTIE)

/** \brief 6E4, Tx Buffer Cancellation Finished Interrupt Enable 1 */
#define CAN1_N1_TX_BCIE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCIE*)0xF02186E4u)
/** Alias (User Manual Name) for CAN1_N1_TX_BCIE.
* To use register names with standard convension, please use CAN1_N1_TX_BCIE.
*/
#define CAN1_TXBCIE1 (CAN1_N1_TX_BCIE)

/** \brief 6F0, Tx Event FIFO Configuration 1 */
#define CAN1_N1_TX_EFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFC*)0xF02186F0u)
/** Alias (User Manual Name) for CAN1_N1_TX_EFC.
* To use register names with standard convension, please use CAN1_N1_TX_EFC.
*/
#define CAN1_TXEFC1 (CAN1_N1_TX_EFC)

/** \brief 6F4, Tx Event FIFO Status 1 */
#define CAN1_N1_TX_EFS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFS*)0xF02186F4u)
/** Alias (User Manual Name) for CAN1_N1_TX_EFS.
* To use register names with standard convension, please use CAN1_N1_TX_EFS.
*/
#define CAN1_TXEFS1 (CAN1_N1_TX_EFS)

/** \brief 6F8, Tx Event FIFO Acknowledge 1 */
#define CAN1_N1_TX_EFA /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFA*)0xF02186F8u)
/** Alias (User Manual Name) for CAN1_N1_TX_EFA.
* To use register names with standard convension, please use CAN1_N1_TX_EFA.
*/
#define CAN1_TXEFA1 (CAN1_N1_TX_EFA)

/** \brief 900, Access Enable Register CAN Node 2 0 */
#define CAN1_N2_ACCENNODE0 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ACCENNODE0*)0xF0218900u)
/** Alias (User Manual Name) for CAN1_N2_ACCENNODE0.
* To use register names with standard convension, please use CAN1_N2_ACCENNODE0.
*/
#define CAN1_ACCENNODE20 (CAN1_N2_ACCENNODE0)

/** \brief 908, Start Address Node 2 */
#define CAN1_N2_STARTADR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_STARTADR*)0xF0218908u)
/** Alias (User Manual Name) for CAN1_N2_STARTADR.
* To use register names with standard convension, please use CAN1_N2_STARTADR.
*/
#define CAN1_STARTADR2 (CAN1_N2_STARTADR)

/** \brief 90C, End Address Node 2 */
#define CAN1_N2_ENDADR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ENDADR*)0xF021890Cu)
/** Alias (User Manual Name) for CAN1_N2_ENDADR.
* To use register names with standard convension, please use CAN1_N2_ENDADR.
*/
#define CAN1_ENDADR2 (CAN1_N2_ENDADR)

/** \brief 910, Interrupt Signalling Register 2 */
#define CAN1_N2_ISREG /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ISREG*)0xF0218910u)
/** Alias (User Manual Name) for CAN1_N2_ISREG.
* To use register names with standard convension, please use CAN1_N2_ISREG.
*/
#define CAN1_ISREG2 (CAN1_N2_ISREG)

/** \brief 914, Interrupt routing for Groups 1 2 */
#define CAN1_N2_GRINT1 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GRINT1*)0xF0218914u)
/** Alias (User Manual Name) for CAN1_N2_GRINT1.
* To use register names with standard convension, please use CAN1_N2_GRINT1.
*/
#define CAN1_GRINT12 (CAN1_N2_GRINT1)

/** \brief 918, Interrupt routing for Groups 2 2 */
#define CAN1_N2_GRINT2 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GRINT2*)0xF0218918u)
/** Alias (User Manual Name) for CAN1_N2_GRINT2.
* To use register names with standard convension, please use CAN1_N2_GRINT2.
*/
#define CAN1_GRINT22 (CAN1_N2_GRINT2)

/** \brief 920, Node 2 Timer Clock Control Register */
#define CAN1_N2_NT_CCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_CCR*)0xF0218920u)
/** Alias (User Manual Name) for CAN1_N2_NT_CCR.
* To use register names with standard convension, please use CAN1_N2_NT_CCR.
*/
#define CAN1_NTCCR2 (CAN1_N2_NT_CCR)

/** \brief 924, Node 2 Timer A Transmit Trigger Register */
#define CAN1_N2_NT_ATTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_ATTR*)0xF0218924u)
/** Alias (User Manual Name) for CAN1_N2_NT_ATTR.
* To use register names with standard convension, please use CAN1_N2_NT_ATTR.
*/
#define CAN1_NTATTR2 (CAN1_N2_NT_ATTR)

/** \brief 928, Node 2 Timer B Transmit Trigger Register */
#define CAN1_N2_NT_BTTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_BTTR*)0xF0218928u)
/** Alias (User Manual Name) for CAN1_N2_NT_BTTR.
* To use register names with standard convension, please use CAN1_N2_NT_BTTR.
*/
#define CAN1_NTBTTR2 (CAN1_N2_NT_BTTR)

/** \brief 92C, Node 2 Timer C Transmit Trigger Register */
#define CAN1_N2_NT_CTTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_CTTR*)0xF021892Cu)
/** Alias (User Manual Name) for CAN1_N2_NT_CTTR.
* To use register names with standard convension, please use CAN1_N2_NT_CTTR.
*/
#define CAN1_NTCTTR2 (CAN1_N2_NT_CTTR)

/** \brief 930, Node 2 Timer Receive Timeout Register */
#define CAN1_N2_NT_RTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_RTR*)0xF0218930u)
/** Alias (User Manual Name) for CAN1_N2_NT_RTR.
* To use register names with standard convension, please use CAN1_N2_NT_RTR.
*/
#define CAN1_NTRTR2 (CAN1_N2_NT_RTR)

/** \brief 940, Node 2 Port Control Register */
#define CAN1_N2_NPCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NPCR*)0xF0218940u)
/** Alias (User Manual Name) for CAN1_N2_NPCR.
* To use register names with standard convension, please use CAN1_N2_NPCR.
*/
#define CAN1_NPCR2 (CAN1_N2_NPCR)

/** \brief A00, Core Release Register 2 */
#define CAN1_N2_CREL /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_CREL*)0xF0218A00u)
/** Alias (User Manual Name) for CAN1_N2_CREL.
* To use register names with standard convension, please use CAN1_N2_CREL.
*/
#define CAN1_CREL2 (CAN1_N2_CREL)

/** \brief A04, Endian Register 2 */
#define CAN1_N2_ENDN /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ENDN*)0xF0218A04u)
/** Alias (User Manual Name) for CAN1_N2_ENDN.
* To use register names with standard convension, please use CAN1_N2_ENDN.
*/
#define CAN1_ENDN2 (CAN1_N2_ENDN)

/** \brief A0C, Data Bit Timing & Prescaler Register 2 */
#define CAN1_N2_DBTP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_DBTP*)0xF0218A0Cu)
/** Alias (User Manual Name) for CAN1_N2_DBTP.
* To use register names with standard convension, please use CAN1_N2_DBTP.
*/
#define CAN1_DBTP2 (CAN1_N2_DBTP)

/** \brief A10, Test Register 2 */
#define CAN1_N2_TEST /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TEST*)0xF0218A10u)
/** Alias (User Manual Name) for CAN1_N2_TEST.
* To use register names with standard convension, please use CAN1_N2_TEST.
*/
#define CAN1_TEST2 (CAN1_N2_TEST)

/** \brief A14, RAM Watchdog 2 */
#define CAN1_N2_RWD /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RWD*)0xF0218A14u)
/** Alias (User Manual Name) for CAN1_N2_RWD.
* To use register names with standard convension, please use CAN1_N2_RWD.
*/
#define CAN1_RWD2 (CAN1_N2_RWD)

/** \brief A18, CC Control Register 2 */
#define CAN1_N2_CCCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_CCCR*)0xF0218A18u)
/** Alias (User Manual Name) for CAN1_N2_CCCR.
* To use register names with standard convension, please use CAN1_N2_CCCR.
*/
#define CAN1_CCCR2 (CAN1_N2_CCCR)

/** \brief A1C, Nominal Bit Timing & Prescaler Register 2 */
#define CAN1_N2_NBTP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NBTP*)0xF0218A1Cu)
/** Alias (User Manual Name) for CAN1_N2_NBTP.
* To use register names with standard convension, please use CAN1_N2_NBTP.
*/
#define CAN1_NBTP2 (CAN1_N2_NBTP)

/** \brief A20, Timestamp Counter Configuration 2 */
#define CAN1_N2_TSCC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TSCC*)0xF0218A20u)
/** Alias (User Manual Name) for CAN1_N2_TSCC.
* To use register names with standard convension, please use CAN1_N2_TSCC.
*/
#define CAN1_TSCC2 (CAN1_N2_TSCC)

/** \brief A24, Timestamp Counter Value 2 */
#define CAN1_N2_TSCV /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TSCV*)0xF0218A24u)
/** Alias (User Manual Name) for CAN1_N2_TSCV.
* To use register names with standard convension, please use CAN1_N2_TSCV.
*/
#define CAN1_TSCV2 (CAN1_N2_TSCV)

/** \brief A28, Timeout Counter Configuration 2 */
#define CAN1_N2_TOCC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TOCC*)0xF0218A28u)
/** Alias (User Manual Name) for CAN1_N2_TOCC.
* To use register names with standard convension, please use CAN1_N2_TOCC.
*/
#define CAN1_TOCC2 (CAN1_N2_TOCC)

/** \brief A2C, Timeout Counter Value 2 */
#define CAN1_N2_TOCV /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TOCV*)0xF0218A2Cu)
/** Alias (User Manual Name) for CAN1_N2_TOCV.
* To use register names with standard convension, please use CAN1_N2_TOCV.
*/
#define CAN1_TOCV2 (CAN1_N2_TOCV)

/** \brief A40, Error Counter Register 2 */
#define CAN1_N2_ECR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ECR*)0xF0218A40u)
/** Alias (User Manual Name) for CAN1_N2_ECR.
* To use register names with standard convension, please use CAN1_N2_ECR.
*/
#define CAN1_ECR2 (CAN1_N2_ECR)

/** \brief A44, Protocol Status Register 2 */
#define CAN1_N2_PSR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_PSR*)0xF0218A44u)
/** Alias (User Manual Name) for CAN1_N2_PSR.
* To use register names with standard convension, please use CAN1_N2_PSR.
*/
#define CAN1_PSR2 (CAN1_N2_PSR)

/** \brief A48, Transmitter Delay Compensation Register 2 */
#define CAN1_N2_TDCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TDCR*)0xF0218A48u)
/** Alias (User Manual Name) for CAN1_N2_TDCR.
* To use register names with standard convension, please use CAN1_N2_TDCR.
*/
#define CAN1_TDCR2 (CAN1_N2_TDCR)

/** \brief A50, Interrupt Register 2 */
#define CAN1_N2_IR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_IR*)0xF0218A50u)
/** Alias (User Manual Name) for CAN1_N2_IR.
* To use register names with standard convension, please use CAN1_N2_IR.
*/
#define CAN1_IR2 (CAN1_N2_IR)

/** \brief A54, Interrupt Enable 2 */
#define CAN1_N2_IE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_IE*)0xF0218A54u)
/** Alias (User Manual Name) for CAN1_N2_IE.
* To use register names with standard convension, please use CAN1_N2_IE.
*/
#define CAN1_IE2 (CAN1_N2_IE)

/** \brief A80, Global Filter Configuration 2 */
#define CAN1_N2_GFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GFC*)0xF0218A80u)
/** Alias (User Manual Name) for CAN1_N2_GFC.
* To use register names with standard convension, please use CAN1_N2_GFC.
*/
#define CAN1_GFC2 (CAN1_N2_GFC)

/** \brief A84, Standard ID Filter Configuration 2 */
#define CAN1_N2_SIDFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_SIDFC*)0xF0218A84u)
/** Alias (User Manual Name) for CAN1_N2_SIDFC.
* To use register names with standard convension, please use CAN1_N2_SIDFC.
*/
#define CAN1_SIDFC2 (CAN1_N2_SIDFC)

/** \brief A88, Extended ID Filter Configuration 2 */
#define CAN1_N2_XIDFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_XIDFC*)0xF0218A88u)
/** Alias (User Manual Name) for CAN1_N2_XIDFC.
* To use register names with standard convension, please use CAN1_N2_XIDFC.
*/
#define CAN1_XIDFC2 (CAN1_N2_XIDFC)

/** \brief A90, Extended ID AND Mask 2 */
#define CAN1_N2_XIDAM /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_XIDAM*)0xF0218A90u)
/** Alias (User Manual Name) for CAN1_N2_XIDAM.
* To use register names with standard convension, please use CAN1_N2_XIDAM.
*/
#define CAN1_XIDAM2 (CAN1_N2_XIDAM)

/** \brief A94, High Priority Message Status 2 */
#define CAN1_N2_HPMS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_HPMS*)0xF0218A94u)
/** Alias (User Manual Name) for CAN1_N2_HPMS.
* To use register names with standard convension, please use CAN1_N2_HPMS.
*/
#define CAN1_HPMS2 (CAN1_N2_HPMS)

/** \brief A98, New Data 1 2 */
#define CAN1_N2_NDAT1 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NDAT1*)0xF0218A98u)
/** Alias (User Manual Name) for CAN1_N2_NDAT1.
* To use register names with standard convension, please use CAN1_N2_NDAT1.
*/
#define CAN1_NDAT12 (CAN1_N2_NDAT1)

/** \brief A9C, New Data 2 2 */
#define CAN1_N2_NDAT2 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NDAT2*)0xF0218A9Cu)
/** Alias (User Manual Name) for CAN1_N2_NDAT2.
* To use register names with standard convension, please use CAN1_N2_NDAT2.
*/
#define CAN1_NDAT22 (CAN1_N2_NDAT2)

/** \brief AA0, Rx FIFO 0 Configuration 2 */
#define CAN1_N2_RX_F0C /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0C*)0xF0218AA0u)
/** Alias (User Manual Name) for CAN1_N2_RX_F0C.
* To use register names with standard convension, please use CAN1_N2_RX_F0C.
*/
#define CAN1_RXF0C2 (CAN1_N2_RX_F0C)

/** \brief AA4, Rx FIFO 0 Status 2 */
#define CAN1_N2_RX_F0S /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0S*)0xF0218AA4u)
/** Alias (User Manual Name) for CAN1_N2_RX_F0S.
* To use register names with standard convension, please use CAN1_N2_RX_F0S.
*/
#define CAN1_RXF0S2 (CAN1_N2_RX_F0S)

/** \brief AA8, Rx FIFO 0 Acknowledge 2 */
#define CAN1_N2_RX_F0A /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0A*)0xF0218AA8u)
/** Alias (User Manual Name) for CAN1_N2_RX_F0A.
* To use register names with standard convension, please use CAN1_N2_RX_F0A.
*/
#define CAN1_RXF0A2 (CAN1_N2_RX_F0A)

/** \brief AAC, Rx Buffer Configuration 2 */
#define CAN1_N2_RX_BC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_BC*)0xF0218AACu)
/** Alias (User Manual Name) for CAN1_N2_RX_BC.
* To use register names with standard convension, please use CAN1_N2_RX_BC.
*/
#define CAN1_RXBC2 (CAN1_N2_RX_BC)

/** \brief AB0, Rx FIFO 1 Configuration 2 */
#define CAN1_N2_RX_F1C /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1C*)0xF0218AB0u)
/** Alias (User Manual Name) for CAN1_N2_RX_F1C.
* To use register names with standard convension, please use CAN1_N2_RX_F1C.
*/
#define CAN1_RXF1C2 (CAN1_N2_RX_F1C)

/** \brief AB4, Rx FIFO 1 Status 2 */
#define CAN1_N2_RX_F1S /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1S*)0xF0218AB4u)
/** Alias (User Manual Name) for CAN1_N2_RX_F1S.
* To use register names with standard convension, please use CAN1_N2_RX_F1S.
*/
#define CAN1_RXF1S2 (CAN1_N2_RX_F1S)

/** \brief AB8, Rx FIFO 1 Acknowledge 2 */
#define CAN1_N2_RX_F1A /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1A*)0xF0218AB8u)
/** Alias (User Manual Name) for CAN1_N2_RX_F1A.
* To use register names with standard convension, please use CAN1_N2_RX_F1A.
*/
#define CAN1_RXF1A2 (CAN1_N2_RX_F1A)

/** \brief ABC, Rx Buffer/FIFO Element Size Configuration 2 */
#define CAN1_N2_RX_ESC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_ESC*)0xF0218ABCu)
/** Alias (User Manual Name) for CAN1_N2_RX_ESC.
* To use register names with standard convension, please use CAN1_N2_RX_ESC.
*/
#define CAN1_RXESC2 (CAN1_N2_RX_ESC)

/** \brief AC0, Tx Buffer Configuration 2 */
#define CAN1_N2_TX_BC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BC*)0xF0218AC0u)
/** Alias (User Manual Name) for CAN1_N2_TX_BC.
* To use register names with standard convension, please use CAN1_N2_TX_BC.
*/
#define CAN1_TXBC2 (CAN1_N2_TX_BC)

/** \brief AC4, Tx FIFO/Queue Status 2 */
#define CAN1_N2_TX_FQS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_FQS*)0xF0218AC4u)
/** Alias (User Manual Name) for CAN1_N2_TX_FQS.
* To use register names with standard convension, please use CAN1_N2_TX_FQS.
*/
#define CAN1_TXFQS2 (CAN1_N2_TX_FQS)

/** \brief AC8, Tx Buffer Element Size Configuration 2 */
#define CAN1_N2_TX_ESC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_ESC*)0xF0218AC8u)
/** Alias (User Manual Name) for CAN1_N2_TX_ESC.
* To use register names with standard convension, please use CAN1_N2_TX_ESC.
*/
#define CAN1_TXESC2 (CAN1_N2_TX_ESC)

/** \brief ACC, Tx Buffer Request Pending 2 */
#define CAN1_N2_TX_BRP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BRP*)0xF0218ACCu)
/** Alias (User Manual Name) for CAN1_N2_TX_BRP.
* To use register names with standard convension, please use CAN1_N2_TX_BRP.
*/
#define CAN1_TXBRP2 (CAN1_N2_TX_BRP)

/** \brief AD0, Tx Buffer Add Request 2 */
#define CAN1_N2_TX_BAR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BAR*)0xF0218AD0u)
/** Alias (User Manual Name) for CAN1_N2_TX_BAR.
* To use register names with standard convension, please use CAN1_N2_TX_BAR.
*/
#define CAN1_TXBAR2 (CAN1_N2_TX_BAR)

/** \brief AD4, Tx Buffer Cancellation Request 2 */
#define CAN1_N2_TX_BCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCR*)0xF0218AD4u)
/** Alias (User Manual Name) for CAN1_N2_TX_BCR.
* To use register names with standard convension, please use CAN1_N2_TX_BCR.
*/
#define CAN1_TXBCR2 (CAN1_N2_TX_BCR)

/** \brief AD8, Tx Buffer Transmission Occurred 2 */
#define CAN1_N2_TX_BTO /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BTO*)0xF0218AD8u)
/** Alias (User Manual Name) for CAN1_N2_TX_BTO.
* To use register names with standard convension, please use CAN1_N2_TX_BTO.
*/
#define CAN1_TXBTO2 (CAN1_N2_TX_BTO)

/** \brief ADC, Tx Buffer Cancellation Finished 2 */
#define CAN1_N2_TX_BCF /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCF*)0xF0218ADCu)
/** Alias (User Manual Name) for CAN1_N2_TX_BCF.
* To use register names with standard convension, please use CAN1_N2_TX_BCF.
*/
#define CAN1_TXBCF2 (CAN1_N2_TX_BCF)

/** \brief AE0, Tx Buffer Transmission Interrupt Enable 2 */
#define CAN1_N2_TX_BTIE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BTIE*)0xF0218AE0u)
/** Alias (User Manual Name) for CAN1_N2_TX_BTIE.
* To use register names with standard convension, please use CAN1_N2_TX_BTIE.
*/
#define CAN1_TXBTIE2 (CAN1_N2_TX_BTIE)

/** \brief AE4, Tx Buffer Cancellation Finished Interrupt Enable 2 */
#define CAN1_N2_TX_BCIE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCIE*)0xF0218AE4u)
/** Alias (User Manual Name) for CAN1_N2_TX_BCIE.
* To use register names with standard convension, please use CAN1_N2_TX_BCIE.
*/
#define CAN1_TXBCIE2 (CAN1_N2_TX_BCIE)

/** \brief AF0, Tx Event FIFO Configuration 2 */
#define CAN1_N2_TX_EFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFC*)0xF0218AF0u)
/** Alias (User Manual Name) for CAN1_N2_TX_EFC.
* To use register names with standard convension, please use CAN1_N2_TX_EFC.
*/
#define CAN1_TXEFC2 (CAN1_N2_TX_EFC)

/** \brief AF4, Tx Event FIFO Status 2 */
#define CAN1_N2_TX_EFS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFS*)0xF0218AF4u)
/** Alias (User Manual Name) for CAN1_N2_TX_EFS.
* To use register names with standard convension, please use CAN1_N2_TX_EFS.
*/
#define CAN1_TXEFS2 (CAN1_N2_TX_EFS)

/** \brief AF8, Tx Event FIFO Acknowledge 2 */
#define CAN1_N2_TX_EFA /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFA*)0xF0218AF8u)
/** Alias (User Manual Name) for CAN1_N2_TX_EFA.
* To use register names with standard convension, please use CAN1_N2_TX_EFA.
*/
#define CAN1_TXEFA2 (CAN1_N2_TX_EFA)

/** \brief D00, Access Enable Register CAN Node 3 0 */
#define CAN1_N3_ACCENNODE0 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ACCENNODE0*)0xF0218D00u)
/** Alias (User Manual Name) for CAN1_N3_ACCENNODE0.
* To use register names with standard convension, please use CAN1_N3_ACCENNODE0.
*/
#define CAN1_ACCENNODE30 (CAN1_N3_ACCENNODE0)

/** \brief D08, Start Address Node 3 */
#define CAN1_N3_STARTADR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_STARTADR*)0xF0218D08u)
/** Alias (User Manual Name) for CAN1_N3_STARTADR.
* To use register names with standard convension, please use CAN1_N3_STARTADR.
*/
#define CAN1_STARTADR3 (CAN1_N3_STARTADR)

/** \brief D0C, End Address Node 3 */
#define CAN1_N3_ENDADR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ENDADR*)0xF0218D0Cu)
/** Alias (User Manual Name) for CAN1_N3_ENDADR.
* To use register names with standard convension, please use CAN1_N3_ENDADR.
*/
#define CAN1_ENDADR3 (CAN1_N3_ENDADR)

/** \brief D10, Interrupt Signalling Register 3 */
#define CAN1_N3_ISREG /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ISREG*)0xF0218D10u)
/** Alias (User Manual Name) for CAN1_N3_ISREG.
* To use register names with standard convension, please use CAN1_N3_ISREG.
*/
#define CAN1_ISREG3 (CAN1_N3_ISREG)

/** \brief D14, Interrupt routing for Groups 1 3 */
#define CAN1_N3_GRINT1 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GRINT1*)0xF0218D14u)
/** Alias (User Manual Name) for CAN1_N3_GRINT1.
* To use register names with standard convension, please use CAN1_N3_GRINT1.
*/
#define CAN1_GRINT13 (CAN1_N3_GRINT1)

/** \brief D18, Interrupt routing for Groups 2 3 */
#define CAN1_N3_GRINT2 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GRINT2*)0xF0218D18u)
/** Alias (User Manual Name) for CAN1_N3_GRINT2.
* To use register names with standard convension, please use CAN1_N3_GRINT2.
*/
#define CAN1_GRINT23 (CAN1_N3_GRINT2)

/** \brief D20, Node 3 Timer Clock Control Register */
#define CAN1_N3_NT_CCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_CCR*)0xF0218D20u)
/** Alias (User Manual Name) for CAN1_N3_NT_CCR.
* To use register names with standard convension, please use CAN1_N3_NT_CCR.
*/
#define CAN1_NTCCR3 (CAN1_N3_NT_CCR)

/** \brief D24, Node 3 Timer A Transmit Trigger Register */
#define CAN1_N3_NT_ATTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_ATTR*)0xF0218D24u)
/** Alias (User Manual Name) for CAN1_N3_NT_ATTR.
* To use register names with standard convension, please use CAN1_N3_NT_ATTR.
*/
#define CAN1_NTATTR3 (CAN1_N3_NT_ATTR)

/** \brief D28, Node 3 Timer B Transmit Trigger Register */
#define CAN1_N3_NT_BTTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_BTTR*)0xF0218D28u)
/** Alias (User Manual Name) for CAN1_N3_NT_BTTR.
* To use register names with standard convension, please use CAN1_N3_NT_BTTR.
*/
#define CAN1_NTBTTR3 (CAN1_N3_NT_BTTR)

/** \brief D2C, Node 3 Timer C Transmit Trigger Register */
#define CAN1_N3_NT_CTTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_CTTR*)0xF0218D2Cu)
/** Alias (User Manual Name) for CAN1_N3_NT_CTTR.
* To use register names with standard convension, please use CAN1_N3_NT_CTTR.
*/
#define CAN1_NTCTTR3 (CAN1_N3_NT_CTTR)

/** \brief D30, Node 3 Timer Receive Timeout Register */
#define CAN1_N3_NT_RTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_RTR*)0xF0218D30u)
/** Alias (User Manual Name) for CAN1_N3_NT_RTR.
* To use register names with standard convension, please use CAN1_N3_NT_RTR.
*/
#define CAN1_NTRTR3 (CAN1_N3_NT_RTR)

/** \brief D40, Node 3 Port Control Register */
#define CAN1_N3_NPCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NPCR*)0xF0218D40u)
/** Alias (User Manual Name) for CAN1_N3_NPCR.
* To use register names with standard convension, please use CAN1_N3_NPCR.
*/
#define CAN1_NPCR3 (CAN1_N3_NPCR)

/** \brief E00, Core Release Register 3 */
#define CAN1_N3_CREL /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_CREL*)0xF0218E00u)
/** Alias (User Manual Name) for CAN1_N3_CREL.
* To use register names with standard convension, please use CAN1_N3_CREL.
*/
#define CAN1_CREL3 (CAN1_N3_CREL)

/** \brief E04, Endian Register 3 */
#define CAN1_N3_ENDN /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ENDN*)0xF0218E04u)
/** Alias (User Manual Name) for CAN1_N3_ENDN.
* To use register names with standard convension, please use CAN1_N3_ENDN.
*/
#define CAN1_ENDN3 (CAN1_N3_ENDN)

/** \brief E0C, Data Bit Timing & Prescaler Register 3 */
#define CAN1_N3_DBTP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_DBTP*)0xF0218E0Cu)
/** Alias (User Manual Name) for CAN1_N3_DBTP.
* To use register names with standard convension, please use CAN1_N3_DBTP.
*/
#define CAN1_DBTP3 (CAN1_N3_DBTP)

/** \brief E10, Test Register 3 */
#define CAN1_N3_TEST /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TEST*)0xF0218E10u)
/** Alias (User Manual Name) for CAN1_N3_TEST.
* To use register names with standard convension, please use CAN1_N3_TEST.
*/
#define CAN1_TEST3 (CAN1_N3_TEST)

/** \brief E14, RAM Watchdog 3 */
#define CAN1_N3_RWD /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RWD*)0xF0218E14u)
/** Alias (User Manual Name) for CAN1_N3_RWD.
* To use register names with standard convension, please use CAN1_N3_RWD.
*/
#define CAN1_RWD3 (CAN1_N3_RWD)

/** \brief E18, CC Control Register 3 */
#define CAN1_N3_CCCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_CCCR*)0xF0218E18u)
/** Alias (User Manual Name) for CAN1_N3_CCCR.
* To use register names with standard convension, please use CAN1_N3_CCCR.
*/
#define CAN1_CCCR3 (CAN1_N3_CCCR)

/** \brief E1C, Nominal Bit Timing & Prescaler Register 3 */
#define CAN1_N3_NBTP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NBTP*)0xF0218E1Cu)
/** Alias (User Manual Name) for CAN1_N3_NBTP.
* To use register names with standard convension, please use CAN1_N3_NBTP.
*/
#define CAN1_NBTP3 (CAN1_N3_NBTP)

/** \brief E20, Timestamp Counter Configuration 3 */
#define CAN1_N3_TSCC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TSCC*)0xF0218E20u)
/** Alias (User Manual Name) for CAN1_N3_TSCC.
* To use register names with standard convension, please use CAN1_N3_TSCC.
*/
#define CAN1_TSCC3 (CAN1_N3_TSCC)

/** \brief E24, Timestamp Counter Value 3 */
#define CAN1_N3_TSCV /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TSCV*)0xF0218E24u)
/** Alias (User Manual Name) for CAN1_N3_TSCV.
* To use register names with standard convension, please use CAN1_N3_TSCV.
*/
#define CAN1_TSCV3 (CAN1_N3_TSCV)

/** \brief E28, Timeout Counter Configuration 3 */
#define CAN1_N3_TOCC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TOCC*)0xF0218E28u)
/** Alias (User Manual Name) for CAN1_N3_TOCC.
* To use register names with standard convension, please use CAN1_N3_TOCC.
*/
#define CAN1_TOCC3 (CAN1_N3_TOCC)

/** \brief E2C, Timeout Counter Value 3 */
#define CAN1_N3_TOCV /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TOCV*)0xF0218E2Cu)
/** Alias (User Manual Name) for CAN1_N3_TOCV.
* To use register names with standard convension, please use CAN1_N3_TOCV.
*/
#define CAN1_TOCV3 (CAN1_N3_TOCV)

/** \brief E40, Error Counter Register 3 */
#define CAN1_N3_ECR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ECR*)0xF0218E40u)
/** Alias (User Manual Name) for CAN1_N3_ECR.
* To use register names with standard convension, please use CAN1_N3_ECR.
*/
#define CAN1_ECR3 (CAN1_N3_ECR)

/** \brief E44, Protocol Status Register 3 */
#define CAN1_N3_PSR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_PSR*)0xF0218E44u)
/** Alias (User Manual Name) for CAN1_N3_PSR.
* To use register names with standard convension, please use CAN1_N3_PSR.
*/
#define CAN1_PSR3 (CAN1_N3_PSR)

/** \brief E48, Transmitter Delay Compensation Register 3 */
#define CAN1_N3_TDCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TDCR*)0xF0218E48u)
/** Alias (User Manual Name) for CAN1_N3_TDCR.
* To use register names with standard convension, please use CAN1_N3_TDCR.
*/
#define CAN1_TDCR3 (CAN1_N3_TDCR)

/** \brief E50, Interrupt Register 3 */
#define CAN1_N3_IR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_IR*)0xF0218E50u)
/** Alias (User Manual Name) for CAN1_N3_IR.
* To use register names with standard convension, please use CAN1_N3_IR.
*/
#define CAN1_IR3 (CAN1_N3_IR)

/** \brief E54, Interrupt Enable 3 */
#define CAN1_N3_IE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_IE*)0xF0218E54u)
/** Alias (User Manual Name) for CAN1_N3_IE.
* To use register names with standard convension, please use CAN1_N3_IE.
*/
#define CAN1_IE3 (CAN1_N3_IE)

/** \brief E80, Global Filter Configuration 3 */
#define CAN1_N3_GFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GFC*)0xF0218E80u)
/** Alias (User Manual Name) for CAN1_N3_GFC.
* To use register names with standard convension, please use CAN1_N3_GFC.
*/
#define CAN1_GFC3 (CAN1_N3_GFC)

/** \brief E84, Standard ID Filter Configuration 3 */
#define CAN1_N3_SIDFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_SIDFC*)0xF0218E84u)
/** Alias (User Manual Name) for CAN1_N3_SIDFC.
* To use register names with standard convension, please use CAN1_N3_SIDFC.
*/
#define CAN1_SIDFC3 (CAN1_N3_SIDFC)

/** \brief E88, Extended ID Filter Configuration 3 */
#define CAN1_N3_XIDFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_XIDFC*)0xF0218E88u)
/** Alias (User Manual Name) for CAN1_N3_XIDFC.
* To use register names with standard convension, please use CAN1_N3_XIDFC.
*/
#define CAN1_XIDFC3 (CAN1_N3_XIDFC)

/** \brief E90, Extended ID AND Mask 3 */
#define CAN1_N3_XIDAM /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_XIDAM*)0xF0218E90u)
/** Alias (User Manual Name) for CAN1_N3_XIDAM.
* To use register names with standard convension, please use CAN1_N3_XIDAM.
*/
#define CAN1_XIDAM3 (CAN1_N3_XIDAM)

/** \brief E94, High Priority Message Status 3 */
#define CAN1_N3_HPMS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_HPMS*)0xF0218E94u)
/** Alias (User Manual Name) for CAN1_N3_HPMS.
* To use register names with standard convension, please use CAN1_N3_HPMS.
*/
#define CAN1_HPMS3 (CAN1_N3_HPMS)

/** \brief E98, New Data 1 3 */
#define CAN1_N3_NDAT1 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NDAT1*)0xF0218E98u)
/** Alias (User Manual Name) for CAN1_N3_NDAT1.
* To use register names with standard convension, please use CAN1_N3_NDAT1.
*/
#define CAN1_NDAT13 (CAN1_N3_NDAT1)

/** \brief E9C, New Data 2 3 */
#define CAN1_N3_NDAT2 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NDAT2*)0xF0218E9Cu)
/** Alias (User Manual Name) for CAN1_N3_NDAT2.
* To use register names with standard convension, please use CAN1_N3_NDAT2.
*/
#define CAN1_NDAT23 (CAN1_N3_NDAT2)

/** \brief EA0, Rx FIFO 0 Configuration 3 */
#define CAN1_N3_RX_F0C /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0C*)0xF0218EA0u)
/** Alias (User Manual Name) for CAN1_N3_RX_F0C.
* To use register names with standard convension, please use CAN1_N3_RX_F0C.
*/
#define CAN1_RXF0C3 (CAN1_N3_RX_F0C)

/** \brief EA4, Rx FIFO 0 Status 3 */
#define CAN1_N3_RX_F0S /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0S*)0xF0218EA4u)
/** Alias (User Manual Name) for CAN1_N3_RX_F0S.
* To use register names with standard convension, please use CAN1_N3_RX_F0S.
*/
#define CAN1_RXF0S3 (CAN1_N3_RX_F0S)

/** \brief EA8, Rx FIFO 0 Acknowledge 3 */
#define CAN1_N3_RX_F0A /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0A*)0xF0218EA8u)
/** Alias (User Manual Name) for CAN1_N3_RX_F0A.
* To use register names with standard convension, please use CAN1_N3_RX_F0A.
*/
#define CAN1_RXF0A3 (CAN1_N3_RX_F0A)

/** \brief EAC, Rx Buffer Configuration 3 */
#define CAN1_N3_RX_BC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_BC*)0xF0218EACu)
/** Alias (User Manual Name) for CAN1_N3_RX_BC.
* To use register names with standard convension, please use CAN1_N3_RX_BC.
*/
#define CAN1_RXBC3 (CAN1_N3_RX_BC)

/** \brief EB0, Rx FIFO 1 Configuration 3 */
#define CAN1_N3_RX_F1C /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1C*)0xF0218EB0u)
/** Alias (User Manual Name) for CAN1_N3_RX_F1C.
* To use register names with standard convension, please use CAN1_N3_RX_F1C.
*/
#define CAN1_RXF1C3 (CAN1_N3_RX_F1C)

/** \brief EB4, Rx FIFO 1 Status 3 */
#define CAN1_N3_RX_F1S /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1S*)0xF0218EB4u)
/** Alias (User Manual Name) for CAN1_N3_RX_F1S.
* To use register names with standard convension, please use CAN1_N3_RX_F1S.
*/
#define CAN1_RXF1S3 (CAN1_N3_RX_F1S)

/** \brief EB8, Rx FIFO 1 Acknowledge 3 */
#define CAN1_N3_RX_F1A /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1A*)0xF0218EB8u)
/** Alias (User Manual Name) for CAN1_N3_RX_F1A.
* To use register names with standard convension, please use CAN1_N3_RX_F1A.
*/
#define CAN1_RXF1A3 (CAN1_N3_RX_F1A)

/** \brief EBC, Rx Buffer/FIFO Element Size Configuration 3 */
#define CAN1_N3_RX_ESC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_ESC*)0xF0218EBCu)
/** Alias (User Manual Name) for CAN1_N3_RX_ESC.
* To use register names with standard convension, please use CAN1_N3_RX_ESC.
*/
#define CAN1_RXESC3 (CAN1_N3_RX_ESC)

/** \brief EC0, Tx Buffer Configuration 3 */
#define CAN1_N3_TX_BC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BC*)0xF0218EC0u)
/** Alias (User Manual Name) for CAN1_N3_TX_BC.
* To use register names with standard convension, please use CAN1_N3_TX_BC.
*/
#define CAN1_TXBC3 (CAN1_N3_TX_BC)

/** \brief EC4, Tx FIFO/Queue Status 3 */
#define CAN1_N3_TX_FQS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_FQS*)0xF0218EC4u)
/** Alias (User Manual Name) for CAN1_N3_TX_FQS.
* To use register names with standard convension, please use CAN1_N3_TX_FQS.
*/
#define CAN1_TXFQS3 (CAN1_N3_TX_FQS)

/** \brief EC8, Tx Buffer Element Size Configuration 3 */
#define CAN1_N3_TX_ESC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_ESC*)0xF0218EC8u)
/** Alias (User Manual Name) for CAN1_N3_TX_ESC.
* To use register names with standard convension, please use CAN1_N3_TX_ESC.
*/
#define CAN1_TXESC3 (CAN1_N3_TX_ESC)

/** \brief ECC, Tx Buffer Request Pending 3 */
#define CAN1_N3_TX_BRP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BRP*)0xF0218ECCu)
/** Alias (User Manual Name) for CAN1_N3_TX_BRP.
* To use register names with standard convension, please use CAN1_N3_TX_BRP.
*/
#define CAN1_TXBRP3 (CAN1_N3_TX_BRP)

/** \brief ED0, Tx Buffer Add Request 3 */
#define CAN1_N3_TX_BAR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BAR*)0xF0218ED0u)
/** Alias (User Manual Name) for CAN1_N3_TX_BAR.
* To use register names with standard convension, please use CAN1_N3_TX_BAR.
*/
#define CAN1_TXBAR3 (CAN1_N3_TX_BAR)

/** \brief ED4, Tx Buffer Cancellation Request 3 */
#define CAN1_N3_TX_BCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCR*)0xF0218ED4u)
/** Alias (User Manual Name) for CAN1_N3_TX_BCR.
* To use register names with standard convension, please use CAN1_N3_TX_BCR.
*/
#define CAN1_TXBCR3 (CAN1_N3_TX_BCR)

/** \brief ED8, Tx Buffer Transmission Occurred 3 */
#define CAN1_N3_TX_BTO /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BTO*)0xF0218ED8u)
/** Alias (User Manual Name) for CAN1_N3_TX_BTO.
* To use register names with standard convension, please use CAN1_N3_TX_BTO.
*/
#define CAN1_TXBTO3 (CAN1_N3_TX_BTO)

/** \brief EDC, Tx Buffer Cancellation Finished 3 */
#define CAN1_N3_TX_BCF /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCF*)0xF0218EDCu)
/** Alias (User Manual Name) for CAN1_N3_TX_BCF.
* To use register names with standard convension, please use CAN1_N3_TX_BCF.
*/
#define CAN1_TXBCF3 (CAN1_N3_TX_BCF)

/** \brief EE0, Tx Buffer Transmission Interrupt Enable 3 */
#define CAN1_N3_TX_BTIE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BTIE*)0xF0218EE0u)
/** Alias (User Manual Name) for CAN1_N3_TX_BTIE.
* To use register names with standard convension, please use CAN1_N3_TX_BTIE.
*/
#define CAN1_TXBTIE3 (CAN1_N3_TX_BTIE)

/** \brief EE4, Tx Buffer Cancellation Finished Interrupt Enable 3 */
#define CAN1_N3_TX_BCIE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCIE*)0xF0218EE4u)
/** Alias (User Manual Name) for CAN1_N3_TX_BCIE.
* To use register names with standard convension, please use CAN1_N3_TX_BCIE.
*/
#define CAN1_TXBCIE3 (CAN1_N3_TX_BCIE)

/** \brief EF0, Tx Event FIFO Configuration 3 */
#define CAN1_N3_TX_EFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFC*)0xF0218EF0u)
/** Alias (User Manual Name) for CAN1_N3_TX_EFC.
* To use register names with standard convension, please use CAN1_N3_TX_EFC.
*/
#define CAN1_TXEFC3 (CAN1_N3_TX_EFC)

/** \brief EF4, Tx Event FIFO Status 3 */
#define CAN1_N3_TX_EFS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFS*)0xF0218EF4u)
/** Alias (User Manual Name) for CAN1_N3_TX_EFS.
* To use register names with standard convension, please use CAN1_N3_TX_EFS.
*/
#define CAN1_TXEFS3 (CAN1_N3_TX_EFS)

/** \brief EF8, Tx Event FIFO Acknowledge 3 */
#define CAN1_N3_TX_EFA /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFA*)0xF0218EF8u)
/** Alias (User Manual Name) for CAN1_N3_TX_EFA.
* To use register names with standard convension, please use CAN1_N3_TX_EFA.
*/
#define CAN1_TXEFA3 (CAN1_N3_TX_EFA)

/******************************************************************************/
/******************************************************************************/
/** \addtogroup IfxSfr_Can_Registers_Cfg_Can2
 * \{  */
/** \brief 0, Embedded SRAM for messages */
#define CAN2_RAM ((void*)0xF0220000u)
#define CAN2_RAM_SIZE (0x4000u)
/** \brief 0, CAN Clock Control Register */
#define CAN2_CLC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_CLC*)0xF0228000u)

/** \brief 8, Module Identification Register */
#define CAN2_ID /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_ID*)0xF0228008u)

/** \brief 30, Module Control Register */
#define CAN2_MCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_MCR*)0xF0228030u)

/** \brief DC, Access Enable Register Control 0 */
#define CAN2_ACCENCTR0 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_ACCENCTR0*)0xF02280DCu)

/** \brief E8, OCDS Control and Status */
#define CAN2_OCS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_OCS*)0xF02280E8u)

/** \brief EC, Kernel Reset Status Clear Register */
#define CAN2_KRSTCLR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_KRSTCLR*)0xF02280ECu)

/** \brief F0, Kernel Reset Register 1 */
#define CAN2_KRST1 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_KRST1*)0xF02280F0u)

/** \brief F4, Kernel Reset Register 0 */
#define CAN2_KRST0 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_KRST0*)0xF02280F4u)

/** \brief FC, Access Enable Register 0 */
#define CAN2_ACCEN0 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_ACCEN0*)0xF02280FCu)

/** \brief 100, Access Enable Register CAN Node 0 0 */
#define CAN2_N0_ACCENNODE0 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ACCENNODE0*)0xF0228100u)
/** Alias (User Manual Name) for CAN2_N0_ACCENNODE0.
* To use register names with standard convension, please use CAN2_N0_ACCENNODE0.
*/
#define CAN2_ACCENNODE00 (CAN2_N0_ACCENNODE0)

/** \brief 108, Start Address Node 0 */
#define CAN2_N0_STARTADR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_STARTADR*)0xF0228108u)
/** Alias (User Manual Name) for CAN2_N0_STARTADR.
* To use register names with standard convension, please use CAN2_N0_STARTADR.
*/
#define CAN2_STARTADR0 (CAN2_N0_STARTADR)

/** \brief 10C, End Address Node 0 */
#define CAN2_N0_ENDADR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ENDADR*)0xF022810Cu)
/** Alias (User Manual Name) for CAN2_N0_ENDADR.
* To use register names with standard convension, please use CAN2_N0_ENDADR.
*/
#define CAN2_ENDADR0 (CAN2_N0_ENDADR)

/** \brief 110, Interrupt Signalling Register 0 */
#define CAN2_N0_ISREG /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ISREG*)0xF0228110u)
/** Alias (User Manual Name) for CAN2_N0_ISREG.
* To use register names with standard convension, please use CAN2_N0_ISREG.
*/
#define CAN2_ISREG0 (CAN2_N0_ISREG)

/** \brief 114, Interrupt routing for Groups 1 0 */
#define CAN2_N0_GRINT1 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GRINT1*)0xF0228114u)
/** Alias (User Manual Name) for CAN2_N0_GRINT1.
* To use register names with standard convension, please use CAN2_N0_GRINT1.
*/
#define CAN2_GRINT10 (CAN2_N0_GRINT1)

/** \brief 118, Interrupt routing for Groups 2 0 */
#define CAN2_N0_GRINT2 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GRINT2*)0xF0228118u)
/** Alias (User Manual Name) for CAN2_N0_GRINT2.
* To use register names with standard convension, please use CAN2_N0_GRINT2.
*/
#define CAN2_GRINT20 (CAN2_N0_GRINT2)

/** \brief 120, Node 0 Timer Clock Control Register */
#define CAN2_N0_NT_CCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_CCR*)0xF0228120u)
/** Alias (User Manual Name) for CAN2_N0_NT_CCR.
* To use register names with standard convension, please use CAN2_N0_NT_CCR.
*/
#define CAN2_NTCCR0 (CAN2_N0_NT_CCR)

/** \brief 124, Node 0 Timer A Transmit Trigger Register */
#define CAN2_N0_NT_ATTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_ATTR*)0xF0228124u)
/** Alias (User Manual Name) for CAN2_N0_NT_ATTR.
* To use register names with standard convension, please use CAN2_N0_NT_ATTR.
*/
#define CAN2_NTATTR0 (CAN2_N0_NT_ATTR)

/** \brief 128, Node 0 Timer B Transmit Trigger Register */
#define CAN2_N0_NT_BTTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_BTTR*)0xF0228128u)
/** Alias (User Manual Name) for CAN2_N0_NT_BTTR.
* To use register names with standard convension, please use CAN2_N0_NT_BTTR.
*/
#define CAN2_NTBTTR0 (CAN2_N0_NT_BTTR)

/** \brief 12C, Node 0 Timer C Transmit Trigger Register */
#define CAN2_N0_NT_CTTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_CTTR*)0xF022812Cu)
/** Alias (User Manual Name) for CAN2_N0_NT_CTTR.
* To use register names with standard convension, please use CAN2_N0_NT_CTTR.
*/
#define CAN2_NTCTTR0 (CAN2_N0_NT_CTTR)

/** \brief 130, Node 0 Timer Receive Timeout Register */
#define CAN2_N0_NT_RTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_RTR*)0xF0228130u)
/** Alias (User Manual Name) for CAN2_N0_NT_RTR.
* To use register names with standard convension, please use CAN2_N0_NT_RTR.
*/
#define CAN2_NTRTR0 (CAN2_N0_NT_RTR)

/** \brief 140, Node 0 Port Control Register */
#define CAN2_N0_NPCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NPCR*)0xF0228140u)
/** Alias (User Manual Name) for CAN2_N0_NPCR.
* To use register names with standard convension, please use CAN2_N0_NPCR.
*/
#define CAN2_NPCR0 (CAN2_N0_NPCR)

/** \brief 200, Core Release Register 0 */
#define CAN2_N0_CREL /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_CREL*)0xF0228200u)
/** Alias (User Manual Name) for CAN2_N0_CREL.
* To use register names with standard convension, please use CAN2_N0_CREL.
*/
#define CAN2_CREL0 (CAN2_N0_CREL)

/** \brief 204, Endian Register 0 */
#define CAN2_N0_ENDN /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ENDN*)0xF0228204u)
/** Alias (User Manual Name) for CAN2_N0_ENDN.
* To use register names with standard convension, please use CAN2_N0_ENDN.
*/
#define CAN2_ENDN0 (CAN2_N0_ENDN)

/** \brief 20C, Data Bit Timing & Prescaler Register 0 */
#define CAN2_N0_DBTP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_DBTP*)0xF022820Cu)
/** Alias (User Manual Name) for CAN2_N0_DBTP.
* To use register names with standard convension, please use CAN2_N0_DBTP.
*/
#define CAN2_DBTP0 (CAN2_N0_DBTP)

/** \brief 210, Test Register 0 */
#define CAN2_N0_TEST /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TEST*)0xF0228210u)
/** Alias (User Manual Name) for CAN2_N0_TEST.
* To use register names with standard convension, please use CAN2_N0_TEST.
*/
#define CAN2_TEST0 (CAN2_N0_TEST)

/** \brief 214, RAM Watchdog 0 */
#define CAN2_N0_RWD /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RWD*)0xF0228214u)
/** Alias (User Manual Name) for CAN2_N0_RWD.
* To use register names with standard convension, please use CAN2_N0_RWD.
*/
#define CAN2_RWD0 (CAN2_N0_RWD)

/** \brief 218, CC Control Register 0 */
#define CAN2_N0_CCCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_CCCR*)0xF0228218u)
/** Alias (User Manual Name) for CAN2_N0_CCCR.
* To use register names with standard convension, please use CAN2_N0_CCCR.
*/
#define CAN2_CCCR0 (CAN2_N0_CCCR)

/** \brief 21C, Nominal Bit Timing & Prescaler Register 0 */
#define CAN2_N0_NBTP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NBTP*)0xF022821Cu)
/** Alias (User Manual Name) for CAN2_N0_NBTP.
* To use register names with standard convension, please use CAN2_N0_NBTP.
*/
#define CAN2_NBTP0 (CAN2_N0_NBTP)

/** \brief 220, Timestamp Counter Configuration 0 */
#define CAN2_N0_TSCC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TSCC*)0xF0228220u)
/** Alias (User Manual Name) for CAN2_N0_TSCC.
* To use register names with standard convension, please use CAN2_N0_TSCC.
*/
#define CAN2_TSCC0 (CAN2_N0_TSCC)

/** \brief 224, Timestamp Counter Value 0 */
#define CAN2_N0_TSCV /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TSCV*)0xF0228224u)
/** Alias (User Manual Name) for CAN2_N0_TSCV.
* To use register names with standard convension, please use CAN2_N0_TSCV.
*/
#define CAN2_TSCV0 (CAN2_N0_TSCV)

/** \brief 228, Timeout Counter Configuration 0 */
#define CAN2_N0_TOCC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TOCC*)0xF0228228u)
/** Alias (User Manual Name) for CAN2_N0_TOCC.
* To use register names with standard convension, please use CAN2_N0_TOCC.
*/
#define CAN2_TOCC0 (CAN2_N0_TOCC)

/** \brief 22C, Timeout Counter Value 0 */
#define CAN2_N0_TOCV /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TOCV*)0xF022822Cu)
/** Alias (User Manual Name) for CAN2_N0_TOCV.
* To use register names with standard convension, please use CAN2_N0_TOCV.
*/
#define CAN2_TOCV0 (CAN2_N0_TOCV)

/** \brief 240, Error Counter Register 0 */
#define CAN2_N0_ECR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ECR*)0xF0228240u)
/** Alias (User Manual Name) for CAN2_N0_ECR.
* To use register names with standard convension, please use CAN2_N0_ECR.
*/
#define CAN2_ECR0 (CAN2_N0_ECR)

/** \brief 244, Protocol Status Register 0 */
#define CAN2_N0_PSR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_PSR*)0xF0228244u)
/** Alias (User Manual Name) for CAN2_N0_PSR.
* To use register names with standard convension, please use CAN2_N0_PSR.
*/
#define CAN2_PSR0 (CAN2_N0_PSR)

/** \brief 248, Transmitter Delay Compensation Register 0 */
#define CAN2_N0_TDCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TDCR*)0xF0228248u)
/** Alias (User Manual Name) for CAN2_N0_TDCR.
* To use register names with standard convension, please use CAN2_N0_TDCR.
*/
#define CAN2_TDCR0 (CAN2_N0_TDCR)

/** \brief 250, Interrupt Register 0 */
#define CAN2_N0_IR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_IR*)0xF0228250u)
/** Alias (User Manual Name) for CAN2_N0_IR.
* To use register names with standard convension, please use CAN2_N0_IR.
*/
#define CAN2_IR0 (CAN2_N0_IR)

/** \brief 254, Interrupt Enable 0 */
#define CAN2_N0_IE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_IE*)0xF0228254u)
/** Alias (User Manual Name) for CAN2_N0_IE.
* To use register names with standard convension, please use CAN2_N0_IE.
*/
#define CAN2_IE0 (CAN2_N0_IE)

/** \brief 280, Global Filter Configuration 0 */
#define CAN2_N0_GFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GFC*)0xF0228280u)
/** Alias (User Manual Name) for CAN2_N0_GFC.
* To use register names with standard convension, please use CAN2_N0_GFC.
*/
#define CAN2_GFC0 (CAN2_N0_GFC)

/** \brief 284, Standard ID Filter Configuration 0 */
#define CAN2_N0_SIDFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_SIDFC*)0xF0228284u)
/** Alias (User Manual Name) for CAN2_N0_SIDFC.
* To use register names with standard convension, please use CAN2_N0_SIDFC.
*/
#define CAN2_SIDFC0 (CAN2_N0_SIDFC)

/** \brief 288, Extended ID Filter Configuration 0 */
#define CAN2_N0_XIDFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_XIDFC*)0xF0228288u)
/** Alias (User Manual Name) for CAN2_N0_XIDFC.
* To use register names with standard convension, please use CAN2_N0_XIDFC.
*/
#define CAN2_XIDFC0 (CAN2_N0_XIDFC)

/** \brief 290, Extended ID AND Mask 0 */
#define CAN2_N0_XIDAM /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_XIDAM*)0xF0228290u)
/** Alias (User Manual Name) for CAN2_N0_XIDAM.
* To use register names with standard convension, please use CAN2_N0_XIDAM.
*/
#define CAN2_XIDAM0 (CAN2_N0_XIDAM)

/** \brief 294, High Priority Message Status 0 */
#define CAN2_N0_HPMS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_HPMS*)0xF0228294u)
/** Alias (User Manual Name) for CAN2_N0_HPMS.
* To use register names with standard convension, please use CAN2_N0_HPMS.
*/
#define CAN2_HPMS0 (CAN2_N0_HPMS)

/** \brief 298, New Data 1 0 */
#define CAN2_N0_NDAT1 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NDAT1*)0xF0228298u)
/** Alias (User Manual Name) for CAN2_N0_NDAT1.
* To use register names with standard convension, please use CAN2_N0_NDAT1.
*/
#define CAN2_NDAT10 (CAN2_N0_NDAT1)

/** \brief 29C, New Data 2 0 */
#define CAN2_N0_NDAT2 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NDAT2*)0xF022829Cu)
/** Alias (User Manual Name) for CAN2_N0_NDAT2.
* To use register names with standard convension, please use CAN2_N0_NDAT2.
*/
#define CAN2_NDAT20 (CAN2_N0_NDAT2)

/** \brief 2A0, Rx FIFO 0 Configuration 0 */
#define CAN2_N0_RX_F0C /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0C*)0xF02282A0u)
/** Alias (User Manual Name) for CAN2_N0_RX_F0C.
* To use register names with standard convension, please use CAN2_N0_RX_F0C.
*/
#define CAN2_RXF0C0 (CAN2_N0_RX_F0C)

/** \brief 2A4, Rx FIFO 0 Status 0 */
#define CAN2_N0_RX_F0S /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0S*)0xF02282A4u)
/** Alias (User Manual Name) for CAN2_N0_RX_F0S.
* To use register names with standard convension, please use CAN2_N0_RX_F0S.
*/
#define CAN2_RXF0S0 (CAN2_N0_RX_F0S)

/** \brief 2A8, Rx FIFO 0 Acknowledge 0 */
#define CAN2_N0_RX_F0A /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0A*)0xF02282A8u)
/** Alias (User Manual Name) for CAN2_N0_RX_F0A.
* To use register names with standard convension, please use CAN2_N0_RX_F0A.
*/
#define CAN2_RXF0A0 (CAN2_N0_RX_F0A)

/** \brief 2AC, Rx Buffer Configuration 0 */
#define CAN2_N0_RX_BC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_BC*)0xF02282ACu)
/** Alias (User Manual Name) for CAN2_N0_RX_BC.
* To use register names with standard convension, please use CAN2_N0_RX_BC.
*/
#define CAN2_RXBC0 (CAN2_N0_RX_BC)

/** \brief 2B0, Rx FIFO 1 Configuration 0 */
#define CAN2_N0_RX_F1C /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1C*)0xF02282B0u)
/** Alias (User Manual Name) for CAN2_N0_RX_F1C.
* To use register names with standard convension, please use CAN2_N0_RX_F1C.
*/
#define CAN2_RXF1C0 (CAN2_N0_RX_F1C)

/** \brief 2B4, Rx FIFO 1 Status 0 */
#define CAN2_N0_RX_F1S /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1S*)0xF02282B4u)
/** Alias (User Manual Name) for CAN2_N0_RX_F1S.
* To use register names with standard convension, please use CAN2_N0_RX_F1S.
*/
#define CAN2_RXF1S0 (CAN2_N0_RX_F1S)

/** \brief 2B8, Rx FIFO 1 Acknowledge 0 */
#define CAN2_N0_RX_F1A /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1A*)0xF02282B8u)
/** Alias (User Manual Name) for CAN2_N0_RX_F1A.
* To use register names with standard convension, please use CAN2_N0_RX_F1A.
*/
#define CAN2_RXF1A0 (CAN2_N0_RX_F1A)

/** \brief 2BC, Rx Buffer/FIFO Element Size Configuration 0 */
#define CAN2_N0_RX_ESC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_ESC*)0xF02282BCu)
/** Alias (User Manual Name) for CAN2_N0_RX_ESC.
* To use register names with standard convension, please use CAN2_N0_RX_ESC.
*/
#define CAN2_RXESC0 (CAN2_N0_RX_ESC)

/** \brief 2C0, Tx Buffer Configuration 0 */
#define CAN2_N0_TX_BC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BC*)0xF02282C0u)
/** Alias (User Manual Name) for CAN2_N0_TX_BC.
* To use register names with standard convension, please use CAN2_N0_TX_BC.
*/
#define CAN2_TXBC0 (CAN2_N0_TX_BC)

/** \brief 2C4, Tx FIFO/Queue Status 0 */
#define CAN2_N0_TX_FQS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_FQS*)0xF02282C4u)
/** Alias (User Manual Name) for CAN2_N0_TX_FQS.
* To use register names with standard convension, please use CAN2_N0_TX_FQS.
*/
#define CAN2_TXFQS0 (CAN2_N0_TX_FQS)

/** \brief 2C8, Tx Buffer Element Size Configuration 0 */
#define CAN2_N0_TX_ESC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_ESC*)0xF02282C8u)
/** Alias (User Manual Name) for CAN2_N0_TX_ESC.
* To use register names with standard convension, please use CAN2_N0_TX_ESC.
*/
#define CAN2_TXESC0 (CAN2_N0_TX_ESC)

/** \brief 2CC, Tx Buffer Request Pending 0 */
#define CAN2_N0_TX_BRP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BRP*)0xF02282CCu)
/** Alias (User Manual Name) for CAN2_N0_TX_BRP.
* To use register names with standard convension, please use CAN2_N0_TX_BRP.
*/
#define CAN2_TXBRP0 (CAN2_N0_TX_BRP)

/** \brief 2D0, Tx Buffer Add Request 0 */
#define CAN2_N0_TX_BAR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BAR*)0xF02282D0u)
/** Alias (User Manual Name) for CAN2_N0_TX_BAR.
* To use register names with standard convension, please use CAN2_N0_TX_BAR.
*/
#define CAN2_TXBAR0 (CAN2_N0_TX_BAR)

/** \brief 2D4, Tx Buffer Cancellation Request 0 */
#define CAN2_N0_TX_BCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCR*)0xF02282D4u)
/** Alias (User Manual Name) for CAN2_N0_TX_BCR.
* To use register names with standard convension, please use CAN2_N0_TX_BCR.
*/
#define CAN2_TXBCR0 (CAN2_N0_TX_BCR)

/** \brief 2D8, Tx Buffer Transmission Occurred 0 */
#define CAN2_N0_TX_BTO /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BTO*)0xF02282D8u)
/** Alias (User Manual Name) for CAN2_N0_TX_BTO.
* To use register names with standard convension, please use CAN2_N0_TX_BTO.
*/
#define CAN2_TXBTO0 (CAN2_N0_TX_BTO)

/** \brief 2DC, Tx Buffer Cancellation Finished 0 */
#define CAN2_N0_TX_BCF /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCF*)0xF02282DCu)
/** Alias (User Manual Name) for CAN2_N0_TX_BCF.
* To use register names with standard convension, please use CAN2_N0_TX_BCF.
*/
#define CAN2_TXBCF0 (CAN2_N0_TX_BCF)

/** \brief 2E0, Tx Buffer Transmission Interrupt Enable 0 */
#define CAN2_N0_TX_BTIE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BTIE*)0xF02282E0u)
/** Alias (User Manual Name) for CAN2_N0_TX_BTIE.
* To use register names with standard convension, please use CAN2_N0_TX_BTIE.
*/
#define CAN2_TXBTIE0 (CAN2_N0_TX_BTIE)

/** \brief 2E4, Tx Buffer Cancellation Finished Interrupt Enable 0 */
#define CAN2_N0_TX_BCIE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCIE*)0xF02282E4u)
/** Alias (User Manual Name) for CAN2_N0_TX_BCIE.
* To use register names with standard convension, please use CAN2_N0_TX_BCIE.
*/
#define CAN2_TXBCIE0 (CAN2_N0_TX_BCIE)

/** \brief 2F0, Tx Event FIFO Configuration 0 */
#define CAN2_N0_TX_EFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFC*)0xF02282F0u)
/** Alias (User Manual Name) for CAN2_N0_TX_EFC.
* To use register names with standard convension, please use CAN2_N0_TX_EFC.
*/
#define CAN2_TXEFC0 (CAN2_N0_TX_EFC)

/** \brief 2F4, Tx Event FIFO Status 0 */
#define CAN2_N0_TX_EFS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFS*)0xF02282F4u)
/** Alias (User Manual Name) for CAN2_N0_TX_EFS.
* To use register names with standard convension, please use CAN2_N0_TX_EFS.
*/
#define CAN2_TXEFS0 (CAN2_N0_TX_EFS)

/** \brief 2F8, Tx Event FIFO Acknowledge 0 */
#define CAN2_N0_TX_EFA /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFA*)0xF02282F8u)
/** Alias (User Manual Name) for CAN2_N0_TX_EFA.
* To use register names with standard convension, please use CAN2_N0_TX_EFA.
*/
#define CAN2_TXEFA0 (CAN2_N0_TX_EFA)

/** \brief 500, Access Enable Register CAN Node 1 0 */
#define CAN2_N1_ACCENNODE0 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ACCENNODE0*)0xF0228500u)
/** Alias (User Manual Name) for CAN2_N1_ACCENNODE0.
* To use register names with standard convension, please use CAN2_N1_ACCENNODE0.
*/
#define CAN2_ACCENNODE10 (CAN2_N1_ACCENNODE0)

/** \brief 508, Start Address Node 1 */
#define CAN2_N1_STARTADR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_STARTADR*)0xF0228508u)
/** Alias (User Manual Name) for CAN2_N1_STARTADR.
* To use register names with standard convension, please use CAN2_N1_STARTADR.
*/
#define CAN2_STARTADR1 (CAN2_N1_STARTADR)

/** \brief 50C, End Address Node 1 */
#define CAN2_N1_ENDADR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ENDADR*)0xF022850Cu)
/** Alias (User Manual Name) for CAN2_N1_ENDADR.
* To use register names with standard convension, please use CAN2_N1_ENDADR.
*/
#define CAN2_ENDADR1 (CAN2_N1_ENDADR)

/** \brief 510, Interrupt Signalling Register 1 */
#define CAN2_N1_ISREG /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ISREG*)0xF0228510u)
/** Alias (User Manual Name) for CAN2_N1_ISREG.
* To use register names with standard convension, please use CAN2_N1_ISREG.
*/
#define CAN2_ISREG1 (CAN2_N1_ISREG)

/** \brief 514, Interrupt routing for Groups 1 1 */
#define CAN2_N1_GRINT1 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GRINT1*)0xF0228514u)
/** Alias (User Manual Name) for CAN2_N1_GRINT1.
* To use register names with standard convension, please use CAN2_N1_GRINT1.
*/
#define CAN2_GRINT11 (CAN2_N1_GRINT1)

/** \brief 518, Interrupt routing for Groups 2 1 */
#define CAN2_N1_GRINT2 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GRINT2*)0xF0228518u)
/** Alias (User Manual Name) for CAN2_N1_GRINT2.
* To use register names with standard convension, please use CAN2_N1_GRINT2.
*/
#define CAN2_GRINT21 (CAN2_N1_GRINT2)

/** \brief 520, Node 1 Timer Clock Control Register */
#define CAN2_N1_NT_CCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_CCR*)0xF0228520u)
/** Alias (User Manual Name) for CAN2_N1_NT_CCR.
* To use register names with standard convension, please use CAN2_N1_NT_CCR.
*/
#define CAN2_NTCCR1 (CAN2_N1_NT_CCR)

/** \brief 524, Node 1 Timer A Transmit Trigger Register */
#define CAN2_N1_NT_ATTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_ATTR*)0xF0228524u)
/** Alias (User Manual Name) for CAN2_N1_NT_ATTR.
* To use register names with standard convension, please use CAN2_N1_NT_ATTR.
*/
#define CAN2_NTATTR1 (CAN2_N1_NT_ATTR)

/** \brief 528, Node 1 Timer B Transmit Trigger Register */
#define CAN2_N1_NT_BTTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_BTTR*)0xF0228528u)
/** Alias (User Manual Name) for CAN2_N1_NT_BTTR.
* To use register names with standard convension, please use CAN2_N1_NT_BTTR.
*/
#define CAN2_NTBTTR1 (CAN2_N1_NT_BTTR)

/** \brief 52C, Node 1 Timer C Transmit Trigger Register */
#define CAN2_N1_NT_CTTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_CTTR*)0xF022852Cu)
/** Alias (User Manual Name) for CAN2_N1_NT_CTTR.
* To use register names with standard convension, please use CAN2_N1_NT_CTTR.
*/
#define CAN2_NTCTTR1 (CAN2_N1_NT_CTTR)

/** \brief 530, Node 1 Timer Receive Timeout Register */
#define CAN2_N1_NT_RTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_RTR*)0xF0228530u)
/** Alias (User Manual Name) for CAN2_N1_NT_RTR.
* To use register names with standard convension, please use CAN2_N1_NT_RTR.
*/
#define CAN2_NTRTR1 (CAN2_N1_NT_RTR)

/** \brief 540, Node 1 Port Control Register */
#define CAN2_N1_NPCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NPCR*)0xF0228540u)
/** Alias (User Manual Name) for CAN2_N1_NPCR.
* To use register names with standard convension, please use CAN2_N1_NPCR.
*/
#define CAN2_NPCR1 (CAN2_N1_NPCR)

/** \brief 600, Core Release Register 1 */
#define CAN2_N1_CREL /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_CREL*)0xF0228600u)
/** Alias (User Manual Name) for CAN2_N1_CREL.
* To use register names with standard convension, please use CAN2_N1_CREL.
*/
#define CAN2_CREL1 (CAN2_N1_CREL)

/** \brief 604, Endian Register 1 */
#define CAN2_N1_ENDN /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ENDN*)0xF0228604u)
/** Alias (User Manual Name) for CAN2_N1_ENDN.
* To use register names with standard convension, please use CAN2_N1_ENDN.
*/
#define CAN2_ENDN1 (CAN2_N1_ENDN)

/** \brief 60C, Data Bit Timing & Prescaler Register 1 */
#define CAN2_N1_DBTP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_DBTP*)0xF022860Cu)
/** Alias (User Manual Name) for CAN2_N1_DBTP.
* To use register names with standard convension, please use CAN2_N1_DBTP.
*/
#define CAN2_DBTP1 (CAN2_N1_DBTP)

/** \brief 610, Test Register 1 */
#define CAN2_N1_TEST /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TEST*)0xF0228610u)
/** Alias (User Manual Name) for CAN2_N1_TEST.
* To use register names with standard convension, please use CAN2_N1_TEST.
*/
#define CAN2_TEST1 (CAN2_N1_TEST)

/** \brief 614, RAM Watchdog 1 */
#define CAN2_N1_RWD /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RWD*)0xF0228614u)
/** Alias (User Manual Name) for CAN2_N1_RWD.
* To use register names with standard convension, please use CAN2_N1_RWD.
*/
#define CAN2_RWD1 (CAN2_N1_RWD)

/** \brief 618, CC Control Register 1 */
#define CAN2_N1_CCCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_CCCR*)0xF0228618u)
/** Alias (User Manual Name) for CAN2_N1_CCCR.
* To use register names with standard convension, please use CAN2_N1_CCCR.
*/
#define CAN2_CCCR1 (CAN2_N1_CCCR)

/** \brief 61C, Nominal Bit Timing & Prescaler Register 1 */
#define CAN2_N1_NBTP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NBTP*)0xF022861Cu)
/** Alias (User Manual Name) for CAN2_N1_NBTP.
* To use register names with standard convension, please use CAN2_N1_NBTP.
*/
#define CAN2_NBTP1 (CAN2_N1_NBTP)

/** \brief 620, Timestamp Counter Configuration 1 */
#define CAN2_N1_TSCC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TSCC*)0xF0228620u)
/** Alias (User Manual Name) for CAN2_N1_TSCC.
* To use register names with standard convension, please use CAN2_N1_TSCC.
*/
#define CAN2_TSCC1 (CAN2_N1_TSCC)

/** \brief 624, Timestamp Counter Value 1 */
#define CAN2_N1_TSCV /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TSCV*)0xF0228624u)
/** Alias (User Manual Name) for CAN2_N1_TSCV.
* To use register names with standard convension, please use CAN2_N1_TSCV.
*/
#define CAN2_TSCV1 (CAN2_N1_TSCV)

/** \brief 628, Timeout Counter Configuration 1 */
#define CAN2_N1_TOCC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TOCC*)0xF0228628u)
/** Alias (User Manual Name) for CAN2_N1_TOCC.
* To use register names with standard convension, please use CAN2_N1_TOCC.
*/
#define CAN2_TOCC1 (CAN2_N1_TOCC)

/** \brief 62C, Timeout Counter Value 1 */
#define CAN2_N1_TOCV /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TOCV*)0xF022862Cu)
/** Alias (User Manual Name) for CAN2_N1_TOCV.
* To use register names with standard convension, please use CAN2_N1_TOCV.
*/
#define CAN2_TOCV1 (CAN2_N1_TOCV)

/** \brief 640, Error Counter Register 1 */
#define CAN2_N1_ECR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ECR*)0xF0228640u)
/** Alias (User Manual Name) for CAN2_N1_ECR.
* To use register names with standard convension, please use CAN2_N1_ECR.
*/
#define CAN2_ECR1 (CAN2_N1_ECR)

/** \brief 644, Protocol Status Register 1 */
#define CAN2_N1_PSR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_PSR*)0xF0228644u)
/** Alias (User Manual Name) for CAN2_N1_PSR.
* To use register names with standard convension, please use CAN2_N1_PSR.
*/
#define CAN2_PSR1 (CAN2_N1_PSR)

/** \brief 648, Transmitter Delay Compensation Register 1 */
#define CAN2_N1_TDCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TDCR*)0xF0228648u)
/** Alias (User Manual Name) for CAN2_N1_TDCR.
* To use register names with standard convension, please use CAN2_N1_TDCR.
*/
#define CAN2_TDCR1 (CAN2_N1_TDCR)

/** \brief 650, Interrupt Register 1 */
#define CAN2_N1_IR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_IR*)0xF0228650u)
/** Alias (User Manual Name) for CAN2_N1_IR.
* To use register names with standard convension, please use CAN2_N1_IR.
*/
#define CAN2_IR1 (CAN2_N1_IR)

/** \brief 654, Interrupt Enable 1 */
#define CAN2_N1_IE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_IE*)0xF0228654u)
/** Alias (User Manual Name) for CAN2_N1_IE.
* To use register names with standard convension, please use CAN2_N1_IE.
*/
#define CAN2_IE1 (CAN2_N1_IE)

/** \brief 680, Global Filter Configuration 1 */
#define CAN2_N1_GFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GFC*)0xF0228680u)
/** Alias (User Manual Name) for CAN2_N1_GFC.
* To use register names with standard convension, please use CAN2_N1_GFC.
*/
#define CAN2_GFC1 (CAN2_N1_GFC)

/** \brief 684, Standard ID Filter Configuration 1 */
#define CAN2_N1_SIDFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_SIDFC*)0xF0228684u)
/** Alias (User Manual Name) for CAN2_N1_SIDFC.
* To use register names with standard convension, please use CAN2_N1_SIDFC.
*/
#define CAN2_SIDFC1 (CAN2_N1_SIDFC)

/** \brief 688, Extended ID Filter Configuration 1 */
#define CAN2_N1_XIDFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_XIDFC*)0xF0228688u)
/** Alias (User Manual Name) for CAN2_N1_XIDFC.
* To use register names with standard convension, please use CAN2_N1_XIDFC.
*/
#define CAN2_XIDFC1 (CAN2_N1_XIDFC)

/** \brief 690, Extended ID AND Mask 1 */
#define CAN2_N1_XIDAM /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_XIDAM*)0xF0228690u)
/** Alias (User Manual Name) for CAN2_N1_XIDAM.
* To use register names with standard convension, please use CAN2_N1_XIDAM.
*/
#define CAN2_XIDAM1 (CAN2_N1_XIDAM)

/** \brief 694, High Priority Message Status 1 */
#define CAN2_N1_HPMS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_HPMS*)0xF0228694u)
/** Alias (User Manual Name) for CAN2_N1_HPMS.
* To use register names with standard convension, please use CAN2_N1_HPMS.
*/
#define CAN2_HPMS1 (CAN2_N1_HPMS)

/** \brief 698, New Data 1 1 */
#define CAN2_N1_NDAT1 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NDAT1*)0xF0228698u)
/** Alias (User Manual Name) for CAN2_N1_NDAT1.
* To use register names with standard convension, please use CAN2_N1_NDAT1.
*/
#define CAN2_NDAT11 (CAN2_N1_NDAT1)

/** \brief 69C, New Data 2 1 */
#define CAN2_N1_NDAT2 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NDAT2*)0xF022869Cu)
/** Alias (User Manual Name) for CAN2_N1_NDAT2.
* To use register names with standard convension, please use CAN2_N1_NDAT2.
*/
#define CAN2_NDAT21 (CAN2_N1_NDAT2)

/** \brief 6A0, Rx FIFO 0 Configuration 1 */
#define CAN2_N1_RX_F0C /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0C*)0xF02286A0u)
/** Alias (User Manual Name) for CAN2_N1_RX_F0C.
* To use register names with standard convension, please use CAN2_N1_RX_F0C.
*/
#define CAN2_RXF0C1 (CAN2_N1_RX_F0C)

/** \brief 6A4, Rx FIFO 0 Status 1 */
#define CAN2_N1_RX_F0S /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0S*)0xF02286A4u)
/** Alias (User Manual Name) for CAN2_N1_RX_F0S.
* To use register names with standard convension, please use CAN2_N1_RX_F0S.
*/
#define CAN2_RXF0S1 (CAN2_N1_RX_F0S)

/** \brief 6A8, Rx FIFO 0 Acknowledge 1 */
#define CAN2_N1_RX_F0A /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0A*)0xF02286A8u)
/** Alias (User Manual Name) for CAN2_N1_RX_F0A.
* To use register names with standard convension, please use CAN2_N1_RX_F0A.
*/
#define CAN2_RXF0A1 (CAN2_N1_RX_F0A)

/** \brief 6AC, Rx Buffer Configuration 1 */
#define CAN2_N1_RX_BC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_BC*)0xF02286ACu)
/** Alias (User Manual Name) for CAN2_N1_RX_BC.
* To use register names with standard convension, please use CAN2_N1_RX_BC.
*/
#define CAN2_RXBC1 (CAN2_N1_RX_BC)

/** \brief 6B0, Rx FIFO 1 Configuration 1 */
#define CAN2_N1_RX_F1C /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1C*)0xF02286B0u)
/** Alias (User Manual Name) for CAN2_N1_RX_F1C.
* To use register names with standard convension, please use CAN2_N1_RX_F1C.
*/
#define CAN2_RXF1C1 (CAN2_N1_RX_F1C)

/** \brief 6B4, Rx FIFO 1 Status 1 */
#define CAN2_N1_RX_F1S /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1S*)0xF02286B4u)
/** Alias (User Manual Name) for CAN2_N1_RX_F1S.
* To use register names with standard convension, please use CAN2_N1_RX_F1S.
*/
#define CAN2_RXF1S1 (CAN2_N1_RX_F1S)

/** \brief 6B8, Rx FIFO 1 Acknowledge 1 */
#define CAN2_N1_RX_F1A /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1A*)0xF02286B8u)
/** Alias (User Manual Name) for CAN2_N1_RX_F1A.
* To use register names with standard convension, please use CAN2_N1_RX_F1A.
*/
#define CAN2_RXF1A1 (CAN2_N1_RX_F1A)

/** \brief 6BC, Rx Buffer/FIFO Element Size Configuration 1 */
#define CAN2_N1_RX_ESC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_ESC*)0xF02286BCu)
/** Alias (User Manual Name) for CAN2_N1_RX_ESC.
* To use register names with standard convension, please use CAN2_N1_RX_ESC.
*/
#define CAN2_RXESC1 (CAN2_N1_RX_ESC)

/** \brief 6C0, Tx Buffer Configuration 1 */
#define CAN2_N1_TX_BC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BC*)0xF02286C0u)
/** Alias (User Manual Name) for CAN2_N1_TX_BC.
* To use register names with standard convension, please use CAN2_N1_TX_BC.
*/
#define CAN2_TXBC1 (CAN2_N1_TX_BC)

/** \brief 6C4, Tx FIFO/Queue Status 1 */
#define CAN2_N1_TX_FQS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_FQS*)0xF02286C4u)
/** Alias (User Manual Name) for CAN2_N1_TX_FQS.
* To use register names with standard convension, please use CAN2_N1_TX_FQS.
*/
#define CAN2_TXFQS1 (CAN2_N1_TX_FQS)

/** \brief 6C8, Tx Buffer Element Size Configuration 1 */
#define CAN2_N1_TX_ESC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_ESC*)0xF02286C8u)
/** Alias (User Manual Name) for CAN2_N1_TX_ESC.
* To use register names with standard convension, please use CAN2_N1_TX_ESC.
*/
#define CAN2_TXESC1 (CAN2_N1_TX_ESC)

/** \brief 6CC, Tx Buffer Request Pending 1 */
#define CAN2_N1_TX_BRP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BRP*)0xF02286CCu)
/** Alias (User Manual Name) for CAN2_N1_TX_BRP.
* To use register names with standard convension, please use CAN2_N1_TX_BRP.
*/
#define CAN2_TXBRP1 (CAN2_N1_TX_BRP)

/** \brief 6D0, Tx Buffer Add Request 1 */
#define CAN2_N1_TX_BAR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BAR*)0xF02286D0u)
/** Alias (User Manual Name) for CAN2_N1_TX_BAR.
* To use register names with standard convension, please use CAN2_N1_TX_BAR.
*/
#define CAN2_TXBAR1 (CAN2_N1_TX_BAR)

/** \brief 6D4, Tx Buffer Cancellation Request 1 */
#define CAN2_N1_TX_BCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCR*)0xF02286D4u)
/** Alias (User Manual Name) for CAN2_N1_TX_BCR.
* To use register names with standard convension, please use CAN2_N1_TX_BCR.
*/
#define CAN2_TXBCR1 (CAN2_N1_TX_BCR)

/** \brief 6D8, Tx Buffer Transmission Occurred 1 */
#define CAN2_N1_TX_BTO /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BTO*)0xF02286D8u)
/** Alias (User Manual Name) for CAN2_N1_TX_BTO.
* To use register names with standard convension, please use CAN2_N1_TX_BTO.
*/
#define CAN2_TXBTO1 (CAN2_N1_TX_BTO)

/** \brief 6DC, Tx Buffer Cancellation Finished 1 */
#define CAN2_N1_TX_BCF /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCF*)0xF02286DCu)
/** Alias (User Manual Name) for CAN2_N1_TX_BCF.
* To use register names with standard convension, please use CAN2_N1_TX_BCF.
*/
#define CAN2_TXBCF1 (CAN2_N1_TX_BCF)

/** \brief 6E0, Tx Buffer Transmission Interrupt Enable 1 */
#define CAN2_N1_TX_BTIE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BTIE*)0xF02286E0u)
/** Alias (User Manual Name) for CAN2_N1_TX_BTIE.
* To use register names with standard convension, please use CAN2_N1_TX_BTIE.
*/
#define CAN2_TXBTIE1 (CAN2_N1_TX_BTIE)

/** \brief 6E4, Tx Buffer Cancellation Finished Interrupt Enable 1 */
#define CAN2_N1_TX_BCIE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCIE*)0xF02286E4u)
/** Alias (User Manual Name) for CAN2_N1_TX_BCIE.
* To use register names with standard convension, please use CAN2_N1_TX_BCIE.
*/
#define CAN2_TXBCIE1 (CAN2_N1_TX_BCIE)

/** \brief 6F0, Tx Event FIFO Configuration 1 */
#define CAN2_N1_TX_EFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFC*)0xF02286F0u)
/** Alias (User Manual Name) for CAN2_N1_TX_EFC.
* To use register names with standard convension, please use CAN2_N1_TX_EFC.
*/
#define CAN2_TXEFC1 (CAN2_N1_TX_EFC)

/** \brief 6F4, Tx Event FIFO Status 1 */
#define CAN2_N1_TX_EFS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFS*)0xF02286F4u)
/** Alias (User Manual Name) for CAN2_N1_TX_EFS.
* To use register names with standard convension, please use CAN2_N1_TX_EFS.
*/
#define CAN2_TXEFS1 (CAN2_N1_TX_EFS)

/** \brief 6F8, Tx Event FIFO Acknowledge 1 */
#define CAN2_N1_TX_EFA /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFA*)0xF02286F8u)
/** Alias (User Manual Name) for CAN2_N1_TX_EFA.
* To use register names with standard convension, please use CAN2_N1_TX_EFA.
*/
#define CAN2_TXEFA1 (CAN2_N1_TX_EFA)

/** \brief 900, Access Enable Register CAN Node 2 0 */
#define CAN2_N2_ACCENNODE0 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ACCENNODE0*)0xF0228900u)
/** Alias (User Manual Name) for CAN2_N2_ACCENNODE0.
* To use register names with standard convension, please use CAN2_N2_ACCENNODE0.
*/
#define CAN2_ACCENNODE20 (CAN2_N2_ACCENNODE0)

/** \brief 908, Start Address Node 2 */
#define CAN2_N2_STARTADR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_STARTADR*)0xF0228908u)
/** Alias (User Manual Name) for CAN2_N2_STARTADR.
* To use register names with standard convension, please use CAN2_N2_STARTADR.
*/
#define CAN2_STARTADR2 (CAN2_N2_STARTADR)

/** \brief 90C, End Address Node 2 */
#define CAN2_N2_ENDADR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ENDADR*)0xF022890Cu)
/** Alias (User Manual Name) for CAN2_N2_ENDADR.
* To use register names with standard convension, please use CAN2_N2_ENDADR.
*/
#define CAN2_ENDADR2 (CAN2_N2_ENDADR)

/** \brief 910, Interrupt Signalling Register 2 */
#define CAN2_N2_ISREG /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ISREG*)0xF0228910u)
/** Alias (User Manual Name) for CAN2_N2_ISREG.
* To use register names with standard convension, please use CAN2_N2_ISREG.
*/
#define CAN2_ISREG2 (CAN2_N2_ISREG)

/** \brief 914, Interrupt routing for Groups 1 2 */
#define CAN2_N2_GRINT1 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GRINT1*)0xF0228914u)
/** Alias (User Manual Name) for CAN2_N2_GRINT1.
* To use register names with standard convension, please use CAN2_N2_GRINT1.
*/
#define CAN2_GRINT12 (CAN2_N2_GRINT1)

/** \brief 918, Interrupt routing for Groups 2 2 */
#define CAN2_N2_GRINT2 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GRINT2*)0xF0228918u)
/** Alias (User Manual Name) for CAN2_N2_GRINT2.
* To use register names with standard convension, please use CAN2_N2_GRINT2.
*/
#define CAN2_GRINT22 (CAN2_N2_GRINT2)

/** \brief 920, Node 2 Timer Clock Control Register */
#define CAN2_N2_NT_CCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_CCR*)0xF0228920u)
/** Alias (User Manual Name) for CAN2_N2_NT_CCR.
* To use register names with standard convension, please use CAN2_N2_NT_CCR.
*/
#define CAN2_NTCCR2 (CAN2_N2_NT_CCR)

/** \brief 924, Node 2 Timer A Transmit Trigger Register */
#define CAN2_N2_NT_ATTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_ATTR*)0xF0228924u)
/** Alias (User Manual Name) for CAN2_N2_NT_ATTR.
* To use register names with standard convension, please use CAN2_N2_NT_ATTR.
*/
#define CAN2_NTATTR2 (CAN2_N2_NT_ATTR)

/** \brief 928, Node 2 Timer B Transmit Trigger Register */
#define CAN2_N2_NT_BTTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_BTTR*)0xF0228928u)
/** Alias (User Manual Name) for CAN2_N2_NT_BTTR.
* To use register names with standard convension, please use CAN2_N2_NT_BTTR.
*/
#define CAN2_NTBTTR2 (CAN2_N2_NT_BTTR)

/** \brief 92C, Node 2 Timer C Transmit Trigger Register */
#define CAN2_N2_NT_CTTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_CTTR*)0xF022892Cu)
/** Alias (User Manual Name) for CAN2_N2_NT_CTTR.
* To use register names with standard convension, please use CAN2_N2_NT_CTTR.
*/
#define CAN2_NTCTTR2 (CAN2_N2_NT_CTTR)

/** \brief 930, Node 2 Timer Receive Timeout Register */
#define CAN2_N2_NT_RTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_RTR*)0xF0228930u)
/** Alias (User Manual Name) for CAN2_N2_NT_RTR.
* To use register names with standard convension, please use CAN2_N2_NT_RTR.
*/
#define CAN2_NTRTR2 (CAN2_N2_NT_RTR)

/** \brief 940, Node 2 Port Control Register */
#define CAN2_N2_NPCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NPCR*)0xF0228940u)
/** Alias (User Manual Name) for CAN2_N2_NPCR.
* To use register names with standard convension, please use CAN2_N2_NPCR.
*/
#define CAN2_NPCR2 (CAN2_N2_NPCR)

/** \brief A00, Core Release Register 2 */
#define CAN2_N2_CREL /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_CREL*)0xF0228A00u)
/** Alias (User Manual Name) for CAN2_N2_CREL.
* To use register names with standard convension, please use CAN2_N2_CREL.
*/
#define CAN2_CREL2 (CAN2_N2_CREL)

/** \brief A04, Endian Register 2 */
#define CAN2_N2_ENDN /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ENDN*)0xF0228A04u)
/** Alias (User Manual Name) for CAN2_N2_ENDN.
* To use register names with standard convension, please use CAN2_N2_ENDN.
*/
#define CAN2_ENDN2 (CAN2_N2_ENDN)

/** \brief A0C, Data Bit Timing & Prescaler Register 2 */
#define CAN2_N2_DBTP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_DBTP*)0xF0228A0Cu)
/** Alias (User Manual Name) for CAN2_N2_DBTP.
* To use register names with standard convension, please use CAN2_N2_DBTP.
*/
#define CAN2_DBTP2 (CAN2_N2_DBTP)

/** \brief A10, Test Register 2 */
#define CAN2_N2_TEST /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TEST*)0xF0228A10u)
/** Alias (User Manual Name) for CAN2_N2_TEST.
* To use register names with standard convension, please use CAN2_N2_TEST.
*/
#define CAN2_TEST2 (CAN2_N2_TEST)

/** \brief A14, RAM Watchdog 2 */
#define CAN2_N2_RWD /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RWD*)0xF0228A14u)
/** Alias (User Manual Name) for CAN2_N2_RWD.
* To use register names with standard convension, please use CAN2_N2_RWD.
*/
#define CAN2_RWD2 (CAN2_N2_RWD)

/** \brief A18, CC Control Register 2 */
#define CAN2_N2_CCCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_CCCR*)0xF0228A18u)
/** Alias (User Manual Name) for CAN2_N2_CCCR.
* To use register names with standard convension, please use CAN2_N2_CCCR.
*/
#define CAN2_CCCR2 (CAN2_N2_CCCR)

/** \brief A1C, Nominal Bit Timing & Prescaler Register 2 */
#define CAN2_N2_NBTP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NBTP*)0xF0228A1Cu)
/** Alias (User Manual Name) for CAN2_N2_NBTP.
* To use register names with standard convension, please use CAN2_N2_NBTP.
*/
#define CAN2_NBTP2 (CAN2_N2_NBTP)

/** \brief A20, Timestamp Counter Configuration 2 */
#define CAN2_N2_TSCC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TSCC*)0xF0228A20u)
/** Alias (User Manual Name) for CAN2_N2_TSCC.
* To use register names with standard convension, please use CAN2_N2_TSCC.
*/
#define CAN2_TSCC2 (CAN2_N2_TSCC)

/** \brief A24, Timestamp Counter Value 2 */
#define CAN2_N2_TSCV /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TSCV*)0xF0228A24u)
/** Alias (User Manual Name) for CAN2_N2_TSCV.
* To use register names with standard convension, please use CAN2_N2_TSCV.
*/
#define CAN2_TSCV2 (CAN2_N2_TSCV)

/** \brief A28, Timeout Counter Configuration 2 */
#define CAN2_N2_TOCC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TOCC*)0xF0228A28u)
/** Alias (User Manual Name) for CAN2_N2_TOCC.
* To use register names with standard convension, please use CAN2_N2_TOCC.
*/
#define CAN2_TOCC2 (CAN2_N2_TOCC)

/** \brief A2C, Timeout Counter Value 2 */
#define CAN2_N2_TOCV /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TOCV*)0xF0228A2Cu)
/** Alias (User Manual Name) for CAN2_N2_TOCV.
* To use register names with standard convension, please use CAN2_N2_TOCV.
*/
#define CAN2_TOCV2 (CAN2_N2_TOCV)

/** \brief A40, Error Counter Register 2 */
#define CAN2_N2_ECR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ECR*)0xF0228A40u)
/** Alias (User Manual Name) for CAN2_N2_ECR.
* To use register names with standard convension, please use CAN2_N2_ECR.
*/
#define CAN2_ECR2 (CAN2_N2_ECR)

/** \brief A44, Protocol Status Register 2 */
#define CAN2_N2_PSR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_PSR*)0xF0228A44u)
/** Alias (User Manual Name) for CAN2_N2_PSR.
* To use register names with standard convension, please use CAN2_N2_PSR.
*/
#define CAN2_PSR2 (CAN2_N2_PSR)

/** \brief A48, Transmitter Delay Compensation Register 2 */
#define CAN2_N2_TDCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TDCR*)0xF0228A48u)
/** Alias (User Manual Name) for CAN2_N2_TDCR.
* To use register names with standard convension, please use CAN2_N2_TDCR.
*/
#define CAN2_TDCR2 (CAN2_N2_TDCR)

/** \brief A50, Interrupt Register 2 */
#define CAN2_N2_IR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_IR*)0xF0228A50u)
/** Alias (User Manual Name) for CAN2_N2_IR.
* To use register names with standard convension, please use CAN2_N2_IR.
*/
#define CAN2_IR2 (CAN2_N2_IR)

/** \brief A54, Interrupt Enable 2 */
#define CAN2_N2_IE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_IE*)0xF0228A54u)
/** Alias (User Manual Name) for CAN2_N2_IE.
* To use register names with standard convension, please use CAN2_N2_IE.
*/
#define CAN2_IE2 (CAN2_N2_IE)

/** \brief A80, Global Filter Configuration 2 */
#define CAN2_N2_GFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GFC*)0xF0228A80u)
/** Alias (User Manual Name) for CAN2_N2_GFC.
* To use register names with standard convension, please use CAN2_N2_GFC.
*/
#define CAN2_GFC2 (CAN2_N2_GFC)

/** \brief A84, Standard ID Filter Configuration 2 */
#define CAN2_N2_SIDFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_SIDFC*)0xF0228A84u)
/** Alias (User Manual Name) for CAN2_N2_SIDFC.
* To use register names with standard convension, please use CAN2_N2_SIDFC.
*/
#define CAN2_SIDFC2 (CAN2_N2_SIDFC)

/** \brief A88, Extended ID Filter Configuration 2 */
#define CAN2_N2_XIDFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_XIDFC*)0xF0228A88u)
/** Alias (User Manual Name) for CAN2_N2_XIDFC.
* To use register names with standard convension, please use CAN2_N2_XIDFC.
*/
#define CAN2_XIDFC2 (CAN2_N2_XIDFC)

/** \brief A90, Extended ID AND Mask 2 */
#define CAN2_N2_XIDAM /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_XIDAM*)0xF0228A90u)
/** Alias (User Manual Name) for CAN2_N2_XIDAM.
* To use register names with standard convension, please use CAN2_N2_XIDAM.
*/
#define CAN2_XIDAM2 (CAN2_N2_XIDAM)

/** \brief A94, High Priority Message Status 2 */
#define CAN2_N2_HPMS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_HPMS*)0xF0228A94u)
/** Alias (User Manual Name) for CAN2_N2_HPMS.
* To use register names with standard convension, please use CAN2_N2_HPMS.
*/
#define CAN2_HPMS2 (CAN2_N2_HPMS)

/** \brief A98, New Data 1 2 */
#define CAN2_N2_NDAT1 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NDAT1*)0xF0228A98u)
/** Alias (User Manual Name) for CAN2_N2_NDAT1.
* To use register names with standard convension, please use CAN2_N2_NDAT1.
*/
#define CAN2_NDAT12 (CAN2_N2_NDAT1)

/** \brief A9C, New Data 2 2 */
#define CAN2_N2_NDAT2 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NDAT2*)0xF0228A9Cu)
/** Alias (User Manual Name) for CAN2_N2_NDAT2.
* To use register names with standard convension, please use CAN2_N2_NDAT2.
*/
#define CAN2_NDAT22 (CAN2_N2_NDAT2)

/** \brief AA0, Rx FIFO 0 Configuration 2 */
#define CAN2_N2_RX_F0C /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0C*)0xF0228AA0u)
/** Alias (User Manual Name) for CAN2_N2_RX_F0C.
* To use register names with standard convension, please use CAN2_N2_RX_F0C.
*/
#define CAN2_RXF0C2 (CAN2_N2_RX_F0C)

/** \brief AA4, Rx FIFO 0 Status 2 */
#define CAN2_N2_RX_F0S /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0S*)0xF0228AA4u)
/** Alias (User Manual Name) for CAN2_N2_RX_F0S.
* To use register names with standard convension, please use CAN2_N2_RX_F0S.
*/
#define CAN2_RXF0S2 (CAN2_N2_RX_F0S)

/** \brief AA8, Rx FIFO 0 Acknowledge 2 */
#define CAN2_N2_RX_F0A /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0A*)0xF0228AA8u)
/** Alias (User Manual Name) for CAN2_N2_RX_F0A.
* To use register names with standard convension, please use CAN2_N2_RX_F0A.
*/
#define CAN2_RXF0A2 (CAN2_N2_RX_F0A)

/** \brief AAC, Rx Buffer Configuration 2 */
#define CAN2_N2_RX_BC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_BC*)0xF0228AACu)
/** Alias (User Manual Name) for CAN2_N2_RX_BC.
* To use register names with standard convension, please use CAN2_N2_RX_BC.
*/
#define CAN2_RXBC2 (CAN2_N2_RX_BC)

/** \brief AB0, Rx FIFO 1 Configuration 2 */
#define CAN2_N2_RX_F1C /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1C*)0xF0228AB0u)
/** Alias (User Manual Name) for CAN2_N2_RX_F1C.
* To use register names with standard convension, please use CAN2_N2_RX_F1C.
*/
#define CAN2_RXF1C2 (CAN2_N2_RX_F1C)

/** \brief AB4, Rx FIFO 1 Status 2 */
#define CAN2_N2_RX_F1S /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1S*)0xF0228AB4u)
/** Alias (User Manual Name) for CAN2_N2_RX_F1S.
* To use register names with standard convension, please use CAN2_N2_RX_F1S.
*/
#define CAN2_RXF1S2 (CAN2_N2_RX_F1S)

/** \brief AB8, Rx FIFO 1 Acknowledge 2 */
#define CAN2_N2_RX_F1A /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1A*)0xF0228AB8u)
/** Alias (User Manual Name) for CAN2_N2_RX_F1A.
* To use register names with standard convension, please use CAN2_N2_RX_F1A.
*/
#define CAN2_RXF1A2 (CAN2_N2_RX_F1A)

/** \brief ABC, Rx Buffer/FIFO Element Size Configuration 2 */
#define CAN2_N2_RX_ESC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_ESC*)0xF0228ABCu)
/** Alias (User Manual Name) for CAN2_N2_RX_ESC.
* To use register names with standard convension, please use CAN2_N2_RX_ESC.
*/
#define CAN2_RXESC2 (CAN2_N2_RX_ESC)

/** \brief AC0, Tx Buffer Configuration 2 */
#define CAN2_N2_TX_BC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BC*)0xF0228AC0u)
/** Alias (User Manual Name) for CAN2_N2_TX_BC.
* To use register names with standard convension, please use CAN2_N2_TX_BC.
*/
#define CAN2_TXBC2 (CAN2_N2_TX_BC)

/** \brief AC4, Tx FIFO/Queue Status 2 */
#define CAN2_N2_TX_FQS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_FQS*)0xF0228AC4u)
/** Alias (User Manual Name) for CAN2_N2_TX_FQS.
* To use register names with standard convension, please use CAN2_N2_TX_FQS.
*/
#define CAN2_TXFQS2 (CAN2_N2_TX_FQS)

/** \brief AC8, Tx Buffer Element Size Configuration 2 */
#define CAN2_N2_TX_ESC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_ESC*)0xF0228AC8u)
/** Alias (User Manual Name) for CAN2_N2_TX_ESC.
* To use register names with standard convension, please use CAN2_N2_TX_ESC.
*/
#define CAN2_TXESC2 (CAN2_N2_TX_ESC)

/** \brief ACC, Tx Buffer Request Pending 2 */
#define CAN2_N2_TX_BRP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BRP*)0xF0228ACCu)
/** Alias (User Manual Name) for CAN2_N2_TX_BRP.
* To use register names with standard convension, please use CAN2_N2_TX_BRP.
*/
#define CAN2_TXBRP2 (CAN2_N2_TX_BRP)

/** \brief AD0, Tx Buffer Add Request 2 */
#define CAN2_N2_TX_BAR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BAR*)0xF0228AD0u)
/** Alias (User Manual Name) for CAN2_N2_TX_BAR.
* To use register names with standard convension, please use CAN2_N2_TX_BAR.
*/
#define CAN2_TXBAR2 (CAN2_N2_TX_BAR)

/** \brief AD4, Tx Buffer Cancellation Request 2 */
#define CAN2_N2_TX_BCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCR*)0xF0228AD4u)
/** Alias (User Manual Name) for CAN2_N2_TX_BCR.
* To use register names with standard convension, please use CAN2_N2_TX_BCR.
*/
#define CAN2_TXBCR2 (CAN2_N2_TX_BCR)

/** \brief AD8, Tx Buffer Transmission Occurred 2 */
#define CAN2_N2_TX_BTO /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BTO*)0xF0228AD8u)
/** Alias (User Manual Name) for CAN2_N2_TX_BTO.
* To use register names with standard convension, please use CAN2_N2_TX_BTO.
*/
#define CAN2_TXBTO2 (CAN2_N2_TX_BTO)

/** \brief ADC, Tx Buffer Cancellation Finished 2 */
#define CAN2_N2_TX_BCF /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCF*)0xF0228ADCu)
/** Alias (User Manual Name) for CAN2_N2_TX_BCF.
* To use register names with standard convension, please use CAN2_N2_TX_BCF.
*/
#define CAN2_TXBCF2 (CAN2_N2_TX_BCF)

/** \brief AE0, Tx Buffer Transmission Interrupt Enable 2 */
#define CAN2_N2_TX_BTIE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BTIE*)0xF0228AE0u)
/** Alias (User Manual Name) for CAN2_N2_TX_BTIE.
* To use register names with standard convension, please use CAN2_N2_TX_BTIE.
*/
#define CAN2_TXBTIE2 (CAN2_N2_TX_BTIE)

/** \brief AE4, Tx Buffer Cancellation Finished Interrupt Enable 2 */
#define CAN2_N2_TX_BCIE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCIE*)0xF0228AE4u)
/** Alias (User Manual Name) for CAN2_N2_TX_BCIE.
* To use register names with standard convension, please use CAN2_N2_TX_BCIE.
*/
#define CAN2_TXBCIE2 (CAN2_N2_TX_BCIE)

/** \brief AF0, Tx Event FIFO Configuration 2 */
#define CAN2_N2_TX_EFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFC*)0xF0228AF0u)
/** Alias (User Manual Name) for CAN2_N2_TX_EFC.
* To use register names with standard convension, please use CAN2_N2_TX_EFC.
*/
#define CAN2_TXEFC2 (CAN2_N2_TX_EFC)

/** \brief AF4, Tx Event FIFO Status 2 */
#define CAN2_N2_TX_EFS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFS*)0xF0228AF4u)
/** Alias (User Manual Name) for CAN2_N2_TX_EFS.
* To use register names with standard convension, please use CAN2_N2_TX_EFS.
*/
#define CAN2_TXEFS2 (CAN2_N2_TX_EFS)

/** \brief AF8, Tx Event FIFO Acknowledge 2 */
#define CAN2_N2_TX_EFA /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFA*)0xF0228AF8u)
/** Alias (User Manual Name) for CAN2_N2_TX_EFA.
* To use register names with standard convension, please use CAN2_N2_TX_EFA.
*/
#define CAN2_TXEFA2 (CAN2_N2_TX_EFA)

/** \brief D00, Access Enable Register CAN Node 3 0 */
#define CAN2_N3_ACCENNODE0 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ACCENNODE0*)0xF0228D00u)
/** Alias (User Manual Name) for CAN2_N3_ACCENNODE0.
* To use register names with standard convension, please use CAN2_N3_ACCENNODE0.
*/
#define CAN2_ACCENNODE30 (CAN2_N3_ACCENNODE0)

/** \brief D08, Start Address Node 3 */
#define CAN2_N3_STARTADR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_STARTADR*)0xF0228D08u)
/** Alias (User Manual Name) for CAN2_N3_STARTADR.
* To use register names with standard convension, please use CAN2_N3_STARTADR.
*/
#define CAN2_STARTADR3 (CAN2_N3_STARTADR)

/** \brief D0C, End Address Node 3 */
#define CAN2_N3_ENDADR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ENDADR*)0xF0228D0Cu)
/** Alias (User Manual Name) for CAN2_N3_ENDADR.
* To use register names with standard convension, please use CAN2_N3_ENDADR.
*/
#define CAN2_ENDADR3 (CAN2_N3_ENDADR)

/** \brief D10, Interrupt Signalling Register 3 */
#define CAN2_N3_ISREG /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ISREG*)0xF0228D10u)
/** Alias (User Manual Name) for CAN2_N3_ISREG.
* To use register names with standard convension, please use CAN2_N3_ISREG.
*/
#define CAN2_ISREG3 (CAN2_N3_ISREG)

/** \brief D14, Interrupt routing for Groups 1 3 */
#define CAN2_N3_GRINT1 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GRINT1*)0xF0228D14u)
/** Alias (User Manual Name) for CAN2_N3_GRINT1.
* To use register names with standard convension, please use CAN2_N3_GRINT1.
*/
#define CAN2_GRINT13 (CAN2_N3_GRINT1)

/** \brief D18, Interrupt routing for Groups 2 3 */
#define CAN2_N3_GRINT2 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GRINT2*)0xF0228D18u)
/** Alias (User Manual Name) for CAN2_N3_GRINT2.
* To use register names with standard convension, please use CAN2_N3_GRINT2.
*/
#define CAN2_GRINT23 (CAN2_N3_GRINT2)

/** \brief D20, Node 3 Timer Clock Control Register */
#define CAN2_N3_NT_CCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_CCR*)0xF0228D20u)
/** Alias (User Manual Name) for CAN2_N3_NT_CCR.
* To use register names with standard convension, please use CAN2_N3_NT_CCR.
*/
#define CAN2_NTCCR3 (CAN2_N3_NT_CCR)

/** \brief D24, Node 3 Timer A Transmit Trigger Register */
#define CAN2_N3_NT_ATTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_ATTR*)0xF0228D24u)
/** Alias (User Manual Name) for CAN2_N3_NT_ATTR.
* To use register names with standard convension, please use CAN2_N3_NT_ATTR.
*/
#define CAN2_NTATTR3 (CAN2_N3_NT_ATTR)

/** \brief D28, Node 3 Timer B Transmit Trigger Register */
#define CAN2_N3_NT_BTTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_BTTR*)0xF0228D28u)
/** Alias (User Manual Name) for CAN2_N3_NT_BTTR.
* To use register names with standard convension, please use CAN2_N3_NT_BTTR.
*/
#define CAN2_NTBTTR3 (CAN2_N3_NT_BTTR)

/** \brief D2C, Node 3 Timer C Transmit Trigger Register */
#define CAN2_N3_NT_CTTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_CTTR*)0xF0228D2Cu)
/** Alias (User Manual Name) for CAN2_N3_NT_CTTR.
* To use register names with standard convension, please use CAN2_N3_NT_CTTR.
*/
#define CAN2_NTCTTR3 (CAN2_N3_NT_CTTR)

/** \brief D30, Node 3 Timer Receive Timeout Register */
#define CAN2_N3_NT_RTR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NT_RTR*)0xF0228D30u)
/** Alias (User Manual Name) for CAN2_N3_NT_RTR.
* To use register names with standard convension, please use CAN2_N3_NT_RTR.
*/
#define CAN2_NTRTR3 (CAN2_N3_NT_RTR)

/** \brief D40, Node 3 Port Control Register */
#define CAN2_N3_NPCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NPCR*)0xF0228D40u)
/** Alias (User Manual Name) for CAN2_N3_NPCR.
* To use register names with standard convension, please use CAN2_N3_NPCR.
*/
#define CAN2_NPCR3 (CAN2_N3_NPCR)

/** \brief E00, Core Release Register 3 */
#define CAN2_N3_CREL /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_CREL*)0xF0228E00u)
/** Alias (User Manual Name) for CAN2_N3_CREL.
* To use register names with standard convension, please use CAN2_N3_CREL.
*/
#define CAN2_CREL3 (CAN2_N3_CREL)

/** \brief E04, Endian Register 3 */
#define CAN2_N3_ENDN /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ENDN*)0xF0228E04u)
/** Alias (User Manual Name) for CAN2_N3_ENDN.
* To use register names with standard convension, please use CAN2_N3_ENDN.
*/
#define CAN2_ENDN3 (CAN2_N3_ENDN)

/** \brief E0C, Data Bit Timing & Prescaler Register 3 */
#define CAN2_N3_DBTP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_DBTP*)0xF0228E0Cu)
/** Alias (User Manual Name) for CAN2_N3_DBTP.
* To use register names with standard convension, please use CAN2_N3_DBTP.
*/
#define CAN2_DBTP3 (CAN2_N3_DBTP)

/** \brief E10, Test Register 3 */
#define CAN2_N3_TEST /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TEST*)0xF0228E10u)
/** Alias (User Manual Name) for CAN2_N3_TEST.
* To use register names with standard convension, please use CAN2_N3_TEST.
*/
#define CAN2_TEST3 (CAN2_N3_TEST)

/** \brief E14, RAM Watchdog 3 */
#define CAN2_N3_RWD /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RWD*)0xF0228E14u)
/** Alias (User Manual Name) for CAN2_N3_RWD.
* To use register names with standard convension, please use CAN2_N3_RWD.
*/
#define CAN2_RWD3 (CAN2_N3_RWD)

/** \brief E18, CC Control Register 3 */
#define CAN2_N3_CCCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_CCCR*)0xF0228E18u)
/** Alias (User Manual Name) for CAN2_N3_CCCR.
* To use register names with standard convension, please use CAN2_N3_CCCR.
*/
#define CAN2_CCCR3 (CAN2_N3_CCCR)

/** \brief E1C, Nominal Bit Timing & Prescaler Register 3 */
#define CAN2_N3_NBTP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NBTP*)0xF0228E1Cu)
/** Alias (User Manual Name) for CAN2_N3_NBTP.
* To use register names with standard convension, please use CAN2_N3_NBTP.
*/
#define CAN2_NBTP3 (CAN2_N3_NBTP)

/** \brief E20, Timestamp Counter Configuration 3 */
#define CAN2_N3_TSCC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TSCC*)0xF0228E20u)
/** Alias (User Manual Name) for CAN2_N3_TSCC.
* To use register names with standard convension, please use CAN2_N3_TSCC.
*/
#define CAN2_TSCC3 (CAN2_N3_TSCC)

/** \brief E24, Timestamp Counter Value 3 */
#define CAN2_N3_TSCV /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TSCV*)0xF0228E24u)
/** Alias (User Manual Name) for CAN2_N3_TSCV.
* To use register names with standard convension, please use CAN2_N3_TSCV.
*/
#define CAN2_TSCV3 (CAN2_N3_TSCV)

/** \brief E28, Timeout Counter Configuration 3 */
#define CAN2_N3_TOCC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TOCC*)0xF0228E28u)
/** Alias (User Manual Name) for CAN2_N3_TOCC.
* To use register names with standard convension, please use CAN2_N3_TOCC.
*/
#define CAN2_TOCC3 (CAN2_N3_TOCC)

/** \brief E2C, Timeout Counter Value 3 */
#define CAN2_N3_TOCV /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TOCV*)0xF0228E2Cu)
/** Alias (User Manual Name) for CAN2_N3_TOCV.
* To use register names with standard convension, please use CAN2_N3_TOCV.
*/
#define CAN2_TOCV3 (CAN2_N3_TOCV)

/** \brief E40, Error Counter Register 3 */
#define CAN2_N3_ECR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_ECR*)0xF0228E40u)
/** Alias (User Manual Name) for CAN2_N3_ECR.
* To use register names with standard convension, please use CAN2_N3_ECR.
*/
#define CAN2_ECR3 (CAN2_N3_ECR)

/** \brief E44, Protocol Status Register 3 */
#define CAN2_N3_PSR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_PSR*)0xF0228E44u)
/** Alias (User Manual Name) for CAN2_N3_PSR.
* To use register names with standard convension, please use CAN2_N3_PSR.
*/
#define CAN2_PSR3 (CAN2_N3_PSR)

/** \brief E48, Transmitter Delay Compensation Register 3 */
#define CAN2_N3_TDCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TDCR*)0xF0228E48u)
/** Alias (User Manual Name) for CAN2_N3_TDCR.
* To use register names with standard convension, please use CAN2_N3_TDCR.
*/
#define CAN2_TDCR3 (CAN2_N3_TDCR)

/** \brief E50, Interrupt Register 3 */
#define CAN2_N3_IR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_IR*)0xF0228E50u)
/** Alias (User Manual Name) for CAN2_N3_IR.
* To use register names with standard convension, please use CAN2_N3_IR.
*/
#define CAN2_IR3 (CAN2_N3_IR)

/** \brief E54, Interrupt Enable 3 */
#define CAN2_N3_IE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_IE*)0xF0228E54u)
/** Alias (User Manual Name) for CAN2_N3_IE.
* To use register names with standard convension, please use CAN2_N3_IE.
*/
#define CAN2_IE3 (CAN2_N3_IE)

/** \brief E80, Global Filter Configuration 3 */
#define CAN2_N3_GFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_GFC*)0xF0228E80u)
/** Alias (User Manual Name) for CAN2_N3_GFC.
* To use register names with standard convension, please use CAN2_N3_GFC.
*/
#define CAN2_GFC3 (CAN2_N3_GFC)

/** \brief E84, Standard ID Filter Configuration 3 */
#define CAN2_N3_SIDFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_SIDFC*)0xF0228E84u)
/** Alias (User Manual Name) for CAN2_N3_SIDFC.
* To use register names with standard convension, please use CAN2_N3_SIDFC.
*/
#define CAN2_SIDFC3 (CAN2_N3_SIDFC)

/** \brief E88, Extended ID Filter Configuration 3 */
#define CAN2_N3_XIDFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_XIDFC*)0xF0228E88u)
/** Alias (User Manual Name) for CAN2_N3_XIDFC.
* To use register names with standard convension, please use CAN2_N3_XIDFC.
*/
#define CAN2_XIDFC3 (CAN2_N3_XIDFC)

/** \brief E90, Extended ID AND Mask 3 */
#define CAN2_N3_XIDAM /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_XIDAM*)0xF0228E90u)
/** Alias (User Manual Name) for CAN2_N3_XIDAM.
* To use register names with standard convension, please use CAN2_N3_XIDAM.
*/
#define CAN2_XIDAM3 (CAN2_N3_XIDAM)

/** \brief E94, High Priority Message Status 3 */
#define CAN2_N3_HPMS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_HPMS*)0xF0228E94u)
/** Alias (User Manual Name) for CAN2_N3_HPMS.
* To use register names with standard convension, please use CAN2_N3_HPMS.
*/
#define CAN2_HPMS3 (CAN2_N3_HPMS)

/** \brief E98, New Data 1 3 */
#define CAN2_N3_NDAT1 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NDAT1*)0xF0228E98u)
/** Alias (User Manual Name) for CAN2_N3_NDAT1.
* To use register names with standard convension, please use CAN2_N3_NDAT1.
*/
#define CAN2_NDAT13 (CAN2_N3_NDAT1)

/** \brief E9C, New Data 2 3 */
#define CAN2_N3_NDAT2 /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_NDAT2*)0xF0228E9Cu)
/** Alias (User Manual Name) for CAN2_N3_NDAT2.
* To use register names with standard convension, please use CAN2_N3_NDAT2.
*/
#define CAN2_NDAT23 (CAN2_N3_NDAT2)

/** \brief EA0, Rx FIFO 0 Configuration 3 */
#define CAN2_N3_RX_F0C /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0C*)0xF0228EA0u)
/** Alias (User Manual Name) for CAN2_N3_RX_F0C.
* To use register names with standard convension, please use CAN2_N3_RX_F0C.
*/
#define CAN2_RXF0C3 (CAN2_N3_RX_F0C)

/** \brief EA4, Rx FIFO 0 Status 3 */
#define CAN2_N3_RX_F0S /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0S*)0xF0228EA4u)
/** Alias (User Manual Name) for CAN2_N3_RX_F0S.
* To use register names with standard convension, please use CAN2_N3_RX_F0S.
*/
#define CAN2_RXF0S3 (CAN2_N3_RX_F0S)

/** \brief EA8, Rx FIFO 0 Acknowledge 3 */
#define CAN2_N3_RX_F0A /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F0A*)0xF0228EA8u)
/** Alias (User Manual Name) for CAN2_N3_RX_F0A.
* To use register names with standard convension, please use CAN2_N3_RX_F0A.
*/
#define CAN2_RXF0A3 (CAN2_N3_RX_F0A)

/** \brief EAC, Rx Buffer Configuration 3 */
#define CAN2_N3_RX_BC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_BC*)0xF0228EACu)
/** Alias (User Manual Name) for CAN2_N3_RX_BC.
* To use register names with standard convension, please use CAN2_N3_RX_BC.
*/
#define CAN2_RXBC3 (CAN2_N3_RX_BC)

/** \brief EB0, Rx FIFO 1 Configuration 3 */
#define CAN2_N3_RX_F1C /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1C*)0xF0228EB0u)
/** Alias (User Manual Name) for CAN2_N3_RX_F1C.
* To use register names with standard convension, please use CAN2_N3_RX_F1C.
*/
#define CAN2_RXF1C3 (CAN2_N3_RX_F1C)

/** \brief EB4, Rx FIFO 1 Status 3 */
#define CAN2_N3_RX_F1S /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1S*)0xF0228EB4u)
/** Alias (User Manual Name) for CAN2_N3_RX_F1S.
* To use register names with standard convension, please use CAN2_N3_RX_F1S.
*/
#define CAN2_RXF1S3 (CAN2_N3_RX_F1S)

/** \brief EB8, Rx FIFO 1 Acknowledge 3 */
#define CAN2_N3_RX_F1A /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_F1A*)0xF0228EB8u)
/** Alias (User Manual Name) for CAN2_N3_RX_F1A.
* To use register names with standard convension, please use CAN2_N3_RX_F1A.
*/
#define CAN2_RXF1A3 (CAN2_N3_RX_F1A)

/** \brief EBC, Rx Buffer/FIFO Element Size Configuration 3 */
#define CAN2_N3_RX_ESC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_RX_ESC*)0xF0228EBCu)
/** Alias (User Manual Name) for CAN2_N3_RX_ESC.
* To use register names with standard convension, please use CAN2_N3_RX_ESC.
*/
#define CAN2_RXESC3 (CAN2_N3_RX_ESC)

/** \brief EC0, Tx Buffer Configuration 3 */
#define CAN2_N3_TX_BC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BC*)0xF0228EC0u)
/** Alias (User Manual Name) for CAN2_N3_TX_BC.
* To use register names with standard convension, please use CAN2_N3_TX_BC.
*/
#define CAN2_TXBC3 (CAN2_N3_TX_BC)

/** \brief EC4, Tx FIFO/Queue Status 3 */
#define CAN2_N3_TX_FQS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_FQS*)0xF0228EC4u)
/** Alias (User Manual Name) for CAN2_N3_TX_FQS.
* To use register names with standard convension, please use CAN2_N3_TX_FQS.
*/
#define CAN2_TXFQS3 (CAN2_N3_TX_FQS)

/** \brief EC8, Tx Buffer Element Size Configuration 3 */
#define CAN2_N3_TX_ESC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_ESC*)0xF0228EC8u)
/** Alias (User Manual Name) for CAN2_N3_TX_ESC.
* To use register names with standard convension, please use CAN2_N3_TX_ESC.
*/
#define CAN2_TXESC3 (CAN2_N3_TX_ESC)

/** \brief ECC, Tx Buffer Request Pending 3 */
#define CAN2_N3_TX_BRP /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BRP*)0xF0228ECCu)
/** Alias (User Manual Name) for CAN2_N3_TX_BRP.
* To use register names with standard convension, please use CAN2_N3_TX_BRP.
*/
#define CAN2_TXBRP3 (CAN2_N3_TX_BRP)

/** \brief ED0, Tx Buffer Add Request 3 */
#define CAN2_N3_TX_BAR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BAR*)0xF0228ED0u)
/** Alias (User Manual Name) for CAN2_N3_TX_BAR.
* To use register names with standard convension, please use CAN2_N3_TX_BAR.
*/
#define CAN2_TXBAR3 (CAN2_N3_TX_BAR)

/** \brief ED4, Tx Buffer Cancellation Request 3 */
#define CAN2_N3_TX_BCR /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCR*)0xF0228ED4u)
/** Alias (User Manual Name) for CAN2_N3_TX_BCR.
* To use register names with standard convension, please use CAN2_N3_TX_BCR.
*/
#define CAN2_TXBCR3 (CAN2_N3_TX_BCR)

/** \brief ED8, Tx Buffer Transmission Occurred 3 */
#define CAN2_N3_TX_BTO /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BTO*)0xF0228ED8u)
/** Alias (User Manual Name) for CAN2_N3_TX_BTO.
* To use register names with standard convension, please use CAN2_N3_TX_BTO.
*/
#define CAN2_TXBTO3 (CAN2_N3_TX_BTO)

/** \brief EDC, Tx Buffer Cancellation Finished 3 */
#define CAN2_N3_TX_BCF /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCF*)0xF0228EDCu)
/** Alias (User Manual Name) for CAN2_N3_TX_BCF.
* To use register names with standard convension, please use CAN2_N3_TX_BCF.
*/
#define CAN2_TXBCF3 (CAN2_N3_TX_BCF)

/** \brief EE0, Tx Buffer Transmission Interrupt Enable 3 */
#define CAN2_N3_TX_BTIE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BTIE*)0xF0228EE0u)
/** Alias (User Manual Name) for CAN2_N3_TX_BTIE.
* To use register names with standard convension, please use CAN2_N3_TX_BTIE.
*/
#define CAN2_TXBTIE3 (CAN2_N3_TX_BTIE)

/** \brief EE4, Tx Buffer Cancellation Finished Interrupt Enable 3 */
#define CAN2_N3_TX_BCIE /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_BCIE*)0xF0228EE4u)
/** Alias (User Manual Name) for CAN2_N3_TX_BCIE.
* To use register names with standard convension, please use CAN2_N3_TX_BCIE.
*/
#define CAN2_TXBCIE3 (CAN2_N3_TX_BCIE)

/** \brief EF0, Tx Event FIFO Configuration 3 */
#define CAN2_N3_TX_EFC /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFC*)0xF0228EF0u)
/** Alias (User Manual Name) for CAN2_N3_TX_EFC.
* To use register names with standard convension, please use CAN2_N3_TX_EFC.
*/
#define CAN2_TXEFC3 (CAN2_N3_TX_EFC)

/** \brief EF4, Tx Event FIFO Status 3 */
#define CAN2_N3_TX_EFS /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFS*)0xF0228EF4u)
/** Alias (User Manual Name) for CAN2_N3_TX_EFS.
* To use register names with standard convension, please use CAN2_N3_TX_EFS.
*/
#define CAN2_TXEFS3 (CAN2_N3_TX_EFS)

/** \brief EF8, Tx Event FIFO Acknowledge 3 */
#define CAN2_N3_TX_EFA /*lint --e(923, 9078)*/ (*(volatile Ifx_CAN_N_TX_EFA*)0xF0228EF8u)
/** Alias (User Manual Name) for CAN2_N3_TX_EFA.
* To use register names with standard convension, please use CAN2_N3_TX_EFA.
*/
#define CAN2_TXEFA3 (CAN2_N3_TX_EFA)


/** \}  */

/******************************************************************************/

/******************************************************************************/

#endif /* IFXCAN_REG_H */
