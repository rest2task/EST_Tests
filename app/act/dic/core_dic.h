#ifndef _CORE_DIC_H_
#define _CORE_DIC_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"

#pragma pack(1)
typedef struct EN_CORE_PO
{
	UI16  O_CP_FWD;					//0x22100000 中子A进
	UI16  O_CP_FSET;				//0x22100001 中子A进装模
	UI16  O_CP_FRSV;				//0x22100002 中子A进备用
	UI16  O_CP_BWD;					//0x22100003 中子A退
	UI16  O_CP_BSET;				//0x22100004 中子A退装模
	UI16  O_CP_BRSV;				//0x22100005 中子A退备用
}CORE_PO, *PCORE_PO;

typedef struct EN_CORE_PI
{
	UI16  I_CORE_STA;                //0x22110000  ;中子A启动
	UI16  I_CP_FWD ;                 //0x22110001  ;中子A进终	 	; CORE PULL #1 FWD SENSOR
	UI16  I_CP_BWD ;                 //0x22110002  ;中子A退终   	; CORE PULL #1 BWD SENSOR
	UI16  I_CP_ACT ;                 //0x22110003  ;中子A     	; CORE PULLING #1 POSITION
	UI16  I_SC_CHK ;                 //0x22110004  ;绞牙A电眼   	; MOLD SCREW #1 CHECK SENSOR
	UI16  I_CP_FWD2;                 //0x22110005  ;中子A进终2  	; 眼睛机专用    05-11-21
	UI16  I_CP_RLS;                  //0x22110006  ;中子A泄压
}CORE_PI, *PCORE_PI;

typedef struct EN_CORE_MACHSET
{
	/*斜率+液压动作前后延时+阀动作前后延时+其他机器参数*/
    UI16  DAPRES_CORERAMP;                      /* 0x22120000 中子启动压力斜率           .01S    默认0.50		范围0.00-1.99*/
    UI16  DAFLOW_CORERAMP;                      /* 0x22120001 中子启动速度斜率           .01S    默认0.50		范围0.00-1.99*/
    UI16  DAPRES_COREOFFRAMP;                   /* 0x22120002 中子关闭压力斜率           .01S    默认0.00		范围0.00-1.99*/
    UI16  DAFLOW_COREOFFRAMP;                   /* 0x22120003 中子关闭速度斜率           .01S    默认0.00		范围0.00-1.99*/
    UI16  DAPRES_CORESETUP;                     /* 0x22120004 中子慢速调试压力               默认 30 		范围0-小动作压力上限*/
    UI16  DAFLOW_CORESETUP;                     /* 0x22120005 中子慢速调试速度               默认 30 		范围0-99*/
		
    UI16  TM_COREINCTDLY;                       /* 0x22120006 中子进延迟计时         chg by rch 12-12-26   默认0.30	范围0.00-99.99   20200219 */
    UI16  TM_COREOUTCTDLY;                      /* 0x22120007 中子退延迟计时         chg by rch 12-12-26   默认0.30	范围0.00-99.99   20200219 */
    UI16  TM_COREENDDLY;                        /* 0x22120008 中子动作结束延时 			默认0.30		范围0.00-0.99    */
    UI16  ADPOSI_EFFCPR2;                       /* 0x22120009 途中中子位置有效区         */
    UI16  TM_MIDDLECOREDLY;                     /* 0x2212000A 中途中子动作延迟           */
    UI16  TM_CPPUMP_DLY;                        /* 0x2212000B 特殊中子阀延迟关计时  0.01s  chg by rch 2012-12-26*/

    UI16   COREF_HYDELAY;                       /*0x2212000C  中子进液压延迟*/
    UI16   COREB_HYDELAY;                       /*0x2212000D  中子退液压延迟*/
    UI16   TMDLY_VLV_ONCOREF;                   /*0x2212000E  中子进阀开延迟			默认0.00		范围0.00-5.00*/
    UI16   TMDLY_VLV_OFFCOREF;                  /*0x2212000F  中子进阀关延迟			默认0.00		范围0.00-5.00*/
    UI16   TMDLY_VLV_ONCOREB;                   /*0x22120010  中子退阀开延迟			默认0.00		范围0.00-5.00*/
    UI16   TMDLY_VLV_OFFCOREB;                  /*0x22120011  中子退阀关延迟			默认0.00		范围0.00-5.00*/
	
	UI16  TM_COREINENDDLY;                      /*0x22120012  中子进动作结束延时  默认0.30	范围0.00-9.99*/
	UI16  TM_COREOUTENDDLY;                     /*0x22120013  中子退动作结束延时  默认0.30	范围0.00-9.99*/

	/*以下四个位置有效区由面板根据设定的中子位置有效区进行计算后下发，主要是为了解决使用十字头的问题*/
	UI16   ADPOSI_EFFCOREINPLUS;                /*0x22120014 中子进位置有效区(+)     默认50.0   范围0-开模五段终止位置 (推力座转换)*/
	UI16   ADPOSI_EFFCOREINMINUS;               /*0x22120015 中子进位置有效区(-)     默认50.0   范围0-开模五段终止位置 (推力座转换)*/
	UI16   ADPOSI_EFFCOREOUTPLUS;               /*0x22120016 中子退位置有效区(+)     默认50.0   范围0-开模五段终止位置 (推力座转换)*/
	UI16   ADPOSI_EFFCOREOUTMINUS;              /*0x22120017 中子退位置有效区(-)     默认50.0   范围0-开模五段终止位置 (推力座转换)*/
}CORE_MACHSET, *PCORE_MACHSET;

typedef struct EN_CORE_MOLDSET
{
	/*功能+警报参数*/
    UI16  FL_COREPROTECT;                       /* 0x22130000 中子保护功能  		0：不使用	1：使用		默认不使用     */
    UI16  FL_CORE_CTRL;                         /* 0x22130001 中子功能选择 (0=不用 1=中子 2=绞牙) */
    UI16  FL_CORE_TRAVEL;                       /* 0x22130002 中子进控制方式 0=行程 1=时间 */
    UI16  FL_CORE_OUTTRAVEL;                    /* 0x22130003 中子退控制方式 0=行程 1=时间 */
    UI16  FL_SPECIALCODE;                       /* 0x22130004 特殊中子功能, 0 = 不使用， 1 - 6*/
    UI16  FL_INJ_FWDOFF;                        /* 0x22130005 射出时中子（A.B...F）进保持: 0:开，1: 不开*/
    UI16  ADPOSI_CORE_FWD;                      /* 0x22130006 中子进动作点 默认0 0-关始 1-中途 2-关终*/ 
    UI16  ADPOSI_CORE_BWD;                      /* 0x22130007 中子退动作点 默认0 0-开终 1-中途 2-开始*/
    UI16  PRI_CORE_FWD;                         /* 0x22130008 中子进优先级 范围1-6 默认值A-1，B-2，C-3，D-4，E-5，F-6*/
    UI16  PRI_CORE_BWD;                         /* 0x22130009 中子退优先级 范围1-6 默认值A-6，B-5，C-4，D-3，E-2，F-1*/
	UI16  CORE_LIMIT_CHECK;                     /* 0x2213000A 进退限位器检查 默认1 0-不使用 1-使用*/
    UI16  CLAMP_CORE_CHECK;                     /* 0x2213000B 开关模全程检测中子 默认1 0-不使用 1-使用*/
    UI16  TM_HOLD_CORE_FWD;                     /* 0x2213000C 中子进保持计时 范围0.00-99.99s 默认0.00S */
    UI16  TM_HOLD_CORE_BWD;                     /* 0x2213000D 中子退保持计时 范围0.00-99.99s 默认0.00S */
}CORE_MOLDSET, *PCORE_MOLDSET;

typedef struct EN_CORE_ACTPARA
{
	UI16  DAPRES_COREIN;                       /* 0x22140000 中子a进压力   		默认50	范围0-小动作压力上限             */
	UI16  DAPRES_COREOUT;                      /* 0x22140001 中子a退压力   		默认50	范围0-小动作压力上限             */

    UI16  DAFLOW_COREIN;                       /* 0x22140002 中子a进速度     	默认50	范围0-99           */
    UI16  DAFLOW_COREOUT;                      /* 0x22140003 中子a退速度		    默认45	范围0-99            */

	UI16  TM_COREIN;                           /* 0x22140004 中子a进计时   		默认3.00	范围0.00-99.99           20200219 */
	UI16  TM_COREOUT;                          /* 0x22140005 中子a退计时    		默认3.00	范围0.00-99.99           20200219  */

	UI16  CN_SCREWIN;                          /* 0x22140006 绞牙a进计数		   	默认0		范围0-9999             */
	UI16  CN_SCREWOUT;                         /* 0x22140007 绞牙a退计数     	默认0		范围0-9999              */

	UI16   ADPOSI_COREINACT;                   /* 0x22140008 中子a进动作位置			默认0.0	      */
	UI16  ADPOSI_COREOUTACT;                   /* 0x22140009 中子a退动作位置			默认0.0	      */

	UI16   DAPRES_CORESLOW;                    /* 0x2214000A 中子慢速调试压力		   	默认30		范围0-小动作压力上限          */
    UI16   DAFLOW_CORESLOW;                    /* 0x2214000B 中子慢速调试速度   		默认0		范围0-99         */

	UI16   PER_DAFLOW_CORE;                    /* 0x2214000C  中子速度百分比  范围0-100  默认0   */

	UI16   DAFLOW_CLSSTARTRAMP_CORE;			/* 0x2214000D	中子前关模停止斜率	默认0.50s	范围:0~1.99s*/
	UI16   DAFLOW_CLSENDRAMP_CORE;				/* 0x2214000E	中子后关模启动斜率	默认0.50s	范围:0~1.99s*/

	UI16   DAFLOW_OPNSTARTRAMP_CORE;			/* 0x2214000F	中子前开模停止斜率	默认0.50s	范围:0~1.99s*/
	UI16   DAFLOW_OPNENDRAMP_CORE;				/* 0x22140010	中子后开模启动斜率	默认0.50s	范围:0~1.99s*/
}CORE_ACTPARA, *PCORE_ACTPARA;

typedef struct EN_CORE_HYDR
{
	UI16  HYDR_PORT_CORE;					   //0x22150000 中子的DA输出通道：位操作-->port1……port4
    UI16  HYDR_PUMPS_CORE;					   //0x22150001 中子动作的泵浦限制:位操作-->P1……P7
	UI16  SERVO_KP_CORE;					   //0x22150002 中子动作的PID_KP			默认800		范围0-2500
	UI16  SERVO_KI_CORE;					   //0x22150003 中子动作的PID_KI			默认40		范围0-200
	UI16  SERVO_KD_CORE;					   //0x22150004 中子动作的PID_KD
}CORE_HYDR, *PCORE_HYDR;

typedef struct EN_CORE_STATE
{
	UI16  FL_ERROR0;						   //0x22160000 警报1
	UI16  FL_ERROR1;						   //0x22160001 警报2
    UI16  Step_CoreFwd;                        //0x22160002 中子进动作步显示    成品画面显示
    UI16  Step_CoreBwd;                        //0x22160003 中子退动作步显示    成品画面显示
    UI16  COREFWD_STATUS;                      //0x22160004 中子进终信号  0无效 1有效   中子画面显示
    UI16  COREBWD_STATUS;                      //0x22160005 中子退终信号  0无效 1有效   中子画面显示
    UI16  CORE_STATUS;                         //0x22160006 中子实际状态  0中间 1进终 2退终
}CORE_STATE, *PCORE_STATE;

#pragma pack()

#ifdef DIC_ENUM_NEED
/*ACT SN start from 1*/
enum EN_CORE_ACT_SN
{
    OS_CPRF  = 1,               // 2211  ; '中子进  '
    OS_CPRB ,                   // 2212  ; '中子退  '

    OS_CPRAF,                   // 2213, 调模中子进
    OS_CPRAB,                   // 2214, 调模中子退

    OS_CPRLS,                   // 2215, 中子泄压
	
	CORE_ACT_SN_END
};

enum EN_CORE_ACTNAME
{
	COREFWD			= 	STAGE_1_STEP,	//0x22111000 	中子进
	COREFWDSLW		=	STAGE_2_STEP,	//0x22111100	中子进慢
	COREFWDHOLD		=	STAGE_3_STEP,	//0x22111200	中子进保持
		
	COREBWD			= 	STAGE_1_STEP,	//0x22121000	中子退
	COREBWDSLW		=	STAGE_2_STEP,	//0x22121100	中子退慢
	COREBWDHOLD		=	STAGE_3_STEP,	//0x22121200	中子退保持

    COREFWDRLS	    = 	STAGE_1_STEP,	//0x22151000 	中子进泄压
	COREBWDRLS	    = 	STAGE_2_STEP,	//0x22151100 	中子退泄压
};

#define     ER0_CORE      0x0001        // #22100-#22600  ; 中子A~F未到定位
#define     ER0_CEFF      0x0002        // #22101-#22601  ; 中子A~F动作位置偏差
//20170921 LET  机械手新增
#define     ER0_ROB2_CPRF    0x0004     // #22102-#22602 ;  '等待机械手    -中子A-F 进安全'           22100103
#define     ER0_ROB2_CPRB    0x0008     // #22103-#22603 ;  '等待机械手    -中子A-F 退安全'           22100104
#define     ER0_ROBO_CPRF    0x0010     // #22104-#22604 ;  '机械手失败    -中子A-F 进安全'           22100105
#define     ER0_ROBO_CPRB    0x0020     // #22105-#22605 ;  '机械手失败    -中子A-F 退安全'           22100106

//20211215 ccz 新中子功能增加
#define     ER0_SITE        0x0040      // #22106-#22606 ; 中子A~F进退限位器同时有信号   22100107
#define     ER0_EJTF_CRF    0x0080      // #22107-#22607 ; 托模进时，中子A~F进未到定位   22100108
#define     ER0_EJTF_CRB    0x0100      // #22108-#22608 ; 托模进时，中子A~F退未到定位   22100109
#define     ER0_EJTB_CRF    0x0200      // #22109-#22609 ; 托模退时，中子A~F进未到定位   2210010A
#define     ER0_EJTB_CRB    0x0400      // #2210A-#2260A ; 托模退时，中子A~F退未到定位   2210010B
#define     ER0_CRF_EJTB    0x0800      // #2210B-#2260B ; 中子A~F进时，托模退未到定位   2210010C
#define     ER0_CLS_CRF     0x1000      // #2210C-#2260C ; 关模时，中子A~F进未到定位     2210010D
#define     ER0_CLS_CRB     0x2000      // #2210D-#2260D ; 关模时，中子A~F退未到定位     2210010E
#define     ER0_OPN_CRF     0x4000      // #2210E-#2260E ; 开模时，中子A~F进未到定位     2210010F
#define     ER0_OPN_CRB     0x8000      // #2210F-#2260F ; 开模时，中子A~F退未到定位     22100110
#define     ER0_SEQ_CRF     0x10000     // #22110-#22610 ; 顺序检查:中子A~F进未到定位    22100201
#define     ER0_SEQ_CRB     0x20000     // #22111-#22611 ; 顺序检查:中子A~F退未到定位    22100202
#define     ER0_CRF_POS     0x40000     // #22112-#22612 ; 中子A~F进动作位置偏差         22100203
#define     ER0_CRB_POS     0x80000     // #22113-#22613 ; 中子A~F退动作位置偏差         22100204
#define     ER0_CRB_EJTB    0x100000    // #22114-#22614 ; 中子A~F退时，托模退未到定位   22100205

#endif

#ifdef __cplusplus
}
#endif

#endif


