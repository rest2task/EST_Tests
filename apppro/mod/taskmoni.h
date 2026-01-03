#ifndef _TASKMONI_H
#define _TASKMONI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "typedef.h"


/*other task on update */
void OtherTaskMoni();

/*return operating mode index */
UI16 OperateModeIndex();
/*return random data */
UI16 GetRandomData();


void PanelMainRestart();
UI32 GetLED();
UI32 GetLED_2();

UI16 GetKeyCnt();//20191224.cyx 心跳交互计数:连续发送5次无反馈心跳，认为按键板通讯失败
void SetKeyCnt(UI16 cnt);
void Reset_KeyCnt();
BOOL MainProtOkForRS232();

#ifdef __cplusplus
}
#endif

#endif




