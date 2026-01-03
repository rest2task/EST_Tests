#ifndef _PARAMRECORD_h
#define _PARAMRECORD_h
#include "typedef.h"
#include "obj.h"
#include "mod.h"
#ifdef __cplusplus
extern "C"
{ 
#endif

#define		MAX_PARA_RECORDS			(500 + 1)

typedef struct tyPARAMHEAD
{
    UI16 front;
    UI16 rear;
}PARAMHEAD;

typedef struct tyPARAMITEM
{
    char		idTitle[32];
    UI32		addr;
    UI8			nPoint;
    UI32		wValue;
    UI32		wOldValue;
    UI32		datetime;
    UI8         base;
    UI8         rev1;
    UI16        rev2;
    UI32		rev[7];
}PARAMITEM;

typedef struct tyPARAMRECORD
{
    PARAMHEAD head;
    PARAMITEM item[MAX_PARA_RECORDS];
} DB_PARAMRECORD,*PDBRECORD;

typedef struct tyRecordItem{
    const char*		idTitle;
    const char*		param;
    char	newvalue[12];
    char	oldvalue[12];
    char	time[20];
}RecordItem;

int RecordValueEX(POBJ pobj, int oldvalue, PATTR pattr);
void RecordValue(POBJ pobj,PATTR pattr,UI32 oldvalue,UI8 npoint, UI8 base);

BOOL	ReadRecord(RecordItem*item,UI32 index);
BOOL    SetRecordByManual(char *title,UI32 addr,UI8 npoint,UI32 newValue,UI32 oldValue,UI8 base);
void	ParamRecordInit();

#define RECORD_FILE_PATH	SAVE_PATH"recordparam.dat"
#define MAP_FILE			DATA_PATH"tag.map"


#ifdef __cplusplus
}
#endif

#endif



