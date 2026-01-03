/*===========================================================================+
|  Class    : Move control process                                           |
|  Task     : Move control utility process                                   |
|----------------------------------------------------------------------------|
|  Compile  : CC'C2000 -                                                     |
|  Link     : CC'C2000 -                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Xiaqinghua                                                     |
|  Version  : V1.03                                                          |
|  Creation : 06/10/2009                                                    |
|  Revision : 07/09/2009                                                               |
+===========================================================================*/

#include    "movectrl.h"
#include    "hydr.h"
#include    "servo.h"
#include    "configdf.h"
#include    "machine.h"
#include    "servo.h"
#include		"pump.h"
#include	"systempro.h"
#include    "ServFive.h"


/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
MOVEDPCH_STRU    g_Dispatch[5];
MOVE_STRU        p_WorkStru[5];
MOVE_STRU        g_NullUnit;
extern  SERVO            g_hydrservo; 

BOOL DAIsAdjust(void);
void movedispatchproc(void);

/*---------------------------------------------------------------------------+
|           code segment                                                     |
+---------------------------------------------------------------------------*/     

 //此函数用于动作液压控制列表设定 在动作需要设定时调用
 //在动作设定的时候注意pramp,framp,prampstart,frampstart的设定
void   SetMoveUnitDataList(WORD mode,WORD pres, WORD flow,WORD pramp,WORD framp,WORD prampstart,WORD frampstart,WORD hydrdly,
												WORD hydrport,WORD ServoPumpLmt,WORD NormalPumpLmt,WORD Servo_P,WORD Servo_I,WORD PresFlag,WORD KiClear,WORD Priority,WORD Speed_tretching_hide)
{
	extern HYDR g_hydrout;
	extern PUMP_STRU_5  g_pump_5;
	int i;
	WORD port,list;

	WORD ServoPumpLmtbuf = 0;
	for(i=0;i<5;i++)
	{
		if(Priority == 0xffff)
			list = 0;
		else
		{
		  list=((Priority>>(i*2))&3)+1;

		  
		  if((!ChkSysCfg(CO0_SERVOP)&&ChkSysCfg(XF6_CANSRV)) && (PresFlag != BACKPFLAG))	
     	      g_pump_5.svrflag |= hydrport;
		}
		
		if((hydrport>>i)&1)
		{
			port=i+1;

			if(ChkRunFlag(INIF_USEECAT))
			{
				if(ServoPumpLmt > 0)
				{
					ServoPumpLmtbuf = ServoPumpLmt;
					
					ServoPumpLmtbuf = ServoPumpLmtbuf & hydrport;	//得出主站与从站重叠部分
					ServoPumpLmt = ServoPumpLmt ^ ServoPumpLmtbuf;	//得出纯正的从站
				}				
			}

			g_Dispatch[port].List[list].CtrlMode = mode;		//设定控制模式	主要区分速度控制及位置控制
			g_Dispatch[port].List[list].PressSet     = pres;	                     //设定压力
			g_Dispatch[port].List[list].PressRamp    = pramp;	                    //压力斜坡 非起压过程压力转换斜率  

			g_Dispatch[port].List[list].SpeedSet     = flow;	 	           				 //设定速度
			g_Dispatch[port].List[list].SpeedRamp    = framp;		       				    //速度斜坡 非起压过程流量转换斜率

			g_Dispatch[port].List[list].HydrDlyTime  = hydrdly;	                 //液压延迟时间

			g_Dispatch[port].List[list].HydrPortNo   = port;		               //液压通道编号
			g_Dispatch[port].List[list].ServoPumpLmt    = ServoPumpLmt;		       // 伺服系统的泵浦配置
			g_Dispatch[port].List[list].NormalPumpLmt   = NormalPumpLmt;		       // 定量变量泵的泵浦配置
			g_Dispatch[port].List[list].KiClear         = KiClear;		             // 积分清除标记  
			
			if (Speed_tretching_hide > 150)
				Speed_tretching_hide = 0;
			g_Dispatch[port].List[list].speed_tretching_hide = Speed_tretching_hide;
			

	
			if (p_WorkStru[port].MoveStatus == 1)                                     //判断是否为起压过程
			{
				g_Dispatch[port].List[list].PressRamp    = prampstart;	                     
				g_Dispatch[port].List[list].SpeedRamp    = frampstart;		       			      
			}

			if((Servo_P > 2500) || (Servo_P == 0))     //对所有动作的设定P值做范围限制保护     20170418
				Servo_P = 800;
			if((Servo_I > 200) || (Servo_I == 0))      //对所有动作的设定P值做范围限制保护     20170418
				Servo_I = 40;
			g_Dispatch[port].List[list].PIDPara_P   = Servo_P;
			g_Dispatch[port].List[list].PIDPara_I   = Servo_I;
			g_Dispatch[port].List[list].presflag   = PresFlag;
			g_Dispatch[port].Fuction[list] = 1;

			
			if(ChkRunFlag(INIF_USEECAT))
			{
				ServoPumpLmt = 0;	//第一个油路为主油路，后面的油路都不跟随从站
			}
		}
  }

	//0xffff 表示关闭控制通道，关闭时优先对优先级函数进行处理，将关闭操作优先执行。
	//否则在卓越主从控制时，出现主泵从泵运行后立刻将原从泵以主泵的形式切掉。
	if (Priority == 0xffff)
	{
		movedispatchproc();
	}
}


void   SetMoveUnitData_Da_pres(WORD mode,WORD pres,WORD pramp, WORD prampstart, WORD hydrdly,
												WORD hydrport,WORD NormalPumpLmt,WORD PresFlag,WORD KiClear,WORD Priority,WORD Direct,WORD Speed_tretching_hide)
{
	int i;
	WORD port;
	WORD  ghy_no = 0;

	for(i=0; i<MAXDAPORT; i++)
	{		
		if((hydrport>>i)&1)
		{
			port=i+1;
			p_WorkStru[port].mode = mode;
			p_WorkStru[port].PressSet		 = pres;				//设定压力
			p_WorkStru[port].PressRamp		 = pramp*100; 	//压力斜坡 1ms  
			p_WorkStru[port].PressLast		 = p_WorkStru[port].PressCurrent; 							 //上级动作压力 
			p_WorkStru[port].PressCurrent100b	 = p_WorkStru[port].PressCurrent*ACCURACY; 
	
			p_WorkStru[port].HydrDlyTime	 = hydrdly;			//液压延迟时间
			p_WorkStru[port].HydrPortNo 	 = port;		//液压通道编号
			p_WorkStru[port].NormalPumpLmt	 = NormalPumpLmt; 	// 定量变量泵的泵浦配置	 	
	  		p_WorkStru[port].presflag       = PresFlag;
			p_WorkStru[port].KiClear 		 = KiClear;
			p_WorkStru[port].Direction_P = Direct;
			
			if (p_WorkStru[port].MoveStatus == 1)              //判断是否为起压过程
			{
				p_WorkStru[port].PressRamp    = prampstart*100;	                            			      
			}
				
			if (p_WorkStru[port].PressRamp>0)	
			{
				p_WorkStru[port].PressRampOK = 0;

				if(p_WorkStru[port].PressRamp>60000)
			   		p_WorkStru[port].PressRamp=60000;

				if(port > 0)
					ghy_no = port - 1;   
				 p_WorkStru[port].PressRampbit = (MAX_LOOP_100US*ACCURACY*g_hydric[ghy_no].limit)/p_WorkStru[port].PressRamp;	
			} 
			else
			{
				p_WorkStru[port].PressRampOK = 1;
				p_WorkStru[port].PressCurrent = p_WorkStru[port].PressSet; 
				p_WorkStru[port].PressCurrent100b    = p_WorkStru[port].PressCurrent*ACCURACY; 
			}
				
			p_WorkStru[port].MoveStart100us = Get100UsTick();  
		}
  	}
}

void   SetMoveUnitData_Da_flow(WORD mode,WORD flow,WORD framp,WORD frampstart,WORD hydrdly,
												WORD hydrport,WORD NormalPumpLmt,WORD PresFlag,WORD KiClear,WORD Priority,WORD Direct,WORD Speed_tretching_hide)
{
	int i;
	WORD port;

	for(i=0; i<MAXDAPORT; i++)
	{		
		if((hydrport>>i)&1)
		{
			port=i+1;
			p_WorkStru[port].mode = mode;

			p_WorkStru[port].SpeedSet		 = flow;						 //设定速度
			p_WorkStru[port].SpeedRamp		 = framp*100; 				 //速度斜坡 1ms
			p_WorkStru[port].SpeedLast		 = p_WorkStru[port].SpeedCurrent; 							   //上级动作速度
			p_WorkStru[port].SpeedCurrent100b	 = p_WorkStru[port].SpeedCurrent*FLOWACCURACY;  
			
			p_WorkStru[port].HydrDlyTime	 = hydrdly;			//液压延迟时间
			p_WorkStru[port].HydrPortNo 	 = port;		//液压通道编号
			p_WorkStru[port].NormalPumpLmt	 = NormalPumpLmt; 	// 定量变量泵的泵浦配置	 	
	  		p_WorkStru[port].presflag       = PresFlag;
			p_WorkStru[port].KiClear 		 = KiClear;
			p_WorkStru[port].Direction_F = Direct;
			
			if (p_WorkStru[port].MoveStatus == 1)              //判断是否为起压过程
			{                   
				p_WorkStru[port].SpeedRamp    = frampstart*100;		       			      
			}
		
			if (p_WorkStru[port].SpeedRamp>0)
			{
				p_WorkStru[port].SpeedRampOK = 0;	
				if(p_WorkStru[port].SpeedRamp>40000)
			   		p_WorkStru[port].SpeedRamp=40000;
				p_WorkStru[port].SpeedRampbit = MAX_LOOP_100US*FLOWACCURACY*FLWLMT/p_WorkStru[port].SpeedRamp;
			}
			else
			{
				p_WorkStru[port].SpeedRampOK = 1;
				p_WorkStru[port].SpeedCurrent = p_WorkStru[port].SpeedSet;
				p_WorkStru[port].SpeedCurrent100b    = p_WorkStru[port].SpeedCurrent*FLOWACCURACY;	 
			}  
							
			p_WorkStru[port].MoveStart100us = Get100UsTick();  
		}
  	}
}


/*     运动部件参数设定函数   接收传递过来的运动设定参数用于计算                                                 */
void   SetMoveUnitData(WORD portno,WORD Priority)
{
	int servono,actspd,mcpres,presramp,spdramp,pumplist,dely,KiClear;
	int presch;
	WORD mode,Mode = 0;
	WORD limitramp;
	WORD SpdDraw;
	WORD presflag;

	servono = portno - 1;

	if (g_pumpn[servono] == NULL)
		return;
	
	actspd = g_Dispatch[portno].List[Priority].SpeedSet;
	spdramp = g_Dispatch[portno].List[Priority].SpeedRamp * 10;
	mcpres = g_Dispatch[portno].List[Priority].PressSet;
	presramp = g_Dispatch[portno].List[Priority].PressRamp*10;
	dely = g_Dispatch[portno].List[Priority].HydrDlyTime;
	pumplist = g_Dispatch[portno].List[Priority].ServoPumpLmt;
	KiClear = g_Dispatch[portno].List[Priority].KiClear;
	mode = g_Dispatch[portno].List[Priority].CtrlMode;
	SpdDraw = g_Dispatch[portno].List[Priority].speed_tretching_hide;
	presflag = g_Dispatch[portno].List[Priority].presflag;

	if(presflag == INJDFLAG)
	{
		//如果是注射斜率，那么对斜率不受限制
		limitramp = 0;
	}
	else
	{
		//注射斜率以外动作，都需要限制斜率的
		limitramp = 1;
	}	

	if(!DAIsAdjust())
	{
		SetHydrDisplay(portno,mcpres/10,actspd);
	}

	if(g_pumpn[servono]->McCtrl.Fl_Pres == TRUE)
	{
		presch = g_pumpn[servono]->McCtrl.PresCtrlCh-1;
		
		g_pumpn[servono]->McCtrl.pMcPres->presflag = presflag;
		
		if(g_Dispatch[portno].List[Priority].KiClear == 1)
		{
			//清积分
			SetServoPIDPara(g_Dispatch[portno].List[Priority].PIDPara_P,g_Dispatch[portno].List[Priority].PIDPara_I,g_Dispatch[portno].List[Priority].PIDPara_D,presch);
		}
		else
		{
			//不进行清积分操作	
			SetServoPIDPara_NoInit(g_Dispatch[portno].List[Priority].PIDPara_P,g_Dispatch[portno].List[Priority].PIDPara_I,g_Dispatch[portno].List[Priority].PIDPara_D,presch);   
		} 
		
		InitCtrlTableCnt(g_pumpn[servono]->McCtrl.RealPres,presch); 	
	}
	else
	{			
		if(CHK_MOTOR_MODE(MOTOR_CTRL_5MODE))
		{		
			p_Work_5[portno].PIDPara_P_5  = g_Dispatch[portno].List[Priority].PIDPara_P;
			p_Work_5[portno].PIDPara_I_5  = g_Dispatch[portno].List[Priority].PIDPara_I;
			p_Work_5[portno].PIDPara_D_5  = g_Dispatch[portno].List[Priority].PIDPara_D;
			g_press[portno].pidpara.td = presflag;
			
			if(g_Dispatch[portno].List[Priority].KiClear == 1)	//首次设定时，发送PI和初始化can发送步骤
			InitServo_PISet(portno);
		}
	}

    if (ChkSysCfg(CO43_PfRamp))
    {
        Mode = Mode | 0x0100;
    }
	
	SetMotionControlOutPara(servono,mode, actspd, 0, mcpres, 0, 0, presramp, spdramp, 0, pumplist, Mode,dely,KiClear,limitramp,SpdDraw,presflag);
}

           
/*     运动位置定位控制函数                                                     */
WORD   MoveLocationMeterControlProc(int curposn,int endposn)
{
  WORD  RetData=0;
  if (curposn<0) curposn=0;
  if (curposn>=endposn) RetData = 1;				    
  return(RetData);
}

/*     运动时间定位控制函数     100us精度                                        */
WORD   MoveLocationTM100UsControlProc(DWORD worktime, DWORD start100us)
{
  WORD  RetData=0;
  if ((DWORD)((DWORD)Get100UsTick()-start100us) >= worktime)  RetData = 1;  				    
  return(RetData);
}

#if 0
/*     运动时间定位控制函数     1ms精度                                        */
WORD   MoveLocationTMMsControlProc(DWORD worktime, DWORD startms)
{
  WORD  RetData=0;
  if ((DWORD)((DWORD)GetMsTick()-startms) >= worktime)  RetData = 1;  				    
  return(RetData);
}

/*     运动时间定位控制函数     10ms精度                                        */
WORD   MoveLocationTM10msControlProc(WORD worktime)
{
  WORD  RetData=0;
  if ((WORD)((WORD)Get10MsTick()-p_WorkStru->MoveStart10ms) >= worktime)  RetData = 1;  				    
  return(RetData);
}
#endif

/*     运动开关定位控制函数                                                     */
WORD   MoveLocationNMControlProc(WORD endpb)
{
  WORD  RetData=0;
  if (TestPI(endpb)) RetData = 1;				    
  return(RetData);
}

/*     运动记数定位控制函数                                                     */
//      g_pbcnt.cnt = 0;                   need to be initial
//      g_pbcnt.cursts = testpb(endpb);
WORD   MoveLocationCntControlProc(PPINT_ST pst,WORD pbcnt)
{
  WORD  RetData=0;
  if(pst != NULL)
  {
	  PICntLoop(pst);
	  if (GetPICnt(pst)>=pbcnt) RetData = 1;
  }
  return(RetData);
  
}
