/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    domain.c

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:           2014.06.06

Description:  domain operation for Logical addressing access

Others:

Function List:  
                    ec_domain_init                         :  Domain structure init
                    ec_domain_clear                       : Free specific domain structure
                    ec_domain_clear_data               :Free domain buffer
                    ec_domain_add_fmmu_config    : Add one FMMU config to domain
                    ec_domain_add_datagram         : Add one Logical addressing datagram to domain
                    shall_count                              :  Check the specific SC was added to domain
                    ec_domain_finish                      : calculate FMMU info in domain
                    ec_domain_fmmu_count           : return the number of FMMU in domain
                    ec_domain_find_fmmu              : find one FMMU in domain
                    ecrt_domain_reg_pdo_entry_list: PDO and domain interface
                    ecrt_domain_size                      : Return active data size of domain
                    ecrt_domain_external_memory  : Allocate external memory to domain
                    ecrt_domain_data                      : Return data buffer in domain
                    ecrt_domain_process                 : Domain process
                    ecrt_domain_queue                   : Add datagram to domain datagram queue
                    ecrt_domain_state                     : Update domain state
                    
History:    

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/

/******************************************************************************
 *
 *  $Id: domain.c,v 1.6 2015/02/15 09:12:58 kidd Exp $
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
   EtherCAT domain methods.
*/

/*****************************************************************************/

#include <stdio.h>

#include "list.h"
#include "errno.h"
#include "ecrt.h"
#include "ec_mm.h"
#include "debug.h"

#include "globals.h"
#include "master.h"
#include "slave_config.h"
#include "domain.h"

/*****************************************************************************/

void ec_domain_clear_data(ec_domain_t *);

/*****************************************************************************/

/** Domain constructor.
 */
void ec_domain_init(
        ec_domain_t *domain, /**< EtherCAT domain. */
        ec_master_t *master, /**< Parent master. */
        unsigned int index /**< Index. */
        )
{
    domain->master = master;
    domain->index = index;
    INIT_LIST_HEAD(&domain->fmmu_configs);
    domain->data_size = 0;
    domain->tx_size = 0;
    domain->data = NULL;
    domain->data_origin = EC_ORIG_INTERNAL;
    domain->logical_base_address = 0x00000000;
    INIT_LIST_HEAD(&domain->datagrams);
    domain->working_counter = 0x0000;
    domain->expected_working_counter = 0x0000;
    domain->working_counter_changes = 0;
    domain->notify_jiffies = 0;
}

/*****************************************************************************/

/** Domain destructor.
 */
void ec_domain_clear(ec_domain_t *domain /**< EtherCAT domain */)
{
    ec_datagram_t *datagram, *next;

    // dequeue and free datagrams
    list_for_each_entry_safe(datagram,ec_datagram_t, next, &domain->datagrams, list) {
        ec_datagram_clear(datagram);
#ifdef DYN_MEM_ALLOC
        ec_free(datagram);
#endif
    }

    ec_domain_clear_data(domain);
}

/*****************************************************************************/

/** Frees internally allocated memory.
 */
void ec_domain_clear_data(
        ec_domain_t *domain /**< EtherCAT domain. */
        )
{
    if (domain->data_origin == EC_ORIG_INTERNAL && domain->data) {
#ifdef DYN_MEM_ALLOC
        ec_free(domain->data);
#else
		buffer_pool_put(domain->data);
#endif
    }
    domain->data = NULL;
    domain->data_origin = EC_ORIG_INTERNAL;
}

/*****************************************************************************/

/** Adds an FMMU configuration to the domain.
 */
void ec_domain_add_fmmu_config(
        ec_domain_t *domain, /**< EtherCAT domain. */
        ec_fmmu_config_t *fmmu /**< FMMU configuration. */
        )
{
    fmmu->domain = domain;

    domain->data_size += fmmu->data_size;
    domain->tx_size += fmmu->tx_size;
    list_add_tail(&fmmu->list, &domain->fmmu_configs);

    EC_MASTER_DBG(domain->master, 1, "Domain %u:"
            " Added %u bytes, total %zu.\n",
            domain->index, fmmu->data_size, domain->data_size);
}

/*****************************************************************************/

/** Allocates a domain datagram and appends it to the list.
 *
 * The datagram type and expected working counters are determined by the
 * number of input and output fmmus that share the datagram.
 *
 * \retval  0 Success.
 * \retval <0 Error code.
 */
int ec_domain_add_datagram(
        ec_domain_t *domain, /**< EtherCAT domain. */
        uint32_t logical_offset, /**< Logical offset. */
        size_t data_size, /**< Size of the data. */
        uint8_t *data, /**< Process data. */
        const unsigned int used[] /**< Slave config counter for in/out. */
        )
{
    ec_datagram_t *datagram;
    int ret;
#ifdef DYN_MEM_ALLOC
#ifdef EC_DYN_MEM_PRT
	_DBG_PRINTF_("%d in %s, ec_malloc size=%d\n", __LINE__, __FILE__, sizeof(ec_datagram_t));
#endif
    if (!(datagram = (ec_datagram_t *)ec_malloc(sizeof(ec_datagram_t), 0))) {
        EC_MASTER_ERR(domain->master,
                "Failed to allocate domain datagram!\n");
        return -ENOMEM;
    }
#else
	datagram = domain_dgram_get();
	if (!datagram) {
		EC_MASTER_ERR(domain->master,
				"Failed to allocate domain datagram!\n");
		return -ENOMEM;
	}
#endif
    ec_datagram_init(datagram);
    snprintf(datagram->name, EC_DATAGRAM_NAME_SIZE,
            "domain%u-%u", domain->index, logical_offset);

    if (used[EC_DIR_OUTPUT] && used[EC_DIR_INPUT]) { // inputs and outputs
        ret = ec_datagram_lrw(datagram, logical_offset, data_size, data);
#ifdef DYN_MEM_ALLOC
        if (ret < 0) {
            ec_free(datagram);
            return ret;
        }
#else
		if (ret < 0) {
			_DBG_PRINTF_("Error(%d in %s): No space for datagram!\n", __LINE__, __FILE__);
			return ret;
		}
		
#endif
        // If LRW is used, output FMMUs increment the working counter by 2,
        // while input FMMUs increment it by 1.
        domain->expected_working_counter +=
            used[EC_DIR_OUTPUT] * 2 + used[EC_DIR_INPUT];
    } else if (used[EC_DIR_OUTPUT]) { // outputs only
        ret = ec_datagram_lwr(datagram, logical_offset, data_size, data);
#ifdef DYN_MEM_ALLOC
        if (ret < 0) {
            ec_free(datagram);
            return ret;
        }
#else
		if (ret < 0) {
			_DBG_PRINTF_("Error(%d in %s): No space for datagram!\n", __LINE__, __FILE__);
			return ret;
		}
				
#endif
        domain->expected_working_counter += used[EC_DIR_OUTPUT];
    } else { // inputs only (or nothing)
        ret = ec_datagram_lrd(datagram, logical_offset, data_size, data);
#ifdef DYN_MEM_ALLOC
        if (ret < 0) {
            ec_free(datagram);
            return ret;
        }
#else
		if (ret < 0) {
			_DBG_PRINTF_("Error(%d in %s): No space for datagram!\n", __LINE__, __FILE__);
			return ret;
		}						
#endif
        domain->expected_working_counter += used[EC_DIR_INPUT];
    }

    ec_datagram_zero(datagram);
    list_add_tail(&datagram->list, &domain->datagrams);
    datagram->domain = domain;
    return 0;
}

/*****************************************************************************/

/** Domain finish helper function.
 *
 * Detects, if a slave configuration has already been taken into account for
 * a datagram's expected working counter calculation.
 *
 * Walks through the list of all FMMU configurations for the current datagram
 * and ends before the current datagram.
 */
int shall_count(
        const ec_fmmu_config_t *cur_fmmu, /**< Current FMMU with direction to
                                            search for. */
        const ec_fmmu_config_t *first_fmmu /**< Datagram's first FMMU. */
        )
{
    for (; first_fmmu != cur_fmmu;
            first_fmmu = list_entry(first_fmmu->list.next,
                ec_fmmu_config_t, list)) {

        if (first_fmmu->sc == cur_fmmu->sc
                && first_fmmu->dir == cur_fmmu->dir) {
            return 0; // was already counted
        }
    }

    return 1;
}

/*****************************************************************************/

/** Finishes a domain.
 *
 * This allocates the necessary datagrams and writes the correct logical
 * addresses to every configured FMMU.
 *
 * \todo Check for FMMUs that do not fit into any datagram.
 *
 * \retval  0 Success
 * \retval <0 Error code.
 */
int ec_domain_finish(
        ec_domain_t *domain, /**< EtherCAT domain. */
        uint32_t base_address /**< Logical base address. */
        )
{
    uint32_t datagram_offset;
    size_t datagram_size;
    unsigned int datagram_count;
    unsigned int datagram_used[EC_DIR_COUNT];
    ec_fmmu_config_t *fmmu;
    const ec_fmmu_config_t *datagram_first_fmmu = NULL;
    const ec_datagram_t *datagram;
    int ret;

    domain->logical_base_address = base_address;

    if (domain->data_size && domain->data_origin == EC_ORIG_INTERNAL) {
#ifdef DYN_MEM_ALLOC
#ifdef EC_DYN_MEM_PRT
		_DBG_PRINTF_("%d in %s, ec_malloc size=%d\n", __LINE__, __FILE__, domain->data_size);
#endif
        if (!(domain->data =
                    (uint8_t *) ec_malloc(domain->data_size, 0))) {
            EC_MASTER_ERR(domain->master, "Failed to allocate %zu bytes"
                    " internal memory for domain %u!\n",
                    domain->data_size, domain->index);
            return -ENOMEM;
        }
#else
		//if (!(domain->data =buffer_pool_get())) {
		domain->data =buffer_pool_get(0);
		if (domain->data == NULL) {
			EC_MASTER_ERR(domain->master, "Failed to allocate %zu bytes"
					" internal memory for domain %u!\n",
					domain->data_size, domain->index);
			return -ENOMEM;
		}
			
#endif
    }

    // Cycle through all domain FMMUs and
    // - correct the logical base addresses
    // - set up the datagrams to carry the process data
    // - calculate the datagrams' expected working counters
    datagram_offset = 0;
    datagram_size = 0;
    datagram_count = 0;
    datagram_used[EC_DIR_OUTPUT] = 0;
    datagram_used[EC_DIR_INPUT] = 0;

    if (!list_empty(&domain->fmmu_configs)) {
        datagram_first_fmmu =
            list_entry(domain->fmmu_configs.next, ec_fmmu_config_t, list);
    }

    list_for_each_entry(fmmu,ec_fmmu_config_t, &domain->fmmu_configs, list) {

        // Correct logical FMMU address
        fmmu->logical_start_address += base_address;
        fmmu->domain_address += base_address;

        // Increment input/output counter to determine datagram types
        // and calculate expected working counters
        if (shall_count(fmmu, datagram_first_fmmu)) {
            datagram_used[fmmu->dir]++;
        }

        // If the current FMMU's data do not fit in the current datagram,
        // allocate a new one.
        if (datagram_size + fmmu->tx_size > EC_MAX_DATA_SIZE) {
            ret = ec_domain_add_datagram(domain,
                    domain->logical_base_address + datagram_offset,
                    datagram_size, domain->data + datagram_offset,
                    datagram_used);
            if (ret < 0)
                return ret;

            datagram_offset += datagram_size;
            datagram_size = 0;
            datagram_count++;
            datagram_used[EC_DIR_OUTPUT] = 0;
            datagram_used[EC_DIR_INPUT] = 0;
            datagram_first_fmmu = fmmu;
        }

        datagram_size += fmmu->tx_size;
    }

    // Allocate last datagram, if data are left (this is also the case if the
    // process data fit into a single datagram)
    if (datagram_size) {
        ret = ec_domain_add_datagram(domain,
                domain->logical_base_address + datagram_offset,
                datagram_size, domain->data + datagram_offset,
                datagram_used);
        if (ret < 0)
            return ret;
        datagram_count++;
    }

    EC_MASTER_INFO(domain->master, "Domain%u: Logical address 0x%08x,"
            " %zu byte, expected working counter %u.\n", domain->index,
            domain->logical_base_address, domain->data_size,
            domain->expected_working_counter);
    list_for_each_entry(datagram,ec_datagram_t, &domain->datagrams, list) {
        EC_MASTER_INFO(domain->master, "  Datagram %s: Logical offset 0x%08x,"
                " %zu byte, type %s.\n", datagram->name,
                EC_READ_U32(datagram->address), datagram->data_size,
                ec_datagram_type_string(datagram));
    }
    
    return 0;
}

/*****************************************************************************/

/** Get the number of FMMU configurations of the domain.
 */
unsigned int ec_domain_fmmu_count(const ec_domain_t *domain)
{
    const ec_fmmu_config_t *fmmu;
    unsigned int num = 0;

    list_for_each_entry(fmmu,ec_fmmu_config_t, &domain->fmmu_configs, list) {
        num++;
    }

    return num;
}

/*****************************************************************************/

/** Get a certain FMMU configuration via its position in the list.
 */
const ec_fmmu_config_t *ec_domain_find_fmmu(
        const ec_domain_t *domain, /**< EtherCAT domain. */
        unsigned int pos /**< List position. */
        )
{
    const ec_fmmu_config_t *fmmu;

    list_for_each_entry(fmmu,ec_fmmu_config_t, &domain->fmmu_configs, list) {
        if (pos--)
            continue;
        return fmmu;
    }

    return NULL;
}

/******************************************************************************
 *  Application interface
 *****************************************************************************/

int ecrt_domain_reg_pdo_entry_list(ec_domain_t *domain,
        const ec_pdo_entry_reg_t *regs)
{
    const ec_pdo_entry_reg_t *reg;
    ec_slave_config_t *sc;
    int ret;
    
    EC_MASTER_DBG(domain->master, 1, "ecrt_domain_reg_pdo_entry_list("
            "domain = 0x%p, regs = 0x%p)\n", domain, regs);

    for (reg = regs; reg->index; reg++) {
        sc = ecrt_master_slave_config_err(domain->master, reg->alias,
                reg->position, reg->vendor_id, reg->product_code, 0);
        if (IS_ERR(sc))
            return PTR_ERR(sc);

        ret = ecrt_slave_config_reg_pdo_entry(sc, reg->index,
                        reg->subindex, domain, reg->bit_position);
        if (ret < 0)
            return ret;

        *reg->offset = ret;
    }

    return 0;
}

/*****************************************************************************/

size_t ecrt_domain_size(const ec_domain_t *domain)
{
    return domain->data_size;
}

/*****************************************************************************/

void ecrt_domain_external_memory(ec_domain_t *domain, uint8_t *mem)
{
    EC_MASTER_DBG(domain->master, 1, "ecrt_domain_external_memory("
            "domain = 0x%p, mem = 0x%p)\n", domain, mem);
    ec_domain_clear_data(domain);

    domain->data = mem;
    domain->data_origin = EC_ORIG_EXTERNAL;
}

/*****************************************************************************/

uint8_t *ecrt_domain_data(ec_domain_t *domain)
{
    return domain->data;
}

/*****************************************************************************/

void ecrt_domain_process(ec_domain_t *domain)
{
    uint16_t working_counter_sum;
    ec_datagram_t *datagram;

    working_counter_sum = 0x0000;
    list_for_each_entry(datagram,ec_datagram_t, &domain->datagrams, list) {
        ec_datagram_output_stats(datagram);
        if (datagram->state == EC_DATAGRAM_RECEIVED) {
            working_counter_sum += datagram->working_counter;
        }
    }

    if (working_counter_sum != domain->working_counter) {
        domain->working_counter_changes++;
        domain->working_counter = working_counter_sum;
    }

    if (domain->working_counter_changes &&
        jiffies - domain->notify_jiffies > HZ) {
        domain->notify_jiffies = jiffies;
        if (domain->working_counter_changes == 1) {
            EC_MASTER_INFO(domain->master, "Domain %u: Working counter"
                    " changed to %u/%u.\n", domain->index,
                    domain->working_counter, domain->expected_working_counter);
        } else {
            EC_MASTER_INFO(domain->master, "Domain %u: %u working counter"
                    " changes - now %u/%u.\n", domain->index,
                    domain->working_counter_changes, domain->working_counter,
                    domain->expected_working_counter);
        }
        domain->working_counter_changes = 0;
    }
}

/*****************************************************************************/

void ecrt_domain_queue(ec_domain_t *domain)
{
    ec_datagram_t *datagram;

    list_for_each_entry(datagram,ec_datagram_t, &domain->datagrams, list) {
        ec_master_queue_datagram(domain->master, datagram);
    }
}

/*****************************************************************************/

void ecrt_domain_state(const ec_domain_t *domain, ec_domain_state_t *state)
{
    state->working_counter = domain->working_counter;

    if (domain->working_counter) {
        if (domain->working_counter == domain->expected_working_counter) {
            state->wc_state = EC_WC_COMPLETE;
        } else {
            state->wc_state = EC_WC_INCOMPLETE;
        }
    } else {
        state->wc_state = EC_WC_ZERO;
    }
}

/*****************************************************************************/
