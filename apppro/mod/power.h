#ifndef _POWER_h
#define _POWER_h
#include "typedef.h"
#include "obj.h"
#include "mod.h"
#ifdef __cplusplus
extern "C"
{ 
#endif

#define MAX_ITEMS			(100+1)

typedef struct tyPOWER_HEAD
{
	UI16    flag;
	UI16	cur_no;
	UI16	reserve[6];
} POWER_HEAD;

typedef struct tyPOWER_ITEM
{
	UI16		flag;

	UI16		total;
	UI16		cls;
	UI16		open;
	UI16		inject;
	UI16		charge;
	UI16		suckB;
	UI16		eject;
	UI16		other;

	UI16		rev[2];
} POWER_ITEM,*PPOWER_ITEM;

typedef struct tyPOWERRECRD
{
	POWER_HEAD Head;
	POWER_ITEM items[MAX_ITEMS+1];
}POWERRECRD;

void ProdEnergySave();
void ProdEnergy();
void	PowerParamInit();
UI32 PowerGetRecordCnt();
POWER_ITEM	PowerReadRecord(UI32 index);
POWERRECRD* PowerGetRecord();
#ifdef __cplusplus
}
#endif

#endif



