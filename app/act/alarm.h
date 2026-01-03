#ifndef _ALARM_H_
#define _ALARM_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include	"typedef.h"
#include  "part.h"
#include	"alarm_dic.h"

#define ALARM_INTVAL				1000
#define ALARM_OFF_DELAY			2000
#define STANDAR_ALARM_INTVAL		2000
#define TEMP_ALARM_CNT			8

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef struct tag_ALARM
  {
    WORD        alarmstart;   // alarm start time register
    WORD        alarmlast ;   // alarm begin time for 1 sec alarm purse
    WORD        alarmcnt  ;
    WORD        alarmflag ;   // 0 Alarm off 1 alarm on 2 alarm on 1 second
    WORD		alarmsetcnt_temp;	//临时警报设定次数，默认5, 范围4~99次
  } ALARM_STRU;

typedef struct tyALARM
{
	PART			part;
	
	PALARM_PO		po;
	PALARM_PI		pi;
	PALARM_MACHSET	mh;
	PALARM_STATE	st;

	ALARM_STRU		g_alarm;
	ALARM_STRU		temp_alarm;

	UI16			key_run_start_ms;
}ALARM, *PALARM;

#define ALARM_INIT_INFO {{PO_ID, CAL_LEN(ALARM_PO)}, {PI_ID, CAL_LEN(ALARM_PI)}, {MH_ID, CAL_LEN(ALARM_MACHSET)}, {ST_ID, CAL_LEN(ALARM_STATE)}}


extern ALARM m_alarm;

extern void StandAlarm(BOOL bforce);    //标准警报
extern void TempAlarm(BOOL bforce);        //临时警报 07-7-27
extern void AlarmOff(void);
extern BOOL InitAlarmPart(void);
void AlarmSetEmerge(BOOL bemerge);
BOOL AlarmIsOn(void);
void AlarmManualInit(void);
void AlarmLoop(void);
void AlarmOn(void);
BOOL TempAlarmIsOn(void);
void TempAlarmOff(void);

void AlarmDataTxChk(PPART pp, UI32 err,BOOL bforce);
void ChkEmgExtPi(void);


#define ALARM_DATA_TX(pp,err)  AlarmDataTxChk(pp,err,FALSE)
#define ALARM_DATA_TX_MUST(pp,err)  AlarmDataTxChk(pp,err,TRUE)



#ifdef __cplusplus
}
#endif

#endif


