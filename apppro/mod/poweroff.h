#ifndef POWEROFF_H
#define POWEROFF_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "prodpower.h"

//断电保护数据结构体 20190104 hz
typedef struct  ty_POWERDOWNDATA
{
    UI16 flag;  // 20190111 hz
    UI32 totalopen;
    UI8 cur_hour;
    UI8 cur_day;
    UI8 cur_month;
    DB_DAYPOWER DayPower;
    DB_MONTHPOWER MonthPower;
    DB_YEARPOWER YearPower;
    UI32 totalopennocle; //20190129 YX 开模总数不清零
    UI32 hour_cnt;//小时产量
    UI32 hour_opencnt;//小时开模数 //20201104
    UI32 hour_power;//小时能耗 //20201104
    DB_POWER Power;//能耗结构体 //20201104
    UI32 pack_open_cnt; //本包开模数
    UI32 auto_lub_cnt;  //自动润滑计数 //20200410
    UI32 auto_lub_cnt2; //自动润滑计数2
    UI32 auto_lub_cnt3; //自动润滑计数3
    UI32 prod_goodcnt; //良品数 //20220923 dyl 增加良品数保存
    UI32 revbak[30];    //结构体预留，后续最多就这些 //20230518 优化断电保护
    UI16 flagend;       //flag结束标志，如果这个flag没写完，认为文件未保存完整，丢弃
}POWERDOWNDATA, *pPOWERDOWNDATA;

/*get power failure protection data */
int GetProtectData(void* data);

/*set power failure protection data */
void SetProtectData(void* data); //20190107 hz
extern UI8 g_cur_hour, g_cur_day, g_cur_month;

#ifndef WIN32
#ifdef IMX6

void PowerOffInint();
void PowerOffMonitor();
void SavePoweroffData();

#endif
#endif
#ifdef __cplusplus
}
#endif

#endif // POWEROFF_H
