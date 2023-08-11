/**
 * \file IfxSmuStdby.h
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
 * \defgroup IfxLld_Smu_Std_Stdby STDBY
 * \ingroup IfxLld_Smu_Std
 * \defgroup IfxLld_Smu_Std_Stdby_Enumerations Enumerations
 * \ingroup IfxLld_Smu_Std_Stdby
 * \defgroup IfxLld_Smu_Std_Stdby_StdByFunctions StdBy Functions
 * \ingroup IfxLld_Smu_Std_Stdby
 */

#ifndef IFXSMUSTDBY_H
#define IFXSMUSTDBY_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "_Impl/IfxSmu_cfg.h"
#include "IfxSmu_reg.h"
#include "Scu/Std/IfxScuWdt.h"
#include "_Utilities/Ifx_Assert.h"
#include "IfxPms_reg.h"

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Smu_Std_Stdby_Enumerations
 * \{ */
/** \brief This bit  (CMD_STDBY.B.ASCE) controls if a status flag set in an AGx register upon detection of the alarm event can be cleared by software or not. When ASCE is enabled, software shall write a 1 to bit position in AGx to clear the bit
 * (W1C). When a W1C action takes place the ASCE bit is automatically cleared to 0 by hardware and software shall set the ASCE bit again.
 * 0B SMU_stdby alarm status bits in AGi cannot be cleared.
 * 1B SMU_stdby alarm status bits in AGi can be cleared
 */
typedef enum
{
    IfxSmuStdby_AlarmStatusClear_disable = 0,  /**< \brief SMU_stdby alarm status bits in AGi cannot be cleared. */
    IfxSmuStdby_AlarmStatusClear_enable  = 1   /**< \brief SMU_stdby alarm status bits in AGi can be cleared */
} IfxSmuStdby_AlarmStatusClear;

/** \brief Status flag for alarm (AG2i_STDBY.B.SFz)
 * 0B Status flag  does not report a fault condition
 * 1B Status flag reports a fault condition
 */
typedef enum
{
    IfxSmuStdby_AlarmStatusFlag_noFaultExist = 0,  /**< \brief Status flag does not report a fault condition */
    IfxSmuStdby_AlarmStatusFlag_faultExist   = 1   /**< \brief Status flag reports a fault condition */
} IfxSmuStdby_AlarmStatusFlag;

/** \brief Fault signaling configuration flag (AG2iFSP_STDBY.B.FEz)
 */
typedef enum
{
    IfxSmuStdby_FaultSignalAlarmConfigFlagEvent_disable = 0,  /**< \brief FSP disabled for this alarm event */
    IfxSmuStdby_FaultSignalAlarmConfigFlagEvent_enable  = 1   /**< \brief FSP enabled for this alarm event. */
} IfxSmuStdby_FaultSignalAlarmConfigFlagEvent;

/** \brief Error Pin Fault State Status  (AG2i_STDBY.B.FSPERR)
 */
typedef enum
{
    IfxSmuStdby_FspErrorPinFaultState_noFault = 0,  /**< \brief Error pin was not set into fault state */
    IfxSmuStdby_FspErrorPinFaultState_fault   = 1   /**< \brief The Error pin was set into fault state */
} IfxSmuStdby_FspErrorPinFaultState;

/** \brief SMU_stdby Error pin function to be able set Pin to fault state. (CMD_STDBY.B.FSP0EN or CMD_STDBY.B.FSP1EN)
 */
typedef enum
{
    IfxSmuStdby_FspErrorPinState_inactive = 0,  /**< \brief SMU_stdby Error Pin fault indication function on pin inactive. */
    IfxSmuStdby_FspErrorPinState_active   = 1   /**< \brief SMU_stdby Error Pin fault indication function on pin inactive. */
} IfxSmuStdby_FspErrorPinState;

/** \brief SMU_stdby Module Enable(CMD_STDBY.B.SMUEN)
 */
typedef enum
{
    IfxSmuStdby_SmuStdbyModuleState_disable = 0,  /**< \brief SMU_stdby disabled. */
    IfxSmuStdby_SmuStdbyModuleState_enable  = 1   /**< \brief SMU_stdby enabled. */
} IfxSmuStdby_SmuStdbyModuleState;

/** \} */

/** \addtogroup IfxLld_Smu_Std_Stdby_StdByFunctions
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief This function clears the TSTEN, TSTRUN, TSTDONE, TSTOK, SMUERR and PMSERR flags
 * \return None
 */
IFX_INLINE void IfxSmuStdby_clearSmuStdbyMonBistFlags(void);

/** \brief This function enable / disables he Alarm status clear in the Smu Standby command register .
 * This controls if a status flag set in an AGx register upon detection of the alarm event can be cleared by software or not. When ASCE is enabled, software shall write a 1 to bit position in AGx to clear the bit
 * (W1C). When a W1C action takes place the ASCE bit is automatically cleared to 0 by hardware and software shall set the ASCE bit again.
 * \return None
 */
IFX_INLINE void IfxSmuStdby_enableAlarmStatusClear(void);

/** \brief This function enables the Smu Standby module
 * \param enable SMU standby enable/disable Module
 * \return None
 */
IFX_INLINE void IfxSmuStdby_enableSmuStdby(IfxSmuStdby_SmuStdbyModuleState enable);

/** \brief This function enables the Smu standby Built-in Self test
 * \return None
 */
IFX_INLINE void IfxSmuStdby_enableSmuStdbyMonBist(void);

/** \brief Funtion returns Smu Standby Alarm Group status
 * \param alarmGroup Smu standby Alarm group
 * \return Smu standby status
 */
IFX_INLINE uint32 IfxSmuStdby_getSmuStdbyAlarmGroupStatus(uint8 alarmGroup);

/** \brief Funtion returns Smu Standby Alarm status
 * \param alarmGroup Smu standby Alarm group
 * \param alarmNum alarm number
 * \return Smu standby status
 */
IFX_INLINE uint32 IfxSmuStdby_getSmuStdbyAlarmStatus(uint8 alarmGroup, uint8 alarmNum);

/** \brief This function returns the SMU standby module status (enabled/disabled).
 * \return Smu standby status
 */
IFX_INLINE IfxSmuStdby_SmuStdbyModuleState IfxSmuStdby_getSmuStdbyModuleStatus(void);

/** \brief This function returns the Smu Stdby BIST PMSERR flag
 * \return Smu Stdby BIST PMSERR flag
 */
IFX_INLINE boolean IfxSmuStdby_getSmuStdbyMonBistPmsErrorFlag(void);

/** \brief This function returns the Smu Stdby BIST SMUERR flag
 * \return Smu Stdby BIST SMUERR flag
 */
IFX_INLINE boolean IfxSmuStdby_getSmuStdbyMonBistSmuErrorFlag(void);

/** \brief This function returns the Smu Stdby BIST TSTDONE flag
 * \return Smu Stdby BIST TSTDONE flag
 */
IFX_INLINE boolean IfxSmuStdby_getSmuStdbyMonBistTestDoneFlag(void);

/** \brief This function returns the Smu Stdby BIST TSTOK flag
 * \return Smu Stdby BIST TSTOK flag
 */
IFX_INLINE boolean IfxSmuStdby_getSmuStdbyMonBistTestOkFlag(void);

/** \brief This function returns the Smu Stdby BIST TSTRUN flag
 * \return Smu Stdby BIST TSTRUN flag
 */
IFX_INLINE boolean IfxSmuStdby_getSmuStdbyMonBistTestRunFlag(void);

/** \brief This function sets FSP0 Error pin fault indication function .
 * \param active Set FSP0 Error pin fault indication function to active / inactive
 * \return None
 */
IFX_INLINE void IfxSmuStdby_setFsp0ErrorPinActive(IfxSmuStdby_FspErrorPinState active);

/** \brief This function sets FSP1 Error pin fault indication function.
 * \param active Set FSP1 Error pin fault indication function to active / inactive
 * \return None
 */
IFX_INLINE void IfxSmuStdby_setFsp1ErrorPinActive(IfxSmuStdby_FspErrorPinState active);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Function to configure FSP Alarm group functionality
 * \param alarmGroup alarm group
 * \param flags Fault Signal Alarm Group config event flags
 * \return None
 */
IFX_EXTERN void IfxSmuStdby_setFaultSignalAGConfigEventFlags(uint8 alarmGroup, uint32 flags);

/** \brief Function to configure FSP functionality , FaultSignalAlarmConfigEventFlag (AG2iFSP_STDBY.B.FEz)
 * \param alarmGroup alarm group
 * \param alarmNum alarm number
 * \param enable enable/disable Fault Signal Alarm config event flag
 * \return None
 */
IFX_EXTERN void IfxSmuStdby_setFaultSignalAlarmConfigEventFlag(uint8 alarmGroup, uint8 alarmNum, IfxSmuStdby_FaultSignalAlarmConfigFlagEvent enable);

/** \brief Function to configure alarm status. (AG2i_STDBY.B.SFz)
 * \param alarmGroup alarm group
 * \param alarmNum alarm number
 * \param status enable/disable Alarm status flag
 * \return None
 */
IFX_EXTERN void IfxSmuStdby_setSmuStdbyAlarmStatusFlag(uint8 alarmGroup, uint8 alarmNum, IfxSmuStdby_AlarmStatusFlag status);

/** \brief This function enables the Smu standby Built-in Self test
 * \return None
 */
IFX_EXTERN void IfxSmuStdby_startSmuStdbyMonBist(void);

/** \} */

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE void IfxSmuStdby_clearSmuStdbyMonBistFlags(void)
{
    Ifx_PMS_MONBISTCTRL monBistCtrl;
    uint16              passwd = IfxScuWdt_getSafetyWatchdogPassword();

    /* disable the write-protection for registers */
    IfxScuWdt_clearSafetyEndinit(passwd);

    monBistCtrl.U         = PMS_MONBISTCTRL.U;
    monBistCtrl.B.TSTCLR  = 1;
    monBistCtrl.B.BITPROT = 1;
    PMS_MONBISTCTRL.U     = monBistCtrl.U;

    /* Restore back the write-protection for registers */
    IfxScuWdt_setSafetyEndinit(passwd);
}


IFX_INLINE void IfxSmuStdby_enableAlarmStatusClear(void)
{
    Ifx_PMS_CMD_STDBY cmdStdby;
    uint16            passwd = IfxScuWdt_getSafetyWatchdogPassword();
    /* disable the write-protection for registers */
    IfxScuWdt_clearSafetyEndinit(passwd);
    cmdStdby.U         = PMS_CMD_STDBY.U;
    cmdStdby.B.ASCE    = 1;
    cmdStdby.B.BITPROT = 1;
    PMS_CMD_STDBY.U    = cmdStdby.U;
    /* Restore back the write-protection for registers */
    IfxScuWdt_setSafetyEndinit(passwd);
}


IFX_INLINE void IfxSmuStdby_enableSmuStdby(IfxSmuStdby_SmuStdbyModuleState enable)
{
    Ifx_PMS_CMD_STDBY cmdStdby;
    uint16            passwd = IfxScuWdt_getSafetyWatchdogPassword();
    /* disable the write-protection for registers */
    IfxScuWdt_clearSafetyEndinit(passwd);

    cmdStdby.U         = PMS_CMD_STDBY.U;
    cmdStdby.B.SMUEN   = enable;
    cmdStdby.B.BITPROT = 1;
    PMS_CMD_STDBY.U    = cmdStdby.U;

    /* Restore back the write-protection for registers */
    IfxScuWdt_setSafetyEndinit(passwd);
}


IFX_INLINE void IfxSmuStdby_enableSmuStdbyMonBist(void)
{
    Ifx_PMS_MONBISTCTRL monBistCtrl;
    uint16              passwd = IfxScuWdt_getSafetyWatchdogPassword();
    /* disable the write-protection for registers */
    IfxScuWdt_clearSafetyEndinit(passwd);

    monBistCtrl.U         = PMS_MONBISTCTRL.U;
    monBistCtrl.B.TSTEN   = 1;
    monBistCtrl.B.BITPROT = 1;
    PMS_MONBISTCTRL.U     = monBistCtrl.U;

    /* Restore back the write-protection for registers */
    IfxScuWdt_setSafetyEndinit(passwd);
}


IFX_INLINE uint32 IfxSmuStdby_getSmuStdbyAlarmGroupStatus(uint8 alarmGroup)
{
    uint32 alarmGroupStatus = 0;

    if (alarmGroup == 20U)
    {
        alarmGroupStatus = PMS_AG_STDBY0.U & 0x0000FFFFU;
    }
    else if (alarmGroup == 21U)
    {
        alarmGroupStatus = PMS_AG_STDBY1.U;
    }
    else
    {
        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, FALSE);
    }

    return alarmGroupStatus;
}


IFX_INLINE uint32 IfxSmuStdby_getSmuStdbyAlarmStatus(uint8 alarmGroup, uint8 alarmNum)
{
    uint32 alarmStatus = 0;

    if (alarmGroup == 20U)
    {
        alarmStatus = (PMS_AG_STDBY0.U >> alarmNum) & 0x01U;
    }
    else if (alarmGroup == 21U)
    {
        alarmStatus = (PMS_AG_STDBY1.U >> alarmNum) & 0x01U;
    }
    else
    {
        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, FALSE);
    }

    return alarmStatus;
}


IFX_INLINE IfxSmuStdby_SmuStdbyModuleState IfxSmuStdby_getSmuStdbyModuleStatus(void)
{
    return (IfxSmuStdby_SmuStdbyModuleState)PMS_CMD_STDBY.B.SMUEN;
}


IFX_INLINE boolean IfxSmuStdby_getSmuStdbyMonBistPmsErrorFlag(void)
{
    return PMS_MONBISTSTAT.B.PMSERR;
}


IFX_INLINE boolean IfxSmuStdby_getSmuStdbyMonBistSmuErrorFlag(void)
{
    return PMS_MONBISTSTAT.B.SMUERR;
}


IFX_INLINE boolean IfxSmuStdby_getSmuStdbyMonBistTestDoneFlag(void)
{
    return PMS_MONBISTSTAT.B.TSTDONE;
}


IFX_INLINE boolean IfxSmuStdby_getSmuStdbyMonBistTestOkFlag(void)
{
    return PMS_MONBISTSTAT.B.TSTOK;
}


IFX_INLINE boolean IfxSmuStdby_getSmuStdbyMonBistTestRunFlag(void)
{
    return PMS_MONBISTSTAT.B.TSTRUN;
}


IFX_INLINE void IfxSmuStdby_setFsp0ErrorPinActive(IfxSmuStdby_FspErrorPinState active)
{
    Ifx_PMS_CMD_STDBY cmdStdby;
    uint16            passwd = IfxScuWdt_getSafetyWatchdogPassword();
    /* disable the write-protection for registers */
    IfxScuWdt_clearSafetyEndinit(passwd);

    cmdStdby.U         = PMS_CMD_STDBY.U;
    cmdStdby.B.FSP0EN  = (boolean)active;
    cmdStdby.B.BITPROT = 1;
    PMS_CMD_STDBY.U    = cmdStdby.U;
    /* Restore back the write-protection for registers */
    IfxScuWdt_setSafetyEndinit(passwd);
}


IFX_INLINE void IfxSmuStdby_setFsp1ErrorPinActive(IfxSmuStdby_FspErrorPinState active)
{
    Ifx_PMS_CMD_STDBY cmdStdby;
    uint16            passwd = IfxScuWdt_getSafetyWatchdogPassword();
    /* disable the write-protection for registers */
    IfxScuWdt_clearSafetyEndinit(passwd);

    cmdStdby.U         = PMS_CMD_STDBY.U;
    cmdStdby.B.FSP1EN  = (boolean)active;
    cmdStdby.B.BITPROT = 1;
    PMS_CMD_STDBY.U    = cmdStdby.U;
    /* Restore back the write-protection for registers */
    IfxScuWdt_setSafetyEndinit(passwd);
}


#endif /* IFXSMUSTDBY_H */
