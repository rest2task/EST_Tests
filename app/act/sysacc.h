#ifndef _SYSACC_H_
#define _SYSACC_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include	"part.h"
#include  "sysacc_dic.h"

typedef struct tySYSACC
{
	PART				part;
	
	PSYSACC_PO			po;
	PSYSACC_PI			pi;
	PSYSACC_ACTPARA		pa;
	PSYSACC_HYDR		hy;
	PSYSACC_STATE		st;

	ACT					act[SYSACC_ACT_SN_END - 1];
}SYSACC, *PSYSACC;

#define SYSACC_INIT_INFO	{{PO_ID, CAL_LEN(SYSACC_PO)}, {PI_ID, CAL_LEN(SYSACC_PI)},  {PA_ID, CAL_LEN(SYSACC_ACTPARA)}, {HY_ID, CAL_LEN(SYSACC_HYDR)}, {ST_ID, CAL_LEN(SYSACC_STATE)}}


BOOL InitSysAccPart(PSYSACC pp, UI8 sn);

#ifdef __cplusplus
}
#endif

#endif
