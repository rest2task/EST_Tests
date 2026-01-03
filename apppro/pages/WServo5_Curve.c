#include "WServo5_Curve.h"
#include "typedef.h"
#include "pagefram.h"
#include "database.h"
#include "canmod.h"
#include "language.h"
#include "texttbl.h"
#include "curvesctl.h"
#include "stdio.h"
#include "servo.h"
#include "buttonwgt.h"
#include "oprintf.h"
#include "privdef.h"

DEFINE_SERVO5_CURVE_MAP

CONNECT_SERVO5_CURVE_EVENT

#define TMP_SERVO_PRSSET	TMPVAL(1)   //设定压力选择 临时变量

#define TMP_SERVO_CURRENT	TMPVAL(10)   //输出电流 临时变量
#define TMP_SERVO_VOLTAGE	TMPVAL(11)   //母线电压 临时变量
#define TMP_SERVO_MOTORSPDX	TMPVAL(12)   //电机转速 临时变量

#define TMP_SERVO_SERVOID   TMPVAL(400) //伺服驱动ID 临时变量

#define TMP_SERVO_MOTORSPD	      TMPSTR(0)   //电机转速 临时字符串变量
#define TMP_SERVO_CURVESWTICH     TMPSTR(92)  //开始与暂停 临时字符串变量

#define SERVO_CURVE   0x331300de        //伺服曲线开始或停止

#define HYDR_STATE_DA_OUTPUT0	0x09160002 // 540 DA输出1
#define HYDR_STATE_DA_OUTPUT1	0x09160003 // 541 DA速度输出1

#define HYDRSERVO_STATE_SERVO_PRESS1	0x0A160042  //驱动器反馈压力1
extern  UI8 nServoId;
static  int bCurveShow[]={1,0,1,0,1,0};
static  BOOL bpaint =FALSE;
static	BOOL isStart = FALSE;
//static  const char* trans;

/************************************************************************/
/* 伺服曲线画面进入                                                     */
/************************************************************************/
static void Servo5CurvePageIn(Servo5_Curve_PG* ppg)
{
	UI8 i;

	//require slow data
	if((VarAdrToUI16(SYS_FL_MACH_CODE0) & 0x04) ==0 && (VarAdrToUI16(SYS_FL_MACH_CODE54) & 0x1020))
	{
		for(i=1;i<8;i++){
			if(VarAdrToUI16(SERVO_STATE_CANST1+i-1) & 0x80){
				Send_TaskCan(CAN_TX_WRITE,i,0xBF10,3);
				Send_TaskCan(CAN_TX_WRITE,i,0xBF11,0x100F);
				Send_TaskCan(CAN_TX_WRITE,i,0xBF12,0x1004);
				Send_TaskCan(CAN_TX_WRITE,i,0xBF13,0x1002);
			}
		}

		bCurveShow[5]=0;
	}

	for(i=0;i<6;i++){
		VarAdrSetInt(TMP_SERVO_PRSSET+i,bCurveShow[i]);
	}

	VarAdrSetInt(TMP_SERVO_SERVOID,nServoId-1);//默认显示驱动器

	bpaint =FALSE;

	ButtonSetText(ppg->button10,GetTextTran(TEXT_START));
	VarAdrSetInt(SERVO_CURVE,1);

}

static void Servo5CurvePageOut()
{
	UI8 i;

	//close slow data
	if((VarAdrToUI16(SYS_FL_MACH_CODE0) & 0x04) ==0 && (VarAdrToUI16(SYS_FL_MACH_CODE54) & 0x1020))
	{
		for(i=1;i<8;i++){
			if(VarAdrToUI16(SERVO_STATE_CANST1+i-1) & 0x80)
				Send_TaskCan(CAN_TX_WRITE,i,0xBF10,0);
		}
	}
}

#define DL_200MS	(100)	//200ms
static void Servo5CurveUpdate(PWGT pgraph)
{
	static UI32 lasttime = 0;//最近时间
	char cbuf[10];
	static int m_nTime=0;
	FPOINT tmppoint;

	VarAdrSetInt(TMP_SERVO_CURRENT,dbsvox[nServoId].wData[ID_1004]);//电流
	VarAdrSetInt(TMP_SERVO_VOLTAGE,dbsvox[nServoId].wData[ID_1002]);//母线电压

	if(dbsvox[nServoId].wData[ID_100F]>=32768)
	{
		snprintf(cbuf,sizeof(cbuf)-1,"%d",dbsvox[nServoId].wData[ID_100F]-65536);//显示用
		VarAdrSetInt(TMP_SERVO_MOTORSPDX,0);//画曲线用
	}
	else
	{
		snprintf(cbuf,sizeof(cbuf)-1,"%d",dbsvox[nServoId].wData[ID_100F]);
		VarAdrSetInt(TMP_SERVO_MOTORSPDX,dbsvox[nServoId].wData[ID_100F]);//画曲线用
	}
	VarAdrSetStr(TMP_SERVO_MOTORSPD,cbuf);

	if(isStart && ((GetMsTick() - lasttime) >= DL_200MS))	//定时对曲线进行赋值
	{
		PGRAPH_WGT pGraph = (PGRAPH_WGT)pgraph;
		lasttime = GetMsTick();
		if(m_nTime++ > 570) //宽度
		{
			Graph_Clear(pGraph);
			m_nTime = 0;
		}
		
		tmppoint.x = m_nTime;

		if(VarAdrToInt(TMP_SERVO_PRSSET) !=0 )
		{
			tmppoint.y = VarAdrToInt(HYDR_STATE_DA_OUTPUT0) ;	// 540 DA输出1 设定压力
			GraphAddPointByCurveName(pGraph,"prsSet",&tmppoint,1);
		}
		
		if(VarAdrToInt(TMP_SERVO_PRSSET+1) !=0 )
		{
			tmppoint.y = VarAdrToInt(HYDR_STATE_DA_OUTPUT1) /10.0;	//547 DA输出8 设定流量
			GraphAddPointByCurveName(pGraph,"spdSet",&tmppoint,1);
		}

		if(VarAdrToInt(TMP_SERVO_PRSSET+2) !=0 )	// 553	位置尺相对位置实际压
		{
            tmppoint.y = VarAdrToInt(HYDRSERVO_STATE_SERVO_PRESS1) /10.0; //20210623.cyx
			GraphAddPointByCurveName(pGraph,"prsReal",&tmppoint,1);
		}

		if(VarAdrToInt(TMP_SERVO_PRSSET+3) != 0 )	//电机电流
		{
			tmppoint.y = dbsvox[nServoId].wData[ID_1004]/10.0 ;	//547 DA输出8 设定流量
			GraphAddPointByCurveName(pGraph,"current",&tmppoint,1);
		}

		if(VarAdrToInt(TMP_SERVO_PRSSET+4) != 0 )	//电机转速
		{
			int svospd;
			if(dbsvox[nServoId].wData[ID_100F] < 32768)
				svospd = dbsvox[nServoId].wData[ID_100F];
			else
				svospd = dbsvox[nServoId].wData[ID_100F] - 65536;
            tmppoint.y = svospd;	/* 0.14*/
            GraphAddPointByCurveName(pGraph,"motorSpd",&tmppoint,1);
		}

		if(VarAdrToInt(TMP_SERVO_PRSSET+5) != 0 )	//母线电压
		{
			tmppoint.y = dbsvox[nServoId].wData[ID_1002] /10.0;	 //dbsvox[nServoId].wData[ID_1002]
			GraphAddPointByCurveName(pGraph,"vol",&tmppoint,1);
		}
        GraphRePaint((PGRAPH_WGT)servo5_curve_pg.graph1);
	}
}
//need to prefect
static BOOL Servo5CurveStart(PGRAPH_WGT pwgt,PWGT pwgtx)
{
	bpaint = !bpaint;
	if(bpaint)
	{
		VarAdrSetInt(SERVO_CURVE,0);
		ButtonSetText(pwgtx,GetTextTran(TEXT_STOP));
		Graph_Clear(pwgt);
	}
	else
	{
		VarAdrSetInt(SERVO_CURVE,1);
		ButtonSetText(pwgtx,GetTextTran(TEXT_START));
		CurvesStop(pwgt);
	}

	return TRUE;
}

static int button10OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static void CtlConnect()
{
    WgtMsgConnect(servo5_curve_pg.button10, WGT_CLICK_MSG, button10OnClick);
}
static int Servo5_CurveOnInit(Servo5_Curve_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Servo5_CurveOnShow(Servo5_Curve_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	isStart = FALSE;
    Servo5CurvePageIn(ppg);

    return 0;
}
static int Servo5_CurveOnHide(Servo5_Curve_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    Servo5CurvePageOut();

    return 0;
}
static int Servo5_CurveOnUpdate(Servo5_Curve_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    Servo5CurveUpdate(ppg->graph1);
    return 0;
}
#include "oprintf.h"
static int Servo5_CurveOnChange(Servo5_Curve_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
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
    else if(addr >= TMP_SERVO_PRSSET && addr <= (TMP_SERVO_PRSSET+5))
    {
        value = VarAdrToInt(addr);
        bCurveShow[addr-TMP_SERVO_PRSSET] = value;
    }

    return 0;
}
static int button10OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	isStart = !isStart; 
    Servo5CurveStart((PGRAPH_WGT)servo5_curve_pg.graph1,pwgt);

	return 0;
}
