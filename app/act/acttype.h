#ifndef _ACTTYPE_H_
#define _ACTTYPE_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "part.h"
#include "actctrl.h"
#include "movectrl.h"
#include "acttbl.h"

/*pact->ftaskstat, 0-7 bit set action task common status bit, 
				15-8 bits for task special status bits, defined in each partion own head file
*/
#define	INTF_RUN		0x0001
//#define	INTF_HOLD		0x0002      // 3   ; task is hold
//#define	INTF_NSTEP		0x0004      // 0   ; step is new
//#define	INTF_OFF		0x0010      // 4   ; OFF PROCESS
//#define	INTF_SUB		0x0020      // 5   ; SUB-TASK
//#define	INTF_DLY		0x0080      // 7   ; DELAY 100ms

#define INTF_ERR_MSK	0xff00		//	act ftaskstat error mask	
#define INTF_CEC		0x0001      // 2   ; CONTINUE 1 ECHO
//#define INTF_STEPNEW	0x0002		//	   ; 新步开始，用于判断是否要进行阀和液压输出。
#define INTF_ERR		0x0100      // 6   ; ERROR
#define INTF_ERR2		0x0200		//	   ; ERROR 2
#define INTF_ERR3		0x0400		//	   ; ERROR 8

typedef enum enACT_STEP
{
	INIT1_STEP	= 0x0000,					/*动作初始化，或前期条件判断*/
	INIT2_STEP	= 0x0100,					/*动作初始化，或前期条件判断*/
	INIT3_STEP	= 0x0200,					/*动作初始化，或前期条件判断*/
	INIT4_STEP = 0x0300,					/*动作初始化，或前期条件判断*/
	INIT5_STEP = 0x0400,					/*动作初始化，或前期条件判断*/
	
	PRE_MOV_STEP 	= 0x0F00,				/*动作进入运动前状态，虚拟步，动作函数在INIT_STEP转STAGE_N_STEP由动作控制函数自动插入*/

	STAGE_1_STEP = 0x1000,				
	STAGE_2_STEP = 0x1100,
	STAGE_3_STEP = 0x1200,
	STAGE_4_STEP = 0x1300,
	STAGE_5_STEP = 0x1400,
	STAGE_6_STEP = 0x1500,
	STAGE_7_STEP = 0x1600,
	STAGE_8_STEP = 0x1700,
	STAGE_9_STEP = 0x1800,
	STAGE_10_STEP = 0x1900,
	STAGE_11_STEP = 0x1A00,
	STAGE_12_STEP = 0x1B00,
	STAGE_13_STEP = 0x1C00,
	STAGE_14_STEP = 0x1D00,
	STAGE_15_STEP = 0x1E00,
	STAGE_16_STEP = 0x1F00,
	STAGE_17_STEP = 0x2000,
	STAGE_18_STEP = 0x2100,
	STAGE_19_STEP = 0x2200,
	STAGE_20_STEP = 0x2300,
	STAGE_21_STEP = 0x2400,
	STAGE_22_STEP = 0x2500,

	/*特殊步*/
	STAGE_SPEC1_STEP = 0x8000,
	STAGE_SPEC2_STEP = 0x8100,
	STAGE_SPEC3_STEP = 0x8200,

	END_STEP = 0xfE00,						/*动作结束步，关阀动作结束*/

	PRE_FIN_STEP = 0xFF00,					/*动作任务结束前，虚拟步，动作控制函数自动插入*/
	INVALID_STEP = 0xFFFF
}ACT_STEP;

#define M_STEP_GAP		0x100
#define MIN_STEP(x)		((x)&0xff)
#define MAIN_STEP(x)	(((x) >> 8) &0xff)

typedef enum enACT_RET
{
	ACT_OK = 0,			/*动作函数返回OK，表示动作可能进入一个新的step或者在原步等待*/
	ACT_ERR,			/*动作运行出错，但动作继续运行*/

	ACT_ERR_ABORT,		/*动作运行出错，动作异常结束，有可能需要启动其他动作*/
	ACT_INTF_OFF,		/*手动结束*/
	ACT_END,			/*动作结束*/
	INVALD_ACT_RET	= 0xffff
}ACT_RET;

/*动作运行状态，主要用于动作嵌入和转接*/
typedef enum enACT_MOV_STATE
{
	ACT_MOV_INIT = 0,				/*动作初始化，做必要的条件检查*/
	ACT_MOV_PRE  = 1,				/*动作初始化完成，条件满足，开始运动前, 用于动作运动前的动作插入*/
	ACT_MOVING	= 2,				/*动作开始运动，开阀*/
	ACT_MOV_OFF 	=3,				/*运动结束，关阀*/
	ACT_MOV_END_PRE 	=4,			/*已完成运动结束的后期处理，动作任务即将正常结束，用于动作结束前的任务插入*/
	ACT_MOV_END =5,					/*任务正常结束*/
	ACT_MOV_ABORT = 6	,			/*任务异常结束*/
	INVALD_MOV_ST = 0xffff,	
}ACT_MOV_STATE;

UI16 Get_M_Step_No(int step);

#define GET_M_STEP(step)		((step) & 0xff00)
#define GET_M_STEP_NO(step)	((WORD)Get_M_Step_No((int)step))
#define ACT_SN_BIT(sn)			((UI16)1 << (sn -1))


/*
typedef struct tyMOV_PARA
{
	UI16		id;
	UI16		stage;
	UI16		press;
	UI16		flow;
	UI16		pos;
	UI16		time;
	UI16		pmplmt;
}MOV_PARA, *PMOV_PARA;
*/

#define ACT_NTYPE	0x00		//缺省类型，一般动作
#define ACT_NORM	0x01		
#define ACT_CTRL	0x02		//动作调度动作
#define ACT_CTRB	0x04		//动作调度动作   但名称不上传


typedef struct tyACT* PACT;
typedef  ACT_RET  (*ACT_RUN_FUNC)(struct tyACT* pact, UI32 para);
typedef void (*ACT_OP_FUNC)(void* pp, struct tyACT* pact);
typedef struct tyPART* PPART;

//动作结构体
typedef struct tyACT
{
	UID				id;					//动作ID
	UID       		gid;				//动作组ID，一般是主动作id
	UI8				type;				//动作类型，位定义, ACT_NTYPE ACT_NORM ACT_CTRL

	/*ACT run status*/
	UI16			mov_st;			    //动作运动状态，见ACT_MOV_STATE定义
	UI16			ftaskstat;			//动作状态标记，主要是INTF_CEC(连续动作)、INTF_ERR(出错标记)
	UI8				mov_m;				//当前设定动作段数
	UI8				last_hydr_port;		//最近打开的油路端口，按位计算(0-7)位表示8个油路
	UI16			last_hydr_pri;		//最近使用油路优先级，每2位表示一个油路优先级

	UI16			last_da_port;		//上一次打开的模拟量端口
	UI16			last_da_pri;		//上一次打开的模拟量的优先级
	
	UI16			step;					/*动作步*/
	UI32			start_ms;				/*动作开始时间, 由动作函数修改*/
	UI32			step_100us;				/*动作步开始时间，由动作函数修改*/
	UI16			step_100ms;				/*动作步开始时间，100ms 单位, 由动作函数修改*/
	UI16			next_step;				/*下一步*/
	UI16			next_step_sav;			//在执行虚拟步时临时保存next_step
	
	PPART 			part;					//动作所属的部件指针
	//PACT_FORK		pfork;
	PACT_TSK		ptsk;					//动作执行时关联的任务控制指针，动作不执行时被置为NULL

	ACT_RUN_FUNC	act_func;				//动作执行函数指针
	ACT_OP_FUNC		stop_func;				//动作停止函数指针，在动作手动放开时执行
	ACT_OP_FUNC		stop_func_err;				//动作停止函数指针，在动作警报强制停止
	ACT_OP_FUNC		stop_func_hold;				//动作停止函数指针，在动作挂起时停止
	ACT_OP_FUNC		react_func;				/*动作激活函数指针，动作挂起后重新激活运行时调用，用于重新执行动作挂起前阀和液压的操作.
											在动作函数中调用SET_MOV CHK_SET_MOV CHK_SET_MOV_M时被赋值*/
	ACT_OP_FUNC		react_func2;
	BOOL			cannotbeclear;				//不能被清除
		
	UI32			act_step_num;

	UI8				spc_data;				//特殊标记	0:前部动作	1:后部动作	注射为0	保压为1
	UI16			reactive_step;		//被中断动作恢复固定步骤，0表示恢复上一次结束步
}ACT, *PACT;

//动作停止列表
typedef struct tyACT_STOP_FUNC
{
	void*			stop_func;				//手动放开动作停止函数指针
	void*			stop_func_err;			//警报动作停止函数指针
	void*			stop_func_hold;			//潜入动作动作停止函数指针
}ACT_STOP_FUNC,*PACT_STOP_FUNC;

//动作类型信息
typedef struct tyACT_INIT_INFO
{
	UI8				act_sn;				//动作序号，从1开始
	UI8				g_act_sn;			//主动作的动作序号
	UI8				act_type;			//动作类型，主要区别是否为调度动作
	ACT_RUN_FUNC	act_func;			//动作函数执行
	void*			stop_func;			//动作停止函数指针
	PACT_STOP_FUNC	stop_func_list;			//动作停止函数指针列表
	BOOL			cannotbeclear;			//动作不能被清除
}ACT_INIT_INFO, *PACT_INIT_INFO;


#define ACT_STEP_NEW(step)			ActStepNew(pact, step)
#define ACT_START()					(pact->start_ms = GetMsTick(),pact->act_step_num = 0)
#define DIFF_SEC(x)				    ((UI16)(GetSecTick() - (UI16)(x)))

#define DIFF_100MS(x)				((UI16)(Get100MsTick() - (UI16)(x)))
#define DIFF_10MS(x)				((UI16)(Get10MsTick() - (UI16)(x)))
#define DIFF_MS(x)				    ((UI32)(GetMsTick() - (UI32)(x)))
#define ACT_STEP_100MS				((UI16)(Get100MsTick() - pact->step_100ms))
#define ACT_STEP_10MS				((UI32)(Get100UsTick() - pact->step_100us)/100)
#define ACT_STEP_MS					((UI32)(Get100UsTick() - pact->step_100us)/10)
#define ACT_STEP_100US				((UI32)(Get100UsTick() - pact->step_100us))

#define STEP_100MS_CHK(chk_100ms)	(ActMov100UsChk(pact->ptsk, pact->step_100us, ((UI32)chk_100ms)*1000))
#define STEP_10MS_CHK(chk_10ms)		(ActMov100UsChk(pact->ptsk, pact->step_100us, ((UI32)chk_10ms)*100))
#define STEP_MS_CHK(chk_ms)			(ActMov100UsChk(pact->ptsk, pact->step_100us, ((UI32)chk_ms)*10))

#define STEP_MS_CHK_INJ(chk_ms)			(ActMov100UsChk_INJ(pact->ptsk, pact->step_100us, (UI32)chk_ms))	 //射出时间精度用   DK 2014.1105

#define ACT_RUN_MS					((UI32)(GetMsTick() - pact->start_ms))
#define GET_ACT(pp, act_sn)			((pp)->act + (act_sn) -1)
#define SET_REACT_FUNC(pact, func)	((pact)->react_func = (ACT_OP_FUNC)(func))
#define SET_MOV(func)				(ActSetMov((ACT_OP_FUNC)(func), (PPART)pp, pact))
#define SET_MOV2(func)				(ActSetMov2((ACT_OP_FUNC)(func), (PPART)pp, pact))

#define CHK_SET_MOV(func)			(ActChkAndSetMovM((ACT_OP_FUNC)(func), (PPART)pp, pact, 0))
#define CHK_SET_MOV_M(func,m)		(ActChkAndSetMovM((ACT_OP_FUNC)(func), (PPART)pp, pact, m))
#define SET_OFF(func)				(func(pp, pact), SET_REACT_FUNC(pact, NULL))
#define INIT_ACT(pp, info)			(InitAct((PPART)(pp), (pp)->act, (info), sizeof(info)/sizeof(info[0])))

#ifdef __cplusplus
}
#endif

#endif

