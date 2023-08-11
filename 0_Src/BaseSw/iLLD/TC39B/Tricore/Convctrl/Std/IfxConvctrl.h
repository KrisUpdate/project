/**
 * \file IfxConvctrl.h
 * \brief CONVCTRL  basic functionality
 * \ingroup IfxLld_Convctrl
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
 * \defgroup IfxLld_Convctrl_Std_ConfigurationFunctions Configuration Functions
 * \ingroup IfxLld_Convctrl_Std
 * \defgroup IfxLld_Convctrl_Std_Initialization Initialization Functions
 * \ingroup IfxLld_Convctrl_Std
 * \defgroup IfxLld_Convctrl_Std_Enumerations Enums
 * \ingroup IfxLld_Convctrl_Std
 */

#ifndef IFXCONVCTRL_H
#define IFXCONVCTRL_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "_Impl/IfxConvctrl_cfg.h"
#include "IfxConverter_reg.h"
#include "Cpu/Std/IfxCpu_Intrinsics.h"
#include "_Utilities/Ifx_Assert.h"
#include "IfxConverter_bf.h"
#include "Scu/Std/IfxScuCcu.h"
#include "Scu/Std/IfxScuWdt.h"

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Convctrl_Std_Enumerations
 * \{ */
/** \brief Prescaling factor between the peripheral clock and the phase synchronization signal.To be filled in PHSCFG.B.PHSDIV
 */
typedef enum
{
    IfxConvctrl_PhaseSynchronizerDivider_off = 0,  /**< \brief Off, the phase synchronization signal is constantly active */
    IfxConvctrl_PhaseSynchronizerDivider_2   = 1,  /**< \brief The phase synchronization signal is generated at fPER / 2 */
    IfxConvctrl_PhaseSynchronizerDivider_3   = 2,  /**< \brief The phase synchronization signal is generated at fPER / 3 */
    IfxConvctrl_PhaseSynchronizerDivider_4   = 3,  /**< \brief The phase synchronization signal is generated at fPER / 4 */
    IfxConvctrl_PhaseSynchronizerDivider_5   = 4,  /**< \brief The phase synchronization signal is generated at fPER / 5 */
    IfxConvctrl_PhaseSynchronizerDivider_6   = 5,  /**< \brief The phase synchronization signal is generated at fPER / 6 */
    IfxConvctrl_PhaseSynchronizerDivider_7   = 6,  /**< \brief The phase synchronization signal is generated at fPER / 7 */
    IfxConvctrl_PhaseSynchronizerDivider_8   = 7,  /**< \brief The phase synchronization signal is generated at fPER / 8 */
    IfxConvctrl_PhaseSynchronizerDivider_9   = 8,  /**< \brief The phase synchronization signal is generated at fPER / 9 */
    IfxConvctrl_PhaseSynchronizerDivider_10  = 9,  /**< \brief The phase synchronization signal is generated at fPER / 10 */
    IfxConvctrl_PhaseSynchronizerDivider_11  = 10, /**< \brief The phase synchronization signal is generated at fPER / 11 */
    IfxConvctrl_PhaseSynchronizerDivider_12  = 11, /**< \brief The phase synchronization signal is generated at fPER / 12 */
    IfxConvctrl_PhaseSynchronizerDivider_13  = 12, /**< \brief The phase synchronization signal is generated at fPER / 13 */
    IfxConvctrl_PhaseSynchronizerDivider_14  = 13, /**< \brief The phase synchronization signal is generated at fPER / 14 */
    IfxConvctrl_PhaseSynchronizerDivider_15  = 14, /**< \brief The phase synchronization signal is generated at fPER / 15 */
    IfxConvctrl_PhaseSynchronizerDivider_16  = 15  /**< \brief The phase synchronization signal is generated at fPER / 15 */
} IfxConvctrl_PhaseSynchronizerDivider;

/** \brief Modes of sensitivity to the suspend signal coming from the OCDS Trigger Switch (OTGS).
 * To be filled in OCS.B.SUS
 */
typedef enum
{
    IfxConvctrl_SuspendMode_noSuspend = 0,  /**< \brief Module will not suspent */
    IfxConvctrl_SuspendMode_1         = 1,  /**< \brief Suspend Mode 1 : Stop generating synchronization pulses */
    IfxConvctrl_SuspendMode_2         = 2   /**< \brief Disable the Phase Synchronizer */
} IfxConvctrl_SuspendMode;

/** \} */

/** \addtogroup IfxLld_Convctrl_Std_ConfigurationFunctions
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief This will disable all the Converter Controller Alarms(from reaching out of module) in addition to clearing the ALF flag.
 * \param convctrl Pointer to Converter Controller SFRs
 * \return None
 */
IFX_INLINE void IfxConvctrl_clearAlarmFlag(Ifx_CONVERTER *convctrl);

/** \brief Access to all CONVCTRL registers is enabled.
 * \param convctrl Pointer to Converter Controller SFRs
 * \return None
 */
IFX_INLINE void IfxConvctrl_enableConfigurationAccess(Ifx_CONVERTER *convctrl);

/** \brief Enables the module clock, thereby enabling the module
 * \param convctrl Pointer to Converter Controller SFRs
 * \return None
 */
IFX_INLINE void IfxConvctrl_enableModule(Ifx_CONVERTER *convctrl);

/** \brief Introducing fault in the Down Counter
 * \param convctrl Pointer to Converter Controller SFRs
 * \return None
 */
IFX_INLINE void IfxConvctrl_injectCounterFault(Ifx_CONVERTER *convctrl);

/** \brief Introducing fault in Phase Synchronizer Divider(PHSDIV)
 * \param convctrl Pointer to Converter Controller SFRs
 * \return None
 */
IFX_INLINE void IfxConvctrl_injectPhaseSyncDividerFault(Ifx_CONVERTER *convctrl);

/**
 * \param convctrl Pointer to Converter Controller SFRs
 * \return TRUE - Safety problem has been detected
 * FALSE - No error indicated
 */
IFX_INLINE boolean IfxConvctrl_isErrorDetected(Ifx_CONVERTER *convctrl);

/**
 * \param convctrl Pointer to Converter Controller SFRs
 */
IFX_INLINE boolean IfxConvctrl_isModuleSuspended(Ifx_CONVERTER *convctrl);

/**
 * \param convctrl Pointer to Converter Controller SFRs
 * \return None
 */
IFX_INLINE void IfxConvctrl_lockConfigurationAccess(Ifx_CONVERTER *convctrl);

/** \brief Set the Phase Synchronizer Divider value
 * \param convctrl Pointer to Converter Controller SFRs
 * \param dividerValue Phase Synchronizer divider value
 * \return None
 */
IFX_INLINE void IfxConvctrl_setDividerValue(Ifx_CONVERTER *convctrl, IfxConvctrl_PhaseSynchronizerDivider dividerValue);

/** \brief Configure the suspend mode
 * \param convctrl Pointer to Converter Controller SFRs
 * \param mode Suspend Modes
 * \return None
 */
IFX_INLINE void IfxConvctrl_setSuspendMode(Ifx_CONVERTER *convctrl, IfxConvctrl_SuspendMode mode);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/**
 * \param convctrl Pointer to Converter Controller SFRs
 * \return None
 */
IFX_EXTERN void IfxConvctrl_resetModule(Ifx_CONVERTER *convctrl);

/** \} */

/** \addtogroup IfxLld_Convctrl_Std_Initialization
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/**
 * \param convctrl Pointer to Converter Controller SFRs
 * \param divisionFactor Prescaling factor between the peripheral clock and the phase synchronization signal.
 * \return None
 */
IFX_INLINE void IfxConvctrl_initializeModule(Ifx_CONVERTER *convctrl, IfxConvctrl_PhaseSynchronizerDivider divisionFactor);

/** \} */

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE void IfxConvctrl_clearAlarmFlag(Ifx_CONVERTER *convctrl)
{
    convctrl->PHSSFTY.B.ALFCLR = 1;
}


IFX_INLINE void IfxConvctrl_enableConfigurationAccess(Ifx_CONVERTER *convctrl)
{
    uint16 passwd = IfxScuWdt_getCpuWatchdogPassword();
    IfxScuWdt_clearCpuEndinit(passwd);

    convctrl->CCCTRL.B.TC = 0xB;

    IfxScuWdt_setCpuEndinit(passwd);
}


IFX_INLINE void IfxConvctrl_enableModule(Ifx_CONVERTER *convctrl)
{
    uint16 passwd = IfxScuWdt_getCpuWatchdogPassword();

    IfxScuWdt_clearCpuEndinit(passwd);
    convctrl->CLC.U = 0x00000000;
    IfxScuWdt_setCpuEndinit(passwd);
}


IFX_INLINE void IfxConvctrl_initializeModule(Ifx_CONVERTER *convctrl, IfxConvctrl_PhaseSynchronizerDivider divisionFactor)
{
    IfxConvctrl_enableModule(convctrl);
    IfxConvctrl_enableConfigurationAccess(convctrl);
    IfxConvctrl_setDividerValue(convctrl, divisionFactor);
    IfxConvctrl_lockConfigurationAccess(convctrl);
}


IFX_INLINE void IfxConvctrl_injectCounterFault(Ifx_CONVERTER *convctrl)
{
    /*Writing 0 into FICN0 and 1 into FICN1 simultaneously*/

    Ifx_CONVERTER_PHSSFTY phssfty;
    phssfty.U           = convctrl->PHSSFTY.U;

    phssfty.B.FICN0     = 0;
    phssfty.B.FICN1     = 1;

    convctrl->PHSSFTY.U = phssfty.U;
}


IFX_INLINE void IfxConvctrl_injectPhaseSyncDividerFault(Ifx_CONVERTER *convctrl)
{
    /*Writing 0 into FIPD0 and 1 into FIPD1 simultaneously*/

    Ifx_CONVERTER_PHSSFTY phssfty;
    phssfty.U           = convctrl->PHSSFTY.U;

    phssfty.B.FIPD0     = 0;
    phssfty.B.FIPD1     = 1;

    convctrl->PHSSFTY.U = phssfty.U;
}


IFX_INLINE boolean IfxConvctrl_isErrorDetected(Ifx_CONVERTER *convctrl)
{
    return convctrl->PHSSFTY.B.ALF;
}


IFX_INLINE boolean IfxConvctrl_isModuleSuspended(Ifx_CONVERTER *convctrl)
{
    return convctrl->OCS.B.SUSSTA;
}


IFX_INLINE void IfxConvctrl_lockConfigurationAccess(Ifx_CONVERTER *convctrl)
{
    uint16 passwd = IfxScuWdt_getCpuWatchdogPassword();
    IfxScuWdt_clearCpuEndinit(passwd);

    convctrl->CCCTRL.B.TC = 0x0;

    IfxScuWdt_setCpuEndinit(passwd);
}


IFX_INLINE void IfxConvctrl_setDividerValue(Ifx_CONVERTER *convctrl, IfxConvctrl_PhaseSynchronizerDivider dividerValue)
{
    Ifx_CONVERTER_PHSCFG phscfg;

    phscfg.U           = convctrl->PHSCFG.U;

    phscfg.B.PDWC      = 1; /*Enabling write access to Phase Synchronizer Divider(PHSDIV) */

    phscfg.B.PHSDIV    = dividerValue;

    convctrl->PHSCFG.U = phscfg.U;
}


IFX_INLINE void IfxConvctrl_setSuspendMode(Ifx_CONVERTER *convctrl, IfxConvctrl_SuspendMode mode)
{
    Ifx_CONVERTER_OCS ocs;

    ocs.U           = convctrl->OCS.U;
    /* remove protection and configure the suspend mode */
    ocs.B.SUS_P     = 1;
    ocs.B.SUS       = mode;
    convctrl->OCS.U = ocs.U;
}


#endif /* IFXCONVCTRL_H */
