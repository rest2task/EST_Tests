#include "WDayOutput.h"
#include "typedef.h"
#include "pagefram.h"
#include "prodpower.h"
#include "file.h"
#include "time.h"
#include "stdio.h"
#include "panelrec.h"
#include "usermod.h"//20210914 dyl touch

DEFINE_DAYOUTPUT_MAP

CONNECT_DAYOUTPUT_EVENT

#define  TMP_PASSWORD   TMPVAL(3)
#define  TMP_MONTH      TMPVAL(10)
#define  TMP_DAY        TMPVAL(11)
#define  DAY_PROD_CNT   TMPVAL(12)
#define  TMP_HOUR0      TMPVAL(200)
#define  DAY_MAX        TMPVAL(9)
#define  TMP_DATE       TMPSTR(11)
#define  TMP_YEAR       TMPVAL(13)
#define  TMP_YEAR_MAX   TMPVAL(14)
#define  TMP_YEAR_MIN   TMPVAL(15)

void ShowDayRecord()
{
    int i;
    UI32 year,month, day;
    UI32 per_hour, all_day;
    all_day = 0;
    year = VarAdrToInt(TMP_YEAR);
    month = VarAdrToInt(TMP_MONTH);
    day = VarAdrToInt(TMP_DAY);

    //20250318 jhh 跨年产量能耗数据保存
    struct tm* t;
    time_t time_now;
    time(&time_now);
    t = localtime(&time_now);
    UI32 cur_year;
    cur_year = t->tm_year;

    if(month >= 1 && month <= 12 && day >= 1 && day <= 31)
    {
        for(i = 0; i < 24; ++i)
        {
            per_hour = 0;
            if(year == cur_year + 1900 - 1)//画面选择查看年份等于机器年份的去年
            {
                per_hour = g_G10ProdCntLast.prod_cnt[month-1][day-1][i];
                all_day += per_hour;
            }
            else if(year == cur_year + 1900)//画面选择查看年份等于机器年份
            {
                per_hour = g_G10ProdCnt.prod_cnt[month-1][day-1][i];
                all_day += per_hour;
            }
            VarAdrSetInt(TMP_HOUR0+i, per_hour);
        }
    }
    else
    {
        return;
    }
    VarAdrSetInt(DAY_PROD_CNT, all_day);
}

//设置日的最大值
static void SetMaxDay()
{
    BOOL bLeapYear;
    UI16 cur_month,cur_year;
    const UI16 s_daysInMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    struct tm* t;
    char date[20];
    time_t time_now;
    time(&time_now);
    t = localtime(&time_now);

    cur_month = VarAdrToInt(TMP_MONTH);
    //cur_year = t->tm_year+1900;
    cur_year = VarAdrToInt(TMP_YEAR);//20250318 jhh 跨年产量能耗数据保存

    if ((cur_year%4 == 0) && (cur_year%100 != 0) || (cur_year%400 == 0))
    {
        bLeapYear = TRUE;
    }
    else
    {
        bLeapYear = FALSE;
    }

    if(bLeapYear && (2 == cur_month))
    {
        VarAdrSetInt(DAY_MAX, 29);
    }
    else
    {
        VarAdrSetInt(DAY_MAX, s_daysInMonth[cur_month-1]);
    }

    if(VarAdrToInt(TMP_DAY) > VarAdrToInt(DAY_MAX))
    {
        VarAdrSetInt(TMP_DAY, VarAdrToInt(DAY_MAX));
    }

    snprintf(date, sizeof(date), "%d-%d-%d", cur_year, cur_month, VarAdrToInt(TMP_DAY));
    VarAdrSetStr(TMP_DATE, date);
}

static void CtlConnect()
{
}
static int DayOutputOnInit(DayOutput_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int DayOutputOnShow(DayOutput_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_USERLOCK);//20210914 dyl touch

    struct tm* t;
    time_t time_now;
    time(&time_now);
    t = localtime(&time_now);

    if(VarAdrToInt(d_machine1_MOLDSET_SRV1))
    {
        WGTSetVisible(ppg->label33, FALSE);
        WGTSetVisible(ppg->button8, FALSE);
        WGTSetVisible(ppg->edit74, FALSE);
    }
    else
    {
        WGTSetVisible(ppg->label33, TRUE);
        WGTSetVisible(ppg->button8, TRUE);
        WGTSetVisible(ppg->edit74, TRUE);

        VarAdrSetInt(TMP_PASSWORD, 1234);
        WGTSetEnable(ppg->button8, FALSE);
    }

    VarAdrSetInt(TMP_YEAR, t->tm_year+1900);
    VarAdrSetInt(TMP_MONTH, t->tm_mon+1);
    VarAdrSetInt(TMP_DAY, t->tm_mday);

    VarAdrSetInt(TMP_YEAR_MAX, t->tm_year+1900);//20250318 jhh 跨年产量能耗数据保存
    VarAdrSetInt(TMP_YEAR_MIN, t->tm_year+1900-1);

    SetMaxDay();//20200107.cyx

    ShowDayRecord();
    return 0;
}
static int DayOutputOnHide(DayOutput_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch
    return 0;
}
static int DayOutputOnUpdate(DayOutput_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    ShowDayRecord();

    return 0;
}
static int DayOutputOnChange(DayOutput_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 addr;
    PATTR pattr;

    struct tm* t;
    time_t time_now;
    time(&time_now);
    t = localtime(&time_now);

    pattr = (PATTR)msg_para;

    addr = OBJAttrGetAdr(pattr);

    switch (addr)
    {
    case TMP_PASSWORD:
        if (VarAdrToInt(TMP_PASSWORD) == 9595)
        {
            WGTSetEnable(ppg->button8, TRUE);
        }
        else
        {
            WGTSetEnable(ppg->button8, FALSE);
        }
        break;
    case TMP_YEAR:
    case TMP_MONTH:
    case TMP_DAY:
        SetMaxDay();//20200107.cyx
        break;
    default :
        if(pwgt == ppg->button8)
        {
            PanelRecAdd(pwgt, PNL_CLEAR_ALL_REC, 0);
            if(VarAdrToInt(TMP_YEAR) == t->tm_year + 1900)//20250318 jhh 跨年产量能耗数据保存 根据选择年份清空对应文件
            {
                G10ClearAll();
            }
            else
            {
                G10LastClearAll();
            }
        }
        break;
    }
    return 0;
}
