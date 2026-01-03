#ifndef _COREPROTECT_H_
#define _COREPROTECT_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include	"typedef.h"
#include  	"part.h"

void CoreStatusJudge(PCORE pp);
void CoreStatusLoop(void);
BOOL CoreFwdErr(PCORE pp);
BOOL CoreBwdErr(PCORE pp);
BOOL CoreLimitSwitchErrCheck(PCORE pp);
BOOL EjectFwdCoreStateErrCheck(PCORE pp);
BOOL EjectBwdCoreStateErrCheck(PCORE pp);
BOOL CoreFBwdEjectStateErrCheck(PCORE pp, BOOL isfwd);
BOOL CoreFwdValidPositionErrCheck(PCORE pp);
BOOL CoreBwdValidPositionErrCheck(PCORE pp);
BOOL ClampClsCoreStateErrCheck(PCORE pp, UI16 clmp_step);
BOOL ClampOpenCoreStateErrCheck(PCORE pp, UI16 clmp_step);
BOOL CoreInSequenceErrCheck(PCORE pp);
BOOL CoreOutSequenceErrCheck(PCORE pp);

#ifdef __cplusplus
}
#endif

#endif

