/**
 * \file IfxFce.h
 * \brief FCE  basic functionality
 * \ingroup IfxLld_Fce
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
 *
 * \defgroup IfxLld_Fce_Std_Enum Enumerations
 * \ingroup IfxLld_Fce_Std
 * \defgroup IfxLld_Fce_Std_Module Module Functions
 * \ingroup IfxLld_Fce_Std
 * \defgroup IfxLld_Fce_Std_Support Support Function
 * \ingroup IfxLld_Fce_Std
 * \defgroup IfxLld_Fce_Std_InterruptStatus Interrupt Status Functions
 * \ingroup IfxLld_Fce_Std
 */

#ifndef IFXFCE_H
#define IFXFCE_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "_Impl/IfxFce_cfg.h"
#include "Cpu/Std/IfxCpu_Intrinsics.h"
#include "Scu/Std/IfxScuWdt.h"
#include "IfxFce_reg.h"
#include "Src/Std/IfxSrc.h"

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Fce_Std_Enum
 * \{ */
/** \brief Ifx_FCE_CHx(x= 0,1), Specifies the channel used for CRC
 */
typedef enum
{
    IfxFce_CrcChannel_0 = 0,      /**< \brief Specifies the channel0 used for CRC  */
    IfxFce_CrcChannel_1,          /**< \brief Specifies the channel1 used for CRC  */
    IfxFce_CrcChannel_2,          /**< \brief Specifies the channel2 used for CRC  */
    IfxFce_CrcChannel_3,          /**< \brief Specifies the channel3 used for CRC  */
    IfxFce_CrcChannel_4,          /**< \brief Specifies the channel4 used for CRC  */
    IfxFce_CrcChannel_5,          /**< \brief Specifies the channel5 used for CRC  */
    IfxFce_CrcChannel_6,          /**< \brief Specifies the channel6 used for CRC  */
    IfxFce_CrcChannel_7           /**< \brief Specifies the channel7 used for CRC  */
} IfxFce_CrcChannel;

/** \} */

/** \brief Specify the CRC kernel used by the fce channel
 */
typedef enum
{
    IfxFce_CrcKernel_0 = 0,  /**< \brief Specifies to use kernel 0 */
    IfxFce_CrcKernel_1 = 1,  /**< \brief Specifies to use kernel 1 */
    IfxFce_CrcKernel_2 = 2,  /**< \brief Specifies to use kernel 2 */
    IfxFce_CrcKernel_3 = 3   /**< \brief Specifies to use kernel 3 */
} IfxFce_CrcKernel;

/** \addtogroup IfxLld_Fce_Std_Module
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Disable the control of FCE module
 * \param fce Specifies pointer to FCE module registers
 * \return None
 */
IFX_INLINE void IfxFce_disableModule(Ifx_FCE *fce);

/** \brief Enable the control of FCE module
 * \param fce Specifies pointer to FCE module registers
 * \return None
 */
IFX_INLINE void IfxFce_enableModule(Ifx_FCE *fce);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Reset the module by clearing the kernel
 * \param fce Specifies pointer to FCE module registers
 * \return None
 */
IFX_EXTERN void IfxFce_resetModule(Ifx_FCE *fce);

/** \} */

/** \addtogroup IfxLld_Fce_Std_Support
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Reflects the CRC data and returns it
 * \param crcStartValue start value for reflection
 * \param crcLength length of reflected value
 * \return Reflected CRC data
 */
IFX_EXTERN uint32 IfxFce_reflectCrc32(uint32 crcStartValue, uint8 crcLength);

/** \} */

/** \addtogroup IfxLld_Fce_Std_InterruptStatus
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Returns the SRC pointer for FCE
 * \param fce Specifies pointer to FCE module registers
 */
IFX_INLINE volatile Ifx_SRC_SRCR *IfxFce_getSrcPointer(Ifx_FCE *fce);

/** \} */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Specifies pointer to FCE module registers
 * \param fce Specifies the pointer to FCE module handler
 * \param channel Specifies the Channel
 * \return None
 */
IFX_INLINE void IfxFce_clearCrcErrorFlags(Ifx_FCE *fce, IfxFce_CrcChannel channel);

/** \brief Gets the CRC interrupt status
 * \param fce Specifies pointer to FCE module registers
 * \param channel Specifies the Channel
 * \return Return Crc Interrupt Status
 */
IFX_INLINE Ifx_FCE_IN_STS IfxFce_getCrcInterruptStatus(Ifx_FCE *fce, IfxFce_CrcChannel channel);

/** \brief Set the length of over which CRC checksum is calculated.
 * \param fce Specifies pointer to FCE module registers
 * \param channel Specifies the channel
 * \param crcLength Specifies the Length of CRC
 * \return None
 */
IFX_INLINE void IfxFce_setChannelCrcLength(Ifx_FCE *fce, IfxFce_CrcChannel channel, uint32 crcLength);

/** \brief Set the CRC start value
 * \param fce Specifies pointer to FCE module registers
 * \param channel Specifies the channel
 * \param crcStartValue Set the crc value
 * \return None
 */
IFX_INLINE void IfxFce_setCrcstartValue(Ifx_FCE *fce, IfxFce_CrcChannel channel, uint32 crcStartValue);

/** \brief Set expected crc value to be checked.
 * \param fce Specifies pointer to FCE module registers
 * \param channel Specifies the channel
 * \param expectedCrc Expected CRC value to be checked
 * \return None
 */
IFX_INLINE void IfxFce_setExpectedCrc(Ifx_FCE *fce, IfxFce_CrcChannel channel, uint32 expectedCrc);

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE void IfxFce_clearCrcErrorFlags(Ifx_FCE *fce, IfxFce_CrcChannel channel)
{
    fce->IN[channel].STS.U = 0x00000000U;
}


IFX_INLINE void IfxFce_disableModule(Ifx_FCE *fce)
{
    uint16 password = IfxScuWdt_getCpuWatchdogPassword();
    IfxScuWdt_clearCpuEndinit(password);
    fce->CLC.B.DISR = 1;
    IfxScuWdt_setCpuEndinit(password);
}


IFX_INLINE void IfxFce_enableModule(Ifx_FCE *fce)
{
    uint16 password = IfxScuWdt_getCpuWatchdogPassword();
    IfxScuWdt_clearCpuEndinit(password);
    fce->CLC.B.DISR = 0;
    IfxScuWdt_setCpuEndinit(password);
}


IFX_INLINE Ifx_FCE_IN_STS IfxFce_getCrcInterruptStatus(Ifx_FCE *fce, IfxFce_CrcChannel channel)
{
    Ifx_FCE_IN_STS interruptStatus;
    interruptStatus.U = fce->IN[channel].STS.U;

    return interruptStatus;
}


IFX_INLINE volatile Ifx_SRC_SRCR *IfxFce_getSrcPointer(Ifx_FCE *fce)
{
    // FIXME: There are 2 FCEs in AurixPlus. Currently HardCoded to FCE0. Needs to be adapted
    return &SRC_FCE0;
}


IFX_INLINE void IfxFce_setChannelCrcLength(Ifx_FCE *fce, IfxFce_CrcChannel channel, uint32 crcLength)
{
    /*write the dafault value 0xFACECAFE to the register */
    fce->IN[channel].LENGTH.U = 0xFACECAFE;
    fce->IN[channel].LENGTH.U = crcLength;
}


IFX_INLINE void IfxFce_setCrcstartValue(Ifx_FCE *fce, IfxFce_CrcChannel channel, uint32 crcStartValue)
{
    fce->IN[channel].CRC.U = crcStartValue;
}


IFX_INLINE void IfxFce_setExpectedCrc(Ifx_FCE *fce, IfxFce_CrcChannel channel, uint32 expectedCrc)
{
    /*write the dafault value 0xFACECAFE to the register */
    fce->IN[channel].CHECK.U = 0xFACECAFE;
    fce->IN[channel].CHECK.U = expectedCrc;
}


#endif /* IFXFCE_H */
