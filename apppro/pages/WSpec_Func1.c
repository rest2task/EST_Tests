#include "WSpec_Func1.h"
#include "typedef.h"
#include "pagefram.h"
#include "database.h"
#include "keymod.h"
#include "key.h"
#include "tabpagewgt.h"
#include "buttonwgt.h"
#include "information.h"
#include "editwgt.h"
#include "pushtab.h"
#include "kjump.h"
#include "command.h"
#include "coreprot.h"
#include "usermod.h"//20210914 dyl touch

DEFINE_SPEC_FUNC1_MAP

CONNECT_SPEC_FUNC1_EVENT

#define SPEC_FUNC1_NUM 18 //特殊功能数量

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

//设置当前按钮状态
static void SetCurButtonState()
{
    int i;
    PWGT pwgt;
    for(i = 0; i < SPEC_FUNC1_NUM; i++)
    {
        pwgt = PageFramCurWgtByIndex(i+1);
        if(TabPageGetCurSel(spec_func1_pg.tabpage1) == i)
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
    //浮动缸
    if(VarAdrToInt(SYS_FL_MACH_CODE48) & 0x2000)
    {
        TabPageSetSubVisble(spec_func1_pg.tabpage1, 1, TRUE);
        WGTSetVisible(spec_func1_pg.button2, TRUE);
    }
    else
    {
        TabPageSetSubVisble(spec_func1_pg.tabpage1, 1, FALSE);
        WGTSetVisible(spec_func1_pg.button2, FALSE);
    }
    //开关模联动
    if(VarAdrToInt(SYS_FL_MACH_CODE1) & 0x2000)
    {
        TabPageSetSubVisble(spec_func1_pg.tabpage1, 3, TRUE);
        WGTSetVisible(spec_func1_pg.button4, TRUE);
    }
    else
    {
        TabPageSetSubVisble(spec_func1_pg.tabpage1, 3, FALSE);
        WGTSetVisible(spec_func1_pg.button4, FALSE);
    }
    //座进前射出
    if(VarAdrToInt(SYS_FL_MACH_CODE4) & 0x4000)
    {
        TabPageSetSubVisble(spec_func1_pg.tabpage1, 6, TRUE);
        WGTSetVisible(spec_func1_pg.button7, TRUE);
    }
    else
    {
        TabPageSetSubVisble(spec_func1_pg.tabpage1, 6, FALSE);
        WGTSetVisible(spec_func1_pg.button7, FALSE);
    }
    //动作开环
    if(VarAdrToInt(SYS_FL_MACH_CODE5) & 0x80)
    {
        TabPageSetSubVisble(spec_func1_pg.tabpage1, 7, TRUE);
        WGTSetVisible(spec_func1_pg.button8, TRUE);
    }
    else
    {
        TabPageSetSubVisble(spec_func1_pg.tabpage1, 7, FALSE);
        WGTSetVisible(spec_func1_pg.button8, FALSE);
    }
    //压塑
    if(VarAdrToInt(SYS_FL_MACH_CODE2) & 0x4)
    {
        TabPageSetSubVisble(spec_func1_pg.tabpage1, 8, TRUE);
        WGTSetVisible(spec_func1_pg.button9, TRUE);
    }
    else
    {
        TabPageSetSubVisble(spec_func1_pg.tabpage1, 8, FALSE);
        WGTSetVisible(spec_func1_pg.button9, FALSE);
    }
    //排气
    if(VarAdrToInt(SYS_FL_MACH_CODE3) & 0x100)
    {
        TabPageSetSubVisble(spec_func1_pg.tabpage1, 9, TRUE);
        WGTSetVisible(spec_func1_pg.button10, TRUE);
    }
    else
    {
        TabPageSetSubVisble(spec_func1_pg.tabpage1, 9, FALSE);
        WGTSetVisible(spec_func1_pg.button10, FALSE);
    }
    //挡板
    if(VarAdrToInt(SYS_FL_MACH_CODE3) & 0x0001)//挡板code4+8000改为code3+1 20250310 jhh
    {
        TabPageSetSubVisble(spec_func1_pg.tabpage1, 10, TRUE);
        WGTSetVisible(spec_func1_pg.button11, TRUE);
    }
    else
    {
        TabPageSetSubVisble(spec_func1_pg.tabpage1, 10, FALSE);
        WGTSetVisible(spec_func1_pg.button11, FALSE);
    }
    //BMC
    if(VarAdrToInt(SYS_FL_MACH_CODE0) & 0x2)
    {
        TabPageSetSubVisble(spec_func1_pg.tabpage1, 11, TRUE);
        WGTSetVisible(spec_func1_pg.button12, TRUE);
    }
    else
    {
        TabPageSetSubVisble(spec_func1_pg.tabpage1, 11, FALSE);
        WGTSetVisible(spec_func1_pg.button12, FALSE);
    }
    //特殊中子斜率
    if(VarAdrToInt(d_clamp1_MOLDSET_FL_CLAMP_CORE_RAMP))
    {
        TabPageSetSubVisble(spec_func1_pg.tabpage1, 16, TRUE);
        WGTSetVisible(spec_func1_pg.button17, TRUE);
    }
    else
    {
        TabPageSetSubVisble(spec_func1_pg.tabpage1, 16, FALSE);
        WGTSetVisible(spec_func1_pg.button17, FALSE);
    }
    //再次储料
    if(VarAdrToInt(SYS_FL_MACH_CODE54) & 0x0080)
    {
        TabPageSetSubVisble(spec_func1_pg.tabpage1, 17, TRUE);
        WGTSetVisible(spec_func1_pg.button18, TRUE);
    }
    else
    {
        TabPageSetSubVisble(spec_func1_pg.tabpage1, 17, FALSE);
        WGTSetVisible(spec_func1_pg.button18, FALSE);
    }
    //20211227 dyl
    if ((VarAdrToInt(d_inject1_MOLDSET_ADPOSI_CHGFLOWMOLD)==0)&&(VarAdrToInt(d_inject1_MOLDSET_TM_CHGFLOWMOLDING)==0))
    {
        VarSendSaveSetIntByAdr(p_PP_MACHSET_REMELT_FUNC,0);
    }
    else
    {
        if(VarAdrToInt(d_machine1_MACHSET_FL_MACHINTERNAL6)&0x0080){
            if (VarAdrToInt(d_inject1_MOLDSET_TM_CHGFLOWMOLDING)!=0)
            {
                VarSendSaveSetIntByAdr(p_PP_MACHSET_REMELT_FUNC,1);
            }
            else if (VarAdrToInt(d_inject1_MOLDSET_ADPOSI_CHGFLOWMOLD)!=0)
            {
                VarSendSaveSetIntByAdr(p_PP_MACHSET_REMELT_FUNC,2);
            }
        }
        else{
            VarSendSaveSetIntByAdr(p_PP_MACHSET_REMELT_FUNC,0);
        }
    }
}

//蓄能器
static void SetNozzlePosValueAndShow()
{
    UI16 channl = VarAdrToInt(d_injstor1_MACHSET_INJSTOR_METERCH);

//    if(IV5200 || IV5300)
    if (g_mainresource_current.nADNum == 8)//20220718 dyl 当3101/3202等使用6路电子尺时的电子尺通道选择
    {
        if(channl>=4 && channl<=7)
        {
            WGTSetAttrByName(spec_func1_pg.tabpage1_Tb6_dtacc,"text",d_admeter1_STATE_ADPOSI_RELATIVE_4+(channl-4));
        }
    }
//    else if (IV3100 && ((VarAdrToUI16(d_machine1_VERSION_model_and_cat)&0x1FFF)==3101))//20220120 dyl 电子尺通道统一处理
//    {
//        if(channl == 7)
//        {
//            WGTSetAttrByName(spec_func1_pg.tabpage1_Tb6_dtacc,"text",d_admeter1_STATE_ADPOSI_RELATIVE_EXT1);
//        }
//        else if((channl>=4)&&(channl<=6))
//        {
//            WGTSetAttrByName(spec_func1_pg.tabpage1_Tb6_dtacc,"text",d_admeter1_STATE_ADPOSI_RELATIVE_4+(channl-4));
//        }
//    }
    else if (g_mainresource_current.nADNum == 6)//20220718 dyl 当3101/3202等使用6路电子尺时的电子尺通道选择
    {
        if(channl == 7)
        {
            WGTSetAttrByName(spec_func1_pg.tabpage1_Tb6_dtacc,"text",d_admeter1_STATE_ADPOSI_RELATIVE_EXT1);
        }
        else if((channl>=4)&&(channl<=6))
        {
            WGTSetAttrByName(spec_func1_pg.tabpage1_Tb6_dtacc,"text",d_admeter1_STATE_ADPOSI_RELATIVE_4+(channl-4));
        }
    }
    else
    {
        if(channl==4)
        {
            WGTSetAttrByName(spec_func1_pg.tabpage1_Tb6_dtacc,"text",d_admeter1_STATE_ADPOSI_RELATIVE_4);
        }
        else if(channl>=5 && channl<=7)
        {
            WGTSetAttrByName(spec_func1_pg.tabpage1_Tb6_dtacc,"text",d_admeter1_STATE_ADPOSI_RELATIVE_EXT1+(channl-5));//20220120 dyl
        }
    }
}

//检查电子尺通道是否已使用
static BOOL CheckChannelUsed(UI32 value)
{
    int i;

    if(VarAdrToInt(SYS_FL_METER) & 0x0004)
    {
        if(value == VarAdrToInt(d_nozzle1_MACHSET_NZL_METERCH))
        {
            return TRUE;
        }
    }

    //20220923 dyl ECAT模式下才判断蓄能器压力检测通道号
    if(VarAdrToUI16(SYS_FL_MACH_CODE1)&0x04)
    {
        for(i = 0; i < VarAdrToInt(SERVO_CURRENT_NUM); i++)
        {
            if(value == VarAdrToInt(d_Pump1_MACHSET_PRESSAD_CH + i*0x100000))
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

//压塑前射出一段计时超过最大值则赋最大值
static void SetInjMoldInTime()
{
    UI32 maxtime = VarAdrToInt(p_PP_OBJ_INJ_1_TM_BEF_COMPRS_MAX);
    if(VarAdrToInt(d_inject1_MOLDSET_TM_INJMOLDINTIME) > maxtime)
    {
        VarSendSaveSetIntByAdr(d_inject1_MOLDSET_TM_INJMOLDINTIME,maxtime);
    }
}

//20241214 chj 针阀提前关时间选项变灰设置
static void SetNeedVlvEarlyOffTmVis()
{
    if(VarAdrToInt(d_inject1_MACHSET_FL_INj_SPC) && (VarAdrToInt(d_inject1_MACHSET_INJ_SPC_FUNC_CLS) == 0))
    {
        WGTSetEnable(spec_func1_pg.tabpage1_Tb13_labelNdlOffTm_unit, TRUE);
        WGTSetEnable(spec_func1_pg.tabpage1_Tb13_edNdlOffTm, TRUE);
    }
    else {
        WGTSetEnable(spec_func1_pg.tabpage1_Tb13_labelNdlOffTm_unit, FALSE);
        WGTSetEnable(spec_func1_pg.tabpage1_Tb13_edNdlOffTm, FALSE);
    }
}

//20250109 fqh 针阀2提前关时间选项变灰设置
static void SetNeedVlv2EarlyOffTmVis()
{
    if(VarAdrToInt(d_inject1_MACHSET_FL_INj_SPC2) && (VarAdrToInt(d_inject1_MACHSET_INJ_SPC_FUNC_CLS2) == 0))
    {
        WGTSetEnable(spec_func1_pg.tabpage1_Tb13_labelNdlOffTm_unit2, TRUE);
        WGTSetEnable(spec_func1_pg.tabpage1_Tb13_edNdlOffTm2, TRUE);
    }
    else {
        WGTSetEnable(spec_func1_pg.tabpage1_Tb13_labelNdlOffTm_unit2, FALSE);
        WGTSetEnable(spec_func1_pg.tabpage1_Tb13_edNdlOffTm2, FALSE);
    }
}

//20241205 chj 胶口针阀页逻辑 使用针阀同步输出吹气时，此吹气不可用--将动作时间设置为 0
static void Inj_SPC_Func()
{
    UI16 tmpval1=0,tmpval2=0;
    tmpval1 = VarAdrToInt(d_inject1_MACHSET_FL_INJSPC_AIR);
    tmpval2 = VarAdrToInt(d_inject1_MACHSET_FL_INJSPC_AIR1);
    //oprintf("tmpval1tmpval1tmpval1---%d--%d\r\n",tmpval1,tmpval2);
    if(tmpval1 !=0 )
    {
        VarSendSaveSetIntByAdr(FL_BLAST1 + 0x100000*(tmpval1-1),256);
    }
    if(tmpval2 !=0 )
    {
        VarSendSaveSetIntByAdr(FL_BLAST1 + 0x100000*(tmpval2-1),256);
    }
}

//胶木机 20241205 chj 排气压力，速度，只有在金鹰厂商下才显示
static void SetExhaustPrsSpdShow()
{
    if(((VarAdrToInt(d_machine1_MACHSET_FL_CUSTOMID)&0xFF00) == 0x6400))
    {
        WGTSetVisible(spec_func1_pg.tabpage1_Tb10_stExhaustPrs, TRUE);
        WGTSetVisible(spec_func1_pg.tabpage1_Tb10_stExhaustSpd, TRUE);
        WGTSetVisible(spec_func1_pg.tabpage1_Tb10_stExhaustPrs_unit, TRUE);
        WGTSetVisible(spec_func1_pg.tabpage1_Tb10_stExhaustSpd_unit, TRUE);
        WGTSetVisible(spec_func1_pg.tabpage1_Tb10_edExhaustPrs, TRUE);
        WGTSetVisible(spec_func1_pg.tabpage1_Tb10_edExhaustSpd, TRUE);
    }
    else
    {
        WGTSetVisible(spec_func1_pg.tabpage1_Tb10_stExhaustPrs, FALSE);
        WGTSetVisible(spec_func1_pg.tabpage1_Tb10_stExhaustSpd, FALSE);
        WGTSetVisible(spec_func1_pg.tabpage1_Tb10_stExhaustPrs_unit, FALSE);
        WGTSetVisible(spec_func1_pg.tabpage1_Tb10_stExhaustSpd_unit, FALSE);
        WGTSetVisible(spec_func1_pg.tabpage1_Tb10_edExhaustPrs, FALSE);
        WGTSetVisible(spec_func1_pg.tabpage1_Tb10_edExhaustSpd, FALSE);
    }
}
//20250106 chj 针阀开变灰控制
static void SetNdOpenShow()
{
    if((VarAdrToInt(d_inject1_MACHSET_FL_INj_SPC) != 0) && (VarAdrToInt(d_inject1_MACHSET_FL_INJ_SPC_FUNC) == 1))
    {
        WGTSetEnable(spec_func1_pg.tabpage1_Tb13_edNdlOnTm, TRUE);
        WGTSetEnable(spec_func1_pg.tabpage1_Tb13_labelNdlOnTm_unit, TRUE);

    }
    else
    {
        WGTSetEnable(spec_func1_pg.tabpage1_Tb13_edNdlOnTm, FALSE);
        WGTSetEnable(spec_func1_pg.tabpage1_Tb13_labelNdlOnTm_unit, FALSE);
    }

    if((VarAdrToInt(d_inject1_MACHSET_FL_INj_SPC) != 0) && (VarAdrToInt(d_inject1_MACHSET_FL_INJ_SPC_FUNC) == 0))
    {
        WGTSetEnable(spec_func1_pg.tabpage1_Tb13_edNdlDltOnTm, TRUE);
        WGTSetEnable(spec_func1_pg.tabpage1_Tb13_labelNdlDltOnTm_unit, TRUE);
    }
    else
    {
        WGTSetEnable(spec_func1_pg.tabpage1_Tb13_edNdlDltOnTm, FALSE);
        WGTSetEnable(spec_func1_pg.tabpage1_Tb13_labelNdlDltOnTm_unit, FALSE);
    }

}

//20250109 fqh 针阀2开变灰控制
static void SetNd2OpenShow()
{
    if((VarAdrToInt(d_inject1_MACHSET_FL_INj_SPC2) != 0) && (VarAdrToInt(d_inject1_MACHSET_FL_INJ_SPC_FUNC2) == 1))
    {
        WGTSetEnable(spec_func1_pg.tabpage1_Tb13_edNdlOnTm2, TRUE);
        WGTSetEnable(spec_func1_pg.tabpage1_Tb13_labelNdlOnTm_unit2, TRUE);

    }
    else
    {
        WGTSetEnable(spec_func1_pg.tabpage1_Tb13_edNdlOnTm2, FALSE);
        WGTSetEnable(spec_func1_pg.tabpage1_Tb13_labelNdlOnTm_unit2, FALSE);
    }

    if((VarAdrToInt(d_inject1_MACHSET_FL_INj_SPC2) != 0) && (VarAdrToInt(d_inject1_MACHSET_FL_INJ_SPC_FUNC2) == 0))
    {
        WGTSetEnable(spec_func1_pg.tabpage1_Tb13_edNdlDltOnTm2, TRUE);
        WGTSetEnable(spec_func1_pg.tabpage1_Tb13_labelNdlDltOnTm_unit2, TRUE);
    }
    else
    {
        WGTSetEnable(spec_func1_pg.tabpage1_Tb13_edNdlDltOnTm2, FALSE);
        WGTSetEnable(spec_func1_pg.tabpage1_Tb13_labelNdlDltOnTm_unit2, FALSE);
    }

}

//20250708 jhh 移动电储料功能到储料特殊功能画面 电储料液压通道可见
static void SetChgSpcHydr()
{
    if((VarAdrToInt(d_inject1_MACHSET_FL_ELECTRIC_CHG) == 1) && (VarAdrToInt(d_inject1_MACHSET_FL_ELECCHG_HYDR) == 1))
    {
        WGTSetVisible(spec_func1_pg.tabpage1_Tb16_label213, TRUE);
        WGTSetVisible(spec_func1_pg.tabpage1_Tb16_dropdown25, TRUE);
    }
    else
    {
        WGTSetVisible(spec_func1_pg.tabpage1_Tb16_label213, FALSE);
        WGTSetVisible(spec_func1_pg.tabpage1_Tb16_dropdown25, FALSE);
    }
}

static void CtlConnect()
{
}
static int Spec_Func1OnInit(Spec_Func1_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Spec_Func1OnShow(Spec_Func1_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_USERLOCK);//20210914 dyl touch

    G10SpecFunc1GuideSwitch();
    TabPageShow();
    //关模高压二段
    EditSetShowMode(ppg->tabpage1_Tb1_edit15,PUSHTAB,SetClampPosPush);
    if (VarAdrToInt(d_clamp1_MACHSET_FL_CLSHIPRES2) == FALSE)//20211105 dyl HighPrs#2
    {
        if (VarAdrToInt(p_PP_MACHSET_CLAMPVIEW_HIGH2_VISIBLE) != FALSE)
        {
            VarSendSaveSetIntByAdr(p_PP_MACHSET_CLAMPVIEW_HIGH2_VISIBLE, FALSE);
        }
    }
    //20241226 chj 威力士快速机模式下，部分参数变灰：关模高压二段,储料移动检测，储前电子尺检测
    if(((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x7A00) && VarAdrToInt(p_PP_MACHSET_WLSMACHINE_TYPE))
    {
        WGTSetEnable(ppg->tabpage1_Tb1_stHiPrs,FALSE);
        WGTSetEnable(ppg->tabpage1_Tb1_stbt21,FALSE);

        WGTSetEnable(ppg->tabpage1_Tb16_label63,FALSE);
        WGTSetEnable(ppg->tabpage1_Tb16_stbutton5,FALSE);

        WGTSetEnable(ppg->tabpage1_Tb16_label208,FALSE);
        WGTSetEnable(ppg->tabpage1_Tb16_stbutton6,FALSE);

    }
    else
    {
        WGTSetEnable(ppg->tabpage1_Tb1_stHiPrs,TRUE);
        WGTSetEnable(ppg->tabpage1_Tb1_stbt21,TRUE);

        WGTSetEnable(ppg->tabpage1_Tb16_label63,TRUE);
        WGTSetEnable(ppg->tabpage1_Tb16_stbutton5,TRUE);

        WGTSetEnable(ppg->tabpage1_Tb16_label208,TRUE);
        WGTSetEnable(ppg->tabpage1_Tb16_stbutton6,TRUE);
    }



    //蓄能器
    //20220923 dyl ECAT模式下才判断蓄能器压力检测通道号
    if(VarAdrToUI16(SYS_FL_MACH_CODE1)&0x04)
    {
        if((VarAdrToInt(SYS_FL_METER)&0x0004)!= 0)
        {
            WGTSetVisible(spec_func1_pg.tabpage1_Tb6_label19, TRUE);
            WGTSetVisible(spec_func1_pg.tabpage1_Tb6_edit1, TRUE);
        }
        else
        {
            WGTSetVisible(spec_func1_pg.tabpage1_Tb6_label19, FALSE);
            WGTSetVisible(spec_func1_pg.tabpage1_Tb6_edit1, FALSE);
        }
    }
    else
    {
        WGTSetVisible(spec_func1_pg.tabpage1_Tb6_label19, FALSE);
        WGTSetVisible(spec_func1_pg.tabpage1_Tb6_edit1, FALSE);
    }
    SetNozzlePosValueAndShow();

    //压塑
    SetInjMoldInTime();

    //动作开环 //20211118 dyl
    if(VarAdrToInt(SYS_FL_MACH_CODE52) & 0x800)
    {
        VarAdrSetInt(p_PP_TMP_TMPVAL20, TRUE);
    }
    else
    {
        VarAdrSetInt(p_PP_TMP_TMPVAL20, FALSE);
    }

    //胶木机 20241205 chj 排气压力，速度，只有在金鹰厂商下才显示
    SetExhaustPrsSpdShow();

    //20250106 chj 针阀开变灰控制
    SetNdOpenShow();

    //20250109 fqh 针阀2开变灰控制
    SetNd2OpenShow();

    //20250708 jhh 移动电储料功能到储料特殊功能画面 电储料液压通道可见
    SetChgSpcHydr();

    if(!WGTGetVisible(PageFramCurWgtByIndex(TabPageGetCurSel(ppg->tabpage1)+1)))
    {
        TabPageSelPage(ppg->tabpage1, 0);
    }

    if((VarAdrToInt(p_PP_OBJ_SPECIAL_DATA_SETUP_VISIBLE)==0) && (VarAdrToInt(p_PP_OBJ_OPENLOOP_OVERADJ_SHOW)==0))//20211224 dyl 射出开环标签显示与否
    {
        WGTSetVisible(spec_func1_pg.tabpage1_Tb8_label39, FALSE);
    }
    else
    {
        WGTSetVisible(spec_func1_pg.tabpage1_Tb8_label39, TRUE);
    }

    //20241214 chj 针阀提前关时间选项变灰设置
    SetNeedVlvEarlyOffTmVis();

    //20260109 fqh 针阀2提前关时间选项变灰设置
    SetNeedVlv2EarlyOffTmVis();

    FkeyTab(ppg->tabpage1, GUI_KEY_FUN1);
    return 0;
}
static int Spec_Func1OnHide(Spec_Func1_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch
    return 0;
}
static int Spec_Func1OnUpdate(Spec_Func1_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetCurButtonState();
    return 0;
}
static int Spec_Func1OnChange(Spec_Func1_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 addr,value,offset;
    PATTR pattr;

    pattr = (PATTR)msg_para;

    addr = OBJAttrGetAdr(pattr);
    value = VarAdrToInt(addr);

    //20210914 dyl touch 根据索引来定位那个tab子页，兼容触摸和按键
    offset = WGTGetGOffset(pwgt);
    if(offset > 0 && offset <= SPEC_FUNC1_NUM)
    {
        TabPageSelPage(ppg->tabpage1, offset-1);
    }

    switch (addr)
    {
    //高压二段
    case d_clamp1_MACHSET_FL_CLSHIPRES2:
        if(VarAdrToInt(d_clamp1_ACTPARA_ADPOS_CLSHIPRES2) > VarAdrToInt(d_clamp1_ACTPARA_ADPOSI_CLSPROTECTEND))
        {
            VarSendSaveSetIntByAdr(d_clamp1_ACTPARA_ADPOS_CLSHIPRES2, VarAdrToInt(d_clamp1_ACTPARA_ADPOSI_CLSPROTECTEND));
        }
        if (VarAdrToInt(d_clamp1_MACHSET_FL_CLSHIPRES2) == FALSE)//20211105 dyl HighPrs#2
        {
            if (VarAdrToInt(p_PP_MACHSET_CLAMPVIEW_HIGH2_VISIBLE) != FALSE)
            {
                VarSendSaveSetIntByAdr(p_PP_MACHSET_CLAMPVIEW_HIGH2_VISIBLE, FALSE);
            }
        }
        break;
    //开模泄压
    case d_clamp1_MOLDSET_FL_OPENDRAINFUNCTIO://开模泄压选择
        if(VarAdrToUI16(OPMODE_INDEX) != MODE_MANUAL)
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//原始值
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }
        break;
    //蓄能器
    case d_inject1_MOLDSET_FL_INCACC://射出蓄能器
        if(VarAdrToUI16(OPMODE_INDEX) != MODE_MANUAL)
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//原始值
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }
        break;
    case d_machine1_MACHSET_INJSTOR_HYPORT://蓄能器油路选择
        if(VarAdrToUI16(OPMODE_INDEX) != MODE_MANUAL)
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//原始值
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }
        CodeSendSave();
        break;
    case d_injstor1_MACHSET_INJSTOR_METERCH://压力检测通道
        if(CheckChannelUsed(value))
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//原始值
            ShowMsg(VW_MSG_METERCHANNELUSED);
            return -1;
        }
        else
        {
            SetNozzlePosValueAndShow();
        }
        break;
    //座进前射出
    case d_inject1_MOLDSET_FL_AUTOPURGE://座进前射出方式
        if(VarAdrToUI16(OPMODE_INDEX) != MODE_MANUAL)
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//原始值
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }
        break;
    //压塑
    case d_machine1_MOLDSET_FL_INJMOLDIN://压塑功能
        if(VarAdrToUI16(OPMODE_INDEX) != MODE_MANUAL)
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//原始值
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }
        break;
    //排气
    case d_clamp1_MOLDSET_FL_EXHAUSTMODE://排气方式
        if(VarAdrToUI16(OPMODE_INDEX) != MODE_MANUAL)
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//原始值
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }
        //胶木机 20241205 chj 排气压力，速度，只有在金鹰厂商下才显示
        SetExhaustPrsSpdShow();
        break;
    case d_clamp1_MOLDSET_FL_EXHAUST_CLAMPCLS://排气后关模 //20250306 jhh 非手动情况下提示
        if(VarAdrToUI16(OPMODE_INDEX) != MODE_MANUAL)
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//原始值
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }
        break;
        //20211221 dyl
    case d_machine1_MOLDSET_FL_CLOSJTCORE:
        if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL)//0211221 dyl 中子动作点
        {
            if (value==1)//20210820 dyl 中子动作点跟随特殊中子功能及联动功能变化
            {
                CorePointByLink();
            }
            else if(value>1)
            {
                VarSendSaveSetIntByAdr(d_coreA_MOLDSET_ADPOSI_CORE_FWD+(value-1)*0x100000,1);
                VarSendSaveSetIntByAdr(d_coreA_ACTPARA_ADPOSI_COREINACT+(value-1)*0x100000,VarAdrToInt(d_clamp1_MOLDSET_ADPOSI_EJTTRAVELCLOSE));
                //20210730 dyl 中子有效区位置
                if(PushtableType())
                {
                    CoreEffeAreaByPushtab(d_coreA_ACTPARA_ADPOSI_COREINACT+(value-1)*0x100000, d_coreA_MACHSET_ADPOSI_EFFCOREINPLUS+(value-1)*0x100000);
                }
            }
        }
        else
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//原始值
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }
        break;
    case d_machine1_MOLDSET_FL_OPNEJTCORE:
        if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL)//20211221 dyl 中子动作点
        {
            if(value==2)//0210820 dyl 中子动作点跟随特殊中子功能及联动功能变化
            {
                CorePointByLink();
            }
            else if(value>2)
            {
                VarSendSaveSetIntByAdr(d_coreA_MOLDSET_ADPOSI_CORE_BWD+(value-2)*0x100000,1);
                VarSendSaveSetIntByAdr(d_coreA_ACTPARA_ADPOSI_COREOUTACT+(value-2)*0x100000,VarAdrToInt(d_clamp1_MOLDSET_ADPOSI_EJTTRAVEL));
                //20210730 dyl 中子有效区位置
                if(PushtableType())
                {
                    CoreEffeAreaByPushtab(d_coreA_ACTPARA_ADPOSI_COREOUTACT+(value-2)*0x100000, d_coreA_MACHSET_ADPOSI_EFFCOREOUTPLUS+(value-2)*0x100000);
                }
            }
        }
        else
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//原始值
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }
        break;
    case d_clamp1_MOLDSET_ADPOSI_EJTTRAVELCLOSE:
        if(VarAdrToInt(d_machine1_MOLDSET_FL_CLOSJTCORE)==1)
        {
            //20210820 dyl 中子动作点跟随特殊中子功能及联动功能变化
            CorePointByLink();
        }
        else if(VarAdrToInt(d_machine1_MOLDSET_FL_CLOSJTCORE)>1)
        {
            VarSendSaveSetIntByAdr(d_coreA_ACTPARA_ADPOSI_COREINACT+(VarAdrToInt(d_machine1_MOLDSET_FL_CLOSJTCORE)-1)*0x100000,value);
            //20210730 dyl 中子有效区位置
            if(PushtableType())
            {
                CoreEffeAreaByPushtab(d_coreA_ACTPARA_ADPOSI_COREINACT+(VarAdrToInt(d_machine1_MOLDSET_FL_CLOSJTCORE)-1)*0x100000, d_coreA_MACHSET_ADPOSI_EFFCOREINPLUS+(VarAdrToInt(d_machine1_MOLDSET_FL_CLOSJTCORE)-1)*0x100000);
            }
        }
        break;
    case d_clamp1_MOLDSET_ADPOSI_EJTTRAVEL:
        if(VarAdrToInt(d_machine1_MOLDSET_FL_OPNEJTCORE)==2)
        {
            //20210820 dyl 中子动作点跟随特殊中子功能及联动功能变化
            CorePointByLink();
        }
        else if(VarAdrToInt(d_machine1_MOLDSET_FL_OPNEJTCORE)>2)
        {
            VarSendSaveSetIntByAdr(d_coreA_ACTPARA_ADPOSI_COREOUTACT+(VarAdrToInt(d_machine1_MOLDSET_FL_OPNEJTCORE)-2)*0x100000,value);
            //20210730 dyl 中子有效区位置
            if(PushtableType())
            {
                CoreEffeAreaByPushtab(d_coreA_ACTPARA_ADPOSI_COREOUTACT+(VarAdrToInt(d_machine1_MOLDSET_FL_OPNEJTCORE)-2)*0x100000, d_coreA_MACHSET_ADPOSI_EFFCOREOUTPLUS+(VarAdrToInt(d_machine1_MOLDSET_FL_OPNEJTCORE)-2)*0x100000);
            }
        }
        break;
        //20211227 dyl
    case p_PP_MACHSET_REMELT_FUNC:
        if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL)
        {
            if (value==1)//计时
            {
                VarAdrSetInt(SYS_FL_MACH_CODE54, VarAdrToInt(SYS_FL_MACH_CODE54)|0x0080);
                if (VarAdrToInt(d_inject1_MOLDSET_ADPOSI_CHGFLOWMOLD) != 0)
                {
                    VarSendSaveSetIntByAdr(d_inject1_MOLDSET_ADPOSI_CHGFLOWMOLD, 0);
                }
            }
            else if (value==2)//距离
            {
                VarAdrSetInt(SYS_FL_MACH_CODE54, VarAdrToInt(SYS_FL_MACH_CODE54)|0x0080);
                if (VarAdrToInt(d_inject1_MOLDSET_TM_CHGFLOWMOLDING) != 0)
                {
                    VarSendSaveSetIntByAdr(d_inject1_MOLDSET_TM_CHGFLOWMOLDING, 0);
                }
            }
            else
            {
                VarAdrSetInt(SYS_FL_MACH_CODE54, VarAdrToInt(SYS_FL_MACH_CODE54)&(~0x0080));
                if (VarAdrToInt(d_inject1_MOLDSET_ADPOSI_CHGFLOWMOLD) != 0)
                {
                    VarSendSaveSetIntByAdr(d_inject1_MOLDSET_ADPOSI_CHGFLOWMOLD, 0);
                }
                if (VarAdrToInt(d_inject1_MOLDSET_TM_CHGFLOWMOLDING) != 0)
                {
                    VarSendSaveSetIntByAdr(d_inject1_MOLDSET_TM_CHGFLOWMOLDING, 0);
                }
            }
            CodeSendSave();
        }
        else
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//原始值
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }
        break;
    case d_inject1_MOLDSET_TM_CHGFLOWMOLDING:
        if(value == 0)
        {
            if (VarAdrToInt(d_inject1_MOLDSET_ADPOSI_CHGFLOWMOLD) == 0)
            {
                if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL)
                {
                    VarAdrSetInt(SYS_FL_MACH_CODE54, VarAdrToInt(SYS_FL_MACH_CODE54)&(~0x0080));
                    CodeSendSave();
                }
                else
                {
                    VarSendSaveSetIntByAdr(addr,msg_value);//原始值
                    ShowMsg(VW_MSG_MANUAL);
                    return -1;
                }
            }
        }
        break;
    case d_inject1_MOLDSET_ADPOSI_CHGFLOWMOLD:
        if(value == 0)
        {
            if (VarAdrToInt(d_inject1_MOLDSET_TM_CHGFLOWMOLDING) == 0)
            {
                if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL)
                {
                    VarAdrSetInt(SYS_FL_MACH_CODE54, VarAdrToInt(SYS_FL_MACH_CODE54)&(~0x0080));
                    CodeSendSave();
                }
                else
                {
                    VarSendSaveSetIntByAdr(addr,msg_value);//原始值
                    ShowMsg(VW_MSG_MANUAL);
                    return -1;
                }
            }
        }
        break;
    case d_inject1_MACHSET_FL_INJSPC_AIR:
    case d_inject1_MACHSET_FL_INJSPC_AIR1:
        if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL)
        {
            Inj_SPC_Func();//20241205 chj 胶口针阀页逻辑 使用针阀同步输出吹气时，此吹气不可用--将动作时间设置为 0
        }
        else
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//原始值//20250306 jhh 非手动情况下提示
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }
        break;
    case d_inject1_MACHSET_FL_INj_SPC:
        Inj_SPC_Func();//20241205 chj 胶口针阀页逻辑 使用针阀同步输出吹气时，此吹气不可用--将动作时间设置为 0
        SetNeedVlvEarlyOffTmVis();//20241214 chj 针阀提前关时间选项变灰设置
        SetNdOpenShow();//20250106 chj 针阀开变灰控制
        break;
    case d_inject1_MACHSET_INJ_SPC_FUNC_CLS:
        SetNeedVlvEarlyOffTmVis();//20241214 chj 针阀提前关时间选项变灰设置
        break;
    case d_inject1_MACHSET_FL_INJ_SPC_FUNC:
        SetNdOpenShow();//20250106 chj 针阀开变灰控制
        break;

    //20250109 fqh 针阀2新增内容
    case d_inject1_MACHSET_FL_INj_SPC2:
        if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL)
        {
            SetNeedVlv2EarlyOffTmVis();//20250109 fqh 针阀2提前关时间选项变灰设置
            SetNd2OpenShow();//20250109 fqh 针阀2开变灰控制
        }
        else
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//原始值//20250306 jhh 非手动情况下提示
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }
        break;
    case d_inject1_MACHSET_INJ_SPC_FUNC_CLS2:
        if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL)
        {
            SetNeedVlv2EarlyOffTmVis();//20250109 fqh 针阀2提前关时间选项变灰设置
        }
        else
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//原始值//20250306 jhh 非手动情况下提示
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }
        break;
    case d_inject1_MACHSET_FL_INJ_SPC_FUNC2:
        if(VarAdrToInt(OPMODE_INDEX) == MODE_MANUAL)
        {
            SetNd2OpenShow();//20250109 fqh 针阀开变灰控制
        }
        else
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//原始值//20250306 jhh 非手动情况下提示
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }
        break;
    case d_inject1_MOLDSET_FL_PREVENTCOOL:
        if(VarAdrToInt(OPMODE_INDEX) != MODE_MANUAL)
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//原始值//20250306 jhh 非手动情况下提示
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }
        break;
    case d_inject1_MACHSET_FL_ELECTRIC_CHG://20250708 jhh 移动电储料功能到储料特殊功能画面
        SetChgSpcHydr();
        break;
    case d_inject1_MACHSET_FL_ELECTRIC_CHG_MODE:
        if(VarAdrToInt(OPMODE_INDEX) != MODE_MANUAL)
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//20250708 jhh 移动电储料功能到储料特殊功能画面
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }
        break;
    case d_inject1_MACHSET_PORT_ELECCHG_DA:
        if(VarAdrToInt(OPMODE_INDEX) != MODE_MANUAL)
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//20250708 jhh 移动电储料功能到储料特殊功能画面
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }
        break;
    case d_inject1_MACHSET_FL_ELECCHG_HYDR:
        if(VarAdrToInt(OPMODE_INDEX) != MODE_MANUAL)
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//20250708 jhh 移动电储料功能到储料特殊功能画面
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }
        else
        {
            SetChgSpcHydr();
        }
        break;
    case d_inject1_MACHSET_PORT_ELECCHG_HYDR:
        if(VarAdrToInt(OPMODE_INDEX) != MODE_MANUAL)
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//20250708 jhh 移动电储料功能到储料特殊功能画面
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }
        break;
    default :
        break;
    }

    if(pwgt ==ppg->tabpage1_Tb8_dropdown1)//20211118 dyl
    {
        if(VarAdrToUI16(OPMODE_INDEX) != MODE_MANUAL)
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//原始值
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }
        else
        {
            if(VarAdrToInt(p_PP_TMP_TMPVAL20))
            {
                VarAdrSetInt(SYS_FL_MACH_CODE52, VarAdrToInt(SYS_FL_MACH_CODE52)|0x800);
            }
            else
            {
                VarAdrSetInt(SYS_FL_MACH_CODE52, VarAdrToInt(SYS_FL_MACH_CODE52)&(~0x800));
            }
            CodeSendSave();
        }
    }

    return 0;
}
