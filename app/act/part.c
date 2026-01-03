/******************************************************************************
  文 件 名   : part.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 实现部件公用的功能函数
  函数列表   :
              AbsDiff
              AddPartToTbl
              CallPartCmd
              ChkActRun
              ChkPartErr
              ClearAllPartError
              ClearAllPartSub
              ClearAllPartSubRxBit
              ClrPartErr
              GetActByID
              GetActRunFlag
              GetNextPartByInd
              GetPartActBySN
              GetPartByID
              GetPartByInd
              GetPartSubData
              GetPartSubNeedRxBit
              GetPartTypeInfo
              GetSubDataByID
              InitPart
              InitPartTypeInfo
              PartDataReqTx
              PartDataRx
              PartDataTx
              PartErrSet
              PartMakeSubIdBits
              PartSetSubRxBit
              PartSubReqTx
              PartSubTx
              SetActRunFlag
              SetPartErr
              SetPartSubData
  修改历史   :
  1.日    期   : 2013年5月17日
    作    者   : ren chaohong
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "com.h"
#include "part.h"
#include "acttype.h"
#include "basectrl.h"
#include "machine.h"
#include "protcmd.h"
#include "memtbl.h"
#include "printf.h"
#include "cfgdb.h"
#include "sram.h"
#include "Alarm.h"
#include "AlarmClass.h"
#include "TempCtrlMw.h"
/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
extern const PART_TYPE_INFO part_type_info[]; 
extern const UI16 part_type_info_num;

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
WORD GetRealSlaveNum(void);
BOOL SendMonAlarmSt(UI16 imm);
BOOL ChkAlarmIsEmpty(void);
void InsAlarmInAlarmCache(void);
void ClrAlarmCache(void);



/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
PART_IND part_ind_tbl[PART_TYPE_END + 1];
PART_TBL part_tbl;

/*该表目前固定，是否改成可配置，下一步考虑*/
UI8 part_info_ind[PART_TYPE_END];

PPART_DATA_CHANGE_CACHE ppart_data_change_cache;
/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/


/*******************************************************************************************
 *Brief			:初始化面板参数变更缓冲区
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20190815
********************************************************************************************/
void InitPartDataChangeCache()
{
	//功能涉及数据都申请外部内存。(变更的数据量不大，且使用不平频繁，所以考虑用外部)
	ppart_data_change_cache = AllocMemTbl(MEMTBL_DYNC_MEM_EXT, sizeof(PART_DATA_CHANGE_CACHE), 1);

	ppart_data_change_cache->data_num = 0;
	ppart_data_change_cache->change_cache_hd[0].pdat = &(ppart_data_change_cache->change_cache[0]);
}

/*******************************************************************************************
 *Brief			:检测当前数据是否已保存在缓冲区内，如果存在则缓冲区内数据立刻生效
 *Param			:psub 数据目标首地址    offset 数据目标偏移量   len 数据长度    
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20220311
********************************************************************************************/
BOOL ChkDelPartDataChangeCache(UI16* psub,UI16 offset,UI16 len)
{
    UI16 i;
	PPART_DATA_CHANGE_CACHE_HD ppart_data_cache_hd = (PPART_DATA_CHANGE_CACHE_HD)&(ppart_data_change_cache->change_cache_hd[0]);//初始化指针
	
	if(ppart_data_change_cache->data_num > 0)
	{
		for(i = 0; i< ppart_data_change_cache->data_num; i++,ppart_data_cache_hd++)
		{
			if(ppart_data_cache_hd->len > 0)//如果有其他路径已经对该参数进行填写了，则len=0，所以这里不需要再赋值
            {
                if(ppart_data_cache_hd->psub == psub && ppart_data_cache_hd->psub != NULL)
                {
                    if((offset + len < ppart_data_cache_hd->offset) || (offset > ppart_data_cache_hd->offset + ppart_data_cache_hd->len))
                    {
                        continue;
                    }
                    else
                    {                        
                        MemcpyPro(ppart_data_cache_hd->psub + ppart_data_cache_hd->offset, ppart_data_cache_hd->pdat, ppart_data_cache_hd->len<<1);
                        CallPartCmd(ppart_data_cache_hd->pp, ppart_data_cache_hd->cmd, (ppart_data_cache_hd->offset<< 16) |(ppart_data_cache_hd->len & 0xffff));
                        
                        ppart_data_cache_hd->len = 0;                        
                    }
                }
            } 

        }
    }
    return TRUE;
}



/*******************************************************************************************
 *Brief			:在全自动时面板设定参数，先保存在缓冲内，在再循环结束时赋值
 *Param			:
 *Return Value	:TRUE : 存储成功	FALSE : 未存储
 *Aauthor		:ZT
 *Date			:20190815
********************************************************************************************/
BOOL SetPartDataChangeCache(PPART pp,UI32 cmd,UI16* psub,UI16 offset,UI16* pdat,UI16 len)
{
	UI16 i;
	UI16* pdat_c;
	const UID protectlist[] = {CLAMP_ID, EJECT_ID, INJECT_ID};//数据保护部件列表	数组内的部件进行缓存保护

	if(ChkOperMode(OM_AUTO)&&(((cmd>>24)&0xFF)==MMI_DATA_RX))//在自动模式下，只针对单个数据修改
	{
		for(i = 0;i<ARRAY_NUM(protectlist);i++)
		{
			if(((pp->id>>8) & 0xFF) == protectlist[i])
			{//数据保护列表内数据需要进行缓存保护
				if (ChkSysCfg(XF1_NOBCK))
				{//不管是不是在当前部件在运行，都将修改是数据缓存起来。
					break;
				}
				else
				{
					if(ChkPartActRun(pp))
					{//动作运行时缓存
						break;
					}
					else
					{//不缓存
					    ChkDelPartDataChangeCache(psub,offset,len);
						return FALSE;
					}
				}				
			}

			if(i == ARRAY_NUM(protectlist) - 1)
			{//非保护数据，返回FALSE直接生效
			    ChkDelPartDataChangeCache(psub,offset,len);
				return FALSE;
			}
		}
		
		if(ppart_data_change_cache->data_num >= CHANGE_CACHE_HD_NUM)
		{//缓存个数达上限，返回FALSE直接生效
		    ChkDelPartDataChangeCache(psub,offset,len);
			return FALSE;
		}

		PPART_DATA_CHANGE_CACHE_HD ppart_data_cache_hd = &(ppart_data_change_cache->change_cache_hd[ppart_data_change_cache->data_num]);

		/**********数据变更的特征值保存************/
		ppart_data_cache_hd->psub = psub;
		ppart_data_cache_hd->offset = offset;
		ppart_data_cache_hd->len = len;
		ppart_data_cache_hd->pp = pp;
		ppart_data_cache_hd->cmd = cmd;
		/******************************************/
		
		pdat_c = ppart_data_cache_hd->pdat;				

		if((pdat_c + len) >= (&(ppart_data_change_cache->change_cache[CHANGE_CACHE_LEN])))
		{
			//如果数据存储地址超出了内存范围，表示数据已满，个数直接赋值最大值，返回FALSE直接生效
			ppart_data_change_cache->data_num = CHANGE_CACHE_HD_NUM;
            ChkDelPartDataChangeCache(psub,offset,len);
			return FALSE;
		}
		else
		{
			MemcpyPro(pdat_c, pdat, len<<1);
			ppart_data_cache_hd++;
			ppart_data_cache_hd->pdat = pdat_c + len;//位移存储数据地址并保存，为下一个数据存储做准备
			
			ppart_data_change_cache->data_num++;
		}

		return TRUE;
	}
	else
	{
	    ChkDelPartDataChangeCache(psub,offset,len);
		return FALSE;
	}	
}

/*******************************************************************************************
 *Brief			:数据搬运，将缓冲内的数据搬运到dic中，进行参数变更操作
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20190815
********************************************************************************************/
void MovePartDataChangeCache()
{
	UI16 i;
	PPART_DATA_CHANGE_CACHE_HD ppart_data_cache_hd = (PPART_DATA_CHANGE_CACHE_HD)&(ppart_data_change_cache->change_cache_hd[0]);//初始化指针
	
	if(ppart_data_change_cache->data_num > 0)
	{
		for(i = 0; i< ppart_data_change_cache->data_num;i++,ppart_data_cache_hd++)
		{
			if(ppart_data_cache_hd->len > 0 && ppart_data_cache_hd->psub != NULL)//如果有其他路径已经对该参数进行填写了，则len=0，所以这里不需要再赋值
			{
				MemcpyPro(ppart_data_cache_hd->psub + ppart_data_cache_hd->offset, ppart_data_cache_hd->pdat, ppart_data_cache_hd->len<<1);
				CallPartCmd(ppart_data_cache_hd->pp, ppart_data_cache_hd->cmd, (ppart_data_cache_hd->offset<< 16) |(ppart_data_cache_hd->len & 0xffff));
			}
		}

		//数据全部搬运后，对缓存做清零处理。
		ppart_data_change_cache->data_num = 0;

		//所有头内长度写0，表示未输入，第一组数据赋值首地址
		for(i = 0;i < CHANGE_CACHE_HD_NUM;i++)
		{
			ppart_data_change_cache->change_cache_hd[i].len = 0;
		}
	}	
}


/*******************************************************************************************
 *Brief			:动作为运行的部件缓存器数据搬运
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20191017
********************************************************************************************/
void MovePartDataChangeCacheWithActStart(UID actid)
{	
	if (ChkSysCfg(XF1_NOBCK))
	{//只在再循环及非自动时赋值
		return;
	}
	
	PPART_DATA_CHANGE_CACHE_HD ppart_data_cache_hd = (PPART_DATA_CHANGE_CACHE_HD)&(ppart_data_change_cache->change_cache_hd[0]);//初始化指针
	UI16 i = 0;
	
	if(ppart_data_change_cache->data_num > 0)
	{
		for(i = 0; i< ppart_data_change_cache->data_num;i++,ppart_data_cache_hd++)
		{
			//当前数据为有赋值数据，且是当前刚开始的动作id对应的部件数据，进行赋值操作
			if((ppart_data_cache_hd->len > 0  && ppart_data_cache_hd->psub != NULL) && ((actid&0xFFF0) == ((ppart_data_cache_hd->pp->id)&0xFFF0)))
			{
				MemcpyPro(ppart_data_cache_hd->psub + ppart_data_cache_hd->offset, ppart_data_cache_hd->pdat, ppart_data_cache_hd->len<<1);

				CallPartCmd(ppart_data_cache_hd->pp, ppart_data_cache_hd->cmd, (ppart_data_cache_hd->offset<< 16) |(ppart_data_cache_hd->len & 0xffff));

				ppart_data_cache_hd->len = 0;
			}
		}
	}
}

/********************************************************************************************/



const PART_TYPE_INFO* GetPartTypeInfo(UI8 part_type)
{
	if((part_type = part_info_ind[part_type]) < part_type_info_num)
		return part_type_info + part_type;

	return NULL;
}

UI32 CallPartCmd(PPART pp, UI32 cmd, UI32 para)
{
	UI8	tmp;
	const PART_TYPE_INFO* pt;
	
	tmp = GET_PART_TYPE(pp->id);
	if((pt = GetPartTypeInfo(tmp)) != NULL)
	{
		if(pt->cmd_func != NULL)
		{
			if(pt->cat == PART_CAT_SYS || pt->cat == PART_CAT_BCTRL)
				return (*(SYS_CMD_FUNC)(pt->cmd_func))(cmd, para);
			else
				return (*(PART_CMD_FUNC)(pt->cmd_func))(pp, cmd, para);
		}
	}

	return FALSE;
}

void PartSetSubRxBit(PPART pp, UI8 sub_id)
{
	if(sub_id < PART_SUB_END)
	{
		pp->sub_rx_bits |= SUB_BIT(sub_id);
	}
}

PPART GetPartByInd(int ind)
{
	if(ind >= 0 && ind < part_tbl.num)
		return part_tbl.part[ind];
	else
		return NULL;
}

PPART GetNextPartByInd(int* ind)
{
	int	i;

	if(ind == NULL || *ind >= part_tbl.num)
		return NULL;


	i = *ind + 1;

	if(i < 0)
		i = 0;
	
	while(i < part_tbl.num && part_tbl.part[i] == NULL)
		i++;

	*ind = i;
	if(i < part_tbl.num)
		return part_tbl.part[i];
	else
		return NULL;
}

PACT GetPartActBySN(PPART pp, UI8 sn)
{
	if(pp != NULL)
	{
		if(pp->act_num > sn || pp->pact_tbl != NULL)
			return pp->pact_tbl + sn - 1;
	}

	return NULL;
}

UI16 PartMakeSubIdBits(PPART pp, UI16 sub_id_bits)
{
	UI16* 	plen;
	UI16**	psub;
	int		i;
	UI16		set_b;	
	
	if(pp == NULL)
		return 0;

	sub_id_bits = (sub_id_bits & (( 1 << PART_SUB_NUM) - 1));
	plen = pp->sub_len;
	psub = pp->psub;
	set_b = 1;
	for(i = 0; i < PART_SUB_NUM; i++)
	{
		if(plen[i] <= 0 || psub[i] == NULL)
			sub_id_bits &=  (~set_b);
		set_b <<= 1;
	}

	return sub_id_bits;
}


void ClearAllPartSubRxBit()
{
	int i;

	for(i = 0; i < part_tbl.num; i++)
	{
		if(part_tbl.part[i] != NULL)
		{
			part_tbl.part[i]->sub_rx_bits = (SUB_BIT(PO_ID) | SUB_BIT(PI_ID));
		}
	}
}

UI16 GetPartSubNeedRxBit(PPART pp)
{
	const PART_TYPE_INFO* pinfo;
	UI8 tmp;
	
	if(pp != NULL)
	{
		tmp = GET_PART_TYPE(pp->id);
		if((pinfo = GetPartTypeInfo(tmp)) != NULL)
			return (PartMakeSubIdBits(pp, ~pp->sub_rx_bits) & (~pinfo->stor_sub_bits) & (~pinfo->state_sub_bits));
		else
			return PartMakeSubIdBits(pp, ~pp->sub_rx_bits);
	}
	else
		return 0;
}

void ClearAllPartError()
{
	int i;
	PPART pp;

	
	db_printf("Clear All Part Error;\r\n");

	for(i = 0; i < part_tbl.num; i++)
	{
		pp = part_tbl.part[i];
		if(pp != NULL && pp->psub[ST_ID] != NULL)
		{
			//屏蔽所有驱动器警报状态的清除
			if ((pp->id&0x0F00) == 0x0F00)//pump部件
			{
				pp->psub[ST_ID][0] = pp->psub[ST_ID][0]&0x0001;//滤除驱动器警报
				pp->psub[ST_ID][1] = 0;
			}
			else
			{
				pp->psub[ST_ID][0] = 0;
				pp->psub[ST_ID][1] = 0;
			}			
		}
	}

	ClrAlarmCache();

	if (g_tempmw_num > 0)
    {
        for (i = 0; i < g_tempmw_num; i++)
        {
        	if(g_temp_mw[i] != NULL)
        	{
        		g_temp_mw[i]->mw_maul_statue = 1;
				g_temp_mw[i]->mw_maul_starttm = GetMsTick();
        	}
        }
    }

	SendMonAlarmSt(0);
}

void ClearAllPartSub(UI8 sub_id)
{
	int i;
	PPART pp;
	if(sub_id >= PART_SUB_NUM)
		return;
	
	for(i = 0; i < part_tbl.num; i++)
	{
		pp = part_tbl.part[i];
		if(pp != NULL && pp->psub[sub_id] != NULL)
			memset(pp->psub[sub_id], 0, pp->sub_len[sub_id]*sizeof(UI16));
	}
}

/*******************************************************************************************
 *Brief			:	警报等级处理，按照警报等级做出相应处理:
 					1、等级一:关马达、切手动、警报输出(自动)
 					2、等级二:切手动、警报输出(自动)
 					3、等级三:警报输出(自动)
 					4、等级四:仅界面提示

 					注:如果有特殊处理方式，则按照特殊处理代码处理
 *Param			:palarm_class 警报分级结构体指针	alarm 手动时警报开启
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20200213
********************************************************************************************/
void PartErrClassPro(PPART pp, PALARM_CLASS palarm_class, UI16 alarm)
{	
	if (palarm_class->alarm_operate != NULL)
	{		
		//警报时特殊处理程序
		(*palarm_class->alarm_operate)(pp);
	}
	else
	{
		switch (palarm_class->alarm_class)
		{
			case ALARM_CLASS_1st://关马达、切手动
  				MotorDown();				
				ManualInitErr();
				StandAlarm(alarm);
 				SetOperMode(OM_MANUAL);
				break;
			case ALARM_CLASS_2nd://切手动
				ManualInitErr();
				StandAlarm(alarm);
 				SetOperMode(OM_MANUAL);
				break;
			case ALARM_CLASS_3rd:
				StandAlarm(alarm);
				break;
			default:
				break;
		}
	}
}

/*******************************************************************************************
 *Brief			:	该函数在原有基础上做了添加:
 					1、警报等级结构(警报等级表、警报统一处理)
 					2、警报详细数据上传功能。
 *Param			:err 警报代码
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20200213
********************************************************************************************/
void SetPartErr(PPART pp, UI32 err)
{
	//默认警报 FALSE,若果部分特殊警报需要在手动状态下也警报的话，
	//需要在设定警报下面添加警报代码
	UI16 alarm = FALSE;		
	PALARM_CLASS palarm_class;

	if(pp != NULL && pp->psub[ST_ID] != NULL && err != 0) // SetEcatInitStep(0)时err=0,会导致运行急停警报 2022-5-24 ccz;
	{		
		InsAlarmBuf((pp->id>>8)&0xFF,err);	//记录最新的警报信息

		if (ChkPartErr(pp,err))
		{
			return;
		}
		
#ifdef DEBUG		
		if(!(*(UI32*)(pp->psub[ST_ID]) &err))
			db_printf("Part %04x, error %04x\r\n", pp->id, err);
#endif
		//获得警报信息表
		palarm_class = GetAlarmClassInfo((pp->id>>8)&0xFF,err);

		//判定警报是否上传		
		if (!ChkSysCfg(XF5_SFSENCTL))
        {
        	if (palarm_class->alarm_updata > 0)
        	{
        		if(palarm_class->alarm_updata == 2)
        		{
					ALARM_DATA_TX_MUST(pp, err);
				}
				else
				{
					ALARM_DATA_TX(pp, err);
				}				
			}            
        }

		//警报等级处理
		PartErrClassPro(pp, palarm_class, alarm);
		
		//pp->psub[ST_ID]不一定是4字节对齐
		pp->psub[ST_ID][0] |= (err &0xffff);
		pp->psub[ST_ID][1] |= ((err >> 16) &0xffff);

		SendMonAlarmSt(1);
	}

	return;
}

void ClrPartErr(PPART pp, UI32 err)
{	
	if(pp != NULL && pp->psub[ST_ID] != NULL)
	{
		DelAlarmInAlarmCache((pp->id>>8)&0xFF,err);
		
		if (ChkPartErr(pp,err))
		{
			//pp->psub[ST_ID]不一定是4字节对齐
			pp->psub[ST_ID][0] &= ~(err &0xffff);
			pp->psub[ST_ID][1] &= ~((err >> 16) &0xffff);

			SendMonAlarmSt(0);
		}		
	}
}

UI32 ChkPartErr(PPART pp, UI32 err)
{
	UI32 tmp;
	
	if(pp != NULL && pp->psub[ST_ID] != NULL)
	{
		//pp->psub[ST_ID]不一定是4字节对齐
		tmp = ((UI32)pp->psub[ST_ID][1] << 16) + pp->psub[ST_ID][0];
		return (tmp & err);
	}
	else
		return 0;
}

BOOL PartSubReqTx(PPART pp, UI16 sub_id_bits, BOOL brsp)
{
	PPROT_SUB_REQ_PKT	ppkt;
	
	if(pp == NULL)
		return FALSE;

	sub_id_bits = PartMakeSubIdBits(pp, sub_id_bits);

	if((ppkt = (PPROT_SUB_REQ_PKT)ProtNewPkt(SUB_REQ_CMD, pp->id, brsp, sizeof(PROT_SUB_REQ_PKT))) != NULL)
	{
		ppkt->sub_id_bits = sub_id_bits;
		return MainProtTxPkt(ppkt);
	}
	else
		return FALSE;
}

BOOL PartDataReqTx(PPART pp, UI16 sub_id, UI16 len, UI16 offset)
{
	UI16	  	tlen;
	UI16*	psub;
	PPROT_DAT_REQ_PKT	ppkt;
	
	if(pp == NULL || sub_id >= PART_SUB_NUM || len == 0)
		return FALSE;

	psub = pp->psub[sub_id];
	if(psub == NULL)
		return FALSE;
		
	tlen = pp->sub_len[sub_id];
	if(offset >= tlen)
		return FALSE;
		
	if(len > tlen - offset)
		len = tlen - offset;

	if((ppkt = (PPROT_DAT_REQ_PKT)ProtNewPkt(DAT_REQ_CMD, pp->id, FALSE, sizeof(PROT_DAT_REQ_PKT))) != NULL)
	{
		ppkt->sub_ind = MAKE_SUB_IND(sub_id, len);
		ppkt->offset = offset;
		return MainProtTxPkt(ppkt);
	}
	else
		return FALSE;
}

BOOL PartSubTx(PPART pp, UI16 sub_id_bits, BOOL brsp)
{
	UI16* 	plen;
	UI16**	psub;
	UI16		tlen;
	int		i;
	UI8		sub_num;
	UI8		sub_id[PART_SUB_NUM];
	PPROT_SUB_RSP_PKT	ppkt;
	UI16*	pn;

	
	if(pp == NULL)
		return FALSE;

	//db_printf("part sub data recv, id = %04x, sub=%04x\r\n", pp->id, sub_id_bits);
	plen = pp->sub_len;
	psub = pp->psub;
	sub_num = 0;
	tlen = 0;

	for(i = 0; i < PART_SUB_NUM; i++)
	{
		if(plen[i] > 0 && psub[i] != NULL && (sub_id_bits & 0x01))
		{
			sub_id[sub_num] = i;
			sub_num++;
			tlen += plen[i];
		}

		sub_id_bits >>= 1;
	}

	if(sub_num <= 0 && tlen <= 0)
		return TRUE;

	if((ppkt = (PPROT_SUB_RSP_PKT)ProtNewPkt(brsp ? SUB_RSP_CMD : SUB_ACTV_CMD, pp->id, brsp, sizeof(PROT_SUB_RSP_PKT) + (sub_num + tlen- 1)*sizeof(UI16))) != NULL)
	{
		pn = ppkt->sub_ind + sub_num;
		ppkt->sub_num = sub_num;
		for(i = 0; i < sub_num; i++)
		{
			CallPartCmd(pp, MAKE_CMD(sub_id[i], CMD_DATA_TX), tlen);
			tlen = plen[sub_id[i]];
			ppkt->sub_ind[i] = MAKE_SUB_IND(sub_id[i], tlen);
			//Memcpy1(pn, psub[sub_id[i]], tlen);
			MemcpyPro(pn, psub[sub_id[i]], tlen << 1);
			pn += tlen;
		}
		return MainProtTxPkt(ppkt);
	}
	else
		return FALSE;
}


BOOL PartDataTx(PPART pp, UI8 sub_id, UI16 len, UI16 offset, BOOL brsp)
{
	UI16	  	tlen;
	PPROT_DAT_RSP_PKT	ppkt;
	
	if(pp == NULL || sub_id >= PART_SUB_NUM || len == 0 || pp->psub[sub_id] == NULL)
		return FALSE;

	tlen = pp->sub_len[sub_id];
	if(offset >= tlen)
		return FALSE;

	if(len > tlen - offset)
		len = tlen - offset;

	if((ppkt = (PPROT_DAT_RSP_PKT)ProtNewPkt(brsp ? DAT_RSP_CMD : DAT_ACTV_CMD, pp->id, brsp, sizeof(PROT_DAT_RSP_PKT) + (len - 1)*sizeof(UI16))) != NULL)
	{
		ppkt->sub_ind = MAKE_SUB_IND(sub_id, len);
		ppkt->offset = offset;
		
		CallPartCmd(pp, MAKE_CMD(sub_id, CMD_DATA_TX), (offset << 16) |(len & 0xffff));
		//Memcpy1(ppkt->sub_data, pp->psub[sub_id] + offset, len);
		MemcpyPro(ppkt->sub_data, pp->psub[sub_id] + offset, len<<1);
		return MainProtTxPkt(ppkt);
	}
	else
		return FALSE;
}

void PartNUllSubRx(PPART pp,  UI8 rx_type)
{
	int i;
	
	if(pp != NULL && rx_type == MMI_SUB_RX)
	{
		/*用于启动交互过程中，表示面板没有主机所请求的部件
		 把部件子类数据的接收标志置1，不用一直请求面板缺失的部件，
		 使启动交互能进行下去		*/
		for(i = 0; i < PART_SUB_NUM; i++)
			PartSetSubRxBit(pp, i);

		SetMachineErr(ER0_PARTNULL);
	}
}

void PartDataRx(PPART pp, UI8 sub_id, UI16 len, UI16 offset, UI16* pdat, UI8 rx_type)
{
	UI16*	psub;
	UI16	tlen;
	
	if(pp == NULL || len <= 0 || sub_id >= PART_SUB_NUM || pdat == NULL)
		return;

	tlen = pp->sub_len[sub_id];
	psub = pp->psub[sub_id]; 

#if 1
	if(rx_type == MMI_SUB_RX)
		db_printf("part sub recv, id = %04x, sub=%x, len = %d-%d\r\n", pp->id, sub_id, len, tlen);
	else
		db_printf("part data recv, id = %04x, sub=%x, offset = %04x, len = %d, data = %04x\r\n", pp->id, sub_id, offset, len, *pdat);
#endif

	if(psub == NULL)
		return;
		
	if(offset >= tlen)
		return;

	if(len > tlen - offset)
		len = tlen - offset;

	
	//Memcpy1(psub + offset, pdat, len);
		

	if (rx_type == MMI_SUB_RX)
	{
		PartSetSubRxBit(pp, sub_id);
	}
		

	if(rx_type == MMI_SUB_RX || rx_type == MMI_DATA_RX)  //解决面板改变开模联动时，需按手动才生效问题
	{
		if((sub_id == MH_ID || sub_id == MD_ID || sub_id == PA_ID) && len > 0)
		{
			/*由于部件数据可能变化，通知动作列表需要更新*/
			ActForkTblOutDate(TRUE);
		}
	}
	
	if (!SetPartDataChangeCache(pp,MAKE_CMD(((rx_type <<8) |sub_id), CMD_DATA_RX),psub,offset,pdat,len))
	{
		MemcpyPro(psub + offset, pdat, len<<1);		
		if(len > 0)
		{
			VersionDiff();
			CallPartCmd(pp, MAKE_CMD(((rx_type <<8) |sub_id), CMD_DATA_RX), (offset << 16) |(len & 0xffff));
		}
	}

		
/*	if(pp->id == MK_PART(AUTOCTRL_ID, 1))// && sub_id == MD_ID)
	{
		db_printf("match\r\n");
		ly= 0;
	}*/
	return;
}

BOOL GetPartSubData(PPART pp, UI8 sub_id, UI16 ind, UI16* pval)
{
	if(pp != NULL && pval != NULL  && sub_id < PART_SUB_NUM && ind < pp->sub_len[sub_id] && pp->psub[sub_id] != NULL)
	{
		*pval = pp->psub[sub_id][ind];
		return TRUE;
	}

	return FALSE;
}

UI16 GetSubDataByID(UI16 part_id, UI8 sub_id, UI16 ind)
{
	PPART pp;

	pp = GetPartByID(part_id);
	if(pp != NULL && sub_id < PART_SUB_NUM && ind < pp->sub_len[sub_id] && pp->psub[sub_id] != NULL)
		return  pp->psub[sub_id][ind];
	else
		return 0;
}

BOOL SetPartSubData(PPART pp, UI8 sub_id, UI16 ind, UI16 val)
{
	if(pp != NULL && sub_id < PART_SUB_NUM && ind < pp->sub_len[sub_id] && pp->psub[sub_id] != NULL)
	{
		pp->psub[sub_id][ind] = val;		
		CallPartCmd(pp, MAKE_CMD(((sub_id <<8) |sub_id), CMD_DATA_RX), (ind << 16) |(1 & 0xffff));
		return TRUE;
	}

	return FALSE;
}

BOOL AddPartToTbl(PPART pp)
{
	UI8			sn;
	UI8			type;
	UI8			ind;
	
	if(pp == NULL)
		return TRUE;
		
	type = GET_PART_TYPE(pp->id);
	sn = GET_PART_SN(pp->id);

	if(type > PART_TYPE_END || sn == 0)
		return FALSE;

	/*if part == 0, system part*/
	if(sn > MAX_PART_SN)
		return FALSE;

	ind = part_ind_tbl[type].ind[sn -1];
	if(ind == 0)
	{
		/*new part*/
		if(part_tbl.num >= MAX_PART_NUM)
		{
			db_printf("AddPartToTbl part num over zhe MAX_PART_NUM, part_num = %d, MAX_PART_NUM = %d \r\n", part_tbl.num, MAX_PART_NUM);
			return FALSE;
		}
		part_tbl.part[part_tbl.num] = pp;
		part_tbl.num++;
		part_ind_tbl[type].ind[sn -1] = part_tbl.num;
	}
	else if(part_tbl.part[ind - 1] != pp)
		return FALSE;

	return TRUE;
}

PPART GetPartByID(UI16 part_id)
{
	UI8			sn;
	UI8			type;
	UI8			ind;
	
	type = GET_PART_TYPE(part_id);
	sn = GET_PART_SN(part_id);

	if(type > PART_TYPE_END || sn == 0 || sn > MAX_PART_SN)
		return NULL;

	ind = part_ind_tbl[type].ind[sn -1]; 
	if(ind == 0 || ind > part_tbl.num)
		return NULL;

	return (part_tbl.part[ind -1]);
}

PACT GetActByID(UI16 act_id)
{
	UI8			sn;
	UI8			act_sn;
	UI8			type;
	UI8			ind;
	PPART		pp;
	
	type = GET_PART_TYPE(act_id);
	sn = GET_PART_SN(act_id);
	act_sn = GET_ACT_SN(act_id);
	
	if(type > PART_TYPE_END || sn == 0 || act_sn > MAX_ACT_SN || sn > MAX_PART_SN)
		return NULL;

	ind = part_ind_tbl[type].ind[sn -1]; 
	if(ind == 0 || ind > part_tbl.num)
		return NULL;

	if((pp = part_tbl.part[ind -1]) == NULL)
		return NULL;

	if(pp->act_num < act_sn || pp->pact_tbl == NULL)
		return NULL;
		
	return pp->pact_tbl + act_sn - 1;
}

void SetActRunFlag(void*  pp, UI8 sn, BOOL brun)
{
	UI16 mask;

	if(pp != NULL && sn <= ((PPART)pp)->act_num && sn > 0)
	{
		mask = (1 << (sn -1));
		if(brun)
			((PPART)pp)->act_run_flag |= mask;
		else
			((PPART)pp)->act_run_flag &= (~mask);
	}
}

UI16 GetActRunFlag(void* pp)
{
	return ((PPART)pp)->act_run_flag;
}

UI16 ChkActRun(void* pp, UI16 mask)
{
	if(pp)
		return (((PPART)pp)->act_run_flag & mask);
	else	
		return FALSE;
}

UI16 ChkPartActRun(void* pp)
{
	if(pp)
		return (((PPART)pp)->act_run_flag > 0);
	else	
		return FALSE;
}

/*******************************************************************************************
 *Brief			:检查任务是否停止 状态为TSK_IDLE
 *Param			:pact 动作任务首地址	act_sn 任务序号
 *Return Value	:TRUE 停止	FALSE 运行
 *Aauthor		:ZT
 *Date			:20191106
********************************************************************************************/
UI16 ChkActIsIDLE(PACT pact, UI16 act_sn)
{
	PACT 	pt = NULL;
	
	if(pact == NULL)
	{
		return TRUE;
	}
	else
	{
		pt = pact + act_sn - 1;
		return (pt->ptsk->pst->tsk_st == TSK_IDLE);
	}
}

BOOL PartErrSet(void* pp, UI32 err_msk, BOOL bset)
{
	if(pp == NULL)
		return FALSE;
		
	if(bset)
	{
		SET_PART_ERR(pp, err_msk);
		return TRUE;
	}
	else
	{
		CLR_PART_ERR(pp, err_msk);
		return FALSE;
	}
}

void InitPartTypeInfo()//初始化部件信息
{
	int i;

	memset(part_info_ind, 0xff, sizeof(part_info_ind));
	for(i = 0; i < part_type_info_num; i++)
	{
		if(part_type_info[i].type < ARRAY_NUM(part_info_ind))
			part_info_ind[part_type_info[i].type] = i;
	}
}

BOOL InitPart(PPART pp,  UI8 type, UI8 sn, const  SUB_INIT_INFO* psub_init, int sub_num)//初始化部件特征数据结构，并分配各个子类的内存空间
{
	int		i;
	UI16	len;
	UI16*	ptmp;
	void**	psub;
	const PART_TYPE_INFO* pinfo;
	
	if(pp == NULL)
		return FALSE;

	memset(pp, 0, sizeof(*pp));
	
	pp->id = MK_PART(type, sn);   //根据部件类型生成部件ID
	
	if(sub_num== 0 || psub_init == NULL)
		return TRUE;

	len = 0;	
	for(i = 0; i < sub_num; i++)    //计算所有子类所需要的内存长度
	{
		if(psub_init[i].sub_id < PART_SUB_NUM)
			len += psub_init[i].sub_len;
	}

	pinfo = GetPartTypeInfo(type);   //part_type_info[]中序号指针

//	if (pinfo->cat == PART_CAT_SUB_SRAM)
//	{
		ptmp = AllocMemTbl(MEMTBL_DYNC_MEM_EXT, sizeof(UI16), len);//申请部件各个子类需要的内存空间并返回指针
//	}
//	else
//	{
//		ptmp = AllocMemTbl((pinfo == NULL || pinfo->cat != PART_CAT_BCTRL) ? MEMTBL_ACT_PART : MEMTBL_CTRL_PART, sizeof(UI16), len);//申请部件各个子类需要的内存空间并返回指针
//	}
	

	if(ptmp == NULL)
	{
		SetMachineErr(ER0_MEM);
		return FALSE;
	}
	
	psub = (void**)(pp + 1);
	for(i = 0; i < sub_num; i++)
	{
		if(psub_init[i].sub_len > 0 && psub_init[i].sub_id < PART_SUB_NUM)//初始化各个子类的首地址指针到部件特真结构体中
		{
			pp->sub_len[psub_init[i].sub_id] = psub_init[i].sub_len;
			pp->psub[psub_init[i].sub_id] = ptmp;
			*psub = ptmp;
			ptmp += psub_init[i].sub_len;
		}
		
		psub++;
	}

	AddPartToTbl(pp);//在part_ind_tbl中建立本部件结构体指针在part_tbl的索引目录,并将本部件特真数据结构的首地址保存到part_tbl中

	/*装入存储的部件数据 */
	if(pinfo != NULL)   //取出部件中的子类需要从下位机配置中读取的数据并初始化这个子类
	{
		UI16 sub_bit = 1;
		const PART_CFG_IND* pcfg;
		
		for(i = 0; i < ARRAY_NUM(pp->sub_len); i++)
		{
			if((pinfo->stor_sub_bits & sub_bit) && pp->sub_len[i] > 0 && pp->psub[i] != NULL)
			{
				if((pcfg = PartCfgRead(pp, i)) != NULL)
					PartDataRx(pp, pcfg->sub_id, pcfg->len, pcfg->offset, pcfg->psave, INIT_CFG_RX);
			}
			
			sub_bit <<= 1;
		}
	}

	return TRUE;
}

int AbsDiff(int x1, int x2)
{
	return ((x1) > (x2) ? ((x1) - (x2)) : ((x2) -(x1)));
}
int AbsDiff_2(int x1, int x2)
{
    return ((x1) > (x2) ? ((x1) - (x2)) : 0);
}


int AbsDiffMin(int x1,int x2)
{
	return ((x1) > (x2) ? (x2) : (x1));
}
