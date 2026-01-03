#include "WPump_Cfg_x.h"
#include "typedef.h"
#include "database.h"
#include "canmod.h"
#include "servo.h"
#include "information.h"
#include "oprecord.h"
#include "usermod.h"//20210914 dyl touch


DEFINE_PUMP_CFG_X_MAP

CONNECT_PUMP_CFG_X_EVENT

#define TMP_VICE_STARTSPD2	TMPVAL(0)  //副泵启动速度
#define TMP_VICE_STARTSPD3	TMPVAL(1)  //副泵启动速度
#define TMP_VICE_STARTSPD4	TMPVAL(2)  //副泵启动速度
#define TMP_VICE_STARTSPD5	TMPVAL(3)  //副泵启动速度
#define TMP_VICE_STARTSPD6	TMPVAL(4)  //副泵启动速度
#define TMP_VICE_STARTSPD7	TMPVAL(5)  //副泵启动速度

#define TMP_VICE_STOPSPD2	TMPVAL(10)  //副泵退出速度
#define TMP_VICE_STOPSPD3	TMPVAL(11)  //副泵退出速度
#define TMP_VICE_STOPSPD4	TMPVAL(12)  //副泵退出速度
#define TMP_VICE_STOPSPD5	TMPVAL(13)  //副泵退出速度
#define TMP_VICE_STOPSPD6	TMPVAL(14)  //副泵退出速度
#define TMP_VICE_STOPSPD7	TMPVAL(15)  //副泵退出速度

static void PumpCfgPageIn()
{
	UI8 i;

	if((VarAdrToUI16(SYS_FL_MACH_CODE0)&0x04)==0 && VarAdrToUI16(SYS_FL_MACH_CODE54)& 0x1020)
	{
		if(VarAdrToUI16(SYS_FL_HYDRAUIC)&0x0800){
			for(i=2;i<8;i++){
				if(VarAdrToUI16(SERVO_STATE_CANST1+i-1) & 0x80){ //online
					Send_TaskCan(CAN_TX_READ,i,0xA322,0);//副泵启动
					Send_TaskCan(CAN_TX_READ,i,0xA320,0);//副泵退出
				}
			}
		}
	}
	/*vice pump start or stop speed set */
	for(i=0;i<6;i++){
		VarAdrSetInt(TMP_VICE_STARTSPD2+i,dbsvox[2+i].wData[ID_A322]/10);
		VarAdrSetInt(TMP_VICE_STOPSPD2+i,dbsvox[2+i].wData[ID_A320]/10);
	}
}

static void CtlConnect()
{
}
static int Pump_Cfg_xOnInit(Pump_Cfg_x_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Pump_Cfg_xOnShow(Pump_Cfg_x_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_MODELOCK);//20210914 dyl touch

	PumpCfgPageIn();

    return 0;
}
static int Pump_Cfg_xOnHide(Pump_Cfg_x_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch

    return 0;
}
static int Pump_Cfg_xOnUpdate(Pump_Cfg_x_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageProtect();//20210914 dyl touch //PAGE_PROTECT

    return 0;
}
static int Pump_Cfg_xOnChange(Pump_Cfg_x_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	UI16 value;
	UI8 index;
	UI32 addr;
	PATTR pattr;

	pattr = (PATTR)msg_para;

	addr = OBJAttrGetAdr(pattr);

	if(addr >=TMP_VICE_STARTSPD2 && addr <= TMP_VICE_STARTSPD7)
	{
		index =(UI8)(addr - TMP_VICE_STARTSPD2);
		if(VarAdrToUI16(OPMODE_INDEX)==MODE_MANUAL)
		{
			if(VarAdrToUI16(SERVO_STATE_CANST1+1+index) & 0x80){
				value = VarAdrToUI16(TMP_VICE_STARTSPD2+index);
				Send_TaskCan(CAN_TX_WRITE,index+2,0xA322,value*10);
				Send_TaskCan(CAN_TX_WRITE,index+2,0xA323,value*10);
				Send_TaskCan(CAN_TX_READ, index+2,0xA322,0);
			}
		}
		else
		{
			VarSendSaveSetIntByAdr(addr,msg_value);//原始值
			ShowMsg(VW_MSG_MANUAL);
			return -1;
		}
	}
	else if(addr >=TMP_VICE_STOPSPD2 && addr <= TMP_VICE_STOPSPD7)
	{
		index =(UI8)(addr - TMP_VICE_STOPSPD2);
		if(VarAdrToUI16(OPMODE_INDEX)==MODE_MANUAL)
		{
			if(VarAdrToUI16(SERVO_STATE_CANST1+1+index) & 0x80){
				value=VarAdrToUI16(TMP_VICE_STOPSPD2+index);
				Send_TaskCan(CAN_TX_WRITE,index+2,0xA320,value*10);
				Send_TaskCan(CAN_TX_READ, index+2,0xA320,0);
			}
		}
		else
		{
			VarSendSaveSetIntByAdr(addr,msg_value);//原始值
			ShowMsg(VW_MSG_MANUAL);
			return -1;
		}
	}

    return 0;
}
