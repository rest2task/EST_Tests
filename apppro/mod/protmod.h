#ifndef _PROTMOD_H
#define _PROTMOD_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "prot.h"

void ModPktRecv(PPROT_HD phd, UI8 prot_id);
BOOL GetMainHand();
void InitMainHand();

#ifdef __cplusplus
}
#endif

#endif




