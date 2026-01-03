/******************************************************************************
  文 件 名   : core.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 中子部件及中子动作
  函数列表   :
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
#include "robot.h"
#include "core.h"
#include "safedoor.h"
#include "admeter.h"
#include "machine.h"
#include "coreprotect.h"

void InitCoreClampRamp(PCORE pp)
{
	UI16 Acc;	
	Acc = ((UI32)GetSys()->pclmp->md->SLOPE_MAX_CLS_SPD*(UI32)pp->pa->DAFLOW_CLSSTARTRAMP_CORE/100);
	Acc = Acc < 2 ? 2: Acc;		
	pp->cls_ramp_start = 100000/Acc;
	
	Acc = ((UI32)GetSys()->pclmp->md->SLOPE_MAX_CLS_SPD*(UI32)pp->pa->DAFLOW_CLSENDRAMP_CORE/100);
	Acc = Acc < 2 ? 2: Acc;	
	pp->cls_ramp_end = 100000/Acc;

	
	Acc = ((UI32)GetSys()->pclmp->md->SLOPE_MAX_OPN_SPD*(UI32)pp->pa->DAFLOW_OPNSTARTRAMP_CORE/100);
	Acc = Acc < 2 ? 2: Acc;		
	pp->opn_ramp_start = 100000/Acc;
	
	Acc = ((UI32)GetSys()->pclmp->md->SLOPE_MAX_OPN_SPD*(UI32)pp->pa->DAFLOW_OPNENDRAMP_CORE/100);
	Acc = Acc < 2 ? 2: Acc;	
	pp->opn_ramp_end = 100000/Acc;
}

void InitCoreReach()
{
    static UI16 core_reach_ok = 0;
	UI16 i = 0;
    PCLAMP pclmp = (PCLAMP)GetPartByID(MK_PART(CLAMP_ID, 1));

    if(pclmp == NULL)
        return;
    
    if(core_reach_ok == 0)
    {
        core_reach_ok = 1; //开机后首次全自动时初始化中子状态
        
        for(i = 0;i < CLAMP_MAX_CORE;i++)
    	{
    		if(pclmp->pcore[i] != NULL)
    		{
    		    if(pclmp->pcore[i]->md->FL_SPECIALCODE == 1 || pclmp->pcore[i]->md->FL_SPECIALCODE == 2)
                    pclmp->pcore[i]->corests = 1;
                else
                    pclmp->pcore[i]->corests = 2;
    		}
    	}
    }
}

BOOL CoreStateErrChk(PCORE pp, BOOL bfwd)
{
	if(pp != NULL && pp->md->FL_CORE_CTRL > 0 && pp->md->FL_CORE_CTRL < 3)
	{
		if((bfwd && CoreFwdErr(pp)) || (!bfwd && CoreBwdErr(pp)))
		{
			SET_PART_ERR(pp, ER0_CORE);
			return TRUE;
		}
	}

	return FALSE;
}

/*-----------------------------------------------------------------------------------+
|      射出过程中子处理函数                                                          |
+-----------------------------------------------------------------------------------*/
void CoreFwdPOSet(PCORE pp, BOOL bset)
{
	if(pp != NULL && pp->md->FL_CORE_CTRL==1 )
	{
		if(bset)
			SetPOBit(pp->po->O_CP_FWD, PC_ON);  	
		else
			SetPOBit(pp->po->O_CP_FWD, PC_OFF);  	
	}
}

void CoreFwdOff(PCORE pp)
{
	if(pp != NULL)
		SetPOBit(pp->po->O_CP_FWD, PC_OFF);
}

void CoreFwdComplete(PCORE pp)
{
	if(pp->pclmp)
		RobotCoreFwdEnd(pp->pclmp->prbt,GET_PART_SN(((PPART)pp)->id));

	pp->corests=1;
	if((pp->md->FL_SPECIALCODE==1)&&(ChkOperMode(OM_AUTO)))
	{
		pp->corestsf=1;
	}
}

ACT_RET CoreFwdPre(PCORE pp, UI32 bspecial)
{
	if(pp->md->FL_CORE_CTRL > 0 && pp->md->FL_CORE_CTRL < 3)
	{	
		if(pp->st->CORE_STATUS == 1)  //中子进完成，直接结束 ccz 2021.6.24
			return ACT_END;

		if((pp->md->FL_SPECIALCODE==1)&&(ChkOperMode(OM_AUTO)))
		{	
			if(pp->corestsf==1)
				return ACT_END;
		}
		
		if(!ChkSysCfg(XF6_EJTCR) && CoreFBwdEjectStateErrCheck(pp,TRUE)) //中子进动作前检测托退终
			return ACT_ERR_ABORT;

		if(CoreFwdValidPositionErrCheck(pp)) //中子动作前检查是否在位置有效区内
			return ACT_ERR_ABORT;

		if(ChkSysCfg(XF6_CRSEQ) && CoreInSequenceErrCheck(pp)) //中子进动作前顺序检查
			return ACT_ERR_ABORT;
	}          
	else
		return ACT_END;

	return ACT_OK;
}

void SetCoreComVlvOn(PCORE pp)
{
	UI16 FL_OPNEJTCORE = 0,FL_OPNEJTCORE2 = 0;

	FL_OPNEJTCORE = GetSysMold()->FL_OPNEJTCORE;
	FL_OPNEJTCORE2 = GetSysMold()->FL_CLOSJTCORE;
	if(ChkSysCfg(CO1_SOEC)&& ((FL_OPNEJTCORE>=2 && FL_OPNEJTCORE <=7)||(FL_OPNEJTCORE2>=1 && FL_OPNEJTCORE2<=6)))
 	{
		SetPOBit(GetSysPO(O_CP_PUMP), PC_ON);	
		SetPOBit(GetSysPO(O_SOEC_SLM), PC_ON);
		SetPOBit(GetSysPO(O_OPN_EJE_COR), PC_ON);	
	}
	
	if(ChkSysCfg(XF1_HTMHYD))
	{
		SetPOBit(GetSysPO(O_HYD_VLV2), PC_ON);	
		SetPOBit(GetSysPO(O_HYD_COMB), PC_OFF);
	}  
	if(ChkSysCfg(XF1_NMV))
	{
		SetPOBit(GetSysPO(O_HYD_VLV2), PC_ON);
	}
}

void SetCoreComVlvOff(PCORE pp)
{
	SetPOBitDelay(GetSysPO(O_CP_PUMP),PC_OFF, pp->mh->TM_CPPUMP_DLY);
	SetPOBit(GetSysPO(O_HYD_VLV2),PC_OFF);	
	if(ChkSysCfg(CO1_SOEC))
	{
		if(!ChkActRun(pp->pclmp, ACT_SN_BIT(OS_OPNM) | ACT_SN_BIT(OS_EOPN)))
			SetPOBit(GetSysPO(O_OPN_EJE_COR), PC_OFF);
		SetPOBitDelay(GetSysPO(O_SOEC_SLM),PC_OFF, GetSysMold()->TMDLY_LINKVLVOFF);
 	}	
}

void SetCoreFwd(PCORE pp, PACT pact)
{
	UI16 hydelay;
	if(pp->mh->COREF_HYDELAY > 9999)
		pp->mh->COREF_HYDELAY = 0;
	if(pp->mh->TMDLY_VLV_ONCOREF > 500)
		pp->mh->TMDLY_VLV_ONCOREF = 0;
	hydelay = pp->mh->COREF_HYDELAY;
	SetCoreComVlvOn(pp);

	if(pp->pclmp != NULL)
		RobotCoreFwd(pp->pclmp->prbt,GET_PART_SN(((PPART)pp)->id));	

  	SetPOBitDelay(pp->po->O_CP_FWD, PC_ON, pp->mh->TMDLY_VLV_ONCOREF);	

	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_COREIN, pp->pa->DAFLOW_COREIN, 0, 0, pp->mh->DAPRES_CORERAMP, pp->mh->DAFLOW_CORERAMP, HYDRDELY, 
						 HYDR_PORT(pp->hy->HYDR_PORT_CORE), pp->hy->HYDR_PUMPS_CORE, pp->hy->HYDR_PUMPS_CORE, pp->hy->SERVO_KP_CORE, pp->hy->SERVO_KI_CORE, 
						 pp->hy->SERVO_KD_CORE, TRUE, HYDR_PRI(pp->hy->HYDR_PORT_CORE),pp->pa->PER_DAFLOW_CORE);
}

void SetCoreFwdOff(PCORE pp, PACT pact)
{
	if(pp != NULL)
	{
		if(pp->mh->TMDLY_VLV_OFFCOREF > 500)
			pp->mh->TMDLY_VLV_OFFCOREF = 0;
		SetCoreComVlvOff(pp);
		SetPOBitDelay(pp->po->O_CP_FWD, PC_OFF, pp->mh->TMDLY_VLV_OFFCOREF);	
		HYDR_CLS(pp->mh->DAPRES_COREOFFRAMP, pp->mh->DAFLOW_COREOFFRAMP);
	}
}

void SetCoreFwdOffDelay(PCORE pp, PACT pact)
{
	if(pp != NULL)
	{
		if(pp->mh->TMDLY_VLV_OFFCOREF > 500)
			pp->mh->TMDLY_VLV_OFFCOREF = 0;
		SetCoreComVlvOff(pp);
		SetPOBitDelay(pp->po->O_CP_FWD, PC_OFF, pp->mh->TMDLY_VLV_OFFCOREF);	
		HYDR_CLS(pp->mh->DAPRES_COREOFFRAMP, pp->mh->DAFLOW_COREOFFRAMP);
	}
}

ACT_RET CoreFwdBackgrd(PCORE pp, PACT pact)        //clampopn background work 
{
	//if(ChkSysCfg(CO11_SAFECORE))                //2014-08-04  中子检测安全门
	//{
    	if((FSafeDoorIsCloseErrChk(pp->pclmp->psfdr)))
			return ACT_ERR_ABORT;
	//}   

	if(pp->md->CORE_LIMIT_CHECK == 1 && CoreLimitSwitchErrCheck(pp)) //中子动作过程中检查中子行程开关
		return ACT_ERR_ABORT;
	
	if (pact->step >= COREFWD && RobotCoreFwdChk(pp->pclmp->prbt,GET_PART_SN(((PPART)pp)->id)))
	{
		return ACT_ERR_ABORT;
	}

	
	
	return ACT_OK;
}

ACT_RET CoreBwdBackgrd(PCORE pp, PACT pact)        //clampopn background work 
{
	if(ChkSysCfg(CO11_SAFECORE))                //2014-08-04  中子检测安全门
	{
    	if((FSafeDoorIsCloseErrChk(pp->pclmp->psfdr)))
			return ACT_ERR_ABORT;
	}   

	if(pp->md->CORE_LIMIT_CHECK == 1 && CoreLimitSwitchErrCheck(pp)) //中子动作过程中检查中子行程开关
		return ACT_ERR_ABORT;
	
	if (pact->step >= COREBWD && RobotCoreBwdChk(pp->pclmp->prbt,GET_PART_SN(((PPART)pp)->id)))
	{
		return ACT_ERR_ABORT;
	}
	
	return ACT_OK;
}

ACT_RET CoreAdjFwdBackgrd(PCORE pp, PACT pact)        //clampopn background work 
{
	if(pp->md->CORE_LIMIT_CHECK == 1 && CoreLimitSwitchErrCheck(pp)) //中子动作过程中检查中子行程开关
		return ACT_ERR_ABORT;
	
	if (pact->step >= COREFWD && RobotCoreFwdChk(pp->pclmp->prbt,GET_PART_SN(((PPART)pp)->id)))
	{
		return ACT_ERR_ABORT;
	}

	return ACT_OK;
}

ACT_RET CoreAdjBwdBackgrd(PCORE pp, PACT pact)        //clampopn background work 
{
	if(pp->md->CORE_LIMIT_CHECK == 1 && CoreLimitSwitchErrCheck(pp)) //中子动作过程中检查中子行程开关
		return ACT_ERR_ABORT;
	
	if (pact->step >= COREBWD && RobotCoreBwdChk(pp->pclmp->prbt,GET_PART_SN(((PPART)pp)->id)))
	{
		return ACT_ERR_ABORT;
	}
	
	return ACT_OK;
}


ACT_RET CoreFwd(PACT pact, UI32 para)
{
	PCORE pp = (PCORE)(pact->part);
	ACT_RET ret = ACT_OK;
	
	if(pp == NULL)
		return ACT_ERR_ABORT;

	if(CHK_PART_ERR(pp->pclmp, ER0_CLSP))
		return ACT_ERR_ABORT;
	
	switch (pact->step)
	{
		case INIT1_STEP:
			if((ret = CoreFwdPre(pp, para)) == ACT_OK)
			{
				ACT_STEP_NEW(INIT1_STEP + 1);
			}
			break;
			
		case INIT1_STEP + 1:
			if(pp->pclmp == NULL || !RobotCoreFwdErrChk(pp->pclmp->prbt,GET_PART_SN(((PPART)pp)->id)))
			{
				ACT_STEP_NEW(INIT2_STEP);
			}
			break;
			
		case INIT2_STEP:
			if(pp->mh->TM_COREINCTDLY > 9999)
				pp->mh->TM_COREINCTDLY = 9999;
			if(STEP_10MS_CHK(pp->mh->TM_COREINCTDLY))
			{
				pp->corests=0;
				ACT_STEP_NEW(COREFWD);
			}
			break;
				
		case COREFWD:
			ResetPICnt(&pp->sc_chk_cnt, pp->pi->I_SC_CHK, 1);
			ActSetTM(pact->ptsk, pp->pa->TM_COREIN);
		case COREFWD + 1:
			/*忽略特殊中子*/
			if((pp->md->FL_CORE_TRAVEL ==0 && ((pp->md->FL_CORE_CTRL ==2 && MoveLocationCntControlProc(&pp->sc_chk_cnt, pp->pa->CN_SCREWIN)) ||			/*走行程计数, 忽略SCREWTBL值*/
			   (pp->md->FL_CORE_CTRL !=2 && MoveLocationNMControlProc(pp->pi->I_CP_FWD)))) ||																/*走行程开关*/
			   (pp->md->FL_CORE_TRAVEL !=0 && MoveLocationTMControlProc(pp->pa->TM_COREIN*10)))									/*走时间*/
			{
				CoreStatusJudge(pp);//此处进行中子状态判断，以便进入后台函数判断行程开关是否异常				
				ACT_STEP_NEW(COREFWDHOLD);
			}

			CHK_SET_MOV(SetCoreFwd);
			break;

        case COREFWDHOLD:
			if(pp->md->TM_HOLD_CORE_FWD > 9999)
				pp->md->TM_HOLD_CORE_FWD = 0;
		case COREFWDHOLD + 1:
			if(STEP_10MS_CHK(pp->md->TM_HOLD_CORE_FWD))
			    ACT_STEP_NEW(END_STEP);

            CHK_SET_MOV(SetCoreFwd);
			break;

		case END_STEP:
			SET_OFF(SetCoreFwdOffDelay);
			if(pp->mh->TM_COREINENDDLY <= 999 && pp->mh->TM_COREINENDDLY > 0)
				ACT_STEP_NEW(END_STEP + 1);
			else
			{
				CoreFwdComplete(pp);
				ret = ACT_END;
			}
			break;
		case END_STEP + 1:
			if(STEP_10MS_CHK(pp->mh->TM_COREINENDDLY))
			{
				CoreFwdComplete(pp);
				ret = ACT_END;
			}
			break;
			
		default:
			break;
	}
	
    if(ret == ACT_END || ret == ACT_ERR_ABORT)	
    {
		return ret;
    }
	else
    	return CoreFwdBackgrd(pp,pact);
}

void CoreBwdComplete(PCORE pp)
{
	if(pp->pclmp)
		RobotCoreBwdEnd(pp->pclmp->prbt,GET_PART_SN(((PPART)pp)->id));

	pp->corests=2;

	if((pp->md->FL_SPECIALCODE==1)&&(ChkOperMode(OM_AUTO)))
	{	
		pp->corestsb=1;		
	}
}

ACT_RET CoreBwdPre(PCORE pp, UI32 bspecial)
{	
	if(pp->md->FL_CORE_CTRL > 0 && pp->md->FL_CORE_CTRL < 3)
	{
		if(pp->st->CORE_STATUS == 2)  //中子退完成，直接结束 ccz 2021.6.24
			return ACT_END;

		if((pp->md->FL_SPECIALCODE==1)&&(ChkOperMode(OM_AUTO)))
		{
			if(pp->corestsb==1)
				return 	ACT_END;
		}

        if(!ChkSysCfg(XF6_EJTCR) && CoreFBwdEjectStateErrCheck(pp,FALSE)) //中子退动作前检测托退终
			return ACT_ERR_ABORT;
        
		if(CoreBwdValidPositionErrCheck(pp)) //中子动作前检查是否在位置有效区内
			return ACT_ERR_ABORT;

		if(ChkSysCfg(XF6_CRSEQ) && CoreOutSequenceErrCheck(pp)) //中子退动作前顺序检查
			return ACT_ERR_ABORT;
	}
	else
		return ACT_END;

	return ACT_OK;
}

void SetCoreBwd(PCORE pp, PACT pact)
{
	UI16 hydelay;
	if(pp->mh->COREB_HYDELAY > 9999)
		pp->mh->COREB_HYDELAY = 0;
	if(pp->mh->TMDLY_VLV_ONCOREB > 500)
		pp->mh->TMDLY_VLV_ONCOREB =0;
	
	hydelay = pp->mh->COREB_HYDELAY;
	SetCoreComVlvOn(pp);

	if(pp->pclmp != NULL)
		RobotCoreBwd(pp->pclmp->prbt,GET_PART_SN(((PPART)pp)->id));	

  	SetPOBitDelay(pp->po->O_CP_BWD, PC_ON, pp->mh->TMDLY_VLV_ONCOREB);	

	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_COREOUT, pp->pa->DAFLOW_COREOUT, 0, 0, pp->mh->DAPRES_CORERAMP, pp->mh->DAFLOW_CORERAMP, HYDRDELY, 
						 HYDR_PORT(pp->hy->HYDR_PORT_CORE),   pp->hy->HYDR_PUMPS_CORE, pp->hy->HYDR_PUMPS_CORE, pp->hy->SERVO_KP_CORE, pp->hy->SERVO_KI_CORE, 
						 pp->hy->SERVO_KD_CORE, TRUE, HYDR_PRI(pp->hy->HYDR_PORT_CORE),pp->pa->PER_DAFLOW_CORE);
}

void SetCoreBwdOff(PCORE pp, PACT pact)
{
	if(pp != NULL)
	{
		if(pp->mh->TMDLY_VLV_OFFCOREB > 500)
			pp->mh->TMDLY_VLV_OFFCOREB = 0;
		SetCoreComVlvOff(pp);
		SetPOBitDelay(pp->po->O_CP_BWD, PC_OFF, pp->mh->TMDLY_VLV_OFFCOREB);	
		HYDR_CLS(pp->mh->DAPRES_COREOFFRAMP, pp->mh->DAFLOW_COREOFFRAMP);
	}
}

void SetCoreBwdOffDelay(PCORE pp, PACT pact)
{
	if(pp != NULL)
	{
		if(pp->mh->TMDLY_VLV_OFFCOREB > 500)
			pp->mh->TMDLY_VLV_OFFCOREB = 0;
		SetCoreComVlvOff(pp);
		SetPOBitDelay(pp->po->O_CP_BWD, PC_OFF, pp->mh->TMDLY_VLV_OFFCOREB);	
		HYDR_CLS(pp->mh->DAPRES_COREOFFRAMP, pp->mh->DAFLOW_COREOFFRAMP);
	}
}
ACT_RET CoreBwd(PACT pact, UI32 para)
{
	PCORE pp = (PCORE)(pact->part);
	ACT_RET ret = ACT_OK;
	WORD FlCoreOutTravel;
	
	if(pp == NULL)
		return ACT_ERR_ABORT;
	switch (pact->step)
	{
		case INIT1_STEP:
			if((ret = CoreBwdPre(pp, para)) == ACT_OK)
			{
				ACT_STEP_NEW(INIT1_STEP + 1);
			}
			break;
			
		case INIT1_STEP + 1:
			if(pp->pclmp == NULL || !RobotCoreBwdErrChk(pp->pclmp->prbt,GET_PART_SN((((PPART)pp)->id))))
			{
				ACT_STEP_NEW(INIT2_STEP);
			}
			break;
			
		case INIT2_STEP:
			if(pp->mh->TM_COREOUTCTDLY > 9999)
				pp->mh->TM_COREOUTCTDLY = 9999;
			if(STEP_10MS_CHK(pp->mh->TM_COREOUTCTDLY))
			{
				pp->corests=0;
				ACT_STEP_NEW(COREBWD);
			}
			break;
				
		case COREBWD:
			ResetPICnt(&pp->sc_chk_cnt, pp->pi->I_SC_CHK, 1);
			ActSetTM(pact->ptsk, pp->pa->TM_COREOUT);
		case COREBWD + 1:
			FlCoreOutTravel = pp->md->FL_CORE_OUTTRAVEL;
			if((FlCoreOutTravel ==0 && ((pp->md->FL_CORE_CTRL ==2 && MoveLocationCntControlProc(&pp->sc_chk_cnt, pp->pa->CN_SCREWOUT)) ||			/*走行程计数, 忽略SCREWTBL值*/
			   (pp->md->FL_CORE_CTRL !=2 && MoveLocationNMControlProc(pp->pi->I_CP_BWD)))) ||																/*走行程开关*/
			   (FlCoreOutTravel !=0 && MoveLocationTMControlProc(pp->pa->TM_COREOUT*10)))									/*走时间*/
			{
				CoreStatusJudge(pp);//此处进行中子状态判断，以便进入后台函数判断行程开关是否异常				
				ACT_STEP_NEW(COREBWDHOLD);
			}

			CHK_SET_MOV(SetCoreBwd);
			break;

        case COREBWDHOLD:
			if(pp->md->TM_HOLD_CORE_BWD > 9999)
				pp->md->TM_HOLD_CORE_BWD = 0;
		case COREBWDHOLD + 1:
			if(STEP_10MS_CHK(pp->md->TM_HOLD_CORE_BWD))
			    ACT_STEP_NEW(END_STEP);
            
            CHK_SET_MOV(SetCoreBwd);
			break;
            
		case END_STEP:
			SET_OFF(SetCoreBwdOffDelay);
			if(pp->mh->TM_COREOUTENDDLY <= 999 && pp->mh->TM_COREOUTENDDLY > 0)
				ACT_STEP_NEW(END_STEP + 1);
			else
			{
				CoreBwdComplete(pp);
				ret = ACT_END;
			}
			break;
		case END_STEP + 1:
			if(STEP_10MS_CHK(pp->mh->TM_COREOUTENDDLY))
			{
				CoreBwdComplete(pp);
				ret = ACT_END;
			}
			break;
			
		default:
			break;
	}
	
    if(ret == ACT_END || ret == ACT_ERR_ABORT)	
    {
	    return ret;
    }
	else
    	return CoreBwdBackgrd(pp,pact);
}

void SetCoreAdjFwd(PCORE pp, PACT pact)
{
	UI16 hydelay;
	if(pp->mh->COREF_HYDELAY > 9999)
		pp->mh->COREF_HYDELAY = 0;
	hydelay = pp->mh->COREF_HYDELAY;
	SetCoreComVlvOn(pp);

	if(pp->pclmp != NULL)
		RobotCoreFwd(pp->pclmp->prbt,GET_PART_SN(((PPART)pp)->id));	

  	SetPOBit(pp->po->O_CP_FWD, PC_ON);	

	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->mh->DAPRES_CORESETUP, pp->mh->DAFLOW_CORESETUP, 0, 0, pp->mh->DAPRES_CORERAMP, pp->mh->DAFLOW_CORERAMP, HYDRDELY, 
						 HYDR_PORT(pp->hy->HYDR_PORT_CORE), pp->hy->HYDR_PUMPS_CORE, pp->hy->HYDR_PUMPS_CORE, pp->hy->SERVO_KP_CORE, pp->hy->SERVO_KI_CORE, 
						 pp->hy->SERVO_KD_CORE, TRUE, HYDR_PRI(pp->hy->HYDR_PORT_CORE),pp->pa->PER_DAFLOW_CORE);
}

ACT_RET AdjCoreFwd(PACT pact, UI32 para)
{
	PCORE pp = (PCORE)(pact->part);
	ACT_RET ret = ACT_OK;
	
	if(!ChkOperMode(OM_ADJU))
		return ACT_ERR_ABORT;

	if(pp == NULL)
		return ACT_ERR_ABORT;

	switch (pact->step)
	{
    	case INIT1_STEP:
			ACT_STEP_NEW(COREFWD);
			break;
			
		case COREFWD:
            ResetPICnt(&pp->sc_chk_cnt, pp->pi->I_SC_CHK, 1);
		case COREFWD + 1:
			/*打上CODE码后，一直等到按键停止*/
            if(!ChkSysCfg(XF5_INJTM))
            {
                if((pp->md->FL_CORE_TRAVEL ==0 && ((pp->md->FL_CORE_CTRL ==2 && MoveLocationCntControlProc(&pp->sc_chk_cnt, pp->pa->CN_SCREWIN)) ||			/*走行程计数, 忽略SCREWTBL值*/
    			   (pp->md->FL_CORE_CTRL !=2 && MoveLocationNMControlProc(pp->pi->I_CP_FWD)))) ||																/*走行程开关*/
    			   (pp->md->FL_CORE_TRAVEL !=0 && MoveLocationTMControlProc(pp->pa->TM_COREIN*10)))									/*走时间*/
    			{
    				CoreStatusJudge(pp);//此处进行中子状态判断，以便进入后台函数判断行程开关是否异常				
    				ACT_STEP_NEW(END_STEP);
    			}
            }
            
			CHK_SET_MOV(SetCoreAdjFwd);
			break;

        case END_STEP:
            SET_OFF(SetCoreFwdOffDelay);
			CoreFwdComplete(pp);
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
    	return CoreAdjFwdBackgrd(pp,pact);
}

void SetCoreAdjBwd(PCORE pp, PACT pact)
{
	UI16 hydelay;
	if(pp->mh->COREB_HYDELAY > 9999)
		pp->mh->COREB_HYDELAY = 0;
	hydelay = pp->mh->COREB_HYDELAY;
	SetCoreComVlvOn(pp);

	if(pp->pclmp != NULL)
		RobotCoreBwd(pp->pclmp->prbt,GET_PART_SN(((PPART)pp)->id));	

  	SetPOBit(pp->po->O_CP_BWD, PC_ON);	

	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->mh->DAPRES_CORESETUP, pp->mh->DAFLOW_CORESETUP, 0, 0, pp->mh->DAPRES_CORERAMP, pp->mh->DAFLOW_CORERAMP, HYDRDELY, 
						 HYDR_PORT(pp->hy->HYDR_PORT_CORE),   pp->hy->HYDR_PUMPS_CORE, pp->hy->HYDR_PUMPS_CORE, pp->hy->SERVO_KP_CORE, pp->hy->SERVO_KI_CORE, 
						 pp->hy->SERVO_KD_CORE, TRUE, HYDR_PRI(pp->hy->HYDR_PORT_CORE),pp->pa->PER_DAFLOW_CORE);
}

ACT_RET AdjCoreBwd(PACT pact, UI32 para)
{
	PCORE pp = (PCORE)(pact->part);
	ACT_RET ret = ACT_OK;
	
	if(!ChkOperMode(OM_ADJU))
		return ACT_ERR_ABORT;

	if(pp == NULL)
		return ACT_ERR_ABORT;

	switch (pact->step)
	{
    	case INIT1_STEP:
			ACT_STEP_NEW(COREBWD);
			break;
			
		case COREBWD:
            ResetPICnt(&pp->sc_chk_cnt, pp->pi->I_SC_CHK, 1);
		case COREBWD + 1:
			/*打上CODE码后，一直等到按键停止*/
            if(!ChkSysCfg(XF5_INJTM))
            {
    			if((pp->md->FL_CORE_OUTTRAVEL ==0 && ((pp->md->FL_CORE_CTRL ==2 && MoveLocationCntControlProc(&pp->sc_chk_cnt, pp->pa->CN_SCREWOUT)) ||			/*走行程计数, 忽略SCREWTBL值*/
    			   (pp->md->FL_CORE_CTRL !=2 && MoveLocationNMControlProc(pp->pi->I_CP_BWD)))) ||																/*走行程开关*/
    			   (pp->md->FL_CORE_OUTTRAVEL !=0 && MoveLocationTMControlProc(pp->pa->TM_COREOUT*10)))									/*走时间*/
    			{
    				CoreStatusJudge(pp);//此处进行中子状态判断，以便进入后台函数判断行程开关是否异常				
    				ACT_STEP_NEW(END_STEP);
    			}
            }

			CHK_SET_MOV(SetCoreAdjBwd);
			break;

        case END_STEP:
            SET_OFF(SetCoreBwdOffDelay);
			CoreBwdComplete(pp);
			ret = ACT_END;
			break;
		default: 
			break;	   
	}
	if(ret == ACT_END || ret == ACT_ERR_ABORT)	
    {
		return ret;
    }
	else
    	return CoreAdjBwdBackgrd(pp,pact);
}

ACT_RET CoreRelease(PACT pact, UI32 para)   //由中子A泄压调度所有中子
{
	PCORE pp = (PCORE)(pact->part);
    PCLAMP pclmp = (PCLAMP)GetPartByID(MK_PART(CLAMP_ID, 1));
	ACT_RET ret = ACT_OK;
    static UI16 core_rls_sn = 0;
	
	if(!ChkOperMode(OM_MANUAL))
		return ACT_ERR_ABORT;

	if(pp == NULL)
		return ACT_ERR_ABORT;

	switch (pact->step)
	{
    	case INIT1_STEP:
            core_rls_sn = 0;
			ACT_STEP_NEW(INIT1_STEP + 1);
			break;
        case INIT1_STEP + 1:
            if(pclmp->pcore[core_rls_sn] != NULL)
			    ACT_STEP_NEW(COREFWDRLS);
            else
                ACT_STEP_NEW(STAGE_3_STEP); //指针为空直接判断下一部件
			break;
			
		case COREFWDRLS:    //中子进泄压
		    if(pclmp->pcore[core_rls_sn]->md->FL_CORE_CTRL > 0 && pclmp->pcore[core_rls_sn]->md->FL_CORE_CTRL < 3)
            {      
                SetPOBit(pclmp->pcore[core_rls_sn]->po->O_CP_FWD,PC_ON);
                pclmp->pcore[core_rls_sn]->coreReleaseDlyTm = Get10MsTick();
                ACT_STEP_NEW(COREFWDRLS + 1);
            }
            else
            {
                ACT_STEP_NEW(STAGE_3_STEP);
            }
            break;
		case COREFWDRLS + 1:
            if(DIFF_10MS(pclmp->pcore[core_rls_sn]->coreReleaseDlyTm) >= 50)
            {
                SetPOBit(pclmp->pcore[core_rls_sn]->po->O_CP_FWD,PC_OFFD);
                ACT_STEP_NEW(COREBWDRLS);
            }
			break;

        case COREBWDRLS:    //中子退泄压
            SetPOBit(pclmp->pcore[core_rls_sn]->po->O_CP_BWD,PC_ON);
            pclmp->pcore[core_rls_sn]->coreReleaseDlyTm = Get10MsTick();
            ACT_STEP_NEW(COREBWDRLS + 1);
            break;
		case COREBWDRLS + 1:
            if(DIFF_10MS(pclmp->pcore[core_rls_sn]->coreReleaseDlyTm) >= 50)
            {
                SetPOBit(pclmp->pcore[core_rls_sn]->po->O_CP_BWD,PC_OFFD);
                pclmp->pcore[core_rls_sn]->coreReleaseDlyTm = Get10MsTick();
                ACT_STEP_NEW(STAGE_3_STEP);
            }
			break;

        case STAGE_3_STEP:
            core_rls_sn++;
            if(core_rls_sn < CLAMP_MAX_CORE)
                ACT_STEP_NEW(INIT1_STEP + 1); 
            else
                ACT_STEP_NEW(END_STEP); 
			break;

        case END_STEP:
            ret = ACT_END;
            break;
            
		default: 
			break;	   
	}

    if(!ActTskIsEmptyExpAct(pact->id)) //有其它任务时立即结束
        return ACT_ERR_ABORT;
    else
	    return ret;
}

void CoreReleaseOff(PCORE pp, PACT pact)
{
    UI16 i;
    PCORE pcorex;

    for(i = 0;i < CLAMP_MAX_CORE;i++)
    {
        pcorex = (PCORE)GetPartByID(MK_PART(CORE_ID, i+1));
    	if(pcorex != NULL)
    	{
    	    SetPOBit(pcorex->po->O_CP_FWD,PC_OFFD);
            SetPOBit(pcorex->po->O_CP_BWD,PC_OFFD);
    	}
    }
}

void CoreRleaseLoop()
{
    PCORE pcore1 = (PCORE)GetPartByID(MK_PART(CORE_ID, 1));

    if(pcore1 != NULL)
    {
        if(CheckPI(pcore1->pi->I_CP_RLS) != PI_NOUSED)
        {
            switch(pcore1->CoreRls_Step)
            {
                case 0:
                    if(CheckPI(pcore1->pi->I_CP_RLS) == PI_RESET)
                    {
                        pcore1->CoreRls_Step++;
                    }
                    break;

                case 1:
                    if(CheckPI(pcore1->pi->I_CP_RLS) == PI_SET)
                    {
                        if(ChkOperMode(OM_MANUAL))
                        {
                            ForkAct(pcore1->act + OS_CPRLS - 1, FIRST_FORK, NULL, 0, 50);
                            pcore1->CoreRls_Step++;
                        }
                        else
                            pcore1->CoreRls_Step = 0;
                    }
                    break;

                case 2:
                    if(!ChkActRun(pcore1, ACT_SN_BIT(OS_CPRLS)))
                    {
                        pcore1->CoreRls_Step = 0;
                    }
                    break;

                default:
                    pcore1->CoreRls_Step = 0;
                    break;
            }
        }
        else
            pcore1->CoreRls_Step = 0;
    }
}

void CoreSetup(PCORE pp)
{
	if(pp != NULL)
		pp->pclmp = (PCLAMP)GetMachineSubPart(CLAMP_ID, 1);
}

BOOL InitCorePart(PCORE pp, UI8 sn)
{
	static const SUB_INIT_INFO sub_init[]  = CORE_INIT_INFO;

	static const ACT_INIT_INFO act_init[] = {{OS_CPRF, OS_CPRF, ACT_NTYPE, CoreFwd, SetCoreFwdOff},
									   {OS_CPRB, OS_CPRB, ACT_NTYPE, CoreBwd, SetCoreBwdOff},
									   {OS_CPRAF, OS_CPRF, ACT_NTYPE, AdjCoreFwd, SetCoreFwdOff},
									   {OS_CPRAB, OS_CPRB, ACT_NTYPE, AdjCoreBwd, SetCoreBwdOff},
									   {OS_CPRLS, OS_CPRLS, ACT_NTYPE, CoreRelease, CoreReleaseOff}};
										  
	if(!INIT_PART(pp, CORE_ID, sn, sub_init))
		return FALSE;

	if(!INIT_ACT(pp, act_init))
		return FALSE;

	CoreSetup(pp);

	pp->corestsf=0;
	pp->corestsb=0;
	pp->CoreChk = 0x0101;      //中子进退检查      2015-4-1
	pp->sn = sn;
	
	if(sn == 1)
	{
		pp->pi->I_CP_FWD = 12;
		pp->pi->I_CP_BWD = 13;		
	}
	else if(sn == 2)
	{
		pp->pi->I_CP_FWD = 23;
		pp->pi->I_CP_BWD = 24;		
	}
	return TRUE;
}

UI32 CoreCmd(PCORE pp, UI32 cmd, UI32 para)
{
	UI32 ret = TRUE;
	UI16 offset,len;
	
	if(pp == NULL)
		return FALSE;

	switch(GET_CMD(cmd))
	{
		case CMD_MANUAL_INIT:
			break;
		case CMD_SETUP:
			CoreSetup(pp);
			break;
		case CMD_DATA_RX:
			offset = (para >> 16 & 0xffff);
            len = (para & 0xffff);
			
			if((GET_CMD_PARA(cmd) & 0xff) == PA_ID)
			{
				if(len > 0 && (GET_IND(PCORE_ACTPARA, DAFLOW_OPNENDRAMP_CORE) >= offset && GET_IND(PCORE_ACTPARA, DAFLOW_CLSSTARTRAMP_CORE) <= (offset + len - 1)))
				{
					//修改斜率相关参数时，对应缓存进行计算
					InitCoreClampRamp(pp);
				}
			}
			break;
		default:
			ret = FALSE;
			break;
	}

	return ret;
}
