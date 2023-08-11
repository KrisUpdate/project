/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#include "tegra_console.h"
#include "opt.h"
#include "stats.h"
#include "udp.h"
#include "udpapp.h"
#include "AsclinShellInterface.h"
#include <string.h>


#define TEGRA_CONSOLE_DEBUG_ENABLE 0

#if TEGRA_CONSOLE_DEBUG_ENABLE == 1
#define TEGRA_CONSOLE_DEBUG_PRINTF(x) AsclinShellInterface_printf x
#else
#define TEGRA_CONSOLE_DEBUG_PRINTF(x)
#endif

#define TEGRA_CONSOLE_ALWAYS_PRINTF(x) AsclinShellInterface_printf x
#define TEGRA_CONSOLE_ALWAYS_PRINTF_UART(x) Ifx_Console_print x

ip_addr_t cmd_src_ip;
u16_t cmd_src_port = 0;

struct udp_pcb *tegra_console_server_ptr = NULL;  // DPipe print take this to send data, so it cannot be static
static boolean print_udp_command_enable = TRUE;

void print_udp_cmd_ctrl(boolean enable)
{
	print_udp_command_enable = enable;
}

static void tegra_console_server_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p,
	    ip_addr_t *addr, u16_t port)
{
	LWIP_UNUSED_ARG(arg);
	LWIP_UNUSED_ARG(pcb);
	char empty_arg[] = " ";
	char *firstlf;
	TEGRA_CONSOLE_DEBUG_PRINTF(("Server Rec: ip address is 0x%x, port %d\n\r", *addr, port));
#if PROJECT == PROJECT_V31C
	if((port != TEGRA_A_CLIENT_PORT) &&
	   (port != TEGRA_A_CLIENT_RETRY_PORT))
#else
	if((port != TEGRA_A_CLIENT_PORT) &&
	   (port != TEGRA_B_CLIENT_PORT) &&
	   (port != TEGRA_A_CLIENT_RETRY_PORT) &&
	   (port != TEGRA_B_CLIENT_RETRY_PORT))
#endif
	{ /* we don't care message from other ports */
		return;
	}
	if(p->payload != NULL)
	{
		firstlf = strchr((char*)p->payload,'\n');
		*firstlf = '\0';

		if(print_udp_command_enable)
		{
			TEGRA_CONSOLE_ALWAYS_PRINTF_UART(("UDP Command>%s\n\r", p->payload));
		}


		char* cmd = NULL;
		char* arg = NULL;
		if(strchr(p->payload, ' ') != NULL)
		{ /* we have parameter */
			cmd = strtok((char*)p->payload, " ");
			arg = strtok(NULL, "\n");
		}
		else
		{
			cmd = strtok((char*)p->payload, "\n");
		}
		if(cmd != NULL)
		{
			if(arg == NULL)
			{
				arg = empty_arg;
			}
			cmd_src_ip = *addr; // magic global for DPipe print and send data with udp
			cmd_src_port = port;  // magic global for DPipe print and send data with udp
			AsclinShellInterface_execute_cmd(cmd, arg);
			//tegra_console_server_send(pcb, addr, port, "ACK", 4);
		}
	}
}

void tegra_console_server_send(struct udp_pcb * udp_ptr,
		ip_addr_t* ip_ptr, uint16 port, uint8* buf, uint16 len)
{
	TEGRA_CONSOLE_DEBUG_PRINTF(("%s\n\r", __func__));
	struct pbuf *udp_buf = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);
	if(udp_buf == NULL)
	{
		TEGRA_CONSOLE_ALWAYS_PRINTF(("Can not alloc the buffer\n\r"));
	}
	else
	{
		udp_buf->payload = buf;
		udp_sendto(udp_ptr, udp_buf, ip_ptr, port);
		pbuf_free(udp_buf);
	}
}

void tegra_console_init_udp(struct udp_pcb **udp_pcb_ptr, u16_t port, udp_recv_fn recv)
{
	TEGRA_CONSOLE_DEBUG_PRINTF(("%s\n\r", __func__));

	*udp_pcb_ptr = udp_new();
	if(*udp_pcb_ptr != NULL)
	{
		err_t err;

		err = udp_bind(*udp_pcb_ptr, IP_ADDR_ANY, port);
		udp_recv(*udp_pcb_ptr, recv, NULL);
		if (err == ERR_OK)
		{
			TEGRA_CONSOLE_DEBUG_PRINTF(("Successfully listen to UDP port: %d for commands\n\r", port));
		}
		else
		{
			TEGRA_CONSOLE_ALWAYS_PRINTF(("fail to bind\n\r"));
		}
	}
	else
	{
		TEGRA_CONSOLE_ALWAYS_PRINTF(("UDP_APP: fail to get the udb object\n\r"));
	}
}

void tegra_console_init(void)
{
	tegra_console_init_udp(&tegra_console_server_ptr, TEGRA_CONSOLE_SERVER_PORT, tegra_console_server_recv);
}
