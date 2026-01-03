/******************************************************************************
  文 件 名   : suckback.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 螺杆部件射退动作
  函数列表   :
              SetSuckBack
              SetSuckBackAdj
              SetSuckBackBfChg
              SuckBack
              SuckBackGrd
              SuckBackPre
              SuckBackTimeoutChk
              SuckBwdAdj
              SuckBwdBfChg
              SuckBwdOff
              SuckBwdVlv
              SuckBwdVlvOff
  修改历史   :
  1.日    期   : 2013年5月17日
    作    者   : ren chaohong
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "actcom.h"
#include "clamp.h"
#include "nozzle.h"
#include "robot.h"
#include "inject.h"
#include "machapi.h"
#include "HydrPos.h"
#include "EleCtrlApp.h"


/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
extern I16 flow_dot;

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
UI16 GetMaxRPM(WORD ServoNo);

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

/*-----------------------------------------------------------------------------------+
|      射退初始条件检查                                                              |
+-----------------------------------------------------------------------------------*/
ACT_RET SuckBackPre(PINJECT pp)
{
    if(!InjTempChkPro(pp))
	{
		return ACT_ERR_ABORT;
	}

	return ACT_OK;
}


void SetSuckPosOff(PINJECT pp)
{
	PACT pact = pp->part.pact_tbl+OS_SUCK-1;
	
    PosControlEnd(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_SUCKBACK)));
}


void SuckBwdVlv(PINJECT pp)
{
	if(pp->mh->TMDLY_VLV_ONSUCKBK > 500)
		pp->mh->TMDLY_VLV_ONSUCKBK = 0;

	if(pp->mh->FL_CHR_RL2_SUK)
	{
		SetPOBit(pp->po->O_CHR_RL2, PC_ON);
	}
	
	SetPOBit(pp->po->O_CHG_SUC, PC_ON);
	SetPOBitDelay(pp->po->O_SUC_SOL, PC_ON, pp->mh->TMDLY_VLV_ONSUCKBK);
	SetPOBit(pp->po->O_INJ_SRV, PC_ON);
	if(CUS_ID == CS_ZHISHANG)
		SetPOBit(pp->po->O_CHR_REL, PC_ON);		  //储料泄压阀
}

void SetSuckBack(PINJECT pp, PACT pact)
{
	UI16 hydelay,mode;
	if(pp->mh->SUCKBACK_HYDELAY > 9999)
		pp->mh->SUCKBACK_HYDELAY = 0;
	hydelay = pp->mh->SUCKBACK_HYDELAY;

	SuckBwdVlv(pp);
	if(pp->md->FL_SUCK_SOOMTH == 1)
		mode = MC_CTR_MODE_POSI|MC_CTR_MODE_PRES;
	else
		mode = MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES;

    if(pp->mh->MODE_CHOOSE == 1)
	{
	    ActSet_Da_Pres(pact, MC_CTR_MODE_DA, pp->pa->DAPRES_SUCKBACK,pp->pressramp_suck[SUCK_RAMP_START]/10, pp->pressramp_suck[SUCK_RAMP_START]/10,
				0,SUCK_SPC_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_SUCKBACK),1);
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->pa->DAFLOW_SUCKBACK,pp->spdramp_suck[SUCK_RAMP_START]/10, pp->spdramp_suck[SUCK_RAMP_START]/10,
			    0,SUCK_SPC_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_SUCKBACK),1);

        SetHydrDisplay_P(SUCK_SPC_DISPLAY_CH, pp->pa->DAPRES_SUCKBACK);
		SetHydrDisplay_F(SUCK_SPC_DISPLAY_CH, pp->pa->DAFLOW_SUCKBACK*10);
	}
    else
    {
        ActSetHydr_SpdSR(pact, mode, pp->pa->DAPRES_SUCKBACK,pp->pa->DAFLOW_SUCKBACK,pp->pressramp_suck[SUCK_RAMP_START]/10,pp->spdramp_suck[SUCK_RAMP_START]/10,pp->pressramp_suck[SUCK_RAMP_START]/10,pp->spdramp_suck[SUCK_RAMP_START]/10,
    						(pact->ftaskstat & INTF_CEC) ? 0 : HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_SUCKBACK),pp->hy->HYDR_PUMPS_SUCKBACK,pp->hy->HYDR_PUMPS_SUCKBACK,
    						pp->hy->SERVO_KP_SUCKBACK,pp->hy->SERVO_KI_SUCKBACK, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_SUCKBACK),pp->pa->PER_DAFLOW_SUCKBACK); 
    }

	if (pp->mh->FL_INJ_PROV == 1)
	{		
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->pa->DAFLOW_SUCKBACK,pp->spdramp_suck[SUCK_RAMP_START]/10, pp->spdramp_suck[SUCK_RAMP_START]/10,
				0,INJ_PROV_DA_CH,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_SUCKBACK),INJ_PROV_BWD);
	}
}

void SetSuckBackSlow(PINJECT pp, PACT pact)
{
	UI16 hydelay;
	if(pp->mh->SUCKBACK_HYDELAY > 9999)
		pp->mh->SUCKBACK_HYDELAY = 0;
	hydelay = pp->mh->SUCKBACK_HYDELAY;

	SuckBwdVlv(pp);

    if(pp->mh->MODE_CHOOSE == 1)
	{
	    ActSet_Da_Pres(pact, MC_CTR_MODE_DA, pp->pa->DAPRES_SUCKSLOW,pp->pa->DAPRES_SUCKRAMPSLOW, pp->pa->DAPRES_SUCKRAMPSLOW,
				0,SUCK_SPC_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_SUCKBACK),1);
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->pa->DAFLOW_SUCKSLOW,pp->pa->DAFLOW_SUCKRAMPSLOW, pp->pa->DAFLOW_SUCKRAMPSLOW,
			    0,SUCK_SPC_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_SUCKBACK),1);

        SetHydrDisplay_P(SUCK_SPC_DISPLAY_CH, pp->pa->DAPRES_SUCKSLOW);
		SetHydrDisplay_F(SUCK_SPC_DISPLAY_CH, pp->pa->DAFLOW_SUCKSLOW*10);
	}
    else
    {
        ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_SUCKSLOW,pp->pa->DAFLOW_SUCKSLOW,pp->pa->DAPRES_SUCKRAMPSLOW,pp->pa->DAFLOW_SUCKRAMPSLOW,pp->pa->DAPRES_SUCKRAMPSLOW,pp->pa->DAFLOW_SUCKRAMPSLOW,
    						HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_SUCKBACK),pp->hy->HYDR_PUMPS_SUCKBACK,pp->hy->HYDR_PUMPS_SUCKBACK,
    						pp->hy->SERVO_KP_SUCKBACK,pp->hy->SERVO_KI_SUCKBACK, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_SUCKBACK),pp->pa->PER_DAFLOW_SUCKBACK); 
    }

	if (pp->mh->FL_INJ_PROV == 1)
	{		
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->pa->DAFLOW_SUCKSLOW,pp->pa->DAFLOW_SUCKRAMPSLOW, pp->pa->DAFLOW_SUCKRAMPSLOW,
				0,INJ_PROV_DA_CH,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_SUCKBACK),INJ_PROV_BWD);
	}
}


void SetSuckBackBfChg(PINJECT pp, PACT pact)
{
	UI16 hydelay;
	if(pp->mh->SUCKBACK_HYDELAY > 9999)
		pp->mh->SUCKBACK_HYDELAY = 0;
	hydelay = pp->mh->SUCKBACK_HYDELAY;
	SuckBwdVlv(pp);

    if(pp->mh->MODE_CHOOSE == 1)
	{
	    ActSet_Da_Pres(pact, MC_CTR_MODE_DA, pp->pa->DAPRES_SUCKBACK,pp->pressramp_suck[SUCK_RAMP_START]/10, pp->pressramp_suck[SUCK_RAMP_START]/10,
				0,SUCK_SPC_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_SUCKBACK),1);
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->pa->DAFLOW_SUCKBACK,pp->spdramp_suck[SUCK_RAMP_START]/10, pp->spdramp_suck[SUCK_RAMP_START]/10,
			    0,SUCK_SPC_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_SUCKBACK),1);

        SetHydrDisplay_P(SUCK_SPC_DISPLAY_CH, pp->pa->DAPRES_SUCKBACK);
		SetHydrDisplay_F(SUCK_SPC_DISPLAY_CH, pp->pa->DAFLOW_SUCKBACK*10);
	}
    else
    {
        ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_SUCKBACK,pp->pa->DAFLOW_SUCKBACK,pp->pressramp_suck[SUCK_RAMP_START]/10,pp->spdramp_suck[SUCK_RAMP_START]/10,pp->pressramp_suck[SUCK_RAMP_START]/10,pp->spdramp_suck[SUCK_RAMP_START]/10,
    				HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_SUCKBACK),pp->hy->HYDR_PUMPS_SUCKBACK,pp->hy->HYDR_PUMPS_SUCKBACK,
    				pp->hy->SERVO_KP_SUCKBACK,pp->hy->SERVO_KI_SUCKBACK, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_SUCKBACK),pp->pa->PER_DAFLOW_SUCKBACK); 
    }

	if (pp->mh->FL_INJ_PROV == 1)
	{		
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->pa->DAFLOW_SUCKBACK,pp->spdramp_suck[SUCK_RAMP_START]/10, pp->spdramp_suck[SUCK_RAMP_START]/10,
				0,INJ_PROV_DA_CH,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_SUCKBACK),INJ_PROV_BWD);
	}
}

BOOL SuckBackTimeoutChk(PINJECT pp, PACT pact)
{
	UI32 SuckBackMaxTime;

	if(ChkOperMode(OM_AUTO) && AutoAlarmIsStart())
		SuckBackMaxTime = pp->st->TM_SUCKBACK1_MAX;
	else
		SuckBackMaxTime = pp->md->TM_SUCKBACKDEFAULTMAX;

	return (ACT_STEP_MS >= SuckBackMaxTime*10);
}

void SuckBwdVlvOff(PINJECT pp)
{
	if(pp->mh->TMDLY_VLVOFFSUCKBACKEND > 500)
		pp->mh->TMDLY_VLVOFFSUCKBACKEND = 0;
	SetPOBit(pp->po->O_CHR_RL2, PC_OFF);
	SetPOBit(pp->po->O_CHG_SUC, PC_OFF);
	SetPOBitDelay(pp->po->O_SUC_SOL, PC_OFF, pp->mh->TMDLY_VLVOFFSUCKBACKEND);
	SetPOBit(pp->po->O_INJ_SRV, PC_OFF);
	if(CUS_ID == CS_ZHISHANG)
		SetPOBit(pp->po->O_CHR_REL, PC_OFF);		  //储料泄压阀
}

void SuckBwdOff(PINJECT pp, PACT pact)
{
	if(pp != NULL)
	{
		SuckBwdVlvOff(pp);	
        if(pp->mh->MODE_CHOOSE == 1)
    	{
    		HYDR_CLS_Da_P(SUCK_SPC_PORT,pp->pressramp_suck[SUCK_RAMP_END]/10,1);
    	    HYDR_CLS_Da_F(SUCK_SPC_PORT,pp->spdramp_suck[SUCK_RAMP_END]/10,1);
            
            SetHydrDisplay_P(SUCK_SPC_DISPLAY_CH, 0);
    		SetHydrDisplay_F(SUCK_SPC_DISPLAY_CH, 0);
    	}
        else
		    HYDR_CLS(pp->pressramp_suck[SUCK_RAMP_END]/10,pp->spdramp_suck[SUCK_RAMP_END]/10);
		if(pp->md->FL_SUCK_SOOMTH == 1)
			SetSuckPosOff(pp);

		if (pp->mh->FL_INJ_PROV == 1)
		{		
			HYDR_CLS_Da_F(INJ_PROV_DA_CH,pp->spdramp_suck[SUCK_RAMP_END]/10,INJ_PROV_BWD);
		}
	}
}

void SuckBwdOffDelay(PINJECT pp, PACT pact)
{
	if(pp != NULL)
	{
		SuckBwdVlvOff(pp);	
        if(pp->md->FL_SUCK_SOOMTH == 1)
			SetSuckPosOff(pp);
        
        if(pp->mh->MODE_CHOOSE == 1)
    	{
    		HYDR_CLS_Da_P(SUCK_SPC_PORT,pp->pressramp_suck[SUCK_RAMP_END]/10,1);
    	    HYDR_CLS_Da_F(SUCK_SPC_PORT,pp->spdramp_suck[SUCK_RAMP_END]/10,1);
            
            SetHydrDisplay_P(SUCK_SPC_DISPLAY_CH, 0);
    		SetHydrDisplay_F(SUCK_SPC_DISPLAY_CH, 0);
    	}
        else
        {
    		if(pp->md->FL_SUCK_SOOMTH == 1)
    		{
    			HYDR_CLS(0,0);
    		}
    		else
    		{
    			HYDR_CLS(pp->pressramp_suck[SUCK_RAMP_END]/10,pp->spdramp_suck[SUCK_RAMP_END]/10);
    		}
        }
		if (pp->mh->FL_INJ_PROV == 1)
		{		
			HYDR_CLS_Da_F(INJ_PROV_DA_CH,pp->spdramp_suck[SUCK_RAMP_END]/10,INJ_PROV_BWD);
		}
	}
}


ACT_RET SuckBackGrd(PINJECT pp, PACT pact)
{
	pp->st->TM_SUCKBK_RT = ACT_RUN_MS/10;
	return ACT_OK;
}

void SuckBackStartInit(PINJECT pp, PACT pact)
{
	UI16 i=0;
	UI32 speedbuf = 0;
	if(pp == NULL)
		return;
	                                                                              
	for(i = 0;i<SUCK_SEG;i++)
	{
		pp->Suck_Flow_Buf[i] = 0;
		pp->Suck_Pos_Buf[i] = 0;
	}
	pp->Suck_Endpos = 0;
	pp->Suck_Num = 0;

	if (!flow_dot)
	{
		if (pp->pa->DAFLOW_SUCKBACK == 99)
		{
			speedbuf = 999;
		}
		else
		{
			speedbuf = pp->pa->DAFLOW_SUCKBACK*10;
		}
	}
	else
	{
		speedbuf = pp->pa->DAFLOW_SUCKBACK;
	}

	pp->Suck_Flow_Buf[0] = speedbuf*(UI32)pp->pa->PER_DAFLOW_SUCKBACK/100;
	pp->Suck_Pos_Buf[0] = pp->pa->ADPOSI_SUCKBACKEND;

	//设置终点位置
	pp->Suck_Endpos = pp->Suck_Pos_Buf[0];
	pp->Suck_Num = 1;
}


void SetSuckPos_Bwd(PINJECT pp, PACT pact)
{
	int seg = 0;
	int PSramp[3] = {0};
	int PPramp[3] = {0};
	int Pspd[MAXNUMSEG] = {0};
	int Ppres[MAXNUMSEG] = {0};
	int Ppos[MAXNUMSEG] = {0};
	UI16 i = 0;
	UI16 endposbuf = 0xFFFF;	//射退时，电子尺逐步变大，所以此处初始化为最大值
	UI16 motor_max_spd = 0;
	int nowpos = 0;
	UI16 flow_limit = 0;
	UI16 mm = 0x01;

	motor_max_spd = GetMaxRPM(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_SUCKBACK))-1);
	
	if(motor_max_spd < 500 || motor_max_spd > 9999)
	{
		motor_max_spd = 1800;
	}
	flow_limit = pp->mh->LOWFLOW_SUCK;
	if(flow_limit < 20 || flow_limit > 200)
		flow_limit = 20;
	
	InitSetHydrStruCtrlData(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_SUCKBACK)), motor_max_spd, 0, pp->mh->DIA_CYLINDER_SUCK,pp->mh->NUM_CYLINDER_SUCK, GetMotorPump_Q(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_SUCKBACK)),HYDR_PUMP(pp->hy->HYDR_PUMPS_SUCKBACK)),pp->mh->EFFICIENCY_FACTOR_SUCK,0,flow_limit, pp->md->PUMP_K_SUCK);
	
	nowpos = GetMeterPosByCh(pp->mh->INJ_METERCH);
	if(nowpos < 0)
	{
		/*防止负数影响判断*/
		nowpos = 0;
	}
	
	if((int)(pp->Suck_Endpos) > nowpos)
	{
		endposbuf = pp->Suck_Endpos;
	}
		
	seg = 1;
	for(i = 0;i<seg;i++)
	{		
		if(pp->Suck_Pos_Buf[i] < endposbuf)
		{
			Pspd[i] = ActSetSpdToRPM(HYDR_PORT(pp->hy->HYDR_PORT_SUCKBACK)-1,pp->Suck_Flow_Buf[i]);
			Ppos[i] = pp->Suck_Pos_Buf[i];
		}
		else
		{
			Pspd[i] = ActSetSpdToRPM(HYDR_PORT(pp->hy->HYDR_PORT_SUCKBACK)-1,pp->Suck_Flow_Buf[i]);
			Ppos[i] = endposbuf;
			seg = i+1;
			break;
		}
	}


	PSramp[0] = pp->spdramp_suck[SUCK_RAMP_START];	
	PSramp[1] = pp->spdramp_suck[SUCK_RAMP_START];
	PSramp[2] = pp->spdramp_suck[SUCK_RAMP_END];

	if (ChkSysCfg(CO8_YL3))
	{
		mm = mm|DECMODE_SPD;
	}

    SetPosCtrlPara(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_SUCKBACK)), DIRECFWD, pp->mh->INJ_METERCH, PSramp, Pspd, PPramp, Ppres, Ppos, seg, pp->md->PUMP_DEAD_SUCK, pp->mh->SUCKBACK_HYDELAY,mm,0,0,0);
}

ACT_RET SuckBack(PACT pact, UI32 para)
{
	PMACHINE pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1));

	PINJECT pp = (PINJECT)(pact->part);
	ACT_RET ret = ACT_OK;
	int tmp,tmpp;
	
	if(pp == NULL)
		return ACT_ERR_ABORT;

	if(ActIODelayLimit(pp->po->O_SUC_SOL, TRUE))
	{
	    return ACT_ERR_ABORT;
	}
	
	switch(pact->step)
	{
		case INIT1_STEP:  
            if(ChkOperMode(OM_AUTO))		//自动模式下压料缸按键有效,射出动作开始后按键无效，等储料结束后才有效(lzh 2022.1.4)
			{
				pp->pchgprs->chgprsflag = 0;
			}
            
			if(SuckBackPre(pp) == ACT_ERR_ABORT)
			{
				return ACT_ERR_ABORT;
			}

			ACT_START();
			SuckBackStartInit(pp, pact);
			tmp = GetMeterPosByCh(pp->mh->INJ_METERCH);
			if(GetMeterLenByCh(pp->mh->INJ_METERCH) >= 0 && tmp >= pp->pa->ADPOSI_SUCKBACKEND)
				ACT_STEP_NEW(END_STEP + 2);
			else
			{
				if(ChkSysCfg(CO3_WSTO) && pp->pa->TM_SUCKSLOW > 0)
				{
					pp->InjPosstart = INT_ABS(GetMeterPosByCh(pp->mh->INJ_METERCH));
					ACT_STEP_NEW(STAGE_SPEC3_STEP);
				}
				else
					ACT_STEP_NEW(STAGE_1_STEP);
			}
			
			pp->st->ADPOSI_CHGEND = INT_ABS(tmp);
			break;
			
		case STAGE_SPEC3_STEP:
		case STAGE_SPEC3_STEP + 1:	
			tmpp = GetMeterLenByCh(pp->mh->INJ_METERCH);
			if(pp->pa->TM_SUCKSLOW > 2000)
				pp->pa->TM_SUCKSLOW = 2000;
			if((tmpp>0 && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->INJ_METERCH),(pp->InjPosstart + pp->pa->ADPOSI_SUCKSLOW)))
				|| ACT_STEP_100US >=  ((UI32)pp->pa->TM_SUCKSLOW)*10)
					ACT_STEP_NEW(STAGE_1_STEP);
			CHK_SET_MOV(SetSuckBackSlow);
			break;
			
		case STAGE_1_STEP:
			if(pp->md->FL_SUCK_SOOMTH == 1)
			{
				SET_MOV2(SetSuckPos_Bwd);
			}
		case STAGE_1_STEP + 1:
			tmp = GetMeterLenByCh(pp->mh->INJ_METERCH);
			if((tmp > 0 && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->INJ_METERCH), pp->pa->ADPOSI_SUCKBACKEND)) ||
			   (tmp <= 0 && MoveLocationNMControlProc(pp->pi->I_SUC_END)))
				ACT_STEP_NEW(END_STEP);

			if(SuckBackTimeoutChk(pp, pact))
			{
				SET_PART_ERR(pp, ER0_SUCK);
			}

			CHK_SET_MOV(SetSuckBack);
			break;
			
		case END_STEP:
			SET_OFF(SuckBwdOffDelay);
			ACT_STEP_NEW(END_STEP + 1);
			if(pp->mh->TMDLY_VLVOFFSUCKBACKEND > 500)
				pp->mh->TMDLY_VLVOFFSUCKBACKEND = 0;
			if(pp->mh->TMDLY_SUCKBACKEND > 999)
				pp->mh->TMDLY_SUCKBACKEND = 999;
			if(pp->mh->TMDLY_SUCKBACKEND < pp->mh->TMDLY_VLVOFFSUCKBACKEND)
				ActSetMovTM(pact->ptsk, pp->mh->TMDLY_VLVOFFSUCKBACKEND);
			else
				ActSetMovTM(pact->ptsk, pp->mh->TMDLY_SUCKBACKEND);
			break;
		case END_STEP + 1:
			if(pp->mh->TMDLY_SUCKBACKEND > 999)
				pp->mh->TMDLY_SUCKBACKEND = 999;
			if(GetPOBit(pp->po->O_SUC_SOL) <= 1 && ACT_STEP_100US > ((UI32)pp->mh->TMDLY_SUCKBACKEND)*100)
			{
				ACT_STEP_NEW(END_STEP + 2);
			}
			break;
				
		case END_STEP + 2:
			if(ChkOperMode(OM_AUTO))
			{
				pp->st->TM_SUCKBACK1 = ACT_RUN_MS/10;
				
				if(AutoAlarmCanUpdate())
					pp->st->TM_SUCKBACK1_MAX = pp->st->TM_SUCKBACK1  + (pp->st->TM_SUCKBACK1 >> 1) + 200;

				pp->st->ADPOSI_SUCKBACK = INT_ABS(GetMeterPosByCh(pp->mh->INJ_METERCH));
				InjectCmd( pp,CMD_QC_PARA,3);
			}
            ret = ACT_END;
			break;
		default:
			ret = ACT_END;
			break;
	}
	
	if(ret != ACT_END && ret != ACT_ERR_ABORT)
		ret = SuckBackGrd(pp, pact);

	return ret;
}

/*储料前射退*/
ACT_RET SuckBwdBfChg(PACT pact, UI32 para)
{
	PINJECT pp = (PINJECT)(pact->part);
	ACT_RET ret = ACT_OK;
	
	if(pp == NULL)
		return ACT_ERR_ABORT;

	switch(pact->step)
	{
		case INIT1_STEP:
			if(SuckBackPre(pp) == ACT_ERR_ABORT)
			{
				return ACT_ERR_ABORT;
			}
			else if((para == 2) && ChkOperMode(OM_MANUAL))   //手动储料时，强制执行储前射退
			{
				if (pp->mh->FL_CHG_CHECKMM == 0)
				{
					return ACT_END;
				}
				
				if(GetMeterPosByCh(pp->mh->INJ_METERCH) < 50)  //当电子尺小于5mm时，强制执行
				{
					ACT_STEP_NEW(STAGE_5_STEP);
				}
				else
				{
					return ACT_END;
				}
			}
			else
			{
				if ((pp->md->FL_SUCKBACKBEFORECHG == 2 && pp->md->TM_SUCKBACKBEFORECHG > 0)
					||(pp->md->FL_SUCKBACKBEFORECHG == 1 && pp->md->POS_SUCKBACKBEFORECHG > 0))
					ACT_STEP_NEW(STAGE_1_STEP);					// 储料前射退计时
				else
					return ACT_END;
			}
			break;
			
		case STAGE_1_STEP:
			if(pp->md->FL_SUCKBACKBEFORECHG ==1)
			{
				if(GetMeterPosByCh(pp->mh->INJ_METERCH) >= pp->pa->ADPOSI_SUCKBACKEND)  //增加保护，防止射出动作未做时，机器不一直停在储前射退这个动作
					pp->SuckBackPosBefChg = pp->pa->ADPOSI_SUCKBACKEND;
				else
					pp->SuckBackPosBefChg = GetMeterPosByCh(pp->mh->INJ_METERCH)+pp->md->POS_SUCKBACKBEFORECHG;
			}
			else
				ActSetTM(pact->ptsk, pp->md->TM_SUCKBACKBEFORECHG);
		case STAGE_1_STEP + 1:
			if((pp->md->FL_SUCKBACKBEFORECHG == 1 && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->INJ_METERCH), pp->SuckBackPosBefChg)) 
				|| (pp->md->FL_SUCKBACKBEFORECHG != 1 && MoveLocationTMControlProc(pp->md->TM_SUCKBACKBEFORECHG*10)))
				ACT_STEP_NEW(END_STEP);

			CHK_SET_MOV(SetSuckBackBfChg);
			break;
		case STAGE_5_STEP:
		case STAGE_5_STEP + 1:
			if(MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->INJ_METERCH), 50))
				ACT_STEP_NEW(END_STEP);
			
			CHK_SET_MOV(SetSuckBackBfChg);
			break;
		case END_STEP:
			SET_OFF(SuckBwdOff);
			SetPOBit(pp->po->O_SUC_SOL,PC_OFFD);
			ACT_STEP_NEW(END_STEP + 1);
			if(pp->mh->TMDLY_VLVOFFSUCKBACKEND > pp->mh->TMDLY_SUCKBACKENDBEFORCHG)
				ActSetMovTM(pact->ptsk, pp->mh->TMDLY_VLVOFFSUCKBACKEND);
			else
				ActSetMovTM(pact->ptsk, pp->mh->TMDLY_SUCKBACKENDBEFORCHG);
			break;
		case END_STEP + 1:
			if((ACT_STEP_100US > ((UI32)pp->mh->TMDLY_SUCKBACKENDBEFORCHG)*100) && (GetPOBit(pp->po->O_SUC_SOL) <= 1))
			{
				ret = ACT_END;
			}
			break;

		default:
			ret = ACT_END;
			break;
	}

	if(ret == ACT_END || ret == ACT_ERR_ABORT)
		return ret;
	else		
		return SuckBackGrd(pp, pact);
}

void SetSuckBackAdj(PINJECT pp, PACT pact)
{
	UI16 hydelay;
	if(pp->mh->SUCKBACK_HYDELAY > 9999)
		pp->mh->SUCKBACK_HYDELAY = 0;
	hydelay = pp->mh->SUCKBACK_HYDELAY;
	SuckBwdVlv(pp);

    if(pp->mh->MODE_CHOOSE == 1)
	{
	    ActSet_Da_Pres(pact, MC_CTR_MODE_DA, pp->mh->DAPRES_CHGSETUP,pp->pressramp_suck[SUCK_RAMP_START]/10, pp->pressramp_suck[SUCK_RAMP_START]/10,
				0,SUCK_SPC_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_SUCKBACK),1);
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->mh->DAFLOW_CHGSETUP,pp->spdramp_suck[SUCK_RAMP_START]/10, pp->spdramp_suck[SUCK_RAMP_START]/10,
			    0,SUCK_SPC_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_SUCKBACK),1);

        SetHydrDisplay_P(SUCK_SPC_DISPLAY_CH, pp->mh->DAPRES_CHGSETUP);
		SetHydrDisplay_F(SUCK_SPC_DISPLAY_CH, pp->mh->DAFLOW_CHGSETUP*10);
	}
    else
    {
        ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->mh->DAPRES_CHGSETUP,pp->mh->DAFLOW_CHGSETUP,pp->pressramp_suck[SUCK_RAMP_START]/10,pp->spdramp_suck[SUCK_RAMP_START]/10,pp->pressramp_suck[SUCK_RAMP_START]/10,pp->spdramp_suck[SUCK_RAMP_START]/10,
    						HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_SUCKBACK),pp->hy->HYDR_PUMPS_SUCKBACK,pp->hy->HYDR_PUMPS_SUCKBACK,
    						pp->hy->SERVO_KP_SUCKBACK,pp->hy->SERVO_KI_SUCKBACK, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_SUCKBACK),pp->pa->PER_DAFLOW_SUCKBACK); 
    }

	if (pp->mh->FL_INJ_PROV == 1)
	{		
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->mh->DAFLOW_CHGSETUP,pp->spdramp_suck[SUCK_RAMP_START]/10, pp->spdramp_suck[SUCK_RAMP_START]/10,
				0,INJ_PROV_DA_CH,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_SUCKBACK),INJ_PROV_BWD);
	}
}
  	
ACT_RET SuckBwdAdj(PACT pact, UI32 para)
{
	PINJECT pp = (PINJECT)(pact->part);
	ACT_RET ret = ACT_OK;
	int tmp;
	
	if(pp == NULL)
		return ACT_ERR_ABORT;

	if(!ChkOperMode(OM_ADJU))
		return ACT_ERR_ABORT;

	if(ActIODelayLimit(pp->po->O_SUC_SOL, TRUE))
	{
	    return ACT_ERR_ABORT;
	}
	
	switch(pact->step)
	{
		case INIT1_STEP:
			if(!InjTempChkSimple(pp))
				return ACT_ERR_ABORT;
			else
				ACT_STEP_NEW(STAGE_1_STEP);
			break;
			
		case STAGE_1_STEP:
		case STAGE_1_STEP + 1:
			if(!ChkSysCfg(XF5_RSV))
			{
				tmp = GetMeterLenByCh(pp->mh->INJ_METERCH);
				if((tmp >= 0 && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->INJ_METERCH) ,pp->pa->ADPOSI_SUCKBACKEND)) ||
				   (tmp <= 0 && MoveLocationNMControlProc(pp->pi->I_SUC_END)))
					ACT_STEP_NEW(END_STEP);
			}

			CHK_SET_MOV(SetSuckBackAdj);
			break;
			
		case END_STEP:
			SET_OFF(SuckBwdOff);
			ret = ACT_END;
			break;

		default:
			ret = ACT_END;
			break;
	}

	if(ret == ACT_END || ret == ACT_ERR_ABORT)
		return ret;
	else		
		return SuckBackGrd(pp, pact);
}

void SetSuckbackSync(PINJECT pp, PACT pact)
{
    SuckBwdVlv(pp);
    
    ActSet_Da_Pres(pact, MC_CTR_MODE_DA, pp->pa->DAPRES_CHGBACKHLP,pp->pressramp_suck[SUCK_RAMP_START]/10, pp->pressramp_suck[SUCK_RAMP_START]/10,
			0,SUCK_SPC_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_SUCKBACK),1);
	ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->pa->DAFLOW_CHGBACKHLP,pp->spdramp_suck[SUCK_RAMP_START]/10, pp->spdramp_suck[SUCK_RAMP_START]/10,
		    0,SUCK_SPC_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_SUCKBACK),1);

    SetHydrDisplay_P(SUCK_SPC_DISPLAY_CH, pp->pa->DAPRES_CHGBACKHLP);
	SetHydrDisplay_F(SUCK_SPC_DISPLAY_CH, pp->pa->DAFLOW_CHGBACKHLP*10);
}

void SuckbackSyncOff(PINJECT pp, PACT pact)
{
	SuckBwdVlvOff(pp);
    
	HYDR_CLS_Da_P(SUCK_SPC_PORT,pp->pressramp_suck[SUCK_RAMP_END]/10,1);
    HYDR_CLS_Da_F(SUCK_SPC_PORT,pp->spdramp_suck[SUCK_RAMP_END]/10,1);
    
    SetHydrDisplay_P(SUCK_SPC_DISPLAY_CH, 0);
	SetHydrDisplay_F(SUCK_SPC_DISPLAY_CH, 0);
}

ACT_RET SuckbackSync(PACT pact, UI32 para)
{
	PINJECT pp = (PINJECT)pact->part;
    ACT_RET ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;
		
	switch (pact->step)
	{
		case INIT1_STEP:
            if(pp->mh->MODE_CHOOSE != 1)
                ACT_STEP_NEW(END_STEP);
            else
			    ACT_STEP_NEW(STAGE_1_STEP);
			break;

   		case STAGE_1_STEP:
		case STAGE_1_STEP + 1:
    		if(ChkActRun(pp, ACT_SN_BIT(OS_CHRG) | ACT_SN_BIT(OS_ACHRG) | ACT_SN_BIT(OS_PCHRG) | ACT_SN_BIT(OS_CHRGBFINJ) | ACT_SN_BIT(OS_DACHRG)))
			{

			}
			else
			{
				ACT_STEP_NEW(END_STEP);	
			}
			
			CHK_SET_MOV(SetSuckbackSync);
      		break;

		case END_STEP:
			SET_OFF(SuckbackSyncOff);
			ret = ACT_END;
      		break;
			
      	default:
      		ret = ACT_END;
            break;
	}

	return ret;
}

