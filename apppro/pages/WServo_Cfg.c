#include "WServo_Cfg.h"
#include "typedef.h"
#include "pagefram.h"
#include "information.h"
#include "alarmrecord.h"
#include "database.h"
#include "servo.h"
#include "canmod.h"
#include "texttbl.h"
#include "buttonwgt.h"
#include "oprintf.h"
#include "editwgt.h"
#include "key.h"
#include "privdef.h"
#include "stdlib.h"

DEFINE_SERVO_CFG_MAP

CONNECT_SERVO_CFG_EVENT

#define TMP_SERVO_DATAADDR       TMPVAL(1)    //驱动数据起始地址  临时变量1
#define TMP_SERVO_RATEDPOWER     TMPVAL(3)    //额定功率          临时变量3
#define TMP_SERVO_RATEDVOLT      TMPVAL(4)    //额定电压          临时变量4
#define TMP_SERVO_RATEDCURR      TMPVAL(5)    //额定电流          临时变量5
#define TMP_SERVO_RATEDROTAT     TMPVAL(7)    //额定转速          临时变量7
#define TMP_SERVO_MAXROTAT       TMPVAL(8)    //最大转速          临时变量8

#define TMP_SERVO_DRIVEMD	   TMPVAL(188)   //驱动模式 临时变量188
#define TMP_SERVO_SERVOID	   TMPVAL(450)   //伺服驱动ID 临时变量450

#define TMP_SERVO_MOTORBRAND   TMPVAL(302)  //电机品牌
#define TMP_SERVO_MOTORTYPE    TMPVAL(301)  //电机型号
#define TMP_SERVO_SELFSTATE    TMPVAL(305)  //自学习状态
#define TMP_SERVO_SELFSTEP     TMPVAL(300)  //自学习步奏提示

#define TMP_SERVO_SERVOTYPE	     TMPSTR(10)   //驱动机型 临时字符串变量10


#define SERVO_MOTORBRAND       0x331300d1  //电机品牌
#define SERVO_PARRESETOK       0x331300d2  //驱动器参数重置对话框确认
#define SERVO_SELFLEARN        0x331300d3  //伺服自学习确认
#define SERVO_SELFLEARNSWITCH  0x331300d4  //伺服自学习开关
#define SERVO_MINROTATINGSPD   0x331300d5  //最大转速下限
#define SERVO_MAXROTATINGSPD   0x331300d6  //最大转速上限
#define SERVO_EVERYSELECT      0x331300d7  //伺服自学习各选项
#define SERVO_RATEDSET         0x331300d8  //额定数据是否允许修改
#define SERVO_MAXROTATIONGSET  0x331300d9  //最大转速是否允许修改
#define SERVO_SELFLEARNPROMPT  0x331300da  //自学习步奏或警报提示

#define MOTORBRAND(addr,Id)   ((VarAdrToUI16((UI32)(addr))&(1 << ((UI8)Id-1)))>0?1:0)

//page dialog define
#define PAGE_DRIVEPARRESET    "Servo_Ask2"  //驱动器参数重置询问框
#define PAGE_SELFLEARNDYNAMIC "Servo_Ask1"  //动态自学习询问框
#define PAGE_SELFLEARNSTATIC  "Servo_Ask5"  //静态自学习询问框
#define PAGE_SELFLEARN5MODE   "Servo_Ask4"  //自学习成功5模式询问框
#define PAGE_SELFLEARN3MODE   "Servo_Ask6"  //自学习成功3模式询问框


#define ALARM_EMERGENCYSTOP    0x100101     //警报：紧急停止

extern UI8 nServoId;
extern UI8 wHelp;
extern UI32 g_warm1,g_warm2;

extern UI16 Phase_ServoxFixedValue[][5];
extern UI16 Inova_ServoxFixedValue[][5];

//驱动器参数重置确认条件
extern BOOL bDriveParResetConfirm;
//界面切换确认条件
extern BOOL bSwitchPageConfirm;

//自学习方式 0 静态 1动态
static UI16 bDynamicLearn =0; 

//自学习状态 0-无,1-自学习中,2-自学习被中断,3-自学习成功,4-自学习失败
//static UI16 flSelfLearnState =0;
//自学习上次状态 0-无,1-自学习中,2-自学习被中断,3-自学习成功,4-自学习失败
static I16 flSelfLearnLastState = -1;
//马达状态 
static I16 flMotorStatus =-1;
//电机运行状态
static I16 flMotorRunning =-1;
//驱动器模式
static I16 flServoMode =-1;
//电机型号
static I16 cMotorType =-1;
//驱动器版本
static I16 nServoId_Version =-1;
//电机品牌
static I16 wMotorBrand=-1;

//
static UI32 time_3s = 0;

UI32 time_1500ms=0;

/************************************************************************/
/* 自学习各伺服数据读取                                                 */
/************************************************************************/
static void SetServo()
{
	Send_TaskCan(CAN_TX_READ,nServoId,0xE000,0);
	Send_TaskCan(CAN_TX_READ,nServoId,0xF70A,0);
	Send_TaskCan(CAN_TX_READ,nServoId,0xF101,0);
	Send_TaskCan(CAN_TX_READ,nServoId,0xF102,0);
	Send_TaskCan(CAN_TX_READ,nServoId,0xF103,0);
	Send_TaskCan(CAN_TX_READ,nServoId,0xF105,0);
	Send_TaskCan(CAN_TX_READ,nServoId,0xF00A,0);

	Send_TaskCan(CAN_TX_READ,nServoId,0xF10B,0);
	Send_TaskCan(CAN_TX_READ,nServoId,0xF10C,0);
	Send_TaskCan(CAN_TX_READ,nServoId,0xF10D,0);
	Send_TaskCan(CAN_TX_READ,nServoId,0xF10E,0);
	Send_TaskCan(CAN_TX_READ,nServoId,0xF10F,0);

	Send_TaskCan(CAN_TX_READ,nServoId,0xF20D,0);
	Send_TaskCan(CAN_TX_READ,nServoId,0xF20E,0);
	Send_TaskCan(CAN_TX_READ,nServoId,0xF20F,0);
	Send_TaskCan(CAN_TX_READ,nServoId,0xF210,0);

	Send_TaskCan(CAN_TX_READ,nServoId,0xA102,0);
	Send_TaskCan(CAN_TX_READ,nServoId,0xA005,0);

	VarAdrSetInt(TMP_SERVO_MOTORTYPE,g_dbsvotype.cServoType[nServoId-1]);

	//nSecond = 0;
	time_3s = GetTick();
}
/************************************************************************/
/* 自学习各状态标记                                                     */
/************************************************************************/
static void ServoResetStatus()
{
	VarAdrSetInt(SERVO_PARRESETOK,0);
	VarAdrSetInt(SERVO_SELFLEARN,0);
	bSwitchPageConfirm = FALSE;
	bDriveParResetConfirm=FALSE;

}
/************************************************************************/
/* 自学习页面进入                                                       */
/************************************************************************/
static void ServoPageIn(Servo_Cfg_PG* ppg)
{
	VarAdrSetInt(TMP_SERVO_SELFSTATE,bDynamicLearn);//自学习状态
	VarAdrSetInt(TMP_SERVO_MOTORBRAND,MOTORBRAND(SERVO_MOTORBRAND,nServoId));//电机品牌
	VarAdrSetInt(TMP_SERVO_SERVOID,nServoId-1);
	VarAdrSetInt(TMP_SERVO_MOTORTYPE,g_dbsvotype.cServoType[nServoId-1]);
	VarAdrSetStr(TMP_SERVO_SERVOTYPE,"");//驱动型号

	// flag init
	flMotorStatus         = -1;
	flMotorRunning        = -1;
	flServoMode           = -1;
	cMotorType            = -1;
	wMotorBrand           = -1;
	flSelfLearnLastState  = -1;
	nServoId_Version      = -1;
	VarAdrSetInt(SERVO_SELFLEARNSWITCH,0);
	VarAdrSetInt(TMP_SERVO_SELFSTEP,0);
	ServoResetStatus();
//20181019hz
	if (VarAdrToInt(TMP_SERVO_MOTORBRAND) == 1)
	{
		WGTSetVisible(ppg->dropdown10, TRUE);
		WGTSetVisible(ppg->dropdown8, FALSE);
	} 
	else
	{
		WGTSetVisible(ppg->dropdown10, FALSE);
		WGTSetVisible(ppg->dropdown8, TRUE);
	}

	//servo data require
	SetServo();
}
/************************************************************************/
/* 驱动器参数重置                                                       */
/************************************************************************/
static BOOL DriveParReset()
{
	if(dbsvox[nServoId].wData[ID_3000] == 1 || dbsvox[nServoId].wData[ID_3000] == 2){
		/*电机正在运行中...*/
		ShowMsg(VW_MSG_MOTORISRUNNING);//31

		return FALSE;
	}
	if(dbsvox[nServoId].wData[ID_E000] != 0) {
		/*驱动模式错误，请先转为调试模式*/
		ShowMsg(VW_MSG_CHANGESERVOMODETODEBUG);//32

		return FALSE;
	}
	ServoResetStatus();
	bDriveParResetConfirm = TRUE;
	PanelShowDialogByName(PAGE_DRIVEPARRESET);

	return TRUE;
}
/************************************************************************/
/* 驱动器调试须知                                                       */
/************************************************************************/
static void DriveDebugStep()
{
	wHelp =0;
	PanelShowPageByName(PAGE_SERVOHELP);
}

/************************************************************************/
/* 电机品牌选择                                                         */
/************************************************************************/
static BOOL MotorTypeSel()
{
	UI16 wpos,wValue;
	PVAR pdata;

	if(dbsvox[nServoId].wData[ID_3000]==1 || dbsvox[nServoId].wData[ID_3000]==2)
		return TRUE;
	if(dbsvox[nServoId].wData[ID_E000]){
		/*驱动器控制模式错误*/
		ShowMsg(VW_MSG_DRIVERMODEERR);//30
		return FALSE;
	}
 
	wValue = VarAdrToUI16(TMP_SERVO_MOTORBRAND);
	if(wValue)
	{
		VarAdrSetInt(SERVO_MOTORBRAND,((VarAdrToUI16(SERVO_MOTORBRAND) | (1 << (nServoId-1)))));
	}
	else 
	{
		VarAdrSetInt(SERVO_MOTORBRAND,((VarAdrToUI16(SERVO_MOTORBRAND) & (~(1 << (nServoId-1))))));
	}
	if(pdata=VarAdrToVar(SERVO_MOTORBRAND))
		VarSaveInt(pdata);

	g_dbsvotype.cServoType[nServoId-1] = 0;
	wpos =(UI16)((UI32)&g_dbsvotype.cServoType[nServoId-1]-(UI32)&g_dbsvotype);
	SaveServoType(wpos,&g_dbsvotype.cServoType[nServoId-1],sizeof(g_dbsvotype.cServoType[nServoId-1]));

	return TRUE;
}
/************************************************************************/
/* 菲仕电机选择                                                         */
/************************************************************************/
static BOOL PhaseMotorSel()
{
	UI8 i;
	UI16 wValue,wMaxValue;
	UI16 wAddr,wpos;

	if(dbsvox[nServoId].wData[ID_3000]==1 || dbsvox[nServoId].wData[ID_3000]==2) 
		return TRUE;
	if(dbsvox[nServoId].wData[ID_E000]){
		/*驱动器控制模式错误*/
		ShowMsg(VW_MSG_DRIVERMODEERR);
		//VarAdrSetInt(INFORMATION_INDEX,30);
		//PanelShowDialogByName(PAGE_INFORMATION);
		return FALSE;
	}
	wValue = VarAdrToUI16(TMP_SERVO_MOTORTYPE);
	if (wValue){
		wMaxValue=Phase_ServoxFixedValue[wValue-1][3]*11/10;
		Send_TaskCan(CAN_TX_WRITE,nServoId,0xF00A,wMaxValue);
		Send_TaskCan(CAN_TX_WRITE,nServoId,0xF00C,wMaxValue);
		Send_TaskCan(CAN_TX_READ, nServoId,0xF00A,0);
		wAddr=0xF101;
		for(i=0;i<5;i++){
			Send_TaskCan(CAN_TX_WRITE,nServoId,wAddr,Phase_ServoxFixedValue[wValue-1][i]);
			Send_TaskCan(CAN_TX_READ,nServoId,wAddr++,0);
		}
	}

	g_dbsvotype.cServoType[nServoId-1] = wValue;
	wpos =(UI16)((UI32)&g_dbsvotype.cServoType[nServoId-1]-(UI32)&g_dbsvotype);
	SaveServoType(wpos,&g_dbsvotype.cServoType[nServoId-1],sizeof(g_dbsvotype.cServoType[nServoId-1]));

	//nSecond = 0;
	time_3s = GetTick();

	return TRUE;
}

/************************************************************************/
/* 汇川电机选择                                                         */
/************************************************************************/
static BOOL InovanceMotorSel()
{
	UI8 i;
	UI16 wValue,wMaxValue;
	UI16 wAddr,wpos;

	if(dbsvox[nServoId].wData[ID_3000]==1 || dbsvox[nServoId].wData[ID_3000]==2) 
		return TRUE;
	if(dbsvox[nServoId].wData[ID_E000]) {
		/*驱动器控制模式错误*/
		ShowMsg(VW_MSG_DRIVERMODEERR);
		return FALSE;
	}

	wValue = VarAdrToUI16(TMP_SERVO_MOTORTYPE);
	if (wValue){
		wMaxValue=Inova_ServoxFixedValue[wValue-1][3]*11/10;
		Send_TaskCan(CAN_TX_WRITE,nServoId,0xF00A,wMaxValue);
		Send_TaskCan(CAN_TX_WRITE,nServoId,0xF00C,wMaxValue);
		Send_TaskCan(CAN_TX_READ, nServoId,0xF00A,0);
		wAddr=0xF101;
		for(i=0;i<5;i++){
			Send_TaskCan(CAN_TX_WRITE,nServoId,wAddr,Inova_ServoxFixedValue[wValue-1][i]);
			Send_TaskCan(CAN_TX_READ,nServoId,wAddr++,0);
		}
	}

	g_dbsvotype.cServoType[nServoId-1] = wValue;
	wpos =(UI16)((UI32)&g_dbsvotype.cServoType[nServoId-1]-(UI32)&g_dbsvotype);
	SaveServoType(wpos,&g_dbsvotype.cServoType[nServoId-1],sizeof(g_dbsvotype.cServoType[nServoId-1]));

	//nSecond = 0;
	time_3s = GetTick();

	return TRUE;
}
/************************************************************************/
/* 额定功率修改                                                         */
/************************************************************************/
static void RatedPowerChg()
{
	UI16 value;

	value =  VarAdrToUI16(TMP_SERVO_RATEDPOWER);
	Send_TaskCan(CAN_TX_WRITE,nServoId,0xF101,value);
	Send_TaskCan(CAN_TX_READ ,nServoId,0xF101,0);
}
/************************************************************************/
/* 额定电压修改                                                         */
/************************************************************************/
static void RatedVoltChg()
{
	UI16 value;

	value =  VarAdrToUI16(TMP_SERVO_RATEDVOLT);
	Send_TaskCan(CAN_TX_WRITE,nServoId,0xF102,value);
	Send_TaskCan(CAN_TX_READ ,nServoId,0xF102,0);
}
/************************************************************************/
/* 额定电流修改                                                         */
/************************************************************************/
static void RatedCurrentChg()
{
	UI16 value;

	value =  VarAdrToUI16(TMP_SERVO_RATEDCURR);
	Send_TaskCan(CAN_TX_WRITE,nServoId,0xF103,value);
	Send_TaskCan(CAN_TX_READ ,nServoId,0xF103,0);
}
/************************************************************************/
/* 额定转速修改                                                         */
/************************************************************************/
static void RatedRotateChg()
{
	UI16 value,max_value;

	value = VarAdrToUI16(TMP_SERVO_RATEDROTAT);
	max_value=(UI16)(value*110/15.0+0.5);
	Send_TaskCan(CAN_TX_WRITE,nServoId,0xF00A,max_value);
	Send_TaskCan(CAN_TX_WRITE,nServoId,0xF00C  ,max_value);
	Send_TaskCan(CAN_TX_READ ,nServoId,0xF00A,0);
	Send_TaskCan(CAN_TX_WRITE,nServoId,0xF104,(UI16)(value*100/15.0+0.5));
	Send_TaskCan(CAN_TX_READ ,nServoId,0xF104,0);
	Send_TaskCan(CAN_TX_WRITE,nServoId,0xF105,value);
	Send_TaskCan(CAN_TX_READ, nServoId,0xF105,0);
}
/************************************************************************/
/* 最大转速修改                                                         */
/************************************************************************/
static void MaxRotateChg()
{
	UI16 value;

	value = VarAdrToUI16(TMP_SERVO_MAXROTAT);
	value=(UI16)(value*100/15.0+0.5);
	Send_TaskCan(CAN_TX_WRITE,nServoId,0xF00A,value);//IDE_F00A
	Send_TaskCan(CAN_TX_WRITE,nServoId,0xF00C,value);
	Send_TaskCan(CAN_TX_READ,nServoId,0xF00A,0);
}
/************************************************************************/
/* 自学习模式选择                                                       */
/************************************************************************/
static void SelfLearnSel()
{
	UI16 value;

	value = VarAdrToUI16(TMP_SERVO_SELFSTATE);
	if(value)
	{
		bDynamicLearn = TRUE;
	}
	else
	{
		bDynamicLearn = FALSE;
	}
}
/************************************************************************/
/* 自学习开始或自学习停止                                               */
/************************************************************************/
static BOOL ServoSelfLearn()
{

	if (VarAdrToUI16(SERVO_SELFLEARNSWITCH)) //flSelfLearnSwitch
	{
		VarAdrSetInt(SERVO_SELFLEARNSWITCH,0);//flSelfLearnSwitch = FALSE;
		VarAdrSetInt(TMP_SERVO_SELFSTEP,2);//自学习步骤flSelfLearnState  = 2;
		Send_TaskCan(CAN_TX_WRITE,nServoId,0xF002,0);
		Send_TaskCan(CAN_TX_WRITE,nServoId,0x2100,5);
	}
	else
	{
		if(dbsvox[nServoId].wData[ID_8000])
		{
			/*驱动器有报警,请先排除!*/
			ShowMsg(VW_MSG_MOTORALARM);//33
			return FALSE;
		}
		if(VarAdrToUI16(MOTOR_STATE_STATE))
		{
			/*请先关闭电机*/
			ShowMsg(VW_MSG_CLOSEMOTOR);//34
			return FALSE;
		}
		if(dbsvox[nServoId].wData[ID_3000] == 1 || dbsvox[nServoId].wData[ID_3000] == 2)
		{
			/*电机正在运行中...*/
			ShowMsg(VW_MSG_MOTORISRUNNING);//31
			return FALSE;
		}
		if(dbsvox[nServoId].wData[ID_E000] != 0) 
		{
			/*驱动模式错误，请先转为调试模式*/
			ShowMsg(VW_MSG_CHANGESERVOMODETODEBUG);//32
			return FALSE;
		}

		ServoResetStatus();
		VarAdrSetInt(SERVO_SELFLEARN,1);//bSelfLearnConfirm = TRUE;

		if(bDynamicLearn)
		{
			PanelShowDialogByName(PAGE_SELFLEARNDYNAMIC);
		}
		else
		{
			PanelShowDialogByName(PAGE_SELFLEARNSTATIC);
		}

	}
	return TRUE;
}
/************************************************************************/
/* 自学习页面刷新                                                       */
/************************************************************************/
static void ServoPageUpdate(Servo_Cfg_PG* ppg)
{
	UI16 i;
	//static UI32 time_1500ms=0;
	static UI16 speed_max =0;//最大转速上限
	static UI16 speed_min =0;//最大转速下限
	static UI8  nId=0;
	static UI16 max_speed=0;//最大转速

	//drive data require
	if((GetTick()-time_3s)>3000)
	{
		time_3s = GetTick();
		Send_TaskCan(CAN_TX_READ,nServoId,0xF70A,0);
		Send_TaskCan(CAN_TX_READ,nServoId,0xE000,0);
		Send_TaskCan(CAN_TX_READ,nServoId,0x3000,0);
		Send_TaskCan(CAN_TX_READ,nServoId,0x8000,0);	
	}

	if(nId!=nServoId)
	{
		nId = nServoId;
		speed_min =0;
		speed_max =0;
		max_speed = 0;
	}
	//drive data setting
	for(i=0;i<SERVOXADDR_COUNT;i++)	
	{
		if(i==7)
		{
			if((dbsvox[nServoId].wData[i]-max_speed)>10 || max_speed==0)
			{
				max_speed =dbsvox[nServoId].wData[i];
				VarAdrSetInt(TMP_SERVO_DATAADDR+i,max_speed);
			}
		}
		else
		{
			VarAdrSetInt(TMP_SERVO_DATAADDR+i,dbsvox[nServoId].wData[i]);
		}
	}
	VarAdrSetInt(TMP_SERVO_DATAADDR+ID_F00A,(UI16)(dbsvox[nServoId].wData[ID_F00A]*15/100.0+0.5));

	//max_rotating spd min and max
	if(abs(dbsvox[nServoId].wData[ID_F105]-speed_min)>10 || (speed_min==0))
	{
		speed_min = dbsvox[nServoId].wData[ID_F105];
		VarAdrSetInt(SERVO_MINROTATINGSPD,dbsvox[nServoId].wData[ID_F105]);
	}
	if(abs((int)((dbsvox[nServoId].wData[ID_F105]*1.35)-speed_max))>10 || (speed_max==0))
	{
		speed_max = (UI16)(dbsvox[nServoId].wData[ID_F105]*1.35);
		VarAdrSetInt(SERVO_MAXROTATINGSPD,(UI16)(dbsvox[nServoId].wData[ID_F105]*1.35));
	}

	//自学习时检测到急停键
	if(VarAdrToUI16(SERVO_SELFLEARNSWITCH) && (g_warm1 == 0x100101 || g_warm2 == 0x100101))
	{
		VarAdrSetInt(SERVO_SELFLEARNSWITCH,0);//flSelfLearnSwitch = FALSE;
		Send_TaskCan(CAN_TX_WRITE,nServoId,0xF002,0);
		Send_TaskCan(CAN_TX_WRITE,nServoId,0x2100,5);
		VarAdrSetInt(TMP_SERVO_SELFSTEP,0);//flSelfLearnState  = 0;
		/*自学习被紧急停止*/
		ShowMsg(VW_SERVO_SELFLEARN_EMERGENCY_STOP);//35
	}
	//自学习成功判定
	else if(VarAdrToUI16(SERVO_SELFLEARNSWITCH)
		&& (dbsvox[nServoId].wData[ID_8000] || (dbsvox[nServoId].wData[ID_3000]==3 && ((GetTick()-time_1500ms)>3000))))
	{
		//OWARN("learn=%d  nServoId=%d wdata=%d",VarAdrToUI16(SERVO_SELFLEARNSWITCH),nServoId,dbsvox[nServoId].wData[ID_8000]);
		VarAdrSetInt(SERVO_SELFLEARNSWITCH,0);//flSelfLearnSwitch = FALSE;	
		SetServo();
		if(dbsvox[nServoId].wData[ID_8000] == 0)
		{
			VarAdrSetInt(TMP_SERVO_SELFSTEP,3);//flSelfLearnState  = 3
			ServoResetStatus();
			bSwitchPageConfirm = TRUE;
			if((VarAdrToUI16(SYS_FL_MACH_CODE0) & 0x04) ==0 && (VarAdrToUI16(SYS_FL_MACH_CODE54) & 0x1020))
			{
				PanelShowDialogByName(PAGE_SELFLEARN5MODE);
			}
			else 
			{
				PanelShowDialogByName(PAGE_SELFLEARN3MODE);
			}
		}
		else
		{
			VarAdrSetInt(TMP_SERVO_SELFSTEP,0);//flSelfLearnState =0;
			/*电机自学习失败*/
			ShowMsg(VW_SERVO_SELFLEARNFAILURE);//36
		}
	}

	//servo self learn button
	if(VarAdrToUI16(SERVO_SELFLEARNSWITCH))
	{
		ButtonSetText(ppg->button12,GetTextTran(TEXT_STOPLEARN));
		// /WGTSetFocus(ppg->button12);
		//if(trans = GetMapTran(pmap_texttbl,TEXT_STOPLEARN))
		//	VarAdrSetStr(TMP_SERVO_SELFLEARN,trans);
	}
	else
	{
		ButtonSetText(ppg->button12,GetTextTran(TEXT_STARTLEARN));
		//if(trans = GetMapTran(pmap_texttbl,TEXT_STARTLEARN))
		//	VarAdrSetStr(TMP_SERVO_SELFLEARN,trans);
	}

	if ( flMotorStatus !=  VarAdrToUI16(MOTOR_STATE_STATE) || flMotorRunning != dbsvox[nServoId].wData[ID_3000]
	|| flServoMode != dbsvox[nServoId].wData[ID_E000] || wMotorBrand != MOTORBRAND(SERVO_MOTORBRAND,nServoId) || cMotorType !=  g_dbsvotype.cServoType[nServoId-1]
	|| VarAdrToUI16(TMP_SERVO_SELFSTEP) != flSelfLearnLastState
		|| nServoId_Version != dbsvox[nServoId].wData[ID_F70A] )
	{
		//电机参数 -  电机停止,马达关闭，驱动器模式＝0，自定义，非自学习状态
		if (dbsvox[nServoId].wData[ID_3000]==1 || dbsvox[nServoId].wData[ID_3000]==2
			|| VarAdrToUI16(MOTOR_STATE_STATE)
			|| dbsvox[nServoId].wData[ID_E000]
		|| g_dbsvotype.cServoType[nServoId-1]|| VarAdrToUI16(TMP_SERVO_SELFSTEP) == 1 )
		{
			VarAdrSetInt(SERVO_RATEDSET,1); //额定数据是否允许修改
            EditSetBKColor(ppg->edit1,LIGHT_GREEN);
			EditSetTextColor(ppg->edit1,0xffffff);

            EditSetBKColor(ppg->edit5,LIGHT_GREEN);
			EditSetTextColor(ppg->edit5,0xffffff);

            EditSetBKColor(ppg->edit6,LIGHT_GREEN);
			EditSetTextColor(ppg->edit6,0xffffff);

            EditSetBKColor(ppg->edit55,LIGHT_GREEN);
			EditSetTextColor(ppg->edit55,0xffffff);
			
		}
		else
		{
			VarAdrSetInt(SERVO_RATEDSET,0); //额定数据是否允许修改
			EditSetBKColor(ppg->edit1,0xffffff);
			EditSetTextColor(ppg->edit1,0x000000);

			EditSetBKColor(ppg->edit5,0xffffff);
			EditSetTextColor(ppg->edit5,0x000000);

			EditSetBKColor(ppg->edit6,0xffffff);
			EditSetTextColor(ppg->edit6,0x000000);

			EditSetBKColor(ppg->edit55,0xffffff);
			EditSetTextColor(ppg->edit55,0x000000);
		}
		//最大转速 - 电机停止,马达关闭，非自学习状态
		if (dbsvox[nServoId].wData[ID_3000]==1 || dbsvox[nServoId].wData[ID_3000]==2
			|| VarAdrToUI16(MOTOR_STATE_STATE) || VarAdrToUI16(TMP_SERVO_SELFSTEP) == 1)
		{
			VarAdrSetInt(SERVO_MAXROTATIONGSET,1);//最大转速是否变灰
            EditSetBKColor(ppg->edit8,LIGHT_GREEN);
			EditSetTextColor(ppg->edit8,0xffffff);
		}
		else
		{
			VarAdrSetInt(SERVO_MAXROTATIONGSET,0);//最大转速是否变灰

			EditSetBKColor(ppg->edit8,0xffffff);
			EditSetTextColor(ppg->edit8,0x000000);
		}

		//设备识别码 -   非自学习状态
		//hwnd = GetIDCWindow(IDE_SERVOID);
		if(VarAdrToUI16(TMP_SERVO_SELFSTEP) == 1)
		{
			WGTSetEnable(ppg->dropdown2,FALSE);
			//VarAdrSetInt(SERVO_EVERYSELECT,0);//各选项是否变灰
		}
		else
		{
			WGTSetEnable(ppg->dropdown2,TRUE);
			//VarAdrSetInt(SERVO_EVERYSELECT,1);//各选项是否变灰
		}
		//驱动器模式 -   电机停止，马达关闭，非自学习状态
		//hwnd = GetIDCWindow(IDE_E000);
		if (dbsvox[nServoId].wData[ID_3000]==1 || dbsvox[nServoId].wData[ID_3000]==2
			|| VarAdrToUI16(MOTOR_STATE_STATE)  || VarAdrToUI16(TMP_SERVO_SELFSTEP) == 1 )
		{
			//VarAdrSetInt(SERVO_EVERYSELECT,0);//各选项是否变灰
			WGTSetEnable(ppg->dropdown3,FALSE);
		}
		else
		{
			WGTSetEnable(ppg->dropdown3,TRUE);
			//VarAdrSetInt(SERVO_EVERYSELECT,1);//各选项是否变灰
		}
		//驱动器模式
		//if (flServoMode != dbsvox[nServoId].wData[ID_E000])
		//{
		//pwnd = GetWindow(hwnd);
		//((QComboBox*)(pwnd->ctrl))->setCurrentIndex(::GetParamValue(WPARAM1 + ID_E000));
		//VarAdrSetInt()
		//}
		//电机品牌
		//hwnd = GetIDCWindow(IDE_MOTORBRAND);
		if(wMotorBrand != MOTORBRAND(SERVO_MOTORBRAND,nServoId))
		{
			VarAdrSetInt(TMP_SERVO_MOTORBRAND,MOTORBRAND(SERVO_MOTORBRAND,nServoId));
		}
		//电机品牌型号 -  电机停止,马达关闭，驱动器模式＝0，非自学习状态
		if (dbsvox[nServoId].wData[ID_3000]==1 || dbsvox[nServoId].wData[ID_3000]==2
			|| VarAdrToUI16(MOTOR_STATE_STATE)
			|| dbsvox[nServoId].wData[ID_E000] || VarAdrToUI16(TMP_SERVO_SELFSTEP) == 1 )
		{
			WGTSetEnable(ppg->dropdown9,FALSE);
			VarAdrSetInt(SERVO_EVERYSELECT,0);//各选项是否变灰
		}
		else
		{
			WGTSetEnable(ppg->dropdown9,TRUE);
			VarAdrSetInt(SERVO_EVERYSELECT,1);//各选项是否变灰
		}
		//电机型号
		if (cMotorType != g_dbsvotype.cServoType[nServoId-1]) 
		{
			VarAdrSetInt(TMP_SERVO_MOTORTYPE,g_dbsvotype.cServoType[nServoId-1]);
		}

		if (nServoId_Version != dbsvox[nServoId].wData[ID_F70A]) 
		{
			SetServo();
		}

		flMotorStatus        = VarAdrToUI16(MOTOR_STATE_STATE);
		flMotorRunning       = dbsvox[nServoId].wData[ID_3000];
		flServoMode          = dbsvox[nServoId].wData[ID_E000];
		wMotorBrand          = MOTORBRAND(SERVO_MOTORBRAND,nServoId);
		cMotorType           = g_dbsvotype.cServoType[nServoId-1];
		flSelfLearnLastState = VarAdrToUI16(TMP_SERVO_SELFSTEP);
		nServoId_Version     = dbsvox[nServoId].wData[ID_F70A];
	}


	//确认对话框判定后需要处理
	if(VarAdrToUI16(SERVO_PARRESETOK)) //bMsgxConfirmation
	{ 
		//自学习
		if (VarAdrToUI16(SERVO_SELFLEARN)) //bSelfLearnConfirm
		{
			ServoResetStatus();
			VarAdrSetInt(SERVO_SELFLEARNSWITCH,1); //flSelfLearnSwitch = TRUE;	
			//oprintf("SERVO_PARRESETOK=%d\r\n",VarAdrToUI16(SERVO_SELFLEARNSWITCH));
			VarAdrSetInt(TMP_SERVO_SELFSTEP,1);//自学习步骤flSelfLearnState  = 1;
			time_1500ms = GetTick();
			Send_TaskCan(CAN_TX_WRITE,nServoId,0xF002,0);
			//1-静态 2-动态 5-停止
			if(bDynamicLearn)
				Send_TaskCan(CAN_TX_WRITE,nServoId,0x2100,2);
			else
				Send_TaskCan(CAN_TX_WRITE,nServoId,0x2100,1);

			//PageWgtSetFocus(PanelCurPage(),ppg->button12);
		}
		//切换界面
		else if(bSwitchPageConfirm)
		{
			UI16 wValue_F00A;
			ServoResetStatus();
			wValue_F00A=dbsvox[nServoId].wData[ID_F00A];
			if((VarAdrToUI16(SYS_FL_MACH_CODE0) & 0x04) ==0 && (VarAdrToUI16(SYS_FL_MACH_CODE54) & 0x1020))
				Send_TaskCan(CAN_TX_WRITE,nServoId,0xE000,5);
			else  Send_TaskCan(CAN_TX_WRITE,nServoId,0xE000,3);
			Send_TaskCan(CAN_TX_READ ,nServoId,0xE000,0);
			Send_TaskCan(CAN_TX_WRITE,nServoId,0xF00A, wValue_F00A);
			Send_TaskCan(CAN_TX_WRITE,nServoId,0xF00C, wValue_F00A);
			Send_TaskCan(CAN_TX_READ ,nServoId,0xF00A,0);
			//页面跳转
			SendKey(KEY_FUN3);
			// /PanelShowPageByName(PAGE_SERVO5DEBUG);
		}
		//驱动器参数重置确认条件
		else if(bDriveParResetConfirm)
		{
			ServoResetStatus();
			Send_TaskCan(CAN_TX_WRITE,nServoId,0x2000  ,0x100);
			SetServo();
		}
	}

	if(dbsvox[nServoId].wData[ID_8000] == 0)
	{
		VarAdrSetInt(SERVO_SELFLEARNPROMPT,0);
		WGTSetVisible(ppg->label156,FALSE);
	}
	else
	{
		WGTSetVisible(ppg->label156,TRUE);
		VarAdrSetInt(SERVO_SELFLEARNPROMPT,1);
		//VarAdrSetInt(TMP_SERVO_SERVOSTATE,dbsvox[nServoId].wData[ID_8000]);
	}
}

static int button10OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int button14OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int button12OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static void CtlConnect()
{
    WgtMsgConnect(servo_cfg_pg.button12, WGT_CLICK_MSG, button12OnClick);
    WgtMsgConnect(servo_cfg_pg.button14, WGT_CLICK_MSG, button14OnClick);
    WgtMsgConnect(servo_cfg_pg.button10, WGT_CLICK_MSG, button10OnClick);
}
static int Servo_CfgOnInit(Servo_Cfg_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Servo_CfgOnShow(Servo_Cfg_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	ServoPageIn(ppg);

    return 0;
}
static int Servo_CfgOnHide(Servo_Cfg_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	VarAdrSetInt(SERVO_SELFLEARNSWITCH,0);//伺服自学习开关

    return 0;
}
static int Servo_CfgOnUpdate(Servo_Cfg_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	ServoPageUpdate(ppg);

    return 0;
}
static int Servo_CfgOnChange(Servo_Cfg_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	UI16 value;
	UI32 addr;
	PATTR pattr;

	pattr = (PATTR)msg_para;

	addr = OBJAttrGetAdr(pattr);

	if(addr == TMP_SERVO_SERVOID)
	{
		value = VarAdrToUI16(TMP_SERVO_SERVOID);
		nServoId = value +1;
		//20181217 hz
		VarAdrSetInt(TMP_SERVO_MOTORBRAND,MOTORBRAND(SERVO_MOTORBRAND,nServoId));
		VarAdrSetInt(TMP_SERVO_MOTORTYPE,g_dbsvotype.cServoType[nServoId-1]);
		if (VarAdrToInt(TMP_SERVO_MOTORBRAND) == 1)
		{
			WGTSetVisible(ppg->dropdown10, TRUE);
			WGTSetVisible(ppg->dropdown8, FALSE);
		} 
		else
		{
			WGTSetVisible(ppg->dropdown10, FALSE);
			WGTSetVisible(ppg->dropdown8, TRUE);
		}

		SetServo();
		
	}
	else if(addr == TMP_SERVO_DRIVEMD)
	{
		UI16 value_F00A;

		value  = VarAdrToUI16(TMP_SERVO_DRIVEMD);
		value_F00A=dbsvox[nServoId].wData[ID_F00A];
		Send_TaskCan(CAN_TX_WRITE,nServoId,0xE000,value);
		Send_TaskCan(CAN_TX_READ ,nServoId,0xE000,0);
		Send_TaskCan(CAN_TX_WRITE,nServoId,0xF00A,value_F00A);
		Send_TaskCan(CAN_TX_WRITE,nServoId,0xF00C,value_F00A);
		Send_TaskCan(CAN_TX_READ ,nServoId,0xF00A,0);
		SetServo();
	}
	//20181019hz
	else if(addr == TMP_SERVO_MOTORBRAND)
	{
		MotorTypeSel();
		if (VarAdrToInt(TMP_SERVO_MOTORBRAND) == 1)
		{
			WGTSetVisible(ppg->dropdown10, TRUE);
			WGTSetVisible(ppg->dropdown8, FALSE);
		} 
		else
		{
			WGTSetVisible(ppg->dropdown10, FALSE);
			WGTSetVisible(ppg->dropdown8, TRUE);
		}
	}
	else if(addr == TMP_SERVO_RATEDPOWER) //额定功率
	{
		RatedPowerChg();
	}
	else if(addr == TMP_SERVO_RATEDVOLT) //额定电压
	{
		RatedVoltChg();
	}
	else if(addr == TMP_SERVO_RATEDCURR) //额定电流
	{
		RatedCurrentChg();
	}
	else if(addr == TMP_SERVO_RATEDROTAT) //额定转速
	{
		RatedRotateChg();
	}
	else if(addr == TMP_SERVO_MAXROTAT) //最大转速
	{
		MaxRotateChg();
	}
	else if (addr == TMP_SERVO_SELFSTATE) //自学习状态模式
	{
		SelfLearnSel();
	}
	else if ( pwgt == ppg->dropdown8)
	{
		PhaseMotorSel();
	}
	else if ( pwgt == ppg->dropdown10)
	{
		InovanceMotorSel();
	}
	
	

    return 0;
}
static int button10OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	DriveParReset();

    return 0;
}
static int button14OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	DriveDebugStep();

    return 0;
}
static int button12OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	ServoSelfLearn();

    return 0;
}
