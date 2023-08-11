/**
1 * @file
 * Ethernet Interface for TC2xx
 * Author: Dian Tresna Nugraha <dian.nugraha@infineon.com>
 *         Holger Dienst <holger.dienst@infineon.com>
 * Copyright (c) 2015 Infineon Technologies A.G.
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/**
 * This file is a skeleton for developing Ethernet network interface
 * drivers for lwIP. Add code to the low_level functions and do a
 * search-and-replace for the word "ethernetif" to replace it with
 * something that better describes your network interface.
 */

#include "lwip/opt.h"

#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include "netif/etharp.h"
//#include "netif/ppp_oe.h"

#include "Ifx_Lwip.h"
#include "Ifx_Netif.h"
#include <GEth/Eth/IfxGEth_eth.h>
#include "GethBasicDemo.h"
#include <string.h>

#if !FEATURE_AVB && PROJECT == PROJECT_PAGASUS2
#else
#include "ptp_glue.h"
#endif

#include "Ifx_Console.h"
#include "Configuration.h"
#if FEATURE_MARVELL_6321
#include "Marvell88E6321Demo.h"
#endif
/* Define those to better describe your network interface. */
#define IFNAME0 'e'
#define IFNAME1 'n'
extern App_GethBasic g_GethBasic;
extern IfxGeth_Eth_Config config;
extern uint8 IFX_ALIGN(64) channel0TxBuffer1[IFXGETH_MAX_TX_DESCRIPTORS][IFXGETH_MAX_BUFFER_SIZE];
extern uint8 IFX_ALIGN(64) channel0RxBuffer1[IFXGETH_MAX_RX_DESCRIPTORS][IFXGETH_MAX_BUFFER_SIZE+4];
/* Transmit interrupt count on a channel */
extern volatile uint32 chn0TxInterrupt;
extern volatile uint32 chn1TxInterrupt;
extern volatile uint32 chn2TxInterrupt;
extern volatile uint32 chn3TxInterrupt;

/* Receive interrupt count on a channel */
extern volatile uint32 chn0RxInterrupt;
extern volatile uint32 chn1RxInterrupt;
extern volatile uint32 chn2RxInterrupt;
extern volatile uint32 chn3RxInterrupt;
extern volatile uint32 origRXcount ;
uint8 TX_PACKET_DEBUG = DEFAULT_TX_PACKET_DEBUG;
uint8 RX_PACKET_DEBUG = DEFAULT_RX_PACKET_DEBUG;

#if IFXGETH_NUM_OF_CHANNELS >=2
extern uint8 IFX_ALIGN(64) channel1TxBuffer1[IFXGETH_MAX_TX_DESCRIPTORS][IFXGETH_MAX_BUFFER_SIZE];
extern uint8 IFX_ALIGN(64) channel1RxBuffer1[IFXGETH_MAX_RX_DESCRIPTORS][IFXGETH_MAX_BUFFER_SIZE+4];
#endif
#if IFXGETH_NUM_OF_CHANNELS >=3
extern uint8 IFX_ALIGN(64) channel2TxBuffer1[IFXGETH_MAX_TX_DESCRIPTORS][IFXGETH_MAX_BUFFER_SIZE];
extern uint8 IFX_ALIGN(64) channel2RxBuffer1[IFXGETH_MAX_RX_DESCRIPTORS][IFXGETH_MAX_BUFFER_SIZE+4];
#endif
#if  IFXGETH_NUM_OF_CHANNELS >=4
extern uint8 IFX_ALIGN(64) channel3TxBuffer1[IFXGETH_MAX_TX_DESCRIPTORS][IFXGETH_MAX_BUFFER_SIZE];
extern uint8 IFX_ALIGN(64) channel3RxBuffer1[IFXGETH_MAX_RX_DESCRIPTORS][IFXGETH_MAX_BUFFER_SIZE+4];
#endif
#define printf Ifx_Console_print
#if FEATURE_MARVELL_6321
boolean receive_rmu = FALSE;
#endif
#define BIT(nr) (1UL << (nr))
/* DMA descriptor bits for RX normal descriptor (write back format) */
#define DWCEQOS_DMA_RDES1_IPCE    BIT(7)
#define DWCEQOS_DMA_RDES3_FD      BIT(29)
#define DWCEQOS_DMA_RDES3_LD      BIT(28)
#define DWCEQOS_DMA_RDES3_ES      BIT(15)
#define DWCEQOS_DMA_RDES3_PL(x)   ((x) & 0x7fff)

static uint32 GetRxFrameSize(IfxGeth_RxDescr *descr)
{
  uint32 len;

  uint32 rdes3 = descr->RDES3.U;
  uint32 rdes1 = descr->RDES1.U;

  if (((rdes3 & DWCEQOS_DMA_RDES3_ES) != 0U) ||
	  ((rdes1 & DWCEQOS_DMA_RDES1_IPCE) != 0U) ||
	  ((rdes3 & DWCEQOS_DMA_RDES3_LD) == 0U))
  {
	/* Error, this block is invalid */
	len = 0xFFFFFFFFU;
  }
  else
  {
    /* Subtract CRC */
	len = DWCEQOS_DMA_RDES3_PL(rdes3) - 4U;
  }
  if(len!=0xFFFFFFFFU )LWIP_DEBUGF(RX_PACKET_DEBUG,("rxframesize %ld\r\n",len));
  return len;
}

static void dump_packet(uint8 *buf,uint32 length)
{
/*
	printf("dump ethernet packet\r\n");
	for(int i = 0 ;i<length;i++)
		printf("%02X ",buf[i]);
	printf("\r\n");
	*/
	//[hint] if you want to show the packet data
	//       you can turn on flag and call this function
#if 0
	uint16 i;
	uint16 count=0;
	char buffer_str[256*4];
	memset(buffer_str, 0, sizeof(buffer_str));
	for(i = 0; i<length; i++)
	{
		if(i%16 == 0)
		{
			count=0;
			if(i > 0){
				Ifx_Console_print("%s\n\r", buffer_str);
				memset(buffer_str, 0, sizeof(buffer_str));
			}
			sprintf(buffer_str, "%02X", (uint8)buf[i]);

		}else{
			count++;
			if(count == 8){
				sprintf(buffer_str, "%s - %02X", buffer_str, (uint8)buf[i]);
			}else{
				sprintf(buffer_str, "%s %02X", buffer_str, (uint8)buf[i]);
			}
		}
	}
	Ifx_Console_print("%s\n\r", buffer_str);
#endif
}

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void low_level_init(netif_t *netif)
{
    int     i;

    /* set MAC hardware address length */
    netif->hwaddr_len = ETHARP_HWADDR_LEN;

    /* set MAC hardware address */
    for (i = 0; i < ETHARP_HWADDR_LEN; i++)
    {
        netif->hwaddr[i] = g_Lwip.eth_addr.addr[i];
    }

    /* maximum transfer unit */
    netif->mtu = 1500;

    /* device capabilities */
    /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
    /* we don't set the LINK_UP flag because we don't say when it is linked */
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP |NETIF_FLAG_LINK_UP;

    /* Do whatever else is needed to initialize interface. */
    {

        /* we set the LINK_UP flag if we have a valid link */
    	//because we have 2 mac on same switch, assumed that link is up
    	//if (!checkLink(1))
    	//	netif->flags |= NETIF_FLAG_LINK_UP;
    }
}


/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

static err_t low_level_output(netif_t *netif, pbuf_t *p)
{
    IfxGeth_Eth      *ethernetif = &g_GethBasic.drivers.geth;
    struct pbuf *q;
    uint8 *buf;
    uint32 framelen = 0U;
    uint32 origTXcount = chn0TxInterrupt;
    uint8 txDecrIdx = ethernetif->txChannel[0].curDescIdx;
    uint32 timeoutCount = 0;
    uint32 buffer1Address = 0;
    uint32 buffer1Length = 0;
    IfxGeth_Eth_FrameConfig frameConfig;

    //initiate transfer();
    LWIP_DEBUGF(TX_PACKET_DEBUG, ("origTXcount %d \r\n",origTXcount));

    if (p->tot_len > ethernetif->txChannel[IfxGeth_TxDmaChannel_0].txBuf1Size) {
    	LWIP_DEBUGF(TX_PACKET_DEBUG, ("buffer size < tot_len \r\n"));
    	return ERR_BUF;
    }
    IfxGeth_TxDescr *descr = (IfxGeth_TxDescr *)IfxGeth_Eth_getActualTxDescriptor((IfxGeth_Eth *)ethernetif, IfxGeth_TxDmaChannel_0);

    if (descr->TDES3.R.OWN)
    {
     // IfxGeth_dma_setTxDescriptorTailPointer(ethernetif->gethSFR, IfxGeth_TxDmaChannel_0, descr[1]);
    	//IfxGeth_Eth_waitTransmitBuffer
    	LWIP_DEBUGF(TX_PACKET_DEBUG, ("tx descr owned ,shuffled it\r\n"));
    	IfxGeth_Eth_shuffleTxDescriptor(ethernetif,IfxGeth_TxDmaChannel_0);
      return ERR_BUF;
    }
#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif
    LWIP_DEBUGF(TX_PACKET_DEBUG, ("%s %d\r\n",__func__,__LINE__));

    buf = channel0TxBuffer1[txDecrIdx];
    for(q = p; q != NULL; q = q->next) {
    /* Send the data from the pbuf to the interface, one pbuf at a
       time. The size of the data in each pbuf is kept in the ->len
       variable. */
        MEMCPY(buf+framelen, q->payload, q->len);
        framelen += (uint32)q->len;
        //buf += q->len;
        //Ifx_Console_print("low_level_output: [YC] data=%#x, %d\r\n", q->payload, q->len);
    }
    //Print Tx Buffer packet data
    //Ifx_Console_print("low_level_output: [YC] framelen[%d], p->tot_len[%d]\n\r Tx Buffer:", framelen, p->tot_len);
    //dump_packet(buf, framelen);
    //TODO signal that packet should be sent();
    frameConfig.packetLength = framelen;
	frameConfig.channelId = IfxGeth_TxDmaChannel_0;
	//debug information
	LWIP_DEBUGF(TX_PACKET_DEBUG, ("tx framelen %d\r\n",framelen));
	if(TX_PACKET_DEBUG == LWIP_DBG_ON)
		dump_packet(&channel0TxBuffer1[txDecrIdx],frameConfig.packetLength );
	LWIP_DEBUGF(TX_PACKET_DEBUG, ("\r\n"));

#if FEATURE_PTP
	if (p->tx_ts) {
		descr->TDES2.R.TTSE_TMWD = 1;
		buffer1Address = descr->TDES0.U;
		buffer1Length = descr->TDES2.R.B1L;
	}
#endif
	IfxGeth_Eth_sendFrame(&g_GethBasic.drivers.geth, &frameConfig);// send a frame and update the current descriptor

	while (chn0TxInterrupt != (origTXcount ))
	{
		timeoutCount++;
		if(timeoutCount >= GETHWAITTIMEOUT)
			break;
	};

#if FEATURE_PTP
	if (p->tx_ts)
	{
		while (descr->TDES3.R.OWN);
		p->tss = descr->TDES1.W.TTSH;
		p->tsss = descr->TDES0.W.TTSL;

		// Restore descriptor to read format
		descr->TDES0.U           = buffer1Address;
		descr->TDES1.U           = 0; /* buffer2 not used */
		descr->TDES2.R.B1L       = buffer1Length;
		descr->TDES2.R.VTIR      = 0; /* do not use VLAN tag */
		descr->TDES2.R.B2L       = 0; /* buffer2 not used */
		descr->TDES2.R.TTSE_TMWD = 0; /* timestamp not used */
		descr->TDES2.R.IOC       = 0; /* interrupt disabled */

		p->tx_ts = 0;
	}
#endif

	IfxGeth_dma_clearInterruptFlag(g_GethBasic.drivers.geth.gethSFR, IfxGeth_DmaChannel_0, IfxGeth_DmaInterruptFlag_transmitInterrupt);

#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

    LINK_STATS_INC(link.xmit);

    return ERR_OK;
}

static err_t low_level_output1(netif_t *netif, pbuf_t *p)
{
    IfxGeth_Eth      *ethernetif = &g_GethBasic.drivers.geth;
    struct pbuf *q;
    uint8 *buf;
    uint32 framelen = 0U;
    uint32 origTXcount = chn1TxInterrupt;
    uint8 txDecrIdx = ethernetif->txChannel[1].curDescIdx;
    uint32 timeoutCount = 0;
    IfxGeth_Eth_FrameConfig frameConfig;

    //initiate transfer();
    LWIP_DEBUGF(TX_PACKET_DEBUG, ("origTXcount %d \r\n",origTXcount));

    if (p->tot_len > ethernetif->txChannel[IfxGeth_TxDmaChannel_1].txBuf1Size) {
    	LWIP_DEBUGF(TX_PACKET_DEBUG, ("buffer size < tot_len \r\n"));
    	return ERR_BUF;
    }
    IfxGeth_TxDescr *descr = (IfxGeth_TxDescr *)IfxGeth_Eth_getActualTxDescriptor((IfxGeth_Eth *)ethernetif, IfxGeth_TxDmaChannel_1);

    if (descr->TDES3.R.OWN)
    {
     // IfxGeth_dma_setTxDescriptorTailPointer(ethernetif->gethSFR, IfxGeth_TxDmaChannel_1, descr[1]);
    	//IfxGeth_Eth_waitTransmitBuffer
    	LWIP_DEBUGF(TX_PACKET_DEBUG, ("tx descr owned ,shuffled it\r\n"));
    	IfxGeth_Eth_shuffleTxDescriptor(ethernetif,IfxGeth_TxDmaChannel_1);
      return ERR_BUF;
    }
#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif
    LWIP_DEBUGF(TX_PACKET_DEBUG, ("%s %d\r\n",__func__,__LINE__));

    buf = channel1TxBuffer1[txDecrIdx];
    for(q = p; q != NULL; q = q->next) {
    /* Send the data from the pbuf to the interface, one pbuf at a
       time. The size of the data in each pbuf is kept in the ->len
       variable. */
        MEMCPY(buf, q->payload, q->len);
        framelen += (uint32)q->len;
        buf += q->len;
    }

    //TODO signal that packet should be sent();
    frameConfig.packetLength = framelen;
	frameConfig.channelId = IfxGeth_TxDmaChannel_1;
	//debug information
	LWIP_DEBUGF(TX_PACKET_DEBUG, ("tx framelen %d\r\n",framelen));
	if(TX_PACKET_DEBUG == LWIP_DBG_ON)
		dump_packet(&channel1TxBuffer1[txDecrIdx],frameConfig.packetLength );
	LWIP_DEBUGF(TX_PACKET_DEBUG, ("\r\n"));
	IfxGeth_Eth_sendFrame(&g_GethBasic.drivers.geth, &frameConfig);// send a frame and update the current descriptor
	while (chn1TxInterrupt != (origTXcount ))
	{
		timeoutCount++;
		if(timeoutCount >= GETHWAITTIMEOUT)
			break;
	};
	IfxGeth_dma_clearInterruptFlag(g_GethBasic.drivers.geth.gethSFR, IfxGeth_DmaChannel_1, IfxGeth_DmaInterruptFlag_transmitInterrupt);

#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

    LINK_STATS_INC(link.xmit);

    return ERR_OK;
}

#if PROJECT == PROJECT_PAGASUS2
static err_t low_level_output2(netif_t *netif, pbuf_t *p)
{
    IfxGeth_Eth      *ethernetif = &g_GethBasic.drivers.geth;
    struct pbuf *q;
    uint8 *buf;
    uint32 framelen = 0U;
    uint32 origTXcount = chn2TxInterrupt;
    uint8 txDecrIdx = ethernetif->txChannel[2].curDescIdx;
    uint32 timeoutCount = 0;
    uint32 buffer1Address = 0;
    uint32 buffer1Length = 0;
    IfxGeth_Eth_FrameConfig frameConfig;

    //initiate transfer();
    LWIP_DEBUGF(TX_PACKET_DEBUG, ("origTXcount %d \r\n",origTXcount));

    if (p->tot_len > ethernetif->txChannel[IfxGeth_TxDmaChannel_2].txBuf1Size) {
    	LWIP_DEBUGF(TX_PACKET_DEBUG, ("buffer size < tot_len \r\n"));
    	return ERR_BUF;
    }
    IfxGeth_TxDescr *descr = (IfxGeth_TxDescr *)IfxGeth_Eth_getActualTxDescriptor((IfxGeth_Eth *)ethernetif, IfxGeth_TxDmaChannel_2);

    if (descr->TDES3.R.OWN)
    {
     // IfxGeth_dma_setTxDescriptorTailPointer(ethernetif->gethSFR, IfxGeth_TxDmaChannel_2, descr[1]);
    	//IfxGeth_Eth_waitTransmitBuffer
    	LWIP_DEBUGF(TX_PACKET_DEBUG, ("tx descr owned ,shuffled it\r\n"));
    	IfxGeth_Eth_shuffleTxDescriptor(ethernetif,IfxGeth_TxDmaChannel_2);
      return ERR_BUF;
    }
#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif
    LWIP_DEBUGF(TX_PACKET_DEBUG, ("%s %d\r\n",__func__,__LINE__));

    buf = channel2TxBuffer1[txDecrIdx];
    for(q = p; q != NULL; q = q->next) {
    /* Send the data from the pbuf to the interface, one pbuf at a
       time. The size of the data in each pbuf is kept in the ->len
       variable. */
        MEMCPY(buf+framelen, q->payload, q->len);
        framelen += (uint32)q->len;
        //buf += q->len;
        //Ifx_Console_print("low_level_output: [YC] data=%#x, %d\r\n", q->payload, q->len);
    }
    //Print Tx Buffer packet data
    //Ifx_Console_print("low_level_output: [YC] framelen[%d], p->tot_len[%d]\n\r Tx Buffer:", framelen, p->tot_len);
    //dump_packet(buf, framelen);
    //TODO signal that packet should be sent();
    frameConfig.packetLength = framelen;
	frameConfig.channelId = IfxGeth_TxDmaChannel_2;
	//debug information
	LWIP_DEBUGF(TX_PACKET_DEBUG, ("tx framelen %d\r\n",framelen));
	if(TX_PACKET_DEBUG == LWIP_DBG_ON)
		dump_packet(&channel2TxBuffer1[txDecrIdx],frameConfig.packetLength );
	LWIP_DEBUGF(TX_PACKET_DEBUG, ("\r\n"));

#if FEATURE_PTP
	if (p->tx_ts) {
		descr->TDES2.R.TTSE_TMWD = 1;
		buffer1Address = descr->TDES0.U;
		buffer1Length = descr->TDES2.R.B1L;
	}
#endif

	IfxGeth_Eth_sendFrame(&g_GethBasic.drivers.geth, &frameConfig);// send a frame and update the current descriptor

	while (chn2TxInterrupt != (origTXcount ))
	{
		timeoutCount++;
		if(timeoutCount >= GETHWAITTIMEOUT)
			break;
	};

#if FEATURE_PTP
	if (p->tx_ts)
	{
		while (descr->TDES3.R.OWN);
		p->tss = descr->TDES1.W.TTSH;
		p->tsss = descr->TDES0.W.TTSL;

		// Restore descriptor to read format
		descr->TDES0.U           = buffer1Address;
		descr->TDES1.U           = 0; /* buffer2 not used */
		descr->TDES2.R.B1L       = buffer1Length;
		descr->TDES2.R.VTIR      = 0; /* do not use VLAN tag */
		descr->TDES2.R.B2L       = 0; /* buffer2 not used */
		descr->TDES2.R.TTSE_TMWD = 0; /* timestamp not used */
		descr->TDES2.R.IOC       = 0; /* interrupt disabled */

		p->tx_ts = 0;
	}
#endif

	IfxGeth_dma_clearInterruptFlag(g_GethBasic.drivers.geth.gethSFR, IfxGeth_DmaChannel_2, IfxGeth_DmaInterruptFlag_transmitInterrupt);

#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

    LINK_STATS_INC(link.xmit);

    return ERR_OK;
}
#endif

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static pbuf_t *low_level_input(netif_t *netif)
{
    IfxGeth_Eth *ethernetif = &g_GethBasic.drivers.geth;
    pbuf_t *p = NULL;
    pbuf_t *q;
    uint8 *buffer;
    uint32   len;
    uint8 rxDecrIdx = ethernetif->rxChannel[0].curDescIdx;
    int has_ts;

	g_GethBasic.drivers.geth.gethSFR->MAC_RXQ_CTRL1.B.UPQ = IfxGeth_RxDmaChannel_0;
	//if(origRXcount!=chn0RxInterrupt)
	//LWIP_DEBUGF(QCI_PTP_DEBUG, ("*\n\r"));
	if(IfxGeth_Eth_isRxDataAvailable(ethernetif, IfxGeth_RxDmaChannel_0) == TRUE)
    {
 
		origRXcount = chn0RxInterrupt;
		//we always read base descriptor,that's why we only get rx packet after circular descriptor full
		//IfxGeth_RxDescr *descr = IfxGeth_Eth_getBaseRxDescriptor(ethernetif, IfxGeth_RxDmaChannel_0);
		//len = GetRxFrameSize(descr);
		//try to read current descriptor
		IfxGeth_RxDescr descr = ethernetif->rxChannel[0].rxDescrList->descr[rxDecrIdx];
		len = GetRxFrameSize(&descr);
		LWIP_DEBUGF(RX_PACKET_DEBUG, ("GetRxFrameSize len %d \r\n",len));

		if(len > 0U && len <= (IFXGETH_MAX_BUFFER_SIZE+4))
		{
		#if ETH_PAD_SIZE
			len += ETH_PAD_SIZE;    /* allow room for Ethernet padding */
		#endif
			/* We allocate a pbuf chain of pbufs from the pool. */
			p = pbuf_alloc(PBUF_RAW, (u16_t)len, PBUF_POOL);
			if (p != NULL)
			{
		#if ETH_PAD_SIZE
				pbuf_header(p, -ETH_PAD_SIZE);  /* drop the padding word */
		#endif
				buffer = (uint8 *)IfxGeth_Eth_getReceiveBuffer(ethernetif, IfxGeth_RxDmaChannel_0);
				LWIP_DEBUGF(RX_PACKET_DEBUG, ("channel0RxBuffer1 0x%08x\r\n",&channel0RxBuffer1[rxDecrIdx]));
                //Print Rx buffer packet data
                //Ifx_Console_print("low_level_input: [YC]  IfxGeth_Eth_getReceiveBuffer. length[%d]\n\r Rx buffer:",len);
				//dump_packet(buffer,len);
				LWIP_DEBUGF(RX_PACKET_DEBUG, ("buffer 0x%08x\r\n",buffer));
				if(RX_PACKET_DEBUG == LWIP_DBG_ON)
				{
					if(RX_PACKET_DEBUG == LWIP_DBG_ON)
					{
					#if ETH_PAD_SIZE
						LWIP_DEBUGF(RX_PACKET_DEBUG, ("rx buffer from array \r\n"));
						//dump_packet(channel0RxBuffer1[rxDecrIdx],len-ETH_PAD_SIZE);
						LWIP_DEBUGF(RX_PACKET_DEBUG, ("rx buffer from IfxGeth_Eth_getReceiveBuffer\r\n",buffer));
						dump_packet(buffer,len-ETH_PAD_SIZE);
					#else
						dump_packet(channel0RxBuffer1[rxDecrIdx],len);
						LWIP_DEBUGF(RX_PACKET_DEBUG, ("rx buffer from IfxGeth_Eth_getReceiveBuffer",buffer));
						dump_packet(buffer,len);
					#endif
					}
				}
				len = 0U;
				/* We iterate over the pbuf chain until we have read the entire
				 * packet into the pbuf. */
				for (q = p; q != NULL; q = q->next)
				{
				  /* Read enough bytes to fill this pbuf in the chain. The
				   * available data in the pbuf is given by the q->len
				   * variable.
				   * This does not necessarily have to be a memcpy, you can also preallocate
				   * pbufs for a DMA-enabled MAC and after receiving truncate it to the
				   * actually received size. In this case, ensure the tot_len member of the
				   * pbuf is the sum of the chained pbuf len members.
				   */
				  LWIP_DEBUGF(RX_PACKET_DEBUG, ("%s %d\r\n",__func__,__LINE__));
				  MEMCPY(q->payload, buffer, q->len);
				  buffer = &buffer[q->len];
				}
				LWIP_DEBUGF(RX_PACKET_DEBUG, ("%s %d\r\n",__func__,__LINE__));
		#if ETH_PAD_SIZE
				pbuf_header(p, ETH_PAD_SIZE);    /* Reclaim the padding word */
		#endif

#if FEATURE_PTP
				has_ts = IfxGeth_Eth_getActualRxDescriptor(ethernetif, IfxGeth_RxDmaChannel_0)->RDES1.W.TSA;
#endif

				IfxGeth_Eth_freeReceiveBuffer(ethernetif, IfxGeth_RxDmaChannel_0);
				LWIP_DEBUGF(RX_PACKET_DEBUG, ("%s %d\r\n",__func__,__LINE__));
				LINK_STATS_INC(link.recv);

#if FEATURE_PTP
				// Process context descriptor
				if (has_ts) {
					// Wait for RX descriptor
					while (IfxGeth_Eth_isRxDataAvailable(ethernetif, IfxGeth_RxDmaChannel_0) != 1);

					// If RX descriptor is of context type...
					if (IfxGeth_Eth_getActualRxDescriptor(ethernetif, IfxGeth_RxDmaChannel_0)->RDES3.C.CTXT == 1) {
						p->tss = IfxGeth_Eth_getActualRxDescriptor(ethernetif, IfxGeth_RxDmaChannel_0)->RDES1.C.RTSH;
						p->tsss = IfxGeth_Eth_getActualRxDescriptor(ethernetif, IfxGeth_RxDmaChannel_0)->RDES0.C.RTSL;
						// Restore descriptor type to read format
						IfxGeth_Eth_getActualRxDescriptor(ethernetif, IfxGeth_RxDmaChannel_0)->RDES0.U
								= (uint32)config.dma.rxChannel[0].rxBuffer1Size * (uint32)rxDecrIdx
								+ (uint32)config.dma.rxChannel[0].rxBuffer1StartAddress;
						IfxGeth_Eth_getActualRxDescriptor(ethernetif, IfxGeth_RxDmaChannel_0)->RDES1.U = 0;
						IfxGeth_Eth_getActualRxDescriptor(ethernetif, IfxGeth_RxDmaChannel_0)->RDES2.U = 0;
						IfxGeth_Eth_freeReceiveBuffer(ethernetif, IfxGeth_RxDmaChannel_0);
						LWIP_DEBUGF(RX_PACKET_DEBUG, ("%s %d\r\n",__func__,__LINE__));
						LINK_STATS_INC(link.recv);
					}
				}
#endif
			}
			else
				LWIP_DEBUGF(RX_PACKET_DEBUG, ("%s %d pbuf is null\r\n",__func__,__LINE__));
		}
		else
		{
		  /* Discard frame */
		  //IfxGeth_Eth_shuffleRxDescriptor(ethernetif, IfxGeth_RxDmaChannel_0);
			LWIP_DEBUGF(RX_PACKET_DEBUG, (" /* Discard frame */ \r\n"));

			LINK_STATS_INC(link.memerr);
			LINK_STATS_INC(link.drop);
		}
    }
    /* Wake up RX by writing tail pointer */
    IfxGeth_dma_setRxDescriptorTailPointer(ethernetif->gethSFR, IfxGeth_RxDmaChannel_0, (uint32)&IfxGeth_Eth_rxDescrList[IFXGETH_MAX_RX_DESCRIPTORS]);
	IfxGeth_dma_clearInterruptFlag(g_GethBasic.drivers.geth.gethSFR, IfxGeth_DmaChannel_0, IfxGeth_DmaInterruptFlag_receiveInterrupt);

    return p;
}


/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
err_t ifx_netif_input(netif_t *netif)
{
    //IfxEth *ethernetif = netif->state;
    eth_hdr_t *ethhdr;
    pbuf_t    *p;

    /* move received packet into a new pbuf */
    p = low_level_input(netif);

    /* no packet could be read, silently ignore this */
    if (p == NULL)
    {
        //LWIP_DEBUGF(NETIF_DEBUG, ("ifx_netif_input: p == NULL!\n\r"));
        return ERR_OK;
    }

    /* points to packet payload, which starts with an Ethernet header */
    ethhdr = p->payload;
    /* pirun lets dump some data */
    LWIP_DEBUGF(NETIF_DEBUG, ("ifx_netif_input:\n\r"));
    LWIP_DEBUGF(NETIF_DEBUG, ("len %d tot_len %d\n\r",p->len,p->tot_len));
    LWIP_DEBUGF(NETIF_DEBUG, ("ethhdr->type: 0x%x dst %02x %02x %02x %02x %02x %02x src %02x %02x %02x %02x %02x %02x\n\r",ethhdr->type,
    		ethhdr->dest.addr[0],ethhdr->dest.addr[1],ethhdr->dest.addr[2],ethhdr->dest.addr[3],ethhdr->dest.addr[4],ethhdr->dest.addr[5],
    		ethhdr->src.addr[0],ethhdr->src.addr[1],ethhdr->src.addr[2],ethhdr->src.addr[3],ethhdr->src.addr[4],ethhdr->src.addr[5])
    		);
    if(RX_PACKET_DEBUG == LWIP_DBG_ON)
    {
	#if ETH_PAD_SIZE
		dump_packet((p->payload+ETH_PAD_SIZE),p->len-ETH_PAD_SIZE);
	#else
		dump_packet(p->payload,p->len);
	#endif
    }
    /* end */

    switch (htons(ethhdr->type))
    {
    /* IP or ARP packet? */
    case ETHTYPE_IP:
    case ETHTYPE_ARP:
    case ETHTYPE_MSRP:
    case ETHTYPE_MVRP:
#if PPPOE_SUPPORT
    /* PPPoE packet? */
    case ETHTYPE_PPPOEDISC:
    case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */

        /* full packet send to tcpip_thread to process */
        if (netif->input(p, netif) != ERR_OK)
        {
            LWIP_DEBUGF(NETIF_DEBUG, ("ifx_netif_input: IP input error\n\r"));
            pbuf_free(p);
            p = NULL;
        }

        break;


#if FEATURE_PTP
    case ETHTYPE_PTP:
        //LWIP_DEBUGF(QCI_PTP_DEBUG, ("ifx_netif_input: ETHTYPE_PTP type received...\r\n"));
        ptpRxEnqueue(p);
        pbuf_free(p);
        p = NULL;
        break;
#endif
#if FEATURE_MARVELL_6321
    case ETHTYPE_QINQ:
      Ifx_Console_print("receive ETHTYPE_QINQ\n\r");
      rmu_rx_queue_enqueue(p);
      receive_rmu = TRUE;
      pbuf_free(p);
      p = NULL;
      break;
#endif
    default:
        if (netif->input(p, netif) != ERR_OK)
        {
            LWIP_DEBUGF(NETIF_DEBUG, ("ifx_netif_input: IP input error\n\r"));
            pbuf_free(p);
            p = NULL;
        }
        break;
    }

    return ERR_OK;
}


/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t ifx_netif_init(netif_t *netif)
{
    IfxGeth_Eth *ethernetif;

    LWIP_ASSERT("netif != NULL", (netif != NULL));
    LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_TRACE, ("ifx_netif_init ( %#x)\n\r", netif));

    ethernetif = IfxEth_get();

    if (ethernetif == NULL)
    {
        LWIP_DEBUGF(NETIF_DEBUG, ("ifx_netif_init: out of memory\n\r"));
        return ERR_MEM;
    }

#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
	#if SECURITY_GATEWAY
    netif->hostname = "SecurityGateway";
    #else
    netif->hostname = "Gateway";
	#endif
#endif /* LWIP_NETIF_HOSTNAME */

    /*
     * Initialize the snmp variables and counters inside the struct netif.
     * The last argument should be replaced with your link speed, in units
     * of bits per second.
     */
    //NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);

    netif->state      = ethernetif;
    netif->name[0]    = IFNAME0;
    netif->name[1]    = IFNAME1;
    /* We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...) */
    netif->output     = etharp_output;
    netif->linkoutput = low_level_output;
    netif->linkoutput1 = low_level_output1;
#if PROJECT == PROJECT_PAGASUS2
    netif->linkoutput2 = low_level_output2;
#endif
    /* initialize the hardware */
    low_level_init(netif);

    return ERR_OK;
}
