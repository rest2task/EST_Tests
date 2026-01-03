/******************************************************************************
  文 件 名   : chargpress.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 储料下压缸及动作函数
  函数列表   :
              ChargePressBwd
              ChargePressFwd
              ChargePressFwdPIChk
              ChargePressLvlPIChk
              ChgPrsBwd
              ChgPrsBwdOff
              ChgPrsFwd
              ChgPrsFwdOff
              InitChgPressPart
              SetChargePressFwdPO
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
#include "inject.h"
#include "chargpress.h"

void SetChargePressFwdPO(PCHGPRS pp, BOOL bon)
{
	if(pp != NULL)
	{
		if(bon)
			SetPOBit(pp->po->O_PRS_FWD, PC_ON);
		else
			SetPOBit(pp->po->O_PRS_FWD, PC_OFF);
	}
}

PI_STATUS ChargePressLvlPIChk(PCHGPRS pp)
{
	if(pp != NULL)
		return CheckPI(pp->pi->I_PRS_LVL);
	else
		return PI_NOUSED;
}

PI_STATUS ChargePressFwdPIChk(PCHGPRS pp)
{
	if(pp != NULL)
		return CheckPI(pp->pi->I_PRS_FWD);
	else
		return PI_NOUSED;
}

/*---------------------------------------------------------------------------+
|           code segment                                                     |
+---------------------------------------------------------------------------*/
void ChgPrsFwd(PCHGPRS pp, PACT pact)                  
{
	/*UI16 pressramp = 0, flowramp = 0;
	UI16 hydelay;
	
	hydelay = DEF_HYDELAY;*/
	
	SetPOBit(pp->po->O_PRS_FWD,PC_ON);

	/*if(pp->pinj != NULL)
	{
		pressramp = pp->pinj->mh->DAPRES_CHGRAMP;
		flowramp = pp->pinj->mh->DAFLOW_CHGRAMP;
	}

	ActSetHydr(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_CHGPRSFWD,pp->pa->DAFLOW_CHGPRSFWD,0,pressramp,0,flowramp,
	            HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_CHGPRS),pp->hy->HYDR_PUMPS_CHGPRS,pp->hy->HYDR_PUMPS_CHGPRS,
	            pp->hy->SERVO_KP_CHGPRS,pp->hy->SERVO_KI_CHGPRS, 0, !(pact->ftaskstat & INTF_CEC), HYDR_PRI(pp->hy->HYDR_PORT_CHGPRS)); */
}

void ChgPrsFwdOff(PCHGPRS pp, PACT pact) 
{
    SetPOBit(pp->po->O_PRS_FWD,PC_OFF);
    //HYDR_CLS(0, 0);
}

void ChgPrsFwdOffDelay(PCHGPRS pp, PACT pact) 
{
    SetPOBit(pp->po->O_PRS_FWD,PC_OFF);
    //HYDR_CLS(0, 0);
}

ACT_RET ChargePressFwd(PACT pact, UI32 para)                    //储料加压控制
{
	PCHGPRS pp = (PCHGPRS)(pact->part);
	ACT_RET ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;
	switch(pact->step)
	{
		case INIT1_STEP:
      		if(TestPI(pp->pi->I_PRS_FWD)) 
				ret = ACT_END;
			else
				ACT_STEP_NEW(STAGE_1_STEP);
			break;
		
		case STAGE_1_STEP:
		case STAGE_1_STEP + 1:
			if(MoveLocationNMControlProc(pp->pi->I_PRS_FWD))
				ACT_STEP_NEW(END_STEP);

            if(!ChkSysCfg(CO5_INGEJT) && ChkOperMode(OM_AUTO))
			{
				if(pp->chgprsflag == 1) 	//自动模式下射出动作开始后，压料缸动作按键无效，手动压料缸动作结束(lzh 2022.1.4)
				{
					ACT_STEP_NEW(END_STEP);
				}
			}

			CHK_SET_MOV(ChgPrsFwd);
			break;

		case END_STEP:
			SET_OFF(ChgPrsFwdOffDelay);
			ret = ACT_END;
		default:
			ret = ACT_END;
	}
		
	return ret;
}  

void ChgPrsBwd(PCHGPRS pp, PACT pact)                  
{
 	/*UI16 pressramp= 0, flowramp = 0;
	UI16 hydelay;
	
	hydelay = DEF_HYDELAY;*/
	
    SetPOBit(pp->po->O_PRS_BWD,PC_ON);

	/*if(pp->pinj != NULL)
	{
		pressramp = pp->pinj->mh->DAPRES_CHGRAMP;
		flowramp = pp->pinj->mh->DAFLOW_CHGRAMP;
	}

	ActSetHydr(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_CHGPRSBWD,pp->pa->DAFLOW_CHGPRSBWD,0,pressramp,0,flowramp,
						HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_CHGPRS),pp->hy->HYDR_PUMPS_CHGPRS,pp->hy->HYDR_PUMPS_CHGPRS,
						pp->hy->SERVO_KP_CHGPRS,pp->hy->SERVO_KI_CHGPRS, 0, !(pact->ftaskstat & INTF_CEC), HYDR_PRI(pp->hy->HYDR_PORT_CHGPRS)); */
}

void ChgPrsBwdOff(PCHGPRS pp, PACT pact)      //动作步监控 手动放开调用
{
    SetPOBit(pp->po->O_PRS_BWD,PC_OFF);
    //HYDR_CLS(0, 0);
}

void ChgPrsBwdOffDelay(PCHGPRS pp, PACT pact)    //动作步监控 自动结束时调用
{
    SetPOBit(pp->po->O_PRS_BWD,PC_OFF);
    //HYDR_CLS(0, 0);
}

ACT_RET ChargePressBwd(PACT pact, UI32 para)                      //储料加压缸后退
{
	PCHGPRS pp = (PCHGPRS)(pact->part);
	ACT_RET ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;
	switch(pact->step)
	{
		case INIT1_STEP:
      		if(TestPI(pp->pi->I_PRS_BWD)) 
			{
      			SET_PART_ERR(pp,ER0_CHGPRSBWD);
				TempAlarm(TRUE);
				ret = ACT_END;
      		}
			else
				ACT_STEP_NEW(STAGE_1_STEP);
			break;
		
		case STAGE_1_STEP:
		case STAGE_1_STEP + 1:
			if(MoveLocationNMControlProc(pp->pi->I_PRS_BWD))
			{
				SET_PART_ERR(pp,ER0_CHGPRSBWD);
				TempAlarm(TRUE);
				ACT_STEP_NEW(END_STEP);
			}

			if(!ChkSysCfg(CO5_INGEJT) && ChkOperMode(OM_AUTO))	//自动模式下射出动作开始后，压料缸动作按键无效，手动压料缸动作结束(lzh 2022.1.4)
			{
				if(pp->chgprsflag == 1)
				{
					ACT_STEP_NEW(END_STEP);
				}
			}

			CHK_SET_MOV(ChgPrsBwd);
			break;

		case END_STEP:
			SET_OFF(ChgPrsBwdOffDelay);
			ret = ACT_END;
		default:
			ret = ACT_END;
	}
	
    return ret;
}  

BOOL InitChgPressPart(PCHGPRS pp, UI8 sn)
{
	static const SUB_INIT_INFO sub_init[]  = CHGPRS_INIT_INFO;

	static const ACT_INIT_INFO act_init[] = {{OS_CHGPRSF, OS_CHGPRSF, ACT_NTYPE, ChargePressFwd, ChgPrsFwdOff},
									   {OS_CHGPRSB, OS_CHGPRSB, ACT_NTYPE, ChargePressBwd, ChgPrsBwdOff}};

	if(!INIT_PART(pp, CHGPRS_ID, sn, sub_init))
		return FALSE;

	if(!INIT_ACT(pp, act_init))
		return FALSE;

	pp->pinj = (PINJECT)GetMachineSubPart(INJECT_ID, sn);
	
	return TRUE;
}
