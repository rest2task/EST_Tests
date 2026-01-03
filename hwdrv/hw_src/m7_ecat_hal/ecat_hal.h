/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _ECAT_HAL_H_
#define _ECAT_HAL_H_


#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "fsl_enet.h"
#include "fsl_phy.h"

#include "fsl_debug_console.h"
#include "types.h"
#include "netdevice.h"
#include "device.h"
#include "config.h"
#include "ecdev.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
 
#define ENET_RXBD_NUM (4)
#define ENET_TXBD_NUM (4)
//#define ENET_RXBD_NUM (1)
//#define ENET_TXBD_NUM (1)
#define ENET_RXBUFF_SIZE (ENET_FRAME_MAX_FRAMELEN)
#define ENET_TXBUFF_SIZE (ENET_FRAME_MAX_FRAMELEN)
//#define ENET_DATA_LENGTH (1000)
#define ENET_DATA_LENGTH (60)
#define ENET_TRANSMIT_DATA_NUM (20)
#ifndef APP_ENET_BUFF_ALIGNMENT
#define APP_ENET_BUFF_ALIGNMENT ENET_BUFF_ALIGNMENT
#endif
 
#define CORE_CLK_FREQ CLOCK_GetFreq(kCLOCK_AhbClk)
//#define ECAT_PHY     0x05U
//#define ECAT_PHY     0x01U   //83848
//#define ECAT_PHY     0x03U   //8512
//#define ECAT_PHY     0x00U
#define ECAT_ENET    ENET
 
 
 
extern gpio_pin_config_t phy_gpio_config ;
extern enet_config_t enet_config;
extern enet_handle_t g_enet_handle;
extern uint8_t g_macAddr[6] ;
extern enet_buffer_config_t enet_buffConfig[1];


//for phy tst    
extern uint8_t g_frame[ENET_DATA_LENGTH + 14];
/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif
void BOARD_InitModuleClock(void);
void nop_delay(uint32_t cnt);
void EMAC_dma_tx_start(void);
void timer_adjust(int rate);
int EMAC_drv_init(void);
    
        
//for phy tst    
void ENET_BuildBroadCastFrame(void);    
    
//gongw, 2015.01.12, moved from S3C6410 drivers
typedef struct board_info {
    void    *dev_base;  //chenhao 2017.04.01
	u16		tx_pkt_cnt;
	u16		tx_pkt_err_tm;		//异常时间
	u16		tx_pkt_err_lastcnt;	//上一次的异常cnt
	u16		queue_pkt_len;
	u16		queue_ip_summed;
	u16		dbug_cnt;
	u8		io_mode;		/* 0:word, 2:byte */
	u8		imr_all;

	unsigned int	in_suspend :1;
	unsigned int	wake_supported :1;

    void (*inblk)(volatile void *port, void *data, int length);
    void (*outblk)(volatile void *port, void *data, int length);
    void (*dumpblk)(volatile void *port, int length);

	struct net_device  *ndev;
	struct mii_if_info mii;
	u32		msg_enable;
	u32		wake_state;

	int		rx_csum;
	int		can_csum;
	int		ip_summed;	
	ec_device_t *ecdev;
} board_info_t;  

extern uint32_t	ECAT_PHY;
    
/* @} */
#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* _FSL_PHY_H_ */
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/


