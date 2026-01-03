/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    sdo_request.c

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:           2014.06.06

Description:  SDO request structure process
Others:

Function List:  
                    ec_sdo_request_init:                  
                    ec_sdo_request_clear:
                    ec_sdo_request_copy
                    ec_sdo_request_clear_data
                    ec_sdo_request_address
                    ec_sdo_request_alloc
                    ec_sdo_request_copy_data
                    ec_sdo_request_timed_out
                    ecrt_sdo_request_timeout
                    ecrt_sdo_request_data
                    ecrt_sdo_request_data_size
                    ecrt_sdo_request_state
                    ecrt_sdo_request_read
                    ecrt_sdo_request_read
History:

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/

/******************************************************************************
 *
 *  $Id: sdo_request.c,v 1.8 2015/02/28 01:09:50 kidd Exp $
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
 * Canopen over EtherCAT SDO request functions.
 */

/*****************************************************************************/

#include "errno.h"
#include "types.h"
#include "ec_globals.h"
#include "config.h"
#include <string.h>
#include "debug.h"
#include "ec_mm.h"

#include "sdo_request.h"

/*****************************************************************************/

/** Default timeout in ms to wait for SDO transfer responses.
 */
#define EC_SDO_REQUEST_RESPONSE_TIMEOUT 1000

/*****************************************************************************/

void ec_sdo_request_clear_data(ec_sdo_request_t *);

/*****************************************************************************/

/** SDO request constructor.
 */
void ec_sdo_request_init(
        ec_sdo_request_t *req /**< SDO request. */
        )
{
    req->complete_access = 0;
    req->data = NULL;
    req->mem_size = 0;
    req->data_size = 0;
    req->dir = EC_DIR_INVALID;
    req->issue_timeout = 0; // no timeout
    req->response_timeout = EC_SDO_REQUEST_RESPONSE_TIMEOUT;
    req->state = EC_INT_REQUEST_INIT;
    req->errno = 0;
    req->abort_code = 0x00000000;
}

/*****************************************************************************/

/** SDO request destructor.
 */
void ec_sdo_request_clear(
        ec_sdo_request_t *req /**< SDO request. */
        )
{
    ec_sdo_request_clear_data(req);
}

/*****************************************************************************/

/** Copy another SDO request.
 *
 * \attention Only the index subindex and data are copied.
 */
int ec_sdo_request_copy(
        ec_sdo_request_t *req, /**< SDO request. */
        const ec_sdo_request_t *other /**< Other SDO request to copy from. */
        )
{
    req->complete_access = other->complete_access;
    req->index = other->index;
    req->subindex = other->subindex;
    return ec_sdo_request_copy_data(req, other->data, other->data_size);
}

/*****************************************************************************/

/** SDO request destructor.
 */
void ec_sdo_request_clear_data(
        ec_sdo_request_t *req /**< SDO request. */
        )
{
    if (req->data) {
#ifdef DYN_MEM_ALLOC
        ec_free(req->data);
#else
		buffer_pool_put(req->data);
#endif
        req->data = NULL;
    }

    req->mem_size = 0;
    req->data_size = 0;
}

/*****************************************************************************/

/** Set the SDO address.
 */
void ec_sdo_request_address(
        ec_sdo_request_t *req, /**< SDO request. */
        uint16_t index, /**< SDO index. */
        uint8_t subindex /**< SDO subindex. */
        )
{
    req->index = index;
    req->subindex = subindex;
}

/*****************************************************************************/

/** Pre-allocates the data memory.
 *
 * If the \a mem_size is already bigger than \a size, nothing is done.
 *
 * \return 0 on success, otherwise -ENOMEM.
 */
int ec_sdo_request_alloc(
        ec_sdo_request_t *req, /**< SDO request. */
        size_t size /**< Data size to allocate. */
        )
{
    if (size <= req->mem_size)
        return 0;

    ec_sdo_request_clear_data(req);
#ifdef DYN_MEM_ALLOC
#ifdef EC_DYN_MEM_PRT
	_DBG_PRINTF_("%d in %s, ec_malloc size=%d\n", __LINE__, __FILE__, size);
#endif
    if (!(req->data = (uint8_t *) ec_malloc(size, 0))) {
        EC_ERR("Failed to allocate %zu bytes of SDO memory.\n", size);
        return -ENOMEM;
    }
#else
	req->data = buffer_pool_get(size);
	if (req->data == NULL) {
		EC_ERR("Failed to allocate %zu bytes of SDO memory.\n", size);
		return -ENOMEM;
	}
	
#endif
    req->mem_size = size;
    req->data_size = 0;
    return 0;
}

/*****************************************************************************/

/** Copies SDO data from an external source.
 *
 * If the \a mem_size is to small, new memory is allocated.
 *
 * \retval  0 Success.
 * \retval <0 Error code.
 */
int ec_sdo_request_copy_data(
        ec_sdo_request_t *req, /**< SDO request. */
        const uint8_t *source, /**< Source data. */
        size_t size /**< Number of bytes in \a source. */
        )
{
    int ret = ec_sdo_request_alloc(req, size);
    if (ret < 0)
        return ret;
#ifdef CONFIG_USE_LIB_MEMCPY
    memcpy(req->data, source, size);
#else
	ec_memcpy(req->data, source, size);
#endif
    req->data_size = size;
    return 0;
}

/*****************************************************************************/

/** Checks, if the timeout was exceeded.
 *
 * \return non-zero if the timeout was exceeded, else zero.
 */
int ec_sdo_request_timed_out(const ec_sdo_request_t *req /**< SDO request. */)
{
    return req->issue_timeout
        && jiffies - req->jiffies_start > HZ * req->issue_timeout / 1000;
}

/*****************************************************************************
 * Application interface.
 ****************************************************************************/

void ecrt_sdo_request_timeout(ec_sdo_request_t *req, uint32_t timeout)
{
    req->issue_timeout = timeout;
}

/*****************************************************************************/

uint8_t *ecrt_sdo_request_data(ec_sdo_request_t *req)
{
    return req->data;
}

/*****************************************************************************/

size_t ecrt_sdo_request_data_size(const ec_sdo_request_t *req)
{
    return req->data_size;
}

/*****************************************************************************/

ec_request_state_t ecrt_sdo_request_state(const ec_sdo_request_t *req)
{
   return ec_request_state_translation_table[req->state];
}

/*****************************************************************************/

void ecrt_sdo_request_read(ec_sdo_request_t *req)
{
    req->dir = EC_DIR_INPUT;
    req->state = EC_INT_REQUEST_QUEUED;
    req->errno = 0;
    req->abort_code = 0x00000000;
    req->jiffies_start = jiffies;
}

/*****************************************************************************/

void ecrt_sdo_request_write(ec_sdo_request_t *req)
{
    req->dir = EC_DIR_OUTPUT;
    req->state = EC_INT_REQUEST_QUEUED;
    req->errno = 0;
    req->abort_code = 0x00000000;
    req->jiffies_start = jiffies;
}

