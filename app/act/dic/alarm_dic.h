#ifndef _ALARM_DIC_H_
#define _ALARM_DIC_H_

#ifdef __cplusplus
extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"

#pragma pack(1)

typedef struct EN_ALARM_PO
{
	UI16  O_ALARM   ;                 //0x0B100000  ;警报         ; 
	UI16  O_FLASH   ;                 //0x0B100001  ;闪光         ; 

	UI16  O_OK_LIT  ;                 //0x0B100002  ;正常灯   	    ; 
	UI16  O_ERR_LIT ;                 //0x0B100003  ;不正常灯     	;
	UI16  O_EMG_LIT ;                 //0x0B100004  ;紧急停止灯   	;
	UI16  O_AUT_LIT ;                 //0x0B100005  ;自动灯    	;
	UI16  O_MAN_LIT;                  //0x0B100006  ;手动灯       	;
	UI16  O_ALM_SRV;                  //0x0B100007	;警报备用
}ALARM_PO, *PALARM_PO;

typedef struct EN_ALARM_PI
{
	UI16  I_EMG_EXT;                 //0x0B200000  ;外部急停键         ;
	UI16  I_KEY_RUN;				 //0x0B200001  ;外部运行键         ;
}ALARM_PI, *PALARM_PI;


typedef struct EN_ALARM_MACHSET
{
    UI16  CN_BUZZERACT;                 // 0x0B120000 警报次数到关马达	范围0-99	默认30
    UI16  TM_HEATEROFFIFALARM;          // 0x0B120001 警报时间到关电热	范围0.00-600.00	 默认5.00  20200219
    UI16  FL_ALARM;                     // 0x0B120002 警报功能	默认0
    UI16  FL_AlARMONMANL;               // 0x0B120003 手动状态下开蜂鸣器			  0不开   1开	默认不开
    UI16  FL_ALARMCNT;					// 0x0B120004 临时警报次数  	范围1-99     默认5次
}ALARM_MACHSET, *PALARM_MACHSET;

typedef struct EN_ALARM_STATE
{
    UI16  FL_ERROR0;                    //0x0B160000 警报1
    UI16  FL_ERROR1;                    //0x0B160001 警报2

	UI16  AUTOALARM_SHORTNUM;			//0x0B160002 自动警报开启剩余模数
}ALARM_STATE, *PALARM_STATE;

#pragma pack()
#ifdef DIC_ENUM_NEED

#endif
#define     ER0_PNLS_EXT        0x0001      // #0B100 '外部急停按钮已按下'

#ifdef __cplusplus
}
#endif

#endif
