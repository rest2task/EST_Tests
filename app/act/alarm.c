/******************************************************************************
  文 件 名   : alarm.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 告警部件
  函数列表   :
              AlarmIsOn
              AlarmLoop
              AlarmManualInit
              AlarmOff
              AlarmOffInner
              AlarmOn
              AlarmOnInner
              AlarmSetEmerge
              InitAlarmPart
              StandAlarm
              TempAlarm
              TempAlarmLoop
  修改历史   :
  1.日    期   : 2013年5月17日
    作    者   : ren chaohong
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "com.h"
#include "ioproc.h"
#include "motor.h"
#include "machapi.h"
#include "alarm.h"
#include "machine.h"
#include "EtherCATProc.h"
#include "math.h"
#include "EleCtrlApp.h"
#include "da.h"
#include "keycmd.h"



/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
 extern   MACHINE m_machine;
 extern PART_IND part_ind_tbl[PART_TYPE_END + 1];
 extern PART_TBL part_tbl;
 extern ADMETER m_ad;
 extern DA m_da;


/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
BOOL ChkAlarmIsEmpty(void);
void InsAlarmInAlarmCache(void);
/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
ALARM m_alarm;

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
 *Brief			:获得外部运行键IO信号
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20201127
********************************************************************************************/
BOOL KeyRunExtPi()
{
	PALARM pp = &m_alarm;

	if (TestPI(pp->pi->I_KEY_RUN))
	{		
		if (DIFF_MS(pp->key_run_start_ms) > 20)
		{
			g_statkey.runkey = TRUE;	
			return TRUE;
		}
	}
	else
	{
		pp->key_run_start_ms = GetMsTick();		
	}
	
	return FALSE;
}

/*******************************************************************************************
 *Brief			:获得外部急停IO按钮信号
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20201127
********************************************************************************************/
void ChkEmgExtPi()
{
	PALARM pp = &m_alarm;

	if(TestPI(pp->pi->I_EMG_EXT))
    {
        if(!CHK_PART_ERR(pp, ER0_PNLS_EXT))
            SET_PART_ERR(pp, ER0_PNLS_EXT);

        if (GetHeatingStatus() && !ChkSysCfg(CO1_EMERG))
	        WarmOff();
    }
    else
    {
        if(CHK_PART_ERR(pp, ER0_PNLS_EXT))
            CLR_PART_ERR(pp, ER0_PNLS_EXT);
    }
}

void AlarmOnInner(ALARM_STRU* pa)
{
	PALARM pp = &m_alarm;

	SetPOBit(pp->po->O_ALARM, PC_ON);
	SetPOBit(pp->po->O_FLASH,PC_ON);
	pa->alarmstart = Get10MsTick();
	pa->alarmlast = pp->g_alarm.alarmstart;
	pa->alarmflag = 1;
}

BOOL AlarmIsOn()
{
	return (m_alarm.g_alarm.alarmflag != 0);
}

BOOL TempAlarmIsOn()
{
	return (m_alarm.temp_alarm.alarmflag != 0);
}

void AlarmOn()
{
	AlarmOnInner(&m_alarm.g_alarm);
}

void AlarmOffInner(ALARM_STRU* pa)
{
	PALARM pp = &m_alarm;;

	SetPOBit(pp->po->O_ALARM, PC_OFF);
	SetPOBit(pp->po->O_FLASH,PC_OFF);
	pa->alarmstart = 0;
	pa->alarmlast = 0;
	pa->alarmcnt = 0;
	pa->alarmflag = 0;
}

void TempAlarmLoop()
{
	PALARM pp = &m_alarm;;
	
	if (pp->temp_alarm.alarmflag)
	{
		if(DIFF_10MS(pp->temp_alarm.alarmlast) > ALARM_INTVAL/10)
		{
			pp->temp_alarm.alarmlast = Get10MsTick();
			SetPOBit(pp->po->O_ALARM, GetPOBit(pp->po->O_ALARM)^1 + 4);
			pp->temp_alarm.alarmcnt++;
		}
	}

	//BMC机警报缺料时触发临时警报，警报灯一直响，到这一模结束时再清除(lzh 2022.1.4)
	if(ChkSysCfg(CO0_BMC) && ChkOperMode(OM_AUTO))
	{
		
	}
	else if ((pp->temp_alarm.alarmcnt > pp->temp_alarm.alarmsetcnt_temp))	//手动情况下压料缸退到位后输出警报，警报灯输出设定次数后停止(lzh 2022.1.4) 
		AlarmOffInner(&pp->temp_alarm);
}

void AlarmLoop()
{
	PALARM pp = &m_alarm;

	if (AlarmIsOn() && ChkAlarmIsEmpty())
	{
		AlarmOff();		
	}

	if(pp->g_alarm.alarmflag)
	{
    	if(DIFF_10MS(pp->g_alarm.alarmlast)> ALARM_INTVAL/10)
    	{
      		pp->g_alarm.alarmlast = Get10MsTick();
			SetPOBit(pp->po->O_ALARM, GetPOBit(pp->po->O_ALARM)^1 + 4);

			SetPOBit(pp->po->O_OK_LIT, PC_OFF);
			SetPOBit(pp->po->O_AUT_LIT, PC_OFF);
			SetPOBit(pp->po->O_MAN_LIT, PC_OFF);
			pp->g_alarm.alarmcnt++;
    	}
    		
    	if(pp->g_alarm.alarmcnt>= 2*pp->mh->CN_BUZZERACT) 
    	{
			if (MotorIsOn())
        		MotorDown();
			if ((pp->mh->TM_HEATEROFFIFALARM > 0)&&(DIFF_10MS(pp->g_alarm.alarmstart)> pp->mh->TM_HEATEROFFIFALARM) && GetHeatingStatus() && !(ChkRunFlag(INIF_RESET)&&ChkSysCfg(CO1_EMERG))) 
			{
				GetSys()->md->CUSTOM_PARA[0] = 1;
				WarmOff();
			}
    	}
	}
	else
	{
		if(DIFF_10MS(pp->g_alarm.alarmlast) > ALARM_OFF_DELAY/10)
    		{
      			pp->g_alarm.alarmlast = Get10MsTick();
      			SetPOOnOff(pp->po->O_OK_LIT, 1);
      			
			if(ChkOperMode(OM_AUTO))
      		{	
				if(CycleTskIsRun() && DIFF_MS(GetCycleTskStartMs())>= ALARM_OFF_DELAY)
				{ 
					SetPOBit(pp->po->O_AUT_LIT,PC_OFF);
					SetPOBit(pp->po->O_MAN_LIT, GetPOBit(pp->po->O_MAN_LIT) ^1 + 4);
				}	
				else
        		{
        			SetPOOnOff(pp->po->O_AUT_LIT, 1);
					SetPOBit(pp->po->O_MAN_LIT, PC_OFF);
				}  
			}
			else  
			{	
				SetPOBit(pp->po->O_MAN_LIT, PC_ON);
				SetPOBit(pp->po->O_AUT_LIT, PC_OFF);
      		}
		}
	}

    if(!pp->g_alarm.alarmflag && !pp->temp_alarm.alarmflag)
    {
        if (TestPOIsOn(pp->po->O_ALARM))
			SetPOBit(pp->po->O_ALARM, PC_OFF);
		
		if (TestPOIsOn(pp->po->O_FLASH))
			SetPOBit(pp->po->O_FLASH, PC_OFF);
    }
  
	TempAlarmLoop();
}

/*function for other module*/
/*stand Alarm off*/
void AlarmOff()
{
	AlarmOffInner(&m_alarm.g_alarm);
}

void TempAlarmOff()
{
	AlarmOffInner(&m_alarm.temp_alarm);
}

void StandAlarm(BOOL bforce)    //标准警报
{
	PALARM pp = &m_alarm;
	if((!ChkOperMode(OM_MANUAL)&&!ChkOperMode(OM_ADJU)) || bforce || (pp->mh->FL_AlARMONMANL == 1))
	{
		InsAlarmInAlarmCache();	//将触发警报的警报信息录入。
		
		if(!AlarmIsOn())
		{
			AlarmOnInner(&m_alarm.g_alarm);
		}
	}
}

void TempAlarm(BOOL bforce)        //临时警报 07-7-27
{
	//(*err_bits) |= err;
	UI16 alarmsetcnt_temp = 0;

	if(bforce || ChkOperMode(OM_AUTO))
	{
		AlarmOnInner(&m_alarm.temp_alarm);

		alarmsetcnt_temp = m_alarm.mh->FL_ALARMCNT;
		if(alarmsetcnt_temp < 4 || alarmsetcnt_temp > 99)
		{
			m_alarm.temp_alarm.alarmsetcnt_temp = 5*2;
		}
		else
		{
			m_alarm.temp_alarm.alarmsetcnt_temp = alarmsetcnt_temp*2;
		}
	}
}

void AlarmSetEmerge(BOOL bemerge)
{
	PALARM pp = &m_alarm;

	if(bemerge)
	{
		SetPOBit(pp->po->O_EMG_LIT, PC_ON);
		SetPOBit(pp->po->O_OK_LIT, PC_OFF);
	}
	else
	{
		SetPOBit(pp->po->O_EMG_LIT, PC_OFF);
		SetPOBit(pp->po->O_OK_LIT, PC_ON);
	}
}

/*******************************************************************************************
 *Brief			:整合需要上传的警报信息
 *Param			:
 *Return Value	:
 *Author			:DK
 *Date			:20190115
********************************************************************************************/
BOOL AlarmDataTx(PPART pp, UI32 err)
{	
    PPORT_ALARM_DATA_PKT	ppkt;
    PPROT_TSK_ALARM_ST_ITEM pit;
    PACT_TSK_ST pst;
    PACT_TSK ptsk;
    PADMETER pad = &m_ad;
    PTEMPCTRLCOMM pt = g_tempcomm[0];
	PDA pda = &m_da;

    UI8 i =0;
    UI8 num =0;

	for (i = 0; i < MAX_TSK_NUM; i++)
    {
        //查找当前有几个任务正在运行
        if (act_tsk[i].pst != NULL && act_tsk[i].pst->tsk_st != TSK_IDLE)
        {
            num++;
        }
        //目前面板上记录3个动作
        if(num >= 3)
            num =3 ;
    }

    if ((ppkt = (PPORT_ALARM_DATA_PKT)ProtNewPkt(ALARM_DATA_TX_CMD, 0, FALSE,  sizeof(PORT_ALARM_DATA_PKT)+ num * sizeof(PROT_TSK_ALARM_ST_ITEM))) != NULL)
    {
        ppkt->part_id = pp->id;
        ppkt->current_err0 = (err & 0xffff);
        ppkt->current_err1 = ((err >> 16) & 0xffff);
        
        ppkt->item_len = sizeof(PROT_TSK_ALARM_ST_ITEM) / sizeof(UI16);
       
	   ppkt->num = num;
	   pit = (PPROT_TSK_ALARM_ST_ITEM)(ppkt + 1);

	   for (i = 0; i < MAX_TSK_NUM && num > 0; i++)
        {
            ptsk = act_tsk + i;
            pst = ptsk->pst;
            if (pst != NULL && pst->tsk_st != TSK_IDLE)
            {
                pit->act_id = pst->act_id;		//记录当前动作ID
                pit->step = pst->next_step;		//记录当前动作步
                pit->start_ms = pst->act_run_ms;//记录当前动作时间

                pit++;
                num--;
            }
        }
		
        //上传输入、输出点的相对值
        for ( i = 0; i<PI_STATUS_NUM ;i++)
        {
            ppkt->pi_input[i] = GetPI16(i);
            ppkt->po_output[i] = GetPO16(i);				
        }

        //上传AD值的相对位置
        for ( i = 0; i < ADMETER_CH_NUM; i++)
        {
        	if (i >= MAXADPORT)
        	{
				ppkt->adposi_relative[i] = pad->st->ADPOSI_RELATIVE_EXT[i - MAXADPORT];//AD相对位置
				//ppkt->ad_relative[i] = pad->st->ADPOSI_METER[i - MAXADPORT];//AD采集数据
			}
			else
			{
				ppkt->adposi_relative[i] = pad->st->ADPOSI_RELATIVE[i];
				ppkt->ad_relative[i] = pad->st->ADPOSI_METER[i];	
			}            
        }

		for (i = 0;i<MAX_DA_NUM;i++)
		{
			ppkt->da_value[i] = pda->st->DA_VAL1[i];
		}

        //ppkt->ele_spdout_rpm[i]等数组个数定义8个，为了后期扩展预留
        //但是由于现在的从站个数只有4个，因此只对前4个进行赋值
        for(i = 0; i < PUMP_ECAT_NUM; i++) 
        {
            ppkt->servo_err[i] = PdoRecvData[i].AlarmPara;
            ppkt->ele_spdout_rpm[i] = g_pumpn[i]->st->MOTOR_SPEED_SET;
            ppkt->ele_real_spd[i] = g_pumpn[i]->st->MOTOR_SPEED_REAL;
            ppkt->ele_real_torque[i] = g_pumpn[i]->st->MOTOR_TORQUE_REAL;
        }

        //记录当前开模数
        ppkt->cn_moldnum_l = GetSys()->pclmp->st->CN_MOLDOPNNUM1;
        ppkt->cn_moldnum_h = GetSys()->pclmp->st->CN_MOLDOPNNUM0;
		
        ppkt->port_adtemp = pt->curchnl;
		
		if(ppkt->port_adtemp == 0xFF)
		{
			//记录温度值
	        for(i = 0; i < TEMP_E502_CH_NUM; i++) 
	        {
	            ppkt->temp_real[i] = 0;
	        }

			ppkt->port_adtemp = 0;
			ppkt->wtemp_lm73 = 0;
			ppkt->ad_tpvalue = 0;
		}
		else
		{
			//记录温度值
	        for(i = 0; i < pt->mh->DATEMP_CH_NUM; i++) 
	        {
	            ppkt->temp_real[i] = pt->st->DATEMP_REAL[i];
	        }

			ppkt->ad_tpvalue = pt->tempctrldatastu.tempdata[ppkt->port_adtemp].test;
			ppkt->wtemp_lm73 = pt->st->WTEMP_LM73;
		}       

        return MainProtTxPkt(ppkt);
    }
    else
    {
        return FALSE;
    }

}

/*******************************************************************************************
 *Brief			:自动、或者切成手动的警报(比如后安全门)等警报，上传信息
 *Param			:
 *Return Value	:
 *Author			:DK
 *Date			:20190115
********************************************************************************************/
void AlarmDataTxChk(PPART pp, UI32 err,BOOL bforce)
{
	if ((ChkOperMode(OM_AUTO)||(bforce ==TRUE))&&(!ChkPartErr(pp,err)))
	{   
        AlarmDataTx(pp,err);
	}
}


BOOL InitAlarmPart()
{
	PALARM pp = &m_alarm;
	static const SUB_INIT_INFO sub_init[] = ALARM_INIT_INFO;

	if(!INIT_PART(pp, ALARM_ID, 1, sub_init))
		return FALSE;
	return TRUE;
}

void AlarmManualInit()
{
	PALARM pp = &m_alarm;

	
	pp->g_alarm.alarmstart = 0;
	pp->g_alarm.alarmlast  = 0;
	pp->g_alarm.alarmflag  = 0;
	pp->g_alarm.alarmcnt   = 0;	

	pp->temp_alarm.alarmstart = 0;
	pp->temp_alarm.alarmlast  = 0;
	pp->temp_alarm.alarmflag  = 0;
	pp->temp_alarm.alarmcnt   = 0;	
	if(ChkSysCfg(XF2_ADLRV))
	{
		AutoAlarmReStart();
	}
}

