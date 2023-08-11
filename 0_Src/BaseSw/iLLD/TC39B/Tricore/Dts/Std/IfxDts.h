/**
 * \file IfxDts.h
 * \brief DTS  basic functionality
 * \ingroup IfxLld_Dts
 *
 * \version iLLD_1_0_1_9_0
 * \copyright Copyright (c) 2017 Infineon Technologies AG. All rights reserved.
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
 * \defgroup IfxLld_Dts_Std_Control Control functions
 * \ingroup IfxLld_Dts_Std
 * \defgroup IfxLld_Dts_Std_Status Status functions
 * \ingroup IfxLld_Dts_Std
 */

#ifndef IFXDTS_H
#define IFXDTS_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "_Impl/IfxDts_cfg.h"
#include "IfxPms_reg.h"
#include "IfxPms_bf.h"
#include "Cpu/Std/IfxCpu_Intrinsics.h"
#include "Src/Std/IfxSrc.h"
/** \addtogroup IfxLld_Dts_Std_Status
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Gets the Interrupt request source.
 * \return Address of interrupt request source
 */
IFX_INLINE volatile Ifx_SRC_SRCR *IfxDts_getSrcPointer(void);

/** \brief Read the temperature measurement result
 * \return Temperature measurement result
 */
IFX_INLINE uint16 IfxDts_getTemperatureValue(void);

/** \} */

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE volatile Ifx_SRC_SRCR *IfxDts_getSrcPointer(void)
{
    return &SRC_PMSDTS;
}


IFX_INLINE uint16 IfxDts_getTemperatureValue(void)
{
    return (uint16)MODULE_PMS.DTSSTAT.B.RESULT;
}


#endif /* IFXDTS_H */
