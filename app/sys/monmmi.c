/******************************************************************************
  文 件 名   : monmmi.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 上位机监控数据集命令处理，监控数据发送
  函数列表   :
              DataSetCmdRx
              MMIMonLoop
              MMIMonReset
              MonDelItem
              MonTx
              SendMonCycleRunSt
              TranslateMonInd
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
#include "protcmd.h"
#include "systempro.h"
#include "machapi.h"
#include "printf.h"
#include "actctrl.h"
#include "clamp.h"
#include "monmmi.h"
#include "cfgdb.h"
#include  "SRAM.h"


STATE_MAPPING g_state_mapping[] =
{
	{0x0516002B,0x0E160038,1}
};


MMI_MON_ST mon_st;
DAT_REC_ST	data_rec_st;

void StateMappingInit()
{
	UI16 i = 0,j = 0;	
	
	for(i = 0; i< ARRAY_NUM(g_state_mapping) ; i++)
	{
		for(j = 0;j<3;j++)
		{
			g_state_mapping[i].pmon_ind[j] = NULL;
			g_state_mapping[i].pmon_offset[j] = 0;
		}
	}
}

void StateMappingSet(UID id,UI16 offset,UI16 len,PMON_IND pmon_ind, UI16 addr,UI16 set_id)
{
	UI16 i = 0,j = 0;	
	PPART pp;
	UI16 partid;
	
	for(i = 0; i< ARRAY_NUM(g_state_mapping) ; i++)
	{
		if(id == (g_state_mapping[i].dataid_old >> 16))
		{
			if(((g_state_mapping[i].dataid_old&0xFFFF) >= offset) && ((g_state_mapping[i].dataid_old&0xFFFF) <=(offset+len)))
			{
				for(j = 0;j<3;j++)
				{
					if(g_state_mapping[i].pmon_ind[j] == NULL)
					{
						g_state_mapping[i].pmon_ind[j] = pmon_ind;
						g_state_mapping[i].pmon_offset[j] = (g_state_mapping[i].dataid_old&0xFFFF) - offset;
						g_state_mapping[i].addr_m[j] = addr;
						g_state_mapping[i].set_id[j] = set_id;	

						partid = g_state_mapping[i].dataid_new >> 16;
						pp = GetPartByID(partid);
						if(pp!=NULL)
						{
							g_state_mapping[i].addr_z[j] = pp->psub[partid&0x0F] + (UI16)((g_state_mapping[i].dataid_new)&0x00FF);
						}						
						break;
					}				
				}		

				break;
			}
		}
	}
	
	
}

void StateMappingPro(UI16* pnow , UI16 set_id)
{
	UI16 i = 0, j = 0;
	
	if(pnow == NULL)
		return;
	
	for(i = 0; i< ARRAY_NUM(g_state_mapping) ; i++)
	{
		for(j = 0;j<3;j++)
		{
			if((g_state_mapping[i].addr_z[j] != NULL) && (g_state_mapping[i].set_id[j] == set_id))
			{
				*(pnow + g_state_mapping[i].addr_m[j]) = *(g_state_mapping[i].addr_z[j]);
			}
		}		
	}
}

BOOL HeartBeatSend(void);

MMI_MON_ACT mon_act[] = {{1, 80, SentTskState, 0},
						  {2, PROT_FAIL_TM/4, HeartBeatSend, 0}};				//心跳
						  
BOOL HeartBeatSend()
{
	SendOperMode();
	return TRUE;
}

BOOL MonTx(UI16 set_id, PMON_IND pind, int num, int len, BOOL need_ack)
{
	PPROT_DATSET_RSP_PKT ppkt;
	UI16* pnow;
	PMON_IND pend;
	UI16 tlen;
	
	if(len + sizeof(PROT_DATSET_RSP_PKT) >  PROT_MAX_DAT_LEN || pind == NULL || num <= 0)
		return FALSE;

	if((ppkt = (PPROT_DATSET_RSP_PKT)ProtNewPkt(DATASET_RSP_CMD, 0, need_ack,  len + sizeof(*ppkt))) != NULL)
	{
		//转为WORD单位计数的长度
		len = len >> 1;
		ppkt->set_id = set_id;
		pnow = (UI16*)(ppkt + 1);
		pend = pind + num;
		while(pind < pend && len> 0)
		{	
			tlen = 0;
			if(pind->len > len)
				break;
			
			if(pind->pdat != NULL && pind->dat_len <= pind->len && pind->len <= len)
			{
				/*外部内存*/				
#ifdef SYS232				
				MemcpyPro(pnow, pind->pdat, pind->dat_len << 1);
#else
				memcpy(pnow, pind->pdat, pind->dat_len << 1);
#endif
				tlen = pind->dat_len;
			}

			if(tlen < pind->len)
				memset(pnow + tlen, 0, (pind->len - tlen) << 1);
			
			
			pnow += pind->len;
			len -= pind->len;
			pind++;
		}

		StateMappingPro((UI16*)(ppkt + 1),set_id);
		
		return MainProtTxPkt(ppkt);
	}

	return FALSE;
}


BOOL SendMonCycleRunSt()
{
	PMON_ITEM pmon;
	
	pmon = mon_st.mon;
	while(pmon < mon_st.mon+ ARRAY_NUM(mon_st.mon))
	{
		if(pmon->set_id == MON_CYCLE_ST_ID)
		{
			if(MonTx(pmon->set_id, mon_st.ind + pmon->ind_start, pmon->ind_num, pmon->data_len, TRUE))
			{
				pmon->last_tx_ms = GetMsTick();
				return TRUE;
			}
		}
		
		pmon++;
	}

	return FALSE;
}

/*******************************************************************************************
 *Brief			:警报状态数据上传触发程序。
 				设定警报:原则上立刻上传。
 				清除警报:延迟等待后再上传。(可以消除清除警报后立刻再次警报的重复报警问题)
 *Param			:imm:0时，对上一次时间进行赋值，延迟设定间隔时间后再上传。imm:1时，在允许范围内立即上传 imm:2时，立刻上传
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20200909
********************************************************************************************/
BOOL SendMonAlarmSt(UI16 imm)
{
	PMON_ITEM pmon;
	
	pmon = mon_st.mon;
	while (pmon < mon_st.mon+ ARRAY_NUM(mon_st.mon))
	{
	    if (imm == 2)
            pmon->last_tx_ms = GetMsTick() - pmon->period_ms - 1;
        
		if (pmon->trig_send > REC_SEND_NUM)
		{
			pmon->trig_send = 1;
			if (imm == 0)
			{
				pmon->last_tx_ms = GetMsTick();
			}
			return TRUE;
		}
		
		pmon++;
	}

	return FALSE;
}


void MonDelItem(int sn)
{
	int i;
	int start_ind;
	int num;
	
	if(sn >= ARRAY_NUM(mon_st.mon))
		return;

	start_ind = mon_st.mon[sn].ind_start;
	num = mon_st.mon[sn].ind_num;
	mon_st.mon[sn].set_id = 0;
	mon_st.mon[sn].period_ms = 0;
	if(num > 0 && start_ind + num < mon_st.ind_num)
	{
		for(i = 0; i < ARRAY_NUM(mon_st.mon); i++)
		{
			if(i != sn && mon_st.mon[i].ind_start >= start_ind + num)
				mon_st.mon[i].ind_start -= num;
		}
		
		memcpy(mon_st.ind + start_ind, mon_st.ind + start_ind + num, sizeof(mon_st.ind[0])*(mon_st.ind_num - start_ind - num));
		mon_st.ind_num -= num;
	}

	mon_st.mon[sn].ind_num = 0;
	mon_st.mon[sn].ind_start = 0;
	
	return;
}

void TranslateMonInd(PMON_IND pmon_ind, PPROT_DATSET_REQ_PKT ppkt, int num)
{
	int i;
	PPART pp;
	UI16 addr = 0;
	UI16 sub_len;
	UI16 sub_id;
	UI16 set_id;

	PDATSET_IND pdat_ind = (PDATSET_IND)(ppkt + 1);
	set_id = ppkt->set_id;
	
	if(pmon_ind == NULL || pdat_ind == NULL || num <= 0)
		return;

	for(i = 0; i < num; i++)
	{
		sub_id = pdat_ind->sub_ind;
		sub_len = SUB_IND_LEN(sub_id);
		sub_id = SUB_IND_ID(sub_id);
		pp = GetPartByID(pdat_ind->part_id);		

		//新增状态地址映射功能，此处设定映射地址	-zt 2019-09-17
		StateMappingSet((pdat_ind->part_id | sub_id),pdat_ind->offset,sub_len,pmon_ind,addr,set_id);
		
		db_printf("ind = %d, part_id = %04x, sub_id = %d, len= %d, offset = %d\r\n", i, pdat_ind->part_id, sub_id, sub_len, pdat_ind->offset);
		if(pp != NULL && sub_id < PART_SUB_NUM && pp->psub[sub_id] != NULL && pp->sub_len[sub_id] > pdat_ind->offset)
		{		
			pmon_ind->pdat = pp->psub[sub_id] + pdat_ind->offset;
			if(pp->sub_len[sub_id] > pdat_ind->offset + sub_len)
				pmon_ind->dat_len = sub_len;
			else
				pmon_ind->dat_len = pp->sub_len[sub_id] -pdat_ind->offset;
		}
		else
		{
			pmon_ind->pdat = NULL;
			pmon_ind->dat_len = 0;
		}

		pmon_ind->len = sub_len;
		addr = addr + sub_len;
		pmon_ind++;
		pdat_ind++;
	}
}

void DataSetCmdRx(PPROT_DATSET_REQ_PKT ppkt)
{
	UI16 len;
	PDATSET_IND pind;
	PMON_ITEM pmon;
	int	i, free_sn = -1;	
	
	if(ppkt == NULL)
		return;

	len = 0;
	pind = (PDATSET_IND)(ppkt + 1);
	for(i = 0; i < ppkt->ind_num; i++)
	{
		len +=SUB_IND_LEN(pind->sub_ind)*sizeof(UI16) ;
		pind++;
	}
	
	if(len+ sizeof(PROT_DATSET_RSP_PKT) > PROT_MAX_DAT_LEN || len <= 0)
		return;

	pmon = NULL;
	for(i = 0; i < ARRAY_NUM(mon_st.mon); i++)
	{
		if(mon_st.mon[i].set_id != 0 && ppkt->set_id == mon_st.mon[i].set_id)
		{
			pmon = mon_st.mon + i;
			if((ppkt->period_ms < MON_MIN_PERIOD && ppkt->set_id >= MON_PRE_DEF_ID_END) || ppkt->ind_num != pmon->ind_num)
			{
				/*需要删除该数据监控*/
				MonDelItem(i);
				break;
			}
			else
			{
				/*条目相同，占用原索引位置*/
				TranslateMonInd(mon_st.ind + pmon->ind_start,ppkt, pmon->ind_num);
				pmon->period_ms = ppkt->period_ms;
				return;
			}
		}
		else if(mon_st.mon[i].set_id == 0 && free_sn == -1)
			free_sn = i;
	}
	
	if(mon_st.ind_num + ppkt->ind_num < ARRAY_NUM(mon_st.ind))
	{
		TranslateMonInd(mon_st.ind + mon_st.ind_num, ppkt, ppkt->ind_num);
		if((ppkt->period_ms == 0 && ppkt->set_id >= MON_PRE_DEF_ID_END) || ppkt->set_id == 0)
			MonTx(ppkt->set_id, mon_st.ind + mon_st.ind_num, ppkt->ind_num, len, TRUE);		//单次数据直接发送
		else
		{
			/*增加一个监控数据集*/
			if(free_sn == -1)
			{
				free_sn = (mon_st.next_free_mon + 1)%ARRAY_NUM(mon_st.mon);
				MonDelItem(free_sn);
			}
			
			mon_st.next_free_mon = free_sn;
			pmon = mon_st.mon + free_sn;
			pmon->set_id = ppkt->set_id;
			pmon->period_ms = ppkt->period_ms;
			pmon->ind_start = mon_st.ind_num;
			pmon->ind_num = ppkt->ind_num;
			pmon->data_len = len;
			pmon->last_tx_ms = 0;

			//Id为0x182的状态数据为警报状态,将警报状态数据的上传模式
			//在主机上做出调整,将原来的数据定时上传,改为数据触发上传	-20200909zt	
			if (pmon->set_id == 0x182)
			{
			    if (ChkSysCfg(XF5_DHMAN))
                {         
				    pmon->trig_send = 0;//1;	//警报主动上传功能关闭。M7周期较快，不需要像M4一样靠减少232通讯来加快主循环。
                }
                else
                {
                    pmon->trig_send = 1;
                }
			}
			else
			{
				pmon->trig_send = 0;
			}
			
			mon_st.ind_num += pmon->ind_num;
		}
	}
	else
	{
		int abc = 0;
		abc++;
		db_printf("ind_num out\r\n");
	}
}

BOOL DataRecSend(PDAT_REC prec, BOOL ack)
{
	PPROT_DATREC_RSP_PKT ppkt;
	BOOL ret = FALSE;
	UI16	Send_falg = 0;

	if(prec->Prot_cmd == DATREC_QXRC_CMD)
	{
		Send_falg = DATREC_QXTX_CMD;
		ack = 0;						//后台曲线时，取消应答
	}
	else
		Send_falg = DATREC_RSP_CMD;
	if((ppkt = (PPROT_DATREC_RSP_PKT)ProtNewPkt(Send_falg, 0, ack, sizeof(PROT_DATREC_RSP_PKT) + prec->rec_len*sizeof(UI16))) != NULL)
	{
		ppkt->rec_id = prec->rec_id;
		ppkt->rec_len = prec->rec_len;
		ppkt->rec_ticks = prec->rec_start_ticks;
		memcpy(ppkt + 1, prec->prec_buf + prec->send_offset, prec->rec_len*sizeof(UI16));

		if((ret = ProtTxPkt(ppkt, prec->prot_id)) == TRUE)
		{
		}
	}

	return ret;
}

void RecTriggerStart(PDAT_REC prec)
{
	if(prec != NULL)
	{
		prec->rec_state = REC_START;
		prec->rec_len = 0;
		prec->rec_ticks = 0;
		prec->rec_start_ticks = 0;
		prec->rec_start_ms = GetMsTick();
		prec->rec_sample_tm = 0;
		prec->send_offset = 0;
		
		/*单次触发*/
		if(prec->trigger_id == REC_SCOP_SINGLE)
		{
			/*判断是否已经申请到缓存，已申请到了，就不再申请。*/
			if(prec->prec_buf != NULL && prec->prec_buf == prec->rec_buf)
			{
				prec->buf_size = 0;
				if((prec->prec_buf = (UI16*)GetCache(&(prec->buf_size))) == NULL)
				{
					/*单次 记录，无法申请到记录缓存，释放本次记录*/
					prec->rec_state = REC_IDLE;
				}
			}
			
			/*从GetCache获取的prec->buf_size是字节数，要转换成WORD数*/
			prec->buf_size = prec->buf_size/sizeof(WORD);
		}
	}	
}

void RecTriggerEnd(PDAT_REC prec)
{
	/*发送一个记录数为0的包,表示本次记录结束*/
	prec->rec_len = 0;
	DataRecSend(prec, TRUE);
	prec->rec_start_ticks = prec->rec_ticks;
	
	if(prec->trigger_id == REC_SCOP_SINGLE)
	{
		FreeCache(prec->prec_buf);
		prec->rec_state = REC_IDLE;
		prec->prec_buf = prec->rec_buf;
	}
	else if(prec->trigger_id == REC_SCOP_CONT)
	{
		prec->rec_state = REC_IDLE;
	}
	else
	{
		prec->rec_state = REC_TRIGGER;
	}
}

BOOL RecTriggerCheck(PDAT_REC prec)
{
	PTRIG_CON ptrig;
	UI16 tmp_val;
	BOOL trig_tmp = FALSE;
	BOOL trig_ret = FALSE;
	
	ptrig = prec->trig_con;
	tmp_val = *(ptrig->trig_pdat);
	if(ptrig->trig_mode == TRIG_RISING)			//上升沿触发
	{
		trig_ret = (ptrig->last_value < ptrig->threshold && tmp_val >= ptrig->threshold);
	}
	else if(ptrig->trig_mode == TRIG_FALLING)	//下降沿触发
	{
		trig_ret = (ptrig->last_value > ptrig->threshold && tmp_val <= ptrig->threshold);
	}
	ptrig->last_value = tmp_val;

	/*最多只检查两个触发条件*/
	if(prec->trig_num > 1)
	{
		ptrig++;
		tmp_val = *(ptrig->trig_pdat);
		if(ptrig->trig_mode == TRIG_RISING)			//上升沿触发
		{
			trig_tmp = (ptrig->last_value < ptrig->threshold && tmp_val >= ptrig->threshold);
		}
		else if(ptrig->trig_mode == TRIG_FALLING)	//下降沿触发
		{
			trig_tmp = (ptrig->last_value > ptrig->threshold && tmp_val <= ptrig->threshold);
		}
		ptrig->last_value = tmp_val;
		
		if(ptrig->and_or == TRIG_AND)
		{
			 trig_ret = (trig_ret & trig_tmp);
		}
		else if(ptrig->and_or == TRIG_OR)
		{
			trig_ret = (trig_ret | trig_tmp);
		}
	}

	return trig_ret;
}

void DataRecCmdRx(PPROT_DATREC_REQ_PKT ppkt, UI8 prot_id, UI16 len,UI16 prot_cmd)
{
	int i, k;
	PDAT_REC prec;
	PDATSET_IND pind;
	PPART pp;
	
	if(ppkt == NULL || ppkt->rec_period <= 0 || ppkt->ind_num > ARRAY_NUM(prec->rec_pdat))
		return;

	for(i = 0; i < ARRAY_NUM(data_rec_st.dat_rec); i++)
	{
		prec = data_rec_st.dat_rec + i;
		if(prec->rec_state == REC_IDLE || (prec->rec_id == ppkt->rec_id && prec->prot_id == prot_id))
			break;
	}

	if(i >= ARRAY_NUM(data_rec_st.dat_rec))
		return;

	/*记录数据个数必须大于0*/
	if(ppkt->ind_num <= 0)
		return;
	
	memset(prec, sizeof(*prec), 0);
	prec->rec_id = ppkt->rec_id;				//协议索引取值，面板自定义
	prec->trigger_id = ppkt->trigger_id;		//动作ID取值，当为动作ID时，代表动作开始时记录，结束时停止记录并上传
	prec->rec_period = ppkt->rec_period;		//采样周期时间取值
	prec->rec_total_ms = ppkt->rec_total_ms;	//记录的总时间取值
	prec->rec_dat_num = ppkt->ind_num;			//采样的条数取值
	prec->prot_id = prot_id;
	prec->Prot_cmd = prot_cmd;					//记录面板发送下来的大命令，用于在上传时做区分，一个给面板，一个给后台
	
	/*示波器数据记录功能，时间单位为主机运行周期数*/
	/*其他需转换主机运行周期数*/
	if(prec->trigger_id != REC_SCOP_CONT && prec->trigger_id != REC_SCOP_SINGLE)
	{
		if(prec->rec_period < REC_MIN_PERIOD)
			prec->rec_period = REC_MIN_PERIOD;
		
		prec->rec_period = (prec->rec_period*10)/MAX_LOOP_100US;	//将时间单位转化为周期数，如面板下发了a，即代表10ms采集一个点，此处转化为10*10/5 = 20,代表每20个周期采集一个点
	}

	if(prec->trigger_id == REC_SCOP_CONT)
	{
		if(prec->rec_period > 0)
			prec->rec_period = prec->rec_period - 1;		//后台曲线按照周期数往下发送，由于后面的地方是用(prec->rec_sample_tm-- <= 0)，所以这里要减去一才能使时间对的上
	}

	pind =(PDATSET_IND)(ppkt + 1);
	//db_printf("rec id %d", ppkt->ind_num);
	for(i = 0; i < ppkt->ind_num; i++)
	{
		pp = GetPartByID(pind->part_id);

		//db_printf("part: %x, sub %d, offset: %d\r\n", pind->part_id, pind->sub_ind, pind->offset);
		if(pp != NULL && pind->sub_ind < PART_SUB_NUM && pp->psub[pind->sub_ind] != NULL && pp->sub_len[pind->sub_ind] > pind->offset)
			prec->rec_pdat[i] = pp->psub[pind->sub_ind] + pind->offset;
		else
			prec->rec_pdat[i] = NULL;
		pind++;
	}

	/*新协议增加触发条件设定。根据包的长度来判断是否有触发条件设定*/
	if(len > sizeof(PROT_DATREC_REQ_PKT) + PROT_PACK_LEN + ppkt->ind_num*sizeof(DATSET_IND))
	{
		PTRIGSET_IND ptrig;
		UI16 trig_num;

		trig_num = *(UI16*)pind;
		ptrig = (PTRIGSET_IND)(((UI16*)pind) + 1);

		if(trig_num > TRIG_NUM)
			trig_num = TRIG_NUM;

		for(i = 0, k = 0; i < trig_num; i++)
		{
			pp = GetPartByID(ptrig->part_id);

			//db_printf("part: %x, sub %d, offset: %d\r\n", pind->part_id, pind->sub_ind, pind->offset);
			if(pp != NULL && ptrig->sub_ind < PART_SUB_NUM && pp->psub[ptrig->sub_ind] != NULL && pp->sub_len[ptrig->sub_ind] > ptrig->offset)
			{
				prec->trig_con[k].trig_pdat = pp->psub[ptrig->sub_ind] + ptrig->offset;
				prec->trig_con[k].last_value = *(prec->trig_con[k].trig_pdat);
				prec->trig_con[k].threshold = ptrig->threshold;
				prec->trig_con[k].trig_mode = ptrig->trig_mod;
				prec->trig_con[k].and_or = ptrig->and_or;
				k++;
			}
			
			ptrig++;
		}
		prec->trig_num = k;	
	}

	if(prec->prec_buf != prec->rec_buf)
		FreeCache(prec->prec_buf);
	
	prec->prec_buf = prec->rec_buf;			//将数据缓存地址指针初始化，指向缓存的首地址
	prec->buf_size = ARRAY_NUM(prec->rec_buf);	//缓存大小保存

	/*示波器记录类型，无触发条件，直接开始记录*/
	if(((prec->trigger_id == REC_SCOP_CONT ||prec->trigger_id == REC_SCOP_SINGLE) && prec->trig_num == 0) || prec->trigger_id == 0)
	{
		RecTriggerStart(prec);
	}
	else
	{
		prec->rec_state = REC_TRIGGER;
	}
}

void DataRecOPRx(PPROT_DATREC_OP_PKT ppkt, UI8 prot_id)
{
	int i;
	PDAT_REC prec;

	if(ppkt == NULL)
		return;
	
	for(i = 0; i < ARRAY_NUM(data_rec_st.dat_rec); i++)
	{
		prec = data_rec_st.dat_rec + i;
		if(prec->rec_state != REC_IDLE && (prec->rec_id == ppkt->rec_id && prec->prot_id == prot_id))
			break;
	}

	if(i >= ARRAY_NUM(data_rec_st.dat_rec))
		return;

	switch(ppkt->op_type)
	{
		case DATREC_OP_STOP:
		case DATREC_OP_CLOSE:		//接收到该命令后，停止曲线的采集

			if(prec->trigger_id == REC_SCOP_SINGLE)
				FreeCache(prec->prec_buf);
			
			prec->rec_state = REC_IDLE;
			break;
			
		case DATREC_OP_TX:
			DataRecSend(prec, TRUE);
			prec->rec_len = 0;
			prec->rec_start_ticks = prec->rec_ticks;
			break;
		case DATREC_OP_STOP_TX:
			prec->rec_state = REC_STOP;
			DataRecSend(prec, TRUE);
			prec->rec_len = 0;
			prec->rec_start_ticks = prec->rec_ticks;
			break;
		case DATREC_OP_CLR:
			prec->rec_len = 0;
			break;
		case DATREC_OP_START:
			RecTriggerStart(prec);
			break;
		default:
			break;
	}
}


void DataRec_Stop(PPROT_DATREC_OP_PKT ppkt, UI8 prot_id)	//后台曲线停止命令函数
{
	int i;
	PDAT_REC prec;

	if(ppkt == NULL)
		return;
	
	for(i = 0; i < ARRAY_NUM(data_rec_st.dat_rec); i++)
	{
		prec = data_rec_st.dat_rec + i;

		if((prec->trigger_id == REC_SCOP_CONT) && (prec->rec_state != REC_IDLE)
			&& prec->prot_id == prot_id)
			prec->rec_state = REC_IDLE;
	}

	if(i >= ARRAY_NUM(data_rec_st.dat_rec))
		return;

	switch(ppkt->op_type)
	{
		case DATREC_OP_STOP:
		case DATREC_OP_CLOSE:
				break;
			
		default:
			break;
	}
}


void Data_Pc_Stop(void)	//后台曲线停止，当面板与主机通讯断开时
{
	int i;
	PDAT_REC prec;

	for(i = 0; i < ARRAY_NUM(data_rec_st.dat_rec); i++)
	{
		prec = data_rec_st.dat_rec + i;

		if((prec->trigger_id == REC_SCOP_CONT) && (prec->rec_state != REC_IDLE))
			prec->rec_state = REC_IDLE;
	}
}


void DataRecSendNext(PDAT_REC prec)
{
	UI16 rec_len_sav;

	if(prec->send_offset >= prec->rec_len)
		return;
	
	/*保存已记录数据长度，避免被修改*/
	rec_len_sav = prec->rec_len;

	prec->rec_start_ticks = prec->send_offset/prec->rec_dat_num;
	if(rec_len_sav - prec->send_offset >= DAT_REC_BUF_SIZE)
	{
		prec->rec_len = DAT_REC_BUF_SIZE;
	}
	else
	{
		prec->rec_len = rec_len_sav - prec->send_offset;
	}
		
	if(DataRecSend(prec, FALSE))
	{
		prec->send_offset += prec->rec_len;
	}

	/*记录数据长度*/
	prec->rec_len = rec_len_sav;
}


/*一个主机运行周期只能调用一次。*/
void DataRecLoop(PDAT_REC prec)
{
	int i;
	UI16 rec_len = 0;	//数据长度临时变量，每放一个曲线数据到缓存中则加一，最后再把当前这一个循环发的数据赋值给prec->rec_len保存
	UI8 dat_num = 0;	//每一个周期所要采集的个数，也就是所有需要的线的条数
	UI16 tmp_len = 0;
	
	/*时间已改成主机运行周期数*/
	if(prec->rec_sample_tm-- <= 0)
	{
		prec->rec_sample_tm =prec->rec_period;	//要采集的周期数赋值
		dat_num = prec->rec_dat_num;			//采集的数据个数赋值
		tmp_len = prec->rec_len + dat_num;		//记录已经采集到的数据总长度，每周期增加已知的曲线个数，当达到DAT_REC_BUF_SIZE时，再一次性发送出去
		if(tmp_len > prec->send_offset + DAT_REC_BUF_SIZE)		//当非触发模式时，send_offset一直为0，只要累计长度到达上限，即启动发送
		{
			if(prec->trigger_id != REC_SCOP_SINGLE)		//非单次触发
			{
				DataRecSend(prec, prec->rec_start_ticks == 0);
				prec->rec_len = 0;
				prec->rec_start_ticks = prec->rec_ticks;
			}
			else
			{
				//单次触发
				if(tmp_len > prec->buf_size)
				{
					//记录结束，进入数据发送状态，发送剩余的数据
					prec->rec_state = REC_SEND;
					return;
				}
				else
				{
					/*数据记录长度大于DAT_REC_BUF_SIZE时发送数据*/
					if(ProtTxIsIdle(prec->prot_id))
						DataRecSendNext(prec);
				}
			}
		}
			
		prec->rec_ticks++;
		rec_len = prec->rec_len;		//将存储在缓存的个数拿出来，如最开始是0，一次画4条线，则一遍过后，由于rec_len++，导致prec->rec_len也加了4，第二遍时，则是从第五个缓存地址开始放数据
		for(i = 0; i < dat_num; i++)
		{
			if(prec->rec_pdat[i] != NULL)
				prec->prec_buf[rec_len++] = *(prec->rec_pdat[i]);
			else
				prec->prec_buf[rec_len++] = 0;
		}

		prec->rec_len = rec_len;
	}

	if(prec->rec_total_ms > MIN_REC_TOTAL_MS && GetMsTick() > prec->rec_start_ms + prec->rec_total_ms 
		&& (prec->trigger_id != REC_SCOP_CONT))		//在pc后台监控时，不进行这个处理，否则会导致主机给后台发送长度为0的数据包
	{
		/*记录时间到，停止记录*/
		prec->rec_state = REC_TRIGGER_END1;
	}
 }

void MMIMonReset()
{
	int i;

	for(i = 0; i < ARRAY_NUM(mon_act); i++)
		mon_act[i].last_tx_ms = 0;

	for(i = 0; i < ARRAY_NUM(mon_st.mon); i++)
		mon_st.mon[i].last_tx_ms = 0;

	/*屏蔽状态数据一段时间，减少面板处理手动键的响应时间*/
	//mon_st.mon_start_ms = GetMsTick() + MON_RESET_STOP_MS;
}


void MMIMonRecLoop()		//曲线数据采集函数
{
	int i;
	PDAT_REC prec;
	
	for(i = 0; i < DAT_REC_NUM; i++)
	{
		prec = data_rec_st.dat_rec + i;
		if(prec->rec_state == REC_IDLE)
			continue;

		if(prec->rec_state == REC_TRIGGER && prec->trig_num > 0)	//处于等待触发状态，同时触发条件大于0时
		{
			/*检查触发条件*/
			if(RecTriggerCheck(prec))
			{
				RecTriggerStart(prec);
			}
		}

		if(prec->rec_state == REC_START)
		{
			DataRecLoop(prec);
		}
		else if(prec->rec_state == REC_TRIGGER_END1)
		{
			if(prec->rec_len > 0)
			{
				DataRecSend(prec, FALSE);
				prec->rec_len = 0;
			}

			RecTriggerEnd(prec);
		}
		else if(prec->rec_state == REC_SEND)
		{
			/*单次记录数据发送。*/
			if(ProtTxIsIdle(prec->prot_id))
			{
				/*发送监控数据*/
				DataRecSendNext(prec);
				if(prec->rec_len <= prec->send_offset)
				{
					/*已发送完毕*/
					RecTriggerEnd(prec);
				}
			}
		}
	}
}

void MMIMonLoop()
{
	int i;
	int sn;
	UI32 max, diff;
	PMON_ITEM pmon;
	
	if(!MainProtTxIsIdle() ||!ComIsOk())
		return;

	//if(mon_st.mon_start_ms != 0 && GetMsTick() < mon_st.mon_start_ms)
	//	return;
	
	//mon_st.mon_start_ms = 0;
	
	for(i = 0; i < sizeof(mon_act)/sizeof(mon_act[0]); i++)
	{
		if((UI16)(GetMsTick() - mon_act[i].last_tx_ms) >= mon_act[i].intval_ms || mon_act[i].last_tx_ms == 0)
		{
			if(mon_act[i].func != NULL && (*mon_act[i].func)())
				mon_act[i].last_tx_ms = GetMsTick();
		}
	}

	/*最多只发一个监控数据*/
	diff = 0;
	sn = -1;
	max = 0;
	for(i = 0; i < MON_ITEM_NUM; i++)
	{
		pmon = mon_st.mon + i;
		if((pmon->trig_send <= REC_SEND_NUM)&&(pmon->set_id > MON_PRE_DEF_ID_END) && GetMsTick() - pmon->last_tx_ms > pmon->period_ms)
		{			
			diff = GetMsTick() - pmon->last_tx_ms -pmon->period_ms;
			if(diff > max)
			{
				sn = i;
				max = diff; 
			}
		}
	}

	if(sn != -1)
	{
		pmon = mon_st.mon + sn;
		if(MonTx(pmon->set_id, mon_st.ind + pmon->ind_start, pmon->ind_num, pmon->data_len, pmon->set_id < MON_PRE_DEF_ID_END))
		{
			pmon->last_tx_ms = GetMsTick();
			if (pmon->trig_send > 0)
			{
				pmon->trig_send++;
			}
		}
	}
	else
		ShowExtOnline();						//扩展板在线状态
}

void MMIMonActRunNotify(UI16 act_id)
{
	int i;
	PDAT_REC prec;
	
	for(i = 0; i < ARRAY_NUM(data_rec_st.dat_rec); i++)
	{
		prec = data_rec_st.dat_rec + i;

		/*trigger_id = 1，表示按全自动周期触发，在前置再循环时，结束数据记录，在再循环开始时开始记录数据*/
		if(prec->trigger_id == 1)
		{
			if(prec->rec_state == REC_START && act_id == MK_ACT(MACHINE_ID, 1, OS_PRCYC))
			{
				RecTriggerEnd(prec);
			}

			continue;
		}
		
		if(prec->trigger_id != act_id || prec->rec_state != REC_TRIGGER)
			continue;

		RecTriggerStart(prec);
	}
}

void MMIMonActEndNotify(UI16 act_id)
{
	int i;
	PDAT_REC prec;
	
	for(i = 0; i < ARRAY_NUM(data_rec_st.dat_rec); i++)
	{
		prec = data_rec_st.dat_rec + i;

		/*trigger_id = 1，表示按全自动周期触发，在前置再循环时，结束数据记录，在再循环开始时开始记录数据*/
		if(prec->trigger_id == 1)
		{
		 	if(prec->rec_state == REC_TRIGGER && act_id == MK_ACT(MACHINE_ID, 1, OS_RCYC))
			{
				RecTriggerStart(prec);
			}
			
			continue;
		}
		else if(prec->trigger_id != act_id || prec->rec_state != REC_START)
			continue;

		if(prec->rec_total_ms <= MIN_REC_TOTAL_MS)
		{
			prec->rec_state = REC_TRIGGER_END1;
		}
	}
}

