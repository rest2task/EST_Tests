/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    fmmu_conifg.h

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:           2014.06.06

Description:  FMMU Config definition

Others:

History:    

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/


/** \file
 * EtherCAT FMMU configuration structure.
 */

/*****************************************************************************/

#ifndef __EC_FMMU_CONFIG_H__
#define __EC_FMMU_CONFIG_H__

#include "globals.h"
#include "sync.h"

/*****************************************************************************/

/** FMMU configuration.
 */
typedef struct {
    struct list_head        list;                   /**< List node used by domain. */
    const ec_slave_config_t *sc;                    /**< EtherCAT slave config. */
    const ec_domain_t       *domain;                /**< Domain. */
    uint8_t                 sync_index;             /**< Index of sync manager to use. */
    ec_direction_t          dir;                    /**< FMMU direction. */
    uint32_t                logical_start_address;  /**< Logical start address. */
    size_t                  tx_size;                /**< Transmitted (bus) size. FIXME */
    uint32_t                domain_address;         /** Domain start address */
    unsigned int            data_size;              /**< Covered PDO size. */
} ec_fmmu_config_t;

/*****************************************************************************/

void ec_fmmu_config_init(ec_fmmu_config_t *, ec_slave_config_t *,
        uint8_t, ec_direction_t);

void ec_fmmu_config_domain(ec_fmmu_config_t *, ec_domain_t *,
        uint32_t , size_t);

void ec_fmmu_config_page(const ec_fmmu_config_t *, const ec_sync_t *,
        uint8_t *);

/*****************************************************************************/

#endif
