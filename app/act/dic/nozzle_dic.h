#ifndef _NOZZLE_DIC_H_
#define _NOZZLE_DIC_H_

#include "typedef.h"

#pragma pack(1)

typedef struct EN_NOZZLE_PO
{
    UI16  O_NOZ_FWD;                 //0x11100000 座进
    UI16  O_SRV_NOZF;                //0x11100001 座进备用
    UI16  O_NOZ_BWD;                 //0x11100002 座退
    UI16  O_SRV_NOZB;                //0x11100003 座退备用
}NOZZLE_PO, *PNOZZLE_PO;

typedef struct EN_NOZZLE_PI
{
    UI16  I_NOZ_FWD;                 //0x11110000 座进终
    UI16  I_NOZ_FWS;                 //0x11110001 座进慢速
    UI16  I_NOZ_BWD;                 //0x11110002 座退终
    UI16  I_NOZ_BWS;                 //0x11110003 座退慢速

}NOZZLE_PI, *PNOZZLE_PI;

typedef struct EN_NOZZLE_MACHSET
{
    UI16 NZL_METERCH;				           /* 0x11120000 座台部件电子尺通道号*/

    /*斜率+液压动作前后延时+阀动作前后延时+其他机器参数*/
    UI16  DAPRES_NOZRAMP;                      /* 0x11120001 座台启动压力斜率         范围0.00-1.99s    默认0.50*/
    UI16  DAFLOW_NOZRAMP;          	     	   /* 0x11120002 座台启动速度斜率         范围0.00-1.99s    默认0.50*/
    UI16  DAPRES_OFFNOZRAMP;                   /* 0x11120003 座台关闭压力斜率         范围0.00-1.99s    默认0.00*/
    UI16  DAFLOW_OFFNOZRAMP;          	       /* 0x11120004 座台关闭速度斜率         范围0.00-1.99s    默认0.00*/
    UI16  DAPRES_NOZSETUP;                     /* 0x11120005 座台慢速调试压力 范围0-140            默认30*/
    UI16  DAFLOW_NOZSETUP;                     /* 0x11120006 座台慢速调试速度 范围0-99             默认30*/
    /*所有阀延迟都为0.00~9.99s*/
    UI16  TM_DLYNOZADV;                        /* 0x11120007 座进延时              .01S    默认0.00*/
    UI16  TM_DLYNOZCOM;                        /* 0x11120008 座进动作结束延时 范围0.00-9.99s           默认0.00*/
    UI16  TM_DLYNOZRET;                        /* 0x11120009 座退延迟              .01S    默认0.00*/
    UI16  TM_DLYNOZBCK;                        /* 0x1112000A 座退动作结束延时 范围0.00-9.99s           默认0.00*/
    UI16  TM_NOZADVSLOW;                       /* 0x1112000B 座台进慢速计时          范围0.00-5.00s 默认0.00*/
    UI16  TMDLY_VLVOFFNOZBWD;                  /* 0x1112000C 座退阀延迟关计时 范围0.00-1.00s           默认0.00*/

    UI16  NZLF_HYDELAY;						   /* 0x1112000D 座进液压延迟开          范围0.000-9.990s   默认0.000*/
    UI16  NZLB_HYDELAY;						   /* 0x1112000E 座退液压延迟开          范围0.000-9.990s   默认0.000*/
    UI16  TMDLY_VLV_ONNZLF;					   /* 0x1112000F 座进阀延时开计时 范围0.00-5.00s           默认0.00*/
    UI16  TMDLY_VLV_OFFNZLF;				   /* 0x11120010 座进阀延时关计时 范围0.00-5.00s           默认0.00*/
    UI16  TMDLY_VLV_ONNZLB;					   /* 0x11120011 座退阀延时开计时 范围0.00-5.00s           默认0.00*/
    UI16  NOZEND;			   				   /* 0x11120012 座台使用方式 DK 2015.0608 立式机使用*/
}NOZZLE_MACHSET, *PNOZZLE_MACHSET;

typedef struct EN_NOZZLE_MOLDSET
{
    /*功能+警报参数*/
    /*冷却*/
    UI16  TM_COOLINGBEFORECHG_L;               /* 0x11130000 储前冷却计时    默认0.00       0.00-3600.00   20200219*/
    UI16  TM_COOLING_L;                        /* 0x11130001 冷却计时     默认2.00  0.00-3600.00    20200219*/
    UI16  FL_NOZRET;                           /* 0x11130002 料台活动 0=不用 1=储料後 2=开模前 3=射出後 默认0*/
    /*+++++++++警报参数+++++++++*/
    UI16  TM_NOZDEFAULTMAX;                	   /* 0x11130003 座台计时警报上限内设值     */
    UI16  FL_COOLCHG;                		   /* 0x11130004 储前冷却使用不使用     */
	UI16  TM_COOLINGBEFORECHG_H;			   /* 0x11130005 储前冷却计时高16位    默认0.00       0.00-3600.00   20200219*/
	UI16  TM_COOLING_H;						   /* 0x11130006 冷却计时 高16位    默认2.00  0.00-3600.00   20200219*/
}NOZZLE_MOLDSET, *PNOZZLE_MOLDSET;

typedef struct EN_NOZZLE_ACTPARA
{
    /*PF+POS+TIME+HYDR(PORT;PUMPS)+SERVO(PORT;PUMPS;PID)*/
    /*+++++++++PF+POS+TIME+++++++++*/
    UI16  DAPRES_NOZADV;                       /* 0x11140000 座台进压力          范围0-140 默认50      */
    UI16  DAFLOW_NOZADV;                       /* 0x11140001 座台进速度          范围0-99    默认30      */
    UI16  DAPRES_NOZADVSLOW;                   /* 0x11140002 座台进慢速压力       */
    UI16  DAFLOW_NOZADVSLOW;                   /* 0x11140003 座台进慢速速度*/
    UI16  DAPRES_NOZRET;                       /* 0x11140004 座台退压力          范围0-140 默认50      */
    UI16  DAFLOW_NOZRET;                       /* 0x11140005 座台退速度          范围0-99    默认30      */
    UI16  DAPRES_NOZRETSLOW;               	   /* 0x11140006 座台退慢速压力       */
    UI16  DAFLOW_NOZRETSLOW;               	   /* 0x11140007 座台退慢速速度*/

    UI16  ADPOSI_NOZADVSLOWEND;                /* 0x11140008 座台进慢速终止位置*/
    UI16  ADPOSI_NOZRETSLOWEND;            	   /* 0x11140009 座台退慢速终止位置*/
    UI16  ADPOSI_NOZRETSLOWDIST;           	   /* 0x1114000A 座台退慢速距离*/
    UI16  ADPOSI_NOZADVEND;                    /* 0x1114000B 座台进终止位置 范围0-250.0           默认0.0    */
    UI16  ADPOSI_NOZRETEND;                    /* 0x1114000C 座台退终止位置 范围0-250.0           默认0.0    */

    UI16  TM_NOZRETACT;                        /* 0x1114000D 座退计时     范围0.00-99.99 默认0.00    */
    UI16  TM_NOZADVACT;                        /* 0x1114000E 座进计时     范围0.00-99.99 默认0.00    */

	UI16  FL_OILHEAT;						   /* 0x1114000F 油温预热功能开启  0  关闭   1   开启   默认关闭*/ 
	UI16  DAPRES_OILHEAT;					   /* 0x11140010 油温预热压力  范围0-系统压力上限 默认30*/
	UI16  DAFLOW_OILHEAT;					   /* 0x11140011 油温预热速度  范围0-99  默认20*/
	UI16  TM_OILHEAT;						   /* 0x11140012 油温预热时间  范围  0-120 分    默认10分钟*/

	UI16  PER_DAFLOW_NOZZLE;				   /* 0x11140013 座台速度百分比  范围 0-100  默认 0 */
}NOZZLE_ACTPARA, *PNOZZLE_ACTPARA;

typedef struct EN_NOZZLE_HYDR
{
    /*+++++++++HYDR(PORT;PUMPS)+++++++++*/
    UI16  HYDR_PORT_NOZZLE;						//0x11150000 座台的DA输出通道：位操作-->port1……port4
    UI16  HYDR_PUMPS_NOZZLE;					//0x11150001 座台装模的泵浦限制:位操作-->P1……P7
    UI16  SERVO_KP_NOZZLE;						//0x11150002 座台动作的PID_KP 范围0-2500 默认800
    UI16  SERVO_KI_NOZZLE;						//0x11150003 座台动作的PID_KI 范围0-200         默认40
    UI16  SERVO_KD_NOZZLE;						//0x11150004 座台动作的PID_KD
}NOZZLE_HYDR, *PNOZZLE_HYDR;

typedef struct EN_NOZZLE_STATE
{
    UI16  FL_ERROR0;				//0x11160000 警报1
    UI16  FL_ERROR1;				//0x11160001 警报2
    UI16  TM_NOZZLEFWD;             //0x11160002 座进计时实际值 0.01s
    UI16  TM_NOZZLEBWD;             //0x11160003 座退计时实际值 0.01s
    UI16  TM_COOL_RT_L;				//0x11160004 实时计算的冷却计时时间，用于面板显示, 0.00s。  20200219
    UI16  Step_NozzleFwd;           //0x11160005 座台进实时动作步     成品画面显示
    UI16  Step_NozzleBwd;           //0x11160006 座台退实时动作步     成品画面显示
    UI16  TM_COOL_START;            //0x11160007 冷却计时起始时间		    单位:10ms       去除该功能，面板画线改为由0开启，否则所有相关的都得32位，数据太多 /* 20200219*/
    UI16  TM_NOZZLEF_START;			//0x11160008 座进计时起始值		   单位:10ms     去除该功能，面板画线改为由0开启，否则所有相关的都得32位，数据太多 /* 20200219*/
    UI16  TM_NOZZLEB_START;			//0x11160009 座退计时起始值		   单位:10ms       去除该功能，面板画线改为由0开启，否则所有相关的都得32位，数据太多 /* 20200219*/
    UI16  TM_NOZZLEF_RT;			//0x1116000A 座进计时实时值		   单位:10ms
    UI16  TM_NOZZLEB_RT;			//0x1116000B 座退计时实时值		   单位:10ms 
	UI16  TM_COOL_RT_H;				//0x1116000C 实时计算的冷却计时时间高16位，用于面板显示, 0.00s。	/* 20200219*/
	UI16  TM_COOL_L;				//0x1116000D 冷却计时实际值	低位 单位:10ms
	UI16  TM_COOL_H;				//0x1116000E 冷却计时实际值 高位 单位:10ms
}NOZZLE_STATE, *PNOZZLE_STATE;

#pragma pack()


#ifdef DIC_ENUM_NEED

/*ACT SN start from 1*/
enum EN_NOZZLE_ACT_SN
{
    OS_NOZF    =  1,      	// 1   ; '座台进  '
    OS_NOZB,                // 2   ; '座台退  '
    OS_ANOZF,               // 3   ; '调模座台进  '
    OS_ANOZB,      			// 4   ; '调模座台退  '

    OS_COOL,				// 5   ; '正常冷却    '
    OS_COLBC,				// 6      储料前冷却
    OS_OILHRAT,				//7 	油温预热   20200218
    OS_NOZF_SYNC,           // 8   ; '电储料同步座台进' (面板翻译座台进)

    NOZZLE_ACT_SN_END
};


enum EN_NOZZL_ACTNAME
{
    /*OS_NOZF*/
    NOZZLEFWD = STAGE_1_STEP,           //座台进
    NOZZLEFWDSLW = STAGE_2_STEP,		//座台进慢速

    /*OS_NOZB*/
    NOZZLEBWD = STAGE_1_STEP,           //座台退
    NOZZLEBWDSLW = STAGE_2_STEP,		//座台退慢速
};

#define     ER0_NZLIMT      0x0001          //#11100 '座进终未到位      '
#endif


#endif


