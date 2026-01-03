/******************************************************************************
  文 件 名   : prot.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 通信协议处理，数据包的接收和发送
  函数列表   :
              ProtAckHandle
              ProtBufInit
              ProtChkBufTxEnd
              ProtChkPkt
              ProtFreeBuf
              ProtFreeTxReq
              ProtGetBuf
              ProtGetTxReq
              ProtInit
              ProtMatchAck
              ProtNewPkt
              ProtPackHd
              ProtPackTail
              MainProtReset
              ProtRx
              ProtRxOK
              ProtRxReset
              ProtTaskLoop
              ProtTx
              MainProtTxIsIdle
              ProtTxPkt
              ProtTxReset
              ProtTxTimeOut
              ProtDevTx
  修改历史   :
  1.日    期   : 2013年5月17日
    作    者   : ren chaohong
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

#include "com.h"
#include "crc16.h"
#include "hwcfg.h"
#include "prot.h"
#include "protcmd.h"
#include "printf.h"
#include "machapi.h"
#include "systempro.h"
#include "can2.h"
#include "sram.h" 
#include "udpctrl.h" 

#ifndef SYS232

DEF_ALIGN UI8 prot_buf[(64 + BUF_REV_SIZE)* BUF_64_NUM + (128 + BUF_REV_SIZE)*BUF_128_NUM + (256 + BUF_REV_SIZE)*BUF_256_NUM 
				 		   + (512 + BUF_REV_SIZE)*BUF_512_NUM + (1024 + BUF_REV_SIZE)*BUF_1024_NUM];
#endif
static BUF_HD buf_lnk_hd[BUF_TYPE_NUM];
PROT_ST	prot_st;
UI16	Udp_uart_flag = 0;		//udp通讯，串口232通讯区分标记 ，为1 代表Udp通讯，为0 代表232通讯


/*define a dummy device */
void DummyReset(UI8 dev_id)
{
}

int DummyRead(UI8 dev_id, UI8 peer_id,  void* pbuf, int len)
{
	/*不支持直接读取，返回0*/
	return 0;
}

int DummyWrite(UI8 dev_id, UI8 peer_id,  void* pbuf, int len)
{
	return len;
}

BOOL DummyIsTxEnd(UI8 dev_id, void* pbuf)
{
	return TRUE;
}

#ifdef DM9000_ENABLE
void Udp_Uart_Reset(UI8 uart)	//udp,uart初始化调度接口函数
{
	if(Udp_uart_flag == 1)
	{
		UdpReset(uart);
	}
	else if(Udp_uart_flag == 0)
	{
		UartReset(uart);
	}
}

int Udp_Uart_Recieve(UI8 uart, UI8 peer_id, void* pbuf, int len)	//udp,uart接收调度函数
{
	if(Udp_uart_flag == 1)
	{
		return Udp_Data_Recieve(uart, peer_id, pbuf, len);
	}
	else if(Udp_uart_flag == 0)
	{
		return UartRead(uart, peer_id, pbuf, len);
	}
	else
	{
		return 0;
	}
}

int Udp_Uart_Write(UI8 uart, UI8 peer_id, void* pbuf, int len)	//udp,uart发送调度函数
{
	if(Udp_uart_flag == 1)
	{
		return Udp_Data_Tx(uart, peer_id, pbuf, len);
	}
	else if(Udp_uart_flag == 0)
	{
		return UartWrite(uart, peer_id, pbuf, len);
	}
	else 
	{
		return 0;
	}
}


BOOL Udp_Uart_TxEnd(UI8 uart, void* pbuf)	//udp,uart发送完毕判断调度函数
{
	if(Udp_uart_flag == 1)
	{
		return UdpBuf_IsTxEnd(uart, pbuf);
	}
	else if(Udp_uart_flag == 0)
	{
		return UartBufIsTxEnd(uart, pbuf);
	}
	else
	{
		return TRUE;
	}
}
#endif	


const PROT_DEV prot_dev[] = {
	
#ifdef DM9000_ENABLE
							{PROT_UART_DEV, DEV_NEEDREAD, Udp_Uart_Reset, Udp_Uart_Recieve, Udp_Uart_Write, Udp_Uart_TxEnd},
#else								
							{PROT_UART_DEV, DEV_NEEDREAD, UartReset, UartRead, UartWrite, UartBufIsTxEnd},
#endif	

/*第二个CAN 
Can2Read 目前不返回数据，CAN驱动主动调用ProtDevDataIn把数据读取到协议缓存*/
						   {PROT_CAN2_DEV, 0, Can2Reset, Can2Read, Can2Write, Can2BufIsTxEnd},

/*第二个调试串口*/
#ifdef DEBUG
/*如定义了DEBUG，第二个调试串口用于调试信息打印，不做协议处理*/
						   {PROT_UART2_DEV, 0, DummyReset, DummyRead, DummyWrite, DummyIsTxEnd}
#else
						   {PROT_UART2_DEV, DEV_NEEDREAD, UartReset, UartRead, UartWrite, UartBufIsTxEnd}
#endif
						   };
						   
void* ProtGetBuf(int size)
{
	int	i;
	PBUF_HD		phd;
	PBUF_LNK	plnk;
	
	if(size <= 0)
		return NULL;

	phd = buf_lnk_hd;
	for(i = 0; i < BUF_TYPE_NUM; i++)
	{
		if(phd->size >= size && phd->plnk != NULL)
		{
			plnk = phd->plnk;

			if(plnk->size != phd->size || plnk->bused)
			{
				/*memory buffer  error, do something here*/
				db_printf("prot buf error\r\n");
			}
			
			phd->plnk = plnk->pnext;
			plnk->bused = TRUE;
			//db_printf("Get buf %08x\r\n", plnk);
			return (void*)((int)plnk + BUF_REV_SIZE);
		}
		
		phd++;
	}

	return NULL;
}

BOOL ProtFreeBuf(void* pbuf)
{
	PBUF_LNK plnk;
	PBUF_HD phd;
	int	i;
	
	if(pbuf == NULL || pbuf < prot_buf || pbuf > prot_buf + sizeof(prot_buf))
		return FALSE;
		
	plnk = (PBUF_LNK)((int)(pbuf) - BUF_REV_SIZE);

	if(plnk->bused == FALSE)
		return FALSE;
		
	phd = buf_lnk_hd;
	for(i = 0; i< BUF_TYPE_NUM; i++)
	{
		if(phd->size == plnk->size)
		{
			plnk->pnext = phd->plnk;
			phd->plnk = plnk;
			plnk->bused = FALSE;
			return TRUE;
		}
		phd++;
	}

	//mprintf("tx buf err\r\n");
	/*memory buffer error, do something here*/
	return FALSE;
}

void ProtBufInit()
{
	int	i;
	int	j;
	UI8	buf_num[BUF_TYPE_NUM] = {BUF_64_NUM, BUF_128_NUM, BUF_256_NUM, BUF_512_NUM, BUF_1024_NUM};	//8,4,4,4,1
	UI16 buf_size[BUF_TYPE_NUM] = {64, 128, 256,512, 1024};
	PBUF_LNK plnk;
	PBUF_HD phd;
	
	plnk = (PBUF_LNK)prot_buf;
	for(i = 0; i < BUF_TYPE_NUM; i ++)
	{
		phd = buf_lnk_hd + i;
		phd->size = buf_size[i];
		phd->plnk = NULL;
		for(j = 0; j < buf_num[i]; j++)
		{
			plnk->size = buf_size[i];
			plnk->bused = FALSE;
			plnk->pnext = phd->plnk;
			phd->plnk = plnk;
			plnk = (PBUF_LNK)((int)plnk + buf_size[i] + BUF_REV_SIZE);
		}
	}
}

//__inline 
PPROT_TX_REQ ProtGetTxReq()
{
	int	i;
	PPROT_TX_REQ preq;

	preq = prot_st.tx_req;

	for(i = 0; i < PROT_TX_REQ_NUM; i++)
	{
		if(!preq->bused)
		{
			preq->bused = TRUE;
			return  preq;
		}
		preq++;
	}

	return NULL;
}

static __inline void ProtFreeTxReq(PPROT_TX_REQ preq)
{
	if(preq != NULL)
	{
		if(preq->pbuf != NULL)
		{
			ProtFreeBuf(preq->pbuf);
			preq->pbuf = NULL;
		}
		
		preq->bused = FALSE;
	}
 }

void ProtRxReset(PPROT_RX prx)
{
	prx->hd_ind = 0;
	prx->len = 0;
	prx->st = PROT_RX_START;
	prx->last_rx_ms = 0;
	prx->bdata_new =FALSE;
}

void ProtTxReset(PPROT_TX ptx)
{
	PPROT_TX_REQ	preq;

	ptx->btx = FALSE;
	ptx->last_tx_ms = 0;
	ptx->len = 0;

	while((preq = (PPROT_TX_REQ)DLinkDeQ(&ptx->req_fin_w)) != NULL)
		ProtFreeTxReq(preq);

	while((preq = (PPROT_TX_REQ)DLinkDeQ(&ptx->req_tx_w)) != NULL)
		ProtFreeTxReq(preq);
}


static __inline void ProtDevTx(PPROT_TX ptx)
{
	UI32 ms_ticks;

	if(ptx->len <= 0)
		return;
	
	ms_ticks = GetMsTick();
	if((*ptx->pdev->DevWrite)(ptx->pdev->dev_id, ptx->peer_id, ptx->buf, ptx->len) <= 0)
	{
		if(ptx->last_tx_ms != 0 && ms_ticks - ptx->last_tx_ms  > PROT_MAX_PKT_TX_MS*4)
		{
			/*UART halt, need reset*/
			(*ptx->pdev->DevReset)(ptx->pdev->dev_id);
			(*ptx->pdev->DevWrite)(ptx->pdev->dev_id, ptx->peer_id, ptx->buf, ptx->len);
		}
		else
			return;
	}

	if(ptx->len > 0)
	{
#if 0		
		int i;
		db_printf("Tx: ");
		for(i = 0; i < ptx->len; i++)
			db_printf("%02x ", ptx->buf[i]);
		db_printf("\r\n");
#endif		
	}

	ptx->btx = TRUE;
	ptx->last_tx_ms = ms_ticks;
}

void ProtTxTimeOut(PPROT_TX ptx)
{
	/*只处理主通信协议*/
	if(ptx != NULL && ptx->prot_id == 0)
	{
		if(prot_st.last_rx_ok_ms == 0 || (UI32)(GetMsTick() -  prot_st.last_rx_ok_ms) > PROT_FAIL_TM)
		{
			/*communicate fail */
			SetComState(FALSE);
			prot_st.last_rx_ok_ms = 0;
		}
	}
}

void ProtRxOK(PPROT_RX prx)
{
	/*只处理主通信协议*/
	if(prx != NULL && prx->prot_id == 0)
	{
		prot_st.last_rx_ok_ms = GetMsTick();
		SetComState(TRUE);
	}
}

static __inline void ProtChkBufTxEnd(PPROT_TX ptx)
{
	PPROT_TX_REQ	preq;
	UI32			ms_ticks;
	D_LINK*			pln;	
	D_LINK_HD*		phd;	

	if(ptx->btx)
	{
		ms_ticks = GetMsTick();
		if(!(*ptx->pdev->DevTxIsEnd)(ptx->pdev->dev_id, ptx->buf))
		{
			/*Is UART  halt? reset it*/
			if(ptx->last_tx_ms + PROT_MAX_PKT_TX_MS*2 < ms_ticks)
			{
				db_printf("Uart halt, clear\r\n");
				(*ptx->pdev->DevReset)(ptx->pdev->dev_id);
			}
			else
				return;
		}
		
		ptx->btx = FALSE;
		ptx->len = 0;
		while((preq = (PPROT_TX_REQ)DLinkDeQ(&ptx->req_fin_w)) != NULL)
		{
			if(!preq->bneed_ack)
				ProtFreeTxReq(preq);
			else
			{
				preq->last_tx_ms = ms_ticks;
				DLinkInQ(&ptx->req_tx_w, (D_LINK *)preq);
			}
		}
	}

	/*send tx req in waite link list*/
	phd = &(ptx->req_tx_w);
	pln = phd->hd.prev;
	ms_ticks = GetMsTick();
 	while(pln != &(phd->hd))
	{
		preq = (PPROT_TX_REQ)(pln);
		pln = pln->prev;
		if(preq->last_tx_ms == 0 || ms_ticks - preq->last_tx_ms >=  PROT_ACK_WAITE)
		{
			if(ptx->len + preq->len <= PROT_TX_BUF_LEN)
			{
				memcpy(ptx->buf + ptx->len, preq->pbuf, preq->len);
				ptx->len += preq->len;
				DLinkRemove((D_LINK *)preq);
				
				if(preq->bneed_ack)
				{
					 if(preq->retry >= PROT_TX_RETRY)
					 {
						/*tx fail, not receive ack from peer side, do somethin here*/
						//db_printf("wait req ack fail, %d\r\n", prot_st.uart1_rx.rx_bytes);
						ProtTxTimeOut(ptx);
						ProtFreeTxReq(preq);
					 }
					 else
					 {
						preq->retry++;
						DLinkInQ(&ptx->req_fin_w, (D_LINK *)preq);
					}
				}
				else
					ProtFreeTxReq(preq);
			}
			else
				break;
		}
	}
	
	ProtDevTx(ptx);		
};

BOOL ProtTx(void* pbuf, int len, BOOL bneed_ack, UI8 prot_id)
{
	PPROT_TX 		ptx;
	PPROT_TX_REQ	preq;
	
	if(len > PROT_MAX_PKT_LEN ||  len <= 0 ||pbuf == NULL || prot_id >= ARRAY_NUM(prot_st.prot_tx))
		return FALSE;
		
	ptx = prot_st.prot_tx + prot_id;
	if(!ptx->btx && len + ptx->len<= PROT_TX_BUF_LEN)
	{
#ifdef SYS232
		MemcpyPro(ptx->buf + ptx->len, pbuf, len);
#else
		memcpy(ptx->buf + ptx->len, pbuf, len);
#endif
		
		ptx->len += len;
		if(bneed_ack && (preq = ProtGetTxReq()) != NULL)
		{
			preq->bneed_ack = TRUE;
			preq->len =len;
			preq->pbuf = pbuf;
			preq->retry = 1;
			preq->last_tx_ms = 0;

			DLinkInQ(&ptx->req_fin_w, (D_LINK *)preq);
		}
		else
			ProtFreeBuf(pbuf);

		ProtDevTx(ptx);
	}
	else
	{
		if((preq = ProtGetTxReq()) == NULL)
			return FALSE;

		preq->bneed_ack =bneed_ack;
		preq->len =len;
		preq->pbuf = pbuf;
		preq->retry = 0;
		preq->last_tx_ms = 0;
		DLinkInQ(&ptx->req_tx_w, (D_LINK *)preq);
	}

	return TRUE;
}

BOOL MainProtTxIsIdle()
{
	return (!prot_st.prot_tx[0].btx);
}

BOOL ProtTxIsIdle(UI8 prot_id)
{
	if(prot_id < ARRAY_NUM(prot_st.prot_tx))
	{
		return (!prot_st.prot_tx[prot_id].btx);
	}
	else
	{
		return TRUE;
	}
}


static __inline void ProtChkPkt(PPROT_RX prx, BOOL btmout)
{
	UI8*		pn;
	UI8*		pt;
	UI16		len;
	UI16		minlen;
	UI16 		tlen;
	
	pn = prx->buf + prx->hd_ind;

	if(prx->st >= PROT_RX_HD)
		minlen = ((PPROT_HD)pn)->len;
	else
		minlen = PROT_PACK_LEN;

	len = prx->len;
	
	if(btmout)
	{
		prx->st = PROT_RX_START;
		minlen = PROT_PACK_LEN;
		pn++;
	}
	
	while(pn + minlen <= prx->buf + len)
	{
		if(prx->st == PROT_RX_START)
		{
			//if(*pn == SOT && ((PPROT_HD)pn)->ver == PROT_VER && GET_UI16(&((PPROT_HD)pn)->len) <= PROT_MAX_PKT_LEN && GET_UI16(&((PPROT_HD)pn)->len) >= PROT_PACK_LEN)
			//if(phd->sot == SOT && phd->ver == PROT_VER && phd->len <= PROT_MAX_PKT_LEN)
			if(*pn == SOT && ((PPROT_HD)pn)->ver == PROT_VER)
			{
				tlen = GET_UI16(&((PPROT_HD)pn)->len) ;
				if( tlen <= PROT_MAX_PKT_LEN && tlen >= PROT_PACK_LEN)
				{
					prx->st = PROT_RX_HD;
					minlen = ((PPROT_HD)pn)->len;
				}
				else
					pn++;
			}
			else
				pn++;
		}
		else if(prx->st == PROT_RX_HD)
		{
//			if(*(pn + minlen - 1) == EOT && CRC_CCITT16(pn, minlen -  PROT_TAIL_LEN) == *((UI16*)(pn + minlen - PROT_TAIL_LEN)))
			if(CRC_CCITT16(pn, minlen - PROT_TAIL_LEN) == GET_UI16(pn + minlen - PROT_TAIL_LEN))
			//if(CRC_CCITT16(pn, minlen - PROT_TAIL_LEN))
			{
				/*EOT find, CRC16 check ok, prot pkt recv*/

				ProtRxOK(prx);

				//保证传给ProtPktRecv包地址是4字节对齐
				pt = pn;
				if((UI32)pt & 0x03)
				{
					//没有4字节对齐，拷贝内存前移，使pn为4字节对齐
					pt = (UI8*)((UI32)pt & (~(UI32)0x03));
					if(pt >=prx->buf)
						memcpy(pt, pn, minlen);
					else
						db_printf("prot rx buf err\r\n");
				}

				ProtPktRecv((PPROT_HD)pt, prx->prot_id);
				prx->last_pkt_100ms	= Get100MsTick();
				prx->pkt_bytes += minlen;
				pn += minlen;
			}
			else
			{
				//db_printf("pkt crc error, id = %04x, cmd = %04x, len = %d\r\n", ((PPROT_HD)pn)->id, ((PPROT_HD)pn)->cmd, ((PPROT_HD)pn)->len);
				pn++;
			}
			prx->st = PROT_RX_START;
			minlen = PROT_PACK_LEN;
		}
		else
			pn++;
	}

	prx->hd_ind = pn - prx->buf;
	if(pn + minlen > prx->buf + PROT_RX_BUF_LEN || prx->hd_ind == prx->len)
	{
		if(prx->hd_ind > 0 && prx->hd_ind <= prx->len)
		{
			if(prx->len > sizeof(prx->buf))
				db_printf("error\r\n");

			prx->len = prx->len - prx->hd_ind;
			
			if(prx->len > 0)
				memcpy(prx->buf, prx->buf + prx->hd_ind, prx->len);
			
			prx->hd_ind = 0;
		}
		else
		{
			/*error, reset rx state*/
			ProtRxReset(prx);
		}
	}
}

static __inline void ProtDevRead(PPROT_RX prx)
{
	int	rn;

	if(prx == NULL || prx->pdev->DevRead == NULL)
		return;

	do
	{
		if(prx->len >= PROT_RX_BUF_LEN)
			ProtRxReset(prx);
		
		rn = (*prx->pdev->DevRead)(prx->pdev->dev_id, prx->peer_id, prx->buf + prx->len, PROT_RX_BUF_LEN - prx->len);
		if(rn > 0)
		{
#if 0			
			int i;
			
			db_printf("RX:");
			for(i = 0; i < rn; i++)
				db_printf("%02x ", prx->buf[prx->len + i]);
			db_printf("\r\n");
#endif		
			prx->len += rn;
			prx->rx_bytes += rn;
			
			ProtChkPkt(prx, FALSE);
			prx->last_rx_ms = GetMsTick();
		}
		else if(prx->len > 0)
		{
			/*判断接收超时，如果接收等待时间已经超过等待时间，超时*/
#if 0			
			static int max_rx_tm = 0;
			if(max_rx_tm < GetMsTick() - prx->last_rx_ms)
			{
				max_rx_tm = GetMsTick() - prx->last_rx_ms;
			}

			db_printf("max_rx_tm : %d\r\n", max_rx_tm);
#else
			if((UI32)(GetMsTick() - prx->last_rx_ms) > PROT_RX_PKT_TMOUT)
			{
				db_printf("Prot rx pkt timeout, %d, %d\r\n", prx->len, prx->st);
				ProtChkPkt(prx, TRUE);
				ProtRxReset(prx);
			}
#endif
		}
	}while(rn != 0);
}

static __inline void ProtDataScan(PPROT_RX prx)
{
	if(prx == NULL)
		return;

	if(prx->bdata_new)
	{
		ProtChkPkt(prx, FALSE);
		prx->last_rx_ms = GetMsTick();
		prx->bdata_new = FALSE;
		
		if(prx->len >= PROT_RX_BUF_LEN)
			ProtRxReset(prx);
	}
	else if((UI32)(GetMsTick() - prx->last_rx_ms) > PROT_RX_PKT_TMOUT && prx->len > 0)
	{
		/*判断接收超时，如果接收等待时间已经超过等待时间，超时*/
		db_printf("Prot rx pkt timeout, %d, %d\r\n", prx->len, prx->st);
		ProtChkPkt(prx, TRUE);
		ProtRxReset(prx);
	}
}

#if 0
UI8 prot_test_data[1024];
int prot_test_len;
#endif

void ProtDevDataIn(void* pdat, int len, UI8 peer_id, UI8 dev_id)
{
	int i;
	PPROT_RX prx;

	if(pdat == NULL || len <= 0)
		return;
	
	/*接收到数据，第一个为UART，需要主动读取，i 从1开始*/
	for(i = 1; i < ARRAY_NUM(prot_st.prot_rx); i++)
	{
		prx = prot_st.prot_rx + i;
		if(prx->pdev->dev_id == dev_id && prx->peer_id == peer_id)
		{
			if(prx->len >= PROT_RX_BUF_LEN)
				ProtRxReset(prx);

			if(len > PROT_RX_BUF_LEN - prx->len)
				len = PROT_RX_BUF_LEN - prx->len;

			memcpy(prx->buf + prx->len, pdat, len);

#if 0
			if(prot_test_len + len > sizeof(prot_test_data))
			{
				prot_test_len = 0;
				memset(prot_test_data, 0, sizeof(prot_test_data));
			}
			memcpy(prot_test_data + prot_test_len, pdat, len);
			prot_test_len += len;
#endif

#if 0
{
			int i;
			
			db_printf("RX:");
			for(i = 0; i < len; i++)
				db_printf("%02x ", prx->buf[prx->len + i]);
			db_printf("\r\n");
}
#endif		
			prx->len += len;
			prx->bdata_new = TRUE;
			prx->rx_bytes += len;
			break;
		}
	}
}

static __inline void ProtMatchAck(D_LINK_HD* phd, UI16 cmd, UI16 id)
{
	PPROT_TX_REQ preq;
	D_LINK* pln;
	
	pln =phd->hd.next;
 	while(pln != &(phd->hd))
	{
		preq = (PPROT_TX_REQ)pln;
		pln = pln->next;

		if(preq->bneed_ack && preq->pbuf != NULL && preq->retry > 0 && ((PPROT_HD)preq->pbuf)->sot == SOT && 
		  (((PPROT_HD)preq->pbuf)->cmd & CMD_BITS) == (cmd & CMD_BITS) && ((PPROT_HD)preq->pbuf)->id == id)
		{
			//db_printf("Ack Mach\r\n");
			DLinkRemove((D_LINK *)preq);
			ProtFreeTxReq(preq);
			SetPktAckOK(id, cmd);
		}
	}
}


__inline PPROT_HD ProtPackHd(PPROT_HD phd, UI16 cmd, UI16 id, BOOL bneed_ack, UI16 len)
{
	PROT_HD phd_a;
	PPROT_HD phd_b = &phd_a;
	if(phd == NULL || len > PROT_MAX_PKT_LEN)
		return NULL;

	
	phd_b->sot = SOT;
	
	if(bneed_ack)	
		phd_b->cmd = ((cmd & CMD_BITS) | CMD_ACK_BIT);
	else
		phd_b->cmd = (cmd & CMD_BITS);
	phd_b->id = id;
	phd_b->len = len;
	phd_b->ver = PROT_VER;

#ifdef SYS232
				MemcpyPro(phd, phd_b, sizeof(PROT_HD));
#else
				memcpy(phd,phd_b,sizeof(PROT_HD));
#endif
	
	return phd;
}

void* ProtNewPkt(UI16 cmd, UI16 id, BOOL bneed_ack, UI16 pkt_len)
{
	PPROT_HD phd;

	if(pkt_len > PROT_MAX_PKT_LEN - PROT_PACK_LEN)
		return NULL;

	if((phd = ProtGetBuf(pkt_len + PROT_PACK_LEN)) != NULL)
	{
		ProtPackHd(phd, cmd, id, bneed_ack, pkt_len + PROT_PACK_LEN);
		return (phd + 1);
	}
	else
	{
		//mprintf("NULL free buf\r\n");
		return NULL;
	}
}

__inline BOOL ProtPackTail(PPROT_HD phd)
{
	if(phd->len <= PROT_MAX_PKT_LEN - PROT_PACK_LEN && phd->len >= PROT_PACK_LEN)
	{
		 //*(UI8*)((int)(phd) + phd->len - 1) = EOT; 
		 *(UI16*)((int)(phd) + phd->len - PROT_TAIL_LEN) = CRC_CCITT16(phd, phd->len - PROT_TAIL_LEN);
	}
	else
		return FALSE;

	return TRUE;
}

BOOL ProtTxPkt(void* ppkt, UI8 prot_id)
{
	PPROT_HD phd;
	PBUF_LNK plnk;
		
	
	if(ppkt == NULL)
		return FALSE;
		
	phd = PROT_PKT_TO_HD(ppkt);

	plnk = (PBUF_LNK)((int)(phd) - BUF_REV_SIZE);
	if(plnk->size < phd->len)
		db_printf("pkt too long\r\n");
	
	if(!ProtPackTail(phd) || !ProtTx(phd, phd->len, (phd->cmd& CMD_ACK_BIT) == CMD_ACK_BIT, prot_id))
	{
		ProtFreeBuf(phd);
		return FALSE;
	}

	return TRUE;
}


void ProtAckHandle(UI16 cmd, UI16 id, UI8 prot_id)
{
	PPROT_TX	ptx;

	if(prot_id >= ARRAY_NUM(prot_st.prot_tx))
		return;
	
	ptx = prot_st.prot_tx + prot_id;
	
	//if(cmd ==( KEYPAD_CMD | 0x8000))
	//	db_printf("Ack Recv, id = %04x, cmd = %04x\r\n", id, cmd);
	
	ProtMatchAck(&ptx->req_fin_w, cmd, id);
	ProtMatchAck(&ptx->req_tx_w, cmd, id);
}

void ProtTaskLoop()
{
	int i;
	PPROT_RX prx;

	for(i = 0; i < ARRAY_NUM(prot_st.prot_rx); i++)
	{
		ProtChkBufTxEnd(prot_st.prot_tx +i);
		prx = prot_st.prot_rx +i;
		if(prx->pdev->attr & DEV_NEEDREAD)
		{
			/*需要先读取*/
			ProtDevRead(prx);
		}
		else
		{
			/*设备已回调ProtDevDataIN读取到了协议数据，只需要检测包*/
			ProtDataScan(prx);
		}
		ProtChkBufTxEnd(prot_st.prot_tx);
	}
}

void MainProtReset()
{
	ProtRxReset(prot_st.prot_rx);
	ProtTxReset(prot_st.prot_tx);
#ifdef DM9000_ENABLE
	Udp_Uart_Reset(PROT_UART_DEV);
#else
	UartReset(PROT_UART_DEV);
#endif
}

void ProtInit()
{
	int i;
	
	ProtBufInit();

	memset(&prot_st, 0, sizeof(prot_st));
	for(i = 0; i < ARRAY_NUM(prot_st.prot_tx); i++)
	{
		DLinkHDInit(&prot_st.prot_tx[i].req_fin_w);
		DLinkHDInit(&prot_st.prot_tx[i].req_tx_w);
	}

	/*第一个发送接收为缺省的面板串口通讯*/
	prot_st.prot_rx[PROT_UART_PANEL].peer_id = PROT_PEER_PANEL;
	prot_st.prot_rx[PROT_UART_PANEL].pdev = prot_dev;			/*UART1*/
	prot_st.prot_tx[PROT_UART_PANEL].peer_id = PROT_PEER_PANEL;
	prot_st.prot_tx[PROT_UART_PANEL].pdev = prot_dev;			/*UART1*/
	prot_st.prot_rx[PROT_UART_PANEL].prot_id = PROT_UART_PANEL;
	prot_st.prot_tx[PROT_UART_PANEL].prot_id = PROT_UART_PANEL;

	/*第二个发送接收为与PC机示波器监控软件通信*/
	prot_st.prot_rx[PROT_MON_PC].peer_id = PROT_PEER_PC;
	prot_st.prot_tx[PROT_MON_PC].peer_id = PROT_PEER_PC;
	prot_st.prot_rx[PROT_MON_PC].prot_id = PROT_MON_PC;
	prot_st.prot_tx[PROT_MON_PC].prot_id = PROT_MON_PC;
#ifdef DEBUG
	/*在调试状态下使用CAN2*/
	prot_st.prot_tx[PROT_MON_PC].pdev = prot_dev + 1;			//CAN2
	prot_st.prot_rx[PROT_MON_PC].pdev = prot_dev + 1;			//CAN2
#else
	/*使用串口2*/
	prot_st.prot_tx[PROT_MON_PC].pdev = prot_dev + 2;			//UART2
	prot_st.prot_rx[PROT_MON_PC].pdev = prot_dev + 2;			//UART2
#endif
}

/*主协议出错*/
void MainProtErr()
{
	PPROT_RX prx;

	prx = prot_st.prot_rx;
    if(Get100MsTick() - prx->last_pkt_100ms > 50)
	{
		ProtSendOldMMIWarn();
		prx->last_pkt_100ms = Get100MsTick();
	}
}


/*如果是老面板，按老的串口通信协议发送版本不匹配警报*/
#define COM_TX_OPERSTAT     0x112A  		 //操作状态
void ProtSendOldMMIWarn()
{
	UI16 old_err_warn[] = {0xEB90, COM_TX_OPERSTAT, 0x0516, 0x000B, 0x0000, 0x0000, 0x0000, 0x0000, (UI16)(1 << 15), 0x0000, 0x0000};
	UI16 chksum = 0;
	int	i;
	
	/*先把串口通信波特率到19200*/
	UartInit(19200);
	MainProtReset();
	
	for(i = 1; i < ARRAY_NUM(old_err_warn) - 1; i++)
		chksum = chksum^old_err_warn[i];
	old_err_warn[i] = chksum;

	for(i = 0; i <  ARRAY_NUM(old_err_warn); i++)
	{
		old_err_warn[i] = (((old_err_warn[i] >> 8) &0xff) | (( old_err_warn[i] << 8) & 0xff00));
	}
	
	ProtTx(old_err_warn, sizeof(old_err_warn), FALSE, 0);
	delay_ms(40);
	ProtTaskLoop();

	/*切换到新通信协议*/
	UartInit(RS232_BR);
	MainProtReset();
}


