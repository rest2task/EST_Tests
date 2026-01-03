#include "udpctrl.h" 
#include "stdio.h"
#include "string.h" 
#include "actcom.h"
#include "machine.h"
#include "hwmid.h"

UDP_CTRL udp_ctrl[UDP_DEV_NUM];  

UI8 	udp_demo_sendbuf[UDP_DEMO_TX_BUFSIZE]={0};	//udp发送数据缓存
UI16	Udp_send_flag = 0;	//Udp发送数据标记
UI16	Udp_get_flag = 0;	//Udp接收数据标记


void UdpReset(UI8 uart)
{
	PUDP_CTRL pudp;

	if(uart < UDP_DEV_NUM)
		pudp = udp_ctrl + uart;
	else
		return ;

	pudp->tx_len = 0;
	pudp->tx_data = NULL;
	pudp->tx_cnt = pudp->tx_len + 1;
	pudp->btx = FALSE;

	pudp->Rx_data = NULL;
	pudp->Rx_len = 0;

	pudp->tx_Set = 0;
	pudp->tx_Now = 0;
}

#ifdef DM9000_ENABLE
int Udp_Data_Recieve(UI8 uart, UI8 peer_id, void* pbuf, int len)
{
	int tmp = 0;
	
	if(pbuf == NULL || len < 0)
		return 0;
		
	if(m_get_udpdata_flag())	//是否收到数据
    {    	
    	m_clear_udpdata_flag();	//标记数据已经被处理了

		if(Udp_uart_flag != 1)		//当前状态为非网络通讯时，不做数据搬运，直接跳走，防止出现数据残留
			return 0;
		
    	if(len > m_get_udplen())	//接收缓存最大1024，当剩余长度大于当前的数据长度时，将数据放入接收缓存
			tmp  = m_get_udplen();
		else
			tmp = len;
		
		memcpy(pbuf, m_get_udpdata_adr(), tmp); 
		
		Udp_get_flag = TRUE;
		
		return tmp;
    } 
	else
		return 0;
}
#endif

BOOL UdpBuf_IsTxEnd(UI8 uart, void* pbuf)	//Udp数据发送完毕返回真，否则返回0
{
	PUDP_CTRL pudp;

	if(uart < UDP_DEV_NUM)
		pudp = udp_ctrl + uart;
	else
		return TRUE;
		
	return (pbuf != (void*)pudp->tx_data || pudp->tx_cnt > pudp->tx_len);
}


void UdpTxEnd()		//当Udp数据发送完毕时，将发送次数加大到大于发送长度，并清除发送标记
{
	PUDP_CTRL pudp = udp_ctrl;
	/*加1，使pctrl->tx_cnt > pctrl->tx_len，表示发送结束*/
	pudp->tx_cnt = pudp->tx_len +1;
	pudp->btx = FALSE;
	
	pudp->tx_Set = 0;
	pudp->tx_Now = 0;
}


int Udp_Data_Tx(UI8 uart, UI8 peer_id, void* pbuf, int len)
{
	PUDP_CTRL  pudp;
	
	if(pbuf == NULL || len == 0)
		return 0;

	if(uart < UDP_DEV_NUM)
		pudp = udp_ctrl + uart;
	else
		return 0;
	
	if(pudp == NULL)
		return 0;

	if(len > UDP_DEMO_TX_BUFSIZE)
		return 0;

	if(pudp->tx_cnt <= pudp->tx_len)	//当发送次数小于等于len 时，代表还没发送完，不要再进入后续的程序代码运行
		return 0;
	
	Udp_send_flag = TRUE;

	pudp->tx_data = pbuf;
	pudp->tx_cnt = 0;
	pudp->tx_len = len;

	return 1;
}

#ifdef DM9000_ENABLE
void am335x_udp_process(void)
{	
	PUDP_CTRL  pudp = udp_ctrl;

	m_lwip_pkt_handle(); 
    if(Udp_get_flag == TRUE)	//是否收到数据，当前循环有数据在接收时，则当前循环不进行数据的发送
    {   	
        Udp_get_flag = 0; 
        m_set_udplen(0);
	} 
	else
	{
		if(Udp_uart_flag != 1)	//当前发送不是udp时，不输出数据，防止在开机的时候，udp与232切换过程中，由于会清空缓存，导致此处udp发送时，实际数据没有，从而输出一串全部为0的数据包的问题
			Udp_send_flag = false;
		
		if(m_udp_request_finished())
		{
			if(Udp_send_flag == TRUE)
		    {
		    	pudp->Udp_tx_cnt++;

		    	if(pudp->tx_len <= UDP_TX_MAXLEN)	//当数据包小于UDP_TX_MAXLEN，不需要进行拆包，直接发送即可
		    	{
			        Udp_send_flag = false;
					memcpy(udp_demo_sendbuf,pudp->tx_data,pudp->tx_len);
					m_udp_senddata(udp_demo_sendbuf,pudp->tx_len);	
					UdpTxEnd();
		    	}
				else
				{
					if(pudp->tx_len%UDP_TX_MAXLEN != 0)	//当有余数时，发送包数需要加1 ，用于发送剩余的个数。而刚好可以整除时，包数不需要加1
						pudp->tx_Set = (pudp->tx_len/UDP_TX_MAXLEN) + 1;	//计算当前包拆解后，实际需要发送的次数，有余数时，代表还有一个不满UDP_TX_MAXLEN个数据的包需要发送
					else
						pudp->tx_Set = (pudp->tx_len/UDP_TX_MAXLEN);
					if(pudp->tx_Now < pudp->tx_Set)
					{
						if(pudp->tx_Now + 1 == pudp->tx_Set)	//发送拆包后的最后一包数据
						{
							Udp_send_flag = false;
							memcpy(udp_demo_sendbuf,pudp->tx_data + (pudp->tx_Now * UDP_TX_MAXLEN),pudp->tx_len - pudp->tx_cnt);
							m_udp_senddata(udp_demo_sendbuf,pudp->tx_len - pudp->tx_cnt);
							UdpTxEnd();
						}
						else			//发送拆包后的，前面的数据包，每包数据长度都一样，都是UDP_TX_MAXLEN，
						{
							memcpy(udp_demo_sendbuf,pudp->tx_data + (pudp->tx_Now * UDP_TX_MAXLEN),UDP_TX_MAXLEN);
							m_udp_senddata(udp_demo_sendbuf,UDP_TX_MAXLEN);
							pudp->tx_Now++;
							pudp->tx_cnt = pudp->tx_cnt + UDP_TX_MAXLEN;
						}
					}
					else
					{
						Udp_send_flag = false;
						UdpTxEnd();
					}
				}
		    }
		}
	}
	
    m_lwip_periodic_handle();		
}
#endif

void Udp_Init(UI32 br)
{
	int i;
	
	memset(udp_demo_sendbuf,0,UDP_DEMO_TX_BUFSIZE);
	memset(&udp_ctrl, 0, sizeof(udp_ctrl));
	
	/*加1，tx_cnt > tx_len，表示发送结束*/
	for(i = 0; i < UDP_DEV_NUM; i ++)
		udp_ctrl[i].tx_cnt = udp_ctrl[i].tx_len + 1;

	udp_ctrl[0].hw_uart = NULL;
}























