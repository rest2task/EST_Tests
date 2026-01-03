#ifndef _LUB_DIC_H_
#define _LUB_DIC_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"

#pragma pack(1)

typedef struct EN_LUB_PO
{
    UI16  O_LUBRCTR ;                 //0x30100000 润滑          //润滑2          //润滑3
    UI16  O_LUBSRV;                   //0x30100001 润滑备用        //润滑2备用        //润滑3备用
}LUB_PO, *PLUB_PO;

typedef struct EN_LUB_PI
{
    UI16  I_LUB_LVL ;                 //0x30110000 润滑油位检知 //润滑2油位检知 //润滑3油位检知
    UI16  I_LUB_CHK ;                 //0x30110001 润滑检知         //润滑2检知         //润滑3检知
    UI16  I_LUB_SRV;                  //0x30110002 润滑备用         //润滑2备用         //润滑3备用
}LUB_PI, *PLUB_PI;

typedef struct EN_LUB_MACHSET
{
    /*斜率+液压动作前后延时+阀动作前后延时+其他机器参数*/

    UI16  FL_LUBMODE;						// 0x30120000 润滑模式           范围0-2         默认0
    UI16  TM_LUBDLY;						// 0x30120001 自动润滑延迟         范围0.00-99.99    默认0.00s   20200219
    UI16  TM_LUBTOTAL;						// 0x30120002 自动润滑总计时 范围0.00-600.00 默认10.00s	20200219
    UI16  TM_LUBACT;						// 0x30120003 自动润滑计时         范围0.00-600.00 默认10.00s	20200219
    UI16  CN_LUBUNIT;						// 0x30120004 自动润滑模数         范围0-10000     默认300
    UI16  TM_LUBFIRST;						// 0x30120005 首次润滑时间  默认150.00s  范围0.00-600.00 20200219*/
    UI16  TM_SWITCHCHECK;					// 0x30120006 润滑检知检测时间 默认300.00 范围0.00-600.00  20200219
    UI16  CN_LUB3;							// 0x30120007 润滑次数	默认1			范围3-10
    UI16  TM_LUB3;							// 0x30120008 润滑时间 默认10			范围1-600
    UI16  TM_LUBDELAY3;						// 0x30120009 润滑间歇 默认0			范围1-600
    UI16  TM_LUBFORCE;                      // 0x3012000A 润滑延迟断开时间     	    默认10s      范围10-15s
}LUB_MACHSET, *PLUB_MACHSET;

typedef struct EN_LUB_STATE
{
    UI16  FL_ERROR0;						//0x30160000 警报1
    UI16  FL_ERROR1;						//0x30160001 警报2
    UI16  FL_MOLDLUBRIC;					//0x30160002 润滑状态
    UI16  AUTOLUB_NEED;                     //0x30160003 本循环是否需要启动润滑
    UI16  Step_Lub;							//0x30160004 润滑实时动作步    成品画面显示
    UI16  LUB_CLMP_LEFT;					//0x30160005 开润滑剩余模数	
    UI16  CN_CLAMPLUBRIC;					//0x30160006 自动润滑模数
}LUB_STATE, *PLUB_STATE;

#pragma pack()

#ifdef DIC_ENUM_NEED


/*ACT SN start from 1*/
enum EN_LUB_ACT_SN
{
    OS_LUBR   = 1,     	 //润滑 //润滑2 //润滑3

    LUB_ACT_SN_END
};

#define     ER1_LUBL      0x0001			//#30100 '润滑1 油位低'
#define     ER1_LUBR      0x0002			//#30101 '润滑1 压力未到'         
#define     ER1_LUBERR    0x0004			//#30102 '润滑1 压力开关故障' 
//#30200 润滑2 '油位低'
//#30201 润滑2 '压力未到'         
//#30202 润滑2 '压力开关故障'
//#30300 润滑3 '油位低'
//#30301 润滑3 '压力未到'         
//#30302 润滑3 '压力开关故障'
#endif

#ifdef __cplusplus
}
#endif

#endif




