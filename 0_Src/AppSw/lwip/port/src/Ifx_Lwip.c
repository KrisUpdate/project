/**
 * \file Ifx_Lwip.c
 * \brief
 *
 * \version V0.1
 * \copyright Copyright (c) 2015 Infineon Technologies AG. All rights reserved.
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
#include <Cpu/Std/Ifx_Types.h>
#include <Cpu/Std/IfxCpu.h>
#include "netio.h"
#include "ping.h"
#include "Ifx_Lwip.h"
#include "lwiperf.h"
#include "lwipopts.h"
#include "Ifx_Netif.h"
#include "tcp_priv.h"
#include <GEth/Eth/IfxGeth_Eth.h>
#include "ConfigurationIsr.h"
#include "configuration.h"
#include <string.h>
#include <stdarg.h>
#if !FEATURE_AVB && PROJECT == PROJECT_PAGASUS2
#else
#include "avb_conf.h"
#endif
#include "Project_definition.h"

void qci_udp_server_init(void);

#define printf Ifx_Console_print

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define IFX_LWIP_TIMER_TICK_MS      (1U)    // number of timer ticks per millisecond

#define IFX_LWIP_ARP_PERIOD         (ARP_TMR_INTERVAL / IFX_LWIP_TIMER_TICK_MS)
#define IFX_LWIP_TCP_FAST_PERIOD    (TCP_FAST_INTERVAL / IFX_LWIP_TIMER_TICK_MS)
#define IFX_LWIP_TCP_SLOW_PERIOD    (TCP_SLOW_INTERVAL / IFX_LWIP_TIMER_TICK_MS)
#define IFX_LWIP_DHCP_COARSE_PERIOD (DHCP_COARSE_TIMER_MSECS / IFX_LWIP_TIMER_TICK_MS)
#define IFX_LWIP_DHCP_FINE_PERIOD   (DHCP_FINE_TIMER_MSECS / IFX_LWIP_TIMER_TICK_MS)
#define IFX_LWIP_LINK_PERIOD        (100U / IFX_LWIP_TIMER_TICK_MS) /* 100 ms */

#define IFX_LWIP_FLAG_ARP           (1U << 1)
#define IFX_LWIP_FLAG_TCP_FAST      (1U << 2)
#define IFX_LWIP_FLAG_TCP_SLOW      (1U << 3)
#define IFX_LWIP_FLAG_LINK          (1U << 4)
#define IFX_LWIP_FLAG_DHCP_COARSE   (1U << 5)
#define IFX_LWIP_FLAG_DHCP_FINE     (1U << 6)
extern boolean boot_msg_flag;
#if 0
#define PRINTF_ALWAYS(x)        DEBUG_PRINTF_ALWAYS(x)
#define PRINTF_BOOT(x)       ({if(boot_msg_flag){DEBUG_PRINTF_ALWAYS(x);}})
#else
#define PRINTF_BOOT(debug, message)       ({if(boot_msg_flag){LWIP_DEBUGF(debug, message);}})
#endif


/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/
#if PROJECT == PROJECT_PAGASUS2 || PROJECT == PROJECT_V3NA
struct udp_pcb *gw_sendback_pcb;
#endif

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
#if LWIP_VAR_LOCATION == 0
	#if defined(__GNUC__)
	#pragma section ".bss_cpu0" awc0
	#endif
	#if defined(__TASKING__)
	#pragma section farbss "bss_cpu0"
	#pragma section fardata "data_cpu0"
	#endif
	#if defined(__DCC__)
	#pragma section DATA ".data_cpu0" ".bss_cpu0" far-absolute RW
	#endif
#elif LWIP_VAR_LOCATION == 1
	#if defined(__GNUC__)
	#pragma section ".bss_cpu1" awc1
	#endif
	#if defined(__TASKING__)
	#pragma section farbss "bss_cpu1"
	#pragma section fardata "data_cpu1"
	#endif
	#if defined(__DCC__)
	#pragma section DATA ".data_cpu1" ".bss_cpu1" far-absolute RW
	#endif
#elif LWIP_VAR_LOCATION == 2
	#if defined(__GNUC__)
	#pragma section ".bss_cpu2" awc2
	#endif
	#if defined(__TASKING__)
	#pragma section farbss "bss_cpu2"
	#pragma section fardata "data_cpu2"
	#endif
	#if defined(__DCC__)
	#pragma section DATA ".data_cpu2" ".bss_cpu2" far-absolute RW
	#endif
#else
#error "Set LWIP_VAR_LOCATIONs to a valid value!"
#endif

Ifx_Lwip g_Lwip={.initialized = FALSE};;
IfxGeth_Eth   g_IfxEth;
ip_addr_t ping_target_ip ;

#if defined(__GNUC__)
#pragma section
#endif
#if defined(__TASKING__)
#pragma section farbss restore
#pragma section fardata restore
#endif
#if defined(__DCC__)
#pragma section DATA RW
#endif

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

#define Ifx_Lwip_timerIncr(var, PERIOD, FLAG) \
    {                                         \
        var += 1;                             \
        if (var >= PERIOD)                    \
        {                                     \
            var         = 0;                  \
            timerFlags |= FLAG;               \
        }                                     \
    }

/** \brief Timer interrupt callback */
void Ifx_Lwip_onTimerTick(void)
{
    Ifx_Lwip *lwip       = &g_Lwip;
    uint16    timerFlags = lwip->timerFlags;

    Ifx_Lwip_timerIncr(lwip->timer.arp, IFX_LWIP_ARP_PERIOD, IFX_LWIP_FLAG_ARP);

    Ifx_Lwip_timerIncr(lwip->timer.tcp_fast, IFX_LWIP_TCP_FAST_PERIOD, IFX_LWIP_FLAG_TCP_FAST);
    Ifx_Lwip_timerIncr(lwip->timer.tcp_slow, IFX_LWIP_TCP_SLOW_PERIOD, IFX_LWIP_FLAG_TCP_SLOW);

    Ifx_Lwip_timerIncr(lwip->timer.dhcp_coarse, IFX_LWIP_DHCP_COARSE_PERIOD, IFX_LWIP_FLAG_DHCP_COARSE);
    Ifx_Lwip_timerIncr(lwip->timer.dhcp_fine, IFX_LWIP_DHCP_FINE_PERIOD, IFX_LWIP_FLAG_DHCP_FINE);

    Ifx_Lwip_timerIncr(lwip->timer.link, IFX_LWIP_LINK_PERIOD, IFX_LWIP_FLAG_LINK);

    lwip->timerFlags = timerFlags;
}


/** \brief Polling the timer event flags */
void Ifx_Lwip_pollTimerFlags(void)
{
    Ifx_Lwip *lwip = &g_Lwip;
    uint16    timerFlags;

    /* disable interrupts */
    boolean interruptState = IfxCpu_disableInterrupts();

    timerFlags       = lwip->timerFlags;
    lwip->timerFlags = 0;

    /* enable interrupts again */
    IfxCpu_restoreInterrupts(interruptState);

    if (timerFlags & IFX_LWIP_FLAG_DHCP_COARSE)
    {
        dhcp_coarse_tmr();
    }

    if (timerFlags & IFX_LWIP_FLAG_DHCP_FINE)
    {
        dhcp_fine_tmr();
    }

    if (timerFlags & IFX_LWIP_FLAG_TCP_FAST)
    {
        tcp_fasttmr();
    }

    if (timerFlags & IFX_LWIP_FLAG_TCP_SLOW)
    {
        tcp_slowtmr();
    }

    if (timerFlags & IFX_LWIP_FLAG_ARP)
    {
        etharp_tmr();
    }
#if PROJECT == PROJECT_PAGASUS2 || PROJECT == PROJECT_V31C
#ifdef TC397B
/* Use RGMII */
	if (timerFlags & IFX_LWIP_FLAG_LINK)
	{
		IfxGeth_Eth *ethernetif = g_Lwip.netif.state;
		// we set the correct duplexMode
		IfxGeth_mac_setDuplexMode(ethernetif->gethSFR, IfxGeth_DuplexMode_fullDuplex);
		// 1000MBit speed
		IfxGeth_mac_setLineSpeed(ethernetif->gethSFR, IfxGeth_LineSpeed_1000Mbps);
		netif_set_link_up(&g_Lwip.netif);
	 }
#else
	if (timerFlags & IFX_LWIP_FLAG_LINK) {
		if (!IfxEth_isLinkActive())
			netif_set_link_down(&g_Lwip.netif);
		else
			netif_set_link_up(&g_Lwip.netif);
	}
#endif
#endif
#if 0 //because we have 2 mac on same switch,assumed that link always up
    if (timerFlags & IFX_LWIP_FLAG_LINK)
    {
    	if (!checkLink(1))
    	{
    	    //LWIP_DEBUGF(IFX_LWIP_DEBUG, ("netif_set_link_down!\n\r"));
    		netif_set_link_down(&g_Lwip.netif);
    	}
    	else
    	{
    	    //LWIP_DEBUGF(IFX_LWIP_DEBUG, ("netif_set_link_up!\n\r"));
    		netif_set_link_up(&g_Lwip.netif);
    	}
    }
#endif

}


/** \brief Polling the ETH receive event flags */
void Ifx_Lwip_pollReceiveFlags(void)
{
    /**
     * We are assuming that the only interrupt source is an incoming packet
     */
    //while (ethernetif_tc29x_timerFlags_interrupt())
    {
        ifx_netif_input(&g_Lwip.netif);
    }
}


//________________________________________________________________________________________
// INITIALIZATION FUNCTION

/** \brief LWIP initialization function
 *
 * The followings are executed: */
ip_addr_t g_default_ipaddr;
void Ifx_Lwip_init(eth_addr_t ethAddr)
{
    ip_addr_t  default_netmask, default_gw;
    IP4_ADDR(&default_gw, 192,168,1,1);
#if SECURITY_GATEWAY
    IP4_ADDR(&g_default_ipaddr, 192,168,1,202);
#else
    IP4_ADDR(&g_default_ipaddr, 192,168,1,100);
#if AVB_NUM_SOURCES
    IP4_ADDR(&g_default_ipaddr, 192,168,1,200);
#else
    IP4_ADDR(&g_default_ipaddr, 192,168,1,150);
#endif
#endif
    IP4_ADDR(&default_netmask, 255,255,255,0);
    if(g_Lwip.initialized == TRUE )
    {
        LWIP_DEBUGF(IFX_LWIP_DEBUG, ("Ifx_Lwip_init done before!\n\r"));
    	return ;
    }
    LWIP_DEBUGF(IFX_LWIP_DEBUG, ("Ifx_Lwip_init start!\n\r"));

    /** - initialise LWIP (lwip_init()) */
    lwip_init();

    /** - initialise and add a \ref netif */
    g_Lwip.eth_addr = ethAddr;
    netif_add(&g_Lwip.netif, &g_default_ipaddr, &default_netmask, &default_gw,
        (void *)0, ifx_netif_init, ethernet_input);
    netif_set_default(&g_Lwip.netif);
    netif_set_up(&g_Lwip.netif);
    g_Lwip.initialized = TRUE;
#if 0
    /** - assign \ref dhcp to \ref netif */
    dhcp_set_struct(&g_Lwip.netif, &g_Lwip.dhcp);
    /* we start the dhcp always here also when we don't have a link here */
    dhcp_start(&g_Lwip.netif);
    LWIP_DEBUGF(IFX_LWIP_DEBUG, ("DHCP\n\r"));

#else
	/* Static mode. */
	printf("Static IP Address Assigned\r\n");
#endif
	netif_set_link_up(&g_Lwip.netif);
    LWIP_DEBUGF(IFX_LWIP_DEBUG, ("Ifx_Lwip_init end!\n\r"));

#if 1
    {   /** - initialize some network applications (ping, echo, httpd) */
        void echo_init(void);
        void httpd_init(void);
        echo_init();
        httpd_init();
        lwiperf_start_tcp_server_default(NULL,NULL);
        netio_init();
        ping_init(&ping_target_ip);
        qci_udp_server_init();
        //qci_v31g_server_init();
        //qci_udp_client_init();   //TODO: add quanta udp client init
    }
#endif
}
/**
 * \ingroup interrupts
 *
 * This interrupt is raised by the ethernet. The initialization is done by IfxEth_init().
 *
 * \isrProvider \ref ISR_PROVIDER_ETH
 * \isrPriority \ref ISR_PRIORITY_ETH
 *
 */
#if 0
IFX_INTERRUPT(ISR_Eth, ISR_PROVIDER_ETHERNET, ISR_PRIORITY_ETHERNET_TX)
{
	IfxGeth_Eth *eth = IfxEth_get();
/*    IfxSrc_clearRequest(&SRC_ETH);*/

    if (IfxEth_isTxInterrupt(eth) != FALSE)
    {
        IfxEth_clearTxInterrupt(eth);
        eth->isrTxCount++;
    }

    if (IfxEth_isRxInterrupt(eth) != FALSE)
    {
        IfxEth_clearRxInterrupt(eth);
        eth->isrRxCount++;
    }

    eth->txDiff = eth->txCount - eth->isrTxCount;
    eth->rxDiff = eth->rxCount - eth->isrRxCount;
    eth->isrCount = eth->isrCount + 1;
}
#endif

/** \brief LWIP initialization function
 *
 * The followings are executed: */
#if PROJECT == PROJECT_PAGASUS2 || PROJECT == PROJECT_V31C || PROJECT == PROJECT_V3NA
#include "GethBasicDemo.h"

#if 0
boolean Ifx_Lwip_IsDhcpBound(void) {
	if (g_Lwip.netif.dhcp != NULL) {
		if (g_Lwip.netif.dhcp->state == DHCP_BOUND) {
			return TRUE;
		}
	}
	return FALSE;
}
#endif

IFX_EXTERN void Ifx_Lwip_init_nv(boolean is_restart, eth_addr_t ethAddr) {
	ip_addr_t default_ipaddr, default_netmask, default_gw;

    GethBasicDemo_firstinit();
#if PROJECT == PROJECT_V31C || PROJECT == PROJECT_PAGASUS2
    GethBasicDemo_init(ethAddr);
#else
    GethBasicDemo_init();
#endif
	IP4_ADDR(&default_gw, 10, 0, 0, 1);
	IP4_ADDR(&default_ipaddr, 10, 0, 0, 4);
	IP4_ADDR(&default_netmask, 255, 255, 255, 255);
	//LWIP_DEBUGF(IFX_LWIP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE, ("Ifx_Lwip_init start!\n"));
Ifx_Console_print("[LWIP] Ifx_Lwip_init_nv\r\n");
	if (!is_restart) {
		/** - initialise LWIP (lwip_init()) */
        PRINTF_BOOT(IFX_LWIP_DEBUG, ("lwip_init!\n\r"));
		lwip_init();
	}

	/** - initialise and add a \ref netif */
	g_Lwip.eth_addr = ethAddr;
	netif_add(&g_Lwip.netif, &default_ipaddr, &default_netmask, &default_gw,
			(void *) 0, ifx_netif_init, ethernet_input);
	netif_set_default(&g_Lwip.netif);
	netif_set_up(&g_Lwip.netif);

	//LWIP_DEBUGF(IFX_LWIP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE, ("Ifx_Lwip_init end!\n"));

	if (!is_restart) {

		/** - initialize some network applications (ping, echo, httpd) */
		void ping_init(const ip_addr_t* ping_addr);
		void echo_init(void);
		void httpd_init(void);
		if (!(g_Lwip.initialized)) //ycdebug
		{
            PRINTF_BOOT(IFX_LWIP_DEBUG,  ("ping_init!\n\r"));
			ping_init(&ping_target_ip);
			echo_init();
			//httpd_init();
		}
	}
	// mark the initialisation as finished

	g_Lwip.initialized = TRUE;
	qci_udp_server_init();
#if PROJECT == PROJECT_PAGASUS2 || PROJECT == PROJECT_V3NA
	gw_sendback_pcb = udp_new();
#endif
}

void Ifx_Lwip_deinit() {
	netif_set_down(&g_Lwip.netif);
	netif_remove(&g_Lwip.netif);
	g_Lwip.initialized = FALSE;
}

IFX_EXTERN void Ifx_Lwip_set_ip(uint8 ipaddr1, uint8 ipaddr2, uint8 ipaddr3,
		uint8 ipaddr4, uint8 mask1, uint8 mask2, uint8 mask3, uint8 mask4) {
	ip_addr_t default_ipaddr, default_netmask, default_gw;
	IP4_ADDR(&default_gw, ipaddr1, ipaddr2, ipaddr3, 1);
	IP4_ADDR(&default_ipaddr, ipaddr1, ipaddr2, ipaddr3, ipaddr4);
	IP4_ADDR(&default_netmask, mask1, mask2, mask3, mask4);
	netif_set_addr(&g_Lwip.netif, &default_ipaddr, &default_netmask,
			&default_gw);
}


static boolean is_dhcp_init = FALSE;

IFX_EXTERN void Ifx_Lwip_init_dhcp(void) {
	if (!is_dhcp_init) {
		/** - assign \ref dhcp to \ref netif */
		dhcp_set_struct(&g_Lwip.netif, &g_Lwip.dhcp);
		/* we start the dhcp always here also when we don't have a link here */
		dhcp_start(&g_Lwip.netif);
	}
	is_dhcp_init = TRUE;
}

IFX_EXTERN void Ifx_Lwip_deinit_dhcp(void) {
	if (is_dhcp_init) {
		/* by testing we should not need the release, it will bring the netif down */
		//dhcp_release(&g_Lwip.netif);

		dhcp_stop(&g_Lwip.netif);
	}
	is_dhcp_init = FALSE;
}
#elif PROJECT == PROJECT_V3AA

IFX_EXTERN void Ifx_Lwip_set_ip(uint8 ipaddr1, uint8 ipaddr2, uint8 ipaddr3,
		uint8 ipaddr4, uint8 mask1, uint8 mask2, uint8 mask3, uint8 mask4) {
	ip_addr_t default_ipaddr, default_netmask, default_gw;
	IP4_ADDR(&default_gw, ipaddr1, ipaddr2, ipaddr3, 1);
	IP4_ADDR(&default_ipaddr, ipaddr1, ipaddr2, ipaddr3, ipaddr4);
	IP4_ADDR(&default_netmask, mask1, mask2, mask3, mask4);
	netif_set_addr(&g_Lwip.netif, &default_ipaddr, &default_netmask,
			&default_gw);
}

IFX_EXTERN const ip_addr_t *get_ipv4_addr(const struct netif *netif)
{
#if LWIP_IPV4
    if (!netif_is_up(netif)) {
        return NULL;
    }

    if (!ip4_addr_isany(netif_ip4_addr(netif))) {
        return netif_ip_addr4(netif);
    }
#endif
    return NULL;
}

#endif
//________________________________________________________________________________________

