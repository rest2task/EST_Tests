/******************************************************************************
  文 件 名   : clampopn.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 开模及其他辅助动作函数
  函数列表   :
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
#include "airblow.h"
#include "safedoor.h"
#include "robot.h"
#include "eject.h"
#include "core.h"
#include "clamp.h"
#include "lub.h"
#include "machapi.h"
#include "machine.h"
#include "mainlib.h"
#include "hydr.h"
#include "EleCtrlApp.h"
#include "HydrPos.h"
#include "robot.h"
#include "coreprotect.h"


extern void SetAutoStartProc(void);
UI16 PosMid(PACT pact);
UI16 GetMaxRPM(WORD ServoNo);


extern MACHINE m_machine;
extern HYDR         g_hydrout;
extern	UI16	m_speed;
extern I16 flow_dot;


UI16				tm_10mspcult2=0;
UI16				tm_CloslowTime=0;       //开模慢速阀动作时间标记
UI16				tm_ClosUtlTime=0; 

BOOL ClpOpenInHydrPos(PCLAMP pp)
{	   
	if(pp->md->FL_CLSE_SMOOTH > 0 || ((pp->md->FL_OPN_LOCATE > 0) && (pp->md->FL_CLSOPEN_MODE == 0)))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void InitClampTmProject(PCLAMP pp)
{
	pp->clampopnmidtm = 0;
	pp->clampopntm = 0;
}

void InitClampTmProStart(PCLAMP pp,UI16 enable)
{
	pp->clampopntmchk_enable = enable;
	
	pp->allow_robot_mid = 1;
	pp->allow_robot = 1;
	pp->clampopntmchk = 0;
	pp->clampopnmidtmchk = 0;

}

UI16 ChkClampOpnTm(UI16* RememberTm, UI32 NowTm, UI32 LimitTm,UI16 enable)
{
	if (enable == 0)
		return 1;
	
	if (NowTm < LimitTm)
		return 0;

	if(ChkSysCfg(XF0_BACRL))
		return 1;

	//自动模式下做自动记录
	if (ChkOperMode(OM_AUTO))
	{
		if (*RememberTm == 0)
		{				
			*RememberTm = (UI16)NowTm;

			return 1;
		}
		else if (INT_ABS((int)(*RememberTm)-(int)NowTm) < (UI32)(*RememberTm)>>2)
		{
			*RememberTm = (UI16)NowTm;
			return 1;
		}
		else
		{
			*RememberTm = 0;
			return 0;
		}
	}
	else
	{
		*RememberTm = (UI16)NowTm;//手动下只记录不做自动判定
		return 1;
	}
}
BOOL ChkClampOpenPosInEffcpr(PCLAMP pp,UI16 pos)
{
	if (MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pos + pp->mh->ADPOSI_EFFCPR))
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

/*******************************************************************************************
 *Brief			:油温影响开模位置微调整初始化
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20200909
********************************************************************************************/
void InitOilTempAi(PCLAMP pp)
{
	UI16 i = 0;
	
	pp->g_oiltempai.clampmaxdiff = 0.8;
	pp->g_oiltempai.clampopnpos_20 = 0;
	pp->g_oiltempai.first_flag = 0;
	pp->g_oiltempai.lastdiff_flag = 0;
	pp->g_oiltempai.clampopnpos_filt = GetSys()->pclmp->st->ADPOSI_OPNEND;
	pp->g_oiltempai.clampopn_cn = 0;
	pp->g_oiltempai.clampopn_Complete_flag = 0;

	pp->g_oiltempai.cycle_tm = 1;
	pp->g_oiltempai.kp = 40;

	for (i = 0;i < OILCLAMPPOS_BUFMAX; i++)
	{
		pp->g_oiltempai.clamppos_buf[i] = 0;
	}	
}

/*******************************************************************************************
 *Brief			:油温控制微调整-开模开始标记设定(开模开始标记加开模结束标记组成完整开模)
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20200909
********************************************************************************************/
void OilAiOpenStart(PCLAMP pp, WORD flag)
{
	pp->g_oiltempai.clampopn_Complete_flag = 0;
	
	if (flag)
	{
		pp->g_oiltempai.clampopn_Complete_flag = pp->g_oiltempai.clampopn_Complete_flag | 0x1;
	}
}

/*******************************************************************************************
 *Brief			:油温控制微调整-开模结束标记设定
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20200909
********************************************************************************************/
void OilAiOpenEnd(PCLAMP pp, WORD flag)
{
	if (flag)
	{		
		pp->g_oiltempai.clampopn_Complete_flag = pp->g_oiltempai.clampopn_Complete_flag | 0x2;
	}
}

/*******************************************************************************************
 *Brief			:油温控制微调整-开模前计算开模设定位置，通过对开模设定位置的微调整来调节开模过冲距离
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20200909
********************************************************************************************/
int OilTempAiPro(PCLAMP pp)
{
	UI16 i = 0;
	UI16 min = 0;
	UI16 minno = 0;

	if (pp->mh->FL_CLAMP_FINE_TUNING != 1)
		return pp->pa->ADPOSI_OPENENDEND;
	
	if (pp->g_oiltempai.clampopn_Complete_flag == 0x03)//完整开模时，才能运行
	{
		pp->g_oiltempai.clamppos_buf[pp->g_oiltempai.clampopn_cn%10] = pp->st->ADPOSI_OPNEND;

		if (pp->g_oiltempai.first_flag == 0)
		{//首次完整开模,记录过冲位置		
			pp->g_oiltempai.setpos = 0;	

			if (pp->g_oiltempai.clampopn_cn >= 1)
			{				
				pp->g_oiltempai.first_flag = 1;

				min = pp->g_oiltempai.clamppos_buf[0];
				for (i = 0; i < 10; i++)
				{
					if (pp->g_oiltempai.clamppos_buf[i] == 0)
						continue;
					
					if (pp->g_oiltempai.clamppos_buf[i] < min)
					{
						min = pp->g_oiltempai.clamppos_buf[i];
					}
				}

				pp->g_oiltempai.clampopnpos_20 = min;
				

				if (pp->g_oiltempai.clampopnpos_20 <  pp->pa->ADPOSI_OPENENDEND)
				{
					pp->g_oiltempai.clampopnpos_20 = pp->pa->ADPOSI_OPENENDEND + 2;
				}
			}
		}
		else
		{		
			//使用低通滤波器
			min = pp->g_oiltempai.clamppos_buf[0];
			minno = 0;
			for (i = 1; i < 10; i++)
			{
				if (pp->g_oiltempai.clamppos_buf[i] == 0)
					continue;
				
				if (pp->g_oiltempai.clamppos_buf[i] < min || min == 0)
				{
					min = pp->g_oiltempai.clamppos_buf[i];
					minno = i;
				}
			}

			//实际开模位置与规划位置偏差超10.0mm后，重新初始化油温算法。
			if (ABS_DIFF(pp->g_oiltempai.clampopnpos_20,pp->st->ADPOSI_OPNEND) > 100)
			{
				InitOilTempAi(pp);
				return pp->pa->ADPOSI_OPENENDEND;
			}

			pp->g_oiltempai.clampopnpos_filt = min;

			if (pp->g_oiltempai.clampopn_cn%pp->g_oiltempai.cycle_tm == 0 && pp->g_oiltempai.clampopnpos_filt != 0)
			{
				pp->g_oiltempai.ek = pp->g_oiltempai.clampopnpos_20 - pp->g_oiltempai.clampopnpos_filt;
				
				if (pp->g_oiltempai.ek > 50)
				{
					pp->g_oiltempai.ek = 50;
				}
				if (pp->g_oiltempai.ek < -50)
				{
					pp->g_oiltempai.ek = -50;
				}				

				pp->g_oiltempai.setpos = pp->g_oiltempai.setpos + pp->g_oiltempai.ek*pp->g_oiltempai.kp/100;
				
				pp->g_oiltempai.clampopnpos_filt_last = pp->g_oiltempai.clampopnpos_filt;

				if (pp->g_oiltempai.lastsetpos != pp->g_oiltempai.setpos)
				{
					pp->g_oiltempai.clamppos_buf[minno] = 0;
				}
				
				pp->g_oiltempai.lastsetpos = pp->g_oiltempai.setpos;
			}
		}

		pp->g_oiltempai.clampopn_cn++;
		
		if (pp->g_oiltempai.clampopn_cn >= 10000)
			pp->g_oiltempai.clampopn_cn = 100;
	}

	pp->g_oiltempai.clampopn_Complete_flag = 0; //运行后清除完整开模标记
	
	return pp->pa->ADPOSI_OPENENDEND + pp->g_oiltempai.setpos;
}


/*******************************************************************************************
 *Brief			:油温控制微调整-控制介入的接口函数，放在开模动作后台
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20200909
********************************************************************************************/
void OilAiOpenBack(PCLAMP pp, PACT pact)
{
	if (pp->mh->FL_CLAMP_FINE_TUNING != 1)
		return;

	//开模开始时
	if (pact->step == INIT1_STEP && pact->next_step != INIT1_STEP)
	{		
		pp->clampend = OilTempAiPro(pp);
		OilAiOpenStart(pp, ChkAdFwdInplace(pp->mh->CLMP_METERCH));
	}

	//开模结束时
	if (pact->next_step == END_STEP && pact->step != END_STEP)
	{
		OilAiOpenEnd(pp, TRUE);
	}
}


//=========================开模微调算法(油温\润滑)=end =====================

void ClampOpenStartInit(PCLAMP pp, PACT pact)
{
	UI16 i=0,j=0,endposbuf=0,sn = 0;
	UI32 flowlimit = 0;
	if(pp == NULL)
		return;
	
	//压力速度位置在开模开始时放入，防止开模一二三四五段期间中间参数被更改
	/*开关模运动数据缓存*/
	for(i = 0;i<CLAMP_SEG;i++)
	{
		pp->flow_clamp[i] = 0;
		pp->posi_clamp[i] = 0;
	}
	
	for(i = 0;i<CLAMP_POSEND_SEG;i++)
	{
		pp->endpos_clamp[i] = 0;
	}	

	flowlimit = pp->mh->DAFLOW_CLPOPN_VLVLIMIT*pp->md->CLPOPN_SPD_UTI/100;
	
	/*设置数据写入混缓存，防止动作时变更*/
	pp->flow_clamp[0] = pp->pa->DAFLOW_OPENSLOW;
    pp->flow_clamp[1] = pp->pa->DAFLOW_OPEN2;
    pp->flow_clamp[2] = pp->pa->DAFLOW_OPEN3;
    pp->flow_clamp[3] = pp->pa->DAFLOW_OPEN4;
	pp->flow_clamp[4] = pp->pa->DAFLOW_OPENEND;
	
	pp->posi_clamp[0] = pp->pa->ADPOSI_OPENENDSLOW;
	pp->posi_clamp[1] = pp->pa->ADPOSI_OPENEND2;
	pp->posi_clamp[2] = pp->pa->ADPOSI_OPENEND3;
	pp->posi_clamp[3] = pp->pa->ADPOSI_OPENEND4;
	pp->posi_clamp[4] = pp->pa->ADPOSI_OPENENDEND;

	/*根据开模段数调整各段设定值*/
	for(i = (5 - pp->clampopnnum); i > 0; i--)
	{
		pp->flow_clamp[i] = pp->flow_clamp[i+1];
		pp->posi_clamp[i] = pp->posi_clamp[i+1];
	}

	/*途中中子位置初始化*/
	for(i=0;i<8;i++)
	{
		pp->core_sn1[i] = 0;
		pp->core_sn2[i] = 0;
		pp->corebwd[i] = 0;
	}
	pp->clpopnmidcorenum = PosMid(pact);
	/*设置终点位置缓存*/

	if (pp->mh->FL_CLAMP_FINE_TUNING == 1)
	{
		pp->endpos_clamp[0] = pp->clampend;
	}
	else
	{
		pp->endpos_clamp[0] = pp->pa->ADPOSI_OPENENDEND;
	}
	
	pp->endpos_num = 1;

	if(pp->prbt != NULL && pp->prbt->md->FL_ROBOTCTRL && pp->posi_clamp[4] > pp->mh->ADPOSI_EFFCPR && 
		 (pp->md->POS_ROBOT > pp->pa->ADPOSI_CLSPROTECTEND && pp->mh->FL_ADPOSROBOT==1) && (pp->mh->TM_ROBOT_ACT>0))
	{
		pp->endpos_clamp[pp->endpos_num] = pp->md->POS_ROBOT;
		pp->endpos_num = 2;
	}
	
	if(pp->clpopnmidcorenum > 0)
	{
		/*终止位置的数量不能超过宏定义的最大值*/
		if(pp->clpopnmidcorenum < CLAMP_POSEND_SEG)
		{
			for(i = 0;i<pp->clpopnmidcorenum;i++)
			{
				pp->endpos_clamp[pp->endpos_num] = pp->corebwd[i];
				pp->core_sn2[pp->endpos_num] = pp->core_sn1[i];
 				pp->endpos_num++;
			}
		}		
	}

	/*位置排序，由于是开模，所以结束位置有小到大排列*/
	for(i = 0;i<pp->endpos_num-1;i++)
	{
		endposbuf = pp->endpos_clamp[i];
		for(j = i+1;j<pp->endpos_num;j++)
		{
			if(endposbuf > pp->endpos_clamp[j])
			{
				endposbuf = pp->endpos_clamp[j];
				pp->endpos_clamp[j] = pp->endpos_clamp[i];
				pp->endpos_clamp[i] = endposbuf;

				sn = pp->core_sn2[j];
				pp->core_sn2[j] = pp->core_sn2[i];
				pp->core_sn2[i] = sn;
			}
		}
	}

	for (i = 0;i < CLAMP_SEG;i++)
	{
		if (!flow_dot)
		{
			if (pp->flow_clamp[i] == 99)
			{
				pp->flow_clamp[i] = 999;
			}
			else
			{
				pp->flow_clamp[i] = pp->flow_clamp[i]*10;
			}			
		}

		if (i > 0)//开模一段不进行变速
		{
			pp->flow_clamp[i] = (UI32)pp->flow_clamp[i]*(UI32)flowlimit/100;
		}
		else
		{
			pp->flow_clamp[i] = (UI32)pp->flow_clamp[i]*(UI32)pp->pa->PER_DAFLOW_OPEN1/100;
		}
	}

	pp->core_sn_last = 0;
}


/*******************************************************************************************
 *Brief			:退出新控制算法函数，结束控制，
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20181225
********************************************************************************************/
void SetClampOpenPosOff(PCLAMP pp)
{
	PACT pact = pp->part.pact_tbl+OS_OPNM-1;

	if(ClpOpenInHydrPos(pp))
    	PosControlEnd(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_CLAMPOPN)));
}

/*******************************************************************************************
 *Brief			:新算法开模初始化设定，每次激活开模动作都需要调用一次，对开模整个动作做预判
 				规划时使用的数据做一次初始化
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20181225
********************************************************************************************/
void SetClampOpenPos(PCLAMP pp, PACT pact)
{
    int seg = 0;
    int PSramp[MAXNUMSEG] = {0};
    int PPramp[MAXNUMSEG] = {0};
    int Pspd[MAXNUMSEG] = {0};
    int Ppres[MAXNUMSEG] = {0};
    int Ppos[MAXNUMSEG] = {0};
	UI16 i = 0;
	UI16 endposbuf = 0xFFFF;	//初始化为最大值
	UI32 motor_max_spd = 0;
	int nowpos = 0;
	UI16 flow_limit = 0, press_limit = 0;
	UI16 mm;
	UI16 coreendramp = 0;
	UI16 core_sn = 0;
    UI16 pump_k = 0;
	
	motor_max_spd = GetMaxRPM(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_CLAMPOPN))-1);

	if(motor_max_spd < 500 || motor_max_spd > 9999)
	{
		motor_max_spd = 1800;
	}
	
	/*设定底流*/
	flow_limit = pp->mh->LOWFLOW_OPN;
    press_limit = pp->mh->LOWPRES_OPN*10;
	if(flow_limit < 20 || flow_limit > 200)
		flow_limit = 20;	

    if((pp->md->FL_OPN_LOCATE > 0) && (pp->md->FL_CLSOPEN_MODE == 0))
		pump_k = pp->md->OPN_POS_KP;
	else
		pump_k = pp->md->PUMP_K;
    
	InitSetHydrStruCtrlData(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_CLAMPOPN)), motor_max_spd, pp->mh->DIA_PISDIA_M, pp->mh->DIA_CYLINDER_M,pp->mh->DIA_OIL_NUM, GetMotorPump_Q(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_CLAMPOPN)),HYDR_PUMP(pp->hy->HYDR_PUMPS_OPNFAST)),pp->mh->EFFICIENCY_FACTOR_OPN,press_limit,flow_limit, pump_k);

	/*使用临近的终止位置为当前开模的终止位置*/	
	for(i = 0;i<pp->endpos_num;i++)
	{
		nowpos = GetMeterPosByCh(pp->mh->CLMP_METERCH);
		if(nowpos < 0)
		{
			/*防止负数影响判断*/
			nowpos = 0;
		}
		
		if((int)(pp->endpos_clamp[i]) > nowpos)
		{
			endposbuf = pp->endpos_clamp[i];
			if (pp->core_sn2[i] > 0)
			{
				core_sn = pp->core_sn2[i];
				coreendramp = pp->pcore[pp->core_sn2[i] - 1]->pa->DAFLOW_OPNENDRAMP_CORE;
			}
			if (i > 0)
			{
				pp->core_sn_last = pp->core_sn2[i-1];
			}
			else
			{
				pp->core_sn_last = 0;
			}
			break;
		}
	}
	
	
	seg = 5;
	for(i = 0;i<seg;i++)
	{
		if(pp->flow_clamp[i] == 990)
		{
			pp->flow_clamp[i] = 999;
		}
		
		/*将原有的开模段数及参数按照现有的结束位置重排*/
		if(pp->posi_clamp[i] < endposbuf)
		{
			Pspd[i] = ActSetSpdToRPM(HYDR_PORT(pp->hy->HYDR_PORT_CLAMPOPN)-1,pp->flow_clamp[i]);
			Ppos[i] = pp->posi_clamp[i];
		}
		else
		{
			Pspd[i] = ActSetSpdToRPM(HYDR_PORT(pp->hy->HYDR_PORT_CLAMPOPN)-1,pp->flow_clamp[i]);
			Ppos[i] = endposbuf;
			
			seg = i+1;
			break;
		}
	}

	//当开合模途中中子标记等于2时，说明有途中中子刚结束，所以使用对应中子设定的开模启动斜率和结束斜率
	if (pp->core_sn_last > 0 && pp->md->FL_CLAMP_CORE_RAMP > 0)
	{
		PSramp[0] = pp->pcore[pp->core_sn_last-1]->pa->DAFLOW_OPNSTARTRAMP_CORE*10;
		PSramp[1] = pp->pcore[pp->core_sn_last-1]->pa->DAFLOW_OPNSTARTRAMP_CORE*10;
	}
	else
	{
		/*位置在开模一段范围，使用启动斜率，非这个范围内使用途中*/
		if(GetMeterPosByCh(pp->mh->CLMP_METERCH) < pp->pa->ADPOSI_OPENENDSLOW)
		{
			PSramp[0] = pp->spdramp_opn[CLP_RAMP_OPN_START];
		}
		else
		{
			/*自动状态时，途中启动的唯一可能目前就是途中中子及途中机械手*/		
			if(ChkOperMode(OM_AUTO))
			{
				PSramp[0] = pp->spdramp_opn_manual[CLP_RAMP_OPN_MANUALSTART];
			}
			else
			{
				/*手动途中启动时，使用手动启动斜率*/
				PSramp[0] = pp->spdramp_opn_manual[CLP_RAMP_OPN_MANUALSTART];
			}
		}	
		
		PSramp[1] = pp->spdramp_opn[CLP_RAMP_OPN_MID];
	}

	if (core_sn > 0 && pp->md->FL_CLAMP_CORE_RAMP > 0)
	{
		PSramp[2] = coreendramp*10;
	}
	else
	{
		/*结束位置为开模五段位置时，使用结束斜率，非五段位置时使用途中*/
		if(Ppos[seg-1] == pp->pa->ADPOSI_OPENENDEND)
		{
			PSramp[2] = pp->spdramp_opn[CLP_RAMP_OPN_END];
		}
		else
		{
			PSramp[2] = pp->spdramp_opn[CLP_RAMP_OPN_END];
		}
	}

	if(ChkSysCfg(XF2_COFST))
	{//使用大斜坡
		mm = 1;
	}
	else
	{//使用等比例斜坡
		mm = 0;
	}

	if (ChkSysCfg(CO8_YL3))
	{
		mm = mm|DECMODE_SPD;
	}

    if((pp->md->FL_OPN_LOCATE > 0) && (pp->md->FL_CLSOPEN_MODE == 0))
	{
		if (pp->md->MODE_OPN_LOCATE == 0)
		{
			mm = mm|(DECMODE_POS1<<12);
		}
		else if (pp->md->MODE_OPN_LOCATE == 1)
		{
			mm = mm|(DECMODE_POS2<<12); 
		}
		else
		{
			mm = mm|(DECMODE_POS3<<12); 
		}

		if(pp->md->FL_EXTRAMP_LIMIT > 0)	//使用外部斜率指令来限制速度及压力
		{
			mm = mm|DECMODE_SETDATA_LIMIT;
		}

		mm = mm|DECMODE_CHG_END_POS;
	}
    else
		mm = mm|DECMODE_HOLD_ON_SPD;		//解决平顺模式下开模结束会有200ms延时的问题
    
    SetPosCtrlPara(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_CLAMPOPN)), DIRECFWD, pp->mh->CLMP_METERCH, PSramp, Pspd, PPramp, Ppres, Ppos, seg, 
		pp->md->PUMP_DEAD, pp->mh->CLMPOPN_HYDRDELAY,mm,0,pp->md->OPN_OVER_SHOOT,1);
}


ACT_RET ClampOpnPreChk(PCLAMP pp)
{
	RobotClampCloseEnd(pp->prbt, FALSE);

	if(CheckPI(pp->pi->I_AIR_PRES) == PI_SET)//LY
  	{
		SET_PART_ERR(pp, ER0_AIRPRES);
		return ACT_ERR_ABORT;
	}
	if((CheckPI(pp->pi->I_CLS_UPGA) == 0))
	{
		SET_PART_ERR(pp, ER0_UPGAERR);
		return ACT_ERR_ABORT;
	}

	return ACT_OK;
}

void SetClampOpnRelease(PCLAMP pp, PACT pact)
{
	SetPOBit(pp->po->O_REL_PRS,PC_ON);	
	SetPOBit(pp->po->O_GUIDE,PC_ON);	
	SetPOBit(pp->po->O_OPN_REL,PC_ON);
}

void SetClampOpnRelOff(PCLAMP pp, PACT pact)
{
	SetPOBit(pp->po->O_REL_PRS, PC_OFF);
	SetPOBit(pp->po->O_GUIDE, PC_OFF);
    SetPOBit(pp->po->O_OPN_REL,PC_OFF);		
}

void ClampOpnSlowVlv(PCLAMP pp)
{
	if(pp->mh->TMDLY_VLV_ONOPENSLOWSTART > 500) //数据保护，超限采用0
		pp->mh->TMDLY_VLV_ONOPENSLOWSTART = 0;
	if(pp->mh->TM_OPENSLOWVLV > 999)
		pp->mh->TM_OPENSLOWVLV = 0;
	if(pp->mh->TMDLY_VLV_ONOPN > 500)
		pp->mh->TMDLY_VLV_ONOPN = 0;
	
	if(ChkSysCfg(CO1_OPLU) || ((CUS_ID==CS_YONGFENG)&&(pp->pa->ADPOSI_ULT_CLPOPEN > 0)))
		SetPOBitDelay(pp->po->O_CLS_ULT, PC_ON, pp->mh->TMDLY_VLV_ONOPENSLOWSTART);

	SetPOBit(pp->po->O_REL_PRS, PC_OFF);
	if(pp->md->FL_OPENFAST==2)
	{
		SetPOBit(pp->po->O_OPN_FST, PC_ON);
	}
	
	SetPOBitDelay(pp->po->O_OPN_SOL, PC_ON, pp->mh->TMDLY_VLV_ONOPN);
	SetPOBit(pp->po->O_MEC_SAF_AIR, PC_ON);

	SetPOBit(pp->po->O_CL_TOHI, PC_OFF);

	if(pp->md->FL_OPENFAST2POS == 1 || pp->md->FL_OPENFAST2POS == 3)
	{
		SetPOBitDelay(pp->po->O_OP_SSLW, PC_ON, pp->mh->TMDLY_VLV_ONOPENSLOWSTART);
	}
	else if (CUS_ID == CS_TONGYONG)
	{
		SetPOBit(pp->po->O_OPN_FST, PC_ON);

		if (ChkSysCfg(XF5_A500CHR))
		{
			SetPOBit(pp->po->O_OPN_SOL, PC_OFFD);
		}
	}
	
	SetPOBit(pp->po->O_OPN_REL,PC_ON);
	SetPOBit(pp->po->O_OPN_CLS,PC_ON);
	
	if (pp->mh->TM_CLAMPOPENULT2>0)
  	{	
  		SetPOBit(pp->po->O_CLS_ULT2,PC_ON);
  		tm_10mspcult2 = Get10MsTick();
  	}
	
	if((pp->mh->TM_OPENSLOWVLV>0) && ChkSysCfg(CO1_OPLT))
	{
		tm_CloslowTime = GetMsTick();
	}
	
	if((CUS_ID == CS_GDWA)&&(pp->md->Tm_Cls_Ult>0))
	{
	   SetPOBit(pp->po->O_CLS_ULT3,PC_ON);
	   tm_ClosUtlTime=GetMsTick();
	}
}


void ClampOpenStart(PCLAMP pp, PACT pact)
{		
	PMACHINE pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1));
	RobotClampCloseEnd(pp->prbt, FALSE);
}

void SetClampOpenSlow(PCLAMP pp, PACT pact)
{
	UI16 FL_OPNEJTCORE;
	UI16 hydelay,mode;
	UI16 pres,flow,vlvk;
	if(pp->mh->CLMPOPN_HYDRDELAY > 9999)
		pp->mh->CLMPOPN_HYDRDELAY = 0;
	hydelay = pp->mh->CLMPOPN_HYDRDELAY;

	FL_OPNEJTCORE = GetSysMold()->FL_OPNEJTCORE;
	if ((ChkSysCfg(CO1_SOEC)) && FL_OPNEJTCORE>=1 && FL_OPNEJTCORE<=5)
		SetPOBit(GetSysPO(O_OPN_EJE_COR), PC_ON);

	ClampOpnSlowVlv(pp);

	if(pp->md->FL_CLSOPEN_MODE == 2)
	{
		mode  = MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES;
		pres = pp->pa->DAPRES_OPENSPD1;
		flow = pp->pa->DAFLOW_OPENSPD1;
	}
	else if(ClpOpenInHydrPos(pp))
	{
		mode  = MC_CTR_MODE_POSI|MC_CTR_MODE_PRES;
		pres = pp->pa->DAPRES_OPENSLOW;
		flow = pp->pa->DAFLOW_OPENSLOW;
	}
	else
	{
		mode  = MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES;
		pres = pp->pa->DAPRES_OPENSLOW;
		flow = pp->pa->DAFLOW_OPENSLOW;
	}

	if(ChkOperMode(OM_MANUAL) && pp->md->FL_MANULOPN1SPD == 1)
	{
		pres = 240;
	}
	
	ActSetHydr_SpdSR(pact, mode, pres,flow,pp->pressramp_opn[CLP_RAMP_OPN_START]/10, pp->spdramp_opn[CLP_RAMP_OPN_START]/10,pp->pressramp_opn[CLP_RAMP_OPN_START]/10,pp->spdramp_opn[CLP_RAMP_OPN_START]/10,
						 (pact->ftaskstat & INTF_CEC) ? 0 : HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_CLAMPOPN),pp->hy->HYDR_PUMPS_OPNSLOW,pp->hy->HYDR_PUMPS_OPNSLOW,
						 pp->hy->SERVO_KP_OPN,pp->hy->SERVO_KI_OPN, 0, !(pact->ftaskstat & INTF_CEC),HYDR_PRI(pp->hy->HYDR_PORT_CLAMPOPN),pp->pa->PER_DAFLOW_OPEN1);  

	if(pp->md->FL_CLSOPEN_MODE == 2 || ChkLocateDa(pp))
	{
	    if(pp->pa->DAFLOW_VLV == 0 || pp->pa->DAFLOW_VLV >99)
			vlvk = flow;
		else
			vlvk = pp->pa->DAFLOW_VLV;
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, vlvk, 0,0,
					0,CLAMP_OPN_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPOPN),CLAMP_OPN_PID_DIREC);
	}
	else if(pp->md->FL_CLSOPEN_MODE == 4)
	{
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->pa->DAFLOW_VLV, 0,0, 
					0,CLAMP_OPN_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPOPN),CLAMP_OPN_P_DIREC);
	}
}

void SetClampOpenSlow1(PCLAMP pp, PACT pact)
{
	UI16 FL_OPNEJTCORE;
	UI16 hydelay,mode;
	if(pp->mh->CLMPOPN_HYDRDELAY > 9999)
		pp->mh->CLMPOPN_HYDRDELAY = 0;
	hydelay = pp->mh->CLMPOPN_HYDRDELAY;

	FL_OPNEJTCORE = GetSysMold()->FL_OPNEJTCORE;
	if ((ChkSysCfg(CO1_SOEC)) && FL_OPNEJTCORE>=1 && FL_OPNEJTCORE<=5)
		SetPOBit(GetSysPO(O_OPN_EJE_COR), PC_ON);

	ClampOpnSlowVlv(pp);

	if(ClpOpenInHydrPos(pp))
		mode  = MC_CTR_MODE_POSI|MC_CTR_MODE_PRES;
	else
		mode  = MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES;
	ActSetHydr_SpdSR(pact, mode, pp->pa->DAPRES_SCLAMPOPN,pp->pa->DAFLOW_SCLAMPOPN,pp->pressramp_opn[CLP_RAMP_OPN_START]/10, pp->spdramp_opn[CLP_RAMP_OPN_START]/10,pp->pressramp_opn[CLP_RAMP_OPN_START]/10,pp->spdramp_opn[CLP_RAMP_OPN_START]/10,
						 (pact->ftaskstat & INTF_CEC) ? 0 : HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_CLAMPOPN),pp->hy->HYDR_PUMPS_OPNSLOW,pp->hy->HYDR_PUMPS_OPNSLOW,
						 pp->hy->SERVO_KP_OPN,pp->hy->SERVO_KI_OPN, 0, !(pact->ftaskstat & INTF_CEC),HYDR_PRI(pp->hy->HYDR_PORT_CLAMPOPN),pp->md->CLPOPN_SPD_UTI);  
	if(pp->md->FL_CLSOPEN_MODE == 2 || ChkLocateDa(pp))
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->pa->DAFLOW_SCLAMPOPN, 0,0,
					0,CLAMP_OPN_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPOPN),CLAMP_OPN_PID_DIREC);
}


void ClampOpenFastVlv(PCLAMP pp)
{
	UI16 FL_OPNEJTCORE;
	if(pp->mh->TMDLY_VLV_OFFOPENSLOWSTART > 500) //数据保护，超限采用0
		pp->mh->TMDLY_VLV_OFFOPENSLOWSTART = 0;
	if(pp->mh->TMDLY_VLV_ONOPN > 500)
		pp->mh->TMDLY_VLV_ONOPN = 0;
	
	SetPOBit(GetSysPO(O_FST_SOL), PC_ON);
	SetPOBitDelay(pp->po->O_OPN_SOL, PC_ON, pp->mh->TMDLY_VLV_ONOPN);
	SetPOBitDelay(pp->po->O_OP_SSLW, PC_OFF, pp->mh->TMDLY_VLV_OFFOPENSLOWSTART);
	
	if(pp->md->FL_OPENFAST > 0 || (CUS_ID == CS_TONGYONG))
	{
		SetPOBit(pp->po->O_OPN_FST, PC_ON);
	}

	SetPOBit(pp->po->O_MEC_SAF_AIR, PC_ON);

	SetPOBit(pp->po->O_CL_TOHI, PC_OFF);
	SetPOBit(pp->po->O_REL_PRS, PC_OFF);
	SetPOBit(pp->po->O_CLS_ULT, PC_OFF);
	SetPOBit(pp->po->O_OPN_REL,PC_ON);	
	SetPOBit(pp->po->O_OPN_CLS,PC_ON);
	SetPOBit(pp->po->O_OPN_BAC,PC_ON);
	SetPOBit(pp->po->O_CLAMP_FST, PC_ON);

	if(CUS_ID == CS_SHIBITUO)
		SetPOBit(pp->po->O_OP_SETU,PC_ON);
	
	FL_OPNEJTCORE = GetSysMold()->FL_OPNEJTCORE;
	if ((ChkSysCfg(CO1_SOEC)) && FL_OPNEJTCORE>=1 && FL_OPNEJTCORE<=5)
		SetPOBit(GetSysPO(O_OPN_EJE_COR), PC_ON);
}

void SetClampOpenFast(PCLAMP pp, PACT pact)
{
	int bcec = ((pact->ftaskstat & INTF_CEC) != 0);
	UI16 hydelay,mode,flowlimit = 0;
	if(pp->mh->CLMPOPN_HYDRDELAY > 9999)
		pp->mh->CLMPOPN_HYDRDELAY = 0;
	hydelay = pp->mh->CLMPOPN_HYDRDELAY;

	ClampOpenFastVlv(pp);
	if(ClpOpenInHydrPos(pp))
	{
		mode  = MC_CTR_MODE_POSI|MC_CTR_MODE_PRES;
	}
	else
	{
		mode  = MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES;
	}
	
	flowlimit = pp->mh->DAFLOW_CLPOPN_VLVLIMIT*pp->md->CLPOPN_SPD_UTI/100;
	ActSetHydr_SpdSR(pact, mode, pp->pa->DAPRES_OPEN2,pp->pa->DAFLOW_OPEN2,pp->pressramp_opn[CLP_RAMP_OPN_FAST1]/10,pp->spdramp_opn[CLP_RAMP_OPN_FAST1]/10,pp->pressramp_opn[CLP_RAMP_OPN_START]/10,pp->spdramp_opn[CLP_RAMP_OPN_START]/10,
						 bcec ? 0 : HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_CLAMPOPN),pp->hy->HYDR_PUMPS_OPNFAST,pp->hy->HYDR_PUMPS_OPNFAST,
						 pp->hy->SERVO_KP_OPN,pp->hy->SERVO_KI_OPN, 0, !bcec, HYDR_PRI(pp->hy->HYDR_PORT_CLAMPOPN),flowlimit); 

    if(ChkLocateDa(pp))
	{	
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->pa->DAFLOW_OPEN2, 0,0,
					0,CLAMP_OPN_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPOPN),CLAMP_OPN_PID_DIREC);
	}
}

void SetClampOpenSlowSpc(PCLAMP pp, PACT pact)
{
//	UI16 FL_OPNEJTCORE;
	UI16 hydelay,mode;
	UI16 pres,flow,flowlimit = 0;
	if(pp->mh->CLMPOPN_HYDRDELAY > 9999)
		pp->mh->CLMPOPN_HYDRDELAY = 0;
	hydelay = pp->mh->CLMPOPN_HYDRDELAY;

    ClampOpenFastVlv(pp);
    
	if(pp->mh->DAPFLOW_OPNRAMPSTART == 0)
		pp->mh->DAPFLOW_OPNRAMPSTART = 199;	
	if(pp->mh->DAPRESS_OPNRAMPSTART == 0)
		pp->mh->DAPRESS_OPNRAMPSTART = 199;	

	mode  = MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES;
	pres = pp->mh->DAPRESS_OPNMIDSTART;
	flow = pp->mh->DAPFLOW_OPNMIDSTART;
	
	flowlimit = pp->mh->DAFLOW_CLPOPN_VLVLIMIT*pp->md->CLPOPN_SPD_UTI/100;
	
	ActSetHydr_SpdSR(pact, mode, pres,flow,pp->mh->DAPRESS_OPNRAMPSTART, pp->mh->DAPFLOW_OPNRAMPSTART,pp->mh->DAPRESS_OPNRAMPSTART,pp->mh->DAPFLOW_OPNRAMPSTART,
						 (pact->ftaskstat & INTF_CEC) ? 0 : HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_CLAMPOPN),pp->hy->HYDR_PUMPS_OPNSLOW,pp->hy->HYDR_PUMPS_OPNSLOW,
						 pp->hy->SERVO_KP_OPN,pp->hy->SERVO_KI_OPN, 0, !(pact->ftaskstat & INTF_CEC),HYDR_PRI(pp->hy->HYDR_PORT_CLAMPOPN),flowlimit);  

	if(pp->md->FL_CLSOPEN_MODE == 2 || ChkLocateDa(pp))
	{
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->mh->DAPFLOW_OPNMIDSTART, 0,0,
					0,CLAMP_OPN_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPOPN),CLAMP_OPN_PID_DIREC);
	}
	else if(pp->md->FL_CLSOPEN_MODE == 4)
	{
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->pa->DAFLOW_VLV, 0,0,
					0,CLAMP_OPN_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPOPN),CLAMP_OPN_P_DIREC);
	}
}

void SetClampOpenFast2(PCLAMP pp, PACT pact)
{
	int bcec = ((pact->ftaskstat & INTF_CEC) != 0);
	UI16 hydelay,mode,flowlimit=0;
	if(pp->mh->CLMPOPN_HYDRDELAY > 9999)
		pp->mh->CLMPOPN_HYDRDELAY = 0;
	hydelay = pp->mh->CLMPOPN_HYDRDELAY;

	if(ClpOpenInHydrPos(pp))
	{
		mode  = MC_CTR_MODE_POSI|MC_CTR_MODE_PRES;
	}
	else
	{
		mode  = MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES;
	}
	
	flowlimit = pp->mh->DAFLOW_CLPOPN_VLVLIMIT*pp->md->CLPOPN_SPD_UTI/100;
	ActSetHydr_SpdSR(pact, mode, pp->pa->DAPRES_OPEN3,pp->pa->DAFLOW_OPEN3,pp->pressramp_opn[CLP_RAMP_OPN_FAST2]/10,pp->spdramp_opn[CLP_RAMP_OPN_FAST2]/10,pp->pressramp_opn[CLP_RAMP_OPN_START]/10,pp->spdramp_opn[CLP_RAMP_OPN_START]/10,
						 bcec ? 0 : HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_CLAMPOPN),pp->hy->HYDR_PUMPS_OPNFAST,pp->hy->HYDR_PUMPS_OPNFAST,
						 pp->hy->SERVO_KP_OPN,pp->hy->SERVO_KI_OPN, 0,!bcec, HYDR_PRI(pp->hy->HYDR_PORT_CLAMPOPN),flowlimit);

    if(ChkLocateDa(pp))
	{		
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->pa->DAFLOW_OPEN3, 0,0,
					0,CLAMP_OPN_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPOPN),CLAMP_OPN_PID_DIREC);
	}
    
	if((pp->clampopnnum != 5)||(!bcec))
	{
		ClampOpenFastVlv(pp);
	}		
}

void SetClampOpenFast3(PCLAMP pp, PACT pact)
{
	int bcec = ((pact->ftaskstat & INTF_CEC) != 0);
	UI16 hydelay,mode;
	UI16 pres,flow,flowlimit=0;
	if(pp->mh->CLMPOPN_HYDRDELAY > 9999)
		pp->mh->CLMPOPN_HYDRDELAY = 0;
	hydelay = pp->mh->CLMPOPN_HYDRDELAY;

	if(pp->md->FL_CLSOPEN_MODE == 2)
	{
		mode  = MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES;
		pres = pp->pa->DAPRES_OPENSPD2;
		flow = pp->pa->DAFLOW_OPENSPD2;
	}
	else if(ClpOpenInHydrPos(pp))
	{
		mode  = MC_CTR_MODE_POSI|MC_CTR_MODE_PRES;
		pres = pp->pa->DAPRES_OPEN4;
		flow = pp->pa->DAFLOW_OPEN4;
	}
	else
	{
		mode  = MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES;
		pres = pp->pa->DAPRES_OPEN4;
		flow = pp->pa->DAFLOW_OPEN4;
	}
		
	flowlimit = pp->mh->DAFLOW_CLPOPN_VLVLIMIT*pp->md->CLPOPN_SPD_UTI/100;
	ActSetHydr_SpdSR(pact, mode, pres,flow,pp->pressramp_opn[CLP_RAMP_OPN_FAST3]/10,pp->spdramp_opn[CLP_RAMP_OPN_FAST3]/10,pp->pressramp_opn[CLP_RAMP_OPN_START]/10,pp->spdramp_opn[CLP_RAMP_OPN_START]/10,
						 bcec ? 0 : HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_CLAMPOPN),pp->hy->HYDR_PUMPS_OPNFAST,pp->hy->HYDR_PUMPS_OPNFAST,
						 pp->hy->SERVO_KP_OPN,pp->hy->SERVO_KI_OPN, 0,!bcec,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPOPN),flowlimit);
	//if(!bcec)
		ClampOpenFastVlv(pp);
	//
	if(pp->md->FL_CLSOPEN_MODE == 2 || ChkLocateDa(pp))
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, flow, 0,0,
					0,CLAMP_OPN_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPOPN),CLAMP_OPN_PID_DIREC);
}

void SetOpenEndVlv(PCLAMP pp)
{
	UI16 FL_OPNEJTCORE;
	if(pp->mh->TMDLY_VLV_ONOPENSLOWSTART > 500) //数据保护，超限采用0
		pp->mh->TMDLY_VLV_ONOPENSLOWSTART = 0;		

	SetPOBit(pp->po->O_OP_ESLW, PC_ON);
	SetPOBitDelay(pp->po->O_OPN_SOL, PC_ON, pp->mh->TMDLY_VLV_ONOPN);
	SetPOBit(pp->po->O_MEC_SAF_AIR, PC_ON);

	if (CUS_ID == CS_TONGYONG)
	{
		if (pp->md->FL_OPENFAST2POS==3 || pp->md->FL_OPENFAST2POS==2)
		{
			SetPOBit(pp->po->O_OPN_FST, PC_OFF);
		}
		else
		{
			SetPOBit(pp->po->O_OPN_FST, PC_ON);
			if (ChkSysCfg(XF5_A500CHR))
			SetPOBit(pp->po->O_OPN_SOL, PC_OFFD);
		}
	}
	else
	{
		if (pp->md->FL_OPENFAST==3 || ((pp->md->FL_OPENFAST==1 || pp->md->FL_OPENFAST==2) && (pp->md->FL_CLSOPEN_MODE == 4)))
		{
			SetPOBit(pp->po->O_OPN_FST, PC_ON);
		}
		else
		{
			SetPOBit(pp->po->O_OPN_FST, PC_OFF);
		}
	}	

	SetPOBit(pp->po->O_REL_PRS, PC_OFF);
	SetPOBit(pp->po->O_CL_TOHI, PC_OFF);
	SetPOBit(GetSysPO(O_FST_SOL), PC_OFF);
	
	if (pp->md->FL_OPENFAST2POS == 2 || pp->md->FL_OPENFAST2POS == 3)
	{
		SetPOBitDelay(pp->po->O_OP_SSLW, PC_ON, pp->mh->TMDLY_VLV_ONOPENSLOWSTART);
	}
	else
	{
		SetPOBit(pp->po->O_OP_SSLW, PC_OFF);
	}
	
	SetPOBit(pp->po->O_OPN_REL,PC_OFF);	
	SetPOBit(pp->po->O_OPN_CLS,PC_ON);
	SetPOBit(pp->po->O_OPN_BAC,PC_ON);
	SetPOBit(pp->po->O_CLAMP_FST, PC_OFF);

	if(CUS_ID == CS_SHIBITUO)
		SetPOBit(pp->po->O_OP_SETU,PC_ON);
	
	FL_OPNEJTCORE = GetSysMold()->FL_OPNEJTCORE;
	if ((ChkSysCfg(CO1_SOEC)) && FL_OPNEJTCORE>=1 && FL_OPNEJTCORE<=5)
		SetPOBit(GetSysPO(O_OPN_EJE_COR), PC_ON);
}

void SetClampOpenEnd(PCLAMP pp, PACT pact)
{
	int bcec = ((pact->ftaskstat & INTF_CEC) != 0);
	UI16 hydelay,mode,flowlimit=0;
	if(pp->mh->CLMPOPN_HYDRDELAY > 9999)
		pp->mh->CLMPOPN_HYDRDELAY = 0;
	hydelay = pp->mh->CLMPOPN_HYDRDELAY;

	SetOpenEndVlv(pp);

	if(ClpOpenInHydrPos(pp))
	{
		mode  = MC_CTR_MODE_POSI|MC_CTR_MODE_PRES;
	}
	else
	{		
		mode  = MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES;
	}
	
	flowlimit = pp->mh->DAFLOW_CLPOPN_VLVLIMIT*pp->md->CLPOPN_SPD_UTI/100;
	ActSetHydr_SpdSR(pact, mode, pp->pa->DAPRES_OPENEND,pp->pa->DAFLOW_OPENEND,pp->pressramp_opn[CLP_RAMP_OPN_LOW]/10,pp->spdramp_opn[CLP_RAMP_OPN_LOW]/10,pp->pressramp_opn[CLP_RAMP_OPN_START]/10,pp->spdramp_opn[CLP_RAMP_OPN_START]/10,
						 bcec ? 0 : HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_CLAMPOPN),pp->hy->HYDR_PUMPS_OPNEND,pp->hy->HYDR_PUMPS_OPNEND,
						 pp->hy->SERVO_KP_OPN,pp->hy->SERVO_KI_OPN, 0, !bcec, HYDR_PRI(pp->hy->HYDR_PORT_CLAMPOPN),flowlimit); 

    if(ChkLocateDa(pp))
	{		
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->pa->DAFLOW_OPENEND, 0,0,
					0,CLAMP_OPN_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPOPN),CLAMP_OPN_PID_DIREC);
	}
}

void        CLampOpenOffVlv1(PCLAMP pp)
{	
	int i;
	BOOL bchk = FALSE;

	pp->clampfunc = 0;
	SetPOBit(GetSysPO(O_FST_SOL), PC_OFF);
	SetPOBit(pp->po->O_OPN_FST, PC_OFF);
	if(CUS_ID == CS_ENGE)
		SetPOBitDelay(pp->po->O_OP_SSLW, PC_OFF, pp->mh->TMDLY_VLV_OFFOPENEND);
	else
		SetPOBit(pp->po->O_OP_SSLW, PC_OFF);
	SetPOBit(pp->po->O_REL_PRS, PC_OFF);
	SetPOBit(pp->po->O_CLS_ULT, PC_OFF);	
	SetPOBit(pp->po->O_GUIDE, PC_OFF);
	SetPOBit(pp->po->O_REL_PRS, PC_OFF);
	SetPOBit(pp->po->O_OPN_REL,PC_OFF);	
	SetPOBit(pp->po->O_OPN_CLS,PC_OFF);
	SetPOBit(pp->po->O_OPN_BAC,PC_OFF);
	SetPOBit(pp->po->O_CLAMP_FST, PC_OFF);
	SetPOBit(pp->po->O_CLS_ULT2, PC_OFF);
	SetPOBit(pp->po->O_OP_ESLW, PC_OFF);
	if(CUS_ID == CS_SHIBITUO)
		SetPOBit(pp->po->O_OP_SETU,PC_OFF);
	if(CUS_ID==CS_GDWA)
	  SetPOBit(pp->po->O_CLS_ULT3,PC_OFF);

	
	if(ChkSysCfg(CO1_SOEC))
	{
		for(i = 0; i < CLAMP_MAX_EJECT; i++)
		{
			if(ChkActRun(pp->pejt[i], ACT_SN_BIT(OS_EJTF) | ACT_SN_BIT(OS_EJTB)))
			{
				bchk = TRUE;
				break;
			}
		}

		if(!bchk && !ChkCoreIsRun())
			SetPOBit(GetSysPO(O_OPN_EJE_COR), PC_OFF);
	}
}

void ClampOpenOffVlvDelay(PCLAMP pp)
{
	if(pp->mh->TMDLY_VLV_OFFOPENEND > 500)  
		pp->mh->TMDLY_VLV_OFFOPENEND = 0;
	
	CLampOpenOffVlv1(pp);

	SetPOBit(pp->po->O_MEC_SAF_AIR, PC_OFF);
	SetPOBitDelay(pp->po->O_OPN_SOL, PC_OFF, pp->mh->TMDLY_VLV_OFFOPENEND);
	
	SetClampOpenPosOff(pp);//新算法控制关闭
}

void ClampOpenOffDelay(PCLAMP pp, PACT pact)
{
	extern HYDR g_hydrout;
    g_hydrout.clopnrel = 1;

	if(pp->md->FL_CLSOPEN_MODE == 2) 
	{
		HYDR_CLS(0,0);
	}
	else if(ClpOpenInHydrPos(pp))
	{
		HYDR_CLS(0, 0);
	}
	else
	{
		HYDR_CLS(pp->pressramp_opn[CLP_RAMP_OPN_END]/10, pp->spdramp_opn[CLP_RAMP_OPN_END]/10);
	}
	
	ClampOpenOffVlvDelay(pp);	
}

void ClampLubCount(PCLAMP pp)
{
	int i;

	if(pp->LubCoolCnt >= 1 || (pp->md->FL_LUBBYPASS == 0&&pp->LubCoolCnt==0))
	{
		for(i = 0; i < CLAMP_MAX_LUB; i++)
		{
			LubNeedChk(pp->plub[i],pp->LubCoolCnt);
		}
	}
}

void ClampOpenCount(PCLAMP pp)
{
	PMACHINE pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1));
	
	if(pp->st->CN_MOLDOPNNUM1==65535)
	{
    		pp->st->CN_MOLDOPNNUM1 = 0;
    		pp->st->CN_MOLDOPNNUM0++;	
  	}
	else 	
    	pp->st->CN_MOLDOPNNUM1++;	

	if(pp->st->CN_MOLDOPNNUM1_NCL == 65535)
	{
    		pp->st->CN_MOLDOPNNUM1_NCL = 0;
    		pp->st->CN_MOLDOPNNUM0_NCL++;	
  	}
	else 	
    	pp->st->CN_MOLDOPNNUM1_NCL++;	

	if(pm->st->MODOPENNUM1== 65535)
	{
		pm->st->MODOPENNUM1 = 0;
		pm->st->MODOPENNUM0++;	
	}
	else
		pm->st->MODOPENNUM1++;	
	
	if(pp->LubCoolCnt < 1)//&& pp->md->FL_LUBBYPASS != 0)
		pp->LubCoolCnt++;


	if(!ChkInjectCushErr())
	{
		pp->st->CN_VALIDMOLD++;
		pp->st->CN_CURPACKMOLD++;
	}	
/*        QC监控                 2014-07-06*/	
	if(pm->md->Qc_Fun ==1)
	{
		if((pm->st->Qc_Opn_Cnt==5)&&(pm->qc_chk_start == 0))
		{
			pm->st->Qc_Opn_Cnt = 0;
			pm->qc_chk_start   = 1;
		}
		else
			pm->st->Qc_Opn_Cnt++;	
	}
}

BOOL ClampOpnTimeout(PCLAMP pp, PACT pact)
{
	UI32    maxtime;

	if(ChkOperMode(OM_AUTO) && AutoAlarmIsStart())
		maxtime = pp->st->TM_CLAMPOPN_MAX;
	else
	{
		if(pp->md->TM_CLAMPOPNDEFAULTMAX >100 && pp->md->TM_CLAMPOPNDEFAULTMAX < 9999)
			maxtime = pp->md->TM_CLAMPOPNDEFAULTMAX;
		else
			maxtime = CLAMP_DEFAULT_TM;  
	}

	return (ACT_RUN_MS > maxtime*10);
}

UI16 PosMid(PACT pact)
{
   int	i,j,cnt = 0;
   UI16 ly[CLAMP_MAX_CORE],t,sn;
   PCLAMP pp = (PCLAMP)(pact->part);
   if(pp->clampmidcorebwd!=0)
   {
		for(i=0;i<CLAMP_MAX_CORE;i++)
		{
			pp->core_sn1[i] = 0;
			if((pp->clampmidcorebwd>>i)&1)
			{				
				if(pp->pcore[i] != NULL)
				{
					pp->core_sn1[cnt] = i+1;
					ly[cnt] = pp->pcore[i]->pa->ADPOSI_COREOUTACT;
				}
				if(cnt>0)
				{
					if((ly[cnt]!=ly[cnt-1]))	 //位置不同时，才会增加途中停止位置的个数  20170816
						cnt++;
				}
				else
				{
					pp->corebwd[0] = ly[cnt];	 //首个途中位置    20170816
					cnt++;
				}
			}		
		}
		for(i=0;i<(cnt-1);i++)
		{
			for(j=0;j<(cnt-i-1);j++)
			{
				if( ly[j]> ly[j+1]) 		   //对途中位置进行排序  由小到大依次排序	20170816
				{
					t = ly[j];
					ly[j] = ly[j+1];
					ly[j+1] = t;

					sn = pp->core_sn1[j];
					pp->core_sn1[j] = pp->core_sn1[j+1];
					pp->core_sn1[j+1] = sn;
				}
			}
		}
		for(i=0;i<cnt;i++)
		{
			pp->corebwd[i] = ly[i];
		}
  }
	return cnt; 				//返回途中位置个数1即一个途中位置，2即两个途中位置	20170816
}



ACT_RET ClampOpnBackgrd(PCLAMP pp, PACT pact, UI32 para)        //clampopn background work 
{
	PADMETER padm = (PADMETER)GetPartByID(MK_PART(ADMETER_ID, 1));
	PMACHINE  pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1)); 
	int i;
    int servo_spd;
	
	if(pp->mh->TM_OPENSLOWVLV > 999)
		pp->mh->TM_OPENSLOWVLV = 0;
	if(padm != NULL)
	{
		if(pp->mh->ADPOSI_EFFCPR <= 0 || pp->mh->ADPOSI_EFFCPR > padm->st->ADPOSI_METERMAX[1])   //开模位置有效区超限做保护，默认为50
			pp->mh->ADPOSI_EFFCPR = 500;
	}
	
	m_speed = pp->st->CLAMP_SPEED;

	for(i=0;i<CLAMP_MAX_CORE;i++)
	{
		if(pp->pcore[i] != NULL)
		{
			if(pp->pcore[i]->md->CORE_LIMIT_CHECK == 1 && CoreLimitSwitchErrCheck(pp->pcore[i])) //开模动作过程中检查中子行程开关
				return ACT_ERR_ABORT;

			if(pp->pcore[i]->md->CLAMP_CORE_CHECK == 1 && ClampOpenCoreStateErrCheck(pp->pcore[i],pact->step)) //开模动作过程中检测中子状态
				return ACT_ERR_ABORT;
		}
	}
	
	if(ChkSysCfg(CO0_CMPW) && (pact->step >= CLAMPOPENSLOW1) && ClampOpnTimeout(pp, pact))
	{
		SET_PART_ERR(pp, ER0_OPNM);
		if(pm->md->FL_TIME_ERR == 1)
		{}
		else
			return ACT_ERR_ABORT;
	}

	//途中机械手的输出，及机械手相关保护都在动作开始后到动作结束前生效
	if (pact->step < END_STEP && pact->step >= CLAMPOPENSLOW1 && para == 0)
	{		
		//必须在机械手未输出时保护，机械手输出后模区安全会停止输入。
		if((pp->mh->FL_ADPOSROBOT==0 || (pp->mh->FL_ADPOSROBOT==1 && (GetMeterPosByCh(pp->mh->CLMP_METERCH) < pp->md->POS_ROBOT) && (pp->pa->ADPOSI_OPENENDEND > pp->mh->ADPOSI_EFFCPR) && (pp->md->POS_ROBOT > pp->pa->ADPOSI_CLSPROTECTEND)))
            && RobotClampOpnErrChk(pp->prbt)) //警报判定放在后面，避免欧规机械手超过输出位置后仍警报
  			return ACT_ERR_ABORT;

		if(pp->prbt != NULL && pp->clampopnmidtmchk == 0 && pp->prbt->md->FL_ROBOTCTRL && GetMeterPosByCh(pp->mh->CLMP_METERCH) >= pp->md->POS_ROBOT&& pp->pa->ADPOSI_OPENENDEND > pp->mh->ADPOSI_EFFCPR && 
		 (pp->md->POS_ROBOT >=  pp->pa->ADPOSI_CLSPROTECTEND && pp->mh->FL_ADPOSROBOT==1))
		{
			//开模到触发途中机械手的时间不得小于0.3s
			pp->clampopnmidtmchk = 1;
			
			if (ChkClampOpnTm(&(pp->clampopnmidtm),ACT_RUN_MS/10,(UI32)CLMPOPNMID_MINTM,pp->clampopntmchk_enable))
			{
				
				if(pp->prbt->po->O_R_OPMD > 0 && pp->prbt->po->O_R_OPMD <= MAX_PO_NUM)
					RobotClampOpenMid(pp->prbt);
				else
					RobotClampOpenEnd(pp->prbt);
				if(pact->step!=(INIT5_STEP)&&((pp->mh->TM_ROBOT_ACT>0) || CheckPIUsed(pp->prbt->pi->I_R_OPMD))&&pp->clsopnlaststep==65535)
				{
					if(pact->step ==0)
						pp->clsopnlaststep = pact->step+1;
					else
						pp->clsopnlaststep = pact->step&0xff00;
					if(CheckPIUsed(pp->prbt->pi->I_R_OPMD) && (TestPI(pp->prbt->pi->I_R_OPMD)))
					{//防止该点存在，同时信号一直有时，驱动器切停又立马启动，会引起波动
					}
					else
					{
						SET_OFF(ClampOpenOffDelay);
						ACT_STEP_NEW(INIT5_STEP);
					}
				}
			}
		}
	}

	if(ChkSysCfg(XF4_BAKNF))
	{
		if(ClampsafeLight(pp))         //模板光幕检查
	        return ACT_ERR_ABORT;
	}

	if(ChkSysCfg(CO1_OPLT)&&(GetPOBit(pp->po->O_OP_SSLW)==1))
	{
		if((UI16)DIFF_MS(tm_CloslowTime)>=(pp->mh->TM_OPENSLOWVLV*10) && (pact->step <= CLAMPOPENFAST))
			SetPOBit(pp->po->O_OP_SSLW,PC_OFF);
	}
	if((CUS_ID==CS_GDWA)&&(GetPOBit(pp->po->O_CLS_ULT3)==1))
	{
		if((UI16)DIFF_MS(tm_ClosUtlTime)>=(pp->md->Tm_Cls_Ult*10))
			SetPOBit(pp->po->O_CLS_ULT3,0);
	}

    if (CUS_ID == CS_GUANGSHU)	//光塑位置控制一慢阀切
	{
		if (pp->pa->ADPOS_FIRSSLW_OFF > 0)
		{
			if (MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->pa->ADPOS_FIRSSLW_OFF))
			{
				SetPOBit(pp->po->O_OP_SSLW,PC_OFFD);
			}
		}		
	}
    
	pp->st->TM_CLAMPOPN_RT = CalMsTo10MsUI16(pact->start_ms);	
	
    if(ChkSysCfg(CO11_SAFECLMPOPN))            //2014-08-04 开模检测安全门
    {
		if ((FSafeDoorIsCloseErrChk(pp->psfdr)))                 //前安全门保护
			return ACT_ERR_ABORT;
    }   

	if((CheckPI(pp->pi->I_CLS_UPGA) == 0))
	{
		SET_PART_ERR(pp, ER0_UPGAERR);
		return ACT_ERR_ABORT;
	}
	
	if((pp->mh->TM_CLAMPOPENULT2)>0)
	{
		if ((TestPOIsOn(pp->po->O_CLS_ULT2)) && ((UI16)DIFF_10MS(tm_10mspcult2)>pp->mh->TM_CLAMPOPENULT2))
			SetPOBit(pp->po->O_CLS_ULT2,0);
	}

	if (CUS_ID == CS_YONGFENG)
	{
		//甬丰特殊开模慢速差动
		if(MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->pa->ADPOSI_ULT_CLPOPEN))
		{		
			if (TestPOIsOn(pp->po->O_CLS_ULT))
			{
				SetPOBit(pp->po->O_CLS_ULT,PC_OFF);
			}
		}
	}

	//开模慢速检测落料电眼满
	if(ChkSysCfg(XF3_HS7HT))
	{
		if(ChkOperMode(OM_SENR))
		{
			if((pact->step < CLAMPOPENFAST))
			{
				ClampSensorErrChk(pp);
			}
		}
	}

	if (pp->md->FL_CLSOPEN_MODE == 3 || pp->md->FL_CLSOPEN_MODE == 4)
	{
		OilAiOpenBack(pp,pact);
	}

	if ((pp->clampopntmchk == 0) && (pact->step >= CLAMPOPENSLOW1))
	{
		if (GetMeterPosByCh(pp->mh->CLMP_METERCH) >= pp->clampend*9/10)
		{
			pp->allow_robot = ChkClampOpnTm(&(pp->clampopntm),ACT_RUN_MS/10,CLMPOPNEND_MINTM,pp->clampopntmchk_enable);
			pp->clampopntmchk = 1;
		}		
	}

    if(ChkLocateDa(pp) && ((pact->step & 0xFF00) >= CLAMPOPENFAST) && ((pact->step & 0xFF00) <= CLAMPOPENEND)) //只在开模二段~开模五段同步输出
    {
        if(MIN_STEP(pact->step) == 0)
        {
            pp->Open_port = GetHydrPort_Ch(HYDR_PORT(pp->hy->HYDR_PORT_CLAMPOPN));  //获取液压通道号
            pp->Open_port = (pp->Open_port > g_pump_num)?0:(pp->Open_port-1);
        }

        if(g_pumpn[pp->Open_port] != NULL)
        {
            servo_spd = g_pumpn[pp->Open_port]->McPara.Mcnow[1] * VIR_MAX_SPD_K;  //驱动转速换算成对应模拟量输出比例

            if(servo_spd < 1*HYDRAD)
                servo_spd = 1*HYDRAD;

            SetVlvData_mn(CLAMP_OPN_PID_CH, servo_spd, CLAMP_OPN_PID_DIREC);
        }
}
    
	return ACT_OK;
}

ACT_RET ClampOpnBackgrd2(PCLAMP pp, PACT pact) 
{	
	int i;
	
	for(i=0;i<CLAMP_MAX_CORE;i++)
	{
		if(pp->pcore[i] != NULL)
		{
			if(pp->pcore[i]->md->CORE_LIMIT_CHECK == 1 && CoreLimitSwitchErrCheck(pp->pcore[i])) //调模开模/自动调模开模动作过程中检查中子行程开关
				return ACT_ERR_ABORT;
		}
	}
	
	if((pp->mh->TM_CLAMPOPENULT2)>0)
	{
		if ((TestPOIsOn(pp->po->O_CLS_ULT2)) && ((UI16)DIFF_10MS(tm_10mspcult2)>pp->mh->TM_CLAMPOPENULT2))
			SetPOBit(pp->po->O_CLS_ULT2,0);
	}

	if(pact->step >= CLAMPOPENSLOW1)
	{
		if((pp->mh->FL_ADPOSROBOT==0 || (pp->mh->FL_ADPOSROBOT==1 && (GetMeterPosByCh(pp->mh->CLMP_METERCH) < pp->md->POS_ROBOT) && (pp->pa->ADPOSI_OPENENDEND > pp->mh->ADPOSI_EFFCPR) && (pp->md->POS_ROBOT > pp->pa->ADPOSI_CLSPROTECTEND)))
		&&RobotClampOpnErrChk(pp->prbt))
  			return ACT_ERR_ABORT;
	}

	if(ChkSysCfg(XF4_BAKNF))
	{
		if(ClampsafeLight(pp))         //模板光幕检查
	        return ACT_ERR_ABORT;
	}

	//开模慢速检测落料电眼满
	if(ChkSysCfg(XF3_HS7HT))
	{
		if(ChkOperMode(OM_SENR))
		{
			if((pact->step < CLAMPOPENFAST))
			{
				ClampSensorErrChk(pp);
			}
		}
	}

    if(ChkSysCfg(CO11_SAFECLMPOPN))            //2014-08-04 开模检测安全门
    {
		if ((FSafeDoorIsCloseErrChk(pp->psfdr)))                 //前安全门保护
			return ACT_ERR_ABORT;
    } 
    
	return ACT_OK;
}

ACT_RET ClampOpen(PACT pact, UI32 para)
{
	PCLAMP pp = (PCLAMP)(pact->part);
	PSAFEDOOR ph = (PSAFEDOOR)GetPartByID(MK_PART(SAFEDOOR_ID, 1));
	ACT_RET ret = ACT_OK;
	int	tmp;
	
	if(pp == NULL)
		return ACT_ERR_ABORT;

	if(ActIODelayLimit(pp->po->O_OPN_SOL, TRUE))
	{
	    return ACT_ERR_ABORT;
	}
	pp->mh->CLMP_METERCH = DEF_CLAMP_METER_CH;
	switch (pact->step)
	{
		case INIT1_STEP:
			if((ret = ClampOpnPreChk(pp)) != ACT_OK)
				return ret;
			else
			{
				if(!RobotClampPreErrChk(pp->prbt))
				{
					if(pp->mh->TMDLY_OPENBEGIN > 999)
						pp->mh->TMDLY_OPENBEGIN	 = 0;
					ACT_STEP_NEW(INIT1_STEP + 1);
					ActSetMovTM(pact->ptsk, pp->mh->TMDLY_OPENBEGIN);
					pp->clampend = pp->pa->ADPOSI_OPENENDEND;
					pp->qc_pos_opnstart = (GetMeterPosByCh(pp->mh->CLMP_METERCH));
				}
			}

			
			pp->clsopnlaststep =65535;
			pp->clpopnmidcorenum = PosMid(pact);
			pp->clampclserr_opn=0;  //异常开模标记此处清零
			InitClampTmProStart(pp, ChkAdFwdInplace(pp->mh->CLMP_METERCH));
			if(ChkSysCfg(XF3_LHACC)&&(para==1)&&(ChkOperMode(OM_AUTO)))
			{
				pp->clampclserr_opn=1;  //异常开模标记此处置1
			}					
      		break;
      	case INIT1_STEP + 1:
			if(ACT_STEP_100US > ((UI32)pp->mh->TMDLY_OPENBEGIN)*100)
			{			
				ClampOpenStartInit(pp,pact);
				
				if(pp->md->FL_OPENDRAINFUNCTIO)
				{
					ACT_STEP_NEW(CLAMPOPNRELON);
				}
				else
				{
					if((!TestPI(pp->pi->I_CLS_END))&&(pp->qc_pos_opnstart< (pp->pa->ADPOSI_OPENENDEND-50)))	
					{
						ACT_STEP_NEW(CLAMPOPNSPC);
					}
					else 
					{
						ACT_STEP_NEW(CLAMPOPENSLOW1);
					}
				}
			}
			if(GetOperMode() == OM_MANUAL)
			{
				if(GetMeterPosByCh(pp->mh->CLMP_METERCH) >= pp->pa->ADPOSI_OPENENDEND)
				{
					ACT_STEP_NEW(END_STEP);
				}
				else if(GetMeterLenByCh(pp->mh->CLMP_METERCH) == 0 && TestPI(pp->pi->I_OPN_END))
				{
					ACT_STEP_NEW(END_STEP);
				}
			}
			break;			
    	case INIT5_STEP:                    //2014-10-28  通用韩国机机械手			  
			if(CheckPIUsed(pp->prbt->pi->I_R_OPMD))     //优先判断输入点"允许完全开模"    20170710
			{
				if(TestPI(pp->prbt->pi->I_R_OPMD))
				{
					ACT_STEP_NEW(pp->clsopnlaststep);
					pact->ftaskstat  = 0;
				}
			}
			else
			{
				if(STEP_10MS_CHK(pp->mh->TM_ROBOT_ACT))
				{
					ACT_STEP_NEW(pp->clsopnlaststep);
					pact->ftaskstat  = 0;
				}
			}
			break;
      	case CLAMPOPNRELON:
		case CLAMPOPNRELON + 1:
			if(STEP_10MS_CHK(pp->md->TM_OPENDRAIN))
				ACT_STEP_NEW(CLAMPOPNRELOFF);

			CHK_SET_MOV(SetClampOpnRelease);
			break;
		case CLAMPOPNRELOFF:
			pact->reactive_step = CLAMPOPNRELOFF;
			pp->qc_pos_opnstart = (GetMeterPosByCh(pp->mh->CLMP_METERCH));
			SetClampOpnRelOff(pp, pact);
			ClampOpenStart(pp, pact);
			ACT_START();
			if((pp->qc_pos_opnstart>pp->pa->ADPOSI_OPENENDSLOW)&&(pp->qc_pos_opnstart< (pp->pa->ADPOSI_OPENENDEND-50)))	
			{
				ACT_STEP_NEW(CLAMPOPNSPC);
			}
			else
			{
				ACT_STEP_NEW(CLAMPOPENSLOW1);
			}
			break;
		case CLAMPOPNSPC:
			pact->reactive_step = CLAMPOPNRELOFF;
		case CLAMPOPNSPC + 1:
			tmp = GetMeterLenByCh(pp->mh->CLMP_METERCH);
			if(pp->mh->TM_OPNPROTECT<200)
				pp->mh->TM_OPNPROTECT = 200;
			if((tmp > 0 && (MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), (pp->mh->ADPOS_OPNSTART+pp->qc_pos_opnstart)) || MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->pa->ADPOSI_OPENENDEND)))
				||((ACT_RUN_MS>=pp->mh->TM_OPNPROTECT)))
			{
			   	ACT_STEP_NEW(CLAMPOPENSLOW1);		
			}
			CHK_SET_MOV(SetClampOpenSlowSpc);
			break;			
		case CLAMPOPENSLOW1:
			if(ClpOpenInHydrPos(pp))
			{
				SET_MOV2(SetClampOpenPos);
			}
		case CLAMPOPENSLOW1 + 1:
			tmp = GetMeterLenByCh(pp->mh->CLMP_METERCH);
			if((tmp > 0 && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->pa->ADPOSI_OPENENDSLOW)) ||
			   (tmp <= 0 && MoveLocationNMControlProc(pp->pi->I_CLS_HIP)))
			{
				pp->st->TM_CLAMPOPN1F = ACT_STEP_10MS;
				
				if (pp->clampopnnum == 2)
				{
					if (pp->md->FL_CLSOPEN_MODE == 3 || pp->md->FL_CLSOPEN_MODE == 4)
					{
						//开模一段\四段\终止段
						ACT_STEP_NEW(CLAMPOPENFST3);
					}
					else
					{
						//开模一段\终止段
						ACT_STEP_NEW(CLAMPOPENEND);
					}					
				}
				else if (pp->clampopnnum == 3)
				{
					//开模一段\四段\终止段
					ACT_STEP_NEW(CLAMPOPENFST3);
				}
				else if (pp->clampopnnum == 4)
				{
					//开模一段\三段\四段\终止段
					ACT_STEP_NEW(CLAMPOPENFST2);
				}
				else
				{
					//开模一段\二段\三段\四段\终止段
			   		ACT_STEP_NEW(CLAMPOPENFAST);
				}				
			}
			else if((CUS_ID == CS_HAIJIN)&&ChkOperMode(OM_MANUAL)&&(pp->mh->SPCLAMP==1)&&(FSafeDoorIsOpen(pp->psfdr)==0)&&(STEP_10MS_CHK(pp->mh->TM_SCLAMPOPN)))
			{
				ACT_STEP_NEW(END_STEP);
				ForkAct(pp->act + OS_ERROPN- 1, SUSPEND_FORK, pact->ptsk, 0, 0);
			}

			CHK_SET_MOV(SetClampOpenSlow);
			break;
		case CLAMPOPENFAST:
		case CLAMPOPENFAST + 1:
			tmp = GetMeterLenByCh(pp->mh->CLMP_METERCH);
			if((tmp > 0 && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->pa->ADPOSI_OPENEND2)))
				ACT_STEP_NEW(CLAMPOPENFST2);
			else if(tmp <= 0 && MoveLocationNMControlProc(pp->pi->I_CLS_LOW))
			   	ACT_STEP_NEW(CLAMPOPENEND);

			CHK_SET_MOV(SetClampOpenFast);
			break;			
		case CLAMPOPENFST2:
		case CLAMPOPENFST2 + 1:
			if(MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->pa->ADPOSI_OPENEND3))
			{
//				if(pp->pa->ADPOSI_OPENEND4 >pp->pa->ADPOSI_OPENEND3 && pp->pa->ADPOSI_OPENEND4 <= pp->pa->ADPOSI_OPENENDEND && pp->md->FL_OPN4 == 1)
//					ACT_STEP_NEW(CLAMPOPENFST3);
//				else
//					ACT_STEP_NEW(CLAMPOPENEND);
				ACT_STEP_NEW(CLAMPOPENFST3);
			}
			CHK_SET_MOV(SetClampOpenFast2);
			break;
		case CLAMPOPENFST3:
		case CLAMPOPENFST3 + 1:		
			if (pp->md->FL_CLSOPEN_MODE == 3 || pp->md->FL_CLSOPEN_MODE == 4)
			{
				if(MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), (pp->pa->ADPOSI_OPENENDEND - pp->md->AD_POSIDISS)))
				{
					ACT_STEP_NEW(CLAMPOPENEND);
				}
			}
			else
			{
				if(MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->pa->ADPOSI_OPENEND4))
				{
					ACT_STEP_NEW(CLAMPOPENEND);
				}
			}
			CHK_SET_MOV(SetClampOpenFast3);
			break;
		case CLAMPOPENEND:
		case CLAMPOPENEND + 1:
			tmp = GetMeterLenByCh(pp->mh->CLMP_METERCH);

			if (pp->mh->FL_CLAMP_FINE_TUNING == 1)
			{
				if (MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->clampend) || ChkHydrPosMode(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_CLAMPOPN)),HYDRNOPOS))
				{
					ACT_STEP_NEW(END_STEP);
				}
			}
			else
			{
				if((tmp > 0 && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->pa->ADPOSI_OPENENDEND)) ||
			   		(tmp <= 0 && MoveLocationNMControlProc(pp->pi->I_OPN_END)))
				{
					ACT_STEP_NEW(END_STEP);
				}
			}
			
			CHK_SET_MOV(SetClampOpenEnd);
			break;
		case END_STEP:
            if (ACT_STEP_MS > 200 || ChkHydrPosMode(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_CLAMPOPN)), HYDRNOPOS))
            {
    			SET_OFF(ClampOpenOffDelay);
    			ACT_STEP_NEW(END_STEP + 1);

    			if(pp->mh->TMDLY_VLV_OFFOPENEND > 500)  
    				pp->mh->TMDLY_VLV_OFFOPENEND = 0;
    			if(pp->mh->TMDLY_OPENEND > 999)
    				pp->mh->TMDLY_OPENEND = 0;
    			
    			if(pp->mh->TMDLY_OPENEND > pp->mh->TMDLY_VLV_OFFOPENEND)
    				tmp = pp->mh->TMDLY_OPENEND;
    			else
    				tmp = pp->mh->TMDLY_VLV_OFFOPENEND;
    				
    			ActSetMovTM(pact->ptsk, tmp);
            }
			break;
		case END_STEP + 1:	
			tmp = GetMeterLenByCh(pp->mh->CLMP_METERCH);
			if(para == 0)
			{
		   		if ((pp->prbt->md->FL_ROBOTCTRL != 0) &&((pp->allow_robot == 0)||((CUS_ID == CS_HAIXIN) && (ChkOperMode(OM_AUTO)||(pp->qc_pos_opnstart<pp->pa->ADPOSI_OPENENDSLOW)) && (ACT_RUN_MS/10 < pp->md->TM_CLPOPN_ROBOT_PROTECT))))
				{
					RobotClampCloseStart(pp->prbt);
					SET_PART_ERR(pp, ER0_OPNPROT);
					return ACT_ERR_ABORT;
				}
				else
				{
					RobotClampOpenEnd(pp->prbt);
					MoldClampOpenEnd(pp->prbt);
				}
			}
			ACT_STEP_NEW(END_STEP + 2);		
			break;	
		case END_STEP + 2:
			if((ACT_STEP_100US >= ((UI32)pp->mh->TMDLY_OPENEND)*100 && GetPOBit(pp->po->O_OPN_SOL) <= 1 && (CUS_ID != CS_SHIBITUO))
				|| (ACT_STEP_100US >= ((UI32)pp->mh->TMDLY_OPENEND)*100 && GetPOBit(pp->po->O_OP_SETU) <= 1 && (CUS_ID == CS_SHIBITUO)))
			{
				if(para == 0)
				{
//					RobotClampOpenEnd(pp->prbt);
//					MoldClampOpenEnd(pp->prbt);
					ClampLubCount(pp);
					pp->st->TM_CLAMPOPN = ACT_RUN_MS/10;

					if(AutoAlarmCanUpdate())
						pp->st->TM_CLAMPOPN_MAX = pp->st->TM_CLAMPOPN   + (pp->st->TM_CLAMPOPN  >> 1) + 200;

					pp->st->TM_CLAMPOPN_RT = pp->st->TM_CLAMPOPN;
                    pp->st->TM_CLAMPOPN2 = pp->st->TM_CLAMPOPN;
				}
				if(para == 1)
				{
					if (CHK_PART_ERR(pp->psfdr, ER0_CSDR)&&(ChkSysCfg(CO2_EJTM)))/*clamp open after Clamp close task error*/
					{
					    if(!FSafeDoorIsCloseErrChk(pp->psfdr))
                              ph->clampdoorclose=1;
						
						AlarmOff();					
						SetAutoStartProc();
					}
				}
                if(ChkRunFlag(INIF_AUTO1))
                {
					pp->st->ADPOSI_OPNEND = GetMeterPosByCh(pp->mh->CLMP_METERCH);
					if(pp->st->TM_CLAMPOPN == 0)
						pp->st->CLAMP_OPN_SPD = 0;
					else
						pp->st->CLAMP_OPN_SPD = (pp->st->ADPOSI_OPNEND-pp->qc_pos_opnstart)*100/pp->st->TM_CLAMPOPN;
					ClampCmd( pp, CMD_QC_PARA,2);
                }	

                if(ChkLocateDa(pp))
					HYDR_CLS_Da_F(CLAMP_OPN_PID_PORT,0,CLAMP_OPN_PID_DIREC);	

				if (ChkSysCfg(XF4_SAVC2))
				{
					PartDataTx((PPART)pp, ST_ID,1,GET_IND(PCLAMP_STATE, ADPOSI_OPNEND),TRUE);
                    PartDataTx((PPART)pp, ST_ID,1,GET_IND(PCLAMP_STATE, TM_CLAMPOPN2),TRUE);
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
		return ClampOpnBackgrd(pp, pact, para);
}

ACT_RET ClampOpen_p(PACT pact, UI32 para)
{
	PCLAMP pp = (PCLAMP)(pact->part);
	PSAFEDOOR ph = (PSAFEDOOR)GetPartByID(MK_PART(SAFEDOOR_ID, 1));
	ACT_RET ret = ACT_OK;
	int tmp;
	
	if(pp == NULL)
		return ACT_ERR_ABORT;

	if(ActIODelayLimit(pp->po->O_OPN_SOL, TRUE))
	{
		return ACT_ERR_ABORT;
	}
	pp->mh->CLMP_METERCH = DEF_CLAMP_METER_CH;
	switch (pact->step)
	{
		case INIT1_STEP:
			if((ret = ClampOpnPreChk(pp)) != ACT_OK)
				return ret;
			else
			{
				if(!RobotClampPreErrChk(pp->prbt))
				{
					if(pp->mh->TMDLY_OPENBEGIN > 999)
						pp->mh->TMDLY_OPENBEGIN  = 0;
					ACT_STEP_NEW(INIT1_STEP + 1);
					ActSetMovTM(pact->ptsk, pp->mh->TMDLY_OPENBEGIN);
					pp->qc_pos_opnstart = (GetMeterPosByCh(pp->mh->CLMP_METERCH));
					pp->clampend = pp->pa->ADPOSI_OPENENDEND;
				}
			}
			pp->clsopnlaststep =65535;
			pp->clampclserr_opn=0;	//异常开模标记此处清零			

			pp->CoreNum = 0;			
			pp->clpopnmidcorenum = PosMid(pact);
			InitClampTmProStart(pp, ChkAdFwdInplace(pp->mh->CLMP_METERCH));
			PosParaInit_DA(1,pp->hy->POSN_KP_OPN,pp->hy->POSN_KI_OPN,pp->hy->POSN_KD_OPN);
			if(ChkSysCfg(XF3_LHACC)&&(para==1)&&(ChkOperMode(OM_AUTO)))
			{
				pp->clampclserr_opn=1;	//异常开模标记此处置1
			}					
			break;
		case INIT1_STEP + 1:
			if(ACT_STEP_100US > ((UI32)pp->mh->TMDLY_OPENBEGIN)*100)
			{				
				ClampOpenStartInit(pp,pact);
				
				if(pp->md->FL_OPENDRAINFUNCTIO)
				{
					ACT_STEP_NEW(CLAMPOPNRELON);
				}
				else
				{
					if((!TestPI(pp->pi->I_CLS_END))&&(pp->qc_pos_opnstart< (pp->clampend-50)))	
					{
						ACT_STEP_NEW(CLAMPOPNSPC);
					}
					else 
					{
						ACT_STEP_NEW(CLAMPOPENSLOW1);
					}
				}
			}
			if(GetOperMode() == OM_MANUAL)
			{
				if(GetMeterPosByCh(pp->mh->CLMP_METERCH) >= pp->clampend)
				{
					ACT_STEP_NEW(END_STEP);
				}
				else if(GetMeterLenByCh(pp->mh->CLMP_METERCH) == 0 && TestPI(pp->pi->I_OPN_END))
				{
					ACT_STEP_NEW(END_STEP);
				}
			}
			break;			
		case INIT5_STEP:					//2014-10-28  通用韩国机机械手			  
			if(CheckPIUsed(pp->prbt->pi->I_R_OPMD)) 	//优先判断输入点"允许完全开模"	  20170710
			{
				if(TestPI(pp->prbt->pi->I_R_OPMD))
				{
					ACT_STEP_NEW(pp->clsopnlaststep);
					pact->ftaskstat  = 0;
				}
			}
			else
			{
				if(STEP_10MS_CHK(pp->mh->TM_ROBOT_ACT))
				{
					ACT_STEP_NEW(pp->clsopnlaststep);
					pact->ftaskstat  = 0;
				}
			}
			break;
			
		case CLAMPOPNRELON:
		case CLAMPOPNRELON + 1:
			if(STEP_10MS_CHK(pp->md->TM_OPENDRAIN))
				ACT_STEP_NEW(CLAMPOPNRELOFF);

			CHK_SET_MOV(SetClampOpnRelease);
			break;
		case CLAMPOPNRELOFF:
			pp->qc_pos_opnstart = (GetMeterPosByCh(pp->mh->CLMP_METERCH));
			SetClampOpnRelOff(pp, pact);
			ClampOpenStart(pp, pact);
			ACT_START();
			if((pp->qc_pos_opnstart>pp->pa->ADPOSI_OPENENDSLOW)&&(pp->qc_pos_opnstart< (pp->clampend-50))) 
			{
				ACT_STEP_NEW(CLAMPOPNSPC);
			}
			else
			{
				ACT_STEP_NEW(CLAMPOPENSLOW1);
			}
			break;
			
		case CLAMPOPNSPC:
		case CLAMPOPNSPC + 1:
			tmp = GetMeterLenByCh(pp->mh->CLMP_METERCH);
			if(pp->mh->TM_OPNPROTECT<200)
				pp->mh->TM_OPNPROTECT = 200;
			if((tmp > 0 && (MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), (pp->mh->ADPOS_OPNSTART+pp->qc_pos_opnstart)) || MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->pa->ADPOSI_OPENENDEND)))
				||((ACT_RUN_MS>=pp->mh->TM_OPNPROTECT)))
			{
				ACT_STEP_NEW(CLAMPOPENSLOW1);		
			}

			pp->pflowendda= getflowdavalue((UI32)pp->pa->DAFLOW_VLV*FLOWACCURACY*10,CLAMP_VLV_USE);	 //用于启动时手动放开
			CHK_SET_MOV(SetClampOpenSlowSpc);
			break;			
			
		case CLAMPOPENSLOW1:
			if(ClpOpenInHydrPos(pp))
			{
				SET_MOV2(SetClampOpenPos);
			}
		case CLAMPOPENSLOW1 + 1:
			tmp = GetMeterLenByCh(pp->mh->CLMP_METERCH);
			if((tmp > 0 && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->pa->ADPOSI_OPENENDSLOW)) ||
			   (tmp <= 0 && MoveLocationNMControlProc(pp->pi->I_CLS_HIP)))
			{
				pp->st->TM_CLAMPOPN1F = ACT_STEP_10MS;
				ACT_STEP_NEW(CLAMPOPENEND);
			}
			
			pp->pflowendda= getflowdavalue((UI32)pp->pa->DAFLOW_VLV*FLOWACCURACY*10,CLAMP_VLV_USE);	 //用于第一段时手动放开
			CHK_SET_MOV(SetClampOpenSlow);
			break;
	
		case CLAMPOPENEND:
			g_sramp_da.speedlast_da = (((float)pp->pflowendda/4095)*10000);
		case CLAMPOPENEND + 1:
			pp->clampfunc = 1;
			tmp = GetMeterLenByCh(pp->mh->CLMP_METERCH);
			if((tmp > 0 && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->clampend)) 
				|| (tmp <= 0 && MoveLocationNMControlProc(pp->pi->I_OPN_END)))
			{
				ACT_STEP_NEW(END_STEP);
			}
			
			if((pp->CoreNum + 1) > pp->clpopnmidcorenum)
			{
				PospidProc_DA(pp->pa->DAPRES_OPENEND,pp->pa->DAFLOW_OPENEND,pp->clampend,GetMeterPosByCh(pp->mh->CLMP_METERCH),0x01,pp->mh->DAPRES_SPDOPNRAMP1*10,pp->mh->DAFLOW_SPDOPNRAMP3*10,pp->mh->DAPRESS_OPNPOSPIDEND,1);				
			}
			else
			{
				if(pp->CoreNum == 0)
				{
					PospidProc_DA(pp->pa->DAPRES_OPENEND,pp->pa->DAFLOW_OPENEND,pp->corebwd[pp->CoreNum],GetMeterPosByCh(pp->mh->CLMP_METERCH),0x01,pp->mh->DAPRES_SPDOPNRAMP1*10,pp->mh->DAFLOW_SPDOPNRAMP3*10,pp->mh->DAPRESS_OPNPOSPIDEND,1);
				}
				else
				{
					PospidProc_DA(pp->mh->DAPRES_OPNSETUP,pp->mh->DAFLOW_OPNSETUP,(pp->corebwd[pp->CoreNum]+10),GetMeterPosByCh(pp->mh->CLMP_METERCH),0x01,pp->mh->DAPRES_SPDOPNRAMP1*10,pp->mh->DAFLOW_SPDOPNRAMP3*10,pp->mh->DAPRESS_OPNPOSPIDEND,1);
				}
			} 

			tmp = GetMeterLenByCh(pp->mh->CLMP_METERCH);
			if((pp->CoreNum + 1) > pp->clpopnmidcorenum)
			{
				if (MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->clampend) || ChkHydrPosMode(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_CLAMPOPN)),HYDRNOPOS))
				{
					ACT_STEP_NEW(END_STEP);
					pp->clampfunc = 0;
				}
			}
			else
			{
				if((tmp > 0 && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->corebwd[pp->CoreNum])) 
					|| (tmp <= 0 && MoveLocationNMControlProc(pp->pi->I_OPN_END)))
				{
					pp->CoreNum++; 
					pp->clampfunc = 0;
					ACT_STEP_NEW(CLAMPOPNRELOFF);
				}	
			}
			
			CHK_SET_MOV(SetClampOpenEnd);
			break;

		case END_STEP:
			ActSet_Da_flow(pact, MC_CTR_MODE_DA, 99-pp->pflowend, 0,0,
					0,CLAMP_OPN_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPOPN),CLAMP_OPN_P_DIREC);		
			SET_OFF(ClampOpenOffDelay);
			ACT_STEP_NEW(END_STEP + 1);
			break;
			
		case END_STEP + 1:	
			ActSet_Da_flow(pact, MC_CTR_MODE_DA, 99-pp->pflowend, 0,0,
					0,CLAMP_OPN_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPOPN),CLAMP_OPN_P_DIREC);		
			tmp = GetMeterLenByCh(pp->mh->CLMP_METERCH);

			if(para == 0)
			{
		   		if ((pp->prbt->md->FL_ROBOTCTRL != 0) &&((pp->allow_robot == 0)||((CUS_ID == CS_HAIXIN) && (ChkOperMode(OM_AUTO)||(pp->qc_pos_opnstart<pp->pa->ADPOSI_OPENENDSLOW)) && (ACT_RUN_MS/10 < pp->md->TM_CLPOPN_ROBOT_PROTECT))))
				{
					RobotClampCloseStart(pp->prbt);
					SET_PART_ERR(pp, ER0_OPNPROT);
					return ACT_ERR_ABORT;
				}
				else
				{
					RobotClampOpenEnd(pp->prbt);
					MoldClampOpenEnd(pp->prbt);
				}
			}
			ACT_STEP_NEW(END_STEP + 2);
			if(pp->mh->TMDLY_OPENEND > pp->mh->TMDLY_VLV_OFFOPENEND)
				tmp = pp->mh->TMDLY_OPENEND;
			else
				tmp = pp->mh->TMDLY_VLV_OFFOPENEND;
			ActSetMovTM(pact->ptsk, tmp);
			break;
			
		case END_STEP + 2:
			if(ACT_STEP_100US >= ((UI32)pp->mh->TMDLY_VLV_OFFOPENEND)*100)
			{
				HYDR_CLS_Da_F(CLAMP_OPN_PID_PORT,0,CLAMP_OPN_P_DIREC);	
				ACT_STEP_NEW(END_STEP + 3);
			}
			else
			{	
				ActSet_Da_flow(pact, MC_CTR_MODE_DA, 99-pp->pflowend, 0,0,
					0,CLAMP_OPN_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPOPN),CLAMP_OPN_P_DIREC);		
			}
			break;
			
		case END_STEP + 3:
			if((ACT_STEP_100US >= ((UI32)pp->mh->TMDLY_OPENEND)*100 && GetPOBit(pp->po->O_OPN_SOL) <= 1 && (CUS_ID != CS_SHIBITUO))
				|| (ACT_STEP_100US >= ((UI32)pp->mh->TMDLY_OPENEND)*100 && GetPOBit(pp->po->O_OP_SETU) <= 1 && (CUS_ID == CS_SHIBITUO)))
			{
				if(para == 0)
				{
					ClampLubCount(pp);
					pp->st->TM_CLAMPOPN = ACT_RUN_MS/10;

					if(AutoAlarmCanUpdate())
						pp->st->TM_CLAMPOPN_MAX = pp->st->TM_CLAMPOPN	+ (pp->st->TM_CLAMPOPN	>> 1) + 200;

					pp->st->TM_CLAMPOPN_RT = pp->st->TM_CLAMPOPN;
                    pp->st->TM_CLAMPOPN2 = pp->st->TM_CLAMPOPN;
				}
				if(para == 1)
				{
					if (CHK_PART_ERR(pp->psfdr, ER0_CSDR)&&(ChkSysCfg(CO2_EJTM)))/*clamp open after Clamp close task error*/
					{
						if(!FSafeDoorIsCloseErrChk(pp->psfdr))
							  ph->clampdoorclose=1;
						
						AlarmOff(); 				
						SetAutoStartProc();
					}
				}
				if(ChkRunFlag(INIF_AUTO1))
				{
					pp->st->ADPOSI_OPNEND = GetMeterPosByCh(pp->mh->CLMP_METERCH);
					if(pp->st->TM_CLAMPOPN == 0)
						pp->st->CLAMP_OPN_SPD = 0;
					else
						pp->st->CLAMP_OPN_SPD = (pp->st->ADPOSI_OPNEND-pp->qc_pos_opnstart)*100/pp->st->TM_CLAMPOPN;
					ClampCmd( pp, CMD_QC_PARA,2);
				}		

				if (ChkSysCfg(XF4_SAVC2))
				{
					PartDataTx((PPART)pp, ST_ID,1,GET_IND(PCLAMP_STATE, ADPOSI_OPNEND),TRUE);
                    PartDataTx((PPART)pp, ST_ID,1,GET_IND(PCLAMP_STATE, TM_CLAMPOPN2),TRUE);
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
		return ClampOpnBackgrd(pp, pact, para);
}


ACT_RET ClampOpen_pid(PACT pact, UI32 para)
{
	PCLAMP pp = (PCLAMP)(pact->part);
    PSAFEDOOR ph = (PSAFEDOOR)GetPartByID(MK_PART(SAFEDOOR_ID, 1));
	PTEMPCTRLCOMM ptmp = (PTEMPCTRLCOMM)GetPartByID(MK_PART(TEMPCTRLCOMM_ID, 1));
	ACT_RET ret = ACT_OK;
	int tmp;
    UI16 vlvk;
	
	if(pp == NULL)
		return ACT_ERR_ABORT;

	if(ActIODelayLimit(pp->po->O_OPN_SOL, TRUE))
	{
		return ACT_ERR_ABORT;
	}

	pp->mh->CLMP_METERCH = DEF_CLAMP_METER_CH;
	switch (pact->step)
	{
		case INIT1_STEP:
			if((ret = ClampOpnPreChk(pp)) != ACT_OK)
				return ret;
			else
			{
				if(!RobotClampPreErrChk(pp->prbt))
				{
					if(pp->mh->TMDLY_OPENBEGIN > 999)
						pp->mh->TMDLY_OPENBEGIN  = 0;
					ACT_STEP_NEW(INIT1_STEP + 1);
					ActSetMovTM(pact->ptsk, pp->mh->TMDLY_OPENBEGIN);
					pp->qc_pos_opnstart = (GetMeterPosByCh(pp->mh->CLMP_METERCH));
					pp->clampend = pp->pa->ADPOS_OPENSPD3;
				}
			}
			pp->clsopnlaststep =65535;
            pp->clampclserr_opn=0;	//异常开模标记此处清零
			pp->CoreNum = 0;		
            pp->pflowendda = 0;
			pp->clpopnmidcorenum = PosMid(pact);
			PosParaInit_DA(1,pp->hy->POSN_KP_OPN,pp->hy->POSN_KI_OPN,pp->hy->POSN_KD_OPN);
			InitClampTmProStart(pp, ChkAdFwdInplace(pp->mh->CLMP_METERCH));
			if(ChkSysCfg(XF3_LHACC)&&(para==1)&&(ChkOperMode(OM_AUTO)))
			{
				pp->clampclserr_opn=1;  //异常开模标记此处置1
			}
			break;
		case INIT1_STEP + 1:
			if(ACT_STEP_100US > ((UI32)pp->mh->TMDLY_OPENBEGIN)*100)
			{
				if(pp->md->FL_OPENDRAINFUNCTIO)
				{
					ACT_STEP_NEW(CLAMPOPNRELON);
				}
				else
				{
					if((pp->qc_pos_opnstart>pp->pa->ADPOS_OPENSPD1)&&(pp->qc_pos_opnstart< (pp->clampend-50)))	
					{
						ACT_STEP_NEW(CLAMPOPNSPC);
					}
					else 
						ACT_STEP_NEW(CLAMPOPENSLOW1);
				}
			}
			if(GetOperMode() == OM_MANUAL)
			{
				if(GetMeterPosByCh(pp->mh->CLMP_METERCH) >= pp->clampend)
				{
					ACT_STEP_NEW(END_STEP);
				}
				else if(GetMeterLenByCh(pp->mh->CLMP_METERCH) == 0 && TestPI(pp->pi->I_OPN_END))
				{
					ACT_STEP_NEW(END_STEP);
				}
			}
			break;
		case INIT5_STEP:	 
			if(STEP_10MS_CHK(pp->mh->TM_ROBOT_ACT))
			{
				ACT_STEP_NEW(pp->clsopnlaststep);
				pact->ftaskstat  = 0;
			}
			break;
			
		case CLAMPOPNRELON:
		case CLAMPOPNRELON + 1:
			if(STEP_10MS_CHK(pp->md->TM_OPENDRAIN))
				ACT_STEP_NEW(CLAMPOPNRELOFF);

			CHK_SET_MOV(SetClampOpnRelease);
			break;

		case CLAMPOPNRELOFF:
			pp->qc_pos_opnstart = (GetMeterPosByCh(pp->mh->CLMP_METERCH));
			SetClampOpnRelOff(pp, pact);
			ClampOpenStart(pp, pact);
			ACT_START();
			if((pp->qc_pos_opnstart>pp->pa->ADPOS_OPENSPD1)&&(pp->qc_pos_opnstart< (pp->clampend-50)))	
			{
				ACT_STEP_NEW(CLAMPOPNSPC);
			}
			else
				ACT_STEP_NEW(CLAMPOPENSLOW1);
			break;

		case CLAMPOPNSPC:
		case CLAMPOPNSPC + 1:		   
			tmp = GetMeterLenByCh(pp->mh->CLMP_METERCH);
			if(pp->mh->TM_OPNPROTECT <= 200)
				pp->mh->TM_OPNPROTECT = 200;
			if((tmp > 0 && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), (pp->mh->ADPOS_OPNSTART+pp->qc_pos_opnstart)))
				||((ACT_RUN_MS>=pp->mh->TM_OPNPROTECT)))
			{
				ACT_STEP_NEW(CLAMPOPENSLOW1);			
			}

			//当离终止位置很接近时，此处可以屏蔽使用途中启动功能，防止距离拉的过远   20200102
			if((pp->CoreNum + 1) > pp->clpopnmidcorenum)	//无途中中子功能时
			{
				if(tmp > 0 && (pp->pflowendda >= PospidProc_DASPC(pp->pa->DAPRES_OPENSPD2,pp->pa->DAFLOW_OPENSPD2,pp->clampend,GetMeterPosByCh(pp->mh->CLMP_METERCH),0x01,pp->mh->DAFLOW_SPDOPNRAMP2*10)))
				{
				
					ACT_STEP_NEW(CLAMPOPENSLOW1);			
				}
			}
			else
			{
				if(tmp > 0 && (pp->pflowendda >= PospidProc_DASPC(pp->pa->DAPRES_OPENSPD2,pp->pa->DAFLOW_OPENSPD2,pp->corebwd[pp->CoreNum],GetMeterPosByCh(pp->mh->CLMP_METERCH),0x01,pp->mh->DAFLOW_SPDOPNRAMP2*10)))
				{
					ACT_STEP_NEW(CLAMPOPENSLOW1);			
				}
			} 
			pp->pflowendda= getflowdavalue((UI32)pp->mh->DAPFLOW_OPNMIDSTART*FLOWACCURACY*10,CLAMP_VLV_USE);	 //用于启动时手动放开
			CHK_SET_MOV(SetClampOpenSlowSpc);  
			break;
			
		case CLAMPOPENSLOW1:
		case CLAMPOPENSLOW1 + 1:
			tmp = GetMeterLenByCh(pp->mh->CLMP_METERCH);
			if((tmp > 0 && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->pa->ADPOS_OPENSPD1)) 
				|| (tmp <= 0 && MoveLocationNMControlProc(pp->pi->I_CLS_HIP)))
			{
				{
					pp->st->TM_CLAMPOPN1F = ACT_STEP_10MS;
					ACT_STEP_NEW(CLAMPOPENFST3);		
				}
			}			
			if((pp->CoreNum + 1) > pp->clpopnmidcorenum)
			{				 
			}
			else
			{			  
				if(tmp > 0 && pp->corebwd[pp->CoreNum] <= pp->pa->ADPOS_OPENSPD1 && (pp->pflowendda >= PospidProc_DASPC(pp->pa->DAPRES_OPENSPD1,pp->pa->DAFLOW_OPENSPD1,pp->corebwd[pp->CoreNum],GetMeterPosByCh(pp->mh->CLMP_METERCH),0x01,pp->mh->DAPRES_OPNRAMP*10)))
				{					  
					PospidProc_DA(pp->pa->DAPRES_OPENSPD1,pp->pa->DAFLOW_OPENSPD1,pp->corebwd[pp->CoreNum],GetMeterPosByCh(pp->mh->CLMP_METERCH),0x01,pp->mh->DAPRES_SPDOPNRAMP1*10,pp->mh->DAFLOW_SPDOPNRAMP3*10,pp->mh->DAPRESS_OPNPOSPIDEND,1);
					if(tmp > 0 && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->corebwd[pp->CoreNum]))
					{
						pp->CoreNum++; 
						ACT_STEP_NEW(CLAMPOPNRELOFF);
					}						 
				}
			}

            if(pp->pa->DAFLOW_VLV == 0 || pp->pa->DAFLOW_VLV >99)
    			vlvk = pp->pa->DAFLOW_OPENSPD1;
    		else
    			vlvk = pp->pa->DAFLOW_VLV;
            
			pp->pflowendda= getflowdavalue((UI32)vlvk*FLOWACCURACY*10,CLAMP_VLV_USE);	 //用于第一段时手动放开
			CHK_SET_MOV(SetClampOpenSlow);
			break;

		case CLAMPOPENFST3:
			g_sramp_da.speedlast_da = (((float)pp->pflowendda/4095)*10000);
		case CLAMPOPENFST3 + 1:
			pp->clampfunc = 1;
			pp->clampfunc_sv = 1;
			if((pp->CoreNum + 1) > pp->clpopnmidcorenum)
			{
				AdposServoRampDownUp(pp->pa->DAPRES_OPENSPD2, pp->pa->DAFLOW_OPENSPD2, pp->mh->DAPRESS_OPNPOSPIDEND, pp->pa->DAFLOW_OPENSPD2, pp->pa->ADPOS_OPENSPD1, pp->clampend, GetMeterPosByCh(pp->mh->CLMP_METERCH)); 
				PospidProc_DA(pp->pa->DAPRES_OPENSPD2,pp->pa->DAFLOW_OPENSPD2,pp->clampend,GetMeterPosByCh(pp->mh->CLMP_METERCH),0x01,pp->mh->DAPRES_SPDOPNRAMP1*10,pp->mh->DAFLOW_SPDOPNRAMP3*10,pp->mh->DAPRESS_OPNPOSPIDEND,1);				
			}
			else
			{
				if(pp->CoreNum == 0)
				{
					AdposServoRampDownUp(pp->pa->DAPRES_OPENSPD2, pp->pa->DAFLOW_OPENSPD2, pp->mh->DAPRESS_OPNPOSPIDEND, pp->pa->DAFLOW_OPENSPD2, pp->pa->ADPOS_OPENSPD1, pp->corebwd[pp->CoreNum], GetMeterPosByCh(pp->mh->CLMP_METERCH)); 
					PospidProc_DA(pp->pa->DAPRES_OPENSPD2,pp->pa->DAFLOW_OPENSPD2,pp->corebwd[pp->CoreNum],GetMeterPosByCh(pp->mh->CLMP_METERCH),0x01,pp->mh->DAPRES_SPDOPNRAMP1*10,pp->mh->DAFLOW_SPDOPNRAMP3*10,pp->mh->DAPRESS_OPNPOSPIDEND,1);
				}
				else
				{
					AdposServoRampDownUp(pp->pa->DAPRES_OPENSPD2, pp->pa->DAFLOW_OPENSPD2, pp->mh->DAPRESS_OPNPOSPIDEND, pp->pa->DAFLOW_OPENSPD2, pp->corebwd[pp->CoreNum - 1], pp->corebwd[pp->CoreNum], GetMeterPosByCh(pp->mh->CLMP_METERCH));								  
					PospidProc_DA(pp->mh->DAPRES_OPNSETUP,pp->mh->DAFLOW_OPNSETUP,(pp->corebwd[pp->CoreNum]+10),GetMeterPosByCh(pp->mh->CLMP_METERCH),0x01,pp->mh->DAPRES_SPDOPNRAMP1*10,pp->mh->DAFLOW_SPDOPNRAMP3*10,pp->mh->DAPRESS_OPNPOSPIDEND,1);
				}
			} 

			tmp = GetMeterLenByCh(pp->mh->CLMP_METERCH);
			if((pp->CoreNum + 1) > pp->clpopnmidcorenum)
			{
				if(MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->clampend))
				{
					ACT_STEP_NEW(END_STEP);
					pp->clampfunc = 0;
					pp->clampfunc_sv = 0;
				}
			}
			else
			{
				if((tmp > 0 && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->corebwd[pp->CoreNum])) ||
					(tmp <= 0 && MoveLocationNMControlProc(pp->pi->I_OPN_END)))
				{
					pp->CoreNum++; 
					pp->clampfunc = 0;
					pp->clampfunc_sv = 0;
					ACT_STEP_NEW(CLAMPOPNRELOFF);
				}	
			}
			CHK_SET_MOV(SetClampOpenFast3); 
			break;
			
		case END_STEP:
			ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->pflowend, 0,0,
					0,CLAMP_OPN_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPOPN),CLAMP_OPN_PID_DIREC);		
			SET_OFF(ClampOpenOffDelay);
			ACT_STEP_NEW(END_STEP + 1);
			break;
			
		case END_STEP + 1:	
			ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->pflowend, 0,0,
					0,CLAMP_OPN_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPOPN),CLAMP_OPN_PID_DIREC);		
			tmp = GetMeterLenByCh(pp->mh->CLMP_METERCH);

			if(para == 0)
			{
		   		if ((pp->prbt->md->FL_ROBOTCTRL != 0) &&((pp->allow_robot == 0)||((CUS_ID == CS_HAIXIN) && (ChkOperMode(OM_AUTO)||(pp->qc_pos_opnstart<pp->pa->ADPOSI_OPENENDSLOW)) && (ACT_RUN_MS/10 < pp->md->TM_CLPOPN_ROBOT_PROTECT))))
				{
					RobotClampCloseStart(pp->prbt);
					SET_PART_ERR(pp, ER0_OPNPROT);
					return ACT_ERR_ABORT;
				}
				else
				{
					RobotClampOpenEnd(pp->prbt);
					MoldClampOpenEnd(pp->prbt);
				}
			}
			ACT_STEP_NEW(END_STEP + 2);
			if(pp->mh->TMDLY_OPENEND > pp->mh->TMDLY_VLV_OFFOPENEND)
				tmp = pp->mh->TMDLY_OPENEND;
			else
				tmp = pp->mh->TMDLY_VLV_OFFOPENEND;
			ActSetMovTM(pact->ptsk, tmp);
			break;
			
		case END_STEP + 2:
			if(ACT_STEP_100US >= ((UI32)pp->mh->TMDLY_VLV_OFFOPENEND)*100)
			{
				HYDR_CLS_Da_F(CLAMP_OPN_PID_PORT,0,CLAMP_OPN_PID_DIREC);	
				ACT_STEP_NEW(END_STEP + 3);
			}
			else
			{	
				ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->pflowend, 0,0,
					0,CLAMP_OPN_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPOPN),CLAMP_OPN_PID_DIREC);		
			}
			break;
			
		case END_STEP + 3:
			if((ACT_STEP_100US >= ((UI32)pp->mh->TMDLY_OPENEND)*100 && GetPOBit(pp->po->O_OPN_SOL) <= 1 && (CUS_ID != CS_SHIBITUO))
				|| (ACT_STEP_100US >= ((UI32)pp->mh->TMDLY_OPENEND)*100 && GetPOBit(pp->po->O_OP_SETU) <= 1 && (CUS_ID == CS_SHIBITUO)))
			{
				if(para == 0)
				{
					//RobotClampOpenEnd(pp->prbt);
					ClampLubCount(pp);
					pp->st->TM_CLAMPOPN = ACT_RUN_MS/10;
					pp->st->TM_CLAMPOPN_MAX = pp->st->TM_CLAMPOPN	+ (pp->st->TM_CLAMPOPN	>> 1) + 200;
					pp->st->TM_CLAMPOPN_RT = pp->st->TM_CLAMPOPN;
                    pp->st->TM_CLAMPOPN2 = pp->st->TM_CLAMPOPN;
				}
				if(para == 1)
				{
					if (CHK_PART_ERR(pp->psfdr, ER0_CSDR)&&(ChkSysCfg(CO2_EJTM)))/*clamp open after Clamp close task error*/
					{
					    if(!FSafeDoorIsCloseErrChk(pp->psfdr))
                              ph->clampdoorclose=1;
						
						AlarmOff();					
						SetAutoStartProc();
					}
				}
				if(ChkRunFlag(INIF_AUTO1))
				{
					pp->st->ADPOSI_OPNEND = GetMeterPosByCh(pp->mh->CLMP_METERCH);
					if(pp->st->TM_CLAMPOPN == 0)
						pp->st->CLAMP_OPN_SPD = 0;
					else
						pp->st->CLAMP_OPN_SPD = (pp->st->ADPOSI_OPNEND-pp->qc_pos_opnstart)*100/pp->st->TM_CLAMPOPN;
				}	

				if (ChkSysCfg(XF4_SAVC2))
				{
					PartDataTx((PPART)pp, ST_ID,1,GET_IND(PCLAMP_STATE, ADPOSI_OPNEND),TRUE);
                    PartDataTx((PPART)pp, ST_ID,1,GET_IND(PCLAMP_STATE, TM_CLAMPOPN2),TRUE);
				}
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
		return ClampOpnBackgrd(pp, pact, para);
}

void ClampOpenOffVlv(PCLAMP pp)
{	
	if(pp->mh->TMDLY_VLV_OFFOPENEND > 500)  
		pp->mh->TMDLY_VLV_OFFOPENEND = 0;
	
	SetPOBit(pp->po->O_MEC_SAF_AIR, PC_OFF);
	SetPOBitDelay(pp->po->O_OPN_SOL, PC_OFF, pp->mh->TMDLY_VLV_OFFOPENEND);
	
	CLampOpenOffVlv1(pp);
}

void ClampOpenOff(PCLAMP pp, PACT pact)
{
	if(pp != NULL && pact != NULL)
	{
	    extern HYDR g_hydrout;
        g_hydrout.clopnrel = 1;
		pp->clampfunc = 0;
		pp->clampfunc_sv = 0;
		WORD smooth = FALSE;
		
		if(pp->md->FL_CLSOPEN_MODE == 2)
		{
		    HYDR_CLS(0, 0);	
			HYDR_CLS_Da_F(CLAMP_OPN_PID_PORT,pp->mh->DAPFLOW_OPNMIDEND_RAMPOFF,CLAMP_OPN_PID_DIREC);
		}
		else if(ClpOpenInHydrPos(pp))
		{		
			if(!ChkSysCfg(XF1_CHRLT))
			{
				if((smooth = SmoothStop(pact->last_hydr_port,DIRECFWD,pp->mh->CLMP_METERCH,pp->spdramp_opn_manual[CLP_RAMP_OPN_MANUALEND],(ACT_OP_FUNC_NEW)ClampOpenOffVlv, pact)) == FALSE)
				{
					HYDR_CLS(pp->pressramp_opn[CLP_RAMP_OPN_END]/10, pp->spdramp_opn_manual[CLP_RAMP_OPN_MANUALEND]/10);
				}
			}
			else
			{
				HYDR_CLS(pp->pressramp_opn[CLP_RAMP_OPN_END]/10, pp->spdramp_opn_manual[CLP_RAMP_OPN_MANUALEND]/10);
			}

			if(pp->md->FL_CLSOPEN_MODE == 4)
			{
				HYDR_CLS_Da_F(CLAMP_OPN_PID_PORT,0,CLAMP_OPN_P_DIREC);
			}
		}
		else
		{
			HYDR_CLS(pp->pressramp_opn[CLP_RAMP_OPN_END]/10, pp->spdramp_opn[CLP_RAMP_OPN_END]/10);
		}

        if(ChkLocateDa(pp))
			HYDR_CLS_Da_F(CLAMP_OPN_PID_PORT,100,CLAMP_OPN_PID_DIREC);	
        
		if(!smooth)
		{
			ClampOpenOffVlv(pp);
		}			
	}
}

void ClampOpenOffME(PCLAMP pp, PACT pact)
{
	if(pp != NULL && pact != NULL)
	{
	    extern HYDR g_hydrout;
        g_hydrout.clopnrel = 1;
		pp->clampfunc = 0;
		pp->clampfunc_sv = 0;
		WORD smooth = FALSE;
		
		if(pp->md->FL_CLSOPEN_MODE == 2)
		{
		    HYDR_CLS(0, 0);	
			HYDR_CLS_Da_F(CLAMP_OPN_PID_PORT,pp->mh->DAPFLOW_OPNMIDEND_RAMPOFF,CLAMP_OPN_PID_DIREC);
		}
		else if(ClpOpenInHydrPos(pp))
		{		
			if(!ChkSysCfg(XF1_CHRLT))
			{
				if((smooth = SmoothStop(pact->last_hydr_port,DIRECFWD,pp->mh->CLMP_METERCH,pp->spdramp_opn_manual[CLP_RAMP_OPN_MANUALEND],(ACT_OP_FUNC_NEW)ClampOpenOffVlv, pact)) == FALSE)
				{
					HYDR_CLS(pp->pressramp_opn[CLP_RAMP_OPN_END]/10, pp->spdramp_opn_manual[CLP_RAMP_OPN_MANUALEND]/10);
				}
			}
			else
			{
				HYDR_CLS(pp->pressramp_opn[CLP_RAMP_OPN_END]/10, pp->spdramp_opn_manual[CLP_RAMP_OPN_MANUALEND]/10);
			}

			if(pp->md->FL_CLSOPEN_MODE == 4)
			{
				HYDR_CLS_Da_F(CLAMP_OPN_PID_PORT,0,CLAMP_OPN_P_DIREC);
			}
		}
		else
		{
			HYDR_CLS(pp->pressramp_opn[CLP_RAMP_OPN_END]/10, pp->spdramp_opn[CLP_RAMP_OPN_END]/10);
		}

        if(ChkLocateDa(pp))
			HYDR_CLS_Da_F(CLAMP_OPN_PID_PORT,100,CLAMP_OPN_PID_DIREC);	
        
		if(!smooth)
		{
			ClampOpenOffVlv(pp);
		}			
	}

	//开模手动放开停止及开模错误时停止动作，需要停止机械手点输出。
	RobotClampCloseStart(pp->prbt);
}


void SetClampOpnAdj(PCLAMP pp, PACT pact)
{
	UI16 hydelay;
	if(pp->mh->CLMPOPN_HYDRDELAY > 9999)
		pp->mh->CLMPOPN_HYDRDELAY = 0;
	hydelay = pp->mh->CLMPOPN_HYDRDELAY;
	ClampOpnSlowVlv(pp);	
	
	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->mh->DAPRES_OPNSETUP,pp->mh->DAFLOW_OPNSETUP,pp->pressramp_opn[CLP_RAMP_OPN_START]/10,pp->spdramp_opn_manual[CLP_RAMP_OPN_MANUALEND]/10,pp->pressramp_opn[CLP_RAMP_OPN_START]/10,pp->spdramp_opn[CLP_RAMP_OPN_START]/10,
	                	HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_CLAMPOPN),pp->hy->HYDR_PUMPS_OPNSETUP,pp->hy->HYDR_PUMPS_OPNSETUP,
						pp->hy->SERVO_KP_OPN,pp->hy->SERVO_KI_OPN, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPOPN),pp->md->CLPOPN_SPD_UTI); 
	if(pp->md->FL_CLSOPEN_MODE == 2 || ChkLocateDa(pp))
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->mh->DAFLOW_OPNSETUP, 0,0,
					0,CLAMP_OPN_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPOPN),CLAMP_OPN_PID_DIREC);
}

ACT_RET ClampOpnAdj(PACT pact, UI32 para)
{
	PCLAMP	pp = (PCLAMP)(pact->part);
	ACT_RET ret = ACT_OK;
	int	tmp;

	if(pp == NULL)
		return ACT_ERR_ABORT;

	if(!ChkOperMode(OM_ADJU))
		return ACT_ERR_ABORT;

	if(ActIODelayLimit(pp->po->O_OPN_SOL, TRUE))
	{
	    return ACT_ERR_ABORT;
	}

	switch (pact->step)
	{
		case INIT1_STEP:
			if((CheckPI(pp->pi->I_CLS_UPGA) == 0))
			{
				SET_PART_ERR(pp, ER0_UPGAERR);
				return ACT_ERR_ABORT;
			}

			ACT_STEP_NEW(CLAMPOPENSLOW1);
			break;			
		case CLAMPOPENSLOW1:
		case CLAMPOPENSLOW1 + 1:
			if(!ChkSysCfg(XF5_CLAMPCLS))
			{
				tmp = GetMeterLenByCh(pp->mh->CLMP_METERCH);
				if ((tmp > 0 && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->pa->ADPOSI_OPENENDEND))||
				  (tmp <= 0 && MoveLocationNMControlProc(pp->pi->I_OPN_END)))
					ACT_STEP_NEW(END_STEP);
			}
	
			CHK_SET_MOV(SetClampOpnAdj);
			break;  
			
		case END_STEP:
			SET_OFF(ClampOpenOff);
			ret = ACT_END;
			break;

		default:
			ret = ACT_END;
			break;
	}
	if(ret == ACT_END || ret == ACT_ERR_ABORT)
		return ret;
	else
		return ClampOpnBackgrd2(pp, pact);
}  

/*自动调模开模*/
ACT_RET AutoAdjClampOpn(PACT pact, UI32 para)
{
	PCLAMP pp = (PCLAMP)(pact->part);
	ACT_RET ret = ACT_OK;
    int	tmp;
	static	int AutoAdjOpenPosEnd;

	if(pp == NULL)
		return ACT_ERR_ABORT;
	switch (pact->step)
	{
		case INIT1_STEP:
		    ACT_STEP_NEW(CLAMPOPNAUTOADJ);
		case CLAMPOPNAUTOADJ:
		case CLAMPOPNAUTOADJ+1:
			tmp = GetMeterLenByCh(pp->mh->CLMP_METERCH);
			if(ChkSysCfg(XF1_CLPZE))
			{
				AutoAdjOpenPosEnd = 1000;   //此处先暂时用100.0mm，不适用于有推力座机器，待完善
			}
			else
			{
				if(pp->md->FL_CLSOPEN_MODE == 2)
				{
					AutoAdjOpenPosEnd = pp->pa->ADPOS_OPENSPD1;
				}
				else
				{
					AutoAdjOpenPosEnd = pp->pa->ADPOSI_OPENENDSLOW;
				}				
			}
			if((tmp > 0 && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), AutoAdjOpenPosEnd)))
			   	ACT_STEP_NEW(END_STEP);
			CHK_SET_MOV(SetClampOpenSlow);			
			break;
		case END_STEP:
			SET_OFF(ClampOpenOff);
			ret = ACT_END;
			break;
	}
	if(ret == ACT_END || ret == ACT_ERR_ABORT)
		return ret;
	else
		return ClampOpnBackgrd2(pp, pact);	
}


/*-----------------------------------------------------------------------------------+
|      排气开模                                                                      |
+-----------------------------------------------------------------------------------*/
ACT_RET ClampOpnExhaust(PACT pact, UI32 para)
{
	PCLAMP pp = (PCLAMP)(pact->part);
	ACT_RET ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;
	
	if(!ChkOperMode(OM_AUTO))
		return ACT_END;
		
	switch (pact->step)
	{
		case INIT1_STEP:
			ACT_STEP_NEW(CLAMPOPENSLOW1);
			break;
			
		case CLAMPOPENSLOW1:
		case CLAMPOPENSLOW1 + 1:
            if(pp->md->FL_EXHAUSTMODE==0||(pp->md->FL_EXHAUSTMODE==7))
			{
				if(((para==EXHAST_CNT_1)&&(MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->md->ADPOSI_EXHAUSTOPEN) ||
				  STEP_10MS_CHK(pp->md->TM_EXHAUSTOPEN)))||
				  ((para==EXHAST_CNT_2)&&(MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->md->ADPOSI_EXHAUSTOPEN2) ||
				  STEP_10MS_CHK(pp->md->TM_EXHAUSTOPEN2)))||
				  ((para==EXHAST_CNT_3)&&(MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->md->ADPOSI_EXHAUSTOPEN3) ||
				  STEP_10MS_CHK(pp->md->TM_EXHAUSTOPEN3)))||
				  ((para==EXHAST_CNT_4)&&(MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->md->ADPOSI_EXHAUSTOPEN4) ||
				  STEP_10MS_CHK(pp->md->TM_EXHAUSTOPEN4))))
			        ACT_STEP_NEW(END_STEP);
			}
            else if(pp->md->FL_EXHAUSTMODE == 9)
			{
				if(((para==EXHAST_INJEND)&&(MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->md->ADPOSI_EXHAUSTOPENAFTINJ) ||
				  STEP_10MS_CHK(pp->md->TM_EXHAUSTOPENAFTINJ)))||
				  ((para==EXHAST_HOLD1)&&(MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->md->ADPOSI_EXHAUSTOPENAFTHOLD1) ||
				  STEP_10MS_CHK(pp->md->TM_EXHAUSTOPENAFTHOLD1)))||
				  ((para==EXHAST_HOLD2)&&(MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->md->ADPOSI_EXHAUSTOPENAFTHOLD2) ||
				  STEP_10MS_CHK(pp->md->TM_EXHAUSTOPENAFTHOLD2)))||
				  ((para==EXHAST_HOLDEND)&&(MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->md->ADPOSI_EXHAUSTOPENAFTHOLD) ||
				  STEP_10MS_CHK(pp->md->TM_EXHAUSTOPENAFTHOLD))))
					ACT_STEP_NEW(END_STEP);
			}
            else
            {
    			if(MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->md->ADPOSI_EXHAUSTOPEN) 
    				|| STEP_10MS_CHK(pp->md->TM_EXHAUSTOPEN))
    				ACT_STEP_NEW(END_STEP);
            }
			
			CHK_SET_MOV(SetClampOpenSlow);
			break;

		case END_STEP:
			SET_OFF(ClampOpenOff);
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
		return ClampOpnBackgrd2(pp, pact);
}

/*-----------------------------------------------------------------------------------+
|     异常时开模                                                                      |
+-----------------------------------------------------------------------------------*/
ACT_RET ClampOpnERR(PACT pact, UI32 para)
{
	PCLAMP pp = (PCLAMP)(pact->part);
	ACT_RET ret = ACT_OK;
	int	tmp;
	
	if(pp == NULL)
		return ACT_ERR_ABORT;
		
	switch (pact->step)
	{
		case INIT1_STEP:
			if(pp->mh->TM_SCLAPMCLS!=0)
			{
				ForkAct(pp->act + OS_ERRPRS- 1, SUSPEND_FORK, pact->ptsk, 0, 0);
			}
			ACT_STEP_NEW(INIT2_STEP);
			break;
			
		case INIT2_STEP:
		case INIT2_STEP + 1:
			tmp = GetMeterLenByCh(pp->mh->CLMP_METERCH);
			if((tmp > 0 && MoveLocationMeterControlProc(GetMeterPosByCh(pp->mh->CLMP_METERCH), pp->pa->ADPOSI_OPENENDSLOW)) ||
				(tmp <= 0 && MoveLocationNMControlProc(pp->pi->I_CLS_HIP)))
			{
				ACT_STEP_NEW(END_STEP);
			}
			CHK_SET_MOV(SetClampOpenSlow1);
			break;

		case END_STEP:
			SET_OFF(ClampOpenOff);
			ret = ACT_END;
			break;
		default:
			ret = ACT_END;
			break;
	}

	if(ret == ACT_END || ret == ACT_ERR_ABORT)
		return ret;
	else
		return ClampOpnBackgrd2(pp, pact);
}


  																														 
