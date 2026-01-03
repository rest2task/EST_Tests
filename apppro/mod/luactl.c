#include <string.h>
#include "pagefram.h"
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
#include "oprintf.h"
#include "typedef.h"
#include "luactl.h"
#include "lwgt.h"
#include "stepcurve.h"
#include "keymod.h"
#include "coreprot.h"
#include "buttonwgt.h"//20211224 dyl
#include "key.h"
#include "password.h"
#ifdef CLUA_WGT
/************************************************************************/
/* 设置控件属性     
入栈参数:  1.控件名（字符串类型） /  索引（整形） //2018.4.12 添加控件索引查询
			2.属性名
			3.值
返回入栈:	
			1.原值 /nil
如果设置成功，返回TRUE，如果失败则返回nil
*/
/************************************************************************/
static int WGT_SetAttrValue (lua_State *L) 
{
	int t = lua_type(L, 1);
	const char *attrname = luaL_checkstring(L, 2);

	if(attrname == NULL)
		goto err;

	{
		PWGT pwgt = NULL;
		if(t == LUA_TNUMBER)
		{
			int index = luaL_checkint(L, 1);
			pwgt = PageFramCurWgtByIndex(index);
		}
		else if(t == LUA_TSTRING)
		{
			const char *wgtname = luaL_checkstring(L, 1);
			pwgt = PageFramCurWgtByName(wgtname);
		}
		if(pwgt == NULL)
			goto err;
		t = lua_type(L, 3);
		switch (t)
		{
		case LUA_TBOOLEAN:
			{
				int value = lua_toboolean(L,3);
				if(!OBJSetValueByAttrName((POBJ)pwgt,attrname,(int)value,0,NULL,ATTR_UINT))
					goto err;
			}
			break;
		case LUA_TNUMBER:
			{
				lua_Number	value = luaL_checknumber(L,3);

				if(!OBJSetValueByAttrName((POBJ)pwgt,attrname,(int)value,0,NULL,ATTR_UINT))
					goto err;
			}
			break;

		case LUA_TSTRING:
			{
				char *value_str =(char *) luaL_checkstring(L, 3);
				if(!OBJSetValueByAttrName((POBJ)pwgt,attrname,0,0,value_str,ATTR_STR))
					goto err;
			}
			
			break;

		default:
			OWARN("number or string expected");
			goto err;
			break;
		}

	}
	
	lua_pushboolean(L,1);
	return 1;

err:
	lua_pushnil(L);
	return 1;
}

/************************************************************************/
/* 获取控件属性的值   
入栈参数:  1.控件名 （字符串类型） /  索引（整形） //2018.4.12 添加控件索引查询
			2.属性名
返回入栈:	
			1.当前值/nil
如果成功，则返回 当前值，如果失败则返回nil
*/
/************************************************************************/
static int WGT_GetAttrValue (lua_State *L) 
{
	int t = lua_type(L, 1);
	const char *attrname = luaL_checkstring(L, 2);
	PWGT pwgt = NULL;

	if(attrname == NULL)
	{
		goto err;
	}

	if(t == LUA_TNUMBER)
	{
		int index = luaL_checkint(L, 1);
		pwgt = PageFramCurWgtByIndex(index);
	}
	else if(t == LUA_TSTRING)
	{
		const char *wgtname = luaL_checkstring(L, 1);
		pwgt = PageFramCurWgtByName(wgtname);
	}

	if(pwgt == NULL)
		goto err;
	
	{
		int value_int;
		float value_d;
		char * value_str = NULL;
		int type;

                if(!OBJGetValueByAttrName((POBJ)pwgt,attrname,&value_int,&value_d,&value_str,&type))
			goto err;
	
		switch (type)
		{
		case ATTR_BOOL:
			lua_pushboolean(L,value_int);	break;
		case ATTR_INT:
		case ATTR_UINT:
			lua_pushinteger(L,value_int);	break;
		case ATTR_STR:
			lua_pushstring(L,value_str);	break;
		default:
			goto err;
			break;
		}
	}
	
	return 1;
err:
	lua_pushnil(L);
	return 1;
}

/************************************************************************/
/* 设置焦点
入栈参数:  
1.控件名 （字符串类型） /  索引（整形） //2018.4.12 添加控件索引查询
*/
/************************************************************************/
static int WGT_SetFocus (lua_State *L) 
{
	int t = lua_type(L, 1);
	PWGT pwgt = NULL;

	if(t == LUA_TNUMBER)
	{
		int index = luaL_checkint(L, 1);
		pwgt = PageFramCurWgtByIndex(index);
	}
	else if(t == LUA_TSTRING)
	{
		const char *wgtname = luaL_checkstring(L, 1);
		pwgt = PageFramCurWgtByName(wgtname);
	}

	if(pwgt == NULL)
	{
		return 0;
	}

	PageWgtSetFocus(pwgt);
	return 0;
}
/************************************************************************/
/* 
*/
/************************************************************************/
#include "editwgt.h"
#include "../mod/pushtab.h"
//#include "pushtab.h"
static int WGT_SetPushTable (lua_State *L) 
{
	const char *page_name = luaL_checkstring(L, 1);
	const char *wgtname = luaL_checkstring(L, 2);
	
	PWGT pwgt = NULL;

	if(wgtname == NULL || page_name == NULL)
	{
		return 0;
	}

	pwgt =PageGetWgtByName(page_name,wgtname);
	EditSetShowMode(pwgt,PUSHTAB,SetClampPosPush);
	return 0;
}

/************************************************************************/
/* 多页控件隐藏/变灰子页 + 选择子页
*/
/************************************************************************/
#include "tabpagewgt.h"
static int WGT_TabPageSetSubVisble (lua_State *L)
{
    const char *wgtname = luaL_checkstring(L, 1);
    int index = luaL_checkint(L, 2);
    BOOL enable = lua_toboolean(L, 3);

    PWGT pwgt = NULL;

    if(wgtname == NULL)
    {
        return 0;
    }

    pwgt = PageFramCurWgtByName(wgtname);
    TabPageSetSubVisble(pwgt,index,enable);
    return 0;
}

#ifdef USE_QT
static int WGT_TabPageSelPage (lua_State *L)
{
    const char *wgtname = luaL_checkstring(L, 1);
    int index = luaL_checkint(L, 2);

    PWGT pwgt = NULL;

    if(wgtname == NULL)
    {
        return 0;
    }

    pwgt = PageFramCurWgtByName(wgtname);
    TabPageSelPage(pwgt,index);
    return 0;
}

static int WGT_TabPageGetCurPage (lua_State *L)
{
    const char *wgtname = luaL_checkstring(L, 1);
    UI16 index = 0;

    PWGT pwgt = NULL;

    if(wgtname == NULL)
    {
        return 0;
    }

    pwgt = PageFramCurWgtByName(wgtname);
    index = TabPageGetCurSel(pwgt);
    return index;
}
/**
* @brief     :tab键画面切换 画面show的时候调用
* @param     :tab控件名称  画面对应的F键
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191202
*/
static int WGT_FkeyTab (lua_State *L)
{
    const char *wgtname = luaL_checkstring(L, 1);
    int fkey = luaL_checkint(L, 2);

    PWGT pwgt = NULL;

    if(wgtname == NULL)
    {
        return 0;
    }

    pwgt = PageFramCurWgtByName(wgtname);
    FkeyTab(pwgt, fkey);
    return 0;
}
#endif
/************************************************************************/
/* 导航切换
*/
/************************************************************************/
extern void PushGuideSwtich();
static int WGT_PushGuideSwtich (lua_State *L) 
{
	PushGuideSwtich();
	return 0;
}

extern void InjCurveGuideSwtich();
static int WGT_InjCurveGuideSwtich (lua_State *L) 
{
	InjCurveGuideSwtich();
	return 0;
}

extern void BlowGuideSwtich();
static int WGT_BlowGuideSwtich (lua_State *L) 
{
	BlowGuideSwtich();
	return 0;
}

extern void TempGuideSwitch();
static int WGT_TempGuideSwitch (lua_State *L) 
{
	TempGuideSwitch();
	return 0;
}

extern void CoreFuncGuideSwtich();
static int WGT_CoreFuncGuideSwtich (lua_State *L) 
{
	CoreFuncGuideSwtich();
	return 0;
}
//20190307 hz
extern void MainGuideSwitch();
static int WGT_MainGuideSwtich (lua_State *L)
{
	MainGuideSwitch();
	return 0;
}

extern void TempXGuideSwitch();
static int WGT_TempXGuideSwtich (lua_State *L)
{
    TempXGuideSwitch();
    return 0;
}
//  20190326 hz
extern void OtherGuideSwitch();
static int WGT_OtherGuideSwtich (lua_State *L)
{
	OtherGuideSwitch();
	return 0;
}
extern void FastGuideSwitch();
static int WGT_FastGuideSwtich (lua_State *L)
{
	FastGuideSwitch();
	return 0;
}
// 20190424 CHZ
extern void RotateGuideSwitch();
static int WGT_RotateGuideSwitch (lua_State *L)
{
	RotateGuideSwitch();
	return 0;
}
// 20190424 CHZ
extern void VEjectGuideSwitch();
static int WGT_VEjectGuideSwitch(lua_State *L)
{
	VEjectGuideSwitch();
	return 0;
}
// 20190424 CHZ
extern void VMainGuideSwitch();
static int WGT_VMainGuideSwitch(lua_State *L)
{
	VMainGuideSwitch();
	return 0;
}
// 20190424 CHZ
extern void VInjCurveGuideSwitch();
static int WGT_VInjCurveGuideSwitch(lua_State *L)
{
	VInjCurveGuideSwitch();
	return 0;
}
//20190813.cyx
extern void InitPushTable();
static int WGT_InitPushTable(lua_State *L)
{
    InitPushTable();
    return 0;
}
extern void G10ClampGuideSwitch();
static int WGT_G10ClampGuideSwitch(lua_State *L)
{
    G10ClampGuideSwitch();
    return 0;
}
extern void G10MainGuideSwitch();
static int WGT_G10MainGuideSwitch(lua_State *L)
{
    G10MainGuideSwitch();
    return 0;
}
//20200306
extern void G10SeqVlvGuideSwitch();
static int WGT_G10SeqVlvGuideSwitch(lua_State *L)
{
    G10SeqVlvGuideSwitch();
    return 0;
}

extern void G10MachSetGuideSwitch();
static int WGT_G10MachSetGuideSwitch(lua_State *L)
{
    G10MachSetGuideSwitch();
    return 0;
}

extern void G10ProdManageGuideSwitch();
static int WGT_G10ProdManageGuideSwitch(lua_State *L)
{
    G10ProdManageGuideSwitch();
    return 0;
}

extern void P3200ClampGuideSwitch();
static int WGT_P3200ClampGuideSwitch (lua_State *L)
{
    P3200ClampGuideSwitch();
    return 0;
}
extern void P3200EjectGuideSwtich();
static int WGT_P3200EjectGuideSwtich (lua_State *L)
{
    P3200EjectGuideSwtich();
    return 0;
}
extern void P3200EjectAdjGuideSwtich();
static int WGT_P3200EjectAdjGuideSwtich (lua_State *L)
{
    P3200EjectAdjGuideSwtich();
    return 0;
}
extern void P3200ChargeGuideSwtich();
static int WGT_P3200ChargeGuideSwtich (lua_State *L)
{
    P3200ChargeGuideSwtich();
    return 0;
}
extern void P3200ADAdjustGuideSwtich();
static int WGT_P3200ADAdjustGuideSwtich (lua_State *L)
{
    P3200ADAdjustGuideSwtich();
    return 0;
}
extern void P3200SysParaGuideSwtich();
static int WGT_P3200SysParaGuideSwtich (lua_State *L)
{
    P3200SysParaGuideSwtich();
    return 0;
}
extern void G10LubGuideSwtich();//20211206 dyl 润滑参数按钮引导切换
static int WGT_G10LubGuideSwtich(lua_State *L)
{
    G10LubGuideSwtich();
    return 0;
}
/************************************************************************/
/* 控件接口函数
*/
/************************************************************************/
#include "dropwgt.h"
static int WGT_DropSetItems (lua_State *L) 
{
	const char *wgtname = luaL_checkstring(L, 1);
	const char *text = luaL_checkstring(L, 2);

	PWGT pwgt =NULL;

	if(wgtname == NULL || text == NULL)
	{
		return 0;
	}

	pwgt =PageFramCurWgtByName(wgtname);

	if(pwgt !=NULL)
	{
		DropSetItems(pwgt,text);
	}

	return 0;
}

/************************************************************************/
/* 
*/
/************************************************************************/
static int WGT_SetAttrAdrByName (lua_State *L) 
{
	const char *wgtname = luaL_checkstring(L, 1);
	const char *attr_name = luaL_checkstring(L, 2);
	int addr = (int)luaL_checknumber(L,3);
	PWGT pwgt = NULL;

	if(wgtname == NULL || attr_name == NULL)
	{
		return 0;
	}

	pwgt =PageFramCurWgtByName(wgtname);
	WGTSetAttrByName(pwgt,attr_name,addr);
	return 0;
}

static int WGT_SetEnable (lua_State *L)
{
    const char *wgtname = luaL_checkstring(L, 1);
    int value = (int)luaL_checknumber(L,2);
    PWGT pwgt = NULL;

    if(wgtname == NULL)
    {
        return 0;
    }

    pwgt =PageFramCurWgtByName(wgtname);
    WGTSetEnable(pwgt, value);
    return 0;
}
static int WGT_SetVisible (lua_State *L)
{
    const char *wgtname = luaL_checkstring(L, 1);
    int value = (int)luaL_checknumber(L,2);
    PWGT pwgt = NULL;

    if(wgtname == NULL)
    {
        return 0;
    }

    pwgt =PageFramCurWgtByName(wgtname);
    WGTSetVisible(pwgt, value);
    return 0;
}
/************************************************************************/
/* 通过控件名称获得该索引
如果失败返回0
*/
/************************************************************************/
static int WGT_GetOffsetByName (lua_State *L) 
{
	const char *wgtname = luaL_checkstring(L, 1);
	PWGT pwgt = NULL;

	if(wgtname == NULL)
	{
		lua_pushnumber(L,0);
		return 1;
	}

	pwgt =PageFramCurWgtByName(wgtname);
	if(pwgt != NULL)
		lua_pushnumber(L,RU32(pwgt->g_offset));
	else
		lua_pushnumber(L,0);

	return 1;
}
/************************************************************************/
/*阶梯曲线重绘函数 参数1.控件名称 20190621 hz
*/
/************************************************************************/
static int WGT_StepCurveRepaint(lua_State *L)
{
#ifdef IMX6
    PWGT pwgt = NULL;
    const char *wgtname = luaL_checkstring(L, 1);
    pwgt = PageFramCurWgtByName(wgtname);
    StepCureveRePaint(pwgt);
    return 0;
#endif
    return 0;
}

/************************************************************************/
/*状态按钮 选择item 20191223.cyx
*/
/************************************************************************/
#ifdef USE_QT
#include "statebuttonwgt.h"
static int WGT_StateButtonSetIndex(lua_State *L)
{
    PWGT pwgt = NULL;
    const char *wgtname = luaL_checkstring(L, 1);
    const char *attr_name = luaL_checkstring(L, 2);
    int index = luaL_checkint(L, 2);

    if(wgtname == NULL)
    {
        return 0;
    }

    pwgt = PageFramCurWgtByName(wgtname);
    StateButtonSetIndex(pwgt,index);
    return 0;
}
#endif

static int WGT_ButtonSetText(lua_State *L) //20211224 dyl 按钮文字显示
{
    PWGT pwgt = NULL;
    const char *wgtname = luaL_checkstring(L, 1);
    const char *attr_name = luaL_checkstring(L, 2);

    if(wgtname == NULL || attr_name == NULL)
    {
        return 0;
    }

    pwgt =PageFramCurWgtByName(wgtname);
    ButtonSetText(pwgt,attr_name);
    return 0;
}

static int WGT_DropSetIndex(lua_State *L)//20220923 dyl 下拉框选择
{
    PWGT pwgt = NULL;
    const char *wgtname = luaL_checkstring(L, 1);
    const char *attr_name = luaL_checkstring(L, 2);
    int index = luaL_checkint(L, 2);

    if(wgtname == NULL)
    {
        return 0;
    }

    pwgt = PageFramCurWgtByName(wgtname);
    DropSetCurIndex(pwgt,index);
    return 0;
}
#endif	//END WGT

#ifdef CLUA_DB
/************************************************************************/
/* 获取地址对应的值（整数）                                     */
/************************************************************************/
static int DB_GetAdrInt (lua_State *L) 
{
	lua_Number	adr = luaL_checknumber(L,1);
	lua_pushnumber(L,VarAdrToInt((UI32)adr));
	return 1;
}

/************************************************************************/
/* 设置地址对应的值（整数）
返回 成功返回设定的值 失败返回nil
*/
/************************************************************************/
static int DB_SetAdrInt (lua_State *L) 
{
	UI32	value ;
	UI32	adr = (UI32)luaL_checknumber(L,1);
	int param2_type = lua_type(L,2);
	
	if(LUA_TNUMBER == param2_type)
		value= (UI32)lua_tonumber(L,2);//luaL_checknumber(L,2)
	else if(LUA_TBOOLEAN == param2_type)
	{	
		if(lua_toboolean(L,2))
			value = 1;
		else
			value = 0;
	}
	if(VarAdrSetInt(adr,value) == value )
		lua_pushnumber(L, value);
	else
		lua_pushnil(L);
	return 1;
}

/************************************************************************/
/* 获取地址对应的值（字符串）                                   */
/************************************************************************/
static int DB_GetAdrStr (lua_State *L) 
{
	lua_Number	adr = luaL_checknumber(L,1);
	lua_pushstring(L,VarAdrToStr((UI32)adr));
	return 1;
}

/************************************************************************/
/* 设置地址对应的值（字符串） 成功返回 true 失败返回 nil              */
/************************************************************************/
static int DB_SetAdrStr (lua_State *L) 
{
	UI32	adr = (UI32)luaL_checknumber(L,1);
	const char * value_str = luaL_checkstring(L, 2);
	if(value_str == NULL)
	{
		lua_pushnil(L);
		return 1;
	}

	if(VarAdrSetStr(adr,value_str) == NULL)
	{
		lua_pushnil(L);
		return 1;
	}

	lua_pushboolean(L,TRUE);
	return 1;
}

/************************************************************************/
/*保存地址对应的值 成功返回 true 失败返回 false              */
/************************************************************************/
static int DB_SaveAdrInt(lua_State *L)
{
	UI32	adr = (UI32)luaL_checknumber(L,1);

	if(!VarAdrSaveInt(adr))
	{
		lua_pushnil(L);
		return 1;
	}

	lua_pushboolean(L,TRUE);
	return 1;
}

/************************************************************************/
/* 发送地址对应的值到下位机 成功返回 true 失败返回 nil              */
/************************************************************************/
static int DB_SendAdrInt(lua_State *L)
{
	UI32	adr = (UI32)luaL_checknumber(L,1);

	if(!VarSendByAdr(adr))
	{
		lua_pushnil(L);
		return 1;
	}

	lua_pushboolean(L,1);
	return 1;
}

/************************************************************************/
/* 
设置，保存，并下发对应地址的值
成功返回true，失败返回nil
*/
/************************************************************************/
static int DB_CtlAdrInt(lua_State *L)
{
	UI32	adr = (UI32)luaL_checknumber(L,1);
	UI32	value = (UI32)lua_tonumber(L,2);//luaL_checknumber

	if(VarAdrSetInt(adr,value) != value )
		goto err;
	if(!VarAdrSaveInt(adr))
		goto err;
	if(!VarSendByAdr(adr))
		goto err;

	lua_pushnumber(L, value);
	return 1;
err:
	lua_pushnil(L);
	return 1;
}

/************************************************************************/
/* 
请求机器配置
成功返回true，失败返回nil
*/
/************************************************************************/
static int DB_RequestCfgData(lua_State *L)
{
	extern void RequestCfgData();
	RequestCfgData();
	return 0;
}

/************************************************************************/
/* 
获取页面保护的状态
*/
/************************************************************************/
static int DB_IsViewProtect(lua_State *L)
{
	extern UI16 IsViewProtect();
	lua_pushboolean(L,IsViewProtect());
	return 1;
}

/************************************************************************/
/* 
获取参数页面保护的状态
*/
/************************************************************************/
static int DB_IsParamViewHide(lua_State *L)
{
	extern UI16 IsParamViewHide();
	lua_pushboolean(L,IsParamViewHide());
	return 1;
}

/************************************************************************/
/* 
获取密码提示框显示的状态
*/
/************************************************************************/
static int DB_IsPwdPrompt(lua_State *L)
{
	extern UI16 IsPwdPrompt();
	lua_pushboolean(L,IsPwdPrompt());
	return 1;
}

/************************************************************************/
/*
编辑框控件设置数字
*/
/************************************************************************/
static int DB_EditSetNum(lua_State *L)
{
    const char *wgtname = luaL_checkstring(L, 1);
    int int_value = luaL_checkint(L, 2);
    int dot_num = luaL_checkint(L, 3);

    PWGT pwgt = NULL;

    if(wgtname == NULL)
    {
        return 0;
    }

    pwgt = PageFramCurWgtByName(wgtname);
    EditSetNum(pwgt,int_value,dot_num);
    return 0;
}

static int DB_EditSetBKColor(lua_State *L)
{
    const char *wgtname = luaL_checkstring(L, 1);
    lua_Number int_value = luaL_checknumber(L, 2);

    PWGT pwgt = NULL;

    if(wgtname == NULL)
    {
        return 0;
    }

    pwgt = PageFramCurWgtByName(wgtname);
    EditSetBKColor(pwgt,(CGUI_COLOR)int_value);
    return 0;
}
static int DB_EditSetTextColor(lua_State *L)
{
    const char *wgtname = luaL_checkstring(L, 1);
    lua_Number int_value = luaL_checknumber(L, 2);

    PWGT pwgt = NULL;

    if(wgtname == NULL)
    {
        return 0;
    }

    pwgt = PageFramCurWgtByName(wgtname);
    EditSetTextColor(pwgt,(CGUI_COLOR)int_value);
    return 0;
}
static int DB_SETONE(lua_State *L)
{
    int num = luaL_checkint(L, 1);
    int pos = luaL_checkint(L, 2);

    num|=(1<<(pos));
    lua_pushnumber(L, num);
    return 1;
}
static int DB_SETZERO(lua_State *L)
{
    int num = luaL_checkint(L, 1);
    int pos = luaL_checkint(L, 2);

    num&=~(1<<(pos));
    lua_pushnumber(L, num);
    return 1;
}
static int DB_REVERSEBIT(lua_State *L)
{
    int num = luaL_checkint(L, 1);
    int pos = luaL_checkint(L, 2);

    num^=(1<<(pos));
    lua_pushnumber(L, num);
    return 1;
}
static int DB_GETBIT(lua_State *L)
{
    int num = luaL_checkint(L, 1);
    int pos = luaL_checkint(L, 2);
    int ret;

    ret = ((num)>>(pos)&1);
    lua_pushnumber(L, ret);
    return 1;
}
#endif	//END DB

#ifdef CLUA_UI16ARRAYLIB
/************************************************************************/
/* 创建字符数组
*/
/************************************************************************/
static int newarray (lua_State *L) 
{
	int n = luaL_checkint(L, 1);
	size_t nbytes = sizeof(UI16Array) + (n - 1)*sizeof(UI16);
	UI16Array *a = (UI16Array *)lua_newuserdata(L, nbytes);

	luaL_getmetatable(L, UI16_ARRAY_NAME);
	lua_setmetatable(L, -2);

	a->size = n;
	return 1;
}

static UI16Array *checkarray (lua_State *L) {
	void *ud = luaL_checkudata(L, 1, UI16_ARRAY_NAME);
	luaL_argcheck(L, ud != NULL, 1, "`array' expected");
	return (UI16Array *)ud;
}

static UI16 *getelem(lua_State *L)
{
	UI16Array *a = checkarray(L);
	int index = luaL_checkint(L,2);
	luaL_argcheck(L,1<=index && index <= a->size,2,"index out of range");

	return &a->values[index-1];
}

/************************************************************************/
/*设置数组
*/
/************************************************************************/
static int setarray (lua_State *L) 
{
	//char newvalue = (char)luaL_checknumber(L,3);
    *getelem(L) = (UI16)luaL_checknumber(L,3);
	return 0;
}

/************************************************************************/
/* 获取数组元素
*/
/************************************************************************/
static int getarray (lua_State *L)
{
	lua_pushnumber(L,*getelem(L));
	return 1;
}

/************************************************************************/
/* 获取数组大小
*/
/************************************************************************/
static int getsize (lua_State *L) 
{
	UI16Array *a = checkarray(L);
	lua_pushnumber(L, a->size);
	return 1;
}
#endif //END CLUA_UI16ARRAYLIB

#ifdef CLUA_PLUGIN
/************************************************************************/
/* 
获取错误信息
返回值：1.错误信息字符串 2.开模数 3 时间字符串
*/
/************************************************************************/
#include "../mod/alarmrecord.h"
//#include "alarmrecord.h"
extern DB_WARNRECORD AlarmReadRecord(UI32 index);
static int PLUG_ALM_GetRecord(lua_State *L)
{
	UI32	index = (UI32)luaL_checknumber(L,1);
	DB_WARNRECORD item = AlarmReadRecord(index);
	const char *ptrans = NULL;
	UI32 nId;
	char strtmp[100];

	if(item.flag != MARK_USED )
	{
		lua_pushnil(L);
		return 1;
	}
	
	if(pmap_alarm!=NULL)
	{
		ptrans = GetMapTran(pmap_alarm,item.nId);
		if(ptrans!=NULL)
		{
			//alarm num
			nId = item.nId - 0x101;
			snprintf(strtmp,sizeof(strtmp)-1,"#%05X-",ALARM_NUMBER(nId));//(((nId&0xFFF00000)|((nId&0xF00)<<8)|((nId&0x0F)<<12))>>12)
			strcat(strtmp,ptrans);
		}
		lua_pushstring(L,strtmp);
	}
	else
	{
		lua_pushstring(L,"UNKNOWN ERR");
	}
	lua_pushnumber(L,item.wShotCount);
	TimeToStr(strtmp,"yyyy-MM-dd hh:mm:ss",item.datetime);
	lua_pushstring(L,strtmp);
	
	return 3;
}

/************************************************************************/
/* 
获取错误信息
返回值：1.错误信息字符串 2.开模数 3 时间字符串
*/
/************************************************************************/
static int PLUG_ALM_Clear(lua_State *L)
{
	AlarmClearRecord();
	return 0;
}
/************************************************************************/
/* 松模初始化话命令                                                     */
/************************************************************************/
static int PLUG_CMD_ReleaseMdInit(lua_State *L)
{
	extern void ReleaseMdInit();
	ReleaseMdInit();
	return 0 ;
}

/************************************************************************/
/* 向下位机发送code码，并发送保存命令
*/
/************************************************************************/
static int PLUG_CMD_CodeSendSave(lua_State *L)
{
	extern void CodeSendSave();
	CodeSendSave();
	return 0;
}

/************************************************************************/
/* 向下位机发送code码，并发送保存命令
*/
/************************************************************************/
static int PLUG_CMD_ADMeterZero(lua_State *L)
{
	UI32 zero_addr = (UI32)luaL_checkinteger(L,1);
	UI32 abs_addr = (UI32)luaL_checkinteger(L,2);
	extern BOOL AdMeterZero(UI32 zero_addr,UI32 abs_addr);
	AdMeterZero(zero_addr,abs_addr) ;
	return 0;
}

/************************************************************************/
/* 显示信息框
*/
/************************************************************************/
#include "../mod/information.h"
//#include "information.h"
static int PLUG_INFO_ShowMsg(lua_State *L)
{
	UI32 index = (UI32)luaL_checkinteger(L,1);
	ShowMsg(index);
	return 0;
}

/************************************************************************/
/* 显示特殊中子功能说明信息框
*/ //20211221 dyl
/************************************************************************/
#define PAGE_SPCCORE_INGO "SpcCore_Information"  //特殊中子功能说明
static int PLUG_SPCCOREINFO_ShowMsg(lua_State *L)
{
    PanelShowDialogByName(PAGE_SPCCORE_INGO);
    return 0;
}


/************************************************************************/
/* 通过物理键值获取实际键值
*/
/************************************************************************/
#include "../mod/keymod.h"
//#include "keymod.h"
static int PLUG_KEY_KeyToMap(lua_State *L)
{
	UI32 wkey = (UI32)luaL_checkinteger(L,1);
	UI32 rkey =  KeyToMap(wkey);
	lua_pushnumber(L,rkey);
	return 1;
}

/************************************************************************/
/* 通过发送按键获得物理灯
*/
/************************************************************************/
static int PLUG_KEY_KeyToLed(lua_State *L)
{
	UI32 wkey = (UI32)luaL_checkinteger(L,1);
	UI32 led =  KeyToLed(wkey);
	lua_pushnumber(L,led);
	return 1;
}

/************************************************************************/
/* 发送键值和状态
*/
/************************************************************************/
static int PLUG_KEY_SendPanelKey(lua_State *L)
{
	UI32 wkey = (UI32)luaL_checkinteger(L,1);
	UI16 pressed =  (UI16)luaL_checkinteger(L,2);
	SendPanelKey(wkey,pressed);
	return 0;
}

/************************************************************************/
/* 检测按键是否选择
*/
/************************************************************************/
static int PLUG_KEY_ChkKeySel(lua_State *L)
{
	UI16 wkey = (UI16)luaL_checkinteger(L,1);
	lua_pushboolean(L,ChkKeySel(wkey));
	return 1;
}

/************************************************************************/
/* 设置按键选择
*/
/************************************************************************/
static int PLUG_KEY_SetKeySel(lua_State *L)
{
	UI16 wkey = (UI16)luaL_checkinteger(L,1);
	lua_pushnumber(L,SetKeySel(wkey));
	return 1;
}

/************************************************************************/
/* 清除按键选择
*/
/************************************************************************/
static int PLUG_KEY_ClrKeySel(lua_State *L)
{
	UI16 wkey = (UI16)luaL_checkinteger(L,1);
	lua_pushnumber(L,ClrKeySel(wkey));
	return 1;
}
/************************************************************************/
/* 保存所有配置
*/
/************************************************************************/
static int PLUG_MACHCFG_SaveAll(lua_State *L)
{
	extern void SaveAllMachcfg();
	SaveAllMachcfg();
	return 0;
}

/************************************************************************/
/* 读取所有配置
*/
/************************************************************************/
static int PLUG_MACHCFG_LoadAll(lua_State *L)
{
	extern void LoadAllMachcfg();
	LoadAllMachcfg();
	return 0;
}

/************************************************************************/
/* 导出配置到save文件夹下machcfg.dat
*/
/************************************************************************/
static int PLUG_MACHCFG_Export(lua_State *L)
{
	extern void MachCfgExport();
	MachCfgExport();
	return 0;
}

/************************************************************************/
/* 重置机器配置
*/
/************************************************************************/
static int PLUG_MACHCFG_Reset(lua_State *L)
{
	extern BOOL MachCfgReset();
	MachCfgReset();
	return 0;
}

/************************************************************************/
/* 保存模具参数
*/
/************************************************************************/
#include "../mod/moldset.h"
//#include "moldset.h"
static int PLUG_MOLD_Save(lua_State *L)
{
	UI16 index = (UI16)luaL_checkinteger(L,1);
	lua_pushboolean(L,SaveMoldSet(index)) ;
	return 1;
}

/************************************************************************/
/* 读取模具参数并下发
*/
/************************************************************************/
static int PLUG_MOLD_Read(lua_State *L)
{
	UI16 index = (UI16)luaL_checkinteger(L,1);
	lua_pushboolean(L,ReadMoldSet(index)) ;
	return 1;
}

/************************************************************************/
/* 读取当前的模具名称和索引
*/
/************************************************************************/
static int PLUG_MOLD_GetCurMold(lua_State *L)
{
	char name[65];
	int id;
	MoldSet_GetCur(name,&id);
	lua_pushstring(L,name);
	lua_pushnumber(L,id);
	return 2;
}

/************************************************************************/
/* 读取当前的模具名称和索引
*/
/************************************************************************/
#include "../mod/pushtab.h"
//#include "pushtab.h"
static int PLUG_PUSH_TransMeter(lua_State *L)
{
	UI16 wPumpMeter = (UI16)luaL_checkinteger(L,1);
	lua_pushnumber(L,TransMeter(wPumpMeter));

	return 1;
}

/************************************************************************/
/* 读取当前的模具名称和索引
*/
/************************************************************************/
static int PLUG_PUSH_UnTransMeter(lua_State *L)
{
	luaL_checkany(L,2);
	{
		UI16 wClampMeter = (UI16)luaL_checkinteger(L,1);
		BOOL bPoint = lua_toboolean(L,2);
		lua_pushnumber(L,UnTransMeter(wClampMeter,bPoint));
	}
	
	return 1;
}

/************************************************************************/
/* 读取当前推力座类型
*/
/************************************************************************/
static int PLUG_PUSH_Type(lua_State *L)
{
	UI16 ret = PushtableType();
	if(ret == 0)
		lua_pushnil(L);
	else
		lua_pushnumber(L,ret);
	return 1;
}
/************************************************************************/
/* 读取面板配置类型
 * PANEL_H7 = 0,  //横屏7寸
    PANEL_H8 = 1,  //横屏8寸
    PANEL_H10 = 2, //横屏10寸
    PANEL_V10 = 3  //竖屏10寸
*/
/************************************************************************/
#include "../mod/verinfo.h"
static int PLUG_Panel_Type(lua_State *L)
{
    UI16 ret = GetPanelType();
	if(ret == 0)
		lua_pushnil(L);
	else
		lua_pushnumber(L,ret);
	return 1;
}

/************************************************************************/
/* 显示询问框
*/
/************************************************************************/
static int PLUG_Question_Show(lua_State *L)
{
	/*luaL_checkanyfunction()
	lua_pushnumber(L,PushtableType());
	return 1;*/
	return 0;
}

/************************************************************************/
/* can通信
*/
/************************************************************************/
static int PLUG_CAN_Write(lua_State *L)
{
	/*luaL_checkanyfunction()
	lua_pushnumber(L,PushtableType());
	return 1;*/
	return 0;
}

/************************************************************************/
/* can通信任务
*/
/************************************************************************/
#include "canmod.h"
static int PLUG_Send_TaskCan(lua_State *L)
{
	int i;
	luaL_checkany(L,4);
	{
		for( i=0;i<=4;i++)
		{
			if(lua_type(L,i)!=LUA_TNUMBER)
			{
				OWARN("type err");
				return 0;
			}
		}
	}
	{
		UI8 cmd = (UI8)luaL_checknumber(L,1);
		UI8 id = (UI8)luaL_checknumber(L,1);
		UI16 addr = (UI16)luaL_checknumber(L,1);
		UI32 date = (UI32)luaL_checknumber(L,1);
		Send_TaskCan(cmd,id,addr,date);
	}
	
	
	return 0;
}
/************************************************************************/
/* 面板与主机重启
*/
/************************************************************************/
static int PLUG_PROT_UpgradReset(lua_State *L)
{
	extern void PanelMainRestart();
	PanelMainRestart();
	return 0;
}

/************************************************************************/
/* 发送系统控制命令
*/
/************************************************************************/
extern BOOL ProtSysCtrl(UI16 ctrl_cmd, UI16 data_len, UI16* pdat);
static int PLUG_PROT_SysCtrl(lua_State *L)
{
	UI16 ctrl_cmd = (UI16)luaL_checkinteger(L,1);
	UI16 data_len = (UI16)luaL_checkinteger(L,2);
	UI16 *data = NULL;
	int type;
	if(data_len>0)
	{
		int i=0;
		luaL_checkany(L,2+data_len);
		data = OBJMemMalloc(data_len * sizeof(UI16));
		for(i=0;i<data_len;i++)
		{
			type = lua_type(L,3+i);
			if(type != LUA_TNUMBER)
				goto err;
			data[i] = (UI16)luaL_checkinteger(L,3+i);
		}
		ProtSysCtrl(ctrl_cmd,data_len,data);
	}
	else
	{
		ProtSysCtrl(ctrl_cmd,0,data);
	}
	OBJMemFree(data);
	lua_pushboolean(L,1);
	return 1;
err:
	OBJMemFree(data);
	lua_pushboolean(L,0);
	return 1;
}

/************************************************************************/
/* 获取转换表对应的text
*/
/************************************************************************/
#include "../mod/texttbl.h"
//#include "texttbl.h"
static int PLUG_GetTextTran(lua_State *L)
{
	UI32 mapid = (UI32)luaL_checkinteger(L,1);
	lua_pushstring(L,GetTextTran(mapid)) ;
	return 1;
}

/************************************************************************/
/* 发送数据到下位机
参数：1.部件id		2.子id		3.偏移		4.数据（UI16Array类型）
*/
/************************************************************************/
static int PLUG_ProtSendPartSubData(lua_State *L)
{
#ifdef CLUA_UI16ARRAYLIB
	extern BOOL ProtSendPartSubData(UI16 part_id, UI8 sub_id, UI16 offset, UI16 len, UI16* pdat);
	UI16 partid	 = (UI16)luaL_checkinteger(L,1);
	UI8 subid	 = (UI8)luaL_checkinteger(L,2);
	UI16 offset	 = (UI16)luaL_checkinteger(L,3);
	UI16Array *array = (UI16Array*)luaL_checkudata(L, 4, UI16_ARRAY_NAME);
	luaL_argcheck(L, array != NULL, 4, "`array' expected");
	oprintf("array->values=%d\r\n",array->values[0]);
	ProtSendPartSubData(partid,subid,offset,array->size,array->values);
#endif
  return 0;
}

/************************************************************************/
/*获取当前操作状态
*/
/************************************************************************/
#include "../mod/database.h"
//#include "database.h"
static int PLUG_GetOPMode(lua_State *L)
{
	lua_pushnumber(L,VarAdrToUI16(OPMODE_INDEX));
	return 1;
}

/************************************************************************/
/*获取星期数据
*/
/************************************************************************/
extern UI16 GetWeekData();
static int PLUG_GetWeekData(lua_State *L)
{
	lua_pushnumber(L,GetWeekData());
	return 1;
}

/************************************************************************/
/*临时地址
*/
/************************************************************************/
static int PLUG_TMPVAL(lua_State *L)
{
	UI32 value = (UI32)luaL_checkinteger(L,1);
	lua_pushnumber(L,(BASEADR_VAL+value));
	return 1;
}

static int PLUG_TMPSTR(lua_State *L)
{
	UI32 value = (UI32)luaL_checkinteger(L,1);
	lua_pushnumber(L,(BASEADR_STR+value));
	return 1;
}

/************************************************************************/
/*切换导航
*/
/************************************************************************/
extern void NaviSetIndex(void* pframe, int index);
static int PLUG_NaviSetIndex(lua_State *L)
{
	int value = (int)luaL_checkinteger(L,1);
	NaviSetIndex(PanelCurPage(),value);
	return 0;
}

/************************************************************************/
/*获取加热对应配置
传 入 参 数  : 
  1. 温度段数：warm_index
								  //2. 配置类型字符串：cfg_name Hour （加温时）nMinute（加温分）Warm  （开始加温）ClsHour（关加温时）ClsMin  （关加温分）WarmCls   （关闭加温）
  2.配置索引 （按结构体来）
*/
/************************************************************************/
#include "../mod/warm.h"
//#include "warm.h"
static int PLUG_GetWarmCFG(lua_State *L)
{
	int warm_index = (int)luaL_checkinteger(L,1);
	int cfg_index = (int)luaL_checkinteger(L,2);
	UI16 value ;
	//const char *cfg_name = luaL_checkstring(L, 2);
	if(warm_index >= 7 || warm_index<0)
		return 0;
	value = *(&(dbwarmx.dbwarmconfig[warm_index].nHour)+sizeof(UI16)*cfg_index);
	lua_pushnumber(L,value);
	return 1;
}

/************************************************************************/
/*设置加热对应配置
传 入 参 数  : 
  1. 温度段数：warm_index
  2. 配置索引 （按结构体来）
  3. 设置的值
*/
/************************************************************************/
static int PLUG_SetWarmCFG(lua_State *L)
{
	int warm_index = (int)luaL_checkinteger(L,1);
	int cfg_index = (int)luaL_checkinteger(L,2);
	UI16 value = (UI16)luaL_checkinteger(L,3);
	
	if(warm_index >= 7 || warm_index<0)
		return 0;
	memcpy(((&(dbwarmx.dbwarmconfig[warm_index].nHour))+sizeof(UI16)*cfg_index),&value,sizeof(UI16));
	return 0;
}

/************************************************************************/
/* 加热预设保存函数  
1. 保存的偏移位置：dst
2. 温度段数：warm_index
*/
/************************************************************************/
static int PLUG_SaveWarmSet(lua_State *L)
{
	UI32 wpos;
	int warm_index = (int)luaL_checkinteger(L,1);

	if(warm_index >= 7 || warm_index < 0)
		return 0;
	wpos =((UI32)&dbwarmx.dbwarmconfig[warm_index]-(UI32)&dbwarmx);
	SaveWarmSet(wpos, &dbwarmx.dbwarmconfig[warm_index],sizeof(dbwarmx.dbwarmconfig[warm_index]));
	return 0;
}

/************************************************************************/
/*加热预设读取函数 
*/
/************************************************************************/
static int PLUG_LoadWarmSet(lua_State *L)
{
	LoadWarmSet();
	return 0;
}

/************************************************************************/
/*生产管理部分
*/
/************************************************************************/
#include "../mod/prodmanage.h"
//#include "prodmanage.h"
static int PLUG_ProdSetPerMold(lua_State *L)
{
	ProdSetPerMold();
	return 0;
}
static int PLUG_ProdChgMode(lua_State *L)
{
	ProdChgMode();
	return 0;
}
static int PLUG_ProdSetGoalCnt(lua_State *L)
{
	ProdSetGoalCnt();
	return 0;
}
static int PLUG_ProdClrCnt(lua_State *L)
{
	ProdClrCnt();
	return 0;
}
static int PLUG_ProdClrPkg(lua_State *L)
{
	ProdClrPkg();
	return 0;
}
//20211112 dyl 开机总时间、运行总计时统计
static int PLUG_ProdSetRealTimeCount(lua_State *L)
{
    ProdSetRealTimeCount();
    return 0;
}
static int PLUG_ProdClrBootTime(lua_State *L)
{
    ProdClrBootTime();
    return 0;
}
static int PLUG_ProdClrProdTime(lua_State *L)
{
    ProdClrProdTime();
    return 0;
}
/************************************************************************/
/*读取参数记录 
*/
/************************************************************************/
#include "../mod/paramrecord.h"
//#include "paramrecord.h"
static int PLUG_ReadRecord(lua_State *L)
{
	RecordItem item;
	UI32 index = luaL_checkinteger(L,1);
	if(lua_istable(L,2) && ReadRecord(&item,index))
	{
		lua_pushstring(L,"idTitle");
		lua_pushstring(L,item.idTitle);
		lua_settable(L,2);

		lua_pushstring(L,"newvalue");
		lua_pushstring(L,item.newvalue);
		lua_settable(L,2);

		lua_pushstring(L,"oldvalue");
		lua_pushstring(L,item.oldvalue);
		lua_settable(L,2);

		lua_pushstring(L,"time");
		lua_pushstring(L,item.time);
		lua_settable(L,2);

		lua_pushstring(L,"param");
		lua_pushstring(L,item.param);
		lua_settable(L,2);

		lua_pushboolean(L,true);
		return 1;
	}
	lua_pushboolean(L,false);
	return 1;
}
#include "file.h"
static int PLUG_ClearParamRecord(lua_State *L)
{
	FileDelete(RECORD_FILE_PATH);
	ParamRecordInit();
	return 0;
}

//添加参数记录
static int PLUG_SetRecordByManual(lua_State *L)
{
    PPAGE_FRAM pf = PanelCurPage();
    UI32 addr = luaL_checkinteger(L,1);
    UI8 npoint = (UI8)luaL_checkinteger(L,2);
    UI32 newvalue = luaL_checkinteger(L,3);
    UI32 oldvalue = luaL_checkinteger(L,4);
    UI8 base = 0;

    if(pf != NULL)
    {
        SetRecordByManual((char*)PageFramGetName(pf),addr,npoint,newvalue,oldvalue,base);
    }

    return 0;
}

/************************************************************************/
/* 检测当前系统是否是windows系统
*/
/************************************************************************/
static int PLUG_IsWindowsSys(lua_State *L)
{
#ifdef WIN32
	lua_pushboolean( L,true);
#else
	lua_pushboolean(L,false);
#endif
	return 1;
}

/************************************************************************/
/* 设置窗口背景
*/
/************************************************************************/
static int PLUG_PageFramBackground(lua_State *L)
{
	PageFramBackground((void*)luaL_optstring(L, 1, NULL));
	return 0;
}

extern BOOL IsPayChecking();
/************************************************************************/
/* 分期付款检测
*/
/************************************************************************/
static int PLUG_IsPayChecking(lua_State *L)
{
	lua_pushboolean(L,IsPayChecking());
	return 1;
}

extern UI16 ControlPass(UI8 year,UI8 month,UI8 day,UI16 random);
static int PLUG_ControlPass(lua_State *L)
{
	luaL_checkany(L,4);
	{
		UI8 year = (UI8)luaL_checknumber(L,1);
		UI8 month = (UI8)luaL_checknumber(L,2);
		UI8 day = (UI8)luaL_checknumber(L,3);
		UI16 random = (UI16)luaL_checknumber(L,4);
		lua_pushnumber(L,ControlPass(year,month,day,random));
	}
	return 1;
}

static int PLUG_SetDate(lua_State *L)
{
#ifndef WIN32
	luaL_checkany(L,3);
	{
		UI8 year = (UI8)luaL_checknumber(L,1);
		UI8 month = (UI8)luaL_checknumber(L,2);
		UI8 day = (UI8)luaL_checknumber(L,3);

		set_date(year,month,day);

	}
#endif
	return 0;
}
static int PLUG_SetTime(lua_State *L)
{
#ifndef WIN32
	luaL_checkany(L,3);
	{
		UI8 hour = (UI8)luaL_checknumber(L,1);
		UI8 minute = (UI8)luaL_checknumber(L,2);
		UI8 second = (UI8)luaL_checknumber(L,3);

		set_time(hour,minute,second);
	}
#endif
	return 0;
}

/************************************************************************/
/* 显示首页
*/
/************************************************************************/
static int PLUG_ShowMainPage(lua_State *L)
{
	PanelShowPage(PanelMainPage());
	return 0;
}


/************************************************************************/
/* 跳转到对应的画面 20190801 YX 
*/
/************************************************************************/
static int PLUG_PanelShowPageByName (lua_State *L)
{
    const char *wgtname = luaL_checkstring(L, 1);

    if(wgtname == NULL)
    {
        return 0;
    }

    PanelShowPageByName(wgtname);

    return 1;
}


/************************************************************************/
/*解析csv文件，并返回 
成功返回 PHASH_TBL
失败返回nil
*/
/************************************************************************/
static int PLUG_ParseCSVFile(lua_State *L)
{
	luaL_checkany(L,1);
	{
		const char* file = luaL_optstring(L, 1, NULL);
		if(file == NULL)
			lua_pushnil(L);
		else
		{
			PHASH_TBL pmap_tbl = NULL;
			if(ParseCSVFile(file,&pmap_tbl))
				lua_pushinteger(L,(int)pmap_tbl);
			else
				lua_pushnil(L);
		}
	}
	return 1;
}


/************************************************************************/
/*获得对应翻译
入参：参数1：对应phashtable指针
	   参数2：索引
返回：对应翻译或者nil
*/
/************************************************************************/
static int PLUG_GetMapTran(lua_State *L)
{
	luaL_checkany(L,2);
	{
		PHASH_TBL pmap_tbl = (PHASH_TBL)((int)luaL_checknumber(L,1));
		int index = (int)luaL_checknumber(L,2);
		const char * trans = GetMapTran(pmap_tbl,index);
		if(trans)
		{
			lua_pushstring(L,trans);
		}
		else
			lua_pushnil(L);
	}
	return 1;
}

/************************************************************************/
/*获得伺服信息
入参：参数1  伺服号
	   参数2  数据偏移
返回：值或者 nil
*/
/************************************************************************/
#include "../mod/servo.h"
//#include "servo.h"
static int PLUG_GetServoData(lua_State *L)
{
	luaL_checkany(L,2);
	{
		int servoid = (int)luaL_checknumber(L,1);
		int index = (int)luaL_checknumber(L,2);
		if(servoid > 7 || index > ID_1017) //max
			lua_pushnil(L);
		else
			lua_pushnumber(L,dbsvox[servoid].wData[index]);
	}
	return 1;
}

/************************************************************************/
/*获得伺服数据个数
入参：参数1  伺服号
返回：值的个数
*/
/************************************************************************/
static int PLUG_GetServoDataCnt(lua_State *L)
{
	luaL_checkany(L,1);
	{
		int servoid = (int)luaL_checknumber(L,1);
		if(servoid > 7 ) //max
			lua_pushnumber(L,0);
		else
			lua_pushnumber(L,dbsvox[servoid].nCount);
	}
	return 1;
}

/************************************************************************/
/*设置伺服数据个数
入参：参数1  伺服号
		参数2   个数
*/
/************************************************************************/
static int PLUG_SetServoDataCnt(lua_State *L)
{
	luaL_checkany(L,2);
	{
		int servoid = (int)luaL_checknumber(L,1);
		int cnt = (int)luaL_checknumber(L,2);
		if(servoid <= 7 ) //max
			dbsvox[servoid].nCount = cnt;
	}
	return 0;
}

/************************************************************************/
/*获取当前用户等级
*/
/************************************************************************/
#include "../mod/usermod.h"
static int PLUG_GetCurrentUserId(lua_State *L)
{
    lua_pushnumber(L,GetCurrentUserId());
    return 1;
}

/************************************************************************/
/*设置画面锁模式
*/
/************************************************************************/
static int PLUG_SetPageLockMode(lua_State *L)
{
    int index = (int)luaL_checkinteger(L,1);
    SetPageLockMode(index);
    return 0;
}

#include "../mod/lubr.h"
/************************************************************************/
/*保存初始润滑时间
*/
/************************************************************************/
static int PLUG_SaveLubStartTm(lua_State *L)
{
    int index = (int)luaL_checkinteger(L,1);
    SaveLubStartTm(index);
    return 0;
}

/************************************************************************/
/*获取润滑时间
*/
/************************************************************************/
static int PLUG_GetLubHour(lua_State *L)
{
    int index = (int)luaL_checkinteger(L,1);
    int value = GetLubHour(index);
    lua_pushinteger(L,value);
    return 1;
}

/************************************************************************/
/*设置润滑时间
*/
/************************************************************************/
static int PLUG_SetLubHour(lua_State *L)
{
    int index = (int)luaL_checkinteger(L,1);
    int value = (int)luaL_checkinteger(L,2);
    SetLubHour(index, value);
    return 0;
}

/************************************************************************/
/*获取剩余润滑时间
*/
/************************************************************************/
static int PLUG_GetLeftLubHour(lua_State *L)
{
    int index = (int)luaL_checkinteger(L,1);
    int value = GetLeftLubHour(index);
    lua_pushinteger(L,value);
    return 1;
}

/************************************************************************/
/*设置剩余润滑时间
*/
/************************************************************************/
static int PLUG_SetLeftLubHour(lua_State *L)
{
    int index = (int)luaL_checkinteger(L,1);
    int value = (int)luaL_checkinteger(L,2);
    SetLeftLubHour(index, value);
    return 0;
}

static int PLUG_PageFrameYSort(lua_State *L)
{
    PPAGE_FRAM pf = PanelCurPage();

    if(pf != NULL)
    {
        PageFramYSort(pf);
    }

    return 0;
}

//20210607.cyx xo重新排序 + //20211229 dyl
static int PLUG_PageFrameXSort(lua_State *L)
{
    PPAGE_FRAM pf = PanelCurPage();

    if(pf != NULL)
    {
        PageFramXSort(pf);
    }

    return 0;
}

//20211229 dyl 子窗体光标重新排序
static int PLUG_PageSubXExChange(lua_State *L)
{
    PPAGE_FRAM pf = PanelCurPage();
    const char *wgtname = luaL_checkstring(L, 1);
    int srcxo = (int)luaL_checkinteger(L,2);
    int dstxo = (int)luaL_checkinteger(L,3);

    if(wgtname == NULL)
    {
        return 0;
    }

    PageSubXExChange(pf, wgtname,srcxo,dstxo);

    return 0;
}

/************************************************************************/
/*中子保护功能
*/ //20211221 dyl
/************************************************************************/
//中子进退动作位置根据开模终止位置变化
static int PLUG_SyncCoreAdvRetPos(lua_State *L)
{
    UI32 dwAddr = luaL_checkinteger(L,1);
    SyncCoreAdvRetPos(dwAddr);

    return 0;
}
//中子动作点跟随特殊中子功能及联动功能变化
static int PLUG_CorePointByLink(lua_State *L)
{
    CorePointByLink();

    return 0;
}
//20210730 dyl 中子有效区位置
static int PLUG_CoreEffeAreaByPushtab(lua_State *L)
{
    UI32 Pos_dwAddr = luaL_checkinteger(L,1);
    UI32 Effe_dwAddr = luaL_checkinteger(L,2);
    CoreEffeAreaByPushtab(Pos_dwAddr, Effe_dwAddr);

    return 0;
}


/************************************************************************/
/* 获取当前AD电子尺的个数                                     */
/************************************************************************/
static int PLUG_GetADMaxNumber (lua_State *L)//20220718 dyl 当3101/3202等使用6路电子尺时的电子尺通道选择
{
    lua_pushnumber(L,GetADMaxNumber());
    return 1;
}


//20230110 //CSJ 开关模压力上限密码保护计算 //20241017 chj 金鹰特殊厂商密码计算
static int PLUG_CalClampPrsMaxProtectPSW(lua_State *L)
{
    UI32 Resualt_Addr = luaL_checkinteger(L,1);
    Cal_Clamp_Prs_Max_Protect_PSW(Resualt_Addr);
    return 0;
}

#endif //plugin end

/* }================================================================== */
static const luaL_Reg lWGTLib[] = {
#if LUA_OPTIMIZE_MEMORY != 2
#undef MIN_OPT_LEVEL
#define MIN_OPT_LEVEL 0

#ifdef CLUA_WGT
	{"SetAttrValue",   WGT_SetAttrValue},
	{"GetAttrValue",   WGT_GetAttrValue},
    {"SetFocus",		WGT_SetFocus},
	{"SetPushTable",	WGT_SetPushTable},
	{"PushGuideSwtich",	WGT_PushGuideSwtich},
	{"InjCurveGuideSwtich",	WGT_InjCurveGuideSwtich},
	{"WGTSetAttrByName",WGT_SetAttrAdrByName},
    {"WGTSetEnable",WGT_SetEnable},
    {"WGTSetVisible",WGT_SetVisible},
    {"GetOffsetByName",WGT_GetOffsetByName},
    {"StepCurveRepaint", WGT_StepCurveRepaint},
	{"BlowGuideSwtich",	WGT_BlowGuideSwtich},
	{"DropSetItems",	WGT_DropSetItems},
	{"TempGuideSwitch",	WGT_TempGuideSwitch},
	{"CoreFuncGuideSwtich",WGT_CoreFuncGuideSwtich},
	{"MainGuideSwitch",WGT_MainGuideSwtich},
	{"TempXGuideSwitch",WGT_TempXGuideSwtich},
	{"OtherGuideSwitch",WGT_OtherGuideSwtich},
	{"FastGuideSwitch",WGT_FastGuideSwtich},
	{"RotateGuideSwitch",WGT_RotateGuideSwitch},//20190424 CHZ
	{"VEjectGuideSwitch",WGT_VEjectGuideSwitch},
	{"VMainGuideSwitch",WGT_VMainGuideSwitch},
	{"VInjCurveGuideSwitch",WGT_VInjCurveGuideSwitch},
    {"InitPushTable",   WGT_InitPushTable},//20190813.cyx
    {"G10ClampGuideSwitch",WGT_G10ClampGuideSwitch},
    {"G10MainGuideSwitch",WGT_G10MainGuideSwitch},
    {"G10SeqVlvGuideSwitch",WGT_G10SeqVlvGuideSwitch},//20200306
    {"G10MachSetGuideSwitch",WGT_G10MachSetGuideSwitch},//20200727.cyx
    {"G10ProdManageGuideSwitch",WGT_G10ProdManageGuideSwitch},//20201104
    {"G10LubGuideSwtich",WGT_G10LubGuideSwtich},//20211206 dyl 润滑参数按钮引导切换
    {"P3200ClampGuideSwitch",WGT_P3200ClampGuideSwitch},//20200219.cyx
    {"P3200EjectGuideSwtich",WGT_P3200EjectGuideSwtich},//20200220.cyx
    {"P3200EjectAdjGuideSwtich",WGT_P3200EjectAdjGuideSwtich},
    {"P3200ChargeGuideSwtich",WGT_P3200ChargeGuideSwtich},//20200220.cyx
    {"P3200ADAdjustGuideSwtich",WGT_P3200ADAdjustGuideSwtich},//20200220.cyx
    {"P3200SysParaGuideSwtich",WGT_P3200SysParaGuideSwtich},//20200220.cyx
    {"TabPageSetSubVisble",WGT_TabPageSetSubVisble},//20191118.cyx
#ifdef USE_QT
    {"TabPageSelPage",WGT_TabPageSelPage},
    {"TabPageGetCurPage",WGT_TabPageGetCurPage},
    {"FkeyTab",WGT_FkeyTab},
    {"StateButtonSetIndex",WGT_StateButtonSetIndex},
    {"ButtonSetText",WGT_ButtonSetText},//20211224 dyl 按钮文字显示
    {"DropSetCurIndex",WGT_DropSetIndex},//20220923 dyl 下拉框选择
#endif //end USE_QT
#endif
#ifdef CLUA_DB
	{"VarAdrToInt",   DB_GetAdrInt},
	{"VarAdrToUI16",   DB_GetAdrInt},
	{"VarAdrSetInt",   DB_SetAdrInt},
	{"VarAdrToStr",   DB_GetAdrStr},
	{"VarAdrSetStr",   DB_SetAdrStr},
	{"VarAdrSaveInt",   DB_SaveAdrInt},
	{"VarSendByAdr",   DB_SendAdrInt},
	{"CtlAdrInt",   DB_CtlAdrInt},
	{"RequestCfgData",   DB_RequestCfgData},
	{"IsViewProtect",   DB_IsViewProtect},
	{"IsParamViewHide",   DB_IsParamViewHide},
	{"IsPwdPrompt",   DB_IsPwdPrompt},
    {"EditSetNum",   DB_EditSetNum},
    {"EditSetBKColor",   DB_EditSetBKColor},
    {"EditSetTextColor",   DB_EditSetTextColor},
    {"setone", DB_SETONE},
    {"setzero", DB_SETZERO},
    {"reversebit", DB_REVERSEBIT},
    {"getbit", DB_GETBIT},
#endif

#ifdef CLUA_PLUGIN
	{"ALM_GetRecord",   PLUG_ALM_GetRecord},
	{"ALM_Clear",   PLUG_ALM_Clear},
	{"CMD_ReleaseMdInit",PLUG_CMD_ReleaseMdInit},
	{"CMD_CodeSendSave",   PLUG_CMD_CodeSendSave},
	{"ADMeterZero",   PLUG_CMD_ADMeterZero},
	{"INFO_ShowMsg",   PLUG_INFO_ShowMsg},
	{"ShowMsg",   PLUG_INFO_ShowMsg},
    {"ShowSpcCoreMsg",   PLUG_SPCCOREINFO_ShowMsg},//20211221 dyl
	{"KeyToMap",   PLUG_KEY_KeyToMap},
	{"KeyToLed",   PLUG_KEY_KeyToLed},
    {"SendPanelKey",   PLUG_KEY_SendPanelKey},
	{"ChkKeySel",   PLUG_KEY_ChkKeySel},
	{"SetKeySel",   PLUG_KEY_SetKeySel},
	{"ClrKeySel",   PLUG_KEY_ClrKeySel},
	{"MACHCFG_SaveAll",   PLUG_MACHCFG_SaveAll},
	{"MACHCFG_LoadAll",   PLUG_MACHCFG_LoadAll},
	{"MACHCFG_Export",   PLUG_MACHCFG_Export},
	{"MACHCFG_Reset",   PLUG_MACHCFG_Reset},
	{"MOLD_Save",   PLUG_MOLD_Save},
	{"MOLD_Read",   PLUG_MOLD_Read},
	{"MOLD_GetCurMold",   PLUG_MOLD_GetCurMold},
	{"TransMeter",   PLUG_PUSH_TransMeter},
	{"UnTransMeter",   PLUG_PUSH_UnTransMeter},
	{"PUSH_Type",   PLUG_PUSH_Type},
	{"Question_Show",   PLUG_Question_Show},
	{"CAN_Write",   PLUG_CAN_Write},
	{"Send_TaskCan",   PLUG_Send_TaskCan},
	{"PROT_UpgradReset",   PLUG_PROT_UpgradReset},
	{"PROT_SysCtrl",   PLUG_PROT_SysCtrl},
	{"ProtSysCtrl",   PLUG_PROT_SysCtrl},
	{"GetTextTran",   PLUG_GetTextTran},
	{"ProtSendPartSubData",   PLUG_ProtSendPartSubData},
	{"GetOPMode",   PLUG_GetOPMode},
	{"GetWeekData",   PLUG_GetWeekData},
	{"TMPVAL",   PLUG_TMPVAL},
	{"TMPSTR",   PLUG_TMPSTR},
	{"NaviSetIndex",   PLUG_NaviSetIndex},
	{"GetWarmCFG",   PLUG_GetWarmCFG},
	{"SetWarmCFG",   PLUG_SetWarmCFG},
	{"LoadWarmSet",   PLUG_LoadWarmSet},
	{"SaveWarmSet",   PLUG_SaveWarmSet},
	{"ProdSetPerMold",   PLUG_ProdSetPerMold},
	{"ProdChgMode",   PLUG_ProdChgMode},
	{"ProdSetGoalCnt",   PLUG_ProdSetGoalCnt},
	{"ProdClrCnt",   PLUG_ProdClrCnt},
	{"ProdClrPkg",   PLUG_ProdClrPkg},
    {"ProdSetRealTimeCount",   PLUG_ProdSetRealTimeCount},//20211112 dyl 开机总时间、运行总计时统计
    {"ProdClrBootTime",   PLUG_ProdClrBootTime},//20211112 dyl 开机总时间、运行总计时统计
    {"ProdClrProdTime",   PLUG_ProdClrProdTime},//20211112 dyl 开机总时间、运行总计时统计
	{"ReadRecord",   PLUG_ReadRecord},
	{"ClearParamRecord",   PLUG_ClearParamRecord},
    {"SetRecordByManual",   PLUG_SetRecordByManual},//20210629
	{"IsWindowsSys",   PLUG_IsWindowsSys},
	{"PageFramBackground",   PLUG_PageFramBackground},
	{"IsPayChecking",   PLUG_IsPayChecking},
	{"ControlPass",   PLUG_ControlPass},
	{"set_date",   PLUG_SetDate},
	{"set_time",   PLUG_SetTime},
	{"ShowMainPage",   PLUG_ShowMainPage},
	{"ParseCSVFile",   PLUG_ParseCSVFile},
	{"GetMapTran",   PLUG_GetMapTran},
	{"GetServoData",   PLUG_GetServoData},
	{"GetServoDataCnt",   PLUG_GetServoDataCnt},
	{"SetServoDataCnt",   PLUG_SetServoDataCnt},
	{"PanelType",   PLUG_Panel_Type},
    {"PanelShowPageByName",   PLUG_PanelShowPageByName},
    {"GetCurrentUserId",   PLUG_GetCurrentUserId},//20191118.cyx
    {"SetPageLockMode",    PLUG_SetPageLockMode},
    {"SaveLubStartTm",      PLUG_SaveLubStartTm},//20210507
    {"GetLubHour",          PLUG_GetLubHour},
    {"SetLubHour",          PLUG_SetLubHour},
    {"GetLeftLubHour",      PLUG_GetLeftLubHour},
    {"SetLeftLubHour",      PLUG_SetLeftLubHour},
    {"PageFrameYSort",      PLUG_PageFrameYSort},//20210623.cyx
    {"PageFrameXSort",      PLUG_PageFrameXSort},//20210607.cyx + //20211229 dyl
    {"PageSubXExChange",    PLUG_PageSubXExChange},//20211229 dyl
    {"SyncCoreAdvRetPos",   PLUG_SyncCoreAdvRetPos},//20211221 dyl
    {"CorePointByLink",     PLUG_CorePointByLink},//20211221 dyl
    {"CoreEffeAreaByPushtab", PLUG_CoreEffeAreaByPushtab},//20211221 dyl
    {"GetADMaxNumber",      PLUG_GetADMaxNumber},//20220718 dyl 当3101/3202等使用6路电子尺时的电子尺通道选择
    {"CalClampPrsMaxProtectPSW", PLUG_CalClampPrsMaxProtectPSW}, //20241017 chj 金鹰特殊厂商密码计算 开关模压力上限密码保护计算
#endif
#endif //LUA_OPTIMIZE_MEMORY

#ifdef CLUA_TEST
	{"test1",   TEST1},
#endif

#ifdef CLUA_UI16ARRAYLIB
	{"newArray", newarray},
	{"setArray", setarray},
	{"getArray", getarray},
	{"arraySize", getsize},
#endif	//CLUA_UI16ARRAYLIB
	{NULL, NULL}
};

void Register_WGTLib_init()  //20181212 hz
{
	Register_WGTLib(lWGTLib, sizeof(lWGTLib));
}
