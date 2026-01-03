#include "WYearPower.h"
#include "typedef.h"
#include "pagefram.h"
#include "prodpower.h"
#include "file.h"
#include "time.h"
#include "panelrec.h"
#include "usermod.h"//20210914 dyl touch

DEFINE_YEARPOWER_MAP

CONNECT_YEARPOWER_EVENT

#define  TMP_PASSWORD   TMPVAL(212)
#define  TMP_YEAR       TMPVAL(213)//20250318 jhh 跨年产量能耗数据保存
#define  TMP_YEAR_MAX   TMPVAL(214)
#define  TMP_YEAR_MIN   TMPVAL(215)

#define  TMP_OPENCNT1   TMPVAL(1)
#define  TMP_POWER1     TMPVAL(31)

static void ShowYearPower()
{
    int i, j, k;
    UI32 mouth_opencnt, day_opencnt, hour_opencnt;
    UI32 mouth_power, day_power, hour_power;
    struct tm* t;
    time_t time_now;
    time(&time_now);
    t = localtime(&time_now);
    UI32 cur_year,year;//20250318 jhh 跨年产量能耗数据保存
    cur_year = t->tm_year;
    year = VarAdrToInt(TMP_YEAR);

    for(i = 0; i < YEAR_MON; ++i)
    {
        mouth_opencnt = 0;
        mouth_power = 0;
        for(j = 0; j < MON_DAY; ++j)
        {
            day_opencnt = 0;
            day_power = 0;
            for(k = 0; k < DAY_HOUR; ++k)
            {
                //20250318 jhh 跨年产量能耗数据保存
                if(year == cur_year + 1900 - 1)//画面选择查看年份等于机器年份的去年
                {
                    hour_opencnt = g_G10ProdCntLast.open_cnt[i][j][k];
                    day_opencnt += hour_opencnt;

                    hour_power = g_G10ProdCntLast.power[i][j][k];
                    day_power += hour_power;
                }
                else if(year == cur_year + 1900)//画面选择查看年份等于机器年份
                {
                    hour_opencnt = g_G10ProdCnt.open_cnt[i][j][k];
                    day_opencnt += hour_opencnt;

                    hour_power = g_G10ProdCnt.power[i][j][k];
                    day_power += hour_power;
                }
            }
            mouth_opencnt += day_opencnt;
            mouth_power += day_power;
        }

        VarAdrSetInt(TMP_OPENCNT1+i, mouth_opencnt);
        VarAdrSetInt(TMP_POWER1+i, mouth_power);
    }
}

static void UpdateCurYearPower()//只更新当前月
{
    int i, j;
    BOOL bLeapYear;
    UI16 cur_year,cur_month,cur_day,show_day,year;
    UI32 mouth_opencnt, day_opencnt, hour_opencnt;
    UI32 mouth_power, day_power, hour_power;
    const UI16 s_daysInMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    struct tm* t;
    time_t time_now;
    time(&time_now);
    t = localtime(&time_now);

    cur_year = t->tm_year+1900;
    cur_month = t->tm_mon+1;
    cur_day = t->tm_mday;
    year = VarAdrToInt(TMP_YEAR);//20250318 jhh 跨年产量能耗数据保存

    if(g_G10ProdCnt.year != (VarAdrToInt(TMP_YEAR) - 1900))
    {
        return;
    }

    if ((year%4 == 0) && (year%100 != 0) || (year%400 == 0))//20250318 jhh 跨年产量能耗数据保存
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

    mouth_opencnt = 0;
    mouth_power = 0;
    for(i = 0; i < show_day; ++i)
    {
        day_opencnt = 0;
        day_power = 0;
        for(j = 0; j < DAY_HOUR; ++j)
        {
            hour_opencnt = g_G10ProdCnt.open_cnt[cur_month-1][i][j];
            day_opencnt += hour_opencnt;

            hour_power = g_G10ProdCnt.power[cur_month-1][i][j];
            day_power += hour_power;
        }
        mouth_opencnt += day_opencnt;
        mouth_power += day_power;
    }

    VarAdrSetInt(TMP_OPENCNT1+cur_month-1, mouth_opencnt);
    VarAdrSetInt(TMP_POWER1+cur_month-1, mouth_power);
}

static void CtlConnect()
{
}
static int YearPowerOnInit(YearPower_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int YearPowerOnShow(YearPower_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_USERLOCK);//20210914 dyl touch

    //20250318 jhh 跨年产量能耗数据保存
    struct tm* t;
    time_t time_now;
    time(&time_now);
    t = localtime(&time_now);
    VarAdrSetInt(TMP_YEAR, t->tm_year+1900);
    VarAdrSetInt(TMP_YEAR_MAX, t->tm_year+1900);
    VarAdrSetInt(TMP_YEAR_MIN, t->tm_year+1900-1);

    VarAdrSetInt(TMP_PASSWORD, 1234);
    WGTSetEnable(ppg->button8, FALSE);

    ShowYearPower();
    return 0;
}
static int YearPowerOnHide(YearPower_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch
    return 0;
}
static int YearPowerOnUpdate(YearPower_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UpdateCurYearPower();

    return 0;
}
static int YearPowerOnChange(YearPower_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
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
    case TMP_YEAR://20250318 jhh 跨年产量能耗数据保存
        ShowYearPower();
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
                ShowYearPower();
            }
        }
        break;
    }
    return 0;
}
