/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    sdo.c

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:           2014.06.06

Description:  SDO structure process
Others:

Function List:  
                    ec_sdo_init: PDO list init                  
                    ec_sdo_clear: PDO list clear
                    ec_sdo_get_entry:
                    ec_sdo_get_entry_const:

History:

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/

/******************************************************************************
 *
 *  $Id: sdo.c,v 1.4 2015/02/28 01:09:50 kidd Exp $
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
   CANopen SDO functions.
*/

/*****************************************************************************/


#include "ec_mm.h"
#include "master.h"

#include "sdo.h"

/*****************************************************************************/

/** Constructor.
 */
void ec_sdo_init(
        ec_sdo_t *sdo, /**< SDO. */
        ec_slave_t *slave, /**< Parent slave. */
        uint16_t index /**< SDO index. */
        )
{
    sdo->slave = slave;
    sdo->index = index;
    sdo->object_code = 0x00;
#if 0
    sdo->name = NULL;
#endif
    sdo->max_subindex = 0;
    INIT_LIST_HEAD(&sdo->entries);
}

/*****************************************************************************/

/** SDO destructor.
 *
 * Clears and frees an SDO object.
 */
void ec_sdo_clear(
        ec_sdo_t *sdo /**< SDO. */
        )
{
    ec_sdo_entry_t *entry, *next;

    // free all entries
    list_for_each_entry_safe(entry,ec_sdo_entry_t, next, &sdo->entries, list) {
        list_del(&entry->list);
        ec_sdo_entry_clear(entry);

#ifdef DYN_MEM_ALLOC	//gonggw, 2015.02.14
        ec_free(entry);
#else
		sdo_entry_pool_put(entry);
#endif
    }
#if 0
    if (sdo->name)
        ec_free(sdo->name);
#endif
}

/*****************************************************************************/

/** Get an SDO entry from an SDO via its subindex.
 * 
 * \retval >0 Pointer to the requested SDO entry.
 * \retval NULL SDO entry not found.
 */
ec_sdo_entry_t *ec_sdo_get_entry(
        ec_sdo_t *sdo, /**< SDO. */
        uint8_t subindex /**< Entry subindex. */
        )
{
    ec_sdo_entry_t *entry;

    list_for_each_entry(entry, ec_sdo_entry_t,&sdo->entries, list) {
        if (entry->subindex != subindex)
            continue;
        return entry;
    }

    return NULL;
}

/*****************************************************************************/

/** Get an SDO entry from an SDO via its subindex.
 *
 * const version.
 * 
 * \retval >0 Pointer to the requested SDO entry.
 * \retval NULL SDO entry not found.
 */
const ec_sdo_entry_t *ec_sdo_get_entry_const(
        const ec_sdo_t *sdo, /**< SDO. */
        uint8_t subindex /**< Entry subindex. */
        )
{
    const ec_sdo_entry_t *entry;

    list_for_each_entry(entry,ec_sdo_entry_t, &sdo->entries, list) {
        if (entry->subindex != subindex)
            continue;
        return entry;
    }

    return NULL;
}

/*****************************************************************************/
