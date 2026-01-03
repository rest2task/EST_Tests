#ifndef _OIL_H_
#define _OIL_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include	"part.h"
#include "oil_dic.h"

typedef struct tyOIL
{
	PART			part;

	POIL_PO			po;
	POIL_PI			pi;
	POIL_MACHSET	mh;
	POIL_STATE		st;

	UI16		TM_START;				//漏油检测起始时间 20190409
}OIL, *POIL;

#define OIL_INIT_INFO	{{PO_ID, CAL_LEN(OIL_PO)}, {PI_ID, CAL_LEN(OIL_PI)}, {MH_ID, CAL_LEN(OIL_MACHSET)}, {ST_ID, CAL_LEN(OIL_STATE)}}

BOOL OilLvErr(void);
BOOL OilFltErr(void);
void OilCoolerChk(void);
BOOL OilCmd(UI32 cmd, UI32 para);
BOOL OilTmpErr(void);     
BOOL InitOilPart(void);
void CheckOilLeak(void);
BOOL WaterTmpErr(void);

#ifdef __cplusplus
}
#endif

#endif
