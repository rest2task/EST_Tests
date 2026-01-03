
/******************************************************************************
  文 件 名   : machine.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 机器部件及全局功能函数
  函数列表   :
              AllocSubPartMem
              CheckSysConfig
              ChkCoreIsRun
              ChkInjectCushErr
              ChkMachineErr
              ChkOperMode
              ClrMachineErr
              CycleTskIsRun
              GetMachineSubPart
              GetMachineSubPartList
              GetMachineSubPartNum
              GetMoldAlarmMode
              GetOperMode
              GetSysCfg
              GetSysConfig
              GetSysMold
              GetSysPO
              InitMachinePart
              InitMachineVersion
              InitSubPart
              MachineCmd
              MachineMotorOnSafeChk
              MachineSetup
              MachineSubPartCmd
              MachineSysCfgModify
              SendOperMode
              SetAgeOldTm
              SetMachineErr
              SetMoldAlarmMode
              SetOperMode
              SetOperTM
  修改历史   :
  1.日    期   : 2013年5月17日
    作    者   : ren chaohong
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
	 
#include "com.h"
#include "hwcfg.h"
#include "actcom.h"
#include "machine.h"
#include "motor.h"
#include "recycle.h"
#include "systempro.h"
#include "cfgdb.h"
#include "version.h"
#include "canhead.h"
#include "clamp.h"
#ifdef PLC_ENABLE
#include "il3.h"
#endif
#include "memtbl.h"
#include "zerodebug.h"
#include "useriap.h"	 
#include "sram.h"
#include "nozzle.h"
#include "lub.h"
#include "types.h"
#include "ZeroDebug.h"
#include "sram.h"		  
#include "CalibrationAggControl.h"
#include "TempCtrlComm.h"
#include "TempCtrlMw.h"
#include "hydrpos.h"
#include "hwmid.h"	 
#include "udp_demo.h" 
#include "ServFive.h"
#include "coreprotect.h"

/*----------------------------------------------*
* 外部变量说明								 *
*----------------------------------------------*/
extern UI16 iser_mem;
extern UI16 FuncAiClampOpn;

/*----------------------------------------------*
* 外部函数原型说明							 *
*----------------------------------------------*/
BOOL ECATStateCheck(void);
BOOL m_da_protect(void);
BOOL m_cpld_test_card(void);
void CheckIoForce(void);
void FeedDog(void);
void ModbuDownloadPro(void);
void OilPortTbCopy(void);
WORD GetRealSlaveNum(void);
void MotorServoSateChk(void);
void InitMotorControlMode(void);
void AdMeterSpeedLoopPro(void);
void WarmFreeTimeChk(void);
UI16 GetMainPortBit(void);
void IOForceTimeOut(void);

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
MACHINE m_machine;
INNER_PARA	inner_pa;

DWORD	LubStart;		 
WORD	LubStartFirst=0;
BOOL	WKPOint = 0;
WORD	WKPress;
UI16	m_speed;
UI32  	curetshot = 0;
WORD 	g_sram_err = 0;

UI16    g_max_loop_100us = 5;

UI16 need_rest_cfg[] = 
{
    CO0_SERVOP,     //CODE0 + 4  使用电脑PID
    CO1_ECAT,       //CODE1 + 4  使用EtherCat通讯控制	
    XF2_AICLAMP,    //CODE50 + 4000  开模AI记忆
    XF6_HSERV0,     //CODE54 + 20  使用伺服驱动
    XF6_CANSRV,     //CODE54 + 1000  使用CAN SERVO
    CO9_HIGH,       //温度控制 + 20  温度归零/校准值使用出厂调试区数据
};

UI8 last_cfg[sizeof(need_rest_cfg)/sizeof(UI16)] = {0};

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

void CPU_Need_Reset_Init()
{
    UI16 i;
    
    for(i = 0;i < sizeof(need_rest_cfg)/sizeof(UI16);i++)
    {
        last_cfg[i] = ChkSysCfg(need_rest_cfg[i]);
    }
}

void CPU_Need_Reset_Chk()
{
    BOOL need_reset = FALSE;
    UI16 i;

    for(i = 0;i < sizeof(need_rest_cfg)/sizeof(UI16);i++)
    {
    	if(ChkSysCfg(need_rest_cfg[i]) != last_cfg[i]) //配置发生改变时需重启
        {
            last_cfg[i] = ChkSysCfg(need_rest_cfg[i]);
            need_reset = TRUE;
        }
    }

    if(need_reset)
    {
        SendRequestResetToPanel();
    }
}

/*******************************************************************************************
 *Brief			:对外部SRAM存储区域进行读写校验
 *Param			:
 *Return Value	:g_sram_err = 1；SRAM读写存在问题。
 *Aauthor		:ZT
 *Date			:20200821
********************************************************************************************/
void SramDataChk()
{
	WORD *psrcdata;
	WORD sramreaddata;
	WORD j;
	UI32 i;

	g_sram_err = 0;

	for (j = 0; j < 4; j++)
	{
		psrcdata = (WORD*)0x1E000000;	//SRAM起始地址
		for (i = 0;i < 0x40000; i++)	//0x40000 = 256K*WORD
		{
			*psrcdata++ = (WORD)(i);
		}

		psrcdata = (WORD*)0x1E000000;	//SRAM起始地址
		for (i = 0;i < 0x40000; i++)	//0x40000 = 256K*WORD
		{
			sramreaddata = *psrcdata++;
			if (sramreaddata != (WORD)(i))	//比较
			{
				g_sram_err = 1;	//数据校验失败
			}
		}
		
		FeedDog();

		psrcdata = (WORD*)0x1E000000;	//SRAM起始地址
		for (i = 0;i < 0x40000; i++)	//0x40000 = 256K*WORD
		{
			*psrcdata++ = (WORD)(~i);
		}

		psrcdata = (WORD*)0x1E000000;	//SRAM起始地址
		for (i = 0;i < 0x40000; i++)	//0x40000 = 256K*WORD
		{
			sramreaddata = *psrcdata++;
			if (sramreaddata != (WORD)(~i))	//取反比较
			{
				g_sram_err = 1;	//数据校验失败
			}
		}

		FeedDog();
	}	
	
	psrcdata = (WORD*)0x1E000000;
	for (i = 0;i < 0x40000; i++)
	{
		*psrcdata++ = 0;	//对SRAM内存区域进行赋0初始化
	}
}

void InitMaxLoop100us(void)
{
    if(CHK_MOTOR_MODE(MOTOR_CTRL_ECAT))
        g_max_loop_100us = 5;
    else
        g_max_loop_100us = 4;
}

UI16 G_MAX_LOOP_100US()
{
    return g_max_loop_100us;
}


void InitFuncSel(void)
{
	FuncAiClampOpn = ChkSysCfg(XF2_AICLAMP);	//打开后需要重启生效
}

void VersionDiff()
{
	PMACHINE pp = &m_machine;
	UI32 localdate = 0;//主机版本时间
	UI32 machdate = 0;	//面板版本时间
	UI16 machver = 0;	//面板版本
	UI16 i ;
	PCORE pcore;
	UI8 num;

	if(!ComIsOk())
		return;

	localdate |= pp->vr->cus_info.date[0];
	localdate |= (pp->vr->cus_info.date[1]<<8);
	localdate |= (pp->vr->cus_info.date[2]<<16);
	localdate += 20000000;

	machdate += pp->md->UPPER_CP_CTRLDATE1*10000;
	machdate += pp->md->UPPER_CP_CTRLDATE2;

	machver = pp->md->UPPER_CP_VERSION1;
	
	if (20200415 > machdate)
	{
		//版本日期20200415 
		//温度与模具温度的 电热连续加温不检测	改为 电热连续加温检查	取反
		//新增 射退开储料零背压 默认开启，原面板无此选项。默认开启
		
		for (i = 0;i<g_tempcomm_num;i++)
		{
			g_tempcomm[i]->mh->FL_TEMPWORDUPCHECK = !g_tempcomm[i]->mh->FL_TEMPWORDUPCHECK;
		}
		
		for (i = 0;i<g_tempmw_num;i++)
		{
			g_temp_mw[i]->md->FL_TEMPUPCHECK_MW = !g_temp_mw[i]->md->FL_TEMPUPCHECK_MW;
		}
		pp->pinj->mh->FL_CHR_RL2_SUK = 1;	//老版本面板无此功能设定，默认开启。
			
	}

	if (20201126 > machdate)
	{
		pcore = (PCORE)GetMachineSubPartList(CORE_ID, &num);

		for (i = 0; i< num;i++)
		{
			pcore->md->ADPOSI_CORE_BWD = 1;
			pcore->md->ADPOSI_CORE_FWD = 1;
			pcore++;
		}
	}

	if (machver <= 0103)
	{
		pcore = (PCORE)GetMachineSubPartList(CORE_ID, &num);

		for (i = 0; i< num;i++)
		{
			pcore->mh->TM_COREINENDDLY = pcore->mh->TM_COREENDDLY;
			pcore->mh->TM_COREOUTENDDLY = pcore->mh->TM_COREENDDLY;
			pcore++;
		}
	}
}


void CycleTmOutCount(UI32 ustick_10us)
{
	static UI32 time0,time1,max_time = 0,max_no = 0;
	
	time0 = (A_10usclock - ustick_10us)*10;	
	time1 = (time1*9 + time0)/10;
	max_no++;

	if (max_no%2000 == 0)
		max_time = 0;

	if (max_time < time0)
	{
		max_time = time0;
	}
	

	m_machine.st->MAIN_CYCLE_TMOUT = max_time;
//	if (ChkOperMode(OM_AUTO) && (time1 > 270))
//	{
//		m_machine.st->MAIN_CYCLE_TMOUT++;
//	}
}

/*******************************************************************************************
 *Brief			:检测CD卡是F519还是F518
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20190927
********************************************************************************************/
void cpld_card_chk()
{
	WORD cdtype = 0;

	//大于2的为手动选择的状态
	if(m_machine.mh->CD_TYPE < 3)
	{
		if(m_cpld_test_card())
		{//继电器
			cdtype = 0;
		}
		else
		{//IO板
			cdtype = 1;
		}

		//每次变更数据时保存一次
		if(cdtype != m_machine.mh->CD_TYPE)
		{			
			m_machine.mh->CD_TYPE = cdtype;
			PartCfgSave((PPART)&m_machine, MH_ID);
		}
	}
}
  
 BOOL ActIODelayLimit(WORD name, WORD err)
 {
	 PMACHINE pp = &m_machine;
 
	 if((ChkOperMode(OM_MANUAL) || ChkOperMode(OM_ADJU))&& ChkSysCfg(XF2_ROPNM)&&(GetPOBit(name) <= VLV_DELAY_TM))
	 {
		 //新增的开模关模平滑功能的原理是在速度斜率结束后再关闭阀，而非以往的阀延迟关，所以这里需要新增平滑模式的判定。
		 for (int i = 1;i<= MAXDAPORT;i++)
		 {
			 if (ChkHydrPosMode(i,HYDRSMSTOP) && ((GetPumpMcModeSet(i)&MC_CTR_MODE_POSI) > 0) )
			 {
				 if (err)
				 {
					 SET_PART_ERR(pp, ER0_IODELAYLIM);
				 }
				 return TRUE;
			 }
		 }
		 
		 if (GetPOBit(pp->pclmp->po->O_CLS_SOL) > VLV_DELAY_TM || GetPOBit(pp->pclmp->po->O_OPN_SOL) > VLV_DELAY_TM
		 || GetPOBit(pp->pejt->po->O_EJT_FWD) > VLV_DELAY_TM || GetPOBit(pp->pejt->po->O_EJT_BWD) > VLV_DELAY_TM 
		 || GetPOBit(pp->pinj->po->O_INJ_SOL) > VLV_DELAY_TM || GetPOBit(pp->pinj->po->O_CHR_SOL) > VLV_DELAY_TM || GetPOBit(pp->pinj->po->O_SUC_SOL) > VLV_DELAY_TM 
		 || GetPOBit(pp->pinj->pnzl->po->O_NOZ_FWD) > VLV_DELAY_TM || GetPOBit(pp->pinj->pnzl->po->O_NOZ_BWD) > VLV_DELAY_TM)
		 {
			 if (err)
			 {
				 SET_PART_ERR(pp, ER0_IODELAYLIM);
			 }
			 return TRUE;
		 }		  
	 }
 
 return FALSE;
 }

BOOL CycleTskIsRun()
{
	return ChkActRun(&m_machine, ACT_SN_BIT(OS_RCYC));
}

UI32 GetCycleTskStartMs()
{
	return m_machine.Recyclestart;
}

UI16 GetMoldAlarmMode()
{
	return m_machine.st->FL_AUTOALARM;
}

void SetMoldAlarmMode(UI16 mode)
{
	m_machine.st->FL_AUTOALARM = mode;
}

UI8 AutoAlarmIsStart()
{
	return m_machine.AlarmStart;
}

/*自动警报的监控参数是否可以更新*/
UI8 AutoAlarmCanUpdate()
{
	//无论在自动警报功能未启动时，都刷新监控数据
	return (m_machine.AlarmStart == 0 && ChkOperMode(OM_AUTO));
}

void SetAutoAlarmStart(UI8 mode)
{
	m_machine.AlarmStart = mode;
}

void AutoAlarmReStart()
{
	PCLAMP pp = GetSys()->pclmp;
	SetAutoAlarmStart(0);

	if(ChkSysCfg(XF0_AUTOARMOD))
	{
		SetMoldAlarmMode(0);
	}

	m_alarm.st->AUTOALARM_SHORTNUM = GetSysMold()->CN_AUTOALARMUNIT;
	pp->last_chk_opennum = ((UI32)(pp->st->CN_MOLDOPNNUM0) << 16) + pp->st->CN_MOLDOPNNUM1;
}

UI16 GetModeChgSpc()   //得到储料连动功能是否使用标记，仅海达判断这个标记
{
	return m_machine.md->FL_SpcMode;
}


BOOL MachineMotorOnSafeChk(PPART pmt)
{
	int	i;
	PMACHINE pp = &m_machine;

	for(i = 0; i < pp->safedoor_num; i++)
	{
		if(BSafeDoorIsErr(pp->psfdr + i))
		{
			SET_PART_ERR(pp->psfdr + i, ER3_RSDR);
			return FALSE;
		}
	}

	return TRUE;
}

UI16 GetOperMode()
{
	return m_machine.st->ST_WOPERMODE;
}

UI16 ChkOperMode(UI16 mask)
{
	if(m_machine.st->ST_WOPERMODE == 0)
		return (mask == 0 || mask == 0xffff);
	else
		return (m_machine.st->ST_WOPERMODE & mask);	
}

UI16 ClrOperMode(UI16 mask)
{
	return (m_machine.st->ST_WOPERMODE &= (~mask));
}

UI16 SetOperMode(UI16 mode)
{
	if(mode != m_machine.st->ST_WOPERMODE)
	{		
		m_machine.st->ST_WOPERMODE = mode;
		PartDataTx((PPART)&m_machine, ST_ID, 1, GET_IND(PMACHINE_STATE, ST_WOPERMODE), TRUE);
	}

	return (m_machine.st->ST_WOPERMODE);
}

void SendOperMode()
{
	PartDataTx((PPART)&m_machine, ST_ID, 1, GET_IND(PMACHINE_STATE, ST_WOPERMODE), TRUE);
}

BOOL CheckSysConfig(UI16  ind, UI16 mask)
{
	if(ind < sizeof(MACHINE_MACHSET)/sizeof(UI16) && m_machine.mh != NULL)
		return ((*((UI16*)m_machine.mh +ind) & mask) != 0);
	else
		return FALSE;
}

void SetSysConfig(UI16  ind, UI16 mask)
{
	if(ind < sizeof(MACHINE_MACHSET)/sizeof(UI16) && m_machine.mh != NULL)
	{
		*((UI16*)m_machine.mh +ind) = *((UI16*)m_machine.mh +ind)| mask;
	}
}

void ClrSysConfig(UI16  ind, UI16 mask)
{
	if(ind < sizeof(MACHINE_MACHSET)/sizeof(UI16) && m_machine.mh != NULL)
	{
		*((UI16*)m_machine.mh +ind) = *((UI16*)m_machine.mh +ind)&(~mask);
	}
}


UI16 GetSysCfg(UI16 ind)
{
	if(ind < sizeof(MACHINE_MACHSET)/sizeof(UI16))
		return *((UI16*)m_machine.mh + ind);
	else
		return 0;
}

PMACHINE_MACHSET GetSysConfig()
{
	return m_machine.mh;
}

PMACHINE_MOLDSET GetSysMold()
{
	return m_machine.md;
}

PMACHINE	GetSys()
{
    return &m_machine;
}

UI16	Set_SysTemCode_tomonni()	//发送主机的系统代码到面板上，不一定是发送主机硬件的型号到面板
{
	UI16	tmp = 0;

	tmp = GetHwModel();

	switch(tmp)
	{
		case MODEL_I3101:	//当硬件型号为3101时，发送给面板的系统代码强制转为3100显示   20210810   yxh
			tmp = MODEL_I3100;
			break;

		case MODEL_I3201:	//当硬件型号为3201时，发送给面板的系统代码强制转为3200显示   20210810   yxh
			tmp = MODEL_I3200;	
			break;

        case MODEL_I3202:	//当硬件型号为3202时，发送给面板的系统代码强制转为3200显示   20220628   ccz
			tmp = MODEL_I3200;	
			break;

		case MODEL_I5201:	//当硬件型号为5201时，发送给面板的系统代码强制转为5200显示   20210810   yxh
			tmp = MODEL_I5200;
			break;

		default:			//其他型号时，保持不变
			break;
	}

	return tmp;
}
/*******************************************************************************************
 *Brief			:获取压力上限
 *Param			:
 *Return Value	:反馈 压力上限 0.1bar
 *Aauthor		:ZT
 *Date			:20190426
********************************************************************************************/
WORD GetSysPresLimit()
{
	return m_machine.mh->DAPRES_SYSLMT*10;
}

BOOL ChkInjectCushErr()
{
	int		i;

	for(i = 0; i < m_machine.inject_num; i++)
	{
		if(CHK_PART_ERR(m_machine.pinj + i, ER0_CUSH) != 0)
			return TRUE;
	}

	return FALSE;
}

BOOL ChkCoreIsRun()
{
	int	i;

	for(i = 0; i < m_machine.core_num; i++)
	{
		if(((PPART)(m_machine.pcore + i))->act_run_flag != 0)
			return TRUE;
	}

	return FALSE;
}

UI16 GetSysPI(UI16 ind)
{
	if(ind < sizeof(MACHINE_PI)/sizeof(UI16))
		return *((UI16*)m_machine.pi+ ind);
	else
		return 0;
}


UI16 GetSysPO(UI16 ind)
{
	if(ind < sizeof(MACHINE_PO)/sizeof(UI16))
		return *((UI16*)m_machine.po + ind);
	else
		return 0;
}

void SetMachineErr(UI32 err)
{
	SET_PART_ERR(&m_machine, err);
}

void ClrMachineErr(UI32 err)
{
	CLR_PART_ERR(&m_machine, err);
}

BOOL ChkMachineErr(UI32 err)
{
		return CHK_PART_ERR(&m_machine, err);
}

void MachineSubPartCmd(UI32 cmd, UI32 para)
{
	int i;
	int j;
	PMACHINE pp = &m_machine;
	PPART tmp;
	
	if(pp == NULL ||pp->sub_tbl_num <= 0)
		return;

	for(i = 0; i < pp->sub_tbl_num; i++)
	{
		tmp = pp->sub_tbl[i].pp;
		
		if(tmp != NULL && GET_PART_TYPE(tmp->id) != MACHINE_ID)
		{
			for(j = 0; j < pp->sub_tbl[i].num; j++)
			{
				CallPartCmd(tmp, cmd, para);
				tmp = (PPART)((int)tmp + pp->sub_tbl[i].part_size);
			}
		}
	}
}

BOOL AllocSubPartMem(PSUB_PART_INFO tbl, int tbl_num)
{
	int i,j;
	UI16	saveext = 0;
	const PART_TYPE_INFO* pinfo;
	//const UI16	extdataid[] = {0x0F,0x0E,0x24,0x2F};
	const UI16	extdataid[] = {0x24,0x2F};
	//将部件内存数据放置于外部内存的部件ID
	//注:该部件的数据必须是UI16及以上位数

	if(tbl == NULL)
		return FALSE;
	
	for(i = 0; i < tbl_num; i++)
	{
		/*alloc all sub part memory*/
		if(tbl[i].num > 0 && (pinfo = GetPartTypeInfo(tbl[i].part_type)) != NULL && pinfo->cat != PART_CAT_SYS && pinfo->cat != PART_CAT_BCTRL 
		 && pinfo->cat != PART_CAT_CTRL && pinfo->init_func != NULL)
		{
			saveext = FALSE;
			
			if(tbl[i].num > MAX_PART_SN)
				tbl[i].num = MAX_PART_SN;

			for(j = 0;j<CAL_LEN(extdataid);j++)
			{
				if(extdataid[j] == tbl[i].part_type)
				{
					saveext = TRUE;
					break;
				}
			}
			
			if(saveext)
			{
				if((tbl[i].pp = AllocMemTbl(MEMTBL_DYNC_MEM_EXT, pinfo->part_size, tbl[i].num)) == NULL)
				{
					/*memory full, fatal error*/
					SetMachineErr(ER0_MEM);
					return FALSE;
				}
			}
			else
			{
				if((tbl[i].pp = AllocMemTbl(MEMTBL_DYNC_MEM, pinfo->part_size, tbl[i].num)) == NULL)
				{
					/*memory full, fatal error*/
					SetMachineErr(ER0_MEM);
					return FALSE;
				}
			}
				
			

			tbl[i].part_size = pinfo->part_size;
		}
		else
		{
			tbl[i].part_size = 0;
			tbl[i].pp = NULL;
		}
	}

	return TRUE;
}

BOOL InitSubPart(PSUB_PART_INFO tbl, int tbl_num)
{
	int i;
	int j;
	const PART_TYPE_INFO* pinfo;
	PPART pp;
	
	if(tbl == NULL)
		return FALSE;
	
	for(i = 0; i < tbl_num; i++)
	{
		pp = tbl[i].pp;
		pinfo = GetPartTypeInfo(tbl[i].part_type);

		if(pp != NULL && pinfo != NULL)
		{
			for(j = 0; j < tbl[i].num; j++)
			{
				if(pinfo->init_func != NULL && pinfo->cat != PART_CAT_BCTRL && pinfo->cat != PART_CAT_SYS && pinfo->cat != PART_CAT_CTRL)
					(*(PART_INIT_FUNC)(pinfo->init_func))(pp, j + 1);
				
				pp = (PPART)((int)pp + tbl[i].part_size);
			}
		}
	}

	return TRUE;
}

void InitMachineVersion()
{
	PMACHINE pp = &m_machine;
	UI32 t_dat;
	UI16* ptmp;
	
	pp->vr->hw_ver.test_tag = 1;
	pp->vr->hw_ver.ver_sn = 10;
	pp->vr->hw_ver.reserve = 0;
	pp->vr->hw_ver.cpu_type = 0;
	pp->vr->hw_ver.cat = 0;

	switch(GetHwModel())
	{
		case MODEL_I5201:
			pp->vr->hw_ver.model = 5201;
			break;
		case MODEL_I3201:
			pp->vr->hw_ver.model = 3201;
			break;
        case MODEL_I3202:
			pp->vr->hw_ver.model = 3202;
			break;
		case MODEL_I3101:
			pp->vr->hw_ver.model = 3101;
			break;
			
		default:
			pp->vr->hw_ver.model = 3101;
			break;
	}

	
	pp->vr->hw_type.drv = 1;
	pp->vr->hw_type.mec = 1 + GetMachineType();
	pp->vr->hw_type.arch = 1;
	pp->vr->hw_type.model = 1;

	pp->vr->sw_ver.db_ver = DB_VER;  		
	pp->vr->sw_ver.bbdd_ver = BBDD_VER;	

	pp->vr->cus_info.cus_sn = CUSTOMER;
		
	pp->vr->cus_info.sw_ver = SW_VER;
	t_dat = BUILD_DATE;

	pp->vr->cus_info.date[0] = (t_dat&0xff);
	pp->vr->cus_info.date[1] = ((t_dat >> 8)&0xff);
	pp->vr->cus_info.date[2] = ((t_dat >> 16)&0xff);
	pp->vr->cus_info.cus_sn = CUSTOMER;

	if((ptmp = GetBootVer()) != NULL)
	{
		pp->vr->sw_ver.boot_ver_1 = *ptmp;
		pp->vr->sw_ver.boot_ver_2= *(ptmp + 1);
	}
	else
	{
		pp->vr->sw_ver.boot_ver_1 = 0;
		pp->vr->sw_ver.boot_ver_2= 0;
	}

	pp->vr->mh_info.power_type = TYPE_POWER;
	pp->vr->mh_info.machine_type= TYPE_MACHINE;
	pp->vr->mh_info.application_type= TYPE_APP;
}

/*从机器的机器参数(code码表)设置，版本信息*/
void MachineVersionSet()
{
	PMACHINE pp = &m_machine;

	switch(GetHwModel())
	{
		case MODEL_I5201:
			pp->vr->hw_ver.model = 5201;
			break;
		case MODEL_I3201:
			pp->vr->hw_ver.model = 3201;
			break;
        case MODEL_I3202:
			pp->vr->hw_ver.model = 3202;
			break;
		case MODEL_I3101:
			pp->vr->hw_ver.model = 3101;
			break;
			
		default:
			pp->vr->hw_ver.model = 3101;
			break;
	}

	/*编译日期由编译日期宏指定*/
	pp->mh->FL_CTRLDATE = (BUILD_DATE%10000)*10 + ((BUILD_DATE/10000)%10);

	/*版本日期由编译版本宏指定*/
	pp->mh->FL_VERSION = SW_SN;
	
	if(ChkSysCfg(CO0_SERVOP))
		pp->vr->hw_type.drv = 2;		//伺服机
	else
		pp->vr->hw_type.drv = 1;		//普通液压

	if(pp->mh->FL_CUSTOMID > 255)
	{
		pp->vr->cus_info.cus_sn = ((pp->mh->FL_CUSTOMID >> 8) & 0xff);
	}

	
}

BOOL InitMachinePart()
{
	PMACHINE pp = &m_machine;
	static const SUB_INIT_INFO sub_init[]  = MACHINE_INIT_INFO;
	static const ACT_INIT_INFO act_init[] = 
	{
		{OS_RCYC, OS_RCYC, ACT_CTRL, RecycleProc, NULL},
		{OS_PRCYC, OS_PRCYC, ACT_CTRL, PreCycleProc, NULL},
		{OS_CALI, OS_CALI, ACT_CTRB, CalibrationSlopeAct,CAggCActOff}
	};
	PTBL ptbl;

	memset(pp, 0, sizeof(*pp));
	
	if(!INIT_PART(pp, MACHINE_ID, 1, sub_init))
		return FALSE;

	if(!INIT_ACT(pp, act_init))
		return FALSE;
	
	ptbl = TBLGet(T_MACHINE_SUB, 1, TBL_DB_VER);
	
	if(ptbl == NULL || ptbl->hd.row_len != sizeof(SUB_PART_INFO))
		pp->sub_tbl_num = 0;
	else
		pp->sub_tbl_num = ptbl->hd.row;
	
	pp->sub_tbl = (PSUB_PART_INFO)(ptbl->prow);

	if(!AllocSubPartMem(pp->sub_tbl, pp->sub_tbl_num))
		return FALSE;

	/*经常使用的部件引用*/
	pp->pclmp = (PCLAMP)GetMachineSubPartList(CLAMP_ID, &pp->clamp_num);
	pp->psfdr = (PSAFEDOOR)GetMachineSubPartList(SAFEDOOR_ID, &pp->safedoor_num);
	pp->pejt = (PEJECT)GetMachineSubPartList(EJECT_ID, &pp->eject_num);
	pp->prtt = (PROTATE)GetMachineSubPartList(ROTATE_ID, &pp->rotate_num);
	pp->prbt= (PROBOT)GetMachineSubPartList(ROBOT_ID, &pp->robot_num);
	pp->pinjstr= (PINJSTOR)GetMachineSubPartList(INJSTOR_ID, &pp->injstor_num);
	pp->pinj= (PINJECT)GetMachineSubPartList(INJECT_ID, &pp->inject_num);
	pp->pcore= (PCORE)GetMachineSubPartList(CORE_ID, &pp->core_num);
	pp->plub= (PLUB)GetMachineSubPartList(LUB_ID, &pp->lub_num);
	pp->pservoecat = (PSERVOECAT)GetMachineSubPart(ECAT_ID, 1);
	pp->ptempcomm = (PTEMPCTRLCOMM)GetMachineSubPartList(TEMPCTRLCOMM_ID,&pp->tempcomm_num);
		
	if(!InitSubPart(pp->sub_tbl, pp->sub_tbl_num))
		return FALSE;
	
	SetMotorOnSafeChk(MachineMotorOnSafeChk);
	InitMachineVersion();

	pp->st->WTIME_AGE  = Zerodata_AgeTM();

	pp->Recyclestart = 0;
	pp->cyclestart = 0;
	pp->mh->FL_CTRLMODEL = Set_SysTemCode_tomonni();
	pp->last_da_mode = ChkSysCfg(CO8_YL4);
	MachineCmd(CMD_SETUP, TRUE);

	InitFuncSel();//部分特殊功能开关初始化;
	
	InitSeqVlvMain();
	InitAiClamp();
	InitMotorControlMode();
    InitMaxLoop100us();
	CPU_Need_Reset_Init();
    
	return TRUE;
}

UI32 MachineCmd(UI32 cmd, UI32 para)
{
	int	i = 0;
	UI32 cyctime = 0;	//循环总计时
	UI32 qc_min = 0,qc_max = 0,qc_time = 0;
	UI16 offset = 0;
	UI16 len = 0;
	BOOL ret = TRUE;
	UI16 motor_enable_code = 0;
	
	PMACHINE pp = &m_machine;
	PADMETER pad = (PADMETER)GetPartByID(MK_PART(ADMETER_ID, 1));

    PMOTOR pmtr = (PMOTOR)GetPartByID(MK_PART(MOTOR_ID, 1));
	PADJUST padj = (PADJUST)GetMachineSubPart(ADJUST_ID, 1);
    PINJECT pinj = (PINJECT)GetMachineSubPart(INJECT_ID, 1);
	
	switch(GET_CMD(cmd))
	{
		case MH_CMD_ROBOT_EM:
			for(i = 0; i < pp->robot_num; i++)
			{
				RobotEmergeStop(pp->prbt + i, para);
				RobotEmergeStop2(pp->prbt + i, para);
			}
			break;

		case MH_CMD_SFDR_CH:
			for(i = 0; i < pp->safedoor_num; i++)

				SafeDoorAlarmLoop(pp->psfdr + i);

			/*修改全程计时显示*/
			if(ChkOperMode(OM_AUTO))
				cyctime = DIFF_MS(pp->Recyclestart)/10;
			m_machine.st->TM_CYCLE_L = (UI16)(cyctime & 0x0ffff);
			m_machine.st->TM_CYCLE_H = ((UI16)(cyctime >> 16) & 0x0ffff);
			break;

		case MH_CMD_LOOP_CHK:
			if(MotorIsOn())
			{			
				for(i = 0; i < pp->injstor_num; i++)
					InjStorLoop(pp->pinjstr + i);	
			}

			for(i = 0; i < pp->robot_num; i++)
			{
				EuroRobotChkLoop(pp->prbt + i);
				MoldChkLoop(pp->prbt + i);
			}

			if(CycleTimeErrChk(pp))
			{
				if(pp->md->FL_TIME_ERR == 1)
				{}
				else
					ClearAllActTask();
			}
			/*2015-02-03 通用锁模锁死保护*/
			//if(!TestPI(pp->pclmp->pi->I_CLS_END))
			if (!ChkAdFwdInplace(pp->pclmp->mh->CLMP_METERCH))
			{
				CLR_PART_ERR(pp->pclmp, ER0_CLSLOCK);
				pp->pclmp->clscmphitm  = GetSecTick();
			}
			else
			{
				if(pp->pclmp->mh->TM_CLSHI_DEFAULT > 600)
					pp->pclmp->mh->TM_CLSHI_DEFAULT = 0;
				if(((GetSecTick()-pp->pclmp->clscmphitm)>(pp->pclmp->mh->TM_CLSHI_DEFAULT * 60))&&(pp->pclmp->mh->TM_CLSHI_DEFAULT>0)&&ChkOperMode(OM_MANUAL))
				{
					if(!CHK_PART_ERR(pp->pclmp, ER0_CLSLOCK))
					{
		        		SET_PART_ERR(pp->pclmp, ER0_CLSLOCK);
		        		StandAlarm(TRUE);
					}
				}
			}

			break;
			
		case CMD_MANUAL_INIT:
			/*转到手动时，机器初始化*/
			//CreateActForkTbl(OM_MANUAL);
			MachineSubPartCmd(cmd, para);
			if(pmtr->st->FL_MOTORSTATUS == MOTOR_STARTING)
			{
				MotorStateSet(MOTOR_OFF);
			}
            pinj->pchgprs->chgprsflag = 0;
			//CycleStart(&m_machine);
			break;
			
		case MH_CMD_AUTO_CYCLE:
			for(i = 0; i < pp->robot_num; i++)
				RobotAutoON(pp->prbt + i);

			//CycleTmReset(pp);
			ReCycleStart(pp);
			if(pp->md->Qc_Fun ==1)
			{
				pp->st->Qc_Opn_Cnt = 0;
				pp->qc_chk_start   = 0;
			}

			if (pp->pclmp->last_chk_opennum == 0)
			{
				AutoAlarmReStart();
			}
			
			
			ForkAct(pp->act + OS_RCYC -1, FIRST_FORK, NULL, 0, 0);
			break;
		case CMD_DATA_RX:
			if((GET_CMD_PARA(cmd) & 0xff) == ST_ID)
			{				
				/*判读是否是自动警报设置*/
				offset = (para >> 16 & 0xffff);
				len = (para & 0xffff);

				if (len == 1)
				{
					if (GET_IND(PMACHINE_STATE, FL_AUTOALARM) == offset)
					{
						AutoAlarmReStart();
					}					
				}	
			}
			else if((GET_CMD_PARA(cmd) & 0xff) == MH_ID)
			{
				if(ChkSysCfg(CO1_ECAT))
				{
					//使用Ecat时，必须使用主机压力控制
					SetSysCfg(CO0_SERVOP);
				}					

				/*由于配置工具没有设置，这里检查保护, need t0 chg*/
				if(pp->mh->DAPRES_BACKLIMIT == 0)
					pp->mh->DAPRES_BACKLIMIT = 70;
				
				if(pp->mh->DAPRES_CHARGELIMIT == 0)
					pp->mh->DAPRES_CHARGELIMIT = 140;

				if(pp->mh->DAFLOW_CHARGELIMIT == 0)
					pp->mh->DAFLOW_CHARGELIMIT = 60;

				SetModelIOMax();
				MachineVersionSet();
				pp->mh->FL_CTRLMODEL = Set_SysTemCode_tomonni();

				if (pp->cur_cus_id != pp->mh->FL_CUSTOMID)
				{
					pp->cur_cus_id = pp->mh->FL_CUSTOMID;
					OilPortTbCopy();
				}

				if (ComIsOk())
				{
					if (pp->motor_enable_mode != ChkSysCfg(XF1_HTMT))
					{
						pp->motor_enable_mode = ChkSysCfg(XF1_HTMT);

						if (pp->motor_enable_mode > 0)
						{
							motor_enable_code = 1;
						}
						else
						{
							motor_enable_code = 14;
						}

						for (i = 0; i < PUMP_ECAT_NUM; i++)
					    {//输入驱动器密码
							SdoDtDataUpdata(i, 0x2000, motor_enable_code, 0x0302, 16);
					    }
					}

					
				}
			}
			else if ((GET_CMD_PARA(cmd) & 0xff) == MD_ID)
			{
				/*判读是否是自动警报设置*/
				offset = (para >> 16 & 0xffff);
				len = (para & 0xffff);
				
				if (GET_IND(PMACHINE_MOLDSET, CN_AUTOALARMUNIT) == offset)
				{
					m_alarm.st->AUTOALARM_SHORTNUM = pp->md->CN_AUTOALARMUNIT;
				}

				if (pp->hydr_way_last != pp->md->FL_HYDY_WAY_SET)
				{
					pp->hydr_way_last = pp->md->FL_HYDY_WAY_SET;
					OilPortTbCopy();
				}
                
                GetMainPortBit();
			}
			else if ((GET_CMD_PARA(cmd) & 0xff) == VR_ID)
			{
				InitMachineVersion();	//当FLASH数据内的版本号覆盖时，刷新一遍
			}
			break;			
		case CMD_SETUP:
			MachineSubPartCmd(MAKE_CMD(0, CMD_SETUP), 0);
			break;
		case CMD_QC_PARA:
			if(ChkOperMode(OM_AUTO) && (pp->md->Qc_Fun==1))
			{
				if(pp->md->Qc_Mode_CycTm== 1)  //射出时间QC数据
				{
					qc_time = ((UI32)pp->st->TM_CYCLE_LAST_H << 16) + (UI32)pp->st->TM_CYCLE_LAST_L;
					if(pp->st->Qc_Opn_Cnt == 1)
					{
						pp->qc_avg_cyctm = qc_time;
						qc_min = qc_time;
						qc_max = qc_time;
						pp->st->Qc_Unq_Cnt_CycTm = 0;
					}
					else
					{
						pp->qc_avg_cyctm = pp->qc_avg_cyctm+qc_time;
						qc_min = (qc_min > qc_time)?qc_time: qc_min;
						qc_max = (qc_max < qc_time)?qc_time: qc_max;
					}
					pp->st->Qc_Min_CycTm_L = (UI16)(qc_min & 0x0ffff);
					pp->st->Qc_Min_CycTm_H = (UI16)((qc_min >> 16) & 0x0ffff);
					
					pp->st->Qc_Max_CycTm_L = (UI16)(qc_max & 0x0ffff);
					pp->st->Qc_Max_CycTm_H = (UI16)((qc_max >> 16) & 0x0ffff);
					
					if(((qc_time>((UI32)pp->md->Qc_TmSet_CycTm+ (UI32)pp->md->Qc_DeviationSet_CycTm))
						|| (qc_time<((UI32)pp->md->Qc_TmSet_CycTm - (UI32)pp->md->Qc_DeviationSet_CycTm)))&&((pp->st->Qc_Opn_Cnt) <=pp->md->Qc_OpnNum))
			 			pp->st->Qc_Unq_Cnt_CycTm = pp->st->Qc_Unq_Cnt_CycTm+1;								
				}
			}
			 break;		
		default:
			ret = FALSE;
			break;
	}

	return ret;
}

PPART GetMachineSubPartList(UI8 type, UI8* pnum)
{
	int i;
	PMACHINE pp = &m_machine;
	UI8 num;
	PPART ret;

	num = 0;
	ret = NULL;
	
	for(i = 0; i < pp->sub_tbl_num; i++)
	{
		if(pp->sub_tbl[i].part_type == type)
		{
			if(pp->sub_tbl[i].pp != NULL)
			{
				num = pp->sub_tbl[i].num;
				ret = pp->sub_tbl[i].pp;
			}
			
			break;
		}
	}

	if(pnum != NULL)
		*pnum = num;
	
	return ret;
}

PPART GetMachineSubPart(UI8 type, UI8 sn)
{
	void* ret = NULL;
	PMACHINE pp = &m_machine;
	int i;
	
	if(sn <= 0)
		sn = 1;

	ret = NULL;
	for(i = 0; i < pp->sub_tbl_num; i++)
	{
		if(pp->sub_tbl[i].part_type == type)
		{
			if(pp->sub_tbl[i].num >= sn && pp->sub_tbl[i].part_size > 0 && pp->sub_tbl[i].pp != NULL)
			{
				ret = (PPART)((int)(pp->sub_tbl[i].pp) + pp->sub_tbl[i].part_size*(sn - 1));	
				break;
			}
		}
	}

	if(ret != NULL && !CHK_PART_ST(ret, PART_DISABLE))
		return (PPART)ret;
	else
		return NULL;
}

int GetMachineSubPartNum(UI8 type)
{
	PMACHINE pp = &m_machine;
	int i;
	
	for(i = 0; i < pp->sub_tbl_num; i++)
	{
		if(pp->sub_tbl[i].part_type == type)
		{
			return pp->sub_tbl[i].num;
		}
	}

	return 0;
}

void MachineSysCfgModify()
{   
  	PartCfgSave((PPART)&m_machine, MH_ID);
  	limttoline();
}

void SetOperTM()
{
	UI32 ticks;

	ticks = GetMsTick();
	m_machine.st->TM_OPER_LOWORD = (ticks & 0xffff);
	m_machine.st->TM_OPER_HIGHWORD = ((ticks >> 16) & 0xffff);
}

//联动模式选择
int GetJointActSel() //开托/中子联动 或 开储联动
{
	if(ChkSysCfg(CO1_SOEC) && GetSysMold()->FL_OPNEJTCORE < CLAMP_MAX_CORE+2)
		return GetSysMold()->FL_OPNEJTCORE;
	else if(ChkSysCfg(CO1_OPDC) && GetSysMold()->FL_OPNDURINGCOOL==1)	// && GetSysMold()->FL_OPNEJTCORE==6)
		return (CLAMP_MAX_CORE+2);
	else
		return 0;
}

int GetJointActSel2() //关模中子联动
{
	if(ChkSysCfg(CO1_SOEC) && GetSysMold()->FL_CLOSJTCORE < CLAMP_MAX_CORE+1)
		return GetSysMold()->FL_CLOSJTCORE;
	else
		return 0;
}
/*        QC监控参数                 2014-07-06*/

void GetQcPara()
{
	PMACHINE pp = &m_machine;
	UI32 qc_cyc_avg, qc_cyc_dev;
	UI32 qc_inj_avg, qc_inj_dev;
	
	if(ChkOperMode(OM_AUTO) && (pp->md->Qc_Fun==1))
	{
		if(pp->st->Qc_Opn_Cnt>=pp->md->Qc_OpnNum)
		{
			if(pp->pclmp->md->Qc_Mode_OpnEnd == 1)
			{
				pp->pclmp->st->Qc_Avg_OpnEnd=( (pp->pclmp->qc_avg_opnend)/(pp->st->Qc_Opn_Cnt));  
				pp->pclmp->st->Qc_Deviation_Max_OpnEnd  =  pp->pclmp->st->Qc_Max_OpnEnd - pp->pclmp->st->Qc_Min_OpnEnd;
				pp->pclmp->st->Qc_Unq_Rate_OpnEnd= (pp->pclmp->st->Qc_Unq_Cnt_OpnEnd*100/pp->st->Qc_Opn_Cnt);
			}
			if(pp->pclmp->md->Qc_Mode_OpnSpd== 1)
			{
				pp->pclmp->st->Qc_Avg_OpnSpd= pp->pclmp->qc_avg_opnspd/pp->st->Qc_Opn_Cnt;  
				pp->pclmp->st->Qc_Deviation_Max_OpnSpd  =  pp->pclmp->st->Qc_Max_OpnSpd - pp->pclmp->st->Qc_Min_OpnSpd;
				pp->pclmp->st->Qc_Unq_Rate_OpnSpd= pp->pclmp->st->Qc_Unq_Cnt_OpnSpd*100/pp->st->Qc_Opn_Cnt;
			}			
			if(pp->pclmp->md->Qc_Mode_OpnTm== 1)
			{
				pp->pclmp->st->Qc_Avg_OpnTm= pp->pclmp->qc_avg_opntm/pp->st->Qc_Opn_Cnt;  
				pp->pclmp->st->Qc_Deviation_Max_OpnTm  =  pp->pclmp->st->Qc_Max_OpnTm - pp->pclmp->st->Qc_Min_OpnTm;
				pp->pclmp->st->Qc_Unq_Rate_OpnTm= pp->pclmp->st->Qc_Unq_Cnt_OpnTm*100/pp->st->Qc_Opn_Cnt;
			}					
			if(pp->pclmp->md->Qc_Mode_ClsTm== 1)
			{
				pp->pclmp->st->Qc_Avg_Cls= pp->pclmp->qc_avg_opnclstm/pp->st->Qc_Opn_Cnt;  
				pp->pclmp->st->Qc_Deviation_Max_Cls=  pp->pclmp->st->Qc_Max_Cls- pp->pclmp->st->Qc_Min_Cls;
				pp->pclmp->st->Qc_Unq_Rate_Cls= pp->pclmp->st->Qc_Unq_Cnt_Cls*100/pp->st->Qc_Opn_Cnt;
			}		
			if(pp->pclmp->md->Qc_Mode_ClsLow== 1)
			{
				pp->pclmp->st->Qc_Avg_ClsLow= pp->pclmp->qc_avg_opnclslow/pp->st->Qc_Opn_Cnt;  
				pp->pclmp->st->Qc_Deviation_Max_ClsLow=  pp->pclmp->st->Qc_Max_ClsLow- pp->pclmp->st->Qc_Min_ClsLow;
				pp->pclmp->st->Qc_Unq_Rate_ClsLow= pp->pclmp->st->Qc_Unq_Cnt_ClsLow*100/pp->st->Qc_Opn_Cnt;
			}				
			if(pp->pclmp->md->Qc_Mode_ClsHi== 1)
			{
				pp->pclmp->st->Qc_Avg_ClsHi= pp->pclmp->qc_avg_opnclshi/pp->st->Qc_Opn_Cnt;  
				pp->pclmp->st->Qc_Deviation_Max_ClsHi=  pp->pclmp->st->Qc_Max_ClsHi- pp->pclmp->st->Qc_Min_ClsHi;
				pp->pclmp->st->Qc_Unq_Rate_ClsHi= pp->pclmp->st->Qc_Unq_Cnt_ClsHi*100/pp->st->Qc_Opn_Cnt;
			}		
			if(pp->md->Qc_Mode_CycTm== 1)
			{
				qc_cyc_avg = pp->qc_avg_cyctm/pp->st->Qc_Opn_Cnt;
				pp->st->Qc_Avg_CycTm_L = (UI16)(qc_cyc_avg & 0x0ffff);	//平均值的低16位  20200221
				pp->st->Qc_Avg_CycTm_H = (UI16)((qc_cyc_avg >> 16) & 0x0ffff);	//平均值的高16位  20200221
				//pp->st->Qc_Avg_CycTm= pp->qc_avg_cyctm/pp->st->Qc_Opn_Cnt;  

				qc_cyc_dev = (((UI32)pp->st->Qc_Max_CycTm_H << 16) + (UI32)pp->st->Qc_Max_CycTm_L) - (((UI32)pp->st->Qc_Min_CycTm_H << 16) + (UI32)pp->st->Qc_Min_CycTm_L);
				pp->st->Qc_Deviation_Max_CycTm_L = (UI16)(qc_cyc_dev & 0x0ffff);	//最大偏差值的低16位  20200221
				pp->st->Qc_Deviation_Max_CycTm_H = (UI16)((qc_cyc_dev >> 16) & 0x0ffff);	//最大偏差值的高16位  20200221
				//pp->st->Qc_Deviation_Max_CycTm=  pp->st->Qc_Max_CycTm- pp->st->Qc_Min_CycTm;
				
				pp->st->Qc_Unq_Rate_CycTm= pp->st->Qc_Unq_Cnt_CycTm*100/pp->st->Qc_Opn_Cnt;
			}			
			if(pp->pejt->md->Qc_Mode_EjtTm== 1)
			{
				pp->pejt->st->Qc_Avg_EjtTm= pp->pejt->qc_avg_ejttm/pp->st->Qc_Opn_Cnt;  
				pp->pejt->st->Qc_Deviation_Max_EjtTm=  pp->pejt->st->Qc_Max_EjtTm- pp->pejt->st->Qc_Min_EjtTm;
				pp->pejt->st->Qc_Unq_Rate_EjtTm= pp->pejt->st->Qc_Unq_Cnt_EjtTm*100/pp->st->Qc_Opn_Cnt;
			}		
			if(pp->pinj->md->Qc_Mode_ChgEnd== 1)
			{
				pp->pinj->st->Qc_Avg_ChgEnd= pp->pinj->qc_avg_chgend/pp->st->Qc_Opn_Cnt;  
				pp->pinj->st->Qc_Deviation_Max_ChgEnd=  pp->pinj->st->Qc_Max_ChgEnd- pp->pinj->st->Qc_Min_ChgEnd;
				pp->pinj->st->Qc_Unq_Rate_ChgEnd= pp->pinj->st->Qc_Unq_Cnt_ChgEnd*100/pp->st->Qc_Opn_Cnt;
			}		
			if(pp->pinj->md->Qc_Mode_ChgTm== 1)
			{
				pp->pinj->st->Qc_Avg_ChgTm= pp->pinj->qc_avg_chgtm/pp->st->Qc_Opn_Cnt;  
				pp->pinj->st->Qc_Deviation_Max_ChgTm=  pp->pinj->st->Qc_Max_ChgTm- pp->pinj->st->Qc_Min_ChgTm;
				pp->pinj->st->Qc_Unq_Rate_ChgTm= pp->pinj->st->Qc_Unq_Cnt_ChgTm*100/pp->st->Qc_Opn_Cnt;
			}		
			if(pp->pinj->md->Qc_Mode_HoldEnd== 1)
			{
				pp->pinj->st->Qc_Avg_HoldEnd= pp->pinj->qc_avg_holdend/pp->st->Qc_Opn_Cnt;  
				pp->pinj->st->Qc_Deviation_Max_HoldEnd=  pp->pinj->st->Qc_Max_HoldEnd- pp->pinj->st->Qc_Min_HoldEnd;
				pp->pinj->st->Qc_Unq_Rate_HoldEnd= pp->pinj->st->Qc_Unq_Cnt_HoldEnd*100/pp->st->Qc_Opn_Cnt;
			}	
			if(pp->pinj->md->Qc_Mode_InjEnd== 1)
			{
				pp->pinj->st->Qc_Avg_InjEnd= pp->pinj->qc_avg_injend/pp->st->Qc_Opn_Cnt;  
				pp->pinj->st->Qc_Deviation_Max_InjEnd=  pp->pinj->st->Qc_Max_InjEnd- pp->pinj->st->Qc_Min_InjEnd;
				pp->pinj->st->Qc_Unq_Rate_InjEnd= pp->pinj->st->Qc_Unq_Cnt_InjEnd*100/pp->st->Qc_Opn_Cnt;
			}	
			if(pp->pinj->md->Qc_Mode_InjPrs== 1)
			{
				pp->pinj->st->Qc_Avg_InjPrs= pp->pinj->qc_avg_injpres/pp->st->Qc_Opn_Cnt;  
				pp->pinj->st->Qc_Deviation_Max_InjPrs=  pp->pinj->st->Qc_Max_InjPrs- pp->pinj->st->Qc_Min_InjPrs;
				pp->pinj->st->Qc_Unq_Rate_InjPrs= pp->pinj->st->Qc_Unq_Cnt_InjPrs*100/pp->st->Qc_Opn_Cnt;
			}	
			if(pp->pinj->md->Qc_Mode_InjSpd== 1)
			{
				pp->pinj->st->Qc_Avg_InjSpd= pp->pinj->qc_avg_injspd/pp->st->Qc_Opn_Cnt;  
				pp->pinj->st->Qc_Deviation_Max_InjSpd=  pp->pinj->st->Qc_Max_InjSpd- pp->pinj->st->Qc_Min_InjSpd;
				pp->pinj->st->Qc_Unq_Rate_InjSpd= pp->pinj->st->Qc_Unq_Cnt_InjSpd*100/pp->st->Qc_Opn_Cnt;
			}
			if(pp->pinj->md->Qc_Mode_InjTm== 1)
			{
				qc_inj_avg = pp->pinj->qc_avg_injtm/pp->st->Qc_Opn_Cnt; 
				pp->pinj->st->Qc_Avg_InjTm_L = (UI16)(qc_inj_avg & 0x0ffff);	//平均值的低16位  20200224
				pp->pinj->st->Qc_Avg_InjTm_H = (UI16)((qc_inj_avg >> 16) & 0x0ffff);	//平均值的高16位  20200224
				
				qc_inj_dev =  (((UI32)pp->pinj->st->Qc_Max_InjTm_H << 16) + (UI32)pp->pinj->st->Qc_Max_InjTm_L) - (((UI32)pp->pinj->st->Qc_Min_InjTm_H << 16) + (UI32)pp->pinj->st->Qc_Min_InjTm_L);
				pp->pinj->st->Qc_Deviation_Max_InjTm_L = (UI16)(qc_inj_dev & 0x0ffff);		//最大偏差值的低16位  20200224
				pp->pinj->st->Qc_Deviation_Max_InjTm_H = (UI16)((qc_inj_dev >> 16) & 0x0ffff);	//最大偏差值的高16位   20200224
				
				pp->pinj->st->Qc_Unq_Rate_InjTm= pp->pinj->st->Qc_Unq_Cnt_InjTm*100/pp->st->Qc_Opn_Cnt;
			}		
			if(pp->pinj->md->Qc_Mode_SuckBwd== 1)
			{
				pp->pinj->st->Qc_Avg_SuckBwd= pp->pinj->qc_avg_suckend/pp->st->Qc_Opn_Cnt;  
				pp->pinj->st->Qc_Deviation_Max_SuckBwd=  pp->pinj->st->Qc_Max_SuckBwd- pp->pinj->st->Qc_Min_SuckBwd;
				pp->pinj->st->Qc_Unq_Rate_SuckBwd= pp->pinj->st->Qc_Unq_Cnt_SuckBwd*100/pp->st->Qc_Opn_Cnt;
			}					
			if(pp->pinj->md->Qc_Mode_SuckTm== 1)
			{
				pp->pinj->st->Qc_Avg_SuckTm= pp->pinj->qc_avg_sucktm/pp->st->Qc_Opn_Cnt;  
				pp->pinj->st->Qc_Deviation_Max_SuckTm=  pp->pinj->st->Qc_Max_SuckTm- pp->pinj->st->Qc_Min_SuckTm;
				pp->pinj->st->Qc_Unq_Rate_SuckTm= pp->pinj->st->Qc_Unq_Cnt_SuckTm*100/pp->st->Qc_Opn_Cnt;
			}				
		}
	}	
}
 UI16 GetMachineType()
 {
 	return (m_machine.mh->FL_MACHTYPE & 0xf);
 }

BOOL ChkMachType(MACHINE_TYPE type)
{
	return ((m_machine.mh->FL_MACHTYPE & 0xf) == type);
}
 
UI16 GetCustomID(void)
{
	if(m_machine.mh != NULL)
		return (m_machine.mh->FL_CUSTOMID & 0xff00);
	else
		return 0;
}

void ShowExtOnline()
{
	PMACHINE pp = &m_machine;
	UI8 changok = 0;
	UI8 changeflag = 0;	
	UI8 change_501 = 0;
	int i;
    if((UI16)(Get100MsTick()-pp->extonlinetime) >= 20)
	{
		pp->extonlinetime = Get100MsTick();
	    for (i=0; i<LIO_MAX_NUM; i++)
	    {
	        if((ChkSysCfg(XF6_EXTIO) && i == 0) || (ChkSysCfg(XF6_EXTIO2) && i == 1)) //目前最多支持2块
            {   
    	        if (node_oncheck[CAN_LSPDIO_KIND][i+1] == true && !(pp->st->EXTIOST & (1<<i)))
    	        {
                    pp->st->EXTIOST |= 1<<i;
    				changeflag = 1;
    				if(CHK_PART_ERR(pp, ER0_IOLINK))
    				    CLR_PART_ERR(pp, ER0_IOLINK);
    	        }
    			else if(node_oncheck[CAN_LSPDIO_KIND][i+1] == false && pp->st->EXTIOST & (1<<i))
    			{
    				pp->st->EXTIOST &= (~(1<<i));
    				changeflag = 1;
    				SET_PART_ERR(pp,  ER0_IOLINK);
    			}
    			else
    				node_oncheck[CAN_LSPDIO_KIND][i+1] = false;
            }
            else
            {
                if(pp->st->EXTIOST & (1<<i))
                    changeflag = 1;
                pp->st->EXTIOST &= (~(1<<i));
                node_oncheck[CAN_LSPDIO_KIND][i+1] = false;
            }
	    }
		
	    for (i=0; i<DA_MAX_NUM; i++)
	    {
	        if(ChkSysCfg(XF6_EXTDA) && i == 0) //目前最多支持1块
            {   
    	        if (node_oncheck[CAN_DA_KIND][i+1] == true && !(pp->st->EXTDAST & (1<<i)))
    	        {
                    pp->st->EXTDAST |= 1<<i;
    				changeflag = 1;
    				if(CHK_PART_ERR(pp, ER0_DALINK))
    				    CLR_PART_ERR(pp, ER0_DALINK);

    	        }
    			else if(node_oncheck[CAN_DA_KIND][i+1] == false && pp->st->EXTDAST & (1<<i))
    			{
    				pp->st->EXTDAST &= (~(1<<i));
    				changeflag = 1;
    				SET_PART_ERR(pp,  ER0_DALINK);
    			}
    			else
    				node_oncheck[CAN_DA_KIND][i+1] = false;
            }
            else
            {
                if(pp->st->EXTDAST & (1<<i))
                    changeflag = 1;
                pp->st->EXTDAST &= (~(1<<i));
                node_oncheck[CAN_DA_KIND][i+1] = false;
            }
	    }
		
		for (i=0; i<TEMP_MAX_NUM; i++)
	    {
	    	if (i < g_tempcomm_num && ChkTempMachTypeE50X())
	    	{
				PTEMPCTRLCOMM ptmpcom = g_tempcomm[i];
				if (ptmpcom == NULL)
				{
					continue;
				}
			
		        if (node_oncheck[CAN_TEMPE502_KIND][i+1] == true && !(pp->st->EXT_E502_ST & (1<<i)))
		        {
		        	if (CHK_PART_ERR(ptmpcom, ER0_E502_ERR))
					{
						CLR_PART_ERR(ptmpcom, ER0_E502_ERR);
		        	}
					
	                pp->st->EXT_E502_ST |= 1<<i;
					changok = 1;
		        }
				else if (node_oncheck[CAN_TEMPE502_KIND][i+1] == false && pp->st->EXT_E502_ST & (1<<i))
				{
					SET_PART_ERR(ptmpcom, ER0_E502_ERR);
					pp->st->EXT_E502_ST &= (~(1<<i));
					changok = 1;
				}
				else
					node_oncheck[CAN_TEMPE502_KIND][i+1] = false;
			}			
	    }


		for (i=0; i<TEMPMW_MAX_NUM; i++)
	    {
	    	if (i < g_tempmw_num)
	    	{
				PTEMPCTRLMW pmw = g_temp_mw[i];
				if (pmw == NULL)
				{
					continue;
				}

                if(pmw->md->FL_USE_MW == 1)
                {
    		        if (node_oncheck[CAN_HSPDTEMP_KIND][i+1] == true && !(pp->st->EXT_E501_ST & (1<<i)))
    		        {
    		        	if (CHK_PART_ERR(pmw, ER0_E501_ERR))
    					{
    						CLR_PART_ERR(pmw, ER0_E501_ERR);
    		        	}
    					
    	                pp->st->EXT_E501_ST |= 1<<i;
    					change_501 = 1;
    		        }
    				else if (node_oncheck[CAN_HSPDTEMP_KIND][i+1] == false && pp->st->EXT_E501_ST & (1<<i))
    				{
    					SET_PART_ERR(pmw, ER0_E501_ERR);
    					pp->st->EXT_E501_ST &= (~(1<<i));
    					change_501 = 1;
    				}
    				else
    					node_oncheck[CAN_HSPDTEMP_KIND][i+1] = false;
                }
                else
                {
                    if(pp->st->EXT_E501_ST & (1<<i))
                        change_501 = 1;
                    pp->st->EXT_E501_ST &= (~(1<<i));
                    node_oncheck[CAN_HSPDTEMP_KIND][i+1] = false;
                }
			}			
	    }
		
		if(changeflag==1)
		    PartDataTx((PPART)&m_machine, ST_ID, 3, GET_IND(PMACHINE_STATE, EXTIOST), FALSE);		    
		
		if(changok == 1)
			PartDataTx((PPART)&m_machine, ST_ID, 2, GET_IND(PMACHINE_STATE, EXT_E505_ST), FALSE);

		if(change_501 == 1)
			PartDataTx((PPART)&m_machine, ST_ID, 1, GET_IND(PMACHINE_STATE, EXT_E501_ST), FALSE);
	}
}

void MachineCfgTx()
{
	PMACHINE  pm	 =	&m_machine;

	pm->mh->DA_LINE = inner_pa.pa2;
	inner_pa.pa1 = inner_pa.pa2;      //主机重置时，清除pa1中保存的上次的线性类型
	SaveInnerPara();
	PartDataTx((PPART)(&m_machine), MH_ID, CAL_LEN(*m_machine.mh), 0, TRUE);
}

/*在，每个全自动周期前清除机器的动作实时计算时间*/
void ClearMacineActTime()
{
	PNOZZLE	pnzl = (PNOZZLE)GetMachineSubPart(NOZZLE_ID, 1);
	
	if(m_machine.pclmp != NULL && m_machine.pclmp->st != NULL)
	{
		m_machine.pclmp->st->TM_CLAMPCLS_RT = 0;
		m_machine.pclmp->st->TM_CLAMPOPN_RT = 0;
	}
	
	if(m_machine.pinj!= NULL && m_machine.pinj->st != NULL)
	{
		m_machine.pinj->st->TM_INJ_RT_L= 0;
		m_machine.pinj->st->TM_HOLD_RT_L= 0;
		m_machine.pinj->st->TM_SUCKBK_RT= 0;
		m_machine.pinj->st->TM_CHARGE_RT= 0;

		if(m_machine.pinj->pnzl != NULL)
		{
			m_machine.pinj->pnzl->st->TM_COOL_RT_L = 0;
			m_machine.pinj->pnzl->st->TM_COOL_RT_H = 0;
		}
	}

}

void LubIOCheck()
{
	int i;
	PLUB	plub[CLAMP_MAX_LUB];
	PMOTOR pmtr = (PMOTOR)GetPartByID(MK_PART(MOTOR_ID, 1));

	for(i = 0; i < CLAMP_MAX_LUB; i++)
	{
		plub[i] = (PLUB)GetMachineSubPart(LUB_ID, i + 1);

		if(plub[i] == NULL)
			return;

		if(plub[i]->mh->FL_LUBMODE!=LUB_ONE)
		{
			if(CheckPIUsed(plub[i]->pi->I_LUB_CHK))
			{
				if(TestPI(plub[i]->pi->I_LUB_CHK)&& (LubStartFirst == 1))
				{
					if(DIFF_10MS(LubStart) >= plub[i]->mh->TM_SWITCHCHECK)
					{
						SET_PART_ERR(plub[i], ER1_LUBERR);
						LubStartFirst = 0;
					}
				}
				else
				{
					LubStartFirst = 1;
					LubStart = Get10MsTick();
				}
			}
		}
	}
}

void CoolTMCount()
{
		PMACHINE pp = &m_machine;

		if(pp->pinj == NULL || pp->pinj->pnzl == NULL)
			return;

		if(ChkOperMode(OM_AUTO))
		{
			if(pp->pinj->pnzl->coolsts==1)		
			{
				pp->pinj->pnzl->st->TM_COOL_RT_L = (UI16)(((GetMsTick()- coolstart)/10) & 0x0ffff);
				pp->pinj->pnzl->st->TM_COOL_RT_H = (UI16)((((GetMsTick()- coolstart)/10) >> 16) & 0x0ffff);
			}
		}
		else		//避免切换手动时面板计算倒计时出错
		{
			pp->pinj->pnzl->coolsts = 0;
			pp->pinj->pnzl->st->TM_COOL_RT_L = 0;
			pp->pinj->pnzl->st->TM_COOL_RT_H = 0;
		}
}


void	TY_VolCheck()	          //	相序检知		通用专用
{
	PSERVO    pp = &g_hydrservo;
	
	if(pp == NULL)
		return;
	
	if (CheckPIUsed(pp->pi->I_XX_CHK))
	{
		if(GetOperMode() == OM_MANUAL)
		{
			if(TestPI(pp->pi->I_XX_CHK) && (!CHK_PART_ERR(pp, ER5_POWERERR)))
			{
				SET_PART_ERR(pp, ER5_POWERERR);
				StandAlarm(TRUE);
			}
			else if((TestPI(pp->pi->I_XX_CHK) == 0) && (CHK_PART_ERR(pp, ER5_POWERERR)))
			{
				CLR_PART_ERR(pp, ER5_POWERERR);
			}
		}
		else if(CHK_PART_ERR(pp, ER5_POWERERR))
		{
			CLR_PART_ERR(pp, ER5_POWERERR);
		}
	}
	else
	{
		CLR_PART_ERR(pp, ER5_POWERERR);
	}
}


/***********************************************************************************/
//                         外控压力不足 
//当电机启动完成的时候即6秒后，检测该点是不是有信号，没有信号就报警“外控压力不足”，停电机，
//有信号则不报警；如果做动作或打自动，这个信号连续0.5秒没信号，警报并切电机   
/*************************************************************************************/
void HMD_PressErr()
{
	PSERVO	  pp = &g_hydrservo;

	if(pp == NULL)
		return;
	
	if (CheckPIUsed(pp->pi->I_PR_ERR))
	{
		if (MotorIsOn2())
		{
			if(TestPI(pp->pi->I_PR_ERR) == 0)
			{
				if(WKPOint == 0)
				{
					WKPress = A_100msclock;
					WKPOint = 1;
				}
				else
				{
					if((WORD)(A_100msclock - WKPress)>5) 
					{
						SET_PART_ERR(pp, ER5_PRESSERR);
						StandAlarm(TRUE);
						WKPOint = 0;
					}
				}
			}
		}
		else if ((TestPI(pp->pi->I_PR_ERR)) && (CHK_PART_ERR(pp, ER5_PRESSERR)))
		{
			CLR_PART_ERR(pp, ER5_PRESSERR);
		}
	}
	else
	{
		CLR_PART_ERR(pp, ER5_PRESSERR);
	}
}

void Badproduct()
{
	PCLAMP  pp = (PCLAMP)GetPartByID(MK_PART(CLAMP_ID, 1));
	PMACHINE   pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1)); 
	
	if(pp == NULL || pm == NULL)
		return;

	if(ChkOperMode(OM_AUTO))
	{
		pm->Filter_PRU_cnt = ((UI16)((((UI32)(pp->st->CN_MOLDOPNNUM0) << 16) + pp->st->CN_MOLDOPNNUM1) - curetshot));

		if (pp->md->FL_USE_PRU == 1)
		{
			if((pm->Filter_PRU_cnt < pp->md->FL_FISR_PRU) && (pp->md->FL_FISR_PRU != 0))
			{
				pp->st->CN_PRU = pm->Filter_PRU_cnt;
				if(!TestPOIsOn(pp->po->O_ERR_PRU))
					SetPOBit(pp->po->O_ERR_PRU,PC_ON);

				SET_PART_ERR(pp, ER0_ERRPRU);
			}
			else if(AlarmIsOn() && (pp->md->FL_AUTO_PRU == 1))
			{
				if(!TestPOIsOn(pp->po->O_ERR_PRU))
					SetPOBit(pp->po->O_ERR_PRU,PC_ON);

				SET_PART_ERR(pp, ER0_ERRPRU);
			}
			else if((CheckPIUsed(pp->pi->I_ERR_PRU)==1) && (TestPI(pp->pi->I_ERR_PRU)==1))
			{
				if(!TestPOIsOn(pp->po->O_ERR_PRU))
					SetPOBit(pp->po->O_ERR_PRU,PC_ON);

				SET_PART_ERR(pp, ER0_ERRPRU);
			}
			else
			{
				if(TestPOIsOn(pp->po->O_ERR_PRU))
					SetPOBit(pp->po->O_ERR_PRU,PC_OFF);

				CLR_PART_ERR(pp, ER0_ERRPRU);
			}
		}
		else
		{
			if(TestPOIsOn(pp->po->O_ERR_PRU))
				SetPOBit(pp->po->O_ERR_PRU,PC_OFF);

			CLR_PART_ERR(pp, ER0_ERRPRU);
		}
	}
	else
	{
		pp->st->CN_PRU = 0;
		if(pp->md->FL_MAUL_PRU == 1 && pp->md->FL_USE_PRU == 1)
		{
			if(!TestPOIsOn(pp->po->O_ERR_PRU))
				SetPOBit(pp->po->O_ERR_PRU,PC_ON);
		}
		else
		{
			if(TestPOIsOn(pp->po->O_ERR_PRU))
				SetPOBit(pp->po->O_ERR_PRU,PC_OFF);
		}
		curetshot = ((UI32)(pp->st->CN_MOLDOPNNUM0) << 16) + pp->st->CN_MOLDOPNNUM1;
	}
}

void ChargeClpLinkPo()
{
	PINJECT pp = (PINJECT)GetPartByID(MK_PART(INJECT_ID, 1));

	if (g_pump_num > 1)
	{
		
		if (ActIsInTsk(MK_ACT(INJECT_ID,1, OS_INJE)) || ActIsInTsk(MK_ACT(INJECT_ID,1, OS_AINJE)) || ActIsInTsk(MK_ACT(INJECT_ID,1, OS_NZLINJ)))
		{
			if (TestPOIsOn(pp->po->O_SYN_CHG))
			{
				SetPOBit(pp->po->O_SYN_CHG ,PC_OFF);
			}
		}
		else
		{
			if (ActTskIsEmpty() || (g_pumpn[0]->McPara.McSet[1] == 0 && g_pumpn[1]->McPara.McSet[1] == 0))
			{
				if (TestPOIsOn(pp->po->O_SYN_CHG))
				{
					SetPOBitDelay(pp->po->O_SYN_CHG ,PC_OFF,5);
				}
			}
			else
			{
				if (g_pumpn[1]->McPara.McSet[1] > 0)
				{
					if (!TestPOIsOn(pp->po->O_SYN_CHG))
					{
						SetPOBit(pp->po->O_SYN_CHG ,PC_ON);
					}
				}						
			}
		}
	}
}


/*=============================0102部件特殊阀start===============================*/
void ClampclsSpcVlvProc()			//关模特殊阀输出处理
{
	PCLAMP pclmp = (PCLAMP)GetPartByID(MK_PART(CLAMP_ID,1));
	PACT pact1 = pclmp->act + OS_CLSM-1;
	PACT pact2 = pclmp->act + OS_PRSM-1;
	if(pclmp == NULL || pact1 == NULL || pact2 == NULL)
		return;

	if(pclmp->mh->FL_SPC_CLS == 0)
		return;

	switch(pclmp->mh->FL_SPC_CLS)
	{
		case 0:		// 不使用
			if(TestPOIsOn(pclmp->po->O_SPC_CLS))
				SetPOBit(pclmp->po->O_SPC_CLS,PC_OFF);
			break;
		
		case 1:		//关模四段输出
			if(ChkActRun(pclmp,ACT_SN_BIT(OS_CLSM)) && pact1->step >= CLAMPCLOSE4 && pact1->step < STAGE_SPEC1_STEP)
			{
				if(!TestPOIsOn(pclmp->po->O_SPC_CLS))
					SetPOBit(pclmp->po->O_SPC_CLS,PC_ON);
			}
			else
			{
				if(TestPOIsOn(pclmp->po->O_SPC_CLS))
					SetPOBit(pclmp->po->O_SPC_CLS,PC_OFF);
			}
			break;

		case 2:	//关模一二三段输出
			if(ChkActRun(pclmp,ACT_SN_BIT(OS_CLSM)) && pact1->step >= CLAMPCLOSE1 && pact1->step < CLAMPCLOSE4 && (MIN_STEP(pact1->step) > 0))
			{
				if(!TestPOIsOn(pclmp->po->O_SPC_CLS))
					SetPOBit(pclmp->po->O_SPC_CLS,PC_ON);
			}
			else
			{
				if(TestPOIsOn(pclmp->po->O_SPC_CLS))
					SetPOBit(pclmp->po->O_SPC_CLS,PC_OFF);
			}
			break;
		
		case 3:	//关模二三段输出
			if(ChkActRun(pclmp,ACT_SN_BIT(OS_CLSM)) && pact1->step >= CLAMPCLOSE2 && pact1->step < CLAMPCLOSE4 && (MIN_STEP(pact1->step) > 0))
			{
				if(!TestPOIsOn(pclmp->po->O_SPC_CLS))
					SetPOBit(pclmp->po->O_SPC_CLS,PC_ON);
			}
			else
			{
				if(TestPOIsOn(pclmp->po->O_SPC_CLS))
					SetPOBit(pclmp->po->O_SPC_CLS,PC_OFF);
			}
			break;
		
		case 4:	//关模五段输出
			if(ChkActRun(pclmp,ACT_SN_BIT(OS_PRSM)))
			{
				if(!TestPOIsOn(pclmp->po->O_SPC_CLS))
					SetPOBit(pclmp->po->O_SPC_CLS,PC_ON);
			}
			else
			{
				if(TestPOIsOn(pclmp->po->O_SPC_CLS))
					SetPOBit(pclmp->po->O_SPC_CLS,PC_OFF);
			}
			break;
	
		case 5:	//关模四五段输出
			if(ChkActRun(pclmp,ACT_SN_BIT(OS_PRSM)) || (ChkActRun(pclmp,ACT_SN_BIT(OS_CLSM)) && pact1->step >= CLAMPCLOSE4 && pact1->step < STAGE_SPEC1_STEP))
			{
				if(!TestPOIsOn(pclmp->po->O_SPC_CLS))
					SetPOBit(pclmp->po->O_SPC_CLS,PC_ON);
			}
			else
			{
				if(TestPOIsOn(pclmp->po->O_SPC_CLS))
					SetPOBit(pclmp->po->O_SPC_CLS,PC_OFF);
			}
			break;
	
		case 6:	//关模全段输出
			if(ChkActRun(pclmp,ACT_SN_BIT(OS_PRSM)) || ChkActRun(pclmp,ACT_SN_BIT(OS_CLSM)))
			{
				if(!TestPOIsOn(pclmp->po->O_SPC_CLS))
					SetPOBit(pclmp->po->O_SPC_CLS,PC_ON);
			}
			else
			{
				if(TestPOIsOn(pclmp->po->O_SPC_CLS))
					SetPOBit(pclmp->po->O_SPC_CLS,PC_OFF);
			}
			break;
	
		case 7:		//开关模全段输出
			if(ChkActRun(pclmp,ACT_SN_BIT(OS_CLSM)) || ChkActRun(pclmp,ACT_SN_BIT(OS_PRSM)) || ChkActRun(pclmp,ACT_SN_BIT(OS_OPNM)))
			{
				if(!TestPOIsOn(pclmp->po->O_SPC_CLS))
					SetPOBit(pclmp->po->O_SPC_CLS,PC_ON);
			}
			else
			{
				if(TestPOIsOn(pclmp->po->O_SPC_CLS))
					SetPOBit(pclmp->po->O_SPC_CLS,PC_OFF);
			}
			break;
		
		default:
			if(TestPOIsOn(pclmp->po->O_SPC_CLS))
				SetPOBit(pclmp->po->O_SPC_CLS,PC_OFF);
			break;
	}
}

void ClampopnSpcVlvProc()			//开模特殊阀处理函数
{
	PCLAMP pclmp = (PCLAMP)GetPartByID(MK_PART(CLAMP_ID,1));
	PACT pact1 = pclmp->act + OS_OPNM-1;
	if(pclmp == NULL || pact1 == NULL)
		return;

	if(pclmp->mh->FL_SPC_OPN == 0)
		return;

	switch(pclmp->mh->FL_SPC_OPN)
	{
		case 0:		// 不使用
			if(TestPOIsOn(pclmp->po->O_SPC_OPN))
				SetPOBit(pclmp->po->O_SPC_OPN,PC_OFF);
			break;
		
		case 1:		//开模一段
			if(ChkActRun(pclmp,ACT_SN_BIT(OS_OPNM)) && pact1->step >= CLAMPOPENSLOW1 && pact1->step < CLAMPOPENFAST && (MIN_STEP(pact1->step) > 0))
			{
				if(!TestPOIsOn(pclmp->po->O_SPC_OPN))
					SetPOBit(pclmp->po->O_SPC_OPN,PC_ON);
			}
			else
			{
				if(TestPOIsOn(pclmp->po->O_SPC_OPN))
					SetPOBit(pclmp->po->O_SPC_OPN,PC_OFF);
			}
			break;

		case 2:	//开模一五段输出
			if(ChkActRun(pclmp,ACT_SN_BIT(OS_OPNM)) && ((pact1->step >= CLAMPOPENSLOW1 && pact1->step < CLAMPOPENFAST) || (pact1->step >= CLAMPOPENEND&& pact1->step < STAGE_SPEC1_STEP)) && (MIN_STEP(pact1->step) > 0))
			{
				if(!TestPOIsOn(pclmp->po->O_SPC_OPN))
					SetPOBit(pclmp->po->O_SPC_OPN,PC_ON);
			}
			else
			{
				if(TestPOIsOn(pclmp->po->O_SPC_OPN))
					SetPOBit(pclmp->po->O_SPC_OPN,PC_OFF);
			}
			break;
		
		case 3:	//开模五段输出
			if(ChkActRun(pclmp,ACT_SN_BIT(OS_OPNM)) && pact1->step >= CLAMPOPENEND&& pact1->step < STAGE_SPEC1_STEP)
			{
				if(!TestPOIsOn(pclmp->po->O_SPC_OPN))
					SetPOBit(pclmp->po->O_SPC_OPN,PC_ON);
			}
			else
			{
				if(TestPOIsOn(pclmp->po->O_SPC_OPN))
					SetPOBit(pclmp->po->O_SPC_OPN,PC_OFF);
			}
			break;
		
		case 4:	//开模二三四段输出
			if(ChkActRun(pclmp,ACT_SN_BIT(OS_OPNM)) && pact1->step >= CLAMPOPENFAST && pact1->step < CLAMPOPENEND)
			{
				if(!TestPOIsOn(pclmp->po->O_SPC_OPN))
					SetPOBit(pclmp->po->O_SPC_OPN,PC_ON);
			}
			else
			{
				if(TestPOIsOn(pclmp->po->O_SPC_OPN))
					SetPOBit(pclmp->po->O_SPC_OPN,PC_OFF);
			}
			break;
	
		case 5:	//开模二三四五段输出
			if(ChkActRun(pclmp,ACT_SN_BIT(OS_OPNM)) && pact1->step >= CLAMPOPENFAST && pact1->step < STAGE_SPEC1_STEP)
			{
				if(!TestPOIsOn(pclmp->po->O_SPC_OPN))
					SetPOBit(pclmp->po->O_SPC_OPN,PC_ON);
			}
			else
			{
				if(TestPOIsOn(pclmp->po->O_SPC_OPN))
					SetPOBit(pclmp->po->O_SPC_OPN,PC_OFF);
			}
			break;
	
		case 6:	//开模全段输出
			if(ChkActRun(pclmp,ACT_SN_BIT(OS_OPNM)))
			{
				if(!TestPOIsOn(pclmp->po->O_SPC_OPN))
					SetPOBit(pclmp->po->O_SPC_OPN,PC_ON);
			}
			else
			{
				if(TestPOIsOn(pclmp->po->O_SPC_OPN))
					SetPOBit(pclmp->po->O_SPC_OPN,PC_OFF);
			}
			break;
	
		case 7:		//开关模全段输出
			if(ChkActRun(pclmp,ACT_SN_BIT(OS_CLSM)) || ChkActRun(pclmp,ACT_SN_BIT(OS_PRSM)) || ChkActRun(pclmp,ACT_SN_BIT(OS_OPNM)))
			{
				if(!TestPOIsOn(pclmp->po->O_SPC_OPN))
					SetPOBit(pclmp->po->O_SPC_OPN,PC_ON);
			}
			else
			{
				if(TestPOIsOn(pclmp->po->O_SPC_OPN))
					SetPOBit(pclmp->po->O_SPC_OPN,PC_OFF);
			}
			break;
		
		default:
			if(TestPOIsOn(pclmp->po->O_SPC_OPN))
				SetPOBit(pclmp->po->O_SPC_OPN,PC_OFF);
			break;
	}
}

void InjSpcVlvProc()			//射出特殊阀处理函数
{
	PINJECT pinj = (PINJECT)GetPartByID(MK_PART(INJECT_ID,1));
	PACT pact1 = pinj->act + OS_INJE-1;
	if(pinj == NULL || pact1 == NULL)
		return;

	if(pinj->mh->FL_SPC_INJ == 0)
		return;

	switch(pinj->mh->FL_SPC_INJ)
	{
		case 0:		// 不使用
			if(TestPOIsOn(pinj->po->O_SPC_INJ))
				SetPOBit(pinj->po->O_SPC_INJ,PC_OFF);
			break;
		
		case 1:		//射出保压
			if(ChkActRun(pinj,ACT_SN_BIT(OS_INJE)))
			{
				if(!TestPOIsOn(pinj->po->O_SPC_INJ))
					SetPOBit(pinj->po->O_SPC_INJ,PC_ON);
			}
			else
			{
				if(TestPOIsOn(pinj->po->O_SPC_INJ))
					SetPOBit(pinj->po->O_SPC_INJ,PC_OFF);
			}
			break;

		case 2:	//射出
			if(ChkActRun(pinj,ACT_SN_BIT(OS_INJE)) && ((pact1->step >= INJCT1&& pact1->step < INJCTEND)))
			{
				if(!TestPOIsOn(pinj->po->O_SPC_INJ))
					SetPOBit(pinj->po->O_SPC_INJ,PC_ON);
			}
			else
			{
				if(TestPOIsOn(pinj->po->O_SPC_INJ))
					SetPOBit(pinj->po->O_SPC_INJ,PC_OFF);
			}
			break;
		
		case 3:	//射出保压射退
			if(ChkActRun(pinj,ACT_SN_BIT(OS_INJE)) || ChkActRun(pinj,ACT_SN_BIT(OS_SUCKCHG)) || ChkActRun(pinj,ACT_SN_BIT(OS_SUCK)))
			{
				if(!TestPOIsOn(pinj->po->O_SPC_INJ))
					SetPOBit(pinj->po->O_SPC_INJ,PC_ON);
			}
			else
			{
				if(TestPOIsOn(pinj->po->O_SPC_INJ))
					SetPOBit(pinj->po->O_SPC_INJ,PC_OFF);
			}
			break;
		
		case 4:	//射出保压储料
			if(ChkActRun(pinj,ACT_SN_BIT(OS_INJE)) || ChkActRun(pinj,ACT_SN_BIT(OS_CHRG)))
			{
				if(!TestPOIsOn(pinj->po->O_SPC_INJ))
					SetPOBit(pinj->po->O_SPC_INJ,PC_ON);
			}
			else
			{
				if(TestPOIsOn(pinj->po->O_SPC_INJ))
					SetPOBit(pinj->po->O_SPC_INJ,PC_OFF);
			}
			break;
	
		case 5:	//射出保压储料射退
			if(ChkActRun(pinj,ACT_SN_BIT(OS_INJE)) || ChkActRun(pinj,ACT_SN_BIT(OS_SUCKCHG)) || ChkActRun(pinj,ACT_SN_BIT(OS_CHRG)) || ChkActRun(pinj,ACT_SN_BIT(OS_SUCK)))
			{
				if(!TestPOIsOn(pinj->po->O_SPC_INJ))
					SetPOBit(pinj->po->O_SPC_INJ,PC_ON);
			}
			else
			{
				if(TestPOIsOn(pinj->po->O_SPC_INJ))
					SetPOBit(pinj->po->O_SPC_INJ,PC_OFF);
			}
			break;
		
		default:
			if(TestPOIsOn(pinj->po->O_SPC_INJ))
				SetPOBit(pinj->po->O_SPC_INJ,PC_OFF);
			break;
	}
}

void ChgSpcVlvProc()			//储料特殊阀处理函数
{
	PINJECT pinj = (PINJECT)GetPartByID(MK_PART(INJECT_ID,1));
	PACT pact1 = pinj->act + OS_INJE-1;
	if(pinj == NULL || pact1 == NULL)
		return;

	if(pinj->mh->FL_SPC_CHG == 0)
		return;

	switch(pinj->mh->FL_SPC_CHG)
	{
		case 0:		// 不使用
			if(TestPOIsOn(pinj->po->O_SPC_CHG))
				SetPOBit(pinj->po->O_SPC_CHG,PC_OFF);
			break;
		
		case 1:		//储料
			if(ChkActRun(pinj,ACT_SN_BIT(OS_CHRG)))
			{
				if(!TestPOIsOn(pinj->po->O_SPC_CHG))
					SetPOBit(pinj->po->O_SPC_CHG,PC_ON);
			}
			else
			{
				if(TestPOIsOn(pinj->po->O_SPC_CHG))
					SetPOBit(pinj->po->O_SPC_CHG,PC_OFF);
			}
			break;

		case 2:	//储料射退
			if(ChkActRun(pinj,ACT_SN_BIT(OS_CHRG)) || ChkActRun(pinj,ACT_SN_BIT(OS_SUCK)) || ChkActRun(pinj,ACT_SN_BIT(OS_SUCKCHG)))
			{
				if(!TestPOIsOn(pinj->po->O_SPC_CHG))
					SetPOBit(pinj->po->O_SPC_CHG,PC_ON);
			}
			else
			{
				if(TestPOIsOn(pinj->po->O_SPC_CHG))
					SetPOBit(pinj->po->O_SPC_CHG,PC_OFF);
			}
			break;
		
		case 3:	//射出保压储料
			if(ChkActRun(pinj,ACT_SN_BIT(OS_INJE)) || ChkActRun(pinj,ACT_SN_BIT(OS_CHRG)))
			{
				if(!TestPOIsOn(pinj->po->O_SPC_CHG))
					SetPOBit(pinj->po->O_SPC_CHG,PC_ON);
			}
			else
			{
				if(TestPOIsOn(pinj->po->O_SPC_CHG))
					SetPOBit(pinj->po->O_SPC_CHG,PC_OFF);
			}
			break;
	
		case 4:	//射出保压储料射退
			if(ChkActRun(pinj,ACT_SN_BIT(OS_INJE)) || ChkActRun(pinj,ACT_SN_BIT(OS_SUCKCHG)) || ChkActRun(pinj,ACT_SN_BIT(OS_CHRG)) || ChkActRun(pinj,ACT_SN_BIT(OS_SUCK)))
			{
				if(!TestPOIsOn(pinj->po->O_SPC_CHG))
					SetPOBit(pinj->po->O_SPC_CHG,PC_ON);
			}
			else
			{
				if(TestPOIsOn(pinj->po->O_SPC_CHG))
					SetPOBit(pinj->po->O_SPC_CHG,PC_OFF);
			}
			break;
		
		default:
			if(TestPOIsOn(pinj->po->O_SPC_CHG))
				SetPOBit(pinj->po->O_SPC_CHG,PC_OFF);
			break;
	}
}

/*=============================0102部件特殊阀end=================================*/



void SpecialFunc()           //特殊功能程序
{
	if(ComIsOk())
	{
		if(CUS_ID==CS_NBDAMEI)
		{
			LubIOCheck();
		}
		
		TY_VolCheck();
		HMD_PressErr();     //外控压力不足
		CoolTMCount();	

		if(CUS_ID == CS_TONGYONG)
		{
			Badproduct();           //不良品检测功能
		}

		if(CUS_ID == CS_WEILISHI)
		{
			OpenRelVlvProc();		//真空发生器功能
			CheckOilLeak();			//威力仕漏油检测
		}

        InitCoreReach(); //中子状态初始化
        CoreRleaseLoop();   //中子泄压调度任务
	}
}

PINNER_PARA LoadInnerPara()
{
	const PART_CFG_IND* pcfg;
	UI16 offset;
	UI16 len;
	
	offset = GET_IND(PMACHINE_VERSION, saved_pa);
	pcfg = CfgDbPartRead((PPART)(&m_machine), VR_ID,  CAL_LEN(INNER_PARA), offset, FACTORY_PART_DB);
	if(pcfg == NULL || pcfg->offset > offset)
	{
		memset(&inner_pa, 0, sizeof(inner_pa));
	}
	else
	{
		len = pcfg->len;
		if(len > offset - pcfg->offset)
		{
			len = (pcfg->len - (offset - pcfg->offset))*2;
			if(len > sizeof(inner_pa))
			{
				len = sizeof(inner_pa);
			}
			
			memcpy(&inner_pa, ((UI16*)pcfg->psave) + (offset - pcfg->offset), len); 
		}
	}

	return &inner_pa;
}
	
void SaveInnerPara()
{
	UI16 offset;  
	
	MemcpyPro(&(m_machine.vr->saved_pa), &inner_pa, sizeof(inner_pa));
	offset = GET_IND(PMACHINE_VERSION, saved_pa);
	CfgDbPartSave((PPART)(&m_machine), VR_ID, CAL_LEN(INNER_PARA), offset, FACTORY_PART_DB);
}

PINNER_PARA GetInnerPara()
{
	return &inner_pa;
}

extern DA m_da;

void PollStepFunc()
{
	static WORD PollStep = 0;
	ChkAdposiInplace();
	Robot_Turn();	//机械手点跳变检测
	ChargeClpLinkPo();//储料联动阀输出

	//特殊阀输入输出的处理
	ClampclsSpcVlvProc();
	ClampopnSpcVlvProc();
	InjSpcVlvProc();
	ChgSpcVlvProc();
	AdMeterSpeedLoopPro();
	/*==================*/
	CoreStatusLoop();  //中子状态检测任务
    ServoErr();
    ChkEmgExtPi();  //外部急停按钮检测

	switch (PollStep)
	{
		case 0:
		case 1:
			if(ChkRunFlag(INIF_USEECAT))
			{
				if (ECATStateCheck())
	            {
	                StandAlarm(FALSE);
	            }
			}
			Read_surtemp(); 		//读主板上的室温
			Read_Curtemp();			//读E502上的室温和E502的版本
			break;
		case 2:
			FreeServoDatCache();				
			if (iser_mem == 1)
			{				
				SetMachineErr(ER0_MEM);
			}

			if (g_sram_err == 1)
			{
				SetMachineErr(ER1_SRAMERR);
			}
			CheckAdjUsed();
			MotorServoSateChk();
			break;
		case 3:
			if(!ChkOperMode(OM_AUTO))
			{
				MovePartDataChangeCache();	//将缓存内的数据搬运到设定dic中生效。
			}
            
            if (GetSys()->pinj->md->FL_INJECTWARMPRE == 1)
            {
                SET_PART_ERR(GetSys()->pinj,ER1_NOTMPCHK);
            }

            if(ChkSysCfg(CO9_HIGH))
                LoadZeroDataTemp();
			break;
		case 4:			
			CheckIoForce();
			MoterOverLoadChk();	//马达过载检测
			if ((CUS_ID == CS_HAIXIN)&&(!ChkOperMode(OM_AUTO)))
			{
				OilLvErr();
				MotorTemperErr();
			}
			MotorFreeTimeChk();			
			WarmFreeTimeChk();
			break;
		case 5:
			SpecialFunc();
			ModbuDownloadPro();
			break;
		case 6:
			TempMwcheck();		//该函数主要放一些关于模温的，要时时运行的函数
			IOForceTimeOut(); //IO强制输出超时保护
			break;
		default:
			PollStep = 0;
			break;
	}
	PollStep++;
}

