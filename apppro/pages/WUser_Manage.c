#include "WUser_Manage.h"
#include "typedef.h"
#include "pagefram.h"
#include "usermod.h"
#include "labelwgt.h"
#include "texttbl.h"
#include "privdef.h"
#include "database.h"
#include "oprintf.h"

DEFINE_USER_MANAGE_MAP

CONNECT_USER_MANAGE_EVENT

#define USERPASSWORD1    0x3317001B  //密码1(0)
#define USER_INITRANK	 0x33170025  //用户初始等级
#define USER_LOGINPWD	 TMPVAL(0)   //用户登录密码
#define TMP_USER_RANK	 TMPVAL(1)   //用户等级
#define TMP_USER_NEWPWD	 TMPVAL(2)   //用户新密码
#define TMP_USER_PWDCONF TMPVAL(3)   //用户密码确认

static BOOL bKeyLock;//20191121.cyx 用户权限管理:生产人员lock

static void SetControl(User_Manage_PG* ppg, BOOL bShow)
{
    if(bShow)
    {
        WGTSetEnable(ppg->ed15,TRUE);
        WGTSetEnable(ppg->ed16,TRUE);
        WGTSetEnable(ppg->bt17,TRUE);
    }
    else
    {
        WGTSetEnable(ppg->ed15,FALSE);
        WGTSetEnable(ppg->ed16,FALSE);
        WGTSetEnable(ppg->bt17,FALSE);
    }
    WGTSetVisible(ppg->stPwdState,FALSE);//20250306 chj 在切换等级时，需要将密码修改提示隐藏
}

//初始用户等级设置
static void SetUserInitShow(User_Manage_PG* ppg, BOOL bShow)
{
    WGTSetVisible(ppg->groupbox4,bShow);
    WGTSetVisible(ppg->label49,bShow);
    WGTSetVisible(ppg->drop18,bShow);
    WGTSetVisible(ppg->label50,bShow);
    WGTSetVisible(ppg->bt19,bShow);
}

//用户权限对应密码转换
static UI16 TransUser(UI16 bUserId)
{
    UI16 bPwdId=0;
    switch (bUserId) {
    case 0:
        bPwdId=0;
        break;
    case 1:
        bPwdId=2;
        break;
    case 2:
        bPwdId=4;
        break;
    case 3:
        bPwdId=5;
        break;
    default:
        break;
    }
    return bPwdId;
}

static void CtlConnect()
{
}
static int User_ManageOnInit(User_Manage_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int User_ManageOnShow(User_Manage_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(GetCurrentUserId() >= USER_MANUFACTURER)
    {
        VarAdrSetInt(TMP_USER_RANK,3);
    }
    else
    {
        VarAdrSetInt(TMP_USER_RANK,GetCurrentUserId());
    }
    VarAdrSetInt(USER_LOGINPWD,12345);
    VarAdrSetInt(TMP_USER_NEWPWD,12345);
    VarAdrSetInt(TMP_USER_PWDCONF,54321);

    WGTSetVisible(ppg->stUserState,FALSE);
    WGTSetVisible(ppg->stPwdState,FALSE);
    SetControl(ppg,FALSE);

    if(GetCurrentUserId() > USER_MANUFACTURER)
    {
        SetUserInitShow(ppg, TRUE);
    }
    else
    {
        SetUserInitShow(ppg, FALSE);
    }

    bKeyLock = VarAdrToInt(USER_KEYLOCK);
    VarAdrSetInt(USER_KEYLOCK,0);
    return 0;
}
static int User_ManageOnHide(User_Manage_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(GetCurrentUserId() > USER_EVERYONE)
    {
        VarAdrSetInt(USER_KEYLOCK,0);
    }
    else
    {
        VarAdrSetInt(USER_KEYLOCK,1);
    }
    //VarAdrSetInt(USER_KEYLOCK,bKeyLock);
    return 0;
}
static int User_ManageOnUpdate(User_Manage_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    //自动登出刷新
    if(VarAdrToInt(USER_STATE))
    {
        VarAdrSetInt(TMP_USER_RANK,GetCurrentUserId());
        VarAdrSetInt(USER_LOGINPWD,12345);
        SetControl(ppg,FALSE);
        VarAdrSetInt(USER_STATE,0);
        if(VarAdrToInt(USER_INITRANK) == 0)//20190524.cyx 用户权限管理:生产人员lock
        {
            bKeyLock = VarAdrToInt(USER_KEYLOCK);
            VarAdrSetInt(USER_KEYLOCK,0);
        }

        //初始用户等级设置
        SetUserInitShow(ppg, FALSE);
    }
    return 0;
}
static int User_ManageOnChange(User_Manage_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 i,bId;
    UI32 wUser,wPass,wValue;

    if(pwgt == ppg->drop14)
    {
        SetControl(ppg,FALSE);
    }
    else if(pwgt == ppg->bt12) //用户登入
    {
        i = VarAdrToInt(TMP_USER_RANK);
        wUser = VarAdrToInt(USER_LOGINPWD);
        bId = TransUser(i);

        if(wUser == (VarAdrToInt(USERPASSWORD1+bId)))
        {
            SetControl(ppg,TRUE);
            SetCurrentUserId(i);
            WGTSetVisible(ppg->stUserState,TRUE);
            LabelSetText(ppg->stUserState, GetTextTran(TEXT_USERSUCCESS));
        }
        else if((i == 3)&&(wUser == 9519119))//9519119 为固定值判断,避免资料错乱无法登录高级密码
        {
            SetControl(ppg,FALSE);
            SetCurrentUserId(USER_SUPERVISOR);
            WGTSetVisible(ppg->stUserState,TRUE);
            LabelSetText(ppg->stUserState, GetTextTran(TEXT_USERSUCCESS));
        }
        else
        {
            WGTSetVisible(ppg->stUserState,TRUE);
            LabelSetText(ppg->stUserState, GetTextTran(TEXT_USERERROR));
            return FALSE;
        }

        //20190524.cyx 用户权限管理:生产人员lock
        if(GetCurrentUserId()==USER_EVERYONE)
        {
            VarAdrSetInt(USER_KEYLOCK,1);
            bKeyLock = VarAdrToInt(USER_KEYLOCK);
            VarAdrSetInt(USER_KEYLOCK,0);
        }
        else
        {
            VarAdrSetInt(USER_KEYLOCK,0);
            bKeyLock = VarAdrToInt(USER_KEYLOCK);
        }

        //初始用户等级设置
        if(GetCurrentUserId() > USER_MANUFACTURER)
        {
            SetUserInitShow(ppg, TRUE);
        }
        else
        {
            SetUserInitShow(ppg, FALSE);
        }
    }
    else if(pwgt == ppg->bt13) //自动登出
    {
        //开机初始等级选择
        if(VarAdrToInt(USER_INITRANK)==0)
        {
            SetCurrentUserId(USER_EVERYONE);
            VarAdrSetInt(USER_KEYLOCK,1);//20190524.cyx 用户权限管理:生产人员lock
            bKeyLock = VarAdrToInt(USER_KEYLOCK);
            VarAdrSetInt(USER_KEYLOCK,0);
        }
        else
        {
            if(GetCurrentUserId() > USER_PROFESSIONAL)
            {
                SetCurrentUserId(USER_PROFESSIONAL);
            }
        }
        VarAdrSetInt(TMP_USER_RANK,GetCurrentUserId());
        VarAdrSetInt(USER_LOGINPWD,12345);
        SetControl(ppg,FALSE);

        //VarAdrSetInt(USER_INITRANK,USER_PROFESSIONAL);//20191206.cyx 登出时初始权限下降为1-专业人员
        //VarAdrSaveInt(USER_INITRANK);
        //初始用户等级设置
        SetUserInitShow(ppg, FALSE);
    }
    else if(pwgt == ppg->bt17) //密码修改确认
    {
        i = VarAdrToInt(TMP_USER_RANK);
        wPass = VarAdrToInt(TMP_USER_NEWPWD);
        wValue = VarAdrToInt(TMP_USER_PWDCONF);
        //用户权限对应密码转换
        bId = TransUser(i);
        if (wPass == wValue)
        {
            VarSendSaveSetIntByAdr(USERPASSWORD1+bId,wValue);
            WGTSetVisible(ppg->stPwdState,TRUE);
            LabelSetText(ppg->stPwdState, GetTextTran(TEXT_PWDSUCCESS));
        }
        else
        {
            WGTSetVisible(ppg->stPwdState,TRUE);
            LabelSetText(ppg->stPwdState, GetTextTran(TEXT_PWDNOTMATCH));
            return FALSE;
        }
    }
    else if(pwgt == ppg->bt19) //密码重置
    {
        PanelShowDialogByName(DIALOG_USERPWD_RESET);
    }

    return 0;
}
