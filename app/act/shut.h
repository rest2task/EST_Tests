#ifndef _SHUT_H_
#define _SHUT_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include	"acttype.h"
#include	"shut_dic.h"
#include 	"part.h"
#include	 "clamp.h"
#include	 "inject.h"

typedef struct tyINJECT* PINJECT;
typedef struct tySHUT
{
	PART			part;
	
	PSHUT_PO			po;
	PSHUT_PI			pi;
	PSHUT_MACHSET	mh;
	PSHUT_MOLDSET	md;
	PSHUT_ACTPARA	pa;
	PSHUT_HYDR		hy;
	PSHUT_STATE		st;

	PCLAMP          pclamp;
	
	ACT					act[SHUT_ACT_SN_END - 1];
	PINJECT         pinj;
}SHUT, *PSHUT;


#define SHUT_INIT_INFO	{{PO_ID, CAL_LEN(SHUT_PO)}, {PI_ID, CAL_LEN(SHUT_PI)}, {MH_ID, CAL_LEN(SHUT_MACHSET)}, {MD_ID, CAL_LEN(SHUT_MOLDSET)}, \
							 {PA_ID, CAL_LEN(SHUT_ACTPARA)}, {HY_ID, CAL_LEN(SHUT_HYDR)}, {ST_ID, CAL_LEN(SHUT_STATE)}}
							 
BOOL InitShutPart(PSHUT pp, UI8 sn);	
UI32 ShutCmd(PSHUT pp, UI32 cmd, UI32 para);


#ifdef __cplusplus
}
#endif

#endif


