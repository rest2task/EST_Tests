/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    app_data.c

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:            2013.11.04

Description:    EtherCAT data access Interfaces

Others:

Function List:
                    m_cycle_slave_init: Multiple cycles(one cycle for one EtherCAT Access)
                    app_data_send:      Data sending operation
                    app_data_receive:   Data Receiving operation
                    app_data_process:   Data Processing(Calling app_data_send and app_data_receive)
                    app_sdo_access   :   SDO access function
History:

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/
#include "app_data.h"
#include "app_driver_func.h"

#include "domain.h"
#include "slave_config.h"
#include "list.h"
#include "netdevice.h"
#include "ecrt.h"
#include "debug.h"
#include "Ec_mm.h"
#include "Ecrt.h"


extern unsigned int master_count; //defined in module.c
extern ec_master_t *masters;
extern ec_sdo_request_t app_sdo_request;
m_cycle_slave_t m_cycle_slave_info[M_CYCLE_SLAVE_NUM];

//SDO datagram buffer, used when SDO Access
//u8              sdo_datagram_data[SDO_DATAGRAM_DATA_LEN];
/*external functions declaration*/
void ec_fsm_sii_state_end(ec_fsm_sii_t *);
void ec_fsm_sii_state_error(ec_fsm_sii_t *);
void ec_fsm_coe_end(ec_fsm_coe_t *fsm);
void ec_fsm_coe_error(ec_fsm_coe_t *fsm);
int ecat_eeprom_access(EcatReq_t *app_req, ec_master_t *master);
void app_data_receive_m_cycle(ec_task_t *app_task, ec_datagram_t *datagram);
int ecat_sdo_access(EcatReq_t *app_req, ec_master_t *master, ec_slave_t *slave);

/*
** m_cycle_slave_init: initiazation m_cycle_slave_info
*/
void m_cycle_slave_init()
{
    int i;
    for (i = 0; i < M_CYCLE_SLAVE_NUM; i++)
    {
        m_cycle_slave_info[i].datagram  = NULL;
        m_cycle_slave_info[i].slave     = NULL;
        m_cycle_slave_info[i].flags     = M_CYCLE_TYP_INV;
    }
}
/*
** app_dat_send: send datagrams by register address directory
*/
void app_data_send()
{
    u8                      *app_dataq_buf;
    ec_task_t               *app_task;
    app_ecat_interface_t    *app_interface;
    int                     domain_len;
    EcatReq_t            *app_req;
    u8                      *data_buf;
    int                     i, j;
    ec_slave_t              *slave;
    ec_datagram_t           *datagram;
    ec_task_datagram_t      *task_datagram;

    list_for_each_entry(app_task, ec_task_t, &ec_task_head.ec_tasks, list)
    {
        for (j = 0; j < app_task->sc_num; j++)
        {
            app_interface       = &app_task->app_ecat_inter[j];
            app_dataq_buf       = app_interface->data_buff;
            app_req             = (EcatReq_t *)(app_dataq_buf);
            if ((app_task->task_state) && GET_CON_DATA_ACTIVE(app_req->ConWord))
            {
                app_req->ConWord = SET_STA_ERROR(app_req->ConWord, EC_ERR_TSK_BUSY);
                app_req->ConWord = SET_STA_FINISH(app_req->ConWord);
                app_req->ConWord = CLR_CON_DATA_ACTIVE(app_req->ConWord);
                continue;
            }
            while (GET_CON_DATA_ACTIVE(app_req->ConWord) && (app_req->DataLen >= EC_OFF_BUF_HDR))
            {
                data_buf            = (u8 *)(app_dataq_buf + EC_OFF_BUF_HDR);
                task_datagram       = list_entry(app_task->task_datagrams.next, ec_task_datagram_t, list);
                datagram            = &task_datagram->datagram;
                datagram->data      = data_buf;
#if 1
                if (ec_app_data_p->slave_link_bitmap[(app_req->EscAddress - 1) >> 4] & (1 << (app_req->EscAddress - 1)))
                {
                    app_req->ConWord = SET_STA_ERROR(app_req->ConWord, EC_ERR_NO_SLV);
                    app_req->ConWord = SET_STA_FINISH(app_req->ConWord);
                    app_task->task_state = 0;
                    goto buf_update;
                }
#endif
                if (app_task->domain)
                {
                    app_task->task_state = 1;
                    ecrt_domain_external_memory(app_task->domain, (u8 *)(data_buf));
                    domain_len = 0;
                    list_for_each_entry(datagram, ec_datagram_t, &app_task->domain->datagrams, list)
                    {
                        datagram->data = (u8 *)(app_task->domain->data + domain_len);
                        domain_len += datagram->data_size;
                    }
                }
                else if (app_req->TransferType & 0x1)  //Mailbox Flag
                {
                    switch ((app_req->TransferType & (0xf << 8)) >> 8)
                    {
                        case EC_MBX_ERR:
                        case EC_MBX_AOE:
                        case EC_MBX_EOE:
                            app_req->ConWord = SET_STA_ERROR(app_req->ConWord, EC_ERR_PROTOCOL);
                            app_req->ConWord = SET_STA_FINISH(app_req->ConWord);
                            break;
                        case EC_MBX_COE:
                        {
                            if (app_req->TransferType & (0x1 << 12)) //Multiple cycles of COE for SDO
                            {
                                if (app_req->EscAddress < (app_task->master->slave_count + 1))
                                {
                                    //use task datagram to send SDO data
                                    for (i = 0; i < M_CYCLE_SLAVE_NUM; i++)
                                        if (!m_cycle_slave_info[i].slave)
                                        {
                                            break;
                                        }
                                    if (i < M_CYCLE_SLAVE_NUM)
                                    {
                                        slave = (ec_slave_t *)(app_task->master->slaves + app_req->EscAddress - 1);
                                        m_cycle_slave_info[i].slave     = slave;
                                        m_cycle_slave_info[i].datagram  = app_task->master->fsm.fsm_coe.mbox->datagram;
                                        m_cycle_slave_info[i].flags     = M_CYCLE_TYP_SDO;
                                        app_task->m_clcye_slave         = &m_cycle_slave_info[i];
                                        app_task->master->fsm.fsm_coe.mbox->datagram = datagram;

                                        //turn SDO request to ECAT datagram
                                        list_add_tail(&app_sdo_request.list, &slave->slave_sdo_requests);
                                        app_sdo_request.data_size	= app_req->DataLen - EC_OFF_BUF_HDR;
                                        if (GET_CON_ACCESS(app_req->ConWord) == DEV_OP_WRITE)
                                        {
                                            //ec_memcpy(app_sdo_request.data, datagram->data, app_sdo_request.data_size);
                                            memcpy(app_sdo_request.data, datagram->data, app_sdo_request.data_size);
                                        }
                                        ecat_sdo_access(app_req, app_task->master, slave);
                                        app_task->task_state = 1;
                                    }
                                    else //no more space to keep slave information, Failure
                                    {
                                        m_cycle_slave_init();
                                        app_req->ConWord = SET_STA_ERROR(app_req->ConWord, EC_ERR_SLV_MEM);
                                        app_req->ConWord = SET_STA_FINISH(app_req->ConWord);
                                    }
                                }
                                else
                                {
                                    app_req->ConWord = SET_STA_ERROR(app_req->ConWord, EC_ERR_NO_SLV);
                                    app_req->ConWord = SET_STA_FINISH(app_req->ConWord);
                                }
                            }
                            else //PDO
                            {
//        	                    ec_slave_config_t   *sc;
                                ec_sync_t           *sync;
                                ec_pdo_t            *pdo;
                                ec_pdo_entry_t      *entry;
                                u32                 bit_off;
                                int                 matched = 0;
                                u8                  subindex;
                                size_t              data_size = 0;
                                slave = (ec_slave_t *)(app_task->master->slaves + app_req->EscAddress - 1);
//        	                    sc = slave->config;
                                for (i = 2; i < EC_MAX_SYNC_MANAGERS; i++)
                                {
                                    sync = ec_slave_get_sync(slave, i);

                                    if (sync == NULL)
                                    {
                                        continue;
                                    }
                                    list_for_each_entry(pdo, ec_pdo_t, &(sync->pdos.list), list)
                                    {
                                        if (app_req->OdIndex == pdo->index)
                                        {
                                            matched = 1;
                                            bit_off = 0;
                                            list_for_each_entry(entry, ec_pdo_entry_t, &pdo->entries, list)
                                            {
                                                bit_off += entry->bit_length;
                                                subindex++;
                                            }
                                            data_size = (bit_off + 7) >> 3;
                                        }
                                    }
                                    if (matched)
                                    {
                                        break;
                                    }
                                }
                                if (matched)
                                {
                                    app_req->ConWord = SET_CON_ACCESS(app_req->ConWord, ((sync->control_register >> 2) & 0x3));
                                    app_req->DataLen = data_size + EC_OFF_BUF_HDR; //change to real PDO Length
                                    if (GET_CON_ACCESS(app_req->ConWord) == DEV_OP_READ)
                                    {
                                        ec_datagram_fprd(datagram, app_req->EscAddress, \
                                                         sync->physical_start_address, data_size);
                                        ec_datagram_zero(datagram);
                                    }
                                    else //write
                                    {
                                        ec_datagram_fpwr(datagram, app_req->EscAddress, \
                                                         sync->physical_start_address, data_size);
                                    }
                                    app_task->task_state = 1;
                                }
                            }
                        }
                        case EC_MBX_FOE:
                            break;
                        case EC_MBX_SOE:
                            break;
                        case EC_MBX_VOE:
                            break;
                        default:
                            break;
                    }
                }
                else if (app_req->TransferType & (0x1 << 12)) //Multiple cycles of Non-Mailbox transfer
                {
                    if ((app_req->OdIndex < 0x510) && (app_req->OdIndex > 0x501)) //EEPROM Read or Write
                    {
                        if (app_req->EscAddress < (app_task->master->slave_count + 1))
                        {
                            for (i = 0; i < M_CYCLE_SLAVE_NUM; i++)
                            {
                                if (!m_cycle_slave_info[i].slave)
                                {
                                    break;
                                }
                            }
                            if (i < M_CYCLE_SLAVE_NUM)
                            {
                                slave = (ec_slave_t *)(app_task->master->slaves + app_req->EscAddress - 1);
                                app_task->master->fsm.fsm_sii.slave     = slave;
                                m_cycle_slave_info[i].slave             = slave;
                                m_cycle_slave_info[i].datagram          = app_task->master->fsm.fsm_sii.datagram;
                                m_cycle_slave_info[i].flags             = M_CYCLE_TYP_EEPROM;
                                app_task->m_clcye_slave                 = &m_cycle_slave_info[i];
                                app_task->master->fsm.fsm_sii.datagram  = datagram;
                                //datagram->data                          = data_buf;
                                if (ecat_eeprom_access(app_req, app_task->master))
                                {
                                    app_req->ConWord = SET_STA_ERROR(app_req->ConWord, EC_ERR_EEPROM);
                                    app_req->ConWord = SET_STA_FINISH(app_req->ConWord);
                                }
                                app_task->task_state = 1;
                            }
                            else //no more space to keep slave information, Failure
                            {
                                app_req->ConWord = SET_STA_ERROR(app_req->ConWord, EC_ERR_SLV_MEM);
                                app_req->ConWord = SET_STA_FINISH(app_req->ConWord);
                            }
                        }
                    }
                }
                else
                {
                    if (app_req->DataLen > (ETH_DATA_LEN - EC_OFF_BUF_HDR)) //data length is larger than Ethernet Frame size
                    {
                        app_req->ConWord = SET_STA_ERROR(app_req->ConWord, EC_ERR_LEN);
                        app_req->ConWord = SET_STA_FINISH(app_req->ConWord);
                        app_req->ConWord = CLR_CON_DATA_ACTIVE(app_req->ConWord);
                        break;
                    }
                    app_task->task_state = 1;
                    switch (GET_CON_ACCESS(app_req->ConWord))
                    {
                        case DEV_OP_READ :
                            if (GET_CON_BROADCAST(app_req->ConWord))
                            {
                                ec_datagram_brd(datagram, app_req->OdIndex, app_req->DataLen - EC_OFF_BUF_HDR);
                            }
                            else
                            {
                                ec_datagram_fprd(datagram, app_req->EscAddress, app_req->OdIndex, app_req->DataLen - EC_OFF_BUF_HDR);
                            }
                            ec_datagram_zero(datagram);
                            break;
                        case DEV_OP_WRITE:
                            if (GET_CON_BROADCAST(app_req->ConWord))
                            {
                                ec_datagram_bwr(datagram, app_req->OdIndex, app_req->DataLen - EC_OFF_BUF_HDR);
                            }
                            else
                            {
                                ec_datagram_fpwr(datagram, app_req->EscAddress, app_req->OdIndex, app_req->DataLen - EC_OFF_BUF_HDR);
                            }
                            break;
                        case DEV_OP_RW   :
                            if (GET_CON_BROADCAST(app_req->ConWord))
                            {
                                ec_datagram_brw(datagram, app_req->OdIndex, app_req->DataLen - EC_OFF_BUF_HDR);
                            }
                            else
                            {
                                ec_datagram_fprw(datagram, app_req->EscAddress, app_req->OdIndex, app_req->DataLen - EC_OFF_BUF_HDR);
                            }
                            break;
                        default         :
                            app_req->ConWord = SET_STA_ERROR(app_req->ConWord, EC_ERR_CMD);
                            app_req->ConWord = SET_STA_FINISH(app_req->ConWord);
                            app_task->task_state = 0;
                    }
                }
buf_update:
                app_req->ConWord = CLR_CON_DATA_ACTIVE(app_req->ConWord);
                if (!GET_STA_ERROR(app_req->ConWord))
                {
                    if (app_task->domain) //add datagrams in domain to sending list of master
                    {
                        ecrt_domain_queue(app_task->domain);
                    }
                    else if (!(app_req->TransferType & (0x1 << 12))) //single cycle operation
                    {
                        ec_master_queue_datagram(app_task->master, datagram);
                    }
                }
                task_datagram   = list_entry(task_datagram->list.next, ec_task_datagram_t, list);
                datagram        = &task_datagram->datagram;
                app_dataq_buf   = (u8 *)(app_dataq_buf + ((app_req->DataLen + 1) & (~0x1)));
                app_req         = (EcatReq_t *)app_dataq_buf;
                data_buf        = (u8 *)(app_dataq_buf + EC_OFF_BUF_HDR);
            }
        }
    }
}
/*
** app_dat_receive: receive datagrams
*/
void app_data_receive()
{
    ec_task_t               *app_task;
#ifdef EC_RATE_OPT						
    ec_domain_state_t       domain_state;
#endif							
    ec_datagram_t           *datagram;
    ec_task_datagram_t      *task_datagram;
    EcatReq_t				*app_req;
    list_for_each_entry(app_task, ec_task_t, &ec_task_head.ec_tasks, list)
    {
        //for(j = 0; j < app_task->sc_num; j++)
        {
            if (app_task->task_state)
            {
                //app_interface = &(app_task->app_ecat_inter[i]);
#ifdef EC_RATE_OPT						
                if (app_task->domain) //datagram was sent with domian
                {
                    app_req  = (EcatReq_t *)(app_task->domain->data - EC_OFF_BUF_HDR);
                    ecrt_domain_process(app_task->domain);
                    ecrt_domain_state(app_task->domain, &domain_state);
                    if (domain_state.wc_state == EC_WC_COMPLETE)
                    {
                        app_req->ConWord = SET_STA_FINISH(app_req->ConWord);
                        app_task->task_state = 0;
                    }
                }
                else //No domian, datagram was sent individually
#endif								
                {
                    app_task->task_state = 0;
                    list_for_each_entry(task_datagram, ec_task_datagram_t, &app_task->task_datagrams, list)
                    {
                        datagram                = &task_datagram->datagram;
                        app_req  = (EcatReq_t *)(datagram->data - EC_OFF_BUF_HDR);
                        if (datagram->state == EC_DATAGRAM_RECEIVED)
                        {
                            if (datagram->working_counter == 1)
                            {
                                if (!(app_req->TransferType & (0x1 << 12)))
                                {
                                    datagram->state         = EC_DATAGRAM_INIT;
                                    app_req->ConWord        = SET_STA_FINISH(app_req->ConWord);
                                    app_task->retry_time    = 0;
                                }
                                else
                                {
                                    app_task->task_state    = 1;
                                    app_data_receive_m_cycle(app_task, datagram);
                                }
                            }
                            else
                            {
                                datagram->state         = EC_DATAGRAM_INIT;
                                datagram->skip_count    = 0;
                                app_req->ConWord        = SET_STA_ERROR(app_req->ConWord, EC_ERR_WKC);
                                app_req->ConWord        = SET_STA_FINISH(app_req->ConWord);
                                app_task->retry_time    = 0;

                            }
                        }
                        else if (datagram->state == EC_DATAGRAM_TIMED_OUT)
                        {
                            if (app_task->retry_time >= APP_TASK_RETRY)
                            {
                                datagram->state         = EC_DATAGRAM_INIT;
                                datagram->skip_count    = 0;
                                app_req->ConWord        = SET_STA_ERROR(app_req->ConWord, EC_ERR_TIMEOUT);
                                app_req->ConWord        = SET_STA_FINISH(app_req->ConWord);
                                app_task->retry_time    = 0;
                            }
                            else
                            {
                                datagram->state         = EC_DATAGRAM_INIT;
                                app_task->retry_time++;
                                app_req->ConWord = SET_CON_DATA_ACTIVE(app_req->ConWord);
                            }
                        }
                        else if (datagram->state == EC_DATAGRAM_ERROR)
                        {
                            datagram->state             = EC_DATAGRAM_INIT;
                            datagram->skip_count        = 0;
                            app_req->ConWord            = SET_STA_ERROR(app_req->ConWord, EC_DGRAM_ERROR);
                            app_req->ConWord            = SET_STA_FINISH(app_req->ConWord);
                            app_task->retry_time        = 0;
                        }
                        else if (datagram->state == EC_DATAGRAM_INIT) //first datagram with state EC_DATAGRAM_INIT is the flag of  active datagram end
                        {
                            //break;
                            continue;
                        }
                        else
                        {
                            app_task->task_state = 1;
                        }
                    }
                }
            }
        }
    }
}
/*
** app_dat_receive_m_cycle: receive datagrams which finished in one cycle
*/
void app_data_receive_m_cycle(ec_task_t *app_task, ec_datagram_t *datagram)
{
    EcatReq_t		*app_req;
    m_cycle_slave_t *m_cycle_slave;
//    ec_slave_t      *slave;
    app_req             = (EcatReq_t *)(datagram->data - EC_OFF_BUF_HDR);
    if (app_req->EscAddress  >= (app_task->master->slave_count + 1))
    {
        return;    //No slave matched
    }

//    slave               = (ec_slave_t *)(app_task->master->slaves + app_req->EscAddress - 1);
    m_cycle_slave       = app_task->m_clcye_slave;
    if (m_cycle_slave->flags == M_CYCLE_TYP_SDO)
    {
        if (ec_fsm_coe_exec(&(app_task->master->fsm.fsm_coe)))
        {
            if ((datagram->state != EC_DATAGRAM_QUEUED) && (datagram->state != EC_DATAGRAM_SENT))
            {
                ec_master_queue_datagram(app_task->master, datagram);
            }
        }

        if (app_task->master->fsm.fsm_coe.state == ec_fsm_coe_end) //SDO access finished
        {
#if 1 //gonggw, 2014.09.05, code before 2014.09.05; APP buffer was used request->data directly
            memcpy(datagram->data, app_task->master->fsm.fsm_coe.request->data,
                      app_task->master->fsm.fsm_coe.request->data_size);
#endif
#if 1 //gonggw, 2014.09.03, keep same format as other Access
            app_req->DataLen = app_task->master->fsm.fsm_coe.request->data_size;
#endif
            app_task->master->fsm.fsm_coe.mbox->datagram = m_cycle_slave->datagram;
            m_cycle_slave->slave    = NULL;
            m_cycle_slave->flags    = M_CYCLE_TYP_INV;
            app_task->task_state    = 0;
            app_task->m_clcye_slave = NULL;
            datagram->state         = EC_DATAGRAM_INIT;
            app_req->ConWord        = SET_STA_FINISH(app_req->ConWord);
            app_task->retry_time    = 0;
        }
        else if (app_task->master->fsm.fsm_coe.state == ec_fsm_coe_error) //SDO Process Error
        {

#if 0
            ec_memcpy(datagram->data, slave->fsm.sdo_request->req.abort_code, 4);
#else
#if 0
            _DBG_PRINTF_("SDO Error(data=%x, abort_code=%x)\n", app_sdo_request.data, app_sdo_request.abort_code);
            //_DBG_PRINTF_("request=%x\n",slave->fsm.fsm_coe.request);
            //_DBG_PRINTF_("abort code=%x\n",slave->fsm.fsm_coe.request->abort_code);
            //while(1);
#endif
            memcpy(app_sdo_request.data, &(app_sdo_request.abort_code), 4);
            app_req->DataLen = 4;
#endif
            app_task->master->fsm.fsm_coe.mbox->datagram = m_cycle_slave->datagram;
            m_cycle_slave->slave    = NULL;
            m_cycle_slave->flags    = M_CYCLE_TYP_INV;
            app_task->m_clcye_slave = NULL;
            app_task->task_state    = 0;
            datagram->state         = EC_DATAGRAM_INIT;
            app_req->ConWord        = SET_STA_ERROR(app_req->ConWord, EC_ERR_COE);
            app_req->ConWord        = SET_STA_FINISH(app_req->ConWord);
            app_task->retry_time    = 0;

        }
#if 0
        else
        {
            _DBG_PRINTF_("SDO state=%x\n", app_task->master->fsm.fsm_coe.state);
        }
#endif
    }
    else if (m_cycle_slave->flags == M_CYCLE_TYP_EEPROM)
    {

        if (ec_fsm_sii_exec(&(app_task->master->fsm.fsm_sii)))
        {
            if ((datagram->state != EC_DATAGRAM_QUEUED) && (datagram->state != EC_DATAGRAM_SENT))
            {
                ec_master_queue_datagram(app_task->master, datagram);
            }
        }
        if (app_task->master->fsm.fsm_sii.state == ec_fsm_sii_state_end)
        {
            memcpy(datagram->data, app_task->master->fsm.fsm_sii.value, \
                      app_task->master->fsm.fsm_sii.length);
            app_task->master->fsm.fsm_sii.datagram = m_cycle_slave->datagram;
            m_cycle_slave->slave    = NULL;
            m_cycle_slave->flags    = M_CYCLE_TYP_INV;
            app_task->task_state    = 0;
            app_task->m_clcye_slave = NULL;
            datagram->state         = EC_DATAGRAM_INIT;
            app_req->ConWord        = SET_STA_FINISH(app_req->ConWord);
            app_task->retry_time    = 0;
        }
        else if (app_task->master->fsm.fsm_sii.state == ec_fsm_sii_state_error)
        {
            app_task->master->fsm.fsm_sii.datagram = m_cycle_slave->datagram;
            m_cycle_slave->slave    = NULL;
            m_cycle_slave->flags    = M_CYCLE_TYP_INV;
            app_task->task_state    = 0;
            app_task->m_clcye_slave = NULL;
            datagram->state         = EC_DATAGRAM_INIT;
            app_req->ConWord        = SET_STA_ERROR(app_req->ConWord, EC_ERR_EEPROM);
            app_req->ConWord        = SET_STA_FINISH(app_req->ConWord);
            app_task->retry_time    = 0;
        }
    }
}
void app_data_process()
{
    app_data_send();
    app_data_receive();
}


void app_sdo_access(app_ecat_interface_t *app_interface, u16 index, u8 subindex, u8 *data, u16 datalen, u16 op)
{
    EcatReq_t		*app_req;
    u16             *data_u16 = (u16 *)data;
    app_interface->data_buff = data;
    app_req             = (EcatReq_t *)(app_interface->data_buff);
    app_req->OdIndex    = index;
    app_req->OdSubIndex = subindex << 8;
    app_req->ConWord    = SET_CON_DATA_ACTIVE(app_req->ConWord);
    app_req->ConWord    = SET_CON_ACCESS(app_req->ConWord, op);
    app_req->TransferType = (0x1 << 12) | (EC_MBX_COE << 8) | 0x1;
    app_req->DataLen    = datalen + EC_OFF_BUF_HDR;
    data_u16 = (u16 *)(data + ((app_req->DataLen + 1) & ~0x1));
    *data_u16 = 0;
}
