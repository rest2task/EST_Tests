#include "cpld.h"
#include "hwmid.h"
#include "Adbios.h"
#include "da.h"

UI8 press_sensor1_range = 5;				/*第四路，为压力传感器 0-10v*/
UI8 press_sensor2_range = 5;

WORD ad_channel;
WORD ext_ad_channel;
WORD ad_main_start;
WORD ext_main_start;
volatile WORD   AD_BUFFER[MAX_MAXADPORT] = {0};
volatile WORD   ext_AD_BUFFER[Y209_AD_CHANEL] = {0};
UI8		ad_sensor_range[MAX_MAXADPORT] = {0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5};
UI16 	ad_sensor_press = 0x0000;
WORD ext_ad_channel_used = 0;	//按位操作 bit0 为第一路
extern DA m_da;

/*=========================================================================
 * 函数名: LM73_POWERON
 * 函数说明:  给室温芯片上电
 * 输入参数: 无	
 * 输出参数: 无
 * 增加日期: 2022年7月4日 
===========================================================================*/
void LM73_POWERON()
{
	LM73_POWER &= ~(1 << 0);
}

/*=========================================================================
 * 函数名: LM73_POWEROFF
 * 函数说明:  给室温芯片下电
 * 输入参数: 无	
 * 输出参数: 无
 * 增加日期: 2022年7月4日 
===========================================================================*/
void LM73_POWEROFF()
{
	LM73_POWER |= (1 << 0);
}

void AD_Req(u16 ch)
{
    WORD adcommand;
    adcommand = (uint8_t)(0x5 << 3) + (ch & 0x07);
    AD_CR = adcommand;
}

void EXT_AD_Req(u16 ch)
{
    WORD adcommand;
	
	adcommand = 0xC000 | ((ch & 0xF) << 10) | ((((ch & 0x7) + 5) << 4) | (0x1 << 3) | (ad_sensor_range[ch+hw_cfg.hw_maxadport] & 0x7)) ;

	CPLD_D8_ADS8688_DL_REG = (uint8_t)(adcommand & 0xff);
	CPLD_D8_ADS8688_DH_REG = (uint8_t)((adcommand >> 8)& 0xff);
	CPLD_D8_CMD_REG = (CPLD_OP_READ | CPLD_OP_VALID);
	
}
void AD_start_HAL()
{
	/*不判断上次AD采样是否已完成，防止中断丢失后 AD采样停止问题*/
    {
        ad_main_start = TRUE;
		ad_channel = 0;
        AD_Req(0);
    }
}

void EXT_AD_start_HAL(void)
{
    ext_main_start = TRUE;
	ext_ad_channel = 0; 		  //必须置0保证通道不会错位
    EXT_AD_Req(0);	
}

//==========================================================
// 功能 : 得到AD值    -WD1011
//==========================================================
WORD GetADHW(int portno) 
{ 
	WORD advalue;

	if(portno < MAXADPORT)
	{
	    advalue = AD_BUFFER[portno];  
	  	return(advalue);
	}
	else
		return 0;
}

WORD GetADHW_F508(int portno) 
{ 
	WORD advalue;

	if(portno < Y209_AD_CHANEL)
	{
	    advalue = ext_AD_BUFFER[portno];  
	  	return(advalue);
	}
	else
		return 0;
}

void Start_DA_HAL(uint8_t sel, uint16_t val)
{
	#if 1
    uint16_t tmp_cr;
    tmp_cr = DA_CR;
    tmp_cr = tmp_cr & 0xFF;
    while(tmp_cr>>7)           //忙，则等待    
    {
        tmp_cr = DA_CR;
        tmp_cr = tmp_cr & 0xFF;
    }
    DA_VALUE= (uint16_t)((sel & 0x07)<<12) + val;                        //DATA[13..12]:通道号   DATA[11..0]:数据
	#endif
	//	PRINTF("DA_port:%d VAL:%d\r\n",sel,val);	
}

DA_HW_Q da_q;

void SetDAHW(int portno, WORD value)  
{       
		WORD ind;
    PDA pda = &m_da;
    
   // if(portno < 4)  //IV5000有8段电压通道输出 DK 2017.1030
  	if(portno< HW_DA_PORT_NUM)
    {
		value = (WORD)(value & 0xFFF);

        if(pda != NULL)
            pda->st->DA_VAL1[portno] = value; //主画面显示DA实际值
        
		da_q.da_val[portno] = value;

		if(da_q.tail != da_q.head)
		{
			ind = da_q.tail;
			while(ind != da_q.head)
			{
				if(da_q.hw_q[ind] == portno)
					return;
				ind = ((ind + 1) &(DA_Q_LEN -1));
			}
		}
		
		ind = ((da_q.head + 1) & (DA_Q_LEN -1));
		if(ind != da_q.tail)
		{
			da_q.hw_q[da_q.head] = portno;
			da_q.head = ind;
		}
    }	
}

BOOL da_protect_HAL()
{
	return (BOOL)(DA_CR & DA_PROTECT_STA);					  //返回1：正常；返回0：过流保护	
}

/*=========================================================================
 * 函数名: diable_ext_daccurrent_out(void)
 * @机器类型  IV3100  IV5000 IV3200
 * 功能: 关DA输出 目标芯片: DAC124S085			
 * 说明: 与CPLD配合
 * 增加日期: 2016年9月9日 
===========================================================================*/
void diable_ext_daccurrent_out_HAL(void)
{												
    DA_CR |= 1<<6;									//CUT_PWM=1  
}

/*=========================================================================
 * 函数名: enable_ext_daccurrent_out(void)
 * @机器类型  IV3100  IV5000 IV3200
 * 功能: 开DA输出 目标芯片: DAC124S085			
 * 说明: 与CPLD配合
 * 增加日期: 2016年10月17日 
===========================================================================*/
void enable_ext_daccurrent_out_HAL(void)
{
    DA_CR &= ~(1<<6);							//CUT_PWM=0 			
}

IOVTEST 	f_iovtest;					//IO报警

/*==========================================================================
 * 函数名: io_hvtest()
 * 机器类型： IV5000  IV3200
 * 功能: 测试H24V电压是否正常
 * 返回值：1或0
 * 说明: 与CPLD配合
 * 增加日期: 2016年9月9日 
===========================================================================*/
BOOL io_hvtest()
{
  return (BOOL)(GPIO_IO_PRO & HVTEST_STA);
}
/*=========================================================================
 * 函数名: io_outtest()
 * 机器类型： IV5000  IV3200
 * 功能: 测试IO是否保护
 * 返回值：1或0
 * 说明: 与CPLD配合
 * 增加日期: 2016年9月9日 
===========================================================================*/
BOOL io_outtest()
{
//	uint16_t hv;
//	hv = GPIO_IO_PRO;
	return (BOOL)(GPIO_IO_PRO & OUTTEST_STA);	
}
/*=========================================================================
 * 函数名: io_reset()
 * 机器类型： IV5000  IV3200
 * 功能: 复位IO输出
 * 返回值：无
 * 说明: 与CPLD配合
 * 增加日期: 2016年9月9日 
===========================================================================*/
void io_reset()
{
	GPIO_IO_PRO = (uint16_t)(OUTRESET_STA);
}

void IOVCheck_HAL()
{
	if ((WORD)(Get100UsTick() - f_iovtest.checktime) >= 1000)
	{
		f_iovtest.checktime = Get100UsTick();

		/* IO开关电源检测 2014-3-21 zhangfang */
		if (io_outtest())
		{
		    f_iovtest.ioovertime++;
		    if(f_iovtest.ioovertime >= 3)					//连续300ms检测到才报警
		    {
		        f_iovtest.iooverflag = 1;				//IO输出过载标志位置位
		    }		
		}
		else
		{
		    f_iovtest.ioovertime = 0;
		    f_iovtest.iooverflag = 0;					//IO输出过载标志位清除
		}
		if (io_hvtest())
		{
		    f_iovtest.hvlowtime++;
		    if(f_iovtest.hvlowtime>=20)					//连续500ms检测到才报警
		    {
		        f_iovtest.hvlowflag= 1;
		    }		
		}
		else
		{
		    f_iovtest.hvlowtime = 0;
		    f_iovtest.hvlowflag = 0; 					//测试程序允许解除报警
		}
	}
	
	if (f_iovtest.iooverflag == 1 && f_iovtest.manualflag == 1)
	{
		f_iovtest.manualflag = 0;

		f_iovtest.iooverdelaytime = Get100UsTick();
		while (delt_time(Get100UsTick(), f_iovtest.iooverdelaytime) < 2);		  //延迟至少100us,保证MOS管输出截止

		io_reset();
	}
	else if (f_iovtest.iooverflag == 0)
	{
		f_iovtest.manualflag = 0;
	}

}


uint16_t get_pcb_type(void)
{
	uint16_t pcb_ver;
	pcb_ver = (GPIO_IO_PRO & PCB_TYPE)>>12;
	return (pcb_ver);
}

UI16 GetHwTypeInfo_HAL()		//通过硬件上的4位数据，读取硬件型号，用于软件处规定AD,DA,IO等的最大值
{
	UI16 tmp;
	PHW_CFG_VAR phw = &hw_cfg;

	phw->hw_I3I5 = get_pcb_type() & 0x7;//0x0f;
	
	phw->hw_ADS8688 = 1;

	if(get_pcb_type() & 0x8)
		phw->hw_surtempnew = 0;
	else
		phw->hw_surtempnew = 1;
	return tmp;
}
BOOL cpld_card_test_HAL()
{
	if((GPIO_IO_PRO & OUTCARD_STA) > 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

/*******************************************************************************************
 *Brief 		:CPLD的延迟函数
 *Param 		:无
 *Return Value:无
********************************************************************************************/
inline void cpld_acc_wait()
{

}
/*=========================================================================
 * 函数名: change_channels(uint8_t)
===========================================================================*/
void change_channels(uint8_t sel)
{
	T_CR = (uint8_t)(sel & 0x0F);
}
/*=========================================================================
 * 函数名: Check_Busy_AD_Temp(void)
 * 功能: 温度AD转换判忙 目标芯片: ADS7822		
 * 说明: 与CPLD配合
 * 增加日期: 2016年9月9日 
===========================================================================*/
uint8_t Check_Busy_AD_Temp(void)
{
	uint8_t t;
	t = T_CR & 0xFF;
	return (uint8_t)(t >> 7);
}

/*=========================================================================
 * 函数名: Get_AD_Temp(void)
 * 功能: 温度AD值读取 目标芯片: ADS7822		
 * 说明: 与CPLD配合
 * 增加日期: 2016年9月9日 
===========================================================================*/
uint16_t Get_AD_Temp(void)
{

    uint16_t tmp_temp = 0;
	if(Check_Busy_AD_Temp()==0)
	{
		tmp_temp = (uint16_t)(T_VAL & 0x0fff) + 10;
		return (tmp_temp);
	}
	else
		return 0;
}


