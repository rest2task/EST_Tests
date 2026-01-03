#ifndef _HWAPP_H_
#define _HWAPP_H_

#include "can2.h"
#include "fsl_flexcan.h"

void flexcan_callback(CAN_Type *base, flexcan_handle_t *handle, status_t status, uint32_t result, void *userData);
void flexcan2_callback(CAN_Type *base, flexcan_handle_t *handle, status_t status, uint32_t result, void *userData);
void CanRxFrameMailbox0(void);
void CanTxFrameMailbox4(void);
BOOL Can2TxFrameMailbox(PCAN_FRAM pf);
BOOL Can2RxFrameMailbox(PCAN_FRAM pf);


#endif
