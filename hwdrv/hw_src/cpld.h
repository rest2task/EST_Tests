/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    cpld.h

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:           2015.06.02

Description:   CPLD definition for A1000

Others:

History:    

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/
#ifndef     CPLD_H
#define     CPLD_H

#include "types.h"
#include "Hwcfg.h"
#include "fsl_common.h"

#define		CPLD_DAT_WIDTH16
//#define		CPLD_DAT_WIDTH8

#ifdef CPLD_DAT_WIDTH16
typedef	u16	cpld_data_t;
#else
typedef u8 cpld_data_t;
#endif

// CPLD Registers definition  以下驱动适用于IV5000及IV3200
#define CPLD_BASE				0xA0000000

#define AD_VAL		    (*(uint16_t *)(CPLD_BASE + 0x000000))									//ER-AD 16BIT DATA
#define AD_CR			(*(uint16_t *)(CPLD_BASE + 0x020000))           			//ER-AD CONTROL REG  
#define DA_VALUE		(*(uint16_t *)(CPLD_BASE + 0x040000))									//DA OUT 12BIT
#define CPLD_D8_BASE 0xA1000000

#define CPLD_D8_CMD_REG        	 (*(volatile uint8_t *)(CPLD_D8_BASE + (0x000000<<0)))
#define CPLD_D8_ADS8688_DL_REG   (*(volatile uint8_t *)(CPLD_D8_BASE + (0x0C0000<<0)))
#define CPLD_D8_ADS8688_DH_REG	 (*(volatile uint8_t *)(CPLD_D8_BASE + (0x0E0000<<0)))
                                                                    //  --- CHANNEL ---            DA OUT DATA
#define DA_CR		(*(uint16_t *)(CPLD_BASE + 0x060000))            				//DA OUT CONTROL REG 
                                                                    // D7    D6        D5   D4     D3   D2  D1   D0                                                                   // BUSY  CUT-PWM   ----------NULL---------   I-PROTECT
#define DA_PROTECT_STA  (1<<0)																			//DA过流保护位，1：正常，0：保护   仅适用于IV3200，其他机器无该功能

#define SEG_LED		    (*(uint16_t *)(CPLD_BASE + 0x0A0000)) 								//7-seg led   仅适用于IV5000 ,IV3200无该功能

#define T_VAL			(*(uint16_t *)(CPLD_BASE + 0x080000)) 								//温度数据寄存器    D15 D14 D13 D12    D11......D0																																		//                  -----NULL------    ----AD DATA------
#define T_CR			(*(uint16_t *)(CPLD_BASE + 0x0A0000))									//温度控制寄存器    D15 D14 D13....D8        D7       D6 D5 D4 D3      D2  D1 D0
																																		//                  ------NULL--------  状态启动位    ------NULL----     通道
																																		
#define port4000		(*(uint16_t *)(CPLD_BASE + 0x100000))								//I/O output low-16bit    
#define port4001		(*(uint16_t *)(CPLD_BASE + 0x120000))           		//I/O output 17_32
#define port4002		(*(uint16_t *)(CPLD_BASE + 0x140000))								//IV5000：I/O output 33_48;   IV3200：继电器输出1——16
#define port4003		(*(uint16_t *)(CPLD_BASE + 0x160000))           		//IV5000：I/O output 49_64;   IV3200：无
                                                                    

#define port3000		(*(uint16_t *)(CPLD_BASE + 0x180000))								//I/O input low-16bit
#define port3001		(*(uint16_t *)(CPLD_BASE + 0x1A0000))           		//I/O input 17_32
#define port3002		(*(uint16_t *)(CPLD_BASE + 0x1C0000))								//IV5000：I/O input 33_48;    IV3200：无
#define port3003		(*(uint16_t *)(CPLD_BASE + 0x1E0000))           		//IV5000：无         IV3200：无

#define GPIO_IO_PRO     (*(uint16_t *)(CPLD_BASE + 0x200000))               //IO output pro state
#define HVTEST_STA   		(1<<2)																			//H24V状态监测
#define OUTTEST_STA  		(1<<1)																			//IO输出过流保护状态监测
#define OUTRESET_STA 		(1<<0)	
#define OUTCARD_STA			(1<<3)
#define PCB_TYPE			(0xF<<12)
#define LM73_POWER      (*(uint16_t *)(CPLD_BASE + 0x220000))

//===================================================================================
//CPLD Access Type

#define CPLD_OP_READ			(0 << 4)	//CPLD Read Data From Device
#define CPLD_OP_WRITE			(1 << 4)	//CPLD Write Data To Device
#define CPLD_OP_VALID			(1 << 5)	//CPLD new Operation valid

#define RANGE0					0x0	//default  value,	+/-2.5xVREF
#define RANGE1					0x1	//+/-1.25xVREF
#define RANGE2					0x2	//+/-0.625xVREF
#define RANGE5					0x5	//0--2.5xVREF
#define RANGE6					0x6	//0--1.25xVREF

#define MAX_MAXADPORT		16
#define Y209_AD_CHANEL		8

#define DA_Q_LEN  	16

typedef struct tyDA_HW_Q
{
	WORD	tail;
	WORD	head;
	WORD	boff;
	WORD	hw_q[DA_Q_LEN];
	WORD	da_val[HW_DA_PORT_NUM + 1];
}DA_HW_Q;

extern DA_HW_Q da_q;


void AD_start_HAL(void);
void EXT_AD_start_HAL(void);
void AD_Req(u16 ch);
void EXT_AD_Req(u16 ch);
WORD GetADHW(int portno);
WORD GetADHW_F508(int portno);
void Start_DA_HAL(uint8_t sel, uint16_t val);
BOOL da_protect_HAL(void);
void IOVCheck_HAL(void);
UI16 GetHwTypeInfo_HAL(void);
void cpld_acc_wait(void);
void diable_ext_daccurrent_out_HAL(void);
void enable_ext_daccurrent_out_HAL(void);
BOOL cpld_card_test_HAL(void);
void io_reset(void);
void change_channels(uint8_t sel);
uint16_t Get_AD_Temp(void);
void SetDAHW(int portno, WORD value);  
void LM73_POWERON(void);
void LM73_POWEROFF(void);
uint16_t get_pcb_type(void);


extern WORD ad_main_start;
extern WORD ext_main_start;
extern WORD ad_channel;
extern WORD ext_ad_channel;
extern volatile WORD	AD_BUFFER[MAX_MAXADPORT];                         
extern volatile WORD	ext_AD_BUFFER[Y209_AD_CHANEL];                         
extern UI8	ad_sensor_range[MAX_MAXADPORT] ;   
extern UI16 	ad_sensor_press;
extern WORD 	ext_ad_channel_used;	//按位操作 bit0 为第一路

#endif
