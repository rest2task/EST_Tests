#ifndef _ACTCTRL_H_
#define _ACTCTRL_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"
#include "acttbl.h"
#include "actfork.h"

#define MAX_TSK_NUM	20
#define MAX_TSK_PANEL_SHOW	3

#define ACT_TSK_PLC_DEF_LEN	4
#define SUB_CMD_QUE_SIZE	20

#define TASKNODLY     0
#define TASKSTNDDLY   20

//动作任务状态
typedef enum enTASK_STATE
{
	TSK_IDLE = 0,				//空闲					
	TSK_DLY,					//任务处于开始运行前的延时等待状态
	TSK_RUN,					//任务正常执行
	TSK_SUSPEND,				//任务被挂起
}TASK_STATE;

typedef enum enTSK_FORK_MODE
{
	FIRST_FORK,					/*第一个动作任务*/
	SUSPEND_FORK,				/*由前一个任务启动本任务，其自身挂起*/
	SYNC_FORK					/*由前一个任务启动本任务，其自身也同步运行*/
	//END_FORK					/*由前一个任务结束后启动本任务*/
}FORK_MODE;

//任务监控命令
typedef enum enFORK_CMD
{
	F_CMD_NULL = 0,				//不执行
	F_CMD_RUN = 1,				//运行一个新的动作任务			
	F_CMD_HOLD,					//挂起动作任务		
	F_CMD_REAT,					//激活动作任务	
	F_CMD_STOP,					//结束动作任务
	F_CMD_ABORT					//强制结束动作任务
}FORK_CMD;

typedef struct tySUB_QUE* PSUB_QUE;
/*监控命令队列，如果任务运行到某一状态，需要同时执行多个监控命令，
按监控表的先后顺序放入到列表中*/
typedef struct tySUB_QUE
{
	PSUB_QUE pnext;					//执行队列下一个元素
	UI8 bused;						//是否在使用
	UI8 bplc;						//是否由PLC程序
	UI8 src_cmd;					//对被监控任务执行的命令	
	UI8	dst_cmd;					//新动作的执行的命令
	UI16 dly_ms;					//新动作任务的执行延迟
	UI16 act_sn;					//新动作序号
	PACT pdst_act;					//新动作结构体
	UI32 para;						//新动作任务参数
}SUB_QUE, *PSUB_QUE;

#pragma pack(1)
#define ACT_TSK_ST_DEF_SIZE			40
typedef struct tyACT_TSK_ST
{
	UI16			act_id;										/*0x00  动作 ID*/
	UI16			plc_part;									/*0x01 PLC使用，动作的PART ID*/
	UI16			plc_act;									/*0x02 PLC使用，动作的act  ID*/
	UI16			rev1;										//保留
	
	/*CMP_VAR_IND start*/
	UI16			mov_st;										/*0x03 ACT_MOV_STATE, 由动作函数修改*/
	UI16			step;										/*0x04 动作步，由动作函数修改*/
	UI16			act_ret;									/*0x05 动作函数执行返回结果*/
	UI16			tsk_st;										/*0x06 任务状态 TASK_STATE*/
	UI32			act_tm_n;										//0x07  526 动作时间2  set time, 由动作函数修改
	UI16			parent_act_id;								//0x08 PLC 使用，用于重新运行父动作
	UI32			act_run_ms;									//动作运行的时间，MS单位, 最大0xffff
	UI16			step_100ms_h;									//动作步的运行时间，100MS单位
	UI16			next_step;									//动作下一步
	UI16			fork_mode;									/*0x09 本任务被启动方式*/
	UI32			para;										/*0x0A 任务参数*/
	UI16			meter_ch;									//动作任务启动时传递的参数
	UI16			act_state;									/*动作错误标志*/
	UI16			act_sn;										//动作编号，一般等于act_id
	UI16			rev2[3];
	/*CMPVAR_IND end*/
	
	UI32			dly_100us;									/*任务启动延时*/
	UI32			fork_100us;									/*任务开始调度时间*/
	UI32			mov_start_ms;								/*动作进入运动状态的时系统毫秒, 由动作函数修改*/
	
	UI8			act_run_cn_start;			//动作启动列队顺序
	UI8			act_run_cn;					//动作启动次数，多少个周期运行一次
}ACT_TSK_ST, *PACT_TSK_ST;

#pragma pack()
#define ACT_TSK_ST_SIZE (sizeof(ACT_TSK_ST)/sizeof(UI16) > ACT_TSK_ST_DEF_SIZE ? sizeof(ACT_TSK_ST)/sizeof(UI16) : ACT_TSK_ST_DEF_SIZE)

typedef struct tyACT* PACT;
typedef struct tyACT_TSK* PACT_TSK;
typedef struct tyACT_FORK* PACT_FORK;
typedef struct tyACT_TSK
{
	PACT_TSK_ST	pst;	
	PACT		pact;						/*本任务动作结构体*/
	PACT_TSK	pptsk;						/*启动本动作的任务*/
	PACT_FORK	pfork;						/*当前的任务监控表指针*/
	PSUB_QUE	psub;						/*动作被挂起后，在重新激活前需运行的监控命令队列*/
	LAST_E_PO	last_e_po;

	/*动作运行时需要表索引*/
	PE_PO_ROW      	pepo;       			//指向电气阀输出表
	UI16			bexec_Pohh[MAX_E_PO_ROW][MAX_E_PO_TBL];//同一个监控动作的6个阀是否满足条件过的标记20190905HH
	UI16			epo_num;       			//动作电气配置条数
	UI16			hydr_num;				//油路优先级数目
	PHY_PRI_ROW		phydr;					//指向油路优先级表
	//PHY_PRI_ROW		pstep_hydr;			//当前步的油路优先级
	/**********************/
	
	UI16		hold_save_tsk_st;			/*动作被挂起前的状态*/
	UI16		fork_num;					//任务监控表数目
	UI32		act_set_start_ms;			//ACT time set start 100ms，任务操作计时设定时间
	UI8			bplc;						//是否由PL程序启动本任务
	UI8			bstop_cmd_inque;			/*psub执行的子命令队列里是否已有F_CMD_END，如有应不插入新的子命令，否则报错*/
	UI8			end_fork_disable;			/*是否允许动作结束后转接到下一个动作. 如果本任务是由于上一任务异常出错后启动的, 置1 */
	UI8			skip_hold_stop;				//任务被挂起是是否需要调用动作停止函数，任务激化执行监控命令时 置为TRUE		
}ACT_TSK, *PACT_TSK;

extern ACT_TSK 		act_tsk[MAX_TSK_NUM];


BOOL ForkActTsk(PACT pact, FORK_MODE mode, PACT_TSK pparent_tsk, UI32 para, UI16 dly_ms, BOOL bplc, UI16 act_sn);
BOOL ActTskCmd(PACT_TSK ptsk, PACT pdst_act, UI8 src_cmd, UI8 dst_cmd, UI16 dly_ms, UI32 para, BOOL bplc, UI16 act_sn);
BOOL ForkAct(PACT pact, FORK_MODE mode, PACT_TSK pparent_tsk, UI32 para, UI16 dly_ms);
BOOL ActTskIsEmpty(void);
BOOL ActTskIsEmptyExpAct(UI16 act_id);
BOOL ActCtrInit(void);
void ClearAllActTask(void);
void ClearAllActTaskEnf(void);
void ActTaskLoop(void);
BOOL EndActTsk(PACT_TSK ptsk, BOOL babort);
BOOL SentTskState(void);
BOOL ActIsInTsk(UI16 act_id);
BOOL PartActIsInTsk(UI16 part_id);

BOOL ActForkTblIsUsing(void);

/*PLC call function*/
void PLCReActiveActTsk(UI16 act_id);
void PLCHoldActTsk(UI16 act_id);
void PLCRunActTsk(UI16 act_id, UI16 para, UI16 dly_ms);
void PLCHoldAndRunActTsk(UI16 act_id, UI16 parent_id, UI32 para, UI16 dly_ms);

#ifdef __cplusplus
}
#endif

#endif



