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

#define UDP_OTA_PORT 5001

/*****************************************/
/********  Debug control   ***************/
/*****************************************/
#if PROJECT == PROJECT_PAGASUS2 || PROJECT == PROJECT_V31C
#include <internal_fw_debug.h>
#define UDP_APP_DEBUG_ENABLE 1
#endif

//#if UDP_APP_DEBUG_ENABLE == 1
#define UDP_APP_DEBUG_PRINTF(x)  DEBUG_PRINTF(x)
//#else
//#define UDP_APP_DEBUG_PRINTF(x)
//#endif

#define LOOP_BACK_TEST_ENABLE   0

static const ip_addr_t ip_addr_mine = { IPADDR_MINE };

#define IP_ADDR_MINE        ((ip_addr_t *)&ip_addr_mine)

static rec_ptr receive_ota_cb = NULL;
static struct udp_pcb *udp_object_ota_ptr;
static ip_addr_t dest_ip;
static uint16 dest_port;
static boolean is_dest_known = FALSE;

static void udp_app_recv_ota(void *arg, struct udp_pcb *pcb, struct pbuf *p,
	    ip_addr_t *addr, u16_t port)
{
	LWIP_UNUSED_ARG(arg);
	UDP_APP_DEBUG_PRINTF(("UDP_APP_OTA: receive packet from\r\n"));
	UDP_APP_DEBUG_PRINTF(("UDP_APP_OTA: ip address is 0x%x, port %d\r\n", *addr, port));
	memcpy(&dest_ip, addr, sizeof(ip_addr_t));
	dest_port = port;
	is_dest_known = TRUE;
	receive_ota_cb(p->payload, p->len);
}
#if 1
void ota_udp_app_send(uint8* buf, uint8 len)
{
	UDP_APP_DEBUG_PRINTF(("UDP_APP_OTA: udp_send start\r\n"));
	struct pbuf *udp_buf = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);
	if(udp_buf == NULL)
	{
		DEBUG_PRINTF(("UDP_APP_OTA: Can not alloc the buffer\r\n"));
	}
	else
	{

		udp_buf->payload = buf;
		if(is_dest_known)
		{
			udp_sendto(udp_object_ota_ptr, udp_buf, &dest_ip, dest_port);
			is_dest_known = FALSE;
		}
		else
		{
			DEBUG_PRINTF(("UDP_APP_OTA: don't know the target IP to send\r\n"));
		}
		pbuf_free(udp_buf);
	}

}

void udp_app_register_rec_ota_cb(rec_ptr rec_cb)
{
	if(rec_cb != NULL)
	{
		receive_ota_cb = rec_cb;
	}
}
#endif
void udp_app_init_ota(uint32 ip32)
{
	UDP_APP_DEBUG_PRINTF(("UDP_APP_OTA: udp_init start\r\n"));
	udp_object_ota_ptr = udp_new();
	if(udp_object_ota_ptr != NULL)
	{
		err_t err;
#ifdef BOUND_ANY_IP_ENABLE
		err = udp_bind(udp_object_ota_ptr, IP_ADDR_ANY, UDP_OTA_PORT);
#else
		ip_addr_t ip_addr;
		ip4_addr_set_u32(&ip_addr, ip32);
		err = udp_bind(udp_object_ota_ptr, &ip_addr, UDP_OTA_PORT);
#endif
		udp_recv(udp_object_ota_ptr, udp_app_recv_ota, NULL);
		if (err == ERR_OK)
		{
			UDP_APP_DEBUG_PRINTF(("UDP_APP_OTA: Successfully listen to UDP port: %d\r\n",UDP_OTA_PORT));
		}
		else
		{
			UDP_APP_DEBUG_PRINTF(("UDP_APP_OTA: fail to bind\r\n"));
		}
	}
	else
	{
		UDP_APP_DEBUG_PRINTF(("UDP_APP_OTA: fail to get the udb object\r\n"));
	}

}
