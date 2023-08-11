/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#ifndef __UDPAPP_H_
#define __UDPAPP_H_

typedef void (*rec_ptr)(uint8* data, uint16 len);

void udp_app_init(uint32 ip32);
void udp_app_send(uint8* buf, uint8 len);
void udp_app_register_rec_cb(rec_ptr receive_cb);

#endif /* __UDPAPP_H_ */
