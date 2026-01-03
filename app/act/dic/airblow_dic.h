#ifndef _AIRBLOW_DIC_H_
#define _AIRBLOW_DIC_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"

#pragma pack(1)

typedef struct EN_AIRBLOW_PO
{
    UI16  O_AIR_BLT ;                   // 0x23100000 吹气   
    UI16  O_AIR_SRV;                    // 0x23100001 备用
    UI16  O_AIR_SEQ1;                   // 0x23100002 顺序阀1 20200729
    UI16  O_AIR_SEQ2;                   // 0x23100003 顺序阀2 20200729
    UI16  O_AIR_SEQ3;                   // 0x23100004 顺序阀3 20200729
    UI16  O_AIR_JKF1;                  	// 0x23100005 阀门1-1 20200729
    UI16  O_AIR_JKF2;                  	// 0x23100006 阀门1-2 20200729
}AIRBLOW_PO, *PAIRBLOW_PO;

typedef struct EN_AIRBLOW_MOLDSET
{
    UI16  FL_BLAST;                     /* 23130000 吹气动作模式，默认256，范围   0:开模时间，1:开模位置，   2:射出时间，3:射出位置，
                                         4:关模时间，5:关模位置，6:托模进位置，7:电眼后，8:托模退位置，9:储料位置，256:不使用*/
	UI16  ADPOSI_MOVINGBLASTSTART;      /* 23130001 吹气动作位置	    	.0	范围	0.0-450.0  	默认0.0*/
    UI16  TM_MOVINGPLATENDLY;           /* 23130002 动作延迟计时	      	.01 范围0.00-99.90	默认0.00 */
    UI16  TM_MOVINGPLATENACT;           /* 23130003 公模动作计时		    .01 范围0.00-60.00	默认0.00*/
    UI16  TM_AIRDLY;                    /* 23130004 间隔计时               .01 默认0.00*/
    UI16  TM_AIRALL;                    /* 23130005 总计时                 .01 默认0.00*/

	UI16  FL_SEQ_USE1;                   //23130006   顺序阀1使用  0  不使用   1   时间开时间关  2  位置开位置关  3  时间开位置关  4  位置开时间关 默认0
    UI16  TM_SEQ_DELAY1;                 //23130007   顺序阀1延迟计时          默认0.00 范围0.00-600.00    
    UI16  TM_SEQ_MOVING1;                //23130008   顺序阀1动作计时          默认0.00 范围0.00-600.00        
    UI16  ADPOSI_SEQSTART1;              //23130009   顺序阀1动作开位置     默认0.0 范围 0.0-射出尺上限
    UI16  ADPOSI_SEQEND1;                //2313000A   顺序阀1动作结束位置           默认0.0   范围 0.0-射出尺上限

	
	UI16  FL_SEQ_USE2;                   //2313000B   顺序阀2使用  0  不使用   1   时间开时间关  2  位置开位置关  3  时间开位置关  4  位置开时间关 默认0
    UI16  TM_SEQ_DELAY2;                 //2313000C   顺序阀2延迟计时          默认0.00 范围0.00-600.00    
    UI16  TM_SEQ_MOVING2;                //2313000D   顺序阀2动作计时          默认0.00 范围0.00-600.00        
    UI16  ADPOSI_SEQSTART2;              //2313000E   顺序阀2动作开位置     默认0.0 范围 0.0-射出尺上限
    UI16  ADPOSI_SEQEND2;                //2313000F   顺序阀2动作结束位置           默认0.0   范围 0.0-射出尺上限

	
	UI16  FL_SEQ_USE3;                   //23130010   顺序阀3使用  0  不使用   1   时间开时间关  2  位置开位置关  3  时间开位置关  4  位置开时间关 默认0
    UI16  TM_SEQ_DELAY3;                 //23130011   顺序阀3延迟计时          默认0.00 范围0.00-600.00    
    UI16  TM_SEQ_MOVING3;                //23130012   顺序阀3动作计时          默认0.00 范围0.00-600.00        
    UI16  ADPOSI_SEQSTART3;              //23130013   顺序阀3动作开位置     默认0.0 范围 0.0-射出尺上限
    UI16  ADPOSI_SEQEND3;                //23130014   顺序阀3动作结束位置           默认0.0   范围 0.0-射出尺上限
    
	UI16  FL_VALVEOPEN1;				//	23130015 阀门1启动模式	0:不使用 1:合模开始 2:合模位置 3:锁模位置 4:开始注射 5:注射位置
	UI16  FL_VALVECLS1;					//	23130016 阀门1停止模式	0:运行时间 1:注射保压后 2:注射位置 3:储前射退后 4:储料开始 5:射退开始 6:冷却开始 7:冷却结束 8:射退后
	UI16  TM_ValveDly1;					//	23130017 阀门1动作前延迟计时				默认0.00 
	UI16  ADPOSI_Valve1;				//	23130018 阀门1启动动作位置     			默认0.0
	UI16  TM_ValveMoving1;				//	23130019 阀门1动作计时      			默认0.00
	UI16  TM_ValveEndDly1;				//	2313001A 阀门1动作结束延时      			默认0.00
	UI16  ADPOSI_ValveEnd1;				//	2313001B 阀门1结束动作位置     			默认0.0

	UI16  FL_VALVEOPEN2;				//	2313001C 阀门2启动模式	0:不使用 1:合模开始 2:合模位置 3:锁模位置 4:开始注射 5:注射位置
	UI16  FL_VALVECLS2;					//	2313001D 阀门2停止模式	0:运行时间 1:注射保压后 2:注射位置 3:储前射退后 4:储料开始 5:射退开始 6:冷却开始 7:冷却结束 8:射退后
	UI16  TM_ValveDly2;					//	2313001E 阀门2动作前延迟计时				默认0.00 
	UI16  ADPOSI_Valve2;				//	2313001F 阀门2启动动作位置     			默认0.0
	UI16  TM_ValveMoving2;				//	23130020 阀门2动作计时      			默认0.00
	UI16  TM_ValveEndDly2;				//	23130021 阀门2动作结束延时      			默认0.00
	UI16  ADPOSI_ValveEnd2;				//	23130022 阀门2结束动作位置     			默认0.0

}AIRBLOW_MOLDSET, *PAIRBLOW_MOLDSET;

typedef struct EN_AIRBLOW_STATE
{
    UI16  FL_ERROR0;                    // 23160000 警报1
    UI16  FL_ERROR1;                    // 23160001 警报2
    UI16  Step_AirBlow;                 // 23160002  吹气实时动作步     成品画面显示
    UI16  Step_SEQ;                     // 23160003  顺序阀实时动作步 
}AIRBLOW_STATE, *PAIRBLOW_STATE;

#pragma pack()


#ifdef DIC_ENUM_NEED

/*ACT SN start from 1*/
enum EN_AIRBLOW_ACT_SN
{
    OS_AIRBL   	= 1,	//2311			吹气
    OS_SEQ1     = 2,	//2312          顺序阀1   20200729
    OS_SEQ2     = 3,	//2313          顺序阀2   20200729
    OS_SEQ3     = 4,	//2314          顺序阀3   20200729
	OS_VALVE 	= 5,	//2315		阀门1
	OS_VALVE2 	= 6,	//2316		阀门2
	AIRBLOW_ACT_SN_END
};

#define     ER0_AIRM      0x0001        //#23100-23E00	等待吹气1~14完成  
#define     ER0_SEQ       0x0002        //等待顺序阀1~14完成
#define     ER0_AIRJ      0x0004        //等待胶口针阀1~14完成  20200729

#endif

#ifdef __cplusplus
}
#endif

#endif


