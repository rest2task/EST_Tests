#ifndef _KJUMP_h
#define _KJUMP_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "mod.h"

void KJumpInital(PCMOD pmod);

BOOL KJumpKey(PCPAR para);
BOOL KJumpPage(PCPAR para);
BOOL KJumpNext(PCPAR para);
BOOL KJumpMain(PCPAR para);

void PumpGuideSwitch();
void TempGuideSwitch();
void PushGuideSwtich();
void InjCurveGuideSwtich();
void SysParamGuideSwtich();
void ServoDebuguideSwtich();
void BlowGuideSwtich();
void PowerQCGuideSwtich();
void KeyTestGuideSwtich();
void MainGuideSwitch();
void TempXGuideSwitch();
void OtherGuideSwitch();
void FastGuideSwitch();
void RotateGuideSwitch();//20190424 CHZ
void VEjectGuideSwitch();//20190424 CHZ
void VMainGuideSwitch();//20190424 CHZ
void VInjCurveGuideSwitch();//20190424 CHZ
void KJumpEnable(BOOL enable);

/**************G10新增跳转表***开始***************/
void G10ServoDebug1GuideSwitch();//伺服配置1
void G10ServoStateGuideSwitch();//20211117 dyl 伺服状态画面等级放低到LV2 --伺服机模式的伺服状态
void G10ClampGuideSwitch();//开关模
void G10MainGuideSwitch();//主画面
void G10IOInputGuideSwitch();//IO输入
void G10SeqVlvGuideSwitch();//顺序阀
void G10ParaRecordGuideSwitch();//参数记录
void G10MTGuideSwitch();//模温画面 //20200407HJM 模温功能
void G10MachSetGuideSwitch();//机器结构参数画面
void G10SpecFunc1GuideSwitch();//特殊功能
void G10ProdManageGuideSwitch();//生产管理
void G10LubGuideSwtich();//20211206 dyl 润滑
/**************G10新增跳转表***结束***************/

/**************P3200新增跳转表***开始***************/
void P3200ClampGuideSwitch();//开关模
void P3200EjectGuideSwtich();//托模
void P3200EjectAdjGuideSwtich(); //托模
void P3200ChargeGuideSwtich();//储料
void P3200ADAdjustGuideSwtich();//电子尺调整
void P3200SysParaGuideSwtich();//系统参数
void P3200ProgUpdateGuideSwitch();//程序升级
void P3200MainGuideSwitch();
void P3200TempxGuideSwitch();
/**************P3200新增跳转表***结束***************/

#ifdef __cplusplus
}
#endif

#endif



