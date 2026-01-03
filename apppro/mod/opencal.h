#ifndef OPENCAL_H
#define OPENCAL_H
#include "mod.h"
#include "moldset.h"
#ifdef __cplusplus
extern "C"
{
#endif

void  SaveOpenCalRec(void* src,int nCount);
BOOL  LoadOpenCalRec(void* dst);

BOOL SaveOpenCalData_Mold(UI16 cMoldSet);
BOOL ReadOpenCalData_Mold(UI16 cMoldSet);
void DeleteOpenCal_Mold(UI16 cMoldSet);
void SaveOpenCal_MoldInfo(UI16 cMoldSet,MOLDSET_ITEM* info);
BOOL OpenCalInput();
BOOL OpenCalExportTo(const char* cmd,const char * filepath,UI16 maxsize);

BOOL CheckOpenCalFile(const char* filename, BOOL includehead);

#define OPEN_CAL_PATH SAVE_PATH"opentabcal.dat"

#ifdef __cplusplus
}
#endif
#endif
