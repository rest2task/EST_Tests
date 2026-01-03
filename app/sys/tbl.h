#ifndef _TBL_H_
#define _TBL_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"
#include "protcmd.h"

#pragma pack(1)

#define TBL_DB_VER	0x01

#define MAX_TBL_LIST_NUM	20
#define MAX_DYNC_TBL_NUM	10
#define MAX_TBL_SN			3

//表类型
typedef enum enTBL_TYPE
{
	T_MACHINE_SUB = 1,					//	1	机器部件配置表
	T_E_PO,								//	2	电气输出表
	T_PI_LOG,							//	3	逻辑输入点定义表
	T_PO_LOG,							//	4	逻辑输出点定义表
	T_HYDR_PRI,							//	5	油路及其优先级表
	T_ACT_FORK,							//	6	动作监控表
	T_KEY_MAP,							//	7	键盘映射表
	T_OPN_RAMP,							//	8	开模斜率记忆表
	
	T_ROBOT_PI,							//	9	机械手输入点配置表
	T_ROBOT_PO,							//	10	机械手输出点配置表
	T_TBL_END
}TBL_TYPE;

//表头，一个表由表头和表行数据组成 
typedef struct tyTBL_HD
{
	UI16	type;						//类型，TBL_TYPE
	UI8		ver;						//表数据版本， 目前为0x01
	UI8		sn;							//表的序号，一个类型的表可以有多个
	UI16	row;						//行数
	UI16	max_row;					//允许的最大函数
	//UI16	ext_row_len;				/*存储或者通信时, 行有效数据长度*/
	UI16	row_len;					//每行的数据长度
}TBL_HD, *PTBL_HD;

//表
typedef struct tyTBL
{
	void*	prow;						//指向表的行数据
	TBL_HD	hd;							//表头
}TBL, *PTBL;

#define TBL_STOR			0x01	/*表是否在存储区有存储*/
#define TBL_DYNC			0x02	/*表是否需要动态可变，如是表必须复制到内存*/
#define TBL_UPDATE			0x04	/*表已接收到新数据，需要保存*/
#define TBL_USER			0x08	/*表是否要存储在用户表配置区*/

#define TBL_STOR_PANEL		0x10	/*表是否在面板存储区有存储*/
#define TBL_DYNC_SRAM		0x20	/*表是否需要动态可变且读取变更不平凡，如是表必须复制到内存，且可以使用SRAM*/

typedef struct tyTBL_IND
{
	UI8		ind[MAX_TBL_SN + 1];
}TBL_IND, *PTBL_IND;

typedef struct tyTBL_LST
{
	UI8		cfg_db_id;
	UI8		attr;
	PTBL	ptbl;
}TBL_LST, *PTBL_LST;

//表类型信息
typedef struct tyTBL_TYPE_INFO
{
	UI16 	type;					//表类型
	UI8		ver;					//版本
	UI8		attr_bits;				//属性位
	UI16	ext_row_len;			/*存储或者通信时, 行有效数据长度*/
	UI16	row_len;				//每行数据长度
	UI16	max_row;				//最大行
}TBL_TYPE_INFO, *PTBL_TYPE_INFO;

typedef struct tyTBL_RX_ST
{
	UI8		brx;
	UI8		tbl_ver;
	UI8		tbl_sn;
	UI16	tbl_type;
	UI16	total_row;
	UI16	cur_row;
	UI16	row_len;
}TBL_RX_ST, *PTBL_RX_ST;

#pragma pack()

//向表插入一行，
BOOL TBLInsertRow(PTBL ptbl, void* prow);
BOOL TBLSort(PTBL ptbl);
void TBLClear(PTBL ptbl);
void* TBLGetRow(PTBL ptbl, UID key, UI16* pnum);
PTBL TBLGet(UI16 type, UI8 ver, UI8 sn);
BOOL TBLRspRx(PPROT_TBL_PKT ppkt, int dat_len);
PTBL TBLAlloc(UI16 type, UI8 ver, UI8 sn, UI8 attr, UI16 max_row, UI16 row_len);
const TBL_TYPE_INFO* TBLGetInfo(UI16 type);
PTBL TBLCopy(PTBL pdst, PTBL psrc);
PTBL_LST AddTblToInd(PTBL ptbl, UI8 stor_id, UI8 attr, UI16 new_sn);
BOOL TBLInit(void);
BOOL TBLReqRx(PPROT_TBL_PKT ppkt);
void InitTBLInPanel(void);

#ifdef __cplusplus
}
#endif

#endif

