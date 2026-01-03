#include "WStudy_Message.h"
#include "typedef.h"
#include "pagefram.h"
#include "progbarwgt.h"
#include "database.h"
#include "protcmd.h"
#include "ctrlcmd.h"
#include "tabpagewgt.h"

DEFINE_STUDY_MESSAGE_MAP

CONNECT_STUDY_MESSAGE_EVENT

#define CLAMP_PART       0x2010
#define INJ_PART         0x1010
#define EJECT_PART       0x2110

static void LabelActShow(BOOL clampshow, BOOL injectshow, BOOL ejectshow)
{
    WGTSetVisible(study_message_pg.tabpage1_tab2_lblact1, clampshow);
    WGTSetVisible(study_message_pg.tabpage1_tab3_lblact1, clampshow);
    WGTSetVisible(study_message_pg.tabpage1_Tab4_lblact1, clampshow);
    WGTSetVisible(study_message_pg.tabpage1_Tab5_lblact1, clampshow);
    WGTSetVisible(study_message_pg.tabpage1_Tab6_lblact1, clampshow);
    WGTSetVisible(study_message_pg.tabpage1_tab3_lblactinfo1, clampshow);
    WGTSetVisible(study_message_pg.tabpage1_Tab5_lblactinfo1, clampshow);

    WGTSetVisible(study_message_pg.tabpage1_tab2_lblact2, injectshow);
    WGTSetVisible(study_message_pg.tabpage1_tab3_lblact2, injectshow);
    WGTSetVisible(study_message_pg.tabpage1_Tab4_lblact2, injectshow);
    WGTSetVisible(study_message_pg.tabpage1_Tab5_lblact2, injectshow);
    WGTSetVisible(study_message_pg.tabpage1_Tab6_lblact2, injectshow);
    WGTSetVisible(study_message_pg.tabpage1_tab3_lblactinfo2, injectshow);
    WGTSetVisible(study_message_pg.tabpage1_Tab5_lblactinfo2, injectshow);

    WGTSetVisible(study_message_pg.tabpage1_tab2_lblact3, ejectshow);
    WGTSetVisible(study_message_pg.tabpage1_tab3_lblact3, ejectshow);
    WGTSetVisible(study_message_pg.tabpage1_Tab4_lblact3, ejectshow);
    WGTSetVisible(study_message_pg.tabpage1_Tab5_lblact3, ejectshow);
    WGTSetVisible(study_message_pg.tabpage1_Tab6_lblact3, ejectshow);
    WGTSetVisible(study_message_pg.tabpage1_tab3_lblactinfo3, ejectshow);
    WGTSetVisible(study_message_pg.tabpage1_Tab5_lblactinfo3, ejectshow);
}

static void CtlConnect()
{
}
static int Study_MessageOnInit(Study_Message_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Study_MessageOnShow(Study_Message_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    ProgbarSetCurValue(study_message_pg.tabpage1_Tab6_progbar1, 0);
    VarAdrSetInt(p_PP_MACHSET_SELF_STUDY_FLAG, 1);//自学习标志

    if(VarAdrToInt(p_PP_MACHSET_STUDY_ID) == CLAMP_PART)
    {
        LabelActShow(TRUE, FALSE, FALSE);
    }
    else if(VarAdrToInt(p_PP_MACHSET_STUDY_ID) == INJ_PART)
    {
        LabelActShow(FALSE, TRUE, FALSE);
    }
    else if(VarAdrToInt(p_PP_MACHSET_STUDY_ID) == EJECT_PART)
    {
        LabelActShow(FALSE, FALSE, TRUE);
    }

    return 0;
}
static int Study_MessageOnHide(Study_Message_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    VarAdrSetInt(p_PP_MACHSET_STUDY_STEP, 0);//动作步骤
    VarAdrSetInt(p_PP_MACHSET_SELF_STUDY_FLAG, 0);//自学习标志
    VarAdrSetInt(p_PP_MACHSET_STUDY_ID, 0);

    return 0;
}
static int Study_MessageOnUpdate(Study_Message_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    static UI16 wLastTab = 65535;
    UI16 wTabSel = 0;
    static UI32  time_300ms  = 0;
    static BOOL  bflash  = 0;

    switch(VarAdrToInt(p_PP_MACHSET_STUDY_STEP))
    {
    case 0x10://关模中
        wTabSel = 1;
        break;
    case 0x20://关模到底
        wTabSel = 2;
        break;
    case 0x30://开模中
        wTabSel = 3;
        break;
    case 0x40://开模到底
        wTabSel = 4;
        break;
    case 0x50://重复开关模
        wTabSel = 5;
        break;
    case 0x60://重复开关模结束-进
        wTabSel = 6;
        break;
    case 0x61://重复开关模结束-退
        wTabSel = 6;
        break;
    case 0xF0://关闭自学习操作
        VarAdrSetInt(p_PP_MACHSET_STUDY_STEP, 0);//动作步骤
        VarAdrSetInt(p_PP_MACHSET_SELF_STUDY_FLAG, 0);//自学习标志
        VarAdrSetInt(p_PP_MACHSET_STUDY_ID, 0);
        PanelCloseDialogByName(DIALOG_STUDY_MESSAGE);
        break;
    default:
        break;
    }

    if(wLastTab != wTabSel)
    {
        wLastTab = wTabSel;
        TabPageSelPage(study_message_pg.tabpage1, wTabSel);
    }

    if(wTabSel == 5)
    {
        ProgbarSetCurValue(study_message_pg.tabpage1_Tab6_progbar1, VarAdrToInt(p_PP_MACHSET_STUDY_PROCESS));
    }

    if((GetTick() - time_300ms) > 300)
    {
        time_300ms = GetTick();
        bflash = !bflash;

        WGTSetVisible(study_message_pg.tabpage1_tab2_lbl1, bflash);
        WGTSetVisible(study_message_pg.tabpage1_tab2_lbl2, bflash);
        WGTSetVisible(study_message_pg.tabpage1_tab3_lbl1, bflash);
        WGTSetVisible(study_message_pg.tabpage1_tab3_lbl2, bflash);
        WGTSetVisible(study_message_pg.tabpage1_Tab4_lbl1, bflash);
        WGTSetVisible(study_message_pg.tabpage1_Tab4_lbl2, bflash);
        WGTSetVisible(study_message_pg.tabpage1_Tab5_lbl1, bflash);
        WGTSetVisible(study_message_pg.tabpage1_Tab5_lbl2, bflash);
        WGTSetVisible(study_message_pg.tabpage1_Tab7_lbl1, bflash);
        WGTSetVisible(study_message_pg.tabpage1_Tab7_lbl2, bflash);
    }

    return 0;
}
static int Study_MessageOnChange(Study_Message_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 pdata[4];
    if(pwgt == study_message_pg.tabpage1_tab1_btnext)
    {
        pdata[0] = 0x01;//开始
        pdata[1] = VarAdrToInt(p_PP_MACHSET_STUDY_ID);//动作步
        pdata[2] = VarAdrToInt(p_PP_MACHSET_SELF_STUDY_PARA);//学习项目
        pdata[3] = 0x01;//第一步
        ProtSysCtrl(COMM_SUBCMD_SLOPE_CALIBRATION,4,pdata);
    }
    else if(pwgt == study_message_pg.tabpage1_tab3_btconfirm)
    {
        pdata[0] = 0x01;//开始
        pdata[1] = VarAdrToInt(p_PP_MACHSET_STUDY_ID);//动作步
        pdata[2] = VarAdrToInt(p_PP_MACHSET_SELF_STUDY_PARA);//学习项目
        pdata[3] = 0x02;//请确认3
        ProtSysCtrl(COMM_SUBCMD_SLOPE_CALIBRATION,4,pdata);
    }
    else if(pwgt == study_message_pg.tabpage1_Tab5_btconfirm)
    {
        pdata[0] = 0x01;//开始
        pdata[1] = VarAdrToInt(p_PP_MACHSET_STUDY_ID);//动作步
        pdata[2] = VarAdrToInt(p_PP_MACHSET_SELF_STUDY_PARA);//学习项目
        pdata[3] = 0x03;//请确认5
        ProtSysCtrl(COMM_SUBCMD_SLOPE_CALIBRATION,4,pdata);
    }
    else if(pwgt == study_message_pg.tabpage1_Tab7_btconfirm)
    {
        pdata[0] = 0x01;//开始
        pdata[1] = VarAdrToInt(p_PP_MACHSET_STUDY_ID);//动作步
        pdata[2] = VarAdrToInt(p_PP_MACHSET_SELF_STUDY_PARA);//学习项目
        pdata[3] = 0x04;//请确认7
        ProtSysCtrl(COMM_SUBCMD_SLOPE_CALIBRATION,4,pdata);
    }
    else if(pwgt == study_message_pg.tabpage1_tab1_btclose || pwgt == study_message_pg.tabpage1_tab2_btclose
            || pwgt == study_message_pg.tabpage1_tab3_btclose || pwgt == study_message_pg.tabpage1_Tab4_btclose
            || pwgt == study_message_pg.tabpage1_Tab5_btclose || pwgt == study_message_pg.tabpage1_Tab6_btclose
            || pwgt == study_message_pg.tabpage1_Tab7_btclose)
    {
        pdata[0] = 0xFF;//开始
        pdata[1] = 0;//动作步
        pdata[2] = 0;//学习项目
        pdata[3] = 0;//第一步
        ProtSysCtrl(COMM_SUBCMD_SLOPE_CALIBRATION,4,pdata);

        VarAdrSetInt(p_PP_MACHSET_STUDY_STEP, 0);//动作步骤
        VarAdrSetInt(p_PP_MACHSET_SELF_STUDY_FLAG, 0);//自学习标志
        VarAdrSetInt(p_PP_MACHSET_STUDY_ID, 0);
        PanelCloseDialogByName(DIALOG_STUDY_MESSAGE);
    }

    return 0;
}
