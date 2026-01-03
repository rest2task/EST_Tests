#include "power.h"
#include "typedef.h"
#include "uarthw.h"
#include "machine.h"
#include "TempCtrlComm.h"
#include "machine.h"
#include "hwmid.h"
#include "bsp_uart.h"
#include "bsp_led.h"

extern   MACHINE m_machine;

UI8 RcvBuf[RX_TLEN];
UI8 SendBuf[TX_TLEN];
UI8 RcvNum_Usart;
UI8 TX_LEN;
UI8 TX_CN;
UI16 Power_RX_CN = 0;
MODBUSTXBUF g_modbusTxBuf;
MODBUSTXDATA g_modbusTxData[MAXSENDCN];
UI16 CUREENT[6] = {0,0,0,0,0,0};
UI16 POWER[2]={0,0};
/* CRC 高位字节值表 */
static const unsigned char auchCRCHi[256] =
{
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

static const unsigned char auchCRCLo[256] =
{
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};



unsigned short GetCRC16(unsigned char *puchMsg, unsigned short usDataLen)
{


    unsigned char uchCRCHi = 0xFF;  /* 高CRC字节初始化 */
    unsigned char uchCRCLo = 0xFF;  /* 低CRC 字节初始化 */
    unsigned uIndex = 0; /* CRC循环中的索引 */
    while (usDataLen--) /* 传输消息缓冲区 */
    {
        uIndex = uchCRCHi ^ *puchMsg++;  /* 计算CRC */
        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
        uchCRCLo = auchCRCLo[uIndex];
    }
    return (unsigned short)((unsigned short)uchCRCHi << 8 | uchCRCLo);
}

void Rx_CanData(UI16 id,UI8* pdata,UI8 len)
{
	UI8 k;
	if(id == 0x331)
	{
		for( k=0; k<8; k++)
		{
			RcvBuf[Power_RX_CN++] = pdata[k];
			if(Power_RX_CN > RX_TLEN - 1)
				Power_RX_CN = 0;
			len--;
			if(len == 0)
				break;
		}
	}
}

void Power_transmit_buffer_write(UI8 num,UI8 func,UI16 addr,UI16 len)
{
	WORD j,i;    
	WORD last_tx_buf[6] = {0};
	UI8 SendBuf[8] = {0};
	UI16 crc;
	for(i=0;i<RX_TLEN;i++)
		RcvBuf[i] = 0;	
	
	SendBuf[0]=num;				//0x01;	
	SendBuf[1]=func;
	SendBuf[2]=(addr>>8)&0xff;	
	SendBuf[3]=addr&0xff;			//0x34;
	SendBuf[4]=(len>>8)&0xff;		//0x00;
	SendBuf[5]=len&0xff;			//0x02;
	crc=GetCRC16(SendBuf,6);
	SendBuf[6]=(UI8)(crc>>8);		
	SendBuf[7]=(UI8)(crc&0x00FF);

		
	last_tx_buf[0] = ((0x300 | (CAN_POEWR_KIND<<3)|1));
	last_tx_buf[1] = 4; 									 
	last_tx_buf[2] = ((SendBuf[1]<<8)|SendBuf[0]);
	last_tx_buf[3] = ((SendBuf[2]<<8)|SendBuf[3]);
	last_tx_buf[4] = ((SendBuf[4]<<8)|SendBuf[5]);
	last_tx_buf[5] = ((SendBuf[6]<<8)|SendBuf[7]);

	if(ChkSysCfg(CO43_CAN2EXT))
	{
        Can2TxBufWrite(last_tx_buf);
	}
	else
	{
		if ((g_cantxbuf.p_txbuf_end - g_cantxbuf.p_fill_txbuf) < 6)    
		{
			g_cantxbuf.p_fill_txbuf = g_cantxbuf.p_txbuf_start;
		}	
		for ( j = 0; j < 6; j++)									  //填数据到发送缓冲区
		{
			*g_cantxbuf.p_fill_txbuf = last_tx_buf[j];
			g_cantxbuf.p_fill_txbuf++;
		}
	}
	return; 				
}

void WriteData(UI16 txlen)
{
	m_uart_485_disable_tx();
	
	//由于M7的485在发送完最后一个字节后，需要将状态设置成接收状态。
	//但是因为设置"设置接收状态"的速度要比发送速度快，因此导致最后一次字节发不出去。
	//目前的处理是多发一个字节。
	TX_LEN=txlen + 1 ;
	TX_CN=1;
	CS_RS485_TXD
    m_uart_send_byte(RS485_UART,SendBuf[0]);
	m_uart_485_enable_tx();
}

void Send_3_Buf(UI8 num,UI8 func,UI16 addr,UI16 len)
{
	UI16 crc;
	UI8 i;
	for(i=0;i<RX_TLEN;i++)
		RcvBuf[i] = 0;	

	SendBuf[0]=num;				//0x01;	
	SendBuf[1]=func;
	SendBuf[2]=(addr>>8)&0xff;	//0x00;
	SendBuf[3]=addr&0xff;			//0x34;//能耗表的千瓦时地址
	SendBuf[4]=(len>>8)&0xff;		//0x00;
	SendBuf[5]=len&0xff;			//0x02;
	crc=GetCRC16(SendBuf,6);
	SendBuf[6]=(UI8)(crc>>8);		
	SendBuf[7]=(UI8)(crc&0x00FF);
	
	WriteData(8);	
}

void Send_ReqData(UI8 num,UI8 func,UI16 addr,UI16 len)
{
	if(ChkSysCfg(CO43_POWERCAN))
		Power_transmit_buffer_write(num,func,addr,len);  
	else
		Send_3_Buf(num,func,addr,len);	
}
void PowerRead_HAIXIN()
{
	static UI8  step = 1;
	static UI16 tm;
	UI32 temp_data = 0;
	UI16 crc;
	switch(step)
	{
		case 1:
				//读取A、B、C等三相电电压
				Send_ReqData(1,READ_HOLD_REG,0x00,0x0c);  
				tm=Get100MsTick();
				step++;
				Power_RX_CN=0;
		break;
		
		case 2:
			if((UI16)(Get100MsTick()-tm) > TM_MODBUSRD)	
			{
				crc =  (RcvBuf[3+0x0c*2+1]|RcvBuf[3+0x0c*2]<<8);
				if(crc == GetCRC16(RcvBuf,3+0x0c*2))
				{
					GetSys()->st->VOL_A = (RcvBuf[4]|RcvBuf[3]<<8);   //低16位
					GetSys()->st->VOL_B  = (RcvBuf[8]|RcvBuf[7]<<8);	  //高16位	
					GetSys()->st->VOL_C  = (RcvBuf[12]|RcvBuf[11]<<8);		//低16位
					GetSys()->st->VOL_AB  = (RcvBuf[16]|RcvBuf[15]<<8);	//高16位	
					GetSys()->st->VOL_BC  = (RcvBuf[20]|RcvBuf[19]<<8);	//低16位
					GetSys()->st->VOL_AC  = (RcvBuf[24]|RcvBuf[23]<<8);	//高16位	
				}
				step++;				
			}
			break;

		case 3:
				//读取A、B、C相电流
				Send_ReqData(1,READ_HOLD_REG,0x0C,0x06);  
				tm=Get100MsTick();
				step++;
				Power_RX_CN=0;		
			break;

		case 4:
			if((UI16)(Get100MsTick()-tm) > TM_MODBUSRD)	
			{
				crc =  (RcvBuf[3+0x6*2+1]|RcvBuf[3+0x6*2]<<8);
				if(crc == GetCRC16(RcvBuf,3+0x6*2))
				{
					GetSys()->st->CURRENT_A  = (RcvBuf[4]|RcvBuf[3]<<8);   //低16位
					GetSys()->st->CURRENT_B  = (RcvBuf[8]|RcvBuf[7]<<8);	  //高16位	
					GetSys()->st->CURRENT_C  = (RcvBuf[12]|RcvBuf[11]<<8);		//低16位
				}
				step++;				
			}		
			break;
		case 5:
				//读取总能耗
				Send_ReqData(1,READ_HOLD_REG,0x34,0x02);  
				tm=Get100MsTick();
				step++;
				Power_RX_CN=0;		
			break;

		case 6:
			if((UI16)(Get100MsTick()-tm) > TM_MODBUSRD)	
			{
				crc =  (RcvBuf[3+0x02*2+1]|RcvBuf[3+0x02*2]<<8);
				if(crc == GetCRC16(RcvBuf,3+0x02*2))
				{
					//能耗表是1位小数，但是面板需求是显示2位小数，因此这里乘以10
					temp_data = (RcvBuf[4]| (RcvBuf[3]<<8 )|(RcvBuf[6]<<16)|(RcvBuf[5]<<24))*10;
					GetSys()->st->ENERGY_TOTAL_L  =(temp_data &0xffff);//RcvBuf[4]|RcvBuf[3]<<8;   //低16位
					GetSys()->st->ENERGY_TOTAL_H = ((temp_data >>16 )&0xffff);	  //高16位	
				}
				step++;				
			}		
			break;	
		case 7:
			//读取总功率，单位是W
			Send_ReqData(1,READ_HOLD_REG,0x28,0x02);	
			tm=Get100MsTick();
			step++;
			Power_RX_CN=0;		
			break;

		case 8:
			if((UI16)(Get100MsTick()-tm) > TM_MODBUSRD)	
			{
				crc =  (RcvBuf[3+0x02*2+1]|RcvBuf[3+0x02*2]<<8);
				if(crc == GetCRC16(RcvBuf,3+0x02*2))
				{
					temp_data = (RcvBuf[4]| (RcvBuf[3]<<8 )|(RcvBuf[6]<<16)|(RcvBuf[5]<<24))/10;

					GetSys()->st->POWER_L = (temp_data &0xffff);// RcvBuf[4]|RcvBuf[3]<<8;   //低16位
					GetSys()->st->POWER_H = ((temp_data >>16 )&0xffff);;//RcvBuf[6]|RcvBuf[5]<<8;	  //高16位	
				}
				step++;				
			}		
			break;	
		case 9:
			//读取相线-零线电压
			Send_ReqData(1,READ_HOLD_REG,0x24,0x04);	
			tm=Get100MsTick();
			step++;
			Power_RX_CN=0;		
			break;
		case 10:
			if((UI16)(Get100MsTick()-tm) > TM_MODBUSRD)	
			{
				crc =  (RcvBuf[3+0x04*2+1]|RcvBuf[3+0x04*2]<<8);
				if(crc == GetCRC16(RcvBuf,3+0x04*2))
				{
					GetSys()->st->VOL_LN = (RcvBuf[4]|RcvBuf[3]<<8)*10;   //低16位
					GetSys()->st->VOL_LL = (RcvBuf[8]|RcvBuf[7]<<8)*10;	  //高16位	
				}
				step=1;				
			}		
			break;			
	}

}

void PowerRead_EST()
{
	static UI8  step = 1;
	static UI16 tm;
	UI32 temp_data = 0;
	UI16 crc;
	PMACHINE pm = &m_machine;

	switch(step)
	{
		case 1:
				//读取A、B、C等三相电电压和电流
				Send_ReqData(1,READ_HOLD_REG,0x00,0x06);  
				tm=Get100MsTick();
				step++;
				Power_RX_CN=0;
		break;
		
		case 2:
			if((UI16)(Get100MsTick()-tm) > TM_MODBUSRD)	
			{
				crc =  (RcvBuf[3+6*2+1]|RcvBuf[3+6*2]<<8);
				if(crc == GetCRC16(RcvBuf,3+6*2))
				{
					GetSys()->st->VOL_A = (RcvBuf[4]|RcvBuf[3]<<8);   //低16位
					GetSys()->st->VOL_B  = (RcvBuf[6]|RcvBuf[5]<<8);	  //高16位	
					GetSys()->st->VOL_C  = (RcvBuf[8]|RcvBuf[7]<<8);		//低16位
					GetSys()->st->CURRENT_A  = pm->md->CURRENT_RATIO*((RcvBuf[10]|RcvBuf[9]<<8));	//高16位	
					GetSys()->st->CURRENT_B  = pm->md->CURRENT_RATIO*((RcvBuf[12]|RcvBuf[11]<<8));	//低16位
					GetSys()->st->CURRENT_C  = pm->md->CURRENT_RATIO*((RcvBuf[14]|RcvBuf[13]<<8));	//高16位	
				}
				else
				{
					pm->st->VOL_A  = 0;   //低16位
					pm->st->VOL_B  = 0;	  //高16位	
					pm->st->VOL_C  = 0;		//低16位	
					pm->st->CURRENT_A  = 0;	//高16位	
					pm->st->CURRENT_B  = 0;	//低16位
					pm->st->CURRENT_C  = 0;	//高16位	
				}
				step++;		
			}
			break;

		case 3:
				//读取A、B、C相电压
				Send_ReqData(1,READ_HOLD_REG,0x17,0x03);  
				tm=Get100MsTick();
				step++;
				Power_RX_CN=0;		
			break;

		case 4:
			if((UI16)(Get100MsTick()-tm) > TM_MODBUSRD)	
			{	
				crc =  (RcvBuf[3+3*2+1]|RcvBuf[3+3*2]<<8);
				if(crc == GetCRC16(RcvBuf,3+3*2))
				{
					GetSys()->st->VOL_AB  = (RcvBuf[4]|RcvBuf[3]<<8);   //低16位
					GetSys()->st->VOL_BC  = (RcvBuf[6]|RcvBuf[5]<<8);	  //高16位	
					GetSys()->st->VOL_AC  = (RcvBuf[8]|RcvBuf[7]<<8);		//低16位
				}
				else
				{
					pm->st->VOL_AB  = 0;   		//低16位
					pm->st->VOL_BC  = 0;	  	//高16位	
					pm->st->VOL_AC  = 0;		//低16位					
				}
				step++;				
			}		
			break;
		case 5:
				//读取总能耗
				Send_ReqData(1,READ_HOLD_REG,0x1D,0x02);  
				tm=Get100MsTick();
				step++;
				Power_RX_CN=0;		
			break;
		case 6:
			if((UI16)(Get100MsTick()-tm) > TM_MODBUSRD)	
			{
				crc =  (RcvBuf[3+2*2+1]|RcvBuf[3+2*2]<<8);
				if(crc == GetCRC16(RcvBuf,3+2*2))
				{
					temp_data = pm->md->CURRENT_RATIO*(((RcvBuf[3]<<24|(RcvBuf[4]<<16)|(RcvBuf[5]<<8 )| RcvBuf[6])));
					GetSys()->st->ENERGY_TOTAL_L  =(temp_data &0xffff);//RcvBuf[4]|RcvBuf[3]<<8;   //低16位
					GetSys()->st->ENERGY_TOTAL_H = ((temp_data >>16 )&0xffff);	  //高16位	
				}
				else
				{
					pm->st->ENERGY_TOTAL_L  = 0;  //低16位
					pm->st->ENERGY_TOTAL_H  = 0;  //高16位	
				}
				step++;				
			}		
			break;	
		case 7:
			//读取总功率，单位是W
			Send_ReqData(1,READ_HOLD_REG,0x07,0x01);	
			tm=Get100MsTick();
			step++;
			Power_RX_CN=0;		
			break;

		case 8:
			if((UI16)(Get100MsTick()-tm) > TM_MODBUSRD)	
			{
				crc =  (RcvBuf[3+1*2+1]|RcvBuf[3+1*2]<<8);
				if(crc == GetCRC16(RcvBuf,3+1*2))
				{
					GetSys()->st->POWER_L = pm->md->CURRENT_RATIO*((RcvBuf[4]|RcvBuf[3]<<8));   //低16位
				}
				else
				{
					GetSys()->st->POWER_L = 0;   //低16位
				}		
				step = 1;	
			}		
			break;		
	}
}

void ModbuDownloadPro()
{
	PMACHINE pm = &m_machine;

	if(pm->md->POWER_CTRL == 0)
		return ;
	
	if(CUS_ID == CS_HAIXIN)
		PowerRead_HAIXIN();
	else
		PowerRead_EST();

}


