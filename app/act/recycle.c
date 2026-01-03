/******************************************************************************
  文 件 名   : recycle.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 机器循环及前置再循环动作函数
  函数列表   :
              CycleEnd
              CycleStart
              CycleTimeErrChk
              CycleTmReset
              PreCycleProc
              RecycComplete
              RecycleProc
              RecycToClamp
              SemiRecycle
              SensorRecycle
              TimeRecycle
  修改历史   :
  1.日    期   : 2013年5月17日
    作    者   : ren chaohong
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "actcom.h"
#include "hwcfg.h"
#include "keycmd.h"
#include "machine.h"
#include "monmmi.h"
#include "machapi.h"
#include "AutoRunWithOutServo.h"

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
BOOL SensorDropPIChk(PAUTOCTRL pp);

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


void Step_AllToZero(PMACHINE pp)
{
	PNOZZLE	pnzl = (PNOZZLE)GetMachineSubPart(NOZZLE_ID, 1);
	if(pp!=NULL)
	{
		if(pp->pclmp != NULL && pp->pclmp->st != NULL)
		{
			pp->pclmp->st->TM_CLAMPCLS_RT = 0;
			pp->pclmp->st->TM_CLAMPOPN_RT = 0;
			pnzl->st->TM_NOZZLEF_RT = 0;
			pnzl->st->TM_NOZZLEB_RT = 0;
			pp->pejt->st->TM_EJECT_RT = 0;
		}
		
		if(pp->pinj!= NULL && pp->pinj->st != NULL)
		{
			pp->pinj->st->TM_INJ_RT_L= 0;
			pp->pinj->st->TM_HOLD_RT_L= 0;
			pp->pinj->st->TM_SUCKBK_RT= 0;
			pp->pinj->st->TM_CHARGE_RT= 0;

			if(pp->pinj->pnzl != NULL)
			{
				pp->pinj->pnzl->st->TM_COOL_RT_L = 0;
				pp->pinj->pnzl->st->TM_COOL_RT_H = 0;
			}
		}

		if(pp->pinj != NULL)
		{
			pp->pinj->st->SPEED_TOHOLD=0;
		}
	}
}

void CycleStart(PMACHINE pp)
{
	PNOZZLE pnzl = (PNOZZLE)GetMachineSubPart(NOZZLE_ID, 1);

	if(pp != NULL)
	{
		pp->st->TM_CYCLE_L = 0;
		pp->st->TM_CYCLE_H = 0;
		pp->bactrunning = TRUE;
		
		pp->cyclestart = GetMsTick();

		MovePartDataChangeCache();	//再循环结束时，将缓存内的数据搬运到设定dic中生效。
	}
}

void ReCycleStart(PMACHINE pp)
{
	if(pp != NULL)
	{
		//Step_AllToZero(pp);
		pp->st->TM_CYCLE_L = 0;
		pp->st->TM_CYCLE_H = 0;
		pp->bactrunning = FALSE;
		pp->Recyclestart = GetMsTick();
	}
}

BOOL CycleTimeErrChk(PMACHINE pp)//自动循环时间检测
{
	BOOL  cerr = FALSE;	
	UI32 tmp1,tmp2;
	
	tmp1 = ((UI32)pp->st->TM_CYCLE_MAX_H << 16) + (UI32)pp->st->TM_CYCLE_MAX_L;
	tmp2 = ((UI32)pp->md->TMCYCLEDEFAULTMAX_H << 16) + (UI32)pp->md->TMCYCLEDEFAULTMAX_L;	//警报参数画面中的上限时间
	
	if(ChkOperMode(OM_AUTO) && (pp->bactrunning || (ChkSysCfg(CO1_CYLW) && !ChkRunFlag(INIF_AUTO))))
	{
		if(AutoAlarmIsStart() && tmp1 > 0)	//自动警报开启 使用多次循环统计出的最大循环时间值
			cerr = (DIFF_MS(pp->cyclestart)> tmp1*10); 	
  		else if(tmp2 > 0) 
			cerr = (DIFF_MS(pp->cyclestart) > tmp2*10); //自动警报没开启使用缺省值 

		if(cerr)
		{
			SET_PART_ERR(pp, ER0_CYCL);
		}
	}
	
	return(cerr);
}

void CyclePDInfoSend(PMACHINE pp)
{
	UI32 tmp_tm1, tmp_tm2, tmp_tm3;
	
	if(pp != NULL)
	{
		pp->st->TM_CYCLE_L = 0;
		pp->st->TM_CYCLE_H = 0;
		
		tmp_tm1 = DIFF_MS(pp->Recyclestart)/10;
		tmp_tm2 = DIFF_MS(pp->cyclestart)/10;		
		tmp_tm3 = tmp_tm1 + (tmp_tm1 >>1);
		
		pp->st->TM_CYCLE_WHOLE_L = (UI16)(tmp_tm1 & 0x0ffff);	//全程计时低16位
		pp->st->TM_CYCLE_WHOLE_H = (UI16)((tmp_tm1 >> 16) & 0x0ffff);	//全程计时高16位
		
		pp->st->TM_CYCLE_LAST_L =  (UI16)(tmp_tm2 & 0x0ffff);		//制品时间低16位
		pp->st->TM_CYCLE_LAST_H =  (UI16)((tmp_tm2 >> 16) & 0x0ffff);	//制品时间高16位
				
		if(AutoAlarmCanUpdate())
		{
			pp->st->TM_CYCLE_MAX_L = (UI16)(tmp_tm3 & 0x0ffff);			//警报画面中的循环最大值低16位
			pp->st->TM_CYCLE_MAX_H = (UI16)((tmp_tm3 >> 16) & 0x0ffff);		//警报画面中的循环最大值高16位
		}

		MachineCmd(CMD_QC_PARA,0);
		SendMonCycleRunSt();
	}
}

ACT_RET CycleEnd(PMACHINE pp)
{
	int i;
	
	//SetRunFlag(INIF_CYCLE);
	
	if(pp == NULL)
		return ACT_ERR_ABORT;

	if(ChkOperMode(OM_AUTO) && GetMeterLenByCh(pp->pinj->mh->INJ_METERCH) > 0)			
	{
		if(AutoAlarmCanUpdate())
		{
	    	if(ChkSysCfg(CO2_ICUR))  		
				pp->pinj->st->ADPOSI_INJCUSHION_MAX =pp->pinj->pa->ADPOSI_INJ4END;
			else if(ChkSysCfg(CO2_ICURINJEND))
				pp->pinj->st->ADPOSI_INJCUSHION_MAX =pp->pinj->st->ADPOSI_INJHOLDEND;				
			else
				pp->pinj->st->ADPOSI_INJCUSHION_MAX =pp->pinj->st->ADPOSI_INJCUSHION;			
		}
	}					

	if (!AlarmIsOn() && CycleTimeErrChk(pp))
		return ACT_ERR_ABORT;

	if (ChkSysCfg(XF5_LUBEMRG))
	{
		for(i = 0; i < pp->lub_num; i++)
		{
			if(CHK_PART_ERR(pp->plub + i, (ER1_LUBR | ER1_LUBL)))
			{
				ResetSystem();
				AlarmOn();
				return ACT_ERR_ABORT;
			}
		}
	}

	return ACT_OK;
}

void CycleTmReset(PMACHINE pp)
{
	if(pp != NULL)
		pp->Recyclestart = 0;
}

BOOL ChkLubComplete(PLUB pp)
{
	if(pp==NULL||(CheckPI(pp->pi->I_LUB_CHK) == PI_NOUSED))
		return TRUE;
	else
		return pp->lubcmp;
}

BOOL RecycToClamp(PMACHINE pp)
{
  BOOL ret = FALSE,ret2 =FALSE ;	
  if(ChkSysCfg(CO5_LUBBFCLS) && ((pp->pclmp->plub[0]->alarmlubfirst && (pp->pclmp->plub[0]!=NULL))||(pp->pclmp->plub[1]->alarmlubfirst && (pp->pclmp->plub[1] != NULL))||(pp->pclmp->plub[2]->alarmlubfirst && (pp->pclmp->plub[2] != NULL)))&&ChkRunFlag(INIF_AUTO1))
  {
  	if((!ChkActRun(pp->pclmp->plub[0], 1))&&(!ChkActRun(pp->pclmp->plub[1], 1))&&(!ChkActRun(pp->pclmp->plub[2], 1)))
  	{
		if((CheckPI(pp->pclmp->plub[0]->pi->I_LUB_CHK) != PI_NOUSED)&&(pp->pclmp->plub[0]!=NULL)&&pp->pclmp->plub[0]->alarmlubfirst)
		{
			ForkAct(pp->pclmp->plub[0]->act+OS_LUBR-1, FIRST_FORK, NULL, 0, 0);
			pp->pclmp->plub[0]->st->AUTOLUB_NEED = TRUE;
			pp->pclmp->plub[0]->lubcmp = 0;
		}
		if((pp->pclmp->plub[1] != NULL) && (CheckPI(pp->pclmp->plub[1]->pi->I_LUB_CHK) != PI_NOUSED)&&pp->pclmp->plub[1]->alarmlubfirst)
		{
			ForkAct(pp->pclmp->plub[1]->act+OS_LUBR-1, FIRST_FORK, NULL, 0, 0);		
			pp->pclmp->plub[1]->lubcmp = 0;
			pp->pclmp->plub[1]->st->AUTOLUB_NEED = TRUE;
		}
		if((pp->pclmp->plub[2] != NULL) && (CheckPI(pp->pclmp->plub[2]->pi->I_LUB_CHK) != PI_NOUSED)&&pp->pclmp->plub[2]->alarmlubfirst)	
		{
			ForkAct(pp->pclmp->plub[2]->act+OS_LUBR-1, FIRST_FORK, NULL, 0, 0);
			pp->pclmp->plub[2]->lubcmp = 0;
			pp->pclmp->plub[2]->st->AUTOLUB_NEED = TRUE;
		}
  	}	
	if(ChkLubComplete(pp->pclmp->plub[0])&&ChkLubComplete(pp->pclmp->plub[1])&&ChkLubComplete(pp->pclmp->plub[2]))
		ret = TRUE;
  }
  else
    ret = TRUE;
  if(!RobotRecycleErrChk(pp->prbt))
  	ret2 = TRUE;
  return(ret&ret2);
}

ACT_RET  RecycComplete(PMACHINE pp)
{
	if(RecycToClamp(pp))
		return ACT_END;
	else
		return ACT_OK;
}


ACT_RET SensorDropIsOKPro(PMACHINE pp, PACT pact)
{
	PAUTOCTRL pautoctrl = (PAUTOCTRL)GetPartByID(MK_PART(AUTOCTRL_ID, 1));
	if(pautoctrl == NULL)
		return ACT_ERR_ABORT;

	if (pautoctrl->bdrop_ok == TRUE)
	{
		return ACT_OK;
	}
	
	if (pautoctrl->chk_st == FALSE)
	{
		pautoctrl->chk_st = TRUE;
		pautoctrl->chk_start_10ms = Get10MsTick();
	}
	else
	{
		if (DIFF_10MS(pautoctrl->chk_start_10ms) > pautoctrl->md->TM_SENSORCHK)
		{			
			if (ChkSysCfg(XF3_ADFRS) && !SensorDropPIChk(pautoctrl))
			{//电眼信号首次无效，再做一次脱模，再次检测电眼信号
				if (pautoctrl->chk_err_cnt == 0)
				{
					ForkAct(pp->pejt->act + OS_EJT- 1,SUSPEND_FORK, pact->ptsk, 0,0);
					pautoctrl->chk_err_cnt=1;
					pautoctrl->chk_start_10ms = Get10MsTick();//再次做脱模后，电眼检测时间重新计时
				}
				else
				{
					SET_PART_ERR(pp->pclmp->pactrl, ER0_DROP);
					pp->pclmp->pactrl->chk_err_cnt=0;
					return ACT_ERR_ABORT;
				}
			}
			else
			{
				EndActTsk(((PACT)(pautoctrl->act + OS_SENSORCHK - 1))->ptsk, TRUE);

				if (SensorDropPIChk(pautoctrl))				
					SET_PART_ERR(pautoctrl, ER0_DRFUL);
				else
					SET_PART_ERR(pautoctrl, ER0_DROP);
				return ACT_ERR_ABORT;
			}
		}		
	}

	return ACT_ERR;
}


ACT_RET SensorRecycle(PMACHINE pp, PACT pact)
{
	ACT_RET ret = ACT_OK;
	UI32 cyctm;
	UI16 i = 0;
	
	if(pp == NULL)
		return ACT_ERR_ABORT;
	switch (pact->step)
	{
		case INIT1_STEP:
			ACT_STEP_NEW(INIT1_STEP + 1);
			break;
		case INIT1_STEP + 1:
			if((ret = SensorDropIsOKPro(pp,pact)) == ACT_OK)
			{
			    pp->pclmp->pactrl->chk_err_cnt=0;
				ACT_STEP_NEW(INIT1_STEP + 2);
			}
			break;
			
		case INIT1_STEP + 2:
			for(i = 0; i < CLAMP_MAX_AIRBLOW; i++)
			{
				if(pp->pclmp->airblow[i] != NULL)
				{
					if(pp->pclmp->airblow[i]->md->FL_BLAST == 7)
						ForkAct(pp->pclmp->airblow[i]->act + OS_AIRBL - 1, SYNC_FORK, pact->ptsk, 0, 0);
				}
			}
			ACT_STEP_NEW(STAGE_1_STEP);
			break;
			
		case STAGE_1_STEP:
			ACT_START();
			cyctm = (((UI32)pp->md->TM_CYCLEPAUSE_H) << 16) + ((UI32)pp->md->TM_CYCLEPAUSE_L);
			ActSetTM(pact->ptsk, cyctm);
			ACT_STEP_NEW(STAGE_1_STEP + 1);
			break;
		case STAGE_1_STEP + 1:
			cyctm = (((UI32)pp->md->TM_CYCLEPAUSE_H) << 16) + ((UI32)pp->md->TM_CYCLEPAUSE_L);
			if((UI16)(ACT_RUN_MS/10) >= cyctm)
				ACT_STEP_NEW(END_STEP);
			break;
			
		case END_STEP:
			ret = RecycComplete(pp);
			break;
		default:
			ret = ACT_END;
			break;  	
	}
	return ret;
}


/*该功能函数主要用于在半自动状态直接切换到全自动或电眼自动时，屏蔽掉开关
安全门动作，由于程序采用的是在半自动运行状态中直接跳走结束的方式，
所以当切换后的第一次全自动依旧是走半自动的函数的(主要还是因为有首模标记才导致的)*/
ACT_RET SemiRecycleBack(PMACHINE pp, PACT pact)
{
	PSAFEDOOR psfdr = (PSAFEDOOR)GetPartByID(MK_PART(SAFEDOOR_ID, 1));
	if(ChkSysCfg(CO2_UPCV))
	{
		if((ChkOperMode(OM_TIME) || ChkOperMode(OM_SENR)) && (psfdr->SemiTurnTime == 1))   //从半自动切换到全自动或者电眼自动情况下才执行
		{
			if (!ChkRunFlag(INIF_AUTO1))
			{
				CLR_PART_ERR(pp->psfdr,  ER0_CSDR);
				CLR_PART_ERR(pp->psfdr,  ER0_OSDR);
				psfdr->SemiTurnTime = 0;
				ACT_STEP_NEW(STAGE_1_STEP + 3);
			}
		}
	}		
	return ACT_OK;
}

//extern BOOL clampdoorclose;

/*任务调度动作，直接在动作中启动其他动作*/
ACT_RET SemiRecycle(PMACHINE pp, PACT pact)
{
	ACT_RET ret = ACT_OK;
	PSAFEDOORCHK pchk;
	PSAFEDOOR ph = (PSAFEDOOR)GetPartByID(MK_PART(SAFEDOOR_ID, 1));
	PMACHINE pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1));
	UI32 cyctm;
	
	if(pp == NULL)
		return ACT_ERR_ABORT;

	pchk = GetSafeDorrChkState(pp->psfdr);
	switch (pact->step)
	{
		case INIT1_STEP:
			if(ChkRunFlag(INIF_AUTO)&&ChkSysCfg(CO5_ADJMOLD)) //0x0800  松模问题的自动调模		2014.02.20
	 			AdjMoldInit(FALSE);		// 松模初始化

			ACT_STEP_NEW(STAGE_1_STEP);
			break;
		case INIT1_STEP + 1:
			ACT_STEP_NEW(INIT1_STEP);
			break;
		case STAGE_1_STEP:
			if(pchk == NULL)
			{
				ACT_STEP_NEW(STAGE_1_STEP + 4);
				break;
			}

			if(!FSafeDoorIsOpen(pp->psfdr))       //若为真，安全门关着的状态
			{
				if(ph->clampdoorclose==1)
				{
					ph->clampdoorclose=0;
				    ACT_STEP_NEW(STAGE_1_STEP + 2);
				}   
				else
				{
					if(pchk->dooropn && GET_PART_TYPE(pchk->chkoper) == EJECT_ID)
					{
						pchk->chkoper = 0;
						pchk->dooropn = 0;
						ACT_STEP_NEW(STAGE_1_STEP + 1);
					//如果codde 5中打上了CO5_DORMEC 且脱模时开关安全门没有引起pchk->doormecsaf为真
					//则再循环时需重新开关安全门，如果已经引起pchk->doormecsaf为真，则不需要再开关安全门

						if(ChkSysCfg(CO5_DORMEC))
						{
			          		if (pchk->doormecsaf)
								ACT_STEP_NEW(STAGE_1_STEP + 2); 
						}
						else
							ACT_STEP_NEW(STAGE_1_STEP + 2); //在code 5中没有CO5_DORMEC时，如果脱模时已经开关过安全门了，再循环就不需要再次开关安全门了
	        		}
	        		else	
	        		{
						SET_PART_ERR(pp->psfdr,  ER0_OSDR);
						if (ChkSysCfg(CO3_SDAM))
							StandAlarm(TRUE);

						pchk->chkcnt=0;
					}
				}
			}  
			else
			{
				if(pchk->dooropn && GET_PART_TYPE(pchk->chkoper) == EJECT_ID)  //代表安全门是在脱模时打开的
					ACT_STEP_NEW(STAGE_1_STEP + 2);  //在脱模部分已经判断过安全门打开着的了，不需要再判断一遍
				
				pchk->chkoper = 0;
        		pchk->dooropn = 0;
				ACT_STEP_NEW(STAGE_1_STEP + 1);
			}	  
      		break;

		case STAGE_1_STEP + 1:
			if (FSafeDoorIsOpen(pp->psfdr))    //若为真，代表安全门开着
			{
				pchk->chkcnt ++;
				if (pchk->chkcnt>=pchk->chkcntset)
				{
					CLR_PART_ERR(pp->psfdr, ER0_OSDR);

					ACT_STEP_NEW(STAGE_1_STEP + 2);
					pchk->chkcnt  = 0;
				}  

				if(ClampMecSafeChk(pp->pclmp) != PI_SET)
					pchk->doormecsaf = TRUE;	 
			}
			else 
			{
				SET_PART_ERR(pp->psfdr,  ER0_OSDR);

				pchk->chkcnt  = 0;
				pchk->chkoper = 0;
			}  
			break; 
		case STAGE_1_STEP + 2:
			SET_PART_ERR(pp->psfdr,  ER0_CSDR);
			ACT_STEP_NEW(STAGE_1_STEP + 3);
			break; 

		case STAGE_1_STEP + 3:
      		if(FSafeDoorIsClose(pp->psfdr))         // if (testpb(I_SDR_CLS))
			{
				CLR_PART_ERR(pp->psfdr,  ER0_CSDR);
				ACT_STEP_NEW(STAGE_1_STEP + 5);
				if(ChkSysCfg(CO5_DORMEC) && ClampMecSafeChk(pp->pclmp) != PI_NOUSED)
					ACT_STEP_NEW(STAGE_1_STEP + 4);
			}
			else 
            {
                if(!CHK_PART_ERR(pp->psfdr, ER0_CSDR)) //避免SemiRecycleBack跳转时，安全门未关但是没有提示
                    SET_PART_ERR(pp->psfdr, ER0_CSDR);
                    
                if(ClampMecSafeChk(pp->pclmp) != PI_SET)
				    pchk->doormecsaf = TRUE;
            }
			break;

		case STAGE_1_STEP + 4:
			if (pchk->doormecsaf && ClampMecSafeChk(pp->pclmp) == PI_SET)
			{
				CLR_PART_ERR(pp->pclmp,  ER0_MESA);
				pchk->doormecsaf = FALSE;
				ACT_STEP_NEW(STAGE_1_STEP + 5);
			}
			else
			{    	
				if((UI16)(Get100MsTick() - pact->step_100ms) >= 30)
				{
					SetOperMode(OM_MANUAL);
					ManualInitErr();

					if(CUS_ID == CS_GUANGSHU)
					SET_PART_ERR(pp->pclmp,  ER0_MESA);
				}

				if(CUS_ID != CS_GUANGSHU)
					SET_PART_ERR(pp->pclmp,  ER0_MESA);

				if (ClampMecSafeChk(pp->pclmp) != PI_SET)
    				pchk->doormecsaf = TRUE;	 
			}
			break;  	
			
		case STAGE_1_STEP + 5:
			ACT_STEP_NEW(STAGE_1_STEP + 6);
			ACT_START();
			
			if(pp->pejt != NULL && EjectGetMode(pp->pejt) == 0)
			{
				if(pp->pejt->md->FL_EJTSPECIAL == 1)
					SetEjtFwdOff(pp->pejt, GetPartActBySN((PPART)pp->pejt, OS_EJTF));
				
				ForkAct(GetPartActBySN((PPART)pp->pejt, OS_EJTB), SUSPEND_FORK, pact->ptsk, 0x02, TASKSTNDDLY);
			}
			break;
			
		case STAGE_1_STEP + 6:
			if(ChkSysCfg(CO2_MIDT) ||EjectGetMode(pp->pejt) ==0 || (ChkOperMode(OM_TIME)&&ChkSysCfg(CO2_UPCV)&&!ChkRunFlag(INIF_AUTO1)))
			{
				cyctm = (((UI32)pp->md->TM_CYCLEPAUSE_H) << 16) + ((UI32)pp->md->TM_CYCLEPAUSE_L);
				ActSetTM(pact->ptsk, cyctm);
				if((ACT_RUN_MS/10) >= cyctm)
					ACT_STEP_NEW(STAGE_1_STEP + 7);
			}  
      		else  
				ACT_STEP_NEW(STAGE_1_STEP + 7);
			break;

		case STAGE_1_STEP + 7:
			if(ChkSysCfg(CO4_CKEY) && (ChkRunFlag(INIF_AUTO1)|| ChkSysCfg(CO4_ISOF) || (CUS_ID ==CS_TONGYONG)))
			{
				ClearRunKey();
				SET_PART_ERR(pp, ER0_SKEY);
				ACT_STEP_NEW(STAGE_1_STEP + 8);
			} 	
			else if(CheckPIUsed(pm->pi->I_E_PO16))
			{
			    SET_PART_ERR(pp, ER0_SKEY);
				ACT_STEP_NEW(STAGE_1_STEP + 9);
			}
			else if(!ChkOperMode(OM_SEMI)&&CheckPIUsed(pm->pi->I_E_PO15))
			{
			    SET_PART_ERR(pp, ER0_SKEY);
				ACT_STEP_NEW(STAGE_1_STEP + 10);
			}
			else
				ACT_STEP_NEW(END_STEP);
			break;
		case STAGE_1_STEP + 8:
			if(GetRunKey())
			{
				CLR_PART_ERR(pp, ER0_SKEY);
				ClearRunKey();
				ACT_STEP_NEW(END_STEP);
			}
			break;
		case STAGE_1_STEP + 9:
			if(CheckPI(pm->pi->I_E_PO16) == PI_RESET) //按键必须发生跳变才有效，避免客户直接短接
			{
				ACT_STEP_NEW(STAGE_1_STEP + 11);
			}
			break;
		case STAGE_1_STEP + 10:
			if(CheckPI(pm->pi->I_E_PO15) == PI_RESET) //按键必须发生跳变才有效，避免客户直接短接
			{
				ACT_STEP_NEW(STAGE_1_STEP + 12);
			}
			break;
            
        case STAGE_1_STEP + 11:
			if(CheckPI(pm->pi->I_E_PO16) == PI_SET)
			{
				CLR_PART_ERR(pp, ER0_SKEY);
				ACT_STEP_NEW(END_STEP);
			}
			break;
		case STAGE_1_STEP + 12:
			if(CheckPI(pm->pi->I_E_PO15) == PI_SET)
			{
				CLR_PART_ERR(pp, ER0_SKEY);
				ACT_STEP_NEW(END_STEP);
			}
			break;

		case END_STEP:
			ClrRunFlag(INIF_AUTO);
			ret = RecycComplete(pp);
			break;
		default:
			ret = ACT_END;
			break;  	
	}
	
	if(ret == ACT_END || ret == ACT_ERR_ABORT)
		return ret;
	else
  		return SemiRecycleBack(pp, pact);
}  

ACT_RET TimeRecycle(PMACHINE pp, PACT pact)
{
	ACT_RET ret = ACT_OK;
    PROBOT prbt = (PROBOT)GetPartByID(MK_PART(ROBOT_ID, 1));
	UI32 cyctm;

	if(pp == NULL)
		return ACT_ERR_ABORT;

	switch (pact->step)
	{
		case INIT1_STEP:
			ACT_STEP_NEW(STAGE_1_STEP);
			break;
			
		case STAGE_1_STEP:
			ACT_START();
			cyctm = (((UI32)pp->md->TM_CYCLEPAUSE_H) << 16) + ((UI32)pp->md->TM_CYCLEPAUSE_L);
			ActSetTM(pact->ptsk, cyctm);
			ACT_STEP_NEW(STAGE_1_STEP + 1);
			break;
		case STAGE_1_STEP + 1:
			cyctm = (((UI32)pp->md->TM_CYCLEPAUSE_H) << 16) + ((UI32)pp->md->TM_CYCLEPAUSE_L);
			if((ACT_RUN_MS/10) >= cyctm)
			{
                if(!FSafeDoorIsCloseErrChk(pp->psfdr))//再循环新增检查安全门，防止全自动托模停留，开安全门拿产品被压到手
                {
					if(pp->pejt != NULL && EjectGetMode(pp->pejt) == 0 && (ChkSysCfg(XF3_SPSTO)
						|| ((pp->prbt->md->FL_ROBOTCTRL>0)&&(!RobotClampClsErrChk(pp->prbt)))))
					{
						if(pp->pejt->md->FL_EJTSPECIAL == 1)
							SetEjtFwdOff(pp->pejt, GetPartActBySN((PPART)pp->pejt, OS_EJTF));
					
						if(ChkSysCfg(XF4_INJSV))
    					{
    						if(prbt->md->FL_ROBOTCTRL>0)
    						{
    							if(TestPI(prbt->pi->I_ROBOT))
    								ForkAct(GetPartActBySN((PPART)pp->pejt, OS_EJTB), SUSPEND_FORK, pact->ptsk, 0, TASKSTNDDLY);
    						}
    						else
    						{
    							SET_PART_ERR(prbt, ER0_NOROBOT);
    							return ACT_ERR_ABORT;
    						}
    					}
    					else
    						ForkAct(GetPartActBySN((PPART)pp->pejt, OS_EJTB), SUSPEND_FORK, pact->ptsk, 0, TASKSTNDDLY);
					}
					ACT_STEP_NEW(END_STEP);
                }
			}
			break;
			
		case END_STEP:
			ret = RecycComplete(pp);
			break;
		default:
			ret = ACT_END;
			break;  	
	}
	return ret;
}

ACT_RET PreCycleProc(PACT pact, UI32 para)
{
	PMACHINE pp = (PMACHINE)(pact->part);
    PINJECT pinj = (PINJECT)GetPartByID(MK_PART(INJECT_ID,1));
	ACT_RET ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;
	
	switch (pact->step)
	{
		case INIT1_STEP:
			pp->st->Step_Precycle = 0;
//			if(!ChkSysCfg(CO1_SOEC) || pp->md->FL_OPNEJTCORE !=1 || !ChkActRun(pp->pclmp, ACT_SN_BIT(OS_OPNM)))
			if(!ChkSysCfg(CO1_SOEC) || pp->md->FL_OPNEJTCORE !=1 || !ActIsInTsk(MK_ACT(CLAMP_ID, 1, OS_OPNM)))
			{
				if(pp->PAYMENT_TIMEOUT_FLAG==1)
				{
					if (MotorIsOn())	
	      		        MotorDown();
					SetOperMode(OM_MANUAL);
					ManualInit();
					pp->PAYMENT_TIMEOUT_FLAG=0;
				}
				
				if(EjectWaitOpenCoreBwd(pp->pejt))
				{
					ACT_STEP_NEW(INIT1_STEP + 1);
				}
			}
			break; 
		case INIT1_STEP + 1:
			ClampOpenCount(pp->pclmp);
			if(ShotReached(pp->pclmp))
			{
	/*
				need to do, send monitor MAX data to MMI
				setmonitorlast();
				setmonitormax();	
				sendmonimaxtommi();
	*/			
			}
			
			ACT_STEP_NEW(INIT1_STEP + 2);
		break;

		case INIT1_STEP + 2:
			if(STEP_10MS_CHK(1))
			{
			    ACT_STEP_NEW(INIT1_STEP + 3);
			}
		break;
/*        QC监控参数                 2014-07-09*/	
		case INIT1_STEP + 3:

			CyclePDInfoSend(pp);
			if((pp->md->Qc_Fun == 1)&&(pp->st->Qc_Opn_Cnt<=pp->md->Qc_OpnNum))
				GetQcPara();
			if((pp->md->Qc_Fun == 1)&&(pp->st->Qc_Opn_Cnt==pp->md->Qc_OpnNum))
			{
				pp->qcparasend = Get100MsTick();
			    SET_PART_ERR(pp, ER0_QCPARACHK);				
				ACT_STEP_NEW(INIT1_STEP + 4);				
				PartDataTx((PPART)(pp->pclmp), ST_ID,CAL_LEN(*(pp->pclmp->st)),0,TRUE);
				PartDataTx((PPART)(pp->pejt), ST_ID,CAL_LEN(*(pp->pejt->st)),0,TRUE);
				PartDataTx((PPART)pp, ST_ID,CAL_LEN(*(pp->st)),0,TRUE);
				PartDataTx((PPART)(pp->pinj), ST_ID,CAL_LEN(*(pp->pinj->st)),0,TRUE);				
			}
			else
			ACT_STEP_NEW(END_STEP);
		break;
		case INIT1_STEP + 4:
			if((UI16)(Get100MsTick() - pp->qcparasend)>2)
			{
				SET_PART_ERR(pp, ER0_QCPARACHK);		
				ACT_STEP_NEW(END_STEP);	
			}
		break;
		
	case END_STEP:
		if((pp->md->Qc_Fun == 1)&&(pp->st->Qc_Opn_Cnt>pp->md->Qc_OpnNum)&&(pp->st->Qc_Opn_Cnt-pp->md->Qc_OpnNum>=5))
		{
			CLR_PART_ERR(pp, ER0_QCPARACHK);
		}
		if (!AlarmIsOn())
		{
		    if(ChkSysCfg(CO0_BMC))
			{
				if(TempAlarmIsOn())
				{
					TempAlarmOff();
					
					if(CHK_PART_ERR(pinj,ER0_CHGL))
						CLR_PART_ERR(pinj,ER0_CHGL);

					if(CHK_PART_ERR(pinj->pchgprs,ER0_CHGPRSBWD))
						CLR_PART_ERR(pinj->pchgprs,ER0_CHGPRSBWD);
				}
			}
            
 			ret = CycleEnd(pp);
			ComFailIsPost();
			if(ret != ACT_ERR_ABORT)
			{
				if(!ComIsFail())
				{
					if(ChkSysCfg(XF3_CPRSP))
					{
						/*解决面板修改了影响动作链表参数只有切换到手动时才有效的问题*/
						/*有从面板接收到模具或者机器参数，重新生成动作列表*/
						ActForkTblRefresh(TRUE);
					}
					ForkAct(GetPartActBySN((PPART)pp, OS_RCYC), FIRST_FORK, NULL, 0, 0);
				}
				else
					ComStateReset();
			}
			
			ret = ACT_END;
		}  
		else
		{
			//当放生警报输出时，在再循环时切手动
			SetOperMode(OM_MANUAL);
			ret = ACT_END;			
		}
		break;
		
	default:
		ret = ACT_END;
		break;
	}

	if(ret == ACT_END)
	{
		ReCycleStart(pp);
	}
	return ret;
}

ACT_RET RecycleProc(PACT pact, UI32 para)
{
	PMACHINE pp = (PMACHINE)(pact->part);
	PSAFEDOOR psfdr = (PSAFEDOOR)GetPartByID(MK_PART(SAFEDOOR_ID, 1));
	ACT_RET ret;
	
	if(pp == NULL)
		return ACT_ERR_ABORT;

	if(pact->step == INIT1_STEP)
		pp->bactrunning = FALSE;
		
	//if(((ChkRunFlag(INIF_AUTO) || ChkOperMode(OM_SEMI)) && (ChkSysCfg(CO2_UPCV) == 0))
	//	|| ((ChkSysCfg(CO2_UPCV) == 1) && ((ChkRunFlag(INIF_AUTO) && (psfdr->SemiTurnTime == 1)) || ChkOperMode(OM_SEMI))))
	if(ChkRunFlag(INIF_AUTO) || ChkOperMode(OM_SEMI))
	{
		ret = SemiRecycle(pp, pact);
	}
	else if(ChkOperMode(OM_TIME))
		ret = TimeRecycle(pp, pact);
	else if(ChkOperMode(OM_SENR))
		ret = SensorRecycle(pp, pact);
	else
		return ACT_END;
#if 0	//need to do	
  else if (*(pdbctrl+WOPERMODE) & OM_SENR)
    sensorrecycle();
#endif  

	if(ret == ACT_END)
	{
		CycleStart(pp);
		
		//注塑机动作开始
	}
	
	return ret;
}



