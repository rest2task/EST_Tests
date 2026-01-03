/******************************************************************************
  文 件 名   : TempCtrlMw.c
  版 本 号   : 初稿
  作    者   :yxh
  生成日期   : 2019年12月26日
  最近修改   :
  功能描述   : 模温部件(E501)
  函数列表   :
******************************************************************************/
#include "canhead.h"
#include "can_e502.h"
#include "systempro.h"
#include "machapi.h"
#include "alarm.h"
#include "cfgdb.h"
#include "TempCtrlMw.h"
#include "can_e702.h"

PTEMPCTRLMW	g_temp_mw[TEMPMW_MAX_NUM] = {NULL};
extern DB_TEMPMOLD g_cantempe501[TEMPMW_MAX_NUM];
BOOL can_node_E501_check[TEMPMW_MAX_NUM] = {0};  //E501温度扩展板标记，用于主机给温度扩展板是否已经发送握手命令的标记  20200116
UI16 g_tempmw_num = 0;			//该变量代表配置工具配置的模温板数目
UI16 mw_sendtommi_step = 0;	//模温时时状态值发送给面板的动作步
UI16 mw_sendtommi_tm = 0;	//模温时时状态值发送给面板的时间初始值
UI16 mw_sendtommi_num = 0;	//模温时时状态值发送给面板的当前模温板号
UI16 sendnum = 0;	//模温板当前心跳发送的版本编码
UI16 totalnum = 0;	//模温板实际连接成功的总数目，代表的就是连接成功的模温数目
UI16 mw_piece = 0;	//每一块模温板在线状态，按位操作，第一位为1代表第一块模温板在线，以此类推
UI16 mw_hearttm = 0;	//每一块模温板的心跳发送间隔时间
UI16 mw_scantm = 0;		//模温的心跳扫描时间初始化

BOOL TempMwIsHoldMoldErrChk()
{
	UI16 i = 0;
	PTEMPCTRLMW pp;

	for(i=0;i<g_tempmw_num;i++)
	{
		pp = g_temp_mw[i];
		
		if(pp == NULL)
		{
		//如果指针空，则查看下一个部件地址
			continue;
		}
        
		if(pp->md->FL_USE_MW == 1 && pp->md->FL_TEMPHOLD_MW == 1)
	    {
	        SET_PART_ERR(pp, ER0_TMPHLD_MW);
	        return TRUE;
	    }
	}   

    return FALSE;
}

void	Temp_mold_buf(UI16 num,PTEMPCTRLMW pp)
{
	if(pp == NULL)
		return;

	if(pp->md->DATEMP_NUM_MW < 8 || pp->md->DATEMP_NUM_MW > 16 )
		pp->mw_segments = 8;
	else
		pp->mw_segments = pp->md->DATEMP_NUM_MW;
}

void Temp_Last_Record(PTEMPCTRLMW pp,UI16	ext_board_id,UI16 comd,UI16 num,UI16 data)		//温度的设定参数记录，用于无反馈时的重发
{
	pp->mw_buf[pp->mw_num].ext_board_last = ext_board_id;
	pp->mw_buf[pp->mw_num].ext_command = comd;
	pp->mw_buf[pp->mw_num].ext_num_set = num;
	pp->mw_buf[pp->mw_num].ext_data_set = data;
	pp->mw_buf[pp->mw_num].ext_begin_tm = Get10MsTick();
	pp->mw_buf[pp->mw_num].ext_begin_flag = TRUE;
	pp->mw_buf[pp->mw_num].ext_begin_cnt = 0;

	pp->mw_num++;
	if(pp->mw_num > (MWNUM-1))
		pp->mw_num = 0;
}

void Mw_oncom(PTEMPCTRLMW pp,UI16 i)
{
	if(pp == NULL)
		return;

	pp->st->FL_HEATERSTATUS_MW = TRUE;
	pp->mw_heat_statue = 1;
	pp->mw_heat_starttm = Get10MsTick();
}

void Mw_offcom(PTEMPCTRLMW pp,UI16 i)
{
	if(pp == NULL)
		return;

	pp->st->FL_HEATERSTATUS_MW = FALSE;
	pp->mw_heat_statue = 1;
	pp->mw_heat_starttm = Get10MsTick();
}


void Mwon(void)
{
  	int i;
	
  	if (g_tempmw_num > 0)
    {
        for (i = 0; i < g_tempmw_num; i++)
        {
        	if(g_temp_mw[i] != NULL)
           		Mw_oncom(g_temp_mw[i],i);
        }
    }
}

void Mwoff(void)
{
	int i;

	if (g_tempmw_num > 0)
	{
	    for (i = 0; i < g_tempmw_num; i++)
	    {
	    	if(g_temp_mw[i] != NULL)
				Mw_offcom(g_temp_mw[i],i);          
	    } 
	}
}


void Mw_send_al(PTEMPCTRLMW pp,UI16 i)
{
	if(pp == NULL)
		return;
	//电热开关分时发送
	if((pp->mw_heat_statue == 1) && ((WORD)(Get10MsTick() - pp->mw_heat_starttm) > (MW_HEATONOFF *i)))
	{
		if(pp->md->FL_USE_MW == 1)
		{
			CanExtTempMoldWrite(i+1,TEMP_SWITCH,0,pp->st->FL_HEATERSTATUS_MW);
			Temp_Last_Record(pp,i+1,TEMP_SWITCH,0,pp->st->FL_HEATERSTATUS_MW);
		}
		pp->mw_heat_statue = 0;
	}
	//手动按键分时发送
	if((pp->mw_maul_statue == 1) && ((WORD)(GetMsTick() - pp->mw_maul_starttm) > (MW_MANULE *i)))
	{
		if(pp->md->FL_USE_MW == 1)
		{
			CanExtTempMoldWrite(i+1,TEMP_MAUL,0,0);
			Temp_Last_Record(pp,i+1,TEMP_MAUL,0,0);
		}
		pp->mw_maul_statue = 0;
	}

	//温度一键优化命令分时发送
	if((pp->mw_yh_begin == 1) && ((WORD)(Get10MsTick() - pp->mw_yh_tm) > (MW_MAYHTM * i)))
	{
		if(pp->md->FL_USE_MW == 1)
		{
			CanExtTempMoldWrite(i+1,TEMP_YH,0,0);
			Temp_Last_Record(pp,i+1,TEMP_YH,0,0);
		}
		pp->mw_yh_begin = 0;
	}
	else if((pp->mw_yh_begin == 2) && ((WORD)(Get10MsTick() - pp->mw_yh_tm) > (MW_MAYHTM * i)))
	{
		if(pp->md->FL_USE_MW == 1)
		{
			CanExtTempMoldWrite(i+1,TEMP_YHQZ,0,0);
			Temp_Last_Record(pp,i+1,TEMP_YHQZ,0,0);
		}
		pp->mw_yh_begin = 0;
	}
}

void Mw_send(void)		//模温加热开启与关闭，手动按键命令，一键优化命令发送
{
	int i;
	
  	if (g_tempmw_num > 0)
    {
        for (i = 0; i < g_tempmw_num; i++)
        {
        	if(g_temp_mw[i] != NULL)
        	{
        		Mw_send_al(g_temp_mw[i],i);
        	}
        }
    }
}

void Temp_mwRFStartPro(WORD password,WORD id,WORD ch)
{
	int i = 0;

	for(i = 0;i < TEMPMW_MAX_NUM;i++)
	{
		if((id & (0x01 << i)) > 0)
		{
			if(i >=g_tempmw_num)
			{//当传入id中部件号大于配置id数时，停止
				break;
			}
			
			PTEMPCTRLMW ptempmw = g_temp_mw[i];

            if(password == FORCERFPASSWORD)
			{	
				if(ptempmw != NULL)
				{
					ptempmw->mw_yh_begin = 2;
					ptempmw->mw_yh_tm = Get10MsTick();
				}
			}
			else
			{
				if(ptempmw != NULL)
				{
					ptempmw->mw_yh_begin = 1;
					ptempmw->mw_yh_tm = Get10MsTick();
				}
			}            
            
		}
	}		
}

void Temp_timestate_Pro(void)	//模温时时状态值主动上传
{
  	if (g_tempmw_num > 0 && ComIsOk())
    {
		switch(mw_sendtommi_step)
		{
			case 0:
				mw_sendtommi_step = 1;
				mw_sendtommi_tm = Get10MsTick();
				mw_sendtommi_num = 0;
				break;

			case 1:
				mw_sendtommi_step = 2;
				mw_sendtommi_tm = Get10MsTick();
				break;

			case 2:
				if((WORD)(Get10MsTick() - mw_sendtommi_tm) > 18)
				{
					PTEMPCTRLMW ptempmw = g_temp_mw[mw_sendtommi_num];
					if(ptempmw != NULL)
					{
						if((ptempmw->md->FL_USE_MW == 1) || (ptempmw->mw_close_flag == 1))
							PartSubTx((PPART)ptempmw, SUB_BIT(ST_ID), FALSE);
						mw_sendtommi_step = 1;
						mw_sendtommi_tm = Get10MsTick();
						
						if(mw_sendtommi_num < g_tempmw_num)
							mw_sendtommi_num++;
						else
							mw_sendtommi_num = 0;
					}
					else		//发现某一块为空时，后面的就不再扫描并且从第一块开启重新启动下一轮的上传
					{
						mw_sendtommi_step = 0;
						mw_sendtommi_num = 0;
						mw_sendtommi_tm = Get10MsTick();
					}
				}
				break;
				
			default:
				break;
		}	
    }
}


BOOL GetMwStatus(void)
{
	int i = 0,flag = 0;
	if(g_tempmw_num > 0)
	{
		for(i=0;i<g_tempmw_num;i++)
		{
			if(g_temp_mw[i]->st->FL_HEATERSTATUS_MW == 1)	//只要有任意一块模温板是开启电热的，则认为当前电热是打开的
				flag = 1;
		}

		return flag;
	}
	
	return FALSE;
}

void  Can_e501_test(PTEMPCTRLMW pp)
{
	if(ChkOperMode(OM_AUTO) || (pp == NULL))
	{
		return;
	}
	
	switch (pp->can_e501_writestep)
	{
		case 0:  
			CanExtTempMoldWrite(GET_PART_SN(pp->part.id),TEMP_TEST,0,1);
			pp->can_e501_writetm = GetMsTick();
			pp->st->WRITE_CNT_E501=1;
			pp->can_e501_writestep ++;
			break;	  
		case 1:
			if ((WORD)(GetMsTick() - pp->can_e501_writetm)>=100)
			{
				CanExtTempMoldWrite(GET_PART_SN(pp->part.id),TEMP_TEST,0,1);
				pp->can_e501_writetm = GetMsTick();
				pp->st->WRITE_CNT_E501++;
			}
			break;

			default:
				break;
	}
}


void	Can_Test(void)
{
	int i;
	
	if(g_tempmw_num > 0)
	{
		for (i = 0; i < g_tempmw_num; i++)
	    {
	    	if(g_temp_mw[i] != NULL && (g_temp_mw[i]->can_e501_testmode == 1))
				Can_e501_test(g_temp_mw[i]);          
	    } 
	}
	if(GetSys()->can_e700b_testmode == 1)
	{
		Can_E700B_test();
	}
}

void    TempMoldSetData(PTEMPCTRLMW pp,UI16 i)
{
	if(pp == NULL)
		return;


	switch(pp->mw_put_step)
	{
		case 0:
			if ((g_tempmw_num > 0) && (i < g_tempmw_num))
			{
				g_temp_mw[i]->mh->TEMP_CONTROL_MW = 1;
				if(g_temp_mw[i]->mh->TEMP_CONTROL_MW == 1)
				{
				    Temp_mold_buf(i,g_temp_mw[i]);
					pp->mw_put_step++;
					pp->mw_put_time = GetMsTick();
					pp->mw_sendnum = 0;
				}
				else
					pp->mw_put_step = 100;
			}
			else
				pp->mw_put_step = 100;	//当前检测到的模温编号，大于主机配置的最大模温数时，直接结束
			break;

		case 1:
			if((UI16)(GetMsTick()-pp->mw_put_time) >= 15)	//此处握手成功后，等待15ms，保证其他的模温板都能发送握手成功
			{
				pp->mw_put_step++;
				pp->mw_put_time = GetMsTick();
				pp->mw_sendnum = 0;
			}
			break;
			
		case 2:
			if((UI16)(GetMsTick()-pp->mw_put_time) >= MW_SENDTM)
			{
				pp->mw_put_time = GetMsTick();
				CanExtTempMoldWrite(i+1,TEMP_DATA,pp->mw_sendnum,pp->md->DATEMP_SETTING_MW[pp->mw_sendnum]);				
				pp->mw_sendnum++;
				if(pp->mw_sendnum >= pp->mw_segments)
				{
					pp->mw_sendnum = 0;
					pp->mw_put_step++;
				}
			}
			break;

		case 3:
			if((UI16)(GetMsTick()-pp->mw_put_time) >= MW_SENDTM)
			{
				pp->mw_put_time = GetMsTick();
				CanExtTempMoldWrite(i+1,TEMP_FUN,pp->mw_sendnum,pp->md->DATEMP_FUNC_MW[pp->mw_sendnum]);
				pp->mw_sendnum++;
				if(pp->mw_sendnum >= pp->mw_segments)
				{
					pp->mw_sendnum = 0;
					pp->mw_put_step++;
				}
			}
			break;

		case 4:
			if((UI16)(GetMsTick()-pp->mw_put_time) >= MW_SENDTM)
			{
				pp->mw_put_time = GetMsTick();
				CanExtTempMoldWrite(i+1,TEMP_SET_P,pp->mw_sendnum,pp->md->DAPID_KP_MW[pp->mw_sendnum]);
				pp->mw_sendnum++;
				if(pp->mw_sendnum >= pp->mw_segments)
				{
					pp->mw_sendnum = 0;
					pp->mw_put_step++;
				}
			}
			break;

		case 5:
			if((UI16)(GetMsTick()-pp->mw_put_time) >= MW_SENDTM)
			{
				pp->mw_put_time = GetMsTick();
				CanExtTempMoldWrite(i+1,TEMP_SET_I,pp->mw_sendnum,pp->md->DAPID_TI_MW[pp->mw_sendnum]);
				pp->mw_sendnum++;
				if(pp->mw_sendnum >= pp->mw_segments)
				{
					pp->mw_sendnum = 0;
					pp->mw_put_step++;
				}
			}
			break;

		case 6:
			if((UI16)(GetMsTick()-pp->mw_put_time) >= MW_SENDTM)
			{
				pp->mw_put_time = GetMsTick();
				CanExtTempMoldWrite(i+1,TEMP_SET_D,pp->mw_sendnum,pp->md->DAPID_TD_MW[pp->mw_sendnum]);
				pp->mw_sendnum++;
				if(pp->mw_sendnum >= pp->mw_segments)
				{
					pp->mw_sendnum = 0;
					pp->mw_put_step++;
				}
			}
			break;

		case 7:
			if((UI16)(GetMsTick()-pp->mw_put_time) >= MW_SENDTM)
			{
				pp->mw_put_time = GetMsTick();
				CanExtTempMoldWrite(i+1,TEMP_RAMP,pp->mw_sendnum,pp->md->DATEMP_RAMP_MW[pp->mw_sendnum]);
				pp->mw_sendnum++;
				if(pp->mw_sendnum >= pp->mw_segments)
				{
					pp->mw_sendnum = 0;
					pp->mw_put_step++;
				}
			}
			break;

		case 8:
			if((UI16)(GetMsTick()-pp->mw_put_time) >= MW_SENDTM)
			{
				pp->mw_put_time = GetMsTick();
				CanExtTempMoldWrite(i+1,TEMP_TIME,pp->mw_sendnum,pp->md->TM_RESPONSELESS_MW[pp->mw_sendnum]);
				pp->mw_sendnum++;
				if(pp->mw_sendnum >= pp->mw_segments)
				{
					pp->mw_sendnum = 0;
					pp->mw_put_step++;
				}
			}
			break;

		case 9:
			if((UI16)(GetMsTick()-pp->mw_put_time) >= MW_SENDTM)
			{
				pp->mw_put_time = GetMsTick();
				CanExtTempMoldWrite(i+1,TEMP_DIVH,pp->mw_sendnum,pp->md->DATEMP_MAX_MW[pp->mw_sendnum]);
				pp->mw_sendnum++;
				if(pp->mw_sendnum >= pp->mw_segments)
				{
					pp->mw_sendnum = 0;
					pp->mw_put_step++;
				}
			}
			break;

		case 10:
			if((UI16)(GetMsTick()-pp->mw_put_time) >= MW_SENDTM)
			{
				pp->mw_put_time = GetMsTick();
				CanExtTempMoldWrite(i+1,TEMP_DIVL,pp->mw_sendnum,pp->md->DATEMP_MIN_MW[pp->mw_sendnum]);
				pp->mw_sendnum++;
				if(pp->mw_sendnum >= pp->mw_segments)
				{
					pp->mw_sendnum = 0;
					pp->mw_put_step++;
				}
			}
			break;

		case 11:
			if((UI16)(GetMsTick()-pp->mw_put_time) >= MW_SENDTM)
			{
				pp->mw_put_time = GetMsTick();
				CanExtTempMoldWrite(i+1,TEMP_JKMD,pp->mw_sendnum,pp->md->FL_TKTJ_MW[pp->mw_sendnum]);
				pp->mw_sendnum++;
				if(pp->mw_sendnum >= pp->mw_segments)
				{
					pp->mw_sendnum = 0;
					pp->mw_put_step++;
				}
			}
			break;

		case 12:
			if((UI16)(GetMsTick()-pp->mw_put_time) >= MW_SENDTM)
			{
				pp->mw_put_time = GetMsTick();
				CanExtTempMoldWrite(i+1,TEMP_BL,pp->mw_sendnum,pp->md->DACENT_HEATERON_MW[pp->mw_sendnum]);
				pp->mw_sendnum++;
				if(pp->mw_sendnum >= pp->mw_segments)
				{
					pp->mw_sendnum = 0;
					pp->mw_put_step++;
				}
			}
			break;

		case 13:
			if((UI16)(GetMsTick()-pp->mw_put_time) >= MW_SENDTM)
			{
				pp->mw_put_time = GetMsTick();
				CanExtTempMoldWrite(i+1,TEMP_KEEP,0,pp->md->FL_TEMPHOLD_MW);
				pp->mw_put_step++;
				pp->mw_put_time = GetMsTick();
			}
			break;

		case 14:
			if((UI16)(GetMsTick()-pp->mw_put_time) >= MW_SENDTM)
			{
				pp->mw_put_time = GetMsTick();
				CanExtTempMoldWrite(i+1,TEMP_WARM,0,pp->md->DATEMP_HOLD_MW);
				pp->mw_put_step++;
				pp->mw_put_time = GetMsTick();
			}
			break;

		case 15:
			if((UI16)(GetMsTick()-pp->mw_put_time) >= MW_SENDTM)
			{
				pp->mw_put_time = GetMsTick();
				CanExtTempMoldWrite(i+1,TEMP_CYCL,0,pp->md->DATEMP_SCTM_MIN_MW);
				pp->mw_put_step++;
				pp->mw_put_time = GetMsTick();
			}
			break;

		case 16:
			if((UI16)(GetMsTick()-pp->mw_put_time) >= MW_SENDTM)
			{
				pp->mw_put_time = GetMsTick();
				CanExtTempMoldWrite(i+1,TEMP_DETE,0,pp->md->FL_TEMPUPCHECK_MW);
				pp->mw_put_step++;
				pp->mw_put_time = GetMsTick();
			}
			break;

		case 17:
			if((UI16)(GetMsTick()-pp->mw_put_time) >= MW_SENDTM)
			{
				pp->mw_put_time = GetMsTick();
				CanExtTempMoldWrite(i+1,TEMP_NUM,0,pp->md->DATEMP_NUM_MW);
				pp->mw_put_step++;
				pp->mw_put_time = GetMsTick();
			}
			break;
			
		case 18:
			if((UI16)(GetMsTick()-pp->mw_put_time) >= MW_SENDTM)
			{
				pp->mw_put_time = GetMsTick();
				CanExtTempMoldWrite(i+1,TEMP_YWT,0,pp->md->TM_HEATERWARM_MW);
				pp->mw_put_step++;
				pp->mw_put_time = GetMsTick();
			}
			break;

		case 19:
			if((UI16)(GetMsTick()-pp->mw_put_time) >= MW_SENDTM)
			{
				pp->mw_put_time = GetMsTick();
				CanExtTempMoldWrite(i+1,TEMP_USE,0,pp->md->FL_USE_MW);
				pp->mw_put_step++;
				pp->mw_put_time = GetMsTick();
			}
			break;

		case 20:
			if((UI16)(GetMsTick()-pp->mw_put_time) >= MW_SENDTM)
			{
				pp->mw_put_time = GetMsTick();
				CanExtTempMoldWrite(i+1,TEMP_TOBU,0,pp->md->CONFUNC_MW);
				pp->mw_put_step = 100;
				pp->mw_put_time = GetMsTick();
			}
			break;

		case 100:
			g_cantempe501[i+1].TempMold_node_state = 0;
			g_cantempe501[i+1].node_gard_time = Get10MsTick();
			break;

		default :
			pp->mw_put_step = 0;
			break;
	}
}

void    TempdataReset(PTEMPCTRLMW pp,UI16 n)		//模温扩展板设定数据重发机制，用于给扩展板发送数据后却无回复采用重发3次的机制
{
	WORD ext_board_id_a = 0;
	UI16 com_a = 0,num_a = 0,data_a = 0;

	if(pp == NULL)
		return;

	ext_board_id_a = pp->mw_buf[n].ext_board_last;
	com_a = pp->mw_buf[n].ext_command;
	num_a = pp->mw_buf[n].ext_num_set;
	data_a = pp->mw_buf[n].ext_data_set;
	CanExtTempMoldWrite(ext_board_id_a,com_a,num_a, data_a);		//启动参数重发
	
	pp->mw_buf[n].ext_begin_cnt++;		//重发次数加1
	pp->mw_buf[n].ext_begin_tm = Get10MsTick();		//重发时间重新初始化
}

void Dataresend(void)
{
	int i,n;
	for (i=0; i<g_tempmw_num; i++ )
	{
		PTEMPCTRLMW ptempmw = g_temp_mw[i];
		if (can_node_E501_check[i+1] == TRUE)
		{
			for(n=0;n<MWNUM;n++)
			{
				if(ptempmw != NULL)
				{
					if(ptempmw->mw_buf[n].ext_begin_flag == TRUE && (ptempmw->md->FL_USE_MW == 1)
						&& ((UI16)(Get10MsTick() - ptempmw->mw_buf[n].ext_begin_tm) > 5))
					{
						if(ptempmw->mw_buf[n].ext_begin_cnt < 3)
							TempdataReset(ptempmw,n);
						else
						{
							ptempmw->mw_buf[n].ext_begin_flag = FALSE;
							ptempmw->mw_buf[n].ext_begin_cnt = 0;
							SET_PART_ERR(ptempmw,ER0_DATA_FAILED);
						}
					}
				}
			}
		}		
	}
}

void Mwstatue_clear_al(PTEMPCTRLMW pp,UI16 i)
{
	int j;
	
	if(pp == NULL)
		return;

	if(i<g_tempmw_num)
	{
		if((can_node_E501_check[i+1] == FALSE) || (pp->md->FL_USE_MW == 0))
		{
			pp->st->FL_ERROR0 = pp->st->FL_ERROR0&0x0C04;   //不清除模温保温警报
			pp->st->FL_ERROR1 = 0;
			for(j=0;j<pp->md->DATEMP_NUM_MW;j++)
			{
				pp->st->DATEMP_REAL_MW[j] = 0;
				pp->st->FL_STATUS_MW[j] = 0;
				pp->st->OUTPUT_PER_MW[j] = 0;
			}
			pp->st->FL_HEATERSTATUS_MW = 0;

			pp->st->WTEMP_LM73_MW = 0;
			pp->st->DATEMP_VER_MW = 0;
		}

		for(j=0;j<pp->md->DATEMP_NUM_MW;j++)
		{
			if(pp->md->DATEMP_FUNC_MW[j] == 0)
			{
				pp->st->DATEMP_REAL_MW[j] = 0;
				pp->st->FL_STATUS_MW[j] = 0;
				pp->st->OUTPUT_PER_MW[j] = 0;
			}
		}		

		if(pp->mw_close_flag == 1 && (UI16)(Get10MsTick() - pp->mw_close_tm) >= 200)
		{
			pp->mw_close_flag = 0;
		}
	}
}

void Mwstatue_clear_all(void)
{
	int i;
	
  	if (g_tempmw_num > 0)
    {
        for (i = 0; i < g_tempmw_num; i++)
        {
        	if(g_temp_mw[i] != NULL)
        	{
        		Mwstatue_clear_al(g_temp_mw[i],i);
        	}
        }
    }
}

BOOL Temp_ErrInj_one(PTEMPCTRLMW pp,UI16 i)		//射出时温度按照一块一块轮询检测
{
	int j;
	
	if(pp == NULL)
		return FALSE;

	if(i < g_tempmw_num)
	{
		if ((pp->md->FL_USE_MW == 1) && (can_node_E501_check[i+1] == TRUE))
		{
			for(j = 0;j<pp->md->DATEMP_NUM_MW;j++)
			{
				if((pp->md->DATEMP_FUNC_MW[j] > 2) && (g_temp_mw[0]->md->FL_INJ_ALLOW == 1))
				{
					if(((pp->md->DATEMP_SETTING_MW[j] - pp->md->DATEMP_MIN_MW[j]) > pp->st->DATEMP_REAL_MW[j])
                        || ((pp->md->DATEMP_SETTING_MW[j] + pp->md->DATEMP_MAX_MW[j]) < pp->st->DATEMP_REAL_MW[j]))
					{
						if(!CHK_PART_ERR(pp, ER1_TEMPER_MW))
						{
							SET_PART_ERR(pp, ER1_TEMPER_MW);
							return TRUE;
						}
					}
				}
			}
		}

		return FALSE;
	}
	
	return FALSE;
}


BOOL Temp_ErrInj_all(void)		//射出时检测模温偏差功能
{
	int i;
	BOOL flag = 0;

  	if (g_tempmw_num > 0)
    {
        for (i = 0; i < g_tempmw_num; i++)
        {
        	if(g_temp_mw[i] != NULL)
        	{
        		flag = flag | Temp_ErrInj_one(g_temp_mw[i],i);
        	}
        }
    }

	return flag;
}

void TempErrCheck_one(PTEMPCTRLMW pp,UI16 i)	//模温单块时时检测，超温时报警并切断模温加热
{
	int j;
	
	if(pp == NULL)
		return;

	if(i < g_tempmw_num)
	{
		if ((pp->md->FL_USE_MW == 1) && (can_node_E501_check[i+1] == TRUE))
		{
			for (j = 0;j<pp->md->DATEMP_NUM_MW;j++)
			{
				if (pp->md->DATEMP_FUNC_MW[j] > 2)
				{
				    if (pp->md->FL_TEMPHOLD_MW == 0)
                    {
                        if (pp->st->DATEMP_REAL_MW[j] - pp->md->DATEMP_SETTING_MW[j] > pp->md->DATEMP_MAX_MW[j])
    					{
    						if (!CHK_PART_ERR(pp, ER1_TEMPER_MW))
    							SET_PART_ERR(pp, ER1_TEMPER_MW);
    						if (GetMwStatus())
    				  			Mwoff();
    					}
                    }   
					
				}
			}
		}		
	}
}

void TempErrCheck_all(void)		//模温温度偏差检查
{
	int i;
	
  	if (g_tempmw_num > 0)
    {
        for (i = 0; i < g_tempmw_num; i++)
        {
        	if(g_temp_mw[i] != NULL)
        	{
        		TempErrCheck_one(g_temp_mw[i],i);
        	}
        }
    }
}
void TempMwcheck(void)		//模温板各个数据检查，以及需要放在时时运行中的
{
	Can_Test();			//进入can通讯测试模式
	Dataresend();		//模温扩展板设定参数数据重发机制
	Mw_send();			//模温电热开启和关闭,手动按键命令，一键优化命令分时发送机制
	Temp_timestate_Pro();	//模温的实时状态数据上传
	Mwstatue_clear_all();		//模温关闭或者通信断开时，状态数据等清零
	TempErrCheck_all();		//模温当前温度超温时时检测功能
}


UI32 TempMwCmd(PTEMPCTRLMW pp, UI32 cmd, UI32 para)
{
    //int	i;
    BOOL ret = TRUE;
	UI16 offset,len;
	WORD ext_board_id,j;
	UI16 comd = 0,num = 0,data = 0;		//存放临时的命令字，当前为第几段温度，当前这一段的温度值

    switch (GET_CMD(cmd))
    {
        case CMD_DATA_RX:
			if(pp->md != NULL)
            {
            	offset=((para>>16)&0xFFFF);
				len = (para & 0xffff);
				ext_board_id = GET_PART_SN(pp->part.id);	//得到当前值为第几块模温扩展板
				if((len == 1)&&((GET_CMD_PARA(cmd) & 0xff) == MD_ID) && (offset <= 0x00B9) && ComIsOk())
				{
					if(offset <= 0x000F)  
					{
						j = (offset-0x0000)%TEMP_MW_CH_NUM;      //得到当前值为当前模温扩展板的第几段设定值
						CanExtTempMoldWrite(ext_board_id,TEMP_DATA,j, *(pp->md->DATEMP_SETTING_MW+(offset-0x0000)));
						comd = TEMP_DATA;
						num = j;
						data = *(pp->md->DATEMP_SETTING_MW+(offset-0x0000));
					}
					else if(offset>= 0x0010 && offset <= 0x001F)
					{      
						j = (offset-0x0010)%TEMP_MW_CH_NUM;   
						CanExtTempMoldWrite(ext_board_id,TEMP_FUN,j, *(pp->md->DATEMP_FUNC_MW+(offset-0x0010)));
						comd = TEMP_FUN;
						num = j;
						data = *(pp->md->DATEMP_FUNC_MW+(offset-0x0010));
					}
					else if(offset>= 0x0020 && offset <= 0x002F)
					{
						j = (offset-0x0020)%TEMP_MW_CH_NUM;   
						CanExtTempMoldWrite(ext_board_id,TEMP_SET_P,j, *(pp->md->DAPID_KP_MW+(offset-0x0020)));
						comd = TEMP_SET_P;
						num = j;
						data = *(pp->md->DAPID_KP_MW+(offset-0x0020));
					}
					else if(offset>= 0x0030 && offset <= 0x003F)
					{
						j = (offset-0x0030)%TEMP_MW_CH_NUM;   
						CanExtTempMoldWrite(ext_board_id,TEMP_SET_I,j, *(pp->md->DAPID_TI_MW+(offset-0x0030)));
						comd = TEMP_SET_I;
						num = j;
						data = *(pp->md->DAPID_TI_MW+(offset-0x0030));
					}
					else if(offset>= 0x0040 && offset <= 0x004F)
					{
						j = (offset-0x0040)%TEMP_MW_CH_NUM;   
						CanExtTempMoldWrite(ext_board_id,TEMP_SET_D,j, *(pp->md->DAPID_TD_MW+(offset-0x0040)));
						comd = TEMP_SET_D;
						num = j;
						data = *(pp->md->DAPID_TD_MW+(offset-0x0040));
					}
					else if(offset>= 0x0050 && offset <= 0x005F)
					{
						j = (offset-0x0050)%TEMP_MW_CH_NUM;   
						CanExtTempMoldWrite(ext_board_id,TEMP_RAMP,j, *(pp->md->DATEMP_RAMP_MW+(offset-0x0050)));
						comd = TEMP_RAMP;
						num = j;
						data = *(pp->md->DATEMP_RAMP_MW+(offset-0x0050));
					}
					else if(offset>= 0x0060 && offset <= 0x006F)
					{
						j = (offset-0x0060)%TEMP_MW_CH_NUM;   
						CanExtTempMoldWrite(ext_board_id,TEMP_TIME,j, *(pp->md->TM_RESPONSELESS_MW+(offset-0x0060)));
						comd = TEMP_TIME;
						num = j;
						data = *(pp->md->TM_RESPONSELESS_MW+(offset-0x0060));
					}
					else if(offset>= 0x0070 && offset <= 0x007F)
					{
						j = (offset-0x0070)%TEMP_MW_CH_NUM;   
						CanExtTempMoldWrite(ext_board_id,TEMP_DIVH,j, *(pp->md->DATEMP_MAX_MW+(offset-0x0070)));
						comd = TEMP_DIVH;
						num = j;
						data = *(pp->md->DATEMP_MAX_MW+(offset-0x0070));
					}
					else if(offset>= 0x0080 && offset <= 0x008F)
					{
						j = (offset-0x0080)%TEMP_MW_CH_NUM;   
						CanExtTempMoldWrite(ext_board_id,TEMP_DIVL,j, *(pp->md->DATEMP_MIN_MW+(offset-0x0080)));
						comd = TEMP_DIVL;
						num = j;
						data = *(pp->md->DATEMP_MIN_MW+(offset-0x0080));
					}
					else if(offset>= 0x0090 && offset <= 0x009F)
					{
						j = (offset-0x0090)%TEMP_MW_CH_NUM;   
						CanExtTempMoldWrite(ext_board_id,TEMP_JKMD,j, *(pp->md->FL_TKTJ_MW+(offset-0x0090)));
						comd = TEMP_JKMD;
						num = j;
						data = *(pp->md->FL_TKTJ_MW+(offset-0x0090));
					}
					else if(offset>= 0x00A0 && offset <= 0x00AF)
					{
						j = (offset-0x00A0)%TEMP_MW_CH_NUM;   
						CanExtTempMoldWrite(ext_board_id,TEMP_BL,j, *(pp->md->DACENT_HEATERON_MW+(offset-0x00A0)));
						comd = TEMP_BL;
						num = j;
						data = *(pp->md->DACENT_HEATERON_MW+(offset-0x00A0));
					}
					else if(offset == 0x00B0)
					{
						CanExtTempMoldWrite(ext_board_id,TEMP_KEEP,0, (pp->md->FL_TEMPHOLD_MW));
						comd = TEMP_KEEP;
						num = 0;
						data = pp->md->FL_TEMPHOLD_MW;
					}
					else if(offset == 0x00B1)
					{
						CanExtTempMoldWrite(ext_board_id,TEMP_WARM,0, (pp->md->DATEMP_HOLD_MW));
						comd = TEMP_WARM;
						num = 0;
						data = pp->md->DATEMP_HOLD_MW;
					}
					else if(offset == 0x00B2)
					{
						CanExtTempMoldWrite(ext_board_id,TEMP_CYCL,0, (pp->md->DATEMP_SCTM_MIN_MW));
						comd = TEMP_CYCL;
						num = 0;
						data = pp->md->DATEMP_SCTM_MIN_MW;
					}
					else if(offset == 0x00B3)
					{
						CanExtTempMoldWrite(ext_board_id,TEMP_DETE,0, (pp->md->FL_TEMPUPCHECK_MW));
						comd = TEMP_DETE;
						num = 0;
						data = pp->md->FL_TEMPUPCHECK_MW;
					}
					else if(offset == 0x00B4)
					{
						CanExtTempMoldWrite(ext_board_id,TEMP_NUM,0, (pp->md->DATEMP_NUM_MW));
						comd = TEMP_NUM;
						num = 0;
						data = pp->md->DATEMP_NUM_MW;
					}
					else if(offset == 0x00B5)
					{
						CanExtTempMoldWrite(ext_board_id,TEMP_YWT,0, (pp->md->TM_HEATERWARM_MW));
						comd = TEMP_YWT;
						num = 0;
						data = pp->md->TM_HEATERWARM_MW;
					}
					else if(offset == 0x00B6)
					{
						CanExtTempMoldWrite(ext_board_id,TEMP_USE,0, (pp->md->FL_USE_MW));
						comd = TEMP_USE;
						num = 0;
						data = pp->md->FL_USE_MW;
						if(pp->md->FL_USE_MW == 0)
						{
							pp->mw_close_flag = 1;
							pp->mw_close_tm = Get10MsTick();
						}
						else
						{
							pp->mw_close_flag = 0;
							pp->mw_close_tm = Get10MsTick();
						}
					}
					else if(offset == 0x00B7)
					{
						//CanExtTempMoldWrite(ext_board_id,TEMP_YH,0, (pp->md->FL_TEMP_YH));
						//comd = TEMP_YH;
						//num = 0;
						//data = pp->md->FL_TEMP_YH;
					}
					else if(offset == 0x00B8)
					{
						//CanExtTempMoldWrite(ext_board_id,TEMP_YHQZ,0, (pp->md->FL_TEMP_YHQZ));
						//comd = TEMP_YHQZ;
						//num = 0;
						//data = pp->md->FL_TEMP_YHQZ;
					}
					else if(offset == 0x00B9)
					{
						CanExtTempMoldWrite(ext_board_id,TEMP_TOBU,0, (pp->md->CONFUNC_MW));
						comd = TEMP_TOBU;
						num = 0;
						data = pp->md->CONFUNC_MW;
					}

					Temp_Last_Record(pp,ext_board_id,comd,num,data);
				}
				else if ((len > 1)&&(offset == 0)&&((GET_CMD_PARA(cmd) & 0xff) == MD_ID) && ComIsOk())
				{
					g_cantempe501[pp->mw_id].TempMold_node_state = 1;
					pp->mw_put_step = 0;
				}
			}
			break;
			
		case CMD_MANUAL_INIT:
			pp->mw_maul_statue = 1;
			pp->mw_maul_starttm = GetMsTick();
			break;
        default:
            ret = FALSE;
            break;
    }
    return ret;
}


BOOL InitTempMwPart(PTEMPCTRLMW pp, UI8 sn)
{
	int i;
    static const SUB_INIT_INFO sub_init[] = TEMPCTRLMW_INIT_INFO;
	
    if (!INIT_PART(pp, TEMP_MW, sn, sub_init))
    {
        return FALSE;
    }    

	if(sn >= TEMPMW_MAX_NUM)
	{
		return FALSE;
	}
	
	pp->mw_id = sn<TEMPMW_MAX_NUM?sn:0;	//E502温度模块的部件序号

	g_temp_mw[g_tempmw_num] = pp;
	g_tempmw_num++;

	pp->mw_put_step = 0;
	pp->mw_num = 0;
	pp->mw_sendnum = 0;

	for(i=0;i<MWNUM;i++)
	{
		pp->mw_buf[i].ext_board_last = 0;
		pp->mw_buf[i].ext_command = 0;
		pp->mw_buf[i].ext_num_set = 0;
		pp->mw_buf[i].ext_data_set = 0;
		pp->mw_buf[i].ext_begin_tm = 0;
		pp->mw_buf[i].ext_begin_flag = 0;	
		pp->mw_buf[i].ext_begin_cnt = 0;
	}

	mw_sendtommi_step = 0;
	mw_sendtommi_tm = 0;
	mw_sendtommi_num = 0;

	mw_piece = 0;
	totalnum = 0;
	sendnum = 0;
	mw_hearttm = 0;
	mw_scantm = 0;
    return TRUE;
}


