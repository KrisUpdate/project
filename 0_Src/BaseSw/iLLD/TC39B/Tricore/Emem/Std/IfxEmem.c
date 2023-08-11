/**
 * \file IfxEmem.c
 * \brief EMEM  basic functionality
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
 */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "IfxEmem.h"
#include "Cpu/Std/IfxCpu_Intrinsics.h"

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

IfxEmem_LockedState IfxEmem_getLockedState(void)
{
    return (IfxEmem_LockedState)MODULE_EMEM.SBRCTR.B.STBLOCK;
}


void IfxEmem_setClockEnableState(const IfxEmem_State state)
{
    /* bit is inverted */
    if (IfxEmem_State_enabled == state)
    {
        MODULE_EMEM.CLC.B.DISR = 0;
    }
    else
    {
        MODULE_EMEM.CLC.B.DISR = 1;
    }

    /* wait one cycle for module to be enabled */
    __nop();
}


void IfxEmem_setUnlockMode(Ifx_EMEM *ememCore)
{
    ememCore->SBRCTR.U = 0x2;
    ememCore->SBRCTR.U = 0x6;
    ememCore->SBRCTR.U = 0xE;
}


void IfxEmem_enableModule(Ifx_EMEM *ememCore)
{
    uint16 psw = IfxScuWdt_getCpuWatchdogPassword();
    IfxScuWdt_clearCpuEndinit(psw); // clears the endinit protection
    ememCore->CLC.B.DISR = 0;       // enables the module
    IfxScuWdt_setCpuEndinit(psw);   // sets the endinit protection back on
}


void IfxEmem_disableModule(Ifx_EMEM *ememCore)
{
    uint16 psw = IfxScuWdt_getCpuWatchdogPassword();
    IfxScuWdt_clearCpuEndinit(psw); // clears the endinit protection
    ememCore->CLC.B.DISR = 1;       // disables the module
    IfxScuWdt_setCpuEndinit(psw);   // sets the endinit protection back on
}


void IfxEmem_disableEccErrorReporting(IfxEmem_MpuIndex mpuIndex)
{
    uint16        psw = IfxScuWdt_getCpuWatchdogPassword();
    IfxScuWdt_clearCpuEndinit(psw); // clears the endinit protection

    Ifx_EMEM_MPU *ememMpu = IfxEmem_getAddress(mpuIndex);

    ememMpu->MEMCON.U = ((ememMpu->MEMCON.U & ~(3 << 8)) | ((1 << 8) | (1 << 9)));

    IfxScuWdt_setCpuEndinit(psw);   // sets the endinit protection back on
}


void IfxEmem_enableEccErrorReporting(IfxEmem_MpuIndex mpuIndex)
{
    uint16        psw = IfxScuWdt_getCpuWatchdogPassword();
    IfxScuWdt_clearCpuEndinit(psw); // clears the endinit protection

    Ifx_EMEM_MPU *ememMpu = IfxEmem_getAddress(mpuIndex);

    ememMpu->MEMCON.U = ((ememMpu->MEMCON.U & ~(3 << 8)) | (1 << 8));

    IfxScuWdt_setCpuEndinit(psw);   // sets the endinit protection back on
}


IfxEmem_MpuIndex IfxEmem_getIndex(Ifx_EMEM_MPU *ememMpu)
{
    uint32           index = 0;
    IfxEmem_MpuIndex result;

    result = IfxEmem_MpuIndex_none;

    for (index = 0; index < IFXEMEM_NUM_MPU_MODULES; index++)
    {
        if (IfxEmem_cfg_indexMap[index].module == ememMpu)
        {
            result = (IfxEmem_MpuIndex)IfxEmem_cfg_indexMap[index].index;
            break;
        }
    }

    return result;
}


Ifx_EMEM_MPU *IfxEmem_getAddress(IfxEmem_MpuIndex ememMpu)
{
    Ifx_EMEM_MPU *module = NULL_PTR;

    if (ememMpu < IFXEMEM_NUM_MPU_MODULES)
    {
        module = (Ifx_EMEM_MPU *)IfxEmem_cfg_indexMap[ememMpu].module;
    }
    else
    {
        module = NULL_PTR;
    }

    return module;
}
