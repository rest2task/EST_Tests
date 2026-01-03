/******************************************************************************
  文 件 名   : coreprotect.c
  版 本 号   : V0001.0000
  作    者   : chen chaoze
  生成日期   : 2021年5月13日
  最近修改   :
  功能描述   : 中子保护功能
  函数列表   :
              
  修改历史   :
  1.日    期   : 2021年5月13日
    作    者   : chen chaoze
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
#include "machapi.h"
#include "configdf.h"
#include "clamp.h"
#include "eject.h"
#include "alarm.h"
#include "core.h"
#include "coreprotect.h"
#include "admeter.h"

void CoreStatusJudge(PCORE pp) //中子实际状态判断
{
	if(pp != NULL)
	{
		if(pp->md->FL_CORE_CTRL > 0 && pp->md->FL_CORE_CTRL < 3)
		{
			if(pp->md->FL_CORE_CTRL == 1 && pp->md->FL_CORE_TRAVEL == 0) //中子进:行程
			{
				if(CheckPI(pp->pi->I_CP_FWD) == PI_SET)
					pp->st->COREFWD_STATUS = 1;
				else
					pp->st->COREFWD_STATUS = 0;
			}
			else //中子进:时间 或者 选择绞牙
			{
				if(pp->corests == 1)
					pp->st->COREFWD_STATUS = 1;
				else
					pp->st->COREFWD_STATUS = 0;
			}
			
			if(pp->md->FL_CORE_CTRL == 1 && pp->md->FL_CORE_OUTTRAVEL == 0) //中子退:行程
			{
				if(CheckPI(pp->pi->I_CP_BWD) == PI_SET)
					pp->st->COREBWD_STATUS = 1;
				else
					pp->st->COREBWD_STATUS = 0;
			}
			else //中子退:时间 或者 选择绞牙
			{
				if(pp->corests == 2)
					pp->st->COREBWD_STATUS = 1;
				else
					pp->st->COREBWD_STATUS = 0;
			}

			if(pp->st->COREFWD_STATUS == 1 && pp->st->COREBWD_STATUS == 0)
				pp->st->CORE_STATUS = 1; //只有中子进终有效，则为进终状态
			else if(pp->st->COREFWD_STATUS == 0 && pp->st->COREBWD_STATUS == 1)
				pp->st->CORE_STATUS = 2; //只有中子退终有效，则为退终状态
			else
				pp->st->CORE_STATUS = 0; //进退终都有效或都无效时，则为中间状态
		}
		else
		{
			pp->st->CORE_STATUS = 0;
			pp->st->COREFWD_STATUS = 0;
			pp->st->COREBWD_STATUS = 0;
		}
	}
}

/********************************************************************************
*功能名称  :中子状态检测任务
*功能说明  :时时检测中子状态，当状态发生改变时上传面板。
********************************************************************************/
void CoreStatusLoop(void)  //中子状态检测任务
{
	PCORE pcore;
	UI8 num;
	
	pcore = (PCORE)GetMachineSubPartList(CORE_ID, &num);

	if(num > CLAMP_MAX_CORE)
		num = CLAMP_MAX_CORE;
	
	while(num > 0 && pcore != NULL)
	{		
		CoreStatusJudge(pcore);
		num--;
		pcore++;
	}
	
}

BOOL CoreBwdErr(PCORE pp) //中子退终信号检测
{
	if(pp == NULL)
		return FALSE;

	if(pp->st->CORE_STATUS != 2)
		return TRUE;
	else
		return FALSE;
}

BOOL CoreFwdErr(PCORE pp) //中子进终信号检测
{
	if(pp == NULL)
		return FALSE;

	if(pp->st->CORE_STATUS != 1)
		return TRUE;
	else
		return FALSE;
}

BOOL EjectBwdErr(PEJECT pp) //托退终信号检测
{
	BOOL ejerr = FALSE;
	
	if(pp == NULL)
		return FALSE;

	if(pp->mh->ADPOSI_EJTRETEFF > 99)
		pp->mh->ADPOSI_EJTRETEFF = 10;
	
	if(GetMeterLenByCh(pp->mh->EJT_METERCH) > 0)
		ejerr = (GetMeterPosByCh(pp->mh->EJT_METERCH) >  (int)(pp->pa->ADPOSI_EJTRETEND+ pp->mh->ADPOSI_EJTRETEFF));	

	if(CheckPI(pp->pi->I_EJT_BWD) == PI_RESET)
		ejerr = TRUE;

	if(CheckPI(pp->pi->I_EJT_PROT) == PI_RESET)
		ejerr = TRUE;

	if(ejerr)
	{
		return TRUE;
	}
	
	return FALSE;
}

/********************************************************************************
*功能名称  :中子进退动作及开关模动作限位器检查
*功能说明  :检测到进终信号和退终信号都有效时警报。
*检测动作  :中子和开关模相关动作全过程检测；托模进退和调模托模进前检测。
********************************************************************************/
BOOL CoreLimitSwitchErrCheck(PCORE pp)
{
	if(pp == NULL)
		return FALSE;
	
	if(pp->md->FL_CORE_CTRL > 0 && pp->md->FL_CORE_CTRL < 3)
	{
		if(pp->st->COREFWD_STATUS == 1 && pp->st->COREBWD_STATUS == 1)
		{
			SET_PART_ERR(pp, ER0_SITE);  //中子A~F状态异常
			return TRUE;
		}
	}
	
	return FALSE;
}

/********************************************************************************
*功能名称  :托模进前检测中子状态
*检测逻辑  :检测到"中子退终"无信号时警报。
*特殊说明  :根据正常全自动逻辑，使用特殊中子1/3情况下做托模进动作时中子A应处于进终状态。
*检测动作  :托模进。
********************************************************************************/
BOOL EjectFwdCoreStateErrCheck(PCORE pp)
{
	if(pp == NULL)
		return FALSE;

	if(pp->md->FL_CORE_CTRL > 0 && pp->md->FL_CORE_CTRL < 3)
	{
	    if(ChkSysCfg(CO4_MEJT) && pp->md->FL_SPECIALCODE == 1)
            return FALSE;
        
		/*按照动作逻辑，特殊中子1、3检测中子进终*/
		if(pp->md->FL_SPECIALCODE == 1 || pp->md->FL_SPECIALCODE == 3)
		{
			if(pp->st->CORE_STATUS != 1)
			{
				SET_PART_ERR(pp, ER0_EJTF_CRF);  //托模进时中子A~F进未到定位
				return TRUE;
			}
		}
		else
		{
			if(pp->st->CORE_STATUS != 2)
			{
				SET_PART_ERR(pp, ER0_EJTF_CRB);  //托模进时中子A~F退未到定位
				return TRUE;
			}
		}
	}
	
	return FALSE;
}

/********************************************************************************
*功能名称  :托模退前检测中子状态
*检测逻辑  :检测到"中子退终"无信号时警报。
*特殊说明  :根据正常全自动逻辑，使用特殊中子1/3情况下做托模退动作时中子A应处于进终状态。
*检测动作  :托模退。
********************************************************************************/
BOOL EjectBwdCoreStateErrCheck(PCORE pp)
{
	if(pp == NULL)
		return FALSE;

	if(pp->md->FL_CORE_CTRL > 0 && pp->md->FL_CORE_CTRL < 3)
	{
	    if(ChkSysCfg(CO4_MEJT) && pp->md->FL_SPECIALCODE == 1)
            return FALSE;
            
		/*按照动作逻辑，特殊中子1、3检测中子进终*/
		if(pp->md->FL_SPECIALCODE == 1 || pp->md->FL_SPECIALCODE == 3)
		{
			if(pp->st->CORE_STATUS != 1)
			{
				SET_PART_ERR(pp, ER0_EJTB_CRF);  //托模退时中子A~F进未到定位
				return TRUE;
			}
		}
		else
		{
			if(pp->st->CORE_STATUS != 2)
			{
				SET_PART_ERR(pp, ER0_EJTB_CRB);  //托模退时中子A~F退未到定位
				return TRUE;
			}
		}
	}
	
	return FALSE;
}

/********************************************************************************
*功能名称  :中子动作前检测托退终
*检测逻辑  :检测到"托模退终"无信号时警报。
*特殊说明  :使用特殊中子1时不检测。
*检测动作  :中子进、中子退。
********************************************************************************/
BOOL CoreFBwdEjectStateErrCheck(PCORE pp, BOOL isfwd)
{
	int i;
	PCLAMP pclmp = (PCLAMP)GetPartByID(MK_PART(CLAMP_ID, 1));
	
	if(pp == NULL || pclmp == NULL)
		return FALSE;

	if(pp->md->FL_SPECIALCODE == 1)
		return FALSE;

	if(pp->md->FL_CORE_CTRL > 0 && pp->md->FL_CORE_CTRL < 3)
	{
		for(i=0;i<CLAMP_MAX_EJECT;i++)
		{
			if(pclmp->pejt[i] != NULL)
			{
				if(EjectBwdErr(pclmp->pejt[i]))
				{
				    if(isfwd)
					    SET_PART_ERR(pp, ER0_CRF_EJTB);  //中子A~F进时托模退未到定位
					else
                        SET_PART_ERR(pp, ER0_CRB_EJTB);  //中子A~F退时托模退未到定位
                        
					return TRUE;
				}
			}
		}
	}
	
	return FALSE;
}

/********************************************************************************
*功能名称  :中子进位置有效区检测
*检测逻辑  :(手动/自动)中子进动作前检测模板位置不在有效区范围内时警报。
*有效区说明:1、中子进-关模前  [开模五段终止位置-中子位置有效区，开模五段终止位置+中子位置有效区];
            2、中子进-关模中  [中子进动作位置-中子位置有效区，中子进动作位置+中子位置有效区];
            3、中子进-关模后  [0，关模低压终止位置];
*特殊说明  :
********************************************************************************/
BOOL CoreFwdValidPositionErrCheck(PCORE pp)
{
	PADMETER padm = (PADMETER)GetPartByID(MK_PART(ADMETER_ID, 1));
	PCLAMP pclmp = (PCLAMP)GetPartByID(MK_PART(CLAMP_ID, 1));
	UI16 validPosMax,validPosMin = 0; //临时变量:有效区上限、有效区下限
	
	if(pp == NULL || pclmp == NULL || padm == NULL)
		return FALSE;

	if(pp->mh->ADPOSI_EFFCOREINPLUS == 0 && pp->mh->ADPOSI_EFFCOREINMINUS == 0 && pp->mh->ADPOSI_EFFCOREOUTPLUS == 0 && pp->mh->ADPOSI_EFFCOREOUTMINUS == 0) //位置有效区为0不检测
		return FALSE;
	
	if(pp->md->FL_CORE_CTRL > 0 && pp->md->FL_CORE_CTRL < 3)
	{
		if(pp->md->ADPOSI_CORE_FWD == 0) //关始有效区:[开模五段终止位置-中子位置有效区，开模五段终止位置+中子位置有效区]
		{
			validPosMax = pclmp->pa->ADPOSI_OPENENDEND + pp->mh->ADPOSI_EFFCOREINPLUS;
			validPosMin = (pclmp->pa->ADPOSI_OPENENDEND >= pp->mh->ADPOSI_EFFCOREINMINUS) ? (pclmp->pa->ADPOSI_OPENENDEND - pp->mh->ADPOSI_EFFCOREINMINUS) : 0;	
		}
		else if(pp->md->ADPOSI_CORE_FWD == 1) //关中有效区:[中子进动作位置-中子位置有效区，中子进动作位置+中子位置有效区]
		{
			validPosMax = pp->pa->ADPOSI_COREINACT + pp->mh->ADPOSI_EFFCOREINPLUS;
			validPosMin = (pp->pa->ADPOSI_COREINACT >= pp->mh->ADPOSI_EFFCOREINMINUS) ? (pp->pa->ADPOSI_COREINACT - pp->mh->ADPOSI_EFFCOREINMINUS) : 0;		
		}
		else if(pp->md->ADPOSI_CORE_FWD == 2) //关终有效区:[0，关模低压终止位置]
		{
			validPosMax = pclmp->pa->ADPOSI_CLSPROTECTEND;
			//不检测最小值，避免关模电子尺到位时是负值
		}
		
		if(GetMeterPosByCh(pclmp->mh->CLMP_METERCH) > validPosMax || 
            (pp->md->ADPOSI_CORE_FWD != 2 && GetMeterPosByCh(pclmp->mh->CLMP_METERCH) < validPosMin))
		{
			SET_PART_ERR(pp, ER0_CRF_POS);
			return TRUE; 
		}
	}

	return FALSE;
}

/********************************************************************************
*功能名称  :中子退位置有效区检测
*检测逻辑  :(手动/自动)中子退动作前检测模板位置不在有效区范围内时警报。
*有效区说明:1、中子退-开模前  [0，关模低压终止位置];
            2、中子退-开模中  [中子退动作位置-中子位置有效区，中子退动作位置+中子位置有效区];
            3、中子退-开模后  [开模五段终止位置-中子位置有效区，开模五段终止位置+中子位置有效区]
*特殊说明  :
********************************************************************************/
BOOL CoreBwdValidPositionErrCheck(PCORE pp)
{
	PADMETER padm = (PADMETER)GetPartByID(MK_PART(ADMETER_ID, 1));
	PCLAMP pclmp = (PCLAMP)GetPartByID(MK_PART(CLAMP_ID, 1));
	UI16 validPosMax,validPosMin = 0; //临时变量:有效区上限、有效区下限
	
	if(pp == NULL || pclmp == NULL || padm == NULL)
		return FALSE;

	if(pp->mh->ADPOSI_EFFCOREINPLUS == 0 && pp->mh->ADPOSI_EFFCOREINMINUS == 0 && pp->mh->ADPOSI_EFFCOREOUTPLUS == 0 && pp->mh->ADPOSI_EFFCOREOUTMINUS == 0) //位置有效区为0不检测
		return FALSE;
	
	if(pp->md->FL_CORE_CTRL > 0 && pp->md->FL_CORE_CTRL < 3)
	{
		if(pp->md->ADPOSI_CORE_BWD == 2) //开始有效区:[0，关模低压终止位置]
		{
			validPosMax = pclmp->pa->ADPOSI_CLSPROTECTEND;
			//不检测最小值，避免关模电子尺到位时是负值
		}
		else if(pp->md->ADPOSI_CORE_BWD == 1) //开中有效区:[中子退动作位置-中子位置有效区，中子退动作位置+中子位置有效区]
		{
			validPosMax = pp->pa->ADPOSI_COREOUTACT + pp->mh->ADPOSI_EFFCOREOUTPLUS;
			validPosMin = (pp->pa->ADPOSI_COREOUTACT >= pp->mh->ADPOSI_EFFCOREOUTMINUS) ? (pp->pa->ADPOSI_COREOUTACT - pp->mh->ADPOSI_EFFCOREOUTMINUS) : 0;		
		}
		else if(pp->md->ADPOSI_CORE_BWD == 0) //开终有效区:[开模五段终止位置-中子位置有效区，开模五段终止位置+中子位置有效区]
		{
			validPosMax = pclmp->pa->ADPOSI_OPENENDEND + pp->mh->ADPOSI_EFFCOREOUTPLUS;
			validPosMin = (pclmp->pa->ADPOSI_OPENENDEND >= pp->mh->ADPOSI_EFFCOREOUTMINUS) ? (pclmp->pa->ADPOSI_OPENENDEND - pp->mh->ADPOSI_EFFCOREOUTMINUS) : 0;	
		}
		
		if(GetMeterPosByCh(pclmp->mh->CLMP_METERCH) > validPosMax || 
            (pp->md->ADPOSI_CORE_BWD != 2 && GetMeterPosByCh(pclmp->mh->CLMP_METERCH) < validPosMin))
		{
			SET_PART_ERR(pp, ER0_CRB_POS);
			return TRUE; 
		}
	}

	return FALSE;
}

/********************************************************************************
*功能名称    :关模动作过程中检测中子状态
*检测逻辑    :(手动/自动)关模动作过程中时时检测中子状态不正确时警报。
*中子状态说明:1、中子进-关模前  关模开始运动后应为"中子进终";
              2、中子进-关模中  电子尺位置>中子进动作位置时应为"中子退终";
                                电子尺位置<中子进动作位置-中子位置有效区时应为"中子进终";
                                以上两个范围之间时插中子任务，不检测;
              3、中子进-关模后  关模动作结束前应为"中子退终"。
*特殊说明    :1、联动中子小于关模联动位置时不检测;
              2、特殊中子5/7逻辑特殊，中子进方式为关模前，中子退方式为开模后。
                 理论上开关模全程状态都应为进终，但是实际上开关模全程状态都为退终，需要特殊处理。
********************************************************************************/
BOOL ClampClsCoreStateErrCheck(PCORE pp, UI16 clmp_step)
{
	PCLAMP pclmp = (PCLAMP)GetPartByID(MK_PART(CLAMP_ID, 1));
	int clmppos = GetMeterPosByCh(pclmp->mh->CLMP_METERCH);
	UI16 effectivePos; //中子有效区边界，临时变量
	int NUM;
	
	if(pp == NULL || pclmp == NULL)
		return FALSE;

	NUM = GetJointActSel2();
	if(NUM > 0 && NUM < CLAMP_MAX_CORE+1)
	{
	}	
	else
		NUM = 0;
	
	if(pp->md->FL_CORE_CTRL > 0 && pp->md->FL_CORE_CTRL < 3)
	{
		if(pp->md->FL_SPECIALCODE == 5 || pp->md->FL_SPECIALCODE == 7) //特殊中子5、7特殊处理
		{
			if(CoreBwdErr(pp))
			{
				SET_PART_ERR(pp, ER0_CLS_CRB);  //关模时中子A~F退未到定位
				return TRUE;
			}
		}
		else 
		{
			if(pp->md->ADPOSI_CORE_FWD == 0) //关模前
			{
				if(clmp_step >= STAGE_1_STEP) //初始步不检测，因为关模前中子还未动作
				{
					if(CoreFwdErr(pp))
					{
						SET_PART_ERR(pp, ER0_CLS_CRF);  //关模时中子A~F进未到定位
						return TRUE;
					}
				}
			}
			else if(pp->md->ADPOSI_CORE_FWD == 1) //关模中
			{
				if(clmppos > pp->pa->ADPOSI_COREINACT)
				{
					if(CoreBwdErr(pp))
					{
						SET_PART_ERR(pp, ER0_CLS_CRB);  //关模时中子A~F退未到定位
						return TRUE;
					}
				}
				else if(NUM != GET_PART_SN(pp->part.id)) //联动中子不检测后半段
				{
					if(pp->mh->ADPOSI_EFFCOREOUTMINUS > 0) //中子位置有效区为0时，小于动作位置都可以做中子进动作，因此不检测
					{
						effectivePos = (pp->pa->ADPOSI_COREINACT >= pp->mh->ADPOSI_EFFCOREOUTMINUS) ? (pp->pa->ADPOSI_COREINACT - pp->mh->ADPOSI_EFFCOREOUTMINUS) : 0;
						if(clmppos < effectivePos)
						{
							if(CoreFwdErr(pp))
							{
								SET_PART_ERR(pp, ER0_CLS_CRF);  //关模时中子A~F进未到定位
								return TRUE;
							}
						}
					}
				}
			}
			else if(pp->md->ADPOSI_CORE_FWD == 2) //关模后
			{
				if(((clmppos > pclmp->pa->ADPOSI_CLSPROTECTEND) || !TestPI(pclmp->pi->I_CLS_END))) //关模未结束时检测
				{
					if(CoreBwdErr(pp))
					{
						SET_PART_ERR(pp, ER0_CLS_CRB);  //关模时中子A~F退未到定位
						return TRUE;
					}
				}
			}
		}
	}
	
	return FALSE;
}

/********************************************************************************
*功能名称	 :开模动作过程中检测中子状态
*检测逻辑	 :(手动/自动)开模动作过程中时时检测中子状态不正确时警报。
*中子状态说明:1、中子退-开模前	开模开始运动后应为"中子退终";
			  2、中子退-开模中	电子尺位置<中子退动作位置时应为"中子进终";
								电子尺位置>中子进动作位置+中子位置有效区时应为"中子退终";
								以上两个范围之间时插中子任务，不检测;
			  3、中子退-开模后	开模动作结束前应为"中子进终"。
*特殊说明	 :1、联动中子大于开模联动位置时不检测;
			  2、特殊中子5/7逻辑特殊，中子进方式为关模前，中子退方式为开模后。
                 理论上开关模全程状态都应为进终，但是实际上开关模全程状态都为退终，需要特殊处理。
********************************************************************************/
BOOL ClampOpenCoreStateErrCheck(PCORE pp, UI16 clmp_step)
{
	PCLAMP pclmp = (PCLAMP)GetPartByID(MK_PART(CLAMP_ID, 1));
	int clmppos = GetMeterPosByCh(pclmp->mh->CLMP_METERCH);
	int sel;
	
	if(pp == NULL || pclmp == NULL)
		return FALSE;
	
	sel = GetJointActSel();
	if(sel > 1 && sel < CLAMP_MAX_CORE+2)
		sel = sel - 1;
	else
		sel = 0;

	if(pp->md->FL_CORE_CTRL > 0 && pp->md->FL_CORE_CTRL < 3)
	{
		if(pp->md->FL_SPECIALCODE == 5 || pp->md->FL_SPECIALCODE == 7) //特殊中子5、7特殊处理
		{
			if(CoreBwdErr(pp))
			{
				SET_PART_ERR(pp, ER0_OPN_CRB); //开模时中子A~F退未到定位
				return TRUE;
			}
		}
		else 
		{
			if(pp->md->ADPOSI_CORE_BWD == 2) //开模前
			{
				if(clmp_step >= STAGE_1_STEP)
				{
					if(CoreBwdErr(pp))
					{
						SET_PART_ERR(pp, ER0_OPN_CRB); //开模时中子A~F退未到定位
						return TRUE;
					}
				}
			}
			else if(pp->md->ADPOSI_CORE_BWD == 1) //开模中
			{			
				if(clmppos < pp->pa->ADPOSI_COREOUTACT)
				{
					if(CoreFwdErr(pp))
					{
						SET_PART_ERR(pp, ER0_OPN_CRF);  //开模时中子A~F进未到定位
						return TRUE;
					}
				}
				else if(sel != GET_PART_SN(pp->part.id))  //联动中子后半段不检测
				{
					if(pp->mh->ADPOSI_EFFCOREOUTPLUS > 0) //中子位置有效区为0时，大于动作位置都可以做中子退动作，因此不检测
					{
						if(clmppos > (pp->pa->ADPOSI_COREOUTACT + pp->mh->ADPOSI_EFFCOREOUTPLUS))
						{
							if(CoreBwdErr(pp))
							{
								SET_PART_ERR(pp, ER0_OPN_CRB); //开模时中子A~F退未到定位
								return TRUE;
							}
						}
					}
				}
			}
			else if(pp->md->ADPOSI_CORE_BWD == 0) //开模后
			{
				if((clmppos < pclmp->pa->ADPOSI_OPENENDEND))
				{
					if(CoreFwdErr(pp))
					{
						SET_PART_ERR(pp, ER0_OPN_CRF);  //开模时中子A~F进未到定位
						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}

/********************************************************************************
*功能名称  :中子进顺序检查
*检测逻辑  :(手动/自动)中子进前检测其它中子状态不正确时警报。
*状态说明  :1、当前中子进实际顺序比中子x小时，即当前中子先动作，则中子x应处于"中子退终"状态
            2、当前中子进实际顺序比中子x大时，即当前中子后动作，则中子x应处于"中子进终"状态
*特殊说明  :1、特殊中子不检查
            2、联动中子不检查
********************************************************************************/
BOOL CoreInSequenceErrCheck(PCORE pp)
{
	int NUM;
	PCORE pcore;
	UI8 num;
	
	if(pp == NULL)
		return FALSE;

	NUM = GetJointActSel2();
	if(NUM > 0 && NUM < CLAMP_MAX_CORE+1)
	{
	}	
	else
		NUM = 0;

	if(pp->md->FL_SPECIALCODE > 0 || NUM == GET_PART_SN(pp->part.id))  //特殊中子、联动中子不检测
		return FALSE;
	
	if(pp->md->FL_CORE_CTRL > 0 && pp->md->FL_CORE_CTRL < 3)
	{
		pcore = (PCORE)GetMachineSubPartList(CORE_ID, &num);
		while(num > 0 && pcore != NULL)
		{
			if(pcore->md->FL_SPECIALCODE > 0 || NUM == GET_PART_SN(pcore->part.id))  //特殊中子、联动中子不检测
			{
			}
			else if(GET_PART_SN(pp->part.id) != GET_PART_SN(pcore->part.id))
			{
				if(pcore->md->FL_CORE_CTRL > 0 && pcore->md->FL_CORE_CTRL < 3)
				{
					if(pp->CoreFwdActualSequence < pcore->CoreFwdActualSequence) //实际顺序数值小，表示先做，检测其它中子退终信号
					{
						if(CoreBwdErr(pcore))
						{
							SET_PART_ERR(pcore, ER0_SEQ_CRB);  //顺序检查:中子A~F退未到定位
							return TRUE;
						}
					}
					else if(pp->CoreFwdActualSequence > pcore->CoreFwdActualSequence) //实际顺序数值大，表示后做，检测其它中子进终信号
					{
						if(CoreFwdErr(pcore))
						{
							SET_PART_ERR(pcore, ER0_SEQ_CRF);  //顺序检查:中子A~F进未到定位
							return TRUE;
						}
					}
				}
			}
		
			num--;
			pcore++;
		}
	}
	
	return FALSE;
}

/********************************************************************************
*功能名称  :中子退顺序检查
*检测逻辑  :(手动/自动)中子退前检测其它中子状态不正确时警报。
*状态说明  :1、当前中子退实际顺序比中子x小时，即当前中子先动作，则中子x应处于"中子进终"状态
            2、当前中子退实际顺序比中子x大时，即当前中子后动作，则中子x应处于"中子退终"状态
*特殊说明  :1、特殊中子不检查
            2、联动中子不检查
********************************************************************************/
BOOL CoreOutSequenceErrCheck(PCORE pp)
{
	int sel;
	PCORE pcore;
	UI8 num;
	
	if(pp == NULL)
		return FALSE;
	
	sel = GetJointActSel();
	if(sel > 1 && sel < CLAMP_MAX_CORE+2)
		sel = sel - 1;
	else
		sel = 0;

	if(pp->md->FL_SPECIALCODE > 0 || sel == GET_PART_SN(pp->part.id))  //特殊中子、联动中子不检测
		return FALSE;
	
	if(pp->md->FL_CORE_CTRL > 0 && pp->md->FL_CORE_CTRL < 3)
	{
		pcore = (PCORE)GetMachineSubPartList(CORE_ID, &num);
		while(num > 0 && pcore != NULL)
		{
			if(pcore->md->FL_SPECIALCODE > 0 || sel == GET_PART_SN(pcore->part.id))  //特殊中子、联动中子不检测
			{
			}
			else if(GET_PART_SN(pp->part.id) != GET_PART_SN(pcore->part.id))
			{
				if(pcore->md->FL_CORE_CTRL > 0 && pcore->md->FL_CORE_CTRL < 3)
				{
					if(pp->CoreBwdActualSequence < pcore->CoreBwdActualSequence) //实际顺序数值小，表示先做，检测其它中子进终信号
					{
						if(CoreFwdErr(pcore))
						{
							SET_PART_ERR(pcore, ER0_SEQ_CRF);  //顺序检查:中子A~F进未到定位
							return TRUE;
						}
					}
					else if(pp->CoreBwdActualSequence > pcore->CoreBwdActualSequence) //实际顺序数值大，表示后做，检测其它中子退终信号
					{
						if(CoreBwdErr(pcore))
						{
							SET_PART_ERR(pcore, ER0_SEQ_CRB);  //顺序检查:中子A~F退未到定位
							return TRUE;
						}
					}
				}
			}
		
			num--;
			pcore++;
		}
	}
	
	return FALSE;
}

