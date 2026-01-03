#ifndef _MOTOR_H_
#define _MOTOR_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "acttype.h"
#include  "motor_dic.h"


#define CHK_MOTOR_MODE(mode) (m_motor.motor_control_mode == mode?TRUE:FALSE)
#define MOTOR_MODE	m_motor.motor_control_mode

typedef enum enMOTOR_STATUS
{
	MOTOR_OFF = 0,
	MOTOR_ON = 1,
	MOTOR_STARTING = 2,
	MOTOROFF_STARTING = 3,
	MOTORON_STARTINGPROT = 4,	
	MOTOROFF_STARTINGPROT = 5
}MOTOR_STATUS;

typedef enum enMOTOR_CTRL_MODE
{
	MOTOR_CTRL_COMMON = 0,
	MOTOR_CTRL_5MODE = 1,
	MOTOR_CTRL_ECAT = 2
}MOTOR_CTRL_MODE;


typedef BOOL (*SAFECHK_FUNC)(PPART pp);
typedef struct tyMOTOR
{
	PART				part;
	
	PMOTOR_PO			po;
	PMOTOR_PI			pi;
	PMOTOR_MACHSET		mh;
	PMOTOR_STATE		st;

	ACT					act[MOTOR_ACT_SN_END -1];

	UI16				motor_control_mode;		//0:普通	1:can5模式	2:Ecat
	SAFECHK_FUNC		SafeFunc;
	UI32					motor_on_ms;
	UI16                Delayms1;                      //马达启动延迟时间          2015/5/14
	UI16                Delayms2;                      //马达启动延迟时间          2015/5/14  yxh
	UI16                MotorCnt;                      //马达启动输入点检测次数计数   2015/5/14   yxh

	UI8				enableonnum;//伺服尝试使能次数
	WORD			motor_fi_tm_start;
	UI16*			motorsdoflag;//开关马达sdo完成标记
	UI16*			servosdoflag;//开关马达sdo完成标记

	UI8				ServoNo;
	UI8				SdoNum;

	UI16			pump_main;	

	UI16			fl_motor_temp;		//马达温度功能(特殊-普通机用)

	UI16			motor_freetime_start;	//马达闲置开始计时 - s
	UI16			servonenable_starttm;
}MOTOR, *PMOTOR;

extern	MOTOR m_motor;

#define MOTOR_INIT_INFO	{{PO_ID, CAL_LEN(MOTOR_PO)}, {PI_ID, CAL_LEN(MOTOR_PI)}, {MH_ID, CAL_LEN(MOTOR_MACHSET)}, {ST_ID, CAL_LEN(MOTOR_STATE)}}

BOOL MotorIsOn(void);
BOOL MotorIsOn2(void);

BOOL MotorDown(void);
BOOL MotorInit(void);
BOOL MotorTemperErr(void);
BOOL MotorStart(void);
void SetMotorOnSafeChk(SAFECHK_FUNC pfunc);
void MoterOverLoadChk(void);
BOOL InitMotorPart(void);
int MotorGetTemperCH(void);
void MachineMotorCheck(void);
void MotorEnableOnAct(void);
void MotorE08ReActDly(WORD dly_ms);
void MotorStateSet(WORD state);
BOOL MotorCmd(UI32 cmd, UI32 para);
void MotorFreeTimeChk(void);
void MotorOffOnce(void);

	
#ifdef __cplusplus
}
#endif

#endif



