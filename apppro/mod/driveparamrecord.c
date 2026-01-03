#include "driveparamrecord.h"
#include "time.h"
#include "file.h"
#include "common.h"
#include "language.h"
#include "monmcu.h"
#include "prot.h"
#include "oprintf.h"

static DB_DRIVERECORD m_driverecord;

//20190219.cyx
static BOOL CheckFileValid(FILEHD filehd)
{
	if(FILEHD_IS_OK(filehd))
	{
		return TRUE;
	}
	else
	{
		FileClose(filehd);
		return FALSE;
	}
}

static int ParamisFull(DB_DRIVERECORD* rec)
{
    if((rec->head.rear + 1) % MAX_DRIVE_RECORDS == rec->head.front)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static int ParamisEmpty(DB_DRIVERECORD* rec)
{
    if(rec->head.rear == rec->head.front)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static int ParamDelete(DB_DRIVERECORD* rec)
{
    if(ParamisEmpty(rec))
        return FALSE;

    rec->head.front = (rec->head.front + 1) % MAX_DRIVE_RECORDS;
    return TRUE;
}

static void	WriteParamRec(int index,void* src)
{
    FILEHD filehd;
    filehd = FileOpen(DRIVE_SAVE_PATH);
    if(CheckFileValid(filehd))
    {
        FileSeek(filehd,0);
        FileWrite(filehd,&m_driverecord.head, sizeof(DRIVEHEAD),NULL);
        FileSeek(filehd, sizeof(DRIVEHEAD) + index * sizeof(DRIVEITEM));
        FileWrite(filehd, src, sizeof(DRIVEITEM), NULL);
        FileClose(filehd);
    }
}

static int ParamAdd(DB_DRIVERECORD* rec, DRIVEITEM item)
{
    int index;
    if(ParamisFull(rec))
        ParamDelete(rec);
    rec->item[rec->head.rear] = item;
    index = rec->head.rear;
    rec->head.rear = (rec->head.rear + 1) % MAX_DRIVE_RECORDS;
    WriteParamRec(index, &item);
    return TRUE;
}

static int ParamNum(DB_DRIVERECORD* rec)
{
    return (rec->head.rear - rec->head.front + MAX_DRIVE_RECORDS) % MAX_DRIVE_RECORDS;
}

BOOL   SetDriveRecord(DRIVEITEM* drive,UI32 id,UI32 addr,UI32 addrlength,UI32 inputdata)
{
    if(drive!=NULL)
    {
		UI32 datetime= (UI32)time(NULL);
        drive->wId = id;
        drive->wParamaddress = addr;
        drive->wParamaddresslength = addrlength;
        drive->wInputdata = inputdata;
        TimeToStr(drive->time,"yy/MM/dd hh:mm:ss",datetime);

        ParamAdd(&m_driverecord, *drive);
        return true;
    }
    return false;
}

BOOL	ReadDriveRecord(DRIVEITEM*drive,UI32 index)
{
    DRIVEITEM tmpitem;
    if((index < ParamNum(&m_driverecord)) && !ParamisEmpty(&m_driverecord))
    {
        tmpitem = m_driverecord.item[(m_driverecord.head.rear - index - 1 + MAX_DRIVE_RECORDS) % MAX_DRIVE_RECORDS];
        memcpy(drive,&tmpitem,sizeof(DRIVEITEM));
        return true;
    }
    return false;
}

void	DriveRecordInit()
{
    FILEHD filehd;
    filehd = FileOpen(DRIVE_SAVE_PATH);
    memset(&m_driverecord, 0, sizeof(DB_DRIVERECORD));
    m_driverecord.head.front = 0;
    m_driverecord.head.rear = 0;
    if(CheckFileValid(filehd))
    {
        FileRead(filehd, &m_driverecord, sizeof(DB_DRIVERECORD), NULL);
        FileClose(filehd);
    }
    else
    {
        filehd = FileCreate(DRIVE_SAVE_PATH);
        FileClose(filehd);
    }
}

