#include "WKey.h"
#include "typedef.h"
#include "pagefram.h"
#include "information.h"
#include "database.h"
#include "oprecord.h"
#include "dropwgt.h"
#include "texttbl.h"
#include "tabpagewgt.h"
#include "usermod.h"
#include "pay.h"
#include "verinfo.h"
#include "panelcfg.h"
#include "oprintf.h"
DEFINE_KEY_MAP

CONNECT_KEY_EVENT

#define TMP_AD_GROUPSHOW    TMPVAL(4) //AD分组框可见
#define TMP_DA_GROUPSHOW    TMPVAL(5) //更新分组框可见

static BOOL pagein = TRUE;

enum KEY_GOFFSET
{
    ID_GROUP1   = 1,

    ID_GROUP2	= 11,

    ID_GROUP3	= 21,

    ID_GROUP4	= 31,

    ID_GROUP5	= 41,

    ID_GROUP6	= 51,

    ID_END = 60
};

static void CtlConnect()
{
}
static int KeyOnInit(Key_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(VarAdrToInt(KEY_LAST_FOCUS) == 0)
    {
        VarAdrSetInt(KEY_LAST_FOCUS,ID_GROUP3);
    }

    return 0;
}
static int KeyOnShow(Key_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    VarAdrSetStr(0x3317003e,"btnNEXTx.png");//换页常态按钮为按下状态
    VarAdrSetInt(0x331700ba,0x0000FFFF);//换页文本色为按下状态

    if(GetCurrentUserId() >= USER_MANUFACTURER)
    {
        VarAdrSetInt(TMP_AD_GROUPSHOW,1);
        VarAdrSetInt(TMP_DA_GROUPSHOW,1);
    }
    else if(GetCurrentUserId() == USER_DEALER)
    {
        VarAdrSetInt(TMP_AD_GROUPSHOW,1);
        VarAdrSetInt(TMP_DA_GROUPSHOW,0);
    }
    else
    {
        VarAdrSetInt(TMP_AD_GROUPSHOW,0);
        VarAdrSetInt(TMP_DA_GROUPSHOW,0);
    }

    if (GetCurrentUserId() >= USER_SUPERVISOR)
    {
        WGTSetVisible(ppg->btxtest,TRUE);
        WGTSetVisible(ppg->lbltest,TRUE);
    }
    else
    {
        WGTSetVisible(ppg->btxtest,FALSE);
        WGTSetVisible(ppg->lbltest,FALSE);
    }

    //20211117 dyl 伺服状态画面等级放低到LV2
    if((GetCurrentUserId() >= USER_PROFESSIONAL) && ((VarAdrToUI16(SYS_FL_MACH_CODE1)&0x04)
         || (((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x0004)==0) && (VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020))))
    {
        WGTSetVisible(ppg->btxservo,TRUE);
        WGTSetVisible(ppg->lblservo,TRUE);
    }
    else
    {
        WGTSetVisible(ppg->btxservo,FALSE);
        WGTSetVisible(ppg->lblservo,FALSE);
    }

    pagein = TRUE;

    //20200407HJM 模温功能
    if ((VarAdrToInt(MT_CTRL_PART) == 0) || (VarAdrToInt(MT_BOARD_COUNT) == 0))
    {
        //oprintf("subpart is %d,mt_board_count is %d\n",VarAdrToUI16(MT_CTRL_PART),VarAdrToUI16(MT_BOARD_COUNT));
        WGTSetVisible(ppg->btxMT,false);
        WGTSetVisible(ppg->lblMT,false);
    }
    else
    {
        WGTSetVisible(ppg->btxMT,true);
        WGTSetVisible(ppg->lblMT,true);
    }

    if(VarAdrToInt(USER_STATE))//自动登出刷新
    {
        VarAdrSetInt(USER_STATE,0);
    }

//20250303 chj 触摸版本弹出软键盘输入中文，非触摸版本，面板物理按键输入中文
#ifdef MPANEL_TC
    NaviSetIndex(PanelCurPage(),1);
#else
    NaviSetIndex(PanelCurPage(),0);
#endif

    return 0;
}
static int KeyOnHide(Key_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    VarAdrSetStr(0x3317003e,"btnNEXT.png");//换页常态按钮恢复
    VarAdrSetInt(0x331700ba,0x00000000);//换页文本色恢复

    VarAdrSetInt(KEY_LAST_FOCUS,WGTGetGOffset(PageFrameCurFocusWgt()));

    return 0;
}
static int KeyOnUpdate(Key_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    static UI32 key_last = 0;
    if(pagein)
    {
        pagein = FALSE;
        if(WGTGetVisible(PageFramCurWgtByIndex(VarAdrToInt(KEY_LAST_FOCUS))))
        {
            key_last = VarAdrToInt(KEY_LAST_FOCUS);
            PageWgtSetFocus(PageFramCurWgtByIndex(VarAdrToInt(KEY_LAST_FOCUS)));
        }
        else
        {
            VarAdrSetInt(KEY_LAST_FOCUS,ID_GROUP1);
            key_last = ID_GROUP1;
        }
    }

    if(key_last != VarAdrToInt(KEY_LAST_FOCUS) && !pagein)
    {
        PanelCloseDialogByName(DIALOG_LOGIN);

        if(GetCurrentUserId() >= USER_MANUFACTURER)
        {
            VarAdrSetInt(TMP_AD_GROUPSHOW,1);
            VarAdrSetInt(TMP_DA_GROUPSHOW,1);
        }
        else if(GetCurrentUserId() == USER_DEALER)
        {
            VarAdrSetInt(TMP_AD_GROUPSHOW,1);
            VarAdrSetInt(TMP_DA_GROUPSHOW,0);
        }
        else
        {
            VarAdrSetInt(TMP_AD_GROUPSHOW,0);
            VarAdrSetInt(TMP_DA_GROUPSHOW,0);
        }

        if (GetCurrentUserId() >= USER_SUPERVISOR)
        {
            WGTSetVisible(ppg->btxtest,TRUE);
            WGTSetVisible(ppg->lbltest,TRUE);
        }
        else
        {
            WGTSetVisible(ppg->btxtest,FALSE);
            WGTSetVisible(ppg->lbltest,FALSE);
        }

        //20211117 dyl 伺服状态画面等级放低到LV2
        if((GetCurrentUserId() >= USER_PROFESSIONAL) && ((VarAdrToUI16(SYS_FL_MACH_CODE1)&0x04)
             || (((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x0004)==0) && (VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020))))
        {
            WGTSetVisible(ppg->btxservo,TRUE);
            WGTSetVisible(ppg->lblservo,TRUE);
        }
        else
        {
            WGTSetVisible(ppg->btxservo,FALSE);
            WGTSetVisible(ppg->lblservo,FALSE);
        }

        pagein = TRUE;
    }

    return 0;
}
static int KeyOnChange(Key_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    //GROUP 1
    if(pwgt == ppg->btxclamp)
    {
        if(VarAdrToInt(FL_CLSOPEN_MODE) == 2)
        {
            PanelShowPageByName(PAGE_CLAMPX);
        }
        else if((VarAdrToInt(FL_CLSOPEN_MODE) == 3) || (VarAdrToInt(FL_CLSOPEN_MODE) == 4))
        {
            PanelShowPageByName(PAGE_CLAMP_SMART);
        }
        else
        {
            PanelShowPageByName(PAGE_CLAMP);
        }
    }
    else if(pwgt == ppg->btxeject)
    {
        PanelShowPageByName(PAGE_EJECT);
    }
    else if(pwgt == ppg->btxinject)
    {
        PanelShowPageByName(PAGE_INJECT);
    }
    else if(pwgt == ppg->btxcharge)
    {
        PanelShowPageByName(PAGE_CHARGE);
    }
    else if(pwgt == ppg->btxcore)
    {
        PanelShowPageByName(PAGE_CORE);
    }
    else if(pwgt == ppg->btxtemp)
    {
        PanelShowPageByName(PAGE_TEMP_XX);
    }
    else if(pwgt == ppg->btxnozzle)
    {
        PanelShowPageByName(PAGE_OTHER);
    }
    else if(pwgt == ppg->btxother)
    {
        PanelShowPageByName(PAGE_OTHER_ADJUST);
    }
    //GROUP 2
    else if(pwgt == ppg->btxproduct)
    {
        PanelShowPageByName(PAGE_PRODMANAGE);
    }
    else if(pwgt == ppg->btxmonitor)
    {
        PanelShowPageByName(PAGE_PROD_MONI);
    }
    else if(pwgt == ppg->btxalarm)
    {
        PanelShowPageByName(PAGE_ALARM);
    }
    else if(pwgt == ppg->btxIO)
    {
        PanelShowPageByName(PAGE_IO_INPUT);
    }
    else if(pwgt == ppg->btxrecord)
    {
        PanelShowPageByName(PAGE_PARA_RECORD);
    }
    else if(pwgt == ppg->btxmold)
    {
        //20250303 chj 触摸版本弹出软键盘输入中文，非触摸版本，面板物理按键输入中文
        #ifdef MPANEL_TC
            PanelShowPageByName(PAGE_MOLD_SETX);
        #else
            PanelShowPageByName(PAGE_MOLD_SET);
        #endif

    }
    else if(pwgt == ppg->btxMT)
    {
        PanelShowPageByName(PAGE_MOLD_TEMP);
    }
    //GROUP 3
    else if(pwgt == ppg->btxversion)
    {
        PanelShowPageByName(PAGE_VERSION);
    }
    else if(pwgt == ppg->btxlang)
    {
        PanelShowPageByName(PAGE_LANGUAGE);
    }
    else if(pwgt == ppg->btxuser)
    {
        PanelShowPageByName(PAGE_USER_MANAGE);
    }
    else if(pwgt == ppg->btxsystem)
    {
        PanelShowPageByName(PAGE_SYS_CFG);
    }
    else if(pwgt == ppg->btxdetect)
    {
        PanelShowPageByName(PAGE_DATA_DIAG);
    }
    else if(pwgt == ppg->btxkeytest)
    {
        //20250314 jhh 增加金鹰快速机按键测试画面
        if((VarAdrToInt(p_PP_MACHSET_SPC_KEY_USE_JY) == 1) && (PanelCfgSize()==DISPLAY_SIZE_10) && ((VarAdrToInt(d_machine1_MACHSET_FL_CUSTOMID)&0xFF00) == 0x6400))
        {
            PanelShowPageByName(PAGE_KEY_TEST_JY);
        }
        else
        {
            PanelShowPageByName(PAGE_KEY_TEST);
        }
    }
    else if(pwgt == ppg->btxspcfunc)
    {
        PanelShowPageByName(PAGE_SPEC_FUNC1);
    }
    else if(pwgt == ppg->btxservo)
    {
        if(((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x0004)==0) && (VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020))
        {
            if(((VarAdrToInt(d_machine1_MACHSET_FL_CUSTOMID)&0xFF00) == 0x6400) && (VarAdrToInt(SYS_FL_MACH_CODE54) & 0x100))//20241205 chj 金鹰增加麦格米特模式
            {
                PanelShowPageByName(PAGE_SERVO_STATEMM);
            }
            else
            {
                PanelShowPageByName(PAGE_SERVO_STATE);
            }

        }
        else
        {
            PanelShowPageByName(PAGE_SERVOSTATEX1);
        }
    }
    //GROUP 4
    else if(pwgt == ppg->btxAD)
    {
        PanelShowPageByName(PAGE_AD_ADJUSTX);//20200325
    }
    else if(pwgt == ppg->btxDA)
    {
        PanelShowPageByName(PAGE_DA_ADJUST);
    }
    else if(pwgt == ppg->btxconfig)
    {
        PanelShowPageByName(PAGE_MACH_CFG_5200);//20200325
    }
    else if(pwgt == ppg->btxIOdefine)
    {
        PanelShowPageByName(PAGE_IO_REDEFINE);
    }
    else if(pwgt == ppg->btxpump)
    {
        if(VarAdrToInt(SYS_FL_MACH_CODE1) & 0x04)
        {
            PanelShowPageByName(PAGE_PUMP_CFG_XX);
        }
        else if((VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020)&&((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x04) == 0))
        {
            PanelShowPageByName(PAGE_PUMP_CFG_X);
        }
        else if((VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020)&&(VarAdrToInt(SYS_FL_MACH_CODE0) & 0x04))
        {
            PanelShowPageByName(PAGE_PUMP_CFG_XX);
        }
        else
        {
            PanelShowPageByName(PAGE_PUMP_CFG);
        }
    }
    else if(pwgt == ppg->btxlub)
    {
        PanelShowPageByName(PAGE_LUBR);
    }
    else if(pwgt == ppg->btxparam)
    {
        PanelShowPageByName(PAGE_SYS_PARA);
    }

    //GROUP 5
    else if(pwgt == ppg->btxupdate)
    {
        PanelShowPageByName(PAGE_PROGUPDATE);
    }
    else if(pwgt == ppg->btxdata)
    {
        PanelShowPageByName(PAGE_DATA_OPERATION);
    }
    else if(pwgt == ppg->btxinstall)
    {
        if(IsPayChecking()) //20191219 分期停机功能使用
        {
            PanelShowPage(PanelFindPage(PAGE_INSTALLPAY));
        }
        else
        {
            PanelShowPageByName(PAGE_INSTALL);
        }
    }
    else if(pwgt == ppg->btxtest)
    {
        PanelShowPageByName(PAGE_MAIN_ADJUSTX);
    }
    else if(pwgt == ppg->btxadjmach)
    {
        PanelShowPageByName(PAGE_ADJMACH);
    }
    else if(pwgt == ppg->btxkeydefine)
    {
        PanelShowPageByName(PAGE_KEY_DEFINE);
    }
    //GROUP 6
    else if(pwgt == ppg->btxilink)
    {
        PanelShowPageByName(PAGE_NETCFG);
    }
    else if(pwgt == ppg->btximach)
    {
        PanelShowPageByName(PAGE_NETFORM);
    }


    return 0;
}
