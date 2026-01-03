#ifndef _AUTORUNWITHOUTSERVO_H_
#define _AUTORUNWITHOUTSERVO_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "acttype.h"

//#define BOGUS 1

extern WORD AD_BUFFER_AUTO[4];


void	EcatPdoRecvDataExplain_Auto(void);

#ifdef __cplusplus
}
#endif

#endif

