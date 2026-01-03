#include "WSpec_Func2.h"
#include "typedef.h"
#include "pagefram.h"
#include "database.h"
#include "keymod.h"
#include "key.h"
#include "tabpagewgt.h"
#include "buttonwgt.h"
#include "information.h"
#include "editwgt.h"
#include "kjump.h"
#include "questionDlg.h"//20211221 dyl
#include "pushtab.h"
#include "coreprot.h"
#include "usermod.h"//20210914 dyl touch
#include "protcmd.h"

DEFINE_SPEC_FUNC2_MAP

CONNECT_SPEC_FUNC2_EVENT

#define SPEC_FUNC2_NUM 7 //特殊功能2数量

#define TMP_MIDDLE_CORE_VISIBLE    TMPVAL(101) //途中中子参数是否可见

//information diag //20211221 dyl
#define DIAG_COREPROT_INFORMATION   "CoreProt_Ask"

static BOOL bShowAsk = FALSE;//20230518 chj 中子位置编辑框修改完后，光标无法聚焦在弹出框上
static UI32 wOldValue = 0;

static void ButtonSetStatus(PWGT pwgt, BOOL status)
{
    if(status)
    {
        OBJSetValueByAttrName((POBJ)pwgt,"textColor",GUI_WHITE,0,NULL,ATTR_UINT);
        OBJSetValueByAttrName((POBJ)pwgt,"backColor",0xe1b400,0,NULL,ATTR_UINT);
    }
    else
    {
        OBJSetValueByAttrName((POBJ)pwgt,"textColor",GUI_BLACK,0,NULL,ATTR_UINT);
        OBJSetValueByAttrName((POBJ)pwgt,"backColor",0xFFFFFFFF,0,NULL,ATTR_UINT);
    }
}

//开关模平稳途中中子参数相关
static void ClampcoreVisible(){
    int i =0;
    for(i=0;i<6;i++){
        if(VarAdrToInt(d_coreA_MOLDSET_FL_CORE_CTRL+i*0x100000) &&
           (VarAdrToInt(d_coreA_MOLDSET_ADPOSI_CORE_FWD+i*0x100000) ==1 || VarAdrToInt(d_coreA_MOLDSET_ADPOSI_CORE_BWD+i*0x100000) == 1))
        {
            VarAdrSetInt(TMP_MIDDLE_CORE_VISIBLE, 1);
            break;
        }
    }
}

//设置当前按钮状态
static void SetCurButtonState()
{
    int i;
    PWGT pwgt;
    for(i = 0; i < SPEC_FUNC2_NUM; i++)
    {
        pwgt = PageFramCurWgtByIndex(i+1);
        if(TabPageGetCurSel(spec_func2_pg.tabpage1) == i)
        {
            ButtonSetStatus(pwgt, TRUE);
        }
        else
        {
            ButtonSetStatus(pwgt, FALSE);
        }
    }
}

//子页与按钮显示隐藏
static void TabPageShow()
{
    //启动慢速
    if(VarAdrToInt(SYS_FL_MACH_CODE3) & 0x400)
    {
        TabPageSetSubVisble(spec_func2_pg.tabpage1, 2, TRUE);
        WGTSetVisible(spec_func2_pg.button3, TRUE);
    }
    else
    {
        TabPageSetSubVisble(spec_func2_pg.tabpage1, 2, FALSE);
        WGTSetVisible(spec_func2_pg.button3, FALSE);
    }

    //安全阀
    if(VarAdrToInt(SYS_FL_MACH_CODE5) & 0x4000)
    {
        TabPageSetSubVisble(spec_func2_pg.tabpage1, 3, TRUE);
        WGTSetVisible(spec_func2_pg.button4, TRUE);
        if((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0xB200)
        {
            WGTSetVisible(spec_func2_pg.tabpage1_Tb4_lbl11, TRUE);
            WGTSetVisible(spec_func2_pg.tabpage1_Tb4_ed11, TRUE);
            WGTSetVisible(spec_func2_pg.tabpage1_Tb4_lblunit11, TRUE);
            WGTSetVisible(spec_func2_pg.tabpage1_Tb4_lbl21, TRUE);
            WGTSetVisible(spec_func2_pg.tabpage1_Tb4_ed21, TRUE);
            WGTSetVisible(spec_func2_pg.tabpage1_Tb4_lblunit21, TRUE);
            WGTSetVisible(spec_func2_pg.tabpage1_Tb4_lbl31, FALSE);
            WGTSetVisible(spec_func2_pg.tabpage1_Tb4_ed31, FALSE);
            WGTSetVisible(spec_func2_pg.tabpage1_Tb4_lblunit31, FALSE);
        }
        else
        {
            WGTSetVisible(spec_func2_pg.tabpage1_Tb4_lbl11, FALSE);
            WGTSetVisible(spec_func2_pg.tabpage1_Tb4_ed11, FALSE);
            WGTSetVisible(spec_func2_pg.tabpage1_Tb4_lblunit11, FALSE);
            WGTSetVisible(spec_func2_pg.tabpage1_Tb4_lbl21, FALSE);
            WGTSetVisible(spec_func2_pg.tabpage1_Tb4_ed21, FALSE);
            WGTSetVisible(spec_func2_pg.tabpage1_Tb4_lblunit21, FALSE);
            WGTSetVisible(spec_func2_pg.tabpage1_Tb4_lbl31, TRUE);
            WGTSetVisible(spec_func2_pg.tabpage1_Tb4_ed31, TRUE);
            WGTSetVisible(spec_func2_pg.tabpage1_Tb4_lblunit31, TRUE);
        }
    }
    else
    {
        TabPageSetSubVisble(spec_func2_pg.tabpage1, 3, FALSE);
        WGTSetVisible(spec_func2_pg.button4, FALSE);
    }

    //开关模平稳模式 20241017 chj 增加开关模平稳
    if((VarAdrToInt(p_PP_MACHSET_SWITICHING_MODE_SMOOTH) == 1) && (VarAdrToInt(d_clamp1_MOLDSET_FL_CLSOPEN_MODE) == 0))
    {
        TabPageSetSubVisble(spec_func2_pg.tabpage1, 6, TRUE);
        WGTSetVisible(spec_func2_pg.button7, TRUE);
     }
    else
    {
        TabPageSetSubVisble(spec_func2_pg.tabpage1, 6, FALSE);
        WGTSetVisible(spec_func2_pg.button7, FALSE);
    }

    //液压阀,先预留
    TabPageSetSubVisble(spec_func2_pg.tabpage1, 5, FALSE);
    WGTSetVisible(spec_func2_pg.button6, FALSE);
}

//流动性
static void ActFluidityShow()
{
    if(VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE) == 0)
    {
        WGTSetVisible(spec_func2_pg.tabpage1_Tb2_label234, TRUE);
        WGTSetVisible(spec_func2_pg.tabpage1_Tb2_label235, FALSE);
        WGTSetVisible(spec_func2_pg.tabpage1_Tb2_label236, FALSE);

        WGTSetVisible(spec_func2_pg.tabpage1_Tb2_label237, TRUE);
        WGTSetVisible(spec_func2_pg.tabpage1_Tb2_label238, FALSE);
        WGTSetVisible(spec_func2_pg.tabpage1_Tb2_label239, FALSE);
    }
    else if(VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE) == 1)
    {
        WGTSetVisible(spec_func2_pg.tabpage1_Tb2_label234, FALSE);
        WGTSetVisible(spec_func2_pg.tabpage1_Tb2_label235, TRUE);
        WGTSetVisible(spec_func2_pg.tabpage1_Tb2_label236, FALSE);

        WGTSetVisible(spec_func2_pg.tabpage1_Tb2_label237, FALSE);
        WGTSetVisible(spec_func2_pg.tabpage1_Tb2_label238, TRUE);
        WGTSetVisible(spec_func2_pg.tabpage1_Tb2_label239, FALSE);
    }
    else if(VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE) == 2)
    {
        WGTSetVisible(spec_func2_pg.tabpage1_Tb2_label234, FALSE);
        WGTSetVisible(spec_func2_pg.tabpage1_Tb2_label235, FALSE);
        WGTSetVisible(spec_func2_pg.tabpage1_Tb2_label236, TRUE);

        WGTSetVisible(spec_func2_pg.tabpage1_Tb2_label237, FALSE);
        WGTSetVisible(spec_func2_pg.tabpage1_Tb2_label238, FALSE);
        WGTSetVisible(spec_func2_pg.tabpage1_Tb2_label239, TRUE);
    }
}

static void CtlConnect()
{
}
static int Spec_Func2OnInit(Spec_Func2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Spec_Func2OnShow(Spec_Func2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_USERLOCK);//20210914 dyl touch

    TabPageShow();

    ActFluidityShow();//流动性

    //开关模平稳途中中子参数是否可见
    VarAdrSetInt(TMP_MIDDLE_CORE_VISIBLE,0);
    ClampcoreVisible();

    if(!WGTGetVisible(PageFramCurWgtByIndex(TabPageGetCurSel(ppg->tabpage1)+1)))
    {
        TabPageSelPage(ppg->tabpage1, 0);
    }
    FkeyTab(ppg->tabpage1, GUI_KEY_FUN2);

    bShowAsk = FALSE;//20230518 chj 中子位置编辑框修改完后，光标无法聚焦在弹出框上
    wOldValue = 0;
    return 0;
}
static int Spec_Func2OnHide(Spec_Func2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    bShowAsk = FALSE;//20230518 chj 中子位置编辑框修改完后，光标无法聚焦在弹出框上
    wOldValue = 0;
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch
    return 0;
}
static int Spec_Func2OnUpdate(Spec_Func2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(bShowAsk)//20230518 chj 中子位置编辑框修改完后，光标无法聚焦在弹出框上
    {
        bShowAsk = FALSE;
        ShowDialog_CoreProt(d_autoctrl1_MOLDSET_SYS_COREPOSEFF, wOldValue);
    }
    SetCurButtonState();
    return 0;
}
static int Spec_Func2OnChange(Spec_Func2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 addr,value,offset;
    PATTR pattr;
    int i;

    pattr = (PATTR)msg_para;

    addr = OBJAttrGetAdr(pattr);
    value = VarAdrToInt(addr);

    //20210914 dyl touch 根据索引来定位那个tab子页，兼容触摸和按键
    offset = WGTGetGOffset(pwgt);
    if(offset > 0 && offset <= SPEC_FUNC2_NUM)
    {
        TabPageSelPage(ppg->tabpage1, offset-1);
    }

    switch (addr)
    {
    case d_coreA_ACTPARA_PER_DAFLOW_CORE:
        VarSendSaveSetIntByAdr(d_coreB_ACTPARA_PER_DAFLOW_CORE, value);
        VarSendSaveSetIntByAdr(d_coreC_ACTPARA_PER_DAFLOW_CORE, value);
        VarSendSaveSetIntByAdr(d_coreD_ACTPARA_PER_DAFLOW_CORE, value);
        VarSendSaveSetIntByAdr(d_coreE_ACTPARA_PER_DAFLOW_CORE, value);
        VarSendSaveSetIntByAdr(d_coreF_ACTPARA_PER_DAFLOW_CORE, value);
        break;
    case d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_HIGH:
        if(VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE) == 0)
        {
            if(value != VarAdrToInt(d_inject1_HYDR_SERVO_KP_INJCT))
            {
                VarSendSaveSetIntByAdr(d_inject1_HYDR_SERVO_KP_INJCT, value);
            }
        }
        break;
    case d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MID:
        if(VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE) == 1)
        {
            if(value != VarAdrToInt(d_inject1_HYDR_SERVO_KP_INJCT))
            {
                VarSendSaveSetIntByAdr(d_inject1_HYDR_SERVO_KP_INJCT, value);
            }
        }
        break;
    case d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_LOW:
        if(VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE) == 2)
        {
            if(value != VarAdrToInt(d_inject1_HYDR_SERVO_KP_INJCT))
            {
                VarSendSaveSetIntByAdr(d_inject1_HYDR_SERVO_KP_INJCT, value);
            }
        }
        break;
    case d_autoctrl1_MOLDSET_SYS_SERVO_KI_INJCT_HIGH:
        if(VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE) == 0)
        {
            if(value != VarAdrToInt(d_inject1_HYDR_SERVO_KI_INJCT))
            {
                VarSendSaveSetIntByAdr(d_inject1_HYDR_SERVO_KI_INJCT, value);
            }
        }
        break;
    case d_autoctrl1_MOLDSET_SYS_SERVO_KI_INJCT_MID:
        if(VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE) == 1)
        {
            if(value != VarAdrToInt(d_inject1_HYDR_SERVO_KI_INJCT))
            {
                VarSendSaveSetIntByAdr(d_inject1_HYDR_SERVO_KI_INJCT, value);
            }
        }
        break;
    case d_autoctrl1_MOLDSET_SYS_SERVO_KI_INJCT_LOW:
        if(VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE) == 2)
        {
            if(value != VarAdrToInt(d_inject1_HYDR_SERVO_KI_INJCT))
            {
                VarSendSaveSetIntByAdr(d_inject1_HYDR_SERVO_KI_INJCT, value);
            }
        }
        break;
    case d_autoctrl1_MOLDSET_SYS_SERVO_KP_HOLD_HIGH:
        if(VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE) == 0)
        {
            if(value != VarAdrToInt(d_inject1_HYDR_SERVO_KP_HOLD))
            {
                VarSendSaveSetIntByAdr(d_inject1_HYDR_SERVO_KP_HOLD, value);
            }
        }
        break;
    case d_autoctrl1_MOLDSET_SYS_SERVO_KP_HOLD_MID:
        if(VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE) == 1)
        {
            if(value != VarAdrToInt(d_inject1_HYDR_SERVO_KP_HOLD))
            {
                VarSendSaveSetIntByAdr(d_inject1_HYDR_SERVO_KP_HOLD, value);
            }
        }
        break;
    case d_autoctrl1_MOLDSET_SYS_SERVO_KP_HOLD_LOW:
        if(VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE) == 2)
        {
            if(value != VarAdrToInt(d_inject1_HYDR_SERVO_KP_HOLD))
            {
                VarSendSaveSetIntByAdr(d_inject1_HYDR_SERVO_KP_HOLD, value);
            }
        }
        break;
    case d_autoctrl1_MOLDSET_SYS_SERVO_KI_HOLD_HIGH:
        if(VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE) == 0)
        {
            if(value != VarAdrToInt(d_inject1_HYDR_SERVO_KI_HOLD))
            {
                VarSendSaveSetIntByAdr(d_inject1_HYDR_SERVO_KI_HOLD, value);
            }
        }
        break;
    case d_autoctrl1_MOLDSET_SYS_SERVO_KI_HOLD_MID:
        if(VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE) == 1)
        {
            if(value != VarAdrToInt(d_inject1_HYDR_SERVO_KI_HOLD))
            {
                VarSendSaveSetIntByAdr(d_inject1_HYDR_SERVO_KI_HOLD, value);
            }
        }
        break;
    case d_autoctrl1_MOLDSET_SYS_SERVO_KI_HOLD_LOW:
        if(VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE) == 2)
        {
            if(value != VarAdrToInt(d_inject1_HYDR_SERVO_KI_HOLD))
            {
                VarSendSaveSetIntByAdr(d_inject1_HYDR_SERVO_KI_HOLD, value);
            }
        }
        break;
    //20211221 dyl
    case d_autoctrl1_MOLDSET_SYS_COREPOSEFF:
        if(value==0)
        {
            bShowAsk = TRUE;//20230518 chj 中子位置编辑框修改完后，光标无法聚焦在弹出框上
            wOldValue = msg_value;
            //ShowDialog_CoreProt(addr, msg_value);
        }
        else
        {
            //20210730 dyl//20250416 chj 优化修改开模终止位置后，马上按开模按键，按键值下发卡顿问题 -- 此处一起发送
            for(i=0;i<6;i++)
            {
                if(PushtableType())
                {
                    //20210730 dyl 中子有效区位置
                    //Core In
                    CoreEffeAreaByPushtab_SendAll(d_coreA_ACTPARA_ADPOSI_COREINACT+i*0x100000, d_coreA_MACHSET_ADPOSI_EFFCOREINPLUS+i*0x100000);
                    //Core Out
                    CoreEffeAreaByPushtab_SendAll(d_coreA_ACTPARA_ADPOSI_COREOUTACT+i*0x100000, d_coreA_MACHSET_ADPOSI_EFFCOREOUTPLUS+i*0x100000);
                }
                else
                {
                    VarAdrSetInt(d_coreA_MACHSET_ADPOSI_EFFCOREINPLUS+i*0x100000, value);
                    VarAdrSaveInt(d_coreA_MACHSET_ADPOSI_EFFCOREINPLUS+i*0x100000);
                    VarAdrSetInt(d_coreA_MACHSET_ADPOSI_EFFCOREINMINUS+i*0x100000, value);
                    VarAdrSaveInt(d_coreA_MACHSET_ADPOSI_EFFCOREINMINUS+i*0x100000);
                    VarAdrSetInt(d_coreA_MACHSET_ADPOSI_EFFCOREOUTPLUS+i*0x100000, value);
                    VarAdrSaveInt(d_coreA_MACHSET_ADPOSI_EFFCOREOUTPLUS+i*0x100000);
                    VarAdrSetInt(d_coreA_MACHSET_ADPOSI_EFFCOREOUTMINUS+i*0x100000, value);
                    VarAdrSaveInt(d_coreA_MACHSET_ADPOSI_EFFCOREOUTMINUS+i*0x100000);
                }
                //20250416 chj 优化修改开模终止位置后，马上按开模按键，按键值下发卡顿问题 -- 此处一起发送
                UI16 pdata2[4];
                UI32 startaddr = d_coreA_MACHSET_ADPOSI_EFFCOREINPLUS+0x100000*i;
                UI16 ppart = (startaddr>>16)&0xFFF0;
                UI8 subid = (startaddr>>16)&0xf;
                UI16 offset = startaddr&0xFFFF;
                pdata2[0] = VarAdrToUI16((d_coreA_MACHSET_ADPOSI_EFFCOREINPLUS+0x100000*i));
                pdata2[1] = VarAdrToUI16((d_coreA_MACHSET_ADPOSI_EFFCOREINMINUS+0x100000*i));
                pdata2[2] = VarAdrToUI16((d_coreA_MACHSET_ADPOSI_EFFCOREOUTPLUS+0x100000*i));
                pdata2[3] = VarAdrToUI16((d_coreA_MACHSET_ADPOSI_EFFCOREOUTMINUS+0x100000*i));
                ProtSendPartSubData(ppart,subid,offset,4,pdata2);
            }
        }
        break;
    case d_coreA_MOLDSET_CORE_LIMIT_CHECK:
        if(VarAdrToUI16(OPMODE_INDEX) != MODE_MANUAL)
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//原始值
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }
        if(value==0)
        {
            ShowDialog_CoreProt(addr, msg_value);
        }
        else
        {
            VarSendSaveSetIntByAdr(d_coreB_MOLDSET_CORE_LIMIT_CHECK,value);
            VarSendSaveSetIntByAdr(d_coreC_MOLDSET_CORE_LIMIT_CHECK,value);
            VarSendSaveSetIntByAdr(d_coreD_MOLDSET_CORE_LIMIT_CHECK,value);
            VarSendSaveSetIntByAdr(d_coreE_MOLDSET_CORE_LIMIT_CHECK,value);
            VarSendSaveSetIntByAdr(d_coreF_MOLDSET_CORE_LIMIT_CHECK,value);
        }
        break;
    case d_coreA_MOLDSET_CLAMP_CORE_CHECK:
        if(VarAdrToUI16(OPMODE_INDEX) != MODE_MANUAL)
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//原始值
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }
        if(value==0)
        {
            ShowDialog_CoreProt(addr, msg_value);
        }
        else
        {
            VarSendSaveSetIntByAdr(d_coreB_MOLDSET_CLAMP_CORE_CHECK,value);
            VarSendSaveSetIntByAdr(d_coreC_MOLDSET_CLAMP_CORE_CHECK,value);
            VarSendSaveSetIntByAdr(d_coreD_MOLDSET_CLAMP_CORE_CHECK,value);
            VarSendSaveSetIntByAdr(d_coreE_MOLDSET_CLAMP_CORE_CHECK,value);
            VarSendSaveSetIntByAdr(d_coreF_MOLDSET_CLAMP_CORE_CHECK,value);
        }
        break;
    default :
        break;
    }
    return 0;
}
