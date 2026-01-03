/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    app_driver_func.c

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:            2013.11.04

Description:    Driver Intefaces

Others:

Function List:  
                    app_driver_func:    Unified driver inteface
                    ecat_task_req:       EtherCAT task request
History:    

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/
#include "types.h"
#include "debug.h"
#include "app_driver_func.h"
#include "app_data.h"
#include "master.h"
#include "systempro.h"
#include "ecat_hal.h"
#include "Servo.h"
#include "ServoEcat.h"

extern UI8 ServoType[MAX_SERVO_NUM];

#ifdef HC_IS620_SLAVE
int ecat_is620_board[NUM_IS620];    // ????????????????is620
int ecat_is620_board_total = 0;
ec_task_t    *ec_task_hc;
#endif

#ifdef HC_IS580_SLAVE
int ecat_is580_board[NUM_IS580] = {0};    // ????????????????is620
int ecat_is580_board_total = 0;
ec_task_t    *ec_task_hc;
#endif

#ifdef HC_ES750_SLAVE
int ecat_es750_board[NUM_ES750] = {0};    // ????????????????is620
int ecat_es750_board_total = 0;
ec_task_t    *ec_task_hc;
#endif

void ServoTypeProc(void);

app_ecat_interface_t *ec_app_inter[MAX_SC_PER_TASK];


u32 *app_ecat_config(u32 interval, u32 ec_master_idx, u32 domain_flag, u8 *sc_name, u8 *task_name);
int master_sync(ec_master_t *master);


/**  app_driver_func: unified interface between APP and Driver  **/
/**  dev_id: Device ID, for selecting driver to complete APP request  **/
/**  chip: Device selecting if the number of device with dev_id is more than 1  **/
/**  data: data buffer to exchange data between driver and APP  **/
/**  data_len: data length of data  **/
/**  addr: register address in specified peripherals  **/
/**  addr_len: 0, addr is invalid; else, access specified registers in specified device  **/
/**  op_mode: operation mode, READ/WRITE etc.  **/
/**  return value: 0 for success; negative value for failure  **/
#if 0
int app_driver_func(u32 dev_id, u32 chip, u8 *data, u32 data_len, u8 *addr, u32 addr_len, u32 op_mode)
{
    int ret = 0;
    u32 op_mode_op = op_mode & 0xFFFF;
    switch (dev_id)
    {
    case DEV_TIMER:
    case DEV_PWM:
        ret = timer_process(chip, *(u32 *)addr, (u32 *)data, op_mode_op);
        break;
    case DEV_UART:
        ret = serial_process(data, data_len, addr, addr_len, op_mode);
        break;
    case DEV_ECAT:
    {
        if (op_mode_op == DEV_OP_CONFIG)
        {
            u32 *ecat_cfg_ret;
            u32 interval, ec_master_idx, domain_flag;
            u8  *sc_name;
            u8  *task_name;
            interval        = data_len;
            ec_master_idx   = chip;
            sc_name         = data;
            task_name       = addr;
            domain_flag     = addr_len;
            ecat_cfg_ret    = (u32 *)app_ecat_config(interval, ec_master_idx, domain_flag, sc_name, task_name);
			if (!ecat_cfg_ret)
                ret = DEV_OP_ERR;
            else
            {
                *(u32 *)data = (u32)ecat_cfg_ret;
            }
        }
        else
        {
            if (op_mode_op > DEV_OP_RW)
                ret = DEV_OP_ERR;
            else
                app_data_process();
        }
        break;
    }
    case DEV_WDT:
        s3c6410_wdt_feed();
        break;
    case DEV_I2C:
        if (op_mode & DEV_OP_INT)
        {
            //I2C1_ReceiveNum = 0;
            ret = i2c_int_start((u8)I2C1_SLAVE_ADDRESS7, addr, addr_len, *data, data_len, op_mode_op);
        }
        else
        {
            if (op_mode_op == DEV_OP_READ)
                ret = i2c_read((u8)chip, *(u32 *)addr, addr_len, data, data_len);
            else if (DEV_OP_WRITE)
                ret = i2c_write((u8)chip, *(u32 *)addr, addr_len, data, data_len);
        }
        if (ret)
            ret = DEV_OP_ERR;
        break;
    case DEV_NAND:
        /**  TODO: add NAND functions when needed **/
        break;
    case DEV_GPIO:
        ret = gpio_process(chip,(u32 *)data, data_len, op_mode_op);
        break;
#ifdef A920_620_DRV
    case DEV_AD:
        if (chip == 0) //A620
            *(uint16_t *)data = GetAD_A620(*(int *)addr);
        else if (chip == 1) //A920
            *(uint16_t *)data = GetAD_A920(*(int *)addr);
        else
            return DEV_OP_ERR;
        break;
    case DEV_TEMP:
        if (op_mode_op == DEV_OP_READ)
        {
            ret = Get_ValueLM73((u16 *)data);
        if (ret == 1)
            ret = 0;
        else
            ret = DEV_OP_ERR;
        }
        else
        {
            ret = DEV_OP_ERR;
        }
        break;
#endif            
    default:
        debug("DEV_ID(%x) error\n", dev_id);
        return DEV_ID_ERR;
    }
    return ret;
}
#endif
/*
**ecat_task_req: Application task request
@return value, 0, success; -1 fail
*/

	u64 jiffies_64_store=0;
	extern u64 jiffies_64_ch;
int ecat_task_req()
{
	int			i, slave_num, j;
	u8			slave_state = 0;
	ec_task_t		*ec_task;
	ec_master_t	*master = cur_master;
	_DBG_PRINTF_("Startign Slave Config...\n\r");
	slave_num	= 0;
	int sc_total = 0;
	int sc_squnc = 0;
	int z = 0;
#ifdef BECKHOFF_EL9800_SLAVE
	ec_task					= (ec_task_t *)app_ecat_config(100, 0, 0, "EL9800", "task0");
		
	if (ec_task)
	{
		ec_task_el9800		= ec_task;
		for(j = 0; j < ec_task->sc_num; j++)
			esc_state.station_address[slave_num++]	= ec_task->app_ecat_inter[j].station_addr;
	}
#endif

#ifdef HC_IS580_SLAVE
	ec_task					= (ec_task_t *)app_ecat_config(100, 0, 0, "IS580", "task1");
	if (ec_task)
	{
		ec_task_hc			= ec_task;
		j = sc_total ;
    	sc_total = sc_total + ec_task->sc_num;
		for(z = 0; j < sc_total; j++,z++)
		{
			sc_squnc = ec_task->app_ecat_inter[z].station_addr - 1;
			esc_state.station_address[sc_squnc] = ec_task->app_ecat_inter[z].station_addr;
			ec_app_inter[sc_squnc]	= &(ec_task->app_ecat_inter[z]);
			hc_rxpdo_size[sc_squnc]=ec_app_inter[sc_squnc]->rxpdo_num;
			hc_txpdo_size[sc_squnc]=ec_app_inter[sc_squnc]->txpdo_num;
			ServoType[sc_squnc] = TYPE_IS580;
			slave_num++;
			ecat_is580_board_total++;
      		ecat_is580_board[z]=sc_squnc;
		}
	}
#endif

#ifdef HC_ES750_SLAVE
	ec_task					= (ec_task_t *)app_ecat_config(100, 0, 0, "ES750", "task2");
	if (ec_task)
	{
		ec_task_hc			= ec_task;
		j = sc_total ;
    	sc_total = sc_total + ec_task->sc_num;
		for(z = 0; j < sc_total; j++,z++)
		{
			sc_squnc = ec_task->app_ecat_inter[z].station_addr - 1;
			esc_state.station_address[sc_squnc] = ec_task->app_ecat_inter[z].station_addr;
			ec_app_inter[sc_squnc]	= &(ec_task->app_ecat_inter[z]);
			hc_rxpdo_size[sc_squnc]=ec_app_inter[sc_squnc]->rxpdo_num;
			hc_txpdo_size[sc_squnc]=ec_app_inter[sc_squnc]->txpdo_num;
			ServoType[sc_squnc] = TYPE_ES750;
			slave_num++;
			ecat_es750_board_total++;
      		ecat_es750_board[z]=sc_squnc;
		}
	}
#endif


#ifdef HC_IS620_SLAVE
	ec_task =					(ec_task_t *)app_ecat_config(100, 0, 0, "IS620", "task3");
	if (ec_task)
	{
		ec_task_hc = ec_task;
    	j = sc_total ;
    	sc_total = sc_total + ec_task->sc_num;
		for(z = 0 ; j < sc_total; j++,z++)        
		{
      		sc_squnc = ec_task->app_ecat_inter[z].station_addr - 1;
			esc_state.station_address[sc_squnc] = ec_task->app_ecat_inter[z].station_addr;
			ec_app_inter[sc_squnc]	= &(ec_task->app_ecat_inter[z]);
			hc_rxpdo_size[sc_squnc] = ec_app_inter[sc_squnc]->rxpdo_num;
			hc_txpdo_size[sc_squnc] = ec_app_inter[sc_squnc]->txpdo_num;
			ServoType[sc_squnc] = TYPE_IS620;
			slave_num++;      
     		ecat_is620_board_total++;
      		ecat_is620_board[z]=sc_squnc;
		}
	}
#endif
#ifdef ESTO_E410_SLAVE
	ec_task        = (ec_task_t *)app_ecat_config(100, 0, 1, "E410", "task_est");
	if (ec_task)
	{
		ec_task_e410   = ec_task;
		for(j = 0; j < ec_task->sc_num; j++)
		{
			esc_state.station_address[slave_num++] = ec_task->app_ecat_inter[j].station_addr;
			ec_app_inter[j] = &(ec_task->app_ecat_inter[j]);
			hc_rxpdo_size[j]=ec_app_inter[j]->rxpdo_num;
			hc_txpdo_size[j]=ec_app_inter[j]->txpdo_num;
		}
	}
#endif
	//if (ec_task == NULL)
	if (slave_num == 0)
	{
		_DBG_PRINTF_("*** request task fail\n\r");
		return -1;
	}
	delay_ms(100);
	_DBG_PRINTF_("Activate Master...\n\r");

	if (master_sync(master))
		return -1;
	jiffies_64_store = jiffies_64_ch;
	//jiffies_test = jiffies;
	ecrt_master_activate(master);
	//uart_printf("active=%d\n", (uint32_t)(jiffies - jiffies_test));
	 esc_state.target_wkc = master->slave_count;
	esc_state.start = 1;
	master_dyn_sync_start = 1;
#if (defined(HC_IS620_SLAVE)||defined(HC_IS580_SLAVE)||defined(HC_ES750_SLAVE))
	ec_app_data_p->real_slave_num = cur_master->slave_count;
	for(i = 0; i < ec_app_data_p->real_slave_num; i++)
	{
		ec_app_data_p->slave_state[i] = (u8)(cur_master->slaves[i].current_state);
		slave_state	|= (u8)(cur_master->slaves[i].current_state);
	}
	slave_state = slave_state & 0xFF;
	if (slave_state == 0x8)
	{
		ec_app_data_p->ecat_net_state = ECAT_NET_OK;
	}
	else if (slave_state == 0)
	{
		ec_app_data_p->ecat_net_state = ECAT_NET_MASTER_ERR;
	}
	else if (((slave_state & 0x8) == 0x0) || (slave_state & 0x10))
	{
		ec_app_data_p->ecat_net_state = ECAT_NET_SLAVE_ERR;
	}
	else
	{
		ec_app_data_p->ecat_net_state = ECAT_NET_SLAVE_MISS;
	}
#endif
	ServoTypeProc();
	return 0;
}

