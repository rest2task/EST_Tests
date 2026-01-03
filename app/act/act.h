#ifndef _ACT_H_
#define _ACT_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "acttype.h"


 void ActRunReactive(PACT pact, BOOL bend);
void ActRunHold(PACT pact);
void ActRunStart(PACT pact, PACT_TSK ptsk);
void ActRunEnd(PACT pact);
void ActRunStop(PACT pact);
ACT_RET ActRunLoop(PACT pact, UI32 para);
void ActErrorAbort(PACT pact);
BOOL InitAct(PPART  pp, PACT pact,  const ACT_INIT_INFO* pinit_info, int num);

int GetHydrPri(UI16 port, PACT pact);
UI16 GetHydrPort_Ch(UI16 port);
UI16 GetHydrPort(UI16 port, PACT pact);
UI16 GetHydrPump(UI16 pumps, PACT pact);
void ActStepNew(PACT pact, UI16 new_step);
void ActSetTM(PACT_TSK ptsk, UI32 act_tm);
void ActSetMov(ACT_OP_FUNC pmov, PPART pp, PACT pact);
void ActSetMov2(ACT_OP_FUNC pmov, PPART pp, PACT pact);

void ActSetMovTM(PACT_TSK ptsk, UI16 act_tm);
BOOL ActChkAndSetMovM(ACT_OP_FUNC pmov, PPART pp, PACT pact, UI16 m);
void ActSetHydr(PACT pact,UI16 mode, UI16 press, UI16 flow, UI16 pramp, UI16 framp, UI16 prampstart, UI16 frampstart, UI16 hydrdly, UI16 hydrport, 
					UI16 ServoPumpLmt, UI16 NormalPumpLmt, UI16 Servo_P, UI16 Servo_I, UI16 Servo_D, UI16 KiClear, UI16 Priority);
void ActSet_Da_Pres(PACT pact,UI16 mode, UI16 press, UI16 pramp, UI16 prampstart, UI16 hydrdly, UI16 hydrport, 
					UI16 NormalPumpLmt, UI16 PresFlag, UI16 KiClear, UI16 Priority,UI16 Direct);
void ActSet_Da_flow(PACT pact,UI16 mode, UI16 flow, UI16 framp, UI16 frampstart, UI16 hydrdly, UI16 hydrport, 
					UI16 NormalPumpLmt, UI16 PresFlag, UI16 KiClear, UI16 Priority,UI16 Direct);

void ActSetHydr_SpdSR(PACT pact,UI16 mode, UI16 press, UI16 flow, UI16 pramp, UI16 framp, UI16 prampstart, UI16 frampstart, UI16 hydrdly, UI16 hydrport, 
					UI16 ServoPumpLmt, UI16 NormalPumpLmt, UI16 Servo_P, UI16 Servo_I, UI16 PresFlag, UI16 KiClear, UI16 Priority, UI16 SpdDraw);

void ActClsHydr(PACT pact, UI16 pramp, UI16 framp);
void ActClsDa_Pres(PACT pact, UI16 port,UI16 pramp, WORD dire);
void ActClsDa_Flow(PACT pact, UI16 port,UI16 framp,WORD dire);


BOOL ActMovMsChk(PACT_TSK ptsk, UI32 start_tm, UI32 chk_tm);
BOOL ActMov100MsChk(PACT_TSK ptsk, UI32 start_tm, UI32 chk_tm);
BOOL ActMov10MsChk(PACT_TSK ptsk, UI32 start_tm, UI32 chk_tm);
BOOL ActMov100UsChk(PACT_TSK ptsk, UI32 start_tm, UI32 chk_tm);
BOOL ActMov100UsChk_INJ(PACT_TSK ptsk, UI32 start_tm, UI32 chk_tm);
int ActSetFlowDot(int dot);
int ActSetPresDot(int dot);
void ActClsHydrNoPact(UI16 last_hydr_port,UI16 last_hydr_pri, UI16 pramp, UI16 framp);


#define HYDR_PRI(port)				(GetHydrPri(port, pact))
#define HYDR_PORT(port)				(GetHydrPort(port, pact))
#define HYDR_PUMP(pumps)			(GetHydrPump(pumps, pact))
#define HYDR_CLS(pramp, framp)		(ActClsHydr(pact, pramp, framp))
#define HYDR_CLS_Da_P(port,pramp, direct)	(ActClsDa_Pres(pact,port,pramp,direct))
#define HYDR_CLS_Da_F(port,framp, direct)	(ActClsDa_Flow(pact,port,framp,direct))

#ifdef __cplusplus
}
#endif

#endif


