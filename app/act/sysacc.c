/******************************************************************************
  文 件 名   : sysacc.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 浮动缸部件及处理
  函数列表   :
              InitSysAccPart
              SysAccOff
              SysAccOn
              SysAccProc
              SysAccStop
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
#include "sysacc.h"

/*---------------------------------------------------------------------------+
|           code segment                                                     |
+---------------------------------------------------------------------------*/
void SysAccOn(PSYSACC pp, PACT pact)
{
	if(pp != NULL)
	{
		SetPOBit(pp->po->O_SYS_ACC, PC_ON);

	   /* ActSetHydr(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_SYSACC,pp->pa->DAFLOW_SYSACC,0,0,0,0,
							0,HYDR_PORT(pp->hy->HYDR_PORT_SYSACC),pp->hy->HYDR_PUMPS_SYSACC,pp->hy->HYDR_PUMPS_SYSACC,
							GetkpDefault(),GetkiDefault(), 0,1,HYDR_PRI(pp->hy->HYDR_PORT_SYSACC));	 	*/
			ActSetHydr(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_SYSACC,pp->pa->DAFLOW_SYSACC,0,0,0,0,
							0,HYDR_PORT(pp->hy->HYDR_PORT_SYSACC),1,pp->hy->HYDR_PUMPS_SYSACC,
							GetkpDefault(),GetkiDefault(), 0,1,HYDR_PRI(pp->hy->HYDR_PORT_SYSACC));		//因为 HYDR_PUMPS_SYSACC这个值，在伺服泵浦配置里面没有，所以赋值1，不然电机不会转  DK 2014.11.04
  	}
}

void SysAccOff(PSYSACC pp, PACT pact)
{
	if(pp != NULL)
	{
		SetPOBit(pp->po->O_SYS_ACC,PC_OFF); 
		//HYDR_CLS(pp->hy->HYDR_PORT_SYSACC, 0, 0);
		HYDR_CLS(0, 0);
	}
}

/*--------------------------------------------------------------------------*/
ACT_RET SysAccProc(PACT pact, UI32 para)                          //浮动缸处理
{
	PSYSACC pp = (PSYSACC)pact->part;
	ACT_RET ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;
	
	switch (pact->step) 
	{
		case INIT1_STEP:
			ACT_STEP_NEW(STAGE_1_STEP);
			break;
			
		case STAGE_1_STEP:
		case STAGE_1_STEP + 1:
			if(MoveLocationNMControlProc(pp->pi->I_SYS_ACC))
				ACT_STEP_NEW(END_STEP);

			CHK_SET_MOV(SysAccOn);
			break;
			
    		case END_STEP: 
      	default:
      		SET_OFF(SysAccOff);
      		ret = ACT_END;
      		break;
	}
	
  	return ret;
}  

BOOL InitSysAccPart(PSYSACC pp, UI8 sn)
{
	static const SUB_INIT_INFO sub_init[]  = SYSACC_INIT_INFO;
	
	static const ACT_INIT_INFO act_init[] = {{OS_SYSACC, OS_SYSACC, ACT_NTYPE, SysAccProc, SysAccOff}};
									     
	if(!INIT_PART(pp, SYSACC_ID, sn, sub_init))
		return FALSE;

	if(!INIT_ACT(pp, act_init))
		return FALSE;

	return TRUE;
}

