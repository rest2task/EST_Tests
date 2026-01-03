/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    pdo_entry.c

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:           2014.06.06

Description:  PDO entry process
Others:

Function List:  
                    ec_pdo_entry_init: PDO entry sturctures init                  
                    ec_pdo_entry_init_copy: copy PDO entry structure
                    ec_pdo_entry_clear: Clear PDO entry structure
                    ec_pdo_entry_set_name: Set name to PDO entry
                    ec_pdo_entry_equal: PDO entry compare and return equal status
                    
History:

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/

/******************************************************************************
 *
 *  $Id: pdo_entry.c,v 1.8 2015/02/15 09:12:58 kidd Exp $
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
   EtherCAT process data object entry methods.
*/

/*****************************************************************************/
#include "errno.h"
#include "config.h"
#include <string.h>
#include "debug.h"
#include "Ec_mm.h"

#include "pdo_entry.h"

/*****************************************************************************/

/** PDO entry constructor.
 */
void ec_pdo_entry_init(
        ec_pdo_entry_t *entry /**< PDO entry. */
        )
{
#if 0
    entry->name = NULL;
#else
	entry->name[0]	= '\0';
#endif
}

/*****************************************************************************/

/** PDO entry copy constructor.
 *
 * \retval  0 Success.
 * \retval <0 Error code.
 */
int ec_pdo_entry_init_copy(
        ec_pdo_entry_t *entry, /**< PDO entry. */
        const ec_pdo_entry_t *other /**< PDO entry to copy from. */
        )
{
    entry->index = other->index;
    entry->subindex = other->subindex;
#if 0
    entry->name = NULL;
#else
    entry->name[0] = '\0';
#endif
    entry->bit_length = other->bit_length;

    return ec_pdo_entry_set_name(entry, other->name);
}

/*****************************************************************************/

/** PDO entry destructor.
 */
void ec_pdo_entry_clear(ec_pdo_entry_t *entry /**< PDO entry. */)
{
#if 0
    if (entry->name)
        ec_free(entry->name);
#endif
}

/*****************************************************************************/

/** Set PDO entry name.
 *
 * \retval  0 Success.
 * \retval <0 Error code.
 */
int ec_pdo_entry_set_name(
        ec_pdo_entry_t *entry, /**< PDO entry. */
        const char *name /**< New name. */
        )
{
    unsigned int len;

    if (entry->name && name && !strcmp(entry->name, name))
        return 0;
#if 0
    if (entry->name)
        ec_free(entry->name);

    if (name && (len = strlen(name))) {
#ifdef EC_DYN_MEM_PRT
		_DBG_PRINTF_("%d in %s, ec_malloc size=%d\n", __LINE__, __FILE__, len + 1);
#endif
        if (!(entry->name = (char *) ec_malloc(len + 1, 0))) {
            EC_ERR("Failed to allocate PDO entry name.\n");
            return -ENOMEM;
        }
#ifdef CONFIG_USE_LIB_MEMCPY
        memcpy(entry->name, name, len + 1);
#else
		ec_memcpy(entry->name, name, len + 1);
#endif
    } else {
        entry->name = NULL;
    }
#else
	len = strlen(name);
	if (name && (len != NULL)) {
		if (len > (PDO_ENTRY_NAME_LEN - 1))
		{
			memcpy(entry->name, name, (PDO_ENTRY_NAME_LEN - 1));
			entry->name[PDO_ENTRY_NAME_LEN - 1] = '\0';
		}
		else
		{
			memcpy(entry->name, name, len + 1);			
		}
	}
#endif
    return 0;
}

/*****************************************************************************/

/** Compares two PDO entries.
 *
 * \retval 1 The entries are equal.
 * \retval 0 The entries differ.
 */
int ec_pdo_entry_equal(
        const ec_pdo_entry_t *entry1, /**< First PDO entry. */
        const ec_pdo_entry_t *entry2 /**< Second PDO entry. */
        )
{
    return entry1->index == entry2->index
        && entry1->subindex == entry2->subindex
        && entry1->bit_length == entry2->bit_length;
}

/*****************************************************************************/
