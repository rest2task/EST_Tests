/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    module.c

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:           2014.06.06

Description:  EtherCAT master main program

Others:

Function List:  
                    ec_init_module: EtherCAT module init; module contains EtherCAT master and net device                  
                    ec_cleanup_module: module clean
                    ec_master_count: Return the real number of master
                    ec_mac_equal: MAC address compare
                    ec_mac_print: print MAC address
                    ec_mac_is_zero: check if MAC address is zero
                    ec_mac_is_broadcast: check if MAC address is broadcast
                    ec_mac_parse: MAC address parsing
                    ec_print_data: data print
                    ec_print_data_diff: compare data and print the different one
                    ec_state_string: Print the string format of state
                    ecdev_offer: Offer net device to EtherCAT master
                    ecrt_request_master_err: return requested master
                    ecrt_request_master: return requested master
                    ecrt_release_master: master release
                    ecrt_version_magic: return magic verions number
                    ecrt_attach_master: return the pointer of running master

History:

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/

/******************************************************************************
 *
 *  $Id: module.c,v 1.10 2015/03/03 05:51:05 kidd Exp $
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

/** \file
 * EtherCAT master driver module.
 */

/*****************************************************************************/

#include "errno.h"
#include "types.h"
#include "base_data_op.h"
#include "debug.h"
//#include "useract.h"

#include <string.h>
#include "globals.h"
#include "master.h"
#include "device.h"
//#include "Useract.h"

/*****************************************************************************/

#define MAX_MASTERS 32 /**< Maximum number of masters. */

/*****************************************************************************/

int  ec_init_module(void);
void  ec_cleanup_module(void);

static int ec_mac_parse(uint8_t *, const char *, int);
/*****************************************************************************/
//gonggw, 2013.09.27, variables below to initialize the global variables which initialized by module loading in Linux
u8 mac_addr0[32]; //must be same as MAC address assigned to DM9000 in dm9000_probe
#define MASTER_NUM 1 //number of EtherCAT Master device
#define CURRENT_MASTER_NUM 0 //selected Master num

ec_master_t	master_pool[MASTER_NUM];
static char *main_devices[MAX_MASTERS]; /**< Main devices parameter. */
unsigned int master_count; /**< Number of masters. */
//static char *backup_devices[MAX_MASTERS]; /**< Backup devices parameter. */

ec_master_t *masters; /**< Array of masters. */
static uint8_t macs[MAX_MASTERS][2][ETH_ALEN]; /**< MAC addresses. */

ec_master_t *cur_master; /** current selected master**/

char *ec_master_version_str = EC_MASTER_VERSION; /**< Version string. */


/** Module initialization.
 *
 * Initializes \a master_count masters.
 * \return 0 on success, else < 0
 */
int ec_init_module(void)
{
    int i, ret = 0;
	u8 *mac_addr = "00:16:D4:9F:ED:A4";
    EC_INFO("Master driver %s\n", EC_MASTER_VERSION);
	//gongw, 2013.09.27, the variables below are initilized by module loading in Linux, now, just give constnat
	i = 0;
	while(*mac_addr)
	{
		mac_addr0[i++] = *mac_addr;
		mac_addr++;
	}
    mac_addr0[i] = '\0';
	main_devices[0] = (char*)mac_addr0;
	master_count	= MASTER_NUM;
    // zero MAC addresses
#ifdef CONFIG_USE_LIB_MEMCPY
    memset(macs, 0x00, sizeof(uint8_t) * MAX_MASTERS * 2 * ETH_ALEN);
#else
	ec_memset(macs, 0x00, sizeof(uint8_t) * MAX_MASTERS * 2 * ETH_ALEN);
#endif
    // process MAC parameters
    for (i = 0; i < master_count; i++) {
        ret = ec_mac_parse(macs[i][0], main_devices[i], 0);
        if (ret)
            goto out_class;
    }

    // initialize static master variables
    ec_master_init_static();

    if (master_count) {
#ifdef DYN_MEM_ALLOC
#ifdef EC_DYN_MEM_PRT
		_DBG_PRINTF_("%d in %s, ec_malloc size=%d\n", __LINE__, __FILE__, sizeof(ec_master_t) * master_count);
#endif
        if (!(masters = ec_malloc(sizeof(ec_master_t) * master_count,
                        0))) {
            EC_ERR("Failed to allocate memory"
                    " for EtherCAT masters.\n");
            ret = -ENOMEM;
            goto out_class;
        }
#else
		masters = &master_pool[0];
#endif
#ifdef CONFIG_USE_LIB_MEMCPY
		memset((char *)masters, 0x00, sizeof(ec_master_t) * master_count);
#else
        ec_memset((char *)masters, 0x00, sizeof(ec_master_t) * master_count);
#endif
        cur_master = &masters[CURRENT_MASTER_NUM];      
	}

    for (i = 0; i < master_count; i++) {
		ret = ec_master_init(&masters[i], i, macs[i][0], macs[i][1], EC_DEBUG_LEVEL);
        if (ret)
            goto out_free_masters;
    }

    EC_INFO("%u master%s waiting for devices.\n",
            master_count, (master_count == 1 ? "" : "s"));
    return ret;

out_free_masters:
    for (i--; i >= 0; i--)
        ec_master_clear(&masters[i]);
#ifdef DYN_MEM_ALLOC
	ec_free(masters);
#endif
out_class:
    return ret;
}

/*****************************************************************************/

/** Module cleanup.
 *
 * Clears all master instances.
 */
void ec_cleanup_module(void)
{
    unsigned int i;

    for (i = 0; i < master_count; i++) {
        ec_master_clear(&masters[i]);
    }
#ifdef DYN_MEM_ALLOC
    if (master_count)
        ec_free(masters);
#endif
    EC_INFO("Master module cleaned up.\n");
}

/*****************************************************************************/

/** Get the number of masters.
 */
unsigned int ec_master_count(void)
{
    return master_count;
}

/*****************************************************************************
 * MAC address functions
 ****************************************************************************/

/**
 * \return true, if two MAC addresses are equal.
 */
int ec_mac_equal(
        const uint8_t *mac1, /**< First MAC address. */
        const uint8_t *mac2 /**< Second MAC address. */
        )
{
    unsigned int i;

    for (i = 0; i < ETH_ALEN; i++)
        if (mac1[i] != mac2[i])
            return 0;

    return 1;
}

/*****************************************************************************/

/** Print a MAC address to a buffer.
 *
 * \return number of bytes written.
 */
ssize_t ec_mac_print(
        const uint8_t *mac, /**< MAC address */
        char *buffer /**< target buffer */
        )
{
    off_t off = 0;
    unsigned int i;

    for (i = 0; i < ETH_ALEN; i++) {
		off += sprintf((char *)(buffer + off), "%02x", mac[i]);
        if (i < ETH_ALEN - 1)
        {
			off += sprintf((char *)(buffer + off), ":");
        }
    }

    return off;
}

/*****************************************************************************/

/**
 * \return true, if the MAC address is all-zero.
 */
int ec_mac_is_zero(
        const uint8_t *mac /**< MAC address. */
        )
{
    unsigned int i;

    for (i = 0; i < ETH_ALEN; i++)
        if (mac[i])
            return 0;

    return 1;
}

/*****************************************************************************/

/**
 * \return true, if the given MAC address is the broadcast address.
 */
int ec_mac_is_broadcast(
        const uint8_t *mac /**< MAC address. */
        )
{
    unsigned int i;

    for (i = 0; i < ETH_ALEN; i++)
        if (mac[i] != 0xff)
            return 0;

    return 1;
}

/*****************************************************************************/

/** Parse a MAC address from a string.
 *
 * The MAC address must follow the regexp
 * "([0-9a-fA-F]{2}:){5}[0-9a-fA-F]{2}".
 *
 * \return 0 on success, else < 0
 */
static int ec_mac_parse(uint8_t *mac, const char *src, int allow_empty)
{
    unsigned int i, value;
    const char *orig = src;
    char *rem;
    if (!strlen(src)) {
        if (allow_empty){
            return 0;
        }
        else {
            EC_ERR("MAC address may not be empty.\n");
            return -EINVAL;
        }
    }

    for (i = 0; i < ETH_ALEN; i++) {
        value = simple_strtoul(src, &rem, 16);
        if (rem != src + 2
                || value > 0xFF
                || (i < ETH_ALEN - 1 && *rem != ':')) {
            EC_ERR("Invalid MAC address \"%s\".\n", orig);
            return -EINVAL;
        }
        mac[i] = value;
        if (i < ETH_ALEN - 1)
            src = rem + 1; // skip colon
    }

    return 0;
}

/*****************************************************************************/

/** Outputs frame contents for debugging purposes.
 * If the data block is larger than 256 bytes, only the first 128
 * and the last 128 bytes will be shown
 */
void ec_print_data(const uint8_t *data, /**< pointer to data */
                   size_t size /**< number of bytes to output */
                   )
{
    unsigned int i;

    EC_DBG("");
    for (i = 0; i < size; i++) {
        _DBG_PRINTF_("%02X ", data[i]);

        if ((i + 1) % 16 == 0 && i < size - 1) {
            _DBG_PRINTF_("\n");
            EC_DBG("");
        }

        if (i + 1 == 128 && size > 256) {
            _DBG_PRINTF_("dropped %zu bytes\n", size - 128 - i);
            i = size - 128;
            EC_DBG("");
        }
    }
    _DBG_PRINTF_("\n");
}

/*****************************************************************************/

/** Outputs frame contents and differences for debugging purposes.
 */
void ec_print_data_diff(const uint8_t *d1, /**< first data */
                        const uint8_t *d2, /**< second data */
                        size_t size /** number of bytes to output */
                        )
{
    unsigned int i;

    EC_DBG("");
    for (i = 0; i < size; i++) {
        if (d1[i] == d2[i]) _DBG_PRINTF_(".. ");
        else _DBG_PRINTF_("%02X ", d2[i]);
        if ((i + 1) % 16 == 0) {
            _DBG_PRINTF_("\n");
            EC_DBG("");
        }
    }
    _DBG_PRINTF_("\n");
}

/*****************************************************************************/

/** Prints slave states in clear text.
 */
size_t ec_state_string(uint8_t states, /**< slave states */
                       char *buffer, /**< target buffer
                                       (min. EC_STATE_STRING_SIZE bytes) */
                       uint8_t multi /**< Show multi-state mask. */
                       )
{
    off_t off = 0;
    unsigned int first = 1;

    if (!states) {
        off += sprintf(buffer + off, "(unknown)");
        return off;
    }

    if (multi) { // multiple slaves
        if (states & EC_SLAVE_STATE_INIT) {
            off += sprintf(buffer + off, "INIT");
            first = 0;
        }
        if (states & EC_SLAVE_STATE_PREOP) {
            if (!first) off += sprintf(buffer + off, ", ");
            off += sprintf(buffer + off, "PREOP");
            first = 0;
        }
        if (states & EC_SLAVE_STATE_SAFEOP) {
            if (!first) off += sprintf(buffer + off, ", ");
            off += sprintf(buffer + off, "SAFEOP");
            first = 0;
        }
        if (states & EC_SLAVE_STATE_OP) {
            if (!first) off += sprintf(buffer + off, ", ");
            off += sprintf(buffer + off, "OP");
        }
    } else { // single slave
        if ((states & EC_SLAVE_STATE_MASK) == EC_SLAVE_STATE_INIT) {
            off += sprintf(buffer + off, "INIT");
        } else if ((states & EC_SLAVE_STATE_MASK) == EC_SLAVE_STATE_PREOP) {
            off += sprintf(buffer + off, "PREOP");
        } else if ((states & EC_SLAVE_STATE_MASK) == EC_SLAVE_STATE_BOOT) {
            off += sprintf(buffer + off, "BOOT");
        } else if ((states & EC_SLAVE_STATE_MASK) == EC_SLAVE_STATE_SAFEOP) {
            off += sprintf(buffer + off, "SAFEOP");
        } else if ((states & EC_SLAVE_STATE_MASK) == EC_SLAVE_STATE_OP) {
            off += sprintf(buffer + off, "OP");
        } else {
            off += sprintf(buffer + off, "(invalid)");
        }
        first = 0;
    }

    if (states & EC_SLAVE_STATE_ACK_ERR) {
        if (!first) off += sprintf(buffer + off, " + ");
        off += sprintf(buffer + off, "ERROR");
    }

    return off;
}

/******************************************************************************
 *  Device interface
 *****************************************************************************/

/** Offers an EtherCAT device to a certain master.
 *
 * The master decides, if it wants to use the device for EtherCAT operation,
 * or not. It is important, that the offered net_device is not used by the
 * kernel IP stack. If the master, accepted the offer, the address of the
 * newly created EtherCAT device is returned, else \a NULL is returned.
 *
 * \return Pointer to device, if accepted, or NULL if declined.
 * \ingroup DeviceInterface
 */
ec_device_t *ecdev_offer(
        struct net_device *net_dev, /**< net_device to offer */
        ec_pollfunc_t poll
        )
{
    ec_master_t *master;
    char str[20];
    unsigned int i;
    for (i = 0; i < master_count; i++) {
        master = &masters[i];
        if (ec_mac_equal(master->main_mac, net_dev->dev_addr)
                || ec_mac_is_broadcast(master->main_mac)) {
            ec_mac_print(net_dev->dev_addr, str);
            EC_INFO("Accepting device %s for master %u.\n",
                    str, master->index);

			ec_device_attach(&master->main_device, net_dev, poll);

            snprintf(net_dev->name, IFNAMSIZ, "ec%u", master->index);
            return &master->main_device; // offer accepted
        }
        else {
            if (master->debug_level > 2) {
                ec_mac_print(net_dev->dev_addr, str);
                EC_MASTER_DBG(master, 0, "Master declined device %s.\n",
                        str);
            }
        }
    }

    return NULL; // offer declined
}

/******************************************************************************
 *  Realtime interface
 *****************************************************************************/

/** Request a master.
 *
 * Same as ecrt_request_master(), but with ERR_PTR() return value.
 */
ec_master_t *ecrt_request_master_err(
        unsigned int master_index /**< Master index. */
        )
{
    ec_master_t *master, *errptr = NULL;

    EC_INFO("Requesting master %u...\n", master_index);

    if (master_index >= master_count) {
        EC_ERR("Invalid master index %u.\n", master_index);
        errptr = ERR_PTR(-EINVAL);
        goto out_return;
    }
    master = &masters[master_index];
    return master;
 out_return:
    return errptr;
}

/*****************************************************************************/

ec_master_t *ecrt_request_master(unsigned int master_index)
{
    ec_master_t *master = ecrt_request_master_err(master_index);
    return IS_ERR(master) ? NULL : master;
}

/*****************************************************************************/

void ecrt_release_master(ec_master_t *master)
{
    EC_MASTER_INFO(master, "Releasing master...\n");

    if (!master->reserved) {
        EC_MASTER_WARN(master, "%s(): Master was was not requested!\n",
                __func__);
        return;
    }

    ec_master_leave_operation_phase(master);
    master->reserved = 0;

    EC_MASTER_INFO(master, "Released.\n");
}

/*****************************************************************************/

unsigned int ecrt_version_magic(void)
{
    return ECRT_VERSION_MAGIC;
}

/*****************************************************************************/

/** Return pointer to running master
 */
ec_master_t *ecrt_attach_master(unsigned int master_index)
{
    ec_master_t *master = NULL;

    EC_INFO("Requesting master %u...\n", master_index);

    if (master_index >= master_count) {
        EC_ERR("Invalid master index %u.\n", master_index);
        return master;
    }

    master = &masters[master_index];
    if (master->reserved) {
       // ok master is attached
        EC_INFO("attaching Master %u!\n", master_index);
    } else {
        EC_ERR("No Master %u in use!\n", master_index);
        master = NULL;
    }
    return master;
}

/*****************************************************************************/

/** Global request state type translation table.
 *
 * Translates an internal request state to an external one.
 */
const ec_request_state_t ec_request_state_translation_table[] = {
    EC_REQUEST_UNUSED,  // EC_INT_REQUEST_INIT,
    EC_REQUEST_BUSY,    // EC_INT_REQUEST_QUEUED,
    EC_REQUEST_BUSY,    // EC_INT_REQUEST_BUSY,
    EC_REQUEST_SUCCESS, // EC_INT_REQUEST_SUCCESS,
    EC_REQUEST_ERROR    // EC_INT_REQUEST_FAILURE
};

/*****************************************************************************/

