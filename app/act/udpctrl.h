#ifndef __UDP_CTRL_H
#define __UDP_CTRL_H
//#include "sys.h"
#include "lwip_comm.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "actcom.h"

 
#define UDP_DEMO_TX_BUFSIZE		1024	//定义udp最大发送数据长度
#define UDP_TX_MAXLEN			100		//定义udp每次数据包发送的最大长度
#define UDP_DEV_NUM		2				//udp个数


typedef struct tyUDP_CTRL
{
	void*			hw_uart;
	int				tx_cnt;
	int				tx_len;
	int				btx;
	UI8*			tx_data;
	
	UI8*			Rx_data;
	int 			Rx_len;

	UI16 			tx_Set;		//根据数据包长度，计算出的包的发送次数
	UI16 			tx_Now;		//当前的数据包的已经发送的次数

	UI16			Udp_tx_cnt;	//Udp发送包个数
}UDP_CTRL, *PUDP_CTRL;

void UdpReset(UI8 uart);		//Udp数据初始化
int Udp_Data_Recieve(UI8 uart, UI8 peer_id, void* pbuf, int len);	//Udp数据接收函数
int Udp_Data_Tx(UI8 uart, UI8 peer_id, void* pbuf, int len);	//Udp数据发送函数
BOOL UdpBuf_IsTxEnd(UI8 uart, void* pbuf);	//Udp发送结束标记函数

void Udp_Init(UI32 br);
void am335x_udp_process(void);

extern UDP_CTRL udp_ctrl[UDP_DEV_NUM];

#endif

