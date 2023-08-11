/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef SWITCH_H_
#define SWITCH_H_

#include <Configuration.h>
#include "../ethernet_device.h"

/*\brief Initial all the Ethernet switch
 *\param:  Void
 *\return: void */
IFX_EXTERN void Ethernet_Switch_Initial(uint32 devices);

#endif /* SWITCH_H_ */
