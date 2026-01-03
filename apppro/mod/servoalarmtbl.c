#include "servoalarmtbl.h"
#include "oprintf.h" 
#include "language.h"

#define SERVOALARM_TABLE                  DATA_PATH"servo_alarm.map"

PHASH_TBL pmap_servoalarmtbl=NULL;//

void ServoAlarmTblInital()
{
    ParseCSVFile(SERVOALARM_TABLE,&pmap_servoalarmtbl);//
}

const char* GetServoAlarmTran(int mapid )
{
    if(pmap_servoalarmtbl == NULL)
    {
		return NULL;
    }
    return GetMapTran(pmap_servoalarmtbl,mapid);
}
