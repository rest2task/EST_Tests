/*===========================================================================+
|  Author   : Yanfeng 	                                                     |
|  Version  : V1.00                                                          |
|  Creation : 01/25/2016                                                     |
|  Revision :                                                                |
+===========================================================================*/
#ifndef     __ELECTRLAPP_H
#define     __ELECTRLAPP_H

#include	"typedef.h"
#include	"EtherCATProc.h"
#include  "admeter.h"
#include	"hydr.h"
#include  "servo.h"
#include  "machapi.h"
#include	"machine.h"
#include	"definelist.h"

#define	ELECTRLCYCTIEM	250	/*电动控制的周期时间 250us*/


/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/
int	GetEleSpdRPMByServo(WORD ServoNo);	//获取电动控制的当前输出速度(RPM)
int GetEleSpdMMByServo(UI16 ServoNo);		//获取电动控制的当前输出速度(0.0mm/s)
int GetChargeRPM(void);											//获取储料动作的螺杆转速(RPM)
int GetEleMode(UI16 ServoNo);						//获取从站的当前控制模式
BOOL IsMotoSpeedStop(UI16 ServoNo);			//判断电机是否禁止
WORD	GetSpeedRampByServo(UI16	ServoNo);//获取相应动作的当前速度转换斜率
BOOL	GetPresInsertAllowFlag(WORD ServoNo);//获取当前是否允许压力控制的操作标记
void	SetElecDisplay(WORD ServoNo,WORD ElePres,int ActSpeed);	//设定值的压力速度显示
int	ActSetSpdToRPM(WORD ServoNo,int SetSpeed);								//	动作设定速度转换为RPM
void SetEleOutChargeZero(void);										//设置储料时的螺杆进退速度至0
void SetEleOutChargeRel(void);										//设置储料泄压时的螺杆进退方向
int	HoldDecelPos(WORD HoldSpd,WORD ActOffSpdRamp);	//位置转保压的提前减速位置点计算
BOOL	EleSpdRampOkCheck(WORD ServoNo);							//获取斜率执行完成标记
void	CycleTimeOverRampDeal(void);											//控制周期超时后的斜率操作补偿

BOOL	PositionReachChk(int CurPos,int EndPos,WORD RunDirec);
BOOL	PosCtrlReachChk(WORD ServoNo);
BOOL	PosFixCtrlReachChk(WORD ServoNo,int EndPos);
int GetEndPosPulse(WORD ServoNo);										//获取定位距离
void	SetPosCtrlStop(WORD ServoNo);									//设定定位控制为停止模式
void	SetPosRunState(WORD ServoNo,WORD PosRunState);//设置定位运行的状态
WORD	GetPosRunState(WORD ServoNo);									//获取定位运行的状态

void	SetChargeFlag(WORD Flag);											//设置当前是否处于储料状态
void	PresCtrlAllowChk(WORD ServoNo,WORD CheckNum);	//允许启动压力控制的判断
BOOL	GetPresCtrlAllowFlag(WORD ServoNo);						//获取允许压力控制的标记
BOOL	ElePresOverChk(WORD ServoNo);									//压力控制的超压判断
BOOL	ChargePresOk(void);
BOOL	RealPresOverCheck(void);

WORD GetSlaveLink(void);							//从站连接状态:bit0 bit1...bit31
WORD GetSlaveMissNum(void);						//从站丢失个数
WORD GetPdoOverTimeNum(void);					//pdo连接超时个数
WORD GetEcatNetState(void);						//网络状态
WORD GetRealSlaveNum(void);						//实际从站个数
WORD GetSlaveState(WORD SlaveNo);	//从站状态
BOOL	ECATConfigCheck(void);					//EtherCAT从站配置确认
BOOL	ActPosDecChk(WORD ServoNo);
BOOL ECATStateCheck(void);
int RPMMAX(WORD ServoNo);
int	ActSetSpdToPulse(WORD ServoNo,UI16 SetSpeed);
BOOL	PosCtrlReachChk2(WORD ServoNo,int EndPosPulse);
int GetEleSpdMMByServoS(UI16 ServoNo);
UI16 GetRealEcatSlaveNum(void);

#define	ECAT_SLV_NUM	GetRealEcatSlaveNum()


#endif
