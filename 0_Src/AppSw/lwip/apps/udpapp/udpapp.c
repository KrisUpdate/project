/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#include "configuration.h"
#include "udpapp_config.h"
#include "opt.h"
#include "stats.h"
#include "udp.h"
#include "udpapp.h"


/*****************************************/
/***********  Feature   ******************/
/*****************************************/
/* comment it to use Aurix IP to bind the UPD */
#define BOUND_ANY_IP_ENABLE

#define UDP_PORT 5000

/*****************************************/
/********  Debug control   ***************/
/*****************************************/
#if PROJECT == PROJECT_PAGASUS2 || PROJECT == PROJECT_V31C || PROJECT == PROJECT_V3NA
#include <internal_fw_debug.h>
#define UDP_APP_DEBUG_ENABLE 0
#endif

#if UDP_APP_DEBUG_ENABLE == 1
#define UDP_APP_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
#else
#define UDP_APP_DEBUG_PRINTF(x)
#endif

#define LOOP_BACK_TEST_ENABLE   0

static const ip_addr_t ip_addr_mine = { IPADDR_MINE };

#define IP_ADDR_MINE        ((ip_addr_t *)&ip_addr_mine)

static rec_ptr receive_cb = NULL;
static struct udp_pcb *udp_object_ptr;
static ip_addr_t dest_ip;
static uint16 dest_port;
static boolean is_dest_known = FALSE;

static void udp_app_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p,
	    ip_addr_t *addr, u16_t port)
{
	LWIP_UNUSED_ARG(arg);
	UDP_APP_DEBUG_PRINTF(("UDP_APP: receive packet from\n\r"));
	UDP_APP_DEBUG_PRINTF(("UDP_APP: ip address is 0x%x, port %d\n\r", *addr, port));
	memcpy(&dest_ip, addr, sizeof(ip_addr_t));
	dest_port = port;
	is_dest_known = TRUE;
	receive_cb(p->payload, p->len);
}

void udp_app_send(uint8* buf, uint8 len)
{
	UDP_APP_DEBUG_PRINTF(("UDP_APP: udp_send start\n\r"));
	struct pbuf *udp_buf = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);
	if(udp_buf == NULL)
	{
		UDP_APP_DEBUG_PRINTF(("UDP_APP: Can not alloc the buffer\n\r"));
	}
	else
	{

		udp_buf->payload = buf;
		if(is_dest_known)
		{
			udp_sendto(udp_object_ptr, udp_buf, &dest_ip, dest_port);
			is_dest_known = FALSE;
		}
		else
		{
			UDP_APP_DEBUG_PRINTF(("UDP_APP: don't know the target IP to send\n\r"));
		}
		pbuf_free(udp_buf);
	}

}

void udp_app_register_rec_cb(rec_ptr rec_cb)
{
	if(rec_cb != NULL)
	{
		receive_cb = rec_cb;
	}
}

void udp_app_init(uint32 ip32)
{
	UDP_APP_DEBUG_PRINTF(("UDP_APP: udp_init start\n\r"));
	udp_object_ptr = udp_new();
	if(udp_object_ptr != NULL)
	{
		err_t err;
#ifdef BOUND_ANY_IP_ENABLE
		err = udp_bind(udp_object_ptr, IP_ADDR_ANY, UDP_PORT);
#else
		ip_addr_t ip_addr;
		ip4_addr_set_u32(&ip_addr, ip32);
		err = udp_bind(udp_object_ptr, &ip_addr, UDP_PORT);
#endif
		udp_recv(udp_object_ptr, udp_app_recv, NULL);
		if (err == ERR_OK)
		{
			UDP_APP_DEBUG_PRINTF(("UDP_APP: Successfully listen to UDP port: 5000\n\r"));
		}
		else
		{
			UDP_APP_DEBUG_PRINTF(("UDP_APP: fail to bind\n\r"));
		}
	}
	else
	{
		UDP_APP_DEBUG_PRINTF(("UDP_APP: fail to get the udb object\n\r"));
	}

}
