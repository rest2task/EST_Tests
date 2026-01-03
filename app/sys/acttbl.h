#ifndef _ACTTBL_H_
#define _ACTTBL_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"
#include "tbl.h"
#include "ioproc.h"

#define MAX_PI_LOG_ROW	MAX_PI_NUM
#define MAX_PO_LOG_ROW	MAX_PO_NUM
#define MAX_E_PO_ROW	10
#define IO_TBL_VER	0x01
#define MAX_E_PO_TBL  6
#define MAX_E_PO_FORK_HH  1  //比较项最大数目

#pragma pack(1)

//机械手输入点标准表
typedef struct tyROB_PI_LOG
{
	UID 	part_id;			//逻辑点所属的部件ID
	UI8		ind;				//逻辑点在部件输入子类的偏移
	UI8 	type;				//类型，0:不使用，1:常规，2:欧规12，3:欧规67
	UI8		used_def;			//高4位是否使用，0:不使用，1:使用，2:必须使用
								//低4位默认值，0表示后期添加，1表示默认值
	UI8		rev[3];				
}ROB_PI_LOG, *PROB_PI_LOG;

//机械手输出点标准表
typedef struct tyROB_PO_LOG
{
	UID 	part_id;			//逻辑点所属的部件ID
	UI8		ind;				//逻辑点在部件输出子类的偏移
	UI8 	type;				//类型，0:不使用，1:常规，2:欧规12，3:欧规67
	UI8		used_def;			//高8位是否使用，0:不使用，1:使用，2:必须使用
								//低8位默认值，0表示后期添加，1表示默认值
	UI8		rev[3];					
}ROB_PO_LOG, *PROB_PO_LOG;


//输入逻辑点定义表
typedef struct tyPI_LOG
{
	UI16 	pi_sn;				//硬件输入点序号 ，从1开始
	UID 	part_id;			//逻辑点所属的部件ID
	UI8		ind;				//逻辑点在部件输入子类的偏移
	UI8		cfg;				//最高置1，表示反逻辑，第4位为滤波次数。
}PI_LOG, *PPI_LOG;

//输出逻辑点定义表
typedef struct tyPO_LOG
{
	UI16 	po_sn;			//硬件输出点序号，从1开始
	UID 	part_id;		//逻辑点所属的部件ID
	UI8		ind;			//逻辑点在部件输出子类的偏移
	UI8		rev;
}PO_LOG, *PPO_LOG;

//电气表的输出点
typedef struct tyE_PO
{
	UI16  	part_name;  		//15-4位为部件ID, 3-0填0
	UI16	po_ind;				//输出点编号,只用低8位。
	UI16    onoff_delay;		//低8为输出点输状态及延时; 高8为做关闭操作时延时，以10ms单位
}E_PO, *PE_PO;

//电气表最近打开的输出点
typedef struct tyLAST_E_PO
{
	UI16	step;					//动作步
	UI16	num;					//输出点数目
	UI8		po[MAX_E_PO_TBL];		//硬件输出点表
	UI8		delay[MAX_E_PO_TBL];	//输出点关延迟
}LAST_E_PO, *PLAST_E_PO;

typedef struct tyEXP_VAR_PO
{
	UI16  type;					// 最高位置1表示为有符号数，低位& 0x7fff = VAR_TYPE类型,
	UI16 sub;                   //部件子类号
	UI16 ind;					//CMP_CONST为常数的具体值，其他为索引值
	UI16 ref_id;				//变量所引用的部件ID
}EXP_VAR_PO, *PEXP_VAR_PO;

//逻辑比较项
typedef struct tyFORK_CMP_PO
{
	EXP_VAR_PO var1;				//参数1
	EXP_VAR_PO var2;				//参数2
	UI16		exp_op;				//参数1与参数2逻辑比较符 EXP_OP_TYPE
	UI16		lg_op;				//与前比较项的逻辑运算 LOGIC_TYPE
	UI16		rev[2];				//预留 
}FORK_CMP_PO, *PFORK_CMP_PO;



//电气表项
typedef struct tyE_PO_ROW
{
	UI16 	act_id;   				//动作ID
	UI16 	step;     				//动作区间（HI8开始step,LOW8结束step）
	E_PO	e_po[MAX_E_PO_TBL];		//电气阀自定义状态和编号
	UI16		add_condition; 		/*6个阀是否需要以下附加条件，按位0-5位代表6个阀*/
	UI16		en_oper_mode;		/*0xffff表示忽略操作模式检查	,非0xffff则采用==比较操作  */
	UI16		cmp_num_po; 		/*比较项数目最大1个比较项*/
	FORK_CMP_PO cmp[MAX_E_PO_FORK_HH]; /*比较项，最大1个*/			
}E_PO_ROW, *PE_PO_ROW;

typedef struct tyHYDR_PRI
{
	UI16 	act_id;
	UI16 	step;
	UI16	port_bits;
	UI16	priority;
	//UI16	rev;					//保留缺省值为0，用于指定是否模拟量输出或者伺服输出。
}HY_PRI_ROW, *PHY_PRI_ROW;

#if 0

typedef struct tyE_PO_TBL
{
	PE_PO_ROW	prow;  //行指针
	TBL_HD		hd;  //表头
}E_PO_TBL, *PE_PO_TBL;

typedef struct tyPO_LOG_TBL
{
	TBL_HD		hd;
	PPO_LOG		prow;
}PO_LOG_TBL, *PPO_LOG_TBL;

typedef struct tyPI_LOG_TBL
{
	TBL_HD		hd;
	PPI_LOG		prow;
}PI_LOG_TBL, *PPI_LOG_TBL;
#endif


#pragma pack()

BOOL POLogTblProc(PTBL ptbl);
BOOL PILogTblProc(PTBL ptbl);
PE_PO_ROW  ExPOFilt(UI16 id, UI16 *pnum);//动作电气表筛选
PHY_PRI_ROW HydrPriFilt(UI16 id, UI16 *pnum);//动作电气表筛选
void ExPOOut(PACT pact, PE_PO_ROW prow, int row_num, PLAST_E_PO plst);//动作查帅选表规则输出
void ExPOClear(PLAST_E_PO plst);
PHY_PRI_ROW GetActStepPortPri(PACT pact, PHY_PRI_ROW prow, int num);
BOOL InitActTbl(void);
PTBL GetHydrTbl(void);

#ifdef __cplusplus
}
#endif

#endif






