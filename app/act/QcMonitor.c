/******************************************************************************
  文 件 名   : QcMonitor.c
  版 本 号   : 初稿
  作    者   : zhoutuo
  生成日期   : 2016年5月30日
  最近修改   :
  功能描述   : Qc监控程序
  函数列表   :
  			 GetQcPara
  			 InitQcMonitorPart

  修改历史   :
  1.日    期   : 2016年5月30日
    作    者   :zhotuo
    修改内容   : 创建文件

******************************************************************************/


#include "actcom.h"
#include "QcMonitor.h"
#include "machine.h"

QCMONITOR m_qcmonitor;


/*        QC监控                 2016-06-06*/


PQCMONITOR	GetQc()
{
	return &m_qcmonitor;
}


void QcCntAdd()
{	
	PQCMONITOR pp = &m_qcmonitor;

	if(pp == NULL)
		return;

	if(pp->md->Qc_Fun ==1)
	{
		if((pp->st->Qc_Opn_Cnt==5)&&(pp->qc_chk_start == 0))
		{
			pp->st->Qc_Opn_Cnt = 0;
			pp->qc_chk_start   = 1;
		}
		else
		{
			pp->st->Qc_Opn_Cnt++;	
			
			PartDataTx((PPART)pp, ST_ID, 1, GET_IND(PQCMONITOR_STATE, Qc_Opn_Cnt), FALSE);
		}
	}
}
	



void GetClampClsQc(PCLAMP pclmp)
{

	PQCMONITOR pp = &m_qcmonitor;

	if(pp == NULL)
		return;

	if(ChkOperMode(OM_AUTO) && (pp->md->Qc_Fun==1))
	{
		if(pp->md->Qc_Mode_ClsTm== 1)  //合模时间QC数据
		{
			if(pp->st->Qc_Opn_Cnt == 0)
			{
				pp->qc_avg_opnclstm = pclmp->st->TM_CLAMPCLS;
				pp->st->Qc_Min_Cls	=  pclmp->st->TM_CLAMPCLS;
				pp->st->Qc_Max_Cls	=  pclmp->st->TM_CLAMPCLS; 	
				pp->st->Qc_Unq_Cnt_Cls = 0;
			}
			else
			{
				pp->qc_avg_opnclstm = pp->qc_avg_opnclstm + pclmp->st->TM_CLAMPCLS;
				pp->st->Qc_Min_Cls	=  (pp->st->Qc_Min_Cls>pclmp->st->TM_CLAMPCLS)?pclmp->st->TM_CLAMPCLS: pp->st->Qc_Min_Cls;
				pp->st->Qc_Max_Cls	=  (pp->st->Qc_Max_Cls<pclmp->st->TM_CLAMPCLS)?pclmp->st->TM_CLAMPCLS: pp->st->Qc_Max_Cls;
			}
			if(((pclmp->st->TM_CLAMPCLS>(pp->md->Qc_TmSet_ClsTm+ pp->md->Qc_DeviationSet_ClsTm))||
			(pclmp->st->TM_CLAMPCLS<(pp->md->Qc_TmSet_ClsTm - pp->md->Qc_DeviationSet_ClsTm)))&&((pp->st->Qc_Opn_Cnt+1) <=pp->md->Qc_OpnNum))
				pp->st->Qc_Unq_Cnt_Cls++;						
		}
		if(pp->md->Qc_Mode_ClsLow== 1) //合模低压时间QC数据
		{
			if(pp->st->Qc_Opn_Cnt == 0)
			{
				pp->qc_avg_opnclslow = pclmp->st->TM_CLAMPCLSLO;
				pp->st->Qc_Min_ClsLow  =  pclmp->st->TM_CLAMPCLSLO;
				pp->st->Qc_Max_ClsLow  =  pclmp->st->TM_CLAMPCLSLO;
				pp->st->Qc_Unq_Cnt_ClsLow = 0;
			}
			else
			{
				pp->qc_avg_opnclslow =pp->qc_avg_opnclslow+ pclmp->st->TM_CLAMPCLSLO;
				pp->st->Qc_Min_ClsLow  =  (pp->st->Qc_Min_ClsLow>pclmp->st->TM_CLAMPCLSLO)?pclmp->st->TM_CLAMPCLSLO: pp->st->Qc_Min_ClsLow;
				pp->st->Qc_Max_ClsLow  =  (pp->st->Qc_Max_ClsLow<pclmp->st->TM_CLAMPCLSLO)?pclmp->st->TM_CLAMPCLSLO: pp->st->Qc_Max_ClsLow;
			}
			if(((pclmp->st->TM_CLAMPCLSLO>(pp->md->Qc_TmSet_ClsLow+ pp->md->Qc_DeviationSet_ClsLow))||
			(pclmp->st->TM_CLAMPCLSLO<(pp->md->Qc_TmSet_ClsLow - pp->md->Qc_DeviationSet_ClsLow)))&&((pp->st->Qc_Opn_Cnt+1) <=pp->md->Qc_OpnNum))
				pp->st->Qc_Unq_Cnt_ClsLow++;								
		}
		if(pp->md->Qc_Mode_ClsHi== 1) //合模高压时间QC数据
		{
			if(pp->st->Qc_Opn_Cnt == 0)
			{
				pp->qc_avg_opnclshi = pclmp->st->TM_CLAMPCLSHI;
				pp->st->Qc_Min_ClsHi  =pclmp->st->TM_CLAMPCLSHI;
				pp->st->Qc_Max_ClsHi  =pclmp->st->TM_CLAMPCLSHI;
				pp->st->Qc_Unq_Cnt_ClsHi = 0;
			}
			else
			{
				pp->qc_avg_opnclshi = pp->qc_avg_opnclshi+pclmp->st->TM_CLAMPCLSHI;
				pp->st->Qc_Min_ClsHi	=  (pp->st->Qc_Min_ClsHi>pclmp->st->TM_CLAMPCLSHI)?pclmp->st->TM_CLAMPCLSHI: pp->st->Qc_Min_ClsHi;
				pp->st->Qc_Max_ClsHi	=  (pp->st->Qc_Max_ClsHi<pclmp->st->TM_CLAMPCLSHI)?pclmp->st->TM_CLAMPCLSHI: pp->st->Qc_Max_ClsHi;
			}
			if(((pclmp->st->TM_CLAMPCLSHI>(pp->md->Qc_TmSet_ClsHi+ pp->md->Qc_DeviationSet_ClsHi))||
			(pclmp->st->TM_CLAMPCLSHI<(pp->md->Qc_TmSet_ClsHi - pp->md->Qc_DeviationSet_ClsHi)))&&((pp->st->Qc_Opn_Cnt+1) <=pp->md->Qc_OpnNum))
				pp->st->Qc_Unq_Cnt_ClsHi++; 							
		}										
	}		
}

void GetClampOpnQc(PCLAMP pclmp)
{
	PQCMONITOR pp = &m_qcmonitor;

	if(pp == NULL)
		return;

	if(ChkOperMode(OM_AUTO) && (pp->md->Qc_Fun==1))
	{
		if(pp->md->Qc_Mode_OpnTm==1)//开模QC数据处理
		{
			if(pp->st->Qc_Opn_Cnt == 0)
			{
				pp->qc_avg_opntm = pclmp->st->TM_CLAMPOPN;
				pp->st->Qc_Min_OpnTm = pclmp->st->TM_CLAMPOPN;
				pp->st->Qc_Max_OpnTm = pclmp->st->TM_CLAMPOPN;
				pp->st->Qc_Unq_Cnt_OpnTm = 0;
			}
			else
			{
				pp->qc_avg_opntm =pp->qc_avg_opntm+ pclmp->st->TM_CLAMPOPN;
				pp->st->Qc_Min_OpnTm	=  (pp->st->Qc_Min_OpnTm>pclmp->st->TM_CLAMPOPN)?pclmp->st->TM_CLAMPOPN: pp->st->Qc_Min_OpnTm;
				pp->st->Qc_Max_OpnTm	=  (pp->st->Qc_Max_OpnTm<pclmp->st->TM_CLAMPOPN)?pclmp->st->TM_CLAMPOPN: pp->st->Qc_Max_OpnTm;		
			}
			if(((pclmp->st->TM_CLAMPOPN>(pp->md->Qc_TmSet_OpnTm+ pp->md->Qc_DeviationSet_OpnTm))||
			 (pclmp->st->TM_CLAMPOPN<(pp->md->Qc_TmSet_OpnTm - pp->md->Qc_DeviationSet_OpnTm)))&&((pp->st->Qc_Opn_Cnt+1) <=pp->md->Qc_OpnNum))
				pp->st->Qc_Unq_Cnt_OpnTm++; 						
		}
		if(pp->md->Qc_Mode_OpnEnd==1)//开模QC数据处理
		{
			if(pp->st->Qc_Opn_Cnt == 0)
			{
				pp->qc_avg_opnend = pclmp->st->ADPOSI_OPNEND;
				pp->st->Qc_Min_OpnEnd	= pclmp->st->ADPOSI_OPNEND;
				pp->st->Qc_Max_OpnEnd	= pclmp->st->ADPOSI_OPNEND; 
				pp->st->Qc_Unq_Cnt_OpnEnd = 0;
			}
			else
			{
				pp->qc_avg_opnend = pp->qc_avg_opnend +pclmp->st->ADPOSI_OPNEND;
				pp->st->Qc_Min_OpnEnd	=  (pp->st->Qc_Min_OpnEnd>pclmp->st->ADPOSI_OPNEND)?pclmp->st->ADPOSI_OPNEND: pp->st->Qc_Min_OpnEnd;
				pp->st->Qc_Max_OpnEnd	=  (pp->st->Qc_Max_OpnEnd<pclmp->st->ADPOSI_OPNEND)?pclmp->st->ADPOSI_OPNEND: pp->st->Qc_Max_OpnEnd;		
			}
			if(((pclmp->st->ADPOSI_OPNEND>(pp->md->Qc_TmSet_OpnEnd+ pp->md->Qc_DeviationSet_OpnEnd))||
			 (pclmp->st->ADPOSI_OPNEND<(pp->md->Qc_TmSet_OpnEnd - pp->md->Qc_DeviationSet_OpnEnd)))&&((pp->st->Qc_Opn_Cnt+1) <=pp->md->Qc_OpnNum))
				pp->st->Qc_Unq_Cnt_OpnEnd++;								
		}
		if(pp->md->Qc_Mode_OpnSpd==1)//开模QC数据处理
		{
			UI32 spd = (pclmp->st->ADPOSI_OPNEND-pp->qc_pos_opnstart)/pclmp->st->TM_CLAMPOPN;
			if(pp->st->Qc_Opn_Cnt == 0)
			{
				pp->qc_avg_opnspd = spd;
				pp->st->Qc_Min_OpnSpd = spd;
				pp->st->Qc_Max_OpnSpd =  spd;
				pp->st->Qc_Unq_Cnt_OpnSpd = 0;
			}
			else
			{
				pp->qc_avg_opnspd =pp->qc_avg_opnspd+ spd;
				pp->st->Qc_Min_OpnSpd=	(pp->st->Qc_Min_OpnSpd>spd)?spd: pp->st->Qc_Min_OpnSpd;
				pp->st->Qc_Max_OpnSpd=	(pp->st->Qc_Max_OpnSpd<spd)?spd: pp->st->Qc_Max_OpnSpd; 
			}
			if(((spd>(pp->md->Qc_TmSet_OpnSpd+ pp->md->Qc_DeviationSet_OpnSpd))||
			 (spd<(pp->md->Qc_TmSet_OpnSpd - pp->md->Qc_DeviationSet_OpnSpd)))&&((pp->st->Qc_Opn_Cnt+1) <=pp->md->Qc_OpnNum))
				pp->st->Qc_Unq_Cnt_OpnSpd++;								
		}				
	}

}


void GetInjectQc(PINJECT pinj)
{
	PQCMONITOR pp = &m_qcmonitor;

	if(pp == NULL)
		return;

	if(ChkOperMode(OM_AUTO) && (pp->md->Qc_Fun==1))
	{
		if(pp->md->Qc_Mode_InjTm== 1)  //射出时间QC数据
		{
			if(pp->st->Qc_Opn_Cnt == 0)
			{
				pp->qc_avg_injtm = pinj->st->TM_INJ;
				pp->st->Qc_Min_InjTm = pinj->st->TM_INJ;
				pp->st->Qc_Max_InjTm = pinj->st->TM_INJ;
				pp->st->Qc_Unq_Cnt_InjTm = 0;
			}
			else
			{
				pp->qc_avg_injtm = pp->qc_avg_injtm+pinj->st->TM_INJ;
				pp->st->Qc_Min_InjTm=  (pp->st->Qc_Min_InjTm>pinj->st->TM_INJ)?pinj->st->TM_INJ: pp->st->Qc_Min_InjTm;
				pp->st->Qc_Max_InjTm=  (pp->st->Qc_Max_InjTm<pinj->st->TM_INJ)?pinj->st->TM_INJ: pp->st->Qc_Max_InjTm;
			}
			if(((pinj->st->TM_INJ>(pp->md->Qc_TmSet_InjTm+ pp->md->Qc_DeviationSet_InjTm))||
			 (pinj->st->TM_INJ<(pp->md->Qc_TmSet_InjTm - pp->md->Qc_DeviationSet_InjTm)))&&((pp->st->Qc_Opn_Cnt+1) <=pp->md->Qc_OpnNum))
					pp->st->Qc_Unq_Cnt_InjTm++;											
		}
		if(pp->md->Qc_Mode_InjSpd== 1)  //射出速度QC数据
		{
			if(pp->st->Qc_Opn_Cnt == 0)
			{
				pp->qc_avg_injspd = pinj->st->INJECT_AVERAGESPEED;
				pp->st->Qc_Min_InjSpd = pinj->st->INJECT_AVERAGESPEED;
				pp->st->Qc_Max_InjSpd = pinj->st->INJECT_AVERAGESPEED;
				pp->st->Qc_Unq_Cnt_InjSpd = 0;
			}
			else
			{
				pp->qc_avg_injspd =pp->qc_avg_injspd+ pinj->st->INJECT_AVERAGESPEED;
				pp->st->Qc_Min_InjSpd=  (pp->st->Qc_Min_InjSpd>pinj->st->INJECT_AVERAGESPEED)?pinj->st->INJECT_AVERAGESPEED: pp->st->Qc_Min_InjSpd;
				pp->st->Qc_Max_InjSpd=  (pp->st->Qc_Max_InjSpd<pinj->st->INJECT_AVERAGESPEED)?pinj->st->INJECT_AVERAGESPEED: pp->st->Qc_Max_InjSpd;
			}
			if(((pinj->st->INJECT_AVERAGESPEED>(pp->md->Qc_TmSet_InjSpd+ pp->md->Qc_DeviationSet_InjSpd))||
			 (pinj->st->INJECT_AVERAGESPEED<(pp->md->Qc_TmSet_InjSpd - pp->md->Qc_DeviationSet_InjSpd)))&&((pp->st->Qc_Opn_Cnt+1) <=pp->md->Qc_OpnNum))
				pp->st->Qc_Unq_Cnt_InjSpd++;									
		}		
			if(pp->md->Qc_Mode_InjEnd== 1)  //保压转换位置QC数据
			{
				if(pp->st->Qc_Opn_Cnt == 0)
				{
					pp->qc_avg_injend = pinj->st->ADPOSI_INJCUSHION;
					pp->st->Qc_Min_InjEnd = pinj->st->ADPOSI_INJCUSHION;
					pp->st->Qc_Max_InjEnd = pinj->st->ADPOSI_INJCUSHION;
					pp->st->Qc_Unq_Cnt_InjEnd = 0;
				}
				else
				{
					pp->qc_avg_injend =pp->qc_avg_injend+ pinj->st->ADPOSI_INJCUSHION;
					pp->st->Qc_Min_InjEnd=  (pp->st->Qc_Min_InjEnd>pinj->st->ADPOSI_INJCUSHION)?pinj->st->ADPOSI_INJCUSHION: pp->st->Qc_Min_InjEnd;
					pp->st->Qc_Max_InjEnd=  (pp->st->Qc_Max_InjEnd<pinj->st->ADPOSI_INJCUSHION)?pinj->st->ADPOSI_INJCUSHION: pp->st->Qc_Max_InjEnd;
				}
				if(((pinj->st->ADPOSI_INJCUSHION>(pp->md->Qc_TmSet_InjEnd+ pp->md->Qc_DeviationSet_InjEnd))||
				 (pinj->st->ADPOSI_INJCUSHION<(pp->md->Qc_TmSet_InjEnd - pp->md->Qc_DeviationSet_InjEnd)))&&((pp->st->Qc_Opn_Cnt+1) <=pp->md->Qc_OpnNum))
					pp->st->Qc_Unq_Cnt_InjEnd++;							
			}	
			if(pp->md->Qc_Mode_InjPrs== 1)  //射出压力QC数据
			{
				if(pp->st->Qc_Opn_Cnt == 0)
				{
					pp->qc_avg_injpres = pinj->st->INJECT_MAXPRESS;
					pp->st->Qc_Min_InjPrs = pinj->st->INJECT_MAXPRESS;
					pp->st->Qc_Max_InjPrs = pinj->st->INJECT_MAXPRESS;
					pp->st->Qc_Unq_Cnt_InjPrs = 0;
				}
				else
				{
					pp->qc_avg_injpres = pp->qc_avg_injpres+pinj->st->INJECT_MAXPRESS;
					pp->st->Qc_Min_InjPrs=  (pp->st->Qc_Min_InjPrs>pinj->st->INJECT_MAXPRESS)?pinj->st->INJECT_MAXPRESS: pp->st->Qc_Min_InjPrs;
					pp->st->Qc_Max_InjPrs=  (pp->st->Qc_Max_InjPrs<pinj->st->INJECT_MAXPRESS)?pinj->st->INJECT_MAXPRESS: pp->st->Qc_Max_InjPrs;
				}
				if(((pinj->st->INJECT_MAXPRESS>(pp->md->Qc_TmSet_InjPrs+ pp->md->Qc_DeviationSet_InjPrs))||
				 (pinj->st->INJECT_MAXPRESS<(pp->md->Qc_TmSet_InjPrs - pp->md->Qc_DeviationSet_InjPrs)))&&((pp->st->Qc_Opn_Cnt+1) <=pp->md->Qc_OpnNum))
					pp->st->Qc_Unq_Cnt_InjPrs++;							
			}					
			if(pp->md->Qc_Mode_HoldEnd== 1)  //残料位置QC数据
			{
			//	pp->st->Qc_Avg_HoldEnd=  pp->st->Qc_Avg_HoldEnd+pinj->st->ADPOSI_INJHOLDEND;
				if(pp->st->Qc_Opn_Cnt == 0)
				{
					pp->qc_avg_holdend = pinj->st->ADPOSI_INJHOLDEND;
					pp->st->Qc_Min_HoldEnd = pinj->st->ADPOSI_INJHOLDEND;
					pp->st->Qc_Max_InjEnd = pinj->st->ADPOSI_INJHOLDEND;
					pp->st->Qc_Unq_Cnt_HoldEnd = 0;
				}
				else
				{
					pp->qc_avg_holdend = pp->qc_avg_holdend+pinj->st->ADPOSI_INJHOLDEND;
					pp->st->Qc_Min_HoldEnd=  (pp->st->Qc_Min_HoldEnd>pinj->st->ADPOSI_INJHOLDEND)?pinj->st->ADPOSI_INJHOLDEND: pp->st->Qc_Min_HoldEnd;
					pp->st->Qc_Max_HoldEnd=  (pp->st->Qc_Max_HoldEnd<pinj->st->ADPOSI_INJHOLDEND)?pinj->st->ADPOSI_INJHOLDEND: pp->st->Qc_Max_HoldEnd;
				}
				if(((pinj->st->ADPOSI_INJHOLDEND>(pp->md->Qc_TmSet_HoldEnd+ pp->md->Qc_DeviationSet_HoldEnd))||
				 (pinj->st->ADPOSI_INJHOLDEND<(pp->md->Qc_TmSet_HoldEnd - pp->md->Qc_DeviationSet_HoldEnd)))&&((pp->st->Qc_Opn_Cnt+1) <=pp->md->Qc_OpnNum))
					pp->st->Qc_Unq_Cnt_HoldEnd++;						
			}						
	}
					
}


void GetChargeQc(PINJECT pinj)
{
	PQCMONITOR pp = &m_qcmonitor;
	if(pp == NULL)
		return;
	

	if(ChkOperMode(OM_AUTO) && (pp->md->Qc_Fun==1))
	{

		if(pp->md->Qc_Mode_ChgTm== 1)  //储料时间QC数据
		{
			if(pp->st->Qc_Opn_Cnt == 0)
			{
				pp->qc_avg_chgtm = pinj->st->TM_CHARGE;
				pp->st->Qc_Min_ChgTm = pinj->st->TM_CHARGE;
				pp->st->Qc_Max_ChgTm = pinj->st->TM_CHARGE;
				pp->st->Qc_Unq_Cnt_ChgTm = 0;
			}
			else
			{
				pp->qc_avg_chgtm = pp->qc_avg_chgtm+pinj->st->TM_CHARGE;
				pp->st->Qc_Min_ChgTm=  (pp->st->Qc_Min_ChgTm>pinj->st->TM_CHARGE)?pinj->st->TM_CHARGE: pp->st->Qc_Min_ChgTm;
				pp->st->Qc_Max_ChgTm=  (pp->st->Qc_Max_ChgTm<pinj->st->TM_CHARGE)?pinj->st->TM_CHARGE: pp->st->Qc_Max_ChgTm;
			}
			if(((pinj->st->TM_CHARGE>(pp->md->Qc_TmSet_ChgTm+ pp->md->Qc_DeviationSet_ChgTm))||
			 (pinj->st->TM_CHARGE<(pp->md->Qc_TmSet_ChgTm - pp->md->Qc_DeviationSet_ChgTm)))&&((pp->st->Qc_Opn_Cnt+1) <=pp->md->Qc_OpnNum))
				pp->st->Qc_Unq_Cnt_ChgTm++; 						
		}		
		if(pp->md->Qc_Mode_ChgEnd== 1)	//储料终止位置QC数据
		{
			if(pp->st->Qc_Opn_Cnt == 0)
			{
				pp->qc_avg_chgend = pinj->st->ADPOSI_CHGEND;
				pp->st->Qc_Min_ChgEnd = pinj->st->ADPOSI_CHGEND;
				pp->st->Qc_Max_ChgEnd = pinj->st->ADPOSI_CHGEND; 
				pp->st->Qc_Unq_Cnt_ChgEnd = 0;
			}
			else
			{
				pp->qc_avg_chgend = pp->qc_avg_chgend+pinj->st->ADPOSI_CHGEND;
				pp->st->Qc_Min_ChgEnd=	(pp->st->Qc_Min_ChgEnd>pinj->st->ADPOSI_CHGEND)?pinj->st->ADPOSI_CHGEND: pp->st->Qc_Min_ChgEnd;
				pp->st->Qc_Max_ChgEnd=	(pp->st->Qc_Max_ChgEnd<pinj->st->ADPOSI_CHGEND)?pinj->st->ADPOSI_CHGEND: pp->st->Qc_Max_ChgEnd;
			}
			if(((pinj->st->ADPOSI_CHGEND>(pp->md->Qc_TmSet_ChgEnd+ pp->md->Qc_DeviationSet_ChgEnd))||
			 (pinj->st->ADPOSI_CHGEND<(pp->md->Qc_TmSet_ChgEnd - pp->md->Qc_DeviationSet_ChgEnd)))&&((pp->st->Qc_Opn_Cnt+1) <=pp->md->Qc_OpnNum))
				pp->st->Qc_Unq_Cnt_ChgEnd++;									
		}			
	}
}

void GetSuckbackQc(PINJECT pinj)
{
	PQCMONITOR pp = &m_qcmonitor;
	if(pp == NULL)
		return;

	if(ChkOperMode(OM_AUTO) && (pp->md->Qc_Fun==1))
	{
		if(pp->md->Qc_Mode_SuckTm== 1)	//射退时间QC数据
		{
			if(pp->st->Qc_Opn_Cnt == 0)
			{
				pp->qc_avg_sucktm = pinj->st->TM_SUCKBACK1;
				pp->st->Qc_Min_SuckTm = pinj->st->TM_SUCKBACK1;
				pp->st->Qc_Max_SuckTm =  pinj->st->TM_SUCKBACK1;
				pp->st->Qc_Unq_Cnt_SuckTm = 0;
			}
			else
			{
				pp->qc_avg_sucktm = pp->qc_avg_sucktm+pinj->st->TM_SUCKBACK1;
				pp->st->Qc_Min_SuckTm=	(pp->st->Qc_Min_SuckTm>pinj->st->TM_SUCKBACK1)?pinj->st->TM_SUCKBACK1: pp->st->Qc_Min_SuckTm;
				pp->st->Qc_Max_SuckTm=	(pp->st->Qc_Max_SuckTm<pinj->st->TM_SUCKBACK1)?pinj->st->TM_SUCKBACK1: pp->st->Qc_Max_SuckTm;
			}
			if(((pinj->st->TM_SUCKBACK1>(pp->md->Qc_TmSet_SuckTm+ pp->md->Qc_DeviationSet_SuckTm))||
			 (pinj->st->TM_SUCKBACK1<(pp->md->Qc_TmSet_SuckTm - pp->md->Qc_DeviationSet_SuckTm)))&&((pp->st->Qc_Opn_Cnt+1) <=pp->md->Qc_OpnNum))
				pp->st->Qc_Unq_Cnt_SuckTm++;								
		}	
		if(pp->md->Qc_Mode_SuckBwd== 1)  //射退终止位置QC数据
		{
			if(pp->st->Qc_Opn_Cnt == 0)
			{
				pp->qc_avg_suckend = pinj->st->ADPOSI_SUCKBACK;
				pp->st->Qc_Min_SuckBwd = pinj->st->ADPOSI_SUCKBACK;
				pp->st->Qc_Max_SuckBwd = pinj->st->ADPOSI_SUCKBACK;
				pp->st->Qc_Unq_Cnt_SuckBwd	= 0;
			}
			else
			{
				pp->qc_avg_suckend = pp->qc_avg_suckend+pinj->st->ADPOSI_SUCKBACK;
				pp->st->Qc_Min_SuckBwd=  (pp->st->Qc_Min_SuckBwd>pinj->st->ADPOSI_SUCKBACK)?pinj->st->ADPOSI_SUCKBACK: pp->st->Qc_Min_SuckBwd;
				pp->st->Qc_Max_SuckBwd=  (pp->st->Qc_Max_SuckBwd<pinj->st->ADPOSI_SUCKBACK)?pinj->st->ADPOSI_SUCKBACK: pp->st->Qc_Max_SuckBwd;
			}
			if(((pinj->st->ADPOSI_SUCKBACK>(pp->md->Qc_TmSet_SuckBwd+ pp->md->Qc_DeviationSet_SuckBwd))||
			 (pinj->st->ADPOSI_SUCKBACK<(pp->md->Qc_TmSet_SuckBwd - pp->md->Qc_DeviationSet_SuckBwd)))&&((pp->st->Qc_Opn_Cnt+1) <=pp->md->Qc_OpnNum))
				pp->st->Qc_Unq_Cnt_SuckBwd++;							
		}
	}
											

}

void GetEjectQc(PEJECT pejt)
{
	PQCMONITOR pp = &m_qcmonitor;
	if(pp == NULL)
		return;

	if(ChkOperMode(OM_AUTO) && (pp->md->Qc_Fun==1))
	{
		if(pp->md->Qc_Mode_EjtTm== 1)  //托摸时间QC数据
		{
			if(pp->st->Qc_Opn_Cnt == 0)
			{
				pp->qc_avg_ejttm = pejt->st->TM_EJECT;
				pp->st->Qc_Min_EjtTm = pejt->st->TM_EJECT;
				pp->st->Qc_Max_EjtTm = pejt->st->TM_EJECT;
				pp->st->Qc_Unq_Cnt_EjtTm = 0;
			}
			else
			{
				pp->qc_avg_ejttm = pp->qc_avg_ejttm+pejt->st->TM_EJECT;
				pp->st->Qc_Min_EjtTm=  (pp->st->Qc_Min_EjtTm>pejt->st->TM_EJECT)?pejt->st->TM_EJECT: pp->st->Qc_Min_EjtTm;
				pp->st->Qc_Max_EjtTm=  (pp->st->Qc_Max_EjtTm<pejt->st->TM_EJECT)?pejt->st->TM_EJECT: pp->st->Qc_Max_EjtTm;
			}
			if(((pejt->st->TM_EJECT>(pp->md->Qc_TmSet_EjtTm+ pp->md->Qc_DeviationSet_EjtTm))||
			(pejt->st->TM_EJECT<(pp->md->Qc_TmSet_EjtTm - pp->md->Qc_DeviationSet_EjtTm)))&&((pp->st->Qc_Opn_Cnt+1) <=pp->md->Qc_OpnNum))
				pp->st->Qc_Unq_Cnt_EjtTm++; 					
		}
	}

}

void GetMachineQc()
{
	PQCMONITOR pp = &m_qcmonitor;
	PMACHINE pmac = &m_machine;

	if(pp == NULL)
		return;

	if(ChkOperMode(OM_AUTO) && (pp->md->Qc_Fun==1))
	{
		if(pp->md->Qc_Mode_CycTm== 1)
		{
			if(pp->st->Qc_Opn_Cnt == 1)
			{
				pp->qc_avg_cyctm = pmac->st->TM_CYCLE_LAST;
				pp->st->Qc_Min_CycTm = pmac->st->TM_CYCLE_LAST;
				pp->st->Qc_Max_CycTm =  pmac->st->TM_CYCLE_LAST;
				pp->st->Qc_Unq_Cnt_CycTm = 0;
			}
			else
			{
				pp->qc_avg_cyctm = pp->qc_avg_cyctm+pmac->st->TM_CYCLE_LAST;
				pp->st->Qc_Min_CycTm=  (pp->st->Qc_Min_CycTm>pmac->st->TM_CYCLE_LAST)?pmac->st->TM_CYCLE_LAST: pp->st->Qc_Min_CycTm;
				pp->st->Qc_Max_CycTm=  (pp->st->Qc_Max_CycTm<pmac->st->TM_CYCLE_LAST)?pmac->st->TM_CYCLE_LAST: pp->st->Qc_Max_CycTm;
			}
			if(((pmac->st->TM_CYCLE_LAST>(pp->md->Qc_TmSet_CycTm+ pp->md->Qc_DeviationSet_CycTm))||
			(pmac->st->TM_CYCLE_LAST<(pp->md->Qc_TmSet_CycTm - pp->md->Qc_DeviationSet_CycTm)))&&((pp->st->Qc_Opn_Cnt) <=pp->md->Qc_OpnNum))
	 			pp->st->Qc_Unq_Cnt_CycTm = pp->st->Qc_Unq_Cnt_CycTm+1;								
		}
	}	
}	
BOOL GetQcPara()
{
	PQCMONITOR pp = &m_qcmonitor;

	if(pp == NULL)
		return FALSE;

	if(ChkOperMode(OM_AUTO) && (pp->md->Qc_Fun==1))
	{

		if(pp->st->Qc_Opn_Cnt<=pp->md->Qc_OpnNum)
		{

			if(pp->st->Qc_Opn_Cnt>=pp->md->Qc_OpnNum)
			{
				if(pp->md->Qc_Mode_OpnEnd == 1)
				{
					pp->st->Qc_Avg_OpnEnd=( (pp->qc_avg_opnend)/(pp->st->Qc_Opn_Cnt));  
					pp->st->Qc_Deviation_Max_OpnEnd  =  pp->st->Qc_Max_OpnEnd - pp->st->Qc_Min_OpnEnd;
					pp->st->Qc_Unq_Rate_OpnEnd= (pp->st->Qc_Unq_Cnt_OpnEnd*100/pp->st->Qc_Opn_Cnt);
				}
				if(pp->md->Qc_Mode_OpnSpd== 1)
				{
					pp->st->Qc_Avg_OpnSpd= pp->qc_avg_opnspd/pp->st->Qc_Opn_Cnt;  
					pp->st->Qc_Deviation_Max_OpnSpd  =  pp->st->Qc_Max_OpnSpd - pp->st->Qc_Min_OpnSpd;
					pp->st->Qc_Unq_Rate_OpnSpd= pp->st->Qc_Unq_Cnt_OpnSpd*100/pp->st->Qc_Opn_Cnt;
				}			
				if(pp->md->Qc_Mode_OpnTm== 1)
				{
					pp->st->Qc_Avg_OpnTm= pp->qc_avg_opntm/pp->st->Qc_Opn_Cnt;  
					pp->st->Qc_Deviation_Max_OpnTm  =  pp->st->Qc_Max_OpnTm - pp->st->Qc_Min_OpnTm;
					pp->st->Qc_Unq_Rate_OpnTm= pp->st->Qc_Unq_Cnt_OpnTm*100/pp->st->Qc_Opn_Cnt;
				}					
				if(pp->md->Qc_Mode_ClsTm== 1)
				{
					pp->st->Qc_Avg_Cls= pp->qc_avg_opnclstm/pp->st->Qc_Opn_Cnt;  
					pp->st->Qc_Deviation_Max_Cls=  pp->st->Qc_Max_Cls- pp->st->Qc_Min_Cls;
					pp->st->Qc_Unq_Rate_Cls= pp->st->Qc_Unq_Cnt_Cls*100/pp->st->Qc_Opn_Cnt;
				}		
				if(pp->md->Qc_Mode_ClsLow== 1)
				{
					pp->st->Qc_Avg_ClsLow= pp->qc_avg_opnclslow/pp->st->Qc_Opn_Cnt;  
					pp->st->Qc_Deviation_Max_ClsLow=  pp->st->Qc_Max_ClsLow- pp->st->Qc_Min_ClsLow;
					pp->st->Qc_Unq_Rate_ClsLow= pp->st->Qc_Unq_Cnt_ClsLow*100/pp->st->Qc_Opn_Cnt;
				}				
				if(pp->md->Qc_Mode_ClsHi== 1)
				{
					pp->st->Qc_Avg_ClsHi= pp->qc_avg_opnclshi/pp->st->Qc_Opn_Cnt;  
					pp->st->Qc_Deviation_Max_ClsHi=  pp->st->Qc_Max_ClsHi- pp->st->Qc_Min_ClsHi;
					pp->st->Qc_Unq_Rate_ClsHi= pp->st->Qc_Unq_Cnt_ClsHi*100/pp->st->Qc_Opn_Cnt;
				}		
				if(pp->md->Qc_Mode_CycTm== 1)
				{
					pp->st->Qc_Avg_CycTm= pp->qc_avg_cyctm/pp->st->Qc_Opn_Cnt;  
					pp->st->Qc_Deviation_Max_CycTm=  pp->st->Qc_Max_CycTm- pp->st->Qc_Min_CycTm;
					pp->st->Qc_Unq_Rate_CycTm= pp->st->Qc_Unq_Cnt_CycTm*100/pp->st->Qc_Opn_Cnt;
				}			
				if(pp->md->Qc_Mode_EjtTm== 1)
				{
					pp->st->Qc_Avg_EjtTm= pp->qc_avg_ejttm/pp->st->Qc_Opn_Cnt;  
					pp->st->Qc_Deviation_Max_EjtTm=  pp->st->Qc_Max_EjtTm- pp->st->Qc_Min_EjtTm;
					pp->st->Qc_Unq_Rate_EjtTm= pp->st->Qc_Unq_Cnt_EjtTm*100/pp->st->Qc_Opn_Cnt;
				}		
				if(pp->md->Qc_Mode_ChgEnd== 1)
				{
					pp->st->Qc_Avg_ChgEnd= pp->qc_avg_chgend/pp->st->Qc_Opn_Cnt;  
					pp->st->Qc_Deviation_Max_ChgEnd=  pp->st->Qc_Max_ChgEnd- pp->st->Qc_Min_ChgEnd;
					pp->st->Qc_Unq_Rate_ChgEnd= pp->st->Qc_Unq_Cnt_ChgEnd*100/pp->st->Qc_Opn_Cnt;
				}		
				if(pp->md->Qc_Mode_ChgTm== 1)
				{
					pp->st->Qc_Avg_ChgTm= pp->qc_avg_chgtm/pp->st->Qc_Opn_Cnt;  
					pp->st->Qc_Deviation_Max_ChgTm=  pp->st->Qc_Max_ChgTm- pp->st->Qc_Min_ChgTm;
					pp->st->Qc_Unq_Rate_ChgTm= pp->st->Qc_Unq_Cnt_ChgTm*100/pp->st->Qc_Opn_Cnt;
				}		
				if(pp->md->Qc_Mode_HoldEnd== 1)
				{
					pp->st->Qc_Avg_HoldEnd= pp->qc_avg_holdend/pp->st->Qc_Opn_Cnt;  
					pp->st->Qc_Deviation_Max_HoldEnd=  pp->st->Qc_Max_HoldEnd- pp->st->Qc_Min_HoldEnd;
					pp->st->Qc_Unq_Rate_HoldEnd= pp->st->Qc_Unq_Cnt_HoldEnd*100/pp->st->Qc_Opn_Cnt;
				}	
				if(pp->md->Qc_Mode_InjEnd== 1)
				{
					pp->st->Qc_Avg_InjEnd= pp->qc_avg_injend/pp->st->Qc_Opn_Cnt;  
					pp->st->Qc_Deviation_Max_InjEnd=  pp->st->Qc_Max_InjEnd- pp->st->Qc_Min_InjEnd;
					pp->st->Qc_Unq_Rate_InjEnd= pp->st->Qc_Unq_Cnt_InjEnd*100/pp->st->Qc_Opn_Cnt;
				}	
				if(pp->md->Qc_Mode_InjPrs== 1)
				{
					pp->st->Qc_Avg_InjPrs= pp->qc_avg_injpres/pp->st->Qc_Opn_Cnt;  
					pp->st->Qc_Deviation_Max_InjPrs=  pp->st->Qc_Max_InjPrs- pp->st->Qc_Min_InjPrs;
					pp->st->Qc_Unq_Rate_InjPrs= pp->st->Qc_Unq_Cnt_InjPrs*100/pp->st->Qc_Opn_Cnt;
				}	
				if(pp->md->Qc_Mode_InjSpd== 1)
				{
					pp->st->Qc_Avg_InjSpd= pp->qc_avg_injspd/pp->st->Qc_Opn_Cnt;  
					pp->st->Qc_Deviation_Max_InjSpd=  pp->st->Qc_Max_InjSpd- pp->st->Qc_Min_InjSpd;
					pp->st->Qc_Unq_Rate_InjSpd= pp->st->Qc_Unq_Cnt_InjSpd*100/pp->st->Qc_Opn_Cnt;
				}
				if(pp->md->Qc_Mode_InjTm== 1)
				{
					pp->st->Qc_Avg_InjTm= pp->qc_avg_injtm/pp->st->Qc_Opn_Cnt;  
					pp->st->Qc_Deviation_Max_InjTm=  pp->st->Qc_Max_InjTm- pp->st->Qc_Min_InjTm;
					pp->st->Qc_Unq_Rate_InjTm= pp->st->Qc_Unq_Cnt_InjTm*100/pp->st->Qc_Opn_Cnt;
				}		
				if(pp->md->Qc_Mode_SuckBwd== 1)
				{
					pp->st->Qc_Avg_SuckBwd= pp->qc_avg_suckend/pp->st->Qc_Opn_Cnt;  
					pp->st->Qc_Deviation_Max_SuckBwd=  pp->st->Qc_Max_SuckBwd- pp->st->Qc_Min_SuckBwd;
					pp->st->Qc_Unq_Rate_SuckBwd= pp->st->Qc_Unq_Cnt_SuckBwd*100/pp->st->Qc_Opn_Cnt;
				}					
				if(pp->md->Qc_Mode_SuckTm== 1)
				{
					pp->st->Qc_Avg_SuckTm= pp->qc_avg_sucktm/pp->st->Qc_Opn_Cnt;  
					pp->st->Qc_Deviation_Max_SuckTm=  pp->st->Qc_Max_SuckTm- pp->st->Qc_Min_SuckTm;
					pp->st->Qc_Unq_Rate_SuckTm= pp->st->Qc_Unq_Cnt_SuckTm*100/pp->st->Qc_Opn_Cnt;
				}				
			}
		}
	

		if(pp->st->Qc_Opn_Cnt==pp->md->Qc_OpnNum)
		{
			pp->qcparasend = Get100MsTick();
		    SET_PART_ERR(pp, ER0_QCPARACHK);								
			PartDataTx((PPART)(pp), ST_ID,CAL_LEN(*(pp->st)),0,TRUE);

			return TRUE;
		}
	}
	
	return FALSE;
}


void QcMonitorSetup(PQCMONITOR pp)
{
	int i;

	if(pp != NULL)
	{
		for(i = 0; i < CLAMP_MAX_EJECT; i++)
			pp->pejt[i] = (PEJECT)GetMachineSubPart(EJECT_ID, i + 1);
		
		pp->pclmp= (PCLAMP)GetMachineSubPart(CLAMP_ID, GET_PART_SN(pp->part.id));
		pp->pinj = (PINJECT)GetMachineSubPart(INJECT_ID, GET_PART_SN(pp->part.id));
	}
}


BOOL InitQcMonitorPart()
{

	PQCMONITOR pp = &m_qcmonitor;

	static const SUB_INIT_INFO sub_init[]  = QCMONITOR_INIT_INFO;
										     
	if(!INIT_PART(pp, QC_ID, 1, sub_init))
		return FALSE;


	QcMonitorSetup(pp);
	return TRUE;
}


