#ifndef _MACHAPI_H_
#define _MACHAPI_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"
#include "part.h"
#include "configdf.h"
#include "machine_dic.h"
#include "machine.h"


#define 	ChkSysCfg(code)			(CheckSysConfig((code) >> 4, (1 <<((code)&0x0f))))
#define 	SetSysCfg(code)			(SetSysConfig((code) >> 4, (1 <<((code)&0x0f))))
#define 	ClrSysCfg(code)			(ClrSysConfig((code) >> 4, (1 <<((code)&0x0f))))


#define     OM_MANUAL     0           //     ; '手动    '     MANUAL = 0                
#define     OM_AUTO       0x0001      // 0   ; '自动    '     'AUTO        '
#define     OM_SEMI       0x0002      // 1   ; '半自动  '     'SEMI AUTO   '
#define     OM_SENR       0x0004      // 2   ; '电眼自动'     'SENSOR-AUTO '
#define     OM_TIME       0x0008      // 3   ; '时间自动'     'TIME AUTO   '
#define     OM_ADJU       0x0010      // 4   ; '粗调模  '     'MOLD HT ADJ '
#define     OM_QCHK       0x0020      // 6   ; '机器检测'  'QC CHECK'
#define     OM_PURG       0x0040      // 7   ; '自动清料'     'AUTO PURGE  '
#define     OM_AA_FLG     0x0080      // 11  ; '自动调模'     'AUTO ADJMOLD'
#define     OM_AA_1ST     0x0100      // 12  ; COMPLETED 1ST STEP
#define     OM_AA_DIR     0x0200      // 13  ; ADJUST DIRECTION FWD=1, BWD=0
#define     OM_AA_FST     0x0400      // 14  ; FAST AUTO ADJUST BY COUNT
#define     OM_AA_LEARN		0x0800      // 15  ; '齿距学习'	'DISTANCE LEARN'

#define		OM_CS_RF	0x1000		//16	斜率校准
#define		OM_PR_RF	0x2000		//17	压力校准


#define     MONILEN           0x18
#define     MONIOFS           0x600
#define     MONILAST          0x18
#define     MONIMAX           0x30
#define     MONIMIN           0x48

enum EN_MACHINE_POWER
{
   	P_CYCLE = 0,
   	P_OTHERS,
   	P_CLAMPCLS,         
   	P_CLAMPOPN,            
   	P_INJECT,             
	P_CHARGE,             
	P_SUCKBACK,              
	P_EJECT, 
	P_END            
};

enum EN_MACHINE_PI
{
	I_IE_PO1 = 0,                 //特殊输入1
	I_IE_PO2,
	I_IE_PO3,
	I_IE_PO4,
	I_IE_PO5,
	I_IE_PO6,
	I_IE_PO7,
	I_IE_PO8,
	I_IE_PO9,
	I_IE_PO10,
	I_IE_PO11,
	I_IE_PO12,
	I_IE_PO13,
	I_IE_PO14,
	I_IE_PO15,
	I_IE_PO16,                //特殊输入16
};

enum EN_MACHINE_PO
{
   	O_HYD_VLV2 = 0,                //0D5H ;二油路阀     ; HYDR VLV 2 07-7-6
   	O_HYD_COMB,                //0D6H ;合流阀       ; HYDR COMBINE VLV 07-7-6
   	O_SOEC_SLM,                 //37H  ;连动阀       ;;SAME TIME OPEN/EJECT/CORE  ;;(9B20)
	O_FST_SOL,                 //0CH  ;快速阀       ;;FAST VALVE (CLS/OPN FAST;INJE/CHRG) ;;(9630)
	O_CP_PUMP,                 //48H  ;特殊中子     ; SPECIAL CORE PULLER PUMP
	O_OPN_EJE_COR,             //0E4H ;开模顶出中子阀      ; 伺服多泵，复合动作使用
	O_OE_PO1,                 //特殊输出1
	O_OE_PO2,
	O_OE_PO3,
	O_OE_PO4,
	O_OE_PO5,
	O_OE_PO6,
	O_OE_PO7,
	O_OE_PO8,
	O_OE_PO9,
	O_OE_PO10,
	O_OE_PO11,
	O_OE_PO12,
	O_OE_PO13,
	O_OE_PO14,
	O_E_PO15,
	O_E_PO16,                //特殊输出16
};


#pragma diag_suppress 1296
enum EN_MACHINE_MACHSET
{
	C_FL_HYDRAUIC  = GET_IND(PMACHINE_MACHSET, FL_HYDRAUIC),// 408  液压控制选择码    ;90H
	C_FL_TEMPER,                       // 409  温度控制选择码    ;92H 
	C_FL_METER,                         // 40a  位置尺控制选择码  ;94H 
};

typedef enum MACHINE_CMD
{
	MH_CMD_ROBOT_EM = PART_CMD_END + 1,							/*机械手急停*/
	MH_CMD_SFDR_CH,												/*安全门检查*/
	MH_CMD_LOOP_CHK,											/*一般循环检查 ，模具急升 氮气增压循环 欧规机械后检查*/
	MH_CMD_AUTO_CYCLE											/*机械手制动开始*/
}MACHINE_CMD;

typedef enum MACHINE_TYPE
{
	MACH_HZ = 0,					/*标准卧式机*/
	MACH_VT							/*标准立式机*/
}MACHINE_TYPE;

PPART GetLub(int* pnum);
UI16 GetOperMode(void);
UI16 ChkOperMode(UI16 mask);
UI16 SetOperMode(UI16 mask);
BOOL CheckSysConfig(UI16  ind, UI16 mask);
void SetSysConfig(UI16  ind, UI16 mask);
void ClrSysConfig(UI16  ind, UI16 mask);


PMACHINE_MACHSET GetSysConfig(void);
UI16 GetSysCfg(UI16 ind);
BOOL ChkInjectCushErr(void);
BOOL ChkCoreIsRun(void);
void MachineInit(void);
UI16 GetSysPO(UI16 ind);
UI16 GetSysPI(UI16 ind);
PMACHINE GetSys(void);
WORD GetSysPresLimit(void);


BOOL CycleTskIsRun(void);
UI32 GetCycleTskStartMs(void);
PMACHINE_MOLDSET GetSysMold(void);
void SetMachineErr(UI32 err);
void ClrMachineErr(UI32 err);
BOOL ChkMachineErr(UI32 err);

UI16 GetMoldAlarmMode(void);
void SetMoldAlarmMode(UI16 mode);
UI8 AutoAlarmIsStart(void);
void SetAutoAlarmStart(UI8 mode);

UI16 GetCustomID(void);
BOOL InitMachinePart(void);
UI32 MachineCmd(UI32 cmd, UI32 para);
PPART GetMachineSubPart(UI8 type, UI8 sn);
PPART GetMachineSubPartList(UI8 type, UI8* pnum);
void SetOperTM(void);
void GetTotalPower(void);
void SendOperMode(void);
int GetMachineSubPartNum(UI8 type);
void MachineSysCfgModify(void);
UI16 ClrOperMode(UI16 mask);
int GetJointActSel(void);
int GetJointActSel2(void);

UI16 GetMachineType(void);
BOOL ChkMachType(MACHINE_TYPE type);
void MachineCfgTx(void);

void ShowExtOnline(void);
void ClearMacineActTime(void);
void GetQcPara(void);

UI16 GetModeChgSpc(void);
void  SpecialFunc(void);
void AutoAlarmReStart(void);
UI8 AutoAlarmCanUpdate(void);
BOOL ActIODelayLimit(WORD name, WORD err);

PINNER_PARA LoadInnerPara(void);
void SaveInnerPara(void);
PINNER_PARA GetInnerPara(void);
void InitMachineVersion(void);
void PollStepFunc(void);


#define CUS_ID	GetCustomID()

#ifdef __cplusplus
}
#endif

#endif


