/******************************************************************************
  文 件 名   : admeter.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : AD及电子尺部件
  函数列表   :
              AdMeterCalMax
              AdMeterCmd
              AdMeterLengthModify
              AdMeterLoop
              AdMeterSpeedInit
              AdMeterSpeedLoop
              AdMeterZeroAdj
              GetAdValue
              GetMeterAdValue
              GetMeterLenByCh
              GetMeterMax
              GetMeterPosByCh
              GetPositionAbsolute
			  GetMeterValue
              InitAdMeterPart
              SetAdChType
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
#include "com.h"
#include "hwcfg.h"
#include "AdBios.h"
#include "admeter.h"
#include "memtbl.h"
#include "printf.h"
#include "configdf.h"
#include "cfgdb.h"
#include "machapi.h"
#include "canhead.h"
 #include "config.h"
#include "Servo.h"
#include "AutoRunWithOutServo.h"
#include "hwmid.h"
	 
ADMETER m_ad;

/*******************************************************************************************
 *Brief			:电子尺抖动保护初始化	
 *Param			:ch：电子尺通道号	spd_def：电子尺保护结构体指针
 *Return Value	:FALSE:初始化失败
 *Aauthor		:ZT
 *Date			:20210323
********************************************************************************************/
BOOL SpeedDefInit(PSPEED_DEF spd_def,WORD adDef_pos, WORD ch)
{
	int i = 0;
	
	if(spd_def == NULL)
		return FALSE;

	spd_def->adDef_ch = ch;
	spd_def->threshold = ADDEF_SPD_THRESHOLD_MIN;
	spd_def->adDef_starttm = GetMsTick();
	spd_def->adDef_func = 0;
	spd_def->adDef_pos_no = 0;
	spd_def->adDef_flag = 0;

	for (i = 0;i < ADDEF_POS_BUF_NUM;i++)
	{
		spd_def->adDef_pos_last[i] = adDef_pos; 
	}
	return TRUE;
}


BOOL SpeedDefInitPro()
{
	WORD i = 0;
	PADMETER pad = &m_ad;
	
	for(i = 0; i < 3; i++)
	{
		SpeedDefInit(&(pad->spd_def[i]), GetAdValue(i), i);
	}
	return TRUE;
}

BOOL CheckSpdDefSt(WORD ch)
{
	PADMETER pad = &m_ad;
	ch--;

	if (ch < 3)
	{
		if (pad->spd_def[ch].adDef_err_cn > 0)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}

/*******************************************************************************************
 *Brief			:电子尺抖动保护运行函数
 *Param			:spd_def 电子尺保护结构体指针 adDef_pos 当前位置
 *Return Value	:FALSE:电子尺异常	TRUE:电子尺正常
 *Aauthor		:ZT
 *Date			:20210323
********************************************************************************************/
WORD SpeedDefLoop(PSPEED_DEF spd_def ,WORD adDef_pos)
{
	int i = 0;
	//int spd_buf = 0;	//每周期位移量缓冲区(速度)
	UI16 adDef_err_max_cn = 0;
	int adDef_spd_last[ADDEF_SPD_BUF_NUM] = {0};	//AD码的运动速度
	int adDef_add_last[ADDEF_ADD_BUF_NUM] = {0};	//AD码的加速度
	UI16 add_dire_flag = ADDEF_ADD_BUF_NUM; 		//同方向，超出阀值的次数
	int cnt = 0;
	int pos1= 0,pos2=0;
	int spd_max = 0,spd_min = 0;
	int add_max = 0, add_min = 0;		//最大加速度，最小加速度
	int max_min_diff = 0;			//最大值与最小值速度的差值小于最大速度的1/4
	UI16 flag = 0;	
	
	//关闭滤波功能
	if(ChkSysCfg(COA_CHL4))
		return adDef_pos;

	spd_def->adDef_pos_no++;
	if(spd_def->adDef_pos_no >= ADDEF_POS_BUF_NUM)
	{
		spd_def->adDef_pos_no = 0;
	}
	
	spd_def->adDef_pos_last[spd_def->adDef_pos_no] = adDef_pos;

	//前50ms不做保护,上一次警报异常及开机后都等待50ms后再开始滤波
	if (spd_def->adDef_func > 0)
	{
		//通过位置计算过往的速度(ad/周期)
		for (i = 0;i < ADDEF_SPD_BUF_NUM;i++)
		{
			if ((spd_def->adDef_pos_no-i) >= 0)
				pos1 = spd_def->adDef_pos_last[(UI16)(spd_def->adDef_pos_no-i)];
			else
				pos1 = spd_def->adDef_pos_last[(UI16)(ADDEF_POS_BUF_NUM + (spd_def->adDef_pos_no-i))];
			
			if ((spd_def->adDef_pos_no-i-1) >= 0)
				pos2 = spd_def->adDef_pos_last[(UI16)(spd_def->adDef_pos_no-i-1)];
			else
				pos2 = spd_def->adDef_pos_last[(UI16)(ADDEF_POS_BUF_NUM + (spd_def->adDef_pos_no-i-1))];
	
			adDef_spd_last[i] = pos1 - pos2;
		}

		//通过速度计算过往的加速度
		for (i = 0;i < ADDEF_ADD_BUF_NUM;i++) 
		{
			adDef_add_last[i] = adDef_spd_last[i] - adDef_spd_last[ADDEF_ADD_BUF_NUM + i];

			//判断所有add的方向是否一致,且所有加速度是否都超过阈值，其中预留两组数据为速度的过渡
			//if ((i < 2) || (i > (ADDEF_ADD_BUF_NUM - 3)))
			if (i < 3) 
			{
				if (adDef_add_last[i] > 0)
					add_dire_flag++;

				if (adDef_add_last[i] < 0)
					add_dire_flag--;
			}
			else
			{
				if (adDef_add_last[i] > 0 && (adDef_add_last[i] > spd_def->threshold))
					add_dire_flag++;
	
				if (adDef_add_last[i] < 0 && (INT_ABS(adDef_add_last[i]) > spd_def->threshold))
					add_dire_flag--;
			}
		}

		//24V与GND断开判定
		if ((spd_def->adDef_flag == ADDEF_NOERR) && (INT_ABS(adDef_spd_last[0]) > AD_10VGND_OFF_NUM))
		{
			spd_def->adDef_offspd = (adDef_spd_last[6] + adDef_spd_last[7] + adDef_spd_last[8] + adDef_spd_last[9])/4;

			if (adDef_add_last[0] > 0)
			{
				spd_def->adDef_off_add = 1;
				spd_def->adDef_flag = ADDEF_GNDOFF; //GND断开,GND断开时，是往大走
			}
			else
			{
				spd_def->adDef_off_add = 0;
				spd_def->adDef_flag = ADDEF_10VOFF; //10V断开，10v断开时，是往小走
			}

			if ((spd_def->adDef_pos_no-1) < 0)
				cnt = ADDEF_POS_BUF_NUM + (spd_def->adDef_pos_no-1);
			else
				cnt = (spd_def->adDef_pos_no-1);

			//防止补偿速度异常，做一定的限制
			if (spd_def->adDef_offspd > 10)
				spd_def->adDef_offspd = 10;
			else if (spd_def->adDef_offspd < -10)
				spd_def->adDef_offspd = -10;
			spd_def->adDef_offpos = spd_def->adDef_pos_last[cnt];
			spd_def->adDef_rev_pos = spd_def->adDef_offpos;

			spd_def->adDef_minspd_diff = 1000;
			spd_def->adDef_starttm = GetMsTick();
		}

		//24V与GND恢复判定
		if (spd_def->adDef_flag == ADDEF_GNDOFF || spd_def->adDef_flag == ADDEF_10VOFF)
		{
			if (DIFF_MS(spd_def->adDef_starttm) > 15)
			{
				if (spd_def->adDef_pos_last[spd_def->adDef_pos_no] > 1000 && spd_def->adDef_pos_last[spd_def->adDef_pos_no] < 60535)
				//if((spd_def->adDef_off_add == 1 && (adDef_spd_last[0] < - (AD_10VGND_OFF_NUM/2))) ||(spd_def->adDef_off_add == 0 && (adDef_spd_last[0] > (AD_10VGND_OFF_NUM/2))))
				{
					spd_def->adDef_flag = ADDEF_OFFING; 
					spd_def->adDef_starttm = GetMsTick();
				}
			}
		}
		
		//信号线异常断开判定
		if ((spd_def->adDef_flag == ADDEF_NOERR)&&(add_dire_flag <= 2 || add_dire_flag >= ADDEF_ADD_BUF_NUM*2 - 2))
		{
			spd_max = adDef_spd_last[0];
			spd_min = adDef_spd_last[0];						
			
			for (i = 1;i < ADDEF_ADD_BUF_NUM;i++)	//得到缓存区中10个速度中的最大速度和最小速度
			{
				if (adDef_spd_last[i] > spd_max)
					spd_max = adDef_spd_last[i];

				if (adDef_spd_last[i] < spd_min)
					spd_min = adDef_spd_last[i];
			}

			flag = 0;
			//后半截的加速度相对稳定
			for (i = 1;i < ADDEF_ADD_BUF_NUM/2;i++)
			{
				if (INT_ABS(adDef_spd_last[i] - adDef_spd_last[i-1]) > 10)
				{
					flag = 1;
				}				
			}

			//AD信号断开时，区段内的速度相对稳定

			max_min_diff = MAX(INT_ABS(spd_max),INT_ABS(spd_min))/3;
			max_min_diff = max_min_diff<5?5:max_min_diff;

			//正常信号线断开时其变化速率相对稳定
			//if(ABS_DIFF(spd_max ,spd_min) <= max_min_diff && INT_ABS(spd_max) > 3)
			if (flag == 0 && (ABS_DIFF(spd_max ,spd_min) <= max_min_diff && INT_ABS(spd_max) > 3))
			{
				//断开速度过大为24V或者GND断开
				if (INT_ABS(spd_max) < 200)
				{
					if ((spd_def->adDef_pos_no-ADDEF_ADD_BUF_NUM-1) < 0)
						pos1 = spd_def->adDef_pos_last[ADDEF_POS_BUF_NUM + (spd_def->adDef_pos_no-ADDEF_ADD_BUF_NUM-1)];
					else
						pos1 = spd_def->adDef_pos_last[(spd_def->adDef_pos_no-ADDEF_ADD_BUF_NUM-1)];

					if ((spd_def->adDef_pos_no-ADDEF_ADD_BUF_NUM-7) < 0)
						pos2 = spd_def->adDef_pos_last[ADDEF_POS_BUF_NUM + (spd_def->adDef_pos_no-ADDEF_ADD_BUF_NUM-7)];
					else
						pos2 = spd_def->adDef_pos_last[(spd_def->adDef_pos_no-ADDEF_ADD_BUF_NUM-7)];

					//计算断开时的电子尺移动速度
					spd_def->adDef_offspd = (pos1 - pos2)/6;

					if(spd_def->adDef_offspd >= 0)//正向运行时
					{
						if (add_dire_flag > ADDEF_ADD_BUF_NUM)
							spd_def->adDef_flag = ADDEF_FWD_ACC;//电子尺断开时的速度大于原本的速度	(风险较大)
						else
							spd_def->adDef_flag = ADDEF_FWD_DEC;//电子尺断开时的速度小于原本的速度	(风险较小)
					}
					else
					{								
						if (add_dire_flag > ADDEF_ADD_BUF_NUM)
							spd_def->adDef_flag = ADDEF_BWD_ACC;//电子尺断开时的速度大于原本的速度	(风险较大)
						else
							spd_def->adDef_flag = ADDEF_BWD_DEC;//电子尺断开时的速度小于原本的速度	(风险较小)
					}

					//如果实际速度已经大于50字每周期，则不做滤波
					if (INT_ABS(spd_def->adDef_offspd) > 200 || spd_def->adDef_flag == ADDEF_FWD_DEC || spd_def->adDef_flag == ADDEF_BWD_DEC)
					{
						spd_def->adDef_flag = ADDEF_NOERR;
					}
					else
					{
						//计算断开时的电子尺位置
						if ((spd_def->adDef_pos_no-ADDEF_ADD_BUF_NUM-1) < 0)
							cnt = ADDEF_POS_BUF_NUM + (spd_def->adDef_pos_no-ADDEF_ADD_BUF_NUM-1);
						else
							cnt = (spd_def->adDef_pos_no-ADDEF_ADD_BUF_NUM-1);

						spd_def->adDef_offpos = spd_def->adDef_pos_last[cnt];

						//对断开时加速度的方向做保存，用于结束判定
						if (add_dire_flag > ADDEF_ADD_BUF_NUM)
						{
							spd_def->adDef_off_add = 1;
						}
						else
						{
							spd_def->adDef_off_add = 0;
						}

						//防止补偿速度异常，做一定的限制
						if (spd_def->adDef_offspd > 10)
							spd_def->adDef_offspd = 10;
						else if (spd_def->adDef_offspd < -10)
							spd_def->adDef_offspd = -10;
						
						spd_def->adDef_rev_pos = spd_def->adDef_offpos + ADDEF_ADD_BUF_NUM*spd_def->adDef_offspd;

						//计算合并断开时的滤波次数的加速度和用于异常结束判定
						spd_def->adDef_minspd_diff = 0;
						for (i = 0;i < ADDEF_ADD_BUF_NUM;i++)
						{
							spd_def->adDef_minspd_diff = spd_def->adDef_minspd_diff + adDef_add_last[i];
						}
						spd_def->adDef_minspd_diff = INT_ABS(spd_def->adDef_minspd_diff);
					}
				}					
			}	
		}
		
		//信号线异常恢复判定
		if (spd_def->adDef_flag > ADDEF_NOERR && spd_def->adDef_flag < ADDEF_OFFING)
		{
			add_max = adDef_add_last[0];
			add_min = adDef_add_last[0];						
			
			for (i = 1;i < ADDEF_ADD_BUF_NUM;i++)
			{
				if (adDef_add_last[i] > spd_max)
					add_max = adDef_add_last[i];

				if (adDef_add_last[i] < spd_min)
					add_min = adDef_add_last[i];
			}
			
			if (((spd_def->adDef_off_add == 0)&&(add_max > spd_def->adDef_minspd_diff/2)) || ((spd_def->adDef_off_add == 1)&&(add_min < -spd_def->adDef_minspd_diff/2)))
			{
				spd_def->adDef_flag = ADDEF_OFFING; //突然速度偏差很大时，且本来就在断开中时，此时判断为断开恢复
				spd_def->adDef_starttm = GetMsTick();
			}
		}
	}
	else
	{
		if(DIFF_MS(spd_def->adDef_starttm) > 50)	//每次重置都有50ms的重新采集期
			spd_def->adDef_func = 1;
	}
	
	if (spd_def->adDef_flag == ADDEF_OFFING)
	{
		//判断出异常结束后，做一定时间的延迟判定，防止结束时的部分异常数据干扰。
		if(DIFF_MS(spd_def->adDef_starttm) >= 5)
		{	
			spd_def->adDef_flag = ADDEF_NOERR;	//延迟结束后，恢复正常
			spd_def->adDef_func = 0;
			spd_def->adDef_err_cn = 0;
			spd_def->adDef_starttm = GetMsTick();
		}
		spd_def->adDef_rev_pos = spd_def->adDef_rev_pos + spd_def->adDef_offspd;	//上周期位置增加预测速度，求出当前预测位置

		if (spd_def->adDef_rev_pos > 0xFFFF)
			spd_def->adDef_rev_pos = 0xFFFF;

		if (spd_def->adDef_rev_pos < 0)
			spd_def->adDef_rev_pos = 0;

		adDef_pos = INT_ABS(spd_def->adDef_rev_pos);

	}
	
	if ((spd_def->adDef_flag == ADDEF_FWD_ACC) || (spd_def->adDef_flag == ADDEF_BWD_ACC) 
		|| (spd_def->adDef_flag == ADDEF_10VOFF) || (spd_def->adDef_flag == ADDEF_GNDOFF))	//只有电子尺断开时的速度大于原本的速度时，才进行保护
	{	
		//异常数据处理
		spd_def->adDef_rev_pos = spd_def->adDef_rev_pos + spd_def->adDef_offspd;	//上周期位置增加预测速度，求出当前预测位置

		if (spd_def->adDef_rev_pos > 0xFFFF)
			spd_def->adDef_rev_pos = 0xFFFF;

		if (spd_def->adDef_rev_pos < 0)
			spd_def->adDef_rev_pos = 0;

		adDef_pos = INT_ABS(spd_def->adDef_rev_pos);

		adDef_err_max_cn = 300; //150ms

		spd_def->adDef_err_cn++;	//累计故障次速

		if (spd_def->adDef_err_cn > adDef_err_max_cn)	//次数达10次后报警并重置
		{
			spd_def->adDef_func = 0;
			spd_def->adDef_err_cn = 0;
			spd_def->adDef_flag = 0;
			spd_def->adDef_starttm = GetMsTick();

			//可能不警报。
			//屏蔽警报输出，滤波功能生效。
		
			if ((ADDEF_ALLALARM == 1) && !ChkSysCfg(COA_CHL4C))
			{
				if (spd_def->adDef_ch == 1)
					SET_PART_ERR(GetSys(),ER0_METER_CLM);				
			}
		}		
	}
	else
	{//正常数据处理
		if (spd_def->adDef_err_cn>0)
			spd_def->adDef_err_cn--;

		spd_def->threshold = ADDEF_SPD_THRESHOLD_MIN;
	}

	//
	/*=====================================================*/

	return adDef_pos;
}


WORD GetPresSensorValueByPumpNo(UI16 ch)
{
	if (ch>=g_pump_num)
		return 0;
	
	if (g_pumpn[ch] == NULL)
	{
		return 0;
	}
	else
	{
		return g_pumpn[ch]->McCtrl.RealPres;
	}
}

int	GetMeterPosByCh(UI8 ch)
{
	PADMETER pad = &m_ad;
	
	if (ch <= MAXADPORT && ch > 0)
	{
		return pad->ch_meter[ch - 1];
	}
	else if (ch <= (ADMETER_CH_NUM_EXT + MAXADPORT) && ch > 0)
	{
		return pad->ch_meter_ext[ch - MAXADPORT - 1];
	}
	else
	{
		return 0;
	}
}

int GetMeterMax(UI8 ch)
{
	PADMETER pad = &m_ad;

	if (ch <= MAXADPORT && ch > 0)
	{
		return pad->mh->ADPOSI_MACHINELEN[ch - 1];
	}
	else if (ch <= (ADMETER_CH_NUM_EXT + MAXADPORT) && ch > 0)
	{
		return pad->mh->ADPOSI_MACHINELEN_EXT[ch - MAXADPORT - 1];
	}
	else
	{
		return 0;
	}
}

int	GetMeterLenByCh(UI8 ch)
{
	PADMETER pad = &m_ad;
	
	if (ch <= MAXADPORT && ch > 0)
	{
		return pad->mh->ADPOSI_PTMSETLEN[ch - 1];
	}
	else if (ch <= (ADMETER_CH_NUM_EXT + MAXADPORT) && ch > 0)
	{
		return pad->mh->ADPOSI_PTMSETLEN_EXT[ch - MAXADPORT - 1];
	}
	else
	{
		return 0;
	}
}

WORD GetAdValue(int portno)
{
	PADMETER pad = &m_ad;
	UI16 tmp;

	if(portno <0)
		return 0;	
	
	if(portno < MAXADPORT)
	{
		#if BOGUS
			tmp = AD_BUFFER_AUTO[portno];
		#else	
			tmp = HWGetAdValue(portno);
			pad->st->ADPOSI_METER[portno] = tmp;
			
			if(GetAdChType(portno + 1) == CH_PRESS_5V)
			{
				if(tmp < (MAX_ADVAULE/2))	//此处tmp为UI16，不能直接乘以2，否则会超出65535
					tmp = tmp *2;
				else
					tmp = 0xFFFF;
			}
		#endif
	}
  	else
		tmp = 0;

	return(pad->ch_ad_value[portno] = tmp);
}

/*******************************************************************************************
 *Brief			:获得ad信号的对应通道的换算值
 *Param			:UI16 ch :硬件通道。通道上限为主板通道+F508通道。IV3200主板通道4段，如果ch等
 				于5时，F508上为第一通道。
 *Return Value	:ad换算值
 *Aauthor		:ZT
 *Date			:20191127
********************************************************************************************/
int GetMeterValue(UI16 ch)
{
	PADMETER pad = &m_ad;

	
	if (ch < MAXADPORT)
	{
		return pad->ch_meter[ch];
	}
	else if (ch < (ADMETER_CH_NUM_EXT + MAXADPORT))
	{
		return pad->ch_meter_ext[ch - MAXADPORT];
	}
	else
	{
		return 0;
	}
}

/************************电子尺均值滤波***************************************/
UI16 Ruler_Filtering(UI16 ad_now)
{
	PADMETER pad = &m_ad;
	UI16 i = 0,j = 0,temp = 0;
	UI32 avg_clamp = 0;
	UI16 buff[ADMETER_FILTERING] = {0};
	BOOL exchange = 0;

	pad->ruler_clamp[pad->Put_clamp] = ad_now;
	for(i=0; i<ADMETER_FILTERING-1; i++)
		buff[i] = pad->ruler_clamp[i];
	
	pad->Put_clamp++;

	if(pad->Put_clamp > (ADMETER_FILTERING -1))
		pad->Put_clamp = 0;
		
	for(i=0; i<ADMETER_FILTERING-1; i++)
	{
		exchange = FALSE;
		for(j=ADMETER_FILTERING-1; j>i; j--)
		{
			if (buff[j-1] > buff[j])
			{
				temp = buff[j - 1];
				buff[j - 1] = buff[j];
				buff[j] = temp;
				exchange = TRUE;
			}
		}

		if(exchange == FALSE)	//当没有交换过时，代表已经是由小到大排序好的情况，直接退出当前排序
			break;
	}

	//avg_clamp = ((UI32)buff[ADMETER_FILTERING/2] +  (UI32)buff[(ADMETER_FILTERING/2) -1])/2;
	avg_clamp = ((UI32)buff[1] + (UI32)buff[2]+ (UI32)buff[3]+ (UI32)buff[4]+ (UI32)buff[5]+ (UI32)buff[6])/6;
	
	return ((UI16)avg_clamp);
}

/***********************************************************************************
  * @函数名称	GetPositionAbsolute
  * @函数说明   计算电子尺举例，因为时间的缘故，按照12、13、14的次序进行
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
 ***********************************************************************************/
void GetPositionAbsolute()
{
	PADMETER pad = &m_ad;	
	UI16 ad_value;
	
	int i;
			
	pad->last_sample_100us = Get100UsTick();
	for(i = 0; i < MAXADPORT; i++)
	{
		if((pad->mh->ADPOSI_PTMSETLEN[i] > 0)&& (pad->mh->ADPOSI_MACHINELEN[i] > 0))
		{ 
			if (i == 1 || (GetSysCfg(CO1_CHGE) && (i == 2)))//20220110,增加对托模的检测//目前只针对开合模电子尺做异常检测处理
				ad_value = SpeedDefLoop(&(pad->spd_def[i]),GetAdValue(i));
			else
				ad_value = GetAdValue(i);
			pad->ch_ad_value[i] = ad_value;
			if(pad->ch_type[i] == CH_METER || pad->ch_type[i] == CH_PRESS_10V)		 //0-10V输入AD
			{
				if(ChkSysCfg(CO3_TSTO))
				{
					if(i==1)
					{
						ad_value = Ruler_Filtering(ad_value);
						pad->ch_ad_value[i] = ad_value;
					}
				}
				
				pad->st->ADPOSI_ABSOLUTE[i] = (((UI32)ad_value)*pad->mh->ADPOSI_PTMSETLEN[i] + MAX_ADVAULE/2)/MAX_ADVAULE;
				pad->ch_meter_c[i] = (((UI32)ad_value)*pad->mh->ADPOSI_PTMSETLEN[i]*10 + MAX_ADVAULE/2)/MAX_ADVAULE  - (int)pad->mh->ZSET[i]*10;
			}
			else if(pad->ch_type[i] == CH_PRESS_5V)		// 1-5V输入AD
			{

				pad->st->ADPOSI_ABSOLUTE[i] = ((int)ad_value)*pad->mh->ADPOSI_PTMSETLEN[i]/((int)MAX_ADVAULE*4/5);
			}
			else
			{
				pad->st->ADPOSI_ABSOLUTE[i] = (((UI32)ad_value)*pad->mh->ADPOSI_PTMSETLEN[i] + MAX_ADVAULE/2)/MAX_ADVAULE;
			}
				
			pad->ch_meter[i] = (int)pad->st->ADPOSI_ABSOLUTE[i] - (int)pad->mh->ZSET[i];
			pad->ch_relative[i] =INT_ABS(pad->ch_meter[i]);
			pad->st->ADPOSI_RELATIVE[i] = pad->ch_relative[i];
		}
		else 
		{
			pad->ch_meter[i] = 0;
			pad->ch_relative[i] = 0;
			pad->st->ADPOSI_RELATIVE[i] = 0;
			pad->st->ADPOSI_METERMAX[i] = 0;
		}
	}
#ifdef EXT_ERULER_CAP
	for(i = 0; i < ADMETER_CH_NUM_EXT; i++)
	{
		if(pad->mh->ADPOSI_PTMSETLEN_EXT[i] > 0)
		{
			ad_value = GetADHW_Y209(i);

			if(GetAdChType(i + MAXADPORT + 1) == CH_PRESS_5V)
			{
				if(ad_value < (MAX_ADVAULE/2))	//此处tmp为UI16，不能直接乘以2，否则会超出65535
					ad_value = ad_value *2;
				else
					ad_value = 0xFFFF;
			}
			
			pad->ch_ad_value_ext[i] = ad_value;
			if(pad->ch_type_ext[i] == CH_METER || pad->ch_type_ext[i] == CH_PRESS_10V)		 //0-10V输入AD
			{
	     		pad->st->ADPOSI_ABSOLUTE_EXT[i] = ((((UI32)ad_value)*pad->mh->ADPOSI_PTMSETLEN_EXT[i]+ MAX_ADVAULE/2)/MAX_ADVAULE); 
			}
			else if(pad->ch_type_ext[i] == CH_PRESS_5V)     // 1-5V输入AD
			{

				pad->st->ADPOSI_ABSOLUTE_EXT[i] = ((int)ad_value)*pad->mh->ADPOSI_PTMSETLEN_EXT[i]/((int)MAX_ADVAULE*4/5);
			}
			else
			{
				pad->st->ADPOSI_ABSOLUTE_EXT[i] = ((((UI32)ad_value)*pad->mh->ADPOSI_PTMSETLEN_EXT[i]+ MAX_ADVAULE/2)/MAX_ADVAULE); 
			}
			
			pad->ch_meter_ext[i] = (int)pad->st->ADPOSI_ABSOLUTE_EXT[i] - (int)pad->mh->ZSET_EXT[i]; 
			pad->ch_relative_ext[i] =INT_ABS(pad->ch_meter_ext[i]); 
			pad->st->ADPOSI_RELATIVE_EXT[i] = pad->ch_relative_ext[i];   
			
		}
		else
		{
			pad->ch_meter_ext[i] = 0;
			pad->ch_relative_ext[i] = 0;
			pad->st->ADPOSI_RELATIVE_EXT[i] = 0;
			pad->st->ADPOSI_METERMAX_EXT[i] = 0;
		}
	}
#endif
}					

void AdMeterLoop()
{
	m_ad_start();	
#ifdef EXT_ERULER_CAP
	m_extad_start();
#endif
	GetPositionAbsolute();
}

void AdMeterLoopdly(UI16 dly)
{
	m_ad_start();	
#ifdef EXT_ERULER_CAP
	m_extad_start();
#endif
	if(dly<5||dly>10)		//延迟范围5到10MS
		dly = 5;
	delay_ms(dly);
	GetPositionAbsolute();
}


void SetAdChType(UI16 portno, AD_CH_TYPE type)
{
	PADMETER pad = &m_ad;
	
	if(portno <= MAXADPORT && portno > 0)
	{
		pad->ch_type[portno - 1] = type;
	}
	else if((portno <= (MAXADPORT +ADMETER_CH_NUM_EXT)) && (portno > 0))
	{
		pad->ch_type_ext[portno - 1 - MAXADPORT] = type;
	}
}

AD_CH_TYPE GetAdChType(UI16 portno)
{
	PADMETER pad = &m_ad;
	
	if(portno <= MAXADPORT && portno > 0)
	{
		return pad->ch_type[portno - 1];
	}
	else if((portno <= (MAXADPORT +ADMETER_CH_NUM_EXT)) && (portno > 0))
	{
		return pad->ch_type_ext[portno - 1 - MAXADPORT];
	}
	else
		return CH_METER;
}

void InitAdMeterRang(PADMETER pp)
{
	UI16 i;
	
	for(i = 0; i < ADMETER_CH_NUM; i++)
	{
		pp->ch_type[i] = CH_METER; 	 // 0-10V AD输入			
		pp->ch_type_ext[i]  = CH_METER;	// 0-10V AD输入
	}
}

BOOL InitAdMeterPart()
{
	PADMETER pp = &m_ad;
	static const SUB_INIT_INFO sub_init[]  = ADMETER_INIT_INFO;
	memset(pp, 0, sizeof(*pp));
	if(!INIT_PART(pp, ADMETER_ID, 1, sub_init))
		return FALSE;

	InitAdMeterRang(pp);
	return TRUE;
}

BOOL AdMeterCalMax()
{
	PADMETER pp = &m_ad;
	int i;
	
	for (i=0; i<ADMETER_CH_NUM;i++)
	{
		if(pp->mh->ADPOSI_PTMSETLEN[i] > 0)
			pp->st->ADPOSI_METERMAX[i] = pp->mh->ADPOSI_PTMSETLEN[i] - pp->mh->ZSET[i] < pp->mh->ADPOSI_MACHINELEN[i] ? pp->mh->ADPOSI_PTMSETLEN[i] - pp->mh->ZSET[i] : pp->mh->ADPOSI_MACHINELEN[i];
	}

	for (i=0; i<ADMETER_CH_NUM_EXT;i++)
	{
		if(pp->mh->ADPOSI_PTMSETLEN_EXT[i] > 0)
			pp->st->ADPOSI_METERMAX_EXT[i] = pp->mh->ADPOSI_PTMSETLEN_EXT[i] - pp->mh->ZSET_EXT[i] < pp->mh->ADPOSI_MACHINELEN_EXT[i] ? pp->mh->ADPOSI_PTMSETLEN_EXT[i] - pp->mh->ZSET_EXT[i] : pp->mh->ADPOSI_MACHINELEN_EXT[i];
	}

	PartCfgSave((PPART)pp, MH_ID);
	return PartSubTx((PPART)pp, SUB_BIT(ST_ID), FALSE);
}

void SetExtAdSwitch()
{
	PADMETER pp = &m_ad;
	UI16 i = 0;
	
	for(i = 0; i < ADMETER_CH_NUM_EXT; i++)
	{
		if(pp->mh->ADPOSI_PTMSETLEN_EXT[i] > 0)
		{
			ext_ad_channel_used = ext_ad_channel_used | (0x01<<i);
		}
		else
		{
			ext_ad_channel_used = ext_ad_channel_used & (~(0x01<<i));
		}
	}
}

BOOL AdMeterCmd(UI32 cmd, UI32 para)
{
	UI32 ret = TRUE;
	
	switch(GET_CMD(cmd))
	{
		case CMD_DATA_RX:
			if((GET_CMD_PARA(cmd) & 0xff) == MH_ID)
			{
				AdMeterCalMax();
				SetExtAdSwitch();
			}
			break;
			
		default:
			ret = TRUE;
			break;
	}
	return ret;
}

void AdMeterLengthModify(UI16 ch, UI16 val, WORD type)
{
	PADMETER pp = &m_ad;
	UI32     tmp;
	
	if(type == 0)
	{
		if(ch >= MAXADPORT)
		{
			return;
		}

		if(pp->ch_relative[ch] > 30)
		{
			tmp = (UI32)val * pp->mh->ADPOSI_PTMSETLEN[ch];
			tmp =( tmp + pp->ch_relative[ch]/2)/pp->ch_relative[ch];
			if(tmp > 30 && tmp < 30000)
			{
				pp->mh->ADPOSI_PTMSETLEN[ch] = tmp;
				tmp = (val*pp->mh->ZSET[ch] + pp->ch_relative[ch]/2)/pp->ch_relative[ch];	//4舍五入
				pp->mh->ZSET[ch] = tmp;
			}
		}
		else
			return;
	}
	else
	{
		if(ch >= ADMETER_CH_NUM_EXT)
		{
			return;
		}

		if(pp->ch_relative_ext[ch] > 30)
		{
			tmp = (UI32)val * pp->mh->ADPOSI_PTMSETLEN_EXT[ch];
			tmp =( tmp + pp->ch_relative_ext[ch]/2)/pp->ch_relative_ext[ch];
			if(tmp > 30 && tmp < 30000)
			{
				pp->mh->ADPOSI_PTMSETLEN_EXT[ch] = tmp;
				tmp = (val*pp->mh->ZSET_EXT[ch] + pp->ch_relative_ext[ch]/2)/pp->ch_relative_ext[ch];	//4舍五入
				pp->mh->ZSET_EXT[ch] = tmp;
			}
		}
		else
			return;
	}
	PartCfgSave((PPART)pp, MH_ID);
	PartSubTx((PPART)pp, SUB_BIT(MH_ID), TRUE);
}

void AdMeterZeroAdj(WORD type)
{
	int i;
	PADMETER pp =&m_ad;

	if(type == 0)
	{
		for( i=0; i< MAXADPORT; i++)
		{
			if(pp->mh->ADPOSI_PTMSETLEN[i] > 0)
				pp->st->ADPOSI_METERMAX[i] = (pp->mh->ADPOSI_PTMSETLEN[i] - pp->mh->ZSET[i]) < pp->mh->ADPOSI_MACHINELEN[i] ?
											 (pp->mh->ADPOSI_PTMSETLEN[i] - pp->mh->ZSET[i]) : pp->mh->ADPOSI_MACHINELEN[i];
		}

		PartCfgSave((PPART)pp, MH_ID);
		PartDataTx((PPART)pp, ST_ID, CAL_LEN(pp->st->ADPOSI_METERMAX), GET_IND(PADMETER_STATE, ADPOSI_METERMAX[0]), TRUE);
	}
	else
	{		
		for( i=0; i< ADMETER_CH_NUM_EXT; i++)
		{
			if(pp->mh->ADPOSI_PTMSETLEN_EXT[i] > 0)
				pp->st->ADPOSI_METERMAX_EXT[i] = (pp->mh->ADPOSI_PTMSETLEN_EXT[i] - pp->mh->ZSET_EXT[i]) < pp->mh->ADPOSI_MACHINELEN_EXT[i] ?
											 (pp->mh->ADPOSI_PTMSETLEN_EXT[i] - pp->mh->ZSET_EXT[i]) : pp->mh->ADPOSI_MACHINELEN_EXT[i];
		}

		PartCfgSave((PPART)pp, MH_ID);
		PartDataTx((PPART)pp, ST_ID, CAL_LEN(pp->st->ADPOSI_METERMAX_EXT), GET_IND(PADMETER_STATE, ADPOSI_METERMAX_EXT[0]), TRUE);
	}
}

BOOL AdMeterSpeedInit(UI8 ch, PSPEED_CAL pcal,UI8 accuracy)
{
	int i;
	UI16 pos;
	UI16 cur_100us;
	PADMETER  pp = &m_ad;
	
	if(pcal == NULL)
		return FALSE;

 	if(ch == 0 || ch >  ADMETER_CH_NUM)
		return FALSE;
	if(accuracy>4)
		accuracy = 0;
	pcal->ch = ch - 1;
	pcal->spd_accuracy = accuracy;
	pos = pp->ch_ad_value[pcal->ch];
	cur_100us = (UI16)Get100UsTick() - (SPEED_TM_MAX_CHG/2)*(ARRAY_NUM(pcal->win_pos) - 1);
	for(i = 0; i < ARRAY_NUM(pcal->win_pos); i++)
	{
		pcal->win_pos[i] = pos;
		pcal->win_100us[i]= cur_100us;
		cur_100us+= SPEED_TM_MAX_CHG/2;
	}

	pcal->cur_wind_ind = ARRAY_NUM(pcal->win_pos) - 1;
	pcal->speed = 0;
	pcal->last_chg_tm = SPEED_TM_MAX_CHG/2;
	pcal->cur_speed_samp = ARRAY_NUM(pcal->speed_samp);
	for(i = 0; i < ARRAY_NUM(pcal->speed_samp); i++)
		pcal->speed_samp[i] = 0;
	
	return TRUE;
}


UI32 AdMeterSpeedLoop(PSPEED_CAL pcal)
{
	UI8 win_ind;
	UI16 cur_pos;
	UI16 pos_diff;
	
	UI16 cur_100us;
	UI16 tmp_pos;
	UI16 tmp_100us;
	int i, j;
	PADMETER pp = &m_ad;

	if(pcal == NULL || pcal->ch >= ADMETER_CH_NUM)
		return 0;

	win_ind = pcal->cur_wind_ind;
	if(win_ind >= ARRAY_NUM(pcal->win_pos))
		win_ind = 0;

	if(pcal->cur_speed_samp < ARRAY_NUM(pcal->speed_samp))
	{
		i = 0;
		do
		{
			win_ind = ((pcal->cur_wind_ind - pcal->cur_speed_samp)&(ARRAY_NUM(pcal->win_pos) - 1));
			cur_pos = pcal->win_pos[win_ind];
			cur_100us = pcal->win_100us[win_ind];

			win_ind = ((pcal->cur_wind_ind - pcal->cur_speed_samp - ARRAY_NUM(pcal->win_pos)/2 )&(ARRAY_NUM(pcal->win_pos) - 1));
			tmp_pos = pcal->win_pos[win_ind];
			tmp_100us = pcal->win_100us[win_ind];

			if(tmp_pos > cur_pos)
				tmp_pos = tmp_pos - cur_pos;
			else
				tmp_pos = cur_pos - tmp_pos;
			tmp_100us = cur_100us - tmp_100us;

			if(tmp_100us > 0)
				pcal->speed_samp[pcal->cur_speed_samp] = (((UI32)(tmp_pos)*10000 + (tmp_100us << 1)) )/tmp_100us;
			else
				pcal->speed_samp[pcal->cur_speed_samp] = pcal->speed_samp[0];

			pcal->cur_speed_samp++;
			if(pcal->cur_speed_samp >= ARRAY_NUM(pcal->speed_samp))
			{
				pcal->speed = pcal->speed_samp[0];
				for(j = 1; j < ARRAY_NUM(pcal->speed_samp); j++)
					pcal->speed += pcal->speed_samp[j];

				pcal->speed = (pcal->speed + ARRAY_NUM(pcal->speed_samp)/2)/ARRAY_NUM(pcal->speed_samp);
				if (pcal->spd_accuracy == 1)
				{
					//反馈速度使用0.01mm/s
					pcal->speed = (pcal->speed*pp->mh->ADPOSI_PTMSETLEN[pcal->ch]*10)/MAX_ADVAULE;
				}
				else
				{
					pcal->speed = (pcal->speed*pp->mh->ADPOSI_PTMSETLEN[pcal->ch])/MAX_ADVAULE;
				}
				break;
			}
		}while(i++ < (SPEED_SAMPLE_NUM - 1)/2);
	}	
	else
	{
		cur_pos = pp->ch_ad_value[pcal->ch];		//读取AD值，提高精度
		cur_100us = pp->last_sample_100us;			//使用电子尺最近的采样时间
		tmp_pos = pcal->win_pos[win_ind];
		tmp_100us = pcal->win_100us[win_ind];
		if(cur_pos > tmp_pos)
			pos_diff = cur_pos - tmp_pos;
		else
			pos_diff = tmp_pos - cur_pos;

		if((pos_diff >= SPEED_POS_MIN_CHG || ((UI16)(cur_100us - tmp_100us) >= (pcal->last_chg_tm << 1))) && (UI16)(cur_100us - tmp_100us) > SPEED_MIN_CHG_TM)
		{
			/*新数据，覆盖最老的数据*/
			win_ind =((win_ind + 1)&(ARRAY_NUM(pcal->win_pos) - 1));		

			pcal->win_pos[win_ind] = cur_pos;
			pcal->win_100us[win_ind] = cur_100us;

			pcal->cur_wind_ind = win_ind;
			pcal->last_chg_tm = cur_100us - tmp_100us;
			if(pcal->last_chg_tm < 2)
				pcal->last_chg_tm = 2;

			if(pos_diff == 0)
			{
				pcal->last_chg_tm = (pcal->last_chg_tm << 1);
				if(pcal->last_chg_tm > SPEED_TM_MAX_CHG/2)
					pcal->last_chg_tm = SPEED_TM_MAX_CHG/2;
			}
			
			/*读取一半窗口之前的数据*/
			win_ind = ((win_ind - ARRAY_NUM(pcal->win_pos)/2)&(ARRAY_NUM(pcal->win_pos) - 1));
			tmp_100us = cur_100us - pcal->win_100us[win_ind];
			tmp_pos = pcal->win_pos[win_ind];

			if(tmp_pos > cur_pos)
				tmp_pos = tmp_pos - cur_pos;
			else
				tmp_pos = cur_pos - tmp_pos;

			if(tmp_100us > 0)
			{
				pcal->speed_samp[0] = (((UI32)(tmp_pos)*10000 + (tmp_100us << 1) ))/tmp_100us;
				pcal->cur_speed_samp = 1;
			}
		}
		else
		{
			/*低速时，修正速度计算误差*/
			if(pos_diff < SPEED_POS_MIN_CHG && cur_100us - tmp_100us > pcal->last_chg_tm*2 && pcal->last_chg_tm >= SPEED_TM_MAX_CHG/4)
			{
				pcal->speed = (pcal->speed*pcal->last_chg_tm)/(cur_100us - tmp_100us);
			}
		}
		
	}
	
	return (UI16)(pcal->speed);
}


void AdMeterSpeedInitPro()
{
	AdMeterSpeedInit(1,&(m_ad.speed_cal[0]),0);
	AdMeterSpeedInit(2,&(m_ad.speed_cal[1]),0);
	AdMeterSpeedInit(3,&(m_ad.speed_cal[2]),0);
}

void AdMeterSpeedLoopPro()
{
	m_ad.admeter_speed[0] = AdMeterSpeedLoop(&(m_ad.speed_cal[0]));
	m_ad.admeter_speed[1] = AdMeterSpeedLoop(&(m_ad.speed_cal[1]));
	m_ad.admeter_speed[2] = AdMeterSpeedLoop(&(m_ad.speed_cal[2]));

	GetSys()->pinj->st->INJECT_SPEED = m_ad.admeter_speed[0];
	GetSys()->pclmp->st->CLAMP_SPEED = m_ad.admeter_speed[1];
}

UI16 GetAdMeterSpeed(UI16 ch)
{
	if (ch == 0 || ch > ADMETER_SPSD_NUM)
	{
		return 0;
	}
	else
	{
		return m_ad.admeter_speed[ch - 1];
	}
}


void AdMeterCfgTx()
{
	PartDataTx((PPART)(&m_ad), MH_ID, CAL_LEN(*m_ad.mh), 0, TRUE);
}

