#ifndef _MOLDTEMP_h
#define _MOLDTEMP_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "typedef.h"
#include "mod.h"

//mold temp
typedef struct tagDB_TEMPMOLD
{
    UI16        wMarkUsed;
    UI16	    nTempId;
    UI16        wData[8];
    UI16        wSetData[8];
    UI16        wFuncData[8];
    UI16	    wHeat;
    UI16	    wStart;
    UI16	    wStart1;
    UI16	    wHold;
    UI16	    wIntermediate;

    UI16        wPid[5];
    UI16        wReservd[5];
} DB_TEMPMOLD,*PDB_TEMPMOLD;

typedef struct tagDB_TEMPMOLD2
{
    UI16        wMarkUsed;
    UI16	    nTempId;
    UI16        wData[8];
    UI16        wSetData[8];
    UI16        wFuncData[8];
    UI16	    wHeat;
    UI16	    wStart;
    UI16	    wStart1;
    UI16	    wHold;
    UI16	    wIntermediate;

    UI16        wPid[5];
    UI16        wReservd[5];
} DB_TEMPMOLD2,*PDB_TEMPMOLD2;

void MoldTempInital(PCMOD pmod);

/*injet func page in*/
BOOL MoldTempPageIn(PCPAR para);


#ifdef __cplusplus
}
#endif

#endif



