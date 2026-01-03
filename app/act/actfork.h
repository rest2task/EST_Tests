#ifndef _ACTFORK_H_
#define _ACTFORK_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"
#include "actctrl.h"
#include "tbl.h"

#define FORK_CMP_NUM	4
#define FORK_TBL_SIZE 	50
#define FORK_TBL_VER	0x01

typedef enum enEXP_OP_TYPE
{
	EQ_OP,					// ==
	NO_EQ_OP,				// !=
	GT_OP,					// >
	LS_OP,					// <
	GT_EQ_OP,				// >=
	LS_EQ_OP,				// <=
}EXP_OP_TYPE;

typedef enum enLOGIC_TYPE
{
	AND_LG = 1,
	OR_LG = 2
}LOGIC_TYPE;

typedef enum enVAR_TYPE
{
	VAR_NULL = 0,			/*无效变量*/
	VAR_CONST,				/*常数*/
	VAR_POINTER ,			/*指针类型*/
	VAR_IND,				/*动作变量, 见VAR_IND_TYPE定义*/
	VAR_SUB,				/*子类数据*/

	VAR_SRC_SUB = 0x10,		/*源部件子类数据, 0x10- 0x1f, 减0x10对应子类ID,,  PO_ID - ST_ID */
	VAR_DST_SUB = 0x20,		/*目的部件子类数据, 0x20- 0x2f, 减0x20对应子类ID, PO_ID - ST_ID */
	VAR_OTH_SUB = 0x30,	    /*其他部件子类数据, 0x30- 0x3f, 减0x30对应子类ID, PO_ID - ST_ID, 部件由EXP_VAR.ref_id指定 */
}VAR_TYPE;

typedef enum enVAR_IND_TYPE
{
	VAR_MOVST,					/*动作运动状态*/				
	VAR_STEP,					/*动作当前步*/
	VAR_RET,					/*动作返回状态值*/
	VAR_ACT_ST,					/*动作运行状态*/
	VAR_SETTM,					/*动作设定时间*/
	VAR_P_ACT_ID = 6,			/*动作父动作ID*/
	VAR_RUN_MS,					/*动作运行时间MS*/
	VAR_STEP_100MS = 9,			/*当前步的运行时间100MS*/
	VAR_NEXT_STEP,				/*动作下一步*/
	VAR_METER = 0x0f,			/*电子尺*/
	VAR_ACT_DEF_BEGIN = 0x10	/*由每个动作定义的运动变量索引开始值*/
}VAR_IND_TYPE;

typedef struct tyEXP_VAR
{
	UI8 type;			/* 最高位置1表示为有符号数，&0x7f = VAR_TYPE类型*/
	UI8 rev[3];			//对齐		
	UI16 ind;			/*CMP_CONST为常数的具体值，其他为索引值*/
	UID	ref_id;			/*变量所引用的部件ID*/
}EXP_VAR, *PEXP_VAR;

//逻辑比较项
typedef struct tyFORK_CMP
{
	EXP_VAR var1;				//参数1
	EXP_VAR var2;				//参数2
	UI8		exp_op;				//参数1与参数2逻辑比较符 EXP_OP_TYPE
	UI8		lg_op;				//与前比较项的逻辑运算 LOGIC_TYPE
	UI8		rev[2];				//对齐
}FORK_CMP, *PFORK_CMP;

#define INVALIDE_EN	0xffff
#define SKIP_STEP	INVALIDE_EN
#define SKIP_NSTEP	INVALIDE_EN
#define SKIP_MOV	INVALIDE_EN
#define SKIP_OPER	INVALIDE_EN
#define SKIP_RET	INVALIDE_EN
#define SKIP_ERR	INVALIDE_EN
//动作监控表
typedef struct tyACT_FORK
{
	UID		src_act_sn;			/*被监控的动作ID*/
	UID		dst_act_id;			/*条件满足，命令执行的目的动作*/
	UI16	dst_act_sn;
	UI8		src_cmd;			/*条件满足时，被监控动作执行的操作*/
	UI8		dst_cmd;			/*条件满足时，目的动作执行的操作*/
	UI32	para;				/*动作函数参数*/
	UI16	rev1[4];			//保留，0
	UI16	dly_ms;				/*动作延迟时间*/
	UI16	en_oper_mode;		/*操作模式，0xFFFF表示忽略操作模式检查*/
	UI16	en_mov_st;			/*0xffff表示不检查运动状态*/
	UI16	en_act_ret;			/*缺省为0，即ACT_OK, 0xffff表示不检查返回值*/
	UI16	en_step;			/*0xffff表示不检查动作步*/
	UI16	en_next_step;		/*下一步, 0xffff表示不检查*/
	UI16	en_err;				/*动作错误状态, 0xffff表示不检查*/
	UI8		cmp_num;			/*比较项数目*/
	UI8		cmp_type;			/*比较项类型，目前只有一种*/
	FORK_CMP	cmp[FORK_CMP_NUM];		/*比较项，最大4个*/
	
	UI8		bexec;				/*分支命令是否已经执行*/
	UI8		rev2[3];
	PACT	pdst_act;			/*新任务的动作结构指针*/
}ACT_FORK, *PACT_FORK;

#define ACT_FORK_EXT_LEN	OFFSETOF(PACT_FORK, bexec)

typedef struct tyACT_TSK* PACT_TSK;

PACT_FORK GetActForkList(UID act_id, UI16* pnum);
BOOL ScanActForkTbl(PACT_TSK ptsk);
void CreateActForkTbl(UI16 oper_mode);
BOOL ActForkTblInit(void);
BOOL ActForkTblInsert(PACT_FORK pfork);

#define DEF_FORK(x)	DefaultFork(&x)	
#define ActEndFork(s_act, d_act)		ActNoCmpFork(s_act, d_act, SKIP_STEP, SKIP_NSTEP, SKIP_MOV, SKIP_ERR, OM_AUTO, ACT_END, TASKSTNDDLY, 0, FALSE, 0)
#define ActEndNoDelyFork(s_act, d_act)		ActNoCmpFork(s_act, d_act, SKIP_STEP, SKIP_NSTEP, SKIP_MOV, SKIP_ERR, OM_AUTO, ACT_END, 0, 0, FALSE, 0)
#define ActEndModeFork(s_act, d_act, oper)	ActNoCmpFork(s_act, d_act, SKIP_STEP, SKIP_NSTEP, SKIP_MOV, SKIP_ERR, oper, ACT_END, TASKSTNDDLY, 0, FALSE, 0)
#define ActEndModeNoDelyFork(s_act, d_act, oper)	ActNoCmpFork(s_act, d_act, SKIP_STEP, SKIP_NSTEP, SKIP_MOV, SKIP_ERR, oper, ACT_END, 0, 0, FALSE, 0)
#define ActEndErrFork(s_act, d_act, err)	ActNoCmpFork(s_act, d_act, SKIP_STEP, SKIP_NSTEP, SKIP_MOV, err, OM_AUTO, ACT_END, TASKSTNDDLY, 0, FALSE, 0)
#define ActErrAbortFork(s_act, d_act, para, oper)	ActNoCmpFork(s_act, d_act, SKIP_STEP, SKIP_NSTEP, SKIP_MOV, SKIP_ERR, oper, ACT_ERR_ABORT, TASKSTNDDLY, para, FALSE, 0)
#define ActSimpleFork(s_act, d_act, step, mov, oper, bhold)	ActNoCmpFork(s_act, d_act, step, SKIP_NSTEP, mov, SKIP_ERR, oper, SKIP_RET, TASKSTNDDLY, 0, bhold, 0)
#define ActNoDlyFork(s_act, d_act, step, mov, oper, bhold)	ActNoCmpFork(s_act, d_act, step, SKIP_NSTEP, mov, SKIP_ERR, oper, SKIP_RET, 0, 0, bhold, 0)
#define ActNextStepFork(s_act, d_act, next_step, mov, bhold)	ActNoCmpFork(s_act, d_act, SKIP_STEP, next_step, mov, SKIP_ERR, OM_AUTO, SKIP_RET, TASKSTNDDLY, 0, bhold, 0)
#define ActStepFork(s_act, d_act, step, bhold)	ActNoCmpFork(s_act, d_act, step, SKIP_NSTEP, SKIP_MOV, SKIP_ERR, OM_AUTO, SKIP_RET, TASKSTNDDLY, 0, bhold, 0)
#define ActStepModeFork(s_act, d_act, step, oper, bhold)	ActNoCmpFork(s_act, d_act, step, SKIP_NSTEP, SKIP_MOV, SKIP_ERR, oper, SKIP_RET, TASKSTNDDLY, 0, bhold, 0)
#define ActStepMovFork(s_act, d_act, step, mov, bhold)	ActNoCmpFork(s_act, d_act, step, SKIP_NSTEP, mov, SKIP_ERR, OM_AUTO, SKIP_RET, TASKSTNDDLY, 0, bhold, 0)
#define ActMovFork(s_act, d_act, mov, bhold)	ActNoCmpFork(s_act, d_act, SKIP_STEP, SKIP_NSTEP, mov, SKIP_ERR, OM_AUTO, SKIP_RET, TASKSTNDDLY, 0, bhold, 0)
#define ActMovModeFork(s_act, d_act, mov, oper, bhold)	ActNoCmpFork(s_act, d_act, SKIP_STEP, SKIP_NSTEP, mov, SKIP_ERR, oper, SKIP_RET, TASKSTNDDLY, 0, bhold, 0)
#define ActStepMovParaFork(s_act, d_act, step, mov, para, bhold)	ActNoCmpFork(s_act, d_act, step, SKIP_NSTEP, mov, SKIP_ERR, OM_AUTO, SKIP_RET, TASKSTNDDLY, para, bhold, 0)
#define ActMovForkmaul(s_act, d_act, mov, bhold,para)	ActNoCmpFork(s_act, d_act, SKIP_STEP, SKIP_NSTEP, mov, SKIP_ERR, OM_MANUAL, SKIP_RET, TASKSTNDDLY, para, bhold, 0)

void ActNoCmpFork(UI16 src_act, UI16 new_act, UI16 step, UI16 next_step, UI16 mov_st, UI16 err_bit, UI16 oper_mode, UI16 act_ret, UI16 dly_ms, UI32 para, BOOL bhold, UI16 act_sn);
void DefaultFork(PACT_FORK pfork);
void ActStopFork(UI16 src_act, UI16 dst_act, UI16 mov_st, UI16 oper_mode);
void ActAbortFork(UI16 src_act, UI16 dst_act, UI16 mov_st, UI16 oper_mode);
void ActForkTblClear(void);
void ActForkTblPrint(void);
void ActForkTblOutDate(BOOL bset);
void ActForkTblRefresh(BOOL bforce);

#ifdef __cplusplus
}
#endif

#endif

