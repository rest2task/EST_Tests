#ifndef _AUTOCTRL_H_
#define _AUTOCTRL_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include	"acttype.h"
#include	"autoctrl_dic.h"


typedef enum enSENSOR_CHK_ST
{
	SENSOR_CHK_INIT = 0,
	SENSOR_CHK_START,
	SENSOR_CHK_TMOUT
}SENSOR_CHK_ST;

typedef struct tyCLAMP* PCLAMP;
typedef struct tyAUTOCTRL
{
	PART				part;
	
	PAUTOCTRL_PI		pi;
	PAUTOCTRL_MOLDSET	md;
	PAUTOCTRL_STATE		st;
	
	ACT 		act[AUTOCTRL_ACT_SN_END -1];


	PCLAMP		pclmp;
	
	UI16		pi_chk_ok[MAX_SENSOR_PI_NUM];
	UI16		chk_pi_num;
	UI16		bdrop_ok;
	UI16		chk_st;
	UI16		chk_start_10ms;

	UI16		chk_set_cnt;
	UI16        chk_err_cnt;   //电眼检出失败次数
}AUTOCTRL, *PAUTOCTRL;

#define AUTOCTRL_INIT_INFO		{{PI_ID, CAL_LEN(AUTOCTRL_PI)}, {MD_ID, CAL_LEN(AUTOCTRL_MOLDSET)}, {ST_ID, CAL_LEN(AUTOCTRL_STATE)}}

/*外部接口*/
UI32 AutoCtrlCmd(PAUTOCTRL pp, UI32 cmd, UI32 para);
BOOL SensorErrChk(PAUTOCTRL pp);
BOOL InitAutoCtrlPart(PAUTOCTRL pp, UI8 sn);
BOOL SensorDropIsOK(PAUTOCTRL pp);
void SensorDropChkReset(PAUTOCTRL pp);


#ifdef __cplusplus
}
#endif

#endif
