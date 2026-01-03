/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    pdo.h

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:           2014.06.06

Description:  PDO process structure and parameters definition
Others:

History:

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/

/******************************************************************************
 *
 *  $Id: pdo.h,v 1.4 2015/02/15 02:07:42 kidd Exp $
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
   EtherCAT Process data object structure.
*/

/*****************************************************************************/

#ifndef __EC_PDO_H__
#define __EC_PDO_H__

#include "list.h"

#include "globals.h"
#include "pdo_entry.h"

/*****************************************************************************/

#define PDO_NAME_LEN	4	//Max length of PDO name
/** PDO description.
 */
typedef struct {
    struct list_head list; /**< List item. */
    uint16_t index; /**< PDO index. */
    int8_t sync_index; /**< Assigned sync manager. \todo remove? */
#if 0 //gonggw, 2015.02.13, original code
	char *name; /**< PDO name. */
#else
	char name[PDO_NAME_LEN]; /**< PDO name. */
	uint8_t	used;
#endif
    struct list_head entries; /**< List of PDO entries. */
} ec_pdo_t;

/*****************************************************************************/

void ec_pdo_init(ec_pdo_t *);
int ec_pdo_init_copy(ec_pdo_t *, const ec_pdo_t *);
void ec_pdo_clear(ec_pdo_t *);
void ec_pdo_clear_entries(ec_pdo_t *);
int ec_pdo_set_name(ec_pdo_t *, const char *);
ec_pdo_entry_t *ec_pdo_add_entry(ec_pdo_t *, uint16_t, uint8_t, uint8_t);
int ec_pdo_copy_entries(ec_pdo_t *, const ec_pdo_t *);
int ec_pdo_equal_entries(const ec_pdo_t *, const ec_pdo_t *);
unsigned int ec_pdo_entry_count(const ec_pdo_t *);
const ec_pdo_entry_t *ec_pdo_find_entry_by_pos_const(
        const ec_pdo_t *, unsigned int);

void ec_pdo_print_entries(const ec_pdo_t *);

/*****************************************************************************/

#endif
