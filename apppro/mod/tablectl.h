#ifndef _TABLECTL_h
#define _TABLECTL_h
#include "typedef.h"
#include "protcmd.h"
#ifdef __cplusplus
extern "C"
{ 
#endif

#define PI_INPUT1                   0x06160002 //输入点1
#define PO_OUTPUT1                  0x0616000A //输出点1

//20200727.cyx AI记忆开模算法 数据最多 12*20
#define MAX_OPENROWLENTH    24 //AI记忆开模算法最大列数 24BYTE=12UI16
#define MAX_OPENTABROW      20 //AI记忆开模算法最大行数
enum enTableType
{
    TB_MACHINE=0x0001,					//机械配置表，定义机器所包括的部件及个数。
    TB_O_ELEC=0x0002,					//输出点电气表，定义在每个动作阶段需要打开输出点的定义
    TB_I_LOGIC=0x0003,					//输入点逻辑定义表，定义每个物理输入点对应的部件逻辑输入点
    TB_O_LOGIC=0x0004,					//输出点逻辑定义表，定义每个物理输出点对应的部件逻辑输出点
    TB_OIL=0x0005,						//动作油路及优先级表，定义每个动作的的油路端口及优先级。
    TB_ACT=0x0006,						//动作流程表，定义主要动作的流程。
    TB_KEYMAP=0x0007,					//虚拟键盘映射表。
    TB_OPENCAL=0x0008,                  //AI记忆开模算法
    TB_ROBOT_PI=0x0009,
    TB_ROBOT_PO=0x000A,
    TB_PUMP = 0x000B,                   //pump表
};

enum enTableSN
{
    TB_CUR = 1,				//当前表 ？
    TB_ORI = 2,				//原始表（出厂表）
};

typedef struct tyTABLELIST * PTABLELIST;
typedef	struct tyTABLELIST{
		PPROT_TBL_HD pcur;
		PTABLELIST pnext;
	}TABLELIST;

typedef struct tyTBL_INPUT
{
	UI16	sn;//输入点序号，从1开始
	UI16	part_id;//输入点对应逻辑点所属的部件ID
	UI8		ind;//逻辑点在部件PI_ID子类数据中的偏移
    UI8		cfg;//低4位滤波次数(0-15),  高4位第一位-反逻辑，第二位-是否使用强制功能，第三位-强制(输入或不输入)，第四位不详
}TBL_INPUT;

//20241223 fqh pump表新增的结构体类型
typedef struct tyCUS_PORT
{
    UI8		hydr_type;  //油路表类型
    UI8		hydr_sn;//油路表行号
    UI16	part_id;//部件id
    UI16	act_id;//动作id
    UI16	port;//主油路
    UI16	pump;//从站配置
    UI16    act_flag;//动作特殊标记
}CUS_PORT,*PCUS_PORT;

//20191210 机械配置表，定义机器所包括的部件及个数。
typedef struct tySUB_PART_INFO
{
    UI8		part_type;
    UI8		num;
}SUB_PART_INFO, *PSUB_PART_INFO;

typedef struct tyTBL_INPUT TBL_OUTPUT;	//  输出表结构和输入表结构一样

void TableRecCtl(PPROT_TBL_PKT ppkt, I32 dat_len,I32 row_len);
void RequestTable(BOOL isOri);//是否获得原始数据
PPROT_TBL_HD GetTableByType(UI16 type,UI16 sn);
void ProcessIo();//IO点数据分配到dic数据中
TBL_OUTPUT * TableFindOutput(PPROT_TBL_HD ptb,UI16 sn);
TBL_INPUT * TableFindInput(PPROT_TBL_HD ptb,UI16 sn);

void AddOpenCalTable(PPROT_TBL_HD ptb);
void AddPumpTable(PPROT_TBL_HD ptb);
UI8  GetSubPartNum(UI8 part_type);

//发送单个表给下位机
BOOL ProtSendTbl(PPROT_TBL_HD ptable);

#ifdef __cplusplus
}
#endif

#endif



