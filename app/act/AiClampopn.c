/******************************************************************************
  文 件 名   : CalibrationAggControl.c
  版 本 号   : 初稿
  作    者   : zhou tuo
  生成日期   : 2019年8月6日
  最近修改   :
  功能描述   : 液压系统各参数校准动作
  函数列表   :


******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "acttype.h"
#include "typelst_dic.h"
#include "AiClampopn.h"
#include "stdlib.h"
#include "memtbl.h"
#include "string.h"
#include "Machapi.h"

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
void InitClampRampPro(void);

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
PAICLAMPDATA_ROW pg_aiclpopn_data;

UI16	FuncAiClampOpn = 0;
UI16	pos_diff = 200;
/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

BOOL AiTBLReqRx()
{
	UI16 row_len;
	PPROT_TBL_PKT ppkt;
	const TBL_TYPE_INFO* pinfo;

	if (FuncAiClampOpn == 0)
		return FALSE;
	
	if((pinfo = TBLGetInfo(T_OPN_RAMP)) == NULL)
		return FALSE;

	row_len = pinfo->ext_row_len;

	if(row_len == 0)
		return FALSE;
		
	if((ppkt = (PPROT_TBL_PKT)ProtNewPkt(TBL_REQ_CMD, 0, FALSE, sizeof(PROT_TBL_PKT))) != NULL)
	{
		/*设置包头*/
		ppkt->type = T_OPN_RAMP;
		ppkt->sn = 1;
		ppkt->row_num = 20;
		ppkt->start_row = 0;

		return MainProtTxPkt(ppkt);
	}

	return FALSE;
}


BOOL AiTBLRspRx(UI16 start_row, UI16 row_num)
{
	UI16 tbl_type;
	UI16 row_len;
	UI8* prow;
	UI8* psrc;
	PPROT_TBL_PKT ppkt;
	UI16 i,j;
	UI16* test;
	
	const TBL_TYPE_INFO* pinfo;
	PTBL p_tbl;	

	if (FuncAiClampOpn == 0)
		return FALSE;
	
	tbl_type = T_OPN_RAMP;	

	if ((pinfo = TBLGetInfo(T_OPN_RAMP)) == NULL)
		return FALSE;

	if (pinfo->max_row < (start_row+row_num))
		return FALSE;

	if ((p_tbl = TBLGet(T_OPN_RAMP, 0, TBL_DB_VER)) == NULL)
		return FALSE;

	if (p_tbl->prow == NULL)
		return FALSE;

	row_len = pinfo->ext_row_len;

	if (row_len == 0)
		return FALSE;
		
	if ((ppkt = (PPROT_TBL_PKT)ProtNewPkt(TBL_RSP_CMD, 0, FALSE, row_len*row_num + sizeof(PROT_TBL_PKT))) != NULL)
	{
		/*设置包头*/
		ppkt->type = tbl_type;
		ppkt->sn = 1;
		ppkt->row_num = row_num;
		ppkt->start_row = start_row;

		prow = (UI8*)(ppkt + 1);

		/*拷贝行数据，如果行长度相同，一次拷贝，否则一行行拷贝*/
		psrc = (UI8*)((UI8*)p_tbl->prow + start_row*pinfo->row_len);
		
		memcpy(prow, psrc, row_len*row_num);

		db_printf("TBL_RSP_CMD recv, type = %04x, sn = %d, start= %d, num = %d\r\n", ppkt->type,ppkt->sn,ppkt->start_row, ppkt->row_num);		
		test = (UI16*)psrc;
		for (i = 0;i<row_num;i++)
		{
			db_printf("Row %d: ",start_row+1);
			for (j = 0;j < pinfo->row_len/2;j++,test++)
			{						
				db_printf("%d, ", *test);
			}
			db_printf("\r\n");
		}
		return MainProtTxPkt(ppkt);
	}

	return FALSE;
}

/*******************************************************************************************
 *Brief			:下位机主动清楚AI表数据
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20200724
********************************************************************************************/
void ClearAiClampTB(void)
{
	PTBL ptb;

	if (FuncAiClampOpn == 0)
		return;
	
	ptb = TBLGet(T_OPN_RAMP, 0, TBL_DB_VER);
	memset(ptb->prow, 0, ptb->hd.max_row*ptb->hd.row_len);

	AiTBLRspRx(0,ptb->hd.max_row);
}

/*******************************************************************************************
 *Brief			:对Ai开模表进行初始化，分配外部内存给该表使用
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20200717
********************************************************************************************/
void InitAiClamp()
{
	int i = 0;
	PAICLAMPDATA_ROW p_ai;
	PAICLAMPDATA_TBROW p_tbrow;
	PTBL ptb;

	if (FuncAiClampOpn == 0)
		return;

	if (ChkSysCfg(XF1_EJTNH))
	{
		pos_diff = 400;
	}
	else
	{
		pos_diff = 200;
	}
	
	ptb = TBLGet(T_OPN_RAMP, 0, TBL_DB_VER);
	p_tbrow = (PAICLAMPDATA_TBROW)(ptb->prow);

	pg_aiclpopn_data = AllocMemTbl(MEMTBL_DYNC_MEM_EXT,sizeof(AICLAMPDATA_ROW),AICLP_ROW_NUM);
	p_ai = pg_aiclpopn_data;
	
	for (i = 0;i < AICLP_ROW_NUM;i++,p_ai++,p_tbrow++)
	{
		p_ai->p_rampArray = p_tbrow;
		p_ai->pos = 1200 + i*POS_DIFF;
		p_ai->ramp_num = 0;
	}
}

void InitAiClampRampNum(void)
{
	UI16 i = 0,j = 0;
	PAICLAMPDATA_ROW p_ai;
	p_ai = pg_aiclpopn_data;
	
	if (FuncAiClampOpn == 0)
		return;
	
	for (i = 0;i < AICLP_ROW_NUM;i++,p_ai++)
	{
		p_ai->ramp_num = 0;
		for (j = 0;j < AICLP_ROWRAMP_NUM;j++)
		{
			if (p_ai->p_rampArray->ramp[j] > 0)
			{
				p_ai->ramp_num++;
			}			
		}
	}

	InitClampRampPro();
}


/*******************************************************************************************
 *Brief			:对位置及速度的ramp数据保存
 *Param			:pos:位置 单位0.1mm	spd:速度 单位0.1%	ramp:斜率 1%
 *Return Value	:TRUE:插入成功
 *Aauthor		:ZT
 *Date			:20200717
********************************************************************************************/
UI16 AiClampRampDataInput(UI16 pos,UI16 spd,UI16 ramp)
{	
	PAICLAMPDATA_ROW p_ai = pg_aiclpopn_data;
	
	UI16 i = 0,j = 0;

	if (FuncAiClampOpn == 0)
		return FALSE;
	
	if (p_ai == NULL)
	{
		return FALSE;
	}

	for (i = 0;i < AICLP_ROW_NUM;i++,p_ai++)
	{
		if (((p_ai->pos - POS_DIFF/2) < pos && (p_ai->pos + POS_DIFF/2) >= pos)
			||((i==0)&&((p_ai->pos - POS_DIFF/2) >= pos))
			||((i==(AICLP_ROW_NUM-1))&&((p_ai->pos + POS_DIFF/2) <= pos)))
		{
			for (j = 0;j < AICLP_ROWRAMP_NUM;j++)
			{
				if (spd > j*100 && spd <= (j+1)*100)
				{
					if (p_ai->p_rampArray->ramp[j] == 0)
					{
						p_ai->ramp_num++;//如果该数据原来是空的，新增数据时对斜率个数值并行增加。
					}

					if (p_ai->p_rampArray->ramp[j] != ramp)	//当前斜率设定与原来的不同，则更新数据并上传面板。
					{
						p_ai->p_rampArray->ramp[j] = ramp;
						//Updata 数据上传面板
						AiTBLRspRx(i, 1);
					}
					return TRUE;
				}
			}			
		}
	}

	return FALSE;
}

UI16 AiClampRampDataGet(PAICLAMPDATA_ROW p_ai,UI16 spd)
{
	UI16 j = 0;
	UI16 ramp1 = 0,ramp2 = 0,ramp0 = 0;
	UI16 ramp_cn1 = 0,ramp_cn2 = 0;
	UI16 dir = 0;
	UI16 ramp_cn;

	if (FuncAiClampOpn == 0)
		return 0;
	
	if (p_ai == NULL || spd == 0 || spd > 1000)
	{
		return 0;
	}

	//ramp_cn = (spd-1)/100;
	
	for (j = 0;j < AICLP_ROWRAMP_NUM;j++)
	{
		if (p_ai->p_rampArray->ramp[j] > 0 && dir == 0)
		{
			ramp1 = p_ai->p_rampArray->ramp[j];
			ramp_cn1 = j;
		}

		if (p_ai->p_rampArray->ramp[j] > 0 && dir == 1)
		{
			ramp2 = p_ai->p_rampArray->ramp[j];
			ramp_cn2 = j;
			break;
		}

		if (spd > j*100 && spd <= (j+1)*100)
//		if (ramp_cn == j)
		{
				ramp_cn = j;
			if (p_ai->p_rampArray->ramp[j] != 0)	//对应斜率区间有数值时，直接使用
			{
				ramp0 = p_ai->p_rampArray->ramp[j];
			}
			
			dir = 1;
		}
	}

	if (ramp0 != 0)	//查到对应位置区间的行数据
	{
		return ramp0;
	}
	else if (ramp1 == 0 && ramp2 == 0)	//无任何存储数据 返回0
	{
		return 0; 
	}
	else if (ramp1 != 0 && ramp2 != 0)	
	{
		return (((ramp_cn-ramp_cn1)*(ramp2-ramp1)/(ramp_cn2-ramp_cn1))+ramp1);
	}
	else
	{
		if (ramp1 == 0)
		{
			return ramp2;
		}
		else
		{
			return ramp1;
		}
	}
}

UI16 AiClampRowDataGet(UI16 pos,UI16 spd)
{
	PAICLAMPDATA_ROW p_ai = pg_aiclpopn_data;
	PAICLAMPDATA_ROW p_ai1 = NULL,p_ai2 = NULL,p_ai0 = NULL;
	UI16 dir = 0;
	UI16 i = 0;
	UI16 ramp1 = 0,ramp2 = 0;
	UI16 ramp_pos = 0;

	if (FuncAiClampOpn == 0)
		return 0;

	if (p_ai == NULL)
	{
		return 0;
	}

	for (i = 0;i < AICLP_ROW_NUM;i++,p_ai++)
	{
		if (p_ai->ramp_num > 0 && dir == 0)
		{
			p_ai1 = p_ai;

			if (p_ai->pos >= pos)
			{
				break;
			}
		}

		if (p_ai->ramp_num > 0 && dir == 1)
		{
			p_ai2 = p_ai;
			break;
		}
		
		if ((p_ai->pos - POS_DIFF/2) < pos && (p_ai->pos + POS_DIFF/2) >= pos)
		{
			ramp_pos = p_ai->pos;
			if (p_ai->ramp_num > 0)
			{
				p_ai0 = p_ai;
			}

			dir = 1;
		}
	}

	if (p_ai0 != NULL)	//查到对应位置区间的行数据
	{
		return AiClampRampDataGet(p_ai0, spd);
	}
	else if (p_ai1 == NULL && p_ai2 == NULL)	//无任何存储数据 返回0
	{
		return 0; 
	}
	else if (p_ai1 != NULL && p_ai2 != NULL)	
	{
		ramp1 = AiClampRampDataGet(p_ai1, spd);
		ramp2 = AiClampRampDataGet(p_ai2, spd);

		return (((ramp_pos-p_ai1->pos)*(ramp2-ramp1)/(p_ai2->pos-p_ai1->pos))+ramp1);
	}
	else
	{
		if (p_ai1 == NULL)
		{
			return AiClampRampDataGet(p_ai2, spd);
		}
		else
		{
			return AiClampRampDataGet(p_ai1, spd);
		}
	}
}



