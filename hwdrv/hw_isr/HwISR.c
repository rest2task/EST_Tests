#include "bsp_can.h"
#include "bsp_uart.h"
#include "bsp_key_it.h"
#include "bsp_systick.h"
#include "fsl_flexcan.h"
#include "fsl_lpuart.h"
#include "fsl_flexcan.h"
#include "canhead.h"
#include "Configdf.h"
#include "machapi.h"
#include "Uart.h"
#include "cpld.h"
#include "adbios.h"
#include "dabios.h"
#include "App_data.h"
#include "hwmid.h"
#include "power.h"
#include "bsp_led.h"

void MemManage_Handler(void)
{
	mprintf("MemManage_Handler Error\r\n");
	HWRESET_SET ;	//主机重启													//P7_5引脚输出低电平, 自复位
	while(1)
	{
	}	
}

void BusFault_Handler(void)
{
	mprintf("BusFault_Handler Error\r\n");
	HWRESET_SET ;	//主机重启												//P7_5引脚输出低电平, 自复位
	while(1)
	{
	}	
}

void UsageFault_Handler(void)
{
	mprintf("UsageFault_Handler Error\r\n");
	HWRESET_SET ;	//主机重启													//P7_5引脚输出低电平, 自复位
	while(1)
	{
	}
}

/*******************************************************************************************
 *Brief			:AD中断
 *Param			:无
 *Return Value	:无
********************************************************************************************/
void AD_OK_IRQHandler(void)
{ 
   	uint16_t temp = 0;
    WORD tempchnl = 0;
	
    tempchnl = ad_channel;
    /* 清除中断标志位 */
    GPIO_PortClearInterruptFlags(AD_OK_GPIO,
                                 1U << AD_OK_GPIO_PIN); 	
    temp = AD_VAL;
	
    if (tempchnl < MAXADPORT)
	{
		AD_BUFFER[ad_channel] = (uint16_t)temp;
	}
	
    ad_channel++;	 

    if (ad_channel > MAXADPORT-1)
    {
        ad_main_start = 0;
        ad_channel = 0;
           
    }
    if (ad_main_start)
    {
        AD_Req(ad_channel);   
    }   
    __DSB(); 
}
/*******************************************************************************************
 *Brief			:DA中断
 *Param			:无
 *Return Value	:无
********************************************************************************************/
void DAIrq_Proc()
{
    WORD tmpword;

    if (da_q.tail != da_q.head)
    {
        tmpword = da_q.hw_q[da_q.tail];

        if (tmpword < 8)
        {
            m_da_start(tmpword, da_q.da_val[tmpword]);
        }
		
        da_q.tail = ((da_q.tail + 1) & (DA_Q_LEN - 1));
    }
}

/*********************************************************************************************************
* 函数名称 ：GPIO_IRQHandler
* 函数功能 ：GPIO中断服务程序，读扩展卡AD转换结果。
* 入口参数 ：无
* 出口参数 ：无
*********************************************************************************************************/
WORD last_ext_AD_BUFFER[8];
void EXT_AD_OK_IRQHandler(void)
{
	volatile u16 temp;
	volatile u32  STATISR;
    /* 清除中断标志位 */
	GPIO_PortClearInterruptFlags(EXT_AD_OK_GPIO, 1U << EXT_AD_OK_GPIO_PIN); 
	temp = (CPLD_D8_ADS8688_DL_REG | (CPLD_D8_ADS8688_DH_REG << 8));							//获取电子尺数据	

	ext_AD_BUFFER[ext_ad_channel] = (u16)temp;	
	ext_ad_channel ++;																			//通道次数++
	if (ext_ad_channel > (Y209_AD_CHANEL-1))
	{
		ext_main_start = 0;																		//所有通道采集完毕，状态置0
		ext_ad_channel = 0;
	
	}
	if (ext_main_start)																			//状态为1才能发送通道命令
	{
		EXT_AD_Req(ext_ad_channel);															//发送通道采集命令
	}
	__DSB(); 
}

/*******************************************************************************************
 *Brief			:时间中断
 *Param			:无
 *Return Value	:无
********************************************************************************************/
void SysTick_Handler(void)
{
	#ifdef ECAT_ENABLE
		ec_timer_isr();		//ECAT的时间中断
	#endif
	A_10usclock++;
    if (!(A_10usclock % 10))
    {
        A_100usclock++;                          
        g_clock.temp100us++;
    }
		
    if (g_clock.temp100us >= 10)
	{
        g_clock.temp100us = 0;
        A_1msclock++ ;                                                    //1ms
        g_clock.temp1ms ++;
        if (g_clock.temp1ms>=10)
        {
            g_clock.temp1ms =0;                                    //10ms
            A_10msclock++ ;
            g_clock.temp10ms ++;        
            if (g_clock.temp10ms>=10)
            {
                g_clock.temp10ms =0;
                A_100msclock++ ;
                g_clock.temp100ms ++;   
                if (g_clock.temp100ms>=10)
                {
                    g_clock.temp100ms =0;
                    A_1sclock++ ;
                }        
            }        
        }
    }

	//模拟量输出放在时间中断中
    if (A_10usclock % 5 == 0)   //50us发送1个通道，保证8个通道在一个周期内发送完成
    {
    	//DA输出函数放在应用层，在这里直接调用就行。
        DAIrq_Proc();  
    }	
}


/*******************************************************************************************
 *Brief			:串口中断
 *Param			:无
 *Return Value	:无
********************************************************************************************/
void PANEL_UART_IRQHandler(void)
{
	//发送
	//应用层需要发送串口数据
	if(UartTxSt())		//为真代表串口需要发送数据		  
	{
		if(kLPUART_TxDataRegEmptyFlag & LPUART_GetStatusFlags(PANEL_UART))
		{
			if(UartIsTxing()) //当发送次数小于发送长度，代表正在发送过程中
			{
				//应用层的串口数据正在发送
				//从应用层函数GetUartData()获得串口数据，并发送出去
				m_uart_send_byte(PANEL_UART,GetUartData());			
			}
			else 			
			{
				//应用层的串口数据发送完成
				//关闭串口中断
				LPUART_DisableInterrupts(PANEL_UART,kLPUART_TxDataRegEmptyInterruptEnable);

				//应用层的数据发完后，对部分参数赋值
				UartTxEnd(); 
			}	
		}
	}

	//接收
	if (kLPUART_RxDataRegFullFlag&LPUART_GetStatusFlags(PANEL_UART))
	{
		//往应用层填串口数据
		UartRecData((UI8)(LPUART_ReadByte(PANEL_UART) &((UI8)0xFF)));
	}
	else if(!UartTxSt())
	{
		//表示应用层没在发送 数据，关闭串口
		LPUART_DisableInterrupts(PANEL_UART,kLPUART_TxDataRegEmptyInterruptEnable);
	}

	/*以下代码是串口的错误处理*/
	if(kLPUART_RxOverrunFlag&LPUART_GetStatusFlags(PANEL_UART))	
	{		
		LPUART_ClearStatusFlags(PANEL_UART, (uint32_t)kLPUART_RxOverrunFlag);	
	}	
	if(kLPUART_FramingErrorFlag&LPUART_GetStatusFlags(PANEL_UART))	
	{		
		LPUART_ClearStatusFlags(PANEL_UART, (uint32_t)kLPUART_FramingErrorFlag);	
	}		
	if(kLPUART_RxActiveEdgeFlag&LPUART_GetStatusFlags(PANEL_UART))	
	{		
		LPUART_ClearStatusFlags(PANEL_UART, (uint32_t)kLPUART_RxActiveEdgeFlag);	
	}	

	__DSB();
	
}

/*******************************************************************************************
 *Brief			:485 中断
 *Param			:无
 *Return Value	:无
********************************************************************************************/
void RS485IRQHandler(void)
{
  if (kLPUART_RxDataRegFullFlag&LPUART_GetStatusFlags(RS485_UART))
  {
		RcvBuf[Power_RX_CN++] = LPUART_ReadByte(RS485_UART);
		if(Power_RX_CN>RX_TLEN-1)
			Power_RX_CN=0;
  }

  if(kLPUART_TxDataRegEmptyFlag & LPUART_GetStatusFlags(RS485_UART))
  {
		if(!(ChkSysCfg(CO43_POWERCAN)))
	 	{
		 	if(TX_CN<TX_LEN)
				LPUART_WriteByte(RS485_UART,SendBuf[TX_CN++]);
			else
			{
				LPUART_DisableInterrupts(RS485_UART,kLPUART_TxDataRegEmptyInterruptEnable);
				TX_CN=TX_CN+1;
				CS_RS485_RXD ;
			}
	 	}	
  }
  
  	/*以下代码是串口的错误处理*/
	if(kLPUART_RxOverrunFlag&LPUART_GetStatusFlags(RS485_UART))	
	{		
		LPUART_ClearStatusFlags(RS485_UART, (uint32_t)kLPUART_RxOverrunFlag);	
	}	
	if(kLPUART_FramingErrorFlag&LPUART_GetStatusFlags(RS485_UART))	
	{		
		LPUART_ClearStatusFlags(RS485_UART, (uint32_t)kLPUART_FramingErrorFlag);	
	}		
	if(kLPUART_RxActiveEdgeFlag&LPUART_GetStatusFlags(RS485_UART))	
	{		
		LPUART_ClearStatusFlags(RS485_UART, (uint32_t)kLPUART_RxActiveEdgeFlag);	
	}	
	__DSB();
	
}


/*******************************************************************************************
 *Brief			:CAN1中断
 *Param			:参数根据不同的平台的情况来填
 *Return Value	:无
********************************************************************************************/
void flexcan_callback(CAN_Type *base, flexcan_handle_t *handle, status_t status, uint32_t result, void *userData)
{
    switch (status)
    {
        case kStatus_FLEXCAN_RxIdle:
            if (RX1_MESSAGE_BUFFER_NUM == result)
            {
				//接收数据，放到接收缓存中，直接调用此函数就行
				CanReceiveInterrupt();
				
				rxXfer1.mbIdx = RX1_MESSAGE_BUFFER_NUM;
				rxXfer1.frame = &rx1_frame;
				FLEXCAN_TransferReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer1);
            }
            break;

        case kStatus_FLEXCAN_TxIdle:
			//发送放在缓存中的数据，直接调用此函数就行
			CanTransmitInterrupt();
            break;

        default:
            break;
    }
}

/*******************************************************************************************
 *Brief			:CAN2中断
 *Param			:参数根据不同的平台的情况来填
 *Return Value	:无
********************************************************************************************/
void flexcan2_callback(CAN_Type *base, flexcan_handle_t *handle, status_t status, uint32_t result, void *userData)
{
    switch (status)
    {
        case kStatus_FLEXCAN_RxIdle:
            if (RX2_MESSAGE_BUFFER_NUM == result)
            {
				//接收数据，放到接收缓存中，直接调用此函数就行
				Can2RxIsr();
				
				rxXfer2.mbIdx = RX2_MESSAGE_BUFFER_NUM;
				rxXfer2.frame = &rx2_frame;
				FLEXCAN_TransferReceiveNonBlocking(CAN2, &flexcan2Handle, &rxXfer2);
            }
            break;

        case kStatus_FLEXCAN_TxIdle:
			//发送放在缓存中的数据，直接调用此函数就行
			Can2TxIsr();
            break;

        default:
            break;
    }
}


/*******************************************************************************************
 *Brief			:CAN1的接收邮箱
 *Param			:p_rebuf 为应用层当前的空闲buf，需要将驱动层收到的数据放入
 *Return Value	:无
********************************************************************************************/
void CanRxFrameMailbox0()
{
	WORD	data_len;
	WORD	data_from_id;
	WORD	last_rx_buf[6];
	WORD	can_buf_ptr16[4];
	WORD	i;

	//获取can数据、ID、长度
	can_buf_ptr16[0] =(rxXfer1.frame->dataByte1<<8|rxXfer1.frame->dataByte0);
	can_buf_ptr16[1] =(rxXfer1.frame->dataByte3<<8|rxXfer1.frame->dataByte2);
	can_buf_ptr16[2] =(rxXfer1.frame->dataByte5<<8|rxXfer1.frame->dataByte4);
	can_buf_ptr16[3] =(rxXfer1.frame->dataByte7<<8|rxXfer1.frame->dataByte6);
	data_from_id = rxXfer1.frame->id>>18;
	data_len = rxXfer1.frame->length;



	//以下代码是接收到的数据进行处理，并放到接收buf中
	if (data_len>8)
	{  
		data_len = 8;  
	}

	last_rx_buf[0] = data_from_id;
	last_rx_buf[1] = data_len;
	last_rx_buf[2] = can_buf_ptr16[0];
	last_rx_buf[3] = can_buf_ptr16[1];
	last_rx_buf[4] = can_buf_ptr16[2];
	last_rx_buf[5] = can_buf_ptr16[3];

    Rx_CanData(data_from_id ,(UI8*)(&last_rx_buf[2]),data_len);

	//对数据第3、4、5的值进行大小端互换
	SWI_HIB_LOB(last_rx_buf[3]);
	SWI_HIB_LOB(last_rx_buf[4]);
	SWI_HIB_LOB(last_rx_buf[5]);

	//将can接收到的数据放到接收buf中
	for ( i=0; i<6; i++)
	{
		*g_canint.p_receive_rxbuf = last_rx_buf[i];
		g_canint.p_receive_rxbuf++;
	}
}
/*******************************************************************************************
 *Brief			:CAN1的发送邮箱
 *Param			:p_txbuf 为当期应用层要发送的数据buf,需要驱动层从中取出，并发送
 *Return Value	:无
********************************************************************************************/
void CanTxFrameMailbox4()
{
	WORD tx_msg_buf[6];
	
	//id
    tx_msg_buf[0] = *g_canint.p_send_txbuf++; 
	
	//长度
    tx_msg_buf[1] = (*g_canint.p_send_txbuf++)*2; 
    
	//4个word的数据
    tx_msg_buf[2] = *g_canint.p_send_txbuf++;
    tx_msg_buf[3] = *g_canint.p_send_txbuf++;
    tx_msg_buf[4] = *g_canint.p_send_txbuf++;
    tx_msg_buf[5] = *g_canint.p_send_txbuf++;	

	
	tx1_frame.id     = FLEXCAN_ID_STD(tx_msg_buf[0]);
	tx1_frame.format = kFLEXCAN_FrameFormatStandard;
	tx1_frame.type   = kFLEXCAN_FrameTypeData;
	tx1_frame.length = tx_msg_buf[1] ;

	tx1_frame.dataByte0 = (tx_msg_buf[2]&0xFF);
	tx1_frame.dataByte1 = ((tx_msg_buf[2]>>8)&0xFF);
	tx1_frame.dataByte2 = ((tx_msg_buf[3]>>8)&0xFF);
	tx1_frame.dataByte3 = (tx_msg_buf[3]&0xFF);
	tx1_frame.dataByte4 = ((tx_msg_buf[4]>>8)&0xFF);
	tx1_frame.dataByte5 = (tx_msg_buf[4]&0xFF);
	tx1_frame.dataByte6 = ((tx_msg_buf[5]>>8)&0xFF);
	tx1_frame.dataByte7 = (tx_msg_buf[5]&0xFF);
	txXfer1.mbIdx = TX1_MESSAGE_BUFFER_NUM;
	txXfer1.frame = &tx1_frame;
	FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer1);
}

/*******************************************************************************************
 *Brief			:CAN2的接收邮箱
 *Param			:pf为应用层当前的空闲buf，需要将驱动层收到的数据放入
 *Return Value	:无
********************************************************************************************/
BOOL Can2RxFrameMailbox(PCAN_FRAM pf)
{
    UI16    data_len;
    UI16    data_from_id;
    UI16    last_rx_buf[6];
    UI16    i;
	WORD	can_buf_ptr16[4];

	//获取can数据、ID、长度
	can_buf_ptr16[0] =(rxXfer2.frame->dataByte1<<8|rxXfer2.frame->dataByte0);
	can_buf_ptr16[1] =(rxXfer2.frame->dataByte3<<8|rxXfer2.frame->dataByte2);
	can_buf_ptr16[2] =(rxXfer2.frame->dataByte5<<8|rxXfer2.frame->dataByte4);
	can_buf_ptr16[3] =(rxXfer2.frame->dataByte7<<8|rxXfer2.frame->dataByte6);
	data_from_id = rxXfer2.frame->id>>18;
	data_len = rxXfer2.frame->length;

	
	if (data_len>8)
	{  
		data_len = 8;  
	}

	last_rx_buf[0] = data_from_id;
	last_rx_buf[1] = data_len;
	last_rx_buf[2] = can_buf_ptr16[0];
	last_rx_buf[3] = can_buf_ptr16[1];
	last_rx_buf[4] = can_buf_ptr16[2];
	last_rx_buf[5] = can_buf_ptr16[3];

    Rx_CanData(data_from_id ,(UI8*)(&last_rx_buf[2]),data_len);
	
	SWI_HIB_LOB(last_rx_buf[3]);
	SWI_HIB_LOB(last_rx_buf[4]);
	SWI_HIB_LOB(last_rx_buf[5]);

	if ( (g_canrxbuf.p_rxbuf_end - g_canint.p_receive_rxbuf) < 6)    //leave enough space to transmit
	{
		g_canint.p_receive_rxbuf = g_canrxbuf.p_rxbuf_start;
	}
	
	for ( i=0; i<6; i++)
	{
		*g_canint.p_receive_rxbuf = last_rx_buf[i];
		g_canint.p_receive_rxbuf++;
	}
	return FALSE;

} 

/*******************************************************************************************
 *Brief			:CAN2的发送邮箱
 *Param			:无
 *Return Value	:无
********************************************************************************************/
BOOL Can2TxFrameMailbox(PCAN_FRAM pf)
{
	//can2的发送采用传入pf的方式，因此can2请使用以下固定方式，
	tx2_frame.id     = FLEXCAN_ID_STD(pf->cob_id);
	tx2_frame.format = kFLEXCAN_FrameFormatStandard;
	tx2_frame.type   = kFLEXCAN_FrameTypeData;
	tx2_frame.length = pf->len;

	tx2_frame.dataByte0 = pf->data[0];
	tx2_frame.dataByte1 = pf->data[1];
	tx2_frame.dataByte2 = pf->data[2];
	tx2_frame.dataByte3 = pf->data[3];
	tx2_frame.dataByte4 = pf->data[4];
	tx2_frame.dataByte5 = pf->data[5];
	tx2_frame.dataByte6 = pf->data[6];
	tx2_frame.dataByte7 = pf->data[7];
	
	txXfer2.mbIdx = TX2_MESSAGE_BUFFER_NUM;
	txXfer2.frame = &tx2_frame;
	FLEXCAN_TransferSendNonBlocking(CAN2, &flexcan2Handle, &txXfer2);
	
	return TRUE;
}

