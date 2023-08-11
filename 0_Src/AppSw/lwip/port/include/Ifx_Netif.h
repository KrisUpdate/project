#ifndef IFX_LWIP_NETIF_H
#define IFX_LWIP_NETIF_H

err_t ifx_netif_init(struct netif *netif);
err_t ifx_netif_input(struct netif *netif);

#endif
