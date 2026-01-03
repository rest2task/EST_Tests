#ifndef _DRIVEPARAMRECORD_h
#define _DRIVEPARAMRECORD_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "mod.h"

#define		MAX_DRIVE_RECORDS   (300 + 1)

typedef struct tyDRIVEHEAD
{
    UI16 front;
    UI16 rear;
}DRIVEHEAD;

typedef struct tyDRIVEITEM
{
    UI32	wId;
    UI32	wParamaddress;
    UI32	wParamaddresslength;
    UI32	wInputdata;
    char	time[20];
    UI32	rev[8];
}DRIVEITEM;

typedef struct tyDRIVERECORD
{
    DRIVEHEAD head;
    DRIVEITEM item[MAX_DRIVE_RECORDS];
} DB_DRIVERECORD,*PDBDRIVERECORD;


BOOL    SetDriveRecord(DRIVEITEM* drive,UI32 id,UI32 addr,UI32 addrlength,UI32 inputdata);
BOOL    ReadDriveRecord(DRIVEITEM*drive,UI32 index);
void	DriveRecordInit();

#define DRIVE_SAVE_PATH   SAVE_PATH"driverec.dat"


#ifdef __cplusplus
}
#endif

#endif



