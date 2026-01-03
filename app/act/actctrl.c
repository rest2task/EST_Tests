/******************************************************************************
  文 件 名   : actctrl.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月16日
  最近修改   :
  功能描述   : 动作任务控制：启动、运行、挂起、结束及任务监控和分支动作
  函数列表   :
              ActCtrInit
              ActSubQueFullErr
              ActTskCmd
              ActTskCmdErr
              ActTskFullErr
              ActTskIsEmpty
              ActTskRun
              AllocSubQue
              ClearAllActTask 
              ClearSubCmdQue
              EndActTsk
              ForkAct
              ForkActTsk
              HoldActTsk
              PLCEndActTsk
              PLCHoldActTsk
              PLCHoldAndRunActTsk
              PLCReActiveActTsk
              PLCRunActTsk
              ReActiveActTsk
              SentTskState
              SubCmdDeQue
              SubCmdInQue
  修改历史   :
  1.日    期   : 2013年5月16日
    作    者   : ren chaohong
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "com.h"
#include "actctrl.h"
#include "memtbl.h"
#include "act.h"
#include "basectrl.h"
#include "machapi.h"
#include "protcmd.h"
#include "monmmi.h"
#include "printf.h"

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
void PoFlag_Clear_hh(PACT_TSK ptsk,PE_PO_ROW prow,int row_num);

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/
ACT_TSK			act_tsk[MAX_TSK_NUM];
SUB_QUE			sub_que[SUB_CMD_QUE_SIZE];

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define TSK_TO_IND(tsk)		(((tsk) >= act_tsk && (tsk) < act_tsk +  MAX_TSK_NUM) ? (tsk) - act_tsk  + 1: 0)
#define IND_TO_TSK(ind)		(((ind) > 0 && ind <= MAX_TSK_NUM) ? act_tsk + (ind) - 1 : NULL)

UI16 Get_M_Step_No(int step)
{
	//step = pact->step - step;
	if(step >= 0)
	{
		return (UI16)((step>>8) + 1);
	}
	else
	{
		return 0;
	}
}


void ActTskFullErr()
{
	/*do somthing here，go to manual mode*/
	ManualInitErr();
	SetMachineErr(ER0_TSKFORK);
	db_printf("TSK full\r\n");
}


void ActTskCmdErr()
{
	/*do somthing here, goto manual mode*/
	//ManualInitErr();
	//SetMachineErr(ER0_TSKCMD);
	db_printf("TSK cmd errorl\r\n");
}

void ActSubQueFullErr()
{
	/*do somthing here, goto manual mode*/
	ManualInitErr();
	SetMachineErr(ER0_TSKFUL);
	db_printf("No free sub quel\r\n");
}

PSUB_QUE AllocSubQue()
{
	int i;

	for(i = 0; i < ARRAY_NUM(sub_que); i++)
	{
		if(!sub_que[i].bused)
			return sub_que + i;
	}

	/*sub que full*/
	ActSubQueFullErr();
	return NULL;
}

void ClearSubCmdQue(PACT_TSK ptsk)
{
	PSUB_QUE pq, pt;
	int i;
	
	if(ptsk != NULL)
	{
		pq = ptsk->psub;
		i = 0;
		while(pq != NULL && i < ARRAY_NUM(sub_que))
		{
			pq->bused = FALSE;
			pt = pq->pnext;
			pq->pnext = NULL;
			pq = pt;
			i++;
		}

		if(i >= ARRAY_NUM(sub_que))
		{
			/*sub cmd que error, reset sub_que*/	
			memset(sub_que, 0, sizeof(sub_que));
		}

		ptsk->psub = NULL;
	}
}

/*****************************************************************************
 函 数 名  : SubCmdInQue
 功能描述  : 把监控命令入到任务命令队列中
 输入参数  : PACT_TSK ptsk : 当前任务 
             PACT pdst_act :监控命令要启动的新动作  
             UI8 src_cmd:对当前任务执行的命令    
             UI8 dst_cmd:对新动作执行的命令     
             UI16 dly_ms:如启动新动作任务，为新任务延迟时间    
             UI32 para:传递给新动作任务的参数     
             BOOL bplc:命令是否由PLC发起     
             UI16 act_sn:新动作动作序号，一般为动作ID    
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年5月16日
    作    者   : ren chaohong
    修改内容   : 新生成函数

*****************************************************************************/
BOOL SubCmdInQue(PACT_TSK ptsk, PACT pdst_act, UI8 src_cmd, UI8 dst_cmd, UI16 dly_ms, UI32 para, BOOL bplc, UI16 act_sn)
{
	PSUB_QUE pt;
	PSUB_QUE psub;
	
	if(ptsk == NULL)
		return FALSE;

	if((psub = AllocSubQue()) == NULL)
		return FALSE;

	psub->bplc = bplc;
	psub->src_cmd = src_cmd;
	psub->dst_cmd = dst_cmd;
	psub->dly_ms = dly_ms;
	psub->pdst_act = pdst_act;
	psub->para = para;
	psub->act_sn = act_sn;
	
	if(ptsk->psub == NULL)
	{
		ptsk->psub = psub;
		psub->pnext = NULL;
	}
	else
	{
		pt = ptsk->psub;
		while(pt->pnext != NULL)
			pt = pt->pnext;

		psub->pnext = pt->pnext;
		pt->pnext= psub;
	}

	db_printf("sub cmd enter queue, act %04x, sub = %04x\r\n",  ptsk->pact->id, pdst_act != NULL ? pdst_act->id : 0);
	psub->bused = TRUE;

	return TRUE;
}

PSUB_QUE SubCmdDeQue(PACT_TSK ptsk)
{
	PSUB_QUE pq;
	
	if(ptsk == NULL)
		return FALSE;

	pq = ptsk->psub;
	if(pq == NULL)
		return NULL;
	
	ptsk->psub = pq->pnext;

	
	pq->pnext = NULL;
	pq->bused = FALSE;
	return pq;
}

void ActTskRun(PACT_TSK ptsk)
{
	PACT_TSK_ST pst;
	
	if(ptsk == NULL || ptsk->pact == NULL)
		return;

	pst = ptsk->pst;
	pst->tsk_st = TSK_DLY;
	pst->fork_100us = Get100UsTick();
	ActRunStart(ptsk->pact, ptsk);
}


BOOL HoldActTsk(PACT_TSK ptsk)
{
	PACT_TSK_ST	pst;
	
	if(ptsk == NULL)
		return FALSE;
		
	pst = ptsk->pst;
	if(pst->tsk_st == TSK_RUN || pst->tsk_st == TSK_DLY )
	{
		ptsk->hold_save_tsk_st = pst->tsk_st;
		pst->tsk_st = TSK_SUSPEND;
		if(!ptsk->skip_hold_stop)
			ActRunHold(ptsk->pact);
	}

	return TRUE;
}

/*****************************************************************************
 函 数 名  : ReActiveActTsk
 功能描述  : 挂起任务重新激活，如任务队列有监控命令，执行监控命令，直到命令队列
 			为空或再次被挂起。命令队列空时，任务被激活运行。
 			与ActTskCmd存在嵌套调用的可能，嵌套深度不会超过SUB_CMD_QUE_SIZE。
 输入参数  : PACT_TSK ptsk  : 要激活的任务。
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年5月16日
    作    者   : ren chaohong
    修改内容   : 新生成函数

*****************************************************************************/
BOOL ReActiveActTsk(PACT_TSK ptsk)
{
	PACT_TSK_ST pst;
	PSUB_QUE pque;
	
	if(ptsk == NULL || ptsk->pst->tsk_st != TSK_SUSPEND)
		return FALSE;

	pst = ptsk->pst;
	
	/*设置运行标志，以便执行命令队列中命令，直到队列为空或者任务再次被挂起*/
	pst->tsk_st = TSK_RUN;
	ptsk->skip_hold_stop = TRUE;
	while(pst->tsk_st != TSK_SUSPEND && (pque = SubCmdDeQue(ptsk)) != NULL)
		ActTskCmd(ptsk, pque->pdst_act, pque->src_cmd, pque->dst_cmd, pque->dly_ms, pque->para, pque->bplc, pque->act_sn);
	
	ptsk->skip_hold_stop = FALSE;
	if(pst->tsk_st != TSK_SUSPEND)
	{
		pst->tsk_st = ptsk->hold_save_tsk_st;
		if(pst->tsk_st == TSK_RUN)
			ActRunReactive(ptsk->pact, (pst->act_ret == ACT_END || pst->act_ret == ACT_ERR_ABORT));
	}

	return TRUE;
}

/*****************************************************************************
 函 数 名  : EndActTsk
 功能描述  : 结束动作任务。
 输入参数  : PACT_TSK ptsk : 要结束的任务
             BOOL babort : 是否异常强制结束，= FALSE，时，如父任务为挂起状态，则重新
             激活父任务。   
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年5月16日
    作    者   : ren chaohong
    修改内容   : 新生成函数

*****************************************************************************/
BOOL EndActTsk(PACT_TSK ptsk, BOOL babort)
{
	BOOL ret = TRUE;
	
	if(ptsk == NULL)
		return FALSE;

	if(ptsk->pst->fork_mode == SUSPEND_FORK)
	{
		if(ptsk->pptsk != NULL && ptsk->pptsk->pst->tsk_st == TSK_SUSPEND)
		{		
			if (ptsk->pst->act_ret != ACT_ERR_ABORT && ptsk->pst->act_ret != ACT_INTF_OFF)
				ReActiveActTsk(ptsk->pptsk);

			ptsk->pptsk = NULL;
		}
	}

//	if(ptsk->pst->act_ret != ACT_ERR_ABORT || babort)
	{
		/*clear all sub que cmd*/
		if(ptsk->psub != NULL)
		{
			ClearSubCmdQue(ptsk);
			if(ptsk->pst->act_ret != ACT_ERR_ABORT)
				ret = FALSE;
		}
		
		ptsk->psub = NULL;
	}
	
    if (babort || ptsk->pst->act_ret == ACT_ERR_ABORT || ptsk->pst->act_ret == ACT_INTF_OFF)
	{
		ActRunStop(ptsk->pact);
	}
	
	if(ptsk->psub == NULL)
	{
		ActRunEnd(ptsk->pact);
		ptsk->pact = NULL;
		ptsk->psub = NULL;
		ptsk->pfork = NULL;
		ptsk->pptsk = NULL;
		ptsk->fork_num = 0;
		ptsk->bplc = FALSE;
		//ptsk->pstep_hydr = NULL;
		memset(ptsk->pst, 0, sizeof(*ptsk->pst));
		ptsk->pst->tsk_st = TSK_IDLE;

	}
	else
	{
		HoldActTsk(ptsk);
	}

	return ret;
}

/*****************************************************************************
 函 数 名  : ForkActTsk
 功能描述  : 启动一个新动作任务，如父任务处在挂起状态，命令进入父任务命令队列，在
 			父任务激活时再执行。
 			获取动作的监控表项、电气图表项、油路优先级表项等。
 输入参数  : PACT pact:要启动新动作          
             FORK_MODE mode:启动方式，FIRST_FORK: 第一个任务,  SUSPEND_FORK: 父任务
             挂起，SYNC_FORK: 新任务和父任务同步运行。      
             PACT_TSK pparent_tsk:启动新任务的任务，即新任务的父任务。如无，为NULL   
             UI32 para : 传递给新任务的参数。             
             UI16 dly_ms : 新任务的运行延迟          
             BOOL bplc : 是否由PLC程序启动             
             UI16 act_sn: 给任务的动作序号，一般为动作ID           
 输出参数  : 无
 返 回 值 : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年5月16日
    作    者   : ren chaohong
    修改内容   : 新生成函数

*****************************************************************************/
BOOL ForkActTsk(PACT pact, FORK_MODE mode, PACT_TSK pparent_tsk, UI32 para, UI16 dly_ms, BOOL bplc, UI16 act_sn)
{
	int	i;
	PACT_TSK ptsk;
	PACT_TSK_ST pst;
	PACT_FORK	pfork;
	static UI8	time_sharing_db = 0;
	const UI16	time_sharing_actid[] = {0x2301,0x2302,0x3001};
	
	
	if(pact == NULL)
		return FALSE;

	if(pact->ptsk != NULL)
	{
		db_printf("Error act %04xalready run\r\n", pact->id);
		return FALSE;
	}

	if(pparent_tsk != NULL && pparent_tsk->pst->tsk_st == TSK_SUSPEND && mode != FIRST_FORK)
		return SubCmdInQue(pparent_tsk, pact, mode == SUSPEND_FORK ? F_CMD_HOLD : F_CMD_NULL, F_CMD_RUN, dly_ms, para, bplc, act_sn);
	
	if(pparent_tsk == NULL)
		mode = FIRST_FORK;

	for(i = 0; i < MAX_TSK_NUM; i++)
	{
		if(act_tsk[i].pst->tsk_st == TSK_IDLE)
			break;
	}

	if(i >= MAX_TSK_NUM)
	{
		ActTskFullErr();
		return FALSE;
	};

	ptsk = act_tsk+ i;
	pst = ptsk->pst;
	pst->act_id = pact->id;
	pst->mov_st = ACT_MOV_INIT;
	pst->step = INIT1_STEP;
	pst->act_ret = ACT_OK;
	pst->para = para;
	pst->fork_mode = mode;
	pst->dly_100us = dly_ms*10;
	pst->meter_ch = CallPartCmd(pact->part, MAKE_CMD(0, CMD_METERCH), 0);
//	pst->step_start_ms = pst->act_start_ms;

	pst->mov_start_ms = 0;

	pst->plc_part = GET_PART_TYPE(pact->id);
	pst->plc_act = GET_PART_ACT_SN(pact->id);

	if(act_sn == 0)
		pst->act_sn = pst->act_id;
	else
		pst->act_sn = act_sn;

	//=====分周期任务调度功能===============
	for (i = 0;i<CAL_LEN(time_sharing_actid);i++)
	{
		if ((time_sharing_actid[i]| pst->act_id) == pst->act_id)
		{
			pst->act_run_cn_start = time_sharing_db;
			time_sharing_db++;

			if(time_sharing_db == 100)
			{
				time_sharing_db = 0;
			}
			
			pst->act_run_cn = 8;//如果该动作存在分时表内，使用8周期控制
			break;
		}

		if (i == (CAL_LEN(time_sharing_actid) - 1))
		{
			pst->act_run_cn = 1;//默认使用单周期控制
		}
	}
	//======================================
		
	if(pparent_tsk != NULL && pparent_tsk->pst->act_ret == ACT_ERR_ABORT)
		ptsk->end_fork_disable = TRUE;
	else
		ptsk->end_fork_disable = FALSE;

	ptsk->skip_hold_stop = FALSE;
	ptsk->pact = pact;
	ptsk->pptsk = pparent_tsk;
	ptsk->psub = NULL;
	ptsk->pfork = GetActForkList(pst->act_sn, &ptsk->fork_num);
	/*clear bexec byte*/
	pfork = ptsk->pfork;
	while(pfork != NULL && pfork->src_act_sn == pst->act_sn)
	{
		if(pfork->pdst_act == 0)
			pfork->pdst_act = GetActByID(pfork->dst_act_id);
		(pfork++)->bexec = FALSE;
	}
	
	ptsk->bplc = bplc;
	ptsk->bstop_cmd_inque = FALSE;
	memset(&(ptsk->last_e_po), 0, sizeof(ptsk->last_e_po));

	ptsk->pepo = ExPOFilt(pact->id, &ptsk->epo_num);
	PoFlag_Clear_hh(ptsk,ptsk->pepo,ptsk->epo_num);
	ptsk->last_e_po.num = 0;

/*	
	ptsk->pepo = ExPOFilt(pact->gid, &ptsk->epo_num);
	if(ptsk->pepo == NULL)
	{
		//没有本动作电气表项，使用主动作
		ptsk->pepo = ExPOFilt(pact->gid, &ptsk->epo_num);
	}
*/
	ptsk->phydr = HydrPriFilt(pact->id, &ptsk->hydr_num);
	if(ptsk->phydr == NULL)
	{
		/*没有本动作油路优先级表项，使用主动作*/
		ptsk->phydr = HydrPriFilt(pact->gid, &ptsk->hydr_num);
	}
	
	ptsk->act_set_start_ms = 0;

	
	if(mode == SUSPEND_FORK)
	{
		HoldActTsk(pparent_tsk);
	}

	ActTskRun(ptsk);
	
	MovePartDataChangeCacheWithActStart(pact->id);//在每各动作开始时，检测数据缓冲区内是否有数据需要搬运
	return TRUE;
}

BOOL ForkAct(PACT pact, FORK_MODE mode, PACT_TSK pparent_tsk, UI32 para, UI16 dly_ms)
{
	return ForkActTsk(pact, mode, pparent_tsk, para, dly_ms, FALSE, 0);
}

void ClearAllActTask()
{
	PACT_TSK	ptsk;
	
	ptsk = act_tsk;
	while(ptsk < act_tsk + MAX_TSK_NUM)
	{
		//停止未被标记不可清除的动作
		if (!ptsk->pact->cannotbeclear)
		{
			if(ptsk->pst->tsk_st == TSK_RUN && ptsk->pact != NULL)
				ActRunStop(ptsk->pact);

			ptsk->pst->act_ret = ACT_ERR_ABORT;

			if(ptsk->psub != NULL);
				ClearSubCmdQue(ptsk);

			ptsk->psub = NULL;	
		}		
		ptsk++;
	}
	memset(sub_que, 0, sizeof(sub_que));

#ifdef PLC_ENABLE
	AnalysisIL(0);
#endif
 }

void ClearAllActTaskEnf()
{
	PACT_TSK	ptsk;
	
	ptsk = act_tsk;
	while(ptsk < act_tsk + MAX_TSK_NUM)
	{
		//停止未被标记不可清除的动作
		if (ptsk->pact->id != 0x0412)
		{
			if(ptsk->pst->tsk_st == TSK_RUN && ptsk->pact != NULL)
				ActRunStop(ptsk->pact);

			ptsk->pst->act_ret = ACT_ERR_ABORT;

			if(ptsk->psub != NULL);
				ClearSubCmdQue(ptsk);

			ptsk->psub = NULL;	
		}
		ptsk++;
	}
	memset(sub_que, 0, sizeof(sub_que));

#ifdef PLC_ENABLE
	AnalysisIL(0);
#endif
 }


/*****************************************************************************
 函 数 名  : ActTskCmd
 功能描述  : 执行监控命令
 输入参数  : PACT_TSK ptsk    
             PACT pdst_act    
             UI8 src_cmd      
             UI8 dst_cmd      
             UI16 dly_ms      
             UI32 para        
             BOOL bplc        
             UI16 dst_act_sn  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年5月16日
    作    者   : ren chaohong
    修改内容   : 新生成函数

*****************************************************************************/
BOOL ActTskCmd(PACT_TSK ptsk, PACT pdst_act, UI8 src_cmd, UI8 dst_cmd, UI16 dly_ms, UI32 para, BOOL bplc, UI16 dst_act_sn)
{
	BOOL ret = TRUE;
	
	if(ptsk == NULL && src_cmd != F_CMD_NULL)
		return FALSE;

	if(pdst_act == NULL && dst_cmd != F_CMD_NULL)
		return FALSE;

	if(src_cmd == F_CMD_NULL && dst_cmd == F_CMD_NULL)
		return FALSE;
	
	/*检查父任务是否已经挂起*/
	if(ptsk != NULL && ptsk->pst->tsk_st == TSK_SUSPEND)
	{
		if(ptsk->bstop_cmd_inque && (src_cmd != F_CMD_STOP || dst_cmd != F_CMD_NULL ||src_cmd != F_CMD_ABORT))
		{
			/*error, new cmd after end cmd*/
			ActTskCmdErr();
			return FALSE;
		}
		
		if(!SubCmdInQue(ptsk, pdst_act, src_cmd, dst_cmd, dly_ms, para, bplc, dst_act_sn))
			return FALSE;
		else
		{
			if(src_cmd == F_CMD_STOP || src_cmd == F_CMD_ABORT)
				ptsk->bstop_cmd_inque = TRUE;
			
			return TRUE;
		}
	}

	if(dst_cmd == F_CMD_RUN)
	{
		if(src_cmd == F_CMD_HOLD && pdst_act->ptsk == NULL)
			ret = ForkActTsk(pdst_act, SUSPEND_FORK, ptsk, para, dly_ms, bplc, dst_act_sn);
		else if(src_cmd == F_CMD_NULL && pdst_act->ptsk == NULL)
			ret = ForkActTsk(pdst_act, SYNC_FORK, ptsk, para, dly_ms, bplc, dst_act_sn);
	}
	else 
	{
		if(dst_cmd ==F_CMD_REAT && pdst_act->ptsk != NULL)
			ret = ReActiveActTsk(pdst_act->ptsk);
		else if(dst_cmd == F_CMD_HOLD && pdst_act->ptsk != NULL)
			ret = HoldActTsk(pdst_act->ptsk);
		else if(dst_cmd == F_CMD_STOP || dst_cmd == F_CMD_ABORT)
		{
			if(pdst_act->ptsk != NULL)
				EndActTsk(pdst_act->ptsk, dst_cmd == F_CMD_ABORT);
			ret = TRUE;
		}
		
		
		if(src_cmd == F_CMD_HOLD)
			ret = HoldActTsk(ptsk);
		else if(src_cmd == F_CMD_STOP || src_cmd == F_CMD_ABORT)
			ret = EndActTsk(ptsk, src_cmd == F_CMD_ABORT);
	}

	/**/
	if(ret == FALSE)
		ActTskCmdErr();

	return ret;
}

/*****************************************************************************
 函 数 名  : ActTaskLoop
 功能描述  : 任务运行函数
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年5月16日
    作    者   : ren chaohong
    修改内容   : 新生成函数

*****************************************************************************/
void ActTaskLoop()
{
	PACT_TSK	ptsk;
	PACT_TSK_ST	pst;
	UI16		act_ret;
	UI16		step;
	PACT		pact;
	ACT_MOV_STATE	mov_st;
	static UI16		act_run_cn_total = 100;
	UI16		act_run_flag = 0;
	
	ptsk = act_tsk;
	act_run_cn_total++;
	if(act_run_cn_total > 9999)
	{
		act_run_cn_total = 100;
	}
	
	while(ptsk < act_tsk + MAX_TSK_NUM)
	{
		pst = ptsk->pst;

		if(pst->tsk_st == TSK_IDLE)
		{
			ptsk++;
			continue;
		}

		//============分周期任务调度功能===============
		act_run_flag = 1;
		if (pst->act_run_cn > 1)
		{
			if ((act_run_cn_total-(UI16)(pst->act_run_cn_start))%(UI16)(pst->act_run_cn) > 0)
			{	
				act_run_flag = 0;
			}
		}
		//=============================================

		if (act_run_flag)
		{
			if(pst->tsk_st == TSK_RUN || (pst->tsk_st == TSK_DLY && (Get100UsTick() - pst->fork_100us) >= pst->dly_100us))
			{
				if(pst->tsk_st == TSK_DLY)
				{
					pst->tsk_st = TSK_RUN;
					MMIMonActRunNotify(pst->act_id);
				}
				
				if (pst->act_ret == ACT_INTF_OFF)
	            {
	                EndActTsk(ptsk, TRUE);
	            }
	            else if(pst->act_ret == ACT_ERR_ABORT)
				{
					EndActTsk(ptsk, TRUE);
				}
				else if(pst->act_ret == ACT_END && !ptsk->bplc)
				{
					EndActTsk(ptsk, FALSE);
				}
				else if(ptsk->pact != NULL)
				{
					pact = ptsk->pact;
					
					act_ret = ActRunLoop(pact, ptsk->pst->para);
					
	                pst->act_ret = act_ret;
	                if (act_ret == ACT_ERR_ABORT || act_ret == ACT_INTF_OFF)
	                {
	                    ActRunStop(pact);
	                }
						
					mov_st = (ACT_MOV_STATE)pact->mov_st;
					step = pact->step;
						
					if(mov_st != pst->mov_st || step != pst->step)
					{
						if(mov_st == ACT_MOVING && (mov_st != pst->mov_st ||  pst->mov_start_ms == 0))
							pst->mov_start_ms = GetMsTick();

						db_printf("ACT %04x state chg, ST: %d->%x, step:%04x->%04x\r\n", pact->id, pst->mov_st, mov_st, pst->step, step);

					//	if(MIN_STEP(step) == 0 && ptsk->pepo != NULL && act_ret != ACT_ERR_ABORT)
					//		ExPOOut(pact, ptsk->pepo, ptsk->epo_num, &ptsk->last_e_po);
					}
					if(ptsk->pepo != NULL && act_ret != ACT_ERR_ABORT)
					{
						if(!ChkSysCfg(CO3_PWRM))
						{
							ExPOOut(pact, ptsk->pepo, ptsk->epo_num, &ptsk->last_e_po);
						}						
					}


					pst->act_ret = act_ret;
					pst->mov_st = mov_st;
					pst->step = step;
					pst->next_step = pact->next_step;
					pst->step_100ms_h = ACT_STEP_100MS;
					pst->act_state = pact->ftaskstat; 

					pact->act_step_num++;	//没运行一次动作，动作步数累积一次。
					pst->act_run_ms = ACT_RUN_MS;

					if(act_ret == ACT_END)
						db_printf("act end\r\n");
					
					if(ptsk->pfork != NULL) 
						ScanActForkTbl(ptsk);
				}
			}
			else if (pst->act_ret == ACT_ERR_ABORT || pst->act_ret == ACT_INTF_OFF)
				EndActTsk(ptsk, TRUE);
		}		

		/*手动放开，置动作终止返回值，任务在下一次循环被结束*/
		if (ChkRunFlag(INTF_OFF) && pst->tsk_st != TSK_IDLE && ((!ptsk->pact->cannotbeclear)&&((pst->act_id&0xFF00)!=0x3000))) //手动放开动作不关闭润滑动作
		{			
			if(ptsk->pact != NULL)
			{
				pst->act_ret = ACT_INTF_OFF;
				ActErrorAbort(ptsk->pact);
			}
		}		
		ptsk++;
	}

	/*清除手动放开标志*/
	ClrRunFlag(INTF_OFF);
}

void PLCHoldAndRunActTsk(UI16 act_id, UI16 parent_id, UI32 para, UI16 dly_ms)
{
	PACT 	pact;
	PACT	ppact;
	
	pact = GetActByID(act_id);
	ppact = GetActByID(parent_id);

	if(pact == NULL || ppact == NULL)
		SetPLCRet(FALSE);
	else
		SetPLCRet(ForkActTsk(pact, SUSPEND_FORK, ppact->ptsk, para, dly_ms, TRUE, 0));
}

void PLCRunActTsk(UI16 act_id, UI16 para, UI16 dly_ms)
{
	PACT 	pact;
	
	pact = GetActByID(act_id);

	if(pact == NULL)
		SetPLCRet(FALSE);
	else
		SetPLCRet(ForkActTsk(pact, FIRST_FORK, NULL, (UI32)para, dly_ms, TRUE, 0));
}

void PLCEndActTsk(UI16 act_id)
{
	PACT 	pact;
	
	pact = GetActByID(act_id);
	
	if(pact == NULL || pact->ptsk == NULL)
		SetPLCRet(FALSE);
	else
	{
		if(pact->ptsk != NULL && pact->ptsk->pst != NULL && pact->ptsk->pst->act_ret != ACT_END)
			EndActTsk(pact->ptsk, TRUE);
		else
			EndActTsk(pact->ptsk, FALSE);
		
		SetPLCRet(TRUE);
	}
}

void PLCHoldActTsk(UI16 act_id)
{
	PACT 	pact;
	
	pact = GetActByID(act_id);
	
	if(pact == NULL || pact->ptsk == NULL)
		SetPLCRet(FALSE);
	else
	{
		HoldActTsk(pact->ptsk);
		SetPLCRet(TRUE);
	}
}

void PLCReActiveActTsk(UI16 act_id)
{
	PACT 		pact;
	
	pact = GetActByID(act_id);
	
	if(pact == NULL || pact->ptsk == NULL)
		SetPLCRet(FALSE);
	else
	{
		ReActiveActTsk(pact->ptsk);
		SetPLCRet(TRUE);
	}
}

BOOL ActTskIsEmpty()
{
	int i;

	for(i = 0; i < ARRAY_NUM(act_tsk); i++)
	{
		if(act_tsk[i].pst->tsk_st != TSK_IDLE)
			return FALSE;
	}

	return TRUE;
}

/*******************************************************************************************
 *Brief			:判断任务是否为处对应动作外无其他动作
 *Param			:例外动作的id
 *Return Value	:FALSE :非空	TRUE :空
 *Aauthor		:ZT
 *Date			:20201117
********************************************************************************************/
BOOL ActTskIsEmptyExpAct(UI16 act_id)
{
	int i;

	for(i = 0; i < ARRAY_NUM(act_tsk); i++)
	{
		if((act_tsk[i].pst->tsk_st != TSK_IDLE) && (act_tsk[i].pact->id != act_id))
			return FALSE;
	}

	return TRUE;
}


BOOL ActIsInTsk(UI16 act_id)
{
	int i;
	
	for(i = 0; i < ARRAY_NUM(act_tsk); i++)
	{
		if(act_tsk[i].pact != NULL && act_tsk[i].pact->id == act_id)
			return TRUE;
	}

	return FALSE;
}

BOOL PartActIsInTsk(UI16 part_id)
{
	int i;
	
	for(i = 0; i < ARRAY_NUM(act_tsk); i++)
	{
		if(act_tsk[i].pact != NULL && ((act_tsk[i].pact->id&0xFFF0) == (part_id&0xFFF0)))
			return TRUE;
	}

	return FALSE;
}


BOOL ActCtrInit()
{
	int	i;

	memset(act_tsk, 0, sizeof(act_tsk));
	memset(sub_que, 0, sizeof(sub_que));
	
	for(i = 0; i < MAX_TSK_NUM; i++)
	{
		if((act_tsk[i].pst = (PACT_TSK_ST)AllocMemTbl(MEMTBL_ACT_TSK, sizeof(UI16), ACT_TSK_ST_SIZE)) == NULL)
		{
			SetMachineErr(ER0_MEM);
			return FALSE;
		}
	}

	ActForkTblInit();
	return TRUE;
}

BOOL ActForkTblIsUsing()
{
	int	i;

	for(i = 0; i < ARRAY_NUM(act_tsk); i++)
	{
		if(act_tsk[i].pst->tsk_st != TSK_IDLE)
		{
			if(act_tsk[i].pfork != NULL)
				return TRUE;
		}
	}

	return FALSE;
}

BOOL SentTskState()
{
	int	i;
	int	num;
	UI32 tm_set,tm_run,tm_step,tm_yx;	//动作设定时间，运行时间，动作每步的时间，动作运动时间
	PPROT_TSK_ST_MSG_PKT ppkt;
	PPROT_TSK_ST_ITEM pit;
	PACT_TSK_ST pst;
	PACT_TSK ptsk;
	

	num = 0;
	for(i = 0; i < MAX_TSK_NUM; i++)
	{
		if(act_tsk[i].pst != NULL && act_tsk[i].pst->tsk_st == TSK_RUN && ((act_tsk[i].pact->type & ACT_CTRB) != ACT_CTRB)) //运行中的任务才上传，避免挂起任务过多，导致面板上不显示动作 20220801 ccz
		{
			num++;
		}		
	}

	//按照面板实际显示的面板动作数来显示。提高在多吹气联动动作时主机与232的负担 -20200909zt
	if (num > MAX_TSK_PANEL_SHOW)
	{
		num = MAX_TSK_PANEL_SHOW;
	}

	if((ppkt = (PPROT_TSK_ST_MSG_PKT)ProtNewPkt(TSK_ST_MSG, 0, FALSE,  sizeof(PROT_TSK_ST_MSG_PKT) + num*sizeof(PROT_TSK_ST_ITEM))) != NULL)
	{
		ppkt->num = num;
		ppkt->item_len = sizeof(PROT_TSK_ST_ITEM)/sizeof(UI16);
		pit = (PPROT_TSK_ST_ITEM)(ppkt + 1);
		for(i = 0; i < MAX_TSK_NUM && num > 0; i++)
		{
			ptsk = act_tsk + i;
			pst = ptsk->pst;
			if(pst != NULL && pst->tsk_st == TSK_RUN && ((ptsk->pact->type & ACT_CTRB) != ACT_CTRB) )
			{
				pit->act_id = pst->act_id;
				tm_set = pst->act_tm_n;
				pit->act_tm_L = (UI16)(tm_set & 0x0ffff);
				pit->act_tm_H = (UI16)((tm_set >> 16) & 0xffff);
				
				pit->step = pst->step;

				pit->tsk_st = pst->tsk_st;
				if(ptsk->act_set_start_ms != 0)
					tm_run = CalMsTo10MsUI16(ptsk->act_set_start_ms);
				else if(ptsk->pact != NULL && ptsk->pact->start_ms != 0)
					tm_run = CalMsTo10MsUI16(ptsk->pact->start_ms);
				else
					tm_run = Cal100UsTo10MsUI16(pst->fork_100us);

				pit->start_ms_L = (UI16)(tm_run & 0x0ffff);
				pit->start_ms_H = (UI16)((tm_run >> 16) & 0xffff);
				
				if(ptsk->pact != NULL && ptsk->pact->step_100us != 0)
					tm_step = Cal100UsTo10MsUI16(ptsk->pact->step_100us);
				else
					tm_step = 0;
				pit->step_ms_L = (UI16)(tm_step & 0x0ffff);
				pit->step_ms_H = (UI16)((tm_step >> 16) & 0xffff);
				
				if(ptsk->pact != NULL)
					tm_yx = CalMsTo10MsUI16(ptsk->pact->start_ms);
				else
					tm_yx = 0;
				pit->mov_ms_L = (UI16)(tm_yx & 0x0ffff);
				pit->mov_ms_H = (UI16)((tm_yx >> 16) & 0xffff);

				//特殊处理-在动作结束立刻上传时，面板对结束的动作步做了特殊处理，不进行刷新(如下方设定，面板会进行刷新) -20200909zt
				if (ptsk->pact->mov_st == ACT_MOV_END_PRE)
				{
					if (pit->act_tm_L == 0)
					{
						pit->act_tm_L = 1;
						pit->step = PRE_FIN_STEP;
					}
				}
				
				pit++;
				num--;
			}
		}

		return MainProtTxPkt(ppkt);
		//return ProtTxPkt(ppkt, 1);
	}
	else
		return FALSE;
}

