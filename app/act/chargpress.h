#ifndef _CHARGPRESS_H_
#define _CHARGPRESS_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "acttype.h"
#include "chargpress_dic.h"

typedef struct tyINJECT* PINJECT;
typedef struct tyCHGPRS
{
	PART				part;
	
	PCHGPRS_PO			po;
	PCHGPRS_PI			pi;
	PCHGPRS_ACTPARA		pa;
	PCHGPRS_HYDR		hy;
	PCHGPRS_STATE		st;

	PINJECT				pinj;
	
	ACT					act[CHGPRS_ACT_SN_END - 1];

    UI16				chgprsflag;
}CHGPRS, *PCHGPRS;

#define CHGPRS_INIT_INFO	 {{PO_ID, CAL_LEN(CHGPRS_PO)}, {PI_ID, CAL_LEN(CHGPRS_PI)},  {PA_ID, CAL_LEN(CHGPRS_ACTPARA)}, {HY_ID, CAL_LEN(CHGPRS_HYDR)}, \
							 {ST_ID, CAL_LEN(CHGPRS_STATE)}}
							 
void SetChargePressFwdPO(PCHGPRS pp, BOOL bon);
PI_STATUS ChargePressLvlPIChk(PCHGPRS pp);
PI_STATUS ChargePressFwdPIChk(PCHGPRS pp);
BOOL InitChgPressPart(PCHGPRS pp, UI8 sn);


#ifdef __cplusplus
}
#endif

#endif



