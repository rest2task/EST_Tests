#ifndef _USERACT_H_
#define _USERACT_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "part.h"
#include "acttype.h"
#include "machine.h"
#include "motor.h"

#define USER_ACT_MAP_MAX		50

typedef struct tyUSER_ACT_MAP
{
	UI8	number;
	UI8	part_type;
	UI8	part_sn; 
	UI8	act_sn;
}USER_ACT_MAP, *PUSER_ACT_MAP;

typedef enum enUSER_ACT
{
	U_RECYCL,
	U_CLMP_OPN,
	U_CLMP_OPN_ADJ,
	U_CLMP_CLS,
	U_CLMP_CLS_ADJ,
	U_CLMP_HLD,
	U_INJECT,
	U_INJECT_ADJ,
	U_SUCK,
	U_SUCK_ADJ,
	U_CHRG,
	U_CHRG_ADJ,
	U_AUTO_PURG,
	U_SFD_CLS,
	U_SFD_OPN,
	U_NZL_FWD,
	U_NZL_FWD_ADJ,
	U_NZL_BWD,
	U_NZL_BWD_ADJ,
	U_EJT_FWD,
	U_EJT_FWD_ADJ,
	U_EJT_BWD,
	U_EJT_BWD_ADJ,
	U_EJTB_FWD,
	U_EJTB_FWD_ADJ,
	U_EJTB_BWD,
	U_EJTB_BWD_ADJ,

	U_COREA_IN,
	U_COREA_IN_ADJ,
	U_COREA_OUT,
	U_COREA_OUT_ADJ,
	
	U_COREB_IN,
	U_COREB_IN_ADJ,
	U_COREB_OUT,
	U_COREB_OUT_ADJ,
	
	U_COREC_IN,
	U_COREC_IN_ADJ,
	U_COREC_OUT,
	U_COREC_OUT_ADJ,

	U_CORED_IN,
	U_CORED_IN_ADJ,
	U_CORED_OUT,
	U_CORED_OUT_ADJ,

	U_CHRG_PRSF,
	U_CHRG_PRSB,
	
	U_LUB,
	U_LUBB,

	U_AIRBLOWA,
	U_AIRBLOWB,
	U_AIRBLOWC,
	U_AIRBLOWD,

	U_MOTOR_ON, 

	U_ADJ_FWD,
	U_ADJ_BWD,
	U_ADJ_AUTO,
	
	U_ACT_END
}USER_ACT;

#define USER_ACT_MAP_INFO	 	{{U_RECYCL, MACHINE_ID, 1, OS_RCYC}, 		\
								{U_CLMP_OPN, CLAMP_ID, 1, OS_OPNM}, 		\
								{U_CLMP_OPN_ADJ, CLAMP_ID, 1, OS_AOPN},	\
								{U_CLMP_CLS, CLAMP_ID, 1, OS_CLSM}, 		\
								{U_CLMP_CLS_ADJ, CLAMP_ID, 1, OS_ACLS}, 	\
								{U_CLMP_HLD, CLAMP_ID, 1, OS_PRSM}, 		\
								{U_INJECT, INJECT_ID, 1, OS_INJE}, 		\
								{U_INJECT_ADJ, INJECT_ID, 1, OS_AINJE}, 	\
								{U_SUCK, INJECT_ID, 1, OS_SUCK}, 			\
								{U_SUCK_ADJ, INJECT_ID, 1, OS_ASUCK}, 		\
								{U_CHRG, INJECT_ID, 1, OS_CHRG},			\
								{U_CHRG_ADJ, INJECT_ID, 1, OS_ACHRG},		\
								{U_AUTO_PURG, INJECT_ID, 1, OS_APURG},		\
								{U_SFD_CLS, SAFEDOOR_ID, 1, OS_SDRC},		\
								{U_SFD_OPN, SAFEDOOR_ID, 1, OS_SDRO},		\
								{U_NZL_FWD, NOZZLE_ID, 1, OS_NOZF},		\
								{U_NZL_FWD_ADJ, NOZZLE_ID, 1, OS_ANOZF},	\
								{U_NZL_BWD, NOZZLE_ID, 1, OS_NOZB},		\
								{U_NZL_BWD_ADJ, NOZZLE_ID, 1, OS_ANOZB},	\
								{U_EJT_FWD, EJECT_ID, 1, OS_EJT},			\
								{U_EJT_FWD_ADJ, EJECT_ID, 1, OS_AEJTF},	\
								{U_EJT_BWD, EJECT_ID, 1, OS_EJTB},			\
								{U_EJT_BWD_ADJ, EJECT_ID, 1, OS_AEJTB},	\
								{U_EJTB_FWD, EJECT_ID, 2, OS_EJT},			\
								{U_EJTB_FWD_ADJ, EJECT_ID, 2, OS_AEJTF},	\
								{U_EJTB_BWD, EJECT_ID, 2, OS_EJTB},			\
								{U_EJTB_BWD_ADJ, EJECT_ID, 2, OS_AEJTB},	\
								{U_COREA_IN, CORE_ID, 1, OS_CPRF},			\
								{U_COREA_IN_ADJ, CORE_ID, 1, OS_CPRAF},			\
								{U_COREA_OUT, CORE_ID, 1, OS_CPRB},		\
								{U_COREA_OUT_ADJ, CORE_ID, 1, OS_CPRAB},		\
								{U_COREB_IN, CORE_ID, 2, OS_CPRF},			\
								{U_COREB_IN_ADJ, CORE_ID, 2, OS_CPRAF},			\
								{U_COREB_OUT, CORE_ID, 2, OS_CPRB},		\
								{U_COREB_OUT_ADJ, CORE_ID, 2, OS_CPRAB},		\
								{U_COREC_IN, CORE_ID, 3, OS_CPRF},			\
								{U_COREC_IN_ADJ, CORE_ID, 3, OS_CPRAF},			\
								{U_COREC_OUT, CORE_ID, 3, OS_CPRB},		\
								{U_COREC_OUT_ADJ, CORE_ID, 3, OS_CPRAB},		\
								{U_CORED_IN, CORE_ID, 4, OS_CPRF},			\
								{U_CORED_IN_ADJ, CORE_ID, 4, OS_CPRAF},			\
								{U_CORED_OUT, CORE_ID, 4, OS_CPRB},		\
								{U_CORED_OUT_ADJ, CORE_ID, 4, OS_CPRAB},		\
								{U_CHRG_PRSF, CHGPRS_ID, 1, OS_CHGPRSF},\
								{U_CHRG_PRSB, CHGPRS_ID, 1, OS_CHGPRSB},\
								{U_LUB, LUB_ID, 1, OS_LUBR},				\
								{U_LUBB, LUB_ID, 2, OS_LUBR},				\
								{U_AIRBLOWA, AIRBLOW_ID, 1, OS_AIRBL},		\
								{U_AIRBLOWB, AIRBLOW_ID, 2, OS_AIRBL},		\
								{U_AIRBLOWC, AIRBLOW_ID, 3, OS_AIRBL},		\
								{U_AIRBLOWD, AIRBLOW_ID, 4, OS_AIRBL},		\
								{U_MOTOR_ON, MOTOR_ID, 1, OS_MOTON},		\
								{U_ADJ_FWD, ADJUST_ID, 1, OS_ADJF},			\
								{U_ADJ_BWD, ADJUST_ID, 1, OS_ADJB},			\
								{U_ADJ_AUTO, ADJUST_ID, 1, OS_ADJA}}

BOOL ChkUserActIsRun(int act_name);
BOOL RunUserAct(int act_name, UI32 para, UI16 dly_ms);
void StopUserAct(int act_name);
PPART GetUserActPart(int act_name);
int sprintf(char *buf, const char *fmt, ...);
int snprintf(char *buf, size_t size, const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
