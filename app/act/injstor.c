/******************************************************************************
  文 件 名   : injstor.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 射出氮气增压
  函数列表   :
              InitInjStorPart
              InjectStorageOff
              InjectStorageProc
              InjStorLoop
              SetInjectStorageOn
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
#include "servo.h"
#include "nozzle.h"
#include "inject.h"
#include "injstor.h"
#include "hwcfg.h"

void SetInjectStorageOn(PINJSTOR pp, PACT pact)
{
	SetPOBit(pp->po->O_INJ_STO,  PC_ON);		/* INJ STORAGE CHARGE VALVE */
	SetPOBit(pp->po->O_STO_REL, PC_ON);
	
	if(pp->mode & INJSTOR_PQ)		/* 储能时开比例阀 */
	{
		SetNozzleFwdPO(InjectGetNozzle(pp->pinj), FALSE);

		if (pp->pa->DAFLOW_INJSTOR > HYDR_FLOW_MAX) 
			pp->pa->DAPRES_INJSTOR = HYDR_FLOW_MAX;
			
		ActSetHydr(pact,MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_INJSTOR,pp->pa->DAFLOW_INJSTOR,0,0,
					0,0,0,HYDR_PORT(pp->hy->HYDR_PORT_INJSTOR),pp->hy->HYDR_PUMPS_INJSTOR,pp->hy->HYDR_PUMPS_INJSTOR,
					GetkpDefault(),GetkiDefault(), 0,1, HYDR_PRI(pp->hy->HYDR_PORT_INJSTOR));         
	}  
}

void InjectStorageOff(PINJSTOR pp, PACT pact)
{  
	if(pp != NULL)
	{
		SetPOBit(pp->po->O_INJ_STO, PC_OFF);
		SetPOBit(pp->po->O_STO_REL,PC_OFF);
	
	  	if(pp->mode & INJSTOR_PQ)		
			HYDR_CLS(0, 0);
	}
}


void InjectStorageOffDelay(PINJSTOR pp, PACT pact)
{
	if(pp != NULL)
	{
		SetPOBit(pp->po->O_INJ_STO, PC_OFF);
		SetPOBit(pp->po->O_STO_REL,PC_OFF);
	
	  	if(pp->mode & INJSTOR_PQ)
			HYDR_CLS(0, 0);
	}
}

ACT_RET InjectStorageProc(PACT pact, UI32 para)              //氮气辅助
{	
	int PresSensor;
	PINJSTOR pp = (PINJSTOR)(pact->part);
	ACT_RET ret = ACT_OK;
	UI8	tmp;

	if(pp == NULL)
		return ACT_ERR_ABORT;
		
	if(pp->mh->INJSTOR_METERCH <= 0 || (PresSensor = GetMeterPosByCh(pp->mh->INJSTOR_METERCH)) <= 0)
		PresSensor = 2500;
	switch (pact->step) 
	{
		case INIT1_STEP:
			if (CheckPI(pp->pi->I_INJ_STO) == PI_NOUSED)
				ACT_STEP_NEW(END_STEP);
			else
			{
				pp->mode = 0;
				if(ChkSysCfg(XF1_INJST) && ChkSysCfg(COA_CHL5)&& pp->pinj != NULL)	/*使用压力传感器 */
					pp->mode |= INJSTOR_PRESS;

				if(ChkSysCfg(CO3_STEN)==0)		/*PQ阀*/ 
					pp->mode |= INJSTOR_PQ;
			}
			ACT_STEP_NEW(STAGE_1_STEP);
			break;

		case STAGE_1_STEP:
		case STAGE_1_STEP + 1:
			tmp = (pp->mode & (INJSTOR_PQ |INJSTOR_PRESS));
			if((tmp == (INJSTOR_PQ |INJSTOR_PRESS) && MoveLocationMeterControlProc(PresSensor+10, GetInjectHoldPress(pp->pinj)*10)) ||      /*压力传感器 PQ阀*/
			     (tmp == INJSTOR_PQ && MoveLocationNMControlProc(pp->pi->I_INJ_STO)) ||		/*PQ阀*/
			     (tmp == INJSTOR_PRESS && PresSensor+10 > GetInjectHoldPress(pp->pinj)*10) ||					/*压力传感器*/
			    (tmp == 0 && TestPI(pp->pi->I_INJ_STO)))													/*无，用输入点*/
			{
				ACT_STEP_NEW(END_STEP);
			}

			CHK_SET_MOV(SetInjectStorageOn);
			break;
			
		case END_STEP:
			SET_OFF(InjectStorageOffDelay);
			if(pp->mode & INJSTOR_PQ)
				ret = ACT_END;
			else
				ACT_STEP_NEW(ACT_END + 1);
			break;
		case END_STEP+ 1:
			if(((pp->mode & INJSTOR_PRESS) && PresSensor+20 > GetInjectHoldPress(pp->pinj)*10) ||(!(pp->mode & INJSTOR_PRESS) && TestPI(pp->pi->I_INJ_STO)==0))
			{
				if(InjectAccIsUsed(pp->pinj) && ChkActRun(pp->pinj, ACT_SN_BIT(OS_INJE)))// | ACT_SN_BIT(OS0_HLDP)))
					ACT_STEP_NEW(INIT1_STEP);
				else
					ret = ACT_END;
			}
			break;
		default:
			ret =  ACT_END;
			break;
	}
	
    return ret;
}      

void InjStorLoop(PINJSTOR pp)
{
	if(pp == NULL)
		return;

	if(InjectAccIsUsed(pp->pinj) && ChkActRun(pp, ACT_SN_BIT(OS_INJSTO)))			// | ACT_SN_BIT(OS0_HLDP)))
		ForkAct(GET_ACT(pp, OS_INJSTO), FIRST_FORK, NULL, 0, 0);
}

BOOL InitInjStorPart(PINJSTOR pp, UI8 sn)
{
	static const SUB_INIT_INFO sub_init[]  = INJSTOR_INIT_INFO;
	
	static const ACT_INIT_INFO act_init[] = {{OS_INJSTO, OS_INJSTO, ACT_NTYPE, InjectStorageProc, InjectStorageOff}};
									     
	if(!INIT_PART(pp, INJSTOR_ID, sn, sub_init))
		return FALSE;

	if(!INIT_ACT(pp, act_init))
		return FALSE;

	pp->pinj = (PINJECT)GetMachineSubPart(INJECT_ID, sn);

	pp->pi->I_INJ_STO = 2;

	pp->mh->INJSTOR_METERCH = DEF_PRESSENS2_METER_CH;

	return TRUE;
}

