#include "WDayPower.h"
#include "typedef.h"
#include "pagefram.h"
#include "prodpower.h"
#include "file.h"
#include "time.h"
#include "panelrec.h"
#include "usermod.h"//20210914 dyl touch

DEFINE_DAYPOWER_MAP

CONNECT_DAYPOWER_EVENT

#define  TMP_MONTH      TMPVAL(210)
#define  TMP_DAY        TMPVAL(211)
#define  TMP_PASSWORD   TMPVAL(212)
#define  TMP_YEAR       TMPVAL(213)//20250318 jhh 跨年产量能耗数据保存
#define  TMP_YEAR_MAX   TMPVAL(214)
#define  TMP_YEAR_MIN   TMPVAL(215)
#define  DAY_MAX        TMPVAL(300)

#define  TMP_OPENCNT1   TMPVAL(1)
#define  TMP_POWER1     TMPVAL(31)

static void ShowDayPower()
{
    int i;
    UI32 month, day, year;

    month = VarAdrToInt(TMP_MONTH);
    day = VarAdrToInt(TMP_DAY);
    year = VarAdrToInt(TMP_YEAR);//20250318 jhh 跨年产量能耗数据保存

    struct tm* t;
    time_t time_now;
    time(&time_now);
    t = localtime(&time_now);
    UI32 cur_year;
    cur_year = t->tm_year;
    UI32 hour_opencnt, hour_power;

    if(month >= 1 && month <= 12 && day >= 1 && day <= 31)
    {
        for(i = 0; i < DAY_HOUR; i++)
        {
            hour_opencnt = 0;
            hour_power = 0;
            if(year == cur_year + 1900 - 1)//画面选择查看年份等于机器年份的去年
            {
                hour_opencnt = g_G10ProdCntLast.open_cnt[month-1][day-1][i];
                hour_power = g_G10ProdCntLast.power[month-1][day-1][i];
            }
            else if(year == cur_year + 1900)//画面选择查看年份等于机器年份
            {
                hour_opencnt = g_G10ProdCnt.open_cnt[month-1][day-1][i];
                hour_power = g_G10ProdCnt.power[month-1][day-1][i];
            }
            VarAdrSetInt(TMP_OPENCNT1+i, hour_opencnt);
            VarAdrSetInt(TMP_POWER1+i, hour_power);
        }
    }
    else
    {
        return;
    }

}

//设置日的最大值
static void SetMaxDay()
{
    BOOL bLeapYear;
    UI16 cur_month,cur_year;
    const UI16 s_daysInMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    struct tm* t;
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
}

static void CtlConnect()
{
}
static int DayPowerOnInit(DayPower_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int DayPowerOnShow(DayPower_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_USERLOCK);//20210914 dyl touch

    struct tm* t;
    time_t time_now;
    time(&time_now);
    t = localtime(&time_now);

    VarAdrSetInt(TMP_MONTH, t->tm_mon+1);
    VarAdrSetInt(TMP_DAY, t->tm_mday);
    VarAdrSetInt(TMP_YEAR, t->tm_year+1900);//20250318 jhh 跨年产量能耗数据保存
    VarAdrSetInt(TMP_YEAR_MAX, t->tm_year+1900);
    VarAdrSetInt(TMP_YEAR_MIN, t->tm_year+1900-1);

    VarAdrSetInt(TMP_PASSWORD, 1234);
    WGTSetEnable(ppg->button8, FALSE);
    SetMaxDay();

    ShowDayPower();

    return 0;
}
static int DayPowerOnHide(DayPower_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch
    return 0;
}
static int DayPowerOnUpdate(DayPower_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    ShowDayPower();

    return 0;
}
static int DayPowerOnChange(DayPower_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
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
        SetMaxDay();
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
