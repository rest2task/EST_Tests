#include "actcom.h"
#include "shut.h"
#include "clamp.h"
#include "inject.h"

extern HYDR g_hydrout;


void        ShutFwdOff(PSHUT pp, PACT pact)
{
	PHYDR     ph = &g_hydrout;
  	if(pp != NULL)
	{
		SetPOBit(pp->po->O_SHUTFWD,PC_OFF); 
		SetPOBit(ph->po->O_PWRM_P3,PC_OFF);
		SetPOBit(GetSysPO(O_SOEC_SLM), PC_OFF);
		HYDR_CLS(pp->mh->DAPRES_SHUTFWDOFFRAMP, pp->mh->DAFLOW_SHUTFWDOFFRAMP);
	}
}
   
void 		ShutFwdOn(PSHUT pp, PACT pact)
{
	PHYDR     ph = &g_hydrout;
	if(pp != NULL)
	{
		SetPOBit(pp->po->O_SHUTFWD, PC_ON);
		SetPOBit(ph->po->O_PWRM_P3,PC_ON);
		SetPOBit(GetSysPO(O_SOEC_SLM), PC_ON);
		ActSetHydr(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_SHUTFWD,pp->pa->DAFLOW_SHUTFWD,pp->mh->DAPRES_SHUTFWDRAMP,pp->mh->DAFLOW_SHUTFWDRAMP,pp->mh->DAPRES_SHUTFWDRAMP,pp->mh->DAFLOW_SHUTFWDRAMP,
							0,HYDR_PORT(pp->hy->HYDR_PORT_SHUT),1,1,
							GetkpDefault(),GetkiDefault(), 0,1,HYDR_PRI(pp->hy->HYDR_PORT_SHUT));		//因为 HYDR_PUMPS_SYSACC这个值，在伺服泵浦配置里面没有，所以赋值1，不然电机不会转  DK 2014.11.04
  	}
}

void        ShutBwdOff(PSHUT pp, PACT pact)
{
	PHYDR     ph = &g_hydrout;
  	if(pp != NULL)
	{
		SetPOBit(pp->po->O_SHUTBWD,PC_OFF); 
		SetPOBit(ph->po->O_PWRM_P3,PC_OFF);
		SetPOBit(GetSysPO(O_SOEC_SLM), PC_OFF);
		HYDR_CLS(pp->mh->DAPRES_SHUTBWDOFFRAMP, pp->mh->DAFLOW_SHUTBWDOFFRAMP);
	}
}
   
void 		ShutBwdOn(PSHUT pp, PACT pact)
{
	PHYDR     ph = &g_hydrout;
	if(pp != NULL)
	{
		SetPOBit(pp->po->O_SHUTBWD, PC_ON);
		SetPOBit(ph->po->O_PWRM_P3,PC_ON);
		SetPOBit(GetSysPO(O_SOEC_SLM), PC_ON);
		ActSetHydr(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_SHUTBWD,pp->pa->DAFLOW_SHUTBWD,pp->mh->DAPRES_SHUTBWDRAMP,pp->mh->DAFLOW_SHUTBWDRAMP,pp->mh->DAPRES_SHUTBWDRAMP,pp->mh->DAFLOW_SHUTBWDRAMP,
							0,HYDR_PORT(pp->hy->HYDR_PORT_SHUT),1,1,
							GetkpDefault(),GetkiDefault(), 0,1,HYDR_PRI(pp->hy->HYDR_PORT_SHUT));		//因为 HYDR_PUMPS_SYSACC这个值，在伺服泵浦配置里面没有，所以赋值1，不然电机不会转  DK 2014.11.04
  	}
}

ACT_RET  ShutFwdProc(PACT pact, UI32 para)
{
	PSHUT pp = (PSHUT)pact->part;
	ACT_RET ret = ACT_OK;
	
	if(pp == NULL)
		return ACT_ERR_ABORT;
	
	switch (pact->step) 
	{
		case INIT1_STEP:
			ACT_STEP_NEW(SHUTFWD);
			break;
		case SHUTFWD:
		case SHUTFWD+1:
			if((MoveLocationNMControlProc(pp->pi->I_SHUTFWD)&&(pp->mh->FL_SHUT_MODE ==1))||
				(MoveLocationTM10msControlProc(pp->pa->TM_SHUTFWD)&&(pp->mh->FL_SHUT_MODE ==0)))
				ACT_STEP_NEW(END_STEP);		
			CHK_SET_MOV(ShutFwdOn);					
			break;
		case END_STEP:
      		SET_OFF(ShutFwdOff);
      		ret = ACT_END;			
			break;
		default:
			ret = ACT_END;
			break;			
	}
	return ret;
}

ACT_RET  ShutBwdProc(PACT pact, UI32 para)
{
	PSHUT pp = (PSHUT)pact->part;
	ACT_RET ret = ACT_OK;
	
	if(pp == NULL)
		return ACT_ERR_ABORT;
	
	switch (pact->step) 
	{
		case INIT1_STEP:
			ACT_STEP_NEW(SHUTBWD);
			break;
		case SHUTBWD:
		case SHUTBWD+1:
			if((MoveLocationNMControlProc(pp->pi->I_SHUTBWD)&&(pp->mh->FL_SHUT_MODE ==1))||
				(MoveLocationTM10msControlProc(pp->pa->TM_SHUTBWD*10)&&(pp->mh->FL_SHUT_MODE ==0)))
				ACT_STEP_NEW(END_STEP);		
			CHK_SET_MOV(ShutBwdOn);			
			break;
		case END_STEP:
      		SET_OFF(ShutBwdOff);
      		ret = ACT_END;			
			break;
		default:
			ret = ACT_END;
			break;			
	}
	return ret;
}

void ShutSetup(PSHUT pp)
{
	if(pp != NULL)
	{
		pp->pinj= (PINJECT)GetMachineSubPart(INJECT_ID, GET_PART_SN(pp->part.id));
		pp->pclamp = (PCLAMP)GetMachineSubPart(CLAMP_ID, GET_PART_SN(pp->part.id));
	}
}

BOOL InitShutPart(PSHUT pp, UI8 sn)
{
	static const SUB_INIT_INFO sub_init[]  = SHUT_INIT_INFO;
	
	static const ACT_INIT_INFO act_init[] = {{OS_SHUTFWD, OS_SHUTFWD, ACT_NTYPE, ShutFwdProc, ShutFwdOff},
									     {OS_SHUTBWD, OS_SHUTBWD,ACT_NTYPE,  ShutBwdProc, ShutBwdOff}};
									     
	if(!INIT_PART(pp, SHUT_ID, sn, sub_init))
		return FALSE;

	if(!INIT_ACT(pp, act_init))
		return FALSE;

	ShutSetup(pp);	
	return TRUE;
}

UI32 ShutCmd(PSHUT pp, UI32 cmd, UI32 para)
{
	UI32 ret = TRUE;
	
	if(pp == NULL)
		return FALSE;

	switch(GET_CMD(cmd))
	{
		case CMD_SETUP:
			ShutSetup(pp);
			break;
			
		default:
			ret = FALSE;
			break;
	}

	return ret;
}



