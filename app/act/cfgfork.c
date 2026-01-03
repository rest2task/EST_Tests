/******************************************************************************
  文 件 名   : cfgfork.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 根据配置数据生成动作监控表，实现动作流程和插入
  函数列表   :
              ActForkClmpClsBfCoreFwd
              ActForkClmpClsMidCoreFwd
              ActForkClmpOpnBfCoreBwd
              ActForkClmpOpnEndCoreBwd
              ActForkClmpOpnMidCoreBwd
              ActForkClmpOpnMidEject
              ActForkClmpPrsEndCoreFwd
              ActForkEjectBwd
              CreateActForkTbl
              CreateMainActForkTbl
              CreateSubActForkTbl
              GetJointActSel
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

#include "actfork.h"
#include "machine.h"

#define A_RCYC	MK_ACT(MACHINE_ID, 1, OS_RCYC)
#define A_CLSM	MK_ACT(CLAMP_ID, 1, OS_CLSM)
#define A_PRSM	MK_ACT(CLAMP_ID, 1, OS_PRSM)
#define A_NZLF	MK_ACT(NOZZLE_ID, 1, OS_NOZF)
#define A_INJ	MK_ACT(INJECT_ID, 1, OS_INJE)
#define A_CHRG	MK_ACT(INJECT_ID, 1, OS_CHRG)
#define A_SUCK	MK_ACT(INJECT_ID, 1, OS_SUCK)
#define A_COOL	MK_ACT(NOZZLE_ID, 1, OS_COOL)
#define A_OPNM	MK_ACT(CLAMP_ID, 1, OS_OPNM)
#define A_EJT	MK_ACT(EJECT_ID, 1, OS_EJT)
#define A_PCYC	MK_ACT(MACHINE_ID, 1, OS_PRCYC)

#define CLAMP1	MK_PART(CLAMP_ID, 1)
#define INJECT1	MK_PART(INJECT_ID, 1)
#define NOZZLE1	MK_PART(NOZZLE_ID, 1)
#define SAFEDOOR1	MK_PART(SAFEDOOR_ID, 1)
#define LUB1	MK_PART(LUB_ID, 1)
#define LUB2	MK_PART(LUB_ID, 2)
#define LUB3	MK_PART(LUB_ID, 3)
#define SYSACC1	MK_PART(SYSACC_ID, 1)

#define ACT_SN_BEGIN	0xff00


/*关模前脱模退*/
void ActForkEjectBwd()
{
	ACT_FORK  fork;
	
	DEF_FORK(fork);
	fork.src_act_sn = MK_ACT(CLAMP_ID, 1, OS_CLSM);
	fork.src_cmd = F_CMD_HOLD;
	fork.dst_cmd = F_CMD_RUN;
	{
		PEJECT pejt;
		UI8 num;
		
		pejt = (PEJECT)GetMachineSubPartList(EJECT_ID, &num);
		fork.en_oper_mode = INVALIDE_EN;
		fork.en_mov_st = ACT_MOV_PRE;
		fork.en_step = INVALID_STEP;
		fork.cmp_num = 1;
		while(num > 0 && pejt != NULL)
		{
			if(!CHK_PART_ST(pejt, PART_DISABLE))
			{
				fork.cmp[0].var1.type = VAR_DST_SUB + PI_ID;
				fork.cmp[0].var1.ind = GET_IND(PEJECT_PI, I_EJT_BWD);
				fork.cmp[0].var2.type = VAR_CONST;
				fork.cmp[0].var2.ind = PI_SET;
				fork.cmp[0].exp_op = NO_EQ_OP;
				fork.cmp[0].lg_op = AND_LG;

				fork.dst_act_id = pejt->act[OS_EJTB - 1].id;
				//fork.dst_act_id = MK_ACT(GET_PART_TYPE(pejt->part.id), GET_PART_SN(pejt->part.id), OS_EJTB);
				ActForkTblInsert(&fork);
			}
			num--;
			pejt++;
		}
	}
}

void ActForkCorePri()//中子进退优先级排序
{
	PCORE pcore;
	UI8 num;   
	UI16 i,j;
	UI16 coreFwdPri[CLAMP_MAX_CORE] = {0}; //中子进优先级缓存
	UI16 coreBwdPri[CLAMP_MAX_CORE] = {0}; //中子退优先级缓存
	
	UI16 cur_mode,cur_pos,cur_pri = 0;//临时变量:当前中子的位置方式、动作位置、优先级
	UI16 other_mode,other_pos,other_pri = 0;//临时变量:其它中子的位置方式、动作位置、优先级
	UI16 high_priority_num = 0;//实际优先级更高的中子数量
	
	PCLAMP pclmp = (PCLAMP)GetPartByID(MK_PART(CLAMP_ID, 1));
	PMACHINE pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1));
	
	pcore = (PCORE)GetMachineSubPartList(CORE_ID, &num);//返回了起始第一个指针地址和数量 

	if(num > CLAMP_MAX_CORE)
		num = CLAMP_MAX_CORE;

	for(i=0;i<num;i++)
	{
		if((pcore+i) != NULL)
		{
			if(ChkSysCfg(XF2_SUCWO))
			{
				coreFwdPri[i] = (pcore+i)->md->PRI_CORE_FWD;
				coreBwdPri[i] = (pcore+i)->md->PRI_CORE_BWD;
			}
			else    //中子优先级功能不使用时使用默认排序
			{
				coreFwdPri[i] = i + 1;
				coreBwdPri[i] = num - i;
			}
		}
	}
		
	if(1)//中子进的排序
	{
		for(i=0;i<num;i++)//进顺序初始化赋值ABCDEFGH 12345678
		{
			pclmp->coreinturn[i] = i+1;
		}

		/*计算比当前中子优先级更高的中子数量*/
		for(i=0;i<num;i++)
		{
			if((pcore+i) != NULL)
			{
				high_priority_num = 0;
				cur_mode = (pcore+i)->md->ADPOSI_CORE_FWD;//当前中子位置方式
				cur_pos = (pcore+i)->pa->ADPOSI_COREINACT;//当前中子动作位置
				cur_pri = coreFwdPri[i];//当前中子优先级
				for(j=0;j<num;j++)
				{
					if(j!=i && (pcore+j) != NULL)
					{
						other_mode = (pcore+j)->md->ADPOSI_CORE_FWD;//其它中子位置方式
						other_pos = (pcore+j)->pa->ADPOSI_COREINACT;//其它中子动作位置
						other_pri = coreFwdPri[j];//其它中子优先级
						
						if(other_mode < cur_mode)
							high_priority_num++; //一、动作位置方式不同:位置方式小的实际优先级高;					
						else if((other_mode == cur_mode) && (cur_mode == 0 || cur_mode == 2) && (other_pri < cur_pri))
							high_priority_num++; //二、动作位置方式相同，都为关模前或关模后:设定优先级小的实际优先级高;
						else if((other_mode == cur_mode) && cur_mode == 1 && (other_pos == cur_pos) && (other_pri < cur_pri))
							high_priority_num++; //三、动作位置方式相同，都为关模中，且动作位置相同:设定优先级小的实际优先级高;
						else if((other_mode == cur_mode) && cur_mode == 1 && (other_pos > cur_pos))
							high_priority_num++; //四、动作位置方式相同，都为关模中，但动作位置不同:设定动作位置大的优先级高。
					}
				}
				(pcore+i)->CoreFwdActualSequence= high_priority_num + 1; //若更高优先级中子个数为3，则表示此中子顺序为4
			}
		}

		for(i=0;i<num;i++) //根据排序结果，获得中子编号的排列
		{
			for(j=0;j<num;j++)
			{
				if((pcore+j) != NULL)
				{
					if((pcore+j)->CoreFwdActualSequence == (i+1))
					{
						pclmp->coreinturn[i]=j+1; //coreinturn[2]=5表示中子E进实际顺序为3
						break;///当查询到一个中子优先级后，就跳出当前j，进入下一个i
					}
				}
			}
		}

		for(i=0;i<num;i++)
		{
			pclmp->coreinturn[i]=pclmp->coreinturn[i]-1;
		} 
	}	 

	if(1)//中子退的排序
	{
		for(i=0;i<num;i++)//退顺序初始化赋值HGFEDCBA 87654321
		{
			pclmp->coreoutturn[i] = num-i;//87654321
		}
		
   		/*计算比当前中子优先级更高的中子数量*/
		for(i=0;i<num;i++)
		{
			if((pcore+i) != NULL)
			{
				high_priority_num = 0;
				cur_mode = (pcore+i)->md->ADPOSI_CORE_BWD;//当前中子位置方式
				cur_pos = (pcore+i)->pa->ADPOSI_COREOUTACT;//当前中子动作位置
				cur_pri = coreBwdPri[i];//当前中子优先级
				for(j=0;j<num;j++)
				{
					if(j!=i && (pcore+j) != NULL)
					{
						other_mode = (pcore+j)->md->ADPOSI_CORE_BWD;//其它中子位置方式
						other_pos = (pcore+j)->pa->ADPOSI_COREOUTACT;//其它中子动作位置
						other_pri = coreBwdPri[j];//其它中子优先级
						
						if(other_mode > cur_mode)
							high_priority_num++; //一、动作位置方式不同:位置方式大的实际优先级高;					
						else if((other_mode == cur_mode) && (cur_mode == 0 || cur_mode == 2) && (other_pri < cur_pri))
							high_priority_num++; //二、动作位置方式相同，都为开模前或开模后:设定优先级小的实际优先级高;
						else if((other_mode == cur_mode) && cur_mode == 1 && (other_pos == cur_pos) && (other_pri < cur_pri))
							high_priority_num++; //三、动作位置方式相同，都为开模中，且动作位置相同:设定优先级小的实际优先级高;
						else if((other_mode == cur_mode) && cur_mode == 1 && (other_pos < cur_pos))
							high_priority_num++; //四、动作位置方式相同，都为开模中，但动作位置不同:设定动作位置小的优先级高。
					}
				}
				(pcore+i)->CoreBwdActualSequence = high_priority_num + 1; //若更高优先级中子个数为3，则表示此中子顺序为4
			}
		}

		for(i=0;i<num;i++) //根据排序结果，获得中子编号的排列
		{
			for(j=0;j<num;j++)
			{
				if((pcore+j) != NULL)
				{
					if(((pcore+j)->CoreBwdActualSequence) == (i+1))
					{
						pclmp->coreoutturn[i]=j+1; //coreinturn[1]=4表示中子D进实际顺序为2
						break;//当查询到一个中子优先级后，就跳出当前j，进入下一个i
					}	
				}
			}
		}
		
		for(i=0;i<num;i++)
		{
			pclmp->coreoutturn[i]=pclmp->coreoutturn[i]-1;
		}		 
	}
}

/*******************************************************************************************
 *任务名称	:关模前中子进	
 *判定条件	:中子进位置方式=0(关模前)
 *插入条件	:电子尺位置≥开模五段终止位置-开模有效区
 *作者		:陈超泽
 *日期		:2021-6-9
********************************************************************************************/
void ActForkClmpClsBfCoreFwd(UID id)
{
	ACT_FORK  fork;
	UI16 tmp_pos;
	int NUM;
	UI8 corenum;

	NUM = GetJointActSel2();
	if(NUM > 0 && NUM < CLAMP_MAX_CORE+1)
	{
	}	
	else
		NUM = 0;
	
	DEF_FORK(fork);
	fork.src_act_sn = id;
	fork.src_cmd = F_CMD_HOLD;
	fork.dst_cmd = F_CMD_RUN;
	{
		PCLAMP	pclmp;
		
	    fork.en_mov_st = ACT_MOV_PRE;
		if((pclmp = (PCLAMP)GetMachineSubPart(CLAMP_ID, 1)) != NULL)
		{
		    PCORE pcore;
			UI8 num;
	
			if(pclmp->pa->ADPOSI_OPENENDEND < pclmp->mh->ADPOSI_EFFCPR)
				tmp_pos = 0;
			else
				tmp_pos = pclmp->pa->ADPOSI_OPENENDEND - pclmp->mh->ADPOSI_EFFCPR;
			
			pcore = (PCORE)GetMachineSubPartList(CORE_ID, &num);
			
			if(ChkSysCfg(XF2_SUCWO))//中子优先级功能 
            {
                pcore = (PCORE)GetMachineSubPartList(CORE_ID, &num); 
                corenum = num;                
                pcore = (pcore+pclmp->coreinturn[corenum-num]); 
            } 
			
			while(num > 0 && pcore != NULL)
			{
				if(pcore->md->FL_SPECIALCODE == 0 || pcore->md->FL_SPECIALCODE == 3 || pcore->md->FL_SPECIALCODE == 6)
				{
					if(NUM != GET_PART_SN(pcore->part.id))
					{
						if(!CHK_PART_ST(pcore, PART_DISABLE)&& ((pcore->md->FL_CORE_CTRL >0 )&& (pcore->md->FL_CORE_CTRL <3))
						  && (pcore->md->ADPOSI_CORE_FWD == 0))
						{
							fork.cmp_num = 1;
							fork.cmp[0].var1.type = VAR_IND;
							fork.cmp[0].var1.ind = VAR_METER;
							fork.cmp[0].var2.type = VAR_CONST;
							fork.cmp[0].var2.ind =tmp_pos;
							fork.cmp[0].exp_op = GT_EQ_OP;
							fork.cmp[0].lg_op = AND_LG;
							
							fork.dst_act_id = pcore->act[OS_CPRF - 1].id;
							ActForkTblInsert(&fork);
						}
					}
				}
				num--;
				pcore++;
				
				if(ChkSysCfg(XF2_SUCWO)) 
                {
                    pcore = (PCORE)GetMachineSubPartList(CORE_ID, NULL);                    
                    pcore = (pcore+pclmp->coreinturn[corenum-num]); 
                }
			}
		}
	}
}

/*******************************************************************************************
 *任务名称	:关模中中子进/关模联动中子进	
 *判定条件	:中子进位置方式=1(关模中)
 *插入条件	:电子尺位置≤中子进动作位置/关模联动位置
 *作者		:陈超泽
 *日期		:2021-6-9
********************************************************************************************/
void ActForkClmpClsMidCoreFwd(UID id)
{
	ACT_FORK  fork;
	int NUM;
	UI8 corenum;
	
	NUM = GetJointActSel2();
	if(NUM > 0 && NUM < CLAMP_MAX_CORE+1)
	{
	}	
	else
		NUM = 0;
	
	DEF_FORK(fork);
	fork.src_act_sn = id;
	fork.dst_cmd = F_CMD_RUN;
	fork.en_mov_st = ACT_MOVING;
	fork.en_oper_mode = INVALIDE_EN;
	{
		PCLAMP	pclmp;
		if((pclmp = (PCLAMP)GetMachineSubPart(CLAMP_ID, 1)) != NULL)
		{
		    PCORE pcore;
			UI8 num;
	
			pcore = (PCORE)GetMachineSubPartList(CORE_ID, &num);

			if(ChkSysCfg(XF2_SUCWO)) 
            {
                pcore = (PCORE)GetMachineSubPartList(CORE_ID, &num); 
                corenum = num;                
                pcore = (pcore+pclmp->coreinturn[corenum-num]); 
            }  
			pclmp->clampmidcorefwd = 0;
			while(num > 0 && pcore != NULL)
			{
				if(pcore->md->FL_SPECIALCODE == 0 || pcore->md->FL_SPECIALCODE == 3 || pcore->md->FL_SPECIALCODE == 6)
				{
					if(!CHK_PART_ST(pcore, PART_DISABLE)&& pcore->md->FL_CORE_CTRL >0 && pcore->md->FL_CORE_CTRL <3 )
					{
						//关摸联动中子进判断
						if(NUM == GET_PART_SN(pcore->part.id))
						{
							if(ChkSysCfg(C08_MANUAL))
								fork.en_oper_mode = INVALIDE_EN;
							else
								fork.en_oper_mode = OM_AUTO;
							fork.src_cmd = F_CMD_NULL;
							fork.dst_cmd = F_CMD_RUN;							
							fork.cmp_num = 1;
							fork.cmp[0].var1.type = VAR_IND;
							fork.cmp[0].var1.ind = VAR_METER;
							fork.cmp[0].var2.type = VAR_CONST;
							fork.cmp[0].var2.ind = pclmp->md->ADPOSI_EJTTRAVELCLOSE;
							fork.cmp[0].exp_op = LS_EQ_OP;
							fork.cmp[0].lg_op = AND_LG;
							pcore->CoreChk = pcore->CoreChk & 0xff; //清除高八位数据为0，使不检查中子进
							fork.dst_act_id = pcore->act[OS_CPRF - 1].id;
							ActForkTblInsert(&fork);
							fork.en_oper_mode = INVALIDE_EN;
						}
						else if(pcore->md->ADPOSI_CORE_FWD == 1)
						{
							fork.cmp_num = 1;
							fork.src_cmd = F_CMD_HOLD;
							fork.dst_cmd = F_CMD_RUN;
							fork.cmp[0].var1.type = VAR_IND;
							fork.cmp[0].var1.ind = VAR_METER;
							fork.cmp[0].var2.type = VAR_CONST;
							fork.cmp[0].var2.ind = pcore->pa->ADPOSI_COREINACT;
							fork.cmp[0].exp_op = LS_EQ_OP;
							fork.cmp[0].lg_op = AND_LG;

							fork.dst_act_id = pcore->act[OS_CPRF - 1].id;
							
							ActForkTblInsert(&fork);
							pclmp->clampmidcorefwd=( pclmp->clampmidcorefwd|(1<<(GET_PART_SN(pcore->part.id)-1))); //2015-03-01  途中中子进标记
						}
					}
				}

				num--;
				pcore++;

				if(ChkSysCfg(XF2_SUCWO)) 
                {
                    pcore = (PCORE)GetMachineSubPartList(CORE_ID, NULL);                
                    pcore = (pcore+pclmp->coreinturn[corenum-num]); 
                }  
			}
		}
	}
}

/*******************************************************************************************
 *任务名称	:压模后中子进	
 *判定条件	:中子进位置方式=1(关模后)
 *插入条件	:压到"关模终"(即压模结束)
 *作者		:陈超泽
 *日期		:2021-6-9
********************************************************************************************/
void ActForkClmpPrsEndCoreFwd(UID id)
{
	ACT_FORK  fork;
	int NUM;
	UI8 corenum;

	NUM = GetJointActSel2();
	if(NUM > 0 && NUM < CLAMP_MAX_CORE+1)
	{
	}	
	else
		NUM = 0;
	
	DEF_FORK(fork);
	fork.src_act_sn = id;
	fork.src_cmd = F_CMD_HOLD;
	fork.dst_cmd = F_CMD_RUN;
	fork.en_mov_st = ACT_MOV_END_PRE;
	{

		PCLAMP	pclmp;
		if((pclmp = (PCLAMP)GetMachineSubPart(CLAMP_ID, 1)) != NULL)
		{
		    PCORE pcore;
			UI8 num;
	
			pcore = (PCORE)GetMachineSubPartList(CORE_ID, &num);
			
			if(ChkSysCfg(XF2_SUCWO)) 
            {
                pcore = (PCORE)GetMachineSubPartList(CORE_ID, &num); 
                corenum = num;                
                pcore = (pcore+pclmp->coreinturn[corenum-num]); 
            }   
			
			while(num > 0 && pcore != NULL)
			{
				if(pcore->md->FL_SPECIALCODE == 0 || pcore->md->FL_SPECIALCODE == 3 || pcore->md->FL_SPECIALCODE == 6)
				{
					if(NUM != GET_PART_SN(pcore->part.id))
					{
						if(!CHK_PART_ST(pcore, PART_DISABLE)&& ((pcore->md->FL_CORE_CTRL >0 )&& (pcore->md->FL_CORE_CTRL <3))
						    &&(pcore->md->ADPOSI_CORE_FWD == 2))
						{
							fork.dst_act_id = pcore->act[OS_CPRF - 1].id;
							ActForkTblInsert(&fork);
						}
					}
				}

				num--;
				pcore++;
				
				if(ChkSysCfg(XF2_SUCWO)) 
                {
                    pcore = (PCORE)GetMachineSubPartList(CORE_ID, NULL);                
                    pcore = (pcore+pclmp->coreinturn[corenum-num]); 
                }
			}
		}
	}
}

/*******************************************************************************************
 *任务名称	:开模前中子退	
 *判定条件	:中子退位置方式=2(开模前)
 *插入条件	:电子尺位置≤关模低压终止位置
 *作者		:陈超泽
 *日期		:2021-6-9
********************************************************************************************/
void ActForkClmpOpnBfCoreBwd(UID id)
{
	ACT_FORK  fork;
	int sel;
	
	DEF_FORK(fork);
	fork.src_act_sn = id;
	fork.src_cmd = F_CMD_HOLD;
	fork.dst_cmd = F_CMD_RUN;
    fork.en_mov_st = ACT_MOV_PRE;

	sel = GetJointActSel();
	if(sel > 1 && sel < CLAMP_MAX_CORE+2)
		sel = sel - 1;
	else
		sel = 0;
	
	{
		PCLAMP	pclmp;
		if((pclmp = (PCLAMP)GetMachineSubPart(CLAMP_ID, 1)) != NULL)
		{
			PCORE pcore;
			UI8 num;

			pcore = (PCORE)GetMachineSubPartList(CORE_ID, &num);

			if(ChkSysCfg(XF2_SUCWO)) 
            {
                pcore = (PCORE)GetMachineSubPartList(CORE_ID, &num);              
                pcore = (pcore+pclmp->coreoutturn[CLAMP_MAX_CORE-num]); 
            }
			else
				pcore += num;
			
			while(num > 0 && pcore != NULL)
			{
				if(ChkSysCfg(XF2_SUCWO)) 
                {                
                }
                else
					pcore--;

				if(pcore->md->FL_SPECIALCODE == 0 || pcore->md->FL_SPECIALCODE == 2 || pcore->md->FL_SPECIALCODE == 6)
				{
					//排除联动中子
					if(sel != GET_PART_SN(pcore->part.id))
					{
						if(!CHK_PART_ST(pcore, PART_DISABLE)&& ((pcore->md->FL_CORE_CTRL >0 )&& (pcore->md->FL_CORE_CTRL < 3))
						  &&(pcore->md->ADPOSI_CORE_BWD == 2))
						{
							fork.cmp_num = 1;
							fork.cmp[0].var1.type = VAR_IND;
							fork.cmp[0].var1.ind = VAR_METER;
							fork.cmp[0].var2.type = VAR_CONST;
							fork.cmp[0].var2.ind =pclmp->pa->ADPOSI_CLSPROTECTEND;
							fork.cmp[0].exp_op = LS_EQ_OP;
							fork.cmp[0].lg_op = AND_LG;
							fork.dst_act_id = pcore->act[OS_CPRB - 1].id;
							ActForkTblInsert(&fork);
						}
					}
				}

				num--;
				if(ChkSysCfg(XF2_SUCWO)) 
                {
                    pcore = (PCORE)GetMachineSubPartList(CORE_ID, NULL);             
                    pcore = (pcore+pclmp->coreoutturn[CLAMP_MAX_CORE-num]); 
                } 
			}
		}
	}
}

/*******************************************************************************************
 *任务名称	:开模中中子退/开模联动中子退	
 *判定条件	:中子退位置方式=1(开模中)
 *插入条件	:电子尺位置≥中子退动作位置/开模联动位置
 *作者		:陈超泽
 *日期		:2021-6-9
********************************************************************************************/
void ActForkClmpOpnMidCoreBwd(UID id)
{
	PMACHINE pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1));
	ACT_FORK  fork;
	int sel;
	
	DEF_FORK(fork);
	fork.src_act_sn = id;
	fork.dst_cmd = F_CMD_RUN;
	fork.en_mov_st = ACT_MOVING;

	sel = GetJointActSel();
	if(sel > 1 && sel < CLAMP_MAX_CORE+2)
	{
		sel = sel - 1;
		pm->clampopncore_num=sel; 
	}
	else
		sel = 0;
	
	{
		PCLAMP	pclmp;
		PCORE pcore;
		UI8 num;
		
		if((pclmp = (PCLAMP)GetMachineSubPart(CLAMP_ID, 1)) != NULL)
		{
 			pcore = (PCORE)GetMachineSubPartList(CORE_ID, &num);
			if(ChkSysCfg(XF2_SUCWO)) 
            {
                pcore = (PCORE)GetMachineSubPartList(CORE_ID, &num);            
                pcore = (pcore+pclmp->coreoutturn[CLAMP_MAX_CORE-num]); 
            }
            else
				pcore += num;

			pclmp->clampmidcorebwd = 0;
			while(num > 0 && pcore != NULL)
			{
				if(ChkSysCfg(XF2_SUCWO)) 
                {                
                }
                else 
					pcore--;

				if(pcore->md->FL_SPECIALCODE == 0 || pcore->md->FL_SPECIALCODE == 2 || pcore->md->FL_SPECIALCODE == 6)
				{
					if(!CHK_PART_ST(pcore, PART_DISABLE)&& pcore->md->FL_CORE_CTRL > 0 && pcore->md->FL_CORE_CTRL < 3)
					{
						//开模联动中子退判断
						if(sel == GET_PART_SN(pcore->part.id))
						{
							if(ChkSysCfg(C08_MANUAL))
								fork.en_oper_mode = INVALIDE_EN;
							else
								fork.en_oper_mode = OM_AUTO;
							fork.cmp_num = 1;
							fork.src_cmd = F_CMD_NULL;
							fork.cmp[0].var1.type = VAR_IND;
							fork.cmp[0].var1.ind = VAR_METER;
							fork.cmp[0].var2.type = VAR_CONST;
							fork.cmp[0].var2.ind = pclmp->md->ADPOSI_EJTTRAVEL;
							fork.cmp[0].exp_op = GT_EQ_OP;
							fork.cmp[0].lg_op = AND_LG;
							fork.dst_act_id = pcore->act[OS_CPRB - 1].id;
							ActForkTblInsert(&fork);
							
							fork.en_oper_mode = INVALIDE_EN;
						}
						else if(pcore->md->ADPOSI_CORE_BWD == 1)
						{
							fork.cmp_num = 1;
							fork.src_cmd = F_CMD_HOLD;
							fork.cmp[0].var1.type = VAR_IND;
							fork.cmp[0].var1.ind = VAR_METER;
							fork.cmp[0].var2.type = VAR_CONST;
							fork.cmp[0].var2.ind = pcore->pa->ADPOSI_COREOUTACT;
							fork.cmp[0].exp_op = GT_EQ_OP;
							fork.cmp[0].lg_op = AND_LG;

							fork.dst_act_id = pcore->act[OS_CPRB - 1].id;
							ActForkTblInsert(&fork);
							pclmp->clampmidcorebwd=( pclmp->clampmidcorebwd|(1<<(GET_PART_SN(pcore->part.id)-1))); //2015-03-01  途中中子退标记
						}
					}
				}				

				num--;
				if(ChkSysCfg(XF2_SUCWO)) 
                {
                    pcore = (PCORE)GetMachineSubPartList(CORE_ID, NULL);                 
                    pcore = (pcore+pclmp->coreoutturn[CLAMP_MAX_CORE-num]); 
                }  
			}
		}
	}
}

/*******************************************************************************************
 *任务名称	:开模后中子退	
 *判定条件	:中子退位置方式=0(开模后)
 *插入条件	:电子尺位置≥开模五段终止位置(即开模结束)
 *作者		:陈超泽
 *日期		:2021-6-9
********************************************************************************************/
void ActForkClmpOpnEndCoreBwd(UID id)
{
	ACT_FORK  fork;
	int sel;
	
	DEF_FORK(fork);
	fork.src_act_sn = id;
	fork.src_cmd = F_CMD_HOLD;
	fork.dst_cmd = F_CMD_RUN;
    fork.en_mov_st = ACT_MOV_END_PRE;

	sel = GetJointActSel();
	if(sel > 1 && sel < CLAMP_MAX_CORE+2)
		sel = sel - 1;
	else
		sel = 0;
	{
		PCLAMP	pclmp;
		if((pclmp = (PCLAMP)GetMachineSubPart(CLAMP_ID, 1)) != NULL)
		{
			PCORE pcore;
			UI8 num;
			pcore = (PCORE)GetMachineSubPartList(CORE_ID, &num);
			if(ChkSysCfg(XF2_SUCWO)) 
            {
                pcore = (PCORE)GetMachineSubPartList(CORE_ID, &num);             
                pcore = (pcore+pclmp->coreoutturn[CLAMP_MAX_CORE-num]); 
            }
            else 
				pcore += num;			
				
			while(num > 0 && pcore != NULL)
			{
				if(ChkSysCfg(XF2_SUCWO)) 
                {                
                }
                else 
					pcore--;

				if(pcore->md->FL_SPECIALCODE == 0 || pcore->md->FL_SPECIALCODE == 2 || pcore->md->FL_SPECIALCODE == 6)
				{
					if(sel != GET_PART_SN(pcore->part.id))
					{
						if(!CHK_PART_ST(pcore, PART_DISABLE)&& ((pcore->md->FL_CORE_CTRL >0 )&& (pcore->md->FL_CORE_CTRL <3))
						    &&(pcore->md->ADPOSI_CORE_BWD == 0))
						{
							fork.dst_act_id = pcore->act[OS_CPRB - 1].id;
							ActForkTblInsert(&fork);
						}
					}
				}

				num--;
				if(ChkSysCfg(XF2_SUCWO)) 
                {
                    pcore = (PCORE)GetMachineSubPartList(CORE_ID, NULL);                
                    pcore = (pcore+pclmp->coreoutturn[CLAMP_MAX_CORE-num]); 
                }
			}
		}
	}
}

/*特殊中子
特殊中子说明：
为了避免手动状态下做中子动作警报"动作位置偏差"，需要根据中子动作时模板的位置让面板强制设定位置方式。 ccz

1：关模-》压模-》座进-》射出-》储料-》射退-》座退-》冷却-》开模-》顶进-》中子A退-》中子A进-》剩余顶针次数-》循环
     （开关模皆不做中子A）   //中子A进位置方式:关模前   中子A退位置方式:开模后

2：关模-》压模-》座进-》射出-》储料-》射退-》座退-》冷却-》开模-》顶针-》中子A进-》开关安全门下一循环
     （合模不做中子A进，开模正常中子A退）   //中子A进位置方式:关模前

3：关模-》压模-》座进-》射出-》储料-》射退-》座退-》冷却-》开模-》顶针-》中子A退-》开关安全门下一循环
     （合模正常做中子A进，开模不做中子A退）   //中子A退位置方式:开模后

4：关模-》压模-》座进-》中子A进-》射出-》中子A退-》储料-》射退-》座退-》冷却-》开模-》顶针-》开关安全门下一循环
     （开关模不做中子A进和A退）   //中子A进位置方式:关模后   中子A退位置方式:开模前

5：关模-》压模-》座进-》射出-》储料-》射退-》座退-》冷却-》开模-》顶针-》中子A进-》中子A退-》开关安全门下一循环
     （开关模不做A进和A退）   //中子A进位置方式:关模前   中子A退位置方式:开模后

6：关模-》压模-》座进-》射出-》储料-》射退-》中子A退-》座退-》冷却-》开模-》顶针-》开关安全门下一循环
     （合模正常做中子A进，开模不做中子A退）   //中子A退位置方式:开模前

7：关模-》压模-》座进-》射出-》储料-》射退-》冷却-》开模-》中子A进-》中子A退-》顶针-》开关安全门下一循环
     （托模前中子进退）   //中子A进位置方式:关模前   中子A退位置方式:开模后
  
所有特殊中子的插入皆在自动情况下，手动情况不于考虑
*/
void ActForkSpecialCore()
{
	UI8 core_sn;
	
	PCORE pcore;
	UI8 num;
	
	pcore = (PCORE)GetMachineSubPartList(CORE_ID, &num);
	while(num > 0 && pcore != NULL)
	{
		if(!CHK_PART_ST(pcore, PART_DISABLE)&& (pcore->md->FL_CORE_CTRL >0 && pcore->md->FL_CORE_CTRL <3))
		{
			core_sn = GET_PART_SN(pcore->part.id);
			switch(pcore->md->FL_SPECIALCODE)
			{
				case 1:		//原来是在开模后，中子退，进，再脱模	 DK 2014.1104		还是有问题。。每次脱模进，都会有中子
			 		ActNoCmpFork(MK_ACT(EJECT_ID, 1, OS_EJTF), MK_ACT(CORE_ID, core_sn, OS_CPRB), SKIP_STEP, SKIP_NSTEP, ACT_MOV_END_PRE, SKIP_ERR, OM_AUTO, SKIP_RET, TASKSTNDDLY, 1, TRUE, 0);
					if(!ChkSysCfg(CO4_MEJT))
						ActNoCmpFork(MK_ACT(EJECT_ID, 1, OS_EJTF), MK_ACT(CORE_ID, core_sn, OS_CPRF), SKIP_STEP, SKIP_NSTEP, ACT_MOV_END_PRE, SKIP_ERR, OM_AUTO, SKIP_RET, TASKSTNDDLY, 1, TRUE, 0);
					else
						ActNoCmpFork(MK_ACT(CLAMP_ID, 1, OS_CLSM), MK_ACT(CORE_ID, core_sn, OS_CPRF), INIT1_STEP, SKIP_NSTEP, SKIP_MOV, SKIP_ERR, OM_AUTO, SKIP_RET, TASKSTNDDLY, 0, TRUE, 0);
					break;
				case 2:
					ActMovFork(MK_ACT(EJECT_ID, 1, OS_EJT), MK_ACT(CORE_ID, core_sn, OS_CPRF), ACT_MOV_END_PRE, TRUE);
					break;
				case 3:
					ActMovFork(MK_ACT(EJECT_ID, 1, OS_EJT), MK_ACT(CORE_ID, core_sn, OS_CPRB), ACT_MOV_END_PRE, TRUE);
					break;
				case 4:
					ActMovFork(MK_ACT(NOZZLE_ID, 1, OS_NOZF), MK_ACT(CORE_ID, core_sn, OS_CPRF), ACT_MOV_END_PRE, TRUE);
					ActMovFork(MK_ACT(INJECT_ID, 1, OS_INJE), MK_ACT(CORE_ID, core_sn, OS_CPRB), ACT_MOV_END_PRE, TRUE);
					break;
				case 5:
					ActMovFork(MK_ACT(EJECT_ID, 1, OS_EJT), MK_ACT(CORE_ID, core_sn, OS_CPRF), ACT_MOV_END_PRE, TRUE);
					ActMovFork(MK_ACT(EJECT_ID, 1, OS_EJT), MK_ACT(CORE_ID, core_sn, OS_CPRB), ACT_MOV_END_PRE, TRUE);
					break;
				case 6:
					ActMovFork(MK_ACT(INJECT_ID, 1, OS_SUCK), MK_ACT(CORE_ID, core_sn, OS_CPRB), ACT_MOV_END_PRE, TRUE);
					break;				
				case 7:
					ActMovFork(MK_ACT(EJECT_ID, 1, OS_EJT), MK_ACT(CORE_ID, core_sn, OS_CPRF), ACT_MOV_INIT, TRUE);
					ActMovFork(MK_ACT(EJECT_ID, 1, OS_EJT), MK_ACT(CORE_ID, core_sn, OS_CPRB), ACT_MOV_INIT, TRUE);
					break;
				default:
					break;
			}
		}

		num--;
		pcore++;
	}
}

//开模脱模联动
void ActForkClmpOpnMidEject()
{
	ACT_FORK  fork;
	PCLAMP	pclmp;
	UI16	POS_CLAMPEJT;	//开模联动脱模位置中间量
	UI16	POS_END;	//开模终止位置
	UI16	POS_SLOW;	//开模一段位置
	
	DEF_FORK(fork);
	fork.src_act_sn = MK_ACT(CLAMP_ID, 1, OS_OPNM);
	fork.src_cmd = F_CMD_NULL;
	fork.dst_cmd = F_CMD_RUN;
	fork.en_oper_mode = INVALIDE_EN;
	fork.en_mov_st = ACT_MOVING;
	fork.en_oper_mode = INVALIDE_EN;

	if(ChkSysCfg(C08_MANUAL))
		fork.en_oper_mode = INVALIDE_EN;
	else
		fork.en_oper_mode = OM_AUTO;

	if((pclmp = (PCLAMP)GetMachineSubPart(CLAMP_ID, 1)) != NULL)
	{
		
		fork.cmp_num = 1;
 		fork.cmp[0].var1.type = VAR_IND;
		fork.cmp[0].var1.ind = VAR_METER;
		fork.cmp[0].var2.type = VAR_CONST;

		if(pclmp->md->FL_CLSOPEN_MODE == 2)		//当用方向比例阀时，终止位置为另外一个地址数据，虽然面板上将这两个地址同步过，这里还是再加一句保护判断  20200217
		{
			POS_SLOW = pclmp->pa->ADPOS_OPENSPD1;
			POS_END = pclmp->pa->ADPOS_OPENSPD3;
		}
		else
		{	
			POS_SLOW = pclmp->pa->ADPOSI_OPENENDSLOW;
			POS_END = pclmp->pa->ADPOSI_OPENENDEND;
		}
		
		/*避免脱模顶针损害模具, 联动位置不能小于开模一慢位置*/
		POS_CLAMPEJT = pclmp->md->ADPOSI_EJTTRAVEL;
		if (pclmp->md->ADPOSI_EJTTRAVEL < POS_SLOW)
		{
			POS_CLAMPEJT = POS_SLOW;
		}

		if(POS_CLAMPEJT >= POS_END)
		{
			POS_CLAMPEJT = POS_END;
		}
		
		fork.cmp[0].var2.ind = POS_CLAMPEJT;
		fork.cmp[0].exp_op = GT_EQ_OP;
		fork.cmp[0].lg_op = AND_LG;
		fork.dst_act_id = MK_ACT(EJECT_ID, 1, OS_EJT);
		ActForkTblInsert(&fork);
	}
}


//射出途中排气  按照时间
void ActForkEXHAUSTMidInj()
{
	ACT_FORK  fork;
	//UI16 tmp_tm;
	PINJECT pinj;
	//射出途中排气
	DEF_FORK(fork);
	fork.src_act_sn = MK_ACT(INJECT_ID, 1, OS_INJE);
	fork.src_cmd = F_CMD_HOLD;
	fork.dst_cmd = F_CMD_RUN;
	fork.en_mov_st = ACT_MOVING;
	fork.en_oper_mode = OM_AUTO;
	if((pinj = (PINJECT)GetMachineSubPart(INJECT_ID, 1)) != NULL)
	{
			fork.cmp_num = 1;
			fork.cmp[0].var1.type = VAR_IND;
			fork.cmp[0].var1.ind = VAR_RUN_MS;
			fork.cmp[0].var2.type = VAR_CONST;
			if(ChkSysCfg(CO1_INTM))	
				fork.cmp[0].var2.ind = pinj->mh->TM_EXINJTM;
			else
				fork.cmp[0].var2.ind = pinj->mh->TM_EXINJTM*10;
			fork.cmp[0].exp_op = GT_EQ_OP;
			fork.cmp[0].lg_op = AND_LG;
			fork.dst_act_id =MK_ACT(CLAMP_ID, 1, OS_EXHAST) ;
			ActForkTblInsert(&fork);
	}
}


//射出途中排气  按照位置
void ActForkEXHAUSPosidInj()
{
	ACT_FORK  fork;
	//UI16 tmp_tm;
	PINJECT pinj;
	//射出途中排气
	DEF_FORK(fork);
	fork.src_act_sn = MK_ACT(INJECT_ID, 1, OS_INJE);
	fork.src_cmd = F_CMD_HOLD;
	fork.dst_cmd = F_CMD_RUN;
	fork.en_mov_st = ACT_MOVING;
	fork.en_oper_mode = OM_AUTO;
	if((pinj = (PINJECT)GetMachineSubPart(INJECT_ID, 1)) != NULL)
	{
			fork.cmp_num = 1;
			fork.cmp[0].var1.type = VAR_IND;
			fork.cmp[0].var1.ind = VAR_METER;
			fork.cmp[0].var2.type = VAR_CONST;
			fork.cmp[0].var2.ind = pinj->mh->POS_EXINJTM;
			fork.cmp[0].exp_op = LS_EQ_OP;
			fork.cmp[0].lg_op = AND_LG;
			fork.dst_act_id =MK_ACT(CLAMP_ID, 1, OS_EXHAST) ;
			ActForkTblInsert(&fork);
	}
}


void ActForkSeqVlv()
{
	UI16 tmp_val;
	if(ChkSysCfg(CO43_BLAST))
	{
		tmp_val = GetSubDataByID(MK_PART(INJECT_ID, 1), MH_ID, GET_IND(PINJECT_MACHSET, INJ_METERCH));
		ActNoCmpFork(MK_ACT(INJECT_ID, 1, OS_INJE),  MK_ACT(AIRBLOW_ID,  1, OS_SEQ1), INJCT1, SKIP_NSTEP, ACT_MOVING, SKIP_ERR, SKIP_OPER, SKIP_RET, 0, tmp_val, FALSE, 0);
	}
}


void  CreateSubActForkTbl()
{
	PCLAMP pclmp = (PCLAMP)GetPartByID(MK_PART(CLAMP_ID, 1));
	PADMETER padm = (PADMETER)GetPartByID(MK_PART(ADMETER_ID, 1));
	PNOZZLE	pn = (PNOZZLE)GetPartByID(MK_PART(NOZZLE_ID, 1));
	UI16 tmp_val,tmp_val2;
	int num;
	UI32 injtm32;
	int i;

	if(pclmp->mh->ADPOSI_EFFCPR <= 0 || pclmp->mh->ADPOSI_EFFCPR > padm->st->ADPOSI_METERMAX[1])   //开模位置有效区超限做保护，默认为50
		pclmp->mh->ADPOSI_EFFCPR = 500;
	/*特殊中子*/
	ActForkSpecialCore();

	ActForkCorePri();//中子进退优先级排序
        
	if(ChkSysCfg(CO3_SDRP))
		ActErrAbortFork(MK_ACT(SAFEDOOR_ID, 1, OS_SDRC), MK_ACT(SAFEDOOR_ID, 1, OS_SDRO), 0, INVALIDE_EN);
	
	/*关模************************************/
	if(ChkSysCfg(XF0_SYSACC))
		ActNoDlyFork(MK_ACT(CLAMP_ID, 1, OS_CLSM), MK_ACT(SYSACC_ID, 1, OS_SYSACC),  INIT1_STEP, INVALIDE_EN, INVALIDE_EN, TRUE);

	if(ChkSysCfg(CO5_ADJMOLD))
		ActNoDlyFork(MK_ACT(CLAMP_ID, 1, OS_CLSM), MK_ACT(ADJUST_ID, 1, OS_ADJMOLD),  INIT1_STEP, INVALIDE_EN, INVALIDE_EN, TRUE);


	//胶口针阀
	if(GetSubDataByID(MK_PART(INJECT_ID, 1), MH_ID, GET_IND(PINJECT_MACHSET, FL_INJ_SPC_FUNC)) == 0 && (GetSubDataByID(MK_PART(INJECT_ID, 1), MH_ID, GET_IND(PINJECT_MACHSET, POS_INJ_SPC)) > 0))
	{
		ActNoDlyFork(MK_ACT(CLAMP_ID, 1, OS_CLSM), MK_ACT(INJECT_ID, 1, OS_NEEDLEVALVE),  INIT1_STEP, INVALIDE_EN, OM_AUTO, FALSE);
		ActNoDlyFork(MK_ACT(CLAMP_ID, 1, OS_PRSM), MK_ACT(INJECT_ID, 1, OS_NEEDLEVALVE),  INIT1_STEP, INVALIDE_EN, OM_AUTO, FALSE);
	}
	else
	{
		ActNoDlyFork(MK_ACT(INJECT_ID, 1, OS_INJE), MK_ACT(INJECT_ID, 1, OS_NEEDLEVALVE),  INIT1_STEP, INVALIDE_EN, INVALIDE_EN, FALSE);
	}

	ActNoDlyFork(MK_ACT(INJECT_ID, 1, OS_INJE), MK_ACT(INJECT_ID, 1, OS_NEEDLEVALVE),  INIT1_STEP, INVALIDE_EN, OM_MANUAL, FALSE);
	ActNoDlyFork(MK_ACT(INJECT_ID, 1, OS_AINJE), MK_ACT(INJECT_ID, 1, OS_NEEDLEVALVE),  INIT1_STEP, INVALIDE_EN, SKIP_OPER, FALSE); //原本这里判断调模模式，导致手动射出走调模模式时不输出胶口针阀 2022.5.24 ccz
	
	
	/*合模前脱模退*/
//	ActForkEjectBwd();		
	/*关模前中子*/
	ActForkClmpClsBfCoreFwd(MK_ACT(CLAMP_ID, 1, OS_CLSM));
	if(ChkSysCfg(CO4_APUG))     //调模关摸前中子
		ActForkClmpClsBfCoreFwd(MK_ACT(CLAMP_ID, 1, OS_ACLS));


	//关模中中子
	ActForkClmpClsMidCoreFwd(MK_ACT(CLAMP_ID, 1, OS_CLSM));
	if(ChkSysCfg(CO4_APUG))     //调模关摸中中子
		ActForkClmpClsMidCoreFwd(MK_ACT(CLAMP_ID, 1, OS_ACLS));


	/******************************************/
	
	/*压模*************************************/
	/*压模中中子进*/
	ActForkClmpClsMidCoreFwd(MK_ACT(CLAMP_ID, 1, OS_PRSM));
	
	/*合模结束中子进*/
	ActForkClmpPrsEndCoreFwd(MK_ACT(CLAMP_ID, 1, OS_PRSM));
	if(ChkSysCfg(CO4_APUG))     //调模压模结束中子进
		ActForkClmpPrsEndCoreFwd(MK_ACT(CLAMP_ID, 1, OS_ACLS));
	
	/*****************************************/
	
	/*座进*/
	/*座进前射出*/
	if(ChkSysCfg(CO4_INPG) && ((GetSubDataByID(MK_PART(INJECT_ID, 1), MD_ID, GET_IND(PINJECT_MOLDSET, FL_AUTOPURGE)) == 1) 
		|| (GetSubDataByID(MK_PART(INJECT_ID, 1), MD_ID, GET_IND(PINJECT_MOLDSET, FL_AUTOPURGE)) == 2)))
		ActMovFork(MK_ACT(NOZZLE_ID, 1, OS_NOZF), MK_ACT(INJECT_ID, 1, OS_NZLINJ),  ACT_MOV_PRE, TRUE);

	/*******************************************/	

	/*射出**************************************/
	/*射前储料*/
	if(ChkSysCfg(XF6_INJDBL) && GetSubDataByID(MK_PART(INJECT_ID, 1), MD_ID, GET_IND(PINJECT_MOLDSET, TM_CHGFLOWMOLDING)) > 0)
		ActMovFork(MK_ACT(INJECT_ID, 1, OS_INJE), MK_ACT(INJECT_ID, 1, OS_CHRGBFINJ),  ACT_MOV_PRE, TRUE);
	
	if(GetSubDataByID(MK_PART(EJECT_ID, 1), MD_ID, GET_IND(PEJECT_MOLDSET, FL_INJEJECT)) == 1)
		ActNoCmpFork(MK_ACT(INJECT_ID, 1, OS_INJE), MK_ACT(EJECT_ID, 1, OS_EJT), SKIP_STEP, SKIP_NSTEP, ACT_MOV_END_PRE, SKIP_ERR, OM_AUTO, SKIP_RET, TASKSTNDDLY, 0x2118, TRUE, 0);

    if(ChkSysCfg(CO43_IINJ) && (GetSubDataByID(MK_PART(EJECT_ID, 1), MD_ID, GET_IND(PEJECT_MOLDSET, FL_INJEJECT)) == 1) && (GetSubDataByID(MK_PART(EJECT_ID, 1), MD_ID, GET_IND(PEJECT_MOLDSET, FL_INJEJTBWDBEFCLAMPOPN)) == 1))
		ActStepMovParaFork(MK_ACT(CLAMP_ID, 1, OS_OPNM), MK_ACT(EJECT_ID, 1, OS_EJT),  INIT1_STEP, INVALIDE_EN , 0x2119 , TRUE);
    
	//储能加压
	if(ChkSysCfg(CO3_STEN) && GetSubDataByID(MK_PART(INJECT_ID, 1), MD_ID, GET_IND(PINJECT_MOLDSET, FL_INCACC)) == 1) 
		ActMovModeFork(MK_ACT(INJECT_ID, 1, OS_INJE), MK_ACT(INJSTOR_ID, 1, OS_INJSTO),  ACT_MOV_PRE, SKIP_OPER, TRUE);

	/*射出前吹气*/
	num = GetMachineSubPartNum(AIRBLOW_ID);
	for(i = 0; i < num; i++)
	{
		tmp_val = GetSubDataByID(MK_PART(AIRBLOW_ID, i + 1), MD_ID, GET_IND(PAIRBLOW_MOLDSET, FL_BLAST));
		if(tmp_val == 2 || tmp_val == 3)
		{
			tmp_val = GetSubDataByID(MK_PART(AIRBLOW_ID, i + 1), MD_ID, GET_IND(PAIRBLOW_MOLDSET, TM_MOVINGPLATENACT));
			if(tmp_val > 0)
			{
				tmp_val = GetSubDataByID(MK_PART(INJECT_ID, 1), MH_ID, GET_IND(PINJECT_MACHSET, INJ_METERCH));

				if(ChkSysCfg(CO3_MAIR))
					ActNoCmpFork(MK_ACT(INJECT_ID, 1, OS_INJE),  MK_ACT(AIRBLOW_ID, i + 1, OS_AIRBL), INJCT1, SKIP_NSTEP, ACT_MOVING, SKIP_ERR, SKIP_OPER, SKIP_RET, 0, tmp_val, FALSE, 0);
				else
					ActNoCmpFork(MK_ACT(INJECT_ID, 1, OS_INJE),  MK_ACT(AIRBLOW_ID, i + 1, OS_AIRBL), INJCT1, SKIP_NSTEP, ACT_MOVING, SKIP_ERR, OM_AUTO, SKIP_RET, 0, tmp_val, FALSE, 0);
			}
		}
	}

	/*射出时顺序阀*/
	ActForkSeqVlv();	

	/*储料位置吹气*/
    num = GetMachineSubPartNum(AIRBLOW_ID);
	for(i = 0; i < num; i++)
	{
		tmp_val = GetSubDataByID(MK_PART(AIRBLOW_ID, i + 1), MD_ID, GET_IND(PAIRBLOW_MOLDSET, FL_BLAST));
		if(tmp_val == 9)
		{
			tmp_val = GetSubDataByID(MK_PART(AIRBLOW_ID, i + 1), MD_ID, GET_IND(PAIRBLOW_MOLDSET, TM_MOVINGPLATENACT));
			if(tmp_val > 0)
			{
				tmp_val = GetSubDataByID(MK_PART(INJECT_ID, 1), MH_ID, GET_IND(PINJECT_MACHSET,INJ_METERCH));
				if(ChkSysCfg(CO3_MAIR))
					ActNoCmpFork(MK_ACT(INJECT_ID, 1, OS_CHRG),  MK_ACT(AIRBLOW_ID, i + 1, OS_AIRBL), SKIP_STEP, SKIP_NSTEP, ACT_MOV_PRE, SKIP_ERR, SKIP_OPER, SKIP_RET, 0, tmp_val, FALSE, 0);
				else
					ActNoCmpFork(MK_ACT(INJECT_ID, 1, OS_CHRG),  MK_ACT(AIRBLOW_ID, i + 1, OS_AIRBL), SKIP_STEP, SKIP_NSTEP, ACT_MOV_PRE, SKIP_ERR, OM_AUTO, SKIP_RET, 0, tmp_val, FALSE, 0);
			}
		}
	}

	/*关模时吹气*/
	num = GetMachineSubPartNum(AIRBLOW_ID);
	for(i = 0; i < num; i++)
	{
		tmp_val2 = GetSubDataByID(MK_PART(AIRBLOW_ID, i + 1), MD_ID, GET_IND(PAIRBLOW_MOLDSET, FL_BLAST));
		if(tmp_val2 == 4 || tmp_val2 == 5)
		{
			tmp_val = GetSubDataByID(MK_PART(AIRBLOW_ID, i + 1), MD_ID, GET_IND(PAIRBLOW_MOLDSET, TM_MOVINGPLATENACT));
			if(tmp_val > 0 || (tmp_val2 == 5 && ChkSysCfg(XF2_DPUMP)))	//甬江客户吹气特殊
			{
				tmp_val =  GetSubDataByID(MK_PART(CLAMP_ID, 1), MH_ID, GET_IND(PCLAMP_MACHSET, CLMP_METERCH));
				
				if(ChkSysCfg(CO3_MAIR))
					ActNoCmpFork(MK_ACT(CLAMP_ID, 1, OS_CLSM),  MK_ACT(AIRBLOW_ID, i + 1, OS_AIRBL), SKIP_NSTEP, SKIP_NSTEP, ACT_MOV_PRE, SKIP_ERR, SKIP_OPER, SKIP_RET, 0, tmp_val, FALSE, 0);
				else
					ActNoCmpFork(MK_ACT(CLAMP_ID, 1, OS_CLSM),  MK_ACT(AIRBLOW_ID, i + 1, OS_AIRBL), SKIP_NSTEP, SKIP_NSTEP, ACT_MOV_PRE, SKIP_ERR, OM_AUTO, SKIP_RET, 0, tmp_val, FALSE, 0);
			}
		}
	}
	
		
	/*二次射出，储料*/
	if(ChkSysCfg(XF6_INJDBL) && GetSubDataByID(MK_PART(INJECT_ID, 1), MD_ID, GET_IND(PINJECT_MOLDSET, ADPOSI_CHGFLOWMOLD)) > 0)
		ActNoCmpFork(MK_ACT(INJECT_ID, 1, OS_INJE), MK_ACT(INJECT_ID, 1, OS_CHRGBFINJ), SKIP_STEP, INJCT1, SKIP_MOV, SKIP_ERR, OM_AUTO, ACT_OK, 0, 2, TRUE, 0);	 //只在全自动下动作 DK 2014.1105
	//	ActNoCmpFork(MK_ACT(INJECT_ID, 1, OS_INJE), MK_ACT(INJECT_ID, 1, OS_CHRGBFINJ), SKIP_STEP, INJCT1, SKIP_MOV, SKIP_ERR, SKIP_OPER, ACT_OK, 0, 2, TRUE, 0);
	if((ChkSysCfg(CO2_INJCLS))&&(pclmp->md->FL_EXHAUSTMODE==7))
	{
		if(GetSysMold()->FL_INJMOLDIN ==2)			//压塑在射出一段后
		{
			injtm32 = GetSubDataByID(MK_PART(INJECT_ID, 1), PA_ID, GET_IND(PINJECT_ACTPARA, TM_INJ1)) + (GetSubDataByID(MK_PART(INJECT_ID, 1), PA_ID, GET_IND(PINJECT_ACTPARA, TM_INJ1_H)) << 16);
			if((UI32)GetSubDataByID(MK_PART(INJECT_ID, 1), MD_ID, GET_IND(PINJECT_MOLDSET, TM_INJMOLDINTIME)) < injtm32)
			{
				tmp_val = INJCT2;
			}
			else
			{
				if(GetSubDataByID(MK_PART(INJECT_ID, 1), MD_ID, GET_IND(PINJECT_MOLDSET, FL_TURNTOHOLD)) == 5)
				{
					tmp_val = INJCT2;
				}
				else
				{
					tmp_val = HOLD1;
				}
			}
		}			
		else if(GetSysMold()->FL_INJMOLDIN ==1)		/*压塑在射出后，压模*/
			tmp_val = HOLD1;
		else
			tmp_val = 0;
		
		if(tmp_val)
		{
		    ActNoCmpFork(MK_ACT(INJECT_ID, 1, OS_INJE), MK_ACT(CLAMP_ID, 1, OS_PRSM), SKIP_STEP, tmp_val, SKIP_MOV, SKIP_ERR, OM_AUTO, SKIP_RET, TASKSTNDDLY, 0, TRUE, ACT_SN_BEGIN + 1);
		}
		else if(GetSysMold()->FL_INJMOLDIN ==3)
		{
		    ActNoCmpFork(MK_ACT(INJECT_ID, 1, OS_INJE), MK_ACT(CLAMP_ID, 1, OS_PRSM), SKIP_STEP, INJCT2, SKIP_MOV, SKIP_ERR, OM_AUTO, SKIP_RET, TASKSTNDDLY, 0, TRUE, ACT_SN_BEGIN + 1);
		}
	}

    if((ChkSysCfg(CO2_INJCLS))&&(GetSysMold()->FL_INJMOLDIN == 4))
		ActNoCmpFork(MK_ACT(INJECT_ID, 1, OS_INJE), MK_ACT(CLAMP_ID, 1, OS_CLSM), SKIP_STEP, INJCT2, SKIP_MOV, SKIP_ERR, OM_AUTO, SKIP_RET, TASKSTNDDLY, 1, TRUE, ACT_SN_BEGIN + 1);
    
	if(ChkSysCfg(CO3_BAKE))
	{
		tmp_val = GetSubDataByID(MK_PART(CLAMP_ID, 1), MD_ID, GET_IND(PCLAMP_MOLDSET, FL_EXHAUSTMODE));
		/*排气在射出后*/
		if(tmp_val == 1)
			ActNextStepFork(MK_ACT(INJECT_ID, 1, OS_INJE), MK_ACT(CLAMP_ID, 1, OS_EXHAST),  HOLD1, SKIP_MOV, TRUE);
		else if(tmp_val == 2)		//排气在保压一段后
			ActNextStepFork(MK_ACT(INJECT_ID, 1, OS_INJE), MK_ACT(CLAMP_ID, 1, OS_EXHAST),  HOLD2,SKIP_MOV, TRUE);//之前的显示时间有问题  2014.1104
		else if(tmp_val == 3)		//排气在保压后
			ActMovFork(MK_ACT(INJECT_ID, 1, OS_INJE), MK_ACT(CLAMP_ID, 1, OS_EXHAST),  ACT_MOV_END_PRE, TRUE);
		else if(tmp_val == 5)
			ActForkEXHAUSTMidInj();
		else if(tmp_val == 6)
			ActForkEXHAUSPosidInj();
        else if(tmp_val == 8)		//排气在保压二段后
		    ActNextStepFork(MK_ACT(INJECT_ID, 1, OS_INJE), MK_ACT(CLAMP_ID, 1, OS_EXHAST),  HOLD3,SKIP_MOV, TRUE);
        else if(tmp_val == 9)
		{
			if(GetSubDataByID(MK_PART(CLAMP_ID, 1), MD_ID, GET_IND(PCLAMP_MOLDSET, FL_EXHAUSTAFTINJ)) == 1)
				ActNoCmpFork(MK_ACT(INJECT_ID, 1, OS_INJE), MK_ACT(CLAMP_ID, 1, OS_EXHAST), SKIP_STEP, HOLD1, SKIP_MOV, SKIP_ERR, OM_AUTO, SKIP_RET, TASKSTNDDLY, EXHAST_INJEND, TRUE, 0);

			if(GetSubDataByID(MK_PART(CLAMP_ID, 1), MD_ID, GET_IND(PCLAMP_MOLDSET, FL_EXHAUSTAFTHOLD1)) == 1)
				ActNoCmpFork(MK_ACT(INJECT_ID, 1, OS_INJE), MK_ACT(CLAMP_ID, 1, OS_EXHAST), SKIP_STEP, HOLD2, SKIP_MOV, SKIP_ERR, OM_AUTO, SKIP_RET, TASKSTNDDLY, EXHAST_HOLD1, TRUE, 0);

			if(GetSubDataByID(MK_PART(CLAMP_ID, 1), MD_ID, GET_IND(PCLAMP_MOLDSET, FL_EXHAUSTAFTHOLD2)) == 1)
				ActNoCmpFork(MK_ACT(INJECT_ID, 1, OS_INJE), MK_ACT(CLAMP_ID, 1, OS_EXHAST), SKIP_STEP, HOLD3, SKIP_MOV, SKIP_ERR, OM_AUTO, SKIP_RET, TASKSTNDDLY, EXHAST_HOLD2, TRUE, 0);
			
			if(GetSubDataByID(MK_PART(CLAMP_ID, 1), MD_ID, GET_IND(PCLAMP_MOLDSET, FL_EXHAUSTAFTHOLD)) == 1)
				ActStepMovParaFork(MK_ACT(INJECT_ID, 1, OS_INJE), MK_ACT(CLAMP_ID, 1, OS_EXHAST),  END_STEP, INVALIDE_EN , EXHAST_HOLDEND , TRUE);
		}
	}

	/****************************************/
	
	/*储料***********************************/
	if(ChkSysCfg(CO0_BMC))
	{
		ActStepModeFork(MK_ACT(INJECT_ID, 1, OS_CHRG), MK_ACT(CHGPRS_ID, 1, OS_CHGPRSF), STAGE_1_STEP, SKIP_OPER, FALSE);
	}
    
	//储前冷却
	ActMovFork(MK_ACT(INJECT_ID, 1, OS_CHRG), MK_ACT(NOZZLE_ID, 1, OS_COLBC), ACT_MOV_PRE, TRUE);

	//储前射退
	if(GetSubDataByID(INJECT1, MD_ID, GET_IND(PINJECT_MOLDSET, FL_SUCKBACKBEFORECHG)) > 0 && 
	  (GetSubDataByID(INJECT1, MD_ID, GET_IND(PINJECT_MOLDSET, POS_SUCKBACKBEFORECHG)) > 0 || GetSubDataByID(INJECT1, MD_ID, GET_IND(PINJECT_MOLDSET, TM_SUCKBACKBEFORECHG)) > 0))
	{
		ActNoCmpFork(MK_ACT(INJECT_ID, 1, OS_CHRG), MK_ACT(INJECT_ID, 1, OS_SUCKCHG), INIT1_STEP, SKIP_NSTEP, SKIP_MOV, SKIP_ERR, OM_AUTO, SKIP_RET, TASKSTNDDLY, 0, TRUE, 0);
	}

	if (GetSubDataByID(INJECT1, MH_ID, GET_IND(PINJECT_MACHSET, FL_CHG_CHECKMM)) == 1)
	{		
		//手动时，强制执行储前射退功能
		ActMovForkmaul(MK_ACT(INJECT_ID, 1, OS_CHRG), MK_ACT(INJECT_ID, 1, OS_SUCKCHG), ACT_MOV_PRE, TRUE,2);
	}	
	
	// 边开模边储料
	//if(GetJointActSel()==6)
	if(ChkSysCfg(CO1_OPDC) && GetSysMold()->FL_OPNDURINGCOOL==1)
		ActStepMovFork(MK_ACT(INJECT_ID, 1, OS_CHRG), MK_ACT(NOZZLE_ID, 1, OS_COOL), CHARGE1, ACT_MOVING, FALSE);
		
	
	//手动储料后做射退
	if(!ChkSysCfg(CO1_MCNS))
		ActEndModeFork(MK_ACT(INJECT_ID, 1, OS_CHRG), MK_ACT(INJECT_ID, 1, OS_SUCK), OM_MANUAL);

    if(GetSubDataByID(INJECT1, MH_ID, GET_IND(PINJECT_MACHSET, FL_ELECTRIC_CHG)) == 1
        && GetSubDataByID(INJECT1, MH_ID, GET_IND(PINJECT_MACHSET, FL_ELECTRIC_NOZZLEFWD)) == 0) //电储料同步座台进 2023-5-22 ccz
	{
		ActNoDlyFork(MK_ACT(INJECT_ID, 1, OS_CHRG), MK_ACT(NOZZLE_ID, 1, OS_NOZF_SYNC), STAGE_1_STEP, INVALIDE_EN, INVALIDE_EN, FALSE);
	}

    if(GetSubDataByID(INJECT1, MH_ID, GET_IND(PINJECT_MACHSET, MODE_CHOOSE)) == 1) //免烘干机，储料时输出射退动作
	{
		ActNoDlyFork(MK_ACT(INJECT_ID, 1, OS_CHRG), MK_ACT(INJECT_ID, 1, OS_SUCK_SYNC), SKIP_STEP, ACT_MOV_PRE, SKIP_OPER, FALSE);
        ActNoDlyFork(MK_ACT(INJECT_ID, 1, OS_ACHRG), MK_ACT(INJECT_ID, 1, OS_SUCK_SYNC), SKIP_STEP, ACT_MOV_PRE, SKIP_OPER, FALSE);
        ActNoDlyFork(MK_ACT(INJECT_ID, 1, OS_PCHRG), MK_ACT(INJECT_ID, 1, OS_SUCK_SYNC), SKIP_STEP, ACT_MOV_PRE, SKIP_OPER, FALSE);
        ActNoDlyFork(MK_ACT(INJECT_ID, 1, OS_CHRGBFINJ), MK_ACT(INJECT_ID, 1, OS_SUCK_SYNC), SKIP_STEP, ACT_MOV_PRE, SKIP_OPER, FALSE);
        ActNoDlyFork(MK_ACT(INJECT_ID, 1, OS_DACHRG), MK_ACT(INJECT_ID, 1, OS_SUCK_SYNC), SKIP_STEP, ACT_MOV_PRE, SKIP_OPER, FALSE);
	}
    
	/**************************************/
	
	
	/*射退********************************/
	//射退后排气
	if(ChkSysCfg(CO3_BAKE) && GetSubDataByID(CLAMP1, MD_ID, GET_IND(PCLAMP_MOLDSET, FL_EXHAUSTMODE)) == 4)
		ActMovFork(MK_ACT(INJECT_ID, 1, OS_SUCK),MK_ACT(CLAMP_ID, 1, OS_EXHAST), ACT_MOV_END_PRE, TRUE);

	/*************************************/

	/*冷却********************************/
	/*冷却前开安全门*/
	if(ChkSysCfg(CO3_HYPD) && !ChkSysCfg(XF5_SAFEH2) && GetSubDataByID(SAFEDOOR1, MD_ID, GET_IND(PSAFEDOOR_MOLDSET, FL_PWRDOORCTRL)) == 1 && 
	  GetSubDataByID(SAFEDOOR1, MD_ID, GET_IND(PSAFEDOOR_MOLDSET, FL_PWRDOOROPEN))	== 0)
		ActSimpleFork(MK_ACT(NOZZLE_ID, 1, OS_COOL), MK_ACT(SAFEDOOR_ID, 1, OS_SDRO), STAGE_1_STEP, INVALIDE_EN, OM_SEMI, TRUE);

	//储能加压
	if(GetSubDataByID(INJECT1, MD_ID, GET_IND(PINJECT_MOLDSET, FL_INCACC)) == 1 && !ChkSysCfg(CO3_STEN))
		ActStepModeFork(MK_ACT(NOZZLE_ID, 1, OS_COOL), MK_ACT(INJSTOR_ID, 1, OS_INJSTO), STAGE_1_STEP, SKIP_OPER, FALSE);

	/*结束射出增压动作*/
	ActStopFork(MK_ACT(NOZZLE_ID, 1, OS_COOL), MK_ACT(INJSTOR_ID, 1, OS_INJSTO), ACT_MOV_END_PRE, SKIP_OPER);
	
	/********************************************/
	
	/*开模***************************************/
	/*开模前开安全门*/
	if(!ChkSysCfg(CO3_HYPD) && GetSubDataByID(SAFEDOOR1, MD_ID, GET_IND(PSAFEDOOR_MOLDSET, FL_PWRDOORCTRL)) == 1 && 
	  GetSubDataByID(SAFEDOOR1, MD_ID, GET_IND(PSAFEDOOR_MOLDSET, FL_PWRDOOROPEN))	== 0)
		ActSimpleFork(MK_ACT(CLAMP_ID, 1, OS_OPNM), MK_ACT(SAFEDOOR_ID, 1, OS_SDRO), SKIP_STEP, ACT_MOV_PRE, OM_SEMI, FALSE);

	//浮动缸
	if(ChkSysCfg(XF0_SYSACC)/* && CheckPI(GetSubDataByID(SYSACC1, PI_ID, GET_IND(PSYSACC_PI, I_SYS_ACC))) != PI_NOUSED*/)
		ActNoDlyFork(MK_ACT(CLAMP_ID, 1, OS_OPNM), MK_ACT(SYSACC_ID, 1, OS_SYSACC),  INIT1_STEP, INVALIDE_EN, INVALIDE_EN, TRUE);
	
	//开模前中子退
	ActForkClmpOpnBfCoreBwd(MK_ACT(CLAMP_ID, 1, OS_OPNM));
	if(ChkSysCfg(CO4_APUG))    //调模开模前中子退
		ActForkClmpOpnBfCoreBwd(MK_ACT(CLAMP_ID, 1, OS_AOPN));

	//开模前吹气
	num = GetMachineSubPartNum(AIRBLOW_ID);
	for(i = 0; i < num; i++)
	{
		tmp_val = GetSubDataByID(MK_PART(AIRBLOW_ID, i + 1), MD_ID, GET_IND(PAIRBLOW_MOLDSET, FL_BLAST));
		if(tmp_val < 2)
		{
			tmp_val = GetSubDataByID(MK_PART(AIRBLOW_ID, i + 1), MD_ID, GET_IND(PAIRBLOW_MOLDSET, TM_MOVINGPLATENACT));
			if(tmp_val > 0)
			{
				tmp_val = GetSubDataByID(MK_PART(CLAMP_ID, 1), MH_ID, GET_IND(PCLAMP_MACHSET, CLMP_METERCH));
				if(ChkSysCfg(CO3_MAIR))
					ActNoCmpFork(MK_ACT(CLAMP_ID, 1, OS_OPNM),  MK_ACT(AIRBLOW_ID, i + 1, OS_AIRBL), SKIP_STEP, SKIP_NSTEP, ACT_MOV_INIT, SKIP_ERR, SKIP_OPER, SKIP_RET, 0, tmp_val, FALSE, 0);
				else
					ActNoCmpFork(MK_ACT(CLAMP_ID, 1, OS_OPNM),  MK_ACT(AIRBLOW_ID, i + 1, OS_AIRBL), SKIP_STEP, SKIP_NSTEP, ACT_MOV_PRE, SKIP_ERR, OM_AUTO, SKIP_RET, 0, tmp_val, FALSE, 0);
			}
		}
	}

	//托模时吹气
	num = GetMachineSubPartNum(AIRBLOW_ID);
	for(i = 0; i < num; i++)
	{
		tmp_val = GetSubDataByID(MK_PART(AIRBLOW_ID, i + 1), MD_ID, GET_IND(PAIRBLOW_MOLDSET, FL_BLAST));
		if(tmp_val == 6)
		{
			tmp_val = GetSubDataByID(MK_PART(AIRBLOW_ID, i + 1), MD_ID, GET_IND(PAIRBLOW_MOLDSET, TM_MOVINGPLATENACT));
			if(tmp_val > 0)
			{
				tmp_val = GetSubDataByID(MK_PART(EJECT_ID, 1), MH_ID, GET_IND(PEJECT_MACHSET,EJT_METERCH));
				if(ChkSysCfg(CO3_MAIR))
					ActNoCmpFork(MK_ACT(EJECT_ID, 1, OS_EJTF),  MK_ACT(AIRBLOW_ID, i + 1, OS_AIRBL), SKIP_STEP, SKIP_NSTEP, ACT_MOV_PRE, SKIP_ERR, SKIP_OPER, SKIP_RET, 0, tmp_val, FALSE, 0);
				else
					ActNoCmpFork(MK_ACT(EJECT_ID, 1, OS_EJTF),  MK_ACT(AIRBLOW_ID, i + 1, OS_AIRBL), SKIP_STEP, SKIP_NSTEP, ACT_MOV_PRE, SKIP_ERR, OM_AUTO, SKIP_RET, 0, tmp_val, FALSE, 0);
			}
		}
	}

	//托模退时吹气，20210810
	num = GetMachineSubPartNum(AIRBLOW_ID);
	for(i = 0; i < num; i++)
	{
		tmp_val = GetSubDataByID(MK_PART(AIRBLOW_ID, i + 1), MD_ID, GET_IND(PAIRBLOW_MOLDSET, FL_BLAST));
		if(tmp_val == 8)
		{
			tmp_val = GetSubDataByID(MK_PART(AIRBLOW_ID, i + 1), MD_ID, GET_IND(PAIRBLOW_MOLDSET, TM_MOVINGPLATENACT));
			if(tmp_val > 0)
			{
				tmp_val = GetSubDataByID(MK_PART(EJECT_ID, 1), MH_ID, GET_IND(PEJECT_MACHSET,EJT_METERCH));
				if(ChkSysCfg(CO3_MAIR))
					ActNoCmpFork(MK_ACT(EJECT_ID, 1, OS_EJTB),  MK_ACT(AIRBLOW_ID, i + 1, OS_AIRBL), SKIP_STEP, SKIP_NSTEP, ACT_MOV_PRE, SKIP_ERR, SKIP_OPER, SKIP_RET, 0, tmp_val, FALSE, 0);
				else
					ActNoCmpFork(MK_ACT(EJECT_ID, 1, OS_EJTB),  MK_ACT(AIRBLOW_ID, i + 1, OS_AIRBL), SKIP_STEP, SKIP_NSTEP, ACT_MOV_PRE, SKIP_ERR, OM_AUTO, SKIP_RET, 0, tmp_val, FALSE, 0);
			}
		}
	}

	//开模联动/开模中中子
	ActForkClmpOpnMidCoreBwd(MK_ACT(CLAMP_ID, 1, OS_OPNM));
	if(ChkSysCfg(CO4_APUG))    //调模开模连动中子/途中中子
		ActForkClmpOpnMidCoreBwd(MK_ACT(CLAMP_ID, 1, OS_AOPN));

	//开模脱模联动
	if(GetJointActSel() == 1)
		ActForkClmpOpnMidEject();

	//开模后中子退
	ActForkClmpOpnEndCoreBwd(MK_ACT(CLAMP_ID, 1, OS_OPNM));
	if(ChkSysCfg(CO4_APUG))      //调模开模后中子退
		ActForkClmpOpnEndCoreBwd(MK_ACT(CLAMP_ID, 1, OS_AOPN));

	//开模后润滑
	if((GetSubDataByID(LUB1, MH_ID, GET_IND(PLUB_MACHSET, FL_LUBMODE))==0 && GetMachineSubPart(LUB_ID, 1) != NULL)  //润滑模式为0，不检测润滑检知
	||(GetSubDataByID(LUB1, MH_ID, GET_IND(PLUB_MACHSET, FL_LUBMODE))!=0 && GetMachineSubPart(LUB_ID, 1) != NULL && CheckPI(GetSubDataByID(LUB1, PI_ID, GET_IND(PLUB_PI, I_LUB_CHK))) != PI_NOUSED))
		ActMovFork(MK_ACT(CLAMP_ID, 1, OS_OPNM), MK_ACT(LUB_ID, 1, OS_LUBR), ACT_MOV_END_PRE, FALSE);
	if((GetSubDataByID(LUB2, MH_ID, GET_IND(PLUB_MACHSET, FL_LUBMODE))==0 && GetMachineSubPart(LUB_ID, 2) != NULL)
	||(GetSubDataByID(LUB2, MH_ID, GET_IND(PLUB_MACHSET, FL_LUBMODE))!=0&&GetMachineSubPart(LUB_ID, 2) != NULL &&  CheckPI(GetSubDataByID(LUB2, PI_ID, GET_IND(PLUB_PI, I_LUB_CHK))) != PI_NOUSED))
		ActMovFork(MK_ACT(CLAMP_ID, 1, OS_OPNM),  MK_ACT(LUB_ID, 2, OS_LUBR), ACT_MOV_END_PRE, FALSE);
	if((GetSubDataByID(LUB3, MH_ID, GET_IND(PLUB_MACHSET, FL_LUBMODE))==0 && GetMachineSubPart(LUB_ID, 3) != NULL)
	||(GetSubDataByID(LUB3, MH_ID, GET_IND(PLUB_MACHSET, FL_LUBMODE))!=0&&GetMachineSubPart(LUB_ID, 3) != NULL &&  CheckPI(GetSubDataByID(LUB3, PI_ID, GET_IND(PLUB_PI, I_LUB_CHK))) != PI_NOUSED))
		ActMovFork(MK_ACT(CLAMP_ID, 1, OS_OPNM),  MK_ACT(LUB_ID, 3, OS_LUBR), ACT_MOV_END_PRE, FALSE);

	//开模后开安全门
	if(GetSubDataByID(SAFEDOOR1, MD_ID, GET_IND(PSAFEDOOR_MOLDSET, FL_PWRDOORCTRL)) == 1 &&  
	  GetSubDataByID(SAFEDOOR1, MD_ID, GET_IND(PSAFEDOOR_MOLDSET, FL_PWRDOOROPEN))	== 1)
		ActSimpleFork(MK_ACT(CLAMP_ID, 1, OS_OPNM), MK_ACT(SAFEDOOR_ID, 1, OS_SDRO), INVALIDE_EN, ACT_MOV_END_PRE, OM_SEMI, ChkSysCfg(CO3_HYPD)&& !ChkSysCfg(XF5_SAFEH2));

	
	ActSimpleFork(MK_ACT(CLAMP_ID, 1, OS_OPNM), MK_ACT(AUTOCTRL_ID, 1, OS_SENSORCHK), STAGE_1_STEP, INVALIDE_EN, OM_SENR, FALSE);
	
	/*脱模**************************/
	/*******************************/
}

void CreateMainActForkTbl()
{
	UI16 tmp_val;
    PCLAMP pclmp = (PCLAMP)GetPartByID(MK_PART(CLAMP_ID, 1));

	ActEndNoDelyFork(MK_ACT(MACHINE_ID, 1, OS_RCYC), MK_ACT(CLAMP_ID, 1, OS_CLSM));	
	
	//关模结束转动作
	if(GetSysMold()->FL_INJMOLDIN && ChkSysCfg( CO2_INJCLS)&&(pclmp->md->FL_EXHAUSTMODE==7) || (GetSysMold()->FL_INJMOLDIN == 4))
		ActEndFork(MK_ACT(CLAMP_ID, 1, OS_CLSM),  MK_ACT(NOZZLE_ID, 1, OS_NOZF));
	else
		ActEndNoDelyFork(MK_ACT(CLAMP_ID, 1, OS_CLSM),  MK_ACT(CLAMP_ID, 1, OS_PRSM));
	
	ActEndModeNoDelyFork(MK_ACT(CLAMP_ID, 1, OS_CLSM),  MK_ACT(CLAMP_ID, 1, OS_PRSM), OM_MANUAL);

  	//关模异常
	if(!ChkSysCfg(CO2_CSTP))
	{
		ActErrAbortFork(MK_ACT(CLAMP_ID, 1, OS_CLSM),  MK_ACT(CLAMP_ID, 1, OS_OPNM), 1, OM_AUTO);
		ActErrAbortFork(MK_ACT(CLAMP_ID, 1, OS_PRSM),  MK_ACT(CLAMP_ID, 1, OS_OPNM), 1, OM_AUTO);
	}

	/*压模结束转动作*/
	ActEndFork(MK_ACT(CLAMP_ID, 1, OS_PRSM), MK_ACT(NOZZLE_ID, 1, OS_NOZF));

	/*座进结束转动作*/
	//tmp_val = GetSubDataByID(MK_PART(INJECT_ID, 1), MH_ID, GET_IND(PINJECT_MACHSET, TMDLY_INJBEFORE));
	//if(tmp_val == 0 || tmp_val >999)
	//	tmp_val = TASKSTNDDLY;
	//else
	//	tmp_val = tmp_val*10;
	
	if(ChkSysCfg(XF5_INJBCHR))
		//ActEndFork(MK_ACT(NOZZLE_ID, 1, OS_NOZF), MK_ACT(INJECT_ID, 1, OS_CHRG));
		ActNoCmpFork(MK_ACT(NOZZLE_ID, 1, OS_NOZF), MK_ACT(INJECT_ID, 1, OS_CHRG),  SKIP_STEP, SKIP_NSTEP, SKIP_MOV, SKIP_ERR, OM_AUTO, ACT_END, 0, 0, FALSE, 0);
	else
		//ActEndFork(MK_ACT(NOZZLE_ID, 1, OS_NOZF), MK_ACT(INJECT_ID, 1, OS_INJE));
		ActNoCmpFork(MK_ACT(NOZZLE_ID, 1, OS_NOZF), MK_ACT(INJECT_ID, 1, OS_INJE),  SKIP_STEP, SKIP_NSTEP, SKIP_MOV, SKIP_ERR, OM_AUTO, ACT_END, 0, 0, FALSE, 0);

	/*射出保压结束转动作*/
	tmp_val = GetSubDataByID(MK_PART(NOZZLE_ID, 1), MD_ID, GET_IND(PNOZZLE_MOLDSET, FL_NOZRET));
	if(ChkSysCfg(XF5_INJBCHR))
		ActEndFork(MK_ACT(INJECT_ID, 1, OS_INJE), MK_ACT(CLAMP_ID, 1, OS_OPNM));
	else
	{
		if(tmp_val == 3)
			ActEndFork(MK_ACT(INJECT_ID, 1, OS_INJE), MK_ACT(NOZZLE_ID, 1, OS_NOZB));
		else
			ActEndFork(MK_ACT(INJECT_ID, 1, OS_INJE), MK_ACT(INJECT_ID, 1, OS_CHRG));
	}
	
	//储料结束转动作
	if(ChkSysCfg(XF5_INJBCHR))
		ActEndErrFork(MK_ACT(INJECT_ID, 1, OS_CHRG), MK_ACT(INJECT_ID, 1, OS_INJE), 0);
	else
	{
		if(ChkSysCfg(CO1_OPDC) && GetSysMold()->FL_OPNDURINGCOOL==1)   //在使用储料连动开模时，上面已经调用储料开始转冷却，在这里不管射退选择哪个，都必须是执行射退
		{
			ActEndErrFork(MK_ACT(INJECT_ID, 1, OS_CHRG), MK_ACT(INJECT_ID, 1, OS_SUCK), 0);
		}
		else
		{
			if(GetSubDataByID(INJECT1, MD_ID, GET_IND(PINJECT_MOLDSET, FL_SUCKBACKMODE)) == 1)
				ActEndErrFork(MK_ACT(INJECT_ID, 1, OS_CHRG), MK_ACT(NOZZLE_ID, 1, OS_COOL), 0);
			else
				ActEndErrFork(MK_ACT(INJECT_ID, 1, OS_CHRG), MK_ACT(INJECT_ID, 1, OS_SUCK), 0);
		}
	}
	
	//储料未定时完成,不停在开模前
	if(!ChkSysCfg(CO2_HPEM))
	{
		if(GetSubDataByID(INJECT1, MD_ID, GET_IND(PINJECT_MOLDSET, FL_SUCKBACKMODE)) == 1)
			ActEndErrFork(MK_ACT(INJECT_ID, 1, OS_CHRG), MK_ACT(NOZZLE_ID, 1, OS_COOL), INTF_ERR2);
		else
			ActEndErrFork(MK_ACT(INJECT_ID, 1, OS_CHRG), MK_ACT(INJECT_ID, 1, OS_SUCK), INTF_ERR2);
	}


	//射退结束转动作，在开模储料联动时不转动作
	//if(GetJointActSel() != 6)
	if(!ChkSysCfg(CO1_OPDC) || GetSysMold()->FL_OPNDURINGCOOL!=1)
	{
		tmp_val= GetSubDataByID(NOZZLE1, MD_ID, GET_IND(PNOZZLE_MOLDSET, FL_NOZRET));
		if(GetSubDataByID(INJECT1, MD_ID, GET_IND(PINJECT_MOLDSET, FL_SUCKBACKMODE)) == 1)
		{
			if(tmp_val == 1 || tmp_val == 2)
				ActEndFork(MK_ACT(INJECT_ID, 1, OS_SUCK),MK_ACT(NOZZLE_ID, 1, OS_NOZB));
			else
				ActEndFork(MK_ACT(INJECT_ID, 1, OS_SUCK),MK_ACT(CLAMP_ID, 1, OS_OPNM));
		}
		else
		{
			if(tmp_val == 1)
				ActEndFork(MK_ACT(INJECT_ID, 1, OS_SUCK),MK_ACT(NOZZLE_ID, 1, OS_NOZB));
			else
				ActEndFork(MK_ACT(INJECT_ID, 1, OS_SUCK),MK_ACT(NOZZLE_ID, 1, OS_COOL));
		}
	}
/*	else       //在储料连动开模的情况下，增加储料，射退完成后，再执行座台退功能
	{
		tmp_val= GetSubDataByID(NOZZLE1, MD_ID, GET_IND(PNOZZLE_MOLDSET, FL_NOZRET));
		if(GetSubDataByID(INJECT1, MD_ID, GET_IND(PINJECT_MOLDSET, FL_SUCKBACKMODE)) == 1)
		{
			if(tmp_val == 1)
				ActEndFork(MK_ACT(INJECT_ID, 1, OS_SUCK),MK_ACT(NOZZLE_ID, 1, OS_NOZB));
		}
		else
		{
			if(tmp_val == 1)
				ActEndFork(MK_ACT(INJECT_ID, 1, OS_SUCK),MK_ACT(NOZZLE_ID, 1, OS_NOZB));
		}
	}*/

	/*冷却结束转动作*/
	if(ChkSysCfg(CO1_OPDC)&&(GetSysMold()->FL_OPNDURINGCOOL==1)) //&&(GetSysMold()->FL_OPNEJTCORE==6))
		ActEndFork(MK_ACT(NOZZLE_ID, 1, OS_COOL), 	MK_ACT(CLAMP_ID, 1, OS_OPNM));
	else
	{
		if(GetSubDataByID(INJECT1, MD_ID, GET_IND(PINJECT_MOLDSET, FL_SUCKBACKMODE)) == 1)
			ActEndFork(MK_ACT(NOZZLE_ID, 1, OS_COOL), MK_ACT(INJECT_ID, 1, OS_SUCK));
		else
		{
			if(GetSubDataByID(NOZZLE1, MD_ID, GET_IND(PNOZZLE_MOLDSET, FL_NOZRET)) == 2)
				ActEndFork(MK_ACT(NOZZLE_ID, 1, OS_COOL), MK_ACT(NOZZLE_ID, 1, OS_NOZB));
			else
				ActEndFork(MK_ACT(NOZZLE_ID, 1, OS_COOL), MK_ACT(CLAMP_ID, 1, OS_OPNM));
		}
	}

	/*座退结束转动作*/
	tmp_val = GetSubDataByID(MK_PART(NOZZLE_ID, 1), MD_ID, GET_IND(PNOZZLE_MOLDSET, FL_NOZRET));
	if(tmp_val == 1)
	{
		if(GetSubDataByID(INJECT1, MD_ID, GET_IND(PINJECT_MOLDSET, FL_SUCKBACKMODE)) == 1)
			ActEndFork(MK_ACT(NOZZLE_ID, 1, OS_NOZB), MK_ACT(CLAMP_ID, 1, OS_OPNM));
		else
			ActEndFork(MK_ACT(NOZZLE_ID, 1, OS_NOZB), MK_ACT(NOZZLE_ID, 1, OS_COOL));
	}
	else if(tmp_val == 2)
		ActEndFork(MK_ACT(NOZZLE_ID, 1, OS_NOZB), MK_ACT(CLAMP_ID, 1, OS_OPNM));
	else if(tmp_val == 3)
		ActEndFork(MK_ACT(NOZZLE_ID, 1, OS_NOZB), MK_ACT(INJECT_ID, 1, OS_CHRG));

	


	if(GetJointActSel() != 1)
		ActEndFork(MK_ACT(CLAMP_ID, 1, OS_OPNM), MK_ACT(EJECT_ID, 1, OS_EJT));

	ActEndFork(MK_ACT(EJECT_ID, 1, OS_EJT), MK_ACT(MACHINE_ID, 1, OS_PRCYC));
}


void SimpleRecycle()
{
    ActEndFork(MK_ACT(MACHINE_ID, 1, OS_RCYC), MK_ACT(CLAMP_ID, 1, OS_CLSM));
    ActEndFork(MK_ACT(CLAMP_ID, 1, OS_CLSM),  MK_ACT(CLAMP_ID, 1, OS_PRSM));
    ActEndModeFork(MK_ACT(CLAMP_ID, 1, OS_CLSM),  MK_ACT(CLAMP_ID, 1, OS_PRSM), OM_MANUAL);
    //关模异常
    if (!ChkSysCfg(CO2_CSTP))
    {
        ActNoCmpFork(MK_ACT(CLAMP_ID, 1, OS_CLSM), MK_ACT(CLAMP_ID, 1, OS_OPNM), SKIP_STEP, SKIP_NSTEP, SKIP_MOV, SKIP_ERR, OM_AUTO, ACT_ERR_ABORT, 40, 1, FALSE, 0);
        ActNoCmpFork(MK_ACT(CLAMP_ID, 1, OS_PRSM), MK_ACT(CLAMP_ID, 1, OS_OPNM), SKIP_STEP, SKIP_NSTEP, SKIP_MOV, SKIP_ERR, OM_AUTO, ACT_ERR_ABORT, 40, 1, FALSE, 0);
    }

	ActEndFork(MK_ACT(CLAMP_ID, 1, OS_PRSM),  MK_ACT(NOZZLE_ID, 1, OS_COOL));

    ActEndFork(MK_ACT(NOZZLE_ID, 1, OS_COOL), MK_ACT(CLAMP_ID, 1, OS_OPNM));
    ActEndFork(MK_ACT(CLAMP_ID, 1, OS_OPNM), MK_ACT(EJECT_ID, 1, OS_EJT));
	
	//开模后润滑
	if((GetSubDataByID(LUB1, MH_ID, GET_IND(PLUB_MACHSET, FL_LUBMODE))==0 && GetMachineSubPart(LUB_ID, 1) != NULL)  //润滑模式为0，不检测润滑检知
	||(GetSubDataByID(LUB1, MH_ID, GET_IND(PLUB_MACHSET, FL_LUBMODE))!=0 && GetMachineSubPart(LUB_ID, 1) != NULL && CheckPI(GetSubDataByID(LUB1, PI_ID, GET_IND(PLUB_PI, I_LUB_CHK))) != PI_NOUSED))
		ActMovFork(MK_ACT(CLAMP_ID, 1, OS_OPNM), MK_ACT(LUB_ID, 1, OS_LUBR), ACT_MOV_END_PRE, FALSE);
	if((GetSubDataByID(LUB2, MH_ID, GET_IND(PLUB_MACHSET, FL_LUBMODE))==0 && GetMachineSubPart(LUB_ID, 2) != NULL)
	||(GetSubDataByID(LUB2, MH_ID, GET_IND(PLUB_MACHSET, FL_LUBMODE))!=0&&GetMachineSubPart(LUB_ID, 2) != NULL &&  CheckPI(GetSubDataByID(LUB2, PI_ID, GET_IND(PLUB_PI, I_LUB_CHK))) != PI_NOUSED))
		ActMovFork(MK_ACT(CLAMP_ID, 1, OS_OPNM),  MK_ACT(LUB_ID, 2, OS_LUBR), ACT_MOV_END_PRE, FALSE);
	if((GetSubDataByID(LUB3, MH_ID, GET_IND(PLUB_MACHSET, FL_LUBMODE))==0 && GetMachineSubPart(LUB_ID, 3) != NULL)
	||(GetSubDataByID(LUB3, MH_ID, GET_IND(PLUB_MACHSET, FL_LUBMODE))!=0&&GetMachineSubPart(LUB_ID, 3) != NULL &&  CheckPI(GetSubDataByID(LUB3, PI_ID, GET_IND(PLUB_PI, I_LUB_CHK))) != PI_NOUSED))
		ActMovFork(MK_ACT(CLAMP_ID, 1, OS_OPNM),  MK_ACT(LUB_ID, 3, OS_LUBR), ACT_MOV_END_PRE, FALSE);

    ActEndFork(MK_ACT(EJECT_ID, 1, OS_EJT), MK_ACT(MACHINE_ID, 1, OS_PRCYC));

	ActNoDlyFork(MK_ACT(INJECT_ID, 1, OS_INJE), MK_ACT(INJECT_ID, 1, OS_NEEDLEVALVE),  INIT1_STEP, INVALIDE_EN, OM_MANUAL, FALSE);
	ActNoDlyFork(MK_ACT(INJECT_ID, 1, OS_AINJE), MK_ACT(INJECT_ID, 1, OS_NEEDLEVALVE),  INIT1_STEP, INVALIDE_EN, OM_ADJU, FALSE);
}


#if 1
void CreateActForkTbl(UI16 oper_mode)
{
	ActForkTblClear();

	if (GetSys()->md->FL_WET_DRY_CYCLES == 1)
	{//干循环
		SimpleRecycle();
	}
	else
	{//正常循环
		CreateMainActForkTbl();
		CreateSubActForkTbl();
	}
	
	if(GetJointActSel() != 0 && GetHydrTbl() == NULL)
		SetMachineErr(ER0_HYTBL);
	
	ActForkTblOutDate(FALSE);
	ActForkTblPrint();
}
#else
void CreateActForkTbl(UI16 oper_mode)
{
	SetOperMode(OM_AUTO);
	ActForkTblClear();
	
	ActMovFork(MK_ACT(CLAMP_ID, 1, OS_OPNM), MK_ACT(AIRBLOW_ID, 1, OS_AIRBL), ACT_MOV_PRE, TRUE);	
	ActMovFork(MK_ACT(CLAMP_ID, 1, OS_OPNM), MK_ACT(NOZZLE_ID, 1, OS_COOL), ACT_MOV_PRE, TRUE);	
	ActMovFork(MK_ACT(CLAMP_ID, 1, OS_OPNM), MK_ACT(AIRBLOW_ID, 2, OS_AIRBL), ACT_MOV_PRE, TRUE);	
	ActMovFork(MK_ACT(CLAMP_ID, 1, OS_OPNM), MK_ACT(AIRBLOW_ID, 3, OS_AIRBL), ACT_MOV_PRE, FALSE);	
	ActMovFork(MK_ACT(CLAMP_ID, 1, OS_OPNM), MK_ACT(AIRBLOW_ID, 4, OS_AIRBL), ACT_MOV_PRE, TRUE);	
	ActMovFork(MK_ACT(NOZZLE_ID, 1, OS_COOL), MK_ACT(CORE_ID, 1, OS_CPRB), ACT_MOV_PRE, TRUE);	
	ActMovFork(MK_ACT(NOZZLE_ID, 1, OS_COOL), MK_ACT(LUB_ID, 1, OS_LUBR), ACT_MOV_PRE, TRUE);	
	ActMovFork(MK_ACT(CORE_ID, 1, OS_CPRB), MK_ACT(LUB_ID, 2, OS_LUBR), ACT_MOV_PRE, FALSE);	
	ForkActTsk(GetActByID(MK_ACT(CLAMP_ID, 1, OS_OPNM)), FIRST_FORK, NULL, 0, 0, FALSE);

	ActForkTblPrint();
}
#endif
