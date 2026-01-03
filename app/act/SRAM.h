/*************************************************/
#ifndef     SRAM_H
#define     SRAM_H

//#include "types.h"
#include "typedef.h"
#include "servo.h"
#include "prot.h"

//#define		SYS232 

#define		CFG_WORD_CACHE_NUM	3
#define		MAXDAPORT	4
#define     CANTXBUFFSIZE        0x400		//2011-1-11 whaul  From 0x200 to 0x400
#define     CANRXBUFFSIZE        0x400
extern WORD canrbl[CANRXBUFFSIZE + 10];                 // can rxbuffer 
extern WORD cantbl[CANTXBUFFSIZE + 10];                 // can txbuffer 

extern UI16 mem_block_e[20000];
extern UI16 cfg_cache_word[3][2000];
extern PRESS_STRU g_press[MAXDAPORT];
extern UI16 gs_polinktb[MAX_PO_NUM][10];	//输出点链接存储区

#ifdef SYS232
extern UI8 prot_buf[(64 + BUF_REV_SIZE)* BUF_64_NUM + (128 + BUF_REV_SIZE)*BUF_128_NUM + (256 + BUF_REV_SIZE)*BUF_256_NUM 
				 		   + (512 + BUF_REV_SIZE)*BUF_512_NUM + (1024 + BUF_REV_SIZE)*BUF_1024_NUM];
#endif


#endif
