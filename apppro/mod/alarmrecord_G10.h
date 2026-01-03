#ifndef _ALARMRECORD_G10_h
#define _ALARMRECORD_G10_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "mod.h"

#define     G10_MAX_RECORDS	 500  //500条记录

//警报记录
typedef struct tyDB_G10_WARNRECORD
{
	UI16    flag;
	UI32    nId;
	UI32    wShotCount;
	UI32    datetime;
	UI32    rev[2];
        UI16    subId;
        UI16    res1[16];//温度部件#1警报解析
        UI16    res2[16];
}DB_G10_WARNRECORD,*PDB_G10_WARNRECORD;

typedef struct tyDB_G10_WARNRECOD_HEAD
{
	UI16 flag;
	UI16 cur_no;
	UI16 reserve[6];
}DB_G10_WARNRECOD_HEAD;

typedef struct tyG10_WARNRECD
{
        DB_G10_WARNRECOD_HEAD warnHead;
        DB_G10_WARNRECORD     warnitems[G10_MAX_RECORDS+1];
}G10_WARNRECD;

extern G10_WARNRECD m_g10_warnrecd;

DB_G10_WARNRECORD G10_AlarmReadRecord(UI32 index);
BOOL G10_AlarmWRRecord(UI32 cIndex,UI32 wErrId);
void G10_LoadAlarmRecord();
void G10_AlarmClearRecord();

//20201118 警报帮助结构体
typedef struct tyG10_WARNHELPECD
{
    UI32    nId;
    UI32    subId;
    UI32    datetime;
    UI16    res1[16];
}G10_WARNHELPRECD;
UI16 FindWarnId(UI32 cIndex);
UI32 GetPanelErrTime();
extern G10_WARNHELPRECD m_g10_warnhelprecd[];

#define G10_ALARM_WR_PATH   SAVE_PATH"new_alarmrec.dat"

#ifdef __cplusplus
}
#endif

#endif



