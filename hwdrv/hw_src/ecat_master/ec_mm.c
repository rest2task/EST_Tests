/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    ec_mm.c

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:             2013.09.24

Description:    Dynamic memory allocation

Others:

Function List:  
                    ec_mm_init          :  EC_MM initialization
                    ec_mm_array_init:  EC_MM array initialization
                    ec_malloc            :  Dynamic memory allocation; compatible with mallooc
                    ec_calloc             :  Dynamic memory allocation and initializing to 0; compatible with callooc
                    ec_zalloc             :  Dynamic memory allocation and initializing to 0; compatible with zallooc
                    ec_memset         :  Set the meory content to specific data; compatible with memset
                    ec_memcpy         :  meory copy; compatible with memcpy
History:    

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/

#include "debug.h"
#include "types.h"
#include "config.h"
#include "list.h"
#include "pdo.h"
#include "pdo_entry.h"
#include "sdo.h"
#include "sdo_entry.h"
#include "slave.h"
#include "sync.h"
#include "domain.h"
#include "datagram.h"
#include "fsm_master.h"
#include "ec_mm.h"
#ifdef DYN_MEM_ALLOC
struct ec_mm_poll_struct	ec_mm_pool;
struct ec_mm_poll_struct	*ec_mm_pool_ptr;
struct ec_mm_alloc_array	ec_mm_info_arr0;
/**
ec_mm_init: ec_master memory management global variable initialization
u8 *ec_mm_base: 	base address of memory to be allocated (called memory pool)
u32 ec_mm_len: 		total length of memory pool 
*/
int ec_mm_init(u8 *ec_mm_base, u32 ec_mm_len)
{
	int i, j;
	_DBG_PRINTF_("EC_MM start initializing EC Memory Management!\n");

	if((u32)ec_mm_base & ((1<<EC_PAGE_ADDR_BITS) - 1))
	{
		debug("%s, base address(0x%x) is not aligned\n", __func__, ec_mm_base);
		return -1;
	}
	if(ec_mm_len & ((1<<EC_PAGE_ADDR_BITS) - 1))
	{
		debug("%s, length(0x%x) is not aligned\n", ec_mm_len);
		return -1;
	}
	ec_mm_pool.mm_length	= ec_mm_len;
	//ec_mm_pool.addr_bits_per_flag_byte	= EC_PAGE_ADDR_BITS + 3; //PAGE_BITS + bits number of byte(8=2^5)
	ec_mm_pool.ec_mm_flag	= (u8 *)ec_mm_base;
	ec_mm_pool.mm_base		= (u8 *)ec_mm_base;
	ec_mm_pool.mm_page_size = EC_PAGE_SIZE;
	ec_mm_pool.mm_page_bits = EC_PAGE_ADDR_BITS;
	ec_mm_pool.ec_mm_info_header = &ec_mm_info_arr0;
	ec_mm_pool.ec_mm_info_count = 0;
	ec_mm_pool.ec_mm_info_num	= EC_MM_INFO_ARRAY;
	ec_mm_info_arr0.next 	= (struct ec_mm_alloc_array	*)0xFFFFFFFF;
	ec_mm_pool_ptr			= &ec_mm_pool;
	for(i = 0; i < EC_MM_INFO_ARRAY; i++)
		ec_mm_info_arr0.ec_mm_info[i].flag = 0;
	///clear flags
	for (i = 0; i < (ec_mm_pool.mm_length >> (ec_mm_pool.mm_page_bits + 3)); i++)
		ec_mm_pool.ec_mm_flag[i] = 0x0;
	
	ec_mm_pool.end_flag_index	= i;
	
	i = i >> EC_PAGE_ADDR_BITS;
	j = 0;
	do
	{
		if (i >= 8)
			ec_mm_pool.ec_mm_flag[j] = (u8)0xff;
		else
			ec_mm_pool.ec_mm_flag[j] = (u8)((1 << i) - 1);
		i = i - 8;
		j++;
	} while(i > 0);
	ec_mm_pool.start_flag_index	= j;
	return 0;
		
}
int ec_mm_array_init ()
{
	struct ec_mm_alloc_array	*next_array, *ec_mm_array_ptr;
	int i;
	ec_mm_pool_ptr->ec_mm_info_count++;
	next_array = (struct ec_mm_alloc_array	*)ec_malloc(sizeof(struct ec_mm_alloc_array), 0);
	ec_mm_array_ptr = ec_mm_pool_ptr->ec_mm_info_header;
	if (!ec_mm_array_ptr)
		return -1;
	while((u32)ec_mm_array_ptr->next != (u32)0xFFFFFFFF)
		ec_mm_array_ptr		= ec_mm_array_ptr->next;
	ec_mm_array_ptr->next	= next_array;
	next_array->next		= (struct ec_mm_alloc_array	*)0xFFFFFFFF;
	
	for(i = 0; i < EC_MM_INFO_ARRAY; i++)
		next_array->ec_mm_info[i].flag = 0;
	
	ec_mm_pool_ptr->ec_mm_info_num += EC_MM_INFO_ARRAY;
	return 0;
}
void ec_free(u8 *addr)
{
	struct ec_mm_alloc_array *ec_mm_array_ptr;
	struct ec_mm_alloc_info	*ec_mm_info_ptr;
	int i;
	int flag_index, bit_offset;
	u32 ec_mm_base, ec_mm_max;
	ec_mm_base = (u32)(ec_mm_pool_ptr->mm_base);
	ec_mm_max  = (u32)(ec_mm_pool_ptr->mm_base + ec_mm_pool_ptr->mm_length - 1);
	if (((u32)addr < ec_mm_base) || ((u32)addr > ec_mm_max))
	{
		_DBG_PRINTF_("***WARNING, address(%x) is beyond the space, return\n", addr);
		return;
	}
	ec_mm_info_ptr 	= (struct ec_mm_alloc_info *)0;
	ec_mm_array_ptr = ec_mm_pool_ptr->ec_mm_info_header;
	while((u32)ec_mm_array_ptr != (u32)0xFFFFFFFF)
	{
		for(i = 0; i < EC_MM_INFO_ARRAY; i++)
		{
			if (ec_mm_array_ptr->ec_mm_info[i].start_address == (u32)addr)
			{
				ec_mm_info_ptr = &(ec_mm_array_ptr->ec_mm_info[i]);
				break;
			}
		}
		if (ec_mm_info_ptr)
			break;
		ec_mm_array_ptr = ec_mm_array_ptr->next;
	}
	if(!ec_mm_info_ptr)
	{
		_DBG_PRINTF_("***%s, try to free an unallocatd memory @ address %p\n", __func__, addr);
		return;
	}
	flag_index = (ec_mm_info_ptr->start_address & (ec_mm_pool_ptr->mm_length - 1)) >> \
					(ec_mm_pool_ptr->mm_page_bits + 3);
	bit_offset = (ec_mm_info_ptr->start_address & (ec_mm_pool_ptr->mm_length - 1)) >> \
					ec_mm_pool_ptr->mm_page_bits;
	bit_offset = bit_offset & 0x7;

#ifdef EC_MM_DEBUG
	_DBG_PRINTF_("$$$%s, memory from 0x%p, length=%x, freed\n", __func__, \
			ec_mm_info_ptr->start_address, (ec_mm_info_ptr->mem_len << ec_mm_pool_ptr->mm_page_bits));
#endif
	while(ec_mm_info_ptr->mem_len)
	{
		if(ec_mm_info_ptr->mem_len >= 8)
		{
			ec_mm_pool_ptr->ec_mm_flag[flag_index++] = 0;
			bit_offset = 0;
			ec_mm_info_ptr->mem_len -= 8;
		}
		else
		{
			ec_mm_pool_ptr->ec_mm_flag[flag_index] &= ~(((1 << ec_mm_info_ptr->mem_len) - 1) << bit_offset);
			ec_mm_info_ptr->mem_len=0;
		}
			
	}
	ec_mm_info_ptr->mem_len			= 0;
	ec_mm_info_ptr->flag 			= 0x0;
	ec_mm_info_ptr->start_address 	= 0;
	ec_mm_pool_ptr->ec_mm_info_count--;
}

u8 * ec_malloc(size_t size, u32 flags)
{
	int page_size, page_size_tmp;
	int i, j;
	int flag_index;
	int bit_index;
	u8	cmp_format;
	u32 ret;
	struct ec_mm_alloc_array	*ec_mm_array_ptr;
	struct ec_mm_alloc_info		*ec_mm_info_ptr;
	if ((size > ULONG_MAX) || size == 0)
		return NULL;
	if (ec_mm_pool_ptr->ec_mm_info_count == (ec_mm_pool_ptr->ec_mm_info_num - 1))
	{
		 if (ec_mm_array_init())
		 {
		 	_DBG_PRINTF_("***%s, call ec_mm_array_init failure!\n", __func__);
			return NULL; //allocate ec_mm_array failure
		 }
		 //the ec_mm_info_count is increment 2 times when allocation memory for ec_mm_array, must 
		 //decrement one here
		 ec_mm_pool_ptr->ec_mm_info_count--;
	}
	page_size = (size + (ec_mm_pool_ptr->mm_page_size- 1)) >> ec_mm_pool_ptr->mm_page_bits;
	i = ec_mm_pool_ptr->start_flag_index;
	flag_index = 0;
	page_size_tmp = page_size;
	cmp_format = 0xff;
	bit_index  = 0;
	while((page_size_tmp > 0) && (i < ec_mm_pool_ptr->end_flag_index))
	{
		if (ec_mm_pool_ptr->ec_mm_flag[i] == 0xff)
		{
			i++;
			continue;
		}
		else if (page_size_tmp > 8)
		{
			j = 0;
			cmp_format = 0xff;
			if (!(ec_mm_pool_ptr->ec_mm_flag[i] & cmp_format))
			{
				flag_index = !flag_index ? i : flag_index;
				page_size_tmp = page_size_tmp - 8;
			}
			else
			{
				flag_index = 0;
				page_size_tmp = page_size;
			}
			i++;
		}
		else
		{
			j = 0;
			cmp_format = (1 << page_size_tmp) - 1;
			if (flag_index > 0) //part of memory has allocated, this part of memory must be continous with the allocated memory
			{
			
				page_size_tmp = 0;
				if(cmp_format & ec_mm_pool_ptr->ec_mm_flag[i])
				{
					flag_index = 0;
					page_size_tmp = page_size;					
					i++;
				}
			}
			else //all requested memory is less than 8 pages
			{
				while((j + page_size_tmp) <= 8)
				{
					if (cmp_format & ec_mm_pool_ptr->ec_mm_flag[i])
					{
						cmp_format = cmp_format << 1;
						//break;
					}
					else
					{
						flag_index 	  = i;
						page_size_tmp = 0;
						break;
					}
					j++;
				}
				if (j + page_size_tmp > 8)
					i++;
			}
				
		}
	}
	//set the allocated flags to 1
	i = flag_index;
	if (!flag_index) //memory allocated failure
	{
		_DBG_PRINTF_("***%s, request memory allocation failure!\n", __func__);
		return NULL;
	}
	else
	{
		page_size_tmp = page_size;
		while(page_size_tmp)
		{
			if (page_size_tmp >=8)
			{
				ec_mm_pool_ptr->ec_mm_flag[i] = 0xff;
				page_size_tmp = page_size_tmp - 8;
			}
			else
			{
				ec_mm_pool_ptr->ec_mm_flag[i] |= cmp_format;
				page_size_tmp = 0;
			}
			i++;
		}
	}
	//record memory allocation info and retrun address
	ret = (u32)(ec_mm_pool_ptr->mm_base) + (flag_index << (ec_mm_pool_ptr->mm_page_bits + 3)) + \
			(j << (ec_mm_pool_ptr->mm_page_bits));
	
#ifdef EC_MM_DEBUG
	_DBG_PRINTF_("***%s, memory from 0x%08x, real size=0x%x, flag_index:%d, %d pages allocated\n", __func__, ret, \
				size, flag_index, page_size);
#endif
	ec_mm_info_ptr 	= (struct ec_mm_alloc_info *)0;
	ec_mm_array_ptr = ec_mm_pool_ptr->ec_mm_info_header;
	while((u32)ec_mm_array_ptr != (u32)0xFFFFFFFF)
	{
		for(i = 0; i < EC_MM_INFO_ARRAY; i++)
		{
			if (ec_mm_array_ptr->ec_mm_info[i].flag==0)
			{
				ec_mm_info_ptr = &(ec_mm_array_ptr->ec_mm_info[i]);
				break;
			}
		}
		if (ec_mm_info_ptr)
			break;
		ec_mm_array_ptr = ec_mm_array_ptr->next;
	}
	ec_mm_info_ptr->flag 			= 0x1;
	ec_mm_info_ptr->mem_len 		= page_size;
	ec_mm_info_ptr->start_address 	= ret;
	ec_mm_pool_ptr->ec_mm_info_count++;
#ifdef EC_MM_DEBUG
	_DBG_PRINTF_("%dth info, flag=%x, mem_len=%d, start_addr=%p, info_cnt=%d\n", \
		i, ec_mm_info_ptr->flag, ec_mm_info_ptr->mem_len, ec_mm_info_ptr->start_address, ec_mm_pool_ptr->ec_mm_info_count);
#endif
	return (u8 *)ret;
}

/**
 * ec_calloc - allocate memory for an array. The memory is set to zero.
 * @n: number of elements.
 * @size: element size.
 * @flags: the type of memory to allocate, for compatible with Linux interface, not used in ec_master
 **/
u8 * ec_calloc(size_t n, size_t size, u32 flags)
{
	u8 * ret;
	int i;
	if (size != 0 && n > ULONG_MAX / size)
		return NULL;
	ret = ec_malloc(n * size, flags);
	if (!ret)
	{
		debug("%s, allocate %d bytes failre!!!\n ", __func__, n*size);

		return NULL;
	}
	for (i = 0; i < ((n * size + 3) >>2); i++)
		((u32 *)ret)[i] = 0x0;
	return ret;
}

u8 * ec_zalloc(size_t size, u32 flags)
{
	u8 * ret;
	int i;
	if (!size)
		return NULL;
	ret = ec_malloc(size, flags);
	if (!ret)
	{
		debug("%s, allocate %d bytes failre!!!\n ", __func__, size);
		return NULL;
	}
	for (i = 0; i < ((size + 3) >>2); i++)
		((u32 *)ret)[i] = 0x0;
	return ret;
}



#else
ec_pdo_t					pdo_pool[PDO_POOL_DEPTH];
ec_pdo_entry_t				pdo_entry_pool[PDO_ENTRY_POOL_DEPTH];
//ec_sdo_t					sdo_pool[SDO_POOL_DEPTH];
//ec_sdo_entry_t				sdo_entry_pool[SDO_ENTRY_POOL_DEPTH];
//ec_master_sdo_request_t		master_sdo_request_pool[MASTER_SDO_REQUEST_DEPTH];
//ec_sdo_request_t			sdo_request_pool[MASTER_SDO_REQUEST_DEPTH];
//ec_master_soe_request_t		master_soe_request_pool[MASTER_SOE_REQUEST_DEPTH];
uint8_t						buffer_pool[BUFFER_POOL_DEPTH * BUFFER_POOL_UNIT] __attribute__((aligned(BUFFER_POOL_UNIT)));
uint8_t 					buffer_pool_flag[BUFFER_POOL_DEPTH]; //buffer management 

//ec_datagram_t				domain_dgram_array[DOMAIN_DATAGRAM_NUM];
s16							domain_dgram_idx;
//ec_domain_t					domain_array[DOMAIN_NUM];
s16							domain_idx;
ec_slave_t					ec_slave_array[EC_SLAVE_NUM];
s16							ec_slave_idx;
ec_slave_config_t			ec_slave_config_array[EC_SLAVE_CONFIG_NUM];
s16							ec_slave_config_idx;
char *						sii_string_ptr[MAX_STRING_PER_SLAVE];
s16							sii_string_idx;
uint8_t						no_free_buffer[NO_FREE_BUFFER_LEN];
uint32_t					no_free_buffer_idx;
ec_sync_t					sii_sync_array[EC_SLAVE_NUM][EC_MAX_SYNC_MANAGERS];
s16							sii_sync_idx;

uint8_t *buffer_pool_get(int size)
{
	int unit_size;
	int i, cnt;
	uint8_t *data;
	if (size == 0)
		return NULL;
	else
		unit_size = (size + BUFFER_POOL_UNIT -1)/BUFFER_POOL_UNIT;
	
	if (unit_size > BUFFER_POOL_DEPTH)
	{
		_DBG_PRINTF_("Error: Requested size Too Big!\n");
		return NULL;
	}
	cnt = 0;
	data = NULL;
	for(i = 0; i < BUFFER_POOL_DEPTH; i++)
	{
		if (buffer_pool_flag[i] ==0)
			cnt++;
		else
			cnt = 0;

		if(cnt == unit_size)
		{
			i -= (cnt-1);
			buffer_pool_flag[i++] = cnt;
			data = (uint8_t *)(buffer_pool + (i * BUFFER_POOL_UNIT));
			cnt--;
			while(cnt)
			{
				buffer_pool_flag[i++] = 1;
			}
			break;
		}
		
	}
	return data;
}
void buffer_pool_put(uint8_t *data)
{
	int index;
	int i;
	if ((data < buffer_pool) || (data > (buffer_pool + (BUFFER_POOL_UNIT * BUFFER_POOL_DEPTH))))
		return;
	index = (data - buffer_pool)/BUFFER_POOL_UNIT;
	for(i = 1; i < buffer_pool_flag[index]; i++)
		buffer_pool_flag[index + i] = 0;
	buffer_pool_flag[index] = 0;
}
ec_datagram_t *domain_dgram_get()
{
	if (domain_dgram_idx < DOMAIN_DATAGRAM_NUM)
	{
		domain_dgram_idx++;
		return NULL;
	}
	else
	{
		domain_dgram_idx = 0xFFFF;
		_DBG_PRINTF_("Error: No enough domain_dgram_array reserved\n");
		return NULL;
	}
}

ec_pdo_t *pdo_pool_get()
{
	int i;
	for(i = 0; i < PDO_POOL_DEPTH; i++)
	{
		if(pdo_pool[i].used == 0)
		{
#if 0
			_DBG_PRINTF_("!!pdo %x get\n", &(pdo_pool[i]));
#endif
			pdo_pool[i].used = 1;
			return &(pdo_pool[i]);
		}
	}
	return NULL;
}
void pdo_pool_put(ec_pdo_t *pdo)
{
#if 0
	_DBG_PRINTF_("++pdo %x put\n", pdo);
#endif
	pdo->used = 0;
}

ec_pdo_entry_t *pdo_entry_pool_get()
{
	int i;
	for(i = 0; i < PDO_ENTRY_POOL_DEPTH; i++)
	{
		if(pdo_entry_pool[i].used == 0)
		{
#if 0
			_DBG_PRINTF_("**pdo entry %x get\n", &(pdo_entry_pool[i]));
#endif
			pdo_entry_pool[i].used = 1;
			return &(pdo_entry_pool[i]);
		}
	}
	return NULL;
}
void pdo_entry_pool_put(ec_pdo_entry_t *entry)
{
#if 0
	_DBG_PRINTF_("@@pdo entry %x put\n", entry);
#endif
	entry->used = 0;
}
ec_sdo_t *sdo_pool_get()
{
//	int i;
//	for(i = 0; i < SDO_POOL_DEPTH; i++)
//	{
//		if(sdo_pool[i].used == 0)
//		{
//			sdo_pool[i].used = 1;
//			return &(sdo_pool[i]);
//		}
//	}
	return NULL;
}
void sdo_pool_put(ec_sdo_t *sdo)
{
	sdo->used = 0;
}
ec_master_sdo_request_t *sdo_entry_pool_get()
{
//	int i;
//	for(i = 0; i < SDO_REQUEST_DEPTH; i++)
//	{
//		if(sdo_entry_pool[i].used == 0)
//		{
//			sdo_entry_pool[i].used = 1;
//			//return &(sdo_entry_pool[i]);			
//			return (ec_master_sdo_request_t *)(&(sdo_entry_pool[i]));
//		}
//	}
	return NULL;
}
void sdo_entry_pool_put(ec_sdo_entry_t *entry)
{
	entry->used = 0;
}
ec_master_sdo_request_t *master_sdo_request_pool_get()
{
//	int i;
//	for(i = 0; i < MASTER_SDO_REQUEST_DEPTH; i++)
//	{
//		if(master_sdo_request_pool[i].used == 0)
//		{
//			master_sdo_request_pool[i].used = 1;
//			return &(master_sdo_request_pool[i]);
//		}
//	}
	return NULL;
}
void master_sdo_request_pool_put(ec_master_sdo_request_t *request)
{
	request->used = 0;
}
ec_sdo_request_t *sdo_request_pool_get()
{
//	int i;
//	for(i = 0; i < SDO_REQUEST_DEPTH; i++)
//	{
//		if(sdo_request_pool[i].used == 0)
//		{
//			sdo_request_pool[i].used = 1;
//			return &(sdo_request_pool[i]);
//		}
//	}
	return NULL;
}
void sdo_request_pool_put(ec_sdo_request_t *request)
{
	request->used = 0;
}
ec_master_soe_request_t *master_soe_request_pool_get()
{
//	int i;
//	for(i = 0; i < MASTER_SOE_REQUEST_DEPTH; i++)
//	{
//		if(master_soe_request_pool[i].used == 0)
//		{
//			master_soe_request_pool[i].used = 1;
//			return &(master_soe_request_pool[i]);
//		}
//	}
	return NULL;
}
void master_soe_request_pool_put(ec_master_soe_request_t *request)
{
	request->used = 0;
}

uint8_t *no_free_buf_get(int size)
{
	uint8_t *data;
	if ((no_free_buffer_idx + size) > (NO_FREE_BUFFER_LEN-1))
	{
		return NULL;
	}
	else
	{
		data = &no_free_buffer[no_free_buffer_idx];
		no_free_buffer_idx += size;
		return data;
	}
}
extern uint32_t CGU_ClockSourceFrequency[];

int ec_mm_init(void)
{
	int i;
	_DBG_PRINTF_("EC_MM start initializing EC Memory Management!\n");
	domain_dgram_idx	= 0;
	ec_slave_idx		= 0;
	domain_idx			= 0;
	ec_slave_config_idx	= 0;
	sii_string_idx		= 0;
	no_free_buffer_idx	= 0;
	sii_sync_idx		= 0;
    
	for(i = 0; i < PDO_POOL_DEPTH; i++)
		pdo_pool[i].used = 0;
	
	for(i = 0; i < PDO_ENTRY_POOL_DEPTH; i++)
		pdo_entry_pool[i].used = 0;

	for(i = 0; i < BUFFER_POOL_DEPTH; i++)
		buffer_pool_flag[i] = 0; //关马达断驱动器强电方案时而会出现Ecat初始化异常的现象。(SDO申请内存失败)
		
//	for(i = 0; i < SDO_POOL_DEPTH; i++)
//		sdo_pool[i].used = 0;

//	for(i = 0; i < SDO_ENTRY_POOL_DEPTH; i++)
//		sdo_entry_pool[i].used = 0;
	
//	for(i = 0; i < MASTER_SDO_REQUEST_DEPTH; i++)
//		master_sdo_request_pool[i].used = 0;

//	for(i = 0; i < MASTER_SOE_REQUEST_DEPTH; i++)
//		master_soe_request_pool[i].used = 0;
	
//	for(i = 0; i < SDO_REQUEST_DEPTH; i++)
//		sdo_request_pool[i].used = 0;
	return 0;
	
}
#endif

void * ec_memset(u8 *s, int c, size_t n)
{
	char *xs;
	int i;
	u32 c_u32;
	u8  c_u8;
	//c_u32 = (c & 0xff) | ((c & 0xff)<<8) | ((c & 0xff)<<16) | ((c & 0xff)<<24);
	c_u8 = (c & 0xff);
    c_u32 = c_u8 | (c_u8 << 8) | (c_u8 << 16) | (c_u8 << 24);
	xs = (char *) s;
    while(((u32)xs & 3) && (n > 0))
	{
		*xs = c_u8;
		xs++;
        n--;
	}
	if(n >= 4)
	{
		for(i = 0; i < (n >> 2); i++)
			((u32 *)xs)[i] = c_u32;
		
		xs = xs + (i << 2);
	}
	n = n & 0x3;
	for(i = 0; i < n; i++)
		xs[i] = c_u8;
	return s;
}

void * ec_memcpy(void *__dest, const void *__src, size_t __n)
{
	u8 	*d, *s;
	d = (u8 *)__dest;
	s = (u8 *)__src;
	
	while(__n>>3)
	{
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		__n -= 8;
	}
	if (__n & (1 << 2)) {
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
	}

	if (__n & (1 << 1)) {
		*d++ = *s++;
		*d++ = *s++;
	}

	if (__n & 1)
		*d++ = *s++;

	return __dest;
	
}

