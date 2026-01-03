#ifndef __UDP_DEMO_H
#define __UDP_DEMO_H
//#include "sys.h"
#include "lwip_comm.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "actcom.h"
   
 
#define UDP_DEMO_RX_BUFSIZE		2000	//定义udp最大接收数据长度 
#define UDP_DEMO_PORT			7777 //6666	//定义udp连接的端口 

extern u8 	udp_demo_recvbuf[UDP_DEMO_RX_BUFSIZE];
extern u8 	udp_demo_flag;
extern UI16 Udp_getData_lens;	


void udp_demo_recv(void *arg,struct udp_pcb *upcb,struct pbuf *p,struct ip_addr *addr,u16_t port);
void udp_demo_senddata(UI8* databuf,int len);
void udp_demo_connection_close(struct udp_pcb *upcb);
void am335x_udp_init(void);
void lwip_test_ui(u8 mode);

#endif

