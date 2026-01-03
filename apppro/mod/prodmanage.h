#ifndef _PROD_MONI_H
#define _PROD_MONI_H
#include "typedef.h"
#include "mod.h"
#ifdef __cplusplus



extern "C"
{ 
#endif

//20211112 dyl 开机总时间、运行总计时统计
#define RESERVE2_NUM    16

typedef struct tagTIMECOUNT
{
    UI16 flag;
    UI16 wTotalBootTime_H;           //1 历史累计开机总时间 high 16//NET_FUNC 2019.5.8 csj
    UI16 wTotalBootTime_L;           //2 历史累计开机总时间 low 16
    UI16 wBootRunTime_H;             //3 本次开机时间 high 16
    UI16 wBootRunTime_L;             //4 本次开机时间 low 16
    UI16 wTotalProdTime_H;           //5 历史生产运行总时间 high 16
    UI16 wTotalProdTime_L;           //6 历史生产运行总时间 low 16
    UI16 wTotalMotorTime_H;          //7 历史马达开累计时间 high 16
    UI16 wTotalMotorTime_L;          //8 历史马达开累计时间 low 16
    UI16 wReserve[RESERVE2_NUM];     //预留16个数据地址
}TIMECOUNT;

extern TIMECOUNT m_dbTimeCount;
/***************end //20211112 dyl************/

void ProdInit();
BOOL ProdClrCnt();
BOOL ProdClrPkg();
BOOL ProdSetGoalCnt();
BOOL ProdSetPerMold();
BOOL ProdChgMode();

//20211112 dyl 开机总时间、运行总计时统计
void TimeCountInit();
void Time_Save();
void UpdateTimeCount();
void ProdSetRealTimeCount();
BOOL ProdClrBootTime();
BOOL ProdClrProdTime();

#define WD_TIMECNT_PATH SAVE_PATH"runtimecount.dat"
/***************end //20211112 dyl************/

#ifdef __cplusplus
}
#endif

#endif
