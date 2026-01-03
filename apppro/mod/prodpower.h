#ifndef PRODPOWER_H
#define PRODPOWER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "typedef.h"
#include "obj.h"
#include "vardb.h"
#include "common.h"
#include "database.h"

#define DAY_HOUR 24
#define MON_DAY  31
#define YEAR_MON 12

#define DAY 0
#define MON 1
#define YEAR 2

#define DAYPOWERFILE SAVE_PATH"dayprodpower.dat"
#define MONTHPOWERFILE SAVE_PATH"monthprodpower.dat"
#define YEARPOWERFILE SAVE_PATH"yearprodpower.dat"
#define HEATERSETFILE SAVE_PATH"heaterset.dat"

#define G10_PRODCNT_FILE SAVE_PATH"productcnt.dat"
#define G10_POWER_FILE SAVE_PATH"power.dat" //能耗结构体数据储存文件
#define G10_PRODCNT_LAST_FILE SAVE_PATH"productcntlast.dat"//20250318 jhh 跨年产量能耗数据保存
#define POWERMETER_FILE SAVE_PATH"powermeter.dat"

//能耗统计-日 //20190403 hz
typedef struct DayPower{
    UI16  DayFlag;
    UI32 dwCount;
    UI32 dwProduct;
    UI32 dwPower;
} DB_DAYPOWER;

//能耗统计-月 //20190403 hz
typedef struct MonthPower{
    UI16  MonthFlag;
    UI32 dwCount;
    UI32 dwProduct;
    UI32 dwPower;
} DB_MONTHPOWER;

//能耗统计-年 //20190403 hz
typedef struct YearPower{
    UI16  YearFlag;
    UI32 dwCount;
    UI32 dwProduct;
    UI32 dwPower;
} DB_YEARPOWER;

typedef struct tagDB_HEATERSET{       //加热圈功耗文件读取数据结构体
    UI16 wHeatPower[16];              //加热圈16段功率
    UI16 wHeatFactor[16];             //加热圈16段系数
}DB_HEATERSET;

//G10 产量功能 只保存过去一年的产量（往前推一年）
typedef struct tyG10PRODCNT
{
    UI16 year;
    UI32 prod_cnt[YEAR_MON][MON_DAY][DAY_HOUR];//产量
    UI32 open_cnt[YEAR_MON][MON_DAY][DAY_HOUR];//开模数 //20201104
    UI32 power[YEAR_MON][MON_DAY][DAY_HOUR];//能耗 //20201104
}G10PRODCNT;

//能耗结构体
typedef struct tyDB_POWER
{
    UI32 dwAutoPower;           //生产能耗
    UI32 dwOtherPower;          //其他能耗
    UI32 dwTotalPower;          //总能耗
    UI32 dwCurrentPower;        //电表能耗
    UI32 dwOpenCnt;             //开模数
    UI32 tmModify;              //时间
}DB_POWER;

extern DB_DAYPOWER g_dbDayPower[24];
extern DB_MONTHPOWER g_dbMonthPower[31];
extern DB_YEARPOWER g_dbYearPower[12];
extern DB_HEATERSET g_dbHeaterSet;
extern G10PRODCNT g_G10ProdCnt;
extern G10PRODCNT g_G10ProdCntLast;
extern DB_POWER g_dbPower;//20201104

//mod API
void ProdPower();
void GetHeaterPower();
void InitProdPower();
void WriteProdPower(const char* filename, void* src, UI16 size);
void G10SaveProdCnt();
void G10SaveProdCntLast();
//pages API
void ClearPower(int date);
void G10ClearOneDay(UI16 cur_month, UI16 cur_day);
void G10ClearOneMonth(UI16 cur_month);
void G10ClearAll();
void G10LastClearAll();
//20201104
void ProductPowerData();
void SetCurrentCheck(BOOL state);
BOOL GetCurrentCheck();
void SavePowerMeter();
void LoadPowerMeter();

/***************监控面板写文件功能-START***********************************************************************/
#define WRITECNT_SAVE_PATH  SAVE_PATH"writecntsave.dat"
#define  ALARM_WRITE_FILE_CNT_TOO_MUCH  0x33100102  //写文件次数过多

typedef struct tyMONIWRITECNT
{
    UI16 Min24;                 //24小时写的次数：计时
    UI32 WriteCnt24;            //24小时写的次数：次数
    UI16 ContinueWriteCnt;      //连续多少模写文件：连续模数
    UI32 WriteTotalCnt;         //历史总写文件次数
    UI16 FlagWriteError;        //写的次数过多，需要开机提示
    UI16 Bak[4];
}MONIWRITECNT;
extern MONIWRITECNT g_moniwritecntdata;
extern BOOL b_MoniWriteFlie;
void LoadMoniWriteData();
void SaveMoniWriteData();
void Moni24HourWrite();
void MoniContinueCntWrite();
void MoniWriteFileTick();

/***************监控面板写文件功能-END*************************************************************************/

#ifdef __cplusplus
}
#endif

#endif // PRODPOWER_H
