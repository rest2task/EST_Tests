/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    ec_mm.h

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:           2013.09.24

Description:     Memory management header file for mm.c

Others:


History:    

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/

#ifndef _EC_MM_H_
#define _EC_MM_H_
#include "types.h"
#include "slave.h"
#include "domain.h"
#include "ecrt.h"
#include "slave_config.h"

//#define DYN_MEM_ALLOC						//use Dynamic memory allocation when defined

#ifdef DYN_MEM_ALLOC
#define EC_PAGE_ADDR_BITS	7
#define EC_PAGE_SIZE (1<<EC_PAGE_ADDR_BITS) //minimum unit to be allocated is 128 byts
#define ULONG_MAX	(~0UL)

#define EC_MM_INFO_ARRAY 	64		//number of element in a ec_mm_alloc_array

struct ec_mm_alloc_info {
	u32 start_address; 				//start address of allocated memory
	u32 mem_len; 					//length of allocated mmeory, use page as unit
	u32 flag;						//the data in allocation info entry is active or not
};


struct ec_mm_alloc_array {
	struct ec_mm_alloc_info	ec_mm_info[EC_MM_INFO_ARRAY];	
	struct ec_mm_alloc_array	*next; //link to next ec_mm_alloc_array, the value of 0xFFFFFFFF means last array
};

//Memory Management structure for total dynamic allocation
//mm_length bytes memory is divided to mm_length / 128 pages, the minimum allocated memory is 128 bytes
struct ec_mm_poll_struct {
	u8 	 *mm_base; 					//base address of total memory for dynamic allocation(Memory pool)
	u32  mm_length; 				//total length of memory
	u32	 mm_page_size; 				//minimum unit of ec memory alllocation
	u32  mm_page_bits;  			//number of address bits of page
	u8	 *ec_mm_flag;				//the first 1KB in memory pool is used to keep ec_mm_falg. 
									//One bit for one page; 0, page is free;1, page is allocated
	/*variables below to manage allocated memory*/
	//u32	 addr_bits_per_flag_byte;	//number of address bits managed by one flag byte
	u32  start_flag_index; 			//start index of flag; the index to exclude the flags for all flag bytes
	u32  end_flag_index;   			//end index of flag; the index to exclude the flags for all flag bytes
	struct ec_mm_alloc_array	*ec_mm_info_header; //point to the first ec_mm_alloc_array
	int	 ec_mm_info_count;			//active entry number
	int	 ec_mm_info_num;			//total entry number, initialized to EC_MM_INFO_ARRAY
};
u8 * ec_calloc(size_t n, size_t size, u32 flags);
u8 * ec_zalloc(size_t size, u32 flags);
u8 * ec_malloc(size_t size, u32 flags);
void ec_free(u8 *addr);
#else
/******************** used for memory Pool ********************///slaves 4 to 8
#define PDO_POOL_DEPTH			(32*2)			//Total nubmer of PDO Pool
#define PDO_ENTRY_POOL_DEPTH		(190*2)			//Total number of PDO entry Pool
#define SDO_POOL_DEPTH			(4*2)			//Total nubmer of SDO Pool
#define SDO_ENTRY_POOL_DEPTH		(4*2* SDO_POOL_DEPTH)	//Total number of SDO entry Pool(Consider 3 entries per PDO in average)
#define MASTER_SDO_REQUEST_DEPTH	(4*2)			//Total number of ec_master SDO Request Pool
#define SDO_REQUEST_DEPTH		(4*2) 			//Total number of SDO Request Pool
#define MASTER_SOE_REQUEST_DEPTH	2			//Total number of ec_master SOE Request Pool

#define BUFFER_POOL_UNIT		(16*2)			//base unit of buffer pool
#define BUFFER_POOL_DEPTH		(16*2)			//Total number of buffer pool
/******************** End of memory Pool ********************///slaves 4 to 8


//Domain datagram and data buffer
#define DOMAIN_NUM		(2*2) 	//Maximun number of domain
#define DOMAIN_DATAGRAM_NUM	(4*2) 	//Maximun number of datagram for domain
extern ec_domain_t				domain_array[DOMAIN_NUM];
extern s16						domain_idx;

#define EC_SLAVE_NUM			24 //Maximum number of slaves in EtherCAT network
extern ec_slave_t				ec_slave_array[EC_SLAVE_NUM];
extern s16						ec_slave_idx;

#define EC_SLAVE_CONFIG_NUM		24 //Maximum number of slaves config
extern ec_slave_config_t		ec_slave_config_array[EC_SLAVE_CONFIG_NUM];
extern s16						ec_slave_config_idx;

#define MAX_STRING_PER_SLAVE	40
extern char *					sii_string_ptr[MAX_STRING_PER_SLAVE];
extern s16						sii_string_idx;

#define NO_FREE_BUFFER_LEN		800
extern uint8_t					no_free_buffer[NO_FREE_BUFFER_LEN];
extern uint32_t					no_free_buffer_idx;

extern ec_sync_t				sii_sync_array[EC_SLAVE_NUM][EC_MAX_SYNC_MANAGERS];
extern s16						sii_sync_idx;
#endif
//function prototype declaration
int ec_mm_init(void);
//int ec_mm_init(u8 *ec_mm_base, u32 ec_mm_len);
//int ec_mm_array_init ();
void * ec_memcpy(void *__dest, const void *__src, size_t __n);
void * ec_memset(u8 *s, int c, size_t n);
ec_master_soe_request_t *master_soe_request_pool_get(void);

uint8_t *buffer_pool_get(int size);
ec_pdo_t *pdo_pool_get(void);
ec_datagram_t *domain_dgram_get(void);
ec_pdo_entry_t *pdo_entry_pool_get(void);
ec_master_sdo_request_t *master_sdo_request_pool_get(void);
uint8_t *no_free_buf_get(int size);
ec_sdo_request_t *sdo_request_pool_get(void);
void buffer_pool_put(uint8_t *data);
void sdo_request_pool_put(ec_sdo_request_t *request);
void pdo_pool_put(ec_pdo_t *pdo);
void master_sdo_request_pool_put(ec_master_sdo_request_t *request);
void master_soe_request_pool_put(ec_master_soe_request_t *request);
void pdo_entry_pool_put(ec_pdo_entry_t *entry);
void sdo_pool_put(ec_sdo_t *sdo);
void sdo_entry_pool_put(ec_sdo_entry_t *entry);
ec_sdo_t *sdo_pool_get(void);
ec_master_sdo_request_t *sdo_entry_pool_get(void);




#endif

