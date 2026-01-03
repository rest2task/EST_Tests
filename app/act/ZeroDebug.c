/*******************************************************************************
** 文件名称: 	ZeroDebug.c
** 版    本：  	1.0
** 工作环境: 	RealView MDK-ARM 4.20
** 作    者: 	张芳
** 生成日期: 	2012-02-25
** 功    能:	测试界面及A310附加功能 
** 相关文件:	stm32f2xx.h
******************************************************************************/
#include "com.h"
#include "stdlib.h"
#include "hwcfg.h"
#include "ZeroDebug.h"
#include "machapi.h"
#include "motor.h"
#include "Useriap.h"
#include "dabios.h"
#include "systempro.h"
#include "hwmid.h"

WORD   	Agecount = 0;				//老练时间计数
WORD    oldtime;					//机器老练时间
WORD 	bzerodata_ok = FALSE;
UI16    zerodata_tempzero[TEMP_CH_NUM] = {0}, zerodata_tempb[TEMP_CH_NUM] = {0};

void FeedDog(void);

void Zerodata_Load()
{
	UI16* psrcdata;
	int i;
   	WORD XOR1=0;
	
	psrcdata = (WORD *)Addr_Zerodata;	 		//读调试参数表
	for(i=0;i<256;i++)
		XOR1 = (WORD)(XOR1 ^ psrcdata[i]);
	if (XOR1 != 0 || psrcdata[WDATA_CORRECT] != 0x55aa)				//此值固定，否则视为参数出错，需复位参数
	{
		bzerodata_ok = FALSE;
		oldtime = 0;
	}
	else
	{
		oldtime = psrcdata[WTIME_AGEB];
		bzerodata_ok = TRUE;

        for(i = 0;i < TEMP_CH_NUM;i++)
        {
            zerodata_tempzero[i] = psrcdata[WTEMP_CH0ZERO + (i%8)];
            zerodata_tempb[i] = psrcdata[WTEMP_CH0B + (i%8)];
        }
	}
}

UI16 Zerodata_AgeTM()
{
	return oldtime;
}

WORD* ZeroData_Get()
{
	Zerodata_Load();

	if(bzerodata_ok)
	{
		return ((WORD *)Addr_Zerodata);
	}
	else
	{
		return NULL;
	}
}
/*******************************************************************************
  * @函数名称	ZeroDebug
  * @函数说明   7474校正界面下的校准及设置
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void ZeroDebug()
{
	WORD Resetkey;
	if (ChkRunFlag(INIF_DEBUG)) 						/*进入调试界面,ctrl_tbl[WDEBUG_STATE] = 1*/
	{
/******************************* 手动 紧停按键测试	WD110311***************************************/
		Resetkey = RESETKEYIDR;
		if (ChkSysCfg(XF0_REMERG)) Resetkey=!RESETKEYIDR;
		if (Resetkey)
		{
			m_on_canb_led();
			while(1)
			{
				FeedDog();
				LedFlash();
				if (!(RESETKEYIDR))
				{
					m_off_canb_led();
					break;
				}
			}
		}
		if (MANUALKEYIDR)
		{
			m_on_cana_led();
			while(1)
			{
				 FeedDog();
				 LedFlash();
				if (!(MANUALKEYIDR))
				{
					m_off_cana_led();
					break;
				}
			}
		}
	}
}

