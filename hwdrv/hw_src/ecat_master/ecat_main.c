#include "ec_mm.h"
#include "ec_globals.h"
#include "types.h"
#include "ecrt.h"
#include "debug.h"
#include "domain.h"
#include "master.h"
#include "sync_config.h"
#include "slave_config.h"
#include "sync.h"
#include "app_driver_func.h"
#include "app_data.h"
#include "config.h"
//#include "Lpc43xx_timer.h"


extern struct ec_mm_poll_struct ec_mm_pool;
extern struct ec_mm_poll_struct *ec_mm_pool_ptr;
extern ec_master_t	            *masters;	//all masters which can be used here
extern unsigned int master_count; /**< Number of masters. */

extern u8 *app_rxpdo_buf[];

extern int loop_counter;
extern int endless_chk_start;
/*APP PDO interface*/
extern ec_datagram_t *app_rxpdo_dgram[];
extern app_ecat_interface_t *ec_app_inter[];

#define MASTER_IDX              0 //master index
#define				            DATA_BUF_LEN 0x40
u8					            datagram0_data[DATA_BUF_LEN];
u8					            datagram1_data[DATA_BUF_LEN];
u32 		                    *ecat_data_base0 = NULL;
u32 		                    *ecat_data_base1 = NULL;
ec_datagram_t                   datagram0;
ec_datagram_t                   datagram1;
ec_master_t 		            *master;	//selected master

#define	NUM_EC_TASK				2			//total number of EtherCAT Task
app_ecat_interface_t            *ec_inter[NUM_EC_TASK];

u8                              *domain0_pd;
u8                              *domain1_pd;
int                             run=0;
static unsigned int             counter = 0;
static unsigned int             blink = 0;
#define DOMAIN_SEL	0
//#define DIO_BLINK              //Digital IO Blink Test
//#define READ_SM                       //Read SyncManager Test
//#define READ_FMMU                   //Read FMMU Test
//#define DPRAM_TEST                  //Read DPRAM Test
//#define EEPROM_ACCESS          //EEPROM Read/Write Test
//#define SDO_ACCESS              //SDO ACCESS
#define PDO_ACCESS              //PDO ACCESS
//#define DC_0x0900            //Write and read registers of 0x0900 from all slaves

//0, 汇川的原始数据(EEPROM中的第0x0--0x6的16-bit字)
//1, FB1111-0142的原始配置数据
//2, EEPROM Test
//#define EEPROM_CFG_SEL 2


#define TEST_CNT 60000000         //Test finish counter
#define BLINK_NUM 500

#define PDO_CYCLE 50              //PDO cycle in Timer Tick

#define TIMING_CHK_DEPTH    32

//#ifdef PDO_LOSS_RATE
//rx/tx PDO statistics
extern u32 rxpdo_send_cnt, rxpdo_rec_cnt;
extern u32 txpdo_send_cnt, txpdo_rec_cnt;
extern u32 rxpdo_queue, rxpdo_sent;
extern u32 txpdo_queue, txpdo_sent;
extern u32 rxpdo_miss;

#endif


extern u64 test_buf[][2];
extern int test_buf32[][3];
extern u32 idx;

u16 TempDatax[8];
u16 TempDatax2[8];

#ifdef ECAT_TIME_TEST
#define TEST_BUFFER_DEP	128
int test_idx = 0;
int test_start = 0;
u32 test_rec_time[6];
#endif
void ecat_test(void *arg)
{
	int counter = 0;
	u32 cnt_timer = 0;
	int divcounter = 0;
	int divider = 10;
	int ret;
    u16 con_sta;
    int i;
    u16 acc;
    u16 op_size = 2;
	u16 data_off = 0;
    u16 ecat_op_done = 1;
	u32	ecat_op_flag = 0;
	u32	ustick_jiffies;
#ifdef ECAT_TIME_TEST	
	u32 ecat_timer_buf[TEST_BUFFER_DEP][6];
#endif
	u32 timer_start;
#if EEPROM_CFG_SEL == 0
    u8 src_buf[DATA_BUF_LEN] = {0x08, 0x0e, 0x00, 0x04, 0x0A, 0x00, 0x00, 0x00, \
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x00};
#elif EEPROM_CFG_SEL == 1
    u8 src_buf[DATA_BUF_LEN] = {0x04, 0x0f, 0x00, 0x44 0x10 0x27 0x00, 0xff \
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEE, 0x00};
#elif EEPROM_CFG_SEL == 2
    u8 src_buf[DATA_BUF_LEN] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, \
                                0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
#endif
    u32 data_cnt;
    app_ecat_interface_t            *ec_inter_sel;
#if defined(READ_SM)
    u16 dpram_addr = 0x800;
    int data_size = 8*8;
#elif defined(READ_FMMU)
    u16 dpram_addr = 0x600;
    int data_size = 16*8;
#elif defined (EEPROM_ACCESS)
    u16 dpram_addr = 0x502;
    int data_size = 2;
    u16 word_offset = 0x000;
#elif defined (DIO_BLINK)
    u16 dpram_addr = 0xF00;
    int data_size = 4;
#elif defined(SDO_ACCESS)
   u16 dpram_addr = 0x1c12;
   int data_size = 0x4;
#elif defined(PDO_ACCESS)
    u16 rxpdo_idx = 0x1600;
    u16 rxpdo_size = 0x6;
    u16 txpdo_idx = 0x1a00;
    u16 txpdo_size = 0x6;
#elif defined(DC_0x0900)
    u16 dpram_addr = 0x0980;
    int data_size = 0x30;
	u32 tmp_0x900_buf0[1024];
	u32 tmp_0x900_buf1[1024];
	u8	*tmp_buf0_ptr = (u8 *)tmp_0x900_buf0;
	u8	*tmp_buf1_ptr = (u8 *)tmp_0x900_buf1;
	int test_count = 0;
#else
    u16 dpram_addr = 0x0140;
    int data_size = 0x20;
#endif
    
	static unsigned int blink_reg = 0;
    static unsigned int jiffies_reg = 0;
    EcatReq_t ecatreq;
    //For Timing Check
    u32 timing_chk_buf[TIMING_CHK_DEPTH];
    u32 timing_chk_cnt  = 0;
    u64 jiffies_64_reg  = 0;
    u32 pdo_cycle_ok    = 0;
    u32 jiffies_start[10], jiffies_end[10];
    int jiff_idx = 0;
    //used for timer adjust test
    int timer_adj_buf[2048][2];
    int timer_adj_idx = 0;
	run         			= 1;
    domain0_pd  			= datagram0_data;
	domain1_pd  			= datagram1_data;
    for(i = 0; i < DATA_BUF_LEN; i++)
    {
        domain0_pd[i]   	= 0;
        domain1_pd[i]   	= 0;
    }
    
    blink       			= 0;
    acc                     = DEV_OP_READ;
    data_cnt                = 0;
	uart_printf("Start %s\n", __func__);
    if (master->slave_count > 1)
        ec_inter_sel            = ec_inter[1];
    else
        ec_inter_sel            = ec_inter[0];        
#ifdef ECAT_TIME_TEST
	test_start = 1;
#endif
	//master_dbg_config(3);
	ecat_master_set(1);

    while (run) {
		ecat_op_flag = 0;
 		ustick_jiffies = jiffies;
#if 0
		{
			volatile int tmp = 500000;
			while(tmp--);
		}
#endif
		blink_reg = blink;
        if ((counter % BLINK_NUM)==0)
        {
            blink = !blink;
        }
        ec_memset(&ecatreq, 0, sizeof(EcatReq_t));
        if (DOMAIN_SEL)
        {
            ecatreq.ConWord     = SET_CON_DATA_ACTIVE(ecatreq.ConWord);
            ecatreq.ConWord     = SET_CON_ACCESS(ecatreq.ConWord, DEV_OP_WRITE);
            ecatreq.TransferType= 0;
            ecatreq.EscAddress  = ec_inter_sel->station_addr;
            ecatreq.OdIndex     = 0;
            ecatreq.OdSubIndex  = 0;
            ecatreq.DataLen     = 4;
            //EC_WRITE_U8((ec_inter_sel->data_buff+EC_OFF_BUF_HDR), blink ? 0x0 : 0x0F);
            EC_WRITE_U16((ec_inter_sel->data_buff+EC_OFF_BUF_HDR+2), blink ? (u16)(counter >> 8) : (u16)(counter >> 8));
            if (blink_reg != blink)
                uart_printf("data in domain1_pd is 0x%x...\n", *((unsigned char *)((unsigned int)(ec_inter_sel->data_buff + EC_OFF_BUF_HDR))));
            EcatDataCreate(ec_inter_sel->data_buff, &ecatreq);
        }
        else
        {
#if defined(DIO_BLINK)
            ecatreq.ConWord     = SET_CON_DATA_ACTIVE(ecatreq.ConWord);
            ecatreq.ConWord     = SET_CON_ACCESS(ecatreq.ConWord, DEV_OP_WRITE);
            ecatreq.TransferType= 0;
            ecatreq.EscAddress  = ec_inter_sel->station_addr;
            ecatreq.OdIndex     = dpram_addr;
            ecatreq.OdSubIndex  = 0;
            ecatreq.DataLen     = data_size;
            EcatDataCreate(ec_inter_sel->data_buff, &ecatreq);
            //EC_WRITE_U8((ec_inter_sel->data_buff+EC_OFF_BUF_HDR+2), blink ? 0x0 : 0x5A);
            EC_WRITE_U16((ec_inter_sel->data_buff+EC_OFF_BUF_HDR+2), blink ? 0x5a5a : 0xa5a5);
            //EC_WRITE_U32((ec_inter_sel->data_buff+EC_OFF_BUF_HDR), blink ? 0x55aa55aa : 0xaa55aa55);
            
            if (blink_reg != blink)
            {
                uart_printf("data is 0x%x, jiffies=0x%x...\n", *((unsigned int *)((unsigned int)(ec_inter_sel->data_buff + EC_OFF_BUF_HDR))), \
                                                                (jiffies-jiffies_reg));
                jiffies_reg = jiffies;
            }
#elif defined(READ_SM) || defined(READ_FMMU)
            ecatreq.ConWord     = SET_CON_DATA_ACTIVE(ecatreq.ConWord);
            ecatreq.ConWord     = SET_CON_ACCESS(ecatreq.ConWord, DEV_OP_READ);
            ecatreq.TransferType= 0;
            ecatreq.EscAddress  = ec_inter_sel->station_addr;
            ecatreq.OdIndex     = dpram_addr;
            ecatreq.OdSubIndex  = 0;
            ecatreq.DataLen     = data_size;
            EcatDataCreate(ec_inter_sel->data_buff, &ecatreq);
            for(i = 0; i < data_size; i=i + 4)
                EC_WRITE_U32((ec_inter_sel->data_buff+EC_OFF_BUF_HDR + i), 0);
            
#elif defined(EEPROM_ACCESS)
            data_size           = 16;
            if(acc == DEV_OP_READ)
            {
                op_size = (data_size > 4) ? 8 : data_size;
                ec_memset(ec_inter_sel->data_buff + EC_OFF_BUF_HDR, 0x0, op_size);
            }
            else if (acc == DEV_OP_WRITE)
            {
                op_size = 2;
                ec_memcpy(ec_inter_sel->data_buff + EC_OFF_BUF_HDR, &(src_buf[data_cnt]), op_size);
            }
            ecatreq.ConWord     = SET_CON_DATA_ACTIVE(ecatreq.ConWord);
            ecatreq.ConWord     = SET_CON_ACCESS(ecatreq.ConWord, acc);
            ecatreq.TransferType= (0x1 << 12); //EEPROM Access needs more one EtherCAT cycles
            ecatreq.EscAddress  = ec_inter_sel->station_addr;
            ecatreq.OdIndex     = dpram_addr;
            ecatreq.OdSubIndex  = word_offset + ((data_cnt+1)/2);
            ecatreq.DataLen     = op_size;
            EcatDataCreate(ec_inter_sel->data_buff, &ecatreq);
#elif defined(SDO_ACCESS)
            if (ecat_op_done == 1)
            {
            	{
                	u32 *app_rxpdo_buf32;
                    if (ec_inter_sel	== ec_inter[1])
                        app_rxpdo_buf32 = (u32 *)app_rxpdo_buf[1];
                    else
                        app_rxpdo_buf32 = (u32 *)app_rxpdo_buf[0];
                        
					static int pdo_cnt = 0;
                	*app_rxpdo_buf32 = pdo_cnt++;
            	}
                if (counter%4 == 0)
                {
                    ecatreq.OdIndex     = dpram_addr;
                    ecatreq.OdSubIndex  = 0x0 << 8;
                }
                else if (counter%4  == 1)
                {
                    ecatreq.OdIndex     = dpram_addr;
                    ecatreq.OdSubIndex  = 0x1 << 8;
                }
                else if (counter%4  == 2)
                {
                    ecatreq.OdIndex     = dpram_addr+1;
                    ecatreq.OdSubIndex  = 0x0 << 8;
                }
                else if (counter%4 == 3)
                {
                    ecatreq.OdIndex     = dpram_addr+1;
                    ecatreq.OdSubIndex  = 0x1 << 8;
                }
                ecatreq.OdIndex     = 0x60FF;
                ecatreq.OdSubIndex  = 0x0 << 8;
                
    		    acc					= DEV_OP_READ;
    		    //acc					= DEV_OP_WRITE;
                op_size             = 2;
                ecatreq.ConWord     = SET_CON_DATA_ACTIVE(ecatreq.ConWord);
                ecatreq.ConWord     = SET_CON_ACCESS(ecatreq.ConWord, acc);
                ecatreq.TransferType= (0x1 << 12); //SDO Access needs more one EtherCAT cycles
                ecatreq.TransferType|= (EC_MBX_COE << 8) | 0x01;
                ecatreq.EscAddress  = ec_inter_sel->station_addr;
                ecatreq.DataLen     = op_size;
                EcatDataCreate(ec_inter_sel->data_buff, &ecatreq);
            }
#elif defined(PDO_ACCESS)
#if 1
/*
            app_rxpdo_dgram[0]->data[0] = (u8)counter;
            app_rxpdo_dgram[0]->data[1] = 0x5a;
            app_rxpdo_dgram[0]->data[2] = 0x00;
            app_rxpdo_dgram[0]->data[3] = 0x00;
            app_rxpdo_dgram[0]->data[4] = 0x00;
            app_rxpdo_dgram[0]->data[5] = 0x00;
*/
		rx_domain_dgram.data[0] = (u8)counter;
		rx_domain_dgram.data[1] = 0x5a;
		rx_domain_dgram.data[2] = 0x00;
		rx_domain_dgram.data[3] = 0x00;
		rx_domain_dgram.data[4] = 0x00;
		rx_domain_dgram.data[5] = 0x00;

#else
			app_rxpdo_dgram[0]->data[4] = (u8)counter;
			app_rxpdo_dgram[0]->data[5] = 0x5a;
			app_rxpdo_dgram[0]->data[2] = 0x00;
			app_rxpdo_dgram[0]->data[3] = 0x00;
			app_rxpdo_dgram[0]->data[0] = 0x00;
			app_rxpdo_dgram[0]->data[1] = 0x00;
#endif
			if ((jiffies - cnt_timer) > HZ/4)
			{
				counter++;
				cnt_timer = jiffies;
				}
            pdo_cycle_ok = 0;
#elif defined(DC_0x0900)
			//ec_inter_sel 		= ec_inter[0];
            //ec_memset(ec_inter_sel->data_buff, 0x0, 0x400);

            if (counter%2 == 0)
            {
                ecatreq.OdIndex     = 0x910;
                data_size           = 0x10;
            }
            else
            {
                ecatreq.OdIndex     = 0x990;
                data_size           = 0x10;
            }
#if 1 //read time registers only
            acc                 = DEV_OP_READ;
            op_size             = data_size;
#else //update port receive time and read time registers
            acc                 = (counter % 2) ? DEV_OP_WRITE : DEV_OP_READ;
            op_size             = (counter % 2) ? 0x1 : data_size;
#endif
            ecatreq.ConWord     = SET_CON_DATA_ACTIVE(ecatreq.ConWord);
            ecatreq.ConWord     = SET_CON_ACCESS(ecatreq.ConWord, acc);
            ecatreq.TransferType|= 0;
            ecatreq.EscAddress  = ec_inter_sel->station_addr;
            //ecatreq.OdIndex     = dpram_addr;
            ecatreq.OdSubIndex  = 0;
            ecatreq.DataLen     = op_size;
            EcatDataCreate(ec_inter_sel->data_buff, &ecatreq);
#elif defined(DPRAM_TEST) //DPRAM Test
            ec_inter_sel   = ec_inter[0];
            jiffies_64_reg = jiffies_64;
            if (do_div(jiffies_64_reg, PDO_CYCLE) == 0)
                pdo_cycle_ok = 1;
            if (pdo_cycle_ok)
            {
            	//acc					= (counter % 2) ? DEV_OP_READ : DEV_OP_WRITE;
            	acc					= DEV_OP_WRITE;
#ifdef ESTO_E410_SLAVE
                op_size             = 6;
#else
                op_size             = 8;
#endif
                ecatreq.ConWord     = SET_CON_DATA_ACTIVE(ecatreq.ConWord);
                ecatreq.ConWord     = SET_CON_ACCESS(ecatreq.ConWord, acc);
                //ecatreq.ConWord     = SET_CON_BROADCAST(ecatreq.ConWord);
                ecatreq.TransferType= 0;
                ecatreq.EscAddress  = ec_inter_sel->station_addr;
                ecatreq.OdIndex     = 0x1800;
                ecatreq.OdSubIndex  = 0;
                ecatreq.DataLen     = op_size;
                if (pdo_cycle_ok)
                    EcatDataCreate(ec_inter_sel->data_buff, &ecatreq);
                {
                    u8 *tmp_data = ec_inter_sel->data_buff + EC_OFF_BUF_HDR;
					for(i = 0; i < op_size; i++)
					{
                        tmp_data[i] = (i+1) & 0xFF;
					}
                    tmp_data[0] = 0x5a;
                    tmp_data[1] = 0x4b;
                        
                }
            }
#endif
        }
		ecat_slave_reconfig();

        //app_data_process();
#ifdef ECAT_TIME_TEST

#ifdef ECAT_REC_TIME_TEST
		if(test_idx < TEST_BUFFER_DEP)
			test_rec_time[0] = read_timer(DEBUG_TIMER);
		
#endif
		ecat_receive();
#ifdef ECAT_STATE_CHK_TIME_TEST
		if(test_idx < TEST_BUFFER_DEP)
			test_rec_time[0] = read_timer(DEBUG_TIMER);
#endif
		ecat_state_check();
#ifdef ECAT_STATE_CHK_TIME_TEST
		if(test_idx < TEST_BUFFER_DEP)
			test_rec_time[0] = read_timer(DEBUG_TIMER) - test_rec_time[0];
#endif
#ifdef ECAT_QUEUE_TIME_TEST
		if(test_idx < TEST_BUFFER_DEP)
			test_rec_time[0] = read_timer(DEBUG_TIMER);
#endif
		//ecat_queue();
#ifdef ECAT_QUEUE_TIME_TEST
		if(test_idx < TEST_BUFFER_DEP)
			test_rec_time[0] = read_timer(DEBUG_TIMER) - test_rec_time[0];
#endif
#ifdef ECAT_SEND_TIME_TEST
		if(test_idx < TEST_BUFFER_DEP)
			test_rec_time[0] = read_timer(DEBUG_TIMER);
#endif
		ecat_send();

		if(test_idx < TEST_BUFFER_DEP)
		{
			//ecat_timer_buf[test_idx] = read_timer(DEBUG_TIMER) - timer_start;
			ecat_timer_buf[test_idx][0] = test_rec_time[0];
			ecat_timer_buf[test_idx][1] = test_rec_time[1];
			ecat_timer_buf[test_idx][2] = test_rec_time[2];
			ecat_timer_buf[test_idx][3] = test_rec_time[3];
			ecat_timer_buf[test_idx][4] = test_rec_time[4];
			ecat_timer_buf[test_idx][5] = test_rec_time[5];
			test_idx++;
			if (test_idx == TEST_BUFFER_DEP)
			{
				int m;
				_DBG_PRINTF_("Send Time CAL:\n");
				for(m = 0; m < TEST_BUFFER_DEP; m++)
				{
					_DBG_PRINTF_("%d: loc0=%d, loc1=%d, loc2=%d, loc3=%d, loc4=%d\n", m, 
					ecat_timer_buf[m][0], ecat_timer_buf[m][1], ecat_timer_buf[m][2],
					ecat_timer_buf[m][3], ecat_timer_buf[m][4]);
				}
			}
		}

#else
        //ecat_net_op();
		EcatReceive();
        if (ecat_rec_flag)
		{
			ecat_op_flag = 1;
			ecat_rec_flag = 0;
		}
		while( ((jiffies - ustick_jiffies) < pdo_cycle_period()) && (get_jiffies() % pdo_cycle_period()))
		{
			if (ecat_op_flag < 2)
			{
				if (ecat_op_flag == 0)
				{
					EcatReceive();
					if (ecat_rec_flag)
					{
						ecat_op_flag = 1;
						ecat_rec_flag = 0;
					}
				}
				else if (ecat_op_flag == 1)
				{
					EcatProc();	//	将控制指令通过EtherCAT发送
					EcatSend();
					ecat_op_flag = 2;
				}
			}
		}
		
		if (ecat_op_flag < 2)
		{
			EcatSend();
		}

#endif //end of #ifndef 		ECAT_TIME_TEST
        con_sta = *(u16 *)(ec_inter_sel->data_buff + 2);
        if (GET_STA_ERROR(con_sta))
        {
            uart_printf("ECAT APP Access Error: %x\n", GET_STA_ERROR(con_sta));
        }
        ecat_op_done = GET_STA_FINISH(con_sta);

#if defined(READ_SM)
        for(i = 0; i < data_size; i = i + 8)
        {
            uart_printf("SM%d configuration information......\n", i>>3);
            uart_printf("Start Physical Address is 0x%x\n", *((u16 *)(ec_inter_sel->data_buff+EC_OFF_BUF_HDR+i)));
            uart_printf("Length is 0x%x\n",                 *((u16 *)(ec_inter_sel->data_buff+EC_OFF_BUF_HDR+i+2)));
            uart_printf("Control Register is 0x%x\n",       *((u8 *)(ec_inter_sel->data_buff+EC_OFF_BUF_HDR+i+4)));
            uart_printf("Status Register is 0x%x\n",        *((u8 *)(ec_inter_sel->data_buff+EC_OFF_BUF_HDR+i+5)));
            uart_printf("Active Register is 0x%x\n",        *((u8 *)(ec_inter_sel->data_buff+EC_OFF_BUF_HDR+i+6)));
            uart_printf("PDI Control Register is 0x%x\n",   *((u8 *)(ec_inter_sel->data_buff+EC_OFF_BUF_HDR+i+7)));
            uart_printf("****************************\n");
        }
#ifdef DATAGRAM_TIMEOUT_TEST //defined in debug.h
		uart_printf("...TimeOut Datagrams info...\n");
		for(i = 0; i < 16; i = i + 2)
		{
			uart_printf("address=%08x, TimeOut times: %d\n", test_jiff[i], test_jiff[i+1]);
		}
#endif
        while(1);
#elif defined(READ_FMMU)
        for(i = 0; i < data_size; i = i + 16)
        {
            uart_printf("SM%d configuration information......\n", i>>4);
            uart_printf("Logical Start Address is 0x%x\n", *((u32 *)(ec_inter_sel->data_buff+EC_OFF_BUF_HDR+i)));
            uart_printf("Length is 0x%x\n", *((u32 *)(ec_inter_sel->data_buff+EC_OFF_BUF_HDR+i+4)));
            uart_printf("Logical start bit is 0x%x\n", *((u8 *)(ec_inter_sel->data_buff+EC_OFF_BUF_HDR+i+6)));
            uart_printf("Logical Stop bit is 0x%x\n", *((u8 *)(ec_inter_sel->data_buff+EC_OFF_BUF_HDR+i+7)));
            uart_printf("Physical Start Address is 0x%x\n", *((u16 *)(ec_inter_sel->data_buff+EC_OFF_BUF_HDR+i+8)));
            uart_printf("Physical start bit is 0x%x\n", *((u8 *)(ec_inter_sel->data_buff+EC_OFF_BUF_HDR+i+10)));
            uart_printf("Type Register is 0x%x\n", *((u8 *)(ec_inter_sel->data_buff+EC_OFF_BUF_HDR+i+11)));
            uart_printf("Activate Register is 0x%x\n", *((u8 *)(ec_inter_sel->data_buff+EC_OFF_BUF_HDR+i+12)));
            uart_printf("****************************\n");
        }
        while(1);
#elif defined (EEPROM_ACCESS)
        {
            u16 *eeprom_data;
            
            if (acc == DEV_OP_READ)
            {
                if (data_cnt == 0)
                    uart_printf("eeprom content(slave%d): \n", ec_inter_sel->station_addr);
                eeprom_data = (u16 *)(ec_inter_sel->data_buff + EC_OFF_BUF_HDR);
                uart_printf("%x: ", word_offset + data_cnt);
                for(i = 0; i < (op_size/2); i++)
                {
                    uart_printf("%04x ", eeprom_data[i]);
                }
                uart_printf("\n");
                data_cnt += op_size;
                if (data_cnt >= data_size)
                {
                    run = 0;
                }
                else
                {
                    //acc = DEV_OP_WRITE;
                }
            }
            else
            {
                data_cnt += op_size;
                if (data_cnt >= data_size)
                {
                    acc = DEV_OP_READ;
                    data_cnt = 0;
                }
            }
        }
#elif defined(DPRAM_TEST)
    if (pdo_cycle_ok)
    {
        if(timing_chk_cnt < TIMING_CHK_DEPTH)
            timing_chk_buf[timing_chk_cnt++] = jiffies;
        else if(timing_chk_cnt == TIMING_CHK_DEPTH)
        {
            uart_printf("\n");
            uart_printf("PDO Access Timing check:\n");
            for(i = 0; i < TIMING_CHK_DEPTH; i = i + 4)
            {
                uart_printf("%x: ", i);
                uart_printf("0x%x, 0x%x, 0x%x, 0x%x\n", timing_chk_buf[i], timing_chk_buf[i+1], timing_chk_buf[i+2], timing_chk_buf[i+3]);
            }
            timing_chk_cnt++;
        }
        if(0)
        {
            for(i = 0; i < op_size; i++)
            {
                uart_printf("%02x ", *((u8 *)(ec_inter_sel->data_buff+EC_OFF_BUF_HDR + i + data_off)));
            }
			uart_printf("\n");
			data_cnt += op_size;
			data_cnt = 0;
        }
    }
    pdo_cycle_ok = 0;
#elif defined(SDO_ACCESS)
 #if 1
        if (ecat_op_done)
        {
            op_size = *(u16 *)(ec_inter_sel->data_buff + EC_OFF_LENGTH);
            //if(acc == DEV_OP_READ)
            if(GET_CON_ACCESS(*(u16 *)(ec_inter_sel->data_buff + EC_OFF_CONTROL)) == DEV_OP_READ)
            {
                uart_printf("ESC%d, SDO(index=0x%x, subindex=0x%x): ", *(u16 *)(ec_inter_sel->data_buff+EC_OFF_SLV_ADDR),\
                    *(u16 *)(ec_inter_sel->data_buff+EC_OFF_INDEX), *(u8 *)(ec_inter_sel->data_buff+EC_OFF_SUBINDEX + 1));
                for(i = 0; i < op_size; i++)
                {
                    uart_printf("0x%x, ", *(u8 *)(ec_inter_sel->data_buff+EC_OFF_BUF_HDR + i));
                }
                uart_printf("\n");
            }
            else
            {
                uart_printf("SDO(index=0x%x, subindex=0x%x) write finised\n", \
			    *(u16 *)(ec_inter_sel->data_buff+EC_OFF_INDEX), *(u16 *)(ec_inter_sel->data_buff+EC_OFF_SUBINDEX));
            }
        }
#endif
    counter++;
#elif defined(PDO_ACCESS)
		op_size = *(u16 *)(ec_inter_sel->data_buff + EC_OFF_LENGTH)-EC_OFF_BUF_HDR;
        //if (GET_CON_ACCESS(*(u16 *)(ec_inter_sel->data_buff + EC_OFF_CONTROL)) == DEV_OP_READ)
        if (0)
        {
			uart_printf("PDO(index=0x%x, subindex=0x%x): ", \
				*(u16 *)(ec_inter_sel->data_buff+EC_OFF_INDEX), *(u16 *)(ec_inter_sel->data_buff+EC_OFF_SUBINDEX));
			for(i = 0; i < op_size; i++)
			{
				uart_printf("0x%x, ", *(u8 *)(ec_inter_sel->data_buff+EC_OFF_BUF_HDR + i));
			}
			uart_printf("\n");
		}
#elif defined(DC_0x0900)

        if (acc == DEV_OP_READ)
        {
			u32 *tmp_buf0_ptr32;
            tmp_buf0_ptr32 = (u32 )(ec_inter[0]->data_buff+EC_OFF_BUF_HDR);
			uart_printf("%x:\n", *(u16 *)(ec_inter[0]->data_buff+EC_OFF_INDEX));
            for(i = 0; i < data_size/4; i=i+4)
            {
    			uart_printf("%08x %08x %08x %08x\n",tmp_buf0_ptr32[i+0],  tmp_buf0_ptr32[i+1],  tmp_buf0_ptr32[i+2],  tmp_buf0_ptr32[i+3]);
            }
            uart_printf("\n");
            if (counter == 99)
				run = 0;
        }
#endif //end of "#if defined(READ_SM)"
	
	}
}
#if 1
int start_dbg = 0;
#endif

int ecat_main()
{
	int i;
	u32 re_adjust;
	timer_init_spec(DEBUG_TIMER, 0, 1);
	ecat_master_init();
	/*********add Application configuration data**********/
	uart_printf("%s, ^_^  start ECAT Application  ^_^\n", __func__);
    master = masters;
    //clear Watch dog register

	ec_datagram_init(&datagram0);
	datagram0.data_origin = EC_ORIG_EXTERNAL;
	datagram0.data = datagram0_data;
	
	ec_datagram_init(&datagram1);
	datagram1.data_origin = EC_ORIG_EXTERNAL;
	datagram1.data = datagram1_data;
	if (ec_task_e410)
	{
    	ec_inter[0] = ec_app_inter[0];
    	ec_inter[0]->data_buff  = datagram0_data;
		uart_printf("Slave in e410(slave_num=%d):\n", master->slave_count);
		for(i = 0; i < ec_task_e410->sc_num; i++)
			uart_printf("slave%d, %x\n", i, ec_inter[i]->station_addr);
	}
	else
	{
    	ec_inter[0] = ec_app_inter[0];
    	//ec_inter[1] = ec_app_inter[1];
    	ec_inter[0]->data_buff  = datagram0_data;
    	//ec_inter[1]->data_buff  = datagram1_data;
		uart_printf("Slave in ec_task_hc:\n");
		for(i = 0; i < ec_task_hc->sc_num; i++)
			uart_printf("slave%d, %x\n", i, ec_inter[i]->station_addr);
	}

#ifdef DC_0x0900
#if 1
	start_dbg = 1;
#endif
    for(i = 0; i < master_count;i++)
	    ecat_dc_update_static(&masters[i]);
#endif

#ifdef TIME_TEST_CAL
    set_timer_fast();
#endif
#if defined(DIO_BLINK)
    ec_datagram_fpwr(&datagram1,ec_inter[0]->station_addr, 0x420, 0x2);
    ec_datagram_zero(&datagram1);
    ec_master_queue_datagram(master, &datagram1);
    ecrt_master_send(master);
	do
	{
    	ecrt_master_receive(master);
	}while(datagram1.state != EC_DATAGRAM_RECEIVED);
	datagram1.state = EC_DATAGRAM_INIT;
	
    ec_datagram_fprd(&datagram1,ec_inter[0]->station_addr, 0x420, 0x2);
    ec_datagram_zero(&datagram1);
    ec_master_queue_datagram(master, &datagram1);
    ecrt_master_send(master);
	do
	{
    	ecrt_master_receive(master);
	}while(datagram1.state != EC_DATAGRAM_RECEIVED);
	datagram1.state = EC_DATAGRAM_INIT;
    uart_printf("reg[0x420]=%04x\n", EC_READ_U16(datagram1.data));
#elif defined(DC_0x0900)
	uart_printf("trans delay of slave 0: %08x\n", master->slaves[0].transmission_delay);
	uart_printf("trans delay of slave 1: %08x\n", master->slaves[1].transmission_delay);
#endif

    ecrt_master_receive(master);
	ecat_test(master);
	uart_printf("Hi, here is the END of ecat_main\n");
}
