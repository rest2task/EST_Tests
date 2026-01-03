#ifndef _ROTATE_DIC_H_
#define _ROTATE_DIC_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#pragma pack(1)
typedef struct EN_ROTATE_PO
{
  UI16  O_ROTSLOW;                  //0x27100000 滑模慢速    
  UI16  O_ROT_FWD ;                 //0x27100001 滑模进    
  UI16  O_ROT_BWD ;                 //0x27100002 滑模退       
  UI16  O_ROT_FWDS ;                //0x27100003 左滑模快速   
  UI16  O_ROT_BWDS ;                //0x27100004 右滑模快速   
  UI16  O_ROT_RSV;                  //0x27100005 滑模备用
}ROTATE_PO, *PROTATE_PO;

typedef struct EN_ROTATE_PI
{
  UI16  I_ROT_FWD ;                 //0x27110000 滑模进终   
  UI16  I_ROT_BWD ;                 //0x27110001 滑模退终    
  UI16  I_ROT_FWS ;                 //0x27110002 滑模进慢速  
  UI16  I_ROT_BWS ;                 //0x27110003 滑模退慢速 
  UI16  I_ROT_RSV ;                 //0x27110004 滑模备用
}ROTATE_PI, *PROTATE_PI;

typedef struct EN_ROTATE_MACHSET
{
  UI16 ROTATE_METERCH;                        /* 0x27120000 滑模电子尺通道号 */
	  /*斜率+液压动作前后延时+阀动作前后延时+其他机器参数*/
	  /*+++++++++滑模斜率+++++++++*/
  UI16  DAFLOW_SCLPRAMP;                      /* 0x27120001 滑模启动速度斜率 */
  UI16  DAPRES_SCLPRAMP;                      /* 0x27120002 滑模启动压力斜率 */
  UI16  DAFLOW_SCLPOFFRAMP;                   /* 0x27120003 滑模结束速度斜率 */
  UI16  DAPRES_SCLPOFFRAMP;                   /* 0x27120004 滑模结束压力斜率 */  
  UI16  TM_SCLPBDLY;                          /* 0x27120005 左滑模延迟 范围0.00-99.99 默认0.00   20200219*/
  UI16  TM_SCLPFDLY;                          /* 0x27120006 右滑模延迟 范围0.00-99.99 默认0.00   20200219*/
  UI16  TM_ROTATEFAST;                        /* 0x27120007 滑模快速保护计时范围0.00-99.99 默认10.00   20200219 */
  UI16  TM_ROTLOC;                            /* 0x27120008 滑模保护计时 */
  UI16  SCLB_HYDELAY;						  /* 0x27120009 滑模退液压延迟*/
  UI16	SCLF_HYDELAY;						  /* 0x2712000A 滑模进液压延迟*/
}ROTATE_MACHSET, *PROTATE_MACHSET;

typedef struct EN_ROTATE_ACTPARA
{
	  /*PF+POS+TIME+HYDR(PORT;PUMPS)+SERVO(PORT;PUMPS;PID)*/
  UI16  ADPOSI_SCLPBFAST;                     /* 0x27140000 滑退快速位置   chg by rch , 2013-1-4*/
  UI16  ADPOSI_SCLPBLOW;                      /* 0x27140001 滑退慢速位置   chg by rch , 2013-1-4*/
  UI16  ADPOSI_SCLPFFAST;                     /* 0x27140002 滑进快速位置   chg by rch , 2013-1-4*/
  UI16  ADPOSI_SCLPFLOW;                      /* 0x27140003 滑进慢速位置   chg by rch , 2013-1-4*/
    
  UI16  DAFLOW_SCLPBLOW;                      /* 0x27140004 滑模退慢速速度 chg by rch , 2013-1-4*/
  UI16  DAFLOW_SCLPBFAST;                     /* 0x27140005 滑模退快速速度 chg by rch , 2013-1-4*/
  UI16  DAFLOW_SCLPFLOW;                      /* 0x27140006 滑模进慢速速度 chg by rch , 2013-1-4*/
  UI16  DAFLOW_SCLPFFAST;                     /* 0x27140007 滑模进快速速度 chg by rch , 2013-1-4*/
  UI16  DAPRES_SCLPBLOW;                      /* 0x27140008 滑模退慢速压力 chg by rch , 2013-1-4*/
  UI16  DAPRES_SCLPBFAST;                     /* 0x27140009 滑模退快速压力 chg by rch , 2013-1-4*/
  UI16  DAPRES_SCLPFLOW;                      /* 0x2714000A 滑模进慢速压力 chg by rch , 2013-1-4*/
  UI16  DAPRES_SCLPFFAST;                     /* 0x2714000B 滑模进快速压力 chg by rch , 2013-1-4*/

}ROTATE_ACTPARA, *PROTATE_ACTPARA;

typedef struct EN_ROTATE_HYDR
{
		/*+++++++++HYDR(PORT;PUMPS)+++++++++*/
    UI16  HYDR_PORT_ROTATEFWD;				 //0x27150000 滑模进动作的DA输出通道：位操作-->port1……port4
    UI16  HYDR_PUMPS_ROTATEFWD;				 //0x27150001 滑模进动作的比例速度限制:位操作-->P1……P7

	UI16  HYDR_PORT_ROTATEBWD;               //0x27150002 滑模退动作的DA输出通道：位操作-->port1……port4
	UI16  HYDR_PUMPS_ROTATEBWD;              //0x27150003 滑模退动作的比例速度限制:位操作-->P1……P7
	/*滑模PID*/
    UI16  SERVO_KP_ROTATEFWD;                //0x27150004 滑模进的PID_KP
    UI16  SERVO_KI_ROTATEFWD;				 //0x27150005 滑模进的PID_KI
    UI16  SERVO_KD_ROTATEFWD;				 //0x27150006 滑模进的PID_KD

    UI16  SERVO_KP_ROTATEBWD;				 //0x27150007 滑模退的PID_KP
    UI16  SERVO_KI_ROTATEBWD;				 //0x27150008 滑模退的PID_KI
    UI16  SERVO_KD_ROTATEBWD;				 //0x27150009 滑模退的PID_KD
 }ROTATE_HYDR, *PROTATE_HYDR;

typedef struct EN_ROTATE_STATE
{
    UI16  FL_ERROR0;						 //0x27160000 警报5
    UI16  FL_ERROR1;						 //0x27160001 警报5
    UI16  Step_RotateFwd;                    //0x27160002 转盘进实时动作步   成品画面显示
    UI16  Step_RotateBwd;                    //0x27160003 转盘退实时动作步   成品画面显示
    
	UI16 GRADE_ERR0;					  // 0x27160004  警报等级:二进制的11 为最低级的提示信息，不需要记录*/ 
	UI16 GRADE_ERR1;					  // 0x27160005  警报等级:二进制的11 为最低级的提示信息，不需要记录*/ 
	UI16 GRADE_ERR2;					  // 0x27160006  警报等级:二进制的11 为最低级的提示信息，不需要记录*/ 
	UI16 GRADE_ERR3;					  // 0x27160007  警报等级:二进制的11 为最低级的提示信息，不需要记录*/ 
}ROTATE_STATE, *PROTATE_STATE;
#pragma pack()

#ifdef DIC_ENUM_NEED

/*ACT SN start from 1*/
enum  EN_ROTATE_ACT_SN
{
  OS_RTTF= 1,        	//   滑模进       
  OS2_RTTB,             //   滑模退       
  OS_CLAMP_ROT,         //   开模滑模调度
  OS_RCYC_ROT,          //   循环开始滑模调度
  ROTATE_ACT_SN_END
};

enum EN_ROTATE_ACTNAME
{
    ROTATEFWD = STAGE_1_STEP,                      /* 滑模进快速*/
    ROTATEFWDSLW = STAGE_2_STEP,                   /* 滑模进慢速*/

    ROTATEFWDSETUP = STAGE_SPEC2_STEP,
    
    ROTATEBWD = STAGE_1_STEP,                      /* 滑模退快速*/   
    ROTATEBWDSLW = STAGE_2_STEP,                   /* 滑模退慢速*/

    ROTATEBWDSETUP = STAGE_SPEC2_STEP,             /* 滑模退慢速*/
};

#define     ER0_ROTATEF		0x0001      //#27100；滑模未到定位     
#define     ER0_ROTATEB  	0x0002      //#27101；滑模退未到定位   
#define     ER0_MTHK        0x0004      //#27102；滑模监控失败 

#endif


#ifdef __cplusplus
}
#endif

#endif
