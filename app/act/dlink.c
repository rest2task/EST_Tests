/******************************************************************************
  文 件 名   : dlink.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 提供链表和双向链表功能
  函数列表   :
              DLinkDeQ
              DLinkHDInit
              DLinkHead
              DLinkInQ
              DLinkInsert
              DLinkInsertBefore
              DLinkLen
              DLinkPop
              DLinkPush
              DLinkRemove
              DLinkTail
  修改历史   :
  1.日    期   : 2013年5月17日
    作    者   : ren chaohong
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

#include "dlink.h"

/* insert a link to double link, pnew will insert after plnk */
void DLinkInsert(D_LINK *plnk, D_LINK *pnew)
{
	if(plnk == NULL || pnew == NULL)
		return;
		
	pnew->next = plnk->next;

	/* if the plnk is not the head of double link*/
	if(plnk->next != NULL)
	{
		plnk->next->prev = pnew;
	}

	plnk->next = pnew;
	pnew->prev = plnk;
}

/* insert a link to double link, pnew will insert before plnk */
void DLinkInsertBefore(D_LINK *plnk, D_LINK *pnew)
{
	if(plnk == NULL || pnew == NULL)
		return;
		
	pnew->prev = plnk->prev;

	/* if the plnk is not the tail of double link*/
	if(plnk->prev != NULL)
	{
		plnk->prev->next = pnew;
	}

	pnew->next = plnk;
	plnk->prev = pnew;
}

/* remove a link from double link*/
void DLinkRemove(D_LINK *plnk)
{
	if(plnk == NULL)
		return;

	/* if the plnk is not the head of double link*/
	if(plnk->prev != NULL)
	{
		plnk->prev->next = plnk->next;
	}

	/* if link is not the tail of double link*/
	if(plnk->next != NULL)
	{
		plnk->next->prev = plnk->prev;
	}
	
	plnk->next = NULL;
	plnk->prev = NULL;
}

/* add a link to queue tail */
D_LINK * DLinkInQ(D_LINK_HD *phd, D_LINK *plnk)
{
	if(phd == NULL || plnk == NULL)
		return NULL;
		
	DLinkInsert(&phd->hd, plnk);
	phd->len++;
	return plnk;
}

/*get a link from queue head*/
D_LINK * DLinkDeQ(D_LINK_HD *phd)
{
	D_LINK 	*plnk = NULL;

	if(phd == NULL)
		return NULL;
		
	if(phd->hd.prev != &phd->hd)
	{
		plnk = phd->hd.prev;
		DLinkRemove(plnk);
		phd->len--;
	}

	return plnk;
}

/* add a link to queue tail */
D_LINK * DLinkPush(D_LINK_HD *phd, D_LINK *plnk)
{
	if(phd == NULL || plnk == NULL)
		return NULL;
		
	DLinkInsertBefore(&phd->hd, plnk);
	phd->len++;
	return plnk;
}

/*get a link from queue tail*/
D_LINK * DLinkPop(D_LINK_HD *phd)
{
	D_LINK 	*plnk = NULL;

	if(phd == NULL)
		return NULL;
		
	if(phd->hd.next != &phd->hd)
	{
		plnk = phd->hd.next;
		DLinkRemove(plnk);
		phd->len--;
	}

	return plnk;
}

void  DLinkHDInit(D_LINK_HD *phd)
{
	phd->hd.next = &phd->hd;
	phd->hd.prev = &phd->hd;
	phd->len = 0;
}

D_LINK* DLinkTail(D_LINK_HD *phd)
{
	if(phd->hd.next != &phd->hd)
		return phd->hd.next;
	else
		return NULL;
}

D_LINK* DLinkHead(D_LINK_HD *phd)
{
	if(phd->hd.prev != &phd->hd)
		return phd->hd.prev;
	else
		return NULL;
}

int DLinkLen(D_LINK_HD *phd)
{
	return phd->len;
}
