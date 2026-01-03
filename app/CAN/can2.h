#ifndef _CAN2_H_
#define _CAN2_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"

#define CAN_PAYLOAD_LEN	8
#define CAN_Q_FRAM_NUM		32

/*PC 监控端*/
#define PC_CAN_ID		0x71
/*智能注射卡*/
#define IINJ_CAN_ID		0x72

/*只接收PC端SDO包*/
#define CAN_SDO_NUM			2

typedef struct tyCAN_FRAM
{
	UI16	cob_id;
	UI8		len;
	UI8		rsv;
	UI8		data[CAN_PAYLOAD_LEN];
}CAN_FRAM, *PCAN_FRAM;

typedef struct tyCAN_QUE
{
	UI8			head;
	UI8			tail;
	CAN_FRAM	fram[CAN_Q_FRAM_NUM];
}CAN_QUE, *PCAN_QUE;

typedef struct tyCAN_SDO_TX
{
	UI16	tx_len;
	UI16	tx_cnt;
	UI8		can_id;
	void*	buf;
}CAN_SDO_TX, *PCAN_SDO_TX;

typedef struct tyCAN
{
	CAN_QUE	 	rx_que;
	CAN_QUE	 	tx_que;
	CAN_SDO_TX sdo_tx[CAN_SDO_NUM];
}CAN, *PCAN;

void Can2RxIsr(void);
void Can2TxIsr(void);
void Can2Reset(UI8 dev_id);
int Can2Write(UI8 dev_id, UI8 peer_id, void* pbuf, int len);
int Can2Read(UI8 dev_id, UI8 peer_id,  void* pbuf, int len);
BOOL Can2BufIsTxEnd(UI8 dev_id, void* pbuf);
void Can2Init(void);
void Can2Loop(void);
void Can2TxBufWrite(WORD *last_tx_buf);

#ifdef __cplusplus
}
#endif

#endif

