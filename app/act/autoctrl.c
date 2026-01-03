/******************************************************************************
  文 件 名   : autoctrl.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 自动电眼及其他自动控制部件
  函数列表   :
              AutoCtrlCmd
              AutoCtrlSetup
              InitAutoCtrlPart
              SensorDropChkReset
              SensorDropIsOK
              SensorDropPIChk
              SensorErrChk
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
#include "autoctrl.h"
#include "clamp.h"

void SensorDropChkReset(PAUTOCTRL pp)
{
	int i;
	
	if(pp != NULL)
	{
		pp->bdrop_ok = FALSE;
		pp->chk_st = SENSOR_CHK_INIT;
		pp->chk_set_cnt = 0;
		
		if(ChkSysCfg(XF5_MSENS))
			pp->chk_pi_num = MAX_SENSOR_PI_NUM;
		else
			pp->chk_pi_num = 1;

		for(i = 0; i < pp->chk_pi_num; i++)
			pp->pi_chk_ok[i] = 0;
	}
}

BOOL SensorDropIsOK(PAUTOCTRL pp)
{
	if(pp != NULL)
		return pp->bdrop_ok;
	
	return TRUE;
}


BOOL SensorDropPIChk(PAUTOCTRL pp)
{
	int i;
	
	if (pp != NULL)
	{
		for (i = 0; i < pp->chk_pi_num; i++)
		{
			if (ChkSysCfg(XF6_CHKMR))
			{
				if (CheckPI(pp->pi->I_PRD_CHK[i]) == PI_RESET)
				{
					return TRUE;
				}
			}
			else
			{
				if (CheckPI(pp->pi->I_PRD_CHK[i]) == PI_SET)
				{
					return TRUE;
				}
			}
		}		
	}

	return FALSE;	
}


BOOL SensorErrChk(PAUTOCTRL pp)
{
	int i;
	BOOL pi_chk_set;
	
	if(pp != NULL)
	{
		pi_chk_set = TRUE;
		for(i = 0; i < pp->chk_pi_num; i++)
		{
		    if(ChkSysCfg(XF6_CHKMR))
			{
				if(CheckPI(pp->pi->I_PRD_CHK[i]) == PI_SET)  //(testpb(I_PRD_CHK));
					pi_chk_set = FALSE;
			}
			else
			{
				if(CheckPI(pp->pi->I_PRD_CHK[i]) == PI_RESET)  //(testpb(I_PRD_CHK));
					pi_chk_set = FALSE;
			}
		}

		if(pp->chk_pi_num > 0)
		{
			if(pi_chk_set == TRUE)
			{
				if((pp->chk_set_cnt)++ > 5)  //此处有问题，当再循环时间为0时，可能会出现料没有完全掉落而引起报警      2015-7-21
				{
				    SET_PART_ERR(pp, ER0_DRFUL);   //警报替换为"电眼检测物料已满"
					return TRUE;
				}
			}
			else
				pp->chk_set_cnt = 0;
		}
	}

	return FALSE;
}

ACT_RET SensorDropChkPro(PACT pact, UI32 para)
{
    PAUTOCTRL pp = (PAUTOCTRL)(pact->part);
    ACT_RET ret = ACT_OK;
	UI16 i = 0;

    if(pp == NULL)
        return ACT_ERR_ABORT;

    if(!ChkOperMode(OM_SENR))
		return ACT_ERR_ABORT;
	
    switch (pact->step)
    {
        case INIT1_STEP://电眼检出初始化
			if(ChkSysCfg(XF5_MSENS))
				pp->chk_pi_num = MAX_SENSOR_PI_NUM;
			else
				pp->chk_pi_num = 1;

			for (i = 0; i < pp->chk_pi_num; i++)
			{
				pp->pi_chk_ok[i] = 0;
			}
			pp->chk_err_cnt = 0;
			pp->bdrop_ok = FALSE;
			pp->chk_st = FALSE;
			ACT_STEP_NEW(STAGE_1_STEP);
            break;                        
        case STAGE_1_STEP://电眼检出前延迟
			if (ACT_STEP_10MS > pp->md->TM_SENSORDLY)
			{
				ACT_STEP_NEW(STAGE_2_STEP);
			}
            break; 

        case STAGE_2_STEP://初次检测
			if (SensorDropPIChk(pp))
			{
				ACT_STEP_NEW(STAGE_4_STEP);
			}
			break;
			
		case STAGE_3_STEP://滤波检测
			if (SensorDropPIChk(pp))
			{	
				if (ACT_STEP_MS > 2)
				{
					ACT_STEP_NEW(STAGE_4_STEP);
				}				
			}
			else
			{//检出点信号未持续2ms，检出失败
				ACT_STEP_NEW(STAGE_2_STEP);
			}
			break;
			
		case STAGE_4_STEP://物料满检测
			pp->chk_start_10ms = Get10MsTick();
			ACT_STEP_NEW(STAGE_4_STEP + 1);
			break;
		case STAGE_4_STEP + 1:
			if (!SensorDropPIChk(pp))//检出电眼完成一个跳变，表示检出完成
			{
				pp->bdrop_ok = TRUE;
				ACT_STEP_NEW(END_STEP);
    		}
			else
			{
				if (!ChkSysCfg(XF1_SORCP))
				{
					//检出电眼 在设定检测等待时间内都有信号时警报"电眼检测物料已满"
					if (DIFF_10MS(pp->chk_start_10ms) > pp->md->TM_SENSORCHK)
					{
						SET_PART_ERR(pp, ER0_DRFUL);
						ACT_STEP_NEW(END_STEP);
					}
				}
				else
				{
					pp->bdrop_ok = TRUE;
					ACT_STEP_NEW(END_STEP);
				}				
			}
			break;
			
        case END_STEP:
            ret =  ACT_END;
            break;
			
        default:
            ret =  ACT_END;
            break;
    }

    return ret;
}   


void AutoCtrlSetup(PAUTOCTRL pp)
{
	if(pp != NULL)
		pp->pclmp = (PCLAMP)GetMachineSubPart(CLAMP_ID, GET_PART_SN(pp->part.id));
 }

BOOL InitAutoCtrlPart(PAUTOCTRL pp, UI8 sn)
{
	static const SUB_INIT_INFO sub_init[]  = AUTOCTRL_INIT_INFO;
	static const ACT_INIT_INFO act_init[] = 
	{
		{OS_SENSORCHK, OS_SENSORCHK, ACT_CTRB, SensorDropChkPro, NULL}
	};

	
	if(!INIT_PART(pp, AUTOCTRL_ID, sn, sub_init))
		return FALSE;	

	if(!INIT_ACT(pp, act_init))
		return FALSE;

	pp->pclmp = (PCLAMP)GetMachineSubPart(CLAMP_ID, sn);

	if(ChkSysCfg(XF5_MSENS))
		pp->chk_pi_num = MAX_SENSOR_PI_NUM;
	else
		pp->chk_pi_num = 1;
	
	return TRUE;
}

UI32 AutoCtrlCmd(PAUTOCTRL pp, UI32 cmd, UI32 para)
{
	UI32 ret = TRUE;
	
	if(pp == NULL)
		return FALSE;

	switch(GET_CMD(cmd))
	{
		case CMD_MANUAL_INIT:
			SensorDropChkReset(pp);
			break;
			
		case CMD_SETUP:
			AutoCtrlSetup(pp);
			break;
		default:
			ret = FALSE;
			break;
	}

	return ret;
}

