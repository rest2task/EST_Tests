#ifndef _ALARMRECORD_h
#define _ALARMRECORD_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "mod.h"

#define		MAX_RECORDS				100  //100条记录
#define     ALARM_NUMBER(x)      (((x&0xFFF00000)|((x&0xF00)<<8)|((x&0x0F)<<12))>>12)

//警报记录
typedef struct tyDB_WARNRECORD
{
	UI16    flag;
	UI32    nId;
	UI32    wShotCount;
	UI32    datetime;
	UI32    rev[2];
}DB_WARNRECORD,*PDB_WARNRECORD;

typedef struct tyDB_WARNRECOD_HEAD
{
	UI16 flag;
	UI16 cur_no;
	UI16 reserve[6];
}DB_WARNRECOD_HEAD;

typedef struct tyWARNRECD
{
	DB_WARNRECOD_HEAD warnHead;
	DB_WARNRECORD     warnitems[MAX_RECORDS+1];
}WARNRECD;

extern WARNRECD m_warnrecd;

extern PHASH_TBL pmap_alarm;

void AlarmRecdInital(PCMOD pmod);
//
DB_WARNRECORD AlarmReadRecord(UI32 index);
void AlarmWRRecord(UI32 cIndex);//G10执行G10_AlarmWRRecord
void LoadAlarmRecord();//G10执行G10_LoadAlarmRecord
void AlarmClearRecord();//G10执行G10_AlarmClearRecord


#define ALARM_FILE		DATA_PATH"alarm.map"
#define ALARM_WR_PATH   SAVE_PATH"alarmrec.dat"


#ifdef __cplusplus
}
#endif

#endif



