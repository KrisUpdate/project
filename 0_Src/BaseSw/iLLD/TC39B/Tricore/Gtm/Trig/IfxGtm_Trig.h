/**
 * \file IfxGtm_Trig.h
 * \brief GTM TRIG details
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
 * \defgroup IfxLld_Gtm_Trig GTM Trigger Configuration
 * \ingroup IfxLld_Gtm
 * \defgroup IfxLld_Gtm_Trig_Enumerations Enumerations
 * \ingroup IfxLld_Gtm_Trig
 * \defgroup IfxLld_Gtm_Trig_Data_Structures Data Structures
 * \ingroup IfxLld_Gtm_Trig
 * \defgroup IfxLld_Gtm_Trig_Trigger_Functions Trigger Functions
 * \ingroup IfxLld_Gtm_Trig
 */

#ifndef IFXGTM_TRIG_H
#define IFXGTM_TRIG_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "Cpu/Std/Ifx_Types.h"
#include "_Impl/IfxGtm_cfg.h"
#include "Gtm/Std/IfxGtm_Tim.h"

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Gtm_Trig_Enumerations
 * \{ */
/** \brief Enum for ADC group
 */
typedef enum
{
    IfxGtm_Trig_AdcGroup_0,   /**< \brief For ADC group 0  */
    IfxGtm_Trig_AdcGroup_1,   /**< \brief For ADC group 1  */
    IfxGtm_Trig_AdcGroup_2,   /**< \brief For ADC group 2  */
    IfxGtm_Trig_AdcGroup_3,   /**< \brief For ADC group 3  */
    IfxGtm_Trig_AdcGroup_4,   /**< \brief For ADC group 4 */
    IfxGtm_Trig_AdcGroup_5,   /**< \brief For ADC group 5  */
    IfxGtm_Trig_AdcGroup_6,   /**< \brief For ADC group 6  */
    IfxGtm_Trig_AdcGroup_7,   /**< \brief For ADC group 7  */
    IfxGtm_Trig_AdcGroup_8,   /**< \brief For ADC group 8  */
    IfxGtm_Trig_AdcGroup_9,   /**< \brief For ADC group 9  */
    IfxGtm_Trig_AdcGroup_10,  /**< \brief For ADC group 10  */
    IfxGtm_Trig_AdcGroup_11   /**< \brief For ADC group 11  */
} IfxGtm_Trig_AdcGroup;

/** \brief Enum for ADC trigger
 */
typedef enum
{
    IfxGtm_Trig_AdcTrig_0,
    IfxGtm_Trig_AdcTrig_1,
    IfxGtm_Trig_AdcTrig_2,
    IfxGtm_Trig_AdcTrig_3,
    IfxGtm_Trig_AdcTrig_4,
    IfxGtm_Trig_AdcTrig_count  /**< \brief count of the enum definition */
} IfxGtm_Trig_AdcTrig;

/** \brief Enum for ADC trigger channel
 */
typedef enum
{
    IfxGtm_Trig_AdcTrigChannel_3,
    IfxGtm_Trig_AdcTrigChannel_4,
    IfxGtm_Trig_AdcTrigChannel_5,
    IfxGtm_Trig_AdcTrigChannel_6,
    IfxGtm_Trig_AdcTrigChannel_7,
    IfxGtm_Trig_AdcTrigChannel_11,
    IfxGtm_Trig_AdcTrigChannel_12,
    IfxGtm_Trig_AdcTrigChannel_13,
    IfxGtm_Trig_AdcTrigChannel_14,
    IfxGtm_Trig_AdcTrigChannel_15,
    IfxGtm_Trig_AdcTrigChannel_count  /**< \brief count of the enum definition */
} IfxGtm_Trig_AdcTrigChannel;

/** \brief Enum for ADC trigger source
 */
typedef enum
{
    IfxGtm_Trig_AdcTrigSource_atom0,
    IfxGtm_Trig_AdcTrigSource_atom1,
    IfxGtm_Trig_AdcTrigSource_atom2,
    IfxGtm_Trig_AdcTrigSource_atom3,
    IfxGtm_Trig_AdcTrigSource_atom4,
    IfxGtm_Trig_AdcTrigSource_atom5,
    IfxGtm_Trig_AdcTrigSource_atom6,
    IfxGtm_Trig_AdcTrigSource_atom7,
    IfxGtm_Trig_AdcTrigSource_atom8,
    IfxGtm_Trig_AdcTrigSource_atom9,
    IfxGtm_Trig_AdcTrigSource_atom10,
    IfxGtm_Trig_AdcTrigSource_atom11,
    IfxGtm_Trig_AdcTrigSource_tom0,
    IfxGtm_Trig_AdcTrigSource_tom1,
    IfxGtm_Trig_AdcTrigSource_tom2,
    IfxGtm_Trig_AdcTrigSource_tom3,
    IfxGtm_Trig_AdcTrigSource_tom4,
    IfxGtm_Trig_AdcTrigSource_tom5,
    IfxGtm_Trig_AdcTrigSource_count    /**< \brief count of the enum definition */
} IfxGtm_Trig_AdcTrigSource;

/** \brief CAN device enum
 */
typedef enum
{
    IfxGtm_Trig_CanDevice_0,     /**< \brief CAN device 0  */
    IfxGtm_Trig_CanDevice_1,     /**< \brief CAN device 1  */
    IfxGtm_Trig_CanDevice_2,     /**< \brief CAN device 2  */
    IfxGtm_Trig_CanDevice_count  /**< \brief count of the enum definition */
} IfxGtm_Trig_CanDevice;

/** \brief Enum for CAN trigger
 */
typedef enum
{
    IfxGtm_Trig_CanTrig_0,     /**< \brief For CAN Trig 0  */
    IfxGtm_Trig_CanTrig_1,     /**< \brief For CAN Trig 1  */
    IfxGtm_Trig_CanTrig_2,     /**< \brief For CAN Trig 2  */
    IfxGtm_Trig_CanTrig_3,     /**< \brief For CAN Trig 3  */
    IfxGtm_Trig_CanTrig_count  /**< \brief count of the enum definition */
} IfxGtm_Trig_CanTrig;

/** \brief Enum for CAN trigger channel
 */
typedef enum
{
    IfxGtm_Trig_CanTrigChannel_0,
    IfxGtm_Trig_CanTrigChannel_1,
    IfxGtm_Trig_CanTrigChannel_2,
    IfxGtm_Trig_CanTrigChannel_3,
    IfxGtm_Trig_CanTrigChannel_4,
    IfxGtm_Trig_CanTrigChannel_5,
    IfxGtm_Trig_CanTrigChannel_6,
    IfxGtm_Trig_CanTrigChannel_7,
    IfxGtm_Trig_CanTrigChannel_11,
    IfxGtm_Trig_CanTrigChannel_12,
    IfxGtm_Trig_CanTrigChannel_13,
    IfxGtm_Trig_CanTrigChannel_14,
    IfxGtm_Trig_CanTrigChannel_count  /**< \brief count of the enum definition */
} IfxGtm_Trig_CanTrigChannel;

/** \brief Enum for CAN trigger source
 */
typedef enum
{
    IfxGtm_Trig_CanTrigSource_tom0,
    IfxGtm_Trig_CanTrigSource_tom1,
    IfxGtm_Trig_CanTrigSource_atom0,
    IfxGtm_Trig_CanTrigSource_atom1,
    IfxGtm_Trig_CanTrigSource_atom2,
    IfxGtm_Trig_CanTrigSource_count   /**< \brief count of the enum definition */
} IfxGtm_Trig_CanTrigSource;

/** \brief Enum for EDSADC trigger
 */
typedef enum
{
    IfxGtm_Trig_EdsadcTrig_0,
    IfxGtm_Trig_EdsadcTrig_1,
    IfxGtm_Trig_EdsadcTrig_2,
    IfxGtm_Trig_EdsadcTrig_3
} IfxGtm_Trig_EdsadcTrig;

/** \brief Enum for EDSADC trigger source
 */
typedef enum
{
    IfxGtm_Trig_EdsadcTrigSource_tom0_6,
    IfxGtm_Trig_EdsadcTrigSource_tom0_7,
    IfxGtm_Trig_EdsadcTrigSource_tom0_13,
    IfxGtm_Trig_EdsadcTrigSource_tom0_14,
    IfxGtm_Trig_EdsadcTrigSource_atom0_4,
    IfxGtm_Trig_EdsadcTrigSource_atom0_5,
    IfxGtm_Trig_EdsadcTrigSource_atom0_6,
    IfxGtm_Trig_EdsadcTrigSource_atom0_7,
    IfxGtm_Trig_EdsadcTrigSource_atom1_4,
    IfxGtm_Trig_EdsadcTrigSource_atom1_5,
    IfxGtm_Trig_EdsadcTrigSource_atom1_6,
    IfxGtm_Trig_EdsadcTrigSource_atom1_7,
    IfxGtm_Trig_EdsadcTrigSource_atom4_6,
    IfxGtm_Trig_EdsadcTrigSource_atom4_7,
    IfxGtm_Trig_EdsadcTrigSource_atom6_6,
    IfxGtm_Trig_EdsadcTrigSource_reserved0,
    IfxGtm_Trig_EdsadcTrigSource_tom2_6,
    IfxGtm_Trig_EdsadcTrigSource_tom2_7,
    IfxGtm_Trig_EdsadcTrigSource_tom2_13,
    IfxGtm_Trig_EdsadcTrigSource_tom2_14,
    IfxGtm_Trig_EdsadcTrigSource_atom2_4,
    IfxGtm_Trig_EdsadcTrigSource_atom2_5,
    IfxGtm_Trig_EdsadcTrigSource_atom2_6,
    IfxGtm_Trig_EdsadcTrigSource_atom2_7,
    IfxGtm_Trig_EdsadcTrigSource_atom3_4,
    IfxGtm_Trig_EdsadcTrigSource_atom3_5,
    IfxGtm_Trig_EdsadcTrigSource_atom3_6,
    IfxGtm_Trig_EdsadcTrigSource_atom3_7,
    IfxGtm_Trig_EdsadcTrigSource_atom5_6,
    IfxGtm_Trig_EdsadcTrigSource_atom5_7,
    IfxGtm_Trig_EdsadcTrigSource_atom6_7,
    IfxGtm_Trig_EdsadcTrigSource_reserved1,
    IfxGtm_Trig_EdsadcTrigSource_tom1_7,
    IfxGtm_Trig_EdsadcTrigSource_tom1_14,
    IfxGtm_Trig_EdsadcTrigSource_tom3_7,
    IfxGtm_Trig_EdsadcTrigSource_tom3_14,
    IfxGtm_Trig_EdsadcTrigSource_tom4_7,
    IfxGtm_Trig_EdsadcTrigSource_tom4_14,
    IfxGtm_Trig_EdsadcTrigSource_tom5_7,
    IfxGtm_Trig_EdsadcTrigSource_tom5_14,
    IfxGtm_Trig_EdsadcTrigSource_atom7_6,
    IfxGtm_Trig_EdsadcTrigSource_atom7_7,
    IfxGtm_Trig_EdsadcTrigSource_atom8_6,
    IfxGtm_Trig_EdsadcTrigSource_atom8_7,
    IfxGtm_Trig_EdsadcTrigSource_atom9_6,
    IfxGtm_Trig_EdsadcTrigSource_atom9_7,
    IfxGtm_Trig_EdsadcTrigSource_atom11_6,
    IfxGtm_Trig_EdsadcTrigSource_reserved2,
    IfxGtm_Trig_EdsadcTrigSource_tom1_6,
    IfxGtm_Trig_EdsadcTrigSource_atom10_6,
    IfxGtm_Trig_EdsadcTrigSource_atom10_7,
    IfxGtm_Trig_EdsadcTrigSource_atom11_7
} IfxGtm_Trig_EdsadcTrigSource;

/** \brief Enum for MSC trigger channel
 */
typedef enum
{
    IfxGtm_Trig_MscTrigChannel_0,
    IfxGtm_Trig_MscTrigChannel_1,
    IfxGtm_Trig_MscTrigChannel_2,
    IfxGtm_Trig_MscTrigChannel_3,
    IfxGtm_Trig_MscTrigChannel_4,
    IfxGtm_Trig_MscTrigChannel_5,
    IfxGtm_Trig_MscTrigChannel_6,
    IfxGtm_Trig_MscTrigChannel_7,
    IfxGtm_Trig_MscTrigChannel_8,
    IfxGtm_Trig_MscTrigChannel_9,
    IfxGtm_Trig_MscTrigChannel_10,
    IfxGtm_Trig_MscTrigChannel_11,
    IfxGtm_Trig_MscTrigChannel_12,
    IfxGtm_Trig_MscTrigChannel_13,
    IfxGtm_Trig_MscTrigChannel_14,
    IfxGtm_Trig_MscTrigChannel_15,
    IfxGtm_Trig_MscTrigChannel_count  /**< \brief count of the enum definition */
} IfxGtm_Trig_MscTrigChannel;

/** \brief Enum for MSC trigger Input Type
 */
typedef enum
{
    IfxGtm_Trig_MscTrigInput_low,
    IfxGtm_Trig_MscTrigInput_lowExtended,
    IfxGtm_Trig_MscTrigInput_high
} IfxGtm_Trig_MscTrigInput;

/** \brief Enum for MSC trigger output select
 */
typedef enum
{
    IfxGtm_Trig_MscTrigOutput_0,
    IfxGtm_Trig_MscTrigOutput_1,
    IfxGtm_Trig_MscTrigOutput_2,
    IfxGtm_Trig_MscTrigOutput_3
} IfxGtm_Trig_MscTrigOutput;

/** \brief Enum for MSC Sets
 */
typedef enum
{
    IfxGtm_Trig_MscTrigSet_1,     /**< \brief For MSC set 1  */
    IfxGtm_Trig_MscTrigSet_2,     /**< \brief For MSC set 2  */
    IfxGtm_Trig_MscTrigSet_3,     /**< \brief For MSC set 3  */
    IfxGtm_Trig_MscTrigSet_4,     /**< \brief For MSC set 4  */
    IfxGtm_Trig_MscTrigSet_5,     /**< \brief For MSC set 5  */
    IfxGtm_Trig_MscTrigSet_6,     /**< \brief For MSC set 6  */
    IfxGtm_Trig_MscTrigSet_7,     /**< \brief For MSC set 7  */
    IfxGtm_Trig_MscTrigSet_8,     /**< \brief For MSC set 8  */
    IfxGtm_Trig_MscTrigSet_9,     /**< \brief For MSC set 9  */
    IfxGtm_Trig_MscTrigSet_count  /**< \brief count of the enum definition */
} IfxGtm_Trig_MscTrigSet;

/** \brief Enum for MSC Signal
 */
typedef enum
{
    IfxGtm_Trig_MscTrigSignal_0,     /**< \brief For MSC set 0  */
    IfxGtm_Trig_MscTrigSignal_1,     /**< \brief For MSC set 1  */
    IfxGtm_Trig_MscTrigSignal_2,     /**< \brief For MSC set 2  */
    IfxGtm_Trig_MscTrigSignal_3,     /**< \brief For MSC set 3  */
    IfxGtm_Trig_MscTrigSignal_4,     /**< \brief For MSC set 4  */
    IfxGtm_Trig_MscTrigSignal_5,     /**< \brief For MSC set 5  */
    IfxGtm_Trig_MscTrigSignal_6,     /**< \brief For MSC set 6  */
    IfxGtm_Trig_MscTrigSignal_7,     /**< \brief For MSC set 7  */
    IfxGtm_Trig_MscTrigSignal_8,     /**< \brief For MSC set 8  */
    IfxGtm_Trig_MscTrigSignal_9,     /**< \brief For MSC set 9  */
    IfxGtm_Trig_MscTrigSignal_10,    /**< \brief For MSC set 10  */
    IfxGtm_Trig_MscTrigSignal_11,    /**< \brief For MSC set 11  */
    IfxGtm_Trig_MscTrigSignal_12,    /**< \brief For MSC set 12  */
    IfxGtm_Trig_MscTrigSignal_13,    /**< \brief For MSC set 13  */
    IfxGtm_Trig_MscTrigSignal_14,    /**< \brief For MSC set 14  */
    IfxGtm_Trig_MscTrigSignal_15     /**< \brief For MSC set 15  */
} IfxGtm_Trig_MscTrigSignal;

/** \brief Enum for MSC trigger source
 */
typedef enum
{
    IfxGtm_Trig_MscTrigSource_tom0,
    IfxGtm_Trig_MscTrigSource_tom1,
    IfxGtm_Trig_MscTrigSource_tom2,
    IfxGtm_Trig_MscTrigSource_tom3,
    IfxGtm_Trig_MscTrigSource_tom4,
    IfxGtm_Trig_MscTrigSource_tom5,
    IfxGtm_Trig_MscTrigSource_atom0,
    IfxGtm_Trig_MscTrigSource_atom1,
    IfxGtm_Trig_MscTrigSource_atom2,
    IfxGtm_Trig_MscTrigSource_atom3,
    IfxGtm_Trig_MscTrigSource_atom4,
    IfxGtm_Trig_MscTrigSource_atom5,
    IfxGtm_Trig_MscTrigSource_atom6,
    IfxGtm_Trig_MscTrigSource_atom7,
    IfxGtm_Trig_MscTrigSource_atom8,
    IfxGtm_Trig_MscTrigSource_atom9,
    IfxGtm_Trig_MscTrigSource_atom10,
    IfxGtm_Trig_MscTrigSource_atom11,
    IfxGtm_Trig_MscTrigSource_count    /**< \brief count of the enum definition */
} IfxGtm_Trig_MscTrigSource;

/** \brief Enum for PSI5 trigger source
 */
typedef enum
{
    IfxGtm_Trig_Psi5TrigSource_no_Trigger,
    IfxGtm_Trig_Psi5TrigSource_tom2_6,
    IfxGtm_Trig_Psi5TrigSource_tom2_7,
    IfxGtm_Trig_Psi5TrigSource_tom2_13,
    IfxGtm_Trig_Psi5TrigSource_tom2_14,
    IfxGtm_Trig_Psi5TrigSource_atom2_4,
    IfxGtm_Trig_Psi5TrigSource_atom2_5,
    IfxGtm_Trig_Psi5TrigSource_atom2_6,
    IfxGtm_Trig_Psi5TrigSource_atom2_7,
    IfxGtm_Trig_Psi5TrigSource_tom0_6,
    IfxGtm_Trig_Psi5TrigSource_tom0_7,
    IfxGtm_Trig_Psi5TrigSource_tom0_13,
    IfxGtm_Trig_Psi5TrigSource_tom0_14
} IfxGtm_Trig_Psi5TrigSource;

/** \brief Enum for DSADC trigger source
 */
typedef enum
{
    IfxGtm_Trig_Psi5sTrigSource_no_Trigger,
    IfxGtm_Trig_Psi5sTrigSource_tomX_6,
    IfxGtm_Trig_Psi5sTrigSource_tomX_7,
    IfxGtm_Trig_Psi5sTrigSource_tomX_13,
    IfxGtm_Trig_Psi5sTrigSource_tomX_14,
    IfxGtm_Trig_Psi5sTrigSource_atomX_4,
    IfxGtm_Trig_Psi5sTrigSource_atomX_5,
    IfxGtm_Trig_Psi5sTrigSource_atomX_6,
    IfxGtm_Trig_Psi5sTrigSource_atomX_7
} IfxGtm_Trig_Psi5sTrigSource;

/** \brief Enum for SENT group
 */
typedef enum
{
    IfxGtm_Trig_SentGroup_0,    /**< \brief For SENT group 0  */
    IfxGtm_Trig_SentGroup_1,    /**< \brief For SENT group 1  */
    IfxGtm_Trig_SentGroup_2,    /**< \brief For SENT group 2  */
    IfxGtm_Trig_SentGroup_3,    /**< \brief For SENT group 3  */
    IfxGtm_Trig_SentGroup_4,    /**< \brief For SENT group 4 */
    IfxGtm_Trig_SentGroup_5,    /**< \brief For SENT group 5  */
    IfxGtm_Trig_SentGroup_6,    /**< \brief For SENT group 6  */
    IfxGtm_Trig_SentGroup_7,    /**< \brief For SENT group 7  */
    IfxGtm_Trig_SentGroup_8,    /**< \brief For SENT group 8  */
    IfxGtm_Trig_SentGroup_9,    /**< \brief For SENT group 9  */
    IfxGtm_Trig_SentGroup_10,   /**< \brief For SENT group 10  */
    IfxGtm_Trig_SentGroup_11,   /**< \brief For SENT group 11  */
    IfxGtm_Trig_SentGroup_12,   /**< \brief For SENT group 12  */
    IfxGtm_Trig_SentGroup_13,   /**< \brief For SENT group 13  */
    IfxGtm_Trig_SentGroup_14,   /**< \brief For SENT group 14  */
    IfxGtm_Trig_SentGroup_15    /**< \brief For SENT group 15 */
} IfxGtm_Trig_SentGroup;

/** \brief Enum for SENT trigger
 */
typedef enum
{
    IfxGtm_Trig_SentTrig_0,     /**< \brief sent trigger 0  */
    IfxGtm_Trig_SentTrig_1,     /**< \brief sent trigger 1  */
    IfxGtm_Trig_SentTrig_2,     /**< \brief sent trigger 2  */
    IfxGtm_Trig_SentTrig_3      /**< \brief sent trigger 3  */
} IfxGtm_Trig_SentTrig;

/** \brief Enum for SENT trigger channel
 */
typedef enum
{
    IfxGtm_Trig_SentTrigChannel_3,
    IfxGtm_Trig_SentTrigChannel_4,
    IfxGtm_Trig_SentTrigChannel_5,
    IfxGtm_Trig_SentTrigChannel_6,
    IfxGtm_Trig_SentTrigChannel_7,
    IfxGtm_Trig_SentTrigChannel_11,
    IfxGtm_Trig_SentTrigChannel_12,
    IfxGtm_Trig_SentTrigChannel_13,
    IfxGtm_Trig_SentTrigChannel_14,
    IfxGtm_Trig_SentTrigChannel_15,
    IfxGtm_Trig_SentTrigChannel_count  /**< \brief count of the enum definition */
} IfxGtm_Trig_SentTrigChannel;

/** \brief Enum for SENT trigger source
 */
typedef enum
{
    IfxGtm_Trig_SentTrigSource_atom0,
    IfxGtm_Trig_SentTrigSource_atom1,
    IfxGtm_Trig_SentTrigSource_atom2,
    IfxGtm_Trig_SentTrigSource_atom3,
    IfxGtm_Trig_SentTrigSource_atom4,
    IfxGtm_Trig_SentTrigSource_atom5,
    IfxGtm_Trig_SentTrigSource_atom6,
    IfxGtm_Trig_SentTrigSource_atom7,
    IfxGtm_Trig_SentTrigSource_atom8,
    IfxGtm_Trig_SentTrigSource_atom9,
    IfxGtm_Trig_SentTrigSource_atom10,
    IfxGtm_Trig_SentTrigSource_atom11,
    IfxGtm_Trig_SentTrigSource_tom0,
    IfxGtm_Trig_SentTrigSource_tom1,
    IfxGtm_Trig_SentTrigSource_tom2,
    IfxGtm_Trig_SentTrigSource_tom3,
    IfxGtm_Trig_SentTrigSource_tom4,
    IfxGtm_Trig_SentTrigSource_tom5,
    IfxGtm_Trig_SentTrigSource_count    /**< \brief count of the enum definition */
} IfxGtm_Trig_SentTrigSource;

/** \} */

/** \addtogroup IfxLld_Gtm_Trig_Trigger_Functions
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/**
 * \param gtm Pointer to GTM module
 * \param edsadcChannel EDSADC channel
 * \param tim TIM object
 * \param timChannel TIM channel
 * \return TRUE on success else FALSE
 */
IFX_EXTERN boolean IfxGtm_Trig_fromEdsadc(Ifx_GTM *gtm, uint32 edsadcChannel, IfxGtm_Tim tim, IfxGtm_Tim_Ch timChannel);

/**
 * \param gtm Pointer to GTM module
 * \param msc MSC Id
 * \param input MSC Input Type
 * \param output Output Select
 * \param signal MSC signal
 * \return TRUE on success else FALSE
 */
IFX_EXTERN boolean IfxGtm_Trig_fromMsc(Ifx_GTM *gtm, uint32 msc, IfxGtm_Trig_MscTrigInput input, IfxGtm_Trig_MscTrigOutput output, IfxGtm_Trig_MscTrigSignal signal);

/**
 * \param gtm Pointer to GTM module
 * \param canTrig CAN trigger
 * \param canDevice CAN device
 * \param source CAN trigger source
 * \param channel CAN trigger channel
 * \return TRUE on success else FALSE
 */
IFX_EXTERN boolean IfxGtm_Trig_toCan(Ifx_GTM *gtm, IfxGtm_Trig_CanTrig canTrig, IfxGtm_Trig_CanDevice canDevice, IfxGtm_Trig_CanTrigSource source, IfxGtm_Trig_CanTrigChannel channel);

/**
 * \param gtm Pointer to GTM module
 * \param adcGroup ADC group
 * \param adcTrig ADC trigger
 * \param source ADC trigger source
 * \param channel ADC trigger channel
 * \return TRUE on success else FALSE
 */
IFX_EXTERN boolean IfxGtm_Trig_toEVadc(Ifx_GTM *gtm, IfxGtm_Trig_AdcGroup adcGroup, IfxGtm_Trig_AdcTrig adcTrig, IfxGtm_Trig_AdcTrigSource source, IfxGtm_Trig_AdcTrigChannel channel);

/**
 * \param gtm Pointer to GTM module
 * \param adcGroup ADC group
 * \param adcTrig ADC trigger
 * \return None
 */
IFX_EXTERN void IfxGtm_Trig_toEVadcClear(Ifx_GTM *gtm, IfxGtm_Trig_AdcGroup adcGroup, IfxGtm_Trig_AdcTrig adcTrig);

/**
 * \param gtm Pointer to GTM module
 * \param edsadcChannel EDSADC channel
 * \param edsadcTrig EDSADC trigger
 * \param sel DSADC trigger source
 * \return None
 */
IFX_EXTERN void IfxGtm_Trig_toEdsadc(Ifx_GTM *gtm, uint32 edsadcChannel, IfxGtm_Trig_EdsadcTrig edsadcTrig, IfxGtm_Trig_EdsadcTrigSource sel);

/**
 * \param gtm Pointer to GTM module
 * \param signal MSC signal
 * \param mscSet MSC set
 * \param source MSC trigger source
 * \param channel MSC trigger channel
 * \return TRUE on success else FALSE
 */
IFX_EXTERN boolean IfxGtm_Trig_toMsc(Ifx_GTM *gtm, IfxGtm_Trig_MscTrigSignal signal, IfxGtm_Trig_MscTrigSet mscSet, IfxGtm_Trig_MscTrigSource source, IfxGtm_Trig_MscTrigChannel channel);

/**
 * \param gtm Pointer to GTM module
 * \param psi5Channel PSI5 channel
 * \param sel PSI5 trigger source
 * \return None
 */
IFX_EXTERN void IfxGtm_Trig_toPsi5(Ifx_GTM *gtm, uint32 psi5Channel, IfxGtm_Trig_Psi5TrigSource sel);

/**
 * \param gtm Pointer to GTM module
 * \param psi5sChannel PSI5S channel
 * \param sel PSI5S trigger source
 * \return None
 */
IFX_EXTERN void IfxGtm_Trig_toPsi5s(Ifx_GTM *gtm, uint32 psi5sChannel, IfxGtm_Trig_Psi5sTrigSource sel);

/**
 * \param gtm Pointer to GTM module
 * \param sentGroup SENT group
 * \param sentTrig SENT trigger
 * \param source SENT trigger source
 * \param channel SENT trigger channel
 * \return TRUE on success else FALSE
 */
IFX_EXTERN boolean IfxGtm_Trig_toSent(Ifx_GTM *gtm, IfxGtm_Trig_SentGroup sentGroup, IfxGtm_Trig_SentTrig sentTrig, IfxGtm_Trig_SentTrigSource source, IfxGtm_Trig_SentTrigChannel channel);

/** \} */

#endif /* IFXGTM_TRIG_H */
