
#ifndef _HYDRPOSAPI_H_
#define _HYDRPOSAPI_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

//#include    "hydr.h"
#include "com.h"
#include "acttype.h"
#include "ApiCommon.h"


/*---------------------------------------------------------------------------+
|           NewPosControl                                                    |
+---------------------------------------------------------------------------*/
typedef void (*ACT_OP_FUNC_NEW)(void* pp);
typedef void (*ACT_OP_FUNC2_NEW)(void* pp,void* pact);

#define HYDR_POS_MAX	4	//最大定位控制通道数


#define CHKSTOPLOOP_RPM_THRESHOLD	30	//单位rpm	检测电机指令转速低于多少时停止闭环
#define CHKSTOPPO_RPM_THRESHOLD	5		//单位rpm	检测电机指令转速低于多少时关阀
#define STOPDLYTIME_MS	100				//单位ms		电机停止转动后	等待判定的时间
#define CHKSPEEDSTOP_MMS_THRESHOLD 50	//单位0.1mm/s	用于判定动作在未到位时油缸停止的 油缸速度阈值

#define SAFEDOWNRAMP -10		//异常时 安全的减速步幅

#define RPM2CYLINDERSPD	0.003f	// 驱动转速 →油缸速度的转换系数，转速 * 此系数  = 油缸速度，单位(0.1mm/ms)/1RPM
#define ACC_999	999
#define PI 3.1415	//宏定义π
#define MAXHYDRPOSPART	3	//需要液压定位的部件数
#define	MAXNUMSEG		8	//	段数
#define	MAXHYDROUTPORT		2		//	输出量的最大端口数：0=设定转速,1=设定压力

#define DIRECFWD	0	//正向 电子尺变大的方向
#define DIRECBWD	1	//反向 电子尺减小的方向

#define SPDCTRLPORT	0	//速度通道
#define PRESCTRLPORT	1	//压力通道


#define DECMODE_POS	0xF000

#define DECMODE_POS1	1
#define DECMODE_POS2	2
#define DECMODE_POS3	3


typedef enum enHYDRPOS_DECMODE
{
	DECMODE_DEC_OLDRAMP = 0x01,		//使用大斜坡
	DECMODE_SPD			= 0x02,		//使用电子尺速度闭环
	DECMODE_DRAG 		= 0x04,		//结尾使用拖拽方式
	DECMODE_OLD_RAMP 	= 0X08,     //加减速走老斜率
	DECMODE_END_LOOP 	= 0X10,     //结束段闭环	
	DECMODE_CHG_END_POS	= 0X20,     //定位可调节
	DECMODE_SETDATA_LIMIT = 0x40,	//使用外部斜率指令来限制速度及压力
	DECMODE_MAXSPD10000 = 0x80,		//使用10000的最大速度值，使算法数量级与经典电脑的最大值保持一致
	DECMODE_HOLD_ON_SPD = 0x100,	//到达目标位置时，保持当前速度为目标速度
	DECMODE_MAXSPD999   = 0x200,	//使用999的最大速度值，使算法数量级与经典电脑的最大值保持一致
}HYDRPOS_DECMODE;

typedef enum enHYDRPOS_CTRL_MODE
{
	HYDRNOPOS	= 0X00,		//	跳脱控制
	HYDRPOSI	= 0X01,		//	位置模式
	HYDRSPED	= 0X02,		//	速度模式
	HYDRPOSI2	= 0x04,		//	位置模式2
	HYDRSMSTOP	= 0x10,		//	平滑停止
	HYDRSDRAG	= 0X20,		//	拖拽动作模式
}HYDRPOS_CTRL_MODE;


typedef enum enHYDRPOS_CTRL_STEP
{
	HP_STEP0	 = 0X00,		
	HP_STEP1	 = 0X01,		//	
	HP_STEP2	 = 0X02,		//	
	HP_STEP3	 = 0X03,		//	
	HP_STEP4	 = 0X04,		//	
	HP_STEP5	 = 0X05,		//	
	HP_STEP6	 = 0x06,	
	HP_STEPEND 	 = 0xFD,
	HP_STEPENDD  = 0xFE,
	HP_STEPENDD2 = 0xFF,
}HYDRPOS_CTRL_STEP;

int	GetMeterPosByCh(UI8 ch);
UI16 GetAdMeterSpeed(UI16 ch);
UI16 GetSysPresLimit(void);
UI16 G_MAX_LOOP_100US(void);
void ActClsHydr(PACT pact, UI16 pramp, UI16 framp);


typedef struct tyHydrPos
{
	UI16	PosCtrlMode;
  	int		HydrPosSet[MAXNUMSEG]  ;	//	各段设定位置
	int		CurPos;						//当前位置	0.1mm
	int		CurPosLast;					//上次位置	0.1mm
	
	int		StartPres;	//减速开始压力
	int		StartSpd;	//减速开始速度
	int		DecPos;		//实际减速距离

	int		EndPos;						//当前控制目标位置
	int		EndSpd;						//当前控制目标速度
	int		EndPres;					//当前控制目标压力
	int		FinallyEnd;					//终止位置
	UI16	DoLoop;						//使用闭环	
	UI16	DecMode;					//减速模式

    UI16	kp_pos;

    int		MaxControlPos;				//控制的最大距离
    UI16	DragBefPos;					//拖拽前距离
}HydrPos, *PHydrPos;

typedef struct tyHydrCtrl
{
	int		HydrSet[MAXHYDROUTPORT][MAXNUMSEG]  ;	//	设定值
    int 	RampUp[MAXHYDROUTPORT];		//上升斜率	ms
    int 	RampMid[MAXHYDROUTPORT];	//中间斜率	ms
    int 	RampDown[MAXHYDROUTPORT];	//减速斜率	ms
    
    float	RampStep[MAXHYDROUTPORT];	//步幅
    float	RampStepSet[MAXHYDROUTPORT];	//设定步幅
    WORD	DieTime;					//死区时间	ms

	float	HydrOut[MAXHYDROUTPORT];	//输出值	rpm
	UI16	SegCur; 					//当前段数
	UI16	SegSet;						//设定段数
		
	UI16	RunDirec;	//方向 电子尺大方向为正向
	UI16	HydrDlyTimeMs;	//液压延迟时间
	UI16	HydrDlyTimeMs_Start;	//液压延迟时间开始值
	UI16	last_hydr_port;			//开启的油路
	UI16	last_hydr_pri;			//开启的优先级
	
	PPART	pp;
	PACT	pact;

	ACT_OP_FUNC_NEW	StopVlvFunc;	//结束阀函数
	ACT_OP_FUNC2_NEW	Stop2VlvFunc;	//结束压力流量及阀输出函数
	ACT_OP_FUNC2_NEW	StartVlvFunc;	//开底流函数
	UI16		FirstStart;				//首次启动标记	当上升到设定值，或者开始减速时清除标记
	int		endspeed;				//各个动作到位时的终止速度
	int		endpress;				//各个动作到位时的终止压力 0.1bar
}HydrCtrl, *PHydrCtrl;

typedef struct tyHydrStruCtrl
{	
	UI16	HydrPosId;			
	UI16	runstate;			//状态 0:不使用	1:占用
	UI16	HydrControlMode;	//液压控制模式 0x0=跳出控制;0x01=位置;0x02=速度;
	UI16	HydrControlStep;	//液压控制步骤;
	UI16	HydrControlLastStepTm_ms;
	UI16	AdmeterNo;			//电子尺通道号
	float	k_MotortoAD;		//转速对应实际移动速度的公约数
	UI16	MotorMaxRPM;		//驱动器最大转速
	UI16	RealMotorMaxRPM;	//真实设定的最大转速
	
	float	KpAbaseM2;			//
	
	HydrCtrl	HydrCtrlData;	//液压控制速度控制
	HydrPos		PosData;		//液压位置控制
}HydrStruCtrl, *PHydrStruCtrl;

typedef struct tyHydrPosDataKey
{	
	UI16	HydrPosNum;			//定位控制编号
	PHydrStruCtrl	pHydrStruCtrl[HYDR_POS_MAX];	//定位控制通道数据的指针
}HydrPosDataKey, *PHydrPosDataKey;

typedef struct tySTOP_CTRL_PARA
{
	WORD HydrPosId;		//定位控制编号(1开始)
	WORD RunDirec;		//控制方向
	WORD AdNo;			//定位控制用到的反馈位置的电子尺编号(1开始)
	int  PSramp;		//速度停止斜率
	ACT_OP_FUNC_NEW VlvFunc;	//关阀函数指针
	PACT pact;					//act结构体指针
	PPART pp;
}STOP_CTRL_PARA, *PSTOP_CTRL_PARA;


typedef struct tyPOS_CTRL_PROC_PARA
{
	WORD HydrPosId;		//定位控制编号(1开始)
	WORD RunDirec;		//控制方向
	WORD AdNo;			//定位控制用到的反馈位置的电子尺编号(1开始)
	int *PSramp;		//速度斜率组合数据指针(seg段)
	int *Pspd;			//设定速度组合数据指针(seg段)
	int *PPramp;		//压力斜率组合数据指针(seg段)
	int *Ppres;			//设定压力组合数据指针(seg段)
	int *Ppos;			//设定切换位置组合数据指针(seg段)
	WORD seg;			//设定的段数
	WORD dietime;		//死区时间 1ms
	WORD hydrdlytime;	//液压动作延迟时间 1ms
	WORD mode;			//减速模式	参考 HYDRPOS_DECMODE
	int sm;				//底流速度 0.1rpm
	WORD MaxRpm;		//最大速度 0.1rpm
	WORD kpPos;			//位置Kp 默认120
	float k_MotortoAD;	//转速对应实际移动速度的公约数 0.1rpm->0.1mm/s
	int diffpos;
	int movenum;
	int endpress;		//结束压力
}POS_CTRL_PROC_PARA, *PPOS_CTRL_PROC_PARA;


//PUMP_CONTROL_PARA结构体内的数据只支持赋值，不支持API内调用
typedef struct tyPOS_CTRL_PROC_RT_PARA
{
	HANDLE	SetSpd;				//设定转速	0.1rpm
	HANDLE	SetPress;			//设定压力	0.1bar
	HANDLE	SetTorque;
	HANDLE	SetSpdLast;			//上一次系统输出的设定转速	0.1rpm
	HANDLE	SetPressLast;		//上一次系统输出的设定压力	0.1bar
	HANDLE	SetTorqueLast;
	HANDLE	RealPres;			//实际压力
}POS_CTRL_PROC_RT_PARA, *PPOS_CTRL_PROC_RT_PARA;


/*******************************************************************************************
 *Brief			:申请定位控制通道号，在分配时会对该通道申请相应的内存空间。
 *Param			:
 *Return Value	:申请的通道号(1开始)
 *Aauthor		:ZT
 *Date			:20230711
********************************************************************************************/
WORD ApplyHydrPosControl(void);

/*******************************************************************************************
 *Brief			:获得该通道实际设置的最小转速
 *Param			:WORD HydrPosId 定位通道号
 *Return Value	:实际设置的最小转速(按照MotorMaxRPM的单位)
 *Aauthor		:ZT
 *Date			:20240125
********************************************************************************************/
int GetRealEndSpd(WORD HydrPosId);

/*******************************************************************************************
 *Brief			:获得该通道实际设置的最小压力
 *Param			:WORD HydrPosId 定位通道号
 *Return Value	:实际设置的最小压力(0.1bar)
 *Aauthor		:ZT
 *Date			:20240125
********************************************************************************************/
int GetRealEndPress(WORD HydrPosId);


/*******************************************************************************************
 *Brief			:检查对应HydrPosId通道下的定位功能的模式是否为ControlMode
 *Param			:WORD HydrPosId 定位通道号; WORD ControlMode 需要检查的控制模式
 *Return Value	:-1 判定失败；0 不是对应的模式；1 是对应的模式
 *Aauthor		:ZT
 *Date			:20230614
********************************************************************************************/
I16 ChkHydrPosMode(WORD HydrPosId, WORD ControlMode);

/*******************************************************************************************
 *Brief			:设置定位控制参数，数据以指针地址的形式传输
 *Param			:void* ppara 结构体POS_CTRL_PROC_PARA的指针形式传递
 *Return Value	:0:设置失败	1:设置成功
 *Aauthor		:ZT
 *Date			:20230711
********************************************************************************************/
UI16 SetPosCtrlParaTran(void* ppara);

/*******************************************************************************************
 *Brief			:设置平滑停止的控制参数，数据以指针地址的形式传输
 *Param			:void* ppara 结构体 STOP_CTRL_PARA 的指针形式传递
 *Return Value	:0:设置失败	1:设置成功
 *Aauthor		:ZT
 *Date			:20230711
********************************************************************************************/
WORD SmoothStopTran(void* ppara);

/*******************************************************************************************
 *Brief			:结束对应编号的对应定位控制模式，输出0指令，并执行切阀函数。
 *Param			:WORD HydrPosId 	定位通道号(1开始)
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20230711
********************************************************************************************/
void PosControlEnd(WORD portno);


/*******************************************************************************************
 *Brief			:初始化拖拽功能，配置相应的停止动作及再开始动作
 *Param			:WORD HydrPosId 定位通道号(1号)
 				 PACT pact		act结构体的指针
 				 ACT_OP_FUNC2_NEW VlvFuncOff	动作关闭函数地址
 				 ACT_OP_FUNC2_NEW VlvFuncOn		再开始动作函数地址
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20230711
********************************************************************************************/
void InitDragCtrlFunc(UI16 portno, PACT pact, PPART pp, ACT_OP_FUNC2_NEW VlvFuncOff,ACT_OP_FUNC2_NEW VlvFuncOn);

/*******************************************************************************************
 *Brief			:在使用算法后，开模减速结束后，拖拽前的停止位置
 *Param			:WORD HydrPosId 定位通道号(1号)
 *Return Value	:拖拽前的停止位置(0.1mm)
 *Aauthor		:ZT
 *Date			:20230711
********************************************************************************************/
int GetRealLocatPos(WORD portno);

/*******************************************************************************************
 *Brief			:判定是否已经结束定位动作，开始进入拖拽动作了
 *Param			:WORD HydrPosId 	定位通道号(1开始)
 *Return Value	:-1:失败 0:未进入 1:进入
 *Aauthor		:ZT
 *Date			:20230711
********************************************************************************************/
int ChkHydrPosStepOverLoop(WORD portno);

/*******************************************************************************************
 *Brief			:获得定位控制的通道总数
 *Param			:
 *Return Value	:定位通道的总数
 *Aauthor		:ZT
 *Date			:20230711
********************************************************************************************/
WORD GetHydrPosNum(void);

/*******************************************************************************************
 *Brief			:定位控制功能主函数，周期性调用，最终完成规划闭环控制
 *Param			:void* ppara 结构体 PUMP_CONTROL_PARA 的指针地址
 *Return Value	:控制转速(0.1rpm)，如果失败返回0.0rpm
 *Aauthor		:ZT
 *Date			:20230711
********************************************************************************************/
int NewPosControlTran(void* ppara);

/*******************************************************************************************
 *Brief			:获得该通道的最大转速
 *Param			:WORD HydrPosId 定位通道号
 *Return Value	:配置的该通道的最大转速
 *Aauthor		:ZT
 *Date			:20240116
********************************************************************************************/
int GetRealMotorMaxRPM(WORD HydrPosId);

/*******************************************************************************************
 *Brief			:获得定位控制方向
 *Param			:WORD HydrPosId 定位通道号
 *Return Value	:目前的定位控制的控制方向
 *Aauthor		:ZT
 *Date			:20240122
********************************************************************************************/
int GetPosCtrlRunDirec(WORD HydrPosId);

int GetHydrPosPresCommand(WORD HydrPosId, int CurOutPres);
WORD GetHydrPosApiVer(void);



#ifdef __cplusplus
}
#endif

#endif

