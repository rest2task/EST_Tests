/************************************************************
Copyright (C), 2012-2017, EST. Co., Ltd.
FileName: Ainitial.c
Author:   zhangfang
Version : v1.0
Date:  	  2017-7
Description: LPC4337 system source initial
=============================================================
History:
<author>
<time>
<version >
<desc>
**************************************************************/
#include	"common.h"
#include	"SRAM.h"
#include	"string.h"


DEF_ALIGN UI16 mem_block_e[20000] = {0};	//外部扩展，目前存放部分部件的内存数据。 39k
DEF_ALIGN UI16 cfg_cache_word[CFG_WORD_CACHE_NUM][2000]={0};	//SRAM内的公共内存，用于部分自学习等

DEF_ALIGN PRESS_STRU g_press[MAXDAPORT] = {0};

//can

DEF_ALIGN WORD canrbl[CANRXBUFFSIZE + 10] = {0};                 // can rxbuffer 
DEF_ALIGN WORD cantbl[CANTXBUFFSIZE + 10] = {0};                 // can txbuffer 

#ifdef SYS232

DEF_ALIGN UI8 prot_buf[(64 + BUF_REV_SIZE)* BUF_64_NUM + (128 + BUF_REV_SIZE)*BUF_128_NUM + (256 + BUF_REV_SIZE)*BUF_256_NUM 
				 		   + (512 + BUF_REV_SIZE)*BUF_512_NUM + (1024 + BUF_REV_SIZE)*BUF_1024_NUM] = {0};
#endif
DEF_ALIGN UI16 gs_open_loop_stop_data[3][4000] = {0};	//半开环减速速度存储区

DEF_ALIGN UI16 gs_polinktb[MAX_PO_NUM][10] = {0};	//输出点链接存储区

