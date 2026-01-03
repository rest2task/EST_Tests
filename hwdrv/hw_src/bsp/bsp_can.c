#include "fsl_debug_console.h"
#include "fsl_flexcan.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_iomuxc.h"
#include "Typedef.h"
#include "gpt_timer.h"
#include "bsp_can.h"
#include "canhead.h"
#include "Configdf.h"
#include "machapi.h"
#include "Typedef.h"
#include "HwISR.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define EXAMPLE_FLEXCAN_IRQn CAN2_IRQn
#define EXAMPLE_FLEXCAN_IRQHandler CAN2_IRQHandler
#define DLC (8)
/* To get most precise baud rate under some circumstances, users need to set
   quantum which is composed of PSEG1/PSEG2/PROPSEG. Because CAN clock prescaler
   = source clock/(baud rate * quantum), for e.g. 84M clock and 1M baud rate, the
   quantum should be .e.g 14=(6+3+1)+4, so prescaler is 6. By default, quantum
   is set to 10=(3+2+1)+4, because for most platforms e.g. 120M source clock/(1M
   baud rate * 10) is an integer. Remember users must ensure the calculated
   prescaler an integer thus to get precise baud rate. */
#define SET_CAN_QUANTUM 0
#define PSEG1 3
#define PSEG2 2
#define PROPSEG 1

/* Select 80M clock divided by USB1 PLL (480 MHz) as master flexcan clock source */
#define FLEXCAN_CLOCK_SOURCE_SELECT (2U)
/* Clock divider for master flexcan clock source */
#define FLEXCAN_CLOCK_SOURCE_DIVIDER (3U)
/* Get frequency of flexcan clock */
#define EXAMPLE_CAN_CLK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 6) / (FLEXCAN_CLOCK_SOURCE_DIVIDER + 1U))
#if (defined(FSL_FEATURE_FLEXCAN_HAS_ERRATA_5829) && FSL_FEATURE_FLEXCAN_HAS_ERRATA_5829)
/* To consider the First valid MB must be used as Reserved TX MB for ERR005829
   If RX FIFO enable(RFEN bit in MCE set as 1) and RFFN in CTRL2 is set default zero, the first valid TX MB Number is 8
   If RX FIFO enable(RFEN bit in MCE set as 1) and RFFN in CTRL2 is set by other value(0x1~0xF), User should consider
   detail first valid MB number
   If RX FIFO disable(RFEN bit in MCE set as 0) , the first valid MB number is zero */



#endif
#ifndef DEMO_FORCE_CAN_SRC_OSC
#define DEMO_FORCE_CAN_SRC_OSC 0
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
 
void CanReceiveInterrupt(void);
unsigned char Check_TxMb_Status(CAN_Type *base, uint8_t mbIdx);

/*******************************************************************************
 * Variables
 ******************************************************************************/
flexcan_handle_t flexcanHandle,flexcan2Handle;

volatile bool wakenUp    = false;
//flexcan_mb_transfer_t txXfer, rxXfer;
flexcan_mb_transfer_t txXfer1, rxXfer1;
flexcan_mb_transfer_t txXfer2, rxXfer2;
#if (defined(USE_CANFD) && USE_CANFD)
flexcan_fd_frame_t frame;
#else
//flexcan_frame_t frame;
flexcan_frame_t tx1_frame;
flexcan_frame_t rx1_frame;
flexcan_frame_t tx2_frame;
flexcan_frame_t rx2_frame;

#endif
uint32_t txIdentifier;
uint32_t rxIdentifier;

uint8_t flexcan_time;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief FlexCAN Call Back function
 */
 


void CanReceiveBlocking_HAL()
{
	rxXfer1.mbIdx = RX1_MESSAGE_BUFFER_NUM;
	rxXfer1.frame = &rx1_frame;
	FLEXCAN_TransferReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer1);

	rxXfer2.mbIdx = RX2_MESSAGE_BUFFER_NUM;
	rxXfer2.frame = &rx2_frame;
	FLEXCAN_TransferReceiveNonBlocking(CAN2, &flexcan2Handle, &rxXfer2);
		
}


void CanCMRSet()
{
}

void FlexCAN_init(void)
{
	flexcan_config_t flexcanConfig;
    flexcan_rx_mb_config_t mbConfig;
   // uint8_t node_type;
	
		IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_02_FLEXCAN1_TX,1U);                                   
		IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_03_FLEXCAN1_RX,1U);                                    
		IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_02_FLEXCAN1_TX,0x10B0u);                               
		IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_03_FLEXCAN1_RX,0x10B0u);                               
    /*Clock setting for FLEXCAN*/
    CLOCK_SetMux(kCLOCK_CanMux, FLEXCAN_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_CanDiv, FLEXCAN_CLOCK_SOURCE_DIVIDER);

		txIdentifier =  0;//0x3d1; 
		rxIdentifier =  0;//0x309;

//    /* Get FlexCAN module default Configuration. */
//    
   //   flexcanConfig.clkSrc = kFLEXCAN_ClkSrcOsc;
      flexcanConfig.baudRate = 100000U;
  //    flexcanConfig.baudRateFD = 1000000U;
//      flexcanConfig.maxMbNum = 16;
//      flexcanConfig.enableLoopBack = false;
//      flexcanConfig.enableSelfWakeup = false;
//      flexcanConfig.enableIndividMask = false;
//   //   flexcanConfig.enableDoze = false;
 //     flexcanConfig.timingConfig = timingConfig;

    FLEXCAN_GetDefaultConfig(&flexcanConfig);
    flexcanConfig.baudRate = 500000U;

    FLEXCAN_Init(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ);

    /* Create FlexCAN handle structure and set call back function. */
    FLEXCAN_TransferCreateHandle(EXAMPLE_CAN, &flexcanHandle, flexcan_callback, NULL);

    /* Set Rx Masking mechanism. */
    FLEXCAN_SetRxMbGlobalMask(EXAMPLE_CAN, FLEXCAN_RX_MB_STD_MASK(rxIdentifier, 0, 0));

    /* Setup Rx Message Buffer. */
    mbConfig.format = kFLEXCAN_FrameFormatStandard;
    mbConfig.type   = kFLEXCAN_FrameTypeData;
    mbConfig.id     = FLEXCAN_ID_STD(rxIdentifier);
    FLEXCAN_SetRxMbConfig(EXAMPLE_CAN, RX1_MESSAGE_BUFFER_NUM, &mbConfig, true);
    FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX1_MESSAGE_BUFFER_NUM, true);
    tx1_frame.dataByte0 = 0;
    rx1_frame.dataByte0 = 0;

	rxXfer1.mbIdx = RX1_MESSAGE_BUFFER_NUM;
	rxXfer1.frame = &rx1_frame;
	FLEXCAN_TransferReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer1);
}

void FlexCAN2_init(void)
{
	flexcan_config_t flexcanConfig;
    flexcan_rx_mb_config_t mbConfig;
   // uint8_t node_type;
	
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_02_FLEXCAN2_TX,1U);                                   
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_03_FLEXCAN2_RX,1U);                                    
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_02_FLEXCAN2_TX,0x10B0u);                               
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_03_FLEXCAN2_RX,0x10B0u);                               
    /*Clock setting for FLEXCAN*/
    CLOCK_SetMux(kCLOCK_CanMux, FLEXCAN_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_CanDiv, FLEXCAN_CLOCK_SOURCE_DIVIDER);

	txIdentifier =  0;//0x3d1; 
	rxIdentifier =  0;//0x361;// 0x309;
//    /* Get FlexCAN module default Configuration. */
//    
   //   flexcanConfig.clkSrc = kFLEXCAN_ClkSrcOsc;
      flexcanConfig.baudRate = 100000U;
  //    flexcanConfig.baudRateFD = 1000000U;
//      flexcanConfig.maxMbNum = 16;
//      flexcanConfig.enableLoopBack = false;
//      flexcanConfig.enableSelfWakeup = false;
//      flexcanConfig.enableIndividMask = false;
//   //   flexcanConfig.enableDoze = false;
 //     flexcanConfig.timingConfig = timingConfig;

    FLEXCAN_GetDefaultConfig(&flexcanConfig);
    flexcanConfig.baudRate = 500000U;

    FLEXCAN_Init(CAN2, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ);

    /* Create FlexCAN handle structure and set call back function. */
    FLEXCAN_TransferCreateHandle(CAN2, &flexcan2Handle, flexcan2_callback, NULL);

    /* Set Rx Masking mechanism. */
    FLEXCAN_SetRxMbGlobalMask(CAN2, FLEXCAN_RX_MB_STD_MASK(rxIdentifier, 0, 0));

    /* Setup Rx Message Buffer. */
    mbConfig.format = kFLEXCAN_FrameFormatStandard;
    mbConfig.type   = kFLEXCAN_FrameTypeData;
    mbConfig.id     = FLEXCAN_ID_STD(rxIdentifier);
    FLEXCAN_SetRxMbConfig(CAN2, RX2_MESSAGE_BUFFER_NUM, &mbConfig, true);
    FLEXCAN_SetTxMbConfig(CAN2, TX2_MESSAGE_BUFFER_NUM, true);
    tx2_frame.dataByte0 = 0;
    rx2_frame.dataByte0 = 0;
	
	rxXfer2.mbIdx = RX2_MESSAGE_BUFFER_NUM;
	rxXfer2.frame = &rx2_frame;
	FLEXCAN_TransferReceiveNonBlocking(CAN2, &flexcan2Handle, &rxXfer2);

}



