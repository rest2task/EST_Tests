#include "WKey_Test.h"
#include "pagefram.h"
#include "cgui.h"
#include "database.h"
#include "strhash.h"
#include "keyprot.h"
#include "keymod.h"
#include "keydef.h"
#include "udisk.h" //20210914 dyl touch
#include "information.h"

#define KEY_MAXCOUNT	(70)

static PHASH_TBL g_pkeySets = NULL;
static PHASH_TBL g_pkeyLED  = NULL;
#define ADD_KEYS(KEYID,KEYADDR)		HashTblAddPairByInt(g_pkeySets,KEYID,(void*)KEYADDR)
#define ADD_LEDS(KEYID,LEDID)		HashTblAddPairByInt(g_pkeyLED,KEYID,(void*)LEDID)
#define START_TMPADDR	(0x33150000)
#define END_TMPADDR		(0x33150039)
#define MAINPRESS_COUNT	(3)			//按3次退出测试界面
UI32 g_keyState=0;
static UI32 LEDState=0;

#define DEV_VALUE 100 //文本色和背景色关联变量偏差值
extern void QtEventLoop();//20210914 dyl touch

static void InitKeyHash();

static BOOL KeyTestShowInit()
{
	int i=0;
	TestKeyStart();
	for(i= START_TMPADDR ;i <= END_TMPADDR;i++)
	{
        VarAdrSetInt(i,GUI_WHITE);//20201216 白底黑字
        VarAdrSetInt(i+DEV_VALUE,GUI_BLACK);
	}
	LEDState=0;
	//g_keyState = getKeyState();//取得原先的lcd状态
	return TRUE;
}

static BOOL KeyTestCloseInit()
{
	TestKeyEnd();
    //KeyCmdSend2(KEY_LCD_TX_CMD,(UI16)(_LCD_ON_STATE>>16),(UI16)(_LCD_ON_STATE));
    SetLCD(TRUE);//20190417 hz
	//setKeyState(g_keyState);//重新设置到原先的状态
	return TRUE;
}

static void TestKey(UI32 key,UI32 press)
{
	BOOL bState;
	static UI32 nMainCnt=0;
	UI32 LEDID;
	//static UI32 LEDState=0;
	void * pvalue = HashTblFindPairByInt(g_pkeySets,key);


	if(pvalue != NULL && press==1)
	{
		UI32 addr = (UI32)pvalue;
		UI32 oldvalue = VarAdrToInt(addr);
        if  (oldvalue != GUI_YELLOW)
		{
            VarAdrSetInt(addr,GUI_YELLOW);//20201216 黄底红字
            VarAdrSetInt(addr+DEV_VALUE,GUI_RED);
			bState=TRUE;
		}
		else
		{
            VarAdrSetInt(addr,GUI_WHITE);//20201216 白底黑字
            VarAdrSetInt(addr+DEV_VALUE,GUI_BLACK);
			bState=FALSE;
		}

		if(key != GUI_KEY_MAINPAGE)
		{
			nMainCnt =0;
		}

		if(key == GUI_KEY_MAINPAGE)
		{
			nMainCnt++;
			if(nMainCnt>=(MAINPRESS_COUNT))
			{
				nMainCnt=0;
				KeyTestCloseInit();
				SendKey(KEY_MAIN);
			}
		}
		//点灯
		pvalue = HashTblFindPairByInt(g_pkeyLED,key);
		if(pvalue!=NULL)
		{
			LEDID = (UI32)pvalue;
			if(bState)
			{
				LEDState |= LEDID;
			}
			else
			{
				LEDState &= ~LEDID;
			}
			KeyCmdSend2(KEY_LED_TX_CMD,(UI16)(LEDState>>16),(UI16)LEDState);
		}
        if(key == KEY_FUN8)//屏幕背光测试
		{
			if(!bState)
			{
                //KeyCmdSend2(KEY_LCD_TX_CMD,(UI16)(_LCD_OFF_STATE>>16),(UI16)(_LCD_OFF_STATE));
                SetLCD(FALSE);//20190417 hz
			}
			else
			{
                //KeyCmdSend2(KEY_LCD_TX_CMD,(UI16)(_LCD_ON_STATE>>16),(UI16)(_LCD_ON_STATE));
                SetLCD(TRUE);//20190417 hz
			}

		}
	}
}

static void InitKeyHash()
{
#ifndef PANEL_NOKEYBOARD//G15:KeyBoard/G10
	if(g_pkeySets == NULL)
	{
		g_pkeySets = HashTblAlloc(KEY_MAXCOUNT);
		if(g_pkeySets!=NULL)
		{
			ADD_KEYS(GUI_KEY_MAINPAGE,0x33150000);//绑定零时地址
            ADD_KEYS(GUI_KEY_FUN1,0x33150001);
            ADD_KEYS(GUI_KEY_FUN2,0x33150002);
			ADD_KEYS(GUI_KEY_FUN3,0x33150003);
			ADD_KEYS(GUI_KEY_FUN4,0x33150004);
			ADD_KEYS(GUI_KEY_FUN5,0x33150005);
			ADD_KEYS(GUI_KEY_FUN6,0x33150006);
			ADD_KEYS(GUI_KEY_FUN7,0x33150007);
			ADD_KEYS(GUI_KEY_NEXTPAGE,0x33150008);

			ADD_KEYS(_N7,0x33150009);
			ADD_KEYS(_N8,0x3315000A);
			ADD_KEYS(_N9,0x3315000B);
			ADD_KEYS(GUI_KEY_UP,0x3315000C);

			ADD_KEYS(_N4,0x3315000D);
			ADD_KEYS(_N5,0x3315000E);
			ADD_KEYS(_N6,0x3315000F);
			ADD_KEYS(GUI_KEY_LEFT,0x33150010);
			ADD_KEYS(GUI_KEY_DOWN,0x33150011);
			ADD_KEYS(GUI_KEY_RIGHT,0x33150012);

			ADD_KEYS(_N1,0x33150013);
			ADD_KEYS(_N2,0x33150014);
			ADD_KEYS(_N3,0x33150015);
			ADD_KEYS(GUI_KEY_FUN8,0x33150016);//system
            ADD_KEYS(KEY_FUN8,0x33150017);//语言
			ADD_KEYS(KEY_HELP,0x33150018);

			ADD_KEYS(_DEL,0x33150019);
			ADD_KEYS(_N0,0x3315001A);
			ADD_KEYS(_ENTER,0x3315001B);

			ADD_KEYS(_KEY_PANEL_MANUAL,0x3315001C);
			ADD_KEYS(_KEY_PANEL_SEMIAUTO,0x3315001D);
			ADD_KEYS(_KEY_PANEL_TIMEAUTO,0x3315001E);
			ADD_KEYS(_KEY_PANEL_SENSORAUTO,0x3315001F);//运行
			ADD_KEYS(_KEY_PANEL_RUN,0x3315001F);//运行
			ADD_KEYS(_KEY_PANEL_MOLDADJNEW,0x33150020);//调模
			ADD_KEYS(_KEY_PANEL_MOLDADJ,0x33150020);//调模
			ADD_KEYS(_KEY_PANEL_MOLDTHICK,0x33150021);
			ADD_KEYS(_KEY_PANEL_MOLDTHIN,0x33150022);

			ADD_KEYS(_KEY_PANEL_MOLDOPEN,0x33150023);
			ADD_KEYS(_KEY_PANEL_MOLDCLOSE,0x33150024);
			ADD_KEYS(_KEY_PANEL_COREAOUT,0x33150025);
			ADD_KEYS(_KEY_PANEL_COREAIN,0x33150026);
			ADD_KEYS(_KEY_PANEL_INJECT,0x33150027);
			ADD_KEYS(_KEY_PANEL_CHARGE,0x33150028);
			ADD_KEYS(_KEY_PANEL_SUCKBACK,0x33150029);

			ADD_KEYS(_KEY_PANEL_EJECTRET,0x3315002A);
			ADD_KEYS(_KEY_PANEL_EJECTADV,0x3315002B);
            ADD_KEYS(_KEY_PANEL_CORESELECT,0x3315002C); //中子切换
            ADD_KEYS(_KEY_PANEL_AIRSELECT,0x3315002D);  //吹气切换
//			ADD_KEYS(_KEY_PANEL_COREBOUT,0x3315002C);
//			ADD_KEYS(_KEY_PANEL_COREBIN,0x3315002D);
			ADD_KEYS(_KEY_PANEL_AUTOPURGE,0x3315002E);
			ADD_KEYS(_KEY_PANEL_NOZZLEADV,0x3315002F);
			ADD_KEYS(_KEY_PANEL_NOZZLERET,0x33150030);

			ADD_KEYS(_KEY_PANEL_AIRBLASTMOV,0x33150031);
			ADD_KEYS(_KEY_PANEL_AIRBLASTSTAT,0x33150032);
//			ADD_KEYS(_KEY_PANEL_CORECOUT,0x33150033);
//			ADD_KEYS(_KEY_PANEL_CORECIN,0x33150034);

			ADD_KEYS(_KEY_PANEL_LUBRICATION,0x33150035);
            ADD_KEYS(_KEY_LUBRICATION1,0x33150035);//20200930
			ADD_KEYS(_KEY_PANEL_SW2,0x33150036);
            ADD_KEYS(_KEY_LUBRICATION2,0x33150036);//20200930
			ADD_KEYS(_KEY_PANEL_SW3,0x33150037);
			ADD_KEYS(_KEY_PANEL_MOTOR,0x33150038);
			ADD_KEYS(_KEY_PANEL_HEATER,0x33150039);
		}
	}
	if(g_pkeyLED == NULL)
	{
		g_pkeyLED = HashTblAlloc(KEY_MAXCOUNT);
		if(g_pkeyLED!=NULL)
		{
			ADD_LEDS(_KEY_PANEL_MANUAL,_LED_MANUAL);
			ADD_LEDS(_KEY_PANEL_SEMIAUTO,_LED_SEMIAUTO);
			ADD_LEDS(_KEY_PANEL_TIMEAUTO,_LED_TIMEAUTO);
			ADD_LEDS(_KEY_PANEL_SENSORAUTO,_LED_SENSORAUTO);
            ADD_LEDS(_KEY_PANEL_RUN,_LED_RUN);
			ADD_LEDS(_KEY_PANEL_MOTOR,_LED_MOTORFLASH);
			ADD_LEDS(_KEY_PANEL_HEATER,_LED_HEATER);
			ADD_LEDS(_KEY_PANEL_MOLDADJ,_LED_ADJMOLD);
			ADD_LEDS(_KEY_PANEL_MOLDADJNEW,_LED_ADJMOLD);
			ADD_LEDS(_KEY_PANEL_CHARGE,_LED_CHARGE);
			ADD_LEDS(_KEY_PANEL_AUTOPURGE,_LED_AUTOPURGE);
			ADD_LEDS(_KEY_PANEL_AIRBLASTMOV,_LED_AIRBLASTMOV);
			ADD_LEDS(_KEY_PANEL_AIRBLASTSTAT,_LED_AIRBLASTSTAT);
			ADD_LEDS(_KEY_PANEL_LUBRICATION,_LED_LUBRICATION);
            ADD_LEDS(_KEY_LUBRICATION1,_LED_LUBRICATION);//20200930
			//panel10
			ADD_LEDS(_KEY_PANEL_CORESELECT,_LED_CORESELECT);
			ADD_LEDS(_KEY_PANEL_AIRSELECT,_LED_AIRSELECT);
            ADD_LEDS(_KEY_PANEL_SW2,_LED_SW1);//20201124 开放备用键的按键灯
            ADD_LEDS(_KEY_LUBRICATION2,_LED_SW1);
            ADD_LEDS(_KEY_PANEL_SW3,_LED_SW2);
		}
	}
#endif
}

DEFINE_KEY_TEST_MAP

CONNECT_KEY_TEST_EVENT


static void CtlConnect()
{
}
static int Key_TestOnInit(Key_Test_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	SetTestKeyCB(TestKey);
	InitKeyHash();
    return 0;
}
static int Key_TestOnShow(Key_Test_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	KeyTestShowInit();

    //20210914 dyl touch
    if(IsTouchType())
    {
        WGTSetVisible(ppg->label_check, FALSE);
        WGTSetVisible(ppg->label_ok, FALSE);
        WGTSetVisible(ppg->label_error, FALSE);

        //20230518 chj 注意：需要配合ts.env
        //目前全键版本存在电容屏触摸和电阻屏触摸，电阻屏触摸不存在触摸测试，需要隐藏
        //注意加此判断，需要同步修改ts.env，通过这个文件判断当前是电阻还是电容触摸
        if(FileExist("/tmp/hasrestouch"))
        {
            WGTSetVisible(ppg->buttontouch, FALSE);
        }
        else{
            WGTSetVisible(ppg->buttontouch, TRUE);
        }
    }
    else
    {
        WGTSetVisible(ppg->label_check, FALSE);
        WGTSetVisible(ppg->label_ok, FALSE);
        WGTSetVisible(ppg->label_error, FALSE);

        WGTSetVisible(ppg->buttontouch, FALSE);
    }

    return 0;
}
static int Key_TestOnHide(Key_Test_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	KeyTestCloseInit();
    return 0;
}
static int Key_TestOnUpdate(Key_Test_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Key_TestOnChange(Key_Test_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(pwgt == ppg->buttontouch)
    {
        if(OperateModeIndex() != MODE_MANUAL)//20230518 chj 非手动情况下，无法测试
        {
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }

        WGTSetVisible(ppg->label_check, TRUE);
        WGTSetVisible(ppg->label_ok, FALSE);
        WGTSetVisible(ppg->label_error, FALSE);

        //20210914 dyl touch
        //force refresh
        PageFramReFresh(PanelCurPage(),TRUE);
        #ifdef USE_QT
            QtEventLoop();  //20190329 hz
        #else
            GUX_Exec();
        #endif
        if (CheckTouchScreen())
        {
            WGTSetVisible(ppg->label_ok, TRUE);
            WGTSetVisible(ppg->label_check, FALSE);
            WGTSetVisible(ppg->label_error, FALSE);
        }
        else
        {
            WGTSetVisible(ppg->label_ok, FALSE);
            WGTSetVisible(ppg->label_check, FALSE);
            WGTSetVisible(ppg->label_error, TRUE);
        }
    }

    return 0;
}
