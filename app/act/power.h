#ifndef _MODBUS_H_
#define _MODBUS_H_

#include "typedef.h"
#define 	TX_TLEN 30
#define 	RX_TLEN 30
#define		TM_MODBUSRD 10
#define	 	MAXSENDCN	10
extern  UI8 RcvBuf[RX_TLEN];
extern  UI8 SendBuf[TX_TLEN];
extern  UI8 RcvNum_Usart;
extern UI8 TX_LEN;
extern UI8 TX_CN;
extern UI16 Power_RX_CN;

typedef enum enFunctionName	
{
	READ_COIL=0x01,				// 1 读线圈
	READ_IOGENERIC,				// 2 读离散量输入
	READ_HOLD_REG,				// 3 读保持寄存器
	READ_INPUT_REG,				// 4 读输入寄存器
	WRITE_SINGLE_COIL,			// 5 写单个线圈
	WRITE_SINGLE_REG,			// 6 写单个寄存器
	
	WRITE_MULTI_COIL=0x0F,		// 15 写多个线圈
	WRITE_MULTI_REG,			// 16 写多个寄存器

	READ_FILERECORD=0x14,		// 20 读文件记录
	WRITE_FILERECORD,			// 21 写文件记录
	SHILED_WRITEREG,			// 22 屏蔽写寄存器
	READ_WRITE_MULTI_REG,		// 23 读/写多个寄存

	READ_HEADINGCODE=0x2B,		// 43读取设备识别码
}FunctionName;

typedef struct tag_ModbusTxBuf
{
	BYTE    htask;
    BYTE    htasktail;
}MODBUSTXBUF;

typedef struct tag_ModbusTxData
{
	UI8 	id;		//从站号
	UI8 	fun;	//功能码
	UI16 	addr;	//寄存器地址
	UI16    count;	//寄存器数量
	UI16*	datarx;	//回复的数据
	UI16	rxlen;	//接收数据的长度
}MODBUSTXDATA;

void ModbuDownloadPro(void);
void Rx_CanData(UI16 id,UI8* pdata,UI8 len);
void PowerRead_EST(void);

#endif

