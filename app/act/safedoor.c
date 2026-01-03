/******************************************************************************
  文 件 名   : safedoor.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 安全门及动作函数
  函数列表   :
              BSafeDoorAlarm
              BSafeDoorIsErr
              FSafeDoorIsClose
              FSafeDoorIsCloseErrChk
              FSafeDoorIsOpen
              GetSafeDorrChkState
              InitSafeDoorPart
              SafeDoorAlarmLoop
              SafeDoorClose
              SafeDoorClsOff
              SafeDoorCmd
              SafeDoorOpen
              SafeDoorOpenChk
              SafeDoorOpnOff
              SafeDoorSetup
              SetSafeDoorClsFst
              SetSafeDoorClsSlw
              SetSafeDoorOpnFst
              SetSafeDoorOpnSlow
              SpecialSafeDoorCheck
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

void AutoSafeDoorChk(PSAFEDOOR  pp)
{
	if (ChkOperMode(OM_AUTO))
	{
		if((FSafeDoorIsOpen(pp)) && !ChkSysCfg(XF2_RSNOM) && !ChkAdFwdInplace(pp->pclmp->mh->CLMP_METERCH))
		{
			if((WORD)(Get100MsTick()-pp->Tm_TurntoManule)>300)
			{
				SET_PART_ERR(GetSys(), ER1_TMOUT);
			}
		}
		else
		{
			pp->Tm_TurntoManule = Get100MsTick();
		}
	}
	else
	{
		pp->Tm_TurntoManule = Get100MsTick();
	}		
}



/*******************************************************************************************
 *Brief			:安全门急停判定
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20201123
********************************************************************************************/
void SafeDoorRemerg(PSAFEDOOR  pp)
{	
	PI_STATUS testpi = CheckPI(pp->pi->I_SAFE_REMERG);

	if (ChkSysCfg(CO4_PWDR) && (testpi != PI_NOUSED))
	{
		if(testpi == pp->i_safe_remerg_temp)
		{
			if((UI16)((UI16)GetMsTick() - pp->i_safe_remerg_starttm) > 20)
			{
				if(testpi != pp->i_safe_remerg_last)
				{
					if (testpi)
					{
						SetRunFlag(INIF_SAFE_RESET);
						SET_PART_ERR(pp,ER0_SAFE_REMERG);
					}
					else
					{
						ClrRunFlag(INIF_SAFE_RESET);
						CLR_PART_ERR(pp,ER0_SAFE_REMERG);
					}	
					
					pp->i_safe_remerg_last = testpi;
				}
			}

			if (ChkRunFlag(INIF_SAFE_RESET))
			{
				SET_PART_ERR(pp,ER0_SAFE_REMERG);
			}
		}
		else
		{
			pp->i_safe_remerg_temp = testpi;
			pp->i_safe_remerg_starttm= (UI16)GetMsTick();
		}
	}	
}


void ManualSafeDoorErrOut()
{
   PSAFEDOOR  pp = (PSAFEDOOR)GetPartByID(MK_PART(SAFEDOOR_ID, 1));
   if(CHK_PART_ERR(pp, ER5_SACTLERR))    
       pp->sferroutbit = 1;
}

void SafeDoorErrOut(PSAFEDOOR pp)
{
    if(pp->sferroutbit == 1)
	{
	    pp->sferroutbit = 2;    
	    SetPOBit(pp->po->O_SDR_ERR,PC_ON);
	}
	else if(pp->sferroutbit == 2)
	{
	    pp->sferroutbit = 3;
	    SetPOBitDelay(pp->po->O_SDR_ERR, PC_OFF, 300);
	}
}

void SafeDoorErr()
{
    PSAFEDOOR  pp = (PSAFEDOOR)GetPartByID(MK_PART(SAFEDOOR_ID, 1));

	if(pp==NULL)
	    return;

    SafeDoorErrOut(pp);

	if(CheckPI(pp->pi->I_SDR_ERR) == PI_SET && GetPOBit(pp->po->O_SDR_ERR) == 0)
	{
	    if((UI16)(GetMsTick()-pp->sferrfiltime) >= 200)
		{
		    pp->sferrfiltime = GetMsTick();
		    if(!CHK_PART_ERR(pp, ER5_SACTLERR))
			{
		        SET_PART_ERR(pp, ER5_SACTLERR);
		        StandAlarm(TRUE);
				if(ChkSysCfg(CO5_SAFEDRCTL))
				{
					if(MotorIsOn())	
	      		        MotorDown();
				}
			}
		}
	}
	else
	{
	    pp->sferrfiltime = GetMsTick();
	}
}

BOOL SafeDoorValveAlarm(PSAFEDOOR pp)
{   
    if(CHK_PART_ERR(pp, ER5_SQ42)||CHK_PART_ERR(pp, ER5_SAVALVE)||CHK_PART_ERR(pp, ER5_PRESAVALVE)||CHK_PART_ERR(pp,ER1_SQ42SAVALVE))
	    return TRUE;
	else
	    return FALSE;	   
}
void SafeDoorValveCheck(PSAFEDOOR pp)
{
    PCLAMP pclmp;
	
	if(pp==NULL || pp->pclmp==NULL)
	    return;
	
	pclmp = pp->pclmp;
    if(CheckPI(pp->pi->I_SDR_CLS) == PI_RESET)
	{
	    if(pp->SQ42checkflag == 0)
		{
		    pp->SQ42checkflag = 1;
		    pp->SQ42checktime = GetMsTick();
		}
		else if(pp->SQ42checkflag == 1)
		{
			if(CheckPI(pp->pi->I_SDR_SQ42) == PI_RESET)
		    {
			    if((UI16)(GetMsTick()-pp->SQ42checktime) >= 3000)
				{
		            SET_PART_ERR(pp, ER5_SQ42);
						
		            StandAlarm(TRUE);
				}
			}
			else
			{
				CLR_PART_ERR(pp, ER5_SQ42);

			    pp->SQ42checkflag = 2;
			    pp->sfvchecktime = GetMsTick();
			}		       
		}
		else if(pp->SQ42checkflag == 2)
		{
		    if((UI16)(GetMsTick()-pp->sfvchecktime) >= 500)
			{
			    if(CheckPI(pp->pi->I_SDR_VALVE) == PI_RESET && !ChkActRun(pclmp, ACT_SN_BIT(OS_OPNM)))
				{
		            SET_PART_ERR(pp, ER1_SQ42SAVALVE);
		            StandAlarm(TRUE);
				}
				else
				{
					CLR_PART_ERR(pp, ER1_SQ42SAVALVE);
				}
			    if(CheckPI(pp->pi->I_SDR_PREVAVLE) == PI_RESET)
				{
		            SET_PART_ERR(pp, ER5_PRESAVALVE);
		            StandAlarm(TRUE);
				}
				else
				{
					CLR_PART_ERR(pp, ER5_PRESAVALVE);
				}
			}
		}
	}
	else
	{
	    pp->SQ42checkflag = 0;

		CLR_PART_ERR(pp, ER5_SQ42);
		CLR_PART_ERR(pp, ER1_SQ42SAVALVE);
		CLR_PART_ERR(pp, ER5_PRESAVALVE);
	}
}

void BSafeDoorAlarm(PSAFEDOOR pp)
{
	BOOL  needalarm;	

	if(pp == NULL)
		return;
		
	needalarm= (ChkOperMode(OM_AUTO|OM_AA_FLG) | AlarmIsOn());

	if(ChkSysCfg(XF3_RSDFM))
	{
		if(ChkActRun(pp->pclmp, ACT_SN_BIT(OS_CLSM) |ACT_SN_BIT(OS_PRSM) |ACT_SN_BIT(OS_OPNM)) && MotorIsOn())
			MotorDown();
	}                         

	if(ChkSysCfg(XF3_RSDMA))            // turn to manual
  	{
		ManualInitErr(); 
		SetOperMode(OM_MANUAL);   //之前没有这句， 手动没切	   DK 2014.1104
  	}
	else 
	{
		if (MotorIsOn())	
      		MotorDown();

		ManualInitErr();
	 	SetOperMode(OM_MANUAL);
	}	

	if (needalarm)
		StandAlarm(TRUE);
}


BOOL BSafeDoorIsErr(PSAFEDOOR pp)
{
	BOOL bclose= TRUE;	
	
	if(pp == NULL)
		return FALSE;

	if(CheckPIUsed(pp->pi->I_RSD_CLS))
	{
		if(ChkSysCfg(CO0_RSDL) == 0)
			bclose = !TestPI(pp->pi->I_RSD_CLS);
		else
			bclose = TestPI(pp->pi->I_RSD_CLS);
			
   		if(CheckPIUsed(pp->pi->I_RSD_CLS2))
   		{
			if(ChkSysCfg(CO0_RSDL) == 0)
				bclose = (bclose || !TestPI(pp->pi->I_RSD_CLS2));
			else
				bclose = (bclose || TestPI(pp->pi->I_RSD_CLS2));
   		}
	}
	
	return(bclose);
}

BOOL FSafeDoorIsOpen(PSAFEDOOR pp)
{
	BOOL bopen= FALSE;		

	if(pp == NULL)
		return FALSE;
		
	if(CheckPIUsed(pp->pi->I_SDR_CLS))
	{
		if(CUS_ID==CS_YUYAOLOG)
			bopen = !TestPI(pp->pi->I_SDR_CLS);
		else
		{
			if(ChkSysCfg(CO0_SDRL) != 0)
				bopen = TestPI(pp->pi->I_SDR_CLS);
			else
				bopen = !TestPI(pp->pi->I_SDR_CLS);
		}
			
   		if(CheckPIUsed(pp->pi->I_SDR_CL2))
   		{
			if(ChkSysCfg(CO0_SDRL) != 0)
				bopen = (bopen && TestPI(pp->pi->I_SDR_CL2));
			else
				bopen = (bopen && !TestPI(pp->pi->I_SDR_CL2));
   		}
	}
	
	return(bopen);
}

BOOL FSafeDoorIsClose(PSAFEDOOR pp)
{
	BOOL bclose= FALSE;		

	if(pp == NULL)
		return FALSE;
		
	if(CheckPIUsed(pp->pi->I_SDR_CLS))
	{
		if(CUS_ID==CS_YUYAOLOG)
			bclose = TestPI(pp->pi->I_SDR_CLS);
		else
		{
			if(ChkSysCfg(CO0_SDRL) != 0)
				bclose = !TestPI(pp->pi->I_SDR_CLS);
			else
				bclose = TestPI(pp->pi->I_SDR_CLS);
		}
			
   		if(CheckPIUsed(pp->pi->I_SDR_CL2))
   		{
			if(ChkSysCfg(CO0_SDRL) != 0)
				bclose = (bclose && !TestPI(pp->pi->I_SDR_CL2));
			else
				bclose = (bclose && TestPI(pp->pi->I_SDR_CL2));
   		}
	}

	return(bclose);
}

BOOL FSafeDoorIsCloseSAFERUL(PSAFEDOOR pp)
{
	BOOL bclose= FALSE;		

	if(pp == NULL)
		return FALSE;
	if(CheckPIUsed(pp->pi->I_SDR_SAFE_RULE))
	{
		if(ChkSysCfg(CO0_SDRL) != 0)
			bclose = !TestPI(pp->pi->I_SDR_SAFE_RULE);
		else
			bclose = TestPI(pp->pi->I_SDR_SAFE_RULE);	
	}
	return(bclose);
}

BOOL FSafeDoorIsCloseErrChk(PSAFEDOOR pp)
{
	if(PartErrSet(pp, ER0_CSDR, !FSafeDoorIsClose(pp)))
	{
		StandAlarm(FALSE);
		return TRUE;
	}

	return FALSE;
}

void SpecialSafeDoorCheck(PSAFEDOOR pp)
{
	if(pp == NULL)
		return;

	if(!CheckPIUsed(pp->pi->I_SDR_CL2))
		return;
		
	switch (pp->checkstep)
    {	
      	case 0:
			if (TestPI(pp->pi->I_SDR_CLS) != TestPI(pp->pi->I_SDR_CL2))
        	{
        		pp->checkstart = Get100MsTick();
        		pp->checkstep ++;	
        	}
        	break;	

      	case 1:
        	if (TestPI(pp->pi->I_SDR_CLS)==TestPI(pp->pi->I_SDR_CL2))
          		pp->checkstep = 0;
          			
        	else if (DIFF_100MS(pp->checkstart) >SAFEDOOR_SPECIALCHK_ERR_TM/100)
        	{
				pp->checkstep ++;	
				SET_PART_ERR(pp, ER3_FSER);
				StandAlarm(TRUE);
				pp->chkerr = TRUE;
        	}
        	break;	

		case 2: 
        	if (pp->chkerr)
			{
				if (!CHK_PART_ERR(pp, ER3_FSER))
				{
					SET_PART_ERR(pp, ER3_FSER);
					StandAlarm(TRUE);
				}				     
			}

			break;
	}	  
}

void SafeDoorAlarmLoop(PSAFEDOOR pp)
{
	if(pp == NULL)
		return;

	if(CUS_ID!=CS_YUYAOLOG)
		SpecialSafeDoorCheck(pp);
	if(ChkSysCfg(XF0_SAFED))
    	SafeDoorValveCheck(pp);
	
	if(BSafeDoorIsErr(pp))  	        
	{
		if((UI16)(Get10MsTick()-pp->bsafedoor_alarm_10ms) > 50)
		{
			if(!pp->bsafedoor_alarm|| !ActTskIsEmpty())
			{
				SET_PART_ERR(pp, ER3_RSDR);
				pp->bsafedoor_alarm = 1;
			}
		}	
	}
	else
	{ 
		CLR_PART_ERR(pp, ER3_RSDR);
		pp->bsafedoor_alarm_10ms = Get10MsTick();
		pp->bsafedoor_alarm = 0;
	}

	if(ChkSysCfg(CO4_USSS) && ChkOperMode(OM_AUTO) && !CycleTskIsRun()) 
	{
		if(FSafeDoorIsOpen(pp))
		{
			SET_PART_ERR(pp, ER0_CSDR);			
			ManualInitErr();
			StandAlarm(FALSE);
		    SetOperMode(OM_MANUAL);
		}  
  	}
	AutoSafeDoorChk(pp);
	SafeDoorRemerg(pp);
}

void SafeDoorOpnOff(PSAFEDOOR pp, PACT pact)
{
	if( pp != NULL)
	{
		SetPOBit(pp->po->O_SDR_OPN,PC_OFF);
		SetPOBit(pp->po->O_SDR_FST,PC_OFF);
		SetPOBit(pp->po->O_SDR_SLW,PC_OFF);
		if(ChkSysCfg(CO3_HYPD))
			HYDR_CLS(pp->mh->DAPRES_SAFEDROFFRAMP, pp->mh->DAFLOW_SAFEDROFFRAMP);
	}
}

void SafeDoorOpnOffDelay(PSAFEDOOR pp, PACT pact)
{
	if( pp != NULL)
	{
		SetPOBit(pp->po->O_SDR_OPN,PC_OFF);
		SetPOBit(pp->po->O_SDR_FST,PC_OFF);
		SetPOBit(pp->po->O_SDR_SLW,PC_OFF);
		if(ChkSysCfg(CO3_HYPD))
			HYDR_CLS(pp->mh->DAPRES_SAFEDROFFRAMP, pp->mh->DAFLOW_SAFEDROFFRAMP);
	}
}


void SafeDoorClsOff(PSAFEDOOR pp, PACT pact)
{
	if(pp != NULL)
	{
		SetPOBit(pp->po->O_SDR_CLS, PC_OFF);
		SetPOBit(pp->po->O_SDR_FST, PC_OFF);
		SetPOBit(pp->po->O_SDR_SLW, PC_OFF);
		
		if(ChkSysCfg(CO3_HYPD))
			HYDR_CLS(pp->mh->DAPRES_SAFEDROFFRAMP, pp->mh->DAFLOW_SAFEDROFFRAMP);
	}
}

void SafeDoorClsOffDelay(PSAFEDOOR pp, PACT pact)
{
	if(pp != NULL)
	{
		SetPOBit(pp->po->O_SDR_CLS, PC_OFF);
		SetPOBit(pp->po->O_SDR_FST, PC_OFF);
		SetPOBit(pp->po->O_SDR_SLW, PC_OFF);
		
		if(ChkSysCfg(CO3_HYPD))
			HYDR_CLS(pp->mh->DAPRES_SAFEDROFFRAMP, pp->mh->DAFLOW_SAFEDROFFRAMP);
	}
}


/*---------------------------------------------------------------------------+
|           code segment                                                     |
+---------------------------------------------------------------------------*/
void SetSafeDoorOpnFst(PSAFEDOOR pp, PACT pact)
{
	UI16 hydelay;
	
	hydelay = pp->mh->SFDROPN_HYDELAY;
	SetPOBit(pp->po->O_SDR_OPN, PC_ON);
	SetPOBit(pp->po->O_SDR_FST, PC_ON);
	if(pp->hy->SERVO_KP_SAFETYDOOR==0)
		pp->hy->SERVO_KP_SAFETYDOOR = 700;
	if(pp->hy->SERVO_KI_SAFETYDOOR==0)
		pp->hy->SERVO_KI_SAFETYDOOR=100;
	if(ChkSysCfg(CO3_HYPD))
	{
		ActSetHydr(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_SAFETYDOOR,pp->pa->DAFLOW_SAFETYDOOR, 0,0, pp->mh->DAPRES_SAFEDRZRAMP,pp->mh->DAFLOW_SAFEDRZRAMP,
							HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_SAFETYDOOR),pp->hy->HYDR_PUMPS_SAFETYDOOR,pp->hy->HYDR_PUMPS_SAFETYDOOR,
							pp->hy->SERVO_KP_SAFETYDOOR,pp->hy->SERVO_KI_SAFETYDOOR,pp->hy->SERVO_KD_SAFETYDOOR,1,HYDR_PRI(pp->hy->HYDR_PORT_SAFETYDOOR));  
	}
}
	
void SetSafeDoorOpnSlow(PSAFEDOOR pp, PACT pact)
{
	//UI16 hydelay;
	
	//hydelay = pp->pa->SFDROPN_HYDELAY;
	SetPOBit(pp->po->O_SDR_FST,PC_OFF);
  	SetPOBit(pp->po->O_SDR_OPN,PC_ON);
	SetPOBit(pp->po->O_SDR_SLW,PC_ON);
	if(pp->hy->SERVO_KP_SAFETYDOOR==0)
		pp->hy->SERVO_KP_SAFETYDOOR = 700;
	if(pp->hy->SERVO_KI_SAFETYDOOR==0)
		pp->hy->SERVO_KI_SAFETYDOOR=100;
	if (ChkSysCfg(CO3_HYPD))
	{
		ActSetHydr(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_SAFETYDOORSLOW,pp->pa->DAFLOW_SAFETYDOORSLOW,0,0,       0,0,
							0,HYDR_PORT(pp->hy->HYDR_PORT_SAFETYDOOR),pp->hy->HYDR_PUMPS_SAFETYDOOR,pp->hy->HYDR_PUMPS_SAFETYDOOR,
							pp->hy->SERVO_KP_SAFETYDOOR,pp->hy->SERVO_KI_SAFETYDOOR,pp->hy->SERVO_KD_SAFETYDOOR,0,HYDR_PRI(pp->hy->HYDR_PORT_SAFETYDOOR)); 
	}
}

void SetSafeDoorClsFst(PSAFEDOOR pp, PACT pact)
{
	UI16 hydelay;
	
	hydelay = pp->mh->SFDRCLS_HYDELAY;

	SetPOBit(pp->po->O_SDR_CLS,PC_ON);
	SetPOBit(pp->po->O_SDR_FST,PC_ON);
	if(pp->hy->SERVO_KP_SAFETYDOOR==0)
		pp->hy->SERVO_KP_SAFETYDOOR = 700;
	if(pp->hy->SERVO_KI_SAFETYDOOR==0)
		pp->hy->SERVO_KI_SAFETYDOOR=100;	
	if(ChkSysCfg(CO3_HYPD))
	{
	    ActSetHydr(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_SAFETYDOOR,pp->pa->DAFLOW_SAFETYDOOR,0,0,      pp->mh->DAPRES_SAFEDRZRAMP,pp->mh->DAFLOW_SAFEDRZRAMP,
						HYDRDELY,HYDR_PORT(pp->hy->HYDR_PORT_SAFETYDOOR),pp->hy->HYDR_PUMPS_SAFETYDOOR,pp->hy->HYDR_PUMPS_SAFETYDOOR,
						pp->hy->SERVO_KP_SAFETYDOOR,pp->hy->SERVO_KI_SAFETYDOOR,pp->hy->SERVO_KD_SAFETYDOOR,1,HYDR_PRI(pp->hy->HYDR_PORT_SAFETYDOOR)); 
	}
}

void SetSafeDoorClsSlw(PSAFEDOOR pp, PACT pact)
{
	//UI16 hydelay;
	
	//hydelay = pp->pa->SFDRCLS_HYDELAY;

	SetPOBit(pp->po->O_SDR_CLS, PC_ON);
	SetPOBit(pp->po->O_SDR_FST, PC_OFF);
	SetPOBit(pp->po->O_SDR_SLW, PC_ON);
	if(pp->hy->SERVO_KP_SAFETYDOOR==0)
		pp->hy->SERVO_KP_SAFETYDOOR = 700;
	if(pp->hy->SERVO_KI_SAFETYDOOR==0)
		pp->hy->SERVO_KI_SAFETYDOOR=100;	

	if(ChkSysCfg(CO3_HYPD))
	{
		ActSetHydr(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->pa->DAPRES_SAFETYDOORSLOW,pp->pa->DAFLOW_SAFETYDOORSLOW,0,0,      0,0,
					0,HYDR_PORT(pp->hy->HYDR_PORT_SAFETYDOOR),pp->hy->HYDR_PUMPS_SAFETYDOOR,pp->hy->HYDR_PUMPS_SAFETYDOOR,
					pp->hy->SERVO_KP_SAFETYDOOR,pp->hy->SERVO_KI_SAFETYDOOR,pp->hy->SERVO_KD_SAFETYDOOR,0,HYDR_PRI(pp->hy->HYDR_PORT_SAFETYDOOR)); 
	}
}

/*eject safedoorchk*/
/*此函数中的chkcntset很重要，如果在安全门开的时间段，
无法检测到机械安全检知点变化，由于SafeDoorOpenChk不会再进入，
则在再循环过程中，需要重新开关一遍安全门，
反之则不需要开关安全门*/

void SafeDoorOpenChk(PSAFEDOOR pp, PACT pact)
{
	PSAFEDOORCHK pst;
	
	if(pp != NULL)
	{
		pst = &(pp->chk_st);
		pst->dooropn = FSafeDoorIsOpen(pp);
		if(pst->dooropn)
		{
			pst->chkcnt++;

			if(ChkSysCfg(XF0_MLATX))  //永生要求代码打上，面板“安全门检测时间”灰掉。此处默认500ms。
            {
            	pp->mh->SFDRCLS_TMCHECK = 50;
            }
			else
			{
				/*最小300ms，最大2s*/
				if(pp->mh->SFDRCLS_TMCHECK < 30 || pp->mh->SFDRCLS_TMCHECK > 200)
				{
					pp->mh->SFDRCLS_TMCHECK = 30;
				}
			}

//			if(pst->chkcnt >= 1000 && pact != NULL)   //占时先默认为1000，即0.4*1000 = 400ms
			if(pst->chkcnt >= (pp->mh->SFDRCLS_TMCHECK*100)/MAX_LOOP_100US && pact != NULL)
				pst->chkoper = pact->id;
		}
		else
		{
			pst->chkcnt = 0;
			pst->chkoper = 0;
			pst->dooropn = FALSE;
		}

		if(ClampMecSafeChk(pp->pclmp) != PI_SET) 
			pst->doormecsaf = TRUE;
	}
}

PSAFEDOORCHK GetSafeDorrChkState(PSAFEDOOR pp)
{
	if(pp != NULL)
		return &(pp->chk_st);
	else
		return NULL;
}

ACT_RET SafeDoorOpen(PACT pact, UI32 para)
{
	PSAFEDOOR pp = (PSAFEDOOR)(pact->part);
	ACT_RET	ret = ACT_OK;

	if(pp == NULL || ChkRunFlag(INIF_SAFE_RESET))
		return ACT_ERR_ABORT;
	
	switch (pact->step)
	{
		case INIT1_STEP:		//YF
			ACT_START();
			if(ChkActRun(pp, ACT_SN_BIT(OS_SDRC)))
                return ACT_ERR_ABORT;   //安全门关过程中，不允许执行安全门开
			else if(ChkSysCfg(CO4_PWDR))
			{
				if(pp->pa->TM_PWRDOOROPEN > 0)
					ActSetTM(pact->ptsk, pp->pa->TM_PWRDOOROPEN);
					
				ACT_STEP_NEW(SAFETYDOOROPN);
      		}
     		else
     		{
				ACT_STEP_NEW(END_STEP);
     			ret =  ACT_END;
			}
			break;
			
		case SAFETYDOOROPN:
		case SAFETYDOOROPN + 1:
			if (MoveLocationNMControlProc(pp->pi->I_SDR_OSL))
				ACT_STEP_NEW(SAFETYDOOROPNSLOW);

			if(TestPI(pp->pi->I_SDR_OPN) || (pp->pa->TM_PWRDOOROPEN != 0 && ACT_RUN_MS >= ((UI32)pp->pa->TM_PWRDOOROPEN)*10))
				ACT_STEP_NEW(END_STEP);

			CHK_SET_MOV(SetSafeDoorOpnFst);
			break;
			
		case SAFETYDOOROPNSLOW:
		case SAFETYDOOROPNSLOW + 1:
			if(MoveLocationNMControlProc(pp->pi->I_SDR_OPN) || (pp->pa->TM_PWRDOOROPEN != 0 && ACT_RUN_MS >= ((UI32)pp->pa->TM_PWRDOOROPEN)*10))
				ACT_STEP_NEW(END_STEP);

			if(ChkSysCfg(XF1_SOSLS) && STEP_10MS_CHK( pp->mh->TM_SAFEDOORSLOWVLV))
				SetPOBit(pp->po->O_SDR_SLW, PC_OFFD);

			CHK_SET_MOV(SetSafeDoorOpnSlow);
			break;

		case END_STEP:
			SET_OFF(SafeDoorOpnOffDelay);
			ret =  ACT_END;
			break;
      			
		default:
			ret =  ACT_END;
			break;
	}
	return ret;
}	

	
ACT_RET SafeDoorClose(PACT pact, UI32 para)
{
	PSAFEDOOR pp = (PSAFEDOOR)(pact->part);
	ACT_RET	ret = ACT_OK;

	if(pp == NULL || ChkRunFlag(INIF_SAFE_RESET))
		return ACT_ERR_ABORT;
	
	switch (pact->step)
	{
		case INIT1_STEP:		//YF
			ACT_START();
			if(ChkActRun(pp, ACT_SN_BIT(OS_SDRO)))
                return ACT_ERR_ABORT;   //安全门开过程中，不允许执行安全门关
			else if(ChkSysCfg(CO4_PWDR))
			{
				CLR_PART_ERR(pp, ER5_PDSA);
				if(CheckPI(pp->pi->I_SDR_CLS) == PI_RESET)
					ACT_STEP_NEW(SAFETYDOORCLS);
				else
					ACT_STEP_NEW(SAFETYDOORCLSSLOW);
				//InitServoPIDPara(pp->hy->HYDR_PORT_SAFETYDOOR);
      		}
			else
			{
				ACT_STEP_NEW(END_STEP);
				ret = ACT_END;
			}
			break;

		case SAFETYDOORCLS:
		case SAFETYDOORCLS + 1:
			if(MoveLocationNMControlProc(pp->pi->I_SDR_SLW))
				ACT_STEP_NEW(SAFETYDOORCLSSLOW);

			if (TestPI(pp->pi->I_SDR_CLS)) 
				ACT_STEP_NEW(END_STEP);
				
			if (CheckPI(pp->pi->I_SDR_SAF) == PI_SET ||CheckPI(pp->pi->I_SAF_PLT) == PI_SET)
			{
				pact->ftaskstat |= INTF_ERR;
				ACT_STEP_NEW(END_STEP);
			}
			else if(CheckPI(pp->pi->I_SAFE_EDGE) == PI_RESET)
			{
				pact->ftaskstat |= INTF_ERR;
				CLR_PART_ERR(pp, ER0_CSDR);
				SET_PART_ERR(pp, ER0_SDREDGE);
				ACT_STEP_NEW(END_STEP);
			}

			CHK_SET_MOV(SetSafeDoorClsFst);
			break;
		      
		case SAFETYDOORCLSSLOW:
		case SAFETYDOORCLSSLOW + 1:
			if(MoveLocationNMControlProc(pp->pi->I_SDR_CLS))
				ACT_STEP_NEW(END_STEP);

			if(CheckPI(pp->pi->I_SDR_SAF) == PI_SET ||CheckPI(pp->pi->I_SAF_PLT) == PI_SET)
			{
				pact->ftaskstat |= INTF_ERR;
				ACT_STEP_NEW(END_STEP);
			}
			else if(CheckPI(pp->pi->I_SAFE_EDGE) == PI_RESET)
			{
				pact->ftaskstat |= INTF_ERR;
				CLR_PART_ERR(pp, ER0_CSDR);
				SET_PART_ERR(pp, ER0_SDREDGE);
				ACT_STEP_NEW(END_STEP);
			}

			if(ChkSysCfg(XF1_SOSLS) && STEP_10MS_CHK(pp->mh->TM_SAFEDOORSLOWVLV))
				SetPOBit(pp->po->O_SDR_SLW, PC_OFFD);

			CHK_SET_MOV(SetSafeDoorClsSlw);
			break;		

		case END_STEP:
			SET_OFF(SafeDoorClsOffDelay);
			if(pact->ftaskstat & INTF_ERR)
			{
				CLR_PART_ERR(pp, ER0_CSDR);
				if (!CHK_PART_ERR(pp, ER0_SDREDGE))
				{
					SET_PART_ERR(pp, ER5_PDSA);
				}
				ret= ACT_ERR_ABORT;
			}
			else
				ret = ACT_END;
			break;
		default:
			ret =  ACT_END;
			break;  	
	}	
	return ret;
}	

void SafeDoorSetup(PSAFEDOOR pp)
{
	if(pp != NULL)
		pp->pclmp = (PCLAMP)GetMachineSubPart(CLAMP_ID, GET_PART_SN(pp->part.id));
}

BOOL InitSafeDoorPart(PSAFEDOOR pp, UI8 sn)
{
	static const SUB_INIT_INFO sub_init[]  = SAFEDOOR_INIT_INFO;
	
	static const ACT_INIT_INFO act_init[] = {{OS_SDRO, OS_SDRO, ACT_NTYPE, SafeDoorOpen, SafeDoorOpnOff},
									   		{OS_SDRC, OS_SDRC, ACT_NTYPE, SafeDoorClose, SafeDoorClsOff}};
									     
	if(!INIT_PART(pp, SAFEDOOR_ID, sn, sub_init))
		return FALSE;

	if(!INIT_ACT(pp, act_init))
		return FALSE;

	SafeDoorSetup(pp);
 
	pp->chkerr = FALSE;
	pp->checkstep = 0;
	pp->checkstart = 0;

	pp->pi->I_SDR_CL2 = 1;
	pp->pi->I_RSD_CLS = 4;
	pp->pi->I_SDR_CLS = 11;
	
	return TRUE;
}

UI32 SafeDoorCmd(PSAFEDOOR pp, UI32 cmd, UI32 para)
{
	UI32 ret = TRUE;
	
	if(pp == NULL)
		return FALSE;

	switch(GET_CMD(cmd))
	{
		case CMD_MANUAL_INIT:
			pp->checkstep = 0;
			pp->chkerr = 0;
			pp->chk_st.chkcnt = 0;
			pp->chk_st.dooropn = 0;
			pp->chk_st.chkoper = 0;
			pp->chk_st.chkcntset = (UI16)(80 * 10 /MAX_LOOP_100US);         //安全门开检测时间设为默认值80ms
			pp->SemiTurnTime = 0;
			break;
		case CMD_SETUP:
			SafeDoorSetup(pp);
			break;

		default:
			ret = FALSE;
			break;
	}

	return ret;
}


