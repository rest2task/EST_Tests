#include "WMonthOutput.h"
#include "typedef.h"
#include "pagefram.h"
#include "prodpower.h"
#include "file.h"
#include "time.h"
#include "stdio.h"
#include "panelrec.h"
#include "usermod.h"//20210914 dyl touch

DEFINE_MONTHOUTPUT_MAP

CONNECT_MONTHOUTPUT_EVENT

#define  TMP_PASSWORD   TMPVAL(3)
#define  TMP_MONTH      TMPVAL(10)
#define  TMP_HISTOGRAM_CNT        TMPVAL(11)
#define  MONTH_PROD_CNT   TMPVAL(12)
#define  TMP_DAY1      TMPVAL(200)
#define  TMP_DATE      TMPSTR(11)
#define  TMP_YEAR      TMPVAL(20)//20250318 jhh 跨年产量能耗数据保存
#define  TMP_YEAR_MAX  TMPVAL(21)
#define  TMP_YEAR_MIN  TMPVAL(22)


void ShowMonRecord()
{
    struct tm* t;
    char date[20];
    time_t time_now;
    time(&time_now);
    t = localtime(&time_now);

    int i, j;
    BOOL bLeapYear;
    UI16 cur_month,cur_year,year;
    UI32 month_cnt, day_cnt, hour_cnt;
    const UI16 s_daysInMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    month_cnt = 0;
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
        VarAdrSetInt(TMP_HISTOGRAM_CNT, 29);
    }
    else
    {
        VarAdrSetInt(TMP_HISTOGRAM_CNT, s_daysInMonth[cur_month-1]);
    }

    //20250318 jhh 跨年产量能耗数据保存
    if(cur_month >= 1 && cur_month <= 12)
    {
        for(i = 0; i < 31; ++i)
        {
            day_cnt = 0;
            for(j = 0; j < 24; ++j)
            {
                if(year == cur_year + 1900 - 1)//画面选择查看年份等于机器年份的去年
                {
                    hour_cnt = g_G10ProdCntLast.prod_cnt[cur_month-1][i][j];
                    day_cnt += hour_cnt;
                }
                else if(year == cur_year + 1900)//画面选择查看年份等于机器年份
                {
                    hour_cnt = g_G10ProdCnt.prod_cnt[cur_month-1][i][j];
                    day_cnt += hour_cnt;
                }
            }
            VarAdrSetInt(TMP_DAY1+i, day_cnt);
            month_cnt += day_cnt;
        }
    }
    else
    {
        return;
    }

    VarAdrSetInt(MONTH_PROD_CNT, month_cnt);

    snprintf(date, sizeof(date), "%d-%d", year, cur_month);
    VarAdrSetStr(TMP_DATE, date);
}

static void CtlConnect()
{
}
static int MonthOutputOnInit(MonthOutput_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int MonthOutputOnShow(MonthOutput_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
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

    VarAdrSetInt(TMP_MONTH, t->tm_mon+1);
    VarAdrSetInt(TMP_YEAR, t->tm_year+1900);//20250318 jhh 跨年产量能耗数据保存
    VarAdrSetInt(TMP_YEAR_MAX, t->tm_year+1900);
    VarAdrSetInt(TMP_YEAR_MIN, t->tm_year+1900-1);
    ShowMonRecord();
    return 0;
}
static int MonthOutputOnHide(MonthOutput_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch
    return 0;
}
static int MonthOutputOnUpdate(MonthOutput_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    ShowMonRecord();

    return 0;
}
static int MonthOutputOnChange(MonthOutput_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
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
