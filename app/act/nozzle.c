/******************************************************************************
  文 件 名   : nozzle.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 座台部件及动作函数
  函数列表   :
              CoolBeforeCharge
              CoolNormal
              InitNozzlePart
              NozzleBwd
              NozzleBwdAdj
              NozzleBwdBackgrd
              NozzleBwdOff
              NozzleCalBwdMode
              NozzleCalFwdMode
              NozzleCmd
              NozzleFwd
              NozzleFwdAdj
              NozzleFwdEndErrChk
              NozzleFwdOff
              NozzleIsReach
              NozzleSetup
              NozzleType
              SetNozzleBwd
              SetNozzleBwdAdj
              SetNozzleFwd
              SetNozzleFwdAdj
              SetNozzleFwdPO
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
#include "nozzle.h"
#include "clamp.h"
#include "inject.h"
#include "safedoor.h"

UI32		coolstart;	
/*Called by Inject storage action*/
void SetNozzleFwdPO(PNOZZLE pp, BOOL bon)
{
	if(pp != NULL)
	{
		if(!bon)
			SetPOBit(pp->po->O_NOZ_FWD, PC_OFFD);
		else
			SetPOBit(pp->po->O_NOZ_FWD, PC_ON);
	}
}

NOZZLE_TYPE NozzleType(PNOZZLE pp)
{
	return (NOZZLE_TYPE)pp->md->FL_NOZRET;
}

BOOL NozzleFwdEndErrChk(PNOZZLE pp)
{
	if(pp != NULL)
	{
		if(!TestPI(pp->pi->I_NOZ_FWD))
		{
			SET_PART_ERR(pp, ER0_NZLIMT);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL NozzleIsReach(PNOZZLE pp)
{
	if ((ChkSysCfg(COA_CHL4N) && GetMeterLenByCh(pp->mh->NZL_METERCH) > 0 && !ChkSysCfg(CO0_SERVOP)) ||//增加判断非伺服机时，才有效
		(ChkSysCfg(COA_CHL4N) && (GetMeterLenByCh(pp->mh->NZL_METERCH) > 0) && ChkSysCfg(CO0_SERVOP) && ChkSysCfg(CO1_ECAT))) //使用ECAT模式有效
	{
		if(GetMeterPosByCh(pp->mh->NZL_METERCH) >= (int)pp->pa->ADPOSI_NOZADVEND)
			return(TRUE);
		else
			return(FALSE);  
	}	
	else
	{
		return(TestPI(pp->pi->I_NOZ_FWD));
	}
}

/*---------------------------------------------------------------------------+
|           code segment                                                     |
+---------------------------------------------------------------------------*/
void SetNozzleFwd(PNOZZLE pp, PACT pact)
{
	UI16 hydelay;
	if(pp->mh->NZLF_HYDELAY > 9999)
		pp->mh->NZLF_HYDELAY = 0;
	if(pp->mh->TMDLY_VLV_ONNZLF > 500)
		pp->mh->TMDLY_VLV_ONNZLF = 0;
	if(pp->pa->TM_NOZADVACT > 9999)
		pp->pa->TM_NOZADVACT = 0;
	
	hydelay = pp->mh->NZLF_HYDELAY;
	if (pp->mode == NZ_ACT_FAST || pp->mode == NZ_ACT_TM)
		ActSetTM(pact->ptsk, pp->pa->TM_NOZADVACT);

		/*打开座进阀*/
	if(NozzleIsReach(pp) || ChkOperMode(OM_AUTO))
	{
		SetPOBit(pp->po->O_NOZ_FWD,PC_ON);
	}
	else
	{
		SetPOBitDelay(pp->po->O_NOZ_FWD,PC_ON, pp->mh->TMDLY_VLV_ONNZLF);
	}
	
	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_NOZADV,pp->pa->DAFLOW_NOZADV, 0, 0, pp->mh->DAPRES_NOZRAMP,pp->mh->DAFLOW_NOZRAMP,
						HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_NOZZLE),pp->hy->HYDR_PUMPS_NOZZLE,pp->hy->HYDR_PUMPS_NOZZLE,
						pp->hy->SERVO_KP_NOZZLE,pp->hy->SERVO_KI_NOZZLE, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_NOZZLE), pp->pa->PER_DAFLOW_NOZZLE); 
}

void SetNozzleFwdAdj(PNOZZLE pp, PACT pact)
{
	UI16 hydelay;
	if(pp->mh->NZLF_HYDELAY > 9999)
		pp->mh->NZLF_HYDELAY = 0;
	hydelay = pp->mh->NZLF_HYDELAY;
	
    ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->mh->DAPRES_NOZSETUP,pp->mh->DAFLOW_NOZSETUP,0,0,pp->mh->DAPRES_NOZRAMP,pp->mh->DAFLOW_NOZRAMP,
						(pact->ftaskstat & INTF_CEC) ? 0 : HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_NOZZLE),pp->hy->HYDR_PUMPS_NOZZLE,pp->hy->HYDR_PUMPS_NOZZLE,
						pp->hy->SERVO_KP_NOZZLE,pp->hy->SERVO_KI_NOZZLE, 0,   !(pact->ftaskstat & INTF_CEC),HYDR_PRI(pp->hy->HYDR_PORT_NOZZLE), pp->pa->PER_DAFLOW_NOZZLE); 

	SetPOBit(pp->po->O_NOZ_FWD,PC_ON);
}

void NozzleFwdOff(PNOZZLE pp, PACT pact)
{
	if(pp->mh->TMDLY_VLV_OFFNZLF > 500)
		pp->mh->TMDLY_VLV_OFFNZLF = 0;
	HYDR_CLS(pp->mh->DAPRES_OFFNOZRAMP, pp->mh->DAFLOW_OFFNOZRAMP);
	if(!ChkOperMode(OM_AUTO)) 
		SetPOBitDelay(pp->po->O_NOZ_FWD,PC_OFF, pp->mh->TMDLY_VLV_OFFNZLF);
}

void NozzleFwdOffDelay(PNOZZLE pp, PACT pact)
{
	if(pp->mh->TMDLY_VLV_OFFNZLF > 500)
		pp->mh->TMDLY_VLV_OFFNZLF = 0;
	HYDR_CLS(pp->mh->DAPRES_OFFNOZRAMP, pp->mh->DAFLOW_OFFNOZRAMP);

	if(!ChkOperMode(OM_AUTO)) 
		SetPOBitDelay(pp->po->O_NOZ_FWD,PC_OFF, pp->mh->TMDLY_VLV_OFFNZLF);
}

ACT_RET NozzleBackgrd(PNOZZLE pp, PACT pact)       //2014-08-04
{
	if(ChkSysCfg(CO11_SAFENOZZLE))   
   	{
		if (FSafeDoorIsCloseErrChk(pp->pinj->pclmp->psfdr))                 //前安全门保护
			return ACT_ERR_ABORT;
   	}

	if(ChkSysCfg(CO11_NOZPUA))
	{
		if (InjectPurgeGuard(pp->pinj, pact) == ACT_ERR)
			return ACT_ERR_ABORT;
	}

	pp->st->TM_NOZZLEF_RT = ACT_RUN_MS/10;

	return ACT_OK;	
}
/*判断座进模式*/
UI8 NozzleCalFwdMode(PNOZZLE pp)
{
	UI8	mode = NZ_ACT_STD;
	
	if(pp)
	{
		 mode = NZ_ACT_STD;
		if (ChkSysCfg(COA_CHL4N) && GetMeterLenByCh(pp->mh->NZL_METERCH) > 0 && !ChkSysCfg(CO0_SERVOP))  //增加判断非伺服机时，才有效
		{
			mode = NZ_ACT_METER;			 //座台使用电子尺
		}
		else if (ChkSysCfg(COA_CHL4N) && (GetMeterLenByCh(pp->mh->NZL_METERCH) > 0) && ChkSysCfg(CO0_SERVOP) && ChkSysCfg(CO1_ECAT)) //使用ECAT模式有效
		{
			mode = NZ_ACT_METER;			 //座台使用电子尺
		}
		else
		{
			mode = NZ_ACT_STD;				//标准座进
		}
				
		if(ChkSysCfg(XF3_SHFST))
			 mode = NZ_ACT_FAST;				 //声海快速机座进动作

		if(pp->pa->TM_NOZADVACT > 0 && ChkSysCfg(CO3_TNOZ))
		{
			if (ChkOperMode(OM_AUTO))			//座进用时间	
			{
				if(ChkRunFlag(INIF_AUTO1))		
					 mode = NZ_ACT_TM;		//自动时首模座进动作
				else
					 mode = NZ_ACT_NO;		 //非首模，座进结束
			}
			else
				 mode = NZ_ACT_TM;		//手动时首模座进动作, 按时间
		}
	}

	return mode;
}

ACT_RET NozzleFwd(PACT pact, UI32 para)
{
	PMACHINE pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1));

	PNOZZLE pp = (PNOZZLE)pact->part;
	ACT_RET ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;
	
	if(ActIODelayLimit(pp->po->O_NOZ_FWD, TRUE))
	{
	    return ACT_ERR_ABORT;
	}
	switch (pact->step)
	{
		case INIT1_STEP:
			if(ChkSysCfg(CO4_CACE) && pp->pinj != NULL && ClampClsEndErrChk(pp->pinj->pclmp))            //美规标准 座进前需关模终
				return ACT_ERR_ABORT;

			if (pp->mh->TM_DLYNOZADV> NOZZLE_TM_MAX)
				pp->mh->TM_DLYNOZADV = 0;

			pp->mode = NozzleCalFwdMode(pp);
			ACT_STEP_NEW(INIT1_STEP + 1);
			break;
			
		case INIT1_STEP + 1:   
			//有同步储料任务未完成，等待  
			if (!ChkActRun(pp->pinj, ACT_SN_BIT(OS_CHRG) |ACT_SN_BIT(OS_SUCK)) &&  STEP_10MS_CHK(pp->mh->TM_DLYNOZADV))
				ACT_STEP_NEW(INIT1_STEP+2);
			break; 
        case  INIT1_STEP+2:
   		    if(ChkSysCfg(CO11_NOZPUA))		   //2014-08-04  座进前检查射出防护罩
   		    {
		    	if(InjectPurgeGuard(pp->pinj, pact) == ACT_OK)		//射出防护罩
		    	{
					ACT_STEP_NEW(INIT1_STEP + 3);
		   		}
   		    }
			else
				ACT_STEP_NEW(INIT1_STEP + 3);
			break; 

		case INIT1_STEP + 3:
			if(pp->mode == NZ_ACT_NO)
			{
				ACT_STEP_NEW(END_STEP);
			}
			else
			{
				ACT_START();
				ACT_STEP_NEW(NOZZLEFWD);
			}
			break;
			
		case NOZZLEFWD:
		case NOZZLEFWD + 1:
			if(pp->mode == NZ_ACT_TM && MoveLocationTM10msControlProc(pp->pa->TM_NOZADVACT))		 //做时间座进
				ACT_STEP_NEW(NOZZLEFWDSLW);
			else if(pp->mode == NZ_ACT_STD && MoveLocationNMControlProc(pp->pi->I_NOZ_FWD))			//开关式座进动作
				ACT_STEP_NEW(NOZZLEFWDSLW);
			else if(pp->mode == NZ_ACT_FAST && MoveLocationTM10msControlProc(pp->pa->TM_NOZADVACT))	//声海快速机座进动作，无慢速
				ACT_STEP_NEW(END_STEP);
			else if(pp->mode == NZ_ACT_METER && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->NZL_METERCH),pp->pa->ADPOSI_NOZADVEND)) //使用电子尺座进动作
				ACT_STEP_NEW(NOZZLEFWDSLW);

			CHK_SET_MOV(SetNozzleFwd);
			break;
		case NOZZLEFWDSLW:
		case NOZZLEFWDSLW + 1:
			/*只有座台运动过，才需要座台进慢速计时*/
			if(pp->mh->TM_NOZADVSLOW > 500)
				pp->mh->TM_NOZADVSLOW = 0;
			if(MoveLocationTM10msControlProc(pp->mh->TM_NOZADVSLOW) || (!(pact->ftaskstat & INTF_CEC) && (ChkOperMode(OM_AUTO))))
				ACT_STEP_NEW(END_STEP);

			CHK_SET_MOV(SetNozzleFwdAdj);
			break;
		case END_STEP:
			SET_OFF(NozzleFwdOffDelay);                                //座进关闭			
			pp->st->TM_NOZZLEFWD = (UI16)(ACT_RUN_MS/10);
			if(pp->mh->TM_DLYNOZCOM > NOZZLE_TM_MAX)
				pp->mh->TM_DLYNOZCOM = 0;
			ACT_STEP_NEW(END_STEP + 1);
			ActSetMovTM(pact->ptsk, pp->mh->TM_DLYNOZCOM);
			break;
		case END_STEP + 1:
			if(ACT_STEP_100US >= ((UI32)pp->mh->TM_DLYNOZCOM)*100)
			{
				ret = ACT_END;
			}
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
		return NozzleBackgrd(pp, pact);
}

ACT_RET NozzleFwdAdj(PACT pact, UI32 para)
{
	PNOZZLE pp = (PNOZZLE)pact->part;

	if(pp == NULL)
		return ACT_ERR_ABORT;

	if(!ChkOperMode(OM_ADJU))
		return ACT_ERR_ABORT;

	if(ActIODelayLimit(pp->po->O_NOZ_FWD, TRUE))
	{
	    return ACT_ERR_ABORT;
	}
		
	switch (pact->step)
	{
		case INIT1_STEP:
			ACT_STEP_NEW(STAGE_1_STEP);
			break;

   		case STAGE_1_STEP:
		case STAGE_1_STEP + 1:
    		MoveLocationNoControlProc();
			CHK_SET_MOV(SetNozzleFwdAdj);
      		break;
			
      	default:
      		return ACT_END;
	}

	return ACT_OK;
}

void SetNozzleBwd(PNOZZLE pp, PACT pact)
{
	UI16 hydelay;
	if(pp->mh->NZLB_HYDELAY > 9999)
		pp->mh->NZLB_HYDELAY = 0;
	if(pp->mh->TMDLY_VLV_ONNZLB > 500)
		pp->mh->TMDLY_VLV_ONNZLB = 0;
	hydelay = pp->mh->NZLB_HYDELAY;
	SetPOBitDelay(pp->po->O_NOZ_BWD,PC_ON, pp->mh->TMDLY_VLV_ONNZLB);
	SetPOBit(pp->po->O_NOZ_FWD,PC_OFFD);

	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_NOZRET,pp->pa->DAFLOW_NOZRET, 0, 0, pp->mh->DAPRES_NOZRAMP,pp->mh->DAFLOW_NOZRAMP,
						HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_NOZZLE),pp->hy->HYDR_PUMPS_NOZZLE,pp->hy->HYDR_PUMPS_NOZZLE,
						pp->hy->SERVO_KP_NOZZLE,pp->hy->SERVO_KI_NOZZLE, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_NOZZLE), pp->pa->PER_DAFLOW_NOZZLE); 
}

void        SetNozzleBwdAdj(PNOZZLE pp, PACT pact)
{
	UI16 hydelay;
	if(pp->mh->NZLB_HYDELAY > 9999)
		pp->mh->NZLB_HYDELAY = 0;
	hydelay = pp->mh->NZLB_HYDELAY;
	SetPOBit(pp->po->O_NOZ_BWD,PC_ON);
	
	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->mh->DAPRES_NOZSETUP,pp->mh->DAFLOW_NOZSETUP,0,0,pp->mh->DAPRES_NOZRAMP,pp->mh->DAFLOW_NOZRAMP,
						HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_NOZZLE),pp->hy->HYDR_PUMPS_NOZZLE,pp->hy->HYDR_PUMPS_NOZZLE,
                        pp->hy->SERVO_KP_NOZZLE,pp->hy->SERVO_KI_NOZZLE, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_NOZZLE), pp->pa->PER_DAFLOW_NOZZLE); 
}

void NozzleBwdOff(PNOZZLE pp, PACT pact)
{
	if(pp->mh->TMDLY_VLVOFFNOZBWD <= 100)
		SetPOBitDelay(pp->po->O_NOZ_BWD, PC_OFF, pp->mh->TMDLY_VLVOFFNOZBWD);	
	else  
		SetPOBit(pp->po->O_NOZ_BWD,PC_OFF);
	HYDR_CLS(pp->mh->DAPRES_OFFNOZRAMP, pp->mh->DAFLOW_OFFNOZRAMP);
}


void NozzleBwdOffDelay(PNOZZLE pp, PACT pact)
{
	if(pp->mh->TMDLY_VLVOFFNOZBWD <= 100)
		SetPOBitDelay(pp->po->O_NOZ_BWD, PC_OFF, pp->mh->TMDLY_VLVOFFNOZBWD);	
	else  
		SetPOBit(pp->po->O_NOZ_BWD,PC_OFF);
	HYDR_CLS(pp->mh->DAPRES_OFFNOZRAMP, pp->mh->DAFLOW_OFFNOZRAMP);
}

ACT_RET NozzleBwdBackgrd(PNOZZLE pp, PACT pact)
{
	pp->st->TM_NOZZLEB_RT = ACT_RUN_MS/10;

  	return ACT_OK;
}

/*判断座退模式*/
UI8 NozzleCalBwdMode(PNOZZLE pp)
{
	UI8	mode = NZ_ACT_STD;
	
	if(pp)
	{	
		if (pp->pa->TM_NOZRETACT > 9999)
		{
			pp->pa->TM_NOZRETACT = 0;
		}
		
		mode = NZ_ACT_STD;
		if(ChkSysCfg(COA_CHL4N) && GetMeterLenByCh(pp->mh->NZL_METERCH) > 0 && !ChkSysCfg(CO0_SERVOP)) //增加判断非伺服机时，才有效
		{
			mode = NZ_ACT_METER;			 //座台使用电子尺
		}		
		else if (ChkSysCfg(COA_CHL4N) && (GetMeterLenByCh(pp->mh->NZL_METERCH) > 0) && ChkSysCfg(CO0_SERVOP) && ChkSysCfg(CO1_ECAT)) //使用ECAT模式有效
		{
			mode = NZ_ACT_METER;			 //座台使用电子尺
		}
		else if(pp->pa->TM_NOZRETACT > 0)
		{
			mode = NZ_ACT_TM;				//标准座退
		}
				
		if(ChkSysCfg(XF3_SHFST))
			 mode = NZ_ACT_FAST;				 //声海快速机座退动作

		if(ChkOperMode(OM_AUTO) == 0 && ChkSysCfg(CO3_TBEM))
			mode = NZ_ACT_NCTRL;
	}

	return mode;
}


ACT_RET NozzleBwd(PACT pact, UI32 para)
{
	PMACHINE pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1));

	PNOZZLE pp = (PNOZZLE)pact->part;
	ACT_RET ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;

	if(ActIODelayLimit(pp->po->O_NOZ_BWD, TRUE))
	{
	    return ACT_ERR_ABORT;
	}
	switch (pact->step)
	{
  		case INIT1_STEP:
			if (pp->mh->TM_DLYNOZRET> NOZZLE_TM_MAX)
				pp->mh->TM_DLYNOZRET = 0;

			pp->mode = NozzleCalBwdMode(pp);
			ACT_STEP_NEW(INIT1_STEP + 1);
			break;
		case INIT1_STEP + 1:   
			if (STEP_10MS_CHK(pp->mh->TM_DLYNOZRET))
			{
				ACT_START();
				if(pp->mode != NZ_ACT_NO)
					ACT_STEP_NEW(NOZZLEBWD);
				else
					ACT_STEP_NEW(END_STEP);
			}
			break;
			
		case NOZZLEBWD:
		case NOZZLEBWD + 1:
			if(pp->pa->TM_NOZRETACT > 9999)
				pp->pa->TM_NOZRETACT = 0;
			if((pp->mode == NZ_ACT_TM && MoveLocationTM10msControlProc(pp->pa->TM_NOZRETACT))  ||		 //做时间座退
			     (pp->mode == NZ_ACT_STD && MoveLocationNMControlProc(pp->pi->I_NOZ_BWD)) ||				//开关式座进动退
			     (pp->mode == NZ_ACT_FAST && MoveLocationTM10msControlProc(pp->pa->TM_NOZRETACT))	|| 		//声海快速机座退动作，无慢速
			     (pp->mode == NZ_ACT_METER && MoveLocationMeterControlProc(pp->pa->ADPOSI_NOZRETEND, GetMeterPosByCh(pp->mh->NZL_METERCH)))) //使用电子尺座退动作
				ACT_STEP_NEW(END_STEP);
			else if(pp->mode == NZ_ACT_NCTRL)
				MoveLocationNoControlProc();					/*动作不停止?*/
			if(CHK_SET_MOV(SetNozzleBwd))
				ActSetTM(pact->ptsk, pp->pa->TM_NOZRETACT);
			break;

		case END_STEP:
			SET_OFF(NozzleBwdOffDelay);                                //座退关闭
			
			if(pp->mh->TM_DLYNOZBCK > NOZZLE_TM_MAX)
				pp->mh->TM_DLYNOZBCK = 0;
				
			ACT_STEP_NEW(END_STEP + 1);
			break;

		case END_STEP + 1:
			if(STEP_10MS_CHK(pp->mh->TM_DLYNOZBCK))
			{
				pp->st->TM_NOZZLEBWD= ACT_RUN_MS/10;
				ret = ACT_END;
			}
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
		return NozzleBwdBackgrd(pp, pact);
}


ACT_RET NozzleBwdAdj(PACT pact, UI32 para)
{
	PNOZZLE pp = (PNOZZLE)pact->part;

	if(pp == NULL)
		return ACT_ERR_ABORT;

	if(!ChkOperMode(OM_ADJU))
		return ACT_ERR_ABORT;

	if(ActIODelayLimit(pp->po->O_NOZ_BWD, TRUE))
	{
	    return ACT_ERR_ABORT;
	}
		
	switch (pact->step)
	{
		case INIT1_STEP:
			ACT_STEP_NEW(STAGE_1_STEP);
			break;

   		case STAGE_1_STEP:
		case STAGE_1_STEP + 1:
    		MoveLocationNoControlProc();

			CHK_SET_MOV(SetNozzleBwdAdj);
			break;

		default:
      		return ACT_END;
	}

	return NozzleBwdBackgrd(pp, pact);	
}

void SetNozzleFwdSync(PNOZZLE pp, PACT pact)
{
	UI16 hydelay,port,pri;
    UI16 i;
    
	if(pp->mh->NZLF_HYDELAY > 9999)
		pp->mh->NZLF_HYDELAY = 0;
	hydelay = pp->mh->NZLF_HYDELAY;

    port = HYDR_PORT(pp->hy->HYDR_PORT_NOZZLE);
    for(i = 0;i < 4;i++)
    {
        if((port>>i)&1)
        {
            pri = 0x3<<(i*2);   //对应通道第四优先级，避免座台和开模同一油路且使用开储联动时，电储料结束关闭座台进时清除开模压力流量 2023-5-22 ccz
            break;
        }
    }
    
    ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->mh->DAPRES_NOZSETUP,pp->mh->DAFLOW_NOZSETUP, 0, 0, pp->mh->DAPRES_NOZRAMP,pp->mh->DAFLOW_NOZRAMP,
						(pact->ftaskstat & INTF_CEC) ? 0 : HYDRDELY, port,pp->hy->HYDR_PUMPS_NOZZLE,pp->hy->HYDR_PUMPS_NOZZLE,
						pp->hy->SERVO_KP_NOZZLE,pp->hy->SERVO_KI_NOZZLE, 0, !(pact->ftaskstat & INTF_CEC), pri, pp->pa->PER_DAFLOW_NOZZLE); 
    
	SetPOBit(pp->po->O_NOZ_FWD,PC_ON);
}

void NozzleFwdSyncOff(PNOZZLE pp, PACT pact)
{
	HYDR_CLS(pp->mh->DAPRES_OFFNOZRAMP, pp->mh->DAFLOW_OFFNOZRAMP);
	SetPOBit(pp->po->O_NOZ_FWD,PC_OFF);
}

ACT_RET NozzleFwdSync(PACT pact, UI32 para)
{
	PNOZZLE pp = (PNOZZLE)pact->part;
	PINJECT pinj = (PINJECT)GetPartByID(MK_PART(INJECT_ID,1));
    PACT pact_chg = (PACT)(pinj->act + OS_CHRG- 1);
    ACT_RET ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;
		
	switch (pact->step)
	{
		case INIT1_STEP:
			if(ChkOperMode(OM_AUTO) || CheckPI(pp->pi->I_NOZ_FWD) == PI_SET)
				ACT_STEP_NEW(STAGE_1_STEP);
			else
                return ACT_END;
			break;

   		case STAGE_1_STEP:
		case STAGE_1_STEP + 1:
    		if(pinj != NULL && ChkActRun(pinj,ACT_SN_BIT(OS_CHRG)) && pact_chg->step < END_STEP) //让储料结束延迟生效，否则座台结束转射退时声音较大 20230522 ccz
			{

			}
			else
			{
				ACT_STEP_NEW(END_STEP);	
			}
			
			CHK_SET_MOV(SetNozzleFwdSync);
      		break;

		case END_STEP:
			SET_OFF(NozzleFwdSyncOff);
			ret = ACT_END;
      		break;
			
      	default:
      		ret = ACT_END;
            break;
	}

	return ret;
}

ACT_RET CoolNormal(PACT pact, UI32 para)
{
	PMACHINE pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1));

	PNOZZLE pp = (PNOZZLE)pact->part;
	UI32  TM_cool;
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
			pact->ptsk->act_set_start_ms=coolstart;
			pact->ptsk->pst->act_tm_n = (UI32)((((UI32)pp->md->TM_COOLING_H) << 16)+ (UI32)pp->md->TM_COOLING_L);
			break;
		case STAGE_1_STEP + 1:
			TM_cool=((((UI32)pp->md->TM_COOLING_H) << 16)+ (UI32)pp->md->TM_COOLING_L)*10;
			if((GetMsTick()- coolstart) >= TM_cool)
				ACT_STEP_NEW(END_STEP);
			break;
		case END_STEP:
			if (pp->pinj != NULL)
			{
				ClampOvlSolOff(pp->pinj->pclmp);
			}
			
			TM_cool = (GetMsTick()- coolstart)/10;
			
			pp->st->TM_COOL_H = (TM_cool>>16)&0xFFFF;
			pp->st->TM_COOL_L = TM_cool&0xFFFF;
			ret = ACT_END;
			pp->coolsts=0;
			break;
		default:
			ret = ACT_END;
			break;
	}
	return ret;
}

ACT_RET CoolBeforeCharge(PACT pact, UI32 para)
{
	UI32 cooltime;
	PNOZZLE pp = (PNOZZLE)pact->part;
	ACT_RET ret= ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;
		
	switch (pact->step)
	{
		case INIT1_STEP:
			cooltime = ((((UI32)pp->md->TM_COOLINGBEFORECHG_H) << 16) + (UI32)pp->md->TM_COOLINGBEFORECHG_L);
			if (cooltime == 0)
			{
				ACT_STEP_NEW(END_STEP);
			}
			else
			{
				ACT_STEP_NEW(STAGE_1_STEP);
			}			
			break;			
		case STAGE_1_STEP:	
			cooltime = ((((UI32)pp->md->TM_COOLINGBEFORECHG_H) << 16) + (UI32)pp->md->TM_COOLINGBEFORECHG_L);
			if(STEP_10MS_CHK(cooltime))
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

void  SetOilStart(PNOZZLE pp, PACT pact)	//油温预热开启
{	
	ActSetHydr(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_OILHEAT,pp->pa->DAFLOW_OILHEAT,0,0,100,100,
						0,HYDR_PORT(pp->hy->HYDR_PORT_NOZZLE),pp->hy->HYDR_PUMPS_NOZZLE,pp->hy->HYDR_PUMPS_NOZZLE,
                        pp->hy->SERVO_KP_NOZZLE,pp->hy->SERVO_KI_NOZZLE, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_NOZZLE)); 
}

void Oiloff(PNOZZLE pp, PACT pact)	//油温预热功能关闭
{
	HYDR_CLS(0, 0);
}

ACT_RET OilHeat(PACT pact, UI32 para)	//油温预热功能
{
	PMOTOR pmot = (PMOTOR)GetPartByID(MK_PART(MOTOR_ID, 1));
	PNOZZLE pp = (PNOZZLE)pact->part;
	ACT_RET ret= ACT_OK;
	
	if(pp == NULL)
		return ACT_ERR_ABORT;

	if(!ChkOperMode(OM_ADJU))
		return ACT_ERR_ABORT;

	if(pp->pa->FL_OILHEAT == 0)
		return ACT_ERR_ABORT;
	
	switch (pact->step)
	{
		case INIT1_STEP:
			if(!MotorIsOn())
			{

				SET_PART_ERR(pmot, ER0_HTMT);
				return ACT_ERR_ABORT;
			}
			else
			{

				ACT_STEP_NEW(STAGE_1_STEP);
			}
			break;
   		case STAGE_1_STEP:		
		case STAGE_1_STEP + 1:	
			if(STEP_100MS_CHK(pp->pa->TM_OILHEAT*60*10))
				ACT_STEP_NEW(END_STEP);
			
			CHK_SET_MOV(SetOilStart);
			break;

		case END_STEP:
			SET_OFF(Oiloff);
			ret = ACT_END;
			break;

		default:
      		ret = ACT_END;
			break;
	}
	
	return ret;
}

void NozzleSetup(PNOZZLE pp)
{
	if(pp != NULL)
	{
		pp->pinj = (PINJECT)GetMachineSubPart(INJECT_ID, GET_PART_SN(pp->part.id));
		if((ChkSysCfg(COA_CHL4N) && !ChkSysCfg(CO0_SERVOP))||  //增加判断非伺服机时，才有效
			(ChkSysCfg(COA_CHL4N) && ChkSysCfg(CO0_SERVOP) && ChkSysCfg(CO1_ECAT)))//Ecat卓越系统有效
		{
			if(pp->mh->NZL_METERCH == 0)
				pp->mh->NZL_METERCH = 5;	//缺省为第5路
		}
	}
}

BOOL InitNozzlePart(PNOZZLE pp, UI8 sn)
{
	static const SUB_INIT_INFO sub_init[]  = NOZZLE_INIT_INFO;
	
	static const ACT_INIT_INFO act_init[] = 
	{
		{OS_NOZF, OS_NOZF, ACT_NTYPE, NozzleFwd, NozzleFwdOff},
		{OS_NOZB, OS_NOZB,ACT_NTYPE, NozzleBwd, NozzleBwdOff},
		{OS_ANOZF, OS_NOZF, ACT_NTYPE, NozzleFwdAdj, NozzleFwdOff},
		{OS_ANOZB, OS_NOZB, ACT_NTYPE, NozzleBwdAdj, NozzleBwdOff},
		{OS_COOL, OS_COOL, ACT_CTRL, CoolNormal, NULL},
		{OS_COLBC, OS_COLBC, ACT_CTRL, CoolBeforeCharge, NULL},
		{OS_OILHRAT,OS_OILHRAT,ACT_NTYPE,OilHeat,Oiloff},
		{OS_NOZF_SYNC, OS_NOZF,ACT_CTRL, NozzleFwdSync, NozzleFwdSyncOff},
	};
									     
	if(!INIT_PART(pp, NOZZLE_ID, sn, sub_init))
		return FALSE;

	if(!INIT_ACT(pp, act_init))
		return FALSE;

	NozzleSetup(pp);

	pp->pi->I_NOZ_FWD = 16;
	pp->pi->I_NOZ_BWD = 17;
	
	return TRUE;
}

UI32 NozzleCmd(PNOZZLE pp, UI32 cmd, UI32 para)
{
	UI32 ret = TRUE;
	
	if(pp == NULL)
		return FALSE;

	switch(GET_CMD(cmd))
	{
		case CMD_SETUP:
			NozzleSetup(pp);
			break;
		case CMD_DATA_RX:			
			break;
		default:
			ret = FALSE;
			break;
	}

	return ret;
}
