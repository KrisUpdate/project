/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef ETHERNET_DEVICE_H
#define ETHERNET_DEVICE_H

#include <Configuration.h>

#define switch_sja1105  0x01
#define switch_88e6321  0x02
#define phy_88q2112     0x04
#define switch_sja1105_step2 0x08
#define default_config_init  0x10

/*\brief Initial all the Ethernet Devices
 *\param:  void
 *\return: void */
IFX_EXTERN void Ethernet_Devices_Initial(uint32 devices);


/*\brief De-Initial all the Ethernet Devices
 * 		 now only support de-initial the PHY
 *\param:  void
 *\return: void */
IFX_EXTERN void Ethernet_Devices_DeInitial(uint32 devices);

#endif /* ETHERNET_DEVICE_H */
