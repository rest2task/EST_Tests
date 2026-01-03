#ifndef _DLINK_H_
#define _DLINK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "typedef.h"

typedef struct tyD_LINK
{
	struct tyD_LINK	*next;
	struct tyD_LINK	*prev;
}D_LINK ;

typedef struct tyD_LINK_HD
{
	D_LINK	hd;
	UI32	len;
}D_LINK_HD;

void  DLinkHDInit(D_LINK_HD *phd);
D_LINK * DLinkInQ(D_LINK_HD *phd, D_LINK *plnk);
D_LINK * DLinkDeQ(D_LINK_HD *phd);
D_LINK * DLinkHead(D_LINK_HD *phd);
D_LINK * DLinkTail(D_LINK_HD *phd);
D_LINK * DLinkPush(D_LINK_HD *phd, D_LINK *plnk);
D_LINK * DLinkPop(D_LINK_HD *phd);

int DLinkLen(D_LINK_HD *phd);


void DLinkInsert(D_LINK *plnk, D_LINK *pnew);
void DLinkRemove(D_LINK *plnk);

#ifdef __cplusplus
}
#endif

#endif
