/******************************************************************************
  文 件 名   : IOproc.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : IO部件及功能函数
  函数列表   :
              CheckPI
              CheckPIUsed
              ClearAllPOOut
              ClearPOOut
              GetModelIOMax
              GetPI32
              GetPICnt
              GetPICycleMs
              GetPO32
              GetPOBit
              InitIOPart
              LoopPI
              LoopPOOnOff
              OutPO
              PICntLoop
              PIFilt
              POForceEnd
              POForceOutPut
              ResetPICnt
              SetModelIOMax
              SetPICfg
              SetPO32
              SetPOBit
              SetPOBitDelay
              SetPOOnOff
              TestPI
              TestPOIsOn
  修改历史   :
  1.日    期   : 2013年5月17日
    作    者   : ren chaohong
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
	 
#include "com.h"
#include "ioproc.h"
#include "machapi.h"
#include "canhead.h"
#include "servo.h"
#include "motor.h"
#include "robot.h"
#include "Useriap.h"
#include "hwcfg.h"
#include "memtbl.h"
#include "cpld.h"
#include "hwcfg.h"
#include "hwmid.h"

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
extern MACHINE m_machine;
extern IOVTEST 	f_iovtest;					//IO报警

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
 void m_io_check_hv(void);
void SendIOForceExitToPanel(void);
BOOL TempIOForceErr(void);


/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
static IO  m_io;

volatile UI16* po_out0 = &(port4000);
volatile UI16* po_out1 = &(port4001);
volatile UI16* po_out2 = &(port4002);
volatile UI16* po_out3 = &(port4003);
volatile UI16* pi_in0 = &(port3000);
volatile UI16* pi_in1 = &(port3001);
volatile UI16* pi_in2 = &(port3002);



/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define IS_INN_EXT_GAP(name)		(pp->pi_inn_status_msk != 0 && (name) > pp->pi_inn_num && (name) <= (pp->pi_inn_status_num << 4))
#define IS_PO_INN_EXT_GAP(name)		(pp->po_inn_out_msk != 0 && (name) > pp->po_inn_num && (name) <= (pp->po_inn_out_num << 4))

void IOForceTimeOut()  //IO强制输出超时保护，避免客户长时间强制输出电热导致烧料筒 2025-2-24 ccz
{
    static UI16 io_force_tm = 0;
    PIO pp = &m_io;
    BOOL exit_flag = FALSE;
    
    if(ChkRunFlag(INIF_CHKPC))
    {
        if(GetSys()->md->TM_IOFORCE_OUT < 1 || GetSys()->md->TM_IOFORCE_OUT > 15)
            GetSys()->md->TM_IOFORCE_OUT = 10;
        
        if(DIFF_SEC(io_force_tm) >= 60*GetSys()->md->TM_IOFORCE_OUT)
        {
            SET_PART_ERR(pp, ER0_FORCE_TM_OUT);
            exit_flag = TRUE;
        }

        if(TempIOForceErr())
        {
            SET_PART_ERR(pp, ER0_FORCE_TEMP);
            exit_flag = TRUE;
        }

        if(exit_flag)
        {
            POForceEnd();
            SendIOForceExitToPanel(); //通知面板清除IO强制输出表，否则再次进入强制输出状态时会输出清除前的所有点
        }
    }
    else
        io_force_tm = GetSecTick();
}

WORD GetPoChainNextPo(WORD cur_name)
{
	return (0x7FFF & m_io.po_chain[cur_name -1]);
}


WORD GetMaxPo()
{
	return m_io.po_max;
}

/*******************************************************************************************
  * @函数名称	outpc
  * @函数说明   IO输出及继电器输出驱动
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
 ****************************************************************************************/
void IOVErr()
{
 	PIO pp = &m_io;

	m_io_check_hv();

    if(f_iovtest.hvlowflag == 1)
	{
	    SET_PART_ERR(pp, ER0_H24V);
	}
	else
	{
	    CLR_PART_ERR(pp, ER0_H24V);
	}

    if(f_iovtest.iooverflag == 1)
	{		
	    SET_PART_ERR(pp, ER0_IOV);
	}
	else
	{
	    CLR_PART_ERR(pp, ER0_IOV);
	}

	if(ChkSysCfg(XF6_EXTIO))		//第一块IO板失压和电流过载检测
	{
		if(pp->IO_LossPres[1] == 1)
			SET_PART_ERR(pp, ER0_H24_1);
		else
			CLR_PART_ERR(pp, ER0_H24_1);

		if(pp->IO_Overcurrent[1] == 1)
			SET_PART_ERR(pp, ER0_IOV_1);
		else
			CLR_PART_ERR(pp, ER0_IOV_1);
	}

	if(ChkSysCfg(XF6_EXTIO2))		//第二块IO板失压和电流过载检测
	{
		if(pp->IO_LossPres[2] == 1)
			SET_PART_ERR(pp, ER0_H24_2);
		else
			CLR_PART_ERR(pp, ER0_H24_2);

		if(pp->IO_Overcurrent[2] == 1)
			SET_PART_ERR(pp, ER0_IOV_2);
		else
			CLR_PART_ERR(pp, ER0_IOV_2);
	}
}
/*******************************************************************************************
  * @函数名称	outpc
  * @函数说明   IO输出及继电器输出驱动
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
 ****************************************************************************************/
void OutPO()
{
 	PIO pp = &m_io;
	int i;
	UI16 IO_time;
	UI16* stbuf[] = {&HW_PO_OUT_0,&HW_PO_OUT_1,&HW_PO_OUT_2,&HW_PO_OUT_3};

	if(pp->po_inn_out_msk > 0)
	{
		*stbuf[pp->po_inn_out_num - 1] = (pp->st->po_output[pp->po_inn_out_num - 1] & pp->po_inn_out_msk);
	}
	else
	{
		*stbuf[pp->po_inn_out_num - 1] = pp->st->po_output[pp->po_inn_out_num - 1];
	}

	for(i = pp->po_inn_out_num - 1 ; i > 0; i--)
	{
		CPLD_WAIT();
		*stbuf[i - 1] = pp->st->po_output[i - 1];
	}

    /*if((pp->po_ext_out_num > 0 && (Get100UsTick() - pp->last_ext_po_100us > 100))||ChkRunFlag(INIF_CHKPC))
    {
		pp->last_ext_po_100us = Get100UsTick();
		for(i = 0; i < pp->po_ext_out_num; i++)
		{
			CanExtIOWrite(i + 1, pp->st->po_output + pp->po_inn_out_num + i);
		}
    }*/

	if(pp->po_ext_out_num > 0)
		IO_time = 100/pp->po_ext_out_num;
	else
		IO_time = 100;
	if((pp->po_ext_out_num > 0 && (Get100UsTick() - pp->last_ext_po_100us > IO_time))||ChkRunFlag(INIF_CHKPC))
    {
		pp->last_ext_po_100us = Get100UsTick();

		if(ChkRunFlag(INIF_CHKPC))	//强制时，直接输出，不采用错开的方式
		{
			for(i = 0; i < pp->po_ext_out_num; i++)
			{
				CanExtIOWrite(i + 1, pp->st->po_output + pp->po_inn_out_num + i);
			}
		}
		else
		{
			CanExtIOWrite(pp->Ionum + 1, pp->st->po_output + pp->po_inn_out_num + pp->Ionum);
			pp->Ionum++;
			if(pp->Ionum >= pp->po_ext_out_num)
				pp->Ionum = 0; 
		}
    }
}

static __forceinline void SetPOBitInner(WORD  name,WORD postat)
{
	PIO	pio = &m_io;
	UI16 val;
	UI16 now_100us;
	UI16 delay;
	UI16 old_delay;
	
	name--;
	val = pio->st->PO_STAT[name ];
	delay = (postat >> 1);

	if(delay > MAX_PO_DELAY/100)
		delay = MAX_PO_DELAY/100;

	/*转换成100us单位*/
	delay = delay*100;
	now_100us = (UI16)Get100UsTick();
	old_delay = (UI16)(pio->po_set100us[name] -now_100us);
	if((val & 0x01) != (postat &0x01) ||(val > 1 && old_delay < MAX_PO_DELAY && delay < old_delay))
	{
		if(delay < pio->po_vlvdelay[name] && pio->po_vlvdelay[name] > PO_SAMPL_TM)
		{
			delay = pio->po_vlvdelay[name];

			if(postat < 2)
				postat |= 0x0002;
		}
		
		pio->st->PO_STAT[name] = postat;
		pio->po_set100us[name] = (UI16)(now_100us + delay);

        if(ChkRunFlag(INIF_CHKPC))
		    return;
        
		/*如果是没有延迟的立即开或关，直接对output置位，防止一个运行周期内，
		多次对同一个输出点设置时，立即开关被延迟开关覆盖*/
		if(postat < 2)
			pio->st->po_output[name>>4] |= (1 << (name & 0x0f));
	}
}

static __forceinline void SetPOOnOffInner(WORD  name,WORD pcstat)
{
 	PIO pio = &m_io;

	SetPOBitInner(name, pcstat);
	name--;
		
	/*output now*/
	if(pcstat < 2 && name < pio->po_max)
	{
		if(pcstat)
			pio->st->po_output[name >> 4] |= (1 << (name &0x0f));		//相应位赋1
		else
			pio->st->po_output[name >> 4] &= (~(1 << (name &0x0f)));	//相应位赋0

		pio->st->PO_STAT[name] = pcstat;
		pio->po_vlvdelay[name] = VLV_MIN_RSP_TM;
		OutPO();
	}
}

void ChkSetPOBit(WORD  name,WORD postat)
{
	if (!TestPOIsOn(name))
	{
		SetPOBit(name,postat);
	}
}

/*增加输出点链接处理，用于一个逻辑点关联多个输出点*/
void SetPOBit(WORD  name,WORD postat)
{
	WORD tmp_name;
	WORD spc_link = 0;
	WORD i = 0;

	tmp_name = name;

	while(tmp_name > 0 && tmp_name <= MAX_PO_NUM)
	{
		if(tmp_name <= m_io.po_max)
			SetPOBitInner(tmp_name, postat);

		if (!spc_link)
		{
			//输出链接异常，直接结束链表功能
			tmp_name = GetPoChainNextPo(tmp_name);
			if (tmp_name == 0)
			{
				spc_link = 1;
			}
		}
		
		if (spc_link)
		{
			if (i < MAX_PO_LINK_NUM)
			{
				tmp_name = gs_polinktb[name - 1][i];
			}	
			else
			{
				tmp_name = 0;
			}
			i++;
		}
	}
}

/*增加输出点链接处理，用于一个逻辑点关联多个输出点*/
void SetPOOnOff(WORD  name,WORD pcstat)
{
	WORD tmp_name;
	WORD spc_link = 0;
	WORD i = 0;
	
	tmp_name = name;

	while(tmp_name > 0 && tmp_name <= MAX_PO_NUM)
	{
		if(tmp_name <= m_io.po_max)
			SetPOOnOffInner(tmp_name, pcstat);

		if (!spc_link)
		{
			//输出链接异常，直接结束链表功能
			tmp_name = GetPoChainNextPo(tmp_name);
			if (tmp_name == 0)
			{
				spc_link = 1;
			}
		}
		
		if (spc_link)
		{
			if (i < MAX_PO_LINK_NUM)
			{
				tmp_name = gs_polinktb[name - 1][i];
			}	
			else
			{
				tmp_name = 0;
			}
			i++;
		}
	}
}

BOOL TestPOIsOn(WORD  name)
{
 	PIO pio = &m_io;

	if(name <= 0 || name > pio->po_max)
		return FALSE;
		
	return(pio->st->PO_STAT[name - 1]& 0x01);
}

PO_STATUS CheckPO(WORD name)
{
    PIO	pp = &m_io;
    UI16 tmp;
//   	UI8 ind;

    if (name > 0 && name <= m_io.po_max)
    {
        /*检查是否位于内部输入点与外部输入点之间的无效输入点*/
        if (IS_PO_INN_EXT_GAP(name))
        {
            return PO_NOUSED;
        }
		name--;

		tmp = (pp->st->po_output[name/16]>>(name%16))&0x01;

        if (tmp == 0)
        {
            return PO_RESET;
        }
        else
        {
            return PO_SET;
        }
    }
    else
    {
        return PO_NOUSED;
    }
}


UI16 GetPOBit(WORD name)
{
 	PIO pio = &m_io;

	if(name <= 0 || name > pio->po_max)
		return FALSE;
		
	return pio->st->PO_STAT[name - 1];
}

void ClearPOOut()
{                  
	int	i;
	
 	PIO pio = &m_io;

	for(i = 0; i < PO_STATUS_NUM; i++)
		pio->st->po_output[i] = 0;

		/*
		保留以下下输出状态，其他清零
		O_HEATER  ,                 //73H  ;电热开       ; HEATER POWER CONTROL
	    O_COOLER  ,                 //74H  ;冷却水       ; COOLING WATER
	    O_CONVEYR ,                 //75H  ;输送带       ; CONVEYER
	    O_COOL1   ,                 //76H  ;冷却器 1     ;;BARREL COOLER #1
	    O_COOL2   ,                 //77H  ;冷却器 2     ;;BARREL COOLER #2
	    O_COOL3   ,                 //78H  ;冷却器 3     ;;BARREL COOLER #3
	    O_COOL4   ,                 //79H  ;冷却器 4     ;;BARREL COOLER #4
	    O_COOL5   ,                 //7AH  ;冷却器 5     ;;BARREL COOLER #5
	    O_COOL6   ,                 //7BH  ;冷却器 6     ;;BARREL COOLER #6,
	    O_COOL7   ,                 //7CH  ;冷却器 7     ;;BARREL COOLER #7,
	    O_COOL8   ,                 //7DH  ;冷却器 8     ;;BARREL COOLER #8,
	    O_COOL9   ,                 //7EH  ;冷却器 9     ;;BARREL COOLER #9,
	    O_OILHEAT ,                 //7FH  ;油温加热     ;;OIL HEAT  ;;(9823)7HT

	    O_MAN_LIT ,                 //95H  ;手动灯       ; MANUAL LITE  ;;(2722)7HT
	    O_RESERVE96,                //96H  OUTPUT RESERVED   */

	OutPO();
 }

void LoopPOOnOff()
{
	PIO	pp = &m_io;
	WORD tmp_val;
	WORD tmp_out, tmp_sav;
	int	i, offset;
	UI16 diff;
	UI16 chk_100us;
	int  max_po_num;
	max_po_num = m_io.po_max;
	if(max_po_num < 64 )
		max_po_num = 64;
	
	//extern UI32 test_start_ms;
	//if((UI16)((UI16)Get100UsTick() - pp->po_100us_tick) >= PO_SAMPL_TM)
	{
		chk_100us = pp->po_100us_tick;
		pp->po_100us_tick = (UI16)Get100UsTick();
		chk_100us = (UI16)(pp->po_100us_tick - chk_100us);

		tmp_out = pp->st->po_output[0];
		offset =0;
		//for(i = 0; i <MAX_PO_NUM; i++)
		for(i = 0; i <max_po_num; i++)
		{
			if(pp->po_vlvdelay[i] != 0)
			{
				if(pp->po_vlvdelay[i] >= chk_100us)
					pp->po_vlvdelay[i] -= chk_100us;
				else
					pp->po_vlvdelay[i] = 0;
			}
			
			tmp_val = pp->st->PO_STAT[i];
			if(tmp_val > 0x01)
			{
				diff = (UI16)(pp->po_set100us[i]- pp->po_100us_tick);
				if(diff == 0 || diff > MAX_PO_DELAY)
				{
					tmp_val = (tmp_val & 0x01);
					pp->st->PO_STAT[i] = tmp_val;
				}
			}
			
			if(i < pp->po_max)
			{
				offset = (i & 0x0f);
				if(tmp_val <= 0x01)
				{
					tmp_sav = tmp_out;
					if(tmp_val)
						tmp_out |= (1 <<offset);		//相应位赋1
					else
						tmp_out &= (~(1 << offset));	//相应位赋0

					if(tmp_sav != tmp_out)
					{
						pp->po_vlvdelay[i] = VLV_MIN_RSP_TM;
						//db_printf("PO %d, set at %d ms\r\n", i + i, GetMsTick() - test_start_ms);
					}
				}

				if(offset == 0x0f)
				{
					pp->st->po_output[i>>4] = tmp_out;
					tmp_out = pp->st->po_output[(i>>4) + 1];
				}
			}	
		}

		if(offset != 0x0f)
			pp->st->po_output[i>>4] = tmp_out;
 	}

	OutPO();
}

void PIFilt(UI32 st, UI32  ind)
{
	/*尽量使用临时变量，加快速度*/
	int i;
	PIO pp = &m_io;
	UI32 last_st;
	UI32 filt_st;
	UI32 bit;
	UI32 cnt;
	UI32 val;
	UI32 name;
	
	filt_st = pp->st->pi_input[ind];
	last_st =pp->pi_status[ind];
	if(st != last_st || filt_st != st)
	{
		bit = 1;
		name = (ind << 4);
		for(i = 0; i < 0x10; i++)
		{
			val = (st & bit);
			if((last_st & bit) != val)
			{
				if(pp->pi_core[name]) //中子信号单独滤波
				{
					if(val)
						cnt = 50/MAX_LOOP_100US; //10*500us=5ms 无信号变有信号滤波时间
					else
						cnt = 200/MAX_LOOP_100US; //40*500us=20ms 有信号变无信号滤波时间
					pp->pi_filt_cnt[name] = cnt;
				}
				else
				{
					cnt = (pp->pi_cfg[name] & PI_FILT_MSK);
					if(ChkSysCfg(CO43_RSV1))
						cnt = cnt * 10;
					pp->pi_filt_cnt[name] = cnt;
				}

				/*无过滤，有变化直接赋值*/
				if(cnt < 1 && val != (filt_st & bit))
				{
					if(val)
						filt_st |= bit;
					else
						filt_st &= (~bit);
				}	
			}
			else
			{
				if(val != (filt_st & bit))
				{
					cnt = pp->pi_filt_cnt[name];
					if(cnt < 1)
					{
						if(val)
							filt_st |= bit;
						else
							filt_st &= (~bit);
						cnt = (pp->pi_cfg[name] & PI_FILT_MSK);
						if(ChkSysCfg(CO43_RSV1))
							cnt = cnt * 10;
					}
					else
						cnt--;
					pp->pi_filt_cnt[name] = cnt;
				}
			}

			bit <<= 1;
			name++;
		}

		pp->st->pi_input[ind] = filt_st;
		pp->pi_status[ind] = st;
	}
}
 
BOOL LoopPI()
{
	int	i;
	PIO pp = &m_io;
	UI16 st;
	UI16 stbuf[] = {HW_PI_IN_0,HW_PI_IN_1,HW_PI_IN_2};

	if(pp->pi_inn_status_msk > 0)
	{
		st = (stbuf[pp->pi_inn_status_num - 1] & pp->pi_inn_status_msk);
	}
	else
	{
		st = stbuf[pp->pi_inn_status_num - 1];		
	}
	PIFilt(st, pp->pi_inn_status_num - 1);

	for(i=pp->pi_inn_status_num - 1 ;i>0;i--)
	{
		PIFilt(stbuf[i-1], i-1);
	}
		
	if(pp->pi_ext_num > 0)
	{
		for(i = 0; i < pp->pi_ext_status_num; i++)
		{
			CanExtIORead(i + 1, &st);			/*IO CAN ID 从1 开始*/
			PIFilt(st, pp->pi_inn_status_num + i);
		}
	}

	return TRUE;
}

BOOL CheckPIUsed(WORD name)
{
	PIO pp = &m_io;
	if (name == 0xFFFF)
    {
        //表示未使用但需要强制判断
        return TRUE;
    }
	
	if(name <= 0 || name > pp->pi_max)
		return FALSE;

	/*检查是否位于内部输入点与外部输入点之间的无效输入点*/
	if(IS_INN_EXT_GAP(name))
		return FALSE;

	return TRUE;
}


BOOL TestPI(WORD  name)
{
	PIO	pp = &m_io;
	I8 st;
	UI8 ind;

	if(name == 0 || pp->pi_max < name)
		return FALSE;

	/*检查是否位于内部输入点与外部输入点之间的无效输入点*/
	if(IS_INN_EXT_GAP(name))
		return FALSE;
	
	name--;

	ind = (name >> 4);
	if(pp->pi_cfg[name]&PI_FORCE_BIT)
		st=!(pp->pi_cfg[name]>>5&0x01);
	else
		st = (pp->st->pi_input[ind] >> (name & 0x0f) & 0x01);
	
	if(pp->pi_cfg[name] & PI_NEG_BIT)
		st = !st;
	
	return (pp->st->PI_STAT[name] = st);
}	

/*if PI not used , return -1, otherwise return PI value, 0 or 1*/
PI_STATUS CheckPI(WORD name)
{
	PIO	pp = &m_io;
	UI16 tmp;
   	UI8 ind;

	if(name > 0 && name <= m_io.pi_max)
	{
		/*检查是否位于内部输入点与外部输入点之间的无效输入点*/
		if(IS_INN_EXT_GAP(name))
			return PI_NOUSED;
		
		name--;
		
		ind = (name >> 4);
		if(pp->pi_cfg[name]&PI_FORCE_BIT)
			tmp=!(pp->pi_cfg[name]>>5&0x01);
		else
			tmp = (pp->st->pi_input[ind] >> (name & 0x0f) & 0x01);
		if(pp->pi_cfg[name] & PI_NEG_BIT)
			tmp = !tmp;

		pp->st->PI_STAT[name] = tmp;
		
		if(tmp == 0)
			return PI_RESET;
		else
			return PI_SET;
	}
	else if (name == 0xFFFF)
    {
        //未定义点，检测时返回PI_RESET表示无信号。
        return PI_RESET;
    }
	else
		return PI_NOUSED;
}

/*---------------------------------------------------------------------------+
|           code segment                                                     |
+---------------------------------------------------------------------------*/
void GetModelIOMax(PIO pio)
{ 
	
	pio->pi_inn_num = HW_PIMAX;
	pio->po_inn_num = HW_POMAX;

	pio->pi_ext_num = 0;
	pio->po_ext_num = 0;

	if(ChkSysCfg(XF6_EXTIO))
	{
		pio->pi_ext_num = EXT_PBMAX;
		pio->po_ext_num = EXT_PCMAX;
		if(ChkSysCfg(XF6_EXTIO2))
		{
			pio->pi_ext_num += EXT_PBMAX;
			pio->po_ext_num += EXT_PCMAX;
		}
	}	
	
	pio->pi_max =((pio->pi_inn_num + 0x10 - 1)/0x10)*0x10+ pio->pi_ext_num;
	pio->po_max = ((pio->po_inn_num + 0x10 - 1)/0x10)*0x10+ pio->po_ext_num;
}

void SetModelIOMax()
{
	PIO pp = &m_io;
	
	GetModelIOMax(pp);

	pp->po_100us_tick = 0;
	pp->pi_inn_status_msk = (1 << (pp->pi_inn_num%0x10)) - 1;
	pp->pi_inn_status_num = (pp->pi_inn_num + 0x10 - 1)/0x10;
	pp->pi_ext_status_num = (pp->pi_ext_num + 0x10 - 1)/0x10;
	
	pp->po_inn_out_msk = (1 << (pp->po_inn_num%0x10)) - 1;
	pp->po_inn_out_num = (pp->po_inn_num + 0x10 - 1)/0x10;
	pp->po_ext_out_num = (pp->po_ext_num + 0x10 - 1)/0x10;
}

void PICntLoop(PPINT_ST pcnt)
{
	PI_STATUS 	st;
	UI16		w_tm;
	
	if(pcnt == NULL)
  		return;
  
	if((st = CheckPI(pcnt->name)) != PI_NOUSED)
	{
		if(pcnt->bstart ==0)
		{
			pcnt->start_ms = GetMsTick();
			pcnt->cycle_tm = 0;
			if(st == PI_SET)
			{
				pcnt->bstart = 1;
				pcnt->cycle_start_ms = GetMsTick();
				pcnt->last_st = st;	
	      		pcnt->onecyc = 0;
	      		pcnt->cycle_cnt = 0; 
				pcnt->last_cycle_tm = 0;
			}
		}
		else 
		{	
			if(st != pcnt->last_st)
			{
				pcnt->onecyc++;
				pcnt->last_st = st;
				/*每过一个孔，输入点变化两次，需要乘以2*/
			    if(pcnt->onecyc >= pcnt->cnt_per_cycle*2)
				{
					pcnt->onecyc = 0;
					if(pcnt->cnt_per_cycle> 0)
					{
				        pcnt->cycle_cnt ++;
						pcnt->cycle_tm = (GetMsTick() - pcnt->cycle_start_ms);
						pcnt->cycle_start_ms = GetMsTick();
						pcnt->last_cycle_tm = pcnt->cycle_tm;
					}
				}
			}
			else if(pcnt->last_cycle_tm > 0)
			{
				w_tm = (GetMsTick() - pcnt->cycle_start_ms);
				if(pcnt->onecyc > 0)
					w_tm = w_tm*(pcnt->cnt_per_cycle + pcnt->onecyc/2)/pcnt->onecyc;
				
				if(w_tm > (pcnt->last_cycle_tm + pcnt->last_cycle_tm /2))
					pcnt->cycle_tm = w_tm*((w_tm + pcnt->last_cycle_tm/2)/pcnt->last_cycle_tm);
			}
		}
	}
	else
	{
		pcnt->onecyc = 0;
		pcnt->cycle_cnt = 0; 
		pcnt->cycle_tm = 0;
		pcnt->bstart = 0;
	}
}

void ResetPICnt(PPINT_ST pcnt, UI8 pi_name, UI16 cnt_per_cycle)
{
	if(pcnt != NULL)
	{
		memset(pcnt, 0, sizeof(*pcnt));
		pcnt->name = pi_name;
		pcnt->cnt_per_cycle = cnt_per_cycle;
	}
}

UI32 GetPICycleMs(PPINT_ST pcnt)
{
	if(pcnt != NULL && (pcnt->bstart==1))
		return (pcnt->cycle_tm);
	else
		return 0;
}

UI16 GetPICnt(PPINT_ST pcnt)
{
	if(pcnt != NULL)
		return pcnt->cycle_cnt;
	else
		return 0;
}

UI32 GetPI32(int ind)
{
	UI32 tmp = 0;
	PIO pp = &m_io;

	ind = ind << 1;
	if(ind >= 0 && ind < PI_STATUS_NUM)
	{
		if(ind + 1 < PI_STATUS_NUM)
			tmp = (pp->st->pi_input[ind+ 1] << 16);

		tmp += pp->st->pi_input[ind];
	}

	return tmp;
}

UI32 SetPO32(int ind, UI32 val)
{
	UI32 tmp = 0;
	PIO pp = &m_io;

	ind = ind << 1;
	if(ind >= 0 && ind < PI_STATUS_NUM)
	{
		if(ind + 1 < PI_STATUS_NUM)
			pp->st->po_output[ind+ 1] =( (val >> 16) & 0xffff);
		
		pp->st->po_output[ind] = (val & 0xffff);
	}

	return tmp;
}

UI32 GetPO32(int ind)
{
	UI32 tmp = 0;
	PIO pp = &m_io;

	ind = ind << 1;
	if(ind >= 0 && ind < PI_STATUS_NUM)
	{
		if(ind + 1 < PI_STATUS_NUM)
			tmp = (pp->st->po_output[ind+ 1] << 16);

		tmp += pp->st->po_output[ind];
	}

	return tmp;
}

UI16 GetPI16(int ind)
{	
    PIO pp = &m_io;
    if (ind >= 0 && ind < PI_STATUS_NUM)
    {
        return	pp->st->pi_input[ind];
    }
    else
    {
        return 0;
    }
}

UI16 GetPO16(int ind)
{
    PIO pp = &m_io;
    if (ind >= 0 && ind < PI_STATUS_NUM)
    {
        return pp->st->po_output[ind];
    }
    else
    {
        return 0;
    }
}



void ClearAllPOOut()
{
	PIO pp = &m_io;
	PMOTOR pmtr = (PMOTOR)GetPartByID(MK_PART(MOTOR_ID, 1));
	PROBOT  probt = (PROBOT)GetPartByID(MK_PART(ROBOT_ID, 1));
	
    UI8 lub_num;
    PLUB  plub = (PLUB)GetMachineSubPartList(LUB_ID, &lub_num);
	int	i, po_len = 0;
	UI16* ptemppo = NULL;
	PPART pt;
	UI16  cur_name;

	f_iovtest.manualflag = 1;

	for(i = 0; i < PO_STATUS_NUM; i++)
		pp->st->po_output[i] = 0;

	if(pmtr != NULL)
	{
		cur_name = pmtr->po->O_SERVO_EN;
		while(cur_name > 0 && cur_name <= MAX_PO_NUM)
		{
			if(cur_name <= pp->po_max && (pp->st->PO_STAT[cur_name - 1] & 0x01))
			{
				pp->st->po_output[((cur_name - 1) >> 4)] |= (1 << ((cur_name - 1) & 0x0f));
			}

			cur_name = GetPoChainNextPo(cur_name);
		}	  

		if(ChkSysCfg(CO4_MOTR))
		{
			cur_name = pmtr->po->O_MTR_STR;
			while(cur_name > 0 && cur_name <= MAX_PO_NUM)
			{
				if(cur_name <= pp->po_max && (pp->st->PO_STAT[cur_name - 1] & 0x01))
				{
					pp->st->po_output[((cur_name - 1) >> 4)] |= (1 << ((cur_name - 1) & 0x0f));
				}
				cur_name = GetPoChainNextPo(cur_name);
			}
		}	  
	}

	/*排除润滑点*/
	for (i = 0; i < lub_num; i++)
    {
        if (plub != NULL)
        {
            cur_name = plub->po->O_LUBRCTR;
            while (cur_name > 0 && cur_name <= MAX_PO_NUM)
            {
                if (cur_name <= pp->po_max && (pp->st->PO_STAT[cur_name - 1] & 0x01))
                {
                    pp->st->po_output[((cur_name - 1) >> 4)] |= (1 << ((cur_name - 1) & 0x0f));
                }
				cur_name = GetPoChainNextPo(cur_name);
            }
        }
        plub++;
    }
    /*排除机械手输出点*/
	if(probt!= NULL)
	{
			cur_name = probt->po->O_ROBOT;
			while(cur_name > 0 && cur_name <= MAX_PO_NUM)
			{
				if(cur_name <= pp->po_max && (pp->st->PO_STAT[cur_name - 1] & 0x01))
				{
					pp->st->po_output[((cur_name - 1) >> 4)] |= (1 << ((cur_name - 1) & 0x0f));
				}
				cur_name = GetPoChainNextPo(cur_name);
			}		
	}

	/*排除开模结束输出点*/
	if(probt!= NULL)
	{
			cur_name = probt->po->O_R_OPNM;
			while(cur_name > 0 && cur_name <= MAX_PO_NUM)
			{
				if(cur_name <= pp->po_max && (pp->st->PO_STAT[cur_name - 1] & 0x01))
				{
					pp->st->po_output[((cur_name - 1) >> 4)] |= (1 << ((cur_name - 1) & 0x0f));
				}
				cur_name = GetPoChainNextPo(cur_name);
			}		
	}
	
	/*排除模具开模完成输出点*/
	if(probt!= NULL)
	{
			cur_name = probt->po->O_M_OPN;
			while(cur_name > 0 && cur_name <= MAX_PO_NUM)
			{
				if(cur_name <= pp->po_max && (pp->st->PO_STAT[cur_name - 1] & 0x01))
				{
					pp->st->po_output[((cur_name - 1) >> 4)] |= (1 << ((cur_name - 1) & 0x0f));
				}
				cur_name = GetPoChainNextPo(cur_name);
			}		
	}
	
	/*排除机械手急停输出点*/
	if(probt!= NULL)
	{
			cur_name = probt->po->O_R_EMERG;
			while(cur_name > 0 && cur_name <= MAX_PO_NUM)
			{
				if(cur_name <= pp->po_max && (pp->st->PO_STAT[cur_name - 1] & 0x01))
				{
					pp->st->po_output[((cur_name - 1) >> 4)] |= (1 << ((cur_name - 1) & 0x0f));
				}
				cur_name = GetPoChainNextPo(cur_name);
			}		
	}
	/*排除机械手急停2输出点*/
	if(probt!= NULL)
	{
			cur_name = probt->po->O_R_EMERG2;
			while(cur_name > 0 && cur_name <= MAX_PO_NUM)
			{
				if(cur_name <= pp->po_max && (pp->st->PO_STAT[cur_name - 1] & 0x01))
				{
					pp->st->po_output[((cur_name - 1) >> 4)] |= (1 << ((cur_name - 1) & 0x0f));
				}
				cur_name = GetPoChainNextPo(cur_name);
			}		
	}
	/*排除温度部件的输出点*/
	if((pt = GetPartByID(MK_PART(TEMPCTRL_ID, 1))) != NULL)
	{
		po_len = pt->sub_len[PO_ID];
		ptemppo = pt->psub[PO_ID];
		if(ptemppo == NULL)
			po_len = 0;
	}

	for(i = 0; i <  po_len; i ++)
	{
		cur_name = ptemppo[i];
		
		while(cur_name > 0 && cur_name <= MAX_PO_NUM)
		{
			if(cur_name <= pp->po_max && (pp->st->PO_STAT[cur_name - 1] & 0x01))
			{
				pp->st->po_output[((cur_name - 1) >> 4)] |= (1 << ((cur_name - 1) & 0x0f));
			}

			cur_name = GetPoChainNextPo(cur_name);
		}
	}

	/*排除通讯温度1部件的输出点*/
	if((pt = GetPartByID(MK_PART(TEMPCTRLCOMM_ID, 1))) != NULL)
	{
		po_len = pt->sub_len[PO_ID];
		ptemppo = pt->psub[PO_ID];
		if(ptemppo == NULL)
			po_len = 0;
	}

	for(i = 0; i <  po_len; i ++)
	{
		cur_name = ptemppo[i];
		
		while(cur_name > 0 && cur_name <= MAX_PO_NUM)
		{
			if(cur_name <= pp->po_max && (pp->st->PO_STAT[cur_name - 1] & 0x01))
			{
				pp->st->po_output[((cur_name - 1) >> 4)] |= (1 << ((cur_name - 1) & 0x0f));
			}

			cur_name = GetPoChainNextPo(cur_name);
		}
	}

	/*排除通讯温度2部件的输出点*/
	if((pt = GetPartByID(MK_PART(TEMPCTRLCOMM_ID, 2))) != NULL)
	{
		po_len = pt->sub_len[PO_ID];
		ptemppo = pt->psub[PO_ID];
		if(ptemppo == NULL)
			po_len = 0;
	}
	
	for(i = 0; i <  po_len; i ++)
	{
		cur_name = ptemppo[i];
		
		while(cur_name > 0 && cur_name <= MAX_PO_NUM)
		{
			if(cur_name <= pp->po_max && (pp->st->PO_STAT[cur_name - 1] & 0x01))
			{
				pp->st->po_output[((cur_name - 1) >> 4)] |= (1 << ((cur_name - 1) & 0x0f));
			}

			cur_name = GetPoChainNextPo(cur_name);
		}
	}
	
	for( i = 0; i < pp->po_max; i ++)
	{
		if(!(pp->st->po_output[i >> 4] & (1 << (i & 0x0f))))
			pp->st->PO_STAT[i] = 0;
	}

	OutPO();
}

void SetPICfg(WORD name, UI8 cfg)
{
	PIO pp = &m_io;
	if(name <= 0)
		return;
	
	if(name > pp->pi_max)
	{
		//如果该点已超出范围，对cfg清0
		pp->pi_cfg[name - 1] = 0;
		return;
	}

	if((cfg & 0x40) > 0)
	{
		SetPIForceCfg(1);
	}
	pp->pi_cfg[name-1] = cfg;
}

void SetPICore(WORD name, BOOL iscore)
{
	PIO pp = &m_io;

	if(pp != NULL)
	{
		if(name <= 0 || name > pp->pi_max)
			return;

		name--;
		pp->pi_core[name] = iscore;
	}
}

void ClearPIForceCfg()
{
	PIO pp = &m_io;
	UI16 i = 0;
	for (i = 0; i < MAX_PI_NUM;i++)
	{
		pp->pi_cfg[i] = (pp->pi_cfg[i]&0x9F);
	}
	
	pp->force_flag = 0;
}


void SetPIForceCfg(UI16 state)
{
	PIO pp = &m_io;
	
	pp->force_flag = state;
}

/****************************************
 * 函数名： initpbpctable()
 * 功  能： 配置输入输出
****************************************/
BOOL InitIOPart()
{
	static const SUB_INIT_INFO sub_init[]  = IO_INIT_INFO;
	
	PIO	pp = &m_io;

	
	if(!INIT_PART(pp, IO_ID, 1, sub_init))
		return FALSE;

	POChainClear();
	SetModelIOMax();
	
	return TRUE;
}

void POForceOutPut(UI16 len, UI16* poutput)
{

	//清除所有输出点状态 PC强制输出时候只能输出面板发下来需要输出的点
	memset(m_io.st->po_output, 0, sizeof(m_io.st->po_output));
	//将系统置位为IO强制输出状态
	SetRunFlag(INIF_CHKPC);

	if(len > 0 && poutput != NULL)
	{
		if(len > sizeof(m_io.st->po_output)/sizeof(UI16))
			len = sizeof(m_io.st->po_output)/sizeof(UI16);
		MemcpyPro(m_io.st->po_output, poutput, len*sizeof(UI16));

		//硬件输出
		OutPO();
	}
}

void POForceEnd()
{
	//清除所有输出点状态 PC强制输出时候只能输出面板发下来需要输出的点
	if(ChkRunFlag(INIF_CHKPC))
	{
		memset(m_io.st->po_output, 0, sizeof(m_io.st->po_output));
	
		ClrRunFlag(INIF_CHKPC);
	  	ClearAllPOOut();
	}
}

/*清楚输出点链接*/
void POChainClear()
{
	memset(m_io.po_chain, 0, sizeof(m_io.po_chain));
	memset(gs_polinktb, 0, sizeof(UI16)*MAX_PO_LINK_NUM*MAX_PO_NUM);
}

/*把pre_name对应的物理点加入到cur_name的链接上*/
void POChainAdd(WORD pre_name, WORD cur_name)
{
	if(cur_name == 0 || cur_name > MAX_PO_NUM)
		return;

	m_io.po_chain[cur_name - 1] = pre_name;
}

/*设置IO输入输出硬件寄存器地址，A920/A620与A921/A621的硬件地址不同，
需要根据硬件管脚类型配置IO输入输出的寄存器地址*/
void IOSetHWRegAdr(BOOL bnew_adr)
{

}

/*****************************************************************************
 函 数 名  : CheckIoForce
 功能描述  : 检测所有输入点的强制输入是否使用。
 				  只要有一个点使用，则提示"输入点强制激活中"。
 				  否则清掉该报警
 输入参数  : 无
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

*****************************************************************************/

void CheckIoForce()
{
    PIO	pp = &m_io;


	if(pp->force_flag > 0)
	{
		 SET_PART_ERR(pp, ER0_IO_FORCE);
	}
	else
	{
		 CLR_PART_ERR(pp, ER0_IO_FORCE);
	}
}


/*------------------------------------------------------------
//	名称:进终点确认
//	功能:用于取代电眼开关
//	涉及参数:
//	涉及函数:ChkEncodeFwd,ChkEncodeBwd
//	作者:周拓
------------------------------------------------------------*/

BOOL ChkAdFwdInplace(WORD AdNo)
{
    PIO	pp = &m_io;
	
	if(AdNo > 0)
	{
		AdNo = AdNo - 1;
   	 	return (pp->adpos_inplance_in >> AdNo & 0x01);
	}
	else
	{
		return FALSE;
	}
}

BOOL ChkAdBwdInplace(WORD AdNo)
{
    PIO	pp = &m_io;

	if(AdNo > 0)
	{
		AdNo = AdNo - 1;
    	return (pp->adpos_inplance_out >> AdNo & 0x01);
	}
	else
	{
		return FALSE;
	}
}


void SetInplaceInFlag(WORD ServoNo, BOOL flag)	//进到位标记
{
    PIO	pp = &m_io;
	
    if (flag)
    {
        pp->adpos_inplance_in = pp->adpos_inplance_in | (0x0001 << ServoNo);
    }
    else
    {
        pp->adpos_inplance_in = pp->adpos_inplance_in & ~(0x0001 << ServoNo);
    }
}
void SetInplaceOutFlag(WORD ServoNo, BOOL flag)		//退到位标记
{
    PIO	pp = &m_io;
	
    if (flag)
    {
        pp->adpos_inplance_out = pp->adpos_inplance_out | (0x0001 << ServoNo);
    }
    else
    {
        pp->adpos_inplance_out = pp->adpos_inplance_out & ~(0x0001 << ServoNo);
    }
}

void ChkAdposiInplace()		//检查动作是否到位
{
    PMACHINE pp = &m_machine;

    if (CheckPIUsed(pp->pclmp->pi->I_CLS_END))
    {
        if (TestPI(pp->pclmp->pi->I_CLS_END))
        {
            SetInplaceInFlag(pp->pclmp->mh->CLMP_METERCH-1, TRUE);
        }
        else
        {
            SetInplaceInFlag(pp->pclmp->mh->CLMP_METERCH-1, FALSE);
        }
    }
    else
    {
        if (MoveLocationMeterControlProc(pp->pclmp->mh->ADPOSI_EFFCPR, GetMeterPosByCh(pp->pclmp->mh->CLMP_METERCH)))
        {
            SetInplaceInFlag(pp->pclmp->mh->CLMP_METERCH-1, TRUE);
        }
        else
        {
            SetInplaceInFlag(pp->pclmp->mh->CLMP_METERCH-1, FALSE);
        }
    }
}

/*******************************************************************************************
 *Brief			:将对应的关联点插入到链对应的关联数组中
 *Param			:subpo:子输出点	mainpo:主输出点
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20201123
********************************************************************************************/
BOOL InsPoLinkData(UI16 subpo,UI16 mainpo)
{
	if (mainpo > MAX_PO_NUM || mainpo == 0 || subpo == 0)
	{
		return FALSE;
	}
	
	for (int i = 0;i < MAX_PO_LINK_NUM;i++)
	{
		if (gs_polinktb[mainpo - 1][i] == 0)
		{
			gs_polinktb[mainpo - 1][i] = subpo;
			return TRUE;
		}
	}

	SET_PART_ERR(&m_io, ER0_POLINK);
	return FALSE;
}

/*******************************************************************************************
 *Brief			:相关性输出点绑定输出，可以将不同的输出点绑定到一起，做跟随输出动作
 				实例：
 				1、客户需要两个不同的射出点，时可以定义一个射出2，设定射出2跟随射出1动作
 				2、如一个点需要在射出、保压、储料时打开，则关联射出点和储料点。
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20201120
********************************************************************************************/
void InitPoSpecialLink()
{
	UI16 set = 0;
	UI16 po_main[16] = {0};	//跟随点只有一个，但是可能存在多个部件，所以最大定义16
	UI16 po_main_num = 0;
	UI16 part_sn = 0;
	UI16 link_i = 0;
	UI16 part_i = 0;
	UI16 po_main_i = 0;
	UI16 po_sn = 0;
	
	POLINKLOG polink[] = 
	{
		{0xFFAC, 1},
			{MK_PART(INJECT_ID, 1),  GET_IND(PINJECT_PO, O_INJ_CHGSUCK)},	//*注射储料射退阀
			{MK_PART(INJECT_ID, 1),  GET_IND(PINJECT_PO, O_INJ_SOL)},		//射出
			{MK_PART(INJECT_ID, 1),  GET_IND(PINJECT_PO, O_CHR_SOL)},		//储料
			{MK_PART(INJECT_ID, 1),  GET_IND(PINJECT_PO, O_SUC_SOL)},		//射退
		{0xFFAC, 1},
			{MK_PART(CLAMP_ID, 1),	GET_IND(PCLAMP_PO, O_CLS_TOTAL)},	//*模座总阀
			{MK_PART(CLAMP_ID, 1),	GET_IND(PCLAMP_PO, O_CLS_SOL)},		//关模
			{MK_PART(CLAMP_ID, 1),	GET_IND(PCLAMP_PO, O_OPN_SOL)},		//开模
			{MK_PART(EJECT_ID, 1),  GET_IND(PEJECT_PO, O_EJT_FWD)},		//托模进
			{MK_PART(EJECT_ID, 1),  GET_IND(PEJECT_PO, O_EJT_BWD)},		//托模退
			{MK_PART(CORE_ID, 0),	GET_IND(PCORE_PO, O_CP_FWD)},		//中子进
			{MK_PART(CORE_ID, 0),	GET_IND(PCORE_PO, O_CP_BWD)},		//中子退
			{MK_PART(ADJUST_ID, 1),	GET_IND(PADJUST_PO, O_ADJ_FWD)},	//调模进
			{MK_PART(ADJUST_ID, 1),	GET_IND(PADJUST_PO, O_ADJ_BWD)},	//调模退
		{0xFFAC, 1},			
			{MK_PART(INJECT_ID, 1),  GET_IND(PINJECT_PO, O_INJ_OPCL)},	//*射出开关模
			{MK_PART(INJECT_ID, 1),  GET_IND(PINJECT_PO, O_INJ_SOL)},	//射出
			{MK_PART(CLAMP_ID, 1),	GET_IND(PCLAMP_PO, O_CLS_SOL)},		//关模
			{MK_PART(CLAMP_ID, 1),	GET_IND(PCLAMP_PO, O_OPN_SOL)},		//开模
		{0xFFAC, 1},		
			{MK_PART(INJECT_ID, 1),  GET_IND(PINJECT_PO, O_INH_CHR)},	//*射出保压储料
			{MK_PART(INJECT_ID, 1),  GET_IND(PINJECT_PO, O_INJ_SOL)},		//射出
			{MK_PART(INJECT_ID, 1),  GET_IND(PINJECT_PO, O_CHR_SOL)},		//储料
		{0xFFAC, 0},
	};

	for (link_i = 1;link_i < ARRAY_NUM(polink);link_i++)
	{		
		if (polink[link_i].part_id == 0xFFAC)
		{
			continue;
		}
		//初始化设定
		if (polink[link_i-1].part_id == 0xFFAC)
		{
			set = polink[link_i-1].ind;
			part_sn = GET_PART_SN(polink[link_i].part_id);

			if (part_sn == 0)
			{
				po_main_num = GetMachineSubPartNum(GET_PART_TYPE(polink[link_i].part_id));
				for (part_i = 0; part_i < po_main_num; part_i++)
				{
					po_main[part_i] = GetSubDataByID(polink[link_i].part_id|(part_i<<4), PO_ID, polink[link_i].ind);
				}
			}
			else
			{
				po_main_num = 1;
				po_main[0] = GetSubDataByID(polink[link_i].part_id, PO_ID, polink[link_i].ind);
			}
		}
		else
		{
			//第一个点跟随后面所有的点
			if (set == 1)
			{
				part_sn = GET_PART_SN(polink[link_i].part_id);
				if (part_sn == 0)
				{
					for (part_i = 0; part_i < GetMachineSubPartNum(GET_PART_TYPE(polink[link_i].part_id)); part_i++)
					{
						po_sn = GetSubDataByID(polink[link_i].part_id|(part_i<<4), PO_ID, polink[link_i].ind);
						if (po_sn != 0)
						{
							for (po_main_i = 0; po_main_i < po_main_num; po_main_i++)
							{
								if (po_main[po_main_i] != 0)
								{
									InsPoLinkData(po_main[po_main_i], po_sn);
								}
							}
						}
					}
				}
				else
				{
					po_sn = GetSubDataByID(polink[link_i].part_id, PO_ID, polink[link_i].ind);
					if (po_sn != 0)
					{
						for (po_main_i = 0; po_main_i < po_main_num; po_main_i++)
						{
							if (po_main[po_main_i] != 0)
							{
								InsPoLinkData(po_main[po_main_i], po_sn);
							}
						}
					}
				}
			}
			else//后面所有点跟随第一个点
			{
				part_sn = GET_PART_SN(polink[link_i].part_id);
				if (part_sn == 0)
				{
					for (part_i = 0; part_i < GetMachineSubPartNum(GET_PART_TYPE(polink[link_i].part_id)); part_i++)
					{
						po_sn = GetSubDataByID(polink[link_i].part_id|(part_i<<4), PO_ID, polink[link_i].ind);
						if (po_sn != 0)
						{
							for (po_main_i = 0; po_main_i < po_main_num; po_main_i++)
							{
								if (po_main[po_main_i] != 0)
								{
									InsPoLinkData(po_sn, po_main[po_main_i]);
								}
							}
						}
					}
				}
				else
				{
					po_sn = GetSubDataByID(polink[link_i].part_id, PO_ID, polink[link_i].ind);
					if (po_sn != 0)
					{
						for (po_main_i = 0; po_main_i < po_main_num; po_main_i++)
						{
							if (po_main[po_main_i] != 0)
							{
								InsPoLinkData(po_sn, po_main[po_main_i]);
							}
						}
					}
				}
			}
		}
	}
}



