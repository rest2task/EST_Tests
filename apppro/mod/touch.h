#ifndef _TOUCH_h
#define _TOUCH_h

#ifdef __cplusplus
extern "C"
{
#endif
#include "typedef.h"

#define TOUCHSREEN_FILE "/tmp/touchcheck.txt"  //触摸屏检测文件 //20210914 dyl touch

void ProcessCommRxData3(UI8* buf, int len);
void SetKeySoundInit();
void ReceieveRxKey(UI16 key);//串口2接收到0xF0;
BOOL IsM792();

#ifdef __cplusplus
}
#endif

#endif



