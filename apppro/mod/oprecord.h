#ifndef _OPRECORD_h
#define _OPRECORD_h
#include "typedef.h"
#include "obj.h"
#include "mod.h"
#ifdef __cplusplus
extern "C"
{ 
#endif
typedef enum enOPTYPE
{
	OP_CLEANSHOT=0,
	OP_AUTOSTART,
    OP_SEMIAUTO,//状态记录增加半自动,粗调模,自动调模 //20200115.cyx
    OP_ADJMOLD,
    OP_AUTOADJMOLD,
	OP_MANUAL,
	OP_AUTOSTOP,
	OP_DF=0xFEFF
}OPTYPE;
#define		OP_MAX_RECORDS			(500)
#define     MODE_MANUAL         0x00
#define     MODE_AUTO           0x01
#define     MODE_SEMIAUTO       0x02
#define     MODE_SENSORAUTO     0x04
#define     MODE_TIMEAUTO       0x08

#define     MODE_MACROADJ       0x10
#define     MODE_POSSET         0x20
#define     MODE_AUTOPURGE      0x40
#define     MODE_AUTOADJMOLD    0x80

#define     MODE_RAMPADJUST     0x1000
#define     MODE_PRESADJUST     0x2000

//20190625 YX
#define     MODE_PARTRZ         0x0100
#define     MODE_AA_MOD         0x0200

#define     MODE_AA_LEARN       0x0800 //20211224 dyl 齿距自学习


typedef struct tyOPRECORD_HEAD
{
	UI16    flag;
	UI16	cur_no;
	UI16	reserve[6];
} OPRECORD_HEAD;

typedef struct tyOPRECORD_ITEM
{
	UI16		flag;
	UI32		nShot;
	I32			opId;
	UI32		datetime;
	UI32		rev[2];
} OPRECORD_ITEM,*POPRECORD_ITEM;

typedef struct tyOPRECRD
{
	OPRECORD_HEAD opHead;
	OPRECORD_ITEM opitems[OP_MAX_RECORDS+1];
}OPRECRD;

void OPWRRecord(UI16 type);
void OPLEDMonitorRecord(UI16 opmode);
BOOL OPHasChange();
void OPResetChange();
void	OPParamInit();
void OPResetRecord();
I32 OPGetCurRecordNum();
OPRECORD_ITEM	OPReadRecord(I32 index);
OPRECRD * GetOpRecord();
#define		OP_FILE_PATH   SAVE_PATH"operatmoni.dat"

/*****************************TY  Operate2************************/
void OPWRRecord2(UI16 type);//20190813 CHZ
BOOL OPHasChange2();
void OPResetChange2();
void	OPParamInit2();
void OPResetRecord2();
I32 OPGetCurRecordNum2();
OPRECORD_ITEM	OPReadRecord2(I32 index);
#define		OP_FILE_PATH2   SAVE_PATH"operatmoni2.dat"

#ifdef __cplusplus
}
#endif

#endif



