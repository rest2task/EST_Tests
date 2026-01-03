
/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    app_config.h

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:            2013.11.04

Description:    EtherCAT Slave configuration information.
			  The file could only be inlcuded by appp_config.c
Others:

History:    

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/

#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_

#include "domain.h"
#include "master.h"
#include "sync_config.h"
#include "slave_config.h"
#include "sync.h"
#include "app_data.h"

#include "Typedef.h"

/******  EtherCAT slave device information definition  *******/
//Beckhoff ET1100
#ifdef BECKHOFF_EL9800_SLAVE
/*
 * Vendor ID:       0x00000002
 * Product code:    0x04570862
 * Revision number: 0x0064008E
 */
#define VID_BECKHOFF        0x2
#define Beckhoff_ET1100     0x00000002, 0x04570862
#define PCODE_EL9800        0x04570862
/******  Configuration informaion of EL9800  ******/ 

//divide 8 Digital Input/Output to 4 groups of 2 Ports
#define ALIAS_EL9800        0
#define POS_EL9800 		    ALIAS_EL9800, 0
//Watchdog and SYNC parameters
#define EL9800_WDT_DIV      0x09C2  //watchdog divider
#define EL9800_PDO_WDT      0x03E8  //PDO watchdog time(EL9800_WDT_DIV * EL9800_PDO_WDT * 40ns)
#define EL9800_DC_ACTIVE    0x300
//#define EL9800_SYNC_CYCLE      1000000 //1ms
//#define EL9800_SYNC_CYCLE   500000
#define EL9800_SYNC_CYCLE (RXPDO_CYCLE * (1000000000/HZ)) //Same as RXPDO cycle, in ns


unsigned int el9800_pdo_off[MAX_PDO_NUM][MAX_ENTRY_PER_PDO];
ec_pdo_entry_info_t slave_el9800_out_pdo_entries[] = {
	{0x1600, 0x01, 8},	//OUTPUT
	{0x1600, 0x02, 8}	//OUTPUT
};
ec_pdo_entry_info_t slave_el9800_in_pdo_entries[] = {
	{0x1A00, 0x01, 8},	//INPUT
	{0x1A00, 0x02, 8}	//INPUT
};
ec_pdo_info_t slave_el9800_pdos[] = {
	{0x1600, 2, slave_el9800_out_pdo_entries + 0},
	{0x1a00, 2, slave_el9800_in_pdo_entries + 0},
	{0xFFFF}
};

ec_sync_info_t slave_el9800_syncs[] = {
	{2, EC_DIR_OUTPUT, 1, slave_el9800_pdos    ,    EC_WD_DISABLE},
	{3, EC_DIR_INPUT,  1, slave_el9800_pdos + 1,    EC_WD_DISABLE},
	{0xff}
};
ec_pdo_entry_reg_t el9800_domain_regs[] = {
	//gonggw, 2013.08.14, for EL9800_4A
 	{POS_EL9800, Beckhoff_ET1100, 0x1600, 0x01, &el9800_pdo_off[0][0]},
 	{POS_EL9800, Beckhoff_ET1100, 0x1600, 0x02, &el9800_pdo_off[0][1]},
 	{POS_EL9800, Beckhoff_ET1100, 0x1A00, 0x01, &el9800_pdo_off[1][0]},
 	{POS_EL9800, Beckhoff_ET1100, 0x1A00, 0x02, &el9800_pdo_off[1][1]},
 	{}
};
#endif	//end of "#ifdef BECKHOFF_EL9800_SLAVE"


/******  Configuration informaion of HUICHUAN EtherCAT slave board  ******/ 
/******  用于汇川IS580+ECAT扩展卡驱动  ******/ 
#ifdef HC_IS580_SLAVE

//#define HC_XML_V17 
#define HC_XML_V23 
#if defined(HC_XML_V17)
#define REV_NUM_IS580       0x00010000
#define VID_HUICHUAN        0x20
#elif defined(HC_XML_V23)
#define REV_NUM_IS580       0x00010000
#define VID_HUICHUAN        0x100000
#endif
#define IS580_PRODUCT_ID    0x0001010A
#define ALIAS_IS580         0
//Watchdog and SYNC parameters
#define IS580_WDT_DIV      0x09C2  //watchdog divider
#define IS580_PDO_WDT      0x03E8  //PDO watchdog time(IS620_WDT_DIV * IS620_PDO_WDT * 40ns)
#define IS580_DC_ACTIVE    0x300
#define IS580_SYNC_CYCLE (RXPDO_CYCLE * (1000000000/HZ)) //Same as RXPDO cycle, in ns


ec_pdo_entry_info_t slave_is580_out_pdo_entries[] = {
	{0x60FF, 0x0, 32},// 速度
	{0x60E0, 0x0, 16},// 正扭矩
	{0x6040, 0x0, 16},//控制指令
	//{0x20A4, 0x13, 16},
	//{0x20A4, 0x14, 16},
	//{0x20A4, 0x15, 16},
};
ec_pdo_entry_info_t slave_is580_in_pdo_entries[] = {
	{0x606C, 0x00, 32},	//实际速度
	{0x6077, 0x00, 16},	//实际扭矩
	{0x603F, 0x00, 32},	//读取驱动器警报地址
	{0x20F7, 0x08, 16}, //模块温度
	{0x20F9, 0x3B, 16},	//电机温度
	{0x2070, 0x05, 16},	//电机电流 
	{0x6041, 0x00, 16}	//驱动器状态字
};

ec_pdo_info_t slave_is580_pdos[] = {
	{0x1600, ARRAY_NUM(slave_is580_out_pdo_entries), slave_is580_out_pdo_entries},
	{0x1A00, ARRAY_NUM(slave_is580_in_pdo_entries) , slave_is580_in_pdo_entries },
	{0xFFFF}
};

ec_sync_info_t slave_is580_syncs[] = {
	 {2, EC_DIR_OUTPUT, 1, slave_is580_pdos    ,    EC_WD_ENABLE},
	 {3, EC_DIR_INPUT,  1, slave_is580_pdos + 1,    EC_WD_DISABLE},
	 {0xff}
};

#endif // end of "#ifdef HC_IS580_SLAVE"

/******  用于汇川ES750驱动  ******/ 
#ifdef HC_ES750_SLAVE

#define HC_XML_V23 
#if defined(HC_XML_V17)
#define REV_NUM_ES750       0x00010000
#define VID_HUICHUAN        0x20
#elif defined(HC_XML_V23)
#define REV_NUM_ES750       0x00010000
#define VID_HUICHUAN        0x100000
#endif
#define ES750_PRODUCT_ID    0x00190116
#define ALIAS_ES750         0
//Watchdog and SYNC parameters
#define ES750_WDT_DIV      0x09C2  //watchdog divider
#define ES750_PDO_WDT      0x03E8  //PDO watchdog time(IS620_WDT_DIV * IS620_PDO_WDT * 40ns)
#define ES750_DC_ACTIVE    0x300
#define ES750_SYNC_CYCLE (RXPDO_CYCLE * (1000000000/HZ)) //Same as RXPDO cycle, in ns


ec_pdo_entry_info_t slave_es750_out_pdo_entries[] = {
	{0x60FF, 0x0, 32},// 速度
	{0x60E0, 0x0, 16},// 正扭矩
	{0x6040, 0x0, 16},//控制指令
	//{0x20A4, 0x13, 16},
	//{0x20A4, 0x14, 16},
	//{0x20A4, 0x15, 16},
};
ec_pdo_entry_info_t slave_es750_in_pdo_entries[] = {
	{0x606C, 0x00, 32},	//实际速度
	{0x6077, 0x00, 16},	//实际扭矩
	{0x603F, 0x00, 32},	//读取驱动器警报地址
	{0x20F7, 0x08, 16}, //模块温度
	{0x20F9, 0x3B, 16},	//电机温度
	{0x2070, 0x05, 16},	//电机电流 
	{0x6041, 0x00, 16}	//驱动器状态字
};

ec_pdo_entry_info_t slave_es751_out_pdo_entries[] = {
	{0x68FF, 0x0, 32},// 速度
	{0x68E0, 0x0, 16},// 正扭矩
	{0x6840, 0x0, 16},//控制指令
	{0x28A4, 0x13, 16},
	{0x28A4, 0x14, 16},
	{0x28A4, 0x15, 16},
};
ec_pdo_entry_info_t slave_es751_in_pdo_entries[] = {
	{0x686C, 0x00, 32},	//实际速度
	{0x6877, 0x00, 16},	//实际扭矩
	{0x683F, 0x00, 32},	//读取驱动器警报地址
	{0x28F7, 0x08, 16}, //模块温度
	{0x28F9, 0x3B, 16},	//电机温度
	{0x2870, 0x05, 16},	//电机电流 
	{0x6841, 0x00, 16}	//驱动器状态字
};


ec_pdo_info_t slave_es750_out_pdos[] = {
	{0x1600, ARRAY_NUM(slave_es750_out_pdo_entries), slave_es750_out_pdo_entries},
	{0x1610, ARRAY_NUM(slave_es751_out_pdo_entries), slave_es751_out_pdo_entries},
	{0xFFFF}
};

ec_pdo_info_t slave_es750_in_pdos[] = {
	{0x1A00, ARRAY_NUM(slave_es750_in_pdo_entries), slave_es750_in_pdo_entries},
	{0x1A10, ARRAY_NUM(slave_es751_in_pdo_entries), slave_es751_in_pdo_entries},
	{0xFFFF}
};

ec_sync_info_t slave_es750_syncs[] = {
	 {2, EC_DIR_OUTPUT, 2, slave_es750_out_pdos    ,    EC_WD_ENABLE},
	 {3, EC_DIR_INPUT,  2, slave_es750_in_pdos     ,    EC_WD_DISABLE},
	 {0xff}
};

#endif // end of "#ifdef HC_ES750_SLAVE"


/******  用于汇川ES650/ES680驱动  ******/ 
#ifdef HC_IS620_SLAVE

#define HC_XML_V23
#if defined(HC_XML_V17)
#define REV_NUM_IS620       0x00010000
#define VID_HUICHUAN        0x20
#elif defined(HC_XML_V23)
#define REV_NUM_IS620       0x00010001
#define VID_HUICHUAN        0x100000
#endif
#define IS620_PRODUCT_ID    0x000C0108
#define ALIAS_IS620         0
#define POS_IS620   	    ALIAS_IS620, 0
//Watchdog and SYNC parameters
#define IS620_WDT_DIV      0x09C2  //watchdog divider
#define IS620_PDO_WDT      0x03E8  //PDO watchdog time(IS620_WDT_DIV * IS620_PDO_WDT * 40ns)
#define IS620_DC_ACTIVE    0x300
#define IS620_SYNC_CYCLE (RXPDO_CYCLE * (1000000000/HZ)) //Same as RXPDO cycle, in ns

ec_pdo_entry_info_t slave_is620_out_pdo_entries[] = {
	{0x60FF, 0x0, 32},// 速度
	{0x60E0, 0x0, 16},// 正扭矩
	{0x60E1, 0x0, 16},// 负扭矩	
	{0x6040, 0x0, 16},//控制指令// 0x1B06	压力反馈	单位0.1bar
	{0x2018, 0x1, 32},
	{0x2018, 0x3, 32},
	{0x2018, 0x5, 32},
	{0x2018, 0x7, 32}
};
ec_pdo_entry_info_t slave_is620_in_pdo_entries[] = {
	{0x6064, 0x00, 32},	//实际位置
	{0x606C, 0x00, 32}, //实际速度
	{0x6077, 0x00, 16},	//实际扭矩
	{0x200B, 0x2e, 16},	//驱动器警报地址
	{0x200B, 0x1C, 16}, //模块温度
	{0x200B, 0x58, 16},	//电机温度
	{0x200B, 0x19, 16},	//电机电流	
	{0x6041, 0x00, 16}	//驱动器状态字
};

ec_pdo_info_t slave_is620_pdos[] = {
	{0x1600, ARRAY_NUM(slave_is620_out_pdo_entries), slave_is620_out_pdo_entries},
	{0x1A00, ARRAY_NUM(slave_is620_in_pdo_entries) , slave_is620_in_pdo_entries },
	{0xFFFF}
};
ec_sync_info_t slave_is620_syncs[] = {
	{2, EC_DIR_OUTPUT, 1, slave_is620_pdos    ,    EC_WD_ENABLE},
	{3, EC_DIR_INPUT,  1, slave_is620_pdos + 1,    EC_WD_DISABLE},
	{0xff}
};


/******  Configuration informaion of ESTO EtherCAT slave board  ******/ 
/*
** Vendor ID and product Code in EEPROM of ESTO board
** Vendor ID:       0x00000021
** Product code:    0x1
*/
#ifdef ESTO_E410_SLAVE
#define VID_ESTO            0x21
#define PCODE_E410          0x00000001
#define REV_NUM_E410        0x00000002

#define ESTO_ID             VID_ESTO, PCODE_E410  
#define ESTO_E410           VID_ESTO, REV_NUM_E410
#define ALIAS_E410          0
#define POS_E410	        ALIAS_E410, 0

//Watchdog and SYNC parameters
#define E410_WDT_DIV        0x09C2  //watchdog divider
#define E410_PDO_WDT        0x03E8  //PDO watchdog time(E410_PDO_WDT * E410_WDT_DIV * 40ns)
#define E410_DC_ACTIVE      0x300
//#define E410_SYNC_CYCLE      1000000 //1ms
//#define E410_SYNC_CYCLE     500000
#define E410_SYNC_CYCLE (RXPDO_CYCLE * (1000000000/HZ)) //Same as RXPDO cycle, in ns

unsigned int e410_pdo_off[MAX_PDO_NUM][MAX_ENTRY_PER_PDO];


ec_pdo_entry_info_t slave_e410_out_pdo_entries[] = {
	{0x6040, 0x0, 16},	//OUTPUT
    {0x6041, 0x0, 32} //OUTPUT
};
ec_pdo_entry_info_t slave_e410_in_pdo_entries[] = {
	{0x6042, 0x00, 16},	//INPUT
    {0x6064, 0x00, 32} //INPUT
};
ec_pdo_info_t slave_e410_pdos[] = {
	{0x1600, 2, slave_e410_out_pdo_entries + 0},
	{0x1A00, 2, slave_e410_in_pdo_entries + 0},
	{0xFFFF}
};

ec_sync_info_t slave_e410_syncs[] = {
	{2, EC_DIR_OUTPUT, 1, slave_e410_pdos    ,    EC_WD_ENABLE},
    {3, EC_DIR_INPUT,  1, slave_e410_pdos + 1,    EC_WD_DISABLE},
    {0xff}
};

ec_pdo_entry_reg_t e410_domain_regs[] = {
 	{POS_E410, ESTO_ID, 0x6040, 0x00, &e410_pdo_off[0][0]},
 	{POS_E410, ESTO_ID, 0x6041, 0x00, &e410_pdo_off[0][1]},
 	{POS_E410, ESTO_ID, 0x6042, 0x00, &e410_pdo_off[1][0]},
 	{POS_E410, ESTO_ID, 0x6064, 0x00, &e410_pdo_off[1][1]},
 	{}
};
#endif
#endif
#endif

