#ifndef _SAFEDOOR_H_
#define _SAFEDOOR_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "part.h"
#include "safedoor_dic.h"
//#include "clamp.h"

#define SAFEDOOR_SPECIALCHK_ERR_TM	2000    /*ms*/

typedef struct tag_SAFEDOORCHK
 {
    WORD       chkoper;
    BOOL       dooropn;
    WORD       chkcnt;
    WORD       chkcntset;
    BOOL       doormecsaf;	// 检测安全检知输入点
} SAFEDOORCHK, *PSAFEDOORCHK;  	  
  

typedef struct tyCLAMP* PCLAMP;
typedef struct tySAFEDOOR
{
	PART					part;
	
	PSAFEDOOR_PO			po;
	PSAFEDOOR_PI			pi;
	PSAFEDOOR_MACHSET		mh;
	PSAFEDOOR_MOLDSET		md;
	PSAFEDOOR_ACTPARA		pa;
	PSAFEDOOR_HYDR			hy;
	PSAFEDOOR_STATE		st;

	ACT						act[SAFEDOOR_ACT_SN_END];

	/*安全门特殊检测*/
	UI8						chkerr;						
	UI8						checkstep;
	UI8						cls_err;
	UI16					checkstart;
	PCLAMP					pclmp;

	SAFEDOORCHK				chk_st;
	UI16                    bsafedoor_alarm_10ms;
	UI16					bsafedoor_alarm;	//如果为1说明操作过重置 0未操作	注意 这是专用数据，其它地方不得使用
	
	UI16                    SQ42checkflag;
	UI16                    SQ42checktime;
	UI16                    sfvchecktime;

	UI16                    sferrfiltime;
	UI16                    sferroutbit;
	
    UI16                    clampdoorclose;
	UI16                    SemiTurnTime;       //从半自动切到全自动后置1，其他时候清零
    UI16                    i_safe_remerg_last;
	UI16                    i_safe_remerg_temp;
	UI16                    i_safe_remerg_starttm;

	UI16					Tm_TurntoManule;
}SAFEDOOR, *PSAFEDOOR;

#define SAFEDOOR_INIT_INFO	{{PO_ID, CAL_LEN(SAFEDOOR_PO)}, {PI_ID, CAL_LEN(SAFEDOOR_PI)}, {MH_ID, CAL_LEN(SAFEDOOR_MACHSET)}, {MD_ID, CAL_LEN(SAFEDOOR_MOLDSET)}, \
							 {PA_ID, CAL_LEN(SAFEDOOR_ACTPARA)}, {HY_ID, CAL_LEN(SAFEDOOR_HYDR)}, {ST_ID, CAL_LEN(SAFEDOOR_STATE)}}

BOOL BSafeDoorIsErr(PSAFEDOOR pp);
BOOL FSafeDoorIsClose(PSAFEDOOR pp);
BOOL FSafeDoorIsOpen(PSAFEDOOR pp);
BOOL FSafeDoorIsCloseErrChk(PSAFEDOOR pp);
void SpecialSafeDoorCheck(PSAFEDOOR pp);
void SafeDoorAlarmLoop(PSAFEDOOR pp);
BOOL InitSafeDoorPart(PSAFEDOOR pp, UI8 sn);
PSAFEDOORCHK GetSafeDorrChkState(PSAFEDOOR pp);
UI32 SafeDoorCmd(PSAFEDOOR pp, UI32 cmd, UI32 para);
void SafeDoorOpenChk(PSAFEDOOR pp, PACT pact);
BOOL SafeDoorValveAlarm(PSAFEDOOR pp);
void ManualSafeDoorErrOut(void);
void SafeDoorErr(void);
#ifdef __cplusplus
}
#endif

#endif


