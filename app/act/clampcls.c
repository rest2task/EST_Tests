
/******************************************************************************
  文 件 名   : clampcls.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 关模、锁模等动作函数
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
#include "machapi.h"
#include "mainlib.h"
#include "inject.h"
#include "EleCtrlApp.h"
#include "HydrPos.h"
#include "coreprotect.h"

extern HYDR             g_hydrout;
WORD				TM_SPCLPCLSERR;
extern	UI16	m_speed;
extern I16 flow_dot;


UI16 GetMaxRPM(WORD ServoNo);

BOOL ClpClsInHydrPos(PCLAMP pp)
{	
	if (pp->md->CLS_POS_KP < 10)
	{
		pp->md->CLS_POS_KP = 10;
	}
	else if (pp->md->CLS_POS_KP > 2000)
	{
		pp->md->CLS_POS_KP = 2000;
	}
	
	if(pp->md->FL_CLSE_SMOOTH > 0 || ((pp->md->FL_CLS_LOCATE > 0) && (pp->md->FL_CLSOPEN_MODE == 0)))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void GetClampClsPresMax(PCLAMP pp, PACT pact)	//	获取高压锁模的锁模力
{
	int press  = GetPumpPress(HYDR_PORT(pp->hy->HYDR_PORT_CLAMPCLS));
	press = (press < 0) ? 0:press;
	
	if(pp->st->DAPRES_CLSHI_MAX < press)
	{
		pp->st->DAPRES_CLSHI_MAX =  press;
    }	
}

UI16 PosMidCls(PACT pact)
{
   int	i,j,cnt = 0;
   UI16 ly[CLAMP_MAX_CORE],t,sn;
   PCLAMP pp = (PCLAMP)(pact->part);
   if(pp->clampmidcorefwd!=0)
   {
		for(i=0;i<CLAMP_MAX_CORE;i++)	
		{			
			pp->core_sn1[i] = 0;
			if((pp->clampmidcorefwd>>i)&1)
			{
				ly[cnt] = pp->pcore[i]->pa->ADPOSI_COREINACT;
				pp->core_sn1[cnt] = i+1;
				
				if(cnt>0)
				{
					if((ly[cnt]!=ly[cnt-1]))
						cnt++;
				}
				else
				{
					pp->corefwd[0] = ly[cnt];
					cnt++;
				}				
			}
		}
		for(i=0;i<(cnt-1);i++)
		{
			for(j=0;j<(cnt-i-1);j++)
			{
				if( ly[j]< ly[j+1])
				{
				   t = ly[j+1];
				   ly[j+1] = ly[j];
				   ly[j] = t;
				   
				   sn = pp->core_sn1[j+1];
				   pp->core_sn1[j+1] = pp->core_sn1[j];
				   pp->core_sn1[j] =sn;
				}
			}
		}
		for(i=0;i<cnt;i++)
	   {
		   pp->corefwd[i] = ly[i];
	   }
  }
  return cnt;
}


void ClampCloseInit(PCLAMP pp, PACT pact)
{
	UI16 i=0,j=0,endposbuf=0,sn = 0,spdsr = 0;
	
	if(pp == NULL)
		return;

	/*开关模运动数据缓存清零*/
	for(i = 0;i<CLAMP_SEG;i++)
	{
		pp->flow_clamp[i] = 0;
		pp->posi_clamp[i] = 0;
	}
	
	for(i = 0;i<CLAMP_POSEND_SEG;i++)
	{
		pp->endpos_clamp[i] = 0;
	}	
	pp->endpos_num_cls = 0;

	if (pp->md->FL_CLSFAST == 0)
	{
		spdsr = pp->md->CLPCLS_SPD_UTI;
	}
	else
	{
		spdsr = pp->md->CLPCLS_SPD_ULT;
	}
	
	//将设定的速度和位置放到算法运算时需要用到的变量中，防止动作过程中修改参数引起问题

	pp->flow_clamp[0] = pp->pa->DAFLOW_CLS1;
	pp->flow_clamp[1] = pp->pa->DAFLOW_CLS2;
	pp->flow_clamp[2] = pp->pa->DAFLOW_CLS3;
	pp->flow_clamp[3] = pp->pa->DAFLOW_CLSPROTECT;
	pp->flow_clamp[4] = pp->pa->DAFLOW_CLSHIGH;
	pp->flow_clamp[5] = 0;
	pp->flow_clamp[6] = 0;

	pp->posi_clamp[0] = pp->pa->ADPOSI_CLS1END;
	pp->posi_clamp[1] = pp->pa->ADPOSI_CLS2END;
	pp->posi_clamp[2] = pp->pa->ADPOSI_CLS3END;
	pp->posi_clamp[3] = pp->pa->ADPOSI_CLSPROTECTEND;
	pp->posi_clamp[4] = 0;
	pp->posi_clamp[5] = 0;
	pp->posi_clamp[6] = 0;

	//关模3段不使用时，将二段的压力，速度和位置赋给3段
	if(pp->md->FL_CLS3 == 0)
	{
		pp->flow_clamp[2] = pp->flow_clamp[1];
		pp->posi_clamp[2] = pp->posi_clamp[1];
	}

	/*途中中子位置初始化*/
	for(i=0;i<8;i++)
	{
		pp->core_sn1[i] = 0;
		pp->core_sn2[i] = 0;
		pp->corefwd[i] = 0;
	}
	pp->clpclsmidcorenum = PosMidCls(pact);
	
	if((pp->md->FL_CLS_LOCATE > 0) && (pp->md->FL_CLSOPEN_MODE == 0))
	{
		if(pp->md->FL_CLS3 == 0)
		{
			pp->endpos_clamp[0] = pp->pa->ADPOSI_CLS2END;
		}
		else
		{
			pp->endpos_clamp[0] = pp->pa->ADPOSI_CLS3END;//pp->pa->ADPOSI_CLSPROTECTEND;
		}
	}
	else
	{
		pp->endpos_clamp[0] = pp->pa->ADPOSI_CLSPROTECTEND;
	}
    
	pp->endpos_num_cls = 1;

	//将途中中子位置放入终止位置对应的缓存中
	if(pp->clpclsmidcorenum > 0)
	{
		if(pp->clpclsmidcorenum < CLAMP_POSEND_SEG)
		{
			for(i = 0;i<pp->clpclsmidcorenum;i++)
			{
				pp->endpos_clamp[pp->endpos_num_cls] = pp->corefwd[i];
				pp->core_sn2[pp->endpos_num_cls] = pp->core_sn1[i];
				pp->endpos_num_cls++;
			}
		}		
	}

	/*位置排序，由于是关模，所以结束位置由大到小排列*/
	for(i = 0;i<pp->endpos_num_cls-1;i++)
	{
		endposbuf = pp->endpos_clamp[i];
		for(j = i+1;j<pp->endpos_num_cls;j++)
		{
			if(endposbuf < pp->endpos_clamp[j])
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

		if (i == 3)
		{
			pp->flow_clamp[i] = (UI32)pp->flow_clamp[i]*(UI32)pp->pa->PER_DAFLOW_LOWPRES/100;
		}
		else
		{
			pp->flow_clamp[i] = (UI32)pp->flow_clamp[i]*(UI32)spdsr/100;
		}
	}

	pp->core_sn_last = 0;
}


void SetClampClsPos(PCLAMP pp, PACT pact)
{
	UI16 mm = 0;
    UI16 seg = 0;
    int PSramp[3] = {0};
    int PPramp[3] = {0};
    int Pspd[MAXNUMSEG] = {0};
    int Ppres[MAXNUMSEG] = {0};
    int Ppos[MAXNUMSEG] = {0};
	UI16 i = 0;
	UI16 endposbuf = 0x0;	//关模时初始化为最小值
	UI16 motor_max_spd = 0;
	int nowpos = 0;
	UI16 flow_limit = 0, press_limit = 0;
	int end = 0;
	UI16 coreendramp = 0;
	UI16 core_sn = 0;
    UI16 pump_k = 0;

	/*使用临近的终止位置为当前开模的终止位置*/
	for(i = 0;i<pp->endpos_num_cls;i++)
	{
		nowpos = GetMeterPosByCh(pp->mh->CLMP_METERCH);
		if(nowpos < 0)
		{
			/*防止负数影响判断*/
			nowpos = 0;
		}
		
		if((int)(pp->endpos_clamp[i]) < nowpos)
		{
			if (pp->core_sn2[i] != 0)
			{
				core_sn = pp->core_sn2[i];				
				coreendramp = pp->pcore[pp->core_sn2[i] - 1]->pa->DAFLOW_CLSENDRAMP_CORE;
			}
			if (i > 0)
			{
				pp->core_sn_last = pp->core_sn2[i-1];
			}
			else
			{
				pp->core_sn_last = 0;
			}
			endposbuf = pp->endpos_clamp[i];
			break;
		}
	}
	
	motor_max_spd = GetMaxRPM(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_CLAMPCLS))-1);
	
	if(motor_max_spd < 500 || motor_max_spd > 9999)
	{
		motor_max_spd = 1800;
	}

	if((pp->md->FL_CLS_LOCATE > 0) && (pp->md->FL_CLSOPEN_MODE == 0))
	{
		seg = 3;
		pump_k = pp->md->CLS_POS_KP;
        if(core_sn > 0)    //有途中中子时，以底压底流做闭环
        {
            flow_limit = pp->mh->LOWFLOW_CLS;
            press_limit = pp->mh->LOWPRES_CLS*10;
        }
        else
        {
    		flow_limit = pp->pa->DAFLOW_CLSPROTECT * 10;
    		press_limit = pp->pa->DAPRES_CLSPROTECT * 10;
        }
	}
	else
	{
		seg = 4;
		pump_k = pp->md->PUMP_K_CLS;
		
		if(endposbuf == pp->posi_clamp[3])
		{
			flow_limit = pp->flow_clamp[3];
		}
		else
		{
			flow_limit = pp->mh->LOWFLOW_CLS;
			if(flow_limit < 20)
				flow_limit = 20;
		}

		press_limit = 0;
	}
		
	InitSetHydrStruCtrlData(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_CLAMPCLS)), motor_max_spd, 0, pp->mh->DIA_CYLINDER_M,pp->mh->DIA_OIL_NUM, GetMotorPump_Q(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_CLAMPCLS)),HYDR_PUMP(pp->hy->HYDR_PUMPS_CLSSLOW)),pp->mh->EFFICIENCY_FACTOR_CLS, press_limit, flow_limit, pump_k);
    
	for(i = 0;i<seg;i++)
	{
		if(pp->flow_clamp[i] == 990)
		{
			pp->flow_clamp[i] = 999;
		}
		/*将原有的关模段数及参数按照现有的结束位置重排*/
		if(pp->posi_clamp[i] > endposbuf)
		{
			Pspd[i] = ActSetSpdToRPM(HYDR_PORT(pp->hy->HYDR_PORT_CLAMPCLS)-1,pp->flow_clamp[i]);
			Ppos[i] = pp->posi_clamp[i];
		}
		else
		{
			Pspd[i] = ActSetSpdToRPM(HYDR_PORT(pp->hy->HYDR_PORT_CLAMPCLS)-1,pp->flow_clamp[i]);
			Ppos[i] = endposbuf;
			seg = i+1;
			break;
		}
	}

	if (pp->core_sn_last > 0 && pp->md->FL_CLAMP_CORE_RAMP > 0)
	{
		PSramp[0] = pp->pcore[pp->core_sn_last-1]->pa->DAFLOW_CLSSTARTRAMP_CORE*10;
		PSramp[1] = pp->pcore[pp->core_sn_last-1]->pa->DAFLOW_CLSSTARTRAMP_CORE*10;
	}
	else
	{
		/*当前位置大于开模终位置减去2mm时，使用启动斜率，否则使用途中*/
		if(pp->clampcls_start_pos >= AbsDiff(pp->pa->ADPOSI_OPENENDEND,20))
		{		
			PSramp[0] = pp->spdramp_cls[CLP_RAMP_CLS_START];
		}
		else
		{
			/*自动状态时，途中启动的唯一可能目前就是途中中子及途中机械手*/		
			if(ChkOperMode(OM_AUTO))
			{
				PSramp[0] = pp->spdramp_cls_manual[CLP_RAMP_CLS_MANUALSTART];
			}
			else
			{
				/*手动途中启动时，使用手动启动斜率*/
				PSramp[0] = pp->spdramp_cls_manual[CLP_RAMP_CLS_MANUALSTART];
			}
		}				

		PSramp[1] =pp->spdramp_cls[CLP_RAMP_CLS_MID];
	}

	if (core_sn > 0 && pp->md->FL_CLAMP_CORE_RAMP > 0)
	{
		PSramp[2] = coreendramp*10;
	}
	else
	{
		/*结束位置为关模低压位置时，使用结束斜率，非低压位置时使用途中*/
		if(Ppos[seg-1] == pp->posi_clamp[3])
		{
			PSramp[2] = pp->spdramp_cls[CLP_RAMP_CLS_END];
		}
		else
		{
			PSramp[2] = pp->spdramp_cls[CLP_RAMP_CLS_END];
		}
	}
	
	if(endposbuf == pp->posi_clamp[3])
		end = ActSetSpdToRPM(HYDR_PORT(pp->hy->HYDR_PORT_CLAMPCLS)-1,pp->flow_clamp[3]);
	else
		end = 0;
	
	if(ChkSysCfg(XF2_SUCKM))
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

    if((pp->md->FL_CLS_LOCATE > 0) && (pp->md->FL_CLSOPEN_MODE == 0))
	{
		if (pp->md->MODE_CLS_LOCATE == 0)
		{
			mm = mm|(DECMODE_POS1<<12);
		}
		else if (pp->md->MODE_CLS_LOCATE == 1)
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
    mm = mm|DECMODE_HOLD_ON_SPD;
    
    SetPosCtrlPara(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_CLAMPCLS)), DIRECBWD, pp->mh->CLMP_METERCH, PSramp, Pspd, PPramp, Ppres, Ppos, seg, pp->md->PUMP_DEAD_CLS, pp->mh->CLMPCLS_HYDRDELAY,mm,end,0,1);
}


void ClampClsStart(PCLAMP pp, PACT pact)
{
	PMACHINE pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1));
	
	pp->close_start_ms = GetMsTick();
}

/*-----------------------------------------------------------------------------------+
|      关快设定                                                                  |
+-----------------------------------------------------------------------------------*/
void ClampClsSlowVlv(PCLAMP pp)
{
	if(pp->mh->TMDLY_VLV_ONCLSULTRA > 500) //做保护，超限赋值为0
		pp->mh->TMDLY_VLV_ONCLSULTRA = 0;
	if(pp->mh->TMDLY_VLV_ONCLS > 500)
		pp->mh->TMDLY_VLV_ONCLS = 0;
	
	SetPOBitDelay(pp->po->O_CLS_SOL, PC_ON, pp->mh->TMDLY_VLV_ONCLS);
	SetPOBit(pp->po->O_CLS_SOL2, PC_ON);			//关摸安全阀
	SetPOBit(pp->po->O_OPN_REL, PC_ON);
	if(pp->md->FL_CLSFAST == 1)
	{
		if(CUS_ID==CS_YUYAOLOG)
		{
			SetPOBit(pp->po->O_OPN_FST, PC_ON);
		}

		SetPOBitDelay(pp->po->O_CLS_ULT, PC_ON, pp->mh->TMDLY_VLV_ONCLSULTRA);
		SetPOBit(pp->po->O_OPN_CLS,PC_ON);
		SetPOBitDelay(pp->po->O_CLS_ULT2, PC_ON, pp->mh->TMDLY_VLV_ONCLSULTRA);   // 差动阀延时输出 
		if(ChkSysCfg(XF0_UTVLV))
			SetPOBitDelay(pp->po->O_OPN_SOL, PC_ON, pp->mh->TMDLY_VLV_ONCLSULTRA);
	}
	
	if(CUS_ID == CS_GDWA)
	   SetPOBit(pp->po->O_CLS_ULT3,PC_ON);

	if (CUS_ID == CS_ZHISHANG)	//	CS_ZHISHANG 0x0B300 温州至上
	{
		if (pp->md->FL_CLSFAST == 1)
		{
			SetPOBit(pp->po->O_CL_TOHI,PC_ON);	//	O_CL_TOHI , //0B6H	关模至高压
			SetPOBit(pp->po->O_CLS_FST, PC_ON);
		}
		else
		{
			SetPOBit(pp->po->O_CL_TOHI,PC_OFF); //	O_CL_TOHI , //0B6H	关模至高压
			SetPOBit(pp->po->O_CLS_FST, PC_OFF);
		}
	}
	else
	{
		SetPOBit(pp->po->O_CL_TOHI, PC_ON);
	}
}


void ClampClsFastVlv(PCLAMP pp)
{
	if(pp->mh->TMDLY_VLV_ONCLSULTRA > 500) //做保护，超限赋值为0
		pp->mh->TMDLY_VLV_ONCLSULTRA = 0;
	if(pp->mh->TMDLY_VLV_ONCLS > 500)
		pp->mh->TMDLY_VLV_ONCLS = 0;
	
	SetPOBitDelay(pp->po->O_CLS_SOL, PC_ON, pp->mh->TMDLY_VLV_ONCLS);
	SetPOBit(pp->po->O_CLS_SOL2, PC_ON);            //关摸安全阀
    SetPOBit(pp->po->O_OPN_REL, PC_ON);
	
	if(pp->md->FL_CLSFAST == 1)
	{
		if(CUS_ID==CS_YUYAOLOG)
		{
			SetPOBit(pp->po->O_OPN_FST, PC_ON);
		}

		SetPOBitDelay(pp->po->O_CLS_ULT, PC_ON, pp->mh->TMDLY_VLV_ONCLSULTRA);
		SetPOBit(pp->po->O_OPN_CLS,PC_ON);
		SetPOBitDelay(pp->po->O_CLS_ULT2, PC_ON, pp->mh->TMDLY_VLV_ONCLSULTRA);   // 差动阀延时输出	
		if(ChkSysCfg(XF0_UTVLV))
		{
			SetPOBitDelay(pp->po->O_OPN_SOL, PC_ON, pp->mh->TMDLY_VLV_ONCLSULTRA);
		}
	}
	
    if(CUS_ID == CS_GDWA)
	   SetPOBit(pp->po->O_CLS_ULT3,PC_ON);

	if (CUS_ID == CS_ZHISHANG)	//  CS_ZHISHANG	0x0B300	温州至上
  	{
  		if (pp->md->FL_CLSFAST == 1)
  		{
  			SetPOBit(pp->po->O_CL_TOHI,PC_ON);	//	O_CL_TOHI , //0B6H  关模至高压
  			SetPOBit(pp->po->O_CLS_FST, PC_ON);
  		}
  		else
  		{
  			SetPOBit(pp->po->O_CL_TOHI,PC_OFF);	//	O_CL_TOHI , //0B6H  关模至高压
  			SetPOBit(pp->po->O_CLS_FST, PC_OFF);
  		}
  	}
	else
	{
		SetPOBit(pp->po->O_CL_TOHI, PC_ON);
		SetPOBit(pp->po->O_CLS_FST, PC_ON);
	}
}

void ClampClsLowPresVlv(PCLAMP pp)
{
	if(pp->mh->TMDLY_VLV_OFFCLSULTRA > 500)  //时间保护，超限采用50ms   
		pp->mh->TMDLY_VLV_OFFCLSULTRA = 5;
	if(pp->mh->TMDLY_VLV_ONCLSLOW > 500)  //时间保护，超限采用0ms
		pp->mh->TMDLY_VLV_ONCLSLOW = 0;
	if(pp->mh->TMDLY_VLV_ONCLS > 500)
		pp->mh->TMDLY_VLV_ONCLS = 0;
	
	SetPOBitDelay(pp->po->O_CLS_SOL, PC_ON, pp->mh->TMDLY_VLV_ONCLS);
	SetPOBit(pp->po->O_CLS_SOL2, PC_ON);
	SetPOBit(pp->po->O_CL_TOHI, PC_ON);
    SetPOBit(pp->po->O_OPN_REL, PC_ON);
	SetPOBit(GetSysPO(O_FST_SOL), PC_OFF);
	SetPOBit(pp->po->O_CLS_FST, PC_OFF);

	if((ChkSysCfg(CO2_CLSLOW) && pp->md->FL_CLSLOWPRES == 1)  || !ChkSysCfg(CO2_CLSLOW))
	{
		SetPOBitDelay(pp->po->O_CLS_LOW,PC_ON, pp->mh->TMDLY_VLV_ONCLSLOW);
	}

	if(!ChkSysCfg(CO2_ULTL))
	{
		
    	SetPOBitDelay(pp->po->O_CLS_ULT, PC_OFF, pp->mh->TMDLY_VLV_OFFCLSULTRA);

		/*差动2关延迟不能超过500ms,海晶要求*/
		if(pp->mh->TMDLY_VLV_OFFCLSULTRA< 50)
		{
			SetPOBitDelay(pp->po->O_CLS_ULT2, PC_OFF, pp->mh->TMDLY_VLV_OFFCLSULTRA);
		}
		else
		{
			SetPOBitDelay(pp->po->O_CLS_ULT2, PC_OFF, 50);
		}

		SetPOBit(pp->po->O_OPN_CLS,PC_OFF);
		if(ChkSysCfg(CO43_UTVLV))
		{
			if (pp->md->FL_CLSFAST == 1)
			{
				SetPOBit(pp->po->O_CLS_ULT,PC_ON);
				SetPOBit(pp->po->O_OPN_CLS,PC_ON);
			}
		}
    	else if(ChkSysCfg(XF0_UTVLV)) 
    	{
			SetPOBitDelay(pp->po->O_OPN_SOL, PC_OFF,pp->mh->TMDLY_VLV_OFFCLSULTRA);  	
    	}
  	}
  			
	if(ChkSysCfg(CO43_FASTCL))
  		SetPOBit(pp->po->O_CLS_FST, PC_ON); 
	else
		SetPOBit(pp->po->O_CLS_FST, PC_OFF);

	if (CUS_ID == CS_ZHISHANG)	//  CS_ZHISHANG	0x0B300	温州至上
  	{
  		if (pp->md->FL_CLSFAST == 1)
  		{
  			SetPOBit(pp->po->O_CL_TOHI,PC_ON);	//	O_CL_TOHI , //0B6H  关模至高压
			SetPOBit(pp->po->O_CLS_FST, PC_ON); //   关摸快速阀，当差动使用时，一直开到低压完成
  		}
  		else
  		{
  			SetPOBit(pp->po->O_CL_TOHI,PC_OFF);	//	O_CL_TOHI , //0B6H  关模至高压
  			SetPOBit(pp->po->O_CLS_FST, PC_OFF); //
  		}
		SetPOBit(pp->po->O_CLS_ULT,PC_OFF);
  	}
	if(CUS_ID == CS_SHIBITUO)          //关摸高压   关摸低压，高压段开，思必拓
		SetPOBit(pp->po->O_CL_HIP,PC_ON); 
	if(CUS_ID == CS_GDWA)
	   SetPOBit(pp->po->O_CLS_ULT3,PC_ON);
}

void        ClampClsOffvVv1(PCLAMP pp)
{
	SetPOBit(pp->po->O_CLS_LOW, PC_OFF);
	SetPOBit(GetSysPO(O_FST_SOL), PC_OFF);
	SetPOBit(pp->po->O_CLS_FST, PC_OFF);
	SetPOBit(pp->po->O_CL_TOHI, PC_OFF);
	SetPOBit(pp->po->O_CLS_ULT, PC_OFF);
	SetPOBit(pp->po->O_OPN_CLS, PC_OFF);
	if(ChkSysCfg(XF0_UTVLV))
		SetPOBit(pp->po->O_OPN_SOL, PC_OFF);
    SetPOBit(pp->po->O_OPN_REL, PC_OFF);
	SetPOBit(pp->po->O_OPN_FST, PC_OFF);
	SetPOBit(pp->po->O_CLAMP_FST, PC_OFF);
	SetPOBit(pp->po->O_CLS_ULT2,PC_OFF);
	SetPOBit(pp->po->O_CL_HIP,PC_OFF); 
	if(CUS_ID == CS_GDWA)
	   SetPOBit(pp->po->O_CLS_ULT3,PC_OFF);
}

void ClampClsOffVlv(PCLAMP pp)
{
	int	i;
	if(pp->mh->TMDLY_VLV_OFFCLSEND > 500)  //数据保护，超限采用0
		pp->mh->TMDLY_VLV_OFFCLSEND = 0; 
		
  	SetPOBitDelay(pp->po->O_CLS_SOL, PC_OFF, pp->mh->TMDLY_VLV_OFFCLSEND);
	SetPOBitDelay(pp->po->O_CLS_SOL2, PC_OFF, pp->mh->TMDLY_VLV_OFFCLSEND2); 
	for(i = 0; i < CLAMP_MAX_EJECT; i++)
		EjectClampClsBwdVlvOff(pp->pejt[i]);
	
	ClampClsOffvVv1(pp);	
}

void ClampClsSlowSetup(PCLAMP pp, PACT pact)
{
	UI16 hydelay;
	if(pp->mh->CLMPCLS_HYDRDELAY > 9999)
		pp->mh->CLMPCLS_HYDRDELAY = 0;
	ClampClsFastVlv(pp);
	if(CUS_ID != CS_SHIBITUO)
		SetPOBit(pp->po->O_CLS_FST,PC_OFFD); 
	hydelay = pp->mh->CLMPCLS_HYDRDELAY;
	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->mh->DAPRES_CLSSETUP,pp->mh->DAFLOW_CLSSETUP, pp->pressramp_cls[CLP_RAMP_CLS_FAST]/10,pp->spdramp_cls[CLP_RAMP_CLS_FAST]/10,
						pp->pressramp_cls[CLP_RAMP_CLS_START]/10,pp->spdramp_cls[CLP_RAMP_CLS_START]/10,  (pact->ftaskstat & INTF_CEC) ? 0 : HYDRDELY, HYDR_PORT(pp->hy->HYDR_PORT_CLAMPCLS),
						pp->hy->HYDR_PUMPS_CLSSETUP,pp->hy->HYDR_PUMPS_CLSSETUP,    pp->hy->SERVO_KP_CLSSETUP,pp->hy->SERVO_KI_CLSSETUP, 0, !(pact->ftaskstat & INTF_CEC), 
						HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS),pp->md->CLPCLS_SPD_UTI);    
}


void ClampClsSlow_Spc(PCLAMP pp, PACT pact)	//关模启动段
{
	UI16 hydelay;
	UI16 spdsr = 0;
	if(pp->mh->CLMPCLS_HYDRDELAY > 9999)
		pp->mh->CLMPCLS_HYDRDELAY = 0;
	ClampClsFastVlv(pp);
	if(CUS_ID != CS_SHIBITUO && CUS_ID != CS_NINGSHU)
		SetPOBit(pp->po->O_CLS_FST,PC_OFFD); 
	hydelay = pp->mh->CLMPCLS_HYDRDELAY;

	if (pp->md->FL_CLSFAST == 0)
	{
		spdsr = pp->md->CLPCLS_SPD_UTI;
	}
	else
	{
		spdsr = pp->md->CLPCLS_SPD_ULT;
	}
	
	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_CLS_SLOW,pp->pa->DAFLOW_CLS_SLOW, pp->pa->DAPRES_CLS_RAMPSLOW,pp->pa->DAFLOW_CLS_RAMPSLOW,
						pp->pa->DAPRES_CLS_RAMPSLOW,pp->pa->DAFLOW_CLS_RAMPSLOW,  (pact->ftaskstat & INTF_CEC) ? 0 : HYDRDELY, HYDR_PORT(pp->hy->HYDR_PORT_CLAMPCLS),
						pp->hy->HYDR_PUMPS_CLSSLOW,pp->hy->HYDR_PUMPS_CLSSLOW,    pp->hy->SERVO_KP_CLSSETUP,pp->hy->SERVO_KI_CLSSETUP, 0, !(pact->ftaskstat & INTF_CEC), 
						HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS), spdsr);    

    if(ChkLocateDa(pp))
	{		
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->pa->DAFLOW_CLS_SLOW, 0,0,
					0,CLAMP_CLS_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS),CLAMP_CLS_PID_DIREC);
	}
}

void SetClampClsSlow(PCLAMP pp, PACT pact)
{
	int	i;
	UI16 hydelay,mode;
	UI16 pumps,spdsr = 0;
	if(pp->mh->CLMPCLS_HYDRDELAY > 9999)
		pp->mh->CLMPCLS_HYDRDELAY = 0;
	hydelay = pp->mh->CLMPCLS_HYDRDELAY;

	ClampClsSlowVlv(pp);
	if(CUS_ID != CS_SHIBITUO && CUS_ID != CS_NINGSHU)
		SetPOBit(pp->po->O_CLS_FST, PC_OFFD);

	if (ChkSysCfg(CO0_EJTB))
	{
		for(i = 0; i < CLAMP_MAX_EJECT; i++)
			EjectClampClsVlvOn(pp->pejt[i]);
	}
	if(pp->md->FL_CLSOPEN_MODE == 3)
		pumps = pp->hy->HYDR_PUMPS_CLSFAST;
	else
		pumps = pp->hy->HYDR_PUMPS_CLSSLOW;
	
	if(ClpClsInHydrPos(pp) && !ChkSysCfg(XF1_ROBD))
		mode  = MC_CTR_MODE_POSI|MC_CTR_MODE_PRES;
	else
		mode  = MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES;

	if (pp->md->FL_CLSFAST == 0)
	{
		spdsr = pp->md->CLPCLS_SPD_UTI;
	}
	else
	{
		spdsr = pp->md->CLPCLS_SPD_ULT;
	}
	
	ActSetHydr_SpdSR(pact, mode, pp->pa->DAPRES_CLS1,pp->pa->DAFLOW_CLS1,pp->pressramp_cls[CLP_RAMP_CLS_START]/10,pp->spdramp_cls[CLP_RAMP_CLS_START]/10,
	                      pp->pressramp_cls[CLP_RAMP_CLS_START]/10,pp->spdramp_cls[CLP_RAMP_CLS_START]/10,    (pact->ftaskstat & INTF_CEC) ? 0 : HYDRDELY, HYDR_PORT(pp->hy->HYDR_PORT_CLAMPCLS),
	                      pumps,pumps,      pp->hy->SERVO_KP_CLS,pp->hy->SERVO_KI_CLS, 0,  !(pact->ftaskstat & INTF_CEC), 
	                      HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS),spdsr);

	
	if(pp->md->FL_CLSOPEN_MODE == 4)
    {   
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->mh->DAFLOW_VLV_CLS * ((float)pp->pa->DAFLOW_CLS1/99),0,0,
					0,CLAMP_CLS_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS),CLAMP_CLS_P_DIREC);
    }
    else if(ChkLocateDa(pp))
	{		
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->pa->DAFLOW_CLS1, 50,50,
					0,CLAMP_CLS_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS),CLAMP_CLS_PID_DIREC);
	}

	pp->mov_start_ms = GetMsTick();
}

void SetClampClsStartSpc(PCLAMP pp, PACT pact)
{
	int	i;
	UI16 spdsr = 0;
	//UI16 hydelay;

	ClampClsSlowVlv(pp);
	if(CUS_ID != CS_SHIBITUO && CUS_ID != CS_NINGSHU)
		SetPOBit(pp->po->O_CLS_FST, PC_OFFD);

	if (ChkSysCfg(CO0_EJTB))
	{
		for(i = 0; i < CLAMP_MAX_EJECT; i++)
			EjectClampClsVlvOn(pp->pejt[i]);
	}
	if (pp->md->FL_CLSFAST == 0)
	{
		spdsr = pp->md->CLPCLS_SPD_UTI;
	}
	else
	{
		spdsr = pp->md->CLPCLS_SPD_ULT;
	}
	
	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES,pp->mh->DAPRESS_CLSMIDSTART,pp->mh->DAPFLOW_CLSMIDSTART,pp->mh->DAPRESS_CLSMIDRAMPSTART,pp->mh->DAPFLOW_CLSMIDRAMPSTART,
	                  pp->mh->DAPRESS_CLSMIDRAMPSTART,pp->mh->DAPFLOW_CLSMIDRAMPSTART,    0, HYDR_PORT(pp->hy->HYDR_PORT_CLAMPCLS),
	                  pp->hy->HYDR_PUMPS_CLSSLOW,pp->hy->HYDR_PUMPS_CLSSLOW,      pp->hy->SERVO_KP_CLS,pp->hy->SERVO_KI_CLS, 0,  !(pact->ftaskstat & INTF_CEC), 
	                  HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS), spdsr);
	
	if(pp->md->FL_CLSOPEN_MODE == 2 || ChkLocateDa(pp))
	{
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->mh->DAPFLOW_CLSMIDSTART, 0,0,
					0,CLAMP_CLS_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS),CLAMP_CLS_PID_DIREC);	
	}
	else if(pp->md->FL_CLSOPEN_MODE == 4)
	{
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->mh->DAFLOW_VLV_CLS* ((float)pp->mh->DAPFLOW_CLSMIDSTART/99), 0,0,
					0,CLAMP_CLS_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS),CLAMP_CLS_P_DIREC);
	}
	
	pp->mov_start_ms = GetMsTick();
}

void SetClampClsFast(PCLAMP pp, PACT pact)
{
	UI16 hydelay,mode,spdsr = 0;
	if(pp->mh->CLMPCLS_HYDRDELAY > 9999)
		pp->mh->CLMPCLS_HYDRDELAY = 0;
	hydelay = pp->mh->CLMPCLS_HYDRDELAY;
	SetPOBit(GetSysPO(O_FST_SOL), PC_ON);

	if(ClpClsInHydrPos(pp) && !ChkSysCfg(XF1_ROBD))
		mode  = MC_CTR_MODE_POSI|MC_CTR_MODE_PRES;
	else
		mode  = MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES;

	if (pp->md->FL_CLSFAST == 0)
	{
		spdsr = pp->md->CLPCLS_SPD_UTI;
	}
	else
	{
		spdsr = pp->md->CLPCLS_SPD_ULT;
	}
	
	ActSetHydr_SpdSR(pact, mode, pp->pa->DAPRES_CLS2,pp->pa->DAFLOW_CLS2,pp->pressramp_cls[CLP_RAMP_CLS_FAST]/10,pp->spdramp_cls[CLP_RAMP_CLS_FAST]/10,
						 pp->pressramp_cls[CLP_RAMP_CLS_START]/10,pp->spdramp_cls[CLP_RAMP_CLS_START]/10,      (pact->ftaskstat & INTF_CEC) ? 0 : HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_CLAMPCLS),
						 pp->hy->HYDR_PUMPS_CLSFAST,pp->hy->HYDR_PUMPS_CLSFAST, pp->hy->SERVO_KP_CLS,pp->hy->SERVO_KI_CLS, 0, !(pact->ftaskstat & INTF_CEC), 
						 HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS),spdsr);

	ClampClsFastVlv(pp);	

	if(pp->md->FL_CLSOPEN_MODE == 4)
	{
		ActSet_Da_flow(pact, MC_CTR_MODE_DA,pp->mh->DAFLOW_VLV_CLS* ((float)pp->pa->DAFLOW_CLS2/99), 0,0,
					0,CLAMP_CLS_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS),CLAMP_CLS_P_DIREC);
	}
    else if(ChkLocateDa(pp))
	{		
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->pa->DAFLOW_CLS2, 0,0,
					0,CLAMP_CLS_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS),CLAMP_CLS_PID_DIREC);
	}
}

void SetClampClsFast2(PCLAMP pp, PACT pact)
{
	UI16 hydelay,mode,spdsr = 0;
	if(pp->mh->CLMPCLS_HYDRDELAY > 9999)
		pp->mh->CLMPCLS_HYDRDELAY = 0;
	hydelay = pp->mh->CLMPCLS_HYDRDELAY;
	
	if(ClpClsInHydrPos(pp) && !ChkSysCfg(XF1_ROBD))
		mode  = MC_CTR_MODE_POSI|MC_CTR_MODE_PRES;
	else
		mode  = MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES;

	if (pp->md->FL_CLSFAST == 0)
	{
		spdsr = pp->md->CLPCLS_SPD_UTI;
	}
	else
	{
		spdsr = pp->md->CLPCLS_SPD_ULT;
	}
	
	ActSetHydr_SpdSR(pact, mode, pp->pa->DAPRES_CLS3,pp->pa->DAFLOW_CLS3,pp->pressramp_cls[CLP_RAMP_CLS_FAST]/10,pp->spdramp_cls[CLP_RAMP_CLS_FAST]/10,
						 pp->pressramp_cls[CLP_RAMP_CLS_START]/10,pp->spdramp_cls[CLP_RAMP_CLS_START]/10,     (pact->ftaskstat & INTF_CEC) ? 0 : HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_CLAMPCLS),
						 pp->hy->HYDR_PUMPS_CLSFAST,pp->hy->HYDR_PUMPS_CLSFAST,    pp->hy->SERVO_KP_CLS,pp->hy->SERVO_KI_CLS, 0,!(pact->ftaskstat & INTF_CEC), 
						 HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS),spdsr);
	if(pp->md->FL_CLSOPEN_MODE == 2 || ChkLocateDa(pp))
	{
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->pa->DAFLOW_CLS3, 0,0,
					0,CLAMP_CLS_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS),CLAMP_CLS_PID_DIREC);
	}
	else if(pp->md->FL_CLSOPEN_MODE == 4)
	{
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->mh->DAFLOW_VLV_CLS* ((float)pp->pa->DAFLOW_CLS3/99), 0,0,
					0,CLAMP_CLS_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS),CLAMP_CLS_P_DIREC);
	}


	ClampClsFastVlv(pp);	
}


void SetClampLowPres(PCLAMP pp, PACT pact)
{
	UI16 hydelay,mode,vlvk;
	if(pp->mh->CLMPCLS_HYDRDELAY > 9999)
		pp->mh->CLMPCLS_HYDRDELAY = 0;
	hydelay = pp->mh->CLMPCLS_HYDRDELAY;
	ClampClsLowPresVlv(pp);

	if(ClpClsInHydrPos(pp) && !ChkSysCfg(XF1_ROBD))
		mode  = MC_CTR_MODE_POSI|MC_CTR_MODE_PRES;
	else
		mode  = MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES;
	ActSetHydr_SpdSR(pact, mode, pp->pa->DAPRES_CLSPROTECT,pp->pa->DAFLOW_CLSPROTECT,pp->pressramp_cls[CLP_RAMP_CLS_LOW]/10,pp->spdramp_cls[CLP_RAMP_CLS_LOW]/10,
						 pp->pressramp_cls[CLP_RAMP_CLS_START]/10,pp->spdramp_cls[CLP_RAMP_CLS_START]/10,     (pact->ftaskstat & INTF_CEC) ? 0 : HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_CLAMPCLS),
						 pp->hy->HYDR_PUMPS_CLSPROTECT,pp->hy->HYDR_PUMPS_CLSPROTECT, pp->hy->SERVO_KP_CLS,pp->hy->SERVO_KI_CLS, 0,!(pact->ftaskstat & INTF_CEC), 
						 HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS),pp->pa->PER_DAFLOW_LOWPRES);
	if(pp->md->FL_CLSOPEN_MODE == 2 || ChkLocateDa(pp))
	{
		if(pp->pa->DAFLOW_VLVCLS == 0 || pp->pa->DAFLOW_VLVCLS >99)
			vlvk = pp->pa->DAFLOW_CLSPROTECT;
		else
			vlvk = pp->pa->DAFLOW_VLVCLS;
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, vlvk, 0,0,
					0,CLAMP_CLS_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS),CLAMP_CLS_PID_DIREC);
	}	
	else if(pp->md->FL_CLSOPEN_MODE == 4)
	{
		ActSet_Da_flow(pact, MC_CTR_MODE_DA,pp->mh->DAFLOW_LOWVLV_CLS, 0,0,
					0,CLAMP_CLS_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS),CLAMP_CLS_P_DIREC);
	}
}

/*-----------------------------------------------------------------------------------+
|      关闭关模压力流量和关阀函数                                                               						 |
+-----------------------------------------------------------------------------------*/
void ClampPressOff(PCLAMP pp, PACT pact)
{ 
	pp->clampfunc = 0;
	if(pp->md->FL_CLSOPEN_MODE == 2)
	{
	    HYDR_CLS(0, 0);	
		HYDR_CLS_Da_F(CLAMP_CLS_PID_PORT,2,CLAMP_CLS_PID_DIREC);
	}
	else
		HYDR_CLS(pp->mh->DAPRES_CLSOFFRAMP, pp->mh->DAFLOW_CLSOFFRAMP);
  	ClampClsOffVlv(pp);

	if(ClpClsInHydrPos(pp))
		PosControlEnd(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_CLAMPCLS)));

	if(pp->md->FL_CLSOPEN_MODE == 4)
		HYDR_CLS_Da_F(CLAMP_CLS_PID_PORT,100,CLAMP_CLS_P_DIREC);
    else if(ChkLocateDa(pp))
		HYDR_CLS_Da_F(CLAMP_CLS_PID_PORT,0,CLAMP_CLS_PID_DIREC);
}

/*-----------------------------------------------------------------------------------+
|      关闭关模压力流量和关阀函数                                                               						 |
+-----------------------------------------------------------------------------------*/
void ClampClsOff(PCLAMP pp, PACT pact)
{ 
	pp->clampfunc = 0;
	if(pp->md->FL_CLSOPEN_MODE == 2)
	{
	    HYDR_CLS(0, 0);	
		HYDR_CLS_Da_F(CLAMP_CLS_PID_PORT,2,CLAMP_CLS_PID_DIREC);
	}
	else
		HYDR_CLS(pp->pressramp_cls[CLP_RAMP_CLS_END]/10, pp->spdramp_cls[CLP_RAMP_CLS_END]/10);
  	ClampClsOffVlv(pp);

	if(ClpClsInHydrPos(pp))
		PosControlEnd(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_CLAMPCLS)));

	if(pp->md->FL_CLSOPEN_MODE == 4)
		HYDR_CLS_Da_F(CLAMP_CLS_PID_PORT,100,CLAMP_CLS_P_DIREC);
    else if(ChkLocateDa(pp))
		HYDR_CLS_Da_F(CLAMP_CLS_PID_PORT,0,CLAMP_CLS_PID_DIREC);	
}
void ClampClsOff_2(PCLAMP pp, PACT pact)
{ 	
	WORD smooth = FALSE;
	pp->clampfunc = 0;
	if(pp->md->FL_CLSOPEN_MODE == 2)
	{
	    HYDR_CLS(0, 0);	
		HYDR_CLS_Da_F(CLAMP_CLS_PID_PORT,pp->mh->DAPFLOW_CLSMIDEND_RAMPOFF,CLAMP_CLS_PID_DIREC);
	}
	else if(ClpClsInHydrPos(pp))
	{
		if(!ChkSysCfg(XF1_CHRLT))
		{
			if((smooth = SmoothStop(pact->last_hydr_port,DIRECBWD,pp->mh->CLMP_METERCH,pp->spdramp_cls_manual[CLP_RAMP_CLS_MANUALEND],(ACT_OP_FUNC_NEW)ClampClsOffVlv,pact)) == FALSE)
			{	
				HYDR_CLS(pp->spdramp_cls_manual[CLP_RAMP_CLS_MANUALEND]/10, pp->spdramp_cls_manual[CLP_RAMP_CLS_MANUALEND]/10);
			}
		}
		else
		{
			HYDR_CLS(pp->spdramp_cls_manual[CLP_RAMP_CLS_MANUALEND]/10, pp->spdramp_cls_manual[CLP_RAMP_CLS_MANUALEND]/10);
		}
	}
	else
	{
		HYDR_CLS(pp->pressramp_cls[CLP_RAMP_CLS_END]/10, pp->spdramp_cls[CLP_RAMP_CLS_END]/10);
	}

	if (!smooth)
	{
		ClampClsOffVlv(pp);
	}

	if(pp->md->FL_CLSOPEN_MODE == 4)
		HYDR_CLS_Da_F(CLAMP_CLS_PID_PORT,100,CLAMP_CLS_P_DIREC);
    else if(ChkLocateDa(pp))
		HYDR_CLS_Da_F(CLAMP_CLS_PID_PORT,100,CLAMP_CLS_PID_DIREC);	
}

void ClampClsOff_3(PCLAMP pp, PACT pact)	//关模高压时的手动放开，由于不需要smooth，所以此处单独出来
{ 	
	pp->clampfunc = 0;
	if(pp->md->FL_CLSOPEN_MODE == 2)
	{
	    HYDR_CLS(0, 0);	
		HYDR_CLS_Da_F(CLAMP_CLS_PID_PORT,pp->mh->DAPFLOW_CLSMIDEND_RAMPOFF,CLAMP_CLS_PID_DIREC);
	}
	else if(ClpClsInHydrPos(pp))
	{
		HYDR_CLS(pp->spdramp_cls_manual[CLP_RAMP_CLS_MANUALEND]/10, pp->spdramp_cls_manual[CLP_RAMP_CLS_MANUALEND]/10);
		if(ClpClsInHydrPos(pp))
			PosControlEnd(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_CLAMPCLS)));
	}
	else
	{
		HYDR_CLS(pp->pressramp_cls[CLP_RAMP_CLS_END]/10, pp->spdramp_cls[CLP_RAMP_CLS_END]/10);
	}
	ClampClsOffVlv(pp);

	if(pp->md->FL_CLSOPEN_MODE == 4)
		HYDR_CLS_Da_F(CLAMP_CLS_PID_PORT,100,CLAMP_CLS_P_DIREC);
    else if(ChkLocateDa(pp))
		HYDR_CLS_Da_F(CLAMP_CLS_PID_PORT,100,CLAMP_CLS_PID_DIREC);	
}

/*-----------------------------------------------------------------------------------+
|      关闭关模压力流量和关阀函数                                                               						 |
+-----------------------------------------------------------------------------------*/
void ClampClsEndVlvOff(PCLAMP pp, PACT pact)
{ 
	if ((ChkOperMode(OM_AUTO) && GetSysMold()->FL_INJMOLDIN && ChkSysCfg( CO2_INJCLS)))
		ClampClsOff(pp, pact);
}

ACT_RET ClampClsPreChk(PCLAMP pp, PACT pact)
{
	if(OilLvErr() || OilFltErr() || OilTmpErr() || WaterTmpErr())
		return ACT_ERR_ABORT;

	/*循环前的检测 ，开始*/
	if (MotorTemperErr())
		return ACT_ERR_ABORT;

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

	if(CUS_ID == CS_ZW)
	{
	 	if(CheckPI(pp->pi->I_RSV_CLS) == PI_RESET)//LY
	  	{
			SET_PART_ERR(pp, ER0_CLSP);
			return ACT_ERR_ABORT;
		}
	}

	if(SafeDoorValveAlarm(pp->psfdr))
		return ACT_ERR_ABORT;
	if(RobotConfigErrChk(pp->prbt))
        return ACT_ERR_ABORT;
	return ACT_OK;
}

BOOL ClampClsMecSafeErrChk(PCLAMP pp)
{
	if(ChkSysCfg(CO0_MSNAG))
		return PartErrSet(pp, ER0_MESA, CheckPI(pp->pi->I_MEC_SAF) == PI_SET);
	else
		return PartErrSet(pp, ER0_MESA, CheckPI(pp->pi->I_MEC_SAF) == PI_RESET);
}

PI_STATUS ClampMecSafeChk(PCLAMP pp)
{
	if(pp != NULL)
	{
		if(ChkSysCfg(CO0_MSNAG))
			return((PI_STATUS)(!CheckPI(pp->pi->I_MEC_SAF)));
		else
			return(CheckPI(pp->pi->I_MEC_SAF));
	}
	else
		return PI_NOUSED;
}

 //关模安全保护
BOOL ClampClsSafeProtectErrChk(PCLAMP pp)
{
   	if(CheckPI(pp->pi->I_HYD_SAF) == PI_SET)// ||CheckPI(pp->pi->I_HYD_SAF) == PI_SET || CheckPI(pp->pi->I_CLSSENSOR) == PI_SET)
   	{
		SET_PART_ERR(pp, ER0_HYSEN);
		return TRUE;
   	}

	return FALSE;
}

BOOL ClampClsTimeCount(PCLAMP pp, PACT pact)//关模限时监测   
{
	WORD    maxtime;

  	if(ChkSysCfg(CO0_CMPW))
  	{
  		if(ChkOperMode(OM_AUTO) && AutoAlarmIsStart())
  			maxtime = pp->st->TM_CLAMPCLS_MAX;
		else if(pp->md->TM_CLAMPCLSDEFAULTMAX > 100 &&  pp->md->TM_CLAMPCLSDEFAULTMAX <= 9999)
	      		maxtime = pp->md->TM_CLAMPCLSDEFAULTMAX;
		else
			maxtime = CLAMP_DEFAULT_TM;  

		if((UI16)(ACT_RUN_MS/10) > maxtime)
		{
			SET_PART_ERR(pp, ER0_CLSM);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL ClampClslLowProtectErrChk(PCLAMP pp)
{
	WORD    maxtime;
	if(ChkSysCfg(XF3_CLSLL))
	{
		maxtime = pp->md->TM_CLSPROTECTDEFAULTMAX;
	}
	else
	{
	  	if(ChkOperMode(OM_AUTO) && AutoAlarmIsStart())
			maxtime = pp->st->TM_CLAMPCLSLO_MAX;
		else
			maxtime = pp->md->TM_CLSPROTECTDEFAULTMAX;
	}

	if((UI16)(DIFF_MS(pp->clslowpres_start_ms)/10) > maxtime)
	{
		SET_PART_ERR(pp, ER0_CLSP);
		return TRUE;
	}
	else
		return FALSE;
}

 
ACT_RET ClampClsBackgrd(PCLAMP pp, PACT pact)
{
	int i;
    int servo_spd;

	if((ChkOperMode(OM_MANUAL)&&pp->forceclamp ==0)&&ChkMachType(MACH_VT))
		return ACT_ERR_ABORT;
	
	m_speed = pp->st->CLAMP_SPEED;

	if(pact->step < STAGE_1_STEP)
		return ACT_OK;

	for(i=0;i<CLAMP_MAX_CORE;i++)
	{
		if(pp->pcore[i] != NULL)
		{
			if(pp->pcore[i]->md->CORE_LIMIT_CHECK == 1 && CoreLimitSwitchErrCheck(pp->pcore[i])) //关模动作过程中检查中子行程开关
				return ACT_ERR_ABORT;
			
			if(pp->pcore[i]->md->CLAMP_CORE_CHECK == 1 && ClampClsCoreStateErrCheck(pp->pcore[i],pact->step)) //关模动作过程中检测中子状态
				return ACT_ERR_ABORT;
		}
	}
	
	if (FSafeDoorIsCloseErrChk(pp->psfdr))                 //前安全门保护
		return ACT_ERR_ABORT;
	
	if(ClampsafeLight(pp))         //模板光幕检查
        return ACT_ERR_ABORT;

	if(ClampClsEjectBwdErrChk(pp))
	{		
		if ((pp->pejt[0] !=NULL) && ((UI16)(Get10MsTick() - pp->ClampBwdErr_10ms) > 2))  //修改时间为20ms，滤波同时防止时间过长，关摸会运行一段距离
		{
			if (EjectBwdErrChk(pp->pejt[0], TRUE))
			{
				StandAlarm(FALSE);
			}
			return ACT_ERR_ABORT;
		}
	}
	else 
		pp->ClampBwdErr_10ms = Get10MsTick();
	
	if(RobotClampClsProtectErrChk(pp->prbt) || ClampClsSafeProtectErrChk(pp) ||
	   MoldClampClsProtectErrChk(pp->prbt) || (pact->step >= STAGE_1_STEP && (ClampClsMecSafeErrChk(pp) ||  ClampClsTimeCount(pp, pact))))
		return ACT_ERR_ABORT;

    //合模任意段(高压前)开始都开0.5s脱退阀   QRC 2017.04.19
	if (ChkSysCfg(XF3_COLOF)&&(pp->clampejectbwdflag==0)&&(pact->step > STAGE_1_STEP))
	{
		for(i = 0; i < CLAMP_MAX_EJECT; i++)
			EjectClampClsVlvOn(pp->pejt[i]);

		 pp->clampejectbwdflag=1;
		 pp->mov_start_ms = GetMsTick();
	}
	
	if((((ChkSysCfg(XF3_COLOF)&&pp->clampejectbwdflag==1&&pact->step > STAGE_1_STEP && pp->mov_start_ms != 0 && (UI32)(GetMsTick() - pp->mov_start_ms) > 500)
	||(!ChkSysCfg(XF3_COLOF)&&pact->step > STAGE_1_STEP && pp->mov_start_ms != 0 && (UI32)(GetMsTick() - pp->mov_start_ms) > 500))&&(CUS_ID!=CS_YUYAOLOG))
	|| (CUS_ID==CS_YUYAOLOG && pact->step >= CLAMPCLOSE4))
	{
		for(i = 0; i < CLAMP_MAX_EJECT; i++)
		{
			if(pp->pejt[i] != NULL)
				EjectClampClsBwdVlvOff(pp->pejt[i]);
		}
	}

	if(pp->close_start_ms != 0)
		pp->st->TM_CLAMPCLS_RT = CalMsTo10MsUI16(pp->close_start_ms);	

	if((CheckPI(pp->pi->I_CLS_UPGA) == 0))
	{
		SET_PART_ERR(pp, ER0_UPGAERR);
		return ACT_ERR_ABORT;
	}

	if(CUS_ID==CS_NINGSHU)
	{
		if(pp->md->FL_CLSFAST == 1)	// 差动合模[快速合模]
		{
			if(TestPOIsOn(pp->po->O_CLS_ULT) == 1)
			{
				if(TestPOIsOn(pp->po->O_CLAMP_FST) == 1)
					SetPOBit(pp->po->O_CLAMP_FST, 0);
			}
			else
			{
				if(TestPOIsOn(pp->po->O_CLAMP_FST) == 0)
					SetPOBit(pp->po->O_CLAMP_FST, 1);
			}
		}
		else
		{
			if(TestPOIsOn(pp->po->O_CLAMP_FST) == 0)
				SetPOBit(pp->po->O_CLAMP_FST, 1);
		}
	}
	
	if(CheckPIUsed(pp->prbt->pi->I_M_ALAR))    //模具警报关摸  时时检测，有信号时就出警报   20161220
	{
		if(TestPI(pp->prbt->pi->I_M_ALAR) == 1)
		{
			SET_PART_ERR(pp->prbt,ER0_MoldO);
		}
	}

	if(!ChkSysCfg(XF3_HS7HT))
	{
		if(ChkOperMode(OM_SENR))
		{
			if((pact->step < STAGE_5_STEP)&&(pact->step > CLAMPCLOSE3))
			{
				if(ClampSensorErrChk(pp))
					return ACT_ERR_ABORT;
			}
			
		}
	}

    if(ChkLocateDa(pp) && ((pact->step & 0xFF00) >= CLAMPCLOSE1) && ((pact->step & 0xFF00) <= CLAMPCLOSE3)) //只在关模一段~关模三段同步输出
    {
        if(MIN_STEP(pact->step) == 0)
        {
            pp->Cls_port = GetHydrPort_Ch(HYDR_PORT(pp->hy->HYDR_PORT_CLAMPCLS));  //获取液压通道号
            pp->Cls_port = (pp->Cls_port > g_pump_num)?0:(pp->Cls_port-1);
        }

        if(g_pumpn[pp->Cls_port] != NULL)
        {
            servo_spd = g_pumpn[pp->Cls_port]->McPara.Mcnow[1] * VIR_MAX_SPD_K;  //驱动转速换算成对应模拟量输出比例

            if(servo_spd < 1*HYDRAD)
                servo_spd = 1*HYDRAD;

            if(pp->pa->DAFLOW_VLVCLS > 0 && pp->pa->DAFLOW_VLVCLS <= 99)
            {
                if(servo_spd < pp->pa->DAFLOW_VLVCLS*HYDRAD)
                    servo_spd = pp->pa->DAFLOW_VLVCLS*HYDRAD; //使用低压阀口时，提前减小至低压阀口，避免进入低压时阀口突变
            }

            SetVlvData_mn(CLAMP_CLS_PID_CH, servo_spd, CLAMP_CLS_PID_DIREC);
        }
    }
    
	return ACT_OK;
}

ACT_RET ClampClose(PACT pact, UI32 para)
{
	int i;
	PCLAMP	pp = (PCLAMP)(pact->part);
	ACT_RET	ret = ACT_OK;
    static UI16 pres_flag = 0;

	
	PCORE	pcore = (PCORE)GetPartByID(MK_PART(CORE_ID, 1));

	if(pp == NULL)
		return ACT_ERR_ABORT;

	if(ActIODelayLimit(pp->po->O_CLS_SOL, TRUE))
	{
	    return ACT_ERR_ABORT;
	}
	
	pp->mh->CLMP_METERCH = DEF_CLAMP_METER_CH;
		
	switch (pact->step)						  
	{
		case  INIT1_STEP:
            pres_flag = 0;
			pp->clslowpres_start_ms = GetMsTick();			
			//为了特殊中子1，保证脱模时中子进退做一次。 DK 2014.1104
			if((pcore->md->FL_SPECIALCODE==1)&&(ChkOperMode(OM_AUTO)))
			{
				pcore->corestsf=0;
				pcore->corestsb=0;
			}	
      		if((ret = ClampClsPreChk(pp, pact)) != ACT_OK)
				return ret;
			else
			{
				ACT_STEP_NEW(INIT1_STEP + 1);
			}

			/*开模终止位置移动至合模前检测保证开模终点的准确性 2014-11-20*/
			pp->st->ADPOSI_OPNEND = GetMeterPosByCh(pp->mh->CLMP_METERCH);
			if(pp->st->TM_CLAMPOPN == 0)
				pp->st->CLAMP_OPN_SPD = 0;
			else
				pp->st->CLAMP_OPN_SPD = (pp->st->ADPOSI_OPNEND-pp->qc_pos_opnstart)*100/pp->st->TM_CLAMPOPN;

			ClampCloseInit(pp,pact);
			ClampCmd( pp, CMD_QC_PARA,2);							
			break;      			
    	case INIT1_STEP + 1: 
		 	 // 保护机械手  05-12-31
			if(!RobotClampClsErrChk(pp->prbt) && !MoldClampClsErrChk(pp->prbt))
			{
				if(!ChkSysCfg(CO0_OFFM))		//机械手信号翻转检测				ws-2018-12-10
				{
					if(ChkOperMode(OM_AUTO) && (!(ChkRunFlag(INIF_AUTO1))))	//自动非首模时，需检测到0-1的翻转信号才能关模
					{
						if(pp->prbt->flag_Robot_Turn == 0)
						{
							CLR_PART_ERR(pp->prbt, ER0_ROB_TURN);
							RobotClampCloseStart(pp->prbt);
							MoldClampCloseStart(pp->prbt);
							ACT_STEP_NEW(INIT1_STEP + 2);
						}
						else
                        {
                            if (ACT_STEP_MS > 200) //等待
    						{
    							if(!CHK_PART_ERR(pp->prbt, ER0_ROB_TURN))
    							{
    								SET_PART_ERR(pp->prbt, ER0_ROB_TURN);
    							}

                                if (ACT_STEP_MS > 1000)
                                {
        							return ACT_ERR_ABORT;
                                }
    						}                            
                        }
					}
					else
					{
						RobotClampCloseStart(pp->prbt);
						MoldClampCloseStart(pp->prbt);
						ACT_STEP_NEW(INIT1_STEP + 2);
					}
				}
				else
				{
					RobotClampCloseStart(pp->prbt);
					MoldClampCloseStart(pp->prbt);
					ACT_STEP_NEW(INIT1_STEP + 2);
				}
			}
    		break;   			
    	case INIT1_STEP + 2:			
			if(pp->md->FL_CLSWAITCHG == 1)
			{
				if(ChkActRun(pp->pinj, ACT_SN_BIT(OS_CHRG)|ACT_SN_BIT(OS_SUCK)|ACT_SN_BIT(OS_SUCKCHG)))
					return ACT_OK;
			}
			
			if(!FSafeDoorIsCloseErrChk(pp->psfdr))
			{
				if(ChkSysCfg(XF3_SHUT2)&&(CHK_PART_ERR(pp->psfdr,ER3_FSER)))
				{
					return ACT_ERR_ABORT;
				}
				else
				{
					ACT_STEP_NEW(INIT1_STEP + 3);
				}				
			}
			break;
    	case INIT1_STEP + 3:
			//机械安全检知及吹气动作检测
			if(!ClampClsMecSafeErrChk(pp) && ChkAirBlowComplete())
			{
				GetSys()->cyclestart = GetMsTick();
				ACT_STEP_NEW(INIT1_STEP + 4);
		    }
			break;
    	case INIT1_STEP + 4:
			for(i = 0; i < CLAMP_MAX_EJECT; i++)
			{
				if(pp->pejt[i] != NULL && EjectBwdErrChk(pp->pejt[i], TRUE))
					ForkAct(pp->pejt[i]->act + OS_EJTB - 1, SUSPEND_FORK, pact->ptsk, 1, 0);
			}
			ACT_STEP_NEW(INIT1_STEP + 5);
			pp->ClampBwdErr_10ms = Get10MsTick();
		    break;
		case INIT1_STEP + 5:
			for(i = 0; i < CLAMP_MAX_EJECT; i++)
			{
				if(pp->pejt[i] != NULL )
					CLR_PART_ERR(pp->pejt[i],ER0_EJTP);
			}
			pact->reactive_step = INIT1_STEP + 5;
            pp->clampcls_start_pos = GetMeterPosByCh(pp->mh->CLMP_METERCH);
	        pp->clampejectbwdflag=0;  //合模脱退阀开标记在此置0  QRC 2017.04.19
			ClampClsStart(pp, pact);
			ACT_START();		
			
			if(GetMeterLenByCh(pp->mh->CLMP_METERCH) > 0)
			{
				if (GetMeterPosByCh(pp->mh->CLMP_METERCH) <=pp->pa->ADPOSI_CLSPROTECTEND)
					ACT_STEP_NEW(END_STEP);
				else
				{
					if(pp->pa->TM_CLS_SLOW > 0)
					{
						if((( GetMeterPosByCh(pp->mh->CLMP_METERCH) - pp->pa->ADPOSI_CLS_SLOW)>pp->pa->ADPOSI_CLSPROTECTEND))
						{
							pp->cls_Poss_start  =  GetMeterPosByCh(pp->mh->CLMP_METERCH) -  pp->pa->ADPOSI_CLS_SLOW;
							ACT_STEP_NEW(STAGE_SPEC2_STEP);
						}
						else
							ACT_STEP_NEW(CLAMPCLOSE1);
					}
					else
						ACT_STEP_NEW(CLAMPCLOSE1);
				}
      		}
      		else
      		{
				if(TestPI(pp->pi->I_OPN_END))//如果从开模终开始从关模开始做
					ACT_STEP_NEW(CLAMPCLOSE1);
			    else if(TestPI(pp->pi->I_CLS_END) ||TestPI(pp->pi->I_CLS_HIP))//如果压到关模终或者关模高压则程序跳转到关模高压
					ACT_STEP_NEW(END_STEP);
			    else if(TestPI(pp->pi->I_CLS_LOW))//如果压到关模低压 则从关模低压开始做
					ACT_STEP_NEW(CLAMPCLOSE4);
			    else                     //一个点都没压到就直接输出装模压力流量到压到任何一输出点然后跳转到相应的STEP
					ACT_STEP_NEW(CLAMPCLSSETUP);
      		}
      		break;
		//关模开始时候没压到任何输入点直接设定装模压力流量
		case CLAMPCLSSETUP:
		case CLAMPCLSSETUP + 1:
			if(TestPI(pp->pi->I_OPN_END))//如果从开模终开始从关模开始做
				ACT_STEP_NEW(CLAMPCLOSE1);
			else if(TestPI(pp->pi->I_CLS_END) ||TestPI(pp->pi->I_CLS_HIP))//如果压到关模终或者关模高压则程序跳转到关模高压
				ACT_STEP_NEW(END_STEP);
			else if(TestPI(pp->pi->I_CLS_LOW))//如果压到关模低压 则从关模低压开始做
				ACT_STEP_NEW(CLAMPCLOSE4);

			CHK_SET_MOV(ClampClsSlowSetup);
			break;			

		case STAGE_SPEC2_STEP:
		case STAGE_SPEC2_STEP + 1:	
			if(pp->pa->TM_CLS_SLOW > 2000)
				pp->pa->TM_CLS_SLOW = 2000;
			if((GetMeterLenByCh(pp->mh->CLMP_METERCH) > 0))
			{
				if(MoveLocationMeterControlProc(pp->cls_Poss_start, GetMeterPosByCh(pp->mh->CLMP_METERCH))
					|| (ACT_STEP_100US >=  ((UI32)pp->pa->TM_CLS_SLOW)*10))
				{
					ACT_STEP_NEW(CLAMPCLOSE1);
				}

			}
			else
			{
				ACT_STEP_NEW(CLAMPCLOSE1);
			}
			
			CHK_SET_MOV(ClampClsSlow_Spc);
			break;
			
      	case CLAMPCLOSE1: 
			if(ClpClsInHydrPos(pp) && !ChkSysCfg(XF1_ROBD))
				SET_MOV2(SetClampClsPos);
		case CLAMPCLOSE1 + 1:
			if(GetMeterLenByCh(pp->mh->CLMP_METERCH) > 0)				/*走电子尺*/
			{
		      	if(MoveLocationMeterControlProc(pp->pa->ADPOSI_CLS1END, GetMeterPosByCh(pp->mh->CLMP_METERCH)))
					ACT_STEP_NEW(CLAMPCLOSE2);
			}
			else 				/*走行程开关*/
			{
				if(MoveLocationNMControlProc(pp->pi->I_CLS_LOW))
					ACT_STEP_NEW(CLAMPCLOSE4);
			}

			if (pp->md->FL_CLSOPEN_MODE == 3 || pp->md->FL_CLSOPEN_MODE == 4)
			{
				ACT_STEP_NEW(CLAMPCLOSE2);//智能模式从二段开始(启动->关模快速->关模低压->关模高压)
			}

			CHK_SET_MOV(SetClampClsSlow);
      		break;
      	case CLAMPCLOSE2 :
      	case CLAMPCLOSE2 + 1:
			if(MoveLocationMeterControlProc(pp->pa->ADPOSI_CLS2END, GetMeterPosByCh(pp->mh->CLMP_METERCH)))
			{
				if((pp->pa->ADPOSI_CLS3END <pp->pa->ADPOSI_CLS2END && pp->pa->ADPOSI_CLS3END> pp->pa->ADPOSI_CLSPROTECTEND) 
					&& pp->md->FL_CLS3 == 1)
					ACT_STEP_NEW(CLAMPCLOSE3);
				else
					ACT_STEP_NEW(CLAMPCLOSE4);
			}
			CHK_SET_MOV(SetClampClsFast);
			break;
		case CLAMPCLOSE3:
		case CLAMPCLOSE3 + 1:
			if(MoveLocationMeterControlProc(pp->pa->ADPOSI_CLS3END, GetMeterPosByCh(pp->mh->CLMP_METERCH)))
			{
				ACT_STEP_NEW(CLAMPCLOSE4);
			}
			CHK_SET_MOV(SetClampClsFast2);
			break;
		case CLAMPCLOSE4:
			pp->clslowpres_start_ms = GetMsTick();
			pp->st->TM_CLAMPMOV = ACT_RUN_MS/10;
		case CLAMPCLOSE4 + 1:
			if(GetMeterLenByCh(pp->mh->CLMP_METERCH) > 0) 				/*走电子尺*/
			{
				if(ChkSysCfg(CO2_INJCLS) && (para == 0) && (GetSysMold()->FL_INJMOLDIN == 4) && ChkOperMode(OM_AUTO))
				{	//压塑方式选择关模途中的时候必须关模到注压位置后才能结束关模，即使注压位置小于关模低压位置(lzh 2022.3.3)
					
				}
				else if(MoveLocationMeterControlProc(pp->pa->ADPOSI_CLSPROTECTEND, GetMeterPosByCh(pp->mh->CLMP_METERCH)))
					ACT_STEP_NEW(END_STEP);
			}
			else 				/*走行程开关*/
			{
				if(MoveLocationNMControlProc(pp->pi->I_CLS_HIP))
					ACT_STEP_NEW(END_STEP);
			}
			
			if(ClampClslLowProtectErrChk(pp))
				return ACT_ERR_ABORT;
			if(ChkSysCfg(XF3_CLSLL))
			{
				pp->st->TM_CLAMPCLSLO = (UI16)(DIFF_MS(pp->clslowpres_start_ms)/10);
			}
			CHK_SET_MOV(SetClampLowPres);
			break;

		case END_STEP:
			SET_OFF(ClampClsEndVlvOff);
			if(pact->ftaskstat & INTF_CEC)
			{
				if(!ChkSysCfg(XF3_CLSLL))
				{
					pp->st->TM_CLAMPCLSLO = (UI16)(DIFF_MS(pp->clslowpres_start_ms)/10);

					if(AutoAlarmCanUpdate())
						pp->st->TM_CLAMPCLSLO_MAX = pp->st->TM_CLAMPCLSLO + (pp->st->TM_CLAMPCLSLO >> 1) + 10;
				}
			}
            if(pres_flag == 0 && ChkOperMode(OM_AUTO) && (para == 1) && ChkSysCfg(CO2_INJCLS) && (GetSysMold()->FL_INJMOLDIN == 4))
			{
			    pres_flag = 1;  //保证只做一次压模。这里有激活步，压模结束激活关模时从INIT1_STEP + 5开始，会再跑一次这里再调压模，就会陷入死循环 ccz
				ForkAct(pp->act + OS_PRSM- 1, SUSPEND_FORK, pact->ptsk, 0, 0);
			}
			ret = ACT_END;	
			break;
			
		default:
			ret = ACT_END;
			break;
	}

    //排气方式为关模途中时，关模到注压位置后射出，射出到压塑位置后继续关模，关模结束后再继续注射(lzh 2021.12.13)
	if(ChkOperMode(OM_AUTO) && (para == 0) && (pact->step < END_STEP) && ChkSysCfg(CO2_INJCLS) && (GetSysMold()->FL_INJMOLDIN == 4))
	{
		if(GetMeterLenByCh(pp->mh->CLMP_METERCH) > 0) 				/*走电子尺*/
		{
			if(MoveLocationMeterControlProc(pp->md->ADPOSI_INJMOLDIN, GetMeterPosByCh(pp->mh->CLMP_METERCH)))
				ACT_STEP_NEW(END_STEP);
		}
	}
    
	if(ret == ACT_END || ret == ACT_ERR_ABORT)
	{
		return ret;
	}
	else
  		return ClampClsBackgrd(pp, pact);
}

ACT_RET Clampclose_pid(PACT pact, UI32 para)
{
	int i;
	PCLAMP	pp = (PCLAMP)(pact->part);
	ACT_RET	ret = ACT_OK;
	
	PCORE	pcore = (PCORE)GetPartByID(MK_PART(CORE_ID, 1));
    UI16 vlvk;

	if(pp == NULL)
		return ACT_ERR_ABORT;

	if(ActIODelayLimit(pp->po->O_CLS_SOL, TRUE))
	{
	    return ACT_ERR_ABORT;
	}
	
	pp->mh->CLMP_METERCH = DEF_CLAMP_METER_CH;
		
	switch (pact->step)						  
	{
		case  INIT1_STEP:
			pp->clslowpres_start_ms = GetMsTick();
			//为了特殊中子1，保证脱模时中子进退做一次。 DK 2014.1104
			if((pcore->md->FL_SPECIALCODE==1)&&(ChkOperMode(OM_AUTO)))
			{
				pcore->corestsf=0;
				pcore->corestsb=0;
			}	
      		if((ret = ClampClsPreChk(pp, pact)) != ACT_OK)
				return ret;
			else
			{
				ACT_STEP_NEW(INIT1_STEP + 1);
			}

			/*开模终止位置移动至合模前检测保证开模终点的准确性 2014-11-20*/
			pp->st->ADPOSI_OPNEND = GetMeterPosByCh(pp->mh->CLMP_METERCH);
			if(pp->st->TM_CLAMPOPN == 0)
				pp->st->CLAMP_OPN_SPD = 0;
			else
				pp->st->CLAMP_OPN_SPD = (pp->st->ADPOSI_OPNEND-pp->qc_pos_opnstart)*100/pp->st->TM_CLAMPOPN;

			ClampCmd( pp, CMD_QC_PARA,2);							
			break;      			
    	case INIT1_STEP + 1: 
		 	 // 保护机械手  05-12-31
			if(!RobotClampClsErrChk(pp->prbt) && !MoldClampClsErrChk(pp->prbt))
			{
				if(!ChkSysCfg(CO0_OFFM))		//机械手信号翻转检测				ws-2018-12-10
				{
					if(ChkOperMode(OM_AUTO) && (!(ChkRunFlag(INIF_AUTO1))))	//自动非首模时，需检测到0-1的翻转信号才能关模
					{
						if(pp->prbt->flag_Robot_Turn == 0)
						{
							CLR_PART_ERR(pp->prbt, ER0_ROB_TURN);
							RobotClampCloseStart(pp->prbt);
							MoldClampCloseStart(pp->prbt);
							ACT_STEP_NEW(INIT1_STEP + 2);
						}
						else
                        {
                            if (ACT_STEP_MS > 200) //等待
    						{
    							if(!CHK_PART_ERR(pp->prbt, ER0_ROB_TURN))
    							{
    								SET_PART_ERR(pp->prbt, ER0_ROB_TURN);
    							}

                                if (ACT_STEP_MS > 1000)
                                {
        							return ACT_ERR_ABORT;
                                }
    						}                            
                        }
					}
					else
					{
						RobotClampCloseStart(pp->prbt);
						MoldClampCloseStart(pp->prbt);
						ACT_STEP_NEW(INIT1_STEP + 2);
					}
				}
				else
				{
					RobotClampCloseStart(pp->prbt);
					MoldClampCloseStart(pp->prbt);
					ACT_STEP_NEW(INIT1_STEP + 2);
				}
			}
    		break;   			
    	case INIT1_STEP + 2:
			if(pp->md->FL_CLSWAITCHG == 1)
			{
				if(ChkActRun(pp->pinj, ACT_SN_BIT(OS_CHRG)|ACT_SN_BIT(OS_SUCK)|ACT_SN_BIT(OS_SUCKCHG)))
					return ACT_OK;
			}
			if(!FSafeDoorIsCloseErrChk(pp->psfdr))
			{
				if(ChkSysCfg(XF3_SHUT2)&&(CHK_PART_ERR(pp->psfdr,ER3_FSER)))
				{
					return ACT_ERR_ABORT;
				}
				else
				{
					ACT_STEP_NEW(INIT1_STEP + 3);
				}				
			}
			break;
    	case INIT1_STEP + 3:			
			//机械安全检知及吹气动作检测
			if(!ClampClsMecSafeErrChk(pp) && ChkAirBlowComplete())
			{
				GetSys()->cyclestart = GetMsTick();
				ACT_STEP_NEW(INIT1_STEP + 4);
		    }
			break;
    	case INIT1_STEP + 4:
			for(i = 0; i < CLAMP_MAX_EJECT; i++)
			{
				if(pp->pejt[i] != NULL && EjectBwdErrChk(pp->pejt[i], TRUE))
					ForkAct(pp->pejt[i]->act + OS_EJTB - 1, SUSPEND_FORK, pact->ptsk, 1, 0);
			}
			ACT_STEP_NEW(INIT1_STEP + 5);
			pp->ClampBwdErr_10ms = Get10MsTick();
		    break;
		case INIT1_STEP + 5:
			for(i = 0; i < CLAMP_MAX_EJECT; i++)
			{
				if(pp->pejt[i] != NULL )
					CLR_PART_ERR(pp->pejt[i],ER0_EJTP);
			}

	        pp->clampejectbwdflag=0;  //合模脱退阀开标记在此置0  QRC 2017.04.19
	        for(i=0;i<8;i++)
				pp->corefwd[i] = 0;
			pp->clpclsmidcorenum = PosMidCls(pact);
			pp->CoreNumCls = 0; 
            pp->pclsflowendda = 0;
            PosParaInit_DA(0,pp->hy->POSN_KP_CLS,pp->hy->POSN_KI_CLS,pp->hy->POSN_KD_CLS);
			ACT_STEP_NEW(INIT1_STEP + 6);
			break;
			
		case INIT1_STEP + 6:
			ClampClsStart(pp, pact);
			ACT_START();		
			
			if(GetMeterLenByCh(pp->mh->CLMP_METERCH) > 0)
			{
				if (GetMeterPosByCh(pp->mh->CLMP_METERCH) <= pp->pa->ADPOSI_CLSPROTECTEND)
					ACT_STEP_NEW(END_STEP);
				else
				{
					ACT_STEP_NEW(CLAMPCLSSPC);	
           			pp->clsslowendpos = GetMeterPosByCh(pp->mh->CLMP_METERCH) -  pp->mh->ADPOS_CLSMIDSTART;
				}
      		}
      		else
      		{
				if(TestPI(pp->pi->I_OPN_END))//如果从开模终开始从关模开始做
					ACT_STEP_NEW(CLAMPCLOSE1);
			    else if(TestPI(pp->pi->I_CLS_END) ||TestPI(pp->pi->I_CLS_HIP))//如果压到关模终或者关模高压则程序跳转到关模高压
					ACT_STEP_NEW(END_STEP);
			    else if(TestPI(pp->pi->I_CLS_LOW))//如果压到关模低压 则从关模低压开始做
					ACT_STEP_NEW(CLAMPCLOSE4);
			    else                     //一个点都没压到就直接输出装模压力流量到压到任何一输出点然后跳转到相应的STEP
					ACT_STEP_NEW(CLAMPCLSSETUP);
      		}
      		break;
			
		case CLAMPCLSSETUP:
		case CLAMPCLSSETUP + 1:
			if(TestPI(pp->pi->I_OPN_END))//如果从开模终开始从关模开始做
				ACT_STEP_NEW(CLAMPCLOSE1);
			else if(TestPI(pp->pi->I_CLS_END) ||TestPI(pp->pi->I_CLS_HIP))//如果压到关模终或者关模高压则程序跳转到关模高压
				ACT_STEP_NEW(END_STEP);
			else if(TestPI(pp->pi->I_CLS_LOW))//如果压到关模低压 则从关模低压开始做
				ACT_STEP_NEW(CLAMPCLOSE4);

			CHK_SET_MOV(ClampClsSlowSetup);
			break;	
			
        case CLAMPCLSSPC:
		case CLAMPCLSSPC + 1:        
            if(pp->mh->TM_CLSMIDPROTECT > 9999)
                pp->mh->TM_CLSMIDPROTECT = 9999;	
			if(((GetMeterLenByCh(pp->mh->CLMP_METERCH) > 0)&&((pp->clsslowendpos >= GetMeterPosByCh(pp->mh->CLMP_METERCH))))
                || (ACT_STEP_MS>=pp->mh->TM_CLSMIDPROTECT))                
			{              
					ACT_STEP_NEW(CLAMPCLOSE3);
			}	

            if((pp->CoreNumCls + 1) > pp->clpclsmidcorenum)
            {
        		if((GetMeterLenByCh(pp->mh->CLMP_METERCH) > 0)  && (pp->pclsflowendda >= PospidProc_DASPC(pp->pa->DAPRES_CLS3,pp->pa->DAFLOW_CLS3,GetMeterPosByCh(pp->mh->CLMP_METERCH),pp->pa->ADPOSI_CLS3END,0x01,pp->mh->DAPRES_CLSRAMP2*10)))
        		{
        		   	ACT_STEP_NEW(CLAMPCLOSE3);			
        		}
            }
            else
            {
        		if((GetMeterLenByCh(pp->mh->CLMP_METERCH) > 0)  && (pp->pclsflowendda >= PospidProc_DASPC(pp->pa->DAPRES_CLS3,pp->pa->DAFLOW_CLS3,GetMeterPosByCh(pp->mh->CLMP_METERCH),pp->corefwd[pp->CoreNumCls],0x01,pp->mh->DAPRES_CLSRAMP2*10)))
        		{
        		   	ACT_STEP_NEW(CLAMPCLOSE3);		
        		}
            } 
			pp->pclsflowendda = getflowdavalue((UI32)pp->mh->DAPFLOW_CLSMIDSTART*FLOWACCURACY*10,CLAMP_VLV_USE);
			CHK_SET_MOV(SetClampClsStartSpc);               
			break;

		case CLAMPCLOSE3:
			g_sramp_da.speedlast_da = (((float)pp->pclsflowendda/4095)*10000);
		case CLAMPCLOSE3 + 1:
			pp->clampfunc = 1;

            if(pp->pa->DAFLOW_VLVCLS == 0 || pp->pa->DAFLOW_VLVCLS > 99)
    			vlvk = pp->pa->DAFLOW_CLSPROTECT;
    		else
    			vlvk = pp->pa->DAFLOW_VLVCLS;
            
			//关模起始位置使用开模终止位置，中途中子关位置PID只处理范围在关模快速段的途中中子，即低压段和高压段的途中中子不进行处理
            if((pp->CoreNumCls + 1) > pp->clpclsmidcorenum || (pp->corefwd[pp->CoreNumCls] < pp->pa->ADPOSI_CLS3END))
            {
				PospidProc_DA(pp->pa->DAPRES_CLS3,pp->pa->DAFLOW_CLS3,GetMeterPosByCh(pp->mh->CLMP_METERCH),pp->pa->ADPOSI_CLS3END,0x01,pp->mh->DAPRES_CLSRAMP2*10,pp->mh->DAPRES_CLSRAMPLOWPRES*10,pp->pa->DAPRES_CLSPROTECT,vlvk);              
				//比低压位置还小的关模途中中子时，目标位置用低压位置
			//	AdposServoRampDownUp(pp->pa->DAPRES_CLS3,pp->pa->DAFLOW_CLS3, pp->mh->DAPRESS_CLSPOSPIDEND, pp->pa->DAFLOW_CLS3, pp->pa->ADPOS_OPENSPD3, pp->pa->ADPOSI_CLS3END, GetMeterPosByCh(pp->mh->CLMP_METERCH));

				if(MoveLocationMeterControlProc(pp->pa->ADPOSI_CLS3END, GetMeterPosByCh(pp->mh->CLMP_METERCH)))
				{
					pp->clampfunc = 0;
                    ACT_STEP_NEW(CLAMPCLOSE4);
				}
            }
            else
            {
                if(MoveLocationMeterControlProc(pp->corefwd[pp->CoreNumCls], GetMeterPosByCh(pp->mh->CLMP_METERCH)))
                {
                    pp->CoreNumCls++;
					pp->clampfunc = 0;
                    ACT_STEP_NEW(INIT1_STEP + 6);
                } 

                if(pp->CoreNumCls == 0)
                {
                //    AdposServoRampDownUp(pp->pa->DAPRES_CLS3,pp->pa->DAFLOW_CLS3, pp->mh->DAPRESS_CLSPOSPIDEND, pp->pa->DAFLOW_CLS3, pp->pa->ADPOS_OPENSPD3, pp->corefwd[pp->CoreNumCls], GetMeterPosByCh(pp->mh->CLMP_METERCH)); 
					PospidProc_DA(pp->pa->DAPRES_CLS3,pp->pa->DAFLOW_CLS3,GetMeterPosByCh(pp->mh->CLMP_METERCH),pp->corefwd[pp->CoreNumCls],0x01,pp->mh->DAPRES_CLSRAMP2*10,pp->mh->DAPRES_CLSRAMPLOWPRES*10,pp->mh->DAPRESS_CLSPOSPIDEND,0x01);
                }
				else
				{
                 //   AdposServoRampDownUp(pp->mh->DAPRES_CLSSETUP,pp->mh->DAFLOW_CLSSETUP, pp->mh->DAPRESS_CLSPOSPIDEND, pp->mh->DAFLOW_CLSSETUP, pp->corefwd[pp->CoreNumCls - 1], pp->corefwd[pp->CoreNumCls], GetMeterPosByCh(pp->mh->CLMP_METERCH)); 
					PospidProc_DA(pp->mh->DAPRES_CLSSETUP,pp->mh->DAFLOW_CLSSETUP,(GetMeterPosByCh(pp->mh->CLMP_METERCH)-50),pp->corefwd[pp->CoreNumCls],0x01,pp->mh->DAPRES_CLSRAMP2*10,pp->mh->DAPRESS_CLSMIDEND_RAMPOFF*10,pp->mh->DAPRESS_CLSPOSPIDEND,1);
				}          
            }
			CHK_SET_MOV(SetClampClsFast2);
			break;

		case CLAMPCLOSE4:
			pp->clslowpres_start_ms = GetMsTick();
			pp->st->TM_CLAMPMOV = ACT_RUN_MS/10;
		case CLAMPCLOSE4 + 1:
			if(GetMeterLenByCh(pp->mh->CLMP_METERCH) > 0)
			{
				if(MoveLocationMeterControlProc(pp->pa->ADPOSI_CLSPROTECTEND, GetMeterPosByCh(pp->mh->CLMP_METERCH)))                   
					ACT_STEP_NEW(END_STEP);
			}
			else
			{
				if(MoveLocationNMControlProc(pp->pi->I_CLS_HIP))
					ACT_STEP_NEW(END_STEP);
			}
			
			if(ClampClslLowProtectErrChk(pp))
				return ACT_ERR_ABORT;

			CHK_SET_MOV(SetClampLowPres);
			break;

		case END_STEP:
			SET_OFF(ClampClsEndVlvOff);
			if(pact->ftaskstat & INTF_CEC)
			{
				pp->st->TM_CLAMPCLSLO = (UI16)(DIFF_MS(pp->clslowpres_start_ms)/10);

				if(AutoAlarmCanUpdate())
					pp->st->TM_CLAMPCLSLO_MAX = pp->st->TM_CLAMPCLSLO + (pp->st->TM_CLAMPCLSLO >> 1) + 10;
			}
			ret = ACT_END;	
			break;
			
		default:
			ret = ACT_END;
			break;
	}
	
	if(ret == ACT_END || ret == ACT_ERR_ABORT)
		return ret;
	else
  		return ClampClsBackgrd(pp, pact);
}

/*-----------------------------------------------------------------------------------+
|      关模高压开阀关阀函数                                                                 |
+-----------------------------------------------------------------------------------*/
void ClampClsHiVlv(PCLAMP pp)
{
	if(pp->mh->TMDLY_VLV_ONCLS > 500)
		pp->mh->TMDLY_VLV_ONCLS = 0;
	SetPOBitDelay(pp->po->O_CLS_SOL, PC_ON, pp->mh->TMDLY_VLV_ONCLS);
	SetPOBit(pp->po->O_CLS_SOL2, PC_ON);
    SetPOBit(pp->po->O_OPN_REL, PC_OFF);

	SetPOBit(pp->po->O_CLS_LOW, PC_OFF);
	SetPOBit(GetSysPO(O_FST_SOL), PC_OFF);
	SetPOBit(pp->po->O_CLS_FST, PC_OFF);
	SetPOBit(pp->po->O_CL_TOHI, PC_OFF);

	if (CUS_ID==CS_HAIXIN)
	{
    	SetPOBitDelay(pp->po->O_CLS_ULT, PC_OFF, pp->mh->TMDLY_VLV_OFFCLSULTRA);
	}
	else
	{
		SetPOBit(pp->po->O_CLS_ULT, PC_OFF);
	}
	
	SetPOBit(pp->po->O_OPN_CLS, PC_OFF);

	if(ChkSysCfg(XF0_UTVLV))
		SetPOBit(pp->po->O_OPN_SOL, PC_OFF);

	if(pp->md->FL_CLSFAST == 0)
		SetPOBit(pp->po->O_CLAMP_FST, PC_ON);
	else 
		SetPOBit(pp->po->O_CLAMP_FST, PC_OFF);
	/*[宁塑]关模:              差动使用=差动开，该点关；差动关，该点开。
					                差动不使用=该点开
    			   锁模：          差动使用=该点关；差动不使用=该点开
			  开模快速：该点开*/
	SetPOBit(pp->po->O_CL_HIP,PC_ON); 
	if(CUS_ID == CS_GDWA)
	   SetPOBit(pp->po->O_CLS_ULT3,PC_ON);
}

void SetClampHiPres(PCLAMP pp, PACT pact)
{
    UI16 tmp_hiflow_da = 0;
	ClampClsHiVlv(pp);	

	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_CLSHIGH,pp->pa->DAFLOW_CLSHIGH,pp->pressramp_cls[CLP_RAMP_CLS_HIGH1]/10,pp->spdramp_cls[CLP_RAMP_CLS_HIGH1]/10,
						 pp->pressramp_cls[CLP_RAMP_CLS_START]/10,pp->spdramp_cls[CLP_RAMP_CLS_START]/10, 0, HYDR_PORT(pp->hy->HYDR_PORT_CLAMPCLS),
						 pp->hy->HYDR_PUMPS_CLSHIG,pp->hy->HYDR_PUMPS_CLSHIG, pp->hy->SERVO_KP_HIPRS,pp->hy->SERVO_KI_HIPRS, 0, !(pact->ftaskstat & INTF_CEC), 
						 HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS), pp->pa->PER_DAFLOW_HIGHPRES);
	if(pp->md->FL_CLSOPEN_MODE == 2 || ChkLocateDa(pp))
	{
	    if(pp->pa->DAFLOW_VLVCLSHI > 0 && pp->pa->DAFLOW_VLVCLSHI < 100)
            tmp_hiflow_da = pp->pa->DAFLOW_VLVCLSHI;
        else
            tmp_hiflow_da = pp->pa->DAFLOW_CLSHIGH;
        
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, tmp_hiflow_da, pp->spdramp_cls[CLP_RAMP_CLS_HIGH1]/10, 0,
					0,CLAMP_CLS_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS),CLAMP_CLS_PID_DIREC);	
	}
	else if(pp->md->FL_CLSOPEN_MODE == 4)
	{
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->mh->DAFLOW_HIGVLV_CLS, 0,0,
					0,CLAMP_CLS_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS),CLAMP_CLS_P_DIREC);
	}
	pp->hipress_start_ms = GetMsTick();
}

void SetClampHiPres2(PCLAMP pp, PACT pact)
{
	UI16 hydelay;
    UI16 tmp_hiflow_da = 0;
	if(pp->mh->CLMPCLS_HYDRDELAY > 9999)
		pp->mh->CLMPCLS_HYDRDELAY = 0;
	hydelay = pp->mh->CLMPCLS_HYDRDELAY;
	ClampClsHiVlv(pp);	

	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_CLSHIGH,pp->pa->DAFLOW_CLSHIPRES2,pp->pressramp_cls[CLP_RAMP_CLS_HIGH2]/10,pp->spdramp_cls[CLP_RAMP_CLS_HIGH2]/10,
						 pp->pressramp_cls[CLP_RAMP_CLS_START]/10,pp->spdramp_cls[CLP_RAMP_CLS_START]/10, (pact->ftaskstat & INTF_CEC) ? 0 : HYDRDELY, HYDR_PORT(pp->hy->HYDR_PORT_CLAMPCLS),
						 pp->hy->HYDR_PUMPS_CLSHIG,pp->hy->HYDR_PUMPS_CLSHIG, pp->hy->SERVO_KP_HIPRS,pp->hy->SERVO_KI_HIPRS, 0, !(pact->ftaskstat & INTF_CEC), 
						 HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS),pp->pa->PER_DAFLOW_HIGHPRES);
	if(pp->md->FL_CLSOPEN_MODE == 2 || ChkLocateDa(pp))
	{
	    if(pp->pa->DAFLOW_VLVCLSHI > 0 && pp->pa->DAFLOW_VLVCLSHI < 100)
            tmp_hiflow_da = pp->pa->DAFLOW_VLVCLSHI;
        else
            tmp_hiflow_da = pp->pa->DAFLOW_CLSHIPRES2;
        
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, tmp_hiflow_da, pp->spdramp_cls[CLP_RAMP_CLS_HIGH2]/10, 0,
					0,CLAMP_CLS_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS),CLAMP_CLS_PID_DIREC);	
	}
	else if(pp->md->FL_CLSOPEN_MODE == 4)
	{
		ActSet_Da_flow(pact, MC_CTR_MODE_DA,pp->mh->DAFLOW_HIGVLV_CLS, 0,0,
					0,CLAMP_CLS_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS),CLAMP_CLS_P_DIREC);
	}
}

BOOL ClampHiPressProtectErrChk(PCLAMP pp)
{
	WORD    maxtime;

  	if(ChkOperMode(OM_AUTO) && AutoAlarmIsStart())
		maxtime = pp->st->TM_CLAMPCLSHI_MAX;
	else
		maxtime = pp->md->TM_CLSHIGHDEFAULTMAX;

	if((UI16)(DIFF_MS(pp->hipress_start_ms)/10) > maxtime)
	{
		SET_PART_ERR(pp, ER0_CLSP);
		return TRUE;
	}
	else
		return FALSE;
}

ACT_RET ClampPressBackgrd(PCLAMP pp, PACT pact)
{
	int i;
	
	for(i=0;i<CLAMP_MAX_CORE;i++)
	{
		if(pp->pcore[i] != NULL)
		{
			if(pp->pcore[i]->md->CORE_LIMIT_CHECK == 1 && CoreLimitSwitchErrCheck(pp->pcore[i])) //关模动作过程中检查中子行程开关
				return ACT_ERR_ABORT;
			
			if(pp->pcore[i]->md->CLAMP_CORE_CHECK == 1 && ClampClsCoreStateErrCheck(pp->pcore[i],pact->step)) //关模动作过程中检测中子状态
				return ACT_ERR_ABORT;		
		}
	}
	
	if((ChkOperMode(OM_MANUAL)&&pp->forceclamp ==0)&&ChkMachType(MACH_VT))
		return ACT_ERR_ABORT;
	if (FSafeDoorIsCloseErrChk(pp->psfdr))                 //前安全门保护
	{
		return ACT_ERR_ABORT;
	}
	
	if(ClampsafeLight(pp))         //模板光幕检查
        return ACT_ERR_ABORT;

	if(ClampClsEjectBwdErrChk(pp))
	{
		if ((pp->pejt[0] !=NULL) && ((UI16)(Get10MsTick() - pp->ClampBwdErr_10ms) > 2))
		{
			if (EjectBwdErrChk(pp->pejt[0], TRUE))
			{
				StandAlarm(FALSE);
			}
			return ACT_ERR_ABORT;
		}
	}
	else 
		pp->ClampBwdErr_10ms = Get10MsTick();

	if(RobotClampClsProtectErrChk(pp->prbt) || MoldClampClsProtectErrChk(pp->prbt))
		return ACT_ERR_ABORT;

  	if (ChkSysCfg(CO5_ADJMOLD)) //0x0800  松模问题的自动调模		2014.02.20
      	GetClampclsPresMax(pact,pp->padj);	//	获取高压锁模的锁模力

	GetClampClsPresMax(pp, pact);	//获得最大锁模力
	
	if(pp->close_start_ms != 0)
		pp->st->TM_CLAMPCLS_RT = CalMsTo10MsUI16(pp->close_start_ms);	

	if((CheckPI(pp->pi->I_CLS_UPGA) == 0))
	{
		SET_PART_ERR(pp, ER0_UPGAERR);
		return ACT_ERR_ABORT;
	}

	if (GetMeterPosByCh(pp->mh->CLMP_METERCH)<=10)      //如果距离模座位置1ms时，关差
  	{		
  		SetPOBit(pp->po->O_CLS_ULT2,0);
  	}

	if(CheckPIUsed(pp->prbt->pi->I_M_ALAR))    //模具警报关摸  时时检测，有信号时就出警报   20161220
	{
		if(TestPI(pp->prbt->pi->I_M_ALAR) == 1)
		{
			SET_PART_ERR(pp->prbt,ER0_MoldO);
		}
	}

	m_speed = pp->st->CLAMP_SPEED;
		
	return ACT_OK;
}

ACT_RET ClampPress(PACT pact, UI32 para)
{
	PCLAMP	pp = (PCLAMP)(pact->part);
	ACT_RET ret = ACT_OK;
	int tmp;
	int i;
	
	if(pp == NULL)
		return ACT_ERR_ABORT;

	if(ActIODelayLimit(pp->po->O_CLS_SOL, TRUE))
	{
	    return ACT_ERR_ABORT;
	}
	
	switch (pact->step)
	{
		case  INIT1_STEP:
			pp->mov_start_ms = 0;
			pp->st->DAPRES_CLSHI_MAX = 0;
			if (ChkSysCfg(CO5_ADJMOLD)) //0x0800  松模问题的自动调模		2014.02.20
      	        GetClampclsStartPres(pact,pp->padj);	//	获取锁模力检测时的开始压力

			if(pp->mh->TMDLY_HYDCLSHIGH > 0)
			{				
				CHK_SET_MOV(SetClampLowPres);	//为了衔接关模动作，如果没有此设定。在高压延迟进计时时放开，压力流量不会切
			}

			ACT_STEP_NEW(INIT1_STEP + 1);
			ACT_START();
			pact->start_ms = pp->close_start_ms;
			break;
			
		case INIT1_STEP + 1:
			if(pp->mh->TMDLY_HYDCLSHIGH > 999)
				pp->mh->TMDLY_HYDCLSHIGH = 0;
			
			if(STEP_10MS_CHK(pp->mh->TMDLY_HYDCLSHIGH))
				ACT_STEP_NEW(CLAMPHIPRESS);
			break;

		case CLAMPHIPRESS:
			pp->hipress_start_ms = GetMsTick();
			CHK_SET_MOV(SetClampHiPres);//need to do 为了关闭关模的压力流量
		case CLAMPHIPRESS + 1:
			if(pp->md->TM_CLMPCLSHI > 999)
				pp->md->TM_CLMPCLSHI = 0;
			if((pp->md->TM_CLMPCLSHI>0)&&(pp->md->TM_CLMPCLSHI<999))
			{
				if(MoveLocationTMControlProc((UI32)(pp->md->TM_CLMPCLSHI*10)))
				{
					ACT_STEP_NEW(CLAMPPRESSHOLD);
				}
			}
			else
			{
				if(pp->mh->FL_CLSHIPRES2 ==1)
				{
					if(MoveLocationMeterControlProc(pp->pa->ADPOS_CLSHIPRES2, GetMeterPosByCh(pp->mh->CLMP_METERCH)))	
					{
						ACT_STEP_NEW(CLAMPHIPRES2);
					}
					else if (ChkAdFwdInplace(pp->mh->CLMP_METERCH))
					{
						ACT_STEP_NEW(CLAMPPRESSHOLD);
						pp->clscmpflag = 1;
						pp->clscmpflagmid = 1;
					}
						
				}
				else
				{
					//if (MoveLocationNMControlProc(pp->pi->I_CLS_END)) 
					if (ChkAdFwdInplace(pp->mh->CLMP_METERCH))
					{
						ACT_STEP_NEW(CLAMPPRESSHOLD);
						pp->clscmpflag = 1;
						pp->clscmpflagmid = 1;
					}
				}
				if(ClampHiPressProtectErrChk(pp))
					return ACT_ERR_ABORT;
			}
			CHK_SET_MOV(SetClampHiPres);
			break;
		case CLAMPHIPRES2:
		case CLAMPHIPRES2+1:
			//if (MoveLocationNMControlProc(pp->pi->I_CLS_END)) 
			if (ChkAdFwdInplace(pp->mh->CLMP_METERCH))
			{				
				ACT_STEP_NEW(CLAMPPRESSHOLD);
				pp->clscmpflag = 1;				
				pp->clscmpflagmid = 1;
			}	
			
			if(ClampHiPressProtectErrChk(pp))
				return ACT_ERR_ABORT;			
			CHK_SET_MOV(SetClampHiPres2);
			break;
		case CLAMPPRESSHOLD:
			if(pp->md->TM_HOLDCLSHIGH > 9999)
				pp->md->TM_HOLDCLSHIGH = 0;
			ActSetTM(pact->ptsk, pp->md->TM_HOLDCLSHIGH);
			ACT_STEP_NEW(CLAMPPRESSHOLD + 1);
			break;
		case CLAMPPRESSHOLD + 1:
			if(pp->md->TM_HOLDCLSHIGH > 9999)
				pp->md->TM_HOLDCLSHIGH = 0;
			if(ACT_STEP_100US >= ((UI32)pp->md->TM_HOLDCLSHIGH)*100)
				ACT_STEP_NEW(END_STEP);
			break;
			
		case END_STEP:
			if(pp->mh->TMDLY_VLV_OFFCLSEND > 500)
				pp->mh->TMDLY_VLV_OFFCLSEND = 0;
			if(pp->mh->TMDLY_CLSENDDLY > 999)
				pp->mh->TMDLY_CLSENDDLY = 0;

            if(pp->md->HIPRESS_DEVIATION_FUNCTION == 1)   //高压锁模压力偏差判定
			{				
				if((pp->st->DAPRES_CLSHI_MAX + pp->md->HIPRESS_DEVIATION*10 < pp->pa->DAPRES_CLSHIGH*10)
					|| (pp->st->DAPRES_CLSHI_MAX > pp->pa->DAPRES_CLSHIGH*10 + pp->md->HIPRESS_DEVIATION*10))
				{
					SET_PART_ERR(pp, ER0_HIPRESSERR);
					return ACT_ERR_ABORT;
				}
			}
            
            if(ClpClsInHydrPos(pp))
                SET_OFF(ClampPressOff); //平顺算法下,高压结束走关闭压力/速度斜率
            else
			    SET_OFF(ClampClsOff);
			ACT_STEP_NEW(END_STEP + 1);
			ActSetTM(pact->ptsk, pp->mh->TMDLY_CLSENDDLY);
			pact->mov_st = ACT_MOVING;
			
			if (pp->mh->TMDLY_CLSENDDLY > pp->mh->TMDLY_VLV_OFFCLSEND)
			{
				tmp = pp->mh->TMDLY_CLSENDDLY;
			}
			else
			{
				tmp = pp->mh->TMDLY_VLV_OFFCLSEND;
			}
			
			ActSetMovTM(pact->ptsk, tmp);
			
			for(i=0;i<CLAMP_MAX_CORE;i++)
			{
				if(((pp->pcore[i]->CoreChk>>8)==0) && (pp->pcore[i]->part.act_run_flag == 1))
				{
					pp->pcore[i]->act->ptsk->pst->act_ret = ACT_ERR_ABORT;
					SET_PART_ERR(pp->pcore[i], ER0_CORE);
					return ACT_ERR_ABORT;
				}	
			}
			break;

		case END_STEP + 1:
			if(ACT_STEP_100US >= ((UI32)pp->mh->TMDLY_CLSENDDLY)*100 && GetPOBit(pp->po->O_CLS_SOL) <= 1)
			{
				/*---------关模结束采集相应时间，锁模时间包含关模保持-----------*/
				pp->st->TM_CLAMPCLSHI = (UI16)(DIFF_MS(pp->hipress_start_ms)/10);
				pp->st->TM_CLAMPCLS = (UI16)(DIFF_MS(pp->close_start_ms)/10); 	 	
				pp->st->TM_CLAMPCLS_RT = pp->st->TM_CLAMPCLS;
                pp->st->TM_CLAMPCLS2 = pp->st->TM_CLAMPCLS;
				
				coolstart = GetMsTick();

				if(AutoAlarmCanUpdate())
				{
					pp->st->TM_CLAMPCLS_MAX = pp->st->TM_CLAMPCLS +  (pp->st->TM_CLAMPCLS >> 1) + 20;
					pp->st->TM_CLAMPCLSHI_MAX = pp->st->TM_CLAMPCLSHI + (pp->st->TM_CLAMPCLSHI >> 1) + 20;
				}
				
				ClampCmd( pp, CMD_QC_PARA,1);
				/*--------------------------------------------------------------*/
				
				RobotClampCloseEnd(pp->prbt, TRUE);
				if (ChkSysCfg(CO5_ADJMOLD)) //0x0800  松模问题的自动调模		2014.02.20
				{
				    CheckAdjMold(pp->padj);	//判断是否需要调模
				  	GetAvgPres(pp->padj);	//	记录关模次数并计算锁模力参考值
				} 

				if (ChkSysCfg(XF4_SAVC2))
				{
					PartDataTx((PPART)pp, ST_ID,5,GET_IND(PCLAMP_STATE, TM_CLAMPCLS_MAX),TRUE);	
                    PartDataTx((PPART)pp, ST_ID,1,GET_IND(PCLAMP_STATE, TM_CLAMPCLS2),TRUE);	
				}
				ret = ACT_END;
			}
			else
				pact->mov_st = ACT_MOVING;
			break;
		default:
			ret = ACT_END;
			break;
	}

	if(ret == ACT_END || ret == ACT_ERR_ABORT)
		return ret;
	else
  		return ClampPressBackgrd(pp, pact);
}  

void SetClampClsAdj(PCLAMP pp , PACT pact)
{
	UI16 hydelay;
	if(pp->mh->CLMPCLS_HYDRDELAY > 9999)
		pp->mh->CLMPCLS_HYDRDELAY = 0;
	if(pp->mh->TMDLY_VLV_ONCLS > 500)
		pp->mh->TMDLY_VLV_ONCLS = 0;
	
	hydelay = pp->mh->CLMPCLS_HYDRDELAY;
	
	SetPOBitDelay(pp->po->O_CLS_SOL, PC_ON, pp->mh->TMDLY_VLV_ONCLS);
	SetPOBit(pp->po->O_CLS_SOL2, PC_ON);
	SetPOBit(pp->po->O_CLS_ULT2, PC_ON);

    SetPOBit(pp->po->O_OPN_REL, PC_ON);	
	ActSetHydr_SpdSR(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->mh->DAPRES_CLSSETUP,pp->mh->DAFLOW_CLSSETUP,pp->pressramp_cls[CLP_RAMP_CLS_START]/10,pp->spdramp_cls_manual[CLP_RAMP_CLS_MANUALSTART]/10,
						 pp->pressramp_cls[CLP_RAMP_CLS_START]/10,pp->spdramp_cls[CLP_RAMP_CLS_START]/10, HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_CLAMPCLS),pp->hy->HYDR_PUMPS_CLSSETUP,pp->hy->HYDR_PUMPS_CLSSETUP,
						 pp->hy->SERVO_KP_CLS,pp->hy->SERVO_KI_CLS, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS),pp->md->CLPCLS_SPD_UTI); 
	if(pp->md->FL_CLSOPEN_MODE == 2 || ChkLocateDa(pp))
		ActSet_Da_flow(pact, MC_CTR_MODE_DA, pp->mh->DAFLOW_CLSSETUP, 0,0,
					0,CLAMP_CLS_PID_PORT,0x01, 0,1,HYDR_PRI(pp->hy->HYDR_PORT_CLAMPCLS),CLAMP_CLS_PID_DIREC);	
}


ACT_RET ClampClsAdjBackgrd(PCLAMP pp)
{
	int i;
	
	if(!ChkOperMode(OM_ADJU))
		return ACT_ERR_ABORT;

	for(i=0;i<CLAMP_MAX_CORE;i++)
	{
		if(pp->pcore[i] != NULL)
		{
			if(pp->pcore[i]->md->CORE_LIMIT_CHECK == 1 && CoreLimitSwitchErrCheck(pp->pcore[i])) //调模关模动作过程中检查中子行程开关
				return ACT_ERR_ABORT;
		}
	}
	
	if (FSafeDoorIsCloseErrChk(pp->psfdr))                 //前安全门保护
	{
		return ACT_ERR_ABORT;
	}
	
	if(ClampEjectBwdErrChk(pp))
		return ACT_ERR_ABORT;

	if((CheckPI(pp->pi->I_CLS_UPGA) == 0))
	{
		SET_PART_ERR(pp, ER0_UPGAERR);
		return ACT_ERR_ABORT;
	}

	if (MotorTemperErr())
		return ACT_ERR_ABORT;

	if(RobotClampClsProtectErrChk(pp->prbt))
		return ACT_ERR_ABORT;
	
	return ACT_OK;
}


ACT_RET ClampClsAdj(PACT pact, UI32 para)
{
	PCLAMP	pp = (PCLAMP)(pact->part);
	ACT_RET ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;

	if(ActIODelayLimit(pp->po->O_CLS_SOL, TRUE))
	{
	    return ACT_ERR_ABORT;
	}

	switch (pact->step)
	{
		case INIT1_STEP:
			if((CUS_ID==CS_GDWA)&&(CHK_PART_ERR(pp->psfdr,ER3_FSER)))
   				return ACT_ERR_ABORT;
			else if(ChkSysCfg(XF3_SHUT2)&&(CHK_PART_ERR(pp->psfdr,ER3_FSER)))
				return ACT_ERR_ABORT;
			ACT_STEP_NEW(INIT1_STEP + 1);
			break;

		case INIT1_STEP + 1:
			// 保护机械手  05-12-31
			if(!RobotClampClsErrChk(pp->prbt))         //调模关模新增机械手保护   QRC.20170426
			{
				RobotClampCloseStart(pp->prbt);
				ACT_STEP_NEW(CLAMPCLOSE1);
			}
			break;
			
		case CLAMPCLOSE1:
		case CLAMPCLOSE1 + 1:
			if(!ChkSysCfg(XF5_CLAMPCLS))
			{
				if(GetMeterLenByCh(pp->mh->CLMP_METERCH) > 0) 				/*走电子尺*/
				{
					if(MoveLocationMeterControlProc(pp->pa->ADPOSI_CLSPROTECTEND, GetMeterPosByCh(pp->mh->CLMP_METERCH)))
						ACT_STEP_NEW(CLAMPCLOSE2);
				}
				else 				/*走行程开关*/
				{
					if(MoveLocationNMControlProc(pp->pi->I_CLS_HIP))
						ACT_STEP_NEW(CLAMPCLOSE2);
				}		
			}
			
			CHK_SET_MOV(SetClampClsAdj);
			break;
			
		case CLAMPCLOSE2:
		case CLAMPCLOSE2 + 1:
			if(ChkMachType(MACH_VT))
			{
				ACT_STEP_NEW(END_STEP);	
			}
			else
			{
				//if(MoveLocationNMControlProc(pp->pi->I_CLS_END)) 
				if (ChkAdFwdInplace(pp->mh->CLMP_METERCH))
					ACT_STEP_NEW(END_STEP);			
			}
			CHK_SET_MOV(SetClampClsAdj);
			break;
			
		case END_STEP:
			SET_OFF(ClampClsOff);
			ret = ACT_END;
			break;

		default:
			ret = ACT_END;
			break;
	}

	if(ret == ACT_END || ret == ACT_ERR_ABORT)
		return ret;
	else
		return ClampClsAdjBackgrd(pp);
}  

/*******************************************************************************
	排气压模
*******************************************************************************/  	
ACT_RET ClampPressExhaust(PACT pact, UI32 para)
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
			ACT_STEP_NEW(STAGE_1_STEP);
			break;

		case CLAMPHIPRESS:
		case CLAMPHIPRESS + 1:
			//if(MoveLocationNMControlProc(pp->pi->I_CLS_END))
			if (ChkAdFwdInplace(pp->mh->CLMP_METERCH))
			{
				ACT_STEP_NEW(END_STEP);
			}
			CHK_SET_MOV(SetClampHiPres);
			break;

		case END_STEP:
			SET_OFF(ClampClsOff);
			ret = ACT_END;
			break;
		default:
			ret = ACT_END;
			break;
	}

    if (FSafeDoorIsCloseErrChk(pp->psfdr))                 //前安全门保护
		return ACT_ERR_ABORT;
    
	return ret;
}  

/*******************************************************************************
	开模异常时压模
*******************************************************************************/
ACT_RET ClampPressERR(PACT pact, UI32 para)
{
	PCLAMP pp = (PCLAMP)(pact->part);
	ACT_RET ret = ACT_OK;
	
	if(pp == NULL)
		return ACT_ERR_ABORT;
	
	switch (pact->step)
	{
		case INIT1_STEP:
			CHK_SET_MOV(SetClampHiPres);
			//TM_SPCLPCLSERR=A_100msclock;
			ACT_STEP_NEW(STAGE_1_STEP);
			break;
	
		case STAGE_1_STEP:
		case STAGE_1_STEP + 1:
			//if(STEP_10MS_CHK(TM_SPCLPCLSERR)>=pp->mh->TM_SCLAPMCLS)
			if(STEP_10MS_CHK(pp->mh->TM_SCLAPMCLS))
				ACT_STEP_NEW(END_STEP);
			break;

		case END_STEP:
			SET_OFF(ClampClsOff);
			ret = ACT_END;
			break;
		default:
			ret = ACT_END;
			break;
	}
	
		return ret;
}  

ACT_RET ClampPresMoldClsBackgrd(PCLAMP pp, PACT pact)
{
	int i;
	
	for(i=0;i<CLAMP_MAX_CORE;i++)
	{
		if(pp->pcore[i] != NULL)
		{
			if(pp->pcore[i]->md->CORE_LIMIT_CHECK == 1 && CoreLimitSwitchErrCheck(pp->pcore[i])) //自动调模压模动作过程中检查中子行程开关
				return ACT_ERR_ABORT;	
		}
	}
	
	if((ChkOperMode(OM_MANUAL) && pp->forceclamp ==0)&&ChkMachType(MACH_VT))
		return ACT_ERR_ABORT;

	if(pact->step < STAGE_1_STEP)
		return ACT_OK;
	
	if (FSafeDoorIsCloseErrChk(pp->psfdr))                 //前安全门保护
		return ACT_ERR_ABORT;

	if(ClampClsEjectBwdErrChk(pp))
	{
		if ((pp->pejt[0] !=NULL) && ((UI16)(Get10MsTick() - pp->ClampBwdErr_10ms) > 2))
		{
			if (EjectBwdErrChk(pp->pejt[0], TRUE))
			{
				StandAlarm(FALSE);
			}
			return ACT_ERR_ABORT;
		}
	}
	else
	{
		pp->ClampBwdErr_10ms = Get10MsTick();
	}
	
	if(RobotClampClsProtectErrChk(pp->prbt) || ClampClsSafeProtectErrChk(pp) ||
	    MoldClampClsProtectErrChk(pp->prbt) || (pact->step >= STAGE_1_STEP && ClampClsMecSafeErrChk(pp)))
		return ACT_ERR_ABORT;
	
	if((CheckPI(pp->pi->I_CLS_UPGA) == 0))
	{
		SET_PART_ERR(pp, ER0_UPGAERR);
		return ACT_ERR_ABORT;
	}

	return ACT_OK;
}

/*自动调模压模*/
ACT_RET AutoAdjustPresMold(PACT pact, UI32 para)
{
	PCLAMP pp = (PCLAMP)(pact->part);
	ACT_RET ret = ACT_OK;
	UI16 clampopnslowpos = 0;
	
    if(pp == NULL)
		return ACT_ERR_ABORT;

	if(pp->md->FL_CLSOPEN_MODE == 2)
	{
		clampopnslowpos = pp->pa->ADPOS_OPENSPD1;
	}
	else
	{
		clampopnslowpos = pp->pa->ADPOSI_OPENENDSLOW;
	}

	switch (pact->step)
	{
		case  INIT1_STEP:
            pp->hipress_start_ms = GetMsTick();
			if(pp->padj->mh->TM_AUTOADJCLSPROTECTACT>9999)				
				pp->padj->mh->TM_AUTOADJCLSPROTECTACT= 9999;			
			if(pp->md->TM_CLSHIGHDEFAULTMAX>1000)				
				pp->md->TM_CLSHIGHDEFAULTMAX = 1000;	
			if(!ChkSysCfg(XF2_DPUMP))
			{
				if(pp->padj->adj_mode == 5)
				  ACT_STEP_NEW(CLAMPAUTOADJFIRST);
				else
			      ACT_STEP_NEW(CLAMPAUTOADJAGIN);		
			}
			else
		    	ACT_STEP_NEW(STAGE_3_STEP);
			break;

		case STAGE_3_STEP:
		case STAGE_3_STEP + 1:

			if(GetMeterLenByCh(pp->mh->CLMP_METERCH) > 0) 	
			{
				//if (MoveLocationNMControlProc(pp->pi->I_CLS_END))
				if (ChkAdFwdInplace(pp->mh->CLMP_METERCH))
				{
					if(pp->padj->adj_mode == 5)
						ACT_STEP_NEW(END_STEP);
					else
						ACT_STEP_NEW(CLAMPAUTOADJAGIN+2);
				}
				else
				{
					if(MoveLocationMeterControlProc(pp->pa->ADPOSI_CLSPROTECTEND, GetMeterPosByCh(pp->mh->CLMP_METERCH)))
					{
						if(pp->padj->adj_mode == 5)
				  			ACT_STEP_NEW(CLAMPAUTOADJFIRST);
						else
			      			ACT_STEP_NEW(CLAMPAUTOADJAGIN);
					}
				}

				if(pp->padj->adj_mode == 5)
				{
					if((UI16)(DIFF_MS(pp->hipress_start_ms)/10)>pp->padj->mh->TM_AUTOADJCLSPROTECTACT)
					{
						SET_OFF(ClampClsOff);						
						if((GetMeterPosByCh(pp->mh->CLMP_METERCH)+10) >=clampopnslowpos)
						{
							SET_PART_ERR(pp, ER0_OPNS);
							return ACT_ERR_ABORT;
						}
						else
						{
						   pp->padj->adj_mode = 3;
						   pp->padj->adj_direct = 1;
						   pp->padj->adj_next_bwd_cnt = 128;
						   ret = ACT_END;
						}
					}
				}
				else
				{
					if((UI16)(DIFF_MS(pp->hipress_start_ms)/10)>pp->md->TM_CLSHIGHDEFAULTMAX)
					{
						SET_OFF(ClampClsOff);
					    if(pp->padj->adj_direct == 0)
				            pp->padj->adj_next_bwd_cnt = pp->padj->adj_fwd_cnt/2;
				        else
				            pp->padj->adj_next_bwd_cnt = pp->padj->adj_bwd_cnt/2;	
						if(pp->padj->adj_next_bwd_cnt == 0)
							pp->padj->adj_next_bwd_cnt =1;
						pp->padj->adj_mode = 4;
						pp->padj->adj_direct = 1;
						ret = ACT_END;
					}
				}
			}
			else 			
			{
				if(MoveLocationNMControlProc(pp->pi->I_CLS_HIP))
					ACT_STEP_NEW(END_STEP);
			}
			CHK_SET_MOV(SetClampClsAdj);
			break;
		
		case  CLAMPAUTOADJFIRST:
		case  CLAMPAUTOADJFIRST+1:
		   // if (MoveLocationNMControlProc(pp->pi->I_CLS_END))
		   	if (ChkAdFwdInplace(pp->mh->CLMP_METERCH))
		   	{
				ACT_STEP_NEW(END_STEP);
			}
			else
			{
				if((UI16)(DIFF_MS(pp->hipress_start_ms)/10)>pp->padj->mh->TM_AUTOADJCLSPROTECTACT)
				{
					SET_OFF(ClampClsOff);
					if((GetMeterPosByCh(pp->mh->CLMP_METERCH)+10) >=clampopnslowpos)
					{
						SET_PART_ERR(pp, ER0_OPNS);
						return ACT_ERR_ABORT;
					}
					else
					{
					   pp->padj->adj_mode = 3;
					   pp->padj->adj_direct = 1;
					   pp->padj->adj_next_bwd_cnt = 128;
					   ret = ACT_END;
					}
				}	
			}
			CHK_SET_MOV(SetClampHiPres);
			break;

		case  CLAMPAUTOADJAGIN:
		case  CLAMPAUTOADJAGIN+1:	
			//if (MoveLocationNMControlProc(pp->pi->I_CLS_END))
			if (ChkAdFwdInplace(pp->mh->CLMP_METERCH))
			{
				ACT_STEP_NEW( CLAMPAUTOADJAGIN+2);
			}
			else
			{
				if((UI16)(DIFF_MS(pp->hipress_start_ms)/10)>pp->md->TM_CLSHIGHDEFAULTMAX)
				{
					SET_OFF(ClampClsOff);
				    if(pp->padj->adj_direct == 0)
			            pp->padj->adj_next_bwd_cnt = pp->padj->adj_fwd_cnt/2;
			        else
			            pp->padj->adj_next_bwd_cnt = pp->padj->adj_bwd_cnt/2;	
					if(pp->padj->adj_next_bwd_cnt == 0)
						pp->padj->adj_next_bwd_cnt =1;
					pp->padj->adj_mode = 4;
					pp->padj->adj_direct = 1;
					ret = ACT_END;
				}
			}
			CHK_SET_MOV(SetClampHiPres);
			break;
		case  CLAMPAUTOADJAGIN+2:
			SET_OFF(ClampClsOff);
			if(pp->padj->adj_direct == 0)
			{
			   pp->padj->adj_next_fwd_cnt = pp->padj->adj_fwd_cnt/2;
			   if( pp->padj->adj_next_fwd_cnt == 0)
			   	 pp->padj->adj_next_fwd_cnt = 1;
			   pp->padj->adj_mode = 2;
			   pp->padj->adj_direct = 0;				   
			}
			else
			{
			    if(pp->padj->adj_bwd_cnt == 1)
			      pp->padj->adj_mode = 8;
				else
				{
				  if( pp->padj->adj_bwd_cnt == 2)
					 pp->padj->adj_next_fwd_cnt  = 2;
				  else
			          pp->padj->adj_next_fwd_cnt = pp->padj->adj_bwd_cnt/2;
			      pp->padj->adj_mode = 2;
			      pp->padj->adj_direct = 0;				  
				}
			}
			ret =  ACT_END;
			break;
		case  END_STEP:
			SET_OFF(ClampClsOff);
			pp->padj->adj_mode = 1;
			pp->padj->adj_direct = 0;
			ret =  ACT_END;
			break;
	}

	if(ret == ACT_END || ret == ACT_ERR_ABORT)
		return ret;
	else
  		return ClampPresMoldClsBackgrd(pp, pact);
}
