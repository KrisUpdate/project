/*
 * qci_udp_client.h
 *
 *  Created on: 2019¦~2¤ë13¤é
 *      Author: 95111301
 */

#ifndef _QCI_UDP_CLIENT_H_
#define _QCI_UDP_CLIENT_H_

//void qci_udp_example_send();

IFX_EXTERN void qci_udp_send(struct udp_pcb *pcb, struct qci_udp_packet packet);
IFX_EXTERN void qci_udp_multi_send(struct udp_pcb *pcb, uint8 *udp_data, int nbytes);

#endif /* _QCI_UDP_CLIENT_H_ */
