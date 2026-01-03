#ifndef _VERINFO_h
#define _VERINFO_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "mod.h"

typedef struct tyVERSIONREAD{
	UI16 wPanelMacName;                 // 0   面板机器名称
	UI16 wPanelMacMode;                 // 1   面板机架构造
	UI16 wPanelMacProp;                 // 2   面板机械特性
	UI16 wPanelDBVer;                   // 3   面板数据库版本
	UI16 wPanelPlatform;                // 4   面板平台版本
	UI16 wPanelSupportSys;              // 5   面板支撑系统
	UI16 wPanelManuCode;                // 6   面板厂商代码
	UI16 wPanelFactory;                 // 7   面板厂商
	UI16 wPanelDate1;                   // 8   面板日期1
	UI16 wPanelDate2;                   // 9   面板日期2
	UI16 wPanelViewVer1;                // A   面板画面版本1
	UI16 wPanelViewVer2;                // B   面板画面版本2
	UI16 wPanelViewType;                // C   面板画面类别
}VERSIONREAD,*PVERSIONREAD;

typedef struct tyOUTVERSION{
	UI16 wOutPanelFactory;               //0  文件导出数据－面板厂商
	UI16 wPanelType;                     //1  面板类型 7/8/10/12/15
}OUTVERSION;

enum enPANEL_TYPE
{
    PANEL_H7 = 0,  //横屏7寸
    PANEL_H8 = 1,  //横屏8寸
    PANEL_H10 = 2, //横屏10寸
    PANEL_V10 = 3, //竖屏10寸
    PANEL_VT15 = 4 //竖屏触摸15寸
};

extern VERSIONREAD m_verinfo;
extern OUTVERSION  m_panelcode;

//20200326
extern PHASH_TBL pmap_powertype;
extern PHASH_TBL pmap_machtype;
extern PHASH_TBL pmap_usetype;

BOOL PanelCfgLoad();
UI16 PanelType();
UI16 GetPanelType();
void KernelVersionRead();//20190708.cyx
UI32 GetKernelVersion(int index);//20190708.cyx
BOOL PanelVerLoad();

#define PANELCODE_INIT_FILE	SAVE_PATH"panelcfg.dat"
#define KERNEL_VERSION_FILE	SAVE_PATH"KernelVer.txt" //20190708.cyx

//20200326
#define POWERTYPE_FILE		DATA_PATH"power_type.map"
#define MACHTYPE_FILE		DATA_PATH"mach_type.map"
#define USETYPE_FILE		DATA_PATH"use_type.map"

#ifdef __cplusplus
}
#endif

#endif



