/**
 * \file IfxSmu.h
 * \brief SMU  basic functionality
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
 * \defgroup IfxLld_Smu_Std_Enumerations Enumerations
 * \ingroup IfxLld_Smu_Std
 * \defgroup IfxLld_Smu_Std_ModuleFunctions Module Functions
 * \ingroup IfxLld_Smu_Std
 */

#ifndef IFXSMU_H
#define IFXSMU_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "_Impl/IfxSmu_cfg.h"
#include "IfxSmu_reg.h"
#include "Scu/Std/IfxScuWdt.h"
#include "_Utilities/Ifx_Assert.h"
#include "IfxSrc_reg.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/** \brief Argument to start Alive test
 */
#define IFXSMU_START_ALIVE_TEST_ARG (5)

/** \brief Argument to stop Alive test
 */
#define IFXSMU_STOP_ALIVE_TEST_ARG  (0xA)

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Smu_Std_Enumerations
 * \{ */
/** \brief Command interface
 */
typedef enum
{
    IfxSmu_Command_start            = 0,  /**< \brief Forces the SSM to go to the RUN state from the START state */
    IfxSmu_Command_activateFSP      = 1,  /**< \brief Activates the Fault Signaling Protocol. */
    IfxSmu_Command_releaseFSP       = 2,  /**< \brief Turns the FSP into the inactive fault free state */
    IfxSmu_Command_activatePES      = 3,  /**< \brief Triggers the activation of the Port Emergency Stop (PES). */
    IfxSmu_Command_stopRT           = 4,  /**< \brief Stop the recovery Timer. */
    IfxSmu_Command_alarmStatusClear = 5,  /**< \brief Alarm Status Clear Enable Command */
    IfxSmu_Command_triggerAlarm     = 6,  /**< \brief Triggers a software based alarm */
    IfxSmu_Command_aliveTest        = 7   /**< \brief Triggers a SMU alive alarm */
} IfxSmu_Command;

/** \brief Fault Signaling Protocol configuration
 */
typedef enum
{
    IfxSmu_FspMode_BiStableProtocol      = 0, /**< \brief Bi-stable protocol */
    IfxSmu_FspMode_dualRailProtocol      = 1, /**< \brief dualRailProtocol */
    IfxSmu_FspMode_TimeSwitchingProtocol = 2  /**< \brief Time switching protocol */
} IfxSmu_FspMode;

/** \brief Dividing factor to apply to the reference clock fBACK. The divided clock is used as reference to generate the timing of the fault signaling protocol fault state.
 */
typedef enum
{
    IfxSmu_FspPrescalar1_referenceClockDiv2   = 0,  /**< \brief FSMU_FS = reference clock frequency divided by 2 */
    IfxSmu_FspPrescalar1_referenceClockDiv4   = 1,  /**< \brief FSMU_FS = reference clock frequency divided by 4 */
    IfxSmu_FspPrescalar1_referenceClockDiv8   = 2,  /**< \brief FSMU_FS = reference clock frequency divided by 8 */
    IfxSmu_FspPrescalar1_referenceClockDiv16  = 3,  /**< \brief FSMU_FS = reference clock frequency divided by 16 */
    IfxSmu_FspPrescalar1_referenceClockDiv32  = 4,  /**< \brief FSMU_FS = reference clock frequency divided by 32 */
    IfxSmu_FspPrescalar1_referenceClockDiv64  = 5,  /**< \brief FSMU_FS = reference clock frequency divided by 64 */
    IfxSmu_FspPrescalar1_referenceClockDiv128 = 6,  /**< \brief FSMU_FS = reference clock frequency divided by 128 */
    IfxSmu_FspPrescalar1_referenceClockDiv256 = 7   /**< \brief FSMU_FS = reference clock frequency divided by 256 */
} IfxSmu_FspPrescalar1;

/** \brief Dividing factor to apply to the reference clock fBACK in order to generate the timing of the fault free state for the time switching modes of the fault signaling protocol.
 */
typedef enum
{
    IfxSmu_FspPrescalar2_referenceClockDiv512  = 0,  /**< \brief FSMU_FS = reference clock frequency divided by 512 */
    IfxSmu_FspPrescalar2_referenceClockDiv1024 = 1,  /**< \brief FSMU_FS = reference clock frequency divided by 1024 */
    IfxSmu_FspPrescalar2_referenceClockDiv2048 = 2,  /**< \brief FSMU_FS = reference clock frequency divided by 2048 */
    IfxSmu_FspPrescalar2_referenceClockDiv4096 = 3   /**< \brief FSMU_FS = reference clock frequency divided by 4096 */
} IfxSmu_FspPrescalar2;

typedef enum
{
    IfxSmu_InternalAlarmAction_disabled = 0,  /**< \brief Alarm disabled. */
    IfxSmu_InternalAlarmAction_igcs0    = 2,  /**< \brief Sends an interrupt request to the interrupt system according to the IGCS0 */
    IfxSmu_InternalAlarmAction_igcs1    = 3,  /**< \brief Sends an interrupt request to the interrupt system according to the IGCS1 */
    IfxSmu_InternalAlarmAction_igcs2    = 4,  /**< \brief Sends an interrupt request to the interrupt system according to the IGCS2 */
    IfxSmu_InternalAlarmAction_nmi      = 5,  /**< \brief Sends an NMI request to the SCU */
    IfxSmu_InternalAlarmAction_reset    = 6,  /**< \brief Sends a reset request to the SCU. */
    IfxSmu_InternalAlarmAction_cpureset = 7   /**< \brief Triggers a CPU reset request using CPU Reset Configuration Set */
} IfxSmu_InternalAlarmAction;

/** \brief Interrupt Generation Configuration
 */
typedef enum
{
    IfxSmu_InterruptGenerationConfiguration_0 = 0,      /**< \brief Interrupt Generation Configuration 0  */
    IfxSmu_InterruptGenerationConfiguration_1,          /**< \brief Interrupt Generation Configuration 1  */
    IfxSmu_InterruptGenerationConfiguration_2           /**< \brief Interrupt Generation Configuration 2  */
} IfxSmu_InterruptGenerationConfiguration;

/** \brief Interrupt Request
 */
typedef enum
{
    IfxSmu_InterruptRequest_0 = 0,      /**< \brief Interrupt Request 0  */
    IfxSmu_InterruptRequest_1,          /**< \brief Interrupt Request 1  */
    IfxSmu_InterruptRequest_2           /**< \brief Interrupt Request 2  */
} IfxSmu_InterruptRequest;

/** \brief Enum for the FSP_DIR output port control direction
 */
typedef enum
{
    IfxSmu_PortControlHwDir_fsp0AndFsp1Input       = 0,  /**< \brief fsp0InputAndFsp1Input */
    IfxSmu_PortControlHwDir_fsp0OutputAndFsp1Input = 1,  /**< \brief fsp0OutputAndFsp1Input */
    IfxSmu_PortControlHwDir_fsp0AndFsp1Output      = 3   /**< \brief fsp0OutputAndFsp1Input */
} IfxSmu_PortControlHwDir;

/** \brief Enum for the FSP_DIR output port control enable
 */
typedef enum
{
    IfxSmu_PortControlHwEnable_fsp0AndFsp1Disabled        = 0,  /**< \brief fsp0AndFsp1Disabled */
    IfxSmu_PortControlHwEnable_fsp0EnabledAndFsp1Disabled = 1,  /**< \brief fsp0EnabledAndFsp1Disabled */
    IfxSmu_PortControlHwEnable_fsp0AndFsp1Enabled         = 3   /**< \brief fsp0AndFsp1Enabled */
} IfxSmu_PortControlHwEnable;

/** \brief Running state of the SMU State Machine
 */
typedef enum
{
    IfxSmu_SmuState_start       = 0, /**< \brief START state */
    IfxSmu_SmuState_run         = 1, /**< \brief RUN State */
    IfxSmu_SmuState_fault       = 2, /**< \brief FAULT State */
    IfxSmu_SmuState_unspecified = 3  /**< \brief unspecified State */
} IfxSmu_SmuState;

/** \brief OCDS Suspend Control (OCDS.SUS)
 */
typedef enum
{
    IfxSmu_SuspendMode_none = 0,  /**< \brief No suspend */
    IfxSmu_SuspendMode_hard = 1,  /**< \brief Hard Suspend */
    IfxSmu_SuspendMode_soft = 2   /**< \brief Soft Suspend */
} IfxSmu_SuspendMode;

/** \} */

/** \brief Alarm Executed Status
 */
typedef enum
{
    IfxSmu_AlarmExecutionStatus_irq0    = 0,   /**< \brief IRQ0 Request Status */
    IfxSmu_AlarmExecutionStatus_irq1    = 1,   /**< \brief IRQ1 Request Status */
    IfxSmu_AlarmExecutionStatus_irq2    = 2,   /**< \brief IRQ2 Request Status */
    IfxSmu_AlarmExecutionStatus_rst0    = 3,   /**< \brief RST0 Request Status */
    IfxSmu_AlarmExecutionStatus_rst1    = 4,   /**< \brief RST1 Request Status */
    IfxSmu_AlarmExecutionStatus_rst2    = 5,   /**< \brief RST2 Request Status */
    IfxSmu_AlarmExecutionStatus_rst3    = 6,   /**< \brief RST3 Request Status */
    IfxSmu_AlarmExecutionStatus_rst4    = 7,   /**< \brief RST4 Request Status */
    IfxSmu_AlarmExecutionStatus_rst5    = 8,   /**< \brief RST5 Request Status */
    IfxSmu_AlarmExecutionStatus_nmi     = 9,   /**< \brief NMI Request Status */
    IfxSmu_AlarmExecutionStatus_rst     = 10,  /**< \brief RST Request Status */
    IfxSmu_AlarmExecutionStatus_ems     = 11,  /**< \brief EMS Request Status */
    IfxSmu_AlarmExecutionStatus_irq0aem = 16,  /**< \brief IRQ0 AEM Request Status */
    IfxSmu_AlarmExecutionStatus_irq1aem = 17,  /**< \brief IRQ1 AEM Request Status */
    IfxSmu_AlarmExecutionStatus_irq2aem = 18,  /**< \brief IRQ2 AEM Request Status */
    IfxSmu_AlarmExecutionStatus_rst0aem = 19,  /**< \brief RST0 AEM Request Status */
    IfxSmu_AlarmExecutionStatus_rst1aem = 20,  /**< \brief RST1 AEM Request Status */
    IfxSmu_AlarmExecutionStatus_rst2aem = 21,  /**< \brief RST2 AEM Request Status */
    IfxSmu_AlarmExecutionStatus_rst3aem = 22,  /**< \brief RST3 AEM Request Status */
    IfxSmu_AlarmExecutionStatus_rst4aem = 23,  /**< \brief RST4 AEM Request Status */
    IfxSmu_AlarmExecutionStatus_rst5aem = 24,  /**< \brief RST5 AEM Request Status */
    IfxSmu_AlarmExecutionStatus_nmiaem  = 25,  /**< \brief NMI AEM Request Status */
    IfxSmu_AlarmExecutionStatus_rstaem  = 26,  /**< \brief RST AEM Request Status */
    IfxSmu_AlarmExecutionStatus_emsaem  = 27   /**< \brief EMS AEM Request Status */
} IfxSmu_AlarmExecutionStatus;

/** \addtogroup IfxLld_Smu_Std_ModuleFunctions
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief This function clears SMU alarm exected status of the requested alarm mechanism
 * \param alarmEx alarm mechanism
 * \return None
 */
IFX_INLINE void IfxSmu_clearAlarmExecutedStatus(IfxSmu_AlarmExecutionStatus alarmEx);

/** \brief Function clears the Register monitor test mode enable Flag.
 *
 * The RMCTL register needs to be unlocked by using IfxSmu_unlockConfigRegisters() before calling this function.
 * \param testModeEnable Test Mode Flag
 * \return None
 */
IFX_INLINE void IfxSmu_clearRegMonTestModeEnable(uint8 testModeEnable);

/** \brief Function clears the Register Monitor Error Flag (RMEF.EFz).
 * \param errorFlag Error Flag
 * \return None
 */
IFX_INLINE void IfxSmu_clearRegisterMonitorErrorFlag(uint8 errorFlag);

/** \brief Function clears the Register Monitor status register.
 * \param statusFlag status Flag
 * \return None
 */
IFX_INLINE void IfxSmu_clearRegisterMonitorStatus(uint8 statusFlag);

/** \brief The function returns the alarm Executed status of requested alarm mechanism
 * \param alarmEx alarm mechanism
 * \return alarm executed status of requested alarm mechanism
 */
IFX_INLINE boolean IfxSmu_getAlarmExecutedStatus(IfxSmu_AlarmExecutionStatus alarmEx);

/** \brief Function returns the Port control hardware direction.
 * \return port Conrol HW direction
 */
IFX_INLINE IfxSmu_PortControlHwDir IfxSmu_getPortControlHwDir(void);

/** \brief Function returns the Port control hardware enable.
 * \return port Control HW enable
 */
IFX_INLINE IfxSmu_PortControlHwEnable IfxSmu_getPortControlHwEnable(void);

/** \brief Function returns the value ofRegister Monitor Error Flags register.
 * \return Returns the  Register Monitor Error Flags register value.
 */
IFX_INLINE uint32 IfxSmu_getRegisterMonitorErrorFlag(void);

/** \brief Function returns the value of Register Monitor Self Test Status register.
 * \return Returns the Register monitor self Test Status register value.
 */
IFX_INLINE uint32 IfxSmu_getRegisterMonitorStatus(void);

/** \brief Returns the module's suspend state.
 * TRUE :if module is suspended.
 * FALSE:if module is not yet suspended.
 * \param smu Pointer to SMU module registers
 * \return Suspend status (TRUE / FALSE)
 */
IFX_INLINE boolean IfxSmu_isModuleSuspended(Ifx_SMU *smu);

/** \brief Function sets the Port control hardware Enable and direction.
 *
 * The PCTL register needs to be unlocked by using IfxSmu_unlockConfigRegisters() before calling this function.
 * \param portEnable port enable
 * \param hwDir Hardware direction
 * \return None
 */
IFX_INLINE void IfxSmu_setPortControlHwEnableAndDir(IfxSmu_PortControlHwEnable portEnable, IfxSmu_PortControlHwDir hwDir);

/** \brief Function sets the Register monitor test mode enable Flag.
 *
 * The RMCTL register needs to be unlocked by using IfxSmu_unlockConfigRegisters() before calling this function.
 * \param testModeEnable Test Mode Flag
 * \return None
 */
IFX_INLINE void IfxSmu_setRegMonTestModeEnable(uint8 testModeEnable);

/** \brief Configure the Module to Hard/Soft suspend mode.
 * Note: The api works only when the OCDS is enabled and in Supervisor Mode. When OCDS is disabled the OCS suspend control is ineffective.
 * \param smu Pointer to SMU module registers
 * \param mode Module suspend mode
 * \return None
 */
IFX_INLINE void IfxSmu_setSuspendMode(Ifx_SMU *smu, IfxSmu_SuspendMode mode);

/** \brief This function starts the SMU alive test
 * \return None
 */
IFX_INLINE void IfxSmu_startAliveTest(void);

/** \brief This function Stops the SMU alive test
 * \return None
 */
IFX_INLINE void IfxSmu_stopAliveTest(void);

/** \brief This function waits for SMU alarm to be set and then clears it..
 * \param alarm smu alarm group and position
 * \return None
 */
IFX_INLINE void IfxSmu_waitForAlarm(IfxSmu_Alarm alarm);

/** \brief Returns The SRCR pointer for the selected Interrupt
 * \param intRequest Interrupt Request
 * \return Pointer to SRCR register
 */
IFX_INLINE volatile Ifx_SRC_SRCR *IfxSmu_getSrcPointer(IfxSmu_InterruptRequest intRequest);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief This function activates the FSP to indicate a FAULT state on the error pin to the safe state switching device.
 * \return None
 */
IFX_EXTERN void IfxSmu_activateFSP(void);

/** \brief The function allows to switch the SMU peripheral into the  RUN fault-free state as requested by the caller.
 * \return None
 */
IFX_EXTERN void IfxSmu_activateRunState(void);

/** \brief This function clears SMU alarm status of the requested alarm. It needs to be called after alarm status is read.
 * \param alarm smu alarm group and position
 * \return None
 */
IFX_EXTERN void IfxSmu_clearAlarmStatus(IfxSmu_Alarm alarm);

/** \brief This function configures the PES feature for internal action.
 * Bit0/1/2/3 - SMU_IGCS0/SMU_IGCS1/SMU_IGCS2/SMU_NMI/SMU_CPU_RESET activates PES correspondingly.
 *
 * The AGC register needs to be unlocked by using IfxSmu_unlockConfigRegisters() before calling this function.
 * \param pesAction PES action bit0/1/2/3 - SMU_IGCS0/SMU_IGCS1/SMU_IGCS2/SMU_NMI/SMU_CPU_RESET activates PES correspondingly.
 * \return None
 */
IFX_EXTERN void IfxSmu_configAlarmActionPES(uint8 pesAction);

/** \brief Enable FAULT to RUN State Transition.
 *
 * The AGC register needs to be unlocked by using IfxSmu_unlockConfigRegisters() before calling this function.
 * \param enable Enable / Disable Fault to Run Transition ( True - Enable, False - Disable)
 * \return None
 */
IFX_EXTERN void IfxSmu_enableFaultToRunState(boolean enable);

/** \brief Configure FSP (fault Signalling Protocol) Port Emergency Stop.
 *
 * The FSP register needs to be unlocked by using IfxSmu_unlockConfigRegisters() before calling this function.
 * \param enable Enable / Disable PES ( True - Enable, False - Disable)
 * \return None
 */
IFX_EXTERN void IfxSmu_enablePortEmergencyStop(boolean enable);

/** \brief The function provides the FSP action currently configured for the requested alarm.
 * \param alarm smu alarm group and position
 * \param intAlarmAction internal alarm action
 * \return FSP enabled/disabled for the particular alarm
 */
IFX_EXTERN boolean IfxSmu_getAlarmAction(IfxSmu_Alarm alarm, IfxSmu_InternalAlarmAction *intAlarmAction);

/** \brief The function provides the alarm group status from the stored debug  registers
 * \param alarmGroup smu alarm group
 * \return alarm group status from the stored debug registers
 */
IFX_EXTERN uint32 IfxSmu_getAlarmGroupDebugStatus(uint8 alarmGroup);

/** \brief The function returns the alarm status of requested alarm.
 * \param alarm smu alarm group and position
 * \return alarm status of requested alarm.
 */
IFX_EXTERN boolean IfxSmu_getAlarmStatus(IfxSmu_Alarm alarm);

/** \brief The function returns if any alarms requiring the requested recovery timer was SET while the recovery timer was running.
 * \param timerNum Recovery Timer unit
 * \return returns status of missed event (0 - no missed event, 1 - missed event)
 */
IFX_EXTERN boolean IfxSmu_getRTMissedEvent(uint8 timerNum);

/** \brief The function provides the current state of the SMU.
 * \return return the current state of SMU
 */
IFX_EXTERN IfxSmu_SmuState IfxSmu_getSmuState(void);

/** \brief The function locks the SMU configuration registers to prevent any modification to configuration register content
 * \return None
 */
IFX_EXTERN void IfxSmu_lockConfigRegisters(void);

/** \brief The function switches the SMU peripheral from the FAULT state to the RUN state.
 * \return None
 */
IFX_EXTERN void IfxSmu_releaseFSP(void);

/** \brief The function sets the alarm action for the desired alarm.
 * \param alarm smu alarm group and position
 * \param intAlarmAction internal alarm action
 * \return None
 */
IFX_EXTERN void IfxSmu_setAlarmAction(IfxSmu_Alarm alarm, IfxSmu_InternalAlarmAction intAlarmAction);

/** \brief The API sets the requested alarm.
 * This function can be used by the user software to trigger SW SMU alarm. During the START state of the SMU, it shall be possible to set any of the alarms. However,   during the RUN state, only the SW alarms shall be set
 * \param alarm smu alarm group and position
 * \return None
 */
IFX_EXTERN void IfxSmu_setAlarmStatus(IfxSmu_Alarm alarm);

/** \brief Configure FSP (fault Signalling Protocol) mode.
 *
 * The FSP register needs to be unlocked by using IfxSmu_unlockConfigRegisters() before calling this function.
 * \param mode FSP mode
 * \return None
 */
IFX_EXTERN void IfxSmu_setFspMode(IfxSmu_FspMode mode);

/** \brief The API stops the requested recovery timer unit.
 * \param timerNum Recovery Timer unit to be stopped
 * \return None
 */
IFX_EXTERN void IfxSmu_stopRT(uint8 timerNum);

/** \brief The function unlocks the SMU configuration registers for modification.
 * \return None
 */
IFX_EXTERN void IfxSmu_unlockConfigRegisters(void);

/** \brief Configures The Interrupt Generation
 * \param config Configuration Selection
 * \param intRequest Interrupt Request
 * \return None
 */
IFX_EXTERN void IfxSmu_ConfigureInterruptGeneration(IfxSmu_InterruptGenerationConfiguration config, IfxSmu_InterruptRequest intRequest);

/** \brief The function temporarily locks the SMU configuration registers for modification.
 * \return None
 */
IFX_EXTERN void IfxSmu_temporaryLockConfigRegisters(void);

/** \} */

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE void IfxSmu_clearAlarmExecutedStatus(IfxSmu_AlarmExecutionStatus alarmEx)
{
    uint16 passwd = IfxScuWdt_getSafetyWatchdogPassword();

    /* disable the write-protection for registers */
    IfxScuWdt_clearSafetyEndinit(passwd);

    /* Write 1 in AEX bit to clear alarm Executed mechanism */
    MODULE_SMU.AEXCLR.U = (uint32)((uint32)0x1U << alarmEx);

    /* Restore back the write-protection for registers */
    IfxScuWdt_setSafetyEndinit(passwd);
}


IFX_INLINE void IfxSmu_clearRegMonTestModeEnable(uint8 testModeEnable)
{
    uint16 passwd = IfxScuWdt_getSafetyWatchdogPassword();

    /* disable the write-protection for registers */
    IfxScuWdt_clearSafetyEndinit(passwd);

    MODULE_SMU.RMCTL.U &= ~(1U << testModeEnable);
    /* Restore back the write-protection for registers */
    IfxScuWdt_setSafetyEndinit(passwd);
}


IFX_INLINE void IfxSmu_clearRegisterMonitorErrorFlag(uint8 errorFlag)
{
    uint16 passwd = IfxScuWdt_getSafetyWatchdogPassword();

    /* disable the write-protection for registers */
    IfxScuWdt_clearSafetyEndinit(passwd);

    MODULE_SMU.RMEF.U &= ~(1U << errorFlag);

    /* Restore back the write-protection for registers */
    IfxScuWdt_setSafetyEndinit(passwd);
}


IFX_INLINE void IfxSmu_clearRegisterMonitorStatus(uint8 statusFlag)
{
    uint16 passwd = IfxScuWdt_getSafetyWatchdogPassword();

    /* disable the write-protection for registers */
    IfxScuWdt_clearSafetyEndinit(passwd);

    MODULE_SMU.RMSTS.U &= ~(1U << statusFlag);

    /* Restore back the write-protection for registers */
    IfxScuWdt_setSafetyEndinit(passwd);
}


IFX_INLINE boolean IfxSmu_getAlarmExecutedStatus(IfxSmu_AlarmExecutionStatus alarmEx)
{
    boolean alarmExecuteStatus = (boolean)((MODULE_SMU.AEX.U >> alarmEx) & 0x1);
    return alarmExecuteStatus;
}


IFX_INLINE IfxSmu_PortControlHwDir IfxSmu_getPortControlHwDir(void)
{
    return (IfxSmu_PortControlHwDir)MODULE_SMU.PCTL.B.HWDIR;
}


IFX_INLINE IfxSmu_PortControlHwEnable IfxSmu_getPortControlHwEnable(void)
{
    return (IfxSmu_PortControlHwEnable)MODULE_SMU.PCTL.B.HWEN;
}


IFX_INLINE uint32 IfxSmu_getRegisterMonitorErrorFlag(void)
{
    uint32 errorFlags = 0;
    errorFlags = MODULE_SMU.RMEF.U;
    return errorFlags;
}


IFX_INLINE uint32 IfxSmu_getRegisterMonitorStatus(void)
{
    uint32 selfTestStatus = 0;
    selfTestStatus = MODULE_SMU.RMSTS.U;
    return selfTestStatus;
}


IFX_INLINE boolean IfxSmu_isModuleSuspended(Ifx_SMU *smu)
{
    Ifx_SMU_OCS ocs;

    // read the status
    ocs.U = smu->OCS.U;

    // return the status
    return ocs.B.SUSSTA;
}


IFX_INLINE void IfxSmu_setPortControlHwEnableAndDir(IfxSmu_PortControlHwEnable portEnable, IfxSmu_PortControlHwDir hwDir)
{
    uint16       passwd = IfxScuWdt_getSafetyWatchdogPassword();
    Ifx_SMU_PCTL pctl;
    /* disable the write-protection for registers */
    IfxScuWdt_clearSafetyEndinit(passwd);

    pctl.U            = MODULE_SMU.PCTL.U;
    pctl.B.HWEN       = portEnable;
    pctl.B.HWDIR      = hwDir;
    pctl.B.PCS        = 1;
    MODULE_SMU.PCTL.U = pctl.U;

    /* Restore back the write-protection for registers */
    IfxScuWdt_setSafetyEndinit(passwd);
}


IFX_INLINE void IfxSmu_setRegMonTestModeEnable(uint8 testModeEnable)
{
    uint16 passwd = IfxScuWdt_getSafetyWatchdogPassword();

    /* disable the write-protection for registers */
    IfxScuWdt_clearSafetyEndinit(passwd);

    MODULE_SMU.RMCTL.U |= (1U << testModeEnable);
    /* Restore back the write-protection for registers */
    IfxScuWdt_setSafetyEndinit(passwd);
}


IFX_INLINE void IfxSmu_setSuspendMode(Ifx_SMU *smu, IfxSmu_SuspendMode mode)
{
    Ifx_SMU_OCS ocs;

    // remove protection and configure the suspend mode.
    ocs.B.SUS_P = 1;
    ocs.B.SUS   = mode;
    smu->OCS.U  = ocs.U;
}


IFX_INLINE void IfxSmu_startAliveTest(void)
{
    Ifx_SMU_CMD cmdSfr;
    cmdSfr.U         = MODULE_SMU.CMD.U;
    cmdSfr.B.CMD     = IfxSmu_Command_aliveTest;
    cmdSfr.B.ARG     = IFXSMU_START_ALIVE_TEST_ARG;
    MODULE_SMU.CMD.U = cmdSfr.U;
}


IFX_INLINE void IfxSmu_stopAliveTest(void)
{
    Ifx_SMU_CMD cmdSfr;
    cmdSfr.U         = MODULE_SMU.CMD.U;
    cmdSfr.B.CMD     = IfxSmu_Command_aliveTest;
    cmdSfr.B.ARG     = IFXSMU_STOP_ALIVE_TEST_ARG;
    MODULE_SMU.CMD.U = cmdSfr.U;
}


IFX_INLINE void IfxSmu_waitForAlarm(IfxSmu_Alarm alarm)
{
    while (!IfxSmu_getAlarmStatus(alarm))
    {}

    IfxSmu_clearAlarmStatus(alarm);
}


IFX_INLINE volatile Ifx_SRC_SRCR *IfxSmu_getSrcPointer(IfxSmu_InterruptRequest intRequest)
{
    return &MODULE_SRC.SMU.SMU[0].SR[intRequest];
}


#endif /* IFXSMU_H */
