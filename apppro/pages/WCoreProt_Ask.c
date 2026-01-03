#include "WCoreProt_Ask.h"
#include "typedef.h"
#include "panel.h"
#include "questionDlg.h"
#include "dbenum.h"
#include "vardb.h"

DEFINE_COREPROT_ASK_MAP

CONNECT_COREPROT_ASK_EVENT

#define CORENUM 6
static UI32 local_addr = -1;
static UI16 local_value = -1;
static BOOL bClickButton = FALSE;//20220928 dyl 记录弹框出来后选择过取消/确定按键，防止没有按按键直接切画面

static int OKOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int CancelOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static void CtlConnect()
{
    WgtMsgConnect(coreprot_ask_pg.Cancel, WGT_CLICK_MSG, CancelOnClick);
    WgtMsgConnect(coreprot_ask_pg.OK, WGT_CLICK_MSG, OKOnClick);
}

void ShowDialog_CoreProt(UI32 changed_Addr, UI16 old_value)
{
    local_addr = changed_Addr;
    local_value = old_value;
    bClickButton = FALSE;//20220928 dyl 记录弹框出来后选择过取消/确定按键，防止没有按按键直接切画面
    PanelShowDialogByName(coreprot_ask_pg_name);
}

static int CoreProt_AskOnInit(CoreProt_Ask_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int CoreProt_AskOnShow(CoreProt_Ask_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int CoreProt_AskOnHide(CoreProt_Ask_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    //20220928 dyl 记录弹框出来后选择过取消/确定按键，防止没有按按键直接切画面
    if(bClickButton == FALSE)
    {
        if(local_addr==d_autoctrl1_MOLDSET_SYS_COREPOSEFF)
        {
            VarSendSaveSetIntByAdr(d_autoctrl1_MOLDSET_SYS_COREPOSEFF,local_value);
        }
        else if(local_addr==d_coreA_MOLDSET_CORE_LIMIT_CHECK)
        {
            VarSendSaveSetIntByAdr(d_coreA_MOLDSET_CORE_LIMIT_CHECK,local_value);
        }
        else if(local_addr==d_coreA_MOLDSET_CLAMP_CORE_CHECK)
        {
            VarSendSaveSetIntByAdr(d_coreA_MOLDSET_CLAMP_CORE_CHECK,local_value);
        }
    }
    return 0;
}
static int CoreProt_AskOnUpdate(CoreProt_Ask_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int CoreProt_AskOnChange(CoreProt_Ask_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(pwgt == PageGetWgtByName(coreprot_ask_pg_name,"btnclose"))
    {

        PanelCloseDialogByName(coreprot_ask_pg_name);//20191223.cyx
    }
    bClickButton = TRUE;//20220928 dyl 记录弹框出来后选择过取消/确定按键，防止没有按按键直接切画面
    return 0;
}
static int OKOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    int i;
    if(local_addr==d_autoctrl1_MOLDSET_SYS_COREPOSEFF)
    {
        for(i=0;i<CORENUM;i++)
        {
            VarSendSaveSetIntByAdr(d_coreA_MACHSET_ADPOSI_EFFCOREINPLUS+i*0x100000,0);
            VarSendSaveSetIntByAdr(d_coreA_MACHSET_ADPOSI_EFFCOREINMINUS+i*0x100000,0);
            VarSendSaveSetIntByAdr(d_coreA_MACHSET_ADPOSI_EFFCOREOUTPLUS+i*0x100000,0);
            VarSendSaveSetIntByAdr(d_coreA_MACHSET_ADPOSI_EFFCOREOUTMINUS+i*0x100000,0);
        }
    }
    else if(local_addr==d_coreA_MOLDSET_CORE_LIMIT_CHECK)//20230518 chj 1->0
    {
        VarSendSaveSetIntByAdr(d_coreB_MOLDSET_CORE_LIMIT_CHECK,0);
        VarSendSaveSetIntByAdr(d_coreC_MOLDSET_CORE_LIMIT_CHECK,0);
        VarSendSaveSetIntByAdr(d_coreD_MOLDSET_CORE_LIMIT_CHECK,0);
        VarSendSaveSetIntByAdr(d_coreE_MOLDSET_CORE_LIMIT_CHECK,0);
        VarSendSaveSetIntByAdr(d_coreF_MOLDSET_CORE_LIMIT_CHECK,0);
    }
    else if(local_addr==d_coreA_MOLDSET_CLAMP_CORE_CHECK)//20230518 chj 1->0
    {
        VarSendSaveSetIntByAdr(d_coreB_MOLDSET_CLAMP_CORE_CHECK,0);
        VarSendSaveSetIntByAdr(d_coreC_MOLDSET_CLAMP_CORE_CHECK,0);
        VarSendSaveSetIntByAdr(d_coreD_MOLDSET_CLAMP_CORE_CHECK,0);
        VarSendSaveSetIntByAdr(d_coreE_MOLDSET_CLAMP_CORE_CHECK,0);
        VarSendSaveSetIntByAdr(d_coreF_MOLDSET_CLAMP_CORE_CHECK,0);
    }
    bClickButton = TRUE;//20220928 dyl 记录弹框出来后选择过取消/确定按键，防止没有按按键直接切画面

    PanelCloseDialogByName(coreprot_ask_pg_name);

    return 0;
}
static int CancelOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(local_addr==d_autoctrl1_MOLDSET_SYS_COREPOSEFF)
    {
        VarSendSaveSetIntByAdr(d_autoctrl1_MOLDSET_SYS_COREPOSEFF,local_value);
    }
    else if(local_addr==d_coreA_MOLDSET_CORE_LIMIT_CHECK)
    {
        VarSendSaveSetIntByAdr(d_coreA_MOLDSET_CORE_LIMIT_CHECK,local_value);
    }
    else if(local_addr==d_coreA_MOLDSET_CLAMP_CORE_CHECK)
    {
        VarSendSaveSetIntByAdr(d_coreA_MOLDSET_CLAMP_CORE_CHECK,local_value);
    }
    bClickButton = TRUE;//20220928 dyl 记录弹框出来后选择过取消/确定按键，防止没有按按键直接切画面
    PanelCloseDialogByName(coreprot_ask_pg_name);

    return 0;
}
