#include "com.h"
#include "part.h"
#include "acttbl.h"
#include "acttype.h"
#include "cfgdb.h"
#include "actfork.h"
#include "machine.h"

PTBL p_log_pi_tbl = NULL;
PTBL p_log_po_tbl = NULL;
PTBL p_e_po_tbl = NULL;
PTBL p_hydr_pri_tbl = NULL;

void InitPoSpecialLink(void);


BOOL PILogTblProc(PTBL ptbl)
{
	PPART pp;
	int i;
	PPI_LOG prow;
	UI16 row_len;
	UI16 row;
	UI8 cfg;
	UI16 last_pi;
	
	if(ptbl == NULL || ptbl->prow == NULL)
		return FALSE;
	
	/*清楚部件的所有输入点*/
	ClearAllPartSub(PI_ID);
	prow = (PPI_LOG)ptbl->prow;
	row = ptbl->hd.row;
	row_len = ptbl->hd.row_len;
	SetPIForceCfg(0);
	for(i = 0; i < row; i ++)
	{
		//mprintf("PITb, part_id = %04x	pi_sn = %d	ind = %d	cfg = %04x\r\n", prow->part_id,prow->pi_sn,prow->ind,prow->cfg);
		pp = GetPartByID(prow->part_id);
		if(pp != NULL)
		{
			/*如果表中，同一个逻辑点定义了多个物理点，以其中最小的有效物理点为准*/
			last_pi = 0;
			GetPartSubData(pp, PI_ID, prow->ind, &last_pi);
			if((CheckPIUsed(last_pi) == CheckPIUsed(prow->pi_sn) && last_pi > prow->pi_sn) || (CheckPIUsed(prow->pi_sn) && !CheckPIUsed(last_pi)))
			{
				SetPartSubData(pp, PI_ID, prow->ind, prow->pi_sn);

				/*除了I_RPM_CHK 螺杆转数检知外，所有的输入点缺省为1次滤波*/
				cfg = prow->cfg;
				if((cfg & 0x0f) == 0)
					cfg = (cfg | 0x01);
				SetPICfg(prow->pi_sn, cfg);

				if((GET_PART_TYPE(prow->part_id) == CORE_ID) && (prow->ind > 0) && (prow->ind < 3))
					SetPICore(prow->pi_sn,TRUE);
				else
					SetPICore(prow->pi_sn,FALSE);
			}
		}
		prow = (PPI_LOG)((UI32)prow + row_len);
	}

	return TRUE;
}

BOOL POLogTblProc(PTBL ptbl)
{
	PPART pp;
	int i;
	PPO_LOG prow;
	UI16 row_len;
	UI16 row;
	UI16 last_po;
	
	if(ptbl == NULL || ptbl->prow == NULL)
		return FALSE;
	
	/*清楚部件的所有输出点*/
	ClearAllPartSub(PO_ID);

	POChainClear();

	prow = (PPO_LOG)ptbl->prow;
	row = ptbl->hd.row;
	row_len = ptbl->hd.row_len;
	for(i = 0; i < row; i ++)
	{
		pp = GetPartByID(prow->part_id);

		//新增MaxPo判定，大于最大PO数量的点都废弃，防止多个点时Po点关联数据大于MaxPo
		if(pp != NULL && (prow->po_sn <= GetMaxPo()))
		{
			if(GetPartSubData(pp, PO_ID, prow->ind, &last_po) == TRUE)
			{
				if(last_po != 0)
				{
					/*设置输出点链接，用于一个逻辑点对应多个物理点*/
					POChainAdd(last_po, prow->po_sn);
				}
			}
			
			SetPartSubData(pp, PO_ID, prow->ind, prow->po_sn);
		}
		prow = (PPO_LOG)((UI32)prow + row_len);
	}
	
	InitPoSpecialLink();
	return TRUE;
}

PE_PO_ROW ExPOFilt(UI16 id, UI16 *pnum)//动作电气表筛选
{
	return (PE_PO_ROW)TBLGetRow(p_e_po_tbl, id, pnum);
}

PHY_PRI_ROW HydrPriFilt(UI16 id, UI16 *pnum)//动作电气表筛选
{
	return (PHY_PRI_ROW)TBLGetRow(p_hydr_pri_tbl,  id, pnum);
}

PTBL GetHydrTbl()
{
	return p_hydr_pri_tbl;
}

#if 0
void ExPOOn(PLAST_E_PO plst)
{
	int i;
	int num;

	if(plst != NULL)
	{
		for(i = 0; i < num; i++)
			SetPOBit(plst->po[i], (plst->st[i] << 1));
	}
}

void ExPOOff(PLAST_E_PO plst)
{
	int i;
	int num;

	if(plst != NULL)
	{
		for(i = 0; i < num; i++)
			SetPOBit(plst->po[i], (plst->st[i] << 1));
	}
}
#endif

void ExPOClear(PLAST_E_PO plst)
{
	int i;
	int num;

	num = plst->num;
	if(num != 0)
	{
		db_printf("EPO CLR:");
		for(i = 0; i < num; i++)
		{
			SetPOBit(plst->po[i], (plst->delay[i]));
			db_printf(" %d", plst->po[i]);
		}
		db_printf("\r\n");
	}
	plst->num = 0;
	plst->step = 0;
}

BOOL ExPOForceOut(PE_PO_ROW prow)
{
	int i;
	UI32 id;
	UI32 ind;
	PE_PO pe;
	UI32 name_st;
	PPART pp;
	
	if(prow == NULL)
	{
		return FALSE;
	}

	pe = prow->e_po;
	for(i = 0; i < ARRAY_NUM(prow->e_po); i++)
	{
		id = (pe->part_name & 0xfff0);
		ind = (pe->po_ind& 0xff);
		if(id == 0)
		{
			break;
		}
		else if((pp =GetPartByID(id)) != NULL)
		{
			name_st = pe->onoff_delay;
			if(pp->psub[PO_ID] != NULL && pp->sub_len[PO_ID] > ind)
			{
				ind = pp->psub[PO_ID][ind];
				SetPOBit(ind, name_st&0xff);
				db_printf("%d ", ind);
			}
		}

		pe++;
	}
	
	return TRUE;
}

BOOL ExPOForceOutClr(PE_PO_ROW prow)
{
	int i;
	UI32 id;
	UI32 ind;
	PE_PO pe;
	UI32 name_st;
	PPART pp;
	
	if(prow == NULL)
	{
		return FALSE;
	}

	pe = prow->e_po;
	for(i = 0; i < ARRAY_NUM(prow->e_po); i++)
	{
		id = (pe->part_name & 0xfff0);
		ind = (pe->po_ind& 0xff);
		if(id == 0)
		{
			break;
		}
		else if((pp =GetPartByID(id)) != NULL)
		{
			if(pp->psub[PO_ID] != NULL && pp->sub_len[PO_ID] > ind)
			{
				ind = pp->psub[PO_ID][ind];
				name_st = pe->onoff_delay;
				if((name_st & 0x01))
				{
					SetPOBit(ind, ((name_st >> 8) & 0xff));
				}
			}
		}

		pe++;
	}
	
	return TRUE;
}

UI16    GetVarValuePo(PEXP_VAR_PO pv)
{
	UI16 var;
	PPART pp;
	pp = GetPartByID(pv->ref_id);
	
	if(pv == NULL)
		return 0;
	
    switch (pv->type)
    {
       	case VAR_CONST:
			var = pv->ind;
			break;
			
		case VAR_SUB:
			if(pp != NULL && pv->sub < PART_SUB_NUM && (pp->sub_len[pv->sub] > pv->ind))	//部件不为空，子类小于最大子类数，同时参数的偏移量小于该子类数据的总长度
				var = pp->psub[pv->sub][pv->ind];
			break;
			
		case VAR_POINTER:
			//if(*(pv->ind) != NULL)
			//	var = *(pv->ind);
			var=0;
			break;
		
		case VAR_IND:
			//if(*(pv->ind) != NULL)
			//	var = *(pv->ind);
			var=0;
			break;

		case VAR_NULL:
			//if(*(pv->ind) != NULL)VAR_NULL
			//	var = *(pv->ind);
			var=0;
			break;		
		
		default:
			if(pp != NULL && pv->sub < PART_SUB_NUM && (pp->sub_len[pv->sub] > pv->ind))	//部件不为空，子类小于最大子类数，同时参数的偏移量小于该子类数据的总长度
				var = pp->psub[pv->sub][pv->ind];
			break;			      
      }	
	return var;
}

BOOL  ChkExtPohh(UI16 cmp_num,PE_PO_ROW peh)
{
	UI16 var1,var2,i;
	BOOL varlast = FALSE,varnow = FALSE;

	PFORK_CMP_PO pf;
	pf = (PFORK_CMP_PO)peh->cmp;
	
	if(cmp_num ==0||pf==NULL)
		return 0 ;
	if(cmp_num > MAX_E_PO_FORK_HH)   //比较项最大值做保护，防止数据出错时，由于比较项过大，引起指针指偏
		cmp_num = MAX_E_PO_FORK_HH;
	
	for(i=0;i<cmp_num;i++)
	{
		var1 = GetVarValuePo((PEXP_VAR_PO)(&pf->var1));
		var2 = GetVarValuePo((PEXP_VAR_PO)(&pf->var2));
		switch(pf->exp_op)
		{
	       	case EQ_OP:        // ==                                          
				if(var1==var2)
					varnow = TRUE;
				else
					varnow = FALSE;
				break;
			case NO_EQ_OP:     // !=
				if(var1!=var2)
					varnow = TRUE;
				else
					varnow = FALSE;
				break;
	       	case GT_OP:        // >
				if(var1>var2)
					varnow = TRUE;
				else
					varnow = FALSE;
				break;
			case LS_OP:        // <
				if(var1<var2)
					varnow = TRUE;
				else
					varnow = FALSE;
				break;
	       	case GT_EQ_OP:      // >=
				if(var1>=var2)
					varnow = TRUE;
				else
					varnow = FALSE;
				break;
			case LS_EQ_OP:       // <=
				if(var1<=var2)
					varnow = TRUE;
				else
					varnow = FALSE;
				break;		
			default:
				break;		
		}	
		if(i==0)
		{
			varlast=varnow;
		}
		else
		{
			if(pf->lg_op == 1)                //一共4个比较项，与前一个比较项之间的关系   为1时则代表AND
				varlast = (varlast&&varnow);
			else                              //一共4个比较项，与前一个比较项之间的关系  其他时则代表OR        pf->lg_op = 2
				varlast = (varlast||varnow);
		}
		//pf++;//现在比较项只有1个不需要累加了20190911HH
	}
	return varlast;
}



void PoFlag_Clear_hh(PACT_TSK ptsk,PE_PO_ROW prow,int row_num)
{
	int i;
	int j;
	PE_PO_ROW pend;

	if(prow == NULL || row_num <= 0)
		return;

	if(row_num > MAX_E_PO_ROW)
		row_num = MAX_E_PO_ROW;   //暂定每个动作最多配置10个电气点
		
	pend = prow + row_num;

	while(prow < pend)
	{
		for(i=0;i<row_num;i++)
		{
			for(j=0;j<MAX_E_PO_TBL;j++)
			{
			ptsk->bexec_Pohh[i][j] = FALSE;
			}
		}
			
		prow++;
	}

}


void ExPOOut(PACT pact, PE_PO_ROW prow, int row_num, PLAST_E_PO plst)//动作查帅选表规则输出
{
	PPART pp;
	int i;
	UI32 ind;
	PE_PO_ROW pend;
	UI32 tbl_step; 
	UI32 cur_step;
	PE_PO pe;
	UI32 id;
	UI32 name_st;
	int po_num;
	PACT_TSK ptsk;
	int j=0;

	if(prow ==NULL || row_num <= 0)
		return;

	if(row_num > MAX_E_PO_ROW)
	{
			SetMachineErr(ER0_TSKFORK);
	   row_num = MAX_E_PO_ROW;	//暂定每个动作最多配置10个电气点
	}	
	ptsk=pact->ptsk;//20190329

	pend = prow + row_num;
	cur_step = MAIN_STEP(pact->step);
	if((cur_step & 0x80) == (MAIN_STEP(STAGE_SPEC1_STEP) & 0x80) && cur_step < END_STEP)
		cur_step = cur_step - (MAIN_STEP(STAGE_SPEC1_STEP) & 0x80) + 0x10;
		
	while(prow < pend)
	{
		tbl_step = prow->step;
		if(cur_step>=((tbl_step >> 8) & 0xff) && cur_step <= (tbl_step & 0xff))
		{
			//当上一次有输出点输出过，同时动作的动作步变化时，关闭输出过的所有电气点，所有按照动作步输出点位的机制是动作步刚进入时打开，跳到下一个大步时则关闭
			if(plst->num > 0 && plst->step != tbl_step)
				ExPOClear(plst);
			
			pe = prow->e_po;  //取出要输出的是哪个部件下的点位，该点位的偏移，该点位的状态是开还是关的所在的那一行
			po_num = 0;       //将输出点的个数清零
			db_printf("EPO %04x step:", tbl_step);
			for(i = 0; i < ARRAY_NUM(prow->e_po); i++)
			{
				id = (pe->part_name & 0xfff0);   //取出要输出的电气点，在哪个部件号，不是指该电气点在哪里动作时输出的动作的部件号
				ind = (pe->po_ind& 0xff);     	 //取出要输出的电气点，在该部件下的偏移

				if((pp =GetPartByID(id)) != NULL)
				{
					if(ptsk->bexec_Pohh[j][i] == ((cur_step << 8) | TRUE))
					{
						pe++;
						continue;
					}					
					
					if((prow->add_condition & (1<<i)) != 0)//如果该阀点使用附加条件则判断下面的条件20190830HH
					{
					//如果不满足条件则进入20190830HH
					if(!(((((prow->en_oper_mode !=1)&&(prow->en_oper_mode==GetOperMode()))||((prow->en_oper_mode ==1)&&(ChkOperMode(OM_AUTO)    )))||(prow->en_oper_mode ==0xFFFF))&&
						((prow->cmp_num_po==0)||((prow->cmp_num_po>0)&&(ChkExtPohh(prow->cmp_num_po,prow))))
					  ))
					  {
					  pe++;
					  continue;
					  }
						
					}
				
					name_st = pe->onoff_delay; //得到开关状态，以及关延迟时间
					if(pp->psub[PO_ID] != NULL && pp->sub_len[PO_ID] > ind)  //输出点子类不为空，同时输出点子类的总数大于偏移量					
					{
						ind = pp->psub[PO_ID][ind];   //得到该 输出点的物理地址，即在画面上处于第几点					
						SetPOBit(ind, name_st&0xff);
						ptsk->bexec_Pohh[j][i] = (cur_step << 8) | TRUE;//记录当前表对应点已经输出,输出后就不再输出20190830HH	
						db_printf("%d ", ind);
						if(plst->step == 0 && (name_st & 0x01))  //当为首次进入时，且状态为输出时,防止后面输出点个数统计出错						
						{
							plst->po[po_num] = ind;   //记录当前输出的点的物理地址，用于后面关闭该点
							plst->delay[po_num] = (name_st >> 8) & 0xff;  //记录当前输出点的延迟关时间，用于后面点关闭时使用
							po_num++;   //将输出的点个数加1，一个动作，最大可支持6个点电气点
						}
					}
				}
				
				pe++;  //指向下一行，即该动作下，配置的下一个电气输出点，最多支持MAX_E_PO_TBL 个				
			}

			db_printf("\r\n");
			if(po_num > 0)  //当有点位输出过时，记录下当前的动作步和输出的点位个数			
			{
				plst->step = tbl_step;
				plst->num = po_num;
			}
			
			return;
		}

		prow++;  //指向下一个要 输出的电气点所在那一行
		j++;//当前任务动作的监控数量累加20191209HH

	}

	/*没有找到，也需关闭最近输出的电气输出点*/
	if(plst->num > 0)
		ExPOClear(plst);
}

//得到当前动作步的油路优先级表设置
PHY_PRI_ROW GetActStepPortPri(PACT pact, PHY_PRI_ROW prow, int num)
{
	PHY_PRI_ROW	pend;
	UI32	tbl_step;
	UI32	cur_step;
	
	if(prow == NULL || num <= 0)
		return NULL;

	pend = prow + num;
	cur_step = MAIN_STEP(pact->step);
	while(prow < pend)
	{
		tbl_step = prow->step;
		if(cur_step >= ((tbl_step >> 8) & 0xff) && cur_step <= (tbl_step & 0xff))
		{
			return prow;
		}
		prow++;
	}

	//没有找到油路优先级表设置，清除上一次查找结果。
	return NULL;
}

#if 0
BOOL PIOTblCheck(PTBL_HD phd, UI8* prow, int len)
{
	PTBL_TYPE_INFO pinfo;
	int i;
	int j;
	
	if((pinfo = TBLGetInfo(T_PI_LOG)) == NULL)
		return FALSE;
	
	if(prow == NULL || len < phd->row*phd->row_len || pinfo->ext_row_len != phd->row_len)
		return FALSE;

	for(i = 0; i < phd->row; i++)
	{
		for(j = j + 1; j < phd->row; j++)
		{
			
		}
	}
}
#endif

BOOL InitActTbl()
{
	p_log_pi_tbl = TBLGet(T_PI_LOG, 1, TBL_DB_VER);
	p_log_po_tbl = TBLGet(T_PO_LOG, 1, TBL_DB_VER);
	p_e_po_tbl = TBLGet(T_E_PO, 1, TBL_DB_VER);

	if((p_hydr_pri_tbl = TBLGet(T_HYDR_PRI, 1, TBL_DB_VER)) == NULL)
	{
		/*如果用户表存储区没有，查找出厂表存储区*/
		if((p_hydr_pri_tbl = TBLGet(T_HYDR_PRI, 2, TBL_DB_VER)) == NULL)
		{
			/*如果出厂表存储区也没有，用程序内置的油路优先级表*/
			SetHyPriTblDefault();
			AddTblToInd((PTBL)CfgDbTblRead(T_HYDR_PRI, 1, FACTORY_TBL_DB), FACTORY_TBL_DB, TBL_STOR, 0);
			p_hydr_pri_tbl = TBLGet(T_HYDR_PRI, 1, TBL_DB_VER);
		}
	}
	
	PILogTblProc(p_log_pi_tbl);
	POLogTblProc(p_log_po_tbl);

	return (p_log_pi_tbl != NULL && p_log_po_tbl != NULL);  
}

