#include "udp_demo.h" 
#include "malloc.h"
#include "stdio.h"
#include "string.h" 
#include "fsl_debug_console.h"
#include "actcom.h"
#include "machine.h"

//UDP接收数据缓冲区
u8 udp_demo_recvbuf[UDP_DEMO_RX_BUFSIZE];	//UDP接收数据缓冲区 ，2000

//UDP 测试全局状态标记变量
//bit7:没有用到
//bit6:0,没有收到数据;1,收到数据了.
//bit5:0,没有连接上;1,连接上了.
//bit4~0:保留
u8 	udp_demo_flag = 0;			//udp数据接收标记

UI16 Udp_getData_lens = 0;	//Udp接收到的数据包总长度  

struct udp_pcb *udppcb;  	//定义一个TCP服务器控制块
u8 flag_udppcb = 0;	

void lwip_test_ui(u8 mode)
{
	if(mode&1<<0)
	{
		mprintf("RT1061 DM9000");
		mprintf("Ethernet lwIP Test\n\r");
		mprintf("chenhao  ");
		mprintf("2019/01/17  \n\r"); 	
	}
	if(mode&1<<1)
	{
		mprintf("lwIP Init Successed \n\r");
		mprintf("Static IP:%d.%d.%d.%d\n\r",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);//′òó??2ì?IPμ??·
		// 这里删除了，再次检测speed的代码。防止出现没接网线，开机慢的情况
		mprintf("PORT: 6666\n\r");
		mprintf("every 500ms send data \n\r");
        mprintf("CPU: %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
        mprintf("AHB: %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
        mprintf("SEMC: %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
        mprintf("SYSPLL: %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
        mprintf("SYSPLLPFD0: %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
        mprintf("SYSPLLPFD1: %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
        mprintf("SYSPLLPFD2: %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
        mprintf("SYSPLLPFD3: %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));
	}
}


//设置远端IP地址
void udp_demo_set_remoteip(void)
{
	//前三个IP保持和DHCP得到的IP一致
	lwipdev.remoteip[0]=lwipdev.ip[0];
	lwipdev.remoteip[1]=lwipdev.ip[1];
	lwipdev.remoteip[2]=lwipdev.ip[2]; 
	lwipdev.remoteip[3]= 6;         // linux 335x  的ip地址 由于tcpip 客户端测试
	PRINTF("Remote IP:%d.%d.%d..%d\n\r",lwipdev.remoteip[0],lwipdev.remoteip[1],lwipdev.remoteip[2],lwipdev.remoteip[3]);//远端IP
} 


void am335x_udp_init(void)
{
 	err_t err;
	struct ip_addr rmtipaddr;  	//远端ip地址

    udp_demo_set_remoteip();//先选择IP	
	PRINTF("ch-udp Local IP:%d.%d.%d.%d\n\r",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);//服务器IP
	PRINTF("ch-udp Remote IP:%d.%d.%d.%d\n\r",lwipdev.remoteip[0],lwipdev.remoteip[1],lwipdev.remoteip[2],lwipdev.remoteip[3]);//远端IP
	PRINTF("ch-udp Remote Port:%d\n\r",UDP_DEMO_PORT);//客户端端口号
	PRINTF("ch-udp STATUS:Disconnected\n\r"); 
	udppcb=udp_new();
	if(udppcb)//创建成功
	{ 
		IP4_ADDR(&rmtipaddr,lwipdev.remoteip[0],lwipdev.remoteip[1],lwipdev.remoteip[2],lwipdev.remoteip[3]);
		err=udp_connect(udppcb,&rmtipaddr,UDP_DEMO_PORT);//UDP客户端连接到指定IP地址和端口号的服务器
		if(err==ERR_OK)
		{
			err=udp_bind(udppcb,IP_ADDR_ANY,UDP_DEMO_PORT);//绑定本地IP地址与端口号
			if(err==ERR_OK)	//绑定完成
			{
				udp_recv(udppcb,udp_demo_recv,NULL);//注册接收回调函数 
				PRINTF("ch-udp STATUS:Connected   \n\r");//标记连接上了(UDP是非可靠连接,这里仅仅表示本地UDP已经准备好)
				udp_demo_flag |= 1<<5;			//标记已经连接上
				PRINTF("ch-udp Receive Data: \n\r");//提示消息		
                flag_udppcb = 1;	
			}else flag_udppcb = 0;
		}else flag_udppcb = 0;		
	}else flag_udppcb = 0;
}


//UDP回调函数
void udp_demo_recv(void *arg,struct udp_pcb *upcb,struct pbuf *p,struct ip_addr *addr,u16_t port)
{
	u32 data_len = 0;
	struct pbuf *q;
	if(p!=NULL)	//接收到不为空的数据时
	{
		memset(udp_demo_recvbuf,0,UDP_DEMO_RX_BUFSIZE);  //数据接收缓冲区清零
		for(q=p;q!=NULL;q=q->next)  //遍历完整个pbuf链表
		{
			//判断要拷贝到UDP_DEMO_RX_BUFSIZE中的数据是否大于UDP_DEMO_RX_BUFSIZE的剩余空间，如果大于
			//的话就只拷贝UDP_DEMO_RX_BUFSIZE中剩余长度的数据，否则的话就拷贝所有的数据
			if(q->len > (UDP_DEMO_RX_BUFSIZE-data_len)) 
				memcpy(udp_demo_recvbuf+data_len,q->payload,(UDP_DEMO_RX_BUFSIZE-data_len));//拷贝数据
			else 
				memcpy(udp_demo_recvbuf+data_len,q->payload,q->len);
			data_len += q->len;  	
			if(data_len > UDP_DEMO_RX_BUFSIZE) 
				break; //超出TCP客户端接收数组,跳出	
		}
		Udp_getData_lens = data_len;	//记录当前这一包数据的总长度
		upcb->remote_ip=*addr; 				//记录远程主机的IP地址
		upcb->remote_port=port;  			//记录远程主机的端口号
		lwipdev.remoteip[0]=upcb->remote_ip.addr&0xff; 		//IADDR4
		lwipdev.remoteip[1]=(upcb->remote_ip.addr>>8)&0xff; //IADDR3
		lwipdev.remoteip[2]=(upcb->remote_ip.addr>>16)&0xff;//IADDR2
		lwipdev.remoteip[3]=(upcb->remote_ip.addr>>24)&0xff;//IADDR1 
		udp_demo_flag|=1<<6;	//标记接收到数据了
		pbuf_free(p);//释放内存
	}
	else
	{
		udp_disconnect(upcb); 
        PRINTF("udp_recv Connect break！\n\r");  
		udp_demo_flag &= ~(1<<5);	//标记连接断开
	} 
} 

//UDP服务器发送数据
void udp_demo_senddata(UI8* databuf,int len)
{
	struct pbuf *ptr;
	ptr=pbuf_alloc(PBUF_TRANSPORT,len,PBUF_POOL); //申请内存
	if(ptr)
	{
		pbuf_take(ptr,databuf,len);
		udp_send(udppcb,ptr);	//udp发送数据 
		pbuf_free(ptr);//释放内存
	} 
} 

//关闭UDP连接
void udp_demo_connection_close(struct udp_pcb *upcb)
{
	udp_disconnect(upcb); 
	udp_remove(upcb);			//断开UDP连接 
	udp_demo_flag &= ~(1<<5);	//标记连接断开
    PRINTF(" udp_close Connect break！\n\r");  
}
