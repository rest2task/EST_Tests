/******************************************************************************
  文 件 名   : charge.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 螺杆部件储料动作函数
  函数列表   :
              AutoPurge
              AutoPurgeCharge
              ChargeAdj
              ChargeBackGrd
              ChargeBeforeInj
              ChargeOff
              ChargeOffVlv
              ChargePre
              ChargeProc
              ChargeTimeout
              ChargeVlv
              GetChargeRpm
              SetChargeAdjBwd
              SetChargeBack
              SetChargeBwdNum
  修改历史   :
  1.日    期   : 2013年5月17日
    作    者   : ren chaohong
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "actcom.h"
#include "inject.h"
#include "nozzle.h"
#include "clamp.h"
#include "chargpress.h"
#include "useriap.h"
#include "machapi.h"
#include "safedoor.h"

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
extern HYDR g_hydrout;

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
void	LoopHydrauic_sv(void);

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
#define BACKPRESS_PRI	3

BOOL ChargeAftInj(PINJECT pp)
{
	int tmp;
	tmp = GetMeterLenByCh(pp->mh->INJ_METERCH);

	if((tmp >= 0) && (GetMeterPosByCh(pp->mh->INJ_METERCH) < 50))
	{
		SET_PART_ERR(pp, ER0_CHBF);
		return TRUE;
	} 
	else
		return FALSE;
}


ACT_RET ChargePre(PINJECT pp)                         //储料条件判断
{	
	if(!InjTempChkPro(pp))
	{
		return ACT_ERR_ABORT;
	}

	return ACT_OK;
}


void OpenRelVlvProc()
{
	PINJECT pinj = (PINJECT)GetPartByID(MK_PART(INJECT_ID,1));
	
	if(pinj == NULL)
		return;
	
	if(pinj->relvlvopnflag > 0)
	{
		if (pinj->relvlvopnflag == 1)
		{
			if(!TestPOIsOn(pinj->po->O_CHG_RELOIL))
			{
				SetPOBit(pinj->po->O_CHG_RELOIL,PC_ON);
			}

			pinj->relvlvopnflag = 2;
		}		
		
		pinj->st->TM_RELVLVOPN = (UI16)(Get10MsTick() - pinj->Tm_relvlvopn);
			
		if(pinj->st->TM_RELVLVOPN > pinj->mh->TM_RELVLV)
		{
			if(TestPOIsOn(pinj->po->O_CHG_RELOIL))
			{
				SetPOBit(pinj->po->O_CHG_RELOIL,PC_OFF);				
			}	
			
			pinj->st->TM_RELVLVOPN = 0;
			pinj->relvlvopnflag = 0;
		}
	}
	else
	{
		pinj->Tm_relvlvopn = Get10MsTick();
	}
}


void ChargeVlv(PINJECT pp)                        		 //打开储料方向阀
{
	SetPOBit(GetSysPO(O_FST_SOL), PC_ON);		//快速阀  
	if(ChkSysCfg(CO5_YXBIG))
	{
	}
	else
	{
		if(pp->md->FL_CHGBACK==1)
			SetPOBit(pp->po->O_CHR_BAC, PC_ON);	 //储料背压阀
	}

	if(pp->md->FL_CHARGERELEASE == 1)
	{                             
		SetPOBit(pp->po->O_CHR_REL, PC_ON);		  //储料泄压阀
		SetPOBit(pp->po->O_CHG_SUC, PC_ON);	 //储料射退阀
	}  
	if(pp->md->FL_CHRGFAST == 1)
		SetPOBit(pp->po->O_CHR_FST, PC_ON);		 //储料快速阀

	if(ChkSysCfg(XF1_HTMHYD))                     			//海太双油路
	{
		SetPOBit(GetSysPO(O_HYD_VLV2), PC_ON);	 //双油路阀
		if(pp->md->FL_CHRGFAST == 1)
			SetPOBit(GetSysPO(O_HYD_COMB), PC_ON);			//并流阀
	}	

	if (ChkSysCfg(XF4_OILVL))
	{
		SetNozzleFwdPO(pp->pnzl, FALSE);
	}
	else
	{
		if(NozzleIsReach(pp->pnzl) || (ChkOperMode(OM_AUTO) && NozzleType(pp->pnzl) < NZ_AFT_INJ))
		SetNozzleFwdPO(pp->pnzl, TRUE); 		//非储料前座退开座进阀				 
	}	

	if(pp->mh->TMDLY_VLV_ONCHG > 500)
		pp->mh->TMDLY_VLV_ONCHG = 0;
	SetPOBitDelay(pp->po->O_CHR_SOL, PC_ON, pp->mh->TMDLY_VLV_ONCHG);		 //标准储料阀
	
	if(ChkSysCfg(CO43_CHARGE))
		SetPOBit(pp->po->O_INJ_SOL, PC_ON);		// 恩格 ，储料时打开射出泵
	SetPOBit(pp->po->O_PRE_MOLDENABLE, PC_ON);		//光塑，电储料时输出预塑使能 20230527dylong
}

UI16 Get_BackPort_Simulation(PINJECT pp)	//得到模拟量的背压通道
{
	UI16 i = 0,Back_port_Simulation = 0;
	for (i=0;i<4;i++)	//面板上DA画面中的背压通道设定值设定(1,2,3,4)时主机接收到的是(1,2,4,8)，这里是按位来发送的，此处需要更改下，否则设置3时，却跑到第四通道去了   20201014  yxh
	{
		if((pp->hy->HYDR_PORT_CHRGBACK >> i) & 1)
		{
			Back_port_Simulation = i + 1;
			break;
		}
	}

	return Back_port_Simulation;
}

BOOL Chg_Move_Check(PINJECT pp)
{
	if(pp->md->FL_CHGMOVE == 1 && MotorIsOn())		//马达开启同时功能选择使用时才检测
	{
		if(DIFF_10MS(pp->Chg_nomove_Tm) > pp->md->TM_CHGMOVE)
		{
			pp->Chg_nomove_Tm = Get10MsTick();
			if(MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->INJ_METERCH), (pp->Chg_nomove_Pos + pp->md->POS_CHGMOVE)))
			{
				if((pp->Chg_nomove_Pos +  pp->md->POS_CHGMOVE) < GetMeterMax(pp->mh->INJ_METERCH))
					pp->Chg_nomove_Pos = GetMeterPosByCh(pp->mh->INJ_METERCH);
			}
			else
			{
				SET_PART_ERR(pp, ER0_CHRG);
				return TRUE;		//储料电子尺连续一段时间运动太小，即认为卡住或者出现其他异常
			}
		}
	}
	return FALSE;
}


void SetChargeBack(PINJECT pp, PACT pact,WORD backpres)			// 储料背压 09-11-04
{	
	/*如果比例背压为第1路，且为普通机，则比例背压不处理*/
	if(ChkSysCfg(CO2_CHGBK) && (pp->hy->HYDR_PORT_CHRGBACK > 1 ||(ServoTest()&&pp->hy->HYDR_PORT_CHRGBACK>0)))					 
	{
		if (backpres==0)
		{
			SetPOBit(pp->po->O_CHR_RL2, PC_ON);   //零背压时开储料泄压阀
			if(ChkSysCfg(CO5_YXBIG))
				SetPOBit(pp->po->O_CHR_BAC, PC_OFF);  
		}
		else
		{
			if(ChkSysCfg(CO5_YXBIG))
				SetPOBit(pp->po->O_CHR_BAC, PC_ON); 
			SetPOBit(pp->po->O_CHR_RL2, PC_OFF);
		}
		if(ChkSysCfg(CO5_BAPRSACC))
			backpres = backpres/10;

		ActSet_Da_Pres(pact, MC_CTR_MODE_DA, backpres,0, 0,
					0,pp->hy->HYDR_PORT_CHRGBACK,pp->hy->HYDR_PUMPS_CHARGE, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CHARGE),1);

		if(pp->hy->HYDR_PORT_CHRGBACK == 1)
		{
			SetHydrDisplay_P(0x02,backpres);
		}
		else
		{
			SetHydrDisplay_P(Get_BackPort_Simulation(pp),backpres);
		}
	}  
}

void SetChargeBwdNum(PINJECT pp, PACT pact)
{
	UI16 hydelay,flow=0;
	UI16 ind = GET_M_STEP_NO(pact->step - CHARGE1) - 1;
	if(pp->mh->CHG_HYDELAY > 9999)
		pp->mh->CHG_HYDELAY = 0;
	hydelay = pp->mh->CHG_HYDELAY;

	ChargeVlv(pp);	

  
 	if(pp->mh->FL_ELECTRIC_CHG == 1)
    {
        if(MotorIsOn2())
        {
            ActSet_Da_flow(pact, MC_CTR_MODE_DA, *(&(pp->pa->DAFLOW_CHG1) + ind), 0, pp->mh->DAFLOW_CHGRAMP,
    				(pact->ftaskstat & INTF_CEC) ? 0 : HYDRDELY, ELECCHG_DA_PORT, ELECCHG_DA_PORT, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CHARGE),1);
        }
        SetHydrDisplay_F(ELECCHG_DISPLAY_CH, (*(&(pp->pa->DAFLOW_CHG1) + ind))*10);
    }
    else if(ChkOperMode(OM_AUTO) || (CUS_ID != CS_WEILISHI) || pp->md->SPEED_LIMIT == 0)
	{	  
		ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, *(&(pp->pa->DAPRES_CHG1) + ind),*(&(pp->pa->DAFLOW_CHG1) + ind),0,0,pp->mh->DAPRES_CHGRAMP, pp->mh->DAFLOW_CHGRAMP,
					   (pact->ftaskstat & INTF_CEC) ? 0 : HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_CHARGE),pp->hy->HYDR_PUMPS_CHARGE,pp->hy->HYDR_PUMPS_CHARGE,
						pp->hy->SERVO_KP_CHARGE,pp->hy->SERVO_KI_CHARGE, 0,  !(pact->ftaskstat & INTF_CEC), HYDR_PRI(pp->hy->HYDR_PORT_CHARGE),pp->mh->DAFLOW_VLVLIMIT);
  	}
  	else
	{	  
		flow = pp->md->SPEED_LIMIT < *(&(pp->pa->DAFLOW_CHG1) + ind)?pp->md->SPEED_LIMIT: *(&(pp->pa->DAFLOW_CHG1) + ind);
		ActSetHydr(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, *(&(pp->pa->DAPRES_CHG1) + ind),flow,0,0,pp->mh->DAPRES_CHGRAMP, pp->mh->DAFLOW_CHGRAMP,
					   (pact->ftaskstat & INTF_CEC) ? 0 : HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_CHARGE),pp->hy->HYDR_PUMPS_CHARGE,pp->hy->HYDR_PUMPS_CHARGE,
						pp->hy->SERVO_KP_CHARGE,pp->hy->SERVO_KI_CHARGE, 0,  !(pact->ftaskstat & INTF_CEC), HYDR_PRI(pp->hy->HYDR_PORT_CHARGE));
  	}
	if(ChkSysCfg(CO2_CHGBK) && (pp->hy->HYDR_PORT_CHRGBACK > 1 ||(ServoTest()&&pp->hy->HYDR_PORT_CHRGBACK>0)))
	{
		SetChargeBack(pp,pact,*(&(pp->pa->DAPRES_CHGBACK1) + ind));
	}
}

void SetChargeBwdAg(PINJECT pp, PACT pact)
{
	UI16 hydelay,flow=0;
	UI16 ind = pp->md->SEC_CHG - 1;
    
	if(pp->mh->CHG_HYDELAY > 9999)
		pp->mh->CHG_HYDELAY = 0;
	hydelay = pp->mh->CHG_HYDELAY;

	ChargeVlv(pp);	

  
 	if(pp->mh->FL_ELECTRIC_CHG == 1)
    {
        if(MotorIsOn2())
        {
            ActSet_Da_flow(pact, MC_CTR_MODE_DA, *(&(pp->pa->DAFLOW_CHG1) + ind), 0, pp->mh->DAFLOW_CHGRAMP,
    				(pact->ftaskstat & INTF_CEC) ? 0 : HYDRDELY, ELECCHG_DA_PORT, ELECCHG_DA_PORT, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CHARGE),1);
        }
        SetHydrDisplay_F(ELECCHG_DISPLAY_CH, (*(&(pp->pa->DAFLOW_CHG1) + ind))*10);
    }
    else if(ChkOperMode(OM_AUTO) || (CUS_ID != CS_WEILISHI) || pp->md->SPEED_LIMIT == 0)
	{	  
		ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, *(&(pp->pa->DAPRES_CHG1) + ind),*(&(pp->pa->DAFLOW_CHG1) + ind),0,0,pp->mh->DAPRES_CHGRAMP, pp->mh->DAFLOW_CHGRAMP,
					   (pact->ftaskstat & INTF_CEC) ? 0 : HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_CHARGE),pp->hy->HYDR_PUMPS_CHARGE,pp->hy->HYDR_PUMPS_CHARGE,
						pp->hy->SERVO_KP_CHARGE,pp->hy->SERVO_KI_CHARGE, 0,  !(pact->ftaskstat & INTF_CEC), HYDR_PRI(pp->hy->HYDR_PORT_CHARGE),pp->mh->DAFLOW_VLVLIMIT);
  	}
  	else
	{	  
		flow = pp->md->SPEED_LIMIT < *(&(pp->pa->DAFLOW_CHG1) + ind)?pp->md->SPEED_LIMIT: *(&(pp->pa->DAFLOW_CHG1) + ind);
		ActSetHydr(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, *(&(pp->pa->DAPRES_CHG1) + ind),flow,0,0,pp->mh->DAPRES_CHGRAMP, pp->mh->DAFLOW_CHGRAMP,
					   (pact->ftaskstat & INTF_CEC) ? 0 : HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_CHARGE),pp->hy->HYDR_PUMPS_CHARGE,pp->hy->HYDR_PUMPS_CHARGE,
						pp->hy->SERVO_KP_CHARGE,pp->hy->SERVO_KI_CHARGE, 0,  !(pact->ftaskstat & INTF_CEC), HYDR_PRI(pp->hy->HYDR_PORT_CHARGE));
  	}
	if(ChkSysCfg(CO2_CHGBK) && (pp->hy->HYDR_PORT_CHRGBACK > 1 ||(ServoTest()&&pp->hy->HYDR_PORT_CHRGBACK>0)))
	{
		SetChargeBack(pp,pact,*(&(pp->pa->DAPRES_CHGBACK1) + ind));
	}
}


void SetChargeBwdPurge(PINJECT pp, PACT pact)
{
	UI16 hydelay;
	UI16 flow = 0;
	
	if(pp->mh->CHG_HYDELAY > 9999)
		pp->mh->CHG_HYDELAY = 0;
	hydelay = pp->mh->CHG_HYDELAY;
	
	if(ChkSysCfg(CO43_CHARGE))
		SetPOBit(pp->po->O_INJ_SOL, PC_ON);		// 恩格 ，储料时打开射出泵

	if (CUS_ID == CS_WEILISHI && pp->md->SPEED_LIMIT > 0)
	{
		if (pp->pa->DAFLOW_CHGPURGE > pp->md->SPEED_LIMIT)
		{
			flow = pp->md->SPEED_LIMIT;
		}
		else
		{
			flow = pp->pa->DAFLOW_CHGPURGE;
		}
	}
	else
	{
		flow = pp->pa->DAFLOW_CHGPURGE;
	}

    if(pp->mh->FL_ELECTRIC_CHG == 1)
    {
        if(MotorIsOn2())
        {
            ActSet_Da_flow(pact, MC_CTR_MODE_DA, flow, 0, pp->mh->DAFLOW_CHGRAMP,
    				(pact->ftaskstat & INTF_CEC) ? 0 : HYDRDELY, ELECCHG_DA_PORT, ELECCHG_DA_PORT, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CHARGE),1);
        }
        SetHydrDisplay_F(ELECCHG_DISPLAY_CH, flow*10);
    }
    else
    {
        ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_CHGPURGE,flow,0,0,pp->mh->DAPRES_CHGRAMP, pp->mh->DAFLOW_CHGRAMP,
                             (pact->ftaskstat & INTF_CEC) ? 0 : HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_CHARGE),pp->hy->HYDR_PUMPS_CHARGE,pp->hy->HYDR_PUMPS_CHARGE,
                              pp->hy->SERVO_KP_CHARGE,pp->hy->SERVO_KI_CHARGE, 0,  !(pact->ftaskstat & INTF_CEC), HYDR_PRI(pp->hy->HYDR_PORT_CHARGE), pp->mh->DAFLOW_VLVLIMIT);
    }
	
	if(ChkSysCfg(CO2_CHGBK) && (pp->hy->HYDR_PORT_CHRGBACK > 1 ||(ServoTest()&&pp->hy->HYDR_PORT_CHRGBACK>0)))
		SetChargeBack(pp,pact, *(&(pp->pa->DAPRES_CHGBACKPUGRE)));
}



void ChargeOffVlv(PINJECT pp)              //储料方向阀关闭
{
	SetPOBit(GetSysPO(O_FST_SOL), PC_OFF); 
	SetPOBit(pp->po->O_CHR_FST, PC_OFF); 
	SetPOBit(pp->po->O_CHR_BAC, PC_OFF); 
	SetPOBit(pp->po->O_CHR_REL, PC_OFF); 
	SetPOBit(pp->po->O_CHR_RL2, PC_OFF); 
	SetPOBit(pp->po->O_CHG_SUC, PC_OFF); 
	SetPOBit(pp->po->O_CHR_FST, PC_OFF); 
    SetPOBit(pp->po->O_PRE_MOLDENABLE, PC_OFF);		//光塑，电储料时输出预塑使能 20230527dylong
	SetPOBit(GetSysPO(O_FST_SOL), PC_OFF); 
	SetPOBit(GetSysPO(O_HYD_VLV2), PC_OFF); 
	SetPOBit(GetSysPO(O_HYD_COMB), PC_OFF); 
	SetPOBitDelay(GetSysPO(O_SOEC_SLM),PC_OFF, GetSysMold()->TMDLY_LINKVLVOFF); 
	SetChargePressFwdPO(pp->pchgprs, FALSE);
	SetNozzleFwdPO(pp->pnzl, FALSE);
	ClampChargeOff(pp->pclmp);
	pp->st->CN_CHRGSPD = 0;
}

void ChargeOff(PINJECT pp, PACT pact)
{
    if(pp->mh->FL_ELECTRIC_CHG == 1)
    {
        HYDR_CLS_Da_F(ELECCHG_DA_PORT, pp->mh->DAFLOW_CHGOFFRAMP, 1);
        SetHydrDisplay_F(ELECCHG_DISPLAY_CH, 0);
    }
    else
    	HYDR_CLS(pp->mh->DAPRES_CHGOFFRAMP, pp->mh->DAFLOW_CHGOFFRAMP);
    
	if(ChkSysCfg(CO2_CHGBK) && (pp->hy->HYDR_PORT_CHRGBACK > 1 ||(ServoTest()&&pp->hy->HYDR_PORT_CHRGBACK>0)))    //2014 - 12-03   修改在普通机模式下背压通道必须大于1才输出背压
	{
		HYDR_CLS_Da_P(pp->hy->HYDR_PORT_CHRGBACK,pp->mh->DAPRES_CHGOFFRAMP,1);

		if(pp->hy->HYDR_PORT_CHRGBACK == 1)
		{
			SetHydrDisplay_P(0x02,0);
		}
		else
		{
			SetHydrDisplay_P(Get_BackPort_Simulation(pp),0);
		}
	}

	if(pp->mh->TMDLY_VLVOFFCHGEND > 500)
		pp->mh->TMDLY_VLVOFFCHGEND = 0;
	ChargeOffVlv(pp);                                    
	SetPOBitDelay(pp->po->O_CHR_SOL,PC_OFF, pp->mh->TMDLY_VLVOFFCHGEND);	

	if(ChkSysCfg(CO43_CHARGE))
		SetPOBit(pp->po->O_INJ_SOL, PC_OFF);                                       // 恩格 ，储料时打开射出泵
}


void ChargeOffDelay(PINJECT pp, PACT pact)
{
    if(pp->mh->FL_ELECTRIC_CHG == 1)
    {
        HYDR_CLS_Da_F(ELECCHG_DA_PORT, pp->mh->DAFLOW_CHGOFFRAMP, 1);
        SetHydrDisplay_F(ELECCHG_DISPLAY_CH, 0);
    }
    else
	    HYDR_CLS(pp->mh->DAPRES_CHGOFFRAMP, pp->mh->DAFLOW_CHGOFFRAMP);
    
	if(ChkSysCfg(CO2_CHGBK) && (pp->hy->HYDR_PORT_CHRGBACK > 1 ||(ServoTest()&&pp->hy->HYDR_PORT_CHRGBACK>0)))    //2014 - 12-03   修改在普通机模式下背压通道必须大于1才输出背压
	{
		HYDR_CLS_Da_P(pp->hy->HYDR_PORT_CHRGBACK,pp->mh->DAPRES_CHGOFFRAMP,1);

		if(pp->hy->HYDR_PORT_CHRGBACK == 1)
		{
			SetHydrDisplay_P(0x02,0);
		}
		else
		{
			SetHydrDisplay_P(Get_BackPort_Simulation(pp),0);
		}
	}

	if(pp->mh->TMDLY_VLVOFFCHGEND > 500)
		pp->mh->TMDLY_VLVOFFCHGEND = 0;
	ChargeOffVlv(pp);                                    
	SetPOBitDelay(pp->po->O_CHR_SOL,PC_OFF, pp->mh->TMDLY_VLVOFFCHGEND);	
	
	if(ChkSysCfg(CO43_CHARGE))
		SetPOBit(pp->po->O_INJ_SOL, PC_OFF);                                       // 恩格 ，储料时打开射出泵
}

void  ClearChgOut(PINJECT pp, PACT pact)
{
	ChargeOff(pp,pact);	
	
	if(ChkActRun(pp, ACT_SN_BIT(OS_CHRG)))
	{
		EndActTsk(((PACT)(pp->act + OS_CHRG - 1))->ptsk, FALSE);
	}
}

void GetChargeRpm(PINJECT pp, PACT pact)                     // 储料转速计算  
{
	UI32 TMChg;	
	UI32 rpd_tmp;
    PPUMP ppump = (PPUMP)GetMachineSubPart(PUMP_ID, 2);
    UI16 ind = GET_M_STEP_NO(pact->step - CHARGE1) - 1;
	
	PICntLoop(&pp->chg_cnt);
	TMChg = GetPICycleMs(&pp->chg_cnt);
	if(ChkSysCfg(CO4_SAF2) && ppump != NULL)
	{
		pp->st->CN_CHRGSPD=ppump->st->MOTOR_SPEED_REAL/0.43;
	}
    else if(ChkSysCfg(CO5_REMOVE_NEWHYDR))
	{
		pp->st->CN_CHRGSPD = ((*(&(pp->pa->DAFLOW_CHG1) + ind)*350)/99)*10;
	}
	else
	{
		if(TMChg > 0)
		{
			rpd_tmp = (60000.0*10/TMChg);
			if(rpd_tmp< 0xffff)
				pp->st->CN_CHRGSPD = rpd_tmp;
			else
				pp->st->CN_CHRGSPD = 0xffff - 1;
		}
	}
}
ACT_RET GetChargeRpmadj(PACT pact, UI32 para)
{
	ACT_RET ret = ACT_OK;
	PINJECT pp = (PINJECT)(pact->part);
	int tmp = 0;

	if(!DAIsAdjust() || ChkActRun(pp, ACT_SN_BIT(OS_CHRG)))
	{
		return ACT_ERR_ABORT;
	}
	
	switch(pact->step)
	{
		case INIT1_STEP:
			if((ret = ChargePre(pp)) != ACT_OK)
			{
				return ret;
			}

			SetPOBit(pp->po->O_CHR_SOL,PC_ON);

			if(!ChkSysCfg(CO5_YXBIG) && pp->md->FL_CHGBACK==1)
				SetPOBit(pp->po->O_CHR_BAC, PC_ON);	 //储料背压阀
				
			ResetPICnt(&(pp->chg_cnt), pp->pi->I_RPM_CHK, GetSysConfig()->CN_CHGRPMHOLE);

			ACT_STEP_NEW(STAGE_1_STEP);
			break;
		case STAGE_1_STEP:			
		case STAGE_1_STEP+1:
			tmp = GetMeterLenByCh(pp->mh->INJ_METERCH);
			GetChargeRpm(pp, pact);
			
			if(pp->md->SEC_CHG > 3)  
				pp->md->SEC_CHG = 3;
			if((tmp > 0 && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->INJ_METERCH), *(&(pp->pa->ADPOSI_CHG1END) + pp->md->SEC_CHG-1))) ||
			   (tmp <= 0 && MoveLocationNMControlProc(pp->pi->I_CHR_END)))
			{
				ACT_STEP_NEW(END_STEP);
			}	

			CHK_SET_MOV(SetChargeBwdAdj);
			break;
		case END_STEP:
			ClearChgOut(pp,pact);
			SET_PART_ERR(pp, ER0_CHFL);
			break;
		default:
			break;
	}

	LoopHydrauic_sv();
	return ret;
}


void EndDaChgActTsk()
{
	PINJECT pp = (PINJECT)GetPartByID(MK_PART(INJECT_ID, 1));
	EndActTsk(((PACT)(pp->act + OS_DACHRG - 1))->ptsk, TRUE);
}

void SetChargeBwdAdj(PINJECT pp, PACT pact)
{
    if(pp->mh->FL_ELECTRIC_CHG == 1)
    {
        if(MotorIsOn2())
        {
            ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->pa->DAFLOW_CHG1, 0, pp->mh->DAFLOW_CHGRAMP,
    				0, ELECCHG_DA_PORT, ELECCHG_DA_PORT, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CHARGE),1);
        }
        SetHydrDisplay_F(ELECCHG_DISPLAY_CH, pp->pa->DAFLOW_CHG1*10);
    }
    else
    {
    	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES,pp->pa->DAPRES_CHG1,pp->pa->DAFLOW_CHG1,0,0,pp->mh->DAPRES_CHGRAMP, pp->mh->DAFLOW_CHGRAMP,
                          0,HYDR_PORT(pp->hy->HYDR_PORT_CHARGE),pp->hy->HYDR_PUMPS_CHARGE,pp->hy->HYDR_PUMPS_CHARGE,
                          pp->hy->SERVO_KP_CHARGE,pp->hy->SERVO_KI_CHARGE, 0,  !(pact->ftaskstat & INTF_CEC), HYDR_PRI(pp->hy->HYDR_PORT_CHARGE), pp->mh->DAFLOW_VLVLIMIT);
    }
}

BOOL ChargeTimeout(PINJECT pp, PACT pact)
{
	DWORD    maxtime;

	if(ChkOperMode(OM_AUTO) && AutoAlarmIsStart())
	{
		maxtime = (UI32)pp->st->TM_CHARGE_MAX;
	}
	else
	{
		maxtime = ((UI32)pp->md->TM_CHGDEFAULTMAX_H << 16) + ((UI32)pp->md->TM_CHGDEFAULTMAX_L);
	}

	if(ACT_RUN_MS >= maxtime*10)
	{
		SET_PART_ERR(pp, ER0_HPEM);
		return TRUE;
	}	
  	return FALSE;
}

BOOL PurgeGuardChgErrChk(PINJECT pp)
{
	return PartErrSet(pp, ER0_PURG, CheckPI(pp->pi->I_PUR_GUA) != PI_SET);
}

BOOL DriveErr()
{
	PSERVO	ps = &g_hydrservo;
    PINJECT pp = (PINJECT)GetPartByID(MK_PART(INJECT_ID, 1));
	
	if(pp != NULL && CheckPIUsed(pp->pi->I_DRI_ALM))
	{
		if((CheckPI(pp->pi->I_DRI_ALM) == PI_SET)&&!CHK_PART_ERR(ps, ER5_SVCTLERR))
		{
			if(MotorIsOn())	
			{
				SET_PART_ERR(ps, ER5_SVCTLERR);	
				StandAlarm(TRUE);
                return TRUE;
			}
		}			
	}

    return FALSE;
}

ACT_RET ChargeBackGrd(PINJECT pp, PACT pact)         //charge background work 
{ 
	ACT_RET ret = ACT_OK;
    PPUMP ppump = (PPUMP)GetPartByID(MK_PART(PUMP_ID, 2));
    PMOTOR pmotor = (PMOTOR)GetPartByID(MK_PART(MOTOR_ID, 1));
    PINJECT pinj = (PINJECT)GetPartByID(MK_PART(INJECT_ID, 1));
    PACT pact1;
    
    if(ChkSysCfg(CO4_SAF2))
    {
        if(ppump != NULL && ((ppump->st->MOTOR_CURRENT)>(pmotor->mh->CURRENT_ALARM_MAX)) && (pmotor->mh->CURRENT_ALARM_MAX>0))
        {
            SET_PART_ERR(pmotor,ER1_CURRENTERR);
            ManualInitErr();
            SetOperMode(OM_MANUAL);
            StandAlarm(TRUE);
            if(MotorIsOn()) 
                 MotorDown();   
            return ACT_ERR_ABORT;
        }
    }

	if(!InjTempChkSimple(pp))
		return ACT_ERR_ABORT;

	if(ChkSysCfg(CO11_SAFECHG))          //2014-08-04储料检测安全门
	{
    	if((FSafeDoorIsCloseErrChk(pp->pclmp->psfdr)))
			return ACT_ERR_ABORT;
	}
    if(ChkSysCfg(CO11_CHGPUA)&&(PurgeGuardChgErrChk(pp)))
		return ACT_ERR_ABORT;
		
	if(pact->step > CHARGE1 && Chg_Move_Check(pp))
		return ACT_ERR_ABORT;
	
	if(ChkSysCfg(CO0_BMC))		 //BMC机 储料加压 07-7-27
	{
		if(ChargePressLvlPIChk(pp->pchgprs) == PI_SET)
		{
			if( !CHK_PART_ERR(pp, ER0_CHGL))
			{
				SET_PART_ERR(pp, ER0_CHGL);
				TempAlarm(TRUE);
				pact->ftaskstat |= INTF_ERR;
				ret = ACT_ERR;
			}
			else
				pact->ftaskstat &= (~INTF_ERR);
		}
		
		if(ChargePressFwdPIChk(pp->pchgprs) == PI_SET)
		{
			SET_PART_ERR(pp, ER0_CHGL);			
			ManualInitErr();
			StandAlarm(FALSE);
 			SetOperMode(OM_MANUAL);
			return ACT_ERR_ABORT;
		}

        if(pact->step >= END_STEP)
		{
			pact1 = pp->pchgprs->act + OS_CHGPRSF-1;
			if(ChkActRun(pp->pchgprs,ACT_SN_BIT(OS_CHGPRSF)))
				EndActTsk(pact1->ptsk,TRUE);
		}
	}

	if(pact->step < END_STEP)
	{
		if(ChargeTimeout(pp, pact))
		{
			pact->ftaskstat |= INTF_ERR2;
			ACT_STEP_NEW(END_STEP);
		}

		//调机时选择时间储料，用于空打全自动
		if((pp->mh->FL_CHARGEBYTM > 0) && (pp->mh->TM_CHARGE > 0))
		{
			if ((UI16)(ACT_RUN_MS/10) > pp->mh->TM_CHARGE)
			{
				ACT_STEP_NEW(END_STEP);
			}
		}
	}	

	
	if (pact->step >= CHARGE1 && pact->step <= CHARGE5)
	{
		GetChargeRpm(pp, pact);
	}

	if(GetMeterLenByCh(pp->mh->INJ_METERCH) <= 0 && MoveLocationNMControlProc(pp->pi->I_CHR_END))
	{
		ACT_STEP_NEW(END_STEP);
	}
	
	pp->st->TM_CHARGE_RT = ACT_RUN_MS/10;

    if(DriveErr())
        return ACT_ERR_ABORT;
    
	return ret;
}

ACT_RET ChargeProc(PACT pact, UI32 para)
{
	PMACHINE pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1));

	PINJECT pp = (PINJECT)(pact->part);
	PNOZZLE	pnzl = (PNOZZLE)GetMachineSubPart(NOZZLE_ID, 1);
	ACT_RET ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;

	if(ActIODelayLimit(pp->po->O_CHR_SOL, TRUE))
	{
	    return ACT_ERR_ABORT;
	}
		
	switch (pact->step)
	{
		case INIT1_STEP:
            if(ChkOperMode(OM_AUTO))
			{							//自动模式下做储料动作时，压料缸动作按键无效，但压料缸进动作仍然有效(lzh 2022.1.4)
				pp->pchgprs->chgprsflag = 2;
			}
            
			if((ret = ChargePre(pp)) != ACT_OK)
			{
				return ret;
			}
			
			ACT_STEP_NEW(INIT2_STEP);
			break;
		case INIT2_STEP:
			EndActTsk(((PACT)(pp->act + OS_NEEDLEVALVE- 1))->ptsk, TRUE);
			
			ResetPICnt(&(pp->chg_cnt), pp->pi->I_RPM_CHK, GetSysConfig()->CN_CHGRPMHOLE);
			pnzl->coolsts=1;
			coolstart = GetMsTick();

			if (pp->md->SEC_CHG > 5)
			{
				pp->md->SEC_CHG = 2;
			}				
			ACT_START();
			ACT_STEP_NEW(CHARGE1);
			break;
		case CHARGE1:						
			pp->Chg_nomove_Tm = Get10MsTick();
			pp->Chg_nomove_Pos = GetMeterPosByCh(pp->mh->INJ_METERCH);
			if(pp->mh->FL_CHG_CHECKMM == 1)
			{
				if(ChargeAftInj(pp))
				{
					return ACT_ERR_ABORT;
				}
			}			
		case CHARGE1 + 1:
			if (MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->INJ_METERCH), pp->pa->ADPOSI_CHG1END))
			{
				if(pp->pa->ADPOSI_CHG2END > GetMeterMax(pp->mh->INJ_METERCH) || pp->md->SEC_CHG <= 1)
				{
					ACT_STEP_NEW(END_STEP);
				}
				else	
				{
					ACT_STEP_NEW(CHARGE2);
				}
			}			

			CHK_SET_MOV(SetChargeBwdNum);
			break;
		case CHARGE2:
		case CHARGE2 + 1:
			if (MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->INJ_METERCH), pp->pa->ADPOSI_CHG2END))
			{
				if(pp->pa->ADPOSI_CHG3END > GetMeterMax(pp->mh->INJ_METERCH) || pp->md->SEC_CHG <= 2)
				{
					ACT_STEP_NEW(END_STEP);
				}
				else	
				{
					ACT_STEP_NEW(CHARGE3);
				}
			}

			CHK_SET_MOV(SetChargeBwdNum);
			break;
		case CHARGE3:
		case CHARGE3 + 1:
			if (MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->INJ_METERCH), pp->pa->ADPOSI_CHG3END))
			{
				if(pp->pa->ADPOSI_CHG4END > GetMeterMax(pp->mh->INJ_METERCH) || pp->md->SEC_CHG <= 3)
				{
					ACT_STEP_NEW(END_STEP);
				}
				else	
				{
					ACT_STEP_NEW(CHARGE4);
				}
			}

			CHK_SET_MOV(SetChargeBwdNum);
			break;
		case CHARGE4:
		case CHARGE4 + 1:
			if (MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->INJ_METERCH), pp->pa->ADPOSI_CHG4END))
			{
				if(pp->pa->ADPOSI_CHG5END > GetMeterMax(pp->mh->INJ_METERCH) || pp->md->SEC_CHG <= 4)
				{
					ACT_STEP_NEW(END_STEP);
				}
				else	
				{
					ACT_STEP_NEW(CHARGE5);
				}
			}

			CHK_SET_MOV(SetChargeBwdNum);
			break;
		case CHARGE5:
		case CHARGE5 + 1:
			if (MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->INJ_METERCH), pp->pa->ADPOSI_CHG5END))
			{
				ACT_STEP_NEW(END_STEP);
			}

			CHK_SET_MOV(SetChargeBwdNum);
			break;
		case END_STEP:			
			SET_OFF(ChargeOffDelay);
			ACT_STEP_NEW(END_STEP + 1);
			if(pp->mh->TMDLY_CHGEND > 999)
				pp->mh->TMDLY_CHGEND = 0;
			if(pp->mh->TMDLY_VLVOFFCHGEND > 500)
				pp->mh->TMDLY_VLVOFFCHGEND = 0;
			if(pp->mh->TMDLY_CHGEND < pp->mh->TMDLY_VLVOFFCHGEND)
				ActSetMovTM(pact->ptsk, pp->mh->TMDLY_VLVOFFCHGEND/10);
			else
				ActSetMovTM(pact->ptsk, pp->mh->TMDLY_CHGEND/10);
			break;
		case END_STEP + 1:			
			if(GetPOBit(pp->po->O_CHR_SOL) <= 1 && ACT_STEP_MS > pp->mh->TMDLY_CHGEND*10)
			{
				if(!(pact->ftaskstat & INTF_ERR2))
				{
					pp->st->TM_CHARGE = ACT_RUN_MS/10;
					pp->st->TM_CHARGE_RT = pp->st->TM_CHARGE;
			
					if(AutoAlarmCanUpdate())
					{
						pp->st->TM_CHARGE_MAX = pp->st->TM_CHARGE	+ (pp->st->TM_CHARGE >> 1) + 5 + pp->md->TM_CHARGE_UP;
			
						if(pp->st->TM_CHARGE+500 > pp->st->TM_CHARGE_MAX)
							pp->st->TM_CHARGE_MAX = pp->st->TM_CHARGE+500;	 //自动警报时，储料警报上限过小，警报上限改为上一模储料时间加5S
					}
					
					pp->st->ADPOSI_CHGEND = INT_ABS(GetMeterPosByCh(pp->mh->INJ_METERCH));
					InjectCmd( pp,CMD_QC_PARA,2);
				}
			
				ACT_STEP_NEW(END_STEP + 2);
			}
			break;
		case END_STEP + 2:
			ret = ACT_END;
			break;
		default:
			ret = ACT_ERR_ABORT;
			break;
	}
	
	if(ret != ACT_ERR_ABORT && ret != ACT_END)
		ret = ChargeBackGrd(pp, pact);

	return ret;
}

void SetChargeAdjBwd(PINJECT pp, PACT pact)
{
	UI16 hydelay = 0;
	UI16 flow = 0;
	
	if(pp->mh->CHG_HYDELAY > 9999)
		pp->mh->CHG_HYDELAY = 0;
	hydelay = pp->mh->CHG_HYDELAY;
	SetPOBit(pp->po->O_CHG_SUC, PC_ON); 
	SetPOBit(pp->po->O_CHR_SOL, PC_ON);

	if (CUS_ID == CS_WEILISHI && pp->md->SPEED_LIMIT > 0)
	{
		
		if (pp->mh->DAFLOW_CHGSETUP > pp->md->SPEED_LIMIT)
		{
			flow = pp->md->SPEED_LIMIT;
		}
		else
		{
			flow = pp->mh->DAFLOW_CHGSETUP;
		}
	}
	else
	{
		flow = pp->mh->DAFLOW_CHGSETUP;
	}

    if(pp->mh->FL_ELECTRIC_CHG == 1)
    {
        if(MotorIsOn2())
        {
            ActSet_Da_flow(pact, MC_CTR_MODE_DA, flow, 0, pp->mh->DAFLOW_CHGRAMP,
    				(pact->ftaskstat & INTF_CEC) ? 0 : HYDRDELY, ELECCHG_DA_PORT, ELECCHG_DA_PORT, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CHARGE),1);
        }
        SetHydrDisplay_F(ELECCHG_DISPLAY_CH, flow*10);
    }
    else
    {
        ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->mh->DAPRES_CHGSETUP,flow,0, 0,pp->mh->DAPRES_CHGRAMP,pp->mh->DAFLOW_CHGRAMP,
                          HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_CHARGE),pp->hy->HYDR_PUMPS_CHARGE,pp->hy->HYDR_PUMPS_CHARGE,
                            pp->hy->SERVO_KP_CHARGE,pp->hy->SERVO_KI_CHARGE, 0, !(pact->ftaskstat & INTF_CEC), HYDR_PRI(pp->hy->HYDR_PORT_CHARGE),pp->mh->DAFLOW_VLVLIMIT); 
    }
}

ACT_RET ChargeAdj(PACT pact, UI32 para)                              //调模状态下手动储料
{
	PINJECT pp = (PINJECT)(pact->part);
	ACT_RET ret = ACT_OK;
    static UI16    charge_end = 0;
	int	tmp;

	if(pp == NULL)
		return ACT_ERR_ABORT;

	switch(pact->step)
	{
		case INIT1_STEP:
			if(!InjTempChkSimple(pp))
				return ACT_ERR_ABORT;
			else
			{				
				ResetPICnt(&(pp->chg_cnt), pp->pi->I_RPM_CHK, GetSysConfig()->CN_CHGRPMHOLE);
				ACT_STEP_NEW(STAGE_1_STEP);
			}
			break;
			
		case STAGE_1_STEP:
            charge_end = *(&(pp->pa->ADPOSI_CHG1END)+pp->md->SEC_CHG-1);    //得出对应的储料结束位置
		case STAGE_1_STEP + 1:
			if(!ChkSysCfg(XF5_INJMUTL))
			{
				tmp = GetMeterLenByCh(pp->mh->INJ_METERCH);
				if((tmp > 0 && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->INJ_METERCH), charge_end)) ||
				   (tmp <= 0 && MoveLocationNMControlProc(pp->pi->I_CHR_END)))
				{
				   	ACT_STEP_NEW(END_STEP);
				}
			}

			CHK_SET_MOV(SetChargeAdjBwd);
			break;
		case END_STEP:
			SET_OFF(ChargeOff);
			ret = ACT_END;
			break;
		default:
			ret = ACT_END;
			break;
	}

	if(ret == ACT_END || ret == ACT_ERR_ABORT)
		return ret;
	
	return ChargeBackGrd(pp, pact);
}  

ACT_RET AutoPurgeCharge(PACT pact, UI32 para)              //自动清料储料
{
	PINJECT pp = (PINJECT)(pact->part);
	ACT_RET ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;

	switch(pact->step)
	{
		case INIT1_STEP:
			if((ret = ChargePre(pp)) != ACT_OK)
			    return ret;
			else
			{				
				ResetPICnt(&(pp->chg_cnt), pp->pi->I_RPM_CHK, GetSysConfig()->CN_CHGRPMHOLE);
			    ACT_STEP_NEW(STAGE_1_STEP);
			}
			break;
			
		case STAGE_1_STEP:
		case STAGE_1_STEP + 1:
			if(MoveLocationTM10msControlProc(pp->md->TM_AUTOPURGE))
				ACT_STEP_NEW(END_STEP);

			if(ChkSysCfg(CO2_CHGX))
			{
				if(CHK_SET_MOV_M(SetChargeBwdPurge, 0))
				{
					ChargeVlv(pp);
					ActSetTM(pact->ptsk, pp->md->TM_AUTOPURGE);
				}
			}
			else
			{
				if(CHK_SET_MOV_M(SetChargeBwdNum, 0))
				{
					ChargeVlv(pp);
					ActSetTM(pact->ptsk, pp->md->TM_AUTOPURGE);
				}
			}
			break;
		case END_STEP:
			SET_OFF(ChargeOff);
			ret = ACT_END;
			break;

		default:
			ret = ACT_END;
			break;
	}

	if(ret == ACT_END || ret == ACT_ERR_ABORT)
		return ret;
	
	return ChargeBackGrd(pp, pact);
}  


ACT_RET AutoPurge(PACT pact, UI32 para)              //自动清料
{
	PINJECT pp = (PINJECT)(pact->part);
	ACT_RET ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;

	switch(pact->step)
	{
		case INIT1_STEP:
			pp->autopurgecnt = 0;
			if(!InjTempChkSimple(pp))
			{
				SetOperMode(OM_MANUAL);
				return ACT_ERR_ABORT;
			}
			else
				ACT_STEP_NEW(STAGE_1_STEP);
			break;
			
		case STAGE_1_STEP:
			/*启动自动清料储料*/
			ACT_STEP_NEW(STAGE_2_STEP);
			ForkAct(pp->act + OS_PCHRG- 1, SUSPEND_FORK, pact->ptsk, 0, TASKSTNDDLY);
			break;

		case STAGE_2_STEP:
			ACT_STEP_NEW(STAGE_3_STEP);
			ForkAct(pp->act + OS_INJE - 1, SUSPEND_FORK, pact->ptsk, 0, TASKSTNDDLY);
			break;

		case STAGE_3_STEP:
			pp->autopurgecnt++;
			if(pp->autopurgecnt < pp->md->CN_AUTOPURGE)
				ACT_STEP_NEW(STAGE_1_STEP);
			else
				ACT_STEP_NEW(END_STEP);
			break;
			
		case END_STEP:
			ret = ACT_END;
			SetOperMode(OM_MANUAL);
			break;

		default:
			ret = ACT_END;
			break;
	}

	return ret;
}  

/*射前储料, para = 1, 射前时间储料， = 2 二次射出储料做距离*/
ACT_RET ChargeBeforeInj(PACT pact, UI32 para) 
{
	PINJECT pp = (PINJECT)(pact->part);
	ACT_RET ret = ACT_OK;
	
	if(pp == NULL)
		return ACT_ERR_ABORT;

	switch(pact->step)
	{
		case INIT1_STEP:
			if(para == 2)
				pp->chg_pos = pp->md->ADPOSI_CHGFLOWMOLD + GetMeterPosByCh(pp->mh->INJ_METERCH);
			
			if((ret = ChargePre(pp)) != ACT_OK)                  //储料条件判断
				return ret;
			else
				ACT_STEP_NEW(STAGE_1_STEP);
			break;
			
		case STAGE_1_STEP:
		case STAGE_1_STEP + 1:
			if((para != 2 && STEP_10MS_CHK(pp->md->TM_CHGFLOWMOLDING)) ||
			  (para == 2 && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->INJ_METERCH), pp->chg_pos)))
				ACT_STEP_NEW(END_STEP);

			if(CHK_SET_MOV_M(SetChargeBwdAg, 0))
			{
				ChargeVlv(pp);
				SetPOBit(pp->po->O_CHR_BAC,PC_OFF);        //储料背压阀   
			}
			break;
			
		case END_STEP:
			SET_OFF(ChargeOffDelay);
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
		return ChargeBackGrd(pp, pact);
}
