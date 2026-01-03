/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    device.c

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:           2014.06.06

Description:   EtherCAT driver model

Others:

Function List:  
                    ec_device_init            :  software init for EtherCAT
                    ec_device_clear          : close and free etherCAT driver
                    ec_device_attach        : attacth EtherCAT driver with net device
                    ec_device_detach       : dettacth EtherCAT driver with net device
                    ec_device_open          : open one EtherCAT device
                    ec_device_close         :  close one EtherCAT device
                    ec_device_tx_data     : select TX SKB buffer
                    ec_device_send         : call net device to send data
                    ec_device_clear_stats: clear the driver's status
                    ec_device_poll           : poll the status of net device
                    ecdev_withdraw         : release device(for compatible with original Linux driver, not used)
                    ecdev_open               : open one EtherCAT driver
                    ecdev_close               : close one EtherCAT driver
                    ecdev_receive            : use net device to receive data
                    ecdev_set_link           : generate BWR datagram
                    ecdev_get_link           : generate BRW datagram
                    
History:    

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/


/******************************************************************************
 *
 *  $Id: device.c,v 1.3 2015/02/28 02:38:13 kidd Exp $
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
   EtherCAT device methods.
*/

/*****************************************************************************/

#include "fsl_enet.h"

#include "errno.h"
#include "skbuff.h"
#include "netdevice.h"
#include "base_data_op.h"
#include <string.h>

#include "device.h"
//#include "lpc43xx_emac.h"
#include "eth_op.h"
#include "master.h"
#include "config.h"
#include "app_data.h"
#include "debug.h"
#include "Ec_mm.h"


extern enet_handle_t g_enet_handle;




#ifdef EC_DEBUG_RING
#define timersub(a, b, result) \
    do { \
        (result)->tv_sec = (a)->tv_sec - (b)->tv_sec; \
        (result)->tv_usec = (a)->tv_usec - (b)->tv_usec; \
        if ((result)->tv_usec < 0) { \
            --(result)->tv_sec; \
            (result)->tv_usec += 1000000; \
        } \
    } while (0)
#endif

/** List of intervals for frame statistics [s].
 */
static const unsigned int rate_intervals[] = {
    1, 10, 60
};

/*****************************************************************************/

/** Constructor.
 * 
 * \return 0 in case of success, else < 0
 */
extern uint8_t TX_BUF[ETH_FRAG_SIZE];

int ec_device_init(
        ec_device_t *device, /**< EtherCAT device */
        ec_master_t *master /**< master owning the device */
        )
{
    unsigned int i;
    struct ethhdr *eth;
#ifdef EC_DEBUG_IF
    char ifname[10];
    char mb = 'x';
#endif
#ifdef EC_DEBUG_RING
    device->debug_frame_index = 0;
    device->debug_frame_count = 0;
#endif

    device->master = master;
    //device->tx_ring_index = 0; //gonggw, 2015.01.27, original code
    device->tx_ring_index = NUM_TX_DESC - 1; //gonggw, 2015.01.27, first tx_ring_index to 0 when tx_ring_index++%NUM_TX_DESC
    

#ifdef EC_DEBUG_IF
    if (device == &master->main_device)
        mb = 'm';
#endif

    for (i = 0; i < NUM_TX_DESC; i++) {
        device->tx_skb[i].head = (unsigned char	*)TX_BUF;
        device->tx_skb[i].data = device->tx_skb[i].head;
        device->tx_skb[i].tail = device->tx_skb[i].head;
        device->tx_skb[i].end = device->tx_skb[i].tail + ETH_FRAG_SIZE;
        // add Ethernet-II-header
        skb_reserve(&device->tx_skb[i], ETH_HLEN);
        eth = (struct ethhdr *) skb_push(&device->tx_skb[i], ETH_HLEN);
        eth->h_proto = htons(0x88A4);
#ifdef CONFIG_USE_LIB_MEMCPY
        memset(eth->h_dest, 0xFF, ETH_ALEN);
#else
		ec_memset(eth->h_dest, 0xFF, ETH_ALEN);
#endif
    }

    ec_device_detach(device); // resets remaining fields
    return 0;

}

/*****************************************************************************/

/** Destructor.
 */
void ec_device_clear(
        ec_device_t *device /**< EtherCAT device */
        )
{

    if (device->open) 
        ec_device_close(device);
#ifdef EC_DEBUG_IF
    ec_debug_clear(&device->dbg);
#endif
}

/*****************************************************************************/

/** Associate with net_device.
 */
void ec_device_attach(
        ec_device_t *device, /**< EtherCAT device */
        struct net_device *net_dev, /**< net_device structure */
        ec_pollfunc_t poll /**< pointer to device's poll function */
        )
{
    unsigned int i;
    struct ethhdr *eth;

    ec_device_detach(device); // resets fields

    device->dev = net_dev;
    device->poll = poll;
    for (i = 0; i < NUM_TX_DESC; i++) {
        device->tx_skb[i].dev = net_dev;
        eth = (struct ethhdr *) (device->tx_skb[i].data);
#ifdef CONFIG_USE_LIB_MEMCPY
        memcpy(eth->h_source, net_dev->dev_addr, ETH_ALEN);
#else
        ec_memcpy(eth->h_source, net_dev->dev_addr, ETH_ALEN);
#endif
    }
}

/*****************************************************************************/

/** Disconnect from net_device.
 */
void ec_device_detach(
        ec_device_t *device /**< EtherCAT device */
        )
{
    unsigned int i;

#ifdef EC_DEBUG_IF
    ec_debug_unregister(&device->dbg);
#endif

    device->dev = NULL;
    device->poll = NULL;
    device->open = 0;
    device->link_state = 0; // down

    ec_device_clear_stats(device);
    for (i = 0; i < NUM_TX_DESC; i++)
        device->tx_skb[i].dev = NULL;
}

/*****************************************************************************/

/** Opens the EtherCAT device.
 *
 * \return 0 in case of success, else < 0
 */
int ec_device_open(
        ec_device_t *device /**< EtherCAT device */
        )
{
    int ret;

    if (!device->dev) {
        EC_MASTER_ERR(device->master, "No net_device to open!\n");
        return -ENODEV;
    }

    if (device->open) {
        EC_MASTER_WARN(device->master, "Device already opened!\n");
        return 0;
    }

    device->link_state = 0;

    ec_device_clear_stats(device);

    ret = device->dev->netdev_ops->ndo_open(device->dev);
    if (!ret)
        device->open = 1;

    return ret;
}

/*****************************************************************************/

/** Stops the EtherCAT device.
 *
 * \return 0 in case of success, else < 0
 */
int ec_device_close(
        ec_device_t *device /**< EtherCAT device */
        )
{
    int ret;

    if (!device->dev) {
        EC_MASTER_ERR(device->master, "No device to close!\n");
        return -ENODEV;
    }

    if (!device->open) {
        EC_MASTER_WARN(device->master, "Device already closed!\n");
        return 0;
    }

    ret = device->dev->netdev_ops->ndo_stop(device->dev);
    if (!ret)
        device->open = 0;

    return ret;
}

/*****************************************************************************/

/** Returns a pointer to the device's transmit memory.
 *
 * \return pointer to the TX socket buffer
 */

uint8_t *ec_device_tx_data(
        ec_device_t *device /**< EtherCAT device */
        )
{
    volatile enet_tx_bd_struct_t *curBuffDescrip;
    curBuffDescrip = g_enet_handle.txBdCurrent[0];
    
    uint32_t ring_idx = device->tx_ring_index;
    device->tx_ring_index++;
    if (device->tx_ring_index == NUM_TX_DESC)
        device->tx_ring_index = 0;
//    if (!(TX_DESC_STAT(device->tx_ring_index) & OWN_BIT))
//        return device->tx_skb[device->tx_ring_index].data + ETH_HLEN;
    
    if (!(curBuffDescrip->control & ENET_BUFFDESCRIPTOR_TX_READY_MASK))    //chenhao 2019.10.30
    {
         return device->tx_skb[device->tx_ring_index].data + ETH_HLEN;
    }
    else
    {
        device->tx_ring_index = ring_idx;
        return NULL;
    }
}

/*****************************************************************************/

/** Sends the content of the transmit socket buffer.
 *
 * Cuts the socket buffer content to the (now known) size, and calls the
 * start_xmit() function of the assigned net_device.
 */

//gonggw, 2014.10.16, to print datagram type

#ifdef EC_RATE_OPT	
static const char *ec_cmd[] = {
    "?",
    "APRD",
    "APWR",
    "APRW",
    "FPRD",
    "FPWR",
    "FPRW",
    "BRD",
    "BWR",
    "BRW",
    "LRD",
    "LWR",
    "LRW",
    "ARMW",
    "FRMW"
};
#endif

void ec_device_send(
        ec_device_t *device, /**< EtherCAT device */
        size_t size /**< number of bytes to send */
        )
{
    struct sk_buff *skb = &(device->tx_skb[device->tx_ring_index]);
    // frame statistics
    if (jiffies - device->stats_jiffies >= HZ) {
        unsigned int i;
        u32 tx_frame_rate =
            (u32) (device->tx_count - device->last_tx_count) * 1000;
        u32 tx_byte_rate =
            (device->tx_bytes - device->last_tx_bytes);
        u64 loss = device->tx_count - device->rx_count;
        s32 loss_rate = (s32) (loss - device->last_loss) * 1000;
        for (i = 0; i < EC_RATE_COUNT; i++) {
            unsigned int n = rate_intervals[i];
            device->tx_frame_rates[i] =
                (device->tx_frame_rates[i] * (n - 1) + tx_frame_rate) / n;
            device->tx_byte_rates[i] =
                (device->tx_byte_rates[i] * (n - 1) + tx_byte_rate) / n;
            device->loss_rates[i] =
                (device->loss_rates[i] * (n - 1) + loss_rate) / n;
        }
        device->last_tx_count = device->tx_count;
        device->last_tx_bytes = device->tx_bytes;
        device->last_loss = loss;
        device->stats_jiffies = jiffies;
    }

    // set the right length for the data
    skb->len = ETH_HLEN + size;
#ifdef EC_RATE_OPT		
    if (device->master->debug_level > 2) {
        EC_MASTER_DBG(device->master, 2, "Sending frame(%s), address=%02x%02x%02x%02x:\n", \
   		     ec_cmd[skb->data[16]], skb->data[19], skb->data[18], skb->data[21], skb->data[20]);
        ec_print_data(skb->data, ETH_HLEN + size);
    }
#endif
    /*
    printf("\n\r chen---Sending frame(%s), address=%02x%02x%02x%02x:\n\r", \
   		     ec_cmd[skb->data[16]], skb->data[19], skb->data[18], skb->data[21], skb->data[20]);
        ec_print_data(skb->data, ETH_HLEN + size);
    printf("\n\r ");
    */
    
    // start sending
    if (device->dev->netdev_ops->ndo_start_xmit(skb, device->dev) ==
            NETDEV_TX_OK)
    {
        device->tx_count++;
        device->tx_bytes += ETH_HLEN + size;
#ifdef EC_DEBUG_IF
        ec_debug_send(&device->dbg, skb->data, ETH_HLEN + size);
#endif
#ifdef EC_DEBUG_RING
        ec_device_debug_ring_append(
                device, TX, skb->data + ETH_HLEN, size);
#endif
    } else {
        device->tx_errors++;
    }
}

/*****************************************************************************/

/** Clears the frame statistics.
 */
void ec_device_clear_stats(
        ec_device_t *device /**< EtherCAT device */
        )
{
    unsigned int i;

    // zero frame statistics
    device->tx_count = 0;
    device->rx_count = 0;
    device->tx_errors = 0;
    device->tx_bytes = 0;
    device->last_tx_count = 0;
    device->last_tx_bytes = 0;
    device->last_loss = 0;
    for (i = 0; i < EC_RATE_COUNT; i++) {
        device->tx_frame_rates[i] = 0;
        device->tx_byte_rates[i] = 0;
        device->loss_rates[i] = 0;
    }
}

/*****************************************************************************/

#ifdef EC_DEBUG_RING
/** Appends frame data to the debug ring.
 */
void ec_device_debug_ring_append(
        ec_device_t *device, /**< EtherCAT device */
        ec_debug_frame_dir_t dir, /**< direction */
        const void *data, /**< frame data */
        size_t size /**< data size */
        )
{
    ec_debug_frame_t *df = &device->debug_frames[device->debug_frame_index];

    df->dir = dir;
    if (dir == TX)
        do_gettimeofday(&df->t);
    else
        df->t = device->timeval_poll;
#ifdef CONFIG_USE_LIB_MEMCPY
    memcpy(df->data, data, size);
#else
	ec_memcpy(df->data, data, size);
#endif
    df->data_size = size;

    device->debug_frame_index++;
    device->debug_frame_index %= EC_DEBUG_RING_SIZE;
    if (device->debug_frame_count < EC_DEBUG_RING_SIZE)
        device->debug_frame_count++;
}

/*****************************************************************************/

/** Outputs the debug ring.
 */
void ec_device_debug_ring_print(
        const ec_device_t *device /**< EtherCAT device */
        )
{
    int i;
    unsigned int ring_index;
    const ec_debug_frame_t *df;
    struct timeval t0, diff;

    // calculate index of the newest frame in the ring to get its time
    ring_index = (device->debug_frame_index + EC_DEBUG_RING_SIZE - 1)
        % EC_DEBUG_RING_SIZE;
    t0 = device->debug_frames[ring_index].t;

    EC_MASTER_DBG(device->master, 1, "Debug ring %u:\n", ring_index);

    // calculate index of the oldest frame in the ring
    ring_index = (device->debug_frame_index + EC_DEBUG_RING_SIZE
            - device->debug_frame_count) % EC_DEBUG_RING_SIZE;

    for (i = 0; i < device->debug_frame_count; i++) {
        df = &device->debug_frames[ring_index];
        timersub(&t0, &df->t, &diff);

        EC_MASTER_DBG(device->master, 1, "Frame %u, dt=%u.%06u s, %s:\n",
                i + 1 - device->debug_frame_count,
                (unsigned int) diff.tv_sec,
                (unsigned int) diff.tv_usec,
                (df->dir == TX) ? "TX" : "RX");
        ec_print_data(df->data, df->data_size);

        ring_index++;
        ring_index %= EC_DEBUG_RING_SIZE;
    }
}
#endif

/*****************************************************************************/

/** Calls the poll function of the assigned net_device.
 *
 * The master itself works without using interrupts. Therefore the processing
 * of received data and status changes of the network device has to be
 * done by the master calling the ISR "manually".
 */
void ec_device_poll(
        ec_device_t *device /**< EtherCAT device */
        )
{
#ifdef EC_HAVE_CYCLES
    device->cycles_poll = get_cycles();
#endif
    device->jiffies_poll = jiffies;
#ifdef EC_DEBUG_RING
    do_gettimeofday(&device->timeval_poll);
#endif
    device->poll(device->dev);
}

/******************************************************************************
 *  Device interface
 *****************************************************************************/

/** Withdraws an EtherCAT device from the master.
 *
 * The device is disconnected from the master and all device ressources
 * are freed.
 *
 * \attention Before calling this function, the ecdev_stop() function has
 *            to be called, to be sure that the master does not use the device
 *            any more.
 * \ingroup DeviceInterface
 */
void ecdev_withdraw(ec_device_t *device /**< EtherCAT device */)
{
    ec_master_t *master = device->master;
    char str[20];

    ec_mac_print(device->dev->dev_addr, str);
    EC_MASTER_INFO(master, "Releasing main device %s.\n", str);
    ec_device_detach(device);
}

/*****************************************************************************/

/** Opens the network device and makes the master enter IDLE phase.
 *
 * \return 0 on success, else < 0
 * \ingroup DeviceInterface
 */
int ecdev_open(ec_device_t *device /**< EtherCAT device */)
{
    int ret;

    ret = ec_device_open(device);

    if (ret) {
        EC_MASTER_ERR(device->master, "Failed to open device!\n");
        return ret;
    }
    ret = ec_master_enter_idle_phase(device->master);
    if (ret) {
        EC_MASTER_ERR(device->master, "Failed to enter IDLE phase!\n");
        return ret;
    }

    return 0;
}

/*****************************************************************************/

/** Makes the master leave IDLE phase and closes the network device.
 *
 * \return 0 on success, else < 0
 * \ingroup DeviceInterface
 */
void ecdev_close(ec_device_t *device /**< EtherCAT device */)
{
    ec_master_leave_idle_phase(device->master);

    if (ec_device_close(device))
        EC_MASTER_WARN(device->master, "Failed to close device!\n");
}

/*****************************************************************************/

/** Accepts a received frame.
 *
 * Forwards the received data to the master. The master will analyze the frame
 * and dispatch the received commands to the sending instances.
 * 
 * \ingroup DeviceInterface
 */
void ecdev_receive(
        ec_device_t *device, /**< EtherCAT device */
       const void *data, /**< pointer to received data */
        size_t size /**< number of bytes received */
        )
{
  //const void *ec_data = data + ETH_HLEN;			
    void *ec_data = NULL;
			ec_data = (void * )((int)data + ETH_HLEN);		
	
	
    size_t ec_size = size - ETH_HLEN;
	ecat_rec_flag = 1;

    if (!data) {
        EC_MASTER_WARN(device->master, "%s() called with NULL data.\n",
                __func__);
        return;
    }

    device->rx_count++;

#ifdef EC_RATE_OPT
    if (device->master->debug_level > 2) {
        EC_MASTER_DBG(device->master, 2, "Received frame:\n");
        ec_print_data(data, size);
    }
#endif
		
#ifdef EC_DEBUG_IF
    ec_debug_send(&device->dbg, data, size);
#endif
#ifdef EC_DEBUG_RING
    ec_device_debug_ring_append(device, RX, ec_data, ec_size);
#endif

    ec_master_receive_datagrams(device->master, ec_data, ec_size);
}

/*****************************************************************************/

/** Sets a new link state.
 *
 * If the device notifies the master about the link being down, the master
 * will not try to send frames using this device.
 * 
 * \ingroup DeviceInterface
 */
void ecdev_set_link(
        ec_device_t *device, /**< EtherCAT device */
        uint8_t state /**< new link state */
        )
{
    if (!device) {
        EC_MASTER_WARN(device->master, "ecdev_set_link(): No device!\n");
        return;
    }

    if (state != device->link_state) {
        device->link_state = state;
        EC_MASTER_INFO(device->master,
                "Link state changed to %s.\n", (state ? "UP" : "DOWN"));
    }
	
}

/*****************************************************************************/

/** Reads the link state.
 *
 * \ingroup DeviceInterface
 */
uint8_t ecdev_get_link(
        const ec_device_t *device /**< EtherCAT device */
        )
{
    if (!device) {
        EC_MASTER_WARN(device->master, "ecdev_get_link(): No device!\n");
        return 0;
    }

    return device->link_state;
}

