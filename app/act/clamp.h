#ifndef _CLAMP_H_
#define _CLAMP_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include	"acttype.h"
#include 	"ioproc.h"
#include	"clamp_dic.h"
#include    "adjust.h"
#include 	"admeter.h"
//#include 	"EleCtrlApp.h"


//#include "airblow.h"
//#include "robot.h"
//#include "safedoor.h"
//#include "eject.h"

#define CLAMP_MAX_AIRBLOW		15
#define CLAMP_MAX_EJECT			1
#define CLAMP_MAX_CORE			6  //对应面板可设置的中子数
#define CLAMP_MAX_LUB			4

#define CLAMP_DEFAULT_TM		6000
#define CLAMP_VLV_USE			1			//比例阀使用的模拟量通道  20200102


#define CLAMP_SEG 10
#define CLAMP_POSEND_SEG	10

#define   TM_DEFAULTMIN			10
#define	TM_DEFAULTMAX		999


#define CLAMP_CLS_PID_DIREC	1
#define CLAMP_OPN_PID_DIREC 2

#define CLAMP_CLS_P_DIREC 1
#define CLAMP_OPN_P_DIREC 1

#define CLAMP_CLS_PID_CH	2
#define CLAMP_OPN_PID_CH	2
#define CLAMP_CLS_PID_PORT	1 << (CLAMP_CLS_PID_CH - 1)
#define CLAMP_OPN_PID_PORT	1 << (CLAMP_OPN_PID_CH - 1)


#define OILCLAMPPOS_BUFMAX	10

#define CLMPOPNMID_MINTM	25	
#define CLMPOPNEND_MINTM	50
typedef struct tyOilTempAi
{
	UI32	clampopnpos_filt;	//滤波后当前开模位置数据
	UI32	clampopnpos_filt_last;	//滤波后当前开模位置数据
	UI16	clampopnpos_20;		//全自动前20模开模位置
	UI16	clampmaxdiff;		//最大允许偏差值	0.1mm
	UI16	lastdiff_flag;		//上一模开始偏差标记	
	int		setpos;				//计算得到的设定位置
	int		lastsetpos;
	UI16	first_flag;			//首模标记
	UI16	clampopn_cn;		//开模次数
	UI16	clampopn_Complete_flag;	//完整开模标记
	UI16	clampopn_use_lowflow;	//开模使用底流
	UI16	cycle_tm;
	UI16	kp;
	UI16	ki;
	UI16	ti;
	int 	ik;
	int		ek;
	UI16	clamppos_buf[OILCLAMPPOS_BUFMAX];	//开模终点记录

	UI16	lubstart_flag;		//润滑动作开始
	int		lubdiffpos[10];		//润滑动作影响位移偏差
	UI16	lubclampopn_cn;		//润滑开始计数
}OilTempAi, *POilTempAi;



typedef struct tyAIRBLOW* PAIRBLOW;
typedef struct tyROBOT* PROBOT;
typedef struct tySAFEDOOR* PSAFEDOOR;
typedef struct tyEJECT* PEJECT;
typedef struct tyROTATE* PROTATE;
typedef struct tyCORE* PCORE;
typedef struct tyAUTOCTRL* PAUTOCTRL;
typedef struct tyLUB* PLUB;
typedef struct tyINJECT* PINJECT;
typedef struct tyNOZZLE* PNOZZLE;

typedef struct tyCLAMP
{
	PART			part;

	PCLAMP_PO		po;
	PCLAMP_PI		pi;
	PCLAMP_MACHSET	mh;
	PCLAMP_MOLDSET	md;
	PCLAMP_ACTPARA	pa;
	PCLAMP_HYDR		hy;
	PCLAMP_STATE	st;

	ACT				act[CLAMP_ACT_SN_END -1];

	/*part reference*/
	PAIRBLOW	airblow[CLAMP_MAX_AIRBLOW];
	PROBOT		prbt;
	PSAFEDOOR	psfdr;
	PEJECT		pejt[CLAMP_MAX_EJECT];
	PCORE		pcore[CLAMP_MAX_CORE];
	PLUB		plub[CLAMP_MAX_LUB];
	PAUTOCTRL	pactrl;
	PADJUST     padj;
	PINJECT		pinj;
	PNOZZLE		pnzl;

	OilTempAi 	g_oiltempai;
	
	UI32		mov_start_ms;
	
	/*关模使用*/
	UI32		clslowpres_start_ms;
	UI32		close_start_ms;
	UI32		hipress_start_ms;
	
	UI16		LubCoolCnt;
	UI32		last_chk_opennum;
	UI16		ClampBwdErr_10ms;
	UI16        forceclamp;//左右合模按键进入合模
	UI16        qc_pos_opnstart;
	UI32        qc_avg_opnend;
	UI32        qc_avg_opntm;
	UI32        qc_avg_opnspd;
	UI32        qc_avg_opnclstm;	
	UI32        qc_avg_opnclslow;			
	UI32        qc_avg_opnclshi;			
	UI16		delaycheck0;
	UI16		delaycheck1;	
	UI16		safevlv_step;	//安全阀步骤
	
	UI16        clsopnlaststep;         //开模上次步骤
	UI16        clscmpflag;             //关模完成标记
	UI16        clscmphitm;             //锁模高压锁死时间
	UI16        clampmidcorebwd;        //16位表示开模途中中子退标记比如低16是0x0008表示开模中子D退标记位途中中子一次类推
	UI16        clampmidcorefwd;        //16位表示合模途中中子进标记比如低16是0x0001表示开模中子A进标记位途中中子一次类推
	UI16        corefwd[8];             //途中中子进排序
	UI16        corebwd[8];             //途中中子退排序
	UI16        core_sn1[CLAMP_POSEND_SEG];             //途中中子进排序
	UI16        core_sn2[CLAMP_POSEND_SEG];             //途中中子进排序
	UI16		core_sn_last;			//上一次中子运行编号
	
	UI16         coreinturn[CLAMP_MAX_CORE];//中子进动作顺序12345678
    UI16         coreoutturn[CLAMP_MAX_CORE];//中子退动作顺序87654321
    
	UI16        clpopnmidcorenum;       //开模途中中子退分段条数
	UI16        clpclsmidcorenum;       //合模途中中子进分段条数
	UI8        clampejectbwdflag;      //合模脱退标记
	
	UI32        tm_CloslowTime;        //开模慢速阀时间标记

	UI8         clampclserr_opn;        //异常开模标记
	UI16        clscmpflagmid;         //关模完成标记   用于途中机械手做标记使用，当有代码code 0:0x01时

	/*开关模运动数据缓存*/
	UI16	flow_clamp[CLAMP_SEG];	//流量数据缓存	1%
	UI16	posi_clamp[CLAMP_SEG];	//位置数据缓存	0.1mm
	UI16	endpos_clamp[CLAMP_POSEND_SEG];	//终止位置数据缓存 0.1mm
	UI16	endpos_num;
	UI16	endpos_num_cls;	//关模终止位置的段数

	/*开关模油泵到油缸的效能自学习*/
	UI32	spd_oil;	//油缸速度
	UI32	spd_motor;	//电机速度
	UI16	spd_num;	//速度计数次数
	
	UI16	rf_lastpos;	//上一次电子尺位置
	UI16	rf_posstop_starttm;	//运动停止起始时间	100ms
	UI16	rf_modelen_max;	//模座油缸最大行程	0.1mm
	UI16	rf_modelen_20;
	UI16	rf_modelen_50;
	UI16	rf_modelen_80;
	UI16	rf_num;

	UI16	rf_K[4];
	UI16	rf_K_num[4];

	UI16	spdramp_cls[6];
	UI16	spdramp_opn[6];
	UI16	pressramp_cls[6];
	UI16	pressramp_opn[6];

	UI16	spdramp_cls_manual[2];
	UI16	spdramp_opn_manual[2];
	int		cls_Poss_start;		//关模启动位置
	
	/****************** 方向比例阀开关模时新增变量*********************/
	UI16        clampfunc;     		    //使用特殊开模功能
	UI16        clampfunc_sv;     		    //使用特殊开模功能
	UI16        CoreNum;		//开模途中中子退个数
	UI16        CoreNumCls;		//关模途中中子进个数
	UI16        pflowend;  //开模 PID运算结束时，最后的流量值
	UI16        pflowendda; //开模 PID运算结束时,最后的流量da值
	UI16        pclsflowendda; // 关摸PID运算结束时,最后的流量da值
	UI16		clsslowendpos;
	UI16		clampend;				//快速开模结束为止临时变量
	

	UI16		AiClampRamp;		//AI开模ramp
	UI16		clampopnnum;		//开模段数缓存

	UI16        I_CLP_CHK_LAST_TIME;    //模板检知持续时间
	UI16		clampopntmchk_enable;	//开模时间保护使能信号
	UI16		allow_robot_mid;
	UI16		allow_robot;
	UI16		clampopnmidtmchk;
	UI16		clampopntmchk;		//开模到90%检测
	UI16		clampopnmidtm;		//开模到途中机械手位置所用时间
	UI16		clampopntm;			//开模到90%位置时所用时间

    int		    clampcls_start_pos;		//关模开始位置(用于平顺算法判断是否走手动启动斜率)

    UI16		Open_port;			//开模液压通道
    UI16		Cls_port;			//关模液压通道
}CLAMP, *PCLAMP;

#define CLAMP_INIT_INFO		{{PO_ID, CAL_LEN(CLAMP_PO)}, {PI_ID, CAL_LEN(CLAMP_PI)}, {MH_ID, CAL_LEN(CLAMP_MACHSET)}, {MD_ID, CAL_LEN(CLAMP_MOLDSET)},  \
							 {PA_ID, CAL_LEN(CLAMP_ACTPARA)}, {HY_ID, CAL_LEN(CLAMP_HYDR)}, {ST_ID, CAL_LEN(CLAMP_STATE)}}

typedef enum enCLAMP_RAMP_SEG
{
	//关模斜率
	CLP_RAMP_CLS_START = 0,
	CLP_RAMP_CLS_FAST,
	CLP_RAMP_CLS_LOW,
	CLP_RAMP_CLS_HIGH1,
	CLP_RAMP_CLS_HIGH2,
	CLP_RAMP_CLS_END,
	CLP_RAMP_CLS_MID = 1,	//使用新油路算法时，只有三个开始、中间、结束，中间则直接采用FAST

	//开模斜率
	CLP_RAMP_OPN_START = 0,
	CLP_RAMP_OPN_FAST1,
	CLP_RAMP_OPN_FAST2,
	CLP_RAMP_OPN_FAST3,
	CLP_RAMP_OPN_LOW,
	CLP_RAMP_OPN_END,
	CLP_RAMP_OPN_MID = 1,	//使用新油路算法时，只有三个开始、中间、结束，中间则直接采用FAST	

	//手动启动斜率
	CLP_RAMP_CLS_MANUALSTART = 0,
	CLP_RAMP_CLS_MANUALEND,

	CLP_RAMP_OPN_MANUALSTART = 0,
	CLP_RAMP_OPN_MANUALEND,

	
}CLAMP_RAMP_SEG;


/*Self module funtion*/
BOOL ChkLocateDa(PCLAMP pp);
ACT_RET ClampClose(PACT pact, UI32 para);
ACT_RET ClampClsAdj(PACT pact, UI32 para);
ACT_RET ClampPress(PACT pact, UI32 para);
ACT_RET ClampOpen(PACT pact, UI32 para);
ACT_RET ClampOpnAdj(PACT pact, UI32 para);
void ClampOpenOff(PCLAMP pp, PACT pact);
void ClampOpenOffME(PCLAMP pp, PACT pact);

ACT_RET AutoAdjustPresMold(PACT pact, UI32 para);
ACT_RET ClampPressExhaust(PACT pact, UI32);
ACT_RET ClampOpnExhaust(PACT pact, UI32 para);
ACT_RET AutoAdjClampOpn(PACT pact, UI32 para);
ACT_RET ClampPressERR(PACT pact, UI32 para);
ACT_RET ClampOpnERR(PACT pact, UI32 para);
ACT_RET ClampOpenly(PACT pact, UI32 para);
ACT_RET ClampOpen_pid(PACT pact, UI32 para);
ACT_RET ClampOpen_p(PACT pact, UI32 para);
ACT_RET Clampclose_pid(PACT pact, UI32 para);



/*funtion interface for extern module*/
void ClampChargeOff(PCLAMP pp);
void ClampClsVlvSet(PCLAMP pp, BOOL bset);
BOOL ClampCoreStateErrChk(PCLAMP pp, BOOL bfwd);
void ClampCoreFwdPOSet(PCLAMP pp, BOOL bset);
void ClampCoreFwdOff(PCLAMP pp);
BOOL ClampTempErrChk(PCLAMP pp);
void ClampOvlSolOff(PCLAMP pp);
BOOL ClampOpenReachErrChk(PCLAMP pp);
BOOL ClampEjectBwdErrChk(PCLAMP pp);
BOOL ClampClsEjectBwdErrChk(PCLAMP pp);
BOOL ClampClsEndErrChk(PCLAMP pp);
BOOL InitClampPart(PCLAMP pp, UI8 sn);
UI32 ClampCmd(PCLAMP pp, UI32 cmd, UI32 para);
PI_STATUS ClampMecSafeChk(PCLAMP pp);
BOOL ShotReached(PCLAMP pp);
BOOL ClampSensorErrChk(PCLAMP pp);
void ClampEjectFwdStart(PCLAMP pp);
BOOL ClampEjectActEnable(PCLAMP pp);
BOOL ClampEjectBwdIsErr(PCLAMP pp);
BOOL ClampEjectFwdIsErr(PCLAMP pp);
void ClampOpenCount(PCLAMP pp);
void MachineSafeCheckPro(void);
void ClampClsOff_2(PCLAMP pp, PACT pact);
void ClampClsOff_3(PCLAMP pp, PACT pact);

ACT_RET ClampClsPreChk_CBAC(void);
BOOL  ClampsafeLight(PCLAMP pp);//安全光幕检查


#ifdef __cplusplus
}
#endif

#endif
