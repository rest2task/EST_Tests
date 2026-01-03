#include "prodpower.h"
#include "file.h"
#include "time.h"
#include "panelcfg.h"
#include "poweroff.h"
#include "alarmrecord.h"
#include "oprintf.h"

//20190403 hz

DB_DAYPOWER g_dbDayPower[24];
DB_MONTHPOWER g_dbMonthPower[31];
DB_YEARPOWER g_dbYearPower[12];
DB_HEATERSET g_dbHeaterSet;
G10PRODCNT g_G10ProdCnt;
G10PRODCNT g_G10ProdCntLast;//20250318 jhh 跨年产量能耗数据保存
DB_POWER g_dbPower;//20201104
static BOOL bCurrentCheck = TRUE; //电表数据检测

DB_HEATERSET default_HeaterSet = {
    {2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500},//01~20 加热圈16个的功率  wHeatPower 1~16
    {10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10} //21-30 加热圈16段系数    wHeatFactor 1~16
};

//产量写入
void WriteProdPower(const char* filename, void* src, UI16 size)
{
    FILEHD filehd;
    if(PanelCfgStyle() == G_TYPE)
        return;
    filehd = FileOpen(filename);
    if(!FILEHD_IS_OK(filehd))
    {
        filehd = FileCreate(filename);
        FileSeek(filehd, 0);
        FileWrite(filehd, src, size, NULL);
    }
    else
    {
        FileSeek(filehd, 0);
        FileWrite(filehd, src, size, NULL);
    }
    FileClose(filehd);
}

//产量读入
void ReadProdPower(const char* filename, void* dst, void* def, UI16 size)
{
    FILEHD filehd;

    filehd = FileOpen(filename);
    if(!FILEHD_IS_OK(filehd))
    {
        filehd = FileCreate(filename);
        if(def == NULL){
            memset(dst, 0, size);
        }
        else{
            memcpy(dst, def, size);
        }
        FileSeek(filehd, 0);
        FileWrite(filehd, dst, size, NULL);
    }
    if(FILEHD_IS_OK(filehd))
    {
        FileRead(filehd, dst, size, NULL);
    }
    FileClose(filehd);
}

/***************G10-产量功能*************************************************************************/
void G10SaveProdCnt()
{
    FILEHD filehd;

    if(PanelCfgStyle() != G_TYPE)
        return;

    filehd = FileOpen(G10_PRODCNT_FILE);
    if(!FILEHD_IS_OK(filehd))
    {
        filehd = FileCreate(G10_PRODCNT_FILE);
        FileSeek(filehd, 0);
        FileWrite(filehd, &g_G10ProdCnt, sizeof(G10PRODCNT), NULL);
    }
    else
    {
        FileSeek(filehd, 0);
        FileWrite(filehd, &g_G10ProdCnt, sizeof(G10PRODCNT), NULL);
    }
    FileClose(filehd);
}

//20250318 jhh 跨年产量能耗数据保存
void G10SaveProdCntLast()
{
    FILEHD filehd;

    if(PanelCfgStyle() != G_TYPE)
        return;

    filehd = FileOpen(G10_PRODCNT_LAST_FILE);
    if(!FILEHD_IS_OK(filehd))
    {
        filehd = FileCreate(G10_PRODCNT_LAST_FILE);
        FileSeek(filehd, 0);
        FileWrite(filehd, &g_G10ProdCntLast, sizeof(G10PRODCNT), NULL);
    }
    else
    {
        FileSeek(filehd, 0);
        FileWrite(filehd, &g_G10ProdCntLast, sizeof(G10PRODCNT), NULL);
    }
    FileClose(filehd);
}

void G10LoadProdCnt()
{
    FILEHD filehd;

    if(PanelCfgStyle() != G_TYPE)
        return;

    filehd = FileOpen(G10_PRODCNT_FILE);
    if(!FILEHD_IS_OK(filehd))
    {
        filehd = FileCreate(G10_PRODCNT_FILE);
        memset(&g_G10ProdCnt, 0, sizeof(G10PRODCNT));
        FileSeek(filehd, 0);
        FileWrite(filehd, &g_G10ProdCnt, sizeof(G10PRODCNT), NULL);
    }
    if(FILEHD_IS_OK(filehd))
    {
        FileRead(filehd, &g_G10ProdCnt, sizeof(G10PRODCNT), NULL);
    }
    FileClose(filehd);
}

//20250318 jhh 跨年产量能耗数据保存
void G10LoadProdCntLast()
{
    FILEHD filehd;

    if(PanelCfgStyle() != G_TYPE)
        return;

    filehd = FileOpen(G10_PRODCNT_LAST_FILE);
    if(!FILEHD_IS_OK(filehd))
    {
        filehd = FileCreate(G10_PRODCNT_LAST_FILE);
        memset(&g_G10ProdCntLast, 0, sizeof(G10PRODCNT));
        FileSeek(filehd, 0);
        FileWrite(filehd, &g_G10ProdCntLast, sizeof(G10PRODCNT), NULL);
    }
    if(FILEHD_IS_OK(filehd))
    {
        FileRead(filehd, &g_G10ProdCntLast, sizeof(G10PRODCNT), NULL);
    }
    FileClose(filehd);
}

/***********************************
* cur_hour: current hour [0-23]
* cur_day: current day   [1-31]
* cur_month: current month  [0-11]
* per_prod: product nums per mod
************************************/
void G10ProdCnt(UI16 cur_hour, UI16 cur_day, UI16 cur_month, UI16 cur_year, UI16 per_prod)
{
    if(PanelCfgStyle() != G_TYPE)
        return;
    if(g_G10ProdCnt.year != cur_year)//年份不同则清空产量数据 //20201104
    {
        //20250318 jhh 跨年产量能耗数据保存 只对向后跨年做文件拷贝处理，其余情况无操作
        if(g_G10ProdCnt.year < cur_year)//正常年份向后跨一年(2024->2025)
        {
            memcpy(&g_G10ProdCntLast, &g_G10ProdCnt, sizeof(G10PRODCNT));
            G10SaveProdCntLast();
            memset(&g_G10ProdCnt, 0, sizeof(G10PRODCNT));
        }
        else
        {
            ;
        }
        g_G10ProdCnt.year = cur_year;
        G10SaveProdCnt();
    }

    g_G10ProdCnt.prod_cnt[cur_month][cur_day-1][cur_hour] += per_prod;
    g_G10ProdCnt.open_cnt[cur_month][cur_day-1][cur_hour] += 1;//20201104
}

void G10ClearOneDay(UI16 cur_month, UI16 cur_day)
{
    int hour;
    if(cur_month >= YEAR_MON || cur_day >= MON_DAY)
        return;

    for(hour = 0; hour < DAY_HOUR; ++hour)
        g_G10ProdCnt.prod_cnt[cur_month][cur_day][hour] = 0;
    G10SaveProdCnt();
}

void G10ClearOneMonth(UI16 cur_month)
{
    int day, hour;
    if(cur_month >= YEAR_MON)
        return;

    for(day = 0; day < MON_DAY; ++day)
    {
        for(hour = 0; hour < DAY_HOUR; ++hour)
            g_G10ProdCnt.prod_cnt[cur_month][day][hour] = 0;
    }
    G10SaveProdCnt();
}

void G10ClearAll()
{
    memset(&g_G10ProdCnt, 0, sizeof(G10PRODCNT));
    struct tm* t;//20230518 chj 优化能耗表 此处需要赋年份
    time_t time_now;
    time(&time_now);
    t = localtime(&time_now);
    g_G10ProdCnt.year = t->tm_year;
    G10SaveProdCnt();
}

//20250318 jhh 跨年产量能耗数据保存 清除历年产量文件
void G10LastClearAll()
{
    memset(&g_G10ProdCntLast, 0, sizeof(G10PRODCNT));
    struct tm* t;//20230518 chj 优化能耗表 此处需要赋年份
    time_t time_now;
    time(&time_now);
    t = localtime(&time_now);
    g_G10ProdCntLast.year = t->tm_year - 1;
    G10SaveProdCntLast();
}

/***************G10-产量功能-END*************************************************************************/

//产量初始化
void InitProdPower()
{
    if(PanelCfgStyle() == G_TYPE)//20201104
    {
        G10LoadProdCnt();
        G10LoadProdCntLast();//20250318 jhh 跨年产量能耗数据保存
    }
    else
    {
        ReadProdPower(DAYPOWERFILE, g_dbDayPower, 0, sizeof (DB_DAYPOWER)*DAY_HOUR);
        ReadProdPower(MONTHPOWERFILE, g_dbMonthPower, 0, sizeof (DB_MONTHPOWER)*MON_DAY);
        ReadProdPower(YEARPOWERFILE, g_dbYearPower, 0, sizeof (DB_YEARPOWER)*YEAR_MON);
        ReadProdPower(HEATERSETFILE, &g_dbHeaterSet, &default_HeaterSet, sizeof (DB_HEATERSET));
    }

    LoadPowerMeter();//20230518 chj 假如断电保护文件有问题，需要面板定时保存下电表能耗数据
}

/***********************************
* cur_hour: current hour [0-23]
* cur_day: current day   [1-31]
* per_mod: mod nums per refresh
* per_prod: product nums per mod
************************************/
void DayProdPower(UI16 cur_hour, UI16 cur_day, UI16 per_mod, UI16 per_prod, UI32 cur_min_power, UI32 power_cycle)
{
    //according to demand, onle record current day's data
    if(g_dbDayPower[0].DayFlag != cur_day)
    {
        memset(g_dbDayPower, 0, sizeof (DB_DAYPOWER)*DAY_HOUR);
        g_dbDayPower[0].DayFlag = cur_day;
    }
    else {
        g_dbDayPower[0].DayFlag = cur_day;
    }
    g_dbDayPower[cur_hour].dwCount = g_dbDayPower[cur_hour].dwCount + per_mod;
    g_dbDayPower[cur_hour].dwProduct = g_dbDayPower[cur_hour].dwProduct + per_prod;
    g_dbDayPower[cur_hour].dwPower = g_dbDayPower[cur_hour].dwPower + power_cycle + cur_min_power;
}

/***********************************
* cur_day: current day  [1-31]
* cur_month: current month  [0-11]
* per_mod: mod nums per refresh
* per_prod: product nums per mod
************************************/
void MonthProdPower(UI16 cur_day, UI16 cur_month, UI16 per_mod, UI16 per_prod, UI32 cur_min_power, UI32 power_cycle)
{
    //according to demand, onle record current month's data
    if(g_dbMonthPower[0].MonthFlag != cur_month)
    {
        memset(g_dbMonthPower, 0, sizeof (DB_MONTHPOWER)*MON_DAY);
        g_dbMonthPower[0].MonthFlag = cur_month;
    }
    else {
        g_dbMonthPower[0].MonthFlag = cur_month;
    }
    g_dbMonthPower[cur_day-1].dwCount = g_dbMonthPower[cur_day-1].dwCount + per_mod;
    g_dbMonthPower[cur_day-1].dwProduct = g_dbMonthPower[cur_day-1].dwProduct + per_prod;
    g_dbMonthPower[cur_day-1].dwPower = g_dbMonthPower[cur_day-1].dwPower + power_cycle + cur_min_power;
}

/***********************************
* cur_month: current month  [0-11]
* cur_year: current year   [1900 - ]
* per_mod: mod nums per refresh
* per_prod: product nums per mod
************************************/
void YearProdPower(UI16 cur_month, UI16 cur_year, UI16 per_mod, UI16 per_prod, UI32 cur_min_power, UI32 power_cycle)
{
    //according to demand, onle record current year's data
    if(g_dbYearPower[0].YearFlag != cur_year)
    {
        memset(g_dbYearPower, 0, sizeof (DB_YEARPOWER)*YEAR_MON);
        g_dbYearPower[0].YearFlag = cur_year;
    }
    else{
        g_dbYearPower[0].YearFlag = cur_year;
    }
    g_dbYearPower[cur_month].dwCount = g_dbYearPower[cur_month].dwCount + per_mod;
    g_dbYearPower[cur_month].dwProduct = g_dbYearPower[cur_month].dwProduct + per_prod;
    g_dbYearPower[cur_month].dwPower = g_dbYearPower[cur_month].dwPower + power_cycle + cur_min_power;
}

// 1min heater power 20190418 hz
void GetHeaterPower()
{
	UI16 cur_hour, cur_day, cur_month, cur_year;
	int i;
	UI32 cur_min_power = 0;
    static UI32 time_6000ms = 0;

	struct tm* t;
	time_t time_now;

    if(PanelCfgStyle() == G_TYPE)
        return;

    time(&time_now);
    t = localtime(&time_now);
    cur_hour = t->tm_hour;
	cur_day = t->tm_mday;
	cur_month = t->tm_mon;
	cur_year = t->tm_year;

    if(GetTick() - time_6000ms >= 6000)
	{
		for(i = 0; i < 16; ++i)
        {
            //20230518 chj 计算的过程中，超出UI32，目前的计算方式，存在1s的偏差
            cur_min_power += VarAdrToInt(DATEMP_WORKTM1+i) * g_dbHeaterSet.wHeatPower[i]/1000* g_dbHeaterSet.wHeatFactor[i]/60/60;
		}
		time_6000ms = GetTick();
	}

	YearProdPower(cur_month, cur_year, 0, 0, cur_min_power, 0);
	MonthProdPower(cur_day, cur_month, 0, 0, cur_min_power, 0);
	DayProdPower(cur_hour, cur_day, 0, 0, cur_min_power, 0);
}

//refresh data and save data
void ProdPower()
{
    UI16 cur_hour, cur_day, cur_month, cur_year;
    UI16 per_prod;
	UI32 power_cycle;
    const UI16 per_mod = 1; //every mod refresh
    
    static UI16 s_hour = 0;
    static UI16 s_day = 0;
    static UI16 s_month = 0;
    static UI16 s_year = 0;

    struct tm* t;
    time_t time_now;
    time(&time_now);
    t = localtime(&time_now);

    per_prod = VarAdrToUI16(PROD_PERCNT);
    cur_hour = t->tm_hour;
    cur_day = t->tm_mday;
    cur_month = t->tm_mon;
    cur_year = t->tm_year;

    if(PanelCfgStyle() != G_TYPE)
    {
        power_cycle = VarAdrToInt(POWER_CYCLE)/10;
        YearProdPower(cur_month, cur_year, per_mod, per_prod, 0, power_cycle);
        MonthProdPower(cur_day, cur_month, per_mod, per_prod, 0, power_cycle);
        DayProdPower(cur_hour, cur_day, per_mod, per_prod, 0, power_cycle);
    }
    else if(PanelCfgStyle() == G_TYPE)
    {
        G10ProdCnt(cur_hour, cur_day, cur_month, cur_year, per_prod);
    }

    if((s_hour != cur_hour) || (s_day != cur_day) || (s_month != cur_month) || (s_year != cur_year))
    {
        if(PanelCfgStyle() != G_TYPE)
        {
            WriteProdPower(DAYPOWERFILE, g_dbDayPower, sizeof (DB_DAYPOWER)*DAY_HOUR);
            WriteProdPower(MONTHPOWERFILE, g_dbMonthPower, sizeof (DB_MONTHPOWER)*MON_DAY);
            WriteProdPower(YEARPOWERFILE, g_dbYearPower, sizeof (DB_YEARPOWER)*YEAR_MON);
        }
        else if(PanelCfgStyle() == G_TYPE)
        {
            G10SaveProdCnt();
        }
        s_hour = cur_hour;
        s_day = cur_day;
        s_month = cur_month;
        s_year = cur_year;

        g_cur_hour = cur_hour;
        g_cur_day = cur_day;
        g_cur_month = cur_month;
    }
}

//choose date[DAY, MON, YEAR]
void ClearPower(int date)
{
    UI16 cur_day, cur_month;
    UI32 count, product, power;
    int i;
    struct tm* t;
    time_t time_now;
    time(&time_now);
    t = localtime(&time_now);

    cur_day = t->tm_mday;
    cur_month = t->tm_mon;

    count = 0;
    product = 0;
    power = 0;

    switch (date) {
    case DAY:
        for (i = 0; i < DAY_HOUR; ++i) {
            count += g_dbDayPower[i].dwCount;
            product += g_dbDayPower[i].dwProduct;
            power += g_dbDayPower[i].dwPower;
        }
        memset(g_dbDayPower, 0, sizeof (DB_DAYPOWER)*DAY_HOUR);
        //mon
        if(g_dbMonthPower[cur_day-1].dwCount >= count)
        {
            g_dbMonthPower[cur_day-1].dwCount -= count;
        }
        else {
            g_dbMonthPower[cur_day-1].dwCount = 0;
        }
        if(g_dbMonthPower[cur_day-1].dwProduct >= product)
        {
            g_dbMonthPower[cur_day-1].dwProduct -= product;
        }
        else {
            g_dbMonthPower[cur_day-1].dwProduct = 0;
        }
        if(g_dbMonthPower[cur_day-1].dwPower >= power)
        {
            g_dbMonthPower[cur_day-1].dwPower -= power;
        }
        else {
            g_dbMonthPower[cur_day-1].dwPower = 0;
        }
        //year
        if(g_dbYearPower[cur_month].dwCount >= count)
        {
            g_dbYearPower[cur_month].dwCount -= count;
        }
        else {
            g_dbYearPower[cur_month].dwCount = 0;
        }
        if(g_dbYearPower[cur_month].dwProduct >= product)
        {
            g_dbYearPower[cur_month].dwProduct -= product;
        }
        else {
            g_dbYearPower[cur_month].dwProduct = 0;
        }
        if(g_dbYearPower[cur_month].dwPower >= power)
        {
            g_dbYearPower[cur_month].dwPower -= power;
        }
        else {
            g_dbYearPower[cur_month].dwPower = 0;
        }

        WriteProdPower(DAYPOWERFILE, g_dbDayPower, sizeof (DB_DAYPOWER)*DAY_HOUR);
        WriteProdPower(MONTHPOWERFILE, g_dbMonthPower, sizeof (DB_MONTHPOWER)*MON_DAY);
        WriteProdPower(YEARPOWERFILE, g_dbYearPower, sizeof (DB_YEARPOWER)*YEAR_MON);
        break;
    case MON:
        for (i = 0; i < MON_DAY; ++i) {
            count += g_dbMonthPower[i].dwCount;
            product += g_dbMonthPower[i].dwProduct;
            power += g_dbMonthPower[i].dwPower;
        }
        memset(g_dbDayPower, 0, sizeof (DB_DAYPOWER)*DAY_HOUR);
        memset(g_dbMonthPower, 0, sizeof (DB_MONTHPOWER)*MON_DAY);

        if(g_dbYearPower[cur_month].dwCount >= count)
        {
            g_dbYearPower[cur_month].dwCount -= count;
        }
        else {
            g_dbYearPower[cur_month].dwCount = 0;
        }
        if(g_dbYearPower[cur_month].dwProduct >= product)
        {
            g_dbYearPower[cur_month].dwProduct -= product;
        }
        else {
            g_dbYearPower[cur_month].dwProduct = 0;
        }
        if(g_dbYearPower[cur_month].dwPower >= power)
        {
            g_dbYearPower[cur_month].dwPower -= power;
        }
        else {
            g_dbYearPower[cur_month].dwPower = 0;
        }

        WriteProdPower(DAYPOWERFILE, g_dbDayPower, sizeof (DB_DAYPOWER)*DAY_HOUR);
        WriteProdPower(MONTHPOWERFILE, g_dbMonthPower, sizeof (DB_MONTHPOWER)*MON_DAY);
        WriteProdPower(YEARPOWERFILE, g_dbYearPower, sizeof (DB_YEARPOWER)*YEAR_MON);
        break;
    case YEAR:
        memset(g_dbDayPower, 0, sizeof (DB_DAYPOWER)*DAY_HOUR);
        memset(g_dbMonthPower, 0, sizeof (DB_MONTHPOWER)*MON_DAY);
        memset(g_dbYearPower, 0, sizeof (DB_YEARPOWER)*YEAR_MON);

        WriteProdPower(DAYPOWERFILE, g_dbDayPower, sizeof (DB_DAYPOWER)*DAY_HOUR);
        WriteProdPower(MONTHPOWERFILE, g_dbMonthPower, sizeof (DB_MONTHPOWER)*MON_DAY);
        WriteProdPower(YEARPOWERFILE, g_dbYearPower, sizeof (DB_YEARPOWER)*YEAR_MON);
        break;
    default:
        break;
    }
}

/**
 * @brief      : 能耗数据处理
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20201104
 */
void ProductPowerData()
{
    UI16 cur_hour, cur_day, cur_month, cur_year;
    static UI32 dwSaveCurrentPower = 0;//已保存能耗
    static UI32 dwCurrentPower = 0;//当前能耗
    long dwCurrentMinPower;//未保存能耗 //20230518 chj 优化能耗表，后续要对大小判断

    struct tm* t;
    time_t time_now;

    //20230518 chj 优化能耗表，开机时候，主机先传电压，再传电表总计，所以电表总计为0，也不做处理
    if((VarAdrToInt(d_machine1_STATE_VOL_A) == 0) || (VarAdrToInt(d_machine1_STATE_ENERGY_TOTAL_L) == 0))  return;

    dwSaveCurrentPower = g_dbPower.dwCurrentPower;
    dwCurrentPower = VarAdrToInt(d_machine1_STATE_ENERGY_TOTAL_L);

    //20230518 chj 优化能耗表，不做警报提示
    /*
    if((dwCurrentPower > dwSaveCurrentPower && (dwCurrentPower - dwSaveCurrentPower) >1000)
            || ((dwCurrentPower < dwSaveCurrentPower && (dwSaveCurrentPower - dwCurrentPower) >1000)))
    {
        bCurrentCheck = FALSE;
        return;
    }
    else
    {
        bCurrentCheck = TRUE;
    }
    */

    dwCurrentMinPower = dwCurrentPower - dwSaveCurrentPower;
    if(dwCurrentMinPower != 0)
    {
        dwSaveCurrentPower = dwCurrentPower;
        g_dbPower.dwCurrentPower = dwCurrentPower;

        if((dwCurrentMinPower < 0) || (dwCurrentMinPower > 5000))//20230518 chj 优化能耗表，负数不做累加
        {
            return;
        }

        g_dbPower.dwTotalPower += dwCurrentMinPower;
        if(OperateModeIndex() == MODE_MANUAL)
        {
            g_dbPower.dwOtherPower += dwCurrentMinPower;
        }
        else
        {
            g_dbPower.dwAutoPower += dwCurrentMinPower;
        }

        //能耗增加
        time(&time_now);
        t = localtime(&time_now);

        cur_hour = t->tm_hour;
        cur_day = t->tm_mday;
        cur_month = t->tm_mon;
        cur_year = t->tm_year;
        g_G10ProdCnt.power[cur_month][cur_day-1][cur_hour] += dwCurrentMinPower;
    }
}

/**
 * @brief      : 设置电表数据状态
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20201104
 */
void SetCurrentCheck(BOOL state)
{
    bCurrentCheck = state;
}

/**
 * @brief      : 获取电表数据状态
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20201104
 */
BOOL GetCurrentCheck()
{
    return bCurrentCheck;
}

//假如断电保护文件有问题，需要面板定时保存下电表能耗数据
void SavePowerMeter()
{
    FILEHD filehd;

    filehd = FileOpen(POWERMETER_FILE);
    if(!FILEHD_IS_OK(filehd))
    {
        filehd = FileCreate(POWERMETER_FILE);
        FileSeek(filehd, 0);
        FileWrite(filehd, &g_dbPower, sizeof(DB_POWER), NULL);
    }
    else
    {
        FileSeek(filehd, 0);
        FileWrite(filehd, &g_dbPower, sizeof(DB_POWER), NULL);
    }
    FileClose(filehd);
}


void LoadPowerMeter()
{
    FILEHD filehd;

    filehd = FileOpen(POWERMETER_FILE);
    if(!FILEHD_IS_OK(filehd))
    {
        filehd = FileCreate(POWERMETER_FILE);
        memset(&g_dbPower, 0, sizeof(DB_POWER));
        FileSeek(filehd, 0);
        FileWrite(filehd, &g_dbPower, sizeof(DB_POWER), NULL);
    }
    if(FILEHD_IS_OK(filehd))
    {
        FileRead(filehd, &g_dbPower, sizeof(DB_POWER), NULL);
    }
    FileClose(filehd);
}

/***************监控面板写文件功能-START***********************************************************************/
MONIWRITECNT g_moniwritecntdata;
BOOL b_MoniWriteFlie;
static UI32 TimeWriteFile_30s_Start = 0;
#define ContinueCntWrite_Min  100
#define Continue24HourWrite_Min 8000

void LoadMoniWriteData()
{
    b_MoniWriteFlie = FALSE;
    FILEHD filehd;
    filehd = FileOpen(WRITECNT_SAVE_PATH);

    if(FILEHD_IS_OK(filehd))
    {
        memset(&g_moniwritecntdata, 0, sizeof(MONIWRITECNT));
        FileRead(filehd,&g_moniwritecntdata,sizeof(MONIWRITECNT),NULL);

        FileClose(filehd);
    }
    else
    {
        filehd = FileCreate(WRITECNT_SAVE_PATH);
        FileClose(filehd);
    }

    if(g_moniwritecntdata.FlagWriteError == 0xEB90)
    {
        oprintf("write file cnt too much!\n");
        b_MoniWriteFlie = TRUE;
        TimeWriteFile_30s_Start = GetTick();
        g_moniwritecntdata.Min24 = 0;
        g_moniwritecntdata.WriteCnt24 = 0;
        g_moniwritecntdata.ContinueWriteCnt = 0;
        g_moniwritecntdata.FlagWriteError = 0;
        SaveMoniWriteData();

        AlarmWRRecord(ALARM_WRITE_FILE_CNT_TOO_MUCH);
    }

    //做保护，防止干扰
    if(VarAdrToInt(p_PP_MACHSET_24HOURWRITEFILEMAX) < Continue24HourWrite_Min)
    {
        VarAdrSetInt(p_PP_MACHSET_24HOURWRITEFILEMAX,Continue24HourWrite_Min);
        VarAdrSaveInt(p_PP_MACHSET_24HOURWRITEFILEMAX);
    }

    if(VarAdrToInt(p_PP_MACHSET_ContinueWriteFilw) < ContinueCntWrite_Min)
    {
        VarAdrSetInt(p_PP_MACHSET_ContinueWriteFilw,ContinueCntWrite_Min);
        VarAdrSaveInt(p_PP_MACHSET_ContinueWriteFilw);
    }

}

//开机发现写文件过多标记，闪烁警报20s
void MoniWriteFileTick()
{
    if(b_MoniWriteFlie)
    {
        UI32 TimeWriteFile_30s_End = GetTick();
        if((TimeWriteFile_30s_End - TimeWriteFile_30s_Start) >= (1000*20))//20s 卓越画面加载完后，还有等待数据接收，需要时间久一点
        {
            oprintf("write file cnt too much flash end!\n");
            b_MoniWriteFlie = FALSE;
        }
    }
}

void SaveMoniWriteData()
{
    FILEHD filehd;

    filehd = FileOpen(WRITECNT_SAVE_PATH);
    if(!FILEHD_IS_OK(filehd))
    {
        filehd = FileCreate(WRITECNT_SAVE_PATH);
        FileSeek(filehd, 0);
        FileWrite(filehd, &g_moniwritecntdata, sizeof(MONIWRITECNT), NULL);
    }
    else
    {
        FileSeek(filehd, 0);
        FileWrite(filehd, &g_moniwritecntdata, sizeof(MONIWRITECNT), NULL);
    }
    FileClose(filehd);
}

static BOOL HasMoniContinueCntFlag = FALSE;
static UI32 Moni24Shotcnt = 0;
void Moni24HourWrite()
{
    g_moniwritecntdata.Min24 += 1;
    g_moniwritecntdata.WriteCnt24 += (GetWriteCnt() - Moni24Shotcnt);
    g_moniwritecntdata.WriteTotalCnt += (GetWriteCnt() - Moni24Shotcnt);
    Moni24Shotcnt = GetWriteCnt();

    UI32 uCnt = VarAdrToInt(p_PP_MACHSET_24HOURWRITEFILEMAX);
    if(uCnt == 0)
    {
        //oprintf("uCnt 24--------------------- : 0\n");
        uCnt = Continue24HourWrite_Min;
    }

    if(g_moniwritecntdata.WriteCnt24 > uCnt)
    {
        g_moniwritecntdata.FlagWriteError = 0xEB90;
        if(HasMoniContinueCntFlag == FALSE)
        {
            oprintf("24h write too much\n");
            SaveMoniWriteData();
            HasMoniContinueCntFlag = TRUE;
        }
    }
    else
    {
        if(g_moniwritecntdata.Min24 > 24*60)
        {
            if(g_moniwritecntdata.FlagWriteError != 0xEB90)
            {
                g_moniwritecntdata.Min24 = 0;
                g_moniwritecntdata.WriteCnt24 = 0;
            }
        }
    }

}

static UI16 LastCurrentWriteCnt = 0;
void MoniContinueCntWrite()
{
    UI16 CurrentWriteCnt = GetWriteCnt();
    if(CurrentWriteCnt != LastCurrentWriteCnt)
    {
        LastCurrentWriteCnt = CurrentWriteCnt;
        g_moniwritecntdata.ContinueWriteCnt++;

        UI32 uCnt = VarAdrToInt(p_PP_MACHSET_ContinueWriteFilw);
        if(uCnt == 0)
        {
            //oprintf("uCnt Continue--------------------- : 0\n");
            uCnt = ContinueCntWrite_Min;
        }

        if(g_moniwritecntdata.ContinueWriteCnt > uCnt)
        {
            g_moniwritecntdata.FlagWriteError = 0xEB90;
            if(HasMoniContinueCntFlag == FALSE)
            {
                oprintf("diffcnt write too much\n");
                SaveMoniWriteData();
                HasMoniContinueCntFlag = TRUE;
            }
        }
    }
    else
    {
        if(g_moniwritecntdata.FlagWriteError != 0xEB90)
        {
            g_moniwritecntdata.ContinueWriteCnt = 0;
        }
    }
}

/***************监控面板写文件功能-END*************************************************************************/

