/**
 * \file IfxGtm_Spe.h
 * \brief GTM  basic functionality
 * \ingroup IfxLld_Gtm
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
 *
 * This Module provides the standard interfaces to manipulate and read SPE sub-module registers in GTM.
 * This Module can be used to abstract the register names and addressing from higher layer software code.
 *
 * \defgroup IfxLld_Gtm_Std_Spe Spe Basic Functionality
 * \ingroup IfxLld_Gtm_Std
 * \defgroup IfxLld_Gtm_Std_Spe_SPEDataStructures SPE Data Structures
 * \ingroup IfxLld_Gtm_Std_Spe
 * \defgroup IfxLld_Gtm_Std_Spe_SPEEnumerations SPE Enumerations
 * \ingroup IfxLld_Gtm_Std_Spe
 * \defgroup IfxLld_Gtm_Std_Spe_SPEFunctions SPE Functions
 * \ingroup IfxLld_Gtm_Std_Spe
 */

#ifndef IFXGTM_SPE_H
#define IFXGTM_SPE_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "_Impl/IfxGtm_cfg.h"
#include "IfxGtm.h"
#include "Cpu/Std/IfxCpu_Intrinsics.h"
#include "IfxGtm_bf.h"
#include "Src/Std/IfxSrc.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/** \brief Macro to get the TRIG_SEL value in CTRL_STAT register.
 * Input(x): Enum value of IfxGtm_Spe_TriggerSource
 */
#define IFXGTM_SPE_GET_TRIGSEL(x)                                    ((uint32)(x & (uint32)3))

/** \brief Macro to get the ETRIG_SEL value in CTRL_STAT register.
 * Input(x): Enum value of IfxGtm_Spe_TriggerSource
 */
#define IFXGTM_SPE_GET_ETRIG(x)                                      ((uint32)((x & (uint32)4) >> 2))

/** \brief sets a single input pattern into the SPE_PAT register.
 */
#define IFXGTM_SPE_SETSINGLEPATTERN(spepat_temp, pattern, pat_index) ((spepat_temp | ((uint32)0xFu << (pat_index * 4))) & (pattern << (pat_index * 4)))

/** \brief sets all the input patterns specified into the SPE_PAT register.
 * This can be used to derive SPE_PAT from pattern array specified by user.
 */
#define IFXGTM_SPE_GETALLPATTERNS(pat_ptr) \
    ((pat_ptr->U) |                        \
     ((pat_ptr + 1)->U << 4) |             \
     ((pat_ptr + 2)->U << 8) |             \
     ((pat_ptr + 3)->U << 12) |            \
     ((pat_ptr + 4)->U << 16) |            \
     ((pat_ptr + 5)->U << 20) |            \
     ((pat_ptr + 6)->U << 24) |            \
     ((pat_ptr + 7)->U << 28))

/** \brief macro used to set a single channel output into the SPE OUT PAT register.
 */
#define IFXGTM_SPE_SETCHANNELOUTPUT(speoutpat, output, ch)           ((speoutpat | ((uint32)0x3u << (ch * 2))) & ((uint32)output << (ch * 2)))

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Gtm_Std_Spe_SPEEnumerations
 * \{ */
/** \brief enumeration containing TOM channel output types possible from SPE.
 * Used to specify the Channel outputs in individual SPE_OUT_PAT registers.
 */
typedef enum
{
    IfxGtm_Spe_ChannelOutputType_tomCh0Sour = 0,  /**< \brief set output to TOM Channel 0 SOUR output */
    IfxGtm_Spe_ChannelOutputType_tomCh1Sour = 1,  /**< \brief set output to TOM Channel 1 SOUR output */
    IfxGtm_Spe_ChannelOutputType_0          = 2,  /**< \brief set output to 0 */
    IfxGtm_Spe_ChannelOutputType_1          = 3   /**< \brief set output to 1 */
} IfxGtm_Spe_ChannelOutputType;

/** \brief Specifies the type of control command which selects the output pattern register(OUT_PAT) used to udpate OUT_CTRL.
 * Used to fill in the SPE_CTRL_CMD field in SPE_CMD register.
 */
typedef enum
{
    IfxGtm_Spe_ControlCommand_patPtr    = 0,  /**< \brief use SPE_PAT_PTR to select OUT_PAT */
    IfxGtm_Spe_ControlCommand_bwdPatPtr = 1,  /**< \brief Use SPE_PAT_PTR_BWD to select OUT_PAT */
    IfxGtm_Spe_ControlCommand_outPat6   = 2,  /**< \brief Select OUT_PAT_6 directly. */
    IfxGtm_Spe_ControlCommand_outPat7   = 3   /**< \brief Select OUT_PAT_7 directly. */
} IfxGtm_Spe_ControlCommand;

/** \brief Describes the rotation direction of the input signal patterns.
 */
typedef enum
{
    IfxGtm_Spe_Direction_fwd = 0,  /**< \brief Rotation direction is forward (Incrementing pattern) */
    IfxGtm_Spe_Direction_bwd = 1   /**< \brief Rotation direction is backwards (decrementing Pattern) */
} IfxGtm_Spe_Direction;

/** \brief Input signal which is mapped from TIMj_CH(48)
 * Used to set GTM.SPE.CTRL_STAT.B.SIEx
 */
typedef enum
{
    IfxGtm_Spe_InputSignal_chX = 0,  /**< \brief Select for CHx */
    IfxGtm_Spe_InputSignal_chY = 1,  /**< \brief select for CHy */
    IfxGtm_Spe_InputSignal_chZ = 2   /**< \brief select for CHz */
} IfxGtm_Spe_InputSignal;

/** \brief mode of interrupt - used to specify the IRQ_MODE register.
 */
typedef enum
{
    IfxGtm_Spe_InterruptMode_level       = 0,  /**< \brief level mode */
    IfxGtm_Spe_InterruptMode_pulse       = 1,  /**< \brief Pulse mode */
    IfxGtm_Spe_InterruptMode_pulseNotify = 2,  /**< \brief Pulse Notify mode */
    IfxGtm_Spe_InterruptMode_singlePulse = 3   /**< \brief Single Pulse Mode */
} IfxGtm_Spe_InterruptMode;

/** \brief Enumeration containing interrupt signals from SPE.
 * This is used to manipulate and retrieve interrupt flags from Interrupt control registers in SPE.
 */
typedef enum
{
    IfxGtm_Spe_InterruptSignal_nipd  = 0, /**< \brief new input pattern detected interrupt */
    IfxGtm_Spe_InterruptSignal_dchg  = 1, /**< \brief direction change interrupt */
    IfxGtm_Spe_InterruptSignal_perr  = 2, /**< \brief Pattern error: invalid input pattern interrupt */
    IfxGtm_Spe_InterruptSignal_bis   = 3, /**< \brief Bouncing Input signal interrupt */
    IfxGtm_Spe_InterruptSignal_rcmp  = 4, /**< \brief Revolution compare match interrupt */
    IfxGtm_Spe_InterruptSignal_count = 5  /**< \brief Number of interrupt signals from SPE. */
} IfxGtm_Spe_InterruptSignal;

/** \brief SPE module output channel number.
 */
typedef enum
{
    IfxGtm_Spe_OutChannel_0,     /**< \brief SPE output channel 0  */
    IfxGtm_Spe_OutChannel_1,     /**< \brief SPE output channel 1  */
    IfxGtm_Spe_OutChannel_2,     /**< \brief SPE output channel 2  */
    IfxGtm_Spe_OutChannel_3,     /**< \brief SPE output channel 3  */
    IfxGtm_Spe_OutChannel_4,     /**< \brief SPE output channel 4  */
    IfxGtm_Spe_OutChannel_5,     /**< \brief SPE output channel 5  */
    IfxGtm_Spe_OutChannel_6,     /**< \brief SPE output channel 6  */
    IfxGtm_Spe_OutChannel_7      /**< \brief SPE output channel 7  */
} IfxGtm_Spe_OutChannel;

/** \brief Pattern Index Number of the  valid Input sequence to the SPE.
 * range from 0-7.
 */
typedef enum
{
    IfxGtm_Spe_PatternIndex_0,     /**< \brief SPE Pattern Index 0  */
    IfxGtm_Spe_PatternIndex_1,     /**< \brief SPE Pattern Index 1  */
    IfxGtm_Spe_PatternIndex_2,     /**< \brief SPE Pattern Index 2  */
    IfxGtm_Spe_PatternIndex_3,     /**< \brief SPE Pattern Index 3  */
    IfxGtm_Spe_PatternIndex_4,     /**< \brief SPE Pattern Index 4  */
    IfxGtm_Spe_PatternIndex_5,     /**< \brief SPE Pattern Index 5  */
    IfxGtm_Spe_PatternIndex_6,     /**< \brief SPE Pattern Index 6  */
    IfxGtm_Spe_PatternIndex_7      /**< \brief SPE Pattern Index 7  */
} IfxGtm_Spe_PatternIndex;

/** \} */

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Gtm_Std_Spe_SPEDataStructures
 * \{ */
/** \brief specifies the output pattern structure.
 */
typedef struct
{
    uint32 CH0 : 2;     /**< \brief output from Channel 0 */
    uint32 CH1 : 2;     /**< \brief output from Channel 0 */
    uint32 CH2 : 2;     /**< \brief output from Channel 2 */
    uint32 CH3 : 2;     /**< \brief output from Channel 3 */
    uint32 CH4 : 2;     /**< \brief output from Channel 4 */
    uint32 CH5 : 2;     /**< \brief output from Channel 5 */
    uint32 CH6 : 2;     /**< \brief output from Channel 6 */
    uint32 CH7 : 2;     /**< \brief output from Channel 7 */
} IfxGtm_Spe_ChannelOutputs;

/** \brief Struct describing the individual input signals in the pattern
 */
typedef struct
{
    uint32 valid : 1;     /**< \brief validity of the specified Pattern */
    uint32 x : 1;         /**< \brief CHx input Signal */
    uint32 y : 1;         /**< \brief CHy input Signal */
    uint32 z : 1;         /**< \brief CHz input Signal */
} IfxGtm_Spe_PatternSignal;

/** \} */

/** \addtogroup IfxLld_Gtm_Std_Spe_SPEDataStructures
 * \{ */
/** \brief Struct describing the pattern of Input signal to the SPE submodule.
 */
typedef union
{
    IfxGtm_Spe_PatternSignal Signal;       /**< \brief Individual Pattern Signals. */
    uint32                   U;            /**< \brief read pattern as word */
} IfxGtm_Spe_InputPattern;

/** \brief specifies the output pattern to be given to the TOM output from given SPE.
 * Union implementation can be read in word or individual channel output units.
 */
typedef union
{
    IfxGtm_Spe_ChannelOutputs Outputs;       /**< \brief Structure containing individual channel outputs. */
    uint32                    U;             /**< \brief word */
} IfxGtm_Spe_OutputPattern;

/** \} */

/** \addtogroup IfxLld_Gtm_Std_Spe_SPEFunctions
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief clears all the Interrupts
 * \param spe pointer to the spe device.
 * \return None
 */
IFX_INLINE void IfxGtm_Spe_clearAllInterrupts(Ifx_GTM_SPE *spe);

/** \brief clears the specific interrupt flag.
 * \param spe pointer to the spe device.
 * \param interrupt interrupt signal
 * \return None
 */
IFX_INLINE void IfxGtm_Spe_clearInterrupt(Ifx_GTM_SPE *spe, IfxGtm_Spe_InterruptSignal interrupt);

/** \brief enables/disables the SPE module based on boolean input.
 * \param spe pointer to spe device.
 * \param enabled enable status for SPE sub module
 * \return None
 */
IFX_INLINE void IfxGtm_Spe_enableModule(Ifx_GTM_SPE *spe, boolean enabled);

/** \brief returns the actual input pattern as a word format.
 * \param spe pointer to the spe device.
 * \return actual input pattern
 */
IFX_INLINE uint32 IfxGtm_Spe_getActualInputPat(Ifx_GTM_SPE *spe);

/** \brief returns the backwards pattern pointer.
 * \param spe pointer to the spe device.
 * \return current pattern pointer
 */
IFX_INLINE uint32 IfxGtm_Spe_getBwdPatternPtr(Ifx_GTM_SPE *spe);

/** \brief retrieves the Control command used to select Pattern pointer which selects OUT_PAT.
 * \param spe pointer to the spe device.
 * \return control command selection
 */
IFX_INLINE IfxGtm_Spe_ControlCommand IfxGtm_Spe_getControlCommand(Ifx_GTM_SPE *spe);

/** \brief gets the actual rotation direction of the input signals.
 * \param spe pointer to the spe device.
 * \return Direction
 */
IFX_INLINE IfxGtm_Spe_Direction IfxGtm_Spe_getDir(Ifx_GTM_SPE *spe);

/** \brief gets the new input pattern in word format.
 * \param spe pointer to the spe device.
 * \return new input pattern
 */
IFX_INLINE uint32 IfxGtm_Spe_getNewInputPat(Ifx_GTM_SPE *spe);

/** \brief gets the output control register (OUT_CTRL) as a whole word to read the curent output patttern.
 * \param spe pointer to the spe device
 * \return output control register as whole word
 */
IFX_INLINE uint32 IfxGtm_Spe_getOutputCtrl(Ifx_GTM_SPE *spe);

/** \brief returns the output pattern for the specified index as a whole word.
 * \param spe pointer to the spe device
 * \param patIndex step or index number of the pattern specified.
 * \return output pattern as whole word
 */
IFX_INLINE uint32 IfxGtm_Spe_getOutputPattern(Ifx_GTM_SPE *spe, IfxGtm_Spe_PatternIndex patIndex);

/** \brief returns the pattern pointer which indicates the currently pointed SPE_OUT_PAT register.
 * \param spe pointer to the spe device.
 * \return current pattern pointer
 */
IFX_INLINE uint32 IfxGtm_Spe_getPatternPtr(Ifx_GTM_SPE *spe);

/** \brief gets the previous rotation direction.
 * \param spe pointer to the spe device.
 * \return Previous Direction
 */
IFX_INLINE IfxGtm_Spe_Direction IfxGtm_Spe_getPrevDir(Ifx_GTM_SPE *spe);

/** \brief gets the previous input pattern in word format.
 * \param spe pointer to the spe device.
 * \return previous input pattern
 */
IFX_INLINE uint32 IfxGtm_Spe_getPreviousInputPat(Ifx_GTM_SPE *spe);

/** \brief returns the step counter as a 32 bit word
 * \param spe pointer to the spe device
 * \return step counter
 */
IFX_INLINE uint32 IfxGtm_Spe_getRevCounter(Ifx_GTM_SPE *spe);

/** \brief Get the Pointer to the Interrupt Request Node of specified SPE object.
 * \param spe Spe Index Number
 * \return SRC pointer
 */
IFX_INLINE volatile Ifx_SRC_SRCR *IfxGtm_Spe_getSrcPointer(IfxGtm_Spe spe);

/** \brief selects the Tim Input Channels to give input to the SPE submodule.
 * \param spe pointer to the spe device.
 * \param timInput TIM input signal to be selected
 * \return None
 */
IFX_INLINE void IfxGtm_Spe_selectTimInput(Ifx_GTM_SPE *spe, IfxGtm_Spe_TimInput timInput);

/** \brief sets the Backward pattern pointer to the CTRL_STAT2 register. This will be used to command if control command is set to backwards in CTRL_CMD register.
 * \param spe pointer to the spe device
 * \param patIndex pattern index number
 * \return None
 */
IFX_INLINE void IfxGtm_Spe_setBwdPatternPtr(Ifx_GTM_SPE *spe, IfxGtm_Spe_PatternIndex patIndex);

/** \brief set the CTRL_CMD field to select which pattern pointer is used to select OUT_PAT.
 * \param spe pointer to the spe device.
 * \param cmd control command selection
 * \return None
 */
IFX_INLINE void IfxGtm_Spe_setControlCommand(Ifx_GTM_SPE *spe, IfxGtm_Spe_ControlCommand cmd);

/** \brief Used to set the Fast shutoff Mode.
 * True: Enable Fast Shutoff
 * False: Disable Fast Shutoff
 * \param spe pointer to the spe device
 * \param enabled status of Fast shutoff
 * \return None
 */
IFX_INLINE void IfxGtm_Spe_setFastShutoff(Ifx_GTM_SPE *spe, boolean enabled);

/** \brief sets the desired mode as the interrupt mode from SPE submodule.
 * \param spe pointer to the spe device
 * \param mode interrupt mode
 * \return None
 */
IFX_INLINE void IfxGtm_Spe_setInterruptMode(Ifx_GTM_SPE *spe, IfxGtm_Spe_InterruptMode mode);

/** \brief writes into the out_ctrl register with the specified output pattern.
 * \param spe pointer to the spe device
 * \param patPtr pointer to the output pattern
 * \return None
 */
IFX_INLINE void IfxGtm_Spe_setOutputCtrl(Ifx_GTM_SPE *spe, IfxGtm_Spe_OutputPattern *patPtr);

/** \brief sets the Pattern Index into the CTRL_STAT register, which will in turn point to the corresponding SPE_OUT_PAT register.
 * \param spe pointer to the spe device
 * \param patIndex pattern index number
 * \return None
 */
IFX_INLINE void IfxGtm_Spe_setPatternPtr(Ifx_GTM_SPE *spe, IfxGtm_Spe_PatternIndex patIndex);

/** \brief sets the step counter to a specific value.
 * \param spe pointer to the spe device
 * \param count step counter value.
 * \return None
 */
IFX_INLINE void IfxGtm_Spe_setRevCounter(Ifx_GTM_SPE *spe, uint32 count);

/** \brief Trigger the update of OUT_CTRL register with the OUT_PAT selected by Control Command.
 * \param spe pointer to the spe device.
 * \return None
 */
IFX_INLINE void IfxGtm_Spe_triggerUpdate(Ifx_GTM_SPE *spe);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief disables the SPE interrupt of specified type.
 * \param spe pointer to the spe device
 * \param interrupt interrupt signal
 * \return None
 */
IFX_EXTERN void IfxGtm_Spe_disableInterrupt(Ifx_GTM_SPE *spe, IfxGtm_Spe_InterruptSignal interrupt);

/** \brief enables the error interrupt of the specified type.
 * \param spe pointer to the spe device
 * \param interrupt interrupt signal
 * \param enabled interrupt enable status
 * \return None
 */
IFX_EXTERN void IfxGtm_Spe_enableErrorInterrupt(Ifx_GTM_SPE *spe, IfxGtm_Spe_InterruptSignal interrupt, boolean enabled);

/** \brief enables/disables the input to sPE from the selected input signal source.
 * \param spe pointer to spe device
 * \param inputSignal Input signal
 * \param enabled enable signal
 * \return None
 */
IFX_EXTERN void IfxGtm_Spe_enableInput(Ifx_GTM_SPE *spe, IfxGtm_Spe_InputSignal inputSignal, boolean enabled);

/** \brief enables the SPE interrupt of specified type.
 * \param spe pointer to the spe device
 * \param interrupt interrupt signal
 * \return None
 */
IFX_EXTERN void IfxGtm_Spe_enableInterrupt(Ifx_GTM_SPE *spe, IfxGtm_Spe_InterruptSignal interrupt);

/** \brief return fast shutoff level.
 * \param spe pointer to the spe device.
 * \param channel TOM[x] output channel
 * \return fast shutoff level
 */
IFX_EXTERN uint32 IfxGtm_Spe_getFastShutoffLevel(Ifx_GTM_SPE *spe, IfxGtm_Spe_OutChannel channel);

/** \brief returns the interrupt status of specified interrupt signal
 * \param spe pointer to the spe device
 * \param interrupt interrupt signal
 * \return interrupt status
 */
IFX_EXTERN boolean IfxGtm_Spe_getInterruptStatus(Ifx_GTM_SPE *spe, IfxGtm_Spe_InterruptSignal interrupt);

/** \brief returns the step counter compare value as a 32 bit word.
 * \param spe pointer to the spe device
 * \return step counter
 */
IFX_EXTERN uint32 IfxGtm_Spe_getRevCompare(Ifx_GTM_SPE *spe);

/** \brief selects the source which will trigger the spe control output.a
 * \param spe pointer to spe device
 * \param trigger trigger source
 * \return None
 */
IFX_EXTERN void IfxGtm_Spe_selectTrigger(Ifx_GTM_SPE *spe, IfxGtm_Spe_TriggerSource trigger);

/** \brief sets all the specified input patterns into the SPE_PAT register
 * \param spe pointer to the spe device
 * \param patPtr pointer to the pattern specified
 * \return None
 */
IFX_EXTERN void IfxGtm_Spe_setAllInputPatterns(Ifx_GTM_SPE *spe, IfxGtm_Spe_InputPattern *patPtr);

/** \brief writes SPE[i]_PAT with the individual pattern specified according to the pattern index.
 * \param spe pointer to the spe device
 * \param patPtr pointer to the pattern specified
 * \param patIndex step or index number of the pattern specified.
 * \return None
 */
IFX_EXTERN void IfxGtm_Spe_setInputPattern(Ifx_GTM_SPE *spe, IfxGtm_Spe_InputPattern *patPtr, IfxGtm_Spe_PatternIndex patIndex);

/** \brief forces the interrupt of specified type by software
 * \param spe pointer to the spe device
 * \param interrupt interrupt signal
 * \return None
 */
IFX_EXTERN void IfxGtm_Spe_setInterrupt(Ifx_GTM_SPE *spe, IfxGtm_Spe_InterruptSignal interrupt);

/** \brief sets the output pattern from pointer to the corresponding SPE_OUT_PAT register specified by the index.
 * \param spe pointer to the spe device
 * \param patPtr pointer to the output pattern
 * \param patIndex step or index number of the pattern specified.
 * \return None
 */
IFX_EXTERN void IfxGtm_Spe_setOutputPattern(Ifx_GTM_SPE *spe, IfxGtm_Spe_OutputPattern *patPtr, IfxGtm_Spe_PatternIndex patIndex);

/** \brief sets the step compare count to the specified value.
 * \param spe pointer to the spe device
 * \param count step counter value.
 * \return None
 */
IFX_EXTERN void IfxGtm_Spe_setRevCompare(Ifx_GTM_SPE *spe, uint32 count);

/** \} */

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE void IfxGtm_Spe_clearAllInterrupts(Ifx_GTM_SPE *spe)
{
    spe->IRQ.NOTIFY.U |= (uint32)0x1Fu;
}


IFX_INLINE void IfxGtm_Spe_clearInterrupt(Ifx_GTM_SPE *spe, IfxGtm_Spe_InterruptSignal interrupt)
{
    spe->IRQ.NOTIFY.U |= ((uint32)1 << (uint32)interrupt);
}


IFX_INLINE void IfxGtm_Spe_enableModule(Ifx_GTM_SPE *spe, boolean enabled)
{
    spe->CTRL_STAT.B.EN = (uint32)enabled;
}


IFX_INLINE uint32 IfxGtm_Spe_getActualInputPat(Ifx_GTM_SPE *spe)
{
    return (uint32)(spe->CTRL_STAT.B.AIP);
}


IFX_INLINE uint32 IfxGtm_Spe_getBwdPatternPtr(Ifx_GTM_SPE *spe)
{
    return (uint32)(spe->CTRL_STAT2.B.SPE_PAT_PTR_BWD);
}


IFX_INLINE IfxGtm_Spe_ControlCommand IfxGtm_Spe_getControlCommand(Ifx_GTM_SPE *spe)
{
    return (IfxGtm_Spe_ControlCommand)(spe->CMD.B.SPE_CTRL_CMD);
}


IFX_INLINE IfxGtm_Spe_Direction IfxGtm_Spe_getDir(Ifx_GTM_SPE *spe)
{
    return (IfxGtm_Spe_Direction)(spe->CTRL_STAT.B.ADIR);
}


IFX_INLINE uint32 IfxGtm_Spe_getNewInputPat(Ifx_GTM_SPE *spe)
{
    return (uint32)(spe->CTRL_STAT.B.NIP);
}


IFX_INLINE uint32 IfxGtm_Spe_getOutputCtrl(Ifx_GTM_SPE *spe)
{
    return spe->OUT_CTRL.U;
}


IFX_INLINE uint32 IfxGtm_Spe_getOutputPattern(Ifx_GTM_SPE *spe, IfxGtm_Spe_PatternIndex patIndex)
{
    return spe->OUT_PAT[patIndex].U;
}


IFX_INLINE uint32 IfxGtm_Spe_getPatternPtr(Ifx_GTM_SPE *spe)
{
    return (uint32)(spe->CTRL_STAT.B.SPE_PAT_PTR);
}


IFX_INLINE IfxGtm_Spe_Direction IfxGtm_Spe_getPrevDir(Ifx_GTM_SPE *spe)
{
    return (IfxGtm_Spe_Direction)(spe->CTRL_STAT.B.PDIR);
}


IFX_INLINE uint32 IfxGtm_Spe_getPreviousInputPat(Ifx_GTM_SPE *spe)
{
    return (uint32)(spe->CTRL_STAT.B.PIP);
}


IFX_INLINE uint32 IfxGtm_Spe_getRevCounter(Ifx_GTM_SPE *spe)
{
    return (uint32)(spe->REV_CNT.B.REV_CNT);
}


IFX_INLINE volatile Ifx_SRC_SRCR *IfxGtm_Spe_getSrcPointer(IfxGtm_Spe spe)
{
    return (Ifx_SRC_SRCR *)(&(MODULE_SRC.GTM_SPEIRQ[spe]));
}


IFX_INLINE void IfxGtm_Spe_selectTimInput(Ifx_GTM_SPE *spe, IfxGtm_Spe_TimInput timInput)
{
    spe->CTRL_STAT.B.TIM_SEL = (uint32)(timInput & (uint32)1);
}


IFX_INLINE void IfxGtm_Spe_setBwdPatternPtr(Ifx_GTM_SPE *spe, IfxGtm_Spe_PatternIndex patIndex)
{
    spe->CTRL_STAT2.B.SPE_PAT_PTR_BWD = (uint32)patIndex;
}


IFX_INLINE void IfxGtm_Spe_setControlCommand(Ifx_GTM_SPE *spe, IfxGtm_Spe_ControlCommand cmd)
{
    spe->CMD.B.SPE_CTRL_CMD = (uint32)cmd;
}


IFX_INLINE void IfxGtm_Spe_setFastShutoff(Ifx_GTM_SPE *spe, boolean enabled)
{
    spe->CTRL_STAT.B.FSOM = (uint32)enabled;
}


IFX_INLINE void IfxGtm_Spe_setInterruptMode(Ifx_GTM_SPE *spe, IfxGtm_Spe_InterruptMode mode)
{
    spe->IRQ.MODE.B.IRQ_MODE = (uint32)mode;
}


IFX_INLINE void IfxGtm_Spe_setOutputCtrl(Ifx_GTM_SPE *spe, IfxGtm_Spe_OutputPattern *patPtr)
{
    spe->OUT_CTRL.U = patPtr->U;
}


IFX_INLINE void IfxGtm_Spe_setPatternPtr(Ifx_GTM_SPE *spe, IfxGtm_Spe_PatternIndex patIndex)
{
    spe->CTRL_STAT.B.SPE_PAT_PTR = (uint32)patIndex;
}


IFX_INLINE void IfxGtm_Spe_setRevCounter(Ifx_GTM_SPE *spe, uint32 count)
{
    spe->REV_CNT.B.REV_CNT = count;
}


IFX_INLINE void IfxGtm_Spe_triggerUpdate(Ifx_GTM_SPE *spe)
{
    spe->CMD.B.SPE_UPD_TRIG = (uint32)1;
}


#endif /* IFXGTM_SPE_H */
