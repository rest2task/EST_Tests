#ifndef _INJSTOR_H_
#define _INJSTOR_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "part.h"
#include "injstor_dic.h"

#define INJSTOR_PQ			0x01
#define INJSTOR_PRESS		0x02

typedef struct tyINJECT* PINJECT;
typedef struct INJSTOR
{
	PART				part;

	PINJSTOR_PO			po;
	PINJSTOR_PI			pi;
	PINJSTOR_MACHSET	mh;
	PINJSTOR_MOLDSET	md;
	PINJSTOR_ACTPARA	pa;
	PINJSTOR_HYDR		hy;
	PINJSTOR_STATE		st;

	ACT					act[INJSTOR_ACT_SN_END - 1];

	PINJECT				pinj;
	UI8					mode;
}INJSTOR, *PINJSTOR;

#define INJSTOR_INIT_INFO	{{PO_ID, CAL_LEN(INJSTOR_PO)}, {PI_ID, CAL_LEN(INJSTOR_PI)}, {MH_ID, CAL_LEN(INJSTOR_MACHSET)}, {MD_ID, CAL_LEN(INJSTOR_MOLDSET)}, \
							 {PA_ID, CAL_LEN(INJSTOR_ACTPARA)}, {HY_ID, CAL_LEN(INJSTOR_HYDR)}, {ST_ID, CAL_LEN(INJSTOR_STATE)}}


BOOL InitInjStorPart(PINJSTOR pp, UI8 sn);
void InjStorLoop(PINJSTOR pp);

#ifdef __cplusplus
}
#endif

#endif
