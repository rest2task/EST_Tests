/******************************************************************************
  文 件 名   : rotate.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 滑模部件及动作
  函数列表   :
              InitRotatePart
              RotateBackgrd
              RotateBwd
              RotateBwdBackgrd
              RotateBwdPreChk
              RotateClampClsErrChk
              RotateCmd
              RotateFwd
              RotateFwdPreChk
              RotatePIErrChk
              RotateSetup
              SetRotateBwdFst
              SetRotateBwdoff
              SetRotateBwdSlow
              SetRotateFwdFst
              SetRotateFwdoff
              SetRotateFwdSlow
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
#include "clamp.h"
#include "eject.h"
#include "core.h"
#include "safedoor.h"
#include "robot.h"
#include "rotate.h"
#include "lock.h"
#include "machine.h"
#include "servo.h"
extern   MACHINE m_machine;
BOOL RotatePIErrChk(PROTATE pp, BOOL bchk_fwd)
{
	if(pp != NULL)
	{
		if(GetSysConfig()->FL_ROTLAC && ChkSysCfg(CO43_ROTLAC) &&  ((!bchk_fwd && CheckPI(pp->pi->I_ROT_BWD)== PI_RESET) ||(bchk_fwd && CheckPI(pp->pi->I_ROT_FWD)== PI_RESET)))
		{
			if(bchk_fwd)
			{
				SET_PART_ERR(pp, ER0_ROTATEF);
			}
			else
			{
				SET_PART_ERR(pp, ER0_ROTATEB);
			}			
			StandAlarm(FALSE);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL RotateClampClsErrChk(PROTATE pp)
{
	if(pp != NULL && pp->last_act_sn != 0)
		return RotatePIErrChk(pp, pp->last_act_sn != OS2_RTTB);
    else if( pp->last_act_sn == 0)
		return RotatePIErrChk(pp, 0);
	return FALSE;
}

void SetRotateFwdFst(PROTATE pp, PACT pact)
{
	UI16 hydelay;
	
	hydelay = pp->mh->SCLF_HYDELAY;

	SetPOBit(pp->po->O_ROT_FWD, PC_ON);
	SetPOBit(pp->po->O_ROT_FWDS, PC_ON);

	if(ChkOperMode(OM_ADJU))
		SetPOBit(pp->po->O_ROTSLOW, PC_ON);

	ActSetHydr(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_SCLPFFAST, pp->pa->DAFLOW_SCLPFFAST, 0, 0, pp->mh->DAPRES_SCLPRAMP, pp->mh->DAFLOW_SCLPRAMP, HYDRDELY, 
						 HYDR_PORT(pp->hy->HYDR_PORT_ROTATEFWD),   pp->hy->HYDR_PUMPS_ROTATEFWD, pp->hy->HYDR_PUMPS_ROTATEFWD,    pp->hy->SERVO_KP_ROTATEFWD,pp->hy->SERVO_KI_ROTATEFWD, 
						 0, TRUE, HYDR_PRI(pp->hy->HYDR_PORT_ROTATEFWD));

	ActSetTM(pact->ptsk, pp->mh->TM_ROTLOC);
}

void SetRotateFwdSlow(PROTATE pp, PACT pact)
{
	UI16 hydelay;
	
	hydelay = pp->mh->SCLF_HYDELAY;
	SetPOBit(pp->po->O_ROT_FWD, PC_ON);
	SetPOBit(pp->po->O_ROT_FWDS, PC_OFF);

	ActSetHydr(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_SCLPFLOW, pp->pa->DAFLOW_SCLPFLOW, 0, 0, pp->mh->DAPRES_SCLPRAMP, pp->mh->DAFLOW_SCLPRAMP, HYDRDELY, 
						 HYDR_PORT(pp->hy->HYDR_PORT_ROTATEFWD),   pp->hy->HYDR_PUMPS_ROTATEFWD, pp->hy->HYDR_PUMPS_ROTATEFWD,    pp->hy->SERVO_KP_ROTATEFWD,pp->hy->SERVO_KI_ROTATEFWD, 
						 0, TRUE, HYDR_PRI(pp->hy->HYDR_PORT_ROTATEFWD));
	
  	ActSetTM(pact->ptsk, pp->mh->TM_ROTLOC);
}

void SetRotateFwdoff(PROTATE pp, PACT pact)
{
	if(pp != NULL)
	{
		HYDR_CLS(pp->mh->DAPRES_SCLPOFFRAMP, pp->mh->DAFLOW_SCLPOFFRAMP);
		SetPOBit(pp->po->O_ROT_FWD, PC_OFF);
		SetPOBit(pp->po->O_ROTSLOW, PC_OFF);
		SetPOBit(pp->po->O_ROT_FWDS, PC_OFF);
	}
}

void SetRotateFwdoffDelay(PROTATE pp, PACT pact)
{
	if(pp != NULL)
	{
		HYDR_CLS(pp->mh->DAPRES_SCLPOFFRAMP, pp->mh->DAFLOW_SCLPOFFRAMP);
		SetPOBit(pp->po->O_ROT_FWD, PC_OFF);
		SetPOBit(pp->po->O_ROTSLOW, PC_OFF);
		SetPOBit(pp->po->O_ROT_FWDS, PC_OFF);
	}
}

ACT_RET RotateBackgrd(PROTATE pp , PACT pact)
{
	if(pact->step >= STAGE_1_STEP && pact->step < END_STEP && LockBwdErrChk(pp->plck))
		return ACT_ERR_ABORT;
	
	if(pact->step >= STAGE_1_STEP && ACT_RUN_MS >= (UI32)pp->mh->TM_ROTLOC*100)
	{
		SET_PART_ERR(pp, ER0_MTHK);
		StandAlarm(FALSE);
		return ACT_ERR_ABORT;	
	}
	
	return ACT_OK;
}

ACT_RET RotateFwdPreChk(PROTATE pp)
{
	if(pp->pclmp!=NULL&& ClampOpenReachErrChk(pp->pclmp))
		return ACT_ERR_ABORT;

	if(ClampEjectBwdErrChk(pp->pclmp)&&!ChkSysCfg(XF0_EJTTM))
		return ACT_ERR_ABORT;
		
	if(ClampCoreStateErrChk(pp->pclmp, FALSE))
		return ACT_ERR_ABORT;


	//直角机检测双滑模式 ，检测另外一个滑模是否退到位。
	if(GetSysConfig()->FL_ROTLAC==2 && pp->pother_rtt != NULL && CheckPI(pp->pother_rtt->pi->I_ROT_BWD)==PI_RESET)
	{
		SET_PART_ERR(pp->pother_rtt, ER0_ROTATEB);
		StandAlarm(FALSE);
		return ACT_ERR_ABORT;
	}  

	return ACT_OK;
}

ACT_RET RotateFwd(PACT pact, UI32 para)
{
	PROTATE pp = (PROTATE)pact->part;
    ACT_RET	ret = ACT_OK;
	int tmp;

	if(pp == NULL)
		return ACT_ERR_ABORT;
	switch (pact->step) 
	{
		case INIT1_STEP:
			if(GetSysConfig()->FL_ROTLAC && ChkSysCfg(CO43_ROTLAC))
			{
				if((ret = RotateFwdPreChk(pp)) != ACT_OK)
					return ret;
				else
					ACT_STEP_NEW(INIT1_STEP + 1);
			}
			else
				return ACT_ERR_ABORT;   
			break;
			
    	case INIT1_STEP + 1: 
			// 保护机械手  05-12-31
			if(pp->pclmp != NULL && !RobotRotateErrChk(pp->pclmp->prbt))
				ACT_STEP_NEW(INIT2_STEP);
    		break;

		case INIT2_STEP:
			tmp = GetMeterLenByCh(pp->mh->ROTATE_METERCH);
			if(STEP_10MS_CHK(pp->mh->TM_SCLPFDLY))
			{
				ACT_START();
				pp->last_act_sn = OS_RTTF;
				if(tmp>0)
				{
					if(GetMeterPosByCh(pp->mh->ROTATE_METERCH)>= pp->pa->ADPOSI_SCLPFFAST)
						ACT_STEP_NEW(ROTATEFWDSLW);
					else
						ACT_STEP_NEW(ROTATEFWD);
				}
				else
				{
					if(CheckPI(pp->pi->I_ROT_FWS) == PI_SET)
						ACT_STEP_NEW(ROTATEFWDSLW);
					else if(TestPI(pp->pi->I_ROT_FWD))
						ACT_STEP_NEW(END_STEP);
					else if(TestPI(pp->pi->I_ROT_BWD))
						ACT_STEP_NEW(ROTATEFWD);
					else
						ACT_STEP_NEW(ROTATEFWDSETUP);
				}
			}
			break;
		case ROTATEFWDSETUP:
		case ROTATEFWDSETUP + 1:
			if(CheckPI(pp->pi->I_ROT_FWS) == PI_SET)
				ACT_STEP_NEW(ROTATEFWDSLW);
			else if(TestPI(pp->pi->I_ROT_FWD))
				ACT_STEP_NEW(END_STEP);
			else if(TestPI(pp->pi->I_ROT_BWD))
				ACT_STEP_NEW(ROTATEFWD);

			CHK_SET_MOV(SetRotateFwdSlow);
			break;

 		case ROTATEFWD:
			if(pp->mh->TM_ROTATEFAST > 1000)
				pp->mh->TM_ROTATEFAST = 1000;
		case ROTATEFWD+1:
			tmp = GetMeterLenByCh(pp->mh->ROTATE_METERCH);
			if((tmp> 0 && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->ROTATE_METERCH), pp->pa->ADPOSI_SCLPFFAST)) ||
			  (tmp<= 0 && MoveLocationNMControlProc(pp->pi->I_ROT_FWS) ) ||  (pp->mh->TM_ROTATEFAST > 0 && STEP_10MS_CHK(pp->mh->TM_ROTATEFAST)))
				ACT_STEP_NEW(ROTATEFWDSLW);
			else if(tmp<= 0 && MoveLocationNMControlProc(pp->pi->I_ROT_FWD))
				ACT_STEP_NEW(END_STEP);
			
			CHK_SET_MOV(SetRotateFwdFst);
			break;
			
		case ROTATEFWDSLW:
		case ROTATEFWDSLW+1:	
			tmp = GetMeterLenByCh(pp->mh->ROTATE_METERCH);
			if((tmp> 0 && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->ROTATE_METERCH), pp->pa->ADPOSI_SCLPFFAST)) ||
			  (tmp<= 0 && MoveLocationNMControlProc(pp->pi->I_ROT_FWD) ))
			  ACT_STEP_NEW(END_STEP);
			CHK_SET_MOV(SetRotateFwdSlow);
			break;
			
		case END_STEP:
			SET_OFF(SetRotateFwdoffDelay);
			pp->slidests = 1;
			ret = ACT_END;
            break;

 		default:
			ret = ACT_END;
			break;
	}
	if(ret == ACT_END || ret == ACT_ERR_ABORT)
	{
	    return ret;
    }
	else
       return RotateBackgrd(pp, pact);
}

void SetRotateBwdFst(PROTATE pp, PACT pact)
{
	UI16 hydelay;
	
	hydelay = pp->mh->SCLB_HYDELAY;
	SetPOBit(pp->po->O_ROT_BWD, PC_ON);
	SetPOBit(pp->po->O_ROT_BWDS, PC_ON);

	if(ChkOperMode(OM_ADJU))
		SetPOBit(pp->po->O_ROTSLOW, PC_ON);

	ActSetHydr(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_SCLPBFAST, pp->pa->DAFLOW_SCLPBFAST, 0, 0, pp->mh->DAPRES_SCLPRAMP, pp->mh->DAFLOW_SCLPRAMP, HYDRDELY, 
						 HYDR_PORT(pp->hy->HYDR_PORT_ROTATEBWD),   pp->hy->HYDR_PUMPS_ROTATEBWD, pp->hy->HYDR_PUMPS_ROTATEBWD,    pp->hy->SERVO_KP_ROTATEBWD,pp->hy->SERVO_KI_ROTATEBWD, 
						 0, TRUE, HYDR_PRI(pp->hy->HYDR_PORT_ROTATEBWD));

	ActSetTM(pact->ptsk, pp->mh->TM_ROTLOC);
}

void SetRotateBwdSlow(PROTATE pp, PACT pact)
{
	UI16 hydelay;
	
	hydelay = pp->mh->SCLB_HYDELAY;
	SetPOBit(pp->po->O_ROT_BWD, PC_ON);
	SetPOBit(pp->po->O_ROT_BWDS, PC_OFF);

	ActSetHydr(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_SCLPBLOW, pp->pa->DAFLOW_SCLPBLOW, 0, 0, pp->mh->DAPRES_SCLPRAMP, pp->mh->DAFLOW_SCLPRAMP, HYDRDELY, 
						 HYDR_PORT(pp->hy->HYDR_PORT_ROTATEBWD),   pp->hy->HYDR_PUMPS_ROTATEBWD, pp->hy->HYDR_PUMPS_ROTATEBWD,    pp->hy->SERVO_KP_ROTATEBWD,pp->hy->SERVO_KI_ROTATEBWD, 
						 0, TRUE, HYDR_PRI(pp->hy->HYDR_PORT_ROTATEBWD));
	
  	ActSetTM(pact->ptsk, pp->mh->TM_ROTLOC);
}

void SetRotateBwdoff(PROTATE pp, PACT pact)
{
	if(pp != NULL)
	{
		HYDR_CLS(pp->mh->DAPRES_SCLPOFFRAMP, pp->mh->DAFLOW_SCLPOFFRAMP);
		SetPOBit(pp->po->O_ROT_BWD, PC_OFF);
		SetPOBit(pp->po->O_ROTSLOW, PC_OFF);
		SetPOBit(pp->po->O_ROT_BWDS, PC_OFF);
	}
}

void SetRotateBwdoffDelay(PROTATE pp, PACT pact)
{
	if(pp != NULL)
	{
		HYDR_CLS(pp->mh->DAPRES_SCLPOFFRAMP, pp->mh->DAFLOW_SCLPOFFRAMP);
		SetPOBit(pp->po->O_ROT_BWD, PC_OFF);
		SetPOBit(pp->po->O_ROTSLOW, PC_OFF);
		SetPOBit(pp->po->O_ROT_BWDS, PC_OFF);
	}
}


ACT_RET RotateBwdBackgrd(PROTATE pp , PACT pact)
{
	if(pp->pclmp != NULL && RobotClampClsProtectErrChk(pp->pclmp->prbt)) 
		return ACT_ERR_ABORT;	

	if(pp->pclmp != NULL && FSafeDoorIsCloseErrChk(pp->pclmp->psfdr))                 //前安全门保护
		return ACT_ERR_ABORT;

	return ACT_OK;
}

ACT_RET RotateBwdPreChk(PROTATE pp)
{
	if(pp->pclmp!=NULL&& ClampOpenReachErrChk(pp->pclmp))
		return ACT_ERR_ABORT;

	if(ClampEjectBwdErrChk(pp->pclmp)&&!ChkSysCfg(XF0_EJTTM))
		return ACT_ERR_ABORT;
		
	if(ClampCoreStateErrChk(pp->pclmp, FALSE))
		return ACT_ERR_ABORT;

	return ACT_OK;
}



ACT_RET RotateBwd(PACT pact, UI32 para)
{
	PROTATE pp = (PROTATE)pact->part;
	ACT_RET	ret = ACT_OK;
	int tmp;
	
	if(pp == NULL)
		return ACT_ERR_ABORT;
	switch (pact->step) 
	{
		case INIT1_STEP:
			if(GetSysConfig()->FL_ROTLAC && ChkSysCfg(CO43_ROTLAC))
			{
				if((ret = RotateBwdPreChk(pp)) != ACT_OK)
					return ret;
				else
					ACT_STEP_NEW(INIT1_STEP + 1);
			}
			else
				return ACT_ERR_ABORT;   
			break;

		case INIT1_STEP + 1: 
		 	 // 保护机械手  05-12-31
			if(pp->pclmp != NULL && !RobotRotateErrChk(pp->pclmp->prbt))
				ACT_STEP_NEW(INIT2_STEP);
    		break;

		case INIT2_STEP:
			tmp = GetMeterLenByCh(pp->mh->ROTATE_METERCH);
			if(STEP_10MS_CHK(pp->mh->TM_SCLPBDLY))
			{
				ACT_START();
				pp->last_act_sn = OS2_RTTB;
				if(tmp>0)
				{
					if( GetMeterPosByCh(pp->mh->ROTATE_METERCH)<pp->pa->ADPOSI_SCLPBFAST)
						ACT_STEP_NEW(ROTATEBWDSLW);
					else
						ACT_STEP_NEW(ROTATEBWD);
				}
				else
				{
					if(CheckPI(pp->pi->I_ROT_BWS) == PI_SET)
						ACT_STEP_NEW(ROTATEBWDSLW);
					else if(TestPI(pp->pi->I_ROT_BWD))
						ACT_STEP_NEW(END_STEP);
					else if(TestPI(pp->pi->I_ROT_FWD))
						ACT_STEP_NEW(ROTATEBWD);
					else
						ACT_STEP_NEW(ROTATEBWDSETUP);
				}
			}
			break;
			
		case ROTATEBWDSETUP:
		case ROTATEBWDSETUP+1:
			if(CheckPI(pp->pi->I_ROT_BWS) == PI_SET)
				ACT_STEP_NEW(ROTATEBWDSLW);
			else if(TestPI(pp->pi->I_ROT_BWD))
				ACT_STEP_NEW(END_STEP);
			else if(TestPI(pp->pi->I_ROT_FWD))
				ACT_STEP_NEW(ROTATEBWD);
			
			CHK_SET_MOV(SetRotateBwdSlow);
			break;

		case ROTATEBWD:
			if(pp->mh->TM_ROTATEFAST > 1000)
				pp->mh->TM_ROTATEFAST = 1000;
		case ROTATEBWD+1:
			tmp = GetMeterLenByCh(pp->mh->ROTATE_METERCH);
			if((tmp> 0 && MoveLocationMeterControlProc(pp->pa->ADPOSI_SCLPBFAST, GetMeterPosByCh(pp->mh->ROTATE_METERCH))) ||
			  (tmp<= 0 && MoveLocationNMControlProc(pp->pi->I_ROT_BWS) ) ||  (pp->mh->TM_ROTATEFAST > 0 && STEP_10MS_CHK(pp->mh->TM_ROTATEFAST)))
			{
				ACT_STEP_NEW(ROTATEBWDSLW);
			}
			else if(tmp<= 0 && MoveLocationNMControlProc(pp->pi->I_ROT_BWD))
				ACT_STEP_NEW(END_STEP);

			CHK_SET_MOV(SetRotateBwdFst);
			break;
   
		case ROTATEBWDSLW:
		case ROTATEBWDSLW+1:
			tmp = GetMeterLenByCh(pp->mh->ROTATE_METERCH);
			if((tmp> 0 && MoveLocationMeterControlProc(pp->pa->ADPOSI_SCLPBLOW, GetMeterPosByCh(pp->mh->ROTATE_METERCH))) ||
			  (tmp<= 0 && MoveLocationNMControlProc(pp->pi->I_ROT_BWD) ))
				ACT_STEP_NEW(END_STEP);	
			CHK_SET_MOV(SetRotateBwdSlow);
			break;

		case END_STEP: 
			SET_OFF(SetRotateBwdoffDelay);
			pp->slidests = 2;
			ret = ACT_END;
			break;

		default:
			ret = ACT_END;
			break;
	}
	if(ret == ACT_END || ret == ACT_ERR_ABORT)
	{
    	return ret;
	}
	else
		return RotateBackgrd(pp, pact);
}


ACT_RET ClampOpnRotate(PACT pact, UI32 para)
{
	PROTATE pp = (PROTATE)pact->part;
	PROTATE pp1 = (PROTATE)GetMachineSubPart(ROTATE_ID, 2);
	PMACHINE pm=&m_machine;
	PEJECT  pejt = (PEJECT)GetMachineSubPart(EJECT_ID, 1);
	PEJECT  pejt1 = (PEJECT)GetMachineSubPart(EJECT_ID, 2);
	ACT_RET	ret = ACT_OK;
	switch(pact->step)
	{
        case INIT1_STEP:		 //调度步骤	
		 if(ChkSysCfg(CO43_ROTLAC))
		 {
       		if(pm->mh->FL_ROTLAC == 1)
				ACT_STEP_NEW(INIT2_STEP);
			else if(pm->mh->FL_ROTLAC == 2)
				ACT_STEP_NEW(INIT4_STEP);
			else if(pm->mh->FL_ROTLAC == 3)
				ACT_STEP_NEW(INIT3_STEP);
			else
				ACT_STEP_NEW(END_STEP);  
		 }
		 else
		 	ACT_STEP_NEW(END_STEP);  
		break;		
        case INIT2_STEP:		 //单滑机	
          	ForkAct(pp->act +OS2_RTTB-1, SUSPEND_FORK, pact->ptsk, 0, 0);//滑模A退
		  	ACT_STEP_NEW(INIT5_STEP);
		break;		
        case INIT3_STEP:		 //双滑机
        	if(pm->md->FL_SCLPCTRL == 0)         //左滑模
        	{
			    ForkAct(pp->act +OS_RTTF-1, SUSPEND_FORK, pact->ptsk, 0, 0);//滑模A进
			    pm->lastslidests =2;
        	}
			else if(pm->md->FL_SCLPCTRL == 1)    //右滑模
			{
				ForkAct(pp->act +OS2_RTTB-1, SUSPEND_FORK, pact->ptsk, 0, 0);//滑模A退
				pm->lastslidests =4;
			}
		    else if(pm->md->FL_SCLPCTRL == 2)    //双滑模
		    {			
				if((pp->slidests ==0)||(pp->slidests ==2))
				{
					ForkAct(pp->act +OS_RTTF-1, SUSPEND_FORK, pact->ptsk, 0, 0);//滑模A进
					pm->lastslidests = 2;
				}
			    else if(pp->slidests ==1)
			    {
					ForkAct(pp->act +OS2_RTTB-1, SUSPEND_FORK, pact->ptsk, 0, 0);//滑模A退
					pm->lastslidests = 4;
			    }
		    }		
			ACT_STEP_NEW(INIT5_STEP);
		break;
        case INIT4_STEP:		 //直角机
			if(pm->md->FL_SCLPCTRL == 0)         //左滑模
			{
				if(pp!= NULL)	//滑模B退
			    	ForkAct(pp->act +OS2_RTTB-1, SUSPEND_FORK, pact->ptsk, 0, 0);//滑模1退	
			    pm->lastslidests = 2;  	
			    	
			}		
			else if(pm->md->FL_SCLPCTRL == 1)    //右滑模
			{
				if(pp1!= NULL)	//滑模B退
			    	ForkAct(pp1->act +OS2_RTTB-1, SUSPEND_FORK, pact->ptsk, 0, 0);//滑模B退		
			    pm->lastslidests = 4;	
			}
			else if(pm->md->FL_SCLPCTRL == 2)    //双滑模
			{
				if(CheckPIUsed(pp1->pi->I_ROT_BWD)&&CheckPIUsed(pp1->pi->I_ROT_FWD))
				{
			    	if(pp->slidests ==1)	//滑模B退
			    	{		
						pm->lastslidests = 2;
			    		ForkAct(pp->act +OS2_RTTB-1, SUSPEND_FORK, pact->ptsk, 0, 0);//滑模A退

			    	}
			    	else if(pp1->slidests ==1)  //滑模B退
			    	{	    	
						pm->lastslidests = 4;
			    	    ForkAct(pp1->act +OS2_RTTB-1, SUSPEND_FORK, pact->ptsk, 0, 0);  //滑模B退	
		
			    	}
				}
				else
					ForkAct(pp->act +OS2_RTTB-1, SUSPEND_FORK, pact->ptsk, 0, 0);//滑模A退
			}
			ACT_STEP_NEW(INIT5_STEP);        
		break;			
		case INIT5_STEP:
			if(((pm->lastslidests ==0)||(pm->lastslidests ==2)) && pejt != NULL)
				ForkAct(pejt->act +OS_EJT-1, SUSPEND_FORK, pact->ptsk, 0, 0);//顶针A
			else if(pm->lastslidests ==4 && pejt1 != NULL)
				ForkAct(pejt1->act +OS_EJT-1, SUSPEND_FORK, pact->ptsk, 0, 0);//顶针B
			ACT_STEP_NEW(END_STEP);    	
			break;		
	    case END_STEP:
			ret = ACT_END;
		default:
			ret = ACT_END;
	}
	return ret;
}


ACT_RET RecycleRotate(PACT pact, UI32 para)
{
	PROTATE pp = (PROTATE)pact->part;
	PMACHINE pm=&m_machine;
	PROTATE pp1 = (PROTATE)GetMachineSubPart(ROTATE_ID, 2);
	ACT_RET	ret = ACT_OK;
	switch(pact->step)
	{
		case INIT1_STEP:		 //调度步骤	
		 if(ChkSysCfg(CO43_ROTLAC))
		 {
       		if(pm->mh->FL_ROTLAC == 1)
				ACT_STEP_NEW(INIT2_STEP);
			else if(pm->mh->FL_ROTLAC == 2)
				ACT_STEP_NEW(INIT4_STEP);
			else if(pm->mh->FL_ROTLAC == 3)
				ACT_STEP_NEW(INIT3_STEP);		
			else
				ACT_STEP_NEW(END_STEP);  
		 }
		 else
		 	ACT_STEP_NEW(END_STEP);  
        break;
        case INIT2_STEP:		 //单滑机		
          	ForkAct(pp->act +OS_RTTF-1, SUSPEND_FORK, pact->ptsk, 0, 0);//滑模A进
		  	ACT_STEP_NEW(END_STEP);        
	    break;		
        case INIT3_STEP:		 //双滑机
        	if(pm->md->FL_SCLPCTRL == 0)         //左滑模
        		ForkAct(pp->act +OS2_RTTB-1, SUSPEND_FORK, pact->ptsk, 0, 0);//滑模A退
			else if(pm->md->FL_SCLPCTRL == 1)    //右滑模
				ForkAct(pp->act +OS_RTTF-1, SUSPEND_FORK, pact->ptsk, 0, 0);//滑模A进
		    else if(pm->md->FL_SCLPCTRL == 2)    //双滑模
		    {			
				if(ChkRunFlag(INIF_AUTO1))
					ForkAct(pp->act +OS_RTTF-1, SUSPEND_FORK, pact->ptsk, 0, 0);//滑模A进
		    }		
			ACT_STEP_NEW(END_STEP);        
        break;
        case INIT4_STEP:		 //直角机
        	if(pm->md->FL_SCLPCTRL == 0)         //左滑模
        		ForkAct(pp->act +OS_RTTF-1, SUSPEND_FORK, pact->ptsk, 0, 0);//滑模A进
        	else if(pm->md->FL_SCLPCTRL == 1)    //右滑模
        	{
 				if(pp1!= NULL)	//滑模B退
			    	ForkAct(pp1->act +OS_RTTF-1, SUSPEND_FORK,pact->ptsk, 0, 0);//滑模B进	
        	}
        	else if(pm->md->FL_SCLPCTRL == 2)    //双滑模
        	{
				if(((pp1->slidests ==2)&&(pm->lastslidests ==4))||((pp1->slidests ==0)&&(pp->slidests==0))||(pm->lastslidests ==0))
				{
					pm->lastslidests = 1;
					ForkAct(pp->act +OS_RTTF-1, SUSPEND_FORK, pact->ptsk, 0, 0);//滑模A进
					
				}
				else if((pp->slidests ==2)&&(pm->lastslidests ==2))
				{			
					pm->lastslidests = 3;
					ForkAct(pp1->act +OS_RTTF-1, SUSPEND_FORK,pact->ptsk, 0, 0);//滑模B进	

				}
				else
					ForkAct(pp->act +OS_RTTF-1, SUSPEND_FORK, pact->ptsk, 0, 0);//滑模A进
        	}
			ACT_STEP_NEW(END_STEP);
        break;
	    case END_STEP:
			ret = ACT_END;
		default:
			ret = ACT_END;	
			break;
	}
	return ret;
}


void RotateSetup(PROTATE pp)
{
	if(pp != NULL)
	{
		pp->pclmp = (PCLAMP)GetMachineSubPart(CLAMP_ID, 1);
		
		if(GET_PART_SN(pp->part.id) == 1)
			pp->pother_rtt = (PROTATE)GetMachineSubPart(ROTATE_ID, 2);
		else if(GET_PART_SN(pp->part.id) == 2)
			pp->pother_rtt = (PROTATE)GetMachineSubPart(ROTATE_ID, 1);
		else
			pp->pother_rtt = NULL;

		pp->plck = (PLOCK)GetMachineSubPart(LOCK_ID, 1);
	}
}

BOOL InitRotatePart(PROTATE pp, UI8 sn)
{
	static const SUB_INIT_INFO sub_init[]  = ROTATE_INIT_INFO;
	
	static const ACT_INIT_INFO act_init[] = {{OS_RTTF, OS_RTTF, ACT_NTYPE, RotateFwd, SetRotateFwdoff},
									   {OS2_RTTB, OS2_RTTB, ACT_NTYPE, RotateBwd, SetRotateBwdoff},
									   {OS_CLAMP_ROT,OS_CLAMP_ROT, ACT_CTRL,ClampOpnRotate,NULL},
									   {OS_RCYC_ROT,OS_RCYC_ROT, ACT_CTRL,	RecycleRotate,NULL}};
									     
	if(!INIT_PART(pp, ROTATE_ID, sn, sub_init))
		return FALSE;

	if(!INIT_ACT(pp, act_init))
		return FALSE;

	RotateSetup(pp);

	pp->last_act_sn = 0;
	pp->slidests = 0;
	pp->mh->ROTATE_METERCH = 4;
	return TRUE;
}

UI32 RotateCmd(PROTATE pp, UI32 cmd, UI32 para)
{
	UI32 ret = TRUE;
	PMACHINE pm=&m_machine;
	if(pp == NULL)
		return FALSE;

	switch(GET_CMD(cmd))
	{
		case CMD_MANUAL_INIT:
			pp->last_act_sn = 0;
			pp->slidests = 0;
			pm->lastslidests = 0;
			break;
			
		case CMD_SETUP:
			RotateSetup(pp);
			break;

		default:
			ret = FALSE;
			break;
	}

	return ret;
}
