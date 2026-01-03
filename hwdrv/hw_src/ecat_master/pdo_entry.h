/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    pdo_entry.h

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:           2014.06.06

Description:  PDO entry definition
Others:
                    
History:

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/

/******************************************************************************
 *
 *  $Id: pdo_entry.h,v 1.4 2015/02/15 02:07:42 kidd Exp $
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

#ifndef __EC_PDO_ENTRY_H__
#define __EC_PDO_ENTRY_H__

#include "list.h"

#include "globals.h"
#include "ecdev.h"

/*****************************************************************************/
#define PDO_ENTRY_NAME_LEN	4	//Max length of PDO name

/** PDO entry description.
 */
typedef struct {
    struct list_head list; /**< list item */
    uint16_t index; /**< PDO entry index */
    uint8_t subindex; /**< PDO entry subindex */
#if 0 //gonggw, 2015.02.13, original code
    char *name; /**< entry name */
#else
    char name[PDO_ENTRY_NAME_LEN]; /**< entry name */
	uint8_t used;
#endif
    uint8_t bit_length; /**< entry length in bit */
} ec_pdo_entry_t;

/*****************************************************************************/

void ec_pdo_entry_init(ec_pdo_entry_t *);
int ec_pdo_entry_init_copy(ec_pdo_entry_t *, const ec_pdo_entry_t *);
void ec_pdo_entry_clear(ec_pdo_entry_t *);
int ec_pdo_entry_set_name(ec_pdo_entry_t *, const char *);
int ec_pdo_entry_equal(const ec_pdo_entry_t *, const ec_pdo_entry_t *);

/*****************************************************************************/

#endif
