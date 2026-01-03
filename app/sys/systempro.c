/******************************************************************************
  文 件 名   : systempro.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 系统处理函数
  函数列表   :
              CalMsTo100MsUI16
              CalMsTo10MsUI16
              ChkRunFlag
              ClrRunFlag
              ComInitOK
              ComIsFail
              ComIsOk
              ComMMINextPart
              ComStateProc
              ComStateReset
              delay_ms
              delt_time
              GetRunFlag
              InitBaseCtrl
              LedFlash
              LoadPartConfig
              LoadTblConfig
              ManualInitProc
              ManualReset
              ResetSystem
              SendBootUpMsgToMMI
              SendKeyToMMI
              SetComState
              SetRunFlag
              SysAdjCfgCmdRx
              SysMemOutFatalErr
              SystemReset
              SystemRestart
              UpgradeCmdRx
  修改历史   :
  1.日    期   : 2013年5月17日
    作    者   : ren chaohong
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "common.h"
#include "systempro.h"
#include "machapi.h"
#include "hwcfg.h"
#include "keycmd.h"
#include "ioproc.h"
#include "admeter.h"
#include "motor.h"
#include "alarm.h"
#include "oil.h"
#include "dabios.h"
#include "actctrl.h"
#include "keycmd.h"
#include "prot.h"
#include "hydr.h"
#include "monmmi.h"
#include "cfgdb.h"
#include "hydr.h"
#include "da.h"
#include "protcmd.h"
#include "useriap.h"
#include "servo.h"
#include "inject.h"
#include "safedoor.h"
#include "adjust.h"
#include "canhead.h"
#include "machine.h"
#include "can_e502.h"
#include "act.h"
	 
#include "pump.h"
#include "TempCtrlComm.h"
#include "AlarmClass.h"
#include "TempCtrlMw.h"
#include "hwmid.h"
#include "dm9000.h"
#include "ServFive.h"


/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
extern UI16 FuncAiClampOpn;
extern UI16 g_actopen;
/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
void EjectCalibration(void);
void InjectCalibration(void);
void ServoRecoverActNoEn(void);
void TempTempRFStartPro(WORD password,WORD id,WORD ch);
void InitTemp(PTEMPCTRLCOMM pp);
void ClampCalibration(void);
void LubLastClmpUp(void);
void DA_OUTPUMP_SPDOUT(UI16 pres,UI16 spd,UI16 no);
void OilPortTbCopy(void);
WORD InChangeDaByMode(WORD value);
BOOL SendMonAlarmSt(UI16 imm);
void AdMeterSpeedInitPro(void);
void InitWarmFreeTime(void);


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



LED_STRU	g_led;
SYS_ST	sys_st;
UI16 sys_rcv_flag = 0;	//机器配置中的MH申请标记,最多只允许执行一次

extern HYDR      g_hydrout;
extern INNER_PARA inner_pa;
extern u32 g_sdo_abort_code;


/*硬件配置不在编译时选择，而在程序运行时选择，定义硬件配置变量*/

HW_CFG_VAR	hw_cfg = {MODEL_I3201, I3201_MAXADPORT, I3201_MAX_ADVAULE, I3201_PIMAX, I3201_POMAX, I3201_INN_MAXTEMPCHNL, I3201_MAXDAPORT};

void LedFlash()
{
	if (DIFF_100MS(g_led.cputime) >= 5) 	/*0.5s*/
	{
		g_led.cputime = Get100MsTick();
		if (g_led.cpusts)								/*转标志*/
		{
			g_led.cpusts  = 0;
			m_on_run_led() ;								/*RUN亮*/

			if (ComIsOk()) //通信正常
			{
				m_off_uart_led();							/*UART灭*/
			}
			if (g_led.upgrade)
			{
				m_on_canb_led();							/*CANB亮*/
			}
		}
		else
		{
			m_off_run_led();								/*RUN灭*/	
			g_led.cpusts  = 1;
			if (ComIsOk())
			{
				m_on_uart_led();							/*UART亮*/
			}

			if (g_led.upgrade)
			{
				m_off_canb_led();							/*CANB灭*/
			}
		}
	}
}


/*******************************************************************************************
  * @函数名称	systeminit
  * @函数说明   系统各参数初始化
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
 ****************************************************************************************/
BOOL InitBaseCtrl()
{
	InitIOPart();
	InitAdMeterPart();
	InitTempCtrlPart();
	InitMotorPart();
	InitOilPart();
	InitAlarmPart();
	InitHydrPart();
	InitDAPart();
	InitServoPart();
	InitServoEcatPart();
	InitPartDataChangeCache();
	InitListSaveCfgDb();	
	InitAlarmClassIndex();
	AdMeterSpeedInitPro();
	return TRUE;
}

void ResetSystem(void)	  		
{
	DaOutPortEnable(FALSE);

	MotorDown();

	if (!ChkSysCfg(CO1_EMERG))
	{
		GetSys()->md->CUSTOM_PARA[0] = 5;
	    WarmOff();
	}
	
	ManualInit();	
	AlarmSetEmerge(TRUE);
	ClearAllHydrOut(0);
}

void SystemReset()  //面板急停按下
{
	ResetSystem();
	SetRunFlag(INIF_RESET);
	SetMachineErr(ER0_PNLS);
	SetOperMode(OM_MANUAL);
    MachineCmd(MH_CMD_ROBOT_EM, TRUE);
}

void RestartSystem()
{
	DaOutPortEnable(TRUE);

	ClearAllPartError();
	ClrRunFlag(INIF_RESET);
	AlarmSetEmerge(FALSE);
	AlarmOff();
}

void SystemRestart()  //面板急停放开
{
    RestartSystem();
    MachineCmd(MH_CMD_ROBOT_EM, FALSE);
}

void LoadPartConfig()
{
	LoadCfgDb(USER_PART_DB);
}

void LoadTblConfig()
{
	LoadCfgDb(FACTORY_TBL_DB);
	LoadCfgDb(USER_TBL_DB);
}

void ComStateReset()
{
	sys_st.com_step = COM_INIT_START;

	sys_st.rx_mmi_part_cnt = 0;	
#ifdef DM9000_ENABLE
	sys_st.Udp_init_step = 0;
	sys_st.Udp_init_cnt = 0;
#endif
}

void ComInitOK()
{
	sys_st.com_step = COM_INIT_OK;
	if(ChkRunFlag(INIF_COLD)) //曲线初始化
	{
		LubLastClmpUp();
		inithydatacomisok();		
		MachineCmd(MH_CMD_ROBOT_EM,FALSE);	//用于刚开机时，就输出一下注塑机急停
	}
	
	ClrRunFlag(INIF_COLD);
	CreateActForkTbl(GetOperMode());

	VersionDiff();	

	if (FuncAiClampOpn)
	{
		AiTBLReqRx();	//Ai表请求
	}
	SendMonAlarmSt(0);
}

void ComMMINextPart()
{
	sys_st.cur_part = GetNextPartByInd(&sys_st.load_part_ind);

	//如果没有代码code 3:0x01   刚开机时，不请求挡板的相关数据，防止报警面板数据不全
	if(sys_st.cur_part != NULL && sys_st.cur_part->id == MK_PART(SHUT_ID, 1) && !ChkSysCfg(CO3_NFMI))
	{
		sys_st.cur_part = GetNextPartByInd(&sys_st.load_part_ind);
	}
	
	if(sys_st.cur_part == NULL && sys_st.req_retry > MAX_SUB_REQ_RETRY)
	{
		//sys_st.com_step = COM_BOOTUP_OK;
	}
	
	sys_st.last_req_tx_ms = 0;
	sys_st.req_retry = 0;
	sys_st.last_sub_bits = 0;
}

#ifdef DM9000_ENABLE
void Udp_Uart_Change(void)		//udp与232切换发启动信号函数
{
	if(sys_st.Udp_init_step > 4)
		sys_st.Udp_init_step = 0;
	
	switch(sys_st.Udp_init_step)
	{
		case 0:
			m_dm_init();
			sys_st.Udp_init_step++;
			sys_st.Udp_init_cnt = 0;
			break;
			
		case 1:
			m_on_cana_led();
			Udp_uart_flag = 1;		//udp数据发送3s
			sys_st.Udp_init_cnt++;
			if(sys_st.Udp_init_cnt > BOOT_VER_TRY)
			{
				sys_st.Udp_init_cnt = 0;
				MainProtReset();
				sys_st.Udp_init_step++;	
				m_off_cana_led();
			}
			break;

		case 2:					//中间必须要有时间等待，因为存在通讯的重发
			m_on_cana_led();
			m_on_canb_led();
			Udp_uart_flag = 2;
			sys_st.Udp_init_cnt++;
			if(sys_st.Udp_init_cnt > BOOT_VER_TRY)
			{
				sys_st.Udp_init_cnt = 0;
				MainProtReset();
				sys_st.Udp_init_step++;	
				m_off_cana_led();
				m_off_canb_led();
				UartInit(RS232_BR);
			}
			break;
			
		case 3:
			m_on_canb_led();
			Udp_uart_flag = 0;		
			sys_st.Udp_init_cnt++;
			if(sys_st.Udp_init_cnt > BOOT_VER_TRY)
			{
				sys_st.Udp_init_cnt = 0;
				MainProtReset();
				sys_st.Udp_init_step++;	
				m_off_canb_led();
			}
			break;
			
		case 4:

			m_on_cana_led();
			m_on_canb_led();
			Udp_uart_flag = 2;		
			sys_st.Udp_init_cnt++;
			if(sys_st.Udp_init_cnt > BOOT_VER_TRY)
			{
				sys_st.Udp_init_cnt = 0;
				MainProtReset();
				sys_st.Udp_init_step = 1;	
				m_off_cana_led();
				m_off_canb_led();
			}
			break;
			
		default:
			sys_st.Udp_init_step = 0;
			sys_st.Udp_init_cnt = 0;
			break;
	}	
}
#endif

void ComStateProc()
{
	UI16 sub_bits;
	if(sys_st.com_step == COM_INIT_OK)
		return;

	if(sys_st.com_step == COM_INIT_START)
	{
		if(GetMsTick() - sys_st.last_req_tx_ms > (MMI_ACK_TM*4) || sys_st.last_req_tx_ms == 0)
		{		
#ifdef DM9000_ENABLE
				Udp_Uart_Change();
				SendBootUpMsgToMMI();
				Data_Pc_Stop();
#else
				if(ChkRunFlag(INIF_COLD))
					SendBootUpMsgToMMI();
#endif
			
			SendOperMode();
			sys_st.last_req_tx_ms = GetMsTick();
		}
	}
	else if(sys_st.com_step == COM_BOOTUP_OK)
	{
		if(ChkRunFlag(INIF_COLD))
		{
			sys_st.load_part_ind = -1;
			
			ClearAllPartSubRxBit();
			LoadPartConfig();
			ComMMINextPart();
			if(ChkSysCfg(CO43_INJPRE))
            {
    			sys_st.com_step = COM_PART_PARA;
    			sys_st.last_req_tx_ms = 0;
            }
            else
            {
    			sys_st.com_step = COM_SYS_PARA;
			    sys_st.last_req_tx_ms = GetMsTick();
            }
		}
		else
			ComInitOK();
	}

	if(sys_st.cur_part != NULL && sys_st.com_step == COM_SYS_PARA)
	{
	    /*解决十字头转换问题，确保面板向主机请求到CODE码表(十字头)后，主机再向面板请求数据，保证模板位置经过转换*/
		if(sys_rcv_flag == 0x01 || GetMsTick() - sys_st.last_req_tx_ms > 5000)
		{
			if(sys_rcv_flag == 0)
				SetMachineErr(ER1_DATAINTERR);
			sys_rcv_flag = 0x02;
			sys_st.com_step = COM_PART_PARA;
			sys_st.last_req_tx_ms = 0;
		}
	}
	else if(sys_st.cur_part != NULL && sys_st.com_step == COM_PART_PARA)
	{
		sub_bits = GetPartSubNeedRxBit(sys_st.cur_part);
		if(sub_bits == 0 || sub_bits != sys_st.last_sub_bits || GetMsTick() - sys_st.last_req_tx_ms > MMI_ACK_TM || sys_st.last_req_tx_ms == 0)
		{
			if(sub_bits == 0)
			{
				/*放在调用ComMMINextPart之前，因为该函数会把sys_st.last_sub_bits清零*/
				if(sys_st.last_sub_bits != 0)
					sys_st.rx_mmi_part_cnt++;

				ComMMINextPart();
			}
			
			//因为面板在加载画面过程中，不会回复部件数据给主机，导致重试次数到后，部件没有数据。
			//所以要判读至少已成功获取到3个部件数据后才能用超时重试判断。
			else if(sys_st.req_retry > MAX_SUB_REQ_RETRY && sys_st.rx_mmi_part_cnt > 3)
			{
				/*没有从面板获取到该部件的配置数据，禁用该部件*/
				SET_PART_ST(sys_st.cur_part, PART_DISABLE);

				SetMachineErr(ER0_PARTNULL);
				ComMMINextPart();
				db_printf("time out\r\n");
			}
			else
			{
				PartSubReqTx(sys_st.cur_part, sub_bits, FALSE);

				/*如得到部分子类数据，则不认为超时*/
				if(sys_st.rx_mmi_part_cnt > 0 && sys_st.last_sub_bits == sub_bits)
				{
					sys_st.req_retry++;
					db_printf("retry ++ \r\n");
				}
				
				sys_st.last_req_tx_ms = GetMsTick();
				sys_st.last_sub_bits = sub_bits;
			}
		}
	}
	else if(sys_st.cur_part == NULL && sys_st.com_step == COM_PART_PARA)
		ComInitOK();
}

void ManualInitProc(BOOL berr_init)
{
	int i;
	ClrRunFlag(INIF_AUTO|INIF_AUTO1 | INIF_CHKPC);

	ClearAllPOOut();
	MachineCmd(CMD_MANUAL_INIT, (UI32)berr_init);
	AlarmManualInit();
	InitHydrData(); 	
	
	if(!berr_init)
	{
		g_sdo_abort_code = 0;
		ClearAllActTaskEnf();//强制清除所有任务
		InitKey();
		
		ServoRecoverActNoEn();
		ManualSafeDoorErrOut();
		ClearAllPartError();
		MotorEnableOnAct();		//驱动重新使能

		InitWarmFreeTime();
	}
	else
	{
		ClearAllActTask();//清除任务，除一些设置不可清除的任务外
	}

	ClearRunKey();
	for(i=0;i<LIO_MAX_NUM;i++)
	{
		g_statkey.IOoutclr[i] = 1;
		g_statkey.IOflag[i] = 0;
	}	
}

void ManualReset()
{
	if(ChkRunFlag(INIF_MOTOR))
		MotorDown();

	MMIMonReset();
	MainProtReset();
	SendKeyToMMI(KEY_MANUAL);
	//db_printf("Send Manual key\r\n");
	SetOperMode(OM_MANUAL);
	
//	if(bauto)
//		SendMonCycleRunSt();
	
	ManualInit();
#ifdef DM9000_ENABLE		//Dm9000时，按手动键时，不进入通讯的初始化部分，因为DM9000在通讯初始化的时候，会有232和udp的重练机制
#else
	ComStateReset();
#endif
	if(ChkRunFlag(INIF_RESET))
		SetMachineErr(ER0_PNLS);	

	MoldAlarmClear();
}

void SendKeyToMMI(WORD key)
{
	PPROT_KEYPAD_RSP_PKT	ppkt;
	
	if((ppkt = (PPROT_KEYPAD_RSP_PKT)ProtNewPkt(KEYPAD_CMD, 0, TRUE, sizeof(PROT_KEYPAD_RSP_PKT))) != NULL)
	{
		ppkt->keycode = ((key &0x0ff) | RSP_KEYON);
		MainProtTxPkt(ppkt);
	}
}

void SendIOForceExitToPanel()  //发送给面板IO强制输出退出命令 2025-2-24 ccz
{
	PPROT_CTRL_CPU_TO_PANEL_CMD_PKT	ppkt;
	
	if((ppkt = (PPROT_CTRL_CPU_TO_PANEL_CMD_PKT)ProtNewPkt(SYS_CTRL_CPU_TO_PANEL_CMD, 0, FALSE, sizeof(PROT_CTRL_CPU_TO_PANEL_CMD_PKT)))!= NULL)
	{
		ppkt->ctrl_cmd = PO_TIMEOUT_EXIT_CMD;
        ppkt->data_len = 0;
		MainProtTxPkt(ppkt);
	}
}

void SendRequestResetToPanel()  //发送给面板请求重启命令(在需要重启才能生效的配置代码发生改变时，发送此命令) 2023-7-4 ccz
{
	PPROT_CTRL_CPU_TO_PANEL_CMD_PKT	ppkt;
	
	if((ppkt = (PPROT_CTRL_CPU_TO_PANEL_CMD_PKT)ProtNewPkt(SYS_CTRL_CPU_TO_PANEL_CMD, 0, FALSE, sizeof(PROT_CTRL_CPU_TO_PANEL_CMD_PKT)))!= NULL)
	{
		ppkt->ctrl_cmd = CFG_CHG_NEED_RESET_CMD;
        ppkt->data_len = 0;
		MainProtTxPkt(ppkt);
	}
}

void SendBootUpMsgToMMI()
{
	UI16 BootFlag = 0;
	PPROT_BOOTUP_MSG_PKT	ppkt;

#ifdef DM9000_ENABLE
	if(Udp_uart_flag == 1)		//此处为在启动时，发送Udp的启动命令，在协议层上，告诉面板当前的是udp
		BootFlag = BOOTUP_MSG_UDP;
	else
		BootFlag = BOOTUP_MSG;
#else
		BootFlag = BOOTUP_MSG;
#endif

	if(Udp_uart_flag != 2)	//该标记为2时，代表不发送
	{
		if((ppkt = (PPROT_BOOTUP_MSG_PKT)ProtNewPkt(BootFlag, 0, TRUE, sizeof(PROT_BOOTUP_MSG_PKT)) )!= NULL)
		{
			/*set all to zero */
			ppkt->boot_type = 0;
			ppkt->error_bits = 0;
			ppkt->hw_ver = 0;
			ppkt->sw_ver = 0;
			db_printf("Send BootupMsg\r\n");
			MainProtTxPkt(ppkt);
		}
	}
}

/*g_systemflag*/
UI32 GetRunFlag()
{
	return sys_st.system_flag;
}

UI32 ChkRunFlag(UI32 mask)
{
	return (sys_st.system_flag & mask);
}

UI32 SetRunFlag(UI32 mask)
{
	return (sys_st.system_flag |= mask);
}

UI32 ClrRunFlag(UI32 mask)
{
	return (sys_st.system_flag &= (~mask));
}

BOOL ComIsOk()
{
	if(sys_st.com_step >= COM_INIT_OK)
		return TRUE;
	else
		return FALSE;
}

BOOL ComIsFail()
{
	return sys_st.com_fail;
}

void ComFailIsPost()
{
	if(sys_st.com_fail_post)
	{
		sys_st.com_fail_post = FALSE;
		sys_st.com_fail = TRUE;
		ComStateReset();
		ManualInit();
		SetOperMode(OM_MANUAL);
	}
}

/*发送的包已成功收到对方的应答*/
void SetPktAckOK(UI16 id, UI16 cmd)
{
	/*只确认BOOTUP命令包面板是否已收到*/
	if(sys_st.com_step == COM_INIT_START && (cmd == BOOTUP_MSG || cmd == BOOTUP_MSG_UDP))
	{
		sys_st.com_step = COM_BOOTUP_OK;

#ifdef DM9000_ENABLE
		if(cmd == BOOTUP_MSG_UDP)	
		{
			Udp_uart_flag = 1;	//此处代表建立的通讯方式，走的是UDP通讯
		}
		else
		{
			Udp_uart_flag = 0;		//此处代表建立的通讯方式，走的是232通讯	
		}
#else
		Udp_uart_flag = 0;
#endif
	}
}

void SetComState(BOOL bok)
{
	PMACHINE pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1));
	if(bok)
	{
		if(sys_st.com_step == COM_INIT_START &&( !ChkRunFlag(INIF_COLD)))
			sys_st.com_step = COM_BOOTUP_OK;

		sys_st.com_fail = FALSE;
		sys_st.com_fail_post = FALSE;
	}
	else
	{
		
		if(sys_st.com_step == COM_INIT_OK)
		{
			if(!ChkOperMode(OM_AUTO))
			{
				sys_st.com_fail = TRUE;
				ComStateReset();
				SetRunFlag(INTF_OFF);
			}
			else
			{
				if(sys_st.com_fail_post == FALSE)	//只记录在自动状态下，每断开一次记录一次
				{
					sys_st.err_cnt = TRUE;		//记录标记，用于记录是否出现错误，如果出现过，就在切手动的时候将数据保存到flash中
				}
				
				//在再循环时置通信失败
				sys_st.com_fail_post = TRUE;
			}
		}
	}
}

UI32 CalMsTo100MsUI16(UI32 ms_t)
{
	UI32 tmp;

	tmp = (GetMsTick() - ms_t + 50)/100;

	if(tmp > 0xffffffff)
		return 0xfffffffE;
	else
		return tmp;
}

UI32 CalMsTo10MsUI16(UI32 ms_t)
{
	UI32 tmp;

	tmp = (GetMsTick() - ms_t + 5)/10;
	if(tmp > 0xffffffff)
		return 0xfffffffE;
	else
		return tmp;
}

UI32 Cal100UsTo10MsUI16(UI32 us100_t)
{
	UI32 tmp;

	tmp = (Get100UsTick() - us100_t + 50)/100;
	if(tmp > 0xffffffff)
		return 0xfffffffE;
	else
		return tmp;
}

/*系统内存不足，致命错误*/
void SysMemOutFatalErr()
{
	/*do someting here*/
}

void UpgradeCmdRx(UI16 type)
{
	/**/
	if(ChkOperMode(OM_MANUAL))
	{
		MotorDown();     //2014-11-12  重启关马达
		ClearAllPOOut();
		LoopPOOnOff();
		ClearAllHydrOut(0);
  
		/*升级重启*/
		if(type == 0x0001)
			m_clr_flag();
		
		/*Reboot */
		HWRESET_SET;			/*强制复位*/
		while(1);
	}
}

void SysDataSave(UI16 id)
{
 	PPART pp;
	UI16 partid,partno,ind;
	const PART_TYPE_INFO* pinfo;
	
	partid = (id&0xFF00)>>8;
	partno = (id&0x00F0)>>4;
	ind = id&0x000F;

	if(id == 0x0014)//防止面板下发保存主机的版本信息
		return;
	
 	pp = GetMachineSubPart(partid,partno);

	if(pp != NULL)
	{
		pinfo = GetPartTypeInfo(partid);   //part_type_info[]中序号指针
		
		if((pinfo->stor_sub_bits & (0x01<<ind)) > 0)
		{
			PartCfgSave(pp, ind);
		}	
	}	 
}

void SysAdjCfgCmdRx(UI16 ctrl_cmd, UI16 data_len, UI16* pdat)
{
  PSERVO    ps = &g_hydrservo;
  PPUMP_STRU_5 pp = &g_pump_5;
  PHYDR py = &g_hydrout;
  PMACHINE    pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1));
  PINJECT  pinj = (PINJECT)GetPartByID(MK_PART(INJECT_ID, 1)); 
  PLUB plub = NULL;
  UI16 portnolast = 0,portno = 0,ly = 0;
  UI16 i = 0;
  UI16 data = 0;
  PACT pact;

 	if(ChkOperMode(OM_MANUAL) || (ChkOperMode(OM_CS_RF)&&(ctrl_cmd == SLOPE_CALIBRATION)))
	{ 	switch(ctrl_cmd)
		{
		case CFG_RESET_CMD:				//用户配置数据复位
			if(ChkOperMode(OM_MANUAL))
			{
				UserPartCfgReset();//出厂区的数据复制到用户区
				UserTblCfgReset();
				LoadPartConfig(); 
				SetBackReset();
				/*主动发送当前主机配置数据*/
				MachineCfgTx();
				AdMeterCfgTx();
				DAMaxMinStCpy();
				SetRunFlag(INIF_CFGDBCHG);
			}
			break;
		case CFG_USER_TO_FACTORY_CMD:				//用户配置数据复制到出厂配置数据
			if(ChkOperMode(OM_MANUAL))
			{
				FactoryPartCfgReset();
				FactoryTblCfgReset();
//				LoadPartConfig();
//				SetRunFlag(INIF_CFGDBCHG);
			}
			break;
		case ENTER_ADJ_CMD:			//进入调机状态
			if(ChkOperMode(OM_MANUAL))
			{
				MotorDown();
				GetSys()->md->CUSTOM_PARA[0] = 6;
				WarmOff();
				//ManualInit();
				SetOperMode(OM_MANUAL);
				SetRunFlag(INIF_DEBUG);

				if(g_tempcomm_num > 0)
				{
					EnterDebugE502();
				}
			}
			break;
		case EXIT_ADJ_CMD:			//退出调机状态
			ClrRunFlag(INIF_DEBUG);
			/*清楚DA输出*/
			DAMaxMinAdj(0, 0, 0);
			
			if(g_tempcomm_num > 0)
			{
				PTEMPCTRLCOMM ptc;
				
				ExitDebugE502();

				for(i = 0; i < g_tempcomm_num;i++)
				{
					ptc = g_tempcomm[i];
		
					if(ptc == NULL)
					{
					//如果指针空，则查看下一个部件地址
						continue;
					}
					InitTemp(ptc);
				}				
			}
			break;
		case DA_ADJ_CMD:			//DA最大最小值调整
			if(pdat != NULL)
			{
				DAMaxMinAdj(pdat[0], pdat[1], pdat[2]);
			}				
			break;

		case TEMP_ZERO_CMD:			//温度归零调机
			if(g_tempcomm_num > 0)
			{
				TempZeroSet(pdat[0]);
			}		
			break;
		case TEMP_ADJ_CMD:			//温度校准调机
			if(g_tempcomm_num > 0)
			{
				TempCalbriate(pdat[0]);
			}			
			break;

		case FORCE_PO_CMD:			//输出点强制输出
			POForceOutPut(data_len, pdat);
			break;

		case EXIT_PO_CMD:			//退出输出点强制输出
			POForceEnd();
			break;

		case ENTER_DA_CMD:			//进入DA曲线调试
			if(ChkOperMode(OM_MANUAL))
			{
				SetOperMode(OM_MANUAL);  //进入DA画面 时，做手动处理，防止动作正常执行的时候进入
				ManualInit();
				SetDACurAdjMode(1);
			}
			HydrLoadDACur();
			setselfdefinehydrtbl();
			DAMaxMinStCpy();
			if(!ChkRunFlag(INIF_DASCREEN))
			{
			    SetRunFlag(INIF_DASCREEN);
			    AdjustHydrDisplay(1);
			}
			break;
		case EXIT_DA_CMD:			//退出DA曲线调试
		    if(DAIsAdjust())
			{
				EndDaChgActTsk();
			    ClearAllHydrOut(0);
			    SetDACurAdjMode(0);
				
				pact = pinj->act + OS_INJE - 1;
				HYDR_CLS(0,0);		    

				CLR_PART_ERR(pinj,ER0_CHFL);
				DA_OUTPUMP_Clear();
				DA_OUTPUMP_SPDOUT(0,0,0);
			}
			if(pdat[0]==1)
			{
				if(!ChkRunFlag(INIF_DASCREEN))
			        SetRunFlag(INIF_DASCREEN);
			}
			else
			{
			    if(ChkRunFlag(INIF_DASCREEN))
				{
				    ClrRunFlag(INIF_DASCREEN);
				    AdjustHydrDisplay(0);
				}
			}

			if(ChkSysCfg(CO1_ECAT)&&ChkSysCfg(CO5_INJCHADA))
			{
				g_hydric[1].Press = 0;
				g_hydric[1].Flow = 0;
				g_actopen = 0;
			}
			break;
			
		case PRESS_MAX_ADJ_CMD:		//DA压力最大值调整
			//portno从0开始。
			ClearAllHydrOut2(pdat[0] + 1);
			SetDACurAdjMode(2);
			SetDATempMax(pdat[0]*2, pdat[1]);
			SetPressHydrOutData(pdat[0] + 1, InChangeDaByMode(pdat[1]), pdat[2]);

			if(!(ChkSysCfg(CO2_CHGBK) && ((pinj->hy->HYDR_PORT_CHRGBACK>>pdat[0])&1)))
				DA_OUTPUMP_SPDOUT(GetSys()->mh->DAPRES_SYSLMT,pdat[2],pdat[0] + 1);
			break;
		case FLOW_MAX_ADJ_CMD:		//DA流量最大值调整
			if(ChkOperMode(OM_MANUAL))
			{
				ClearAllHydrOut2(pdat[0] + 1);
				SetDACurAdjMode(2);
				SetDATempMax(pdat[0]*2 + 1, pdat[1]);
				SetFlowHydrOutData(pdat[0] + 1, pdat[2], InChangeDaByMode(pdat[1]));
			}
			break;
		case DA_MAX_ADJOVER_CMD:    //DA最大值调整结束
			EndDaChgActTsk();
			ClearAllHydrOut(0);
			DA_OUTPUMP_Clear();
            //DAMaxMinAdj(0, 0, 0);
			SetDACurAdjMode(1);
			if(ChkSysCfg(CO1_ECAT)&&ChkSysCfg(CO5_INJCHADA))
			{
				g_hydric[1].Press = 0;
				g_hydric[1].Flow = 0;
				g_actopen = 0;
				pact = pinj->act + OS_INJE - 1;
				HYDR_CLS(0,0);
			}
			
			DA_OUTPUMP_SPDOUT(0,0,0);
			//hydraupresout(pdat[0] + 1,0,1);  
			//hydrauflowout(pdat[0] + 1,0,1);
			break;	

		case PRESS_DAMIN_ADJ_CMD:		//DA压力最小值调整
			//portno从0开始。
			ClearAllHydrOut2(pdat[0] + 1);
			SetDACurAdjMode(2);
			SetDATempMin(pdat[0]*2, pdat[1]);
			SetPressHydrOutData(pdat[0] + 1, InChangeDaByMode(pdat[1]), pdat[2]);

			if(!(ChkSysCfg(CO2_CHGBK) && ((pinj->hy->HYDR_PORT_CHRGBACK>>pdat[0])&1)))
				DA_OUTPUMP_SPDOUT(GetSys()->mh->DAPRES_SYSLMT,pdat[2],pdat[0] + 1);
			break;

		case FLOW_DAMIN_ADJ_CMD:		//DA流量最小值调整
			if(ChkOperMode(OM_MANUAL))
			{
				ClearAllHydrOut2(pdat[0] + 1);
				SetDACurAdjMode(2);
				SetDATempMin(pdat[0]*2 + 1, pdat[1]);
				SetFlowHydrOutData(pdat[0] + 1, pdat[2], InChangeDaByMode(pdat[1]));
			}
			break;
			
		case PRESS_CUR_CMD:			//DA压力曲线调整
			if(ChkOperMode(OM_MANUAL))
			{
				db_printf("dat: %d, %d, %d\r\n",pdat[0], pdat[1], pdat[2]); 
				ClearAllHydrOut(pdat[0] + 1);
				SetDACurAdjMode(1);
				SetPressHydrOutData(pdat[0] + 1, pdat[1], pdat[2]);

				if(!(ChkSysCfg(CO2_CHGBK) && ((pinj->hy->HYDR_PORT_CHRGBACK>>pdat[0])&1)))
					DA_OUTPUMP_SPDOUT(GetSys()->mh->DAPRES_SYSLMT,pdat[2],pdat[0] + 1);
			}
			break;
		case FLOW_CUR_CMD:			//DA流量曲线调整
			if(ChkOperMode(OM_MANUAL))
			{
				ClearAllHydrOut(pdat[0] + 1);
				SetDACurAdjMode(1);
				SetFlowHydrOutData(pdat[0] + 1, pdat[2], pdat[1]);
			}
			break;			
		case DA_PAIR_CUR_CMD:		//DA曲线压力流量同时调整     即强制输出
			if(ChkOperMode(OM_MANUAL))
			{
				ClearAllHydrOut(pdat[0] + 1);
				SetDACurAdjMode(1);
				if((pdat[2] == 99) || pdat[2] == 999)
					pdat[2] = 100;
				SetHydrOutData(pdat[0] + 1, pdat[1], pdat[2]);
				DA_OUTPUMP_SPDOUT(pdat[1],pdat[2],pdat[0] + 1);
			}
			break;			
		case DA_CUR_SAVE_CMD:			//保存DA曲线
			ClearAllActTask();
			DATempMaxMinSave();
			HydrDaCurSave();	
			ClearAllHydrOut(0);

			if(ChkOperMode(OM_MANUAL))
			{
				inner_pa.pa1 = pm->mh->DA_LINE;
				SaveInnerPara();
			}
			break;
		case METER_LENGTH_ADJ_CMD:		//电子尺长度调整
			//pdat[0]通道号 pdat[1]数值 pdat[2]类型:0为主机电子尺 1为外扩电子尺
			data = pdat[2];
			if(data > 1)
			{
				data = 0;
			}
			AdMeterLengthModify(pdat[0], pdat[1],data);
			AdMeterZeroAdj(data);
			break;
		case METER_ZERO_ADJ_CMD:		//电子尺长度调整
			data = pdat[0];
			if(data > 1)
			{
				data = 0;
			}
			AdMeterZeroAdj(data);
			break;
		case MACHINE_CFG_MODI_CMD:		//系统机器参数修改
			if(pm->mh->FL_VERSION != 0  && pm->mh->FL_CTRLDATE != 0 && pm->mh->FL_CTRLMODEL != 0 &&  pm->mh->DAPRES_SYSLMT != 0) 	//增加保护，防止面板未接收到数据的情况下，往下保存数据
		  	{
				MachineSysCfgModify();

                if(data_len > 0 && pdat[0] == 0x95) //0x95表示保存CODE码
                    CPU_Need_Reset_Chk(); //主机重启判断
		  	}
		    else
		  	{
		  		SET_PART_ERR(pm,ER0_CFG_ERR);
		  		LoadCfgDb(USER_PART_DB);
		  	}
			PartDataTx((PPART)pm, MH_ID, CAL_LEN(*(pm->mh)), 0, TRUE);
		  	SetRunFlag(INIF_CFGDBCHG);
			break;
			
		case DA_CFG_RESET_CMD:			//DA曲线及最大值 最小值重置.
			ClearAllHydrOut(0);
			//DAMaxMinReset();
			SetHydrCurReset(FALSE);
			//MachineCfgTx();
			break;
		case CAN_TEST_CMD:         //CAN通信开始
			if(pdat[0] == 1)
			{
				pp->cantestmode  = 1;
				ps->cantestmode = 1;
				ps->mh->READCNT  = 0;
				ps->mh->WRITECNT = 0;
				pp->writestep  = 0;
				pp->readstep   = 10;
				pp->ServoNum   = pdat[1];
		    }
			else
			{
				pp->cantestmode = 0;
				ps->cantestmode = 0;
				pp->writestep  = 0;
	      	 	pp->readstep   = 0;  
			}			
			break;
		case AMB_TEMP_ADJ_CMD:         //室温校准
			if (pdat[0] == 0)
			{//板温的室温矫正
				AMBTempAdjMain();
			}
			else
			{//E502的室温矫正
				if (g_tempcomm_num > 0)
				{
					AMBTempAdj(pdat[0]);
				}
			}
			break;
		case MOLD_ADJ_CMD:            //松模调模采样初始化
			AdjMoldInit(TRUE);
			break;
        case CHGBACK_ADJ_CMD:         //背压线性重新生成
			 for(ly=0;ly<4;ly++)
			 {
			 	if(pdat[0] == 0)
			 	    portnolast = 0;
				else
				{
			 		if((pdat[0]>>ly)&1)
			 			portnolast = ly+1;
				}
				if(pdat[1] == 0)
					portno = 0;
				else
				{
					if((pdat[1]>>ly)&1)
						portno = ly+1;
				}
			 }
            chgbackpresset(portnolast,portno,pdat[2]);
            break;
		case CFG_ERASE_CMD:			///清除主机配置内容，重启后主机配置被重写成程序内置的缺省配置。
	 		if(ChkOperMode(OM_MANUAL) && !MotorIsOn())
	 		{
				CfgDbSetDefault();
				UpgradeCmdRx(0);			/*只重启主机*/
	 		}
			break;
		case ZEROLINE_ADJ_CMD:
			if(ChkOperMode(OM_MANUAL))
			{
				//getdefaulthydrtbl();
				getdefaulthydrtblLine(pdat[1],pdat[0]);
				PartCfgSave( &g_hydrout, ZD_ID);//保存内存数据到用户区
				PartDataTx((PPART)py, ZD_ID, CAL_LEN(*(py->zd)), 0, FALSE);
				//CfgDbPartSave((PPART)pp, ZD_ID, CAL_LEN(HYDR_ZDSET), 0, FACTORY_PART_DB);
				inner_pa.pa1 = pdat[0];
				SaveInnerPara();
			}
			break;
		case CAN_FORBID_CMD:
			/*禁止CAN通信*/
			if(!MotorIsOn())
			{
				CanDisableTx(TRUE);
			}
			break;
		
		case CAN_ENBALE_CMD:
			/*允许CAN通信*/
			CanDisableTx(FALSE);
			break;
		case COMM_SUBCMD_TEMPOPTIMIZE_CMD:
			TempTempRFStartPro(pdat[0],pdat[1],pdat[2]);
			break;
		case SYS_DATA_SAVE_CMD:
			SysDataSave(pdat[0]);
			break;
		case CAN_TEST_E502:			
			if(pdat[0] == 1)
			{
				PTEMPCTRLCOMM ptc;
				ptc = g_tempcomm[pdat[1] - 1];
				
				if(ptc != NULL)
				{
					ptc->can_e502_testmode = 1;
					ptc->can_e502_Readstep = 0;
					ptc->can_e502_writestep = 0;
					ptc->can_e502_num = pdat[1];		//e502第几块通讯采用面板上发送下来的数据
					ptc->st->WRITE_CNT_E502 = 0;
					ptc->st->READ_CNT_E502 = 0;
				}					
			}
			else
			{
				PTEMPCTRLCOMM ptc;
				ptc = g_tempcomm[pdat[1] - 1];
				
				if(ptc != NULL)
				{
					ptc->can_e502_testmode = 0;
					ptc->can_e502_Readstep = 0;
					ptc->can_e502_writestep = 0;
				}
			}
			break;
		case ERR_CNT_CLEAR:		//清空警报次数
			break;
		case CAN_TEST_EXIT:
			for(i = 0; i < g_tempcomm_num;i++)
			{
				PTEMPCTRLCOMM ptc = g_tempcomm[i];
				
				if(ptc != NULL)
				{
					ptc->can_e502_testmode = 0;
					ptc->can_e502_Readstep = 0;
					ptc->can_e502_writestep = 0;
				}
			}

			for(i = 0; i < g_tempmw_num;i++)
			{
				PTEMPCTRLMW pmw = g_temp_mw[i];
				
				if(pmw != NULL)
				{
					pmw->can_e501_testmode = 0;
					pmw->can_e501_writestep = 0;
				}
			}

			if(pm != NULL)
			{
				pm->can_e700b_testmode = 0;
				pm->can_e700b_writestep = 0;
			}
			break;
		case SLOPE_CALIBRATION://斜率校准
			if(pdat[0] == 0x01)
			{//开始
				if((pdat[1]&0xFFF0) == 0x2010)
				{
					ClampCalibration();	
				}
				else if((pdat[1]&0xFFF0) == 0x2110)
				{
					EjectCalibration();
				}
				else if((pdat[1]&0xFFF0) == 0x1010)
				{
					InjectCalibration();
				}
				SetOperMode(OM_CS_RF);
			}
			else if(pdat[0] == 0xFF)
			{
				if(ChkOperMode(OM_CS_RF))
				{
					ManualReset();
				}		
			}
			break;
		case TEMP_PID_UPDATE_CMD:
			TempPIDUpdate(pdat[0]);
			break;

		case CAN_TEST_E501:		//data[0]为通讯测试开启和关闭，data[1]代表第几块模温板
			if (pdat[1] > g_tempmw_num)
			{
				break;
			}
			if(pdat[0] == 1)
			{
				PTEMPCTRLMW pmw;
				pmw = g_temp_mw[pdat[1] - 1];
				
				if(pmw != NULL)
				{
					pmw->can_e501_testmode = 1;
					pmw->can_e501_writestep = 0;
					pmw->st->WRITE_CNT_E501 = 0;
					pmw->st->READ_CNT_E501 = 0;
				}
			}
			else
			{
				PTEMPCTRLMW pmw;
				pmw = g_temp_mw[pdat[1] - 1];
				
				if(pmw != NULL)
				{
					pmw->can_e501_testmode = 0;
					pmw->can_e501_writestep = 0;
				}
			}
			break;

		case COMM_MW_TEMPOPTIMIZE_CMD:
			Temp_mwRFStartPro(pdat[0],pdat[1],pdat[2]);	//pdat[0]为0代表普通优化，8367为强制优化，pdat[1]为第几块扩展板，按位组合 0xff，最后一位pdat[2]暂时不用
			break;
		case CAN_TEST_E700B :
			if(pdat[0] == 1)
			{
				if(pm != NULL)
				{
					pm->can_e700b_testmode = 1;
					pm->can_e700b_writestep = 0;
					pm->st->WRITE_CNT_E700B = 0;
					pm->st->READ_CNT_E700B = 0;
				}
			}
			else
			{
				if(pm != NULL)
				{
					pm->can_e700b_testmode = 0;
					pm->can_e700b_writestep = 0;
				}				
			}
			break;
			
		default:
			break;
      }
	}
	else if(ChkOperMode(OM_AUTO))
	{
		switch(ctrl_cmd)
		{
			case PAYMENT_TIMEOUT_CMD:
				pm->PAYMENT_TIMEOUT_FLAG=1;	
				break;
			case OPEN_RELVLV_CMD:
				pinj->relvlvopnflag = TRUE;
				break;
			default:
				break;		
		}	
	}

	switch(ctrl_cmd)
	{
		case NEED_LUB_CMD:
			if (pdat[0] <= pm->lub_num)
			{
				plub = (PLUB)GetMachineSubPart(LUB_ID, pdat[0]);

				if (plub != NULL)
				{
					if((plub->mh->FL_LUBMODE == 3) && (CheckPIUsed(plub->pi->I_LUB_CHK)))
					{
						plub->st->AUTOLUB_NEED = TRUE;
						plub->lubcmp = 0;
						LubClmpLeftClear(plub);
					}
				}
			}
			break;
		default:
			break;
	}
}

/*-----------------------------------------------------------------------------------+
|		计算时间差  0-65535之间数据                                          |
+------------------------------------------------------------------------------------*/
WORD delt_time(WORD Cur_T,WORD Del_T)
{ 
	WORD temp;
	if (Cur_T >= Del_T)
	{
		temp = (WORD)(Cur_T - Del_T);
	}
	else
	{
		temp = (WORD)(Cur_T + 65536 - Del_T);
	}
	return temp;
}

void delay_ms(WORD CN)
{
	WORD tp,tp1;
	tp = A_1msclock;
	tp1 = A_1msclock;
	FeedDog();
	while(delt_time(A_1msclock,tp) < CN)
	{
		if((CN > 50) && (delt_time(A_1msclock,tp1) > 50))		//当时间超过50ms时喂一次狗，目前测试发现的狗时间不准，基本要大于500ms   20210914
		{
			FeedDog();
			tp1 = A_1msclock; 
		}
		;
	}
	FeedDog();
}	

UI16* GetBootVer()
{
	UI16 tmp = *(UI16*)(Addr_Infoexchange + BOOT_VER_OFFSET*2)/100;

	/*A6/A9 bootloader版本为80xx，A1000的bootloader的版本为90xx*/
	if(tmp == 80 || tmp == 90)
	{
		return (UI16*)(Addr_Infoexchange + BOOT_VER_OFFSET*2);
	}
	else
	{
		return NULL;
	}
}

void CtrlModelInit(UI16 ctrl_model)
{
	if(ctrl_model == MODEL_I3521)
	{
		if(hw_cfg.hw_I3I5 == PCB_NAME_3201)
		{
			hw_cfg.hw_model = MODEL_I3201;
			hw_cfg.hw_maxadport = I3201_MAXADPORT;
			hw_cfg.hw_max_advalue = I3201_MAX_ADVAULE;
			hw_cfg.hw_pimax = I3201_PIMAX;
			hw_cfg.hw_pomax = I3201_POMAX;
			hw_cfg.hw_inn_maxtempchnl = I3201_INN_MAXTEMPCHNL;
			hw_cfg.hw_da_port_num = I3201_MAXDAPORT;
			hw_cfg.press_senor_ch = I3201_MAXADPORT;
		}
        else if(hw_cfg.hw_I3I5 == PCB_NAME_3202)
		{
			hw_cfg.hw_model = MODEL_I3202;
			hw_cfg.hw_maxadport = I3202_MAXADPORT;
			hw_cfg.hw_max_advalue = I3202_MAX_ADVAULE;
			hw_cfg.hw_pimax = I3202_PIMAX;
			hw_cfg.hw_pomax = I3202_POMAX;
			hw_cfg.hw_inn_maxtempchnl = I3202_INN_MAXTEMPCHNL;
			hw_cfg.hw_da_port_num = I3202_MAXDAPORT;
			hw_cfg.press_senor_ch = I3202_MAXADPORT;
		}
		else if(hw_cfg.hw_I3I5 == PCB_NAME_5201)
		{
			hw_cfg.hw_model = MODEL_I5201;
			hw_cfg.hw_maxadport = I5201_MAXADPORT;
			hw_cfg.hw_max_advalue = I5201_MAX_ADVAULE;
			hw_cfg.hw_pimax = I5201_PIMAX;
			hw_cfg.hw_pomax = I5201_POMAX;
			hw_cfg.hw_inn_maxtempchnl = I5201_INN_MAXTEMPCHNL;
			hw_cfg.hw_da_port_num = I5201_MAXDAPORT;
			hw_cfg.press_senor_ch = I5201_MAXADPORT;
		}
		else if(hw_cfg.hw_I3I5 == PCB_NAME_3101)
		{
			hw_cfg.hw_model = MODEL_I3101;
			hw_cfg.hw_maxadport = I3101_MAXADPORT;
			hw_cfg.hw_max_advalue = I3101_MAX_ADVAULE;
			hw_cfg.hw_pimax = I3101_PIMAX;
			hw_cfg.hw_pomax = I3101_POMAX;
			hw_cfg.hw_inn_maxtempchnl = I3101_INN_MAXTEMPCHNL;
			hw_cfg.hw_da_port_num = I3101_MAXDAPORT;
			hw_cfg.press_senor_ch = I3101_MAXADPORT;
		}
		else
		{
			hw_cfg.hw_model = MODEL_I3201;
			hw_cfg.hw_maxadport = I3201_MAXADPORT;
			hw_cfg.hw_max_advalue = I3201_MAX_ADVAULE;
			hw_cfg.hw_pimax = I3201_PIMAX;
			hw_cfg.hw_pomax = I3201_POMAX;
			hw_cfg.hw_inn_maxtempchnl = I3201_INN_MAXTEMPCHNL;
			hw_cfg.hw_da_port_num = I3201_MAXDAPORT;
			hw_cfg.press_senor_ch = I3201_MAXADPORT;
		}
	}
	else
	{		
		hw_cfg.hw_model = MODEL_I3201;
		hw_cfg.hw_maxadport = I3201_MAXADPORT;
		hw_cfg.hw_max_advalue = I3201_MAX_ADVAULE;
		hw_cfg.hw_pimax = I3201_PIMAX;
		hw_cfg.hw_pomax = I3201_POMAX;
		hw_cfg.hw_inn_maxtempchnl = I3201_INN_MAXTEMPCHNL;
		hw_cfg.hw_da_port_num = I3201_MAXDAPORT;
		hw_cfg.press_senor_ch = I3201_MAXADPORT;
	}
}
