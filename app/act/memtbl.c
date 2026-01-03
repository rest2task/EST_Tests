/******************************************************************************
  文 件 名   : memtbl.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 简单内存分配功能
  函数列表   :
              AllocMemTbl
              InitMemTbl
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

#include "typedef.h"
#include "string.h"
#include "memtbl.h"
#include "hwcfg.h"
#include "machapi.h"
#include  "SRAM.h"

/*
DEF_ALIGN UI16 m_plc_d[PLC_D_TOTAL_SIZE];
DEF_ALIGN UI8 dync_part_mem[DYNC_MEM_SIZE];

MEM_TBL memtbl[MEMTBL_TYPE_END] = {{TSK_ST_TBL_SIZE*sizeof(UI16), 0, (UI8*)(m_plc_d + TSK_ST_TBL_OFFSET)},
									{CTRL_PART_MEMTBL_SIZE*sizeof(UI16), 0, (UI8*)(m_plc_d + CTRL_TBL_OFFSET)},
									{ACT_PART_MEMTBL_SIZE*sizeof(UI16), 0, (UI8*)(m_plc_d + ACT_TBL_OFFSET)},
									{sizeof(dync_part_mem), 0, dync_part_mem}};
*/
/*不再分区，使用同一块内存*/
DEF_ALIGN UI8 mem_block[DYNC_MEM_SIZE];

#if 1
MEM_TBL memtbl[MEMTBL_TYPE_END] = 
{
	{sizeof(mem_block), 0, (UI8*)(mem_block)},
    {sizeof(mem_block_e), 0, (UI8*)(mem_block_e)}
};
#else
MEM_TBL memtbl[MEMTBL_TYPE_END] = {sizeof(mem_block), 0, (UI8*)(mem_block)};
#endif

UI16 g_MemSize = 0;
UI16 iser_mem = 0;

void* AllocMemTbl(UI8 type, int size_t, int num)
{
    PMEM_TBL ptbl;
    void* pmem = NULL;
    int	size;


    size = size_t*num;	

    /*不再分区，使用同一块内存*/
#if 0
    if (type >=  MEMTBL_TYPE_END)
    {
        return NULL;
    }

    ptbl = memtbl + type;
#else
#if 1
    if (type == MEMTBL_SDO_TBL)
    {
        ptbl = memtbl + 1;
    }

	else if(type == MEMTBL_DYNC_MEM_EXT)
	{
		ptbl = memtbl + 1;
	}
    else
    {
        ptbl = memtbl;
		g_MemSize = g_MemSize + size;
    }
#else
    ptbl = memtbl;

#endif
#endif

    //保证每次返回的内存地址以4字节对齐
    if ((ptbl->free_pos & 0x3) != 0)
    {
        ptbl->free_pos += (4 - (ptbl->free_pos & 0x3));
    }

    if (ptbl->pmem == NULL || size > ptbl->size - ptbl->free_pos || ptbl->size <= ptbl->free_pos)
    {
        db_printf("Memtbl full, %d\r\n", type);
        iser_mem = 1;
        return NULL;
    }


    if (size > 0)
    {
        pmem = ptbl->pmem + ptbl->free_pos;
        ptbl->free_pos = ptbl->free_pos + size;
    }

    memset(pmem, 0, size);
    return pmem;
}

void *MemcpyPro(void *dest, void *src, size_t count)
{
    if (dest == NULL || src == NULL)
    {
        return NULL;
    }
    UI16 *pdest = (UI16*)(dest);
    UI16 *psrc = (UI16*)(src);

		int b = (int)dest%2;

    int n = count / 2;
    int m = count % 2;

	
    if (((UI8 *)src + n) <= (UI8 *)dest || (UI8 *)src >= ((UI8 *)dest + n))
	/*内存不重叠*/
	{
    	/*当被赋值地址存在8位便宜，没有对齐时*/
    	if(b > 0)
    	{
			UI8 *pdest_8 = (UI8 *)dest;
			UI8 *psrc_8 = (UI8 *)src;
			/*头部特殊处理*/
			pdest_8--;
			pdest = (UI16*)(pdest_8);
			pdest[0] = (pdest[0]&0x00FF) | ((psrc_8[0]&0x00FF)<<8);

			psrc_8--;
			psrc = (UI16*)(psrc_8);

			for (size_t i = 1; i < n; i++)
	        {
	            pdest[i] = psrc[i];
	        }

			if(m == 0)
			{
				m = 1;
			}
			else
			{
				pdest[n] = psrc[n];
			}
		}
		else
		{
			for (size_t i = 0; i < n; i++)
	        {
	            pdest[i] = psrc[i];
	        }
		}        
    }
    else  
	/*内存重叠*/
    {

		/*当被赋值地址存在8位便宜，没有对齐时*/
    	if(b > 0)
    	{
			UI8 *pdest_8 = (UI8 *)dest;
			UI8 *psrc_8 = (UI8 *)src;
			/*头部特殊处理*/
			pdest_8--;
			pdest = (UI16*)(pdest_8);
			pdest[0] = (pdest[0]&0x00FF) | ((psrc_8[0]&0x00FF)<<8);

			psrc_8--;
			psrc = (UI16*)(psrc_8);

			if (pdest > psrc)
	        {
	            //从高字节开始拷贝
	            for (ssize_t i = n - 1; i != 0; --i)
	            {
	                pdest[i] = psrc[i];
	            }
	        }
	        else
	        {
	            //从低字节开始拷贝
	            for (size_t i = 1; i < n; i++)
	            {
	                pdest[i] = psrc[i];
	            }
	        }

			if(m == 0)
			{
				m = 1;
			}
			else
			{
				pdest[n] = psrc[n];
			}
		}
		else
		{
			if (pdest > psrc)
	        {
	            //从高字节开始拷贝
	            for (ssize_t i = n - 1; i != -1; --i)
	            {
	                pdest[i] = psrc[i];
	            }
	        }
	        else
	        {
	            //从低字节开始拷贝
	            for (size_t i = 0; i < n; i++)
	            {
	                pdest[i] = psrc[i];
	            }
	        }
		}         
    }

	if(m > 0)
	{
		pdest[n] = (pdest[n]&0xFF00) | ((psrc[n]&0x00FF));
	}
	
    return pdest;
}


void *Memcpy1(UI16 *dest, UI16 *src, size_t count)
{
    if (dest == NULL || src == NULL)
    {
        return NULL;
    }
    UI16 *pdest = (UI16*)(dest);
    UI16 *psrc = (UI16*)(src);
    int n = count;

    if ((src + count) <= dest || src >= (dest + count))
    {
        for (size_t i = 0; i < n; i++)
        {
            pdest[i] = psrc[i];
        }
    }
    else  //内存重叠
    {
        if (pdest > psrc)
        {
            //从高字节开始拷贝
            for (ssize_t i = n - 1; i != -1; --i)
            {
                pdest[i] = psrc[i];
            }
        }
        else
        {
            //从低字节开始拷贝
            for (size_t i = 0; i < n; i++)
            {
                pdest[i] = psrc[i];
            }
        }
    }
    return pdest;
}
int UI16Count(int cn)
{
    int ret;
    if (cn % 2 == 0)
    {
        ret = cn / 2;
    }
    else
    {
        ret = cn / 2 + 1;
    }

    return ret;
}

BOOL InitMemTbl()
{
    return TRUE;
}
