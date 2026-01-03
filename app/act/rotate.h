#ifndef _ROTATE_H_
#define _ROTATE_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include	"acttype.h"
#include	"rotate_dic.h"

//#define CLAMP_MAX_AIRBLOW	  8
//#define CLAMP_MAX_ROTATE		2
//#define ROTATE_MAX_EJECT		  2
//#define ROTATE_MAX_CORE		  4

#define    ROTATE_MAX_LOC		2

#define   TM_DEFAULTMIN			10
#define	TM_DEFAULTMAX		999
#define	ROTATE_DEFAULT_TM		600

typedef struct tyROTATE*  PROTATE;
typedef struct tyLOCK*	PLOCK;
typedef struct tyROTATE
{
	PART			part;

	PROTATE_PO		po;
	PROTATE_PI		pi;
	PROTATE_MACHSET	mh;
	PROTATE_ACTPARA	pa;
	PROTATE_HYDR	hy;
	PROTATE_STATE	st;

	ACT			act[ROTATE_ACT_SN_END -1];

	/*part reference*/
	PCLAMP      pclmp;
	PROTATE     pother_rtt;
	PLOCK		plck;

	UI8			last_act_sn;
	UI16        slidests  ;       // slide status           0  unstable  1 FWD end       2  BWD end   
}ROTATE, *PROTATE;

#define ROTATE_INIT_INFO    {{PO_ID, CAL_LEN(ROTATE_PO)}, {PI_ID, CAL_LEN(ROTATE_PI)}, {MH_ID, CAL_LEN(ROTATE_MACHSET)},  \
                                 {PA_ID, CAL_LEN(ROTATE_ACTPARA)}, {HY_ID, CAL_LEN(ROTATE_HYDR)}, {ST_ID, CAL_LEN(ROTATE_STATE)}}


BOOL InitRotatePart(PROTATE pp, UI8 sn);
BOOL RotateErr(PROTATE pp);
BOOL RotatePIErrChk(PROTATE pp, BOOL bchk_fwd);
BOOL RotateClampClsErrChk(PROTATE pp);
UI32 RotateCmd(PROTATE pp, UI32 cmd, UI32 para);

#ifdef __cplusplus
}
#endif

#endif

