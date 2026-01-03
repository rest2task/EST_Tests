#ifndef _KEYMOD_h
#define _KEYMOD_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "mod.h"
#include "wgt.h"
#include "pagefram.h"

#define PANEL_FUNC_MAX  80  //最大80个功能按键值可定义
#define PANEL_TBL_CNT   6   //按键表结构体元素个数
#define MAX_IO_OUT_CB   5   //输出页面强制输出值归零最大回调数


typedef struct tyKEYTBL
{
	UI32 phykey;       //物理按键值
	UI32 phylight;     //物理灯值
	UI32 sendkey;      //发送按键值
	UI32 keynum;      //按键名称序号
	UI32 newsendkey;   //新发送按键值
	UI32 newkeynum;   //新按键名称序号
}KEYTBL;

typedef struct tyKEYNAME
{
	const char* pcn;   //按键中文名
	const char* pen;   //按键英文名
}KEYNAME;

typedef struct tyKEYINFO
{
	KEYTBL  keytbl[PANEL_FUNC_MAX];
	KEYNAME keyname[PANEL_FUNC_MAX];
	UI16  cfgflag;   //按键信息表标记
}KEYINFO,PKEYINFO;


#define  KSEL_COREA			0x0002      //中子AD
#define  KSEL_COREB			0x0004      //中子BE
#define  KSEL_COREC			0x0008      //中子CC 

#define  KSEL_BLAST2		0x0020      //吹气2组 
#define  KSEL_BLAST3		0x0040      //吹气3组
#define  KSEL_BLAST4	    0x0080      //吹气4组
#define  KSEL_BLAST5        0x0100      //吹气5组
#define  KSEL_BLAST6	    0x0200      //吹气6组 
#define  KSEL_BLAST7	    0x0400      //吹气7组 

extern BOOL g_bEnterPress;//确认按键长按标记
extern BOOL m_isInForce;//强制输出标记

/*===========================================================================+ 
|           Get key table By key xml file function                           | 
+===========================================================================*/ 
void XmlToMapKey();

/*===========================================================================+ 
|           Get sendkey By physical key function                             | 
+===========================================================================*/ 
UI16 KeyToMap(UI32 oriKey);

/*===========================================================================+ 
|           Get LED By sendkey function                                      | 
+===========================================================================*/ 
UI32 KeyToLed(UI32 sendkey);

/*===========================================================================+ 
|           Whether key table by xml file function                           | 
+===========================================================================*/ 
UI16 IsKeyByXml();

/*===========================================================================+ 
|           Manual key rx function                                           | 
+===========================================================================*/ 
void MCUKeyRx(UI16 keycode);

//
void SendPanelKey(UI32 wKey,UI16 press);
int KeyGetCallBack(int cur_key, int last_key);
void PanelKeyProcCallBack(int cur_key, int press);
UI32 JYKeyChange(UI32 cur_key);//20241101 金鹰10寸快速机特殊面膜按键修改
UI32 JYLubKeyChange(UI32 cur_key);//20241101 金鹰10寸快速机特殊面膜按键修改
void CoreSpcPorcess();//20241225 chj 中子如果单组开了，无需按中子切换,已经金鹰快速机模式下，中子按键为下拉框对应的选择项
//test key
typedef void (*TESTKEY_CB)(UI32 key,UI32 press);
TESTKEY_CB SetTestKeyCB(TESTKEY_CB cb);
void TestKeyStart();
void TestKeyEnd();

//da adj
typedef void (*DAADJUST_CB)(UI32 key,UI32 press);
DAADJUST_CB SetDaAdjustCB(DAADJUST_CB da_linear);

//machcfg change
typedef void (*MACHCFG_CB)(int key,int press);
MACHCFG_CB SetMachcfgCB(MACHCFG_CB data_change);

//machcfg5200 change
typedef void (*MACHCFG5200_CB)(int key,int press);
MACHCFG5200_CB SetMachcfg5200CB(MACHCFG5200_CB data_change);

//PUMP change 20241223 fqh pump修改的回调函数
typedef void (*PUMPFUNC_CB)(int key,int press);
PUMPFUNC_CB SetPumpFuncCB(PUMPFUNC_CB data_change);

//da adj change
typedef void (*DAADJ_CB)(int key,int press);
DAADJ_CB SetDaAdjCB(DAADJ_CB data_change);

//adex change
typedef void (*ADEX_CB)(int key,int press);
ADEX_CB SetADExCB(ADEX_CB data_change);

//adx change
typedef void (*ADX_CB)(int key,int press);
ADX_CB SetADxCB(ADX_CB data_change);

//specfunc select change
typedef void (*SPECFUNC_SELECT_CB)(int key,int press);
SPECFUNC_SELECT_CB SetSpecFuncSelectCB(SPECFUNC_SELECT_CB data_change);

//keys select
UI16 SetKeySel(UI16 keysel_bit);
UI16 ClrKeySel(UI16 keysel_bit);
BOOL ChkKeySel(UI16 keysel_bit);

//help 
BOOL SetHelpLink(BOOL b_link);
BOOL SetHelpLinkx(BOOL b_linkx);

//force output to zero 20190507
typedef void (*IO_OUT_CB)(void);
IO_OUT_CB SetIOOutZeroCB(IO_OUT_CB force_value_zero);
typedef void (*IO_OUT2_CB)(void);
IO_OUT2_CB SetIOOut2ZeroCB(IO_OUT2_CB force_value_zero);

typedef void (*TAB_CB)(UI32 key,UI32 press);
/**
* @brief     :Tab画面特殊按键处理
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191113
*/
TAB_CB SetTabCB(TAB_CB cb);
void FkeyTab(PWGT tabpage, UI32 fkey);
BOOL SetCancelFocus(int obj_type);

typedef void (*ECAT_CB)(UI32 key,UI32 press);
ECAT_CB SetEcatCB(ECAT_CB cb);

typedef void (*MAIN_CB)(UI32 key,UI32 press);
MAIN_CB SetMainCB(MAIN_CB cb);

typedef BOOL (*DATADIAG_CB)(int key,int press);
DATADIAG_CB SetDataDiagCB(DATADIAG_CB cb);

typedef BOOL (*CANCELFOCUS_CB)(int obj_type);
CANCELFOCUS_CB SetCancelFocusCB(CANCELFOCUS_CB cancelfocus_cb);

typedef BOOL (*SERVOSTATEX1_CB)(int key,int press);
SERVOSTATEX1_CB SetServoStatex1CB(SERVOSTATEX1_CB cb);

void Reset_ActiveTime_Count();//20191224.cyx 重置屏保时间计数
void Reset_PanelNoKeyPress();//20250319 jhh 更新画面主机更新过程中不进入屏保
BOOL GetLockState();

BOOL DownSetTabPageFocus();//20210804 光标跳转进TABPAGE

BOOL PageFramXSort(PPAGE_FRAM pf);//20211229 dyl 画面光标xo重新排序
BOOL PageSubXExChange(PPAGE_FRAM pf,const char *wgtname, int srcxo,int dstxo);//20211229 dyl 子窗体光标重新排序

//20250319 chj 优化点屏幕功能，每次亮屏，需要获取驱动信号，来确定，是否真实亮屏了
BOOL GetScreenState();
//20250319 chj 优化点屏幕功能，往brightness文件内写入数值
int set_brightness(int value);

#ifdef __cplusplus
}
#endif

#endif



