#ifndef _VTEST_H_
#define _VTEST_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "part.h"
#include "actctrl.h"
#include "movectrl.h"
#include "acttbl.h"

BOOL VTestGetAdValue(UI16 portno, UI16 *val);
void VTestActFlowSet(PACT pact, UI16 flow);
void VTestFlowReset(void);
void VTESTWarmOn(void);
void VTESTWarmOff(void);
void VTESTSurTempUV(UI16 uv);
void VTESTSetCurCh(UI8 ch);
void VTESTChHeatOnOff(UI8 ch, BOOL bon);
UI16 VTESTGetTempUV(void);
void VTESTTempLoop(void);
void VTestTempReset(void);
void VTESTAutoStart(void);
UI32 VTESTPISet(UI32 pi_st);

#ifdef __cplusplus
}
#endif

#endif


