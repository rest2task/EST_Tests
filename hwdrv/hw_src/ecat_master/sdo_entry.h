/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    sdo_entry.h

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:           2014.06.06

Description:  
Others:

History:

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/

/******************************************************************************
 *
 *  $Id: sdo_entry.h,v 1.3 2015/02/15 02:07:42 kidd Exp $
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
   EtherCAT CANopen SDO entry structure.
*/

/*****************************************************************************/

#ifndef __EC_SDO_ENTRY_H__
#define __EC_SDO_ENTRY_H__

#include "list.h"

#include "globals.h"

/*****************************************************************************/

struct ec_sdo;
typedef struct ec_sdo ec_sdo_t; /**< \see ec_sdo. */

/*****************************************************************************/

/** CANopen SDO entry.
 */
#define SDO_ENTRY_DESC_LEN	12
typedef struct {
    struct list_head list; /**< List item. */
    ec_sdo_t *sdo; /**< Parent SDO. */
    uint8_t subindex; /**< Subindex. */
    uint16_t data_type; /**< Data type. */
    uint16_t bit_length; /**< Data size in bit. */
    uint8_t read_access[EC_SDO_ENTRY_ACCESS_COUNT]; /**< Read access. */
    uint8_t write_access[EC_SDO_ENTRY_ACCESS_COUNT]; /**< Write access. */
#if 0
    char *description; /**< Description. */
#else
    char description[SDO_ENTRY_DESC_LEN]; /**< Description. */
	uint8_t	used;
#endif
} ec_sdo_entry_t;

/*****************************************************************************/

void ec_sdo_entry_init(ec_sdo_entry_t *, ec_sdo_t *, uint8_t);
void ec_sdo_entry_clear(ec_sdo_entry_t *);

/*****************************************************************************/

#endif
