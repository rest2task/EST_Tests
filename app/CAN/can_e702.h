#ifndef CANE702_H
#define CANE702_H

#include "common.h"           //dsp上的common.h
#include "canhead.h"

typedef struct tagCanE702Comm CanE702Comm;
struct tagCanE702Comm {
    WORD    node_state;
    BOOL    flag_send;
    WORD*   p_origin_data;
    WORD    origin_data_length;    
    BOOL    flag_wait;
    WORD    send_count;
    WORD    reply_time;
    WORD    reply_count;
    BOOL    comm_err;
    WORD    tx_buf[4];
    WORD    rx_buf[4];
    WORD    node_guarding_time;
    WORD    node_guarding_count;
    WORD    heart_starttime;
    
		WORD		E702_pcb_version;		// E702_pcb_版本=LOBYTE(rxbuf[3])
		WORD		E702_soft_version;	// E702_软件版本=rxbuf[4]
		WORD		E702_DataAddr;			// 申请读数据返回的地址值
		WORD		E702_DataReceive;		// 申请读数据返回的数据值

		WORD		handshake;

		WORD		delayflag;
		WORD		delaytime;
};

extern  CanE702Comm    g_cane702comm[DA_MAX_NUM];

void	E702_CanStructInit(void);
void	E702_CanNodePresenceCheck(WORD *rx_buf);//====接收到E702的握手信号的处理
void	E702_CanNodePresenceRevert(WORD kind,WORD id);//====回复握手信号给E702的处理
void	E702_CanSDOTransmitBufferWrite(WORD *p_tx_data, WORD func_code, WORD dst_node, WORD send_count);//======将发送数据填入发送缓冲区
void	E702_CanTransmit (void);//======设置发送到E702的数据
void	E702_DataWrite(WORD	id,WORD	data0,WORD	data1,WORD data2);//======应用程序中调用的发送到E702的数据设置
void	E702_Port1_DataWrite(WORD pres,WORD	flow,WORD Direction);
void	E702_Port2_DataWrite(WORD pres,WORD	flow,WORD Direction);
void	E702_Port1_AbsDataWrite(WORD pres,WORD	flow);
void	E702_Port2_AbsDataWrite(WORD pres,WORD	flow);
void	E702_DataSafe(WORD addr,WORD data);
void	E702_DataRead(WORD addr,WORD data);
void	E702_DataReadReceive(WORD *rx_buf);
void	E702_HeartReceive(WORD *rx_buf);//======接收到E702的心跳信号的处理
void	E702_HeartSend(WORD kind,WORD id);//======发送心跳信号到E702的处理
void	ApplyNodePresenceFromE702(WORD *rx_buf);//======主机向E702请求握手信号
void	E702_DataRequest(void);
void	E702_CanStructInitsystem(void);
void	E702_CanTxDelayFlagSet(WORD flag);
void	E702_CanTransmitDelay(void);
void    Can_E700B_test(void);

#endif
