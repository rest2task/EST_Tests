/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    fsm_slave_scan.c

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:           2014.06.06

Description:  slave scan FSM

Others:

Function List:  
                    ec_fsm_slave_scan_init: slave scan FSM structures init                   
                    ec_fsm_slave_scan_clear: slave scan FSM structures clear 
                    ec_fsm_slave_scan_running: flag of slave scan is not error or end
                    ec_fsm_slave_scan_exec:Executing current slave scan state function
                    ec_fsm_slave_scan_success: Slave Scan success flag
                    ec_fsm_slave_scan_state_start: Start state of slave scan
                    ec_fsm_slave_scan_state_address:Slave Scan FSM: Send read AL state datagram
                    ec_fsm_slave_scan_state_state: Slave Scan FSM: Check the AL state and Send read slave base info datagram 
                    ec_fsm_slave_scan_state_base:  Slave Scan FSM: analyze the returned Slave's base info
                    ec_fsm_slave_scan_state_dc_cap:Slave Scan FSM: Read Slave's port receiving time
                    ec_fsm_slave_scan_state_dc_times: Slave Scan FSM: record Slave's port receiving time
                    ec_fsm_slave_scan_enter_datalink: enter Read data Link status state
                    ec_fsm_slave_scan_state_datalink:  Slave Scan FSM: set slave's Data Link Status
                    ec_fsm_slave_scan_state_sii_size: Slave Scan FSM: Read the size of valid data in EEPROM
                    ec_fsm_slave_scan_state_sii_data: Slave Scan FSM: Read valid data in EEPROM 
                    ec_fsm_slave_scan_enter_regalias: enter Read alias register state
                    ec_fsm_slave_scan_state_regalias: Slave Scan FSM:  Read alias register 
                    ec_fsm_slave_scan_enter_preop: enter change AL state to PREOP
                    ec_fsm_slave_scan_state_preop: Slave Scan FSM:  Do slave scan when in PREOP
                    ec_fsm_slave_scan_state_sync: Slave Scan FSM:  Read the SyncM config info
                    ec_fsm_slave_scan_enter_pdos: enter Scan PDO config 
                    ec_fsm_slave_scan_state_pdos: Slave Scan FSM: state of scan PDO config
                    ec_fsm_slave_scan_state_error:Slave Scan FSM: slave scan error flag
                    ec_fsm_slave_scan_state_end: Slave Scan FSM: slave scan success flag

History:

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/

/******************************************************************************
 *
 *  $Id: fsm_slave_scan.c,v 1.10 2015/03/03 05:51:05 kidd Exp $
 *
 *  Copyright (C) 2006-2008  Florian Pose, Ingenieurgemeinschaft IgH
 *
 *  This file is part of the IgH EtherCAT Master.
 *
 *  The IgH EtherCAT Master is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License version 2, as
 *  published by the Free Software Foundation.
 *
 *  The IgH EtherCAT Master is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 *  Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with the IgH EtherCAT Master; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *  ---
 *
 *  The license mentioned above concerns the source code only. Using the
 *  EtherCAT technology and brand is only permitted in compliance with the
 *  industrial property and similar rights of Beckhoff Automation GmbH.
 *
 *****************************************************************************/

/**
   \file
   EtherCAT slave state machines.
*/

/*****************************************************************************/
#include "ecrt.h"
#include "config.h"
#include "debug.h"
#include <string.h>

#include "globals.h"
#include "skbuff.h"
#include "master.h"
#include "mailbox.h"
#include "slave_config.h"
#include "fsm_slave_scan.h"
#include "Ec_mm.h"


/*****************************************************************************/

void ec_fsm_slave_scan_state_start(ec_fsm_slave_scan_t *);
void ec_fsm_slave_scan_state_address(ec_fsm_slave_scan_t *);
void ec_fsm_slave_scan_state_state(ec_fsm_slave_scan_t *);
void ec_fsm_slave_scan_state_base(ec_fsm_slave_scan_t *);
void ec_fsm_slave_scan_state_dc_cap(ec_fsm_slave_scan_t *);
void ec_fsm_slave_scan_state_dc_times(ec_fsm_slave_scan_t *);
void ec_fsm_slave_scan_state_datalink(ec_fsm_slave_scan_t *);
void ec_fsm_slave_scan_state_sii_size(ec_fsm_slave_scan_t *);
void ec_fsm_slave_scan_state_sii_data(ec_fsm_slave_scan_t *);
#ifdef EC_REGALIAS
void ec_fsm_slave_scan_state_regalias(ec_fsm_slave_scan_t *);
#endif
void ec_fsm_slave_scan_state_preop(ec_fsm_slave_scan_t *);
void ec_fsm_slave_scan_state_sync(ec_fsm_slave_scan_t *);
void ec_fsm_slave_scan_state_pdos(ec_fsm_slave_scan_t *);

void ec_fsm_slave_scan_state_end(ec_fsm_slave_scan_t *);
void ec_fsm_slave_scan_state_error(ec_fsm_slave_scan_t *);

void ec_fsm_slave_scan_enter_datalink(ec_fsm_slave_scan_t *);
#ifdef EC_REGALIAS
void ec_fsm_slave_scan_enter_regalias(ec_fsm_slave_scan_t *);
#endif
void ec_fsm_slave_scan_enter_preop(ec_fsm_slave_scan_t *);
void ec_fsm_slave_scan_enter_pdos(ec_fsm_slave_scan_t *);

/*********gonggw, 2013.10.18*****/
#ifdef EC_DEBUG_ADD_FSM_SLAVE_SCAN
static int kidd_cnt;
void ec_fsm_slave_scan_state_kidd_add(ec_fsm_slave_scan_t *);
#endif
/*****************************************************************************/

/** Constructor.
 */
void ec_fsm_slave_scan_init(
        ec_fsm_slave_scan_t *fsm, /**< Slave scanning state machine. */
        ec_datagram_t *datagram, /**< Datagram to use. */
        ec_fsm_slave_config_t *fsm_slave_config, /**< Slave configuration
                                                  state machine to use. */
        ec_fsm_pdo_t *fsm_pdo /**< PDO configuration machine to use. */
        )
{
    fsm->datagram = datagram;
    fsm->fsm_slave_config = fsm_slave_config;
    fsm->fsm_pdo = fsm_pdo;

    // init sub state machines
    ec_fsm_sii_init(&fsm->fsm_sii, fsm->datagram);
}

/*****************************************************************************/

/** Destructor.
 */
void ec_fsm_slave_scan_clear(ec_fsm_slave_scan_t *fsm /**< slave state machine */)
{
    // clear sub state machines
    ec_fsm_sii_clear(&fsm->fsm_sii);
}

/*****************************************************************************/

/**
 * Start slave scan state machine.
 */

void ec_fsm_slave_scan_start(
        ec_fsm_slave_scan_t *fsm, /**< slave state machine */
        ec_slave_t *slave /**< slave to configure */
        )
{
    fsm->slave = slave;
    fsm->state = ec_fsm_slave_scan_state_start;
}

/*****************************************************************************/

/**
   \return false, if state machine has terminated
*/

int ec_fsm_slave_scan_running(const ec_fsm_slave_scan_t *fsm /**< slave state machine */)
{
    return fsm->state != ec_fsm_slave_scan_state_end
        && fsm->state != ec_fsm_slave_scan_state_error;
}

/*****************************************************************************/

/**
   Executes the current state of the state machine.
   If the state machine's datagram is not sent or received yet, the execution
   of the state machine is delayed to the next cycle.
   \return false, if state machine has terminated
*/

int ec_fsm_slave_scan_exec(ec_fsm_slave_scan_t *fsm /**< slave state machine */)
{
    if (fsm->datagram->state == EC_DATAGRAM_QUEUED
        || fsm->datagram->state == EC_DATAGRAM_SENT) {
        // datagram was not sent or received yet.
        return ec_fsm_slave_scan_running(fsm);
    }

    fsm->state(fsm);
    return ec_fsm_slave_scan_running(fsm);
}

/*****************************************************************************/

/**
   \return true, if the state machine terminated gracefully
*/

int ec_fsm_slave_scan_success(const ec_fsm_slave_scan_t *fsm /**< slave state machine */)
{
    return fsm->state == ec_fsm_slave_scan_state_end;
}

/******************************************************************************
 *  slave scan state machine
 *****************************************************************************/

/**
   Slave scan state: START.
   First state of the slave state machine. Writes the station address to the
   slave, according to its ring position.
*/

void ec_fsm_slave_scan_state_start(ec_fsm_slave_scan_t *fsm /**< slave state machine */)
{
    // write station address
    ec_datagram_apwr(fsm->datagram, fsm->slave->ring_position, 0x0010, 2);
    EC_WRITE_U16(fsm->datagram->data, fsm->slave->station_address);
    fsm->retries = EC_FSM_RETRIES;
    fsm->state = ec_fsm_slave_scan_state_address;
}

/*****************************************************************************/

/**
   Slave scan state: ADDRESS.
*/

void ec_fsm_slave_scan_state_address(
        ec_fsm_slave_scan_t *fsm /**< slave state machine */
        )
{
    ec_datagram_t *datagram = fsm->datagram;

    if (datagram->state == EC_DATAGRAM_TIMED_OUT && fsm->retries--)
        return;

    if (datagram->state != EC_DATAGRAM_RECEIVED) {
        fsm->state = ec_fsm_slave_scan_state_error;
        EC_SLAVE_ERR(fsm->slave,
                "Failed to receive station address datagram: ");
        ec_datagram_print_state(datagram);
        return;
    }

    if (datagram->working_counter != 1) {
        fsm->slave->error_flag = 1;
        fsm->state = ec_fsm_slave_scan_state_error;
        EC_SLAVE_ERR(fsm->slave, "Failed to write station address: ");
        ec_datagram_print_wc_error(datagram);
        return;
    }

    // Read AL state
    ec_datagram_fprd(datagram, fsm->slave->station_address, 0x0130, 2);
    ec_datagram_zero(datagram);
    fsm->retries = EC_FSM_RETRIES;
    fsm->state = ec_fsm_slave_scan_state_state;
}

/*****************************************************************************/

/**
   Slave scan state: STATE.
*/

void ec_fsm_slave_scan_state_state(
        ec_fsm_slave_scan_t *fsm /**< slave state machine */
        )
{
    ec_datagram_t *datagram = fsm->datagram;
    ec_slave_t *slave = fsm->slave;

    if (datagram->state == EC_DATAGRAM_TIMED_OUT && fsm->retries--)
        return;

    if (datagram->state != EC_DATAGRAM_RECEIVED) {
        fsm->state = ec_fsm_slave_scan_state_error;
        EC_SLAVE_ERR(slave, "Failed to receive AL state datagram: ");
        ec_datagram_print_state(datagram);
        return;
    }

    if (datagram->working_counter != 1) {
        fsm->slave->error_flag = 1;
        fsm->state = ec_fsm_slave_scan_state_error;
        EC_SLAVE_ERR(slave, "Failed to read AL state: ");
        ec_datagram_print_wc_error(datagram);
        return;
    }

    slave->current_state = (ec_slave_state_t)EC_READ_U8(datagram->data);
    if (slave->current_state & EC_SLAVE_STATE_ACK_ERR) {
        char state_str[EC_STATE_STRING_SIZE];
        ec_state_string(slave->current_state, state_str, 0);
        EC_SLAVE_WARN(slave, "Slave has state error bit set (%s)!\n",
                state_str);
    }

    // read base data
    ec_datagram_fprd(datagram, fsm->slave->station_address, 0x0000, 12);
    ec_datagram_zero(datagram);
    fsm->retries = EC_FSM_RETRIES;
    fsm->state = ec_fsm_slave_scan_state_base;
}

/*****************************************************************************/

/** Slave scan state: BASE.
 */
void ec_fsm_slave_scan_state_base(
        ec_fsm_slave_scan_t *fsm /**< slave state machine */
        )
{
    ec_datagram_t *datagram = fsm->datagram;
    ec_slave_t *slave = fsm->slave;
    u8 octet;
    int i;

    if (datagram->state == EC_DATAGRAM_TIMED_OUT && fsm->retries--)
        return;

    if (datagram->state != EC_DATAGRAM_RECEIVED) {
        fsm->state = ec_fsm_slave_scan_state_error;
        EC_SLAVE_ERR(slave, "Failed to receive base data datagram: ");
        ec_datagram_print_state(datagram);
        return;
    }

    if (datagram->working_counter != 1) {
        fsm->slave->error_flag = 1;
        fsm->state = ec_fsm_slave_scan_state_error;
        EC_SLAVE_ERR(slave, "Failed to read base data: ");
        ec_datagram_print_wc_error(datagram);
        return;
    }

    slave->base_type       = EC_READ_U8 (datagram->data);
    slave->base_revision   = EC_READ_U8 (datagram->data + 1);
    slave->base_build      = EC_READ_U16(datagram->data + 2);

    slave->base_fmmu_count = EC_READ_U8 (datagram->data + 4);
    if (slave->base_fmmu_count > EC_MAX_FMMUS) {
        EC_SLAVE_WARN(slave, "Slave has more FMMUs (%u) than the master can"
                " handle (%u).\n", slave->base_fmmu_count, EC_MAX_FMMUS);
        slave->base_fmmu_count = EC_MAX_FMMUS;
    }

    slave->base_sync_count = EC_READ_U8(datagram->data + 5);
    if (slave->base_sync_count > EC_MAX_SYNC_MANAGERS) {
        EC_SLAVE_WARN(slave, "Slave provides more sync managers (%u)"
                " than the master can handle (%u).\n",
                slave->base_sync_count, EC_MAX_SYNC_MANAGERS);
        slave->base_sync_count = EC_MAX_SYNC_MANAGERS;
    }

    octet = EC_READ_U8(datagram->data + 7);
    for (i = 0; i < EC_MAX_PORTS; i++) {
        slave->ports[i].desc = (ec_slave_port_desc_t)((octet >> (2 * i)) & 0x03);
    }

    octet = EC_READ_U8(datagram->data + 8);
    slave->base_fmmu_bit_operation = octet & 0x01;
    slave->base_dc_supported = (octet >> 2) & 0x01;
    slave->base_dc_range = ((octet >> 3) & 0x01) ? EC_DC_64 : EC_DC_32;

    if (slave->base_dc_supported) {
        // read DC capabilities
        ec_datagram_fprd(datagram, slave->station_address, 0x0910,
                slave->base_dc_range == EC_DC_64 ? 8 : 4);
        ec_datagram_zero(datagram);
        fsm->retries = EC_FSM_RETRIES;
        fsm->state = ec_fsm_slave_scan_state_dc_cap;
    } else {
        ec_fsm_slave_scan_enter_datalink(fsm);
    }
}

/*****************************************************************************/

/**
   Slave scan state: DC CAPABILITIES.
*/

void ec_fsm_slave_scan_state_dc_cap(
        ec_fsm_slave_scan_t *fsm /**< slave state machine */
        )
{
    ec_datagram_t *datagram = fsm->datagram;
    ec_slave_t *slave = fsm->slave;

    if (datagram->state == EC_DATAGRAM_TIMED_OUT && fsm->retries--)
        return;

    if (datagram->state != EC_DATAGRAM_RECEIVED) {
        fsm->state = ec_fsm_slave_scan_state_error;
        EC_SLAVE_ERR(slave, "Failed to receive system time datagram: ");
        ec_datagram_print_state(datagram);
        return;
    }

    if (datagram->working_counter == 1) {
        slave->has_dc_system_time = 1;
        EC_SLAVE_DBG(slave, 1, "Slave has the System Time register.\n");
    } else if (datagram->working_counter == 0) {
        EC_SLAVE_DBG(slave, 1, "Slave has no System Time register; delay "
                "measurement only.\n");
    } else {
        fsm->slave->error_flag = 1;
        fsm->state = ec_fsm_slave_scan_state_error;
        EC_SLAVE_ERR(slave, "Failed to determine, if system time register is "
                "supported: ");
        ec_datagram_print_wc_error(datagram);
        return;
    }

    // read DC port receive times
    ec_datagram_fprd(datagram, slave->station_address, 0x0900, 16);
    ec_datagram_zero(datagram);
    fsm->retries = EC_FSM_RETRIES;
    fsm->state = ec_fsm_slave_scan_state_dc_times;
}

/*****************************************************************************/

/**
   Slave scan state: DC TIMES.
*/

void ec_fsm_slave_scan_state_dc_times(
        ec_fsm_slave_scan_t *fsm /**< slave state machine */
        )
{
    ec_datagram_t *datagram = fsm->datagram;
    ec_slave_t *slave = fsm->slave;
    int i;

    if (datagram->state == EC_DATAGRAM_TIMED_OUT && fsm->retries--)
        return;

    if (datagram->state != EC_DATAGRAM_RECEIVED) {
        fsm->state = ec_fsm_slave_scan_state_error;
        EC_SLAVE_ERR(slave, "Failed to receive system time datagram: ");
        ec_datagram_print_state(datagram);
        return;
    }

    if (datagram->working_counter != 1) {
        fsm->slave->error_flag = 1;
        fsm->state = ec_fsm_slave_scan_state_error;
        EC_SLAVE_ERR(slave, "Failed to get DC receive times: ");
        ec_datagram_print_wc_error(datagram);
        return;
    }

    for (i = 0; i < EC_MAX_PORTS; i++) {
        slave->ports[i].receive_time = EC_READ_U32(datagram->data + 4 * i);
    }

    ec_fsm_slave_scan_enter_datalink(fsm);
}

/*****************************************************************************/

/**
   Slave scan entry function: DATALINK.
*/

void ec_fsm_slave_scan_enter_datalink(
        ec_fsm_slave_scan_t *fsm /**< slave state machine */
        )
{
    ec_datagram_t *datagram = fsm->datagram;
    ec_slave_t *slave = fsm->slave;

    // read data link status
    ec_datagram_fprd(datagram, slave->station_address, 0x0110, 2);
    ec_datagram_zero(datagram);
    fsm->retries = EC_FSM_RETRIES;
    fsm->state = ec_fsm_slave_scan_state_datalink;
}

/*****************************************************************************/
#ifndef ALL_EEPROM_ACC
uint16_t rxpdo_off, rxpdo_size;
uint16_t txpdo_off, txpdo_size;
uint16_t syncm_off, syncm_size;
uint16_t sii_state = 0; //state for accessing SII data
#endif

/**
   Slave scan state: DATALINK.
*/

void ec_fsm_slave_scan_state_datalink(ec_fsm_slave_scan_t *fsm /**< slave state machine */)
{
    ec_datagram_t *datagram = fsm->datagram;
    ec_slave_t *slave = fsm->slave;
    uint16_t dl_status;
    unsigned int i;

    if (datagram->state == EC_DATAGRAM_TIMED_OUT && fsm->retries--)
        return;

    if (datagram->state != EC_DATAGRAM_RECEIVED) {
        fsm->state = ec_fsm_slave_scan_state_error;
        EC_SLAVE_ERR(slave, "Failed to receive DL status datagram: ");
        ec_datagram_print_state(datagram);
        return;
    }

    if (datagram->working_counter != 1) {
        fsm->slave->error_flag = 1;
        fsm->state = ec_fsm_slave_scan_state_error;
        EC_SLAVE_ERR(slave, "Failed to read DL status: ");
        ec_datagram_print_wc_error(datagram);
        return;
    }

    dl_status = EC_READ_U16(datagram->data);
    for (i = 0; i < EC_MAX_PORTS; i++) {
        slave->ports[i].link.link_up =
            dl_status & (1 << (4 + i)) ? 1 : 0;
        slave->ports[i].link.loop_closed =
            dl_status & (1 << (8 + i * 2)) ? 1 : 0;
        slave->ports[i].link.signal_detected =
            dl_status & (1 << (9 + i * 2)) ? 1 : 0;
    }

    // Start fetching SII size

#ifdef EC_DEBUG_ADD_FSM_SLAVE_SCAN
	fsm->state = ec_fsm_slave_scan_state_kidd_add;
	kidd_cnt = 0;
#else
#ifndef ALL_EEPROM_ACC
		rxpdo_off	= 0;
		rxpdo_size	= 0;
		txpdo_off	= 0;
		txpdo_size	= 0;
		sii_state	= 0;
#endif

    fsm->state = ec_fsm_slave_scan_state_sii_size;
	fsm->sii_offset = EC_FIRST_SII_CATEGORY_OFFSET; // first category header
	ec_fsm_sii_read(&fsm->fsm_sii, slave, fsm->sii_offset,
		EC_FSM_SII_USE_CONFIGURED_ADDRESS);
#endif
    fsm->state(fsm); // execute state immediately
}
/************gonggw, 2013.10.18, add extra state to send more datagarams****************/
#ifdef EC_DEBUG_ADD_FSM_SLAVE_SCAN
void ec_fsm_slave_scan_state_kidd_add(ec_fsm_slave_scan_t *fsm /**< slave state machine */)
{
    ec_datagram_t *datagram = fsm->datagram;
    ec_slave_t *slave = fsm->slave;
    uint16_t dl_status;
    unsigned int i;
	static uint16_t eeprom_addr = 0xFFFF;
	uint16_t eeprom_data;
	_DBG_PRINTF_("entering ec_fsm_slave_scan_state_kidd_add, kidd_cnt=%d\n", kidd_cnt);
	if(kidd_cnt == 0)
	{
		eeprom_addr = 0x001C;
		eeprom_data = 0x0;
	    ec_datagram_fpwr(datagram, slave->station_address, 0x0502, 4);
		EC_WRITE_U16(datagram->data, 0x0180);
		EC_WRITE_U16(datagram->data+2,eeprom_addr);
    	fsm->retries = EC_FSM_RETRIES;
	}
	else if(kidd_cnt == 1)
	{
		int i;
		uint16_t pdi_con_reg = 0xffff;
	    ec_datagram_fprd(datagram, slave->station_address, 0x0502, 10);
    	ec_datagram_zero(datagram);
    	fsm->retries = EC_FSM_RETRIES;
	}
	else if(kidd_cnt == 2)
	{
		uint16_t sii_data[5];
		_DBG_PRINTF_("Original EEPROM[0]:");
		for(i = 0; i < 5; i++)
		{
			sii_data[i] = EC_READ_U16(datagram->data + (i*2));
			_DBG_PRINTF_("sii_data[%d]=%04x ", i, sii_data[i]);
		}
		_DBG_PRINTF_("\n");

		if (slave->station_address == 0x0002)
		{
			sii_data[3] = eeprom_data;
			sii_data[0] = 0x0281;
			_DBG_PRINTF_("data will be sent: sii_data[0]=%04x, sii_data[3]=%04x", \
				sii_data[0], sii_data[3]);
	    	ec_datagram_fpwr(datagram, slave->station_address, 0x0502, 8);
			EC_WRITE_U16(datagram->data, sii_data[0]); //EEPROM control register
			EC_WRITE_U16(datagram->data+2,eeprom_addr); //EEPROM address, low 16 bits
			EC_WRITE_U16(datagram->data+4,0x0000);		//EEPROM address, high 16 bits
			EC_WRITE_U16(datagram->data+6,sii_data[3]);	//data to be written to EEPROM @ address eeprom_addr
    		fsm->retries = EC_FSM_RETRIES;
		}
	}
	else if (kidd_cnt == 3)
	{
	    ec_datagram_fprd(datagram, slave->station_address, 0x0502, 10);
    	ec_datagram_zero(datagram);
    	fsm->retries = EC_FSM_RETRIES;
	}
	else if(kidd_cnt == 4)
	{
		uint16_t sii_data[5];
		int i;
		_DBG_PRINTF_("EEPROM status after write:");
		for(i = 0; i < 5; i++)
		{
			sii_data[i] = EC_READ_U16(datagram->data + (i*2));
			_DBG_PRINTF_("sii_data[%d]=%04x ", i, sii_data[i]);
		}
		udelay(1000);
	    ec_datagram_fpwr(datagram, slave->station_address, 0x0502, 4);
		EC_WRITE_U16(datagram->data, 0x0180);
		EC_WRITE_U16(datagram->data+2,eeprom_addr);
    	fsm->retries = EC_FSM_RETRIES;
	}
	else if(kidd_cnt == 5)
	{
		int i;
		uint16_t pdi_con_reg = 0xffff;
	    ec_datagram_fprd(datagram, slave->station_address, 0x0502, 10);
    	ec_datagram_zero(datagram);
    	fsm->retries = EC_FSM_RETRIES;
	}
	else if(kidd_cnt == 6)
	{
		uint16_t sii_data[5];
		_DBG_PRINTF_("New EEPROM[0]:");
		for(i = 0; i < 5; i++)
		{
			sii_data[i] = EC_READ_U16(datagram->data + (i*2));
			_DBG_PRINTF_("sii_data[%d]=%04x ", i, sii_data[i]);		
		}
		_DBG_PRINTF_("\n");
	}
	else
	{
		fsm->state = ec_fsm_slave_scan_state_sii_size;
		fsm->sii_offset = EC_FIRST_SII_CATEGORY_OFFSET; // first category header
		ec_fsm_sii_read(&fsm->fsm_sii, slave, fsm->sii_offset,
		EC_FSM_SII_USE_CONFIGURED_ADDRESS);
		fsm->state(fsm); // execute state immediately
	}
	kidd_cnt++;
}

#endif
/*****************************************************************************/

/**
   Slave scan state: SII SIZE.
*/
#ifndef DYN_MEM_ALLOC
#define SII_DATA_LEN	1800
uint8_t sii_data_buf[SII_DATA_LEN] __aligned(4);
#endif

void ec_fsm_slave_scan_state_sii_size(ec_fsm_slave_scan_t *fsm /**< slave state machine */)
{
    ec_slave_t *slave = fsm->slave;
    uint16_t cat_type, cat_size;

    if (ec_fsm_sii_exec(&fsm->fsm_sii))
        return;

    if (!ec_fsm_sii_success(&fsm->fsm_sii)) {
        fsm->slave->error_flag = 1;
        fsm->state = ec_fsm_slave_scan_state_error;
        EC_SLAVE_ERR(slave, "Failed to determine SII content size:"
                " Reading word offset 0x%04x failed. Assuming %u words.\n",
                fsm->sii_offset, EC_FIRST_SII_CATEGORY_OFFSET);
        slave->sii_nwords = EC_FIRST_SII_CATEGORY_OFFSET;
        goto alloc_sii;
    }

    cat_type = EC_READ_U16(fsm->fsm_sii.value);
#if 0
    cat_size = EC_READ_U16(fsm->fsm_sii.value + 2);
#else
	cat_size = EC_READ_U16(fsm->fsm_sii.value + 1);	//fsm->fsm_sii.value is 16-bit pointer, plus 1 meaning 2 bytes
#endif
#ifndef ALL_EEPROM_ACC
	if (cat_type == SII_CAT_TXPDO)
	{
		txpdo_off = fsm->sii_offset;
		txpdo_size = cat_size;
	}
	else if (cat_type == SII_CAT_RXPDO)
	{
		rxpdo_off = fsm->sii_offset;
		rxpdo_size = cat_size;
	}
	else if (cat_type == SII_CAT_SYNCM)
	{
		syncm_off = fsm->sii_offset;
		syncm_size = cat_size;

	}
#endif
    if (cat_type != 0xFFFF) { // not the last category
        off_t next_offset = 2UL + (long)(fsm->sii_offset) + (long)cat_size;
        if (next_offset >= EC_MAX_SII_SIZE) {
            EC_SLAVE_WARN(slave, "SII size exceeds %u words"
                    " (0xffff limiter missing?).\n", EC_MAX_SII_SIZE);
            // cut off category data...
            slave->sii_nwords = EC_FIRST_SII_CATEGORY_OFFSET;
            goto alloc_sii;
        }
        fsm->sii_offset = next_offset;
        ec_fsm_sii_read(&fsm->fsm_sii, slave, fsm->sii_offset,
                        EC_FSM_SII_USE_CONFIGURED_ADDRESS);
        ec_fsm_sii_exec(&fsm->fsm_sii); // execute state immediately
        return;
    }

    slave->sii_nwords = fsm->sii_offset + 1;
#if 0
	_DBG_PRINTF_("%d in %s, sii_nwords=%d\n", __LINE__, __FILE__, slave->sii_nwords);
#endif
alloc_sii:
#ifdef DYN_MEM_ALLOC
    if (slave->sii_words) {
        EC_SLAVE_WARN(slave, "Freeing old SII data...\n");
        ec_free(slave->sii_words);
    }
#ifdef EC_DYN_MEM_PRT
	_DBG_PRINTF_("%d in %s, ec_malloc size=%d\n", __LINE__, __FILE__, slave->sii_nwords * 2);
#endif
    if (!(slave->sii_words =
                (uint16_t *) ec_malloc(slave->sii_nwords * 2, 0))) {
        EC_SLAVE_ERR(slave, "Failed to allocate %zu words of SII data.\n",
               slave->sii_nwords);
        slave->sii_nwords = 0;
        slave->error_flag = 1;
        fsm->state = ec_fsm_slave_scan_state_error;
        return;
    }
#else
	slave->sii_words =	(uint16_t *)sii_data_buf;
#endif
    // Start fetching SII contents

    fsm->state = ec_fsm_slave_scan_state_sii_data;
    fsm->sii_offset = 0x0000;
    ec_fsm_sii_read(&fsm->fsm_sii, slave, fsm->sii_offset,
            EC_FSM_SII_USE_CONFIGURED_ADDRESS);
    ec_fsm_sii_exec(&fsm->fsm_sii); // execute state immediately
}

/*****************************************************************************/

/**
   Slave scan state: SII DATA.
*/

void ec_fsm_slave_scan_state_sii_data(ec_fsm_slave_scan_t *fsm /**< slave state machine */)
{
    ec_slave_t *slave = fsm->slave;
    uint16_t *cat_word, cat_type, cat_size;

    if (ec_fsm_sii_exec(&fsm->fsm_sii)) return;

    if (!ec_fsm_sii_success(&fsm->fsm_sii)) {
        fsm->slave->error_flag = 1;
        fsm->state = ec_fsm_slave_scan_state_error;
        EC_SLAVE_ERR(slave, "Failed to fetch SII contents.\n");
        return;
    }

    // 2 words fetched

    if (fsm->sii_offset + 2 <= slave->sii_nwords) { // 2 words fit
#ifdef CONFIG_USE_LIB_MEMCPY
        memcpy(slave->sii_words + fsm->sii_offset, fsm->fsm_sii.value, 4);
#else
		ec_memcpy(slave->sii_words + fsm->sii_offset, fsm->fsm_sii.value, 4);
#endif
    } else { // copy the last word
#ifdef CONFIG_USE_LIB_MEMCPY
        memcpy(slave->sii_words + fsm->sii_offset, fsm->fsm_sii.value, 2);
#else
		ec_memcpy(slave->sii_words + fsm->sii_offset, fsm->fsm_sii.value, 2);
#endif
    }

#ifdef ALL_EEPROM_ACC
    if (fsm->sii_offset + 2 < slave->sii_nwords) {
        // fetch the next 2 words
        fsm->sii_offset += 2;
        ec_fsm_sii_read(&fsm->fsm_sii, slave, fsm->sii_offset,
                        EC_FSM_SII_USE_CONFIGURED_ADDRESS);
        ec_fsm_sii_exec(&fsm->fsm_sii); // execute state immediately
        return;
    }
#else
	switch (sii_state)
	{
		case 0:	//STATE 0: fetch data from 0x0 to 0x20 in EEPROM
			if (fsm->sii_offset < 0x20)
			{
				fsm->sii_offset += 2;
			}
			else
			{
				fsm->sii_offset = txpdo_off;
				sii_state = 1;
			}
			break;
		case 1:
			if (fsm->sii_offset < (txpdo_off + txpdo_size))
			{
				fsm->sii_offset += 2;
			}
			else
			{
				fsm->sii_offset = rxpdo_off;
				sii_state = 2;
			}
			break;
		case 2:
			if (fsm->sii_offset < (rxpdo_off + rxpdo_size))
			{
				fsm->sii_offset += 2;
			}
			else
			{
				fsm->sii_offset = syncm_off;
				sii_state = 3;
			}
			break;
		case 3:
			if (fsm->sii_offset < (syncm_off + syncm_size))
			{
				fsm->sii_offset += 2;
			}
			else
			{
				sii_state = 4;
			}
			break;
		default:
			break;
	}
	if (sii_state < 4)
	{
		ec_fsm_sii_read(&fsm->fsm_sii, slave, fsm->sii_offset,
					EC_FSM_SII_USE_CONFIGURED_ADDRESS);
		ec_fsm_sii_exec(&fsm->fsm_sii); // execute state immediately
		return;
	}
#endif

#if 0
	{
		int i;
		_DBG_PRINTF_("DATA in EEPROM:\n");
		for(i = 0; i < slave->sii_nwords; i+=8)
			_DBG_PRINTF_("%d: %04x %04x %04x %04x %04x %04x %04x %04x\n", i, \
				slave->sii_words[i], slave->sii_words[i+1], slave->sii_words[i+2], slave->sii_words[i+3], \
				slave->sii_words[i+4], slave->sii_words[i+5], slave->sii_words[i+6], slave->sii_words[i+7]);
	}
#endif
    // Evaluate SII contents

    ec_slave_clear_sync_managers(slave);

    slave->sii.alias =
        EC_READ_U16(slave->sii_words + 0x0004);
    slave->effective_alias = slave->sii.alias;
    slave->sii.vendor_id =
        EC_READ_U32(slave->sii_words + 0x0008);
    slave->sii.product_code =
        EC_READ_U32(slave->sii_words + 0x000A);
    slave->sii.revision_number =
        EC_READ_U32(slave->sii_words + 0x000C);
    slave->sii.serial_number =
        EC_READ_U32(slave->sii_words + 0x000E);
    slave->sii.boot_rx_mailbox_offset =
        EC_READ_U16(slave->sii_words + 0x0014);
    slave->sii.boot_rx_mailbox_size =
        EC_READ_U16(slave->sii_words + 0x0015);
    slave->sii.boot_tx_mailbox_offset =
        EC_READ_U16(slave->sii_words + 0x0016);
    slave->sii.boot_tx_mailbox_size =
        EC_READ_U16(slave->sii_words + 0x0017);
    slave->sii.std_rx_mailbox_offset =
        EC_READ_U16(slave->sii_words + 0x0018);
    slave->sii.std_rx_mailbox_size =
        EC_READ_U16(slave->sii_words + 0x0019);
    slave->sii.std_tx_mailbox_offset =
        EC_READ_U16(slave->sii_words + 0x001A);
    slave->sii.std_tx_mailbox_size =
        EC_READ_U16(slave->sii_words + 0x001B);
    slave->sii.mailbox_protocols =
        EC_READ_U16(slave->sii_words + 0x001C);

    if (slave->sii_nwords == EC_FIRST_SII_CATEGORY_OFFSET) {
        // sii does not contain category data
        fsm->state = ec_fsm_slave_scan_state_end;
        return;
    }

    if (slave->sii_nwords < EC_FIRST_SII_CATEGORY_OFFSET + 1) {
        EC_SLAVE_ERR(slave, "Unexpected end of SII data:"
                " First category header missing.\n");
        goto end;
    }

    // evaluate category data
#ifdef ALL_EEPROM_ACC
    cat_word = slave->sii_words + EC_FIRST_SII_CATEGORY_OFFSET;
    while (EC_READ_U16(cat_word) != 0xFFFF) {

        // type and size words must fit
        if (cat_word + 2 - slave->sii_words > slave->sii_nwords) {
            EC_SLAVE_ERR(slave, "Unexpected end of SII data:"
                    " Category header incomplete.\n");
            goto end;
        }

        cat_type = EC_READ_U16(cat_word) & 0x7FFF;
        cat_size = EC_READ_U16(cat_word + 1);
        cat_word += 2;

        if (cat_word + cat_size - slave->sii_words > slave->sii_nwords) {
            EC_SLAVE_WARN(slave, "Unexpected end of SII data:"
                    " Category data incomplete.\n");
            goto end;
        }

        switch (cat_type) {
            case SII_CAT_STRINGS:	//STRINGS
                if (ec_slave_fetch_sii_strings(slave, (uint8_t *) cat_word,
                            cat_size * 2))
                    goto end;
                break;
            case SII_CAT_GENERAL:	//General
                if (ec_slave_fetch_sii_general(slave, (uint8_t *) cat_word,
                            cat_size * 2))
                    goto end;
                break;
            case SII_CAT_FMMU:	//FMMU
                break;
            case SII_CAT_SYNCM:	//SyncM
                if (ec_slave_fetch_sii_syncs(slave, (uint8_t *) cat_word,
                            cat_size * 2))
                    goto end;
                break;
            case SII_CAT_TXPDO:	//TXPDO
                if (ec_slave_fetch_sii_pdos( slave, (uint8_t *) cat_word,
                            cat_size * 2, EC_DIR_INPUT))
                    goto end;
                break;
            case SII_CAT_RXPDO:	//RXPDO
                if (ec_slave_fetch_sii_pdos( slave, (uint8_t *) cat_word,
                            cat_size * 2, EC_DIR_OUTPUT))
                    goto end;
                break;
			//gonggw, 2013.10.17, add CAT type of 0x3C, defined in ECAT protocol(ET1000_6) APP level protocol
			case SII_CAT_DC:	//DC
				dev_info(0, "Distributed Clock for future use\n");
                break;
			default:
                EC_SLAVE_DBG(slave, 1, "Unknown category type 0x%04X.\n",
                        cat_type);
        }

        cat_word += cat_size;
        if (cat_word - slave->sii_words >= slave->sii_nwords) {
            EC_SLAVE_WARN(slave, "Unexpected end of SII data:"
                    " Next category header missing.\n");
            goto end;
        }
    }
#else	//else of "#ifdef ALL_EEPROM_ACC"
	if (syncm_size != 0)
	{
		cat_word = (uint16_t *)(slave->sii_words + syncm_off + 2);
		if (ec_slave_fetch_sii_syncs(slave, (uint8_t *) cat_word,
					syncm_size * 2))
			goto end;
	}

	if (txpdo_size != 0)
	{
		cat_word = (uint16_t *)(slave->sii_words + txpdo_off+ 2);
		if (ec_slave_fetch_sii_pdos( slave, (uint8_t *) cat_word, txpdo_size * 2, EC_DIR_INPUT))
                    goto end;
	}
	if (rxpdo_size != 0)
	{
		cat_word = (uint16_t *)(slave->sii_words + rxpdo_off+ 2);
		if (ec_slave_fetch_sii_pdos( slave, (uint8_t *) cat_word, rxpdo_size * 2, EC_DIR_OUTPUT))
                    goto end;
	}

#endif


#ifdef EC_REGALIAS
    ec_fsm_slave_scan_enter_regalias(fsm);
#else
    if (slave->sii.mailbox_protocols & EC_MBOX_COE) {
        ec_fsm_slave_scan_enter_preop(fsm);
    } else {
        fsm->state = ec_fsm_slave_scan_state_end;
    }
#endif
    return;

end:
    EC_SLAVE_ERR(slave, "Failed to analyze category data.\n");
    fsm->slave->error_flag = 1;
    fsm->state = ec_fsm_slave_scan_state_error;
}

/*****************************************************************************/

#ifdef EC_REGALIAS

/** Slave scan entry function: REGALIAS.
 */
void ec_fsm_slave_scan_enter_regalias(
        ec_fsm_slave_scan_t *fsm /**< slave state machine */
        )
{
    ec_datagram_t *datagram = fsm->datagram;
    ec_slave_t *slave = fsm->slave;

    // read alias from register
    EC_SLAVE_DBG(slave, 1, "Reading alias from register.\n");
    ec_datagram_fprd(datagram, slave->station_address, 0x0012, 2);
    ec_datagram_zero(datagram);
    fsm->retries = EC_FSM_RETRIES;
    fsm->state = ec_fsm_slave_scan_state_regalias;
}

/*****************************************************************************/

/** Slave scan state: REGALIAS.
 */
void ec_fsm_slave_scan_state_regalias(
        ec_fsm_slave_scan_t *fsm /**< slave state machine */
        )
{
    ec_datagram_t *datagram = fsm->datagram;
    ec_slave_t *slave = fsm->slave;

    if (datagram->state == EC_DATAGRAM_TIMED_OUT && fsm->retries--)
        return;

    if (datagram->state != EC_DATAGRAM_RECEIVED) {
        fsm->state = ec_fsm_slave_scan_state_error;
        EC_SLAVE_ERR(slave, "Failed to receive register alias datagram: ");
        ec_datagram_print_state(datagram);
        return;
    }

    if (datagram->working_counter != 1) {
        EC_SLAVE_DBG(slave, 1, "Failed to read register alias.\n");
    } else {
        slave->effective_alias = EC_READ_U16(datagram->data);
        EC_SLAVE_DBG(slave, 1, "Read alias %u from register.\n",
                slave->effective_alias);
    }

    if (slave->sii.mailbox_protocols & EC_MBOX_COE) {
        ec_fsm_slave_scan_enter_preop(fsm);
    } else {
        fsm->state = ec_fsm_slave_scan_state_end;
    }
}

#endif // defined EC_REGALIAS

/*****************************************************************************/

/** Enter slave scan state PREOP.
 */
void ec_fsm_slave_scan_enter_preop(
        ec_fsm_slave_scan_t *fsm /**< slave state machine */
        )
{
    ec_slave_t *slave = fsm->slave;
    uint8_t current_state = slave->current_state & EC_SLAVE_STATE_MASK;

    if (current_state != EC_SLAVE_STATE_PREOP
            && current_state != EC_SLAVE_STATE_SAFEOP
            && current_state != EC_SLAVE_STATE_OP) {
        if (slave->master->debug_level > 2) {
            char str[EC_STATE_STRING_SIZE];
            ec_state_string(current_state, str, 0);
            EC_SLAVE_DBG(slave, 0, "Slave is not in the state"
                    " to do mailbox com (%s), setting to PREOP.\n", str);
        }

        fsm->state = ec_fsm_slave_scan_state_preop;
        ec_slave_request_state(slave, EC_SLAVE_STATE_PREOP);
        ec_fsm_slave_config_start(fsm->fsm_slave_config, slave);
        ec_fsm_slave_config_exec(fsm->fsm_slave_config);
    } else {
        EC_SLAVE_DBG(slave, 1, "Reading mailbox"
                " sync manager configuration.\n");

        /* Scan current sync manager configuration to get configured mailbox
         * sizes. */
        ec_datagram_fprd(fsm->datagram, slave->station_address, 0x0800,
                EC_SYNC_PAGE_SIZE * 2);
        fsm->retries = EC_FSM_RETRIES;
        fsm->state = ec_fsm_slave_scan_state_sync;
    }
}

/*****************************************************************************/

/** Slave scan state: PREOP.
 */
void ec_fsm_slave_scan_state_preop(
        ec_fsm_slave_scan_t *fsm /**< slave state machine */
        )
{
    if (ec_fsm_slave_config_exec(fsm->fsm_slave_config))
        return;

    if (!ec_fsm_slave_config_success(fsm->fsm_slave_config)) {
        fsm->state = ec_fsm_slave_scan_state_error;
        return;
    }

    ec_fsm_slave_scan_enter_pdos(fsm);
}

/*****************************************************************************/

/** Slave scan state: SYNC.
 */
void ec_fsm_slave_scan_state_sync(
        ec_fsm_slave_scan_t *fsm /**< slave state machine */
        )
{
    ec_datagram_t *datagram = fsm->datagram;
    ec_slave_t *slave = fsm->slave;

    if (datagram->state == EC_DATAGRAM_TIMED_OUT && fsm->retries--)
        return;

    if (datagram->state != EC_DATAGRAM_RECEIVED) {
        fsm->state = ec_fsm_slave_scan_state_error;
        EC_SLAVE_ERR(slave, "Failed to receive sync manager"
                " configuration datagram: ");
        ec_datagram_print_state(datagram);
        return;
    }

    if (datagram->working_counter != 1) {
        fsm->slave->error_flag = 1;
        fsm->state = ec_fsm_slave_scan_state_error;
        EC_SLAVE_ERR(slave, "Failed to read DL status: ");
        ec_datagram_print_wc_error(datagram);
        return;
    }

    slave->configured_rx_mailbox_offset = EC_READ_U16(datagram->data);
    slave->configured_rx_mailbox_size = EC_READ_U16(datagram->data + 2);
    slave->configured_tx_mailbox_offset = EC_READ_U16(datagram->data + 8);
    slave->configured_tx_mailbox_size = EC_READ_U16(datagram->data + 10);

    EC_SLAVE_DBG(slave, 1, "Mailbox configuration:\n");
    EC_SLAVE_DBG(slave, 1, " RX offset=0x%04x size=%u\n",
            slave->configured_rx_mailbox_offset,
            slave->configured_rx_mailbox_size);
    EC_SLAVE_DBG(slave, 1, " TX offset=0x%04x size=%u\n",
            slave->configured_tx_mailbox_offset,
            slave->configured_tx_mailbox_size);

    ec_fsm_slave_scan_enter_pdos(fsm);
}

/*****************************************************************************/

/** Enter slave scan state PDOS.
 */
void ec_fsm_slave_scan_enter_pdos(
        ec_fsm_slave_scan_t *fsm /**< slave state machine */
        )
{
    ec_slave_t *slave = fsm->slave;

    EC_SLAVE_DBG(slave, 1, "Scanning PDO assignment and mapping.\n");
    fsm->state = ec_fsm_slave_scan_state_pdos;
    ec_fsm_pdo_start_reading(fsm->fsm_pdo, slave);
    ec_fsm_pdo_exec(fsm->fsm_pdo); // execute immediately
}

/*****************************************************************************/

/** Slave scan state: PDOS.
 */
void ec_fsm_slave_scan_state_pdos(
        ec_fsm_slave_scan_t *fsm /**< slave state machine */
        )
{
    if (ec_fsm_pdo_exec(fsm->fsm_pdo))
        return;

    if (!ec_fsm_pdo_success(fsm->fsm_pdo)) {
        fsm->state = ec_fsm_slave_scan_state_error;
        return;
    }

    // reading PDO configuration finished
    fsm->state = ec_fsm_slave_scan_state_end;
}

/******************************************************************************
 * Common state functions
 *****************************************************************************/

/** State: ERROR.
 */
void ec_fsm_slave_scan_state_error(
        ec_fsm_slave_scan_t *fsm /**< slave state machine */
        )
{
}

/*****************************************************************************/

/** State: END.
 */
void ec_fsm_slave_scan_state_end(
        ec_fsm_slave_scan_t *fsm /**< slave state machine */
        )
{
}

/*****************************************************************************/
