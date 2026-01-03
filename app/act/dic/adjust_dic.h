#ifndef _ADJUST_DIC_H_
#define _ADJUST_DIC_H_

#ifdef __cplusplus
extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"

#pragma pack(1)

typedef struct EN_ADJUST_PO
{
    UI16 O_ADJ_FWD ;                 //0x24100000  调模进
    UI16 O_ADJ_BSRV;                 //0x24100001  调模进备用
    UI16 O_ADJ_BWD ;                 //0x24100002  调模退
    UI16 O_ADJ_BSRV2;                //0x24100003  调模退备用
    UI16 O_ADJ_LUB ;                 //0x24100004  调模润滑
    UI16 O_ADJ_USE ;                 //0x24100005  调模使用   在调模或自动调模状态下，该点输出
}ADJUST_PO, *PADJUST_PO;

typedef struct EN_ADJUST_PI
{
    UI16 I_ADJ_FWD ;                 //0x24110000  调模进终
    UI16 I_ADJ_BWD ;                 //0x24110001  调模退终
    UI16 I_ADJ_CHK ;                 //0x24110002  调模电眼
    UI16 I_ADJ_LOK ;                 //0x24110003  调模锁定
}ADJUST_PI, *PADJUST_PI;

typedef struct EN_ADJUST_MACHSET
{
    /*斜率+液压动作前后延时+阀动作前后延时+其他机器参数*/
    UI16 DAFLOW_RAMP;                     /*  0x24120000 调模启动速度斜率       .01S  	范围0.00-1.99    	默认0.50*/
    UI16 DAPRES_RAMP;                     /*  0x24120001 调模启动压力斜率       .01S  	范围0.00-1.99     默认0.50*/
    UI16 DAFLOW_OFFRAMP;                  /*  0x24120002 调模结束速度斜率       .01S   	范围0.00-1.99		默认0.50*/
    UI16 DAPRES_OFFRAMP;                  /*  0x24120003 调模结束压力斜率       .01S    范围0.00-1.99		默认0.50*/
    UI16 TM_AUTOADJCLSPROTECTACT;         /*  0x24120004 自动调模关模低压计时    0.00      	范围0.00-99.99  	默认15.00   20200219*/
    UI16 CNPOSI_ADJPROPERTIONAL;          /*  0x24120005 每齿移动距离(0.01mm/CNT)	范围0.0-100.0				默认0.5  */
    UI16 ADPOSI_ADJCUR;                   /*  0x24120006 模板校正厚度   (0.01mm) 		范围100.0-999.9		默认100.0*/
    UI16 ADPOSI_ADJMIN;                   /*  0x24120007 机器最小模厚           范围最小模厚到模座最大电子尺位置    				默认100.0*/
    UI16 ADPOSI_ADJMAX;                   /*  0x24120008 最大模厚           范围最小模厚到模座最大电子尺位置					默认999.9    */
    UI16 ADJF_HYDELAY;					  /*  0x24120009 调模进液压延迟*/
    UI16 ADJB_HYDELAY;					  /*  0x2412000A 调模退液压延迟*/
    UI16 ADPOSI_ADJTARG;                  /*  0x2412000B 模具厚度(0.01mm)	范围100.0-999.9	默认250.0*/
    UI16 FL_QDC;                          /*  0x2412000C QDC功能使用 0不使用  1  使用   默认不使用*/
	UI16 FL_JOG_FUN;					  /*  0x2412000D 调模点动  0   不使用  1  使用   默认不使用*/
}ADJUST_MACHSET, *PADJUST_MACHSET;

typedef struct EN_ADJUST_ACTPARA
{
    UI16 DAPRES_ADJFFAST;                 /* 24140000 调模快速压力		范围0-系统压力上限	默认50		*/
    UI16 DAFLOW_ADJFFAST;                 /* 24140001 调模快速速度		范围0-调模速度上限	默认50		*/
    UI16 DAPRES_ADJSLOW;                  /* 24140002 调模慢速压力		范围0-系统压力上限	默认50		*/
    UI16 DAFLOW_ADJSLOW;                  /* 24140003 调模慢速速度		范围0-调模速度上限	默认20		*/
    UI16 DAPRES_ADJBWD;                   /* 24140004 调模退压力			范围0-系统压力上限	默认50	*/
    UI16 DAFLOW_ADJBWD;                   /* 24140005 调模退速度    		范围0-调模速度上限	默认50            */
    UI16 TM_ADJUSTSLOW;                   /* 24140006 松模单次调模计时	范围0.00-9.99	默认0.00			*/
    UI16 CN_ADJADV;                       /* 24140007 每次点动步数		范围0-3	默认0		*/
    UI16 CN_ADJFIRST;                     /* 24140008 每次点动计时		范围0.00-1.00	默认0.00	*/
    UI16 ADPOSI_ADJ;                      /* 24140009 调整位置   (0.01mm)	范围	100.0-999.9	默认100.0*/
    UI16 ADPOSI_ADJMD;                    /* 2414000A 调模模式		范围0-2	默认0			*/
    UI16 DAPRES_QDC;                      /* 2414000B QDC功能压力		默认0		*/
    UI16 DAFLOW_QDC;                      /* 2414000C QDC功能速度		默认0		*/
	UI16  PER_DAFLOW_ADJUST;			  /* 2414000D 调模速度百分比  范围 0-100  默认 0 */
}ADJUST_ACTPARA, *PADJUST_ACTPARA;

typedef struct EN_ADJUST_HYDR
{
    UI16 HYDR_PORT_ADJUST;				  // 24150000 调模动作的DA输出通道：位操作-->port1……port4
    UI16 HYDR_PUMPS_ADJUST;				  // 24150001 调模的泵浦限制:位操作-->P1……P7

    UI16 SERVO_KP_ADJUST;				  // 24150002 调模的PID_KP	默认800	范围0-2500
    UI16 SERVO_KI_ADJUST;				  // 24150003 调模的PID_KI	默认40	范围0-200
    UI16 SERVO_KD_ADJUST;				  // 24150004 调模的PID_KD
}ADJUST_HYDR, *PADJUST_HYDR;

typedef struct EN_ADJUST_STATE
{
    UI16 FL_ERROR0;						  // 24160000 警报1
    UI16 FL_ERROR1;						  // 24160001 警报2

    UI16 ADPOSI_ADJREAL;				  // 24160002 调模中的实时位置
    UI16 Step_ManualAdjFwd;               // 24160003 手动调模进实时动作步   成品画面显示   //20170301  let
    UI16 Step_ManualAdjBwd;               // 24160004 手动调模退实时动作步   成品画面显示
    UI16 Step_AUTOAdj;                    // 24160005 自动调模实时动作步   成品画面显示
}ADJUST_STATE, *PADJUST_STATE;
#pragma pack()

#ifdef DIC_ENUM_NEED

/*ACT SN start from 1*/
enum EN_ADJUST_ACT_SN
{
    OS_ADJF   = 1,          // 1 0x2411 手动调模进
    OS_ADJB, 			    // 2 0x2412 手动调模退

    OS_ADJAF  ,             // 3 0x2413 自动调模进
    OS_ADJAB,               // 4 0x2414 自动调模退

    OS_ADJA,			    // 5 0x2415 自动调模

    OS_ADJMOLDF,		    // 6 0x2416 松模调模进
    OS_ADJMOLDB,		    // 7 0x2417 松模调模退

    OS_ADJMOLD,			    // 8 0x2418 松模调模
    OS_ADAJMOLD,			// 9 0x2419 模厚调整

	OS_ADJPOSLEARN,			// 10 0x241A 电眼齿距学习

    ADJUST_ACT_SN_END
};

enum EN_ADJUST_ACTNAME
{
    /*手动调模进*/
    ADJFWDSLOW = STAGE_1_STEP,      //0x24111000 手动调模进慢
    ADJFWDFAST = STAGE_2_STEP ,		//0x24111100 手动调模进快

    /*手动调模退*/
    ADJBWDFAST = STAGE_1_STEP ,		//0x24121000 手动调模退快

    ADJASTART = STAGE_1_STEP,
    ADJCLSPRES = STAGE_2_STEP,      //0x24151100 自动调模压模
    ADJOPNSLOW = STAGE_3_STEP,      //0x24151200 自动调模开模
    ADJFWD       = STAGE_4_STEP,	//0x24151300 自动调模进
    ADJBWD       = STAGE_5_STEP,	//0x24151400 自动调模退
    ADJZX        = STAGE_6_STEP		//0x24151500 自动调模震雄调模
};


#define     ER0_FAAD      0x0001      //0x24100001   ; '完成自动调模  '  'FINISH AUTO ADJUST '
#define     ER0_ADJL      0x0002      //0x24100002   ; '已到达调模终点    '  'ADJUST END TOUCHED '
#define     ER0_ADJS      0x0004      //0x24100003   ; '调模电眼失败  '  'ADJUST SENSOR FAIL '
#define     ER0_ADJF      0x0008      //0x24100004   ; '已到达调模进终    '  'ADJUST FWD END TOUCHED '
#define     ER0_ADJB      0x0010      //0x24100005   ; '已到达调模退终    '  'ADJUST BWD END TOUCHED '

#define     ER0_ADJERRCN	0x0020		//0x24100006   ; '调模进退电眼次数有偏差'
#define     ER0_ADJCNL      0x0040      //0x24100007    ;   '完成齿距自学习'

#endif

#ifdef __cplusplus
}
#endif

#endif




