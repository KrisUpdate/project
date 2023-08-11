/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef ETHERNET_CONFIG_H_
#define ETHERNET_CONFIG_H_

#include "udp.h"

#define IPERF_SERVER_PORT   5001
#define TCP_CLIENT_PORT  46222

extern ip_addr_t cmd_src_ip;
extern u16_t cmd_src_port;

extern struct udp_pcb *tegra_console_server_ptr;

#endif /* ETHERNET_CONFIG_H_ */
