#ifndef _DA_DIC_H_
#define _DA_DIC_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"

#pragma pack(1)

#define	MAX_DA_NUM	16
#define MAX_E700B_CH 4 //E700B上最多2组模拟量，4个通道

typedef struct EN_DA_MACHSET
{
    UI16	WDA_MAX[MAX_DA_NUM];				//0x08120000 - 0x0812000F输出最大值
	UI16	WDA_ZERO[MAX_DA_NUM];				//0x08120010 - 0x0812001F输出归零值(最小值)
}DA_MACHSET, *PDA_MACHSET;

typedef struct EN_DA_STATE
{
	UI16  FL_ERROR0;							//0x08160000警报1
	UI16  FL_ERROR1;							//0x08160001警报2

	UI16  TEMPMAX[MAX_DA_NUM];					//0x08160002 -0x08160011 DA校准时使用的最大值。
	UI16  TEMPMIN[MAX_DA_NUM];					//0x08160012 - 0x08160021 DA校准时使用的最小值。
	UI16  DA_VAL1[MAX_DA_NUM];					//0x08160022 - 0x08160031 直接向硬件DA输出值

	UI16 WDA_STATE;						 		/* 0x08160032 DA测试状态/0-不使用;1-最小值 2-最大值*/
    UI16 WDA_CHANNEL;					 		/* 0x08160033 DA通道值   0-压力1;  1-速度1;	2-压力2;  3-速度2*/
	UI16 WDA_SETDATA;					 		/* 0x08160034 DA设置值					*/

    UI16  DA_E700B_VAL[MAX_E700B_CH];           //0x08160035 - 0x08160038 E700B压力流量指令 P1-F1-P2-F2
    UI16  DA_E700B_DIR[MAX_E700B_CH];           //0x08160039 - 0x0816003C E700B压力流量方向 0:+ 1:-
}DA_STATE, *PDA_STATE;

#pragma pack()

#ifdef DIC_ENUM_NEED


#define		ER0_DAPROTECT 			0x0001			//#00100 'DA输出过流保护(I1)'

#endif

#ifdef __cplusplus
}
#endif

#endif


