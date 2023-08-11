/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef __ETHERNET_H_
#define __ETHERNET_H_

#include <Cpu/Std/Ifx_Types.h>

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
typedef struct
{
	uint8 ipaddr1;
	uint8 ipaddr2;
	uint8 ipaddr3;
	uint8 ipaddr4;
	uint8 mask1;
	uint8 mask2;
	uint8 mask3;
	uint8 mask4;
}t_ip_addr;

typedef enum
{
	DHCP = 0,
	STATIC_IP,
	ROM_DEFAULT,
	MAX_ETH_MODE,
}e_eth_mode;

/* define the default setting when ROM is not set */
#define DEFAULT_IP1    10
#define DEFAULT_IP2    0
#define DEFAULT_IP3    0
#define DEFAULT_IP4    18
#define DEFAULT_MASK1  255
#define DEFAULT_MASK2  255
#define DEFAULT_MASK3  255
#define DEFAULT_MASK4  0

#define DEFAULT_MODE   STATIC_IP

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
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

void Ethernet_Init(e_eth_mode eth_mode, t_ip_addr* ip_p);
void Ethernet_Stop(void);
boolean Ethernet_Is_Ip_Bound(void);
boolean Ethernet_get_ip(uint32* p_ipaddr);
uint8* Ethernet_get_mac(void);
void geth_init_loadsim(t_ip_addr* ip);

#endif  // __ETHERNET_H_
