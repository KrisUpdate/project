/**
 * \file Ifx_Lwip.h
 * \brief lwIP (Light-weight TCP/IP Stack) Ported into AURIX
 * \ingroup lib_lwIP
 *
 *
 * \defgroup lib_lwIP LWIP: Light-weight TCP/IP Stack Ported for AURIX
 * \ingroup library
 */
#ifndef IFX_LWIP_H
#define IFX_LWIP_H

//________________________________________________________________________________________
// INCLUDES

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/ip.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
//#include "lwip/tcp_impl.h"
#include "lwip/dhcp.h"
#include "lwip/init.h"
#include "netif/etharp.h"
//#include "netif/ppp_oe.h"
#include <GEth/Eth/IfxGeth_Eth.h>

//________________________________________________________________________________________
// HELPER MACROS

#define MAC_ADDR(ptr, a, b, c, d, e, f) \
    (ptr)->addr[0] = a;                 \
    (ptr)->addr[1] = b;                 \
    (ptr)->addr[2] = c;                 \
    (ptr)->addr[3] = d;                 \
    (ptr)->addr[4] = e;                 \
    (ptr)->addr[5] = f;

//________________________________________________________________________________________
// TYPEDEFS

/* NOTE: these typedefs are shortcuts to the native LWIP struct types */
typedef struct netif    netif_t;
typedef struct pbuf     pbuf_t;
typedef struct dhcp     dhcp_t;
typedef struct eth_addr eth_addr_t;
typedef struct eth_hdr  eth_hdr_t;
typedef struct ip_addr  ipaddr_t;
typedef struct udp_pcb  udp_pcb_t;
typedef struct tcp_pcb  tcp_pcb_t;

//________________________________________________________________________________________
// DATA STRUCTURES

/** \brief Runtime structure of the AURIX LWIP stack */
typedef struct
{
    netif_t    netif;
#if LWIP_DHCP
    dhcp_t     dhcp;
#endif
    eth_addr_t eth_addr;
    struct
    {
        uint16 arp;
#if LWIP_DHCP
        uint16 dhcp_coarse;
        uint16 dhcp_fine;
#endif
        uint16 tcp_fast;
        uint16 tcp_slow;
        uint16 link;
    }               timer;

    volatile uint16 timerFlags;
    boolean initialized;
} Ifx_Lwip;

/** \brief Configuration structure for the AURIX LWIP stack */
typedef struct
{
    ip_addr_t  ipAddr;      /**< \brief IP address, e.g. : {192,168,220,123} */
    ip_addr_t  netMask;     /**< \brief Network mask, e.g. : {255,255,255,0} */
    ip_addr_t  gateway;     /**< \brief Gateway address, e.g. : {192,168,220,1} */
    eth_addr_t ethAddr;     /**< \brief Ethernet (MAC) address, e.g. : {0x10, 0x20, 0x30, 0x40, 0x50, 0x60} */
} Ifx_Lwip_Config;

//________________________________________________________________________________________
// GLOBAL VARIABLES

IFX_EXTERN Ifx_Lwip g_Lwip;
IFX_EXTERN IfxGeth_Eth   g_IfxEth;

//________________________________________________________________________________________
// FUNCTION PROTOTYPES

/** \addtogroup lib_lwIP
 * \{ */
IFX_EXTERN void     Ifx_Lwip_init(eth_addr_t ethAddr);
IFX_EXTERN void     Ifx_Lwip_onTimerTick(void);
IFX_EXTERN void     Ifx_Lwip_pollTimerFlags(void);
IFX_EXTERN void     Ifx_Lwip_pollReceiveFlags(void);
IFX_INLINE netif_t *Ifx_Lwip_getNetIf(void);
IFX_INLINE uint8   *Ifx_Lwip_getIpAddrPtr(void);
IFX_INLINE uint8   *Ifx_Lwip_getHwAddrPtr(void);

#if PROJECT == PROJECT_PAGASUS2 || PROJECT == PROJECT_V31C
IFX_EXTERN void Ifx_Lwip_deinit();

IFX_EXTERN void Ifx_Lwip_set_ip(uint8 ipaddr1, uint8 ipaddr2, uint8 ipaddr3, uint8 ipaddr4,
		uint8 mask1, uint8 mask2, uint8 mask3, uint8 mask4);
IFX_EXTERN void Ifx_Lwip_init_dhcp(void);
IFX_EXTERN void Ifx_Lwip_deinit_dhcp(void);
#elif PROJECT == PROJECT_V3AA
IFX_EXTERN void Ifx_Lwip_set_ip(uint8 ipaddr1, uint8 ipaddr2, uint8 ipaddr3, uint8 ipaddr4,
		uint8 mask1, uint8 mask2, uint8 mask3, uint8 mask4);
IFX_EXTERN const ip_addr_t *get_ipv4_addr(const struct netif *netif);
#endif

/* This function is used to get the low-level driver */
IFX_INLINE IfxGeth_Eth *IfxEth_get(void);

/** \} */
//________________________________________________________________________________________
// INLINE FUNCTION IMPLEMENTATIONS

/** \brief Returns pointer to the default network interface */
IFX_INLINE netif_t *Ifx_Lwip_getNetIf(void)
{
    return &g_Lwip.netif;
}


/** \brief Returns pointer to the actual IP address */
IFX_INLINE uint8 *Ifx_Lwip_getIpAddrPtr(void)
{
    return (uint8 *)&g_Lwip.netif.ip_addr.addr;
}


/** \brief Returns pointer to the actual H/W (MAC) address */
IFX_INLINE uint8 *Ifx_Lwip_getHwAddrPtr(void)
{
    return g_Lwip.eth_addr.addr;
}

/* This function is used to get the low-level driver */
IFX_INLINE IfxGeth_Eth *IfxEth_get(void)
{
    return &g_IfxEth;
}


#endif /* __IFX_LWIP_H__ */
