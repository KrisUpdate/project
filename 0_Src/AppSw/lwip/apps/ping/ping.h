#ifndef LWIP_PING_H
#define LWIP_PING_H

#include "lwip/ip_addr.h"

/**
 * PING_USE_SOCKETS: Set to 1 to use sockets, otherwise the raw api is used
 */
#ifndef PING_USE_SOCKETS
#define PING_USE_SOCKETS    LWIP_SOCKET
#endif

void ping_init(const ip_addr_t* ping_addr);
void ping_init_nv(const ip_addr_t* ping_addr);

#if !PING_USE_SOCKETS
void ping_send_now(void);
void ping_send_now_nv(ip_addr_t *ping_target);
#endif /* !PING_USE_SOCKETS */

#endif /* LWIP_PING_H */
