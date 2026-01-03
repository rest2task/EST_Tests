/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    foe_request.c

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:           2014.06.06

Description:  File over EtherCAT request(SDO) implementation

Others:

Function List:  
                    ec_foe_request_init          :  SDO request for FoE init
                    ec_foe_request_clear        : FoE request clear
                    ec_foe_request_clear_data:clear data buffer in FoE Request
                    ec_foe_request_alloc        :Re-allocate data buffer for FoE Request
                    ec_foe_request_copy_data:Copy data to buffer in FoE Request
                    ec_foe_request_timed_out: FoE Request Time Out check
                    ec_foe_request_data        :Return the start address of data buffer in request 
                    ec_foe_request_data_size : Return the szie of data buffer in request 
                    ec_foe_request_read        : Generate the Read Request
                    ec_foe_request_write       : Generate the Write Request
History:    

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/

/******************************************************************************
 *
 *  $Id: foe_request.c,v 1.6 2015/02/15 09:12:58 kidd Exp $
 *
 *  Copyright (C) 2008  Olav Zarges, imc Messsysteme GmbH
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
 * File-over-EtherCAT request functions.
 */

/*****************************************************************************/

#include <string.h>

#include "ec_globals.h"
#include "config.h"
#include "ec_mm.h"
#include "debug.h"

#include "foe_request.h"
#include "foe.h"

/*****************************************************************************/

/** Default timeout in ms to wait for FoE transfer responses.
 */
#define EC_FOE_REQUEST_RESPONSE_TIMEOUT 3000

/*****************************************************************************/

void ec_foe_request_clear_data(ec_foe_request_t *);

/*****************************************************************************/

/** FoE request constructor.
 */
void ec_foe_request_init(
        ec_foe_request_t *req, /**< FoE request. */
        uint8_t* file_name /** filename */)
{
    req->buffer = NULL;
    req->file_name = file_name;
    req->buffer_size = 0;
    req->data_size = 0;
    req->dir = EC_DIR_INVALID;
    req->issue_timeout = 0; // no timeout
    req->response_timeout = EC_FOE_REQUEST_RESPONSE_TIMEOUT;
    req->state = EC_INT_REQUEST_INIT;
    req->result = FOE_BUSY;
    req->error_code = 0x00000000;
}

/*****************************************************************************/

/** FoE request destructor.
 */
void ec_foe_request_clear(
        ec_foe_request_t *req /**< FoE request. */
        )
{
    ec_foe_request_clear_data(req);
}

/*****************************************************************************/

/** FoE request destructor.
 */
void ec_foe_request_clear_data(
        ec_foe_request_t *req /**< FoE request. */
        )
{
    if (req->buffer) {
#ifdef DYN_MEM_ALLOC
        ec_free(req->buffer);
#else
		buffer_pool_put(req->buffer);
#endif
        req->buffer = NULL;
    }

    req->buffer_size = 0;
    req->data_size = 0;
}

/*****************************************************************************/

/** Pre-allocates the data memory.
 *
 * If the \a buffer_size is already bigger than \a size, nothing is done.
 */
#ifndef DYN_MEM_ALLOC
uint8_t foe_req_buf[4];
#endif
int ec_foe_request_alloc(
        ec_foe_request_t *req, /**< FoE request. */
        size_t size /**< Data size to allocate. */
        )
{
    if (size <= req->buffer_size)
        return 0;

    ec_foe_request_clear_data(req);
#ifdef DYN_MEM_ALLOC
#ifdef EC_DYN_MEM_PRT
	_DBG_PRINTF_("%d in %s, ec_malloc size=%d\n", __LINE__, __FILE__, size);
#endif
	req->buffer = (uint8_t *) ec_malloc(size, 0);
    if (req->buffer == NULL) {
        EC_ERR("Failed to allocate %zu bytes of FoE memory.\n", size);
        return -1;
    }
#else
	req->buffer = foe_req_buf; //gonggw, 2015.02.14,  MUST be modified when use FOE protocol
#endif
    req->buffer_size = size;
    req->data_size = 0;
    return 0;
}

/*****************************************************************************/

/** Copies FoE data from an external source.
 *
 * If the \a buffer_size is to small, new memory is allocated.
 */
int ec_foe_request_copy_data(
        ec_foe_request_t *req, /**< FoE request. */
        const uint8_t *source, /**< Source data. */
        size_t size /**< Number of bytes in \a source. */
        )
{
    if (ec_foe_request_alloc(req, size))
        return -1;
#ifdef CONFIG_USE_LIB_MEMCPY
    memcpy(req->buffer, source, size);
#else
	ec_memcpy(req->buffer, source, size);
#endif
    req->data_size = size;
    return 0;
}

/*****************************************************************************/

/** Checks, if the timeout was exceeded.
 *
 * \return non-zero if the timeout was exceeded, else zero.
 */
int ec_foe_request_timed_out(
        const ec_foe_request_t *req /**< FoE request. */
        )
{
    return req->issue_timeout
        && jiffies - req->jiffies_start > HZ * req->issue_timeout / 1000;
}

/*****************************************************************************/

/** Set the request timeout.
 */
void ec_foe_request_timeout(
        ec_foe_request_t *req, /**< FoE request. */
        uint32_t timeout /**< Timeout in ms. */
        )
{
    req->issue_timeout = timeout;
}

/*****************************************************************************/

/** Returns a pointer to the request's data.
 *
 * \return Data pointer.
 */
uint8_t *ec_foe_request_data(
        ec_foe_request_t *req /**< FoE request. */
        )
{
    return req->buffer;
}

/*****************************************************************************/

/** Returns the data size.
 *
 * \return Data size.
 */
size_t ec_foe_request_data_size(
        const ec_foe_request_t *req /**< FoE request. */
        )
{
    return req->data_size;
}

/*****************************************************************************/

/** Prepares a read request (slave to master).
 */
void ec_foe_request_read(
        ec_foe_request_t *req /**< FoE request. */
        )
{
    req->dir = EC_DIR_INPUT;
    req->state = EC_INT_REQUEST_QUEUED;
    req->result = FOE_BUSY;
    req->jiffies_start = jiffies;
}

/*****************************************************************************/

/** Prepares a write request (master to slave).
 */
void ec_foe_request_write(
        ec_foe_request_t *req /**< FoE request. */
        )
{
    req->dir = EC_DIR_OUTPUT;
    req->state = EC_INT_REQUEST_QUEUED;
    req->result = FOE_BUSY;
    req->jiffies_start = jiffies;
}

/*****************************************************************************/
