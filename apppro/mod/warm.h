#ifndef _WARM_h
#define _WARM_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "mod.h"

//预温结构体
typedef struct  tyDB_WARM
{
	UI16    nHour;                   //加温时
	UI16    nMinute;                 //加温分
	UI16    bWarm;                   //开始加温
	UI16    nClsHour;                //关加温时
	UI16    nClsMin;                 //关加温分
	UI16    bWarmCls;                //关闭加温
}DB_WARM,*PDB_WARM;

typedef struct  tyDB_WARMX
{
	UI16    flag;                  
	DB_WARM dbwarmconfig[7];  //一周   
}DB_WARMX,*PDB_WARMX;

extern DB_WARMX  dbwarmx;

//
void WarmMonitor();
BOOL LoadWarmSet();
void SaveWarmSet(UI32 dst, void* src, UI16 wCount);


#define WARMDB_INIT_FILE	SAVE_PATH"warm.dat"


#ifdef __cplusplus
}
#endif

#endif



