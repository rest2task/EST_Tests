/******************************************************************************
  文 件 名   : adjust.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 调模部件及动作函数
  函数列表   :
              AdjBwdOff
              AdjCntTMErrChk
              AdjFwdOff
              AdjustCmd
              AdjustSetup
              AdujstInit
              AutoAdj
              AutoAdjBwd
              AutoAdjFwd
              InitAdjustPart
              ManualAdjBwd
              ManualAdjFwd
              SetAdjBwdFast
              SetAdjFwdFast
              SetAdjFwdSlow
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

#include "actcom.h"
#include "adjust.h"
#include "clamp.h"
#include "safedoor.h"
#include "machine.h"
#include "cfgdb.h"
#include "keycmd.h"

extern   HYDR    g_hydrout;

void SetAdjFwdSlow(PADJUST pp, PACT pact)
{
//	UI16 ramp;
	UI16 hydelay;
	
	hydelay = pp->mh->ADJF_HYDELAY;
	
	SetPOBit(pp->po->O_ADJ_FWD, PC_ON);                    //打开方向阀
	if(ChkSysCfg(XF1_NMV))
	{
		SetPOBit(GetSysPO(O_HYD_VLV2), PC_ON);
	}

	 //设定运动控制参数
//	if(pact->ftaskstat & INTF_CEC)
//		ramp = pp->mh->DAFLOW_RAMP;
//	else
//		ramp = pp->mh->DAFLOW_STARTRAMP;
	
	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_ADJFFAST, pp->pa->DAFLOW_ADJSLOW, pp->mh->DAPRES_RAMP, pp->mh->DAFLOW_RAMP, pp->mh->DAPRES_RAMP, pp->mh->DAFLOW_RAMP, HYDRDELY,  
						HYDR_PORT(pp->hy->HYDR_PORT_ADJUST), pp->hy->HYDR_PUMPS_ADJUST, pp->hy->HYDR_PUMPS_ADJUST, pp->hy->SERVO_KP_ADJUST, pp->hy->SERVO_KI_ADJUST, 
						0, !(pact->ftaskstat & INTF_CEC), HYDR_PRI(pp->hy->HYDR_PORT_ADJUST),pp->pa->PER_DAFLOW_ADJUST);
}

void SetAdjFwdFast(PADJUST pp, PACT pact)
{
//	UI16 ramp;
	UI16 hydelay;
	
	hydelay = pp->mh->ADJF_HYDELAY;
	
	SetPOBit(pp->po->O_ADJ_FWD, PC_ON);                    //打开方向阀
	if(ChkSysCfg(XF1_NMV))
	{
		SetPOBit(GetSysPO(O_HYD_VLV2), PC_ON);
	}

	 //设定运动控制参数
//	if(pact->ftaskstat & INTF_CEC)
//		ramp = pp->mh->DAFLOW_RAMP;
//	else
//		ramp = pp->mh->DAFLOW_STARTRAMP;
	
	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_ADJFFAST, pp->pa->DAFLOW_ADJFFAST, pp->mh->DAPRES_RAMP, pp->mh->DAFLOW_RAMP, pp->mh->DAPRES_RAMP, pp->mh->DAFLOW_RAMP, HYDRDELY,  
						 HYDR_PORT(pp->hy->HYDR_PORT_ADJUST), pp->hy->HYDR_PUMPS_ADJUST, pp->hy->HYDR_PUMPS_ADJUST, pp->hy->SERVO_KP_ADJUST, pp->hy->SERVO_KI_ADJUST, 
						 0, !(pact->ftaskstat & INTF_CEC), HYDR_PRI(pp->hy->HYDR_PORT_ADJUST), pp->pa->PER_DAFLOW_ADJUST);
 
}

void AdjFwdOff(PADJUST pp, PACT pact)
{
	if(pp != NULL)
	{
		SetPOBit(pp->po->O_ADJ_FWD, PC_OFF);                    //打开方向阀
		if(ChkSysCfg(XF1_NMV))
		{
			SetPOBit(GetSysPO(O_HYD_VLV2), PC_OFF);
		}

		if(ChkSysCfg(CO0_ADJZX) && pp->fadjposexchg == 1)
		{
			pp->mh->ADPOSI_ADJCUR = pp->st->ADPOSI_ADJREAL;  
			pp->fadjposexchg = 0;
			pp->adj_start = 1;
			PartDataTx((PPART)pp, MH_ID, 1, GET_IND(PADJUST_MACHSET, ADPOSI_ADJCUR), FALSE);
		}  

		HYDR_CLS(pp->mh->DAPRES_OFFRAMP, pp->mh->DAFLOW_OFFRAMP);
	}
}

void AutoAdjoff(PADJUST pp, PACT pact)
{
	if(pp != NULL)
	{
		if(ChkSysCfg(CO0_ADJZX))
			PartDataTx((PPART)pp, MH_ID, 1, GET_IND(PADJUST_MACHSET, ADPOSI_ADJCUR), FALSE);	
	}
}

BOOL AdjCntTMErrChk(PADJUST pp) //09-09-18
{
	if(pp != NULL)
	{
		if(DIFF_100MS(pp->cnt_start_100ms) > ADJCHKERRTIME)
		{
	  		SET_PART_ERR(pp, ER0_ADJS);
			StandAlarm(TRUE);
			return TRUE;
		}
	}

	return FALSE;
}

void  CheckAdjUsed()      //在粗调模状态或自动调模状态时，该输出点输出
{
	PADJUST pp = (PADJUST)GetMachineSubPart(ADJUST_ID, 1);

	if((GetOperMode() == OM_ADJU) || (GetOperMode() == OM_AA_FLG))
	{
		if(GetPOBit(pp->po->O_ADJ_USE) == 0)
			SetPOBit(pp->po->O_ADJ_USE, PC_ON);   
	}
	else
	{
		if(GetPOBit(pp->po->O_ADJ_USE) == 1)
			SetPOBit(pp->po->O_ADJ_USE, 0); 
	}
}


ACT_RET AdjFwdBackgrd(PCLAMP pp)
{
	if (FSafeDoorIsCloseErrChk(pp->psfdr))                 //前安全门保护
	{
		return ACT_ERR_ABORT;
	}

	return ACT_OK;
}

ACT_RET ManualAdjFwd(PACT pact, UI32 para)
{
	int i;
	PADJUST	pp = (PADJUST)(pact->part);
	ACT_RET	ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;

	switch (pact->step)
	{
		case INIT1_STEP:
			pp->fadjposexchg = 0;
			if(TestPI(pp->pi->I_ADJ_FWD))
				ACT_STEP_NEW(END_STEP);
			else
			{
				if(ChkSysCfg(XF2_CPRBA))
				{
					 if(ClampCoreStateErrChk(pp->pclmp, FALSE))   //增加调模进退前判断中子退终
				    	return ACT_ERR_ABORT;
					 else
						ACT_STEP_NEW(INIT1_STEP + 1);       //增加调模进退前判断脱模退终
				}
				else
				{
					if(ChkSysCfg(CO0_ADJZX) || pp->pa->DAFLOW_ADJSLOW <= 0 || pp->pa->ADPOSI_ADJMD != 0)
						ACT_STEP_NEW(ADJFWDFAST);
					else
						ACT_STEP_NEW(ADJFWDSLOW);
				}		
			}
			break;

		case INIT1_STEP + 1:
			for(i = 0; i < CLAMP_MAX_EJECT; i++)
			{
				if(pp->pclmp->pejt[i] != NULL && EjectBwdErrChk(pp->pclmp->pejt[i], TRUE))
					ForkAct(pp->pclmp->pejt[i]->act + OS_EJTB - 1, SUSPEND_FORK, pact->ptsk, 0, 0);
			}
			ACT_STEP_NEW(INIT1_STEP + 2);
		    break;

		case INIT1_STEP + 2:
			for(i = 0; i < CLAMP_MAX_EJECT; i++)
			{
				if(pp->pclmp->pejt[i] != NULL )
					CLR_PART_ERR(pp->pclmp->pejt[i],ER0_EJTP);
			}
			ACT_STEP_NEW(INIT1_STEP + 3);
			break;

		case INIT1_STEP + 3:
			if(ChkSysCfg(CO0_ADJZX) || pp->pa->DAFLOW_ADJSLOW <= 0 || pp->pa->ADPOSI_ADJMD != 0)
				ACT_STEP_NEW(ADJFWDFAST);
			else
				ACT_STEP_NEW(ADJFWDSLOW);
			break;
			
		case ADJFWDSLOW:
		case ADJFWDSLOW + 1:
			if(MoveLocationTM10msControlProc(80))
				ACT_STEP_NEW(ADJFWDFAST);
			
			if(TestPI(pp->pi->I_ADJ_FWD))
				ACT_STEP_NEW(END_STEP);

			CHK_SET_MOV(SetAdjFwdSlow);
			break;
			
		case ADJFWDFAST:
			ResetPICnt(&pp->adj_chk_cnt, pp->pi->I_ADJ_CHK, 1);
			pp->adj_fwd_cnt = 0;
			pp->adj_fwd_onecyc = pp->adj_chk_cnt.onecyc;
			pp->timeoverb = 0;
			pp->cnt_start_100ms = Get100MsTick();
			pp->adj_start_10ms = Get10MsTick();
		case ADJFWDFAST + 1:
			if(MoveLocationNMControlProc(pp->pi->I_ADJ_FWD))
				ACT_STEP_NEW(END_STEP);
		    else
			{
				if(ChkSysCfg(CO0_ADJZX))
				{
					PICntLoop(&pp->adj_chk_cnt);
					if(pp->adj_fwd_cnt != GetPICnt(&pp->adj_chk_cnt))
					{
						pp->adj_fwd_cnt = GetPICnt(&pp->adj_chk_cnt);
						pp->cnt_start_100ms = Get100MsTick();
						if(ChkSysCfg(CO0_FLWDOT))
							pp->st->ADPOSI_ADJREAL = pp->mh->ADPOSI_ADJCUR - pp->mh->CNPOSI_ADJPROPERTIONAL*pp->adj_fwd_cnt/100;
						else
							pp->st->ADPOSI_ADJREAL = pp->mh->ADPOSI_ADJCUR - pp->mh->CNPOSI_ADJPROPERTIONAL*pp->adj_fwd_cnt;
						pp->fadjposexchg = 1;
					}
					else if(AdjCntTMErrChk(pp))
						return ACT_ERR_ABORT;
				}
				else if(pp->pa->ADPOSI_ADJMD != 0)
				{
					if(pp->timeoverb == 0)
					{
					    if(pp->pa->CN_ADJFIRST > 0)
						{
							if((UI16)(Get10MsTick()-pp->adj_start_10ms) >= pp->pa->CN_ADJFIRST)
							{
								AdjFwdOff(pp, pact);
								pp->timeoverb = 1;
								pact->step_100ms = Get100MsTick();
						    }
						}
						else
						{
						    if(pp->pa->CN_ADJADV > 0)
							{
								PICntLoop(&pp->adj_chk_cnt);
								if(pp->adj_fwd_cnt != GetPICnt(&pp->adj_chk_cnt))
								{
									pp->adj_fwd_cnt = GetPICnt(&pp->adj_chk_cnt);
									pp->cnt_start_100ms = Get100MsTick();
								}
								else if(AdjCntTMErrChk(pp))
									return ACT_ERR_ABORT;
								if(pp->adj_fwd_cnt >= pp->pa->CN_ADJADV)
								{
									AdjFwdOff(pp, pact);
									pp->timeoverb = 1;
									pact->step_100ms = Get100MsTick();
								}
							}
							else
							{
								PICntLoop(&pp->adj_chk_cnt);
						    	pp->adj_chk_cnt.bstart = 1;
								if(pp->adj_fwd_onecyc != pp->adj_chk_cnt.onecyc)
								{
									AdjFwdOff(pp, pact);
									pp->timeoverb = 1;
									pact->step_100ms = Get100MsTick(); 
								}
								else if(AdjCntTMErrChk(pp))
									return ACT_ERR_ABORT;
							}
						}
					}
					else if(pp->timeoverb == 1)
					{
					    if(ACT_STEP_100MS >= 10)
						{
						    pp->timeoverb = 2;
							if(pp->pa->ADPOSI_ADJMD == 2)
						        SetAdjFwdFast(pp, pact);
						}
					}
				}
			}

			CHK_SET_MOV(SetAdjFwdFast);
			break;

		case END_STEP:
			//SET_PART_ERR(pp, ER0_ADJL);
			SET_PART_ERR(pp, ER0_ADJF);
			AdjFwdOff(pp, pact);
			ret = ACT_END;
			break;
	}

	if(ret == ACT_END || ret == ACT_ERR_ABORT)
		return ret;
	else if(!ChkSysCfg(CO0_FSAD))
		return AdjFwdBackgrd(pp->pclmp);
	else
		return ret;

}

void SetAdjBwdFast(PADJUST pp, PACT pact)
{
//	UI16 ramp;
	UI16 hydelay;
	
	hydelay = pp->mh->ADJB_HYDELAY;
	
	SetPOBit(pp->po->O_ADJ_BWD, PC_ON);                    //打开方向阀
	if(ChkSysCfg(XF1_NMV))
	{
		SetPOBit(GetSysPO(O_HYD_VLV2), PC_ON);
	}

	 //设定运动控制参数
//	if(pact->ftaskstat & INTF_CEC)
//		ramp = pp->mh->DAFLOW_RAMP;
//	else
//		ramp = pp->mh->DAFLOW_STARTRAMP;
	
	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_ADJBWD, pp->pa->DAFLOW_ADJBWD,pp->mh->DAPRES_RAMP, pp->mh->DAFLOW_RAMP, pp->mh->DAPRES_RAMP, pp->mh->DAFLOW_RAMP, HYDRDELY,  
						 HYDR_PORT(pp->hy->HYDR_PORT_ADJUST), pp->hy->HYDR_PUMPS_ADJUST, pp->hy->HYDR_PUMPS_ADJUST, pp->hy->SERVO_KP_ADJUST, pp->hy->SERVO_KI_ADJUST, 
						 0, (!(pact->ftaskstat & INTF_CEC)), HYDR_PRI(pp->hy->HYDR_PORT_ADJUST), pp->pa->PER_DAFLOW_ADJUST);
}

void AdjBwdOff(PADJUST pp, PACT pact)
{
	if(pp != NULL)
	{
		SetPOBit(pp->po->O_ADJ_BWD, PC_OFF);                    //打开方向阀
		if(ChkSysCfg(XF1_NMV))
		{
			SetPOBit(GetSysPO(O_HYD_VLV2), PC_OFF);
		}

		if(ChkSysCfg(CO0_ADJZX) && pp->fadjposexchg == 1)
		{
			pp->mh->ADPOSI_ADJCUR = pp->st->ADPOSI_ADJREAL;  
			pp->fadjposexchg = 0;
			pp->adj_start = 1;
			PartDataTx((PPART)pp, MH_ID, 1, GET_IND(PADJUST_MACHSET, ADPOSI_ADJCUR), FALSE);			
		}

		HYDR_CLS(pp->mh->DAPRES_OFFRAMP, pp->mh->DAFLOW_OFFRAMP);
		//HYDR_CLS(pp->hy->HYDR_PORT_ADJUST, pp->mh->DAPRES_OFFRAMP, pp->mh->DAFLOW_OFFRAMP);
	}
}
ACT_RET AdjBwdBackgrd(PCLAMP pp)
{
	if(FSafeDoorIsCloseErrChk(pp->psfdr))                 //前安全门保护
	{
		return ACT_ERR_ABORT;
	}

	return ACT_OK;
}



ACT_RET ManualAdjBwd(PACT pact, UI32 para)
{
	int i;
	PADJUST	pp = (PADJUST)(pact->part);
	ACT_RET	ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;

	switch (pact->step)
	{
		case INIT1_STEP:
			pp->fadjposexchg = 0;
			if(TestPI(pp->pi->I_ADJ_BWD))
				ACT_STEP_NEW(END_STEP);
			else
			{
				if(ChkSysCfg(XF2_CPRBA))
				{
					if(ClampCoreStateErrChk(pp->pclmp, FALSE))   //增加调模进退前判断中子退终
				    	return ACT_ERR_ABORT;
					 else
						ACT_STEP_NEW(INIT1_STEP + 1);       //增加调模进退前判断脱模退终
				}
				else
					ACT_STEP_NEW(ADJBWDFAST);
			}
			break;

		case INIT1_STEP + 1:
			for(i = 0; i < CLAMP_MAX_EJECT; i++)
			{
				if(pp->pclmp->pejt[i] != NULL && EjectBwdErrChk(pp->pclmp->pejt[i], TRUE))
					ForkAct(pp->pclmp->pejt[i]->act + OS_EJTB - 1, SUSPEND_FORK, pact->ptsk, 0, 0);
			}
			ACT_STEP_NEW(INIT1_STEP + 2);
		    break;

		case INIT1_STEP + 2:
			for(i = 0; i < CLAMP_MAX_EJECT; i++)
			{
				if(pp->pclmp->pejt[i] != NULL )
					CLR_PART_ERR(pp->pclmp->pejt[i],ER0_EJTP);
			}
			ACT_STEP_NEW(ADJBWDFAST);
			break;
			
		case ADJBWDFAST:
			ResetPICnt(&pp->adj_chk_cnt, pp->pi->I_ADJ_CHK, 1);
			pp->adj_bwd_cnt = 0;
			pp->adj_bwd_onecyc = pp->adj_chk_cnt.onecyc;
			pp->timeoverb = 0;
			pp->cnt_start_100ms = Get100MsTick();
			pp->adj_start_10ms = Get10MsTick();
		case ADJBWDFAST + 1:
			if(MoveLocationNMControlProc(pp->pi->I_ADJ_BWD))
				ACT_STEP_NEW(END_STEP);
			else
			{
				if(ChkSysCfg(CO0_ADJZX))
				{
					PICntLoop(&pp->adj_chk_cnt);
					if(pp->adj_bwd_cnt != GetPICnt(&pp->adj_chk_cnt))
					{
						pp->adj_bwd_cnt = GetPICnt(&pp->adj_chk_cnt);
						pp->cnt_start_100ms = Get100MsTick();
						if(ChkSysCfg(CO0_FLWDOT))
							pp->st->ADPOSI_ADJREAL = pp->mh->ADPOSI_ADJCUR + pp->mh->CNPOSI_ADJPROPERTIONAL*pp->adj_bwd_cnt/100;
						else
							pp->st->ADPOSI_ADJREAL = pp->mh->ADPOSI_ADJCUR + pp->mh->CNPOSI_ADJPROPERTIONAL*pp->adj_bwd_cnt;
						pp->fadjposexchg = 1;
					}
					else if(AdjCntTMErrChk(pp))
						return ACT_ERR_ABORT;
				}
				else if(pp->pa->ADPOSI_ADJMD != 0)
				{
					if(pp->timeoverb == 0)
					{
					    if(pp->pa->CN_ADJFIRST > 0)
						{
							if((UI16)(Get10MsTick()-pp->adj_start_10ms) >= pp->pa->CN_ADJFIRST)
							{
					            SET_OFF(AdjBwdOff);
								pp->timeoverb = 1;
								pact->step_100ms = Get100MsTick();
						    }
						}
						else
						{
							if(pp->pa->CN_ADJADV > 0)
							{
								PICntLoop(&pp->adj_chk_cnt);
								if(pp->adj_bwd_cnt != GetPICnt(&pp->adj_chk_cnt))
								{
									pp->adj_bwd_cnt = GetPICnt(&pp->adj_chk_cnt);
									pp->cnt_start_100ms = Get100MsTick();
								}
								else if(AdjCntTMErrChk(pp))
									return ACT_ERR_ABORT;
			
								if(pp->adj_bwd_cnt >= pp->pa->CN_ADJADV)
								{
						            SET_OFF(AdjBwdOff);
									pp->timeoverb = 1;
									pact->step_100ms = Get100MsTick();
								}
							}
							else
							{
								PICntLoop(&pp->adj_chk_cnt);
						    	pp->adj_chk_cnt.bstart = 1;
								if(pp->adj_bwd_onecyc != pp->adj_chk_cnt.onecyc)
								{
									SET_OFF(AdjBwdOff);
									pp->timeoverb = 1;
									pact->step_100ms = Get100MsTick(); 
								}
								else if(AdjCntTMErrChk(pp))
									return ACT_ERR_ABORT;
							}
						}
					}
					else if(pp->timeoverb == 1)
					{
			            if(ACT_STEP_100MS >= 10)
						{
						    pp->timeoverb = 2;
							if(pp->pa->ADPOSI_ADJMD == 2)
						        SetAdjBwdFast(pp, pact);
						}
					}
				}
			}

			CHK_SET_MOV(SetAdjBwdFast);
			break;
		case END_STEP:
			//SET_PART_ERR(pp, ER0_ADJL);
			SET_PART_ERR(pp, ER0_ADJB);
			SET_OFF(AdjBwdOff);
			ret = ACT_END;
			break;
	}

	if(ret == ACT_END || ret == ACT_ERR_ABORT)
		return ret;
	else if(ChkSysCfg(CO11_ADJSTTUA))
		return AdjBwdBackgrd(pp->pclmp);
	else
		return ret;

}

ACT_RET AutoAdjFwd(PACT pact, UI32 para)
{
	PADJUST	pp = (PADJUST)(pact->part);
	ACT_RET	ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;

	switch (pact->step)
	{
		case INIT1_STEP:
//			pp->fadjposexchg = 0;
			if(TestPI(pp->pi->I_ADJ_FWD))
				ACT_STEP_NEW(END_STEP);
			else
			    ACT_STEP_NEW(ADJFWDFAST);
			break;
		case ADJFWDFAST:
			ResetPICnt(&pp->adj_chk_cnt, pp->pi->I_ADJ_CHK, 1);
			pp->adj_fwd_cnt = 0;
			pp->cnt_start_100ms = Get100MsTick();
        case ADJFWDFAST + 1:
			if(MoveLocationNMControlProc(pp->pi->I_ADJ_FWD))
				ACT_STEP_NEW(END_STEP);
			else
			{
				if(ChkSysCfg(CO0_ADJZX))
				{
					PICntLoop(&pp->adj_chk_cnt);
					if(pp->adj_fwd_cnt != GetPICnt(&pp->adj_chk_cnt))
					{
						pp->adj_fwd_cnt = GetPICnt(&pp->adj_chk_cnt);
						pp->cnt_start_100ms = Get100MsTick();
						if(ChkSysCfg(CO0_FLWDOT))
							pp->st->ADPOSI_ADJREAL = pp->mh->ADPOSI_ADJCUR - pp->mh->CNPOSI_ADJPROPERTIONAL*pp->adj_fwd_cnt/100;
						else
							pp->st->ADPOSI_ADJREAL = pp->mh->ADPOSI_ADJCUR - pp->mh->CNPOSI_ADJPROPERTIONAL*pp->adj_fwd_cnt;
						pp->fadjposexchg = 1;
					}
					else 
					{
						if(pp->st->ADPOSI_ADJREAL<=pp->mh->ADPOSI_ADJTARG)
						{
							ret = ACT_END;
							SET_OFF(AdjFwdOff);
						}
						if(AdjCntTMErrChk(pp))
						return ACT_ERR_ABORT;
					}
				}
				else
				{
					PICntLoop(&pp->adj_chk_cnt);
					if(pp->adj_fwd_cnt != GetPICnt(&pp->adj_chk_cnt))
					{
						pp->adj_fwd_cnt = GetPICnt(&pp->adj_chk_cnt);
						pp->cnt_start_100ms = Get100MsTick();

						if(ChkSysCfg(CO0_FLWDOT))
							pp->st->ADPOSI_ADJREAL = pp->mh->ADPOSI_ADJCUR - pp->mh->CNPOSI_ADJPROPERTIONAL*pp->adj_fwd_cnt/100;
						else
							pp->st->ADPOSI_ADJREAL = pp->mh->ADPOSI_ADJCUR - pp->mh->CNPOSI_ADJPROPERTIONAL*pp->adj_fwd_cnt;
						pp->fadjposexchg = 1;
					}
					else 
					{
						if(pp->adj_mode == 1)
						{
							if((UI16)(Get100MsTick()-pp->cnt_start_100ms)>=10)
							{
							   SET_OFF(AdjFwdOff);
							   pp->adj_mode = 1;
						       pp->adj_direct = 0;
						       ret = ACT_END;						
							}
						}
						else
						{
						    if(pp->adj_fwd_cnt>=pp->adj_next_fwd_cnt)
						    {			
							  SET_OFF(AdjFwdOff);
							  pp->adj_mode = 6;
						      pp->adj_direct = 0;
							  ret = ACT_END;
						    }
				            else if(AdjCntTMErrChk(pp))
						      return ACT_ERR_ABORT;							
						}
					}
				}
			}
		    CHK_SET_MOV(SetAdjFwdFast);
			break;
		case END_STEP:
			//SET_PART_ERR(pp, ER0_ADJL);
			SET_PART_ERR(pp, ER0_ADJF);
			SET_OFF(AdjFwdOff);
			ret = ACT_ERR_ABORT;
			break;
			
	}

	if(ret == ACT_END || ret == ACT_ERR_ABORT)
		return ret;
	else
		return AdjFwdBackgrd(pp->pclmp);
}

ACT_RET AutoAdjBwd(PACT pact, UI32 para)
{
	PADJUST	pp = (PADJUST)(pact->part);
	ACT_RET	ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;
    switch (pact->step)
    {
		case INIT1_STEP:
			if(TestPI(pp->pi->I_ADJ_BWD))
				ACT_STEP_NEW(END_STEP);
			else
				ACT_STEP_NEW(ADJBWDFAST);			
			break;
		case ADJBWDFAST:
			ResetPICnt(&pp->adj_chk_cnt, pp->pi->I_ADJ_CHK, 1);
			pp->adj_bwd_cnt = 0;
			pp->cnt_start_100ms = Get100MsTick();			
		case ADJBWDFAST+1:
			if(MoveLocationNMControlProc(pp->pi->I_ADJ_BWD))
				ACT_STEP_NEW(END_STEP);
			else
			{
				if(ChkSysCfg(CO0_ADJZX))
				{
					PICntLoop(&pp->adj_chk_cnt);
					if(pp->adj_bwd_cnt != GetPICnt(&pp->adj_chk_cnt))
					{
						pp->adj_bwd_cnt = GetPICnt(&pp->adj_chk_cnt);
						pp->cnt_start_100ms = Get100MsTick();
						if(ChkSysCfg(CO0_FLWDOT))
							pp->st->ADPOSI_ADJREAL = pp->mh->ADPOSI_ADJCUR+pp->mh->CNPOSI_ADJPROPERTIONAL*pp->adj_bwd_cnt/100;
						else
							pp->st->ADPOSI_ADJREAL = pp->mh->ADPOSI_ADJCUR+pp->mh->CNPOSI_ADJPROPERTIONAL*pp->adj_bwd_cnt;
						pp->fadjposexchg = 1;
					}
					else 
					{
						if(pp->st->ADPOSI_ADJREAL>=pp->mh->ADPOSI_ADJTARG)
						{
							ret = ACT_END;
							SET_OFF(AdjBwdOff);
						}
						if(AdjCntTMErrChk(pp))
						return ACT_ERR_ABORT;
					}
				}
				else
				{
					PICntLoop(&pp->adj_chk_cnt);
					if(pp->adj_bwd_cnt != GetPICnt(&pp->adj_chk_cnt))
					{
						pp->adj_bwd_cnt = GetPICnt(&pp->adj_chk_cnt);
						pp->cnt_start_100ms = Get100MsTick();

						if(ChkSysCfg(CO0_FLWDOT))
							pp->st->ADPOSI_ADJREAL = pp->mh->ADPOSI_ADJCUR+pp->mh->CNPOSI_ADJPROPERTIONAL*pp->adj_bwd_cnt/100;
						else
							pp->st->ADPOSI_ADJREAL = pp->mh->ADPOSI_ADJCUR+pp->mh->CNPOSI_ADJPROPERTIONAL*pp->adj_bwd_cnt;
						pp->fadjposexchg = 1;
					}
					else
					{
						
						if(pp->adj_bwd_cnt>=pp->adj_next_bwd_cnt)
						{
						   if(pp->adj_mode == 3)
						   {
						   	SET_OFF(AdjBwdOff);
							  pp->adj_mode = 5;	
							  ret = ACT_END;
							  	
						   }
						   else
						   {
						   	SET_OFF(AdjBwdOff);
							  pp->adj_mode = 4;
							  pp->adj_direct = 1;
							   ret = ACT_END;
						   }
						}
					    else if(AdjCntTMErrChk(pp))
						    return ACT_ERR_ABORT;					
					}
				}
			}
			CHK_SET_MOV(SetAdjBwdFast);			
			break;
		case END_STEP:
			//SET_PART_ERR(pp, ER0_ADJL);
			SET_PART_ERR(pp, ER0_ADJB);
			SET_OFF(AdjBwdOff);
			ret = ACT_ERR_ABORT;			
			break;
    }
	return ret;
}

ACT_RET AutoAdj(PACT pact, UI32 para)
{
	int i;
	PADJUST	pp = (PADJUST)(pact->part);
	PMACHINE   pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1)); 
	ACT_RET	ret = ACT_OK;

	if(pp == NULL)
	{
		ClrOperMode(OM_AA_FLG);
		return ACT_ERR_ABORT;
	}
	
	switch (pact->step)
	{
		case INIT1_STEP:
			if(CHK_PART_ERR(pp->pclmp->psfdr, ER3_FSER))
                return ACT_ERR_ABORT;
			else if(FSafeDoorIsOpen(pp->pclmp->psfdr))
	    	{
	      		CLR_PART_ERR(pp->pclmp->psfdr, ER0_OSDR);
	      		SET_PART_ERR(pp->pclmp->psfdr,  ER0_CSDR);
	      		ACT_STEP_NEW(INIT1_STEP + 1);    	
	    	}   
			else
                SET_PART_ERR(pp->pclmp->psfdr,  ER0_OSDR);
	   		break;
	   	
		case INIT1_STEP+1:
			if(CHK_PART_ERR(pp->pclmp->psfdr, ER3_FSER))
                return ACT_ERR_ABORT;
			else if(FSafeDoorIsClose(pp->pclmp->psfdr))
			{
				CLR_PART_ERR(pp->pclmp->psfdr, ER0_CSDR);
				if(ChkSysCfg(XF2_CPRBA))
					ACT_STEP_NEW(INIT1_STEP + 2);
				else if(ChkSysCfg(CO4_CKEY))
					ACT_STEP_NEW(INIT1_STEP + 5);
				else
					ACT_STEP_NEW(ADJCLSPRES);
			    pp->adj_mode = 0;
	   		}
			break;

		case INIT1_STEP + 2:
			 if(ClampCoreStateErrChk(pp->pclmp, FALSE))   //增加调模进退前判断中子退终
			    return ACT_ERR_ABORT;
			 else
				ACT_STEP_NEW(INIT1_STEP + 3);       //增加调模进退前判断脱模退终
			break;
			
		case INIT1_STEP + 3:
			for(i = 0; i < CLAMP_MAX_EJECT; i++)
			{
				if(pp->pclmp->pejt[i] != NULL && EjectBwdErrChk(pp->pclmp->pejt[i], TRUE))
					ForkAct(pp->pclmp->pejt[i]->act + OS_EJTB - 1, SUSPEND_FORK, pact->ptsk, 0, 0);
			}
			ACT_STEP_NEW(INIT1_STEP + 4);
		    break;

		case INIT1_STEP + 4:
			for(i = 0; i < CLAMP_MAX_EJECT; i++)
			{
				if(pp->pclmp->pejt[i] != NULL )
					CLR_PART_ERR(pp->pclmp->pejt[i],ER0_EJTP);
			}
			ACT_STEP_NEW(INIT1_STEP + 5);
			break;
			
		case INIT1_STEP + 5:
			if(ChkSysCfg(CO4_CKEY))
			{
				ClearRunKey();
				SET_PART_ERR(pm, ER0_SKEY);
				ACT_STEP_NEW(INIT1_STEP + 6);
			} 	
			else
				ACT_STEP_NEW(ADJCLSPRES);
			break;
			
		case INIT1_STEP + 6:
			if(GetRunKey())
			{
				CLR_PART_ERR(pm, ER0_SKEY);
				ClearRunKey();
				ACT_STEP_NEW(ADJCLSPRES);
			}
			break;

	   	case ADJCLSPRES:
			if(!ClampEjectBwdErrChk(pp->pclmp))	    //脱模退检测
			{
				if(pp->adj_mode ==0)
					pp->adj_mode = 5;
				ForkAct(pp->pclmp->act + OS_AAPRS- 1, SUSPEND_FORK, pact->ptsk, 0, 1000);
				ACT_STEP_NEW(ADJCLSPRES+1);	
		    }
	   		break;	
		case ADJCLSPRES+1:	
			if((pp->adj_mode == 1)) 		//||(pp->adj_mode == 3)) 锁紧后机器不能调退
			    ACT_STEP_NEW(ADJOPNSLOW+1);	
			else
			    ACT_STEP_NEW(ADJOPNSLOW);			
			break;
	    case ADJOPNSLOW:
			ForkAct(pp->pclmp->act + OS_AAOPN- 1, SUSPEND_FORK, pact->ptsk, 0,1000);
			ACT_STEP_NEW(ADJOPNSLOW+1);		   
	   		break;
	    case ADJOPNSLOW+1:
	   	    if(pp->adj_mode == 8)
			 	ACT_STEP_NEW(END_STEP);
			else		
			{
			    if(pp->adj_direct == 0)
				   ACT_STEP_NEW(ADJFWD);
			    else if(pp->adj_direct ==1)
				   ACT_STEP_NEW(ADJBWD);			
			}
			break;			
		case ADJFWD:
			ForkAct(pp->act + OS_ADJAF- 1, SUSPEND_FORK, pact->ptsk, 0,1000);
			ACT_STEP_NEW( ADJFWD+1);
			break;
		case ADJFWD+1:
			if(pp->adj_mode == 1)
			{
			   pp->adj_mode = 2;
			   pp->adj_direct = 0;
			   pp->adj_next_fwd_cnt = 16;
			   ACT_STEP_NEW(ADJOPNSLOW);
			}
			else
			   ACT_STEP_NEW(ADJCLSPRES);
			break;
		case ADJBWD:
			ForkAct(pp->act + OS_ADJAB- 1, SUSPEND_FORK, pact->ptsk, 0,1000);
			ACT_STEP_NEW(ADJBWD+1);
			break;
		case ADJBWD+1:
			ACT_STEP_NEW(ADJCLSPRES);
			break;
		case END_STEP:
			SET_PART_ERR(pp, ER0_FAAD);
			ClrOperMode(OM_AA_FLG);
			ret = ACT_END;
			break;
	}

	return ret;
}

ACT_RET AutoAdjZx(PACT pact, UI32 para)
{
	int i;
	PADJUST	pp = (PADJUST)(pact->part);
	PMACHINE   pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1)); 
	ACT_RET	ret = ACT_OK;
	if(pp == NULL)
	{
		ClrOperMode(OM_AA_FLG);
		return ACT_ERR_ABORT;
	}
	switch (pact->step)
	{
		case INIT1_STEP:
			if((pp->mh->ADPOSI_ADJTARG>pp->mh->ADPOSI_ADJMAX)||(pp->mh->ADPOSI_ADJTARG<pp->mh->ADPOSI_ADJMIN))
				ret =	ACT_ERR_ABORT;
		
			ACT_STEP_NEW(INIT1_STEP + 1);
			break;

		case INIT1_STEP + 1:
			 if(ClampCoreStateErrChk(pp->pclmp, FALSE))   //增加调模进退前判断中子退终
			    return ACT_ERR_ABORT;
			 else
				ACT_STEP_NEW(INIT1_STEP + 2);       //增加调模进退前判断脱模退终
			break;
			
		case INIT1_STEP + 2:
			for(i = 0; i < CLAMP_MAX_EJECT; i++)
			{
				if(pp->pclmp->pejt[i] != NULL && EjectBwdErrChk(pp->pclmp->pejt[i], TRUE))
					ForkAct(pp->pclmp->pejt[i]->act + OS_EJTB - 1, SUSPEND_FORK, pact->ptsk, 0, 0);
			}
			ACT_STEP_NEW(INIT1_STEP + 3);
		    break;

		case INIT1_STEP + 3:
			for(i = 0; i < CLAMP_MAX_EJECT; i++)
			{
				if(pp->pclmp->pejt[i] != NULL )
					CLR_PART_ERR(pp->pclmp->pejt[i],ER0_EJTP);
			}
			ACT_STEP_NEW(INIT1_STEP + 4);
			break;

		case INIT1_STEP + 4:
			if(ChkSysCfg(CO4_CKEY))
			{
				ClearRunKey();
				SET_PART_ERR(pm, ER0_SKEY);
				ACT_STEP_NEW(INIT1_STEP + 5);
			} 	
			else
				ACT_STEP_NEW(INIT2_STEP);
			break;
			
		case INIT1_STEP + 5:
			if(GetRunKey())
			{
				CLR_PART_ERR(pm, ER0_SKEY);
				ClearRunKey();
				ACT_STEP_NEW(INIT2_STEP);
			}
			break;
			
		case INIT2_STEP:
			if(pp->mh->ADPOSI_ADJCUR<pp->mh->ADPOSI_ADJTARG)
			{
				ForkAct(pp->act + OS_ADJAB- 1,SYNC_FORK, pact->ptsk, 0,500);
				ACT_STEP_NEW(INIT2_STEP+1);
				pp->adj_start = 0;
			}
			else if(pp->mh->ADPOSI_ADJCUR>pp->mh->ADPOSI_ADJTARG)	
			{
				ForkAct(pp->act + OS_ADJAF- 1,SYNC_FORK, pact->ptsk, 0,500);
				ACT_STEP_NEW(INIT2_STEP+1);
				pp->adj_start = 0;
			}
			else
			{
				ACT_STEP_NEW(END_STEP);
				pp->adj_start = 1;
			}
			break;	
		case INIT2_STEP+1:
			if(pp->adj_start == 1)
				ACT_STEP_NEW(END_STEP);
			break;	
		case END_STEP:
			SET_PART_ERR(pp, ER0_FAAD);
			pp->mh->ADPOSI_ADJCUR = pp->st->ADPOSI_ADJREAL;
			PartDataTx((PPART)pp, MH_ID, 1, GET_IND(PADJUST_MACHSET, ADPOSI_ADJCUR), FALSE);
			ClrOperMode(OM_AA_FLG);
			ret = ACT_END;
			break;			
	}
	return ret;
}

void	AdjMoldInit(BOOL bforce)		//	松模调模的参数初始化
{
	int i;
    PADJUST	pp = (PADJUST)GetPartByID(MK_PART(ADJUST_ID, 1));
	PMACHINE pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1));
	
	if(pp->adj_mold.fl_AdjMold == 0 || bforce)
	{
		pp->adj_mold.fl_AdjMold     = 0;		 //允许调模标记 0=不允许调模 1=允许调模    
		pp->adj_mold.AdjMoldDirect  = 0;      //松模时的调模方向   0=不调模 1=调模进 2=调模退 
		pp->adj_mold.Total			= 0;     //锁模力的累加器，用于计算锁模力的平均值
		pp->adj_mold.Avereg		    = 0;         //锁模力平均值，用于判断是否松模的参考
		pp->adj_mold.PresCurrent	= 0;     //当前模锁模力检测压力

		if((WORD)((GetSecTick())-pp->adj_mold.delay) >= 600 || bforce)
		{
			pp->adj_mold.cnt = 0;     //合模计数器 
			for(i=0;i<PRES_MEM_NUM;i++)
			{
			    pp->adj_mold.Presmemory[i]=0;
			}
		}
		
		pp->pclmp->st->DAPRES_CLSHI_MAX = 0;
		pp->pclmp->st->DAPRES_CLSHIAVR = 0;

		if(bforce)
		{
	    	pm->hy->DAPRES_CLSHIAVR = 0;
		    PartCfgSave((PPART)pm, HY_ID);
		}
		else
		{   
			//PartCfgRead((PPART)pm, ZD_ID);
		    if(pm->hy->DAPRES_CLSHIAVR > 0)
			{
				pp->pclmp->st->DAPRES_CLSHIAVR = pm->hy->DAPRES_CLSHIAVR;
				pp->adj_mold.fl_AdjMold = 1;
			}
		}
		
		PartDataTx((PPART)pp->pclmp, ST_ID, 2, GET_IND(PCLAMP_STATE, DAPRES_CLSHI_MAX), FALSE);
	}
}

void	GetClampclsStartPres(PACT pact,PADJUST pp)	//	获取锁模力检测时的开始压力
{
	//在高压锁模开始时获取压力作为锁模力起始压力
//    PADJUST	pp = (PADJUST)GetPartByID(MK_PART(ADJUST_ID, 1));
	PHYDR   ph = &g_hydrout;

	if (ChkSysCfg(CO0_SERVOP)&&ChkSysCfg(CO1_ECAT))//卓越系列使用Ecat通讯控制，压力环内置主机
	{
		pp->adj_mold.PresCurrent = GetPumpPress(HYDR_PORT(pp->pclmp->hy->HYDR_PORT_CLAMPCLS));
	}
	else
	{
		pp->adj_mold.PresCurrent = GetPresSensorValueByPumpNo(ph->mh->PRESSENSORCH-1);
	}
}

void	GetClampclsPresMax(PACT pact,PADJUST pp)	//	获取高压锁模的锁模力
{
	//在高压锁模过程中获取最大的压力值作为当前模的锁模力
//    PADJUST	pp = (PADJUST)GetPartByID(MK_PART(ADJUST_ID, 1));
	PHYDR   ph = &g_hydrout;
	int     pressensor;
	
	if (ChkSysCfg(CO0_SERVOP)&&ChkSysCfg(CO1_ECAT))//卓越系列使用Ecat通讯控制，压力环内置主机
	{
		pressensor = GetPumpPress(HYDR_PORT(pp->pclmp->hy->HYDR_PORT_CLAMPCLS));
	}
	else
	{
		pressensor = GetPresSensorValueByPumpNo(ph->mh->PRESSENSORCH-1);
	}
	
	pp->adj_mold.PresCurrent = (pp->adj_mold.PresCurrent>pressensor)?pp->adj_mold.PresCurrent:pressensor;
	if(pp->pclmp->st->DAPRES_CLSHI_MAX != pp->adj_mold.PresCurrent)
	{
	    pp->pclmp->st->DAPRES_CLSHI_MAX = pp->adj_mold.PresCurrent;
	    PartDataTx((PPART)pp->pclmp, ST_ID, 1, GET_IND(PCLAMP_STATE, DAPRES_CLSHI_MAX), FALSE);
	}
}

void  PresMid(WORD n)               //采集出的压力数据大小排序                               //2014-12-11新修改
{
	int i,j;
	WORD  temp;
	PADJUST	pp = (PADJUST)GetPartByID(MK_PART(ADJUST_ID, 1));	
	for(i=0;i<n-1;i++)
	{
		for(j=0;j<n-i-1;j++)
		{
			if(pp->adj_mold.Presmemory[j] > pp->adj_mold.Presmemory[j+1])
			{
				temp = pp->adj_mold.Presmemory[j];
				pp->adj_mold.Presmemory[j] = pp->adj_mold.Presmemory[j+1];
				pp->adj_mold.Presmemory[j+1] = temp;
			}
		}
	}
}

void	GetAvgPres(PADJUST pp)	//	记录关模次数并计算锁模力参考值
{
	//在高压锁模动作完毕后执行
//    PADJUST	pp = (PADJUST)GetPartByID(MK_PART(ADJUST_ID, 1));
	int i;
  	PMACHINE pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1));
	
	pp->adj_mold.delay = GetSecTick(); 
	pp->adj_mold.cnt++;

	if(pp->adj_mold.cnt >= 1 && pp->adj_mold.cnt <= PRES_MEM_NUM)
		pp->adj_mold.Presmemory[pp->adj_mold.cnt-1] = pp->adj_mold.PresCurrent;

	//pp->adj_mold.Total = pp->adj_mold.Total + pp->adj_mold.PresCurrent;
	if (pp->adj_mold.cnt >= PRES_MEM_NUM && pp->adj_mold.fl_AdjMold == 0)
	{
		pp->adj_mold.fl_AdjMold = 1;		 //允许调模标记 0=不允许调模 1=允许调模
		PresMid(PRES_MEM_NUM);
		for(i=60;i<140;i++)
		{
			pp->adj_mold.Total = pp->adj_mold.Total + pp->adj_mold.Presmemory[i];
		}
 		pp->adj_mold.Avereg = pp->adj_mold.Total / 80;
		pp->pclmp->st->DAPRES_CLSHIAVR = pp->adj_mold.Avereg;
        PartDataTx((PPART)pp->pclmp, ST_ID, 2, GET_IND(PCLAMP_STATE, DAPRES_CLSHI_MAX), FALSE);
		pm->hy->DAPRES_CLSHIAVR = pp->pclmp->st->DAPRES_CLSHIAVR;
		PartCfgSave((PPART)pm, HY_ID);
	}
}

void	CheckAdjMold(PADJUST pp)	//判断是否需要调模
{
	//在高压锁模动作完毕后执行
//    PADJUST	pp = (PADJUST)GetPartByID(MK_PART(ADJUST_ID, 1));
	if (pp->adj_mold.fl_AdjMold == 1)
	{
		if (pp->adj_mold.PresCurrent > (pp->pclmp->st->DAPRES_CLSHIAVR + pp->pclmp->md->DAPOSI_HIPRSCLAMPUP))	// 锁模力变大
			pp->adj_mold.AdjMoldDirect = 2;     //松模时的调模方向   0=不调模 1=调模进 2=调模退 
		else if ((pp->adj_mold.PresCurrent + pp->pclmp->md->DAPOSI_HIPRSCLAMPDOWN) < pp->pclmp->st->DAPRES_CLSHIAVR)	// 锁模力变大
			pp->adj_mold.AdjMoldDirect = 1;     //松模时的调模方向   0=不调模 1=调模进 2=调模退 
		else
			pp->adj_mold.AdjMoldDirect = 0;     //松模时的调模方向   0=不调模 1=调模进 2=调模退 
	}
}

ACT_RET AdjMoldExe(PACT pact, UI32 para)  //松模的自动调模的执行
{
	PADJUST	pp = (PADJUST)(pact->part);
	ACT_RET	ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;
	// 在关模前判断是否执行调模动作
	switch(pact->step)
	{
		case INIT1_STEP:
			ACT_STEP_NEW(INIT2_STEP);
			break;

		case INIT2_STEP:
	        if ((pp->adj_mold.fl_AdjMold == 1) && (pp->adj_mold.AdjMoldDirect == 1) && (pp->pa->TM_ADJUSTSLOW>0) && ChkOperMode(OM_AUTO))
			{
				ForkAct(pp->act + OS_ADJMOLDF - 1, SUSPEND_FORK, pact->ptsk, 0, 0);
			}
	        else if ((pp->adj_mold.fl_AdjMold == 1) && (pp->adj_mold.AdjMoldDirect == 2) && (pp->pa->TM_ADJUSTSLOW>0) && ChkOperMode(OM_AUTO))
			{
				ForkAct(pp->act + OS_ADJMOLDB - 1, SUSPEND_FORK, pact->ptsk, 0, 0);
			}
			ACT_STEP_NEW(END_STEP);
			break;

		case END_STEP:		
			ret = ACT_END;
		default:
			ret = ACT_END;
	}

	return ret;
}

ACT_RET ManualAdjMoldFwd(PACT pact, UI32 para)
{
	PADJUST	pp = (PADJUST)(pact->part);
	ACT_RET	ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;

	switch (pact->step)
	{
		case INIT1_STEP:
			pp->fadjposexchg = 0;
			if(TestPI(pp->pi->I_ADJ_FWD))
				ACT_STEP_NEW(END_STEP);
			else
				ACT_STEP_NEW(ADJFWDFAST);

			break;
			
		case ADJFWDFAST:
			ResetPICnt(&pp->adj_chk_cnt, pp->pi->I_ADJ_CHK, 1);
			pp->adj_fwd_cnt = 0;
			pp->cnt_start_100ms = Get100MsTick();
		case ADJFWDFAST + 1:
			if(MoveLocationTM10msControlProc(pp->pa->TM_ADJUSTSLOW))
				ACT_STEP_NEW(END_STEP);

			PICntLoop(&pp->adj_chk_cnt);
			if(pp->adj_fwd_cnt != GetPICnt(&pp->adj_chk_cnt))
			{
				pp->adj_fwd_cnt = GetPICnt(&pp->adj_chk_cnt);
				pp->cnt_start_100ms = Get100MsTick();
			}
			else if(AdjCntTMErrChk(pp))
				return ACT_ERR_ABORT;

			if(pp->adj_fwd_cnt >= 1)
				ACT_STEP_NEW(END_STEP);

			
			if(TestPI(pp->pi->I_ADJ_FWD))
				ACT_STEP_NEW(END_STEP + 1);

			CHK_SET_MOV(SetAdjFwdFast);
			break;

		case END_STEP:
			AdjFwdOff(pp, pact);
			ret = ACT_END;
			break;
		case END_STEP + 1:
			//SET_PART_ERR(pp, ER0_ADJL);
			SET_PART_ERR(pp, ER0_ADJF);
			AdjFwdOff(pp, pact);
			ret = ACT_END;
			break;
		default:
			ret = ACT_END;
			break;  
	}
 	return ret;
}

ACT_RET ManualAdjMoldBwd(PACT pact, UI32 para)
{
	PADJUST	pp = (PADJUST)(pact->part);
	ACT_RET	ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;

	switch (pact->step)
	{
		case INIT1_STEP:
			pp->fadjposexchg = 0;
			if(TestPI(pp->pi->I_ADJ_BWD))
				ACT_STEP_NEW(END_STEP);
			else
				ACT_STEP_NEW(ADJBWDFAST);

			break;
			
		case ADJBWDFAST:
			ResetPICnt(&pp->adj_chk_cnt, pp->pi->I_ADJ_CHK, 1);
			pp->adj_bwd_cnt = 0;
			pp->cnt_start_100ms = Get100MsTick();
		case ADJBWDFAST + 1:
			if(MoveLocationTM10msControlProc(pp->pa->TM_ADJUSTSLOW))
				ACT_STEP_NEW(END_STEP);

			PICntLoop(&pp->adj_chk_cnt);
			if(pp->adj_bwd_cnt != GetPICnt(&pp->adj_chk_cnt))
			{
				pp->adj_bwd_cnt = GetPICnt(&pp->adj_chk_cnt);
				pp->cnt_start_100ms = Get100MsTick();
			}
			else if(AdjCntTMErrChk(pp))
				return ACT_ERR_ABORT;

			if(pp->adj_bwd_cnt >= 1)
				ACT_STEP_NEW(END_STEP);

			if(TestPI(pp->pi->I_ADJ_BWD))
				ACT_STEP_NEW(END_STEP + 1);

			CHK_SET_MOV(SetAdjBwdFast);
			break;

		case END_STEP:
			AdjBwdOff(pp, pact);
			ret = ACT_END;
			break;
		case END_STEP + 1:
			//SET_PART_ERR(pp, ER0_ADJL);
			SET_PART_ERR(pp, ER0_ADJB);
			AdjBwdOff(pp, pact);
			ret = ACT_END;
			break;
		default:
			ret = ACT_END;
			break;  
	}
 	return ret;
}

ACT_RET AutoAdjProc(PACT pact, UI32 para)
{
	PADJUST	pp = (PADJUST)(pact->part);
	ACT_RET	ret = ACT_OK;
	
	if(pp == NULL)
		return ACT_ERR_ABORT;
	if(ChkSysCfg(CO0_ADJZX))
		ret = AutoAdjZx(pact,para);
	else
		ret =AutoAdj(pact,para);
	return ret;
}

void AutoAdjPosLearnOff(PADJUST pp, PACT pact)
{
	AdjBwdOff(pp, pact);
	AdjFwdOff(pp, pact);

	SetOperMode(OM_ADJU);
}

ACT_RET AutoAdjPosLearn(PACT pact, UI32 para)
{
	PADJUST	pp = (PADJUST)(pact->part);
	PMACHINE   pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1));
	ACT_RET	ret = ACT_OK;
	UI16 i = 0;
	UI16 cnposi = 0;//齿距 0.001mm单位

	if(pp == NULL)
		return ACT_ERR_ABORT;

	switch (pact->step)
	{
		case INIT1_STEP:
			if(ClampOpenReachErrChk(pp->pclmp))	//让用户手动开模
				return ACT_ERR_ABORT;

			SetOperMode(OM_AA_LEARN);

			if(CHK_PART_ERR(pp->pclmp->psfdr, ER3_FSER))
                return ACT_ERR_ABORT;
			else if(FSafeDoorIsOpen(pp->pclmp->psfdr))
	    	{
	      		CLR_PART_ERR(pp->pclmp->psfdr, ER0_OSDR);
	      		SET_PART_ERR(pp->pclmp->psfdr,  ER0_CSDR);
	      		ACT_STEP_NEW(INIT1_STEP + 1);    	
	    	}   
			else
                SET_PART_ERR(pp->pclmp->psfdr,  ER0_OSDR);
	   		break;
	   	
		case INIT1_STEP+1:
			if(CHK_PART_ERR(pp->pclmp->psfdr, ER3_FSER))
                return ACT_ERR_ABORT;
			else if(FSafeDoorIsClose(pp->pclmp->psfdr))
			{
				CLR_PART_ERR(pp->pclmp->psfdr, ER0_CSDR);
				if(ChkSysCfg(XF2_CPRBA))
					ACT_STEP_NEW(INIT1_STEP + 2);
				else if(ChkSysCfg(CO4_CKEY))
					ACT_STEP_NEW(INIT1_STEP + 5);
				else
					ACT_STEP_NEW(ADJBWD);
			    pp->mode_adjposlearn = 0;
	   		}
			break;

		case INIT1_STEP + 2:
			 if(ClampCoreStateErrChk(pp->pclmp, FALSE))   //增加调模进退前判断中子退终
			    return ACT_ERR_ABORT;
			 else
				ACT_STEP_NEW(INIT1_STEP + 3);       //增加调模进退前判断脱模退终
			break;
			
		case INIT1_STEP + 3:
			for(i = 0; i < CLAMP_MAX_EJECT; i++)
			{
				if(pp->pclmp->pejt[i] != NULL && EjectBwdErrChk(pp->pclmp->pejt[i], TRUE))
					ForkAct(pp->pclmp->pejt[i]->act + OS_EJTB - 1, SUSPEND_FORK, pact->ptsk, 0, 0);
			}
			ACT_STEP_NEW(INIT1_STEP + 4);
		    break;

		case INIT1_STEP + 4:
			for(i = 0; i < CLAMP_MAX_EJECT; i++)
			{
				if(pp->pclmp->pejt[i] != NULL )
					CLR_PART_ERR(pp->pclmp->pejt[i],ER0_EJTP);
			}
			ACT_STEP_NEW(INIT1_STEP + 5);
			break;
			
		case INIT1_STEP + 5:
			if(ChkSysCfg(CO4_CKEY))
			{
				ClearRunKey();
				SET_PART_ERR(pm, ER0_SKEY);
				ACT_STEP_NEW(INIT1_STEP + 6);
			} 	
			else
				ACT_STEP_NEW(ADJBWD);
			break;
			
		case INIT1_STEP + 6:
			if(GetRunKey())
			{
				CLR_PART_ERR(pm, ER0_SKEY);
				ClearRunKey();
				ACT_STEP_NEW(ADJBWD);
			}
			break;
			
		case ADJBWD:
			ResetPICnt(&pp->adj_chk_cnt, pp->pi->I_ADJ_CHK, 1);
			pp->adj_bwd_cnt = 0;
			pp->cnt_start_100ms = Get100MsTick();
            pp->adj_start_10ms = Get10MsTick();
		case ADJBWD + 1:
			if (STEP_10MS_CHK(50))
			{
				PICntLoop(&pp->adj_chk_cnt);
				if(pp->adj_bwd_cnt != GetPICnt(&pp->adj_chk_cnt))
				{
					pp->adj_bwd_cnt = GetPICnt(&pp->adj_chk_cnt);
					pp->cnt_start_100ms = Get100MsTick();
				}

				if((DIFF_100MS(pp->cnt_start_100ms) > ADJCHKERRTIME) || TestPI(pp->pi->I_ADJ_BWD))
				{
					if (pp->mode_adjposlearn == 2)
					{
						if (DIFF_10MS(pp->adj_start_10ms) < ADJCHKERRTIME_LEARN)//如果调模退时间 < ADJCHKERRTIME_LEARN 则认为异常
						{
							SET_PART_ERR(pp,ER0_ADJS);
							return ACT_ERR_ABORT;
						}
						else
						{
							ACT_STEP_NEW(END_STEP);
						}
					}
					else
                    {
                        pp->mode_adjposlearn = 1;
						ACT_STEP_NEW(ADJFWD);
                    }
						
				}

				CHK_SET_MOV(SetAdjBwdFast);
			}
			break;
			
		case ADJFWD:
			ResetPICnt(&pp->adj_chk_cnt, pp->pi->I_ADJ_CHK, 1);
			pp->adj_fwd_cnt = 0;
			pp->cnt_start_100ms = Get100MsTick();
            pp->adj_start_10ms = Get10MsTick();
		case ADJFWD + 1:
			if (STEP_10MS_CHK(50))
			{
				PICntLoop(&pp->adj_chk_cnt);
				if(pp->adj_fwd_cnt != GetPICnt(&pp->adj_chk_cnt))
				{
					pp->adj_fwd_cnt = GetPICnt(&pp->adj_chk_cnt);
					pp->cnt_start_100ms = Get100MsTick();
				}

				if((DIFF_100MS(pp->cnt_start_100ms) > ADJCHKERRTIME) || TestPI(pp->pi->I_ADJ_FWD))
				{
					if (DIFF_10MS(pp->adj_start_10ms) < ADJCHKERRTIME_LEARN)//如果调模进时间 < ADJCHKERRTIME_LEARN 则认为异常
					{
						SET_PART_ERR(pp,ER0_ADJS);
						return ACT_ERR_ABORT;
					}
					else
					{
						pp->mode_adjposlearn = 2;
						ACT_STEP_NEW(ADJBWD);
					}
				}

				CHK_SET_MOV(SetAdjFwdFast);
			}
			break;

		case END_STEP:
			AdjBwdOff(pp, pact);

			SetOperMode(OM_ADJU);

			if (ABS_DIFF(pp->adj_bwd_cnt ,pp->adj_fwd_cnt) > ADJCHKERRCN_LEARN)
			{
				SET_PART_ERR(pp,ER0_ADJERRCN);
				return ACT_ERR_ABORT;
			}
			else
			{
				cnposi = ((UI32)(pp->mh->ADPOSI_ADJMAX - pp->mh->ADPOSI_ADJMIN))*100*2/(UI32)(pp->adj_bwd_cnt + pp->adj_fwd_cnt);
				if(ChkSysCfg(CO0_FLWDOT))
					pp->mh->CNPOSI_ADJPROPERTIONAL = cnposi;
				else
					pp->mh->CNPOSI_ADJPROPERTIONAL = cnposi/100;

				pp->mh->ADPOSI_ADJCUR = pp->mh->ADPOSI_ADJMAX;
				pp->st->ADPOSI_ADJREAL = pp->mh->ADPOSI_ADJCUR;
				
				PartDataTx((PPART)pp, MH_ID, 2, GET_IND(PADJUST_MACHSET, CNPOSI_ADJPROPERTIONAL), FALSE);
                SET_PART_ERR(pp,ER0_ADJCNL);
				ret = ACT_END;
			}
			break;
		default:
			ret = ACT_END;
			break;  
	}
 	return ret;
}


void AdjustSetup(PADJUST pp)
{
	if(pp != NULL)
		pp->pclmp = (PCLAMP)GetMachineSubPart(CLAMP_ID, GET_PART_SN(pp->part.id));
}

void AdujstInit(PADJUST pp)
{
	if(pp != NULL)
	{
		pp->adj_bwd_cnt = 0;
		pp->adj_fwd_cnt = 0;
		pp->cnt_start_100ms = 0;
		pp->fadjposexchg = 0;
#if 0	
		g_adjust.FAdjMode  = 0;      // 0 TimeMode
		g_adjust.cntstart  = 0;      // adjust count start time
		g_adjust.cnlast    = 0;      // adjust count on end
		g_adjust.cnadjfwd  = 0;      // adjust fwd count once
		g_adjust.cnadjfwdnext = 0;  
		g_adjust.cnadjbwd  = 0; 
		g_adjust.cnadjbwdnext = 0;
#endif
	}
}

BOOL InitAdjustPart(PADJUST pp, UI8 sn)
{
	static const SUB_INIT_INFO sub_init[]  = ADJUST_INIT_INFO;
	static const ACT_INIT_INFO act_init[] = {{OS_ADJF, OS_ADJF, ACT_NTYPE, ManualAdjFwd, AdjFwdOff},
									   {OS_ADJB, OS_ADJB, ACT_NTYPE, ManualAdjBwd, AdjBwdOff},
									   {OS_ADJAF, OS_ADJF, ACT_NTYPE, AutoAdjFwd, AdjFwdOff},
									   {OS_ADJAB, OS_ADJB, ACT_NTYPE, AutoAdjBwd, AdjBwdOff},
									   {OS_ADJA, OS_ADJA, ACT_CTRB, AutoAdjProc, AutoAdjoff},
									   {OS_ADJMOLDF, OS_ADJMOLDF, ACT_NTYPE, ManualAdjMoldFwd, AdjFwdOff},
									   {OS_ADJMOLDB, OS_ADJMOLDB, ACT_NTYPE, ManualAdjMoldBwd, AdjBwdOff},
									   {OS_ADJMOLD, OS_ADJMOLD, ACT_CTRL, AdjMoldExe, NULL},
									   {OS_ADAJMOLD,OS_ADAJMOLD,ACT_CTRL,NULL,NULL},
									   {OS_ADJPOSLEARN, OS_ADJPOSLEARN, ACT_CTRL,AutoAdjPosLearn,AutoAdjPosLearnOff}};
	
	if(!INIT_PART(pp, ADJUST_ID, sn, sub_init))
		return FALSE;

	if(!INIT_ACT(pp, act_init))
		return FALSE;

	AdjustSetup(pp);
	AdujstInit(pp);
	return TRUE;
}

UI32 AdjustCmd(PADJUST pp, UI32 cmd, UI32 para)
{
	UI16 offset = 0;
	UI16 len = 0;
	if(pp == NULL)
		return FALSE;

	switch(GET_CMD(cmd))
	{
		case CMD_MANUAL_INIT:
			if(pp->fadjposexchg == 1)
				pp->mh->ADPOSI_ADJCUR = pp->st->ADPOSI_ADJREAL;
			else
				pp->st->ADPOSI_ADJREAL = pp->mh->ADPOSI_ADJCUR;
			AdujstInit(pp);
			break;
			
		case CMD_SETUP:
			AdjustSetup(pp);
			break;

		case CMD_DATA_RX:
			offset = (para >> 16 & 0xffff);
            len = (para & 0xffff);
			
			if ((GET_CMD_PARA(cmd) & 0xff) == MH_ID)
			{	
				if (len > 0 &&(GET_IND(PADJUST_MACHSET, ADPOSI_ADJCUR) >= offset && GET_IND(PADJUST_MACHSET, ADPOSI_ADJCUR) <= offset + len))
				{
					pp->st->ADPOSI_ADJREAL = pp->mh->ADPOSI_ADJCUR;	
				}
			}
		break;
	}

	return TRUE;
}

