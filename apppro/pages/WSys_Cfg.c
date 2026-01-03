#include "WSys_Cfg.h"
#include "typedef.h"
#include "pagefram.h"
#include "time.h"
#include "taskmoni.h"
#include "stdio.h"
#include "pay.h"
#include "panel.h"
#include "database.h"
#include "keyprot2.h"
#include "sliderwgt.h"
#include "file.h"
#include <stdlib.h>
#include "prot.h"
#include "usermod.h"//20210914 dyl touch
#include "oprintf.h"
#include "panelrec.h"

DEFINE_SYS_CFG_MAP

CONNECT_SYS_CFG_EVENT

//datetime
#define TMP_SYSCFG_MONTH	TMPVAL(1)  //日期月 临时变量1
#define TMP_SYSCFG_DAY  	TMPVAL(2)  //日期日 临时变量2
#define TMP_SYSCFG_YEAR  	TMPVAL(3)  //日期年 临时变量3
#define TMP_SYSCFG_HOUR 	TMPVAL(4)  //时间时 临时变量4
#define TMP_SYSCFG_MIN  	TMPVAL(5)  //时间分 临时变量5
#define TMP_SYSCFG_SEC  	TMPVAL(6)  //时间秒 临时变量6

//control code
#define TMP_SYSCF_CTRLCODE  TMPVAL(7)  //控制码 临时变量7

//IP and Mac
#define TMP_SYSCFG_IP        TMPVAL(10)   
#define TMP_SYSCFG_MAC       TMPVAL(20)   
#define TMP_SYSCFG_GATEWAY   TMPVAL(60)//20250103 chj 新增网络:网关和子网掩码
#define TMP_SYSCFG_NETMASK   TMPVAL(70)

#ifdef WIN32
#define BACK_IMAGE_PATH		"e:\\pages\\image\\back.png"
#define BACK_IMAGE_PATHX	"e:\\pages\\image\\bg%d.png"
#else
#define BACK_IMAGE_PATH		"/pages/image/back.png"
#define BACK_IMAGE_PATHX	"/pages/image/bg%d.png"
#endif

//Bright //20191223.cyx
#define PANEL_BRIGHT        0x33170051 //背光亮度
#define BRIGHTNESS_AUTO     0x33170052 //背光亮度自动调整
#define CALIBRATE_PASSWORD  TMPVAL(30) //触摸屏校验密码
#define CALIBRATE_FILE      "/opt/tslib/etc/pointercal"

#define SYSCFG_DAY_MAX      TMPVAL(40) //日期日的最大值

static BOOL date_change = FALSE;
static UI8  ntry =0;
static UI16 s_random =0;        //随机数
static UI16 control_code=0;   //控制码
//Brightness //20191223.cyx
static int  m_brightRange = 0;
static BOOL bbright = FALSE;
static UI32 m_brightValue = 0;

static void G15SysCfgUpdate()
{
    static UI32 time_100ms=0;
    static UI32 count = 0;
    PWGT pwgt_slstep;

    pwgt_slstep = PageGetWgtByName(sys_cfg_pg_name,"slstep");

    if(VarAdrToUI16(BRIGHTNESS_AUTO))
    {
        if(GetTick()-time_100ms >= 100)
        {
            count++;
            time_100ms = GetTick();
            if(count >= 2)
            {
                Send_LCDBrightCom3(0x0701);
                count = 0;
            }
            if(abs(m_brightRange - VarAdrToInt(PANEL_BRIGHT)) > 3)
            {
                bbright = TRUE;
            }

            if(bbright)
            {
                if(m_brightRange < VarAdrToInt(PANEL_BRIGHT))
                {
                    m_brightRange++;
                    SliderSetCurValue(pwgt_slstep,m_brightRange);
                }
                else if(m_brightRange > VarAdrToInt(PANEL_BRIGHT))
                {
                    m_brightRange--;
                    SliderSetCurValue(pwgt_slstep,m_brightRange);
                }
                else
                {
                    bbright = FALSE;
                }
            }
        }
    }

    if(m_brightValue != SliderGetCurValue(pwgt_slstep))
    {
        m_brightValue = SliderGetCurValue(pwgt_slstep);
        if(!VarAdrToUI16(BRIGHTNESS_AUTO))
        {
            UI16 brightvalue;
            brightvalue = m_brightValue;
            if(brightvalue > 0x64)
            {
                brightvalue = 0x64;
            }
            else if(brightvalue < 0x8)
            {
                brightvalue = 0x8;
            }
            VarAdrSetInt(PANEL_BRIGHT,brightvalue);
            VarAdrSaveInt(PANEL_BRIGHT);
            m_brightRange = brightvalue;
            Send_LCDBrightCom3(brightvalue|0x0500);
        }
    }
}

//设置日的最大值
static void SetMaxDay()
{
    BOOL bLeapYear;
    UI32 month,year;
    const UI16 s_daysInMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

    month = VarAdrToInt(TMP_SYSCFG_MONTH);
    year = VarAdrToInt(TMP_SYSCFG_YEAR);

    if (((year+2000)%4 == 0) && ((year+2000)%100 != 0) || ((year+2000)%400 == 0))
    {
        bLeapYear = TRUE;
    }
    else
    {
        bLeapYear = FALSE;
    }

    if(bLeapYear && (2 == month))
    {
        VarAdrSetInt(SYSCFG_DAY_MAX, 29);
    }
    else
    {
        VarAdrSetInt(SYSCFG_DAY_MAX, s_daysInMonth[month-1]);
    }

    if(VarAdrToInt(TMP_SYSCFG_DAY) > VarAdrToInt(SYSCFG_DAY_MAX))
    {
        VarAdrSetInt(TMP_SYSCFG_DAY, VarAdrToInt(SYSCFG_DAY_MAX));
    }
}

static void CtlConnect()
{
}
static int Sys_CfgOnInit(Sys_Cfg_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	return 0;
}
static int Sys_CfgOnShow(Sys_Cfg_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_USERLOCK);//20210914 dyl touch

	int i;
	struct tm* t;
	time_t tm_now;

	time(&tm_now);
	t = localtime(&tm_now);

	VarAdrSetInt(TMP_SYSCFG_MONTH,t->tm_mon+1);//月份（从一月开始，0代表一月取值区间为[0,11] 
	VarAdrSetInt(TMP_SYSCFG_DAY,t->tm_mday);
	VarAdrSetInt(TMP_SYSCFG_YEAR,t->tm_year+1900-2000);/* 年份，其值从1900开始 */

    SetMaxDay();//20200107.cyx

    //Random
    s_random = GetRandomData();
    VarAdrSetInt(TMP_SYSCF_CTRLCODE,s_random);

	ntry =0;

    //ip and mac //20191213.cyx 读取IP与MAC
    for(i = 0; i < 4; i++)
    {
        VarAdrSetInt(TMP_SYSCFG_IP+i,NetCtl_ReadIP(i,0));
    }
    for(i = 0; i < 6; i++)
    {
        VarAdrSetInt(TMP_SYSCFG_MAC+i,NetCtl_ReadMac(i,0));
    }
    for(i = 0; i < 4; i++)//20250103 chj 新增网络:网关和子网掩码
    {
        VarAdrSetInt(TMP_SYSCFG_GATEWAY+i,NetCtl_ReadGATEWAY(i,0));
    }
    for(i = 0; i < 4; i++)
    {
        VarAdrSetInt(TMP_SYSCFG_NETMASK+i,NetCtl_ReadNETMASK(i,0));
    }
//    //Bright //20191223.cyx
//#if 0
    if(/*IsG15Type()||*/IsTouchType())//20190629.cyx + //20220104 dyl
    {
        /* //20230518 chj 没有感光
        PWGT pwgt_slstep;
        pwgt_slstep = PageGetWgtByName(sys_cfg_pg_name,"slstep");

        if(VarAdrToUI16(BRIGHTNESS_AUTO))
        {
            Send_LCDBrightCom3(0x0701);
            WGTSetEnable(pwgt_slstep,FALSE);
        }
        else
        {
            WGTSetEnable(pwgt_slstep,TRUE);
        }

        if(VarAdrToInt(PANEL_BRIGHT) < 8)
        {
            VarAdrSetInt(PANEL_BRIGHT,50);
        }
        m_brightRange = VarAdrToInt(PANEL_BRIGHT);
        SliderSetCurValue(pwgt_slstep,VarAdrToInt(PANEL_BRIGHT));
        m_brightValue = SliderGetCurValue(pwgt_slstep);
        */

        VarAdrSetInt(CALIBRATE_PASSWORD,1234);
        WGTSetEnable(PageGetWgtByName(sys_cfg_pg_name,"btcalibration"),FALSE);
        WGTSetEnable(PageGetWgtByName(sys_cfg_pg_name,"edit114"),TRUE);//20220929 dyl
        WGTSetEnable(PageGetWgtByName(sys_cfg_pg_name,"label58"),TRUE);//20220929 dyl
    }
    else
    {
        WGTSetEnable(PageGetWgtByName(sys_cfg_pg_name,"btcalibration"),FALSE);//20220929 dyl
        WGTSetEnable(PageGetWgtByName(sys_cfg_pg_name,"edit114"),FALSE);//20220929 dyl
        WGTSetEnable(PageGetWgtByName(sys_cfg_pg_name,"label58"),FALSE);//20220929 dyl
    }
//#endif

	return 0;
}
static int Sys_CfgOnHide(Sys_Cfg_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch
	return 0;
}
static int Sys_CfgOnUpdate(Sys_Cfg_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	static UI32 time_400ms=0;

	if(GetTick()-time_400ms>=400)
	{
		struct tm* t;
		time_t tm_now;
		time(&tm_now);
		t = localtime(&tm_now);
		VarAdrSetInt(TMP_SYSCFG_HOUR,t->tm_hour);
		VarAdrSetInt(TMP_SYSCFG_MIN,t->tm_min);
		VarAdrSetInt(TMP_SYSCFG_SEC,t->tm_sec);
		time_400ms =GetTick();

#ifndef WIN32
		if(date_change)
		{
			date_change = FALSE;
			VarAdrSetInt(TMP_SYSCFG_MONTH,t->tm_mon+1);//月份（从一月开始，0代表一月取值区间为[0,11] 
			VarAdrSetInt(TMP_SYSCFG_DAY,t->tm_mday);
			VarAdrSetInt(TMP_SYSCFG_YEAR,t->tm_year+1900-2000);/* 年份，其值从1900开始 */
		}
#endif
	}

    //Bright //20191223.cyx
#if 0
    if(IsG15Type())
    {
        G15SysCfgUpdate();
    }
#endif

	return 0;
}
static int Sys_CfgOnChange(Sys_Cfg_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 addr,value;
	UI32 year,month,day;
	UI32 hour,minute,second;
//#ifndef WIN32
//	UI8 year,month,day;
//#endif
	PATTR pattr;

	pattr = (PATTR)msg_para;

	addr = OBJAttrGetAdr(pattr);

	switch(addr)
	{
	case TMP_SYSCFG_MONTH:
	case TMP_SYSCFG_DAY:
	case TMP_SYSCFG_YEAR:
        SetMaxDay();//20200107.cyx

		year = VarAdrToInt(TMP_SYSCFG_YEAR);
		year +=2000;
		month = VarAdrToInt(TMP_SYSCFG_MONTH);
		day = VarAdrToInt(TMP_SYSCFG_DAY);

#ifndef WIN32
        if (IsPayChecking() && ControlPass(year-2000,month,day,s_random)!=control_code){
			ntry++;
			if (ntry>10) PanelShowPage(PanelMainPage());
			return TRUE;
		}
		set_date(year,month,day); 
		date_change =TRUE; 
#endif	
        ProtRxSetLastTm(GetTick());//20201106 解决修改日期可能出现通讯失败警报
        PanelRecAdd(pwgt, PNL_CHANGE_PANEL_TM, 0);//20250306 chj 记录修改时间操作
		break;
	case TMP_SYSCFG_HOUR:
	case TMP_SYSCFG_MIN:
	case TMP_SYSCFG_SEC:
		hour = VarAdrToInt(TMP_SYSCFG_HOUR);
		minute = VarAdrToInt(TMP_SYSCFG_MIN);
		second = VarAdrToInt(TMP_SYSCFG_SEC);

#ifndef WIN32
		year = VarAdrToInt(TMP_SYSCFG_YEAR);
		month = VarAdrToInt(TMP_SYSCFG_MONTH);
		day = VarAdrToInt(TMP_SYSCFG_DAY);
        if (IsPayChecking() && ControlPass(year,month,day,s_random)!=control_code){
			ntry++;
			if (ntry>10) PanelShowPage(PanelMainPage());
			return TRUE;
		}
		set_time(hour,minute,second);
#endif
        ProtRxSetLastTm(GetTick());//20201106 解决修改日期可能出现通讯失败警报
        PanelRecAdd(pwgt, PNL_CHANGE_PANEL_TM, 0);//20250306 chj 记录修改时间操作
		break;
	case TMP_SYSCF_CTRLCODE:
		control_code = VarAdrToUI16(TMP_SYSCF_CTRLCODE);
		break;
    case TMP_SYSCFG_IP: //20191213.cyx 设置IP
    case TMP_SYSCFG_IP+1:
    case TMP_SYSCFG_IP+2:
    case TMP_SYSCFG_IP+3:
        value = VarAdrToInt(addr);
        NetCtl_WriteIP(addr - TMP_SYSCFG_IP,value);
        break;
    case TMP_SYSCFG_MAC://20250103 chj 新增网络:mac修改
    case TMP_SYSCFG_MAC + 1:
    case TMP_SYSCFG_MAC + 2:
    case TMP_SYSCFG_MAC + 3:
    case TMP_SYSCFG_MAC + 4:
    case TMP_SYSCFG_MAC + 5:
        NetCtl_WriteMac((addr - TMP_SYSCFG_MAC),VarAdrToInt(addr));
        break;
    case TMP_SYSCFG_GATEWAY: //设置GATEWAY //20250103 chj 新增网络:网关和子网掩码
    case TMP_SYSCFG_GATEWAY+1:
    case TMP_SYSCFG_GATEWAY+2:
    case TMP_SYSCFG_GATEWAY+3:
        NetCtl_WriteGATEWAY((addr - TMP_SYSCFG_GATEWAY),VarAdrToInt(addr));
        break;
    case TMP_SYSCFG_NETMASK: //设置NETMASK //20250103 chj 新增网络:网关和子网掩码
    case TMP_SYSCFG_NETMASK+1:
    case TMP_SYSCFG_NETMASK+2:
    case TMP_SYSCFG_NETMASK+3:
        NetCtl_WriteNETMASK((addr - TMP_SYSCFG_NETMASK),VarAdrToInt(addr));
        break;
    case BRIGHTNESS_AUTO: //20191223.cyx
        /* //20230518 chj 没有感光
        if(VarAdrToUI16(BRIGHTNESS_AUTO))
        {
            Send_LCDBrightCom3(0x0600);
            WGTSetEnable(PageGetWgtByName(sys_cfg_pg_name,"slstep"),FALSE);
        }
        else
        {
            WGTSetEnable(PageGetWgtByName(sys_cfg_pg_name,"slstep"),TRUE);
            Send_LCDBrightCom3(m_brightRange|0x0500);
        }*/
        break;
    case CALIBRATE_PASSWORD:
        if(VarAdrToInt(CALIBRATE_PASSWORD) == 9595)
        {
            WGTSetEnable(PageGetWgtByName(sys_cfg_pg_name,"btcalibration"),TRUE);
        }
        else
        {
            WGTSetEnable(PageGetWgtByName(sys_cfg_pg_name,"btcalibration"),FALSE);
        }
	default:
		break;
	}

    if(pwgt == PageGetWgtByName(sys_cfg_pg_name,"btcalibration")) //20191223.cyx 触摸屏校验
    {
        char scmd[128];
        if(FileExist(CALIBRATE_FILE))
        {
            snprintf(scmd,sizeof(scmd)-1,CMD_DEL" %s",CALIBRATE_FILE);
            osystem(scmd);
            PanelMainRestart();
        }
    }

	return 0;
}
