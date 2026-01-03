/*===========================================================================+
|  Author   : Yanfeng 	                                                     |
|  Version  : V1.00                                                          |
|  Creation : 09/25/2014                                                     |
|  Revision :                                                                |
+===========================================================================*/
#ifndef     __ETHERCATPROC_H
#define     __ETHERCATPROC_H

#include	"typedef.h"

#define ELESERVONUM 6

//返回不等于0时，对应位为对应驱动器状态
typedef enum enSERVO_STATEFLAG
{
	SERVOST_MODEERR = 0x01,	//返回0:代表无模式错误
	SERVOST_NOVOL	= 0x02,	//返回0:代表主线回路压力正常
	SERVOST_START	= 0x04,	//返回0:代表都是初始状态
	SERVOST_QUICK	= 0x08,	//返回0:代表都是可以开启伺服运行状态
	SERVOST_FAULT	= 0x10,	//返回0:代表无故障
	SERVOST_RDY		= 0x20,	//返回0:代表是准备状态
	SERVOST_ENABLED	= 0x40	//返回0:代表驱动器未使能
}SERVO_STATEFLAG;


typedef struct tyRPDO
{
	int		SpeedCmd;			//	速度指令
	WORD	ForTorque;		//	正向转矩
	WORD	RevTorque;		//	反向转矩

	WORD	CtrlWord;
	WORD	HydrSetPres;
	WORD	HydrSetFlow;
	WORD	HydrRealPrs;

	WORD	pumplist;	//从站跟随驱动
}PDOSEND,*PPDOSEND;

typedef struct tyTPDO
{
	int		RealEncode;		//	实际位置：脉冲计数
	int		RealSpeed;		//	实际速度:0.1rpm
	int		ServoSetSpeed;	//	伺服指令速度
	I16		RealTorque;		//	实际转矩
	WORD	AlarmPara;		//	警报参数
	WORD	ZeroFlag;		//	归零标记	
	WORD	ServoTemp;		//驱动器温度
	WORD	RealPI;			//实际电流
	WORD	RealPV;			//实际电压
	WORD	ServoFlag;		//伺服标记
	WORD	ServoTime;		//从站时钟配置完成标记
	
	int		RealPowr;		//实际功率
	WORD	ServoOverload;	//伺服过载信号	该值大于等于35000时电机过载。
	WORD	ServoBusVoltage;//伺服母线电压
	WORD	RealPres;		//实际压力反馈
	WORD	ServoState;		//通讯状态
	WORD	MotorTemp;		//电机温度
}PDORECV,*PPDORECV;



extern	PDOSEND	PdoSendData[ELESERVONUM];
extern	PDORECV	PdoRecvData[ELESERVONUM];


extern	WORD 	EcatSdoSendData[6*2][32];	//slaves 4 to 8 
extern	WORD 	EcatSdoRecvData[6*2][32]; //slaves 4 to 8 


/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/
void	InitEcatData(void);

int	GetPulsePosByServoNo(WORD	ServoNo);
int GetRealPulsePosByServoNo(WORD	ServoNo);

int	GetRealSpeedByServoNo(WORD	ServoNo);
WORD GetZeroFlagByServoNo(WORD ServoNo);
WORD GetSlaveZeroFlag(WORD SlaveNo);
int GetSERVO_POWER(WORD SlaveNo);
WORD GetServoIsEnable(WORD SlaveNo);
UI16 GetServoErr(WORD SlaveNo);
BOOL IsBusVOver(WORD V);
BOOL IsBusVLower(WORD V);

void	EcatProc(void);
void	EcatOPProc(void);

UI16 GetSERVO_CURRENT2(WORD Servo);
int GetSpdRPM_SpdPulse(WORD ServoNo,int ActSpdPulse);

void ServoStateRecord(void);

void	SetEcatSdoSend(WORD	ServoNo,WORD Index,WORD SubIndex,DWORD SdoData,WORD DataLenth);
void	SetEcatSdoRecv(WORD	ServoNo,WORD Index,WORD SubIndex,WORD DataLenth);
BOOL EcatSdoRecvChk(WORD	ServoNo);
DWORD	GetSdoRecv(WORD	ServoNo,WORD NUM);
BOOL EcatSdoChk(WORD	ServoNo);
void SetEcatOpFlag(DWORD flag_ecat_op);
void EcatReceive(void);
BOOL EcatCtrlOper(void);
DWORD GetEcatOpFlag(void);
void EcatSend(void);
I16 GetRealTorque(WORD SlaveNo);
int GetRPMByServoNo(WORD	ServoNo);

UI8 ChkServoState(SERVO_STATEFLAG st);
void SetServoCtrl(WORD ServoNo,WORD CtrlWord);



#endif
