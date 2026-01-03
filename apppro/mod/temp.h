#ifndef _TEMP_h
#define _TEMP_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "mod.h"

void TempSetInital(PCMOD pmod);
//
typedef void(*PER_HEATMONI_CB)();
PER_HEATMONI_CB PerHeatMoniCbSet(PER_HEATMONI_CB perheat_cb);

void PreHeatMonitor();
UI16 GetWeekData();
BOOL isTempOptimize();
BOOL isTempOptimizex();//20191212
//20200407HJM 模温功能
BOOL isMTOptimizex();

BOOL TempOptimizeJudege();
void TempOptimizeTm();
void MoldTempFuncClose();
BOOL GetMtpTempHeatState();

BOOL MoldTempOptimizeJudege();//20200806

#ifdef __cplusplus
}
#endif

#endif



