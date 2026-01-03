/******************************************************************************
  文 件 名   : uart.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 串口驱动 : 初始化 发送及接收
  函数列表   :
              UartBufIsTxEnd
              UartReset
              UartInit
              UartIsTxEnd
              UartRead
              UartReset
              UartWrite
              USART0_Isr
              USART1_IRQHandler
              USART2_IRQHandler
              USART3_IRQHandler
              USART_Isr
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

#include "string.h"
#include "uart.h"
#include "hwcfg.h"
#include "bsp_uart.h"
#include "fsl_lpuart.h"
#include "hwmid.h"

UART_CTRL uart_ctrl[UART_DEV_NUM];


void UartReset(UI8 uart)
{
	PUART_CTRL pctrl;

	if(uart < UART_DEV_NUM)
		pctrl = uart_ctrl + uart;
	else
		return ;

	pctrl->buf.hd =pctrl->buf.tl;

	pctrl->tx_len = 0;
	pctrl->tx_data = NULL;
	pctrl->tx_cnt = pctrl->tx_len + 1;
	pctrl->btx = FALSE;
	pctrl->notify_cnt_down = 0;
}

int UartRead(UI8 uart, UI8 peer_id, void* pbuf, int len)
{
	PUART_CTRL pctrl;
	UI16	hd;
	UI16	tl;
	UI16	tmp;
	
	if(pbuf == NULL || len < 0)
		return 0;

	if(uart < UART_DEV_NUM)
		pctrl = uart_ctrl + uart;
	else
		return 0;
		
	if(pctrl->notify_cnt_down > 0)
		return 0;

	hd = pctrl->buf.hd;
	tl = pctrl->buf.tl;
	if(hd == tl)
		return 0;

	if(hd < tl)
		hd += RING_BUF_SIZE;
	
	if(len > hd - tl)
		len = hd - tl;

	if(tl + len > RING_BUF_SIZE)
		tmp = len - ((tl + len)&(RING_BUF_SIZE - 1));
	else
		tmp = len;

	memcpy(pbuf, pctrl->buf.buf + tl, tmp);
	if(tmp < len)
		memcpy((UI8*)pbuf + tmp, pctrl->buf.buf, len - tmp);

	pctrl->buf.tl =((tl + len)&(RING_BUF_SIZE -1));
	return len;
}

int UartWrite(UI8 uart, UI8 peer_id, void* pbuf, int len)
{
	PUART_CTRL pctrl ;
	
	if(pbuf == NULL || len == 0)
		return 0;

	if(uart < UART_DEV_NUM)
		pctrl = uart_ctrl + uart;
	else
		return 0;
	
	if(pctrl->tx_cnt <= pctrl->tx_len)
		return 0;
	
	pctrl->btx = FALSE;


	//UART_TX_DISABLE(USART1);
	//mprintf("UART=%x\r\n", (USART_TypeDef*)(pctrl->hw_uart));
	m_uart_disable_tx();
	
	pctrl->tx_data = pbuf;
	pctrl->tx_cnt = 1;
	pctrl->tx_len = len;
	
    m_uart_send_byte((LPUART_Type *)pctrl->hw_uart, (((UI8*)pbuf)[0] & (UI16)0x1FF));

	pctrl->btx = TRUE;
	
	m_uart_enable_tx();

	return 1;
}
BOOL UartBufIsTxEnd(UI8 uart, void* pbuf)
{
	PUART_CTRL pctrl;

	if(uart < UART_DEV_NUM)
		pctrl = uart_ctrl + uart;
	else
		return TRUE;
		
	return (pbuf != (void*)pctrl->tx_data || pctrl->tx_cnt > pctrl->tx_len);
}
/*******************************************************************************************
 *Brief			:判断应用层的串口数据是否发送完成
 *Param			:无
 *Return Value	:返回真表示串口数据还在发送，返回假表示串口数据已经发完
********************************************************************************************/
BOOL UartIsTxing()
{
	PUART_CTRL pctrl = uart_ctrl;

	return (pctrl->tx_data != NULL && pctrl->tx_cnt < pctrl->tx_len	);
}

/*******************************************************************************************
 *Brief			:获取应用层需要发送的串口数据
 *Param			:无
 *Return Value	:返回应用层要发送的串口数据
********************************************************************************************/
UI8 GetUartData()
{
	PUART_CTRL pctrl = uart_ctrl;

	return (pctrl->tx_data[pctrl->tx_cnt++]& (UI16)0x01FF);
}

/*******************************************************************************************
 *Brief			:串口发送中断结束后，对部分参数赋值
 *Param			:
 *Return Value	:
********************************************************************************************/
void UartTxEnd()
{
	PUART_CTRL pctrl = uart_ctrl;
	/*加1，使pctrl->tx_cnt > pctrl->tx_len，表示发送结束*/
	pctrl->tx_cnt = pctrl->tx_len +1;
	pctrl->btx = FALSE;
}

/*******************************************************************************************
 *Brief			:应用层是否要发送串口数据
 *Param			:
 *Return Value	:返回真表示应用层在发送数据，返回假 表示应用层未发数据
********************************************************************************************/
BOOL UartTxSt()
{
	int btx;
	PUART_CTRL pctrl = uart_ctrl;
	btx = pctrl->btx;

	return btx;
}

/*******************************************************************************************
 *Brief			:从串口中断里接收数据
 *Param			:data 为串口中断中接收到的数据
 *Return Value	:无
********************************************************************************************/
void UartRecData(UI8 data)
{
	UI8	tmp;
	PUART_CTRL pctrl = uart_ctrl;
	
	if((tmp = R_BUF_INC(pctrl->buf.hd)) != pctrl->buf.tl)
	{
		pctrl->buf.buf[pctrl->buf.hd] = data;	
		pctrl->buf.hd = tmp;
	}
	
	if(R_BUF_DLEN(pctrl->buf) > (RING_BUF_SIZE >> 1))
		pctrl->notify_cnt_down = 0;
	else
		pctrl->notify_cnt_down = pctrl->byte_interval;	
}

void UartInit(UI32 br)
{
	int i;
	
	memset(&uart_ctrl, 0, sizeof(uart_ctrl));

	/*加1，tx_cnt > tx_len，表示发送结束*/
	for(i = 0; i < UART_DEV_NUM; i ++)
		uart_ctrl[i].tx_cnt = uart_ctrl[i].tx_len + 1;

	uart_ctrl[0].hw_uart = PANEL_UART;
}


