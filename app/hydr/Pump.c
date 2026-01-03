#include 	"Pump.h"
#include 	"actcom.h"
#include  "part.h"
#include	"sram.h"
#include	"hydrpos.h"
#include	"EtherCATProc.h"
#include 	"cfgdb.h"
#include	"AutoRunWithOutServo.h"
#include  "stdlib.h"
#include  "AdBios.h"
#include  "ServFive.h"
#include  "Cpld.h"


extern HYDR g_hydrout;

int	OilControlProc(WORD portno,int SetFlow,int SetPres,int RealPres);
UI16 GetMainPortBit(void);
AD_CH_TYPE GetAdChType(UI16 portno);
void ChkServoErr(PPUMP p_pump, WORD err);
BOOL ECATStateCheck(void);
WORD GetRealSlaveNum(void);


extern MOTOR m_motor;
extern UI16 gs_open_loop_stop_data[3][4000];


#define MAX_PUMP	12
PPUMP	g_pumpn[MAX_PUMP] = {NULL};
UI16	g_pump_num = 0;

void SetVlvData_mn(WORD portno, int value, WORD dir)
{	
	p_WorkStru[portno].SpeedCurrent100b = value;
	p_WorkStru[portno].SpeedCurrent = p_WorkStru[portno].SpeedCurrent100b/FLOWACCURACY;
	p_WorkStru[portno].SpeedSet = p_WorkStru[portno].SpeedCurrent;
	p_WorkStru[portno].SpeedRampOK = 1;
	p_WorkStru[portno].Direction_F = dir;
}

void InsMCParaBak(PPUMP p_pump, WORD ServoNo,WORD mode, int ActSpeed, WORD Torque, WORD McPres, int PrsActSpd, WORD PrsRelSpd, WORD PresRamp, WORD ActSpeedRamp, WORD TorqueRamp, WORD pumplist, WORD Model, WORD dely,WORD KiClear,WORD limitramp,WORD SpdDraw,WORD flag)
{
	p_pump->McParaBak.ServoNo = ServoNo;
	p_pump->McParaBak.mode = mode;
	p_pump->McParaBak.ActSpeed = ActSpeed;
	p_pump->McParaBak.Torque = Torque;
	p_pump->McParaBak.McPres = McPres;
	p_pump->McParaBak.PrsActSpd = PrsActSpd;
	p_pump->McParaBak.PrsRelSpd = PrsRelSpd;
	p_pump->McParaBak.PresRamp = PresRamp;
	p_pump->McParaBak.ActSpeedRamp = ActSpeedRamp;
	p_pump->McParaBak.TorqueRamp = TorqueRamp;
	p_pump->McParaBak.pumplist = pumplist;
	p_pump->McParaBak.Model = Model;
	p_pump->McParaBak.dely = dely;
	p_pump->McParaBak.KiClear = KiClear;
	p_pump->McParaBak.limitramp = limitramp;
	p_pump->McParaBak.SpdDraw = SpdDraw;
	p_pump->McParaBak.flag = flag;
}
void RecMCParaBak(WORD ServoNo, WORD NowPres, WORD NowSpd)
{
	PPUMP p_pump = NULL;
	PMC_PARA_BAK p_para_bak = NULL;
	WORD endpress,endspd;
		
	if ((p_pump = g_pumpn[ServoNo]) == NULL)
	{
		return;
	}

	//====恢复时当前实际速度及压力应该高于算法内部设定的底压及底流=======
	endpress = GetRealEndPress(ServoNo+1);
	endspd = GetRealEndSpd(ServoNo+1);

	if(NowPres < endpress)
		NowPres = endpress;

	if(NowSpd < endspd)
		NowSpd = endspd;
	//===================================================================
	
	p_para_bak = &(p_pump->McParaBak);
	
	if (NowPres > p_para_bak->McPres)
		NowPres = p_para_bak->McPres;

	NowSpd = NowSpd*FLWLMT/GetRealMotorMaxRPM(ServoNo+1);
	
	if (NowSpd > p_para_bak->ActSpeed)
		NowSpd = p_para_bak->ActSpeed;
	
	if ((p_para_bak->mode & MC_CTR_MODE_POSI) > 0)
		SetMotionControlOutPara(p_para_bak->ServoNo,p_para_bak->mode, NowSpd, p_para_bak->Torque, NowPres, p_para_bak->PrsActSpd, p_para_bak->PrsRelSpd, p_para_bak->PresRamp, p_para_bak->ActSpeedRamp, p_para_bak->TorqueRamp, p_para_bak->pumplist, p_para_bak->Model, p_para_bak->dely,p_para_bak->KiClear,p_para_bak->limitramp,p_para_bak->SpdDraw,p_para_bak->flag);
}

BOOL ChkPosModeChangeToNOP(WORD ServoNo)
{
	PPUMP p_pump = NULL;
	WORD PosStep = 0;
	BOOL ret = FALSE;
	
	if ((p_pump = g_pumpn[ServoNo]) == NULL)
	{
		return FALSE;
	}

	PosStep = ChkHydrPosMode(p_pump->McCtrl.PosCtrlCh, HYDRNOPOS);

	if((p_pump->lastPosStep == 0) && (PosStep == 1))
		ret = TRUE;
	
	p_pump->lastPosStep = ChkHydrPosMode(p_pump->McCtrl.PosCtrlCh, HYDRNOPOS);

	return ret;
}

WORD GetPumpMcModeSet(WORD ServoNo)
{
    ServoNo = ServoNo - 1;
    
    if (ServoNo < g_pump_num)
        return g_pumpn[ServoNo]->McCtrl.McModeSet;
    else
        return 0;
}

BOOL ChkServoSpdOver(WORD spd)
{
	for (WORD i = 0;i < PUMP_ECAT_NUM;i++)
	{
		if(g_pumpn[i]->McCtrl.McSpdOutRPM > spd)
		{
			return TRUE;
		}
	}

	return FALSE;
}

int GetPumpPress(WORD hydrport)
{
	UI16 i;

	for(i = 0;i< 4;i++)
	{
		if((hydrport>>i)&1)
		{
		    if (i< g_pump_num)
            {      
		        return g_pumpn[i]->st->SERVO_PRESS;
            }
            else
            {
                return 0;
            }
		}
	}

	return 0;	
}

void SetServoState(WORD ServoNo, int RealEncode, int RealSpeed, int RealTorque, WORD AlarmPara, WORD ServoTemp,
                   WORD RealPI, WORD RealPV, WORD ServoFlag, WORD MotorTemp, int ServoState)
{
    PPUMP	p_pump;
	
    if ((p_pump = g_pumpn[ServoNo]) == NULL)
    {
        return ;
    }

    p_pump->st->ENCODE_POS = RealEncode;
    p_pump->st->MOTOR_SPEED_REAL = RealSpeed;
    p_pump->st->MOTOR_TORQUE_REAL = INT_ABS(RealTorque);	

    p_pump->st->SERVO_STATE = ServoFlag;  
	p_pump->st->SERVO_TEMP = ServoTemp;
	p_pump->st->MOTOR_TEMP = MotorTemp/10;
	if(!MotorIsOn())
	    p_pump->st->MOTOR_CURRENT = 0;
	else
		p_pump->st->MOTOR_CURRENT = RealPI;
	if(AlarmPara == 0)
		PdoSendData[ServoNo].CtrlWord = (PdoSendData[ServoNo].CtrlWord)&(0x7F);
	ChkServoErr(p_pump, AlarmPara);
}


/*******************************************************************************************
 *Brief			:获得该动作排量
 *Param			:pumpmain:主泵编号 pumpbit:该动作泵浦配置
 *Return Value	:合流排量
 *Aauthor		:ZT
 *Date			:20190520
********************************************************************************************/
WORD  GetMotorPump_Q(WORD pumpmain,WORD pumpbit)
{
	PPUMP	pp;
	WORD	pumpno;
	WORD	Q = 0;

	if(pumpmain > 0)
	{
		if(g_hydrout.mh->FL_PWRMATCHTBL == 1)
		{
			pumpbit = pumpbit|(0x01<<(pumpmain - 1));
			for(pumpno = 0; pumpno < PUMP_ECAT_NUM; pumpno++)
			{
				if((pumpbit&(0x01<<pumpno)) > 0)
				{
					pp = g_pumpn[pumpno];
					Q = Q + pp->mh->OIL_PUMP_DISPLACEMENT;
				}		
			}
		}
		else
		{	
			if ((pp = g_pumpn[pumpmain - 1]) != NULL)
			{
				Q = pp->mh->OIL_PUMP_DISPLACEMENT;
			}
		}
	}
	
	return Q/10;
}


/*******************************************************************************************
 *Brief			:获取电机最大速度
 *Param			:驱动编号
 *Return Value	:返回电机最大速度
 *Aauthor		:ZT
 *Date			:20190416
********************************************************************************************/
UI16 GetMaxRPM(WORD ServoNo)
{
	PPUMP	p_pump;

	if (CHK_MOTOR_MODE(MOTOR_CTRL_ECAT))
	{
		if(ServoNo < MAX_PUMP)
		{
			if ((p_pump = g_pumpn[ServoNo]) == NULL)
			{
				return 0;
			}
			else
			{
				return p_pump->mh->MOTOR_SPEED_MAX;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return VIR_MAX_SPD;
	}
}

/*******************************************************************************************
 *Brief			:设定速度转为RPM
 *Param			:Servo:驱动器编号	SetSpd:设定流量0.01%
 *Return Value	:返回RPM
 *Aauthor		:ZT
 *Date			:20190416
********************************************************************************************/
int ActSetSpdToRPM(WORD ServoNo, WORD SetSpd)
{
    return SetSpd * GetMaxRPM(ServoNo) / FLWLMT;
}

/************************************************************************************
|电动控制参数的设置
|		速度、扭矩、压力、压力速度、泄压速度
|		压力ramp、速度ramp、扭矩ramp
|参数说明:WORD Model(bit0......bit15)
|bit0...bit3:上次速度，	0=上次输出速度
|							1=上次实际速度与输出速度的最小值
|bit4...bit7:斜率方式，	0=10ms加加速度的S曲线
|							1=1/3加加速度的S曲线
|bit8...bit11:斜率时间，	0=最大上限对应的时间
|							1=当前差值对应的时间
|bit12	    :设定速度的单位区分，  0=设定速度为mm/s
|											1=设定速度为RPM
************************************************************************************/
void	SetMotionControlOutPara(WORD ServoNo,WORD mode, int ActSpeed, WORD Torque, WORD McPres, int PrsActSpd, WORD PrsRelSpd, WORD PresRamp, WORD ActSpeedRamp, WORD TorqueRamp, WORD pumplist, WORD Model, WORD dely,WORD KiClear,WORD limitramp,WORD SpdDraw,WORD flag)
{
    UI16 i;
    int tmp_spd_a, tmp_set;
    UI16	pumplist_diff = 0;	//差异pump
    UI16	pumplist_add = 0;	//新增pump连接
    UI16	pumplist_sub = 0;	//去除pump连接
    PMC_PARA	PMcPara;
    PMC_CTRL	PMcCtrl;

	if (g_pumpn[ServoNo] == NULL)
	{
		return;
	}

    InsMCParaBak(g_pumpn[ServoNo],ServoNo,mode, ActSpeed, Torque, McPres, PrsActSpd, PrsRelSpd, PresRamp, ActSpeedRamp, TorqueRamp, pumplist, Model, dely,KiClear,limitramp,SpdDraw,flag);
	//其他动作运行时，强行关闭所有滑行动作
	for (i = 0;i < MAXDAPORT;i++)
	{
		if (ChkHydrPosMode(i+1, HYDRSMSTOP))
		{		
	    	PosControlEnd(i + 1);
		}
	}
	
    PMcPara = &(g_pumpn[ServoNo]->McPara);
    PMcCtrl = &(g_pumpn[ServoNo]->McCtrl);

	if (PMcPara == NULL || PMcCtrl == NULL)
	{
		return;
	}

	PMcCtrl->McModeSet = mode;

	//动作开始泄压时，表示一个动作完成，置停止标记
	if ((flag == ZEROHYDRD)||(flag == ZEROHYDRD1))
	{
		PMcCtrl->StopFlag = TRUE;
	}
	else
	{
		PMcCtrl->StopFlag = FALSE;
	}

    //速度百分比调整，当设定为0时，默认关闭(100等于百分之百速度)
	if (SpdDraw == 0)
	{
		SpdDraw = 100;
	}
    
    if(CHK_MOTOR_MODE(MOTOR_CTRL_COMMON))
    {
        PMcCtrl->NormalSlavePump = pumplist;    //普通机模式泵浦
        PMcCtrl->NormalFlowSet = ActSpeed*SpdDraw/100;
    }
    
	/*=============主站从站关联的相关处理代码-start======================*/
	/*																	 */

    pumplist_diff = PMcCtrl->SlavePump ^ pumplist;//异或操作得出差异pump
    pumplist_diff = pumplist_diff & (~(0x01 << ServoNo));	//去掉主站标记，下面处理的是 从站
    pumplist_add = pumplist_diff & pumplist;		//得出新增的pump编号
    pumplist_sub = pumplist_diff ^ pumplist_add;	//得出移除的pump编号


	//查询当前主站，上级任务是否为其他主站的从站，如果是则移除其从站关联
	if (PMcCtrl->MainServoNo != ServoNo)
	{
		g_pumpn[PMcCtrl->MainServoNo]->McCtrl.SlavePump = (g_pumpn[PMcCtrl->MainServoNo]->McCtrl.SlavePump & (~(0x01<<ServoNo)));
	}	

	
    for (i = 0; i < g_pump_num; i++)
    {
    	if (i == ServoNo)
			continue;

		//主站的从站关联添加
        if (((pumplist >> i) & 0x01) > 0)
        { 
			//查询当前从站，上级任务是否为其他主站的从站，如果是则移除其从站关联
			g_pumpn[g_pumpn[i]->McCtrl.MainServoNo]->McCtrl.SlavePump = (g_pumpn[g_pumpn[i]->McCtrl.MainServoNo]->McCtrl.SlavePump & (~(0x01<<i)));		
			g_pumpn[i]->McCtrl.MainServoNo = ServoNo;			//设置跟随的主站编号
			g_pumpn[i]->McCtrl.McModeSet = MC_CTR_MODE_SLAVE;
			g_pumpn[i]->McCtrl.RunState = 1;
			g_pumpn[i]->McCtrl.LimitRamp = limitramp;			
			
			PMcCtrl->SlavePump = PMcCtrl->SlavePump | (0x01 << i);
			//跟随主泵运行的从泵输出合流阀，并关闭分流阀
			SetPOBit(g_pumpn[i]->po->HYD_COMB_PUMP, PC_ON);
			SetPOBit(g_pumpn[i]->po->HYD_SHUNT_PUMP ,PC_OFF);			
        }
		else
		{
			//主泵运行时，未同步且没按安装单向阀的从泵输出分流阀
			if (g_pumpn[i]->mh->MODE_SLAVE_FOLLOW == 1)
			{
				SetPOBit(g_pumpn[i]->po->HYD_SHUNT_PUMP, PC_ON);
			}
		}

		//主站的从站关联移除
		if (((pumplist_sub >> i) & 0x01) > 0)
        {  
			g_pumpn[i]->McCtrl.MainServoNo = ServoNo;			//设置跟随的主站编号
			g_pumpn[i]->McCtrl.McModeSet = MC_CTR_MODE_STOP;
			g_pumpn[i]->McCtrl.RunState = 0;	

			PMcCtrl->SlavePump = PMcCtrl->SlavePump & (~(0x01 << i));

			//在主泵移除原有从泵时，如果从泵未安装单向阀，需要打开对应分流阀
			if (g_pumpn[i]->mh->MODE_SLAVE_FOLLOW == 0)
			{
				SetPOBit(g_pumpn[i]->po->HYD_SHUNT_PUMP, PC_OFF);
			}
			else
			{
				SetPOBit(g_pumpn[i]->po->HYD_SHUNT_PUMP, PC_ON);
			}
			SetPOBit(g_pumpn[i]->po->HYD_COMB_PUMP ,PC_OFF);				
        }
    }

	PMcCtrl->SlavePump = PMcCtrl->SlavePump | (0x01 << ServoNo);//从站增加自身通道
    PMcCtrl->MainServoNo = ServoNo;
	/*																	 */
	/*=============主站从站关联的相关处理代码-end========================*/
	
	PMcPara->ActDelay = dely * MS_CYC_CNT;		
	PMcCtrl->LimitRamp = limitramp;

    if(PMcPara->Mcnow[0] > PMcCtrl->McPresOut)
    {
        PMcPara->Mcnow[0] = PMcCtrl->McPresOut; //开关模平稳算法压力指令提前减，平滑停止结束时需要衔接 2024-5-6 ccz
    }
    PMcPara->Mclast[0] = PMcPara->Mcnow[0];
	
	if (PMcPara->Mcnow[1] > PMcCtrl->McSpdOutRPM)
	{
		PMcPara->Mcnow[1] = PMcCtrl->McSpdOutRPM;
	}
    PMcPara->Mclast[1] = PMcPara->Mcnow[1];	
	
    PMcPara->McSet[0]  = McPres;
    PMcPara->McSet[1]  = ActSetSpdToRPM(ServoNo, ActSpeed)*SpdDraw/100;

	//最大转速限制
	if (g_pumpn[ServoNo]->MotorMaxRPM > 1500)
	{
		if (PMcPara->McSet[1] > g_pumpn[ServoNo]->MotorMaxRPM)
		{
			PMcPara->McSet[1] = g_pumpn[ServoNo]->MotorMaxRPM;
		}
	}

	//特殊功能:直接使用转速设定
    if ((Model & 0x1000))
    {
        PMcPara->McSet[1]  = ActSpeed*SpdDraw/100;
    }

	if ((PMcPara->McSet[1] - PMcPara->Mclast[1]) < 0)
	{
		if (limitramp)
		{
			//最小速度下降斜率保护
			if (ActSpeedRamp < g_pumpn[ServoNo]->mh->SPEED_DEC)
			{
				ActSpeedRamp = g_pumpn[ServoNo]->mh->SPEED_DEC;
			}
		}
	}
	
	if ((PMcPara->McSet[0] - PMcPara->Mclast[0]) < 0)
	{
		//最小压力下降斜率保护
		if (PresRamp < g_pumpn[ServoNo]->mh->PRESS_DEC)
		{
			PresRamp = g_pumpn[ServoNo]->mh->PRESS_DEC;
		}
	}
	else
	{
		//最小压力上升斜率保护
		if (limitramp)
		{
			if (PresRamp < g_pumpn[ServoNo]->mh->PRESS_ACC)
			{
				PresRamp = g_pumpn[ServoNo]->mh->PRESS_ACC;
			}
		}		
	}

	
    PMcPara->ramp[0]	 = PresRamp * abs(PMcPara->McSet[0] - PMcPara->Mclast[0]) / GetSysPresLimit() * MS_CYC_CNT;
    PMcPara->ramp[1]	 = ActSpeedRamp * abs(PMcPara->McSet[1] - PMcPara->Mclast[1]) / GetMaxRPM(ServoNo) * MS_CYC_CNT;

    if (((Model >> 8) & 0x01) > 0)
    {
        PMcPara->ramp[0] = PresRamp * MS_CYC_CNT;
        PMcPara->ramp[1] = ActSpeedRamp * MS_CYC_CNT;
    }

    for (i = 0; i < MAXMCOUTPORT; i++)
    {
        PMcPara->rampok[i]  = FALSE;
        PMcPara->rampstart[i]  = 0;        //斜率启动时间-outsider

        if ((PMcPara->McSet[i] == PMcPara->Mclast[i]) || (PMcPara->ramp[i] == 0))
        {
            PMcPara->ramp[i] = 0;
            PMcPara->rampok[i]  = TRUE;
            PMcPara->Mcnow[i] = PMcPara->McSet[i];
        }
        else
        {
            //速度变化量
            tmp_set = PMcPara->McSet[i] - PMcPara->Mclast[i];
            //计算加速度
            tmp_spd_a = tmp_set * MCACC / PMcPara->ramp[i];
            //赋值加速时间
            PMcPara->Ramp_a[i] = tmp_spd_a;
            //计算加速过程总耗时
            PMcPara->Ramp_T1[i] = 0;
            PMcPara->Ramp_T2[i] = PMcPara->ramp[i];
            PMcPara->Ramp_T3[i] = 0;
        }
    }

	if (PMcCtrl->pMcPres != NULL)
		PMcCtrl->pMcPres->PresSpdLimit = limitramp;	
	
    PMcCtrl->RunState = 1;

	SetPOBit(g_pumpn[ServoNo]->po->HYD_SHUNT_PUMP ,PC_ON);	//主站设定压力流量，启动对应主站分流阀
	SetPOBit(g_pumpn[ServoNo]->po->HYD_COMB_PUMP ,PC_OFFD);	//主站设定压力流量，关闭对应主站合流阀
}


/*******************************************************************************************
 *Brief			:斜率控制函数	速度之间斜率切换时，根据不同的斜率设定规划速度变化
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20190925
********************************************************************************************/
void MotionControlRampOut()
{
	PCLAMP pclmp = (PCLAMP)GetPartByID(MK_PART(CLAMP_ID, 1));
    PMC_PARA	Mc_Out = NULL;
    WORD  lasttime = 0;
    int   tmpcur;
    int		i, j;

    for (j = 0; j < g_pump_num; j++)
    {
        Mc_Out = &(g_pumpn[j]->McPara);

		if(Mc_Out->ActDelay > 0)
		{//动作延迟相当于液压延迟
			Mc_Out->ActDelay--;
			continue;
		}
			
        for (i = 0; i < MAXMCOUTPORT; i++)
        {
            if (!Mc_Out->rampok[i])	//斜率完成标记
            {
                lasttime = Mc_Out->rampstart[i];

                if ((lasttime < Mc_Out->ramp[i]))
                {
                    if (lasttime <= Mc_Out->Ramp_T1[i])
                    {
                        tmpcur = (Mc_Out->Ramp_aa[i] * lasttime * lasttime) / 2 / MCACC + Mc_Out->Mclast[i];
                    }
                    else if (lasttime  <= (Mc_Out->Ramp_T1[i] + Mc_Out->Ramp_T2[i]))
                    {
                        tmpcur = (Mc_Out->Ramp_T1[i] * Mc_Out->Ramp_a[i] / 2 + (lasttime - Mc_Out->Ramp_T1[i] ) * Mc_Out->Ramp_a[i]) / MCACC + Mc_Out->Mclast[i];
                    }
                    else if (lasttime <= Mc_Out->ramp[i])
                    {
                        tmpcur = Mc_Out->McSet[i] - Mc_Out->Ramp_aa[i] * (Mc_Out->ramp[i] - lasttime) * (Mc_Out->ramp[i] - lasttime) / 2 / MCACC;
                    }
                    else
                    {
                        tmpcur = Mc_Out->McSet[i];
                        Mc_Out->rampok[i] = TRUE;
                    }
                }
                else
                {
                    tmpcur = Mc_Out->McSet[i];
                    Mc_Out->rampok[i] = TRUE;
                }
            }
            else
            {
                tmpcur = Mc_Out->McSet[i];
            }

			if(j == 0 && pclmp->clampfunc_sv == 1 && (pclmp->md->FL_CLSOPEN_MODE == 2))	// 1号驱动，同时开关模走算法时，走开关模内部计算出的位置斜率方式
			{
			}
			else
            	Mc_Out->Mcnow[i] = tmpcur;

            Mc_Out->rampstart[i]++;
        }
    }
}

WORD AllocPresCtrlCh(PPUMP p_pump)
{
	UI16 i = 0;
	
	if(p_pump == NULL)
		return 0;
	
	if(p_pump->McCtrl.Fl_Pres)
	{
		return p_pump->McCtrl.PresCtrlCh;
	}
	
	for(i=0;i<MAXDAPORT;i++)
	{
		if(g_press[i].runstate == 0)
		{
			p_pump->McCtrl.Fl_Pres = TRUE;
			p_pump->McCtrl.PresCtrlCh = i+1;
			p_pump->McCtrl.pMcPres = &(g_press[i]);			
			p_pump->McCtrl.McModeSet = p_pump->McCtrl.McModeSet | MC_CTR_MODE_PRES;
			p_pump->McCtrl.pMcPres->ppump = p_pump;
			g_press[i].runstate = 1;
			SetServoPIDPara(600,40,0,i);
			
            db_printf("Allocate Pres Control Ok, Ch = %d\r\n", i);
			return i;
		}
	}	

	db_printf("Allocate Pres Control Err: Full Control!!!\r\n", i);
	return 0;
}

WORD FreePresCtrlCh(PPUMP p_pump)
{
	UI16 ch = 0;
	if(p_pump == NULL)
		return 0;
	
	if(p_pump->McCtrl.Fl_Pres)
	{
		ch = p_pump->McCtrl.PresCtrlCh;
		
		p_pump->McCtrl.Fl_Pres = FALSE;
		p_pump->McCtrl.PresCtrlCh = 0;
		p_pump->McCtrl.pMcPres->ppump = NULL;
		p_pump->McCtrl.pMcPres->runstate = 0;
		p_pump->McCtrl.pMcPres = NULL;		

		db_printf("Free Pres Control Ok, Ch = %d\r\n", ch);
		return ch;
	}
	else
	{
		db_printf("Free Pres Control Err: No control for this pump\r\n", ch);
		return 0;
	}
}

/*******************************************************************************************
 *Brief			:开机时对压力通道进行分配，每个主泵分配一个压力通道
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20190428
********************************************************************************************/

BOOL HydrPrsSenZSetErrChk()
{
	UI16 pumpmain,i;
	PMC_CTRL	PMcCtrl = NULL;

	PHYDR     pp = &g_hydrout;

	if(ChkSysCfg(CO0_SERVOP))
	{//如果不使用内部PID
		pumpmain = 0x01;//GetMainPortBit();

		for (i = 0; i < g_pump_num; i++)
		{
			if (((pumpmain >> i) & 0x01) > 0)
			{
				PMcCtrl = &(g_pumpn[i]->McCtrl);
				
				if(PMcCtrl->RealPres > 50 || PMcCtrl->RealPres < -20)
				{
					SET_PART_ERR(pp, ER0_PRSZSET);
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}


WORD InitPresCtrl()
{
	UI16 pumpmain,i;
	UI16 adpresbit = 0x00;

	if(ChkSysCfg(CO0_SERVOP))
	{//如果不使用内部PID
		pumpmain = 0x07;//GetMainPortBit();

		for (i = 0; i < g_pump_num; i++)
		{
			if (((pumpmain >> i) & 0x01) > 0)
			{
				if(g_pumpn[i]->mh->PRESSAD_CH == 0)
				{
					continue;
				}
//				g_press[i].runstate = 0;					//清除压力控制启动标记
				AllocPresCtrlCh(g_pumpn[i]);				//分配压力控制通道

				if (g_pumpn[i]->McCtrl.pMcPres != NULL)
				{
					if(g_pumpn[i]->mh->REVERSEFLOWMAX > 50)
					{
						g_pumpn[i]->mh->REVERSEFLOWMAX = 50;
					}

					if(!ChkSysCfg(CO8_YL1) && (g_pumpn[i]->mh->REVERSEFLOWMAX < 10))
					{
						g_pumpn[i]->mh->REVERSEFLOWMAX = 10;
					}
					
					g_pumpn[i]->McCtrl.pMcPres->ReverseFlowMax = g_pumpn[i]->mh->REVERSEFLOWMAX*100;
				}
				
				g_pumpn[i]->McCtrl.RealPresCh = g_pumpn[i]->mh->PRESSAD_CH;	//分配压力数据反馈通道
				
				adpresbit = adpresbit|(0x01<<(g_pumpn[i]->mh->PRESSAD_CH - 1));

				//分配压力通道类型给对应AD通道号
				if ((ChkSysCfg(C08_MEJEC) && g_pumpn[i]->mh->PRESSAD_TYPE == 0) || (!ChkSysCfg(C08_MEJEC) && g_pumpn[0]->mh->PRESSAD_TYPE == 0))
				{
					SetAdChType(g_pumpn[i]->mh->PRESSAD_CH, CH_PRESS_5V);
				}
				else
				{
					SetAdChType(g_pumpn[i]->mh->PRESSAD_CH, CH_PRESS_10V);
				}
				
			}
			else
			{
				if (g_pumpn[i]->McCtrl.Fl_Pres == TRUE)
				{
					g_pumpn[i]->McCtrl.pMcPres->runstate = 0;
					g_pumpn[i]->McCtrl.PresCtrlCh = 0;
					g_pumpn[i]->McCtrl.RealPresCh = 0;
					g_pumpn[i]->McCtrl.pMcPres = NULL;
					g_pumpn[i]->McCtrl.Fl_Pres = FALSE;
				}				
			}
		}

		for(i = 0; i<(MAXADPORT +ADMETER_CH_NUM_EXT);i++)
		{
			if(!(adpresbit & (0x01<<i)))
			{
				if(GetAdChType(i + 1) == CH_PRESS_10V || GetAdChType(i + 1) == CH_PRESS_5V)
				{
					SetAdChType(i + 1, CH_METER);
				}
			}
		}
	}
	
	return 1;
}

WORD GetPosCtrlCh(WORD pumpno)
{	
	PPUMP ppump;
	UI16 i = 0;
	
	for(i = 0;i<8;i++)
	{
		if(((pumpno >> i) & 0x01) > 0)
		{
			break;
		}
	}

	
	
	if(i < g_pump_num)
	{	
		ppump = g_pumpn[i];
		return ppump->McCtrl.PosCtrlCh;
	}
	else
	{
		return 0;
	}
}

/*******************************************************************************************
 *Brief			:当停止时。设定速度为0，且实际输出速度为0或者小于0时，模式自动切换到停止模式
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20190912
********************************************************************************************/
void FreeCtrlChPro(PPUMP p_pump)
{
	//只有当动作停止时，才进行停止模式的判定及切换。
	if (p_pump->McCtrl.StopFlag == FALSE)
		return;
	
	if(p_pump->McCtrl.McSpdLastRPM <= 0 && p_pump->McCtrl.RunState > 0 && p_pump->McPara.McSet[1] == 0)
	{		
		if((p_pump->McCtrl.SlavePump & (~(0x01 << p_pump->McCtrl.MainServoNo))) > 0)
		{
			//如果存在从站跟随，在主站切stop时，从站跟随切。如果从泵没有单向阀，这里则不切从泵，使从泵继续跟随主泵
			for(int i = 0; i< g_pump_num; i++)
			{
				if((i != p_pump->McCtrl.MainServoNo) && ((p_pump->McCtrl.SlavePump & (0x01<<i)) > 0)&&(g_pumpn[i]->mh->MODE_SLAVE_FOLLOW == 0))
				{
					g_pumpn[i]->McCtrl.RunState = 0;//从泵需要底压底流时 设定1
        			g_pumpn[i]->McCtrl.SlavePump = 0;
					g_pumpn[i]->McCtrl.McModeSet = MC_CTR_MODE_STOP;

					p_pump->McCtrl.SlavePump = p_pump->McCtrl.SlavePump & (~(0x01 << i));
					SetPOBit(g_pumpn[i]->po->HYD_COMB_PUMP, PC_OFF);
					SetPOBit(g_pumpn[i]->po->HYD_SHUNT_PUMP ,PC_OFF);
				}
			}
		}
		
    	p_pump->McCtrl.McModeSet = MC_CTR_MODE_STOP;
		p_pump->McCtrl.RunState = 1;

		//如果主泵的底流设置是0的话，在主泵切stop时将主泵的分流阀切掉
		if (p_pump->baseflow == 0)
		{
			SetPOBit(p_pump->po->HYD_SHUNT_PUMP, PC_OFF);
		}
	}
}


void DA_OUTPUMP_SPDOUT(UI16 pres,UI16 spd,UI16 no)
{
	PHYDR   ph = &g_hydrout;
	PSERVO  ps = &g_hydrservo;
	UI16 servono = 0;
	UI16 pump = 0;
	int presch = 0;
	WORD pwrm = 0;

	pwrm  =	  ph->mh->DA_TBL_PUMPOUT;	

	for (servono = 0;servono < g_pump_num;servono++)
	{
		if ((pwrm&(0x01<<servono)) > 0)
		{			
			presch = g_pumpn[servono]->McCtrl.PresCtrlCh-1;
			if(presch >= 0 && g_pumpn[servono]->McCtrl.Fl_Pres == TRUE)
			{				
				SetServoPIDPara(ps->mh->KP_DEFAULT,ps->mh->KI_DEFAULT,0,presch);
				InitCtrlTableCnt(g_pumpn[servono]->McCtrl.RealPres,presch);
				if (pres == 0)
				{
					g_pumpn[servono]->McCtrl.pMcPres->presflag = ZEROHYDRD;
				}
				else
				{
					g_pumpn[servono]->McCtrl.pMcPres->presflag = 0;
				}
				
				SetMotionControlOutPara(servono,MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, spd*10, 0, pres*10, 0, 0, 200, 200, 0, pwrm, 0, 100,0,1,0,0); 
				return;
			}

			if (CHK_MOTOR_MODE(MOTOR_CTRL_COMMON))
			{
				if((servono+1) != no)
				{
					setdaout(2*(servono+1)-2,pres*ACCURACY,1);
					SetFlowData(servono+1,spd*ACCURACY,1, 1, spd,1);
				}
			}
			
			return;
		}
		pump = pump | (0x01<<servono);
	}	
	
	SetMotionControlOutPara(0,MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, 0, 0, 0, 0, 0, 200, 200, 0, pump, 0, 100,0,1,0,0);	
}


/*******************************************************************************************
 *Brief			:限制输出速度，防止转速突变
 *Param			:
 *Return Value	:返回输出转速
 *Aauthor		:ZT
 *Date			:20190811
********************************************************************************************/
int SpdLimit(PMC_PARA PMcPara, PMC_CTRL PMcCtrl)
{
	//注射动作不限制斜率

	UI16 presctrlvalid = 0;
	UI16 Down_limit;
	UI16 Up_limit;
	
	if(PMcCtrl->Fl_Pres > 0)
	{
		if(PMcCtrl->pMcPres != NULL)
		{
			presctrlvalid = PMcCtrl->pMcPres->PresCtrlValid;
		}
	}

	//默认加减速。
	Down_limit = PMcPara->RampDown_limit[1];
	Up_limit = PMcPara->RampUp_limit[1];

	if (presctrlvalid == 1)
	{
	//压力控制的加减速限制。
		Down_limit = PMcPara->RampDown_limit[0];
		Up_limit = PMcPara->RampUp_limit[0];
	}

	//解除上升斜率限制，2000转10ms	-	用于快速机快速注射用
	if (PMcCtrl->LimitRamp == 0 && ChkSysCfg(CO1_SAAL))
	{
		Up_limit = 100;
	}

	if (Down_limit > 44)
	{
		Down_limit = 44;
	}	
	
	if(PMcCtrl->McSpdLastRPM > PMcCtrl->McSpdOutRPM)
	{//减速
		if((PMcCtrl->McSpdLastRPM - PMcCtrl->McSpdOutRPM) > (int)(Down_limit))
		{
			PMcCtrl->McSpdOutRPM = PMcCtrl->McSpdLastRPM - (int)(Down_limit);
		}
	}
	else
	{//加速
		if((PMcCtrl->McSpdOutRPM - PMcCtrl->McSpdLastRPM) > (int)(Up_limit))
		{
			PMcCtrl->McSpdOutRPM = PMcCtrl->McSpdLastRPM + (int)(Up_limit);
		}
	}
	
	return PMcCtrl->McSpdOutRPM;
}

void TorqueManage(PPUMP p_pump, PMC_PARA PMcPara, PMC_CTRL PMcCtrl)
{
	if(ChkSysCfg(CO8_YL2))
	{
		PMcCtrl->McTorqueOut = p_pump->mh->TORQUEMAX;
		
		if(PMcCtrl->McModeSet == MC_CTR_MODE_STOP && PMcCtrl->RunState == 0)	//从泵停转
		{
			if(PMcCtrl->McSpdLastRPM == 0)
			{
				PMcCtrl->McTorqueOut = p_pump->mh->TORQUEMAX_SLAVE_STOP;

				if(PMcCtrl->McTorqueOut < PMcCtrl->McLastTorqueOut)
				{
					if((PMcCtrl->McLastTorqueOut - PMcCtrl->McTorqueOut) > (int)(TORQUEDCC))
					{
						PMcCtrl->McTorqueOut = PMcCtrl->McLastTorqueOut - (int)(TORQUEDCC);
					}	
				}
			}
		}
	}
}

/*******************************************************************************************
 *Brief			:从泵启停功能
 *Param			:rpm:当前跟随转速
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20200604
********************************************************************************************/
WORD SlaveFlowStatePro(PPUMP pp,int rpm)
{
	PMC_CTRL PMcCtrl = &(pp->McCtrl);

	if (pp->mh->MODE_SLAVE_FOLLOW > 0)
	{
		return TRUE;
	}

	//主站速度高于设定阈值，从站开始跟随
	if (rpm > (int)PMcCtrl->SlaveStartRpm)
	{
		PMcCtrl->SlaveState = 1;
	}

	//主站速度低于设定阈值，从站不跟随
	if (rpm <= (int)PMcCtrl->SlaveStopRpm || rpm < 0)
	{
		PMcCtrl->SlaveState = 0;
	}
	
	//如果对应的主站已停止，从站不跟随。(主站停止时，可能会出现底压底流，从站不跟随)
	if (g_pumpn[PMcCtrl->MainServoNo]->McCtrl.McModeSet == MC_CTR_MODE_STOP)
	{
		PMcCtrl->SlaveState = 0;
	}

	if (PMcCtrl->SlaveState == 1)
	{
		return TRUE;
	}	
	else
	{
		return FALSE;
	}
}



/*******************************************************************************************
 *Brief			:新运动控制总成，包含停止、压力|速度、压力|位置、DA输出(暂无)等模式控制调度，
 				后面还有对多泵主从联动的处理。	
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20190912
********************************************************************************************/
void PumpStru()
{
    PMC_PARA	PMcPara = NULL;
    PMC_CTRL	PMcCtrl = NULL;
    PPUMP		p_pump = NULL;
	int			tmpflow;
	WORD		servo_err = FALSE;
	WORD		SetPres;
	
    WORD i = 0;	

	//检查Ecat状态及驱动器警报
	if (ChkRunFlag(INIF_USEECAT) && (!ChkServoErrPro() || ECATStateCheck()))
	{
		servo_err = TRUE;
	}

    for (i = 0; i < g_pump_num; i++)
    {
        if (CHK_MOTOR_MODE(MOTOR_CTRL_COMMON) && ((GetSys()->st->PUMP_MAIN>>(i))&0x01)==0)
        {
            continue;
        }
         
        p_pump = g_pumpn[i];
        PMcPara = &(g_pumpn[i]->McPara);
        PMcCtrl = &(g_pumpn[i]->McCtrl);

		//该pump通道开启了压力闭环功能时，需要读取实时压力，用于压力控制
		if(PMcCtrl->Fl_Pres > 0)
		{						
			PMcCtrl->RealPres = GetMeterValue(PMcCtrl->RealPresCh-1);		//从AD读取压力
			if(PMcCtrl->RealPres < 0)
			{
				p_pump->st->SERVO_PRESS = 0;
			}
			else
			{
				p_pump->st->SERVO_PRESS = PMcCtrl->RealPres;
			}			
		}

		if (CHK_MOTOR_MODE(MOTOR_CTRL_COMMON) && !(ChkSysCfg(COA_CHL4N)) && i == 0)
		{
			if(GetMeterValue(MAXADPORT-1) < 0)
			{
				p_pump->st->SERVO_PRESS = 0;
			}
			else
			{
				p_pump->st->SERVO_PRESS = GetMeterValue(MAXADPORT-1);
			}				
		}
        else if (CHK_MOTOR_MODE(MOTOR_CTRL_5MODE))
        {
            if (i < MAX_SERVO_NUM)
            {
                p_pump->st->SERVO_PRESS = g_hydrservo.st->SERVO_PRESS[i];
            }
        }

		//主泵设定
		switch (PMcCtrl->McModeSet)
        {
            /*停止*/
            case MC_CTR_MODE_STOP:
                if (PMcCtrl->RunState == 0 || !MotorIsOn())
				{
					//留给多泵中从泵停止时使用，从泵停止不控制压力
					//马达关闭也无动作
					PMcCtrl->McHydrFlowOut = 0;
                    PMcCtrl->McHydrPresOut = 0;
                    PMcCtrl->McSpdOutRPM = 0;
					PMcCtrl->SetPulse = 0;

					PMcPara->Mcnow[0] = PMcCtrl->McHydrPresOut;
			        PMcPara->Mcnow[1] = PMcCtrl->McSpdOutRPM;
					PMcPara->McSet[0] = PMcCtrl->McHydrPresOut;
					PMcPara->McSet[1] = PMcCtrl->McSpdOutRPM;
					tmpflow = 0;
				}
				else
				{
					if(ChkSysCfg(CO1_ECAT))
					{
						/*底压底流限制*/
						if(PMcPara->Mcnow[0] < p_pump->mh->BASEPRES)
						{
							PMcPara->Mcnow[0] = p_pump->mh->BASEPRES;
						}
						if(PMcPara->Mcnow[1] < p_pump->baseflow)
						{
							PMcPara->Mcnow[1] = p_pump->baseflow;
						}
					}

					if(PMcCtrl->PresCtrlCh > 0)
					{
						tmpflow =OilControlProc(PMcCtrl->PresCtrlCh-1,PMcPara->Mcnow[1] * HYDRADOLD,PMcPara->Mcnow[0],PMcCtrl->RealPres);
					}
					else
					{
						tmpflow = 0;
					}
				}
				
				PMcCtrl->McSpdOutRPM = tmpflow/HYDRADOLD;				
                break;
            /*速度模式-压力使用上限*/
            case MC_CTR_MODE_SPEED:
                if (CUS_ID == CS_HAIXIN)
				{
					SetPres = GetSys()->mh->DAPRES_SYSLMT * 10 * (100 + GetSys()->pinj->mh->FL_INJCHAB_UPPRES)/100;
				}
				else
				{
					SetPres = GetSys()->mh->DAPRES_SYSLMT * 10;
				}
                
				PMcPara->Mcnow[0] = SetPres;
                
				if(PMcCtrl->PresCtrlCh > 0)
				{
					tmpflow = OilControlProc(PMcCtrl->PresCtrlCh-1,PMcPara->Mcnow[1] * HYDRADOLD,SetPres,PMcCtrl->RealPres);
				}
				else
				{
					tmpflow = PMcPara->Mcnow[1] * HYDRADOLD;
				}
				
				PMcCtrl->McSpdOutRPM = tmpflow/HYDRADOLD;
				FreeCtrlChPro(p_pump);
                break;
			/*标准速度压力*/
			case MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES:				
				if(PMcCtrl->PresCtrlCh > 0)
				{					
					tmpflow = OilControlProc(PMcCtrl->PresCtrlCh-1,PMcPara->Mcnow[1] * HYDRADOLD,PMcPara->Mcnow[0],PMcCtrl->RealPres);
				}
				else
				{
					tmpflow = PMcPara->Mcnow[1] * HYDRADOLD;
				}
				
				PMcCtrl->McSpdOutRPM = tmpflow/HYDRADOLD;
				FreeCtrlChPro(p_pump);
				break;
			/*压力位置*/
            case MC_CTR_MODE_PRES|MC_CTR_MODE_POSI:
				PMcPara->Mcnow[1] = NewPosControl(PMcCtrl->PosCtrlCh,PMcPara->Mcnow[0],PMcPara->Mcnow[1],0,PMcCtrl->McSpdLastRPM);
                PMcPara->Mcnow[0] = GetHydrPosPresCommand(PMcCtrl->PosCtrlCh, PMcPara->Mcnow[0]);
				
				if (ChkPosModeChangeToNOP(i))
				{
					RecMCParaBak(i,PMcPara->Mcnow[0] ,PMcPara->Mcnow[1]);
				}
                
				if(PMcCtrl->PresCtrlCh > 0)
				{
					tmpflow = OilControlProc(PMcCtrl->PresCtrlCh-1,PMcPara->Mcnow[1] * HYDRADOLD,PMcPara->Mcnow[0],PMcCtrl->RealPres);
				}
				else
				{
					tmpflow = PMcPara->Mcnow[1] * HYDRADOLD;
				}

				PMcCtrl->McSpdOutRPM = tmpflow/HYDRADOLD;	
				FreeCtrlChPro(p_pump);
				break;
			case MC_CTR_MODE_DA:				
				break;
            default:
                PMcCtrl->McSpdOutRPM = 0;
				PMcCtrl->SetPulse = 0;
				
				PMcPara->Mcnow[0] = PMcCtrl->McHydrPresOut;
		        PMcPara->Mcnow[1] = PMcCtrl->McSpdOutRPM;				
                break;
        }		

		PMcCtrl->McTorqueOut = p_pump->mh->TORQUEMAX;
	}
	

	for (i = 0; i < g_pump_num; i++)
    {
        if (CHK_MOTOR_MODE(MOTOR_CTRL_COMMON) && ((GetSys()->st->PUMP_MAIN>>(i))&0x01)==0)
        {
            continue;
        }
        
        p_pump = g_pumpn[i];
        PMcPara = &(g_pumpn[i]->McPara);
        PMcCtrl = &(g_pumpn[i]->McCtrl);

		/*从泵设定*/
		switch(PMcCtrl->McModeSet)
		{
			case MC_CTR_MODE_SLAVE:	
				//从站跟随主站转速百分比运行		
				if (SlaveFlowStatePro(p_pump,g_pumpn[PMcCtrl->MainServoNo]->McCtrl.McSpdLastRPM))
				{
					PMcCtrl->McSpdOutRPM = g_pumpn[PMcCtrl->MainServoNo]->McCtrl.McSpdOutRPM*GetMaxRPM(i)/GetMaxRPM(PMcCtrl->MainServoNo);
					PMcPara->McSet[1] = g_pumpn[PMcCtrl->MainServoNo]->McPara.McSet[1];

					PMcPara->Mcnow[0] = g_pumpn[PMcCtrl->MainServoNo]->McPara.Mcnow[0];
					PMcPara->Mcnow[1] = PMcCtrl->McSpdOutRPM;	
				}
				else
				{
					PMcCtrl->McHydrFlowOut = 0;
	                PMcCtrl->McHydrPresOut = 0;
	                PMcCtrl->McSpdOutRPM = 0;
					PMcCtrl->SetPulse = 0;
					PMcPara->McSet[1] = 0;
					PMcPara->Mcnow[0] = PMcCtrl->McHydrPresOut;
			        PMcPara->Mcnow[1] = PMcCtrl->McSpdOutRPM;

					if(!ChkSysCfg(CO8_YL2))
						PMcCtrl->McTorqueOut = p_pump->mh->TORQUEMAX_SLAVE_STOP;
				}
				break;
			default:			
				
				break;
		}

		
		switch (MOTOR_MODE)
		{
			case MOTOR_CTRL_5MODE:				
				SetPressData_5(i+1,PMcPara->Mcnow[0]*ACCURACY);
				SetFlowData_5(i+1,PMcCtrl->McSpdOutRPM*VIR_MAX_SPD_K,PMcCtrl->SlavePump);
				break;
			case MOTOR_CTRL_COMMON:
				if(!DAIsAdjust())
				{
					SetPressData(i+1,PMcPara->Mcnow[0]*ACCURACY/10,1);
					SetFlowData(i+1,PMcCtrl->McSpdOutRPM*VIR_MAX_SPD_K,PMcCtrl->NormalSlavePump,PMcCtrl->NormalSlavePump, PMcCtrl->NormalFlowSet/10,1);  
				}
				break;
			case MOTOR_CTRL_ECAT:
				//任意驱动器发生警报时，所有驱动器都保持0速输出。
				
				if(servo_err)
				{
					PMcCtrl->McSpdOutRPM = 0;
				}

				TorqueManage(p_pump, PMcPara,  PMcCtrl);
				SpdLimit(PMcPara, PMcCtrl);	//对速度进行限制

				if (MotorIsOn2()&&(m_motor.mh->PUMP_REVERSE_LIMITTM != 0))
				{
					if (PMcCtrl->McSpdOutRPM < -50 || ((PMcCtrl->McSpdOutRPM >= 0) && (GetRealSpeedByServoNo(i) < -50)))
					{
						if (DIFF_10MS(PMcCtrl->restarttm_10ms) > m_motor.mh->PUMP_REVERSE_LIMITTM)
						{
							SET_PART_ERR(&m_motor,ER1_MOTOREOUTTM);
						}
					}
					else
					{
						PMcCtrl->restarttm_10ms = Get10MsTick();
					}
				}
				else
				{
					PMcCtrl->restarttm_10ms = Get10MsTick();
				}
				
				if (MotorIsOn2() && !((ChkRunFlag(INIF_RESET) || ChkMachineErr(ER0_MEM))))
				{
					PdoSendData[i].ForTorque   =   PMcCtrl->McTorqueOut;		
				    PdoSendData[i].RevTorque   =   PMcCtrl->McTorqueOut;
					//PdoSendData[i].SpeedCmd	   =   PMcCtrl->McSpdOutRPM * (p_pump->EcoderPPR/ 60.0f);
					PdoSendData[i].SpeedCmd	   =   PMcCtrl->McSpdOutRPM;
				}
				else
				{			
					PdoSendData[i].ForTorque   =   0;		
				    PdoSendData[i].RevTorque   =   0;		
					PdoSendData[i].SpeedCmd	   =   0;
				}
				break;			
			default:
				break;
		}
		
		PMcCtrl->McSpdLastRPM = PMcCtrl->McSpdOutRPM;	//保留上一次输出速度
		PMcCtrl->McLastTorqueOut = PMcCtrl->McTorqueOut;
		
		p_pump->st->MOTOR_SPEED_SET = PMcCtrl->McSpdOutRPM;
		PMcCtrl->McLinenow	= PMcPara->Mcnow[1];

        PMcCtrl->McPresOut = PMcPara->Mcnow[0]; //记录当前输出压力
    }
}


BOOL PumpCmd(PPUMP pp,UI32 cmd, UI32 para)
{
	UI32 ret = TRUE;

	switch(GET_CMD(cmd))
	{
		case CMD_DATA_RX:
			if((GET_CMD_PARA(cmd) & 0xff) == MH_ID)
			{				
				UI16 len;
								
				/*判读是否是滑模配置参数*/
				len = (para & 0xffff);

				/*油温上限修改后，初始化相应温度板块的温度配置*/
				if(len > 0)
				{			
					UI16 acc,dec;
					if(ComIsOk())
					{
						InitPresCtrl();
					}
					ChkEncoderPPR();

					pp->McPara.RampUp_limit[0] = (UI32)pp->mh->MOTOR_SPEED_MAX*ACCELERATESTEP/10000;
					pp->McPara.RampDown_limit[0] = (UI32)pp->mh->MOTOR_SPEED_MAX*DECELERATIONSTEP/10000;

					acc = (0 == pp->mh->SPEED_ACC)?1:pp->mh->SPEED_ACC;
					dec = (25 > pp->mh->SPEED_DEC)?25:pp->mh->SPEED_DEC;//速度减速时间下限25ms
					
					pp->McPara.RampUp_limit[1] = pp->mh->MOTOR_SPEED_MAX/(acc*MS_CYC_CNT);
					pp->McPara.RampDown_limit[1] = pp->mh->MOTOR_SPEED_MAX/(dec*MS_CYC_CNT);
					pp->McPara.RampUp_limit[1] = pp->McPara.RampUp_limit[1] == 0?1:pp->McPara.RampUp_limit[1];
					pp->McPara.RampDown_limit[1] = pp->McPara.RampDown_limit[1] == 0?1:pp->McPara.RampDown_limit[1];	

					if (pp->mh->TORQUEMAX < 1000)
					{
						pp->mh->TORQUEMAX = 2000;
					}
					
					if (pp->mh->TORQUEMAX_SLAVE_STOP < 100)
					{
						pp->mh->TORQUEMAX_SLAVE_STOP = 500;
					}			

					if(CHK_MOTOR_MODE(MOTOR_CTRL_ECAT))
						pp->baseflow = (UI16)((UI32)pp->mh->MOTOR_SPEED_MAX*(UI32)pp->mh->BASEFLOW/1000);
					else
						pp->baseflow = 0;

					if (pp->mh->MODE_SLAVE_FOLLOW > 2)
					{
						pp->mh->MODE_SLAVE_FOLLOW = 0;
					}
				}
			}
			else if((GET_CMD_PARA(cmd) & 0xff) == MD_ID)
			{				
				pp->McCtrl.SlaveStartRpm = (WORD)((UI32)pp->mh->MOTOR_SPEED_MAX * (UI32)pp->md->PUMP_SLAVE_START/100);
				pp->McCtrl.SlaveStopRpm = (WORD)((UI32)pp->mh->MOTOR_SPEED_MAX * (UI32)pp->md->PUMP_SLAVE_STOP/100);
			}
			
			break;			
		default:
			ret = TRUE;
			break;
	}

	return ret;
}


BOOL InitPumpPart(PPUMP pp, UI8 sn)
{
    static const SUB_INIT_INFO sub_init[]  = PUMP_INIT_INFO;


    if (!INIT_PART(pp, PUMP_ID, sn, sub_init))
    {
        return FALSE;
    }

	g_pumpn[g_pump_num] = pp;	//赋值
    g_pump_num++;

	pp->mh->PRESS_ACC = 50;	//ms
	pp->mh->PRESS_DEC = 50;	//ms
	pp->pump_no = sn;
    pp->McCtrl.PosCtrlCh  = ApplyHydrPosControl();
    return TRUE;
}

