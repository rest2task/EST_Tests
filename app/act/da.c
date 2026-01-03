/******************************************************************************
  文 件 名   : da.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : DA部件及接口函数
  函数列表   :
              DACmd
              DAIsAdjust
              DAMaxMinAdj
              DAMaxMinReset
              DATempMaxSave
              InitDAPart
              SetDA
              SetDACurAdjMode
              SetDATempMax
  修改历史   :
  1.日    期   : 2013年5月17日
    作    者   : ren chaohong
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "com.h"
#include "da.h"
#include "dabios.h"
#include "hwcfg.h"
#include "cfgdb.h"
#include "systempro.h"
#include "can_e702.h"
#include "configdf.h"
#include "machapi.h"
#include "memtbl.h"
#include "hwmid.h"

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
DA m_da;

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

WORD ChangeDaByMode(WORD value)
{
	UI32 valuebuff = 0;
	
	if (ChkSysCfg(CO8_YL4))
	{
		valuebuff = value;
		valuebuff = valuebuff*4095/10000;
		value = valuebuff;
	}

	return value;
}

WORD InChangeDaByMode(WORD value)
{
	UI32 valuebuff = 0;
	
	if (ChkSysCfg(CO8_YL4))
	{
		valuebuff = value;
		valuebuff = valuebuff*10000/4095;
		value = valuebuff;
	}

	return value;
}


/*******************************************************************************************
 *Brief			:DA调试页面中，针对最小值与最大值进行拉伸，DA调试页面中最小值与最大值都是缓冲用
 *Param			:portno:DA通道号	value:输出值	Direction:1,0-10V 2,-10V-0 3,其他
 *Return Value	:拉伸后的输出值
 *Aauthor		:ZT
 *Date			:20191215
********************************************************************************************/
WORD FlDAMaxZeroAdj(int portno, WORD value, WORD Direction)
{
	PDA pp = &m_da;
	
	if (Direction == 1) //采用方向比例阀时，输出0v到+10V
	{
		pp->st->TEMPMIN[portno] = pp->st->TEMPMIN[portno] < DA_ZEROBIT?DA_ZEROBIT:pp->st->TEMPMIN[portno];
		value = (WORD)(((UI32)value * (pp->st->TEMPMAX[portno] - pp->st->TEMPMIN[portno])) / 4096) + pp->st->TEMPMIN[portno];
	}
	else if(Direction == 2)//采用方向比例阀时，输出-10v到0v
	{
		
		pp->st->TEMPMIN[portno] = pp->st->TEMPMIN[portno] < DA_ZEROBIT?DA_ZEROBIT:pp->st->TEMPMIN[portno];
        if(pp->st->TEMPMIN[portno] > (WORD)(((UI32)value * (pp->st->TEMPMAX[portno] - pp->st->TEMPMIN[portno])) / 4096))
		    value = pp->st->TEMPMIN[portno] - (WORD)(((UI32)value * (pp->st->TEMPMAX[portno] - pp->st->TEMPMIN[portno])) / 4096);
        else
            value = 0;
	}
	else
	{
		value = (WORD)(((UI32)value * (pp->st->TEMPMAX[portno] - pp->st->TEMPMIN[portno])) / 4096) + pp->st->TEMPMIN[portno];
	}

	return value;
}

/*******************************************************************************************
 *Brief			:针对最小值与最大值进行拉伸
 *Param			:portno:DA通道号	value:输出值	Direction:1,0-10V 2,-10V-0 3,其他
 *Return Value	:拉伸后的输出值
 *Aauthor		:ZT
 *Date			:20191215
********************************************************************************************/
WORD DAMaxZeroAdj(int portno, WORD value, WORD Direction)
{
	PDA pp = &m_da;
	
	if (Direction == 1)	//采用方向比例阀时，输出0v到+10V
	{
		pp->mh->WDA_ZERO[portno] = pp->mh->WDA_ZERO[portno] < DA_ZEROBIT?DA_ZEROBIT:pp->mh->WDA_ZERO[portno];
		value = (WORD)(((UI32)value * (pp->mh->WDA_MAX[portno] - pp->mh->WDA_ZERO[portno])) / 4096) + pp->mh->WDA_ZERO[portno];
	}
	else if(Direction == 2)//采用方向比例阀时，输出-10v到0v
	{
		/* 2023-11-17 ccz
		这里的计算原本先取反，再"与"上0xFFF，实际上就是4096-正向计算值。
		例如假设1V对应2200，则-1V对应1896，关于2048对称。但是如果最小值不为2048时计算出的就有问题了。
		现改为假设最小值为2000，且1V对应间隔200，则-1V输出值=2000-200=1800 */
		pp->mh->WDA_ZERO[portno] = pp->mh->WDA_ZERO[portno] < DA_ZEROBIT?DA_ZEROBIT:pp->mh->WDA_ZERO[portno];
        if(pp->mh->WDA_ZERO[portno] > (WORD)(((UI32)value * (pp->mh->WDA_MAX[portno] - pp->mh->WDA_ZERO[portno])) / 4096))
		    value = pp->mh->WDA_ZERO[portno] - (WORD)(((UI32)value * (pp->mh->WDA_MAX[portno] - pp->mh->WDA_ZERO[portno])) / 4096);
        else
            value = 0;
	}
	else
	{
		value = (WORD)(((UI32)value * (pp->mh->WDA_MAX[portno] - pp->mh->WDA_ZERO[portno])) / 4096) + pp->mh->WDA_ZERO[portno];
	}

	return value;
}
/***********************************************************
  * @函数名称	SetDA
  * @函数说明   DA输出驱动程序
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
 **********************************************************/ 
void SetDA(int portno, WORD value,WORD Direction)    
{       
	PDA pp = &m_da;

	value = ChangeDaByMode(value);
	
	if(portno >= HW_DAPORTMAX || portno < 0)
		return;

    if(ChkRunFlag(INIF_DEBUG) == 0) 			//不在测试状态,走线性表,需转换为当前(MAX - MIN)区.    whaul
    {		
  		if(pp->FlDAAdjust == 1) 			  // 进入DA校准画面
    	{
			//不是调最大值,如为调最大值,则为直接输出,无需加算法
			if(!pp->F2DAMAX)
			{
				value = FlDAMaxZeroAdj(portno, value, Direction);
	   			//pp->F2DAMAX = TRUE;
			}
			else
			{
				pp->F2DAMAX = FALSE;
				value = value < DA_ZEROBIT?DA_ZEROBIT:value;
			}
			
  		}
   	 	else
   	 	{
			value = DAMaxZeroAdj(portno, value, Direction);
   	 	}
	}

	if(ChkSysCfg(XF4_AAOE))
	{
		if(portno == 3)
			HWDASet(2, value);
		else if(portno == 2)
		{
		}
		else
			HWDASet(portno, value);
	}
	else
		HWDASet(portno, value);
}

void DAMaxMinAdj(UI16 adj_type, UI16 ch, UI16 value)
{
	PDA	pp = &m_da;
	BOOL need_save = FALSE;
	int i;

	db_printf("AD MAX MIN: %d, %d, %d\r\n", adj_type, ch, value);
	
	if (adj_type == 0)	    			/*DA校准,0-不使用; 1-最小值; 2-最大值 */			
	{
		for(i = 0; i < HW_DAPORTMAX; i++)
			SetDA(i, 0,0);
		return;
	}

	if(ch >= MAX_DA_NUM)
		return;
	
	if(adj_type == 1)				/*DA最小值设置*/
	{
		pp->st->WDA_CHANNEL = ch;
		if(pp->mh->WDA_ZERO[ch] != value)
		{
			pp->mh->WDA_ZERO[ch] = value;
			pp->st->TEMPMIN[ch] = value;
			need_save = TRUE;
		}
		if(GetHwModel() == MODEL_I3201 || GetHwModel() == MODEL_I3202 || GetHwModel() == MODEL_I3101)	//IV3201，3101
		{
			if(ch < HW_DAPORTMAX)
				SetDA(ch, pp->mh->WDA_ZERO[ch],0);
			else if((ch == 4)&& ChkSysCfg(XF6_EXTDA))
			{
				E702_Port1_AbsDataWrite(pp->mh->WDA_ZERO[ch],2048); //范围0--4095
				E702_CanTransmit();
				E702_DataSafe(2,pp->mh->WDA_ZERO[ch]);
				E702_CanTransmit();
			}
			else if((ch == 5) && ChkSysCfg(XF6_EXTDA))
			{
				E702_Port1_AbsDataWrite(2048,pp->mh->WDA_ZERO[ch]); //范围0--4095
				E702_CanTransmit();
				E702_DataSafe(4,pp->mh->WDA_ZERO[ch]);
				E702_CanTransmit();
			}
			else if((ch == 6)&& ChkSysCfg(XF6_EXTDA))
			{
				E702_Port2_AbsDataWrite(pp->mh->WDA_ZERO[ch],2048); //范围0--4095
				E702_CanTransmit();
				E702_DataSafe(6,pp->mh->WDA_ZERO[ch]);
				E702_CanTransmit();
			}
			else if((ch == 7)&& ChkSysCfg(XF6_EXTDA))
			{
				E702_Port2_AbsDataWrite(2048,pp->mh->WDA_ZERO[ch]); //范围0--4095
				E702_CanTransmit();
				E702_DataSafe(8,pp->mh->WDA_ZERO[ch]);
				E702_CanTransmit();
			}									
		}
		else
		{
			if(ch < HW_DAPORTMAX)
			    SetDA(ch, pp->mh->WDA_ZERO[ch],0);
		}
	}
	else if(adj_type == 2)				/*DA最大值设置*/
	{

		pp->st->WDA_CHANNEL = ch;
		if(pp->mh->WDA_MAX[ch] != value)
		{
			pp->mh->WDA_MAX[ch] = value;
			pp->st->TEMPMAX[ch] = value;
			need_save = TRUE;
		}
		if(GetHwModel() == MODEL_I3201 || GetHwModel() == MODEL_I3202 || GetHwModel() == MODEL_I3101)	//IV3201，3101
		{
			if(ch < HW_DAPORTMAX)
				SetDA(ch, pp->mh->WDA_MAX[ch],0);				
			else if(ch == 4 && ChkSysCfg(XF6_EXTDA))
			{
				E702_Port1_AbsDataWrite(pp->mh->WDA_MAX[ch],2048); //范围0--4095
				E702_CanTransmit();
				E702_DataSafe(1,pp->mh->WDA_MAX[ch]);
				E702_CanTransmit();
			}
			else if(ch == 5 && ChkSysCfg(XF6_EXTDA))
			{
				E702_Port1_AbsDataWrite(2048,pp->mh->WDA_MAX[ch]); //范围0--4095
				E702_CanTransmit();
				E702_DataSafe(3,pp->mh->WDA_MAX[ch]);
				E702_CanTransmit();
			}
			else if((ch == 6)&& ChkSysCfg(XF6_EXTDA))
			{
				E702_Port2_AbsDataWrite(pp->mh->WDA_MAX[ch],2048); //范围0--4095
				E702_CanTransmit();
				E702_DataSafe(5,pp->mh->WDA_MAX[ch]);
				E702_CanTransmit();
			}
			else if((ch==7)&& ChkSysCfg(XF6_EXTDA))
			{
				E702_Port2_AbsDataWrite(2048,pp->mh->WDA_MAX[ch]); //范围0--4095
				E702_CanTransmit();
				E702_DataSafe(7,pp->mh->WDA_MAX[ch]);
				E702_CanTransmit();
			}
	
		}
		else
		{
			if(ch < HW_DAPORTMAX)
			    SetDA(ch, pp->mh->WDA_MAX[ch],0);		
		}
	}

	if(need_save)
	{
#if 0		
		for(i = 0; i < MAX_DA_NUM/2; i++)
		{
			pp->mh->WDA_MAX[i*2] = pp->mh->WDA_MAX[0];
			pp->mh->WDA_MAX[i*2 + 1] = pp->mh->WDA_MAX[1];
			
			pp->mh->WDA_ZERO[i*2] = pp->mh->WDA_ZERO[0];
			pp->mh->WDA_ZERO[i*2 + 1] = pp->mh->WDA_ZERO[1];
		}
#endif		
		PartCfgSave((PPART)(&m_da), MH_ID);

		/*同时保存到出厂数据*/
		CfgDbPartSave((PPART)(&m_da), MH_ID, 0, 0, FACTORY_PART_DB);
	}
}

void SetDACurAdjMode(int mode)
{
	if(mode == 0)
		m_da.FlDAAdjust = 0;
	else if(mode == 1)		//进入ADDA校正画面,或者正处于调节压力和速度的调节中
	{
		m_da.FlDAAdjust = 1;
		m_da.F2DAMAX = 0;
	}
	else if(mode == 2)		//进入7474画面，对硬件的最大值和最小值进行调整
	{
		m_da.FlDAAdjust = 1;
		m_da.F2DAMAX = 1;
	}
	else
	{
		m_da.FlDAAdjust = 0;
		m_da.F2DAMAX = 0;
	}
}

void SetDATempMax(UI16 ch, UI16 temp_max)
{
	if(ch < MAX_DA_NUM)
		m_da.st->TEMPMAX[ch] = temp_max;
}

void DATempMaxMinSave()
{
//	PDA	pp = &m_da;

	MemcpyPro(m_da.mh->WDA_MAX, m_da.st->TEMPMAX, sizeof(m_da.mh->WDA_MAX));
	MemcpyPro(m_da.mh->WDA_ZERO, m_da.st->TEMPMIN, sizeof(m_da.mh->WDA_ZERO));

    E702_CanTxDelayFlagSet(1);
	PartCfgSave((PPART)(&m_da), MH_ID);

	/*同时保存到出厂数据*/
	CfgDbPartSave((PPART)(&m_da), MH_ID, 0, 0, FACTORY_PART_DB);
}

void SetDATempMin(UI16 ch, UI16 temp_min)
{
	if(ch < MAX_DA_NUM)
		m_da.st->TEMPMIN[ch] = temp_min;
}

BOOL DAIsAdjust()
{
	PDA pp = &m_da;

	if(ChkRunFlag(INIF_DEBUG) || pp->FlDAAdjust) 
		return TRUE;
	else
		return FALSE;
}

void DAMaxMinReset()
{
	PDA pp = &m_da;
	extern const UI16 da_mh_factory[];
	
	MemcpyPro(pp->mh, (UI16*)da_mh_factory, sizeof(*(pp->mh)));
//	Memcpy1((UI16*)pp->mh, (UI16*)da_mh_factory, 32);
	PartCfgSave((PPART)pp, MH_ID);
	PartDataTx((PPART)pp, MH_ID, CAL_LEN(*(pp->mh)), 0, TRUE);
}

void DAMaxMinStCpy()
{
	PDA pp = &m_da;

	MemcpyPro(pp->st->TEMPMAX, pp->mh->WDA_MAX, sizeof(pp->st->TEMPMAX));
	MemcpyPro(pp->st->TEMPMIN, pp->mh->WDA_ZERO, sizeof(pp->st->TEMPMIN));

	PartDataTx((PPART)pp, ST_ID, CAL_LEN(*(pp->st)), 0, FALSE);
}

BOOL DACmd(UI32 cmd, UI32 para)
{
	UI32 ret = TRUE;
	PDA pp = &m_da;

	switch(GET_CMD(cmd))
	{
		case CMD_DATA_RX:
			if((GET_CMD_PARA(cmd) & 0xff) == MH_ID)
			{
				if(((GET_CMD_PARA(cmd) >> 8) & 0xff) == INIT_CFG_RX)
				{
					MemcpyPro(pp->st->TEMPMAX, pp->mh->WDA_MAX, sizeof(pp->st->TEMPMAX));
					MemcpyPro(pp->st->TEMPMIN, pp->mh->WDA_ZERO, sizeof(pp->st->TEMPMIN));
				}
			}
			break;
			
		default:
			ret = FALSE;
			break;
	}

	return ret;
}

void DAForceOut()
{
	int i;
	
	for(i = 0; i < ARRAY_NUM(m_da.last_out_ms); i++)
		m_da.last_out_ms[i] = 0;
}

BOOL InitDAPart()
{
	static const SUB_INIT_INFO sub_init[]  = DA_INIT_INFO;
	PDA	pp = &m_da;

	if(!INIT_PART(pp, DA_ID, 1, sub_init))
		return FALSE;

	DAForceOut();	
	MemcpyPro(pp->st->TEMPMAX, pp->mh->WDA_MAX, sizeof(pp->st->TEMPMAX));
	MemcpyPro(pp->st->TEMPMIN, pp->mh->WDA_ZERO, sizeof(pp->st->TEMPMIN));
	return TRUE;
}

