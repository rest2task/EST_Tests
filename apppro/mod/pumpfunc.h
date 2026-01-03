#ifndef _PUMPFUNC_h
#define _PUMPFUNC_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "mod.h"
#include "strhash.h"
#include "sclist.h"
#include "protcmd.h"
#include "ioredef.h"

#define MAX_REDEFINE 8
#define MAX_100  100
#define IO_INTPUT_GROUPS 10   //io输入点有10组（每组8个）


typedef struct tyPUMP_INFO
{
    int table_type;       //表类型 enTableType(输入or输出)
    const char* map_path; //输入输出点map表路径
    int io_cnt;           //当前表的总共点数
    PHASH_TBL name_tbl;  //输入输出点hash表
    PIntNode addr_list; //输入输出点地址的增序链表

    //UI32 phy_sn[MAX_REDEFINE]; //重定义物理点
    //UI32 log_sn[MAX_REDEFINE]; //重定义逻辑点

    /* 临时变量地址 */
    //int tmp_exchg0;  //原编号
    //int tmp_exchg1;  //新编号
    //int tmp_id;      //序列id号（从1开始代表使用）
    int tmp_comp;    //部件值关联变量
    int tmp_def;     //输入点值关联变量
    int tmp_def_data;//输入点文本关联变量
    int tmp_def_tran;//输入点值列表
}PUMP_INFO, *PPUMP_INFO;


void PumpInital();
void InitPUMPMap();//加载IO输入输出表

PPUMP_INFO PumpGetInfo(int table_type);



#define PUMP_FUNC_FILE	DATA_PATH"pump_func.map"

#ifdef __cplusplus
}
#endif

#endif
