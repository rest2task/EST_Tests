#ifndef _QCMONITOR_H_
#define _QCMONITOR_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include	"acttype.h"
#include	"qcmonitor_dic.h"
#include	"eject.h"
#include	"clamp.h"
#include	"inject.h"



#define QC_MAX_EJECT 1


typedef struct tyQCMONITOR
{
	PART			part;

	PQCMONITOR_MOLDSET	md;
	PQCMONITOR_STATE	st;

	/*part reference*/
	PEJECT		pejt[QC_MAX_EJECT];
	PCLAMP		pclmp;
	PINJECT		pinj;


	//machine
	UI16		qcparasend;
	UI32		qc_avg_cyctm;
	UI16		qc_chk_start;

	//inject
	UI32        qc_avg_injtm;
	UI32        qc_avg_injspd;
	UI32        qc_avg_injend;
	UI32        qc_avg_injpres;
	UI32        qc_avg_holdend;
	UI32        qc_avg_chgtm;	
	UI32        qc_avg_chgend;
	UI32        qc_avg_sucktm;
	UI32        qc_avg_suckend;


	//clamp
	UI16        qc_pos_opnstart;
	UI32        qc_avg_opnend;
	UI32        qc_avg_opntm;
	UI32        qc_avg_opnspd;
	UI32        qc_avg_opnclstm;	
	UI32        qc_avg_opnclslow;			
	UI32        qc_avg_opnclshi;		

	//eject
	UI32        qc_avg_ejttm;

}QCMONITOR, *PQCMONITOR;


#define QCMONITOR_INIT_INFO		{ {MD_ID, CAL_LEN(QCMONITOR_MOLDSET)},{ST_ID, CAL_LEN(QCMONITOR_STATE)}}


void GetClampClsQc(PCLAMP pclmp);
void GetClampOpnQc(PCLAMP pclmp);
void GetInjectQc(PINJECT pinj);
void GetChargeQc(PINJECT pinj);
void GetSuckbackQc(PINJECT pinj);
void GetEjectQc(PEJECT pejt);
BOOL InitQcMonitorPart(void);
void GetMachineQc(void);
BOOL GetQcPara(void);

void QcCntAdd(void);
PQCMONITOR	GetQc(void);







#ifdef __cplusplus
}
#endif

#endif


