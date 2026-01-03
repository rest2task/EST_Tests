#include "WMonthPower.h"
#include "typedef.h"
#include "pagefram.h"
#include "prodpower.h"
#include "file.h"
#include "time.h"
#include "panelrec.h"
#include "usermod.h"//20210914 dyl touch

DEFINE_MONTHPOWER_MAP

CONNECT_MONTHPOWER_EVENT

#define  TMP_MONTH      TMPVAL(210)
#define  TMP_PASSWORD   TMPVAL(212)
#define  TMP_YEAR       TMPVAL(213)//20250318 jhh 跨年产量能耗数据保存
#define  TMP_YEAR_MAX   TMPVAL(214)
#define  TMP_YEAR_MIN   TMPVAL(215)

#define  TMP_OPENCNT1   TMPVAL(1)
#define  TMP_POWER1     TMPVAL(51)

#define  TMP_DAY29SHOW  TMPVAL(260) //29天可见
#define  TMP_DAY30SHOW  TMPVAL(261) //30天可见
#define  TMP_DAY31SHOW  TMPVAL(262) //31天可见

static void ShowMonPower()
{
    int i, j;
    BOOL bLeapYear;
    UI16 cur_month,cur_year,show_day,year;
    UI32 day_opencnt, hour_opencnt;
    UI32 day_power, hour_power;
    const UI16 s_daysInMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    struct tm* t;
    time_t time_now;
    time(&time_now);
    t = localtime(&time_now);

    cur_month = VarAdrToInt(TMP_MONTH);
    cur_year = t->tm_year;
    year = VarAdrToInt(TMP_YEAR);//20250318 jhh 跨年产量能耗数据保存

    if((year%4 == 0) && (year%100 != 0) || (year%400 == 0))
    {
        bLeapYear = TRUE;
    }
    else
    {
        bLeapYear = FALSE;
    }

    if(bLeapYear && (2 == cur_month))
    {
        show_day = 29;
    }
    else
    {
        show_day = s_daysInMonth[cur_month-1];
    }

    if(show_day >= 31)
    {
        VarAdrSetInt(TMP_DAY29SHOW, TRUE);
        VarAdrSetInt(TMP_DAY30SHOW, TRUE);
        VarAdrSetInt(TMP_DAY31SHOW, TRUE);
    }
    else if(show_day >= 30)
    {
        VarAdrSetInt(TMP_DAY29SHOW, TRUE);
        VarAdrSetInt(TMP_DAY30SHOW, TRUE);
        VarAdrSetInt(TMP_DAY31SHOW, FALSE);
    }
    else if(show_day >= 29)
    {
        VarAdrSetInt(TMP_DAY29SHOW, TRUE);
        VarAdrSetInt(TMP_DAY30SHOW, FALSE);
        VarAdrSetInt(TMP_DAY31SHOW, FALSE);
    }
    else
    {
        VarAdrSetInt(TMP_DAY29SHOW, FALSE);
        VarAdrSetInt(TMP_DAY30SHOW, FALSE);
        VarAdrSetInt(TMP_DAY31SHOW, FALSE);
    }

    //20250318 jhh 跨年产量能耗数据保存
    if(cur_month >= 1 && cur_month <= 12)
    {
        for(i = 0; i < show_day; ++i)
        {
            day_opencnt = 0;
            day_power = 0;
            for(j = 0; j < DAY_HOUR; ++j)
            {
                if(year == cur_year + 1900 - 1)//画面选择查看年份等于机器年份的去年
                {
                    hour_opencnt = g_G10ProdCntLast.open_cnt[cur_month-1][i][j];
                    day_opencnt += hour_opencnt;

                    hour_power = g_G10ProdCntLast.power[cur_month-1][i][j];
                    day_power += hour_power;
                }
                else if(year == cur_year + 1900)//画面选择查看年份等于机器年份
                {
                    hour_opencnt = g_G10ProdCnt.open_cnt[cur_month-1][i][j];
                    day_opencnt += hour_opencnt;

                    hour_power = g_G10ProdCnt.power[cur_month-1][i][j];
                    day_power += hour_power;
                }
            }
            VarAdrSetInt(TMP_OPENCNT1+i, day_opencnt);
            VarAdrSetInt(TMP_POWER1+i, day_power);
        }
    }
    else
    {
        return;
    }
}

static void UpdateCurMonPower()//只更新当前日
{
    int i;
    UI16 cur_month,cur_day;
    UI32 day_opencnt, hour_opencnt,cur_year;//20250318 jhh 跨年产量能耗数据保存;
    UI32 day_power, hour_power;
    struct tm* t;
    time_t time_now;
    time(&time_now);
    t = localtime(&time_now);

    cur_month = t->tm_mon+1;
    cur_day = t->tm_mday;
    cur_year = t->tm_year+1900;//20250318 jhh 跨年产量能耗数据保存

    if(cur_month != VarAdrToInt(TMP_MONTH) || g_G10ProdCnt.year != (VarAdrToInt(TMP_YEAR) - 1900))//20250318 jhh 跨年产量能耗数据保存
    {
        return;
    }

    day_opencnt = 0;
    day_power = 0;
    for(i = 0; i < DAY_HOUR; ++i)
    {
        hour_opencnt = g_G10ProdCnt.open_cnt[cur_month-1][cur_day-1][i];
        day_opencnt += hour_opencnt;

        hour_power = g_G10ProdCnt.power[cur_month-1][cur_day-1][i];
        day_power += hour_power;
    }
    VarAdrSetInt(TMP_OPENCNT1+cur_day-1, day_opencnt);
    VarAdrSetInt(TMP_POWER1+cur_day-1, day_power);
}

static void CtlConnect()
{
}
static int MonthPowerOnInit(MonthPower_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int MonthPowerOnShow(MonthPower_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_USERLOCK);//20210914 dyl touch

    struct tm* t;
    time_t time_now;
    time(&time_now);
    t = localtime(&time_now);

    VarAdrSetInt(TMP_MONTH, t->tm_mon+1);
    VarAdrSetInt(TMP_YEAR, t->tm_year+1900);//20250318 jhh 跨年产量能耗数据保存
    VarAdrSetInt(TMP_YEAR_MAX, t->tm_year+1900);
    VarAdrSetInt(TMP_YEAR_MIN, t->tm_year+1900-1);

    VarAdrSetInt(TMP_PASSWORD, 1234);
    WGTSetEnable(ppg->button8, FALSE);

    ShowMonPower();
    return 0;
}
static int MonthPowerOnHide(MonthPower_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch
    return 0;
}
static int MonthPowerOnUpdate(MonthPower_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UpdateCurMonPower();

    return 0;
}
static int MonthPowerOnChange(MonthPower_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
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
    case TMP_MONTH:
    case TMP_YEAR://20250318 jhh 跨年产量能耗数据保存
        ShowMonPower();
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
                ShowMonPower();
            }
        }
        break;
    }
    return 0;
}
