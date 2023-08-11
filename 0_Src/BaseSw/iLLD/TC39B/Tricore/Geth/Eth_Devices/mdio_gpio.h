/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#ifndef MDIO_GPIO_H_
#define MDIO_GPIO_H_

#include "configuration.h"

void init_MDIO(void);

/* Clause22 register write */

void clause22_write( uint8 phy, uint8 reg, uint16 data);

/* Clause22 register read */

uint16 clause22_read( uint8 phy, uint8 reg);


void clause45_write (uint8 phy, uint8 dev,uint16 adr, uint16 data);

uint16 clause45_read (uint8 phy, uint8 dev,uint16 adr);

/* Read register from current address and increase address register*/

uint16 clause45_read_next (uint8 phy, uint8 dev);

/* Start clause45 read Burst
Set first address , read register and increase address register */

uint16 clause45_read_burst (uint8 phy, uint8 dev,uint16 adr);

#endif /* MDIO_GPIO_H_ */
