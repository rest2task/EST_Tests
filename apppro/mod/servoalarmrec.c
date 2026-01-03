/************************************************
 * Create Servo Alarm Record Module 20190514 hz
************************************************/

#include "servoalarmrec.h"
#include "file.h"
#include "vardb.h"
#include "database.h"
#include "servo.h"
#include "time.h"

SERVOALARMREC m_servoalarmrec;
PHASH_TBL pmap_servoalarm = NULL;

#define FILE_INDEX(NUM)	(sizeof(SERVOALARMREC_HEAD) + sizeof(DB_SERVOALARMREC)*NUM)

static void LoadServoWarnRec()
{
    FILEHD filehd;

    filehd = FileOpen(SERVO_ALARM_WR_PATH);

    if(FILEHD_IS_OK(filehd))
    {
        FileRead(filehd,&m_servoalarmrec,sizeof(SERVOALARMREC),NULL);
        FileClose(filehd);
    }
    else
    {
        filehd = FileCreate(SERVO_ALARM_WR_PATH);
        FileClose(filehd);
    }
}

DB_SERVOALARMREC ReadServoWarnRec(UI16 index)
{
    DB_SERVOALARMREC item;
    memset(&item, 0, sizeof(DB_SERVOALARMREC));
    if(index < MAX_RECORDS)
    {
        item = m_servoalarmrec.servoItem[index];
    }
    return item;
}

static BOOL WriteServoWarnRec(UI16 itmenum)
{
    UI32 index;

    FILEHD filehd;
    filehd = FileOpen(SERVO_ALARM_WR_PATH);
    index = FILE_INDEX(itmenum);
    if(FILEHD_IS_OK(filehd))
    {
        FileSeek(filehd,0);
        FileWrite(filehd,&m_servoalarmrec,sizeof(SERVOALARMREC_HEAD),NULL);
        FileSeek(filehd,index);
        FileWrite(filehd,(char*)(&m_servoalarmrec) + index,sizeof(DB_SERVOALARMREC),NULL);
        FileClose(filehd);
        return TRUE;
    }
    else
    {
        FileClose(filehd);
    }
    return FALSE;
}

static void SaveServoWarnRecord(UI32 cIndex,int wServoId)
{
    UI16 cur;

    cur = m_servoalarmrec.servoHead.cur_no;
    m_servoalarmrec.servoItem[cur].flag = MARK_USED;
    m_servoalarmrec.servoItem[cur].nId = cIndex;
    m_servoalarmrec.servoItem[cur].wServoId = wServoId;
    m_servoalarmrec.servoItem[cur].wShotCount = VarAdrToInt(CLAMP_STATE_MOLDOPNNUM0)<<16 | VarAdrToInt(CLAMP_STATE_MOLDOPNNUM1);
    m_servoalarmrec.servoItem[cur].datetime = (UI32)time(NULL);

    m_servoalarmrec.servoHead.cur_no++;
    if(m_servoalarmrec.servoHead.cur_no > MAX_RECORDS)
    {
        m_servoalarmrec.servoHead.cur_no = 0;
    }

    WriteServoWarnRec(cur);
}

void ServoWarnMoni()
{
    static UI32 dwServo[7]={0};
    int i;
    if(((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x0004) == 0) && (VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020))
    {
        for(i=0; i<7; ++i)
        {
           if((dwServo[i] != VarAdrToInt(SERVO_STATE_ERR1+i)) && (VarAdrToInt(SERVO_STATE_ERR1+i) > 0))
           {
               dwServo[i] = VarAdrToInt(SERVO_STATE_ERR1+i);
               SaveServoWarnRecord(dwServo[i],i+1);
           }
        }
    }
    else if((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x0004) && (VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020))
    {
        for(i=1; i<8; ++i)
        {
           if((dwServo[i-1] != dbsvox[i].wData[ID_8000]) && (dbsvox[i].wData[ID_8000] > 0))
           {
               dwServo[i-1] = dbsvox[i].wData[ID_8000];
               SaveServoWarnRecord(dwServo[i-1],i);
           }
        }
    }
}

void ServoWarnInit()
{
    LoadServoWarnRec();
    ParseCSVFile(SERVO_STATE, &pmap_servoalarm);
}

void ServoWarnClaen()
{
    UI16 i;
    FileDelete(SERVO_ALARM_WR_PATH);
    LoadServoWarnRec();

    m_servoalarmrec.servoHead.cur_no = 0;
    for(i = 0; i < MAX_RECORDS; ++i)
    {
        m_servoalarmrec.servoItem[i].flag = 0;
        //WriteServoWarnRec(i);
    }
}


