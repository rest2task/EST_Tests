/******************************************************************************
  文 件 名   : oil.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 油检测及油温
  函数列表   :
              InitOilPart
              OilFltErr
              OilLvErr
              OilTmpErr
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

#include "ioproc.h"
#include "alarm.h"
#include "oil.h"
#include "machapi.h"

#include "Canhead.h"
#include "TempCtrlComm.h"
#include "systempro.h"
#include "TempCtrl.h"



extern WORD tempcoolinit[TEMP_MAX_NUM];

OIL m_oil;

void InitTemp(PTEMPCTRLCOMM pp);
void InitCyclicData(PTEMPCTRLCOMM pp);
void InitPidParaPro(PTEMPCTRLCOMM pp);
UI16 GetRealSlaveNum(void);

BOOL OilLvErr()
{
	POIL pp = &m_oil;

	if (CheckPI(pp->pi->I_OIL_LVL) == PI_SET)
  	{		
		if (CUS_ID == CS_HAIXIN)
		{
			if (!CHK_PART_ERR(pp, ER1_OILV))
			{
				MotorDown();				
				ManualInitErr();
				SET_PART_ERR(pp, ER1_OILV);
				SetOperMode(OM_MANUAL);
			}			
		}
		else
		{
			SET_PART_ERR(pp, ER1_OILV);
		}
		return TRUE;
  	}

    	return(FALSE);    
}

BOOL OilFltErr()
{
	POIL pp = &m_oil;

	if(CheckPI(pp->pi->I_FIL_BLK) == PI_SET)
	{
		SET_PART_ERR(pp, ER1_FILT);
	    return TRUE;
	}

	return(FALSE);    
}

/*-----------------------------------------------------------------------------------+
|      油温冷却处理                                                                  |
+-----------------------------------------------------------------------------------*/
void  OilCoolerChk()
{
    if (m_oil.st->DATEMP_OIL >= m_oil.mh->DATEMP_COOLERONFOROIL)
    {
        if (!TestPOIsOn(m_oil.po->O_OILCOOL1))
        {
            SetPOBit(m_oil.po->O_OILCOOL1, PC_ON);
        }
    }
    else if (m_oil.st->DATEMP_OIL <= m_oil.mh->DATEMP_COOLEROFFFOROIL)
    {
        if (TestPOIsOn(m_oil.po->O_OILCOOL1))
        {
            SetPOBit(m_oil.po->O_OILCOOL1, PC_OFF);
        }
    }

    if (m_oil.st->DATEMP_OIL2 >= m_oil.mh->DATEMP_COOLERONFORWATER2)
    {
        if (!TestPOIsOn(m_oil.po->O_OILCOOL2))
        {
            SetPOBit(m_oil.po->O_OILCOOL2, PC_ON);
        }
    }
    else if (m_oil.st->DATEMP_OIL2 <= m_oil.mh->DATEMP_COOLEROFFFORWATER2)
    {
        if (TestPOIsOn(m_oil.po->O_OILCOOL2))
        {
            SetPOBit(m_oil.po->O_OILCOOL2, PC_OFF);
        }
    }
}

BOOL OilTmpErr()     
{
	POIL pp = &m_oil;

	if(g_tempcomm_num > 0)
	{
		if (pp->mh->DATEMP_OILMAX == 0)
		{
			return FALSE;
		}
		if (pp->mh->DATEMP_OILMAX < pp->mh->DATEMP_OILMIN)
        {
            return FALSE;
        }

        if (pp->st->DATEMP_OIL<= pp->mh->DATEMP_OILMAX && pp->st->DATEMP_OIL >= pp->mh->DATEMP_OILMIN)
        {
            return FALSE;
        }
        else
        {
        	SET_PART_ERR(pp, ER0_OILT);
            return TRUE;
        }
	}
	return FALSE;
}

BOOL WaterTmpErr()     
{
	POIL pp = &m_oil;

	if(g_tempcomm_num > 0)
	{
		if (pp->mh->DATEMP_OILMAX2 == 0)
		{
			return FALSE;
		}
		if (pp->mh->DATEMP_OILMAX2 < pp->mh->DATEMP_OILMIN2)
        {
            return FALSE;
        }

        if (pp->st->DATEMP_OIL2<= pp->mh->DATEMP_OILMAX2 && pp->st->DATEMP_OIL2 >= pp->mh->DATEMP_OILMIN2)
        {
            return FALSE;
        }
        else
        {
        	SET_PART_ERR(pp, ER0_OILT2);
            return TRUE;
        }
	}
	return FALSE;
}

/************************************************************/
/***函数名称：CheckOilLeak								*****/
/***函数功能：检测机器是否漏油							*****/
/***  当实际系统压力小于用户设定的最小系统压力			*****/
/***  且持续时间超过检测时间时系统警报，警报手动消除	*****/
/***作者：吕振辉										*****/
/***时间：2019-04-09								*****/
/************************************************************/
void CheckOilLeak()
{
	UI16 RealPress;
	UI16 portno;
	BOOL flag = FALSE;
	PADMETER pad = (PADMETER)GetPartByID(MK_PART(ADMETER_ID, 1));
	POIL pp = (POIL)GetPartByID(MK_PART(OIL_ID,1));
	PPUMP ppump;
	WORD i = 0;

	if ((pp == NULL) || (pad == NULL))
		return;

	if(!pp->mh->FL_CHKOIL)		//判断是否使用漏油检测功能（lzh,2018.12.10）
		return;
	
	if (ChkSysCfg(CO1_ECAT))
	{
		if(!MotorIsOn())
		{//马达未开，不检测漏油
			return;
		}
		
		for (i = 0;i < GetRealSlaveNum();i++)
		{
			ppump = g_pumpn[i];
			
			if (ppump->McCtrl.McSpdOutRPM > ppump->baseflow)
			{
				if (ppump->McCtrl.RealPres < (pp->mh->MIN_PRESS * 10))
				{
					if ((UI16)DIFF_10MS(ppump->oil_leak_starttm) > (pp->mh->TM_PROTECT))
					{
						SET_PART_ERR(pp, ER0_OILLEAK);
					}
				}
				else
				{
					ppump->oil_leak_starttm = Get10MsTick();
				}	
			}
			else
			{
				ppump->oil_leak_starttm = Get10MsTick();
			}
		}
	}
	else
	{
		RealPress = pad->ch_meter[DEF_PRSSENS_METR_CH-1];
	
		for(portno = 1;portno < 5;portno++)
		{
			if((p_WorkStru[portno].SpeedSet > 0) && (p_WorkStru[portno].PressSet > 0))
			{
				flag = TRUE;
				break;
			}
		}
		
		if(flag && (RealPress < pp->mh->MIN_PRESS * 10))	//面板显示的系统压力有一位小数，而最小系统压力没有小数（lzh,2018.12.4）
		{
			if((UI16)(Get10MsTick()-pp->TM_START) > pp->mh->TM_PROTECT)
			{
				SET_PART_ERR(pp, ER0_OILLEAK);
			}
		}
		else
		{
			pp->TM_START = Get10MsTick();
		}
	}	
}

void RealOilSet(PTEMPCTRLCOMM pp)
{
	PTEMPCTRL pt = &m_temp;
	int temp73main = 0,temp73 = 0,temp = 0;

    m_oil.st->DATEMP_OIL2 =  pp->st->DATEMP_REAL[8];
	if(m_oil.mh->DATEMP_OILMAX == 0)
	{
		m_oil.st->DATEMP_OIL =  0;
	}
	else
	{
		if (ChkSysCfg(CO9_DCDF) && (pt->fl_first == 2))
		{
			if (pp->st->DATEMP_REAL[pp->curchnl] <= MAXTEMP)
			{
				temp = pp->st->DATEMP_REAL[pp->curchnl];
				temp73 = pp->st->WTEMP_LM73&0x7FFF;
				temp73main = pt->st->WTEMP_LM73&0x7FFF;
				if ((pp->st->WTEMP_LM73&0x8000) > 0)
				{
					temp73 = -temp73;
				}
				if ((pt->st->WTEMP_LM73&0x8000) > 0)
				{
					temp73main = -temp73main;
				}
				
				temp = temp - (temp73 - temp73main)/10 - 3;//由于板温比环境温度高出3-5摄氏度，所有在这里做一个补偿
				if(temp < 0)
				{
					m_oil.st->DATEMP_OIL = 0;
				}
				else
				{
					m_oil.st->DATEMP_OIL = (UI16)temp;
				}
			}	
			else
			{
				m_oil.st->DATEMP_OIL =  pp->st->DATEMP_REAL[pp->curchnl];
			}
		}
		else
		{
			m_oil.st->DATEMP_OIL =  pp->st->DATEMP_REAL[pp->curchnl];
		}		
	}
}


BOOL OilCmd(UI32 cmd, UI32 para)
{
	POIL pp = &m_oil;
	UI32 ret = TRUE;
	
	switch(GET_CMD(cmd))
	{
		case CMD_DATA_RX:
			if((GET_CMD_PARA(cmd) & 0xff) == MH_ID)
			{				
				UI16 offset;
				UI16 len;
								
				/*判读是否是滑模配置参数*/
				offset = (para >> 16 & 0xffff);
				len = (para & 0xffff);

				/*油温上限修改后，初始化相应温度板块的温度配置*/
				if((len == 1) &&(GET_IND(POIL_MACHSET, DATEMP_OILMAX) == offset))
				{				
					if(g_tempcomm_num > 0)
					{
						UI16 temp_id = pp->mh->OIL_TEMPCH/TEMP_E502_CH_NUM + 1;
						if(temp_id <= g_tempcomm_num)
						{
							if(tempcoolinit[temp_id])
							{
								PTEMPCTRLCOMM ptc = g_tempcomm[temp_id-1];
				            	InitTemp(ptc);
								InitCyclicData(ptc);
								InitTempDataStu(ptc);
								InitPidParaPro(ptc);
							}
						}
					}
				}
			}
			break;
			
		default:
			ret = TRUE;
			break;
	}

	return ret;
}


BOOL InitOilPart()
{
	POIL pp = &m_oil;
	static const SUB_INIT_INFO sub_init[] = OIL_INIT_INFO;

	if(!INIT_PART(pp, OIL_ID, 1, sub_init))
		return FALSE;

	return TRUE;
}
