/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    sdo.h

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:           2014.06.06

Description:  SDO structure process definition
Others:

History:

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/

/******************************************************************************
 *
 *  $Id: sdo.h,v 1.3 2015/02/15 02:07:42 kidd Exp $
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
   EtherCAT CANopen SDO structure.
*/

/*****************************************************************************/

#ifndef __EC_SDO_H__
#define __EC_SDO_H__

#include "list.h"

#include "globals.h"
#include "sdo_entry.h"

/*****************************************************************************/

/** CANopen SDO.
 */
#define SDO_NAME_LEN	4
struct ec_sdo {
    struct list_head list; /**< List item. */
    ec_slave_t *slave; /**< Parent slave. */
    uint16_t index; /**< SDO index. */
    uint8_t object_code; /**< Object code. */
#if 0
    char *name; /**< SDO name. */
#else
	char name[SDO_NAME_LEN]; /**< SDO name. */
	uint8_t used;
#endif
    uint8_t max_subindex; /**< Maximum subindex. */
    struct list_head entries; /**< List of entries. */
};

/*****************************************************************************/

void ec_sdo_init(ec_sdo_t *, ec_slave_t *, uint16_t);
void ec_sdo_clear(ec_sdo_t *);

ec_sdo_entry_t *ec_sdo_get_entry(ec_sdo_t *, uint8_t);
const ec_sdo_entry_t *ec_sdo_get_entry_const(const ec_sdo_t *, uint8_t);

/*****************************************************************************/

#endif
