#ifndef _LOCK_H_
#define _LOCK_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include	"acttype.h"
#include	"lock_dic.h"

typedef struct tyLOCK
{
	PART			part;

	PLOCK_PO		po;
	PLOCK_PI		pi;
	PLOCK_MACHSET	mh;
	PLOCK_MOLDSET	md;
	PLOCK_ACTPARA	pa;
	PLOCK_HYDR	hy;
	PLOCK_STATE	st;

	ACT			act[LOCK_ACT_SN_END -1];
}LOCK, *PLOCK;

#define LOCK_INIT_INFO      {{PO_ID, CAL_LEN(LOCK_PO)}, {PI_ID, CAL_LEN(LOCK_PI)}, {MH_ID, CAL_LEN(LOCK_MACHSET)}, {MD_ID, CAL_LEN(LOCK_MOLDSET)}, \
							 {PA_ID, CAL_LEN(LOCK_ACTPARA)}, {HY_ID, CAL_LEN(LOCK_HYDR)}, {ST_ID, CAL_LEN(LOCK_STATE)}}

BOOL InitLockPart(PLOCK pp, UI8 sn);

#ifdef __cplusplus
}
#endif

#endif
