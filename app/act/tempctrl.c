/*===========================================================================+
|  Class    : temperature control                                            |
|  Task     : temperature control  file                                      |
|----------------------------------------------------------------------------|
|  Compile  : ADS1.2                                                    	 |
|  Link     : ADS1.2                                                 		 |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : He zhenhua                                                     |
|  Version  : V1.00                                                          |
|  Creation : 06/04/2003                                                     |
|  Revision :
|----------------------------------------------------------------------------|
|  Author   : whaul
|  Version  : V1.00                                                          |
|  Creation : 14/10/2011                                                     |
|  Revision : for A610                                                       |
+===========================================================================*/
#include "com.h"
#include "hwcfg.h"
#include "ioproc.h"
#include "alarm.h"
#include "canhead.h"
#include "stdlib.h"
#include "machapi.h"
#include "systempro.h"
#include "zerodebug.h"
#include "cfgdb.h"
#include "hwcfg.h"
#include "cpld.h"
#include "tempctrl.h"
#include "hwmid.h"
#include "lm73.h"


/*-----------------------------------------------------------------------------------+
|   变量定义                                                                          |
+------------------------------------------------------------------------------------*/
TEMPAMBIENT_STRU  g_ambient;
TEMPCTRL		m_temp;
TEMP_STRU	  	  g_temp;
WORD			g_outtempuv_cn = 9;

extern const WORD* T_tab;

//K型热电偶负温度对应微伏值表
extern WORD T_tabN[10];
WORD ADtoTemp(WORD ADVALUE);


//===========================================================================
// 功能:温度计算函数  -WD1011
//===========================================================================
void   pulsetotemp()
{
	PTEMPCTRL pp= &m_temp;
	PTEMPCTRLCOMM ptemp = g_tempcomm[0];
  	WORD  temp;
//	WORD ww;
  	WORD  mdvalue;  
  	BOOL  TLOW = 0;
	WORD WTEMP_REAL;

  	mdvalue = ptemp->tempctrldatastu.tempdata[ptemp->curchnl].last; 
  	ptemp->st->WTEMP_ADVALUE = mdvalue; 

  	if ((mdvalue+130) < pp->zd->WTEMP_CHZERO[ptemp->curchnl])		//温度低于零下20度时报错777.	接反/短路
  	{
  		ptemp->st->DATEMP_REAL[ptemp->curchnl] = 777;
		ptemp->st->WTEMP_RELAT = 777;
  	}
  	else
  	{
  	  	if (pp->zd->WTEMP_CHB[ptemp->curchnl] > 250) 		//系数范围限定,不大于(4096 * 1000 / 16397)  = (D400 -Dzero) * 1000 / 16397
   	  	{
  		  	pp->zd->WTEMP_CHB[ptemp->curchnl] = 248;
  	  	}
  	  
  	  	if (mdvalue < pp->zd->WTEMP_CHZERO[ptemp->curchnl])
  	  	{
  	 	 	temp = (WORD)(pp->zd->WTEMP_CHZERO[ptemp->curchnl] - mdvalue);
  	 	 	TLOW = 1;
  	  	}
  	  	else
  	  	{
  	  		temp = (WORD)(mdvalue - pp->zd->WTEMP_CHZERO[ptemp->curchnl]);		   //Dx - Dzero
  	  	 	TLOW = 0;
      	} 
		if(pp->zd->WTEMP_CHB[ptemp->curchnl] > 0)
  	  		temp = (WORD)(temp * 1000 /pp->zd->WTEMP_CHB[ptemp->curchnl]); 		   //转成微伏值  
		else
			temp = (WORD)(temp * 1000);
		WTEMP_REAL = temp;													   //实际UV值
	  
  	  	if (WTEMP_REAL >= T_tab[MAX_TEMPTBL])				//热电偶断线开路(负温度时,此值不大于480对应值值,即负温度不会超过零下480度)
  	  	{
  	    	ptemp->st->DATEMP_REAL[ptemp->curchnl] = 999; 
			ptemp->st->WTEMP_RELAT = 999;	
  	  	}
      	else
  	  	{
//叠加室温对应UV值,送PID计算用
			if (TLOW)		 			//热电偶为负温度,低于短路值.
	  		{
	  	  		if ((WORD)(pp->st->WTEMP_LM73UV & 0x8000))		//室温为负，热电偶为负，则叠加,考虑实际应用,此时置0
		  		{	
	 // 		  		temp = (WORD)(temp + (WORD)(ctrl_tbl[WTEMP_LM73UV] & 0x7FFF));
		  	  		temp = 0;
				}
	  	  		else										//室温为正,热电偶为负
	  	  		{
	  	      		if (pp->st->WTEMP_LM73UV > temp)						//室温绝对值大于热电偶绝对值
			  		{
				  		temp = (WORD)(pp->st->WTEMP_LM73UV - temp);       //叠加室温对应微伏值
				  		TLOW = 0;											//叠加后为正温度
			  		}
			  		else					 								//室温绝对值小于热电偶绝对值
			  		{
	//				  		temp = (WORD)(temp - (WORD)(ctrl_tbl[WTEMP_LM73UV] & 0x7FFF));
						temp = 0;
			  		}
	  	  		}	  
	  		}	
	  		else	   													//热电偶为正温度
	  		{ 
  		  		if ((WORD)(pp->st->WTEMP_LM73UV & 0x8000))					//室温为负温度
		  		{
		  			if ((WORD)(pp->st->WTEMP_LM73UV & 0x7FFF) > temp)	//负室温绝对值大于热电偶绝对值
					{
	//			 		temp = (WORD)((WORD)(ctrl_tbl[WTEMP_LM73UV] & 0x7FFF) - temp);
						temp = 0;
				 		TLOW = 1;
					}
					else									 			//负室温绝对值小于热电偶绝对值
					{
						temp = (WORD)(temp - (WORD)(pp->st->WTEMP_LM73UV & 0x7FFF));
		  			}
		  		}
		  		else
		  		{
		  			temp = (WORD)(temp + pp->st->WTEMP_LM73UV);       	//叠加室温对应微伏值	
  	  	  		}
	  		}		
  	  	  	ptemp->tempctrldatastu.tempdata[ptemp->curchnl].test  = temp;	//对应微伏表中的具体数据   ,此数据已叠加室温对应微伏值  	 
  //得到实际显示值
  		  	if (TLOW) 									//叠加后为负值
  		  	{
				ptemp->st->WTEMP_RELAT = 0;
			}
  		  	else
  		  	{  
  		  	  	mdvalue = ADtoTemp(temp);
				ptemp->st->WTEMP_RELAT = mdvalue; 
  		  	}

      	  	ptemp->st->DATEMP_REAL[ptemp->curchnl] = ptemp->st->WTEMP_RELAT;	//加室温后得到显示温度值
  	  	}
   	}
}

/*******************************************************************************************
  * @函数名称	pulsetotemp14s
  * @函数说明   温度计算
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
 ****************************************************************************************/
void   pulsetotemp14s()
{
	PTEMPCTRLCOMM ptemp = g_tempcomm[0];
	PTEMPCTRL pp= &m_temp;
	WORD 	WTEMP_REAL;
	long  temp;
  	WORD  mdvalue;  
  	BOOL	TLOW = 0;
  	mdvalue = ptemp->tempctrldatastu.tempdata[ptemp->curchnl].last; 
  	ptemp->st->WTEMP_ADVALUE = mdvalue;

	//新增条件:外部室温导线不检查反接
	if(ChkSysCfg(CO9_XIND) && (g_outtempuv_cn == ptemp->curchnl))
	{
      	if (pp->zd->WTEMP_CHB[ptemp->curchnl] > 250) 				/*系数范围限定,不大于(4096 * 1000 / 16397)*/
   	  	{
  		  	pp->zd->WTEMP_CHB[ptemp->curchnl] = 248;
  	  	}
  
  	  	if (mdvalue < pp->zd->WTEMP_CHZERO[ptemp->curchnl])
  	  	{
  	 		temp = (WORD)(pp->zd->WTEMP_CHZERO[ptemp->curchnl] - mdvalue);
  	 	 	TLOW = 1;
  	  	}
  	  	else
  	  	{
  	  		temp = (WORD)(mdvalue - pp->zd->WTEMP_CHZERO[ptemp->curchnl]);
  	  	 	TLOW = 0;
      	} 

	 	if(pp->zd->WTEMP_CHB[ptemp->curchnl] > 0)
	  	  	temp = (WORD)(temp * 1000 / pp->zd->WTEMP_CHB[ptemp->curchnl]); /*转成微伏值*/
		else
			temp = (WORD)(temp * 1000);
		
	  	WTEMP_REAL = temp;									/*实际UV值*/
		

		if (!TLOW || (WTEMP_REAL >= T2P(100)))
		{
			//外部室温大于主板室温时，关闭室温补偿功能
			temp = (WORD)(pp->st->WTEMP_LM73UV & 0x7FFF);
			
			if((WORD)(pp->st->WTEMP_LM73UV & 0x8000))
			{
				TLOW = 1;
			}
			else
			{
				TLOW = 0;
			}
		}
		else
		{
			if ((WORD)(pp->st->WTEMP_LM73UV & 0x8000))			/*室温为负，热电偶为负，则叠加 ,	考虑实际应用,负值置0*/
	  		{	
  	  			temp = (WORD)(pp->st->WTEMP_LM73UV & 0x7FFF) + temp;
				TLOW = 1;
			}
  	  		else													/*热电偶负温度对应电压值小于室温对应电压值(例:环境温度5度,热电偶处温度为零下2度以下时）*/
  	  		{
  	  			if((WORD)(pp->st->WTEMP_LM73UV & 0x7FFF) > temp)
  	  			{
					TLOW = 0;
					temp = (WORD)(pp->st->WTEMP_LM73UV & 0x7FFF) - temp;
				}
				else
				{
					TLOW = 1;
					temp = (WORD)(temp - (WORD)(pp->st->WTEMP_LM73UV & 0x7FFF));
				}
  	      		
  	  		}
					
			if (WTEMP_REAL > T2P(40))
			{
				//外部温度小于主板室温大于40摄氏度时，保持40℃

				if ((WORD)(pp->st->WTEMP_LM73UV & 0x8000))			/*室温为负，热电偶为负，则叠加 ,	考虑实际应用,负值置0*/
		  		{	
					temp = (WORD)(pp->st->WTEMP_LM73UV & 0x7FFF) + T2P(40);
					TLOW = 1;
				}
	  	  		else													/*热电偶负温度对应电压值小于室温对应电压值(例:环境温度5度,热电偶处温度为零下2度以下时）*/
	  	  		{
					if((WORD)(pp->st->WTEMP_LM73UV & 0x7FFF) > T2P(40))
					{
						TLOW = 1;
						temp = (WORD)((WORD)(pp->st->WTEMP_LM73UV & 0x7FFF) - T2P(40));
					}
					else
					{
						TLOW = 0;
						temp = (WORD)(T2P(40) - (WORD)(pp->st->WTEMP_LM73UV & 0x7FFF));
					}
	  	  		}
			}
		}

		if(TLOW > 0)
			ptemp->outtempuv_buff = -temp;	//记录外部冷端温度的mv
		else
			ptemp->outtempuv_buff = temp;	//记录外部冷端温度的mv


		if (TLOW > 0)
		{
			temp = 0;
		}
		
  		ptemp->tempctrldatastu.tempdata[ptemp->curchnl].test  = temp;				/*对应微伏表中的具体数据   ,此数据已叠加室温对应微伏值*/  	 
		/*UV值转换为温度显示*/
		if (TLOW)
		{
			ptemp->st->WTEMP_RELAT = 0;
			ptemp->tempctrldatastu.tempdata[ptemp->curchnl].test = 0;
		}
		else
		{  
	  		mdvalue = ADtoTemp(temp);
		  	ptemp->st->WTEMP_RELAT = mdvalue;
		}			

		if (WTEMP_REAL >= T2P(100))
		{
			ptemp->st->WTEMP_RELAT = 998;
			ptemp->st->DATEMP_REAL[ptemp->curchnl] = 998;	
			SET_PART_ERR(pp, ER0_WTEMPEXT);
		}
		else
		{
			ptemp->st->DATEMP_REAL[ptemp->curchnl] = ptemp->st->WTEMP_RELAT;
		}
	}
	else
	{
		if ((mdvalue+130) < pp->zd->WTEMP_CHZERO[ptemp->curchnl])		/*温度低于零点值时报错.	接反/短路*/
	  	{
	      	ptemp->st->DATEMP_REAL[ptemp->curchnl] = 777;		
			ptemp->st->WTEMP_RELAT = 777;
	  	}
	  	else
	  	{
	      	if (pp->zd->WTEMP_CHB[ptemp->curchnl] > 250) 				/*系数范围限定,不大于(4096 * 1000 / 16397)*/
	   	  	{
	  		  	pp->zd->WTEMP_CHB[ptemp->curchnl] = 248;
	  	  	}
	  
	  	  	if (mdvalue < pp->zd->WTEMP_CHZERO[ptemp->curchnl])
	  	  	{
	  	 		temp = (WORD)(pp->zd->WTEMP_CHZERO[ptemp->curchnl] - mdvalue);
	  	 	 	TLOW = 1;
	  	  	}
	  	  	else
	  	  	{
	  	  		temp = (WORD)(mdvalue - pp->zd->WTEMP_CHZERO[ptemp->curchnl]);
	  	  	 	TLOW = 0;
	      	} 

		 	if(pp->zd->WTEMP_CHB[ptemp->curchnl] > 0)
		  	  	temp = (WORD)(temp * 1000 / pp->zd->WTEMP_CHB[ptemp->curchnl]); /*转成微伏值*/
			else
				temp = (WORD)(temp * 1000);
			
		  	WTEMP_REAL = temp;									/*实际UV值*/
			/*因开路时读到AD值0xfff，有时也会转换正常温度，最大温度表减1*/
			if (WTEMP_REAL >= T_tab[MAX_TEMPTBL - 1])					/*热电偶断线开路(负温度时,此值不大于480对应值值,即负温度不会超过零下480度)*/
			{
	  	      	ptemp->st->DATEMP_REAL[ptemp->curchnl] = 999; 	
	  	      	ptemp->st->WTEMP_RELAT = 999;
	  	  	}
	      	else
	  	  	{	
	  	  		//不使用外部室温导线
	  	  		if (!ChkSysCfg(CO9_XIND))
				{
					if (TLOW)
			  		{
				  		if ((WORD)(pp->st->WTEMP_LM73UV & 0x8000))			/*室温为负，热电偶为负，则叠加 ,	考虑实际应用,负值置0*/
				  		{	
			  	  			temp = 0;
						}
			  	  		else													/*热电偶负温度对应电压值小于室温对应电压值(例:环境温度5度,热电偶处温度为零下2度以下时）*/
			  	  		{
			  	      		if (pp->st->WTEMP_LM73UV > temp)					/*室温绝对值大于热电偶绝对值*/
					  		{
						  		temp = (WORD)(pp->st->WTEMP_LM73UV - temp);   /*叠加室温对应微伏值*/
						  		TLOW = 0;										/*叠加后为正温度*/
					  		}
					  		else					 							/*室温绝对值小于热电偶绝对值  负值置0*/
					  		{
								temp = 0;
					  		}
			  	  		}	  
			  		}		
			  		else
			  		{ 
		  		  		if ((WORD)(pp->st->WTEMP_LM73UV & 0x8000))			/*室温为负温度*/
				  		{
				  			if ((WORD)(pp->st->WTEMP_LM73UV & 0x7FFF) > temp)	/*负室温绝对值大于热电偶绝对值*/
							{
								temp = 0;										/*叠加为负值,置0*/
						 		TLOW = 1;
							}
							else									 			/*负室温绝对值小于热电偶绝对值*/
							{
								temp = (WORD)(temp - (WORD)(pp->st->WTEMP_LM73UV & 0x7FFF));
				  			}
				  		}
				  		else
				  		{
				  			temp = (WORD)(temp + pp->st->WTEMP_LM73UV);       /*叠加室温对应微伏值*/	
		  	  	  		}
			  		}		
				}
				else
				{
					/*热电偶uV值和室温转成uV值过行叠加.*/  
			  		if (TLOW)
			  		{
				  		if (ptemp->outtempuv_buff <= 0)			/*室温为负，热电偶为负，则叠加 ,	考虑实际应用,负值置0*/
				  		{	
			  	  			temp = 0;
							TLOW = 1;
						}
			  	  		else													/*热电偶负温度对应电压值小于室温对应电压值(例:环境温度5度,热电偶处温度为零下2度以下时）*/
			  	  		{
			  	      		if (ptemp->outtempuv_buff > temp)					/*室温绝对值大于热电偶绝对值*/
					  		{
						  		temp = (WORD)(ptemp->outtempuv_buff - temp);   /*叠加室温对应微伏值*/
						  		TLOW = 0;										/*叠加后为正温度*/
					  		}
					  		else					 							/*室温绝对值小于热电偶绝对值  负值置0*/
					  		{
					  			TLOW = 1;
								temp = 0;
					  		}
			  	  		}	  
			  		}		
			  		else
			  		{ 
		  		  		if (ptemp->outtempuv_buff <= 0)			/*室温为负温度*/
				  		{
				  			if (abs(ptemp->outtempuv_buff) > temp)	/*负室温绝对值大于热电偶绝对值*/
							{
								temp = 0;										/*叠加为负值,置0*/
						 		TLOW = 1;
							}
							else									 			/*负室温绝对值小于热电偶绝对值*/
							{
								temp = (WORD)(temp + ptemp->outtempuv_buff);
								TLOW = 0;
				  			}
				  		}
				  		else
				  		{
				  			temp = (WORD)(temp + ptemp->outtempuv_buff);       /*叠加室温对应微伏值*/
							TLOW = 0;
		  	  	  		}
			  		}
				}
				
	  	  		ptemp->tempctrldatastu.tempdata[ptemp->curchnl].test  = temp;				/*对应微伏表中的具体数据   ,此数据已叠加室温对应微伏值*/  	 
				/*UV值转换为温度显示*/
	  			if (TLOW)
	  			{
	  				ptemp->st->WTEMP_RELAT = 0;
				}
	  			else
				{  
	  		  		mdvalue = ADtoTemp(temp);
	  			  	ptemp->st->WTEMP_RELAT = mdvalue;
	  			}
				
	   			ptemp->st->DATEMP_REAL[ptemp->curchnl] = ptemp->st->WTEMP_RELAT;	
	  	  	}
	   	}
	}  	
}


/*--------------------------------------------------------------------------------------------------------------+
	温度转换函数                       
	1、读温度计数器寄存器数据                                            
	2、判断须检测的温度通道测试值是否正常。
	2.1若正常，读出温度并计算、转换成度数,传给上位机显示；计算该通道的PK值；清温度计数器，改变测试温度通道号。
	2.2若不正常，则不改变PK值同时对检测次数计数COUNT[]。
	2.3判断COUNT[]的值是否大于5，若大于5则按2.1处理。否则不处理。
	//对两次采样进行处理,当两次采样都有效(无效),则值为两次平均,如一次有效,一次无效,则取有效那种值
//A610  对8次采样进行处理,偏差小于/大于MAXPULSENEG归为两类,进行平均后赋值给g_temp.tempdata[ptemp->curchnl].last.
+---------------------------------------------------------------------------------------------------------------*/
void   validheatvoltage()
{
	PTEMPCTRLCOMM pp = g_tempcomm[0];
	if(g_temp.cnvalid > 0)
		pp->tempctrldatastu.tempdata[pp->curchnl].last = g_temp.current_sum/ g_temp.cnvalid; 
	else if(g_temp.cninvalid > 0)
		pp->tempctrldatastu.tempdata[pp->curchnl].last = g_temp.invalid_sum/ g_temp.cninvalid; 		
}

void   validheatvoltage14snew()
{
	PTEMPCTRLCOMM pp = g_tempcomm[0];
	if(g_temp.cnvalid > (MAXTEMPCOUNT*7/10))
		pp->tempctrldatastu.tempdata[pp->curchnl].last = g_temp.current_sum/g_temp.cnvalid;
	else //if(g_temp.cninvalid > 0)
		pp->tempctrldatastu.tempdata[pp->curchnl].last = (g_temp.invalid_sum + g_temp.current_sum)/ (g_temp.cninvalid + g_temp.cnvalid);
}

void Temp_cnt_init()
{
	g_temp.count = 0;
	g_temp.current_sum = 0;
	g_temp.invalid_sum = 0;
	g_temp.cninvalid = 0;
	g_temp.cnvalid = 0;	  
	g_temp.fail_count = 0;
}

/*******************************************************************************************
  * @函数名称	getcurtemp
  * @函数说明   温度采集，并平均，转换成温度值，采集次数400次
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
 ****************************************************************************************/
int    getcurtemp()
{
	unsigned long  temp;
	PTEMPCTRLCOMM ptemp = g_tempcomm[0];
  
	temp = m_read_temp_counter();  				/* 读温度*/;

	if (temp)										/*读到有效值*/
	{	
		temp = temp - 10;							/*与温度为零时区别*/

		if (ABS_DIFF(temp, ptemp->tempctrldatastu.tempdata[ptemp->curchnl].last)>=MAXPULSENEG)
		{
			g_temp.invalid_sum += temp;
			g_temp.cninvalid ++;
		}
		else
		{
			g_temp.current_sum +=temp;
			g_temp.cnvalid ++;
		}

		g_temp.count++;     

		if(g_temp.count>=MAXTEMPCOUNT)				/*采样400次完成求平均*/
		{
			if (ChkRunFlag(INIF_DEBUG))
			{
				validheatvoltage();						/*多次求平均*/
				pulsetotemp();      					/*AD值转成温度值A610*/
			}
			else
			{
				validheatvoltage14snew();				/*使用有效值过滤平均*/
				pulsetotemp14s();      					/*AD值转成温度值*/
			}

		  	if (CHK_TEMP_PCT_CTRL(ptemp->mh->DATEMP_FUNC[ptemp->curchnl]))
			{
				if (!ChkSysCfg(XF2_RELCL))
					ptemp->st->DATEMP_REAL[ptemp->curchnl] = 0;
                else
                    ChkTempHighErr(ptemp, ptemp->curchnl);
			}

		  ptemp->st->DATEMP_VALUE[ptemp->curchnl] = ptemp->tempctrldatastu.tempdata[ptemp->curchnl].last;

		  g_temp.count = 0;
		  g_temp.current_sum = 0;
		  g_temp.invalid_sum = 0;
		  g_temp.cninvalid = 0;
		  g_temp.cnvalid = 0;	  
		  g_temp.fail_count = 0;
		  return(2);					
		}
		else
	  		return(1);								/*次采样未到*/
	}	
	else											/*读到数据无效*/
	{
		/*读取失败次数加一*/
		g_temp.fail_count ++;
		if(g_temp.fail_count > 100)
		{
			ptemp->st->DATEMP_VALUE[ptemp->curchnl] = 0;
			/*失败次数大于100次，直接警报777*/
			return 0;
		}
		else
		{
			/*忽略本次采样失败*/
			return(1);
		}
	}
}

/*室温校准*/
void AMBTempAdjMain()
{
	PTEMPCTRL pp = &m_temp;
	
	if (pp->st->WTEMP_LM73 != 8000)		//如为室温出错，则不进行校准
	{
		if (pp->st->WTEMP_ADJAMB == 500)		//清除室温的校正值   20190426
		{
			pp->zd->WTEMP_OFFSET = 0;
		}
		else
		{
			if (pp->st->WTEMP_ADJAMB > g_ambient.curambient)     //ctrl_tbl[WTEMP_LM73])
			{
				pp->zd->WTEMP_OFFSET =(WORD)(pp->st->WTEMP_ADJAMB - g_ambient.curambient);    //ctrl_tbl[WTEMP_LM73]);
			}
			else
			{
				pp->zd->WTEMP_OFFSET =(WORD)((g_ambient.curambient - pp->st->WTEMP_ADJAMB) | 0x8000);
			}
		}
		
		PartCfgSave((PPART)pp, ZD_ID);		
		/*同时保存到出厂数据*/
		PartCfgSaveFa((PPART)pp, ZD_ID);
	}
}


/*=================================================================================================
函数名：SurtempToUV()
功  能：室温值转成微伏及送上位机显示
说  明: ctrl_tbl[WTEMP_LM73]==800，此为出警告阀值，否则判断是否需转换标志(g_ambient.f_transform)
==================================================================================================*/
void SurtempToUV(void)
{	
	PTEMPCTRL pp = &m_temp;
	DWORD midvalue = 0;
	WORD wtemp1 = 0,wtemp2 = 0;

	if (g_ambient.f_transform)	   			//需要转换
	{
	/********************** 进行偏差值补偿校准 送上位机显示 ***************************/
		if ((WORD)(g_ambient.curambient & 0x8000) == 0)				  
		{
		    if (pp->zd->WTEMP_OFFSET & 0x8000)	 		//校准为负值   校准值比当时显示温度低
			{
				if (g_ambient.curambient >= (pp->zd->WTEMP_OFFSET & 0x7fff))
				{
					pp->st->WTEMP_LM73 = (WORD)(g_ambient.curambient - (pp->zd->WTEMP_OFFSET & 0x7fff));
				}
				else	 									//实际绝对值小于校准值
				{
					pp->st->WTEMP_LM73 = 	(WORD)(((pp->zd->WTEMP_OFFSET & 0x7fff) - g_ambient.curambient) | 0x8000);
				}
			}
			else											//校准为正值
			{
				pp->st->WTEMP_LM73 = (WORD)(g_ambient.curambient +pp->zd->WTEMP_OFFSET);	
			}
		}
		else
		{
		    if (pp->zd->WTEMP_OFFSET & 0x8000)	 		//校准为负值   校准值比当时显示温度低
			{
				pp->st->WTEMP_LM73 = (WORD)(((g_ambient.curambient & 0x7fff) + (pp->zd->WTEMP_OFFSET & 0x7fff)) | 0x8000);
			}
			else											//校准为正值
			{
				if ((g_ambient.curambient & 0x7fff) > pp->zd->WTEMP_OFFSET)
				{
				   pp->st->WTEMP_LM73 = (WORD)(((g_ambient.curambient & 0x7fff) - pp->zd->WTEMP_OFFSET) | 0x8000);	
				}
				else
				{
					pp->st->WTEMP_LM73 = (WORD)(pp->zd->WTEMP_OFFSET - (g_ambient.curambient & 0x7fff));	
				}
			}
		}
	
    	if ((WORD)(pp->st->WTEMP_LM73 & 0x8000) == 0)				  
    	{
    		wtemp1 = (WORD)(pp->st->WTEMP_LM73 / 50);			//LM73读到的温度值转换成微伏值表中对应微伏值.
    		wtemp2 = (WORD)(pp->st->WTEMP_LM73 % 50);
    		midvalue =(DWORD)(T_tab[wtemp1 + 1] - T_tab[wtemp1]);
    		pp->st->WTEMP_LM73UV = (WORD)(T_tab[wtemp1] + (WORD)(midvalue * wtemp2 / 50));
    	}
    	else
    	{
    		wtemp1 = (WORD)((pp->st->WTEMP_LM73 & 0x7FFF) / 50);
    		wtemp2 = (WORD)((pp->st->WTEMP_LM73 & 0x7FFF) % 50);
    		midvalue =(DWORD)(T_tabN[wtemp1 + 1] - T_tabN[wtemp1]);
    		pp->st->WTEMP_LM73UV = (WORD)(T_tabN[wtemp1] + (WORD)(midvalue * wtemp2 / 50));		  //负温度时，查负温度表，并把最高位置1.  WD20120228
    	  	pp->st->WTEMP_LM73UV = (WORD)(pp->st->WTEMP_LM73UV | 0x8000);
    	}
    }

	g_ambient.f_transform = 0;			
}

 /******************************************************************
  * @函数名称		LM73ConfigInterrupt
  * @函数说明   配置LM73，AD转换码：14位,中断方式,正常完成需要200ms左右，允许3次失败，即600ms,该函数用于
                LM73读取失败或异常后调用 zf 20160927
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
 *****************************************************************/
void LM73ConfigInterrupt(void)
{
	PTEMPCTRL pp = &m_temp;
	switch (g_lm73.Initstep)
	{
		case 0:
			if(ChkSysCfg(XF2_SDRTF))
			{
				g_lm73.Initstep = 3;
			}
			else
			{
				m_lm73_poweroff();                          			//给LM73断电 zf 20170623
				g_lm73.waittime = A_10msclock;
				g_lm73.Initstep++;
			}
			break;
		case 1:
			if(delt_time(A_10msclock,g_lm73.waittime ) > 10)			//延时，确保断电成功
			{
				m_lm73_poweron(); 	 											//给LM73上电  zf 20170623		
				g_lm73.waittime = A_10msclock;	
				g_lm73.Initstep++;
			}
			break;			
		case 2:																				//zf 20170623
			if(delt_time(A_10msclock,g_lm73.waittime ) > 10)			//上电后延迟100ms
			{
				g_lm73.waittime = A_10msclock;	
				g_lm73.Initstep++;	
			}			
			break;
			
		case 3:
			m_I2C_LM73_Init();												//I2C初始化
			g_lm73.waittime = A_1msclock;
			g_lm73.Initstep = 11;
		    break;
            
		case 11:						//结束
			g_lm73.failcount = 0;
			g_lm73.WRITE_SUCCESS = 0;
            pp->st->WTEMP_LM73 = 0;
			break;
            
		default:	
			g_lm73.failcount = 0;
			g_lm73.WRITE_SUCCESS = 0;
		 break;
	}
}


/*=================================================================
函数名：Get_Temp_LM73()
功  能： 读LM73,并转换成微伏值		精度为0.1摄氏度	 WD20120202
=================================================================*/
void Get_Temp_LM73()
{
    PTEMPCTRL pp = &m_temp;
    WORD  rtvalue;

    switch (g_ambient.readstep)
    {
        case 0:
            if (delt_time(A_10msclock, g_ambient.readtime) > 500)
            {
                g_ambient.readstep++;
                m_start_readlm73();
                g_ambient.readLM73Dlytm = A_10msclock;
            }
            break;
        case 1:
            if (delt_time(A_10msclock, g_ambient.readLM73Dlytm) > 20)
            {
// 				Start_ReadLM73();
                g_ambient.readstep++;
            }
            break;
        case 2:
            rtvalue = m_get_lm73value();
            if (rtvalue)  								//读到有效值,进行比较，决定是否有跳变
            {
                if (ABS_DIFF((g_ambient.curambient & 0x8000), (g_ambient.lastambient & 0x8000)) == 0)			//均为负值或均为正值
                {
                    if (ABS_DIFF((g_ambient.curambient & 0x7fff), (g_ambient.lastambient & 0x7fff)) <= 100)	//偏差不大于10度
                    {
                        g_ambient.lastambient = g_ambient.curambient;
                        g_ambient.f_transform = 1;
                        g_ambient.cninvalid = 0;  		//偏差故障次数清零，以免累加出错，只有当连续读到偏差，才警告.
                    }
                    else
                    {
                        g_ambient.curambient = g_ambient.lastambient;			//保留上次值
                        g_ambient.cninvalid++;
                    }
                }
                else
                {
                    if (((g_ambient.curambient & 0x7fff)+(g_ambient.lastambient & 0x7fff)) <= 100)	//偏差不大于10度
                    {
                        g_ambient.lastambient = g_ambient.curambient;
                        g_ambient.f_transform = 1;
                        g_ambient.cninvalid = 0;
                    }
                    else
                    {
                        g_ambient.curambient = g_ambient.lastambient;			//保留上次值
                        g_ambient.cninvalid++;
                    }
                }

                if (g_ambient.cninvalid >= 3)				
				{
					g_ambient.cninvalid = 0;
					g_ambient.readstep = 3;										//zf 20160927
					g_lm73.Initstep = 0;											//设置LM73初始化步骤	zf 20160927	
					g_lm73.failcount = 0;					
				}
				else
					g_ambient.readstep = 0;
				
				g_ambient.readtime = A_10msclock;								//读到有效值，启动新一轮读时序(延时5秒后开始读)
            }
            else  						//读不到值；I2C_end2不会来,重新进入延时扫描。
			{
				g_ambient.cninvalid++;
				I2C_STOPBit();
				g_ambient.readtime = A_10msclock;
				g_ambient.curambient = g_ambient.lastambient;				
				if (g_ambient.cninvalid >= 3)				
				{
					g_ambient.cninvalid = 0;
					g_ambient.readstep = 3;										//zf 20160927
					g_lm73.Initstep = 0;											//设置LM73初始化步骤	zf 20160927	
					g_lm73.failcount = 0;					
				}
				else
					g_ambient.readstep = 0;	  
			}
            break;

		case 3:																				//zf 20160927
			LM73ConfigInterrupt();											//I2C及LM73初始化
			if(delt_time(A_10msclock,g_ambient.readtime) > 200)
			{
				if (pp->st->WTEMP_LM73 == 8999)					//初始化未成功
				{
					g_ambient.readstep = 3;							//重新初始化							
				}
				else
				{
					g_ambient.readstep = 0;							//初始化成功,重新开始读室温	
				}
				g_ambient.readtime = A_10msclock;
				g_lm73.failcount = 0;											//zf 20160927	
				g_lm73.Initstep = 0;
			}
			break;	
			
        default:
            break;
    }
}

void Get_SurTemp()
{	
	PTEMPCTRL pp = &m_temp;
	WORD  rtvalue;	
	
	switch (g_ambient.base_readstep)
	{
		case 0:
			g_ambient.base_readstep++;
			g_ambient.readtime = A_10msclock;
			break;	
		case 1:
			if(delt_time(A_10msclock,g_ambient.readtime) > 5)
			{
				g_ambient.readtime = A_10msclock;
				rtvalue = m_read_I2C2();
				if (rtvalue)  								//读到有效值,进行比较，决定是否有跳变
				{
					if (ABS_DIFF((g_ambient.curambient & 0x8000), (g_ambient.lastambient & 0x8000))==0) 		/*均为负值或均为正值*/
					{
						 if (ABS_DIFF((g_ambient.curambient & 0x7fff), (g_ambient.lastambient & 0x7fff))<=30)	/*偏差不大于3度*/
						 {	
							g_ambient.lastambient = g_ambient.curambient;
							g_ambient.f_transform = 1;
							g_ambient.cnvalid++;
						 }
						 else
						{
							g_ambient.lastambient = g_ambient.curambient;			/*开机连读，不论对错，均以读到的值为比较基准值,与正常读有别*/
							g_ambient.cnvalid = 0;
						 }
					}
					else
					{
						if (((g_ambient.curambient & 0x7fff)+(g_ambient.lastambient & 0x7fff))<=30)	/*偏差不大于3度*/
						{
							g_ambient.lastambient = g_ambient.curambient;
							g_ambient.f_transform = 1;
							g_ambient.cnvalid++;
						}
						else
						{
							g_ambient.lastambient = g_ambient.curambient;			/*保留上次值*/
							g_ambient.cnvalid = 0;
						}
					}
					//add by rch 
					if(g_ambient.lastambient == 0)
						g_ambient.lastambient = g_ambient.curambient;	
					if (g_ambient.cnvalid >= 3)										/*有效读到3次，认为基准正确，开始正常操作*/
					{
						 g_ambient.f_getbase = 1;
						 g_ambient.readtime = A_10msclock;							/*正常读写起始时间置值*/
						 g_ambient.readstep = 0;
					}
				}
				else  						//读不到值；I2C_end2不会来,重新进入延时扫描。
				{
					g_ambient.cnvalid = 0;	  
				}
				
			}
			
			if ((delt_time(A_10msclock,g_ambient.starttime) >= 100) && (g_ambient.f_getbase == 0))	  		/*连续采样时间，在此期间得到有效值次数大于N次，认为建立基准完成*/
			{
				g_ambient.base_readstep++;
				g_ambient.readtime = A_10msclock;	
				g_ambient.failcount++;
				if(g_ambient.failcount == 6)
				{
					g_ambient.failcount = 0;
					g_ambient.base_readstep = 1;
					pp->st->WTEMP_LM73 = 8000;				/*超时，提示室温偏差*/
				}
			}
			break;
		case 2:																				//zf 20160927
			LM73ConfigInterrupt();											//I2C及LM73初始化
			g_ambient.cnvalid = 0;					/*需在手动去掉此警告时，把< g_ambient.starttime = A_10msclock；> 以便再次连读基准*/
			g_ambient.starttime = A_10msclock;		/*超时出警告后，重新开始计时循环*/
			if(delt_time(A_10msclock,g_ambient.readtime) > 200)
			{
				if (pp->st->WTEMP_LM73 == 8999)					//初始化未成功
				{
					g_ambient.base_readstep = 2;							//重新初始化							
				}
				else
				{
					g_ambient.base_readstep = 0;							//初始化成功,重新开始读室温	
				}
				g_ambient.readtime = A_10msclock;
				g_lm73.failcount = 0;											//zf 20160927	
				g_lm73.Initstep = 0;
			}
			break;			
		default:
			break;
	}
}

/*===================================================
 函数名：Read_surtemp()
 功  能:读室温,2秒钟读一次，温度差值不应该大于2度，否则视为无效
 入口参数：
 出口参数：
=====================================================*/
void Read_surtemp(void)
{
   	PTEMPCTRL pp = &m_temp;

    if((GetHwModel() == MODEL_I3201) || (GetHwModel() == MODEL_I3202) || (GetHwModel() == MODEL_I5201)) //按照机型安排是否需要读取室温
	{
        return;
    }
    
    if (pp->fl_first == 0)
    {
        pp->fl_first = 1;
        g_ambient.starttime = A_10msclock;        
    }

	//开机时延迟200ms后再开始读室温。立刻读会出现，主机配置更新后，主机与面板通讯出现请求异常的问题。
	if (pp->fl_first == 1)
	{
		if (delt_time(A_10msclock, g_ambient.starttime) > 20)
		{
			pp->fl_first = 2;
		}
		return;
	}

    if (g_ambient.f_getbase == 0)  										/*无基准或重读基准（按手动键）*/
    {
        Get_SurTemp();
    }
    else 											/*正常间隔5秒读写，有基准，读到的值与基准比较，跳变大于5度认为故障*/
    {
        if (ComIsOk() || ChkRunFlag(INIF_RESET))			/*紧停或通讯正常情况下读室温，否则不读，保持上次值*/
        {
            Get_Temp_LM73();
        }
    }    
	SurtempToUV();		/*室温转换成微伏值及送显示*/
}


UI32 TempCtrlCmd(UI32 cmd, UI32 para)
{
    BOOL ret = TRUE;

    switch (GET_CMD(cmd))
    {
        case CMD_DATA_RX:       
            break;

        default:
            ret = FALSE;
            break;
    }

    return ret;
}

BOOL InitTempCtrlPart()
{
    static const SUB_INIT_INFO sub_init[] = TEMPCTRL_INIT_INFO;
    PTEMPCTRL	pp = &m_temp;

    if (!INIT_PART(pp, TEMPCTRL_ID, 1, sub_init))
    {
        return FALSE;
    }

    g_ambient.curambient = 0;		 //当前室温
    g_ambient.lastambient = 0;	 //上次室温
    g_ambient.cnvalid	 = 0;			 //室温读到有效次数为零
    g_ambient.cninvalid = 0;
    g_ambient.readstep = 0;
    g_ambient.f_getbase = 0;					//室温基准置为零
    g_ambient.f_transform = 0;				//转换初始化（无需转换）
    g_ambient.starttime = Get10MsTick();		//基准读取起始时间
    g_ambient.readtime = Get10MsTick();
    g_ambient.agetime = Get10MsTick();	   		//老练时间判断

    return TRUE;
}
