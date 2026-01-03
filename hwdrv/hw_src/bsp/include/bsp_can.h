#ifndef _BSP_CAN_H_
#define _BSP_CAN_H_

#include "can2.h"
#include "fsl_flexcan.h"

#define EXAMPLE_CAN CAN1
#define CAN2_TEST   CAN2

#define RX1_MESSAGE_BUFFER_NUM (10)
#define TX1_MESSAGE_BUFFER_NUM (9)
#define RX2_MESSAGE_BUFFER_NUM (12)
#define TX2_MESSAGE_BUFFER_NUM (11)

extern flexcan_mb_transfer_t txXfer1, rxXfer1;
extern flexcan_mb_transfer_t txXfer2, rxXfer2;
extern flexcan_handle_t flexcanHandle,flexcan2Handle;

extern flexcan_frame_t tx1_frame;
extern flexcan_frame_t rx1_frame; 
extern flexcan_frame_t tx2_frame;
extern flexcan_frame_t rx2_frame;

void FlexCAN_init(void);
void FlexCAN2_init(void);
void CanReceiveBlocking_HAL(void);
void CanRxFrameMailbox0_HAL(void);
void CanTxFrameMailbox4_HAL(void);
BOOL Can2TxFrameMailbox_HAL(PCAN_FRAM pf);
BOOL Can2RxFrameMailbox_HAL(PCAN_FRAM pf);



#endif
