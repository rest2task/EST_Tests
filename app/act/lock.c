/******************************************************************************
  文 件 名   : lock.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 定位部件
  函数列表   :
              InitLockPart
              LockBwdProc
              LockFwdProc
              SetLockBwd
              SetLockBwdoff
              SetLockFwd
              SetLockFwdoff
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
#include "lock.h"


/****************************************************************************
				定位进
****************************************************************************/
void SetLockFwd(PLOCK pp, PACT pact)
{
	UI16 hydelay;
	
	hydelay = pp->mh->LOCKF_HYDELAY;
	SetPOBit(pp->po->O_LOC_FWD, PC_ON);

	ActSetHydr(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_LCKFWD, pp->pa->DAFLOW_LCKFWD, 0, 0, pp->mh->DAPRES_LOCRAMP, pp->mh->DAFLOW_LOCRAMP, HYDRDELY, 
				HYDR_PORT(pp->hy->HYDR_PORT_LOCK),   pp->hy->HYDR_PUMPS_LOCK, pp->hy->HYDR_PUMPS_LOCK,    pp->hy->SERVO_KP_LOCK,pp->hy->SERVO_KI_LOCK, 
				0, TRUE, HYDR_PRI(pp->hy->HYDR_PORT_LOCK));
}

void SetLockFwdoff(PLOCK pp, PACT pact)
{
	if(pp != NULL)
	{	
		HYDR_CLS(pp->mh->DAPRES_LOCOFFRAMP, pp->mh->DAFLOW_LOCOFFRAMP);
		SetPOBit(pp->po->O_LOC_FWD,PC_OFF);
	}
}


void SetLockFwdoffDelay(PLOCK pp, PACT pact)
{
	if(pp != NULL)
	{
		HYDR_CLS(pp->mh->DAPRES_LOCOFFRAMP, pp->mh->DAFLOW_LOCOFFRAMP);
		SetPOBit(pp->po->O_LOC_FWD,PC_OFF);
	}
}


ACT_RET	LockFwdProc(PACT pact, UI32 para)
{
	PLOCK pp = (PLOCK)pact->part;
	ACT_RET ret = ACT_OK;
        
	if(pp == NULL)
		return ACT_ERR_ABORT;
	switch (pact->step) 
	{
		case INIT1_STEP:
			if(pp->md->FL_LOC)
				ACT_STEP_NEW(LOCKFWD1);
			else
				ret = ACT_END;
			break;
			
		case LOCKFWD1:
		case LOCKFWD1 + 1:
			if(MoveLocationNMControlProc(pp->pi->I_LOC_FWD))
				ACT_STEP_NEW(END_STEP);
			
			CHK_SET_MOV(SetLockFwd);
			break;
			
		case END_STEP: 
			SET_OFF(SetLockFwdoffDelay);
			ret = ACT_END;
			break;
		default:
			ret = ACT_END;
			break;	
	}
		
	return ret;
}




/****************************************************************************
				定位退
****************************************************************************/
void	SetLockBwd(PLOCK pp, PACT pact)
{
	UI16 hydelay;
	
	hydelay = pp->mh->LOCKB_HYDELAY;
	SetPOBit(pp->po->O_LOC_BWD, PC_ON);
        
	ActSetHydr(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_LCKBWD, pp->pa->DAFLOW_LCKBWD, 0, 0, pp->mh->DAPRES_LOCRAMP, pp->mh->DAFLOW_LOCRAMP, HYDRDELY, 
				HYDR_PORT(pp->hy->HYDR_PORT_LOCK),   pp->hy->HYDR_PUMPS_LOCK, pp->hy->HYDR_PUMPS_LOCK,    pp->hy->SERVO_KP_LOCK,pp->hy->SERVO_KI_LOCK, 
				0, TRUE, HYDR_PRI(pp->hy->HYDR_PORT_LOCK));
}

void SetLockBwdoff(PLOCK pp, PACT pact)
{
	if(pp != NULL)
	{
		HYDR_CLS(pp->mh->DAPRES_LOCOFFRAMP, pp->mh->DAFLOW_LOCOFFRAMP);
		SetPOBit(pp->po->O_LOC_BWD,PC_OFF);
	}
}


void SetLockBwdoffDelay(PLOCK pp, PACT pact)
{
	if(pp != NULL)
	{
		HYDR_CLS(pp->mh->DAPRES_LOCOFFRAMP, pp->mh->DAFLOW_LOCOFFRAMP);
		SetPOBit(pp->po->O_LOC_BWD,PC_OFF);
	}
}

ACT_RET	LockBwdProc(PACT pact, UI32 para)
{
	PLOCK pp = (PLOCK)pact->part;
	ACT_RET ret = ACT_OK;
        
	if(pp == NULL)
		return ACT_ERR_ABORT;
	switch (pact->step) 
	{
		case INIT1_STEP:
			if(pp->md->FL_LOC)
				ACT_STEP_NEW(LOCKBWD1);
			else
				ret = ACT_END;;    
			break;

		case LOCKBWD1:
		case LOCKBWD1 + 1:
			if(MoveLocationNMControlProc(pp->pi->I_LOC_BWD))
				ACT_STEP_NEW(END_STEP);

			CHK_SET_MOV(SetLockBwd);
			break;

		case END_STEP: 
			SET_OFF(SetLockBwdoffDelay);
			ret = ACT_END;
			break;

		default:
			ret = ACT_END;
			break;	
	}
	
	return ret;
}

BOOL InitLockPart(PLOCK pp, UI8 sn)
{
	static const SUB_INIT_INFO sub_init[]  = LOCK_INIT_INFO;
	
	static const ACT_INIT_INFO act_init[] = {{OS_LOCF, OS_LOCF, ACT_NTYPE, LockFwdProc, SetLockFwdoff},
									   		  {OS_LOCB, OS_LOCB, ACT_NTYPE, LockBwdProc, SetLockBwdoff}};
									     
	if(!INIT_PART(pp, LOCK_ID, sn, sub_init))
		return FALSE;

	if(!INIT_ACT(pp, act_init))
		return FALSE;

	return TRUE;
}
