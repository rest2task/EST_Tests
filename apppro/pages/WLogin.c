#include "WLogin.h"
#include "typedef.h"
#include "database.h"
#include "login.h"
#include "panel.h"
#include "pay.h"
#include "keydef.h"
#include "usermod.h"
#include "privdef.h"
#include "keymod.h"
#include "panelcfg.h"
#include "editwgt.h"
#include "oprintf.h"

DEFINE_LOGIN_MAP

CONNECT_LOGIN_EVENT

#define UserPassword1   0x3317001B //用户登录密码1

extern PPAGE_FRAM m_pLoginDlg;//20200303
static UI32 lastPassword = 0; //20241225 chj 修复在弹出框输入密码后，触摸按画面上的确定按键，无效的问题

//用户权限对应权限转换
static UI16 TransPrivilege(UI16 bPrivilege)
{
    UI16 bUsePri = 0;

    switch (bPrivilege){
    case 0:
        bUsePri = 0;
        break;
    case 2:
        bUsePri = 1;
        break;
    case 4:
        bUsePri = 2;
        break;
    case 5:
        bUsePri = 3;
        break;
    default:
        break;
    }

    return bUsePri;
}

static BOOL LoginCheck(Login_PG* ppg)
{
    UI32 passwowrd;

    passwowrd = VarAdrToInt(LOGIN_PWD);

    if(IsPayTimeOut())  //20191219
    {
        PanelCloseDialogByName(login_pg_name);
        return FALSE;
    }

    if(VarAdrToInt(p_PP_MACHSET_G_KEYVALUE))
    {
        if(passwowrd == 1234) //20200303操作模式切换密码
        {
            if(VarAdrToInt(p_PP_MACHSET_FLOPMODE_USEPSW)==1 )
            {
                if(VarAdrToInt(p_PP_MACHSET_G_KEYVALUE)==_KEY_PANEL_SEMIAUTO ||VarAdrToInt(p_PP_MACHSET_G_KEYVALUE)==_KEY_PANEL_SENSORAUTO
                        ||VarAdrToInt(p_PP_MACHSET_G_KEYVALUE)==_KEY_PANEL_TIMEAUTO )
                {
                    SendPanelKey(VarAdrToInt(p_PP_MACHSET_G_KEYVALUE),1);//1：表示按下
                    VarAdrSetInt(p_PP_MACHSET_G_KEYVALUE,0);
                    if(m_pLoginDlg!=NULL)
                    {
                        PanelCloseDialog(m_pLoginDlg);
                    }
                }

            }
        }

        return FALSE;
    }

    if(passwowrd == 1)//screen shot PNG
    {
        SendKey(_KEY_GRAB_PNGIMAGE);
        PanelCloseDialogByName(login_pg_name);
        return FALSE;
    }
    else if(passwowrd == 2)//screen shot BMP
    {
        SendKey(_KEY_GRAB_BMPIMAGE);
        PanelCloseDialogByName(login_pg_name);
        return FALSE;
    }
    else if(passwowrd == 4545)
    {
        if((GetCurrentUserId() >= USER_DEALER) && ((VarAdrToUI16(SYS_FL_MACH_CODE1)&0x04)
             || (((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x0004)==0) && (VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020))))
        {
            if(((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x0004)==0) && (VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020))
            {
                PanelShowPageByName(PAGE_SERVO_STATE);
            }
            else
            {
                PanelShowPageByName(PAGE_SERVOSTATEX1);
            }
            return FALSE;
        }
        else
        {
            WGTSetVisible(ppg->lblinfo,TRUE);
            return FALSE;
        }
    }
    else if(passwowrd == 5656)
    {
        if(GetCurrentUserId() >= USER_DEALER)
        {
            PanelShowPageByName(PAGE_MACH_CFG_5200);
            return FALSE;
        }
        else
        {
            WGTSetVisible(ppg->lblinfo,TRUE);
            return FALSE;
        }
    }
    else if(passwowrd == 5959)
    {
        if(GetCurrentUserId() >= USER_MANUFACTURER)
        {
            PanelShowPageByName(PAGE_PROGUPDATE);
            return FALSE;
        }
        else
        {
            WGTSetVisible(ppg->lblinfo,TRUE);
            return FALSE;
        }
    }
    else if(passwowrd == 7474)
    {
        if(GetCurrentUserId() >= USER_SUPERVISOR)
        {
            PanelShowPageByName(PAGE_MAIN_ADJUSTX);
            return FALSE;
        }
        else
        {
            WGTSetVisible(ppg->lblinfo,TRUE);
            return FALSE;
        }
    }
    else if(passwowrd == 5252)
    {
        if(GetCurrentUserId() >= USER_SUPERVISOR)
        {
            PanelShowPageByName(PAGE_MIAN_RECORD);
            return FALSE;
        }
        else
        {
            WGTSetVisible(ppg->lblinfo,TRUE);
            return FALSE;
        }
    }
    else
    {
        int i;
        for(i = 0; i < 10; i++) //密码权限预留10个
        {
            UI32 user_password = VarAdrToInt(UserPassword1 + i);

            if(passwowrd == user_password)
            {
                UI16 userid = TransPrivilege(i);
                if((userid >= USER_PROFESSIONAL) && (userid <= USER_MANUFACTURER))
                {
                    if(GetCurrentUserId() < userid)
                    {
                        SetCurrentUserId(userid);
                    }

                    VarAdrSetInt(KEY_LAST_FOCUS,21+10*(userid-USER_PROFESSIONAL)); //21代表画面（key）5858密码对应的版本控件索引值
                    PanelShowPageByName(PAGE_NEXTPAGE);
                }

                //20190524.cyx 用户权限管理:生产人员lock + //20220228 dyl
                if(GetCurrentUserId()==USER_EVERYONE)
                {
                    VarAdrSetInt(USER_KEYLOCK,1);
                }
                else
                {
                    VarAdrSetInt(USER_KEYLOCK,0);
                }
            }
        }

        if(!PanelCfgHintStyle())
        {
            PageWgtSetFocus(ppg->ok);
        }

        return FALSE;
    }
}

static int cancelOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int okOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static void CtlConnect()
{
    WgtMsgConnect(login_pg.ok, WGT_CLICK_MSG, okOnClick);
    WgtMsgConnect(login_pg.cancel, WGT_CLICK_MSG, cancelOnClick);
}
static int LoginOnInit(Login_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int LoginOnShow(Login_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    WGTSetVisible(ppg->lblinfo,FALSE);
    VarAdrSetInt(LOGIN_PWD, 11111);

    lastPassword = 0;//20241225 chj 修复在弹出框输入密码后，触摸按画面上的确定按键，无效的问题

    return 0;
}
static int LoginOnHide(Login_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(VarAdrToInt(p_PP_MACHSET_G_KEYVALUE))
    {
        VarAdrSetInt(p_PP_MACHSET_G_KEYVALUE,0);
    }
    PageWgtSetFocus(ppg->ok);

    lastPassword = 0;//20241225 chj 修复在弹出框输入密码后，触摸按画面上的确定按键，无效的问题

    return 0;
}
static int LoginOnUpdate(Login_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    PEDIT_WGT pedit = WgtToEdit(ppg->password);//20241225 chj 修复在弹出框输入密码后，触摸按画面上的确定按键，无效的问题
    if(pedit != NULL)
    {
        if(pedit->ed_value != 0)
        {
            lastPassword = pedit->ed_value;
        }
    }

    return 0;
}
static int LoginOnChange(Login_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 addr;
    PATTR pattr;

    pattr = (PATTR)msg_para;

    addr = OBJAttrGetAdr(pattr);

    if(addr == LOGIN_PWD)
    {
        LoginCheck(ppg);
    }
    else if(pwgt == ppg->ok)
    {
        LoginCheck(ppg);
    }
    else if(pwgt == PageGetWgtByName(login_pg_name,"btnclose"))
    {
        PanelCloseDialogByName(login_pg_name);//20191223.cyx
    }

    return 0;
}
static int cancelOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    PanelCloseDialogByName(login_pg_name);

    return 0;
}
static int okOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    oprintf("lastPassword:%d\n",lastPassword);
    if(lastPassword != 0)//20241225 chj 修复在弹出框输入密码后，触摸按画面上的确定按键，无效的问题
    {
        VarAdrSetInt(LOGIN_PWD,lastPassword);
    }
    return 0;
}
