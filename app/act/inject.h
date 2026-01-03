#ifndef _INJECT_H_
#define _INJECT_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include	"inject_dic.h"
#include	"ioproc.h"
#include	"admeter.h"

//#include "clamp.h"
//#include "nozzle.h"
//#include "robot.h"
//#include "chargpress.h"
	
#define SUCK_SEG		1	//射退段数
#define INJ_SPEED_CHK_INTVAL	20		/*射出速度检查时间间隔 10ms*/
#define INJ_PROV_DA_CH	0x01	//注射比例阀使用AO1

#define SUCK_SPC_PORT   0x01 //免烘干机射退通道:1
#define SUCK_SPC_DISPLAY_CH   0x03 //免烘干机射退显示通道:3

#define INJ_PROV_FWD	0x02
#define INJ_PROV_BWD	0x01

#define ELECCHG_DA_PORT 0X02    //电储料通道:2
#define ELECCHG_DISPLAY_CH  0x02   //电储料显示通道:2

typedef enum enSUCK_RAMP_SEG
{
	//托进斜率
	SUCK_RAMP_START = 0,
	SUCK_RAMP_END,		
}SUCK_RAMP_SEG;


typedef struct tyCLAMP* PCLAMP;
typedef struct tyNOZZLE* PNOZZLE;
typedef struct tyCHGPRS* PCHGPRS;
typedef struct tyINJECT
{
	PART				part;
	
	PINJECT_PO			po;
	PINJECT_PI			pi;
	PINJECT_MACHSET		mh;
	PINJECT_MOLDSET		md;
	PINJECT_ACTPARA		pa;
	PINJECT_HYDR		hy;
	PINJECT_STATE		st;

	ACT					act[INJECT_ACT_SN_END - 1];

	PCLAMP 				pclmp;
	PNOZZLE				pnzl;
	PCHGPRS				pchgprs;


	UI16				injectmode;		//0:标准注射走位置	1:各段注射走时间
	UI16				twice_inj_pos;
	UI16				autopurgecnt;
	UI32				injectstart_100us;
	UI32				injectchk_100us;
	UI16				holdchk_press;
	UI16				chg_pos;
	int					InjHoldPosEnd;	//	记录残料位置的开始值
	int					SuckBackPosBefChg;	//	储前射退位置 = 开始射退位置 + 储前射退距离
	UI16				inj_real_ch;
	
	PICNT_ST			chg_cnt;
	UI32                qc_avg_injtm;
	UI32                qc_avg_injspd;
	UI32                qc_avg_injend;
	UI32                qc_avg_injpres;
	UI32                qc_avg_holdend;
	UI32                qc_avg_chgtm;	
	UI32                qc_avg_chgend;
	UI32                qc_avg_sucktm;
	UI32                qc_avg_suckend;		
	UI16				AdInjectPosn;  // 射出电子尺位置，只在座进前射出使用
	int					InjPosstart;	//	记录射出起始值

	UI16				spdramp_suck[SUCK_RAMP_END + 1];
	UI16				pressramp_suck[SUCK_RAMP_END + 1];

	UI16				Suck_Flow_Buf[SUCK_SEG];  //存放射退速度数据
	UI16				Suck_Pos_Buf[SUCK_SEG];   //存放射退位置数据
	UI16				Suck_Endpos;	//射退动作的终止位置
	UI16				Suck_Num;		//射退终止位置段数

	UI16				relvlvopnflag;
	UI16				Tm_relvlvopn;

	UI32				INJ_TM_N[10];	//射出一段到10段计时的时间存储地址	常保持3位小数点	20200220
	UI32				HOLD_TM_N[10];	//保压一段到10段计时的时间存储地址	常保持3位小数点	20200220

	UI16				needle_valve_allowinj;	//胶口针阀允许注射
	
	UI16				Chg_nomove_Tm;	//储料无法移动检测功能的起始时间标记
	UI16				Chg_nomove_Pos;	//储料无法移动检测功能的起始位置
}INJECT, *PINJECT;

#define INJECT_INIT_INFO		{{PO_ID, CAL_LEN(INJECT_PO)}, {PI_ID, CAL_LEN(INJECT_PI)}, {MH_ID, CAL_LEN(INJECT_MACHSET)}, {MD_ID, CAL_LEN(INJECT_MOLDSET)}, \
							 {PA_ID, CAL_LEN(INJECT_ACTPARA)}, {HY_ID, CAL_LEN(INJECT_HYDR)}, {ST_ID, CAL_LEN(INJECT_STATE)}}

void OpenRelVlvProc(void);
ACT_RET ChargeProc(PACT pact, UI32 para);
ACT_RET ChargePre(PINJECT pp);
void ChargeOff(PINJECT pp, PACT pact);
ACT_RET SuckBack(PACT pact, UI32 para);
void SuckBwdOff(PINJECT pp, PACT pact);
ACT_RET ChargeAdj(PACT pact, UI32 para);                              //调模状态下手动储料
ACT_RET AutoPurgeCharge(PACT pact, UI32 para);              //自动清料储料

BOOL InjectWarmErrChk(PINJECT pp);       //注射储料时 电热检查
ACT_RET ChargeBeforeInj(PACT pact, UI32 para); 
ACT_RET SuckBwdAdj(PACT pact, UI32 para);
void SuckBwdOffDelay(PINJECT pp, PACT pact);
ACT_RET SuckBwdBfChg(PACT pact, UI32 para);
ACT_RET AutoPurge(PACT pact, UI32 para);              //自动清料

/*funtion interface for extern module*/
BOOL InjectTempErrChk(PINJECT pp);
PNOZZLE InjectGetNozzle(PINJECT pp);
UI16 GetInjectHoldPress(PINJECT pp);
BOOL InjectAccIsUsed(PINJECT pp);
PI_STATUS ChkChargAndSuckEndPI(PINJECT pp);
BOOL InitInjectPart(PINJECT pp, UI8 sn);
UI32 InjectCmd(PINJECT pp, UI32 cmd, UI32 para);
BOOL AutoChargeEnableChk(PINJECT pp);
ACT_RET GetChargeRpmadj(PACT pact, UI32 para);
void  ClearChgOut(PINJECT pp, PACT pact);
void SetChargeBwdAdj(PINJECT pp, PACT pact);
ACT_RET InjectPurgeGuard(PINJECT pp, PACT pact);
UI16 InjTempChkPro(PINJECT pp);
UI16 InjTempChkSimple(PINJECT pp);
ACT_RET InjectPreChk_CBAC(void);
void EndDaChgActTsk(void);
ACT_RET SuckbackSync(PACT pact, UI32 para);
void SuckbackSyncOff(PINJECT pp, PACT pact);



#ifdef __cplusplus
}
#endif

#endif

