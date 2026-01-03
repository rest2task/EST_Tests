#ifndef _SERVOALARMTBL_h
#define _SERVOALARMTBL_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "mod.h"

extern	PHASH_TBL pmap_servoalarmtbl;

void ServoAlarmTblInital();
const char* GetServoAlarmTran(int mapid );
#ifdef __cplusplus
}
#endif

#endif



