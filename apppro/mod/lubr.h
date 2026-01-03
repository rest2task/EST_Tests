#ifndef LUBR_H
#define LUBR_H
#include "typedef.h"
#include "obj.h"
#include "mod.h"
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct  ty_DATETIME
{
    UI16 second;    //秒
    UI16 minute;    //分
    UI16 hour;      //时
    UI16 day;       //日
    UI16 month;     //月
    UI16 year;      //年
}DATETIME;

void SaveHMDLub(int dst,void* src,UI16 wCount);
BOOL LoadHMDLub(void* dst,int src,UI16 wCount);
void CheckHMDLubSave();
void CalculateHMDLub();
void UpdateHMDLub();
void CheckHMDLubArrive(UI16 lubno);
void CheckHMDLubCnt();
void InitHMDLub();
void SaveLubStartTm(UI16 lubno);
UI32 GetLubHour(UI16 lubno);
void SetLubHour(UI16 lubno, UI32 value);
UI32 GetLeftLubHour(UI16 lubno);
void SetLeftLubHour(UI16 lubno, UI32 value);

#define HOUR_LUB_PATH SAVE_PATH"HMDHourLub.dat"

#ifdef __cplusplus
}
#endif
#endif // LUBR_H
