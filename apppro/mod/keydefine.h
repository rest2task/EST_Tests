#ifndef KEYDEFINE_H
#define KEYDEFINE_H
#include "typedef.h"
#include "obj.h"
#include "wgt.h"
#ifdef __cplusplus
extern "C"
{
#endif

typedef enum enKEY_DEFINE {
    KEY_DEFAULT,    //默认
    KEY_OPEN,       //开模
    KEY_CLOSE,      //关模
    KEY_INJECT,     //射出
    KEY_SUCKBACK,   //射退
    KEY_EJECTRET,   //托模退
    KEY_EJECTADV,   //托模进
    KEY_CHARGE,     //加料
    KEY_AUTOPURGE,  //清料
    KEY_COREOUT,    //中子出
    KEY_COREIN,     //中子入
    KEY_CORESELECT, //中子切换
    KEY_LUBRICATE,  //润滑
    KEY_AIRBLOW135, //吹气1-3-5
    KEY_AIRBLOW246, //吹气2-4-6
    KEY_AIRSELECT,  //吹气切换
    KEY_MOLDADJ,    //调模使用
    KEY_NOZZLEADV,  //座进
    KEY_NOZZLERET,  //座退
    KEY_MOLDTHICK,  //调模退
    KEY_MOLDTHIN,   //调模进
    KEY_MOTOR,      //马达
    KEY_HEATER,     //电热
    KEY_SELFDEFINE  //自定义
}KEY_DEFINE;

UI16 KeyDefine_FindSendKey(UI32 oriKey);
UI16 KeyDefine_GetSendKey(UI16 value, UI16 num);
BOOL KeyDefine_Def(int cur_key, KEY_DEFINE key_def);
BOOL KeyDefine_LED_State(UI16 num);
BOOL KeyDefine_LED_State2(UI16 num);

#ifdef __cplusplus
}
#endif
#endif // KEYDEFINE_H
