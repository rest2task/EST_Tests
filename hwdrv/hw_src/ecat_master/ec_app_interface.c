/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    ec_app_interface.c

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:            2014.03.17

Description:    Interfaces for EtherCAT Access

Others:

Function List:  
                    cycle_op_init:                 Initialize variables for cycle access, such as PDO sending
                    drift_compen_dyn:          Dynamic Drift compensation
                    ecat_dc_update_static:    Static slaves synchronization
                    ecat_eeprom_access:       Access EEPROM data in EtherCAT slaves
                    ecat_net_op:                   Real Network Operation, send and receive Ethernet data from Ethrnet module
                    ecat_sdo_access:             SDO access interface
                    esc_access:                     Interface for access EtherCAT slave directly
                    esc_reset:                       Reset specific slave
                    esc_state_check:             ESC(EtherCAT Slave Controller) state check
                    esc_state_check_quene:  Add ESC check datagram to master's quene List, called in Timer IRQ Normally
                    esc_state_check_rcv:      Accept and Process the received state check datagram
                    master_dbg_config:        Modify the current master's Debug level
                    master_sync:                 Master and reference slave synchronization
                    master_sync_dyn:          Add master synchronization(with REF) datagram to master's quene List, 
                                                         called in Timer IRQ Normally
                    master_sync_dyn_check: Accept and Process the received master sync datagram
                    rxpdo_cycle:                   Add RXPDO(Master to Slave) datagram to quene
                    set_esc_state_quene:      Add ESC state setting datagram to quene when current state is not the target state
                    txpdo_cycle:                   Add TXPDO(Slave to Master) datagram to quene
History:    

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/
#include "types.h"
#include "debug.h"
#include "app_data.h"
#include "app_driver_func.h"
#include "globals.h"
#include "master.h"
#include "skbuff.h"
#include "datagram.h"
#include "fsm_sii.h"
#include "fsm_coe.h"
#include "netdevice.h"
//#include "lpc43xx_emac.h"
#include "sdo_request.h"
#include "Ec_mm.h"
#include "Servo.h"
//#include "Lpc43xx_timer.h"
#include "ecat_hal.h"
#include "Wdog.h"
#include "systempro.h"
#include "servo.h"
#include "configdf.h"
#include "machapi.h"

void ecat_cycle_isr(int irq);
void ecat_task_top_init(void);
int  ec_init_module(void);
void ec_fsm_slave_config_state_start(ec_fsm_slave_config_t *);
void ec_fsm_master_enter_write_system_times(ec_fsm_master_t *);
void set_esc_state_quene(int dir);
void serial_putc(uint32_t *uart_port, const char c);
int ec_dev_init(void);
int ecat_dc_update_static(ec_master_t *master);
void SetNetErrCnt(WORD NetErrCnt);



extern ec_sdo_request_t		app_sdo_request; /**< SDO request. */
extern const ec_code_msg_t	al_status_messages[];
extern ec_master_t			*masters; /**< Array of masters. */
extern unsigned int			master_count; /**< Number of masters. */
extern struct board_info	*db;	/* Point a board information structure */
extern struct net_device	*ndev;
u8 flag_adjsync=0;

//Global variables about EtherCAT
u64							dyn_sent_jiff64;

u64							sync_diff;
u32							cycle_send;
u32							dyn_dgram_ok; //send dynamic datagram in timer IRQ when 1
u32							data_quene;
//PDO cycle control
u32							pdo_cycle_cnt;
u32							state_chk;
u32							dyn_sync;

u32							ecat_rec_mon;
u32							ecat_rec_mon_reg;
u32							ecat_rec_wdt_cnt;
u32							ecat_rec_wdt_flag;
int							sys_time_offset;

/**
rxpdo_cycle, write RXPDO periodically, add one PDO package to quene
**/
ec_datagram_t				rxpdo_slv_dgram[RXPDO_SLV_NUM];
u8							rxpdo_slv_buf[RXPDO_SLV_NUM][RXPDO_SLV_BUF_LEN];
uint16_t					hc_rxpdo_size[RXPDO_SLV_NUM];

ec_datagram_t				txpdo_slv_dgram[TXPDO_SLV_NUM];
u8							txpdo_slv_buf[TXPDO_SLV_NUM][TXPDO_SLV_BUF_LEN];
uint16_t					hc_txpdo_size[TXPDO_SLV_NUM];

ec_domain_t 				*rx_domain, *tx_domain;
//ec_datagram_t				rx_domain_dgram;
//u8							rx_domain_buf[DOMAIN_BUF_LEN];
//ec_datagram_t				tx_domain_dgram;
//u8							tx_domain_buf[DOMAIN_BUF_LEN];

EC_APP_INTER_t				ec_app_data;
EC_APP_INTER_t				*ec_app_data_p;


uint8_t						pdo_slv_ok[MAX_SLV_NUM] = {0};

//slave reconfig flag; 0 for no slave reconfig; 1 for config station address; 2 for slave config
uint16_t					slave_reconfig;
/* APP PDO interface */
ec_datagram_t				*app_rxpdo_dgram[RXPDO_SLV_NUM];
ec_datagram_t				*app_txpdo_dgram[TXPDO_SLV_NUM];
u8							*app_rxpdo_buf[RXPDO_SLV_NUM];
u8							*app_txpdo_buf[TXPDO_SLV_NUM];
u8							*rxpdo_state;
//datagram and buffer for master and reference slave synchronization dynamically
ec_datagram_t				master_dyn_sync_dgram;
u8							master_dyn_sync_buf[16];
u32							master_dyn_sync_start = 0;
//Slave drift compensation
ec_datagram_t				slv_drift_dgram;
u8							slv_drift_buf[16];

//Set slave FSM
ec_datagram_t				set_state_dgram;
u8							set_state_buf[16];

//cyclic unit control
ec_datagram_t				cyclic_unit_dyn_sync_dgram;
u8							cyclic_unit_dyn_sync_buf[16];
//for dynamic slave state check
ec_esc_state_check_t		esc_state;

//gonggw, 2015.05.12, Selected SKB pointer
u32							ecat_sr_flag; //Interface with APP, EtherCAT datagrams send/received flag; 0 for received; 1 for moved data to dm9000; 2 for sent data to network
u32							ecat_rec_flag;


int							adj_rate;
s32							time_diff_save;


#ifdef PDO_LOSS_RATE
//rx/tx PDO statistics
u32							rxpdo_send_cnt, rxpdo_rec_cnt;
u32							txpdo_send_cnt, txpdo_rec_cnt;
u32							rxpdo_queue, rxpdo_sent,rxpdo_nosend_cnt;
u32							txpdo_queue, txpdo_sent;
#endif
u32							rxpdo_sync0_ph_offset;

extern int					sys_time_offset;
extern u32					timer_tick;
void slave_bitmap_set(uint16_t idx);
void slave_bitmap_clr(uint16_t idx);

int set_esc_state(ec_slave_state_t target_state, u16 station_address);

 u32 inline get_jiffies()
{
	return jiffies;
}


 u32 inline pdo_cycle_period()
{
	return (u32)RXPDO_CYCLE;
}
void pdo_cycle_sync()
{
	while(jiffies % RXPDO_CYCLE);
}
uint32_t q_cnt[8], t_cnt[8];			//slaves 4 to 8
uint32_t no_sent;

uint32_t ch_rxpdo_send_cnt[4]={0};
uint32_t ch_rxpdo_rec_cnt[4]={0};

void rxpdo_cycle()
{
	int i;
#if 1
	for(i = 0; i < cur_master->slave_count; i++)
	{
		if ((pdo_slv_ok[i] == 0) || \
			((rxpdo_slv_dgram[i].state == EC_DATAGRAM_SENT) && (cur_master->ecat_stage == 0)))
			continue;
#ifdef PDO_LOSS_RATE
		if (no_sent == 1)
		{
			//uart_printf("+");
			no_sent = 0;
			rxpdo_nosend_cnt ++;
		}
		if ((rxpdo_slv_dgram[i].state == EC_DATAGRAM_RECEIVED) && (rxpdo_slv_dgram[i].working_counter == 1))
		{
			rxpdo_rec_cnt++;
			q_cnt[i] = 0;
			t_cnt[i] = 0;
			ch_rxpdo_rec_cnt[i]++;
		}
		else if (rxpdo_slv_dgram[i].state == EC_DATAGRAM_QUEUED)
		{
			rxpdo_queue++;
			q_cnt[i]++;
			//uart_printf("Q");
		}
		else if (rxpdo_slv_dgram[i].state == EC_DATAGRAM_SENT)
		{
			rxpdo_sent++;
			t_cnt[i]++;
			//uart_printf("S");
		}
		rxpdo_send_cnt++;
		ch_rxpdo_send_cnt[i]++;
#endif
		#ifdef EC_RATE_OPT
		if (cur_master->ecat_stage == 2)
		{
			if (rxpdo_slv_dgram[i].state != EC_DATAGRAM_RECEIVED)
				uart_printf("%d", i);
			else if (rxpdo_slv_dgram[i].working_counter != 1)
				uart_printf("%c", ('A'+i));
		}
		#endif
		list_del_init(&(rxpdo_slv_dgram[i].queue));
		ec_datagram_fpwr(&rxpdo_slv_dgram[i],cur_master->slaves[i].station_address, 0x1800, hc_rxpdo_size[i]);
		ec_master_queue_datagram(cur_master, &rxpdo_slv_dgram[i]);
	}
#else
        if (pdo_slv_ok[cur_master->slave_count-1] == 0)
			return;
		list_del_init(&(rx_domain_dgram.queue));
		ec_datagram_lwr(&rx_domain_dgram,rx_domain->logical_base_address, rx_domain->data_size, rx_domain->data);
		ec_master_queue_datagram(cur_master, &rx_domain_dgram);
#endif
}

u32 txpdo_jiffies;
int txpdo_diff;
uint16_t slave_link_check[MAX_SLV_NUM];	//Record the number of continuous missed PDOs
uint16_t slave_miss;					//slave disconnection or poweroff flag

void txpdo_cycle()
{
	int i;
	for(i = 0; i < cur_master->slave_count; i++)
	{
		if ((pdo_slv_ok[i] == 0) || ((txpdo_slv_dgram[i].state == EC_DATAGRAM_SENT) && (cur_master->ecat_stage == 0)))
			continue;
		
		if ((txpdo_slv_dgram[i].state == EC_DATAGRAM_RECEIVED) && (txpdo_slv_dgram[i].working_counter == 1))
		{
			txpdo_diff = jiffies - txpdo_jiffies;
			txpdo_jiffies = jiffies;
			if (slave_link_check[i] < MAX_MISS_PKG_NUM)
			{
				slave_link_check[i] = 0;
				if(slave_miss == 1)
				{
					slave_bitmap_clr(i);
					slave_miss		= 0;
				}
			}
#ifdef PDO_LOSS_RATE
			txpdo_rec_cnt++;
#endif
		}
		else 
		{
#ifdef PDO_LOSS_RATE
			if (txpdo_slv_dgram[i].state == EC_DATAGRAM_QUEUED)
			{	
				txpdo_queue++;
			}
			else if (txpdo_slv_dgram[i].state == EC_DATAGRAM_SENT)
			{
				txpdo_sent++;
			}
#endif
			if ((txpdo_slv_dgram[i].state == EC_DATAGRAM_SENT) || ((txpdo_slv_dgram[i].state == EC_DATAGRAM_RECEIVED) && (txpdo_slv_dgram[i].working_counter == 0)))
			//if ((txpdo_slv_dgram[i].state == EC_DATAGRAM_RECEIVED) && (txpdo_slv_dgram[i].working_counter == 0))
			{
				if (slave_link_check[i] < MAX_MISS_PKG_NUM)
					slave_link_check[i]++;
				if (slave_link_check[i] == MAX_MISS_PKG_NUM)
				{
					slave_bitmap_set(i);
					slave_link_check[i] = 0;
					slave_miss		= 1;
				}
			}
		}
		list_del_init(&(txpdo_slv_dgram[i].queue));
		ec_datagram_fprd(&txpdo_slv_dgram[i],cur_master->slaves[i].station_address, 0x1C00, hc_txpdo_size[i]);
		ec_master_queue_datagram(cur_master, &txpdo_slv_dgram[i]);

#ifdef PDO_LOSS_RATE
		txpdo_send_cnt++;
#endif
	}
}
/**
cycle_op_init, variable initialization for cycle operation
**/

void cycle_op_init()
{
    int i;
    for(i = 0; i < RXPDO_SLV_NUM; i++)
    {
        ec_datagram_init(&rxpdo_slv_dgram[i]);
        rxpdo_slv_dgram[i].data_origin	= EC_ORIG_EXTERNAL;
        rxpdo_slv_dgram[i].data			= rxpdo_slv_buf[i];
		app_rxpdo_dgram[i]				= &(rxpdo_slv_dgram[i]);
		app_rxpdo_buf[i]				= app_rxpdo_dgram[i]->data;
    }
    rxpdo_state							= &(app_rxpdo_dgram[0]->state);
    
    for(i = 0; i < TXPDO_SLV_NUM; i++)
    {
        ec_datagram_init(&txpdo_slv_dgram[i]);
        txpdo_slv_dgram[i].data_origin	= EC_ORIG_EXTERNAL;
        txpdo_slv_dgram[i].data			= txpdo_slv_buf[i];
		app_txpdo_dgram[i]				= &(txpdo_slv_dgram[i]);
		app_txpdo_buf[i]				= app_txpdo_dgram[i]->data;
    }
	//Domain datagram
//	ec_datagram_init(&rx_domain_dgram);
//	ec_datagram_init(&tx_domain_dgram);
//	rx_domain_dgram.data_origin			= EC_ORIG_EXTERNAL;
//	rx_domain_dgram.data				= rx_domain_buf;
//	tx_domain_dgram.data_origin			= EC_ORIG_EXTERNAL;
//	tx_domain_dgram.data				= tx_domain_buf;
	rx_domain							= NULL;
	tx_domain							= NULL;
    //datagram for Master and reference slave synchronization
    ec_datagram_init(&master_dyn_sync_dgram);
    master_dyn_sync_dgram.data_origin   = EC_ORIG_EXTERNAL;
    master_dyn_sync_dgram.data          = master_dyn_sync_buf;
    master_dyn_sync_start               = 0;
    //datagram for slaves drift compensation
    ec_datagram_init(&slv_drift_dgram);
    slv_drift_dgram.data_origin         = EC_ORIG_EXTERNAL;
    slv_drift_dgram.data                = slv_drift_buf;
    //Set State FSM
    ec_datagram_init(&set_state_dgram);
    set_state_dgram.data_origin         = EC_ORIG_EXTERNAL;
    set_state_dgram.data                = set_state_buf;
	//Cyclic unit control
    ec_datagram_init(&cyclic_unit_dyn_sync_dgram);
    cyclic_unit_dyn_sync_dgram.data_origin	= EC_ORIG_EXTERNAL;
    cyclic_unit_dyn_sync_dgram.data		= cyclic_unit_dyn_sync_buf;
	
    //state check
    ec_datagram_init(&(esc_state.datagram));
    esc_state.datagram.data_origin      = EC_ORIG_EXTERNAL;
    esc_state.datagram.data             = esc_state.data;
    esc_state.al_state                  = 0;
    esc_state.al_state_code             = 0;
    esc_state.cur_station_address       = 0;
    esc_state.slave_num                 = 0;
    esc_state.state                     = 0;
    esc_state.state_change              = 0;
    esc_state.target_wkc                = 1;
    cycle_send                          = 0;
    data_quene                          = 0;
    for(i = 0; i < MAX_SLV_NUM; i++)
    {
        pdo_slv_ok[i]					= 0;
		slave_link_check[i]				= 0;
        esc_state.station_address[i]	= 0;
    }
	slave_miss						= 0;
	slave_reconfig						= RECONFIG_IDLE;
	ecat_sr_flag						= 0;
	ecat_rec_flag						= 0;
	adj_rate							= 0;
	time_diff_save						= 0;
#ifdef PDO_LOSS_RATE
	rxpdo_rec_cnt						= 0;
	rxpdo_send_cnt						= 0;
	txpdo_rec_cnt						= 0;
	txpdo_send_cnt						= 0;
	rxpdo_queue							= 0;
	rxpdo_sent							= 0;
	txpdo_queue							= 0;
	txpdo_sent							= 0;
#endif
	ec_app_data_p						= &ec_app_data;
	ec_app_data_p->ecat_net_state		= ECAT_NET_INIT;
	ec_app_data_p->last_wkc				= 0xFFFF;
	ec_app_data_p->reconfig_slave_num	= 0xFFFF;
	for(i = 0; i < SLAVE_BITMAP_NUM; i++)
	{
		ec_app_data_p->slave_link_bitmap[i] = 0;
	}
	rxpdo_sync0_ph_offset = RXPDO_PHASE_SHIFT;
}
/**
master_dbg_config, config Debug level
@debug_level, 0 for least debug info; 3 for most
**/
void master_dbg_config(unsigned int debug_level)
{
    ec_master_t  *master = cur_master;
    master->debug_level = debug_level;
}


/**
esc_access, Access ESC with FPWR/FPRD
@master                  : ECAT master pointer
@datagram              : Ecat datagram pointer
@station_address     :  ESC's station address
@mem_address        : Memory address in target ESC
@data_size              : data length in byte]
@dir                        : 0, Read; 1, Write
@send                     : 1, quene, send and receive; 0 only quene
return                      : 0, sucess; -1, data length error
**/
int esc_access(ec_master_t  *master, ec_datagram_t *datagram, u16 station_address, 
                    u16 mem_address, u16 data_size, u16 dir, int send)
{
    int ret = 0;
    if (dir) //Write
    {
        ec_datagram_fpwr(datagram,station_address, mem_address, data_size); 
    }
    else //READ
    {
        ec_datagram_fprd(datagram,station_address, mem_address, data_size);
        ec_datagram_zero(datagram);
    }
    ec_master_queue_datagram(master, datagram);
    if (send)
    {
        ecrt_master_send(master);
        do
        {
            ecrt_master_receive(master);
        }while((datagram->state == EC_DATAGRAM_SENT) && ((int)(jiffies - datagram->jiffies_sent) < ESC_TIMEOUT));
        if (datagram->state != EC_DATAGRAM_RECEIVED)
            ret = -1;
    }
    return ret;
}
/**
ecat_eeprom_access, Access(Read/Write) EEPROM of ESC, read 4 or 8 bytes or write 2 bytes per time
@app_req                    : request from Application
@master                     :  ECAT Master pointer
@return                     :  0, sucess; -1, data length error
**/
int ecat_eeprom_access(EcatReq_t *app_req, ec_master_t *master)
{
	ec_fsm_sii_t    *sii_fsm    = (ec_fsm_sii_t *)(&(master->fsm.fsm_sii));
    int             data_len;
    u8              *data_buf   = (u8 *)((u8 *)app_req + EC_OFF_DATA);
    data_len                    = app_req->DataLen - EC_OFF_DATA;
    if ((data_len <= 0) || (data_len > 8))
    {
        debug("data length error, return\n");
        return -1;
    }
    sii_fsm->mode               = EC_FSM_SII_USE_CONFIGURED_ADDRESS; //use fixed station address
    sii_fsm->word_offset        = app_req->OdSubIndex;
    sii_fsm->length             = (data_len > 4) ? 8 : 4;
    if (GET_CON_ACCESS(app_req->ConWord) == DEV_OP_READ)
       ec_fsm_sii_read(sii_fsm, sii_fsm->slave, app_req->OdSubIndex, EC_FSM_SII_USE_CONFIGURED_ADDRESS);
    else if(GET_CON_ACCESS(app_req->ConWord) == DEV_OP_WRITE)
       ec_fsm_sii_write(sii_fsm, sii_fsm->slave, app_req->OdSubIndex, (u16 *)data_buf, EC_FSM_SII_USE_CONFIGURED_ADDRESS);
    else   
        return -1;
    
    if(ec_fsm_sii_exec(sii_fsm))
    {
        ec_master_queue_datagram(master, sii_fsm->datagram);
        return 0;
    }
    else
        return -1;
}

void ecat_sdo_fail(EcatReq_t *app_req, ec_master_t *master, ec_slave_t *slave)
{
	master->fsm.fsm_coe.state = NULL;
	
}
/**
ecat_sdo_access, Access(Read/Write) SDO of CANOpen
@app_req                    : request from Application
@master                     : ECAT Master pointer
@slave                       : target slave pointer
@return                     : 0, sucess; -1,  Operation error; -2, COE transfer error
**/
int ecat_sdo_access(EcatReq_t *app_req, ec_master_t *master, ec_slave_t *slave)
{
    ec_sdo_request_address(&app_sdo_request, app_req->OdIndex, app_req->OdSubIndex>>8);
    if (GET_CON_ACCESS(app_req->ConWord) == DEV_OP_READ)
        ecrt_sdo_request_read(&app_sdo_request);
    else if (GET_CON_ACCESS(app_req->ConWord) == DEV_OP_WRITE)
        ecrt_sdo_request_write(&app_sdo_request);
    else
        return -1;
    
    ec_fsm_coe_transfer(&(master->fsm.fsm_coe), slave, &app_sdo_request);
    if (ec_fsm_coe_exec(&(master->fsm.fsm_coe))) // execute immediately
    {
        ec_master_queue_datagram(master, master->fsm.fsm_coe.mbox->datagram);
        return 0;
    }
    else
    {
        return -2;
    }
}
/**
esc_reset: reset ESC(EtherCAT Slave Controller) with address "station_address"
@station_address            : station address to be reset
@master                         : master pointer
@return                          : 0, sucess; -1, Reset failure
**/
int esc_reset(u16 station_address, ec_master_t *master)
{
    ec_datagram_t   datagram;
    u8              data_buf[32];
    int             i, timeout;
	ec_datagram_init(&datagram);
    i = 0;
	timeout = 0;
	datagram.data = data_buf;
	datagram.data_origin = EC_ORIG_EXTERNAL;
    do
    {
        if (station_address == 0)
        {
            ec_datagram_bwr(&datagram,0x40, 0x1);
        }
        else
        {
            ec_datagram_apwr(&datagram,station_address, 0x40, 0x1);
        }
        if (i == 0)
            data_buf[0] = 'R';
        else if (i == 1)
            data_buf[0] = 'E';
        else
            data_buf[0] = 'S';
		timeout = 0;
		while(1)
		{
			ec_master_queue_datagram(master, &datagram);
            ecrt_master_send(master);
			if (i == 2)
				break;
			
            do
            {
            	ecrt_master_receive(master);
 			} while(datagram.state == EC_DATAGRAM_SENT);
  
			if (datagram.state != EC_DATAGRAM_RECEIVED)
			{
				if (timeout < 5)
				{
					timeout++;
					continue;
				}
				else
					return -1;
			}
 			else
 			{
				break;
			}
 		}
        i++;
	  } while(i < 3);
    return 0;
}
/**
esc_state_check             : check  the state of slave with address station_address
@station_address           : station address of target ESC
@return value                : al_state when ESC is OK; al_code when ESC is error; 0xff when ESC access failure
**/
u16 esc_state_check(ec_master_t  *master, u16 station_address)
{
    int ret;
    const ec_code_msg_t *al_msg;
    ec_datagram_t datagram;
    u8            data[8];
    u16           al_state;
    u16           al_state_code;

    ec_datagram_init(&datagram);
    datagram.data_origin = EC_ORIG_EXTERNAL;
    datagram.data        = data;
   
    ret = esc_access(master, &datagram, station_address, 0x130, 2, 0, 1);
    if (ret)
        return 0xff;
    al_state= EC_READ_U16(datagram.data);
    if (al_state & 0x10) //ESC Error
    {
        ret = esc_access(master, &datagram, station_address, 0x134, 2, 0, 1);
        if (ret)
            return 0xff;
        al_state_code = EC_READ_U16(datagram.data);
    
        for (al_msg = al_status_messages; al_msg->code != 0xffff; al_msg++) {
            if (al_msg->code != (u32)al_state_code) {
                continue;
            }
        }
        if (al_msg->code == 0xffff)
            _DBG_PRINTF_("Unknown AL status(%x)\n", al_state_code);
        else
            _DBG_PRINTF_("AL status message 0x%04X: \"%s\".\n",
                     al_msg->code, al_msg->message);

        return al_state_code;
    }
    else
        return al_state;
}

/**
esc_state_check_quene  : add ESC state check datagram to quene
@station_address           : station address of target ESC
**/
void esc_state_check_quene()
{
	ec_master_t  *master = cur_master;
	if (esc_state.start == 0)
		 return;
	if (esc_state.state_change == 1)
		 return;
    
	if (esc_state.datagram.state == EC_DATAGRAM_QUEUED)
	//if ((esc_state.datagram.state == EC_DATAGRAM_QUEUED) || (esc_state.datagram.state == EC_DATAGRAM_SENT))
	{
		 return;
	 }
	else
	{
		 list_del_init(&(esc_state.datagram.queue));     
	 }

	if (esc_state.state == 0)
	 {
		ec_datagram_brd(&(esc_state.datagram), 0x130, 2);
	}
	else if (esc_state.state == 1)
	{
		ec_datagram_fprd(&(esc_state.datagram), esc_state.cur_station_address, 0x130, 2);
	}
	else if (esc_state.state == 2)
	{
		ec_datagram_fprd(&(esc_state.datagram), esc_state.cur_station_address, 0x134, 2);
	}
	else
	{
		ec_datagram_brd(&(esc_state.datagram), 0x130, 2);
		esc_state.cur_station_address = 0;
		esc_state.state = 0;
	}

	ec_datagram_zero(&(esc_state.datagram));
	ec_master_queue_datagram(master, &(esc_state.datagram));
}
/**
esc_state_check_rcv       : check the received data after esc_state_check_quene
**/
uint16_t esc_state_miss_cnt = 0;
uint16_t esc_state_pkg_miss_cnt;
void esc_state_check_rcv()
{
	if (esc_state.state_change == 0)
	{
		if (esc_state.datagram.state == EC_DATAGRAM_RECEIVED)
		{
			esc_state_miss_cnt = 0;
			esc_state.datagram.state = EC_DATAGRAM_INIT;
			if (esc_state.state == 0)
			{
#if 0
				if (esc_state.datagram.working_counter == esc_state.target_wkc)
#endif
				{
					esc_state.al_state = EC_READ_U16(esc_state.datagram.data);
					if ((slave_miss == 1) && (esc_state.al_state & ((u16)EC_SLAVE_STATE_INIT)))
					{
						if (slave_reconfig == RECONFIG_IDLE)
							slave_reconfig = RECONFIG_START;
					}
					ec_app_data_p->last_wkc = esc_state.datagram.working_counter;
					if(esc_state.al_state != EC_SLAVE_STATE_OP)
					{
						//cur_master->debug_level = 3;
						if (slave_reconfig !=  RECONFIG_IDLE)
							return;
						dyn_dgram_ok = 0;
						esc_state.slave_num = 0;
  						esc_state.state = 1;
						if (esc_state.al_state & 0x10)
							 esc_state.al_state = 0x14; //force AL Status to Error and SafeOP
						else
						{
							if(esc_state.al_state % 2)
								 esc_state.al_state = (u16)EC_SLAVE_STATE_INIT;
							else if (esc_state.al_state % 4)
								 esc_state.al_state = (u16)EC_SLAVE_STATE_PREOP;
							else
								esc_state.al_state = (u16)EC_SLAVE_STATE_SAFEOP; 
						}
						esc_state.cur_station_address = esc_state.station_address[esc_state.slave_num];
 						esc_state_check_quene();
 						dyn_dgram_ok = 1;
					}
					else
					{
						if (esc_state.datagram.working_counter < esc_state.target_wkc)
						{
							esc_state.slave_num = 0;
							esc_state.state = 1;							
							esc_state.al_state = (u16)EC_SLAVE_STATE_OP; 
							esc_state.cur_station_address = esc_state.station_address[esc_state.slave_num];
							esc_state_check_quene();
						}
					}
				}
			}
			else if (esc_state.state == 1)
			{
				if ((EC_READ_U16(esc_state.datagram.data) == (u16)EC_SLAVE_STATE_OP) || \
				 	(esc_state.datagram.working_counter == 0))
				{
					if (esc_state.datagram.working_counter == 0)
					{
						if (esc_state_miss_cnt < 10)
						{
							esc_state_miss_cnt++;
						}
						else
						{
					 		slave_bitmap_set(esc_state.slave_num - 1);
							slave_reconfig = RECONFIG_START;
					 	}
					}
					else
					{
						esc_state_miss_cnt = 0;
					}
					if (esc_state.slave_num < (esc_state.target_wkc - 1))
 				 	{
						esc_state.slave_num++;
						dyn_dgram_ok = 0;
						esc_state.cur_station_address = esc_state.station_address[esc_state.slave_num];
						esc_state_check_quene();
						dyn_dgram_ok = 1;
				 	}
				 	else
				 	{
						dyn_dgram_ok                    = 0;
						esc_state.cur_station_address   = 0;
						esc_state.slave_num             = 0;
						esc_state.al_state              = EC_SLAVE_STATE_OP;
						esc_state.state                 = 0;
						dyn_dgram_ok                    = 1;
				 	}
 				}
				else
				{
					 if(esc_state.al_state & 0x10)
					{
						dyn_dgram_ok                    = 0;
					 	esc_state.state                 = 2;
					 	esc_state_check_quene();
					 	esc_state.target_fsm            = EC_SLAVE_STATE_OP;
					 	dyn_dgram_ok = 1;                        
					}
					else
					{
						dyn_dgram_ok                    = 0;
						esc_state.state_change          = 1;
						esc_state.target_fsm            = EC_SLAVE_STATE_OP;
					 	set_esc_state_quene(0);
					 	dyn_dgram_ok                    = 1;
					}
				}
			}
			else if (esc_state.state == 2)
			{
				esc_state.al_state_code = EC_READ_U16(esc_state.datagram.data);
				if (esc_state.al_state_code)
				{
					dyn_dgram_ok                    = 0;
					esc_state.state_change          = 1;
					esc_state.target_fsm            = EC_SLAVE_STATE_OP;
					set_esc_state_quene(0);
					dyn_dgram_ok                    = 1;
				 }
				else
			 	{
					dyn_dgram_ok                    = 0;
				 	esc_state.state_change          = 1;
				 	esc_state.target_fsm            = EC_SLAVE_STATE_OP;
					set_esc_state_quene(0);
				 	dyn_dgram_ok                    = 1;
			 	}
			}
		}
		else
		{
			ec_app_data_p->last_wkc = 0;
#if 0
			if (esc_state.datagram.state == EC_DATAGRAM_SENT)
			{
				if (esc_state_miss_cnt < 10)
					esc_state_miss_cnt++;
				else
					slave_reconfig = RECONFIG_START;
			}
#endif
		}
	}
	else
	{
		if (((set_state_dgram.state == EC_DATAGRAM_SENT) && ((int)(jiffies - set_state_dgram.jiffies_sent) > ESC_TIMEOUT)) || \
			(set_state_dgram.state == EC_DATAGRAM_ERROR) || (set_state_dgram.state == EC_DATAGRAM_TIMED_OUT) || (set_state_dgram.state == EC_DATAGRAM_INIT))
		{
			dyn_dgram_ok                            = 0;
			set_state_dgram.state                   = EC_DATAGRAM_INIT;
			 list_del_init(&(set_state_dgram.queue));
			 if (set_state_dgram.type == EC_DATAGRAM_FPRD)
				 set_esc_state_quene(0);
			 else
				set_esc_state_quene(1);
			 dyn_dgram_ok                            = 1;
		 }
		else if (set_state_dgram.state == EC_DATAGRAM_RECEIVED)
		 {
			 set_state_dgram.state = EC_DATAGRAM_INIT;
			if (set_state_dgram.working_counter == 1)
			{
				if (set_state_dgram.type == EC_DATAGRAM_FPRD)
				{
					ec_slave_state_t req_state;
					esc_state.al_state = EC_READ_U16(set_state_dgram.data);
					if (esc_state.al_state == esc_state.target_fsm)
					{
						dyn_dgram_ok                    = 0;
						slave_bitmap_clr(esc_state.cur_station_address - 1);
						esc_state.cur_station_address   = 0;
						esc_state.slave_num             = 0;
						esc_state.al_state              = EC_SLAVE_STATE_OP;
						esc_state.state                 = 0;
						esc_state.state_change          = 0;
						dyn_dgram_ok                    = 1;
					}
					else 
					{
						if (esc_state.al_state & 0x10)
						{
							req_state = (ec_slave_state_t)(esc_state.al_state);
						}
						else if (esc_state.target_fsm < esc_state.al_state)
						{
							req_state = esc_state.target_fsm;
						}
						else
						{
							switch(esc_state.al_state)
							{
								 case EC_SLAVE_STATE_SAFEOP :
									req_state = EC_SLAVE_STATE_OP;
									 break;
								 case EC_SLAVE_STATE_PREOP :
									req_state = EC_SLAVE_STATE_SAFEOP;
									 break;
								case EC_SLAVE_STATE_INIT :
									 req_state = EC_SLAVE_STATE_PREOP;
									 break;
								 default :
									 req_state = EC_SLAVE_STATE_PREOP;
							}
						 }
					}
					EC_WRITE_U16(set_state_dgram.data, (u16)req_state);
					dyn_dgram_ok = 0;
					set_esc_state_quene(1);
					dyn_dgram_ok = 1;
				}
				else //FPWR
				{
					 dyn_dgram_ok = 0;
					 set_esc_state_quene(0);
					 dyn_dgram_ok = 1;
				}
			}
			else
			 {
		 		dyn_dgram_ok = 0;
				if (set_state_dgram.type == EC_DATAGRAM_FPRD)
					set_esc_state_quene(0);
				else
					set_esc_state_quene(1);
				dyn_dgram_ok = 1;
			}
		}
	}
}
/**
master_sync_dyn           : Clock synchronization between master and reference dynamically
@send                           : 1, quene, send and receive; 0 only quene
**/
void cyclic_unit_dyn()
{
    ec_master_t *master = cur_master;
    if (master_dyn_sync_start == 0)
        return;
    if ((cyclic_unit_dyn_sync_dgram.state != EC_DATAGRAM_RECEIVED) && (cyclic_unit_dyn_sync_dgram.state != EC_DATAGRAM_INIT))
    {
        cyclic_unit_dyn_sync_dgram.state = EC_DATAGRAM_INIT;
        list_del_init(&(cyclic_unit_dyn_sync_dgram.queue));
    }
    ec_datagram_fprd(&cyclic_unit_dyn_sync_dgram,master->slave_count, 0x990, 0x8);
    ec_datagram_zero(&cyclic_unit_dyn_sync_dgram);
    ec_master_queue_datagram(master, &cyclic_unit_dyn_sync_dgram);
}
/**
master_sync_dyn_check : check if dynamic master sync has started and adjust timer if necessary
**/
uint32_t sync0_reg, sync0_cnt;
uint64_t next_sync0_reg;
uint8_t	sync0_state = 0;
void cyclic_unit_dyn_check()
{
	volatile u64 nex_sync0_time;
	volatile uint32_t sync0_setup, pdo_setup;
	if (master_dyn_sync_start == 0)
		 return;
	
	if (cyclic_unit_dyn_sync_dgram.state == EC_DATAGRAM_RECEIVED)
	{
		cyclic_unit_dyn_sync_dgram.state = EC_DATAGRAM_INIT;
		if (cyclic_unit_dyn_sync_dgram.working_counter == 1)
		{
			nex_sync0_time	= EC_READ_U64(cyclic_unit_dyn_sync_dgram.data);
			if (sync0_state == 0)
			{
				if (nex_sync0_time != next_sync0_reg)
				{
					next_sync0_reg = nex_sync0_time;
					sync0_cnt++;
					if (sync0_cnt > 2000)
					{
						sync0_state = 1;
						sync0_cnt = 0;
					}
				}
				else
				{
					sync0_cnt = 0;
				}
			}
			else if (sync0_state == 1)
			{
				do_div((u64 *)(&nex_sync0_time), (u32)(S_IN_NS/HZ));
				sync0_setup = nex_sync0_time%RXPDO_CYCLE;
				if (sync0_setup == sync0_reg)
				{
					sync0_cnt++;
					if (sync0_cnt > 5000)
					{
						sync0_setup = (sync0_setup < RXPDO_SYNC0_ORIG_SHIFT) ? (sync0_setup + RXPDO_CYCLE) : sync0_setup;
						//rxpdo_sync0_ph_offset = sync0_setup - RXPDO_SYNC0_ORIG_SHIFT;
						//rxpdo_sync0_ph_offset = RXPDO_CYCLE - sync0_setup - RXPDO_SYNC0_ORIG_SHIFT;
						sync0_state = 2;
						sync0_cnt = 0;
					}
				}
				else
				{
					sync0_reg = sync0_setup;
					sync0_cnt=0;
				}
				
			}
           	cyclic_unit_dyn_sync_dgram.state = EC_DATAGRAM_INIT;
		}
	}
}

/**
master_sync_dyn           : Clock synchronization between master and reference dynamically
@send                           : 1, quene, send and receive; 0 only quene
**/
void master_sync_dyn(int send)
{
    ec_master_t *master = cur_master;
    u64 sys_time_ref;
    s32 time_diff;
    if (master_dyn_sync_start == 0)
        return;
    if ((master_dyn_sync_dgram.state != EC_DATAGRAM_RECEIVED) && (master_dyn_sync_dgram.state != EC_DATAGRAM_INIT))
    {
        master_dyn_sync_dgram.state = EC_DATAGRAM_INIT;
        list_del_init(&(master_dyn_sync_dgram.queue));
    }
    ec_datagram_fprd(&master_dyn_sync_dgram,master->dc_ref_clock->station_address, 0x910, 0x8);
    ec_datagram_zero(&master_dyn_sync_dgram);
    ec_master_queue_datagram(master, &master_dyn_sync_dgram);
    if (send)
    {
    	ecrt_master_send(master);
    	do
    	{
    	    ecrt_master_receive(master);
    	}while(master_dyn_sync_dgram.state != EC_DATAGRAM_RECEIVED);
    	
    	master_dyn_sync_dgram.state = EC_DATAGRAM_INIT;
    	sys_time_ref = EC_READ_U64(master_dyn_sync_dgram.data);
    	do_div(&sys_time_ref, (S_IN_NS/HZ));
#if 0	//gonggw, 2015.04.28, master's time is synchronized with reference slave's system  time
    	time_diff = sys_time_ref - jiffies_64;
#else
		time_diff = sys_time_ref - (jiffies_64 - rxpdo_sync0_ph_offset);
#endif
    	if (time_diff > TIMER_GAP)
    	{
    	    timer_adjust(-1);
    	}
    	else if (time_diff < -TIMER_GAP)
    	{
    	    timer_adjust(1);
    	}
    	else
    	{
    	     timer_adjust(0);
    	}
	}
}
/**
master_sync_dyn_check : check if dynamic master sync has started and adjust timer if necessary
**/
#define ECAT_SYNC 2
#if (ECAT_SYNC == 0)
void master_sync_dyn_check()
{
	u64 sys_time_ref;
	int time_diff, time_diff_abs;
	u32 rem;
	if (master_dyn_sync_start == 0)
		 return;
	
	if (master_dyn_sync_dgram.state == EC_DATAGRAM_RECEIVED)
	 {
		 master_dyn_sync_dgram.state = EC_DATAGRAM_INIT;
		if (master_dyn_sync_dgram.working_counter == 1)
		{
			sys_time_ref = EC_READ_U64(master_dyn_sync_dgram.data);
			 rem = do_div(&sys_time_ref, (S_IN_NS/HZ));
			if (sys_time_offset == 0x55AA55AA)
			{
				//if (sync0_state == 2)
				{
					jiffies_64 = sys_time_ref + rxpdo_sync0_ph_offset;
					jiffies = (u32)jiffies_64;
					sys_time_offset = 0x44BB44BB;
				}
			}
			else if (sys_time_offset == 0x44BB44BB)
			{
				//sys_time_offset = (int)(sys_time_ref - (dyn_sent_jiff64 - rxpdo_sync0_ph_offset));
				sys_time_offset = (int)(RXPDO_CYCLE/2);
				//sys_time_offset = 0;
			}
			else
			{
				time_diff = (int)(sys_time_ref - (dyn_sent_jiff64 - rxpdo_sync0_ph_offset) - sys_time_offset);
				time_diff_abs = (time_diff < 0) ? (-time_diff) : time_diff;
				if (time_diff > 0)
				{
					if (((jiffies + time_diff) % RXPDO_CYCLE) < (jiffies % RXPDO_CYCLE))
						ecat_cycle_isr(0);
				}
				//if ((time_diff != 0) && (time_diff_abs < 100))
				if (time_diff != 0)
				{
					jiffies_64 = jiffies_64 + time_diff;
					jiffies    = (u32)jiffies_64;
				}
			}
           	 master_dyn_sync_dgram.state = EC_DATAGRAM_INIT;
		}
	}
}

#elif (ECAT_SYNC == 1) 
void master_sync_dyn_check()
{
	u64 sys_time_ref;
	int time_diff;//, time_diff_abs;
	static u8 flag_first_sync=0;
	int remain_50=0;
//	u32 rem;
	if (master_dyn_sync_start == 0)
		 return;
	
	 if (master_dyn_sync_dgram.state == EC_DATAGRAM_RECEIVED)
	 {
		 master_dyn_sync_dgram.state = EC_DATAGRAM_INIT;
		if (master_dyn_sync_dgram.working_counter == 1)
		{
			sys_time_ref = EC_READ_U64(master_dyn_sync_dgram.data);
			 //rem = do_div(&sys_time_ref, (S_IN_NS/HZ));
#ifdef EC_RATE_OPT			
			do_div(&sys_time_ref, (S_IN_NS/HZ));
#else
			do_div(&sys_time_ref, 10000);
#endif		
			if (sys_time_offset == 0x55AA55AA)
			{
				//if (sync0_state == 2)
				{
					jiffies_64 = sys_time_ref + rxpdo_sync0_ph_offset;
					jiffies = (u32)jiffies_64;
					sys_time_offset = 0x44BB44BB;
				}
			}
			else if (sys_time_offset == 0x44BB44BB)
			{
				//sys_time_offset = (int)(sys_time_ref - (dyn_sent_jiff64 - rxpdo_sync0_ph_offset));
				sys_time_offset = (int)(RXPDO_CYCLE/2);
				//sys_time_offset = 0;
			}
			else
			{
				remain_50=sys_time_ref%RXPDO_CYCLE;
				remain_50=RXPDO_CYCLE/2-remain_50;
				if(flag_first_sync==0)
				{
					time_diff = (int)(sys_time_ref - (dyn_sent_jiff64 - rxpdo_sync0_ph_offset) - sys_time_offset);
			
					flag_first_sync=1;
					if (time_diff > 0)
					{
					#ifdef EC_RATE_OPT			
						if (((jiffies + time_diff) % RXPDO_CYCLE) < (jiffies % RXPDO_CYCLE))
					#else
						if (((jiffies + time_diff) % 50) < (jiffies % 50))
					#endif	
							ecat_cycle_isr(0);
					}
					//if ((time_diff != 0) && (time_diff_abs < 100))
					if (time_diff != 0)
					{
						jiffies_64 = jiffies_64 + time_diff;
						jiffies    = (u32)jiffies_64;
					}

				}
				else if(flag_first_sync==1)
				{
					if(abs(remain_50)>10)
					{
						/*Watch_zhoutuo[0]++;
						jiffies_64 = jiffies_64 + remain_50;
						jiffies    = (u32)jiffies_64;
						flag_adjsync=1;
						*/
						if(remain_50 < 0)
							remain_50 = remain_50 + RXPDO_CYCLE/2;
						jiffies_64 = jiffies_64 + remain_50;
						jiffies    = (u32)jiffies_64;
						flag_adjsync=1;
					//	Watch_zhoutuo[0]++;
					}
					/*
					else
					{
						time_diff = (int)(sys_time_ref - (dyn_sent_jiff64 - rxpdo_sync0_ph_offset) - sys_time_offset);
						if (time_diff > 0)
						{
							if (((jiffies + time_diff) % RXPDO_CYCLE) < (jiffies % RXPDO_CYCLE))
								ecat_cycle_isr(0);
						}
						//if ((time_diff != 0) && (time_diff_abs < 100))
						if (time_diff != 0)
						{
							jiffies_64 = jiffies_64 + time_diff;
							jiffies    = (u32)jiffies_64;
						}
					}*/
				}
				else
				{
				}
						
			}
           	 master_dyn_sync_dgram.state = EC_DATAGRAM_INIT;
		}
	}
}

#else   //chenhao 2019/12/3 8:21:45
void master_sync_dyn_check()
{
	u64 sys_time_ref;
	int time_diff;//, time_diff_abs;
	static u8 flag_first_sync=0;
	int remain_50=0;
//	u32 rem;
	if (master_dyn_sync_start == 0)
		 return;

	 if (master_dyn_sync_dgram.state == EC_DATAGRAM_RECEIVED)
	 {
		 master_dyn_sync_dgram.state = EC_DATAGRAM_INIT;
		if (master_dyn_sync_dgram.working_counter == 1)
		{
			sys_time_ref = EC_READ_U64(master_dyn_sync_dgram.data);
			 //rem = do_div(&sys_time_ref, (S_IN_NS/HZ));
#ifdef EC_RATE_OPT
			do_div(&sys_time_ref, (S_IN_NS/HZ));
#else
			do_div(&sys_time_ref, 10000);
#endif
			if (sys_time_offset == 0x55AA55AA)
			{
				//if (sync0_state == 2)
				{
					jiffies_64 = sys_time_ref + rxpdo_sync0_ph_offset;
					jiffies = (u32)jiffies_64;
					sys_time_offset = 0x44BB44BB;
				}
			}
			else if (sys_time_offset == 0x44BB44BB)
			{
				//sys_time_offset = (int)(sys_time_ref - (dyn_sent_jiff64 - rxpdo_sync0_ph_offset));
				sys_time_offset = (int)(RXPDO_CYCLE/2);
				//sys_time_offset = 0;
			}
			else
			{
				remain_50=sys_time_ref%RXPDO_CYCLE;
				remain_50=RXPDO_CYCLE/2-remain_50;
				if(flag_first_sync==0)
				{
					time_diff = (int)(sys_time_ref - (dyn_sent_jiff64 - rxpdo_sync0_ph_offset) - sys_time_offset);

					flag_first_sync=1;
					if (time_diff > 0)
					{
					#ifdef EC_RATE_OPT
						if (((jiffies + time_diff) % RXPDO_CYCLE) < (jiffies % RXPDO_CYCLE))
					#else
						if (((jiffies + time_diff) % 50) < (jiffies % 50))
					#endif
							ecat_cycle_isr(0);
					}
					//if ((time_diff != 0) && (time_diff_abs < 100))
					if (time_diff != 0)
					{
						jiffies_64 = jiffies_64 + time_diff;
						jiffies    = (u32)jiffies_64;
					}

				}
				else if(flag_first_sync==1)
				{
					if(abs(remain_50)>2)
					{
						if(remain_50 < 0)
						{
							remain_50 = remain_50 + RXPDO_CYCLE/2;
						}
						jiffies_64 = jiffies_64 + remain_50;
						jiffies    = (u32)jiffies_64;
						flag_adjsync=1;
					}
					if (ComIsOk())
					{
					    flag_first_sync = 2;
					}
				}
				else if(flag_first_sync == 2)
				{
    				time_diff = (int)(sys_time_ref - (dyn_sent_jiff64 - rxpdo_sync0_ph_offset) - sys_time_offset);
    				if (time_diff > 0)
    				{
    					if (((jiffies + time_diff) % RXPDO_CYCLE) < (jiffies % RXPDO_CYCLE))
    						ecat_cycle_isr(0);
    				}
    				if (time_diff != 0)
    				{
    					jiffies_64 = jiffies_64 + time_diff;
    					jiffies    = (u32)jiffies_64;
    				}
					if (!ComIsOk())
					{
					    flag_first_sync = 1;
					}
				}
				else
				{
				}

			}
           	 master_dyn_sync_dgram.state = EC_DATAGRAM_INIT;
		}
	}
}
#endif
/**
master_sync: Clock synchronization between master and reference
@master						: master pointer
@return value, 0, success; -1 fail
**/
#define	USE_RING_POSITION
int master_sync(ec_master_t *master)
{
	u64 sys_time_ref;
	ec_datagram_t datagram0;
	u8  data[64];
	u64 jiffies_ns;
	
	ec_datagram_init(&datagram0);
	 datagram0.data_origin = EC_ORIG_EXTERNAL;
	datagram0.data        = data;
	ecat_dc_update_static(master);
	while(1)
	{
#ifdef USE_RING_POSITION
		ec_datagram_aprd(&datagram0,master->dc_ref_clock->ring_position, 0x910, 0x8);
#else
		ec_datagram_fprd(&datagram0,master->dc_ref_clock->station_address, 0x910, 0x8);
#endif
		 ec_datagram_zero(&datagram0);
		 ec_master_queue_datagram(master, &datagram0);
		 ecrt_master_send(master);
		do
		 {
			 ecrt_master_receive(master);
		 }while((datagram0.state == EC_DATAGRAM_SENT) && ((u32)(jiffies - datagram0.jiffies_sent) < ESC_TIMEOUT));
        
		if ((datagram0.state == EC_DATAGRAM_RECEIVED) && (datagram0.working_counter == 1))
		 {
			 break;
		}
		else
		 {
			 list_del_init(&datagram0.queue);
			 return -1;
		}
	}    
#if 0
		{
			ec_datagram_t datagram0, datagram1;
			ec_datagram_t datagram2, datagram3;
			u8	datagram0_data[32], datagram1_data[32];
			u8	datagram2_data[32], datagram3_data[32];
			u64 sys_time[4];
			ec_datagram_init(&datagram0);
			datagram0.data_origin = EC_ORIG_EXTERNAL;
			datagram0.data = datagram0_data;	 

			ec_datagram_init(&datagram1);
			datagram1.data_origin = EC_ORIG_EXTERNAL;
			datagram1.data = datagram1_data;	 

			ec_datagram_init(&datagram2);
			datagram2.data_origin = EC_ORIG_EXTERNAL;
			datagram2.data = datagram2_data;	 
			
			ec_datagram_init(&datagram3);
			datagram3.data_origin = EC_ORIG_EXTERNAL;
			datagram3.data = datagram3_data;
			
			ec_datagram_aprd(&datagram0,master->slaves[0].ring_position, 0x910, 8);
			ec_datagram_aprd(&datagram1,master->slaves[1].ring_position, 0x910, 8);
			ec_datagram_aprd(&datagram2,master->slaves[2].ring_position, 0x910, 8);
			ec_datagram_aprd(&datagram3,master->slaves[3].ring_position, 0x910, 8);
			ec_master_queue_datagram(master, &datagram0);
			ec_master_queue_datagram(master, &datagram1);
			ec_master_queue_datagram(master, &datagram2);
			ec_master_queue_datagram(master, &datagram3);
			ecrt_master_send(master);
			 do
			{
				 ecrt_master_receive(master);
			 }while((datagram1.state == EC_DATAGRAM_SENT) && ((u32)(jiffies - datagram1.jiffies_sent) < ESC_TIMEOUT));
			 sys_time[0] = EC_READ_U64(datagram0.data);
			 sys_time[1] = EC_READ_U64(datagram1.data);
			 sys_time[2] = EC_READ_U64(datagram2.data);
			 sys_time[3] = EC_READ_U64(datagram3.data);
			 uart_printf("1070=%llx, %llx, %llx, %llx\n", sys_time[0], sys_time[1], sys_time[2], sys_time[3]);

		 }
#endif	
	sys_time_ref = EC_READ_U64(datagram0.data);
	jiffies_ns = sys_time_ref;
	do_div(&jiffies_ns, (S_IN_NS/HZ));
#if 0	//gonggw, 2015.04.28, synchronize system time with system time in reference slave
	jiffies_64 = jiffies_ns;
#else
	jiffies_64 = jiffies_ns + rxpdo_sync0_ph_offset;
#endif
	 jiffies = (u32)jiffies_64;
    
	//if(master->has_app_time == 1)
	{
		master->has_app_time = 0;
		jiffies_ns = (u64)(jiffies_ns * ((u64)(S_IN_NS/HZ)));
		ecrt_master_application_time(master, jiffies_ns);
		//master->app_start_offset +=  (u64)((RXPDO_CYCLE_US * 1000) - (jiffies_ns % (RXPDO_CYCLE_US * 1000)));
	}
	return 0;
}


/**
ecat_dc_update_static: Clock synchronization between slaves
@master: master pointer
@return value, 0, success; -1 fail
**/

int ecat_dc_update_static(ec_master_t *master)
{
	 int i;
	u64 sys_time_ref, sys_time_slv;
	u64 time_diff;
	u64 old_offset, new_offset;
	ec_datagram_t datagram0, datagram1;
	u8  datagram0_data[32], datagram1_data[32];
	 _DBG_PRINTF_("Slaves Static Sync...\n");
	ec_datagram_init(&datagram0);
	datagram0.data_origin = EC_ORIG_EXTERNAL;
	datagram0.data = datagram0_data;     

	ec_datagram_init(&datagram1);
	datagram1.data_origin = EC_ORIG_EXTERNAL;
	datagram1.data = datagram1_data;

	 i = 0;
	if (master->slave_count > 1)
	 {
		 while(i < master->slave_count)
		{
			 if (((&(master->slaves[i])) == master->dc_ref_clock) || (!master->slaves[i].base_dc_supported))
			 {
				 i++;
				 continue;
			 }
			 while(1)
			{
#ifdef USE_RING_POSITION
				ec_datagram_aprd(&datagram0,master->dc_ref_clock->ring_position, 0x910, 0x8);
#else
				ec_datagram_fprd(&datagram0,master->dc_ref_clock->station_address, 0x910, 0x8);
#endif
				ec_datagram_zero(&datagram0);
				ec_master_queue_datagram(master, &datagram0);
    
#ifdef USE_RING_POSITION
				ec_datagram_aprd(&datagram1,master->slaves[i].ring_position, 0x910, 0x20);
#else
				ec_datagram_fprd(&datagram1,master->slaves[i].station_address, 0x910, 0x20);
#endif
				ec_datagram_zero(&datagram1);
 				ec_master_queue_datagram(master, &datagram1);

				ecrt_master_send(master);
				do
				{
					ecrt_master_receive(master);
				}while(((datagram0.state == EC_DATAGRAM_SENT) && ((u32)(jiffies - datagram0.jiffies_sent) < ESC_TIMEOUT)) || \
 						 ((datagram1.state == EC_DATAGRAM_SENT) && ((u32)(jiffies - datagram1.jiffies_sent) < ESC_TIMEOUT)));

				 if ((datagram0.state == EC_DATAGRAM_RECEIVED) && (datagram0.working_counter == 1) &&\
					(datagram1.state == EC_DATAGRAM_RECEIVED) && (datagram1.working_counter == 1))
				 {
					 break;
				 }
				 else
				 {
					 if (datagram0.state != EC_DATAGRAM_RECEIVED)
						 list_del_init(&datagram0.queue);
                    
					  if (datagram1.state != EC_DATAGRAM_RECEIVED)
						 list_del_init(&datagram1.queue);
                    
					 return -1;
				}    
			 }
			datagram0.state = EC_DATAGRAM_INIT;
			datagram1.state = EC_DATAGRAM_INIT;
        
			sys_time_ref = EC_READ_U64(datagram0.data);
			sys_time_slv = EC_READ_U64(datagram1.data);
			old_offset   = EC_READ_U64(datagram1.data + 0x10);
        
			  if(sys_time_slv < sys_time_ref)
			{
				 time_diff = sys_time_ref - sys_time_slv;
				 new_offset=old_offset + time_diff;
			 }
			else
			  {
				time_diff = sys_time_slv - sys_time_ref;
				 new_offset=old_offset - time_diff;
			}

			 while(1)
			 {
#ifdef USE_RING_POSITION
				ec_datagram_apwr(&datagram1,master->slaves[i].ring_position, 0x920, 12);
#else
				ec_datagram_fpwr(&datagram1,master->slaves[i].station_address, 0x920, 12);
#endif
				EC_WRITE_U64(datagram1.data, new_offset);
				EC_WRITE_U32(datagram1.data + 8, master->slaves[i].transmission_delay);
				ec_master_queue_datagram(master, &datagram1);
				ecrt_master_send(master);
				 do
				{
					 ecrt_master_receive(master);
				 }while((datagram1.state == EC_DATAGRAM_SENT) && ((u32)(jiffies - datagram1.jiffies_sent) < ESC_TIMEOUT));
                
				 if ((datagram1.state == EC_DATAGRAM_RECEIVED) && (datagram1.working_counter == 1))
				{
					break;
				 }
				 else
				{
					list_del_init(&datagram1.queue);
					return -1;
				}
			}
			datagram1.state = EC_DATAGRAM_INIT;
			 i++;
		}
#if 0
		{
			ec_datagram_t datagram2, datagram3;
			u8	datagram2_data[32], datagram3_data[32];
			u64 sys_time[4];
			ec_datagram_init(&datagram2);
			datagram2.data_origin = EC_ORIG_EXTERNAL;
			datagram2.data = datagram2_data;	 
			
			ec_datagram_init(&datagram3);
			datagram3.data_origin = EC_ORIG_EXTERNAL;
			datagram3.data = datagram3_data;
			
			ec_datagram_aprd(&datagram0,master->slaves[0].ring_position, 0x910, 8);
			ec_datagram_aprd(&datagram1,master->slaves[1].ring_position, 0x910, 8);
			ec_datagram_aprd(&datagram2,master->slaves[2].ring_position, 0x910, 8);
			ec_datagram_aprd(&datagram3,master->slaves[3].ring_position, 0x910, 8);
			ec_master_queue_datagram(master, &datagram0);
			ec_master_queue_datagram(master, &datagram1);
			ec_master_queue_datagram(master, &datagram2);
			ec_master_queue_datagram(master, &datagram3);
			ecrt_master_send(master);
			 do
			{
				 ecrt_master_receive(master);
			 }while((datagram1.state == EC_DATAGRAM_SENT) && ((u32)(jiffies - datagram1.jiffies_sent) < ESC_TIMEOUT));
			 sys_time[0] = EC_READ_U64(datagram0.data);
			 sys_time[1] = EC_READ_U64(datagram1.data);
			 sys_time[2] = EC_READ_U64(datagram2.data);
			 sys_time[3] = EC_READ_U64(datagram3.data);
			uart_printf("1246=%llx, %llx, %llx, %llx\n", sys_time[0], sys_time[1], sys_time[2], sys_time[3]);
		 }
#endif
		//static drift compensation
		 _DBG_PRINTF_("Slaves Drift Sync...\n");
		 for(i = 0; i < S_DRIFT_COMPEN_TIME; i++)
		 {
		 	 FeedDog();
			 while(1)
			 {
#ifdef USE_RING_POSITION
				ec_datagram_armw(&datagram1,master->dc_ref_clock->ring_position, 0x910, 0x8);
#else
				ec_datagram_frmw(&datagram1,master->dc_ref_clock->station_address, 0x910, 0x8);
#endif
				ec_datagram_zero(&datagram1);
				ec_master_queue_datagram(master, &datagram1);
				ecrt_master_send(master);
				 do
				 {
					 ecrt_master_receive(master);
				 }while((datagram1.state == EC_DATAGRAM_SENT) && ((u32)(jiffies - datagram1.jiffies_sent) < ESC_TIMEOUT));
				if (datagram1.state == EC_DATAGRAM_RECEIVED)
				{
					 break;
				}
				else
				{
					list_del_init(&datagram1.queue);
					return -1;
				 }
			}
		}
	}
	return 0;
}
void ecat_reconfig_wait_slaves()
{
	ec_datagram_t datagram0;
	u8	datagram0_data[32];
	ec_datagram_init(&datagram0);
	datagram0.data_origin = EC_ORIG_EXTERNAL;
	datagram0.data = datagram0_data;	 
//	volatile int ss=0;
	while (1){
  //      for(ss=0;ss<100000;ss++){;}
        ec_datagram_brd(&datagram0, 0x0130, 2);
		ec_master_queue_datagram(cur_master, &datagram0);
		ecrt_master_send(cur_master);
	 	do
		{
			 ecrt_master_receive(cur_master);
	 	}while((datagram0.state == EC_DATAGRAM_SENT) && ((u32)(jiffies - datagram0.jiffies_sent) < ESC_TIMEOUT));
		if (datagram0.working_counter == cur_master->slave_count)
			break;
		while(jiffies % 5);
	}
}
int wait_slaves_ready()
{
	ec_datagram_t datagram0;
	uint16_t reg_wkc = 0xFFFF;
	int	state_counter = 0;
	u8	datagram0_data[32];
	ec_datagram_init(&datagram0);
	datagram0.data_origin = EC_ORIG_EXTERNAL;
	datagram0.data = datagram0_data;	 
//	volatile unsigned int rr=0;
	while (1){
 //       for(rr=0;rr<100000;rr++){;}
		ec_datagram_brd(&datagram0, 0x0130, 2);
		ec_master_queue_datagram(cur_master, &datagram0);
		ecrt_master_send(cur_master);
        
 //       rr=jiffies;
	 	do
		{
            /*
                                //tst chenhao
                    int int_status;
                    int_status = kENET_TxBufferInterrupt | kENET_TxFrameInterrupt;
                    if( int_status & ECAT_ENET->EIR)
                    {
                        printf("s++\n\r");       
              //          ECAT_ENET->EIR = int_status;
                    }
                    //tst chenhao
                    int_status = kENET_RxFrameInterrupt | kENET_RxBufferInterrupt;
                    if( int_status & ECAT_ENET->EIR)
                    {
                        printf("r++\n\r");       
                //        ECAT_ENET->EIR = int_status;
                    }    
                    */
			 ecrt_master_receive(cur_master);
              
	 	}while((datagram0.state == EC_DATAGRAM_SENT) && ((u32)(jiffies - datagram0.jiffies_sent) < ESC_TIMEOUT));
  //      int cnt=0;
 //       printf("\r\n cnt=%d \r\n",(unsigned int)(jiffies-rr));
#if 1
		if (datagram0.working_counter == reg_wkc )
		{
			state_counter++;
		}
		else
		{
			reg_wkc = datagram0.working_counter;
			state_counter = 0;   //chenhao 2020
		}
		if ((reg_wkc > 0) && (state_counter > 20000)) 
		//if ((reg_wkc > 0) && (state_counter > 2))  //chenhao 2020
		{
			//uart_printf("%d.", reg_wkc);
			return 1;
		}
		else if ((ChkSysCfg(XF6_EFFC)) && (reg_wkc == 0) && (state_counter > 0))
		{
			return 0;
		}
#else
		if (datagram0.working_counter == 4)
		{
			uart_printf("%d.", datagram0.working_counter);
			break;
		}
#endif
		FeedDog();
		while(jiffies % 5);
	}
}

void  app_start_sync(ec_master_t *master)
{
	u64 sys_time_ref, app_start_offset;
	u16 sync_u;
	ec_datagram_t datagram0;
	u8  datagram0_data[32];
	ec_datagram_init(&datagram0);
	datagram0.data_origin = EC_ORIG_EXTERNAL;
	datagram0.data = datagram0_data; 
	sync_u = 0x300;
	while(1)
    {
		ec_datagram_bwr(&datagram0, 0x9A0, 0x8);
		EC_WRITE_U32((uint8_t *) (datagram0.data),500000);
		EC_WRITE_U32((uint8_t *) (datagram0.data+4), 0);
		ec_master_queue_datagram(master, &datagram0);
		 ecrt_master_send(master);
		do
		{
			ecrt_master_receive(master);
		}while((datagram0.state == EC_DATAGRAM_SENT) && ((u32)(jiffies - datagram0.jiffies_sent) < ESC_TIMEOUT));

		if ((datagram0.state == EC_DATAGRAM_RECEIVED) && (datagram0.working_counter == 1))
		{
			break;
		}
		else
		{
			list_del_init(&datagram0.queue);
			return ;
		}
    }

    while(1)
    {
#ifdef USE_RING_POSITION
		ec_datagram_aprd(&datagram0,master->dc_ref_clock->ring_position, 0x910, 0x8);
#else
		ec_datagram_fprd(&datagram0,master->dc_ref_clock->station_address, 0x910, 0x8);
#endif
		ec_datagram_zero(&datagram0);
		 ec_master_queue_datagram(master, &datagram0);

		 ecrt_master_send(master);
		 do
		 {
			ecrt_master_receive(master);
		}while((datagram0.state == EC_DATAGRAM_SENT) && ((u32)(jiffies - datagram0.jiffies_sent) < ESC_TIMEOUT));

		if ((datagram0.state == EC_DATAGRAM_RECEIVED) && (datagram0.working_counter == 1))
		{
			break;
		}
		 else
		{
			if (datagram0.state != EC_DATAGRAM_RECEIVED)
				 list_del_init(&datagram0.queue);                    
			 return ;
		 }    
    }
	datagram0.state = EC_DATAGRAM_INIT;
          
	sys_time_ref = EC_READ_U64(datagram0.data);
	app_start_offset = sys_time_ref + (u64)2000000000;
	 while(1)
            {
		ec_datagram_bwr(&datagram0, 0x990, 0x8);
		EC_WRITE_U64(datagram0.data, app_start_offset);
		ec_master_queue_datagram(master, &datagram0);
		ecrt_master_send(master);
 		 do
		{
			 ecrt_master_receive(master);
		 }while((datagram0.state == EC_DATAGRAM_SENT) && ((u32)(jiffies - datagram0.jiffies_sent) < ESC_TIMEOUT));
                
		if ((datagram0.state == EC_DATAGRAM_RECEIVED) && (datagram0.working_counter == 1))
		{
			break;
		}
		else
		 {
			list_del_init(&datagram0.queue);
			return ;
		 }
            }

	 sync_u = 0x300;
	 while(1)
            {
		ec_datagram_bwr(&datagram0, 0x980, 0x2);
		EC_WRITE_U16(datagram0.data, sync_u);
		ec_master_queue_datagram(master, &datagram0);
		ecrt_master_send(master);
 		 do
		{
			 ecrt_master_receive(master);
		 }while((datagram0.state == EC_DATAGRAM_SENT) && ((u32)(jiffies - datagram0.jiffies_sent) < ESC_TIMEOUT));
                
		if ((datagram0.state == EC_DATAGRAM_RECEIVED) && (datagram0.working_counter == 1))
		{
			break;
		}
		else
		 {
			list_del_init(&datagram0.queue);
			return;
		 }
            }
}



/**
drift_compen_dyn: dynamic drift compensation, called periodically 
@master: master pointer
**/
void drift_compen_dyn()
{
    ec_master_t *master = cur_master;
    if (master_dyn_sync_start == 0)
        return;
    
    if (master->slave_count < 2)
        return;
    if ((slv_drift_dgram.state != EC_DATAGRAM_RECEIVED) && (slv_drift_dgram.state != EC_DATAGRAM_INIT))
    {
        slv_drift_dgram.state = EC_DATAGRAM_INIT;
        list_del_init(&slv_drift_dgram.queue);
    }

    ec_datagram_frmw(&slv_drift_dgram,master->dc_ref_clock->station_address, 0x910, 0x8);
    ec_datagram_zero(&slv_drift_dgram);
    ec_master_queue_datagram(master, &slv_drift_dgram);
}
/*
ecat_net_op: EtherCAT datagram receive and send
*/
u32 rec_test_start = 0;
u32 txpdo0_state;
u32 txpdo0_wkc;
void ecat_receive()
{
	ec_master_t *master = cur_master;
   dyn_dgram_ok = 0;
#ifdef EC_RATE_OPT
    rec_test_start = 1;
#endif
    ecrt_master_receive(master);
#ifdef EC_RATE_OPT
    dyn_dgram_ok = 1;
#endif
#if 1
	txpdo0_state = (u32)txpdo_slv_dgram[0].state;
	txpdo0_wkc = (u32)txpdo_slv_dgram[0].working_counter;
#endif
}
void ecat_send()
{
	ec_master_t *master = cur_master;
	{
		dyn_dgram_ok = 0;
		ecrt_master_send(master);
		dyn_dgram_ok = 1;
	}
}
//ecat_queue is called main loop(not in Timer ISR)
#ifdef ECAT_QUEUE_TIME_TEST
extern	int test_idx;
extern	int test_start;
extern	u32 test_rec_time[];
#endif

u32							ec_queue_step=0;
void ecat_queue()
{
#ifdef ECAT_QUEUE_TIME_TEST
	if((test_idx < 128) && (test_start == 1))
		test_rec_time[1] = read_timer(DEBUG_TIMER);
#endif
	rxpdo_cycle();
#ifdef ECAT_QUEUE_TIME_TEST
	if((test_idx < 128) && (test_start == 1))
	{
		test_rec_time[1] = read_timer(DEBUG_TIMER) - test_rec_time[1];
		test_rec_time[2] = read_timer(DEBUG_TIMER);
	}
#endif
	txpdo_cycle();
#ifdef ECAT_QUEUE_TIME_TEST
	if((test_idx < 128) && (test_start == 1))
	{
		test_rec_time[2] = read_timer(DEBUG_TIMER) - test_rec_time[2];
		test_rec_time[3] = read_timer(DEBUG_TIMER);
	}
#endif
#ifdef EC_RATE_OPT
	if (state_chk == 1)
	{
		esc_state_check_quene();
		state_chk = 2;
	}
#endif
#ifdef ECAT_QUEUE_TIME_TEST
	if((test_idx < 128) && (test_start == 1))
	{
		test_rec_time[3] = read_timer(DEBUG_TIMER) - test_rec_time[3];
		test_rec_time[4] = read_timer(DEBUG_TIMER);
	}
#endif

#ifdef EC_RATE_OPT
	if (dyn_sync == 1)
	{
		master_sync_dyn(0);
		drift_compen_dyn();
		//cyclic_unit_dyn();
		dyn_sync = 2;		
	}
#else
	
	if (dyn_sync == 1)
	{
		if(ec_queue_step==0)
		{
			master_sync_dyn(0);			
		}
		else if(ec_queue_step==1)
		{
			drift_compen_dyn();
		}
		else if(ec_queue_step==2)
		{
			esc_state_check_quene();
		}
		ec_queue_step++;
		if(ec_queue_step>=3)
		{
			ec_queue_step = 0;
		}
		
		dyn_sync = 2;		
	}
	
#endif
#ifdef ECAT_QUEUE_TIME_TEST
	if((test_idx < 128) && (test_start == 1))
	{
		test_rec_time[4] = read_timer(DEBUG_TIMER) - test_rec_time[4];
	}
#endif
}
void ecat_state_check()
{
    master_sync_dyn_check();
    esc_state_check_rcv();
	//cyclic_unit_dyn_check();	
}
int ecat_start_xmit()
{	
	if (dyn_sync == 2)
	{
		dyn_sent_jiff64 = jiffies_64;
		dyn_sync = 0;
	}
	EMAC_dma_tx_start();
	ecat_sr_flag = 0;
	return 0;
}
	
void ecat_net_op()
{
	ecat_receive();
	ecat_state_check();
	ecat_queue();
	ecat_send();
}
/** 
*set_esc_state, set the state of ESC with station address to target_target
@target_state, Target state
@station_address, station address of target ESC
@return value:  0, success
                        -0xF0, EC_SLAVE_STATE_BOOT, invalid
                        -0xF1, no target slave in current master
                        -0xF2, ESC access error or timeout
                        -0xF3, time out
                        others, negative value of AL Status Code of ESC FSM
**/
void set_esc_state_quene(int dir)
{
    ec_master_t  *master = cur_master;
    if ((set_state_dgram.state != EC_DATAGRAM_RECEIVED) && (set_state_dgram.state != EC_DATAGRAM_INIT))
    {        
        set_state_dgram.state = EC_DATAGRAM_INIT;
        list_del_init(&(set_state_dgram.queue));
    }

    if (dir == 0) //read
    {
        ec_datagram_fprd(&(set_state_dgram),esc_state.cur_station_address, 0x130, 2);
        ec_datagram_zero(&(set_state_dgram));
    }
    else 
    {
        ec_datagram_fpwr(&(set_state_dgram),esc_state.cur_station_address, 0x120, 2);
    }
    ec_master_queue_datagram(master, &(set_state_dgram));
}
void  inline  ecat_master_set(u32 val)
{
	cur_master->ecat_stage = val;
}
uint32_t over_time_flag;
void ecat_cycle_isr(int irq)
{
	if (irq)
	{
		if (cur_master->ecat_stage == 0)			   	//slaves 4 to 8
		{
				if (!(jiffies % (RXPDO_CYCLE-8)))	//4->8 2019-07-31
				{
					cycle_send = 1;
				}					
		}
		else
		{
				if (!(jiffies % RXPDO_CYCLE))
				{
					cycle_send = 1;
				}			
		}
		/*
		
		if (!(jiffies % RXPDO_CYCLE))
		{
			cycle_send = 1;
		}
		*/
		if (!(jiffies % 100))
		{
			if(flag_adjsync==0)
			{
				state_chk = 1;
				dyn_sync  = 1;
			}
			else
				flag_adjsync = 0;

		}
		if (ec_app_data.ecat_net_state == ECAT_NET_OK)
		{
			if (cur_master->ecat_stage==0)
			{
				if (jiffies % (CONFIG_SYS_HZ/10))
				{
	//				run_led_flash();
				}
			}
		}
	}
	else
	{
		cycle_send = 1;
		if ((jiffies % 100) < RXPDO_CYCLE)
		{
			if(flag_adjsync==0)
			{
				state_chk = 1;
				dyn_sync  = 1;
			}
			else
				flag_adjsync = 0;

		}
	}
	if (ec_app_data.ecat_net_state == ECAT_NET_OK)
	{
		if (cur_master->ecat_stage == 1)
		{
			if (cycle_send)
			{
				if(ecat_sr_flag == 1)
		 		{
		 			if (!ecat_start_xmit())
		 			{
		 				cycle_send = 0;
		 			}
					if ((jiffies - over_time_flag) > (RXPDO_CYCLE + 3))
						(ec_app_data_p->num_overtime_pdo)++;

					over_time_flag = jiffies;
#ifdef	PDO_LOSS_RATE
					no_sent = 0;
#endif
		 		}
#ifdef 	PDO_LOSS_RATE
				else
				{
					no_sent = 1;
				}
#endif
			}
		}
		else
		{
			if (cycle_send)
			{
				rxpdo_cycle();
				cycle_send = 0;
			}
		}
	}
}



#if 0
void ecat_cycle_isr(int irq)
{
	if (irq)
	{
		if (!(jiffies % RXPDO_CYCLE))
		{
			cycle_send = 1;
		}
		if (!(jiffies % 100))
		{
			state_chk = 1;
			dyn_sync  = 1;
		}
		if (ec_app_data.ecat_net_state == ECAT_NET_OK)
		{
			if (cur_master->ecat_stage==0)
			{
				if (jiffies % (CONFIG_SYS_HZ/10))
				{
//					run_led_flash();
				}
			}
		}
	}
	else
	{
		cycle_send = 1;
		if ((jiffies % 100) < RXPDO_CYCLE)
		{
			state_chk = 1;
			dyn_sync  = 1;
		}
	}
	if (ec_app_data.ecat_net_state == ECAT_NET_OK)
	{
		if (cur_master->ecat_stage == 1)
		{
			if (cycle_send)
			{
				if(ecat_sr_flag == 1)
		 		{
		 			if (!ecat_start_xmit())
		 			{
		 				cycle_send = 0;
		 			}
					if ((jiffies - over_time_flag) > (RXPDO_CYCLE + 3))
						(ec_app_data_p->num_overtime_pdo)++;

					over_time_flag = jiffies;
#ifdef	PDO_LOSS_RATE
					no_sent = 0;
#endif
		 		}
#ifdef 	PDO_LOSS_RATE
				else
				{
					no_sent = 1;
				}
#endif
			}
		}
		else
		{
			if (cycle_send)
			{
				rxpdo_cycle();
				cycle_send = 0;
			}
		}
	}
}
#endif
extern u64 timestamp;

void ec_timer_init()
{
	jiffies = 0;
	jiffies_64 = (u64)0;
	cur_master =NULL;
	timestamp = (u64)0;
	sys_time_offset = 0x55AA55AA;
}
u64 jiffies_64_ch;
void ec_timer_isr(void)
{
    jiffies_64++;
    jiffies_64_ch++;
    jiffies             = (u32)jiffies_64;
	ecat_cycle_isr(1);
	
}
/************************  EtherCAT  Interface  ******************/

/*
EcatDataCreate: Create Data(in Bufer) to transmit through EtherCAT
data: data buffer pointer
data_len: active data length(Byte)
op_mode: EtherCAT operationMode(Read/Write/ReadWrite, Broadcast)
station_address: EtherCAT slave station address
reg_address: register address in EtherCAT slave
*/
inline void EcatDataCreate(u8 * data, EcatReq_t *req)
{
    u16 * data_u16;
    data_u16 = (u16 *)data;
    data_u16[EC_OFF_CONTROL>>1] = req->ConWord;
    data_u16[EC_OFF_TYPE>>1]    = req->TransferType;
    data_u16[EC_OFF_SLV_ADDR>>1] = req->EscAddress;
    data_u16[EC_OFF_INDEX>>1] = req->OdIndex;
    data_u16[EC_OFF_SUBINDEX>>1] = req->OdSubIndex;
    data_u16[EC_OFF_LENGTH>>1] = req->DataLen + EC_OFF_BUF_HDR;
    //set (data + data_len + 8), length of next datagram, to 0, to indicate the end of active datagrams
    data_u16 = (u16 *)(data + ((data_u16[EC_OFF_LENGTH>>1] + 1) & ~0x1));
    *data_u16 = 0;
    
}
/*
EcatReq: Ecat Operation Request
@data: data buffer
@pkg_cnt: number of valid package
*/
void EcatReq(u16 *data, int pkg_cnt)
{
	u16 *data16 = (u16 *)data;
	int i;
	for(i = 0; i < pkg_cnt; i++)
	{
		data16[1]	|= 0x1; //Send Request to ECAT Master
		data16		= (u16 *)(data16 + ((*data16 + 1)>>1));
	}
	*data16 = 0; //no more package flag
}


/** 
*ecat_master_init, EtherCAT Master and EMAC initilization
**/

int ecat_master_init()
{
	int net_init_cnt = 0;
    UI16 connect_count = 0;
	ec_mm_init();
	if (ec_init_module())
    {
        _DBG_PRINTF_("EC_MASTER initialization failure\n");
        return 0;
    }
	FeedDog();
    EMAC_drv_init();
	_DBG_PRINTF_("\n\r before task_top_init :  \n\r");
	FeedDog();
    ecat_task_top_init();
	_DBG_PRINTF_("\n\r after task_top_init :  \n\r");
	FeedDog();
    cycle_op_init();
	_DBG_PRINTF_("\n\rEMAC Init time:  \n\r");
    if(ChkSysCfg(CO3_EJTNEW))
        connect_count = 4;
    else
        connect_count = 2;
    while(net_init_cnt < connect_count)//四次尝试Ecat连接
    {
        dyn_dgram_ok = 0;
        if (ec_dev_init())
        {
        	SetNetErrCnt(net_init_cnt);
			if (net_init_cnt < connect_count - 1)
			{
			//	serial_putc((uint32_t*)DEBUG_UART, 8);
				_DBG_PRINTF_("%d", net_init_cnt);
			}
			else
			{
				_DBG_PRINTF_("\n ECAT Init Failed \n");//, 8, net_init_cnt);
				ec_app_data_p->ecat_net_state = ECAT_NET_EMAC_ERR;
				ClrNetErr();
				return 0;
			}
			net_init_cnt++;
        	continue;
		}
		else
		{
			ec_app_data_p->ecat_net_state = ECAT_NET_OK;			
			ecat_task_req();
			ClrNetErr();
			return 1;
		}
       // dyn_dgram_ok = 1;
	}
	//}while(ecat_task_req());
	//app_start_sync(cur_master);
	ClrNetErr();
	return 1;
}

inline void slave_bitmap_set(uint16_t idx)
{
	ec_app_data_p->slave_link_bitmap[idx>>4] |= (1 << (idx & 0xF));
}

inline void slave_bitmap_clr(uint16_t idx)
{
	ec_app_data_p->slave_link_bitmap[idx>>4] &= ~(1 << (idx & 0xF));
}
uint16_t ecat_missed_slave_num()
{
	int i, j;
	uint16_t bitmap = 1;
	for(i = 0; i < SLAVE_BITMAP_NUM; i++)
	{
		for(j = 0; j < 16; j++)
		{
			if (ec_app_data_p->slave_link_bitmap[i] & bitmap)
				break;
			bitmap = bitmap << 1;
		}
		if (j < 16)
			break;
	}
	if ((i == SLAVE_BITMAP_NUM) && (j == 16))
		return 0xFFFF;
	else
		return ((i << 4) + j);
}
void ec_fsm_master_state_dc_write_offset(ec_fsm_master_t *fsm);

void ecat_wrtie_system_time()
{
	ec_master_t *master = cur_master;
	ec_fsm_master_t *fsm  = &(master->fsm);
	fsm->slave = master->slaves; // begin with first slave
	master->ecat_stage = 0;
	ec_fsm_master_enter_write_system_times(fsm);
	ec_master_mbox_queue_datagrams(master, &master->fsm_mbox);
	ecrt_master_send(master);
	while(1)
	{
		ecrt_master_receive(master);
		if (fsm->slave == (&(master->slaves[master->slave_count-1])))
		{
			if ((fsm->datagram->state == EC_DATAGRAM_RECEIVED) && (fsm->state == ec_fsm_master_state_dc_write_offset))
				break;
		}
		if (ec_fsm_master_exec(&master->fsm))
		{
		ec_master_mbox_queue_datagrams(master, &master->fsm_mbox);
	}
	ecrt_master_send(master);
		while(jiffies % 5);
	}
	master->ecat_stage = 1;
	
}
void ecat_slave_reconfig_start()
{
	ec_slave_t *slave;
	ec_datagram_t *datagram;
	//cur_master->debug_level = 3;
	ec_app_data_p->reconfig_slave_num = ecat_missed_slave_num();
	pdo_slv_ok[ec_app_data_p->reconfig_slave_num] = 0;
	if (ec_app_data_p->reconfig_slave_num == 0xFFFF)
		return;
	slave = &(cur_master->slaves[ec_app_data_p->reconfig_slave_num]);
	slave->current_state = EC_SLAVE_STATE_INIT;
	datagram = cur_master->fsm.fsm_slave_scan.datagram;
    // write station address
	ec_datagram_apwr(datagram, slave->ring_position, 0x0010, 2);
	EC_WRITE_U16(datagram->data, slave->station_address);
	cur_master->fsm.fsm_slave_scan.retries = EC_FSM_RETRIES;
	ec_master_queue_datagram(cur_master, datagram);
	slave_reconfig = RECONFIG_STATE_ADDRESS;
}

void ecat_slave_reconfig_state_address()
{
	ec_master_t *master = cur_master;
	ec_slave_t *slave = &(master->slaves[ec_app_data_p->reconfig_slave_num]);
	ec_datagram_t *datagram = master->fsm.fsm_slave_scan.datagram;

	if (datagram->state != EC_DATAGRAM_RECEIVED)
	{
		if ((datagram->state == EC_DATAGRAM_TIMED_OUT) && 
			(master->fsm.fsm_slave_scan.retries--))
		{
			datagram = master->fsm.fsm_slave_scan.datagram;
			ec_datagram_apwr(datagram, slave->ring_position, 0x0010, 2);
			EC_WRITE_U16(datagram->data, slave->station_address);
			ec_master_queue_datagram(master, datagram);
			
		}
		else if (master->fsm.fsm_slave_scan.retries == 0)
		{
			EC_SLAVE_ERR(slave,
					"Failed to receive station address datagram: ");
			ec_datagram_print_state(datagram);
			slave_reconfig = RECONFIG_IDLE;
		}
		return;
	}


	if (datagram->working_counter != 1) {
		slave->error_flag = 1;
		EC_SLAVE_ERR(slave, "Failed to write station address: ");
		ec_datagram_print_wc_error(datagram);
		slave_reconfig = RECONFIG_IDLE;
		return;
	}
#if 1
	master->fsm.fsm_slave_config.slave = slave;
	ec_fsm_slave_config_state_start(&(master->fsm.fsm_slave_config));
	ec_master_queue_datagram(master, datagram);
	slave_reconfig = RECONFIG_STATE_CONFIG;
#else
	master->fsm.fsm_slave_config.slave = slave;
	slave_reconfig = RECONFIG_STATE_STATIC_SYNC;
#endif
}
void ecat_slave_reconfig_state_reconfig()
{
	ec_master_t *master = cur_master;
	ec_slave_t *slave = &(master->slaves[ec_app_data_p->reconfig_slave_num]);
	ec_fsm_slave_config_t *fsm = &(master->fsm.fsm_slave_config);
	if ((master->fsm_datagram.state == EC_DATAGRAM_QUEUED) || (master->fsm_datagram.state == EC_DATAGRAM_SENT))
		return;
	if (!ec_fsm_slave_config_exec(fsm))
	{
		if (ec_fsm_slave_config_success(fsm))
		{
			slave_bitmap_clr(ec_app_data_p->reconfig_slave_num);
			slave_link_check[ec_app_data_p->reconfig_slave_num] = 0;
			pdo_slv_ok[ec_app_data_p->reconfig_slave_num]=1;
#if 1
			ec_app_data_p->reconfig_slave_num = ecat_missed_slave_num();
			if (ec_app_data_p->reconfig_slave_num == 0xFFFF)
			{
				slave_reconfig	= RECONFIG_STATE_STATIC_SYNC;
				slave_miss		= 0;
			}
			else
			{
				ecat_slave_reconfig_start();
			}
#else
			slave_reconfig	= RECONFIG_IDLE;
			slave_miss		= 0;
#endif
		}
	}
	else
	{
		if (slave->current_state > EC_SLAVE_STATE_PREOP)
			pdo_slv_ok[ec_app_data_p->reconfig_slave_num] = 1;
		ec_master_queue_datagram(master, fsm->datagram);
	}

}
#if 0
void ecat_slave_reconfig()
{
	if (slave_reconfig == RECONFIG_IDLE)
		return;
	else if (slave_reconfig == RECONFIG_START)
	{
		cur_master->ecat_stage = 0;
		//ecat_wrtie_system_time();
		master_sync(cur_master);
		cur_master->ecat_stage = 1;
		ecat_slave_reconfig_start();
		cur_master->app_start_offset += (uint64_t)(5 * S_IN_NS);
	}
	else if (slave_reconfig == RECONFIG_STATE_ADDRESS)
		ecat_slave_reconfig_state_address();
	else if (slave_reconfig == RECONFIG_STATE_CONFIG)
	{
		ecat_slave_reconfig_state_reconfig();
	}
	else if (slave_reconfig == RECONFIG_STATE_STATIC_SYNC)
	{
		slave_reconfig	= RECONFIG_IDLE;
	}
}
#else
void ecat_slave_reconfig()
{
	uint64_t reserved_start_offset;
	if (slave_reconfig == RECONFIG_IDLE)
		return;
	else if (slave_reconfig == RECONFIG_START)
	{
		//SetNetErrReconfig();	/*通知面板:主机进入重新配置Ecat状态*/
		reserved_start_offset = cur_master->app_start_offset;
		//cur_master->app_start_offset += (uint64_t)(0 * S_IN_NS);
		cur_master->app_start_offset = (uint64_t)(8* S_IN_NS);
		cur_master->has_app_time = 0;
		ecat_master_set(0);
		ecat_reconfig_wait_slaves();
		master_sync(cur_master);
		ecat_slave_reconfig_start();
		while(1)
		{
			ecrt_master_send(cur_master);
			while(slave_reconfig == RECONFIG_STATE_ADDRESS)
			{
				ecrt_master_receive(cur_master);
				ecat_slave_reconfig_state_address();
				ecrt_master_send(cur_master);
			}
			while(slave_reconfig == RECONFIG_STATE_CONFIG)
			{
				while(jiffies % 5);
				ecrt_master_receive(cur_master);
				if (slave_reconfig == RECONFIG_STATE_STATIC_SYNC)
					break;
				ecat_slave_reconfig_state_reconfig();
				ecrt_master_send(cur_master);
			}
			if (slave_reconfig == RECONFIG_STATE_STATIC_SYNC)
			{
				cur_master->app_start_offset = reserved_start_offset;
				slave_reconfig	= RECONFIG_IDLE;
				break;
			}
		}
		//SetEcatInitStep(0);	/*Ecat网络初始化结束，面板消除显示*/
		while(get_jiffies() % pdo_cycle_period());
		ecat_master_set(1);
	}
}
#endif
void ecat_clear_sii_pdos()
{
	int i, j;
	ec_master_t *master = cur_master;
	ec_slave_t *slave;
	for(i = 0; i < master->slave_count; i++)
	{
		slave = &(master->slaves[i]);
		for(j = 2; j < slave->sii.sync_count; j++)
		{
			ec_sync_clear(&(slave->sii.syncs[j]));
		}
		
	}

}

uint16_t fmmu_phy_start_addr(ec_slave_t *slave, uint16_t sync_addr)
{
	ec_master_t *master = cur_master;
	uint16_t offset = 0;
	int i;
	for(i =0; i < master->slave_count; i++)
	{
		if (slave != (&(master->slaves[i])))
			offset += hc_rxpdo_size[i];
	}
	return (uint16_t)(sync_addr - offset);
}
