/**
 * \file IfxSmuStdby.c
 * \brief SMU  basic functionality
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
 */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "IfxSmuStdby.h"

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

void IfxSmuStdby_setFaultSignalAGConfigEventFlags(uint8 alarmGroup, uint32 flags)
{
    Ifx_PMS_AGFSP_STDBY0 AgfspStdby20;
    Ifx_PMS_AGFSP_STDBY1 AgfspStdby21;
    uint16               passwd = IfxScuWdt_getSafetyWatchdogPassword();
    /* disable the write-protection for registers */
    IfxScuWdt_clearSafetyEndinit(passwd);

    if (alarmGroup == 20U)
    {
        AgfspStdby20.U         = PMS_AGFSP_STDBY0.U;
        AgfspStdby20.U         = flags & 0x0000FFFFU;
        AgfspStdby20.B.BITPROT = 1;
        PMS_AGFSP_STDBY0.U     = AgfspStdby20.U;
    }
    else if (alarmGroup == 21U)
    {
        AgfspStdby21.U         = PMS_AGFSP_STDBY1.U;
        AgfspStdby21.U         = flags & 0x0001FFFFU;
        AgfspStdby21.B.BITPROT = 1;
        PMS_AGFSP_STDBY1.U     = AgfspStdby21.U;
    }
    else
    {
        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, FALSE);
    }

    /* Restore back the write-protection for registers */
    IfxScuWdt_setSafetyEndinit(passwd);
}


void IfxSmuStdby_setFaultSignalAlarmConfigEventFlag(uint8 alarmGroup, uint8 alarmNum, IfxSmuStdby_FaultSignalAlarmConfigFlagEvent enable)
{
    Ifx_PMS_AGFSP_STDBY0 AgfspStdby20;
    Ifx_PMS_AGFSP_STDBY1 AgfspStdby21;
    uint16               passwd = IfxScuWdt_getSafetyWatchdogPassword();
    /* disable the write-protection for registers */
    IfxScuWdt_clearSafetyEndinit(passwd);

    if (alarmGroup == 20U)
    {
        AgfspStdby20.U = PMS_AGFSP_STDBY0.U;

        if (enable)
        {
            AgfspStdby20.U |= 1U << alarmNum;
        }
        else
        {
            AgfspStdby20.U &= ~(1U << alarmNum);
        }

        AgfspStdby20.B.BITPROT = 1;
        PMS_AGFSP_STDBY0.U     = AgfspStdby20.U;
    }
    else if (alarmGroup == 21U)
    {
        AgfspStdby21.U = PMS_AGFSP_STDBY1.U;

        if (enable)
        {
            AgfspStdby21.U |= 1U << alarmNum;
        }
        else
        {
            AgfspStdby21.U &= ~(1U << alarmNum);
        }

        AgfspStdby21.B.BITPROT = 1;
        PMS_AGFSP_STDBY1.U     = AgfspStdby21.U;
    }
    else
    {
        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, FALSE);
    }

    /* Restore back the write-protection for registers */
    IfxScuWdt_setSafetyEndinit(passwd);
}


void IfxSmuStdby_setSmuStdbyAlarmStatusFlag(uint8 alarmGroup, uint8 alarmNum, IfxSmuStdby_AlarmStatusFlag status)
{
    Ifx_PMS_AG_STDBY0 AgStdby20;
    Ifx_PMS_AG_STDBY1 AgStdby21;
    Ifx_PMS_CMD_STDBY cmdStdby;

    uint16            passwd = IfxScuWdt_getSafetyWatchdogPassword();
    /* disable the write-protection for registers */
    IfxScuWdt_clearSafetyEndinit(passwd);

    cmdStdby.U         = PMS_CMD_STDBY.U;
    cmdStdby.B.ASCE    = 1;
    cmdStdby.B.BITPROT = 1;
    PMS_CMD_STDBY.U    = cmdStdby.U;

    if (alarmGroup == 20U)
    {
        AgStdby20.U = PMS_AG_STDBY0.U;

        if (status == IfxSmuStdby_AlarmStatusFlag_faultExist)
        {
            AgStdby20.U |= 1U << alarmNum;
        }
        else
        {
            AgStdby20.U &= ~(1U << alarmNum);
        }

        PMS_AG_STDBY0.U = AgStdby20.U;
    }
    else if (alarmGroup == 21U)
    {
        AgStdby21.U = PMS_AG_STDBY1.U;

        if (status == IfxSmuStdby_AlarmStatusFlag_faultExist)
        {
            AgStdby21.U |= 1U << alarmNum;
        }
        else
        {
            AgStdby21.U &= ~(1U << alarmNum);
        }

        PMS_AG_STDBY1.U = AgStdby21.U;
    }
    else
    {
        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, FALSE);
    }

    /* Restore back the write-protection for registers */
    IfxScuWdt_setSafetyEndinit(passwd);
}


void IfxSmuStdby_startSmuStdbyMonBist(void)
{
    IfxSmuStdby_clearSmuStdbyMonBistFlags();
    uint16          passwd  = IfxScuWdt_getSafetyWatchdogPassword();
    volatile uint32 timeout = 0x1000U;
    /* disable the write-protection for registers */
    IfxScuWdt_clearSafetyEndinit(passwd);

    PMS_MONFILT.U = 0x20000000U;

    while ((PMS_MONFILT.U != 0x20000000U) && (timeout--)) // TO DO: Is wait needed? added based on SoC Ver request , to be confirmed from Concept
    {}

    PMS_MONCTRL.U = 0xa5a5a5U;
    timeout       = 0x1000U;

    while ((PMS_MONCTRL.U != 0xa5a5a5U) && (timeout--))  // TO DO: Is wait needed? added based on SoC Ver request , to be confirmed from Concept

    {}

    PMS_PMSIEN.U &= ~0x00000FFFU;
    /* Restore back the write-protection for registers */
    IfxScuWdt_setSafetyEndinit(passwd);

    IfxSmuStdby_setFaultSignalAGConfigEventFlags(20, 0U);
    IfxSmuStdby_setFaultSignalAGConfigEventFlags(21, 0U);
    IfxSmuStdby_setFsp0ErrorPinActive(IfxSmuStdby_FspErrorPinState_inactive);
    IfxSmuStdby_setFsp1ErrorPinActive(IfxSmuStdby_FspErrorPinState_inactive);
    IfxSmuStdby_enableAlarmStatusClear();
    IfxScuWdt_clearSafetyEndinit(passwd);
    PMS_MONFILT.U = 0x00000000U;
    IfxScuWdt_setSafetyEndinit(passwd);
    IfxSmuStdby_enableSmuStdbyMonBist();
}
