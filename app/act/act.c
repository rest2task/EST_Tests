/******************************************************************************
  文 件 名   : act.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 动作的公用函数
  函数列表   :
              ActCalMovST
              ActChkAndSetMovM
              ActClsHydr
              ActErrorAbort
              ActMov100MsChk
              ActMov10MsChk
              ActMovMsChk
              ActRunEnd
              ActRunHold
              ActRunLoop
              ActRunReactive
              ActRunStart
              ActRunStop
              ActSetHydr
              ActSetMov
              ActSetMovTM
              ActSetTM
              ActStepNew
              GetHydrPort
              GetHydrPri
              InitAct
  修改历史   :
  1.日    期   : 2013年5月17日
    作    者   : ren chaohong
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "com.h"
#include "act.h"
#include "acttbl.h"
#include "printf.h"
#include "hydr.h"
#include "machine.h"
#include "monmmi.h"
#include "inject.h"
#include "Da.h"

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
extern UI16 g_actopen;
extern UI16 g_OilPortSelect;
extern CUS_PORT_LIST cus_port_menu[OILPORTTB_NUM];


/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
WORD ActPreOpenProc(PACT pact);
UI16 GetRealSlaveNum(void);

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
/*动作通过ActSetHydr传过的流量是否带一位小数点，0: 无小数点，1表示1位小数点*/
I16 flow_dot = 0;
I16 pres_dot = 0;

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

ACT_MOV_STATE ActCalMovST(UI16 step)
{
	ACT_MOV_STATE st;
	
	if(step < PRE_MOV_STEP)
		st = ACT_MOV_INIT;
	else if(step < STAGE_1_STEP)
		st = ACT_MOV_PRE;
	else if(step < END_STEP)
		st = ACT_MOVING;
	else
		st = ACT_MOV_OFF;

	return st;
}

void ActErrorAbort(PACT pact)
{
	if(pact != NULL)
	{
		pact->mov_st = ACT_MOV_ABORT;
		pact->step = END_STEP;
	}
}

ACT_RET ActRunLoop(PACT pact, UI32 para)
{
	ACT_RET 		ret = ACT_OK;
	ACT_MOV_STATE 	cur_st, next_st, last_st;

	if(pact != NULL && pact->act_func != NULL && pact->mov_st < ACT_MOV_END && pact->ptsk != NULL)
	{
		pact->step = pact->next_step;
		if(pact->step != PRE_MOV_STEP && pact->step != PRE_FIN_STEP)
		{
			last_st = (ACT_MOV_STATE)pact->mov_st;
			pact->mov_st = INVALD_MOV_ST;
			ret = (*pact->act_func)(pact, para);

			cur_st = ActCalMovST(pact->step);
			next_st = ActCalMovST(pact->next_step);

			if(ret == ACT_ERR_ABORT ||ret == ACT_INTF_OFF)
			{
				ActErrorAbort(pact);
			}
			else 
			{
				/*判断运动前状态*/
				if(cur_st == ACT_MOV_INIT && next_st > ACT_MOV_PRE && pact->mov_st != ACT_MOV_PRE && pact->next_step < END_STEP && pact->step < STAGE_1_STEP)
				{
					/*插入动作运动前虚拟步*/
					pact->next_step_sav = pact->next_step;
					pact->next_step = PRE_MOV_STEP;
				}
				else if(ret == ACT_END && (pact->step >  INIT1_STEP || last_st >= ACT_MOVING || pact->mov_st >= ACT_MOVING))
				{
					/*插入动作结束前虚拟步*/
					pact->next_step_sav = pact->next_step;
					pact->next_step = PRE_FIN_STEP;
					pact->mov_st = ACT_MOV_END_PRE;
					ret = ACT_OK;

					//手动动作，当动作运行结束时立刻上传一次状态数据
					if (pact->ptsk->pst->act_run_ms > 10 && ChkOperMode(OM_MANUAL))
					{
						SentTskState();
					}
				}
			}

			if(pact->mov_st == INVALD_MOV_ST)
				pact->mov_st = cur_st;
		}
		else if(pact->step == PRE_MOV_STEP)
		{
			/*虚拟步，用于动作控制和PLC监控*/
			pact->step = pact->next_step;
			pact->mov_st = ACT_MOV_PRE;
			pact->next_step = pact->next_step_sav;
			ret = ACT_OK;
		}
		else if(pact->step == PRE_FIN_STEP)
		{
			/*虚拟步，用于动作控制和PLC监控*/
			pact->step = pact->next_step;
			pact->mov_st = ACT_MOV_END;
			pact->next_step = pact->next_step_sav;
			ret = ACT_END;
		}
		return ret;
	}
	else
		return ACT_ERR_ABORT;
}

void ActRunStop(PACT pact)
{
	if(pact != NULL)
	{
		db_printf("ACT %04x stop\r\n", pact->id);
		switch(pact->ptsk->pst->act_ret)
		{
			case ACT_INTF_OFF:
				if(pact->stop_func != NULL && pact->part != NULL)
					(*pact->stop_func)(pact->part, pact);
				break;
			case ACT_ERR_ABORT:
				if(pact->stop_func_err != NULL && pact->part != NULL)
					(*pact->stop_func_err)(pact->part, pact);
				break;
			default:
				if(pact->stop_func_hold != NULL && pact->part != NULL)
					(*pact->stop_func_hold)(pact->part, pact);
				break;
		}

		if(pact->ptsk != NULL)
			ExPOClear(&pact->ptsk->last_e_po);
	}
}


void ActRunEnd(PACT pact)
{
	if(pact != NULL)
	{
		UID id;
		id = pact->id;
		if(pact->ptsk != NULL)
			ExPOClear(&(pact->ptsk->last_e_po));

		db_printf("ACT %04x End\r\n", pact->id);
		/*-----------------------------------------------\
		|合模曲线特殊处理								 |
		|合模与锁模绘制一条曲线							 |
		\-----------------------------------------------*/
		if(pact->ptsk->pst->act_ret == ACT_END)
		{
			if(pact->id != MK_ACT(CLAMP_ID, 1, OS_CLSM))
			{				
				if(pact->id == MK_ACT(CLAMP_ID, 1, OS_PRSM))
				{
					id = MK_ACT(CLAMP_ID, 1, OS_CLSM);
				}
				MMIMonActEndNotify(id);
			}			
		}
		else
		{
			if(pact->id == MK_ACT(CLAMP_ID, 1, OS_PRSM))
			{
				id = MK_ACT(CLAMP_ID, 1, OS_CLSM);
			}
			
			MMIMonActEndNotify(id);
		}
		SetActRunFlag((PPART)pact->part, GET_ACT_SN(pact->id), FALSE);
		pact->ptsk = NULL;
	}
}

void ActRunStart(PACT pact, PACT_TSK ptsk)
{
	if(pact != NULL && ptsk != NULL && ptsk->pst != NULL)
	{
		pact->ptsk = ptsk;
		pact->start_ms = GetMsTick();
		pact->step = 0;
		pact->next_step = 0;
		pact->step_100ms = 0;
		pact->step_100us = 0;
		pact->mov_st = ACT_MOV_INIT;
		pact->mov_m = 0;
		pact->next_step_sav = 0;
		pact->react_func = NULL;
		pact->react_func2 = NULL;
		pact->ftaskstat = 0;		
		pact->act_step_num = 0;	//初始化动作步数
		pact->spc_data = 0;
		pact->reactive_step = 0;
		SetActRunFlag((PPART)pact->part, GET_ACT_SN(pact->id), TRUE);
		db_printf("ACT %04x Start\r\n", pact->id);
	}
}

void ActRunHold(PACT pact)
{
	if(pact != NULL)
	{
		/*need to do*/
		ActRunStop(pact);
		SetActRunFlag((PPART)pact->part, GET_ACT_SN(pact->id), FALSE);
		db_printf("ACT %04x Suspend\r\n", pact->id);
		pact->ftaskstat &= (~INTF_CEC);
		//if(pact->part != NULL && pact->hold_func != NULL)
		//	(*pact->hold_func)(pact->part, pact);
	}
}

void ActRunReactive(PACT pact, BOOL bend)
{
	if(pact != NULL)
	{
		SetActRunFlag((PPART)pact->part, GET_ACT_SN(pact->id), TRUE);
		db_printf("ACT %04x Reactivet\r\n", pact->id);
		if(pact->part != NULL && ((pact->react_func != NULL)||(pact->react_func2!=NULL)) && pact->part != NULL && !bend && pact->next_step < END_STEP)
		{
			if(pact->react_func != NULL)
			{
				(*pact->react_func)(pact->part, pact);
			}
			if(pact->react_func2 != NULL)
			{
				(*pact->react_func2)(pact->part, pact);
			}
		}
		
		if (pact->reactive_step != 0)
		{
			pact->step = pact->reactive_step;			
			pact->next_step = pact->reactive_step;
		}
		
		pact->step_100us = Get100UsTick();
		pact->step_100ms = Get100MsTick();
	}
}

void ActStepNew(PACT pact, UI16 new_step)
{
	PACT_TSK ptsk;
	
	pact->step_100us = Get100UsTick();
	pact->step_100ms = Get100MsTick();
	pact->next_step = new_step;

	ptsk = pact->ptsk;
	if(MIN_STEP(new_step) == 0 &&  ptsk != NULL)
	{
		ptsk->pst->act_tm_n = 0;
		ptsk->act_set_start_ms = 0;
	}
}

void ActClsHydr(PACT pact, UI16 pramp, UI16 framp)
{
	extern HYDR g_hydrout;
	UI16 lop_chnal = 0;
	if(pact != NULL && pact->last_hydr_port != 0)
	{
		CloseHydrOut(pact->last_hydr_port, pact->last_hydr_pri, pramp, framp);
		g_hydrout.press_start = 1;

		if(ChkSysCfg(XF3_YWSTO))
		{
			CloseDaOut_Pres(pact->last_hydr_port,pact->last_hydr_pri,pramp,1);
			CloseDaOut_Flow(pact->last_hydr_port,pact->last_hydr_pri,framp,1);
		}
		if(ChkSysCfg(CO5_INJCHADA) && (!DAIsAdjust()))  
		{
			if(ChkSysCfg(XF2_SOECH))
				lop_chnal = 0x02;
			else
				lop_chnal = 0x01;

			CloseDaOut_Pres(lop_chnal,0,0,1);
		}
	}
}

void ActClsDa_Pres(PACT pact, UI16 port,UI16 pramp, WORD dire)	//模拟量输出关闭
{
	if(pact != NULL && port != 0)
	{
		CloseDaOut_Pres(port, pact->last_da_pri, pramp, dire);
	}
}

void ActClsDa_Flow(PACT pact, UI16 port,UI16 framp,WORD dire)	//模拟量输出关闭
{
	if(pact != NULL && port != 0)
	{
		CloseDaOut_Flow(port, pact->last_da_pri, framp,dire);
	}
}


void ActClsHydrNoPact(UI16 last_hydr_port,UI16 last_hydr_pri, UI16 pramp, UI16 framp)
{
	extern HYDR g_hydrout;
	UI16 lop_chnal = 0;
	if(last_hydr_port != 0)
	{
		CloseHydrOut(last_hydr_port, last_hydr_pri, pramp, framp);
		if(ChkSysCfg(XF3_YWSTO))
		{
			CloseDaOut_Pres(last_hydr_port, last_hydr_pri,pramp,1);
			CloseDaOut_Flow(last_hydr_port, last_hydr_pri,framp,1);
		}

		//任何动作 手动放开都切模拟量输出
		if(ChkSysCfg(CO5_INJCHADA) && (!DAIsAdjust()))  
		{
			if(ChkSysCfg(XF2_SOECH))
				lop_chnal = 0x02;
			else
				lop_chnal = 0x01;

			CloseDaOut_Pres(lop_chnal,0,0,1);
		}
		g_hydrout.press_start = 1;
	}
}


void ActSetHydr(PACT pact,UI16 mode, UI16 press, UI16 flow, UI16 pramp, UI16 framp, UI16 prampstart, UI16 frampstart, UI16 hydrdly, UI16 hydrport, 
					UI16 ServoPumpLmt, UI16 NormalPumpLmt, UI16 Servo_P, UI16 Servo_I, UI16 PresFlag, UI16 KiClear, UI16 Priority)
{
	extern HYDR g_hydrout;
	UI16 presshydr = 0;
	UI16 lop_chnal = 0;
	if(pact != NULL)
	{
	    GetSys()->st->PUMP_MAIN |= hydrport;
        
		if(pact->last_hydr_port != hydrport || pact->last_hydr_pri != Priority)
			ActClsHydr(pact, 0, 0);			//need to do 需要关闭斜率

		/*目前液压驱动流量使用1位小数点*/
		if(g_hydrout.mh->FL_PWRMATCHTBL == 0)
		{
		   ServoPumpLmt=1;
		   NormalPumpLmt=1;
		   hydrport = 1;
		}
		else
		{
			ServoPumpLmt = HYDR_PUMP(ServoPumpLmt);
			NormalPumpLmt = HYDR_PUMP(NormalPumpLmt);
		}

		if (!pres_dot)
		{
			presshydr = press*10;
		}
		else
		{
			presshydr = press;
		}

		if (ChkSysCfg(CO5_INJCHADA))
		{			
			if(ChkSysCfg(XF2_SOECH))
				lop_chnal = 0x02;
			else
				lop_chnal = 0x01;

			if (ActPreOpenProc(pact))
			{
				if (mode == (MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES))
				{
					mode = MC_CTR_MODE_SPEED;
				}		
				
				ActSet_Da_Pres(pact, MC_CTR_MODE_DA, press,0,0,0,lop_chnal,0x01, 0,1,0,1);
			}
			else if (!DAIsAdjust())
			{		
					ActSet_Da_Pres(pact, MC_CTR_MODE_DA, GetSysConfig()->DAPRES_SYSLMT, 0, 0,
							0,lop_chnal,0x01, 0,1,0,1);
			}

		}
		

        if(flow_dot)
        {
			/*已有小数点1位，不需要再放大*/
		    SetMoveUnitDataList(mode, presshydr, flow,pramp, framp, prampstart,frampstart, hydrdly, hydrport, ServoPumpLmt, NormalPumpLmt, Servo_P, Servo_I, PresFlag, KiClear, Priority,0);
        }
		else
		{
			if((flow == 99) || ((flow == 999))) //用于解决设定流量设定99，转速却有1% 的偏差问题,同时解决射出流量从一个小数点切换到没有小数点时，面板依旧发999导致的问题
				SetMoveUnitDataList(mode, presshydr, 999,pramp, framp, prampstart,frampstart, hydrdly, hydrport, ServoPumpLmt, NormalPumpLmt, Servo_P, Servo_I, PresFlag, KiClear, Priority,0);
			else
			/*没有小数点，面板传过来的数据需要放大10倍*/
            	SetMoveUnitDataList(mode, presshydr, flow*10,pramp, framp, prampstart,frampstart, hydrdly, hydrport, ServoPumpLmt, NormalPumpLmt, Servo_P, Servo_I, PresFlag, KiClear, Priority,0);
		}
		
		g_hydrout.press_start = 1;
		pact->last_hydr_port = hydrport;
		pact->last_hydr_pri = Priority;

		if(ChkSysCfg(XF3_YWSTO))
		{
			ActSet_Da_Pres(pact, MC_CTR_MODE_DA, press, pramp, prampstart,
					hydrdly,hydrport,NormalPumpLmt, PresFlag,KiClear,Priority,1);
			ActSet_Da_flow(pact, MC_CTR_MODE_DA, flow, framp, frampstart,
					hydrdly,hydrport,NormalPumpLmt, PresFlag,KiClear,Priority,1);
		}
		
	}
}

void ActSetHydr_SpdSR(PACT pact,UI16 mode, UI16 press, UI16 flow, UI16 pramp, UI16 framp, UI16 prampstart, UI16 frampstart, UI16 hydrdly, UI16 hydrport, 
					UI16 ServoPumpLmt, UI16 NormalPumpLmt, UI16 Servo_P, UI16 Servo_I, UI16 PresFlag, UI16 KiClear, UI16 Priority, UI16 SpdDraw)
{
	extern HYDR g_hydrout;
	UI16 lop_chnal = 0;
	UI16 presshydr = 0;
	
	if(pact != NULL)
	{
		GetSys()->st->PUMP_MAIN |= hydrport;
        
		if(pact->last_hydr_port != hydrport || pact->last_hydr_pri != Priority)
			ActClsHydr(pact, 0, 0);			//need to do 需要关闭斜率

		/*目前液压驱动流量使用1位小数点*/
		if(g_hydrout.mh->FL_PWRMATCHTBL == 0)
		{
		   ServoPumpLmt=1;
		   NormalPumpLmt=1;
		   hydrport = 1;
		}
		else
		{
			ServoPumpLmt = HYDR_PUMP(ServoPumpLmt);
			NormalPumpLmt = HYDR_PUMP(NormalPumpLmt);
		}

		if (!pres_dot)
		{
			presshydr = press*10;
		}
		else
		{
			presshydr = press;
		}
		
		if (ChkSysCfg(CO5_INJCHADA))
		{
			if(ChkSysCfg(XF2_SOECH))
				lop_chnal = 0x02;
			else
				lop_chnal = 0x01;
			
			if (ActPreOpenProc(pact))
			{
				if (mode == (MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES))
				{
					mode = MC_CTR_MODE_SPEED;
				}				
				
				ActSet_Da_Pres(pact, MC_CTR_MODE_DA, press,0,0,0,lop_chnal,0x01, 0,1,0,1);
			}
			else if (!DAIsAdjust())
			{		
				if (ChkSysCfg(XF4_OHNOM))
				{
					ActSet_Da_Pres(pact, MC_CTR_MODE_DA, 0, 0, 0,
						0,lop_chnal,0x01, 0,1,0,1);
				}
				else				
				{
					ActSet_Da_Pres(pact, MC_CTR_MODE_DA, GetSysConfig()->DAPRES_SYSLMT, 0, 0,
						0,lop_chnal,0x01, 0,1,0,1);
				}
			}

		}

        if(flow_dot)
        {
			/*已有小数点1位，不需要再放大*/
		    SetMoveUnitDataList(mode, presshydr, flow,pramp, framp, prampstart,frampstart, hydrdly, hydrport, ServoPumpLmt, NormalPumpLmt, Servo_P, Servo_I, PresFlag, KiClear, Priority,SpdDraw);
        }
		else
		{
			if((flow == 99) || ((flow == 999))) //用于解决设定流量设定99，转速却有1% 的偏差问题,同时解决射出流量从一个小数点切换到没有小数点时，面板依旧发999导致的问题
				SetMoveUnitDataList(mode, presshydr, 999,pramp, framp, prampstart,frampstart, hydrdly, hydrport, ServoPumpLmt, NormalPumpLmt, Servo_P, Servo_I, PresFlag, KiClear, Priority,SpdDraw);
			else
			/*没有小数点，面板传过来的数据需要放大10倍*/
            	SetMoveUnitDataList(mode, presshydr, flow*10,pramp, framp, prampstart,frampstart, hydrdly, hydrport, ServoPumpLmt, NormalPumpLmt, Servo_P, Servo_I, PresFlag, KiClear, Priority,SpdDraw);
		}
		
		g_hydrout.press_start = 1;
		pact->last_hydr_port = hydrport;
		pact->last_hydr_pri = Priority;

		if(ChkSysCfg(XF3_YWSTO))
		{
			ActSet_Da_Pres(pact, MC_CTR_MODE_DA, press, pramp, prampstart,
					hydrdly,hydrport,NormalPumpLmt, PresFlag,KiClear,Priority,1);
			ActSet_Da_flow(pact, MC_CTR_MODE_DA, flow, framp, frampstart,
					hydrdly,hydrport,NormalPumpLmt, PresFlag,KiClear,Priority,1);
		}
		
	}
}


void ActSet_Da_Pres(PACT pact,UI16 mode, UI16 press, UI16 pramp, UI16 prampstart, UI16 hydrdly, UI16 hydrport, 
					UI16 NormalPumpLmt, UI16 PresFlag, UI16 KiClear, UI16 Priority,UI16 Direct)
{
	extern HYDR g_hydrout;
	if(pact != NULL)
	{
		//目前液压驱动流量使用1位小数点
		if(g_hydrout.mh->FL_PWRMATCHTBL == 0)
		{
		   NormalPumpLmt=1;
		}
		else
		{
			NormalPumpLmt = HYDR_PUMP(NormalPumpLmt);
		}		
		
		SetMoveUnitData_Da_pres(mode, press, pramp, prampstart, hydrdly, hydrport, NormalPumpLmt, PresFlag, KiClear, Priority,Direct,0);
		
		pact->last_da_port = hydrport;
		pact->last_da_pri = Priority;
	}
}

void ActSet_Da_flow(PACT pact,UI16 mode, UI16 flow, UI16 framp, UI16 frampstart, UI16 hydrdly, UI16 hydrport, 
					UI16 NormalPumpLmt, UI16 PresFlag, UI16 KiClear, UI16 Priority,UI16 Direct)
{
	UI16 flow_set;
	
	extern HYDR g_hydrout;
	if(pact != NULL)
	{
		//目前液压驱动流量使用1位小数点
		if(g_hydrout.mh->FL_PWRMATCHTBL == 0)
		{
		   NormalPumpLmt=1;
		}
		else
		{
			NormalPumpLmt = HYDR_PUMP(NormalPumpLmt);
		}	
		
		if(flow_dot)
			flow_set = flow;
		else
		{
			if((flow == 99) || ((flow == 999)))
				flow_set = 999;
			else
				flow_set = flow * 10;
		}
		
		SetMoveUnitData_Da_flow(mode, flow_set,framp, frampstart, hydrdly, hydrport, NormalPumpLmt, PresFlag, KiClear, Priority,Direct,0);
		
		pact->last_da_port = hydrport;
		pact->last_da_pri = Priority;
	}
}


void ActSetMov(ACT_OP_FUNC pmov, PPART pp, PACT pact)
{
	if(pmov != NULL)
		(*pmov)(pp, pact);

	if(pact != NULL)
	{
		pact->ftaskstat |= INTF_CEC;
		//pact->ftaskstat &= (~INTF_STEPNEW);
		pact->react_func = (ACT_OP_FUNC)(pmov);
	}
}


void ActSetMov2(ACT_OP_FUNC pmov, PPART pp, PACT pact)
{
	if(pmov != NULL)
		(*pmov)(pp, pact);

	if(pact != NULL)
	{
		//pact->ftaskstat |= INTF_CEC;
		//pact->ftaskstat &= (~INTF_STEPNEW);
		pact->react_func2 = (ACT_OP_FUNC)(pmov);
	}
}


BOOL ActChkAndSetMovM(ACT_OP_FUNC pmov, PPART pp, PACT pact, UI16 m)
{
	UI16 step;

	step = pact->step;
	if(MIN_STEP(step) == 0 && step == pact->next_step)// && (pact->ftaskstat & INTF_STEPNEW))
	{
		if(pmov != NULL)
		{
			pact->mov_m = m;
			ActSetMov(pmov, pp, pact);
		}

		ActStepNew(pact, step + 1);
		return TRUE;
	}
	else
		return FALSE;
	
}


BOOL InitAct(PPART  pp, PACT pact,  const ACT_INIT_INFO* pinit_info, int num)
{
	int		i;
	UI8		sn;
	UI8		max_sn = 0;
	PACT 	pt;
	
	if(pact == NULL || pp == NULL)
		return FALSE;

	if(pinit_info == NULL || num == 0)
	{
		pp->act_num = 0;
		pp->pact_tbl = NULL;

		return TRUE;
	}

	max_sn = 0;
	for(i = 0; i < num; i++)
	{
		sn = pinit_info[i].act_sn;
		
		if(sn > MAX_ACT_SN || sn <= 0)
			return FALSE;

		if(sn > max_sn)
		{
			max_sn = sn;
		}
		
		pt = pact + sn - 1;
		memset(pt, 0, sizeof(*pt));
		pt->id = MK_ACT(GET_PART_TYPE(pp->id), GET_PART_SN(pp->id), sn);
		pt->gid = MK_ACT(GET_PART_TYPE(pp->id), GET_PART_SN(pp->id), pinit_info[i].g_act_sn); 
		pt->type = pinit_info[i].act_type;
		pt->act_func = (ACT_RUN_FUNC)(pinit_info[i].act_func);
		pt->stop_func = (ACT_OP_FUNC)(pinit_info[i].stop_func);
		//pt->hold_func = NULL;
		pt->react_func = NULL;
		pt->react_func2 = NULL;


		pt->cannotbeclear = FALSE;

		if(pt->stop_func == NULL)
		{
			if(pinit_info[i].stop_func_list == NULL)
			{
				pt->stop_func = NULL;
				pt->stop_func_err = NULL;
				pt->stop_func_hold = NULL;
			}
			else
			{
				pt->stop_func = (ACT_OP_FUNC)((*pinit_info[i].stop_func_list).stop_func);
				pt->stop_func_err = (ACT_OP_FUNC)((*pinit_info[i].stop_func_list).stop_func_err);					
				pt->stop_func_hold = (ACT_OP_FUNC)((*pinit_info[i].stop_func_list).stop_func_hold);
			}				
		}
		else
		{
			pt->stop_func_err = (ACT_OP_FUNC)(pinit_info[i].stop_func);					
			pt->stop_func_hold = (ACT_OP_FUNC)(pinit_info[i].stop_func);
		}
		
		if(pinit_info[i].cannotbeclear!=NULL && pinit_info[i].cannotbeclear == TRUE)
		{
			pt->cannotbeclear = pinit_info[i].cannotbeclear;
		}
		
		pt->part = pp;
	}

	pp->pact_tbl = pact;
	pp->act_num = max_sn;
	
	return TRUE;
}

int GetHydrPri(UI16 port, PACT pact)
{
	PACT_TSK ptsk;
	PHY_PRI_ROW prow = NULL;
	
	ptsk = pact->ptsk;
	if(pact != NULL && ptsk != NULL && ptsk->phydr != NULL)
	{
		/*非联动状态下是否不使用油路优先级表*/
		if(GetSysMold()->FL_HYTBLSEL == 0 || (!ChkOperMode(OM_MANUAL) && GetJointActSel() != 0))
			prow = GetActStepPortPri(pact, ptsk->phydr, ptsk->hydr_num); 
	}
	
	if(prow != NULL)
	{
		db_printf("Hydr pri, act %04x, pri: %04x\r\n", pact->id, prow->priority);
		return (prow->priority);		//使用油路优先级表中优先级
	}
	
	/*没有油路优先级表项，缺省优先级为0*/
	return 0;			
}

UI16 GetHydrPort_Ch(UI16 port)  //获取液压通道的最小编号
{
    UI16 i;
    
    if(port == 0)
        return 1;
    
    for (i = 0;i < g_pump_num;i++)
	{
		if ((port&(0x01<<i)) > 0)
		{
			return i+1;
		}
	}

    return 1;
}

UI16 GetHydrPort(UI16 port, PACT pact)
{
	PACT_TSK ptsk;
	PHY_PRI_ROW prow = NULL;
	UI16 i = 0;	
	UI16 port1 = 0;
	ptsk = pact->ptsk;

	if(GetSys()->md->FL_HYDY_WAY_SET > 0 && ((GetSys()->md->FL_HYDY_WAY_SET_MODE == 0) || (GetSys()->md->FL_HYDY_WAY_SET_MODE == 1 && (GetJointActSel2() > 0 || GetJointActSel() > 0))))
	{
		//运行内置油路表
		if (GetSys()->md->FL_HYDY_WAY_SET != 2)
		{
			if (g_OilPortSelect > 0)
			{
				for(i = 0;i < ARRAY_NUM(cus_port_menu); i++)
				{
					if (cus_port_menu[i].part_id == 0xFFFF)
					{
						break;
					}
					if(((pact->id>>8) == cus_port_menu[i].part_id) && ((pact->id&0x0F) == cus_port_menu[i].act_id) && (pact->spc_data == cus_port_menu[i].act_flag))
					{
						port1 = cus_port_menu[i].port;
					}
				}
			}
		}
		else
		{//运行配置表
			if(pact != NULL && ptsk != NULL && ptsk->phydr != NULL)
			{
				/*非联动状态下是否不使用油路优先级表*/
				if(GetSysMold()->FL_HYTBLSEL == 0 || (!ChkOperMode(OM_MANUAL) && GetJointActSel() != 0))
					prow = GetActStepPortPri(pact, ptsk->phydr, ptsk->hydr_num); 
			}

			if(prow != NULL)
			{
				db_printf("HYDR port %0x, pri %0x\r\n",  prow->port_bits, prow->priority);
				port1 = prow->port_bits;		//使用油路优先级表中油路端口配置
			}
		}

		//以最小编号的port为主油路
		if (port1 != 0)
		{
			for (i = 0;i < g_pump_num;i++)
			{
				if ((port1&(0x01<<i)) > 0)
				{
					return (0x01<<i);
				}
			}			
		}
	}	
	

	if (GetSys()->md->FL_HYDY_WAY_SET == 0)
	{
		if(pact != NULL && ptsk != NULL && ptsk->phydr != NULL)
		{
			/*非联动状态下是否不使用油路优先级表*/
			if(GetSysMold()->FL_HYTBLSEL == 0 || (!ChkOperMode(OM_MANUAL) && GetJointActSel() != 0))
				prow = GetActStepPortPri(pact, ptsk->phydr, ptsk->hydr_num); 
		}

		if(prow != NULL)
		{
			db_printf("HYDR port %0x, pri %0x\r\n",  prow->port_bits, prow->priority);
			return prow->port_bits;		//使用油路优先级表中油路端口配置
		}
	}	
	
	/*没有油路优先级表项，使用部件端口参数*/
	if((port == 0) || (port > 0x80))
		return 1;
	else
		return port;
}

UI16 GetHydrPump(UI16 pumps, PACT pact)
{
	UI16 i = 0;	
	UI16 port = 0;
	PHY_PRI_ROW prow = NULL;
	PACT_TSK ptsk = pact->ptsk;

	if(GetSys()->md->FL_HYDY_WAY_SET > 0 && ((GetSys()->md->FL_HYDY_WAY_SET_MODE == 0) || (GetSys()->md->FL_HYDY_WAY_SET_MODE == 1 && (GetJointActSel2() > 0 || GetJointActSel() > 0))))
	{
		//运行内置油路表
		if (GetSys()->md->FL_HYDY_WAY_SET != 2)
		{
			if (g_OilPortSelect > 0)
			{
				for(i = 0;i < ARRAY_NUM(cus_port_menu); i++)
				{
					if (cus_port_menu[i].part_id == 0xFFFF)
					{
						break;
					}

					if(((pact->id>>8) == cus_port_menu[i].part_id) && ((pact->id&0x0F) == cus_port_menu[i].act_id) && (pact->spc_data == cus_port_menu[i].act_flag))
					{
						port = cus_port_menu[i].pump;
						if (port == 0)
						{
							port = cus_port_menu[i].port;
						}
					}
				}
			}
		}
		else
		{
			if (pact != NULL && ptsk != NULL && ptsk->phydr != NULL)
			{
				/*非联动状态下是否不使用油路优先级表*/
				if (GetSysMold()->FL_HYTBLSEL == 0 || (!ChkOperMode(OM_MANUAL) && GetJointActSel() != 0))
				{
					prow = GetActStepPortPri(pact, ptsk->phydr, ptsk->hydr_num); 
				}

				if (prow != NULL)
				{
					port = prow->port_bits;
				}
			}
		}

		if (port != 0)
		{
			pumps = port;			
		}
	}	
	
	/*没有油路优先级表项，使用部件端口参数*/
	if(pumps == 0)
		return 0;
	else
		return pumps;
}


/*设置面板的动作显示时间 */
void ActSetTM(PACT_TSK ptsk, UI32 act_tm)
{
	if(ptsk != NULL && act_tm != 0)
	{
		ptsk->pst->act_tm_n = act_tm;
		ptsk->act_set_start_ms = 0;
	}
}

void ActSetMovTM(PACT_TSK ptsk, UI16 act_tm)
{
	if(ptsk != NULL && act_tm != 0)
	{
		ptsk->pst->act_tm_n = act_tm;
		ptsk->act_set_start_ms = GetMsTick();
	}
}

BOOL ActMov100UsChk(PACT_TSK ptsk, UI32 start_tm, UI32 chk_tm)
{
	if(chk_tm == 0 || (Get100UsTick() - start_tm) >= chk_tm)
		return TRUE;
	else 
	{
		//100ms单位
		if(ptsk->pst->act_tm_n == 0)
			ActSetMovTM(ptsk, chk_tm/100);

		return FALSE;
	}
}

BOOL ActMov100UsChk_INJ(PACT_TSK ptsk, UI32 start_tm, UI32 chk_tm)
{
	if(ChkSysCfg(CO1_INTM))//射出时间精确到0.001
	{
		if(chk_tm == 0 || (Get100UsTick() - start_tm) >= chk_tm*10)
			return TRUE;
		else 
		{
			//100ms单位
			if(ptsk->pst->act_tm_n == 0)
				ActSetMovTM(ptsk, chk_tm/10);

			return FALSE;
		}	
	}
	else
	{
		if(chk_tm == 0 || (Get100UsTick() - start_tm) >= chk_tm*100)
			return TRUE;
		else 
		{
			//100ms单位
			if(ptsk->pst->act_tm_n == 0)
				ActSetMovTM(ptsk, chk_tm);

			return FALSE;
		}	
	}
	
	
}

#if 0
BOOL ActMovMsChk(PACT_TSK ptsk, UI32 start_tm, UI32 chk_tm)
{
	if(chk_tm == 0 || (GetMsTick() - start_tm) >= chk_tm)
		return TRUE;
	else 
	{
		//100ms单位
		if(ptsk->pst->act_tm_n == 0)
			ActSetMovTM(ptsk, chk_tm/100);

		return FALSE;
	}
}

BOOL ActMov100MsChk(PACT_TSK ptsk, UI32 start_tm, UI32 chk_tm)
{
	if(chk_tm == 0 || (UI16)(Get100MsTick() - (UI16)start_tm) >= (UI16)chk_tm)
		return TRUE;
	else
	{
		if(ptsk->pst->act_tm_n == 0)
			ActSetMovTM(ptsk, chk_tm);

		return FALSE;
	}
}

BOOL ActMov10MsChk(PACT_TSK ptsk, UI32 start_tm, UI32 chk_tm)
{
	if(chk_tm == 0 || (UI16)(Get10MsTick() - (UI16)start_tm) >= (UI16)chk_tm)
		return TRUE;
	else
	{
		//100ms单位
		if(ptsk->pst->act_tm_n == 0)
			ActSetMovTM(ptsk, chk_tm/10);
		
		return FALSE;
	}
}
#endif

int ActSetFlowDot(int dot)
{
	int tmp;

	tmp = flow_dot;
	
	if(dot > 0)
		flow_dot = 1;
	else
		flow_dot = 0;
		
	return tmp;
}

int ActSetPresDot(int dot)
{
	int tmp;

	tmp = pres_dot;

	if(dot > 0)
		pres_dot = 1;
	else
		pres_dot = 0;
		
	return tmp;
}

