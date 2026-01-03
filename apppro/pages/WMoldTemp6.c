#include "WMoldTemp6.h"
#include "typedef.h"
#include "pagefram.h"
#include "usermod.h"
#include "dbenum.h"
#include "editwgt.h"
#include "database.h"
#include "texttbl.h"
#include "privdef.h"
#include "key.h"
#include "keymod.h"
#include "temp.h"
#include "tabpagewgt.h"
#include "ctrlcmd.h"
#include "protcmd.h"
#include "information.h"
#include "oprintf.h"
#include "stdio.h"
#include "editwgt.h"
#include "labelwgt.h"
#include "usermod.h"//20210914 dyl touch

DEFINE_MOLDTEMP6_MAP

CONNECT_MOLDTEMP6_EVENT

//前缀"TMP"表示临时变量
#define TMP_TEMP_SEG_SEL  TMPVAL(88)   //温度选择段数
#define TMP_TEMP_RT_VALUE TMPSTR(110)  //温度实时值显示
#define TMP_ROOM_TEMP     TMPVAL(55)

#define TEMP_CTRL_FUNC    0x2f630010 //温控功能
#define TEMP_SEG_CNT      0x2f6300b4 //温度段数
#define TEMP_REAL_VALUE   0x2f660002 //温度实际值
#define TEMP_CTRL_STATUS  0x2f660012 //温控状态
#define MT_ROOM_TMP       0x2f660032 //室温的温度

#define TEMP_CTRL_STATUS1_IMG   TMPVAL(300)    /*温度控制状态1图片*/

static const int MAX_MWSEG_CNT = 8;               //温度最大段数
static const int MAX_MWCTRL_WGT_CNT = 9;          //温控功能相关控件最大行数量
static BOOL b_switchopt = FALSE;

static void TempCtlWGTShow(PWGT ppg,BOOL isWGTEnable, BOOL isVisible,BOOL isReadOnly)
{
    WGTSetEnable(ppg, isWGTEnable);
    if(isVisible)
    {
        EditSetBKColor(ppg, 0xFFFFFF);
        EditSetTextColor(ppg, 0x000000);
    }
    else
    {
        EditSetBKColor(ppg, 0xdedede);
        EditSetTextColor(ppg, 0x000000);
    }

    EditSetReadOnly(ppg, isReadOnly);
}

static void TempCtlFunc(UI16 seg)
{
    UI16 fun_selt;
    PWGT pwgt;
    int i;
    fun_selt = VarAdrToInt(TEMP_CTRL_FUNC + seg);

    switch (fun_selt) {
    case 0://close
        for(i=0; i<MAX_MWCTRL_WGT_CNT; i++)
        {
            pwgt = PageFramCurWgtByIndex(i*MAX_MWSEG_CNT+1+seg);
            TempCtlWGTShow(pwgt,FALSE,FALSE,TRUE);
        }
        break;
    case 1:
        for(i=0; i<MAX_MWCTRL_WGT_CNT; i++)
        {
            pwgt = PageFramCurWgtByIndex(i*MAX_MWSEG_CNT+1+seg);
            TempCtlWGTShow(pwgt,FALSE,FALSE,TRUE);
        }
        break;
    case 2:
        for(i=0; i<3; i++)
        {
            pwgt = PageFramCurWgtByIndex(i*MAX_MWSEG_CNT+1+seg);
            TempCtlWGTShow(pwgt,FALSE,FALSE,TRUE);
        }
        pwgt = PageFramCurWgtByIndex(3*8+1+seg);
        TempCtlWGTShow(pwgt, TRUE,TRUE,FALSE);

        pwgt = PageFramCurWgtByIndex(4*8+1+seg);
        TempCtlWGTShow(pwgt, TRUE,TRUE,FALSE);

        for(i=5; i<MAX_MWCTRL_WGT_CNT; i++)
        {
            pwgt = PageFramCurWgtByIndex(i*MAX_MWSEG_CNT+1+seg);
            TempCtlWGTShow(pwgt, FALSE,FALSE,TRUE);
        }
        break;
    case 3:
    case 4:
        for(i=0; i<MAX_MWCTRL_WGT_CNT; i++)
        {
            pwgt = PageFramCurWgtByIndex(i*MAX_MWSEG_CNT+1+seg);
            TempCtlWGTShow(pwgt, TRUE,TRUE,FALSE);
        }
        break;
    default:
        break;
    }
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

    for(i = 0; i < 3; ++i)
    {
        if(getbit(VarAdrToInt(TMP_TEMP_SEG_SEL), i) == 0)
        {
            if(VarAdrToInt(d_TempCtrlMW6_MOLDSET_DATEMP_FUNC_MW5+i) != 0)//20220712 dyl
                VarSendSaveSetIntByAdr(d_TempCtrlMW6_MOLDSET_DATEMP_FUNC_MW5+i, 0);
        }
    }
}

static void SegSelect()
{
    UI16 seg;
    UI16 bit = 0;
    VarAdrSetInt(TMP_TEMP_SEG_SEL, 0);

    seg = VarAdrToInt(TEMP_SEG_CNT);

    switch (seg) {
    case 5:
        break;
    case 6:
        setone(bit, 0);
        break;
    case 7:
        setone(bit, 0);
        setone(bit, 1);
        break;
    case 8:
        setone(bit, 0);
        setone(bit, 1);
        setone(bit, 2);
        break;
    default:
        break;
    }

    VarAdrSetInt(TMP_TEMP_SEG_SEL, bit);

    SegFuncSelect();
}

static const char* TempRealStr(char* buf, UI32 adr, UI16 size)
{
    UI32 num;
    num = VarAdrToInt(adr);
    if(num == 999)
    {
        if(GetTextTran(TEXT_HEATALARM999) != NULL)
            strncpy(buf, GetTextTran(TEXT_HEATALARM999), size);
    }
    else if(num == 777)
    {
        if(GetTextTran(TEXT_HEATALARM777) != NULL)
            strncpy(buf, GetTextTran(TEXT_HEATALARM777), size);
    }
    else if(num == 788)
    {
        if(GetTextTran(TEXT_HEATALARM788) != NULL)
            strncpy(buf, GetTextTran(TEXT_HEATALARM788), size);
    }
    else if(num == 779){
        if(GetTextTran(TEXT_HEATALARM779) != NULL)
            strncpy(buf, GetTextTran(TEXT_HEATALARM779), size);
    }
    else
    {
        snprintf(buf, size-1, "%d", num);
    }
    return buf;
}

static void TempRealShowUpdate()
{
    char buf[20]= "";
    UI16 i, size;
    size = sizeof(buf)-1;

    for(i=0; i<MAX_MWSEG_CNT; i++)
    {
        strncpy(buf, TempRealStr(buf, TEMP_REAL_VALUE + i, size), size);
        VarAdrSetStr(TMP_TEMP_RT_VALUE + i, buf);
    }
}

static void TempCtlStatusVal()
{
    UI16 tempseg;
    UI16 i, j;

    tempseg = VarAdrToInt(TEMP_SEG_CNT);

    for(i=0; i<tempseg; i++)
    {
        if((VarAdrToInt(TEMP_CTRL_STATUS + i) & 0x00ff) == 0)
        {
            if(VarAdrToInt(TEMP_CTRL_FUNC + i) == 0)
                VarAdrSetInt(TEMP_CTRL_STATUS1_IMG+i, 6);
            else
                VarAdrSetInt(TEMP_CTRL_STATUS1_IMG+i, 1);
        }
        else
        {
            if(VarAdrToInt(TEMP_CTRL_FUNC + i) == 0)
                VarAdrSetInt(TEMP_CTRL_STATUS1_IMG+i, 6);
            for(j=0; j<5; j++)
            {
                if((VarAdrToInt(TEMP_CTRL_STATUS + i) >> j) & 0x1)
                    VarAdrSetInt(TEMP_CTRL_STATUS1_IMG+i, j+1);
            }
        }
    }
}

static void ShowRoomTemp(MoldTemp6_PG* ppg)
{
    if(VarAdrToInt(MT_ROOM_TMP)>0x8000)
    {
        VarAdrSetInt(TMP_ROOM_TEMP,VarAdrToInt(MT_ROOM_TMP)-0x8000);
        WGTSetVisible(ppg->lable_add, FALSE);
        WGTSetVisible(ppg->lable_sub, TRUE);
    }
    else
    {
        VarAdrSetInt(TMP_ROOM_TEMP,VarAdrToInt(MT_ROOM_TMP));
        WGTSetVisible(ppg->lable_add, TRUE);
        WGTSetVisible(ppg->lable_sub, FALSE);
    }
}

static void ShowAlarm(MoldTemp6_PG* ppg)
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

    if(VarAdrToInt(d_TempCtrlMW1_STATE_DATEMP_YOUHUAXUQIU) || VarAdrToInt(d_TempCtrlMW2_STATE_DATEMP_YOUHUAXUQIU)
            || VarAdrToInt(d_TempCtrlMW3_STATE_DATEMP_YOUHUAXUQIU) || VarAdrToInt(d_TempCtrlMW4_STATE_DATEMP_YOUHUAXUQIU)
            || VarAdrToInt(d_TempCtrlMW5_STATE_DATEMP_YOUHUAXUQIU) || VarAdrToInt(d_TempCtrlMW6_STATE_DATEMP_YOUHUAXUQIU)
            || VarAdrToInt(d_TempCtrlMW7_STATE_DATEMP_YOUHUAXUQIU))
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

static void SetValMax(UI16 seg)
{
    PWGT pwgt;
    pwgt = PageFramCurWgtByIndex(1+seg);//索引号1为温度一段设定值

    if(VarAdrToInt(d_TempCtrlMW6_MOLDSET_FL_TKTJ_MW0 + seg))//J型上限350
    {
         EditSetMax(pwgt, 350);
    }
    else//K型上限450
    {
         EditSetMax(pwgt, 450);
    }

}

static void CtlConnect()
{
}
static int MoldTemp6OnInit(MoldTemp6_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int MoldTemp6OnShow(MoldTemp6_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_USERLOCK);//20210914 dyl touch

    int i;

    VarAdrSetInt(p_PP_TMP_TMPVAL100, GetWeekData());//time show

    if(GetCurrentUserId() < 2)
    {
        TabPageSetSubVisble(ppg->tabpage1, 1, FALSE);
        TabPageSetSubVisble(ppg->tabpage1, 2, FALSE);
    }
    else
    {
        TabPageSetSubVisble(ppg->tabpage1, 1, TRUE);
        TabPageSetSubVisble(ppg->tabpage1, 2, TRUE);
    }

    for(i=0; i<MAX_MWSEG_CNT; i++)
    {
        TempCtlFunc(i);
        SetValMax(i);
    }

    SegSelect();
    FkeyTab(ppg->tabpage1, GUI_KEY_FUN6);
    TabPageSelPage(ppg->tabpage1, 0);

    return 0;
}
static int MoldTemp6OnHide(MoldTemp6_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch
    return 0;
}
static int MoldTemp6OnUpdate(MoldTemp6_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    TempRealShowUpdate();
    TempCtlStatusVal();
    ShowRoomTemp(ppg);
    ShowAlarm(ppg);

    return 0;
}
static int MoldTemp6OnChange(MoldTemp6_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 index,addr;
    PATTR pattr;
    int i;

    index = WGTGetGOffset(pwgt);

    pattr = (PATTR)msg_para;
    addr = OBJAttrGetAdr(pattr);

    if(index > 90 && index < 100)
    {
        if(GetMtpTempHeatState())//20200812
        {
            VarSendSaveSetIntByAdr(addr, msg_value);
            ShowMsg(VM_MSG_CLOSESPARE2);
            return 0;
        }
        TempCtlFunc(index-91);
    }
    else if(index >= 73 && index <= 80)//热电偶类型
    {
        SetValMax(index-73);
    }

    if (pwgt == ppg->tabpage1_Tb3_ed_temp_seg)
    {
        SegSelect();

        for(i=0; i<MAX_MWSEG_CNT; i++)
        {
            TempCtlFunc(i);
        }
    }

    return 0;
}
