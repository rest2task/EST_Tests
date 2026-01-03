/******************************************************************************
  文 件 名   : actfork.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 动作任务监控表及扫描
  函数列表   :
              ActForkCmpCal
              ActForkTblClear
              ActForkTblInit
              ActForkTblInsert
              ActForkTblPrint
              ActNoCmpFork
              ActStopFork
              DefaultFork
              GetActForkList
              GetVarValue
              ScanActForkTbl
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
#include "actfork.h"
#include "actctrl.h"
#include "part.h"
#include "ioproc.h"
#include "admeter.h"
#include "machapi.h"
#include "machine.h"
#include "tbl.h"

PTBL  pfork_tbl = NULL;
static UI8 bcfg_fork_tbl_outdate = FALSE;

//__forceinline
BOOL GetVarValue(PEXP_VAR pvar, PACT psrc_act, PACT pdst_act, PACT_TSK_ST pst, int* val)
{
	UI16 tmp;
	UI32 type;
	UI32 ind;
	UID ref_id;
	
	type= (pvar->type) & 0x7f;
	ind = pvar->ind;
	ref_id = pvar->ref_id;
	
	if(type == VAR_CONST)
		tmp = ind;
	else if(type == VAR_IND)
	{
		if(ind < VAR_METER)
			tmp = *((UI16*)(&(pst->mov_st)) + ind);
		else if(ind == VAR_METER)
			tmp = (UI16)INT_ABS(GetMeterPosByCh(pst->meter_ch));
		else
			return FALSE;
	}
	else if(type >= VAR_SRC_SUB && type < VAR_OTH_SUB + PART_SUB_NUM)
	{
		UI8 sub_id;
		PPART pp;

		switch(type&0x70)
		{
			case VAR_SRC_SUB: pp = (PPART)psrc_act->part; break;
			case VAR_DST_SUB: pp = (PPART)pdst_act->part; break;
			case VAR_OTH_SUB: pp = GetPartByID(ref_id);break;
			default:
				return FALSE;
		}
		
		sub_id = (type&0x0f);
		if(pp != NULL && sub_id < PART_SUB_END && pp->sub_len[sub_id] > ind && pp->psub[sub_id] != NULL)
		{
			if(sub_id == PI_ID)
				tmp = CheckPI(pp->psub[sub_id][ind]);
			else if(sub_id == PO_ID)
				tmp = GetPOBit(pp->psub[sub_id][ind]);
			else
				tmp = pp->psub[sub_id][ind];
		}
		else
			return FALSE;
	}
	else
		return FALSE;

	/*有符号数*/
	if(pvar->type & 0x80)
		*val = (int)((I16)tmp);
	else
		*((UI32*)val) = tmp;
	
	return TRUE;
}

//__forceinline
BOOL ActForkCmpCal(PACT_FORK pfork, PACT psrc_act, PACT pdst_act, PACT_TSK ptsk)
{
	int var1;
	int var2;
	BOOL log_tmp;
	BOOL log_val;
	PFORK_CMP pcmp, pend;

	log_val = TRUE;
	pcmp = pfork->cmp;
	pend = pcmp + pfork->cmp_num;
	while(pcmp < pend)
	{
		if(pcmp->var1.type == 0 || !GetVarValue(&(pcmp->var1), psrc_act, pdst_act, ptsk->pst, &var1))
			break;

		if(pcmp->var2.type == 0 || !GetVarValue(&(pcmp->var2), psrc_act, pdst_act, ptsk->pst, &var2))
			break;

		switch(pcmp->exp_op)
		{
			case EQ_OP: log_tmp = (var1 == var2); break;
			case NO_EQ_OP: log_tmp = (var1 != var2); break;
			case GT_OP: log_tmp = (var1 > var2); break;
			case LS_OP: log_tmp = (var1 < var2); break;
			case GT_EQ_OP: log_tmp = (var1 >= var2); break;
			case LS_EQ_OP: log_tmp = (var1 <= var2); break;
			default:
				log_tmp = FALSE;
				break;
		}

		if(pcmp->lg_op == AND_LG)
			log_val = (log_val && log_tmp);
		else if(pcmp->lg_op == OR_LG)
			log_val = (log_val || log_tmp);
		else
			break;

		pcmp++;
	}

	if(pcmp < pend)
	{
		pfork->bexec = TRUE;
		return FALSE;
	}

	return log_val;
}

BOOL ScanActForkTbl(PACT_TSK ptsk)
{
	PACT_FORK pfork, pend;
	BOOL bmatched= FALSE;
	PACT psrc_act, pdst_act ;
	
	pfork =ptsk->pfork;
	if((psrc_act = ptsk->pact) == NULL)
		return FALSE;

	pend = pfork + ptsk->fork_num;
	while(pfork < pend)
	{
		if(pfork->src_act_sn != ptsk->pst->act_sn)
			break;
		else if(!pfork->bexec)
		{
			UI16 en_mov_st = pfork->en_mov_st;
			UI16 en_act_ret = pfork->en_act_ret;
			UI16 en_step = pfork->en_step;
			UI16 en_err = pfork->en_err;
			UI16 en_next_step = pfork->en_next_step;
			UI8	cmp_num = pfork->cmp_num;

			pdst_act = pfork->pdst_act;
			if( (en_mov_st != INVALD_MOV_ST && en_mov_st != psrc_act->mov_st) ||(en_act_ret != INVALD_ACT_RET && (en_act_ret != ptsk->pst->act_ret || (en_act_ret == ACT_END && ptsk->end_fork_disable))) ||
			  (en_step != INVALID_STEP && en_step != psrc_act->step)  ||(en_next_step != INVALIDE_EN &&  en_next_step != ptsk->pst->next_step) || 
			  (en_err != INVALIDE_EN && (en_err != (ptsk->pst->act_state & INTF_ERR_MSK)))  ||!ChkOperMode(pfork->en_oper_mode) ||
			  (cmp_num > 0 && !ActForkCmpCal(pfork, psrc_act, pdst_act, ptsk)))
			{
				pfork++;
				continue;
			}

			/*条件满足*/
			bmatched = TRUE;
			pfork->bexec = TRUE;

			if(en_act_ret != ACT_END || ptsk->pptsk == NULL || ptsk->pst->fork_mode != SUSPEND_FORK)
				ActTskCmd(ptsk, pdst_act, pfork->src_cmd, pfork->dst_cmd, pfork->dly_ms, pfork->para, FALSE, pfork->dst_act_sn);
		}

		pfork++;
	}

 	return bmatched;
}


BOOL ActForkTblInit()
{
	pfork_tbl = TBLGet(T_ACT_FORK, FORK_TBL_VER, 1);
	
	if(pfork_tbl == NULL)
		pfork_tbl = TBLAlloc(T_ACT_FORK, FORK_TBL_VER, 1, 0, FORK_TBL_SIZE, sizeof(ACT_FORK));
	
	return (pfork_tbl != NULL);
}

BOOL ActForkTblInsert(PACT_FORK pfork)
{
	BOOL ret;

	ret = TBLInsertRow((PTBL)pfork_tbl, pfork);
	//ActForkTblPrint();
	
	return ret;
}

void ActForkTblClear()
{
	TBLClear((PTBL)pfork_tbl);
}

PACT_FORK GetActForkList(UID act_sn, UI16* pnum)
{
	return (PACT_FORK)TBLGetRow((PTBL)pfork_tbl, act_sn, pnum);
}

void DefaultFork(PACT_FORK pfork)
{
	if(pfork != NULL)
	{
		memset(pfork, 0, sizeof(ACT_FORK)); 
		pfork->en_mov_st = INVALD_MOV_ST; 
		pfork->en_step = INVALID_STEP; 
		pfork->en_oper_mode = SKIP_OPER;
		pfork->en_err = INVALIDE_EN;
		pfork->en_next_step = INVALIDE_EN;
		pfork->dst_cmd = F_CMD_RUN;
	}
}

void ActNoCmpFork(UI16 src_act, UI16 dst_act, UI16 step, UI16 next_step, UI16 mov_st, UI16 err_bit, UI16 oper_mode, UI16 act_ret, UI16 dly_ms, UI32 para, BOOL bhold, UI16 act_sn)
{
	ACT_FORK  fork;
	
	DefaultFork(&fork);

	fork.src_act_sn = src_act;
	fork.dst_act_id = dst_act;
	fork.dly_ms = dly_ms;

	if(act_sn == 0)
		fork.dst_act_sn = dst_act;
	else
		fork.dst_act_sn = act_sn;
	
	if(bhold)
		fork.src_cmd = F_CMD_HOLD;

	fork.en_oper_mode = oper_mode;
	fork.en_mov_st = mov_st;
	fork.en_act_ret = act_ret;
	fork.en_step = step;
	fork.en_next_step = next_step;
	fork.en_err = err_bit;
	fork.para = para;
	ActForkTblInsert(&fork);
}

void ActStopFork(UI16 src_act, UI16 dst_act, UI16 mov_st, UI16 oper_mode)
{
	ACT_FORK  fork;
	
	DefaultFork(&fork);

	fork.src_act_sn = src_act;
	fork.dst_act_id = dst_act;
	fork.en_mov_st = mov_st;
	fork.dst_cmd = F_CMD_STOP;
	ActForkTblInsert(&fork);
}

void ActAbortFork(UI16 src_act, UI16 dst_act, UI16 mov_st, UI16 oper_mode)
{
	ACT_FORK  fork;
	
	DefaultFork(&fork);

	fork.src_act_sn = src_act;
	fork.dst_act_id = dst_act;
	fork.en_mov_st = mov_st;
	fork.dst_cmd = F_CMD_ABORT;
	ActForkTblInsert(&fork);
}

void ActForkTblPrint()
{
	int i;
	PACT_FORK pfork;

	if (pfork_tbl == NULL)
	{
		db_printf("Act fork tbl Err\r\n");
		return;
	}
	
	db_printf("Act fork tbl num: %d\r\n",  pfork_tbl->hd.row);
	pfork = (PACT_FORK)pfork_tbl->prow;
	for(i = 0; i < pfork_tbl->hd.row; i++)
	{
		db_printf("SN: %d, s:%04x, d:%04x, r:%04x, o:%04x, t:%04x, e:%04x\r\n", i, pfork->src_act_sn, pfork->dst_act_id, pfork->en_act_ret, pfork->en_oper_mode, pfork->en_step,
				 pfork->en_err);
		pfork++;
	}
}

void ActForkTblOutDate(BOOL bset)
{
	/*接收到面板发送的部件MD或者MH数据时，需要重新生成动作列表*/
	if(bset)
		bcfg_fork_tbl_outdate = TRUE;
	else
		bcfg_fork_tbl_outdate = FALSE;
		
}

void ActForkTblRefresh(BOOL bforce)
{
	/*如果有动作动作任务引用动作列表，则不刷新，CreateActForkTbl后，bcfg_fork_tbl_outdate会被置为FALSE*/
	if((ChkOperMode(OM_MANUAL) || bforce) && !ActForkTblIsUsing() && bcfg_fork_tbl_outdate == TRUE)
	{
		CreateActForkTbl(GetOperMode());
	}
}

