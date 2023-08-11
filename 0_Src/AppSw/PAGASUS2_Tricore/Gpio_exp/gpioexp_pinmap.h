/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef GPIO_EXP_GPIOEXP_PINMAP_H_
#define GPIO_EXP_GPIOEXP_PINMAP_H_

#include <configuration.h>

typedef struct
{
	uint8 i2cbus;
	uint8 devAddr;
	uint8 group;
	uint8 port;
}IoExpPort_Pin;

IFX_EXTERN IoExpPort_Pin IoExpPort_A74_P00;  /* slave addree 0x74, port P00 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A74_P01;  /* slave addree 0x74, port P01 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A74_P02;  /* slave addree 0x74, port P02 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A74_P03;  /* slave addree 0x74, port P03 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A74_P04;  /* slave addree 0x74, port P04 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A74_P05;  /* slave addree 0x74, port P05 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A74_P06;  /* slave addree 0x74, port P06 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A74_P07;  /* slave addree 0x74, port P07 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A74_P10;  /* slave addree 0x74, port P10 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A74_P11;  /* slave addree 0x74, port P11 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A74_P12;  /* slave addree 0x74, port P12 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A74_P13;  /* slave addree 0x74, port P13 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A74_P14;  /* slave addree 0x74, port P14 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A74_P15;  /* slave addree 0x74, port P15 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A74_P16;  /* slave addree 0x74, port P16 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A74_P17;  /* slave addree 0x74, port P17 */

IFX_EXTERN IoExpPort_Pin IoExpPort_A75_P00;  /* slave addree 0x75, port P00 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A75_P01;  /* slave addree 0x75, port P01 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A75_P02;  /* slave addree 0x75, port P02 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A75_P03;  /* slave addree 0x75, port P03 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A75_P04;  /* slave addree 0x75, port P04 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A75_P05;  /* slave addree 0x75, port P05 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A75_P06;  /* slave addree 0x75, port P06 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A75_P07;  /* slave addree 0x75, port P07 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A75_P10;  /* slave addree 0x75, port P10 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A75_P11;  /* slave addree 0x75, port P11 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A75_P12;  /* slave addree 0x75, port P12 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A75_P13;  /* slave addree 0x75, port P13 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A75_P14;  /* slave addree 0x75, port P14 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A75_P15;  /* slave addree 0x75, port P15 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A75_P16;  /* slave addree 0x75, port P16 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A75_P17;  /* slave addree 0x75, port P17 */

IFX_EXTERN IoExpPort_Pin IoExpPort_A76_P00;  /* slave addree 0x76, port P00 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A76_P01;  /* slave addree 0x76, port P01 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A76_P02;  /* slave addree 0x76, port P02 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A76_P03;  /* slave addree 0x76, port P03 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A76_P04;  /* slave addree 0x76, port P04 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A76_P05;  /* slave addree 0x76, port P05 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A76_P06;  /* slave addree 0x76, port P06 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A76_P07;  /* slave addree 0x76, port P07 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A76_P10;  /* slave addree 0x76, port P10 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A76_P11;  /* slave addree 0x76, port P11 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A76_P12;  /* slave addree 0x76, port P12 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A76_P13;  /* slave addree 0x76, port P13 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A76_P14;  /* slave addree 0x76, port P14 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A76_P15;  /* slave addree 0x76, port P15 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A76_P16;  /* slave addree 0x76, port P16 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A76_P17;  /* slave addree 0x76, port P17 */

IFX_EXTERN IoExpPort_Pin IoExpPort_A77_P00;  /* slave addree 0x77, port P00 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A77_P01;  /* slave addree 0x77, port P01 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A77_P02;  /* slave addree 0x77, port P02 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A77_P03;  /* slave addree 0x77, port P03 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A77_P04;  /* slave addree 0x77, port P04 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A77_P05;  /* slave addree 0x77, port P05 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A77_P06;  /* slave addree 0x77, port P06 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A77_P07;  /* slave addree 0x77, port P07 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A77_P10;  /* slave addree 0x77, port P10 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A77_P11;  /* slave addree 0x77, port P11 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A77_P12;  /* slave addree 0x77, port P12 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A77_P13;  /* slave addree 0x77, port P13 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A77_P14;  /* slave addree 0x77, port P14 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A77_P15;  /* slave addree 0x77, port P15 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A77_P16;  /* slave addree 0x77, port P16 */
IFX_EXTERN IoExpPort_Pin IoExpPort_A77_P17;  /* slave addree 0x77, port P17 */

#if CAMERA_CIM_ENABLE == 1
/* IO Expander on DDPX CIM board */
IFX_EXTERN IoExpPort_Pin IoExpPort_CIM_A74_P00;  /* slave addree 0x74, port P00 */
IFX_EXTERN IoExpPort_Pin IoExpPort_CIM_A74_P01;  /* slave addree 0x74, port P01 */
IFX_EXTERN IoExpPort_Pin IoExpPort_CIM_A74_P02;  /* slave addree 0x74, port P02 */
IFX_EXTERN IoExpPort_Pin IoExpPort_CIM_A74_P03;  /* slave addree 0x74, port P03 */
IFX_EXTERN IoExpPort_Pin IoExpPort_CIM_A74_P04;  /* slave addree 0x74, port P04 */
IFX_EXTERN IoExpPort_Pin IoExpPort_CIM_A74_P05;  /* slave addree 0x74, port P05 */
IFX_EXTERN IoExpPort_Pin IoExpPort_CIM_A74_P06;  /* slave addree 0x74, port P06 */
IFX_EXTERN IoExpPort_Pin IoExpPort_CIM_A74_P07;  /* slave addree 0x74, port P07 */
IFX_EXTERN IoExpPort_Pin IoExpPort_CIM_A74_P10;  /* slave addree 0x74, port P10 */
IFX_EXTERN IoExpPort_Pin IoExpPort_CIM_A74_P11;  /* slave addree 0x74, port P11 */
IFX_EXTERN IoExpPort_Pin IoExpPort_CIM_A74_P12;  /* slave addree 0x74, port P12 */
IFX_EXTERN IoExpPort_Pin IoExpPort_CIM_A74_P13;  /* slave addree 0x74, port P13 */
IFX_EXTERN IoExpPort_Pin IoExpPort_CIM_A74_P14;  /* slave addree 0x74, port P14 */
IFX_EXTERN IoExpPort_Pin IoExpPort_CIM_A74_P15;  /* slave addree 0x74, port P15 */
IFX_EXTERN IoExpPort_Pin IoExpPort_CIM_A74_P16;  /* slave addree 0x74, port P16 */
IFX_EXTERN IoExpPort_Pin IoExpPort_CIM_A74_P17;  /* slave addree 0x74, port P17 */
#endif //CAMERA_CIM_ENABLE == 1

#endif /* GPIO_EXP_GPIOEXP_PINMAP_H_ */
