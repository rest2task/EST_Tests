#ifndef _SAFEDOOR_DIC_H_
#define _SAFEDOOR_DIC_H_

#include "typedef.h"

#pragma pack(1)

typedef struct EN_SAFEDOOR_PO
{
    UI16  O_SDR_OPN ;                 //0x29100000 安全门开 
    UI16  O_SDR_CLS ;                 //0x29100001 安全门关  
    UI16  O_SDR_SLW ;                 //0x29100002 安全门慢速    
    UI16  O_SDR_FST ;                 //0x29100003 安全门快速  
    UI16  O_RSDR_OP ;                 //0x29100004 后安全门开  
    UI16  O_RSDR_CL ;                 //0x29100005 后安全门关   
    UI16  O_SDR_RSV;                  //0x29100006 安全门备用
    UI16  O_SDR_ERR;	 			  //0x29100007 门机复位
    UI16  O_SDR_VALVE;	 			  //0x29100008 安全阀
}SAFEDOOR_PO, *PSAFEDOOR_PO;

typedef struct EN_SAFEDOOR_PI
{
    UI16  I_SDR_CLS ;                 //0x29110000 安全门关    
    UI16  I_SDR_CL2 ;                 //0x29110001 安全门关二   
    UI16  I_SDR_OPN ;                 //0x29110002 安全门开终 //20190403 CHJ 补齐
    UI16  I_SDR_SLW ;                 //0x29110003 安全门关慢     
    UI16  I_SDR_OSL ;                 //0x29110004 安全门开慢   
    UI16  I_SDR_SAF ;                 //0x29110005 安全门防碰   
    UI16  I_RSD_CLS ;                 //0x29110006 后安全门关  
    UI16  I_FOOTSW  ;                 //0x29110007 脚踏开关     
    UI16  I_FOOTEN  ;                 //0x29110008 可用脚踏开关 
    UI16  I_SAF_PLT;                  //0x29110009 底板安全    
    UI16  I_SDR_RSV;                  //0x2911000A 安全门备用
    UI16  I_SDR_SQ42;	              //0x2911000B 安全阀控制开关   
    UI16  I_SDR_VALVE;                //0x2911000C 安全阀检测          
    UI16  I_SDR_PREVAVLE;             //0x2911000D 安全阀先导检测     
    UI16  I_SDR_ERR;                  //0x2911000E 门机故障           
    UI16  I_SDR_SAFE_RULE;            //0x2911000F 安全门检知
    UI16  I_RSD_CLS2 ;                //0x29110010 后安全门关2
    UI16  I_SFE_LINE;                 //0x29110011 安全杆
    UI16  I_SAFE_EDGE;                //0x29110012 安全边沿开关 //20190403 CHJ 补齐
    UI16  I_SAFE_REMERG;			  //0x29110013 安全门急停
}SAFEDOOR_PI, *PSAFEDOOR_PI;

typedef struct EN_SAFEDOOR_MACHSET
{
    /*斜率+液压动作前后延时+阀动作前后延时+其他机器参数*/
    UI16  TM_SAFEDOORSLOWVLV;              /* 0x29120000 安全门慢速阀动作时间 范围0.00-99.99         默认0.00    20200219 */
    UI16  TMDLY_PWRDOOR;                   /* 0x29120001 安全门延迟           范围0.00-99.99       默认0.00 未使用*/
    UI16  DAPRES_SAFEDRZRAMP;              /* 0x29120002 安全门启动压力斜率   范围0.00-1.99        默认0.20*/
    UI16  DAFLOW_SAFEDRZRAMP;          	   /* 0x29120003 安全门启动速度斜率   范围0.00-1.99        默认0.20*/
    UI16  DAPRES_SAFEDROFFRAMP;            /* 0x29120004 安全门关闭压力斜率   范围0.00-1.99        默认0.20*/
    UI16  DAFLOW_SAFEDROFFRAMP;            /* 0x29120005 安全门关闭速度斜率   范围0.00-1.99        默认0.20*/
    UI16  DAPRES_SAFETYDOOR_SETUP;         /* 0x29120006 安全门慢速调试压力   范围0-系统压力上限   默认30*/
    UI16  DAFLOW_SAFETYDOOR_SETUP;         /* 0x29120007 安全门慢速调试速度   范围0-99             默认30*/

    UI16  SFDROPN_HYDELAY;				   /* 0x29120008 安全门开退液压延迟   范围0.000-9.999  默认0.000 */
    UI16  SFDRCLS_HYDELAY;			       /* 0x29120009 安全门关液压延迟     范围0.000-9.999  默认0.000 */
    UI16  SFDRCLS_TMCHECK;                 /* 0x2912000A 安全门开检测时间,    范围0.30-2.00      默认0.40s*/

	UI16  SDR_VALVE_CLSTMCHECK;			   /* 0x2912000B 安全阀关检测计时	范围0~9.99S	默认1.00S*/			
	UI16  SDR_VALVE_OPNTMCHECK;			   /* 0x2912000C 安全阀开检测计时	范围0~9.99S 默认1.00S*/
}SAFEDOOR_MACHSET, *PSAFEDOOR_MACHSET;

typedef struct EN_SAFEDOOR_MOLDSET
{
    /*功能+警报参数*/
    UI16  FL_PWRDOORCTRL;                  /* 0x29130000 自动安全门功能    0不使用1使用  默认0         */
    UI16  FL_PWRDOOROPEN;                  /* 0x29130001 安全门开模式      0开模前1开模完 默认0 */
    UI16  FL_MANULTEST;					   /* 0x29130002 安全门调试功能	   暂未使用*/
    UI16  FL_TESTTIMES;					   /* 0x29130003 安全门调试次数	   暂未使用*/
}SAFEDOOR_MOLDSET, *PSAFEDOOR_MOLDSET;

typedef struct EN_SAFEDOOR_ACTPARA
{
    UI16  DAPRES_SAFETYDOOR;               /* 0x29140000 安全门压力        范围0-70       默认 20     */
    UI16  DAFLOW_SAFETYDOOR;               /* 0x29140001 安全门速度        范围0-40       默认 20     */
    UI16  DAPRES_SAFETYDOORSLOW;           /* 0x29140002 安全门慢速压力    范围0-70       默认 20     */
    UI16  DAFLOW_SAFETYDOORSLOW;           /* 0x29140003 安全门慢速速度    范围0-40       默认 20     */
    UI16  TM_PWRDOOROPEN;                  /* 0x29140004 安全门开计时      范围0.00-99.99   默认 0.00    20200219*/
	UI16  PER_DAFLOW_SAFEDOOR;			   /* 0x29140005 安全门速度百分比  范围0-100  默认100	*/

}SAFEDOOR_ACTPARA, *PSAFEDOOR_ACTPARA;

typedef struct EN_SAFEDOOR_HYDR
{
    UI16  HYDR_PORT_SAFETYDOOR;				//0x29150000 安全门动作的DA输出通道：位操作-->port1……port4
    UI16  HYDR_PUMPS_SAFETYDOOR;			//0x29150001 安全门的比例流量限制:位操作-->P1……P7
    UI16  SERVO_KP_SAFETYDOOR;				//0x29150002 安全门的PID_KP   范围0-2500  默认210
    UI16  SERVO_KI_SAFETYDOOR;				//0x29150003 安全门的PID_KI   范围0-200   默认 10
    UI16  SERVO_KD_SAFETYDOOR;				//0x29150004 安全门的PID_KD  暂未使用
}SAFEDOOR_HYDR, *PSAFEDOOR_HYDR;

typedef struct EN_SAFEDOOR_STATE
{
    UI16  FL_ERROR0;						//0x29160000 警报1
    UI16  FL_ERROR1;						//0x29160001 警报2
    UI16  Step_DoorOpen;                    //0x29160002 安全门开实时动作步
    UI16  Step_DoorClos;                    //0x29160003 安全门关实时动作步
}SAFEDOOR_STATE, *PSAFEDOOR_STATE;

#pragma pack()

#ifdef DIC_ENUM_NEED

enum EN_SAFEDOOR_ACT_SN
{
    OS_SDRO = 1,   				// '安全门开'    
    OS_SDRC,					//  '安全门关
    OS_SDRT,					//   暂未使用                                   20170505  LET

    SAFEDOOR_ACT_SN_END
};

/*错误位定义*/
#define     ER0_CSDR      	0x0001      // #29100；安全门未关    
#define     ER0_OSDR      	0x0002      // #29101；请开安全门
#define     ER3_RSDR      	0x0004      // #29102；背面安全门未关
#define     ER3_FSER      	0x0008      // #29103；安全门限位开关异常
#define     ER3_OSDR      	0x0010      // #29104；请开安全门2  
#define     ER3_CSDR      	0x0020      // #29105；安全门2未关  
#define     ER4_SDRSAF    	0x0040      // #29106；安全门保护    
#define     ER5_SDSA      	0x0080      // #29107；安全门检测异常
#define     ER5_RDSA      	0x0100      //#29108；后安全门检测异常
#define     ER5_PDSA      	0x0200      // #29109；自动门安全异常
#define     ER5_SQ42      	0x0400      // #2910A；安全阀控制开关异常
#define     ER5_SAVALVE  	0x0800      // #2910B；安全阀异常
#define     ER5_PRESAVALVE	0x1000      // #2910C；安全阀先导阀异常
#define     ER0_SLID      	0x2000      // #2910D；请按下关模按钮
#define     ER1_SCLS      	0x4000      // #2910E；合模开关未同步 
#define     ER5_SACTLERR  	0x8000      // #2910F；门机异常
#define     ER5_DOORERR   	0x10000     // #29110；安全杆信号异常
#define		ER0_SAFE_REMERG	0x20000		 // #29111；安全门急停已按下
#define     ER0_SDREDGE     0x100000    // #29205  ; 安全边沿开关异常
#define		ER1_SAVALVEOPN	0x200000	 // #29206	; 安全阀未开
#define     ER1_SQ42SAVALVE 0x400000    // #29207	；SQ42安全阀异常


enum EN_SAFEDOOR_ACTNAME
{
    SAFETYDOOROPN		=	STAGE_1_STEP, 	//	安全门开快速
    SAFETYDOOROPNSLOW   =	STAGE_2_STEP,	//	安全门开慢速

    SAFETYDOORCLS	    =	STAGE_1_STEP, 	//	安全门关快速
    SAFETYDOORCLSSLOW   =	STAGE_2_STEP	//	安全门关慢速
};

#endif

#endif



