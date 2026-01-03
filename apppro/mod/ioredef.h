#ifndef _IOREDEF_h
#define _IOREDEF_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "mod.h"
#include "strhash.h"
#include "sclist.h"
#include "protcmd.h"

#define MAX_REDEFINE 8
#define MAX_100  100
#define IO_INTPUT_GROUPS 10   //io输入点有10组（每组8个）
typedef struct tyIntNode
{
	_SCLIST_HEAD;
	int value;
}IntNode, *PIntNode;

typedef struct tyREDEF_INFO
{
    int table_type;       //表类型 enTableType(输入or输出)
    const char* map_path; //输入输出点map表路径
    int io_cnt;           //当前表的总共点数
    PHASH_TBL name_tbl;  //输入输出点hash表
	PIntNode addr_list; //输入输出点地址的增序链表

	UI32 phy_sn[MAX_REDEFINE]; //重定义物理点
	UI32 log_sn[MAX_REDEFINE]; //重定义逻辑点

	/* 临时变量地址 */
    int tmp_exchg0;  //原编号
    int tmp_exchg1;  //新编号
    int tmp_id;      //序列id号（从1开始代表使用）
    int tmp_comp;    //部件值关联变量
    int tmp_def;     //输入点值关联变量
    int tmp_def_data;//输入点文本关联变量
    int tmp_def_tran;//输入点值列表
}REDEF_INFO, *PREDEF_INFO;

typedef struct tyREDEF_DATA
{
	UI32 phy_sn[MAX_REDEFINE]; //重定义物理点
	UI32 log_sn[MAX_REDEFINE]; //重定义逻辑点
    UI32 swap_sn[2]; //交换点
}REDEF_DATA, *PREDEF_DATA;

typedef struct tyIO_SPEC
{
    //20190325 YX
    BOOL IPointLG[MAX_REDEFINE];//输入点反逻辑标识  1：反逻辑   0：正常  用于IO设置界面
    BOOL IPoint[MAX_100];//输入点反逻辑标识记录
    BOOL bUseForce[MAX_REDEFINE];//使用强制输入   1：使用强制输入 0：不使用强制输入
    BOOL bUseForceRec[MAX_100];//所有点的使用强制输入的记录
    BOOL bForceInput[MAX_REDEFINE];//强制输入    1：强制输入 0：不强制不输入
    BOOL bForceInputRec[MAX_100];//所有点的强制输入的记录
    BOOL m_bFirstInput;//开机后首次接收输入点表 标记位，开机后首次刷表使用，次数一次，使用后置false
}IO_SPEC, *PIO_SPEC;

void IoredefInital();

void IoredefCheckUpdate(int tbltype, int tblsn);
PREDEF_INFO IoredefGetInfo(int table_type);

// 比较当前和原始表，生成重定义列表
void IoredefMakeRedefine(PREDEF_INFO pinfo);

// 由重定义表生成新当前表，发送给下位机
void IoredefRedefineTable(int tbltype, PREDEF_DATA pdata);
//20190325 YX
void ClearFoceInPoint();
void SetCurrentIoNum(UI32 wNum);
UI32 GetCurrentIoNum();
UI8 GetUseForceRec(UI16 wNum);
UI8 GetIPoint(UI16 wNum);
UI8 GetForceInputRec(UI16 wNum);
//20190520.cyx
void SetIPointLG(int i,BOOL state);//设置输入点反逻辑标识
void SetUseForce(int i,BOOL state);//设置使用强制输入
void SetForceInput(int i,BOOL state);//设置强制输入
void RedefForcePoint(UI16 wNum , BOOL wbforce1 ,BOOL wbforce2,BOOL wbflagUseForce,BOOL wbflagForceInput);

void InitRedefData();//20190513.cyx 初始化强制输入表

void InitIOMap();//加载IO输入输出表
extern PHASH_TBL pmap_input;
extern PHASH_TBL pmap_output;

#define INPUT_FILE		DATA_PATH"input.map"
#define OUTPUT_FILE		DATA_PATH"output.map"

#ifdef __cplusplus
}
#endif

#endif
