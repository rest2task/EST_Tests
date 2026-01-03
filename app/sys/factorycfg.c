/******************************************************************************
  文 件 名   : factorycfg.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 程序内置的出厂数据
  函数列表   :
              SetPartCfgDefault
              SetTblCfgDefault
  修改历史   :
  1.日    期   : 2013年5月17日
    作    者   : ren chaohong
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

#include "com.h"
#include "cfgdb.h"
#include "part.h"
#include "machine.h"
#include "basectrl.h"
#include "admeter.h"
#include "da.h"
#include "hydr.h"
#include "zerodebug.h"
#include	"hwcfg.h"

#define DEF_TBL_SN 1

const UI16 machine_mh_factory[CAL_LEN(MACHINE_MACHSET)] =  /*FL_MACHINECTRL0 24word*/
{
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0800, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
#if (HW_MODEL == MODEL_I3521)
	0x0100, 0x0000, 0x0000, 0x0000, MODEL_I3201,   0x0001, 0x0001, 0x0000,
#else
    0x0100, 0x0000, 0x0000, 0x0000, MODEL_I3101,   0x0001, 0x0001, 0x0000,
#endif
	/*FL_HYDRAUSEC - FL_MACHINEMODEL*/
	0x0000, 140, 140, 0, 0x0000, 0x0000, 0x0000,
	/*DAPRES_CLPLIMIT - DAPRES_LIMIT*/
	140, 70, 140,
	/*DAFLOW_NOZZLELIMIT - TMCYCLEDEFAULTMAX*/
	99, 99, 99, 99, 70, 140, 99,
	0,0,0,0,0,0,0,0,
	0   
};

const UI16 admeter_mh_factory[CAL_LEN(ADMETER_MACHSET)] = 
{
	250 * 10, 450 * 10, 150 * 10, 250 * 10, 250 * 10, 250 * 10, 250 * 10, 250 * 10,
	30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000,
	0, 0, 0, 0, 0, 0, 0, 0
};

UI16 da_mh_factory[CAL_LEN(DA_MACHSET)] = 
{
	3702, 3702, 3702, 3702, 3702, 3702, 3702, 3702, 3702, 3702, 3702, 3702, 3702, 3702, 3702, 3702, 			//最大值
    2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048
};

UI16 temp_zd_factory[CAL_LEN(TEMP_ZDSET)] = {340, 340, 340, 340, 340, 340, 340, 340, 340, 340, 340, 340,340, 340, 340, 340,			//温度归零值 
											173, 173, 173, 173, 173, 173, 173, 173, 173, 173, 173, 173, 173, 173, 173, 173};


//pump部件默认值	1为第一油路，压力反馈通道默认4通道，2为其他，默认8通道
const UI16 pump_mh_factory1[CAL_LEN(PUMP_MACHSET)] = {500, 100, 1800, 23, 2200, 4, 0, 0, 0,100,100,100,100,100,100,0x0019,0x010E,10,500};
const UI16 pump_mh_factory2[CAL_LEN(PUMP_MACHSET)] = {500, 100, 1800, 23, 2200, 6, 0, 0, 0,100,100,100,100,100,100,0x0019,0x010E,10,500};


//温度pid保存数据出厂值
const UI16 tempcomm_zd_factory[CAL_LEN(TEMPCOMM_ZDSET)] = 
	{//	1		2		3		4		5		6		7		8		9		10		11		12		13		14		15		16
		30,		30,		30,		30,		30,		30,		30,		30,		30,		30,		30,		30,		30,		30,		30,		30,
		80,		80,		80,		80,		80,		80,		80,		80,		80,		80,		80,		80,		80,		80,		80,		80,
		3480,	3480,	3480,	3480,	3480,	3480,	3480,	3480,	3480,	3480,	3480,	3480,	3480,	3480,	3480,	3480,
		870,	870,	870,	870,	870,	870,	870,	870,	870,	870,	870,	870,	870,	870,	870,	870
	};

const PART_CFG_IND factory_part_ind[] =
{
    {(void*)machine_mh_factory, MK_PART(MACHINE_ID, 1), PART_DB_VER, MH_ID, 0, CAL_LEN(MACHINE_MACHSET)},
    {(void*)admeter_mh_factory, MK_PART(ADMETER_ID, 1), PART_DB_VER, MH_ID, 0, CAL_LEN(ADMETER_MACHSET)},
    {(void*)da_mh_factory, MK_PART(DA_ID, 1), PART_DB_VER, MH_ID, 0, CAL_LEN(DA_MACHSET)},
    {(void*)temp_zd_factory, MK_PART(TEMPCTRL_ID, 1), PART_DB_VER, ZD_ID, 0, CAL_LEN(TEMP_ZDSET)},

    //6个pump存储区域初始化
    {(void*)pump_mh_factory1, MK_PART(PUMP_ID, 1), PART_DB_VER, MH_ID, 0, CAL_LEN(PUMP_MACHSET)},
    {(void*)pump_mh_factory2, MK_PART(PUMP_ID, 2), PART_DB_VER, MH_ID, 0, CAL_LEN(PUMP_MACHSET)},
    {(void*)pump_mh_factory2, MK_PART(PUMP_ID, 3), PART_DB_VER, MH_ID, 0, CAL_LEN(PUMP_MACHSET)},
    {(void*)pump_mh_factory2, MK_PART(PUMP_ID, 4), PART_DB_VER, MH_ID, 0, CAL_LEN(PUMP_MACHSET)},
    {(void*)pump_mh_factory2, MK_PART(PUMP_ID, 5), PART_DB_VER, MH_ID, 0, CAL_LEN(PUMP_MACHSET)},
    {(void*)pump_mh_factory2, MK_PART(PUMP_ID, 6), PART_DB_VER, MH_ID, 0, CAL_LEN(PUMP_MACHSET)},

	//2个tempcomm存储区域初始化
    {(void*)tempcomm_zd_factory, MK_PART(TEMPCTRLCOMM_ID, 1), PART_DB_VER, ZD_ID, 0, CAL_LEN(TEMPCOMM_ZDSET)},    
    {(void*)tempcomm_zd_factory, MK_PART(TEMPCTRLCOMM_ID, 2), PART_DB_VER, ZD_ID, 0, CAL_LEN(TEMPCOMM_ZDSET)},
};

/*机器子部件配置表*/
const SUB_PART_INFO factory_sub_tbl[] =
{
    {CLAMP_ID, DEF_CLAMP_NUM}, {INJECT_ID, DEF_INJECT_NUM}, {NOZZLE_ID, DEF_NOZZLE_NUM}, {SYSACC_ID, DEF_SYSACC_NUM},
    {INJSTOR_ID, DEF_INJSTOR_NUM}, {SAFEDOOR_ID, DEF_SAFEDOOR_NUM}, {LUB_ID, DEF_LUB_NUM}, {ROTATE_ID, DEF_ROTATE_NUM},
    {EJECT_ID, DEF_EJECT_NUM}, {AIRBLOW_ID, DEF_AIRBLOW_NUM}, {CORE_ID, DEF_CORE_NUM}, {ROBOT_ID, DEF_ROBOT_NUM},
    {CHGPRS_ID, DEF_CHGPRS_NUM}, {AUTOCTRL_ID, DEF_AUTOCTRL_NUM}, {LOCK_ID, DEF_LOCK_NUM}, {ADJUST_ID, DEF_CLAMP_NUM},
    {SHUT_ID, DEF_INJECT_NUM}, {PUMP_ID, 3}, {TEMPCTRLCOMM_ID, 1},{TEMP_MW, 0}
};


#pragma diag_suppress 1296
/*缺省输入点配置表*/
const PI_LOG pi_log_factory[] =
{
    {1, MK_PART(SAFEDOOR_ID, 1),  GET_IND(PSAFEDOOR_PI, I_SDR_CL2), 0x05},  	//PB01 47H  ;安全门关二
    {4, MK_PART(SAFEDOOR_ID, 1),  GET_IND(PSAFEDOOR_PI, I_RSD_CLS), 0x05},		//PB04 4CH  ;後安全门关终

    {6, MK_PART(CLAMP_ID, 1),  GET_IND(PCLAMP_PI, I_CLS_END), 0x01},				//PB06 03H  ;关模终
    {7, MK_PART(INJECT_ID, 1),  GET_IND(PINJECT_PI, I_PUR_GUA), 0x01},			//PB07 25H  ;射出防护
    {8, MK_PART(CORE_ID, 1),  GET_IND(PCORE_PI, I_SC_CHK), 0x01},					//PB08 33H  ;绞牙一电眼
    {9, MK_PART(ADJUST_ID, 1),  GET_IND(PADJUST_PI, I_ADJ_CHK), 0x01},			//PB09 42H  ;调模电眼
    {10, MK_PART(INJECT_ID, 1),  GET_IND(PINJECT_PI, I_RPM_CHK), 0x00},			//PB10 27H  ;螺杆转数检知
    {11, MK_PART(SAFEDOOR_ID, 1),  GET_IND(PSAFEDOOR_PI, I_SDR_CLS), 0x05},		//PB11 46H  ;完全门关
    {12, MK_PART(CORE_ID, 1),  GET_IND(PCORE_PI, I_CP_FWD), 0x01},				//PB12 30H  ;中子一进终
    {13, MK_PART(CORE_ID, 1),  GET_IND(PCORE_PI, I_CP_BWD), 0x01},				//PB13 31H  ;中子一退终
    {14, MK_PART(EJECT_ID, 1),  GET_IND(PEJECT_PI, I_EJT_FWD), 0x01},				//PB14 2CH  ;脱进终
    {15, MK_PART(EJECT_ID, 1),  GET_IND(PEJECT_PI, I_EJT_BWD), 0x01},				//PB15 2DH  ;脱退终
    {16, MK_PART(NOZZLE_ID, 1),  GET_IND(PNOZZLE_PI, I_NOZ_FWD), 0x01},			//PB16 28H  ;座进终
    {17, MK_PART(NOZZLE_ID, 1),  GET_IND(PNOZZLE_PI, I_NOZ_BWD), 0x01},			//PB17 29H  ;座退终
    {18, MK_PART(ADJUST_ID, 1),  GET_IND(PADJUST_PI, I_ADJ_FWD), 0x01},			//PB18 40H  ;调模进终
    {19, MK_PART(ADJUST_ID, 1),  GET_IND(PADJUST_PI, I_ADJ_BWD), 0x01},			//PB19 41H  ;调模退终
    {20, MK_PART(AUTOCTRL_ID, 1),  GET_IND(PAUTOCTRL_PI, I_PRD_CHK[0]), 0x01},	//PB20 50H  ;检出电眼
    {21, MK_PART(LUB_ID, 1),  GET_IND(PLUB_PI, I_LUB_CHK), 0x01},					//PB21 56H  ;润滑检知
    {22, MK_PART(ROBOT_ID, 1),  GET_IND(PROBOT_PI, I_ROBOT), 0x01},				//PB22 70H  ;机械手
    {23, MK_PART(CORE_ID, 2),  GET_IND(PCORE_PI, I_CP_FWD), 0x01},				//PB23 34H  ;中子二进终
    {24, MK_PART(CORE_ID, 2),  GET_IND(PCORE_PI, I_CP_BWD), 0x01},				//PB24 35H  ;中子二退终
    {25, MK_PART(CORE_ID, 3),  GET_IND(PCORE_PI, I_CP_FWD), 0x01},
    {26, MK_PART(CORE_ID, 3),  GET_IND(PCORE_PI, I_CP_BWD), 0x01},
    {27, MK_PART(CORE_ID, 4),  GET_IND(PCORE_PI, I_CP_FWD), 0x01},
    {28, MK_PART(CORE_ID, 4),  GET_IND(PCORE_PI, I_CP_BWD), 0x01},


    //扩展输入点，从33开始
    {33, MK_PART(SAFEDOOR_ID, 1),  GET_IND(PSAFEDOOR_PI, I_SDR_OSL), 0x01},		//PB01 4BH  ;安全门开慢
    {34, MK_PART(SAFEDOOR_ID, 1),  GET_IND(PSAFEDOOR_PI, I_SDR_OPN), 0x01},		//PB02 48H  ;完全门开
    {35, MK_PART(SAFEDOOR_ID, 1),  GET_IND(PSAFEDOOR_PI, I_SDR_SAF), 0x01},		//PB03 4AH  ;安全门防碰
    {36, MK_PART(SAFEDOOR_ID, 1),  GET_IND(PSAFEDOOR_PI, I_SDR_SLW), 0x01},		//PB04 49H  ;安全门慢
    {37, MK_PART(LUB_ID, 2), GET_IND(PLUB_PI, I_LUB_CHK), 0x01},					//PB05 5BH  ;润滑检知 #2

    {41, MK_PART(ROBOT_ID, 1),  GET_IND(PROBOT_PI, I_R_CLSM), 0x01},				//PB09 74H  ;关模安全
    {42, MK_PART(ROBOT_ID, 1),  GET_IND(PROBOT_PI, I_R_MAFR), 0x01},			 	//PB10 78H  ;模区安全
    {43, MK_PART(ROBOT_ID, 1),  GET_IND(PROBOT_PI, I_R_EMEG), 0x01},				//PB11 79H  ;机械手急停
    //{44, MK_PART(ROBOT_ID, 1),  GET_IND(PROBOT_PI, I_R_MANUAL), 0x01},				//PB12 7CH  ;机械手联锁
    {45, MK_PART(ROBOT_ID, 1),  GET_IND(PROBOT_PI, I_R_EJTB), 0x01},				//PB13 76H  ;顶退指令
    {46, MK_PART(ROBOT_ID, 1),  GET_IND(PROBOT_PI, I_R_EJTF), 0x01},				//PB14 73H  ;顶进安全
    {47, MK_PART(ROBOT_ID, 1),  GET_IND(PROBOT_PI, I_R_CPRF), 0x01},				//PB15 7AH  ;中子进安全
    {48, MK_PART(ROBOT_ID, 1),  GET_IND(PROBOT_PI, I_R_CPRB), 0x01}
};				//PB16 7BH  ;中子退安全

/*缺省输出点配置表*/
const PO_LOG po_log_factory[] = 	
{
	{1, MK_PART(CLAMP_ID, 1),  GET_IND(PCLAMP_PO, O_CLS_SOL)},  					//PC01 00H  ;关模
    {2, MK_PART(CLAMP_ID, 1),  GET_IND(PCLAMP_PO, O_CLS_ULT)}, 					//PC02 01H  ;差动
    {3, MK_PART(CLAMP_ID, 1),  GET_IND(PCLAMP_PO, O_OPN_SOL)},					//PC03 10H  ;开模
    {4, MK_PART(INJECT_ID, 1),  GET_IND(PINJECT_PO, O_INJ_SOL)},					//PC04 20H  ;射出
    {5, MK_PART(INJECT_ID, 1),  GET_IND(PINJECT_PO, O_SUC_SOL)},					//PC05 27H  ;射退
    {6, MK_PART(INJECT_ID, 1),  GET_IND(PINJECT_PO, O_CHR_SOL)},					//PC06 23H  ;储料
    {7, MK_PART(NOZZLE_ID, 1),  GET_IND(PNOZZLE_PO, O_NOZ_FWD)},					//PC07 30H  ;座进
    {8, MK_PART(NOZZLE_ID, 1),  GET_IND(PNOZZLE_PO, O_NOZ_BWD)},					//PC08 31H  ;座退
    {9, MK_PART(EJECT_ID, 1),  GET_IND(PEJECT_PO, O_EJT_FWD)},					//PC09 34H  ;脱进
    {10, MK_PART(EJECT_ID, 1),  GET_IND(PEJECT_PO, O_EJT_BWD)},					//PC10 35H  ;脱退
    {11, MK_PART(CORE_ID, 1),  GET_IND(PCORE_PO, O_CP_FWD)},						//PC11 40H  ;中子一进
    {12, MK_PART(CORE_ID, 1),  GET_IND(PCORE_PO, O_CP_BWD)},						//PC12 41H  ;中子一退

    {13, MK_PART(ADJUST_ID, 1),  GET_IND(PADJUST_PO, O_ADJ_FWD)},					//PC13 50H  ;调模进
    {14, MK_PART(ADJUST_ID, 1),  GET_IND(PADJUST_PO, O_ADJ_BWD)},					//PC14 51H  ;调模退
    {15, MK_PART(AIRBLOW_ID, 1),  GET_IND(PAIRBLOW_PO, O_AIR_BLT)},						//PC15 70H  ;警报
    {16, MK_PART(AIRBLOW_ID, 2),  GET_IND(PAIRBLOW_PO, O_AIR_BLT)},						//PC16 71H  ;闪光
    {17, MK_PART(CORE_ID, 2),  GET_IND(PCORE_PO, O_CP_FWD)},						//PC17 42H  ;中子二进
    {18, MK_PART(CORE_ID, 2),  GET_IND(PCORE_PO, O_CP_BWD)},						//PC18 43H  ;中子二退
    {19, MK_PART(HYDR_ID, 1),  GET_IND(PHYDR_PO, O_PWRM_P1)},				//PC19 3CH  ;公模吹气
    {20, MK_PART(MACHINE_ID, 1),  GET_IND(PMACHINE_PO, FST_SOL)},					//PC20 0BH  ;快速阀
    {21, MK_PART(CLAMP_ID, 1),  GET_IND(PCLAMP_PO, O_CLS_LOW)},					//PC21 02H  ;关模低压
    {22, MK_PART(CLAMP_ID, 1),  GET_IND(PCLAMP_PO, O_OP_SSLW)},					//PC22 11H  ;开模一慢阀
    {23, MK_PART(AIRBLOW_ID, 2),  GET_IND(PAIRBLOW_PO, O_AIR_BLT)},				//PC23 3DH  ;母模吹气
    //{20, MK_PART(HYDR_ID, 1),  GET_IND(PHYDR_PO, O_PWRM_P1)},					//PC20 0BH  ;快速阀
    //{21, MK_PART(HYDR_ID, 1),  GET_IND(PHYDR_PO, O_PWRM_P2)},					//PC21 02H  ;开模快速
    //{22, MK_PART(HYDR_ID, 1),  GET_IND(PHYDR_PO, O_PWRM_P3)},					//PC22 11H  ;开模低压
    {24, MK_PART(INJECT_ID, 1),  GET_IND(PINJECT_PO, O_CHR_BAC)},					//PC24 24H  ;储料背压
    {25, MK_PART(CORE_ID, 3),  GET_IND(PCORE_PO, O_CP_FWD)},
    {26, MK_PART(CORE_ID, 3),  GET_IND(PCORE_PO, O_CP_BWD)},
    {27, MK_PART(CORE_ID, 4),  GET_IND(PCORE_PO, O_CP_FWD)},
    {28, MK_PART(CORE_ID, 4),  GET_IND(PCORE_PO, O_CP_BWD)},
    {29, MK_PART(HYDR_ID, 1),  GET_IND(PHYDR_PO, O_PWRM_P2)},
    {30, MK_PART(HYDR_ID, 1),  GET_IND(PHYDR_PO, O_PWRM_P3)},
    {31, MK_PART(AIRBLOW_ID, 3),  GET_IND(PAIRBLOW_PO, O_AIR_BLT)},
    {32, MK_PART(AIRBLOW_ID, 4),  GET_IND(PAIRBLOW_PO, O_AIR_BLT)},


    {33, MK_PART(TEMPCTRLCOMM_ID, 1),  GET_IND(PTEMPCTRLCOMM_PO, O_TEMOP[1])},			//PC33 97H  ;电热一
    {34, MK_PART(TEMPCTRLCOMM_ID, 1),  GET_IND(PTEMPCTRLCOMM_PO, O_TEMOP[2])},			//PC34 98H  ;电热二
    {35, MK_PART(TEMPCTRLCOMM_ID, 1),  GET_IND(PTEMPCTRLCOMM_PO, O_TEMOP[3])},			//PC35 99H  ;电热三
    {36, MK_PART(TEMPCTRLCOMM_ID, 1),  GET_IND(PTEMPCTRLCOMM_PO, O_TEMOP[4])},			//PC36 9AH  ;电热四
    {37, MK_PART(TEMPCTRLCOMM_ID, 1),  GET_IND(PTEMPCTRLCOMM_PO, O_TEMOP[5])},			//PC37 9BH  ;电热五
    {38, MK_PART(TEMPCTRLCOMM_ID, 1),  GET_IND(PTEMPCTRLCOMM_PO, O_TEMOP[6])},			//PC36 9CH  ;电热六
    {39, MK_PART(TEMPCTRLCOMM_ID, 1),  GET_IND(PTEMPCTRLCOMM_PO, O_TEMOP[7])},
	{40, MK_PART(TEMPCTRLCOMM_ID, 1),  GET_IND(PTEMPCTRLCOMM_PO, O_TEMOP[8])},
	{41, MK_PART(TEMPCTRLCOMM_ID, 1),  GET_IND(PTEMPCTRLCOMM_PO, O_TEMOP[9])},
	{43, MK_PART(LUB_ID, 1),  GET_IND(PLUB_PO, O_LUBRCTR)}, 					//PC46 72H	;润滑
    {44, MK_PART(ROBOT_ID, 1),  GET_IND(PROBOT_PO, O_ROBOT)},					//PC44 80H  ;机械手
    {45, MK_PART(ALARM_ID, 1),  GET_IND(PALARM_PO, O_FLASH)},					//PC16 71H  ;电热开
    {46, MK_PART(MOTOR_ID, 1),  GET_IND(PMOTOR_PO, O_MTR_STP)},					//PC41 68H  ;马达停
    {47, MK_PART(MOTOR_ID, 1),  GET_IND(PMOTOR_PO, O_MTR_STR)},					//PC42 69H  ;马达开
    {48, MK_PART(MOTOR_ID, 1),  GET_IND(PMOTOR_PO, O_MTR_Y_D)},					//PC43 6AH  ;Y 转 DELTA


    //扩展输出点， 49开始
    {49, MK_PART(SAFEDOOR_ID, 1),  GET_IND(PSAFEDOOR_PO, O_SDR_OPN)},							//PC01 60H  ;安全门开
    {50, MK_PART(SAFEDOOR_ID, 1),  GET_IND(PSAFEDOOR_PO, O_SDR_CLS)},							//PC02 61H  ;安全门关
    {51, MK_PART(SAFEDOOR_ID, 1),  GET_IND(PSAFEDOOR_PO, O_SDR_SLW)},							//PC03 62H  ;安全门慢
    {52, MK_PART(SAFEDOOR_ID, 1),  GET_IND(PSAFEDOOR_PO, O_SDR_FST)},							//PC04 66H  ;安全门快速
	{53, MK_PART(LUB_ID, 2),  GET_IND(PLUB_PO, O_LUBRCTR)}, 								//PC05 6EH	;润滑 #2
	{54, MK_PART(TEMPCTRLCOMM_ID, 1),  GET_IND(PTEMPCTRLCOMM_PO, O_COOL[2])},							//PC06 77H	;冷却器 2

    {55, MK_PART(ROBOT_ID, 1),  GET_IND(PROBOT_PO, O_R_CPRB)},								//PC07 89H  ;机械手中子退 ;;
    {56, MK_PART(ROBOT_ID, 1),  GET_IND(PROBOT_PO, O_R_INJERR)},								//PC08 8AH  射出监控失败
    {57, MK_PART(ROBOT_ID, 1),  GET_IND(PROBOT_PO, O_R_SDRC)},								//PC09 83H  ;机械手安全门关
    {58, MK_PART(ROBOT_ID, 1),  GET_IND(PROBOT_PO, O_R_EJTB)},								//PC10 82H  ;机械手顶退
    {59, MK_PART(ROBOT_ID, 1),  GET_IND(PROBOT_PO, O_R_EJTF)},								//PC11 81H  ;机械手顶进
    {60, MK_PART(ROBOT_ID, 1),  GET_IND(PROBOT_PO, O_R_EMERG)},								//PC12 8BH  ;机械手急停
    {61, MK_PART(ROBOT_ID, 1),  GET_IND(PROBOT_PO, O_R_CPRF)},								//PC13 88H  ;机械手中子进
    {62, MK_PART(ROBOT_ID, 1),  GET_IND(PROBOT_PO, O_R_OPNM)},								//PC14 84H  ;开模结束
    {63, MK_PART(ROBOT_ID, 1),  GET_IND(PROBOT_PO, O_R_AUTO)},								//PC15 87H  ;机械手自动
    {64, MK_PART(ROBOT_ID, 1),  GET_IND(PROBOT_PO, O_R_CLSM)}
};

const E_PO_ROW	e_po_factory[] = {
								  {MK_ACT(CLAMP_ID, 1,  OS_OPNM), 0x1010,    0x0010, 0x000B, 0x0001,   0x0010, 0x000C, 0x0001,   0x0010, 0x000D, 0x0001,   0x0010, 0x000E, 0x0001,   0x0010, 0x000F, 0x0001,   0x0010, 0x0010, 0x0001,	     0x1,	  0x00,0x01,	     4,6,3,0x0711,  1,0,2000,0,    4,1,0,0 },
								  {MK_ACT(CLAMP_ID, 1,  OS_OPNM), 0x1414,    0x0010, 0x000B, 0x0001,   0x0000, 0x0000, 0x0000,   0x0000, 0x0000, 0x0000,   0x0000, 0x0000, 0x0000,   0x0000, 0x0000, 0x0000,   0x0000, 0x0000, 0x0000,  	 0x1,	  0x00,0x01,	     4,6,3,0x0711,  1,0,2000,0,	   4,1,0,0 }
								};


const HY_PRI_ROW hy_pri_factory[] = 
{
	{MK_ACT(INJECT_ID, 1, OS_CHRG), 0x00ff, 0x01, 0x01}, {MK_ACT(INJECT_ID, 1, OS_SUCK), 0x00ff, 0x01, 0x01}, {MK_ACT(EJECT_ID, 1, OS_EJTF), 0x00ff, 0x01, 0x02}, {MK_ACT(EJECT_ID, 1, OS_EJTB), 0x00ff, 0x01, 0x02},
    {MK_ACT(EJECT_ID, 1, OS_VEJTF), 0x00ff, 0x01, 0x02}, {MK_ACT(EJECT_ID, 1, OS_VEJTB), 0x00ff, 0x01, 0x02},
    {MK_ACT(CORE_ID, 1, OS_CPRB), 0x00ff, 0x01, 0x03}, {MK_ACT(CORE_ID, 2, OS_CPRB), 0x00ff, 0x01, 0x03}, {MK_ACT(CORE_ID, 3, OS_CPRB), 0x00ff, 0x01, 0x03},
    {MK_ACT(CORE_ID, 4, OS_CPRB), 0x00ff, 0x01, 0x03}, {MK_ACT(CORE_ID, 5, OS_CPRB), 0x00ff, 0x01, 0x03}, {MK_ACT(CORE_ID, 6, OS_CPRB), 0x00ff, 0x01, 0x03},
    {MK_ACT(CORE_ID, 1, OS_CPRF), 0x00ff, 0x01, 0x03}, {MK_ACT(CORE_ID, 2, OS_CPRF), 0x00ff, 0x01, 0x03}, {MK_ACT(CORE_ID, 3, OS_CPRF), 0x00ff, 0x01, 0x03},
    {MK_ACT(CORE_ID, 4, OS_CPRF), 0x00ff, 0x01, 0x03}, {MK_ACT(CORE_ID, 5, OS_CPRF), 0x00ff, 0x01, 0x03}, {MK_ACT(CORE_ID, 6, OS_CPRF), 0x00ff, 0x01, 0x03},
    {MK_ACT(SHUT_ID, 1, OS_SHUTFWD), 0x00ff, 0x01, 0x03}, {MK_ACT(SHUT_ID, 1, OS_SHUTBWD), 0x00ff, 0x01, 0x03}
};

typedef struct tyFACTORY_TBL_INFO
{
    void* prow;
    UI16 type;
    UI16 row;
} FACTORY_TBL_INFO, *PFACTORY_TBL_INFO;

const FACTORY_TBL_INFO factory_tbl_ind[] = 
{
	{(void*)factory_sub_tbl, T_MACHINE_SUB, ARRAY_NUM(factory_sub_tbl)},
  {(void*)pi_log_factory, T_PI_LOG, ARRAY_NUM(pi_log_factory)},
  {(void*)po_log_factory, T_PO_LOG, ARRAY_NUM(po_log_factory)},
  {(void*)hy_pri_factory, T_HYDR_PRI, ARRAY_NUM(hy_pri_factory)}
};

UI8* SetPartCfgDefault(PCFG_HD phd, UI8*psave, int max_item_num)
{
    PCFG_ITEM pitem;
    UI8* pnow;
    int i;

    UI16* pzd;

    if (phd == NULL || psave == NULL)
    {
        return 0;
    }

    pitem = (PCFG_ITEM)((UI32)(phd + 1) + phd->item_num * phd->item_len);
    pnow = psave;

    /*先装入Zerodebug中温度和DA调整数据*/
    if ((pzd = ZeroData_Get()) != NULL)
    {
        PDA_MACHSET pda;
        PTEMP_ZDSET ptp;

        memset(da_mh_factory, 0, sizeof(da_mh_factory));
        pda = (PDA_MACHSET)da_mh_factory;

        /*装入DA 最大值，最小值*/
        pda->WDA_ZERO[0] = pzd[WDA_PRESSZERO];
        pda->WDA_ZERO[2] = pzd[WDA_PRESSZERO2];
        pda->WDA_ZERO[1] = pzd[WDA_FLUZERO];
        pda->WDA_ZERO[3] = pzd[WDA_FLUZERO2];

        /*用第一组填充剩余组*/
        for (i = 4; i < ARRAY_NUM(pda->WDA_ZERO); i++)
        {
            pda->WDA_ZERO[i] = pda->WDA_ZERO[i & 0x01];
        }

        pda->WDA_MAX[0] = pzd[WDA_PRESSMAX];
        pda->WDA_MAX[2] = pzd[WDA_PRESSMAX2];
        pda->WDA_MAX[1] = pzd[WDA_FLUMAX];
        pda->WDA_MAX[3] = pzd[WDA_FLUMAX2];
        /*用第一组填充剩余组*/
        for (i = 4; i < ARRAY_NUM(pda->WDA_ZERO); i++)
        {
            pda->WDA_MAX[i] = pda->WDA_MAX[i & 0x01];
        }

        /*装入温度归零值和系数*/
		memset(temp_zd_factory, 0, sizeof(temp_zd_factory));
		ptp = (PTEMP_ZDSET)temp_zd_factory;
		ptp->WTEMP_OFFSET = pzd[WTEMP_OFFSET];
		for(i = 0; i < ARRAY_NUM(ptp->WTEMP_CHZERO); i++)
			ptp->WTEMP_CHZERO[i] = pzd[WTEMP_CH0ZERO + (i%8)];
		for(i = 0; i < ARRAY_NUM(ptp->WTEMP_CHB); i++)
			ptp->WTEMP_CHB[i] = pzd[WTEMP_CH0B + (i%8)];
    }

    for (i = 0; i < ARRAY_NUM(factory_part_ind); i++)
    {
        memset(pitem, 0, sizeof(*pitem));
        pitem->item_type = PART_ITEM;
        pitem->item.part_ind = factory_part_ind[i];
        memcpy(pnow, (UI8*)pitem->item.part_ind.psave, pitem->item.part_ind.len * sizeof(UI16));
        pitem->item.part_ind.psave = pnow;
        pnow += pitem->item.part_ind.len * sizeof(UI16);
        pitem = (PCFG_ITEM)((UI32)pitem + phd->item_len);
        phd->item_num++;

        if (phd->item_num > max_item_num)
        {
            break;
        }
    }

    return pnow ;
}


/*返回存储的数据长度*/
UI8* SetTblCfgDefault(PCFG_HD phd, UI8*psave,  int max_item_num)
{
    PCFG_ITEM pitem;
    const TBL_TYPE_INFO* pinfo;

    UI8* pnow;
    UI8* ptmp;
    int i, j;

    if (phd == NULL || psave == NULL)
    {
        return 0;
    }

    pitem = (PCFG_ITEM)((UI32)(phd + 1) + phd->item_num * phd->item_len);
    pnow = psave;
    for (i = 0; i < ARRAY_NUM(factory_tbl_ind); i++)
    {
        if ((pinfo = TBLGetInfo(factory_tbl_ind[i].type)) != NULL)
        {
            memset(pitem, 0, sizeof(*pitem));
            pitem->item_type = TBL_ITEM;
            pitem->item.tbl_ind.hd.type = pinfo->type;
            pitem->item.tbl_ind.hd.ver = pinfo->ver;
            pitem->item.tbl_ind.hd.sn = DEF_TBL_SN;
            pitem->item.tbl_ind.hd.row = factory_tbl_ind[i].row;

            if (pinfo->max_row == 0)
            {
                pitem->item.tbl_ind.hd.max_row = pitem->item.tbl_ind.hd.row;
            }

            pitem->item.tbl_ind.hd.row_len = pinfo->ext_row_len;
            if (pitem->item.tbl_ind.hd.max_row < pitem->item.tbl_ind.hd.row)
            {
                pitem->item.tbl_ind.hd.max_row = pitem->item.tbl_ind.hd.row;
            }
            pitem->item.tbl_ind.psave = pnow;
            if (pitem->item.tbl_ind.hd.row_len == pinfo->row_len)
            {
                memcpy(pnow, factory_tbl_ind[i].prow, pitem->item.tbl_ind.hd.row * pitem->item.tbl_ind.hd.row_len);
                pnow += pitem->item.tbl_ind.hd.row * pitem->item.tbl_ind.hd.row_len;
            }
            else
            {
                ptmp = (UI8*)factory_tbl_ind[i].prow;
                for (j = 0; j < pitem->item.tbl_ind.hd.row; j++)
                {
                    memcpy(pnow, ptmp, pitem->item.tbl_ind.hd.row_len);
                    pnow +=  pitem->item.tbl_ind.hd.row_len;
                    ptmp +=  pinfo->row_len;
                }
            }

            pitem = (PCFG_ITEM)((UI32)pitem + phd->item_len);
            phd->item_num++;
            if (phd->item_num > max_item_num)
            {
                break;
            }
        }
    }

    return pnow ;
}

/*系统配置表没有油路优先级表时，根据程序内置的优先级表设置缺省表*/
void SetHyPriTblDefault()
{
    const TBL_TYPE_INFO* pinfo;

    /*存储到用户表中*/
    pinfo = TBLGetInfo(T_HYDR_PRI);
    if (pinfo != NULL)
    {
        CfgDbTblSave(T_HYDR_PRI, 1, ARRAY_NUM(hy_pri_factory), pinfo->row_len, (void*)hy_pri_factory, FACTORY_TBL_DB);
    }
}

