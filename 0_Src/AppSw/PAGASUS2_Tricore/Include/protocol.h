/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef __PROTOCOL_H_
#define __PROTOCOL_H_

#include <Cpu/Std/Ifx_Types.h>
#include "protocol_common.h"

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
/**\brief Get a package from the raw data
 * \param  data  the raw data buffer
 * \param  size  the raw data buffer size
 * \param  packet  the packet buffer to store the package
 * \return True if successfully find a package
 */
boolean Protocol_GetPackage(uint8* data, uint8 size, tPacket* packet);

/**\brief Prepare a package to send
 * \param  packet  the packet buffer that is going to send
 * \return True if successfully find a package
 */
boolean Protocol_PreparePackage(tPacket* packet);

/**\brief Initial a package with default value and protocol version
 * \param  packet  the packet buffer that is going to send
 * \return True if successfully find a package
 */
void Protocol_InitialPacket(tPacket* packet);

/**\brief Print out the packet content
 * \param  cmd  the packet that is going to print out
 * \return none
 */
void dump_packet(tPacket* cmd);
#endif  // __PROTOCOL_H_
