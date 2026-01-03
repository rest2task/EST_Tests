/******************************************************************************
  文 件 名   : main.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 主函数，初始化及主循环
  函数列表   :
              main
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
#include "machapi.h"
#include "canhead.h"
#include "memtbl.h"
#include "actctrl.h"
#include "prot.h"
#include "systempro.h"
#include "printf.h"
#include "keycmd.h"
#include "ioproc.h"
#include "actctrl.h"
#include "alarm.h"
#include "ZeroDebug.h"
#include "dabios.h"
#include "monmmi.h"
#include "acttbl.h"
#include "cfgdb.h"
#include "hydr.h"
#include "inject.h"
#include "safedoor.h"
#include "can_e702.h"
#include "clamp.h"
#include "canhead.h"
#include "Lub.h"
#include "config.h"
#include "app_data.h"
#include "EtherCATProc.h"
#include "EleCtrlApp.h"
#include "Servo.h"
#include "ServoEcat.h"
#include "TempCtrlComm.h"
#include "hwmid.h"
#include "AutoRunWithOutServo.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_gpt.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "gpt_timer.h"
#include "bsp_led.h"   
#include "bsp_uart.h" 
#include "canhead.h"
#include "cpld.h"
#include "bsp_key_it.h"
#include "bsp_nvic.h"
#include "wdog.h"
#include "fsl_semc.h"
#include "bsp_systick.h"
#include "malloc.h"
#include "udpctrl.h" 
#include "lwip_comm.h" 
#include "dm9000.h"
#include "ServFive.h"


int ecat_master_init(void);
void ecat_slave_reconfig(void);
void PumpStru(void);
void AD_start(void);
UI16 m_get_hwtype(void);
void CycleTmOutCount(UI32 ustick_10us);
void SetEcatInitStep(DWORD EcatInitStep);
void InitSeqVlvMain(void);
void cpld_card_chk(void);
BOOL SpeedDefInitPro(void);


extern UI16 g_MemSize;


int main()
{
  	uint32			 	ustick_jiffies;	
	uint32				ustick_10us;	

  	HwInitial();				 									/*硬件系统参数初始化  -增加初始化LM73 -WD1011*/
	mprintf("System start\r\n");	
	
	Zerodata_Load();
	SetRunFlag(INIF_INITPART);
	InitPartTypeInfo();
	InitMemTbl();
	TBLInit();
	BootInitCfgDb();
	ProtInit();	
	InitBaseCtrl();
	InitMachinePart();
	ActCtrInit();
	LoadPartConfig();
	InitMachineVersion();
	InitActTbl();
	CanInit();
	InitSdoProcess();
	delay_ms(15);//DA初始化后需要等待再输出
	InitHydrData();
	
	/*先启动一次AD采样*/
	AdMeterLoopdly(5);
	LoopPI();														/*获取IO输入端口状态*/
	SetRunFlag(INIF_COLD);
	LoadInnerPara();
	cpld_card_chk();	
    MotorOffOnce(); //上电强制输出一次马达停，避免普通机自锁时，主机异常重启后马达还处于使能状态
	//#if BOGUS
		ClrRunFlag(INIF_USEECAT);
	//#else
	if (!ChkSysCfg(XF6_EFFC))//开马达连接Ecat
	{
		if(ChkSysCfg(CO1_ECAT))
		{
			SetRunFlag(INIF_USEECAT);
		}
		else
		{
			ClrRunFlag(INIF_USEECAT);
		}
	}
	
	//#endif
	if(ChkRunFlag(INIF_USEECAT))
	{
		SetEcatInitStep(ER1_NETINSTART);	/*通知面板启动Ecat网络初始化*/
	/*========Ecat网络初始化===============================*/
		ecat_master_init(); 				//EtherCAT Init
	/*----------Ecat网络初始化完成----------------------------------*/		
		while (get_jiffies() % pdo_cycle_period());
		ecat_master_set(1);

		SetEcatInitStep(0);
	}
#ifdef DM9000_ENABLE
		my_mem_init(SRAMIN);//(SRAMIN); 
		my_mem_init(SRAMEX);//(SRAMEX); 
		//lwip_test_ui(1);			
		mprintf("lwip_comm_init Begin....\n\r");
		while(lwip_comm_init())  
		{
			mprintf("LWIP Init Falied!");
			delay_ms(500);
			mprintf("Retrying...\n\r");
			break;
		}
		//lwip_test_ui(2);		
		delay_ms(500);			
		delay_ms(500);
		mprintf("am335x_udp_init begin;=======\n\r");  
		m_Udp_init();
		mprintf("am335x_udp_init done;========\n\r");
#endif 

	StateMappingInit();	
	SpeedDefInitPro();
	
	ClrRunFlag(INIF_INITPART);
    SetRunFlag(INIF_ETHERCAT);
	
	mprintf("System init end\r\n");	
	mprintf("System ram use %d/%d B\r\n",g_MemSize,DYNC_MEM_SIZE);	
	/*========初始化完成进入正常执行程序=========*/	
	while(TRUE)														
	{		
        //======时间变量赋值=================
        ustick_jiffies = jiffies;
		ustick_10us = A_10usclock;		
		
		AdMeterLoop();					/*涉及AD转换 获取电子尺位置, 0.xxxms*/
		if(!ChkMachineErr(ER0_MEM))
		{
			ActTaskLoop();										/*动作调度及运行,0.xxxms*/
		}
		if(ChkRunFlag(INIF_RESET) || ChkMachineErr(ER0_MEM))		/*按下紧停开关*/
		{
			if (ChkSysCfg(CO1_EMERG))					
			{
				Get_Temp(TRUE);										//强制进行温度扫描和控制
			}
			LoopPOOnOff();											 //输出状态置位
			CanProcess();                       					/*CAN协议*/			
			AlarmLoop();											/*警报循环检查*/ 
			MMIMonLoop();	
			ComStateProc();
		}
		else if(ComIsOk())
		{
			if(!ChkRunFlag(INIF_CHKPC))								//强制输出状态检查(如果系统处于强制输出时,系统运行的输出点就不输出)
				LoopPOOnOff();
			if(DAIsAdjust())
				E702_CanTransmitDelay();
			
			LoopHydrauic();										/*液压控制*/		
			
			if(!ChkSysCfg(CO0_SERVOP) && ChkSysCfg(XF6_CANSRV))
				ServoPIDCtrlProc();	

	
			CanProcess();                       					/*CAN协议*/
			Get_Temp(FALSE);										/*温度数据有效时进行温度控制*/			
			MachineCmd(MH_CMD_SFDR_CH, 0);							//安全门检查
			AlarmLoop();											/*警报循环检查*/ 
			KeyScanProc();											/*按键任务*/
			MachineCmd(MH_CMD_LOOP_CHK, 0);							//机械手处理 欧规机械手动作进程， 模具急升 氮气增压循环
			ZeroDebug();											/*DA及热电偶温度校准以及LM73温度数据捕获，并重新计算温度系数，系数的保存部分需要更改*/  
			MMIMonRecLoop();	
			MMIMonLoop();	
        } 
		else
		{
			ComStateProc();
			LoopPOOnOff();			
			LoopHydrauic();										/*液压控制*/
			
			if(!ChkSysCfg(CO0_SERVOP) && ChkSysCfg(XF6_CANSRV))
				ServoPIDCtrlProc();	

			CanProcess();                       					/*CAN协议*/
			/*通讯中断后，还需对温度控制，
			防止面部死机后，料筒被一直加温,温度不受控*/
			if(!ChkRunFlag(INIF_COLD))
			{
				Get_Temp(FALSE);										/*温度数据有效时进行温度控制*/
			}
		}
		
		if(ChkSysCfg(CO5_SDRVALVE))
		{
			if(!ChkRunFlag(INIF_COLD))
			{
				MachineSafeCheckPro();
			}
		}
		
        IOVErr();
		SetOperTM();
		FeedDog();													/*喂狗,2秒没喂狗就主机重启*/
		LedFlash();													/*LED刷新*/		
		Can2Loop();	
		LoopPI();													/*获取IO输入端口状态*/
		GetEmergKey();												/*急停及手动键处理*/
		DaOutPortEnable(ChkRunFlag(INIF_RESET) == 0);				/*紧停或通讯未正常状态下，DA不输出*/
		//Read_Curtemp();
		//= 发送请求E702的地址数据，用于最大值和最小值的显示和调整[硬件校准和DA校准画面] ==
		E702_DataRequest();
		ProtTaskLoop();	/*通信协议处理*/
/**************************************************/		
		if(ChkOperMode(OM_MANUAL))
		{
			MainProtErr();
			if(ChkRunFlag(INIF_CFGDBCHG))
			{
				ChkHWStor();
				LoadTblConfig();
				InitActTbl();
				ClearAllPOOut();	//新增清除所有输出，为了修复再已有输出的点上重定义一个新的无输出点时，该点保留了输出状态的问题。
				ClrRunFlag(INIF_CFGDBCHG);
			}
		}
		
	
#ifdef DM9000_ENABLE
        m_Udp_ISRHandler();    //由于不使用中断，所以要在这里清标志位
		am335x_udp_process();   //udp网络数据处理
#endif	
		if(!ChkRunFlag(INIF_COLD))
		{
			ListSaveCfgDb();
			PollStepFunc();	
		}
		 
		//CycleTmOutCount(ustick_10us);

		while((WORD)(A_10usclock - ustick_10us) <= 30);
		
		if(ChkRunFlag(INIF_USEECAT))
		{
			/*EtherCAT通讯相关的操作*/
			SetEcatOpFlag(0);
			EcatReceive();
			if (GetEcatNetState() == 1)	/*GetEcatNetState()=1:网络初始化正常*/
			{
				ecat_slave_reconfig();    /*	从站断开重新配置*/
			}
			if (ecat_rec_flag)
			{
				SetEcatOpFlag(1);
				ecat_rec_flag = 0;
			}
		}		

        if(!ChkRunFlag(INIF_USEECAT))
		{
			PumpStru();
		}

        CycleTmOutCount(ustick_10us);

        if (CHK_MOTOR_MODE(MOTOR_CTRL_ECAT))
        {
            while (((UI32)(jiffies - ustick_jiffies) < pdo_cycle_period()) && (get_jiffies() % pdo_cycle_period()))
    		{
            	if(ChkRunFlag(INIF_USEECAT))
            	{
                	EcatCtrlOper();		/*	等待反馈数据-->计算输出量-->搬运数据*/
            	}
    		}
        }
        else
        {
             if((WORD)(A_10usclock - ustick_10us) < MAX_LOOP_10US)
    		{		
    			while((WORD)(A_10usclock - ustick_10us) < MAX_LOOP_10US)
    			{
    				if((WORD)(A_10usclock - ustick_10us) >= MAX_LOOP_10US)
    					break;
    			}
    		}
        }        
		
		if(ChkRunFlag(INIF_USEECAT))
		{
			if (GetEcatOpFlag() < 2)
	        {
	            EcatSend();
	        }

			EtherCatTest((jiffies - ustick_jiffies) > 51);
		}		
	}
}


