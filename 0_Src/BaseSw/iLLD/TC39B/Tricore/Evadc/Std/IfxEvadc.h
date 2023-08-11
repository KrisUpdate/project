/**
 * \file IfxEvadc.h
 * \brief EVADC  basic functionality
 * \ingroup IfxLld_Evadc
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
 * \defgroup IfxLld_Evadc_Std_Enum Enumerations
 * \ingroup IfxLld_Evadc_Std
 * \defgroup IfxLld_Evadc_Std_Global Global Functions
 * \ingroup IfxLld_Evadc_Std
 * \defgroup IfxLld_Evadc_Std_QueueRequest Queue Request Functions
 * \ingroup IfxLld_Evadc_Std
 * \defgroup IfxLld_Evadc_Std_IO IO Pin Configuration Functions
 * \ingroup IfxLld_Evadc_Std
 * \defgroup IfxLld_Evadc_Std_Frequency Frequency Calculation
 * \ingroup IfxLld_Evadc_Std
 * \defgroup IfxLld_Evadc_Std_Group Group Functions
 * \ingroup IfxLld_Evadc_Std
 * \defgroup IfxLld_Evadc_Std_Module Module Functions
 * \ingroup IfxLld_Evadc_Std
 * \defgroup IfxLld_Evadc_Std_Channel Channel Functions
 * \ingroup IfxLld_Evadc_Std
 * \defgroup IfxLld_Evadc_Std_Emux Emux Functions
 * \ingroup IfxLld_Evadc_Std
 * \defgroup IfxLld_Evadc_Std_FastCompare Fast Compare functions
 * \ingroup IfxLld_Evadc_Std
 */

#ifndef IFXEVADC_H
#define IFXEVADC_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "_Impl/IfxEvadc_cfg.h"
#include "_PinMap/IfxEvadc_PinMap.h"
#include "IfxEvadc_bf.h"
#include "Cpu/Std/IfxCpu_Intrinsics.h"
#include "Scu/Std/IfxScuCcu.h"
#include "Scu/Std/IfxScuWdt.h"
#include "IfxCbs_reg.h"

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Evadc_Std_Enum
 * \{ */
/** \brief Defined in MODULE_EVADC.G[x].ARBCFG.B.ANONS and ANONC
 */
typedef enum
{
    IfxEvadc_AnalogConverterMode_off             = 0, /**< \brief Analog Converter off */
    IfxEvadc_AnalogConverterMode_slowStandby     = 1, /**< \brief Slow Standby Mode */
    IfxEvadc_AnalogConverterMode_fastStandby     = 2, /**< \brief Fast Standby Mode */
    IfxEvadc_AnalogConverterMode_normalOperation = 3  /**< \brief Normal operation mode */
} IfxEvadc_AnalogConverterMode;

/** \brief Arbitration round length defined in MODULE_EVADC.G[x].ARBCFG.ARBRND(x=0,1,..,11)
 */
typedef enum
{
    IfxEvadc_ArbitrationRounds_4_slots  = 0,  /**< \brief An arbitration round contains 4 arbitration slots. */
    IfxEvadc_ArbitrationRounds_8_slots  = 1,  /**< \brief An arbitration round contains 8 arbitration slots. */
    IfxEvadc_ArbitrationRounds_16_slots = 2,  /**< \brief An arbitration round contains 16 arbitration slots. */
    IfxEvadc_ArbitrationRounds_20_slots = 3   /**< \brief An arbitration round contains 20 arbitration slots. */
} IfxEvadc_ArbitrationRounds;

/** \brief Boundary Extension defined in MODULE_EVADC.G[x].CHCTR[y].B.BNDSELX(x=0,1,...,11;y=0,1....,16)
 */
typedef enum
{
    IfxEvadc_BoundaryExtension_standard            = 0,   /**< \brief Boundary Standard mode. BNDSELU/BNDSELL as Boundaries */
    IfxEvadc_BoundaryExtension_fastCompareResult1  = 1,   /**< \brief Fast compare mode use as upper boundary  Channel result   1 */
    IfxEvadc_BoundaryExtension_fastCompareResult2  = 2,   /**< \brief Fast compare mode use as upper boundary  Channel result   2 */
    IfxEvadc_BoundaryExtension_fastCompareResult3  = 3,   /**< \brief Fast compare mode use as upper boundary  Channel result   3 */
    IfxEvadc_BoundaryExtension_fastCompareResult4  = 4,   /**< \brief Fast compare mode use as upper boundary  Channel result   4 */
    IfxEvadc_BoundaryExtension_fastCompareResult5  = 5,   /**< \brief Fast compare mode use as upper boundary  Channel result   5 */
    IfxEvadc_BoundaryExtension_fastCompareResult6  = 6,   /**< \brief Fast compare mode use as upper boundary  Channel result   6 */
    IfxEvadc_BoundaryExtension_fastCompareResult7  = 7,   /**< \brief Fast compare mode use as upper boundary  Channel result   7 */
    IfxEvadc_BoundaryExtension_fastCompareResult8  = 8,   /**< \brief Fast compare mode use as upper boundary  Channel result   8 */
    IfxEvadc_BoundaryExtension_fastCompareResult9  = 9,   /**< \brief Fast compare mode use as upper boundary  Channel result   9 */
    IfxEvadc_BoundaryExtension_fastCompareResult10 = 10,  /**< \brief Fast compare mode use as upper boundary  Channel result  10 */
    IfxEvadc_BoundaryExtension_fastCompareResult11 = 11,  /**< \brief Fast compare mode use as upper boundary  Channel result  11 */
    IfxEvadc_BoundaryExtension_fastCompareResult12 = 12,  /**< \brief Fast compare mode use as upper boundary  Channel result  12 */
    IfxEvadc_BoundaryExtension_fastCompareResult13 = 13,  /**< \brief Fast compare mode use as upper boundary  Channel result  13 */
    IfxEvadc_BoundaryExtension_fastCompareResult14 = 14,  /**< \brief Fast compare mode use as upper boundary  Channel result  14 */
    IfxEvadc_BoundaryExtension_fastCompareResult15 = 15   /**< \brief Fast compare mode use as upper boundary  Channel result  15 */
} IfxEvadc_BoundaryExtension;

/** \brief BoundarySel defined in MODULE_EVADC.G[x].CHCTR[y].B.BNDSELL(x=0,1,...,11;y=0,1....,16)
 */
typedef enum
{
    IfxEvadc_BoundarySelection_group0  = 0,  /**< \brief Use group class 0 */
    IfxEvadc_BoundarySelection_group1  = 1,  /**< \brief Use group class 1 */
    IfxEvadc_BoundarySelection_global0 = 2,  /**< \brief Use global class 0 */
    IfxEvadc_BoundarySelection_global1 = 3   /**< \brief Use global class 1 */
} IfxEvadc_BoundarySelection;

/** \brief EVADC Channels
 */
typedef enum
{
    IfxEvadc_ChannelId_none = -1,  /**< \brief None of EVADC channels */
    IfxEvadc_ChannelId_0    = 0,   /**< \brief Channel 0 */
    IfxEvadc_ChannelId_1    = 1,   /**< \brief Channel 1 */
    IfxEvadc_ChannelId_2    = 2,   /**< \brief Channel 2 */
    IfxEvadc_ChannelId_3    = 3,   /**< \brief Channel 3 */
    IfxEvadc_ChannelId_4    = 4,   /**< \brief Channel 4 */
    IfxEvadc_ChannelId_5    = 5,   /**< \brief Channel 5 */
    IfxEvadc_ChannelId_6    = 6,   /**< \brief Channel 6 */
    IfxEvadc_ChannelId_7    = 7,   /**< \brief Channel 7 */
    IfxEvadc_ChannelId_8    = 8,   /**< \brief Channel 8 */
    IfxEvadc_ChannelId_9    = 9,   /**< \brief Channel 9 */
    IfxEvadc_ChannelId_10   = 10,  /**< \brief Channel 10 */
    IfxEvadc_ChannelId_11   = 11,  /**< \brief Channel 11 */
    IfxEvadc_ChannelId_12   = 12,  /**< \brief Channel 12 */
    IfxEvadc_ChannelId_13   = 13,  /**< \brief Channel 13 */
    IfxEvadc_ChannelId_14   = 14,  /**< \brief Channel 14 */
    IfxEvadc_ChannelId_15   = 15   /**< \brief Channel 15 */
} IfxEvadc_ChannelId;

/** \brief ADC channel reference defined in MODULE_EVADC.G[x].CHCTR[y].B.REFSEL(x=0,1,...,11;y=0,1....,16)
 */
typedef enum
{
    IfxEvadc_ChannelReference_standard = 0,  /**< \brief use Varef as reference */
    IfxEvadc_ChannelReference_channel0 = 1   /**< \brief use CH0 as reference */
} IfxEvadc_ChannelReference;

/** \brief Channel Result defined in MODULE_EVADC.G[x].CHCTR[y].B.RESREG(x=0,1,...,11;y=0,1....,16)
 */
typedef enum
{
    IfxEvadc_ChannelResult_0 = 0,      /**< \brief Use Channel result 0  */
    IfxEvadc_ChannelResult_1,          /**< \brief Use Channel result 1  */
    IfxEvadc_ChannelResult_2,          /**< \brief Use Channel result 2  */
    IfxEvadc_ChannelResult_3,          /**< \brief Use Channel result 3  */
    IfxEvadc_ChannelResult_4,          /**< \brief Use Channel result 4  */
    IfxEvadc_ChannelResult_5,          /**< \brief Use Channel result 5  */
    IfxEvadc_ChannelResult_6,          /**< \brief Use Channel result 6  */
    IfxEvadc_ChannelResult_7,          /**< \brief Use Channel result 7  */
    IfxEvadc_ChannelResult_8,          /**< \brief Use Channel result 8  */
    IfxEvadc_ChannelResult_9,          /**< \brief Use Channel result 9  */
    IfxEvadc_ChannelResult_10,         /**< \brief Use Channel result 10  */
    IfxEvadc_ChannelResult_11,         /**< \brief Use Channel result 11  */
    IfxEvadc_ChannelResult_12,         /**< \brief Use Channel result 12  */
    IfxEvadc_ChannelResult_13,         /**< \brief Use Channel result 13  */
    IfxEvadc_ChannelResult_14,         /**< \brief Use Channel result 14  */
    IfxEvadc_ChannelResult_15          /**< \brief Use Channel result 15  */
} IfxEvadc_ChannelResult;

/** \brief External Multiplexer Channel Selection Style as defined in
 * Ifx_EVADC.G[x].EMUXCTR.B.EMXCSS
 */
typedef enum
{
    IfxEvadc_ChannelSelectionStyle_channelNumber = 0,  /**< \brief selects an arbitrary channel */
    IfxEvadc_ChannelSelectionStyle_binary        = 1   /**< \brief Each bit of bitfield EMUXCH selects the
                                                        * associated channel for EMUX control */
} IfxEvadc_ChannelSelectionStyle;

/** \brief Specifies the External Coding scheme(binary/gray)
 * defined in Ifx_EVADC.G[x].EMUXCTR.B.EMXCOD
 */
typedef enum
{
    IfxEvadc_EmuxCodingScheme_binary = 0,  /**< \brief Output the Channel Number in Binary code */
    IfxEvadc_EmuxCodingScheme_gray   = 1   /**< \brief Output the channel number in gray code */
} IfxEvadc_EmuxCodingScheme;

/** \brief Specifies the Emux interface
 */
typedef enum
{
    IfxEvadc_EmuxInterface_0 = 0,  /**< \brief Emux Interface 0 */
    IfxEvadc_EmuxInterface_1 = 1   /**< \brief Emux Interface 1 */
} IfxEvadc_EmuxInterface;

/** \brief External Multiplexer sample time control
 * defined in Ifx_EVADC.G[x].EMUXCTR.B.EMXST
 */
typedef enum
{
    IfxEvadc_EmuxSampleTimeControl_settingChanges = 0,  /**< \brief Use STCE Whenever Setting Changes */
    IfxEvadc_EmuxSampleTimeControl_always         = 1   /**< \brief Use STCE for each conversion of an external channel */
} IfxEvadc_EmuxSampleTimeControl;

/** \brief specifies the External Channel Start select value
 * defined in Ifx_EVADC.G[x].EMUXCTR.B.EMUXSET
 */
typedef enum
{
    IfxEvadc_EmuxSelectValue_0 = 0,      /**< \brief Start Selection Value 0  */
    IfxEvadc_EmuxSelectValue_1,          /**< \brief Start Selection Value 1  */
    IfxEvadc_EmuxSelectValue_2,          /**< \brief Start Selection Value 2  */
    IfxEvadc_EmuxSelectValue_3,          /**< \brief Start Selection Value 3  */
    IfxEvadc_EmuxSelectValue_4,          /**< \brief Start Selection Value 4  */
    IfxEvadc_EmuxSelectValue_5,          /**< \brief Start Selection Value 5  */
    IfxEvadc_EmuxSelectValue_6,          /**< \brief Start Selection Value 6  */
    IfxEvadc_EmuxSelectValue_7           /**< \brief Start Selection Value 7  */
} IfxEvadc_EmuxSelectValue;

/** \brief Specifies External Multiplexer Mode
 * define in Ifx_EVADC.G[x].EMUXCTR.B.EMUXMODE
 */
typedef enum
{
    IfxEvadc_ExternalMultiplexerMode_softwareControl = 0,  /**< \brief Disable The Emux Control */
    IfxEvadc_ExternalMultiplexerMode_steady          = 1,  /**< \brief select steady mode */
    IfxEvadc_ExternalMultiplexerMode_singleStep      = 2,  /**< \brief Select single step mode */
    IfxEvadc_ExternalMultiplexerMode_sequence        = 3   /**< \brief Select Sequence Mode */
} IfxEvadc_ExternalMultiplexerMode;

/** \brief FIFO mode enable
 */
typedef enum
{
    IfxEvadc_FifoMode_seperateResultRegister = 0,  /**< \brief seperate Result Register */
    IfxEvadc_FifoMode_fifoStructure          = 1,  /**< \brief Part of FIFO structure: copy each new valid result */
    IfxEvadc_FifoMode_maximumMode            = 2,  /**< \brief copy new result if bigger */
    IfxEvadc_FifoMode_minimumMode            = 3   /**< \brief copy new result if it is smaller */
} IfxEvadc_FifoMode;

/** \brief gating mode defined in MODULE_EVADC.QMR0.ENGT
 */
typedef enum
{
    IfxEvadc_GatingMode_disabled   = 0,  /**< \brief Gating is disabled, no conversion request are issued */
    IfxEvadc_GatingMode_always     = 1,  /**< \brief Conversion request is issued if at least 1 conversion pending bit is set */
    IfxEvadc_GatingMode_gatingHigh = 2,  /**< \brief Conversion request is issued if at least 1 conversion pending bit is set and the gating signal is high */
    IfxEvadc_GatingMode_gatingLow  = 3   /**< \brief Conversion request is issued if at least 1 conversion pending bit is set and the gating signal is low */
} IfxEvadc_GatingMode;

/** \brief External trigger gating defined in MODULE_EVADC.G[x].QCTRLy.GTSEL(x=0,1,..,11;y=0,1,..,7)
 */
typedef enum
{
    IfxEvadc_GatingSource_0 = 0,      /**< \brief Input signal REQGTx_0  */
    IfxEvadc_GatingSource_1,          /**< \brief Input signal REQGTx_1  */
    IfxEvadc_GatingSource_2,          /**< \brief Input signal REQGTx_2  */
    IfxEvadc_GatingSource_3,          /**< \brief Input signal REQGTx_3  */
    IfxEvadc_GatingSource_4,          /**< \brief Input signal REQGTx_4  */
    IfxEvadc_GatingSource_5,          /**< \brief Input signal REQGTx_5  */
    IfxEvadc_GatingSource_6,          /**< \brief Input signal REQGTx_6  */
    IfxEvadc_GatingSource_7,          /**< \brief Input signal REQGTx_7  */
    IfxEvadc_GatingSource_8,          /**< \brief Input signal REQGTx_8  */
    IfxEvadc_GatingSource_9,          /**< \brief Input signal REQGTx_9  */
    IfxEvadc_GatingSource_10,         /**< \brief Input signal REQGTx_10  */
    IfxEvadc_GatingSource_11,         /**< \brief Input signal REQGTx_11  */
    IfxEvadc_GatingSource_12,         /**< \brief Input signal REQGTx_12  */
    IfxEvadc_GatingSource_13,         /**< \brief Input signal REQGTx_13  */
    IfxEvadc_GatingSource_14,         /**< \brief Input signal REQGTx_14  */
    IfxEvadc_GatingSource_15          /**< \brief Input signal REQGTx_15  */
} IfxEvadc_GatingSource;

/** \brief inputClass defined in MODULE_EVADC.G[x].CHCTR[y].B.ICLSEL(x=0,1,...,11;y=0,1....,16)
 */
typedef enum
{
    IfxEvadc_InputClasses_group0  = 0,  /**< \brief Use group class 0 */
    IfxEvadc_InputClasses_group1  = 1,  /**< \brief Use group class 1 */
    IfxEvadc_InputClasses_global0 = 2,  /**< \brief Use global class 0 */
    IfxEvadc_InputClasses_global1 = 3   /**< \brief Use global class 1 */
} IfxEvadc_InputClasses;

/** \brief ADC channel limit check defined in MODULE_EVADC.G[x].CHCTR[y].B.CHEVMODE(x=0,1,...,11;y=0,1....,16)
 */
typedef enum
{
    IfxEvadc_LimitCheck_noCheck            = 0,  /**< \brief Normal compare mode Event Never Fast Compare mode Event Never */
    IfxEvadc_LimitCheck_eventIfInArea      = 1,  /**< \brief Normal compare mode Event If result is inside the boundary band    Fast Compare mode Event If result switches to high (above comp. value) */
    IfxEvadc_LimitCheck_eventIfOutsideArea = 2,  /**< \brief Normal compare mode Event If result is outside the boundary band   Fast Compare mode Event If result switches to low (below comp. value) */
    IfxEvadc_LimitCheck_always             = 3   /**< \brief Normal compare mode Event Always   Fast Compare mode Event Always */
} IfxEvadc_LimitCheck;

/** \brief Access protection for Group registers defined in MODULE_EVADC.ACCPROT0.U
 */
typedef enum
{
    IfxEvadc_Protection_channelControl0         = 0,   /**< \brief Access control for GxCHCTR0 */
    IfxEvadc_Protection_channelControl1         = 1,   /**< \brief Access control for GxCHCTR1 */
    IfxEvadc_Protection_channelControl2         = 2,   /**< \brief Access control for GxCHCTR2 */
    IfxEvadc_Protection_channelControl3         = 3,   /**< \brief Access control for GxCHCTR3 */
    IfxEvadc_Protection_channelControl4         = 4,   /**< \brief Access control for GxCHCTR4 */
    IfxEvadc_Protection_channelControl5         = 5,   /**< \brief Access control for GxCHCTR5 */
    IfxEvadc_Protection_channelControl6         = 6,   /**< \brief Access control for GxCHCTR6 */
    IfxEvadc_Protection_channelControl7         = 7,   /**< \brief Access control for GxCHCTR7 */
    IfxEvadc_Protection_channelControl8         = 8,   /**< \brief Access control for GxCHCTR8 */
    IfxEvadc_Protection_channelControl9         = 9,   /**< \brief Access control for GxCHCTR9 */
    IfxEvadc_Protection_channelControl10        = 10,  /**< \brief Access control for GxCHCTR10 */
    IfxEvadc_Protection_channelControl11        = 11,  /**< \brief Access control for GxCHCTR11 */
    IfxEvadc_Protection_channelControl12        = 12,  /**< \brief Access control for GxCHCTR12 */
    IfxEvadc_Protection_channelControl13        = 13,  /**< \brief Access control for GxCHCTR13 */
    IfxEvadc_Protection_channelControl14        = 14,  /**< \brief Access control for GxCHCTR14 */
    IfxEvadc_Protection_initGroup0              = 16,  /**< \brief Access control for GxARBCFG, GxARBPR, GxCHASS, GxRRASS, GxICLASS0/1, GxSYNCTR */
    IfxEvadc_Protection_initGroup1              = 17,  /**< \brief Access control for GxARBCFG, GxARBPR, GxCHASS, GxRRASS, GxICLASS0/1, GxSYNCTR */
    IfxEvadc_Protection_initGroup2              = 18,  /**< \brief Access control for GxARBCFG, GxARBPR, GxCHASS, GxRRASS, GxICLASS0/1, GxSYNCTR */
    IfxEvadc_Protection_initGroup3              = 19,  /**< \brief Access control for GxARBCFG, GxARBPR, GxCHASS, GxRRASS, GxICLASS0/1, GxSYNCTR */
    IfxEvadc_Protection_initGroup4              = 20,  /**< \brief Access control for GxARBCFG, GxARBPR, GxCHASS, GxRRASS, GxICLASS0/1, GxSYNCTR */
    IfxEvadc_Protection_initGroup5              = 21,  /**< \brief Access control for GxARBCFG, GxARBPR, GxCHASS, GxRRASS, GxICLASS0/1, GxSYNCTR */
    IfxEvadc_Protection_initGroup6              = 22,  /**< \brief Access control for GxARBCFG, GxARBPR, GxCHASS, GxRRASS, GxICLASS0/1, GxSYNCTR */
    IfxEvadc_Protection_initGroup7              = 23,  /**< \brief Access control for GxARBCFG, GxARBPR, GxCHASS, GxRRASS, GxICLASS0/1, GxSYNCTR */
    IfxEvadc_Protection_initGroup8              = 24,  /**< \brief Access control for GxARBCFG, GxARBPR, GxCHASS, GxRRASS, GxICLASS0/1, GxSYNCTR */
    IfxEvadc_Protection_initGroup9              = 25,  /**< \brief Access control for GxARBCFG, GxARBPR, GxCHASS, GxRRASS, GxICLASS0/1, GxSYNCTR */
    IfxEvadc_Protection_initGroup10             = 26,  /**< \brief Access control for GxARBCFG, GxARBPR, GxCHASS, GxRRASS, GxICLASS0/1, GxSYNCTR */
    IfxEvadc_Protection_initGroup11             = 27,  /**< \brief Access control for GxARBCFG, GxARBPR, GxCHASS, GxRRASS, GxICLASS0/1, GxSYNCTR */
    IfxEvadc_Protection_initGroup12             = 28,  /**< \brief Access control for GxARBCFG, GxARBPR, GxCHASS, GxRRASS, GxICLASS0/1, GxSYNCTR */
    IfxEvadc_Protection_initGroup13             = 29,  /**< \brief Access control for GxARBCFG, GxARBPR, GxCHASS, GxRRASS, GxICLASS0/1, GxSYNCTR */
    IfxEvadc_Protection_initGroup14             = 30,  /**< \brief Access control for GxARBCFG, GxARBPR, GxCHASS, GxRRASS, GxICLASS0/1, GxSYNCTR */
    IfxEvadc_Protection_serviceGroup0           = 32,  /**< \brief Access control for GxSEFLAG, GxSEVNP, GxCEFLAG, GxCEVNP0/1/2, GxREFLAG, GxREVNP0/1, GxSRACT */
    IfxEvadc_Protection_serviceGroup1           = 33,  /**< \brief Access control for GxSEFLAG, GxSEVNP, GxCEFLAG, GxCEVNP0/1/2, GxREFLAG, GxREVNP0/1, GxSRACT */
    IfxEvadc_Protection_serviceGroup2           = 34,  /**< \brief Access control for GxSEFLAG, GxSEVNP, GxCEFLAG, GxCEVNP0/1/2, GxREFLAG, GxREVNP0/1, GxSRACT */
    IfxEvadc_Protection_serviceGroup3           = 35,  /**< \brief Access control for GxSEFLAG, GxSEVNP, GxCEFLAG, GxCEVNP0/1/2, GxREFLAG, GxREVNP0/1, GxSRACT */
    IfxEvadc_Protection_serviceGroup4           = 36,  /**< \brief Access control for GxSEFLAG, GxSEVNP, GxCEFLAG, GxCEVNP0/1/2, GxREFLAG, GxREVNP0/1, GxSRACT */
    IfxEvadc_Protection_serviceGroup5           = 37,  /**< \brief Access control for GxSEFLAG, GxSEVNP, GxCEFLAG, GxCEVNP0/1/2, GxREFLAG, GxREVNP0/1, GxSRACT */
    IfxEvadc_Protection_serviceGroup6           = 38,  /**< \brief Access control for GxSEFLAG, GxSEVNP, GxCEFLAG, GxCEVNP0/1/2, GxREFLAG, GxREVNP0/1, GxSRACT */
    IfxEvadc_Protection_serviceGroup7           = 39,  /**< \brief Access control for GxSEFLAG, GxSEVNP, GxCEFLAG, GxCEVNP0/1/2, GxREFLAG, GxREVNP0/1, GxSRACT */
    IfxEvadc_Protection_serviceGroup8           = 40,  /**< \brief Access control for GxSEFLAG, GxSEVNP, GxCEFLAG, GxCEVNP0/1/2, GxREFLAG, GxREVNP0/1, GxSRACT */
    IfxEvadc_Protection_serviceGroup9           = 41,  /**< \brief Access control for GxSEFLAG, GxSEVNP, GxCEFLAG, GxCEVNP0/1/2, GxREFLAG, GxREVNP0/1, GxSRACT */
    IfxEvadc_Protection_serviceGroup10          = 42,  /**< \brief Access control for GxSEFLAG, GxSEVNP, GxCEFLAG, GxCEVNP0/1/2, GxREFLAG, GxREVNP0/1, GxSRACT */
    IfxEvadc_Protection_serviceGroup11          = 43,  /**< \brief Access control for GxSEFLAG, GxSEVNP, GxCEFLAG, GxCEVNP0/1/2, GxREFLAG, GxREVNP0/1, GxSRACT */
    IfxEvadc_Protection_serviceGroup12          = 44,  /**< \brief Access control for GxSEFLAG, GxSEVNP, GxCEFLAG, GxCEVNP0/1/2, GxREFLAG, GxREVNP0/1, GxSRACT */
    IfxEvadc_Protection_serviceGroup13          = 45,  /**< \brief Access control for GxSEFLAG, GxSEVNP, GxCEFLAG, GxCEVNP0/1/2, GxREFLAG, GxREVNP0/1, GxSRACT */
    IfxEvadc_Protection_serviceGroup14          = 46,  /**< \brief Access control for GxSEFLAG, GxSEVNP, GxCEFLAG, GxCEVNP0/1/2, GxREFLAG, GxREVNP0/1, GxSRACT */
    IfxEvadc_Protection_resultRegisterGroup0    = 48,  /**< \brief Access control for GxRCRx(x=0,1,..,15), GxBOUND, GxRESx(x=0 .. 15) */
    IfxEvadc_Protection_resultRegisterGroup1    = 49,  /**< \brief Access control for GxRCRx(x=0,1,..,15), GxBOUND, GxRESx(x=0 .. 15) */
    IfxEvadc_Protection_resultRegisterGroup2    = 50,  /**< \brief Access control for GxRCRx(x=0,1,..,15), GxBOUND, GxRESx(x=0 .. 15) */
    IfxEvadc_Protection_resultRegisterGroup3    = 51,  /**< \brief Access control for GxRCRx(x=0,1,..,15), GxBOUND, GxRESx(x=0 .. 15) */
    IfxEvadc_Protection_resultRegisterGroup4    = 52,  /**< \brief Access control for GxRCRx(x=0,1,..,15), GxBOUND, GxRESx(x=0 .. 15) */
    IfxEvadc_Protection_resultRegisterGroup5    = 53,  /**< \brief Access control for GxRCRx(x=0,1,..,15), GxBOUND, GxRESx(x=0 .. 15) */
    IfxEvadc_Protection_resultRegisterGroup6    = 54,  /**< \brief Access control for GxRCRx(x=0,1,..,15), GxBOUND, GxRESx(x=0 .. 15) */
    IfxEvadc_Protection_resultRegisterGroup7    = 55,  /**< \brief Access control for GxRCRx(x=0,1,..,15), GxBOUND, GxRESx(x=0 .. 15) */
    IfxEvadc_Protection_resultRegisterGroup8    = 56,  /**< \brief Access control for GxRCRx(x=0,1,..,15), GxBOUND, GxRESx(x=0 .. 15) */
    IfxEvadc_Protection_resultRegisterGroup9    = 57,  /**< \brief Access control for GxRCRx(x=0,1,..,15), GxBOUND, GxRESx(x=0 .. 15) */
    IfxEvadc_Protection_resultRegisterGroup10   = 58,  /**< \brief Access control for GxRCRx(x=0,1,..,15), GxBOUND, GxRESx(x=0 .. 15) */
    IfxEvadc_Protection_resultRegisterGroup11   = 59,  /**< \brief Access control for GxRCRx(x=0,1,..,15), GxBOUND, GxRESx(x=0 .. 15) */
    IfxEvadc_Protection_resultRegisterGroup12   = 60,  /**< \brief Access control for GxRCRx(x=0,1,..,15), GxBOUND, GxRESx(x=0 .. 15) */
    IfxEvadc_Protection_resultRegisterGroup13   = 61,  /**< \brief Access control for GxRCRx(x=0,1,..,15), GxBOUND, GxRESx(x=0 .. 15) */
    IfxEvadc_Protection_resultRegisterGroup14   = 62,  /**< \brief Access control for GxRCRx(x=0,1,..,15), GxBOUND, GxRESx(x=0 .. 15) */
    IfxEvadc_Protection_initFastCompareChannel0 = 64,  /**< \brief Access control for GyFCBFL, GyFCHYST, GyFCCTR, GyFCM,
                                                        * GyFCRAMP */
    IfxEvadc_Protection_initFastCompareChannel1 = 65,  /**< \brief Access control for GyFCBFL, GyFCHYST, GyFCCTR, GyFCM,
                                                        * GyFCRAMP */
    IfxEvadc_Protection_initFastCompareChannel2 = 66,  /**< \brief Access control for GyFCBFL, GyFCHYST, GyFCCTR, GyFCM,
                                                        * GyFCRAMP */
    IfxEvadc_Protection_initFastCompareChannel3 = 67,  /**< \brief Access control for GyFCBFL, GyFCHYST, GyFCCTR, GyFCM,
                                                        * GyFCRAMP */
    IfxEvadc_Protection_initFastCompareChannel4 = 68,  /**< \brief Access control for GyFCBFL, GyFCHYST, GyFCCTR, GyFCM,
                                                        * GyFCRAMP */
    IfxEvadc_Protection_initFastCompareChannel5 = 69,  /**< \brief Access control for GyFCBFL, GyFCHYST, GyFCCTR, GyFCM,
                                                        * GyFCRAMP */
    IfxEvadc_Protection_initFastCompareChannel6 = 70,  /**< \brief Access control for GyFCBFL, GyFCHYST, GyFCCTR, GyFCM,
                                                        * GyFCRAMP */
    IfxEvadc_Protection_initFastCompareChannel7 = 71,  /**< \brief Access control for GyFCBFL, GyFCHYST, GyFCCTR, GyFCM,
                                                        * GyFCRAMP */
    IfxEvadc_Protection_globalConfig            = 80,  /**< \brief Access control for GLOBCFG */
    IfxEvadc_Protection_externalMultiplexer     = 81,  /**< \brief Access control for EMUXSEL, GxEMUXCTR */
    IfxEvadc_Protection_testFunction            = 82   /**< \brief Access control for GLOBTF */
} IfxEvadc_Protection;

/** \brief Arbitration priority, Group x,defined in MODULE_EVADC.G[x].ARBPR.PRIOy(x=0,1,...,11;y=0,1,2)
 */
typedef enum
{
    IfxEvadc_RequestSlotPriority_lowest  = 0, /**< \brief Lowest priority */
    IfxEvadc_RequestSlotPriority_low     = 1, /**< \brief Lowpriority */
    IfxEvadc_RequestSlotPriority_high    = 2, /**< \brief High priority */
    IfxEvadc_RequestSlotPriority_highest = 3  /**< \brief Highest priority */
} IfxEvadc_RequestSlotPriority;

/** \brief Request source start mode defined in MODULE_EVADC.G[x].ARBPR.CSMy(x=0,1,...,11;y=0,1,2)
 */
typedef enum
{
    IfxEvadc_RequestSlotStartMode_waitForStart       = 0, /**< \brief Wait for start */
    IfxEvadc_RequestSlotStartMode_cancelInjectRepeat = 1  /**< \brief Cancel-Inject-Repeat */
} IfxEvadc_RequestSlotStartMode;

/** \brief Request sources
 */
typedef enum
{
    IfxEvadc_RequestSource_queue0 = 0,  /**< \brief 8 stage Queue0 request */
    IfxEvadc_RequestSource_queue1 = 1,  /**< \brief 8 stage Queue1 request */
    IfxEvadc_RequestSource_queue2 = 2   /**< \brief 8 stage Queue2 request */
} IfxEvadc_RequestSource;

/** \brief Enable/disable the sensitivity of the module to sleep signal\n
 * Definition in Ifx_EVADC.CLC.B.EDIS
 */
typedef enum
{
    IfxEvadc_SleepMode_enable  = 0, /**< \brief enables sleep mode */
    IfxEvadc_SleepMode_disable = 1  /**< \brief disables sleep mode */
} IfxEvadc_SleepMode;

/** \brief Service Node defined in MODULE_EVADC.G[x].SRACT.U(x= 0,1,..,11)
 */
typedef enum
{
    IfxEvadc_SrcNr_group0  = 0,  /**< \brief service request line 0 of group */
    IfxEvadc_SrcNr_group1  = 1,  /**< \brief service request line 1 of group */
    IfxEvadc_SrcNr_group2  = 2,  /**< \brief service request line 2 of group */
    IfxEvadc_SrcNr_group3  = 3,  /**< \brief service request line 3 of group */
    IfxEvadc_SrcNr_shared0 = 4,  /**< \brief Select shared service request line 0 */
    IfxEvadc_SrcNr_shared1 = 5,  /**< \brief Select shared service request line 1 */
    IfxEvadc_SrcNr_shared2 = 6,  /**< \brief Select shared service request line 2 */
    IfxEvadc_SrcNr_shared3 = 7   /**< \brief Select shared service request line 3 */
} IfxEvadc_SrcNr;

/** \brief API return values defined in
 * MODULE_EVADC.G[x].QSR0.U,MODULE_EVADC.G[x].QSR1.U,MODULE_EVADC.G[x].QSR2.U(x=0,1,...,11)
 */
typedef enum
{
    IfxEvadc_Status_noError              = 0, /**< \brief No error during api execution */
    IfxEvadc_Status_notInitialised       = 1, /**< \brief Appropriate initialisation not done */
    IfxEvadc_Status_invalidGroup         = 2, /**< \brief Invalid group number */
    IfxEvadc_Status_invalidChannel       = 3, /**< \brief Invalid channel number */
    IfxEvadc_Status_queueFull            = 4, /**< \brief Queue is full */
    IfxEvadc_Status_noAccess             = 5, /**< \brief Access to the group/channel is disabled */
    IfxEvadc_Status_channelsStillPending = 6  /**< \brief Conversion for some of the channels are still pending */
} IfxEvadc_Status;

/** \brief OCDS Suspend Control (OCDS.SUS)
 */
typedef enum
{
    IfxEvadc_SuspendMode_none = 0,  /**< \brief No suspend */
    IfxEvadc_SuspendMode_hard = 1,  /**< \brief Hard Suspend */
    IfxEvadc_SuspendMode_soft = 2   /**< \brief Soft Suspend */
} IfxEvadc_SuspendMode;

/** \brief trigger definition defined in MODULE_EVADC.G[x].QCTRL0.XTMODE(x=0,1,..,11)
 */
typedef enum
{
    IfxEvadc_TriggerMode_noExternalTrigger = 0,  /**< \brief No external trigger */
    IfxEvadc_TriggerMode_uponFallingEdge   = 1,  /**< \brief Trigger event upon a falling edge */
    IfxEvadc_TriggerMode_uponRisingEdge    = 2,  /**< \brief Trigger event upon a rising edge */
    IfxEvadc_TriggerMode_uponAnyEdge       = 3   /**< \brief Trigger event upon any edge */
} IfxEvadc_TriggerMode;

/** \brief Trigger request source defined in MODULE_EVADC.G[x].QCTRLy.XTSEL(x=0,1,..,11;y=0,1,..,7)
 */
typedef enum
{
    IfxEvadc_TriggerSource_0 = 0,      /**< \brief Input signal REQTRx_0  */
    IfxEvadc_TriggerSource_1,          /**< \brief Input signal REQTRx_1  */
    IfxEvadc_TriggerSource_2,          /**< \brief Input signal REQTRx_2  */
    IfxEvadc_TriggerSource_3,          /**< \brief Input signal REQTRx_3  */
    IfxEvadc_TriggerSource_4,          /**< \brief Input signal REQTRx_4  */
    IfxEvadc_TriggerSource_5,          /**< \brief Input signal REQTRx_5  */
    IfxEvadc_TriggerSource_6,          /**< \brief Input signal REQTRx_6  */
    IfxEvadc_TriggerSource_7,          /**< \brief Input signal REQTRx_7  */
    IfxEvadc_TriggerSource_8,          /**< \brief Input signal REQTRx_8  */
    IfxEvadc_TriggerSource_9,          /**< \brief Input signal REQTRx_9  */
    IfxEvadc_TriggerSource_10,         /**< \brief Input signal REQTRx_10  */
    IfxEvadc_TriggerSource_11,         /**< \brief Input signal REQTRx_11  */
    IfxEvadc_TriggerSource_12,         /**< \brief Input signal REQTRx_12  */
    IfxEvadc_TriggerSource_13,         /**< \brief Input signal REQTRx_13  */
    IfxEvadc_TriggerSource_14,         /**< \brief Input signal REQTRx_14  */
    IfxEvadc_TriggerSource_15          /**< \brief Input signal REQTRx_15  */
} IfxEvadc_TriggerSource;

/** \} */

/** \addtogroup IfxLld_Evadc_Std_FastCompare
 * \{ */
/** \brief Selects the behaviour of BoundaryFlag in FCBFL.B.BFA
 */
typedef enum
{
    IfxEvadc_BoundaryFlagActivationMode_0 = 0,  /**< \brief Set boundary flag BFL if result FCR = 1 (input above the defined
                                                 * band or compare value), clear if FCR = 0 (input below) */
    IfxEvadc_BoundaryFlagActivationMode_1 = 1   /**< \brief Set boundary flag BFL if result FCR = 0 (input below the defined
                                                 * band or compare value), clear if FCR = 1 (input above) */
} IfxEvadc_BoundaryFlagActivationMode;

/** \brief Controls in in FCBFL.B.BFI options for whether to use Boundary Flag(FCM.B.BFL) directly or in a inverted format.
 */
typedef enum
{
    IfxEvadc_BoundaryFlagInversionControl_direct = 0,  /**< \brief Use BFL directly */
    IfxEvadc_BoundaryFlagInversionControl_invert = 1   /**< \brief Use inverted BFL */
} IfxEvadc_BoundaryFlagInversionControl;

/** \brief Options for FCBFL.B.BFLNP to select Boundary Flag Node Pointer
 */
typedef enum
{
    IfxEvadc_BoundaryFlagNodePointer_commonBoundaryFlagOutput0 = 0,  /**< \brief Select common boundary flag output 0 */
    IfxEvadc_BoundaryFlagNodePointer_commonBoundaryFlagOutput1 = 1,  /**< \brief Select common boundary flag output 1 */
    IfxEvadc_BoundaryFlagNodePointer_commonBoundaryFlagOutput2 = 2,  /**< \brief Select common boundary flag output 2 */
    IfxEvadc_BoundaryFlagNodePointer_commonBoundaryFlagOutput3 = 3,  /**< \brief Select common boundary flag output 3 */
    IfxEvadc_BoundaryFlagNodePointer_sharedServiceRequestLine0 = 4,  /**< \brief Select shared service request line 0 */
    IfxEvadc_BoundaryFlagNodePointer_sharedServiceRequestLine1 = 5,  /**< \brief Select shared service request line 1 */
    IfxEvadc_BoundaryFlagNodePointer_sharedServiceRequestLine2 = 6,  /**< \brief Select shared service request line 2 */
    IfxEvadc_BoundaryFlagNodePointer_sharedServiceRequestLine3 = 7,  /**< \brief Select shared service request line 3 */
    IfxEvadc_BoundaryFlagNodePointer_disabled                  = 16  /**< \brief Disabled, no common output signal */
} IfxEvadc_BoundaryFlagNodePointer;

/** \brief Options for FCBFL.B.BFS to modify the Boundary Flag(FCM.B.BFL)
 */
typedef enum
{
    IfxEvadc_BoundaryFlagSwControl_noAction = 0,  /**< \brief No Action */
    IfxEvadc_BoundaryFlagSwControl_clear    = 1,  /**< \brief Clear BFL */
    IfxEvadc_BoundaryFlagSwControl_set      = 2,  /**< \brief Set BFL */
    IfxEvadc_BoundaryFlagSwControl_toggle   = 3   /**< \brief Toggle BFL */
} IfxEvadc_BoundaryFlagSwControl;

/** \brief Channel Event Mode for configuring generation of events for Fast Compare channels in FCCTRL.B.CHEVMODE
 */
typedef enum
{
    IfxEvadc_ChannelEventMode_never                    = 0, /**< \brief Never generate a service request */
    IfxEvadc_ChannelEventMode_aboveCompareValue        = 1, /**< \brief Generate a service request in case the result is above compare value */
    IfxEvadc_ChannelEventMode_belowCompareValue        = 2, /**< \brief Generate a service request in case the result is below compare value */
    IfxEvadc_ChannelEventMode_aboveOrBelowCompareValue = 3  /**< \brief Generate a service request in case the result switches to either level(above or below the compare value) */
} IfxEvadc_ChannelEventMode;

/** \brief Defines the frequency of the analog converter clock f_ADCI (base clock for
 * conversion steps), derived from the peripheral clock: f_ADCI = fADC / CP.
 * To be filled in FCCTRL.B.DIVA for Fast Compare channels & ANCFG.B.DIVA for other groups
 */
typedef enum
{
    IfxEvadc_ClockDividerFactor_maxFrequency = 0,  /**< \brief Max. frequency */
    IfxEvadc_ClockDividerFactor_2            = 1,  /**< \brief Frequency Divided by 2  */
    IfxEvadc_ClockDividerFactor_3,                 /**< \brief Frequency Divided by 3  */
    IfxEvadc_ClockDividerFactor_4,                 /**< \brief Frequency Divided by 4  */
    IfxEvadc_ClockDividerFactor_5,                 /**< \brief Frequency Divided by 5  */
    IfxEvadc_ClockDividerFactor_6,                 /**< \brief Frequency Divided by 6  */
    IfxEvadc_ClockDividerFactor_7,                 /**< \brief Frequency Divided by 7  */
    IfxEvadc_ClockDividerFactor_8,                 /**< \brief Frequency Divided by 8  */
    IfxEvadc_ClockDividerFactor_9,                 /**< \brief Frequency Divided by 9  */
    IfxEvadc_ClockDividerFactor_10,                /**< \brief Frequency Divided by 10  */
    IfxEvadc_ClockDividerFactor_11,                /**< \brief Frequency Divided by 11  */
    IfxEvadc_ClockDividerFactor_12,                /**< \brief Frequency Divided by 12  */
    IfxEvadc_ClockDividerFactor_13,                /**< \brief Frequency Divided by 13  */
    IfxEvadc_ClockDividerFactor_14,                /**< \brief Frequency Divided by 14  */
    IfxEvadc_ClockDividerFactor_15,                /**< \brief Frequency Divided by 15  */
    IfxEvadc_ClockDividerFactor_16,                /**< \brief Frequency Divided by 16  */
    IfxEvadc_ClockDividerFactor_17,                /**< \brief Frequency Divided by 17  */
    IfxEvadc_ClockDividerFactor_18,                /**< \brief Frequency Divided by 18  */
    IfxEvadc_ClockDividerFactor_19,                /**< \brief Frequency Divided by 19  */
    IfxEvadc_ClockDividerFactor_20,                /**< \brief Frequency Divided by 20  */
    IfxEvadc_ClockDividerFactor_21,                /**< \brief Frequency Divided by 21  */
    IfxEvadc_ClockDividerFactor_22,                /**< \brief Frequency Divided by 22  */
    IfxEvadc_ClockDividerFactor_23,                /**< \brief Frequency Divided by 23  */
    IfxEvadc_ClockDividerFactor_24,                /**< \brief Frequency Divided by 24  */
    IfxEvadc_ClockDividerFactor_25,                /**< \brief Frequency Divided by 25  */
    IfxEvadc_ClockDividerFactor_26,                /**< \brief Frequency Divided by 26  */
    IfxEvadc_ClockDividerFactor_27,                /**< \brief Frequency Divided by 27  */
    IfxEvadc_ClockDividerFactor_28,                /**< \brief Frequency Divided by 28  */
    IfxEvadc_ClockDividerFactor_29,                /**< \brief Frequency Divided by 29  */
    IfxEvadc_ClockDividerFactor_30,                /**< \brief Frequency Divided by 30  */
    IfxEvadc_ClockDividerFactor_31,                /**< \brief Frequency Divided by 31  */
    IfxEvadc_ClockDividerFactor_32                 /**< \brief Frequency Divided by 32  */
} IfxEvadc_ClockDividerFactor;

/** \brief External Trigger Polarity to be set in FCCTRL.B.XTPOL
 */
typedef enum
{
    IfxEvadc_ExternalTriggerPolarity_direct   = 0, /**< \brief Use selected input signal directly */
    IfxEvadc_ExternalTriggerPolarity_inverted = 1  /**< \brief Invert selected input signal */
} IfxEvadc_ExternalTriggerPolarity;

typedef enum
{
    IfxEvadc_FastCompareAnalogClockSynchronizationDelay_0 = 0,  /**< \brief No delay */
    IfxEvadc_FastCompareAnalogClockSynchronizationDelay_1 = 1,  /**< \brief 1 clock cycle delay */
    IfxEvadc_FastCompareAnalogClockSynchronizationDelay_2 = 2,  /**< \brief 2 clock cycles delay */
    IfxEvadc_FastCompareAnalogClockSynchronizationDelay_3 = 3   /**< \brief 3 clock cycles delay */
} IfxEvadc_FastCompareAnalogClockSynchronizationDelay;

/** \brief Settings for Analog Coverter Controller to be done in FCM.B.ANON
 */
typedef enum
{
    IfxEvadc_FastCompareAnalogConverterControl_off    = 0, /**< \brief Analog converter off */
    IfxEvadc_FastCompareAnalogConverterControl_normal = 1  /**< \brief Normal operation */
} IfxEvadc_FastCompareAnalogConverterControl;

/** \brief Defines the source of the value(s) in bitfield FCM.B.FCREF
 *
 * Note: Software can only write to bitfield FCM.B.FCREF, while AUE = 00B.
 */
typedef enum
{
    IfxEvadc_FastCompareAutomaticUpdate_sw             = 0,  /**< \brief No automatic update.Value(s) written by software. */
    IfxEvadc_FastCompareAutomaticUpdate_alternateValue = 1,  /**< \brief While gate is active (high), value is copied from bitfield FCM.B.FCRCOMPA and while gate is inactive (low), value is copied from bitfield FCM.B.FCRCOMPB */
    IfxEvadc_FastCompareAutomaticUpdate_rampCounter    = 2,  /**< \brief Value(s) copied from ramp counter on ramp start or counter
                                                              * update. */
    IfxEvadc_FastCompareAutomaticUpdate_analogSource   = 3   /**< \brief Value(s) written by the associated converter */
} IfxEvadc_FastCompareAutomaticUpdate;

/** \brief Defines the basic run conditions of the fast compare channel in FCM.B.RUNCOMP
 */
typedef enum
{
    IfxEvadc_FastCompareRunControl_stop      = 0, /**< \brief Stop, no operation */
    IfxEvadc_FastCompareRunControl_alwaysRun = 1  /**< \brief Always run */
} IfxEvadc_FastCompareRunControl;

/** \brief Criteria/mode settings for service request generation in FCM.B.SRG
 */
typedef enum
{
    IfxEvadc_FastCompareServiceRequestGeneration_off       = 0, /**< \brief No service requests are generated */
    IfxEvadc_FastCompareServiceRequestGeneration_rampEnd   = 1, /**< \brief Issue service request when the ramp counter stops */
    IfxEvadc_FastCompareServiceRequestGeneration_newValue  = 2, /**< \brief Issue service request when a value is written to FCREF */
    IfxEvadc_FastCompareServiceRequestGeneration_newResult = 3  /**< \brief Issue service request when a new result available */
} IfxEvadc_FastCompareServiceRequestGeneration;

/** \brief Gate Operating Mode to be filled in FCCTRL.B.GTMODE
 */
typedef enum
{
    IfxEvadc_GateOperatingMode_noGateFunction     = 0,  /**< \brief No gate function */
    IfxEvadc_GateOperatingMode_alternateValueMode = 1,  /**< \brief Alternate value mode */
    IfxEvadc_GateOperatingMode_lockBoundaryFlag   = 2   /**< \brief Lock boundary flag */
} IfxEvadc_GateOperatingMode;

/** \brief Ramp direction setting in FCM.B.FCRDIR
 */
typedef enum
{
    IfxEvadc_RampDirection_down = 0,  /**< \brief Decrement ramp counter and stop when counter <= FCRCOMPB */
    IfxEvadc_RampDirection_up   = 1   /**< \brief Increment ramp counter and stop when counter >= FCRCOMPB */
} IfxEvadc_RampDirection;

/** \brief Defines the run conditions for the ramp generation in FCM.B.RUNRAMP
 */
typedef enum
{
    IfxEvadc_RampRunControl_stop = 0,  /**< \brief Stop, no operation */
    IfxEvadc_RampRunControl_1    = 1,  /**< \brief Start immediately when GxFCRAMP0/1 is written */
    IfxEvadc_RampRunControl_2    = 2,  /**< \brief Start upon the selected trigger event of signal REQTRx */
    IfxEvadc_RampRunControl_3    = 3   /**< \brief Start immediately when GxFCRAMP0/1 is written and
                                        * stop upon the selected trigger event of signal REQTRx */
} IfxEvadc_RampRunControl;

/** \brief Sample timing sync settings for FCM.B.SSE
 */
typedef enum
{
    IfxEvadc_SampleSynchronization_off                     = 0, /**< \brief No synchronization */
    IfxEvadc_SampleSynchronization_synchronizeSampleTiming = 1  /**< \brief Sample timing is synchronized.This is recommended for operation of several ADCs. */
} IfxEvadc_SampleSynchronization;

/** \brief Trigger operating mode in Fast Compare channels, to be entered in FCCTRL.B.XTMODE
 */
typedef enum
{
    IfxEvadc_TriggerOperatingMode_noExternalTrigger = 0,  /**< \brief No external trigger */
    IfxEvadc_TriggerOperatingMode_fallingEdge       = 1,  /**< \brief Trigger event upon a falling edge */
    IfxEvadc_TriggerOperatingMode_risingEdge        = 2,  /**< \brief Trigger event upon a rising edge */
    IfxEvadc_TriggerOperatingMode_anyEdge           = 3   /**< \brief Trigger event upon any edge */
} IfxEvadc_TriggerOperatingMode;

/** \} */

/** \brief Defines the way the analog clock is generated.
 */
typedef enum
{
    IfxEvadc_AnalogClockGenerationMode_synchronized   = 0, /**< \brief Synchronized mode: Initial clock pulse is defined by the phase synchronizer */
    IfxEvadc_AnalogClockGenerationMode_unsynchronized = 1  /**< \brief Unsynchronized mode: The analog clock is generated independently. */
} IfxEvadc_AnalogClockGenerationMode;

typedef enum
{
    IfxEvadc_AnalogClockSynchronizationDelay_0 = 0,  /**< \brief No Delay */
    IfxEvadc_AnalogClockSynchronizationDelay_1 = 1,  /**< \brief 1 clock cycle delay */
    IfxEvadc_AnalogClockSynchronizationDelay_2 = 2,  /**< \brief 2 clock cycles delay */
    IfxEvadc_AnalogClockSynchronizationDelay_3 = 3,  /**< \brief 3 clock cycles delay */
    IfxEvadc_AnalogClockSynchronizationDelay_4 = 4,  /**< \brief 4 clock cycles delay */
    IfxEvadc_AnalogClockSynchronizationDelay_5 = 5,  /**< \brief 5 clock cycles delay */
    IfxEvadc_AnalogClockSynchronizationDelay_6 = 6,  /**< \brief 6 clock cycles delay */
    IfxEvadc_AnalogClockSynchronizationDelay_7 = 7   /**< \brief 7 clock cycles delay */
} IfxEvadc_AnalogClockSynchronizationDelay;

/** \brief Analog Input Precharge Control for Standard Conversions
 */
typedef enum
{
    IfxEvadc_AnalogInputPrechargeControl_noPrecharge = 0,  /**< \brief No Precharge */
    IfxEvadc_AnalogInputPrechargeControl_8Cycles     = 1,  /**< \brief Precharge for 8 clock cycles */
    IfxEvadc_AnalogInputPrechargeControl_16Cycles    = 2,  /**< \brief Precharge for 16 clock cycles */
    IfxEvadc_AnalogInputPrechargeControl_32cycles    = 3   /**< \brief Precharge for 32 clock cycles */
} IfxEvadc_AnalogInputPrechargeControl;

typedef enum
{
    IfxEvadc_ArbitrationMode_0 = 0,
    IfxEvadc_ArbitrationMode_1 = 1
} IfxEvadc_ArbitrationMode;

typedef enum
{
    IfxEvadc_CalibrationSampleTimeControl_2 = 0,  /**< \brief 2*t_ADCI */
    IfxEvadc_CalibrationSampleTimeControl_4 = 1,  /**< \brief 4*t_ADCI */
    IfxEvadc_CalibrationSampleTimeControl_6 = 2,  /**< \brief 6*t_ADCI */
    IfxEvadc_CalibrationSampleTimeControl_8 = 3   /**< \brief 8*t_ADCI */
} IfxEvadc_CalibrationSampleTimeControl;

/** \brief Conversion Mode for standard Conversion
 *  channel precision defined in MODULE_EVADC.G[x].CHCTR[y].B.ICLASS[y].B.CMS(x=0,1,...,11;y=0,1)
 */
typedef enum
{
    IfxEvadc_ChannelNoiseReduction_standardConversion = 0,  /**< \brief Standard Conversions mode */
    IfxEvadc_ChannelNoiseReduction_level1             = 1,  /**< \brief Noise reduction conversion level 1 */
    IfxEvadc_ChannelNoiseReduction_level2             = 2,  /**< \brief Noise reduction conversion level 2 */
    IfxEvadc_ChannelNoiseReduction_level3             = 3   /**< \brief Noise reduction conversion level 3 */
} IfxEvadc_ChannelNoiseReduction;

/** \brief Conversion Mode for EMUX Conversions
 */
typedef enum
{
    IfxEvadc_ChannelNoiseReductionEMUX_standardConversion = 0,  /**< \brief Standard conversion */
    IfxEvadc_ChannelNoiseReductionEMUX_level1             = 1,  /**< \brief Noise reduction conversion level 1, 1 additional conversion step */
    IfxEvadc_ChannelNoiseReductionEMUX_level2             = 2,  /**< \brief Noise reduction conversion level 2, 3 additional conversion steps */
    IfxEvadc_ChannelNoiseReductionEMUX_level3             = 3   /**< \brief Noise reduction conversion level 3, 7 additional conversion steps */
} IfxEvadc_ChannelNoiseReductionEMUX;

/** \brief Data Modification Mode
 */
typedef enum
{
    IfxEvadc_DataModificationMode_standardDataReduction = 0,  /**< \brief Standard Data Reduction */
    IfxEvadc_DataModificationMode_resultFilteringMode   = 1,  /**< \brief Result Filtering Mode */
    IfxEvadc_DataModificationMode_differenceMode        = 2   /**< \brief Difference Mode */
} IfxEvadc_DataModificationMode;

typedef enum
{
    IfxEvadc_DataReductionControlMode_0  = 0,   /**< \brief If DMM=0b00, Data reduction disabled,if DMM=0b01, FIR:a=2,b=1,c=0 */
    IfxEvadc_DataReductionControlMode_1  = 1,   /**< \brief If DMM=0b00, Accumulate 2 result values,if DMM=0b01, FIR:a=1,b=2,c=0 */
    IfxEvadc_DataReductionControlMode_2  = 2,   /**< \brief If DMM=0b00, Accumulate 3 result values,if DMM=0b01, FIR:a=2,b=0,c=1 */
    IfxEvadc_DataReductionControlMode_3  = 3,   /**< \brief If DMM=0b00, Accumulate 4 result values,if DMM=0b01, FIR:a=1,b=1,c=1 */
    IfxEvadc_DataReductionControlMode_4  = 4,   /**< \brief If DMM=0b00, Accumulate 5 result values,if DMM=0b01, FIR:a=1,b=0,c=2 */
    IfxEvadc_DataReductionControlMode_5  = 5,   /**< \brief If DMM=0b00, Accumulate 6 result values,if DMM=0b01, FIR:a=3,b=1,c=0 */
    IfxEvadc_DataReductionControlMode_6  = 6,   /**< \brief If DMM=0b00, Accumulate 7 result values,if DMM=0b01, FIR:a=2,b=2,c=0 */
    IfxEvadc_DataReductionControlMode_7  = 7,   /**< \brief If DMM=0b00, Accumulate 8 result values,if DMM=0b01, FIR:a=1,b=3,c=0 */
    IfxEvadc_DataReductionControlMode_8  = 8,   /**< \brief If DMM=0b00, Accumulate 9 result values,if DMM=0b01, FIR:a=3,b=0,c=1 */
    IfxEvadc_DataReductionControlMode_9  = 9,   /**< \brief If DMM=0b00, Accumulate 10 result values,if DMM=0b01, FIR:a=2,b=1,c=1 */
    IfxEvadc_DataReductionControlMode_10 = 10,  /**< \brief If DMM=0b00, Accumulate 11 result values,if DMM=0b01, FIR:a=1,b=2,c=1 */
    IfxEvadc_DataReductionControlMode_11 = 11,  /**< \brief If DMM=0b00, Accumulate 12 result values,if DMM=0b01, FIR:a=2,b=0,c=2 */
    IfxEvadc_DataReductionControlMode_12 = 12,  /**< \brief If DMM=0b00, Accumulate 13 result values,if DMM=0b01, FIR:a=1,b=1,c=2 */
    IfxEvadc_DataReductionControlMode_13 = 13,  /**< \brief If DMM=0b00, Accumulate 14 result values,if DMM=0b01, FIR:a=1,b=0,c=3 */
    IfxEvadc_DataReductionControlMode_14 = 15,  /**< \brief If DMM=0b00, Accumulate 15 result values,if DMM=0b01, IIR:a=2,b=2 */
    IfxEvadc_DataReductionControlMode_15 = 15   /**< \brief If DMM=0b00, Accumulate 16 result values,if DMM=0b01, IIR:a=3,b=4 */
} IfxEvadc_DataReductionControlMode;

/** \brief Contains options for Idle Precharge levels
 */
typedef enum
{
    IfxEvadc_IdlePrecharge_currentLevel = 0,  /**< \brief The sampling capacitor keeps the current charge */
    IfxEvadc_IdlePrecharge_referenceBy2 = 1   /**< \brief ThSampling capacitor is precharged to approx. half the reference when idle */
} IfxEvadc_IdlePrecharge;

/** \brief Selects the MSB conversion time(in terms of t_ADC)
 */
typedef enum
{
    IfxEvadc_MsbConversionTime_singleClockCycle = 0,  /**< \brief 1 clock cycle for the MSB (standard) */
    IfxEvadc_MsbConversionTime_doubleClockCycle = 1   /**< \brief 2 clock cycle for the MSB */
} IfxEvadc_MsbConversionTime;

/** \brief Reference Precharge Control
 */
typedef enum
{
    IfxEvadc_ReferencePrechargeControl_singleClockPhase  = 0, /**< \brief Precharge the reference input for 1 clock phase */
    IfxEvadc_ReferencePrechargeControl_singleClockPeriod = 1  /**< \brief Precharge the reference input for 1 clock period */
} IfxEvadc_ReferencePrechargeControl;

typedef enum
{
    IfxEvadc_SpreadEarlySamplePointEMUXConversionsMode_nominal = 0,  /**< \brief Nominal sample timing */
    IfxEvadc_SpreadEarlySamplePointEMUXConversionsMode_spread  = 1   /**< \brief Spread sample timing, end of sample phase is varied */
} IfxEvadc_SpreadEarlySamplePointEMUXConversionsMode;

/** \brief Spread Early Sample Point for Standard Conversions
 */
typedef enum
{
    IfxEvadc_SpreadEarlySamplePointStandardConversionsMode_nominal = 0,  /**< \brief Nominal sample timing */
    IfxEvadc_SpreadEarlySamplePointStandardConversionsMode_spread  = 1   /**< \brief Spread sample timing, end of sample phase is varied */
} IfxEvadc_SpreadEarlySamplePointStandardConversionsMode;

/** \brief The 0-1 transition of bit SUCAL initiates the start-up calibrationphase of
 * all analog converters (except for the fast compare channels).
 * Note: The start-up calibration will abort any running conversion
 * and cannot be stopped before it is complete.
 */
typedef enum
{
    IfxEvadc_StartupCalibration_noAction = 0,  /**< \brief No action */
    IfxEvadc_StartupCalibration_initiate = 1   /**< \brief Initiate the start-up calibration phase
                                                * (indication in bit GxARBCFG.CAL) */
} IfxEvadc_StartupCalibration;

/** \brief Adjusts the analog circuitry to the supply voltage used in the application
 * system. Make sure to keep SUPLEV = 0b00 or 0b01 in the case of a 5 V supply.
 */
typedef enum
{
    IfxEvadc_SupplyVoltageLevelControl_automaticControl = 0,  /**< \brief Automatic control: voltage range is controlled by the power supply */
    IfxEvadc_SupplyVoltageLevelControl_upperVoltage     = 1,  /**< \brief Upper voltage range: assume a 5 V power supply is connected */
    IfxEvadc_SupplyVoltageLevelControl_lowerVoltage     = 2   /**< \brief Lower voltage range: assume a 3.3 V power supply is connected */
} IfxEvadc_SupplyVoltageLevelControl;

/** \brief Wait-for-Read Mode Enable
 */
typedef enum
{
    IfxEvadc_WaitForRead_overwriteMode = 0,  /**< \brief Overwrite Mode */
    IfxEvadc_WaitForRead_enabled       = 1   /**< \brief Wait-for-read mode enabled for this register */
} IfxEvadc_WaitForRead;

/** \addtogroup IfxLld_Evadc_Std_Global
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief access function to enable/disable  wait for read mode for result registers
 * \param group pointer to the EVADC group
 * \param resultIdx result register index
 * \param enabled wait for read mode enabled/disabled
 * \return None
 */
IFX_INLINE void IfxEvadc_configureWaitForReadMode(Ifx_EVADC_G *group, uint32 resultIdx, boolean enabled);

/** \brief access function to enable/disable  wait for read mode for global result register
 * \param evadc pointer to the EVADC
 * \param enabled wait for read mode enabled/disabled
 * \return None
 */
IFX_INLINE void IfxEvadc_configureWaitForReadModeForGlobalResultRegister(Ifx_EVADC *evadc, boolean enabled);

/** \brief get the ADC Module  noise reduction level
 * \param evadc pointer to EVADC
 * \param inputClassNum Adc Module input Class number
 * \return Adc input class noise reduction level
 */
IFX_INLINE IfxEvadc_ChannelNoiseReduction IfxEvadc_getGlobalConversionMode(Ifx_EVADC *evadc, uint8 inputClassNum);

/** \brief return conversion result stored in the Global result Register
 * \param evadc pointer to the EVADC module
 * \return global result register
 *
 * \code
 * Ifx_EVADC* evadc = &MODULE_EVADC; // module pointer
 * IfxEvadc_GroupId groupId = IfxEvadc_GroupId0; // for group 0
 * uint32 channels = (1 << 5) | (1 << 2); // enable channel #5 and #2
 * uint32 mask = (1 << 7) | (1 << 5) | (1 << 2); // modify the selection for channel #7, #5 and #2; channel #7 will be disabled
 *
 * //confiure wait for read mode for global result register
 * IfxEvadc_configureWaitForReadModeForGlobalResultRegister(evadc, TRUE);
 *
 * // enable all arbiter request sources
 * adcGroupConfig.arbiter.requestSlotQueue0Enabled           = TRUE; // enable Queue0 mode
 * // enable all gates in "always" mode (no edge detection)
 * adcGroupConfig.queueRequest[0].triggerConfig.gatingMode = IfxEvadc_GatingMode_always;
 *
 * // Add channel to queue with refill enabled
 *  IfxEvadc_Adc_addToQueue(&adcChannel, IFXEVADC_QUEUE_REFILL, IfxEvadc_RequestSource_queue0);
 *
 *  // start the Queue
 *  IfxEvadc_Adc_startQueue(&adcGroup, IfxEvadc_RequestSource_queue0);
 *
 *
 *
 *
 *
 * Ifx_EVADC_GLOB.RES result;
 * result = IfxVadc_getGlobalResult (evadc);
 *
 * \endcode
 *
 */
IFX_INLINE Ifx_EVADC_GLOB_RES IfxEvadc_getGlobalResult(Ifx_EVADC *evadc);

/** \brief Get Sample Time in sec
 * \param evadc Pointer to EVADC Module
 * \param inputClassNum ADC Input Class Number
 * \param analogFrequency ADC module analog frequency in Hz
 * \return ADC input class channel sample time in sec.
 */
IFX_INLINE float32 IfxEvadc_getGlobalSampleTime(Ifx_EVADC *evadc, uint8 inputClassNum, float32 analogFrequency);

/** \brief Get conversion result (Function does not care about the alignment)
 * value = raw * gain + offset.
 * \param group pointer to the EVADC group
 * \param resultIdx result register index
 * \return scaled Conversion result
 *
 * \code
 * Ifx_EVADC_G* group = &MODULE_EVADC.G[0]; // for group 0
 * uint32 channels = (1 << 5) | (1 << 2); // enable channel #5 and #2
 * uint32 mask = (1 << 7) | (1 << 5) | (1 << 2); // modify the selection for channel #7, #5 and #2; channel #7 will be disabled
 *
 * //confiure wait for read mode for global result register
 * IfxEvadc_configureWaitForReadMode(group,IfxEvadc_ChannelResult0, TRUE);
 * IfxEvadc_configureWaitForReadMode(group,IfxEvadc_ChannelResult1, TRUE);
 *
 *
 * // Add channel to queue with refill enabled
 *  IfxEvadc_Adc_addToQueue(&adcChannel, IfxEvadc_RequestSource_queue0, IFXEVADC_QUEUE_REFILL);
 *
 * // start the Queue
 *  IfxEvadc_Adc_startQueue(&adcGroup, IfxEvadc_RequestSource_queue0);
 *
 *
 * // wait for conversion to finish
 *
 * // fetch the result of conversion from result register 0 for group 0
 * Ifx_EVADC_G_RES result;
 * result = IfxEvadc_getResult(group, IfxEvadc_ChannelResult0);
 * \endcode
 *
 */
IFX_INLINE Ifx_EVADC_G_RES IfxEvadc_getResult(Ifx_EVADC_G *group, uint32 resultIdx);

/** \brief Sets the sample time in sec of ADC global class.
 * \param evadc pointer to EVADC module registers.
 * \param groupInputClassNum group input class  number.
 * \param analogFrequency ADC analog frequency in Hz.
 * \param sampleTime the requested sample time in sec for input class.
 * \return None
 */
IFX_INLINE void IfxEvadc_setGlobalSampleTime(Ifx_EVADC *evadc, uint8 groupInputClassNum, float32 analogFrequency, float32 sampleTime);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Get conversion result (Function does not care about the alignment)
 * value = raw * gain + offset.
 * \param evadc EVADC module pointer
 * \param group pointer to the EVADC group
 * \param channel channel Id
 * \param sourceType type of request source
 * \return scaled Conversion result
 *
 * \code
 * Ifx_EVADC evadc;
 * evadc.evadc = &MODULE_EVADC;
 * Ifx_EVADC_G* group = &MODULE_EVADC.G[0]; // for group 0
 * uint32 channels = (1 << 5) | (1 << 2); // enable channel #5 and #2
 * uint32 mask = (1 << 7) | (1 << 5) | (1 << 2); // modify the selection for channel #7, #5 and #2; channel #7 will be disabled
 *
 * //confiure wait for read mode for global result register
 * IfxEvadc_configureWaitForReadMode(group,IfxEvadc_ChannelResult0, TRUE);
 * IfxEvadc_configureWaitForReadMode(group,IfxEvadc_ChannelResult1, TRUE);
 *
 * // Add channel to queue with refill enabled
 *  IfxEvadc_Adc_addToQueue(&adcChannel, IfxEvadc_RequestSource_queue0, IFXEVADC_QUEUE_REFILL);
 *
 * // start the Queue
 *  IfxEvadc_Adc_startQueue(&adcGroup, IfxEvadc_RequestSource_queue0);
 *
 * // fetch the result of conversion for channel 2 of group 0
 * Ifx_EVADC_G_RESresult2;
 * result = IfxEvadc_getResultBasedOnRequestSource(&evadc, group, IfxVadc_ChannelId2,IfxEvadc_RequestSource_queue0 );
 * Ifx_EVADC_G_RESresult5;
 * result = IfxEvadc_getResultBasedOnRequestSource(&evadc, group, IfxEvadc_ChannelId5,IfxEvadc_RequestSource_queue0 );
 * \endcode
 *
 */
IFX_EXTERN Ifx_EVADC_G_RES IfxEvadc_getResultBasedOnRequestSource(Ifx_EVADC *evadc, Ifx_EVADC_G *group, IfxEvadc_ChannelId channel, IfxEvadc_RequestSource sourceType);

/** \} */

/** \addtogroup IfxLld_Evadc_Std_QueueRequest
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Clears all the queue entries including backup stage.
 * \param evadcG pointer to EVADC group registers.
 * \param flushQueue Whether queue is cleared or not.
 * \param sourceType Request source Type(Queue 0,Queue 1,Queue 2)
 * \return None
 */
IFX_INLINE void IfxEvadc_clearQueue(Ifx_EVADC_G *evadcG, boolean flushQueue, IfxEvadc_RequestSource sourceType);

/** \brief Starts a queue of a group by generating a trigger event through software
 * \param group pointer to the EVADC group
 * \param sourceType Request source Type(Queue 0,Queue 1,Queue 2)
 * \return None
 */
IFX_INLINE void IfxEvadc_startQueue(Ifx_EVADC_G *group, IfxEvadc_RequestSource sourceType);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Add an entry to the queue of a group for the specified channel with the following options set:
 * refill incase of aborted conversion
 * source interrupt enable/disable
 * external trigger control of the aborted conversion
 * \param group pointer to the EVADC group
 * \param channel specifies channel Id
 * \param sourceType Request source Type(Queue 0,Queue 1,Queue 2)
 * \param options specifies the refill, source interrupt enable/disable and external trigger control selection
 * \return None
 *
 * For coding example see: \ref IfxEvadc_Adc_getResult
 *
 */
IFX_EXTERN void IfxEvadc_addToQueue(Ifx_EVADC_G *group, IfxEvadc_ChannelId channel, IfxEvadc_RequestSource sourceType, uint32 options);

/** \brief Disables the external trigger.
 * \param evadcG pointer to EVADC group registers.
 * \param sourceType Request source Type(Queue 0,Queue 1,Queue 2)
 * \return None
 */
IFX_EXTERN void IfxEvadc_disableQueueSlotExternalTrigger(Ifx_EVADC_G *evadcG, IfxEvadc_RequestSource sourceType);

/** \brief Enables the external trigger.
 * \param evadcG pointer to EVADC group registers.
 * \param sourceType Request source Type(Queue 0,Queue 1,Queue 2)
 * \return None
 */
IFX_EXTERN void IfxEvadc_enableQueueSlotExternalTrigger(Ifx_EVADC_G *evadcG, IfxEvadc_RequestSource sourceType);

/** \brief Gets the requested queue slot gating mode.
 * \param evadcG pointer to EVADC group registers.
 * \param sourceType Request source Type(Queue 0,Queue 1,Queue 2)
 * \return requested queue0 slot gating mode.
 */
IFX_EXTERN IfxEvadc_GatingMode IfxEvadc_getQueueSlotGatingMode(Ifx_EVADC_G *evadcG, IfxEvadc_RequestSource sourceType);

/** \brief Gets the requested queue slot gating input.
 * \param evadcG pointer to EVADC group registers.
 * \param sourceType Request source Type(Queue 0,Queue 1,Queue 2)
 * \return requested queue0 slot gating input.
 */
IFX_EXTERN IfxEvadc_GatingSource IfxEvadc_getQueueSlotGatingSource(Ifx_EVADC_G *evadcG, IfxEvadc_RequestSource sourceType);

/** \brief Gets the request queue slot priority.
 * \param evadcG pointer to EVADC group registers.
 * \param sourceType Request source Type(Queue 0,Queue 1,Queue 2)
 * \return requested queue0 slot priority.
 */
IFX_EXTERN IfxEvadc_RequestSlotPriority IfxEvadc_getQueueSlotPriority(Ifx_EVADC_G *evadcG, IfxEvadc_RequestSource sourceType);

/** \brief Gets the requested queue slot start mode.
 * \param evadcG pointer to EVADC group registers.
 * \param sourceType Request source Type(Queue 0,Queue 1,Queue 2)
 * \return requested queue slot start mode.
 */
IFX_EXTERN IfxEvadc_RequestSlotStartMode IfxEvadc_getQueueSlotStartMode(Ifx_EVADC_G *evadcG, IfxEvadc_RequestSource sourceType);

/** \brief Gets the requested queue slot trigger input.
 * \param evadcG pointer to EVADC group registers.
 * \param sourceType Request source Type(Queue 0,Queue 1,Queue 2)
 * \return requested queue slot trigger input.
 */
IFX_EXTERN IfxEvadc_TriggerSource IfxEvadc_getQueueSlotTriggerInput(Ifx_EVADC_G *evadcG, IfxEvadc_RequestSource sourceType);

/** \brief Gets the requested queue slot trigger mode.
 * \param evadcG pointer to EVADC group registers.
 * \param sourceType Request source Type(Queue 0,Queue 1,Queue 2)
 * \return requested queue slot trigger mode.
 */
IFX_EXTERN IfxEvadc_TriggerMode IfxEvadc_getQueueSlotTriggerMode(Ifx_EVADC_G *evadcG, IfxEvadc_RequestSource sourceType);

/** \brief Gives the status of the Queue of a group by returning non zero value if the Queue is full
 * \param group pointer to the EVADC group
 * \param sourceType sourceType Request source Type(Queue 0,Queue 1,Queue 2)
 * \return status of the Queue
 *
 * \code
 * Ifx_EVADC_G* group = &MODULE_EVADC.G[0]; // for group 0
 * boolean queueFull = (IfxEvadc_getQueueStatus(group)==IfxEvadc_Status_queueFull)?TRUE:FALSE; // get Queue status for group 0
 * \endcode
 *
 */
IFX_EXTERN IfxEvadc_Status IfxEvadc_getQueueStatus(Ifx_EVADC_G *group, IfxEvadc_RequestSource sourceType);

/** \brief Returns the queue slot requested status.
 * \param evadcG pointer to EVADC group registers.
 * \param sourceType Request source Type(Queue 0,Queue 1,Queue 2)
 * \return TRUE if queue slot request enabled otherwise FALSE.
 */
IFX_EXTERN boolean IfxEvadc_isRequestQueueSlotEnabled(Ifx_EVADC_G *evadcG, IfxEvadc_RequestSource sourceType);

/** \brief Sets the gating configurations.
 * \param evadcG pointer to EVADC group registers.
 * \param gatingSource gate input for group.
 * \param gatingMode gating mode. High level, Low Level  or Gating disabled.
 * \param sourceType Request source Type(Queue 0,Queue 1,Queue 2)
 * \return None
 */
IFX_EXTERN void IfxEvadc_setQueueSlotGatingConfig(Ifx_EVADC_G *evadcG, IfxEvadc_GatingSource gatingSource, IfxEvadc_GatingMode gatingMode, IfxEvadc_RequestSource sourceType);

/** \brief Sets the trigger operating configurations.
 * \param evadcG pointer to EVADC group registers.
 * \param triggerMode trigger mode. Rising, falling any edge leads to an trigger event.
 * \param triggerSource trigger input for group.
 * \param sourceType Request source Type(Queue 0,Queue 1,Queue 2)
 * \return None
 */
IFX_EXTERN void IfxEvadc_setQueueSlotTriggerOperatingConfig(Ifx_EVADC_G *evadcG, IfxEvadc_TriggerMode triggerMode, IfxEvadc_TriggerSource triggerSource, IfxEvadc_RequestSource sourceType);

/** \} */

/** \addtogroup IfxLld_Evadc_Std_IO
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Initializes a EMUX output
 * \param emux the Emux Pin which should be configured
 * \param outputMode the pin output mode which should be configured
 * \param padDriver the pad driver mode which should be configured
 * \return None
 */
IFX_INLINE void IfxEvadc_initEmuxPin(const IfxEvadc_Emux_Out *emux, IfxPort_OutputMode outputMode, IfxPort_PadDriver padDriver);

/** \brief Initializes a GxBFL output
 * \param gxBfl the GxBFL Pin which should be configured
 * \param outputMode the pin output mode which should be configured
 * \param padDriver the pad driver mode which should be configured
 * \return None
 */
IFX_INLINE void IfxEvadc_initGxBflPin(const IfxEvadc_GxBfl_Out *gxBfl, IfxPort_OutputMode outputMode, IfxPort_PadDriver padDriver);

/** \} */

/** \addtogroup IfxLld_Evadc_Std_Frequency
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Calculate the time using analog frequency.
 * \param analogFrequency analog frequency in Hz.
 * \param sampleTime sample time in sec.
 * \return sample time in sec.
 */
IFX_INLINE uint32 IfxEvadc_calculateSampleTime(float32 analogFrequency, float32 sampleTime);

/** \brief Returns the configured Fadci EVADC analog clock frequency in Hz.
 * \param group pointer to the EVADC registers group
 * \return the configured Fadci EVADC analog clock frequency in Hz.
 */
IFX_INLINE float32 IfxEvadc_getAdcAnalogFrequency(Ifx_EVADC_G *group);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief configured Fadcd EVADC digital clock frequency in Hz.
 * \param evadc pointer to the base of EVADC registers
 * \return the configured Fadcd EVADC digital clock frequency in Hz.
 */
IFX_EXTERN float32 IfxEvadc_getAdcDigitalFrequency(Ifx_EVADC *evadc);

/** \} */

/** \addtogroup IfxLld_Evadc_Std_Group
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/**
 * \param evadcG Pointer to EVADC SFRs of a particular group
 * \param group Group No.
 * \param delay Selects an additional clock cycle for the conversion step of the MSB
 * \return None
 */
IFX_INLINE void IfxEvadc_setAnalogClockSynchronizationDelay(Ifx_EVADC_G *evadcG, IfxEvadc_GroupId group, IfxEvadc_AnalogClockSynchronizationDelay delay);

/** \brief Select calibration sample time in units of t_ADC
 * \param evadcG Pointer to EVADC SFRs of a particular group
 * \param group Group No.
 * \param mode Select calibration sample time in units of t_ADC
 * \return None
 */
IFX_INLINE void IfxEvadc_setCalibrationSampleTime(Ifx_EVADC_G *evadcG, IfxEvadc_GroupId group, IfxEvadc_CalibrationSampleTimeControl mode);

/** \brief Clears the all group requests.
 * \param evadcG pointer to EVADC group registers.
 * \return None
 */
IFX_INLINE void IfxEvadc_clearAllResultRequests(Ifx_EVADC_G *evadcG);

/** \brief Gets the EVADC input class channel conversion mode
 * \param evadcG pointer to EVADC group registers.
 * \param inputClassNum EVADC input class number.
 * \return EVADC input class channel resolution.
 */
IFX_INLINE IfxEvadc_ChannelNoiseReduction IfxEvadc_getGroupConversionMode(Ifx_EVADC_G *evadcG, uint8 inputClassNum);

/** \brief Get conversion result for the group
 * \param group pointer to the EVADC group
 * \param results pointer to scaled conversion results
 * \param resultOffset offset for the first result
 * \param numResults number of results
 * \return None
 *
 * \code
 * Here,Three channels are used for queued transfers
 * \code
 *
 * unsigned savedGate = adcGroup.module.evadc->G[adcGroup.groupId].Queue0.QMR0.B.ENGT;
 *
 * // IMPORTANT: for deterministic results we have to disable the queue gate
 * // while filling the queue, otherwise results could be output in the wrong order
 * adcGroup.module.evadc->G[adcGroup.groupId].Queue0.QMR0.B.ENGT = 0;
 *
 * // create channel config
 * IfxEvadc_Adc_ChannelConfig adcChannelConfig[3];
 * IfxEvadc_Adc_Channel adcChannel[3];
 *
 * for(int chnIx=0; chnIx<3; ++chnIx) {
 * IfxEvadc_Adc_initChannelConfig(&adcChannelConfig[chnIx], &adcGroup);
 *
 * adcChannelConfig[chnIx].channelId = (IfxEvadc_ChannelId)(chnIx);
 * adcChannelConfig[chnIx].resultRegister = IfxEvadc_ChannelResult_1; // use result register #1 for all channels
 *
 * // initialize the channel
 * IfxEvadc_Adc_initChannel(&adcChannel[chnIx], &adcChannelConfig[chnIx]);
 *
 * // Add channel to queue with refill enabled
 * IfxEvadc_Adc_addToQueue(&adcChannel[chnIx], IfxEvadc_RequestSource_queue0, IFXEVADC_QUEUE_REFILL);
 *
 * // restore previous gate config
 * adcGroup.module.evadc->G[adcGroup.groupId].Queue0.QMR0.B.ENGT = savedGate;
 *
 * // start the Queue
 * IfxEvadc_Adc_startQueue(&adcGroup,IfxEvadc_RequestSource_queue0); // just for the case that somebody copy&pastes the code - the queue has already been started in previous test
 *
 * Ifx_EVADC_G_RES results[10];
 * result = IfxEvadc_getGroupResult(group, results, 0, 2);
 * \endcode
 *
 */
IFX_INLINE void IfxEvadc_getGroupResult(Ifx_EVADC_G *group, Ifx_EVADC_G_RES *results, uint32 resultOffset, uint32 numResults);

/** \brief Gets the EVADC input class channel sample time.
 * \param evadcG pointer to EVADC group registers.
 * \param inputClassNum ADC input class number.
 * \param analogFrequency ADC module analog frequency in Hz.
 * \return ADC input class channel sample time in sec.
 */
IFX_INLINE float32 IfxEvadc_getGroupSampleTime(Ifx_EVADC_G *evadcG, uint8 inputClassNum, float32 analogFrequency);

/** \brief Gets the configured master index.
 * \param evadcG pointer to EVADC group registers.
 * \return configured master kernel index.
 */
IFX_INLINE uint8 IfxEvadc_getMasterIndex(Ifx_EVADC_G *evadcG);

/** \brief Selects the chargingmode/voltage level in idle mode
 * \param evadcG Pointer to EVADC SFRs of a particular group
 * \param group Group No.
 * \param mode 0 - No precharge, the sampling capacitor keeps the current charge
 * 1 - The sampling capacitor is precharged to approx. half the reference when idle
 * \return None
 */
IFX_INLINE void IfxEvadc_selectIdlePrechargeLevel(Ifx_EVADC_G *evadcG, IfxEvadc_GroupId group, IfxEvadc_IdlePrecharge mode);

/**
 * \param evadcG Pointer to EVADC SFRs of a particular group
 * \param group Group No.
 * \return None
 */
IFX_INLINE void IfxEvadc_enableInputBuffer(Ifx_EVADC_G *evadcG, IfxEvadc_GroupId group);

/**
 * \param evadcG Pointer to EVADC SFRs of a particular group
 * \param group Group No.
 * \param duration Selects the amount of time for reference input precharging
 * \return None
 */
IFX_INLINE void IfxEvadc_setReferencePrechargeDuration(Ifx_EVADC_G *evadcG, IfxEvadc_GroupId group, IfxEvadc_ReferencePrechargeControl duration);

/**
 * \param evadcG Pointer to EVADC SFRs of a particular group
 * \param group EVADC Group No.
 * \return None
 */
IFX_INLINE void IfxEvadc_enableReferencePrecharge(Ifx_EVADC_G *evadcG, IfxEvadc_GroupId group);

/** \brief Resets the ADC group.
 * \param evadcG pointer to EVADC group registers.
 * \return None
 */
IFX_INLINE void IfxEvadc_resetGroup(Ifx_EVADC_G *evadcG);

/** \brief Selects whether sample synchronization must be performed or not.
 * \param evadcG Pointer to EVADC SFRs of a particular group
 * \param group Group No.
 * \param mode Sample Synchronization Options
 * \return None
 */
IFX_INLINE void IfxEvadc_setSampleSynchronizationMode(Ifx_EVADC_G *evadcG, IfxEvadc_GroupId group, IfxEvadc_SampleSynchronization mode);

/** \brief Selects an additional clock cycle for the conversion step of the MSB.
 * \param evadcG Pointer to EVADC SFRs of a particular group
 * \param group EVADC Group No.
 * \param mode Selects an single or double clock cycle(s) for the conversion step of the MSB
 * \return None
 */
IFX_INLINE void IfxEvadc_selectDoubleClockForMSBConversion(Ifx_EVADC_G *evadcG, IfxEvadc_GroupId group, IfxEvadc_MsbConversionTime mode);

/** \brief Sets analog converter group number.
 * \param evadcG pointer to EVADC group registers.
 * \param analogConverterMode group analog converter mode.
 * \return None
 */
IFX_INLINE void IfxEvadc_setAnalogConvertControl(Ifx_EVADC_G *evadcG, IfxEvadc_AnalogConverterMode analogConverterMode);

/**
 * \param evadcG Pointer to EVADC SFRs of a particular group
 * \param inputClassNum Group No.
 * \param mode
 * \return None
 */
IFX_INLINE void IfxEvadc_setAnalogInputPrechargeControlEMUX(Ifx_EVADC_G *evadcG, uint8 inputClassNum, IfxEvadc_AnalogInputPrechargeControl mode);

/** \brief Analog Input Precharge Control for Standard Conversions
 * Buffer must be enabled by GxANCFG.B.BE = 1
 * \param evadcG Pointer to EVADC SFRs of a particular group
 * \param inputClassNum Class No.
 * \param mode
 * \return None
 */
IFX_INLINE void IfxEvadc_setAnalogInputPrechargeControlStandard(Ifx_EVADC_G *evadcG, uint8 inputClassNum, IfxEvadc_AnalogInputPrechargeControl mode);

/** \brief Sets the ADC input class channel resolution.
 * \param evadcG pointer to EVADC group registers.
 * \param inputClassNum input class number.
 * \param conversionMode ADC input class channel Conversion Mode
 * \return None
 */
IFX_INLINE void IfxEvadc_setGroupConversionMode(Ifx_EVADC_G *evadcG, uint8 inputClassNum, IfxEvadc_ChannelNoiseReduction conversionMode);

/**
 * \param evadcG Pointer to EVADC SFRs of a particular group
 * \param inputClassNum Class No.
 * \param conversionMode
 * \return None
 */
IFX_INLINE void IfxEvadc_setGroupConversionModeEMUX(Ifx_EVADC_G *evadcG, uint8 inputClassNum, IfxEvadc_ChannelNoiseReductionEMUX conversionMode);

/** \brief Sets the ADC input class sample time ticks.
 * \param evadcG pointer to EVADC group registers.
 * \param inputClassNum input class number.
 * \param analogFrequency ADC analog frequency in Hz.
 * \param sampleTime request sample time in sec for input class.
 * \return None
 */
IFX_INLINE void IfxEvadc_setGroupSampleTime(Ifx_EVADC_G *evadcG, uint8 inputClassNum, float32 analogFrequency, float32 sampleTime);

/** \brief Set the additional number of cycles(in addition to 2) in sample time.
 * \param evadcG Pointer to EVADC SFRs of a particular group
 * \param inputClassNum Class No.
 * \param analogFrequency ADC analog frequency in Hz.
 * \param sampleTime request sample time in sec for input class.
 * \return None
 */
IFX_INLINE void IfxEvadc_setGroupSampleTimeEMUX(Ifx_EVADC_G *evadcG, uint8 inputClassNum, float32 analogFrequency, float32 sampleTime);

/** \brief Sets the master index.
 * \param evadcG pointer to EVADC group registers.
 * \param masterIndex master index.
 * \return None
 */
IFX_INLINE void IfxEvadc_setMasterIndex(Ifx_EVADC_G *evadcG, uint8 masterIndex);

/**
 * \param evadcG Pointer to EVADC SFRs of a particular group
 * \param inputClassNum Class No.
 * \param mode
 * \return None
 */
IFX_INLINE void IfxEvadc_spreadEarlySamplePointEMUXConversions(Ifx_EVADC_G *evadcG, uint8 inputClassNum, IfxEvadc_SpreadEarlySamplePointEMUXConversionsMode mode);

/**
 * \param evadcG Pointer to EVADC SFRs of a particular group
 * \param inputClassNum Class No.
 * \param mode
 * \return None
 */
IFX_INLINE void IfxEvadc_spreadEarlySamplePointStandardConversions(Ifx_EVADC_G *evadcG, uint8 inputClassNum, IfxEvadc_SpreadEarlySamplePointStandardConversionsMode mode);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Sets the Arbiter slot configurations.
 * \param evadcG pointer to EVADC group registers.
 * \param slotEnable enable/disable of slot.
 * \param prio channel request priority.
 * \param mode Channel Slot start mode.
 * \param slot channel slot Request source.
 * \return None
 */
IFX_EXTERN void IfxEvadc_setArbiterPriority(Ifx_EVADC_G *evadcG, boolean slotEnable, IfxEvadc_RequestSlotPriority prio, IfxEvadc_RequestSlotStartMode mode, IfxEvadc_RequestSource slot);

/** \} */

/** \addtogroup IfxLld_Evadc_Std_Module
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Disable EVADC kernel.
 * \param evadc pointer to the base of EVADC registers.
 * \return None
 */
IFX_INLINE void IfxEvadc_disableModule(Ifx_EVADC *evadc);

/** \brief Disables the post calibration.
 * \param evadcG pointer to the base of EVADC Grroup registers.
 * \param group Index of the group.
 * \param enabled disable or not.
 * \return None
 */
IFX_INLINE void IfxEvadc_disablePostCalibration(Ifx_EVADC_G *evadcG, IfxEvadc_GroupId group, boolean enabled);

/** \brief Enable EVADC kernel.
 * \param evadc pointer to the base of EVADC registers.
 * \return None
 */
IFX_INLINE void IfxEvadc_enableModule(Ifx_EVADC *evadc);

/** \brief gets ADC Calibration Flag CAL status.
 * \param evadc pointer to EVADC group registers.
 * \param adcCalGroupNum ADC CAL group number.
 * \return CAL group status.
 */
IFX_INLINE uint8 IfxEvadc_getAdcCalibrationActiveState(Ifx_EVADC *evadc, uint8 adcCalGroupNum);

/** \brief Gets the global control configuration value.
 * \param evadc pointer to the base of EVADC registers.
 * \return global control configuration value.
 */
IFX_INLINE Ifx_EVADC_GLOBCFG IfxEvadc_getGlobalConfigValue(Ifx_EVADC *evadc);

/** \brief Gives the SRC source address.
 * \param group Index of the group
 * \param index SRC number
 * \return SRC source address
 */
IFX_INLINE volatile Ifx_SRC_SRCR *IfxEvadc_getSrcAddress(IfxEvadc_GroupId group, IfxEvadc_SrcNr index);

/** \brief get status of GLOBCFG.SUCAL Bit field
 * \param evadc Pointer to EVADC
 * \return status of GLOBCFG.SUCAL Bit field
 */
IFX_INLINE boolean IfxEvadc_getStartupCalibrationStatus(Ifx_EVADC *evadc);

/** \brief Initialises ADC converter clock.
 * \param evadc Pointer to EVADC  SFRs
 * \param evadcG Pointer to EVADC Group SFRs
 * \param converterClockDivider ADC converter clock divider.
 * \return None
 */
IFX_INLINE void IfxEvadc_initialiseAdcConverterClock(Ifx_EVADC *evadc, Ifx_EVADC_G *evadcG, IfxEvadc_ClockDividerFactor converterClockDivider);

/** \brief initiates the calibration pulse phase.
 * \param evadc pointer to the base of EVADC registers
 * \return None
 */
IFX_INLINE void IfxEvadc_initiateStartupCalibration(Ifx_EVADC *evadc);

/** \brief Returns the module's suspend state.
 * TRUE :if module is suspended.
 * FALSE:if module is not yet suspended.
 * \param evadc Pointer to EVADC module registers
 * \return Suspend status (TRUE / FALSE)
 */
IFX_INLINE boolean IfxEvadc_isModuleSuspended(Ifx_EVADC *evadc);

/** \brief Sets the channel conversion mode.
 * \param evadc pointer to EVADC module registers.
 * \param InputClassNum input class  number.
 * \param conversionMode ADC conversion Mode. Improves the precision of conversion
 * \return None
 */
IFX_INLINE void IfxEvadc_setGlobalConversionMode(Ifx_EVADC *evadc, uint8 InputClassNum, IfxEvadc_ChannelNoiseReduction conversionMode);

/** \brief Sets the sensitivity of the module to sleep signal
 * \param evadc pointer to EVADC registers
 * \param mode mode selection (enable/disable)
 * \return None
 */
IFX_INLINE void IfxEvadc_setSleepMode(Ifx_EVADC *evadc, IfxEvadc_SleepMode mode);

/** \brief Configure the Module to Hard/Soft suspend mode.
 * Note: The api works only when the OCDS is enabled and in Supervisor Mode. When OCDS is disabled the OCS suspend control is ineffective.
 * \param evadc Pointer to EVADC module registers
 * \param mode Module suspend mode
 * \return None
 */
IFX_INLINE void IfxEvadc_setSuspendMode(Ifx_EVADC *evadc, IfxEvadc_SuspendMode mode);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Disable write access to the EVADC config/control registers.
 * \param evadc pointer to the base of EVADC registers.
 * \param protectionSet Index of the bit in the ACCPROTx {x=0/1} for which write access is to be disabled.
 * \return None
 */
IFX_EXTERN void IfxEvadc_disableAccess(Ifx_EVADC *evadc, IfxEvadc_Protection protectionSet);

/** \brief Enable write access to the EVADC config/control registers.
 * \param evadc pointer to the base of EVADC registers.
 * \param protectionSet Index of the bit in the ACCPROTx {x=0/1} for which write access is to be enabled.
 * \return None
 */
IFX_EXTERN void IfxEvadc_enableAccess(Ifx_EVADC *evadc, IfxEvadc_Protection protectionSet);

/** \brief ADC Module Frequency in Hz
 * \param evadc Pointer to EVADC
 * \return module frequency
 */
IFX_EXTERN float32 IfxEvadc_getAdcModuleFrequency(Ifx_EVADC *evadc);

/** \brief Configure the FadcD evadc digital clock.
 * \param evadc pointer to the base of EVADC registers.
 * \param fAdcD ADC digital clock frequency in Hz.
 * \return calculated ADC digital clock frequency in Hz.
 */
IFX_EXTERN uint32 IfxEvadc_initializeFAdcD(Ifx_EVADC *evadc, uint32 fAdcD);

/** \brief Configure the ADC analog clock.
 * \param evadc pointer to the base of EVADC registers.
 * \param fAdcI ADC analog clock clock frequency in Hz. Range = [5000000, 10000000].
 * \param group Pointer to EVADC Group
 * \return ADC analog clock frequency in Hz.
 */
IFX_EXTERN uint32 IfxEvadc_initializeFAdcI(Ifx_EVADC *evadc, uint32 fAdcI, Ifx_EVADC_G *group);

/** \brief Resets the kernel.
 * \param evadc pointer to the  EVADC Module
 * \return None
 */
IFX_EXTERN void IfxEvadc_resetModule(Ifx_EVADC *evadc);

/** \brief Starts ADC calibration and wait for the end of the calibration process.
 * \param evadc pointer to the base of EVADC registers.
 * \return None
 */
IFX_EXTERN void IfxEvadc_startupCalibration(Ifx_EVADC *evadc);

/** \brief SW workaround of EVADC Master/Slave conversion jitter issue(MCMETILLD-910)
 * \param evadc Pointer to EVADC module registers
 * \return Used for detecting failure in either OCDS Debug Active State Activation or Trigger Line TL1 Activation or both.
 */
IFX_EXTERN boolean IfxEvadc_syncConvWorkAround(Ifx_EVADC *evadc);

/** \} */

/** \addtogroup IfxLld_Evadc_Std_Channel
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Clears the channel request.
 * \param evadcG pointer to EVADC group registers.
 * \param channelId channel id whose request to be cleared.
 * \return None
 */
IFX_INLINE void IfxEvadc_clearChannelRequest(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelId channelId);

/** \brief Enables the FIFO mode.
 * \param evadcG pointer to EVADC group registers.
 * \param resultRegister channel result register.
 * \param fifoMode FIFO mode .
 * \return None
 */
IFX_INLINE void IfxEvadc_enableFifoMode(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelResult resultRegister, IfxEvadc_FifoMode fifoMode);

/**
 * \param evadcG pointer to EVADC group registers.
 * \param resultRegister channel result register.
 * \return None
 */
IFX_INLINE void IfxEvadc_enableServiceRequest(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelResult resultRegister);

/**
 * \param evadcG Pointer to EVADC SFRs of a particular group
 * \param resultRegister
 * \return None
 */
IFX_INLINE void IfxEvadc_disableServiceRequest(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelResult resultRegister);

/** \brief Gets the current ADC channel control configurations.
 * \param evadcG pointer to EVADC group registers.
 * \param channelIndex EVADC channel number.
 * \return current ADC channel control configuration.
 */
IFX_INLINE Ifx_EVADC_G_CHCTR IfxEvadc_getChannelControlConfig(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelId channelIndex);

/** \brief get channel Input class
 * \param evadcG Pointer to EVADC Group
 * \param channelIndex Specifies channel Id
 * \return Input class
 */
IFX_INLINE IfxEvadc_InputClasses IfxEvadc_getChannelInputclass(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelId channelIndex);

/** \brief Gets the channel result service request node pointer 0.
 * \param evadcG pointer to EVADC group registers.
 * \return channel result service request node pointer 0.
 */
IFX_INLINE Ifx_EVADC_G_REVNP0 IfxEvadc_getChannelResultServiceRequestNodePointer0(Ifx_EVADC_G *evadcG);

/** \brief Gets the channel result service request node pointer 1.
 * \param evadcG pointer to EVADC group registers.
 * \return channel result service request node pointer 1.
 */
IFX_INLINE Ifx_EVADC_G_REVNP1 IfxEvadc_getChannelResultServiceRequestNodePointer1(Ifx_EVADC_G *evadcG);

/** \brief Gets the channel service request node pointer.
 * \param evadcG pointer to EVADC group registers.
 * \return channel service request node pointer.
 */
IFX_INLINE Ifx_EVADC_G_CEVNP0 IfxEvadc_getChannelServiceRequestNodePointer0(Ifx_EVADC_G *evadcG);

/** \brief get the channel service request node pointer (CH8--CH15)
 * \param evadcG pointer to EVADC Group register
 * \return Channel service request node pointer for CH8 to CH15
 */
IFX_INLINE Ifx_EVADC_G_CEVNP1 IfxEvadc_getChannelServiceRequestNodePointer1(Ifx_EVADC_G *evadcG);

/** \brief Selects boundary extension.
 * \param evadcG pointer to EVADC group registers.
 * \param channelIndex group channel id.
 * \param boundaryMode boundary extension mode.
 * \return None
 */
IFX_INLINE void IfxEvadc_setBoundaryMode(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelId channelIndex, IfxEvadc_BoundaryExtension boundaryMode);

/** \brief Sets the channel event service request line.
 * \param evadcG pointer to EVADC group registers.
 * \param channelSrcNr channel event Service Node.
 * \param channel channel number.
 * \return None
 */
IFX_INLINE void IfxEvadc_setChannelEventNodePointer0(Ifx_EVADC_G *evadcG, IfxEvadc_SrcNr channelSrcNr, IfxEvadc_ChannelId channel);

/** \brief set channel event node pointer(channel 8- channel 15)
 * \param evadcG pointer to the EVADC Group register
 * \param channelSrcNr service Request
 * \param channel specifies channel ID
 * \return None
 */
IFX_INLINE void IfxEvadc_setChannelEventNodePointer1(Ifx_EVADC_G *evadcG, IfxEvadc_SrcNr channelSrcNr, IfxEvadc_ChannelId channel);

/** \brief Sets the channel input class.
 * \param evadcG pointer to EVADC group registers.
 * \param channelIndex group channel id.
 * \param inputClass channel input class.
 * \return None
 */
IFX_INLINE void IfxEvadc_setChannelInputClass(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelId channelIndex, IfxEvadc_InputClasses inputClass);

/** \brief Sets the channel event mode.
 * \param evadcG pointer to EVADC group registers.
 * \param channelIndex group channel id.
 * \param limitCheck channel event mode.
 * \return None
 */
IFX_INLINE void IfxEvadc_setChannelLimitCheckMode(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelId channelIndex, IfxEvadc_LimitCheck limitCheck);

/**
 * \param evadcG Pointer to EVADC SFRs of a particular group
 * \param resultRegister
 * \param mode 0b00 Standard data reduction (accumulation)
 * 0b01 Result filtering mode.
 * 0b10 Difference mode
 * \return None
 */
IFX_INLINE void IfxEvadc_setDataModificationMode(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelResult resultRegister, IfxEvadc_DataModificationMode mode);

/**
 * \param evadcG Pointer to EVADC SFRs of a particular group
 * \param resultRegister Group No.
 * \param mode Selects an additional clock cycle for the conversion step of the MSB
 * \return None
 */
IFX_INLINE void IfxEvadc_setDataReductionControlMode(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelResult resultRegister, uint32 mode);

/**
 * \param evadcG Pointer to EVADC SFRs of a particular group
 * \param resultRegister
 * \param mode 0b00 - Separate result register
 * 0b01 - Part of a FIFO structure: copy each new valid result
 * 0b10 - Maximum mode: copy new result if bigger
 * 0b11 - Minimum mode: copy new result if smaller
 * \return None
 */
IFX_INLINE void IfxEvadc_setFifoMode(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelResult resultRegister, IfxEvadc_FifoMode mode);

/** \brief Sets group's lower boundary.
 * \param evadcG pointer to EVADC group registers.
 * \param channelIndex group channel id.
 * \param lowerBoundary group lower boundary.
 * \return None
 */
IFX_INLINE void IfxEvadc_setLowerBoundary(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelId channelIndex, IfxEvadc_BoundarySelection lowerBoundary);

/** \brief Selects the refernce input.
 * \param evadcG pointer to EVADC group registers.
 * \param channelIndex group channel id.
 * \param reference reference input.
 * \return None
 */
IFX_INLINE void IfxEvadc_setReferenceInput(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelId channelIndex, IfxEvadc_ChannelReference reference);

/** \brief Sets result event node pointer 0.
 * \param evadcG pointer to EVADC group registers.
 * \param resultSrcNr channel result event service node.
 * \param resultRegister channel result register.
 * \return None
 */
IFX_INLINE void IfxEvadc_setResultNodeEventPointer0(Ifx_EVADC_G *evadcG, IfxEvadc_SrcNr resultSrcNr, IfxEvadc_ChannelResult resultRegister);

/** \brief Sets result event node pointer 1.
 * \param evadcG pointer to EVADC group registers.
 * \param resultSrcNr channel result event service node.
 * \param resultRegister channel result register.
 * \return None
 */
IFX_INLINE void IfxEvadc_setResultNodeEventPointer1(Ifx_EVADC_G *evadcG, IfxEvadc_SrcNr resultSrcNr, IfxEvadc_ChannelResult resultRegister);

/** \brief Sets result store position.
 * \param evadcG pointer to EVADC group registers.
 * \param channelIndex group channel id.
 * \param rightAlignedStorage result store position.
 * \return None
 */
IFX_INLINE void IfxEvadc_setResultPosition(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelId channelIndex, boolean rightAlignedStorage);

/** \brief Sets channel synchronization request.
 * \param evadcG pointer to EVADC group registers.
 * \param channelIndex group channel id.
 * \param synchonize whether channel synchronize or stand alone operation.
 * \return None
 */
IFX_INLINE void IfxEvadc_setSyncRequest(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelId channelIndex, boolean synchonize);

/** \brief Sets group's upper boundary.
 * \param evadcG pointer to EVADC group registers.
 * \param channelIndex group channel id.
 * \param upperBoundary group upper boundary.
 * \return None
 */
IFX_INLINE void IfxEvadc_setUpperBoundary(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelId channelIndex, IfxEvadc_BoundarySelection upperBoundary);

/** \brief Sets the group result register.
 * \param evadcG pointer to EVADC group registers.
 * \param channelIndex group channel id.
 * \param resultRegister result register for group result storage.
 * \return None
 */
IFX_INLINE void IfxEvadc_storeGroupResult(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelId channelIndex, IfxEvadc_ChannelResult resultRegister);

/** \} */

/** \addtogroup IfxLld_Evadc_Std_Emux
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief get global input class conversion mode
 * \param evadc Pointer to EVADC Module space
 * \param inputClassNum global input class number
 * \return External channel resolution for global input class
 */
IFX_INLINE IfxEvadc_ChannelNoiseReduction IfxEvadc_getEmuxGlobalConversionMode(Ifx_EVADC *evadc, uint8 inputClassNum);

/** \brief get the external conversion mode
 * \param evadcG Pointer to EVADC group register space
 * \param inputClassNum Adc input class number
 * \return Adc input class External channel conversion mode
 */
IFX_INLINE IfxEvadc_ChannelNoiseReduction IfxEvadc_getEmuxGroupConversionMode(Ifx_EVADC_G *evadcG, uint8 inputClassNum);

/** \brief Gets the ADC input class sample time of external channel.
 * \param evadcG Pointer to Register Group space
 * \param inputClassNum ADC input class number
 * \param analogFrequency ADC module analog frequency in Hz.
 * \return ADC input class external channel sample time in sec.
 */
IFX_INLINE float32 IfxEvadc_getEmuxGroupSampleTime(Ifx_EVADC_G *evadcG, uint8 inputClassNum, float32 analogFrequency);

/** \brief set the external conversion mode of Global class
 * \param evadc pointer to EVADC Module space
 * \param inputClassNum Global Input Class Number
 * \param conversionMode External Channel resolution
 * \return None
 */
IFX_INLINE void IfxEvadc_setEmuxGlobalConversionMode(Ifx_EVADC *evadc, uint8 inputClassNum, IfxEvadc_ChannelNoiseReduction conversionMode);

/** \brief Sets the sample time of ADC global class for external channel.
 * \param evadc Pointer to EVADC Module space
 * \param inputClassNum Adc input class number
 * \param analogFrequency ADC analog Frequency in HZ
 * \param sampleTime the requested sample time for input class in sec
 * \return None
 */
IFX_INLINE void IfxEvadc_setEmuxGlobalSampleTime(Ifx_EVADC *evadc, uint8 inputClassNum, float32 analogFrequency, float32 sampleTime);

/** \brief set the external channel conversion mode of ADC input class
 * \param evadcG pointer to EVADC Group Register space
 * \param inputClassNum input class number
 * \param conversionMode input class external channel resolution
 * \return None
 */
IFX_INLINE void IfxEvadc_setEmuxGroupConversionMode(Ifx_EVADC_G *evadcG, uint8 inputClassNum, IfxEvadc_ChannelNoiseReduction conversionMode);

/** \brief Sets the ADC input class sample time for external channel.
 * \param evadcG Pointer to EVADC Group Register Space
 * \param inputClassNum input class number
 * \param analogFrequency ADC analog frequency in Hz.
 * \param sampleTime request sample time in sec for input class.
 * \return None
 */
IFX_INLINE void IfxEvadc_setEmuxGroupSampletime(Ifx_EVADC_G *evadcG, uint8 inputClassNum, float32 analogFrequency, float32 sampleTime);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Configure external multiplexer mode
 * \param evadc pointer to Module space
 * \param evadcG Pointer to EVADC group register space
 * \param mode External Multiplexer mode
 * \param channels Specifies channel Id
 * \param startChannel specifies the external channel value from which conversion to be carried out
 * \param code Output the channel number in binary code/gray code
 * \param sampleTimeControl specifies when to use a sample time for external channel
 * \param channelSelectionStyle External Multiplexer Channel Selection Style
 * \return None
 */
IFX_EXTERN void IfxEvadc_configExternalMultiplexerMode(Ifx_EVADC *evadc, Ifx_EVADC_G *evadcG, IfxEvadc_ExternalMultiplexerMode mode, uint8 channels, IfxEvadc_EmuxSelectValue startChannel, IfxEvadc_EmuxCodingScheme code, IfxEvadc_EmuxSampleTimeControl sampleTimeControl, IfxEvadc_ChannelSelectionStyle channelSelectionStyle);

/** \} */

/** \addtogroup IfxLld_Evadc_Std_FastCompare
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/**
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \param control Action to be taken on the BoundaryFlag(FCM.B.BFL)
 * \return None
 */
IFX_INLINE void IfxEvadc_setFastCompareBoundaryFlagSwControl(Ifx_EVADC_FC *evadcFC, IfxEvadc_BoundaryFlagSwControl control);

/** \brief Sets in FCM.B.BFI whether to use Boundary Flag(FCM.B.BFL) directly or in a inverted format
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \param mode Invert or not to invert the Boundary Flag(BFL)
 * \return None
 */
IFX_INLINE void IfxEvadc_setFastCompareBoundaryFlagInversion(Ifx_EVADC_FC *evadcFC, IfxEvadc_BoundaryFlagInversionControl mode);

/**
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \return None
 */
IFX_INLINE void IfxEvadc_disableFastCompareBoundaryFlag(Ifx_EVADC_FC *evadcFC);

/**
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \return None
 */
IFX_INLINE void IfxEvadc_enableFastCompareBoundaryFlag(Ifx_EVADC_FC *evadcFC);

/** \brief Enable/Disable sample timing synchronization
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \param mode Sample synchronization mode select
 * \return None
 */
IFX_INLINE void IfxEvadc_enableFastCompareSampleSynchronization(Ifx_EVADC_FC *evadcFC, IfxEvadc_SampleSynchronization mode);

/** \brief Setting the Reference Precharge Control for Fast Compare Channels in FCCTRL.B.RPC
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \param duration Reference Precharge Control Mode
 *
 * 0 - Precharge the reference input for 1 clock phase
 * 1 - Precharge the reference input for 1 clock period (2 phases)
 * \return None
 */
IFX_INLINE void IfxEvadc_setFastCompareReferencePrechargeDuration(Ifx_EVADC_FC *evadcFC, IfxEvadc_ReferencePrechargeControl duration);

/** \brief Get boundary flag status.
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \return 0(FALSE) - Passive state : Result has not yet crossed the activation boundary, or selected gate signal is inactive, or this boundary flag is disabled
 * 1(TRUE) - Active state:result has crossed the activation boundary
 */
IFX_INLINE boolean IfxEvadc_getFastCompareBoundaryFlagStatus(Ifx_EVADC_FC *evadcFC);

/** \brief Returns current level of the selected trigger input
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \return Current level of the selected trigger input
 */
IFX_INLINE boolean IfxEvadc_getFastCompareExternalTriggerLevelStatus(Ifx_EVADC_FC *evadcFC);

/** \brief Returns result for Fast Compare operation
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \return Result for Fast Compare operation
 */
IFX_INLINE boolean IfxEvadc_getFastCompareResultStatus(Ifx_EVADC_FC *evadcFC);

/** \brief Indicates availability of a new result in bit FCM.B.FCR
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \return Returns value Indicating availability of a new result in bit FCM.B.FCR
 */
IFX_INLINE boolean IfxEvadc_isFastCompareNewResultAvailable(Ifx_EVADC_FC *evadcFC);

/** \brief Selects behaviour of Boundary Flag(BFL) wih respect to the value of FCR.
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \param mode 0 - Set boundary flag BFL if result FCR = 1 (input above the defined band or compare value), clear if FCR = 0 (input below)
 * 1 - Set boundary flag BFL if result FCR = 0 (input below the defined band or compare value), clear if FCR = 1 (input above)
 * \return None
 */
IFX_INLINE void IfxEvadc_selectFastCompareBoundaryFlagActivationMode(Ifx_EVADC_FC *evadcFC, IfxEvadc_BoundaryFlagActivationMode mode);

/**
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \param nodePointer Bounday Flag Node Pointers
 * \return None
 */
IFX_INLINE void IfxEvadc_selectFastCompareBoundaryFlagNodePointer(Ifx_EVADC_FC *evadcFC, IfxEvadc_BoundaryFlagNodePointer nodePointer);

/** \brief Sets the delay of the analog clock(in clock cycles) after the sync signal.
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \param delay Defines the delay of the analog clock in clocks after the sync signal.
 * \return None
 */
IFX_INLINE void IfxEvadc_setFastCompareAnalogClockSynchronizationDelay(Ifx_EVADC_FC *evadcFC, IfxEvadc_FastCompareAnalogClockSynchronizationDelay delay);

/** \brief Set Analog Converter Control modes
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \param mode Analog Converter Control modes
 * \return None
 */
IFX_INLINE void IfxEvadc_setFastCompareAnalogConverter(Ifx_EVADC_FC *evadcFC, IfxEvadc_FastCompareAnalogConverterControl mode);

/**
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \param mode Analog Input Precharge Enable for Fast Comparisons
 * \return None
 */
IFX_INLINE void IfxEvadc_setFastCompareAnalogInputPrechargeControl(Ifx_EVADC_FC *evadcFC, IfxEvadc_AnalogInputPrechargeControl mode);

/** \brief Defines the frequency of the analog converter clock fADCI (base clock for
 * conversion steps), derived from the peripheral clock: fADCI = fADC / divider
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \param divider Fast Compare Analog Internal Clock Divider factor
 * \return None
 */
IFX_INLINE void IfxEvadc_setFastCompareAnalogInternalClockDivider(Ifx_EVADC_FC *evadcFC, IfxEvadc_ClockDividerFactor divider);

/** \brief Sets the source of the value(s) in bitfield FCREF.
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \param mode Defines the source of the value(s) in bitfield FCREF.
 * \return None
 */
IFX_INLINE void IfxEvadc_setFastCompareAutomaticUpdateMode(Ifx_EVADC_FC *evadcFC, IfxEvadc_FastCompareAutomaticUpdate mode);

/**
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \param mode Mode/criteria for generation of service requests
 * \return None
 */
IFX_INLINE void IfxEvadc_setFastCompareChannelEventMode(Ifx_EVADC_FC *evadcFC, IfxEvadc_ChannelEventMode mode);

/** \brief Set external trigger polarity in FCCTRL.B.XTPOL
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \param mode 0 - Use selected input signal directly
 * 1 - Invert selected input signal
 * \return None
 */
IFX_INLINE void IfxEvadc_setFastCompareExternalTriggerPolarity(Ifx_EVADC_FC *evadcFC, IfxEvadc_ExternalTriggerPolarity mode);

/**
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \param mode Fast Compare Gate Operating Mode
 * \return None
 */
IFX_INLINE void IfxEvadc_setFastCompareGateOperatingMode(Ifx_EVADC_FC *evadcFC, IfxEvadc_GateOperatingMode mode);

/** \brief Ramp direction(up or down) settings
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \param rampDirection Ramp direction(up or down) settings
 * \return None
 */
IFX_INLINE void IfxEvadc_setFastCompareRampDirection(Ifx_EVADC_FC *evadcFC, IfxEvadc_RampDirection rampDirection);

/**
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \param referenceValue The input level is compared to this value.
 * \return None
 */
IFX_INLINE void IfxEvadc_setFastCompareReferenceValue(Ifx_EVADC_FC *evadcFC, uint16 referenceValue);

/** \brief Defines the basic run conditions of the fast compare channel.
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \param mode Run mode of the fast compare channel.
 * \return None
 */
IFX_INLINE void IfxEvadc_setFastCompareRunControl(Ifx_EVADC_FC *evadcFC, IfxEvadc_FastCompareRunControl mode);

/**
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \param rampStartMode Defines the run conditions for the ramp generation
 * \return None
 */
IFX_INLINE void IfxEvadc_setFastCompareRunRampControl(Ifx_EVADC_FC *evadcFC, IfxEvadc_RampRunControl rampStartMode);

/** \brief Setting additional sample time in FCCTRL.B.STCF
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \param additionalCycles Number of additional clock cycles to be added to the minimum sample phase of 2 analog clock cycles
 * \return None
 */
IFX_INLINE void IfxEvadc_setFastCompareSampleTimeControl(Ifx_EVADC_FC *evadcFC, uint8 additionalCycles);

/** \brief Sets when to generate service requests
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \param mode Defines when to generate service requests
 * \return None
 */
IFX_INLINE void IfxEvadc_setFastCompareServiceRequestGeneration(Ifx_EVADC_FC *evadcFC, IfxEvadc_FastCompareServiceRequestGeneration mode);

/** \brief Sets the interval at which fast compare operations are triggered in steps of 16 * 1/fADC
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \param interval Defines the interval at which fast compare operations are triggered in steps of 16 * 1/f_ADC
 * \return None
 */
IFX_INLINE void IfxEvadc_setFastCompareTriggerInterval(Ifx_EVADC_FC *evadcFC, uint8 interval);

/**
 * \param evadcFC Pointer to EVADC register set for Fast Compare operation
 * \param mode External Trigger Operating Mode
 * \return None
 */
IFX_INLINE void IfxEvadc_setFastCompareTriggerOperatingMode(Ifx_EVADC_FC *evadcFC, IfxEvadc_TriggerOperatingMode mode);

/** \} */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/**
 * \param evadcG Pointer to EVADC group register
 * \return analog function configuration value
 */
IFX_INLINE Ifx_EVADC_G_ANCFG IfxEvadc_getAnalogFunctionConfiguration(Ifx_EVADC_G *evadcG);

/** \brief Get the sample time of ADC global class for external channel.
 * \param evadc pointer to EVADC Module space
 * \param inputClassNum Adc input class number
 * \param analogFrequency ADC module analog frequency in Hz.
 * \return ADC input class external channel sample time in sec.
 */
IFX_INLINE float32 IfxEvadc_getEmuxGlobalSampleTime(Ifx_EVADC *evadc, uint8 inputClassNum, float32 analogFrequency);

/** \brief Sets the Emux Interface for a particular group
 * \param evadc Pointer to EVADC Module Space
 * \param emuxInterface specifies the EmuxInterface
 * \param group specifies the group ID
 * \return None
 */
IFX_INLINE void IfxEvadc_setEmuxInterfaceForGroup(Ifx_EVADC *evadc, IfxEvadc_EmuxInterface emuxInterface, IfxEvadc_GroupId group);

/**
 * \param evadcG Pointer to EVADC Group Registers
 * \param group EVADC Group Number
 * \return None
 */
IFX_INLINE void IfxEvadc_disableReferencePrecharge(Ifx_EVADC_G *evadcG, IfxEvadc_GroupId group);

/**
 * \param evadcG Pointer to EVADC Group SFRs
 * \param group EVADC Group No.
 * \return None
 */
IFX_INLINE void IfxEvadc_disableInputBuffer(Ifx_EVADC_G *evadcG, IfxEvadc_GroupId group);

/** \brief Return if the clock for EVADC modules is enabled
 * \param evadc Pointer to EVADC
 * \return Status (TRUE/FALSE)
 */
IFX_INLINE boolean IfxEvadc_isModuleEnabled(Ifx_EVADC *evadc);

/** \brief Enable the startup calibaration phase
 * \param evadc Pointer to EVADC
 * \return None
 */
IFX_INLINE void IfxEvadc_enableStartupCalibration(Ifx_EVADC *evadc);

/** \brief Enable the Queue Gating Mode
 * \param evadcG Pointer to EVADC Group SFRs
 * \param queue Request source Type(Queue 0,Queue 1,Queue 2)
 * \param gatingMode gating mode. High level, Low Level  or Gating disabled.
 * \return None
 */
IFX_INLINE void IfxEvadc_selectQueueGatingMode(Ifx_EVADC_G *evadcG, IfxEvadc_RequestSource queue, IfxEvadc_GatingMode gatingMode);

/** \brief Defines the channel number to be converted
 * \param evadcG Pointer to EVADC Group SFRs
 * \param queue Request source Type(Queue 0,Queue 1,Queue 2)
 * \param channel Channel number
 * \return None
 */
IFX_INLINE void IfxEvadc_setChannelRequest(Ifx_EVADC_G *evadcG, IfxEvadc_RequestSource queue, IfxEvadc_ChannelId channel);

/** \brief Sets the global input class registers, define the sample time and data conversion mode
 * \param evadcG Pointer to EVADC Group SFRs
 * \param inputClassNum Input Class Number
 * \param value Value to be written
 * \return None
 */
IFX_INLINE void IfxEvadc_configureIclass(Ifx_EVADC_G *evadcG, uint8 inputClassNum, uint32 value);

/** \brief Returns the pointer to the result register
 * \param evadcG Pointer to EVADC Group SFRs
 * \param channel Channel Number
 * \return The address to the result register
 */
IFX_INLINE volatile uint32 *IfxEvadc_getResultRegisterAddress(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelId channel);

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE void IfxEvadc_setAnalogClockSynchronizationDelay(Ifx_EVADC_G *evadcG, IfxEvadc_GroupId group, IfxEvadc_AnalogClockSynchronizationDelay delay)
{
    evadcG->ANCFG.B.ACSD = delay;
}


IFX_INLINE uint32 IfxEvadc_calculateSampleTime(float32 analogFrequency, float32 sampleTime)
{
    uint32 ticks = 0;

    ticks = (uint32)(sampleTime * analogFrequency) - 2;

    if (ticks > 31)
    {
        ticks = (ticks / 16) + 15;
    }

    ticks = __minu(ticks, 0xFFu);

    return ticks;
}


IFX_INLINE void IfxEvadc_setCalibrationSampleTime(Ifx_EVADC_G *evadcG, IfxEvadc_GroupId group, IfxEvadc_CalibrationSampleTimeControl mode)
{
    evadcG->ANCFG.B.CALSTC = mode;
}


IFX_INLINE void IfxEvadc_clearAllResultRequests(Ifx_EVADC_G *evadcG)
{
    evadcG->REFCLR.U = 0x0000FFFFu;
}


IFX_INLINE void IfxEvadc_clearChannelRequest(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelId channelId)
{
    evadcG->CEFCLR.U = 1 << channelId;
}


IFX_INLINE void IfxEvadc_clearQueue(Ifx_EVADC_G *evadcG, boolean flushQueue, IfxEvadc_RequestSource sourceType)
{
    switch (sourceType)
    {
    case IfxEvadc_RequestSource_queue0:
        evadcG->Q[0].QMR.B.FLUSH = flushQueue;
        break;
    case IfxEvadc_RequestSource_queue1:
        evadcG->Q[1].QMR.B.FLUSH = flushQueue;
        break;
    case IfxEvadc_RequestSource_queue2:
        evadcG->Q[2].QMR.B.FLUSH = flushQueue;
        break;
    }
}


IFX_INLINE void IfxEvadc_configureWaitForReadMode(Ifx_EVADC_G *group, uint32 resultIdx, boolean enabled)
{
    group->RCR[resultIdx].B.WFR = enabled;
}


IFX_INLINE void IfxEvadc_configureWaitForReadModeForGlobalResultRegister(Ifx_EVADC *evadc, boolean enabled)
{
    evadc->GLOB.RCR.B.WFR = enabled;
}


IFX_INLINE void IfxEvadc_setFastCompareBoundaryFlagSwControl(Ifx_EVADC_FC *evadcFC, IfxEvadc_BoundaryFlagSwControl control)
{
    evadcFC->FCBFL.B.BFS = control;
}


IFX_INLINE void IfxEvadc_setFastCompareBoundaryFlagInversion(Ifx_EVADC_FC *evadcFC, IfxEvadc_BoundaryFlagInversionControl mode)
{
    evadcFC->FCBFL.B.BFI = mode;
}


IFX_INLINE void IfxEvadc_disableFastCompareBoundaryFlag(Ifx_EVADC_FC *evadcFC)
{
    evadcFC->FCBFL.B.BFM = 0;
}


IFX_INLINE void IfxEvadc_disableModule(Ifx_EVADC *evadc)
{
    uint16 passwd = IfxScuWdt_getCpuWatchdogPassword();

    IfxScuWdt_clearCpuEndinit(passwd);
    evadc->CLC.B.DISR = 1;
    IfxScuWdt_setCpuEndinit(passwd);
}


IFX_INLINE void IfxEvadc_disablePostCalibration(Ifx_EVADC_G *evadcG, IfxEvadc_GroupId group, boolean enabled)
{
    evadcG->ANCFG.B.DPCAL = enabled;
}


IFX_INLINE void IfxEvadc_enableFastCompareBoundaryFlag(Ifx_EVADC_FC *evadcFC)
{
    evadcFC->FCBFL.B.BFM = 1;
}


IFX_INLINE void IfxEvadc_enableFastCompareSampleSynchronization(Ifx_EVADC_FC *evadcFC, IfxEvadc_SampleSynchronization mode)
{
    Ifx_EVADC_FC_FCM tempFCM;
    tempFCM.U       = evadcFC->FCM.U;
    tempFCM.B.FCMWC = 1;  /*Enabling write permission to FCM.B.SSE*/
    tempFCM.B.SSE   = mode;
    evadcFC->FCM.U  = tempFCM.U;
}


IFX_INLINE void IfxEvadc_enableFifoMode(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelResult resultRegister, IfxEvadc_FifoMode fifoMode)
{
    evadcG->RCR[resultRegister].B.FEN = fifoMode;
}


IFX_INLINE void IfxEvadc_enableModule(Ifx_EVADC *evadc)
{
    uint16 passwd = IfxScuWdt_getCpuWatchdogPassword();

    IfxScuWdt_clearCpuEndinit(passwd);
    evadc->CLC.U = 0x00000000;
    IfxScuWdt_setCpuEndinit(passwd);
}


IFX_INLINE void IfxEvadc_enableServiceRequest(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelResult resultRegister)
{
    evadcG->RCR[resultRegister].B.SRGEN = 1;
}


IFX_INLINE void IfxEvadc_disableServiceRequest(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelResult resultRegister)
{
    evadcG->RCR[resultRegister].B.SRGEN = 0;
}


IFX_INLINE void IfxEvadc_setFastCompareReferencePrechargeDuration(Ifx_EVADC_FC *evadcFC, IfxEvadc_ReferencePrechargeControl duration)
{
    Ifx_EVADC_FC_FCCTRL tempFCCTRL;
    tempFCCTRL.U      = evadcFC->FCCTRL.U;
    tempFCCTRL.B.CPWC = 1; /*Enabling write permission to FCCTRL.B.RPC*/
    tempFCCTRL.B.RPE  = duration;
    evadcFC->FCCTRL.U = tempFCCTRL.U;
}


IFX_INLINE float32 IfxEvadc_getAdcAnalogFrequency(Ifx_EVADC_G *group)
{
#ifdef IFX_CFG_AURIX_SCUCCU_USED
    return IfxScuCcu_getSpbFrequency() / (1U + group->ANCFG.B.DIVA);
#else
    return IfxScuCcu_getAdcFrequency() / (1U + group->ANCFG.B.DIVA);
#endif
}


IFX_INLINE uint8 IfxEvadc_getAdcCalibrationActiveState(Ifx_EVADC *evadc, uint8 adcCalGroupNum)
{
    uint8 status = 0;
    status = evadc->G[adcCalGroupNum].ARBCFG.B.CAL;
    return status;
}


IFX_INLINE Ifx_EVADC_G_ANCFG IfxEvadc_getAnalogFunctionConfiguration(Ifx_EVADC_G *evadcG)
{
    Ifx_EVADC_G_ANCFG analogCfg;
    analogCfg.U = evadcG->ANCFG.U;
    return analogCfg;
}


IFX_INLINE Ifx_EVADC_G_CHCTR IfxEvadc_getChannelControlConfig(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelId channelIndex)
{
    Ifx_EVADC_G_CHCTR tempChctr;
    tempChctr.U = evadcG->CHCTR[channelIndex].U;
    return tempChctr;
}


IFX_INLINE IfxEvadc_InputClasses IfxEvadc_getChannelInputclass(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelId channelIndex)
{
    return (IfxEvadc_InputClasses)evadcG->CHCTR[channelIndex].B.ICLSEL;
}


IFX_INLINE Ifx_EVADC_G_REVNP0 IfxEvadc_getChannelResultServiceRequestNodePointer0(Ifx_EVADC_G *evadcG)
{
    Ifx_EVADC_G_REVNP0 resultServiceRequestNodePtr0;
    resultServiceRequestNodePtr0.U = evadcG->REVNP0.U;
    return resultServiceRequestNodePtr0;
}


IFX_INLINE Ifx_EVADC_G_REVNP1 IfxEvadc_getChannelResultServiceRequestNodePointer1(Ifx_EVADC_G *evadcG)
{
    Ifx_EVADC_G_REVNP1 resultServiceRequestNodePtr1;
    resultServiceRequestNodePtr1.U = evadcG->REVNP1.U;
    return resultServiceRequestNodePtr1;
}


IFX_INLINE Ifx_EVADC_G_CEVNP0 IfxEvadc_getChannelServiceRequestNodePointer0(Ifx_EVADC_G *evadcG)
{
    Ifx_EVADC_G_CEVNP0 serviceRequestNodePtr;
    serviceRequestNodePtr.U = evadcG->CEVNP0.U;
    return serviceRequestNodePtr;
}


IFX_INLINE Ifx_EVADC_G_CEVNP1 IfxEvadc_getChannelServiceRequestNodePointer1(Ifx_EVADC_G *evadcG)
{
    Ifx_EVADC_G_CEVNP1 serviceRequestNodePtr;
    serviceRequestNodePtr.U = evadcG->CEVNP1.U;
    return serviceRequestNodePtr;
}


IFX_INLINE IfxEvadc_ChannelNoiseReduction IfxEvadc_getEmuxGlobalConversionMode(Ifx_EVADC *evadc, uint8 inputClassNum)
{
    return (IfxEvadc_ChannelNoiseReduction)evadc->GLOB.ICLASS[inputClassNum].B.CME;
}


IFX_INLINE float32 IfxEvadc_getEmuxGlobalSampleTime(Ifx_EVADC *evadc, uint8 inputClassNum, float32 analogFrequency)
{
    return (float32)(IFXEVADC_SAMPLETIME_MIN + evadc->GLOB.ICLASS[inputClassNum].B.STCE) / analogFrequency;
}


IFX_INLINE IfxEvadc_ChannelNoiseReduction IfxEvadc_getEmuxGroupConversionMode(Ifx_EVADC_G *evadcG, uint8 inputClassNum)
{
    return (IfxEvadc_ChannelNoiseReduction)evadcG->ICLASS[inputClassNum].B.CME;
}


IFX_INLINE float32 IfxEvadc_getEmuxGroupSampleTime(Ifx_EVADC_G *evadcG, uint8 inputClassNum, float32 analogFrequency)
{
    return (float32)(IFXEVADC_SAMPLETIME_MIN + evadcG->ICLASS[inputClassNum].B.STCE) / analogFrequency;
}


IFX_INLINE boolean IfxEvadc_getFastCompareBoundaryFlagStatus(Ifx_EVADC_FC *evadcFC)
{
    boolean status;

    if (evadcFC->FCBFL.B.BFL == 1)
    {
        status = TRUE;
    }
    else
    {
        status = FALSE;
    }

    return status;
}


IFX_INLINE boolean IfxEvadc_getFastCompareExternalTriggerLevelStatus(Ifx_EVADC_FC *evadcFC)
{
    boolean extTrigLevel;

    if (evadcFC->FCCTRL.B.XTLVL == 1)
    {
        extTrigLevel = TRUE;
    }
    else
    {
        extTrigLevel = FALSE;
    }

    return extTrigLevel;
}


IFX_INLINE boolean IfxEvadc_getFastCompareResultStatus(Ifx_EVADC_FC *evadcFC)
{
    boolean result;

    if (evadcFC->FCBFL.B.FCR == 1)
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }

    return result;
}


IFX_INLINE Ifx_EVADC_GLOBCFG IfxEvadc_getGlobalConfigValue(Ifx_EVADC *evadc)
{
    Ifx_EVADC_GLOBCFG globCfg;
    globCfg.U = evadc->GLOBCFG.U;
    return globCfg;
}


IFX_INLINE IfxEvadc_ChannelNoiseReduction IfxEvadc_getGlobalConversionMode(Ifx_EVADC *evadc, uint8 inputClassNum)
{
    return (IfxEvadc_ChannelNoiseReduction)evadc->GLOB.ICLASS[inputClassNum].B.CMS;
}


IFX_INLINE Ifx_EVADC_GLOB_RES IfxEvadc_getGlobalResult(Ifx_EVADC *evadc)
{
    Ifx_EVADC_GLOB_RES tmpGlobalResult;

    tmpGlobalResult.U = evadc->GLOB.RES.U;

    return tmpGlobalResult;
}


IFX_INLINE float32 IfxEvadc_getGlobalSampleTime(Ifx_EVADC *evadc, uint8 inputClassNum, float32 analogFrequency)
{
    uint32 sampleTime = evadc->GLOB.ICLASS[inputClassNum].B.STCS;

    if (sampleTime > 16)
    {
        sampleTime = (sampleTime - 15) * 16;
    }

    return (float32)(IFXEVADC_SAMPLETIME_MIN + sampleTime) / analogFrequency;
}


IFX_INLINE IfxEvadc_ChannelNoiseReduction IfxEvadc_getGroupConversionMode(Ifx_EVADC_G *evadcG, uint8 inputClassNum)
{
    return (IfxEvadc_ChannelNoiseReduction)evadcG->ICLASS[inputClassNum].B.CMS;
}


IFX_INLINE void IfxEvadc_getGroupResult(Ifx_EVADC_G *group, Ifx_EVADC_G_RES *results, uint32 resultOffset, uint32 numResults)
{
    uint32 idx = 0;

    for (idx = 0; idx < numResults; idx++)
    {
        results[idx].U = group->RES[resultOffset + idx].U;
    }
}


IFX_INLINE float32 IfxEvadc_getGroupSampleTime(Ifx_EVADC_G *evadcG, uint8 inputClassNum, float32 analogFrequency)
{
    uint32 sampleTime = evadcG->ICLASS[inputClassNum].B.STCS;

    if (sampleTime > 16)
    {
        sampleTime = (sampleTime - 15) * 16;
    }

    return (float32)(IFXEVADC_SAMPLETIME_MIN + sampleTime) / analogFrequency;
}


IFX_INLINE uint8 IfxEvadc_getMasterIndex(Ifx_EVADC_G *evadcG)
{
    uint8 masterIndex = 0;
    masterIndex = evadcG->SYNCTR.B.STSEL;
    return masterIndex;
}


IFX_INLINE Ifx_EVADC_G_RES IfxEvadc_getResult(Ifx_EVADC_G *group, uint32 resultIdx)
{
    Ifx_EVADC_G_RES tmpResult;
    tmpResult.U = group->RES[resultIdx].U;

    return tmpResult;
}


IFX_INLINE volatile Ifx_SRC_SRCR *IfxEvadc_getSrcAddress(IfxEvadc_GroupId group, IfxEvadc_SrcNr index)
{
    if ((group >= IfxEvadc_GroupId_12) && (group < IfxEvadc_GroupId_global0)) //Condition for Fast Compare groups
    {
        IFX_ASSERT(IFX_VERBOSE_LEVEL_WARNING, index == 0);                    /* For Fast Compare group, only SR0 is available.So only index = 0 is valid.So asserting warning in case index!=0 */
    }

/*This is being done because the enum IfxEvadc_GroupId has count equal to group number and this count may not be continuous for different derivatives whereas IfxEvadc_cfg_srcAddresses[] has continuous positioning according to number of groups of each type available*/
    if ((group > 7) && (group < 12))
    {
        group = (IfxEvadc_GroupId)(group - (8 - IFXEVADC_NUM_ADC_PRIMARY_GROUPS));
    }
    else if ((group > 11) && (group < 20))
    {
        group = (IfxEvadc_GroupId)(group - (12 - IFXEVADC_NUM_ADC_PRIMARY_GROUPS - IFXEVADC_NUM_ADC_SECONDARY_GROUPS));
    }
    else if ((group == 20) || (group == 21))
    {
        group = (IfxEvadc_GroupId)(group - (20 - IFXEVADC_NUM_ADC_PRIMARY_GROUPS - IFXEVADC_NUM_ADC_SECONDARY_GROUPS - IFXEVADC_NUM_FAST_COMPARE_CHANNELS));
    }

    volatile Ifx_SRC_SRCR *base = IfxEvadc_cfg_srcAddresses[(group << 2) + index];

    return base;
}


IFX_INLINE boolean IfxEvadc_getStartupCalibrationStatus(Ifx_EVADC *evadc)
{
    return (boolean)evadc->GLOBCFG.B.SUCAL;
}


IFX_INLINE void IfxEvadc_selectIdlePrechargeLevel(Ifx_EVADC_G *evadcG, IfxEvadc_GroupId group, IfxEvadc_IdlePrecharge mode)
{
    evadcG->ANCFG.B.IPE = mode;
}


IFX_INLINE void IfxEvadc_initEmuxPin(const IfxEvadc_Emux_Out *emux, IfxPort_OutputMode outputMode, IfxPort_PadDriver padDriver)
{
    IfxPort_setPinModeOutput(emux->pin.port, emux->pin.pinIndex, outputMode, emux->select);
    IfxPort_setPinPadDriver(emux->pin.port, emux->pin.pinIndex, padDriver);
}


IFX_INLINE void IfxEvadc_initGxBflPin(const IfxEvadc_GxBfl_Out *gxBfl, IfxPort_OutputMode outputMode, IfxPort_PadDriver padDriver)
{
    IfxPort_setPinModeOutput(gxBfl->pin.port, gxBfl->pin.pinIndex, outputMode, gxBfl->select);
    IfxPort_setPinPadDriver(gxBfl->pin.port, gxBfl->pin.pinIndex, padDriver);
}


IFX_INLINE void IfxEvadc_initialiseAdcConverterClock(Ifx_EVADC *evadc, Ifx_EVADC_G *evadcG, IfxEvadc_ClockDividerFactor converterClockDivider)
{
    evadcG->ANCFG.B.DIVA = converterClockDivider;
}


IFX_INLINE void IfxEvadc_initiateStartupCalibration(Ifx_EVADC *evadc)
{
    evadc->GLOBCFG.B.SUCAL = 1;
}


IFX_INLINE void IfxEvadc_enableInputBuffer(Ifx_EVADC_G *evadcG, IfxEvadc_GroupId group)
{
    evadcG->ANCFG.B.BE = 1;
}


IFX_INLINE boolean IfxEvadc_isFastCompareNewResultAvailable(Ifx_EVADC_FC *evadcFC)
{
    boolean validFlag;

    if (evadcFC->FCBFL.B.VF == 1)
    {
        validFlag = TRUE;
    }
    else
    {
        validFlag = FALSE;
    }

    return validFlag;
}


IFX_INLINE boolean IfxEvadc_isModuleSuspended(Ifx_EVADC *evadc)
{
    Ifx_EVADC_OCS ocs;

    // read the status
    ocs.U = evadc->OCS.U;

    // return the status
    return ocs.B.SUSSTA;
}


IFX_INLINE void IfxEvadc_setReferencePrechargeDuration(Ifx_EVADC_G *evadcG, IfxEvadc_GroupId group, IfxEvadc_ReferencePrechargeControl duration)
{
    evadcG->ANCFG.B.RPC = duration;
}


IFX_INLINE void IfxEvadc_enableReferencePrecharge(Ifx_EVADC_G *evadcG, IfxEvadc_GroupId group)
{
    evadcG->ANCFG.B.RPE = 1;
}


IFX_INLINE void IfxEvadc_resetGroup(Ifx_EVADC_G *evadcG)
{
    evadcG->ARBCFG.B.ANONC = IfxEvadc_AnalogConverterMode_off;   /* turn off group */
}


IFX_INLINE void IfxEvadc_setSampleSynchronizationMode(Ifx_EVADC_G *evadcG, IfxEvadc_GroupId group, IfxEvadc_SampleSynchronization mode)
{
    evadcG->ANCFG.B.SSE = mode;
}


IFX_INLINE void IfxEvadc_selectDoubleClockForMSBConversion(Ifx_EVADC_G *evadcG, IfxEvadc_GroupId group, IfxEvadc_MsbConversionTime mode)
{
    evadcG->ANCFG.B.DCMSB = mode;
}


IFX_INLINE void IfxEvadc_selectFastCompareBoundaryFlagActivationMode(Ifx_EVADC_FC *evadcFC, IfxEvadc_BoundaryFlagActivationMode mode)
{
    evadcFC->FCBFL.B.BFA = mode;
}


IFX_INLINE void IfxEvadc_selectFastCompareBoundaryFlagNodePointer(Ifx_EVADC_FC *evadcFC, IfxEvadc_BoundaryFlagNodePointer nodePointer)
{
    evadcFC->FCBFL.B.BFLNP = nodePointer;
}


IFX_INLINE void IfxEvadc_setAnalogConvertControl(Ifx_EVADC_G *evadcG, IfxEvadc_AnalogConverterMode analogConverterMode)
{
    evadcG->ARBCFG.B.ANONC = analogConverterMode;
}


IFX_INLINE void IfxEvadc_setAnalogInputPrechargeControlEMUX(Ifx_EVADC_G *evadcG, uint8 inputClassNum, IfxEvadc_AnalogInputPrechargeControl mode)
{
    evadcG->ICLASS[inputClassNum].B.AIPE = mode;
}


IFX_INLINE void IfxEvadc_setAnalogInputPrechargeControlStandard(Ifx_EVADC_G *evadcG, uint8 inputClassNum, IfxEvadc_AnalogInputPrechargeControl mode)
{
    evadcG->ICLASS[inputClassNum].B.AIPS = mode;
}


IFX_INLINE void IfxEvadc_setBoundaryMode(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelId channelIndex, IfxEvadc_BoundaryExtension boundaryMode)
{
    evadcG->CHCTR[channelIndex].B.BNDSELX = boundaryMode;
}


IFX_INLINE void IfxEvadc_setChannelEventNodePointer0(Ifx_EVADC_G *evadcG, IfxEvadc_SrcNr channelSrcNr, IfxEvadc_ChannelId channel)
{
    evadcG->CEVNP0.U &= ~(IFX_EVADC_G_CEVNP0_CEV0NP_MSK << (channel * 4));
    evadcG->CEVNP0.U |= (channelSrcNr << (channel * 4));
}


IFX_INLINE void IfxEvadc_setChannelEventNodePointer1(Ifx_EVADC_G *evadcG, IfxEvadc_SrcNr channelSrcNr, IfxEvadc_ChannelId channel)
{
    evadcG->CEVNP1.U &= ~(IFX_EVADC_G_CEVNP0_CEV1NP_MSK << (channel * 4));
    evadcG->CEVNP1.U |= (channelSrcNr << (channel * 4));
}


IFX_INLINE void IfxEvadc_setChannelInputClass(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelId channelIndex, IfxEvadc_InputClasses inputClass)
{
    evadcG->CHCTR[channelIndex].B.ICLSEL = inputClass;
}


IFX_INLINE void IfxEvadc_setChannelLimitCheckMode(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelId channelIndex, IfxEvadc_LimitCheck limitCheck)
{
    evadcG->CHCTR[channelIndex].B.CHEVMODE = limitCheck;
}


IFX_INLINE void IfxEvadc_setDataModificationMode(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelResult resultRegister, IfxEvadc_DataModificationMode mode)
{
    evadcG->RCR[resultRegister].B.DMM = mode;
}


IFX_INLINE void IfxEvadc_setDataReductionControlMode(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelResult resultRegister, uint32 mode)
{
    if (mode < IFX_EVADC_GLOB_RCR_DRCTR_LEN)
    {
        evadcG->RCR[resultRegister].B.DRCTR = mode;
    }

    else
    {
        IFX_ASSERT(IFX_VERBOSE_LEVEL_WARNING, mode < IFX_EVADC_GLOB_RCR_DRCTR_LEN); /*To indicate exceeding of the range of values supported by DRCTR*/
    }
}


IFX_INLINE void IfxEvadc_setEmuxGlobalConversionMode(Ifx_EVADC *evadc, uint8 inputClassNum, IfxEvadc_ChannelNoiseReduction conversionMode)
{
    evadc->GLOB.ICLASS[inputClassNum].B.CME = conversionMode;
}


IFX_INLINE void IfxEvadc_setEmuxGlobalSampleTime(Ifx_EVADC *evadc, uint8 inputClassNum, float32 analogFrequency, float32 sampleTime)
{
    evadc->GLOB.ICLASS[inputClassNum].B.STCE = IfxEvadc_calculateSampleTime(analogFrequency, sampleTime);
}


IFX_INLINE void IfxEvadc_setEmuxGroupConversionMode(Ifx_EVADC_G *evadcG, uint8 inputClassNum, IfxEvadc_ChannelNoiseReduction conversionMode)
{
    evadcG->ICLASS[inputClassNum].B.CME = conversionMode;
}


IFX_INLINE void IfxEvadc_setEmuxGroupSampletime(Ifx_EVADC_G *evadcG, uint8 inputClassNum, float32 analogFrequency, float32 sampleTime)
{
    evadcG->ICLASS[inputClassNum].B.STCE = IfxEvadc_calculateSampleTime(analogFrequency, sampleTime);
}


IFX_INLINE void IfxEvadc_setEmuxInterfaceForGroup(Ifx_EVADC *evadc, IfxEvadc_EmuxInterface emuxInterface, IfxEvadc_GroupId group)
{
    if (emuxInterface == IfxEvadc_EmuxInterface_0)
    {
        evadc->EMUXSEL.B.EMUXGRP0 = group;
    }
    else
    {
        evadc->EMUXSEL.B.EMUXGRP1 = group;
    }
}


IFX_INLINE void IfxEvadc_setFastCompareAnalogClockSynchronizationDelay(Ifx_EVADC_FC *evadcFC, IfxEvadc_FastCompareAnalogClockSynchronizationDelay delay)
{
    Ifx_EVADC_FC_FCM tempFCM;
    tempFCM.U       = evadcFC->FCM.U;
    tempFCM.B.FCMWC = 1;  /*Enabling write permission to FCM.B.ACSD*/
    tempFCM.B.ACSD  = delay;
    evadcFC->FCM.U  = tempFCM.U;
}


IFX_INLINE void IfxEvadc_setFastCompareAnalogConverter(Ifx_EVADC_FC *evadcFC, IfxEvadc_FastCompareAnalogConverterControl mode)
{
    Ifx_EVADC_FC_FCM tempFCM;
    tempFCM.U       = evadcFC->FCM.U;
    tempFCM.B.FCMWC = 1;  /*Enabling write permission to FCM.B.ANON*/
    tempFCM.B.ANON  = mode;
    evadcFC->FCM.U  = tempFCM.U;
}


IFX_INLINE void IfxEvadc_setFastCompareAnalogInputPrechargeControl(Ifx_EVADC_FC *evadcFC, IfxEvadc_AnalogInputPrechargeControl mode)
{
    Ifx_EVADC_FC_FCCTRL tempFCCTRL;
    tempFCCTRL.U      = evadcFC->FCCTRL.U;
    tempFCCTRL.B.CPWC = 1; /*Enabling write permission to FCCTRL.B.AIPF*/
    tempFCCTRL.B.AIPF = mode;
    evadcFC->FCCTRL.U = tempFCCTRL.U;
}


IFX_INLINE void IfxEvadc_setFastCompareAnalogInternalClockDivider(Ifx_EVADC_FC *evadcFC, IfxEvadc_ClockDividerFactor divider)
{
    Ifx_EVADC_FC_FCCTRL tempFCCTRL;
    tempFCCTRL.U      = evadcFC->FCCTRL.U;
    tempFCCTRL.B.CPWC = 1; /*Enabling write permission to FCCTRL.B.DIVA*/
    tempFCCTRL.B.DIVA = divider;
    evadcFC->FCCTRL.U = tempFCCTRL.U;
}


IFX_INLINE void IfxEvadc_setFastCompareAutomaticUpdateMode(Ifx_EVADC_FC *evadcFC, IfxEvadc_FastCompareAutomaticUpdate mode)
{
    Ifx_EVADC_FC_FCM tempFCM;
    tempFCM.U       = evadcFC->FCM.U;
    tempFCM.B.FCMWC = 1;  /*Enabling write permission to FCM.B.AUE*/
    tempFCM.B.AUE   = mode;
    evadcFC->FCM.U  = tempFCM.U;
}


IFX_INLINE void IfxEvadc_setFastCompareChannelEventMode(Ifx_EVADC_FC *evadcFC, IfxEvadc_ChannelEventMode mode)
{
    Ifx_EVADC_FC_FCCTRL tempFCCTRL;
    tempFCCTRL.U          = evadcFC->FCCTRL.U;
    tempFCCTRL.B.CPWC     = 1; /*Enabling write permission to FCCTRL.B.CHEVMODE*/
    tempFCCTRL.B.CHEVMODE = mode;
    evadcFC->FCCTRL.U     = tempFCCTRL.U;
}


IFX_INLINE void IfxEvadc_setFastCompareExternalTriggerPolarity(Ifx_EVADC_FC *evadcFC, IfxEvadc_ExternalTriggerPolarity mode)
{
    Ifx_EVADC_FC_FCCTRL tempFCCTRL;
    tempFCCTRL.U       = evadcFC->FCCTRL.U;
    tempFCCTRL.B.XTWC  = 1; /*Enabling write permission to FCCTRL.B.XTPOL*/
    tempFCCTRL.B.XTPOL = mode;
    evadcFC->FCCTRL.U  = tempFCCTRL.U;
}


IFX_INLINE void IfxEvadc_setFastCompareGateOperatingMode(Ifx_EVADC_FC *evadcFC, IfxEvadc_GateOperatingMode mode)
{
    Ifx_EVADC_FC_FCCTRL tempFCCTRL;
    tempFCCTRL.U        = evadcFC->FCCTRL.U;
    tempFCCTRL.B.XTWC   = 1; /*Enabling write permission to FCCTRL.B.GTMODE*/
    tempFCCTRL.B.GTMODE = mode;
    evadcFC->FCCTRL.U   = tempFCCTRL.U;
}


IFX_INLINE void IfxEvadc_setFastCompareRampDirection(Ifx_EVADC_FC *evadcFC, IfxEvadc_RampDirection rampDirection)
{
    Ifx_EVADC_FC_FCM tempFCM;
    tempFCM.U        = evadcFC->FCM.U;
    tempFCM.B.FCMWC  = 1; /*Enabling write permission to FCM.B.FCRDIR*/
    tempFCM.B.FCRDIR = rampDirection;
    evadcFC->FCM.U   = tempFCM.U;
}


IFX_INLINE void IfxEvadc_setFastCompareReferenceValue(Ifx_EVADC_FC *evadcFC, uint16 referenceValue)
{
    if (evadcFC->FCM.B.AUE == IfxEvadc_FastCompareAutomaticUpdate_sw)
    {
        evadcFC->FCM.B.FCREF = referenceValue;
    }
    else
    {
        IFX_ASSERT(IFX_VERBOSE_LEVEL_WARNING, evadcFC->FCM.B.AUE == IfxEvadc_FastCompareAutomaticUpdate_sw); /* Writing into FCM.B.FCREF requires FCM.B.AUE = 0 */
    }
}


IFX_INLINE void IfxEvadc_setFastCompareRunControl(Ifx_EVADC_FC *evadcFC, IfxEvadc_FastCompareRunControl mode)
{
    Ifx_EVADC_FC_FCM tempFCM;
    tempFCM.U         = evadcFC->FCM.U;
    tempFCM.B.FCMWC   = 1; /*Enabling write permission to FCM.B.RUNCOMP*/
    tempFCM.B.RUNCOMP = mode;
    evadcFC->FCM.U    = tempFCM.U;
}


IFX_INLINE void IfxEvadc_setFastCompareRunRampControl(Ifx_EVADC_FC *evadcFC, IfxEvadc_RampRunControl rampStartMode)
{
    Ifx_EVADC_FC_FCM tempFCM;
    tempFCM.U         = evadcFC->FCM.U;
    tempFCM.B.FCMWC   = 1; /*Enabling write permission to FCM.B.RUNRAMP*/
    tempFCM.B.RUNRAMP = rampStartMode;
    evadcFC->FCM.U    = tempFCM.U;
}


IFX_INLINE void IfxEvadc_setFastCompareSampleTimeControl(Ifx_EVADC_FC *evadcFC, uint8 additionalCycles)
{
    uint32 stcf = 0;

    if (additionalCycles > 15)
    {
/*
 *
 * Allowed values for additional clock cycles are as follows:
 *
 * Additional Clock Cycles = FCCTRL.B.STCF ;  STCF < 16
 * Additional Clock Cycles = (FCCTRL.B.STCF - 15) * 16; STCF>=16
 */

        IFX_ASSERT(IFX_VERBOSE_LEVEL_WARNING, (additionalCycles % 16 == 0));
    }

    if (additionalCycles > 15)
    {
        stcf = (additionalCycles >> 4) + 15;
    }
    else
    {
        stcf = additionalCycles;
    }

    Ifx_EVADC_FC_FCCTRL tempFCCTRL;
    tempFCCTRL.U      = evadcFC->FCCTRL.U;
    tempFCCTRL.B.CPWC = 1; /*Enabling write permission to FCCTRL.B.STFC*/
    tempFCCTRL.B.STCF = stcf;
    evadcFC->FCCTRL.U = tempFCCTRL.U;
}


IFX_INLINE void IfxEvadc_setFastCompareServiceRequestGeneration(Ifx_EVADC_FC *evadcFC, IfxEvadc_FastCompareServiceRequestGeneration mode)
{
    Ifx_EVADC_FC_FCM tempFCM;
    tempFCM.U       = evadcFC->FCM.U;
    tempFCM.B.FCMWC = 1;  /*Enabling write permission to FCM.B.SRG*/
    tempFCM.B.SRG   = mode;
    evadcFC->FCM.U  = tempFCM.U;
}


IFX_INLINE void IfxEvadc_setFastCompareTriggerInterval(Ifx_EVADC_FC *evadcFC, uint8 interval)
{
    Ifx_EVADC_FC_FCM tempFCM;
    tempFCM.U        = evadcFC->FCM.U;
    tempFCM.B.FCMWC  = 1; /*Enabling write permission to FCM.B.FCTRIV*/
    tempFCM.B.FCTRIV = interval;
    evadcFC->FCM.U   = tempFCM.U;
}


IFX_INLINE void IfxEvadc_setFastCompareTriggerOperatingMode(Ifx_EVADC_FC *evadcFC, IfxEvadc_TriggerOperatingMode mode)
{
    Ifx_EVADC_FC_FCCTRL tempFCCTRL;
    tempFCCTRL.U        = evadcFC->FCCTRL.U;
    tempFCCTRL.B.XTWC   = 1; /*Enabling write permission to FCCTRL.B.XTMODE*/
    tempFCCTRL.B.XTMODE = mode;
    evadcFC->FCCTRL.U   = tempFCCTRL.U;
}


IFX_INLINE void IfxEvadc_setFifoMode(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelResult resultRegister, IfxEvadc_FifoMode mode)
{
    evadcG->RCR[resultRegister].B.FEN = mode;
}


IFX_INLINE void IfxEvadc_setGlobalConversionMode(Ifx_EVADC *evadc, uint8 InputClassNum, IfxEvadc_ChannelNoiseReduction conversionMode)
{
    evadc->GLOB.ICLASS[InputClassNum].B.CMS = conversionMode;
}


IFX_INLINE void IfxEvadc_setGlobalSampleTime(Ifx_EVADC *evadc, uint8 groupInputClassNum, float32 analogFrequency, float32 sampleTime)
{
    evadc->GLOB.ICLASS[groupInputClassNum].B.STCS = IfxEvadc_calculateSampleTime(analogFrequency, sampleTime);
}


IFX_INLINE void IfxEvadc_setGroupConversionMode(Ifx_EVADC_G *evadcG, uint8 inputClassNum, IfxEvadc_ChannelNoiseReduction conversionMode)
{
    evadcG->ICLASS[inputClassNum].B.CMS = conversionMode;
}


IFX_INLINE void IfxEvadc_setGroupConversionModeEMUX(Ifx_EVADC_G *evadcG, uint8 inputClassNum, IfxEvadc_ChannelNoiseReductionEMUX conversionMode)
{
    evadcG->ICLASS[inputClassNum].B.CME = conversionMode;
}


IFX_INLINE void IfxEvadc_setGroupSampleTime(Ifx_EVADC_G *evadcG, uint8 inputClassNum, float32 analogFrequency, float32 sampleTime)
{
    evadcG->ICLASS[inputClassNum].B.STCS = IfxEvadc_calculateSampleTime(analogFrequency, sampleTime);
}


IFX_INLINE void IfxEvadc_setGroupSampleTimeEMUX(Ifx_EVADC_G *evadcG, uint8 inputClassNum, float32 analogFrequency, float32 sampleTime)
{
    evadcG->ICLASS[inputClassNum].B.STCE = IfxEvadc_calculateSampleTime(analogFrequency, sampleTime);
}


IFX_INLINE void IfxEvadc_setLowerBoundary(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelId channelIndex, IfxEvadc_BoundarySelection lowerBoundary)
{
    evadcG->CHCTR[channelIndex].B.BNDSELL = lowerBoundary;
}


IFX_INLINE void IfxEvadc_setMasterIndex(Ifx_EVADC_G *evadcG, uint8 masterIndex)
{
    evadcG->SYNCTR.B.STSEL = masterIndex;
    evadcG->SYNCTR.U      |= (0x00000008U << masterIndex);
}


IFX_INLINE void IfxEvadc_setReferenceInput(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelId channelIndex, IfxEvadc_ChannelReference reference)
{
    evadcG->CHCTR[channelIndex].B.REFSEL = reference;
}


IFX_INLINE void IfxEvadc_setResultNodeEventPointer0(Ifx_EVADC_G *evadcG, IfxEvadc_SrcNr resultSrcNr, IfxEvadc_ChannelResult resultRegister)
{
    evadcG->REVNP0.U &= ~(IFX_EVADC_GLOB_EVNP_REV0NP_MSK << (resultRegister * 4));
    evadcG->REVNP0.U |= (resultSrcNr << (resultRegister * 4));
}


IFX_INLINE void IfxEvadc_setResultNodeEventPointer1(Ifx_EVADC_G *evadcG, IfxEvadc_SrcNr resultSrcNr, IfxEvadc_ChannelResult resultRegister)
{
    evadcG->REVNP1.U &= ~(IFX_EVADC_G_REVNP1_REV8NP_MSK << ((resultRegister - IfxEvadc_ChannelResult_8) * 4));
    evadcG->REVNP1.U |= (resultSrcNr << ((resultRegister - IfxEvadc_ChannelResult_8) * 4));
}


IFX_INLINE void IfxEvadc_setResultPosition(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelId channelIndex, boolean rightAlignedStorage)
{
    evadcG->CHCTR[channelIndex].B.RESPOS = rightAlignedStorage;
}


IFX_INLINE void IfxEvadc_setSleepMode(Ifx_EVADC *evadc, IfxEvadc_SleepMode mode)
{
    uint16 passwd = IfxScuWdt_getCpuWatchdogPassword();
    IfxScuWdt_clearCpuEndinit(passwd);
    evadc->CLC.B.EDIS = mode;
    IfxScuWdt_setCpuEndinit(passwd);
}


IFX_INLINE void IfxEvadc_setSuspendMode(Ifx_EVADC *evadc, IfxEvadc_SuspendMode mode)
{
    Ifx_EVADC_OCS ocs;

    // remove protection and configure the suspend mode.
    ocs.B.SUS_P  = 1;
    ocs.B.SUS    = mode;
    evadc->OCS.U = ocs.U;
}


IFX_INLINE void IfxEvadc_setSyncRequest(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelId channelIndex, boolean synchonize)
{
    evadcG->CHCTR[channelIndex].B.SYNC = synchonize;
}


IFX_INLINE void IfxEvadc_setUpperBoundary(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelId channelIndex, IfxEvadc_BoundarySelection upperBoundary)
{
    evadcG->CHCTR[channelIndex].B.BNDSELU = upperBoundary;
}


IFX_INLINE void IfxEvadc_spreadEarlySamplePointEMUXConversions(Ifx_EVADC_G *evadcG, uint8 inputClassNum, IfxEvadc_SpreadEarlySamplePointEMUXConversionsMode mode)
{
    evadcG->ICLASS[inputClassNum].B.SESPE = mode;
}


IFX_INLINE void IfxEvadc_spreadEarlySamplePointStandardConversions(Ifx_EVADC_G *evadcG, uint8 inputClassNum, IfxEvadc_SpreadEarlySamplePointStandardConversionsMode mode)
{
    evadcG->ICLASS[inputClassNum].B.SESPS = mode;
}


IFX_INLINE void IfxEvadc_startQueue(Ifx_EVADC_G *group, IfxEvadc_RequestSource sourceType)
{
    switch (sourceType)
    {
    case IfxEvadc_RequestSource_queue0:
        group->Q[0].QMR.B.TREV = 1;
        break;
    case IfxEvadc_RequestSource_queue1:
        group->Q[1].QMR.B.TREV = 1;
        break;
    case IfxEvadc_RequestSource_queue2:
        group->Q[2].QMR.B.TREV = 1;
        break;
    }
}


IFX_INLINE void IfxEvadc_storeGroupResult(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelId channelIndex, IfxEvadc_ChannelResult resultRegister)
{
    evadcG->CHCTR[channelIndex].B.RESREG = resultRegister;
}


IFX_INLINE void IfxEvadc_disableReferencePrecharge(Ifx_EVADC_G *evadcG, IfxEvadc_GroupId group)
{
    evadcG->ANCFG.B.RPE = 0;
}


IFX_INLINE void IfxEvadc_disableInputBuffer(Ifx_EVADC_G *evadcG, IfxEvadc_GroupId group)
{
    evadcG->ANCFG.B.BE = 0;
}


IFX_INLINE boolean IfxEvadc_isModuleEnabled(Ifx_EVADC *evadc)
{
    return evadc->CLC.B.DISS == 0;
}


IFX_INLINE void IfxEvadc_enableStartupCalibration(Ifx_EVADC *evadc)
{
    evadc->GLOBCFG.B.SUCAL = 0x1U;
}


IFX_INLINE void IfxEvadc_selectQueueGatingMode(Ifx_EVADC_G *evadcG, IfxEvadc_RequestSource queue, IfxEvadc_GatingMode gatingMode)
{
    switch (queue)
    {
    case IfxEvadc_RequestSource_queue0:
        evadcG->Q[0].QMR.B.ENGT = gatingMode;
        break;
    case IfxEvadc_RequestSource_queue1:
        evadcG->Q[1].QMR.B.ENGT = gatingMode;
        break;
    case IfxEvadc_RequestSource_queue2:
        evadcG->Q[2].QMR.B.ENGT = gatingMode;
        break;
    }
}


IFX_INLINE void IfxEvadc_setChannelRequest(Ifx_EVADC_G *evadcG, IfxEvadc_RequestSource queue, IfxEvadc_ChannelId channel)
{
    switch (queue)
    {
    case IfxEvadc_RequestSource_queue0:
        evadcG->Q[0].QINR.B.REQCHNR = channel;
        break;
    case IfxEvadc_RequestSource_queue1:
        evadcG->Q[1].QINR.B.REQCHNR = channel;
        break;
    case IfxEvadc_RequestSource_queue2:
        evadcG->Q[2].QINR.B.REQCHNR = channel;
        break;
    }
}


IFX_INLINE void IfxEvadc_configureIclass(Ifx_EVADC_G *evadcG, uint8 inputClassNum, uint32 value)
{
    evadcG->ICLASS[inputClassNum].U = value;
}


IFX_INLINE volatile uint32 *IfxEvadc_getResultRegisterAddress(Ifx_EVADC_G *evadcG, IfxEvadc_ChannelId channel)
{
    return (volatile uint32 *)&(evadcG->RES[channel].U);
}


#endif /* IFXEVADC_H */
