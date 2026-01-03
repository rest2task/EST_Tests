#ifndef SERVOALARMREC_H
#define SERVOALARMREC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "typedef.h"
#include "obj.h"
#include "strhash.h"

#define SERVO_ALARM_WR_PATH   SAVE_PATH"servoalarmrec.dat"
#define MAX_RECORDS				500  //100条记录

//伺服警报记录  20190514 hz
typedef struct tySERVOALARMREC_HEAD
{
    UI16   flag;
    UI16   cur_no;
}SERVOALARMREC_HEAD;

typedef struct tyDB_SERVOALARMREC
{
    UI16    flag;
    UI32    nId;
    UI16    wServoId;
    UI32    wShotCount;
    UI32    datetime;
}DB_SERVOALARMREC;

typedef struct tySERVOALARMREC
{
    SERVOALARMREC_HEAD  servoHead;
    DB_SERVOALARMREC    servoItem[MAX_RECORDS+1];
}SERVOALARMREC;


extern SERVOALARMREC m_servoalarmrec;
extern PHASH_TBL pmap_servoalarm;


void ServoWarnInit();
void ServoWarnMoni();
DB_SERVOALARMREC ReadServoWarnRec(UI16 index);
void ServoWarnClaen();



#ifdef __cplusplus
}
#endif

#endif // SERVOALARMREC_H
