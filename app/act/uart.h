#ifndef _UART_H_
#define _UART_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "types.h"
#include "typedef.h"
#include "definelist.h"
#define UART_BAUDRATE		19200
#define UART_TXE_ST			0x80
#define UART_RXNE_ST		0x20
#define UART_TC_ST			0x40
#define UART_ORE_ST			0x08
#define UART_DEV_NUM		2

typedef struct tyUART_CTRL
{
	void*			hw_uart;
	int				tx_cnt;
	int				tx_len;
	int				btx;
	UI8*			tx_data;
	R_BUF 			buf;
	UI8				byte_interval;	/*MS*/
	UI32			notify_cnt_down;
}UART_CTRL, *PUART_CTRL;

extern int UartRead(UI8 uart, UI8 peer_id, void* pbuf, int len);
extern int UartWrite(UI8 uart, UI8 peer_id, void* pbuf, int len);
extern void UartReset(UI8 uart);
extern BOOL UartBufIsTxEnd(UI8 uart, void* pbuf);
extern BOOL UartIsTxing(void);
void UartInit(UI32 br);
UI8 GetUartData(void);
void UartTxEnd(void);
BOOL UartTxSt(void);
void UartRecData(UI8 data);

extern UART_CTRL uart_ctrl[UART_DEV_NUM];


#ifdef __cplusplus
}
#endif

#endif
