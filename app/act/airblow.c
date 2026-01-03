/******************************************************************************
  文 件 名   : airblow.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 吹气部件及动作函数
  函数列表   :
              AirBlowOff
              AirBlowOn
              AirBlowRun
              InitAirBlowPart
  修改历史   :
  1.日    期   : 2013年5月17日
    作    者   : ren chaohong
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "actcom.h"
#include "airblow.h"
#include "machine.h"
#include "recycle.h"
#include "Part.h"
#include "memtbl.h"

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
extern	 MACHINE m_machine;

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
PMODULE_SEQVLV_DATA pg_seq_vlv_data[50] = {NULL};
int	g_seq_vlv_num = 0;

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/


/*******************************************************************************************
 *Brief			:检查吹气动作是否完成：
 				等待吹气：吹气未完成，警报"等待吹气完成"
 				不等待：直接结束吹气任务
 *Param			:TRUE:吹气完成	FALSE:吹气等待
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20201118
********************************************************************************************/
BOOL ChkAirBlowComplete()
{	
	PCLAMP pp = GetSys()->pclmp;
	UI16 chkairblow = TRUE;
	
	if (pp->md->FL_CLPPRE_NOTWAITAIRBLOW == 1&&pp->prbt->md->FL_ROBOTCTRL > 0)
	{		
		chkairblow = FALSE;	//不等待吹气
	}
	else
	{
		chkairblow = TRUE;	//等待吹气
	}
	
	for (int i = 0; i < CLAMP_MAX_AIRBLOW; i++)
	{
		if (pp->airblow[i] != NULL)
		{
			if (chkairblow)
			{
				if (ChkActRun(pp->airblow[i], ACT_SN_BIT(OS_AIRBL)))
				{
					SET_PART_ERR(pp->airblow[i], ER0_AIRM);
					return FALSE;
				}
				else
				{
					CLR_PART_ERR(pp->airblow[i], ER0_AIRM);
				}
			}
			else
			{
				EndActTsk(((PACT)(pp->airblow[i]->act + OS_AIRBL - 1))->ptsk, TRUE);	
			}
		}
	}

	return TRUE;
}

void AirBlowOff(PAIRBLOW pp, PACT pact)
{
	if(pp != NULL)
	{
		SetPOBit(pp->po->O_AIR_BLT,PC_OFF);
	}
	
}

void AirBlowOffDelay(PAIRBLOW pp, PACT pact)
{
	if(pp != NULL)
	{
		SetPOBit(pp->po->O_AIR_BLT,PC_OFF);
	}
}

void AirBlowOn(PAIRBLOW pp, PACT pact)
{
	if(pp != NULL)
		SetPOBit(pp->po->O_AIR_BLT,PC_ON);
}

/*para 表示电子尺通道号*/
ACT_RET AirBlowRun(PACT pact, UI32 para)
{
	PAIRBLOW pp = (PAIRBLOW)(pact->part);
	PMACHINE pm = &m_machine;
	PCLAMP	pc = (PCLAMP)GetPartByID(MK_PART(CLAMP_ID, 1));
	PINJECT	pinj = (PINJECT)GetPartByID(MK_PART(INJECT_ID, 1));
	PNOZZLE	pn = (PNOZZLE)GetPartByID(MK_PART(NOZZLE_ID, 1));
	
	ACT_RET ret = ACT_OK;

	if(pp == NULL)
		return ACT_ERR_ABORT;
	
	if(ChkInjectCushErr()&&ChkSysCfg(CO1_CPSR)&&ChkOperMode(OM_AUTO))   //通用射出监控失败停在顶针前
	{
		CyclePDInfoSend(pm);
		return ACT_ERR_ABORT;
	}

	if(ChkSysCfg(XF3_LHACC)&&(pc->clampclserr_opn==1)&&(ChkOperMode(OM_AUTO)))
		return ACT_END; 	//异常开模时不做吹气动作  QRC 2017.04.26
		
	switch (pact->step)
	{
		case INIT1_STEP:                          // if airblow time = 0 release it
			//吹气设置
			if (pp->md->FL_BLAST == 256)
				return ACT_END;
			
			pp->st->Step_AirBlow = 0;
			if ((pp->md->TM_MOVINGPLATENACT >0  && pp->md->TM_MOVINGPLATENACT < 10000)
				|| ((pp->md->FL_BLAST == 5) && ChkSysCfg(XF2_DPUMP) && ChkOperMode(OM_AUTO)))
			{
				/*手动模式，直接吹气*/
				if(ChkOperMode(OM_MANUAL)&&!ChkSysCfg(CO3_AIRM))
				{
					pp->mode = AIRBLOW_TM_MD;
					ACT_STEP_NEW(STAGE_1_STEP);
				}
				else 
				{
					if (ChkOperMode(OM_AUTO) == 0)
					{
						if (pp->md->FL_BLAST == 1 || pp->md->FL_BLAST == 5)
						{
							pp->pos_ch = GetSys()->pclmp->mh->CLMP_METERCH;
						}
						else if (pp->md->FL_BLAST == 3 || pp->md->FL_BLAST == 9)
						{
							pp->pos_ch = GetSys()->pinj->mh->INJ_METERCH;
						}
						else if (pp->md->FL_BLAST == 6 || pp->md->FL_BLAST == 8)
						{
							pp->pos_ch = GetSys()->pejt->mh->EJT_METERCH;
						}
						else
						{
							pp->pos_ch = 0;
						}
					}
					else
					{
						pp->pos_ch = para;
					}
					
					if(pp->md->FL_BLAST > 9 || pp->md->FL_BLAST == 7 || ((pp->md->FL_BLAST == 0 || pp->md->FL_BLAST == 2 || pp->md->FL_BLAST == 4) || GetMeterLenByCh(pp->pos_ch) <= 0))
					{
						pp->mode = AIRBLOW_TM_MD;
					}
					else
					{						
						if(pp->md->FL_BLAST == 1 || pp->md->FL_BLAST == 6 || pp->md->FL_BLAST == 9)
							pp->mode = AIRBLOW_G_POS_MD;
						else
							pp->mode = AIRBLOW_L_POS_MD;
					}

					ACT_STEP_NEW(INIT1_STEP + 1);
				}
			}
			else
				ACT_STEP_NEW(END_STEP);
			break;
    	case INIT1_STEP + 1:
    		if((pp->mode == AIRBLOW_TM_MD && STEP_10MS_CHK(pp->md->TM_MOVINGPLATENDLY)) ||
    		  (pp->mode == AIRBLOW_G_POS_MD &&  GetMeterPosByCh(pp->pos_ch) >= pp->md->ADPOSI_MOVINGBLASTSTART) ||
			  (pp->mode == AIRBLOW_L_POS_MD &&  GetMeterPosByCh(pp->pos_ch) <= pp->md->ADPOSI_MOVINGBLASTSTART))
    		{
    			if((pp->md->FL_BLAST == 5) && ChkSysCfg(XF2_DPUMP))
    			{
    				ACT_STEP_NEW(STAGE_1_STEP);
    			}
				else
    				ACT_STEP_NEW(INIT1_STEP + 2);
    		}
			break;

		case INIT1_STEP + 2:
    		if((pp->mode == AIRBLOW_G_POS_MD &&  STEP_10MS_CHK(pp->md->TM_MOVINGPLATENDLY)) ||
			  (pp->mode == AIRBLOW_L_POS_MD &&  STEP_10MS_CHK(pp->md->TM_MOVINGPLATENDLY)))
			  {
    			    ACT_STEP_NEW(STAGE_1_STEP);
			  }
			  else if(pp->mode == AIRBLOW_TM_MD)
			  {
    			    ACT_STEP_NEW(STAGE_1_STEP);
			  }
			break;
						
		case STAGE_1_STEP:
		case STAGE_1_STEP + 1:
			if((pp->md->FL_BLAST == 5) && ChkSysCfg(XF2_DPUMP))
			{
				if(CHK_ACT_IS_IDLE(pc, OS_CLSM)&&CHK_ACT_IS_IDLE(pc, OS_PRSM)
					&&CHK_ACT_IS_IDLE(pn, OS_NOZF)&&CHK_ACT_IS_IDLE(pinj, OS_INJE))
//				if(!ChkActRun(pc, ACT_SN_BIT(OS_CLSM))&&!ChkActRun(pc, ACT_SN_BIT(OS_PRSM))
//					&&!ChkActRun(pn, ACT_SN_BIT(OS_NOZF))&&!ChkActRun(pinj, ACT_SN_BIT(OS_INJE)))
					ACT_STEP_NEW(END_STEP);	
			}
			else
			{
				if(STEP_10MS_CHK(pp->md->TM_MOVINGPLATENACT))
					ACT_STEP_NEW(END_STEP);
			}

			CHK_SET_MOV(AirBlowOn);
			break;  
		case END_STEP:
			SET_OFF(AirBlowOffDelay);
			ret =  ACT_END;
			break;
		default:
			ret =  ACT_END;
			break;
  	}

	if(ret == ACT_END || ret == ACT_ERR_ABORT)
	{
		return ret;
	}

  	return ret;
}	  

void InitSeqVlvMain()
{	
	int i = 0;
	PMODULE_SEQVLV_DATA prow;
	UI8 airblow_num;
	GetMachineSubPartList(AIRBLOW_ID, &airblow_num);

	g_seq_vlv_num = (airblow_num*3) > MAX_SEQ_NUM ? MAX_SEQ_NUM :(airblow_num*3);
    prow = AllocMemTbl(MEMTBL_ACT_TSK, sizeof(MODULE_SEQVLV_DATA), g_seq_vlv_num);

	if (prow != NULL)
	{
		for (i = 0;i < g_seq_vlv_num;i++)
		{
			pg_seq_vlv_data[i] = prow++;
		}
	}	
}
void InitSeqVlvPro(PAIRBLOW pp)
{
	UI16 i = 0,j = 0;
	UI16* pdata = (UI16*)&(pp->md->FL_SEQ_USE1);
	UI16* pdata2 = (UI16*)&(pp->po->O_AIR_SEQ1);

	UI16 partind = (pp->part.id>>4)&0xF;
	PMODULE_SEQVLV_DATA pseqdata;

	for (i = (partind - 1)*3,j = 0; j < 3;i++,j++)
	{
		if(i >= g_seq_vlv_num)
		{
			break;
		}
		
		pseqdata = pg_seq_vlv_data[i];
		
		if(pseqdata == NULL)
		{
			break;
		}
		
		pseqdata->seq_dic.FL_SEQ_USE = (*pdata++);
		pseqdata->seq_dic.TM_SEQ_DELAY = (*pdata++);
		pseqdata->seq_dic.TM_SEQ_MOVING = (*pdata++);
		pseqdata->seq_dic.ADPOSI_SEQSTART = (*pdata++);
		pseqdata->seq_dic.ADPOSI_SEQEND = (*pdata++);	
		pseqdata->seq_dic.O_AIR_SEQ = (*pdata2++);	
		pseqdata->admeter_ch = 1;

		if (pseqdata->seq_dic.FL_SEQ_USE > 0)
		{
			pseqdata->step = 1;
		}
		else
		{
			pseqdata->step = 0;
		}

		switch(pseqdata->seq_dic.FL_SEQ_USE)
		{
			case 1:
				pseqdata->startmode = 1;
				pseqdata->endmode = 1;
				break;
			case 2:
				pseqdata->startmode = 2;
				pseqdata->endmode = 2;
				break;
			case 3:
				pseqdata->startmode = 1;
				pseqdata->endmode = 2;
				break;
			case 4:
				pseqdata->startmode = 2;
				pseqdata->endmode = 1;
				break;
		}
	}	
}

UI16 SeqVlvPro()
{
	UI16 SeqSn = 0,Sequse = 0;
	PMODULE_SEQVLV_DATA pseqdata;
	
	for (SeqSn = 0;SeqSn < g_seq_vlv_num;SeqSn++)
	{
		pseqdata = pg_seq_vlv_data[SeqSn];

		if (pseqdata == NULL)
		{
			break;
		}
		
		if (pseqdata->step == 0)
		{
			Sequse++;
			continue;
		}

		switch(pseqdata->step)
		{
			case 1://静默
				pseqdata->starttime = Get10MsTick();
				
				if (pseqdata->startmode == 1)
				{
					pseqdata->step = 2;
				}
				else if (pseqdata->startmode == 2)
				{
					pseqdata->step = 3;
				}
				else
				{
					pseqdata->step = 0;
				}
				break;
			case 2://时间开
				if (DIFF_10MS(pseqdata->starttime) > pseqdata->seq_dic.TM_SEQ_DELAY)
				{
					if(pseqdata->endmode == 1)
					{
						pseqdata->step = 4;
					}
					else
					{
						pseqdata->step = 5;
					}
					pseqdata->starttime = Get10MsTick();
					SetPOBit(pseqdata->seq_dic.O_AIR_SEQ,PC_ON);
				}
				break;
			case 3://位置开
				if (GetMeterPosByCh(pseqdata->admeter_ch) <= pseqdata->seq_dic.ADPOSI_SEQSTART)
				{
					if(pseqdata->endmode == 1)
					{
						pseqdata->step = 4;
					}
					else
					{
						pseqdata->step = 5;
					}
					pseqdata->starttime = Get10MsTick();
					SetPOBit(pseqdata->seq_dic.O_AIR_SEQ,PC_ON);
				}
				break;
			case 4://时间关
				if (DIFF_10MS(pseqdata->starttime) > pseqdata->seq_dic.TM_SEQ_MOVING)
				{
					SetPOBit(pseqdata->seq_dic.O_AIR_SEQ,PC_OFF);
					pseqdata->step = 0;
				}
				break;
			case 5://位置关
				if (GetMeterPosByCh(pseqdata->admeter_ch) <= pseqdata->seq_dic.ADPOSI_SEQEND)
				{
					SetPOBit(pseqdata->seq_dic.O_AIR_SEQ,PC_OFF);
					pseqdata->step = 0;
				}
				break;
			default:
				Sequse++;
				break;
		}
	}

	return g_seq_vlv_num - Sequse;
}

void SeqVlvOffPro()
{
	UI16 SeqSn = 0;
	PMODULE_SEQVLV_DATA pseqdata;
	
	for (SeqSn = 0;SeqSn < g_seq_vlv_num;SeqSn++)
	{
		pseqdata = pg_seq_vlv_data[SeqSn];

		if (pseqdata == NULL)
			break;
		
		if (pseqdata->step > 2)
		{
			SetPOBit(pseqdata->seq_dic.O_AIR_SEQ,PC_OFF);
		}		
	}
}

ACT_RET SeqBlowRun(PACT pact, UI32 para)
{
    ACT_RET ret = ACT_OK;
	UI16 SeqSn = 0;
	PMODULE_SEQVLV_DATA pseqdata;
	
    switch (pact->step)
    {
        case INIT1_STEP:                         
			if(ChkActRun(GetSys()->pinj, ACT_SN_BIT(OS_INJE)))
            {
            	//初始化顺序阀
				for (SeqSn = 0;SeqSn < g_seq_vlv_num;SeqSn++)
				{
					pseqdata = pg_seq_vlv_data[SeqSn];

					if (pseqdata == NULL)
						break;
					
					if (pseqdata->seq_dic.FL_SEQ_USE > 0)
					{
						pseqdata->step = 1;
					}	
					else
					{
						pseqdata->step = 0;
					}
				}
            	ACT_STEP_NEW(STAGE_1_STEP);
			}
			else
			{
				ACT_STEP_NEW(END_STEP);
			}
            break;       
        case STAGE_1_STEP:
            if(ChkActRun(GetSys()->pinj, ACT_SN_BIT(OS_INJE)))
            {
				if(!SeqVlvPro())//所有顺序阀都关闭时，任务结束
				{
					ACT_STEP_NEW(END_STEP);
				}
			}
			else
            {
				ACT_STEP_NEW(END_STEP);
			}
            break;  
        case END_STEP:
            SeqVlvOffPro();
            ret =  ACT_END;
            break;
        default:
            ret =  ACT_END;
            break;
    }
		
		return ret;
}


BOOL InitAirBlowPart(PAIRBLOW pp, UI8 sn)
{
	static const SUB_INIT_INFO sub_init[]  = AIRBLOW_INIT_INFO;
	
	static const ACT_INIT_INFO act_init[] = 
		{
			{OS_AIRBL, OS_AIRBL, ACT_NTYPE, AirBlowRun, AirBlowOff},
			{OS_SEQ1, OS_SEQ1, ACT_CTRB, SeqBlowRun, SeqVlvOffPro}
		};
									     
	if(!INIT_PART(pp, AIRBLOW_ID, sn, sub_init))
		return FALSE;

	if(!INIT_ACT(pp, act_init))
		return FALSE;

	//g_seq_vlv_num = g_seq_vlv_num + 3;
	return TRUE;
}

UI32 AirBlowCmd(PAIRBLOW pp, UI32 cmd, UI32 para)
{
	if(pp == NULL)
		return FALSE;

	switch(GET_CMD(cmd))
	{
		case CMD_DATA_RX:
			InitSeqVlvPro(pp);
		break;
	}

	return TRUE;
}

