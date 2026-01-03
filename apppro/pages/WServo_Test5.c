#include "WServo_Test5.h"
#include "typedef.h"
#include "common.h"
#include "database.h"
#include "canmod.h"
#include "servo.h"
#include "stdio.h"
#include "dataCfgtb.h"

DEFINE_SERVO_TEST5_MAP

CONNECT_SERVO_TEST5_EVENT

typedef enum servoAddr
{
	SERVO_MAXPRS =0xA302, //系统油压  0.0kg/cm2 ～最大油压（A3-03）  新增
	SERVO_MAXRANGE,       //最大油压  0.0---500.0 kg/cm2

	SERVO_MAXREVSPD = 0xA308,//最大反向速 0.0% ～ 100.0% 新增
	SERVO_LOWSPD,            //底流  0.0% ～ 50.0% 新增
	SERVO_LOWPRS,            //底压  0.0---50.0 kg/cm2

	SERVO_OVERLEVEL = 0xA31B, //超调抑制检测等级 0~2000 新增
	SERVO_OVERCOEF,           //超调抵制系数   0~3.000  新增

	SERVO_ROTATDIR = 0xF009, //旋转方向

	SERVO_LOWGAIN = 0xF200,  //速度环比例增益  0---100
	SERVO_HIGHGAIN = 0xF203, //速度环比例增益2  0 ～ 400
	SERVO_MAXTORQUE = 0xF20A,//扭矩上限  0.0---250.0 %  新增

	SERVO_MAXVOL = 0xF41E,   //最大输入对应设定 -100.0% ～ 100.0% 新增
}SERVOADDR;

#define TMP_SERVO_MACHTYPE	     TMPSTR(0)   //机型 临时字符串变量0
//#define TMP_SERVO_SERVOSPEED     TMPSTR(10)  //电机转速 临时字符串变量10
//#define TMP_SERVO_SERVOPOWER     TMPSTR(11)  //输出功耗 临时字符串变量11

#define TMP_SERVO_DATAADDR       TMPVAL(1)    //驱动数据起始地址 临时变量1


#define TMP_SERVO_LOWGAIN        TMPVAL(13)    //速度环比例增益  0---100   临时变量13
#define TMP_SERVO_MAXTORQUE      TMPVAL(15)    //扭矩上限  0.0---250.0 %  新增  临时变量15   
#define TMP_SERVO_ROTATDIR       TMPVAL(28)    //旋转方向                 临时变量28   
#define TMP_SERVO_HIGHGAIN       TMPVAL(31)    //速度环比例增益2  0 ～ 400  临时变量31
#define TMP_SERVO_MAXREVSPD      TMPVAL(36)    //最大反向速 0.0% ～ 100.0% 新增  临时变量36
#define TMP_SERVO_MAXRANGE       TMPVAL(37)    //最大油压  0.0---500.0 kg/cm2    临时变量37
#define TMP_SERVO_LOWPRESS       TMPVAL(38)    //底压  0.0---50.0 kg/cm2         临时变量38
#define TMP_SERVO_LOWFLOW        TMPVAL(49)    //底流  0.0% ～ 50.0% 新增        临时变量49
#define TMP_SERVO_OVERLEVEL      TMPVAL(53)    //超调抑制检测等级 0~2000 新增    临时变量53
#define TMP_SERVO_MAXVOL         TMPVAL(83)    //最大输入对应设定 -100.0% ～ 100.0% 新增 临时变量83
#define TMP_SERVO_MAXPRS         TMPVAL(136)    //系统油压  0.0kg/cm2 ～最大油压（A3-03）  新增  临时变量136
#define TMP_SERVO_OVERCOEF       TMPVAL(149)    //超调抵制系数   0~3.000  新增   临时变量149

#define TMP_SERVO_DRIVEMD	     TMPVAL(188)   //驱动模式 临时变量188
#define TMP_SERVO_SERVOID	     TMPVAL(400)   //伺服驱动ID 临时变量400
#define TMP_SERVO_PRESS1	     TMPVAL(401)   //伺服反馈压力1 临时变量401
#define TMP_SERVO_PRESS2	     TMPVAL(402)   //伺服反馈压力2 临时变量402
#define TMP_SERVO_SERVOPOWER     TMPVAL(451)  //输出功耗 临时字符串变量

#define SERVO_DEBUGZERO          0x331300db    //伺服模式和传感器归零是否允许使用
#define SERVO_ROTATEDDIR         0x331300dc    //旋转方向是否变灰
#define SERVO_MAINID             0x331300dd    //伺服主泵个数，用于压力传感器归零是否可用

#define SERVO_SPEED              0x331300e5 //电机转速

// EST ctrl debug page and servo5 debug page
//#define PAGE_3MODE_DEBUG  	"Servo_Test3" //伺服3模式调试页面

extern UI8 nServoId;

static I16  nServoId_version=-1;
static UI16 nozretprs =0;
static UI16 nozretspd =0;

//定时请求间隔时间
static UI32 time_3s =0;

/************************************************************************/
/* 调试各伺服数据读取                                                   */
/************************************************************************/
static void SetServo()
{
	Send_TaskCan(CAN_TX_READ,nServoId,0xE000,0);
	//Send_TaskCan(CAN_TX_READ,nServoId,0xE002,0);
	Send_TaskCan(CAN_TX_READ,nServoId,0xF20A,0);

	Send_TaskCan(CAN_TX_READ,nServoId,0xA302,0);
	Send_TaskCan(CAN_TX_READ,nServoId,0xA303,0);
	Send_TaskCan(CAN_TX_READ,nServoId,0xA308,0);
	Send_TaskCan(CAN_TX_READ,nServoId,0xA309,0);
	Send_TaskCan(CAN_TX_READ,nServoId,0xA30A,0);
	Send_TaskCan(CAN_TX_READ,nServoId,0xA31B,0);
	Send_TaskCan(CAN_TX_READ,nServoId,0xA31C,0);

	Send_TaskCan(CAN_TX_READ,nServoId,0xF009,0);
	Send_TaskCan(CAN_TX_READ,nServoId,0xF00A,0);

	Send_TaskCan(CAN_TX_READ,nServoId,0xF200,0);
	Send_TaskCan(CAN_TX_READ,nServoId,0xF203,0);
	Send_TaskCan(CAN_TX_READ,nServoId,0xF41E,0);
	Send_TaskCan(CAN_TX_READ,nServoId,0xF70A,0);

	//nSecond = 0;
	time_3s = GetTick();
}
/************************************************************************/
/* 模式5调整页面进入                                                    */
/************************************************************************/
static void ServoDebugPageIn()
{
	UI8 i;

	//machine type name
	VarAdrSetStr(TMP_SERVO_MACHTYPE,g_version.system1);

		//require slow data
	for(i=1;i<8;i++){
		if(VarAdrToUI16(SERVO_STATE_CANST1+i-1) & 0x80){
			Send_TaskCan(CAN_TX_WRITE,i,0xBF00,4);
			Send_TaskCan(CAN_TX_WRITE,i,0xBF01,0x100F);
			Send_TaskCan(CAN_TX_WRITE,i,0xBF02,0x1004);
			Send_TaskCan(CAN_TX_WRITE,i,0xBF03,0x1006);
			Send_TaskCan(CAN_TX_WRITE,i,0xBF04,0x1005);
		}
	}

	//nozzle press and flow
	nozretprs = VarAdrToUI16(NOZZLE_PRES_NOZRET);
	nozretspd = VarAdrToUI16(NOZZLE_FLOW_NOZRET);

	VarAdrSetInt(TMP_SERVO_SERVOID,nServoId-1);//默认显示驱动器1

	//servo data require
	SetServo();

	//
	if(nServoId>2)
	{
		VarAdrSetInt(SERVO_MAINID,0);
	}
	else
	{
		VarAdrSetInt(SERVO_MAINID,1);
	}
}
/************************************************************************/
/* 模式5调整页面退出                                                    */
/************************************************************************/
static void ServoDebugPageOut()
{
	UI8 i;
	PVAR pdata;

	VarAdrSetInt(NOZZLE_PRES_NOZRET,nozretprs);
	if(pdata = VarAdrToVar(NOZZLE_PRES_NOZRET))
		VarSaveInt(pdata);
	VarSendByAdr(NOZZLE_PRES_NOZRET);

	VarAdrSetInt(NOZZLE_FLOW_NOZRET,nozretspd);
	if(pdata = VarAdrToVar(NOZZLE_FLOW_NOZRET))
		VarSaveInt(pdata);
	VarSendByAdr(NOZZLE_FLOW_NOZRET);

	for(i=1;i<8;i++)
	{
		if(VarAdrToUI16(SERVO_STATE_CANST1+i-1) & 0x80)
			Send_TaskCan(CAN_TX_WRITE,i,0xBF00,0);
	}

	nServoId_version = -1;
}
/************************************************************************/
/* 模式5调整页面刷新                                                    */
/************************************************************************/
static void ServoDebugUpdate(Servo_Test5_PG* ppg)
{
	UI16 i;
	//char cbuf[10];

	//drive data require
	if((GetTick()-time_3s)>3000)
	{
		time_3s = GetTick();
		Send_TaskCan(CAN_TX_READ,nServoId,0xE000,0);
		Send_TaskCan(CAN_TX_READ,nServoId,0xF70A,0);
		Send_TaskCan(CAN_TX_READ,1,0x1015,0);
		Send_TaskCan(CAN_TX_READ,nServoId,0x3000,0);
		Send_TaskCan(CAN_TX_READ,nServoId,0x8000,0);
		if(VarAdrToUI16(SYS_FL_HYDRAUIC)&0x0800)
		{
			if(VarAdrToUI16(SERVO_STATE_CANST1+1) & 0x80)//
				Send_TaskCan(CAN_TX_READ,2,0x1015,0);
		}
	}

	//drive data setting
	for(i=0;i<SERVOXADDR_COUNT;i++)	
	{
		VarAdrSetInt(TMP_SERVO_DATAADDR+i,dbsvox[nServoId].wData[i]);
	}
	VarAdrSetInt(TMP_SERVO_DATAADDR+ID_F00A,(UI32)(dbsvox[nServoId].wData[ID_F00A]*15/100.0+0.5));

	//motor speed
	if(dbsvox[nServoId].wData[ID_100F]>=32768)
	{
		VarAdrSetInt(SERVO_SPEED,dbsvox[nServoId].wData[ID_100F]-65536);
	}
	else
	{
		VarAdrSetInt(SERVO_SPEED,dbsvox[nServoId].wData[ID_100F]);
	}

	//press1，press2
	VarAdrSetInt(TMP_SERVO_PRESS1,dbsvox[1].wData[ID_1015]);
	VarAdrSetInt(TMP_SERVO_PRESS2,dbsvox[2].wData[ID_1015]);

	//power setting
	if(dbsvox[nServoId].wData[ID_1005] >= 32768)
	{
		VarAdrSetInt(TMP_SERVO_SERVOPOWER,dbsvox[nServoId].wData[ID_1005]-65536);
	}
	else{
		VarAdrSetInt(TMP_SERVO_SERVOPOWER,dbsvox[nServoId].wData[ID_1005]);
	}

	//驱动器模式 -   电机停止，电机关闭，非自学习状态
	if (dbsvox[nServoId].wData[ID_3000]==1 || dbsvox[nServoId].wData[ID_3000]==2 || VarAdrToUI16(MOTOR_STATE_STATE))
	{
		VarAdrSetInt(SERVO_DEBUGZERO,0);//伺服模式或归零是否变灰
	}
	else{
		VarAdrSetInt(SERVO_DEBUGZERO,1);//伺服模式或归零是否变灰
	}
	//驱动器模式
	//if (flServoMode != dbsvox[nServoId].wData[ID_E000]) {
	//	flServoMode = dbsvox[nServoId].wData[ID_E000];
	//	pwnd = GetWindow(hwnd);
	//	((QComboBox*)(pwnd->ctrl))->setCurrentIndex(::GetParamValue(WPARAM1 + ID_E000));
	//}

	//方向 -   电机停止
	if (dbsvox[nServoId].wData[ID_3000]==1 || dbsvox[nServoId].wData[ID_3000]==2)
		VarAdrSetInt(SERVO_ROTATEDDIR,0);//旋转方向是否变灰
	else
		VarAdrSetInt(SERVO_ROTATEDDIR,1);//旋转方向是否变灰

	if (nServoId_version != dbsvox[nServoId].wData[ID_F70A])
	{
		nServoId_version = dbsvox[nServoId].wData[ID_F70A];

		SetServo();
	}

	//servo alarm
	if(dbsvox[nServoId].wData[ID_8000] == 0)
	{
		WGTSetVisible(ppg->label172,FALSE);
	}
	else
	{
		WGTSetVisible(ppg->label172,TRUE);
	}
}
/************************************************************************/
/* 驱动编号修改                                                         */
/************************************************************************/
static void ServoIDSel()
{
	UI16 value;

	value = VarAdrToUI16(TMP_SERVO_SERVOID);
	nServoId = 1+value;
	SetServo();

	if(nServoId>2)
	{
		VarAdrSetInt(SERVO_MAINID,0);
	}
	else
	{
		VarAdrSetInt(SERVO_MAINID,1);
	}
}
/************************************************************************/
/* 压力传感器学零漂                                                     */
/************************************************************************/
static void LearnZeroDrift()
{
	Send_TaskCan(CAN_TX_WRITE,nServoId,0x2100,0x10);//学零漂
}
/************************************************************************/
/* 驱动器模式修改                                                       */
/************************************************************************/
static void DriveModeSel()
{
	UI16 value_F00A,value;

	value  = VarAdrToUI16(TMP_SERVO_DRIVEMD);
	value_F00A=dbsvox[nServoId].wData[ID_F00A];
	Send_TaskCan(CAN_TX_WRITE,nServoId,0xE000,value);
	Send_TaskCan(CAN_TX_READ ,nServoId,0xE000,0);
	Send_TaskCan(CAN_TX_WRITE,nServoId,0xF00A,value_F00A);
	Send_TaskCan(CAN_TX_WRITE,nServoId,0xF00C,value_F00A);
	Send_TaskCan(CAN_TX_READ ,nServoId,0xF00A,0);
	SetServo();
}

static int button10OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static void CtlConnect()
{
    WgtMsgConnect(servo_test5_pg.button10, WGT_CLICK_MSG, button10OnClick);
}
static int Servo_Test5OnInit(Servo_Test5_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Servo_Test5OnShow(Servo_Test5_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	ServoDebugPageIn();

    return 0;
}
static int Servo_Test5OnHide(Servo_Test5_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	ServoDebugPageOut();

    return 0;
}
static int Servo_Test5OnUpdate(Servo_Test5_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	ServoDebugUpdate(ppg);

    return 0;
}
static int Servo_Test5OnChange(Servo_Test5_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	UI16 value;
	UI32 addr;
	PATTR pattr;

	pattr = (PATTR)msg_para;

	addr = OBJAttrGetAdr(pattr);

	//
	switch(addr)
	{
	case TMP_SERVO_LOWPRESS:  //底压
		value = VarAdrToUI16(TMP_SERVO_LOWPRESS);
		Send_TaskCan(CAN_TX_WRITE,nServoId,SERVO_LOWPRS,value);
		Send_TaskCan(CAN_TX_READ ,nServoId,SERVO_LOWPRS,0);
	case TMP_SERVO_LOWFLOW:  //底流
		value = VarAdrToUI16(TMP_SERVO_LOWFLOW);
		Send_TaskCan(CAN_TX_WRITE,nServoId,SERVO_LOWSPD,value);
		Send_TaskCan(CAN_TX_READ ,nServoId,SERVO_LOWSPD,0);
		break;
	case TMP_SERVO_MAXVOL:  //最大压力
		value = VarAdrToUI16(TMP_SERVO_MAXVOL);
		Send_TaskCan(CAN_TX_WRITE,nServoId,SERVO_MAXVOL,value);
		Send_TaskCan(CAN_TX_READ ,nServoId,SERVO_MAXVOL,0);
		break;
	case TMP_SERVO_MAXRANGE:  //最大量程
		value = VarAdrToUI16(TMP_SERVO_MAXRANGE);
		Send_TaskCan(CAN_TX_WRITE,nServoId,SERVO_MAXRANGE,value);
		Send_TaskCan(CAN_TX_READ ,nServoId,SERVO_MAXRANGE,0);
		break;
	case TMP_SERVO_MAXPRS:  //最大压力
		value = VarAdrToUI16(TMP_SERVO_MAXPRS);
		Send_TaskCan(CAN_TX_WRITE,nServoId,SERVO_MAXPRS,value);
		Send_TaskCan(CAN_TX_READ ,nServoId,SERVO_MAXPRS,0);
		break;
	case TMP_SERVO_ROTATDIR:  //旋转方向
		value = VarAdrToUI16(TMP_SERVO_ROTATDIR);
		Send_TaskCan(CAN_TX_WRITE,nServoId,SERVO_ROTATDIR,value);
		Send_TaskCan(CAN_TX_READ ,nServoId,SERVO_ROTATDIR,0);
		break;
	case TMP_SERVO_MAXTORQUE:  //扭矩上限
		value = VarAdrToUI16(TMP_SERVO_MAXTORQUE);
		Send_TaskCan(CAN_TX_WRITE,nServoId,SERVO_MAXTORQUE,value);
		Send_TaskCan(CAN_TX_READ ,nServoId,SERVO_MAXTORQUE,0);
		break;
	case TMP_SERVO_LOWGAIN:  //速度环比例增益 
		value = VarAdrToUI16(TMP_SERVO_LOWGAIN);
		Send_TaskCan(CAN_TX_WRITE,nServoId,SERVO_LOWGAIN,value);
		Send_TaskCan(CAN_TX_READ ,nServoId,SERVO_LOWGAIN,0);
		break;
	case TMP_SERVO_HIGHGAIN:  //速度环比例增益2
		value = VarAdrToUI16(TMP_SERVO_HIGHGAIN);
		Send_TaskCan(CAN_TX_WRITE,nServoId,SERVO_HIGHGAIN,value);
		Send_TaskCan(CAN_TX_READ ,nServoId,SERVO_HIGHGAIN,0);
		break;
	case TMP_SERVO_MAXREVSPD:  //最大反向速
		value = VarAdrToUI16(TMP_SERVO_MAXREVSPD);
		Send_TaskCan(CAN_TX_WRITE,nServoId,SERVO_MAXREVSPD,value);
		Send_TaskCan(CAN_TX_READ ,nServoId,SERVO_MAXREVSPD,0);
		break;
	case TMP_SERVO_OVERLEVEL:  //超调抑制检测等级
		value = VarAdrToUI16(TMP_SERVO_OVERLEVEL);
		Send_TaskCan(CAN_TX_WRITE,nServoId,SERVO_OVERLEVEL,value);
		Send_TaskCan(CAN_TX_READ ,nServoId,SERVO_OVERLEVEL,0);
		break;
	case TMP_SERVO_OVERCOEF:  //超调抵制系数
		value = VarAdrToUI16(TMP_SERVO_OVERCOEF);
		Send_TaskCan(CAN_TX_WRITE,nServoId,SERVO_OVERCOEF,value);
		Send_TaskCan(CAN_TX_READ ,nServoId,SERVO_OVERCOEF,0);
		break;
	default:
		if(addr == TMP_SERVO_SERVOID)
		{
			ServoIDSel();
		}
		else if(addr == TMP_SERVO_DRIVEMD)
		{
			DriveModeSel();
		}
		break;
	}

    return 0;
}
static int button10OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	LearnZeroDrift();//学零漂

    return 1;
}
