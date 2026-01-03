#include "canhead.h"
#include "can2.h"
#include "prot.h"
#include "string.h"
#include "hwmid.h"

CAN can2;

void Can2RxIsr()
{
	UI8	tail;

	if((tail =((can2.rx_que.tail + 1) & (CAN_Q_FRAM_NUM -1))) != can2.rx_que.head)
	{
		if(Can2RxFrameMailbox(can2.rx_que.fram + can2.rx_que.tail ))
			can2.rx_que.tail = tail;
	}
	else
	{
		/*rx queu buffer full*/
	}
}

void Can2TxIsr()
{
	UI8	head;

	head =can2.tx_que.head;
	if(head != can2.tx_que.tail)
	{
		if(Can2TxFrameMailbox(can2.tx_que.fram + head))
			can2.tx_que.head = ((head + 1) & (CAN_Q_FRAM_NUM -1));
	}
	else
	{
		/*tx queu emptyl*/
	}
}

void Can2Init()
{
	m_can2_init_device(BPS_500K);
	memset(&can2, 0, sizeof(can2));
}

void Can2Rx()
{
	PCAN_QUE	prx;
	UI8			head;
	PCAN_FRAM	pf;
	
	prx = &can2.rx_que;
	head = prx->head;
	while(head != prx->tail)
	{
		pf = prx->fram + head;
		if(pf->len > 2)
		{
			/*只处理SDO数据，SDO数据走高层通讯协议*/
			if((pf->cob_id & 0x780) == CAN_SDO_TX_CODE)
			{
				ProtDevDataIn(pf->data + 2, pf->len - 2, pf->data[0]&0x7f, PROT_CAN2_DEV);
			}
			else
			{
				/*这里处理PDO数据*/
			}
		}
		
		head = ((head + 1)&(CAN_Q_FRAM_NUM - 1));
		prx->head = head;
	}
}

BOOL CopySDOToTxQue(PCAN_SDO_TX psdo)
{
	UI8 tail;
	PCAN_FRAM pf;
	UI16	tx_len;
	
	if(psdo == NULL || psdo->tx_cnt >= psdo->tx_len || psdo->buf == NULL)
		return TRUE;

	while((tail = ((can2.tx_que.tail + 1) & (CAN_Q_FRAM_NUM - 1))) != can2.tx_que.head)
	{
		pf = can2.tx_que.fram + can2.tx_que.tail;
		/*发送方为自己*/
		pf->data[0]= CAN_ID_ME;		
		/*sdo包号设为0*/
		pf->data[1]= 0;				
		pf->cob_id = (psdo->can_id | CAN_SDO_TX_CODE);
		//pf->cob_id = (psdo->can_id | CAN_PDO_TX_CODE);
		tx_len = psdo->tx_len - psdo->tx_cnt; 
		if(tx_len > 6)
		{
			/*加上数据前两个字节: 源id和包号*/
			pf->len = 8;
			memcpy(pf->data + 2, (UI8*)psdo->buf + psdo->tx_cnt, 6);
			psdo->tx_cnt += 6;
		}
		else 
		{
			memcpy(pf->data + 2, (UI8*)psdo->buf + psdo->tx_cnt, tx_len);
			psdo->tx_cnt += tx_len;
			/*加上数据前两个字节: 源id和包号*/
			pf->len = tx_len + 2;
			can2.tx_que.tail = tail;
			return TRUE;
		}

		can2.tx_que.tail = tail;
	}

	return FALSE;
}

void Can2TxBufWrite(WORD *last_tx_buf)
{
	UI8 tail;
	PCAN_FRAM pf;

	if(((tail = ((can2.tx_que.tail + 1) & (CAN_Q_FRAM_NUM - 1))) != can2.tx_que.head))
	{
		pf = can2.tx_que.fram + can2.tx_que.tail;

		pf->cob_id = last_tx_buf[0];
		pf->len = last_tx_buf[1]*2;
		pf->data[0] = last_tx_buf[2];		
		pf->data[1] = last_tx_buf[2]>>8;

		pf->data[2] = last_tx_buf[3]>>8;
		pf->data[3] = last_tx_buf[3];
		pf->data[4] = last_tx_buf[4]>>8;
		pf->data[5] = last_tx_buf[4];
		pf->data[6] = last_tx_buf[5]>>8;
		pf->data[7] = last_tx_buf[5];

		can2.tx_que.tail = tail;
	}
}

void Can2Tx()
{
	UI8 head = can2.tx_que.head;
	int i;
	
	if(m_can2_tx_compltet())
	{
		for(i = 0; i < ARRAY_NUM(can2.sdo_tx); i++)
		{
			if(can2.sdo_tx[i].tx_cnt < can2.sdo_tx[i].tx_len)
			{
				if(!CopySDOToTxQue(can2.sdo_tx + i))
					break;
			}
		}
		
		if(head != can2.tx_que.tail)
		{
			/*启动发送*/
			if(Can2TxFrameMailbox(can2.tx_que.fram + head))
				can2.tx_que.head = ((head + 1) & (CAN_Q_FRAM_NUM -1));
		}
	}
}

void Can2Reset(UI8 dev_id)
{
	int i;
	
	m_can2_init_device(500);

	can2.rx_que.head = can2.rx_que.tail;
	can2.tx_que.tail = can2.rx_que.head;

	for(i = 0; i < ARRAY_NUM(can2.sdo_tx); i++)
	{
		can2.sdo_tx[i].buf = NULL;
		can2.sdo_tx[i].tx_cnt = 0;
		can2.sdo_tx[i].tx_len= 0;
	}
}

int Can2Read(UI8 dev_id, UI8 peer_id,  void* pbuf, int len)
{
	/*不支持直接读取，返回0*/
	return 0;
}

int Can2Write(UI8 dev_id, UI8 peer_id,  void* pbuf, int len)
{
	int i;
	PCAN_SDO_TX  psdo = NULL;
	PCAN_SDO_TX  ptmp;
	
	if(pbuf == NULL || len <= 0)
		return 0;

	for(i = 0; i < ARRAY_NUM(can2.sdo_tx); i++)
	{
		ptmp = can2.sdo_tx + i;
		if(ptmp->can_id == peer_id)
		{
			psdo = ptmp;
			break;
		}
		else if(ptmp->tx_cnt >= ptmp->tx_len)
			psdo = ptmp;
	}

	if(psdo == NULL)
		return -1;

	psdo->buf = pbuf;
	psdo->tx_cnt = 0;
	psdo->tx_len = len;
	//psdo->tx_len = 6;
	
	psdo->can_id = peer_id;
	CopySDOToTxQue(psdo);
	
	return 1;
}

BOOL Can2BufIsTxEnd(UI8 dev_id, void* pbuf)
{
	int i;
	
	if(pbuf == NULL)
		return TRUE;

	for(i = 0; i < ARRAY_NUM(can2.sdo_tx); i++)
	{
		if(can2.sdo_tx[i].buf == pbuf && can2.sdo_tx[i].tx_cnt < can2.sdo_tx[i].tx_len)
			return FALSE;
	}

	return TRUE;
}

void Can2Loop()
{
	Can2Rx();
	/*处理PDO数据发送*/

	/*处理SDO数据发送*/
	Can2Tx();
}
