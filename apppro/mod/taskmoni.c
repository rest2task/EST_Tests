#include "taskmoni.h"
#include "oprintf.h"
#include "keyprot.h"
#include "database.h"
#include "oprecord.h"
#include "key.h"
#include "keymod.h"
#include "prot.h"
#include "time.h"
#include "stdlib.h"
#include "alarmrecord.h"
#include "monmcu.h"
#include "protcmd.h" 
#include "stdio.h"
#include "keydef.h"
#include "verinfo.h"
#include "pushtab.h"
#include "power.h"
#include "prodmoni.h"
#include "panelcfg.h"
#include "poweroff.h"
#include "part.h"
#include "keyprot2.h"
#include "information.h"
#include "keydefine.h"
#include "temp.h"
#include "lubr.h"
#include "prodmanage.h"//20211112 dyl 开机总时间、运行总计时统计
#include "coreprot.h" //20211221 dyl
#include "udisk.h"//20210914 dyl touch

#define  COMM_ALARM_INDEX       0x33100101  //RS232 alarm index

#define SYS_ST_WOPERMODE            0x00160002 //操作状态
#define LUB1_FL_MOLDLUBRIC          0x30160002 //润滑1状态
#define LUB2_FL_MOLDLUBRIC          0x30260002 //润滑2状态
#define LUB3_FL_MOLDLUBRIC          0x30360002 //润滑3状态
#define OPERATE_RECORD_SAVE         0x3312001d //操作状态是否保存
#define CORE_SWITCH                 0x3312005b //中子按键切换
#define BLOW_SWITCH                 0x3312005c //吹气按键切换
#define CORE_SWITCHBK               0x3312005d //中子切换背景色
#define CORE_SWITCHTEXT             0x3312005e //中子切换文本色
#define BLOW_SWITCHBK               0x3312005f //吹气切换背景色
#define BLOW_SWITCHTEXT             0x33120060 //吹气切换文本色

/************************************************************************/
/* 注塑机下位机需要点灯的接收按键                                      */
/************************************************************************/
#define  LUBRI_LED     0x3001
#define  HEAT_LED      0x0020
#define  AUTOPURGE_LED 0x101B
#define  CHARGE_LED    0x1012
#define  MOTOR_LED     0x0411
#define  MOLDADJ_LED   0x01
#define  MANUAL_LED    0x34
#define  SEMIAUTO_LED  0x02
#define  SENSORAUTO_LED  0x04
#define  TIMEAUTO_LED  0x03
#define  RUN_LED       0x05

static UI32 g_led=0;
static UI16 g_random=0;//随机数
static I16  wOpMode =-1;
static BOOL g_panelrestart=FALSE;
extern UI32 g_warm1,g_warm2,g_warm3;
extern UI32 g_warm4,g_warm5,g_warm6,g_warm7,g_warm8,g_warm9,g_warm10;

BOOL g_corekeylock=TRUE;//中子按键失效
BOOL g_airkeylock=TRUE; //吹气按键失效
UI16 g_airkey_active=0;
UI16 g_corekey_active=0;
UI16 g_keycore_sel=0;
UI16 g_keyair_sel=0;
UI16 g_coretime=0;
UI16 g_airtime=0;
UI16 nKeyCnt=0;//20191224.cyx
UI32 dwPanelErr=0;//按键板通讯失败警报

static BOOL b_showswitch = FALSE;//中子、吹气闪烁切换
static BOOL b_switch = FALSE;//core air led

static UI32 JYLEDChange(UI32 led)//20241101 金鹰10寸快速机特殊面膜按键修改
{
    //20241101 金鹰10寸快速机特殊面膜按键修改
   if((VarAdrToInt(p_PP_MACHSET_SPC_KEY_USE_JY) == 1) && (PanelCfgSize()==DISPLAY_SIZE_10) && ((VarAdrToInt(d_machine1_MACHSET_FL_CUSTOMID)&0xFF00) == 0x6400)&& (VarAdrToInt(KEYTEST_ADDR) == 0))
   {
       if(led == _LED_ADJMOLD) led = _LED_SEMIAUTO;
       else if(led == _LED_SEMIAUTO) led = _LED_TIMEAUTO;
       else if(led == _LED_TIMEAUTO) led = _LED_RUN;
       else if(led == _LED_CHARGE) led = 0x7FFFFF;
       else if(led == _LED_AUTOPURGE) led = 0x7FFFFF;
       else if(led == _LED_LUBRICATION) led = 0x7FFFFF;
       else {
           ;
       }
   }
   return led;
}

static void SetLED2(UI32 led,BOOL bState)
{
    //20241101 金鹰10寸快速机特殊面膜按键修改
    led=JYLEDChange(led);

    if(led !=0x7FFFFF)//暂定无按键灯 //20241101 金鹰10寸快速机特殊面膜按键修改
    {
        if (bState)
        {
            g_led|=led;
        }
        else
        {
            g_led&=~led;
        }
    }

}

static void SetLED2ByList(UI32 sendkey,BOOL bState)
{
	UI32 led=0;

	led=KeyToLed(sendkey);

    //20241101 金鹰10寸快速机特殊面膜按键修改
    led=JYLEDChange(led);

	if(led !=0x7FFFFF)//暂定无按键灯
	{
		if(bState)
		{
			g_led|=led;
		}
		else
		{
			g_led&=~led;
		}
	}
}

static void Send_LED() 
{ 
	KeyCmdSend2(KEY_LED_TX_CMD,(UI16)(g_led>>16),(UI16)(g_led));
} 

UI32 GetLED()
{
	return g_led;
}

/************************************************************************/
/* 发送按键灯重置                                                       */
/************************************************************************/
static void Send_LEDReset()
{
    KeyCmdSend2(KEY_LED_TX_CMD,0,0);
}

//新的灯值定义组，走F705协议
static UI32 g_led2=0;
static void SetLED2_2(UI32 led,BOOL bState)
{
    if (bState)
    {
        g_led2|=led;
    }
    else
    {
        g_led2&=~led;
    }
}

static void SetLED2ByList_2(UI32 sendkey,BOOL bState)
{
    UI32 led=0;

    led=KeyToLed(sendkey);
    if(led !=0x7FFFFF)//暂定无按键灯
    {
        if(bState)
        {
            g_led2|=led;
        }
        else
        {
            g_led2&=~led;
        }
    }
}

static void Send_LED_2()
{
    KeyCmdSend2(KEY_LED2_TX_CMD,(UI16)(g_led2>>16),(UI16)(g_led2));
}

UI32 GetLED_2()
{
    return g_led2;
}

/************************************************************************/
/* 发送按键灯重置                                                       */
/************************************************************************/
static void Send_LEDReset_2()
{
    KeyCmdSend2(KEY_LED2_TX_CMD,0,0);
}

/************************************************************************/
/* 发送背光重置                                                         */
/************************************************************************/
static void Send_LCDReset()
{
	KeyCmdSend2(KEY_LCD_TX_CMD,(UI16)(_LCD_RESET_STATE>>16),(UI16)(_LCD_RESET_STATE));
}

/************************************************************************/
/*各动作按键LED指示灯                   //20200227.cyx                     */
/************************************************************************/
static void Act_LED_Indication()
{
    //开模
    if(JudgeActStep(0x20130000, 0x2013FE01) || JudgeActStep(0x20170000, 0x2018FE01))
        SetLED2(_LED_MOLDOPEN,TRUE);
    else
        SetLED2(_LED_MOLDOPEN,FALSE);
    //关模
    if(JudgeActStep(0x20110000, 0x2012FE01) || JudgeActStep(0x20160000, 0x2016FE01)
            || JudgeActStep(0x20190000, 0x2019FE01) || JudgeActStep(0x24180000, 0x2418FE01))
        SetLED2(_LED_MOLDCLOSE,TRUE);
    else
        SetLED2(_LED_MOLDCLOSE,FALSE);
    //托模退
    if(JudgeActStep(0x21130000, 0x2113FE01) || JudgeActStep(0x21150000, 0x2115FE01) || JudgeActStep(0x24170000, 0x2417FE01))
        SetLED2(_LED_EJECTRET,TRUE);
    else
        SetLED2(_LED_EJECTRET,FALSE);
    //托模进
    if(JudgeActStep(0x21120000, 0x2112FE01) || JudgeActStep(0x21140000, 0x2114FE01))
        SetLED2(_LED_EJECTADV,TRUE);
    else
        SetLED2(_LED_EJECTADV,FALSE);
    //射出
    if(JudgeActStep(0x10110000, 0x1011FE01) || JudgeActStep(0x10140000, 0x1014FE01) || JudgeActStep(0x24160000, 0x2416FE01))
        SetLED2(_LED_INJECT,TRUE);
    else
        SetLED2(_LED_INJECT,FALSE);
    //射退
    if(JudgeActStep(0x10130000, 0x1013FE01) || JudgeActStep(0x10190000, 0x1019FE01))
        SetLED2(_LED_SUCKBACK,TRUE);
    else
        SetLED2(_LED_SUCKBACK,FALSE);
    //中子入
    if(JudgeActStep(0x22110000, 0x2211FE01) || JudgeActStep(0x22130000, 0x2213FE01)
            || JudgeActStep(0x22210000, 0x2221FE01) || JudgeActStep(0x22230000, 0x2223FE01)
            || JudgeActStep(0x22310000, 0x2231FE01) || JudgeActStep(0x22330000, 0x2233FE01)
            || JudgeActStep(0x22410000, 0x2241FE01) || JudgeActStep(0x22430000, 0x2243FE01)
            || JudgeActStep(0x22510000, 0x2251FE01) || JudgeActStep(0x22530000, 0x2253FE01)
            || JudgeActStep(0x22610000, 0x2261FE01) || JudgeActStep(0x22630000, 0x2263FE01))
        SetLED2(_LED_COREIN,TRUE);
    else
        SetLED2(_LED_COREIN,FALSE);
    //中子出
    if(JudgeActStep(0x22120000, 0x2212FE01) || JudgeActStep(0x22140000, 0x2214FE01)
            || JudgeActStep(0x22220000, 0x2222FE01) || JudgeActStep(0x22240000, 0x2224FE01)
            || JudgeActStep(0x22320000, 0x2232FE01) || JudgeActStep(0x22340000, 0x2234FE01)
            || JudgeActStep(0x22420000, 0x2242FE01) || JudgeActStep(0x22440000, 0x2244FE01)
            || JudgeActStep(0x22520000, 0x2252FE01) || JudgeActStep(0x22540000, 0x2254FE01)
            || JudgeActStep(0x22620000, 0x2262FE01) || JudgeActStep(0x22640000, 0x2264FE01))
        SetLED2(_LED_COREOUT,TRUE);
    else
        SetLED2(_LED_COREOUT,FALSE);
    //吹气1 3 5 /7 9 11 13 /15 17 19 /21 23 //20241210 chj 28组吹气,增加相应的按键表
    if(JudgeActStep(0x23110000, 0x2311FE01) || JudgeActStep(0x23310000, 0x2331FE01) || JudgeActStep(0x23510000, 0x2351FE01)
     ||JudgeActStep(0x23710000, 0x2371FE01) || JudgeActStep(0x23910000, 0x2391FE01) || JudgeActStep(0x23B10000, 0x23B1FE01) || JudgeActStep(0x23D10000, 0x23D1FE01)
     ||JudgeActStep(0x23170000, 0x2317FE01) || JudgeActStep(0x23370000, 0x2337FE01) || JudgeActStep(0x23570000, 0x2357FE01)
     ||JudgeActStep(0x23770000, 0x2377FE01) || JudgeActStep(0x23970000, 0x2397FE01)
            )
    #ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
        SetLED2_2(_LED_AIRBLASTMOV,TRUE);
    else
        SetLED2_2(_LED_AIRBLASTMOV,FALSE);
    #else//G10/G15:KeyBoard
        SetLED2(_LED_AIRBLASTMOV,TRUE);
    else
        SetLED2(_LED_AIRBLASTMOV,FALSE);
    #endif
    //吹气2 4 6 /8 10 12 14 /16 18 20 /22 24 //20241210 chj 28组吹气,增加相应的按键表
    if(JudgeActStep(0x23210000, 0x2321FE01) || JudgeActStep(0x23410000, 0x2341FE01) || JudgeActStep(0x23610000, 0x2361FE01)
    ||JudgeActStep(0x23810000, 0x2381FE01) || JudgeActStep(0x23A10000, 0x23A1FE01) || JudgeActStep(0x23C10000, 0x23C1FE01) || JudgeActStep(0x23E10000, 0x23E1FE01)
    ||JudgeActStep(0x23270000, 0x2327FE01) || JudgeActStep(0x23470000, 0x2347FE01) || JudgeActStep(0x23670000, 0x2367FE01)
    ||JudgeActStep(0x23870000, 0x2387FE01) || JudgeActStep(0x23A70000, 0x23A7FE01)
           )
    #ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
        SetLED2_2(_LED_AIRBLASTSTAT,TRUE);
    else
        SetLED2_2(_LED_AIRBLASTSTAT,FALSE);
    #else//G10/G15:KeyBoard
        SetLED2(_LED_AIRBLASTSTAT,TRUE);
    else
        SetLED2(_LED_AIRBLASTSTAT,FALSE);
    #endif
    //调模进
    if(JudgeActStep(0x24110000, 0x2411FE01) || JudgeActStep(0x24130000, 0x2413FE01))
    #ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
        SetLED2_2(_LED_MOLDTHIN,TRUE);
    else
        SetLED2_2(_LED_MOLDTHIN,FALSE);
    #else//G10/G15:KeyBoard
        SetLED2(_LED_MOLDTHIN,TRUE);
    else
        SetLED2(_LED_MOLDTHIN,FALSE);
    #endif
    //调模退
    if(JudgeActStep(0x24120000, 0x2412FE01) || JudgeActStep(0x24140000, 0x2414FE01))
    #ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
        SetLED2_2(_LED_MOLDTHICK,TRUE);
    else
        SetLED2_2(_LED_MOLDTHICK,FALSE);
    #else//G10/G15:KeyBoard
        SetLED2(_LED_MOLDTHICK,TRUE);
    else
        SetLED2(_LED_MOLDTHICK,FALSE);
    #endif
    //座进
    if(JudgeActStep(0x11110000, 0x1111FE01) || JudgeActStep(0x11130000, 0x1113FE01))
        SetLED2(_LED_NOZZLEADV,TRUE);
    else
        SetLED2(_LED_NOZZLEADV,FALSE);
    //座退
    if(JudgeActStep(0x11120000, 0x1112FE01) || JudgeActStep(0x11140000, 0x1114FE01))
        SetLED2(_LED_NOZZLERET,TRUE);
    else
        SetLED2(_LED_NOZZLERET,FALSE);

    // 警报清除
    if (IsHaveKeyBoard() && IsG15Type())//20221213 dyl G15全键的备用二键值0x4049,灯值_LED_15_SW2，与警报清除灯值冲突
    {

    }
    else
    {
        if((g_warm1==0) && (g_warm2==0) && (g_warm3==0) && (g_warm4==0) && (g_warm5==0)
                && (g_warm6==0) && (g_warm7==0) && (g_warm8==0) && (g_warm9==0) && (g_warm10==0))
            SetLED2(_LED_ALARMCLEAR,FALSE);
        else
            SetLED2(_LED_ALARMCLEAR,TRUE);
    }
}

//20210118 模温电热使用
static BOOL MDTPUse()
{
    if(isUseMoldFunc())
    {
        int i;

        for(i = 0; i < VarAdrToInt(MT_CTRL_PART); i++)
        {
            if(VarAdrToInt(d_TempCtrlMW1_MOLDSET_FL_USE_MW + i*0x100000))
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

/************************************************************************/
/*LED监控                                                               */
/************************************************************************/
static void LED_Monitor()
{
    UI32    led;
    BOOL    bKeyCnt = FALSE;
	static  UI32   time_600ms =0;
	static  UI32   last_led=0;
	static  UI32   time_1s=0;
	static  BOOL   bAutoAdj=FALSE;//adj and autoadj
	static  BOOL   b_coreswitch=FALSE;//core sel 
	static  BOOL   b_airswitch=FALSE;//air sel
    UI16    heater_state;
//    #ifdef CAPACITIVESCREEN
    UI32    led2;
    static  UI32   last_led2=0;
    static  UI32   time2_1s=0;
//    #endif

	if (wOpMode!=VarAdrToUI16(SYS_ST_WOPERMODE)){
		wOpMode=VarAdrToUI16(SYS_ST_WOPERMODE);
        switch(wOpMode&0xFFFE)
        {
		case MODE_MANUAL:
			if(!IsKeyByXml())
			{
				SetLED2(_LED_MANUAL,TRUE);
				SetLED2(_LED_SEMIAUTO,FALSE);
				SetLED2(_LED_SENSORAUTO,FALSE);
				SetLED2(_LED_TIMEAUTO,FALSE);
                #ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
                    SetLED2_2(_LED_ADJMOLD,FALSE);
                    //SetLED2_2(_LED_ADJMOLDAUTO,FALSE);
                #else//G10/G15:KeyBoard
                    SetLED2(_LED_ADJMOLD,FALSE);
                    //SetLED2(_LED_ADJMOLDAUTO,FALSE);
                #endif
				SetLED2(_LED_AUTOPURGE,FALSE);
			}
			else
			{
				SetLED2ByList(MANUAL_LED,TRUE);
				SetLED2ByList(SEMIAUTO_LED,FALSE);
				SetLED2ByList(SENSORAUTO_LED,FALSE);
				SetLED2ByList(TIMEAUTO_LED,FALSE);
                #ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
                    SetLED2ByList_2(MOLDADJ_LED,FALSE);
                #else//G10/G15:KeyBoard
                    SetLED2ByList(MOLDADJ_LED,FALSE);
                #endif
				SetLED2ByList(AUTOPURGE_LED,FALSE);
			}
			VarAdrSetInt(OPMODE_INDEX,MODE_MANUAL);
			
			ShotCntSave();//
            Time_Save();//20211112 dyl 开机总时间、运行总计时统计
			break;
		case MODE_SEMIAUTO:
			if(!IsKeyByXml())
			{
				SetLED2(_LED_MANUAL,FALSE);
				SetLED2(_LED_SEMIAUTO,TRUE);
				SetLED2(_LED_SENSORAUTO,FALSE);
				SetLED2(_LED_TIMEAUTO,FALSE);
                #ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
                    SetLED2_2(_LED_ADJMOLD,FALSE);
                    //SetLED2_2(_LED_ADJMOLDAUTO,FALSE);
                #else//G10/G15:KeyBoard
                    SetLED2(_LED_ADJMOLD,FALSE);
                    //SetLED2(_LED_ADJMOLDAUTO,FALSE);
                #endif
				SetLED2(_LED_AUTOPURGE,FALSE);
			}
			else
			{
				SetLED2ByList(MANUAL_LED,FALSE);
				SetLED2ByList(SEMIAUTO_LED,TRUE);
				SetLED2ByList(SENSORAUTO_LED,FALSE);
				SetLED2ByList(TIMEAUTO_LED,FALSE);
                #ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
                    SetLED2ByList_2(MOLDADJ_LED,FALSE);
                #else//G10/G15:KeyBoard
                    SetLED2ByList(MOLDADJ_LED,FALSE);
                #endif
				SetLED2ByList(AUTOPURGE_LED,FALSE);
			}
			VarAdrSetInt(OPMODE_INDEX,1);
			break;
		case MODE_SENSORAUTO:
			if(!IsKeyByXml())
			{
				SetLED2(_LED_MANUAL,FALSE);
				SetLED2(_LED_SEMIAUTO,FALSE);
				SetLED2(_LED_SENSORAUTO,FALSE);
				SetLED2(_LED_TIMEAUTO,TRUE);
                #ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
                    SetLED2_2(_LED_ADJMOLD,FALSE);
                    //SetLED2_2(_LED_ADJMOLDAUTO,FALSE);
                #else//G10/G15:KeyBoard
                    SetLED2(_LED_ADJMOLD,FALSE);
                    //SetLED2(_LED_ADJMOLDAUTO,FALSE);
                #endif
				SetLED2(_LED_AUTOPURGE,FALSE);
			}
			else
			{
				SetLED2ByList(MANUAL_LED,FALSE);
				SetLED2ByList(SEMIAUTO_LED,FALSE);
				SetLED2ByList(SENSORAUTO_LED,FALSE);
				SetLED2ByList(TIMEAUTO_LED,TRUE);
                #ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
                    SetLED2ByList_2(MOLDADJ_LED,FALSE);
                #else//G10/G15:KeyBoard
                    SetLED2ByList(MOLDADJ_LED,FALSE);
                #endif
				SetLED2ByList(AUTOPURGE_LED,FALSE);
			}
			VarAdrSetInt(OPMODE_INDEX,2);
			break;
		case MODE_TIMEAUTO:
			if(!IsKeyByXml())
			{
				SetLED2(_LED_MANUAL,FALSE);
				SetLED2(_LED_SEMIAUTO,FALSE);
				SetLED2(_LED_SENSORAUTO,FALSE);
				SetLED2(_LED_TIMEAUTO,TRUE);
                #ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
                    SetLED2_2(_LED_ADJMOLD,FALSE);
                    //SetLED2_2(_LED_ADJMOLDAUTO,FALSE);
                #else//G10/G15:KeyBoard
                    SetLED2(_LED_ADJMOLD,FALSE);
                    //SetLED2(_LED_ADJMOLDAUTO,FALSE);
                #endif
				SetLED2(_LED_AUTOPURGE,FALSE);
			}
			else
			{
				SetLED2ByList(MANUAL_LED,FALSE);
				SetLED2ByList(SEMIAUTO_LED,FALSE);
				SetLED2ByList(SENSORAUTO_LED,FALSE);
				SetLED2ByList(TIMEAUTO_LED,TRUE);
                #ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
                    SetLED2ByList_2(MOLDADJ_LED,FALSE);
                #else//G10/G15:KeyBoard
                    SetLED2ByList(MOLDADJ_LED,FALSE);
                #endif
				SetLED2ByList(AUTOPURGE_LED,FALSE);
			}
			VarAdrSetInt(OPMODE_INDEX,3);
			break;
		case MODE_MACROADJ:
			if(!IsKeyByXml())
			{
				SetLED2(_LED_MANUAL,FALSE);
				SetLED2(_LED_SEMIAUTO,FALSE);
				SetLED2(_LED_SENSORAUTO,FALSE);
				SetLED2(_LED_TIMEAUTO,FALSE);
                #ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
                    SetLED2_2(_LED_ADJMOLD,TRUE);
                    //SetLED2_2(_LED_ADJMOLDAUTO,FALSE);
                #else//G10/G15:KeyBoard
                    SetLED2(_LED_ADJMOLD,TRUE);
                    //SetLED2(_LED_ADJMOLDAUTO,FALSE);
                #endif
				SetLED2(_LED_AUTOPURGE,FALSE);
			}
			else
			{
				SetLED2ByList(MANUAL_LED,FALSE);
				SetLED2ByList(SEMIAUTO_LED,FALSE);
				SetLED2ByList(SENSORAUTO_LED,FALSE);
				SetLED2ByList(TIMEAUTO_LED,FALSE);
                #ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
                    SetLED2ByList_2(MOLDADJ_LED,TRUE);
                #else//G10/G15:KeyBoard
                    SetLED2ByList(MOLDADJ_LED,TRUE);
                #endif
				SetLED2ByList(AUTOPURGE_LED,FALSE);
			}
			VarAdrSetInt(OPMODE_INDEX,4);
			break;
        case MODE_POSSET:
			VarAdrSetInt(OPMODE_INDEX,6);
			break;
		case MODE_AUTOPURGE:
			if(!IsKeyByXml())
			{
				SetLED2(_LED_MANUAL,FALSE);
				SetLED2(_LED_SEMIAUTO,FALSE);
				SetLED2(_LED_SENSORAUTO,FALSE);
				SetLED2(_LED_TIMEAUTO,FALSE);
                #ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
                    SetLED2_2(_LED_ADJMOLD,FALSE);
                    //SetLED2_2(_LED_ADJMOLDAUTO,FALSE);
                #else//G10/G15:KeyBoard
                    SetLED2(_LED_ADJMOLD,FALSE);
                    //SetLED2(_LED_ADJMOLDAUTO,FALSE);
                #endif
				SetLED2(_LED_AUTOPURGE,TRUE);
			}
			else
			{
				SetLED2ByList(MANUAL_LED,FALSE);
				SetLED2ByList(SEMIAUTO_LED,FALSE);
				SetLED2ByList(SENSORAUTO_LED,FALSE);
				SetLED2ByList(TIMEAUTO_LED,FALSE);
                #ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
                    SetLED2ByList_2(MOLDADJ_LED,FALSE);
                #else//G10/G15:KeyBoard
                    SetLED2ByList(MOLDADJ_LED,FALSE);
                #endif
				SetLED2ByList(AUTOPURGE_LED,TRUE);
			}
			VarAdrSetInt(OPMODE_INDEX,7);
			break;
		case MODE_AUTOADJMOLD:
			if(!IsKeyByXml())
			{
				SetLED2(_LED_MANUAL,FALSE);
				SetLED2(_LED_SEMIAUTO,FALSE);
				SetLED2(_LED_SENSORAUTO,FALSE);
				SetLED2(_LED_TIMEAUTO,FALSE);
                #ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
                    SetLED2_2(_LED_ADJMOLD,FALSE);
                    //SetLED2_2(_LED_ADJMOLDAUTO,FALSE);
                #else//G10/G15:KeyBoard
                    SetLED2(_LED_ADJMOLD,FALSE);
                    //SetLED2(_LED_ADJMOLDAUTO,FALSE);
                #endif
				SetLED2(_LED_AUTOPURGE,FALSE);
			}
			else
			{
				SetLED2ByList(MANUAL_LED,FALSE);
				SetLED2ByList(SEMIAUTO_LED,FALSE);
				SetLED2ByList(SENSORAUTO_LED,FALSE);
				SetLED2ByList(TIMEAUTO_LED,FALSE);
                #ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
                    SetLED2ByList_2(MOLDADJ_LED,FALSE);
                #else//G10/G15:KeyBoard
                    SetLED2ByList(MOLDADJ_LED,FALSE);
                #endif
				SetLED2ByList(AUTOPURGE_LED,FALSE);
			}
			VarAdrSetInt(OPMODE_INDEX,11);
			break;
        case MODE_PARTRZ:
            VarAdrSetInt(OPMODE_INDEX,12);
            break;
        case MODE_AA_MOD:
            VarAdrSetInt(OPMODE_INDEX,13);
            break;
        case MODE_AA_LEARN:
            VarAdrSetInt(OPMODE_INDEX,15);
            break;
        case MODE_RAMPADJUST:
            VarAdrSetInt(OPMODE_INDEX,16);
            break;
        case MODE_PRESADJUST:
            VarAdrSetInt(OPMODE_INDEX,17);
            break;
		default:
            VarAdrSetInt(OPMODE_INDEX,14);
		}

        //状态记录由原来的手动/自动开始扩展为手动/半自动开始/自动开始/粗调模/自动调模开始 //20200115.cyx
        if (((wOpMode&0xFFFE)==MODE_SENSORAUTO || (wOpMode&0xFFFE)==MODE_TIMEAUTO) && (VarAdrToUI16(OPERATE_RECORD_SAVE)==1)) OPLEDMonitorRecord(MODE_AUTO);
        else if (((wOpMode&0xFFFE)==MODE_SEMIAUTO) && (VarAdrToUI16(OPERATE_RECORD_SAVE)==1)) OPLEDMonitorRecord(MODE_SEMIAUTO);//半自动
        else if (((wOpMode&0xFFFE)==MODE_MACROADJ) && (VarAdrToUI16(OPERATE_RECORD_SAVE)==1)) OPLEDMonitorRecord(MODE_MACROADJ);//粗调模
        else if (((wOpMode&0xFFFE)==MODE_AUTOADJMOLD) && (VarAdrToUI16(OPERATE_RECORD_SAVE)==1)) OPLEDMonitorRecord(MODE_AUTOADJMOLD);//自动调模
        else if (((wOpMode&0xFFFE)==MODE_MANUAL) && (VarAdrToUI16(OPERATE_RECORD_SAVE)==1)) OPLEDMonitorRecord(MODE_MANUAL);

	}

	/* Adj mold led flashing */
    if((GetTick()-time_600ms >=600) && (VarAdrToUI16(OPMODE_INDEX)==11))
	{
		time_600ms = GetTick();
		if(! bAutoAdj)
			bAutoAdj = TRUE;
		else
			bAutoAdj = FALSE;

        #ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
            if( bAutoAdj)
            {
                if(!IsKeyByXml())
                    SetLED2_2(_LED_ADJMOLD,TRUE);
                else SetLED2ByList_2(MOLDADJ_LED,TRUE);
            }
            else
            {
                if(!IsKeyByXml())
                    SetLED2_2(_LED_ADJMOLD,FALSE);
                else SetLED2ByList_2(MOLDADJ_LED,FALSE);
            }
        #else//G10/G15:KeyBoard
            if( bAutoAdj)
            {
                if(!IsKeyByXml())
                    SetLED2(_LED_ADJMOLD,TRUE);
                else SetLED2ByList(MOLDADJ_LED,TRUE);
    //            SetLED2(_LED_ADJMOLDAUTO,TRUE);
            }
            else
            {
                if(!IsKeyByXml())
                    SetLED2(_LED_ADJMOLD,FALSE);
                else SetLED2ByList(MOLDADJ_LED,FALSE);
    //            SetLED2(_LED_ADJMOLDAUTO,FALSE);
            }
        #endif
	}

	/* comm disconnect close manual led*/
    if(!MainProtOkForRS232())
	{
		if(!IsKeyByXml())
			SetLED2(_LED_MANUAL,FALSE);
		else
			SetLED2ByList(MANUAL_LED,FALSE);
	}

	/*chage led */
    if ((VarAdrToUI16(OPMODE_INDEX)==0 || VarAdrToUI16(OPMODE_INDEX)==4) && JudgeChargeActStep())
	{
		if(!IsKeyByXml())
			SetLED2(_LED_CHARGE,TRUE);
		else
			SetLED2ByList(CHARGE_LED,TRUE);
	}
	else
	{
		if(!IsKeyByXml())
			SetLED2(_LED_CHARGE,FALSE);
		else
			SetLED2ByList(CHARGE_LED,FALSE);
	}
   
	//heater led
    //20200323 电热状态地址切换
    if(IV3200 || IV5200 || IV3300 || IV5300 || IV3100)
    {
        heater_state = VarAdrToUI16(d_TempCtrlComm1_STATE_FL_HEATERSTATUS);//0E160038 E502  E506
    }
    else
    {
        heater_state = VarAdrToUI16(d_tempctrl1_STATE_FL_HEATERSTATUS); //0516002B
    }
	if(!IsKeyByXml())
        SetLED2(_LED_HEATER,heater_state);//电热状态
	else 
        SetLED2ByList(HEAT_LED,heater_state);  

	//lubri led
    if(VarAdrToInt(d_autoctrl1_MOLDSET_KEY_LUBR2_INDEPEND) == 1)//20200930 特殊按键切换
    {
        if(!IsKeyByXml())
            SetLED2(_LED_LUBRICATION,VarAdrToUI16(LUB1_FL_MOLDLUBRIC));//润滑状态
        else
            SetLED2ByList(LUBRI_LED,VarAdrToUI16(LUB1_FL_MOLDLUBRIC));
    }
    else if(VarAdrToUI16(LUB1_FL_MOLDLUBRIC))
	{
		if(!IsKeyByXml())
			SetLED2(_LED_LUBRICATION,VarAdrToUI16(LUB1_FL_MOLDLUBRIC));//润滑状态
		else
			SetLED2ByList(LUBRI_LED,VarAdrToUI16(LUB1_FL_MOLDLUBRIC));
	}
	else if(VarAdrToUI16(LUB2_FL_MOLDLUBRIC))
	{
		if(!IsKeyByXml())
			SetLED2(_LED_LUBRICATION,VarAdrToUI16(LUB2_FL_MOLDLUBRIC));//润滑状态
		else
			SetLED2ByList(LUBRI_LED,VarAdrToUI16(LUB2_FL_MOLDLUBRIC));
	}
	else if(VarAdrToUI16(LUB3_FL_MOLDLUBRIC))
	{
		if(!IsKeyByXml())
			SetLED2(_LED_LUBRICATION,VarAdrToUI16(LUB3_FL_MOLDLUBRIC));//润滑状态
		else 
			SetLED2ByList(LUBRI_LED,VarAdrToUI16(LUB3_FL_MOLDLUBRIC));
	}
	else  
	{
		if(!IsKeyByXml())
			SetLED2(_LED_LUBRICATION,FALSE);//润滑状态
		else 
			SetLED2ByList(LUBRI_LED,FALSE);
	}

	//core sel led  by panel10
	if(b_switch && g_corekey_active)
	{
		if(! b_coreswitch)
			b_coreswitch = TRUE;
		else
			b_coreswitch = FALSE;
		if( b_coreswitch)
			SetLED2(_LED_CORESELECT,TRUE);
		else
			SetLED2(_LED_CORESELECT,FALSE);
	}
	else if(!g_corekey_active)
		SetLED2(_LED_CORESELECT,FALSE);

	////air sel led by panel10
	if(b_switch && g_airkey_active)
	{
		if(! b_airswitch)
			b_airswitch = TRUE;
		else
			b_airswitch = FALSE;

        #ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
            if( b_airswitch)
                SetLED2_2(_LED_AIRSELECT,TRUE);
            else
                SetLED2_2(_LED_AIRSELECT,FALSE);
        #else//G10/G15:KeyBoard
            if( b_airswitch)
                SetLED2(_LED_AIRSELECT,TRUE);
            else
                SetLED2(_LED_AIRSELECT,FALSE);
        #endif
	}
	else if(!g_airkey_active)
    {
        #ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
            SetLED2_2(_LED_AIRSELECT,FALSE);
        #else//G10/G15:KeyBoard
            SetLED2(_LED_AIRSELECT,FALSE);
        #endif
    }

	/* motor led flashing */
    switch(VarAdrToUI16(MOTOR_STATE_STATE)){ //马达状态
	case 0:
		if(!IsKeyByXml())
		{
			SetLED2(_LED_MOTOR,FALSE);
		}
		else
		{
			SetLED2ByList(MOTOR_LED,FALSE);
		}
		SetLED2(_LED_MOTORFLASH,FALSE);
		break;
	case 1:
		if(!IsKeyByXml())
		{
			SetLED2(_LED_MOTOR,TRUE);
		}
		else
		{
			SetLED2ByList(MOTOR_LED,TRUE);
		}
		SetLED2(_LED_MOTORFLASH,FALSE);
		break;
	case 2:
		if(!IsKeyByXml())
		{
			SetLED2(_LED_MOTOR,FALSE);
		}
		else
		{
			SetLED2ByList(MOTOR_LED,FALSE);
		}
		SetLED2(_LED_MOTORFLASH,TRUE);
		break;
	}

//    //20220426
//#ifdef CAPACITIVESCREEN
    if(IsG15Type())
    {
        Act_LED_Indication();//20200227.cyx
    }
//#endif

    //备用1
    if(VarAdrToInt(d_autoctrl1_MOLDSET_KEY_LUBR2_INDEPEND) == 1)//20210118
    {
    #ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
        SetLED2(_LED_SWX1,VarAdrToUI16(LUB2_FL_MOLDLUBRIC));
    #else//G10/G15:KeyBoard
        SetLED2(_LED_SW1,VarAdrToUI16(LUB2_FL_MOLDLUBRIC));//开放备用键的按键灯
    #endif
    }
    else
    {
    #ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
        SetLED2(_LED_SWX1,KeyDefine_LED_State2(0));
    #else//G10/G15:KeyBoard
        SetLED2(_LED_SW1,KeyDefine_LED_State(0));//开放备用键的按键灯
    #endif
    }

    //备用2
    if(MDTPUse())//20210118
    {
    #ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
        SetLED2(_LED_SWX2,GetMtpTempHeatState());//20200812
    #else
        if(PanelCfgSize()==DISPLAY_SIZE_15)//G15:KeyBoard
        {
            SetLED2(_LED_PANEL_CURVE,GetMtpTempHeatState());
        }
        else//G10
        {
            SetLED2(_LED_SW2,GetMtpTempHeatState());
        }
    #endif
    }
    else
    {
    #ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
        SetLED2(_LED_SWX2,KeyDefine_LED_State2(1));
    #else
        if(PanelCfgSize()==DISPLAY_SIZE_15)//G15:KeyBoard
        {
            SetLED2(_LED_PANEL_CURVE,KeyDefine_LED_State(1));
        }
        else//G10
        {
            SetLED2(_LED_SW2,KeyDefine_LED_State(1));
        }
    #endif
    }

#ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
    SetLED2(_LED_SWX3,KeyDefine_LED_State2(2));
    SetLED2(_LED_SWX6,KeyDefine_LED_State2(3));
    SetLED2(_LED_SWX7,KeyDefine_LED_State2(4));
    SetLED2(_LED_SWX8,KeyDefine_LED_State2(5));
#endif

	led=GetLED();

	if (last_led!=led)
	{
        last_led=led;
        //oprintf("Send_LED----:%x\n",led);
        Send_LED();
	}
	else if(GetTick()-time_1s>=1000)
	{
		time_1s = GetTick();
		Send_LED();
        bKeyCnt = TRUE;
	}

    #ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
        led2=GetLED_2();

        if (last_led2!=led2)
        {
            last_led2=led2;
            Send_LED_2();
        }
        else if(GetTick()-time2_1s>=1000)
        {
            time2_1s = GetTick();
            Send_LED_2();
            bKeyCnt = TRUE;
        }
    #endif

    if(bKeyCnt)
    {
        if(nKeyCnt < 10) //20191224.cyx
        {
            nKeyCnt++;//心跳交互计数
        }
    }

}
BOOL MainProtOkForRS232()
{
    static UI32 time_100ms = 0;
    static int time_s = 0;
    static int once = 0;
    UI16 time_10s = 100;

    if((GetTick() - time_100ms > 100) && (once==0))
    {
        time_s++;
        time_100ms = GetTick();
    }

    if(time_s > time_10s)
        once = 1;

    if(VarAdrToUI16(PROGUPDATE_FLAG)==1) //程序更新不提示串口警报
    {
        ProtRxSetLastTm(GetTick());
    }

    if((!MainProtComIsOK()) && once)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }

}
/************************************************************************/
/* RS232监控                                                            */
/************************************************************************/
static void Comm_Monitor()
{
	static UI8   bcomm=1;
	static BOOL  bcommErr=FALSE;
    static BOOL  bPanelComm=TRUE;//20191224.cyx 心跳交互计数:连续发送5次无反馈心跳，认为按键板通讯失败

    if(!MainProtOkForRS232())
	{
        g_warm1 = COMM_ALARM_INDEX;
        g_warm2 = 0;
        g_warm3 = 0;
        //20200407HJM 模温功能
        g_warm4 = 0;
        g_warm5 = 0;
        g_warm6 = 0;
        g_warm7 = 0;
        g_warm8 = 0;
        g_warm9 = 0;
        g_warm10 = 0;
		bcommErr = TRUE;
		if(bcomm)
		{
			bcomm = 0;
            AlarmWRRecord(g_warm1);
		}
	}
	else 
	{
		if(bcommErr)
		{
			bcommErr = FALSE;
			wOpMode =-1;
		}
		bcomm = 1;
	}

    //20191224.cyx 心跳交互计数:连续发送5次无反馈心跳，认为按键板通讯失败
    if(nKeyCnt > 5)
    {
        dwPanelErr = 0x34100101;
        if(bPanelComm)
        {
            AlarmWRRecord(dwPanelErr);
            bPanelComm=FALSE;
        }
    }
    else
    {
        dwPanelErr = 0;
        bPanelComm=TRUE;
    }
}
/************************************************************************/
/* 操作状态                                                             */
/************************************************************************/
UI16 OperateModeIndex()
{
	return VarAdrToUI16(OPMODE_INDEX);
}
/************************************************************************/
/* 随机数                                                               */
/************************************************************************/
UI16 GetRandomData()
{
	return g_random;
}
/************************************************************************/
/* 面板与主机重启                                                       */
/************************************************************************/
void PanelMainRestart()
{
    ProtRebootWait();//20191206 通讯数据交互方式改进，软重启时面板只发送数据，不接收主机数据
    ProtRxSetLastTm(GetTick());//20230518 chj 上面的rebootwait将通讯计时赋值为0，这里需要重新赋值下，不然会出现通讯失败
	ProtUpgradReset(0);//restart main
	g_panelrestart=TRUE;
}
/************************************************************************/
/* 中子与吹气切换                                                       */
/************************************************************************/
void CoreBlowSwitch()
{
	char strcore[5];
	char strair[10];
	char c = 'A';
	UI16 air_x,air_y;

    //20241101 金鹰10寸快速机特殊面膜按键修改,主画面右上角中子按照下拉框选择项来，吹气显示空白
    if((VarAdrToInt(p_PP_MACHSET_SPC_KEY_USE_JY) == 1) && (PanelCfgSize()==DISPLAY_SIZE_10) && ((VarAdrToInt(d_machine1_MACHSET_FL_CUSTOMID)&0xFF00) == 0x6400)&& (VarAdrToInt(KEYTEST_ADDR) == 0))
    {
        snprintf(strcore,sizeof(strcore)-1,"%c",(c+VarAdrToInt(p_PP_MACHSET_CORE_KEY_SELECT)));
        VarAdrSetStr(CORE_SWITCH,strcore);

        snprintf(strair,sizeof(strair)-1,"%c",' ');
        VarAdrSetStr(BLOW_SWITCH,strair);
    }
    else
    {
        if(g_corekey_active)
        {
            snprintf(strcore,sizeof(strcore)-1,"%c",(c+g_keycore_sel-1));
            VarAdrSetStr(CORE_SWITCH,strcore);
            if(b_showswitch)
            {
                VarAdrSetInt(CORE_SWITCHBK,0x0058ff);
                VarAdrSetInt(CORE_SWITCHTEXT,0xffffff);
            }
            else
            {
                VarAdrSetInt(CORE_SWITCHBK,-1);
                VarAdrSetInt(CORE_SWITCHTEXT,DARK_GREEN);
            }
        }
        else{
            if(g_keycore_sel)
            {
                snprintf(strcore,sizeof(strcore)-1,"%c",(c+g_keycore_sel-1));
                VarAdrSetStr(CORE_SWITCH,strcore);
            }
            else{
                VarAdrSetStr(CORE_SWITCH,"A");
            }
            VarAdrSetInt(CORE_SWITCHBK,-1);
            VarAdrSetInt(CORE_SWITCHTEXT,DARK_GREEN);
        }

        //airkey
        air_x = 1+(g_keyair_sel-1)*2;
        air_y = 1+(g_keyair_sel-1)*2+1;
        if(g_airkey_active)
        {
            snprintf(strair,sizeof(strair)-1,"%d/%d",air_x,air_y);
            VarAdrSetStr(BLOW_SWITCH,strair);
            if(b_showswitch)
            {
                VarAdrSetInt(BLOW_SWITCHBK,0x0058ff);
                VarAdrSetInt(BLOW_SWITCHTEXT,0xffffff);
            }
            else
            {
                VarAdrSetInt(BLOW_SWITCHBK,-1);
                VarAdrSetInt(BLOW_SWITCHTEXT,DARK_GREEN);
            }
        }
        else{
            if(g_keyair_sel)
            {
                snprintf(strair,sizeof(strair)-1,"%d/%d",air_x,air_y);
                VarAdrSetStr(BLOW_SWITCH,strair);
            }
            else
            {
                VarAdrSetStr(BLOW_SWITCH,"1/2");
            }
            VarAdrSetInt(BLOW_SWITCHBK,-1);
            VarAdrSetInt(BLOW_SWITCHTEXT,DARK_GREEN);
        }

    }

}
void    ProcessKey(){
	//中子切换
	if(g_corekey_active)
	{
		g_coretime++;
		if(g_coretime>=30)
		{
			g_corekey_active=0;
			g_coretime=0;
			g_corekeylock = TRUE;
		}
		else
		{
			g_corekeylock=FALSE;
		}
	}
	else
	{
		g_coretime=0;
		g_corekeylock=TRUE;
	}
	//吹气切换
	if(g_airkey_active)
	{
		g_airtime++;
		if(g_airtime>=30)
		{
			g_airkey_active=0;
			g_airtime=0;
			g_airkeylock=TRUE;
		}
		else{
			g_airkeylock=FALSE;
		}
	}
	else
	{
		g_airtime=0;
		g_airkeylock=TRUE;
	}
}

/************************************************************************/
/* 开模位置有效区特殊处理                                                             */
/************************************************************************/
void OpenPosEffect()
{
	static BOOL btrans = TRUE;

    if(PushtableType() && btrans)
    {
        UI16 wvalue1,wvalue2;
        UI16 ppart = (CLAMP_MACHSET_EFFCPR>>16)&0xFFF0;
        UI8 subid = (CLAMP_MACHSET_EFFCPR>>16)&0xf;
        UI16 offset = CLAMP_MACHSET_EFFCPR&0xFFFF;
        wvalue1=(UI16)(VarAdrToInt(ADPOSI_OPEN5)-VarAdrToUI16(CLAMP_MACHSET_EFFCPR));
        wvalue2=(UI16)(UnTransMeter(VarAdrToUI16(ADPOSI_OPEN5),FALSE)-UnTransMeter(wvalue1,FALSE));
        ProtSendPartSubData(ppart,subid,offset,1,&wvalue2);
        btrans = FALSE;
    }
}

/************************************************************************/
/* 中子有效区位置  //20211221 dyl                                          */
/************************************************************************/
void CorePosEffect()
{
    static UI16 btrans = 0xFFFF;
    int i;
    if(btrans != PushtableType())
    {
        if (PushtableType())
        {
            //20210730 dyl 中子有效区位置
            for(i=0;i<6;i++)
            {
                //Core In
                CoreEffeAreaByPushtab(d_coreA_ACTPARA_ADPOSI_COREINACT+i*0x100000, d_coreA_MACHSET_ADPOSI_EFFCOREINPLUS+i*0x100000);
                //Core Out
                CoreEffeAreaByPushtab(d_coreA_ACTPARA_ADPOSI_COREOUTACT+i*0x100000, d_coreA_MACHSET_ADPOSI_EFFCOREOUTPLUS+i*0x100000);
            }
        }
        else
        {
            //20210730 dyl 中子有效区位置
            for(i=0;i<6;i++)
            {
                VarSendSaveSetIntByAdr(d_coreA_MACHSET_ADPOSI_EFFCOREINPLUS+i*0x100000, VarAdrToUI16(d_autoctrl1_MOLDSET_SYS_COREPOSEFF));
                VarSendSaveSetIntByAdr(d_coreA_MACHSET_ADPOSI_EFFCOREINMINUS+i*0x100000, VarAdrToUI16(d_autoctrl1_MOLDSET_SYS_COREPOSEFF));
                VarSendSaveSetIntByAdr(d_coreA_MACHSET_ADPOSI_EFFCOREOUTPLUS+i*0x100000, VarAdrToUI16(d_autoctrl1_MOLDSET_SYS_COREPOSEFF));
                VarSendSaveSetIntByAdr(d_coreA_MACHSET_ADPOSI_EFFCOREOUTMINUS+i*0x100000, VarAdrToUI16(d_autoctrl1_MOLDSET_SYS_COREPOSEFF));
            }
        }
        btrans = PushtableType();
    }
}
/************************************************************************/
/* 芯动背压通道特殊处理                                                 */
/************************************************************************/
static void SetBkPrsChanel()
{
	if(PanelCfgStyle() == TIANJIAN_TYPE)
	{
		if(VarAdrToUI16(SYS_FL_CTRLDATE) !=0)
		{
			static UI16  bk_chanel = 0xFFFE;
			UI16 bk_value = (VarAdrToUI16(SYS_FL_MACH_CODE2) & 0x2000);

			if(bk_chanel !=bk_value)
			{
				bk_chanel = bk_value;
				if(bk_value) 
				{
					VarSendSaveSetIntByAdr(INJECT_PORT_CHRGBACK,2);
				}
				else
				{
					VarSendSaveSetIntByAdr(INJECT_PORT_CHRGBACK,4);
				}
			}
		}
	}
}

/************************************************************************/
/* 其他监控函数 eg:comm ,led  , random  ,usb                            */
/************************************************************************/
void OtherTaskMoni()
{
	static UI32  time_1s =0;
    static UI32  time_100ms =0;
	static UI16  second = 0;
	static UI16  half_second = 0;
	static UI16  time_5m =0;
    static UI16  time_1h = 0;//一小时
	static BOOL  bsend=TRUE;
    static BOOL  bsave=TRUE;
	static UI32  time_600ms = 0;
    static UI16  pre_commoni_cnt = 0; //20191205
    static UI16  touchcnt_second=0;//20210914 dyl touch
    static BOOL  btouch_check=FALSE;
    int wUsb;//20210512
    static UI32  dwOpenCount=0;//20220822 dyl 当开模数有变化时保存数据
    //static UI32  time_2s = 0;

	//core air 
	b_switch = FALSE;
	if(GetTick()-time_600ms >=600)
	{
		time_600ms = GetTick();
		b_switch = TRUE;
	}

    /*20191205 配合20191031通讯交互方式修改后，将当前时间赋值给通信接口的上次计时*/
    if(pre_commoni_cnt<1)
    {
        ProtRxSetLastTm(GetTick());
        pre_commoni_cnt ++;
    }
    Comm_Monitor(); //rs232

	if(!VarAdrToInt(KEYTEST_ADDR)) LED_Monitor();

	srand((unsigned)time(NULL));
	g_random = rand()%65535+1; //1-65535

	//request InitData
	if(!IsBootUpMsgRx())
	{
		if(MainProtComIsOK())
		{
			if(ProtRxCnt()>50)
			{
				BootUpMsgRx(0, 0, 0,0);
			}
		}
	}

    //20201104
    if(VarAdrToInt(d_machine1_MOLDSET_SRV1) == 1)
    {
        ProductPowerData();
    }

    MoniWriteFileTick();//20230518 chj 写文件次数监控 //开机发现写文件过多标记，闪烁警报10s

    //shot cnt save 
	if(GetTick()-time_1s >=1000)
	{
		static BOOL btrans = TRUE;//20190614

		b_showswitch = !b_showswitch; //闪烁切换
		ProcessKey();

		if (OperateModeIndex() == MODE_MANUAL)//手动状态保存文件
		{
			ProdEnergySave();
			ProdMoniSave();
        }

		time_1s =  GetTick();
		second++;
        touchcnt_second++;//20210914 dyl touch
		if(second>=60)
		{
			second =0;
			time_5m ++;
#ifdef MPANEL_TC
            DecectTouchTest();//20210914 dyl touch
#endif
            Moni24HourWrite();//20230518 chj 写文件次数监控
		}
		if(time_5m>=5)
		{
            //20220822 dyl 当开模数有变化时保存数据
            if (dwOpenCount != VarAdrToInt(PROD_SHOTCNT))
            {
                dwOpenCount = VarAdrToInt(PROD_SHOTCNT);
                ShotCntSave();
            }
            UpdateHMDLub();//20210507
			time_5m =0;
            time_1h++;
		}
        if(time_1h >= 12)//1小时 //20230518 chj 写文件次数监控
        {
            time_1h = 0;
            SaveMoniWriteData();
            SavePowerMeter();//20230518 chj 假如断电保护文件有问题，需要面板定时保存下电表能耗数据
        }

        //20190614 开模位置有效区特殊处理  卓越条件修改20211224 + //20220228 dyl
        if(IsBootUpMsgRx())
        {
            OpenPosEffect();
            CorePosEffect(); //20211221 dyl
        }
//		//20190614 开模位置有效区特殊处理
//        if((second>5)/* && btrans*/)
//		{
////			btrans = FALSE;
//			OpenPosEffect();
//            CorePosEffect(); //20211221 dyl
//		}

		//20190702芯动DA背压通道特殊改动
		SetBkPrsChanel();
	}

    //20210914 dyl touch
#ifdef MPANEL_TC
    if((touchcnt_second>10) || btouch_check)//开机后延长10s去检测，等面板数据下发完毕
    {
        btouch_check=TRUE;
        DecectTouchTest();//20210914 dyl touch
    }
#endif

    //panel restart
    if(g_panelrestart)
    {
        KeyInit();//20181229 面板重启中时，按键无效

        if(GetTick()-time_100ms >=100)
        {
            half_second++;
            time_100ms =GetTick();
        }

        if((half_second>5) && bsave)
        {
            bsave = FALSE;
            SetLCD(FALSE);//20190417
            Send_LCDOnOff(FALSE);//20190822.cyx
#ifndef WIN32
#ifdef IMX6
            SavePoweroffData();
#else
            SaveProtectData();
#endif
#endif
        }

        if((half_second>19) && bsend) //1900ms close lcd
        {
            Send_LEDReset();
#ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard
            Send_LEDReset_2();
#endif
            Send_LCDReset();
            Send_LCD2ResetCom3();//20190822.cyx
            bsend =FALSE;
        }
        if(half_second>20) //2s reboot
        {
            half_second =0;
#ifdef WIN32
            oprintf("reboot\n");
#else
            osystem("reboot");
#endif
        }
    }

	//core blow key switch  conditions:8'over
	CoreBlowSwitch();

    static BOOL flag = TRUE;
    if(flag)
    {
        if (DataRecvFinish())//20211210 dyl
        {
            flag = FALSE;
            ResourceUse();
        }
    }

	//usb flag check
    wUsb = check_usb();
    if(wUsb)
	{
		VarAdrSetInt(USB_VISIBLE,0);
        if(wUsb == USB_NTFS)
        {
            ShowMsg(VM_MSG_UDISKNTFS);
        }
        else if(wUsb == USB_MultiPartition)
        {
            ShowMsg(VM_MSG_UDISK_DPT);
        }
	}
	else
	{
		VarAdrSetInt(USB_VISIBLE,1);
	}
}

//获得心跳交互计数
UI16 GetKeyCnt()
{
    return nKeyCnt;
}

//设置心跳交互计数
void SetKeyCnt(UI16 cnt)
{
    nKeyCnt = cnt;
}

void Reset_KeyCnt()
{
    SetKeyCnt(0);
}
