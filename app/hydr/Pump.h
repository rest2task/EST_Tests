#ifndef _PUMP_H_
#define _PUMP_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "Pump_dic.h"
#include "Definelist.h"
#include "acttype.h"
//#include "Servo.h"
#include "Hydr.h"


#define MAXPUMPPORT	16
#define MAXMCOUTPORT 3
#define MCACC	1000
#define TORQUEDCC 5
#define MS_CYC_CNT	1000/CYCTM_US	//系统每毫秒运行2次(需要先乘1000后除以CYCTM_US)

#define HYDRAD		1000
#define HYDRADOLD		5

#define PUMPPART	1

#define MINRAMP	2200/(80*2)

#define SERVO_STATE_BUF_NUM	5

#define PUMP_ECAT_NUM	GetRealSlaveNum()

#define VIR_MAX_SPD 2000
#define VIR_MAX_SPD_K   10000*10/VIR_MAX_SPD


typedef enum enMC_CTRL_MODE
{
	MC_CTR_MODE_STOP	= 0x00,			//	停止模式
	MC_CTR_MODE_SPEED	= 0x01,			//	速度模式
	MC_CTR_MODE_POSI	= 0x02,			//	位置模式	
	MC_CTR_MODE_PRES	= 0x04,			//	压力模式
	MC_CTR_MODE_SLAVE	= 0x08,			//	从站模式
	MC_CTR_MODE_DA		= 0x10,			//	输出DA线性
}MC_CTRL_MOD;

typedef struct tyMC_PARA_BAK
{
	WORD ServoNo;
	WORD mode;
	int ActSpeed;
	WORD Torque;
	WORD McPres;
	int PrsActSpd;
	WORD PrsRelSpd;
	WORD PresRamp;
	WORD ActSpeedRamp;
	WORD TorqueRamp;
	WORD pumplist;
	WORD Model;
	WORD dely;
	WORD KiClear;
	WORD limitramp;
	WORD SpdDraw;
	WORD flag;		
}MC_PARA_BAK, *PMC_PARA_BAK;

/********************************************************************************************
|动作参数设置，用于ramp输出
|	MAXELEOUTPORT = 5				
|	0=压力 ;1=设定转速 ;2=设定扭矩 ;3=泄压转速 ;4=压力速度
********************************************************************************************/
typedef struct tyMC_PARA
{
	int		McSet[MAXMCOUTPORT]  ;	//	设定值
  	int		Mcnow[MAXMCOUTPORT]  ;	//	当前值：用于实际使用值	
  	int		Mclast[MAXMCOUTPORT] ;	//	上次当前值	
  	WORD	ramp[MAXMCOUTPORT]    ;	//	当前斜率	
  	WORD	rampok[MAXMCOUTPORT]  ;	//	斜率完成标记
  	WORD  	rampstart[MAXMCOUTPORT];  	//	斜率启动时间
    WORD 	Ramp_T1[MAXMCOUTPORT];
    WORD 	Ramp_T2[MAXMCOUTPORT];
    WORD 	Ramp_T3[MAXMCOUTPORT];
	int 	Ramp_a[MAXMCOUTPORT];
	int 	Ramp_aa[MAXMCOUTPORT];
	
	WORD	RampUp_limit[MAXMCOUTPORT];	//上升步幅限制
	WORD	RampDown_limit[MAXMCOUTPORT];	//下降步幅限制
	
	WORD	ActDelay;				
}MC_PARA, *PMC_PARA;

typedef struct tag_PRESSPOWER *PPRESS_STRU;
typedef struct tyMC_CTRL
{
	//	控制参数
	WORD	PowerMode;		//	动力模式:0=NULL,0x01=电动,0x02=液压(主站),0x04=液压(从站)	
	WORD	SlavePump;		//当slave值为0时表示，当前电机为从站，如果为主站则标记了对应bit位为1，其他位为1时为对应从站
	WORD	MainServoNo;	//主站编号
	WORD	StopFlag;		//停止标记	动作调用停止压力流量函数时标记

	//运行状态:0=停止	1=运行动作中 2=位置控制中
	//主站基本保持运行状态，从站停止时为0
	WORD	RunState;		
	WORD	LimitRamp;		//是否限制加速斜率	1:表示限制

	//从泵启停
	WORD	SlaveStartRpm;	//从泵启动转速
	WORD	SlaveStopRpm;	//从泵停止转速
	WORD	SlaveState;		//从站的启停状态
		
	// 设置参数
	WORD	McModeSet;		//	控制模式设定：0x0=null;0x01=位置;0x02=速度;0x04=压力;0x8=从站跟随	
	WORD	RunDirec;   	//	运行方向：0=正向-->位移量变大方向 ;1=反向-->位移量变小方向

	//	输出参数
	HANDLE		McSpdOutRPM;	//	电动输出转速
	HANDLE		McTorqueOut;	//	电动输出扭矩
	HANDLE		McHydrFlowOut;	//	流量输出
	HANDLE		McHydrPresOut;	//	压力输出	//一位小数点
	HANDLE		McSpdLastRPM;	//	上个周期电动输出转速
	HANDLE		McLastTorqueOut;	//	上个周期电动输出扭矩
	HANDLE		McLinenow;		//	当前值：用于实际使用值
	HANDLE		McPresOut;	    //	电动输出压力

	//	动作前后限位参数
	WORD 	LimitSwitch;	//	1=需要限位0=不需要限位
	HANDLE		PosLimtMax;		//	限位的最大位置	:max + 0.5mm
	HANDLE		PosLimtMin;		//	限位的最小位置	:min - 0.5mm
	HANDLE   	SetPulse;    	//	输出脉冲

	//	压力控制
	WORD		Fl_Pres;		//压力控制是否使用
	WORD		PresCtrlCh;		//压力控制通道	1开始 0表示不使用
	int			RealPres;		//实际压力
	UI16		RealPresCh;		//实际压力反馈AD通道	1开始
	PPRESS_STRU pMcPres;		//压力控制相关数据指针
	

	//	位置控制
	WORD		PosCtrlCh;		//定位控制通道	1开始 0表示不使用

	//	反转超时
	WORD		restarttm_10ms;	//反转开始时间

    //普通机模式使用参数
    WORD	    NormalSlavePump;  //普通机泵浦，用于输出点泵x控制
    WORD        NormalFlowSet;    //普通机速度设定
}MC_CTRL, *PMC_CTRL;


typedef struct tyPUMP
{
	PART				part;

	PPUMP_PO			po;
	PPUMP_MACHSET		mh;
	PPUMP_MOLDSET		md;
	PPUMP_STATE		st;

	WORD	pump_no;
	MC_PARA	McPara;	//运动控制参数	主要用于加速减速斜率计算
	MC_CTRL McCtrl;	//控制的全局参数
	MC_PARA_BAK McParaBak;	//最后一次设定备份
	WORD	lastPosStep;	//上一次的定位步

	UI32	EcoderPPR;

	UI16 servo_flag;
	UI16 servo_err_last;		//上一次警报信息
	UI16 servo_err_clear_tm;	//清除驱动警报计时	
	UI16 servo_err_clear_flag;	//清除驱动警报标记

	UI16 oil_leak_starttm;		//漏油开始计时时间

	UI32 MotorMaxRPM;			//最大转速
	UI16 MotorMaxFrequency;		//最大频率,单位0.01hz
	UI16 MotorPolePair;			//电机极对数
	UI16 MotorRatedSpeed;		//电机额定转速
	UI16 MotorRatedFrequency;	//电机额定频率,单位0.01hz

	
	UI16 baseflow;				//底流rpm
	UI16 sdo_alarm_starttm;	//
	UI16 sdo_alarm_abort;
}PUMP, *PPUMP;

#define PUMP_INIT_INFO	{{PO_ID, CAL_LEN(PUMP_PO)}, {MH_ID, CAL_LEN(PUMP_MACHSET)}, {MD_ID, CAL_LEN(PUMP_MOLDSET)}, {ST_ID, CAL_LEN(PUMP_STATE)}}

void SetMotionControlOutPara(WORD ServoNo,WORD mode, int ActSpeed, WORD Torque, WORD McPres, int PrsActSpd, WORD PrsRelSpd, WORD PresRamp, WORD ActSpeedRamp, WORD TorqueRamp, WORD pumplist, WORD Model, WORD dely,WORD KiClear,WORD limitramp,WORD SpdDraw,WORD flag);
void SetServoState(WORD ServoNo, int RealEncode, int RealSpeed, int RealTorque, WORD AlarmPara, WORD ServoTemp,
                   WORD RealPI, WORD RealPV, WORD ServoFlag, WORD MotorTemp, int ServoState);
void ChgMotionControlSpd(PPUMP pp, int ActSpeed, WORD ActSpeedRamp ,WORD mode);
BOOL InitPumpPart(PPUMP pp, UI8 sn);
WORD InitPresCtrl(void);
WORD GetPosCtrlCh(WORD pumpno);
WORD  GetMotorPump_Q(WORD pumpmain,WORD pumpbit);
BOOL PumpCmd(PPUMP pp,UI32 cmd, UI32 para);
BOOL HydrPrsSenZSetErrChk(void);
int GetPumpPress(WORD hydrport);
BOOL ChkServoSpdOver(WORD spd);
WORD GetPumpMcModeSet(WORD ServoNo);
void SetVlvData_mn(WORD portno, int value, WORD dir);



extern PPUMP	g_pumpn[MAX_PUMP];
extern UI16		g_pump_num;



#ifdef __cplusplus
}
#endif

#endif
