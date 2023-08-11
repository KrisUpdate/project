/**
 * \file IfxSmu_Smu.h
 * \brief SMU SMU details
 * \ingroup IfxLld_Smu
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
 * \defgroup IfxLld_Smu_Smu SMU
 * \ingroup IfxLld_Smu
 * \defgroup IfxLld_Smu_Smu_DataStructures Data Structures
 * \ingroup IfxLld_Smu_Smu
 * \defgroup IfxLld_Smu_Smu_SmuFunctions Smu Functions
 * \ingroup IfxLld_Smu_Smu
 */

#ifndef IFXSMU_SMU_H
#define IFXSMU_SMU_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "Smu/Std/IfxSmu.h"
#include "_Impl/IfxSmu_cfg.h"

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Smu_Smu_DataStructures
 * \{ */
/** \brief Alarm Global Configuration
 */
typedef struct
{
    uint8   igcs0;                            /**< \brief AGC.B.IGCS0 */
    uint8   igcs1;                            /**< \brief AGC.B.IGCS1 */
    uint8   igcs2;                            /**< \brief AGC.B.IGCS2 */
    uint8   resetConfigSet;                   /**< \brief AGC.B.RCS */
    uint8   portEmergencyStop;                /**< \brief AGC.B.PES */
    boolean enableFaultToRunTransition;       /**< \brief AGC.B.EFRST */
} IfxSmu_Smu_AlarmGlobalConfiguration;

/** \brief FSP configuration
 */
typedef struct
{
    IfxSmu_FspPrescalar1 prescalar1;                   /**< \brief FSP.B.PRE1 */
    IfxSmu_FspPrescalar2 prescalar2;                   /**< \brief FSP.B.PRE2 */
    IfxSmu_FspMode       mode;
    boolean              portEmergencyStop;            /**< \brief FSP.B.PES */
    uint16               faultStateDurationHigh;       /**< \brief FSP.B.TFSP_HIGH */
    uint16               faultStateDurationLow;        /**< \brief FSP.B.TFSP_LOW */
} IfxSmu_Smu_FaultSignalingProtocol;

/** \brief Recovery Timer/s Alarm Configuration
 */
typedef struct
{
    uint8 groupIndex0;
    uint8 alarmId0;
    uint8 groupIndex1;
    uint8 alarmId1;
    uint8 groupIndex2;
    uint8 alarmId2;
    uint8 groupIndex3;
    uint8 alarmId3;
} IfxSmu_Smu_RecoveryTimerAlarmConfiguration;

/** \brief Recovery Timer Configuration
 */
typedef struct
{
    boolean enableRecoveryTimer0;        /**< \brief RTC.B.RT0E */
    boolean enableRecoveryTimer1;        /**< \brief RTC.B.RT1E */
    uint32  recoveryTimerDuration;       /**< \brief RTC.B.RTD */
} IfxSmu_Smu_RecoveryTimerConfig;

/** \} */

/** \addtogroup IfxLld_Smu_Smu_DataStructures
 * \{ */
typedef struct
{
    IfxSmu_Smu_FaultSignalingProtocol          fsp;
    IfxSmu_Smu_AlarmGlobalConfiguration        alarmGlobalConfig;
    IfxSmu_Smu_RecoveryTimerConfig             recoveryTimerConfig;
    IfxSmu_Smu_RecoveryTimerAlarmConfiguration recoveryTimerAlarmConfig[2];                           /**< \brief recovery Timer Alarm 0/1 configuration */
    uint32                                     alarmConfigRegister[IFXSMU_NUM_ALARM_GROUPS][3];       /**< \brief AGCF */
    uint32                                     fspConfigRegister[IFXSMU_NUM_ALARM_GROUPS];            /**< \brief AGFSP */
} IfxSmu_Smu_Config;

/** \} */

/** \addtogroup IfxLld_Smu_Smu_SmuFunctions
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Fills the configuration stucture of the SMU Module with default values
 * \param config configuration structure of the module
 * \return None
 */
IFX_EXTERN void IfxSmu_Smu_initModuleConfig(IfxSmu_Smu_Config *config);

/** \brief Initialises the SMU Module
 * \param config configuration structure of the module
 * \return None
 */
IFX_EXTERN void IfxSmu_Smu_initModule(IfxSmu_Smu_Config *config);

/** \brief Check if any alarm is Triggered.
 * \return Returns True if any alarm is set
 */
IFX_EXTERN boolean IfxSmu_Smu_isAlarmTriggered(void);

/** \} */

#endif /* IFXSMU_SMU_H */
