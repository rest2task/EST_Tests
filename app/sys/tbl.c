/******************************************************************************
  文 件 名   : tbl.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 表的基本功能
  函数列表   :
              AddTblToInd
              TBLAlloc
              TBLClear
              TBLCopy
              TBLGet
              TBLGetInfo
              TBLGetRow
              TBLInit
              TBLInsertRow
              TBLMerge
              TBLReqRx
              TBLReqTx
              TBLRspRx
              TBLSort
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

#include "com.h"
#include "memtbl.h"
#include "machine.h"
#include "tbl.h"
#include "acttbl.h"
#include "actfork.h"
#include "prot.h"
#include "cfgdb.h"
#include "sram.h"

#pragma diag_suppress 1296
const static TBL_TYPE_INFO tbl_type_info[] = 
{
	{0, 0, 0, 0, 0, 0},
	{T_MACHINE_SUB, TBL_DB_VER, TBL_STOR|TBL_DYNC, SUB_PART_INFO_EXT_LEN, sizeof(SUB_PART_INFO), MAX_SUB_TBL_NUM},
	{T_E_PO, TBL_DB_VER, TBL_STOR, sizeof(E_PO_ROW), sizeof(E_PO_ROW), 0},
	{T_PI_LOG, TBL_DB_VER, TBL_STOR | TBL_USER, sizeof(PI_LOG), sizeof(PI_LOG), 0},
	{T_PO_LOG, TBL_DB_VER, TBL_STOR | TBL_USER, sizeof(PO_LOG), sizeof(PO_LOG), 0},
	{T_HYDR_PRI, TBL_DB_VER, TBL_STOR, sizeof(HY_PRI_ROW), sizeof(HY_PRI_ROW), 0},
	{T_ACT_FORK, TBL_DB_VER, TBL_STOR|TBL_DYNC, ACT_FORK_EXT_LEN, sizeof(ACT_FORK), 0},
	{T_KEY_MAP, TBL_DB_VER, TBL_STOR, 10, 10},
	{T_OPN_RAMP, TBL_DB_VER, TBL_STOR_PANEL|TBL_DYNC_SRAM,sizeof(AICLAMPDATA_TBROW),sizeof(AICLAMPDATA_TBROW),AICLP_ROW_NUM},
	
    {T_ROBOT_PI, TBL_DB_VER, TBL_STOR | TBL_USER, sizeof(ROB_PI_LOG), sizeof(ROB_PI_LOG), 0},
    {T_ROBOT_PO, TBL_DB_VER, TBL_STOR | TBL_USER, sizeof(ROB_PO_LOG), sizeof(ROB_PO_LOG), 0}
};
	
TBL_LST tbl_lst[MAX_TBL_LIST_NUM];
TBL_IND tbl_ind[T_TBL_END];
TBL	   tbl_dync[MAX_DYNC_TBL_NUM];
TBL_RX_ST tbl_rx_st;

void InitTBLInPanel()
{
	UI16 i = 0;
	TBL_TYPE_INFO tbl;
	for (i = 1;i < T_TBL_END;i++)
	{
		tbl = tbl_type_info[i];
		if (tbl.attr_bits&TBL_STOR_PANEL)	//数据存储在面板上，主机使用时需要放入内存
		{
			TBLAlloc(tbl.type,tbl.ver,0,tbl.attr_bits,tbl.max_row,tbl.row_len);
		}
	}
}

/*按键值从小到大排序插入*/
//BOOL TBLInsertRow(PTBL ptbl, void* prow, UI8 sec_key_size, UI8 sec_key_ind)
BOOL TBLInsertRow(PTBL ptbl, void* prow)
{
	UI16 row_len;
	UID	id;
	UI8* pnow;
	UI8* pend;
	
	if(ptbl == NULL || prow == NULL || ptbl->prow == NULL || ptbl->hd.row_len == 0)
		return FALSE;

	if(ptbl->hd.max_row <= ptbl->hd.row)
		return FALSE;

	row_len = ptbl->hd.row_len;
	id = *(UID*)prow;
	pnow = ptbl->prow;
	pend = pnow + row_len*ptbl->hd.row;
	while(pnow < pend && *(UID*)pnow <= id)
		pnow += row_len;

	/*内存重叠,不能用memcpy*/
	if(pnow < pend)
		memmove(pnow + row_len, pnow, pend -pnow);
	
	memcpy(pnow, prow, row_len);
	ptbl->hd.row++;

	return TRUE;
}

/*按键值从小到大排序*/
BOOL TBLSort(PTBL ptbl)
{
	return TRUE;
}

void TBLClear(PTBL ptbl)
{
	if(ptbl != NULL)
	{
		ptbl->hd.row = 0;
		if(ptbl->prow != NULL)
			memset(ptbl->prow, 0, ptbl->hd.row_len* ptbl->hd.max_row);
	}
}

void* TBLGetRow(PTBL ptbl, UID key, UI16* pnum)
{
	UI16 row_len;
	UI8* pnow;
	UI8* pend;
	void* ret;
	UI16 num;

	if(ptbl == NULL || ptbl->hd.row_len <= 0)
		return NULL;

	num = 0;
	row_len = ptbl->hd.row_len;
	pnow = ptbl->prow;
	pend = (UI8*)ptbl->prow + row_len*ptbl->hd.row;
	ret = NULL;
	
	/*找到key指定的行*/
	while(pnow < pend && *(UID*)pnow != key)
	{
		pnow += row_len;
	}

	if(pnow < pend)
	{
		/*查找有几行*/
		ret = pnow;
		while(pnow < pend && *(UID*)pnow == key)
		{
			num++;
			pnow += row_len;
		}

		if(pnum != NULL)
			*pnum = num;
	}
	
	return ret;
}

PTBL TBLGet(UI16 type, UI8 sn, UI8 ver)
{
	UI8	ind;
	
	if(type >= ARRAY_NUM(tbl_ind) || sn > ARRAY_NUM(tbl_ind[0].ind))
		return NULL;

	ind = tbl_ind[type].ind[sn];
	if(ind > 0)
		return tbl_lst[ind -1].ptbl;
	
	return NULL;
}

const TBL_TYPE_INFO* TBLGetInfo(UI16 type)
{
	if(type < T_TBL_END)
		return tbl_type_info + type;
	else
		return NULL;
}

PTBL TBLCopy(PTBL pdst, PTBL psrc)
{
	UI8* psrc_row;
	UI8* pdst_row;
	int i, num, row_len;
	
	if(pdst == NULL || psrc == NULL || pdst->hd.type != psrc->hd.type || pdst->prow == NULL || psrc->prow == NULL)
		return psrc;

	num = psrc->hd.row;
	if(num > pdst->hd.max_row)
		num = pdst->hd.max_row;

	pdst_row = (UI8*)(pdst->prow);
	psrc_row = (UI8*)(psrc->prow);
	memset(pdst->prow, 0, pdst->hd.row_len*pdst->hd.max_row);
	if(psrc->hd.row_len > pdst->hd.row_len)
		row_len = pdst->hd.row_len;
	else
		row_len = psrc->hd.row_len;
	
	for(i = 0; i < num; i++)
	{
		memcpy(pdst_row, psrc_row, row_len);
		pdst_row += pdst->hd.row_len;
		psrc_row += psrc->hd.row_len;
	}
	pdst->hd.row = psrc->hd.row;
	
	return pdst;
}

PTBL TBLMerge(PTBL pdst, PTBL psrc)
{
	return pdst;
}

PTBL_LST AddTblToInd(PTBL ptbl, UI8 stor_id, UI8 attr, UI16 new_sn)
{
	PTBL_IND pd;
	PTBL_LST plst;
	
	pd = tbl_ind;

	
	if(ptbl == NULL || ptbl->hd.type >= ARRAY_NUM(tbl_ind) || ptbl->hd.sn > MAX_TBL_SN)
		return NULL;

	if(new_sn == 0 || new_sn > MAX_TBL_SN)
		new_sn = ptbl->hd.sn;
	
	pd = tbl_ind + ptbl->hd.type;
	if(pd->ind[new_sn]== 0)
	{
		/*new table*/
		plst = tbl_lst;
		while(plst < tbl_lst + ARRAY_NUM(tbl_lst) && plst->ptbl != NULL)
			plst++;

		if(plst >= tbl_lst + ARRAY_NUM(tbl_lst))
			return NULL;				/*tbl lst full*/
		
		plst->attr = attr;
		plst->cfg_db_id = stor_id;
		plst->ptbl = ptbl;
		pd->ind[new_sn] = plst - tbl_lst + 1;
	}
	else
	{
		/*update table info*/
		plst = tbl_lst + pd->ind[new_sn] - 1;
		plst->attr = attr;
		plst->cfg_db_id = stor_id;
		plst->ptbl = ptbl;
	}

	return plst;
}

PTBL TBLAlloc(UI16 type, UI8 ver, UI8 sn, UI8 attr, UI16 max_row, UI16 row_len)
{
	PTBL ptbl;
	if(max_row <= 0 || row_len <= 0)
		return NULL;
	
	ptbl = TBLGet(type, sn, ver);
	if(ptbl != NULL)
		return ptbl;

	ptbl = tbl_dync;
	while(ptbl < tbl_dync + ARRAY_NUM(tbl_dync))
	{
		if(ptbl->prow == NULL)
			break;

		ptbl++;
	}

	if(ptbl > tbl_dync + ARRAY_NUM(tbl_dync))
		return NULL;

	if(attr&TBL_DYNC_SRAM)
	{
		if((ptbl->prow = AllocMemTbl(MEMTBL_DYNC_MEM_EXT, row_len, max_row)) == NULL)
		return NULL;
	}
	else
	{
		if((ptbl->prow = AllocMemTbl(MEMTBL_DYNC_TBL, row_len, max_row)) == NULL)
		return NULL;
	}	

	ptbl->hd.type = type;
	ptbl->hd.ver = ver;
	ptbl->hd.sn = sn;
	ptbl->hd.max_row = max_row;
	ptbl->hd.row_len = row_len;
	ptbl->hd.row = 0;

	if(AddTblToInd(ptbl, DYNC_MEM_AREA, attr | TBL_DYNC, 0) == NULL)
	{
		/*Error, do somthing here*/
	}
	
	return ptbl;
}

BOOL TBLReqTx()
{
	return TRUE;
}


#ifdef DEBUG
void TBLPrintf(UI16 type, UI8 sn, UI16 row, UI16 row_len, void*prow)
{
	int i;
	int j;

	UI16* pdat;

	db_printf("TBL type %d, SN %d, row: %d, row_len: %d\r\n", type, sn, row, row_len);
	for(i = 0; i < row; i++)
	{
		pdat = (UI16*)((char*)prow + i*row_len);
		for(j = 0; j < ((row_len + 1) >> 1); j++)
			db_printf("%04x ", pdat[j]);
		db_printf("\r\n");
	}
}
#endif

BOOL TBLRspRx(PPROT_TBL_PKT ppkt, int dat_len)
{
	PPROT_TBL_HD phd;
	const TBL_TYPE_INFO* pinfo;
	CFG_DB_ID db_id;
	PTBL ptbl;
	
	
	if(ppkt == NULL)
		return FALSE;

	/*检查是否是一个完整的表*/
	if(ppkt->start_row == 0)
	{
		phd = (PPROT_TBL_HD)(ppkt + 1);
		if(phd->type != ppkt->type ||phd->sn != ppkt->sn)
			return FALSE;

		if(phd->row_num > ppkt->row_num)
			return FALSE;

		if(dat_len < phd->row_num*phd->row_len + sizeof(PROT_TBL_HD) + sizeof(PROT_TBL_PKT))
			return FALSE;

		if((pinfo = TBLGetInfo(phd->type)) == NULL)
			return FALSE;
		
		if(pinfo->attr_bits & TBL_STOR)
		{
			if(pinfo->attr_bits & TBL_USER)
				db_id = USER_TBL_DB;
			else
				db_id = FACTORY_TBL_DB;

#if 0
			TBLPrintf(((PPROT_TBL_HD)(ppkt + 1))->type, ((PPROT_TBL_HD)(ppkt + 1))->sn, ((PPROT_TBL_HD)(ppkt + 1))->row_num, 
					 ((PPROT_TBL_HD)(ppkt + 1))->row_len, (PPROT_TBL_HD)(ppkt + 1) + 1);
#endif
			
			CfgDbTblSave(((PPROT_TBL_HD)(ppkt + 1))->type, ((PPROT_TBL_HD)(ppkt + 1))->sn, ((PPROT_TBL_HD)(ppkt + 1))->row_num, 
						  ((PPROT_TBL_HD)(ppkt + 1))->row_len, (PPROT_TBL_HD)(ppkt + 1) + 1, db_id);
			
			SetRunFlag(INIF_CFGDBCHG);
		}
		else if (pinfo->attr_bits & TBL_STOR_PANEL)
		{
			if (dat_len < phd->row_num*phd->row_len + sizeof(PROT_TBL_HD) + sizeof(PROT_TBL_PKT))
				return FALSE;
		
			ptbl = TBLGet(pinfo->type,0,pinfo->ver);
			if (ptbl != NULL)
			{
				MemcpyPro(ptbl->prow,(PPROT_TBL_HD)(ppkt + 1) + 1,pinfo->row_len*pinfo->max_row);

				if (pinfo->type == T_OPN_RAMP)
				{
					InitAiClampRampNum();
				}	
				
				db_printf("T_OPN_RAMP data:\r\n");
#ifdef DEBUG
				UI16 i,j;
				UI16* test;

				test = (UI16*)ptbl->prow;
				for (i = 0;i<pinfo->max_row;i++)
				{
					db_printf("Row %d: ",i+1);
					for (j = 0;j < pinfo->row_len/2;j++,test++)
					{						
						db_printf("%d, ", *test);
					}
					db_printf("\r\n");
				}
#endif
			}		
		}
	}

	return TRUE;
#if 0		
		{
			if(tbl_rx_st.brx && phd->type != tbl_rx_st.tbl_type && phd->sn != tbl_rx_st.tbl_sn)
				/*不是待接收表的数据，丢弃 */
				return FALSE;
		}
#endif
}


BOOL TBLReqRx(PPROT_TBL_PKT ppkt)
{
	UI16 tbl_type;
	UI16 row_len;
	UI16 len;
	UI16 req_sn;
	UI16 start_row;
	UI16 row_num;
	PTBL ptbl;
	UI8* prow;
	UI8* psrc;
	
	const TBL_TYPE_INFO* pinfo;
	
	if(ppkt == NULL)
		return FALSE;
	
	tbl_type = ppkt->type;

	if((ptbl = TBLGet(tbl_type, ppkt->sn, TBL_DB_VER)) == NULL)
		return FALSE;

	if((pinfo = TBLGetInfo(tbl_type)) == NULL)
		return FALSE;

	if(pinfo->ext_row_len == 0 || pinfo->ext_row_len > ptbl->hd.row_len)
		row_len = ptbl->hd.row_len;
	else
		row_len = pinfo->ext_row_len;

	if(row_len == 0)
		return FALSE;

	if(ppkt->start_row == 0)
	{
		start_row = 0;
		len = sizeof(PROT_TBL_HD);
	}
	else
	{
		len = 0;
		start_row = ppkt->start_row - 1;
	}
	
	if(ppkt->row_num == 0xffff)
		row_num = ptbl->hd.row;
	else if(start_row >= ptbl->hd.row)
		row_num = 0;
	else if(ppkt->row_num + start_row > ptbl->hd.row)
		row_num =  ptbl->hd.row - start_row;
	else
		row_num = ppkt->row_num;
	
	if(len + row_num*row_len + sizeof(PROT_TBL_PKT) > (PROT_MAX_DAT_LEN - PROT_PACK_LEN*2))
		row_num = ((PROT_MAX_DAT_LEN - PROT_PACK_LEN*2) - len - sizeof(PROT_TBL_PKT)) / row_len;
	
	req_sn = ppkt->sn;
	if((ppkt = (PPROT_TBL_PKT)ProtNewPkt(TBL_RSP_CMD, 0, FALSE, len + row_num*row_len + sizeof(PROT_TBL_PKT))) != NULL)
	{
		/*设置包头*/
		ppkt->type = ptbl->hd.type;
		ppkt->sn = req_sn;
		ppkt->row_num = row_num;
		ppkt->start_row = start_row;
		if(len != 0)
		{
			/*需要发送表头，设置表头*/
			((PPROT_TBL_HD)(ppkt + 1))->type = ptbl->hd.type;
			((PPROT_TBL_HD)(ppkt + 1))->ver = ptbl->hd.ver;
			((PPROT_TBL_HD)(ppkt + 1))->row_num = ptbl->hd.row;
			((PPROT_TBL_HD)(ppkt + 1))->sn = req_sn;
			((PPROT_TBL_HD)(ppkt + 1))->row_len = row_len;
			((PPROT_TBL_HD)(ppkt + 1))->rev[0] = 0;
			((PPROT_TBL_HD)(ppkt + 1))->rev[1] = 0;
			((PPROT_TBL_HD)(ppkt + 1))->rev[2] = 0;
			prow = (UI8*)((PPROT_TBL_HD)(ppkt + 1) + 1);
		}
		else
			prow = (UI8*)(ppkt + 1);

		/*拷贝行数据，如果行长度相同，一次拷贝，否则一行行拷贝*/
		psrc = (UI8*)(ptbl->prow) + ptbl->hd.row_len*start_row;
		if(row_len == ptbl->hd.row_len)
			memcpy(prow, psrc, row_num*row_len);
		else
		{
			while(row_num-- > 0)
			{
				
#ifdef SYS232
				MemcpyPro(prow, psrc, row_len);
#else
				memcpy(prow, psrc, row_len);
#endif
				prow += row_len;
				psrc += ptbl->hd.row_len;
			}
		}

		return MainProtTxPkt(ppkt);
	}

	return FALSE;
}

BOOL TBLInit()
{
	memset(tbl_lst, 0 , sizeof(tbl_lst));
	memset(tbl_ind, 0 , sizeof(tbl_ind));
	memset(tbl_dync, 0 , sizeof(tbl_dync));
	memset(&tbl_rx_st, 0, sizeof(tbl_rx_st));

	InitTBLInPanel();

	return TRUE;
}

