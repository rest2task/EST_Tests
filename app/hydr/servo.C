/*===========================================================================+
|  Class    : charge process                                                 |
|  Task     : charge process                                                 |
|----------------------------------------------------------------------------|
|  Compile  : CC'C2000 -                                                     |
|  Link     : CC'C2000 -                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : XiaQinghua                                                     |
|  Version  : V1.01                                                          |
|  Creation : 06/21/2009                                                     |
|  Revision : 06-21-2009                                                     |
+===========================================================================*/

#include    "movectrl.h"
#include    "hydr.h"
#include    "servo.h"
#include    "canhead.h"                                  
#include    "configdf.h"
#include    "part.h"
#include    "hwcfg.h"
#include    "admeter.h"
#include    "ioproc.h"
#include    "machapi.h"
#include    "alarm.h"
#include    "systempro.h"
#include    "motor.h"
#include    "machine.h"
 #include    "da.h"





/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
extern	 HYDR	 g_hydrout;
extern	 MACHINE m_machine;   
extern	 ADMETER m_ad;

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
SERVO	 g_hydrservo; 

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
 
/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/


int GetSpdRPMBySpdPulse(WORD ServoNo,int ActSpdPulse)
{
	if(ServoNo < g_pump_num)
	{
		int ActSpdRPM=0;
		ActSpdRPM = ActSpdPulse / ((fp64)(1<<g_pumpn[ServoNo]->mh->ENCODERESOLUTION)/60.0f);
		return(ActSpdRPM);
	}
	else
	{
		return 0;
	}
}


UI16 GetMotorMaxSpd(WORD servono)
{
	if(servono > 0)
	{
		servono--;
		PSERVOECAT pp = &m_servoecat;
		return pp->servo_max_rpm[servono];
	}
	else
	{
		return 0;
	}
}


void ServoErr()
{
    PSERVO  pp = &g_hydrservo;
	if(pp==NULL)
	    return;
	
	if(CheckPIUsed(pp->pi->I_SV_ERR))
	{
		if((CheckPI(pp->pi->I_SV_ERR) == PI_SET)&&!CHK_PART_ERR(pp, ER5_SVCTLERR))
		{
			if(MotorIsOn())	
			{
				SET_PART_ERR(pp, ER5_SVCTLERR);	
				StandAlarm(TRUE);
			}
		}			
	}
}

void    InitCtrlTableCnt(int lastpres,WORD portno)       //控制数据初始化
{
  g_press[portno].CntZeroSpeed    = 0;
  g_press[portno].LastPress = lastpres;
}	
	
void	InitServoPIDParaCold(WORD portno)   //冷启动PID参数初始化	
{
  int i;	
  for (i=0;i<MAXPRESSSETNUM;i++)
  {
    g_press[portno].PressSet[i].wData = 0;
    g_press[portno].FlowSet[i].wData  = 0;    
  }	
  g_press[portno].presspwr.ek   = 0;
  g_press[portno].presspwr.ek0  = 0;
  g_press[portno].presspwr.ek00 = 0;
  g_press[portno].pidpara.kd    = 0; 
  g_press[portno].FlowOut	= 0;
  g_press[portno].PressOut	= 0;
  g_press[portno].presflag    = ZEROHYDRD;

  InitCtrlTableCnt(0,1);
}
/*-----------------------------------------------------------------------------------------------------------------------------------------------+
|      获取输入输出数据对驱动器设置并记录(此函数就是将计算出的压力流量写到纪录表中然后发给驱动器输出)                                            |
+-----------------------------------------------------------------------------------------------------------------------------------------------*/
void    GetCtrlDataTable(WORD portno,int setflow,int setpres,int realpres)        //获取输入输出数据记录表
{
  g_press[portno].PressSet[0].wData = g_press[portno].PressSet[1].wData;         //上次输入压力纪录

  //获取计算出的当前需要输出的压力
  //并对压力指令进行惯性滤波
	//快速注射时，压力斜率走直线
  if(g_press[portno].PresSpdLimit == 0 && ChkSysCfg(CO1_SAAL))
  {
  	g_press[portno].PressSetF = setpres*100;
	g_press[portno].PressSet[1].wData = setpres;
  }
  else
  {
	g_press[portno].PressSetF = (g_press[portno].PressSetF*97/100+setpres*3);
  	g_press[portno].PressSet[1].wData = g_press[portno].PressSetF/100;  
  }


  g_press[portno].PressReal[0].wData = g_press[portno].PressReal[1].wData;       //实际压力纪录
  g_press[portno].PressReal[1].wData = realpres;                      //实际压力信号（压力传感器数值）纪录
  g_press[portno].FlowSet[0].wData = g_press[portno].FlowSet[1].wData;           //上次输入流量纪录
  g_press[portno].FlowSet[1].wData = setflow;                      //获取计算出的当前需要输出的流量
  g_press[portno].FlowReal[0].wData = g_press[portno].FlowReal[1].wData;         //实际输出转速纪录
  g_press[portno].FlowReal[1].wData = g_press[portno].FlowOut;                   //实际频率百分比来于PID计算
}

/*-----------------------------------------------------------------------------------+
|         获取液压关闭数据对驱动器设置并记录                                         |
|         实际检测压力小于5公斤300ms表示泄压完成，泄压完成函数返回0                  |
+-----------------------------------------------------------------------------------*/
WORD	GetZeroDataTable(WORD portno,int setflow,int setpres,int realpres)		  //获取液压关闭数据记录表
{
	WORD limit_pres = 50 + g_press[portno].ppump->mh->BASEPRES;
	
	g_press[portno].PressSet[0].wData = g_press[portno].PressSet[1].wData;		 //上次输入压力纪录

	//获取计算出的当前需要输出的压力
	//并对压力指令进行惯性滤波
	g_press[portno].PressSetF = (g_press[portno].PressSetF*97/100+setpres*3);
	g_press[portno].PressSet[1].wData = g_press[portno].PressSetF/100;  
	//g_press[portno].PressSet[1].wData = setpres;

	g_press[portno].PressReal[0].wData = g_press[portno].PressReal[1].wData;		 //实际压力纪录
	g_press[portno].PressReal[1].wData = realpres;					  //实际压力信号（压力传感器数值）纪录
	g_press[portno].FlowSet[0].wData = g_press[portno].FlowSet[1].wData;			 //上次输入流量纪录
	g_press[portno].FlowSet[1].wData = setflow;					   //获取计算出的当前需要输出的流量
	g_press[portno].FlowReal[0].wData = g_press[portno].FlowReal[1].wData;		 //实际输出转速纪录
	g_press[portno].FlowReal[1].wData = g_press[portno].FlowOut;					 //实际频率百分比来于PID计算

	if (setpres > g_press[portno].ppump->mh->BASEPRES)
	{
		g_press[portno].CntZeroSpeed = 0;
	}
	
	if ((g_press[portno].PressReal[1].wData<=limit_pres || g_press[portno].CntZeroSpeed > 0)&& (setpres <= g_press[portno].ppump->mh->BASEPRES) && (ActTskIsEmpty() || ActIsInTsk(MK_ACT(NOZZLE_ID,1, OS_COOL))))
	{ 															 //手动无动作情况下实际检测压力小于5公斤300ms表示泄压完成，泄压完成函数返回0
		g_press[portno].CntZeroSpeed ++;
		if (g_press[portno].CntZeroSpeed>100)
		{	
			g_pump.flowlmt=1;
			g_press[portno].CntZeroSpeed = 100;	
			g_press[portno].presflag = ZEROFLAG;
			return(0);
		}  
	}
	else if ((g_press[portno].PressReal[1].wData<=limit_pres || g_press[portno].CntZeroSpeed > 0) && (setpres <= g_press[portno].ppump->mh->BASEPRES))						 //手动状态下有动作切换或全自动半自动下，压力小于5公斤超过300ms就输出0转速，没超过300ms则输出	   
	{ 															 //5%的转速，压力控制在2公斤
		g_press[portno].CntZeroSpeed ++;
		if (g_press[portno].CntZeroSpeed>100)
		{	
			g_press[portno].CntZeroSpeed = 100;	
			g_press[portno].presflag = ZEROFLAG;
			return(0);
		}  
	}
	else															 //泄压未完成，函数返回1
	{
		g_press[portno].CntZeroSpeed	= 0;	
		return(1); 
	} 

	return 1;
}   

void    CalcPressMargin(WORD portno)                                                                    //压力差计算  用于PID计算
{
  g_press[portno].presspwr.ek00 = g_press[portno].presspwr.ek0;	                                        //保存上上次压力差值
  g_press[portno].presspwr.ek0  = g_press[portno].presspwr.ek ;	                                        //保存上次压力差值
  g_press[portno].presspwr.ek   = g_press[portno].PressSet[1].wData-g_press[portno].PressReal[1].wData; //压力差值	= 设定压力 - 实际压力  设定压力是由RAMP计算出来的  
}

void    InitServoPIDDefault(WORD portno)      //最主要的功能是请积分量
{
    PSERVO  pp = &g_hydrservo;
	/*=========================================
	|控制模式设定
	=========================================*/
	if ((pp->mh->KK_PRESSRAMP<1)||(pp->mh->KK_PRESSRAMP>3))                   //默认控制模式都是3主机CAN协议
		pp->mh->KK_PRESSRAMP=3;
  g_press[portno].ControlMode = pp->mh->KK_PRESSRAMP;
	/*=========================================
	|减速步幅设定
	=========================================*/
  g_press[portno].DecelerationStep = DECELERATIONSTEP;	//每周期1.2%,41ms
  g_press[portno].AccelerateSetp = ACCELERATESTEP;	//每周期1.5%,50ms 从0->10000
	/*=========================================
	|最大反向速度设定
	=========================================*/

	if (g_press[portno].ReverseFlowMax > 5000)
	{
		g_press[portno].ReverseFlowMax = 1000;
	}

	//默认泄压转速10%
	if (!ChkSysCfg(CO8_YL1))
	{
		if (g_press[portno].ReverseFlowMax < 1000)
		{
			g_press[portno].ReverseFlowMax = 1000;
		}//默认泄压转速10%
	}
	/*=========================================
	|控制周期(以及采样周期)设定
	=========================================*/
  if (pp->mh->WSERVO_TC != 5)
  {
	  pp->mh->WSERVO_TC = 5;//100us
  }
  
  g_press[portno].CycleTime = CYCTM_US/100;	//100us

  g_press[portno].pidpara.ki=g_press[portno].pidpara.p*g_press[portno].CycleTime*PID_KI_ACC/g_press[portno].pidpara.ti/10;//计算出伺服计算的KI参数
  g_press[portno].presspwr.ik = 0;                                                                          //初始化积分量
  g_press[portno].presspwr.ek0 = 0;
}

WORD GetkpDefault()
{
  PSERVO  pp = &g_hydrservo;
  if(pp->mh->WSERVO_K==0)   //备用P参数保护
  	pp->mh->WSERVO_K = 210;
  return  pp->mh->WSERVO_K;
}

WORD GetkiDefault()
{
  PSERVO  pp = &g_hydrservo;
  if(pp->mh->WSERVO_T ==0)  //备用I参数保护
  	pp->mh->WSERVO_T = 10;
  return  pp->mh->WSERVO_T;
}

void	InitServoPIDPara(WORD portno)                //PID参数初始化 清除积分量
{
  PSERVO  pp = &g_hydrservo;
  g_press[portno].pidpara.p =pp->mh->WSERVO_K;
  g_press[portno].pidpara.ti=pp->mh->WSERVO_T;
  g_press[portno].pidpara.td=0; 
  g_press[portno].pidpara.kd=0; 
  InitServoPIDDefault(portno); 
  g_press[portno].PressOut	= 1;
}

void    SetServoPIDPara(WORD p,WORD i,WORD d,WORD portno)       //PID参数初始化 清除积分量
{
  g_press[portno].pidpara.p =p;
  g_press[portno].pidpara.ti=i;
  g_press[portno].pidpara.td=d;	
  g_press[portno].pidpara.kd=0; 
  InitServoPIDDefault(portno); 
  g_press[portno].PressOut  = 1;
}

void    SetServoPIDPara_NoInit(WORD p,WORD i,WORD d,WORD portno)//PID参数初始化 不清除积分量
{
  g_press[portno].pidpara.p =p;
  g_press[portno].pidpara.ti=i;
  g_press[portno].pidpara.ki=g_press[portno].pidpara.p*g_press[portno].CycleTime*PID_KI_ACC/g_press[portno].pidpara.ti/10;
  g_press[portno].pidpara.td=d;	
  g_press[portno].pidpara.kd=0; 
  g_press[portno].presspwr.ik = ((long)(g_press[portno].presspwr.pk*100) - (long)(g_press[portno].pidpara.p*g_press[portno].presspwr.ek))*PID_KI_ACC;
  g_press[portno].presspwr.ik =(g_press[portno].presspwr.ik>0)?g_press[portno].presspwr.ik:0;
}

BOOL ServoTest()
{
	if(ChkSysCfg(CO0_SERVOP)||ChkSysCfg(XF6_CANSRV)||ChkSysCfg(XF6_HSERV0))
		return TRUE;
	else
		return FALSE;
}
/*---------------------------------------------------------------------------+
PID油压控制    
1.	*MoveLocation:
2.	PresRampOK:					1: 斜率结束标记
3.	Flag_FirstAction: 	1: 动作启动标记
4.	Flag_TaskType:			PIDINJHOLD		0x0001	注射保压类型
5.	MaxReverseFlow:			MAXREVERSEFLOW		3000		//	最大反向速度
6.	Flag_PIDPresDown:		1:PID控制泄压;0:非泄压	
+---------------------------------------------------------------------------*/
void	OilPIDControl(PRESS_STRU	*p_PressStr,WORD portno)
{
	long int   	Delti;
	long int 	pk_buff;
	PHYDR         ph = &g_hydrout;

	//压力超调后开始控制
	if(p_PressStr->PresOverCtrl == 1)
	{
		if(p_PressStr->presspwr.ek > 0)
		{
			p_PressStr->FlowOut = p_PressStr->FlowOutMax;
			return;
		}
		else
		{
			p_PressStr->PresOverCtrl = 2;
		}
	}

	/***************************************************************************
	|当前最大流量
	***************************************************************************/
	/***************************************************************************
	|当前次的积分
	***************************************************************************/
	if(ph->press_start == 1 && p_PressStr->presflag != ZEROHYDRD)
	{
		Delti = 0;
	}
	else
	{
		Delti=(long int)(p_PressStr->pidpara.ki*p_PressStr->presspwr.ek);
	}
	ph->press_start = 0;

	p_PressStr->pidpara.kd=0;//g_press[portno].pidpara.p*(GetSys()->pclmp->pcore[5]->hy->SERVO_KP_CORE)/(g_press[portno].CycleTime); 

	p_PressStr->presspwr.ik=p_PressStr->presspwr.ik+Delti;
  
	p_PressStr->presspwr.ik = (p_PressStr->presspwr.ik>0)?p_PressStr->presspwr.ik:0;
	
	p_PressStr->presspwr.pk=(long int)(p_PressStr->pidpara.p*p_PressStr->presspwr.ek)
	                      + p_PressStr->presspwr.ik / PID_KI_ACC
	                      + p_PressStr->pidpara.kd*(p_PressStr->presspwr.ek-p_PressStr->presspwr.ek0);

	p_PressStr->presspwr.pk=p_PressStr->presspwr.pk/100; 

	pk_buff = p_PressStr->presspwr.pk;
	/***************************************************************************
	|油压控制的输出限制
	***************************************************************************/ 

	if (!ChkSysCfg(CO1_SAAL))
	{
		if (p_PressStr->presspwr.pk>(p_PressStr->FlowReal[1].wData+g_press[portno].AccelerateSetp))              //输出功率限制，流量的上升步幅为最大每周期3%
		{ 
			p_PressStr->presspwr.pk=p_PressStr->FlowReal[1].wData+g_press[portno].AccelerateSetp;                    //计算功率超过3%，按照3%输出
		}
	}
	
	if (p_PressStr->presspwr.pk>p_PressStr->FlowOutMax)                            //计算出的流量对应的功率不能超过设定的流量对应的功率
	{
		p_PressStr->presspwr.pk=p_PressStr->FlowOutMax;                               //超过按照设定的流量对应功率输出
	}
	else
	{
		if (p_PressStr->presspwr.pk<(p_PressStr->FlowReal[1].wData-g_press[portno].DecelerationStep)) //减速步幅保护
		{
			p_PressStr->presspwr.pk=p_PressStr->FlowReal[1].wData-g_press[portno].DecelerationStep;
		}  

		if (p_PressStr->presspwr.pk<(0-g_press[portno].ReverseFlowMax))                       //最大反向速度保护
		{
			p_PressStr->presspwr.pk=0-g_press[portno].ReverseFlowMax;
		}
	}    

	if(pk_buff != p_PressStr->presspwr.pk && ((!ChkSysCfg(CO8_FLPMP)) || (p_PressStr->presspwr.ek > 0)))
	{
		p_PressStr->presspwr.ik = ((long int)(p_PressStr->presspwr.pk*100) - (long int)(p_PressStr->pidpara.p*p_PressStr->presspwr.ek))*PID_KI_ACC;
	}

	/***************************************************************************
	|函数返回输出流量 并显示实际流量
	***************************************************************************/ 
	p_PressStr->presspwr.ik =(p_PressStr->presspwr.ik>0)?p_PressStr->presspwr.ik:0;
	p_PressStr->FlowOut = p_PressStr->presspwr.pk;

	p_PressStr->presspwr.pk0 = p_PressStr->presspwr.pk;
	p_PressStr->presspwr.ik0 = p_PressStr->presspwr.ik;

	if(portno < ARRAY_NUM(g_hydrservo.st->SERVO_FLOWSET))
	{
		g_hydrservo.st->SERVO_FLOWSET[portno] = (p_PressStr->FlowOut+g_press[portno].ReverseFlowMax)/100;
	}

}

/*---------------------------------------------------------------------------+
油压控制工程
模式1：单PID调节
模式2：强PID->超调，并回调->弱PID
模式3：弱PID->超调->强PID->回调->弱PID
			 弱PID->速度到->强PID->超调，并回调->弱PID
模式4：
模式控制参数：KK_PRESSRAMP	0x018F
+---------------------------------------------------------------------------*/
int	OilControlProc(WORD portno,int SetFlow,int SetPres,int RealPres)
{
    PSERVO  pp = &g_hydrservo;
	
	if (g_press[portno].presflag == ZEROFLAG)											  //泄压后零扭矩转速输出过程控制
	{
		if(ChkSysCfg(XF5_PUMP1))
			SetPOBit(pp->po->O_SV_REL,PC_ON);
		else
			SetPOBit(pp->po->O_SV_REL,PC_OFF);

//		g_press[portno].PressOut = g_press[portno].ppump->mh->BASEPRES;
//		g_press[portno].FlowOut = g_press[portno].ppump->baseflow*HYDRADOLD;

		g_pump.flowlmt=1;

		if(g_press[portno].ppump->mh->BASEPRES == 0)
		{
			return 0;
		}
		else
		{
			GetCtrlDataTable(portno,SetFlow,SetPres,RealPres); 
		}
	}
	else if (g_press[portno].presflag == ZEROHYDRD || g_press[portno].presflag == ZEROHYDRD1) 									  //泄压处理
	{
		if(ChkSysCfg(XF5_PUMP1))
		{
			SetPOBit(pp->po->O_SV_REL,PC_OFF);
		}
		else
		{
			SetPOBit(pp->po->O_SV_REL,PC_ON);
		}

		if (GetZeroDataTable(portno,SetFlow,SetPres,RealPres)==0) 
		{
			if(g_press[portno].ppump->mh->BASEPRES == 0)
			{				
				return 0;										   //获取液压关闭斜率数据  泄压完成进入0压力控制 没完成继续泄压
			}
		}
	}
	else
	{	 
		if(ChkSysCfg(XF5_PUMP1))
		{
			SetPOBit(pp->po->O_SV_REL,PC_ON);
		}
		else
		{
			SetPOBit(pp->po->O_SV_REL,PC_OFF);
		}
		GetCtrlDataTable(portno,SetFlow,SetPres,RealPres);									//获取输入输出数据记录表

		//当正常速度控制时且非泄压动作，设定速度为0时输出0
		if (SetFlow == 0 && (g_press[portno].PressReal[1].wData <= (50 + g_press[portno].ppump->mh->BASEPRES)))
		{
			return 0;
		}	
	}

	CalcPressMargin(portno);		                                                        //压力差计算
	
	g_press[portno].FlowOutMax = g_press[portno].FlowSet[1].wData;                              //本次计算所得流量输出为最大流量输出
	OilPIDControl(&g_press[portno],portno);		                                                //非稳态，进入PID计算输出流程

	if(g_press[portno].FlowOut == SetFlow)
	{
		g_press[portno].PresCtrlValid = FALSE;
	}
	else
	{
		g_press[portno].PresCtrlValid = TRUE;
	}

	return g_press[portno].FlowOut;
}

int	Minimum(int num1,int num2)
{
	return((num1<num2)?num1:num2);
}

BOOL InitServoPart()
{
	PSERVO pp = &g_hydrservo;
	
	static const SUB_INIT_INFO sub_init[]  = SERVO_INIT_INFO;
									     
	if(!INIT_PART(pp, SERVO_ID, 1, sub_init))
		return FALSE;

	pp->mh->KP_TRANSITION = 100;
	pp->mh->KI_TRANSITION = 20;
	return TRUE;
}

void SetServoSpeed(UI16 portno, UI16 speed)
{
	if(portno < ARRAY_NUM(g_hydrservo.st->SERVO_FLOWSET))
		g_hydrservo.st->SERVO_SPEED[portno] = speed;
}

void	SetEcatInitStep(DWORD EcatInitStep)
{
	DWORD tmp_A10usclock;
	PSERVO	p_servo = &g_hydrservo;
	CLR_PART_ERR(p_servo, 0XFFFFFFFF);
	if ((EcatInitStep != ER1_NETSTEP1) && (EcatInitStep != ER1_NETSTEP2))
	{					
		SET_PART_ERR(p_servo, EcatInitStep);
	}
	
	PartDataTx((PPART)p_servo, ST_ID, 4, GET_IND(PSERVO_STATE, FL_ERROR0), TRUE);
	tmp_A10usclock  = A_10usclock;
	while((A_10usclock-tmp_A10usclock)<2500)
		ProtTaskLoop();
}
void	SetNetErrCnt(WORD NetErrCnt)
{
	if (NetErrCnt > 7) //共8个警报
		NetErrCnt = 0;
	SetEcatInitStep(ER1_NETSTEP1<<NetErrCnt);
}

void ClrNetErr()
{
	PSERVO	p_servo = &g_hydrservo;
	CLR_PART_ERR(p_servo, 0XFFFFFFFF);
}

void	SetNetErrReconfig()
{
	SetEcatInitStep(ER1_ECATRECFG);
}


