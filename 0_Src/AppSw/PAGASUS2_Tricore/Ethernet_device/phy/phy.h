/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef PHY_H_
#define PHY_H_

#include <Configuration.h>
#include "../ethernet_device.h"

/*\brief Initial all the Ethernet phys
 *\param:  Void
 *\return: void */
IFX_EXTERN void Ethernet_Phy_Initial(uint32 devices);


/*\brief DeInitial all the Ethernet phys
 *\param:  Void
 *\return: void */
IFX_EXTERN void Ethernet_Phy_DeInitial(uint32 devices);


//BroadR-REach PHY Registers
//TODO: need to update the name when getting the spec/ref
#define IFXETH_BROADR_PHY_RDB											0x13
#define IFXETH_BROADR_PHY_RDB_BIT_TIMING_ADJUST							(0x1 << 9)
#define IFXETH_BROADR_PHY_LRE_STATUS_REGISTER_ADDR                      0x01
#define IFXETH_BROADR_PHY_LINK_STATUS_BIT								(0x01 << 2)

#endif /* PHY_H_ */
