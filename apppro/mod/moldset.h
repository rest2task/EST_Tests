#ifndef _MOLDSET_h
#define _MOLDSET_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "mod.h"
#include "privdef.h"

#ifdef GTYPE_OR_PTYPE
#define MAX_MOLDSET 249
#else
#define MAX_MOLDSET 99
#endif

typedef struct tyMOLDSET_CONTEXT
{
	UI16	start_no;
	UI16	cur_no;
	UI16	save_cnt;
	UI8		sel_state[MAX_MOLDSET];
	UI8		is_selall;
}MOLDSET_CONTEXT, *PMOLDSET_CONTEXT;

typedef struct tyMOLDSET_HEAD
{
	UI16    flag;
	UI16	cur_no;
    UI16    bchange; //20200113 增加同一组模具是否有修改；
    UI16	reserve[5];
} MOLDSET_HEAD;


typedef struct tyDATE { 
  UI8    rev;  
  UI8    year;               // years 
    UI8    month;              // months since January - [0~11] 
    UI8    day;                // day of the month - [1~31] 
	
} mDATE;

typedef struct tyMOLDSET_ITEM
{
	UI16    flag;
	char    sName[90+1];
	char	sMater[30+1];
	char	sColor[30+1];
	mDATE    dSave;
	char	rev;
} MOLDSET_ITEM;

#ifdef GTYPE_OR_PTYPE
#define MAX_MOLDPARAMCOUNT     10000   //模具参数最大个数
#else
#define MAX_MOLDPARAMCOUNT     1800   //模具参数最大个数
#endif

#define MAX_MOLDSUBCLASSCOUNT   200   //模具子类最大个数
typedef struct tyDB_MOLDSET_EX
{
	UI16  wCount;
	UI16  wIndCount;
	UI32  wIndAddrArray[MAX_MOLDSUBCLASSCOUNT];
	UI16  wIndCntArray[MAX_MOLDSUBCLASSCOUNT];
	UI16  wDataArray[MAX_MOLDPARAMCOUNT];
} DB_MOLDSET_EX;

extern MOLDSET_CONTEXT g_moldset;
extern MOLDSET_HEAD g_moldset_head;

void MoldSetInital();

//保存moldset,hydr,actpara数据
BOOL SaveMoldSet(UI16 cMoldSet);
//读取moldset,hydr,actpara数据,并下发
BOOL ReadMoldSet(UI16 cMoldSet);
//Get current moldset name and id
void MoldSet_GetCur(char* cur_name, int* cur_id);

//csj 2018.5.21
BOOL MoldInput();
BOOL MoldExportTo(const char* cmd,const char * filepath,UI16 maxsize);

BOOL ReadMoldsetInfoTemp(UI16 cMoldSet,MOLDSET_ITEM* info);
BOOL ReadMoldsetInfoEX(UI16 cMoldSet,DB_MOLDSET_EX*   moldSetx);
void    SaveMoldsetInfo(UI16 cMoldSet,MOLDSET_ITEM* info);
BOOL        SaveMoldSetEx(UI16 cMoldSet,DB_MOLDSET_EX* moldSetx);

BOOL CheckMoldFile(const char* filename, BOOL includehead);

#ifdef __cplusplus
}
#endif

#endif



