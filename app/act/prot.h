#ifndef _PROT_H_
#define _PROT_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"
#include "uart.h"


#define PROT_UART_DEV		0			/*串口0 -4*/
#define PROT_UART2_DEV		1			/*串口0 -4*/
#define PROT_CAN1_DEV		4			/*CAN1 设备*/
#define PROT_CAN2_DEV		5			/*CAN2 设备*/

#define PROT_PEER_PANEL		0x51		/*面板*/
#define PROT_PEER_PC		0x71		/*PC机*/
#define PROT_PEER_IINJ		0x51		/*智能注射卡*/
#define PROT_PEER_TNET		0x51		/*物联网2G模块*/

#define BUF_TYPE_NUM		5
#define BUF_64_NUM			8
#define BUF_128_NUM		4
#define BUF_256_NUM		4
#define BUF_512_NUM		4
#define BUF_1024_NUM		1

#define PROT_MAX_PKT_LEN	1024
#define PROT_TX_REQ_NUM		0x20	//原0x10 ->		0x20
#define PROT_TX_RETRY		3
#define PROT_ACK_MIN_WAITE	150	/*ms*/
#define PROT_MAX_PKT_TX_MS	(((PROT_MAX_PKT_LEN*10*1000)/(UART_BAUDRATE) + 1))
#define PROT_ACK_WAITE	(PROT_MAX_PKT_TX_MS + PROT_ACK_MIN_WAITE)	/*ms*/
#define PROT_TX_BUF_LEN	PROT_MAX_PKT_LEN
#define PROT_RX_BUF_LEN	PROT_MAX_PKT_LEN
#define PROT_FAIL_TM			5000			/**/
#define PROT_RX_PKT_TMOUT		1000			/*ms，接收一个包的前一段与后一段之间的最大时间间隔，要考虑串口驱动缓存大小和波特率的关系*/

#define PROT_UART_PANEL		0
#define PROT_MON_PC			1					/*PC示波器监控软件端口*/
#define PROT_CAN2_IINJ		2
#define PROT_CAN2_TNET		2					/*物联网2G模块*/


#define PROT_PEER_NUM		2

extern UI16	Udp_uart_flag;

typedef struct tyBUF_LNK* PBUF_LNK;

//协议缓存列表
typedef struct tyBUF_LNK
{
	UI16		size;					//缓存大小
	UI16		bused;					//缓存是否已使用 
	PBUF_LNK	pnext;					//指向列表的下一个 
}BUF_LNK, *PBUF_LNK;

//缓存列表头
typedef struct tyBUF_HD
{
	PBUF_LNK	plnk;					//指向列表的第一个缓存
	UI16		size;					//本列表缓存的大小
}BUF_HD, *PBUF_HD;

#define BUF_REV_SIZE	sizeof(BUF_LNK)

/*定义协议通信设备*/
typedef void (*DEV_RESET)(UI8 dev_id);
typedef int (*DEV_RXTX)(UI8 dev_id, UI8 peer_id, void* buf, int len);
typedef BOOL (*DEV_TXISEND)(UI8 dev_id, void* buf);

#define DEV_NEEDREAD	0x01
typedef struct tyPROT_DEV
{
	UI8			dev_id;
	UI8			attr;
	DEV_RESET	DevReset;
	DEV_RXTX	DevRead;
	DEV_RXTX	DevWrite;
	DEV_TXISEND DevTxIsEnd;
}PROT_DEV, *PPROT_DEV;

//通信协议数据包接收状态
enum enPROT_RX_ST
{
	PROT_RX_START = 0,					//开始接收
	PROT_RX_HD = 1,						//已收到包头
	PROT_RX_PKT = 2						//已收到一个完整的包						
};

struct tyPROT_TX_REQ;
//发送请求列表
typedef struct tyPROT_TX_REQ
{
	D_LINK			lnk;				//列表下一个元素
	UI8				bused;				//是否已使用 
	UI8				bneed_ack;			//本发送数据包是否需要对方 应答
	UI8				retry;				//已经发送重试次数
	UI16			len;				//数据包长度
	UI32			last_tx_ms;			//上一次发送时间
	void*			pbuf;				//指向数据包
}PROT_TX_REQ, *PPROT_TX_REQ;

//协议接收
typedef struct tyPROT_RX
{
	UI32		rx_bytes;						//接收到的字节数
	UI32		pkt_bytes;						//接到包的字节累计数
	UI32		last_rx_ms;						//上一次接收到数据的时间
	UI16		len;							//接收缓存数据长度
	UI16		hd_ind;							//下次数据包扫描开始位置				
	UI8		    buf[PROT_RX_BUF_LEN];			//数据接收缓存 4字节对齐
	UI16		st;								//接收状态 	enPROT_RX_ST
	UI16        last_pkt_100ms;					//最近接受包的时间		
	const PROT_DEV*   pdev;							//对应的通信设备
	UI8			peer_id;						//通讯对方ID
	UI8			bdata_new;						//是否有新数据
	UI8			prot_id;						//协议索引 
}PROT_RX, *PPROT_RX;

//协议发送
typedef struct tyPROT_TX
{
	UI16			len;						//发送缓存的数据长度
	const PROT_DEV*	pdev;						//对应的通信设备
	UI8				peer_id;					//通讯对方ID
	UI8				btx;						//是否真正发送
	UI8				prot_id;					//协议索引 
	UI32			last_tx_ms;					//发送开始时间
	D_LINK_HD		req_tx_w;					//待发送的 发送请求列表
	D_LINK_HD		req_fin_w;					//数据包已拷贝到发送缓存的发送请求列表
	UI8				buf[PROT_TX_BUF_LEN];		//发送缓存 4字节
}PROT_TX, *PPROT_TX;

//协议模块处理状态机构体
typedef struct tyPROT_ST
{
	PROT_TX_REQ		tx_req[PROT_TX_REQ_NUM];	//空闲发送请求列表
	UI32			last_rx_ok_ms;				//上一次接收完成数据包的时间
	PROT_RX			prot_rx[PROT_PEER_NUM];		//接收
	PROT_TX			prot_tx[PROT_PEER_NUM];		//发送
}PROT_ST, *PPROT_ST;

#pragma  pack(1)
//通信协议数据包包头
typedef struct tyPROT_HD
{
	UI8		sot;							//数据包开始直接 SOT
	UI8		ver;							//协议版本，目前为0x01
	UI16	len;							//数据包总长度
	UI16	cmd;							//数据包命令
	UI16	id;								//数据对应的目标ID或序号
}PROT_HD, *PPROT_HD;

#define PROT_HD_TO_PKT(x)		(void*)((x) + 1)
#define PROT_PKT_TO_HD(x)		(((PPROT_HD)(x)) - 1)
#define SOT	0xEB
#define EOT	0xAD
#define PROT_HD_LEN		(sizeof(PROT_HD))
#define PROT_TAIL_LEN	2
#define PROT_PACK_LEN	(PROT_HD_LEN + PROT_TAIL_LEN)
#define PROT_VER	0x01
#define PROT_MAX_DAT_LEN	(PROT_MAX_PKT_LEN - PROT_PACK_LEN)

/*接口函数*/
void ProtAckHandle(UI16 cmd, UI16 id, UI8 prot_id);
BOOL ProtTxPkt(void* ppkt, UI8 prot_id);
void ProtInit(void);
void ProtTaskLoop(void);
void* ProtNewPkt(UI16 cmd, UI16 id, BOOL bneed_ack, UI16 pkt_len);
void ProtTaskLoop(void);
void ProtInit(void);
void MainProtReset(void);
BOOL MainProtTxIsIdle(void);
void ProtSendOldMMIWarn(void);
void MainProtErr(void);
void ProtDevDataIn(void* pdat, int len, UI8 peer_id, UI8 dev_id);
BOOL ProtTxIsIdle(UI8 prot_id);

extern void Udp_Uart_Reset(UI8 uart);
extern int Udp_Uart_Recieve(UI8 uart, UI8 peer_id, void* pbuf, int len);
extern int Udp_Uart_Write(UI8 uart, UI8 peer_id, void* pbuf, int len);
extern BOOL Udp_Uart_TxEnd(UI8 uart, void* pbuf);


#define MainProtTxPkt(ppkt)	ProtTxPkt(ppkt, 0)	
#pragma  pack()

#ifdef __cplusplus
}
#endif

#endif
