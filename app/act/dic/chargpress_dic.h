#ifndef _CHARGPRESS_DIC_H_
#define _CHARGPRESS_DIC_H_

#ifdef __cplusplus
extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"

#pragma pack(1)
#define INJECT_TEMP_CH_NUM	14

typedef struct EN_CHGPRS_PO
{
    UI16  O_PRS_FWD;                  //0x13100000  下压缸进      07-7-27
    UI16  O_PRS_BWD;                  //0x13100001  下压缸退
    UI16  O_PRS_SRV;                  //0x13100002	下压缸备用
}CHGPRS_PO, *PCHGPRS_PO;

typedef struct EN_CHGPRS_PI
{
    UI16  I_PRS_FWD;                  //0x13110000  ;缺料极限报警
    UI16  I_PRS_BWD;                  //0x13110001  ;下压缸退终
    UI16  I_PRS_LVL;                  //0x13110002  ;缺料警报
    UI16  I_PRS_SRV;                  //0x13110003	;下压缸备用
}CHGPRS_PI, *PCHGPRS_PI;

typedef struct EN_CHGPRS_ACTPARA
{
    UI16  DAPRES_CHGPRSFWD;                     /* 0x13140000 下压缸进压力   		范围0-小动作压力上限	默认30            */
    UI16  DAFLOW_CHGPRSFWD;                     /* 0x13140001 下压缸进速度   		范围0-99		默认30  */
    UI16  DAPRES_CHGPRSBWD;                     /* 0x13140002 下压缸退压力   		范围0-小动作压力上限	默认30             */
    UI16  DAFLOW_CHGPRSBWD;                     /* 0x13140003 下压缸退速度   		范围0-99		默认30  */
    UI16  DAPRES_CHGPRS_SETUP;                  /* 0x13140004 下压缸动作慢速调试压力  默认30*/
    UI16  DAFLOW_CHGPRS_SETUP;                  /* 0x13140005 下压缸动作慢速调试速度  默认30*/
    UI16  DAPRES_CHGPRSRAMP;					/* 0x13140006 下压缸启动压力斜率    .01S    默认0.50*/
    UI16  DAFLOW_CHGPRSRAMP;					/* 0x13140007 下压缸启动速度斜率    .01S    默认0.50*/
    UI16  DAPRES_CHGPRSOFFRAMP;					/* 0x13140008 下压缸关闭压力斜率    .01S    默认0.00*/
    UI16  DAFLOW_CHGPRSOFFRAMP;					/* 0x13140009 下压缸关闭速度斜率    .01S    默认0.00*/
}CHGPRS_ACTPARA, *PCHGPRS_ACTPARA;

typedef struct EN_CHGPRS_HYDR
{
    /*+++++++++HYDR(PORT;PUMPS)+++++++++*/
    UI16  HYDR_PORT_CHGPRS;						//0x13150000 下压缸的DA输出通道：位操作-->port1……port4
    UI16  HYDR_PUMPS_CHGPRS;					//0x13150001 下压缸动作的泵浦限制:位操作-->P1……P7
    UI16  SERVO_KP_CHGPRS;						//0x13150002 下压缸动作的PID_KP
    UI16  SERVO_KI_CHGPRS;						//0x13150003 下压缸动作的PID_KI
    UI16  SERVO_KD_CHGPRS;						//0x13150004 下压缸动作的PID_KD
}CHGPRS_HYDR, *PCHGPRS_HYDR;

typedef struct EN_CHGPRS_STATE
{
    UI16 FL_ERROR0;								//0x13160000警报1
    UI16 FL_ERROR1;								//0x13160001 警报2
    UI16 Step_ChargeUp;                         //0x13160002 下压缸上实时动作步  成品画面显示
    UI16 Step_ChargeDown;                       //0x13160003 下压缸下实时动作步  成品画面显示
}CHGPRS_STATE, *PCHGPRS_STATE;

#pragma pack()

#ifdef DIC_ENUM_NEED

/*ACT SN start from 1*/
enum EN_CHGPRS_ACT_SN
{
	OS_CHGPRSF = 1, 			// 1311   下压缸进 
	OS_CHGPRSB, 				// 1312   下压缸退

	CHGPRS_ACT_SN_END
};

#define     ER0_CHGPRSBWD      0x0001        // #13100  	压料缸退到位

#endif

#ifdef __cplusplus
}
#endif

#endif


