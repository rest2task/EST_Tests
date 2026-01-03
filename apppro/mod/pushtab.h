 #ifndef _PUSHTAB_h
#define _PUSHTAB_h

#ifdef __cplusplus
extern "C"
{
#endif

#include "mod.h"

#define  NUM_PUSHTABLEDATA      288//最大对应组数
#define  TRANSMETER_ITEM_SIZE 	8  //数据字节数
#define  PUSHTAB_PAGE_GROUPNUM  48 //每页最大组数

/********************五支点新增*****************/ //20190813.cyx
#define  SHOW_LMTAG_ERROR       0x3312007E //五支点参数极限夹角输入有误
#define  USE_PUSHPARA           0x3312007F //是否使用五支点
#define  MAX_PUSHTABLENUM       960//最大对应组数 四位小数生成表
#define  MAX_PUSHTABLENUMA      960//最大对应组数 两位小数生成表
#ifdef __ICCARM__
#define  M_PI		3.14159265358979323846
#endif
/*********************************************/

//推力座结构
typedef struct tyTRANSMETER_ITEM
{
	UI16    wPump;
	UI32    wClamp;
} TRANSMETER_ITEM,*PTRANSMETER_ITEM;

extern TRANSMETER_ITEM g_dbTransMeterTable[];

//转换开关模电子尺最大值
void PushToMaxMeter();

//推力座载入或重置时调用，用于初始化推力座数据
BOOL SetMachineType(UI16 wType);

UI32 TransMeter(UI16 wPumpMeter);
UI16 UnTransMeter(UI16 wClampMeter,BOOL bPoint2);
void InitPushTable();
UI16 PushtableType();
BOOL MachCfgRecv();
void SavePushTableIndex(void* buf,UI16 index);
void SetPushtableType(UI16  push_type);

void PushtabInital(PCMOD pmod);
UI32 SetClampPosPush(PATTR pdata, UI16 wData);

//通讯数据交互是否完成20211129 + //20211210 dyl
BOOL DataRecvFinish();
//设置通讯数据交互是否完成标记20211129 + //20211210 dyl
void SetDataRecvFlag(BOOL bRecv);
//数据交互是否进行中 + //20220106 dyl
BOOL IsDataRecvIng();
void SetDataRecv_IngFlag(BOOL bRecv);
//20241226 chj 威力士快速机模式下，部分参数变灰：关模高压二段,储料移动检测，储前电子尺检测，漏油检测功能数据赋值
void WLS_Fast_Spc();


/********************五支点新增*****************/ //20190813.cyx
extern TRANSMETER_ITEM g_dbTransMeterTable2[];//两位小数生成表
extern TRANSMETER_ITEM g_dbTransMeterTableCalcu[];//四位小数生成表
//五支点参数计算推力座
void CalculateData(double L1,double L2,double L3,double L4,double L5,double H,double h,double Hm,double limitangle);
#define PUSHTAB_CALCU_FILE	SAVE_PATH"pushtabcalcu.dat"//四位小数表用于查看
/*********************************************/

#define PUSHTABDB_INIT_FILE	SAVE_PATH"pushtab.dat"
#define PUSHTABDB_SRC_FILE  SAVE_PATH"pushtab%d.dat"


#ifdef __cplusplus
}
#endif 

#endif 


