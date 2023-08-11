#include "lwip/opt.h"
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/udp.h"
#include "qci_udp_server.h"
#include "qci_udp_packet.h"

#if LWIP_UDP


static struct udp_pcb *qci_udp_server_pcb;
uint32 qci_udp_server_count = 0;
uint32 udp_count = 0;
uint32 mess_count = 0;

static void
qci_udp_server_recv(void *arg, struct udp_pcb *upcb, struct pbuf *p,
                 const ip_addr_t *addr, u16_t port)
{
  LWIP_UNUSED_ARG(arg);
  qci_udp_server_count++;
  struct qci_udp_packet *packet;

  LWIP_DEBUGF(QCI_UDP_SERVICE_DEBUG, ("qci_udp_server_recv %d\r\n", qci_udp_server_count));
  if (p != NULL) {
	uint8 *udp_ptr;
	int udp_idx=0;

	LWIP_DEBUGF(QCI_UDP_SERVICE_DEBUG, ("received udp length is %d\r\n", p->tot_len));
	udp_ptr = (uint8 *)p->payload;
	udp_count++;
	while (udp_idx < p->tot_len) {
		mess_count ++;
		packet = (struct qci_udp_packet *)(udp_ptr + udp_idx);
		if (packet->header.packet_direct == toVIU) {
			parse_qci_udp_packet(addr, packet);
		} else {
			//printf("parse Error!\r\n");
			LWIP_DEBUGF(QCI_UDP_SERVICE_DEBUG, ("to toHOST, discard frame\r\n"));
		}
		udp_idx = udp_idx + QCI_UDP_TOTAL_HEADER_LEN + packet->candlc;
		//printf("message_length: %d\r\n",packet->candlc);
	}

	/* free the pbuf */
	//pbuf_free(p);
  }
}

void qci_udp_server_init(void)
{
	LWIP_DEBUGF(QCI_UDP_SERVICE_DEBUG, ("qci_udp_server_init\r\n"));
	qci_udp_server_pcb = udp_new_ip_type(IPADDR_TYPE_ANY);

	if (qci_udp_server_pcb != NULL) {
		err_t err;

		err = udp_bind(qci_udp_server_pcb, IP_ANY_TYPE, QCI_UDP_SERVER_PORT);
		if (err == ERR_OK) {
			udp_recv(qci_udp_server_pcb, qci_udp_server_recv, NULL);
		} else {
			/* abort? output diagnostic? */
		}
	} else {
			/* abort? output diagnostic? */
	}
}

#endif /* LWIP_UDP */
