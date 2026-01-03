/******************************************************************************
  文 件 名   : robot.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 机械手及机?凳纸涌诤?
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
#include "robot.h"
#include "clamp.h"
#include "systempro.h"
#include "safedoor.h"
#include "core.h"


void RobotSafeChk(PROBOT pp)
{
	UI16 pos = 0;
	if (pp != NULL && !ChkSysCfg(XF4_OPNRL))
	{
		if (CheckPO(pp->po->O_ROBOT) == PO_SET || CheckPO(pp->po->O_R_OPNM) == PO_SET)
		{
		    if (pp->pclmp->md->FL_CLSOPEN_MODE == 2)
				pos = pp->pclmp->pa->ADPOS_OPENSPD3;
			else
				pos = pp->pclmp->pa->ADPOSI_OPENENDEND;

            if(pp->pclmp->mh->FL_ADPOSROBOT == 1 && pp->pclmp->md->POS_ROBOT < pos)
                pos = pp->pclmp->md->POS_ROBOT; //途中机械手使用且途中位置小于开模终止位置时，以途中位置做保护 2023-7-30 ccz

			if (MoveLocationMeterControlProc(pos-500, GetMeterPosByCh(pp->pclmp->mh->CLMP_METERCH)))
			{
				SetPOBit(pp->po->O_ROBOT, PC_OFF);
				SetPOBit(pp->po->O_R_OPNM, PC_OFF);
				SetPOBit(pp->po->O_R_OPMD, PC_OFF);
				
				SET_PART_ERR(pp->pclmp, ER1_OPNMPOSERR);
			}
		}
	}
}

void RobotAutoON(PROBOT pp)
{
	if(pp != NULL)
	{
		if(pp->md->FL_ROBOTCTRL >= 2)
			SetPOBit(pp->po->O_R_AUTO, PC_ON);
	}
}

void RobotClampCloseStart(PROBOT pp)
{
	if(pp != NULL)
	{
		SetPOBit(pp->po->O_ROBOT, PC_OFF);
		SetPOBit(pp->po->O_R_OPNM, PC_OFF);
		SetPOBit(pp->po->O_R_OPMD, PC_OFF);
	}
}

void RobotClampCloseEnd(PROBOT pp, BOOL bset)
{
	if(pp != NULL)
	{
		if(pp->md->FL_ROBOTCTRL >= 2)
		{
			if(bset)
				SetPOBit(pp->po->O_R_CLSM, PC_ON);
			else
				SetPOBit(pp->po->O_R_CLSM, PC_OFF);
		}
	}
}

void RobotClampOpenEnd(PROBOT pp)
{
	if(pp != NULL)
	{
		if(pp->md->FL_ROBOTCTRL != 0)
		{
			if(ChkSysCfg(CO0_KROROBOT))      //韩国机械手必须合模完成一次开模才会输出机械手  2015-03-24 LY
			{
				if(pp->pclmp->clscmpflag == 1)
				{
					pp->pclmp->clscmpflag = 0;
					if(ChkOperMode(OM_MANUAL) && pp->md->FL_MANUALOPN != 1)
						return;				
				}
				else
					return;	
			}
			else
			{
				/*手动模式下，只有FL_MANUALOPN，手动开模使能才能输出*/
				if(ChkOperMode(OM_MANUAL) && pp->md->FL_MANUALOPN != 1)
					return;
			}		
			SetPOBit(pp->po->O_ROBOT, PC_ON);
			SetPOBit(pp->po->O_R_OPNM, PC_ON);
			
			if(!ChkSysCfg(CO0_OFFM))		//机械手信号翻转检测				ws-2018-12-10
			{
				if(ChkOperMode(OM_AUTO))	//自动时，检测机械手的0-1变化
					pp->flag_Robot_Turn = 1;
				else
					pp->flag_Robot_Turn = 0;
			}
		}
		else
		{
			pp->flag_Robot_Turn = 0;
		}
	}
	else
	{
		pp->flag_Robot_Turn = 0;
	}
}
 
 void RobotClampOpenMid(PROBOT pp)	   //开模中途，当有开模中途输出点时使用  20170711
 {
	  if(pp != NULL)
	  {
		  if(pp->md->FL_ROBOTCTRL != 0)
		  {
			  if(ChkSysCfg(CO0_KROROBOT))	   //韩国机械手必须合模完成一次开模才会输出机械手  2015-03-24 LY
			  {
				  if(pp->pclmp->clscmpflagmid == 1)
				  {
					  pp->pclmp->clscmpflagmid = 0;
					  if(ChkOperMode(OM_MANUAL) && pp->md->FL_MANUALOPN != 1)
						  return;			  
				  }
				  else
					  return; 
			  }
			  else
			  {
				  /*手动模式下，只有FL_MANUALOPN，手动开模使能才能输出*/
				  if(ChkOperMode(OM_MANUAL) && pp->md->FL_MANUALOPN != 1)
					  return;
			  } 	  
			  SetPOBit(pp->po->O_R_OPMD, PC_ON);   //新增'开模中途'输出点  20170710
		  }
	  }
  }
  

 //机械手射出
 void RobotInjectOnOff(PROBOT pp, BOOL bon)
{
	if(pp != NULL)
	{
		if(pp->md->FL_ROBOTCTRL >= 2 && bon)
		{
			SetPOBit(pp->po->O_R_INJE, PC_ON);
		}
		else if(!bon)
		{
			SetPOBit(pp->po->O_R_INJE, PC_OFF);
		}
	}
}

 void RobotInjectErr(PROBOT pp)
{
	if(pp != NULL)
		SetPOBit(pp->po->O_R_INJERR, PC_ON);
}

BOOL RobotConfigErrChk(PROBOT pp)		//机械手点位配置检查  
{
	UI16 end1 = FALSE,end2 = FALSE;
	
	if(pp != NULL)
	{
		if (pp->md->FL_ROBOTCTRL == 1)
		{
			if (CheckPI(pp->pi->I_R_MANUAL) == PI_RESET)
			{
				end1 = CheckPI(pp->pi->I_R_CLSM) == PI_NOUSED;
			}
			
			end2 = CheckPI(pp->pi->I_ROBOT) == PI_NOUSED;
		}
		else if ((pp->md->FL_ROBOTCTRL > 1) || (CheckPI(pp->pi->I_R_MANUAL)==PI_RESET))
		{
			end1 = CheckPI(pp->pi->I_R_CLSM) == PI_NOUSED;
		}
	}

    if(end1 | end2)
    {
        if(end1)
            SET_PART_ERR(pp, ER1_ROBCLSM_CFG);
        if(end2)
            SET_PART_ERR(pp, ER1_ROB_CFG);
        return TRUE;
    }
    else
	    return FALSE;
}

BOOL RobotClampClsErrChk(PROBOT pp)		//等待机械手，关摸前判断   20170711   
{
	UI16 end1 = FALSE,end2 = FALSE,end3 = FALSE;
	
	if(pp != NULL)
	{
		if (pp->md->FL_ROBOTCTRL == 1)
		{
			if (CheckPI(pp->pi->I_R_MANUAL) == PI_RESET)
			{
				end1 = PartErrSet(pp, ER0_ROB2_CLSM, CheckPI(pp->pi->I_R_CLSM) == PI_RESET);
                end3 = PartErrSet(pp, ER0_ROB2_MAFR, CheckPI(pp->pi->I_R_MAFR) == PI_RESET);
			}
			
			end2 = PartErrSet(pp, ER0_ROB2, CheckPI(pp->pi->I_ROBOT) == PI_RESET);

			return (end1 | end2 | end3);
		}
		else if ((pp->md->FL_ROBOTCTRL > 1) || (CheckPI(pp->pi->I_R_MANUAL)==PI_RESET))
		{
			end1 = PartErrSet(pp, ER0_ROB2_CLSM, CheckPI(pp->pi->I_R_CLSM) == PI_RESET);
            end3 = PartErrSet(pp, ER0_ROB2_MAFR, CheckPI(pp->pi->I_R_MAFR) == PI_RESET);

            return (end1 | end3);
		}
	}

	return FALSE;
}

BOOL RobotRecycleErrChk(PROBOT pp)
{
	if(pp != NULL)
	{
		if ((pp->md->FL_ROBOTCTRL >= 2 || (CheckPI(pp->pi->I_R_MANUAL)==PI_RESET))&&(!(ChkRunFlag(INIF_AUTO1))))
		{
			return PartErrSet(pp, ER0_ROB2_RCYC, CheckPI(pp->pi->I_R_RCYC) == PI_RESET);
		}
	}

	return FALSE;
}

BOOL RobotClampPreErrChk(PROBOT pp)		//开模前，等待"开模安全"和"模区安全"  20170710
{
	UI16 end1 = FALSE,end2 = FALSE;		

	if(pp != NULL)
	{
		if((pp->md->FL_ROBOTCTRL >= 2) || (CheckPI(pp->pi->I_R_MANUAL)==PI_RESET))
		{
			end1 = PartErrSet(pp, ER0_ROB2_OPNM, CheckPI(pp->pi->I_R_OPNM) == PI_RESET);
			end2 = PartErrSet(pp, ER0_ROB2_MAFR, CheckPI(pp->pi->I_R_MAFR) == PI_RESET);

			return (end1 | end2);
		}		
	}

	return FALSE;
}


BOOL RobotClampOpnErrChk(PROBOT pp)  //开模过程中时时检测  20170710
{
	UI16 end1 = FALSE,end2 = FALSE;	
	
	if(pp != NULL)
	{
		if((pp->md->FL_ROBOTCTRL >= 2) || (CheckPI(pp->pi->I_R_MANUAL)==PI_RESET))
		{
			end1 = PartErrSet(pp, ER0_ROBO_OPNM, CheckPI(pp->pi->I_R_OPNM) == PI_RESET);
			end2 = PartErrSet(pp, ER0_ROBO_MAFR, CheckPI(pp->pi->I_R_MAFR) == PI_RESET);

			return (end1 | end2);
		}		
	}

  	return FALSE;    
}

BOOL RobotClampClsProtectErrChk(PROBOT pp)  //机械手失败，关摸过程中时时判断  20200922
{
	UI16 end1 = FALSE,end2 = FALSE,end3 = FALSE;	
	
  	if(pp != NULL)
  	{  		
		if(pp->md->FL_ROBOTCTRL == 1)
		{
			if (CheckPI(pp->pi->I_R_MANUAL) == PI_RESET)
			{				
				end1 = PartErrSet(pp, ER0_ROBO_CLSM, CheckPI(pp->pi->I_R_CLSM) == PI_RESET);
				end2 = PartErrSet(pp, ER0_ROBO_MAFR, CheckPI(pp->pi->I_R_MAFR) == PI_RESET);
			}
			end3 = PartErrSet(pp, ER0_ROBO, CheckPI(pp->pi->I_ROBOT) == PI_RESET);	

			return (end1 | end2 | end3); 
		}

		if ((pp->md->FL_ROBOTCTRL > 1) || (CheckPI(pp->pi->I_R_MANUAL)==PI_RESET))
		{
			end1 = PartErrSet(pp, ER0_ROBO_CLSM, CheckPI(pp->pi->I_R_CLSM) == PI_RESET);
			end2 = PartErrSet(pp, ER0_ROBO_MAFR, CheckPI(pp->pi->I_R_MAFR) == PI_RESET);

			return (end1 | end2);
		}
  	}

  	return FALSE;
}

BOOL RobotPIErrChk(PROBOT pp, UI8 pi_name,UI32 err_msk)
{
	if(pp != NULL)
	{
		return PartErrSet(pp, err_msk, CheckPI(pi_name)==PI_RESET);
	}

	return FALSE;
}

BOOL RobotPIErrChkCore(PROBOT pp, UI8 pi_name,UI32 err_msk,int i)   //检测中子引起的警报     20170713
{
	if(pp != NULL)
	{
		return PartErrSet(pp->pclmp->pcore[i-1], err_msk, CheckPI(pi_name)==PI_RESET);
	}

	return FALSE;
}


BOOL RobotEjectFwdErrChk(PROBOT pp)  //等待机械手   拖摸进前判断  20170711
{
	if(pp != NULL)
	{
		if((pp->md->FL_ROBOTCTRL >=2) || (CheckPI(pp->pi->I_R_MANUAL)== PI_RESET))			
			return RobotPIErrChk(pp, pp->pi->I_R_EJTF,ER0_ROB2_EJTF);
	}

	return FALSE;
}

BOOL RobotEjectFwdChk(PROBOT pp)  //机械手失败   拖摸进过程中判断  20170711
{
	if(pp != NULL)
	{
		if((pp->md->FL_ROBOTCTRL >=2) || (CheckPI(pp->pi->I_R_MANUAL)== PI_RESET))			
			return RobotPIErrChk(pp, pp->pi->I_R_EJTF,ER0_ROBO_EJTF);
	}

	return FALSE;
}


BOOL RobotEjectBwdErrChk(PROBOT pp)  //等待机械手 拖摸退前判断   20170711
{
	if(pp != NULL)
	{
		if((pp->md->FL_ROBOTCTRL > 1) || (CheckPI(pp->pi->I_R_MANUAL) == PI_RESET))			
			return RobotPIErrChk(pp, pp->pi->I_R_EJTB,ER0_ROB2_EJTB);
	}

	return FALSE;
}

BOOL RobotEjectBwdChk(PROBOT pp)  //机械手失败 拖摸退过程中判断   20170711
{
	if(pp != NULL)
	{
		if((pp->md->FL_ROBOTCTRL > 1) || (CheckPI(pp->pi->I_R_MANUAL) == PI_RESET))			
			return RobotPIErrChk(pp, pp->pi->I_R_EJTB,ER0_ROBO_EJTB);
	}
	return FALSE;
}


BOOL RobotCoreFwdErrChk(PROBOT pp,int i)  //等待机械手   中子进前判断  20170711
{
	if(pp != NULL)
	{
		if(pp->md->FL_ROBOTCTRL > 1  || (CheckPI(pp->pi->I_R_MANUAL) == PI_RESET))			
		{
			if(i==pp->md->FL_CORESEC1)
				return RobotPIErrChkCore(pp, pp->pi->I_R_CPRF,ER0_ROB2_CPRF,i);
			else if((i==pp->md->FL_CORESEC2)&&((pp->md->FL_ROBOTCTRL==3) || (CheckPI(pp->pi->I_R_MANUAL) == PI_RESET)))
				return RobotPIErrChkCore(pp, pp->pi->I_R_C2PRF,ER0_ROB2_CPRF,i);
		}
	}
	
	return FALSE;
}

BOOL RobotCoreFwdChk(PROBOT pp,int i)  //机械手失败   中子进过程判断   20170711 
{
	if(pp != NULL)
	{
		if(pp->md->FL_ROBOTCTRL >=2  || (CheckPI(pp->pi->I_R_MANUAL) == PI_RESET))			
		{
			if(i==pp->md->FL_CORESEC1)
				return RobotPIErrChkCore(pp, pp->pi->I_R_CPRF,ER0_ROBO_CPRF,i);
			else if((i==pp->md->FL_CORESEC2)&&((pp->md->FL_ROBOTCTRL==3) || (CheckPI(pp->pi->I_R_MANUAL) == PI_RESET)))
				return RobotPIErrChkCore(pp, pp->pi->I_R_C2PRF,ER0_ROBO_CPRF,i);
		}
	}
	return FALSE;
}


BOOL RobotCoreBwdErrChk(PROBOT pp,int i)  //等待机械手  中子退前判断20170711
{
	if(pp != NULL)
	{
		if(pp->md->FL_ROBOTCTRL >=2  || (CheckPI(pp->pi->I_R_MANUAL)== PI_RESET))	
		{		
			if(i==pp->md->FL_CORESEC1)
			 	return RobotPIErrChkCore(pp, pp->pi->I_R_CPRB,ER0_ROB2_CPRB,i);
			else if((i==pp->md->FL_CORESEC2)&&((pp->md->FL_ROBOTCTRL==3) || (CheckPI(pp->pi->I_R_MANUAL) == PI_RESET)))
				return RobotPIErrChkCore(pp, pp->pi->I_R_C2PRB,ER0_ROB2_CPRB,i);
		}
	}
	return FALSE;
}

BOOL RobotCoreBwdChk(PROBOT pp,int i)  //机械手失败  中子退过程判断  20170711
{
	if(pp != NULL)
	{
		if(pp->md->FL_ROBOTCTRL >=2  || (CheckPI(pp->pi->I_R_MANUAL)== PI_RESET))	
		{		
			if(i==pp->md->FL_CORESEC1)
			 	return RobotPIErrChkCore(pp, pp->pi->I_R_CPRB,ER0_ROBO_CPRB,i);
			else if((i==pp->md->FL_CORESEC2)&&((pp->md->FL_ROBOTCTRL==3) || (CheckPI(pp->pi->I_R_MANUAL) == PI_RESET)))
				return RobotPIErrChkCore(pp, pp->pi->I_R_C2PRB,ER0_ROBO_CPRB,i);
		}
	}
	
	return FALSE;
}


void RobotCoreFwd(PROBOT pp,int i)
{
	if(pp != NULL)
	{
		if(i==pp->md->FL_CORESEC1)		
			SetPOBit(pp->po->O_R_CPRB, PC_OFF);
		else if(i==pp->md->FL_CORESEC2)
			SetPOBit(pp->po->O_R_C2PRB, PC_OFF);						
	}
}

void RobotCoreFwdEnd(PROBOT pp,int i)
{
	if(pp != NULL)
	{
		if(i==pp->md->FL_CORESEC1)
		{
			SetPOBit(pp->po->O_R_CPRB, PC_OFF);
			SetPOBit(pp->po->O_R_CPRF, PC_ON);
		}
		else if(i==pp->md->FL_CORESEC2)
		{
			SetPOBit(pp->po->O_R_C2PRB, PC_OFF);
			SetPOBit(pp->po->O_R_C2PRF, PC_ON);
		}
	}
}

void RobotCoreBwd(PROBOT pp,int i)
{
	if(pp != NULL)
	{
		if(i==pp->md->FL_CORESEC1)				
			SetPOBit(pp->po->O_R_CPRB, PC_OFF);
		else if(i==pp->md->FL_CORESEC2)
			SetPOBit(pp->po->O_R_C2PRB, PC_OFF);			
	}
}

void RobotCoreBwdEnd(PROBOT pp,int i)
{
	if(pp != NULL)
	{
		if(i==pp->md->FL_CORESEC1)
		{
			SetPOBit(pp->po->O_R_CPRF, PC_OFF);
			SetPOBit(pp->po->O_R_CPRB, PC_ON);
		}
		else if(i==pp->md->FL_CORESEC2)
		{
			SetPOBit(pp->po->O_R_C2PRF, PC_OFF);
			SetPOBit(pp->po->O_R_C2PRB, PC_ON);
		}
	}
}

BOOL InitRobotPart(PROBOT pp, UI8 sn)
{
	static const SUB_INIT_INFO sub_init[]  = ROBOT_INIT_INFO;

	if(!INIT_PART(pp, ROBOT_ID, sn, sub_init))
		return FALSE;

	pp->pi->I_ROBOT = 22;
	pp->po->O_ROBOT = 44;

	pp->pclmp = (PCLAMP)GetMachineSubPart(CLAMP_ID, sn);
	return TRUE;
}

void RobotEmergeStop(PROBOT pp, BOOL bemerge)
{
	if(pp)
	{
		if(bemerge)
			SetPOBit(pp->po->O_R_EMERG, PC_OFF);
		else
			SetPOBit(pp->po->O_R_EMERG, PC_ON);
	}
}

void RobotEmergeStop2(PROBOT pp, BOOL bemerge)    //机械手急停2   
{
	if(pp)
	{
		if(bemerge)
			SetPOBit(pp->po->O_R_EMERG2, PC_OFF);
		else
			SetPOBit(pp->po->O_R_EMERG2, PC_ON);
	}
}


void EuroRobotChkLoop(PROBOT pp)              //欧规机械手动作
{
    int i;
	WORD robotmanual = FALSE;
	
	if(pp == NULL)
		return;

	RobotSafeChk(pp);

    /*新增功能  ，检测到"机械手手动"点后，不管机械手是否使用或者
                 选择常规，欧规，都检测 机械手急停输入点     20170710*/

	robotmanual = (CheckPI(pp->pi->I_R_MANUAL)==PI_RESET) ? TRUE:FALSE;
	
	if ((pp->md->FL_ROBOTCTRL <=1) && (robotmanual == FALSE))
		return;
		
	if((CheckPI(pp->pi->I_R_EMEG) == PI_SET && (ChkSysCfg(XF0_CPRNH) == 0))
	|| (CheckPI(pp->pi->I_R_EMEG) == PI_RESET && (ChkSysCfg(XF0_CPRNH) != 0))) //机械手急停
	{
		if(!ChkPartErr((PPART)pp, ER0_ROBE))
		{
			ResetSystem();
			SET_PART_ERR(pp, ER0_ROBE);
			StandAlarm(TRUE);
		}
	}
	else if(((CheckPI(pp->pi->I_R_EMEG2) == PI_SET && (ChkSysCfg(XF0_CPRNH) == 0))
		|| (CheckPI(pp->pi->I_R_EMEG2) == PI_RESET && (ChkSysCfg(XF0_CPRNH) != 0)))&&((pp->md->FL_ROBOTCTRL==3)||(robotmanual == TRUE))) //机械手急停2
	{
		if(!ChkPartErr((PPART)pp, ER0_ROBE))
		{
			ResetSystem();
			SET_PART_ERR(pp, ER0_ROBE);
			StandAlarm(TRUE);
		}
	}
  	else if(ChkPartErr((PPART)pp, ER0_ROBE)) 
  	{
		RestartSystem();  
  	}

	if(pp->md->FL_ROBOTCTRL <=1)
		return;
	
	if(pp->pclmp != NULL && pp->pclmp->psfdr != NULL && FSafeDoorIsClose(pp->pclmp->psfdr)
		&& !BSafeDoorIsErr(pp->pclmp->psfdr))   //安全装置1和2，关联前后安全门
	{
		if (!TestPOIsOn(pp->po->O_R_SFMA1))
			SetPOBit(pp->po->O_R_SFMA1, PC_ON);
		if (!TestPOIsOn(pp->po->O_R_SFMA2))
			SetPOBit(pp->po->O_R_SFMA2, PC_ON);
	}
	else
	{
		if (TestPOIsOn(pp->po->O_R_SFMA1))
			SetPOBit(pp->po->O_R_SFMA1, 0);
		if (TestPOIsOn(pp->po->O_R_SFMA2))
			SetPOBit(pp->po->O_R_SFMA2, 0);
	}

	if(pp->pclmp != NULL && pp->pclmp->psfdr != NULL && FSafeDoorIsClose(pp->pclmp->psfdr))
 	{
		if(!TestPOIsOn(pp->po->O_R_SDRC))
			SetPOBit(pp->po->O_R_SDRC, PC_ON);
	}
	else 
	{
  		if(TestPOIsOn(pp->po->O_R_SDRC))                                            
 			SetPOBit(pp->po->O_R_SDRC, 0);
	}


	if(pp->pclmp != NULL && pp->pclmp->psfdr != NULL && !BSafeDoorIsErr(pp->pclmp->psfdr))		 //关联后安全门		0906
 	{																														   
		if (!TestPOIsOn(pp->po->O_R_SDRC2))
			SetPOBit(pp->po->O_R_SDRC2, PC_ON);
	}
	else 
	{
  		if (TestPOIsOn(pp->po->O_R_SDRC2))                                            
 			SetPOBit(pp->po->O_R_SDRC2, 0);
	}

	
	if(pp->pclmp == NULL ||!ClampEjectActEnable(pp->pclmp))
	{
		if (!TestPOIsOn(pp->po->O_R_EJTF))
 		{                                            
			SetPOBit(pp->po->O_R_EJTF, PC_ON);
			SetPOBit(pp->po->O_R_EJTB, PC_ON);
		}
	}
    else
    {
		if (!ClampEjectBwdIsErr(pp->pclmp))
		{
  			if(!TestPOIsOn(pp->po->O_R_EJTB))                                            
			{
				SetPOBit(pp->po->O_R_EJTB, PC_ON);
				SetPOBit(pp->po->O_R_EJTF, 0);
			}         
      	} 

		else if (ClampEjectFwdIsErr(pp->pclmp)==0)	
		{
  			if(!TestPOIsOn(pp->po->O_R_EJTF))                                            
			{
				SetPOBit(pp->po->O_R_EJTF, PC_ON);
				SetPOBit(pp->po->O_R_EJTB, 0);
  			}
		}
	}	  

    if (pp->md->FL_CORESEC1 > 0)
    {
        i = pp->md->FL_CORESEC1 - 1;        
        if(pp->pclmp->pcore[i]->md->FL_CORE_CTRL == 1 
        && (pp->pclmp->pcore[i]->corests == 2 || (pp->pclmp->pcore[i]->md->FL_CORE_TRAVEL == 0 && CheckPI(pp->pclmp->pcore[i]->pi->I_CP_BWD) == PI_SET)))
        {
            if(!TestPOIsOn(pp->po->O_R_CPRB))                                            
            {
                SetPOBit(pp->po->O_R_CPRB, PC_ON);
                SetPOBit(pp->po->O_R_CPRF, 0);
            }         
        } 
        
        else if(pp->pclmp->pcore[i]->md->FL_CORE_CTRL == 1 
            && (pp->pclmp->pcore[i]->corests == 1 ||(pp->pclmp->pcore[i]->md->FL_CORE_TRAVEL == 0 && CheckPI(pp->pclmp->pcore[i]->pi->I_CP_FWD) == PI_SET)))
        {
            if(!TestPOIsOn(pp->po->O_R_CPRF))                                            
            {
                SetPOBit(pp->po->O_R_CPRF, PC_ON);
                SetPOBit(pp->po->O_R_CPRB, 0);
            }
        }

        else 
        {
            if(TestPOIsOn(pp->po->O_R_CPRF))                                            
                SetPOBit(pp->po->O_R_CPRF, 0);
            if(TestPOIsOn(pp->po->O_R_CPRB))  
                SetPOBit(pp->po->O_R_CPRB, 0);           
        }
    }

    if (pp->md->FL_CORESEC2 > 0)
    {  
        i = pp->md->FL_CORESEC2 - 1;
        if(pp->pclmp->pcore[i]->md->FL_CORE_CTRL == 1 
        && (pp->pclmp->pcore[i]->corests == 2 || (pp->pclmp->pcore[i]->md->FL_CORE_TRAVEL == 0 && CheckPI(pp->pclmp->pcore[i]->pi->I_CP_BWD) == PI_SET)))
        {             
            if(!TestPOIsOn(pp->po->O_R_C2PRB))                                            
            {
                SetPOBit(pp->po->O_R_C2PRB, PC_ON);
                SetPOBit(pp->po->O_R_C2PRF, 0);
            }         
        }  
        
        else if(pp->pclmp->pcore[i]->md->FL_CORE_CTRL == 1 
            && (pp->pclmp->pcore[i]->corests == 1 ||(pp->pclmp->pcore[i]->md->FL_CORE_TRAVEL == 0 && CheckPI(pp->pclmp->pcore[i]->pi->I_CP_FWD) == PI_SET)))
        {
            if(!TestPOIsOn(pp->po->O_R_C2PRF))                                            
            {
                SetPOBit(pp->po->O_R_C2PRF, PC_ON);
                SetPOBit(pp->po->O_R_C2PRB, 0);
            }
        }
        else 
        {
            if(TestPOIsOn(pp->po->O_R_C2PRF))                                            
                SetPOBit(pp->po->O_R_C2PRF, 0);
            if(TestPOIsOn(pp->po->O_R_C2PRB))  
                SetPOBit(pp->po->O_R_C2PRB, 0);           
        }            
    }
}  


void Robot_Turn()
{
	PROBOT  probt = (PROBOT)GetPartByID(MK_PART(ROBOT_ID, 1));
	
	UI16 pi;
	
	if(probt == NULL)
		return;

	if(!ChkOperMode(OM_AUTO))	//非自动情况下直接结束，不判断0-1的变化
		return;
	
	if(probt->md->FL_ROBOTCTRL == 1)
		pi = probt->pi->I_ROBOT;
	else
		pi = probt->pi->I_R_CLSM;

	
	if(probt->md->FL_ROBOTCTRL != 0)
	{	
		if(probt->flag_Robot_Turn == 1)
		{
			if(CheckPI(pi) == PI_RESET)
			{
				if((UI16)(GetMsTick()-probt->Tm_Robot_Turn) >= 100)	//连续100ms检测到输入点无信号，才认为机械手0-1变化正常
				{
					probt->flag_Robot_Turn = 0;
				}	
			}
			else
			{
				probt->Tm_Robot_Turn = GetMsTick();
			}
		}
		else
		{
			probt->Tm_Robot_Turn = GetMsTick();
		}
	}
	else
	{
		probt->flag_Robot_Turn = 0;
		probt->Tm_Robot_Turn = GetMsTick();
	}
}

void MoldAlarmClear()
{
	PROBOT  probt = (PROBOT)GetPartByID(MK_PART(ROBOT_ID, 1));
	if(!TestPOIsOn(probt->po->O_M_MYALARM))
		SetPOBit(probt->po->O_M_MYALARM,PC_ON);
}

void MoldClampCloseStart(PROBOT pp)      //模具开模完成    输出与开模结束一致   20161219    yxh
{
	if(pp != NULL)
	{
		SetPOBit(pp->po->O_M_OPN, PC_OFF);     //模具开模完成 在关摸前关闭    20161219       yxh
	}
}

void MoldClampOpenEnd(PROBOT pp)      //模具开模完成    输出与开模结束一致 20161219    yxh
{
	if(pp != NULL && pp->md->FL_MOJUPOTE == 1)
	{
		SetPOBit(pp->po->O_M_OPN, PC_ON);    //模具开模完成   在开模结束时输出    20161219      yxh
	}
}

void MoldChkLoop(PROBOT pp)     //模具安全门关      输出与机械手安全门关一致
{
	PALARM   palrm = (PALARM)GetPartByID(MK_PART(ALARM_ID, 1)); 
	PCLAMP   pc = (PCLAMP)GetPartByID(MK_PART(CLAMP_ID, 1)); 
	PMACHINE  pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1)); 
	if(pp == NULL)
		return;

	if(pp->md->FL_MOJUPOTE ==1)
	{
		if(pp->pclmp != NULL && pp->pclmp->psfdr != NULL && FSafeDoorIsClose(pp->pclmp->psfdr))
	 	{
			if (!TestPOIsOn(pp->po->O_M_SFD))
				SetPOBit(pp->po->O_M_SFD, PC_ON);
		}
		else 
		{
	  		if (TestPOIsOn(pp->po->O_M_SFD))                                            
	 			SetPOBit(pp->po->O_M_SFD, PC_OFF);
		}

		if(pp->pclmp == NULL ||!ClampEjectActEnable(pp->pclmp))  //模具顶进完成   与   模具顶退完成
		{
			if (!TestPOIsOn(pp->po->O_M_EJEF))
	 		{                                            
				SetPOBit(pp->po->O_M_EJEF, PC_ON);
				SetPOBit(pp->po->O_M_EJEB, PC_ON);
			}
		}
	    else
	    {
			if (!ClampEjectBwdIsErr(pp->pclmp))
			{
	  			if(!TestPOIsOn(pp->po->O_M_EJEB))                                            
				{
					SetPOBit(pp->po->O_M_EJEB, PC_ON);
					SetPOBit(pp->po->O_M_EJEF, PC_OFF);
				}         
	      	} 

			else if (ClampEjectFwdIsErr(pp->pclmp)==0)	
			{
	  			if(!TestPOIsOn(pp->po->O_M_EJEF))                                            
				{
					SetPOBit(pp->po->O_M_EJEF, PC_ON);
					SetPOBit(pp->po->O_M_EJEB, PC_OFF);
	  			}
			}
		}
	}


	if(TestPI(pc->pi->I_CLS_END) == 1)
	{
	    if(!TestPOIsOn(pc->po->O_RSV_CLS))
		SetPOBit(pc->po->O_RSV_CLS,PC_ON);
	}
	else
        SetPOBit(pc->po->O_RSV_CLS,PC_OFF);

	
	if(CheckPIUsed(pp->pi->I_M_MYALARM))    //模压检测功能   时时检测，有信号时就出警报   20161219
	{
		if(TestPI(pp->pi->I_M_MYALARM) == 1)
		{
			if(!CHK_PART_ERR(pp,ER0_MoldO))
			{
				SET_PART_ERR(pp,ER0_MoldO);
				SetPOBit(palrm->po->O_ALM_SRV,PC_ON);   //检测到有信号就输出，用于采用模腔部分检测时，出问题后的翻斗  20170217
			}
		}
	}

	if(TestPOIsOn(pp->po->O_M_MYALARM))    //模压警报解除输出1s        20161219     yxh
	{
		if((UI16)(Get10MsTick() - pp->AlarmTime) >= 100)
		{
			SetPOBit(pp->po->O_M_MYALARM,0);
			SetPOBit(palrm->po->O_ALM_SRV,0);   
		}
	}
	else
	{
		pp->AlarmTime = Get10MsTick();
	}
}

BOOL MoldPIErrChk(PROBOT pp, UI8 pi_name)       //模具保护功能输入点检测   20161219
{
	if(pp != NULL)
	{
		return PartErrSet(pp, ER0_Mold2, CheckPI(pi_name)==PI_RESET);
	}

	return FALSE;
}

BOOL MoldClampClsErrChk(PROBOT pp)		//模具保护等待     20161219    yxh
{
	UI16 pi;		

	if(pp != NULL && pp->md->FL_MOJUPOTE == 1)
	{
		pi = pp->pi->I_M_CLS;

		return PartErrSet(pp, ER0_Mold2, CheckPI(pi) == PI_RESET);
	}
	else
		return FALSE;
}

BOOL MoldClampClsProtectErrChk(PROBOT pp)     //模具保护    20161219    yxh
{
  	if(pp != NULL)
  	{
  		if(pp->md->FL_MOJUPOTE == 1 && (((CheckPI(pp->pi->I_M_CLS) == PI_RESET && CheckPIUsed(pp->pi->I_M_CLS)))
			|| (CheckPI(pp->pi->I_M_ROB) == PI_RESET && CheckPIUsed(pp->pi->I_M_ROB))))
		{
			SET_PART_ERR(pp, ER0_MoldO);
			return TRUE;
		}
  	}

  	return FALSE;
}

BOOL MoldClampOpnProtectErrChk(PROBOT pp)  //模具保护   201612129   yxh
{
  	if(pp != NULL)
  	{
  		if(pp->md->FL_MOJUPOTE == 1 &&  CheckPI(pp->pi->I_M_CLS) == PI_RESET)
		{
			SET_PART_ERR(pp, ER0_MoldO);
			return TRUE;
		}
  	}

  	return FALSE;
}

BOOL MoldEjectFwdErrChk(PROBOT pp)  //模具保护    20161219
{
	if(pp->md->FL_MOJUPOTE ==1)
		return MoldPIErrChk(pp, pp->pi->I_M_EJEF);

	return FALSE;
}

BOOL MoldEjectBwdErrChk(PROBOT pp)  //模具保护
{
	if(pp->md->FL_MOJUPOTE == 1)	
		return MoldPIErrChk(pp, pp->pi->I_M_EJEB);

	return FALSE;
}

