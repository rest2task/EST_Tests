/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    app_data.h

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:            2013.11.04

Description:   Constants and Parameters for EtherCAT Access

Others:

History:    

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/
#ifndef _APP_DATA_H_
#define _APP_DATA_H_

#include "types.h"
#include "master.h"
#include "datagram.h"

#include "ecrt.h"
#include "list.h"
#include "app_driver_func.h"


#define S_IN_NS			(1000000000ULL)
//#define APP_START_OFFSET	((uint64_t)(8 * S_IN_NS))

#define MS_IN_NS			(1000000ULL)
#define APP_START_OFFSET	((uint64_t)(150 * MS_IN_NS))

//Max retry times when error
#define APP_TASK_RETRY	3

//Max Number of Slave
#define MAX_SLV_NUM		8

//Calculate PDO package loss rate when defined
#define PDO_LOSS_RATE

//Max Number of Slave
#define MAX_PDO_NUM				4
#define MAX_ENTRY_PER_PDO		10

//ECAT Slave Reconfig STATE Definition
#define RECONFIG_IDLE				0
#define RECONFIG_START			1
#define RECONFIG_STATE_ADDRESS	2
#define RECONFIG_STATE_CONFIG		3
#define RECONFIG_STATE_STATIC_SYNC	4
//Slave config information
typedef struct ec_sc_info {
    ec_sync_info_t     *sync_cfg_info;
    ec_pdo_entry_reg_t *pdo_entry_info;
    uint16_t            alias;				/**< Slave alias address. */
    uint16_t            position;			/**< Slave position. */
    uint32_t            vendor_id;			/**< Slave vendor ID. */
    uint32_t            product_code;		/**< Slave product code. */
    uint32_t            sync0_cycle;		/** Sync periond(SYNC0), which written to Register 0x9A0:0x9A3*/
    uint16_t            wdt_div;			/** Watchdog Timer which Written to Register 0x400:0x401*/
    uint16_t            pdo_wdt;			/** PDO Watchdog timer counter, which Written to Register 0x420:0x421*/
    uint16_t            dc_active;			/** Cycle Unit config, which Written to Register 0x980:981*/
    uint16_t            app_sc_id;			/** Slave config id, only number to match ec_sc_info and sc in master->configs*/
} ec_sc_info_t;
//Application config structure
//the name of "ALL" is reserved for configuring all slaves
//"NULL" is reserved for flag of the last element
typedef struct ecat_config
{
    u8                  *name;				//config name
    ec_sc_info_t        *ec_sc_info_array;	//EtherCAT Slave Config informaion array for SC "name"
    uint16_t            num_slave;			//Number of slave with same configuration
} ecat_config_t;
#define MAX_TASK_NUM		(4*2)		//Maximum Task number  //slaves 4 to 8
#define DATAGRAM_PER_TASK	2				// maximum number of datagrams per task

//datagram structure of EtherCAT task
typedef struct ec_task_datagram
{
    struct list_head    list;				//task datagram list
    ec_datagram_t       datagram;			//task datagram
    u16                 station_address;	//station address of EtherCAT slave(for Fix Addressing)
} ec_task_datagram_t;


//EtherCAT task management structure
struct ec_task_top
{
    struct list_head    ec_tasks;			//head of etherCAT task link table
    uint32_t            task_cnt;			//current task number
    uint32_t            task_index;			//index of next task, the operation on task_cnt is just incrementing
};
#define		MAX_SC_PER_TASK     8
//to keep slave information with unfinished SDO transfer
#define		M_CYCLE_SLAVE_NUM   16		//Array size of SDO SLAVE
#define		SDO_DATAGRAM_DATA_LEN   64	//datagram length when SDO Access
//Types of Multiple-Cycle Operation
#define		M_CYCLE_TYP_SDO     0		//SDO
#define		M_CYCLE_TYP_EEPROM  1		//EEPROM Access
#define		M_CYCLE_TYP_INV     0xFFFFFFFF	//Invalid
typedef struct m_cycle_slave
{
    ec_slave_t      *slave;					//slave with unfinished SDO
    ec_datagram_t   *datagram;				//mailbox datagram in slave
    u32             flags;					//Store the type of Multiple cycles operation
} m_cycle_slave_t;
typedef struct ec_task
{
    struct list_head     list;				//task management list which head is ec_tasks of ec_task_top
    ec_master_t          *master;			//EtherCAT master which attached with task
    u32                  task_state;		//State of current task, 0, IDLE, no datagram sent; 1, BUSY, at least 1 datagram sent
    ec_domain_t          *domain;			//Domain attached to task
    uint32_t             task_index;		//Index of current task
    app_ecat_interface_t app_ecat_inter[MAX_SC_PER_TASK];//Interface variable for data exchange
    int                  sc_num;			//number of real Slave(valid app_ecat_inter), must less than MAX_SC_PER_TASK
    m_cycle_slave_t      *m_clcye_slave;	//Record slave information with multiple cycle operation
    u8                   name[20];			//Task name
    u32                  retry_time;		//retry times when error
    struct list_head     task_datagrams;	//EtherCAT datagram list head of the task
} ec_task_t;
typedef struct ec_esc_state_check
{    
    ec_datagram_t datagram;
    u8            data[8];
    u16           al_state;
    u16           al_state_code;
    u16           station_address[MAX_SLV_NUM];	//all slave's address
    u16           cur_station_address;		//current  stateion address
    u16           target_wkc;				//total number of slaves in network
    u16           slave_num;				//current selected slave, not laregr than target_wkc
    u16           state;					//0, read 0x130;1, read AL status code if necessary
    u16           state_change;				//Changing state when 1
    ec_slave_state_t target_fsm;			//used when state_change=1
    int           start;
} ec_esc_state_check_t;

//Application and ECAT master interface
//Ethercat network status
typedef enum {
	ECAT_NET_INIT = 0,						/**< NO network operation */
	ECAT_NET_OK, 							/**< Network Config OK */
	ECAT_NET_EMAC_ERR, 						/**<EMAC Init Fail */
	ECAT_NET_SLAVE_MISS, 					/**<At least one slave fail(not in OP state) */
	ECAT_NET_SLAVE_ERR,						/**<All slaves fail */
	ECAT_NET_MASTER_ERR						/**<Master Error */
} ECAT_NET_STATE_Type;

#define		SLAVE_BITMAP_BIT_WIDTH			16

#define		SLAVE_BITMAP_NUM				((MAX_SLV_NUM-1)/SLAVE_BITMAP_BIT_WIDTH + 1)	//one bit for one slave
typedef struct ecat_app_interface
{    
    u16					real_slave_num;			//Number of slave
	u8					slave_state[MAX_SLV_NUM];//Slave state; 0 for UNknown, 0x1 for INIT, 0x2 for Pre-OP, 0x4 for Safe-OP, 0x8 for OP, 0x10 for ERR
	u32					num_overtime_pdo;
	ECAT_NET_STATE_Type	ecat_net_state;
	uint16_t			last_wkc;
	//one bit for one slave, 0 for OK; 1 for Miss; bit0 of slave_link_bitmap[0] for the First slave
	uint16_t			slave_link_bitmap[SLAVE_BITMAP_NUM];
	uint16_t			reconfig_slave_num;
 } EC_APP_INTER_t;

#define DYN_SYNC_CYCLE_US		500	//in US
#define DYN_STATE_CYCLE_US		100000	//in US

#define DYN_SYNC_CYCLE			(HZ * DYN_SYNC_CYCLE_US/1000000)	//in System timer tick
#define DYN_STATE_CYCLE			(HZ * DYN_STATE_CYCLE_US/1000000)	//in System timer tick


//ESC access Timeout
#define ESC_TIMEOUT				(HZ)
#define MAX_MISS_PKG_NUM		100		//The Maxnum  continuous missed packge
//Maximun GAP between Master and Ref
#define TIMER_GAP   			0
//PDO 
#define RXPDO_SLV_NUM 			8
#define RXPDO_SLV_BUF_LEN   	40
#define RXPDO_CYCLE_US			500			//PDO cycle period in us
#define RXPDO_CYCLE   			(HZ * RXPDO_CYCLE_US/1000000)	//50
//#define RXPDO_PHASE_SHIFT		((u64)((RXPDO_CYCLE/10) * 5))
#define RXPDO_PHASE_SHIFT		((u64)(0))
#define RXPDO_SYNC0_ORIG_SHIFT	(HZ * 120/1000000)
//add datagram to send quene at half time of PDO CYCLE
//#define RXPDO_TIME_QUENE  (RXPDO_CYCLE/2)
#define RXPDO_TIME_QUENE  		0
//10 ticks = 100us; Send and Receive Time, last 100us of PDO clyce, this data only worked when cycle > 250us
#define RXPDO_DGRAM_NUM   		2

#define TXPDO_SLV_NUM 			8
#define TXPDO_SLV_BUF_LEN 		40
//#define TXPDO_CYCLE   ((HZ * 10/1000000) * 25) //in jiffies; (HZ * 10/1000000)->10us
#define TXPDO_CYCLE   			RXPDO_CYCLE
#define TXPDO_TIME_QUENE  		0 //add datagram to send quene at half time of PDO CYCLE

#define DOMAIN_BUF_LEN 			64

//Static Drift Compensation time
#define S_DRIFT_COMPEN_TIME 	(15000*2)	//slaves 4 to 8

#define MASTER_DYN_DGRAM_NUM   2


#define HC_IS580_SLAVE
#define HC_ES750_SLAVE
#define HC_IS620_SLAVE


//Number of Slave in EtherCAT Network
#ifdef BECKHOFF_EL9800_SLAVE
#define NUM_EL9800  			1
#else
#define NUM_EL9800  			0
#endif

#ifdef HC_IS620_SLAVE
#define NUM_IS620   			(4*2)           //slaves 4 to 8
#else
#define NUM_IS620   			0
#endif

#ifdef HC_ES750_SLAVE
#define NUM_ES750   			(4*2)           //slaves 4 to 8
#else
#define NUM_ES750   			0
#endif


#ifdef HC_IS580_SLAVE
#define NUM_IS580   			(4*2)           //slaves 4 to 8
#else
#define NUM_IS580   			0
#endif


#ifdef ESTO_E410_SLAVE
#define NUM_E410    			1
#else
#define NUM_E410    			0
#endif
//SC ID start nunmber
#define SC_ID_START 			0x1
//gonggw, 2015.05.13, main loop Error monitor
#define MAIN_LOOP_TIMEOUT		(RXPDO_CYCLE * 1000)


#if 0
extern ec_datagram_t rxpdo_slv_dgram[RXPDO_SLV_NUM][RXPDO_DGRAM_NUM];
#else
extern ec_datagram_t	rxpdo_slv_dgram[RXPDO_SLV_NUM];
#endif
extern ec_datagram_t	txpdo_slv_dgram[TXPDO_SLV_NUM];
extern uint8_t			pdo_slv_ok[];
extern uint16_t			hc_rxpdo_size[];
extern uint16_t			hc_txpdo_size[];

extern ec_domain_t 		*rx_domain, *tx_domain;
extern ec_datagram_t	rx_domain_dgram;
extern u8				rx_domain_buf[];
extern ec_datagram_t	tx_domain_dgram;
extern u8				tx_domain_buf[];

extern uint16_t			slave_reconfig;

//datagram and buffer for master and reference slave synchronization dynamically
#if 0
extern ec_datagram_t	master_dyn_sync_dgram[];
#else
extern ec_datagram_t	master_dyn_sync_dgram;
#endif
extern u32				master_dyn_sync_start;
extern u32				data_quene;
extern u32				dyn_dgram_ok;					//send dynamic datagram in timer IRQ when 1
extern u32				pdo_cycle_cnt;
extern u32				state_chk;
extern u32				dyn_sync;
extern u64				dyn_sent_jiff64;

//for dynamic slave state check
extern ec_esc_state_check_t esc_state;
//current master
extern ec_master_t		*cur_master;
extern struct ec_task_top ec_task_head;		//defined in app_config.c

extern ec_task_t		*ec_task_hc;
extern ec_task_t		*ec_task_el9800;
extern ec_task_t		*ec_task_e410;

//#define ENDLESS_MON
#ifdef ENDLESS_MON //to endless loop check
extern int				loop_counter;
extern int				endless_chk_start;
extern u32				jiffies_rec;
extern int				loop_counter_reg;
#endif

extern u32				ecat_sr_flag;
extern u32				ecat_rec_flag;
extern volatile u32		jiffies;

//ECAT master and APP interface
extern EC_APP_INTER_t	*ec_app_data_p;

void master_sync_dyn(int send);
void esc_state_check_quene(void);
void ec_timer_isr(void);
void ec_timer_init(void);

void m_cycle_slave_init(void);

#endif

