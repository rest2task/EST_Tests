/******************************************************************************
  文 件 名   : keycmd.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月22日
  最近修改   :
  功能描述   : 键盘命令处理
  函数列表   :
  修改历史   :
  1.日    期   : 2013年5月22日
    作    者   : ren chaohong
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "part.h"
#include "acttype.h"
#include "typedef.h"
#include "keycmd.h"
#include "protcmd.h"
#include "systempro.h"
#include "machine.h"
#include "motor.h"
#include "lub.h"
#include "da.h"
#include "alarm.h"
#include "actfork.h"
#include "servo.h"
#include "safedoor.h"
#include "act.h"
#include "TempCtrlComm.h"
#include "TempCtrlMw.h"
#include "hwmid.h"

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
void ServoRecoverActNoEn(void);
BOOL KeyRunExtPi(void);


/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/
static BOOL KeyInjectProc(UI16 key, BOOL bkeyon, UI16 repeat_cnt, UI32 start_press_ms);
static BOOL KeyChargeProc(UI16 key, BOOL bkeyon, UI16 repeat_cnt, UI32 start_press_ms);
static BOOL KeyAutoPurgeProc(UI16 key, BOOL bkeyon, UI16 repeat_cnt, UI32 start_press_ms);
static BOOL KeyMotorProc(UI16 key, BOOL bkeyon, UI16 repeat_cnt, UI32 start_press_ms);
static BOOL KeyLubProc(UI16 key, BOOL bkeyon, UI16 repeat_cnt, UI32 start_press_ms);
static BOOL KeySafedoorProc(UI16 key, BOOL bkeyon, UI16 repeat_cnt, UI32 start_press_ms);
static BOOL KeyAdjustProc(UI16 key, BOOL bkeyon, UI16 repeat_cnt, UI32 start_press_ms);
static BOOL KeyAirBlowProc(UI16 key, BOOL bkeyon, UI16 repeat_cnt, UI32 start_press_ms);
static BOOL KeyAdjPosLearn(UI16 key, BOOL bkeyon, UI16 repeat_cnt, UI32 start_press_ms);

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/	
KEY_ST	g_key;
KEY_EM	key_em;
STATKEYSTRU 	g_statkey;
WORD	CLSKEY=0; 

const static ACT_KEY_MAP act_key_map[] =
{
	{INJECT_ID, OS_INJE, OS_AINJE, KEYON_OFF,KeyInjectProc}, 
	{INJECT_ID, OS_CHRG, OS_ACHRG, KEYON_SWICTH, KeyChargeProc}, 
	{INJECT_ID, OS_SUCK, OS_ASUCK}, 
	{INJECT_ID, OS_APURG, 0, KEYON_SWICTH, KeyAutoPurgeProc},
	{NOZZLE_ID, OS_NOZF, OS_ANOZF}, 
	{NOZZLE_ID, OS_NOZB, OS_ANOZB}, 
	{CHGPRS_ID, OS_CHGPRSF, 0}, 
	{CHGPRS_ID, OS_CHGPRSB, 0},
	{CLAMP_ID, OS_CLSM, OS_ACLS}, 
	{CLAMP_ID, OS_OPNM, OS_AOPN}, 
	{EJECT_ID, OS_EJT, OS_AEJTF}, 
	{EJECT_ID, OS_EJTB, OS_AEJTB},
	{CORE_ID, OS_CPRF, OS_CPRAF}, 
	{CORE_ID, OS_CPRB, OS_CPRAB}, 
	{AIRBLOW_ID, OS_AIRBL, 0, KEYON_OFF, KeyAirBlowProc}, 
	{MOTOR_ID, OS_MOTON, 0, KEYON_ONE, KeyMotorProc}, 
	{MOTOR_ID, OS_MOTOFF, 0, KEYON_ONE, KeyMotorProc},
	{LUB_ID, OS_LUBR, 0, KEYON_ONE, KeyLubProc}, 
	{ADJUST_ID, OS_ADJF, 0, KEYON_OFF, KeyAdjustProc}, 
	{ADJUST_ID, OS_ADJB, 0, KEYON_OFF, KeyAdjustProc}, 
	{ADJUST_ID, OS_ADJA, 0, KEYON_ONE},
    {ADJUST_ID, OS_ADJPOSLEARN, 0, KEYON_SWICTH,KeyAdjPosLearn}, 
	{SAFEDOOR_ID, OS_SDRO, 0, KEYON_OFF, KeySafedoorProc},
	{SAFEDOOR_ID, OS_SDRC, 0, KEYON_OFF, KeySafedoorProc},
	{HYDR_ID, OS_PRESSELF, 0, KEYON_SWICTH},
	{NOZZLE_ID, OS_OILHRAT, 0, KEYON_SWICTH}
};


/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/ 




void ComKeyRecv(PPROT_KEYPAD_CMD_PKT ppkt)
{
	if(ppkt)
	{
		g_key.currentkey = ppkt->keycode;
		g_key.keyon  = ((ppkt->keystatus & KEYON) == KEYON);
		g_key.bnew = TRUE;
	}
}

void InitKey()
{
	g_key.lastkey   = 0;
	g_key.currentkey = 0;
	g_key.keyon     = FALSE;
	g_key.bnew 		= FALSE;
	g_key.repeat	= FALSE;
	g_key.last_rx_ms = GetMsTick(); 
}

void GetEmergKey()
{
	BOOL key;

	//key关联内部急停键
	key = ((RESETKEYIDR) != 0);

	//急停反逻辑
	if (ChkSysCfg(XF0_REMERG))
	{
		key = !key;
	}
		
	if(key == key_em.tmp_Resetkey)
	{
		if((UI16)((UI16)GetMsTick() - key_em.reset_key_chg_tm) > 20)
		{
			if(key != key_em.last_resetkey)
			{
				if(key)
					SystemReset();
				else
					SystemRestart();
					
				key_em.last_resetkey = key;
			}
		}
	}
	else
	{
		key_em.tmp_Resetkey = key;
		key_em.reset_key_chg_tm = (UI16)GetMsTick();
	}
	
	key = ((MANUALKEYIDR) != 0);
	if(key == key_em.tmp_manualkey)
	{
		if(((UI16)((UI16)GetMsTick() - key_em.manual_key_chg_tm)) > 20)
		{
			if(key != key_em.last_manualkey)
			{
				if(key)
					ManualReset();

				key_em.last_manualkey = key;
			}
		}
	}
	else
	{
		key_em.tmp_manualkey= key;
		key_em.manual_key_chg_tm = (UI16)GetMsTick();
	}
}

void ClearRunKey()
{
	g_statkey.runkey = 0;
}

BOOL GetRunKey()
{
	return g_statkey.runkey;
}


BOOL KeyActRun(UI16 act_id)
{
	if(ChkOperMode(OM_AUTO) && !ChkSysCfg(CO5_INGEJT))
	{	//自动模式下做压料缸动作不切警报标记(lzh 2022.1.4)

	}
	else
	{
		AlarmManualInit();
		ClearAllPartError();
	}
	return ForkAct(GetActByID(act_id), FIRST_FORK, NULL, 0, 0);
}

void SetAutoStartProc()
{
	ClearAllPartError();
	SetRunFlag(INIF_AUTO | INIF_AUTO1);
	CreateActForkTbl(GetOperMode());
	MachineCmd(MH_CMD_AUTO_CYCLE, 0);
}

BOOL KeyInjectProc(UI16 key, BOOL bkeyon, UI16 repeat_cnt, UI32 start_press_ms)
{
	UI16 opermode = GetOperMode();
	PINJECT pinj = (PINJECT)GetPartByID(MK_PART(INJECT_ID, 1));
	UI8 part_sn;

	/*忽略重复按键*/
	if(repeat_cnt > 1)
		return TRUE;
	
	if(opermode != OM_MANUAL && opermode != OM_ADJU)
		return TRUE;
	
	part_sn = GET_PART_SN(key);
        
	if (bkeyon && !ChkActRun(pinj, ACT_SN_BIT(OS_AINJE)) && !ChkActRun(pinj, ACT_SN_BIT(OS_INJE)))
    {	
    	//手动注射走调模注射。
        if (ChkOperMode(OM_ADJU) || (pinj->mh->FL_MANUALINJECTUSEPRES_FLOW == 1))
        {
            KeyActRun(MK_ACT(INJECT_ID, part_sn, OS_AINJE));
        }
        else
        {
            KeyActRun(MK_ACT(INJECT_ID, part_sn, OS_INJE));
        }
    }
    else
    {
        SetRunFlag(INTF_OFF);
    }
	
	return TRUE;
}

BOOL KeyChargeProc(UI16 key, BOOL bkeyon, UI16 repeat_cnt, UI32 start_press_ms)
{
	UI16 opermode = GetOperMode();
	UI8 part_sn;

	/*忽略重复按键*/
	if(repeat_cnt > 1)
		return TRUE;
	
	if(opermode != OM_MANUAL && opermode != OM_ADJU)
		return TRUE;
	
	part_sn = GET_PART_SN(key);
	if(bkeyon)
	{          
		if (ChkActRun(GetPartByID(key), ACT_SN_BIT(OS_CHRG) | ACT_SN_BIT(OS_SUCK) | ACT_SN_BIT(OS_ACHRG) | ACT_SN_BIT(OS_ASUCK) | ACT_SN_BIT(OS_APURG)))
			SetRunFlag(INTF_OFF);
		else
		{
			if(opermode == OM_ADJU)
				KeyActRun(MK_ACT(INJECT_ID, part_sn, OS_ACHRG));
			else if(opermode == OM_MANUAL)
				KeyActRun(MK_ACT(INJECT_ID, part_sn, OS_CHRG));
		}
	}  	

	return TRUE;
}

BOOL KeyAutoPurgeProc(UI16 key, BOOL bkeyon, UI16 repeat_cnt, UI32 start_press_ms)
{
	/*忽略重复按键*/
	if(repeat_cnt > 1)
		return TRUE;

	if(GetOperMode() ==OM_MANUAL && bkeyon)
	  {
		if(AutoChargeEnableChk((PINJECT)GetPartByID(key)))
		{
			SetOperMode(OM_PURG);
			KeyActRun(key);
		}           	
	}
	else if (GetOperMode() ==OM_PURG && bkeyon)
	{
		SetOperMode(OM_MANUAL);
		SetRunFlag(INTF_OFF);
	}

	return TRUE;
}

BOOL KeyMotorProc(UI16 key, BOOL bkeyon, UI16 repeat_cnt, UI32 start_press_ms)
{
	/*忽略重复按键*/
	if(repeat_cnt > 1)
		return TRUE;

	if((GetOperMode() != OM_MANUAL && GetOperMode() != OM_ADJU) || !bkeyon)
		return TRUE;
    if(CUS_ID == CS_TJ)
    {
		if((key ==MK_ACT(MOTOR_ID, 1, OS_MOTON))&&(!MotorIsOn()))		
			KeyActRun(key);
		else if((key ==MK_ACT(MOTOR_ID, 1, OS_MOTOFF)))
		{
			PACT pact;	
			pact = GetActByID(MK_ACT(MOTOR_ID, 1, OS_MOTON));
			EndActTsk(pact->ptsk, TRUE);			
			MotorDown();	
		}
    }
	else
	{
		if(MotorIsOn())
		{
			PACT pact;
	
			pact = GetActByID(key);
			EndActTsk(pact->ptsk, TRUE);
			MotorDown();
		}
		else
			KeyActRun(key);
	}

	return TRUE;
}


BOOL KeyAirBlowProc(UI16 key, BOOL bkeyon, UI16 repeat_cnt, UI32 start_press_ms)
{
	UI16 opermode = GetOperMode();
	UI8 part_sn;
	UI8	part_type;	
	UI8	act_sn;	
	PAIRBLOW pp;

	/*忽略重复按键*/
	if(repeat_cnt > 1)
		return TRUE;
	
	if(opermode != OM_MANUAL && opermode != OM_ADJU)
		return TRUE;
	
	part_sn = GET_PART_SN(key);
	part_type = GET_PART_TYPE(key);
	act_sn = GET_ACT_SN(key);

	pp = (PAIRBLOW)GetPartByID(MK_PART(part_type, part_sn));

	if (pp == NULL)
		return TRUE;
	
	if(bkeyon && !ChkActRun(GetPartByID(MK_PART(part_type, part_sn)), ACT_SN_BIT(act_sn)))
	{	
		pp->act[OS_AIRBL - 1].cannotbeclear = TRUE;
		KeyActRun(key);
	}
	else
	{
		pp->act[OS_AIRBL - 1].cannotbeclear = FALSE;
		ClearAllActTaskEnf();
	}
	
	return TRUE;
}


BOOL KeyAdjPosLearn(UI16 key, BOOL bkeyon, UI16 repeat_cnt, UI32 start_press_ms)
{
	UI16 opermode = GetOperMode();
	UI8 part_sn;
	UI8	part_type;	
	UI8	act_sn;	
	PAIRBLOW pp;

	/*忽略重复按键*/
	if(repeat_cnt > 1)
		return TRUE;
	
	if(opermode != OM_AA_LEARN && opermode != OM_ADJU)
		return TRUE;
	
	part_sn = GET_PART_SN(key);
	part_type = GET_PART_TYPE(key);
	act_sn = GET_ACT_SN(key);

	pp = (PAIRBLOW)GetPartByID(MK_PART(part_type, part_sn));

	if (pp == NULL)
		return TRUE;

    if (bkeyon)
    {
    	if(!ChkActRun(GetPartByID(MK_PART(part_type, part_sn)), ACT_SN_BIT(act_sn)))
    	{	
    		KeyActRun(key);
    	}
    	else
    	{
    		SetRunFlag(INTF_OFF);
    	}
    }
	
	return TRUE;
}


BOOL KeyLubProc(UI16 key, BOOL bkeyon, UI16 repeat_cnt, UI32 start_press_ms)
{
	UI8 part_sn;
	PLUB plub;
	UI8 lub_num;
	int i;
	
	/*忽略重复按键*/
	if(repeat_cnt > 1)
		return TRUE;

	if(!bkeyon)
		return TRUE;

	part_sn = GET_PART_SN(key);
	plub = (PLUB)GetMachineSubPartList(LUB_ID, &lub_num);

	if(plub == NULL || lub_num == 0)
		return TRUE;
	
	for(i = 0; i < lub_num; i++)
	{
		if(part_sn == 0 || part_sn == GET_PART_SN(plub->part.id))
		{
			if(!ChkActRun((PPART)plub, ACT_SN_BIT(OS_LUBR)) //润滑0模式不判断润滑检知点是否配出
			&& (plub->mh->FL_LUBMODE==0||(plub->mh->FL_LUBMODE!=0&&LubChkPIChk(plub) != PI_NOUSED)))
			{					
				KeyActRun(MK_ACT(LUB_ID, GET_PART_SN(plub->part.id), OS_LUBR));
			}		
		}
		plub++;
	}

	return TRUE;
}


BOOL KeyAdjustProc(UI16 key, BOOL bkeyon, UI16 repeat_cnt, UI32 start_press_ms)
{
	PADJUST padj = (PADJUST)GetMachineSubPart(ADJUST_ID, 1);
	UI8 act_sn;
	
	if(GetOperMode() != OM_ADJU)
		return TRUE;

	/*忽略重复按键*/
	if(repeat_cnt > 1)
		return TRUE;

	act_sn = GET_ACT_SN(key);

	if(padj->mh->FL_JOG_FUN == 1)
	{
		if(bkeyon)
		{
			if(act_sn == OS_ADJF)
			{
				if(ChkActRun(GetPartByID(key), ACT_SN_BIT(OS_ADJF)))
					SetRunFlag(INTF_OFF);
				else if(!ChkActRun(GetPartByID(key), ACT_SN_BIT(OS_ADJF)) && !ChkActRun(GetPartByID(key), ACT_SN_BIT(OS_ADJB)))
					KeyActRun(key);
					
			}
			else if(act_sn == OS_ADJB)
			{
				if(ChkActRun(GetPartByID(key), ACT_SN_BIT(OS_ADJB)))
					SetRunFlag(INTF_OFF);
				else if(!ChkActRun(GetPartByID(key), ACT_SN_BIT(OS_ADJF)) && !ChkActRun(GetPartByID(key), ACT_SN_BIT(OS_ADJB)))
					KeyActRun(key);
			}
		}
		return TRUE;
	}
	else
	{
		if(bkeyon)
			KeyActRun(key);
		else
			SetRunFlag(INTF_OFF);
	}
		
	return TRUE;
}

BOOL KeySafedoorProc(UI16 key, BOOL bkeyon, UI16 repeat_cnt, UI32 start_press_ms)
{
	PSAFEDOOR  psfdr = (PSAFEDOOR)GetPartByID(MK_PART(SAFEDOOR_ID, 1));   
	UI16 opermode = GetOperMode();
	UI8 act_sn;

	/*忽略重复按键*/
	if(repeat_cnt > 1)
		return TRUE;

	act_sn = GET_ACT_SN(key);

	if((opermode == OM_MANUAL || opermode == OM_ADJU) && ChkSysCfg(CO0_ATSD))
	{
		if(bkeyon)
		{
			if(act_sn == OS_SDRC)
			{
				if(ChkActRun(GetPartByID(key), ACT_SN_BIT(OS_SDRC)) || ChkActRun(GetPartByID(key), ACT_SN_BIT(OS_SDRO)))
					SetRunFlag(INTF_OFF);
					//ActErrorAbort(psfdr->act+OS_SDRC-1);
				else if(!ChkActRun(GetPartByID(key), ACT_SN_BIT(OS_SDRC)))
					KeyActRun(key);
		    }
			else if(act_sn == OS_SDRO)
			{
				if(ChkActRun(GetPartByID(key), ACT_SN_BIT(OS_SDRO)) || ChkActRun(GetPartByID(key), ACT_SN_BIT(OS_SDRC)))
					SetRunFlag(INTF_OFF);
					//ActErrorAbort(psfdr->act+OS_SDRO-1);
				else if(!ChkActRun(GetPartByID(key), ACT_SN_BIT(OS_SDRO)))
					KeyActRun(key);
			}

		}
		return TRUE;
	}	

	if(opermode == OM_MANUAL || opermode == OM_ADJU || !ChkSysCfg(CO4_PWDR))
		return FALSE;

	if(bkeyon)
	{
		if(ChkActRun(GetPartByID(key), ACT_SN_BIT(OS_SDRC)))
            ActErrorAbort(psfdr->act+OS_SDRC-1);
        else if(ChkActRun(GetPartByID(key), ACT_SN_BIT(OS_SDRO)))
			ActErrorAbort(psfdr->act+OS_SDRO-1);
		else if(ChkOperMode(OM_AA_FLG) || ChkActRun(GetPartByID(MK_PART(MACHINE_ID, 1)), ACT_SN_BIT(OS_RCYC)))
			KeyActRun(key);
	}
	else if(act_sn == OS_OPNM)
	{
		SetRunFlag(INTF_OFF);
	}
	
	return TRUE;
}

void ModeKeyProc(UI16 key, BOOL bkeyon)
{
	WORD opermode;
	PSAFEDOOR	psfdr;
	PAUTOCTRL pat;
	PADJUST padj = (PADJUST)GetMachineSubPart(ADJUST_ID, 1);
	PROBOT  prbt = (PROBOT)GetMachineSubPart(ROBOT_ID, 1);
    PSERVO	p_servo = &g_hydrservo;
	
	opermode = GetOperMode();

	if(!bkeyon)
		return;
	
	switch(key)
	{
		case KEY_SEMIAUTO:
		case KEY_TIMEAUTO:
		case KEY_SENSORAUTO:
			//if(opermode==OM_MANUAL)
			if(!DAIsAdjust())           
			{  
				PEJECT pp;				
				BOOL err = FALSE;

				pp =(PEJECT)GetPartByID(MK_PART(EJECT_ID, 1));
				psfdr = (PSAFEDOOR)GetPartByID(MK_PART(SAFEDOOR_ID, 1));
				pat = (PAUTOCTRL)GetPartByID(MK_PART(AUTOCTRL_ID, 1));
				
				if((pp != NULL) && !ChkSysCfg(XF3_SPSTO)&&(GetSys()->prbt->md->FL_ROBOTCTRL == 0))
					err = (EjectGetMode(pp) == 0);

				if(key == KEY_SEMIAUTO || err)
 					SetOperMode(OM_AUTO | OM_SEMI);
				
				else if(key == KEY_TIMEAUTO)
				{
					if((opermode == (OM_AUTO | OM_SEMI)) || (opermode == (OM_AUTO | OM_SENR)))
						psfdr->SemiTurnTime = 1;
					else
						psfdr->SemiTurnTime = 0;
					SetOperMode(OM_AUTO | OM_TIME);
				}
				else
				{
					if((opermode == (OM_AUTO | OM_SEMI)) || (opermode == (OM_AUTO | OM_TIME)))
					{
						psfdr->SemiTurnTime = 1;
						SensorDropChkReset(pat);
					}
					else
						psfdr->SemiTurnTime = 0;
					SetOperMode(OM_AUTO | OM_SENR);
				}
			
				SetRunFlag(INIF_AUTO | INIF_AUTO1);
			    if(opermode==OM_MANUAL)
				    SetAutoStartProc();

				if((err && key != KEY_SEMIAUTO) && (!ChkSysCfg(XF3_SPSTO)))
					SET_PART_ERR(pp, ER0_EJTM);	
			}  
			break;
			
		case  KEY_ADJUST:
			if(!DAIsAdjust())     //增加限定，在DA校正画面，不响应调模按键    20160510   
			{
			if(opermode==OM_MANUAL) 
			{
				SetOperMode(OM_ADJU);
				AutoAlarmReStart();
			}  
			else if ((opermode==OM_ADJU))
			{				
				if(CheckPIUsed(GetSubDataByID(MK_PART(ADJUST_ID, 1), PI_ID, GET_IND(PADJUST_PI, I_ADJ_CHK))))
				{
					SetOperMode(OM_AA_FLG);
					KeyActRun(MK_ACT(ADJUST_ID, 1, OS_ADJA));
				}  
				else
				{
					ManualInit();
					SetOperMode(OM_MANUAL);
				}
			}
			else
			{
				ManualInit();
				SetOperMode(OM_MANUAL);
			}
			AdjMoldInit(TRUE);
			}
			break;

		case KEY_RUN:
			if(bkeyon && (CHK_PART_ERR(GetSys(),ER0_SKEY)>0))  
			{
				g_statkey.runkey = TRUE;
	        }
			break;
		case KEY_ALARM_CLEAR:	
			if(ChkOperMode(OM_MANUAL) || ChkOperMode(OM_ADJU)) 
			{
				ManualSafeDoorErrOut();	
				ClearAllPartError();	//清除警报	
				ServoRecoverActNoEn();	//清除驱动警报
				MotorEnableOnAct();		//驱动重新使能
			}			
			break;
		case  KEY_WARM:
			if(bkeyon)
			{
				if(CUS_ID == CS_TJ)
				{
					if(!GetHeatingStatus())		
						WarmOn();
				}
				else
				{
					if(GetHeatingStatus())
					{
						GetSys()->md->CUSTOM_PARA[0] = 3;
				  		WarmOff();
					}
					else
						WarmOn();
				}
			}
			break;

		case  KEY_WARMOFF:
			if(bkeyon)
			{
				if(GetHeatingStatus())
				{
					GetSys()->md->CUSTOM_PARA[0] = 4;
				  	WarmOff();
				}
			}
			break;

		case KEY_MWON:			//模温电热开启
			if(bkeyon)
			{
				if(GetMwStatus())
			  		Mwoff();
				else
					Mwon();
			}
			break;

		case KEY_MWOFF:			//模温电热关闭
			if(bkeyon)
			{
				if(GetMwStatus())
				  	Mwoff();
			}
			break;
		
		case KEY_MANUAL:
			if(opermode!=OM_MANUAL && ChkSysCfg(CO43_CHAEJT)) 
			{
				ManualInit();
				SetOperMode(OM_MANUAL);
			}
			break;
			
		case KEY_BY1:
			break;

        case KEY_SVALM_CLEAR:
			if(ChkOperMode(OM_MANUAL) || ChkOperMode(OM_ADJU)) 
			{
				SetPOBit(p_servo->po->O_SV_CLR, PC_ON);
				SetPOBitDelay(p_servo->po->O_SV_CLR, PC_OFF, 100);
			}	
			break;
            
		default:
	        break;
	}
}  

void ActKeyProc(UI16 key, BOOL bkeyon, UI16 repeat_cnt, UI32 start_press_ms)
{
	UI8	part_type;
	UI8	part_sn;
	UI8	act_sn;
	UI8	type;
	const ACT_KEY_MAP* pm;
    PINJECT pinj = (PINJECT)GetPartByID(MK_PART(INJECT_ID,1));
	PACT pact1;
	
	part_type = GET_PART_TYPE(key);
	part_sn = GET_PART_SN(key);
	act_sn = GET_ACT_SN(key);

	if(act_sn == 0)
		return;

	type = KEYON_OFF;
	pm = act_key_map;
	while(pm < act_key_map + ARRAY_NUM(act_key_map))
	{
		if(pm->part_type == part_type && pm->act_sn == act_sn)
		{
			
			if(pm->cmd_func == NULL || (*pm->cmd_func)(key, bkeyon, repeat_cnt, start_press_ms) == FALSE)
			{
				type = pm->type;
				if(ChkOperMode(OM_ADJU) && pm->adj_act_sn != 0)
				{
					key = MK_ACT(part_type, part_sn, pm->adj_act_sn);
					act_sn = pm->adj_act_sn;
				}
				break;
			}
			else
			{
				/**/
				return;
			}
		}
		pm++;
	}

	if((ChkSysCfg(CO5_INGEJT) == 0) && ChkOperMode(OM_AUTO) && (pinj != NULL))		//自动模式下压料缸按键有效,射出动作开始后按键无效，等储料结束后才有效(lzh 2022.1.4)
	{
		if((pinj->pchgprs->chgprsflag == 0) && (part_type == CHGPRS_ID) && (part_sn == 1) && (act_sn == OS_CHGPRSF || act_sn == OS_CHGPRSB))
		{

		}
		else
		{
			return;
		}
	}
	else if(GetOperMode() != OM_MANUAL && GetOperMode() != OM_ADJU)
		return;
	
	/*忽略重复按键，只处理键按下和放开*/
	if(repeat_cnt > 1)
		return;
	
	switch(type)
	{
		case KEYON_OFF:
			if(bkeyon && !ChkActRun(GetPartByID(MK_PART(part_type, part_sn)), ACT_SN_BIT(act_sn)))
				KeyActRun(key);
			else
			{
				if((ChkSysCfg(CO5_INGEJT) == 0) && ChkOperMode(OM_AUTO) && (pinj != NULL))
				{					
					if((part_type == CHGPRS_ID) && (part_sn == 1) && (act_sn == OS_CHGPRSF))
					{
						pact1 = pinj->pchgprs->act + OS_CHGPRSF - 1;
						if(pact1 != NULL)
							EndActTsk(pact1->ptsk,TRUE);
					}
					else if((part_type == CHGPRS_ID) && (part_sn == 1) && (act_sn == OS_CHGPRSB))
					{
						pact1 = pinj->pchgprs->act + OS_CHGPRSB - 1;
						if(pact1 != NULL)
							EndActTsk(pact1->ptsk,TRUE);
					}
					else
						SetRunFlag(INTF_OFF);
				}
				else
					SetRunFlag(INTF_OFF);
			}
			break;
			
		case KEYON_ONE:
			if(bkeyon && !ChkActRun(GetPartByID(MK_PART(part_type, part_sn)), ACT_SN_BIT(act_sn)))
				KeyActRun(key);
			break;
			
		case KEYON_SWICTH:
			if(bkeyon)
			{
				if(!ChkActRun(GetPartByID(MK_PART(part_type, part_sn)), ACT_SN_BIT(act_sn)))
					KeyActRun(key);
				else
					SetRunFlag(INTF_OFF);
			}
			break;
			
		default:
			break;
	}
}

void KeyPreProc(PKEY_ST pkey)
{
	if(pkey->bnew)
	{
		pkey->last_rx_ms = (WORD)GetMsTick();
		if(pkey->lastkey != pkey->currentkey)
		{
			if(pkey->repeat > 0)
			{
				//上次按键没有收到放开按键，变为上次按键的放开键
				pkey->repeat = 0;
				pkey->currentkey = pkey->lastkey;
			}
			else
			{
				//新的按键
				if(pkey->keyon)
					pkey->repeat = 1;
				else
					pkey->repeat = 0;

				pkey->lastkey = pkey->currentkey;
			}
		}
		else
		{
			if(pkey->keyon && pkey->repeat < 0xffff)
				pkey->repeat++;
			else
				pkey->repeat = 0;
		}
	}
#if 1	
	else if((WORD)((WORD)GetMsTick() - pkey->last_rx_ms) > PNLWAITTIME)//  && (WORD)(GetMsTick() - g_key.last_keypress_ms) > 1000)
	{
		/*键接收超时*/
		/*如果当前没有收到键放开，新键一个放开按键*/
		if(pkey->repeat > 0)
		{
			pkey->last_rx_ms = (WORD)GetMsTick();
			pkey->repeat = 0;
			pkey->bnew = TRUE;
			db_printf("Key tmout\r\n");
		}
	}
#endif	
}

void KeyScanProc()
{
	PKEY_ST		pkey;
	PMOTOR pp = (PMOTOR)GetPartByID(MK_PART(MOTOR_ID, 1));
	PADJUST padj = (PADJUST)GetMachineSubPart(ADJUST_ID, 1);
	
	pkey = &g_key;

	KeyPreProc(pkey);

	//if(!pkey->bnew)
	//	return;

	/*忽略连续按键*/
#if 0
	if(pkey->repeat > 1)
	{
		pkey->bnew = FALSE;
		return;
	}
#endif
	KeyRunExtPi();	//外部运行键

    if(pkey->bnew)
    {
		if(pkey->repeat == 1)
			g_key.last_keypress_ms = GetMsTick();

		if(pkey->currentkey < ACT_KEY_START || pkey->currentkey == KEY_BY1)
		{
			if(pkey->repeat == 1)
				ModeKeyProc(pkey->currentkey, pkey->repeat == 1);
		}
		else
		{
			
			if(((!DAIsAdjust())||(pkey->currentkey ==0x0411)||(pkey->currentkey==0x3001))&&!CHK_PART_ERR(pp, ER0_MOTORERR))   //DA校正时候不响应动作,马达和润滑除外   2014-12-04
			{
				if((padj->mh->FL_QDC == 1) && (CUS_ID == CS_TONGYONG) && (ChkOperMode(OM_ADJU)))
				{	
				}
				else
				{
					if(pkey->repeat == 1)
						ActForkTblRefresh(FALSE);
		
					ActKeyProc(pkey->currentkey, pkey->repeat == 1, pkey->repeat, g_key.last_keypress_ms);
				}
			}
		}	
		pkey->bnew = FALSE;
    }
	return;
}

