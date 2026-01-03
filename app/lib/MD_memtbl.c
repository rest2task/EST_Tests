#include "MD_memtbl.h"
#include "stdio.h"
#include "string.h"

DEF_ALIGN UI8 MD_MemPoll[MD_MEMPOLL_LEN];

PMD_MEM_LINK pMemPoll_hd = NULL;
MD_MEM_STATE MemPollState;


/*******************************************************************************************
 *Brief			:缓存管理器初始化
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20221020
********************************************************************************************/
void InitMemPoll()
{
	pMemPoll_hd = (PMD_MEM_LINK)MD_MemPoll;

	pMemPoll_hd->next = pMemPoll_hd;	
	pMemPoll_hd->prev = pMemPoll_hd;
	pMemPoll_hd->state = 0;
	pMemPoll_hd->len = MD_MEMPOLL_LEN;

	MemPollState.app_num = 0;
	MemPollState.idle_rate = 100;	//初始化 100%空闲
	MemPollState.min_idle_rate = 100;	//初始化 100%空闲
	MemPollState.max_mem = MD_MEMPOLL_LEN;
	MemPollState.idle_mem = MD_MEMPOLL_LEN;
	MemPollState.fail_num = 0;
}

/*******************************************************************************************
 *Brief			:新增链表
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20221020
********************************************************************************************/
void MemLinkInsert(PMD_MEM_LINK plnk, PMD_MEM_LINK pnew)
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

/*******************************************************************************************
 *Brief			:删除链表
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20221020
********************************************************************************************/
void MemLinkRemove(PMD_MEM_LINK plnk)
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

/*******************************************************************************************
 *Brief			:申请缓存:缓存长度4字节对齐
 *Param			:len 缓存要求的长度
 *Return Value	:缓存地址
 *Aauthor		:ZT
 *Date			:20221020
********************************************************************************************/
UI8* m_malloc(UI16 len)
{
	PMD_MEM_LINK pMemPoll_now;
	PMD_MEM_LINK pMemPoll_new;
	UI16	found = 0;
	UI16	len_hd = sizeof(MD_MEM_LINK);

	if(len == 0)
		return NULL;

	//第一次申请缓存时完成初始化
	if(pMemPoll_hd == NULL)
		InitMemPoll();

	len = len_hd + len;

	//需要保证申请的缓存长度4字节对齐，但是
	if(len&0x03)
		len = (len&(~(UI16)0x03)) + 0x04;

	//查找符合要求的空闲内存
	for(pMemPoll_now = pMemPoll_hd; ; pMemPoll_now = pMemPoll_now->next)
	{
		if(pMemPoll_now->state == 0 && len <= pMemPoll_now->len)
		{
			found = 1;
			break;
		}

		if(pMemPoll_now->next == pMemPoll_hd)
			break;
	}

	//无相应缓存可以使用
	if(found == 0)
	{
		MemPollState.fail_num++;
		return NULL;
	}
	
	if((pMemPoll_now->len - len) > len_hd)
	{	//内存空间可以拆分	
		//多余空间独立一个新的可用存储单元
		pMemPoll_new = (PMD_MEM_LINK)((UI8*)pMemPoll_now + len);
		MemLinkInsert(pMemPoll_now, pMemPoll_new);

		pMemPoll_new->len = pMemPoll_now->len - len;
		pMemPoll_new->state = 0;

		//被申请的存储单元初始化
		pMemPoll_now->len = len;
	}

	pMemPoll_now->state = 1;	//标记使用
	memset((UI8*)pMemPoll_now + len_hd, 0, len - len_hd);

	//状态
	MemPollState.app_num++;
	MemPollState.idle_mem = MemPollState.idle_mem - pMemPoll_now->len;
	MemPollState.idle_rate = MemPollState.idle_mem*100 / MemPollState.max_mem;
	if(MemPollState.idle_rate < MemPollState.min_idle_rate)
		MemPollState.min_idle_rate = MemPollState.idle_rate;
	
	return (UI8*)pMemPoll_now + len_hd;
}

/*******************************************************************************************
 *Brief			:释放缓存:在释放缓存的同时可以合并相邻的空闲空间
 *Param			:缓存中的其中一个地址
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20221020
********************************************************************************************/
void m_free(UI8* pp)
{
	PMD_MEM_LINK pMemPoll_now = NULL;
	UI16	len_buff = 0;
	UI16	found = 0;

	if(pp == NULL)
		return;

	//查找符合要求的空闲内存
	for(pMemPoll_now = pMemPoll_hd;; pMemPoll_now = pMemPoll_now->next)
	{
		if((UI32)pMemPoll_now <= (UI32)pp && (UI32)pp < ((UI32)pMemPoll_now + pMemPoll_now->len))
		{
			found = 1;
			break;
		}
		
		if(pMemPoll_now->next == pMemPoll_hd)
			break;
	}

	if(found == 0 || pMemPoll_now->state == 0)
		return;

	len_buff = pMemPoll_now->len;
	pMemPoll_now->state = 0;	//释放该内存区域

	if(pMemPoll_now == pMemPoll_hd)
	{//首个内存空间&后一个内存空间为空	当前释放的内存空间与后一个进行合并
		if((pMemPoll_now->next != pMemPoll_now) && (pMemPoll_now->next->state == 0))
		{
			pMemPoll_now->len = pMemPoll_now->next->len + pMemPoll_now->len;
			MemLinkRemove(pMemPoll_now->next);
		}
	}
	else
	{
		if(pMemPoll_now->prev->state == 0)
		{//向前合并缓存
			pMemPoll_now = pMemPoll_now->prev;
			pMemPoll_now->len = pMemPoll_now->next->len + pMemPoll_now->len;
			MemLinkRemove(pMemPoll_now->next);
		}

		if((pMemPoll_now->next != pMemPoll_hd) && (pMemPoll_now->next->state == 0))
		{//非尾部空间向后合并缓存
			pMemPoll_now->len = pMemPoll_now->next->len + pMemPoll_now->len;
			MemLinkRemove(pMemPoll_now->next);
		}
	}

	//状态
	MemPollState.app_num--;
	MemPollState.idle_mem = MemPollState.idle_mem + len_buff;
	MemPollState.idle_rate = MemPollState.idle_mem*100 / MemPollState.max_mem;
}

UI16 GetMdMemIdleRateNow()
{
	return MemPollState.idle_rate;
}

UI16 GetMdMemAppNumNow()
{
	return MemPollState.app_num;
}

UI16 GetMdMemMaxMem()
{
	return MemPollState.max_mem;
}

UI16 GetMdMemMinIdleRateNow()
{
	return MemPollState.min_idle_rate;
}

UI16 GetMdMemFailNum()
{
	return MemPollState.fail_num;
}

MD_MEM_STATE GetMdMemPollState()
{
	return MemPollState;
}

void MD_MemPrint()
{
	PMD_MEM_LINK pMemPoll_now = NULL;
	UI16 no = 0;
	printf("MD_MemPrint:\r\n");
	
	//查找符合要求的空闲内存
	for(pMemPoll_now = pMemPoll_hd;; pMemPoll_now = pMemPoll_now->next)
	{
		printf("Mem %d:%d startaddr %04X -> %04X\r\n",no,pMemPoll_now->state, (UI32)pMemPoll_now,(UI32)pMemPoll_now + pMemPoll_now->len);
		
		if(pMemPoll_now->next == pMemPoll_hd)
			break;
	}

	printf("MD_MemPrint End\r\n");
}

void MD_MemTest()
{	
	InitMemPoll();
	
	while(1)
	{
		UI8* p1,*p2,*p3,*p4,*p5,*p6,*p7,*p8;

		p1 = m_malloc(1024);
		MD_MemPrint();
		p2 = m_malloc(512);
		MD_MemPrint();
		p3 = m_malloc(1024);
		MD_MemPrint();
		p4 = m_malloc(20);
		MD_MemPrint();
		p5 = m_malloc(25);
		MD_MemPrint();
		p6 = m_malloc(100);
		MD_MemPrint();

		m_free(p3);
		MD_MemPrint();
		m_free(p5);
		MD_MemPrint();
		m_free(p4);
		MD_MemPrint();
		
		p7 = m_malloc(500);
		MD_MemPrint();
		p8 = m_malloc(1024);
		MD_MemPrint();

		m_free(p1);
		MD_MemPrint();
		m_free(p6);
		MD_MemPrint();
		m_free(p2);
		MD_MemPrint();

		m_free(p8);
		MD_MemPrint();
		m_free(p7);
		MD_MemPrint();
	}
}

