/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    app_config.c

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:            2013.11.04

Description:    EtherCAT Task configuration for Application

Others:

Function List:
                    ecat_app_slave_init:       EtherCAT slave configuration info initialization
                    ecat_task_top_init:         EtherCAT task variables initilization
                    ec_task_datagram_init:  EtherCAT datagrams variables initialization
                    ecat_task_add:               Add new task to EtherCAT master
                    ecat_task_del:                Remove task from task list and free Resources
                    ecat_slave_config:          Configure the specific slave when other slaves is working
                    app_ecat_config:            APP request new EtherCAT task interface
History:

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/
#include "types.h"
#include "app_config.h"
#include "app_data.h"
#include "ec_globals.h"
#include "app_driver_func.h"
#include "fsm_slave_config.h"
#include "fsm_master.h"
#include "fsm_slave.h"
#include "master.h"
#include "slave.h"
#include "mailbox.h"
#include "sdo_request.h"
#include "base_data_op.h"
#include "debug.h"
#include "App_data.h"
#include "string.h"
#include "machine.h"
#include "wdog.h"
ecat_config_t ecat_config_array[MAX_SLV_NUM];

/******  EtherCAT app task management  ******/
//task table header
struct ec_task_top ec_task_head;
extern m_cycle_slave_t m_cycle_slave_info[M_CYCLE_SLAVE_NUM];
extern u32 dyn_dgram_ok; //defined in ec_app_interface.c

ec_sdo_request_t app_sdo_request; /**< SDO request. */
u8 sdo_datagram_data[SDO_DATAGRAM_DATA_LEN];

#ifdef BECKHOFF_EL9800_SLAVE
ec_sc_info_t ec_sc_info_el9800[NUM_EL9800];
#endif
#ifdef HC_IS580_SLAVE
ec_sc_info_t ec_sc_info_is580[NUM_IS580];
#endif
#ifdef HC_ES750_SLAVE
ec_sc_info_t ec_sc_info_es750[NUM_ES750];
#endif
#ifdef HC_IS620_SLAVE
ec_sc_info_t ec_sc_info_is620[NUM_IS620];
#endif
#ifdef ESTO_E410_SLAVE
ec_sc_info_t ec_sc_info_e410[NUM_E410];
#endif

#ifndef DYN_MEM_ALLOC
ec_task_datagram_t task_datagram_array[MAX_TASK_NUM][DATAGRAM_PER_TASK];
uint16_t	task_datagram_idx;
ec_task_t	ec_task_array[MAX_TASK_NUM];
uint16_t	task_idx;
#endif

u32 *app_ecat_config(u32 interval, u32 ec_master_idx, u32 domain_flag, u8 *sc_name, u8 *task_name);

//slave config name
u8 null_name[]    = "NULL";
char el9800_name[]  = "EL9800";
u8 is580_name[]   = "IS580";
u8 es750_name[]   = "ES750";
u8 is620_name[]   = "IS620";
char e410_name[]    = "E410";
u16 sc_id;
u32 configed_slave;
/*
** ecat_pdo_entry_offset, Initialize the data offset of entries
*/
void ecat_pdo_entry_offset(ec_pdo_info_t *pdo, uint32_t offset[][MAX_ENTRY_PER_PDO])
{
//	ec_pdo_entry_info_t *entry;
    int i, j;
    uint32_t byte_offset = 0;
    j = 0;
    while (pdo->index != 0xFFFF)
    {
        byte_offset = 0;
        for (i = 0; i < pdo->n_entries; i++)
        {
            offset[j][i] = byte_offset;
            byte_offset += (pdo->entries[i].bit_length + 7) >> 3;
        }
    }
}


/*
**ecat_app_slave_init: initialize slave configuration in EtherCAT Network
@return: 0, success; -1, failure
*/
#define SC_ID_INCR	(sc_id++)
void ecat_app_slave_init()
{
    int i, j;
    ec_sc_info_t *ec_sc_info;
    j = 0;
    for (i = 0; i < MAX_SLV_NUM; i++)
    { 
        ecat_config_array[i].name               = null_name;
        ecat_config_array[i].ec_sc_info_array   = NULL;
        ecat_config_array[i].num_slave          = 0;
    }
    sc_id = SC_ID_START;
    configed_slave = 0;
    /************************** Beckhoff EL9800 **************************/
#ifdef BECKHOFF_EL9800_SLAVE
    if (NUM_EL9800 > 0)
    {
#ifdef DYN_MEM_ALLOC
#ifdef EC_DYN_MEM_PRT
        _DBG_PRINTF_("%d in %s, ec_malloc size=%d\n", __LINE__, __FILE__, sizeof(ec_sc_info_t) * NUM_EL9800);
#endif
        ec_sc_info = (ec_sc_info_t *)ec_malloc(sizeof(ec_sc_info_t) * NUM_EL9800, 0);
        if (ec_sc_info == NULL)
        {
            _DBG_PRINTF_("Allocate memory for EL9800 fail\n");
            return -1;
        }
#else
        ec_sc_info = ec_sc_info_el9800;
#endif
        ecat_config_array[j].ec_sc_info_array   = ec_sc_info;
        ecat_config_array[j].name               = el9800_name;
        ecat_config_array[j].num_slave          = NUM_EL9800;
        i = 0;
        j++;
        while (i < NUM_EL9800)
        {
            ec_sc_info->sync_cfg_info   = slave_el9800_syncs;
            ec_sc_info->pdo_entry_info  = el9800_domain_regs;
            ec_sc_info->alias           = ALIAS_EL9800;
            ec_sc_info->position        = i;
            ec_sc_info->app_sc_id       = SC_ID_INCR;
            ec_sc_info->product_code    = PCODE_EL9800;
            ec_sc_info->vendor_id       = VID_BECKHOFF;
            ec_sc_info->sync0_cycle     = EL9800_SYNC_CYCLE;
            ec_sc_info->wdt_div         = EL9800_WDT_DIV;
            ec_sc_info->pdo_wdt         = EL9800_PDO_WDT;
            ec_sc_info->dc_active       = EL9800_DC_ACTIVE;
            ec_sc_info++;
            i++;
        }
        //ecat_pdo_entry_offset(slave_el9800_pdos, el9800_pdo_off);
    }
#endif

		/************************** Huichuan IS650/IS680 **************************/
#ifdef HC_IS620_SLAVE
		if (NUM_IS620 > 0)
		{
#ifdef DYN_MEM_ALLOC
#ifdef EC_DYN_MEM_PRT
			_DBG_PRINTF_("%d in %s, ec_malloc size=%d\n", __LINE__, __FILE__, sizeof(ec_sc_info_t) * NUM_IS620);
#endif
			ec_sc_info = (ec_sc_info_t *)ec_malloc(sizeof(ec_sc_info_t) * NUM_IS620, 0);
			if (ec_sc_info == NULL)
			{
				_DBG_PRINTF_("Allocate memory for IS620 fail\n");
				return -1;
			}
#else
			ec_sc_info = ec_sc_info_is620;
#endif
			ecat_config_array[j].ec_sc_info_array	= ec_sc_info;
			ecat_config_array[j].name				= is620_name;
			ecat_config_array[j].num_slave			= NUM_IS620;
			j++;
			i = 0;
			while (i < NUM_IS620)
			{			
				{
					ec_sc_info->sync_cfg_info	= slave_is620_syncs;
					ec_sc_info->pdo_entry_info	= NULL;
					ec_sc_info->alias			= ALIAS_IS620;
					ec_sc_info->position		= i;
					ec_sc_info->app_sc_id		= SC_ID_INCR;
					
					//ECAT编码校对可设
					if(ChkSysCfg(CO8_FLLMT))
					{				  
						if (i < g_pump_num)
						{			 
							ec_sc_info->product_code = 0x0019010E;
						}
					}
					else
						ec_sc_info->product_code	= IS620_PRODUCT_ID;
	
					ec_sc_info->vendor_id		= VID_HUICHUAN;
					ec_sc_info->sync0_cycle 	= IS620_SYNC_CYCLE;
					ec_sc_info->wdt_div 		= IS620_WDT_DIV;
					ec_sc_info->pdo_wdt 		= IS620_PDO_WDT;
					ec_sc_info->dc_active		= IS620_DC_ACTIVE;
				}
				ec_sc_info++;
				i++;
			}
		}
#endif

		/************************** Huichuan IS580 **************************/
#ifdef HC_IS580_SLAVE
		if (NUM_IS580 > 0)
		{
#ifdef DYN_MEM_ALLOC
#ifdef EC_DYN_MEM_PRT
			_DBG_PRINTF_("%d in %s, ec_malloc size=%d\n", __LINE__, __FILE__, sizeof(ec_sc_info_t) * NUM_IS580);
#endif
			ec_sc_info = (ec_sc_info_t *)ec_malloc(sizeof(ec_sc_info_t) * NUM_IS580, 0);
			if (ec_sc_info == NULL)
			{
				_DBG_PRINTF_("Allocate memory for IS580 fail\n");
				return -1;
			}
#else
			ec_sc_info = ec_sc_info_is580;
#endif
			ecat_config_array[j].ec_sc_info_array	= ec_sc_info;
			ecat_config_array[j].name				= is580_name;
			ecat_config_array[j].num_slave			= NUM_IS580;
			j++;
			i = 0;
			while (i < NUM_IS580)
			{			
				ec_sc_info->sync_cfg_info	= slave_is580_syncs;
				ec_sc_info->product_code	= IS580_PRODUCT_ID;
				ec_sc_info->pdo_entry_info	= NULL;
				ec_sc_info->alias			= ALIAS_IS580;
				ec_sc_info->position		= i;
				ec_sc_info->app_sc_id		= SC_ID_INCR;
				ec_sc_info->vendor_id		= VID_HUICHUAN;
				ec_sc_info->sync0_cycle 	= IS580_SYNC_CYCLE;
				ec_sc_info->wdt_div 		= IS580_WDT_DIV;
				ec_sc_info->pdo_wdt 		= IS580_PDO_WDT;
				ec_sc_info->dc_active		= IS580_DC_ACTIVE;
				
				ec_sc_info++;
				i++;
			}
		}
#endif

			/************************** Huichuan ES750 **************************/
#ifdef HC_ES750_SLAVE
		if (NUM_ES750 > 0)
		{
#ifdef DYN_MEM_ALLOC
#ifdef EC_DYN_MEM_PRT
			_DBG_PRINTF_("%d in %s, ec_malloc size=%d\n", __LINE__, __FILE__, sizeof(ec_sc_info_t) * NUM_ES750);
#endif
			ec_sc_info = (ec_sc_info_t *)ec_malloc(sizeof(ec_sc_info_t) * NUM_ES750, 0);
			if (ec_sc_info == NULL)
			{
				_DBG_PRINTF_("Allocate memory for ES750 fail\n");
				return -1;
			}
#else
			ec_sc_info = ec_sc_info_es750;
#endif
			ecat_config_array[j].ec_sc_info_array	= ec_sc_info;
			ecat_config_array[j].name				= es750_name;
			ecat_config_array[j].num_slave			= NUM_ES750;
			j++;
			i = 0;
			while (i < NUM_ES750)
			{			
				ec_sc_info->sync_cfg_info	= slave_es750_syncs;
				ec_sc_info->product_code	= ES750_PRODUCT_ID;
				ec_sc_info->pdo_entry_info	= NULL;
				ec_sc_info->alias			= ALIAS_ES750;
				ec_sc_info->position		= i;
				ec_sc_info->app_sc_id		= SC_ID_INCR;
				ec_sc_info->vendor_id		= VID_HUICHUAN;
				ec_sc_info->sync0_cycle 	= ES750_SYNC_CYCLE;
				ec_sc_info->wdt_div 		= ES750_WDT_DIV;
				ec_sc_info->pdo_wdt 		= ES750_PDO_WDT;
				ec_sc_info->dc_active		= ES750_DC_ACTIVE;
				
				ec_sc_info++;
				i++;
			}
		}
#endif


    /************************** Huichuan E410 **************************/
#ifdef ESTO_E410_SLAVE
    if (NUM_E410 > 0)
    {
#ifdef DYN_MEM_ALLOC
#ifdef EC_DYN_MEM_PRT
        _DBG_PRINTF_("%d in %s, ec_malloc size=%d\n", __LINE__, __FILE__, sizeof(ec_sc_info_t) * NUM_E410);
#endif
        ec_sc_info = (ec_sc_info_t *)ec_malloc(sizeof(ec_sc_info_t) * NUM_E410, 0);
        if (ec_sc_info == NULL)
        {
            _DBG_PRINTF_("Allocate memory for E410 fail\n");
            return -1;
        }
#else
        ec_sc_info = ec_sc_info_e410;
#endif
        ecat_config_array[j].ec_sc_info_array   = ec_sc_info;
        ecat_config_array[j].name               = e410_name;
        ecat_config_array[j].num_slave          = NUM_E410;
        j++;
        i = 0;
        while (i < NUM_E410)
        {
            ec_sc_info->sync_cfg_info   = slave_e410_syncs;
            ec_sc_info->pdo_entry_info  = e410_domain_regs;
            ec_sc_info->alias           = ALIAS_E410;
            ec_sc_info->position        = i;
            ec_sc_info->app_sc_id       = SC_ID_INCR;
            ec_sc_info->product_code    = PCODE_E410;
            ec_sc_info->vendor_id       = VID_ESTO;
            ec_sc_info->sync0_cycle     = E410_SYNC_CYCLE;
            ec_sc_info->wdt_div         = E410_WDT_DIV;
            ec_sc_info->pdo_wdt         = E410_PDO_WDT;
            ec_sc_info->dc_active       = E410_DC_ACTIVE;
            ec_sc_info++;
            i++;
        }
        //ecat_pdo_entry_offset(slave_e410_pdos, e410_pdo_off);
    }
#endif
#ifndef DYN_MEM_ALLOC
    task_datagram_idx	= 0;
    task_idx			= 0;
#endif
}

/*
** ecat_task_top_init, EtherCAT task management variabes initialization
*/
void ecat_task_top_init()
{
    INIT_LIST_HEAD(&(ec_task_head.ec_tasks));
    ec_task_head.task_cnt = 0;
    ec_task_head.task_index = 0;
    //put the start dress of app_ecat_config to the fixed position in stepping stone(SRAM)
    m_cycle_slave_init();
    ec_sdo_request_init(&app_sdo_request);
    app_sdo_request.data = sdo_datagram_data;
    app_sdo_request.mem_size = SDO_DATAGRAM_DATA_LEN;
    //Slave Config initialization
    ecat_app_slave_init();
    dyn_dgram_ok = 1;
}
void ec_task_datagram_init(ec_task_t *ec_task)
{
    int i;
    ec_task_datagram_t 	*ec_task_datagram_ptr;
#ifdef DYN_MEM_ALLOC
    data_size = (sizeof(ec_task_datagram_t) + 3) & (~0x3);

#ifdef EC_DYN_MEM_PRT
    _DBG_PRINTF_("%d in %s, ec_malloc size=%d\n", __LINE__, __FILE__, data_size * DATAGRAM_PER_TASK);
#endif

    tsk_data_buf = (u8 *)ec_malloc(data_size * DATAGRAM_PER_TASK, 0);
    if (!tsk_data_buf)
    {
        debug("%s, ec_task_datagram_t allocation failure!!!\n", ec_task->name);
    }

    for (i = 0; i < DATAGRAM_PER_TASK; i++)
    {
        ec_task_datagram_ptr = (ec_task_datagram_t *)(tsk_data_buf + (i * data_size));
        ec_datagram_init(&(ec_task_datagram_ptr->datagram));
        list_add_tail(&(ec_task_datagram_ptr->list), &(ec_task->task_datagrams));
        ec_task_datagram_ptr->datagram.data_origin = EC_ORIG_EXTERNAL;
    }
#else

    if (task_datagram_idx < MAX_TASK_NUM)
    {
        ec_task_datagram_ptr = task_datagram_array[task_datagram_idx];
        task_datagram_idx++;
    }
    else
    {
        _DBG_PRINTF_("in %s, No enough task_datagram_array reserved\n", __func__);
        return;
    }

    for (i = 0; i < DATAGRAM_PER_TASK; i++)
    {
        //ec_task_datagram_ptr = (ec_task_datagram_t *)(tsk_data_buf + (i * data_size));
        ec_datagram_init(&(ec_task_datagram_ptr[i].datagram));
        list_add_tail(&(ec_task_datagram_ptr[i].list), &(ec_task->task_datagrams));
        ec_task_datagram_ptr[i].datagram.data_origin = EC_ORIG_EXTERNAL;
    }

#endif

}


/*
** ecat_task_add, create new EtherCAT Task
*/
ec_task_t *ecat_task_add(ec_master_t *master, ec_domain_t *domain, const u8 *name)
{
    ec_task_t *app_task;
    int i;
#ifdef DYN_MEM_ALLOC
#ifdef EC_DYN_MEM_PRT
    _DBG_PRINTF_("%d in %s, ec_malloc size=%d\n", __LINE__, __FILE__, sizeof(ec_task_t));
#endif
    app_task = (ec_task_t *)ec_malloc(sizeof(ec_task_t), 0);
#else
    if (task_idx < MAX_TASK_NUM)
    {
        app_task = &ec_task_array[task_idx];
        task_idx++;
    }
    else
    {
        _DBG_PRINTF_("in %s, No enough ec_task_array reserved\n", __func__);
        app_task = NULL;
    }
#endif
    if (!app_task)
    {
        return NULL;
    }

    app_task->master                    = master;
    app_task->domain                    = domain;
    app_task->sc_num                    = 0;
    strcpy((char *)(app_task->name), (const char *)name);
    //ec_task_info->expected_wkc = 0;
    app_task->task_index                = ec_task_head.task_index++;
    app_task->task_state                = 0;
    app_task->m_clcye_slave             = NULL;
    app_task->retry_time                = 0;
    for (i = 0; i < MAX_SC_PER_TASK; i++)
    {
        app_task->app_ecat_inter[i].station_addr = 0xFFFF;
        app_task->app_ecat_inter[i].data_buff  = NULL;
        app_task->app_ecat_inter[i].ecatprocess = 0;
        app_task->app_ecat_inter[i].sent_1ms   = 0;
    }
    list_add_tail(&app_task->list, &(ec_task_head.ec_tasks));
    ec_task_head.task_cnt++;
    INIT_LIST_HEAD(&(app_task->task_datagrams));
    return app_task;
}

/*
** ecat_task_del, free the selected task
*/
void ecat_task_del(ec_task_t *ec_task_info)
{
    list_del(&ec_task_info->list);
#ifdef DYN_MEM_ALLOC
    ec_free(ec_task_info);
#endif
    ec_task_head.task_cnt--;
}
/*
** ecat_slave_config, config the specific slave
** fsm,     EtherCAT master pointer
**slave,    the EtherCAT slave which will be configured
*/
int ecat_slave_config(ec_master_t *master, ec_slave_t *slave)
{

    ec_fsm_slave_config_start(&master->fsm.fsm_slave_config, slave);
    while (ec_fsm_slave_config_exec(&master->fsm.fsm_slave_config))
    {
        if (!(ec_mbox_is_datagram_state(master->fsm.mbox, EC_DATAGRAM_QUEUED)
                || ec_mbox_is_datagram_state(master->fsm.mbox, EC_DATAGRAM_SENT)))
        {
            ec_master_mbox_queue_datagrams(master, &master->fsm_mbox);
        }
        ecrt_master_send(master);
        ecrt_master_receive(master);
    }

    return 0;
}


/*
** app_ecat_config: interface between ECAT master and application programs
** parameters
** u32 ec_master_idx, requested master index
** u32 ec_sc_idx, requested Slave Config index
** u32 domain_flag, the mode of the new task to transmit data, domain(1) or individual datagram
** return: the pointer of app task; NULL when failure
*/
u32 *app_ecat_config(u32 interval, u32 ec_master_idx, u32 domain_flag, u8 *sc_name, u8 *task_name)
{
    ec_master_t 				*master;
    ec_domain_t				*domain, *domain_sel;
    ec_datagram_t				*datagram;
    static ec_slave_config_t		*sc;
    ec_sc_info_t				*ec_sc_info_ptr;
    ec_task_t *				app_task = NULL;
    int						i, ret;
    int						j, m, k;
    u32						rx_pdo_len, tx_pdo_len;
    u32						domain_offset;
    ecat_config_t				*ecat_config_ptr;
    i = 0;
    while (strcmp((const char *)(ecat_config_array[i].name), "NULL"))
    {
        if (!strcmp((const char *)(ecat_config_array[i].name), (const char *)sc_name))
        {
            break;
        }
        i++;
        if (i == MAX_SLV_NUM)
        {
            return NULL;
        }
    }

    if (ecat_config_array[i].ec_sc_info_array == NULL)
    {
        _DBG_PRINTF_("no config information for %s\n", sc_name);
        return NULL;
    }
    ec_sc_info_ptr = ecat_config_array[i].ec_sc_info_array;
    ecat_config_ptr = &ecat_config_array[i];
    master = ecrt_request_master(ec_master_idx);
    //if all masters were occupied, chose master of first task
    if (!master)
    {
        list_for_each_entry(app_task, ec_task_t, &ec_task_head.ec_tasks, list)
        {
            if (app_task->master)
            {
                master = app_task->master;
                break;
            }
        }
    }
    if (!master)
    {
        return NULL;
    }
    if (domain_flag)
    {
        if (!rx_domain)
        {
            domain		= ecrt_master_create_domain(master);
            rx_domain	= domain;
            if (!domain)
            {
                return NULL;
            }
            //Buffer is provided by application
            domain->data_origin = EC_ORIG_EXTERNAL;
            rx_domain->data		= NULL;
        }
        else
        {
            domain = rx_domain;
        }
    }
    else
    {
        domain = NULL;
    }

    if (ecat_config_ptr->num_slave > MAX_SC_PER_TASK)
    {
        _DBG_PRINTF_("The requested number of slave exeed MAX_SC_PER_TASK(%d)\n", MAX_SC_PER_TASK);
        return NULL;
    }
    i = 0;
    while ((i < ecat_config_ptr->num_slave) && (configed_slave < master->slave_count))
    {

#if 0 //gonggw, 2014.11.24, Original code ,with alias & position match
        sc  = ecrt_master_slave_config(master, ec_sc_info_ptr->alias, ec_sc_info_ptr->position, \
                                       ec_sc_info_ptr->vendor_id, ec_sc_info_ptr->product_code, 0);
        //_DBG_PRINTF_("SC info(%x): slave=%x, station_addr=%x, alias=%x, pos=%x, vendor_id=%x, pr_code=%x\n", ec_sc_info_ptr,
        //        sc->slave, sc->slave->station_address, ec_sc_info_ptr->alias, ec_sc_info_ptr->position, ec_sc_info_ptr->vendor_id, ec_sc_info_ptr->product_code);
#else
        sc  = ecrt_master_slave_config(master, ec_sc_info_ptr->alias, ec_sc_info_ptr->position, \
                                       ec_sc_info_ptr->vendor_id, ec_sc_info_ptr->product_code, ec_sc_info_ptr->app_sc_id);
#endif
        if (!sc)
        {
            debug("Failed to get slave configuration\n");
            i++;
            ec_sc_info_ptr++;
            continue;
            //return NULL;
        }

        if (app_task == NULL)
        {
            app_task = ecat_task_add(master, domain, task_name);
        }
        configed_slave++;
        //Watchdog Timer configuration
        ecrt_slave_config_watchdog(sc, ec_sc_info_ptr->wdt_div, ec_sc_info_ptr->pdo_wdt);
        //Distributed Clock Configuration
        ecrt_slave_config_dc(sc, ec_sc_info_ptr->dc_active, ec_sc_info_ptr->sync0_cycle, 0, 0, 0);
#if 0
        if (master->has_app_time == 0)
        {
            u64 jiffies_ns;
            jiffies_ns = jiffies_64 * 1000;
            do_div(&jiffies_ns, HZ);
            jiffies_ns = jiffies_ns * 1000000;
            ecrt_master_application_time(master, jiffies_ns);
        }
#endif
        if (ecrt_slave_config_pdos(sc, EC_END, ec_sc_info_ptr->sync_cfg_info))
        {
            debug("Failed to configure PDOs.\n");
            return NULL;
        }
        if (app_task->domain)
        {
            if (ecrt_domain_reg_pdo_entry_list(domain, ec_sc_info_ptr->pdo_entry_info))
            {
                debug("PDO entry registration failed!\n");
                return NULL;
            }
        }
        ec_master_request_op(master);
        if (master->active)
        {

            _DBG_PRINTF_("Config Slave slave %d...\n", sc->slave->station_address);
            ecat_slave_config(master, sc->slave);
        }
        if (!app_task->domain)
        {
            ec_task_datagram_init(app_task);
        }

        app_task->app_ecat_inter[app_task->sc_num].station_addr = sc->slave->station_address;
        j = 0;
        rx_pdo_len = 0;
        tx_pdo_len = 0;
        while (ec_sc_info_ptr->sync_cfg_info[j].index != 0xFF)
        {
            for (k = 0; k < ec_sc_info_ptr->sync_cfg_info[j].n_pdos; k++)
            {
                for (m = 0; m < ec_sc_info_ptr->sync_cfg_info[j].pdos[k].n_entries; m++)
                {
                    if (ec_sc_info_ptr->sync_cfg_info[j].dir == EC_DIR_OUTPUT)
                    {
                        rx_pdo_len += ec_sc_info_ptr->sync_cfg_info[j].pdos[k].entries[m].bit_length;
                    }
                    else
                    {
                        tx_pdo_len += ec_sc_info_ptr->sync_cfg_info[j].pdos[k].entries[m].bit_length;
                    }
                }
            }
            j++;
        }
        app_task->app_ecat_inter[app_task->sc_num].rxpdo_num = (u8)(rx_pdo_len >> 3);
        app_task->app_ecat_inter[app_task->sc_num].txpdo_num = (u8)(tx_pdo_len >> 3);
#if 1
        _DBG_PRINTF_("slave%d: rxpdo %d bytes, txpdo %d bytes\n\r", app_task->app_ecat_inter[app_task->sc_num].station_addr,
                     app_task->app_ecat_inter[app_task->sc_num].rxpdo_num, app_task->app_ecat_inter[app_task->sc_num].txpdo_num);
		FeedDog();
#endif
        app_task->sc_num++;
        ec_sc_info_ptr++;
        i++;
    }
    //add new EtherCAT Task
    if (!app_task)
    {
        return NULL;
    }

    if (app_task->domain)
    {
        domain_offset = 0;
        list_for_each_entry(domain_sel, ec_domain_t, &master->domains, list)
        {
            if (domain_sel != domain)
            {
                domain_offset += domain_sel->data_size;
            }
        }
#if 0
        ret = ec_domain_finish(domain, domain_offset);
#endif
        if (ret < 0)
        {
            EC_MASTER_ERR(master, "Failed to finish domain 0x%p!\n", domain);
            return NULL;
        }
        list_for_each_entry(datagram, ec_datagram_t, &app_task->domain->datagrams, list)
        {
            datagram->data_origin = EC_ORIG_EXTERNAL;
        }
    }

    return (u32 *)app_task;
}


