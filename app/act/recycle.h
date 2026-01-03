#ifndef _RECYCLE_H_
#define _RECYCLE_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "machine.h"

ACT_RET RecycleProc(PACT pact, UI32 para);
ACT_RET PreCycleProc(PACT pact, UI32 para);
void CycleTmReset(PMACHINE pp);
void CyclePDInfoSend(PMACHINE pp);


#ifdef __cplusplus
}
#endif

#endif



