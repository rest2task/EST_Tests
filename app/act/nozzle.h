#ifndef _NOZZLE_H_
#define _NOZZLE_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#pragma pack()

#include "part.h"
#include "nozzle_dic.h"

#define NOZZLE_TM_MAX		999
typedef enum enNZ_ACT_MODE
{
	NZ_ACT_STD,						/*标准， 行程开关*/
	NZ_ACT_TM,							/*走时间*/
	NZ_ACT_FAST,						/*声海快速*/
	NZ_ACT_METER,						/*走电子尺*/
	NZ_ACT_NCTRL,						/*无控制*/
	NZ_ACT_NO							/*不做动作 */
}NZ_ACT_MODE;

typedef enum enNOZZLE_TYPE
{
	NZ_NO_USED = 0,				/*不使用*/
	NZ_AFT_CHRG,					/*储料后*/
	NZ_BF_CLMP_OPN,				/*开模前*/
	NZ_AFT_INJ						/*射出后*/
}NOZZLE_TYPE;

typedef struct tyINJECT* PINJECT;
typedef struct tyNOZZLE
{
	PART				part;
	
	PNOZZLE_PO			po;
	PNOZZLE_PI			pi;
	PNOZZLE_MACHSET	mh;
	PNOZZLE_MOLDSET	md;
	PNOZZLE_ACTPARA	pa;
	PNOZZLE_HYDR		hy;
	PNOZZLE_STATE		st;

	ACT					act[NOZZLE_ACT_SN_END - 1];

	PINJECT				pinj;

	UI8					mode;
	UI8					coolsts;   //冷却标记，1冷却开始， 0冷却结束。
}NOZZLE, *PNOZZLE;

#define NOZZLE_INIT_INFO	{{PO_ID, CAL_LEN(NOZZLE_PO)}, {PI_ID, CAL_LEN(NOZZLE_PI)}, {MH_ID, CAL_LEN(NOZZLE_MACHSET)}, {MD_ID, CAL_LEN(NOZZLE_MOLDSET)}, \
							 {PA_ID, CAL_LEN(NOZZLE_ACTPARA)}, {HY_ID, CAL_LEN(NOZZLE_HYDR)}, {ST_ID, CAL_LEN(NOZZLE_STATE)}}
							 

/*funtion interface for extern module*/
BOOL NozzleIsReach(PNOZZLE pp);
NOZZLE_TYPE NozzleType(PNOZZLE pp);
void SetNozzleFwdPO(PNOZZLE pp, BOOL bon);
BOOL NozzleFwdEndErrChk(PNOZZLE pp);
BOOL InitNozzlePart(PNOZZLE pp, UI8 sn);
UI32 NozzleCmd(PNOZZLE pp, UI32 cmd, UI32 para);

extern  UI32		coolstart;	
#ifdef __cplusplus
}
#endif

#endif

