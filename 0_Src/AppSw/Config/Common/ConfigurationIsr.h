/**
 * \file ConfigurationIsr.h
 * \brief Interrupts configuration.
 *
 *
 * \version disabled
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
 * \defgroup IfxLld_Demo_GethBasic_SrcDoc_InterruptConfig Interrupt configuration
 * \ingroup IfxLld_Demo_GethBasic_SrcDoc
 */

#ifndef CONFIGURATIONISR_H
#define CONFIGURATIONISR_H
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
//#include "Configuration_project.h"
#if (PROJECT == PROJECT_TRIBOARD)
#include "ConfigurationIsr_triboard.h"


#elif (PROJECT == PROJECT_PAGASUS2)

#elif (PROJECT == PROJECT_V31C)

#elif (PROJECT == PROJECT_V3NA)

#elif (PROJECT == PROJECT_V3AA)
#include "ConfigurationIsr_v3aa.h"
#else
#error "Please define the correct project in Configuration.h!"
#endif
//------------------------------------------------------------------------------

#endif
