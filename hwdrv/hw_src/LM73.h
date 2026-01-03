/*********************************************************************************
** 文件名称 ： I2CINT.h
** 功能说明 ： LPC17系列芯片硬件I2C软件包。
** 使用说明 ： 主程序要配置好I2C总线接口(I2C引脚功能和I2C中断，并已使能I2C主模式)
**********************************************************************************/
#ifndef __LM73_H
#define __LM73_H
#include    "common.h"
typedef struct tag_LM73
{
	uint8 Initstep;					/*初始化步骤*/
	WORD	waittime;					/*每一个步骤的间隔时间*/
	uint8_t   failcount;				    /*初始化失败次数*/
	uint8_t		read_buf[2];				/*接收数据*/
	BOOL		TEMP_NEW;					/*数据更新标志*/
	BOOL		WRITE_SUCCESS;				/*寄存器配置完成标志*/
}LM73_STRU;

extern	LM73_STRU		  g_lm73; 			//用于LM73初始化 zf 20160927

void I2C_STOPBit(void);
void Start_ReadLM73(void);
WORD ReadValid_LM73(void);
BOOL Get_ValueLM73_HAL(void);
void LM73Config(void);
void LM73WriteReset(void);
void LM73WriteConfig(void);
void LM73WriteControl(void);
void LM73WriteAddr(void);
WORD ReadI2C2(void);


#endif
