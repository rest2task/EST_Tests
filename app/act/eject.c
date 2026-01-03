/******************************************************************************
  文 件 名   : eject.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   :  脱模部件及动作函数
  函数列表   :
              AdjEjtBwd
              AdjEjtFwd
              EjectBackgrd
              EjectBwd
              EjectBwdErrChk
              EjectClampClsBwdVlvOff
              EjectClampClsVlvOn
              EjectCmd
              EjectComVlv
              EjectFwd
              EjectFwdErrChk
              EjectGetMode
              EjectProc
              EjectSetup
              EjtFwdPreChk
              InitEjectPart
              SetEjtAdjBwd
              SetEjtAdjFwd
              SetEjtBwd
              SetEjtBwdFast
              SetEjtBwdOff
              SetEjtFwd
              SetEjtFwdFirst
              SetEjtFwdOff
              SetEjtVbrBwd
              SetEjtVbrFwd
              VibrEjtBwd
              VibrEjtFwd
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
#include "safedoor.h"
#include "robot.h"
#include "eject.h"
#include "machapi.h"
#include "machine.h"
#include "recycle.h"
#include "HydrPos.h"
#include "EleCtrlApp.h"
#include "coreprotect.h"

extern   MACHINE m_machine;
extern I16 flow_dot;

UI16 GetMaxRPM(WORD ServoNo);


void InitEjectRamp(PEJECT pp)
{
	//启动
	UI16 start,Acc;
	//中间
	UI16 mid;
	//结束
	UI16 end;
	
	if(pp->md->FL_EJEF_SMOOTH == 1)
	{
		Acc = ((UI32)pp->md->EJEF_MAX_SPD*(UI32)pp->md->EJTF_SPD_STARTON/100);
		Acc = Acc < 2 ? 2: Acc; 	
		start = 100000/Acc;
		
		Acc = ((UI32)pp->md->EJEF_MAX_SPD*(UI32)pp->md->EJTF_SPD_HALFWAY/100);
		Acc = Acc < 2 ? 2: Acc; 
		mid = 100000/Acc;
		
		Acc = ((UI32)pp->md->EJEF_MAX_SPD*(UI32)pp->md->EJTF_SPD_STOPOFF/100);
		Acc = Acc < 2 ? 2: Acc;
		end = 100000/Acc;

		pp->spdramp_ejtfwd[EJT_RAMP_FWD_START] = start;
		pp->spdramp_ejtfwd[EJT_RAMP_FWD_MID] = mid;
		pp->spdramp_ejtfwd[EJT_RAMP_FWD_END] = end;			

		Acc = ((UI32)pp->md->EJEF_MAX_PRES*(UI32)pp->md->EJTF_PRES_STARTON/100);
		Acc = Acc < 2 ? 2: Acc; 	
		start = GetSysPresLimit()*100/Acc;
		
		Acc = ((UI32)pp->md->EJEF_MAX_PRES*(UI32)pp->md->EJTF_PRES_HALFWAY/100);
		Acc = Acc < 2 ? 2: Acc; 
		mid = GetSysPresLimit()*100/Acc;
		
		Acc = ((UI32)pp->md->EJEF_MAX_PRES*(UI32)pp->md->EJTF_PRES_STOPOFF/100);
		Acc = Acc < 2 ? 2: Acc;
		end = GetSysPresLimit()*100/Acc;

		pp->pressramp_ejtfwd[EJT_RAMP_FWD_START] = start;
		pp->pressramp_ejtfwd[EJT_RAMP_FWD_MID] = mid;
		pp->pressramp_ejtfwd[EJT_RAMP_FWD_END] = end;	
	}
	else
	{		
		pp->spdramp_ejtfwd[EJT_RAMP_FWD_START] = pp->mh->DAFLOW_EJTRAMP*10;
		pp->spdramp_ejtfwd[EJT_RAMP_FWD_MID] = pp->mh->DAFLOW_EJTRAMP*10;
		pp->spdramp_ejtfwd[EJT_RAMP_FWD_END] = pp->mh->DAFLOW_EJTOFFRAMP*10;

		pp->pressramp_ejtfwd[EJT_RAMP_FWD_START] = pp->mh->DAPRES_EJTRAMP*10;
		pp->pressramp_ejtfwd[EJT_RAMP_FWD_MID] = pp->mh->DAPRES_EJTRAMP*10;
		pp->pressramp_ejtfwd[EJT_RAMP_FWD_END] = pp->mh->DAPRES_EJTOFFRAMP*10;
	}

	if(pp->md->FL_EJEF_SMOOTH == 1)
	{
		Acc = ((UI32)pp->md->EJEB_MAX_SPD*(UI32)pp->md->EJTB_SPD_STARTON/100);
		Acc = Acc < 2 ? 2: Acc; 	
		start = 100000/Acc;
		
		Acc = ((UI32)pp->md->EJEB_MAX_SPD*(UI32)pp->md->EJTB_SPD_HALFWAY/100);
		Acc = Acc < 2 ? 2: Acc; 
		mid = 100000/Acc;
		
		Acc = ((UI32)pp->md->EJEB_MAX_SPD*(UI32)pp->md->EJTB_SPD_STOPOFF/100);
		Acc = Acc < 2 ? 2: Acc;
		end = 100000/Acc;

		pp->spdramp_ejtbwd[EJT_RAMP_BWD_START] = start;
		pp->spdramp_ejtbwd[EJT_RAMP_BWD_MID] = mid;
		pp->spdramp_ejtbwd[EJT_RAMP_BWD_END] = end;			

		Acc = ((UI32)pp->md->EJEB_MAX_PRES*(UI32)pp->md->EJTB_PRES_STARTON/100);
		Acc = Acc < 2 ? 2: Acc; 	
		start = GetSysPresLimit()*100/Acc;
		
		Acc = ((UI32)pp->md->EJEB_MAX_PRES*(UI32)pp->md->EJTB_PRES_HALFWAY/100);
		Acc = Acc < 2 ? 2: Acc; 
		mid = GetSysPresLimit()*100/Acc;
		
		Acc = ((UI32)pp->md->EJEB_MAX_PRES*(UI32)pp->md->EJTB_PRES_STOPOFF/100);
		Acc = Acc < 2 ? 2: Acc;
		end = GetSysPresLimit()*100/Acc;

		pp->pressramp_ejtbwd[EJT_RAMP_BWD_START] = start;
		pp->pressramp_ejtbwd[EJT_RAMP_BWD_MID] = mid;
		pp->pressramp_ejtbwd[EJT_RAMP_BWD_END] = end;
	}
	else
	{		
		pp->spdramp_ejtbwd[EJT_RAMP_BWD_START] = pp->mh->DAFLOW_EJTRAMP*10;
		pp->spdramp_ejtbwd[EJT_RAMP_BWD_MID] = pp->mh->DAFLOW_EJTRAMP*10;
		pp->spdramp_ejtbwd[EJT_RAMP_BWD_END] = pp->mh->DAFLOW_EJTOFFRAMP*10;

		pp->pressramp_ejtbwd[EJT_RAMP_BWD_START] = pp->mh->DAPRES_EJTRAMP*10;
		pp->pressramp_ejtbwd[EJT_RAMP_BWD_MID] = pp->mh->DAPRES_EJTRAMP*10;
		pp->pressramp_ejtbwd[EJT_RAMP_BWD_END] = pp->mh->DAPRES_EJTOFFRAMP*10;
	}
}

ACT_RET EjfFwdPreChk_CBAC()
{
	PEJECT pp = (PEJECT)GetMachineSubPart(EJECT_ID, 1);
	if(ClampOpenReachErrChk(pp->pclmp))
	{
		return ACT_ERR_ABORT;
	}
	else
	{
		return ACT_OK;
	}
}

void EjectFlowDot(PEJECT pp)
{
	UI16 i = 0;
	if(flow_dot)
	{
		return;
	}
	
	for(i = 0; i< EJECT_SEG; i++)
	{
		if (pp->Eject_Flow[i] == 99)
		{
			pp->Eject_Flow[i] = 999;
		}
		else
		{
			pp->Eject_Flow[i] = pp->Eject_Flow[i]*10;
		}		
	}
}

void EjectFwdStartInit(PEJECT pp, PACT pact)
{
	UI16 i=0;
	if(pp == NULL)
		return;
	
    pp->EjectFwd_Flow1  = pp->pa->DAFLOW_EJTADVSTARTFIRST;       
 	pp->EjectFwd_Flow2  = pp->pa->DAFLOW_EJTADV; 
	
    pp->EjectFwd_Pos1 	= pp->pa->ADPOSI_EJTADVSTARTFIRSTEND;   
    pp->EjectFwd_Pos2   = pp->pa->ADPOSI_EJTADVFASTEND;   
    pp->EjectFwd_Pos_First = pp->pa->ADPOSI_EJTFFIRSTEND1;       
 	pp->EjectFwd_Pos_Two = pp->pa->ADPOSI_EJTFFIRSTEND2;           
                                                                                         
	for(i = 0;i<EJECT_SEG;i++)
	{
		pp->Eject_Flow[i] = 0;
		pp->Eject_Pos[i] = 0;
	}
	pp->End_ejectpos = 0;
	pp->Eject_Num = 0;

    pp->Eject_Flow[0] = pp->EjectFwd_Flow1;
    pp->Eject_Flow[1] = pp->EjectFwd_Flow2;

	if(pp->md->FL_EJTFIRSTSET == 1 && pp->ejectcnt == 0)
	{
		pp->Eject_Pos[0] = pp->EjectFwd_Pos_First;
    	pp->Eject_Pos[1] = pp->EjectFwd_Pos_Two;
	}
	else if((pp->md->FL_EJTCTRL == 3) && (pp->pa->ADPOSI_EJTADVSTARTFIRSTEND > 0))
	{
		if((pp->ejectcnt)%2 == 0)	//托模采用递增时，且为单次数时，第一段和第二段终止位置设置为一样
		{
			pp->Eject_Pos[0] = pp->EjectFwd_Pos1;	
    		pp->Eject_Pos[1] = pp->EjectFwd_Pos1;
		}
		else
		{
			pp->Eject_Pos[0] = pp->EjectFwd_Pos1;
    		pp->Eject_Pos[1] = pp->EjectFwd_Pos2;
		}
	}
	else
	{
		pp->Eject_Pos[0] = pp->EjectFwd_Pos1;
    	pp->Eject_Pos[1] = pp->EjectFwd_Pos2;
	}

	EjectFlowDot(pp);
	
	for (i = 0;i < EJECT_SEG;i++)
	{
		pp->Eject_Flow[i] = (UI32)pp->Eject_Flow[i]*(UI32)pp->pa->PER_DAFLOW_EJECT/100;
	}
	/*设置终点位置*/
	pp->End_ejectpos = pp->Eject_Pos[1];
	pp->Eject_Num = 1;
}

void SetEjectPos_Fwd(PEJECT pp, PACT pact)
{
    int seg = 0;
    int PSramp[3] = {0};
    int PPramp[3] = {0};
    int Pspd[MAXNUMSEG] = {0};
    int Ppres[MAXNUMSEG] = {0};
    int Ppos[MAXNUMSEG] = {0};
	UI16 i = 0;
	UI16 endposbuf = 0xFFFF;	//初始化为最大值
	UI16 motor_max_spd = 0;
	int nowpos = 0;
	UI16 flow_limit = 0;
	UI16 mm = 0x01;

	
	motor_max_spd = GetMaxRPM(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_EJECT))-1);

	if(motor_max_spd < 500 || motor_max_spd > 9999)
	{
		motor_max_spd = 1800;
	}
	
	flow_limit = pp->mh->EJEFWD_FLOWS;
	if(flow_limit < 20)
		flow_limit = 20;

	//InitSetHydrStruCtrlData_K(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_EJECT)), motor_max_spd, pp->pclmp->pcore[3]->pa->CN_SCREWIN,flow_limit, pp->pclmp->pcore[3]->pa->CN_SCREWOUT);
    InitSetHydrStruCtrlData(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_EJECT)), motor_max_spd, 0, pp->mh->DIA_CYLINDER_E,pp->mh->DIA_OIL_NUM, GetMotorPump_Q(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_EJECT)),HYDR_PUMP(pp->hy->HYDR_PUMPS_EJECT)),pp->mh->EFFICIENCY_FACTOR_EJF,0,flow_limit, pp->md->PUMP_K_EJEFWD);

	nowpos = GetMeterPosByCh(pp->mh->EJT_METERCH);
	if(nowpos < 0)
	{
		/*防止负数影响判断*/
		nowpos = 0;
	}
	
	if((int)(pp->End_ejectpos) > nowpos)
	{
		endposbuf = pp->End_ejectpos;
	}
		
	seg = 2;
	for(i = 0;i<seg;i++)
	{
		if(pp->Eject_Pos[i] < endposbuf)
		{
			Pspd[i] = ActSetSpdToRPM(HYDR_PORT(pp->hy->HYDR_PORT_EJECT)-1,pp->Eject_Flow[i]);
			Ppos[i] = pp->Eject_Pos[i];
		}
		else
		{
			Pspd[i] = ActSetSpdToRPM(HYDR_PORT(pp->hy->HYDR_PORT_EJECT)-1,pp->Eject_Flow[i]);
			Ppos[i] = endposbuf;
			seg = i+1;
			break;
		}
	}


	PSramp[0] = pp->spdramp_ejtfwd[EJT_RAMP_FWD_START];	
	PSramp[1] = pp->spdramp_ejtfwd[EJT_RAMP_FWD_MID];
	PSramp[2] = pp->spdramp_ejtfwd[EJT_RAMP_FWD_END];

	if (ChkSysCfg(CO8_YL3))
	{
		mm = mm|DECMODE_SPD;
	}

    SetPosCtrlPara(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_EJECT)), DIRECFWD, pp->mh->EJT_METERCH, PSramp, Pspd, PPramp, Ppres, Ppos, seg, pp->md->PUMP_DEAD_EJEFWD, pp->mh->EJTF_HYDELAY,mm,0,0,0);
}



void EjectBwdStartInit(PEJECT pp, PACT pact)
{
	UI16 i=0;
	if(pp == NULL)
		return;
	
    pp->EjectBwd_Flow1 = pp->pa->DAFLOW_EJTRETFIRST;       
 	pp->EjectBwd_Flow2 = pp->pa->DAFLOW_EJTRET; 
	pp->EjectBwd_Flow3 = pp->mh->DAFLOW_EJTSETUP;
	
    pp->EjectBwd_Pos1 = pp->pa->ADPOSI_EJTRETFIRST + pp->mh->ADPOSI_EJTRETEFF;   
    pp->EjectBwd_Pos2 = pp->pa->ADPOSI_EJTRETEND + pp->mh->ADPOSI_EJTRETEFF; 
	pp->EjectBwd_Pos3 = pp->pa->ADPOSI_EJTRETEND;
    pp->EjectBwd_Pos_First = pp->pa->ADPOSI_EJTBFIRSTEND1;       
 	pp->EjectBwd_Pos_Two = pp->pa->ADPOSI_EJTBFIRSTEND2 + pp->mh->ADPOSI_EJTRETEFF;           
                                                                                         
	for(i = 0;i<EJECT_SEG;i++)
	{
		pp->Eject_Flow[i] = 0;
		pp->Eject_Pos[i] = 0;
	}
	pp->End_ejectpos = 0;
	pp->Eject_Num = 0;

    pp->Eject_Flow[0] = pp->EjectBwd_Flow1;
    pp->Eject_Flow[1] = pp->EjectBwd_Flow2;
	pp->Eject_Flow[2] = pp->EjectBwd_Flow3;

	if(pp->md->FL_EJTFIRSTSET == 1 && pp->ejectcnt == 1)
	{
		pp->Eject_Pos[0] = pp->EjectBwd_Pos_First;
    	pp->Eject_Pos[1] = pp->EjectBwd_Pos_Two;
		pp->Eject_Pos[2] = pp->pa->ADPOSI_EJTBFIRSTEND2;
	}
	else
	{
		pp->Eject_Pos[0] = pp->EjectBwd_Pos1;
    	pp->Eject_Pos[1] = pp->EjectBwd_Pos2;
		pp->Eject_Pos[2] = pp->EjectBwd_Pos3;
	}

	EjectFlowDot(pp);
	for (i = 0;i < EJECT_SEG;i++)
	{
		pp->Eject_Flow[i] = (UI32)pp->Eject_Flow[i]*(UI32)pp->pa->PER_DAFLOW_EJECT/100;
	}
	//设置终点位置
	pp->End_ejectpos = pp->Eject_Pos[2];
	pp->Eject_Num = 1;
}

void SetEjectPos_Bwd(PEJECT pp, PACT pact)
{
	int seg = 0;
	int PSramp[3] = {0};
	int PPramp[3] = {0};
	int Pspd[MAXNUMSEG] = {0};
	int Ppres[MAXNUMSEG] = {0};
	int Ppos[MAXNUMSEG] = {0};
	UI16 i = 0;
	UI16 endposbuf = 0x0;	//托模退时初始化为最小值
	UI16 motor_max_spd = 0;
	int nowpos = 0;
	UI16 flow_limit = 0;
	UI16 mm = 0x01;
	
	motor_max_spd = GetMaxRPM(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_EJECT))-1);
	if(motor_max_spd < 500 || motor_max_spd > 9999)
	{
		motor_max_spd = 1800;
	}
	flow_limit = pp->mh->EJEBWD_FLOWS;
	if(flow_limit < 20)
		flow_limit = 20;

	//InitSetHydrStruCtrlData_K(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_EJECT)), motor_max_spd, pp->pclmp->pcore[4]->pa->CN_SCREWOUT,flow_limit, pp->pclmp->pcore[5]->pa->CN_SCREWIN);

	InitSetHydrStruCtrlData(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_EJECT)), motor_max_spd, pp->mh->DIA_PISDIA_E, pp->mh->DIA_CYLINDER_E,pp->mh->DIA_OIL_NUM, GetMotorPump_Q(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_EJECT)),HYDR_PUMP(pp->hy->HYDR_PUMPS_EJECT)),pp->mh->EFFICIENCY_FACTOR_EJB,0,flow_limit, pp->md->PUMP_K_EJEBWD);
	
	nowpos = GetMeterPosByCh(pp->mh->EJT_METERCH);
	if(nowpos < 0)
	{
		/*防止负数影响判断*/
		nowpos = 0;
	}
	
	if((int)(pp->End_ejectpos) < nowpos)
	{
		endposbuf = pp->End_ejectpos;
	}
		
	seg = 2;
	for(i = 0;i<seg;i++)
	{
		if(pp->Eject_Flow[i] == 990)
		{
			pp->Eject_Flow[i] = 999;
		}
		if(pp->Eject_Pos[i] > endposbuf)
		{
			Pspd[i] = ActSetSpdToRPM(HYDR_PORT(pp->hy->HYDR_PORT_EJECT) - 1,pp->Eject_Flow[i]);
			Ppos[i] = pp->Eject_Pos[i];
		}
		else
		{
			Pspd[i] = ActSetSpdToRPM(HYDR_PORT(pp->hy->HYDR_PORT_EJECT) - 1,pp->Eject_Flow[i]);
			Ppos[i] = endposbuf;
			seg = i+1;
			break;
		}
	}

	PSramp[0] = pp->spdramp_ejtbwd[EJT_RAMP_BWD_START];	
	PSramp[1] = pp->spdramp_ejtbwd[EJT_RAMP_BWD_MID];
	PSramp[2] = pp->spdramp_ejtbwd[EJT_RAMP_BWD_END];

	if (ChkSysCfg(CO8_YL3))
	{
		mm = mm|DECMODE_SPD;
	}

	SetPosCtrlPara(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_EJECT)), DIRECBWD, pp->mh->EJT_METERCH, PSramp, Pspd, PPramp, Ppres, Ppos, seg, pp->md->PUMP_DEAD_EJEBWD, pp->mh->EJTB_HYDELAY,mm,0,0,0);
}


BOOL EjectWaitOpenCoreBwd(PEJECT pp)  //脱模前等待联动中子退  QRC.20170419
{
	PMACHINE pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1));
	UI8 ret = FALSE;
	 
    if(pp !=NULL)
    {
        if(ChkSysCfg(CO1_SOEC) && pm->md->FL_OPNEJTCORE > 1)
		{
		   if(!ActIsInTsk(MK_ACT(CORE_ID,pm->clampopncore_num, OS_CPRB)))
		   ret=TRUE;//此处用任务链表中的联动中子号，防止等待中子过程中改变联动参数导致脱模提前开始。 QRC.20170419
		}
		else
		   ret=TRUE;
    }
	return ret;		
}

UI16 EjectGetMode(PEJECT pp)
{
	if(pp != NULL)
		return pp->md->FL_EJTCTRL;
	else
		return 0;
}

void EjectClampClsBwdVlvOff(PEJECT pp) //LY
{
	if(pp != NULL)
	{
		if(TestPOIsOn(pp->po->O_EJT_BWD))
			SetPOBit(pp->po->O_EJT_BWD, PC_OFF);
	}
}

void EjectClampClsVlvOn(PEJECT pp)
{
	if(pp != NULL)
	{
		if(CUS_ID==CS_YUYAOLOG || !ChkSysCfg(XF3_EJTVB))//关模时开脱退阀，默认不用脱膜次数设0  QRC.20170419
			SetPOBit(pp->po->O_EJT_BWD, PC_ON);
		else if(pp->md->CN_EJT == 0)
			SetPOBit(pp->po->O_EJT_BWD, PC_ON);
	}
}

BOOL EjectBwdErrChk(PEJECT pp, BOOL bset_err)//LY
{
	BOOL ejerr = FALSE;

	if(pp != NULL)
	{
		if(pp->mh->ADPOSI_EJTRETEFF > 99)
			pp->mh->ADPOSI_EJTRETEFF = 10;
		if(GetMeterLenByCh(pp->mh->EJT_METERCH) > 0)
			ejerr = (GetMeterPosByCh(pp->mh->EJT_METERCH) >  (int)(pp->pa->ADPOSI_EJTRETEND+ pp->mh->ADPOSI_EJTRETEFF));	

		if(CheckPI(pp->pi->I_EJT_BWD) == PI_RESET)
			ejerr = TRUE;

		if(CheckPI(pp->pi->I_EJT_PROT) == PI_RESET)
			ejerr = TRUE;

		if(ejerr && bset_err)
			SET_PART_ERR(pp, ER0_EJTP);
	}

	return ejerr;
}


BOOL EjectFwdErrChk(PEJECT pp,  BOOL bset_err)//LY
{
	BOOL ejerr = FALSE;

	if(pp != NULL)
	{
		if(GetMeterLenByCh(pp->mh->EJT_METERCH) > 0)
			ejerr = (GetMeterPosByCh(pp->mh->EJT_METERCH) <  (int)(pp->pa->ADPOSI_EJTADVFASTEND));	
		else if(CheckPI(pp->pi->I_EJT_FWD) == PI_RESET)
			ejerr = TRUE;

		if(ejerr && bset_err)
			SET_PART_ERR(pp, ER0_EJTP);
	}

	return ejerr;
}

void EjectComVlv(PEJECT pp)
{
	if(ChkSysCfg(CO1_SOEC))
	{
		SetPOBit(GetSysPO(O_CP_PUMP), PC_ON);
		SetPOBit(GetSysPO(O_SOEC_SLM), PC_ON);
    	SetPOBit(GetSysPO(O_OPN_EJE_COR),PC_ON);	
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

void EjectComVlvoff(PEJECT pp)
{
	if(ChkSysCfg(CO1_SOEC))
	{
		if(!ChkActRun(pp->pclmp, ACT_SN_BIT(OS_OPNM) | ACT_SN_BIT(OS_EOPN)))
			SetPOBit(GetSysPO(O_OPN_EJE_COR), PC_OFF);
 	}
	SetPOBit(GetSysPO(O_CP_PUMP), PC_OFF);
	SetPOBitDelay(GetSysPO(O_SOEC_SLM),PC_OFF, GetSysMold()->TMDLY_LINKVLVOFF);
	SetPOBit(GetSysPO(O_HYD_VLV2), PC_OFF);
	SetPOBit(GetSysPO(O_HYD_COMB), PC_OFF);
}

void SetEjtFwdFirst(PEJECT pp, PACT pact)
{
	UI16 hydelay,mode;
	UI16 pramp,framp;

	if(pp->mh->EJTF_HYDELAY > 9999)
		pp->mh->EJTF_HYDELAY = 0;
	if(pp->mh->TMDLY_VLV_ONEJTF > 500)
		pp->mh->TMDLY_VLV_ONEJTF = 0;
	hydelay = pp->mh->EJTF_HYDELAY;
	
	EjectComVlv(pp);
	SetPOBitDelay(pp->po->O_EJT_FWD, PC_ON, pp->mh->TMDLY_VLV_ONEJTF);

	pramp = pp->pressramp_ejtfwd[EJT_RAMP_FWD_START]/10;
	framp = pp->spdramp_ejtfwd[EJT_RAMP_FWD_START]/10;
	if(CUS_ID == CS_YONGJIANG)
	{
		if(pramp < 30)
			pramp = 30;
		if(framp < 30)
			framp = 30;
	}
	
	if(pp->md->FL_EJEF_SMOOTH == 1)
		mode = MC_CTR_MODE_POSI|MC_CTR_MODE_PRES;
	else
		mode = MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES;
	ActSetHydr_SpdSR(pact, mode, pp->pa->DAPRES_EJTADVSTARTFIRST, pp->pa->DAFLOW_EJTADVSTARTFIRST, pramp, framp, pramp, framp, HYDRDELY,  
						 HYDR_PORT(pp->hy->HYDR_PORT_EJECT), pp->hy->HYDR_PUMPS_EJECT, pp->hy->HYDR_PUMPS_EJECT, pp->hy->SERVO_KP_EJECT, pp->hy->SERVO_KI_EJECT, 
						 NOCHANGELAG, TRUE, HYDR_PRI(pp->hy->HYDR_PORT_EJECT), pp->pa->PER_DAFLOW_EJECT);
}


void SetEjtFwd(PEJECT pp, PACT pact)
{
	UI16 hydelay;
	UI16 mode;
	UI16 pramp,framp;
	
	if(pp->mh->EJTF_HYDELAY > 9999)
		pp->mh->EJTF_HYDELAY = 0;
	if(pp->mh->TMDLY_VLV_ONEJTF > 500)
		pp->mh->TMDLY_VLV_ONEJTF = 0;
	hydelay = pp->mh->EJTF_HYDELAY;

	EjectComVlv(pp);
	SetPOBitDelay(pp->po->O_EJT_FWD, PC_ON, pp->mh->TMDLY_VLV_ONEJTF);

	pramp = pp->pressramp_ejtfwd[EJT_RAMP_FWD_START]/10;
	framp = pp->spdramp_ejtfwd[EJT_RAMP_FWD_START]/10;
	if(CUS_ID == CS_YONGJIANG)
	{
		if(pramp < 30)
			pramp = 30;
		if(framp < 30)
			framp = 30;
	}
	
	if(pp->md->FL_EJEF_SMOOTH == 1)
		mode = MC_CTR_MODE_POSI|MC_CTR_MODE_PRES;
	else
		mode = MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES;
	ActSetHydr_SpdSR(pact, mode, pp->pa->DAPRES_EJTADV, pp->pa->DAFLOW_EJTADV, pp->pressramp_ejtfwd[EJT_RAMP_FWD_MID]/10, pp->spdramp_ejtfwd[EJT_RAMP_FWD_MID]/10, pramp, framp, HYDRDELY,  
						 HYDR_PORT(pp->hy->HYDR_PORT_EJECT), pp->hy->HYDR_PUMPS_EJECT, pp->hy->HYDR_PUMPS_EJECT, pp->hy->SERVO_KP_EJECT, pp->hy->SERVO_KI_EJECT, 
						 NOCHANGELAG, (!(pact->ftaskstat & INTF_CEC)), HYDR_PRI(pp->hy->HYDR_PORT_EJECT), pp->pa->PER_DAFLOW_EJECT);
}


void SetEjtFwdOff(PEJECT pp, PACT pact)
{
	if(pp->mh->TMDLY_VLV_OFFEJTF > 500)
		pp->mh->TMDLY_VLV_OFFEJTF = 0;
	SetPOBitDelay(pp->po->O_EJT_FWD, PC_OFF, pp->mh->TMDLY_VLV_OFFEJTF);
	EjectComVlvoff(pp);

	HYDR_CLS(pp->mh->DAPRES_EJTOFFRAMP,pp->mh->DAFLOW_EJTOFFRAMP);
	
	if(pp->md->FL_EJEF_SMOOTH == 1)
		PosControlEnd(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_EJECT)));
}

void SetEjtFwdOffDelay(PEJECT pp, PACT pact)
{
	if(pp->mh->TMDLY_VLV_OFFEJTF > 500)
		pp->mh->TMDLY_VLV_OFFEJTF = 0;
	SetPOBitDelay(pp->po->O_EJT_FWD, PC_OFF, pp->mh->TMDLY_VLV_OFFEJTF);
	EjectComVlvoff(pp);
	
	HYDR_CLS(pp->mh->DAPRES_EJTOFFRAMP,pp->mh->DAFLOW_EJTOFFRAMP);
	if(pp->md->FL_EJEF_SMOOTH == 1)
		PosControlEnd(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_EJECT)));
}

void SetEjtFwdOffDelay1(PEJECT pp, PACT pact)
{
	if(pp->mh->TMDLY_VLV_OFFEJTF > 500)
		pp->mh->TMDLY_VLV_OFFEJTF = 0;
	SetPOBitDelay(pp->po->O_EJT_FWD, PC_OFF, pp->mh->TMDLY_VLV_OFFEJTF);
	EjectComVlvoff(pp);
	HYDR_CLS(0,0);
	if(pp->md->FL_EJEF_SMOOTH == 1)
		PosControlEnd(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_EJECT)));
}
ACT_RET EjtFwdPreChk(PEJECT pp, PACT pact)
{
	int i;
	if(ChkOperMode(OM_SENR) || ChkOperMode(OM_TIME))
	{
		if((GetSys()->prbt->md->FL_ROBOTCTRL == 0) && ((pp->md->FL_EJTCTRL == 0) && !ChkSysCfg(XF3_SPSTO)))
		{
			SetOperMode(OM_SEMI | OM_AUTO);
		}
	}
	
	if(!ChkActRun(pp->pclmp, ACT_SN_BIT(OS_OPNM)))
	{
		if(ClampOpenReachErrChk(pp->pclmp))
			return ACT_ERR_ABORT;

		for(i=0;i<CLAMP_MAX_CORE;i++)
		{
			if(pp->pclmp->pcore[i] != NULL)
			{
				if(pp->pclmp->pcore[i]->md->CORE_LIMIT_CHECK == 1 && CoreLimitSwitchErrCheck(pp->pclmp->pcore[i])) //托模进动作前检查中子行程开关
					return ACT_ERR_ABORT;
				if(!ChkSysCfg(XF6_CREJT) && EjectFwdCoreStateErrCheck(pp->pclmp->pcore[i])) //托模进动作前检查中子状态
					return ACT_ERR_ABORT;
			}
		}
	}

	if((pp->md->CN_EJT==0)&&(pp->md->FL_EJTCTRL!=0))
		return ACT_END;
	else
		return ACT_OK;
}

ACT_RET EjtBwdPreChk(PEJECT pp, PACT pact)
{
	int i;

	for(i=0;i<CLAMP_MAX_CORE;i++)
	{
		if(pp->pclmp->pcore[i] != NULL)
		{
			if(pp->pclmp->pcore[i]->md->CORE_LIMIT_CHECK == 1 && CoreLimitSwitchErrCheck(pp->pclmp->pcore[i])) //托模退动作前检查中子行程开关
				return ACT_ERR_ABORT;
			if(!ChkSysCfg(XF6_CREJT) && EjectBwdCoreStateErrCheck(pp->pclmp->pcore[i])) //托模退动作前检查中子状态
				return ACT_ERR_ABORT;
		}
	}

	return ACT_OK;
}

ACT_RET EjectFwdBackgrd(PEJECT pp, PACT pact)
{
	PSAFEDOORCHK pst;
	int ejectpos = 0;
	pst = &(pp->pclmp->psfdr->chk_st);

	if(pact->step >= EJECTFWD)
	{
		if(RobotEjectFwdChk(pp->pclmp->prbt))
			return ACT_ERR_ABORT;
	}

   	if(ChkSysCfg(CO11_SAFEEJT) || ((ChkSysCfg(XF2_OPNBK)&&ChkOperMode(OM_AUTO))))                                       //脱模检测安全门  2014-08-04
   	{
		if (FSafeDoorIsCloseErrChk(pp->pclmp->psfdr))                 //前安全门保护
			return ACT_ERR_ABORT;
   	}
	else
	{
		if(ChkOperMode(OM_SEMI) && !ChkSysCfg(CO5_SAFEDR) && pp->pclmp != NULL 
			&& pp->pclmp->psfdr != NULL && (pst->chkoper == 0))
		{
			/*脱模停留时，托退动作不检测安全门，避免托退时开关安全门后，下一模不检测安全门开关直接合模*/
			if(pp->md->FL_EJTCTRL != 0 || GET_ACT_SN(pact->id) != OS_EJTB)
			{
				/*加代码XF0_RELBP后，半自动首次脱进不检查安全门   QRC 16.3.24 */
			   if(!(ChkSysCfg(XF0_RELBP)&&(ChkActRun(pp, ACT_SN_BIT(OS_EJTF))&&pp->ejectcnt == 0)))
			   	{
				   SafeDoorOpenChk(pp->pclmp->psfdr, pact);
			   	}
			}
		}
	}

	if(ChkSysCfg(XF4_CPRSN))
	{
		if(ClampsafeLight(pp->pclmp)) 		//模板光幕检查
		return ACT_ERR_ABORT;
	}
	 
	pp->st->TM_EJECT_RT = (UI16)((GetMsTick()-pp->TM_start)/10);    //面板显示范围改成0.00-655.35s

	ejectpos = GetMeterPosByCh(pp->mh->EJT_METERCH);
	if (ejectpos < 0)
		ejectpos = 0;
	pp->st->POS_EJEFWDMAX = pp->st->POS_EJEFWDMAX < ejectpos?ejectpos:pp->st->POS_EJEFWDMAX;
	
    return ACT_OK;
}

ACT_RET EjectBwdBackgrd(PEJECT pp, PACT pact)
{
	PSAFEDOORCHK pst;
	pst = &(pp->pclmp->psfdr->chk_st);

	if(pact->step >= EJECTBWD)
	{
		if(RobotEjectBwdChk(pp->pclmp->prbt))
			return ACT_ERR_ABORT;
	}

   	if(ChkSysCfg(CO11_SAFEEJTB) || ((ChkSysCfg(XF2_OPNBK)&&ChkOperMode(OM_AUTO))))                                       //脱模检测安全门  2014-08-04
   	{
		if (FSafeDoorIsCloseErrChk(pp->pclmp->psfdr))                 //前安全门保护
			return ACT_ERR_ABORT;
   	}
	else
	{
		if(ChkOperMode(OM_SEMI) && !ChkSysCfg(CO5_SAFEDR) && pp->pclmp != NULL 
			&& pp->pclmp->psfdr != NULL && (pst->chkoper == 0))
		{
			/*脱模停留时，托退动作不检测安全门，避免托退时开关安全门后，下一模不检测安全门开关直接合模*/
			if(pp->md->FL_EJTCTRL != 0 || GET_ACT_SN(pact->id) != OS_EJTB)
			{
				/*加代码XF0_RELBP后，半自动首次脱进不检查安全门   QRC 16.3.24 */
			   if(!(ChkSysCfg(XF0_RELBP)&&(ChkActRun(pp, ACT_SN_BIT(OS_EJTF))&&pp->ejectcnt == 0)))
			   	{
				   SafeDoorOpenChk(pp->pclmp->psfdr, pact);
			   	}
			}
		}
	}

	if(ChkSysCfg(XF4_CPRSN))
	{
		if(ClampsafeLight(pp->pclmp)) 		//模板光幕检查
		return ACT_ERR_ABORT;
	}
	pp->st->TM_EJECT_RT = (UI16)((GetMsTick()-pp->TM_start)/10);
    return ACT_OK;
}

ACT_RET EjectFwd (PACT pact, UI32 para)
{
	PEJECT pp = (PEJECT)pact->part;
	ACT_RET	ret = ACT_OK;
   	int	tmp = 0;
    
	if(pp == NULL)
		return ACT_ERR_ABORT;

	if(ActIODelayLimit(pp->po->O_EJT_FWD, TRUE))
	{
	    return ACT_ERR_ABORT;
	}
	
	switch (pact->step) 
	{
		case INIT1_STEP:
			ACT_START();
			if((ret = EjtFwdPreChk(pp, pact)) != ACT_OK)
			{
				return ret;
			}	
			else
			{				
				if(EjectWaitOpenCoreBwd(pp))
				{
					ACT_STEP_NEW(INIT1_STEP + 1);
				}
			}
			break;

		case INIT1_STEP + 1: 			
			if(pp->pclmp == NULL || pp->pclmp->prbt == NULL || (!RobotEjectFwdErrChk(pp->pclmp->prbt)
				&& !MoldEjectFwdErrChk(pp->pclmp->prbt)))
			{
				EjectFwdStartInit(pp,pact);
				if(pp->mh->TMDLY_VLV_ONEJTF > 500)
					pp->mh->TMDLY_VLV_ONEJTF = 0;
				if(pp->md->TM_DLYEJTADV > 9999)
					pp->md->TM_DLYEJTADV = 0;
				if(pp->ejectcnt >0)
				{
					ActSetTM(pact->ptsk, pp->md->TM_DLYEJTADV);
				}
				EjectComVlv(pp);
				if((CUS_ID == CS_BONA)&&pp->ejectcnt != 0 &&(ChkSysCfg(CO0_EJTDLY)))		//仅博纳有提前开托进阀功能		LN 2016-01-06
					SetPOBitDelay(pp->po->O_EJT_FWD, PC_ON, pp->mh->TMDLY_VLV_ONEJTF);
				ACT_STEP_NEW(INIT1_STEP + 2);
			}
			break;
                   
		case INIT1_STEP + 2:
			if (CUS_ID != CS_BONA)
			{
				if(pp->md->TM_DLYEJTADV<4)
					tmp = 4;
				else
					tmp = pp->md->TM_DLYEJTADV;
			}	
			else
			{
				tmp = pp->md->TM_DLYEJTADV;
			}	
			
			ActSetTM(pact->ptsk, pp->md->TM_DLYEJTADV);//当前不为末次托进
//---------------托模进延时功能-----------yxh 2020-2-20
			if(((pp->md->CN_EJT == 1)&&((UI16)(ACT_RUN_MS/10) >=tmp))  //单次托模，不管选择首次，末次，每次，都默认托进前延迟
				||((pp->md->CN_EJT != 1)	//非单次，下面对非单次时做区分，非单次时的首次和非首次
					&&((pp->ejectcnt == 0 && (((pp->md->FL_EjtAdvDlyMd != 1)&&((UI16)(ACT_RUN_MS/10) >=tmp))||(pp->md->FL_EjtAdvDlyMd == 1)))    //首次托进
					||(pp->ejectcnt != 0 	//非首次托进  ,当前为末次托进   增加判断，次数小于实际次数直接当做最后一次 防止自动时将托模次数改小引起的问题
						&& ((pp->md->CN_EJT <= pp->ejectcnt+1 &&(((pp->md->FL_EjtAdvDlyMd != 0)&&((UI16)(ACT_RUN_MS/10) >=tmp))||(pp->md->FL_EjtAdvDlyMd == 0)))		//每次或者末次托进延时方式则延时, 首次时则不延时	
						  	||(pp->md->CN_EJT > pp->ejectcnt+1 &&(((pp->md->FL_EjtAdvDlyMd == 2)&&((UI16)(ACT_RUN_MS/10) >=tmp))||(pp->md->FL_EjtAdvDlyMd != 2)))))		//托进延时方式为每次 托进，则延时
						)
					)	
				)			   		
			{						
				ClampEjectFwdStart(pp->pclmp);
				ACT_STEP_NEW(INIT1_STEP + 3);
			}				
			break;

		case INIT1_STEP + 3:
			if(pp->md->FL_EJEF_SMOOTH == 1)
			{
				SET_MOV2(SetEjectPos_Fwd);
			}

			if((GetMeterLenByCh(pp->mh->EJT_METERCH) > 0) && (pp->pa->ADPOSI_EJTADVSTARTFIRSTEND <= pp->pa->ADPOSI_EJTADVFASTEND))
				ACT_STEP_NEW(EJECTFWD);
			else
				ACT_STEP_NEW(EJECTFWD2);
			if(pp->mh->TMDLY_VLV_ONEJTF > 500)
				pp->mh->TMDLY_VLV_ONEJTF = 0;
			if((CUS_ID == CS_BONA)&&(pp->ejectcnt == 0))
				SetPOBitDelay(pp->po->O_EJT_FWD, PC_ON, pp->mh->TMDLY_VLV_ONEJTF);
			break;

		case EJECTFWD:
		case EJECTFWD +1:
			//if(pp->md->FL_EJTFIRSTSET == 1 && pp->ejectcnt == 0)
			if((pp->md->FL_EJTCTRL == 3) && (pp->pa->ADPOSI_EJTADVSTARTFIRSTEND > 0))
			{
				if((pp->ejectcnt)%2 == 0)
				{
					if(MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->EJT_METERCH), pp->pa->ADPOSI_EJTADVSTARTFIRSTEND))
						ACT_STEP_NEW(STAGE_5_STEP);
				}
				else if((pp->ejectcnt)%2 != 0)
				{
					if(MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->EJT_METERCH), pp->pa->ADPOSI_EJTADVSTARTFIRSTEND))
						ACT_STEP_NEW(EJECTFWD2);
				}
				
			}
			else if(pp->md->FL_EJTFIRSTSET == 1 && pp->ejectcnt == 0)
			{
				if(MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->EJT_METERCH), pp->pa->ADPOSI_EJTFFIRSTEND1))
					ACT_STEP_NEW(EJECTFWD2);
			}
			else
			{
				if(MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->EJT_METERCH), pp->pa->ADPOSI_EJTADVSTARTFIRSTEND))
					ACT_STEP_NEW(EJECTFWD2);
			}
			CHK_SET_MOV(SetEjtFwdFirst);
			break;

		case EJECTFWD2:
		case EJECTFWD2 +1:
			if(pp->pa->TM_EJT1STROUND > 9999)
				pp->pa->TM_EJT1STROUND = 0;
			tmp = GetMeterLenByCh(pp->mh->EJT_METERCH);
			if(pp->md->FL_EJTFIRSTSET == 1 && pp->ejectcnt == 0)
			{
	            if((tmp > 0 && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->EJT_METERCH), pp->pa->ADPOSI_EJTFFIRSTEND2)) ||
				  (tmp <= 0 && !ChkSysCfg(XF0_EJTTM) && MoveLocationNMControlProc(pp->pi->I_EJT_FWD)) ||										/*走行程开关*/
				  (ChkSysCfg(XF0_EJTTM) && MoveLocationTM10msControlProc(pp->pa->TM_EJT1STROUND))) 
					ACT_STEP_NEW(STAGE_5_STEP);
			}
			else
			{
	            if((tmp > 0 && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->EJT_METERCH), pp->pa->ADPOSI_EJTADVFASTEND)) ||
				  (tmp <= 0 && !ChkSysCfg(XF0_EJTTM) && MoveLocationNMControlProc(pp->pi->I_EJT_FWD)) ||										/*走行程开关*/
				  (ChkSysCfg(XF0_EJTTM) && MoveLocationTM10msControlProc(pp->pa->TM_EJT1STROUND))) 
					ACT_STEP_NEW(STAGE_5_STEP);
			}

			CHK_SET_MOV(SetEjtFwd);
			break;
		case STAGE_5_STEP://定次托进保持功能
			if(ChkSysCfg(XF4_MIST) && (pp->md->FL_EJTCTRL == 1) && pp->ejectcnt == 0 && (pp->mh->TM_EJTHOLD > 0))
			{
				UI16 hydelay;
				if(pp->mh->EJTF_HYDELAY > 9999)
					pp->mh->EJTF_HYDELAY = 0;
				hydelay = pp->mh->EJTF_HYDELAY;
				ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES,pp->mh->DAPRES_EJTHOLD, pp->mh->DAFLOW_EJTHOLD, 0, 0, pp->pressramp_ejtfwd[EJT_RAMP_FWD_START]/10, pp->spdramp_ejtfwd[EJT_RAMP_FWD_START]/10, HYDRDELY,	
							HYDR_PORT(pp->hy->HYDR_PORT_EJECT), pp->hy->HYDR_PUMPS_EJECT, pp->hy->HYDR_PUMPS_EJECT, pp->hy->SERVO_KP_EJECT, pp->hy->SERVO_KI_EJECT, 
							0, TRUE, HYDR_PRI(pp->hy->HYDR_PORT_EJECT),pp->pa->PER_DAFLOW_EJECT);

				ACT_STEP_NEW(STAGE_5_STEP+1);
			}
			else
			{
			    SET_OFF(SetEjtFwdOffDelay1);//托进使用算法时，定位动作结束时，关闭曲线的压力流量。
				ACT_STEP_NEW(END_STEP);
			}	

			break;
		case STAGE_5_STEP+1://定次托进保持功能
			if (STEP_10MS_CHK(pp->mh->TM_EJTHOLD))
			{
				ACT_STEP_NEW(END_STEP);
			}
			break;
		case END_STEP: 
			if((pp->md->FL_EJTCTRL == 0) && (pp->md->FL_EJTSPECIAL==1)&&(pact->ftaskstat& INTF_CEC) && ChkOperMode(OM_AUTO))
			{
				UI16 hydelay;
				if(pp->mh->EJTF_HYDELAY > 9999)
					pp->mh->EJTF_HYDELAY = 0;
				hydelay = pp->mh->EJTF_HYDELAY;
                SetPOBit(pp->po->O_EJT_FWD, PC_ON);
				ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->mh->DAPRES_EJTSETUP, pp->mh->DAFLOW_EJTSETUP, 0, 0, pp->pressramp_ejtfwd[EJT_RAMP_FWD_START]/10, pp->spdramp_ejtfwd[EJT_RAMP_FWD_START]/10, HYDRDELY,  
							HYDR_PORT(pp->hy->HYDR_PORT_EJECT), pp->hy->HYDR_PUMPS_EJECT, pp->hy->HYDR_PUMPS_EJECT, pp->hy->SERVO_KP_EJECT, pp->hy->SERVO_KI_EJECT, 
							0, TRUE, HYDR_PRI(pp->hy->HYDR_PORT_EJECT),pp->pa->PER_DAFLOW_EJECT);
			}
			else if(!ChkSysCfg(XF0_EJTHOLD))
				SET_OFF(SetEjtFwdOffDelay);

			ACT_STEP_NEW(END_STEP+ 1);			
			break;
		case END_STEP + 1:
			if(GetPOBit(pp->po->O_EJT_FWD) <= 1)
			{
				ACT_STEP_NEW(END_STEP + 2);
				if(pp->mh->TMDLY_EJT_FWDEND > 999)
					pp->mh->TMDLY_EJT_FWDEND = 0;
				ActSetMovTM(pact->ptsk, pp->mh->TMDLY_EJT_FWDEND);
			}
			break;
		case END_STEP + 2:
			if(pp->mh->TMDLY_EJT_FWDEND > 999)
				pp->mh->TMDLY_EJT_FWDEND = 0;
			if(ChkSysCfg(XF4_MIST) && (pp->md->FL_EJTCTRL == 1) && pp->ejectcnt == 0 && (pp->mh->TM_EJTHOLD > 0))
			{
				SET_OFF(SetEjtFwdOff);
				ret = ACT_END;
			}
			else if(STEP_10MS_CHK(pp->mh->TMDLY_EJT_FWDEND))
			{
				if(ChkSysCfg(XF0_EJTHOLD))
				{
					if((pp->md->FL_EJTCTRL == 0) && (pp->md->FL_EJTSPECIAL==1)&&(pact->ftaskstat& INTF_CEC) && ChkOperMode(OM_AUTO))
					{
						//半脱时不切脱进阀。
					}
					else
						SET_OFF(SetEjtFwdOff);
				}
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
		return EjectFwdBackgrd(pp, pact);
}

void SetEjtBwdFast(PEJECT pp, PACT pact)
{
	UI16 hydelay;
	UI16 mode;
	UI16 pramp,framp;
	
	if(pp->mh->EJTB_HYDELAY > 9999)
		pp->mh->EJTB_HYDELAY = 0;
	if(pp->mh->TMDLY_VLV_ONEJTB > 500)
		pp->mh->TMDLY_VLV_ONEJTB = 0;
	hydelay = pp->mh->EJTB_HYDELAY;
	EjectComVlv(pp);
	SetPOBit(pp->po->O_EJT_FWD, PC_OFF);
	SetPOBitDelay(pp->po->O_EJT_BWD, PC_ON, pp->mh->TMDLY_VLV_ONEJTB);

	pramp = pp->pressramp_ejtbwd[EJT_RAMP_BWD_START]/10;
	framp = pp->spdramp_ejtbwd[EJT_RAMP_BWD_START]/10;
	if(CUS_ID == CS_YONGJIANG)
	{
		if(pramp < 30)
			pramp = 30;
		if(framp < 30)
			framp = 30;
	}
	
	if(pp->md->FL_EJEF_SMOOTH == 1)
		mode = MC_CTR_MODE_POSI|MC_CTR_MODE_PRES;
	else
		mode = MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES;
	ActSetHydr_SpdSR(pact, mode, pp->pa->DAPRES_EJTRETFIRST, pp->pa->DAFLOW_EJTRETFIRST, pramp, framp, pramp, framp, HYDRDELY,  
						 HYDR_PORT(pp->hy->HYDR_PORT_EJECT),pp->hy->HYDR_PUMPS_EJECT, pp->hy->HYDR_PUMPS_EJECT, pp->hy->SERVO_KP_EJECT, pp->hy->SERVO_KI_EJECT, 
						 NOCHANGELAG, TRUE, HYDR_PRI(pp->hy->HYDR_PORT_EJECT), pp->pa->PER_DAFLOW_EJECT);
}

void SetEjtBwd(PEJECT pp, PACT pact)
{
	UI16 hydelay,mode;
	UI16 pramp,framp;
	
	if(pp->mh->EJTB_HYDELAY > 9999)
		pp->mh->EJTB_HYDELAY = 0;
	if(pp->mh->TMDLY_VLV_ONEJTB > 500)
		pp->mh->TMDLY_VLV_ONEJTB = 0;
	
	hydelay = pp->mh->EJTB_HYDELAY;
	SetPOBit(pp->po->O_EJT_FWD, PC_OFF);
	EjectComVlv(pp);
	SetPOBitDelay(pp->po->O_EJT_BWD, PC_ON, pp->mh->TMDLY_VLV_ONEJTB);

	pramp = pp->pressramp_ejtbwd[EJT_RAMP_BWD_START]/10;
	framp = pp->spdramp_ejtbwd[EJT_RAMP_BWD_START]/10;
	if(CUS_ID == CS_YONGJIANG)
	{
		if(pramp < 30)
			pramp = 30;
		if(framp < 30)
			framp = 30;
	}
	if(pp->md->FL_EJEF_SMOOTH == 1)
		mode = MC_CTR_MODE_POSI|MC_CTR_MODE_PRES;
	else
		mode = MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES;
	ActSetHydr_SpdSR(pact, mode, pp->pa->DAPRES_EJTRET, pp->pa->DAFLOW_EJTRET, pp->pressramp_ejtbwd[EJT_RAMP_BWD_MID]/10, pp->spdramp_ejtbwd[EJT_RAMP_BWD_MID]/10, pramp, framp, HYDRDELY,  
						 HYDR_PORT(pp->hy->HYDR_PORT_EJECT), pp->hy->HYDR_PUMPS_EJECT, pp->hy->HYDR_PUMPS_EJECT, pp->hy->SERVO_KP_EJECT, pp->hy->SERVO_KI_EJECT, 
						 NOCHANGELAG, (!(pact->ftaskstat & INTF_CEC)), HYDR_PRI(pp->hy->HYDR_PORT_EJECT), pp->pa->PER_DAFLOW_EJECT);
}

void SetEjtAdjBwd(PEJECT pp, PACT pact)
{
	UI16 hydelay;
	UI16 pramp,framp;
	if(pp->mh->EJTB_HYDELAY > 9999)
		pp->mh->EJTB_HYDELAY = 0;
	hydelay = pp->mh->EJTB_HYDELAY;
	EjectComVlv(pp);
	SetPOBit(pp->po->O_EJT_BWD, PC_ON);

	pramp = pp->pressramp_ejtbwd[EJT_RAMP_BWD_START]/10;
	framp = pp->spdramp_ejtbwd[EJT_RAMP_BWD_START]/10;
	if(CUS_ID == CS_YONGJIANG)
	{
		if(pramp < 30)
			pramp = 30;
		if(framp < 30)
			framp = 30;
	}
	
	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->mh->DAPRES_EJTSETUP, pp->mh->DAFLOW_EJTSETUP, pramp, framp, pramp, framp, HYDRDELY,  
						 HYDR_PORT(pp->hy->HYDR_PORT_EJECT),  pp->hy->HYDR_PUMPS_EJECT, pp->hy->HYDR_PUMPS_EJECT, pp->hy->SERVO_KP_EJECT, pp->hy->SERVO_KI_EJECT, 
						 0, (!(pact->ftaskstat & INTF_CEC)), HYDR_PRI(pp->hy->HYDR_PORT_EJECT), pp->pa->PER_DAFLOW_EJECT);
}


void SetEjtAdjFwd(PEJECT pp, PACT pact)
{
	UI16 hydelay;
	UI16 pramp,framp;
	
	if(pp->mh->EJTB_HYDELAY > 9999)
		pp->mh->EJTB_HYDELAY = 0;
	hydelay = pp->mh->EJTB_HYDELAY;
	EjectComVlv(pp);	
	SetPOBit(pp->po->O_EJT_FWD, PC_ON);

	pramp = pp->pressramp_ejtfwd[EJT_RAMP_FWD_START]/10;
	framp = pp->spdramp_ejtfwd[EJT_RAMP_FWD_START]/10;
	if(CUS_ID == CS_YONGJIANG)
	{
		if(pramp < 30)
			pramp = 30;
		if(framp < 30)
			framp = 30;
	}
	
	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->mh->DAPRES_EJTSETUP, pp->mh->DAFLOW_EJTSETUP, pramp, framp, pramp, framp, HYDRDELY, 
						 HYDR_PORT(pp->hy->HYDR_PORT_EJECT),  pp->hy->HYDR_PUMPS_EJECT, pp->hy->HYDR_PUMPS_EJECT, pp->hy->SERVO_KP_EJECT, pp->hy->SERVO_KI_EJECT, 0,
						 TRUE, HYDR_PRI(pp->hy->HYDR_PORT_EJECT),pp->pa->PER_DAFLOW_EJECT);
}


void SetEjtBwdOff(PEJECT pp, PACT pact)
{
	if(pp->mh->TMDLY_VLV_OFFEJTB > 500)
		pp->mh->TMDLY_VLV_OFFEJTB = 0;
	SetPOBitDelay(pp->po->O_EJT_BWD, PC_OFF, pp->mh->TMDLY_VLV_OFFEJTB);
	EjectComVlvoff(pp);
	HYDR_CLS(pp->mh->DAPRES_EJTOFFRAMP,pp->mh->DAFLOW_EJTOFFRAMP);
	if(pp->md->FL_EJEF_SMOOTH == 1)
		PosControlEnd(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_EJECT)));
}

void SetEjtBwdOffDelay(PEJECT pp, PACT pact)
{
	if(pp->mh->TMDLY_VLV_OFFEJTB > 500)
		pp->mh->TMDLY_VLV_OFFEJTB = 0;
	SetPOBitDelay(pp->po->O_EJT_BWD, PC_OFF, pp->mh->TMDLY_VLV_OFFEJTB);
	EjectComVlvoff(pp);
	HYDR_CLS(pp->mh->DAPRES_EJTOFFRAMP,pp->mh->DAFLOW_EJTOFFRAMP);
	if(pp->md->FL_EJEF_SMOOTH == 1)
		PosControlEnd(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_EJECT)));
}

ACT_RET EjectBwd (PACT pact, UI32 para)
{
	//PMACHINE pm = &m_machine;
	PEJECT pp = (PEJECT)pact->part;
	ACT_RET	ret = ACT_OK;
   	int	tmp = 0;
		
	if(pp == NULL)
		return ACT_ERR_ABORT;

	if(ActIODelayLimit(pp->po->O_EJT_BWD, TRUE))
	{
	    return ACT_ERR_ABORT;
	}
	
	switch (pact->step) 
	{
		case INIT1_STEP:
			if(!ActIsInTsk(MK_ACT(EJECT_ID,1, OS_EJT)))  //托模调度任务未运行时，重置变量(手动按托模退、关模前托模退等)
			{	
				pp->TM_start = GetMsTick();
				pp->st->TM_EJECT_RT = 0;
			}
			if((ret = EjtBwdPreChk(pp, pact)) != ACT_OK)
				return ret;
			else if((pp->md->CN_EJT <= 0) && (CheckPI(pp->pi->I_EJT_BWD) == PI_SET))
				ACT_STEP_NEW(END_STEP);
			else
				ACT_STEP_NEW(INIT1_STEP + 1);
			break;

		case INIT1_STEP + 1:
			if(pp->pclmp == NULL || pp->pclmp->prbt == NULL ||(!RobotEjectBwdErrChk(pp->pclmp->prbt))
				&& !MoldEjectBwdErrChk(pp->pclmp->prbt))
			{
				EjectBwdStartInit(pp,pact);
				ACT_STEP_NEW(INIT1_STEP + 2);
				if(pp->md->TM_DLYEJTRET > 9999)
					pp->md->TM_DLYEJTRET = 0;
				ActSetTM(pact->ptsk, pp->md->TM_DLYEJTRET);			
				EjectComVlv(pp);
				SetPOBit(pp->po->O_EJT_FWD, PC_OFF);		
			}
			break;

		case INIT1_STEP + 2:
			if(pp->mh->TMDLY_VLV_ONEJTB > 500)
				pp->mh->TMDLY_VLV_ONEJTB = 0;
			if(pp->md->TM_DLYEJTRET > 9999)
				pp->md->TM_DLYEJTRET = 0;

			if (CUS_ID != CS_BONA)
			{
				if(pp->md->TM_DLYEJTRET<6)
					tmp = 6;
				else
					tmp = pp->md->TM_DLYEJTRET;	
			}
			else
			{
				tmp = pp->md->TM_DLYEJTRET;	
			}
			
			ActSetTM(pact->ptsk, pp->md->TM_DLYEJTRET);  //增加脱模延迟显示   20160509
	
//-----------新托进退延时功能---------------ln2015-12-23
			if((para != 0x02)&&(((pp->md->CN_EJT == 1 &&((UI16)(ACT_RUN_MS/10) >=tmp)) 	//单次托模，不管选择首次，末次，还是每次，都按照设定时间做延迟
				|| ((pp->md->CN_EJT != 1)			//非单次时，首次托退和非首次托退时
					&& ((pp->ejectcnt == 1 && ((pp->md->FL_EjtRetDlyMd != 1 &&((UI16)(ACT_RUN_MS/10) >=tmp))||(pp->md->FL_EjtRetDlyMd == 1)))		//首次托退时，选择每次或首次时延时，末次托退时不延时
					||(pp->ejectcnt > 1 &&		//非首次托退时，以下做末次托退和中间次数托退 区分
						  ((pp->md->CN_EJT <= pp->ejectcnt && ((pp->md->FL_EjtRetDlyMd != 0 &&((UI16)(ACT_RUN_MS/10) >=tmp))||(pp->md->FL_EjtRetDlyMd == 0)))	 //末次托退时，   增加判断，如果次数小于实际次数直接当做最后一次 防止自动时将托模次数改小引起的问题	，选择末次或者每次时延时,首次托退延时不延时
						  	||(pp->md->CN_EJT > pp->ejectcnt &&	((pp->md->FL_EjtRetDlyMd == 2 &&((UI16)(ACT_RUN_MS/10) >=tmp))||(pp->md->FL_EjtRetDlyMd != 2)))))	//中间次数托退时,当选择不为每次托退延时方式时都不延时
						 )
					 )
				)	|| pp->ejectcnt == 0)||//托退终未到，直接做托退动作时的第一次托退
				((para == 0x02) && ((UI16)(ACT_RUN_MS/10) >=tmp)))	//停留模式托退动作前延迟
			{
				if((GetMeterLenByCh(pp->mh->EJT_METERCH) > 0)&&(pp->pa->ADPOSI_EJTRETFIRST<GetMeterMax(pp->mh->EJT_METERCH)) && (pp->pa->ADPOSI_EJTRETFIRST >= pp->pa->ADPOSI_EJTRETEND))
				{	
					if(pp->md->FL_EJEF_SMOOTH == 1)
					{
						SET_MOV2(SetEjectPos_Bwd);
					}
					ACT_STEP_NEW(EJECTBWD);
				}
				else
					ACT_STEP_NEW(EJECTBWD3);

				if(CUS_ID == CS_BONA)
					SetPOBitDelay(pp->po->O_EJT_BWD, PC_ON, pp->mh->TMDLY_VLV_ONEJTB);	
			}
			break;
			
		case EJECTBWD:
		case EJECTBWD+1:
			if(pp->mh->ADPOSI_EJTRETEFF > 99)
				pp->mh->ADPOSI_EJTRETEFF = 10;
			if(pp->md->FL_EJTFIRSTSET == 1 && pp->ejectcnt == 1)
			{
				if(MoveLocationMeterControlProc(pp->pa->ADPOSI_EJTBFIRSTEND1, GetMeterPosByCh(pp->mh->EJT_METERCH)))
					ACT_STEP_NEW( EJECTBWD2);
			}
			else
			{
				if(MoveLocationMeterControlProc(pp->pa->ADPOSI_EJTRETFIRST + pp->mh->ADPOSI_EJTRETEFF, GetMeterPosByCh(pp->mh->EJT_METERCH)))
					ACT_STEP_NEW( EJECTBWD2);
			}

			CHK_SET_MOV(SetEjtBwdFast);
			break;

		case EJECTBWD2:
		case EJECTBWD2 + 1:
			if(pp->mh->ADPOSI_EJTRETEFF > 99)
				pp->mh->ADPOSI_EJTRETEFF = 10;
			if(pp->md->FL_EJTFIRSTSET == 1 && pp->ejectcnt == 1)
			{
				if(MoveLocationMeterControlProc(pp->pa->ADPOSI_EJTBFIRSTEND2 + pp->mh->ADPOSI_EJTRETEFF, GetMeterPosByCh(pp->mh->EJT_METERCH)))
					ACT_STEP_NEW( EJECTBWD3);
			}
			else
			{
				if(MoveLocationMeterControlProc(pp->pa->ADPOSI_EJTRETEND + pp->mh->ADPOSI_EJTRETEFF, GetMeterPosByCh(pp->mh->EJT_METERCH)))
					ACT_STEP_NEW( EJECTBWD3);
			}
			CHK_SET_MOV(SetEjtBwd);
			break;
			
		case EJECTBWD3:
		case EJECTBWD3 + 1:
			if(pp->pa->TM_EJT2NDROUND > 9999)
				pp->pa->TM_EJT2NDROUND = 0;
			if((ChkSysCfg(XF0_EJTTM) && MoveLocationTM10msControlProc(pp->pa->TM_EJT2NDROUND)) ||
			  (!ChkSysCfg(XF0_EJTTM) && MoveLocationNMControlProc(pp->pi->I_EJT_BWD)))											/*走行程开关*/
				ACT_STEP_NEW(END_STEP);

			if(pact->ftaskstat & INTF_CEC)
				CHK_SET_MOV(SetEjtAdjBwd);
			else
				CHK_SET_MOV(SetEjtBwd);
			break;

		case END_STEP:
			SET_OFF(SetEjtBwdOffDelay);

			if(pp->mh->TMDLY_EJT_BWDEND > 999)
				pp->mh->TMDLY_EJT_BWDEND = 0;

			ACT_STEP_NEW(END_STEP+ 1);
			ActSetMovTM(pact->ptsk, pp->mh->TMDLY_EJT_BWDEND);
			break;
		case END_STEP + 1:
			if(GetPOBit(pp->po->O_EJT_BWD) <= 1)
			{
				if(pp->mh->TMDLY_EJT_BWDEND > 999)
					pp->mh->TMDLY_EJT_BWDEND = 0;
	
				ACT_STEP_NEW(END_STEP+ 2);
				ActSetMovTM(pact->ptsk, pp->mh->TMDLY_EJT_BWDEND);
			}
			break;			
		case END_STEP + 2:
			if(pp->mh->TMDLY_EJT_BWDEND > 999)
				pp->mh->TMDLY_EJT_BWDEND = 0;
			if(ACT_STEP_100US > ((UI32)pp->mh->TMDLY_EJT_BWDEND)*100)
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
		return EjectBwdBackgrd(pp, pact);
}

void EjectVbrFwdStartInit(PEJECT pp, PACT pact)
{
	UI16 i=0;
	if(pp == NULL)
		return;           
                                                                                         
	for(i = 0;i<EJECT_SEG;i++)
	{
		pp->Eject_Flow[i] = 0;
		pp->Eject_Pos[i] = 0;
	}
	pp->End_ejectpos = 0;
	pp->Eject_Num = 0;

    pp->Eject_Flow[0] = pp->pa->DAFLOW_EJTADV;
	pp->Eject_Pos[0] = pp->pa->ADPOSI_EJTADVFASTEND;

	EjectFlowDot(pp);
	for (i = 0;i < EJECT_SEG;i++)
	{
		pp->Eject_Flow[i] = (UI32)pp->Eject_Flow[i]*(UI32)pp->pa->PER_DAFLOW_EJECT/100;
	}
	/*设置终点位置*/
	pp->End_ejectpos = pp->Eject_Pos[0];
	pp->Eject_Num = 1;
}

void SetEjectVbrPos_Fwd(PEJECT pp, PACT pact)
{
    int seg = 0;
    int PSramp[3] = {0};
    int PPramp[3] = {0};
    int Pspd[MAXNUMSEG] = {0};
    int Ppres[MAXNUMSEG] = {0};
    int Ppos[MAXNUMSEG] = {0};
	UI16 i = 0;
	UI16 endposbuf = 0xFFFF;	//初始化为最大值
	UI16 motor_max_spd = 0;
	int nowpos = 0;
	UI16 flow_limit = 0;
	UI16 mm = 0x01;
	
	motor_max_spd = GetMaxRPM(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_EJECT))-1);

	if(motor_max_spd < 500 || motor_max_spd > 9999)
	{
		motor_max_spd = 1800;
	}
	
	flow_limit = pp->mh->EJEFWD_FLOWS;
	if(flow_limit < 20)
		flow_limit = 20;

	//InitSetHydrStruCtrlData_K(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_EJECT)), motor_max_spd, pp->pclmp->pcore[3]->pa->CN_SCREWIN,flow_limit, pp->pclmp->pcore[3]->pa->CN_SCREWOUT);
    InitSetHydrStruCtrlData(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_EJECT)), motor_max_spd, 0, pp->mh->DIA_CYLINDER_E,pp->mh->DIA_OIL_NUM, GetMotorPump_Q(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_EJECT)),HYDR_PUMP(pp->hy->HYDR_PUMPS_EJECT)),pp->mh->EFFICIENCY_FACTOR_EJF,0,flow_limit, pp->md->PUMP_K_EJEFWD);

	nowpos = GetMeterPosByCh(pp->mh->EJT_METERCH);
	if(nowpos < 0)
	{
		/*防止负数影响判断*/
		nowpos = 0;
	}
	
	if((int)(pp->End_ejectpos) > nowpos)
	{
		endposbuf = pp->End_ejectpos;
	}
		
	seg = 1;
	for(i = 0;i<seg;i++)
	{
		if(pp->Eject_Flow[i] == 990)
		{
			pp->Eject_Flow[i] = 999;
		}
		if(pp->Eject_Pos[i] < endposbuf)
		{
			Pspd[i] = ActSetSpdToRPM(HYDR_PORT(pp->hy->HYDR_PORT_EJECT)-1,pp->Eject_Flow[i]);
			Ppos[i] = pp->Eject_Pos[i];
		}
		else
		{
			Pspd[i] = ActSetSpdToRPM(HYDR_PORT(pp->hy->HYDR_PORT_EJECT)-1,pp->Eject_Flow[i]);
			Ppos[i] = endposbuf;
			seg = i+1;
			break;
		}
	}


	PSramp[0] = pp->spdramp_ejtfwd[EJT_RAMP_FWD_START];	
	PSramp[1] = pp->spdramp_ejtfwd[EJT_RAMP_FWD_MID];
	PSramp[2] = pp->spdramp_ejtfwd[EJT_RAMP_FWD_END];

	if (ChkSysCfg(CO8_YL3))
	{
		mm = mm|DECMODE_SPD;
	}

    SetPosCtrlPara(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_EJECT)), DIRECFWD, pp->mh->EJT_METERCH, PSramp, Pspd, PPramp, Ppres, Ppos, seg, pp->md->PUMP_DEAD_EJEFWD, pp->mh->EJTF_HYDELAY,mm,0,0,0);
}



void EjectVbrBwdStartInit(PEJECT pp, PACT pact)
{
	UI16 i=0;
	if(pp == NULL)
		return;
	                                                                                         
	for(i = 0;i<EJECT_SEG;i++)
	{
		pp->Eject_Flow[i] = 0;
		pp->Eject_Pos[i] = 0;
	}
	pp->End_ejectpos = 0;
	pp->Eject_Num = 0;

    pp->Eject_Flow[0] = pp->pa->DAFLOW_EJTRETFIRST;
	pp->Eject_Pos[0] = pp->pa->ADPOSI_EJTADVFASTEND-pp->md->ADPOSI_EJTVIBRATEDISTANCE;

	EjectFlowDot(pp);
	for (i = 0;i < EJECT_SEG;i++)
	{
		pp->Eject_Flow[i] = (UI32)pp->Eject_Flow[i]*(UI32)pp->pa->PER_DAFLOW_EJECT/100;
	}

	//设置终点位置
	pp->End_ejectpos = pp->Eject_Pos[0];
	pp->Eject_Num = 1;
}

void SetEjectVbrPos_Bwd(PEJECT pp, PACT pact)
{
	int seg = 0;
	int PSramp[3] = {0};
	int PPramp[3] = {0};
	int Pspd[MAXNUMSEG] = {0};
	int Ppres[MAXNUMSEG] = {0};
	int Ppos[MAXNUMSEG] = {0};
	UI16 i = 0;
	UI16 endposbuf = 0x0;	//托模退时初始化为最小值
	UI16 motor_max_spd = 0;
	int nowpos = 0;
	UI16 flow_limit = 0;
	UI16 mm = 0x01;

	motor_max_spd = GetMaxRPM(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_EJECT))-1);
	if(motor_max_spd < 500 || motor_max_spd > 9999)
	{
		motor_max_spd = 1800;
	}
	flow_limit = pp->mh->EJEBWD_FLOWS;
	if(flow_limit < 20)
		flow_limit = 20;

	//InitSetHydrStruCtrlData_K(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_EJECT)), motor_max_spd, pp->pclmp->pcore[4]->pa->CN_SCREWOUT,flow_limit, pp->pclmp->pcore[5]->pa->CN_SCREWIN);

	InitSetHydrStruCtrlData(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_EJECT)), motor_max_spd, pp->mh->DIA_PISDIA_E, pp->mh->DIA_CYLINDER_E,pp->mh->DIA_OIL_NUM, GetMotorPump_Q(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_EJECT)),HYDR_PUMP(pp->hy->HYDR_PUMPS_EJECT)),pp->mh->EFFICIENCY_FACTOR_EJB,0,flow_limit, pp->md->PUMP_K_EJEBWD);
	
	nowpos = GetMeterPosByCh(pp->mh->EJT_METERCH);
	if(nowpos < 0)
	{
		/*防止负数影响判断*/
		nowpos = 0;
	}
	
	if((int)(pp->End_ejectpos) < nowpos)
	{
		endposbuf = pp->End_ejectpos;
	}
		
	seg = 1;
	for(i = 0;i<seg;i++)
	{
		if(pp->Eject_Flow[i] == 990)
		{
			pp->Eject_Flow[i] = 999;
		}
		if(pp->Eject_Pos[i] > endposbuf)
		{
			Pspd[i] = ActSetSpdToRPM(HYDR_PORT(pp->hy->HYDR_PORT_EJECT) - 1,pp->Eject_Flow[i]);
			Ppos[i] = pp->Eject_Pos[i];
		}
		else
		{
			Pspd[i] = ActSetSpdToRPM(HYDR_PORT(pp->hy->HYDR_PORT_EJECT) - 1,pp->Eject_Flow[i]);
			Ppos[i] = endposbuf;
			seg = i+1;
			break;
		}
	}

	PSramp[0] = pp->spdramp_ejtbwd[EJT_RAMP_BWD_START];	
	PSramp[1] = pp->spdramp_ejtbwd[EJT_RAMP_BWD_MID];
	PSramp[2] = pp->spdramp_ejtbwd[EJT_RAMP_BWD_END];
	
	if (ChkSysCfg(CO8_YL3))
	{
		mm = mm|DECMODE_SPD;
	}
	
	SetPosCtrlPara(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_EJECT)), DIRECBWD, pp->mh->EJT_METERCH, PSramp, Pspd, PPramp, Ppres, Ppos, seg, pp->md->PUMP_DEAD_EJEBWD, pp->mh->EJTB_HYDELAY,mm,0,0,0);
}


void SetEjtVbrBwd(PEJECT pp, PACT pact)
{
	UI16 hydelay;
	UI16 mode;
	
	if(pp->mh->EJTB_HYDELAY > 9999)
		pp->mh->EJTB_HYDELAY = 0;
	hydelay = pp->mh->EJTB_HYDELAY;
	EjectComVlv(pp);
	SetPOBit(pp->po->O_EJT_BWD, PC_ON);

	if(pp->md->FL_EJEF_SMOOTH == 1)
		mode = MC_CTR_MODE_POSI|MC_CTR_MODE_PRES;
	else
		mode = MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES;
	
	if(pp->md->FL_EJTVIBRATEMODE == 0)
	{
		if(GetMeterLenByCh(pp->mh->EJT_METERCH) > 0 && pp->pa->ADPOSI_EJTRETFIRST<GetMeterMax(pp->mh->EJT_METERCH) && pp->pa->ADPOSI_EJTRETFIRST > pp->pa->ADPOSI_EJTRETEND) 
			ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_EJTRETFIRST, pp->pa->DAFLOW_EJTRETFIRST, 0, 0, 0, 0, HYDRDELY,  
							 HYDR_PORT(pp->hy->HYDR_PORT_EJECT), pp->hy->HYDR_PUMPS_EJECT, pp->hy->HYDR_PUMPS_EJECT, pp->hy->SERVO_KP_EJECT, pp->hy->SERVO_KI_EJECT, 
							 0, TRUE, HYDR_PRI(pp->hy->HYDR_PORT_EJECT),pp->pa->PER_DAFLOW_EJECT);
		else
			ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_EJTRET, pp->pa->DAFLOW_EJTRET, 0, 0, 0, 0, HYDRDELY,  
							 HYDR_PORT(pp->hy->HYDR_PORT_EJECT), pp->hy->HYDR_PUMPS_EJECT, pp->hy->HYDR_PUMPS_EJECT, pp->hy->SERVO_KP_EJECT, pp->hy->SERVO_KI_EJECT, 
							 0, TRUE, HYDR_PRI(pp->hy->HYDR_PORT_EJECT),pp->pa->PER_DAFLOW_EJECT);
	}	
	else
	{
		ActSetHydr_SpdSR(pact, mode, pp->pa->DAPRES_EJTRETFIRST, pp->pa->DAFLOW_EJTRETFIRST, 0, 0, 0, 0, HYDRDELY,  
							 HYDR_PORT(pp->hy->HYDR_PORT_EJECT), pp->hy->HYDR_PUMPS_EJECT, pp->hy->HYDR_PUMPS_EJECT, pp->hy->SERVO_KP_EJECT, pp->hy->SERVO_KI_EJECT, 
							 0, TRUE, HYDR_PRI(pp->hy->HYDR_PORT_EJECT), pp->pa->PER_DAFLOW_EJECT);

	}
}

void SetEjtVbrFwd(PEJECT pp, PACT pact)
{
	UI16 hydelay;
	UI16 mode;
	
	if(pp->mh->EJTF_HYDELAY > 9999)
		pp->mh->EJTF_HYDELAY = 0;
	hydelay = pp->mh->EJTF_HYDELAY;
	EjectComVlv(pp);
	SetPOBit(pp->po->O_EJT_FWD, PC_ON);

	if(pp->md->FL_EJEF_SMOOTH == 1)
		mode = MC_CTR_MODE_POSI|MC_CTR_MODE_PRES;
	else
		mode = MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES;
	
	if(pp->md->FL_EJTVIBRATEMODE == 0)
	{
		ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_EJTADV, pp->pa->DAFLOW_EJTADV, 0, 0, 0, 0, HYDRDELY,  
							 HYDR_PORT(pp->hy->HYDR_PORT_EJECT), pp->hy->HYDR_PUMPS_EJECT, pp->hy->HYDR_PUMPS_EJECT, pp->hy->SERVO_KP_EJECT, pp->hy->SERVO_KI_EJECT, 
							 0, TRUE, HYDR_PRI(pp->hy->HYDR_PORT_EJECT), pp->pa->PER_DAFLOW_EJECT);
	}
	else
	{
		ActSetHydr_SpdSR(pact, mode, pp->pa->DAPRES_EJTADV, pp->pa->DAFLOW_EJTADV, 0, 0, 0, 0, HYDRDELY,  
							 HYDR_PORT(pp->hy->HYDR_PORT_EJECT), pp->hy->HYDR_PUMPS_EJECT, pp->hy->HYDR_PUMPS_EJECT, pp->hy->SERVO_KP_EJECT, pp->hy->SERVO_KI_EJECT, 
							 0, TRUE, HYDR_PRI(pp->hy->HYDR_PORT_EJECT), pp->pa->PER_DAFLOW_EJECT);
	}
}

ACT_RET VibrEjtFwd (PACT pact, UI32 para)
{

	PEJECT pp = (PEJECT)pact->part;
	ACT_RET	ret = ACT_OK;
 	UI16 tmp;
	
	if(pp == NULL)
		return ACT_ERR_ABORT;

	switch (pact->step) 
	{
		case INIT1_STEP:
			EjectComVlv(pp);
			SetPOBit(pp->po->O_EJT_FWD, PC_ON);
			EjectVbrFwdStartInit(pp,pact);
			if(ChkSysCfg(CO0_EJTDOT))
			{
				ACT_STEP_NEW(INIT2_STEP);
			}
			else
			{
				ACT_STEP_NEW(EJECTFWD);
			}
			break;

		case INIT2_STEP:
			if(pp->md->TM_DLYEJTADV > 9999)
				pp->md->TM_DLYEJTADV = 0;

			if (CUS_ID != CS_BONA)
			{
				if(pp->md->TM_DLYEJTADV<4)
					tmp = 4;
				else
					tmp = pp->md->TM_DLYEJTADV;
			}
			else
			{
				tmp = pp->md->TM_DLYEJTADV;
			}
			
			if((UI16)(ACT_RUN_MS/10) >=tmp)
			{
				ACT_STEP_NEW(EJECTFWD);
			}
			break;
		
		case EJECTFWD:
			if((pp->md->FL_EJEF_SMOOTH == 1) && (pp->md->FL_EJTVIBRATEMODE==1))
			{
				SET_MOV2(SetEjectVbrPos_Fwd);
			}
		case EJECTFWD + 1:
			if((pp->md->FL_EJTVIBRATEMODE==1 && GetMeterLenByCh(pp->mh->EJT_METERCH) > 0 && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->EJT_METERCH), pp->pa->ADPOSI_EJTADVFASTEND)) ||
			  ((pp->md->FL_EJTVIBRATEMODE!=1 || GetMeterLenByCh(pp->mh->EJT_METERCH) <= 0) &&  MoveLocationTM10msControlProc(pp->md->TM_EJTVIBRATE))) 
			{
			    if(GetMeterLenByCh(pp->mh->EJT_METERCH) > 0)
					ACT_STEP_NEW(EJECTFWD2);
				else
					ACT_STEP_NEW(END_STEP);
			}

			CHK_SET_MOV(SetEjtVbrFwd);
			break;			
		case EJECTFWD2:
		case EJECTFWD2 + 1:
			if(MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->EJT_METERCH), pp->pa->ADPOSI_EJTADVFASTEND))
				ACT_STEP_NEW(END_STEP);

			CHK_SET_MOV(NULL);
			break;
			
		case END_STEP:
	 		SET_OFF(SetEjtFwdOff);
			ret = ACT_END;
			break;
			
		default:
			ret = ACT_END;
			break;
	}

	if(ret == ACT_END || ret == ACT_ERR_ABORT)
		return ret;
	else
		return EjectFwdBackgrd(pp, pact);
}


ACT_RET VibrEjtBwd(PACT pact, UI32 para)
{
	PEJECT pp = (PEJECT)pact->part;
	ACT_RET	ret = ACT_OK;
	UI16 tmp;
        
	if(pp == NULL)
		return ACT_ERR_ABORT;

	switch (pact->step) 
	{
		case INIT1_STEP:
			EjectComVlv(pp);
			SetPOBit(pp->po->O_EJT_BWD, PC_ON);
			EjectVbrBwdStartInit(pp,pact);
			if(ChkSysCfg(CO0_EJTDOT))
			{
				ACT_STEP_NEW(INIT2_STEP);
			}
			else
			{
				ACT_STEP_NEW(EJECTBWD);
			}
			break;

		case INIT2_STEP:
			if(pp->md->TM_DLYEJTRET > 9999)
				pp->md->TM_DLYEJTRET = 0;

			if (CUS_ID != CS_BONA)
			{
				if(pp->md->TM_DLYEJTRET<6)
					tmp = 6;
				else
					tmp = pp->md->TM_DLYEJTRET;
			}
			else
			{
				tmp = pp->md->TM_DLYEJTRET;
			}
			
			if((UI16)(ACT_RUN_MS/10) >=tmp)
			{
				ACT_STEP_NEW(EJECTBWD);
			}
			break;

		case EJECTBWD:
			if((pp->md->FL_EJEF_SMOOTH == 1) && (pp->md->FL_EJTVIBRATEMODE==1))
			{
				SET_MOV2(SetEjectVbrPos_Bwd);
			}
		case EJECTBWD+1:
			if(pp->md->FL_EJTVIBRATEMODE == 1 &&GetMeterLenByCh(pp->mh->EJT_METERCH) > 0)
	        {
				if(MoveLocationMeterControlProc(pp->pa->ADPOSI_EJTADVFASTEND-pp->md->ADPOSI_EJTVIBRATEDISTANCE, GetMeterPosByCh(pp->mh->EJT_METERCH)))
					ACT_STEP_NEW(END_STEP);
			}
			else
			{
				if(MoveLocationTM10msControlProc(pp->md->TM_EJTVIBRATE))
					ACT_STEP_NEW(END_STEP);
			} 

			CHK_SET_MOV(SetEjtVbrBwd);
			break;

		case END_STEP:
			SET_OFF(SetEjtBwdOff);
			ret = ACT_END;
			break;
		default:
			ret = ACT_END;                 
	}

	if(ret == ACT_END || ret == ACT_ERR_ABORT)
		return ret;
	else
		return EjectBwdBackgrd(pp, pact);
}

ACT_RET AdjEjtFwdPreChk(PEJECT pp, PACT pact)
{
	int i;

	if(ClampOpenReachErrChk(pp->pclmp))
		return ACT_ERR_ABORT;
	
	for(i=0;i<CLAMP_MAX_CORE;i++)
	{
		if(pp->pclmp->pcore[i] != NULL)
		{
			if(pp->pclmp->pcore[i]->md->CORE_LIMIT_CHECK == 1 && CoreLimitSwitchErrCheck(pp->pclmp->pcore[i])) //调模托模进动作时检查中子行程开关
				return ACT_ERR_ABORT;
		}
	}

	return ACT_OK;
}

ACT_RET AdjEjtFwd(PACT pact, UI32 para)
{
	PEJECT pp = (PEJECT)pact->part;
	ACT_RET	ret = ACT_OK;
        
	if(!ChkOperMode(OM_ADJU) && (para != 1))
		return ACT_ERR_ABORT;

	if(pp == NULL)
		return ACT_ERR_ABORT;

	if(ActIODelayLimit(pp->po->O_EJT_FWD, TRUE))
	{
	    return ACT_ERR_ABORT;
	}
	
	switch (pact->step)
	{
		case INIT1_STEP:
			if((ret = AdjEjtFwdPreChk(pp, pact)) != ACT_OK)
				return ret;
			else
				ACT_STEP_NEW(EJECTFWD);
			break;
			
		case EJECTFWD:
		case EJECTFWD + 1:
				if(!ChkSysCfg(XF5_EJENCTL))
				{
					int tmp;
					tmp = GetMeterLenByCh(pp->mh->EJT_METERCH);
					if((tmp > 0 && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->EJT_METERCH), pp->pa->ADPOSI_EJTADVFASTEND)) ||
					  (tmp <= 0 && MoveLocationNMControlProc(pp->pi->I_EJT_FWD)))
					  ACT_STEP_NEW(END_STEP);
				}

			CHK_SET_MOV(SetEjtAdjFwd);
			break;
				
		case END_STEP:
			SET_OFF(SetEjtFwdOff);
			ActSetTM(pact->ptsk,0);
			ret = ACT_END;
			break;
			
		default:
			ret = ACT_END;
			break;
			
	}
	if(ret == ACT_END || ret == ACT_ERR_ABORT)
		return ret;
	else
		return EjectFwdBackgrd(pp, pact);
}

ACT_RET AdjEjtBwd(PACT pact, UI32 para)
{
	PEJECT pp = (PEJECT)pact->part;
	ACT_RET	ret = ACT_OK;
        
	if(!ChkOperMode(OM_ADJU) && para != 1)
		return ACT_ERR_ABORT;

	if(pp == NULL)
		return ACT_ERR_ABORT;

	if(ActIODelayLimit(pp->po->O_EJT_BWD, TRUE))
	{
	    return ACT_ERR_ABORT;
	}
	
	switch (pact->step)
	{
		case INIT1_STEP:
			ACT_STEP_NEW(EJECTBWD);
			break;
			
		case EJECTBWD:
		case EJECTBWD + 1:			
			if(!ChkSysCfg(XF5_EJENCTL))
			{
				int tmp;
				tmp = GetMeterLenByCh(pp->mh->EJT_METERCH);
				if((tmp > 0 && MoveLocationMeterControlProc(pp->pa->ADPOSI_EJTRETEND, GetMeterPosByCh(pp->mh->EJT_METERCH))) ||
				  (tmp <= 0 && MoveLocationNMControlProc(pp->pi->I_EJT_BWD)))
				  ACT_STEP_NEW(END_STEP);
			}	

			CHK_SET_MOV(SetEjtAdjBwd);
			break;

		case END_STEP:
			SET_OFF(SetEjtBwdOff);
			ret = ACT_END;
			
		default:
			ret = ACT_END;
	}

	if(ret == ACT_END || ret == ACT_ERR_ABORT)
		return ret;
	else
		return EjectBwdBackgrd(pp, pact);	
}


/****射出保压结束后的托模进退，用于切料头功能************/
void SetINJEjtFwd(PEJECT pp, PACT pact)
{
	UI16 hydelay;
	UI16 mode;
	
	hydelay = pp->mh->EJTB_HYDELAY;
	EjectComVlv(pp);	
	SetPOBit(pp->po->O_EJT_FWD, PC_ON);
	
	mode = MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES;
	
	ActSetHydr(pact, mode, pp->pa->DAPRES_INJEJT, pp->pa->DAFLOW_INJEJT, 0, 0, pp->mh->DAPRES_EJTRAMP, pp->mh->DAFLOW_EJTRAMP, HYDRDELY, 
						 HYDR_PORT(pp->hy->HYDR_PORT_EJECT),  pp->hy->HYDR_PUMPS_EJECT, pp->hy->HYDR_PUMPS_EJECT, pp->hy->SERVO_KP_EJECT, pp->hy->SERVO_KI_EJECT, 0,
						 TRUE, HYDR_PRI(pp->hy->HYDR_PORT_EJECT));
}

void SetINJEjtBwd(PEJECT pp, PACT pact)
{
	UI16 hydelay;
	UI16 mode;
	
	hydelay = pp->mh->EJTB_HYDELAY;
	EjectComVlv(pp);
	SetPOBit(pp->po->O_EJT_BWD, PC_ON);
	
	mode = MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES;

	ActSetHydr(pact, mode, pp->pa->DAPRES_INJEJTBWD, pp->pa->DAFLOW_INJEJTBWD, 0, 0, pp->mh->DAPRES_EJTRAMP, pp->mh->DAFLOW_EJTRAMP, HYDRDELY,  
						 HYDR_PORT(pp->hy->HYDR_PORT_EJECT),  pp->hy->HYDR_PUMPS_EJECT, pp->hy->HYDR_PUMPS_EJECT, pp->hy->SERVO_KP_EJECT, pp->hy->SERVO_KI_EJECT, 
						 0, (!(pact->ftaskstat & INTF_CEC)), HYDR_PRI(pp->hy->HYDR_PORT_EJECT));
}


ACT_RET InjEjtFwd(PACT pact,UI32 para)
{
	PEJECT pp = (PEJECT)pact->part;
	ACT_RET	ret = ACT_OK;
	int tmp;


	if(pp == NULL)
		return ACT_ERR_ABORT;
	
	switch (pact->step)
	{
		case INIT1_STEP:
			pp->INJEJECT_TMDLY = Get10MsTick();
			ACT_STEP_NEW(INIT1_STEP+1);
			break;
			
		case INIT1_STEP+1:
            if(MoveLocationTM10msControlProc(pp->md->TM_INJEJECTDLY))
				ACT_STEP_NEW(EJECTFWD);	
			break;
			
		case EJECTFWD:
		case EJECTFWD + 1:			
			if(ChkSysCfg(CO43_IINJ))
			{
				tmp = GetMeterLenByCh(pp->mh->EJT_METERCH);
				if((tmp > 0 && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->EJT_METERCH), pp->pa->ADPOSI_INJEJT)) ||
				(tmp <= 0 && MoveLocationNMControlProc(pp->pi->I_EJT_FWD)))
				{
					ACT_STEP_NEW(EJECTFWD + 2);
				}
			}
			else
			{
				if(MoveLocationTM10msControlProc(pp->pa->TM_CUTFWD))
				 	ACT_STEP_NEW(EJECTFWD + 2);
			}

			CHK_SET_MOV(SetINJEjtFwd);
			break;
			
		case EJECTFWD + 2:
			pp->INJEJECT_TM = Get10MsTick();
			ACT_STEP_NEW(EJECTFWD + 3);
			break;
			
		case EJECTFWD + 3:
			if((UI16)(Get10MsTick()-pp->INJEJECT_TM) >= pp->md->TM_INJEJECT)
				ACT_STEP_NEW(END_STEP);
			break;

		case END_STEP:
			SET_OFF(SetEjtFwdOff);
			ActSetTM(pact->ptsk,0);
			ret = ACT_END;
			break;
			
		default:
			ret = ACT_END;
			break;
			
	}
	if(ret == ACT_END || ret == ACT_ERR_ABORT)
		return ret;
	else
		return EjectFwdBackgrd(pp, pact);
}

ACT_RET InjEjtBwd(PACT pact, UI32 para)
{
	PEJECT pp = (PEJECT)pact->part;
	ACT_RET	ret = ACT_OK;
	int tmp;

	if(pp == NULL)
		return ACT_ERR_ABORT;
	
	switch (pact->step)
	{
        case INIT1_STEP:
			ACT_STEP_NEW(INIT1_STEP+1);
			break;
			
		case INIT1_STEP+1:
            if(MoveLocationTM10msControlProc(pp->pa->TM_INJEJECTBWDDLY))
				ACT_STEP_NEW(EJECTBWD);	
			break;
			
		case EJECTBWD:
		case EJECTBWD + 1:			
			if(ChkSysCfg(CO43_IINJ))
			{
				tmp = GetMeterLenByCh(pp->mh->EJT_METERCH);
				if((tmp > 0 && MoveLocationMeterControlProc(pp->pa->ADPOSI_EJTRETEND+pp->mh->ADPOSI_EJTRETEFF, GetMeterPosByCh(pp->mh->EJT_METERCH))) ||
				(tmp <= 0 && MoveLocationNMControlProc(pp->pi->I_EJT_BWD)))
				{
					ACT_STEP_NEW(END_STEP);
				}
			}
			else
			{
				if(MoveLocationTM10msControlProc(pp->pa->TM_CUTBWD))
					ACT_STEP_NEW(END_STEP);
			}

			CHK_SET_MOV(SetINJEjtBwd);
			break;

		case END_STEP:
			SET_OFF(SetEjtBwdOff);
			ret = ACT_END;
			
		default:
			ret = ACT_END;
	}

	if(ret == ACT_END || ret == ACT_ERR_ABORT)
		return ret;
	else
		return EjectBwdBackgrd(pp, pact);	
}


/*脱模动作调度函数*/
ACT_RET EjectProc (PACT pact, UI32 para)
{
	PMACHINE pm = &m_machine;
	PEJECT pp = (PEJECT)pact->part;
	ACT_RET	ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;

	switch(pact->step)
	{
		case INIT1_STEP:			
			pp->st->TM_EJECT_RT = 0;
			pp->st->POS_EJEFWDMAX = 0;
			pp->ejectcnt = 0;	
			pp->TM_start = GetMsTick();
			ACT_START();

			if (para == 0x2118)
			{
				ACT_STEP_NEW(STAGE_1_STEP);
			}
            else if(para == 0x2119)
			{
				ACT_STEP_NEW(STAGE_2_STEP);
			}
			else
			{
				if(ChkInjectCushErr()&&ChkSysCfg(CO1_CPSR)&&ChkOperMode(OM_AUTO)) 		  //通用射出监控失败停在顶针前
				{	
					CyclePDInfoSend(pm);
					return ACT_ERR_ABORT;
				}
				ACT_STEP_NEW(INIT2_STEP);
			}			
			break;

		case INIT2_STEP:
			if(!ChkSysCfg(XF5_SBTPUMPS) && (pp->md->FL_INJEJECT == 0))
			{
				if((pp->md->FL_EJTCTRL > 0) && (EjectBwdErrChk(pp,FALSE)) && ((pp->md->FL_INJEJECT == 0)||(CUS_ID != CS_TONGYONG)))
				{
					ForkAct(pp->act + OS_EJTB - 1, SUSPEND_FORK, pact->ptsk, 0, 0);
				}
			}
			ACT_STEP_NEW(INIT3_STEP);
			break;

		case INIT3_STEP:
			if ((pp->md->FL_EJTCTRL== 2) && (pp->ejectcnt > 0))
				ForkAct(pp->act + OS_VEJTF - 1, SUSPEND_FORK, pact->ptsk, 0, 0);
			else
				ForkAct(pp->act + OS_EJTF - 1, SUSPEND_FORK, pact->ptsk, 0, 0);
			ACT_STEP_NEW(INIT4_STEP);
			break;

		case INIT4_STEP:
			if(pp->md->FL_EJTCTRL != 0)
			{
				if((pp->md->FL_ejectCnt == 1)&& (GetOperMode()==OM_MANUAL))
				{
			 		ACT_STEP_NEW(END_STEP);
			 	}
				else
				{

					/*震动模式，最后一次做完整的脱模退动作*/
					 if(pp->md->FL_EJTCTRL== 2)
					 {
						if((pp->ejectcnt+1) < (pp->md->CN_EJT))		
							ForkAct(pp->act + OS_VEJTB - 1, SUSPEND_FORK, pact->ptsk, 0, 0);
						else
							ForkAct(pp->act + OS_EJTB - 1, SUSPEND_FORK, pact->ptsk, 0, 0);
					 }
					 else
					 {				 	
					   	ForkAct(pp->act + OS_EJTB - 1, SUSPEND_FORK, pact->ptsk, 0, 0);
					 }
				}
				 
				pp->ejectcnt++;
				if((pp->md->FL_ejectCnt == 1) && (GetOperMode()==OM_MANUAL))
				{
					ACT_STEP_NEW(END_STEP);
				}
				else if(pp->ejectcnt < pp->md->CN_EJT) 
					ACT_STEP_NEW(INIT3_STEP);
				else
					ACT_STEP_NEW(END_STEP);
			}
			else
				ACT_STEP_NEW(END_STEP);
			break;

		//切料头动作调度START
		case STAGE_1_STEP:
			ForkAct(pp->act + OS_CUTHEADF- 1, SUSPEND_FORK, pact->ptsk, 0, 0);
			if(ChkSysCfg(CO43_IINJ))
			{
				if(pp->md->FL_INJEJECTBWD == 1)
					ACT_STEP_NEW(STAGE_2_STEP);
				else 
					ACT_STEP_NEW(END_STEP + 1);
			}
			else
				ACT_STEP_NEW(STAGE_2_STEP);
			break;
		case STAGE_2_STEP:
			if (pp->md->FL_INJEJECT)
			{
				ForkAct(pp->act + OS_CUTHEADB- 1, SUSPEND_FORK, pact->ptsk, 0, 0);
			}	
			ACT_STEP_NEW(END_STEP + 1);
			break;
		//切料头动作调度END
			
		case END_STEP:
			pp->st->TM_EJECT = (ACT_RUN_MS/10);			
			ClrRunFlag(INIF_AUTO1);				
			EjectCmd( pp,CMD_QC_PARA,0);
			ret = ACT_END;
			break;

		case END_STEP + 1://切料头调度结束
			ret = ACT_END;
			break;
			
		default:
			ret = ACT_END;
			break;
	}
	
	return ret;
}

void EjectSetup(PEJECT pp)
{	
	if(pp != NULL)
	{
		pp->pclmp = (PCLAMP)GetMachineSubPart(CLAMP_ID, 1);
		pp->ejectcnt = 0;
	}	
}

BOOL InitEjectPart(PEJECT pp, UI8 sn)
{
	static const SUB_INIT_INFO sub_init[]  = EJECT_INIT_INFO;
	
	static const ACT_INIT_INFO act_init[] = 
	{
		{OS_EJT, OS_EJT, ACT_CTRB, EjectProc, NULL},
		{OS_EJTF, OS_EJTF, ACT_NTYPE, EjectFwd, SetEjtFwdOff},
	  	{OS_EJTB, OS_EJTB, ACT_NTYPE, EjectBwd,SetEjtBwdOff},
	   	{OS_AEJTF, OS_EJTF, ACT_NTYPE,AdjEjtFwd, SetEjtFwdOff},
	   	{OS_AEJTB, OS_EJTB, ACT_NTYPE,AdjEjtBwd, SetEjtBwdOff},
	   	{OS_VEJTF, OS_EJTF, ACT_NTYPE,VibrEjtFwd, SetEjtFwdOff},
	   	{OS_VEJTB, OS_EJTB, ACT_NTYPE,VibrEjtBwd, SetEjtBwdOff},
	   	{OS_CUTHEADF, OS_EJTF, ACT_NTYPE,InjEjtFwd, SetEjtFwdOff},
		{OS_CUTHEADB, OS_EJTB, ACT_NTYPE,InjEjtBwd, SetEjtBwdOff}
	};
									     
	if(!INIT_PART(pp, EJECT_ID, sn, sub_init))
		return FALSE;

	if(!INIT_ACT(pp, act_init))
		return FALSE;

	pp->mh->EJT_METERCH = DEF_EJECT_METER_CH;
	
	EjectSetup(pp);
	return TRUE;
}

UI32 EjectCmd(PEJECT pp, UI32 cmd, UI32 para)
{
	UI32 ret = TRUE;
	PMACHINE pm = &m_machine;	
	UI16 offset = 0;
	UI16 len = 0;
	if(pp == NULL)
		return FALSE;

	switch(GET_CMD(cmd))
	{
		case CMD_MANUAL_INIT:
			pp->ejectcnt = 0;
			break;
		case CMD_SETUP:
			EjectSetup(pp);
			break;
		case CMD_QC_PARA:
			if(ChkOperMode(OM_AUTO) && (pm->md->Qc_Fun==1))
			{
				if(pp->md->Qc_Mode_EjtTm== 1)  //射出时间QC数据
				{
					if(pm->st->Qc_Opn_Cnt == 0)
					{
						pp->qc_avg_ejttm = pp->st->TM_EJECT;
						pp->st->Qc_Min_EjtTm = pp->st->TM_EJECT;
						pp->st->Qc_Max_EjtTm = pp->st->TM_EJECT;
						pp->st->Qc_Unq_Cnt_EjtTm = 0;
					}
					else
					{
						pp->qc_avg_ejttm = pp->qc_avg_ejttm+pp->st->TM_EJECT;
						pp->st->Qc_Min_EjtTm=  (pp->st->Qc_Min_EjtTm>pp->st->TM_EJECT)?pp->st->TM_EJECT: pp->st->Qc_Min_EjtTm;
						pp->st->Qc_Max_EjtTm=  (pp->st->Qc_Max_EjtTm<pp->st->TM_EJECT)?pp->st->TM_EJECT: pp->st->Qc_Max_EjtTm;
					}
					if(((pp->st->TM_EJECT>(pp->md->Qc_TmSet_EjtTm+ pp->md->Qc_DeviationSet_EjtTm))||
					(pp->st->TM_EJECT<(pp->md->Qc_TmSet_EjtTm - pp->md->Qc_DeviationSet_EjtTm)))&&((pm->st->Qc_Opn_Cnt+1) <=pm->md->Qc_OpnNum))
			 			pp->st->Qc_Unq_Cnt_EjtTm++;						
				}
			}
			 break;
		case CMD_DATA_RX:
			offset = (para >> 16 & 0xffff);
            len = (para & 0xffff);
				
			if((GET_CMD_PARA(cmd) & 0xff) == MD_ID)
			{
				if(len > 0 
				&& (GET_IND(PEJECT_MOLDSET, EJTB_PRES_STOPOFF) >= offset && GET_IND(PEJECT_MOLDSET, EJEF_MAX_SPD) <= (offset + len - 1)))
				{
					//修改斜率相关参数时，对应缓存进行计算
					InitEjectRamp(pp);
				}
			}
			else if ((GET_CMD_PARA(cmd) & 0xff) == MH_ID)
			{				
				if(len > 0 
				&& (GET_IND(PEJECT_MACHSET, DAFLOW_EJTOFFRAMP) >= offset && GET_IND(PEJECT_MACHSET, DAPRES_EJTRAMP) <= (offset + len - 1)))
				{
					//修改斜率相关参数时，对应缓存进行计算
					InitEjectRamp(pp);
				}
			}
			else if ((GET_CMD_PARA(cmd) & 0xff) == PA_ID)
			{
				pp->pa->PER_DAFLOW_EJECT = (pp->pa->PER_DAFLOW_EJECT > 150||pp->pa->PER_DAFLOW_EJECT == 0)?100:pp->pa->PER_DAFLOW_EJECT;
			}
			break;
		default:
			ret = FALSE;
			break;
	}

	return ret;
}

