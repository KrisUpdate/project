/**
 * \file GethBasicDemo.h
 * \brief Demo transmission and reception
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
 * \defgroup IfxLld_Demo_GethBasic_SrcDoc_Main Demo Source
 * \ingroup IfxLld_Demo_GethBasic_SrcDoc
 * \defgroup IfxLld_Demo_GethBasic_SrcDoc_Main_Interrupt Interrupts
 * \ingroup IfxLld_Demo_GethBasic_SrcDoc_Main
 */

#ifndef GETHDEMOBASIC_H
#define GETHDEMOBASIC_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include <Ifx_Types.h>
#include "IfxGeth_Eth.h"
#include "IfxCpu.h"
#include "IfxPort.h"
#include "Configuration.h"
#include "Ifx_Lwip.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
//#define IFXGETH_MAX_BUFFER_SIZE 64 /* bytes */
// #define IFXGETH_MAX_BUFFER_SIZE 1500 /* bytes */
#define IFXGETH_MAX_BUFFER_SIZE 1568 /* bytes */
#define IFXGETH_NUM_PACKETS IFXGETH_MAX_TX_DESCRIPTORS	/* number of packets to be transmitted per channel */
#define IFXGETH_NUM_OF_CHANNELS 3  /* change # of DMA channels to 3 */
#define IFXGETH_NUM_OF_QUEUES 4
#define GETHWAITTIMEOUT 300000 //1ms for 300mhz  QUANTA LWIP Added
// Tx priorities
//#define IFX_INTPRIO_GETH_DMA_TX0 10
//#define IFX_INTPRIO_GETH_DMA_TX1 11
//#define IFX_INTPRIO_GETH_DMA_TX2 12
//#define IFX_INTPRIO_GETH_DMA_TX3 13
/*
#define IFX_INTPRIO_GETH_DMA_TX0 16
#define IFX_INTPRIO_GETH_DMA_TX1 17
#define IFX_INTPRIO_GETH_DMA_TX2 18
#define IFX_INTPRIO_GETH_DMA_TX3 19
#define IFX_INTPRIO_GETH_DMA_RX0 20
#define IFX_INTPRIO_GETH_DMA_RX1 21
#define IFX_INTPRIO_GETH_DMA_RX2 22
#define IFX_INTPRIO_GETH_DMA_RX3 23
*/
/* PHY settings */
#define RTL8211FD_PHYADDR 				0x1
#define RTL8211FD_ENABLE_AUTONEG		0x1
/* PHY Register Mapping					offset */
#define RTL8211FD_PAGE0x000 			0x000
#define RTL8211FD_BMCR					0 		/* Basic Mode Control Register */
#define RTL8211FD_BMSR					1 		/* Basic Mode status Register */
#define RTL8211FD_PHYID1				2		/* PHY Identification Register */
#define RTL8211FD_PHYID2				3		/* PHY Identification Register */
#define RTL8211FD_ANAR					4
#define RTL8211FD_ANLPAR				5
#define RTL8211FD_ANER					6
#define RTL8211FD_ANNPTR				7
#define RTL8211FD_ANNPRR				8
#define RTL8211FD_GBCR					9
#define RTL8211FD_GBSR					10
#define RTL8211FD_MACR					13
#define RTL8211FD_MAADR					14
#define RTL8211FD_GBESR					15

#define RTL8211FD_PAGE0xA42				0xA42
#define RTL8211FD_INER					18

#define RTL8211FD_PAGE0xA43 			0xA43
#define RTL8211FD_PHYCR1				24
#define RTL8211FD_PHYCR2				25
#define RTL8211FD_PHYSR					26
#define RTL8211FD_GREENETHERNET27		27
#define RTL8211FD_GREENETHERNET28		28
#define RTL8211FD_INSR					29
#define RTL8211FD_PAGSR					31

#define RTL8211FD_PAGE0xA46				0xA46
#define RTL8211FD_PHYSCR				20

#define RTL8211FD_PAGE0xD04				0xD04
#define RTL8211FD_LCR					16
#define RTL8211FD_EEELCR				17

#define RTL8211FD_PAGE0xD08				0xD08
#define RTL8211FD_MIICR					21

#define RTL8211FD_PAGE0xD40				0xD40
#define RTL8211FD_INTBCR				22

/* PHY Definitions in the Mapped Registers */
#define RTL8211FD_PHY_ID1          		0x001C      /* PHY_ID1 for RTL8211F */
#define RTL8211FD_PHY_ID2          		0xC916      /* PHY_ID2 for RTL8211F */

#define RTL8211FD_GREENETHERNETUPDATE 	0x8001
#define RTL8211FD_GREENETHERNETDISABLE 	0x573F
#define RTL8211FD_GREENETHERNETENABLE 	0xD73F

/* RTL8211F BMCR PHY Control Register */
#define RTL8211FD_PHYRESET       		0x8000
#define RTL8211FD_PHYLOOPBACK	   		0x4000
#define RTL8211FD_PHYAUTONEGOTIATION	0x1000
#define RTL8211FD_PHYPOWERDOWN     		0x0800
#define RTL8211FD_PHYISOLATE      		0x0400
#define RTL8211FD_PHYRESTARTNEGOTIATION	0x0200
#define RTL8211FD_PHYFULLDUPLEX			0x0100		/* Full duplex */
#define RTL8211FD_PHYSPEEDG		     	0x0040		/* 1000M Speed */
#define RTL8211F_TX_DELAY         		0x100

/* RTL8211F BMSR PHY Status Register */
#define RTL8211FD_PHYAUTONEGOTIATION_COMPLETE_OFF	5U
#define RTL8211FD_PHYAUTONEGOTIATION_COMPLETE_MSK 0x0020
#define RTL8211FD_PHYRESET_COMPLETE_OFF 15U
#define RTL8211FD_PHYRESET_COMPLETE_MSK 0x8000


/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/
typedef struct
{
	struct
	{
		IfxGeth_Eth geth;  /* GETH handle */
	} drivers;
} App_GethBasic;

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
IFX_EXTERN App_GethBasic g_GethBasic;
IFX_EXTERN eth_addr_t ethAddr;

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
IFX_EXTERN void GethBasicDemo_firstinit(void);
IFX_EXTERN void GethBasicDemo_init(eth_addr_t inforom_mac);
IFX_EXTERN void GethBasicDemo_run(void);

#endif
