#include "WServo_State.h"
#include "typedef.h"
#include "pagefram.h"
#include "database.h"
#include "stdio.h"
#include "servo.h"
#include "canmod.h"
#include "texttbl.h"
#include "protcmd.h"
#include "common.h"
#include "buttonwgt.h"
#include "kjump.h"
#include "dataCfgtb.h"
#include "prot.h"
#include "privdef.h"
#include "ctrlcmd.h"

DEFINE_SERVO_STATE_MAP

CONNECT_SERVO_STATE_EVENT

#define TMP_SERVO_OUTCURRENT     TMPVAL(20)   //输出电流   临时变量20
#define TMP_SERVO_MODTEMP        TMPVAL(30)   //模块温度   临时变量30
#define TMP_SERVO_SERVOID	     TMPVAL(50)   //伺服驱动ID 临时变量50
//comm state date
#define TMP_SERVO_PANELTX	     TMPVAL(60)   //面板与驱动发送   临时变量60
#define TMP_SERVO_PANELRX	     TMPVAL(61)   //面板与驱动接收   临时变量61
#define TMP_SERVO_MAINTX	     TMPVAL(63)   //主机与面板发送 临时变量63
#define TMP_SERVO_MAINRX	     TMPVAL(64)   //主机与面板接收 临时变量64

#define SERVO_PDLOSTBG	     0x331300e2  //面板与驱动丢包背景色
#define SERVO_PMLOSTBG	     0x331300e3  //面板与主机丢包背景色  
#define SERVO_MDLOSTBG	     0x331300e4  //主机与驱动丢包背景色

#define TMP_SERVO_COMMTEST	     TMPVAL(400)  //COMM TEST  临时变量400

#define TMP_SERVO_MACHTYPE	     TMPSTR(0)   //机型 临时字符串变量0
#define TMP_SERVO_SERVOTYPE	     TMPSTR(10)   //驱动机型 临时字符串变量10
#define TMP_SERVO_MOTORTYPE      TMPSTR(20)  //电机机型 临时字符串变量20
#define TMP_SERVO_RUNSTATE       TMPSTR(30)  //运行状态 临时字符串变量30
#define TMP_SERVO_COMMSTATE      TMPSTR(40)  //通讯状态 临时字符串变量40
#define TMP_SERVO_ALARMCODE      TMPSTR(50)  //警报代码 临时字符串变量50
#define TMP_SERVO_SERVOSPEED	 TMPSTR(60)   //伺服转速 临时字符串变量60
#define TMP_SERVO_PANELLOST	     TMPSTR(70)   //面板与驱动丢包率 临时字符串变量70
#define TMP_SERVO_MAINLOST 	     TMPSTR(71)   //主机与面板丢包率 临时字符串变量71
#define TMP_SERVO_DRIVELOST 	 TMPSTR(72)   //主机与驱动丢包率 临时字符串变量72

#define SERVO_MACH_WRITECNT 0x0A120013  //主机-驱动器 发送
#define SERVO_MACH_READCNT  0x0A120014  //主机-驱动器 接收

#define SERVO_RS232_RX      0x331300e0  //232主机与面板接收
#define SERVO_RS232_TX      0x331300e1  //232主机与面板发送

extern DB_SERVOS_EX g_dbsvosx;
extern DB_SERVOTYPT g_dbsvotype;

extern PHASH_TBL pmap_motortbl;//PHASE
extern PHASH_TBL pmap_motortblx;//INOVANCE
extern PHASH_TBL pmap_runstatetbl;//servo run state
extern PHASH_TBL pmap_tblx1;//can state
extern UI8 nServoId;
static BOOL btest=FALSE;
//static const char* trans;

static void ServoStatePageIn(Servo_State_PG* ppg)
{
	UI8 i,j;
	const char *ptrans;

	//machine type name
	g_version = ReadVersion();
	VarAdrSetStr(TMP_SERVO_MACHTYPE,g_version.system1);

	//require slow data
	if((VarAdrToUI16(SYS_FL_MACH_CODE0) & 0x04) ==0 && (VarAdrToUI16(SYS_FL_MACH_CODE54) & 0x1020))
	{
		for(i=1;i<8;i++){
			if(VarAdrToUI16(SERVO_STATE_CANST1+i-1) & 0x80){
				Send_TaskCan(CAN_TX_WRITE,i,0xBF00,7);
				Send_TaskCan(CAN_TX_WRITE,i,0xBF01,0x100F);
				Send_TaskCan(CAN_TX_WRITE,i,0xBF02,0x1004);
				Send_TaskCan(CAN_TX_WRITE,i,0xBF03,0x1707);
				Send_TaskCan(CAN_TX_WRITE,i,0xBF04,0x1019);
				Send_TaskCan(CAN_TX_WRITE,i,0xBF05,0x3000);
				Send_TaskCan(CAN_TX_WRITE,i,0xBF06,0x8000);
				Send_TaskCan(CAN_TX_WRITE,i,0xBF07,0x8001);
			}
		}
	}

	//motor type
	for(j=0;j<7;j++)
	{
		if(pmap_motortbl!=NULL && pmap_motortblx!=NULL)
		{
			if((VarAdrToUI16(SERVO_MOTORBRAND)&(1 << (j)))>0?1:0)
			{
				ptrans = GetMapTran(pmap_motortblx,g_dbsvotype.cServoType[j]);//inovance
			}
			else 
			{
				ptrans = GetMapTran(pmap_motortbl,g_dbsvotype.cServoType[j]);//PHASE
			}
			VarAdrSetStr(TMP_SERVO_MOTORTYPE+j,ptrans);
		}

		VarAdrSetStr(TMP_SERVO_SERVOTYPE+j,"");//驱动型号
	}

	//
	g_dbsvosx.wToDriver=0;
	g_dbsvosx.wFromDriver=0;
	g_dbsvosx.wToControl=0;
	g_dbsvosx.wFromControl=0;

	btest=FALSE;

	ButtonSetText(ppg->button42,GetTextTran(TEXT_TESTBEGIN));
	WGTSetEnable(ppg->dropdown1,TRUE);
	VarAdrSetInt(TMP_SERVO_SERVOID,nServoId-1);//默认显示驱动器1
}

static void ServoStatePageOut()
{
	UI8 i;
	UI16 pdata[2] ={0,0};

	//close slow data require
	if((VarAdrToUI16(SYS_FL_MACH_CODE0) & 0x04) ==0 && (VarAdrToUI16(SYS_FL_MACH_CODE54) & 0x1020))
	{
		for(i=1;i<8;i++){
			if(VarAdrToUI16(SERVO_STATE_CANST1+i-1) & 0x80){
				Send_TaskCan(CAN_TX_WRITE,i,0xBF00,0);
			}
		}
	}
	//close comm test
	if(btest)	
	{
		ProtSysCtrl(CAN_TEST_CMD,2,pdata);
	}

	VarAdrSetInt(TMP_SERVO_COMMTEST,0);
}

static void ServoStateUpdate()
{
	UI8 i,j;
	char cbuf[10];
	int wLost;
	int sendCnt,receiveCnt;
	const char *ptrans;
	const char *ptransx;
	static UI32 time_100ms =0;
	static UI32 time_300ms =0;
	static UI32 time_1s =0;

	//servo state
	for(i=1;i<8;i++) {
		if(dbsvox[i].wData[ID_100F]>=32768)
		{
			snprintf(cbuf,sizeof(cbuf)-1,"%d",dbsvox[i].wData[ID_100F]-65536);
		}
		else 
		{
			snprintf(cbuf,sizeof(cbuf)-1,"%d",dbsvox[i].wData[ID_100F]);
		}
		VarAdrSetStr(TMP_SERVO_SERVOSPEED+i-1,cbuf);//servo speed
		VarAdrSetInt(TMP_SERVO_OUTCURRENT+i-1,dbsvox[i].wData[ID_1004]);//servo outcurrent
		VarAdrSetInt(TMP_SERVO_MODTEMP+i-1,dbsvox[i].wData[ID_1707]);   //servo moduar temp
	}

	if((GetTick()-time_300ms)>300)
	{
		time_300ms = GetTick();

		for(j=1;j<8;j++)
		{
			// run state 
			if(pmap_runstatetbl !=NULL)
			{
				ptrans = GetMapTran(pmap_runstatetbl,dbsvox[j].wData[ID_3000]);
				VarAdrSetStr(TMP_SERVO_RUNSTATE+j-1,ptrans);
			}
			// comm state Online or Offline
			if(pmap_tblx1 !=NULL)
			{
				ptransx = GetMapTran(pmap_tblx1,(dbsvox[j].nCount>0?1:0));//
				VarAdrSetStr((TMP_SERVO_COMMSTATE+j-1),ptransx);
			}
			if(dbsvox[j].nCount==0)
			{
				dbsvox[j].wData[ID_1004]=0;
				dbsvox[j].wData[ID_1707]=0;
				dbsvox[j].wData[ID_3000]=0;
				dbsvox[j].wData[ID_100F]=0;
				dbsvox[j].wData[ID_8001]=0;
			}
			dbsvox[j].nCount=0;


			if(dbsvox[j].wData[ID_8000] == 0)
			{
				cbuf[0] = '\0';	
			}
			else
			{
				snprintf(cbuf,sizeof(cbuf)-1,"ERR%d",dbsvox[j].wData[ID_8000]);
			}
			VarAdrSetStr(TMP_SERVO_ALARMCODE+j-1,cbuf);
		}

	}

	//通讯数据包
	VarAdrSetInt(TMP_SERVO_PANELTX,g_dbsvosx.wToDriver);
	VarAdrSetInt(TMP_SERVO_PANELRX,g_dbsvosx.wFromDriver);
	VarAdrSetInt(TMP_SERVO_MAINTX,g_dbsvosx.wToControl);
	VarAdrSetInt(TMP_SERVO_MAINRX,g_dbsvosx.wFromControl);
	VarAdrSetInt(SERVO_RS232_TX,ProtTxCnt());
	VarAdrSetInt(SERVO_RS232_RX,ProtRxCnt());

	if((GetTick()-time_1s)>1000)
	{
		time_1s = GetTick();
		//panel and drive
		sendCnt    = g_dbsvosx.wToDriver;
		receiveCnt = g_dbsvosx.wFromDriver;
		if(g_dbsvosx.wToDriver == 0)
			wLost = 0;
		else
		{
			wLost = ((sendCnt-receiveCnt)*100)/sendCnt;
		}
		if(wLost>=0)
		{
			WordToStr(cbuf,wLost,0);
		}
		else
		{
			snprintf(cbuf,sizeof(cbuf)-1,"%d",wLost);
		}
		VarAdrSetStr(TMP_SERVO_PANELLOST,cbuf);
		if(wLost>10)
		{
			VarAdrSetInt(SERVO_PDLOSTBG,ORANGE_RED);
		}
		else if(wLost>=0 && wLost<=10)
		{
            VarAdrSetInt(SERVO_PDLOSTBG,LIGHT_GREEN);
		}
		else
		{
			VarAdrSetInt(SERVO_PDLOSTBG,0x000000);
		}
		//panel and main
		sendCnt    = g_dbsvosx.wToControl;
		receiveCnt = g_dbsvosx.wFromControl;
		if(g_dbsvosx.wToControl == 0)
			wLost = 0;
		else
		{
			wLost = ((sendCnt-receiveCnt)*100)/sendCnt;
		}
		if(wLost>=0)
		{
			WordToStr(cbuf,wLost,0);
		}
		else
		{
			snprintf(cbuf,sizeof(cbuf)-1,"%d",wLost);
		}
		VarAdrSetStr(TMP_SERVO_MAINLOST,cbuf);
		if(wLost>10)
		{
			VarAdrSetInt(SERVO_PMLOSTBG,ORANGE_RED);
		}
		else if(wLost>=0 && wLost<=10)
		{
            VarAdrSetInt(SERVO_PMLOSTBG,LIGHT_GREEN);
		}
		else
		{
			VarAdrSetInt(SERVO_PMLOSTBG,0x000000);
		}
		//main and drive
		sendCnt    = VarAdrToUI16(SERVO_MACH_WRITECNT);
		receiveCnt = VarAdrToUI16(SERVO_MACH_READCNT);
		if(VarAdrToUI16(SERVO_MACH_WRITECNT) == 0)
			wLost = 0;
		else
		{
			wLost = ((sendCnt-receiveCnt)*100)/sendCnt;
		}
		if(wLost>=0)
		{
			WordToStr(cbuf,wLost,0);
		}
		else
		{
			snprintf(cbuf,sizeof(cbuf)-1,"%d",wLost);
		}
		VarAdrSetStr(TMP_SERVO_DRIVELOST,cbuf);
		if(wLost>10)
		{
            VarAdrSetInt(SERVO_MDLOSTBG,ORANGE_RED);
		}
		else if(wLost>=0 && wLost<=10)
		{
            VarAdrSetInt(SERVO_MDLOSTBG,LIGHT_GREEN);
		}
		else
		{
			VarAdrSetInt(SERVO_MDLOSTBG,0x000000);
		}
	}

	//comm test
	if(btest)
	{
		if((GetTick()-time_100ms)>100)
		{
			time_100ms = GetTick();
			Send_TaskCan(CAN_TX_MAINTEST,0,0,nServoId);
			++g_dbsvosx.wToControl;

			Send_TaskCan(CAN_TX_READ,nServoId,0x8001,0);
			g_dbsvosx.wToDriver++;
		}
	}
}

static int button42OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static void CtlConnect()
{
	WgtMsgConnect(servo_state_pg.button42, WGT_CLICK_MSG, button42OnClick);
}
static int Servo_StateOnInit(Servo_State_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Servo_StateOnShow(Servo_State_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	ServoStatePageIn(ppg);
    G10ServoStateGuideSwitch();//20211117 dyl 伺服状态画面等级放低到LV2 --伺服机模式的伺服状态
    return 0;
}
static int Servo_StateOnHide(Servo_State_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	ServoStatePageOut();

    return 0;
}
static int Servo_StateOnUpdate(Servo_State_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	ServoStateUpdate();

    return 0;
}
static int Servo_StateOnChange(Servo_State_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
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
	}

	if(pwgt ==ppg->button42)
	{
		UI16 pdata[3];

		btest=!btest;

		if(btest)
		{
			g_dbsvosx.wToDriver=0;
			g_dbsvosx.wFromDriver=0;
			g_dbsvosx.wToControl=0;
			g_dbsvosx.wFromControl=0;
			ButtonSetText(pwgt,GetTextTran(TEXT_TESTEND));
			WGTSetEnable(ppg->dropdown1,FALSE);
		}
		else
		{
			ButtonSetText(pwgt,GetTextTran(TEXT_TESTBEGIN));
			WGTSetEnable(ppg->dropdown1,TRUE);

		}
		if(btest)
			pdata[0]=1;
		else
			pdata[0]=0;
		pdata[1]=nServoId;
		pdata[2]=0;
		ProtSysCtrl(CAN_TEST_CMD,3,pdata);
	}

    return 0;
}
static int button42OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	return 0;
}
