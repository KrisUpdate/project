/*
 * qci_udp_client.c
 *
 *  Created on: 2019年2月13日
 *      Author: 95111301
 */
#include "lwip/debug.h"
#include "lwip/udp.h"
#include "qci_udp_packet.h"
#include "Cpu_Main.h"

void qci_udp_client_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);

uint8 data[600]={0x00};
uint32 message_count = 0;
uint32 qci_udp_client_count = 0;
struct udp_pcb *g_qci_udp_client_pcb;
ip_addr_t ServerIpAddr;
extern App_Cpu g_AppCpu0; /**< \brief CPU 0 global data */

void qci_udp_client_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{

	/* send udp data */
	LWIP_DEBUGF( QCI_UDP_SERVICE_DEBUG, ("%s\r\n",__func__));
	udp_send(upcb, p);
	/* free pbuf */
	pbuf_free(p);

}
void qci_udp_send(struct udp_pcb *pcb, struct qci_udp_packet packet)
{
	struct pbuf *p;
	int idx = 0;

	memcpy(data,(uint8 *)&packet,sizeof(packet));
#if 0
	LWIP_DEBUGF( QCI_UDP_SERVICE_DEBUG, ("dump qci udp out packet\r\n"));
	for(idx = 0 ;idx <sizeof(packet);idx++)
	{
		if(idx % 15 == 0)
			LWIP_DEBUGF( QCI_UDP_SERVICE_DEBUG, ("\r\n"));
		LWIP_DEBUGF( QCI_UDP_SERVICE_DEBUG, ("0x%02X ",data[idx]));
	}
	LWIP_DEBUGF( QCI_UDP_SERVICE_DEBUG, ("\r\n"));
#endif
	/* allocate pbuf from ram*/
	p = pbuf_alloc(PBUF_TRANSPORT,sizeof(packet), PBUF_RAM);
	if (p != NULL)
	{
		/* copy data to pbuf */
		pbuf_take(p, data, sizeof(packet));
#if 0
		LWIP_DEBUGF( QCI_UDP_SERVICE_DEBUG, ("dump qci udp pbuf packet\r\n"));
		memcpy(data,p->payload,p->len);
		for(idx = 0 ;idx <p->len;idx++)
		{
			if(idx % 15  == 0 )
				LWIP_DEBUGF( QCI_UDP_SERVICE_DEBUG, ("\r\n"));
			LWIP_DEBUGF( QCI_UDP_SERVICE_DEBUG, ("0x%02X ",data[idx]));
		}
		LWIP_DEBUGF( QCI_UDP_SERVICE_DEBUG, ("\r\n"));
#endif
		/* send udp data */
		udp_send(pcb, p); //發送數據

		/* free pbuf */
		pbuf_free(p);
	}
}

void qci_udp_multi_send(struct udp_pcb *pcb, uint8 *udp_data, int nbytes)
{
	struct pbuf *p;
	int idx = 0;
	err_t err_code;

	//memcpy(data, udp_data, nbytes);

	/* allocate pbuf from ram*/
	p = pbuf_alloc(PBUF_TRANSPORT, nbytes, PBUF_RAM);
	if (p != NULL)
	{
		/* copy data to pbuf */
		pbuf_take(p, udp_data, nbytes);

		/* send udp data */
		err_code = udp_send(pcb, p); //發送數據

		/* free pbuf */
		pbuf_free(p);
	}
}

#if 0
void qci_udp_example_send()
{
	int idx = 0;
	struct qci_udp_packet packet;
	err_t err;

	if (g_qci_udp_client_pcb!=NULL)
	{
		qci_udp_client_count++;
		LWIP_DEBUGF( QCI_UDP_SERVICE_DEBUG, ("%s %d\r\n",__func__,qci_udp_client_count));

		/* configure destination IP address and port */
		err = udp_connect(g_qci_udp_client_pcb, &ServerIpAddr, QCI_UDP_SERVER_PORT);

		if (err == ERR_OK)
		{
		/* Set a receive callback for the upcb */
			udp_recv(g_qci_udp_client_pcb, qci_udp_client_receive_callback, NULL);
		}
		//header
		packet.header.packet_direct = toVIU;
		if(qci_udp_client_count % 2 ==0)
		{
			packet.header.bus_type = typeCAN;
			packet.canid = 0x7ff;

		}
		else
		{
			packet.header.bus_type = typeLIN;
			packet.canid = 0x11;
		}
		packet.header.target_viu = 0x0;
		packet.header.target_mcu = TARGET_GATEWAY;

		packet.info_len = 80;
		packet.info_code = 0x1;
		packet.bus_id = 0x2; // bus 2
		packet.candlc = 0x8;
		packet.rs = 0x99;

		//generate random payload
		srand(g_AppCpu0.STM1TickCount_1ms);
		for(idx = 0 ;idx < QCI_UDP_PAYLOAD_SIZE; idx++)
			packet.payload[idx] = rand()%64;

		qci_udp_send(g_qci_udp_client_pcb,packet);
		return;
#if 0
		struct pbuf *p;
		int idx = 0;
		LWIP_DEBUGF( QCI_UDP_SERVICE_DEBUG, ("%s \r\n",__func__));

		memcpy(data,(uint8 *)&packet,sizeof(packet));
	#if 1
		LWIP_DEBUGF( QCI_UDP_SERVICE_DEBUG, ("dump qci udp out packet\r\n"));
		for(idx = 0 ;idx <sizeof(packet);idx++)
		{
			if(idx % 15 == 0)
				LWIP_DEBUGF( QCI_UDP_SERVICE_DEBUG, ("\r\n"));
			LWIP_DEBUGF( QCI_UDP_SERVICE_DEBUG, ("0x%02X ",data[idx]));
		}
		LWIP_DEBUGF( QCI_UDP_SERVICE_DEBUG, ("\r\n"));
	#endif
		/* allocate pbuf from ram*/
		p = pbuf_alloc(PBUF_TRANSPORT,sizeof(packet), PBUF_RAM);
		if (p != NULL)
		{
			/* copy data to pbuf */
			pbuf_take(p, data, sizeof(packet));
	#if 0
			LWIP_DEBUGF( QCI_UDP_SERVICE_DEBUG, ("dump qci udp pbuf packet\r\n"));
			memcpy(data,p->payload,p->len);
			for(idx = 0 ;idx <p->len;idx++)
			{
				if(idx % 15  == 0 )
					LWIP_DEBUGF( QCI_UDP_SERVICE_DEBUG, ("\r\n"));
				LWIP_DEBUGF( QCI_UDP_SERVICE_DEBUG, ("0x%02X ",data[idx]));
			}
			LWIP_DEBUGF( QCI_UDP_SERVICE_DEBUG, ("\r\n"));
	#endif
			/* send udp data */
			udp_send(g_qci_udp_client_pcb, p); //發送數據

			/* free pbuf */

			pbuf_free(p);
		}
#endif
	}

}
void qci_udp_client_init(void)
{
	err_t err;
	/*assign destination IP address */
#if SECURITY_GATEWAY
    IP4_ADDR(&ServerIpAddr, 192,168,1,100);
#else
    IP4_ADDR(&ServerIpAddr, 192,168,1,202);
#endif

	LWIP_DEBUGF( QCI_UDP_SERVICE_DEBUG, ("%s\r\n",__func__));

	/*Create a new UDP control block */
	g_qci_udp_client_pcb = udp_new();

}
#endif

