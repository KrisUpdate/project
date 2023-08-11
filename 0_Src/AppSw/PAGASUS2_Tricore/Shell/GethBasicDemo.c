/**
 * \file GethBasicDemo.c
 * \brief Demo GethBasicDemo
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
 */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include <stdio.h>
#include "Ifx_Console.h"
#include "GethBasicDemo.h"
#include "IfxCpu.h"
#include "IfxCpu_Irq.h"
#include "lwip/debug.h" /* for LWIP */
#include "Ifx_Lwip.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define IFXGETH_HEADER_LENGTH       14  /* words */
#define ENABLE_CLAUSE45             1   /* Enable clause 45 mode access via MDIO */
#define IFX_INTTOS_GETH_DMA_TXRX    0   /* TOS = 0, cpu0 handles the interrupt */
/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/
const IfxGeth_Eth_RgmiiPins Rgmiipins = {
        //Set the reference clock
        .txClk = &IfxGeth_TXCLK_P11_4_OUT ,
        .txd0 = &IfxGeth_TXD0_P11_3_OUT ,
        .txd1 = &IfxGeth_TXD1_P11_2_OUT ,
        .txd2 = &IfxGeth_TXD2_P11_1_OUT ,
        .txd3 = &IfxGeth_TXD3_P11_0_OUT ,
        .txCtl = &IfxGeth_TXCTL_P11_6_OUT ,
        .rxClk = &IfxGeth_RXCLKA_P11_12_IN ,
        .rxd0 = &IfxGeth_RXD0A_P11_10_IN ,
        .rxd1 = &IfxGeth_RXD1A_P11_9_IN ,
        .rxd2 = &IfxGeth_RXD2A_P11_8_IN ,
        .rxd3 = &IfxGeth_RXD3A_P11_7_IN ,
        .rxCtl = &IfxGeth_RXCTLA_P11_11_IN ,
        .mdc = &IfxGeth_MDC_P12_0_OUT ,
        .mdio = &IfxGeth_MDIO_P12_1_INOUT ,
        .grefClk = &IfxGeth_GREFCLK_P11_5_IN ,
};
/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
App_GethBasic g_GethBasic; /* Geth demo global data */

uint8 flag = FALSE; /* defined global - reintialize this in functions for EE */

/* Transmit interrupt count on a channel */
volatile uint32 chn0TxInterrupt;
volatile uint32 chn1TxInterrupt;
volatile uint32 chn2TxInterrupt;
volatile uint32 chn3TxInterrupt;

/* Receive interrupt count on a channel */
volatile uint32 chn0RxInterrupt;
volatile uint32 chn1RxInterrupt;
volatile uint32 chn2RxInterrupt;
volatile uint32 chn3RxInterrupt;
volatile uint32 origRXcount = 0;

/* MAC address of source and target*/
#define IS_TALKER 1
#if (IS_TALKER)
const uint8 myMacAddressS[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
#else
const uint8 myMacAddressS[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x66};
#endif
const uint8 myMacAddressD[6] = {0x12, 0x34, 0x56, 0x78, 0x90, 0xab};

#if (IS_TALKER)
eth_addr_t ethAddr={.addr={0x00,0x11,0x22,0x33,0x44,0x55}};
#else
eth_addr_t ethAddr={.addr={0x00,0x11,0x22,0x33,0x44,0x66}};
#endif

extern uint8 TX_PACKET_DEBUG;
extern uint8 RX_PACKET_DEBUG;

/* Geth PHY register values */
uint32 phyreg[38]={0};

/* The aligment is set 64 bit since circular buffer is configured to 64 bit.
 * The User needs to configure the same depending on their use-case           */

uint8 IFX_ALIGN(64) channel0TxBuffer1[IFXGETH_MAX_TX_DESCRIPTORS][IFXGETH_MAX_BUFFER_SIZE];
uint8 IFX_ALIGN(64)channel1TxBuffer1[IFXGETH_MAX_TX_DESCRIPTORS][IFXGETH_MAX_BUFFER_SIZE];
uint8 IFX_ALIGN(64) channel2TxBuffer1[IFXGETH_MAX_TX_DESCRIPTORS][IFXGETH_MAX_BUFFER_SIZE];
uint8 IFX_ALIGN(64) channel3TxBuffer1[IFXGETH_MAX_TX_DESCRIPTORS][IFXGETH_MAX_BUFFER_SIZE];

/* Receive buffers needs extra 4 bytes to accomodate the Frame Check Sequence bytes */
uint8 IFX_ALIGN(64) channel0RxBuffer1[IFXGETH_MAX_RX_DESCRIPTORS][IFXGETH_MAX_BUFFER_SIZE+4];
uint8 IFX_ALIGN(64) channel1RxBuffer1[IFXGETH_MAX_RX_DESCRIPTORS][IFXGETH_MAX_BUFFER_SIZE+4];
uint8 IFX_ALIGN(64) channel2RxBuffer1[IFXGETH_MAX_RX_DESCRIPTORS][IFXGETH_MAX_BUFFER_SIZE+4];
uint8 IFX_ALIGN(64) channel3RxBuffer1[IFXGETH_MAX_RX_DESCRIPTORS][IFXGETH_MAX_BUFFER_SIZE+4];

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------ISR Implementations---------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Demo_GethBasic_SrcDoc_Main_Interrupt
 * \{ */

/** \name Interrupts for Geth
 * \{ */

/** \} */

/** \} */


/** \brief Handle Tx interrupt on Geth DMA channel 0.
 *
 * \isrProvider \ref ISR_PROVIDER_GETHDMATX0
 * \isrPriority \ref ISR_PRIORITY_GETHDMATX0
 *
 */

IFX_INTERRUPT(gethDmaTx0, IFX_INTTOS_GETH_DMA_TXRX, IFX_INTPRIO_GETH_DMA_TX0)
{
    if(g_GethBasic.drivers.geth.gethSFR->DMA_CH[0].STATUS.B.TI == 1) {
        chn0TxInterrupt++;
    }
    /* clear the TX interrupt status for the next interrupt to come */
    IfxGeth_dma_clearInterruptFlag(g_GethBasic.drivers.geth.gethSFR, IfxGeth_DmaChannel_0, IfxGeth_DmaInterruptFlag_transmitInterrupt);
}

/** \brief Handle Tx interrupt on Geth DMA channel 1.
 *
 * \isrProvider \ref ISR_PROVIDER_GETHDMATX1
 * \isrPriority \ref ISR_PRIORITY_GETHDMATX1
 *
 */
IFX_INTERRUPT(gethDmaTx1, IFX_INTTOS_GETH_DMA_TXRX, IFX_INTPRIO_GETH_DMA_TX1)
{
    if(g_GethBasic.drivers.geth.gethSFR->DMA_CH[1].STATUS.B.TI == 1) {
        chn1TxInterrupt++;
    }
    /* clear the TX interrupt status for the next interrupt to come */
    IfxGeth_dma_clearInterruptFlag(g_GethBasic.drivers.geth.gethSFR, IfxGeth_DmaChannel_1, IfxGeth_DmaInterruptFlag_transmitInterrupt);
}

/** \brief Handle Tx interrupt on Geth DMA channel 2.
 *
 * \isrProvider \ref ISR_PROVIDER_GETHDMATX2
 * \isrPriority \ref ISR_PRIORITY_GETHDMATX2
 *
 */
IFX_INTERRUPT(gethDmaTx2, IFX_INTTOS_GETH_DMA_TXRX, IFX_INTPRIO_GETH_DMA_TX2)
{
    if(g_GethBasic.drivers.geth.gethSFR->DMA_CH[2].STATUS.B.TI == 1) {
        chn2TxInterrupt++;
    }
    /* clear the TX interrupt status for the next interrupt to come */
    IfxGeth_dma_clearInterruptFlag(g_GethBasic.drivers.geth.gethSFR, IfxGeth_DmaChannel_2, IfxGeth_DmaInterruptFlag_transmitInterrupt);
}

/** \brief Handle Tx interrupt on Geth DMA channel 3.
 *
 * \isrProvider \ref ISR_PROVIDER_GETHDMATX3
 * \isrPriority \ref ISR_PRIORITY_GETHDMATX3
 *
 */

IFX_INTERRUPT(gethDmaTx3, IFX_INTTOS_GETH_DMA_TXRX, IFX_INTPRIO_GETH_DMA_TX3)
{
    if(g_GethBasic.drivers.geth.gethSFR->DMA_CH[3].STATUS.B.TI == 1) {
        chn3TxInterrupt++;
    }
    /* clear the TX interrupt status for the next interrupt to come */
    IfxGeth_dma_clearInterruptFlag(g_GethBasic.drivers.geth.gethSFR, IfxGeth_DmaChannel_3, IfxGeth_DmaInterruptFlag_transmitInterrupt);
}

/** \brief Handle Rx interrupt on Geth DMA channel 0.
 *
 * \isrProvider \ref ISR_PROVIDER_GETHDMARX0
 * \isrPriority \ref ISR_PRIORITY_GETHDMARX0
 *
 */
IFX_INTERRUPT(gethDmaRx0, IFX_INTTOS_GETH_DMA_TXRX, IFX_INTPRIO_GETH_DMA_RX0)
{
    if(g_GethBasic.drivers.geth.gethSFR->DMA_CH[0].STATUS.B.RI == 1)
    {
        chn0RxInterrupt++;
    }
    /* clear the RX interrupt status for the next interrupt to come */
    IfxGeth_dma_clearInterruptFlag(g_GethBasic.drivers.geth.gethSFR, IfxGeth_DmaChannel_0, IfxGeth_DmaInterruptFlag_receiveInterrupt);
}

/** \brief Handle Rx interrupt on Geth DMA channel 1.
 *
 * \isrProvider \ref ISR_PROVIDER_GETHDMARX1
 * \isrPriority \ref ISR_PRIORITY_GETHDMARX1
 *
 */
IFX_INTERRUPT(gethDmaRx1, IFX_INTTOS_GETH_DMA_TXRX, IFX_INTPRIO_GETH_DMA_RX1)
{
    if(g_GethBasic.drivers.geth.gethSFR->DMA_CH[1].STATUS.B.RI == 1)
    {
        chn1RxInterrupt++;
    }
    /* clear the RX interrupt status for the next interrupt to come */
    IfxGeth_dma_clearInterruptFlag(g_GethBasic.drivers.geth.gethSFR, IfxGeth_DmaChannel_1, IfxGeth_DmaInterruptFlag_receiveInterrupt);
}

/** \brief Handle Rx interrupt on Geth DMA channel 2.
 *
 * \isrProvider \ref ISR_PROVIDER_GETHDMARX2
 * \isrPriority \ref ISR_PRIORITY_GETHDMARX2
 *
 */
IFX_INTERRUPT(gethDmaRx2, IFX_INTTOS_GETH_DMA_TXRX, IFX_INTPRIO_GETH_DMA_RX2)
{
    if(g_GethBasic.drivers.geth.gethSFR->DMA_CH[2].STATUS.B.RI == 1)
    {
        chn2RxInterrupt++;
    }
    /* clear the RX interrupt status for the next interrupt to come */
    IfxGeth_dma_clearInterruptFlag(g_GethBasic.drivers.geth.gethSFR, IfxGeth_DmaChannel_2, IfxGeth_DmaInterruptFlag_receiveInterrupt);
}

/** \brief Handle Rx interrupt on Geth DMA channel 3.
 *
 * \isrProvider \ref ISR_PROVIDER_GETHDMARX3
 * \isrPriority \ref ISR_PRIORITY_GETHDMARX3
 *
 */
IFX_INTERRUPT(gethDmaRx3, IFX_INTTOS_GETH_DMA_TXRX, IFX_INTPRIO_GETH_DMA_RX3)
{
    if(g_GethBasic.drivers.geth.gethSFR->DMA_CH[3].STATUS.B.RI == 1)
    {
        chn3RxInterrupt++;
    }
    /* clear the RX interrupt status for the next interrupt to come */
    IfxGeth_dma_clearInterruptFlag(g_GethBasic.drivers.geth.gethSFR, IfxGeth_DmaChannel_3, IfxGeth_DmaInterruptFlag_receiveInterrupt);
}

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

/** \brief MDIO read operation
 *
 * MDIO read operation.
 */
uint32 IFXETH_EPHY_RTL8211FG_mdio_read_reg(uint32 phyaddr, uint32 regaddr, boolean clause_22_45)
{
    uint32 pdata;
    if ( clause_22_45 == ENABLE_CLAUSE45 )
    {
        GETH_MAC_MDIO_ADDRESS.U = (phyaddr << IFX_GETH_MAC_MDIO_ADDRESS_PA_OFF) | (regaddr << IFX_GETH_MAC_MDIO_ADDRESS_RDA_OFF) | (0 << IFX_GETH_MAC_MDIO_ADDRESS_CR_OFF) | (3 << IFX_GETH_MAC_MDIO_ADDRESS_GOC_0_OFF) | (1<<IFX_GETH_MAC_MDIO_ADDRESS_C45E_OFF) | (1 << IFX_GETH_MAC_MDIO_ADDRESS_GB_OFF);
    }
    else
    {
        GETH_MAC_MDIO_ADDRESS.U = (phyaddr << IFX_GETH_MAC_MDIO_ADDRESS_PA_OFF) | (regaddr << IFX_GETH_MAC_MDIO_ADDRESS_RDA_OFF) | (0 << IFX_GETH_MAC_MDIO_ADDRESS_CR_OFF) | (2 << IFX_GETH_MAC_MDIO_ADDRESS_GOC_0_OFF) | (1 << IFX_GETH_MAC_MDIO_ADDRESS_GB_OFF);
    }
    /* loop to read the busy status. */
    while ((GETH_MAC_MDIO_ADDRESS.U & IFX_GETH_MAC_MDIO_ADDRESS_GB_MSK) == 1)
    {
        __nop();
    }

    //Read the register back
    pdata = GETH_MAC_MDIO_DATA.U;

    return (pdata);

}

/** \brief MDIO write operation
 *
 * MDIO write operation.
 */

void IFXETH_EPHY_RTL8211FG_mdio_write_reg(uint32 phyaddr, uint32 regaddr, uint32 data, boolean clause_22_45)
{
    GETH_MAC_MDIO_DATA.U = data;
    if ( clause_22_45 == ENABLE_CLAUSE45 )
    {
        GETH_MAC_MDIO_ADDRESS.U = ((phyaddr << IFX_GETH_MAC_MDIO_ADDRESS_PA_OFF) | (regaddr << IFX_GETH_MAC_MDIO_ADDRESS_RDA_OFF) | (0 << IFX_GETH_MAC_MDIO_ADDRESS_CR_OFF) | (1 << IFX_GETH_MAC_MDIO_ADDRESS_GOC_0_OFF) | (1<<IFX_GETH_MAC_MDIO_ADDRESS_C45E_OFF) | (1 << IFX_GETH_MAC_MDIO_ADDRESS_GB_OFF));
    }
    else
    {
        GETH_MAC_MDIO_ADDRESS.U = ((phyaddr << IFX_GETH_MAC_MDIO_ADDRESS_PA_OFF) | (regaddr << IFX_GETH_MAC_MDIO_ADDRESS_RDA_OFF) | (2 << IFX_GETH_MAC_MDIO_ADDRESS_CR_OFF) | (1 << IFX_GETH_MAC_MDIO_ADDRESS_GOC_0_OFF) | (1 << IFX_GETH_MAC_MDIO_ADDRESS_GB_OFF));
    }

    /* loop to read the busy status. */
    while ((GETH_MAC_MDIO_ADDRESS.U & IFX_GETH_MAC_MDIO_ADDRESS_GB_MSK) == 1)
    {
        __nop();
    }
}



/** \brief MDIO read operation status
 *
 * MDIO read operation status
 */
void GETH_mdioReadStatus()
{
    /* loop to read the busy status. */
    while ((GETH_MAC_MDIO_ADDRESS.U & IFX_GETH_MAC_MDIO_ADDRESS_GB_MSK) == 1)
    {
    }
}

/** \brief MDIO read operation
 *
 * MDIO read operation to read the data from the PHY register
 */
uint32 GETH_mdioRead(uint32 phyaddr, uint32 regaddr, uint8 clause_22_45)
{
    if ( clause_22_45 == ENABLE_CLAUSE45 )
    {
        GETH_MAC_MDIO_ADDRESS.U = ( (phyaddr << IFX_GETH_MAC_MDIO_ADDRESS_PA_OFF) | (regaddr << IFX_GETH_MAC_MDIO_ADDRESS_RDA_OFF) | (0 << IFX_GETH_MAC_MDIO_ADDRESS_CR_OFF) | (3 << IFX_GETH_MAC_MDIO_ADDRESS_GOC_0_OFF) | (1 << IFX_GETH_MAC_MDIO_ADDRESS_C45E_OFF) | (1 << IFX_GETH_MAC_MDIO_ADDRESS_GB_OFF));
    }
    else
    {
        GETH_MAC_MDIO_ADDRESS.U = ((phyaddr << IFX_GETH_MAC_MDIO_ADDRESS_PA_OFF) | (regaddr << IFX_GETH_MAC_MDIO_ADDRESS_RDA_OFF) | (0 << IFX_GETH_MAC_MDIO_ADDRESS_CR_OFF) | (2 << IFX_GETH_MAC_MDIO_ADDRESS_GOC_0_OFF) | (1 << IFX_GETH_MAC_MDIO_ADDRESS_GB_OFF));
    }

    GETH_mdioReadStatus();

    return (GETH_MAC_MDIO_DATA.U);
}

/** \brief MDIO write operation
 *
 * MDIO write operation to write the data to the PHY register
 */
void GETH_mdioWrite(uint32 phyaddr, uint32 regaddr, uint32 regValue, uint8 clause_22_45)
{
    GETH_MAC_MDIO_DATA.U = regValue;
    if ( clause_22_45 == ENABLE_CLAUSE45 )
    {
        GETH_MAC_MDIO_ADDRESS.U = ( (phyaddr << IFX_GETH_MAC_MDIO_ADDRESS_PA_OFF) | (regaddr << IFX_GETH_MAC_MDIO_ADDRESS_RDA_OFF) | (0 << IFX_GETH_MAC_MDIO_ADDRESS_CR_OFF) | (1 << IFX_GETH_MAC_MDIO_ADDRESS_GOC_0_OFF) | (1 << IFX_GETH_MAC_MDIO_ADDRESS_C45E_OFF) | (1 << IFX_GETH_MAC_MDIO_ADDRESS_GB_OFF));
    }
    else
    {
        GETH_MAC_MDIO_ADDRESS.U = ( (phyaddr << IFX_GETH_MAC_MDIO_ADDRESS_PA_OFF) | (regaddr << IFX_GETH_MAC_MDIO_ADDRESS_RDA_OFF) | (2 << IFX_GETH_MAC_MDIO_ADDRESS_CR_OFF) | (1 << IFX_GETH_MAC_MDIO_ADDRESS_GOC_0_OFF) | (1 << IFX_GETH_MAC_MDIO_ADDRESS_GB_OFF));
    }
    GETH_mdioReadStatus();
}



/***************************************************************************/
// External GPHY ( RealTek -RTL8211FG ) init routine
/***************************************************************************/

/** \brief autonegotiation status
 *
 * Check the PHY register whether autonegotiation is complete
 */

void GETH_PHY_IsAutonegotiationCompleted(uint32 phyaddr)
{
    uint32 flag = FALSE;
    uint32 anc_counter = 0;
    while((++anc_counter < 150000 )&((IFXETH_EPHY_RTL8211FG_mdio_read_reg(phyaddr, RTL8211FD_BMSR, flag) & RTL8211FD_PHYAUTONEGOTIATION_COMPLETE_MSK) != (1 << RTL8211FD_PHYAUTONEGOTIATION_COMPLETE_OFF)))
    {
        /* be here only until auto negotiation is done */
        NOP(1000);
    }

    Ifx_Console_print("anc_counter %d \r\n",anc_counter);
}

/** \brief PHY reset complete
 *
 * Reset the PHY and wait till the reset is completed.
 */

void RTL8211FDReset(uint32 phyaddr)
{
    uint32 data, flag = FALSE;
    /* Check status of reset */
    data = IFXETH_EPHY_RTL8211FG_mdio_read_reg(phyaddr, RTL8211FD_BMCR, flag);
    data |= 0x8000; //Set reset bit
    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_BMCR, data, flag);

    /* Reset cycle is complete */
    while((IFXETH_EPHY_RTL8211FG_mdio_read_reg(phyaddr, RTL8211FD_BMCR, flag) & RTL8211FD_PHYRESET_COMPLETE_MSK) == (1 << RTL8211FD_PHYRESET_COMPLETE_OFF))
    {

    };
}


/***************************************************************************/
/* PHY_initialize: Initialize PHY Module                                   */
/***************************************************************************/

/** \brief Initialise PHY module
 *
 * Configure and enable the PHY module
 */
uint32  IFXETH_EPHY_RTL8211FG_init(uint32 phyaddr)
{
    uint32 i,data, bmcrData = 0;
    uint32 flag = FALSE; /* only Clause 22 enabled now */

    RTL8211FDReset(phyaddr);

    /* Enable 125M; RXC and CLKOUT; EEE is off - PHYCR2 */
    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_PAGSR, RTL8211FD_PAGE0xA43, flag);
    data = IFXETH_EPHY_RTL8211FG_mdio_read_reg(phyaddr, RTL8211FD_PHYCR2, flag);
    data = 0x843; /* Enable 125M; RXC and CLKOUT; EEE is off. */
    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_PHYCR2, data, flag);
    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_PAGSR, RTL8211FD_PAGE0x000, flag);

    /* PHY reset for setting this bits in PHYCR2 */
    RTL8211FDReset(phyaddr);

    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_PAGSR, RTL8211FD_PAGE0xD08, flag);
    data=IFXETH_EPHY_RTL8211FG_mdio_read_reg(phyaddr, 0x11, flag);
    data  |= RTL8211F_TX_DELAY;
    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, 0x11, data, flag);
    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_PAGSR, RTL8211FD_PAGE0x000, flag);

    /* Disable Green Etherent */
    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_PAGSR, RTL8211FD_PAGE0xA43, flag);
    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_GREENETHERNET27, RTL8211FD_GREENETHERNETUPDATE, flag);
    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_GREENETHERNET28, RTL8211FD_GREENETHERNETDISABLE, flag);
    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_PAGSR, RTL8211FD_PAGE0x000, flag);

    /* LED Indications : Set green LED for Link, yellow LED for Active*/
    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_PAGSR, RTL8211FD_PAGE0xD04, flag);
    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_LCR, 0x617F, flag);
    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_PAGSR, RTL8211FD_PAGE0x000, flag);

    /*
     * Configuration set in PHY
     * PHY Loopback - Enabled
     * PHY Autonegotiation - Enabled
     * Full Duplex - enabled
     * Speed = 1Gbps
     */
    bmcrData = 0x1340;   /* loop back mode bit "14" */

    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_BMCR, bmcrData, flag);

    if(RTL8211FD_ENABLE_AUTONEG == 1)
        GETH_PHY_IsAutonegotiationCompleted(RTL8211FD_PHYADDR);

}

uint32  IFXETH_EPHY_RTL8211FG_init2(uint32 phyaddr){
    uint32 i,data, bmcrData = 0;
    uint32 flag = FALSE; /* only Clause 22 enabled now */

    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_PAGSR, RTL8211FD_PAGE0xA43, flag);
    IFXETH_EPHY_RTL8211FG_mdio_read_reg(phyaddr, RTL8211FD_PHYSR, flag);
    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_PAGSR, RTL8211FD_PAGE0x000, flag);


    /* Read PHY registers - IEEE reg */
    for(i=0;i<16;i++)
        phyreg[i]=IFXETH_EPHY_RTL8211FG_mdio_read_reg(phyaddr, i, flag);

    /* Page reg */
    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_PAGSR, RTL8211FD_PAGE0xA42, flag);
    for(i=16;i<24;i++)
        phyreg[i]=IFXETH_EPHY_RTL8211FG_mdio_read_reg(phyaddr, i, flag);
    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_PAGSR, RTL8211FD_PAGE0x000, flag);

    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_PAGSR, RTL8211FD_PAGE0xA43, flag);
    for(i=24;i<32;i++)
        phyreg[i]=IFXETH_EPHY_RTL8211FG_mdio_read_reg(phyaddr, i, flag);
    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_PAGSR, RTL8211FD_PAGE0x000, flag);

    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_PAGSR, RTL8211FD_PAGE0xA46, flag);
    phyreg[33]=IFXETH_EPHY_RTL8211FG_mdio_read_reg(phyaddr, RTL8211FD_PHYSCR, flag);
    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_PAGSR, RTL8211FD_PAGE0x000, flag);

    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_PAGSR, RTL8211FD_PAGE0xD04, flag);
    phyreg[34]=IFXETH_EPHY_RTL8211FG_mdio_read_reg(phyaddr, RTL8211FD_LCR, flag);
    phyreg[35]=IFXETH_EPHY_RTL8211FG_mdio_read_reg(phyaddr, RTL8211FD_EEELCR, flag);
    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_PAGSR, RTL8211FD_PAGE0x000, flag);

    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_PAGSR, RTL8211FD_PAGE0xD08, flag);
    phyreg[36]=IFXETH_EPHY_RTL8211FG_mdio_read_reg(phyaddr, RTL8211FD_MIICR, flag);
    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_PAGSR, RTL8211FD_PAGE0x000, flag);

    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_PAGSR, RTL8211FD_PAGE0xD40, flag);
    phyreg[37]=IFXETH_EPHY_RTL8211FG_mdio_read_reg(phyaddr, RTL8211FD_INTBCR, flag);
    IFXETH_EPHY_RTL8211FG_mdio_write_reg(phyaddr, RTL8211FD_PAGSR, RTL8211FD_PAGE0x000, flag);

    return 0;
}


/** \brief Demo init API
 *
 * This function is called from main during initialization phase
 */

IfxGeth_Eth_Config config;
IfxGeth_Index gethInst;
void GethBasicDemo_firstinit(void){

    IfxGeth_Eth_initModuleConfig(&config, &MODULE_GETH);
    g_GethBasic.drivers.geth.gethSFR = config.gethSFR;

    IfxGeth_enableModule(g_GethBasic.drivers.geth.gethSFR);
    gethInst = IfxGeth_getIndex(g_GethBasic.drivers.geth.gethSFR);

    // configure pins
    config.pins.rgmiiPins = &Rgmiipins;
    config.phyInterfaceMode = IfxGeth_PhyInterfaceMode_rgmii;
    IfxGeth_setPhyInterfaceMode(g_GethBasic.drivers.geth.gethSFR, config.phyInterfaceMode);
    IfxGeth_Eth_setupRgmiiOutputPins(&g_GethBasic.drivers.geth, config.pins.rgmiiPins);
    IfxGeth_Eth_setupRgmiiInputPins(&g_GethBasic.drivers.geth, config.pins.rgmiiPins);

    GETH_GPCTL.B.ALTI0 |= 2;
}

void GethBasicDemo_init(eth_addr_t inforom_mac)
{
    uint32 i;

    IfxGeth_dma_applySoftwareReset(config.gethSFR);

    /* wait until reset is finished or timeout */
    NOP(1000);
    while (g_GethBasic.drivers.geth.gethSFR->DMA_MODE.B.SWR == 1) ;


    config.mac.lineSpeed = IfxGeth_LineSpeed_1000Mbps;

    for(i=0;i<6;++i)
    {
        config.mac.macAddress[i] = inforom_mac.addr[i];
    }

    // configure MTL
    config.mtl.numOfTxQueues = IFXGETH_NUM_TX_QUEUES;
    config.mtl.numOfRxQueues = IFXGETH_NUM_RX_QUEUES;
    config.mtl.txSchedulingAlgorithm = IfxGeth_TxSchedulingAlgorithm_sp;
    config.mtl.rxArbitrationAlgorithm = IfxGeth_RxArbitrationAlgorithm_wsp;
    for(i=0;i<IFXGETH_NUM_OF_QUEUES;++i)
    {
        config.mtl.txQueue[i].storeAndForward = TRUE;
        config.mtl.txQueue[i].txQueueSize = IfxGeth_QueueSize_2048Bytes;
        config.mtl.rxQueue[i].storeAndForward = TRUE;
        config.mtl.rxQueue[i].rxQueueSize = IfxGeth_QueueSize_8192Bytes;
        config.mtl.rxQueue[i].daBasedDmaChannelEnabled         = FALSE;
        config.mtl.rxQueue[i].rxQueueOverflowInterruptEnabled  = TRUE;
    }
    config.mtl.rxQueue[0].rxDmaChannelMap                  = IfxGeth_RxDmaChannel_0;
    config.mtl.rxQueue[1].rxDmaChannelMap                  = IfxGeth_RxDmaChannel_1;
    config.mtl.rxQueue[2].rxDmaChannelMap                  = IfxGeth_RxDmaChannel_2;
    config.mtl.rxQueue[3].rxDmaChannelMap                  = IfxGeth_RxDmaChannel_3;

    // configure DMA
    config.dma.fixedBurstEnabled = TRUE;
    config.dma.addressAlignedBeatsEnabled = TRUE;
    g_GethBasic.drivers.geth.gethSFR->DMA_MODE.B.DA = 1;
    config.dma.numOfTxChannels = IFXGETH_NUM_TX_CHANNELS;
    config.dma.numOfRxChannels = IFXGETH_NUM_RX_CHANNELS;
    for(i=0;i<IFXGETH_NUM_OF_CHANNELS;++i)
    {
        config.dma.txChannel[i].maxBurstLength = IfxGeth_DmaBurstLength_8;
        config.dma.txChannel[i].txDescrList = (IfxGeth_TxDescrList *) &IfxGeth_Eth_txDescrList[gethInst][i];
        config.dma.txChannel[i].txBuffer1Size = IFXGETH_MAX_BUFFER_SIZE;        // 1 descriptor per frame
        config.dma.rxChannel[i].maxBurstLength = IfxGeth_DmaBurstLength_8;
        config.dma.rxChannel[i].rxDescrList = (IfxGeth_RxDescrList *) &IfxGeth_Eth_rxDescrList[gethInst][i];
        config.dma.rxChannel[i].rxBuffer1Size = IFXGETH_MAX_BUFFER_SIZE + 4;        // 1 descriptor per frame

    }
    config.dma.txChannel[0].txBuffer1StartAddress = (uint32 *) &channel0TxBuffer1[0][0];
    config.dma.txChannel[1].txBuffer1StartAddress = (uint32 *) &channel1TxBuffer1[0][0];
    config.dma.txChannel[2].txBuffer1StartAddress = (uint32 *) &channel2TxBuffer1[0][0];
    config.dma.txChannel[3].txBuffer1StartAddress = (uint32 *) &channel3TxBuffer1[0][0];
    config.dma.rxChannel[0].rxBuffer1StartAddress = (uint32 *) &channel0RxBuffer1[0][0];
    config.dma.rxChannel[1].rxBuffer1StartAddress = (uint32 *) &channel1RxBuffer1[0][0];
    config.dma.rxChannel[2].rxBuffer1StartAddress = (uint32 *) &channel2RxBuffer1[0][0];
    config.dma.rxChannel[3].rxBuffer1StartAddress = (uint32 *) &channel3RxBuffer1[0][0];

    config.dma.txInterrupt[0].channelId = IfxGeth_DmaChannel_0;
    config.dma.txInterrupt[0].priority = IFX_INTPRIO_GETH_DMA_TX0;
    config.dma.txInterrupt[1].channelId = IfxGeth_DmaChannel_1;
    config.dma.txInterrupt[1].priority = IFX_INTPRIO_GETH_DMA_TX1;
    config.dma.txInterrupt[2].channelId = IfxGeth_DmaChannel_2;
    config.dma.txInterrupt[2].priority = IFX_INTPRIO_GETH_DMA_TX2;
    config.dma.txInterrupt[3].channelId = IfxGeth_DmaChannel_3;
    config.dma.txInterrupt[3].priority = IFX_INTPRIO_GETH_DMA_TX3;

    config.dma.rxInterrupt[0].channelId = IfxGeth_DmaChannel_0;
    config.dma.rxInterrupt[0].priority = IFX_INTPRIO_GETH_DMA_RX0;
    config.dma.rxInterrupt[1].channelId = IfxGeth_DmaChannel_1;
    config.dma.rxInterrupt[1].priority = IFX_INTPRIO_GETH_DMA_RX1;
    config.dma.rxInterrupt[2].channelId = IfxGeth_DmaChannel_2;
    config.dma.rxInterrupt[2].priority = IFX_INTPRIO_GETH_DMA_RX2;
    config.dma.rxInterrupt[3].channelId = IfxGeth_DmaChannel_3;
    config.dma.rxInterrupt[3].priority = IFX_INTPRIO_GETH_DMA_RX3;

    for(i=0;i<IFXGETH_NUM_OF_CHANNELS;++i)
    {
        config.dma.txInterrupt[i].provider = IfxCpu_Irq_getTos(IfxCpu_getCoreIndex());
        config.dma.rxInterrupt[i].provider = IfxCpu_Irq_getTos(IfxCpu_getCoreIndex());
    }

    /* Configure Geth module */
    IfxGeth_Eth_initModule(&g_GethBasic.drivers.geth, &config);
    IfxCpu_enableInterrupts();
    /* start transmitter/receiver */
    IfxGeth_Eth_startTransmitters(&g_GethBasic.drivers.geth, IFXGETH_NUM_OF_CHANNELS);      // start transmit for 4 channels
    IfxGeth_Eth_startReceivers(&g_GethBasic.drivers.geth, IFXGETH_NUM_OF_CHANNELS);     // start receive for 4 channels

}


/** \brief Demo run API
 *
 * This function is called from main, background loop
 */
void GethBasicDemo_run(void)
{
    int result = 0;
    uint32 i;
    uint32 packet = 0;
    uint32 packetLength ,payloadLength;
    uint32 channelIndex;
    chn0TxInterrupt = 0;
    chn1TxInterrupt = 0;
    chn2TxInterrupt = 0;
    chn3TxInterrupt = 0;
    chn0RxInterrupt = 0;
    chn1RxInterrupt = 0;
    chn2RxInterrupt = 0;
    chn3RxInterrupt = 0;


    /* create frame config */
    IfxGeth_Eth_FrameConfig frameConfig;
    for(channelIndex=0;channelIndex<IFXGETH_NUM_OF_CHANNELS;channelIndex++)
    {
        g_GethBasic.drivers.geth.gethSFR->MAC_RXQ_CTRL1.B.UPQ = channelIndex;

        for (packet = 0; packet < IFXGETH_NUM_PACKETS; ++packet)
        {

            payloadLength = IFXGETH_MAX_BUFFER_SIZE - IFXGETH_HEADER_LENGTH;
            packetLength = IFXGETH_MAX_BUFFER_SIZE;
            frameConfig.packetLength = IFXGETH_MAX_BUFFER_SIZE;


            /* write the header */
            switch (channelIndex)
            {
            case 0:
                frameConfig.channelId = IfxGeth_TxDmaChannel_0;
                IfxGeth_Eth_writeHeader(&g_GethBasic.drivers.geth, &channel0TxBuffer1[packet][0], (uint8 *)myMacAddressS, (uint8 *)myMacAddressD, payloadLength);
                /* write the payload */
                for(i = IFXGETH_HEADER_LENGTH; i < packetLength; ++i)
                    channel0TxBuffer1[packet][i] = i + packet;
                break;
            case 1:
                frameConfig.channelId = IfxGeth_TxDmaChannel_1;
                IfxGeth_Eth_writeHeader(&g_GethBasic.drivers.geth, &channel1TxBuffer1[packet][0], (uint8 *)myMacAddressS, (uint8 *)myMacAddressD, payloadLength);
                /* write the payload */
                for(i = IFXGETH_HEADER_LENGTH; i < packetLength; ++i)
                    channel1TxBuffer1[packet][i] = i + packet;
                break;
            case 2:
                frameConfig.channelId = IfxGeth_TxDmaChannel_2;
                IfxGeth_Eth_writeHeader(&g_GethBasic.drivers.geth, &channel2TxBuffer1[packet][0], (uint8 *)myMacAddressS, (uint8 *)myMacAddressD, payloadLength);
                /* write the payload */
                for(i = IFXGETH_HEADER_LENGTH; i < packetLength; ++i)
                    channel2TxBuffer1[packet][i] = i + packet;
                break;
            case 3:
                frameConfig.channelId = IfxGeth_TxDmaChannel_3;
                IfxGeth_Eth_writeHeader(&g_GethBasic.drivers.geth, &channel3TxBuffer1[packet][0], (uint8 *)myMacAddressS, (uint8 *)myMacAddressD, payloadLength);
                /* write the payload */
                for(i = IFXGETH_HEADER_LENGTH; i < packetLength; ++i)
                    channel3TxBuffer1[packet][i] = i + packet;
                break;
            default:
                break;
            }

            /* send frame */
            IfxGeth_Eth_sendFrame(&g_GethBasic.drivers.geth, &frameConfig);

            switch (channelIndex)
            {
            case 0:
                while (chn0TxInterrupt != (packet + 1)) ;
                while (chn0RxInterrupt != (packet + 1)) ;
                break;
            case 1:
                while (chn1TxInterrupt != (packet + 1)) ;
                while (chn1RxInterrupt != (packet + 1)) ;
                break;
            case 2:
                while (chn2TxInterrupt != (packet + 1)) ;
                while (chn2RxInterrupt != (packet + 1)) ;
                break;
            case 3:
                while (chn3TxInterrupt != (packet + 1)) ;
                while (chn3RxInterrupt != (packet + 1)) ;
                break;
            default:
                break;
            }
            IfxGeth_dma_clearInterruptFlag(g_GethBasic.drivers.geth.gethSFR, (IfxGeth_DmaChannel)frameConfig.channelId, IfxGeth_DmaInterruptFlag_transmitInterrupt);
            /* wait until data is been received */

            /* receive data */
            switch (channelIndex)
            {
            case 0:
                while (IfxGeth_Eth_isRxDataAvailable(&g_GethBasic.drivers.geth, IfxGeth_RxDmaChannel_0) != TRUE);
                /* update the descriptor pointer for next packet */
                IfxGeth_Eth_freeReceiveBuffer(&g_GethBasic.drivers.geth,IfxGeth_RxDmaChannel_0);
                IfxGeth_dma_clearInterruptFlag(g_GethBasic.drivers.geth.gethSFR, IfxGeth_DmaChannel_0, IfxGeth_DmaInterruptFlag_transmitInterrupt);
                break;
            case 1:
                while (IfxGeth_Eth_isRxDataAvailable(&g_GethBasic.drivers.geth, IfxGeth_RxDmaChannel_1) != TRUE);
                IfxGeth_Eth_freeReceiveBuffer(&g_GethBasic.drivers.geth,IfxGeth_RxDmaChannel_1);
                IfxGeth_dma_clearInterruptFlag(g_GethBasic.drivers.geth.gethSFR, IfxGeth_DmaChannel_1, IfxGeth_DmaInterruptFlag_transmitInterrupt);
                break;
            case 2:
                while (IfxGeth_Eth_isRxDataAvailable(&g_GethBasic.drivers.geth, IfxGeth_RxDmaChannel_2) != TRUE);
                IfxGeth_Eth_freeReceiveBuffer(&g_GethBasic.drivers.geth,IfxGeth_RxDmaChannel_2);
                IfxGeth_dma_clearInterruptFlag(g_GethBasic.drivers.geth.gethSFR, IfxGeth_DmaChannel_2, IfxGeth_DmaInterruptFlag_transmitInterrupt);
                break;
            case 3:
                while (IfxGeth_Eth_isRxDataAvailable(&g_GethBasic.drivers.geth, IfxGeth_RxDmaChannel_3) != TRUE);
                IfxGeth_Eth_freeReceiveBuffer(&g_GethBasic.drivers.geth,IfxGeth_RxDmaChannel_3);
                IfxGeth_dma_clearInterruptFlag(g_GethBasic.drivers.geth.gethSFR, IfxGeth_DmaChannel_3, IfxGeth_DmaInterruptFlag_transmitInterrupt);
                break;
            default:
                break;
            }

            /* Compare Data received */
            for ( i = 0; i < packetLength; ++i)
            {

                if(channelIndex == 0){
                    if(channel0RxBuffer1[packet][i] != channel0TxBuffer1[packet][i]){
                        result |= 1;
                        Ifx_Console_print("channel 0 index %d not match\r\n", i);
                    }
                }
                if(channelIndex == 1){
                    if(channel1RxBuffer1[packet][i] != channel1TxBuffer1[packet][i]){
                        result |= 1;
                        Ifx_Console_print("channel 1 index %d not match\r\n", i);
                    }
                }
                if(channelIndex == 2){
                    if(channel2RxBuffer1[packet][i] != channel2TxBuffer1[packet][i])
                        result |= 1;
                }
                if(channelIndex == 3){
                    if(channel3RxBuffer1[packet][i] != channel3TxBuffer1[packet][i])
                        result |= 1;
                }
            }
        }
    }
    Ifx_Console_print("%s result %s \r\n",__func__,((result==0)?"pass":"fail"));
}
