#ifndef _SERVOECAT_DIC_H_
#define _SERVOECAT_DIC_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"

#pragma pack(1)

typedef struct EN_SERVOECAT_STATE
{
	UI16  FL_ERROR0;								//0x0D160000	警报1
	UI16  FL_ERROR1;								//0x0D160001	警报2

	UI16  ETHERCATCOMDATASUM;						//0x0D160002	驱动器传输数据总数
	UI16  ETHERCATCOMDATANUM;						//0x0D160003	驱动器传输当前量
}SERVOECAT_STATE, *PSERVOECAT_STATE;


#pragma pack()

#ifdef DIC_ENUM_NEED

#define     ER0_SDOBETCHERR		0x0001      	//	SDO批量读取失败
#define	  	ER0_SDOSENDERR		0x0002			//	SDO发数据失败
#define	  	ER0_SDOREADERR		0X0004			//	SDO读数据失败
#define		ER0_SERVOERR		0x0008			//	伺服警报,请查看
#define		ER0_ECAT_CTOUT		0x0010			//	程序周期超时



/*ACT SN start from 1*/
enum EN_SERVOECAT_ACT_SN
{
	OS_SDOBATCHDATACOM = 1,						//批量读写驱动器数据
	OS_INITANGLE,								//学习电机角度
	SERVOECAT_ACT_SN_END
};

enum EN_SERVOECAT_ACTNAME
{
	//读数据
	SDOTX1 = STAGE_1_STEP,		
	SDOTX2 = STAGE_2_STEP ,		

	//写数据
	SDORX1 = STAGE_1_STEP ,		
	SDORX2 = STAGE_2_STEP
};


#endif

#ifdef __cplusplus
}
#endif

#endif
