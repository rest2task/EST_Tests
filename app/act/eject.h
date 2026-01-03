#ifndef _EJECT_H_
#define _EJECT_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include	"acttype.h"
#include	"eject_dic.h"

#define EJECT_SEG 3				//托模动作段数	托模进一段，二段以及走调模压力流量时的第3段
#define EJECT_SEG_end  3		//托模动作终止位置


typedef struct tyCLAMP* PCLAMP;
typedef struct tyROTATE* PROTATE;
typedef struct tyEJECT
{
	PART			part;
	
	PEJECT_PO		po;
	PEJECT_PI		pi;
	PEJECT_MACHSET	mh;
	PEJECT_MOLDSET	md;
	PEJECT_ACTPARA	pa;
	PEJECT_HYDR		hy;
	PEJECT_STATE	st;

	ACT				act[EJECT_ACT_SN_END - 1];

	PCLAMP			pclmp;
		
	PROTATE			prtt;
	
	UI16			ejectcnt;
	UI32            qc_avg_ejttm;
	UI32			TM_start;

	
	UI16		EjectFwd_Pres1;	//托模进一段压力
	UI16		EjectFwd_Pres2; //托模进一段位置
	UI16		EjectFwd_Flow1; //托模进一段速度
	UI16		EjectFwd_Flow2; //托模进二段速度
	UI16		EjectFwd_Pos1;  //托模进一段位置
	UI16		EjectFwd_Pos2;	//托模进二段位置
	UI16		EjectFwd_Pos_First;	//首次托模进独立行程时，托进一段位置
	UI16		EjectFwd_Pos_Two;	//首次托模进独立行程时，托进二段位置
		
	UI16		EjectBwd_Flow1; //托模退一段速度
	UI16		EjectBwd_Flow2; //托模退二段速度
	UI16		EjectBwd_Flow3;	//电子尺已经到位，但行程开关还未到位时的调模托模的速度
	
	UI16		EjectBwd_Pos1; //托模退一段位置
	UI16		EjectBwd_Pos2; //托模退二段位置
	UI16		EjectBwd_Pos3; //托模退三段位置，该位置无，实际和二段终止位置一致
	UI16		EjectBwd_Pos_First;	//首次托模退独立行程时，托退一段位置
	UI16		EjectBwd_Pos_Two;	//首次托模退独立行程时，托退二段位置

	UI16		Eject_Flow[EJECT_SEG];  //存放托模速度数据
	UI16		Eject_Pos[EJECT_SEG];   //存放托模位置数据
	UI16		End_ejectpos;	//托模动作的终止位置
	UI16		Eject_Num;		//托模终止位置段数

	UI16		spdramp_ejtfwd[3];
	UI16		spdramp_ejtbwd[3];
	UI16		pressramp_ejtfwd[3];
	UI16		pressramp_ejtbwd[3];
	
	UI16		INJEJECT_TMDLY;
	UI16		INJEJECT_TM;
}EJECT, *PEJECT;

#define EJECT_INIT_INFO      {{PO_ID, CAL_LEN(EJECT_PO)}, {PI_ID, CAL_LEN(EJECT_PI)}, {MH_ID, CAL_LEN(EJECT_MACHSET)}, {MD_ID, CAL_LEN(EJECT_MOLDSET)}, \
							 {PA_ID, CAL_LEN(EJECT_ACTPARA)}, {HY_ID, CAL_LEN(EJECT_HYDR)}, {ST_ID, CAL_LEN(EJECT_STATE)}}


typedef enum enEJECT_RAMP_SEG
{
	//托进斜率
	EJT_RAMP_FWD_START = 0,
	EJT_RAMP_FWD_MID,
	EJT_RAMP_FWD_END,

	//托退斜率
	EJT_RAMP_BWD_START = 0,
	EJT_RAMP_BWD_MID,
	EJT_RAMP_BWD_END,		
}EJECT_RAMP_SEG;

/*funtion interface for extern module*/
BOOL EjectBwdErrChk(PEJECT pp, BOOL bset_err);//LY
BOOL EjectFwdErrChk(PEJECT pp,  BOOL bset_err);//LY
void EjectClampClsBwdVlvOff(PEJECT pp);
void EjectClampClsVlvOn(PEJECT pp);
BOOL InitEjectPart(PEJECT pp, UI8 sn);
UI16 EjectGetMode(PEJECT pp);
UI32 EjectCmd(PEJECT pp, UI32 cmd, UI32 para);
void SetEjtFwdOff(PEJECT pp, PACT pact);
BOOL EjectWaitOpenCoreBwd(PEJECT pp);  //脱模前等待联动中子退  QRC.20170419
ACT_RET EjectFwdBackgrd(PEJECT pp, PACT pact);
ACT_RET EjectBwdBackgrd(PEJECT pp, PACT pact);
ACT_RET EjfFwdPreChk_CBAC(void);


#ifdef __cplusplus
}
#endif

#endif



