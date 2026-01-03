#include "WUserPwd_Reset.h"
#include "typedef.h"
#include "pagefram.h"
#include "database.h"

DEFINE_USERPWD_RESET_MAP

CONNECT_USERPWD_RESET_EVENT

#define USERPASSWORD1    0x3317001B  //密码1(0)

static UI32 UserPwd[] = {0, 5858, 14321, 12995};

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
static int UserPwd_ResetOnInit(UserPwd_Reset_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int UserPwd_ResetOnShow(UserPwd_Reset_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int UserPwd_ResetOnHide(UserPwd_Reset_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int UserPwd_ResetOnUpdate(UserPwd_Reset_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int UserPwd_ResetOnChange(UserPwd_Reset_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 i,bId;
    if(pwgt == ppg->button8)
    {
        for(i = 0; i < 4; i++)
        {
            bId = TransUser(i);
            VarAdrSetInt(USERPASSWORD1+bId, UserPwd[i]);
            VarAdrSaveInt(USERPASSWORD1+bId);
        }

        PanelCloseDialogByName(userpwd_reset_pg_name);
    }
    else if(pwgt == ppg->button9)
    {
        PanelCloseDialogByName(userpwd_reset_pg_name);
    }

    return 0;
}
