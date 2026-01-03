#include "kjump.h"
#include "panel.h"
#include "oprintf.h"
#include "pay.h"
#include "strevent.h"
#include "database.h"
#include "obj.h"
#include "pushtab.h"
#include "login.h"
#include "verinfo.h"
#include "keymod.h"
#include "panelcfg.h"
#include "clua.h"
#include "lauxlib.h"
#include "usermod.h"
#include "tablectl.h"
#include "command.h"

static CPROC_INFO kjump_procs[] =
{
	{"key",		KJumpKey,	CPAR_INT},
	{"page",	KJumpPage,	CPAR_STR},
	//{"enable",	KJumpEnable, CPAR_INT},
	{"next",	KJumpNext},
	{"main",	KJumpMain}
};

static UI32 s_address_color0			= 0x33120000UL;	//导航按键关联文本颜色
static UI32 s_address_text0				= 0x3312000AUL;	//导航按键关联文本
static UI32 s_address_press_image0		= 0x33170029UL;	//导航按键关联按下时背景图片//20191127 0x33120069UL 0x33170033UL
static UI32 s_address_release_image0	= 0x33170033UL;	//导航按键关联正常时背景图片//20191127 0x33120073UL  0x33170029UL
static UI32 s_highlight_color			= 0x0000FFFF;	//文本高亮颜色
static UI32 s_default_color				= 0x00FFFFFF;	//文本默认颜色
static const char* s_default_image		= "btnNULL.png";//默认背景图片
static BOOL s_home_key_update           = TRUE;         //主画面按下后是否刷新导航条

static BOOL m_enableJump = TRUE;

/************************************************************************/
/* private methods                                                       */
/************************************************************************/
/**
* @brief     :导航栏按钮的背景图片、文本色应用端函数
* @param     :几号按钮，当前按钮对象
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
static void NaviBtnUpdateUser(int btnid, PNAVI_BTN pbtn)
{
	PPAGE_FRAM pf = (PPAGE_FRAM)NaviGetPageFram(pbtn);//pbtn->pframe;
	BOOL is_current_page = (pf == PanelCurPage());

    if(!pf || (!(IsParamViewHide() && PageFramGetHideFlag(pf))))
	{
		VarAdrSetStr(s_address_text0+btnid, pbtn->text);
		VarAdrSetStr(s_address_press_image0+btnid, pbtn->press_img);
		VarAdrSetStr(s_address_release_image0+btnid,
			is_current_page ? pbtn->press_img : pbtn->release_img);
	}
	else
	{
		VarAdrSetStr(s_address_text0+btnid, "");
		VarAdrSetStr(s_address_press_image0+btnid, s_default_image);
		VarAdrSetStr(s_address_release_image0+btnid, s_default_image);
	}

	if(pf == PanelMainPage() && PanelCfgStyle() == TIANJIAN_TYPE && btnid == 8) //20190320 天剑风格返回键特殊处理
		is_current_page = 0;

	VarAdrSetInt(s_address_color0+btnid,
		is_current_page ? s_highlight_color : s_default_color);
}

/**
* @brief     :导航栏按钮跳转，应用层函数
* @param     :几号按钮，按钮对象指针
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
static void NaviBtnJumpUser(int btnid, PNAVI_BTN pbtn)
{
    PPAGE_FRAM pcur = PanelCurPage();

	PPAGE_FRAM pf = (PPAGE_FRAM)NaviGetPageFram(pbtn);//pbtn->pframe;
    if(pf == PanelCurPage() && PageFramGetPriv() != 0)  //参数画面密码框出现后再按F键密码框不消失
        return;

    if ((!s_home_key_update) && pf) //20190424
	{
        NaviSetLock(FALSE);
    }

    if((pcur == PanelFindPage(PAGE_ALARMHELP)) || (pcur == PanelFindPage(PAGE_PAGEHELP))) //警报帮助/页面帮助窗口F7返回特殊处理，显示上次页面
    {
        if(pf == PanelMainPage())//20191118.cyx 除返回键其他键无效
        {
            PanelShowPage(PanelLastPage());
            return;
        }
	}

	if(pf && m_enableJump && !IsViewProtect())// !IsParamViewHide()&& RU32(pf->is_visible))
	{
		if(!(PageFramGetHideFlag(pf) && IsParamViewHide()))
		{
            //P10-3200特殊处理 按F1切换动作画面和斜率画面
            if(PanelCfgStyle() == P_TYPE)
            {
                //开关模
                if(btnid==0 && (pcur==PanelFindPage(PAGE_CLAMP)||pcur==PanelFindPage(PAGE_CLAMP_SMART)) && VarAdrToInt(CLAMP_MOLD_FL_CLSE_SMOOTH))
                {
                    PanelShowPageByName(PAGE_CLAMP_ADJRAMP);
                }
                else if(btnid==0 && pcur==PanelFindPage(PAGE_CLAMP_ADJRAMP) && VarAdrToInt(CLAMP_MOLD_FL_CLSE_SMOOTH))
                {
                    if((VarAdrToInt(FL_CLSOPEN_MODE) == 3) || (VarAdrToInt(FL_CLSOPEN_MODE) == 4))
                    {
                        PanelShowPageByName(PAGE_CLAMP_SMART);
                    }
                    else
                    {
                        PanelShowPageByName(PAGE_CLAMP);
                    }
                }
                //托模
                else if(btnid==0 && pcur==PanelFindPage(PAGE_EJECT) && VarAdrToInt(EJECT1_MOLD_FL_EJEF_SMOOTH))
                {
                    PanelShowPageByName(PAGE_EJECT_ADJRAMP);
                }
                else if(btnid==0 && pcur==PanelFindPage(PAGE_EJECT_ADJRAMP) && VarAdrToInt(EJECT1_MOLD_FL_EJEF_SMOOTH))
                {
                    PanelShowPageByName(PAGE_EJECT);
                }
                //储料
                else if(btnid==0 && pcur==PanelFindPage(PAGE_CHARGE) && VarAdrToInt(INJECT_MOLD_FL_SUCK_SOOMTH))
                {
                    PanelShowPageByName(PAGE_CHARGE_ADJRAMP);
                }
                else if(btnid==0 && pcur==PanelFindPage(PAGE_CHARGE_ADJRAMP) && VarAdrToInt(INJECT_MOLD_FL_SUCK_SOOMTH))
                {
                    PanelShowPageByName(PAGE_CHARGE);
                }
                //电子尺
                else if(btnid==0 && pcur==PanelFindPage(PAGE_AD_ADJUSTX))
                {
                    PanelShowPageByName(PAGE_AD_EX);
                }
                else if(btnid==0 && pcur==PanelFindPage(PAGE_AD_EX))
                {
                    PanelShowPageByName(PAGE_AD_ADJUSTX);
                }
                else
                {
                    NaviBtnJumpDefault(btnid, pbtn);
                }
            }
            else
            {
                NaviBtnJumpDefault(btnid, pbtn);
            }
		}
        if(IsPayTimeOut())
		{
            if(pf != PanelFindPage(PAGE_SYS_CFGX)) //20191220 G系列新增 //20230518 chj  修复原来分期后，无法切换到系统画面
            {
                PanelShowPageByName(PAGE_INSTALLPAY);
            }
		}
	}
	else if(IsViewProtect())
	{
		PanelShowDialog(LoginPage());
	}
}

/**
* @brief     :主画面按钮，应用层处理函数
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
static void NaviMainUser()
{
    if (!s_home_key_update) {
        NaviSetLock(TRUE);
    }

    if(m_enableJump)
	{
		NaviMainDefault();
		if(IsPayTimeOut())
		{
			PanelShowPageByName(PAGE_INSTALLPAY);
		}
	}
}

/**
* @brief     :加载lua配置文件，定义默认的导航栏按钮行为及外观
* @param     :lua文件路径
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
static void NaviLoadConfig(const char* path)
{
	lua_State* L = lua_open();

	int res = luaL_dofile(L, path);
	if (res == 0) {
        s_address_color0 = LuaGetInt(L, "address_color0", s_address_color0);
        s_address_text0 = LuaGetInt(L, "address_text0", s_address_text0);
        s_address_press_image0 = LuaGetInt(L, "address_press_image0", s_address_press_image0);
        s_address_release_image0 = LuaGetInt(L, "address_release_image0", s_address_release_image0);
        s_highlight_color = LuaGetInt(L, "highlight_color", s_highlight_color);
        s_default_color = LuaGetInt(L, "default_color", s_default_color);
        s_default_image = LuaGetStr(L, "default_image", s_default_image);
        s_home_key_update = LuaGetBoolean(L, "home_key_update", s_home_key_update);
	}

	lua_close(L);
}

/************************************************************************/
/* MOD interface                                                        */
/************************************************************************/
/**
* @brief     :导航栏按钮配置、回调函数初始化
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
void KJumpInital( PCMOD pmod )
{
	RegisterModProcs(pmod, kjump_procs, ARRAY_NUM(kjump_procs));
	NaviSetBtnJumpCB(NaviBtnJumpUser);
	NaviSetBtnUpdateCB(NaviBtnUpdateUser);
	NaviSetMainCB(NaviMainUser);

	NaviLoadConfig(CONFIG_PATH"kjump_config.lua");
}

BOOL KJumpKey( PCPAR para )
{
	int key = para[0].v;

	if(!IsViewProtect())
	{
        if(PanelCfgStyle() == G_TYPE)
        {
            //20190416.cyx 取消导航条跳转，跳转在WMainframe实现
        }
        else
        {
            NaviJump(key-1);
        }
	}
	else
	{
		PanelShowDialog(LoginPage());
	}

	return TRUE;
}

BOOL KJumpPage( PCPAR para )
{
	const char* form_name = para[0].pstr;
	PPAGE_FRAM ppage;
	if(form_name)
	{
		if(m_enableJump && !IsPayTimeOut())
		{
			ppage = PanelFindPage(form_name);
			PanelShowPage(ppage);
			return TRUE;
		}
	}
	return FALSE;
}

void KJumpEnable(BOOL enable)
{
	if(IsViewProtect())
		m_enableJump = FALSE;
	else
		m_enableJump = enable;
}

BOOL KJumpNext( PCPAR para )
{
	NaviNext();
	return TRUE;
}

BOOL KJumpMain( PCPAR para )
{
	NaviMain();
	return TRUE;
}
/************************************************************************/
/* 泵浦按钮引导切换                                                     */
/************************************************************************/
void PumpGuideSwitch()
{
	if((VarAdrToUI16(SYS_FL_MACH_CODE0)&0x04)==0 && (VarAdrToUI16(SYS_FL_MACH_CODE54)& 0x1020)==0)
	{
		NaviSetIndex(PanelCurPage(),1);
	}
	else
	{
		NaviSetIndex(PanelCurPage(),0);
	}
}
/************************************************************************/
/* 温度按钮引导切换                                                     */
/************************************************************************/
void TempGuideSwitch()
{
    if(VarAdrToInt(TEMP_CURVE))  //20190523 hz
    {
        if(VarAdrToUI16(SYS_FL_MACH_CODE54)&0x0400)
        {
            if(VarAdrToUI16(SYS_FL_TEMPER)&0x08)	//20190121 温度优化
            {
                NaviSetIndex(PanelCurPage(),7);
            }
            else
            {
                NaviSetIndex(PanelCurPage(),5);
            }
        }
        else
        {
            if(VarAdrToUI16(SYS_FL_TEMPER)&0x08)
            {
                NaviSetIndex(PanelCurPage(),6);
            }
            else
            {
                NaviSetIndex(PanelCurPage(),4);
            }
        }
    }
    else
    {
        if(VarAdrToUI16(SYS_FL_MACH_CODE54)&0x0400)
        {
            if(VarAdrToUI16(SYS_FL_TEMPER)&0x08)	//20190121 温度优化
            {
                NaviSetIndex(PanelCurPage(),3);
            }
            else
            {
                NaviSetIndex(PanelCurPage(),1);
            }
        }
        else
        {
            if(VarAdrToUI16(SYS_FL_TEMPER)&0x08)
            {
                NaviSetIndex(PanelCurPage(),2);
            }
            else
            {
                NaviSetIndex(PanelCurPage(),0);
            }
        }
    }
}
/************************************************************************/
/* 推力座/托模按钮引导切换                                              */
/************************************************************************/
void PushGuideSwtich()
{
	if(PushtableType())
	{
        if(VarAdrToInt(USE_PUSHPARA))//20190813.cyx
        {
            NaviSetIndex(PanelCurPage(),3);
        }
        else
        {
            NaviSetIndex(PanelCurPage(),0);
        }
	}
	else 
	{
        if(GetPanelType() == PANEL_H10)
		{
			NaviSetIndex(PanelCurPage(),2);//托模引导
		}
		else
		{
			NaviSetIndex(PanelCurPage(),1);
		}
	}
}
/************************************************************************/
/* 射出曲线按钮/顺序阀按钮引导切换                                      */
/************************************************************************/
void InjCurveGuideSwtich()
{
	UI16 seqvlv,value,func2;
	seqvlv = (VarAdrToUI16(SYSTEM_MACH_CODE43)&0x040);
    value = VarAdrToUI16(INJECT_CUREMODE);
	func2 = ((VarAdrToUI16(SYS_FL_MACH_CODE4)&0x8000) ||(VarAdrToUI16(SYS_FL_MACH_CODE4)&0x4000)||(VarAdrToUI16(SYS_FL_MACH_CODE3)&0x0100));

	if((func2) && seqvlv && (value==1))
	{
		NaviSetIndex(PanelCurPage(),7);
	}
	else if((func2) && seqvlv && (value==0))
	{
		NaviSetIndex(PanelCurPage(),6);
	}
	else if((func2) && (seqvlv==0) && (value==1))
	{
		NaviSetIndex(PanelCurPage(),5);
	}
	else if((func2) && (seqvlv==0) && (value==0))
	{
		NaviSetIndex(PanelCurPage(),4);
	}
	else if((func2==0) && (seqvlv) && (value==1))
	{
		NaviSetIndex(PanelCurPage(),3);
	}
	else if((func2==0) && (seqvlv) && (value==0))
	{
		NaviSetIndex(PanelCurPage(),2);
	}
	else if((func2==0) && (seqvlv==0) && (value==1))
	{
		NaviSetIndex(PanelCurPage(),1);
	}
	else if((func2==0) && (seqvlv==0) && (value==0))
	{
		NaviSetIndex(PanelCurPage(),0);
	}
}
/************************************************************************/
/* 机器系统参数五按钮引导切换                                           */
/************************************************************************/
void SysParamGuideSwtich()
{
	if(VarAdrToUI16(SYS_FL_MACH_CODE3) & 0x0400) //param5
	{
		NaviSetIndex(PanelCurPage(),1); //param5
	}
	else
	{
		NaviSetIndex(PanelCurPage(),0); 
	}
}
/************************************************************************/
/* 伺服3/5调整按钮引导切换                                              */
/************************************************************************/
void ServoDebuguideSwtich()
{
	if((VarAdrToUI16(SYS_FL_MACH_CODE0) & 0x04) && (VarAdrToUI16(SYS_FL_MACH_CODE54) & 0x1020)) //servo3
	{
		NaviSetIndex(PanelCurPage(),1); //
	}
	else
	{
		NaviSetIndex(PanelCurPage(),0); //
	}
}

/************************************************************************/
/* 7组吹气是否使用按钮引导切换                                          */
/************************************************************************/
void BlowGuideSwtich()
{
	//  20190326 hz
	if (VarAdrToUI16(SYSCFG_DISMODE) == 0)
	{
		if((VarAdrToUI16(SYSPAR_BLOW14_FUNC)==1)) //使用7组吹气功能
		{
			NaviSetIndex(PanelCurPage(),1); //
		}
		else
		{
			NaviSetIndex(PanelCurPage(),0); //
		}
	} 
	else
	{
		if((VarAdrToUI16(SYSPAR_BLOW14_FUNC)==1)) //使用7组吹气功能
		{
			NaviSetIndex(PanelCurPage(),3); //
		}
		else
		{
			NaviSetIndex(PanelCurPage(),2); //
		}
	}
	
}

/************************************************************************/
/* QC监控和功耗按钮引导切换                                             */
/************************************************************************/
void PowerQCGuideSwtich()
{
	UI16 use_qc,servo;

	use_qc = VarAdrToUI16(SYS_QC_FUNC);
	servo = (VarAdrToUI16(SYS_FL_MACH_CODE54) & 0x1020);

	if(use_qc && servo)
	{
		NaviSetIndex(PanelCurPage(),3); 
	}
	else if(use_qc && (servo==0))
	{
		NaviSetIndex(PanelCurPage(),2); 
	}
	else if((use_qc==0) && servo)
	{
		NaviSetIndex(PanelCurPage(),1); 
	}
	else
	{
		NaviSetIndex(PanelCurPage(),0); //
	}
}

/************************************************************************/
/* 8寸10寸按键测试按钮引导切换                                             */
/************************************************************************/
void KeyTestGuideSwtich()
{	
    if((GetPanelType() == PANEL_H10) || (GetPanelType() == PANEL_V10))
	{
		NaviSetIndex(PanelCurPage(),1); 
	}
	else
	{
		NaviSetIndex(PanelCurPage(),0); 
	}
}

/************************************************************************/
/* 中子功能按钮引导切换                                                 */
/************************************************************************/
void CoreFuncGuideSwtich()
{
	//  20190326 hz
	if (VarAdrToUI16(SYSCFG_DISMODE) == 0)
	{
        if(GetPanelType() == PANEL_H10)
		{
			NaviSetIndex(PanelCurPage(),1); 
		}
		else
		{
			NaviSetIndex(PanelCurPage(),0); 
		}
	} 
	else
	{
        if(GetPanelType() == PANEL_H10)
		{
			NaviSetIndex(PanelCurPage(),3); 
		}
		else
		{
			NaviSetIndex(PanelCurPage(),2); 
		}
	}
	
}
/************************************************************************/
/* 主页按钮引导切换    20190307 hz                                     */
/************************************************************************/
void MainGuideSwitch()
{
	// 横排竖排显示 20190326 hz
	if (VarAdrToUI16(SYSCFG_DISMODE) == 0)
	{
        if(GetPanelType() == PANEL_H10)
		{
			if(VarAdrToUI16(SYS_FL_MACH_CODE54)&0x0400)
			{
				NaviSetIndex(PanelCurPage(),3);
			}
			else
			{
				NaviSetIndex(PanelCurPage(),2);
			}
		}
		else
		{
			if(VarAdrToUI16(SYS_FL_MACH_CODE54)&0x0400)
			{
				NaviSetIndex(PanelCurPage(),1);
			}
			else
			{
				NaviSetIndex(PanelCurPage(),0);
			}
		}
	} 
	else
	{
        if(GetPanelType() == PANEL_H10)
		{
			if(VarAdrToUI16(SYS_FL_MACH_CODE54)&0x0400)
			{
				NaviSetIndex(PanelCurPage(),7);
			}
			else
			{
				NaviSetIndex(PanelCurPage(),6);
			}
		}
		else
		{
			if(VarAdrToUI16(SYS_FL_MACH_CODE54)&0x0400)
			{
				NaviSetIndex(PanelCurPage(),5);
			}
			else
			{
				NaviSetIndex(PanelCurPage(),4);
			}
		}
	}	
}
/************************************************************************/
/* 扩展温度按钮引导切换-10寸竖屏                                            */
/************************************************************************/
void TempXGuideSwitch()
{
    if(VarAdrToUI16(SYS_FL_MACH_CODE54)&0x0400)
	{
        if(VarAdrToInt(TEMP_CURVE))  //20190523 hz
        {
            if(VarAdrToUI16(SYS_FL_TEMPER)&0x08)	//20190121 温度优化
            {
                NaviSetIndex(PanelCurPage(), 3);
            }
            else
            {
                NaviSetIndex(PanelCurPage(), 2);
            }
        }
        else
        {
            if(VarAdrToUI16(SYS_FL_TEMPER)&0x08)
            {
                NaviSetIndex(PanelCurPage(), 1);
            }
            else
            {
                NaviSetIndex(PanelCurPage(), 0);
            }
        }
    }
}
/************************************************************************/
/* 其它引导切换--横排竖排  20190326 hz                                   */
/************************************************************************/
void OtherGuideSwitch()
{
	if (VarAdrToUI16(SYSCFG_DISMODE) == 0)
	{
		NaviSetIndex(PanelCurPage(),0);
	} 
	else
	{
		NaviSetIndex(PanelCurPage(),1);
	}
}
/************************************************************************/
/* 快设引导切换--横排竖排  20190326 hz                                   */
/************************************************************************/
void FastGuideSwitch()
{
	if (VarAdrToUI16(SYSCFG_DISMODE) == 0)
	{
        if(GetPanelType() == PANEL_H10)
		{
			NaviSetIndex(PanelCurPage(),1);
		}
		else
		{
			NaviSetIndex(PanelCurPage(),0);
		}
	} 
	else
	{
        if(GetPanelType() == PANEL_H10)
		{
			NaviSetIndex(PanelCurPage(),3);
		}
		else
		{
			NaviSetIndex(PanelCurPage(),2);
		}
	}
}

/************************************************************************/
/*立式机滑模跳转     //20190424 CHZ                            */
/************************************************************************/
void RotateGuideSwitch()
{
    if (VarAdrToUI16(SYS_FL_ROTLAC) == 2)
	{
		NaviSetIndex(PanelCurPage(),1);
	}
	else
	{
		NaviSetIndex(PanelCurPage(),0);
	}

}
/************************************************************************/
/*立式机托模跳转     //20190424 CHZ                            */
/************************************************************************/
void VEjectGuideSwitch()
{
    if (VarAdrToUI16(SYS_FL_ROTLAC) == 2 || VarAdrToUI16(SYS_FL_ROTLAC) == 3)
	{
		NaviSetIndex(PanelCurPage(),1);
	}
	else
	{
		NaviSetIndex(PanelCurPage(),0);
	}

}
/************************************************************************/
/*立式机主页面跳转     //20190424 CHZ                            */
/************************************************************************/
void VMainGuideSwitch()
{
    if ((VarAdrToUI16(SYS_FL_ROTLAC)>= 1) && (VarAdrToUI16(SYS_FL_ROTLAC) <=6))
	{
        if(VarAdrToUI16(SYS_FL_ROTLAC) == 2)
		{
			NaviSetIndex(PanelCurPage(),4);
		}
        else if (VarAdrToUI16(SYS_FL_ROTLAC)== 3)
		{
			NaviSetIndex(PanelCurPage(),3);
		}
		else
		{
			NaviSetIndex(PanelCurPage(),1);
		}
	}
    else if(VarAdrToUI16(SYS_FL_ROTLAC) >= 7 && VarAdrToUI16(SYS_FL_ROTLAC) <=9)
	{
		NaviSetIndex(PanelCurPage(),2);
	}
	else
	{
		NaviSetIndex(PanelCurPage(),0);
	}
}
/************************************************************************/
/*立式机射出曲线跳转     //20190424 CHZ                            */
/************************************************************************/
void VInjCurveGuideSwitch()
{
	if (VarAdrToUI16(INJECT_CUREMODE))
	{
		NaviSetIndex(PanelCurPage(),1);
	}
	else
	{
		NaviSetIndex(PanelCurPage(),0);
	}
}

/**************G10新增跳转表***开始***************/ //20191114.cyx
void G10ServoDebug1GuideSwitch()//伺服配置1
{
    PPAGE_FRAM pf;
    pf = PanelCurPage();
    if(GetCurrentUserId() >= USER_DEALER)//20211117 dyl 伺服状态画面等级放低到LV2
    {
        if(VarAdrToInt(SERVO_CURRENT_NUM) > 6)
        {
            NaviSetIndex(pf,1);
        }
        else
        {
            NaviSetIndex(pf,0);
        }
    }
    else
    {
        NaviSetIndex(pf,3);
    }

}
void G10ServoStateGuideSwitch()//20211117 dyl 伺服状态画面等级放低到LV2 --伺服机模式的伺服状态
{
    PPAGE_FRAM pf;
    pf = PanelFindPage(PAGE_SERVO_STATE);
    if(GetCurrentUserId() >= USER_DEALER)//20211117 dyl 伺服状态画面等级放低到LV2
    {
        NaviSetIndex(pf,0);
    }
    else
    {
        NaviSetIndex(pf,1);
    }

}

void G10ClampGuideSwitch()//开关模
{
    PPAGE_FRAM pf;
    pf = PanelFindPage(PAGE_CLAMP);
    if(VarAdrToInt(TEMP_CTRL_PART) > 0)
    {
        NaviSetIndex(pf,1);
    }
    else
    {
        NaviSetIndex(pf,0);
    }
}

void G10MainGuideSwitch()//主画面
{
    PPAGE_FRAM pf;
    pf = PanelFindPage(PAGE_MAIN);
    if(VarAdrToInt(FL_CLSOPEN_MODE) == 2)
    {
        NaviSetIndex(pf,1);
    }
    else if((VarAdrToInt(FL_CLSOPEN_MODE) == 3) || (VarAdrToInt(FL_CLSOPEN_MODE) == 4))
    {
        NaviSetIndex(pf,2);
    }
    else
    {
        NaviSetIndex(pf,0);
    }
}

void G10IOInputGuideSwitch()//IO输入
{
    PPAGE_FRAM pf;
    pf = PanelFindPage(PAGE_IO_INPUT);
    if(IV5200 || IV5300)
    {
        NaviSetIndex(pf,1);
    }
    else
    {
        NaviSetIndex(pf,0);
    }
}

void G10SeqVlvGuideSwitch()// g10顺序阀切换 //20241210 chj 28组吹气
{
    PPAGE_FRAM pf;
    pf = PanelFindPage(PAGE_OTHER_ADJUST);
    if(VarAdrToUI16(SYSTEM_MACH_CODE43)&0x040)
    {
        if(GetSubPartNum(AIRBLOW_ID) > 6)
        {
            if(VarAdrToUI16(d_machine1_MOLDSET_FL_AIR_EXTEND28))
            {
                NaviSetIndex(pf,2 + 3);
            }
            else
            {
                NaviSetIndex(pf,2);
            }

        }
        else
        {
            if(VarAdrToUI16(d_machine1_MOLDSET_FL_AIR_EXTEND28))
            {
                NaviSetIndex(pf,1 + 3);
            }
            else
            {
                NaviSetIndex(pf,1);
            }
        }
    }
    else
    {
        if(VarAdrToUI16(d_machine1_MOLDSET_FL_AIR_EXTEND28))
        {
            NaviSetIndex(pf,0 + 3);
        }
        else
        {
            NaviSetIndex(pf,0);
        }
    }
}

void G10ParaRecordGuideSwitch()// g10参数记录切换
{
    PPAGE_FRAM pf;
    pf = PanelFindPage(PAGE_PARA_RECORD);
    if(GetCurrentUserId() >= USER_SUPERVISOR)//20230518 chj
    {
        NaviSetIndex(pf,2);
    }
    else if(GetCurrentUserId() == USER_MANUFACTURER)
    {
        NaviSetIndex(pf,1);
    }
    else
    {
        NaviSetIndex(pf,0);
    }
}

void G10MTGuideSwitch() //模温
{
    PPAGE_FRAM pf;
    pf = PanelFindPage(PAGE_MOLD_TEMP);

    switch (VarAdrToInt(MT_BOARD_COUNT)) {
    case 1:
        NaviSetIndex(pf,0);
        break;
    case 2:
        NaviSetIndex(pf,1);
        break;
    case 3:
        NaviSetIndex(pf,2);
        break;
    case 4:
        NaviSetIndex(pf,3);
        break;
    case 5:
        NaviSetIndex(pf,4);
        break;
    case 6:
        NaviSetIndex(pf,5);
        break;
    case 7:
        NaviSetIndex(pf,6);
        break;
    default:
        break;
    }
}

void G10MachSetGuideSwitch()//机器结构参数
{
    PPAGE_FRAM pf;
    pf = PanelFindPage(PAGE_MACH_SET);
    if(VarAdrToInt(SYS_FL_MACH_CODE50)&0x4000 && (GetCurrentUserId() >= USER_MANUFACTURER))
    {
        NaviSetIndex(pf,1);
    }
    else
    {
        NaviSetIndex(pf,0);
    }
}

void G10SpecFunc1GuideSwitch() //特殊功能
{
    PPAGE_FRAM pf;
    pf = PanelFindPage(PAGE_SPEC_FUNC1);
    if(GetCurrentUserId() >= USER_MANUFACTURER)
    {
        NaviSetIndex(pf,2);
    }
    else if(GetCurrentUserId() >= USER_DEALER)
    {
        NaviSetIndex(pf,1);
    }
    else
    {
        NaviSetIndex(pf,0);
    }
}

void G10ProdManageGuideSwitch() //生产管理
{
    PPAGE_FRAM pf;
    pf = PanelFindPage(PAGE_PRODMANAGE);

    if(VarAdrToInt(d_machine1_MOLDSET_SRV1))
    {
        NaviSetIndex(pf,1);
    }
    else
    {
        NaviSetIndex(pf,0);
    }
}
/************************************************************************/
/* 润滑参数按钮引导切换  //20211206 dyl                                         */
/************************************************************************/
void G10LubGuideSwtich()
{
    PPAGE_FRAM pf;
    pf = PanelFindPage(PAGE_LUBR);
    if(GetCurrentUserId() >= USER_MANUFACTURER)//20241023 chj 润滑参数权限降低
    {
        NaviSetIndex(pf,1);
    }
    else
    {
        NaviSetIndex(pf,0);
    }
}
/**************G10新增跳转表***结束***************/

/**************P3200新增跳转表***开始***************/
void P3200ClampGuideSwitch()//开关模
{
    if(PushtableType())
    {
        if(VarAdrToInt(CLAMP_MOLD_FL_CLSE_SMOOTH)) //关模平稳使用
        {
            NaviSetIndex(PanelCurPage(),2);
        }
        else
        {
            NaviSetIndex(PanelCurPage(),0);
        }
    }
    else
    {
        if(VarAdrToInt(CLAMP_MOLD_FL_CLSE_SMOOTH))
        {
            NaviSetIndex(PanelCurPage(),3);
        }
        else
        {
            NaviSetIndex(PanelCurPage(),1);
        }
    }
}

void P3200EjectGuideSwtich() //托模
{
    int seg = 0;
    if((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x8B00)
    {
        seg = 0;
    }
    else
    {
        seg = 8;
    }
    if(VarAdrToInt(FL_CLSOPEN_MODE) != 2)
    {
        if (VarAdrToUI16(SYSPAR_BLOW14_FUNC)) //使用7组吹气功能
        {
            if(VarAdrToUI16(EJECT1_MOLD_FL_EJEF_SMOOTH)) //托进平稳使用
            {
                NaviSetIndex(PanelCurPage(),3+seg);
            }
            else
            {
                NaviSetIndex(PanelCurPage(),1+seg);
            }
        }
        else
        {
            if(VarAdrToUI16(EJECT1_MOLD_FL_EJEF_SMOOTH))
            {
                NaviSetIndex(PanelCurPage(),2+seg);
            }
            else
            {
                NaviSetIndex(PanelCurPage(),0+seg);
            }
        }

    }
    else
    {
        if (VarAdrToUI16(SYSPAR_BLOW14_FUNC)) //使用7组吹气功能
        {
            if(VarAdrToUI16(EJECT1_MOLD_FL_EJEF_SMOOTH)) //托进平稳使用
            {
                NaviSetIndex(PanelCurPage(),7+seg);
            }
            else
            {
                NaviSetIndex(PanelCurPage(),5+seg);
            }
        }
        else
        {
            if(VarAdrToUI16(EJECT1_MOLD_FL_EJEF_SMOOTH))
            {
                NaviSetIndex(PanelCurPage(),6+seg);
            }
            else
            {
                NaviSetIndex(PanelCurPage(),4+seg);
            }
        }

    }
}

void P3200EjectAdjGuideSwtich() //托模
{
    int seg = 0;
    if((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x8B00)
    {
        seg = 0;
    }
    else
    {
        seg = 4;
    }
    if (VarAdrToUI16(SYSPAR_BLOW14_FUNC)) //使用7组吹气功能
    {
        if(VarAdrToUI16(EJECT1_MOLD_FL_EJEF_SMOOTH)) //托进平稳使用
        {
            NaviSetIndex(PanelCurPage(),3+seg);
        }
        else
        {
            NaviSetIndex(PanelCurPage(),1+seg);
        }
    }
    else
    {
        if(VarAdrToUI16(EJECT1_MOLD_FL_EJEF_SMOOTH))
        {
            NaviSetIndex(PanelCurPage(),2+seg);
        }
        else
        {
            NaviSetIndex(PanelCurPage(),0+seg);
        }
    }
}
void P3200ChargeGuideSwtich() //储料
{
    if (VarAdrToUI16(INJECT_MOLD_FL_SUCK_SOOMTH)) //射退平稳使用
    {
        NaviSetIndex(PanelCurPage(),1);
    }
    else
    {
        NaviSetIndex(PanelCurPage(),0);
    }
}

void P3200ADAdjustGuideSwtich() //电子尺调整
{
    if(VarAdrToInt(SYS_FL_MACH_CODE1) & 0x04)
    {
        NaviSetIndex(PanelCurPage(),2);
    }
    else if((VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020)&&((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x04) == 0))
    {
        NaviSetIndex(PanelCurPage(),1);
    }
    else if((VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020)&&(VarAdrToInt(SYS_FL_MACH_CODE0) & 0x04))
    {
        NaviSetIndex(PanelCurPage(),2);
    }
    else
    {
        NaviSetIndex(PanelCurPage(),0);
    }
}

void P3200SysParaGuideSwtich() //系统参数
{
    //G特殊处理
    if(1/*VarAdrToInt(SYS_FL_MACH_CODE3) & 0x0400*/)
    {
        if(VarAdrToInt(SYS_FL_TEMPER) & 0x2000)
        {
            NaviSetIndex(PanelCurPage(),3);
        }
        else
        {
            if(VarAdrToInt(SYSPARA_BACK) == 1)
            {
                NaviSetIndex(PanelCurPage(),5);
            }
            else
            {
                NaviSetIndex(PanelCurPage(),1);
            }
        }
    }
    else
    {
        if(VarAdrToInt(SYS_FL_TEMPER) & 0x2000)
        {
            if(VarAdrToInt(SYSPARA_BACK) == 1)
            {
                NaviSetIndex(PanelCurPage(),6);
            }
            else
            {
                NaviSetIndex(PanelCurPage(),2);
            }
        }
        else
        {
            if(VarAdrToInt(SYSPARA_BACK) == 1)
            {
                NaviSetIndex(PanelCurPage(),4);
            }
            else
            {
                NaviSetIndex(PanelCurPage(),0);
            }
        }
    }
}

void P3200ProgUpdateGuideSwitch() //程序升级
{
    if(VarAdrToInt(SYS_FL_CTRLMODEL) == 0x5200)
    {
        NaviSetIndex(PanelCurPage(),1);
    }
    else
    {
        NaviSetIndex(PanelCurPage(),0);
    }
}

void P3200MainGuideSwitch()
{
    if(VarAdrToInt(FL_CLSOPEN_MODE) == 2)
    {
        NaviSetIndex(PanelCurPage(),1);
    }
    else if((VarAdrToInt(FL_CLSOPEN_MODE) == 3) || (VarAdrToInt(FL_CLSOPEN_MODE) == 4))
    {
        NaviSetIndex(PanelCurPage(),2);
    }
    else
    {
        NaviSetIndex(PanelCurPage(),0);
    }
}

void P3200TempxGuideSwitch()
{
    if(VarAdrToInt(MT_BOARD_COUNT) != 0)
    {
        NaviSetIndex(PanelCurPage(), 1);
    }
    else
    {
        NaviSetIndex(PanelCurPage(), 0);
    }
}

/**************P3200新增跳转表***结束***************/
