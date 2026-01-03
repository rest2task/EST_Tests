#ifndef _SYSACC_DIC_H_
#define _SYSACC_DIC_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"

#pragma pack(1)

/*----------浮动缸部件----------*/
typedef struct EN_SYSACC_PO
{
    UI16  O_SYS_ACC ;                 //0x25100000 系统蓄压
    UI16  O_SYS_FDG ;                 //0x25100001 浮动缸     //20180409 LET
    UI16  O_SYS_CHN;                  //0x25100002 浮动缸储能  //20190403 CHJ 补齐
    UI16  O_SYS_XY;                   //0x25100003 浮动缸泄压
}SYSACC_PO, *PSYSACC_PO;

typedef struct EN_SYSACC_PI
{
	UI16  I_SYS_ACC ;                 //0x25110000 系统蓄压检知
	UI16  I_SYS_FDG ;                 //0x25110001 浮动缸 检知     //20180409 LET 
}SYSACC_PI, *PSYSACC_PI;

typedef struct EN_SYSACC_ACTPARA
{
	UI16  DAPRES_SYSACC;                 /*0x25140000  浮动缸压力    范围0-系统压力上限   默认30*/
    UI16  DAFLOW_SYSACC;                 /*0x25140001  浮动缸速度    范围0-99             默认30*/
}SYSACC_ACTPARA, *PSYSACC_ACTPARA;

typedef struct EN_SYSACC_HYDR
{
    UI16  HYDR_PORT_SYSACC;				//0x25150000 浮动缸DA输出通道：位操作-->port1……port4
    UI16  HYDR_PUMPS_SYSACC;			//0x25150000 浮动缸比例速度限制:位操作-->P1……P7
}SYSACC_HYDR, *PSYSACC_HYDR;

typedef struct EN_SYSACC_STATE
{
    UI16  FL_ERROR0;					//0x25160000 警报1
    UI16  FL_ERROR1;					//0x25160001 警报2 
}SYSACC_STATE, *PSYSACC_STATE;


#pragma pack()

#ifdef DIC_ENUM_NEED

enum EN_SYSACC_ACT_SN
{
	OS_SYSACC = 1,

	SYSACC_ACT_SN_END
};

#endif


#ifdef __cplusplus
}
#endif

#endif

