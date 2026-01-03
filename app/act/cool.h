#ifndef _COOL_H_
#define _COOL_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include	"acttype.h"

typedef struct tyCOOL
{
	PART			part;
	
	PCOOL_ACTPARA	pa;
	PCOOL_STATE		st;

	ACT					act[COOL_ACT_SN_END - 1];
}COOL, *PCOOL;

#ifdef __cplusplus
}
#endif

#endif
