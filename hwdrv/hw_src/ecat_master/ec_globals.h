/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    ec_globals.h

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:           2013.09.23

Description:    Global config&definition of EtherCAT

Others:


History:    

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/

/******************************************************************************
 *
 *  $Id: ec_globals.h,v 1.1 2015/02/06 05:23:05 kidd Exp $
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
   Global definitions and macros.
*/

/*****************************************************************************/

#ifndef __EC_GLOBALS_H__
#define __EC_GLOBALS_H__

#include "ec_config.h"
#include "config.h"
#include "types.h"

/******************************************************************************
 *  Overall macros
 *****************************************************************************/

/** Helper macro for EC_STR(), literates a macro argument.
 *
 * \param X argument to literate.
 */
#define EC_LIT(X) #X

/** Converts a macro argument to a string.
 *
 * \param X argument to stringify.
 */
#define EC_STR(X) EC_LIT(X)

/** Master version string
 */ 
#define REV_ESTO "ESTO 1.0"
#define EC_MASTER_VERSION   VERSION_DEV " " EC_STR(REV_ESTO)

/*****************************************************************************/
extern u32 cpu_khz;
extern volatile u32 jiffies;
extern volatile u64 jiffies_64;

#define HZ CONFIG_SYS_HZ
/*******some functions initialization**********/
int dm9000_init(void);
unsigned long long get_ticks(void);
#endif
