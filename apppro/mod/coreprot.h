#ifndef COREPROT_H
#define COREPROT_H
#include "typedef.h"
#include "obj.h"
#include "mod.h"
#ifdef __cplusplus
extern "C"
{
#endif

void SyncCoreAdvRetPos(UI32 dwAddr);//20201110 dyl core pos mode
//20210730 dyl 中子有效区位置
void CoreEffeAreaByPushtab(UI32 Pos_dwAddr, UI32 Effe_dwAddr);
//20210820 dyl 中子动作点跟随特殊中子功能及联动功能变化
void CorePointByLink();
//20210730 dyl 中子有效区位置 //20250416 chj 优化修改开模终止位置后，马上按开模按键，按键值下发卡顿问题 -- 这个函数单独给 SyncCoreAdvRetPos 使用
void CoreEffeAreaByPushtab_SendAll(UI32 Pos_dwAddr, UI32 Effe_dwAddr);

#ifdef __cplusplus
}
#endif
#endif // LUBR_H
