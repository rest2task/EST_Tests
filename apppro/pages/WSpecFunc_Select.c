#include "WSpecFunc_Select.h"
#include "typedef.h"
#include "pagefram.h"
#include "database.h"
#include "keymod.h"
#include "key.h"
#include "tabpagewgt.h"
#include "questionDlg.h"
#include "command.h"
#include "privdef.h"
#include "usermod.h"//20210914 dyl touch

DEFINE_SPECFUNC_SELECT_MAP

CONNECT_SPECFUNC_SELECT_EVENT

#define TMP_FUNC_FLOAT_CYLIND   TMPVAL(0) //浮动缸
#define TMP_FUNC_INJ_BEF_NOZADV TMPVAL(1) //座进前射出
#define TMP_FUNC_OPEN_LOOP      TMPVAL(2) //动作开环
#define TMP_FUNC_COMPRESS       TMPVAL(3) //压塑
#define TMP_FUNC_EXHAUST        TMPVAL(4) //排气
#define TMP_FUNC_BAFFLE         TMPVAL(5) //挡板
#define TMP_FUNC_BMC            TMPVAL(6) //BMC
#define TMP_FUNC_ACTSTARTSLOW   TMPVAL(7) //启动慢速
#define TMP_FUNC_SPCCORERAMP    TMPVAL(8) //特殊中子斜率
#define TMP_FUNC_CLAMPLINK      TMPVAL(9) //开关模联动
#define TMP_FUNC_REMELT         TMPVAL(10)//再次储料
#define TMP_FUNC_SAFETYVLV      TMPVAL(11)//安全阀

static QUESTION g_question;

static int switch_key=0;

//保存功能code码
static void SaveFuncCode()
{
    //浮动缸
    if(VarAdrToInt(TMP_FUNC_FLOAT_CYLIND))
    {
        VarAdrSetInt(SYS_FL_MACH_CODE48, VarAdrToInt(SYS_FL_MACH_CODE48)|0x2000);
    }
    else
    {
        VarAdrSetInt(SYS_FL_MACH_CODE48, VarAdrToInt(SYS_FL_MACH_CODE48)&(~0x2000));
    }
    //座进前射出
    if(VarAdrToInt(TMP_FUNC_INJ_BEF_NOZADV))
    {
        VarAdrSetInt(SYS_FL_MACH_CODE4, VarAdrToInt(SYS_FL_MACH_CODE4)|0x4000);
    }
    else
    {
        VarAdrSetInt(SYS_FL_MACH_CODE4, VarAdrToInt(SYS_FL_MACH_CODE4)&(~0x4000));
    }
    //动作开环
    if(VarAdrToInt(TMP_FUNC_OPEN_LOOP))
    {
        VarAdrSetInt(SYS_FL_MACH_CODE5, VarAdrToInt(SYS_FL_MACH_CODE5)|0x0080);
    }
    else
    {
        VarAdrSetInt(SYS_FL_MACH_CODE5, VarAdrToInt(SYS_FL_MACH_CODE5)&(~0x0080));
    }
    //压塑
    if(VarAdrToInt(TMP_FUNC_COMPRESS))
    {
        VarAdrSetInt(SYS_FL_MACH_CODE2, VarAdrToInt(SYS_FL_MACH_CODE2)|0x0004);
    }
    else
    {
        VarAdrSetInt(SYS_FL_MACH_CODE2, VarAdrToInt(SYS_FL_MACH_CODE2)&(~0x0004));
    }
    //排气
    if(VarAdrToInt(TMP_FUNC_EXHAUST))
    {
        VarAdrSetInt(SYS_FL_MACH_CODE3, VarAdrToInt(SYS_FL_MACH_CODE3)|0x0100);
    }
    else
    {
        VarAdrSetInt(SYS_FL_MACH_CODE3, VarAdrToInt(SYS_FL_MACH_CODE3)&(~0x0100));
    }
    //挡板
    if(VarAdrToInt(TMP_FUNC_BAFFLE))
    {
        VarAdrSetInt(SYS_FL_MACH_CODE3, VarAdrToInt(SYS_FL_MACH_CODE3)|0x0001);//挡板code4+8000改为code3+1 20250310 jhh
    }
    else
    {
        VarAdrSetInt(SYS_FL_MACH_CODE3, VarAdrToInt(SYS_FL_MACH_CODE3)&(~0x0001));
    }
    //BMC
    if(VarAdrToInt(TMP_FUNC_BMC))
    {
        VarAdrSetInt(SYS_FL_MACH_CODE0, VarAdrToInt(SYS_FL_MACH_CODE0)|0x0002);
    }
    else
    {
        VarAdrSetInt(SYS_FL_MACH_CODE0, VarAdrToInt(SYS_FL_MACH_CODE0)&(~0x0002));
    }
    //启动慢速
    if(VarAdrToInt(TMP_FUNC_ACTSTARTSLOW))
    {
        VarAdrSetInt(SYS_FL_MACH_CODE3, VarAdrToInt(SYS_FL_MACH_CODE3)|0x0400);
    }
    else
    {
        VarAdrSetInt(SYS_FL_MACH_CODE3, VarAdrToInt(SYS_FL_MACH_CODE3)&(~0x0400));
    }
    //特殊中子斜率
    if(VarAdrToInt(TMP_FUNC_SPCCORERAMP))
    {
        VarSendSaveSetIntByAdr(d_clamp1_MOLDSET_FL_CLAMP_CORE_RAMP, 1);
    }
    else
    {
        VarSendSaveSetIntByAdr(d_clamp1_MOLDSET_FL_CLAMP_CORE_RAMP, 0);
    }
    //开关模联动
    if(VarAdrToInt(TMP_FUNC_CLAMPLINK))
    {
        VarAdrSetInt(SYS_FL_MACH_CODE1, VarAdrToInt(SYS_FL_MACH_CODE1)|0x2000);
    }
    else
    {
        VarAdrSetInt(SYS_FL_MACH_CODE1, VarAdrToInt(SYS_FL_MACH_CODE1)&(~0x2000));
    }
    //再次储料
    if(VarAdrToInt(TMP_FUNC_REMELT))
    {
        VarAdrSetInt(SYS_FL_MACH_CODE54, VarAdrToInt(SYS_FL_MACH_CODE54)|0x0080);
    }
    else
    {
        VarAdrSetInt(SYS_FL_MACH_CODE54, VarAdrToInt(SYS_FL_MACH_CODE54)&(~0x0080));
    }
    //安全阀
    if(VarAdrToInt(TMP_FUNC_SAFETYVLV))
    {
        VarAdrSetInt(SYS_FL_MACH_CODE5, VarAdrToInt(SYS_FL_MACH_CODE5)|0x4000);
    }
    else
    {
        VarAdrSetInt(SYS_FL_MACH_CODE5, VarAdrToInt(SYS_FL_MACH_CODE5)&(~0x4000));
    }

    CodeSendSave();
}

/************************************************************************/
/* 切页面信息提示框“保存”按钮                                         */
/************************************************************************/
static void SaveButton()
{
    if(VarAdrToUI16(OPMODE_INDEX) == MODE_MANUAL)//20200109.cyx 只有手动状态下才能保存
    {
        SaveFuncCode();
    }

    VarAdrSetInt(p_PP_MACHSET_SPECFUNC_SELECT_CHANGE_FLAG,0);//数据有修改标记还原
    //返回物理按键
    if(switch_key>=GUI_KEY_FUN1 && switch_key<=GUI_KEY_FUN6)
    {
        switch_key = (switch_key-GUI_KEY_FUN1)+ KEY_FUN1;
    }
    else if(switch_key ==GUI_KEY_FUN7)
    {
        switch_key = KEY_FUN7;
    }
    else if(switch_key == GUI_KEY_MAINPAGE)
    {
        switch_key = KEY_MAIN;
    }
    else if(switch_key == GUI_KEY_FUN8)
    {
        switch_key = KEY_PC;
    }
    else if(switch_key == GUI_KEY_NEXTPAGE)//20191126.cyx
    {
        switch_key = KEY_NEXTPAGE;
    }
    SendKey(switch_key);
}
/************************************************************************/
/*  切页面信息提示框“取消”按钮                                        */
/************************************************************************/
static void CancelButton()
{
    VarAdrSetInt(p_PP_MACHSET_SPECFUNC_SELECT_CHANGE_FLAG,0);//数据有修改标记还原
    if(switch_key>=GUI_KEY_FUN1 && switch_key<=GUI_KEY_FUN6) //F1-F6
    {
        switch_key = (switch_key-GUI_KEY_FUN1)+ KEY_FUN1;
    }
    else if(switch_key ==GUI_KEY_FUN7)  //F7
    {
        switch_key = KEY_FUN7;
    }
    else if(switch_key == GUI_KEY_MAINPAGE)
    {
        switch_key = KEY_MAIN;
    }
    else if(switch_key == GUI_KEY_FUN8) //PC
    {
        switch_key = KEY_PC;
    }
    else if(switch_key == GUI_KEY_NEXTPAGE)//20191126.cyx
    {
        switch_key = KEY_NEXTPAGE;
    }
    SendKey(switch_key);
}
/************************************************************************/
/* 配置修改回调函数                                                     */
/************************************************************************/
static void SpecFuncChange(int key,int press)
{
    if(press)
    {
        g_question.pOkfunc = SaveButton;
        g_question.pCancelfunc = CancelButton;
        switch_key = key;

        //20200109.cyx 非手动，自动登出时默认不保存，不弹框
        if((VarAdrToUI16(OPMODE_INDEX) != MODE_MANUAL) || VarAdrToInt(USER_STATE))
        {
            CancelButton();
        }
        else
        {
            ShowQuestion(VW_MSG_SAVEDATA,g_question);
        }
    }
}

static void CtlConnect()
{
}
static int SpecFunc_SelectOnInit(SpecFunc_Select_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetSpecFuncSelectCB(SpecFuncChange);
    VarAdrSetInt(p_PP_MACHSET_SPECFUNC_SELECT_CHANGE_FLAG,0);
    return 0;
}
static int SpecFunc_SelectOnShow(SpecFunc_Select_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_USER_MODELOCK);//20210914 dyl touch

    //浮动缸
    if(VarAdrToInt(SYS_FL_MACH_CODE48) & 0x2000)
    {
        VarAdrSetInt(TMP_FUNC_FLOAT_CYLIND, TRUE);
    }
    else
    {
        VarAdrSetInt(TMP_FUNC_FLOAT_CYLIND, FALSE);
    }
    //座进前射出
    if(VarAdrToInt(SYS_FL_MACH_CODE4) & 0x4000)
    {
        VarAdrSetInt(TMP_FUNC_INJ_BEF_NOZADV, TRUE);
    }
    else
    {
        VarAdrSetInt(TMP_FUNC_INJ_BEF_NOZADV, FALSE);
    }
    //动作开环
    if(VarAdrToInt(SYS_FL_MACH_CODE5) & 0x80)
    {
        VarAdrSetInt(TMP_FUNC_OPEN_LOOP, TRUE);
    }
    else
    {
        VarAdrSetInt(TMP_FUNC_OPEN_LOOP, FALSE);
    }
    //压塑
    if(VarAdrToInt(SYS_FL_MACH_CODE2) & 0x4)
    {
        VarAdrSetInt(TMP_FUNC_COMPRESS, TRUE);
    }
    else
    {
        VarAdrSetInt(TMP_FUNC_COMPRESS, FALSE);
    }
    //排气
    if(VarAdrToInt(SYS_FL_MACH_CODE3) & 0x100)
    {
        VarAdrSetInt(TMP_FUNC_EXHAUST, TRUE);
    }
    else
    {
        VarAdrSetInt(TMP_FUNC_EXHAUST, FALSE);
    }
    //挡板
    if(VarAdrToInt(SYS_FL_MACH_CODE3) & 0x0001)//挡板code4+8000改为code3+1 20250310 jhh
    {
        VarAdrSetInt(TMP_FUNC_BAFFLE, TRUE);
    }
    else
    {
        VarAdrSetInt(TMP_FUNC_BAFFLE, FALSE);
    }
    //BMC
    if(VarAdrToInt(SYS_FL_MACH_CODE0) & 0x2)
    {
        VarAdrSetInt(TMP_FUNC_BMC, TRUE);
    }
    else
    {
        VarAdrSetInt(TMP_FUNC_BMC, FALSE);
    }
    //启动慢速
    if(VarAdrToInt(SYS_FL_MACH_CODE3) & 0x400)
    {
        VarAdrSetInt(TMP_FUNC_ACTSTARTSLOW, TRUE);
    }
    else
    {
        VarAdrSetInt(TMP_FUNC_ACTSTARTSLOW, FALSE);
    }
    //特殊中子斜率
    if(VarAdrToInt(d_clamp1_MOLDSET_FL_CLAMP_CORE_RAMP))
    {
        VarAdrSetInt(TMP_FUNC_SPCCORERAMP, TRUE);
    }
    else
    {
        VarAdrSetInt(TMP_FUNC_SPCCORERAMP, FALSE);
    }
    //开关模联动
    if(VarAdrToInt(SYS_FL_MACH_CODE1) & 0x2000)
    {
        VarAdrSetInt(TMP_FUNC_CLAMPLINK, TRUE);
    }
    else
    {
        VarAdrSetInt(TMP_FUNC_CLAMPLINK, FALSE);
    }
    //再次储料
    if(VarAdrToInt(SYS_FL_MACH_CODE54) & 0x0080)
    {
        VarAdrSetInt(TMP_FUNC_REMELT, TRUE);
    }
    else
    {
        VarAdrSetInt(TMP_FUNC_REMELT, FALSE);
    }
    //安全阀
    if(VarAdrToInt(SYS_FL_MACH_CODE5) & 0x4000)
    {
        VarAdrSetInt(TMP_FUNC_SAFETYVLV, TRUE);
    }
    else
    {
        VarAdrSetInt(TMP_FUNC_SAFETYVLV, FALSE);
    }

    TabPageSelPage(ppg->tabpage1, 0);
    FkeyTab(ppg->tabpage1, GUI_KEY_FUN3);
    return 0;
}
static int SpecFunc_SelectOnHide(SpecFunc_Select_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch
    return 0;
}
static int SpecFunc_SelectOnUpdate(SpecFunc_Select_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageProtect();//20210914 dyl touch //PAGE_PROTECT

    return 0;
}
static int SpecFunc_SelectOnChange(SpecFunc_Select_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 index;

    index = WGTGetGOffset(pwgt);

    if(index)
    {
        VarAdrSetInt(p_PP_MACHSET_SPECFUNC_SELECT_CHANGE_FLAG,1);
    }

    if(pwgt == ppg->tabpage1_Tb1_btsave)
    {
        VarAdrSetInt(p_PP_MACHSET_SPECFUNC_SELECT_CHANGE_FLAG,0);
        SaveFuncCode();
    }

    return 0;
}
