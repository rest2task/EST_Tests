/******************************************************************************
  文 件 名   : cfgdb.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 部件及表配置数据库：删除、读取、存储、更新和上传功能
  函数列表   :
              BootInitCfgDb
              CfgDbCopyItem
              CfgDbFindItem
              CfgDbItemIsSame
              CfgDbPartRead
              CfgDbPartSave
              CfgDbReadItem
              CfgDbTblRead
              CfgDbTblSave      
              CfgDbUpdate
              CfgDbUpdateFail
              CfgDbItemSave
              CfgDbUpload
              CfgStorAdrRelocate
              ChkHWStor
              EepromRead
              EepromWrite
              FlashErase
              FlashRead
              FlashWrite
              FreeCache
              GetCache
              HWStorErase
              HWStorLoad
              HWStorSave
              HWStorWrite
              InitCfgHD
              LoadCfgDb
              UserPartCfgReset
              UserTblCfgReset
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
#include "memtbl.h"
#include "cfgdb.h"
#include "crc16.h"
#include "tbl.h"
#include "useriap.h"
#include "hwcfg.h"
#include "machapi.h"
#include "sram.h"
#include "hwmid.h"

DEF_ALIGN UI8 cfg_cache[CFG_CACHE_NUM][CFG_CACHE_SIZE];

UI8 cfg_cache_bused[CFG_CACHE_NUM];		//系统内部公共内存使用标记
UI8 cfg_cache_word_bused[CFG_WORD_CACHE_NUM];	//系统外部内存公共内存使用标记




static const HW_STOR_AREA hw_stor[HW_STOR_AREA_NUM] = {	{0, DYNC_MEM_AREA, HW_MEM_AREA, DYNC_TBL_SIZE, DYNC_TBL_SIZE, NULL},		//内存
														{1, FACTORY_CFG_AREA, HW_FLASH_AREA, FLASH_4K_SIZE, FLASH_F_P_ADR, NULL},	//FLASH4K
														{2, USER_CFG_AREA, HW_FLASH_AREA, FLASH_4K_SIZE, FLASH_U_P_ADR, NULL},		//FLASH4K
														{3,FACTORY_CFG_AREA, HW_FLASH_AREA, FLASH_4K_SIZE, FLASH_F_T_ADR, NULL},	//FLASH4K		
														{4,USER_CFG_AREA, HW_FLASH_AREA, FLASH_4K_SIZE, FLASH_U_T_ADR, NULL}};		//FLASH4K		

static const HW_STOR_AREA hw_stor_backup[HW_STOR_AREA_NUM] = {{0, DYNC_MEM_AREA_backup, HW_MEM_AREA, DYNC_TBL_SIZE, DYNC_TBL_SIZE, NULL},			//内存
														{1, FACTORY_CFG_AREA_backup, HW_FLASH_AREA, FLASH_4K_SIZE_backup, FLASH_F_P_ADR_backup, NULL},	//FLASH4K
														{2, USER_CFG_AREA_backup, HW_FLASH_AREA, FLASH_4K_SIZE_backup, FLASH_U_P_ADR_backup, NULL},		//FLASH4K
														{3,FACTORY_CFG_AREA_backup, HW_FLASH_AREA, FLASH_4K_SIZE_backup, FLASH_F_T_ADR_backup, NULL},		//FLASH4K		
														{4,USER_CFG_AREA_backup, HW_FLASH_AREA, FLASH_4K_SIZE_backup, FLASH_U_T_ADR_backup, NULL}};		//FLASH4K	
														

HW_STOR_ST hw_stor_st[ARRAY_NUM(hw_stor)];
HW_STOR_ST hw_stor_st_backup[ARRAY_NUM(hw_stor_backup)];		//备份区的存储状态

/*出厂部件数据存储在FLASH16K_3，用户i部件数据存储在EEPROM1区域，
出厂表数据存储在FLASH64K_1区域， 用户 表数据存储在EEPROM2区域*/
CFG_ST cfg_st[CFG_ID_END] = {{0}, {1}, {2} ,{3}, {4}};
CFG_ST cfg_st_backup[CFG_ID_END] = {{0}, {1}, {2} ,{3}, {4}};	//备份区的

CFGDB_UPDATE_ST update_st;

BOOL CfgDbPartSaveL(PPART pp, UI16 sub_id, UI16 len, UI16 offset, CFG_DB_ID db_id);


CFG_SAVE_LIST_MENU cfg_save_list;
#define SAVEWAITCN	5

/*******************************************************************************************
 *Brief			:对列表结构体进行初始化
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20190825
********************************************************************************************/
UI16 InitListSaveCfgDb()
{
	UI16 i = 0;
	cfg_save_list.lastsavetime = 0;
	cfg_save_list.MoveCacheLocation = 0;
	cfg_save_list.LoadCacheLocation = 0;
	
	for(i = 0;i<SAVE_CFGLIST_NUM;i++)
	{
		cfg_save_list.savelist[i].pp = NULL;
		cfg_save_list.savelist[i].sub_id = 0;
		cfg_save_list.savelist[i].offset = 0;
		cfg_save_list.savelist[i].db_id = CFG_ID_START;	
		cfg_save_list.savelist[i].state = 0;		
	}	
	
	return 1;
}


/*******************************************************************************************
 *Brief			:设定写FLASH操作，将其保存在操作列表中
 *Param			:
 *Return Value	:TRUE	操作以保存，等待激活	FLASH	操作列表满或无需缓存，直接操作不等待
 *Aauthor		:ZT
 *Date			:20190825
********************************************************************************************/
UI16 SetListSaveCfgDb(PPART pp, UI16 sub_id, UI16 len, UI16 offset, CFG_DB_ID db_id)
{
#if 1//M7单次写Flash数据时长达到200ms，驱动器必定会出现断使能，所以该功能无效，所以M7将该缓存功能关闭	----2021/11/18 zt
	CfgDbPartSaveL(pp, sub_id, len,offset, db_id);
		return FALSE;
#else
	UI16 i = 0,offsetbuf=0;
	
	if(!MotorIsOn() || ChkRunFlag(INIF_INITPART) || (cfg_save_list.lastsavetime > SAVEWAITCN))
	{
		CfgDbPartSaveL(pp, sub_id, len,offset, db_id);
		return FALSE;
	}

//	//空闲时也等待一个周期
//	if(cfg_save_list.lastsavetime > SAVEWAITCN)
//	{
//		cfg_save_list.lastsavetime = 0;
//	}

	if((cfg_save_list.MoveCacheLocation == cfg_save_list.LoadCacheLocation) && (cfg_save_list.savelist[cfg_save_list.LoadCacheLocation].state == 1))
	{
		//列表已满，直接操作
		CfgDbPartSaveL(pp, sub_id, len,offset, db_id);
		return FALSE;
	}
	
	for(i = 0;i<SAVE_CFGLIST_NUM;i++)
	{
		if(cfg_save_list.savelist[i].state == 1)
		{
			if((cfg_save_list.savelist[i].pp == pp) 
				&& (cfg_save_list.savelist[i].sub_id == sub_id)
				&& (cfg_save_list.savelist[i].db_id == db_id))	
			{
				if((cfg_save_list.savelist[i].offset != offset)
				|| (cfg_save_list.savelist[i].len != len))
				{
					if(cfg_save_list.savelist[i].len == 0 || len == 0)
					{
						//如果其中有一组数据长度为0，说明保存数据为该项的算有数据	ps:程序里几乎都用0
						cfg_save_list.savelist[i].offset = 0;
						cfg_save_list.savelist[i].len = 0;						
					}
					else
					{	
						//如有相同项数据保存，那么取最大范围包含两组数据
						offsetbuf = MIN(cfg_save_list.savelist[i].offset,offset);
						cfg_save_list.savelist[i].len = MAX(cfg_save_list.savelist[i].offset + cfg_save_list.savelist[i].len ,offset + len) - offsetbuf;	
						cfg_save_list.savelist[i].offset = offsetbuf;
					}
				}				

				return TRUE;
			}
		}		
	}	
	
	//将相关操作及数据放入列表中
	cfg_save_list.savelist[cfg_save_list.LoadCacheLocation].pp = pp ;
	cfg_save_list.savelist[cfg_save_list.LoadCacheLocation].sub_id = sub_id;
	cfg_save_list.savelist[cfg_save_list.LoadCacheLocation].offset = offset;
	cfg_save_list.savelist[cfg_save_list.LoadCacheLocation].db_id = db_id;
	cfg_save_list.savelist[cfg_save_list.LoadCacheLocation].state = 1;

	cfg_save_list.LoadCacheLocation++;
	if(cfg_save_list.LoadCacheLocation == SAVE_CFGLIST_NUM)
	{
		cfg_save_list.LoadCacheLocation = 0;
	}
	
	return TRUE;
	
#endif
}

/*******************************************************************************************
 *Brief			:间隔操作FLASH 防止连续操作使得主循环周期过长，伺服驱动器断使能
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20190825
********************************************************************************************/
UI16 ListSaveCfgDb()
{
	UI16 i = 0;

	cfg_save_list.lastsavetime++;
	
	if(cfg_save_list.lastsavetime > SAVEWAITCN)
	{
		if(cfg_save_list.savelist[cfg_save_list.MoveCacheLocation].state > 0)
		{
			i = cfg_save_list.MoveCacheLocation;
			CfgDbPartSaveL(cfg_save_list.savelist[i].pp, cfg_save_list.savelist[i].sub_id, cfg_save_list.savelist[i].len,cfg_save_list.savelist[i].offset, cfg_save_list.savelist[i].db_id);

			cfg_save_list.savelist[i].state = 0;
			
			cfg_save_list.MoveCacheLocation++;
			if(cfg_save_list.MoveCacheLocation == SAVE_CFGLIST_NUM)
			{
				cfg_save_list.MoveCacheLocation = 0;
			}

			return TRUE;
		}
	}
	return FALSE;
}

/*******************************************************************************************
 *Brief			:获取外部内存公用内存的内存地址	使用该内存的结构体内不能存在8位的数据定义
 				使用后一定要进行释放，防止堵塞
 *Param			:公用内存长度地址，函数内会填写相应大小
 *Return Value	:返回对应公共内存的地址指针
 *Aauthor		:ZT
 *Date			:20190911
********************************************************************************************/
UI16* GetCacheWord(UI16* psize)
{
	int i;

	for(i = 0; i <CFG_WORD_CACHE_NUM; i++)
	{
		if(!cfg_cache_word_bused[i])
		{
			if(psize != NULL)
				*psize = sizeof(cfg_cache_word[i]);
			
			cfg_cache_word_bused[i] = TRUE;
			return cfg_cache_word[i];
		}
	}
	
	if(psize != NULL)
		*psize = 0;
	return NULL;
}

/*******************************************************************************************
 *Brief			:针对上面外部内存的公共内存释放，使用完后必须释放相应内存
 *Param			:释放的内存的地址
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20190911
********************************************************************************************/
void FreeCacheWord(void* pcache)
{
	int i;

	for(i = 0; i <CFG_WORD_CACHE_NUM; i++)
	{
		if(((UI16*)pcache >= cfg_cache_word[i]) && ((UI16*)pcache < (cfg_cache_word[i] + sizeof(cfg_cache_word[i])/2)))
		{
			cfg_cache_word_bused[i] = FALSE;
		}
	}
}


UI8* GetCache(UI16* psize)
{
	int i;

	for(i = 0; i <CFG_CACHE_NUM; i++)
	{
		if(!cfg_cache_bused[i])
		{
			if(psize != NULL)
				*psize = sizeof(cfg_cache[i]);
			
			cfg_cache_bused[i] = TRUE;
			return cfg_cache[i];
		}
	}
	
	if(psize != NULL)
		*psize = 0;
	return NULL;
}

void FreeCache(void* pcache)
{
	int i;

	for(i = 0; i <CFG_CACHE_NUM; i++)
	{
		if((UI8*)pcache >= cfg_cache[i] && (UI8*)pcache < cfg_cache[i] + sizeof(cfg_cache[i]))
		{
			cfg_cache_bused[i] = FALSE;
		}
	}
}

BOOL FlashErase(UI32 adr)
{
	db_printf("Flash Erase: %x\r\n", adr);
	return (m_program_erase(adr + FLASH_IAP_BASE) == 0);
}

BOOL FlashWrite(UI32 adr, void* pmem, int len)
{
	UI8 tmp_fill[FLASH_W_ALGN];
	BOOL ret = TRUE;
	int tlen;
	
	/*Program_IAP 写入地址和长度都必须FLASH_W_ALGN的倍数*/
	/*如果adr != FLASH_W_ALGN的倍数，必须先读出部分字节*/
	db_printf("Flash write: %x, %d\r\n", adr, len);

	tlen = (adr & (FLASH_W_ALGN - 1));
	if(tlen > 0)
	{
		memcpy(tmp_fill, (void*)(adr - tlen), tlen);
		if(FLASH_W_ALGN -tlen > len)
		{
			memcpy(tmp_fill + tlen, pmem, len);
			memset(tmp_fill + tlen + len, 0xff, (FLASH_W_ALGN - tlen - len));
			len = 0;
 		}
		else
		{
			memcpy(tmp_fill + tlen, pmem, FLASH_W_ALGN - tlen);
			pmem = (UI8*)pmem + (FLASH_W_ALGN -tlen);
			len -= (FLASH_W_ALGN -tlen);
		}

		adr = adr - tlen;
		ret = (ret && (m_program_iap((UI32)(tmp_fill), adr + FLASH_IAP_BASE, FLASH_W_ALGN/4) != 0));
		adr += FLASH_W_ALGN;
	}

	if(len > 0)
	{
		/*写剩余的字节，如剩余长度不是FLASH_W_ALGN的倍数，则先写FLASH_W_ALGN倍数长度的数据*/
		tlen = (len - (len & (FLASH_W_ALGN -1)));
		ret = (ret && (m_program_iap((UI32)pmem, adr + FLASH_IAP_BASE, tlen >> 2) != 0));	

		//if(tlen<=1024)
		{
			adr += tlen;
			pmem = (UI8*)pmem + tlen;

			/*写最后不足FLASH_W_ALGN字节长度的数据*/
			tlen = (len & (FLASH_W_ALGN -1));
			if(tlen > 0)
			{
				memcpy(tmp_fill, pmem, tlen);
				memset(tmp_fill + tlen, 0xff, (FLASH_W_ALGN - tlen));
				ret = (ret && (m_program_iap((UI32)(tmp_fill), adr + FLASH_IAP_BASE, FLASH_W_ALGN/4) != 0));
			}
		}
	}

	return ret;
}

BOOL FlashRead(UI32 adr, void* pmem, int len)
{
	if(pmem != NULL && len> 0)
	{
		memcpy(pmem, (void*)adr, len);
		return TRUE;
	}
	else
		return FALSE;
}

BOOL EepromWrite(UI32 adr, void* pmem, int len)
{
	/*长度必须为2的倍数*/
	if(pmem != NULL)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL EepromRead(UI32 adr, void* pmem, int len)
{
	/*长度必须为2的倍数*/
	
	if((len& 0x01) == 0 && pmem != NULL)
	{
		return TRUE;
	}

	return FALSE;
}

void CfgStorAdrRelocate(PCFG_HD phd, UI32 src_base, UI32 dst_base)
{
	/*对存储表的地址项进行重新计算, 
	内存写入到flash时, src_base为内存起始地址, dst_base为FLASH段的内存映射地址;
	EEPROM读入到内存时, src_base为0, dst_base为内存起始地址
	内存写入到EEPROM, src_base为内存起始地址, dst_base = 0*/

	int  i;
	PCFG_ITEM pitem;
	
	pitem = (PCFG_ITEM)(phd + 1);
	for(i = 0; i < phd->item_num; i++)
	{
		pitem->item.part_ind.psave = (UI8*)pitem->item.part_ind.psave + dst_base - src_base;
		pitem = (PCFG_ITEM)((UI32)pitem + phd->item_len);
	}
}

BOOL HWStorErase(const HW_STOR_AREA* pa)
{
	UI16 err_id;
	if(pa != NULL)
	{
		if(pa->phy_type == HW_FLASH_AREA)
		{
			hw_stor_st[pa->id].chk_st = HWSTOR_ERASED;
			return FlashErase(pa->area_adr);
		}
		else if(pa->phy_type == HW_EEPROM_AREA)
		{
			hw_stor_st[pa->id].chk_st = HWSTOR_ERASED;
			err_id = 0;
			return EepromWrite(pa->area_adr, &err_id, 2);
		}
	}

	return FALSE;
}

BOOL HWStorWrite(const HW_STOR_AREA* pa, void* pdat, int len, int offset)
{
	if(pa != NULL && pdat != NULL)
	{
		if(pa->phy_type == HW_FLASH_AREA)
			return FlashWrite(pa->area_adr + offset, pdat, len);
		else if(pa->phy_type == HW_EEPROM_AREA)
			return EepromWrite(pa->area_adr + offset, pdat, len);
	}

	return FALSE;
}

PCFG_HD HWStorLoad(const HW_STOR_AREA* pa, HWSTOR_RD_TYPE rd_type, void* pmem, int len)
{
	PCFG_HD phd = NULL;
	
	/*FLASH 读*/
	if(pa->phy_type == HW_FLASH_AREA)
	{
		phd = (PCFG_HD)pa->area_adr;
		if(phd->id != CFG_HD_ID || phd->len < sizeof(CFG_HD)  || phd->len > pa->area_size)
			return NULL;

		if(phd->len < sizeof(CFG_HD) + phd->item_len*phd->item_num)
			return NULL;
		
		if(*(UI16*)((UI32)phd+ phd->len) == CRC_CCITT16(phd, phd->len))
		{
			if(pmem != NULL)
			{
				if(rd_type == HWSTOR_RD_HD && len >= sizeof(CFG_HD))
					memcpy(pmem, (void*)pa->area_adr, sizeof(CFG_HD));
				else if(rd_type == HWSTOR_RD_ITEM && len >= sizeof(CFG_HD) + phd->item_len*phd->item_num)
					memcpy(pmem, (void*)pa->area_adr, sizeof(CFG_HD) + phd->item_len*phd->item_num);
				else if(len >= phd->len)
				{
					memcpy(pmem, (void*)pa->area_adr, phd->len);
					phd = (PCFG_HD)pmem;
					CfgStorAdrRelocate(phd, pa->area_adr, (UI32)pmem);
				}
				else
					return NULL;
			}
		}
		else
			return NULL;
	}
	else if(pa->phy_type == HW_EEPROM_AREA)		//EEPROM读
	{
		if(pmem != NULL)
		{
			if(len >0 && len >= sizeof(CFG_HD))
				phd = pmem;
			else
				return NULL;
		}
		else if(pa->pbuffer == NULL)
		{
			return NULL;
		}
		else
		{
			phd = (PCFG_HD)(pa->pbuffer);
			len = pa->area_size;
		}

		
		EepromRead(pa->area_adr, phd, sizeof(CFG_HD));
		if(phd->id != CFG_HD_ID || phd->len < sizeof(CFG_HD) || phd->len > pa->area_size)
			return NULL;
		if(phd->len < sizeof(CFG_HD) + phd->item_len*phd->item_num)
			return NULL;

		if(rd_type == HWSTOR_RD_ITEM && len>=  sizeof(CFG_HD) + phd->item_len*phd->item_num)
			EepromRead(pa->area_adr+ sizeof(CFG_HD), phd + 1, (phd->item_len*phd->item_num + 1) & 0xfffe);
		else if(len >= phd->len + sizeof(UI16))
		{
			EepromRead(pa->area_adr+ sizeof(CFG_HD), phd + 1, (phd->len -sizeof(CFG_HD) + sizeof(UI16) + 1) & 0xfffe);
			if(*(UI16*)((UI32)phd+ phd->len) == CRC_CCITT16(phd, phd->len))
				CfgStorAdrRelocate(phd, 0, (UI32)phd);
			else
			{
				db_printf("ee read error\r\n");
				return NULL;
			}
		}
		else
			return NULL;
	}

	return phd;			
}

BOOL HWStorSave(const HW_STOR_AREA* pa, PCFG_HD phd)
{
	BOOL ret = FALSE;
	const HW_STOR_AREA* pa2;
	
	//保证长度是4的倍数
	if(phd->len & 0x03)
	{
		phd->len += (4 - (phd->len & 0x03));
	}
	
	if(phd == NULL || pa == NULL|| phd->len + 2 > pa->area_size)
		return FALSE;

	pa2 = hw_stor_backup + pa->id;	//pa2指向当前pa所对应的备份表的首地址
	
	if(pa->phy_type == HW_FLASH_AREA)
	{
		//先对整片删区进行擦除
		if(!FlashErase(pa->area_adr))
			return FALSE;
		
		CfgStorAdrRelocate(phd, (UI32)phd, pa->area_adr);
		*(UI16*)((UI32)phd+ phd->len)= CRC_CCITT16(phd, phd->len);
		ret = FlashWrite(pa->area_adr, phd, phd->len + sizeof(UI16));
		CfgStorAdrRelocate(phd, pa->area_adr, (UI32)phd);
		/*Check flash write is ok*/
		if((hw_stor_st[pa->id].phd = HWStorLoad(pa, HWSTOR_RD_ALL, NULL, 0)) != NULL)
			hw_stor_st[pa->id].chk_st = HWSTOR_OK;
		else
			hw_stor_st[pa->id].chk_st = HWSTOR_CHKFAIL;


		//再对对应的备份区进行数据的擦除和写入    20211110
		if(!FlashErase(pa->area_adr + OFFSET_backup))
			return FALSE;
		
		CfgStorAdrRelocate(phd, (UI32)phd, pa->area_adr + OFFSET_backup);	//此处是将在RAM中的目录项中的关于数据的那个地址指针，重新改编，让他指向flash中
		*(UI16*)((UI32)phd+ phd->len)= CRC_CCITT16(phd, phd->len);	//通过当前的数据和数据长度，算出crc校验位，并放在最后一个位置上，此处还是在RAM中
		ret = FlashWrite(pa->area_adr + OFFSET_backup, phd, phd->len + sizeof(UI16));	//将RAM中的数据写入都falsh当中，并返回一个值，为1代表写入成功，为0代表写入失败
		CfgStorAdrRelocate(phd, pa->area_adr + OFFSET_backup, (UI32)phd);	//此处是将在flash目录项中的关于数据的地址指针，重新改编，并让他指向RAM中。

		//回读检查数据是否正确
		if((hw_stor_st_backup[pa2->id].phd = HWStorLoad(pa2, HWSTOR_RD_ALL, NULL, 0)) != NULL)
			hw_stor_st_backup[pa2->id].chk_st = HWSTOR_OK;
		else
			hw_stor_st_backup[pa2->id].chk_st = HWSTOR_CHKFAIL;
	}
	else if(pa->phy_type == HW_EEPROM_AREA)
	{
		if(pa->pbuffer == NULL)
			return FALSE;

		if((PCFG_HD)pa->pbuffer != phd)
			memcpy((PCFG_HD)pa->pbuffer, phd, phd->len);
		CfgStorAdrRelocate((PCFG_HD)(pa->pbuffer), (UI32)phd, 0);
		phd = (PCFG_HD)(pa->pbuffer);
		*(UI16*)((UI32)phd+ phd->len) = CRC_CCITT16(phd, phd->len);
		ret = EepromWrite(pa->area_adr, pa->pbuffer, phd->len + sizeof(UI16));
		CfgStorAdrRelocate((PCFG_HD)(pa->pbuffer), 0, (UI32)(pa->pbuffer));
		hw_stor_st[pa->id].chk_st = HWSTOR_OK;
		hw_stor_st[pa->id].phd = (PCFG_HD)(pa->pbuffer);
	}

	return ret;
}

PCFG_ITEM CfgDbFindItem(PCFG_HD phd, PCFG_ITEM pitem)
{
	int i;
	PCFG_ITEM pn;
	UI16 type;
	
	if(phd == NULL || pitem == NULL)
		return NULL;

	pn = (PCFG_ITEM)(phd + 1);
	type = pitem->item_type;
	for(i = 0; i < phd->item_num; i++)
	{
		if(pn->item_type == type)
		{
			if(type == PART_ITEM)
			{
				PPART_CFG_IND p1, p2;

				p1 = &pn->item.part_ind;
				p2 = &pitem->item.part_ind;
				if(p1->part_id == p2->part_id && p1->sub_id == p2->sub_id && (p1->offset <= p2->offset && p1->offset + p1->len > p2->offset ||
				  (p1->offset > p2->offset && p2->offset + p2->len >= p1->offset) && p2->len > 0))
					return pn;
			}
			else if(type == TBL_ITEM)
			{
				if(pn->item.tbl_ind.hd.type == pitem->item.tbl_ind.hd.type && pn->item.tbl_ind.hd.sn == pitem->item.tbl_ind.hd.sn)
					return pn;
			}
		}

		pn = (PCFG_ITEM)((UI32)pn + phd->item_len);
	}

	return NULL;
}

/*P1为在存储区的存储项目*/
BOOL CfgDbItemIsSame(PCFG_ITEM p1, PCFG_ITEM p2)
{
	UI16 type;
	int i;
	UI8* pn1;
	UI8* pn2;
	UI16 len1, len2;
	UI16 row;
	
	if(p1 != NULL && p2 != NULL && p1->item_type == p2->item_type && p1->item.part_ind.psave != NULL && p2->item.part_ind.psave != NULL)
	{
		type = p1->item_type;
		if(type == PART_ITEM && p1->item.part_ind.part_id == p2->item.part_ind.part_id && p1->item.part_ind.sub_id == p2->item.part_ind.sub_id && p1->item.part_ind.offset == p2->item.part_ind.offset &&
			p1->item.part_ind.len == p2->item.part_ind.len && memcmp(p1->item.part_ind.psave, p2->item.part_ind.psave, p1->item.part_ind.len*sizeof(UI16)) == 0)
			return TRUE;
		else if(type == TBL_ITEM && p1->item.tbl_ind.hd.row == p2->item.tbl_ind.hd.row && p1->item.tbl_ind.hd.sn == p2->item.tbl_ind.hd.sn && p1->item.tbl_ind.hd.row_len <= p2->item.tbl_ind.hd.row_len)
		{
			if(p1->item.tbl_ind.hd.row_len == p2->item.tbl_ind.hd.row_len)
				return (memcmp(p1->item.tbl_ind.psave, p2->item.tbl_ind.psave, p1->item.tbl_ind.hd.row_len*p1->item.tbl_ind.hd.row) == 0);

			pn1 = p1->item.tbl_ind.psave;
			pn2 = p2->item.tbl_ind.psave;
			len1 =p1->item.tbl_ind.hd.row_len;
			len2 =p2->item.tbl_ind.hd.row_len;
			row = p1->item.tbl_ind.hd.row;
			for(i = 0; i < row; i++)
			{
				if(memcmp(pn1, pn2, len1) != 0)
					return FALSE;

				pn1 += len1;
				pn2 += len2;
			}

			return TRUE;
		}
	}

	return FALSE;
}

int CfgDbCalItemLen(PCFG_ITEM pit)
{
	int len;
	if(pit == NULL)
		return 0;

	if(pit->item_type == TBL_ITEM)
	{
		len = (int)pit->item.tbl_ind.hd.row_len*pit->item.tbl_ind.hd.row;
	}
	else
	{
		len = pit->item.part_ind.len*sizeof(UI16);
	}

	return len;
}

/*P1为在存储区的存储项目, 返回存储的长度*/
int CfgDbCopyItem(PCFG_ITEM p1, PCFG_ITEM p2)
{
	const TBL_TYPE_INFO* ptinfo;
	UI16 len1;
	UI16 len2;
	int row;
	int i;
	UI8* pn1;
	UI8* pn2;
	
	if(p1 == NULL || p2 == NULL || p1->item.part_ind.psave == NULL || p2->item.part_ind.psave == NULL)
		return 0;

	if(p2->item_type == TBL_ITEM)
	{
		p1->item.tbl_ind.hd = p2->item.tbl_ind.hd;
		ptinfo = TBLGetInfo(p2->item.tbl_ind.hd.type);
		if(ptinfo != NULL)
			p1->item.tbl_ind.hd.row_len = ptinfo->ext_row_len;

		len1= p1->item.tbl_ind.hd.row_len;
		len2 = p2->item.tbl_ind.hd.row_len;
		row = p2->item.tbl_ind.hd.row;
		pn1 = p1->item.tbl_ind.psave;
		pn2 = p2->item.tbl_ind.psave;
		if(len1 == len2)
		{
			i = len1*row;
			memcpy(pn1, pn2, i);
			pn1+= i;
		}
		else
		{
			for(i = 0; i < row; i++)
			{
				memcpy(pn1, pn2, len1);
				pn1 += len1;
				pn2 += len2;
			}
		}

		return (pn1 - (UI8*)p1->item.part_ind.psave);
	}
	else if(p2->item_type == PART_ITEM)
	{
		p1->item.part_ind.part_id = p2->item.part_ind.part_id;
		p1->item.part_ind.sub_id = p2->item.part_ind.sub_id;
		p1->item.part_ind.ver = p2->item.part_ind.ver;
		p1->item.part_ind.offset = p2->item.part_ind.offset;
		p1->item.part_ind.len = p2->item.part_ind.len;
		len1 = p2->item.part_ind.len*sizeof(UI16);
		memcpy(p1->item.part_ind.psave, p2->item.part_ind.psave, len1);
		return len1;
	}

	return 0;
}

/*配置项目更新到存储缓存中, phd已拷贝到内存中的存储缓存数据， pitem要更新的存储项数据
stor_size为phd指向的存储数据缓存大小，如0表示不检测是否超出存储缓存。
返回值: TRUE 表示更新成功，FALSE表示失败或数据相同无需更新*/
BOOL CfgDbCacheUpdateItem(PCFG_HD ph, PCFG_ITEM pitem, int stor_size)
{
	PCFG_ITEM pn;

	/*先查找，如果已有存储项且数据相同就不更新*/
	pn = CfgDbFindItem(ph, pitem);
	if(pn != NULL && CfgDbItemIsSame(pn, pitem))
		return FALSE;

	if(pn == NULL)
	{
		
		/*插入一个存储项索引*/
		pn = (PCFG_ITEM)((UI32)(ph + 1) + ph->item_num*ph->item_len);

		if(stor_size != 0 && ph->item_len + CfgDbCalItemLen(pitem) + ph->len > stor_size)
		{
			/*超出存储缓存*/
			return FALSE;
		}
		
		/*修改目录项存储偏移*/
		CfgStorAdrRelocate(ph, 0, ph->item_len);
		/*原存储的数据移动一个目录项长度偏移，由于内存重叠，使用memmove*/
		memmove((PCFG_ITEM)((UI32)pn + ph->item_len), pn, ph->len - (((UI32)pn - (UI32)ph)));
		*pn = *pitem;
		ph->len += ph->item_len;
		pn->item.part_ind.psave = (void*)((UI32)ph + ph->len);
		ph->item_num++;
		ph->len += CfgDbCopyItem(pn, pitem);
	}
	else
	{
		/*已有存储项，但数据不一样，需要更新*/
		int diff_len;
		UI8* pn1;
		PCFG_ITEM pt;
		int i;
		int len;
		BOOL need_mov;

			
		if(pn->item_type == TBL_ITEM)
		{
//			if(pn->item.tbl_ind.hd.row == 0)
//				db_printf("Error row\r\n");
	
//			if(pitem->item.tbl_ind.hd.row == 0)
//				db_printf("Error row\r\n");

			len = (int)(pn->item.tbl_ind.hd.row*pn->item.tbl_ind.hd.row_len);
			diff_len = (int)(pitem->item.tbl_ind.hd.row*pitem->item.tbl_ind.hd.row_len) - len; 
		}
		else 
		{
			len = (int)pn->item.part_ind.len*sizeof(UI16);
			diff_len = (int)(pitem->item.part_ind.len)*sizeof(UI16) - len;
		}

		if(diff_len + (int)ph->len > stor_size && stor_size != 0)
		{
			/*数据超出存储缓存*/
			return FALSE;
		}
		
		if(diff_len != 0)
		{
			pt = (PCFG_ITEM)(ph + 1);
			pn1 = (UI8*)pn->item.part_ind.psave + len;
			need_mov = FALSE;
			for(i = 0; i < ph->item_num; i++)
			{
				/*修改目录项存储偏移*/
				if((UI8*)pt->item.part_ind.psave >= (UI8*)pn->item.part_ind.psave + len)
				{
					/*当len = 0 ，防止修改自身的存储地址*/
					if(pt != pn)
					{
						pt->item.part_ind.psave = (void*)((I32)pt->item.part_ind.psave + diff_len);
						need_mov = TRUE;
					}
				}
				pt= (PCFG_ITEM)((UI32)pt+ ph->item_len);
			}
			/*原存储区中在pn存储项目以后的数据要移动diff_len偏移, 如diff_len > 0 有数据重叠*/
			if(ph->len < (pn1 - (UI8*)ph))
			{
				db_printf("error len\r\n");
				SetMachineErr(ER0_MEM);

				return FALSE;
			}
			
			if(need_mov)
				memmove(pn1 + diff_len, pn1, ph->len - (pn1 - (UI8*)ph));
		}
		
		CfgDbCopyItem(pn, pitem);
		ph->len = ((int)ph->len + diff_len);
	}

	return TRUE;
}

BOOL CfgDbItemSave(CFG_DB_ID cfg_id,  PCFG_ITEM pitem)
{
	PCFG_HD ph;
	PCFG_HD phd;
	UI16 cache_size;
	const HW_STOR_AREA* pa;
	
	if(pitem == NULL && cfg_id >= CFG_ID_END && cfg_id < CFG_ID_START)
		return FALSE;

	pa = hw_stor + cfg_st[cfg_id].stor_ind;
	if(hw_stor_st[pa->id].chk_st != HWSTOR_OK)
		return FALSE;

	if(hw_stor_st[pa->id].phd == NULL)
		hw_stor_st[pa->id].phd = HWStorLoad(pa, HWSTOR_RD_ALL, NULL, 0);

	if((phd = hw_stor_st[pa->id].phd) == NULL)
		return FALSE;

	/*如存储区有缓存，已把数据读入到内存中中，不需要申请缓存，如没有需要申请缓存，把数据拷贝到缓存中，
	EEPROM: 有内存缓存，不需要在重新申请缓存
	FLASH: 没有缓存，只有只读的FLASH映射，需要申请缓存*/
	if(pa->pbuffer == NULL || pa->pbuffer != (UI8*)phd)
	{
		if((ph = (PCFG_HD)GetCache(&cache_size)) == NULL)
			return FALSE;

		if(cache_size > phd->len + 2)
		{
			memcpy(ph, phd, phd->len);
			CfgStorAdrRelocate(ph, (UI32)phd, (UI32)ph); 
		}
		else
		{
			FreeCache(ph);
			return FALSE;
		}
	}
	else 
	{
		ph = phd;
	}
	
	if(CfgDbCacheUpdateItem(ph, pitem, 0))
	{
		HWStorSave(pa, ph);
	}

	/*释放申请的缓存*/
	if(ph != pa->pbuffer)
		FreeCache((UI8*)ph);

	return TRUE;
}

BOOL CfgDbPartSaveL(PPART pp, UI16 sub_id, UI16 len, UI16 offset, CFG_DB_ID db_id)
{
	CFG_ITEM cfg;

	cfg_save_list.lastsavetime = 0;//每次运行完写Flash操作时，初始化该时间

	if(pp == NULL || sub_id >= ARRAY_NUM(pp->psub) || pp->psub[sub_id] == NULL ||offset >= pp->sub_len[sub_id])
		return FALSE;

	/*部件的用户数据在USER_CFG_DB中*/
	if(len + offset > pp->sub_len[sub_id])
		len = pp->sub_len[sub_id] - offset;

	cfg.item_type = PART_ITEM;
	cfg.item.part_ind.part_id = pp->id;
	cfg.item.part_ind.ver = PART_DB_VER;
	cfg.item.part_ind.sub_id = sub_id;
	
	if(len > 0)
		cfg.item.part_ind.len = len;
	else
		cfg.item.part_ind.len = pp->sub_len[sub_id];
		
	cfg.item.part_ind.offset= offset;
	cfg.item.part_ind.psave= pp->psub[sub_id] + offset;
	
	return CfgDbItemSave(db_id, &cfg);
}

BOOL CfgDbPartSave(PPART pp, UI16 sub_id, UI16 len, UI16 offset, CFG_DB_ID db_id)
{
	return SetListSaveCfgDb(pp, sub_id, len, offset, db_id);
}


BOOL CfgDbTblSave(UI16 type, UI8 sn, UI16 row, UI16 row_len, void*prow, CFG_DB_ID db_id)
{
	CFG_ITEM cfg;

	if(prow == NULL)
		return FALSE;

	cfg.item_type = TBL_ITEM;
	cfg.item.tbl_ind.hd.type = type;
	cfg.item.tbl_ind.hd.row = row;
	cfg.item.tbl_ind.hd.row_len= row_len;
	cfg.item.tbl_ind.hd.ver = TBL_DB_VER;
	cfg.item.tbl_ind.hd.sn= sn;
	cfg.item.tbl_ind.psave = prow;

	return CfgDbItemSave(db_id, &cfg);
}

const CFG_ITEM* CfgDbReadItem(CFG_DB_ID cfg_id,  PCFG_ITEM pitem)
{
	PCFG_HD phd;
	const HW_STOR_AREA* pa;
	
	if(pitem == NULL && cfg_id >= CFG_ID_END && cfg_id < CFG_ID_START)
		return NULL;

	pa = hw_stor + cfg_st[cfg_id].stor_ind;
	if(hw_stor_st[pa->id].chk_st != HWSTOR_OK)
		return NULL;

	if(hw_stor_st[pa->id].phd == NULL)
		hw_stor_st[pa->id].phd = HWStorLoad(pa, HWSTOR_RD_ALL, NULL, 0);

	if((phd = hw_stor_st[pa->id].phd) == NULL)
		return NULL;

	return CfgDbFindItem(phd, pitem);
}

const PART_CFG_IND* CfgDbPartRead(PPART pp, UI16 sub_id, UI16 len, UI16 offset, CFG_DB_ID db_id)
{
	CFG_ITEM cfg;
	const CFG_ITEM* pret;
	
	if(pp == NULL || sub_id >= ARRAY_NUM(pp->psub) || pp->psub[sub_id] == NULL ||offset > pp->sub_len[sub_id])
		return FALSE;

	/*部件的用户数据在USER_CFG_DB中*/
	if(len + offset > pp->sub_len[sub_id])
		len = pp->sub_len[sub_id] - offset;

	cfg.item_type = PART_ITEM;
	cfg.item.part_ind.part_id = pp->id;
	cfg.item.part_ind.ver = PART_DB_VER;
	cfg.item.part_ind.sub_id = sub_id;

	if(len > 0)
		cfg.item.part_ind.len = len;
	else
		cfg.item.part_ind.len = pp->sub_len[sub_id];

	cfg.item.part_ind.offset= offset;
	cfg.item.part_ind.psave= pp->psub[sub_id];
	
	if((pret = CfgDbReadItem(db_id, &cfg)) != NULL)
		return (&(pret->item.part_ind));
	else 
		return NULL;
}

const TBL_CFG_IND* CfgDbTblRead(UI16 type, UI8 sn, CFG_DB_ID db_id)
{
	CFG_ITEM cfg;
	const CFG_ITEM* pret;
	
	cfg.item_type = TBL_ITEM;
	cfg.item.tbl_ind.hd.type = type;
	cfg.item.tbl_ind.hd.ver = TBL_DB_VER;
	cfg.item.tbl_ind.hd.sn= sn;
	
	if((pret = CfgDbReadItem(db_id, &cfg)) != NULL)
		return (&(pret->item.tbl_ind));
	else 
		return NULL;
}

void InitCfgHD(PCFG_HD phd, UI8 type)
{
	if(phd != NULL)
	{
		phd->id = CFG_HD_ID;
		phd->ver = CFG_HD_VER;
		phd->len = sizeof(CFG_HD);
		phd->type = type;
		phd->item_num = 0;
		phd->item_len = sizeof(CFG_ITEM);
	}
}

/*把出厂部件配置数据拷贝到用户部件配置区*/
void UserPartCfgReset()
{
	PCFG_HD phd;
	UI16 len;
	
	phd = (PCFG_HD)GetCache(&len);
	if(phd == NULL)
		return;
	
	InitCfgHD(phd, USER_PART_DB);
	HWStorLoad(hw_stor + cfg_st[FACTORY_PART_DB].stor_ind, HWSTOR_RD_ALL, phd, len);
	HWStorSave(hw_stor + cfg_st[USER_PART_DB].stor_ind, phd);
	FreeCache(phd);
}


/*原始区数据与备份区数据，先读到RAM中，再复制到备份区或原始区*/
void Backup_CfgReset(UI16 i,UI16 direct)
{
	PCFG_HD phd;
	UI16 len;
	
	phd = (PCFG_HD)GetCache(&len);
	if(phd == NULL)
		return;
	
	InitCfgHD(phd, i);
	if(direct == 0)		//为0，代表从原始区读取 数据
		HWStorLoad(hw_stor + i, HWSTOR_RD_ALL, phd, len);
	else				//非0，代表从备份区读取数据
		HWStorLoad(hw_stor_backup + i, HWSTOR_RD_ALL, phd, len);
	
	HWStorSave(hw_stor + i, phd);	//写flash，每次都写原始区，这个函数里面，默认会加上对备份区的写
	FreeCache(phd);
}


/*把用户部件配置数据拷贝到出厂部件配置区*/
void FactoryPartCfgReset()
{
	PCFG_HD phd;
	UI16 len;
	
	phd = (PCFG_HD)GetCache(&len);
	if(phd == NULL)
		return;
	
	InitCfgHD(phd, FACTORY_PART_DB);
	HWStorLoad(hw_stor + cfg_st[USER_PART_DB].stor_ind, HWSTOR_RD_ALL, phd, len);
	HWStorSave(hw_stor + cfg_st[FACTORY_PART_DB].stor_ind, phd);
	FreeCache(phd);
}

/*把用户可以修改的表从表出厂配置区拷贝到用户表配置区*/
void UserTblCfgReset()
{
	PCFG_ITEM pit;
	PCFG_HD phd;
	const TBL_TYPE_INFO* pinfo;
	PTBL ptbl;
	UI8 stor_id;
	UI8* pend;
	UI16 item_len;
	UI16 item_num;

	phd = (PCFG_HD)GetCache(NULL);
	if(phd == NULL)
		return;

	InitCfgHD(phd, USER_TBL_DB);
	HWStorSave(hw_stor + cfg_st[USER_TBL_DB].stor_ind, phd);
	FreeCache(phd);
	
	stor_id = cfg_st[FACTORY_TBL_DB].stor_ind;
	phd = hw_stor_st[stor_id].phd;
	if(stor_id < ARRAY_NUM(hw_stor_st) && phd != NULL && hw_stor_st[stor_id].chk_st == HWSTOR_OK)
	{
		pit = (PCFG_ITEM)(hw_stor_st[stor_id].phd + 1);
		item_len= hw_stor_st[stor_id].phd->item_len;
		item_num= hw_stor_st[stor_id].phd->item_num;
		pend = (UI8*)pit + item_len*item_num;  
		while((UI8*)pit < pend)
		{
			if(pit->item_type == TBL_ITEM)
			{
				ptbl = (PTBL)&(pit->item.tbl_ind);
				pinfo = TBLGetInfo(ptbl->hd.type);
				if(pinfo != NULL && (pinfo->attr_bits & TBL_USER))
					CfgDbTblSave(ptbl->hd.type, ptbl->hd.sn, ptbl->hd.row, ptbl->hd.row_len, ptbl->prow, USER_TBL_DB);
			}
			pit = (PCFG_ITEM)((UI32)pit + item_len);
		}
	}

	return;
}

void CfgDbSetDefault()
{
	PCFG_HD		phd;
	UI8* psave;
	UI8* pnow;

	//db_printf("Reset factory CFG\r\n");
	phd = (PCFG_HD)GetCache(NULL);
	InitCfgHD(phd, FACTORY_PART_DB);

	psave = (UI8*)(phd + 1) + sizeof(CFG_ITEM)*MAX_CFG_ITEM_NUM;
	pnow = SetPartCfgDefault(phd, psave, MAX_CFG_ITEM_NUM);
	if(phd->item_num < MAX_CFG_ITEM_NUM)
	{
		CfgStorAdrRelocate(phd, (UI32)psave, (UI32)(psave - sizeof(CFG_ITEM)*(MAX_CFG_ITEM_NUM - phd->item_num)));
		memcpy(psave - sizeof(CFG_ITEM)*(MAX_CFG_ITEM_NUM - phd->item_num), psave, pnow -psave);
	}
	phd->len = (pnow -psave) + sizeof(CFG_HD)  + sizeof(CFG_ITEM)*(phd->item_num) ;
	/*初始化部件出厂配置*/
	HWStorSave(hw_stor + cfg_st[FACTORY_PART_DB].stor_ind, phd);
	phd->type = USER_PART_DB;
	/*重置部件用户配置*/
	HWStorSave(hw_stor + cfg_st[USER_PART_DB].stor_ind, phd);

	//phd = (PCFG_HD)GetCache(NULL);
	InitCfgHD(phd, FACTORY_TBL_DB);
	psave = (UI8*)(phd + 1) + sizeof(CFG_ITEM)*MAX_CFG_ITEM_NUM;
	pnow = SetTblCfgDefault(phd, psave, MAX_CFG_ITEM_NUM);
	if(phd->item_num < MAX_CFG_ITEM_NUM)
	{
		CfgStorAdrRelocate(phd, (UI32)psave, (UI32)(psave - sizeof(CFG_ITEM)*(MAX_CFG_ITEM_NUM - phd->item_num)));
		memcpy(psave - sizeof(CFG_ITEM)*(MAX_CFG_ITEM_NUM - phd->item_num), psave, pnow -psave);
	}
	phd->len = (pnow -psave) + sizeof(CFG_HD)  + sizeof(CFG_ITEM)*(phd->item_num) ;
	HWStorSave(hw_stor + cfg_st[FACTORY_TBL_DB].stor_ind, phd);
	FreeCache((UI8*)phd);
	UserTblCfgReset();
}


/*把用户可以修改的表从表用户配置区拷贝到出厂表配置区*/
void FactoryTblCfgReset()
{
	PCFG_ITEM pit;
	PCFG_HD phd;
	const TBL_TYPE_INFO* pinfo;
	PTBL ptbl;
	UI8 stor_id;
	UI8* pend;
	UI16 item_len;
	UI16 item_num;

//	phd = (PCFG_HD)GetCache(NULL);
//	if(phd == NULL)
//		return;

//	InitCfgHD(phd, FACTORY_TBL_DB);
//	HWStorSave(hw_stor + cfg_st[FACTORY_TBL_DB].stor_ind, phd);
//	FreeCache(phd);
	
	stor_id = cfg_st[USER_TBL_DB].stor_ind;
	phd = hw_stor_st[stor_id].phd;
	if(stor_id < ARRAY_NUM(hw_stor_st) && phd != NULL && hw_stor_st[stor_id].chk_st == HWSTOR_OK)
	{
		pit = (PCFG_ITEM)(hw_stor_st[stor_id].phd + 1);
		item_len= hw_stor_st[stor_id].phd->item_len;
		item_num= hw_stor_st[stor_id].phd->item_num;
		pend = (UI8*)pit + item_len*item_num;  
		while((UI8*)pit < pend)
		{
			if(pit->item_type == TBL_ITEM)
			{
				ptbl = (PTBL)&(pit->item.tbl_ind);
				pinfo = TBLGetInfo(ptbl->hd.type);
				if(pinfo != NULL && (pinfo->attr_bits & TBL_USER))
					CfgDbTblSave(ptbl->hd.type, ptbl->hd.sn, ptbl->hd.row, ptbl->hd.row_len, ptbl->prow, FACTORY_TBL_DB);
			}
			pit = (PCFG_ITEM)((UI32)pit + item_len);
		}
	}

	return;
}

void ChkHWStor()
{
	PMACHINE    pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1));
	PCFG_ST 	pst,pst2;
	UI8 failcnt = 0,failcnt_2 = 0;
	UI16 i = 0;
	
	pst = cfg_st;
	while(pst < cfg_st + ARRAY_NUM(cfg_st))
	{
		if(pst->stor_ind >= ARRAY_NUM(hw_stor))
			pst->stor_ind = 0;

		if(hw_stor_st[pst->stor_ind].chk_st >= HWSTOR_CHKFAIL)
		{
			if((hw_stor_st[pst->stor_ind].phd = HWStorLoad(hw_stor + pst->stor_ind, HWSTOR_RD_ALL, NULL, 0)) != NULL)
			{
				failcnt = 4;
				hw_stor_st[pst->stor_ind].chk_st = HWSTOR_OK;

				db_printf("Flash Data About: TYPE %d, len %d, item_len %d, item_num %d;\r\n",hw_stor_st[pst->stor_ind].phd->type,hw_stor_st[pst->stor_ind].phd->len,hw_stor_st[pst->stor_ind].phd->item_len,hw_stor_st[pst->stor_ind].phd->item_num);
			}
			else
			{
				failcnt++;
				if(failcnt > 3 || pst->stor_ind == 0)
				{
					hw_stor_st[pst->stor_ind].chk_st = HWSTOR_CHKFAIL;
					if(pst->stor_ind != 0 && failcnt > 3)
						SET_PART_ERR(pm,ER0_READ_ERR);
				}
				if(pst->stor_ind > 0)
					delay_ms(20);
			}
		}
		else
		{
			hw_stor_st[pst->stor_ind].phd = NULL;
		}
		
		if(failcnt > 3)
		{
			failcnt = 0;
			pst++;
		}
	}

	//读取备份区的数据
	pst2 = cfg_st_backup;
	while(pst2 < cfg_st_backup + ARRAY_NUM(cfg_st_backup))
	{
		if(pst2->stor_ind >= ARRAY_NUM(hw_stor_backup))
			pst2->stor_ind = 0;

		if(hw_stor_st_backup[pst2->stor_ind].chk_st >= HWSTOR_CHKFAIL)
		{
			if((hw_stor_st_backup[pst2->stor_ind].phd = HWStorLoad(hw_stor_backup + pst2->stor_ind, HWSTOR_RD_ALL, NULL, 0)) != NULL)
			{
				failcnt_2 = 4;
				hw_stor_st_backup[pst2->stor_ind].chk_st = HWSTOR_OK;
			}
			else
			{
				failcnt_2++;
				if(failcnt_2 > 3 || pst2->stor_ind == 0)
				{
					hw_stor_st_backup[pst2->stor_ind].chk_st = HWSTOR_CHKFAIL;
				}
				if(pst2->stor_ind > 0)
					delay_ms(20);
			}
		}
		else
		{
			hw_stor_st_backup[pst2->stor_ind].phd = NULL;
		}
		
		if(failcnt_2 > 3)
		{
			failcnt_2 = 0;
			pst2++;
		}
	}

	//将原始区和备份区的数据进行对比，根据读取是否成功的标记
	for(i=1; i<CFG_ID_END; i++)
	{
		if(hw_stor_st[i].chk_st == HWSTOR_OK)		//原始区数据正确时
		{
			if(hw_stor_st_backup[i].chk_st == HWSTOR_OK)
			{
				//两个区都对，不做任何处理，此处crc不做判断处理，因为地址指针的缘故，CRC检验位肯定不一样
			}
			else		//备份区数据错误时，将原始区数据拷贝到备份区
			{
				Backup_CfgReset(i,0);	//拷贝原始到备份，chk_st标记这里不需要在写了，在里面的写flash处，会重读
			}
		}
		else
		{
			if(hw_stor_st_backup[i].chk_st == HWSTOR_OK)	//原始区数据错误，备份区数据对的时候，将备份区的数据拷贝到原始区
			{
				Backup_CfgReset(i,1);	//拷贝备份区数据到原始区，chk_st标记这里不需要在写了，在里面的写flash处，会重读
			}
			else	//当两个区数据都错误时，这里则不做处理，执行后面CfgDbSetDefault来恢复为默认数据				
			{
			}
		}
	}

	if(hw_stor_st[cfg_st[FACTORY_PART_DB].stor_ind].chk_st != HWSTOR_OK || hw_stor_st[cfg_st[FACTORY_TBL_DB].stor_ind].chk_st != HWSTOR_OK)
	{
		//db_printf("Reset factory CFG\r\n");
		CfgDbSetDefault();
	}
	else if(hw_stor_st[cfg_st[USER_PART_DB].stor_ind].chk_st != HWSTOR_OK)
		UserPartCfgReset();
	else if(hw_stor_st[cfg_st[USER_TBL_DB].stor_ind].chk_st != HWSTOR_OK)
		UserTblCfgReset();
}

/*假设: 用户配置数据只有部件配置数据, 表配置数据和部件出厂数据存分开存储*/
BOOL LoadCfgDb(CFG_DB_ID db_id)
{
	PCFG_ITEM pit;
	const TBL_TYPE_INFO* pinfo;
	PTBL ptbl, ptnew;
	PPART pp;
	UI8 stor_id;
	UI8* pend;
	UI16 item_len;
	UI16 item_num;
	
	if(db_id >= CFG_ID_END)
		return FALSE;
	
	stor_id = cfg_st[db_id].stor_ind;
	/*读取配置数据，如果是表，根据需要在内存创见新表或直接使用FLASH，并把表加入到表索引表中。假设表数据只在表的出厂区*/
	if(stor_id < ARRAY_NUM(hw_stor_st) && hw_stor_st[stor_id].phd != NULL && hw_stor_st[stor_id].chk_st == HWSTOR_OK)
	{
		pit = (PCFG_ITEM)(hw_stor_st[stor_id].phd + 1);
		item_len= hw_stor_st[stor_id].phd->item_len;
		item_num= hw_stor_st[stor_id].phd->item_num;
		pend = (UI8*)pit + item_len*item_num;  
		while((UI8*)pit < pend)
		{
			if(pit->item_type == TBL_ITEM)
			{
				ptbl = (PTBL)&(pit->item.tbl_ind);
				pinfo = TBLGetInfo(ptbl->hd.type);
				if(ptbl->hd.row > 0)
				{
					if((pinfo != NULL && (pinfo->attr_bits & TBL_DYNC)) && TBLGet(ptbl->hd.type,  ptbl->hd.sn,pinfo->ver) == NULL)
					{
						ptnew = TBLAlloc(ptbl->hd.type, pinfo->ver, ptbl->hd.sn, TBL_STOR, pinfo->max_row, pinfo->row_len);
						TBLCopy(ptnew, ptbl);
					}
					else
					{
						AddTblToInd((PTBL)&(pit->item.tbl_ind), db_id, TBL_STOR, 0);
						/*出厂表加入到出厂表索引*/
						if(db_id == FACTORY_TBL_DB)
							AddTblToInd((PTBL)&(pit->item.tbl_ind), db_id, TBL_STOR, 2);
					}
				}
			}
			else if(pit->item_type == PART_ITEM)
			{
				/*这里设置部件数据，要考虑部件是否已经创建。*/
				pp = GetPartByID(pit->item.part_ind.part_id);
				if(pp != NULL)
					PartDataRx(pp, pit->item.part_ind.sub_id, pit->item.part_ind.len, pit->item.part_ind.offset, pit->item.part_ind.psave, INIT_CFG_RX);
			}

			pit = (PCFG_ITEM)((UI32)pit + item_len);
		}
	}

	return TRUE;
}

BOOL BootInitCfgDb()
{
	ChkHWStor();
	LoadCfgDb(FACTORY_TBL_DB);
	LoadCfgDb(USER_TBL_DB);

	return TRUE;
}


/*根据phd内的部件或者表数据更新存储在出厂区和用户区的对应项目数据*/
BOOL CfgDbItemUpdate(PCFG_HD phd)
{
	UI16	c_size;
	int		i;
	PCFG_ITEM pit;
	UI8* pend;
	UI16	item_type;
	BOOL	bneedsave;
	PCFG_HD pt;
	const TBL_TYPE_INFO* ptbl_info;
	
	if(phd == NULL)
		return FALSE;


	if(phd->len < sizeof(CFG_HD) + phd->item_len*phd->item_num)
		return FALSE;
		
	if(*(UI16*)((UI32)phd+ phd->len) != CRC_CCITT16(phd, phd->len))
		return FALSE;

	if((pt = (PCFG_HD)GetCache(&c_size))== NULL)
		return FALSE;

	/*原偏移地址为，重定位到当前内存地址*/
	CfgStorAdrRelocate(phd, 0, (UI32)phd);

 	pend = (UI8*)(phd + 1) + phd->item_len*phd->item_num;  
	for(i = CFG_ID_START; i < CFG_ID_END; i++)
	{
		bneedsave = FALSE;
		if(HWStorLoad(hw_stor + cfg_st[i].stor_ind, HWSTOR_RD_ALL, pt, (int)c_size) == NULL)
			continue;

		if(i == FACTORY_PART_DB || i == USER_PART_DB)
		{
			item_type = PART_ITEM;
		}
		else if(i == FACTORY_TBL_DB || i == USER_TBL_DB)
		{
			item_type = TBL_ITEM;
		}
		else
		{
			continue;
		}

		pit = (PCFG_ITEM)(phd + 1);
		while((UI8*)pit < pend)
		{
			if(pit->item_type == item_type)
			{
				BOOL bneedupdate = TRUE;
				
				if(pit->item_type == TBL_ITEM)
				{
					ptbl_info = TBLGetInfo(pit->item.tbl_ind.hd.type);
					if(i == USER_TBL_DB)
						bneedupdate = (ptbl_info->attr_bits & TBL_USER);
				}

				if(bneedupdate && (UI32)(pit->item.part_ind.psave) <= (UI32)phd + phd->len)
				{
					if(CfgDbCacheUpdateItem(pt, pit, 0))
						bneedsave = TRUE;
				}
			}

			pit = (PCFG_ITEM)((UI8*)pit + phd->item_len);
 		}

		if(bneedsave)
		{
			HWStorSave(hw_stor + cfg_st[i].stor_ind, pt);
		}
	}

	FreeCache(pt);
	
	return TRUE;
}

BOOL CfgDbUpdate(PPACK_PKT_RX_ST pst)
{
	PCFG_HD phd;
	UI8	stor_id;
	UI16 len;
	UI32 offset;
	CFG_DB_ID cfgdb_id;
	
	if(pst == NULL)
		return FALSE;

	if(pst->cache_size < sizeof(CFG_HD) || pst->pcache == NULL || pst->cur_len < pst->cache_fill)
		return FALSE;

	offset = pst->cur_len - pst->cache_fill;
	phd = (PCFG_HD)pst->pcache;
	if(offset == 0)
	{
		/*第一块数据*/
		if((phd->id != CFG_HD_ID && phd->id != CFG_HD_ID + 1 && phd->id != CFG_HD_ID - 1) || phd->ver != CFG_HD_VER || phd->item_len != sizeof(CFG_ITEM) || phd->len < sizeof(CFG_HD) + phd->item_num*phd->item_len)
			return FALSE;

		cfgdb_id = (CFG_DB_ID)phd->type;
		if((cfgdb_id < CFG_ID_START || cfgdb_id >= CFG_ID_END) && cfgdb_id != 0)
			return FALSE;
		
		len = phd->item_len*phd->item_num + sizeof(CFG_HD);
		if(pst->cache_fill < len || phd->len < len || pst->total_len < len + 2)
			return FALSE;

		if(cfgdb_id == 0)
		{
			/*非整个存储区更新，需要一次读取整个更新文件*/
			if(pst->cache_fill >= phd->len)
			{
				/*部件数据或表进行更新，而不是针对整个存储区域*/
				CfgDbItemUpdate(phd);
			}
			else
			{
				return FALSE;
			 	
			}
		}
		
		stor_id = cfg_st[cfgdb_id].stor_ind;
		update_st.cur_crc16 = 0;
		update_st.org_crc16 = 0;
		update_st.cfgdb_id = cfgdb_id;
		update_st.need_len = phd->len + 2;	//最后2字节为CRC16

		if(update_st.need_len > hw_stor[stor_id].area_size)
			return FALSE;
		
		HWStorErase(hw_stor + stor_id);
		if(cfgdb_id == FACTORY_TBL_DB)
			HWStorErase(hw_stor + cfg_st[USER_TBL_DB].stor_ind);
	}
	else
		cfgdb_id = (CFG_DB_ID)update_st.cfgdb_id;
	
	if(update_st.need_len <= 0)
		return FALSE;
	
	stor_id = cfg_st[cfgdb_id].stor_ind;
	len = 0;
	if(update_st.need_len > 2)
	{
		if(pst->cache_fill > update_st.need_len -2)
			len = update_st.need_len - 2;
		else 
			len = pst->cache_fill;

		update_st.org_crc16 = CRC_BCB(phd, len, update_st.org_crc16);
		if(offset == 0)
		{
			phd->type = hw_stor[cfg_st[stor_id].stor_ind].phy_type;
			phd->id = CFG_HD_ID;
			if(phd->type == HW_FLASH_AREA)
				CfgStorAdrRelocate(phd, 0, hw_stor[stor_id].area_adr);
		}
		
		update_st.cur_crc16 = CRC_BCB(phd, len, update_st.cur_crc16);
		HWStorWrite(hw_stor + stor_id, pst->pcache, len, offset);

		update_st.need_len -= len;
	}
	
	if(update_st.need_len == 2 && pst->cache_fill > len)
	{
		update_st.tail_crc16 = pst->pcache[len];
		update_st.need_len--;	
		len++;
	}
		
	if(update_st.need_len == 1 && pst->cache_fill > len)
	{
		update_st.tail_crc16 = update_st.tail_crc16 + (((UI16)pst->pcache[len]) << 8);
		update_st.need_len--;	
		len++;
	}

	if(update_st.need_len == 0)
	{
		/*finish update*/
		if(update_st.org_crc16 != update_st.tail_crc16)
			HWStorErase(hw_stor + stor_id);			/*crc error*/
		else
		{
			/*crc ok, update succese*/
			HWStorWrite(hw_stor + stor_id, &update_st.cur_crc16, 2, offset + len - 2);
			hw_stor_st[stor_id].chk_st = HWSTOR_OK;
			FreeCache(pst->pcache);
			if(cfgdb_id == FACTORY_TBL_DB)
			{
				UserTblCfgReset();
				SetRunFlag(INIF_CFGDBCHG);
			}
			else if(cfgdb_id == FACTORY_PART_DB)
				UserPartCfgReset();
			pst->pcache = GetCache(NULL);
		}
		/*Rx End*/
		return FALSE;
	}

	return TRUE;
}

BOOL CfgDbUploadAll(CFG_DB_ID cfgdb_id, PPACK_PKT_TX_ST pst)
{
	PCFG_HD phd;
	PCFG_HD pt;
	int 	i;
	PCFG_ITEM pit;
	UI8* 	pend;
	
	if(pst == NULL)
		return FALSE;

	if(pst->pcache == NULL)
		return FALSE;
	
	if((pt = HWStorLoad(hw_stor + cfg_st[FACTORY_PART_DB].stor_ind, HWSTOR_RD_ALL, pst->pcache, pst->cache_size)) == NULL)
		return FALSE;
	
	for(i = FACTORY_PART_DB + 1; i < CFG_ID_END; i++)
	{
		if((phd = HWStorLoad(hw_stor + cfg_st[i].stor_ind, HWSTOR_RD_ALL, NULL, 0)) != NULL)
		{
			pit = (PCFG_ITEM)(phd + 1);
			pend = (UI8*)(phd + 1) + phd->item_len*phd->item_num;

			while((UI8*)pit < pend)
			{
				if((UI32)(pit->item.part_ind.psave) <= (UI32)phd + phd->len)
				{
					/*需要检查数据是否会超出缓存大小*/
					CfgDbCacheUpdateItem(pt, pit, pst->cache_size);
				}

				pit = (PCFG_ITEM)((UI8*)pit + phd->item_len);
			}
		}
	}

	pt->type = cfgdb_id;
	CfgStorAdrRelocate(pt, (UI32)pt, 0);
	*(UI16*)((UI32)pt+ pt->len) = CRC_CCITT16(pt, pt->len);
	pst->len = pt->len + 2;
	return TRUE;
}

BOOL CfgDbUpload(CFG_DB_ID cfgdb_id, PPACK_PKT_TX_ST pst)
{
	PCFG_HD phd;
	
	if(pst == NULL)
		return FALSE;

	if(cfgdb_id == 0)
	{
		/*上传合并的配置文件*/
		return CfgDbUploadAll(cfgdb_id, pst);
	}
		
	if(cfgdb_id < CFG_ID_START || cfgdb_id >= CFG_ID_END || pst->cache_size < sizeof(CFG_HD) || pst->pcache == NULL)
		return FALSE;

	if((phd = HWStorLoad(hw_stor + cfg_st[cfgdb_id].stor_ind, HWSTOR_RD_ALL, pst->pcache, pst->cache_size)) != NULL)
	{
		phd->type = cfgdb_id;
		CfgStorAdrRelocate(phd, (UI32)phd, 0);
		*(UI16*)((UI32)phd+ phd->len) = CRC_CCITT16(phd, phd->len);
		pst->len = phd->len + 2;
		return TRUE;
	}

	return FALSE;
}


void CfgDbUpdateFail(CFG_DB_ID cfgdb_id, PPACK_PKT_RX_ST pst)
{
	
}


/*存储面板主机参数文件到FLASH的最后一块*/
BOOL MMIParaFileSave(PPACK_PKT_RX_ST pst)
{
	BOOL ret;
	MMI_PARA_FILE_HD file_hd;
	
	if(pst == NULL || pst->pcache == NULL)
		return FALSE;

	/*文件是否超过存储区大小*/
	if(pst->total_len >= FALSH_ENDBLK_SIZE- MMI_PARA_FILE_OFFSET)
		return FALSE;

	if(pst->cur_len - pst->cache_fill == 0)
	{
		/*第一包数据，先删除FLASH*/
		if(!FlashErase(FLASH_ENDBLK_ADR))
		{
			return FALSE;
		}
	}


	ret = FlashWrite(FLASH_ENDBLK_ADR +  MMI_PARA_FILE_OFFSET + (pst->cur_len - pst->cache_fill), pst->pcache, pst->cache_fill);

	if(ret == TRUE)
	{
		if(pst->cur_len >= pst->total_len)
		{
			/*全部文件数据已收完并已写入到FLASH，写入文件的FLASH头部信息*/
			file_hd.file_id = MMI_PARA_FILE_HD_ID;
			file_hd.file_len = pst->total_len;
			file_hd.rev = 0;
			file_hd.crc_16 = CRC_CCITT16((void*)(FLASH_ENDBLK_ADR + MMI_PARA_FILE_OFFSET), (int)pst->total_len);
			ret = FlashWrite(FLASH_ENDBLK_ADR, &file_hd, sizeof(file_hd));
		}
	}

	return ret;
}

BOOL MMIParaFileRead(PPACK_PKT_TX_ST pst)
{
	UI16 crc_16;
	PMMI_PARA_FILE_HD phd;
	
	if(pst == NULL)
		return FALSE;

	phd = (PMMI_PARA_FILE_HD)(FLASH_ENDBLK_ADR);
	pst->pcache = (UI8*)(FLASH_ENDBLK_ADR + MMI_PARA_FILE_OFFSET);
	pst->cache_size = phd->file_len;
	pst->len = phd->file_len;

	/*检查文件头和数据校验*/
	if(phd->file_id != MMI_PARA_FILE_HD_ID || phd->file_len == 0 || phd->file_len >= FALSH_ENDBLK_SIZE- MMI_PARA_FILE_OFFSET)
	{
		/*长度设0，表示无文件或校验错*/
		pst->len = 0;
		db_printf("file tag error or length error\r\n");
	}
	else
	{
		crc_16 = CRC_CCITT16((void*)(FLASH_ENDBLK_ADR + MMI_PARA_FILE_OFFSET), phd->file_len);
		if(crc_16 != phd->crc_16)
		{
			/*长度设0，表示无文件或校验错*/
			pst->len = 0;
			db_printf("file crc error\r\n");
		}
	}
	
	return TRUE;
}
