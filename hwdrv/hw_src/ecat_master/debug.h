/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    debug.h

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:           2013.09.23

Description:    Debug control

Others:


History:

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/

#ifndef _DEBUG_H_
#define _DEBUG_H_


#include <stdio.h>
#include <string.h>
#include "fsl_debug_console.h"
/***********
* 0, print error
* 1, print error/warning
* 2, print error/warning/infomration
* 3, print error/warning/infomration/debug
************/

int printf(const char *fmt, ...);

//#define EC_DEBUG_LEVEL		3
#define EC_DEBUG_LEVEL		0

#define EC_DEBUG

#define uart_printf printf
#ifdef EC_DEBUG

#define debug(fmt,args...)					\
	if(EC_DEBUG_LEVEL > 2) 					\
		uart_printf((const char *)fmt ,##args)

#define dev_dbg(dev, fmt,args...)			\
	if(EC_DEBUG_LEVEL > 2) 					\
		uart_printf((const char *)fmt ,##args)

#define dev_info(dev, format, arg...)		\
	if(EC_DEBUG_LEVEL > 1) 					\
		uart_printf("INFO:" format , ##arg)
#define dev_warn(dev, format, arg...)		\
	if(EC_DEBUG_LEVEL > 0) 					\
		uart_printf("WARN:" format , ##arg)

#define dev_err(dev, format, arg...)		\
	uart_printf("ERROR:" format , ## arg)

#define _DBG_PRINTF_(fmt,args...) uart_printf(fmt, ##args)
#else
#define debug(fmt,args...)
#define dev_dbg(dev, fmt,args...)
#define dev_info(dev, format, arg...)
#define dev_warn(dev, format, arg...)
#define dev_err(dev, format, arg...)
#define _DBG_PRINTF_(fmt,args...)
#endif

#ifdef EC_DEBUG
//#define EC_MM_DEBUG		//used in "common/ec_mm.c"
//#define EC_DYN_MEM_PRT		//Print dynamic memory allocation info
#define EC_DEBUG_MASTER		//print debug info in "master/master.c"
//#define EC_DEBUG_SLAVE		//print debug info in "master/slave.c"
//#define EC_DEBUG_SLAVE_CONFIG	//print debug info in "master/slave_config.c"
//gonggw,2013.10.18
//#define EC_DEBUG_ADD_FSM_SLAVE_SCAN //add one extra state to FSM_SLAVE_SCAN for send more datagrams
//#define EC_NET_TIME_TEST		//to test the total time of one packet sending and receiving
//#define S3C_NAND_DEBUG
//#define DEBUG_HSMMC

//#define ECAT_TIME_TEST
#ifdef ECAT_TIME_TEST
//#define ECAT_REC_TIME_TEST
//#define ECAT_SEND_TIME_TEST
//#define ECAT_QUEUE_TIME_TEST
//#define ECAT_STATE_CHK_TIME_TEST
#endif
#endif
//#define PRINT_PAUSE_TIMER //stop timer clock in uart_printf("common/uart_printf.c")
//gonggw, 2014.04.09, for Timing debug
#define KIDD_S_R_JIFF_TEST
#ifdef KIDD_S_R_JIFF_TEST
extern unsigned int test_jiff[16];

//#define TIME_TEST_MII_CHECK_MEDIA //time test in mii_check_media
//#define TIME_TEST_IDLE_THREAD         //time test in ec_master_idle_thread
//#define TIME_TEST1
//#define TIME_TEST_ENTER_OP          //Test the time to Enter OP state(fsm_slave_config.c, about line 1695)


#if defined(TIME_TEST_MII_CHECK_MEDIA)
#define TEST_POINT_NUM 7

#elif defined(TIME_TEST_ECRT_MASTER_RECEIVE)
#define TEST_POINT_NUM 8

#elif defined(TIME_TEST_IDLE_THREAD)
#define TEST_POINT_NUM 8

#elif defined(TIME_TEST_ENTER_OP)
#define TEST_POINT_NUM 10

#else
#define TEST_POINT_NUM 8
#endif

#endif


#endif
