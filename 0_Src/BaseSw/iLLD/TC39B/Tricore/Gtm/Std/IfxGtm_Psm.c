/**
 * \file IfxGtm_Psm.c
 * \brief GTM  basic functionality
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
 */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "IfxGtm_Psm.h"

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

void IfxGtm_Psm_Fifo_setChannelEndAddress(IfxGtm_Psm_Fifo fifo, IfxGtm_Psm_FifoChannel channel, uint32 address)
{
    Ifx_GTM_PSM_FIFO_CH *fifoCh        = IfxGtm_Psm_Fifo_getChannelPointer(fifo, channel);
    uint32               start_address = IfxGtm_Psm_Fifo_getChannelStartAddress(fifo, channel);

    if ((address > start_address) &&
        (address < IFXGTM_PSM_FIFORAMSIZE))
    {
        fifoCh->END_ADDR.B.ADDR = (uint32)address;
    }
    else
    {
        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, FALSE);
    }
}


void IfxGtm_Psm_Fifo_setChannelStartAddress(IfxGtm_Psm_Fifo fifo, IfxGtm_Psm_FifoChannel channel, uint32 address)
{
    Ifx_GTM_PSM_FIFO_CH *fifoCh = IfxGtm_Psm_Fifo_getChannelPointer(fifo, channel);

    if (address < (IFXGTM_PSM_FIFORAMSIZE - 1))  /* START address should be within 0 - 1022 */
    {
        fifoCh->START_ADDR.B.ADDR = (uint32)address;
    }
    else
    {
        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, FALSE);
    }
}


void IfxGtm_Psm_Fifo_setChannelSize(IfxGtm_Psm_Fifo fifo, IfxGtm_Psm_FifoChannel channel, uint32 size)
{
    Ifx_GTM_PSM_FIFO_CH *fifoCh        = IfxGtm_Psm_Fifo_getChannelPointer(fifo, channel);
    uint32               start_address = IfxGtm_Psm_Fifo_getChannelStartAddress(fifo, channel);

    if (size < ((IFXGTM_PSM_FIFORAMSIZE - start_address) + (uint32)1))
    {
        fifoCh->END_ADDR.B.ADDR = (uint32)(start_address + size - (uint32)1);
    }
    else
    {
        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, FALSE);
    }
}


void IfxGtm_Psm_Fifo_setChannelUpperWatermark(IfxGtm_Psm_Fifo fifo, IfxGtm_Psm_FifoChannel channel, uint32 upperWm)
{
    Ifx_GTM_PSM_FIFO_CH *fifoCh = IfxGtm_Psm_Fifo_getChannelPointer(fifo, channel);
    uint32               size   = IfxGtm_Psm_Fifo_getChannelSize(fifo, channel);

    if (upperWm < (size))  /* should be within 0 - 1023 */
    {
        fifoCh->UPPER_WM.B.ADDR = (uint32)upperWm;
    }
    else
    {
        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, FALSE);
    }
}


void IfxGtm_Psm_Fifo_setChannelLowerWatermark(IfxGtm_Psm_Fifo fifo, IfxGtm_Psm_FifoChannel channel, uint32 lowerWm)
{
    Ifx_GTM_PSM_FIFO_CH *fifoCh = IfxGtm_Psm_Fifo_getChannelPointer(fifo, channel);
    uint32               size   = IfxGtm_Psm_Fifo_getChannelSize(fifo, channel);

    if (lowerWm < (size))  /* should be within 0 - 1023 */
    {
        fifoCh->LOWER_WM.B.ADDR = (uint32)lowerWm;
    }
    else
    {
        IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, FALSE);
    }
}


IfxGtm_Psm_FifoChannelStatus IfxGtm_Psm_Fifo_getChannelStatus(IfxGtm_Psm_Fifo fifo, IfxGtm_Psm_FifoChannel channel)
{
    Ifx_GTM_PSM_FIFO_CH         *fifoCh     = IfxGtm_Psm_Fifo_getChannelPointer(fifo, channel);
    IfxGtm_Psm_FifoChannelStatus status     = IfxGtm_Psm_FifoChannelStatus_normal;
    uint32                       status_reg = (uint32)fifoCh->STATUS.U;
    uint8                        index      = 0;

    while (index < (uint8)IfxGtm_Psm_FifoChannelStatus_normal)
    {
        if (((status_reg >> index) & (uint32)1) == (uint32)1)
        {
            status = (IfxGtm_Psm_FifoChannelStatus)index;
            break;
        }

        index++;
    }

    return status;
}


void IfxGtm_Psm_Fifo_enableChannelInterrupt(IfxGtm_Psm_Fifo fifo, IfxGtm_Psm_FifoChannel channel, IfxGtm_Psm_FifoChannelInterrupt interrupt)
{
    Ifx_GTM_PSM_FIFO_CH *fifoCh = IfxGtm_Psm_Fifo_getChannelPointer(fifo, channel);
    uint32              *irq_en = (uint32 *)&(fifoCh->IRQ.EN.U);

    *irq_en |= ((uint32)1 << (uint32)interrupt);
}


void IfxGtm_Psm_Fifo_disableChannelInterrupt(IfxGtm_Psm_Fifo fifo, IfxGtm_Psm_FifoChannel channel, IfxGtm_Psm_FifoChannelInterrupt interrupt)
{
    Ifx_GTM_PSM_FIFO_CH *fifoCh = IfxGtm_Psm_Fifo_getChannelPointer(fifo, channel);
    uint32              *irq_en = (uint32 *)&(fifoCh->IRQ.EN.U);

    *irq_en ^= ((uint32)1 << (uint32)interrupt);
}


void IfxGtm_Psm_Fifo_setChannelInterrupt(IfxGtm_Psm_Fifo fifo, IfxGtm_Psm_FifoChannel channel, IfxGtm_Psm_FifoChannelInterrupt interrupt)
{
    Ifx_GTM_PSM_FIFO_CH *fifoCh    = IfxGtm_Psm_Fifo_getChannelPointer(fifo, channel);
    uint32              *irq_force = (uint32 *)&(fifoCh->IRQ.FORCINT.U);

    *irq_force |= ((uint32)1 << (uint32)interrupt);
}


void IfxGtm_Psm_F2a_enableStream(IfxGtm_Psm_F2a f2a, IfxGtm_Psm_F2aStream f2aStream)
{
    Ifx_GTM_PSM_F2A *f2aPtr     = IfxGtm_Psm_F2a_getPointer(f2a);
    uint32          *f2a_enable = (uint32 *)&(f2aPtr->ENABLE.U);
    uint32           value      = (uint32)IfxGtm_Psm_F2aStreamState_enabled << (uint32)(f2aStream * 2);
    uint32           mask       = (uint32)3 << (f2aStream * 2);

    __ldmst_c((volatile void *)f2a_enable, mask, value);
}


void IfxGtm_Psm_F2a_disableStream(IfxGtm_Psm_F2a f2a, IfxGtm_Psm_F2aStream f2aStream)
{
    Ifx_GTM_PSM_F2A *f2aPtr     = IfxGtm_Psm_F2a_getPointer(f2a);
    uint32          *f2a_enable = (uint32 *)&(f2aPtr->ENABLE.U);
    uint32           value      = (uint32)IfxGtm_Psm_F2aStreamState_disabled << (uint32)(f2aStream * 2);
    uint32           mask       = (uint32)3 << (f2aStream * 2);

    __ldmst_c((volatile void *)f2a_enable, mask, value);
}


IfxGtm_Psm_F2aStreamState IfxGtm_Psm_F2a_getStreamState(IfxGtm_Psm_F2a f2a, IfxGtm_Psm_F2aStream f2aStream)
{
    Ifx_GTM_PSM_F2A *f2aPtr     = IfxGtm_Psm_F2a_getPointer(f2a);
    uint32           f2a_enable = f2aPtr->ENABLE.U;

    f2a_enable &= ((uint32)3 << (f2aStream * 2));

    return (IfxGtm_Psm_F2aStreamState)(f2a_enable >> (f2aStream * 2));
}
