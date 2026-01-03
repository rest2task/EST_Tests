/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    device.h

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:           2014.06.06

Description:   EtherCAT driver definition

Others:

History:    

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/


/******************************************************************************
 *
 *  $Id: device.h,v 1.1 2015/02/06 05:23:04 kidd Exp $
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
   EtherCAT device structure.
*/

/*****************************************************************************/

#ifndef __EC_DEVICE_H__
#define __EC_DEVICE_H__

#include "globals.h"
#include "ecdev.h"
#ifdef EC_DEBUG_IF
#include "debug.h"
#endif

/**
 * Size of the transmit ring.
 * This memory ring is used to transmit frames. It is necessary to use
 * different memory regions, because otherwise the network device DMA could
 * send the same data twice, if it is called twice.
 */
//gonggw, 2015.01.19, moved from tcpip.h
// Ethernet network layer definitions
#define ETH_DA_OFS			0                   // Destination MAC address (48 Bit)
#define ETH_SA_OFS			6                   // Source MAC address (48 Bit)
#define ETH_TYPE_OFS		12                  // Type field (16 Bit)
#define ETH_DATA_OFS		14                  // Frame Data
#define ETH_HEADER_SIZE		14

#define FRAME_ARP			0x0806              // frame types (stored in Type/Length field)
#define FRAME_IP			0x0800
#define FRAME_ECAT			0x88A4

#define ETH_FRAG_SIZE		1536		
#define NUM_RX_DESC			1
#define NUM_TX_DESC			1

#ifdef EC_DEBUG_RING
#define EC_DEBUG_RING_SIZE 10

typedef enum {
    TX, RX
} ec_debug_frame_dir_t;

typedef struct {
    ec_debug_frame_dir_t dir;
    struct timeval t;
    unsigned int addr;
    uint8_t data[EC_MAX_DATA_SIZE];
    unsigned int data_size;
} ec_debug_frame_t;

#endif

/*****************************************************************************/

/**
   EtherCAT device.
   An EtherCAT device is a network interface card, that is owned by an
   EtherCAT master to send and receive EtherCAT frames with.
*/

struct ec_device
{
    ec_master_t *master; /**< EtherCAT master */
    struct net_device *dev; /**< pointer to the assigned net_device */
    ec_pollfunc_t poll; /**< pointer to the device's poll function */
    uint8_t open; /**< true, if the net_device has been opened */
    uint8_t link_state; /**< device link state */
    struct sk_buff tx_skb[NUM_TX_DESC]; /**< transmit skb ring */
    unsigned int tx_ring_index; /**< last ring entry used to transmit */
#ifdef EC_HAVE_CYCLES
    cycles_t cycles_poll; /**< cycles of last poll */
#endif
#ifdef EC_DEBUG_RING
    struct timeval timeval_poll;
#endif
    unsigned long jiffies_poll; /**< jiffies of last poll */

    // Frame statistics
    u64 tx_count; /**< Number of frames sent. */
    u64 last_tx_count; /**< Number of frames sent of last statistics cycle. */
    u64 rx_count; /**< Number of frames received. */
    u64 tx_bytes; /**< Number of frames sent. */
    u64 last_tx_bytes; /**< Number of bytes sent of last statistics cycle. */
    u64 tx_errors; /**< Number of transmit errors. */
    u64 last_loss; /**< Tx/Rx difference of last statistics cycle. */
    unsigned int tx_frame_rates[EC_RATE_COUNT]; /**< Transmit rates in
                                                  frames/s for different
                                                  statistics cycle periods. */
    unsigned int tx_byte_rates[EC_RATE_COUNT]; /**< Transmit rates in byte/s
                                                 for different statistics
                                                 cycle periods. */
    int loss_rates[EC_RATE_COUNT]; /**< Frame loss rates for different
                                     statistics cycle periods. */
    unsigned long stats_jiffies; /**< Jiffies of last statistic cycle. */

#ifdef EC_DEBUG_IF
    ec_debug_t dbg; /**< debug device */
#endif
#ifdef EC_DEBUG_RING
    ec_debug_frame_t debug_frames[EC_DEBUG_RING_SIZE];
    unsigned int debug_frame_index;
    unsigned int debug_frame_count;
#endif
};

/*****************************************************************************/

int ec_device_init(ec_device_t *, ec_master_t *);
void ec_device_clear(ec_device_t *);
void ec_device_attach(ec_device_t *, struct net_device *, ec_pollfunc_t);
void ec_device_detach(ec_device_t *);

int ec_device_open(ec_device_t *);
int ec_device_close(ec_device_t *);

void ec_device_poll(ec_device_t *);
uint8_t *ec_device_tx_data(ec_device_t *);
void ec_device_send(ec_device_t *, size_t);
void ec_device_clear_stats(ec_device_t *);

#ifdef EC_DEBUG_RING
void ec_device_debug_ring_append(ec_device_t *, ec_debug_frame_dir_t,
        const void *, size_t);
void ec_device_debug_ring_print(const ec_device_t *);
#endif

/*****************************************************************************/

#endif
