/******************************************************************************
  文 件 名   : clamp.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 模座部件及提供外部接口函数
  函数列表   :
  修改历史   :
  1.日    期   : 2013年5月17日
    作    者   : ren chaohong
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "actcom.h"
#include "airblow.h"
#include "safedoor.h"
#include "robot.h"
#include "eject.h"
#include "core.h"
#include "autoctrl.h"
#include "clamp.h"
#include "machapi.h"
#include "machine.h"
#include "HydrPos.h"
#include "EtherCATProc.h"


/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
extern	 MACHINE m_machine;

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
 void InitOilTempAi(PCLAMP pp);
 void InitClampTmProject(PCLAMP pp);

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
BOOL ChkLocateDa(PCLAMP pp) //开关模平稳使用比例阀
{
    if(pp != NULL)
    {
        if(pp->md->FL_CLSOPEN_MODE == 0 && pp->md->FL_LOCATE_DA == 1)
            return TRUE;
    }

    return FALSE;
}

BOOL  ClampsafeLight(PCLAMP pp)//安全光幕检查
{
	if(pp != NULL)	
	{
		if((CheckPIUsed(pp->pi->I_CLP_CHK))&&!TestPI(pp->pi->I_CLP_CHK))
		{
			if((UI16)(GetMsTick() - pp->I_CLP_CHK_LAST_TIME) > 50)
			{
				SET_PART_ERR(pp, ER0_LGHTSAF);
				StandAlarm(TRUE);
				return TRUE;	
			}
		}
		else
		{
			pp->I_CLP_CHK_LAST_TIME = GetMsTick();
		}
		
	}
	return FALSE;
}


ACT_RET ClampClsPreChk_CBAC()
{
	PCLAMP pp = (PCLAMP)GetMachineSubPart(CLAMP_ID, 1);

	//液压油检知
	if(OilLvErr() || OilFltErr() || OilTmpErr() || WaterTmpErr())
		return ACT_ERR_ABORT;

	//气压检知
	if(CheckPI(pp->pi->I_AIR_PRES) == PI_SET)
  	{
		SET_PART_ERR(pp, ER0_AIRPRES);
		return ACT_ERR_ABORT;
	}

	//托模未到定位
	if(ClampEjectBwdErrChk(pp))
		return ACT_ERR_ABORT;

	//上顶盖检知
	if((CheckPI(pp->pi->I_CLS_UPGA) == PI_RESET))
	{
		SET_PART_ERR(pp, ER0_UPGAERR);
		return ACT_ERR_ABORT;
	}
	
	return ACT_OK;
}

void InitClampRamp(PCLAMP pp)
{
	//启动
	UI16 start,Acc;
	//中间
	UI16 mid;
	//结束
	UI16 end;

	UI16 rampbuf;
	
	if(pp->md->FL_CLSE_SMOOTH == 1)
	{
		Acc = ((UI32)pp->md->SLOPE_MAX_CLS_SPD*(UI32)pp->md->CLS_SPD_STARTON/100);
		Acc = Acc < 2 ? 2: Acc;		
		start = 100000/Acc;
		
		Acc = ((UI32)pp->md->SLOPE_MAX_CLS_SPD*(UI32)pp->md->CLS_SPD_HALFWAY/100);
		Acc = Acc < 2 ? 2: Acc;	
		mid = 100000/Acc;
		
		Acc = ((UI32)pp->md->SLOPE_MAX_CLS_SPD*(UI32)pp->md->CLS_SPD_STOPOFF/100);
		Acc = Acc < 2 ? 2: Acc;
		end = 100000/Acc;

		pp->spdramp_cls[CLP_RAMP_CLS_START] = start;
		pp->spdramp_cls[CLP_RAMP_CLS_FAST] = mid;
		pp->spdramp_cls[CLP_RAMP_CLS_LOW] = mid;
		pp->spdramp_cls[CLP_RAMP_CLS_HIGH1] = pp->mh->DAFLOW_CLSRAMPHIPRES*10;
		pp->spdramp_cls[CLP_RAMP_CLS_HIGH2] = pp->mh->DAFLOW_CLSRAMPHIPRES2*10;
		pp->spdramp_cls[CLP_RAMP_CLS_END] = end;	
			

		Acc = ((UI32)pp->md->SLOPE_MAX_CLS_PRES*(UI32)pp->md->CLS_PRES_STARTON/100);
		Acc = Acc < 2 ? 2: Acc;		
		start = GetSysPresLimit()*100/Acc;
		
		Acc = ((UI32)pp->md->SLOPE_MAX_CLS_PRES*(UI32)pp->md->CLS_PRES_HALFWAY/100);
		Acc = Acc < 2 ? 2: Acc;	
		mid = GetSysPresLimit()*100/Acc;
		
		Acc = ((UI32)pp->md->SLOPE_MAX_CLS_PRES*(UI32)pp->md->CLS_PRES_STOPOFF/100);
		Acc = Acc < 2 ? 2: Acc;
		end = GetSysPresLimit()*100/Acc;

		pp->pressramp_cls[CLP_RAMP_CLS_START] = start;
		pp->pressramp_cls[CLP_RAMP_CLS_FAST] = mid;
		pp->pressramp_cls[CLP_RAMP_CLS_LOW] = mid;
		pp->pressramp_cls[CLP_RAMP_CLS_HIGH1] = pp->mh->DAPRES_CLSRAMPHIPRES*10;
		pp->pressramp_cls[CLP_RAMP_CLS_HIGH2] = pp->mh->DAPRES_CLSRAMPHIPRES2*10;
		pp->pressramp_cls[CLP_RAMP_CLS_END] = end;
		

		Acc = ((UI32)pp->md->SLOPE_MAX_CLS_SPD*(UI32)pp->md->FLOWSTART_CLOSE_MID/100);
		Acc = Acc < 2 ? 2: Acc;		
		start = 100000/Acc;
		
		Acc = ((UI32)pp->md->SLOPE_MAX_CLS_SPD*(UI32)pp->md->FLOWSTOP_CLOSE_MID/100);
		Acc = Acc < 2 ? 2: Acc;
		end = 100000/Acc;
		
		pp->spdramp_cls_manual[CLP_RAMP_CLS_MANUALSTART] = start;
		pp->spdramp_cls_manual[CLP_RAMP_CLS_MANUALEND] = end;
		
	}
	else
	{		
		pp->spdramp_cls[CLP_RAMP_CLS_START] = pp->mh->DAFLOW_CLSRAMP*10;
		pp->spdramp_cls[CLP_RAMP_CLS_FAST] = pp->mh->DAFLOW_CLSRAMP2*10;
		pp->spdramp_cls[CLP_RAMP_CLS_LOW] = pp->mh->DAFLOW_CLSRAMPLOWPRES*10;
		pp->spdramp_cls[CLP_RAMP_CLS_HIGH1] = pp->mh->DAFLOW_CLSRAMPHIPRES*10;
		pp->spdramp_cls[CLP_RAMP_CLS_HIGH2] = pp->mh->DAFLOW_CLSRAMPHIPRES2*10;
		pp->spdramp_cls[CLP_RAMP_CLS_END] = pp->mh->DAFLOW_CLSOFFRAMP*10;

		pp->pressramp_cls[CLP_RAMP_CLS_START] = pp->mh->DAPRES_CLSRAMP*10;
		pp->pressramp_cls[CLP_RAMP_CLS_FAST] = pp->mh->DAPRES_CLSRAMP2*10;
		pp->pressramp_cls[CLP_RAMP_CLS_LOW] = pp->mh->DAPRES_CLSRAMPLOWPRES*10;
		pp->pressramp_cls[CLP_RAMP_CLS_HIGH1] = pp->mh->DAPRES_CLSRAMPHIPRES*10;
		pp->pressramp_cls[CLP_RAMP_CLS_HIGH2] = pp->mh->DAPRES_CLSRAMPHIPRES2*10;
		pp->pressramp_cls[CLP_RAMP_CLS_END] = pp->mh->DAPRES_CLSOFFRAMP*10;
        
        pp->spdramp_cls_manual[CLP_RAMP_CLS_MANUALSTART] = pp->spdramp_cls[CLP_RAMP_CLS_START];
		pp->spdramp_cls_manual[CLP_RAMP_CLS_MANUALEND] = pp->mh->DAPFLOW_CLSMIDEND_RAMPOFF*10;
	}

	if(pp->md->FL_CLSE_SMOOTH == 1)
	{
		
		if (pp->md->FL_CLSOPEN_MODE == 3 || pp->md->FL_CLSOPEN_MODE == 4)
		{		
			pp->AiClampRamp = AiClampRowDataGet(pp->pa->ADPOSI_OPENENDEND_MODE,pp->pa->DAFLOW_OPENEND*10);
			db_printf("AiClampRamp Rsults : Pos = %d, Speed = %d, Ramp = %d\r\n", pp->pa->ADPOSI_OPENENDEND,pp->pa->DAFLOW_OPENEND,pp->AiClampRamp);	
			
			if (pp->AiClampRamp == 0 || pp->AiClampRamp > 100)
			{
				rampbuf = pp->md->OPN_SPD_STOPOFF;
			}
			else
			{
				rampbuf = pp->AiClampRamp;
			}	

			pp->st->AI_CLAMP_OPEN_RAMP = rampbuf;
			PartDataTx((PPART)pp, ST_ID, CAL_LEN(pp->st->AI_CLAMP_OPEN_RAMP), GET_IND(PCLAMP_STATE, AI_CLAMP_OPEN_RAMP), TRUE);
		}
		else
		{
			rampbuf = pp->md->OPN_SPD_STOPOFF;
		}
		
		Acc = ((UI32)pp->md->SLOPE_MAX_OPN_SPD*(UI32)pp->md->OPN_SPD_STARTON/100);
		Acc = Acc < 2 ? 2: Acc;		
		start = 100000/Acc;
		
		Acc = ((UI32)pp->md->SLOPE_MAX_OPN_SPD*(UI32)pp->md->OPN_SPD_HALFWAY/100);
		Acc = Acc < 2 ? 2: Acc;	
		mid = 100000/Acc;
		
		Acc = ((UI32)pp->md->SLOPE_MAX_OPN_SPD*(UI32)rampbuf/100);
		Acc = Acc < 2 ? 2: Acc;
		end = 100000/Acc;

		pp->spdramp_opn[CLP_RAMP_OPN_START] = start;
		pp->spdramp_opn[CLP_RAMP_OPN_FAST1] = mid;
		pp->spdramp_opn[CLP_RAMP_OPN_FAST2] = mid;
		pp->spdramp_opn[CLP_RAMP_OPN_FAST3] = mid;
		pp->spdramp_opn[CLP_RAMP_OPN_LOW] = mid;
		pp->spdramp_opn[CLP_RAMP_OPN_END] = end;	

		Acc = ((UI32)pp->md->SLOPE_MAX_OPN_PRES*(UI32)pp->md->OPN_PRES_STARTON/100);
		Acc = Acc < 2 ? 2: Acc;		
		start = GetSysPresLimit()*100/Acc;
		
		Acc = ((UI32)pp->md->SLOPE_MAX_OPN_PRES*(UI32)pp->md->OPN_PRES_HALFWAY/100);
		Acc = Acc < 2 ? 2: Acc;	
		mid = GetSysPresLimit()*100/Acc;
		
		Acc = ((UI32)pp->md->SLOPE_MAX_OPN_PRES*(UI32)pp->md->OPN_PRES_STOPOFF/100);
		Acc = Acc < 2 ? 2: Acc;
		end = GetSysPresLimit()*100/Acc;

		pp->pressramp_opn[CLP_RAMP_OPN_START] = start;
		pp->pressramp_opn[CLP_RAMP_OPN_FAST1] = mid;
		pp->pressramp_opn[CLP_RAMP_OPN_FAST2] = mid;
		pp->pressramp_opn[CLP_RAMP_OPN_FAST3] = mid;
		pp->pressramp_opn[CLP_RAMP_OPN_LOW] = mid;
		pp->pressramp_opn[CLP_RAMP_OPN_END] = end;

		Acc = ((UI32)pp->md->SLOPE_MAX_OPN_SPD*(UI32)pp->md->FLOWSTART_OPEN_MID/100);
		Acc = Acc < 2 ? 2: Acc; 	
		start = 100000/Acc;
		
		Acc = ((UI32)pp->md->SLOPE_MAX_OPN_SPD*(UI32)pp->md->FLOWSTOP_OPEN_MID/100);
		Acc = Acc < 2 ? 2: Acc;
		end = 100000/Acc;
		
		pp->spdramp_opn_manual[CLP_RAMP_OPN_MANUALSTART] = start;
		pp->spdramp_opn_manual[CLP_RAMP_OPN_MANUALEND] = end;
	}
	else
	{	
		pp->spdramp_opn[CLP_RAMP_OPN_START] = pp->mh->DAFLOW_OPNRAMP*10;
		pp->spdramp_opn[CLP_RAMP_OPN_FAST1] = pp->mh->DAFLOW_OPENFASTRAMP*10;
		pp->spdramp_opn[CLP_RAMP_OPN_FAST2] = pp->mh->DAFLOW_OPENFASTRAMP1*10;
		pp->spdramp_opn[CLP_RAMP_OPN_FAST3] = pp->mh->DAFLOW_OPENFASTRAMP2*10;
		pp->spdramp_opn[CLP_RAMP_OPN_LOW] = pp->mh->DAFLOW_OPNRAMP2*10;
		pp->spdramp_opn[CLP_RAMP_OPN_END] = pp->mh->DAFLOW_OPNOFFRAMP*10;

		pp->pressramp_opn[CLP_RAMP_OPN_START] = pp->mh->DAPRES_OPNRAMP*10;
		pp->pressramp_opn[CLP_RAMP_OPN_FAST1] = pp->mh->DAPRES_OPENFASTRAMP*10;
		pp->pressramp_opn[CLP_RAMP_OPN_FAST2] = pp->mh->DAPRES_OPENFASTRAMP1*10;
		pp->pressramp_opn[CLP_RAMP_OPN_FAST3] = pp->mh->DAPRES_OPENFASTRAMP2*10;
		pp->pressramp_opn[CLP_RAMP_OPN_LOW] = pp->mh->DAPRES_OPNRAMP2*10;
		pp->pressramp_opn[CLP_RAMP_OPN_END] = pp->mh->DAPRES_OPNOFFRAMP*10;
        
        pp->spdramp_opn_manual[CLP_RAMP_OPN_MANUALSTART] =  pp->mh->DAFLOW_OPNRAMP*10;
		pp->spdramp_opn_manual[CLP_RAMP_OPN_MANUALEND] = pp->mh->DAPFLOW_OPNMIDEND_RAMPOFF*10;
	}
}

void InitClampRampPro()
{
	PCLAMP pp = (PCLAMP)GetMachineSubPart(CLAMP_ID, 1);
	InitClampRamp(pp);
}

BOOL ClampOpenReachErrChk(PCLAMP pp)
{
	PADMETER padm = (PADMETER)GetPartByID(MK_PART(ADMETER_ID, 1));
	int tmp;

	if(pp != NULL)
	{
		if(padm != NULL)
		{
			if(pp->mh->ADPOSI_EFFCPR <= 0 || pp->mh->ADPOSI_EFFCPR > padm->st->ADPOSI_METERMAX[1])   //开模位置有效区超限做保护，默认为50
				pp->mh->ADPOSI_EFFCPR = 500;
			if(pp->mh->ADPOSI_ALLOWEJTUPLIMIT <= 0 || pp->mh->ADPOSI_ALLOWEJTUPLIMIT > padm->st->ADPOSI_METERMAX[1])   //开模位置有效区超限做保护，默认为50
				pp->mh->ADPOSI_ALLOWEJTUPLIMIT = 500;
			if(pp->mh->ADPOSI_ALLOWEJTDOWNLIMIT <= 0 || pp->mh->ADPOSI_ALLOWEJTDOWNLIMIT > padm->st->ADPOSI_METERMAX[1])   //开模位置有效区超限做保护，默认为50
				pp->mh->ADPOSI_ALLOWEJTDOWNLIMIT = 500;
		}
		tmp = GetMeterLenByCh(pp->mh->CLMP_METERCH);
		if ((tmp > 0 && GetMeterPosByCh(pp->mh->CLMP_METERCH)>(pp->pa->ADPOSI_OPENENDEND+pp->mh->ADPOSI_ALLOWEJTUPLIMIT)) ||
			(tmp > 0 && GetMeterPosByCh(pp->mh->CLMP_METERCH)<(pp->pa->ADPOSI_OPENENDEND-pp->mh->ADPOSI_ALLOWEJTDOWNLIMIT)) ||
		   (tmp <= 0 && !TestPI(pp->pi->I_OPN_END)))
		{
			SET_PART_ERR(pp, ER0_OPNM);
			return TRUE;
		}
	}

	return FALSE;		
}

void ClampClsVlvSet(PCLAMP pp, BOOL bset)
{
	if(pp != NULL)
	{
		if(bset)
		{
			SetPOBit(pp->po->O_CLS_SOL, PC_ON);
			SetPOBit(pp->po->O_CLS_SOL2, PC_ON);
		}
		else
		{
			SetPOBit(pp->po->O_CLS_SOL, PC_OFF);
			SetPOBit(pp->po->O_CLS_SOL2, PC_OFF);
		}
	}
}

void ClampChargeOff(PCLAMP pp)
{
//	if(pp != NULL)
//	{
//		SetPOBit(pp->po->O_CLS_DH2, PC_OFF); 
//		SetPOBit(pp->po->O_CLS_KEP, PC_OFF); 
//	}
}

BOOL ClampCoreStateErrChk(PCLAMP pp, BOOL  bfwd)
{
	int	i;

	if(pp != NULL)
	{
		for(i = 0; i < CLAMP_MAX_CORE; i++)
		{
			if(pp->pcore[i] != NULL && CoreStateErrChk(pp->pcore[i], bfwd))
				return TRUE;
		}
	}

	return FALSE;
}

BOOL ClampEjectBwdErrChk(PCLAMP pp)
{
	int i;

	if(pp != NULL)
	{
		for(i = 0; i < CLAMP_MAX_EJECT; i++)
		{
			if(pp->pejt[i] != NULL && EjectBwdErrChk(pp->pejt[i], TRUE))
				return TRUE;
		}
	}

	return FALSE;
}

BOOL ClampClsEjectBwdErrChk(PCLAMP pp)
{
	int i;

	if(pp != NULL)
	{
		for(i = 0; i < CLAMP_MAX_EJECT; i++)
		{
			if(pp->pejt[i] != NULL && EjectBwdErrChk(pp->pejt[i], FALSE)&&!ChkSysCfg(XF0_EJTTM))
				return TRUE;
		}
	}

	return FALSE;
}

BOOL ClampEjectBwdIsErr(PCLAMP pp)
{
	int i;

	if(pp != NULL)
	{
		for(i = 0; i < CLAMP_MAX_EJECT; i++)
		{
			if(pp->pejt[i] != NULL && EjectBwdErrChk(pp->pejt[i], FALSE))
				return TRUE;
		}
	}

	return FALSE;
}

BOOL ClampEjectFwdIsErr(PCLAMP pp)
{
	int i;

	if(pp != NULL)
	{
		for(i = 0; i < CLAMP_MAX_EJECT; i++)
		{
			if(pp->pejt[i] != NULL && EjectFwdErrChk(pp->pejt[i], FALSE))
				return TRUE;
		}
	}

	return FALSE;
}

void ClampEjectFwdStart(PCLAMP pp)
{
	if(pp != NULL)
	{
		if(ChkSysCfg(CO1_SOEC) && GetSysMold()->FL_OPNEJTCORE == 1)
		{}
		else
			pp->st->ADPOSI_OPNEND = GetMeterPosByCh(pp->mh->CLMP_METERCH);
	}
}

BOOL ClampEjectActEnable(PCLAMP pp)
{
	int i;

	if(pp != NULL)
	{
		for(i = 0; i < CLAMP_MAX_EJECT; i++)
		{
			if(pp->pejt[i] != NULL && pp->pejt[i]->md->CN_EJT > 0)
				return TRUE;
		}
	}

	return FALSE;
}

BOOL ClampSensorErrChk(PCLAMP pp)
{
	if(pp != NULL && pp->pactrl != NULL)
	{
		return SensorErrChk(pp->pactrl);
	}

	return FALSE;
}

void ClampCoreFwdPOSet(PCLAMP pp, BOOL bset)
{
	int	i;

	if(pp != NULL)
	{
		for(i = 0; i < CLAMP_MAX_CORE; i++)
		{
			if(pp->pcore[i] != NULL && pp->pcore[i]->md->FL_INJ_FWDOFF != 1)
				CoreFwdPOSet(pp->pcore[i], bset);
		}
	}
}

void ClampCoreFwdOff(PCLAMP pp)
{
	int	i;

	if(pp != NULL)
	{
		for(i = 0; i < CLAMP_MAX_CORE; i++)
		{
			if(pp->pcore[i] != NULL)
				CoreFwdOff(pp->pcore[i]);
		}
	}
}
 
BOOL ClampTempErrChk(PCLAMP pp)
{
	if(pp != NULL)
	{
		if(CheckPI(pp->pi->I_MOLDHEAT) == PI_SET)
		{
			SET_PART_ERR(pp, ER0_MDHEAT);
			return TRUE;
		}
	}

	return(FALSE);
}

BOOL ClampClsEndErrChk(PCLAMP pp)
{
	if(pp != NULL)
	{
		//if(!TestPI(pp->pi->I_CLS_END))
		if(!ChkAdFwdInplace(pp->mh->CLMP_METERCH))
		//if(!MoveLocationMeterControlProc(20, GetMeterPosByCh(pp->mh->CLMP_METERCH)))	
		{
			SET_PART_ERR(pp, ER0_CLSM);
			return TRUE;
		}
	}

	return FALSE;
}

void ClampOvlSolOff(PCLAMP pp)
{
//	if(pp != NULL)
//		SetPOBit(pp->po->O_OVR_SOL, PC_OFF);
}

/*排气动作的调度任务*/
ACT_RET ExhaustTaskProc(PACT pact, UI32 para)
{
	PCLAMP pp = (PCLAMP)pact->part;
	ACT_RET ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;

	switch (pact->step) 
	{
		case INIT1_STEP:
			ACT_STEP_NEW(STAGE_1_STEP);
			break;
		case STAGE_1_STEP:
			ACT_STEP_NEW(STAGE_1_STEP + 1);
			break;
		case STAGE_1_STEP + 1:
			if(pp->md->FL_EXHAUSTMODE==0||(pp->md->FL_EXHAUSTMODE==7))
			{
				if(((para == EXHAST_CNT_1)&&(STEP_10MS_CHK(pp->md->TMDLY_BAKEBEFOREEXHAUST)))||
					((para == EXHAST_CNT_2)&&(STEP_10MS_CHK(pp->md->TMDLY_BAKEBEFOREEXHAUST2)))||
					((para == EXHAST_CNT_3)&&(STEP_10MS_CHK(pp->md->TMDLY_BAKEBEFOREEXHAUST3)))||
					((para == EXHAST_CNT_4)&&(STEP_10MS_CHK(pp->md->TMDLY_BAKEBEFOREEXHAUST4))))
					ACT_STEP_NEW(STAGE_2_STEP);
			}
			else
			{
				if(STEP_10MS_CHK(pp->md->TMDLY_BAKEBEFOREEXHAUST))
					ACT_STEP_NEW(STAGE_2_STEP);
			}
      			break;
		case STAGE_2_STEP:
			ACT_STEP_NEW(STAGE_2_STEP + 1);
			ForkAct(pp->act + OS_EOPN - 1, SUSPEND_FORK, pact->ptsk, para, 0);
			break;
		case STAGE_2_STEP + 1:
			if(pp->md->FL_EXHAUSTMODE==0||(pp->md->FL_EXHAUSTMODE==7))
			{
				if(((para == EXHAST_CNT_1)&&(STEP_10MS_CHK(pp->md->TMDLY_BAKEAFTEREXHAUST)))||
					((para == EXHAST_CNT_2)&&(STEP_10MS_CHK(pp->md->TMDLY_BAKEAFTEREXHAUST2)))||
					((para == EXHAST_CNT_3)&&(STEP_10MS_CHK(pp->md->TMDLY_BAKEAFTEREXHAUST3)))||
					((para == EXHAST_CNT_4)&&(STEP_10MS_CHK(pp->md->TMDLY_BAKEAFTEREXHAUST4))))
					ACT_STEP_NEW(STAGE_3_STEP);
			}
			else
			{
				if(STEP_10MS_CHK(pp->md->TMDLY_BAKEAFTEREXHAUST))
				{
					ACT_STEP_NEW(STAGE_3_STEP);
				}
			}
			break;
		case STAGE_3_STEP:
			ForkAct(pp->act + OS_EPRS - 1, SUSPEND_FORK, pact->ptsk, 0, 0);
			ACT_STEP_NEW(END_STEP);
			break;
		case END_STEP:
            ret = ACT_END;
            break;
		default:
			ret = ACT_END;
			break;
	}

	return ret;
}      

void ClampSetup(PCLAMP pp)
{
	int i;

	if(pp != NULL)
	{
		for(i = 0; i < CLAMP_MAX_AIRBLOW; i++)
			pp->airblow[i] = (PAIRBLOW)GetMachineSubPart(AIRBLOW_ID, i + 1);

		for(i = 0; i < CLAMP_MAX_EJECT; i++)
			pp->pejt[i] = (PEJECT)GetMachineSubPart(EJECT_ID, i + 1);
				
		for(i = 0; i < CLAMP_MAX_CORE; i++)
			pp->pcore[i] = (PCORE)GetMachineSubPart(CORE_ID, i + 1);

		for(i = 0; i < CLAMP_MAX_LUB; i++)
			pp->plub[i] = (PLUB)GetMachineSubPart(LUB_ID, i + 1);

		pp->psfdr = (PSAFEDOOR)GetMachineSubPart(SAFEDOOR_ID, GET_PART_SN(pp->part.id));
		pp->prbt = (PROBOT)GetMachineSubPart(ROBOT_ID, GET_PART_SN(pp->part.id));
		pp->pactrl = (PAUTOCTRL)GetMachineSubPart(AUTOCTRL_ID, GET_PART_SN(pp->part.id));
		pp->padj= (PADJUST)GetMachineSubPart(ADJUST_ID, GET_PART_SN(pp->part.id));
		pp->LubCoolCnt = 0;
		pp->pinj = (PINJECT)GetMachineSubPart(INJECT_ID, GET_PART_SN(pp->part.id));
		pp->pnzl = (PNOZZLE)GetMachineSubPart(NOZZLE_ID, GET_PART_SN(pp->part.id));
		
		if(pp->last_chk_opennum == 0)
            pp->last_chk_opennum = ((UI32)(pp->st->CN_MOLDOPNNUM0) << 16) + pp->st->CN_MOLDOPNNUM1;
	}
}


ACT_RET ClampOpenProc(PACT pact, UI32 para)
{
	PCLAMP pp = (PCLAMP)(pact->part);
	ACT_RET ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;
	
	if(pp->md->FL_CLSOPEN_MODE == 2) 
	{
		ret = ClampOpen_pid(pact, para);
	}
	else if (pp->md->FL_CLSOPEN_MODE == 4)
	{
		ret = ClampOpen_p(pact, para);
	}
	else
		ret = ClampOpen(pact, para);

	return ret;
}

ACT_RET ClampCloseProc(PACT pact, UI32 para)
{
	PCLAMP pp = (PCLAMP)(pact->part);
	ACT_RET ret = ACT_OK;
	
	if(pp == NULL)
		return ACT_ERR_ABORT;

 	if(pp->md->FL_CLSOPEN_MODE == 2)
	{
		ret = Clampclose_pid(pact, para);
	}
	else
		ret = ClampClose(pact, para);
		
	return ret;
}


/*开合模转换效率自学习*/
void SetClampOpenSlow_RF(PCLAMP pp, PACT pact)
{
	UI16 hydelay;
	
	hydelay = 0;
	
	SetPOBitDelay(pp->po->O_OPN_SOL, PC_ON, 0);
	
	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, 10,5,0,0,0,0,
					HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_CLAMPOPN),pp->hy->HYDR_PUMPS_OPNSETUP,pp->hy->HYDR_PUMPS_OPNSETUP,
					pp->hy->SERVO_KP_OPN,pp->hy->SERVO_KI_OPN, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPOPN), pp->md->CLPOPN_SPD_UTI); 
}

void SetClampCls20_RF(PCLAMP pp, PACT pact)
{
	UI16 hydelay;
	
	hydelay = pp->mh->CLMPCLS_HYDRDELAY;
	
	SetPOBitDelay(pp->po->O_CLS_SOL, PC_ON, pp->mh->TMDLY_VLV_ONCLS);
	
	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, 80,20,0,0,0,0,
					HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_CLAMPCLS),pp->hy->HYDR_PUMPS_OPNSETUP,pp->hy->HYDR_PUMPS_OPNSETUP,
					pp->hy->SERVO_KP_OPN,pp->hy->SERVO_KI_OPN, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS), pp->md->CLPCLS_SPD_UTI); 
}

void SetClampCls80_RF(PCLAMP pp, PACT pact)
{
	UI16 hydelay;
	
	hydelay = pp->mh->CLMPCLS_HYDRDELAY;
	
	SetPOBitDelay(pp->po->O_CLS_SOL, PC_ON, pp->mh->TMDLY_VLV_ONCLS);
	
	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, 80,80,0,0,0,0,
					HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_CLAMPCLS),pp->hy->HYDR_PUMPS_OPNSETUP,pp->hy->HYDR_PUMPS_OPNSETUP,
					pp->hy->SERVO_KP_OPN,pp->hy->SERVO_KI_OPN, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS),pp->md->CLPCLS_SPD_UTI); 
}

void SetClampOpen20_RF(PCLAMP pp, PACT pact)
{
	UI16 hydelay;
	if(pp->mh->CLMPOPN_HYDRDELAY > 9999)
		pp->mh->CLMPOPN_HYDRDELAY = 0;
	
	hydelay = pp->mh->CLMPOPN_HYDRDELAY;
	
	SetPOBitDelay(pp->po->O_OPN_SOL, PC_ON, pp->mh->TMDLY_VLV_ONOPN);
	
	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, 80,20,0,0,0,0,
					HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_CLAMPOPN),pp->hy->HYDR_PUMPS_OPNSETUP,pp->hy->HYDR_PUMPS_OPNSETUP,
					pp->hy->SERVO_KP_OPN,pp->hy->SERVO_KI_OPN, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPOPN), pp->md->CLPOPN_SPD_UTI); 
}

void SetClampOpen80_RF(PCLAMP pp, PACT pact)
{
	UI16 hydelay;
	if(pp->mh->CLMPOPN_HYDRDELAY > 9999)
		pp->mh->CLMPOPN_HYDRDELAY = 0;
	
	hydelay = pp->mh->CLMPOPN_HYDRDELAY;
	
	SetPOBitDelay(pp->po->O_OPN_SOL, PC_ON, pp->mh->TMDLY_VLV_ONOPN);
	
	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, 100,80,0,0,0,0,
					HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_CLAMPOPN),pp->hy->HYDR_PUMPS_OPNSETUP,pp->hy->HYDR_PUMPS_OPNSETUP,
					pp->hy->SERVO_KP_OPN,pp->hy->SERVO_KI_OPN, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPOPN), pp->md->CLPOPN_SPD_UTI); 
}


void ClampOpenOffRF(PCLAMP pp, PACT pact)
{
	if(pp != NULL && pact != NULL)
	{		
		HYDR_CLS(0, 0);
		SetPOBitDelay(pp->po->O_OPN_SOL, PC_OFF, 5);
	}
}

void ClampClsOffRF(PCLAMP pp, PACT pact)
{
	if(pp != NULL && pact != NULL)
	{		
		HYDR_CLS(0, 0);
		SetPOBitDelay(pp->po->O_CLS_SOL, PC_OFF, 5);
	}
}

void ClampRFOffRF(PCLAMP pp, PACT pact)
{
	if(pp != NULL && pact != NULL)
	{		
		HYDR_CLS(0, 0);
		SetPOBitDelay(pp->po->O_CLS_SOL, PC_OFF, 5);
		SetPOBitDelay(pp->po->O_OPN_SOL, PC_OFF, 5);
	}
}


ACT_RET ClampRF(PACT pact, UI32 para)
{
	PCLAMP pp = (PCLAMP)(pact->part);
	ACT_RET ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;
	
	pp->mh->CLMP_METERCH = DEF_CLAMP_METER_CH;
	
	switch (pact->step)						  
	{
		case INIT1_STEP:
			pp->spd_motor = 0;
			pp->spd_oil = 0;					
			pp->spd_num = 0;	
			pp->rf_num = 0;
			pp->rf_lastpos = GetMeterPosByCh(pp->mh->CLMP_METERCH);
			ACT_STEP_NEW(STAGE_1_STEP);
			break;
		case INIT2_STEP:
			break;
		case STAGE_1_STEP:
			pp->rf_posstop_starttm = Get100MsTick();
		case STAGE_1_STEP+1:
			//慢速开模到最大行程
			if(INT_ABS(GetMeterPosByCh(pp->mh->CLMP_METERCH) - pp->rf_lastpos) > 10)
			{
				pp->rf_posstop_starttm = Get100MsTick();
			}

			if(DIFF_100MS(pp->rf_posstop_starttm) > 50)
			{
				pp->rf_modelen_max = pp->rf_lastpos;//记录最大开模行程
				pp->rf_modelen_20 = pp->rf_modelen_max*2/10;
				pp->rf_modelen_50 = pp->rf_modelen_max*5/10;
				pp->rf_modelen_80 = pp->rf_modelen_max*8/10;
				ACT_STEP_NEW(STAGE_2_STEP);
			}

			pp->rf_lastpos = GetMeterPosByCh(pp->mh->CLMP_METERCH);
			CHK_SET_MOV(SetClampOpenSlow_RF);
			break;
		case STAGE_2_STEP:
			//到最大行程后停止
			SET_OFF(ClampOpenOffRF);
			ACT_STEP_NEW(STAGE_3_STEP);
			break;
		case STAGE_3_STEP:
			if(ACT_STEP_100MS < 20)
				break;
		case STAGE_3_STEP+1:	
			//低速关模
			if(MoveLocationMeterControlProc(pp->rf_modelen_20, GetMeterPosByCh(pp->mh->CLMP_METERCH)))
			{
				pp->rf_K[0] = pp->spd_oil*100/pp->spd_motor + pp->rf_K[0];
				pp->rf_K_num[0]++;

				pp->spd_motor = 0;
				pp->spd_oil = 0;						
				pp->spd_num = 0;
				
				ACT_STEP_NEW(STAGE_4_STEP);
				break;
			}
			
			if(MoveLocationMeterControlProc(pp->rf_modelen_50, GetMeterPosByCh(pp->mh->CLMP_METERCH)))
			{
				pp->spd_motor = pp->spd_motor + GetRPMByServoNo(0);
				pp->spd_oil = pp->spd_oil + pp->st->CLAMP_SPEED;
				pp->spd_num++;	
			}

			CHK_SET_MOV(SetClampCls20_RF);			
			break;
		case STAGE_4_STEP:
		case STAGE_4_STEP+1:
			SET_OFF(ClampClsOffRF);
			ACT_STEP_NEW(STAGE_5_STEP);
			break;
		case STAGE_5_STEP:
			if(ACT_STEP_100MS < 20)
				break;
		case STAGE_5_STEP+1:	
			//低速开模
			if(MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->rf_modelen_80))
			{
				pp->rf_K[1] = pp->spd_oil*100/pp->spd_motor + pp->rf_K[1];
				pp->rf_K_num[1]++;

				pp->spd_motor = 0;
				pp->spd_oil = 0;						
				pp->spd_num = 0;
				
				ACT_STEP_NEW(STAGE_6_STEP);
				break;
			}

			if(MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->rf_modelen_50))
			{
				pp->spd_motor = pp->spd_motor + GetRPMByServoNo(0);
				pp->spd_oil = pp->spd_oil + pp->st->CLAMP_SPEED;
				pp->spd_num++;	
			}

			CHK_SET_MOV(SetClampOpen20_RF);			
			break;
		case STAGE_6_STEP:
		case STAGE_6_STEP+1:
			SET_OFF(ClampOpenOffRF);
			ACT_STEP_NEW(STAGE_7_STEP);
			break;
		case STAGE_7_STEP:
			if(ACT_STEP_100MS < 20)
				break;
		case STAGE_7_STEP+1:	
			//高速关模
			if(MoveLocationMeterControlProc(pp->rf_modelen_20, GetMeterPosByCh(pp->mh->CLMP_METERCH)))
			{
				pp->rf_K[2] = pp->spd_oil*100/pp->spd_motor + pp->rf_K[2];
				pp->rf_K_num[2]++;

				pp->spd_motor = 0;
				pp->spd_oil = 0;						
				pp->spd_num = 0;
				
				ACT_STEP_NEW(STAGE_8_STEP);
				break;
			}

			if(MoveLocationMeterControlProc(pp->rf_modelen_50, GetMeterPosByCh(pp->mh->CLMP_METERCH)))
			{
				pp->spd_motor = pp->spd_motor + GetRPMByServoNo(0);
				pp->spd_oil = pp->spd_oil + pp->st->CLAMP_SPEED;
				pp->spd_num++;	
			}

			CHK_SET_MOV(SetClampCls80_RF);
			break;
		case STAGE_8_STEP:
		case STAGE_8_STEP+1:
			SET_OFF(ClampClsOffRF);
			ACT_STEP_NEW(STAGE_9_STEP);
			break;
		case STAGE_9_STEP:
			if(ACT_STEP_100MS < 20)
				break;
		case STAGE_9_STEP+1:	
			//高速开模
			if(MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->rf_modelen_80))
			{
				pp->rf_K[3] = pp->spd_oil*100/pp->spd_motor + pp->rf_K[3];
				pp->rf_K_num[3]++;

				pp->spd_motor = 0;
				pp->spd_oil = 0;						
				pp->spd_num = 0;
				
				ACT_STEP_NEW(STAGE_10_STEP);
				break;
			}

			if(MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->rf_modelen_50))
			{
				pp->spd_motor = pp->spd_motor + GetRPMByServoNo(0);
				pp->spd_oil = pp->spd_oil + pp->st->CLAMP_SPEED;
				pp->spd_num++;				
			}

			CHK_SET_MOV(SetClampOpen80_RF);			
			break;
		case STAGE_10_STEP:
		case STAGE_10_STEP+1:
			pp->rf_num++;
			SET_OFF(ClampOpenOffRF);
			if(pp->rf_num >= 10)
			{
				ACT_STEP_NEW(STAGE_11_STEP);
			}
			else
			{
				ACT_STEP_NEW(STAGE_3_STEP);
			}
			break;
		case STAGE_11_STEP:
			pp->mh->EFFICIENCY_FACTOR_CLS = pp->rf_K[0]/pp->rf_K_num[0];
			pp->mh->EFFICIENCY_FACTOR_OPN = pp->rf_K[1]/pp->rf_K_num[1];
//			pp->mh->EFFICIENCY_FACTOR_CLS_H = pp->rf_K[2]/pp->rf_K_num[2];
//			pp->mh->EFFICIENCY_FACTOR_OPN_H = pp->rf_K[3]/pp->rf_K_num[3];
			
			ret = ACT_END;
			break;
		default:
			SET_OFF(ClampRFOffRF);
			ret = ACT_END;
			break;
	}

	return ret;
}



/*--------------------*/

BOOL InitClampPart(PCLAMP pp, UI8 sn)
{
    static ACT_STOP_FUNC clmpopn_stop_func_init[] = {ClampOpenOffME, ClampOpenOffME, ClampOpenOff};

	static const SUB_INIT_INFO sub_init[]  = CLAMP_INIT_INFO;

	static const ACT_INIT_INFO act_init[] = {{OS_CLSM, OS_CLSM, ACT_NTYPE, ClampCloseProc, ClampClsOff_2},
									   {OS_PRSM, OS_PRSM, ACT_NTYPE, ClampPress, ClampClsOff_3},
									   {OS_OPNM, OS_OPNM, ACT_NTYPE, ClampOpenProc, NULL, clmpopn_stop_func_init},
									   {OS_ACLS, OS_CLSM, ACT_NTYPE, ClampClsAdj, ClampClsOff_3},
									   {OS_AOPN, OS_OPNM, ACT_NTYPE, ClampOpnAdj, ClampOpenOff},
									   {OS_AAPRS, OS_PRSM, ACT_NTYPE, AutoAdjustPresMold, ClampClsOff_3},
									   {OS_AAOPN, OS_OPNM, ACT_NTYPE, AutoAdjClampOpn, ClampOpenOff},
									   {OS_EOPN, OS_OPNM, ACT_NTYPE, ClampOpnExhaust, ClampOpenOff},
									   {OS_EPRS, OS_PRSM, ACT_NTYPE, ClampPressExhaust, ClampClsOff_3},
									   {OS_EXHAST, OS_EXHAST, ACT_CTRL, ExhaustTaskProc, NULL},
									   {OS_ERRPRS, OS_PRSM, ACT_NTYPE, ClampPressERR, ClampClsOff_3},
									   {OS_ERROPN, OS_OPNM, ACT_NTYPE, ClampOpnERR, ClampOpenOff},
									   {OS_MODERF, OS_MODERF, ACT_NTYPE, ClampRF, ClampRFOffRF},};
	
	if(!INIT_PART(pp, CLAMP_ID, sn, sub_init))
		return FALSE;

	if(!INIT_ACT(pp, act_init))
		return FALSE;

	ClampSetup(pp);
	
	pp->mh->CLMP_METERCH = DEF_CLAMP_METER_CH;
	SetAdChType(pp->mh->CLMP_METERCH, CH_METER);
	pp->mh->CLMP_PRESSCH = DEF_PRESSENS2_METER_CH;
	SetAdChType(pp->mh->CLMP_PRESSCH, CH_METER);
	
	pp->pi->I_CLS_END = 6;
	pp->pa->ADPOSI_CLS1END = 300;
	pp->pa->ADPOSI_CLS2END = 200;
	pp->pa->ADPOSI_CLS3END = 50;
	pp->pa->ADPOSI_CLSPROTECTEND = 30;
	
	pp->pa->ADPOSI_OPENENDSLOW = 5;
	pp->pa->ADPOSI_OPENEND2 = 200;
	pp->pa->ADPOSI_OPENEND3 = 300;
	pp->pa->ADPOSI_OPENEND4 = 350;
	pp->pa->ADPOSI_OPENENDEND = 450;
	pp->md->TM_CLSPROTECTDEFAULTMAX = 1000;
	pp->md->TM_CLSHIGHDEFAULTMAX = 1000;

	return TRUE;
}

UI32 ClampCmd(PCLAMP pp, UI32 cmd, UI32 para)
{
	UI32 ret = TRUE;
	PMACHINE pm = &m_machine;
	UI16 offset = 0;
	UI16 len = 0;	
	if(pp == NULL)
		return FALSE;

	switch(GET_CMD(cmd))
	{
		case CMD_MANUAL_INIT:
			InitClampTmProject(pp);
			break;
			
		case CMD_SETUP:
			ClampSetup(pp);
			break;
			
		case CMD_METERCH:
			ret = pp->mh->CLMP_METERCH;
			break;					
		case CMD_QC_PARA:
			if(ChkOperMode(OM_AUTO) && (pm->md->Qc_Fun==1))
			{
				if(para ==1)// 合模QC数据处理
				{
					if(pp->md->Qc_Mode_ClsTm== 1)  //合模时间QC数据
					{
						if(pm->st->Qc_Opn_Cnt == 0)
						{
							pp->qc_avg_opnclstm = pp->st->TM_CLAMPCLS;
							pp->st->Qc_Min_Cls  =  pp->st->TM_CLAMPCLS;
							pp->st->Qc_Max_Cls  =  pp->st->TM_CLAMPCLS;		
							pp->st->Qc_Unq_Cnt_Cls = 0;
						}
						else
						{
							pp->qc_avg_opnclstm = pp->qc_avg_opnclstm+pp->st->TM_CLAMPCLS;
							pp->st->Qc_Min_Cls  =  (pp->st->Qc_Min_Cls>pp->st->TM_CLAMPCLS)?pp->st->TM_CLAMPCLS: pp->st->Qc_Min_Cls;
							pp->st->Qc_Max_Cls  =  (pp->st->Qc_Max_Cls<pp->st->TM_CLAMPCLS)?pp->st->TM_CLAMPCLS: pp->st->Qc_Max_Cls;
						}
						if(((pp->st->TM_CLAMPCLS>(pp->md->Qc_TmSet_ClsTm+ pp->md->Qc_DeviationSet_ClsTm))||
						 (pp->st->TM_CLAMPCLS<(pp->md->Qc_TmSet_ClsTm - pp->md->Qc_DeviationSet_ClsTm)))&&((pm->st->Qc_Opn_Cnt+1) <=pm->md->Qc_OpnNum))
			 				pp->st->Qc_Unq_Cnt_Cls++;						
					}
					if(pp->md->Qc_Mode_ClsLow== 1) //合模低压时间QC数据
					{
						if(pm->st->Qc_Opn_Cnt == 0)
						{
							pp->qc_avg_opnclslow = pp->st->TM_CLAMPCLSLO;
							pp->st->Qc_Min_ClsLow  =  pp->st->TM_CLAMPCLSLO;
							pp->st->Qc_Max_ClsLow  =  pp->st->TM_CLAMPCLSLO;
							pp->st->Qc_Unq_Cnt_ClsLow = 0;
						}
						else
						{
							pp->qc_avg_opnclslow =pp->qc_avg_opnclslow+ pp->st->TM_CLAMPCLSLO;
							pp->st->Qc_Min_ClsLow  =  (pp->st->Qc_Min_ClsLow>pp->st->TM_CLAMPCLSLO)?pp->st->TM_CLAMPCLSLO: pp->st->Qc_Min_ClsLow;
							pp->st->Qc_Max_ClsLow  =  (pp->st->Qc_Max_ClsLow<pp->st->TM_CLAMPCLSLO)?pp->st->TM_CLAMPCLSLO: pp->st->Qc_Max_ClsLow;
						}
						if(((pp->st->TM_CLAMPCLSLO>(pp->md->Qc_TmSet_ClsLow+ pp->md->Qc_DeviationSet_ClsLow))||
						 (pp->st->TM_CLAMPCLSLO<(pp->md->Qc_TmSet_ClsLow - pp->md->Qc_DeviationSet_ClsLow)))&&((pm->st->Qc_Opn_Cnt+1) <=pm->md->Qc_OpnNum))
			 				pp->st->Qc_Unq_Cnt_ClsLow++;								
					}
					if(pp->md->Qc_Mode_ClsHi== 1) //合模高压时间QC数据
					{
						if(pm->st->Qc_Opn_Cnt == 0)
						{
							pp->qc_avg_opnclshi = pp->st->TM_CLAMPCLSHI;
							pp->st->Qc_Min_ClsHi  =pp->st->TM_CLAMPCLSHI;
							pp->st->Qc_Max_ClsHi  =pp->st->TM_CLAMPCLSHI;
							pp->st->Qc_Unq_Cnt_ClsHi = 0;
						}
						else
						{
							pp->qc_avg_opnclshi = pp->qc_avg_opnclshi+pp->st->TM_CLAMPCLSHI;
							pp->st->Qc_Min_ClsHi	=  (pp->st->Qc_Min_ClsHi>pp->st->TM_CLAMPCLSHI)?pp->st->TM_CLAMPCLSHI: pp->st->Qc_Min_ClsHi;
							pp->st->Qc_Max_ClsHi	=  (pp->st->Qc_Max_ClsHi<pp->st->TM_CLAMPCLSHI)?pp->st->TM_CLAMPCLSHI: pp->st->Qc_Max_ClsHi;
						}
						if(((pp->st->TM_CLAMPCLSHI>(pp->md->Qc_TmSet_ClsHi+ pp->md->Qc_DeviationSet_ClsHi))||
						 (pp->st->TM_CLAMPCLSHI<(pp->md->Qc_TmSet_ClsHi - pp->md->Qc_DeviationSet_ClsHi)))&&((pm->st->Qc_Opn_Cnt+1) <=pm->md->Qc_OpnNum))
			 				pp->st->Qc_Unq_Cnt_ClsHi++;								
					}										
				}	
				else if(para ==2)// 开模QC数据处理
				{
					if(pp->md->Qc_Mode_OpnTm==1)//开模QC数据处理
					{
						if(pm->st->Qc_Opn_Cnt == 0)
						{
							pp->qc_avg_opntm = pp->st->TM_CLAMPOPN;
							pp->st->Qc_Min_OpnTm = pp->st->TM_CLAMPOPN;
							pp->st->Qc_Max_OpnTm = pp->st->TM_CLAMPOPN;
							pp->st->Qc_Unq_Cnt_OpnTm = 0;
						}
						else
						{
							pp->qc_avg_opntm =pp->qc_avg_opntm+ pp->st->TM_CLAMPOPN;
							pp->st->Qc_Min_OpnTm	=  (pp->st->Qc_Min_OpnTm>pp->st->TM_CLAMPOPN)?pp->st->TM_CLAMPOPN: pp->st->Qc_Min_OpnTm;
							pp->st->Qc_Max_OpnTm	=  (pp->st->Qc_Max_OpnTm<pp->st->TM_CLAMPOPN)?pp->st->TM_CLAMPOPN: pp->st->Qc_Max_OpnTm;		
						}
						if(((pp->st->TM_CLAMPOPN>(pp->md->Qc_TmSet_OpnTm+ pp->md->Qc_DeviationSet_OpnTm))||
						 (pp->st->TM_CLAMPOPN<(pp->md->Qc_TmSet_OpnTm - pp->md->Qc_DeviationSet_OpnTm)))&&((pm->st->Qc_Opn_Cnt+1) <=pm->md->Qc_OpnNum))
			 				pp->st->Qc_Unq_Cnt_OpnTm++;							
					}
					if(pp->md->Qc_Mode_OpnEnd==1)//开模QC数据处理
					{
						if(pm->st->Qc_Opn_Cnt == 0)
						{
							pp->qc_avg_opnend = pp->st->ADPOSI_OPNEND;
							pp->st->Qc_Min_OpnEnd	= pp->st->ADPOSI_OPNEND;
							pp->st->Qc_Max_OpnEnd 	= pp->st->ADPOSI_OPNEND; 
							pp->st->Qc_Unq_Cnt_OpnEnd = 0;
						}
						else
						{
							pp->qc_avg_opnend = pp->qc_avg_opnend +pp->st->ADPOSI_OPNEND;
							pp->st->Qc_Min_OpnEnd	=  (pp->st->Qc_Min_OpnEnd>pp->st->ADPOSI_OPNEND)?pp->st->ADPOSI_OPNEND: pp->st->Qc_Min_OpnEnd;
							pp->st->Qc_Max_OpnEnd	=  (pp->st->Qc_Max_OpnEnd<pp->st->ADPOSI_OPNEND)?pp->st->ADPOSI_OPNEND: pp->st->Qc_Max_OpnEnd;		
						}
						if(((pp->st->ADPOSI_OPNEND>(pp->md->Qc_TmSet_OpnEnd+ pp->md->Qc_DeviationSet_OpnEnd))||
						 (pp->st->ADPOSI_OPNEND<(pp->md->Qc_TmSet_OpnEnd - pp->md->Qc_DeviationSet_OpnEnd)))&&((pm->st->Qc_Opn_Cnt+1) <=pm->md->Qc_OpnNum))
			 				pp->st->Qc_Unq_Cnt_OpnEnd++;								
					}
					if(pp->md->Qc_Mode_OpnSpd==1)//开模QC数据处理
					{
						if(pm->st->Qc_Opn_Cnt == 0)
						{
							pp->qc_avg_opnspd = pp->st->CLAMP_OPN_SPD;
							pp->st->Qc_Min_OpnSpd = pp->st->CLAMP_OPN_SPD;
							pp->st->Qc_Max_OpnSpd =  pp->st->CLAMP_OPN_SPD;
							pp->st->Qc_Unq_Cnt_OpnSpd = 0;
						}
						else
						{
							pp->qc_avg_opnspd =pp->qc_avg_opnspd+ pp->st->CLAMP_OPN_SPD;
							pp->st->Qc_Min_OpnSpd=  (pp->st->Qc_Min_OpnSpd>pp->st->CLAMP_OPN_SPD)?pp->st->CLAMP_OPN_SPD: pp->st->Qc_Min_OpnSpd;
							pp->st->Qc_Max_OpnSpd=  (pp->st->Qc_Max_OpnSpd<pp->st->CLAMP_OPN_SPD)?pp->st->CLAMP_OPN_SPD: pp->st->Qc_Max_OpnSpd;	
						}
						if(((pp->st->CLAMP_OPN_SPD>(pp->md->Qc_TmSet_OpnSpd+ pp->md->Qc_DeviationSet_OpnSpd))||
						 (pp->st->CLAMP_OPN_SPD<(pp->md->Qc_TmSet_OpnSpd - pp->md->Qc_DeviationSet_OpnSpd)))&&((pm->st->Qc_Opn_Cnt+1) <=pm->md->Qc_OpnNum))
			 				pp->st->Qc_Unq_Cnt_OpnSpd++;								
					}				
				}
			}				
			break;
		case CMD_DATA_RX:
			offset = (para >> 16 & 0xffff);
            len = (para & 0xffff);
			
			if((GET_CMD_PARA(cmd) & 0xff) == MD_ID)
			{			
				if (len == 1)
				{
					if (GET_IND(PCLAMP_MOLDSET, OPN_SPD_STOPOFF) == offset)
					{
						AiClampRampDataInput(pp->pa->ADPOSI_OPENENDEND_MODE, pp->pa->DAFLOW_OPENEND*10, pp->md->OPN_SPD_STOPOFF);
					}						
				}
					
				if(len > 0 
				&& ((GET_IND(PCLAMP_MOLDSET, SLOPE_MAX_OPN_PRES) >= offset && GET_IND(PCLAMP_MOLDSET, PRESSTART_CLOSE_MID) <= (offset + len - 1))
				||(GET_IND(PCLAMP_MOLDSET, CLS_PRES_STOPOFF) >= offset && GET_IND(PCLAMP_MOLDSET, OPN_SPD_STARTON) <= (offset + len - 1))))
				{
					//修改斜率相关参数时，对应缓存进行计算
					InitClampRamp(pp);
				}				

				//修改斜率相关参数时，对应缓存进行计算
				if (pp->md->FL_CLSOPEN_MODE == 3 || pp->md->FL_CLSOPEN_MODE == 4)
				{
					pp->clampopnnum = 2;
				}
				else
				{
					pp->clampopnnum = pp->md->OPN_NUM;
				}							
			}
			else if((GET_CMD_PARA(cmd) & 0xff) == MH_ID)
			{
				if(len > 0 && (GET_IND(PCLAMP_MACHSET, DAPFLOW_OPNMIDEND_RAMPOFF) >= offset && GET_IND(PCLAMP_MACHSET, DAPRES_CLSRAMP) <= (offset + len - 1)))
				{
					//修改斜率相关参数时，对应缓存进行计算
					InitClampRamp(pp);
					InitOilTempAi(pp);
				}
			}
			else if((GET_CMD_PARA(cmd) & 0xff) == PA_ID)
			{
				if (len == 1)
				{
					if (GET_IND(PCLAMP_ACTPARA, ADPOSI_OPENENDEND) == offset || GET_IND(PCLAMP_ACTPARA, DAFLOW_OPENEND) == offset)
					{
						InitClampRamp(pp);
						InitOilTempAi(pp);
					}
				}				
			}

			
			pp->pa->PER_DAFLOW_CLAMPCLS= pp->pa->PER_DAFLOW_CLAMPCLS > 150 || pp->pa->PER_DAFLOW_CLAMPCLS == 0 ?100:pp->pa->PER_DAFLOW_CLAMPCLS;
			pp->pa->PER_DAFLOW_CLAMPOPN= pp->pa->PER_DAFLOW_CLAMPOPN > 150 || pp->pa->PER_DAFLOW_CLAMPOPN == 0 ?100:pp->pa->PER_DAFLOW_CLAMPOPN;
			pp->pa->PER_DAFLOW_HIGHPRES= pp->pa->PER_DAFLOW_HIGHPRES > 150 || pp->pa->PER_DAFLOW_HIGHPRES == 0 ?100:pp->pa->PER_DAFLOW_HIGHPRES;
			pp->pa->PER_DAFLOW_LOWPRES= pp->pa->PER_DAFLOW_LOWPRES > 150 || pp->pa->PER_DAFLOW_LOWPRES == 0 ?100:pp->pa->PER_DAFLOW_LOWPRES;
			pp->pa->PER_DAFLOW_OPEN1= pp->pa->PER_DAFLOW_OPEN1 > 150 || pp->pa->PER_DAFLOW_OPEN1 == 0 ?100:pp->pa->PER_DAFLOW_OPEN1;
			
			pp->md->CLPCLS_SPD_ULT= pp->md->CLPCLS_SPD_ULT > 150 || pp->md->CLPCLS_SPD_ULT == 0 ?100:pp->md->CLPCLS_SPD_ULT;
			pp->md->CLPCLS_SPD_UTI= pp->md->CLPCLS_SPD_UTI > 150 || pp->md->CLPCLS_SPD_UTI == 0 ?100:pp->md->CLPCLS_SPD_UTI;
			pp->md->CLPOPN_SPD_UTI= pp->md->CLPOPN_SPD_UTI > 150 || pp->md->CLPOPN_SPD_UTI == 0 ?100:pp->md->CLPOPN_SPD_UTI;
			pp->mh->DAFLOW_CLPOPN_VLVLIMIT = pp->mh->DAFLOW_CLPOPN_VLVLIMIT > 150 || pp->mh->DAFLOW_CLPOPN_VLVLIMIT == 0 ?100:pp->mh->DAFLOW_CLPOPN_VLVLIMIT;

            if (CUS_ID != CS_TONGYONG)
                pp->mh->TM_ROBOT_ACT = 0;

			InitClampTmProject(pp);
			break;
		default:
			ret = FALSE;
			break;
	}

	return ret;
}

BOOL ShotReached(PCLAMP pp)
{
	UI32 curshot, shotset;

	if(pp == NULL)
		return FALSE;
	
	curshot = ((UI32)(pp->st->CN_MOLDOPNNUM0) << 16) + pp->st->CN_MOLDOPNNUM1;
	
	if ((AutoAlarmIsStart() == 0) && (GetSysMold()->CN_AUTOALARMUNIT > 0 )&&((!ChkSysCfg(XF0_AUTOARMOD) && GetMoldAlarmMode() == 1) || (ChkSysCfg(XF0_AUTOARMOD))))
	{
		m_alarm.st->AUTOALARM_SHORTNUM = GetSysMold()->CN_AUTOALARMUNIT - (curshot - pp->last_chk_opennum);
		
		if(((curshot - pp->last_chk_opennum) > 3 || ChkSysCfg(XF0_ADJLU))&& (curshot - pp->last_chk_opennum) >= (GetSysMold()->CN_AUTOALARMUNIT))
		{
			SetAutoAlarmStart(1);
			if(ChkSysCfg(XF0_AUTOARMOD))     //自动警报状态下位机自动修改
			{
				SetMoldAlarmMode(1);
			}
			return TRUE;
		}	
	}
	else if(GetSysMold()->CN_AUTOALARMUNIT == 0)
	{
		/*面板会判断FL_AUTOALARM与其不一致时，会重新下发，下位机不主动修改FL_AUTOALARM值，以面板为主*/
		  SetAutoAlarmStart(0);
		if(ChkSysCfg(XF0_AUTOARMOD))     //自动警报状态下位机自动修改
		{
			SetMoldAlarmMode(0);
		}
		m_alarm.st->AUTOALARM_SHORTNUM = GetSysMold()->CN_AUTOALARMUNIT;
		pp->last_chk_opennum = ((UI32)(pp->st->CN_MOLDOPNNUM0) << 16) + pp->st->CN_MOLDOPNNUM1;
	}

	/*  包装控制 07-7-26      */
	if(pp->md->CN_PACKAGE)
	{
    	if(pp->st->CN_CURPACKMOLD>=pp->md->CN_PACKAGE)
		{
			pp->st->CN_CURPACKMOLD= 0;	
			SET_PART_ERR(pp, ER0_PACK);
			TempAlarm(TRUE);
    	}
		else
			CLR_PART_ERR(pp, ER0_PACK);
	}  


	if(pp->md->FL_MOLDREACHCHK == 1)
		curshot = pp->st->CN_VALIDMOLD;

	shotset = ((UI32)(pp->md->CN_PRODUCTSETTING1) << 16) + pp->md->CN_PRODUCTSETTING2;

	if (shotset <= 0)
		shotset = curshot +1;

	if(curshot >= shotset)
	{
		SET_PART_ERR(pp, ER0_SHOT);
	}

	return FALSE;
}

void MachineSafeCheck()
{
	PCLAMP pp = (PCLAMP)GetPartByID(MK_PART(CLAMP_ID, 1));
    PACT pact = pp->act+OS_CLSM-1;
	UI16	vlv_delay;

	if(pp->md->SAFEVLVOFF > 3000)
		vlv_delay = 3000;
	else
		vlv_delay = pp->md->SAFEVLVOFF;
	
	if(!CHK_PART_ERR(pp->psfdr, ER3_FSER))
	{
		if(CheckPI(pp->psfdr->pi->I_SDR_CLS)==PI_SET)
		{
			if((ChkActRun(pp, ACT_SN_BIT(OS_CLSM)) &&(pact->step > INIT1_STEP + 1)&& (pact->step < END_STEP)) || ChkActRun(pp, ACT_SN_BIT(OS_PRSM)))
			{
				//上海光速	在关模前检测安全阀是否已经关闭
				if (pp->safevlv_step == 0 && CUS_ID == CS_GUANGSHU)
				{
					if ((CheckPI(pp->psfdr->pi->I_SDR_VALVE) == PI_SET) && !ChkActRun(pp, ACT_SN_BIT(OS_PRSM)))
					{					
						if (!CHK_PART_ERR(pp->psfdr, ER5_SAVALVE))
						{
							SET_PART_ERR(pp->psfdr, ER5_SAVALVE);						
							ManualInitErr();
							SetOperMode(OM_MANUAL);
							StandAlarm(TRUE);
						}
					}
					else
					{
						pp->safevlv_step = 1;
					}
				}
				else
				{
					if((UI16)(GetMsTick()-pp->delaycheck1) >= vlv_delay)
					{
						pp->delaycheck1 = GetMsTick()-vlv_delay;
						if(CheckPI(pp->psfdr->pi->I_SDR_VALVE) == PI_RESET && !CHK_PART_ERR(pp->psfdr, ER5_SAVALVE))
					  	{
							SET_PART_ERR(pp->psfdr, ER5_SAVALVE);						
							ManualInitErr();
							SetOperMode(OM_MANUAL);
							StandAlarm(TRUE);
						}
					}

					if (CUS_ID != CS_ZHUANHUI)
					{
						if (ChkSysCfg(XF3_EUROB))
						{
							SetPOBit(pp->psfdr->po->O_SDR_VALVE,PC_OFF);
						}
						else
						{
							SetPOBit(pp->psfdr->po->O_SDR_VALVE,PC_ON);
						}
					}				
					pp->delaycheck0 = GetMsTick();
				}
			}
			else
			{
				pp->safevlv_step = 0;
				pp->delaycheck1 = GetMsTick();
				pp->delaycheck0 = GetMsTick();
			}

			if (CUS_ID == CS_ZHUANHUI)
			{
				if(ChkSysCfg(XF3_EUROB))
					SetPOBit(pp->psfdr->po->O_SDR_VALVE,PC_OFFD);
				else
					SetPOBit(pp->psfdr->po->O_SDR_VALVE,PC_ON);
			}
			
		}
		else
		{
			if(CheckPI(pp->psfdr->pi->I_SDR_VALVE) == PI_SET && !CHK_PART_ERR(pp->psfdr, ER5_SAVALVE))
			{
	      		if((UI16)(GetMsTick()-pp->delaycheck0) >= vlv_delay)
	      		{
					SET_PART_ERR(pp->psfdr, ER5_SAVALVE);						
					ManualInitErr();
					SetOperMode(OM_MANUAL);
					StandAlarm(TRUE);
	      		 }
			}
			else
			{
				pp->delaycheck0 = GetMsTick();
			}
			if (ChkSysCfg(XF3_EUROB))
			{
				SetPOBit(pp->psfdr->po->O_SDR_VALVE,PC_ON);
			}
			else
			{
				SetPOBit(pp->psfdr->po->O_SDR_VALVE,PC_OFFD);
			}

			pp->safevlv_step = 0;
			pp->delaycheck1 = GetMsTick();
		}
	}
	else
	{
		if (ChkSysCfg(XF3_EUROB))
		{
			SetPOBit(pp->psfdr->po->O_SDR_VALVE,PC_ON);
		}
		else
		{
			SetPOBit(pp->psfdr->po->O_SDR_VALVE,PC_OFFD);
		}
		
	}
}

void MachineSafeCheck2()
{
	PCLAMP pp = (PCLAMP)GetPartByID(MK_PART(CLAMP_ID, 1));
	UI16	vlv_opn_delay,vlv_cls_delay;
	static UI16	fl_safevalve_st = 0;

	if (pp->psfdr == NULL)
		return;

	if (GetSecTick() > 15)
	{
		fl_safevalve_st = 1;
	}
	
	if(pp->psfdr->mh->SDR_VALVE_OPNTMCHECK > 999)
		vlv_opn_delay = 999;
	else
		vlv_opn_delay = pp->psfdr->mh->SDR_VALVE_OPNTMCHECK;

	if(pp->psfdr->mh->SDR_VALVE_CLSTMCHECK > 999)
		vlv_cls_delay = 999;
	else
		vlv_cls_delay = pp->psfdr->mh->SDR_VALVE_CLSTMCHECK;

	if (TestPOIsOn(pp->po->O_CLS_SOL))
	{
		if (TestPOIsOn(pp->psfdr->po->O_SDR_VALVE))
		{	
			//安全阀超时打开，警报并切手动。
			if (DIFF_10MS(pp->delaycheck1) > vlv_opn_delay)
			{
				if (CheckPI(pp->psfdr->pi->I_SDR_VALVE) == PI_SET)
				{
					SET_PART_ERR(pp->psfdr,ER1_SAVALVEOPN);
					StandAlarm(TRUE);
				}
			}
		}
		else
		{
			//关模阀输出后，安全阀输出前判断，安全阀是否复位。未复位立刻警报。
			if (CheckPI(pp->psfdr->pi->I_SDR_VALVE) == PI_RESET)
			{
				SET_PART_ERR(pp->psfdr,ER5_SAVALVE);
				ManualInitErr();
				StandAlarm(TRUE);								
 				SetOperMode(OM_MANUAL);
			} 

			pp->delaycheck1 = Get10MsTick();			
		}
	}
	

	//做过关模动作，才开始检测安全阀异常。按手动清除状态
	if (fl_safevalve_st == 1)
	{
		if (ChkAdFwdInplace(pp->mh->CLMP_METERCH))
		{
			pp->delaycheck0 = Get10MsTick();
		}	
		
		if (ChkOperMode(OM_AUTO))
		{
			if (CheckPI(pp->psfdr->pi->I_SDR_VALVE) == PI_RESET)
			{
				if (DIFF_10MS(pp->delaycheck0) > vlv_cls_delay)
				{	//非开合模过程及锁模状态时开始计时，超时警报
					SET_PART_ERR(pp->psfdr,ER5_SAVALVE);
					ManualInitErr();
					StandAlarm(TRUE);								
	 				SetOperMode(OM_MANUAL);
				}
			} 	
		}
		else
		{
			if (!ChkAdFwdInplace(pp->mh->CLMP_METERCH))
			{
				if (CheckPI(pp->psfdr->pi->I_SDR_VALVE) == PI_RESET)
				{
					if (DIFF_10MS(pp->delaycheck0) > vlv_cls_delay)
					{
						SET_PART_ERR(pp->psfdr,ER5_SAVALVE);

						if (FSafeDoorIsOpen(pp->psfdr))
						{
							StandAlarm(TRUE);
						}
					}					
				}		
			}
		}
	}

	if ((CheckPO(pp->po->O_CLS_SOL) == PO_SET) || (CheckPO(pp->po->O_OPN_SOL) == PO_SET))
	{
		SetPOBit(pp->psfdr->po->O_SDR_VALVE,PC_ON);	
		pp->delaycheck0 = Get10MsTick();
	}
	else
	{
		if (TestPOIsOn(pp->psfdr->po->O_SDR_VALVE) > 0)
		{
			SetPOBit(pp->psfdr->po->O_SDR_VALVE,PC_OFFD);		
		}
	}
}

void MachineSafeCheckPro()
{
	if (CUS_ID == CS_SHENGBANG)
	{
		MachineSafeCheck2();
	}
	else
	{
		MachineSafeCheck();
	}
}

