#ifndef _MACHINE_H_
#define _MACHINE_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "acttype.h"
#include "machine_dic.h"
#include "clamp.h"
#include "inject.h"
#include "core.h"
#include "eject.h"
#include "nozzle.h"
#include "safedoor.h"
#include "robot.h"
#include "sysacc.h"
#include "lub.h"
#include "airblow.h"
#include "injstor.h"
#include "robot.h"
#include "chargpress.h"
#include "autoctrl.h"
#include "lock.h"
#include "adjust.h"
#include "shut.h"
#include "lock.h"
#include "motor.h"
#include "servoecat.h"
#include "pump.h"
#include "TempCtrlComm.h"
#include "TempCtrlMw.h"
#include "AiClampopn.h"
#include "tempctrl.h"

//#define DM9000_ENABLE	//第四版卓越关闭网络部分相关代码
#define ECAT_ENABLE

//OPER_MODE    
#define DEF_CLAMP_NUM		1
#define DEF_INJECT_NUM		1
#define DEF_NOZZLE_NUM		DEF_INJECT_NUM
#define DEF_SYSACC_NUM		DEF_CLAMP_NUM
#define DEF_INJSTOR_NUM		DEF_INJECT_NUM
#define DEF_SAFEDOOR_NUM	DEF_CLAMP_NUM
#define DEF_LUB_NUM			3
#define DEF_ROTATE_NUM		2
#define DEF_EJECT_NUM		1
#define DEF_AIRBLOW_NUM	 	6
#define DEF_CORE_NUM		6
#define DEF_ROBOT_NUM		DEF_CLAMP_NUM
#define DEF_CHGPRS_NUM		DEF_INJECT_NUM
#define DEF_AUTOCTRL_NUM 	DEF_CLAMP_NUM
#define DEF_LOCK_NUM		2
#define VLV_DELAY_TM		5
#define DEF_ECAT_NUM	1
#define MAX_SUB_TBL_NUM 30

typedef struct tySUB_PART_INFO
{
	UI8		part_type;
	UI8		num;
	UI16	part_size;
	PPART	pp;
}SUB_PART_INFO, *PSUB_PART_INFO;

#define SUB_PART_INFO_EXT_LEN  	OFFSETOF(PSUB_PART_INFO, part_size) 

typedef struct tyMACHINE
{
	PART				part;

	PMACHINE_PO			po;
	PMACHINE_PI			pi;
	PMACHINE_MACHSET	mh;
	PMACHINE_MOLDSET	md;
	PMACHINE_VERSION	vr;
    PMACHINE_HYDR		hy;
	PMACHINE_STATE		st;

	ACT			act[MACHINE_ACT_SN_END -1];

	UI8			sub_tbl_num;
	PSUB_PART_INFO	sub_tbl;

	/*经常使用的部件索引*/
	UI8			clamp_num;
	UI8			safedoor_num;
	UI8			eject_num;
	UI8			rotate_num;
	UI8			robot_num;
	UI8			injstor_num;
	UI8			inject_num;
	UI8			core_num;
	UI8			lub_num;
	UI8			tempcomm_num;
	
	PCLAMP		pclmp;
	PEJECT		pejt;
	PSAFEDOOR	psfdr;
	PROTATE		prtt;
	PROBOT		prbt;
	PINJSTOR	pinjstr;
	PINJECT		pinj;
	PCORE		pcore;
	PLUB		plub;
	PLOCK       plock;
	PSERVOECAT  pservoecat;
	PTEMPCTRLCOMM ptempcomm;

	UI32		Recyclestart;	//全程时间的时间初始化  20200221
	UI32		cyclestart;		//制品时间的时间初始化  20200221
	UI8			bactrunning;
	UI8			AlarmStart;

	UI16        extonlinetime;
	UI16        qcparasend;
	UI32        qc_avg_cyctm;
	UI16        qc_chk_start;
	UI16        PAYMENT_TIMEOUT_FLAG;   //进入分期付款界面标记
	UI8         clampopncore_num;   //联动中子号，用于脱模前等待联动中子  QRC 2017.04.19

	UI16        Filter_PRU_cnt;         //用于记录按下自动后的运行模数,不良品检测时使用
	UI16		can_e700b_testmode;		//E700B进入can通讯测试模式
	UI16		can_e700b_writestep; 	//E700B写步骤
	UI16		can_e700b_writetm;		//E700B写时间标记

	UI16		cur_cus_id;				//当前厂商代码
	UI16		hydr_way_last;

	UI16		motor_enable_mode;
	UI16		last_da_mode;
}MACHINE, *PMACHINE;

#define MACHINE_INIT_INFO		{{PO_ID, CAL_LEN(MACHINE_PO)},{PI_ID, CAL_LEN(MACHINE_PI)},{MH_ID, CAL_LEN(MACHINE_MACHSET)}, {MD_ID, CAL_LEN(MACHINE_MOLDSET)},{VR_ID, CAL_LEN(MACHINE_VERSION)},  \
								 {HY_ID, CAL_LEN(MACHINE_HYDR)}, {ST_ID, CAL_LEN(MACHINE_STATE)}}

#include "machapi.h"

BOOL CycleTimeErrChk(PMACHINE pp);
void ReCycleStart(PMACHINE pp);
void ShowExtOnline(void);
void VersionDiff(void);
void CPU_Need_Reset_Chk(void);


#ifdef __cplusplus   
}
#endif

#endif
