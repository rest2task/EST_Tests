#include "WServoStudy.h"
#include "typedef.h"
#include "pagefram.h"
#include "oprecord.h"
#include "database.h"
#include "texttbl.h"
#include "buttonwgt.h"
#include "privdef.h"
#include "key.h"
#include "keydef.h"
#include "keymod.h"
#include "tabpagewgt.h"
#include "checkboxwgt.h"

DEFINE_SERVOSTUDY_MAP

CONNECT_SERVOSTUDY_EVENT

static void CtlConnect()
{
}
static int ServoStudyOnInit(ServoStudy_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int ServoStudyOnShow(ServoStudy_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x7A00)//20241017 chj 威力士动作调试增加密码
    {
        WGTSetVisible(ppg->tabpage1_Tb1_editpsw,TRUE);
        WGTSetVisible(ppg->tabpage1_Tb1_labelpsw,TRUE);
        VarAdrSetInt(p_PP_TMP_TMPVAL96,0);
    }
    else
    {
        WGTSetVisible(ppg->tabpage1_Tb1_editpsw,FALSE);
        WGTSetVisible(ppg->tabpage1_Tb1_labelpsw,FALSE);
        VarAdrSetInt(p_PP_TMP_TMPVAL96,1);
    }
    VarAdrSetInt(p_PP_TMP_TMPVAL95,1234);

    if(VarAdrToInt(p_PP_SERVO_SERVO_CURRENT_NUM) < 7)
    {
        WGTSetVisible(ppg->label_F5, TRUE);
        WGTSetVisible(ppg->label_F7, FALSE);
        FkeyTab(ppg->tabpage1, GUI_KEY_FUN5);
    }
    else
    {
        WGTSetVisible(ppg->label_F5, FALSE);
        WGTSetVisible(ppg->label_F7, TRUE);
        FkeyTab(ppg->tabpage1, GUI_KEY_FUN7);
    }

    if(VarAdrToInt(SYS_FL_MACH_CODE1) & 0x04)
    {
        TabPageSetSubVisble(ppg->tabpage1,1,TRUE);
        //20250109 fqh 标准程序 超压抑制功能新增
        TabPageSetSubVisble(ppg->tabpage1,2,TRUE);
    }
    else
    {
        TabPageSetSubVisble(ppg->tabpage1,1,FALSE);
        TabPageSetSubVisble(ppg->tabpage1,2,FALSE);
        WGTSetVisible(ppg->label_F5, FALSE);
        WGTSetVisible(ppg->label_F7, FALSE);
    }
    TabPageSelPage(ppg->tabpage1,0);

    return 0;
}
static int ServoStudyOnHide(ServoStudy_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int ServoStudyOnUpdate(ServoStudy_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 op_mode;
    op_mode = VarAdrToUI16(d_machine1_STATE_ST_WOPERMODE);
    if((op_mode & 0xFFFF) == MODE_PRESADJUST)
    {
        ButtonSetText(ppg->tabpage1_Tb2_button1, GetTextTran(TEXT_ADJUSTING));
    }
    else
    {
        ButtonSetText(ppg->tabpage1_Tb2_button1, GetTextTran(TEXT_PRESS_ADJUST));
    }
    return 0;
}
static int ServoStudyOnChange(ServoStudy_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 addr,value;
    PATTR pattr;

    pattr = (PATTR)msg_para;

    addr = OBJAttrGetAdr(pattr);
    value = VarAdrToInt(addr);

    if(addr == d_inject1_HYDR_SERVO_KP_INJCT)
    {
        if(VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE) == 0)
        {
            if(value != VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_HIGH))
            {
                VarSendSaveSetIntByAdr(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_HIGH, value);
            }
        }
        else if(VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE) == 1)
        {
            if(value != VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MID))
            {
                VarSendSaveSetIntByAdr(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MID, value);
            }
        }
        else if(VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE) == 2)
        {
            if(value != VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_LOW))
            {
                VarSendSaveSetIntByAdr(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_LOW, value);
            }
        }
    }
    else if(addr == d_inject1_HYDR_SERVO_KI_INJCT)
    {
        if(VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE) == 0)
        {
            if(value != VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KI_INJCT_HIGH))
            {
                VarSendSaveSetIntByAdr(d_autoctrl1_MOLDSET_SYS_SERVO_KI_INJCT_HIGH, value);
            }
        }
        else if(VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE) == 1)
        {
            if(value != VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KI_INJCT_MID))
            {
                VarSendSaveSetIntByAdr(d_autoctrl1_MOLDSET_SYS_SERVO_KI_INJCT_MID, value);
            }
        }
        else if(VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE) == 2)
        {
            if(value != VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KI_INJCT_LOW))
            {
                VarSendSaveSetIntByAdr(d_autoctrl1_MOLDSET_SYS_SERVO_KI_INJCT_LOW, value);
            }
        }
    }
    else if(addr == d_inject1_HYDR_SERVO_KP_HOLD)
    {
        if(VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE) == 0)
        {
            if(value != VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_HOLD_HIGH))
            {
                VarSendSaveSetIntByAdr(d_autoctrl1_MOLDSET_SYS_SERVO_KP_HOLD_HIGH, value);
            }
        }
        else if(VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE) == 1)
        {
            if(value != VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_HOLD_MID))
            {
                VarSendSaveSetIntByAdr(d_autoctrl1_MOLDSET_SYS_SERVO_KP_HOLD_MID, value);
            }
        }
        else if(VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE) == 2)
        {
            if(value != VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_HOLD_LOW))
            {
                VarSendSaveSetIntByAdr(d_autoctrl1_MOLDSET_SYS_SERVO_KP_HOLD_LOW, value);
            }
        }
    }
    else if(addr == d_inject1_HYDR_SERVO_KI_HOLD)
    {
        if(VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE) == 0)
        {
            if(value != VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KI_HOLD_HIGH))
            {
                VarSendSaveSetIntByAdr(d_autoctrl1_MOLDSET_SYS_SERVO_KI_HOLD_HIGH, value);
            }
        }
        else if(VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE) == 1)
        {
            if(value != VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KI_HOLD_MID))
            {
                VarSendSaveSetIntByAdr(d_autoctrl1_MOLDSET_SYS_SERVO_KI_HOLD_MID, value);
            }
        }
        else if(VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE) == 2)
        {
            if(value != VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KI_HOLD_LOW))
            {
                VarSendSaveSetIntByAdr(d_autoctrl1_MOLDSET_SYS_SERVO_KI_HOLD_LOW, value);
            }
        }
    }

    if(pwgt == ppg->tabpage1_Tb2_button1)
    {
        SendKey(_KEY_PANEL_PRESADJUST);
        SendKey(KEY_RELEASE);//发送弹起键，防止下次按键无效
    }
    else if(pwgt == ppg->tabpage1_Tb2_check1)
    {
        if(CheckboxGetCheck(ppg->tabpage1_Tb2_check1))
        {
            CheckboxSetCheck(ppg->tabpage1_Tb2_checkbox1,0);
            CheckboxSetCheck(ppg->tabpage1_Tb2_checkbox2,0);
        }
    }
    else if(pwgt == ppg->tabpage1_Tb2_checkbox1)
    {
        if(CheckboxGetCheck(ppg->tabpage1_Tb2_checkbox1))
        {
            CheckboxSetCheck(ppg->tabpage1_Tb2_check1,0);
            CheckboxSetCheck(ppg->tabpage1_Tb2_checkbox2,0);
        }
    }
    else if(pwgt == ppg->tabpage1_Tb2_checkbox2)
    {
        if(CheckboxGetCheck(ppg->tabpage1_Tb2_checkbox2))
        {
            CheckboxSetCheck(ppg->tabpage1_Tb2_check1,0);
            CheckboxSetCheck(ppg->tabpage1_Tb2_checkbox1,0);
        }
    }
    else if(pwgt == ppg->tabpage1_Tb1_editpsw)//20241017 chj 威力士动作调试增加密码
    {
        if(value==45625)
        {
            VarAdrSetInt(p_PP_TMP_TMPVAL96,1);
        }
    }

    return 0;
}
