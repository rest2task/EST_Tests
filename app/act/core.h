#ifndef _CORE_H_
#define _CORE_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include	"typedef.h"
#include  	"part.h"
#include	"core_dic.h"

typedef struct tyCLAMP* PCLAMP;
typedef struct tyCORE
{
	PART			part;
	
	PCORE_PO		po;
	PCORE_PI		pi;
	PCORE_MACHSET	mh;
	PCORE_MOLDSET	md;
	PCORE_ACTPARA	pa;
	PCORE_HYDR		hy;
	PCORE_STATE		st;

	ACT				act[CORE_ACT_SN_END - 1];

	PCLAMP			pclmp;
	UI16			sn;

	UI8				bfwdend;			/**/
	PICNT_ST		sc_chk_cnt;			/*咬牙计数*/
	
	UI8				corests;     //中子状态： 0，中间位置 ；1，进到位 ；2，退到位  DK 2015.0108 	

	UI8				corestsf;	  //特殊中子1 ，保证脱模时中子进退只做一次。DK 2014.1104
	UI8				corestsb;	  //特殊中子1 ，保证脱模时中子进退只做一次。DK 2014.1104
	UI16            CoreChk;      //0 不检查中子进退终高八位置1代表检查中子进，低八位置1代表检查中子退

	UI16			opn_ramp_start;		//开模启动斜率ms
	UI16			opn_ramp_end;		//开模结束斜率ms
			
	UI16			cls_ramp_start;		//关模启动斜率ms
	UI16			cls_ramp_end;		//关模结束斜率ms

	UI16            CoreFwdActualSequence; //实际中子进顺序
	UI16            CoreBwdActualSequence; //实际中子退顺序

    UI16            coreReleaseDlyTm;
    UI16            CoreRls_Step;
}CORE, *PCORE;

#define CORE_INIT_INFO		{{PO_ID, CAL_LEN(CORE_PO)}, {PI_ID, CAL_LEN(CORE_PI)}, {MH_ID, CAL_LEN(CORE_MACHSET)}, {MD_ID, CAL_LEN(CORE_MOLDSET)}, \
							 {PA_ID, CAL_LEN(CORE_ACTPARA)}, {HY_ID, CAL_LEN(CORE_HYDR)}, {ST_ID, CAL_LEN(CORE_STATE)}}

BOOL CoreStateErrChk(PCORE pp, BOOL bfwd);		
void CoreFwdPOSet(PCORE pp, BOOL bset);
void CoreFwdOff(PCORE pp);
BOOL InitCorePart(PCORE pp, UI8 sn);
UI32 CoreCmd(PCORE pp, UI32 cmd, UI32 para);
void InitCoreReach(void);
void CoreRleaseLoop(void);


#ifdef __cplusplus
}
#endif

#endif

