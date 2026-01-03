
#include <stdio.h>		
#include <string.h>	
#include <stdlib.h>

#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "fsl_enet.h"
#include "fsl_phy.h"
#include "ecat_hal.h" 

 
#include "app_data.h"
#include "errno.h"

#include "eth_op.h"
#include "bsp_led.h"

uint32_t	ECAT_PHY;


/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define	TIMEOUT			100000

#define CARDNAME		"RT1061_EMAC"
#define DRV_VERSION		"1.00"


uint8_t TX_BUF[ETH_FRAG_SIZE]  __ALIGNED(64);
uint8_t RX_BUF[ETH_FRAG_SIZE] __ALIGNED(64);

//unsigned char mac_addr[6]={0x16, 0x16, 0x16, 0x16, 0x16, 0x16};
unsigned char mac_addr[6]={0x0, 0x16, 0xd4, 0x9f, 0xed, 0xa4};
uint8_t emac_data_buf[8];               //gonggw, keep temp data when current description is not the Last Segment
static unsigned short *rptr;
//static unsigned short *tptr;
//static unsigned int TxDescIndex = 0;
//static unsigned int RxDescIndex = 0;

#ifdef EMAC_PKG_RATE
int test_send_pkg_num;
int test_rec_pkg_num;
int test_offset;
#endif
 


 /*******************************************************************************
 * Variables
 ******************************************************************************/
enet_config_t enet_config;
enet_handle_t g_enet_handle;

/*! @brief The MAC address for ENET device. */
//uint8_t g_macAddr[6] = {0xd4, 0xbe, 0xd9, 0x45, 0x22, 0x60};
unsigned char g_macAddr[6]={0x0, 0x16, 0xd4, 0x9f, 0xed, 0xa4};

void FeedDog(void);


/*! @brief Buffer descriptors should be in non-cacheable region and should be align to "ENET_BUFF_ALIGNMENT". */
AT_NONCACHEABLE_SECTION_ALIGN(enet_rx_bd_struct_t g_rxBuffDescrip[ENET_RXBD_NUM], ENET_BUFF_ALIGNMENT);
AT_NONCACHEABLE_SECTION_ALIGN(enet_tx_bd_struct_t g_txBuffDescrip[ENET_TXBD_NUM], ENET_BUFF_ALIGNMENT);
/*! @brief The data buffers can be in cacheable region or in non-cacheable region.
 * If use cacheable region, the alignment size should be the maximum size of "CACHE LINE SIZE" and "ENET_BUFF_ALIGNMENT"
 * If use non-cache region, the alignment size is the "ENET_BUFF_ALIGNMENT".
 */
SDK_ALIGN(uint8_t g_rxDataBuff[ENET_RXBD_NUM][SDK_SIZEALIGN(ENET_RXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT)],
          APP_ENET_BUFF_ALIGNMENT);
SDK_ALIGN(uint8_t g_txDataBuff[ENET_TXBD_NUM][SDK_SIZEALIGN(ENET_TXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT)],
          APP_ENET_BUFF_ALIGNMENT); 

/* prepare the buffer configuration. */
enet_buffer_config_t enet_buffConfig[1] = {
    {
        ENET_RXBD_NUM,
        ENET_TXBD_NUM,
        SDK_SIZEALIGN(ENET_RXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT),
        SDK_SIZEALIGN(ENET_TXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT),
        &g_rxBuffDescrip[0],
        &g_txBuffDescrip[0],
        &g_rxDataBuff[0][0],
        &g_txDataBuff[0][0],
    }
};

 
/* Hardware Initialization. */
gpio_pin_config_t phy_gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

/*******************************************************************************
* Prototypes
******************************************************************************/
unsigned int mii_check_media (struct mii_if_info *mii, unsigned int ok_to_print, unsigned int init_media);

void ENET_ActiveSend(ENET_Type *base, uint32_t ringId);



 /*******************************************************************************
 * Code
 ******************************************************************************/
static void write_PHY (unsigned int PhyReg, unsigned int data) 
{  
    PHY_Write(ECAT_ENET, ECAT_PHY, PhyReg, data);    
}

static unsigned short read_PHY (unsigned int PhyReg) 
{    
    static unsigned int val;       
    PHY_Read(ECAT_ENET, ECAT_PHY, PhyReg, &val);
    return (val);    
}

void enet_init_pins(void);
void BOARD_init_enet_module_clock(void);
void enet_init(void);
void Init_EMAC(struct net_device *dev)
{
    int  tout;
    bool link = false;
	board_info_t *db = netdev_priv(dev);   
    enet_init();
    
    /* Check the link status. */
    for (tout = 0; tout < TIMEOUT; tout++) 
    {
    	FeedDog();
        PHY_GetLinkStatus(ECAT_ENET, ECAT_PHY, &link);
        if (link)
        {
            /* Link is on. */
            ecdev_set_link(db->ecdev, 1);
            printf("\n\r=====LInk is on chenhao=== \n\r\n\r" );
            break;
        }
    }
	printf("\r\n link:%d\r\n",link);
    
#if 0
//	int id1, id2, tout, regv;
//	uint32_t pclk;
    int  tout;
	board_info_t *db = netdev_priv(dev);   
    uint32_t sysClock;
    status_t status;
    
    enet_init_pins();
    BOARD_init_enet_module_clock();
    
    IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1TxClkOutputDir, true);    
    GPIO_PinInit(GPIO1, 9, &phy_gpio_config);
 //   GPIO_PinInit(GPIO1, 10, &phy_gpio_config);
    /* pull up the ENET_INT before RESET. */
 //   GPIO_WritePinOutput(GPIO1, 10, 1);
    GPIO_WritePinOutput(GPIO1, 9, 0);
    nop_delay(1000000);
    GPIO_WritePinOutput(GPIO1, 9, 1);    
    printf("\r\n 11ENET phy start.1 \r\n");
       ENET_GetDefaultConfig(&enet_config);
    printf("\r\n ENET phy start.2.\r\n");
   
    
    bool link = false;
    phy_speed_t speed;
    phy_duplex_t duplex;
    /* Set SMI to get PHY link status. */
    sysClock = CORE_CLK_FREQ;
    status = PHY_Init(ECAT_ENET, ECAT_PHY, sysClock);
    
   // while(1);
    printf("\r\nENET phy start.3  \r\n");
    while (status != kStatus_Success)
    {
        printf("\r\nPHY Auto-negotiation failed \r\n");
        status = PHY_Init(ECAT_ENET, ECAT_PHY, sysClock);
    }
    printf("\r\n ENET phy start.4 \r\n");
    
    
    /* Check the link status. */
    for (tout = 0; tout < TIMEOUT; tout++) 
    {
        PHY_GetLinkStatus(ECAT_ENET, ECAT_PHY, &link);
        if (link)
        {
            /* Link is on. */
            ecdev_set_link(db->ecdev, 1);
            printf("\n\r=====LInk is on chenhao=== \n\r\n\r" );
            break;
        }
    }
	printf("\r\n link:%d\r\n",link);
    if (link)
    {
        PHY_GetLinkSpeedDuplex(ECAT_ENET, ECAT_PHY, &speed, &duplex);
        enet_config.miiSpeed = (enet_mii_speed_t)speed;
        enet_config.miiDuplex = (enet_mii_duplex_t)duplex;
		printf("\r\n speed %d .duplex:%d\r\n",enet_config.miiSpeed,enet_config.miiDuplex);
    } 
    ENET_Init(ECAT_ENET, &g_enet_handle, &enet_config, &enet_buffConfig[0], &g_macAddr[0], sysClock);
    ENET_ActiveRead(ECAT_ENET);
 #endif
}


// reads a word in little-endian byte order from RX_BUFFER
unsigned short ReadFrame_EMAC(void)
{
    return (*rptr++);
}
void CopyFromFrame_EMAC(void *Dest, unsigned short Size)
{
    unsigned short * piDest;                       // Keil: Pointer added to correct expression
    piDest = Dest;                                 // Keil: Line added
    while (Size > 1) 
    {
        *piDest++ = ReadFrame_EMAC();
        Size -= 2;
    }
    if (Size) 
    {                                                       // check for leftover byte...
        *(unsigned char *)piDest = (char)ReadFrame_EMAC();// the LAN-Controller will return 0
    }                                                       // for the highbyte
}



/******************* gonggw, 2015.01.15, moved from source code on S3C6410 *******************/
struct  board_info *db;	/* Point a board information structure */
struct  net_device *ndev;
struct  net_device *glb_ndev;
void    ec_poll(struct net_device *);
static  int watchdog = 5000;
int EMAC_read_PHY(struct net_device *dev, int phy_id, int location)
{
	return read_PHY(location);
}
void EMAC_write_PHY (struct net_device *dev, int phy_id, int location, int val)
{
	write_PHY(location, (u16)val);
}
static int EMAC_init_hardware(struct net_device *dev)
{
	board_info_t *db = netdev_priv(dev);
	printf("entering %s\n\r", __func__);
	Init_EMAC(dev);
	/* Init Driver variable */
	db->tx_pkt_cnt = 0;
	db->tx_pkt_err_lastcnt = 0;
	db->tx_pkt_err_tm = 0;
	db->queue_pkt_len = 0;
	//gonggw, 2013.09.23, time parameter, must be modified later!!!
	dev->trans_start = (unsigned long)get_ticks();
    return 0;
}
static int EMAC_open(struct net_device *dev)
{
	board_info_t *db = netdev_priv(dev);
    EMAC_init_hardware(dev);
	/* Init driver variable */
	db->dbug_cnt = 0;
	mii_check_media(&db->mii, 1, 1);
	if (!netif_carrier_ok(dev))
		return -100;	//duplex not change, link down   // Network is down 
	else
		return 0;			//OK, Link Up
}


#define PHY_REG_BMCR        0x00        /* Basic Mode Control Register       */
#define PHY_BMCR_RESET		0x8000		/* Reset bit at BMCR PHY reg         */
static int EMAC_stop(struct net_device *ndev)
{
	board_info_t *db = netdev_priv(ndev);
    printf("shutting down %s\n\r", ndev->name);
	netif_carrier_off(ndev);
	write_PHY(PHY_REG_BMCR, PHY_BMCR_RESET);
	return 0;
}
static void EMAC_timeout(struct net_device *dev)
{
	board_info_t *db = netdev_priv(dev);
	EMAC_init_hardware(dev);
	if(!db->ecdev)
	{
		dev->trans_start = (unsigned long)get_ticks();
	}
}
void EMAC_hash_table(struct net_device *dev)
{
    
}

/******************************************************************************
**                          rx func
*******************************************************************************/

static void EMAC_rx(struct net_device *dev)
{
	board_info_t *db = netdev_priv(dev);
	u8 *rdptr = NULL;
   	status_t status, status2;    
    enet_data_error_stats_t eErrStatic;
	uint32_t length = 0;
	u16	j = 0;

	do	//zt	增加do while循环 5次读写
	{
	    /* Get the Frame size */
	    status2 = ENET_GetRxFrameSize(&g_enet_handle, &length);
	    if (length != 0)
	    {
	 //       printf(" -- \n\r ");
	        /* Received valid frame. Deliver the rx buffer with the size equal to length. */
	        rdptr = RX_BUF;
	        status = ENET_ReadFrame(ECAT_ENET, &g_enet_handle, rdptr, length);

	        if (status == kStatus_Success)
	        {
	            ecdev_receive(db->ecdev,rdptr, length);
	  //      printf(" -- \n\r ");
	 /*           
	            printf(" A frame received. the length %d ", length);
	            printf(" Dest Address %02x:%02x:%02x:%02x:%02x:%02x Src Address %02x:%02x:%02x:%02x:%02x:%02x \r\n",
	            rdptr[0], rdptr[1], rdptr[2], rdptr[3], rdptr[4], rdptr[5], rdptr[6], rdptr[7], rdptr[8], rdptr[9],
	            rdptr[10], rdptr[11]);
	*/
	        }
	    }
	    else if (status2 == kStatus_ENET_RxFrameError)
	    {
	        printf("--------- A frame received.  \n\r ");
	        /* Update the received buffer when error happened. */
	        /* Get the error information of the received g_frame. */
	        ENET_GetRxErrBeforeReadFrame(&g_enet_handle, &eErrStatic);
	        /* update the receive buffer. */
	        ENET_ReadFrame(ECAT_ENET, &g_enet_handle, NULL, 0);
	    }	
	}while((status2 != kStatus_ENET_RxFrameEmpty) && (j < 5));

}


/******************************************************************************
**                          tx func
*******************************************************************************/


void EMAC_dma_tx_start(void)
{
    ENET_ActiveSend(ECAT_ENET, 0);
}



static int EMAC_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	int stat = 0;
	board_info_t *db = netdev_priv(dev);
	//debug(db, 3, "%s:\n", __func__);
	if (db->tx_pkt_cnt > 16)  
    {    
		return NETDEV_TX_BUSY;    
    }	
    dev->stats.tx_bytes += skb->len;
	db->tx_pkt_cnt++;	
    //printf("\n\r=====eth_tx stat=%d  == \n\r\n\r",stat );    
    stat = ENET_SendFrame(ECAT_ENET, &g_enet_handle, skb->head ,skb->len);
    
    if (kStatus_Success == stat)
    {
       // printf("The %d frame transmitted success!\r\n", txnumber);
    }
    else
    {
       // printf(" \r\nTransmit frame failed!\r\n");
    }
	if (cur_master->ecat_stage == 0)
	{
		EMAC_dma_tx_start();
	}
	else
	{
        ecat_sr_flag = 1;
#ifdef  EMAC_PKG_RATE
		test_send_pkg_num++;
#endif
	}
	return NETDEV_TX_OK;
}

static void EMAC_tx_done(struct net_device *dev, board_info_t *db)
{
	/* One packet sent complete */
	if (db->tx_pkt_cnt == 0)
		return;
		
	db->tx_pkt_cnt--;
	dev->stats.tx_packets++;
	/* Queue packet check & send */
	if (db->tx_pkt_cnt > 0)
	{
		if (cur_master->ecat_stage == 0)
		{
			EMAC_dma_tx_start();
		}
		else
		{
			ecat_sr_flag  = 1;
#ifdef EMAC_PKG_RATE
			test_send_pkg_num++;
#endif
		}
	
		/*数据发生堵塞后停止填充数据，而直接进行数据发送，理论上来说会让tx_pkt_cnt减小，如果长时间未减小，
		可能并非是缓存堵塞未发出。这边直接多减一次将此故障数据清除 zt*/
		if(db->tx_pkt_cnt < db->tx_pkt_err_lastcnt)
			db->tx_pkt_err_tm = 0;		
				
		db->tx_pkt_err_tm++;
		
		if(db->tx_pkt_err_tm > 100)
			db->tx_pkt_cnt--;
		/*===================修复END=================================*/
	}
	else
	{
		db->tx_pkt_err_tm = 0;
	}

	db->tx_pkt_err_lastcnt = db->tx_pkt_cnt;
}
/******************************************************************************
**                         ec poll func
*******************************************************************************/
//extern uint8_t flag_emac_sended ;
static int EMAC_interrupt( void *dev_id)
{
	struct net_device *dev = (struct net_device *)dev_id;
	board_info_t *db = netdev_priv(dev);
	int int_status;    
    
    int_status = kENET_RxFrameInterrupt | kENET_RxBufferInterrupt;
    if( int_status & ECAT_ENET->EIR)
    {
        EMAC_rx(dev);
        ECAT_ENET->EIR = int_status;
    }
  //   EMAC_rx(dev);
    
    int_status = kENET_TxBufferInterrupt | kENET_TxFrameInterrupt;
    if( int_status & ECAT_ENET->EIR)
    {
        EMAC_tx_done(dev, db);        
        ECAT_ENET->EIR = int_status;
    }
  
#ifdef EMAC_PKG_RATE
    if (cur_master->ecat_stage == 1)
    {
      if (test_send_pkg_num == 10)
			test_offset = test_send_pkg_num - test_rec_pkg_num;

		if ((test_send_pkg_num -test_rec_pkg_num - test_offset) > 2)
		{
			printf("%x, %x, %x\n", test_send_pkg_num, test_rec_pkg_num, test_offset);
			test_offset = test_send_pkg_num - test_rec_pkg_num;
		}
	}
#endif
	return 1;

}
void ec_poll(struct net_device *ndev)
{
	//gonggw, 2013.09.30
	board_info_t *db = netdev_priv(ndev);
 #ifdef EC_RATE_OPT
	mii_check_media(&db->mii, 1, 0);
#endif
	EMAC_interrupt( ndev);

}
static const struct net_device_ops EMAC_netdev_ops = {
	.ndo_open				= EMAC_open,
	.ndo_stop				= EMAC_stop,
	.ndo_start_xmit			= EMAC_start_xmit,
	.ndo_tx_timeout			= EMAC_timeout,
	.ndo_set_multicast_list	= EMAC_hash_table,
};

/******************************************************************
Funciotn: EMAC_probe
Descrioption: EMAC structures init
Calls:
Input:
Ouptut:
Return: 0, success; negative, fail
Others:
******************************************************************/
extern unsigned char chip_mac_addr[6];

int EMAC_probe()
{
	/* Init network device */
	ndev = (struct net_device *)alloc_etherdev(sizeof(struct board_info));
	if (!ndev) {
		dev_err(ndev, "could not allocate device.\n");
		return -ENOMEM;
	}
	dev_dbg(ndev, "EMAC_probe()\n");
	/* setup board info structure */
	db = netdev_priv(ndev);
	db->ndev = ndev;
	glb_ndev = ndev;
    db->dev_base = NULL;   // chenhao   2017.04.01

	db->can_csum = 1;
	db->rx_csum = 1;

	/* driver system function */
	ether_setup(ndev);
#ifdef CONFIG_USE_LIB_MEMCPY
	memcpy(ndev->name, CARDNAME, IFNAMSIZ);
#else
	ec_memcpy(ndev->name, CARDNAME, IFNAMSIZ);
#endif
	ndev->dev_addr =  mac_addr;
	ndev->netdev_ops	= &EMAC_netdev_ops;
	//gonggw, 2013.09.24, must be modified later
	ndev->watchdog_timeo	= watchdog;
	db->msg_enable		    = 0;
	db->mii.phy_id_mask     = 0x1f;
	db->mii.reg_num_mask    = 0x1f;
	db->mii.force_media     = 0;
	db->mii.full_duplex     = 0;
	db->mii.dev	            = ndev;
	db->mii.mdio_read       = EMAC_read_PHY;
	db->mii.mdio_write      = EMAC_write_PHY;

	if (!is_valid_ether_addr(ndev->dev_addr)) {    
		mac_addr[0] = 0x0;
		mac_addr[1] = 0x16;
		mac_addr[2] = 0xd4;
		mac_addr[3] = 0x9f;
		mac_addr[4] = 0xed;
		mac_addr[5] = 0xa4; 
	}
	if (!is_valid_ether_addr(ndev->dev_addr))
	{
		printf("%s: Invalid ethernet MAC address. Please "
			 "set using ifconfig\n\r", ndev->name);
	}
	dev_dbg("%s: MAC: %x:%x:%x:%x:%x:%x\n",
		   ndev->name, ndev->dev_addr[0], ndev->dev_addr[1],ndev->dev_addr[2],ndev->dev_addr[3],
			ndev->dev_addr[4],ndev->dev_addr[5]);


	return 0;

//	printf("not found (%d).\n\r", ret);
//	free_netdev(ndev);

//	return ret;
}

/******************************************************************
Funciotn: EMAC_drv_init
Descrioption: EMAC driver Init entry
Calls:
Input:
Ouptut:
Return: 0, success; negative, fail
Others:
******************************************************************/
int EMAC_drv_init(void)
{
	printf("\n\r%s Ethernet Driver, V%s\n\r", CARDNAME, DRV_VERSION);
	return EMAC_probe();
}



//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------
extern volatile unsigned short int A_1msclock ;


void BOARD_InitModuleClock(void)
{
    const clock_enet_pll_config_t config = {true, false, 1};
    CLOCK_InitEnetPll(&config);
}
void BOARD_init_enet_module_clock(void)
{
    const clock_enet_pll_config_t config = {true, false, 1};
    CLOCK_InitEnetPll(&config);
}

void nop_delay(uint32_t cnt)
{
    volatile uint32_t i = 0;
    for (i = 0; i < cnt; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

void enet_init_pins(void)
{
  CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03u */

  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_09_GPIO1_IO09,        /* GPIO_AD_B0_09 is configured as GPIO1_IO09 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
//  IOMUXC_SetPinMux(
//      IOMUXC_GPIO_AD_B0_10_GPIO1_IO10,        /* GPIO_AD_B0_10 is configured as GPIO1_IO10 */
//      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_12_LPUART1_TX,        /* GPIO_AD_B0_12 is configured as LPUART1_TX */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_13_LPUART1_RX,        /* GPIO_AD_B0_13 is configured as LPUART1_RX */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_04_ENET_RX_DATA00,       /* GPIO_B1_04 is configured as ENET_RX_DATA00 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_05_ENET_RX_DATA01,       /* GPIO_B1_05 is configured as ENET_RX_DATA01 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
     IOMUXC_GPIO_EMC_23_ENET_RX_EN,     // IOMUXC_GPIO_B1_06_ENET_RX_EN,           /* GPIO_B1_06 is configured as ENET_RX_EN */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_07_ENET_TX_DATA00,       /* GPIO_B1_07 is configured as ENET_TX_DATA00 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_08_ENET_TX_DATA01,       /* GPIO_B1_08 is configured as ENET_TX_DATA01 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_09_ENET_TX_EN,           /* GPIO_B1_09 is configured as ENET_TX_EN */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_10_ENET_REF_CLK,         /* GPIO_B1_10 is configured as ENET_REF_CLK */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_B1_10 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_11_ENET_RX_ER,           /* GPIO_B1_11 is configured as ENET_RX_ER */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_04_ENET_MDC, //IOMUXC_GPIO_EMC_40_ENET_MDC,            /* GPIO_EMC_40 is configured as ENET_MDC */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
IOMUXC_GPIO_AD_B1_05_ENET_MDIO,  //      IOMUXC_GPIO_EMC_41_ENET_MDIO,           /* GPIO_EMC_41 is configured as ENET_MDIO */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_09_GPIO1_IO09,        /* GPIO_AD_B0_09 PAD functional properties : */
      0xB0A9u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
//  IOMUXC_SetPinConfig(
//      IOMUXC_GPIO_AD_B0_10_GPIO1_IO10,        /* GPIO_AD_B0_10 PAD functional properties : */
//      0xB0A9u);                               
                                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_12_LPUART1_TX,        /* GPIO_AD_B0_12 PAD functional properties : */
      0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_13_LPUART1_RX,        /* GPIO_AD_B0_13 PAD functional properties : */
      0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B1_04_ENET_RX_DATA00,       /* GPIO_B1_04 PAD functional properties : */
      0xB0E9u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B1_05_ENET_RX_DATA01,       /* GPIO_B1_05 PAD functional properties : */
      0xB0E9u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
     IOMUXC_GPIO_EMC_23_ENET_RX_EN,     //      IOMUXC_GPIO_B1_06_ENET_RX_EN,           /* GPIO_B1_06 PAD functional properties : */
      0xB0E9u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B1_07_ENET_TX_DATA00,       /* GPIO_B1_07 PAD functional properties : */
      0xB0E9u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B1_08_ENET_TX_DATA01,       /* GPIO_B1_08 PAD functional properties : */
      0xB0E9u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B1_09_ENET_TX_EN,           /* GPIO_B1_09 PAD functional properties : */
      0xB0E9u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B1_10_ENET_REF_CLK,         /* GPIO_B1_10 PAD functional properties : */
      0x31u);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: low(50MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Disabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B1_11_ENET_RX_ER,           /* GPIO_B1_11 PAD functional properties : */
      0xB0E9u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
IOMUXC_GPIO_AD_B1_04_ENET_MDC, //      IOMUXC_GPIO_EMC_40_ENET_MDC,            /* GPIO_EMC_40 PAD functional properties : */
      0xB0E9u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
IOMUXC_GPIO_AD_B1_05_ENET_MDIO,  //      IOMUXC_GPIO_EMC_41_ENET_MDIO,           /* GPIO_EMC_41 PAD functional properties : */
      0xB829u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: low(50MHz)
                                                 Open Drain Enable Field: Open Drain Enabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
    
    
}


uint8_t g_frame[ENET_DATA_LENGTH + 14];
/*! @brief Build Frame for transmit. */
void ENET_BuildBroadCastFrame(void)
{
    uint32_t count = 0;
    uint32_t length = ENET_DATA_LENGTH - 14;

    for (count = 0; count < 6U; count++)
    {
        g_frame[count] = 0xFFU;
    }
    memcpy(&g_frame[6], &g_macAddr[0], 6U);
    g_frame[12] = (length >> 8) & 0xFFU;
    g_frame[13] = length & 0xFFU;

    for (count = 0; count < length; count++)
    {
        g_frame[count + 14] = count % 0xFFU;
    }
}

/* 因为两颗PHY芯片分配的物理地址不同，需要根据PCB版本号判断是那颗PHY，version = 0x11,对应DP83848，phy_addr = 1
   version = 0x10 对应YT8512，phy_addr = 3*/
void	Phy_Read(void)
{
	uint32_t GPIO_Value = 0;
	
	GPIO_Value = GPIO_PinRead(CoreBard_Ver0_GPIO, CoreBard_Ver0_GPIO_PIN);
	
	if(GPIO_Value )
	{
		ECAT_PHY = 1;					//Phy芯片为DP83848,PHY硬件地址为1
	}
	else
	{
		ECAT_PHY = 3;					//Phy芯片为YT8512，PHY硬件地址为3
	}
}

void delay_ms(WORD CN);
int flag_no_link = false;
void enet_init(void)
{
	static int error_cnt = 0;
    enet_init_pins();
    BOARD_InitModuleClock();
    IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1TxClkOutputDir, true);
    GPIO_PinInit(GPIO1, 9, &phy_gpio_config);
    GPIO_WritePinOutput(GPIO1, 9, 0);
    delay_ms(100);						//延时100ms，确保reset复位正常，YT8512这里比DP83848时间长
    GPIO_WritePinOutput(GPIO1, 9, 1);
    delay_ms(100);						//复位后，芯片需要一段时间完成内部复位和power-on-strapping工作
    printf("\r\n est ENET phy start.1.\r\n");
	
	Phy_Read();							//获取PHY芯片型号
    
    //for phy 链接状态 tst  
    /* Get default configuration. */
    /* config.miiMode = kENET_RmiiMode;
     * config.miiSpeed = kENET_MiiSpeed100M;
     * config.miiDuplex = kENET_MiiFullDuplex;
     * config.rxMaxFrameLen = ENET_FRAME_MAX_FRAMELEN;
    */
    ENET_GetDefaultConfig(&enet_config);
    printf("\r\n ENET phy start.2.\r\n");
    
    uint32_t sysClock;
    status_t status;
    bool link = false;
    phy_speed_t speed;
    phy_duplex_t duplex;
    /* Set SMI to get PHY link status. */
    sysClock = CORE_CLK_FREQ;
	FeedDog();
    status = PHY_Init(ECAT_ENET, ECAT_PHY, sysClock);
	FeedDog();
    printf("\r\nENET phy start.3  \r\n");
	if (status != kStatus_Success)
	{
		printf("\r\nPHY Auto-negotiation failed \r\n");
		flag_no_link=true;
		error_cnt++;
	}
//    while (status != kStatus_Success)
//    {
//        printf("\r\nPHY Auto-negotiation failed \r\n");
//        status = PHY_Init(ECAT_ENET, ECAT_PHY, sysClock);
//		error_cnt++;
//		if(error_cnt>10)
//		{
//			flag_no_link=true;
//			break;
//		}
//    }
    printf("\r\n ENET phy start.4 \r\n");
    PHY_GetLinkStatus(ECAT_ENET, ECAT_PHY, &link);
	printf("\r\n link:%d\r\n",link);
    if (link)
    {
        PHY_GetLinkSpeedDuplex(ECAT_ENET, ECAT_PHY, &speed, &duplex);
        enet_config.miiSpeed = (enet_mii_speed_t)speed;
        enet_config.miiDuplex = (enet_mii_duplex_t)duplex;
		printf("\r\n speed %d .duplex:%d\r\n",enet_config.miiSpeed,enet_config.miiDuplex);
    }
 
    ENET_Init(ECAT_ENET, &g_enet_handle, &enet_config, &enet_buffConfig[0], &g_macAddr[0], sysClock);
    ENET_ActiveRead(ECAT_ENET);
    
}
void enet_board_tst(void)
{
    bool link = false;
    uint32_t length = ENET_DATA_LENGTH - 14;
    enet_data_error_stats_t eErrStatic;
    uint32_t g_testTxNum = 0;
    uint32_t txnumber = 0;    
    
    status_t status;
      /* Build broadcast for sending. */
    ENET_BuildBroadCastFrame();

    while (1)
    {
        /* Get the Frame size */
        status = ENET_GetRxFrameSize(&g_enet_handle, &length);
        /* Call ENET_ReadFrame when there is a received frame. */
        if (length != 0)
        {
            /* Received valid frame. Deliver the rx buffer with the size equal to length. */
            uint8_t *data = (uint8_t *)malloc(length);
            status = ENET_ReadFrame(ECAT_ENET, &g_enet_handle, data, length);
            if (status == kStatus_Success)
            {
                printf(" A frame received. the length %d ", length);
                printf(" Dest Address %02x:%02x:%02x:%02x:%02x:%02x Src Address %02x:%02x:%02x:%02x:%02x:%02x \r\n",
                       data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9],
                       data[10], data[11]);
            }
            free(data);
        }
        else if (status == kStatus_ENET_RxFrameError)
        {
            /* Update the received buffer when error happened. */
            /* Get the error information of the received g_frame. */
            ENET_GetRxErrBeforeReadFrame(&g_enet_handle, &eErrStatic);
            /* update the receive buffer. */
            ENET_ReadFrame(ECAT_ENET, &g_enet_handle, NULL, 0);
        }

        if (g_testTxNum < ENET_TRANSMIT_DATA_NUM)
        {
            /* Send a multicast frame when the PHY is link up. */
            if (kStatus_Success == PHY_GetLinkStatus(ECAT_ENET, ECAT_PHY, &link))
            {
                if (link)
                {
                    g_testTxNum++;
                    txnumber++;
                    if (kStatus_Success == ENET_SendFrame(ECAT_ENET, &g_enet_handle, &g_frame[0], ENET_DATA_LENGTH))
                    {
                        printf("The %d frame transmitted success!\r\n", txnumber);
                    }
                    else
                    {
                        printf(" \r\nTransmit frame failed!\r\n");
                    }
                }
            }
        }
    }
}











