/******************************************************************************
  文 件 名   : inject.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 螺杆部件及射出相关动作
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
#include "clamp.h"
#include "nozzle.h"
#include "robot.h"
#include "servo.h"
#include "chargpress.h"
#include "inject.h"
#include "machapi.h"
#include "machine.h"
#include "TempCtrlComm.h"

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/	
extern	 MACHINE m_machine;
extern	 HYDR	 g_hydrout;
extern	UI16	m_speed;

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
BOOL Temp_ErrInj_all(void);		//射出时检测模温偏差功能

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


void InitSuckRamp(PINJECT pp)
{
	//启动
	UI16 start,Acc;
	//结束
	UI16 end;
	
	if(pp->md->FL_SUCK_SOOMTH == 1)
	{
		Acc = ((UI32)pp->md->SLOPE_MAX_SUCK_SPD*(UI32)pp->md->SUCK_SPD_STARTON/100);
		Acc = Acc < 2 ? 2: Acc; 	
		start = 100000/Acc;
		
		Acc = ((UI32)pp->md->SLOPE_MAX_SUCK_SPD*(UI32)pp->md->SUCK_SPD_STOPOFF/100);
		Acc = Acc < 2 ? 2: Acc;
		end = 100000/Acc;

		pp->spdramp_suck[SUCK_RAMP_START] = start;
		pp->spdramp_suck[SUCK_RAMP_END] = end;		

		Acc = ((UI32)pp->md->SLOPE_MAX_SUCK_PRES*(UI32)pp->md->SUCK_PRES_STARTON/100);
		Acc = Acc < 2 ? 2: Acc; 	
		start = GetSysPresLimit()*100/Acc;
		
		Acc = ((UI32)pp->md->SLOPE_MAX_SUCK_PRES*(UI32)pp->md->SUCK_PRES_STOPOFF/100);
		Acc = Acc < 2 ? 2: Acc;
		end = GetSysPresLimit()*100/Acc;

		pp->pressramp_suck[SUCK_RAMP_START] = start;
		pp->pressramp_suck[SUCK_RAMP_END] = end;
	}
	else
	{		
		pp->spdramp_suck[SUCK_RAMP_START] = pp->mh->DAFLOW_SUCKBACKRAMP*10;
		pp->spdramp_suck[SUCK_RAMP_END] = pp->mh->DAFLOW_SUCKBACKOFFRAMP*10;

		pp->pressramp_suck[SUCK_RAMP_START] = pp->mh->DAPRES_SUCKBACKRAMP*10;
		pp->pressramp_suck[SUCK_RAMP_END] = pp->mh->DAPRES_SUCKBACKOFFRAMP*10;
	}	
}


void Init_inj_time(PINJECT pp)
{
	UI16 i;
	UI16 injtm = ChkSysCfg(CO1_INTM);

	if(injtm > 0)
	{
		for(i=0;i<10;i++)
		{
			pp->INJ_TM_N[i] = ((UI32)*(&(pp->pa->TM_INJ1_H)+i) << 16) + ((UI32)*(&(pp->pa->TM_INJ1)+i));
			pp->HOLD_TM_N[i] = ((UI32)*(&(pp->pa->TM_INJHOLD1_H)+i) << 16) + ((UI32)*(&(pp->pa->TM_INJHOLD1)+i));
		}
	}
	else
	{
		for(i=0;i<10;i++)
		{
			pp->INJ_TM_N[i] = (((UI32)*(&(pp->pa->TM_INJ1_H)+i) << 16) + ((UI32)*(&(pp->pa->TM_INJ1)+i)))*10;
			pp->HOLD_TM_N[i] = (((UI32)*(&(pp->pa->TM_INJHOLD1_H)+i) << 16) + ((UI32)*(&(pp->pa->TM_INJHOLD1)+i)))*10;
		}

	}
}

ACT_RET InjectPreChk_CBAC()
{
	PINJECT pp = (PINJECT)GetMachineSubPart(INJECT_ID, 1);
	if(InjTempChkPro(pp))
	{
		return ACT_OK;
	}
	else
	{
		return ACT_ERR_ABORT;
	}	
}

PNOZZLE InjectGetNozzle(PINJECT pp)
{
	if(pp != NULL)
		return pp->pnzl;
	else
		return NULL;
}

UI16 GetInjectHoldPress(PINJECT pp)
{
	if(pp != NULL)
		return pp->md->DAPRES_TURNTOHOLD;
	else
		return 0;
}

BOOL AutoChargeEnableChk(PINJECT pp)
{
	if(pp != NULL)
	{
		if(pp->md->TM_AUTOPURGE && pp->md->CN_AUTOPURGE)
			return TRUE;
	}

	return FALSE;
}

BOOL InjectAccIsUsed(PINJECT pp)
{
	if(pp != NULL)
		return (pp->md->FL_INCACC == 1);
	else
		return FALSE;
}

/*Called by Nozzle backward action*/
//PI_STATUS ChkChargAndSuckEndPI(PINJECT pp)
//{
//	if(pp != NULL)
//		return CheckPI(pp->pi->I_SAPB1);
//	else
//		return PI_NOUSED;
//}


BOOL InjectWarmErrChk(PINJECT pp)
{		
	if(pp->md->FL_InjectWarmOn==1)
	{
		if(!GetHeatingStatus())	
		{
			SET_PART_ERR(pp, ER0_WARM);
			return TRUE;
		}	
	}

	return FALSE;
}

BOOL InjectTempErrChk(PINJECT pp)
{
	int	i,j,err = 0;
	
	if(g_tempcomm_num > 0)
	{
		PTEMPCTRLCOMM pc;
		//检测到有使用E502等模块时，使用COMM代码
		for(i = 0;i<g_tempcomm_num;i++)
		{
			pc = g_tempcomm[i];
			if(pc != NULL)
			{
				for(j = 0;j < pc->mh->DATEMP_CH_NUM;j++)
				{
					if(CHK_TEMP_AI_CTRL(pc->mh->DATEMP_FUNC[j]))
				    {
				        if (pc->md->DATEMP_SETTING[j] > pc->st->DATEMP_REAL[j])
				        {
				            if ((pc->md->DATEMP_SETTING[j] - pc->st->DATEMP_REAL[j]) > pc->md->DATEMP_MIN[j])
							{
								SET_PART_ERR(pc, ER0_TEMPER_COMM);
								err = 1;
								break;
							}
				        }
				        else
				        {
				            if ((pc->st->DATEMP_REAL[j] - pc->md->DATEMP_SETTING[j]) > pc->md->DATEMP_MAX[j])
							{
								SET_PART_ERR(pc, ER0_TEMPER_COMM);
								err = 1;
								break;
							}
				        }
				    }
				}
			}							
		}

		if(err == 1)
		{
			return TRUE;
		}
	}
	return FALSE;
}

UI16 InjTempChkPro(PINJECT pp)
{
	if (pp->md->FL_INJECTWARMPRE == 1)
		return TRUE;
	
	if(InjectWarmErrChk(pp))
		return FALSE;
	
	if(TempIsHoldMoldErrChk() || TempMwIsHoldMoldErrChk())
		return FALSE;
	
	if(TempPreWarmErrChk())
		return FALSE;
		
	if(InjectTempErrChk(pp))
		return FALSE;	

	return TRUE;
}

UI16 InjTempChkSimple(PINJECT pp)
{
	if (pp->md->FL_INJECTWARMPRE == 1)
		return TRUE;

	if(InjectTempErrChk(pp))
		return FALSE;
	
	if(InjectWarmErrChk(pp))
		return FALSE;

	return TRUE;
}
/*---------------------------------------------------------------------------+
|           code segment                                                     |
+---------------------------------------------------------------------------*/
ACT_RET  InjectPre(PINJECT pp, PACT pact)
{
	if(!InjTempChkPro(pp))
	{
		return ACT_ERR_ABORT;
	}

	if(Temp_ErrInj_all())
		return ACT_ERR_ABORT;
		
	if(ChkSysCfg(CO2_INCE)&&ClampClsEndErrChk(pp->pclmp))
	{
		return ACT_ERR_ABORT;
	}

  	if (ClampTempErrChk(pp->pclmp))
  		return ACT_ERR_ABORT;

	if(pp->md->FL_INCACC == 1 && !ChkSysCfg(CO3_STEN) && ChkOperMode(OM_AUTO) && NozzleFwdEndErrChk(pp->pnzl))	/* 系统比例阀储能 */
		return ACT_ERR_ABORT;

	return ACT_OK;
}


/*-----------------------------------------------------------------------------------+
|      射出防护罩检查                                                                |
+-----------------------------------------------------------------------------------*/
ACT_RET InjectPurgeGuard(PINJECT pp, PACT pact)
{
	if(PartErrSet(pp, ER0_PURG, CheckPI(pp->pi->I_PUR_GUA) != PI_SET))
	{
		pact->ftaskstat |= INTF_ERR;
		return ACT_ERR;
	}

	pact->ftaskstat&= (~INTF_ERR);

	return ACT_OK;
}

/*-----------------------------------------------------------------------------------+
|      射出，保压各段开阀，压力流量以及压力流量的斜率设定函数                              |
+-----------------------------------------------------------------------------------*/
void HtHydrComb(PINJECT pp)            //海太双油路合流
{
	if(ChkSysCfg(XF1_HTMHYD))
	{
		SetPOBit(GetSysPO(O_HYD_VLV2), PC_ON);
		if(pp->md->FL_INJFAST == 1)
			SetPOBit(GetSysPO(O_HYD_COMB), PC_ON);
	}
}

void  SetInjectTime(PINJECT pp, PACT pact)
{	
	pp->injectchk_100us = pp->INJ_TM_N[0]*10;
	pp->holdchk_press = pp->md->DAPRES_TURNTOHOLD*10;        
}

BOOL InjTMPressChk(PINJECT pp, PACT pact)
{
	if(pp->md->FL_TURNTOHOLD == 1 && pact->step >= INJCT3)
	{
		/*压力*/
		if(GetMeterPosByCh(pp->pclmp->mh->CLMP_PRESSCH) >= pp->holdchk_press)
			return TRUE;		
	}
	else if(pp->md->FL_TURNTOHOLD == 4)
	{
		if(GetMeterPosByCh(pp->mh->INJ_METERCH)<=pp->md->ADPOSI_TURNTOHOLD)
		  return TRUE;				
	}
	else if (pp->md->FL_TURNTOHOLD == 5)
	{
		//分段时间转保压直接在分段时间函数处跳转。
		return FALSE;
	}
	else
	{
		if((pp->md->FL_TURNTOHOLD == 0) && (!ChkSysCfg(XF0_CLSEB)))
		{
			if(GetMeterPosByCh(pp->mh->INJ_METERCH)<=pp->md->ADPOSI_TURNTOHOLD)
		  		return TRUE;	
		}
		
//		if(Get100UsTick() - pp->injectstart_100us >= (pp->injectchk_100us))
//			return TRUE;

		//注射走时间 使用记步数方式		
		if (pact->act_step_num>= (pp->injectchk_100us/MAX_LOOP_100US))
		{
			return TRUE;
		}
			
	}
	return FALSE;
}


void InjectFwdVlv(PINJECT pp, PACT pact)
{
  if(pp->mh->TMDLY_INJVALVEON > 500)
  	pp->mh->TMDLY_INJVALVEON = 0;
  if(ChkSysCfg(CO5_INJCHADA))
  {
  	if(pp->md->Inj_Open==1)
	 SetPOBitDelay(pp->po->O_INJ_OPEN, PC_ON, pp->mh->TMDLY_INJVALVEON);
    else
  	 SetPOBit(pp->po->O_INJ_OPEN,PC_OFF);
  }
	HtHydrComb(pp);
	SetPOBit(GetSysPO(O_FST_SOL), PC_ON);
  	SetPOBit(pp->po->O_IN_SETU, PC_ON);

	if(ChkOperMode(OM_AUTO) && ChkSysCfg(XF6_INJCLS))
		ClampClsVlvSet(pp->pclmp, TRUE);

	if(ChkOperMode(OM_AUTO))// && pp->md->FL_INJECTCOREOFF == 0)
		ClampCoreFwdPOSet(pp->pclmp, TRUE);

	if(NozzleIsReach(pp->pnzl) || ChkOperMode(OM_AUTO))
		SetNozzleFwdPO(pp->pnzl, PC_ON);

	if(pp->pclmp != NULL && pp->pclmp->prbt != NULL)
		RobotInjectOnOff(pp->pclmp->prbt, TRUE);

	if(pp->md->FL_PASSVALUE == 1 || pp->md->FL_PASSVALUE == 3)
		SetPOBit(pp->po->O_PASS_VLV, PC_ON);

	if(pp->md->FL_TURNTOHOLD == 5)   //如果是时间模式，就不需要显示全部的射出时间了。而且这个显示会造成每段射出时间无法归零。DK 2014.1105
	{
	}
	else
	{
		ActSetTM(pact->ptsk, pp->INJ_TM_N[0]/10);
	}
	SetPOBit(pp->po->O_INJ_SRV, PC_ON);

	//if(CUS_ID == CS_YANGXIN)             //广州阳鑫  射出保压时 开储料背压阀
	if(ChkSysCfg(CO2_INJCHG))
	{
		SetPOBit(pp->po->O_CHR_BAC, PC_ON);
	}

	if(pp->md->HYDY_WAY_SET == 1)
	{
		SetPOBit(GetSys()->po->HYD_COMB, PC_ON);
	}

	SetPOBitDelay(pp->po->O_INJ_SOL, PC_ON, pp->mh->TMDLY_INJVALVEON);
}

void SetInjectFwdNum(PINJECT pp, PACT pact)
{
	UI16 hydelay;
	int old_flow_dot;	
	UI16 temppres,tempflow;
	
	WORD ind = GET_M_STEP_NO(pact->step - INJCT1);
	
	if(ind == 0)
		return;

	ind = ind - 1;

	if(pp->mh->INJ_HYDELAY > 9999)
		pp->mh->INJ_HYDELAY = 0;
	hydelay = pp->mh->INJ_HYDELAY;
    /*通用新加20161008 LY*/
    if(GetOperMode() ==OM_PURG && ChkSysCfg(CO2_CHGX))
    {
		temppres =  pp->pa->DAPRES_INJPURGE;
		tempflow =  pp->pa->DAFLOW_INJPURGE;
    }
	else
	{
		if (pp->md->INJPRES_SETMODE == 0)
		{
			temppres =  *(&(pp->pa->DAPRES_INJ1) + ind);
		}
		else
		{
			temppres = pp->pa->DAPRES_INJ1;
		}
			
		tempflow =  *(&(pp->pa->DAFLOW_INJ1) + ind);
	}

	
	InjectFwdVlv(pp, pact);
	
	if (ChkSysCfg(CO1_INJF))
	{
		SetPOBit(pp->po->O_INJ_FST, PC_OFF);
		/*大于射出2段时才关闭射出增压阀*/
		if(ind > 1)
			SetPOBit(pp->po->O_INJ_ACC, PC_OFF);
	}
	else if(pp->md->FL_INJFAST == 1)
		SetPOBit(pp->po->O_INJ_FST, PC_ON); 
	
	old_flow_dot = ActSetFlowDot(ChkSysCfg(CO5_INJFLACC));
	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, temppres,tempflow,pp->mh->DAPRES_INJRAMP2,pp->mh->DAFLOW_INJRAMP2,
						pp->mh->DAPRES_INJRAMP,pp->mh->DAFLOW_INJRAMP, (pact->ftaskstat & INTF_CEC) ? 0 : HYDRDELY, HYDR_PORT(pp->hy->HYDR_PORT_INJCT),
						pp->hy->HYDR_PUMPS_INJCT,pp->hy->HYDR_PUMPS_INJCT,   pp->hy->SERVO_KP_INJCT,pp->hy->SERVO_KI_INJCT, INJDFLAG, (pact->ftaskstat & INTF_CEC) ?0 : 1, 
						HYDR_PRI(pp->hy->HYDR_PORT_INJCT),pp->md->INJ_SPD_UTI);
	
	if((HYDR_PORT(pp->hy->HYDR_PORT_HOLD)!=0x02)&& ChkSysCfg(CO4_APHP))   
	{
		ActSet_Da_Pres(pact, MC_CTR_MODE_DA, GetSysConfig()->DAPRES_SYSLMT,0, 0,
					0,0x02,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_INJCT),1);
	}
	else if (pp->mh->FL_INJ_PROV == 1)
	{		
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, tempflow,pp->mh->DAFLOW_INJRAMP2, pp->mh->DAFLOW_INJRAMP,
				0,INJ_PROV_DA_CH,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_INJCT),INJ_PROV_FWD);
	}

    ActSetFlowDot(old_flow_dot);
}

void SetInjectFwd0(PINJECT pp, PACT pact)
{
	int old_flow_dot;
	UI16 temppres,tempflow;

	/*通用新加20161008 LY*/
    if(GetOperMode() ==OM_PURG && ChkSysCfg(CO2_CHGX))
    {
		temppres =  pp->pa->DAPRES_INJPURGE;
		tempflow =  pp->pa->DAFLOW_INJPURGE;
    }
	else
	{
		temppres =  pp->pa->DAPRES_INJ1;	
		tempflow =  pp->pa->DAFLOW_INJ1;
	}

	InjectFwdVlv(pp, pact);	

	old_flow_dot = ActSetFlowDot(ChkSysCfg(CO5_INJFLACC));
	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, temppres,tempflow,pp->mh->DAPRES_INJRAMP,pp->mh->DAFLOW_INJRAMP,
						pp->mh->DAPRES_INJRAMP,pp->mh->DAFLOW_INJRAMP,pp->mh->INJ_HYDELAY, HYDR_PORT(pp->hy->HYDR_PORT_INJCT),
						pp->hy->HYDR_PUMPS_INJCT,pp->hy->HYDR_PUMPS_INJCT, pp->hy->SERVO_KP_INJCT,pp->hy->SERVO_KI_INJCT, INJDFLAG, (pact->ftaskstat & INTF_CEC) ? 0 : 1, 
						HYDR_PRI(pp->hy->HYDR_PORT_INJCT),pp->md->INJ_SPD_UTI);

	pp->injectstart_100us = Get100UsTick();
	if((HYDR_PORT(pp->hy->HYDR_PORT_HOLD)!=0x02)&& ChkSysCfg(CO4_APHP))
	{
		ActSet_Da_Pres(pact, MC_CTR_MODE_DA, GetSysConfig()->DAPRES_SYSLMT,0, 0,
					0,0x02,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_INJCT),1);
	}
	else if (pp->mh->FL_INJ_PROV == 1)
	{		
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, tempflow,pp->mh->DAFLOW_INJRAMP2, pp->mh->DAFLOW_INJRAMP,
				0,INJ_PROV_DA_CH,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_INJCT),INJ_PROV_FWD);
	}

	ActSetFlowDot(old_flow_dot);
}


void SetInjectFwd1(PINJECT pp, PACT pact)
{
	int old_flow_dot;
	UI16 temppres,tempflow;

    /*通用新加20161008 LY*/
    if(GetOperMode() ==OM_PURG && ChkSysCfg(CO2_CHGX))
    {
		temppres =  pp->pa->DAPRES_INJPURGE;
		tempflow =  pp->pa->DAFLOW_INJPURGE;
    }
	else
	{
		temppres =  pp->pa->DAPRES_INJ1;	
		tempflow =  pp->pa->DAFLOW_INJ1;
	}

	InjectFwdVlv(pp, pact);	

	old_flow_dot = ActSetFlowDot(ChkSysCfg(CO5_INJFLACC));
	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, temppres,tempflow,pp->mh->DAPRES_INJRAMP,pp->mh->DAFLOW_INJRAMP,
						pp->mh->DAPRES_INJRAMP,pp->mh->DAFLOW_INJRAMP,0, HYDR_PORT(pp->hy->HYDR_PORT_INJCT),
						pp->hy->HYDR_PUMPS_INJCT,pp->hy->HYDR_PUMPS_INJCT, pp->hy->SERVO_KP_INJCT,pp->hy->SERVO_KI_INJCT, INJDFLAG, (pact->ftaskstat & INTF_CEC) ? 0 : 1, 
						HYDR_PRI(pp->hy->HYDR_PORT_INJCT),pp->md->INJ_SPD_UTI);

	pp->injectstart_100us = Get100UsTick();
	if((HYDR_PORT(pp->hy->HYDR_PORT_HOLD)!=0x02)&& ChkSysCfg(CO4_APHP))
	{
		ActSet_Da_Pres(pact, MC_CTR_MODE_DA, GetSysConfig()->DAPRES_SYSLMT,0, 0,
					0,0x02,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_INJCT),1);
	}
	else if (pp->mh->FL_INJ_PROV == 1)
	{		
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, tempflow,pp->mh->DAFLOW_INJRAMP2, pp->mh->DAFLOW_INJRAMP,
				0,INJ_PROV_DA_CH,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_INJCT),INJ_PROV_FWD);
	}

    ActSetFlowDot(old_flow_dot);
}

void SetInjectFwd2(PINJECT pp, PACT pact)
{
	SetInjectFwdNum(pp, pact);
	
	if(pp->md->FL_INJFAST == 1)
		SetPOBit(pp->po->O_INJ_FST, PC_ON);

	if(pp->md->FL_INCACC == 1 && ChkOperMode(OM_AUTO))
		SetPOBit(pp->po->O_INJ_ACC, PC_ON);
}

void SetInjectFwd3(PINJECT pp, PACT pact)
{
	SetInjectFwdNum(pp, pact);
}

void SetInjectFwd4(PINJECT pp, PACT pact)
{
	SetInjectFwdNum(pp, pact);
}

void SetInjectFwd5(PINJECT pp, PACT pact)
{
	SetInjectFwdNum(pp, pact);
}

void SetInjectFwd6(PINJECT pp, PACT pact)
{
	SetInjectFwdNum(pp, pact);
}

void SetInjectFwdSlow(PINJECT pp, PACT pact)   //射出慢速启动段
{
	int old_flow_dot;

	InjectFwdVlv(pp, pact);	
	
	if(pp->pa->DAPRES_INJRAMPSLOW > 200)
		pp->pa->DAPRES_INJRAMPSLOW = 199;
	if(pp->pa->DAFLOW_INJRAMPSLOW > 200)
		pp->pa->DAFLOW_INJRAMPSLOW = 199;

	old_flow_dot = ActSetFlowDot(ChkSysCfg(CO5_INJFLACC));
	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_INJSLOW,pp->pa->DAFLOW_INJSLOW,pp->pa->DAPRES_INJRAMPSLOW,pp->pa->DAFLOW_INJRAMPSLOW,
						pp->pa->DAPRES_INJRAMPSLOW,pp->pa->DAFLOW_INJRAMPSLOW, 0, HYDR_PORT(pp->hy->HYDR_PORT_INJCT),
						pp->hy->HYDR_PUMPS_INJCT,pp->hy->HYDR_PUMPS_INJCT, pp->hy->SERVO_KP_INJCT,pp->hy->SERVO_KI_INJCT, INJDFLAG, (pact->ftaskstat & INTF_CEC) ? 0 : 1, 
						HYDR_PRI(pp->hy->HYDR_PORT_INJCT),pp->md->INJ_SPD_UTI);
    
	pp->injectstart_100us = Get100UsTick();
	if((HYDR_PORT(pp->hy->HYDR_PORT_HOLD)!=0x02)&& ChkSysCfg(CO4_APHP))
	{
		ActSet_Da_Pres(pact, MC_CTR_MODE_DA, GetSysConfig()->DAPRES_SYSLMT,0, 0,
					0,0x02,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_INJCT),1);
	}
	else if (pp->mh->FL_INJ_PROV == 1)
	{		
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->pa->DAFLOW_INJSLOW,pp->pa->DAFLOW_INJRAMPSLOW, pp->pa->DAFLOW_INJRAMPSLOW,
				0,INJ_PROV_DA_CH,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_INJCT),INJ_PROV_FWD);
	}

    ActSetFlowDot(old_flow_dot);
}

void InjectHoldVlv(PINJECT pp)
{
	//PHYDR     ph = &g_hydrout;
	if(pp->md->HYDY_WAY_SET == 1)
	{
		SetPOBit(GetSys()->po->HYD_COMB, PC_OFF);
	}
	ClampCoreFwdOff(pp->pclmp);
	
  	SetPOBit(pp->po->O_IN_SETU, PC_ON);
//	SetPOBit(pp->po->O_INJ_PRE, PC_ON);
//  	SetPOBit(pp->po->O_INJ_HOL, PC_ON);

	SetPOBit(GetSysPO(O_FST_SOL), PC_OFF);
	SetPOBit(pp->po->O_INJ_FST, PC_OFF);
	SetPOBit(pp->po->O_INJ_ACC, PC_OFF);
//	SetPOBit(pp->po->O_IN_INJE, PC_OFF);
//	SetPOBit(pp->po->O_INJ_CHR, PC_OFF);
//	SetPOBit(pp->po->O_CHG_INJC, PC_OFF);
	SetPOBit(GetSysPO(O_HYD_VLV2), PC_OFF);
//	SetPOBit(pp->po->O_INJ_FS2, PC_OFF);

//  if (ctrl_tbl[FL_INJHOLDVLV]==1)	// 09-11-24
	if(pp->md->FL_PASSVALUE == 2 || pp->md->FL_PASSVALUE == 3)		//dw
		SetPOBit(pp->po->O_PASS_VLV, PC_ON);
	else if(pp->md->FL_PASSVALUE == 1)
		SetPOBit(pp->po->O_PASS_VLV, PC_OFF);

	if (ChkOperMode(OM_AUTO) && ChkSysCfg(XF6_INJCLS))
		ClampClsVlvSet(pp->pclmp, TRUE);

	if(pp->pclmp != NULL && pp->pclmp->prbt != NULL)
		RobotInjectOnOff(pp->pclmp->prbt, TRUE);

	if(ChkOperMode(OM_AUTO))// && pp->md->FL_INJECTCOREOFF == 0)
		ClampCoreFwdPOSet(pp->pclmp, TRUE);

	if(NozzleIsReach(pp->pnzl) || ChkOperMode(OM_AUTO))
		SetNozzleFwdPO(pp->pnzl, PC_ON);
	SetPOBit(pp->po->O_INJ_SRV, PC_ON);
	//if(CUS_ID == CS_YANGXIN)             //广州阳鑫  射出保压时 开储料背压阀
	if(ChkSysCfg(CO2_INJCHG))
		SetPOBit(pp->po->O_CHR_BAC, PC_ON);
	if(ChkSysCfg(CO5_INJCHADA))        //博纳射出开环输出点关联保压开环
    {
	   if(pp->md->Hod_Open==1)
		  SetPOBit(pp->po->O_INJ_OPEN, PC_ON);
	   else
		  SetPOBit(pp->po->O_INJ_OPEN,PC_OFF);
    }

	SetPOBit(pp->po->O_IN_HOLD, PC_ON);
	SetPOBit(pp->po->O_INJ_SOL, PC_ON);
}

void SetInjectHoldNum(PINJECT pp, PACT pact)
{
	UI16 hydelay;
	int old_flow_dot;
	UI16 temppres,tempflow;

	WORD ind = GET_M_STEP_NO(pact->step - HOLD1);
	
	if(ind == 0)
		return;

	ind = ind - 1;
	
		/*通用新加20161008 LY*/
	if(GetOperMode() ==OM_PURG && ChkSysCfg(CO2_CHGX))
	{
		temppres =	pp->pa->DAPRES_INJPURGE;
		tempflow =	pp->pa->DAFLOW_INJPURGE;
	}
	else
	{
		temppres =	*(&(pp->pa->DAPRES_INJHOLD1) + ind);

		if (pp->md->HOLDSPD_SETMODE == 0)
		{
			tempflow =	*(&(pp->pa->DAFLOW_INJHOLD1) + ind);
		}
		else
		{
			tempflow =	pp->pa->DAFLOW_INJHOLD1;
		}
	}

	if(pp->mh->HOLD_HYDELAY > 9999)
		pp->mh->HOLD_HYDELAY = 0;
	hydelay = pp->mh->HOLD_HYDELAY;
	InjectHoldVlv(pp);
    old_flow_dot = ActSetFlowDot(ChkSysCfg(CO5_INJFLACC));

	//原来流量启动斜率，写成了压力启动斜率。而且原来有个很奇怪的现象，转保压的压力流量都关联到流量斜率。当code1=8（射出精度）去掉时，关联到压力斜率了。			DK 2014.1105
	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, temppres,tempflow,pp->mh->DAPRES_INJHOLDRAMP,pp->mh->DAFLOW_INJHOLDRAMP,	
						pp->mh->DAPRES_INJRAMP,pp->mh->DAFLOW_INJRAMP, (pact->ftaskstat & INTF_CEC) ? 0 : HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_HOLD),pp->hy->HYDR_PUMPS_HOLD,pp->hy->HYDR_PUMPS_HOLD,
						pp->hy->SERVO_KP_HOLD,pp->hy->SERVO_KI_HOLD, HOLDFLAG, !(pact->ftaskstat & INTF_CEC), HYDR_PRI(pp->hy->HYDR_PORT_HOLD),pp->pa->PER_DAFLOW_HOLD);

    
	if((HYDR_PORT(pp->hy->HYDR_PORT_HOLD)!=0x02)&& ChkSysCfg(CO4_APHP))
	{
		ActSet_Da_Pres(pact, MC_CTR_MODE_DA, GetSysConfig()->DAPRES_SYSLMT,0, 0,
					0,0x02,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_INJCT),1);
	}
	else if (pp->mh->FL_INJ_PROV == 1)
	{		
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, tempflow,pp->mh->DAFLOW_INJHOLDRAMP, pp->mh->DAFLOW_INJRAMP,
				0,INJ_PROV_DA_CH,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_INJCT),INJ_PROV_FWD);
	}

    ActSetFlowDot(old_flow_dot);
}

void SetInjectHold1(PINJECT pp, PACT pact)
{
	InjectHoldVlv(pp);
	SetInjectHoldNum(pp, pact);
}

void InjectVlvOff(PINJECT pp)
{
	if(pp->mh->TMDLY_VLV_OFFINJEND > 500)
		pp->mh->TMDLY_VLV_OFFINJEND = 0;
	SetPOBit(pp->po->O_INJ_FST, PC_OFF);
	SetPOBit(pp->po->O_INJ_ACC, PC_OFF);
	SetPOBit(pp->po->O_IN_HOLD, PC_OFF);
	SetPOBit(pp->po->O_PASS_VLV, PC_OFF);
	SetPOBit(pp->po->O_INJ_SRV, PC_OFF);	
	SetPOBit(GetSysPO(O_FST_SOL), PC_OFF);
	SetPOBit(GetSysPO(O_HYD_VLV2), PC_OFF);
	SetPOBit(GetSysPO(O_HYD_COMB), PC_OFF);
	SetPOBit(GetSysPO(O_CP_PUMP), PC_OFF);
	
	if(ChkOperMode(OM_AUTO) && ChkSysCfg(XF6_INJCLS))
		ClampClsVlvSet(pp->pclmp, FALSE);
	
	if(pp->pclmp != NULL && pp->pclmp->prbt != NULL)
		RobotInjectOnOff(pp->pclmp->prbt, FALSE);
	
	ClampCoreFwdOff(pp->pclmp);

	if(!ChkOperMode(OM_AUTO) || ChkSysCfg(CO3_NFOF))
		SetNozzleFwdPO(pp->pnzl, FALSE);
	
	SetPOBit(pp->po->O_CHR_BAC, PC_OFF);
	
	if(CUS_ID==CS_BONA)
	   SetPOBitDelay(pp->po->O_IN_SETU, PC_OFF, pp->mh->TMDLY_VLV_OFFINJEND+10);
	else
	   SetPOBit(pp->po->O_IN_SETU, PC_OFF); 
	SetPOBitDelay(pp->po->O_INJ_OPEN, PC_OFF, pp->mh->TMDLY_VLV_OFFINJEND);
}

void InjectHydrCls(PINJECT pp, PACT pact)
{
	HYDR_CLS(pp->mh->DAPRES_INJOFFRAMP, pp->mh->DAFLOW_INJOFFRAMP);	
}

void InjectOffProc(PINJECT pp, PACT pact, BOOL bdelay)
{	
	if(pp->mh->TMDLY_VLV_OFFINJEND > 500)
		pp->mh->TMDLY_VLV_OFFINJEND = 0;

	if(pp->mh->TM_INJSPC_CLSDLY > 500)
		pp->mh->TM_INJSPC_CLSDLY = 0;
	InjectVlvOff(pp);	
	SetPOBitDelay(pp->po->O_INJ_SOL, PC_OFF, pp->mh->TMDLY_VLV_OFFINJEND);
    SetPOBitDelay(pp->po->O_INJ_OPEN, PC_OFF, pp->mh->TMDLY_VLV_OFFINJEND);

	if(pp->md->HYDY_WAY_SET == 1)
	{
		SetPOBit(GetSys()->po->HYD_COMB, PC_OFF);
	}
	
	if (ChkSysCfg(CO4_INOF) && bdelay)
	{
		ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, 0,0,pp->mh->TMDLY_VLV_OFFINJEND,pp->mh->TMDLY_VLV_OFFINJEND,
							pp->mh->TMDLY_VLV_OFFINJEND,pp->mh->TMDLY_VLV_OFFINJEND,0,HYDR_PORT(pp->hy->HYDR_PORT_HOLD),1,1,
							pp->hy->SERVO_KP_HOLD,pp->hy->SERVO_KI_HOLD, 0,0,HYDR_PRI(pp->hy->HYDR_PORT_HOLD),pp->md->INJ_SPD_UTI);

    	if(pp->hy->HYDR_PORT_HOLD != pp->hy->HYDR_PORT_INJCT)
			ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, 0,0,pp->mh->TMDLY_VLV_OFFINJEND,pp->mh->TMDLY_VLV_OFFINJEND,
								pp->mh->TMDLY_VLV_OFFINJEND,pp->mh->TMDLY_VLV_OFFINJEND,0,HYDR_PORT(pp->hy->HYDR_PORT_INJCT),1,1,
								pp->hy->SERVO_KP_INJCT,pp->hy->SERVO_KI_INJCT, 0,0,HYDR_PRI(pp->hy->HYDR_PORT_INJCT),pp->md->INJ_SPD_UTI);
    }
	else
		InjectHydrCls(pp, pact);
	
	if((HYDR_PORT(pp->hy->HYDR_PORT_HOLD)!=0x02)&& ChkSysCfg(CO4_APHP))
	{
		HYDR_CLS_Da_P(2, 0,1);
	}
	else if (pp->mh->FL_INJ_PROV == 1)
	{		
		HYDR_CLS_Da_F(INJ_PROV_DA_CH,pp->mh->DAFLOW_INJOFFRAMP,INJ_PROV_FWD);
	}
}

void InjectOff(PINJECT pp, PACT pact)
{
	InjectOffProc(pp, pact, FALSE);
}

void InjectOffDelay(PINJECT pp, PACT pact)
{
	InjectOffProc(pp, pact, FALSE);
}

BOOL InjCushErr(PINJECT pp)
{
	BOOL   injerr=FALSE;
	int    maxlmt,minlmt,injposn;

	if(!pp->mh->FL_CUSHIONFUNCTION)
		return(FALSE);

	if(GetMeterLenByCh(pp->mh->INJ_METERCH))
	{
	    
		if(ChkSysCfg(CO2_ICURINJEND))
			injposn = pp->st->ADPOSI_INJHOLDEND;	 //当前射出监控位置
		else
			injposn = pp->st->ADPOSI_INJCUSHION;
		if(ChkSysCfg(CO2_SAVC1))         //射出监控不采用上下偏差的方式
		{
			maxlmt = pp->md->ADPOSI_INJPOSUPCUSHION;
			minlmt = pp->md->ADPOSI_INJPOSNEGCUSHION;
		}
		else
		{
			maxlmt = pp->st->ADPOSI_INJCUSHION_MAX + pp->md->ADPOSI_INJPOSUPCUSHION;//射出监控位置正向允许偏移
			if(pp->md->ADPOSI_INJPOSNEGCUSHION > pp->st->ADPOSI_INJCUSHION_MAX)
				minlmt = 0;
			else  
				minlmt = pp->st->ADPOSI_INJCUSHION_MAX - pp->md->ADPOSI_INJPOSNEGCUSHION;	//射出监控位置负向允许偏移
		}

		if (injposn < minlmt || injposn > maxlmt)
			injerr = TRUE;	
	}
	else
	{
		injerr = (!(TestPI(pp->pi->I_INJ_2ND)));	
	}
	
	return(injerr);			
}

void InjectComplete(PINJECT pp)
{
	if(ChkSysCfg(CO43_INJHOD))
  		pp->st->ADPOSI_INJHOLDEND = INT_ABS(pp->InjHoldPosEnd);
	else
  		pp->st->ADPOSI_INJHOLDEND  = INT_ABS(GetMeterPosByCh(pp->mh->INJ_METERCH));

	pp->st->HOLD_MOVPOSITION = INT_ABS(pp->st->ADPOSI_INJCUSHION - pp->st->ADPOSI_INJHOLDEND);

	
	pp->st->TM_HOLD_H = pp->st->TM_HOLD_RT_H;
	pp->st->TM_HOLD_L = pp->st->TM_HOLD_RT_L;
		
	if(CUS_ID==CS_BONA)
	   SetPOBit(pp->po->O_IN_SETU,PC_OFF);
	if(ChkOperMode(OM_AUTO) &&AutoAlarmIsStart() == 1)
	{
		if(InjCushErr(pp))                 //射出监控失败
		{
			SET_PART_ERR(pp, ER0_CUSH);
			
			if(pp->pclmp != NULL && pp->pclmp->prbt != NULL)
				RobotInjectErr(pp->pclmp->prbt);
		}
	}
    InjectCmd( pp, CMD_QC_PARA, 1);

	if (pp->mh->INJ_SPC_FUNC_CLS == 0)
	{
		EndActTsk(((PACT)(pp->act + OS_NEEDLEVALVE- 1))->ptsk, TRUE);
	}	

    if (ChkSysCfg(XF4_SAVC2))
	{
        PartDataTx((PPART)pp, ST_ID,1,GET_IND(PINJECT_STATE, ADPOSI_INJHOLDEND),TRUE);
	}
}

void InjectChkEnd(PINJECT pp, PACT pact)
{
	WORD m = GET_M_STEP_NO(pact->next_step - INJCT1);
	if (pact->step >= INJCT1 && pact->step < INJCTEND)
	{
		m--;
		int tmp = GetMeterLenByCh(pp->mh->INJ_METERCH);
		if((tmp <= 0 && MoveLocationNMControlProc(pp->pi->I_INJ_2ND))&&((m==0)||(m==1)))
		{
			//非时间方式，直接转保压
			if(pp->md->FL_TURNTOHOLD !=3)
			{
				ACT_STEP_NEW(INJCTEND);
			}
		}

		if ((m+1) > pp->inj_real_ch)
		{			
			ACT_STEP_NEW(INJCTEND);
		}

		if ((pp->injectmode == 0) && InjTMPressChk(pp, pact))
		{
			ACT_STEP_NEW(INJCTEND);			
		}
	}	
}

void HoldChkEnd(PINJECT pp, PACT pact)
{
	WORD m = GET_M_STEP_NO(pact->step - HOLD1);
	if(pact->step >=  HOLD1 && pact->step < HOLDEND)
	{
		m--;
		if(m >=pp->md->SEC_HOLD)  //不判断保压时间是否为0，按照设定段数，使用的段数每段都执行 20160429
		{
			ACT_STEP_NEW(HOLDEND);
		}
	}
}

ACT_RET InjectHold(PINJECT pp, PACT pact)
{
	PMACHINE pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1));
	UI32 tm_hold;
	ACT_RET ret = ACT_OK;
	int m;

	if(ActIODelayLimit(pp->po->O_INJ_SOL, TRUE))
	{
	    return ACT_ERR_ABORT;
	}

	pact->spc_data = 1;
		
	switch(pact->step)
	{
		case HOLD1:				
			if (pp->md->SEC_HOLD < 2 || pp->md->SEC_HOLD > 10)
			{
				pp->md->SEC_HOLD  = 2;
			}

			Init_inj_time(pp);
			ACT_START();
		case HOLD1 + 1:
			if(STEP_MS_CHK(pp->HOLD_TM_N[0]))
			{
				ACT_STEP_NEW(HOLD2);
			}
			CHK_SET_MOV(SetInjectHoldNum);
			break;
		case HOLD2:
		case HOLD2 + 1:
			if(STEP_MS_CHK(pp->HOLD_TM_N[1]))
			{
				//无电子尺时，保压只做两段
				if (GetMeterLenByCh(pp->mh->INJ_METERCH) <= 0 || (pp->md->SEC_HOLD <= 2))
				{
					ACT_STEP_NEW(HOLDEND);
				}
				else
				{
					ACT_STEP_NEW(HOLD3);
				}
			}
			CHK_SET_MOV(SetInjectHoldNum);
			break;
		case HOLD3:				
		case HOLD3 + 1:
			if(STEP_MS_CHK(pp->HOLD_TM_N[2]))
			{
				if (pp->md->SEC_HOLD <= 3)
				{
					ACT_STEP_NEW(HOLDEND);
				}
				else
				{
					ACT_STEP_NEW(HOLD4);
				}
			}
			CHK_SET_MOV(SetInjectHoldNum);
			break;	
		case HOLD4:
		case HOLD4 + 1:
			if(STEP_MS_CHK(pp->HOLD_TM_N[3]))
			{
				if (pp->md->SEC_HOLD <= 4)
				{
					ACT_STEP_NEW(HOLDEND);
				}
				else
				{
					ACT_STEP_NEW(HOLD5);
				}
			}
			CHK_SET_MOV(SetInjectHoldNum);
			break;
		case HOLD5:
		case HOLD5 + 1:
			if(STEP_MS_CHK(pp->HOLD_TM_N[4]))
			{
				if (pp->md->SEC_HOLD <= 5)
				{
					ACT_STEP_NEW(HOLDEND);
				}
				else
				{
					ACT_STEP_NEW(HOLD6);
				}
			}
			CHK_SET_MOV(SetInjectHoldNum);
			break;
		case HOLDEND:
			if(pp->mh->TMDLY_VLV_OFFINJEND > 500)
				pp->mh->TMDLY_VLV_OFFINJEND = 0;
			ActSetMovTM(pact->ptsk, pp->mh->TMDLY_VLV_OFFINJEND);
			ACT_STEP_NEW(HOLDEND + 1);
			break;
		case HOLDEND + 1:
            //HOLDEND+1与HOLDEND+2组成了一个关闭，不要删除，对5模式的加速度限制有意义
            ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, 0,0,pp->mh->DAPRES_INJOFFRAMP, pp->mh->DAFLOW_INJOFFRAMP,
                    pp->mh->DAPRES_INJOFFRAMP, pp->mh->DAFLOW_INJOFFRAMP,0,HYDR_PORT(pp->hy->HYDR_PORT_HOLD),
                    pp->hy->HYDR_PUMPS_HOLD,pp->hy->HYDR_PUMPS_HOLD,   pp->hy->SERVO_KP_HOLD,pp->hy->SERVO_KI_HOLD, INJDFLAG,0,
                    HYDR_PRI(pp->hy->HYDR_PORT_HOLD),pp->md->INJ_SPD_UTI);
            
			ACT_STEP_NEW(HOLDEND + 2);
			break;
		case HOLDEND +2:				
			InjectOffDelay(pp, pact);
			ACT_STEP_NEW(HOLDEND + 3);
			break;				
		case HOLDEND +3:
			if(GetPOBit(pp->po->O_INJ_SOL) <= 1)                //射出阀关闭
			{
				if(pp->mh->TMDLY_INJEND > 200)
					pp->mh->TMDLY_INJEND = 0;
				SetPOBit(pp->po->O_INJ_OPEN,PC_OFF);
				ACT_STEP_NEW(HOLDEND + 4);
				ActSetMovTM(pact->ptsk, pp->mh->TMDLY_INJEND);
			}
			break;
		case HOLDEND + 4:
			if(ACT_STEP_100US >= ((UI32)pp->mh->TMDLY_INJEND)*100)
			{
				InjectComplete(pp);
				ret = ACT_END;
			}
			break;
		//5段保压以上
		default:
			m = (pact->step -HOLD1)/M_STEP_GAP;

			if(STEP_MS_CHK(pp->HOLD_TM_N[m]))
			{
				if(m + 1 >=pp->md->SEC_HOLD)  //不判断保压时间是否为0，按照设定段数，使用的段数每段都执行 20160429
					ACT_STEP_NEW(HOLDEND);
				else 
					ACT_STEP_NEW(HOLD1 + (m + 1) *M_STEP_GAP);
			}

			CHK_SET_MOV(SetInjectHoldNum);
			break;
	}

	if (pact->step < HOLDEND)
	{
		if(ChkSysCfg(CO1_INTM))
		{
			tm_hold = ACT_RUN_MS;
		}
		else
		{
			tm_hold = ACT_RUN_MS/10;
		}

		pp->st->TM_HOLD_RT_L = tm_hold&0xFFFF;
		pp->st->TM_HOLD_RT_H = (tm_hold>>16)&0xFFFF;
	}	
	return ret;
}


void InjectTmUpdata(PINJECT pp, PACT pact)
{
	WORD m = GET_M_STEP_NO(pact->step - INJCT1);
	
	if (ChkSysCfg(CO5_INJET))		//射出时间每段显示 
	{
		if (pact->step >= INJCT1 && pact->step < INJCTEND)
		{
			m--;
			
			if (pact->step == pact->next_step)
				*(&(pp->st->TM_INJE1)+m)= Cal100UsTo10MsUI16(pact->step_100us);
		}		
	}
}

UI16 InjectRunByTime(PINJECT pp, PACT pact)
{
	WORD m = GET_M_STEP_NO(pact->step - INJCT1);
	WORD nextstep = FALSE;
	if(pp->injectmode == 1 && m > 0)
	{					
		m--;
		if((m==0)&&(ChkSysCfg(CO2_INJCLS) && GetSysMold()->FL_INJMOLDIN ==2 &&(ChkOperMode(OM_AUTO))))
		{//时间注射下，兼容压塑功能，第一段注射走压塑前射出一段计时
			if(STEP_MS_CHK_INJ(pp->md->TM_INJMOLDINTIME))
			{
				nextstep = TRUE;
			}
		}
		else
		{	
			if (ACT_STEP_100US >= pp->INJ_TM_N[m]*10)
			//if(STEP_MS_CHK_INJ(pp->INJ_TM_N[m]))	   //修改动作时间，只是在射出时调用，该动作时间函数会判断射出时间的精度 DK 2014.1105	   
			{										   //如果在射出是时间模式，则压塑在走的是射出一段的时间。	   DK 2014.1105
                nextstep = TRUE;
			}	
		}
		
		if(nextstep)
		{//满足下一段条件
			if((m+1)>=pp->md->SEC_INJ)
    		{
				ACT_STEP_NEW(INJCTEND);
        	}
		 	else
		 	{
		 		ACT_STEP_NEW((MAIN_STEP(pact->step) + 1)*M_STEP_GAP);	
		 	}
		}
		return 1;
	}
	else
	{		
		if ((!ChkSysCfg(CO43_RSV)) && (pp->md->FL_TURNTOHOLD == 3))
		{			
			if ((m == pp->inj_real_ch))
			{
				if (ACT_RUN_MS < pp->INJ_TM_N[0])
				{
					return 1;
				}
			}
		}
		
		return 0;
	}
}

/*******************************************************************************************
 *Brief			:检查判断注射实际段数
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20200403
********************************************************************************************/
void InjectRealChChk(PINJECT pp, PACT pact)
{
	UI16 i = 0;
	pp->inj_real_ch = 0;

	if (pp->injectmode == 0)
	{
		for (i = 0; i < 10; i++)
		{
			if (((i+1) > pp->md->SEC_INJ) || (*(&(pp->pa->ADPOSI_INJ1END)+i) == 0 && *(&(pp->pa->DAPRES_INJ1)+i) == 0 && *(&(pp->pa->DAFLOW_INJ1)+i) == 0))   //当压力，流量，位置都为0时，当前射出直接结束  20170503 
			{			
				break;
			}
			pp->inj_real_ch++;
		}
	}
	else
	{
		pp->inj_real_ch = pp->md->SEC_INJ;
	}
}

ACT_RET InjectIn(PINJECT pp, PACT pact)
{
	PMACHINE pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1));
    PCLAMP pclmp= (PCLAMP)GetPartByID(MK_PART(CLAMP_ID, 1));

	int i;
	int	tmpp;
	int tmp,m;
	UI32 inj_time;		//射出整个动作的实际运行时间	20200224

	ACT_RET ret = ACT_OK; 
	
	if(ActIODelayLimit(pp->po->O_INJ_SOL, TRUE))
	{
	    return ACT_ERR_ABORT;
	}
	switch(pact->step)
	{
		case INIT1_STEP:
			Init_inj_time(pp);
		    pp->st->INJECT_MAXPRESS = 0;
		    pp->st->HOLD_MAXPRESS = 0;
            if(ChkOperMode(OM_AUTO))	//自动模式下射出动作开始后，压料缸动作按键无效，手动压料缸动作结束(lzh 2022.1.4)
			{
				pp->pchgprs->chgprsflag = 1;
			}

			if(pp->md->FL_TURNTOHOLD == 5)
			{
				pp->injectmode = 1;
			}
			else
			{
				pp->injectmode = 0;
			}
			
			for(i=0;i<10;i++)
			{
				*(&(pp->st->TM_INJE1)+i) = 0;
			}
			InjectRealChChk(pp, pact);	//初始化判断注射实际段数，屏蔽设定0的情况
			if((ret = InjectPre(pp, pact)) == ACT_OK)		//条件判断
			{
				ACT_STEP_NEW(INIT1_STEP + 1);
			}
			else
				return ret;
			break;
			
		case INIT1_STEP + 1:
			if(InjectPurgeGuard(pp, pact) == ACT_OK)		//射出防护罩
			{
				if(pp->mh->TMDLY_INJBEFORE > 999)
					pp->mh->TMDLY_INJBEFORE = 0;
				if(ACT_STEP_100US > ((UI32)pp->mh->TMDLY_INJBEFORE)*100)
				{
					if((pp->needle_valve_allowinj > 0) || (pp->mh->FL_INj_SPC == 0))
					{
						pp->needle_valve_allowinj = FALSE;
						/*
                        特殊修改：整套系统的液压延迟存在相应bug，当上一个动作斜率未结束时下一个动作
                        如果存在液压延迟的话，在液压延迟阶段的指令压力流量为下个动作运行时斜率所减到
                        的位置。在这里这样写，可以让注射液压延迟真是生效。 -zt 20220315
                        */
						//SetInjectFwd0(pp, pact);                        
						InjectFwdVlv(pp, pact);		
		 				ACT_STEP_NEW(INIT1_STEP + 2);
					}
				}
			}
			break;
		case INIT1_STEP + 2:	
			if(pp->mh->INJ_HYDELAY > 9999)
				pp->mh->INJ_HYDELAY = 0;
			if(ACT_STEP_MS>pp->mh->INJ_HYDELAY)
	 			ACT_STEP_NEW(INIT1_STEP + 3);
			break;
		case INIT1_STEP + 3:
			if (ChkOperMode(OM_AUTO) && ChkSysCfg(XF6_INJDBL) && pp->md->ADPOSI_CHGFLOWMOLD > 0 && pp->md->ADPOSI_CHGFLOWMOLD < GetMeterPosByCh(pp->mh->INJ_METERCH))
			{
				//特殊 使用二次射出
				ACT_STEP_NEW(INJTWIC);
			}
			else
			{
				if (ChkSysCfg(CO3_WSTO) && pp->pa->TM_INJSLOW > 0 && pp->INJ_TM_N[0] > 0)
				{
					pp->InjPosstart = INT_ABS(GetMeterPosByCh(pp->mh->INJ_METERCH));
					ACT_STEP_NEW(STAGE_SPEC3_STEP);
				}							 
				else
					ACT_STEP_NEW(INJCT1);
			}
			break;
		case INJTWIC:
			pp->twice_inj_pos = GetMeterPosByCh(pp->mh->INJ_METERCH) - pp->md->ADPOSI_CHGFLOWMOLD;
		case INJTWIC + 1:  
			if(MoveLocationMeterControlProc(pp->twice_inj_pos, GetMeterPosByCh(pp->mh->INJ_METERCH)))
				ACT_STEP_NEW(INJCT1);		//特殊 使用二次射出, 此时应嵌入储料动作。
				
			CHK_SET_MOV(SetInjectFwd1);
			/*时间保护*/
			break;

		case STAGE_SPEC3_STEP:
		case STAGE_SPEC3_STEP + 1:
			tmpp = GetMeterLenByCh(pp->mh->INJ_METERCH);
			if(pp->pa->TM_INJSLOW > 2000)  
				pp->pa->TM_INJSLOW = 2000;
			if((tmpp>0 && MoveLocationMeterControlProc(pp->InjPosstart,(GetMeterPosByCh(pp->mh->INJ_METERCH) + pp->pa->ADPOSI_INJSLOW)))
				|| ACT_STEP_100US >= (UI32)pp->pa->TM_INJSLOW*10)
			{
				ACT_STEP_NEW(INJCT1);
			}
			CHK_SET_MOV(SetInjectFwdSlow);
			break;
			
		case INJCT1:
			pp->st->ADPOSI_INJSTART = INT_ABS(GetMeterPosByCh(pp->mh->INJ_METERCH));
			pp->InjHoldPosEnd = pp->st->ADPOSI_INJSTART;	//	记录残料位置的开始值
			pp->injectstart_100us = Get100UsTick();
			SetInjectTime(pp, pact);
			ACT_START();
		case INJCT1 + 1:
			tmp = GetMeterLenByCh(pp->mh->INJ_METERCH);
			
			if (ChkSysCfg(CO2_INJCLS) && GetSysMold()->FL_INJMOLDIN ==2 &&(ChkOperMode(OM_AUTO)))
			{
				if( STEP_MS_CHK_INJ(pp->md->TM_INJMOLDINTIME))
				{
					ACT_STEP_NEW(INJCT2);	
					InjectChkEnd(pp, pact);				
				}
			}
            else if((ChkSysCfg(CO2_INJCLS))&&(GetSysMold()->FL_INJMOLDIN == 4) &&(ChkOperMode(OM_AUTO)))
			{
				if(MoveLocationMeterControlProc(pp->md->ADPOSI_INJMOLDIN_INJ1, GetMeterPosByCh(pp->mh->INJ_METERCH)))
				{
					ACT_STEP_NEW(INJCT2);	
					InjectChkEnd(pp, pact);				
				}
			}
			else if(!InjectRunByTime(pp,pact)&&
			((tmp > 0 && MoveLocationMeterControlProc(pp->pa->ADPOSI_INJ1END, GetMeterPosByCh(pp->mh->INJ_METERCH)) ||
  			(tmp <= 0 && MoveLocationNMControlProc(pp->pi->I_INJ_1ST)))))	 //压塑在射出一段后，走时间
			{
	 			ACT_STEP_NEW(INJCT2);
				InjectChkEnd(pp, pact);
                if((ChkOperMode(OM_AUTO))&&(ChkSysCfg(CO3_BAKE))&&(pclmp->md->FL_EXHAUSTMODE==0||pclmp->md->FL_EXHAUSTMODE==7))			//排气选择不使用，做排气次数
				{
					if((pclmp->md->CN_EXHAUST>=0)&&(pp->pclmp->md->TMDLY_BAKEAFTEREXHAUST>0))
					{
					   	if((ChkOperMode(OM_AUTO))&&(ChkSysCfg(CO3_BAKE)))
					 	{
					 		//if(pclmp->md->FL_EXHAUSTMODE==0)
						 	{
								ForkAct(pclmp->act +OS_EXHAST-1, SUSPEND_FORK, pact->ptsk, EXHAST_CNT_1, 0);//排气
						 	}
					 	}
					}
				}
			}			

			CHK_SET_MOV(SetInjectFwd1);
			break;

		/*射出一段完成，如果需要的话, 此时应插入压塑嵌入动作*/
		case INJCT2:
			SetInjectTime(pp, pact);
		case INJCT2 + 1:
			tmp = GetMeterLenByCh(pp->mh->INJ_METERCH);
			if(!InjectRunByTime(pp,pact) && (tmp > 0) && MoveLocationMeterControlProc(pp->pa->ADPOSI_INJ2END, GetMeterPosByCh(pp->mh->INJ_METERCH)))
			{
				ACT_STEP_NEW(INJCT3);
				InjectChkEnd(pp, pact);
                if((ChkOperMode(OM_AUTO))&&(ChkSysCfg(CO3_BAKE))&&(pclmp->md->FL_EXHAUSTMODE==0||pclmp->md->FL_EXHAUSTMODE==7))			//排气选择不使用，做排气次数
				{
					if(pclmp->md->CN_EXHAUST>=1&&pp->pclmp->md->TMDLY_BAKEAFTEREXHAUST2>0)
					{
						if((ChkOperMode(OM_AUTO))&&(ChkSysCfg(CO3_BAKE)))
						{
							//if(pclmp->md->FL_EXHAUSTMODE==0)
							{
								ForkAct(pclmp->act +OS_EXHAST-1, SUSPEND_FORK, pact->ptsk, EXHAST_CNT_2, 0);//排气
							}
						}
					}
				}
			}
			CHK_SET_MOV(SetInjectFwd2);
			break;

		case INJCT3:
		case INJCT3 + 1:
			tmp = GetMeterLenByCh(pp->mh->INJ_METERCH);
			if(!InjectRunByTime(pp,pact) && (tmp > 0) && MoveLocationMeterControlProc(pp->pa->ADPOSI_INJ3END, GetMeterPosByCh(pp->mh->INJ_METERCH)))
			{
				ACT_STEP_NEW(INJCT4);
				InjectChkEnd(pp, pact);
                if((ChkOperMode(OM_AUTO))&&(ChkSysCfg(CO3_BAKE))&&(pclmp->md->FL_EXHAUSTMODE==0||pclmp->md->FL_EXHAUSTMODE==7))			//排气选择不使用，做排气次数
				{
					if(pclmp->md->CN_EXHAUST>=2&&pp->pclmp->md->TMDLY_BAKEAFTEREXHAUST3>0)
					{
						if((ChkOperMode(OM_AUTO))&&(ChkSysCfg(CO3_BAKE)))
						{
							//if(pclmp->md->FL_EXHAUSTMODE==0)
							{
								ForkAct(pclmp->act +OS_EXHAST-1, SUSPEND_FORK, pact->ptsk, EXHAST_CNT_3, 0);//排气
							}
						}
					}
				}
			}
			CHK_SET_MOV(SetInjectFwd3);
			break;
			
		case INJCT4:
		case INJCT4 + 1:
			tmp = GetMeterLenByCh(pp->mh->INJ_METERCH);
			if(!InjectRunByTime(pp,pact) && (tmp > 0) && MoveLocationMeterControlProc(pp->pa->ADPOSI_INJ4END, GetMeterPosByCh(pp->mh->INJ_METERCH)))
			{
				ACT_STEP_NEW(INJCT5);
				InjectChkEnd(pp, pact);
                if((ChkOperMode(OM_AUTO))&&(ChkSysCfg(CO3_BAKE))&&(pclmp->md->FL_EXHAUSTMODE==0||pclmp->md->FL_EXHAUSTMODE==7))			//排气选择不使用，做排气次数
				{
					if(pclmp->md->CN_EXHAUST>=3&&pp->pclmp->md->TMDLY_BAKEAFTEREXHAUST4>0)
					{
						if((ChkOperMode(OM_AUTO))&&(ChkSysCfg(CO3_BAKE)))
						{
							//if(pclmp->md->FL_EXHAUSTMODE==0)
							{
								ForkAct(pclmp->act +OS_EXHAST-1, SUSPEND_FORK, pact->ptsk, EXHAST_CNT_4, 0);//排气
							}
						}
						
					}
				}
			}
			CHK_SET_MOV(SetInjectFwd4);
			break;
		case INJCT5:
		case INJCT5 + 1:
			tmp = GetMeterLenByCh(pp->mh->INJ_METERCH);
			if(!InjectRunByTime(pp,pact) && (tmp > 0) && MoveLocationMeterControlProc(pp->pa->ADPOSI_INJ5END, GetMeterPosByCh(pp->mh->INJ_METERCH)))
			{
				ACT_STEP_NEW(INJCT6);
				InjectChkEnd(pp, pact);
			}
			CHK_SET_MOV(SetInjectFwd5);
			break;
		case INJCT6:
		case INJCT6 + 1:
			tmp = GetMeterLenByCh(pp->mh->INJ_METERCH);
			if(!InjectRunByTime(pp,pact) && (tmp > 0) && MoveLocationMeterControlProc(pp->pa->ADPOSI_INJ6END, GetMeterPosByCh(pp->mh->INJ_METERCH)))
			{
				ACT_STEP_NEW(INJCT7);
				InjectChkEnd(pp, pact);
			}
			CHK_SET_MOV(SetInjectFwd6);
			break;
		//	7~10段在default
        case INJREDUCE:
		case INJREDUCE+1:
			ACT_STEP_NEW(INJCTEND);
			break;
			
		case INJCTEND:			
			if(GetMeterLenByCh(pp->mh->INJ_METERCH) > 0)
			{
				pp->st->ADPOSI_INJCUSHION = INT_ABS(GetMeterPosByCh(pp->mh->INJ_METERCH));
			}
			if(ChkSysCfg(CO1_INTM))
			{
				inj_time = ACT_RUN_MS;
			}
			else
			{
				inj_time = ACT_RUN_MS/10;
			}
			
			pp->st->TM_INJ_L = (UI16)(inj_time & 0x0ffff);
			pp->st->TM_INJ_H = (UI16)((inj_time >> 16) & 0xffff);

            pp->st->TM_INJ2_L = pp->st->TM_INJ_L;
            pp->st->TM_INJ2_H = pp->st->TM_INJ_H;

			if(ACT_RUN_MS > 4)
			{
				pp->st->INJECT_AVERAGESPEED =((UI32) INT_ABS(pp->st->ADPOSI_INJSTART - pp->st->ADPOSI_INJCUSHION)*1000)/(ACT_RUN_MS+ 1);						
			}
			else
			{
				pp->st->INJECT_AVERAGESPEED = 0;
			}
			
			pp->st->SPEED_TOHOLD = pp->st->INJECT_SPEED;
			pp->st->DAPRES_TOHOLD = GetPumpPress(HYDR_PORT(pp->hy->HYDR_PORT_INJCT));

			if (ChkSysCfg(XF4_SAVC2))
			{
                PartDataTx((PPART)pp, ST_ID,20,GET_IND(PINJECT_STATE, ADPOSI_INJSTART),TRUE);
                PartDataTx((PPART)pp, ST_ID,2,GET_IND(PINJECT_STATE, TM_INJ2_L),TRUE);
			}				
			
			ACT_STEP_NEW(HOLD1);
			break;  
			
 		default:
			m = GET_M_STEP_NO(pact->step - INJCT1) - 1;
			if(MoveLocationMeterControlProc(*(&(pp->pa->ADPOSI_INJ1END) + m),  GetMeterPosByCh(pp->mh->INJ_METERCH)))
			{
				ACT_STEP_NEW((MAIN_STEP(pact->step) + 1)*M_STEP_GAP);
				InjectChkEnd(pp, pact);
			}

			CHK_SET_MOV_M(SetInjectFwdNum, m);
			break;
   	}

	if(ChkSysCfg(CO1_INTM))
	{
		inj_time = ACT_RUN_MS;
	}
	else
	{
		inj_time = ACT_RUN_MS/10;
	}
	
	pp->st->TM_INJ_RT_L = inj_time&0xFFFF;
	pp->st->TM_INJ_RT_H = (inj_time>>16)&0xFFFF;

	return ACT_OK;
}  


ACT_RET InjectBackgrd(PINJECT pp, PACT pact)         //inject background work 
{
	int tmp;

    if(!InjTempChkSimple(pp))
		return ACT_ERR_ABORT;

	if(ChkSysCfg(CO11_SAFEINJ))    //2014-08-04  射出检测安全门
	{
    	if((FSafeDoorIsCloseErrChk(pp->pclmp->psfdr)))
			return ACT_ERR_ABORT;
	}
	
	InjectTmUpdata(pp ,pact);	//注射各段实时计时
	InjectChkEnd(pp, pact);		//注射动作结束条件判定

	if(pact->step > INJCT1)
	{
		tmp = GetMeterPosByCh(pp->mh->INJ_METERCH);
		pp->InjHoldPosEnd = (pp->InjHoldPosEnd < tmp) ? pp->InjHoldPosEnd : tmp;
	}

	m_speed = pp->st->INJECT_SPEED;
	return ACT_OK;
}

ACT_RET InjectProc(PACT pact, UI32 para)
{
	PINJECT pp = (PINJECT)(pact->part);
	ACT_RET ret;

	if(pp == NULL)
		return ACT_ERR_ABORT;

	if(CUS_ID==CS_BONA)
       SetPOBit(pp->po->O_IN_SETU,PC_ON);
	if(pact->step < HOLD1 || GET_M_STEP(pact->step) ==STAGE_SPEC3_STEP||GET_M_STEP(pact->step) == INJTWIC||GET_M_STEP(pact->step) == INJREDUCE)
	{
		if(pp->md->FL_TURNTOHOLD == 3)//转保压方式为时间时，就不需要这里特别显示时间了，因为在射出的每一段里，都会显示不同的时间DK 2014.1104
	   	{
			;
		}
	   else
	   {
			ActSetTM(pact->ptsk,  pp->INJ_TM_N[0]/10);
		}
		
		ret = InjectIn(pp, pact);
        if(pp->st->INJECT_MAXPRESS < GetPumpPress(HYDR_PORT(pp->hy->HYDR_PORT_INJCT)))
		{
			pp->st->INJECT_MAXPRESS =  GetPumpPress(HYDR_PORT(pp->hy->HYDR_PORT_INJCT));
        }
	}
	else
	{
		ret = InjectHold(pp, pact);
        if(pp->st->HOLD_MAXPRESS <  GetPumpPress(HYDR_PORT(pp->hy->HYDR_PORT_INJCT)))
        {
		    pp->st->HOLD_MAXPRESS =  GetPumpPress(HYDR_PORT(pp->hy->HYDR_PORT_INJCT));
        }
	}

	/*设置当前射出动作步，用于射出曲线判断*/
	pp->st->INJECT_STEP = pact->step;
	if(ret != ACT_ERR_ABORT && ret != ACT_END)
	{
		ret = InjectBackgrd(pp, pact);
	}

	return ret;
}

void SetInjectNozzle(PINJECT pp, PACT pact)
{
	UI16 hydelay;
	
	if(pp->mh->INJ_HYDELAY > 9999)
		pp->mh->INJ_HYDELAY = 0;
	hydelay = pp->mh->INJ_HYDELAY;
	
	if(pp->md->HYDY_WAY_SET == 1)
	{
		SetPOBit(GetSys()->po->HYD_COMB, PC_ON);
	}
	
	SetPOBit(pp->po->O_INJ_SOL, PC_ON);
	SetPOBit(pp->po->O_IN_SETU, PC_ON);
  if(ChkSysCfg(CO5_INJCHADA))
  {
  	if(pp->md->Inj_Open==1)
	 SetPOBit(pp->po->O_INJ_OPEN, PC_ON);
    else
  	 SetPOBit(pp->po->O_INJ_OPEN,PC_OFF);
  }
	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->mh->DAPRES_INJSETUP, pp->mh->DAFLOW_INJSETUP, 0, 0, 0, 0, HYDRDELY, HYDR_PORT(pp->hy->HYDR_PORT_INJCT), 
						 pp->hy->HYDR_PUMPS_INJCT, pp->hy->HYDR_PUMPS_INJCT, pp->hy->SERVO_KP_INJCT, pp->hy->SERVO_KI_INJCT, INJDFLAG, 
						 TRUE, HYDR_PRI(pp->hy->HYDR_PORT_INJCT),pp->md->INJ_SPD_UTI);

	if (pp->mh->FL_INJ_PROV == 1)
	{
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->mh->DAFLOW_INJSETUP,0, 0,
				0,INJ_PROV_DA_CH,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_INJCT),INJ_PROV_FWD);
	}
}

void SetInjectNozzleOff(PINJECT pp, PACT pact)
{
	if(pp->md->HYDY_WAY_SET == 1)
	{
		SetPOBit(GetSys()->po->HYD_COMB, PC_OFF);
	}
	
	SetPOBit(pp->po->O_INJ_SOL, PC_OFF);
	SetPOBit(pp->po->O_IN_SETU, PC_OFF);
  	SetPOBit(pp->po->O_INJ_OPEN, PC_OFF);
	HYDR_CLS(pp->mh->DAPRES_INJOFFRAMP, pp->mh->DAFLOW_INJOFFRAMP);
}


void SetInjectNozzleOffDelay(PINJECT pp, PACT pact)
{
	SetPOBit(pp->po->O_INJ_SOL, PC_OFF);
	SetPOBit(pp->po->O_IN_SETU, PC_OFF);
  	SetPOBit(pp->po->O_INJ_OPEN, PC_OFF);
	HYDR_CLS(pp->mh->DAPRES_INJOFFRAMP, pp->mh->DAFLOW_INJOFFRAMP);
}


ACT_RET InjectNozzleBackgrd(PINJECT pp, PACT pact)         //inject background work 
{
    if(!InjTempChkSimple(pp))
		return ACT_ERR_ABORT;

	if(ChkSysCfg(CO11_SAFEINJ))    //2014-08-04  射出检测安全门
	{
    	if((FSafeDoorIsCloseErrChk(pp->pclmp->psfdr)))
			return ACT_ERR_ABORT;
	}
	
	m_speed = pp->st->INJECT_SPEED;
	return ACT_OK;
}

ACT_RET InjectNozzle(PACT pact, UI32 para)
{
	PINJECT pp = (PINJECT)(pact->part);
	ACT_RET ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;
	switch(pact->step)
	{
		case INIT1_STEP:      // Special; before nozzle fwd inj autopurge time p f adj inj p f 07-7-9
			if(pp->md->FL_AUTOPURGE == 2)    //座进前射出选择位置
				ACT_STEP_NEW(STAGE_5_STEP);
			else
				ACT_STEP_NEW(STAGE_1_STEP);
			break;

		case STAGE_1_STEP:
		case STAGE_1_STEP + 1:
			if(STEP_10MS_CHK(pp->md->TM_INJBEFORENOZADV))
				ACT_STEP_NEW(END_STEP);

			CHK_SET_MOV(SetInjectNozzle);
			break;

		case STAGE_5_STEP:
			pp->AdInjectPosn = (GetMeterPosByCh(pp->mh->INJ_METERCH) > pp->md->ADPOSI_INJBEFORENOZADV)?GetMeterPosByCh(pp->mh->INJ_METERCH):pp->md->ADPOSI_INJBEFORENOZADV;
			ACT_STEP_NEW(STAGE_6_STEP);
			break;
			
		case STAGE_6_STEP:
		case STAGE_6_STEP + 1:
			if(MoveLocationMeterControlProc((pp->AdInjectPosn-pp->md->ADPOSI_INJBEFORENOZADV), GetMeterPosByCh(pp->mh->INJ_METERCH)))
				ACT_STEP_NEW(END_STEP);
			CHK_SET_MOV(SetInjectNozzle);
			break;

			
		case END_STEP:
			SET_OFF(SetInjectNozzleOffDelay);
			ret = ACT_END;
			break;
		default:
			ret = ACT_END;
	}
		
	if(ret == ACT_END || ret == ACT_ERR_ABORT)
	{
	    return ret;
	}
	else
		return InjectNozzleBackgrd(pp, pact);
}


void SetInjectFwdAdj(PINJECT pp, PACT pact)
{
	UI16 hydelay;
	
	if(pp->mh->INJ_HYDELAY > 9999)
		pp->mh->INJ_HYDELAY = 0;
	hydelay = pp->mh->INJ_HYDELAY;
	if(ChkSysCfg(CO5_INJCHADA))
	{
		if(pp->md->Inj_Open==1)
			SetPOBit(pp->po->O_INJ_OPEN, PC_ON);
		else
			SetPOBit(pp->po->O_INJ_OPEN,PC_OFF);
	}
	
	if(pp->md->HYDY_WAY_SET == 1)
	{
		SetPOBit(GetSys()->po->HYD_COMB, PC_ON);
	}

	SetPOBit(pp->po->O_IN_SETU, PC_ON);
	SetPOBit(pp->po->O_INJ_SOL, PC_ON);
	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->mh->DAPRES_INJSETUP,pp->mh->DAFLOW_INJSETUP,pp->mh->DAPRES_INJRAMP2,pp->mh->DAFLOW_INJRAMP2,
                      pp->mh->DAPRES_INJRAMP,pp->mh->DAFLOW_INJRAMP,HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_INJCT),pp->hy->HYDR_PUMPS_INJSETUP,pp->hy->HYDR_PUMPS_INJSETUP,
                        pp->hy->SERVO_KP_INJCT,pp->hy->SERVO_KI_INJCT, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_INJCT),pp->md->INJ_SPD_UTI);

	if(pp->pclmp != NULL && pp->pclmp->prbt != NULL)
		RobotInjectOnOff(pp->pclmp->prbt, TRUE);

	if (pp->mh->FL_INJ_PROV == 1)
	{		
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->mh->DAFLOW_INJSETUP,pp->mh->DAFLOW_INJRAMP2, pp->mh->DAFLOW_INJRAMP,
				0,INJ_PROV_DA_CH,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_INJCT),INJ_PROV_FWD);
	}
}

ACT_RET InjectAdj(PACT pact, UI32 para)
{
	PINJECT pp = (PINJECT)(pact->part);
	ACT_RET ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;

	if(!ChkOperMode(OM_ADJU) && (pp->mh->FL_MANUALINJECTUSEPRES_FLOW != 1))
		return ACT_ERR_ABORT;

	if(ActIODelayLimit(pp->po->O_INJ_SOL, TRUE))
	{
	    return ACT_ERR_ABORT;
	}
	
	switch(pact->step)
	{
		case INIT1_STEP:      
			if(!InjTempChkSimple(pp))
				return ACT_ERR_ABORT;
			else
				ACT_STEP_NEW(STAGE_1_STEP);
			break;

		case STAGE_1_STEP:
		case STAGE_1_STEP + 1:
			MoveLocationNoControlProc();

			CHK_SET_MOV(SetInjectFwdAdj);
			break;

		default:
			ret = ACT_END;
			break;
	}

	return ret;
}  


void SetNeedleValve(PINJECT pp, PACT pact)
{
	SetPOBit(pp->po->O_INJ_SPC, PC_ON);
}

void SetNeedleValveOff(PINJECT pp, PACT pact)
{
	SetPOBitDelay(pp->po->O_INJ_SPC, PC_OFF,pp->mh->TM_INJSPC_CLSDLY);
}


//胶口针阀动作函数
ACT_RET InjNeedleValve(PACT pact, UI32 para)
{
	PINJECT pp = (PINJECT)(pact->part);
	ACT_RET ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;
	
	switch(pact->step)
	{
		case INIT1_STEP:			
			if(pp->mh->FL_INj_SPC)
			{
				pp->needle_valve_allowinj = FALSE;

				if (ChkOperMode(OM_MANUAL) || (pp->mh->FL_INJ_SPC_FUNC == 1) || (pp->mh->POS_INJ_SPC == 0))
				{
					ACT_STEP_NEW(STAGE_2_STEP);
				}
				else
				{
					ACT_STEP_NEW(STAGE_1_STEP);
				}	
			}
			else
			{
				pp->needle_valve_allowinj = TRUE;
				return ACT_END;
			}					
			break;
		case STAGE_1_STEP://关模时开胶口
			if (pp->mh->POS_INJ_SPC > GetMeterPosByCh(pp->pclmp->mh->CLMP_METERCH))
			{				
				ACT_STEP_NEW(STAGE_1_STEP+1);
			}
			break;
		case STAGE_1_STEP + 1:
			if (ACT_STEP_10MS > pp->mh->TM_INJSPC_OPNELR)
			{
				SetPOBit(pp->po->O_INJ_SPC, PC_ON);
				pp->needle_valve_allowinj = TRUE;
				ACT_STEP_NEW(STAGE_3_STEP);
			}
			break;
		case STAGE_2_STEP://射出同步开 或者 手动注射				
			SetPOBit(pp->po->O_INJ_SPC, PC_ON);
			ACT_STEP_NEW(STAGE_2_STEP+1);
			break;
		case STAGE_2_STEP + 1:
			if (ACT_STEP_10MS > pp->mh->TM_INJSPC_OPNELR)
			{
				pp->needle_valve_allowinj = TRUE;
				ACT_STEP_NEW(STAGE_3_STEP);
			}
			break;
		case STAGE_3_STEP:
			break;
		default:
			ret = ACT_END;
			break;
	}

	return ret;
}  


void InjectSetup(PINJECT pp)
{
	if(pp != NULL)
	{
		pp->pclmp = (PCLAMP)GetMachineSubPart(CLAMP_ID, 1);
		pp->pnzl = (PNOZZLE)GetMachineSubPart(NOZZLE_ID, GET_PART_SN(pp->part.id));
		pp->pchgprs = (PCHGPRS)GetMachineSubPart(CHGPRS_ID, GET_PART_SN(pp->part.id));
	}
}

BOOL InitInjectPart(PINJECT pp, UI8 sn)
{
	static const SUB_INIT_INFO sub_init[]  = INJECT_INIT_INFO;

	static const ACT_INIT_INFO act_init[] = {{OS_INJE, OS_INJE, ACT_NTYPE, InjectProc, InjectOff},
										{OS_CHRG, OS_CHRG, ACT_NTYPE, ChargeProc, ChargeOff},
										{OS_SUCK, OS_SUCK, ACT_NTYPE, SuckBack, SuckBwdOff},
										{OS_AINJE, OS_INJE, ACT_NTYPE, InjectAdj, InjectOff},
									 	{OS_NZLINJ, OS_INJE, ACT_NTYPE, InjectNozzle, SetInjectNozzleOff},
										{OS_ACHRG, OS_CHRG, ACT_NTYPE, ChargeAdj, ChargeOff},
										{OS_PCHRG, OS_CHRG, ACT_NTYPE, AutoPurgeCharge, ChargeOff},
										{OS_CHRGBFINJ, OS_CHRG, ACT_NTYPE, ChargeBeforeInj, ChargeOff},
										{OS_ASUCK, OS_SUCK, ACT_NTYPE, SuckBwdAdj, SuckBwdOff},
										{OS_SUCKCHG, OS_SUCK, ACT_NTYPE, SuckBwdBfChg, SuckBwdOff},
										{OS_APURG, OS_APURG, ACT_CTRL, AutoPurge, NULL},
										{OS_DACHRG, OS_DACHRG, ACT_CTRB, GetChargeRpmadj,ClearChgOut},
										{OS_NEEDLEVALVE,OS_NEEDLEVALVE, ACT_CTRB, InjNeedleValve, SetNeedleValveOff},
										{OS_SUCK_SYNC,OS_SUCK_SYNC, ACT_CTRB, SuckbackSync, SuckbackSyncOff},};

	if(!INIT_PART(pp, INJECT_ID, sn, sub_init))
		return FALSE;

	if(!INIT_ACT(pp, act_init))
		return FALSE;

	InjectSetup(pp);
	
	pp->mh->INJ_METERCH = DEF_INJECT_METER_CH;
	SetAdChType(pp->mh->INJ_METERCH, CH_FINE_METER);


	pp->pi->I_PUR_GUA = 7;
	pp->pi->I_RPM_CHK = 10;

	pp->pa->ADPOSI_INJ1END = 20;
	pp->pa->ADPOSI_INJ2END = 15;
	pp->pa->ADPOSI_INJ3END = 12;
	pp->pa->ADPOSI_INJ4END = 10;
	pp->pa->ADPOSI_INJ5END = 8;
	pp->pa->ADPOSI_INJ6END = 5;
	pp->pa->ADPOSI_INJ7END = 2;
	pp->pa->ADPOSI_INJ8END = 1;

	pp->pa->ADPOSI_CHG1END = 100;
	pp->pa->ADPOSI_CHG2END = 120;
	pp->pa->ADPOSI_CHG3END = 130;
	
	pp->pa->ADPOSI_SUCKBACKEND = 150;

	pp->md->TM_SUCKBACKDEFAULTMAX = 500;
	pp->relvlvopnflag = FALSE;	
	return TRUE;
}

UI32 InjectCmd(PINJECT pp, UI32 cmd, UI32 para)
{
	UI32 ret = TRUE;
	PMACHINE pm = &m_machine;
	UI16 offset = 0;
	UI16 len = 0;
	UI32 qc_tm_inj = 0, qc_tm_min = 0, qc_tm_max = 0;	//qc功能中射出时间，qc功能中射出时间最小值和最大值
	
	if(pp == NULL)
		return FALSE;

	switch(GET_CMD(cmd))
	{
		case CMD_MANUAL_INIT:
			pp->st->CN_CHRGSPD = 0;
			break;
		case CMD_METERCH:
			ret = pp->mh->INJ_METERCH;
			break;
		case CMD_QC_PARA:
			if(ChkOperMode(OM_AUTO) && (pm->md->Qc_Fun==1))
			{
				if(para ==1)//  射出QC数据处理
				{
					if(pp->md->Qc_Mode_InjTm== 1)  //射出时间QC数据
					{
						qc_tm_inj = ((UI32)pp->st->TM_INJ_H << 16) + (UI32)pp->st->TM_INJ_L;
						
						if(pm->st->Qc_Opn_Cnt == 0)
						{
							pp->qc_avg_injtm = qc_tm_inj;
							qc_tm_min = qc_tm_inj;
							qc_tm_max= qc_tm_inj;
							pp->st->Qc_Unq_Cnt_InjTm = 0;
						}
						else
						{
							pp->qc_avg_injtm = pp->qc_avg_injtm + qc_tm_inj;
							qc_tm_min = (qc_tm_min > qc_tm_inj)?qc_tm_inj:qc_tm_min;
							qc_tm_max = (qc_tm_max < qc_tm_inj)?qc_tm_inj:qc_tm_max;
						}

						pp->st->Qc_Min_InjTm_L = (UI16)(qc_tm_min & 0x0ffff);
						pp->st->Qc_Min_InjTm_H = (UI16)((qc_tm_min >> 16) & 0x0ffff);
					
						pp->st->Qc_Max_InjTm_L = (UI16)(qc_tm_max & 0x0ffff);
						pp->st->Qc_Max_InjTm_H = (UI16)((qc_tm_max >> 16) & 0x0ffff);
					
						if(((qc_tm_inj>(pp->md->Qc_TmSet_InjTm+ pp->md->Qc_DeviationSet_InjTm))||
						 (qc_tm_inj<(pp->md->Qc_TmSet_InjTm - pp->md->Qc_DeviationSet_InjTm)))&&((pm->st->Qc_Opn_Cnt+1) <=pm->md->Qc_OpnNum))
			 				pp->st->Qc_Unq_Cnt_InjTm++;											
					}
					if(pp->md->Qc_Mode_InjSpd== 1)  //射出速度QC数据
					{
						if(pm->st->Qc_Opn_Cnt == 0)
						{
							pp->qc_avg_injspd = pp->st->INJECT_AVERAGESPEED;
							pp->st->Qc_Min_InjSpd = pp->st->INJECT_AVERAGESPEED;
							pp->st->Qc_Max_InjSpd = pp->st->INJECT_AVERAGESPEED;
							pp->st->Qc_Unq_Cnt_InjSpd = 0;
						}
						else
						{
							pp->qc_avg_injspd =pp->qc_avg_injspd+ pp->st->INJECT_AVERAGESPEED;
							pp->st->Qc_Min_InjSpd=  (pp->st->Qc_Min_InjSpd>pp->st->INJECT_AVERAGESPEED)?pp->st->INJECT_AVERAGESPEED: pp->st->Qc_Min_InjSpd;
							pp->st->Qc_Max_InjSpd=  (pp->st->Qc_Max_InjSpd<pp->st->INJECT_AVERAGESPEED)?pp->st->INJECT_AVERAGESPEED: pp->st->Qc_Max_InjSpd;
						}
						if(((pp->st->INJECT_AVERAGESPEED>(pp->md->Qc_TmSet_InjSpd+ pp->md->Qc_DeviationSet_InjSpd))||
						 (pp->st->INJECT_AVERAGESPEED<(pp->md->Qc_TmSet_InjSpd - pp->md->Qc_DeviationSet_InjSpd)))&&((pm->st->Qc_Opn_Cnt+1) <=pm->md->Qc_OpnNum))
			 				pp->st->Qc_Unq_Cnt_InjSpd++;									
					}		
					if(pp->md->Qc_Mode_InjEnd== 1)  //保压转换位置QC数据
					{
						if(pm->st->Qc_Opn_Cnt == 0)
						{
							pp->qc_avg_injend = pp->st->ADPOSI_INJCUSHION;
							pp->st->Qc_Min_InjEnd = pp->st->ADPOSI_INJCUSHION;
							pp->st->Qc_Max_InjEnd = pp->st->ADPOSI_INJCUSHION;
							pp->st->Qc_Unq_Cnt_InjEnd = 0;
						}
						else
						{
							pp->qc_avg_injend =pp->qc_avg_injend+ pp->st->ADPOSI_INJCUSHION;
							pp->st->Qc_Min_InjEnd=  (pp->st->Qc_Min_InjEnd>pp->st->ADPOSI_INJCUSHION)?pp->st->ADPOSI_INJCUSHION: pp->st->Qc_Min_InjEnd;
							pp->st->Qc_Max_InjEnd=  (pp->st->Qc_Max_InjEnd<pp->st->ADPOSI_INJCUSHION)?pp->st->ADPOSI_INJCUSHION: pp->st->Qc_Max_InjEnd;
						}
						if(((pp->st->ADPOSI_INJCUSHION>(pp->md->Qc_TmSet_InjEnd+ pp->md->Qc_DeviationSet_InjEnd))||
						 (pp->st->ADPOSI_INJCUSHION<(pp->md->Qc_TmSet_InjEnd - pp->md->Qc_DeviationSet_InjEnd)))&&((pm->st->Qc_Opn_Cnt+1) <=pm->md->Qc_OpnNum))
			 				pp->st->Qc_Unq_Cnt_InjEnd++;							
					}	
					if(pp->md->Qc_Mode_InjPrs== 1)  //射出压力QC数据
					{
						if(pm->st->Qc_Opn_Cnt == 0)
						{
							pp->qc_avg_injpres = pp->st->INJECT_MAXPRESS;
							pp->st->Qc_Min_InjPrs = pp->st->INJECT_MAXPRESS;
							pp->st->Qc_Max_InjPrs = pp->st->INJECT_MAXPRESS;
							pp->st->Qc_Unq_Cnt_InjPrs = 0;
						}
						else
						{
							pp->qc_avg_injpres = pp->qc_avg_injpres+pp->st->INJECT_MAXPRESS;
							pp->st->Qc_Min_InjPrs=  (pp->st->Qc_Min_InjPrs>pp->st->INJECT_MAXPRESS)?pp->st->INJECT_MAXPRESS: pp->st->Qc_Min_InjPrs;
							pp->st->Qc_Max_InjPrs=  (pp->st->Qc_Max_InjPrs<pp->st->INJECT_MAXPRESS)?pp->st->INJECT_MAXPRESS: pp->st->Qc_Max_InjPrs;
						}
						if(((pp->st->INJECT_MAXPRESS>(pp->md->Qc_TmSet_InjPrs+ pp->md->Qc_DeviationSet_InjPrs))||
						 (pp->st->INJECT_MAXPRESS<(pp->md->Qc_TmSet_InjPrs - pp->md->Qc_DeviationSet_InjPrs)))&&((pm->st->Qc_Opn_Cnt+1) <=pm->md->Qc_OpnNum))
			 				pp->st->Qc_Unq_Cnt_InjPrs++;							
					}					
					if(pp->md->Qc_Mode_HoldEnd== 1)  //残料位置QC数据
					{
					//	pp->st->Qc_Avg_HoldEnd=  pp->st->Qc_Avg_HoldEnd+pp->st->ADPOSI_INJHOLDEND;
						if(pm->st->Qc_Opn_Cnt == 0)
						{
							pp->qc_avg_holdend = pp->st->ADPOSI_INJHOLDEND;
							pp->st->Qc_Min_HoldEnd = pp->st->ADPOSI_INJHOLDEND;
							pp->st->Qc_Max_InjEnd = pp->st->ADPOSI_INJHOLDEND;
							pp->st->Qc_Unq_Cnt_HoldEnd = 0;
						}
						else
						{
							pp->qc_avg_holdend = pp->qc_avg_holdend+pp->st->ADPOSI_INJHOLDEND;
							pp->st->Qc_Min_HoldEnd=  (pp->st->Qc_Min_HoldEnd>pp->st->ADPOSI_INJHOLDEND)?pp->st->ADPOSI_INJHOLDEND: pp->st->Qc_Min_HoldEnd;
							pp->st->Qc_Max_HoldEnd=  (pp->st->Qc_Max_HoldEnd<pp->st->ADPOSI_INJHOLDEND)?pp->st->ADPOSI_INJHOLDEND: pp->st->Qc_Max_HoldEnd;
						}
						if(((pp->st->ADPOSI_INJHOLDEND>(pp->md->Qc_TmSet_HoldEnd+ pp->md->Qc_DeviationSet_HoldEnd))||
						 (pp->st->ADPOSI_INJHOLDEND<(pp->md->Qc_TmSet_HoldEnd - pp->md->Qc_DeviationSet_HoldEnd)))&&((pm->st->Qc_Opn_Cnt+1) <=pm->md->Qc_OpnNum))
			 				pp->st->Qc_Unq_Cnt_HoldEnd++;						
					}						
				}
				else if(para ==2)//储料数据QC处理
				{
					if(pp->md->Qc_Mode_ChgTm== 1)  //储料时间QC数据
					{
						if(pm->st->Qc_Opn_Cnt == 0)
						{
							pp->qc_avg_chgtm = pp->st->TM_CHARGE;
							pp->st->Qc_Min_ChgTm = pp->st->TM_CHARGE;
							pp->st->Qc_Max_ChgTm = pp->st->TM_CHARGE;
							pp->st->Qc_Unq_Cnt_ChgTm = 0;
						}
						else
						{
							pp->qc_avg_chgtm = pp->qc_avg_chgtm+pp->st->TM_CHARGE;
							pp->st->Qc_Min_ChgTm=  (pp->st->Qc_Min_ChgTm>pp->st->TM_CHARGE)?pp->st->TM_CHARGE: pp->st->Qc_Min_ChgTm;
							pp->st->Qc_Max_ChgTm=  (pp->st->Qc_Max_ChgTm<pp->st->TM_CHARGE)?pp->st->TM_CHARGE: pp->st->Qc_Max_ChgTm;
						}
						if(((pp->st->TM_CHARGE>(pp->md->Qc_TmSet_ChgTm+ pp->md->Qc_DeviationSet_ChgTm))||
						 (pp->st->TM_CHARGE<(pp->md->Qc_TmSet_ChgTm - pp->md->Qc_DeviationSet_ChgTm)))&&((pm->st->Qc_Opn_Cnt+1) <=pm->md->Qc_OpnNum))
			 				pp->st->Qc_Unq_Cnt_ChgTm++;							
					}		
					if(pp->md->Qc_Mode_ChgEnd== 1)  //储料终止位置QC数据
					{
						if(pm->st->Qc_Opn_Cnt == 0)
						{
							pp->qc_avg_chgend = pp->st->ADPOSI_CHGEND;
							pp->st->Qc_Min_ChgEnd = pp->st->ADPOSI_CHGEND;
							pp->st->Qc_Max_ChgEnd = pp->st->ADPOSI_CHGEND; 
							pp->st->Qc_Unq_Cnt_ChgEnd = 0;
						}
						else
						{
							pp->qc_avg_chgend = pp->qc_avg_chgend+pp->st->ADPOSI_CHGEND;
							pp->st->Qc_Min_ChgEnd=  (pp->st->Qc_Min_ChgEnd>pp->st->ADPOSI_CHGEND)?pp->st->ADPOSI_CHGEND: pp->st->Qc_Min_ChgEnd;
							pp->st->Qc_Max_ChgEnd=  (pp->st->Qc_Max_ChgEnd<pp->st->ADPOSI_CHGEND)?pp->st->ADPOSI_CHGEND: pp->st->Qc_Max_ChgEnd;
						}
						if(((pp->st->ADPOSI_CHGEND>(pp->md->Qc_TmSet_ChgEnd+ pp->md->Qc_DeviationSet_ChgEnd))||
						 (pp->st->ADPOSI_CHGEND<(pp->md->Qc_TmSet_ChgEnd - pp->md->Qc_DeviationSet_ChgEnd)))&&((pm->st->Qc_Opn_Cnt+1) <=pm->md->Qc_OpnNum))
			 				pp->st->Qc_Unq_Cnt_ChgEnd++;									
					}						
				}
				else if(para ==3)
				{
					if(pp->md->Qc_Mode_SuckTm== 1)  //射退时间QC数据
					{
					//	pp->st->Qc_Avg_SuckTm=  pp->st->Qc_Avg_SuckTm+pp->st->TM_SUCKBACK1;
						if(pm->st->Qc_Opn_Cnt == 0)
						{
							pp->qc_avg_sucktm = pp->st->TM_SUCKBACK1;
							pp->st->Qc_Min_SuckTm = pp->st->TM_SUCKBACK1;
							pp->st->Qc_Max_SuckTm =  pp->st->TM_SUCKBACK1;
							pp->st->Qc_Unq_Cnt_SuckTm = 0;
						}
						else
						{
							pp->qc_avg_sucktm = pp->qc_avg_sucktm+pp->st->TM_SUCKBACK1;
							pp->st->Qc_Min_SuckTm=  (pp->st->Qc_Min_SuckTm>pp->st->TM_SUCKBACK1)?pp->st->TM_SUCKBACK1: pp->st->Qc_Min_SuckTm;
							pp->st->Qc_Max_SuckTm=  (pp->st->Qc_Max_SuckTm<pp->st->TM_SUCKBACK1)?pp->st->TM_SUCKBACK1: pp->st->Qc_Max_SuckTm;
						}
						if(((pp->st->TM_SUCKBACK1>(pp->md->Qc_TmSet_SuckTm+ pp->md->Qc_DeviationSet_SuckTm))||
						 (pp->st->TM_SUCKBACK1<(pp->md->Qc_TmSet_SuckTm - pp->md->Qc_DeviationSet_SuckTm)))&&((pm->st->Qc_Opn_Cnt+1) <=pm->md->Qc_OpnNum))
			 				pp->st->Qc_Unq_Cnt_SuckTm++;								
					}	
					if(pp->md->Qc_Mode_SuckBwd== 1)  //射退终止位置QC数据
					{
					//	pp->st->Qc_Avg_SuckBwd=  pp->st->Qc_Avg_SuckBwd+pp->st->ADPOSI_SUCKBACK;
						if(pm->st->Qc_Opn_Cnt == 0)
						{
							pp->qc_avg_suckend = pp->st->ADPOSI_SUCKBACK;
							pp->st->Qc_Min_SuckBwd = pp->st->ADPOSI_SUCKBACK;
							pp->st->Qc_Max_SuckBwd = pp->st->ADPOSI_SUCKBACK;
							pp->st->Qc_Unq_Cnt_SuckBwd  = 0;
						}
						else
						{
							pp->qc_avg_suckend = pp->qc_avg_suckend+pp->st->ADPOSI_SUCKBACK;
							pp->st->Qc_Min_SuckBwd=  (pp->st->Qc_Min_SuckBwd>pp->st->ADPOSI_SUCKBACK)?pp->st->ADPOSI_SUCKBACK: pp->st->Qc_Min_SuckBwd;
							pp->st->Qc_Max_SuckBwd=  (pp->st->Qc_Max_SuckBwd<pp->st->ADPOSI_SUCKBACK)?pp->st->ADPOSI_SUCKBACK: pp->st->Qc_Max_SuckBwd;
						}
						if(((pp->st->ADPOSI_SUCKBACK>(pp->md->Qc_TmSet_SuckBwd+ pp->md->Qc_DeviationSet_SuckBwd))||
						 (pp->st->ADPOSI_SUCKBACK<(pp->md->Qc_TmSet_SuckBwd - pp->md->Qc_DeviationSet_SuckBwd)))&&((pm->st->Qc_Opn_Cnt+1) <=pm->md->Qc_OpnNum))
			 				pp->st->Qc_Unq_Cnt_SuckBwd++;							
					}						
				}
			}
			break;
		case CMD_DATA_RX:			
			offset = (para >> 16 & 0xffff);
			len = (para & 0xffff);
			
			if((GET_CMD_PARA(cmd) & 0xff) == MD_ID)
			{			  	
				if(len > 0 && (GET_IND(PINJECT_MOLDSET, SUCK_SPD_STOPOFF) >= offset && GET_IND(PINJECT_MOLDSET, SLOPE_MAX_SUCK_SPD) <= (offset + len - 1)))
				{
					//修改斜率相关参数时，对应缓存进行计算
					InitSuckRamp(pp);
				}
			}
			else if((GET_CMD_PARA(cmd) & 0xff) == MH_ID)
			{
				if(len > 0 && (GET_IND(PINJECT_MACHSET, DAFLOW_SUCKBACKOFFRAMP) >= offset && GET_IND(PINJECT_MACHSET, DAPRES_SUCKBACKRAMP) <= (offset + len - 1)))
				{
					//修改斜率相关参数时，对应缓存进行计算
					InitSuckRamp(pp);
				}
			}
			else if((GET_CMD_PARA(cmd) & 0xff) == PA_ID)
			{
				if(len > 0 && ((GET_IND(PINJECT_ACTPARA, TM_INJHOLD10) >= offset && GET_IND(PINJECT_ACTPARA, TM_INJ1) <= (offset + len - 1))
					|| (GET_IND(PINJECT_ACTPARA, TM_INJHOLD10_H) >= offset && GET_IND(PINJECT_ACTPARA, TM_INJ1_H) <= (offset + len - 1))))
				{
					Init_inj_time(pp);	//射出保压时间初始化
				}

				pp->pa->PER_DAFLOW_INJECT = pp->pa->PER_DAFLOW_INJECT > 150 || pp->pa->PER_DAFLOW_INJECT == 0 ?100:pp->pa->PER_DAFLOW_INJECT;
				pp->pa->PER_DAFLOW_HOLD = pp->pa->PER_DAFLOW_HOLD > 150 || pp->pa->PER_DAFLOW_HOLD == 0 ?100:pp->pa->PER_DAFLOW_HOLD;
				pp->pa->PER_DAFLOW_CHARGE = pp->pa->PER_DAFLOW_CHARGE > 150 || pp->pa->PER_DAFLOW_CHARGE == 0 ?100:pp->pa->PER_DAFLOW_CHARGE;
				pp->pa->PER_DAFLOW_SUCKBACK = pp->pa->PER_DAFLOW_SUCKBACK > 150 || pp->pa->PER_DAFLOW_SUCKBACK == 0 ?100:pp->pa->PER_DAFLOW_SUCKBACK;
			}
			
			break;
		default:
			ret = FALSE;
			break;
	}

	return ret;
}
