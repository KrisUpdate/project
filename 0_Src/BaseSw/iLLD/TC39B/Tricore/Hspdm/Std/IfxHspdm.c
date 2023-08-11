/**
 * \file IfxHspdm.c
 * \brief HSPDM  basic functionality
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

#include "IfxHspdm.h"

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

void IfxHspdm_setStartAddress(Ifx_HSPDM *hspdm, IfxHspdm_Buffer buffer, uint32 address)
{
    if (address < IFXHSPDM_BUFFER_MAX_SIZE)
    {
        switch (buffer)
        {
        case IfxHspdm_Buffer_a:
            hspdm->BUFA0.B.STARTA = address;
            break;
        case IfxHspdm_Buffer_b:
            hspdm->BUFB0.B.STARTB = address;
            break;
        default:
            break;
        }
    }
    else
    {
        /* do nothin */
    }
}


void IfxHspdm_setEndAddress(Ifx_HSPDM *hspdm, IfxHspdm_Buffer buffer, uint32 address)
{
    if (address < IFXHSPDM_BUFFER_MAX_SIZE)
    {
        switch (buffer)
        {
        case IfxHspdm_Buffer_a:
            hspdm->BUFA0.B.ENDA = address;
            break;
        case IfxHspdm_Buffer_b:
            hspdm->BUFB0.B.ENDB = address;
            break;
        default:
            break;
        }
    }
    else
    {
        /* do nothin */
    }
}


void IfxHspdm_setMuteAddresses(Ifx_HSPDM *hspdm, IfxHspdm_MuteRange muteRange, uint32 startAddress, uint32 endAddress)
{
    switch (muteRange)
    {
    case IfxHspdm_MuteRange_0:
        hspdm->MUTE0.B.START0 = startAddress;
        hspdm->MUTE0.B.END0   = endAddress;
        break;
    case IfxHspdm_MuteRange_1:
        hspdm->MUTE1.B.START1 = startAddress;
        hspdm->MUTE1.B.END1   = endAddress;
        break;
    default:
        /* do nothing */
        break;
    }
}


void IfxHspdm_enableBSB(Ifx_HSPDM *hspdm, IfxHspdm_BSB bsb, boolean enable)
{
    switch (bsb)
    {
    case IfxHspdm_BSB_0:
        hspdm->CON.B.EN0 = (uint32)enable;
        break;
    case IfxHspdm_BSB_1:
        hspdm->CON.B.EN1 = (uint32)enable;
        break;
    default:
        break; /* do nothing here */
    }
}


void IfxHspdm_initBsPin(IfxHspdm_Bs_Out *bsPin, IfxPort_OutputMode outputMode, IfxPort_PadDriver padDriver)
{
    IfxPort_setPinModeOutput(bsPin->pin.port, bsPin->pin.pinIndex, outputMode, bsPin->select);
    IfxPort_setPinPadDriver(bsPin->pin.port, bsPin->pin.pinIndex, padDriver);
}


void IfxHspdm_initMutePin(IfxHspdm_Mute_Out *mutePin, IfxPort_OutputMode outputMode, IfxPort_PadDriver padDriver)
{
    IfxPort_setPinModeOutput(mutePin->pin.port, mutePin->pin.pinIndex, outputMode, mutePin->select);
    IfxPort_setPinPadDriver(mutePin->pin.port, mutePin->pin.pinIndex, padDriver);
}
