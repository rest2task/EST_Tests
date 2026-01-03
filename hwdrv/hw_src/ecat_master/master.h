/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    master.h

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:           2014.06.06

Description:  EtherCAT master structure and parameters definition

Others:

History:

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/

/******************************************************************************
 *
 *  $Id: master.h,v 1.2 2015/02/13 04:09:28 kidd Exp $
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
   EtherCAT master structure.
*/

/*****************************************************************************/

#ifndef __EC_MASTER_H__
#define __EC_MASTER_H__

#include "list.h"

#include "device.h"
#include "domain.h"
#include "ethernet.h"
#include "fsm_master.h"

#include "debug.h"

/*****************************************************************************/

/** Convenience macro for printing master-specific information to syslog.
 *
 * This will print the message in \a fmt with a prefixed "EtherCAT <INDEX>: ",
 * where INDEX is the master index.
 *
 * \param master EtherCAT master
 * \param fmt format string (like in printf())
 * \param args arguments (optional)
 */
#ifdef EC_DEBUG_MASTER
#define EC_MASTER_INFO(master, fmt, args...) \
	if(master->debug_level > 1)	\
	    uart_printf("EtherCAT %u: " fmt, master->index, ##args)
#else
#define EC_MASTER_INFO(master, fmt, args...)
#endif
/** Convenience macro for printing master-specific errors to syslog.
 *
 * This will print the message in \a fmt with a prefixed "EtherCAT <INDEX>: ",
 * where INDEX is the master index.
 *
 * \param master EtherCAT master
 * \param fmt format string (like in printf())
 * \param args arguments (optional)
 */

#ifdef EC_DEBUG_MASTER
#define EC_MASTER_ERR(master, fmt, args...) \
    uart_printf("EtherCAT ERROR %u: " fmt, master->index, ##args)
#else
#define EC_MASTER_ERR(master, fmt, args...)
#endif

/** Convenience macro for printing master-specific warnings to syslog.
 *
 * This will print the message in \a fmt with a prefixed "EtherCAT <INDEX>: ",
 * where INDEX is the master index.
 *
 * \param master EtherCAT master
 * \param fmt format string (like in printf())
 * \param args arguments (optional)
 */

#ifdef EC_DEBUG_MASTER
#define EC_MASTER_WARN(master, fmt, args...) \
	if (master->debug_level > 0) \
	    uart_printf("EtherCAT WARNING %u: " fmt, master->index, ##args)
#else
#define EC_MASTER_WARN(master, fmt, args...)
#endif

/** Convenience macro for printing master-specific debug messages to syslog.
 *
 * This will print the message in \a fmt with a prefixed "EtherCAT <INDEX>: ",
 * where INDEX is the master index.
 *
 * \param master EtherCAT master
 * \param fmt format string (like in printf())
 * \param args arguments (optional)
 */

#ifdef EC_DEBUG_MASTER
#define EC_MASTER_DBG(master, level, fmt, args...) \
    do { \
			if (master->debug_level > 2) \
	            uart_printf("EtherCAT DBG (%s)%u: " fmt, __func__, \
                    master->index, ##args); \
        } \
    while (0)
#else
#define EC_MASTER_DBG(master, level, fmt, args...)
#endif

/*****************************************************************************/

/** EtherCAT master phase.
 */
typedef enum {
    EC_ORPHANED, /**< Orphaned phase. The master has no Ethernet device
                   attached. */
    EC_IDLE, /**< Idle phase. An Ethernet device is attached, but the master
               is not in use, yet. */
    EC_OPERATION /**< Operation phase. The master was requested by a realtime
                   application. */
} ec_master_phase_t;

/*****************************************************************************/

/** Cyclic statistics.
 */
typedef struct {
    unsigned int timeouts; /**< datagram timeouts */
    unsigned int corrupted; /**< corrupted frames */
    unsigned int unmatched; /**< unmatched datagrams (received, but not
                               queued any longer) */
    unsigned long output_jiffies; /**< time of last output */
} ec_stats_t;

/*****************************************************************************/

/** EtherCAT master.
 *
 * Manages slaves, domains and IO.
 */
struct ec_master {
    unsigned int index; /**< Index. */
    unsigned int reserved; /**< \a True, if the master is in use. */

    ec_device_t main_device; /**< EtherCAT main device. */
    const uint8_t *main_mac; /**< MAC address of main device. */

    ec_fsm_master_t fsm; /**< Master state machine. */
    ec_datagram_t fsm_datagram; /**< Datagram used for state machines. */
    ec_mailbox_t fsm_mbox; /**< Mailbox used for state machines. */
    ec_master_phase_t phase; /**< Master phase. */
    unsigned int active; /**< Master has been activated. */
    unsigned int config_changed; /**< The configuration changed. */
    unsigned int injection_seq_fsm; /**< Datagram injection sequence number
                                      for the FSM side. */
    unsigned int injection_seq_rt; /**< Datagram injection sequence number
                                     for the realtime side. */

    ec_slave_t *slaves; /**< Array of slaves on the bus. */
    unsigned int slave_count; /**< Number of slaves on the bus. */

    /* Configuration applied by the application. */
    struct list_head configs; /**< List of slave configurations. */
    struct list_head domains; /**< List of domains. */
    
	u64 app_time; /**< Time of the last ecrt_master_sync() call. */
	u64 app_start_time; /**< Application start time. */
	u64 app_start_offset; //APP start offset after configured in ns
    u8 has_app_time; /**< Application time is valid. */
	
#if 0
    ec_datagram_t ref_sync_datagram; /**< Datagram used for synchronizing the
                                       reference clock to the master clock. */
#endif
    ec_datagram_t sync_datagram; /**< Datagram used for DC drift
                                   compensation. */
#if 0
    ec_datagram_t sync_mon_datagram; /**< Datagram used for DC synchronisation
                                       monitoring. */
#endif
    ec_slave_t *dc_ref_clock; /**< DC reference clock slave. */
#ifdef EC_HAVE_CYCLES
    cycles_t dc_cycles_app_start_time; /** cycles at last ecrt_master_sync() call.*/
#endif
    unsigned long dc_jiffies_app_start_time;/** jiffies at last
                                            ecrt_master_sync() call.*/
    unsigned int scan_busy; /**< Current scan state. */
    unsigned int allow_scan; /**< \a True, if slave scanning is allowed. */

    unsigned int config_busy; /**< State of slave configuration. */

    struct list_head datagram_queue; /**< Datagram queue. */
    uint8_t datagram_index; /**< Current datagram index. */

    struct list_head fsm_datagram_queue; /**< External Datagram queue. */

    unsigned int send_interval; /**< Interval between calls to
                                  ecrt_master_send */
    size_t max_queue_size; /**< Maximum size of datagram queue */

    unsigned int debug_level; /**< Master debug level. */
    ec_stats_t stats; /**< Cyclic statistics. */
	u32		ecat_stage; //0, for init stage; 1 for loop stage

#ifdef EC_EOE
    struct list_head eoe_handlers; /**< Ethernet over EtherCAT handlers. */
#endif

    struct list_head sii_requests; /**< SII write requests. */

    struct list_head reg_requests; /**< Register requests. */
};

/*****************************************************************************/

// static funtions
void ec_master_init_static(void);

// master creation/deletion
#if 0
int ec_master_init(ec_master_t *, unsigned int, const uint8_t *,
        const uint8_t *, dev_t, struct class *, unsigned int);
#else
int ec_master_init(ec_master_t *, unsigned int, const uint8_t *,
        const uint8_t *, unsigned int);
#endif
void ec_master_clear(ec_master_t *);

// phase transitions
int ec_master_enter_idle_phase(ec_master_t *);
void ec_master_leave_idle_phase(ec_master_t *);
int ec_master_enter_operation_phase(ec_master_t *);
void ec_master_leave_operation_phase(ec_master_t *);

// datagram IO
void ec_master_receive_datagrams(ec_master_t *, const uint8_t *, size_t);
void ec_master_queue_datagram(ec_master_t *, ec_datagram_t *);
void ec_master_queue_request_fsm_datagram(ec_master_t *, ec_datagram_t *);
void ec_master_queue_fsm_datagram(ec_master_t *, ec_datagram_t *);
void ec_master_inject_fsm_datagrams(ec_master_t *);

// misc.
void ec_master_set_send_interval(ec_master_t *, unsigned int);
void ec_master_attach_slave_configs(ec_master_t *);
ec_slave_t *ec_master_find_slave(ec_master_t *, uint16_t, uint16_t);
const ec_slave_t *ec_master_find_slave_const(const ec_master_t *, uint16_t,
        uint16_t);
void ec_master_output_stats(ec_master_t *);
#ifdef EC_EOE
void ec_master_clear_eoe_handlers(ec_master_t *);
#endif
void ec_master_clear_slaves(ec_master_t *);

unsigned int ec_master_config_count(const ec_master_t *);
ec_slave_config_t *ec_master_get_config(
        const ec_master_t *, unsigned int);
const ec_slave_config_t *ec_master_get_config_const(
        const ec_master_t *, unsigned int);
unsigned int ec_master_domain_count(const ec_master_t *);
ec_domain_t *ec_master_find_domain(ec_master_t *, unsigned int);
const ec_domain_t *ec_master_find_domain_const(const ec_master_t *,
        unsigned int);
#ifdef EC_EOE
uint16_t ec_master_eoe_handler_count(const ec_master_t *);
const ec_eoe_t *ec_master_get_eoe_handler_const(const ec_master_t *, uint16_t);
#endif

int ec_master_debug_level(ec_master_t *, unsigned int);

ec_domain_t *ecrt_master_create_domain_err(ec_master_t *);
ec_slave_config_t *ecrt_master_slave_config_err(ec_master_t *, uint16_t,
        uint16_t, uint32_t, uint32_t, uint16_t);

void ec_master_calc_dc(ec_master_t *);
void ec_master_request_op(ec_master_t *);
int wait_slaves_ready(void);


/*****************************************************************************/

#endif
