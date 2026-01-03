/******************************************************************************
  文 件 名   : lub.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 润滑部件
  函数列表   :
              InitLubPart
              LubChkPIChk
              LubCmd
              LubLevelErr
              LublicateTask
              LubNeedChk
              LubSensErr
              LubVlvOff
              LubVlvOn
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
#include "lub.h"

void LubLastClmpUp()
{
	WORD i = 0;
	PCLAMP pclmp = (PCLAMP)GetMachineSubPart(CLAMP_ID, 1);

	if(pclmp == NULL)
		return;
		
	for(i = 0; i < CLAMP_MAX_LUB; i++)
	{
		if(pclmp->plub[i] != NULL)
		{			
			pclmp->plub[i]->st->LUB_CLMP_LEFT = pclmp->plub[i]->mh->CN_LUBUNIT - pclmp->plub[i]->st->CN_CLAMPLUBRIC;

			if(pclmp->plub[i]->st->CN_CLAMPLUBRIC >= pclmp->plub[i]->mh->CN_LUBUNIT)
			{
				pclmp->plub[i]->st->LUB_CLMP_LEFT = 1;
			}
			
			PartDataTx((PPART)pclmp->plub[i], ST_ID, 1, GET_IND(PLUB_STATE, LUB_CLMP_LEFT), TRUE);	
		}			
	}
}

void LubClmpLeftClear(PLUB pp)
{
	if(pp != NULL)
	{
		pp->st->CN_CLAMPLUBRIC = 0;
		pp->st->LUB_CLMP_LEFT = pp->mh->CN_LUBUNIT;
		PartDataTx((PPART)pp, ST_ID, 2, GET_IND(PLUB_STATE, LUB_CLMP_LEFT), TRUE);
	}		
}

void LubStateSet(PLUB pp, WORD state)
{
	pp->st->FL_MOLDLUBRIC = state;
	PartDataTx((PPART)pp, ST_ID, 1, GET_IND(PLUB_STATE, FL_MOLDLUBRIC), TRUE);
}

void LubNeedChk(PLUB pp,UI16 lubcnt)
{
	if (pp != NULL)
	{
		if(pp->lub_on)
		{
			pp->st->CN_CLAMPLUBRIC++;
			if(pp->st->CN_CLAMPLUBRIC > pp->mh->CN_LUBUNIT)
			{
				pp->st->CN_CLAMPLUBRIC = pp->mh->CN_LUBUNIT;
			}
			pp->st->LUB_CLMP_LEFT = pp->mh->CN_LUBUNIT - pp->st->CN_CLAMPLUBRIC;
			
			if(pp->mh->CN_LUBUNIT == 0 || pp->st->LUB_CLMP_LEFT == 0 || pp->luberrtwice == 1||lubcnt==0)
			{
				pp->st->CN_CLAMPLUBRIC = 0;
				pp->st->AUTOLUB_NEED = TRUE;
			}
		}
		else
		{
			pp->st->LUB_CLMP_LEFT = 0;
			pp->st->CN_CLAMPLUBRIC = 0;
			pp->st->AUTOLUB_NEED = FALSE;
		}
	}
}

BOOL LubLevelErr(PLUB pp)
{
	if(pp != NULL)
	{
		if(ChkSysCfg(CO3_LUBC)) 
		{
			if (CheckPI(pp->pi->I_LUB_LVL) == PI_RESET)
			{
				SET_PART_ERR(pp, ER1_LUBL);
				return TRUE;
			}
		}
		else if	(CheckPI(pp->pi->I_LUB_LVL) == PI_SET)
		{
			SET_PART_ERR(pp, ER1_LUBL);
			return TRUE;
		}
	}
	return(FALSE);    
}

PI_STATUS LubChkPIChk(PLUB pp)
{
	if(pp != NULL)
		return CheckPI(pp->pi->I_LUB_CHK);
	else
		return PI_NOUSED;
}
/*---------------------------------------------------------------------------+
|           code segment                                                     |
+---------------------------------------------------------------------------*/
void LubVlvOn(PLUB pp, PACT pact)            //i 润滑编号
{
	SetPOBit(pp->po->O_LUBRCTR,PC_ON);
	LubStateSet(pp,TRUE);
}

void LubVlvOff(PLUB pp, PACT pact)
{
	SetPOBit(pp->po->O_LUBRCTR, PC_OFF);
	LubStateSet(pp,FALSE);
}


void LubVlvOffDelay(PLUB pp, PACT pact)
{
	SetPOBit(pp->po->O_LUBRCTR, PC_OFF);
	LubStateSet(pp,FALSE);
}

void LubSensErr(PLUB pp, PACT pact)
{
	LubVlvOff(pp, pact);
	SET_PART_ERR(pp, ER1_LUBR);
}

/*--------------------------------------------------------------------------*/
ACT_RET LublicateTask(PACT pact, UI32 para)
{
	PLUB pp = (PLUB)pact->part;
	ACT_RET ret = ACT_OK;
	UI16	 mode;
	if(pp == NULL)
		return ACT_ERR_ABORT;
	switch (pact->step) 
	{
		case INIT1_STEP:
			if(ChkOperMode(OM_AUTO) && pp->st->AUTOLUB_NEED== 0)
				return ACT_END;

			if (LubLevelErr(pp))                    //润滑油位检查
				return ACT_ERR_ABORT;
			else
			{
				ACT_STEP_NEW(STAGE_1_STEP);
				pp->lublast = Get10MsTick();
				pp->st->AUTOLUB_NEED = 0;
			}
			break;

		case STAGE_1_STEP:
		 	if(pp->mh->TM_LUBACT > 0)
			    LubVlvOn(pp, pact);                           //启动润滑
			ACT_STEP_NEW(STAGE_2_STEP);
			break;

		case STAGE_2_STEP:                                  //模式判断
			mode = pp->mh->FL_LUBMODE;
			if(mode == LUB_ONE && STEP_10MS_CHK(pp->mh->TM_LUBACT))
				ACT_STEP_NEW(END_STEP);
			else if((mode == LUB_TM_AND_PRESS || mode == LUB_TM_OR_PRESS || mode == LUB_TM_OR_PRESS2) && STEP_10MS_CHK(pp->mh->TM_LUBACT))
			{
				 //继电器多次润滑 时间到
				if(TestPI(pp->pi->I_LUB_CHK))
				{
					LubVlvOff(pp, pact);
					ACT_STEP_NEW(STAGE_3_STEP);
					pp->lubdelay = pp->mh->TM_LUBDLY;
					ActSetTM(pact->ptsk, pp->lubdelay);
				}
				else
				{
				    if(ChkSysCfg(CO5_LUBERTWI) && pp->luberrtwice == 0)
					{
						pp->luberrtwice = 1;
						SET_OFF(LubVlvOff);
			            return ACT_END;
					}
					else
					{
					    pp->luberrtwice = 0;
					    LubSensErr(pp, pact);
						pp->alarmlubfirst = 1;
					    return ACT_ERR_ABORT;					
					}
				}
			}
			else if((mode == LUB_TM_OR_PRESS || mode == LUB_TM_OR_PRESS2) && TestPI(pp->pi->I_LUB_CHK))
			{
				 //继电器多次润滑 压力到即转换
				LubVlvOff(pp, pact);
				pp->lubdelay = pp->mh->TM_LUBDLY +  pp->mh->TM_LUBACT - ACT_STEP_10MS;
				ACT_STEP_NEW(STAGE_3_STEP);
		 	}
		 	else if(mode >= LUB_MODE_END)
				ACT_STEP_NEW(END_STEP);
			break;

		case STAGE_3_STEP:
			if (DIFF_10MS(pp->lublast) >= pp->mh->TM_LUBTOTAL)   //润滑总计时
			{
				ACT_STEP_NEW(END_STEP);
			}
			else if(STEP_10MS_CHK(pp->lubdelay))
			{
				LubVlvOn(pp, pact);                           //启动润滑
				ACT_STEP_NEW(STAGE_2_STEP);
				//ActSetTM(pact->ptsk, pp->mh->TM_LUBACT);
			}
			break;

		case END_STEP:
		    pp->luberrtwice = 0;
			SET_OFF(LubVlvOffDelay);
			pp->lubcmp =1;
			pp->alarmlubfirst = 0;
			ret = ACT_END;
			break;
	}	

	return ret;
}  

BOOL InitLubPart(PLUB pp, UI8 sn)
{
	static const SUB_INIT_INFO sub_init[] = LUB_INIT_INFO;
	static const ACT_INIT_INFO act_init[] = {{OS_LUBR, OS_LUBR, ACT_NTYPE, LublicateTask, LubVlvOff,NULL,TRUE}};
	
	if(pp == NULL || sn > MAX_PART_SN)
		return FALSE;

	memset(pp, 0, sizeof(*pp)); 
	if(!INIT_PART(pp, LUB_ID, sn, sub_init))
		return FALSE;

	if(!INIT_ACT(pp, act_init))
		return FALSE;

	pp->lubdelay = 0;
	pp->lublast = 0;
    pp->luberrtwice = 0;

	if(sn == 1)
	{
		pp->pi->I_LUB_CHK = 21;
		pp->po->O_LUBRCTR = 46;
	}
	
	return TRUE;
}

UI32 LubCmd(PLUB pp, UI32 cmd, UI32 para)
{
	UI16 offset = 0;
	UI16 len = 0;
	if(pp == NULL)
		return FALSE;

	switch(GET_CMD(cmd))
	{
		case CMD_MANUAL_INIT:
			break;
		case CMD_DATA_RX:
			offset = (para >> 16 & 0xffff);
            len = (para & 0xffff);
			
			if((GET_CMD_PARA(cmd) & 0xff) == MH_ID)
			{
				//初始化润滑开关
				if((pp->mh->FL_LUBMODE == 0) || ((pp->mh->FL_LUBMODE != 0) && (CheckPIUsed(pp->pi->I_LUB_CHK))))
				{
					pp->lub_on = 1;
				}
				else
				{
					pp->lub_on = 0;
				}

				
				if(len == 1 && (GET_IND(PLUB_MACHSET, CN_LUBUNIT) == offset))
				{
					if(pp->st->CN_CLAMPLUBRIC >= pp->mh->CN_LUBUNIT)
					{
						pp->st->LUB_CLMP_LEFT = 1;
					}
					else
					{
						pp->st->LUB_CLMP_LEFT = pp->mh->CN_LUBUNIT - pp->st->CN_CLAMPLUBRIC;
					}
					PartDataTx((PPART)pp, ST_ID, 1, GET_IND(PLUB_STATE, LUB_CLMP_LEFT), TRUE);
				}
			}
			break;
	}

	return TRUE;
}

