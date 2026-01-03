#include "WTemp_xx.h"
#include "typedef.h"
#include "pagefram.h"
#include "usermod.h"
#include "privdef.h"
#include "vardb.h"
#include "temp.h"
#include "tabpagewgt.h"
#include "warm.h"
#include "database.h"
#include "command.h"
#include "information.h"
#include "texttbl.h"
#include "ctrlcmd.h"
#include "protcmd.h"
#include "editwgt.h"
#include "oprintf.h"
#include "keymod.h"
#include "key.h"
#include "imagewgt.h"
#include "stdio.h"
#include "labelwgt.h"
#include "buttonwgt.h"
#include "keydef.h"
#include "usermod.h"//20210914 dyl touch
#include "password.h"

DEFINE_TEMP_XX_MAP

CONNECT_TEMP_XX_EVENT

#define TMP_ADDR_OFFSET	(6)  //临时变量地址偏移

#define TMP_NHOUR	    TMPVAL(0)  //加热预设开小时  临时变量0-6
#define TMP_NMINUTE  	TMPVAL(7)  //加热预设开分钟  临时变量7-13
#define TMP_BWARM		TMPVAL(14) //加热预设开功能  临时变量14-20

#define TMP_NCLSHOUR	TMPVAL(21) //加热预设关小时  临时变量21-27
#define TMP_NCLSMIN		TMPVAL(28) //加热预设关分钟  临时变量28-34
#define TMP_BWARMCLS	TMPVAL(35) //加热预设关功能  临时变量35-41

#define TMP_WEEK	    TMPVAL(100) //星期变量   临时变量100

//#define AB_PAGE_SEL     TMPVAL(111)  //AB温度切换
#define A_PAGE_SEL     TMPVAL(112)  //A温度切换
#define B_PAGE_SEL     TMPVAL(113)  //B温度切换

#define TMP_START_OPT         TMPSTR(34)
#define TMP_START_FORCE       TMPSTR(35)
#define TMP_ROOM_TEMP         TMPVAL(55)
#define TMP_HOUR              TMPVAL(56)
#define TMP_MIN               TMPVAL(57)
#define TMP_TEMPOPT_PASSWORD   TMPVAL(66)  //温度优化修改密码
#define TMP_SELECT            TMPVAL(88)   //温度选择段数
#define TMP_REAL1            TMPSTR(110)   //温度实际值1显示
#define TMP_REAL2            TMPSTR(120)   //温度实际值1显示
#define TMP_OIL				 TMPSTR(130)   //油温显示
#define TMP_MOTOR            TMPSTR(131)   //电机温度
#define TMP_MOUTH            TMPSTR(132)   //落料口温度
#define OPT_PASSWORD  8367  //修改密码

#define TMP_PREHEAT_PSW	    TMPVAL(200) //临时变量 预温计时密码  //20241017 chj 金鹰特殊要求，预温计时增加密码
#define TMP_PREHEAT_PSW_VISIBLE	    TMPVAL(201) //临时变量 预温计时密码可见
#define TMP_PREHEAT_PSW_CAL TMPVAL(202) //临时变量 预温计时密码计算后的值

static BOOL b_switchopt = FALSE;
static void SegSelect();

/**
* @brief     :定时加温设置
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191122
*/
static void InitWarmData()
{
    int i;

    for(i = 0;i<7;i++)
    {
        VarAdrSetInt(TMP_NHOUR+i,dbwarmx.dbwarmconfig[i].nHour);
        VarAdrSetInt(TMP_NMINUTE+i,dbwarmx.dbwarmconfig[i].nMinute);
        VarAdrSetInt(TMP_BWARM+i,dbwarmx.dbwarmconfig[i].bWarm);
        VarAdrSetInt(TMP_NCLSHOUR+i,dbwarmx.dbwarmconfig[i].nClsHour);
        VarAdrSetInt(TMP_NCLSMIN+i,dbwarmx.dbwarmconfig[i].nClsMin);
        VarAdrSetInt(TMP_BWARMCLS+i,dbwarmx.dbwarmconfig[i].bWarmCls);
    }
}

static BOOL GetMotorState()
{
    int i, choose;
    for(i = 0; i < 9; ++i)
    {
        choose = VarAdrToInt(d_TempCtrlComm1_MACHSET_DATEMP_FUNC_2 + i);
        if(choose == 5)
            return TRUE;
    }
    return FALSE;
}

/**
* @brief     :电机温度控制
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191125
*/

static void MotorTemp(Temp_xx_PG* ppg)
{
    UI16 part;
    part = VarAdrToInt(p_PP_STATE_TEMP_CTRL_PART);

    if(part >= 2)
    {
        WGTSetVisible(ppg->stbtAB, TRUE);
        WGTSetVisible(ppg->image_AB, TRUE);
        WGTSetVisible(ppg->label168,TRUE);
        if(GetMotorState())
        {
            WGTSetVisible(ppg->subpage20, TRUE);
        }
        else
        {
            WGTSetVisible(ppg->subpage20, FALSE);
        }
    }
    else
    {
        WGTSetVisible(ppg->stbtAB, FALSE);
        WGTSetVisible(ppg->image_AB, FALSE);
        WGTSetVisible(ppg->label168,FALSE);
        if(GetMotorState())
        {
            WGTSetVisible(ppg->subpage20, TRUE);
        }
        else
        {
            WGTSetVisible(ppg->subpage20, FALSE);
        }
    }
}



/**
* @brief     :温度优化是否允许操作
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191127
*/
static void TempOptEnable(Temp_xx_PG* ppg,BOOL enable)
{
    WGTSetEnable(ppg->tabpage1_Tb6_btForceOpt,enable);
    WGTSetEnable(ppg->tabpage1_Tb6_btRestore,enable);
}
/**
* @brief     :电热按键警报
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191127
*/
static void ShowAlarm(Temp_xx_PG* ppg)
{
    static BOOL show_switch = FALSE;
    static UI32  time_1400ms = 0;
    static UI32  time_800ms  = 0;
    BOOL bTimer=FALSE;

    if(!show_switch)
    {
        if(GetTick()-(time_1400ms+time_800ms)>=2000)//20200812
        {
            time_1400ms = GetTick()-time_800ms;
            bTimer = TRUE;
        }
    }
    else
    {
        if(GetTick() - (time_800ms+time_1400ms) >=800)
        {
            time_800ms =GetTick()-time_1400ms;
            bTimer=TRUE;
        }
    }
    if(bTimer)
    {
        show_switch=!show_switch;
    }

    if(VarAdrToInt(d_TempCtrlComm1_STATE_DATEMP_YOUHUAXUQIU) || VarAdrToInt(d_TempCtrlComm2_STATE_DATEMP_YOUHUAXUQIU))
    {
        WGTSetVisible(ppg->heaterAlarm,TRUE);//20200812
        if(show_switch)
        {
            LabelSetBkColor(ppg->heaterAlarm,0xfe8247);
        }
        else
        {
            LabelSetBkColor(ppg->heaterAlarm,0xfefe00);
        }
    }
    else
    {
        WGTSetVisible(ppg->heaterAlarm,FALSE);
    }
}

/**
* @brief     :室温显示
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191127
*/
static void ShowRoomTemp(Temp_xx_PG* ppg)
{
    if(VarAdrToInt(p_PP_MACHSET_E502_TEMP_AB) == 1)
    {
        if(VarAdrToInt(d_TempCtrlComm2_STATE_WTEMP_LM73)>0x8000){
            VarAdrSetInt(TMP_ROOM_TEMP,VarAdrToInt(d_TempCtrlComm2_STATE_WTEMP_LM73)-0x8000);
            WGTSetVisible(ppg->tabpage1_Tb6_lable_add, FALSE);
            WGTSetVisible(ppg->tabpage1_Tb6_lable_sub, TRUE);
        }
        else{
            VarAdrSetInt(TMP_ROOM_TEMP,VarAdrToInt(d_TempCtrlComm2_STATE_WTEMP_LM73));
            WGTSetVisible(ppg->tabpage1_Tb6_lable_add, TRUE);
            WGTSetVisible(ppg->tabpage1_Tb6_lable_sub, FALSE);
        }
    }
    else
    {
        if(VarAdrToInt(d_TempCtrlComm1_STATE_WTEMP_LM73)>0x8000){
            VarAdrSetInt(TMP_ROOM_TEMP,VarAdrToInt(d_TempCtrlComm1_STATE_WTEMP_LM73)-0x8000);
            WGTSetVisible(ppg->tabpage1_Tb6_lable_add, FALSE);
            WGTSetVisible(ppg->tabpage1_Tb6_lable_sub, TRUE);
        }
        else{
            VarAdrSetInt(TMP_ROOM_TEMP,VarAdrToInt(d_TempCtrlComm1_STATE_WTEMP_LM73));
            WGTSetVisible(ppg->tabpage1_Tb6_lable_add, TRUE);
            WGTSetVisible(ppg->tabpage1_Tb6_lable_sub, FALSE);
        }
    }
}

/**
* @brief     :温度优化页面进入
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191127
*/
static void TempOptPageIn(Temp_xx_PG* ppg)
{
    VarAdrSetInt(TMP_TEMPOPT_PASSWORD,0);

    TempOptEnable(ppg,FALSE);
}

/**
* @brief     :温度优化页面更新
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191127
*/
static void TempOptUpdate(Temp_xx_PG* ppg)
{
    if(isTempOptimizex())
    {
        if(b_switchopt)
        {
            VarAdrSetStr(TMP_START_OPT, GetTextTran(TEXT_OPTIMIZE_START));
            VarAdrSetStr(TMP_START_FORCE, GetTextTran(TEXT_FORCE_OPTIMIZE_ING));
        }
        else
        {
            VarAdrSetStr(TMP_START_OPT, GetTextTran(TEXT_OPTIMIZE_ING));
            VarAdrSetStr(TMP_START_FORCE, GetTextTran(TEXT_FORCE_OPTIMIZE_START));
        }
        WGTSetEnable(ppg->tabpage1_Tb6_btOptimize, FALSE);
        WGTSetEnable(ppg->tabpage1_Tb6_btForceOpt, FALSE);
        WGTSetEnable(ppg->tabpage1_Tb6_btRestore, FALSE);
    }
    else
    {
        VarAdrSetStr(TMP_START_OPT, GetTextTran(TEXT_OPTIMIZE_START));
        VarAdrSetStr(TMP_START_FORCE, GetTextTran(TEXT_FORCE_OPTIMIZE_START));
        WGTSetEnable(ppg->tabpage1_Tb6_btOptimize, TRUE);
        if(VarAdrToUI16(TMP_TEMPOPT_PASSWORD) == OPT_PASSWORD)
        {
            WGTSetEnable(ppg->tabpage1_Tb6_btForceOpt, TRUE);
            WGTSetEnable(ppg->tabpage1_Tb6_btRestore, TRUE);
        }
        else
        {
            WGTSetEnable(ppg->tabpage1_Tb6_btForceOpt, FALSE);
            WGTSetEnable(ppg->tabpage1_Tb6_btRestore, FALSE);
        }
    }
    ShowRoomTemp(ppg);
    ShowAlarm(ppg);
}

/**
* @brief     :温度优化页面改变
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191127
*/
static int TempOptChange(UI32 addr, PWGT pwgt, Temp_xx_PG* ppg)
{
    UI32 value;
    UI16 pdata[3]={0};

    switch(addr)
    {
    case TMP_TEMPOPT_PASSWORD: //修改密码
        value = VarAdrToUI16(TMP_TEMPOPT_PASSWORD);
        if(value == OPT_PASSWORD)
        {
            TempOptEnable(ppg,TRUE);
        }
        else
        {
            TempOptEnable(ppg,FALSE);
        }
        break;
    default:
        if(pwgt ==  ppg->tabpage1_Tb6_btOptimize) // 优化
        {
            if(VarAdrToInt(d_TempCtrlComm1_STATE_FL_HEATERSTATUS))//20200812
            {
                ShowMsg(VM_MSG_CLOSEHEATER);
                return 0;
            }
            pdata[1]=0xFF;
            ProtSysCtrl(TEMPOPTIMIZE_CMD,3,pdata);
            b_switchopt = FALSE;
        }
        else if(pwgt ==  ppg->tabpage1_Tb6_btForceOpt)//强制优化
        {
            if(VarAdrToInt(d_TempCtrlComm1_STATE_FL_HEATERSTATUS))//20200812
            {
                ShowMsg(VM_MSG_CLOSEHEATER);
                return 0;
            }
            pdata[0] = OPT_PASSWORD;
            pdata[1]=0xFF;
            ProtSysCtrl(TEMPOPTIMIZE_CMD,3,pdata);
            b_switchopt = TRUE;
        }
        else if(pwgt == ppg->tabpage1_Tb6_btRestore)
        {
            if(VarAdrToInt(d_TempCtrlComm1_STATE_FL_HEATERSTATUS))//20200812
            {
                ShowMsg(VM_MSG_CLOSEHEATER);
                return 0;
            }
            pdata[0] = 0xFF;
            ProtSysCtrl(COMM_SUBCMD_TEMP_PID_RESTORE,1,pdata);
            b_switchopt = TRUE;
        }
        break;
    }
}

static void TempCtlFuncReadOnly(PWGT ppg,BOOL enable)
{
    EditSetReadOnly(ppg, enable);
    WGTSetEnable(ppg, TRUE);
    if(enable)
    {
        EditSetBKColor(ppg, 0xdedede);
        EditSetTextColor(ppg, 0x000000);
    }
    else
    {
        EditSetBKColor(ppg, 0xFFFFFF);
        EditSetTextColor(ppg, 0x000000);
    }
}
static void TempCtlFuncEnable(PWGT ppg,BOOL enable)
{
    TempCtlFuncReadOnly(ppg, FALSE);
    WGTSetEnable(ppg, enable);
}
/**
* @brief     :温控功能选择
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191127
*/
static void TempCtlFunc(UI16 seg, UI16 old_value)
{
    UI16 fun_selt;
    PWGT pwgt;
    int i;
    int temp_ab, choose;
    int motor_num = 0;
    int mouth_num = 0;
    int oil_num = 0;
    BOOL func_oil_temp = FALSE;
    BOOL func_motor_temp = FALSE;
    BOOL func_mouth_temp = FALSE;
    temp_ab = VarAdrToInt(p_PP_MACHSET_E502_TEMP_AB);

    if(temp_ab == 1)
    {
        fun_selt = VarAdrToInt(d_TempCtrlComm2_MACHSET_DATEMP_FUNC_2 + seg);
    }
    else
    {
        fun_selt = VarAdrToInt(d_TempCtrlComm1_MACHSET_DATEMP_FUNC_2 + seg);
        for(i = 0; i < 9; ++i)
        {
            choose = VarAdrToInt(d_TempCtrlComm1_MACHSET_DATEMP_FUNC_2 + i);
            if(choose == 5)
            {
                motor_num++;
                if(motor_num >= 2)
                    func_motor_temp = TRUE;
            }
            else if(choose == 6)
            {
                mouth_num++;
                if(mouth_num >= 2)
                    func_mouth_temp = TRUE;
            }
            else if(choose == 7)
            {
                oil_num++;
                if(oil_num >= 2)
                    func_oil_temp = TRUE;
            }
        }
    }

    switch (fun_selt) {
    case 0:
        for(i = 0; i < 9; ++i)
        {
            if(i == 3)
            {
                pwgt = PageFramCurWgtByIndex(i*10+1+seg+9970);
            }
            else
            {
                pwgt = PageFramCurWgtByIndex(i*10+1+seg);
            }
            TempCtlFuncEnable(pwgt, FALSE);
        }
        break;
    case 1:
        for(i = 0; i < 9; ++i)
        {
            if(i == 3)
            {
                pwgt = PageFramCurWgtByIndex(i*10+1+seg+9970);
            }
            else
            {
                pwgt = PageFramCurWgtByIndex(i*10+1+seg);
            }
            TempCtlFuncEnable(pwgt, FALSE);
        }
        break;
    case 2:
        for(i = 0; i < 3; ++i)
        {
            pwgt = PageFramCurWgtByIndex(i*10+1+seg);
            TempCtlFuncEnable(pwgt, FALSE);
        }
        pwgt = PageFramCurWgtByIndex(10001+seg);
        TempCtlFuncEnable(pwgt, TRUE);
        pwgt = PageFramCurWgtByIndex(41+seg);
        TempCtlFuncEnable(pwgt, TRUE);
        for(i = 5; i < 9; ++i)
        {
            pwgt = PageFramCurWgtByIndex(i*10+1+seg);
            TempCtlFuncEnable(pwgt, FALSE);
        }
        break;
    case 3:
    case 4:
        for(i = 0; i < 9; ++i)
        {
            if(i == 3)
            {
                pwgt = PageFramCurWgtByIndex(i*10+1+seg+9970);
            }
            else
            {
                pwgt = PageFramCurWgtByIndex(i*10+1+seg);
            }
            TempCtlFuncEnable(pwgt, TRUE);
        }
        break;
    case 5:
        if(temp_ab == 1)
        {
            ShowMsg(VW_MSG_USEFIRSTONE);
            VarSendSaveSetIntByAdr(d_TempCtrlComm2_MACHSET_DATEMP_FUNC_2+seg, old_value);
        }
        else
        {
            if(func_motor_temp == TRUE)
            {
                ShowMsg(VW_MSG_MOTORTEMPUSED);
                VarSendSaveSetIntByAdr(d_TempCtrlComm1_MACHSET_DATEMP_FUNC_2+seg, old_value);
            }
            else
            {
                for(i = 0; i < 9; ++i)
                {
                    if(i == 3)
                    {
                        pwgt = PageFramCurWgtByIndex(i*10+1+seg+9970);
                    }
                    else
                    {
                        pwgt = PageFramCurWgtByIndex(i*10+1+seg);
                    }
                    TempCtlFuncEnable(pwgt, FALSE);
                }
            }
        }
        break;
    case 6:
        if(temp_ab == 1)
        {
            ShowMsg(VW_MSG_USEFIRSTONE);
            VarSendSaveSetIntByAdr(d_TempCtrlComm2_MACHSET_DATEMP_FUNC_2+seg, old_value);
        }
        else
        {
            if(func_mouth_temp == TRUE)
            {
                ShowMsg(VW_MSG_MOUTHTEMPUSED);
                VarSendSaveSetIntByAdr(d_TempCtrlComm1_MACHSET_DATEMP_FUNC_2+seg, old_value);
            }
            else
            {
                for(i = 0; i < 9; ++i)
                {
                    if(i == 3)
                    {
                        pwgt = PageFramCurWgtByIndex(i*10+1+seg+9970);
                    }
                    else
                    {
                        pwgt = PageFramCurWgtByIndex(i*10+1+seg);
                    }
                    TempCtlFuncEnable(pwgt, FALSE);
                }
            }
        }
        break;
    case 7:
        if(temp_ab == 1)
        {
            ShowMsg(VW_MSG_USEFIRSTONE);
            VarSendSaveSetIntByAdr(d_TempCtrlComm2_MACHSET_DATEMP_FUNC_2+seg, old_value);
        }
        else
        {
            if(func_oil_temp == TRUE)
            {
                ShowMsg(VW_MSG_OILTEMPUSED);
                VarSendSaveSetIntByAdr(d_TempCtrlComm1_MACHSET_DATEMP_FUNC_2+seg, old_value);
            }
            else
            {
                for(i = 0; i < 9; ++i)
                {
                    if(i == 3)
                    {
                        pwgt = PageFramCurWgtByIndex(i*10+1+seg+9970);
                    }
                    else
                    {
                        pwgt = PageFramCurWgtByIndex(i*10+1+seg);
                    }
                    TempCtlFuncEnable(pwgt, FALSE);
                }
            }
        }
        break;
    default:
        break;
    }
}

/**
* @brief     :AB地址切换
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191127
*/
static void SwitchAB(Temp_xx_PG* ppg)
{
    int i;
    PWGT pwgt;

    if(VarAdrToInt(p_PP_MACHSET_E502_TEMP_AB) == 1)
    {
        VarAdrSetInt(A_PAGE_SEL, 0);
        VarAdrSetInt(B_PAGE_SEL, 0xffff & VarAdrToInt(TMP_SELECT));
        ImageSetPicName(ppg->image_AB, "BackCoreB.png");
        for(i = 0; i < 9; ++i)
        {
            pwgt = PageFramCurWgtByIndex(1+i);
            WGTSetAttrByName(pwgt, "text", d_TempCtrlComm2_MOLDSET_DATEMP_SETTING_2 + i);
            pwgt = PageFramCurWgtByIndex(11+i);
            WGTSetAttrByName(pwgt, "text", d_TempCtrlComm2_MOLDSET_DATEMP_MAX_2 + i);
            pwgt = PageFramCurWgtByIndex(21+i);
            WGTSetAttrByName(pwgt, "text", d_TempCtrlComm2_MOLDSET_DATEMP_MIN_2 + i);
            pwgt = PageFramCurWgtByIndex(10001+i);
            WGTSetAttrByName(pwgt, "text", d_TempCtrlComm2_MOLDSET_DACENT_HEATERON_2 + i);
            pwgt = PageFramCurWgtByIndex(41+i);
            WGTSetAttrByName(pwgt, "text", d_TempCtrlComm2_MACHSET_TM_RESPONSELESSCYCLE_2 + i);
            pwgt = PageFramCurWgtByIndex(51+i);
            WGTSetAttrByName(pwgt, "text", d_TempCtrlComm2_MACHSET_DATEMP_RAMP_2 + i);
            pwgt = PageFramCurWgtByIndex(61+i);
            WGTSetAttrByName(pwgt, "text", d_TempCtrlComm2_MACHSET_DAPID_KP_2 + i);
            pwgt = PageFramCurWgtByIndex(71+i);
            WGTSetAttrByName(pwgt, "text", d_TempCtrlComm2_MACHSET_DAPID_TI_2 + i);
            pwgt = PageFramCurWgtByIndex(81+i);
            WGTSetAttrByName(pwgt, "text", d_TempCtrlComm2_MACHSET_DAPID_TD_2 + i);
            pwgt = PageFramCurWgtByIndex(91+i);
            WGTSetAttrByName(pwgt, "value", d_TempCtrlComm2_MACHSET_DATEMP_FUNC_2 + i);
            pwgt = PageFramCurWgtByIndex(101+i);
            WGTSetAttrByName(pwgt, "text", d_TempCtrlComm2_STATE_OUTPUT_PER_2 + i);
        }
        WGTSetAttrByName(ppg->tabpage1_Tb1_stbt_warn_func, "value", d_TempCtrlComm2_MOLDSET_FL_TEMPHOLD);
        WGTSetAttrByName(ppg->tabpage1_Tb1_stbt_sync, "value", d_TempCtrlComm2_MOLDSET_CONFUNC);
        WGTSetAttrByName(ppg->tabpage1_Tb1_ed_warm_temp, "text", d_TempCtrlComm2_MOLDSET_DATEMP_HOLD);
        WGTSetAttrByName(ppg->tabpage1_Tb1_ed_per_tm, "text", d_TempCtrlComm2_MOLDSET_TM_HEATERWARM);
        //WGTSetAttrByName(ppg->tabpage1_Tb5_ed_per_tm, "text", d_TempCtrlComm2_MOLDSET_TM_HEATERWARM);
        WGTSetAttrByName(ppg->tabpage1_Tb1_stbt_no_check, "value", d_TempCtrlComm2_MACHSET_FL_TEMPWORDUPCHECK);
        WGTSetAttrByName(ppg->tabpage1_Tb1_edit96, "text", p_PP_STATE_remain2Min);
        WGTSetAttrByName(ppg->tabpage1_Tb1_edit97, "text", p_PP_STATE_remain2Sec);

        WGTSetAttrByName(ppg->tabpage1_Tb5_stbt_cool, "value", d_TempCtrlComm2_MOLDSET_FL_TEMPCOOL);
        WGTSetAttrByName(ppg->tabpage1_Tb5_ed_temp_seg, "text", d_TempCtrlComm2_MACHSET_DATEMP_CH_NUM);
        WGTSetAttrByName(ppg->tabpage1_Tb5_ed_scan_cyc, "text", d_TempCtrlComm2_MACHSET_DATEMP_SCTM_MIN);
        WGTSetAttrByName(ppg->tabpage1_Tb5_ed_cool_max, "text", d_TempCtrlComm2_MACHSET_DATEMP_TEMPCOOLERON);
        WGTSetAttrByName(ppg->tabpage1_Tb5_ed_cool_min, "text", d_TempCtrlComm2_MACHSET_DATEMP_TEMPCOOLEROFF);
        WGTSetAttrByName(ppg->tabpage1_Tb5_edit_m_temp, "text", d_TempCtrlComm2_STATE_DATEMP_CHARGEPOS);
        WGTSetAttrByName(ppg->tabpage1_Tb5_edit_m_temp_max, "text", d_TempCtrlComm2_MACHSET_TEMPCOOLMAX);
        WGTSetAttrByName(ppg->tabpage1_Tb5_edit_m_temp_on, "text", d_TempCtrlComm2_MACHSET_DATEMP_TEMPCOOLERONMOUTH);
        WGTSetAttrByName(ppg->tabpage1_Tb5_edit_m_temp_off, "text", d_TempCtrlComm2_MACHSET_DATEMP_TEMPCOOLEROFFMOUTH);
    }
    else
    {
        VarAdrSetInt(A_PAGE_SEL, 0xffff & VarAdrToInt(TMP_SELECT));
        VarAdrSetInt(B_PAGE_SEL, 0);
        ImageSetPicName(ppg->image_AB, "BackCoreA.png");
        for(i = 0; i < 9; ++i)
        {
            pwgt = PageFramCurWgtByIndex(1+i);
            WGTSetAttrByName(pwgt, "text", d_TempCtrlComm1_MOLDSET_DATEMP_SETTING_2 + i);
            pwgt = PageFramCurWgtByIndex(11+i);
            WGTSetAttrByName(pwgt, "text", d_TempCtrlComm1_MOLDSET_DATEMP_MAX_2 + i);
            pwgt = PageFramCurWgtByIndex(21+i);
            WGTSetAttrByName(pwgt, "text", d_TempCtrlComm1_MOLDSET_DATEMP_MIN_2 + i);
            pwgt = PageFramCurWgtByIndex(10001+i);
            WGTSetAttrByName(pwgt, "text", d_TempCtrlComm1_MOLDSET_DACENT_HEATERON_2 + i);
            pwgt = PageFramCurWgtByIndex(41+i);
            WGTSetAttrByName(pwgt, "text", d_TempCtrlComm1_MACHSET_TM_RESPONSELESSCYCLE_2 + i);
            pwgt = PageFramCurWgtByIndex(51+i);
            WGTSetAttrByName(pwgt, "text", d_TempCtrlComm1_MACHSET_DATEMP_RAMP_2 + i);
            pwgt = PageFramCurWgtByIndex(61+i);
            WGTSetAttrByName(pwgt, "text", d_TempCtrlComm1_MACHSET_DAPID_KP_2 + i);
            pwgt = PageFramCurWgtByIndex(71+i);
            WGTSetAttrByName(pwgt, "text", d_TempCtrlComm1_MACHSET_DAPID_TI_2 + i);
            pwgt = PageFramCurWgtByIndex(81+i);
            WGTSetAttrByName(pwgt, "text", d_TempCtrlComm1_MACHSET_DAPID_TD_2 + i);
            pwgt = PageFramCurWgtByIndex(91+i);
            WGTSetAttrByName(pwgt, "value", d_TempCtrlComm1_MACHSET_DATEMP_FUNC_2 + i);
            pwgt = PageFramCurWgtByIndex(101+i);
            WGTSetAttrByName(pwgt, "text", d_TempCtrlComm1_STATE_OUTPUT_PER_2 + i);
        }
        WGTSetAttrByName(ppg->tabpage1_Tb1_stbt_warn_func, "value", d_TempCtrlComm1_MOLDSET_FL_TEMPHOLD);
        WGTSetAttrByName(ppg->tabpage1_Tb1_stbt_sync, "value", d_TempCtrlComm1_MOLDSET_CONFUNC);
        WGTSetAttrByName(ppg->tabpage1_Tb1_ed_warm_temp, "text", d_TempCtrlComm1_MOLDSET_DATEMP_HOLD);
        WGTSetAttrByName(ppg->tabpage1_Tb1_ed_per_tm, "text", d_TempCtrlComm1_MOLDSET_TM_HEATERWARM);
        //WGTSetAttrByName(ppg->tabpage1_Tb5_ed_per_tm, "text", d_TempCtrlComm1_MOLDSET_TM_HEATERWARM);
        WGTSetAttrByName(ppg->tabpage1_Tb1_stbt_no_check, "value", d_TempCtrlComm1_MACHSET_FL_TEMPWORDUPCHECK);
        WGTSetAttrByName(ppg->tabpage1_Tb1_edit96, "text", p_PP_STATE_remainMin);
        WGTSetAttrByName(ppg->tabpage1_Tb1_edit97, "text", p_PP_STATE_remainSec);

        WGTSetAttrByName(ppg->tabpage1_Tb5_stbt_cool, "value", d_TempCtrlComm1_MOLDSET_FL_TEMPCOOL);
        WGTSetAttrByName(ppg->tabpage1_Tb5_ed_temp_seg, "text", d_TempCtrlComm1_MACHSET_DATEMP_CH_NUM);
        WGTSetAttrByName(ppg->tabpage1_Tb5_ed_scan_cyc, "text", d_TempCtrlComm1_MACHSET_DATEMP_SCTM_MIN);
        WGTSetAttrByName(ppg->tabpage1_Tb5_ed_cool_max, "text", d_TempCtrlComm1_MACHSET_DATEMP_TEMPCOOLERON);
        WGTSetAttrByName(ppg->tabpage1_Tb5_ed_cool_min, "text", d_TempCtrlComm1_MACHSET_DATEMP_TEMPCOOLEROFF);
        WGTSetAttrByName(ppg->tabpage1_Tb5_edit_m_temp, "text", d_TempCtrlComm1_STATE_DATEMP_CHARGEPOS);
        WGTSetAttrByName(ppg->tabpage1_Tb5_edit_m_temp_max, "text", d_TempCtrlComm1_MACHSET_TEMPCOOLMAX);
        WGTSetAttrByName(ppg->tabpage1_Tb5_edit_m_temp_on, "text", d_TempCtrlComm1_MACHSET_DATEMP_TEMPCOOLERONMOUTH);
        WGTSetAttrByName(ppg->tabpage1_Tb5_edit_m_temp_off, "text", d_TempCtrlComm1_MACHSET_DATEMP_TEMPCOOLEROFFMOUTH);
    }
        SegSelect();
}

/**
* @brief     :段数选择后没用到的段数默认置为关闭
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200317
*/
static void SegFuncSelect()
{
    int i;
    if(VarAdrToInt(p_PP_MACHSET_E502_TEMP_AB) == 1)
    {
        for(i = 0; i < 6; ++i)
        {
            if(getbit(VarAdrToInt(TMP_SELECT), i) == 0)
            {
                if(VarAdrToInt(d_TempCtrlComm2_MACHSET_DATEMP_FUNC_5+i) != 0)//20220712 dyl
                    VarSendSaveSetIntByAdr(d_TempCtrlComm2_MACHSET_DATEMP_FUNC_5+i, 0);
            }
        }
    }
    else
    {
        for(i = 0; i < 6; ++i)
        {
            if(getbit(VarAdrToInt(TMP_SELECT), i) == 0)
            {
                if(VarAdrToInt(d_TempCtrlComm1_MACHSET_DATEMP_FUNC_5+i) != 0)//20220712 dyl
                    VarSendSaveSetIntByAdr(d_TempCtrlComm1_MACHSET_DATEMP_FUNC_5+i, 0);
            }
        }
    }
}

/**
* @brief     :段数选择
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191127
*/
static void SegSelect()
{
    UI16 seg;
    UI16 bit = 0;
    VarAdrSetInt(TMP_SELECT, 0);
    if(VarAdrToInt(p_PP_MACHSET_E502_TEMP_AB) == 1)
    {
        seg = VarAdrToInt(d_TempCtrlComm2_MACHSET_DATEMP_CH_NUM);
    }
    else
    {
        seg = VarAdrToInt(d_TempCtrlComm1_MACHSET_DATEMP_CH_NUM);
    }

    switch (seg) {
    case 5:
        setone(bit, 0);
        break;
    case 6:
        setone(bit, 0);
        setone(bit, 1);
        break;
    case 7:
        setone(bit, 0);
        setone(bit, 1);
        setone(bit, 2);
        break;
    case 8:
        setone(bit, 0);
        setone(bit, 1);
        setone(bit, 2);
        setone(bit, 3);
        break;
    case 9:
        setone(bit, 0);
        setone(bit, 1);
        setone(bit, 2);
        setone(bit, 3);
        setone(bit, 4);
        break;
    case 10:
        setone(bit, 0);
        setone(bit, 1);
        setone(bit, 2);
        setone(bit, 3);
        setone(bit, 4);
        setone(bit, 5);
        break;
    default:
        break;
    }
//    if(VarAdrToInt(p_PP_STATE_TEMP_CTRL_PART) > 1)
//    {
//        if(VarAdrToInt(p_PP_MACHSET_E502_TEMP_AB) == 1)
//        {
//            if(GetMotorState())
//            {
//                setzero(bit, seg-5);
//                setone(bit, 6);
//            }
//        }
//    }
//    else
//    {
//        if(GetMotorState())
//        {
//            setzero(bit, seg-5);
//            setone(bit, 6);
//        }
//    }
    VarAdrSetInt(TMP_SELECT, bit);
    SegFuncSelect();
    if(VarAdrToInt(p_PP_MACHSET_E502_TEMP_AB) == 1)
        VarAdrSetInt(B_PAGE_SEL, 0xffff & bit);
    else
        VarAdrSetInt(A_PAGE_SEL, 0xffff & bit);
}

static void PageIn(Temp_xx_PG* ppg)
{
    int i;
    VarAdrSetInt(p_PP_TMP_TMPVAL100, GetWeekData());
    if(GetCurrentUserId() < 2)
    {
        TabPageSetSubVisble(ppg->tabpage1, 2, FALSE);
        TabPageSetSubVisble(ppg->tabpage1, 3, FALSE);
        TabPageSetSubVisble(ppg->tabpage1, 4, FALSE);
        TabPageSetSubVisble(ppg->tabpage1, 5, FALSE);
    }
    else
    {
        TabPageSetSubVisble(ppg->tabpage1, 2, TRUE);
        TabPageSetSubVisble(ppg->tabpage1, 3, TRUE);
        TabPageSetSubVisble(ppg->tabpage1, 4, TRUE);
        TabPageSetSubVisble(ppg->tabpage1, 5, TRUE);
    }

    for(i = 0; i < 9; ++i)
    {
        if(VarAdrToInt(d_machine1_MACHSET_FL_SERVOMODE) & 0x04)//温度控制+2改为code16+4 20250310 jhh
        {
            if(VarAdrToInt(d_TempCtrlComm1_MOLDSET_DATEMP_SETTING_2 + i) > 350)
                VarSendSaveSetIntByAdr(d_TempCtrlComm1_MOLDSET_DATEMP_SETTING_2 + i, 350);
            if(VarAdrToInt(d_TempCtrlComm2_MOLDSET_DATEMP_SETTING_2 + i) > 350)
                VarSendSaveSetIntByAdr(d_TempCtrlComm2_MOLDSET_DATEMP_SETTING_2 + i, 350);
        }
        TempCtlFunc(i, 0);
    }

    //20241017 chj 金鹰特殊要求，预温计时增加密码
    if(((VarAdrToInt(d_machine1_MACHSET_FL_CUSTOMID)&0xFF00) == 0x6400))
    {
        WGTSetVisible(ppg->tabpage1_Tb1_edpsw,TRUE);
        if(VarAdrToInt(p_PP_MACHSET_SYS_CLAMP_PROTECT)==1)//密码保护功能使用
        {
            VarAdrSetInt(TMP_PREHEAT_PSW_VISIBLE,1);
            VarAdrSetInt(TMP_PREHEAT_PSW,0);
            TempCtlFuncReadOnly(ppg->tabpage1_Tb1_ed_per_tm,true);
        }
        else
        {
            VarAdrSetInt(TMP_PREHEAT_PSW_VISIBLE,0);
            VarAdrSetInt(TMP_PREHEAT_PSW,0);
            TempCtlFuncReadOnly(ppg->tabpage1_Tb1_ed_per_tm,false);
        }
    }
    else
    {
        WGTSetVisible(ppg->tabpage1_Tb1_edpsw,FALSE);
        VarAdrSetInt(TMP_PREHEAT_PSW_VISIBLE,0);
        VarAdrSetInt(TMP_PREHEAT_PSW,0);
        TempCtlFuncReadOnly(ppg->tabpage1_Tb1_ed_per_tm,false);
    }


}
static const char* TempRealStr(char* buf, UI32 adr, UI16 size)
{
    UI32 num;
    num = VarAdrToInt(adr);
    if(num == 999){
        if(GetTextTran(TEXT_HEATALARM999) != NULL)
            strncpy(buf, GetTextTran(TEXT_HEATALARM999), size);
    }
    else if(num == 777){
        if(GetTextTran(TEXT_HEATALARM777) != NULL)
            strncpy(buf, GetTextTran(TEXT_HEATALARM777), size);
    }
    else if(num == 788){
        if(GetTextTran(TEXT_HEATALARM788) != NULL)
            strncpy(buf, GetTextTran(TEXT_HEATALARM788), size);
    }
    else if(num == 779){
        if(GetTextTran(TEXT_HEATALARM779) != NULL)
            strncpy(buf, GetTextTran(TEXT_HEATALARM779), size);
    }
    else {
        snprintf(buf, size-1, "%d", num);
    }
    return buf;
}
static void TempRealShowUpdate()
{
    char buf[20]= "";
    UI16 i, size;
    UI16 oil, motor, mouth;
    size = sizeof(buf)-1;
    for(i = 0; i < 9; ++i){
        if(VarAdrToInt(p_PP_MACHSET_E502_TEMP_AB)){
            strncpy(buf, TempRealStr(buf, d_TempCtrlComm2_STATE_DATEMP_REAL_2 + i, size), size);
            VarAdrSetStr(TMP_REAL2 + i, buf);
        }
        else{
            strncpy(buf, TempRealStr(buf, d_TempCtrlComm1_STATE_DATEMP_REAL_2 + i, size), size);
            VarAdrSetStr(TMP_REAL1 + i, buf);
        }
    }
    oil = VarAdrToInt(d_oil1_STATE_DATEMP_OIL);
    motor = VarAdrToInt(d_motor1_STATE_DATEMP_REALMOTOR);
    mouth = VarAdrToInt(d_TempCtrlComm1_STATE_DATEMP_CHARGEPOS);
    if(oil == 999 || oil == 777 || oil == 788 || oil == 779)
        WGTSetVisible(temp_xx_pg.subpage21_oilunit, FALSE);
    else
        WGTSetVisible(temp_xx_pg.subpage21_oilunit, TRUE);
    if(GetMotorState())
    {
        if(motor == 999 || motor == 777 || motor == 788 || motor == 779)
            WGTSetVisible(temp_xx_pg.subpage20_motorunit, FALSE);
        else
            WGTSetVisible(temp_xx_pg.subpage20_motorunit, TRUE);
    }
    else
    {
        WGTSetVisible(temp_xx_pg.subpage20_motorunit, FALSE);
    }
    if(VarAdrToInt(p_PP_OBJ_MOUTH_VISIBLE))
    {
        if(mouth == 999 || mouth == 777 || mouth == 788 || mouth == 779)
            WGTSetVisible(temp_xx_pg.subpage22_mouth_unit, FALSE);
        else
            WGTSetVisible(temp_xx_pg.subpage22_mouth_unit, TRUE);
    }
    else
    {
        WGTSetVisible(temp_xx_pg.subpage22_mouth_unit, FALSE);
    }
    strncpy(buf, TempRealStr(buf, d_oil1_STATE_DATEMP_OIL, size), size);
    VarAdrSetStr(TMP_OIL, buf);
    strncpy(buf, TempRealStr(buf, d_motor1_STATE_DATEMP_REALMOTOR, size), size);
    VarAdrSetStr(TMP_MOTOR, buf);
    strncpy(buf, TempRealStr(buf, d_TempCtrlComm1_STATE_DATEMP_CHARGEPOS, size), size);
    VarAdrSetStr(TMP_MOUTH, buf);
}
//油温预热启动文本显示
static void BtPrHeatTextShow()
{
    if(VarAdrToInt(d_nozzle1_ACTPARA_FL_OILHEAT))
    {
        if((VarAdrToInt(p_PP_STATE_ActName1) == 0x11171000) || (VarAdrToInt(p_PP_STATE_ActName2) == 0x11171000) ||
                (VarAdrToInt(p_PP_STATE_ActName3) == 0x11171000) || (VarAdrToInt(p_PP_STATE_ActName4) == 0x11171000))
        {
            ButtonSetText(temp_xx_pg.tabpage1_Tb3_btprheat, GetTextTran(TEXT_STOP));
        }
        else
        {
            ButtonSetText(temp_xx_pg.tabpage1_Tb3_btprheat, GetTextTran(TEXT_CLAMP_START));
        }
    }
    else
    {
        ButtonSetText(temp_xx_pg.tabpage1_Tb3_btprheat, GetTextTran(TEXT_CLAMP_START));
    }
}

static void CtlConnect()
{
}
static int Temp_xxOnInit(Temp_xx_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Temp_xxOnShow(Temp_xx_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_USERLOCK);//20210914 dyl touch

    VarAdrSetInt(p_PP_MACHSET_E502_TEMP_AB, 0);
    PageIn(ppg);
    InitWarmData();
    MotorTemp(ppg);
    TempOptPageIn(ppg);
    SegSelect();
    SwitchAB(ppg);
    FkeyTab(ppg->tabpage1, GUI_KEY_FUN6);
    TabPageSelPage(ppg->tabpage1, 0);
    return 0;
}
static int Temp_xxOnHide(Temp_xx_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch
    return 0;
}
static int Temp_xxOnUpdate(Temp_xx_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    TempOptUpdate(ppg);
    TempRealShowUpdate();
    BtPrHeatTextShow();
    return 0;
}
static int Temp_xxOnChange(Temp_xx_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 value;
    UI16 wpos;
    UI32 index,addr;
    PATTR pattr;
    UI32 wgt_index;
    UI16 i;

    pattr = (PATTR)msg_para;

    addr = OBJAttrGetAdr(pattr);

    if(addr>=TMP_NHOUR && addr<=TMP_NHOUR+TMP_ADDR_OFFSET) //0~6
    {
        index = addr - TMP_NHOUR;
        value = VarAdrToUI16(TMP_NHOUR+index);
        dbwarmx.dbwarmconfig[index].nHour = value;
    }
    else if(addr>=TMP_NMINUTE && addr<=TMP_NMINUTE+TMP_ADDR_OFFSET) //7~13
    {
        index = addr - TMP_NMINUTE;
        value = VarAdrToUI16(TMP_NMINUTE+index);
        dbwarmx.dbwarmconfig[index].nMinute = value;
    }
    else if(addr>=TMP_BWARM && addr<=TMP_BWARM+TMP_ADDR_OFFSET) //14~20
    {
        index = addr - TMP_BWARM;
        value = VarAdrToUI16(TMP_BWARM+index);
        dbwarmx.dbwarmconfig[index].bWarm = value;
    }
    else if(addr>=TMP_NCLSHOUR && addr<=TMP_NCLSHOUR+TMP_ADDR_OFFSET) //21~27
    {
        index = addr - TMP_NCLSHOUR;
        value = VarAdrToUI16(TMP_NCLSHOUR+index);
        dbwarmx.dbwarmconfig[index].nClsHour = value;
    }
    else if(addr>=TMP_NCLSMIN && addr<=TMP_NCLSMIN+TMP_ADDR_OFFSET) //28~34
    {
        index = addr - TMP_NCLSMIN;
        value = VarAdrToUI16(TMP_NCLSMIN+index);
        dbwarmx.dbwarmconfig[index].nClsMin = value;
    }
    else if(addr>=TMP_BWARMCLS && addr<=TMP_BWARMCLS+TMP_ADDR_OFFSET) //35~41
    {
        index = addr - TMP_BWARMCLS;
        value = VarAdrToUI16(TMP_BWARMCLS+index);
        dbwarmx.dbwarmconfig[index].bWarmCls = value;
    }
    wpos =(UI16)((UI32)&dbwarmx.dbwarmconfig[index]-(UI32)&dbwarmx);
    SaveWarmSet(wpos,&dbwarmx.dbwarmconfig[index],sizeof(dbwarmx.dbwarmconfig[index]));


    TempOptChange(addr, pwgt, ppg);

    wgt_index = WGTGetGOffset(pwgt);
    if(wgt_index > 90 && wgt_index < 100)
    {
        //20200812
        if(VarAdrToInt(d_TempCtrlComm1_STATE_FL_HEATERSTATUS))
        {
            VarSendSaveSetIntByAdr(addr, msg_value);
            ShowMsg(VM_MSG_CLOSEHEATER);
            return 0;
        }

        TempCtlFunc(wgt_index-91, msg_value);
        MotorTemp(ppg);
    }

    if(pwgt == ppg->tabpage1_Tb5_ed_temp_seg)
    {
        SegSelect();
        for(i=0;i<9;i++)
        {
            TempCtlFunc(i, 0);
        }
        MotorTemp(ppg);
    }
    else if(pwgt == ppg->stbtAB)
    {
        SwitchAB(ppg);
        for(i=0;i<9;i++)
        {
            TempCtlFunc(i, 0);
        }
    }
    else if(pwgt == ppg->tabpage1_Tb3_btprheat)
    {
        if(OperateModeIndex() != 4)//MODE_MACROADJ
        {
            ShowMsg(VM_MSG_CHANGETOADJMOLD);
            return -1;
        }
        SendKey(_KEY_PANEL_OILHEAT);
        SendKey(KEY_RELEASE);
    }
    else if(pwgt == ppg->btxCURV)
    {
        PanelShowPageByName(PAGE_TEMP_CURVE);
    }
    else if(pwgt == ppg->tabpage1_Tb5_ed_scan_cyc)//20221114 dyl
    {
        if(VarAdrToInt(d_TempCtrlComm1_STATE_FL_HEATERSTATUS))
        {
            VarSendSaveSetIntByAdr(addr, msg_value);
            ShowMsg(VM_MSG_CLOSEHEATER);
            return 0;
        }
    }
    else if(pwgt == ppg->tabpage1_Tb1_edpsw)//20241017 chj 金鹰特殊要求，预温计时增加密码
    {
        Cal_Clamp_Prs_Max_Protect_PSW(TMP_PREHEAT_PSW_CAL);
        if(VarAdrToInt(TMP_PREHEAT_PSW) == VarAdrToInt(TMP_PREHEAT_PSW_CAL))
        {
            TempCtlFuncReadOnly(ppg->tabpage1_Tb1_ed_per_tm,false);
        }
        else
        {
            TempCtlFuncReadOnly(ppg->tabpage1_Tb1_ed_per_tm,true);
        }
    }

    return 0;
}
