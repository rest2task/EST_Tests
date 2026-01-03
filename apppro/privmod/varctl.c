#include "database.h"
#include "texttbl.h"
#include "pushtab.h"
#include "varctl.h"
#include "oprintf.h"
#include "servo.h"
#include "varctldata.h"
#include "keymod.h"
#include "panelcfg.h"
#include "privdef.h"
#include "stdlib.h"
#include "verinfo.h"
#include "taskmoni.h"
#include "usermod.h"
#include "stdio.h"
#include "coreprot.h"//20211221 dyl
#include "labelwgt.h"//20220818 dyl 显示/隐藏TAB的提示信息

/********************************自用宏********************************/
#define MAX_STRING			 100  /*combox内容组合最大字节数*/
#define ZERO_DOT			 0    /*没有小数*/
#define ONE_DOT				 1    /*一位小数*/
#define TWO_DOT				 2    /*两位小数*/
#define THREE_DOT			 3    /*三位小数*/
#define ONE_IMG				 1    /*图片索引1*/
#define TWO_IMG				 2    /*图片索引2*/
#define CUSTOMID_BA00		 ((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0xBA00)  /*厂商 威亚*/
#define SYS_SERVO5           (((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x0004) == 0) && ((VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020) != 0))  /*伺服5*/
#define CODE1_2000           ((VarAdrToInt(SYS_FL_MACH_CODE1) & 0x2000) != 0)  /*code1 0x2000*/
#define CODE54_1020          ((VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020) != 0) /*code54 0x1020*/
#define CODE5_8000           ((VarAdrToInt(SYS_FL_MACH_CODE5) & 0x8000) != 0)  /*code5 0x8000*/
#define MAX_TEMP_DEFAULT      450 /*温度设定最大值默认450*/
#define MAX_TEMP_CODE_TEMP_2  350 /*温度设定最大值打温度code 0x02*/
#define SYS_SERVO3           (((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x0004) != 0) && ((VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020) != 0))   /*伺服3*/
#define CUSTOMID_8B00        ((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x8B00)  /*20200226厂商 通用*/
#define CUSTOMID_7A00        ((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x7A00)  /*20200309厂商 威力士*/
#define CUSTOMID_8700        ((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x8700)  /*20201028厂商 海星*/
#define CUSTOMID_6700        ((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x6700)  /*20210518厂商 甬丰*/ //20210518.cyx
#define MAX_ACT              4//最大动作步判断个数

extern UI32 g_warm1,g_warm2,g_warm3,g_warm4,g_warm5,g_warm6,g_warm7,g_warm8,g_warm9,g_warm10;
BOOL b_in_ariblowpage2 = FALSE; //20241210 chj 28组吹气,进入到吹气2画面，复用吹气1画面的控制变灰的变量。
enum MOTOR_STATE
{
	MOTOR_STOP=0,			//停止状态（不亮）
	MOTOR_RUNNING,			//运行状态（常亮）
	MOTOR_START				//启用状态（闪烁）
};

enum enCOLOR{
	COLOR_GREEN= 0x00c01d,
	COLOR_GRAY = 0xA0A0A4,
	COLOR_RED = 0x0042FF, //0xFF4200
	//COLOR_DKGREEN = 0x508000
    COLOR_TRANSPARENT = 0xFFFFFFFF, //20200228 qt透明色
    COLOR_ORANGRED = 0x0033FF, //qt橙红色
};

enum        ITEMID_ACTION {
    ACT_OPEN_BEGIN  = 0x201310,

    ACT_OPEN_END    = 0x201314,

    ACT_CLOSE_BEGIN = 0x201100,

    ACT_CLOSE_END   = 0x201113,

    ACT_INJECT_BEGIN= 0x101100,

    ACT_INJECT_END  = 0x101115,

    ACT_CHARGE_BEGIN= 0x101200,
    ACT_CHARGE_END  = 0x1012FE,

    ACT_HOLD_BEGIN  = 0x10111C,
    ACT_HOLD_END    = 0x101120,

    ACT_SKBACK_BEGIN= 0x101300,
    ACT_SKBACK_END  = 0x101310,

    ACT_NOZADV_BEGIN= 0x111100,
    ACT_NOZADV_END  = 0x111111,

    ACT_NOZRET_BEGIN= 0x111200,
    ACT_NOZRET_END  = 0x111211,

    ACT_LEJECTADV_BEGIN= 0x211200,
    ACT_LEJECTADV_END  = 0x211211,
    ACT_LEJECTRET_BEGIN= 0x211300,
    ACT_LEJECTRET_END  = 0x211312,

    ACT_REJECTADV_BEGIN= 0x212200,
    ACT_REJECTADV_END  = 0x212211,
    ACT_REJECTRET_BEGIN= 0x212300,
    ACT_REJECTRET_END  = 0x212312,

    ACT_CORE_BEGIN  = 0x221100,
    ACT_CORE_END    = 0x226210,
    ACT_AIR_BEGIN  = 0x231110,
    ACT_AIR_END    = 0x23E110,
    ACT_COOL_BEGIN  = 0x111500,
    ACT_COOL_END    = 0x111610
};

/********************************varctl外部dic********************************/
#define EJECT1_FL_EJTFIRSTSET       0x2113000A    /*托膜首次独立行程*/
#define EJECT1_FL_EJTCTRL           0x21130002    /*托模方式*/
#define FL_EJTVIBRATEMODE           0x21130004    /*震动托膜方式*/
#define ADPOSI_EJTADVFASTEND        0x21140009    /*托膜进终止位置*/
#define TM_EJTVIBRATE               0x21130005    /*震动托膜时间*/
#define ADPOSI_EJTVIBRATEDISTANCE   0x21130006    /*震动托膜距离*/
#define EJECT1_PO_O_EJT_FWD	        0x21100000	  /*托进阀*/
#define EJECT1_PO_O_EJT_BWD	        0x21100003	  /*托退阀*/
#define EJECT1_PI_I_EJT_FWD	        0x21110000    /*托模进终*/ 
#define EJECT1_PI_I_EJT_BWD	        0x21110001    /*托模退终*/ 
#define CLAMP_PI_I_CLS_END	        0x20110003    /*合模到底*/
#define CLAMP_MOLDSET_CLS3		    0x20130003    /* 0004 关模3段使用  0:不使用  1：使用  默认0*/
#define CLAMP_ACTCLS2END		    0x2014000B    /* 000C 关模二段终止位置*/
#define CLAMP_MACHSET_CLSHIPRES2	0x20120032    /*关模高压2段  0:不使用   1:使用  默认不使用*/
#define CLAMP_ACTPARA_OPN4          0x2014001B    /* 001C 开模四段终止位置*/
#define CLAMP_MOLDSET_OPN4		    0x20130009    /* 000B 开模4段使用  0:不使用  1：使用  默认0*/
#define CLAMP_ACTPARA_OPN3		    0x2014001A    /* 001B 开模三段终止位置*/
#define CLAMP_PO_O_CLS_SOL		    0x20100000	  /*关模*/
#define CLAMP_PO_O_OPN_SOL		    0x20100007	  /*开模*/
#define HOLE_SPEED_MAX              0x32130008    /*保压速度上限*/
#define ADPOSI_INJ2END              0x1014003d    /*射出2段终止位置*/
#define ADPOSI_INJ3END              0x1014003e    /*射出3段终止位置*/
#define ADPOSI_INJ4END              0x1014003f    /*射出4段终止位置*/
#define ADPOSI_INJ5END              0x10140040    /*射出5段终止位置*/
#define ADPOSI_INJ6END              0x10140041    /*射出6段终止位置*/
#define INJECT_PO_O_INJ_SOL	        0x10100000	  /*射出阀*/
#define NOZZLE_PO_O_NOZ_FWD	        0x11100000	  /*座进阀*/
#define SEC_CHG                     0x10130018    /*储料使用段数*/
#define CHG_3_POS_END               0x10140057    /*储料三段终止位置*/
#define ADPOSI_SUCKBACKEND          0x1014005c    /*射退终止位置*/
#define ADPOSI_SUCKBACKDISTANCE     0x1014005d    /*射退距离*/
#define SUCK_BACK_BEFORE_CHG        0x10130020    /*储料前射退模式 0=不使用 1=距离 2=时间  默认0.0*/
#define CHG_2_POS_END               0x10140056    /*储料二段终止位置*/
#define INJECT_PO_O_CHR_SOL         0x10100007    /*储料*/
#define INJECT_PO_O_SUC_SOL         0x1010000D    /*射退*/
#define FL_OPENJTCORE               0x00130003    /*开模联动选项索引*/
#define FL_CLOSEJTCORE              0x00130012    /*关模联动选项索引*/
#define FL_OPENDRAINFUNCTIO         0x20130004    /*开模泄压选择*/
#define OTHER_PRS_MAX               0x32130004    /*小动作压力上限*/
#define ADDR_INCACC					0x10130000	  /*射出增速储能功能*/
#define COM_PRS_FUNC                0x00130001    /*压塑功能*/
#define EXHAUST_MODE                0x20130016    /*排气方式*/
#define AUTO_PURGE                  0x10130016    /*自动清料选择*/
#define COREA_FL_CORE_CTRL			0x22130001    //中子A功能选择
#define COREB_FL_CORE_CTRL          0x22230001    /*中子B功能选择 (0=不用 1=中子 2=绞牙) */
#define COREC_FL_CORE_CTRL          0x22330001    /*中子C功能选择 (0=不用 1=中子 2=绞牙) */
#define CORED_FL_CORE_CTRL          0x22430001    /*中子D功能选择 (0=不用 1=中子 2=绞牙) */
#define COREE_FL_CORE_CTRL          0x22530001    /*中子E功能选择 (0=不用 1=中子 2=绞牙) */
#define COREF_FL_CORE_CTRL          0x22630001    /*中子F功能选择 (0=不用 1=中子 2=绞牙) */
#define COREA_FL_SPECIALCODE        0x22130004    /*特殊中子代码(0 = 不使用， 1 - 6) */
#define TEMPCTRL_RESPONSELESSCYCLE  0x05130034    /*温度无感应的周期时间*/
#define TEMPCTRL_DACENT_HEATERON    0x05130035    /*温度无感应的通电百分比*/
#define ADJUST_ADPOSI_ADJMD         0x2414000a    /*调模模式(0：长动 1：点动 2：点动加长动)*/
#define ADJUST_CN_ADJFIRST          0x24140008    /*每次点动计时*/
#define NOZZLE_PO_O_NOZ_BWD         0x11100002    /*座退*/
#define FL_PWRDOORCTRL              0x29130000    /*自动安全门*/
#define FL_ROBOTCTRL                0x31130000    /*机械手功能选择*/
#define INJECT_POS_INJCUSHION       0x10160005    /*60C 射出转保压位置*/
#define INJECT_POS_INJHOLDEND       0x10160004    /*60B 射出终点位置实际值*/
#define ADDR_I_SDR_CLS				0x29110000	  /*安全门输出点*/
#define ADDR_I_SDR_CLS2				0x29110001	  /*安全门2输出点*/
#define ADDR_I_RSD_CLS				0x29110006	  /*后安全门输出点*/
#define ADDR_I_RPM_CHK				0x10110008	  /*螺杆转数检知*/
#define ADDR_I_OPN_END				0x2011000B	  /*开模终*/
#define ADDR_I_LUB_CHK1				0x30110001	  /*润滑检知1*/
#define ADDR_I_LUB_CHK2				0x30210001	  /*润滑检知2*/
#define ADDR_I_LUB_CHK3				0x30310001	  /*润滑检知3*/
#define ADDR_I_PRD_CHK				0x32110000	  /*检出电眼*/
#define ADDR_I_NOZ_FWD				0x11110000	  /*座进终*/
#define ADDR_I_NOZ_BWD				0x11110002	  /*座退终*/
#define ADDR_SERVO_ERR2				0x0A16001B    //伺服警报2
#define ADDR_SERVO_ERR3				0x0A16001C    //伺服警报3
#define ADDR_SERVO_ERR4				0x0A16001D    //伺服警报4
#define ADDR_SERVO_ERR5				0x0A16001E    //伺服警报5
#define ADDR_SERVO_ERR6				0x0A16001F    //伺服警报6
#define ADDR_SERVO_ERR7				0x0A160020    //伺服警报7
#define ADDR_SERVO_ERR8				0x0A160021    //伺服警报8
#define ADDR_SERVO_CANST2			0x0A160013    //伺服CAN通信状态2
#define ADDR_SERVO_CANST3			0x0A160014    //伺服CAN通信状态3
#define ADDR_SERVO_CANST4			0x0A160015    //伺服CAN通信状态4
#define ADDR_SERVO_CANST5			0x0A160016    //伺服CAN通信状态5
#define ADDR_SERVO_CANST6			0x0A160017    //伺服CAN通信状态6
#define ADDR_SERVO_CANST7			0x0A160018    //伺服CAN通信状态7
#define ADDR_SERVO_CANST8			0x0A160019    //伺服CAN通信状态8
#define TEMP_STATE_LM73             0x05160022    //室温LM73的温度 
#define HYDR_FL_PWRMATCHTBL         0x09120001    /* 0170 使用面板PUMP配置表*/
#define AIRBLOW1_PO_AIR_SEQVLV	    0x23100002	  //顺序阀1
#define AIRBLOW1_FL_SEQ_USE         0x23130006    //顺序阀1使用方式
#define EXTIOST                     0x00160014    //io扩展板状态
#define EXTDAST                     0x00160015    //da扩展板状态
#define EXTTEMPST                   0x00160016    //温度扩展板状态
#define MDTEMP_STATEX3              0x33120036    //模温在线状态3
#define MDTEMP_STATEX4              0x33120037    //模温在线状态4
#define MDTEMP_STATEX5              0x33120038    //模温在线状态5
#define MDTEMP_STATEX6              0x33120039    //模温在线状态6
#define MDTEMP_STATEX7              0x3312003a    //模温在线状态7
#define SERVO_SELFLEARNPROMPT       0x331300da    //自学习步奏或警报提示
#define ADDR_SUCKBACKEND            0x1014005c    //射退终止位置
#define ADDR_ROBOTCTRL				0x31130000	  //机械手功能
#define ADDR_MOLDLUBRIC				0x30160002	  //润滑状态
#define ADDR_MOLDLUBRIC2			0x30260002	  //润滑状态2
#define ADDR_MOLDLUBRIC3			0x30360002	  //润滑状态3
#define SYS_TM_CYCLE_LAST           0x00160009    //本次循环实际值
#define CLAMP_STATE_TM_CLAMPCLS_RT  0x20160039    /*实时计算的关模时间，用于面板显示, 0.00s。*/
#define CLAMP_STATE_TM_CLAMPOPN_RT  0x2016003A    /*实时计算的开模时间，用于面板显示, 0.00s。*/
#define INJECT_STATE_TM_CHARGE_RT   0x1016004F    //实时计算的储料时间，用于面板显示, 0.00s。
#define NOZZLE_STATE_TM_COOL_RT  	0x11160004    //实时计算的冷却时间，用于面板显示, 0.00s。
#define SYSTEM_STATE_TM_CYCLE		0x00160005    //循环计时
#define CLAMP_STATE_TM_CLAMPCLS     0x20160007    //关模计时实际值            0.00
#define STINJECTTIMELAST            0x3316001d    //上一模射出计时
#define INJECT_STATE_TM_CHARGE      0x1016000A    // 60D 储料计时实际值  0.00S
#define COOL_TIME_LAST              0x3316001e    //上一模冷却时间
#define CLAMP_STATE_TM_CLAMPOPN     0x2016000E          //开模计时实际值            0.00
#define SYSTEM_STATE_TM_CYCLE_LAST  0x00160009 //改为产品的制品时间，不包括再循环时间//本次循环实际值
#define TEMPCTRL2_MACH_DATEMP_RAMP1  0x0522000C //* 00DA 温度加温缓冲区1           # 1- 16 */
#define TEMPCTRL2_STATE_FL_TEMPSTATUS1     0x05260012 // 570 温度加温状态1  # 0 -15
#define SWPUSHPOSOPT                0x3312006e //十字头是否可见
#define AIRBLOW1_14AGAIN               0x3312006f //吹气1-14多次是否使用


/*---------------托膜界面逻辑模块-Ej---------------------*/
static void EjAirModeCtl();
static void EjBefDly();
static void EjMod();
static void VibrateMode();
static void EjImgVal();
static void EjActBkColor();//20200302 动作步背景色
static void EjtAdvEndPosMax();//20210318

/**********************开关模界面逻辑模块-CLP************************************/
static void ClpMaxMinDot();
static void IntellClpFunc();
static void PushPosVisEnable10V();
static void ClampMeterMax2();
static void PushPosVisEnable();
static void ClpImgVal();
static void ClsSegChg();
static void OpenSegChg();
static void ClpActBkColor();//20200302 动作步背景色
static void PerSlowDistVisible();//外设慢速距离可见
static void AIClampOpenRampShow();//20200803
static void OpenLinkPosMin();//20210219
static void OpenDiffEndPosShow();//20210518

/****************************射出界面逻辑模块-Inj****************************/
static void InjVal();
static void InjImgVal();
static void InjActBkColor();//20200302 动作步背景色
static void NeedleVlvVisible();//20200317胶口针阀是否可见（威力士时，标准胶口针阀不可见）
static void NdVlvPosEnable();//20200317威力士针阀开阀位置是否可用
static void NdVlv2PosEnable();//20250109 fqh 胶口针阀2针阀开阀位置是否可用
static void PumpADPrsSensorChnlShow();
static void InjPrsAndHoldSpdModeSet();
static void Inj1TimeEnable();//20210302

/****************************储料界面逻辑模块-Chg****************************/
static void ChgVal();
static void ChgWeiYa();
static void ChgImgVal();
static void ChgActBkColor(); //20200302 动作步背景色

/**************************托膜功能界面变量模块-EJ*****************************/
static void EjRetEndFilterTmEnable();

/**************************开关模能界面变量模块-Clp*****************************/
static void LinkClpVal();
static void FloatCylindVal();
static void OpenRetardTmEnable();

/****************************射出界面功能逻辑模块-Inj****************************/
static void TurnToHold();
static void AccVal();
static void ComPrsVal();
static void OpenLoopEnable();
static void OilLeakVisible();//20200309 漏油功能是否可见

/****************************射出功能2界面逻辑模块-Inj****************************/
static void InjFunc2Val();
static void InjCurve();

/****************************储料功能界面逻辑模块-CHG****************************/
static void ChgFuncVal();
static void AutoPurgeAloneEnable();//20200225自动清料走独立压力流量控制
static void AutoPurgeVal();//20200306自动清料最大值和小数位切换

/****************************中子界面逻辑模块-CORE****************************/
static void CountOrTravel();
static void ActInOrOut();
static void CoreActBkColor();//20200302 动作步背景色
static void CorePointSignalBkColor();//20211221 dyl 中子点位信号背景色

/****************************中子功能界面逻辑模块-CORE****************************/
static void SpecialCoreFuncVISIBLE();
static void WayCoreOutMinPosMax();
static void SpecialCoreFuncEnable();

/****************************温度界面变量模块-TEMP****************************/
static void ManualTempEnable();
static void TempSetValMax();
static void TempImgVal();
static void TempVisible();
static void Temp1SetEnable();
static void OutputCycleMin();
static void Temp_AutoToWarmVisible();//20211201 dyl 自动转保温
static void HeaterCheckDegreeVisible();

/****************************其它界面变量模块-OTHER****************************/
static void EveryInchingStepEnable();
static void PosAdjustEnable();
static void NozPosEnable();
static void NozMeterVisible();
static void PressorVisible();
static void ZeroVisible();
static void NozPoBwdImgVal();
static void QDCFunctionEnable();//20200226
static void NozAdjActBkColor();//20200302
static void SetNozzlePosMax();//20200325

/****************************其它功能界面变量模块-OTHER****************************/
static void AutoSafeDoorEnable();
static void WayRobotFuncEnable();
static void RobCore1LinkEnable();
static void RobCore2LinkEnable();
static void SpecialLinkEnable();
static void WayRobotOutputTmVal();

/****************************开关模参数一界面变量模块-CLP****************************/
static void ClsHighPrsKeepTmVal();

/****************************开关模参数二界面变量模块-CLP****************************/
static void RampVal();
static void OpnDiffVlvActTmEnable();
static void PushPosVislble();

/****************************射出参数界面变量模块-INJ****************************/
static void ManlUseDebugPrsSpdEnable();
static void ManlUseDebugPrsSpdEnable10V();

/****************************储料参数界面变量模块-CHG****************************/
static void ConfluChgSpdMaxEnable();

/****************************中子参数界面变量模块-CORE****************************/
static void CoreSelectEnable();

/****************************温度参数一界面变量模块-TEMP****************************/
static void ExtraTempVisible();
static void OneToSevenVal();

/****************************其它参数一界面变量模块-Other****************************/
static void ReleasemdInitialEnable();
static void LockPrsEffareaMax();

/****************************其它参数二界面变量模块-Other****************************/
static void MoldThickAdjustEnable();
static void MoldThickAdjust();

/****************************主界面变量模块-Main****************************/
static void MainVal();
static void ServoStVisible();
static void MotorVisible();
static void ServoStOneToSevenVisible();
static void ServoColorOneToSevenVisible();

/****************************电子尺调整逻辑界面-AD_Adjust****************************/
static void A920Ser3ModleEnable();

/****************************DA调整界面-DA_Adjust****************************/
static void WeiYaEnable();

/****************************数据检测界面-Data_Diag****************************/
static void Code3Fill2Enable();

/****************************主测试界面-Main_Adjust****************************/
static void AddSignVisible();

/****************************主框架界面-Mainframe****************************/
static void UseServoVisible();

/****************************泵浦配置界面-Pump_cfg****************************/
static void PumpAdjustEnable();
static void MaxReverseSpdMin();

/****************************泵浦配置额外界面-Pump_cfg_x****************************/
static void PumpVisible();
static void ViceStrStopEnable();
static void PumbAdj2to7Enable();

/****************************顺序阀界面-Seq_Vlv****************************/
static void SetSequenceVlvMode();
static void SeqVlvImgVal();

/****************************伺服曲线界面-Servo_Curve****************************/
static void Servo3Enable();
static void Servo5Hide();

/****************************伺服测试3界面-Servo_Test3****************************/
static void SysPrsVisible();

/****************************系统参数界面-Sys_Para****************************/
static void ClpPrsMaxCtrl();//20200302 通用开关模压力上限密码控制

/****************************系统参数3界面-Sys_Para3****************************/
static void SpecialDataSetupVisible();

/****************************系统参数4界面-Sys_Para5****************************/
static void InjSlowStartSpdVal();
static void InjSlowStartDataSetupEnable();

/****************************版本界面-Version****************************/
static void ExBoardEnable();
static void AD78Visible();

/****************************系统自学习界面-Servo_Cfg****************************/
static void ServoSelfLearnOrAlarm();

/****************************数据保护界面-Data_Protect****************************/
static void CoolTmProtect();

/*****************************主界面变量模块-Main*********************************************/
static void MainClampImgVal();
static void MainActPng();
static void LockShow();

/*************************************温度界面-区别标准液压7-8-10寸*****************************************/
static void Temp16ImgVal();
static void Temp6to9Ctl();
static void TempExtraCtl();
static void TempParaExchange();

/*****************************托模界面-Eject*********************************************/
static void BlowAgainCtl();
static void DlyTmActTmEnable();
static void EjtRet2PosMax();

static void BlowCoreKeyVisibl();//20191224 吹气和中子切换按键是否可见  目前R10使用

/*****************************射出界面-Inject*********************************************/
static void HoldCurve();
static void InjPrsVal();

/*****************************日月年产量*********************************************/
static void DatePowerVisible();

/**************************托膜参数界面变量模块-EJ*****************************/
static void EjAdvHoldEnable();
/**************************储料功能界面变量模块-CHG*****************************/
static void WeilishiEnable();
/**************************扩展温度界面变量模块-Tempxx*****************************/
static void TempCtlStatusVal();
static void NormalTempImgVal();
static void BarrelCoolBiasEnable();
static void MouthVisible();

/**************************生产警报界面-Product_Alarm*****************************/
static void BarrelTempBiasEnable();

/**************************IO重定义界面-IO_Redefine*****************************/
static void TempCtrlPartChg();
static void IoRdInputOrOutputMax();

/**************************润滑界面-Lubr*****************************/
static void LubModCntMax(); //20200220 润滑模数上限

/**************************生产界面-ProdManage1*****************************/
static void ForcedRejectVisible(); //20200303 自动初期不良品数是否可见

/**************************PID开关模画面-clampx*****************************/
static void ClampxPID();

/**************************调机画面-AdjMach*****************************/
static void ServoFuncCurveShow();

/**************************特殊功能1画面-Spec_Func1*****************************/
static void OpenLoopOverAdjShow();

/**************************更新画面-ProgUpdate*****************************/
static void UpdateIAPShow();//20211229 dyl IAP

static void SetTabTipsEnable();//20220818 dyl 显示/隐藏TAB的提示信息

void VarCtlMoni(void)
{
    if ((GetPanelType() == PANEL_V10 || GetPanelType() == PANEL_VT15) || (PanelCfgStyle() == G_TYPE) || (PanelCfgStyle() == P_TYPE))
	{
        //SpecialLinkEnable();
		PushPosVislble();
        //MainClampImgVal();
        //MainActPng();
        //Temp16ImgVal();
        //Temp6to9Ctl();
        //TempExtraCtl();
        //TempParaExchange();
        //BlowAgainCtl();
        //HoldCurve();
        //ManlUseDebugPrsSpdEnable10V();
        //PushPosVisEnable10V();
        EjAdvHoldEnable();
        //WeilishiEnable();
        if((PanelCfgStyle() == G_TYPE) || (PanelCfgStyle() == P_TYPE))
        {
            SpecialCoreFuncEnable();
            TempCtlStatusVal();
            NormalTempImgVal();
            BarrelCoolBiasEnable();
            OpenSegChg();
            ClsSegChg();
            BarrelTempBiasEnable();
            TempCtrlPartChg();
            IoRdInputOrOutputMax();
            MouthVisible();
            ClampxPID();
        }
    }
	else
	{
		ManlUseDebugPrsSpdEnable();
		PushPosVisEnable();
	}
    DlyTmActTmEnable();
    EjAirModeCtl();
    //DatePowerVisible();
    //EjBefDly();
    EjMod();
    VibrateMode();
    //EjImgVal();
	ClpMaxMinDot();
    //IntellClpFunc();
    ClampMeterMax2();
    //ClpImgVal();
	InjVal();
    //InjImgVal();
	ChgVal();
	ChgWeiYa();
    //ChgImgVal();
    //EjRetEndFilterTmEnable();
	LinkClpVal();
	FloatCylindVal();
    //OpenRetardTmEnable();
	TurnToHold();
	AccVal();
	ComPrsVal();
	OpenLoopEnable();
	InjFunc2Val();
    //InjCurve();
	ChgFuncVal();
	CountOrTravel();
	ActInOrOut();
	SpecialCoreFuncVISIBLE();
	WayCoreOutMinPosMax();
    //ManualTempEnable();
	TempSetValMax();
    Temp_AutoToWarmVisible();//20211201 dyl 自动转保温
    HeaterCheckDegreeVisible();//20211228 dyl 电热连续加温检查度数
    //TempImgVal();
    //TempVisible();
    //Temp1SetEnable();
	EveryInchingStepEnable();
	PosAdjustEnable();
	NozPosEnable();
    //NozMeterVisible();
    //PressorVisible();
    //ZeroVisible();
    //NozPoBwdImgVal();
	AutoSafeDoorEnable();
	WayRobotFuncEnable();
	RobCore1LinkEnable();
	RobCore2LinkEnable();
    //ClsHighPrsKeepTmVal();
    //RampVal();
    //OpnDiffVlvActTmEnable();
	
    //ConfluChgSpdMaxEnable();
    //CoreSelectEnable();
    //ExtraTempVisible();
    //OneToSevenVal();
	ReleasemdInitialEnable();
	LockPrsEffareaMax();
	MoldThickAdjustEnable();
	MoldThickAdjust();
    MainVal();//M788
    //ServoStVisible();
    //MotorVisible();
    //ServoStOneToSevenVisible();
    //ServoColorOneToSevenVisible();
    //A920Ser3ModleEnable();
	WeiYaEnable();
    //Code3Fill2Enable();
    //AddSignVisible();
    //UseServoVisible();
	PumpAdjustEnable();
	PumpVisible();
	ViceStrStopEnable();
	PumbAdj2to7Enable();
    //SetSequenceVlvMode();
    //SeqVlvImgVal();
    //Servo3Enable();
    //SysPrsVisible();
	SpecialDataSetupVisible();
    InjSlowStartDataSetupEnable();
	ExBoardEnable();
    AD78Visible();
    ServoSelfLearnOrAlarm();
    //CoolTmProtect();
    WayRobotOutputTmVal();
    InjSlowStartSpdVal();

    //BlowCoreKeyVisibl();//20191224 芯动吹气和中子按键是否可见
    LubModCntMax();//20200220
    AutoPurgeAloneEnable();//20200225
    QDCFunctionEnable();//20200226
    AutoPurgeVal();//20200306

    //20200302 动作步背景色
    EjActBkColor();
    ClpActBkColor();
    InjActBkColor();
    ChgActBkColor();
    CoreActBkColor();
    CorePointSignalBkColor();//20211221 dyl 中子点位信号背景色
    NozAdjActBkColor();

    ClpPrsMaxCtrl();
    ForcedRejectVisible();//20200303 自动不良品数显示
    OilLeakVisible();//20200309 漏油功能是否显示
    NeedleVlvVisible();//20200317胶口针阀是否可见（威力士时，标准胶口针阀不可见）
    NdVlvPosEnable();//20200317威力士针阀开阀位置是否可用
    NdVlv2PosEnable();//20250109 fqh 胶口针阀2针阀开阀位置是否可用
    SetNozzlePosMax();//20200325 座台位置最大值转换
    PerSlowDistVisible();//20200603 外设慢速距离可见
    OutputCycleMin();//20200604 输出周期最小值
    ServoFuncCurveShow();//20200623 驱动器FUNC曲线显示可见
    PumpADPrsSensorChnlShow();//20200628 油泵压力传感器AD通道显示
    MaxReverseSpdMin();//20200805 最大反向速度最小值
    AIClampOpenRampShow();//20200803 开模到位减速计算斜率可见
    InjPrsAndHoldSpdModeSet();//20200819 射出压力保压速度模式设置
    OpenLoopOverAdjShow();//20201028 开环压力超调可见
    InjPrsVal();//20201208 射出压力小数位
    EjtRet2PosMax();//20201218 托退二段位置最大值
    OpenLinkPosMin();//20210219 开模联动位置最小值
    Inj1TimeEnable();//20210302 射出一段时间可用
    EjtAdvEndPosMax();//20210318 托进终止位置最大值
    OpenDiffEndPosShow();//20210518 开模差动结束位置可见
    LockShow();//20210712.cyx
    Servo5Hide();//20210623 伺服5隐藏
    UpdateIAPShow();//20211229 dyl IAP
    SetTabTipsEnable();//20220818 dyl 显示/隐藏TAB的提示信息
}

/************************托膜界面逻辑模块-Ej***************************/

/*****************************************
*Subject  :吹气方式选择-仅手动模式启动本函数
*Condition:0开模时间-1开模位置-2射出时间-3射出位置-4关模时间-5关模位置-6托进位置
*Object   :吹气结束位置
*Result   :不可用-可用-不可用-可用

*Object   :吹气结束位置
*Result   :开关模电子尺最大值-开关模电子尺最大值-射出电子尺最大值-射出电子尺最大值
******************************************/
static void EjAirModeCtl()
{
    UI16 i;
    const UI16 air_num1to6 = 6;
    const UI16 air_num7to14 = 8;
    UI32 wAddr = FL_BLAST1;
    if(b_in_ariblowpage2)//20241210 chj 28组吹气,进入到吹气2画面，复用吹气1画面的控制变灰的变量。
    {
        wAddr = d_airblow1_MOLDSET_FL_BLAST2;
    }

    /*托模界面*/
    for (i = 0; i < air_num1to6; ++i)
    {
        switch(VarAdrToInt(wAddr + i * 0x00100000))//20241210 chj 28组吹气,进入到吹气2画面，复用吹气1画面的控制变灰的变量。
        {
        case 0:
        case 10://20241205 chj 吹气方式增加开模后
            VarAdrSetInt(AIR1_POS_ENABLE + i, FALSE);
            VarAdrSetInt(AIR1_POS_MAX + i, VarAdrToInt(ADPOSI_OPEN5));
            break;
        case 1:
            VarAdrSetInt(AIR1_POS_ENABLE + i, TRUE);
            VarAdrSetInt(AIR1_POS_MAX + i, VarAdrToInt(ADPOSI_OPEN5));
            break;
        case 2:
            VarAdrSetInt(AIR1_POS_ENABLE + i, FALSE);
            VarAdrSetInt(AIR1_POS_MAX + i, VarAdrToInt(ADDR_MAX_ADINJECT));
            break;
        case 3:
            VarAdrSetInt(AIR1_POS_ENABLE + i, TRUE);
            VarAdrSetInt(AIR1_POS_MAX + i, VarAdrToInt(ADDR_MAX_ADINJECT));
            break;
        case 4:
            VarAdrSetInt(AIR1_POS_ENABLE + i, FALSE);
            VarAdrSetInt(AIR1_POS_MAX + i, VarAdrToInt(ADPOSI_OPEN5));
            break;
        case 5:
            VarAdrSetInt(AIR1_POS_ENABLE + i, TRUE);
            VarAdrSetInt(AIR1_POS_MAX + i, VarAdrToInt(ADPOSI_OPEN5));
            break;
        case 6:
            VarAdrSetInt(AIR1_POS_ENABLE + i, TRUE);
            VarAdrSetInt(AIR1_POS_MAX + i, VarAdrToInt(ADDR_MAX_ADEJECT));
            break;
        case 7://20200304
            VarAdrSetInt(AIR1_POS_ENABLE + i, FALSE);
            VarAdrSetInt(AIR1_POS_MAX + i, VarAdrToInt(ADDR_MAX_ADEJECT));
            break;
        case 8://Eject ret pos
            VarAdrSetInt(AIR1_POS_ENABLE + i, TRUE);
            VarAdrSetInt(AIR1_POS_MAX + i, VarAdrToInt(ADDR_MAX_ADEJECT));
            break;
        case 9://Charge Pos
            VarAdrSetInt(AIR1_POS_ENABLE + i, TRUE);
            if (VarAdrToInt(SEC_CHG) == 1)//20220208 dyl 储料改为1~3段
            {
                VarAdrSetInt(AIR1_POS_MAX + i, VarAdrToInt(d_inject1_ACTPARA_ADPOSI_CHG1END));
            }
            else if (VarAdrToInt(SEC_CHG) == 2)
            {
                VarAdrSetInt(AIR1_POS_MAX + i, VarAdrToInt(d_inject1_ACTPARA_ADPOSI_CHG2END));
            }
            else if (VarAdrToInt(SEC_CHG) == 3)
            {
                VarAdrSetInt(AIR1_POS_MAX + i, VarAdrToInt(d_inject1_ACTPARA_ADPOSI_CHG3END));
            }
//            VarAdrSetInt(AIR1_POS_MAX + i, VarAdrToInt(ADDR_MAX_ADINJECT));
            break;
        case 256://Not Use
            VarAdrSetInt(AIR1_POS_ENABLE + i, FALSE);
            VarAdrSetInt(AIR1_POS_MAX + i, VarAdrToInt(ADDR_MAX_ADINJECT));
            break;
        default:
            break;
        }
    }

    wAddr = FL_BLAST7;//20241210 chj 28组吹气,进入到吹气2画面，复用吹气1画面的控制变灰的变量。
    if(b_in_ariblowpage2)
    {
        wAddr = d_airblow7_MOLDSET_FL_BLAST2;
    }
    /*吹气界面*/
    for (i = 0; i < air_num7to14; ++i)
    {
        switch(VarAdrToInt(wAddr + i * 0x00100000))//20241210 chj 28组吹气,进入到吹气2画面，复用吹气1画面的控制变灰的变量。
        {
        case 0:
        case 10://20241205 chj 吹气方式增加开模后
            VarAdrSetInt(AIR7_POS_ENABLE + i, FALSE);
            VarAdrSetInt(AIR7_POS_MAX + i, VarAdrToInt(ADPOSI_OPEN5));
            break;
        case 1:
            VarAdrSetInt(AIR7_POS_ENABLE + i, TRUE);
            VarAdrSetInt(AIR7_POS_MAX + i, VarAdrToInt(ADPOSI_OPEN5));
            break;
        case 2:
            VarAdrSetInt(AIR7_POS_ENABLE + i, FALSE);
            VarAdrSetInt(AIR7_POS_MAX + i, VarAdrToInt(ADDR_MAX_ADINJECT));
            break;
        case 3:
            VarAdrSetInt(AIR7_POS_ENABLE + i, TRUE);
            VarAdrSetInt(AIR7_POS_MAX + i, VarAdrToInt(ADDR_MAX_ADINJECT));
            break;
        case 4:
            VarAdrSetInt(AIR7_POS_ENABLE + i, FALSE);
            VarAdrSetInt(AIR7_POS_MAX + i, VarAdrToInt(ADDR_MAX_ADCLAMP));
            break;
        case 5:
            VarAdrSetInt(AIR7_POS_ENABLE + i, TRUE);
            VarAdrSetInt(AIR7_POS_MAX + i, VarAdrToInt(ADDR_MAX_ADCLAMP));
            break;
        case 6:
            VarAdrSetInt(AIR7_POS_ENABLE + i, TRUE);
            VarAdrSetInt(AIR7_POS_MAX + i, VarAdrToInt(ADDR_MAX_ADEJECT));
            break;
        case 7://20200304
            VarAdrSetInt(AIR7_POS_ENABLE + i, FALSE);
            VarAdrSetInt(AIR7_POS_MAX + i, VarAdrToInt(ADDR_MAX_ADEJECT));
            break;
        case 8://Eject ret pos
            VarAdrSetInt(AIR7_POS_ENABLE + i, TRUE);
            VarAdrSetInt(AIR7_POS_MAX + i, VarAdrToInt(ADDR_MAX_ADEJECT));
            break;
        case 9://Charge Pos
            VarAdrSetInt(AIR7_POS_ENABLE + i, TRUE);
            if (VarAdrToInt(SEC_CHG) == 1)//20220208 dyl 储料改为1~3段
            {
                VarAdrSetInt(AIR7_POS_MAX + i, VarAdrToInt(d_inject1_ACTPARA_ADPOSI_CHG1END));
            }
            else if (VarAdrToInt(SEC_CHG) == 2)
            {
                VarAdrSetInt(AIR7_POS_MAX + i, VarAdrToInt(d_inject1_ACTPARA_ADPOSI_CHG2END));
            }
            else if (VarAdrToInt(SEC_CHG) == 3)
            {
                VarAdrSetInt(AIR7_POS_MAX + i, VarAdrToInt(d_inject1_ACTPARA_ADPOSI_CHG3END));
            }
//            VarAdrSetInt(AIR7_POS_MAX + i, VarAdrToInt(ADDR_MAX_ADINJECT));
            break;
        case 256://Not Use
            VarAdrSetInt(AIR7_POS_ENABLE + i, FALSE);
            VarAdrSetInt(AIR7_POS_MAX + i, VarAdrToInt(ADDR_MAX_ADINJECT));
            break;
        default:
            break;
        }
    }
}


/*****************************************
*Subject  :CODE0
*Condition:0-2000
*Object   :托膜前延迟
*Result   :一位小数-两位小数

*Object   :托膜前延迟-最大值
*Result   :999-9999
******************************************/

static void EjBefDly()
{
    if (VarAdrToInt(SYS_FL_MACH_CODE0) & 0x2000)
    {
        VarAdrSetInt(BEF_DLY_DOT, TWO_DOT);
		VarAdrSetInt(BEF_DLY_MAX, 9999);
    }
    else
    {
        VarAdrSetInt(BEF_DLY_DOT, ONE_DOT);
		VarAdrSetInt(BEF_DLY_MAX, 999);
    }
}


/*****************************************
*Subject  :托膜方式
*Condition:0停留-1定次-2震动
*Object   :托膜首次独立行程-combox索引
*Result   :0-保持-0

*Object   :托膜次数
*Result   :不使用-使用-使用

*Object   :托膜震动方式
*Result   :不使用-不使用-使用

*Object   :托膜震动时间/距离
*Result   :不使用-不使用-使用

*Object   :托膜首次独立行程
*Result   :不使用-使用-不使用

*Object   :停留时托进保持是否可用
*Result   :使用-不使用-不使用

*Object   :托进前延迟方式是否可用

*Object   :托退前延迟方式是否可用

*Object   :托模和吹气动作步背景色 20200228
******************************************/

static void EjMod()
{

    switch(VarAdrToInt(EJECT1_FL_EJTCTRL))
    {
    case 0:
		VarAdrSetInt(EJ_COUNT_ENABLE, FALSE);
		VarAdrSetInt(VIBRATE_MOD_ENABLE, FALSE);
		VarAdrSetInt(VIBRATE_TM_ENABLE, FALSE);
		VarAdrSetInt(FST_TRAVEL_ENABLE, FALSE);
		VarAdrSetInt(STAY_EJTADV_KEEP_ENABLE, TRUE);
		VarAdrSetInt(DLY_MODE_BEF_EJTAdV_ENABLE, TRUE);
		VarAdrSetInt(DLY_MODE_BEF_EJTRET_ENABLE, TRUE);
        break;
    case 1:
        if(VarAdrToInt(d_autoctrl1_MOLDSET_FL_EJTCTRL) == 0)
        {
            VarAdrSetInt(EJ_COUNT_ENABLE, FALSE);
        }
        else
        {
            VarAdrSetInt(EJ_COUNT_ENABLE, TRUE);
        }
		VarAdrSetInt(VIBRATE_MOD_ENABLE, FALSE);
		VarAdrSetInt(VIBRATE_TM_ENABLE, FALSE);
		VarAdrSetInt(FST_TRAVEL_ENABLE, TRUE);
		VarAdrSetInt(STAY_EJTADV_KEEP_ENABLE, FALSE);
		VarAdrSetInt(DLY_MODE_BEF_EJTAdV_ENABLE, TRUE);
		VarAdrSetInt(DLY_MODE_BEF_EJTRET_ENABLE, TRUE);
        break;
    case 2:       
        if(VarAdrToInt(d_autoctrl1_MOLDSET_FL_EJTCTRL) == 0)
        {
            VarAdrSetInt(EJ_COUNT_ENABLE, FALSE);
            VarAdrSetInt(DLY_MODE_BEF_EJTAdV_ENABLE, TRUE);//20220929 dyl
            VarAdrSetInt(DLY_MODE_BEF_EJTRET_ENABLE, TRUE);
        }
        else
        {
            VarAdrSetInt(EJ_COUNT_ENABLE, TRUE);
            VarAdrSetInt(DLY_MODE_BEF_EJTAdV_ENABLE, FALSE);//20220929 dyl
            VarAdrSetInt(DLY_MODE_BEF_EJTRET_ENABLE, FALSE);
        }
        if(VarAdrToInt(d_eject1_MOLDSET_CN_EJT) != 0)//20220831 dyl 顶针次数为0时，震动参数变灰
        {
            VarAdrSetInt(VIBRATE_MOD_ENABLE, TRUE);
            VarAdrSetInt(VIBRATE_TM_ENABLE, TRUE);
        }
        else
        {
            VarAdrSetInt(VIBRATE_MOD_ENABLE, FALSE);
            VarAdrSetInt(VIBRATE_TM_ENABLE, FALSE);
        }
		VarAdrSetInt(FST_TRAVEL_ENABLE, FALSE);
		VarAdrSetInt(STAY_EJTADV_KEEP_ENABLE, FALSE);
//		VarAdrSetInt(DLY_MODE_BEF_EJTAdV_ENABLE, FALSE);
//		VarAdrSetInt(DLY_MODE_BEF_EJTRET_ENABLE, FALSE);
        break;
    default:
        break;
    }
}


/*****************************************
*Subject  :震动托膜方式
*Condition:0时间-1位置
*Object   :震动托膜时间/距离-labelVISIBLE
*Result   :时间可见-距离可见

*Object   :震动托膜时间/距离-小数位
*Result   :两位-一位

*Object   :震动托膜时间/距离-最小值
*Result   :0-50

*Object   :震动托膜时间/距离-最大值
*Result   :999-托膜进终止位置

*Object   :震动托膜时间/距离-单位
*Result   :s-mm
******************************************/

static void VibrateMode()
{
    if (VarAdrToInt(FL_EJTVIBRATEMODE))
    {
        VarAdrSetInt(VIBRATE_TM_VISIBLE, FALSE);
        VarAdrSetInt(VIBRATE_DIST_VISIBLE, TRUE);
		VarAdrSetInt(VIBRATE_TorD_DOT, ONE_DOT);
		VarAdrSetInt(VIBRATE_TorD_MIN, 50);
		VarAdrSetInt(VIBRATE_TorD_MAX, VarAdrToInt(ADPOSI_EJTADVFASTEND));
		VarAdrSetStr(VIBRATE_TorD_UNIT, "mm");
    }
    else
    {
        VarAdrSetInt(VIBRATE_TM_VISIBLE, TRUE);
        VarAdrSetInt(VIBRATE_DIST_VISIBLE, FALSE);
		VarAdrSetInt(VIBRATE_TorD_DOT, TWO_DOT);
		VarAdrSetInt(VIBRATE_TorD_MIN, 0);
		VarAdrSetInt(VIBRATE_TorD_MAX, 999);
		VarAdrSetStr(VIBRATE_TorD_UNIT, "s");
    }
}

/*****************************************
*Subject  :托膜位置
*Condition:根据托膜电子尺最大位置和当前位置
*Object   :托膜位置图片
*Result   :1，2，3

*Subject  :托进阀
*Condition:0-1
*Object   :托进阀图片
*Result   :1-2

*Subject  :托退阀
*Condition:0-1
*Object   :托退阀图片
*Result   :1-2

*Subject  :托膜进终
*Condition:0-1
*Object   :托膜进终图片
*Result   :1-2

*Subject  :托膜退终
*Condition:0-1
*Object   :托膜退终图片
*Result   :1-2

*Subject  :合模到底
*Condition:0-1
*Object   :合模到底图片
*Result   :1-2
******************************************/

static void EjImgVal()
{
    static UI8  cEjt=0;
    static UI16 wAdEject=0;
    int  i = 0;
    UI16 uDevision = VarAdrToUI16(ADDR_MAX_ADEJECT) / 30;
    UI16 value = VarAdrToUI16(ADDR_ADPOSI_REAL3);

    while ((value >= (8*uDevision*i + 5*uDevision)) && (i <2))
    {
        i++;
    } 
    if(i != cEjt)
    {
        if(abs(value - wAdEject) > 20)
        {
            cEjt =  i;
            wAdEject = value;
        }
    }
    VarAdrSetInt(EJ_POS_IMG_VAL, cEjt + 1);   /*平台图片命名:eje1-eje2-eje3*/


	if (VarAdrToInt(EJECT1_PO_O_EJT_FWD) == 0)
	{
		VarAdrSetInt(EJ_FWD_PO_IMG_VAL, ONE_IMG);
	} 
	else
	{
		VarAdrSetInt(EJ_FWD_PO_IMG_VAL, TWO_IMG);
	}

	if (VarAdrToInt(EJECT1_PO_O_EJT_BWD) == 0)
	{
		VarAdrSetInt(EJ_BWD_PO_IMG_VAL, ONE_IMG);
	} 
	else
	{
		VarAdrSetInt(EJ_BWD_PO_IMG_VAL, TWO_IMG);
	}

	if (VarAdrToInt(EJECT1_PI_I_EJT_FWD) == 0)
	{
		VarAdrSetInt(EJ_FWD_PI_IMG_VAL, ONE_IMG);
	} 
	else
	{
		VarAdrSetInt(EJ_FWD_PI_IMG_VAL, TWO_IMG);
	}

	if (VarAdrToInt(EJECT1_PI_I_EJT_BWD) == 0)
	{
		VarAdrSetInt(EJ_BWD_PI_IMG_VAL, ONE_IMG);
	}
	else
	{
		VarAdrSetInt(EJ_BWD_PI_IMG_VAL, TWO_IMG);
	}

	if (VarAdrToInt(CLAMP_PI_I_CLS_END) == 0)
	{
		VarAdrSetInt(CLS_END_PI_IMG_VAL, ONE_IMG);
	} 
	else
	{
		VarAdrSetInt(CLS_END_PI_IMG_VAL, TWO_IMG);
	}
}

/*****************************************
*Subject  :
*Condition:
*Object   :托膜动作步背景色变化
*Result   :
******************************************/
static void EjActBkColor()
{
    //20200302 动作步背景色
    UI16 i;
    UI16 stIndex = 0xFF;
    static UI16 stIndexOld = 0xFF;
    static BOOL init_bkcolor = TRUE;
    UI32 addrstep[MAX_ACT] = {VarAdrToInt(ACT_ACTNAME1),VarAdrToInt(ACT_ACTNAME2),VarAdrToInt(ACT_ACTNAME3),VarAdrToInt(ACT_ACTNAME4)};
    UI32 stGroup[] = {p_PP_OBJ_EjAdv1,p_PP_OBJ_EjAdv2,p_PP_OBJ_EjRet1,p_PP_OBJ_EjRet2};
    UI32 stGroupAir[] = {p_PP_OBJ_Air1,p_PP_OBJ_Air2,p_PP_OBJ_Air3,p_PP_OBJ_Air4,
                        p_PP_OBJ_Air5,p_PP_OBJ_Air6,p_PP_OBJ_Air7,p_PP_OBJ_Air8,
                        p_PP_OBJ_Air9,p_PP_OBJ_Air10,p_PP_OBJ_Air11,p_PP_OBJ_Air12,
                        p_PP_OBJ_Air13,p_PP_OBJ_Air14,};

    //20200302
    if(init_bkcolor)
    {
        init_bkcolor = FALSE;
        for (i = 0;  i< ARRAY_NUM(stGroup); i++)
        {
            VarAdrSetInt(stGroup[i],COLOR_TRANSPARENT);
        }

        for (i = 0;  i< ARRAY_NUM(stGroupAir); i++)
        {
            VarAdrSetInt(stGroupAir[i],COLOR_TRANSPARENT);
        }
    }

    if(TRUE)//20201028 背景色变化做标准
    {
        for(i = 0; i < MAX_ACT; i++)//20210220
        {
            switch(addrstep[i]){
            case 0x21120000:
            case 0x21121000:
            case 0x21140000:
            case 0x21141000:
            case 0x21160000:
            case 0x21161000:
            case 0x21220000:
            case 0x21221000:
                stIndex = 0;    //Eject Adv #1
                break;
            case 0x21121100:
            case 0x21161100:
            case 0x21221100:
                stIndex = 1;    //Eject Adv #2
                break;
            case 0x21130000:
            case 0x21130001:
            case 0x21130002:
            case 0x21130101:
            case 0x21131000:
            case 0x21170000:
            case 0x21171000:
            case 0x21230000:
            case 0x21231000:
                stIndex = 2;    //Eject Ret #1
                break;
            case 0x21131100:
            case 0x21231100:
            case 0x21131200:
            case 0x21231200:
                stIndex = 3;    //Eject Ret #2
                break;
            default:
                stIndex = 0xFF;
            }

            if(stIndex != 0xFF)
            {
                break;
            }
        }

        if(stIndex != stIndexOld)
        {
            if((stIndex != 0xFF)&&(stIndexOld != 0xFF)){
                VarAdrSetInt(stGroup[stIndexOld],COLOR_TRANSPARENT);
                VarAdrSetInt(stGroup[stIndex],COLOR_ORANGRED);//0x0033ff
            }
            else if(stIndexOld == 0xFF){
                VarAdrSetInt(stGroup[stIndex],COLOR_ORANGRED);
            }
            else if(stIndex == 0xFF){
                VarAdrSetInt(stGroup[stIndexOld],COLOR_TRANSPARENT);
            }
            stIndexOld = stIndex;
        }

        //20201028 吹气输出状态显示
        for (i = 0;  i< ARRAY_NUM(stGroupAir); i++)
        {
            //20241210 chj 28组吹气,进入到吹气2画面，复用吹气1画面的控制变灰的变量。
            UI32 wAddr = d_airblow1_PO_O_AIR_BLT;
            if(b_in_ariblowpage2)
            {
                wAddr = d_airblow1_PO_O_AIR_BLT2;
            }
            if(VarAdrToInt(wAddr + i*0x100000))
            {
                VarAdrSetInt(stGroupAir[i],COLOR_ORANGRED);
            }
            else
            {
                VarAdrSetInt(stGroupAir[i],COLOR_TRANSPARENT);
            }
        }
    }

}


/**************************托膜功能界面变量模块-Ej*****************************/

/*****************************************
*Subject  :
*Condition:
*Object   :托退终滤波时间是否可用
*Result   :
******************************************/

static void EjRetEndFilterTmEnable()
{
	if (CUSTOMID_BA00)
	{
		VarAdrSetInt(EJRET_END_FILTER_TM_ENABLE, FALSE);
	} 
	else
	{
		VarAdrSetInt(EJRET_END_FILTER_TM_ENABLE, TRUE);
	}
}

/***********************Ej-END******************************/




/**********************开关模界面逻辑模块-Clp************************************/

/*****************************************
*Object   :开模一段位置小数位

*Object   :低压锁模位置小数位

*Object   :高压锁模位置小数位

*Object   :关模一段最大值

*Object   :关模二段位置最大值

*Object   :关模二段位置最小值

*Object   :关模三段位置最小值

*Object   :关模低压位置最大值

*Object   :关模低压最小值

*Object   :开模一段位置最大值

*Object   :开模二段位置最小值

*Object   :开模三段位置最大值

*Object   :开模四段位置最小值

*Object   :开模终止位置最小值
******************************************/
static void ClpMaxMinDot()
{
    UI32 machid = (VarAdrToInt(SYS_MACH_CUSTOMID) & 0xff00);
    UI32 code43 = (VarAdrToInt(SYSTEM_MACH_CODE43) & 0x0400);

	if (PushtableType())
	{
		VarAdrSetInt(OPEN1_POS_DOT, TWO_DOT);
		VarAdrSetInt(CLS_LOW_PRS_POS_DOT, TWO_DOT);
		VarAdrSetInt(CLS_HI_PRS_POS_DOT, TWO_DOT);

        VarAdrSetInt(p_PP_OBJ_OPNMD_MIN_POS,VarAdrToInt(ADPOSI_OPEN1) / 10);
        VarAdrSetInt(p_PP_OBJ_BREAKMD_MAX_POS,VarAdrToInt(ADPOSI_OPEN5) * 10);
        if(VarAdrToInt(ADPOSI_OPEN5) * 10 > 65000)//20241026 chj 修复开关模位置最大值超过65535.00问题--破模位置最大值
        {
            VarAdrSetInt(p_PP_OBJ_BREAKMD_MAX_POS_UI32, 65000);
        }
        else
        {
            VarAdrSetInt(p_PP_OBJ_BREAKMD_MAX_POS_UI32, VarAdrToInt(ADPOSI_OPEN5) * 10);
        }
    }
	else
	{
		VarAdrSetInt(OPEN1_POS_DOT, ONE_DOT);
		VarAdrSetInt(CLS_LOW_PRS_POS_DOT, ONE_DOT);
		VarAdrSetInt(CLS_HI_PRS_POS_DOT, ONE_DOT);

        VarAdrSetInt(p_PP_OBJ_OPNMD_MIN_POS,VarAdrToInt(ADPOSI_OPEN1));
        VarAdrSetInt(p_PP_OBJ_BREAKMD_MAX_POS,VarAdrToInt(ADPOSI_OPEN5));
        VarAdrSetInt(p_PP_OBJ_BREAKMD_MAX_POS_UI32,VarAdrToInt(ADPOSI_OPEN5));//20241026 chj 修复开关模位置最大值超过65535.00问题--破模位置最大值
	}

	if (machid == 0x7200 || machid == 0xBA00)
	{
		VarAdrSetInt(CLS1_POS_MAX, VarAdrToInt(ADPOSI_OPEN5));
	} 
	else
	{
		VarAdrSetInt(CLS1_POS_MAX, VarAdrToInt(ADDR_MAX_ADCLAMP));
	}

    if (VarAdrToInt(CLAMP_MOLDSET_CLS3) == 1 || ((VarAdrToInt(d_clamp1_MOLDSET_FL_CLSOPEN_MODE)==2 || VarAdrToInt(d_clamp1_MOLDSET_FL_CLSOPEN_MODE)==3 || VarAdrToInt(d_clamp1_MOLDSET_FL_CLSOPEN_MODE)==4) && ((PanelCfgStyle()==G_TYPE) || (PanelCfgStyle()==P_TYPE))))
    {
        if ((code43 != 0) && SYS_SERVO3)
        {
            if (machid == 0x7200 || machid == 0xBA00)
            {
                VarAdrSetInt(CLS2_POS_MAX, VarAdrToInt(ADPOSI_OPEN5));
            } 
            else
            {
                VarAdrSetInt(CLS2_POS_MAX, VarAdrToInt(ADDR_ADPOSI_REAL2));
            }
        } 
        else
        {
            VarAdrSetInt(CLS2_POS_MAX, VarAdrToInt(ADPOSI_CLOSE1));
        }

		VarAdrSetInt(CLS2_POS_MIN, VarAdrToInt(ADPOSI_CLOSE3));

		if (PushtableType())
		{
            VarAdrSetInt(CLS3_POS_MIN, VarAdrToInt(ADPOSI_CLOSELOW) / 10);
            VarAdrSetInt(CLS_LOW_PRS_POS_MAX, VarAdrToInt(ADPOSI_CLOSE3) * 10);
            if(VarAdrToInt(ADPOSI_CLOSE3) * 10 > 65000)//20241026 chj 修复开关模位置最大值超过65535.00问题 -关模低压位置最大值
            {
                VarAdrSetInt(p_PP_OBJ_CLS_LOW_PRS_POS_MAX_UI32, 65000);
            }
            else
            {
                VarAdrSetInt(p_PP_OBJ_CLS_LOW_PRS_POS_MAX_UI32, VarAdrToInt(ADPOSI_CLOSE3) * 10);
            }
        }
		else
		{
            VarAdrSetInt(CLS3_POS_MIN, VarAdrToInt(ADPOSI_CLOSELOW));
			VarAdrSetInt(CLS_LOW_PRS_POS_MAX, VarAdrToInt(ADPOSI_CLOSE3));
            VarAdrSetInt(p_PP_OBJ_CLS_LOW_PRS_POS_MAX_UI32, VarAdrToInt(ADPOSI_CLOSE3));//20241026 chj 修复开关模位置最大值超过65535.00问题-关模低压位置最大值
		}
    } 
    else
    {
        if ((code43 != 0) && SYS_SERVO5)
        {
            if (machid == 0x7200 || machid == 0xBA00)
            {
                VarAdrSetInt(CLS2_POS_MAX, VarAdrToInt(ADPOSI_OPEN5));
            } 
            else
            {
                VarAdrSetInt(CLS2_POS_MAX, VarAdrToInt(ADDR_MAX_ADCLAMP));
            }
        } 
        else
        {
            VarAdrSetInt(CLS2_POS_MAX, VarAdrToInt(ADPOSI_CLOSE1));
        }

		if (PushtableType())
		{
            VarAdrSetInt(CLS2_POS_MIN, VarAdrToInt(ADPOSI_CLOSELOW) / 10);
			VarAdrSetInt(CLS_LOW_PRS_POS_MAX, VarAdrToInt(CLAMP_ACTCLS2END) * 10);
            if(VarAdrToInt(CLAMP_ACTCLS2END) * 10 > 65000)//20241026 chj 修复开关模位置最大值超过65535.00问题-关模低压位置最大值
            {
                VarAdrSetInt(p_PP_OBJ_CLS_LOW_PRS_POS_MAX_UI32, 65000);
            }
            else
            {
                VarAdrSetInt(p_PP_OBJ_CLS_LOW_PRS_POS_MAX_UI32, VarAdrToInt(CLAMP_ACTCLS2END) * 10);
            }
        }
		else
		{
            VarAdrSetInt(CLS2_POS_MIN, VarAdrToInt(ADPOSI_CLOSELOW));
			VarAdrSetInt(CLS_LOW_PRS_POS_MAX, VarAdrToInt(CLAMP_ACTCLS2END));
            VarAdrSetInt(p_PP_OBJ_CLS_LOW_PRS_POS_MAX_UI32, VarAdrToInt(CLAMP_ACTCLS2END));//20241026 chj 修复开关模位置最大值超过65535.00问题 -关模低压位置最大值
		}
    }

	if (VarAdrToInt(CLAMP_MACHSET_CLSHIPRES2) == 1)
	{
        VarAdrSetInt(CLS_LOW_PRS_POS_MIN, VarAdrToInt(ADPOSI_CLOSEHIGH));
	} 
	else
	{
		VarAdrSetInt(CLS_LOW_PRS_POS_MIN, 0);
	}

    if((PanelCfgStyle()==G_TYPE) || (PanelCfgStyle()==P_TYPE))
    {
        ;
    }
    else
    {
        if (CODE5_8000 && SYS_SERVO5)
        {
            if (VarAdrToInt(CLAMP_MOLDSET_OPN4) == 1)
            {
                if (PushtableType())
                {
                    VarAdrSetInt(OPEN1_POS_MAX, VarAdrToInt(CLAMP_ACTPARA_OPN4) * 10);
                    if(VarAdrToInt(CLAMP_ACTPARA_OPN4) * 10 > 65000)//20241026 chj 修复开关模位置最大值超过65535.00问题一段位置最大值
                    {
                        VarAdrSetInt(p_PP_OBJ_OPEN1_POS_MAX_UI32, 65000);
                    }
                    else
                    {
                        VarAdrSetInt(p_PP_OBJ_OPEN1_POS_MAX_UI32, VarAdrToInt(CLAMP_ACTPARA_OPN4) * 10);
                    }
                    VarAdrSetInt(OPEN4_POS_MIN, VarAdrToInt(ADPOSI_OPEN1) / 10);
                }
                else
                {
                    VarAdrSetInt(OPEN1_POS_MAX, VarAdrToInt(CLAMP_ACTPARA_OPN4));
                    VarAdrSetInt(p_PP_OBJ_OPEN1_POS_MAX_UI32, VarAdrToInt(CLAMP_ACTPARA_OPN4));//20241026 chj 修复开关模位置最大值超过65535.00问题一段位置最大值
                    VarAdrSetInt(OPEN4_POS_MIN, VarAdrToInt(ADPOSI_OPEN1));
                }
                VarAdrSetInt(OPEN_END_POS_MIN, VarAdrToInt(CLAMP_ACTPARA_OPN4));
            }
        }
        else
        {
            if (PushtableType())
            {
                VarAdrSetInt(OPEN1_POS_MAX, VarAdrToInt(ADPOSI_OPEN2) * 10);
                if(VarAdrToInt(ADPOSI_OPEN2) * 10 > 65000)//20241026 chj 修复开关模位置最大值超过65535.00问题一段位置最大值
                {
                    VarAdrSetInt(p_PP_OBJ_OPEN1_POS_MAX_UI32, 65000);
                }
                else
                {
                    VarAdrSetInt(p_PP_OBJ_OPEN1_POS_MAX_UI32, VarAdrToInt(ADPOSI_OPEN2) * 10);
                }
                VarAdrSetInt(OPEN2_POS_MIN, VarAdrToInt(ADPOSI_OPEN1) / 10);
            }
            else
            {
                VarAdrSetInt(OPEN1_POS_MAX, VarAdrToInt(ADPOSI_OPEN2));
                VarAdrSetInt(p_PP_OBJ_OPEN1_POS_MAX_UI32, VarAdrToInt(ADPOSI_OPEN2));//20241026 chj 修复开关模位置最大值超过65535.00问题一段位置最大值
                VarAdrSetInt(OPEN2_POS_MIN, VarAdrToInt(ADPOSI_OPEN1));
            }

            if (VarAdrToInt(CLAMP_MOLDSET_OPN4) == 1)
            {
                VarAdrSetInt(OPEN3_POS_MAX, VarAdrToInt(CLAMP_ACTPARA_OPN4));
                VarAdrSetInt(OPEN4_POS_MIN, VarAdrToInt(CLAMP_ACTPARA_OPN3));
                VarAdrSetInt(OPEN_END_POS_MIN, VarAdrToInt(CLAMP_ACTPARA_OPN4));
            }
            else
            {
                VarAdrSetInt(OPEN3_POS_MAX, VarAdrToInt(ADPOSI_OPEN5));
                VarAdrSetInt(OPEN_END_POS_MIN, VarAdrToInt(CLAMP_ACTPARA_OPN3));
            }
        }
    }

    //20200629 关模高压二段最大值
    if(VarAdrToInt(d_clamp1_MACHSET_FL_CLSHIPRES2))
    {
        VarAdrSetInt(p_PP_OBJ_CLSHIPRS2_MAX, VarAdrToInt(d_clamp1_ACTPARA_ADPOS_CLSHIPRES2));
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_CLSHIPRS2_MAX, 0);
    }

    //20250103 chj 威力士快速机模式下，对关模高压二段速度的最大值，位置的最小值，范围进行限制
    if(((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x7A00) && VarAdrToInt(p_PP_MACHSET_WLSMACHINE_TYPE))
    {
        VarAdrSetInt(p_PP_OBJ_CLS_HI2_SPD_MAX,30);
        if(PushtableType())
        {
            VarAdrSetInt(p_PP_OBJ_CLS_HI2_POS_MIN,20);
        }
        else
        {
            VarAdrSetInt(p_PP_OBJ_CLS_HI2_POS_MIN,2);
        }
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_CLS_HI2_SPD_MAX,VarAdrToInt(d_autoctrl1_MOLDSET_DAHIFLOW_CLPLIMIT));
        VarAdrSetInt(p_PP_OBJ_CLS_HI2_POS_MIN,0);
    }
}
/*****************************************
*Object   :智能关模功能是否可用

*Object   :智能开模功能是否可用

*Object   :开模四段选项是否可用

*Object   :开模一段文本

*Object   :开模四段文本

*Object   :开模终止文本

*Object   :开模二段十字头位置是否使用

*Object   :开模三段十字头位置是否使用

*Object   :code0-code5-code54-智能开模

*Object   :关模一段文本

*Object   :关模二段文本

*Object   :关模一段十字头位置

*Object   :code43-code0-code54-智能关模

*Object   :高压锁模2十字头位置是否使用

*Object   :关模十字头单位是否使用

*Object   :反-高压锁模2是否使用

*Object   :开模四段压力是否使用

*Object   :智能关模启动距离是否可用

*Object   :智能关模启动保护时间是否可用

*Object   :关模三段选项是否可用
******************************************/
static void IntellClpFunc()
{

	if (SYS_SERVO5)
	{
		VarAdrSetInt(INTELL_CLS_OPTION_ENABLE, TRUE);
		VarAdrSetInt(INTELL_OPEN_OPTION_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(INTELL_CLS_OPTION_ENABLE, FALSE);
		VarAdrSetInt(INTELL_OPEN_OPTION_ENABLE, FALSE);
	}

	if (CODE5_8000 && SYS_SERVO5)
	{
		VarAdrSetInt(OPEN_4_OPTION_ENABLE, FALSE);
		VarAdrSetStr(OPEN1_STR_VAL, GetTextTran(TEXT_CLAMP_START));
		VarAdrSetStr(OPEN4_STR_VAL, GetTextTran(TEXT_CLAMP_FAST));
		VarAdrSetStr(OPEN_END_STR_VAL, GetTextTran(TEXT_CLAMP_END));
		VarAdrSetStr(OPEN_END_STR1_VAL, GetTextTran(TEXT_CLAMP_END));
		VarAdrSetInt(INTELL_OPEN_FUNC_CODE, FALSE);
		if (PushtableType())
		{
			VarAdrSetInt(OPEN2_PUSH_POS_ENABLE, FALSE);
			VarAdrSetInt(OPEN3_PUSH_POS_ENABLE, FALSE);
		}
	} 
	else
	{
		VarAdrSetInt(OPEN_4_OPTION_ENABLE, TRUE);
		VarAdrSetStr(OPEN1_STR_VAL, GetTextTran(TEXT_CLAMP_1));
		VarAdrSetStr(OPEN4_STR_VAL, GetTextTran(TEXT_CLAMP_4));
		VarAdrSetStr(OPEN_END_STR_VAL, GetTextTran(TEXT_CLAMP_5));
		VarAdrSetStr(OPEN_END_STR1_VAL, GetTextTran(TEXT_CLAMP_NOSPC));
		VarAdrSetInt(INTELL_OPEN_FUNC_CODE, TRUE);
		if (PushtableType())
		{
			VarAdrSetInt(OPEN2_PUSH_POS_ENABLE, TRUE);
			VarAdrSetInt(OPEN3_PUSH_POS_ENABLE, TRUE);
		}
	}

	if (((VarAdrToInt(SYSTEM_MACH_CODE43) & 0x0400) != 0) && SYS_SERVO5)
	{
		VarAdrSetInt(INTELLCLS_DIST_ENABLE, TRUE);
		VarAdrSetInt(INTELLCLS_TM_ENABLE, TRUE);
		VarAdrSetInt(CLS_3_OPTION_ENABLE, FALSE);
		VarAdrSetStr(CLS1_STR_VAL, GetTextTran(TEXT_CLAMP_START));
		VarAdrSetStr(CLS2_STR_VAL, GetTextTran(TEXT_CLAMP_FAST));
		if (PushtableType())
		{
			VarAdrSetInt(CLS1_PUSH_POS_ENABLE, FALSE);
		}
		VarAdrSetInt(INTELL_CLS_FUNC_CODE, FALSE);
	} 
	else
	{
		VarAdrSetInt(INTELLCLS_DIST_ENABLE, FALSE);
		VarAdrSetInt(INTELLCLS_TM_ENABLE, FALSE);
		VarAdrSetInt(CLS_3_OPTION_ENABLE, TRUE);
		VarAdrSetStr(CLS1_STR_VAL, GetTextTran(TEXT_CLAMP_1));
		VarAdrSetStr(CLS2_STR_VAL, GetTextTran(TEXT_CLAMP_CLS2));
		if (PushtableType())
		{
			VarAdrSetInt(CLS1_PUSH_POS_ENABLE, TRUE);
		}
		VarAdrSetInt(INTELL_CLS_FUNC_CODE, TRUE);
	}

	if (VarAdrToInt(CLAMP_MOLDSET_OPN4) == 1)
	{
		if (CODE5_8000 && SYS_SERVO5)
		{
			VarAdrSetInt(OPEN4_PRS_ENABLE, FALSE);
		} 
		else
		{
			VarAdrSetInt(OPEN4_PRS_ENABLE, TRUE);
		}
	} 
	else
	{
		VarAdrSetInt(OPEN4_PRS_ENABLE, FALSE);
	}
}

static void PushPosVisEnable10V()
{
	if (VarAdrToInt(CLAMP_MACHSET_CLSHIPRES2) == 1)
	{
		if (PushtableType() && VarAdrToInt(SWPUSHPOSOPT))
		{
			VarAdrSetInt(PUSH_POS_UNIT2_VISIBLE, FALSE);
			VarAdrSetInt(HI_PRS2_PUSH_POS_VISIBLE, TRUE);
		}
		else
		{
			VarAdrSetInt(HI_PRS2_PUSH_POS_VISIBLE, FALSE);
		}
		VarAdrSetInt(NOT_HI_PRS2_POS_ENABLE, FALSE);
	} 
	else
	{
		if (PushtableType() && VarAdrToInt(SWPUSHPOSOPT))
		{
			VarAdrSetInt(PUSH_POS_UNIT2_VISIBLE, TRUE);
		}
		else
		{
			VarAdrSetInt(PUSH_POS_UNIT2_VISIBLE, FALSE);
		}
		VarAdrSetInt(NOT_HI_PRS2_POS_ENABLE, TRUE);
		VarAdrSetInt(HI_PRS2_PUSH_POS_VISIBLE, FALSE);
	}
}

static void PushPosVisEnable()
{
	if (VarAdrToInt(CLAMP_MACHSET_CLSHIPRES2) == 1)
	{
		if (PushtableType())
		{
			VarAdrSetInt(PUSH_POS_UNIT2_VISIBLE, FALSE);
			VarAdrSetInt(HI_PRS2_PUSH_POS_VISIBLE, TRUE);
		}
		else
		{
			VarAdrSetInt(HI_PRS2_PUSH_POS_VISIBLE, FALSE);
		}
		VarAdrSetInt(NOT_HI_PRS2_POS_ENABLE, FALSE);
	} 
	else
	{
		if (PushtableType() && VarAdrToInt(SWPUSHPOSOPT))
		{
			VarAdrSetInt(PUSH_POS_UNIT2_VISIBLE, TRUE);
		}
		else
		{
			VarAdrSetInt(PUSH_POS_UNIT2_VISIBLE, FALSE);
		}
		VarAdrSetInt(NOT_HI_PRS2_POS_ENABLE, TRUE);
		VarAdrSetInt(HI_PRS2_PUSH_POS_VISIBLE, FALSE);
	}
}

/*****************************************
*Object   :开关模电子尺最大值
******************************************/

static void ClampMeterMax2()
{
    static I16 adMaxClamp = -1;
    static UI32 adTransMaxClamp = -1;//推力座转换最大限制值
    //static I16 wDatax= -1;
    static UI32 wDatax1= 0;
    UI32 wData;

    if(PushtableType() != 0)
    {
        if(adMaxClamp != VarAdrToUI16(ADDR_MAX_ADCLAMP))
        {
            adMaxClamp = VarAdrToUI16(ADDR_MAX_ADCLAMP);
            adTransMaxClamp = TransMeter(adMaxClamp);
            wData = TransMeter(0xffff);
            if(adTransMaxClamp<=wData)
                wDatax1 = adTransMaxClamp;
            else
            wDatax1 = wData;
        }
        VarAdrSetInt(ADPOSI_METERMAXCLAMP2,wDatax1);
    }
    else
    {
        VarAdrSetInt(ADPOSI_METERMAXCLAMP2,VarAdrToUI16(ADDR_MAX_ADCLAMP));
    }
}


/*****************************************
*Object   :开模位置图片索引

*Object   :合模阀图片索引

*Object   :开模阀图片索引
******************************************/

static void ClpImgVal()
{
	static UI16 cPicNo = 0;
	static UI32 wAdClamp = 0;
	UI16 i = 0;
    UI32 value = VarAdrToInt(ADDR_ADPOSI_REAL2);
	UI32 uDevision = VarAdrToInt(ADPOSI_OPEN5) / 30;

	while ((value >= (6 * uDevision * i + 5 * uDevision)) && (i < 4))
	{
		i++;
	}
	if (i != cPicNo)
	{
		if (abs(value - wAdClamp) > 20)
		{
			cPicNo = i;
			wAdClamp = value;
		}
		VarAdrSetInt(CLP_POS_IMG_VAL, cPicNo + 1);
	} 

	if (VarAdrToInt(CLAMP_PO_O_CLS_SOL) == 0)
	{
		VarAdrSetInt(CLP_PO_CLS_SOL_IMG_VAL, ONE_IMG);
	}
	else
	{
		VarAdrSetInt(CLP_PO_CLS_SOL_IMG_VAL, TWO_IMG);
	}

	if (VarAdrToInt(CLAMP_PO_O_OPN_SOL) == 0)
	{
		VarAdrSetInt(CLP_PO_OPN_SOL_IMG_VAL, ONE_IMG);
	} 
	else
	{
		VarAdrSetInt(CLP_PO_OPN_SOL_IMG_VAL, TWO_IMG);
	}
}

/*****************************************
*Object   :开关模动作步背景色
******************************************/
static void ClpActBkColor()
{
    //20200302 动作步变色
    UI16 i;
    UI16 stIndex = 0xFF;
    static UI16 stIndexOld = 0xFF;
    static BOOL init_bkcolor = TRUE;
    UI32 addrstep[MAX_ACT] = {VarAdrToInt(ACT_ACTNAME1),VarAdrToInt(ACT_ACTNAME2),VarAdrToInt(ACT_ACTNAME3),VarAdrToInt(ACT_ACTNAME4)};
    UI32 stGroup[] = {p_PP_OBJ_CLS1PIC,p_PP_OBJ_CLS2PIC,p_PP_OBJ_CLS3PIC,p_PP_OBJ_CLSLPIC,p_PP_OBJ_CLSH1PIC,
                      p_PP_OBJ_CLSH2PIC,p_PP_OBJ_OPN1PIC,p_PP_OBJ_OPN2PIC,p_PP_OBJ_OPN3PIC,p_PP_OBJ_OPN4PIC,p_PP_OBJ_OPN5PIC};

    //20200302 动作步变背景色
    if(init_bkcolor)
    {
        init_bkcolor = FALSE;
        for (i = 0;  i< ARRAY_NUM(stGroup); i++)
        {
            VarAdrSetInt(stGroup[i],COLOR_TRANSPARENT);
        }
    }

    if(TRUE)//20201028 背景色变化做标准
    {
        for(i = 0; i < MAX_ACT; i++)//20210220
        {
            switch(addrstep[i]){
            case 0x20110000:
            case 0x20110100:
            case 0x20111000:
            case 0x20112000:
            case 0x20118100:
                stIndex = 0;    //Clamp Close #1
                break;
            case 0x20111100:
                stIndex = 1;    //Clamp Close #2
                break;
            case 0x20111200:
                stIndex = 2;    //Clamp Close #3
                break;
            case 0x20111300:
                stIndex = 3;    //Clamp Close #4
                break;
            case 0x20120000:    //20230518 chj
            case 0x20121000:
                stIndex = 4;    //Clamp Close #5
                break;
            case 0x20121100:    //20230518 chj //高压二段显示的情况下
            case 0x20121101:
                stIndex = 4;    //Clamp Close #5
                if(VarAdrToInt(d_clamp1_MACHSET_FL_CLSHIPRES2))
                {
                    stIndex = 5;//高压二段显示的情况下
                }
                break;
            case 0x20121200:
                stIndex = 5;    //Clamp Close #6
                break;
            case 0x20130000:
            case 0x20131000:
            case 0x20138100:
                stIndex = 6;    //Clamp Open #1
                break;
            case 0x20131100:
                stIndex = 7;    //Clamp Open #2
                break;
            case 0x20131200:
                stIndex = 8;    //Clamp Open #3
                break;
            case 0x20131300:
                stIndex = 9;    //Clamp Open #4
                if(VarAdrToInt(d_clamp1_MOLDSET_FL_CLSOPEN_MODE) == 3)
                {
                    stIndex = 10;//智能模式下 开模四段五段时开模文本亮
                }
                break;
            case 0x20131400:
            case 0x2013FE00:
            case 0x2013FE01:
                stIndex = 10;    //Clamp Open #5
                break;
            default:
                stIndex = 0xFF;
            }

            if(stIndex != 0xFF)
            {
                break;
            }
        }

        if(stIndex != stIndexOld)//20191231 CHZ
        {
            if((stIndex != 0xFF)&&(stIndexOld != 0xFF)){
                VarAdrSetInt(stGroup[stIndexOld],COLOR_TRANSPARENT);
                VarAdrSetInt(stGroup[stIndex],COLOR_ORANGRED);
            }
            else if(stIndexOld == 0xFF){
                VarAdrSetInt(stGroup[stIndex],COLOR_ORANGRED);
            }
            else if(stIndex == 0xFF){
                VarAdrSetInt(stGroup[stIndexOld],COLOR_TRANSPARENT);
            }
            stIndexOld = stIndex;
        }
    }

    if(VarAdrToInt(p_PP_OBJ_CLS1PIC)==COLOR_ORANGRED || VarAdrToInt(p_PP_OBJ_CLS2PIC)==COLOR_ORANGRED || VarAdrToInt(p_PP_OBJ_CLS3PIC)==COLOR_ORANGRED)
    {
        VarAdrSetInt(p_PP_OBJ_CLSMDPIC,COLOR_ORANGRED);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_CLSMDPIC,COLOR_TRANSPARENT);
    }

    if(VarAdrToInt(p_PP_OBJ_OPN2PIC)==COLOR_ORANGRED || VarAdrToInt(p_PP_OBJ_OPN3PIC)==COLOR_ORANGRED || VarAdrToInt(p_PP_OBJ_OPN4PIC)==COLOR_ORANGRED)
    {
        VarAdrSetInt(p_PP_OBJ_OPNFASTPIC,COLOR_ORANGRED);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_OPNFASTPIC,COLOR_TRANSPARENT);
    }

    if((VarAdrToInt(d_clamp1_MOLDSET_OPN_NUM) == 2) && (VarAdrToInt(p_PP_OBJ_OPN1PIC)==COLOR_ORANGRED
         || VarAdrToInt(p_PP_OBJ_OPN2PIC)==COLOR_ORANGRED || VarAdrToInt(p_PP_OBJ_OPN3PIC)==COLOR_ORANGRED || VarAdrToInt(p_PP_OBJ_OPN4PIC)==COLOR_ORANGRED))
    {
        VarAdrSetInt(p_PP_OBJ_OPN1XPIC,COLOR_ORANGRED);
    }
    else if((VarAdrToInt(d_clamp1_MOLDSET_OPN_NUM) == 3) && (VarAdrToInt(p_PP_OBJ_OPN1PIC)==COLOR_ORANGRED
         || VarAdrToInt(p_PP_OBJ_OPN2PIC)==COLOR_ORANGRED || VarAdrToInt(p_PP_OBJ_OPN3PIC)==COLOR_ORANGRED))
    {
        VarAdrSetInt(p_PP_OBJ_OPN1XPIC,COLOR_ORANGRED);
    }
    else if((VarAdrToInt(d_clamp1_MOLDSET_OPN_NUM) == 4) && (VarAdrToInt(p_PP_OBJ_OPN1PIC)==COLOR_ORANGRED
         || VarAdrToInt(p_PP_OBJ_OPN2PIC)==COLOR_ORANGRED))
    {
        VarAdrSetInt(p_PP_OBJ_OPN1XPIC,COLOR_ORANGRED);
    }
    else if((VarAdrToInt(d_clamp1_MOLDSET_OPN_NUM) == 5) && (VarAdrToInt(p_PP_OBJ_OPN1PIC)==COLOR_ORANGRED))
    {
        VarAdrSetInt(p_PP_OBJ_OPN1XPIC,COLOR_ORANGRED);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_OPN1XPIC,COLOR_TRANSPARENT);
    }
}

/**************************开关模功能界面变量模块-Clp*****************************/

/*****************************************
*Object   :开模联动是否可用

*Object   :关模联动是否可用

*Object   :开模联动位置是否可用

*Object   :联动阀延迟关计时是否可用

*Object   :关模联动位置是否可用
******************************************/
static void LinkClpVal()
{
    if ((VarAdrToInt(SYS_FL_MACH_CODE1) & 0x2000) != 0)
    {
        VarAdrSetInt(CLOSE_LINK_ENABLE, TRUE);
        VarAdrSetInt(OPEN_LINK_ENABLE, TRUE);

        if (VarAdrToInt(FL_OPENJTCORE) != 0)
        {
            VarAdrSetInt(LINK_VLV_DLYOFF_TM_ENABLE, TRUE);
            VarAdrSetInt(OPEN_LINK_POS_ENABLE, TRUE);
        }
        else
        {
            VarAdrSetInt(LINK_VLV_DLYOFF_TM_ENABLE, FALSE);
            VarAdrSetInt(OPEN_LINK_POS_ENABLE, FALSE);
        }

        if (VarAdrToInt(FL_CLOSEJTCORE) != 0)
        {
            VarAdrSetInt(CLOSE_LINK_POS_ENABLE, TRUE);
        }
        else
        {
            VarAdrSetInt(CLOSE_LINK_POS_ENABLE, FALSE);
        }
    }
    else
    {
        if (DataRecvFinish())//20220929 dyl
        {
            if (VarAdrToInt(d_machine1_MOLDSET_FL_OPNEJTCORE) != 0)
                VarSendSaveSetIntByAdr(d_machine1_MOLDSET_FL_OPNEJTCORE,0);
            if (VarAdrToInt(d_machine1_MOLDSET_FL_CLOSJTCORE) != 0)
                VarSendSaveSetIntByAdr(d_machine1_MOLDSET_FL_CLOSJTCORE,0);
            CorePointByLink();//中子动作点跟随特殊中子功能及联动功能变化
        }


        VarAdrSetInt(LINK_VLV_DLYOFF_TM_ENABLE, FALSE);
        VarAdrSetInt(CLOSE_LINK_POS_ENABLE, FALSE);
        VarAdrSetInt(OPEN_LINK_POS_ENABLE, FALSE);
        VarAdrSetInt(CLOSE_LINK_ENABLE, FALSE);
        VarAdrSetInt(OPEN_LINK_ENABLE, FALSE);
    }
}
/*****************************************

*Object   :浮动缸压力是否可用

*Object   :浮动缸速度是否可用

******************************************/
static void FloatCylindVal()
{
	if ((VarAdrToInt(SYS_FL_MACH_CODE48) & 0x2000) != 0)
	{
		VarAdrSetInt(FLOAT_CYLIND_PRS_ENABLE, TRUE);
		VarAdrSetInt(FLOAT_CYLIND_SPD_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(FLOAT_CYLIND_PRS_ENABLE, FALSE);
		VarAdrSetInt(FLOAT_CYLIND_SPD_ENABLE, FALSE);
	}
}



/*****************************************
*Subject  :
*Condition:
*Object   :开模减速时间是否可用
*Result   :
******************************************/

static void OpenRetardTmEnable()
{
	if ((VarAdrToInt(SYS_FL_MACH_CODE49) & 0x1000) != 0)
	{
		VarAdrSetInt(OPEN_RETARD_TM_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(OPEN_RETARD_TM_ENABLE, FALSE);
	}
}


/****************************开关模参数二界面逻辑模块-CLP****************************/

/*****************************************
*Object   :部分斜率是否可用

*Object   :开模四段速度斜率文本
******************************************/
static void RampVal()
{
	if (SYS_SERVO5 && CODE5_8000)
	{
		VarAdrSetInt(RAMP_CHOOSED_ENABLE, FALSE);
		VarAdrSetStr(OPN_4_SPD_RAMP_STR_VAL, GetTextTran(TEXT_FASTSPD));
	}
	else
	{
		VarAdrSetInt(RAMP_CHOOSED_ENABLE, TRUE);
		VarAdrSetStr(OPN_4_SPD_RAMP_STR_VAL, GetTextTran(TEXT_FAST3SPD));
	}
}

/*****************************************
*Subject  :
*Condition:
*Object   :开模差动阀动作时间是否可用
*Result   :
******************************************/

static void OpnDiffVlvActTmEnable()
{
	if (CUSTOMID_BA00)
	{
		VarAdrSetInt(OPN_DIFFVLV_ACT_TM_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(OPN_DIFFVLV_ACT_TM_ENABLE, FALSE);
	}
}

/*****************************************
*Subject  :
*Condition:
*Object   :十字头是否可见
*Result   :
******************************************/
static void PushPosVislble()
{
    if(VarAdrToInt(SWPUSHPOSOPT) && PushtableType())
    {
        VarAdrSetInt(PUSH_POS_VISIBLE, TRUE);
    }
    else
    {
        VarAdrSetInt(PUSH_POS_VISIBLE, FALSE);
    }
}

/****************************开关模参数界面逻辑模块-Clp****************************/


/*****************************************
*Object   :关模高压计时小数位

*Object   :关模高压计时最大值
******************************************/

static void ClsHighPrsKeepTmVal()
{
	if ((VarAdrToInt(SYS_FL_MACH_CODE4) & 0x0400) != 0)
	{
		VarAdrSetInt(CLS_HIGHPRS_KEEP_TM_DOT, 2);
		VarAdrSetInt(CLS_HIGHPRS_KEEP_TM_MAX, 9999);

	}
	else
	{
		VarAdrSetInt(CLS_HIGHPRS_KEEP_TM_DOT, 1);
		VarAdrSetInt(CLS_HIGHPRS_KEEP_TM_MAX, 999);
	}
}
/******************************Clp-END************************************/




/****************************射出界面逻辑模块-Inj****************************/

/*****************************************
*Object   :射出2段是否使用

*Object   :射出3段是否使用

*Object   :射出4段是否使用

*Object   :射出5段是否使用

*Object   :射出6段是否使用

*Object   :射出时间2段是否使用

*Object   :射出时间3段是否使用

*Object   :射出时间4段是否使用

*Object   :射出时间5段是否使用

*Object   :射出时间6段是否使用

*Object   :射出位置2段是否使用

*Object   :射出位置3段是否使用

*Object   :射出位置4段是否使用

*Object   :射出位置5段是否使用

*Object   :射出位置6段是否使用

*Object   :射出位置1段最小值

*Object   :射出位置2段最小值

*Object   :射出位置3段最小值

*Object   :射出位置4段最小值

*Object   :射出位置5段最小值

*Object   :保压2段是否使用

*Object   :保压3段是否使用

*Object   :保压4段是否使用

*Object   :保压5段是否使用

*Object   :射出一段位置是否使用

*Object   :射出一段时间是否使用

*Object   :射出总时间是否可见

*Object   :射出分段实时时间是否可见

*Object   :射出分段实时时间是否可用

*Object   :保压位置是否使用

*Object   :保压速度最大值

*Object   :射出速度最大值

*Object   :射出和保压速度小数位

*Object   :射出和保压时间小数位

******************************************/
static void InjVal()
{
	UI16 i;
	const UI16 inj_num2to6 = 5;
	const UI16 num = inj_num2to6 +2;

	for (i = 2; i < num; ++i)
	{
		if (VarAdrToInt(INJECT_SEC_INJ) >= i)
		{
			VarAdrSetInt(INJ_2_ENABLE + (i - 2), TRUE);
		} 
		else
		{
			VarAdrSetInt(INJ_2_ENABLE + (i - 2), FALSE);
		}
	}

	for (i = 2; i < num; ++i)
	{
        //if ((VarAdrToInt(INJECT_FL_TURNTOHOLD) == 3) && ((VarAdrToInt(SYS_FL_MACH_CODE53) & 0x20) != 0) && (VarAdrToInt(INJECT_SEC_INJ) >= i)) 20200317
        if ((VarAdrToInt(INJECT_FL_TURNTOHOLD) == 5) && (VarAdrToInt(INJECT_SEC_INJ) >= i)) //时间分段通过功能码实现
		{
			VarAdrSetInt(INJ_TM_2_ENABLE + (i - 2), TRUE);
		} 
		else
		{
			VarAdrSetInt(INJ_TM_2_ENABLE + (i - 2), FALSE);
		}
	}

	for (i = 2; i < num; ++i)
	{
        //if (!((VarAdrToInt(INJECT_FL_TURNTOHOLD) == 3) && ((VarAdrToInt(SYS_FL_MACH_CODE53) & 0x20) != 0)) && (VarAdrToInt(INJECT_SEC_INJ) >= i)) 20200317
        if ((VarAdrToInt(INJECT_FL_TURNTOHOLD) != 5) && (VarAdrToInt(INJECT_SEC_INJ) >= i))
		{
			VarAdrSetInt(INJ_POS_2_ENABLE + (i - 2), TRUE);
		} 
		else
		{
			VarAdrSetInt(INJ_POS_2_ENABLE + (i - 2), FALSE);
		}
	}

	for (i = 2; i < num; ++i)
	{
		if (VarAdrToInt(INJECT_SEC_INJ) >= i)
		{
			VarAdrSetInt(INJ_POS_1_MIN + (i - 2), VarAdrToInt(ADPOSI_INJ2END + (i - 2)));
		} 
		else
		{
			VarAdrSetInt(INJ_POS_1_MIN + (i - 2), 0);
		}
	}

	for (i = 2; i < num -1; ++i)
	{
		if (VarAdrToInt(INJECT_SEC_HOLD) >= i)
		{
			VarAdrSetInt(HOLD_2_ENABLE + (i - 2), TRUE);
		} 
		else
		{
			VarAdrSetInt(HOLD_2_ENABLE + (i - 2), FALSE);
		}
	}

    //if ((VarAdrToInt(INJECT_FL_TURNTOHOLD) == 3) && ((VarAdrToInt(SYS_FL_MACH_CODE53) & 0x20) != 0)) 20200317
    if(VarAdrToInt(INJECT_FL_TURNTOHOLD) == 5)
	{
		VarAdrSetInt(INJ_POS_1_ENABLE, FALSE);
		VarAdrSetInt(INJ_TM_1_ENABLE, TRUE);
		VarAdrSetInt(INJ_TOTAL_TM_VISIBLE, FALSE);
        VarAdrSetInt(INJ_SEG_TM_VISIBLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(INJ_POS_1_ENABLE, TRUE);
		VarAdrSetInt(INJ_TM_1_ENABLE, FALSE);
		VarAdrSetInt(INJ_TOTAL_TM_VISIBLE, TRUE);
        VarAdrSetInt(INJ_SEG_TM_VISIBLE, FALSE);
	}

//	if (VarAdrToInt(INJECT_FL_TURNTOHOLD) == 0 || VarAdrToInt(INJECT_FL_TURNTOHOLD) == 4)
//	{
//		VarAdrSetInt(HOLD_POS_ENABLE, TRUE);
//	}
//	else
//	{
//		VarAdrSetInt(HOLD_POS_ENABLE, FALSE);
//	}

    if((VarAdrToInt(SYS_FL_MACH_CODE5) & 0x01) != 0)
    {
        VarAdrSetInt(INJ_STATE_TM_VISIBLE, TRUE);
    }
    else
    {
        VarAdrSetInt(INJ_STATE_TM_VISIBLE, FALSE);
    }

    //20211118 dyl 射出分段实时时间是否可用
    for (i = 2; i < num; ++i)
    {
        if ((VarAdrToInt(SYS_FL_MACH_CODE5) & 0x01) && (VarAdrToInt(INJECT_SEC_INJ) >= i))
        {
            VarAdrSetInt(p_PP_OBJ_INJECT_REALTM_2_ENABLE + (i - 2), TRUE);
        }
        else
        {
            VarAdrSetInt(p_PP_OBJ_INJECT_REALTM_2_ENABLE + (i - 2), FALSE);
        }
    }
    //射出总时间设定值对应直线是否可见
    if (VarAdrToInt(INJ_TOTAL_TM_VISIBLE) != 0)
    {
        if (VarAdrToInt(SYS_FL_MACH_CODE5) & 0x01)//射出分段实时时间可见，射出时间对应直线2显示，直线1隐藏
        {
            VarAdrSetInt(p_PP_OBJ_INJECT_LINE2_VISIBLE, TRUE);
            VarAdrSetInt(p_PP_OBJ_INJECT_LINE1_VISIBLE, FALSE);
        }
        else
        {
            VarAdrSetInt(p_PP_OBJ_INJECT_LINE1_VISIBLE, TRUE);
            VarAdrSetInt(p_PP_OBJ_INJECT_LINE2_VISIBLE, FALSE);
        }
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_INJECT_LINE1_VISIBLE, FALSE);
        VarAdrSetInt(p_PP_OBJ_INJECT_LINE2_VISIBLE, FALSE);
    }

	if ((VarAdrToInt(SYS_FL_MACH_CODE5) & 0x0100) != 0)
	{
        VarAdrSetInt(HOLD_SPD_MAX, VarAdrToInt(HOLE_SPEED_MAX)*10+9);
        VarAdrSetInt(INJ_SPD_MAX, VarAdrToInt(d_autoctrl1_MOLDSET_DAFLOW_INJECTLIMIT)*10+9);
		VarAdrSetInt(INJ_AND_HOLD_SPD_DOT, ONE_DOT);      /*确认一位小数， 20180926 hz*/
	}
	else
	{
		VarAdrSetInt(HOLD_SPD_MAX, VarAdrToInt(HOLE_SPEED_MAX));
        VarAdrSetInt(INJ_SPD_MAX, VarAdrToInt(d_autoctrl1_MOLDSET_DAFLOW_INJECTLIMIT)); //2020031799->
		VarAdrSetInt(INJ_AND_HOLD_SPD_DOT, ZERO_DOT);
	}

	if ((VarAdrToInt(SYS_FL_MACH_CODE1) & 0x0008) != 0)
	{
		VarAdrSetInt(INJ_AND_HOLD_TM_DOT, THREE_DOT);
        VarAdrSetInt(p_PP_OBJ_INJ_AND_HOLD_TM_MAX, 3600000);//20200319
	} 
	else
	{
        VarAdrSetInt(INJ_AND_HOLD_TM_DOT, TWO_DOT);//20200319
        VarAdrSetInt(p_PP_OBJ_INJ_AND_HOLD_TM_MAX, 360000);
	}
}

/*****************************************
*Object   :射出位置图片索引

*Object   :射出阀图片索引

*Object   :座进阀图片索引

*Object   :射出动作步背景色 20200228
******************************************/
static void InjImgVal()
{
	static UI16 cInj = 0;
	static UI32 wAdInject = 0;
	UI16 i = 0;
	UI32 value = VarAdrToInt(ADDR_ADPOSI_REAL1);
	UI32 uDevision = VarAdrToInt(ADDR_MAX_ADINJECT) / 30;

	while ((value >= (7 * uDevision * i + 5 * uDevision)) && (i < 3))
	{
		i++;
	}
	if (i != cInj)
	{
		if (abs(value - wAdInject) > 20)
		{
			cInj = i;
			wAdInject = value;
		}
		VarAdrSetInt(INJ_POS_IMG_VAL, cInj + 1);
	} 

	if (VarAdrToInt(INJECT_PO_O_INJ_SOL) == 0)
	{
		VarAdrSetInt(INJ_PO_SOL_IMG_VAL, ONE_IMG);
	} 
	else
	{
		VarAdrSetInt(INJ_PO_SOL_IMG_VAL, TWO_IMG);
	}

	if (VarAdrToInt(NOZZLE_PO_O_NOZ_FWD) == 0)
	{
		VarAdrSetInt(NOZ_PO_FWD_IMG_VAL, ONE_IMG);
	} 
	else
	{
		VarAdrSetInt(NOZ_PO_FWD_IMG_VAL, TWO_IMG);
	}
}

/*****************************************
*Object   :射出动作步背景色
******************************************/
static void InjActBkColor()
{
    //20200302
    UI16 i;
    UI16 stIndex = 0xFF;
    static UI16 stIndexOld = 0xFF;
    static BOOL init_bkcolor = TRUE;
    UI32 addrstep[MAX_ACT] = {VarAdrToInt(ACT_ACTNAME1),VarAdrToInt(ACT_ACTNAME2),VarAdrToInt(ACT_ACTNAME3),VarAdrToInt(ACT_ACTNAME4)};
    UI32 stGroup[] = {p_PP_OBJ_Inj1,p_PP_OBJ_Inj2,p_PP_OBJ_Inj3,p_PP_OBJ_Inj4,p_PP_OBJ_Inj5,
                      p_PP_OBJ_Inj6,p_PP_OBJ_Hold1,p_PP_OBJ_Hold2,p_PP_OBJ_Hold3,p_PP_OBJ_Hold4,p_PP_OBJ_Hold5};

    //20200302
    if(init_bkcolor)
    {
        init_bkcolor = FALSE;
        for (i = 0;  i< ARRAY_NUM(stGroup); i++)
        {
            VarAdrSetInt(stGroup[i],COLOR_TRANSPARENT);
        }
    }

    if(TRUE)//20201028 背景色变化做标准
    {
        for(i = 0; i < MAX_ACT; i++)//20210220
        {
            switch(addrstep[i]){
            case 0x10110000:
            case 0x10111000:
            case 0x10118000:
                stIndex = 0;    //Inject #1
                break;
            case 0x10111100:
                stIndex = 1;    //Inkect #2
                break;
            case 0x10111200:
                stIndex = 2;    //Inject #3
                break;
            case 0x10111300:
                stIndex = 3;    //Inject #4
                break;
            case 0x10111400:
                stIndex = 4;    //Inject #5
                break;
            case 0x10111500:
                stIndex = 5;    //Inject #6
                break;
            case 0x10111C00:
                stIndex = 6;    //Hold #1
                break;
            case 0x10111D00:
                stIndex = 7;    //Hold #2
                break;
            case 0x10111E00:
                stIndex = 8;    //Hold #3
                break;
            case 0x10111F00:
                stIndex = 9;    //Hold #4
                break;
            case 0x10112000:
                stIndex = 10;   //Hold #5
                break;
            case 0x10118100:
                stIndex = VarAdrToInt(INJECT_SEC_INJ) - 1;  //Inject Last Step
                break;
            default:
                stIndex = 0xFF;
            }

            if(stIndex != 0xFF)
            {
                break;
            }
        }

        if(stIndex != stIndexOld)
        {
            if((stIndex != 0xFF)&&(stIndexOld != 0xFF)){
                VarAdrSetInt(stGroup[stIndexOld],COLOR_TRANSPARENT);
                VarAdrSetInt(stGroup[stIndex],COLOR_ORANGRED);//0x0033ff
            }
            else if(stIndexOld == 0xFF){
                VarAdrSetInt(stGroup[stIndex],COLOR_ORANGRED);
            }
            else if(stIndex == 0xFF){
                VarAdrSetInt(stGroup[stIndexOld],COLOR_TRANSPARENT);
            }
            stIndexOld = stIndex;
        }
    }
}

/*****************************************
*Object   :胶口针阀是否可见（威力士时，标准胶口针阀不可见）
******************************************/
static void NeedleVlvVisible()//20200317
{
//    if(CUSTOMID_7A00) //标准的胶口针阀隐藏
//    {
        VarAdrSetInt(p_PP_OBJ_NEEDLEVLV_VISIBLE,FALSE);
//    }
//    else
//    {
//        VarAdrSetInt(p_PP_OBJ_NEEDLEVLV_VISIBLE,TRUE);
//    }
}

/*****************************************
*Object   :威力士针阀开阀位置是否可用
******************************************/
static void NdVlvPosEnable()//20200317
{
    if(VarAdrToInt(d_inject1_MACHSET_FL_INj_SPC) && VarAdrToInt(d_inject1_MACHSET_FL_INJ_SPC_FUNC)==0)
    {
        VarAdrSetInt(p_PP_OBJ_NEEDLEVLV_ONPOS_ENABLE,VarAdrToInt(p_PP_OBJ_NEEDLEVLV_ONPOS_ENABLE) | 0x1); //20250109 fqh 胶口针阀1针阀开阀位置是否可用（按位判断 0x33140306 的第0位）
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_NEEDLEVLV_ONPOS_ENABLE,VarAdrToInt(p_PP_OBJ_NEEDLEVLV_ONPOS_ENABLE) & 0x2);
    }
}

//20250109 fqh 胶口针阀2针阀开阀位置是否可用（按位判断 0x33140306 的第一位）
static void NdVlv2PosEnable()
{
    if(VarAdrToInt(d_inject1_MACHSET_FL_INj_SPC2) && VarAdrToInt(d_inject1_MACHSET_FL_INJ_SPC_FUNC2)==0)
    {
        VarAdrSetInt(p_PP_OBJ_NEEDLEVLV_ONPOS_ENABLE,VarAdrToInt(p_PP_OBJ_NEEDLEVLV_ONPOS_ENABLE) | 0x2);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_NEEDLEVLV_ONPOS_ENABLE,VarAdrToInt(p_PP_OBJ_NEEDLEVLV_ONPOS_ENABLE) & 0x1);
    }
}

/****************************射出界面功能逻辑模块-Inj****************************/

/*****************************************
*Object   :保压压力或速度是否可用

*Object   :保压压力或速度最大值

*Object   :保压压力或速度单位

*Object   :射出压力或速度是否可用

*Object   :射出压力或速度单位

*Object   :保压压力是否可见

*Object   :保压速度是否可见

*Object   :射出压力是否可见

*Object   :射出速度是否可见
******************************************/

static void TurnToHold()
{
    switch(VarAdrToInt(INJECT_FL_TURNTOHOLD))//20211214 dyl 转保压选择变量显示重写
	{
	case 0:   
		VarAdrSetInt(HOLD_PorS_ENABLE, FALSE);
        VarAdrSetStr(HOLD_PorS_UNIT, "mm");
		VarAdrSetInt(INJ_PorS_ENABLE, FALSE);
		VarAdrSetStr(INJ_PorS_UNIT, "bar");      
        VarAdrSetInt(HOLD_PRS_VISIBLE, FALSE);
		VarAdrSetInt(HOLD_SPD_VISIBLE, FALSE);
        VarAdrSetInt(INJ_PRS_VISIBLE, FALSE);
		VarAdrSetInt(INJ_SPD_VISIBLE, FALSE);
        VarAdrSetInt(HOLD_POS_ENABLE, TRUE);
        VarAdrSetInt(p_PP_OBJ_HOLD_PARAM_VISABLE, TRUE);
		break;
	case 1:
		VarAdrSetInt(HOLD_PorS_ENABLE, TRUE);
		VarAdrSetInt(HOLD_PorS_MAX, VarAdrToInt(OTHER_PRS_MAX));
		VarAdrSetStr(HOLD_PorS_UNIT, "bar");
		VarAdrSetInt(INJ_PorS_ENABLE, TRUE);
		VarAdrSetStr(INJ_PorS_UNIT, "bar");
		VarAdrSetInt(HOLD_PRS_VISIBLE, TRUE);
		VarAdrSetInt(HOLD_SPD_VISIBLE, FALSE);
		VarAdrSetInt(INJ_PRS_VISIBLE, TRUE);
		VarAdrSetInt(INJ_SPD_VISIBLE, FALSE);
        VarAdrSetInt(HOLD_POS_ENABLE, FALSE);
        VarAdrSetInt(p_PP_OBJ_HOLD_PARAM_VISABLE, TRUE);
		break;
	case 2:
		VarAdrSetInt(HOLD_PorS_ENABLE, TRUE);
		VarAdrSetInt(HOLD_PorS_MAX, VarAdrToInt(HOLE_SPEED_MAX));
		VarAdrSetStr(HOLD_PorS_UNIT, "%");
		VarAdrSetInt(INJ_PorS_ENABLE, TRUE);
		VarAdrSetStr(INJ_PorS_UNIT, "%");
		VarAdrSetInt(HOLD_PRS_VISIBLE, FALSE);
		VarAdrSetInt(HOLD_SPD_VISIBLE, TRUE);
		VarAdrSetInt(INJ_PRS_VISIBLE, FALSE);
		VarAdrSetInt(INJ_SPD_VISIBLE, TRUE);
        VarAdrSetInt(HOLD_POS_ENABLE, FALSE);
        VarAdrSetInt(p_PP_OBJ_HOLD_PARAM_VISABLE, TRUE);
		break;
	case 3:
		VarAdrSetInt(HOLD_PorS_ENABLE, FALSE);
		VarAdrSetStr(HOLD_PorS_UNIT, "bar");
		VarAdrSetInt(INJ_PorS_ENABLE, FALSE);
		VarAdrSetStr(INJ_PorS_UNIT, "bar");
        VarAdrSetInt(HOLD_PRS_VISIBLE, FALSE);
		VarAdrSetInt(HOLD_SPD_VISIBLE, FALSE);
        VarAdrSetInt(INJ_PRS_VISIBLE, FALSE);
		VarAdrSetInt(INJ_SPD_VISIBLE, FALSE);
        VarAdrSetInt(HOLD_POS_ENABLE, FALSE);
        VarAdrSetInt(p_PP_OBJ_HOLD_PARAM_VISABLE, FALSE);
		break;
	case 4:
		VarAdrSetInt(HOLD_PorS_ENABLE, FALSE);
        VarAdrSetStr(HOLD_PorS_UNIT, "mm");
		VarAdrSetInt(INJ_PorS_ENABLE, FALSE);
		VarAdrSetStr(INJ_PorS_UNIT, "bar");
        VarAdrSetInt(HOLD_PRS_VISIBLE, FALSE);
		VarAdrSetInt(HOLD_SPD_VISIBLE, FALSE);
        VarAdrSetInt(INJ_PRS_VISIBLE, FALSE);
		VarAdrSetInt(INJ_SPD_VISIBLE, FALSE);
        VarAdrSetInt(HOLD_POS_ENABLE, TRUE);
        VarAdrSetInt(p_PP_OBJ_HOLD_PARAM_VISABLE, TRUE);
		break;
    case 5:                                    //20200317时间分段
        VarAdrSetInt(HOLD_PorS_ENABLE, FALSE);
        VarAdrSetStr(HOLD_PorS_UNIT, "bar");
        VarAdrSetInt(INJ_PorS_ENABLE, FALSE);
        VarAdrSetStr(INJ_PorS_UNIT, "bar");
        VarAdrSetInt(HOLD_PRS_VISIBLE, FALSE);
        VarAdrSetInt(HOLD_SPD_VISIBLE, FALSE);
        VarAdrSetInt(INJ_PRS_VISIBLE, FALSE);
        VarAdrSetInt(INJ_SPD_VISIBLE, FALSE);
        VarAdrSetInt(HOLD_POS_ENABLE, FALSE);
        VarAdrSetInt(p_PP_OBJ_HOLD_PARAM_VISABLE, FALSE);
        break;
	default:
		break;
	}
    if (VarAdrToInt(INJECT_FL_TURNTOHOLD) == 0 || VarAdrToInt(INJECT_FL_TURNTOHOLD) == 4)
    {
        VarAdrSetInt(HOLD_POS_ENABLE, TRUE);
    }
    else
    {
        VarAdrSetInt(HOLD_POS_ENABLE, FALSE);
    }
}
/*****************************************
*Object   :蓄能器油路选择是否可用

*Object   :蓄能器压力传感器是否使用
******************************************/
static void AccVal()
{
	switch(VarAdrToInt(ADDR_INCACC))
	{
	case 0:
		VarAdrSetInt(ACC_HY_CIR_SELT_ENABLE, FALSE);
		VarAdrSetInt(PRS_SENSOR_ENABLE, FALSE);
		break;
	case 1:
		VarAdrSetInt(ACC_HY_CIR_SELT_ENABLE, TRUE);
		VarAdrSetInt(PRS_SENSOR_ENABLE, FALSE);
		break;
	case 2:
		VarAdrSetInt(ACC_HY_CIR_SELT_ENABLE, TRUE);
		VarAdrSetInt(PRS_SENSOR_ENABLE, TRUE);
		break;
	default:
		break;
	}
}

/*****************************************
*Object   :压塑前射出一段计时是否可用

*Object   :压塑是否可见

*Object   :压塑前射出一段计时小数位

*Object   :压塑前射出一段计时最大值
******************************************/
static void ComPrsVal()
{
	if (VarAdrToInt(COM_PRS_FUNC) < 2)
	{
		VarAdrSetInt(INJ_1_TM_BEF_COMPRS_ENABLE, FALSE);
	} 
	else
	{
		VarAdrSetInt(INJ_1_TM_BEF_COMPRS_ENABLE, TRUE);
	}

	if ((VarAdrToInt(SYS_FL_MACH_CODE2) & 0x0004) != 0)
	{
		VarAdrSetInt(COM_PRS_VISIBLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(COM_PRS_VISIBLE, FALSE);
	}

    if ((VarAdrToInt(SYS_FL_MACH_CODE1) & 0x0008) != 0)
    {
        VarAdrSetInt(INJ_1_TM_BEF_COMPRS_DOT, 3);
    }
    else
    {
        VarAdrSetInt(INJ_1_TM_BEF_COMPRS_DOT, 2);
    }

    if(VarAdrToInt(d_inject1_MOLDSET_FL_TURNTOHOLD) == 5)
    {
        if ((VarAdrToInt(SYS_FL_MACH_CODE1) & 0x0008) != 0)
        {
            VarAdrSetInt(INJ_1_TM_BEF_COMPRS_MAX, 65000);//20200319
        }
        else
        {
            VarAdrSetInt(INJ_1_TM_BEF_COMPRS_MAX, 9999);//20200319
        }
    }
    else
    {
        if(VarAdrToInt(d_inject1_ACTPARA_TM_INJ1) > 65000)
        {
            VarAdrSetInt(INJ_1_TM_BEF_COMPRS_MAX, 65000);
        }
        else
        {
            VarAdrSetInt(INJ_1_TM_BEF_COMPRS_MAX, VarAdrToInt(d_inject1_ACTPARA_TM_INJ1));
        }
    }

}

/*****************************************
*Subject  :
*Condition:
*Object   :开环功能是否使用
*Result   :
******************************************/

static void OpenLoopEnable()
{
	if ((VarAdrToInt(SYS_FL_MACH_CODE5) & 0x80) != 0)
	{
		VarAdrSetInt(OPEN_LOOP_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(OPEN_LOOP_ENABLE, FALSE);
	}
}

/*****************************************
*Subject  :
*Condition:
*Object   :漏油功能是否可见
*Result   :
******************************************/
static void OilLeakVisible()
{
    if(CUSTOMID_7A00)
    {
        VarAdrSetInt(p_PP_OBJ_OIL_LEAK_VISABLE,TRUE);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_OIL_LEAK_VISABLE,FALSE);
    }
}

/****************************射出界面功能2逻辑模块-Inj****************************/

/*****************************************
*Object   :排气时射出位置是否可用

*Object   :排气时射出时间是否可用

*Object   :座进前射出距离是否可用

*Object   :座进前射出时间是否可用

*Object   :液压喷嘴是否使用

*Object   :座进前射出方式是否可用

*Object   :排气时射出时间最大值

*Object   :排气时射出时间小数位

*Object   :排气功能是否使用
******************************************/
static void InjFunc2Val()
{
    UI32 inj1TmMax = VarAdrToInt(INJECT_TM_INJ1);
	UI32 inj2TmMax = VarAdrToInt(INJECT_TM_INJ1) + VarAdrToInt(INJECT_TM_INJ2);
	UI32 inj3TmMax = inj2TmMax + VarAdrToInt(INJECT_TM_INJ3);
	UI32 inj4TmMax = inj3TmMax + VarAdrToInt(INJECT_TM_INJ4);
	UI32 inj5TmMax = inj4TmMax + VarAdrToInt(INJECT_TM_INJ5);
	UI32 inj6TmMax = inj5TmMax + VarAdrToInt(INJECT_TM_INJ6);

    if (VarAdrToInt(INJECT_FL_TURNTOHOLD) == 5)//20200319
	{
        UI16 mod =  VarAdrToInt(INJECT_SEC_INJ);
        UI32 injTmMax = 0;
        switch (mod) {
        case 1:
            injTmMax = inj1TmMax;
            break;
        case 2:
            injTmMax = inj2TmMax;
            break;
        case 3:
            injTmMax = inj3TmMax;
            break;
        case 4:
            injTmMax = inj4TmMax;
            break;
        case 5:
            injTmMax = inj5TmMax;
            break;
        case 6:
            injTmMax = inj6TmMax;
            break;
        default:
            break;
        }

        if(injTmMax < 60000)
        {
            VarAdrSetInt(EXHT_INJ_TM_MAX, injTmMax);
        }
        else
        {
            VarAdrSetInt(EXHT_INJ_TM_MAX, 60000);
        }
	} 
	else
	{
        if(inj1TmMax < 60000)
            VarAdrSetInt(EXHT_INJ_TM_MAX, inj1TmMax);
        else
            VarAdrSetInt(EXHT_INJ_TM_MAX, 60000);
	}

	if ((VarAdrToInt(SYS_FL_MACH_CODE1) & 0x08) != 0)
	{
		VarAdrSetInt(EXHT_INJ_TM_DOT, THREE_DOT);
	} 
	else
	{
        VarAdrSetInt(EXHT_INJ_TM_DOT, TWO_DOT);//20200319
	}

	if ((VarAdrToInt(SYS_FL_MACH_CODE3) & 0x100) != 0)
	{
		VarAdrSetInt(EXHT_FUNC_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(EXHT_FUNC_ENABLE, FALSE);
	}

	if (((VarAdrToInt(SYS_FL_MACH_CODE3) & 0x100) != 0) && (VarAdrToInt(EXHAUST_MODE) == 6))
	{
		VarAdrSetInt(EXHT_INJ_POS_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(EXHT_INJ_POS_ENABLE, FALSE);
	}

	if (((VarAdrToInt(SYS_FL_MACH_CODE3) & 0x100) != 0) && (VarAdrToInt(EXHAUST_MODE) == 5))
	{
		VarAdrSetInt(EXHT_INJ_TM_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(EXHT_INJ_TM_ENABLE, FALSE);
	}

	if (((VarAdrToInt(SYS_FL_MACH_CODE4) & 0x4000) != 0) && (VarAdrToInt(AUTO_PURGE) == 2))
	{
		VarAdrSetInt(INJ_DIST_BEF_NOZADV_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(INJ_DIST_BEF_NOZADV_ENABLE, FALSE);
	}

	if (((VarAdrToInt(SYS_FL_MACH_CODE4) & 0x4000) != 0) && (VarAdrToInt(AUTO_PURGE) == 1))
	{
		VarAdrSetInt(INJ_TM_BEF_NOZADV_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(INJ_TM_BEF_NOZADV_ENABLE, FALSE);
	}

    if ((VarAdrToInt(SYS_FL_MACH_CODE3) & 0x0001) != 0)//挡板code4+8000改为code3+1 20250310 jhh
    {
		VarAdrSetInt(HYDRAULIC_NOZZLES_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(HYDRAULIC_NOZZLES_ENABLE, FALSE);
	}

    if ((VarAdrToInt(SYS_FL_MACH_CODE4) & 0x4000) != 0)
    {
        VarAdrSetInt(INJ_MODE_BEF_NOZADV_ENABLE, TRUE);
    } 
    else
    {
        VarAdrSetInt(INJ_MODE_BEF_NOZADV_ENABLE, FALSE);
    }
}

/****************************射出参数界面逻辑模块-INJ****************************/


/*****************************************
*Subject  :
*Condition:
*Object   :手动使用调试压力速度是否使用
*Result   :
******************************************/

static void ManlUseDebugPrsSpdEnable()
{
    if ((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x8400)
	{
		VarAdrSetInt(MANL_USE_DEBUG_PRSSPD_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(MANL_USE_DEBUG_PRSSPD_ENABLE, FALSE);
	}
}

static void ManlUseDebugPrsSpdEnable10V()
{
    if ((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x8400 || (VarAdrToInt(SYS_FL_MACH_CODE50) & 0x20))
	{
		VarAdrSetInt(MANL_USE_DEBUG_PRSSPD_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(MANL_USE_DEBUG_PRSSPD_ENABLE, FALSE);
	}
}

static void InjCurve()
{
	if (VarAdrToInt(INJECT_CUREMODE) == 0)
	{
		VarAdrSetInt(INJ_CURVE_POS_VISIBLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(INJ_CURVE_POS_VISIBLE, FALSE);
	}

	if ((VarAdrToInt(SYS_FL_MACH_CODE1) & 0x08) != 0)
	{
		VarAdrSetInt(INJ_CURVE_TM_DOT, THREE_DOT);
	} 
	else
	{
        VarAdrSetInt(INJ_CURVE_TM_DOT, TWO_DOT);//20200324
	}

}



/****************************Inj-END****************************/




/**********************************储料界面逻辑模块-Chg************************************/

/*****************************************
*Object   :储料背压是否可用

*Object   :储料背压3段是否可用

*Object   :储料3段是否使用

*Object   :储前射退距离是否可用

*Object   :储前射退时间是否可用

*Object   :再次储料是否使用

*Object   :储料背压小数位

*Object   :储料背压最大值
******************************************/
static void ChgVal()
{
	if ((VarAdrToInt(SYS_FL_MACH_CODE2) & 0x2000) != 0)
	{
		VarAdrSetInt(BACK_PRS_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(BACK_PRS_ENABLE, FALSE);
	}

    if (((VarAdrToInt(SYS_FL_MACH_CODE2) & 0x2000) != 0) && (VarAdrToInt(SEC_CHG) == 3))
	{
		VarAdrSetInt(BACK_PRS_3_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(BACK_PRS_3_ENABLE, FALSE);
    }

    //20211110 dyl 储料改为1~3段
    if (VarAdrToInt(SEC_CHG) >= 2)//储料2段是否使用-储料画面使用
    {
        VarAdrSetInt(p_PP_OBJ_CHG_2_ENABLE_SHOW, TRUE);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_CHG_2_ENABLE_SHOW, FALSE);
    }
    if (VarAdrToInt(SEC_CHG) == 3)//储料1/2/3段是否是当前选择段数-DA画面使用
    {
        VarAdrSetInt(CHG_3_ENABLE, TRUE);
        VarAdrSetInt(p_PP_OBJ_CHG_1_ENABLE, FALSE);
        VarAdrSetInt(CHG_2_ENABLE, FALSE);
    }
    else if (VarAdrToInt(SEC_CHG) == 2)
    {
        VarAdrSetInt(CHG_2_ENABLE, TRUE);
        VarAdrSetInt(p_PP_OBJ_CHG_1_ENABLE, FALSE);
        VarAdrSetInt(CHG_3_ENABLE, FALSE);
    }
    else if (VarAdrToInt(SEC_CHG) == 1)
    {
        VarAdrSetInt(p_PP_OBJ_CHG_1_ENABLE, TRUE);
        VarAdrSetInt(CHG_2_ENABLE, FALSE);
        VarAdrSetInt(CHG_3_ENABLE, FALSE);
    }


	if (VarAdrToInt(SUCK_BACK_BEFORE_CHG) == 1)
	{
		VarAdrSetInt(SK_BEF_CHG_DIST_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(SK_BEF_CHG_DIST_ENABLE, FALSE);
	}

	if (VarAdrToInt(SUCK_BACK_BEFORE_CHG) == 2)
	{
		VarAdrSetInt(SK_BEF_CHG_TM_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(SK_BEF_CHG_TM_ENABLE, FALSE);
	}

//	if ((VarAdrToInt(SYS_FL_MACH_CODE54) & 0x80) != 0)
//    {
//		VarAdrSetInt(REMELT_ENABLE, TRUE);
//	}
//	else
//	{
//		VarAdrSetInt(REMELT_ENABLE, FALSE);
//	}
    //20211227 dyl 再次储料
    if(VarAdrToInt(p_PP_MACHSET_REMELT_FUNC) == 0)
    {
        VarAdrSetInt(REMELT_ENABLE, FALSE);
        VarAdrSetInt(p_PP_OBJ_REMELT_POS_ENABLE, FALSE);
    }
    else if (VarAdrToInt(p_PP_MACHSET_REMELT_FUNC) == 1)
    {
        VarAdrSetInt(REMELT_ENABLE, TRUE);
        VarAdrSetInt(p_PP_OBJ_REMELT_POS_ENABLE, FALSE);
    }
    else if (VarAdrToInt(p_PP_MACHSET_REMELT_FUNC) == 2)
    {
        VarAdrSetInt(REMELT_ENABLE, FALSE);
        VarAdrSetInt(p_PP_OBJ_REMELT_POS_ENABLE, TRUE);
    }


	if ((VarAdrToInt(SYS_FL_MACH_CODE5) & 0x1000) != 0)
	{
		VarAdrSetInt(BACK_PRS_DOT, ONE_DOT);
		VarAdrSetInt(BACK_PRS_MAX, VarAdrToInt(SYSTEM_DAPRES_BACKLIMIT) * 10);
	} 
	else
	{
		VarAdrSetInt(BACK_PRS_DOT, ZERO_DOT);
		VarAdrSetInt(BACK_PRS_MAX, VarAdrToInt(SYSTEM_DAPRES_BACKLIMIT));
	}
}

/*****************************************
*Object   :加号是否显示

*Object   :射退距离文本

*Object   :射退终止位置最大值

*Object   :再次储料位置最大值

*Object   :储料3段终止位置最大值

*Object   :储料2段位置最大值
******************************************/
static void ChgWeiYa()
{
    UI16 MAX_adCharge = 0;
    if(CUSTOMID_7A00)
    {
        if(VarAdrToInt(ADDR_MAX_ADINJECT) > 50)
        {
            MAX_adCharge = VarAdrToInt(ADDR_MAX_ADINJECT) - 50;
        }
    }
    else
    {
        MAX_adCharge = VarAdrToInt(ADDR_MAX_ADINJECT);
    }

    if (CUSTOMID_BA00 || (PanelCfgStyle()==G_TYPE) || (PanelCfgStyle()==P_TYPE))
	{
		VarAdrSetInt(PLUS_VISIBLE, TRUE);
		VarAdrSetStr(SK_DIST_STR_VAL, GetTextTran(TEXT_DISTANCE));

        if (VarAdrToInt(SEC_CHG) == 1)//20211110 dyl 储料改为1~3段
        {
            if (MAX_adCharge > VarAdrToInt(d_inject1_ACTPARA_ADPOSI_CHG1END))//20201207
            {
                VarAdrSetInt(SK_END_POS_MAX, MAX_adCharge - VarAdrToInt(d_inject1_ACTPARA_ADPOSI_CHG1END));
            }
            else
            {
                VarAdrSetInt(SK_END_POS_MAX, 0);
            }
        }
        else if (VarAdrToInt(SEC_CHG) == 2)
		{
            if (MAX_adCharge > VarAdrToInt(CHG_2_POS_END))//20201207
			{
                VarAdrSetInt(SK_END_POS_MAX, MAX_adCharge - VarAdrToInt(CHG_2_POS_END));
			} 
			else
			{
				VarAdrSetInt(SK_END_POS_MAX, 0);
			}
		}
		else if (VarAdrToInt(SEC_CHG) == 3)
		{
            if (MAX_adCharge > VarAdrToInt(CHG_3_POS_END))//20201207
			{
                VarAdrSetInt(SK_END_POS_MAX, MAX_adCharge - VarAdrToInt(CHG_3_POS_END));
			} 
			else
			{
				VarAdrSetInt(SK_END_POS_MAX, 0);
			}
		}

        if (VarAdrToInt(SEC_CHG) == 1)//20211110 dyl 储料改为1~3段
        {
            VarAdrSetInt(REMELT_POS_MAX, VarAdrToInt(d_inject1_ACTPARA_ADPOSI_CHG1END) + VarAdrToInt(ADPOSI_SUCKBACKDISTANCE));
        }
        else if (VarAdrToInt(SEC_CHG) == 2)
		{
			VarAdrSetInt(REMELT_POS_MAX, VarAdrToInt(CHG_2_POS_END) + VarAdrToInt(ADPOSI_SUCKBACKDISTANCE));
		} 
		else if (VarAdrToInt(SEC_CHG) == 3)
		{
			VarAdrSetInt(REMELT_POS_MAX, VarAdrToInt(CHG_3_POS_END) + VarAdrToInt(ADPOSI_SUCKBACKDISTANCE));
		}
	} 
	else
	{
		VarAdrSetInt(PLUS_VISIBLE, FALSE);
		VarAdrSetStr(SK_DIST_STR_VAL, GetTextTran(TEXT_ENDPOS));
        VarAdrSetInt(REMELT_POS_MAX, VarAdrToInt(ADDR_SUCKBACKEND));
		VarAdrSetInt(SK_END_POS_MAX, VarAdrToInt(ADDR_MAX_ADINJECT));
    }

    //20211110 dyl 储料改为1~3段
    if (VarAdrToInt(SEC_CHG) == 1)//一段
    {
        if (CUSTOMID_BA00)
        {
            VarAdrSetInt(p_PP_OBJ_CHG_1_POS_MAX, VarAdrToInt(ADDR_MAX_ADINJECT) - VarAdrToInt(ADPOSI_SUCKBACKDISTANCE));
        }
        else
        {
            VarAdrSetInt(p_PP_OBJ_CHG_1_POS_MAX, MAX_adCharge);
        }
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_CHG_1_POS_MAX, VarAdrToInt(CHG_2_POS_END));
    }

    if (VarAdrToInt(SEC_CHG) == 2)//二段
    {
        if (CUSTOMID_BA00)
        {
            VarAdrSetInt(CHG_2_POS_MAX, VarAdrToInt(ADDR_MAX_ADINJECT) - VarAdrToInt(ADPOSI_SUCKBACKDISTANCE));
        }
        else
        {
            VarAdrSetInt(CHG_2_POS_MAX, MAX_adCharge);//20201207
        }
    }
    else if (VarAdrToInt(SEC_CHG) > 2)
    {
        VarAdrSetInt(CHG_2_POS_MAX, VarAdrToInt(CHG_3_POS_END));
    }

    if (VarAdrToInt(SEC_CHG) == 3)//三段
    {
        if (CUSTOMID_BA00)
        {
            VarAdrSetInt(CHG_3_POS_MAX, VarAdrToInt(ADDR_MAX_ADINJECT)- VarAdrToInt(ADPOSI_SUCKBACKDISTANCE));
        }
        else
        {
            VarAdrSetInt(CHG_3_POS_MAX, MAX_adCharge);//20201207
        }
    }
}

/*****************************************
*Object   :储料阀图片索引

*Object   :射退阀图片索引

******************************************/
static void ChgImgVal()
{
	VarAdrSetInt(CHG_PO_SOL_IMG_VAL, VarAdrToInt(INJECT_PO_O_CHR_SOL) + 1);
	VarAdrSetInt(SK_PO_SOL_IMG_VAL, VarAdrToInt(INJECT_PO_O_SUC_SOL) + 1);
}

/*****************************************
*Object   :储料动作步背景色
******************************************/
static void ChgActBkColor()
{
    //20200302
    int i=0;
    UI16 stIndex = 0xFF;
    static UI16 stIndexOld = 0xFF;
    static BOOL init_bkcolor = TRUE;
    UI32 addrstep[MAX_ACT] = {VarAdrToInt(ACT_ACTNAME1),VarAdrToInt(ACT_ACTNAME2),VarAdrToInt(ACT_ACTNAME3),VarAdrToInt(ACT_ACTNAME4)};
    UI32 stGroup[] = {p_PP_OBJ_Charge1,p_PP_OBJ_Charge2,p_PP_OBJ_Charge3,p_PP_OBJ_Charge4,p_PP_OBJ_SuckBack};

    //20200302
    if(init_bkcolor)
    {
        init_bkcolor = FALSE;
        for (i = 0;  i< ARRAY_NUM(stGroup); i++)
        {
            VarAdrSetInt(stGroup[i],COLOR_TRANSPARENT);
        }
    }

    if(TRUE)//20201028 背景色变化做标准
    {
        for(i = 0; i < MAX_ACT; i++)//20210220
        {
            switch(addrstep[i]){//20190806 CHZ
            case 0x10120000:
            case 0x10121000:
                stIndex = 0;    //Charge #1
                break;
            case 0x10121100:
                stIndex = 1;    //Charge #2
                break;
            case 0x10121200:
                stIndex = 2;    //Charge #3
                break;
            case 0x10121300:
                stIndex = 3;    //Charge #4
                break;
            case 0x10130000:
            case 0x10131000:
            case 0x1013FE00:
                stIndex = 4;    //Suckback
                break;
            default:
                stIndex = 0xFF;
            }

            if(stIndex != 0xFF)
            {
                break;
            }
        }

        if(stIndex != stIndexOld)
        {
            if((stIndex != 0xFF)&&(stIndexOld != 0xFF)){
                VarAdrSetInt(stGroup[stIndexOld],COLOR_TRANSPARENT);
                VarAdrSetInt(stGroup[stIndex],COLOR_ORANGRED);//0x0033ff
            }
            else if(stIndexOld == 0xFF){
                VarAdrSetInt(stGroup[stIndex],COLOR_ORANGRED);
            }
            else if(stIndex == 0xFF){
                VarAdrSetInt(stGroup[stIndexOld],COLOR_TRANSPARENT);
            }
            stIndexOld = stIndex;
        }
    }
}

/****************************储料功能界面逻辑模块-Chg****************************/

/*****************************************
*Object   :BMC是否可见

*Object   :储料开环是否可用

*Object   :储料开模联动是否可用
******************************************/
static void ChgFuncVal()
{
	if ((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x02) != 0)
	{
		VarAdrSetInt(BMC_VISIBLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(BMC_VISIBLE, FALSE);
	}

	if ((VarAdrToInt(SYS_FL_MACH_CODE5) & 0x80) != 0)
	{
		VarAdrSetInt(CHG_OPEN_LOOP_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(CHG_OPEN_LOOP_ENABLE, FALSE);
	}

	if ((VarAdrToInt(SYS_FL_MACH_CODE1) & 0x100) != 0)
	{
		VarAdrSetInt(CHG_OPEN_LINK_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(CHG_OPEN_LINK_ENABLE, FALSE);
	}
}

/*****************************************
*Object   :自动清料走独立的压力流量是否可见
******************************************/
static void AutoPurgeAloneEnable()
{
    if((VarAdrToInt(SYS_FL_MACH_CODE2) &0x4000) !=0)
    {
        VarAdrSetInt(p_PP_OBJ_PURGE_ALONE_ENABLE,TRUE);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_PURGE_ALONE_ENABLE,FALSE);
    }
}

/*****************************************
*Object   :自动清料时间最大值和小数位切换
******************************************/
static void AutoPurgeVal()
{
    if((VarAdrToInt(SYS_FL_MACH_CODE52) & 0x1) && CUSTOMID_8B00)
    {
        VarAdrSetInt(p_PP_OBJ_CODE52EQUEL1,TWO_DOT);
        VarAdrSetInt(p_PP_OBJ_CODE52EQUEL1MAX,9999);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_CODE52EQUEL1,ONE_DOT);
        VarAdrSetInt(p_PP_OBJ_CODE52EQUEL1MAX,999);
    }
}

/****************************储料参数界面逻辑模块-CHG****************************/

/*****************************************
*Subject  :
*Condition:
*Object   :合流储料速度限制是否使用
*Result   :
******************************************/

static void ConfluChgSpdMaxEnable()
{
    if ((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x8400)
	{
		VarAdrSetInt(CONFLUCHG_SPDMAX_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(CONFLUCHG_SPDMAX_ENABLE, FALSE);
	}
}

/**********************************Chg-END**********************************************/


/****************************中子界面逻辑模块-CORE****************************/

/*****************************************
*Object   :计数时间A-F是否可见

*Object   :行程时间A-F是否可见
******************************************/
static void CountOrTravel()
{
	UI16 i;
	const UI16 core_numAtoF = 6;


	for (i = 0; i < core_numAtoF; ++i)
	{
		if (VarAdrToInt(COREA_FL_CORE_CTRL + i * 0x00100000) == 2)
		{
			VarAdrSetInt(COUNT_TIME_A_VISIBLE + i, TRUE);
			VarAdrSetInt(TRAVEL_TIME_A_VISIBLE + i, FALSE);
		} 
		else
		{
			VarAdrSetInt(COUNT_TIME_A_VISIBLE + i, FALSE);
			VarAdrSetInt(TRAVEL_TIME_A_VISIBLE + i, TRUE);
		}
	}

    if(PanelCfgStyle() == G_TYPE)
    {
        VarAdrSetInt(COUNT_TIME_VISIBLE,VarAdrToInt(COUNT_TIME_A_VISIBLE+VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2));
        VarAdrSetInt(TRAVEL_TIME_VISIBLE,VarAdrToInt(TRAVEL_TIME_A_VISIBLE+VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2));

        VarAdrSetInt(p_PP_OBJ_COUNT_TIME2_VISABLE,VarAdrToInt(COUNT_TIME_A_VISIBLE+VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2+1));
        VarAdrSetInt(p_PP_OBJ_TRAVEL_TIME2_VISIBLE,VarAdrToInt(TRAVEL_TIME_A_VISIBLE+VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2+1));
    }

}

/*****************************************
*Object   :动作位置A-F-IN是否可用

*Object   :动作位置A-F-OUT是否可用
******************************************/
static void ActInOrOut()
{
	UI16 i;
	const UI16 core_numAtoF = 6;


	for (i = 0; i < core_numAtoF; ++i)
	{
		if (!(CODE1_2000 && (VarAdrToInt(FL_CLOSEJTCORE) == 1 + i)))
		{
			VarAdrSetInt(ACT_POS_A_IN_ENABLE + i, TRUE);
		} 
		else
		{
			VarAdrSetInt(ACT_POS_A_IN_ENABLE + i, FALSE);
		}

		if (!(CODE1_2000 && (VarAdrToInt(FL_OPENJTCORE) == 2 + i)))
		{
			VarAdrSetInt(ACT_POS_A_OUT_ENABLE + i, TRUE);
		} 
		else
		{
			VarAdrSetInt(ACT_POS_A_OUT_ENABLE + i, FALSE);
		}
	}

    if(PanelCfgStyle() == G_TYPE)
    {
//        VarAdrSetInt(ACT_POS_IN_ENABLE,VarAdrToInt(ACT_POS_A_IN_ENABLE+VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2)
//                     & (VarAdrToInt(d_coreA_MOLDSET_ADPOSI_CORE_FWD+0x00100000*VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2) == 1));
//        VarAdrSetInt(ACT_POS_OUT_ENABLE,VarAdrToInt(ACT_POS_A_OUT_ENABLE+VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2)
//                     & (VarAdrToInt(d_coreA_MOLDSET_ADPOSI_CORE_BWD+0x00100000*VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2) == 1));

//        VarAdrSetInt(p_PP_OBJ_ACT_POS_IN2_ENABLE,VarAdrToInt(ACT_POS_A_IN_ENABLE+VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2+1)
//                     & (VarAdrToInt(d_coreA_MOLDSET_ADPOSI_CORE_FWD+0x00100000*VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2+0x00100000) == 1));
//        VarAdrSetInt(p_PP_OBJ_ACT_POS_OUT2_ENABLE,VarAdrToInt(ACT_POS_A_OUT_ENABLE+VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2+1)
//                     & (VarAdrToInt(d_coreA_MOLDSET_ADPOSI_CORE_BWD+0x00100000*VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2+0x00100000) == 1));

        //20211221 dyl
        if (VarAdrToInt(ACT_POS_A_IN_ENABLE+VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2))
        {
            if(VarAdrToInt(d_coreA_MOLDSET_ADPOSI_CORE_FWD+0x00100000*VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2) == 1)
                VarAdrSetInt(ACT_POS_IN_ENABLE,TRUE);
            else
                VarAdrSetInt(ACT_POS_IN_ENABLE,FALSE);
            VarAdrSetInt(p_PP_OBJ_COREIN1_POS_READONLY, FALSE);
            VarAdrSetInt(p_PP_OBJ_COREIN1_POS_BKCOLOR, 0xFFFFFF);
        }
        else
        {
            if(VarAdrToInt(d_coreA_MOLDSET_ADPOSI_CORE_FWD+0x00100000*VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2) == 1)
            {
                VarAdrSetInt(ACT_POS_IN_ENABLE,TRUE);
                VarAdrSetInt(p_PP_OBJ_COREIN1_POS_READONLY, TRUE);
                VarAdrSetInt(p_PP_OBJ_COREIN1_POS_BKCOLOR, 0xdedede);
            }
            else
            {
                VarAdrSetInt(ACT_POS_IN_ENABLE,FALSE);
                VarAdrSetInt(p_PP_OBJ_COREIN1_POS_READONLY, FALSE);
                VarAdrSetInt(p_PP_OBJ_COREIN1_POS_BKCOLOR, 0xFFFFFF);
            }
        }
        if (VarAdrToInt(ACT_POS_A_OUT_ENABLE+VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2))
        {
            if (VarAdrToInt(d_coreA_MOLDSET_ADPOSI_CORE_BWD+0x00100000*VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2) == 1)
                VarAdrSetInt(ACT_POS_OUT_ENABLE,TRUE);
            else
                VarAdrSetInt(ACT_POS_OUT_ENABLE,FALSE);
            VarAdrSetInt(p_PP_OBJ_COREOUT1_POS_READONLY, FALSE);
            VarAdrSetInt(p_PP_OBJ_COREOUT1_POS_BKCOLOR, 0xFFFFFF);
        }
        else
        {
            if(VarAdrToInt(d_coreA_MOLDSET_ADPOSI_CORE_BWD+0x00100000*VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2) == 1)
            {
                VarAdrSetInt(ACT_POS_OUT_ENABLE,TRUE);
                VarAdrSetInt(p_PP_OBJ_COREOUT1_POS_READONLY, TRUE);
                VarAdrSetInt(p_PP_OBJ_COREOUT1_POS_BKCOLOR, 0xdedede);
            }
            else
            {
                VarAdrSetInt(ACT_POS_OUT_ENABLE,FALSE);
                VarAdrSetInt(p_PP_OBJ_COREOUT1_POS_READONLY, FALSE);
                VarAdrSetInt(p_PP_OBJ_COREOUT1_POS_BKCOLOR, 0xFFFFFF);
            }
        }
        if (VarAdrToInt(ACT_POS_A_IN_ENABLE+VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2+1))
        {
            if(VarAdrToInt(d_coreA_MOLDSET_ADPOSI_CORE_FWD+0x00100000*VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2+0x00100000) == 1)
                VarAdrSetInt(p_PP_OBJ_ACT_POS_IN2_ENABLE,TRUE);
            else
                VarAdrSetInt(p_PP_OBJ_ACT_POS_IN2_ENABLE,FALSE);
            VarAdrSetInt(p_PP_OBJ_COREIN2_POS_READONLY, FALSE);
            VarAdrSetInt(p_PP_OBJ_COREIN2_POS_BKCOLOR, 0xFFFFFF);
        }
        else
        {
            if(VarAdrToInt(d_coreA_MOLDSET_ADPOSI_CORE_FWD+0x00100000*VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2+0x00100000) == 1)
            {
                VarAdrSetInt(p_PP_OBJ_ACT_POS_IN2_ENABLE,TRUE);
                VarAdrSetInt(p_PP_OBJ_COREIN2_POS_READONLY, TRUE);
                VarAdrSetInt(p_PP_OBJ_COREIN2_POS_BKCOLOR, 0xdedede);
            }
            else
            {
                VarAdrSetInt(p_PP_OBJ_ACT_POS_IN2_ENABLE,FALSE);
                VarAdrSetInt(p_PP_OBJ_COREIN2_POS_READONLY, FALSE);
                VarAdrSetInt(p_PP_OBJ_COREIN2_POS_BKCOLOR, 0xFFFFFF);
            }
        }
        if (VarAdrToInt(ACT_POS_A_OUT_ENABLE+VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2+1))
        {
            if(VarAdrToInt(d_coreA_MOLDSET_ADPOSI_CORE_BWD+0x00100000*VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2+0x00100000) == 1)
                VarAdrSetInt(p_PP_OBJ_ACT_POS_OUT2_ENABLE,TRUE);
            else
                VarAdrSetInt(p_PP_OBJ_ACT_POS_OUT2_ENABLE,FALSE);
            VarAdrSetInt(p_PP_OBJ_COREOUT2_POS_READONLY, FALSE);
            VarAdrSetInt(p_PP_OBJ_COREOUT2_POS_BKCOLOR, 0xFFFFFF);
        }
        else
        {
            if(VarAdrToInt(d_coreA_MOLDSET_ADPOSI_CORE_BWD+0x00100000*VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2+0x00100000) == 1)
            {
                VarAdrSetInt(p_PP_OBJ_ACT_POS_OUT2_ENABLE,TRUE);
                VarAdrSetInt(p_PP_OBJ_COREOUT2_POS_READONLY, TRUE);
                VarAdrSetInt(p_PP_OBJ_COREOUT2_POS_BKCOLOR, 0xdedede);
            }
            else
            {
                VarAdrSetInt(p_PP_OBJ_ACT_POS_OUT2_ENABLE,FALSE);
                VarAdrSetInt(p_PP_OBJ_COREOUT2_POS_READONLY, FALSE);
                VarAdrSetInt(p_PP_OBJ_COREOUT2_POS_BKCOLOR, 0xFFFFFF);
            }
        }

        VarAdrSetInt(p_PP_OBJ_ACT_POINT_IN_ENABLE,VarAdrToInt(ACT_POS_A_IN_ENABLE+VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2)
                     | (VarAdrToInt(d_coreA_MOLDSET_ADPOSI_CORE_FWD+0x00100000*VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2) != 1));
        VarAdrSetInt(p_PP_OBJ_ACT_POINT_OUT_ENABLE,VarAdrToInt(ACT_POS_A_OUT_ENABLE+VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2)
                     | (VarAdrToInt(d_coreA_MOLDSET_ADPOSI_CORE_BWD+0x00100000*VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2) != 1));
        VarAdrSetInt(p_PP_OBJ_ACT_POINT_IN_ENABLE2,VarAdrToInt(ACT_POS_A_IN_ENABLE+VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2+1)
                     | (VarAdrToInt(d_coreA_MOLDSET_ADPOSI_CORE_FWD+0x00100000*VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2+0x00100000) != 1));
        VarAdrSetInt(p_PP_OBJ_ACT_POINT_OUT_ENABLE2,VarAdrToInt(ACT_POS_A_OUT_ENABLE+VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2+1)
                     | (VarAdrToInt(d_coreA_MOLDSET_ADPOSI_CORE_BWD+0x00100000*VarAdrToInt(p_PP_MACHSET_CORE_SELECT)*2+0x00100000) != 1));
        if (VarAdrToInt(p_PP_MACHSET_CORE_SELECT) == 0)
        {
            if(VarAdrToInt(COREA_FL_SPECIALCODE) == 1)
            {
                VarAdrSetInt(p_PP_OBJ_ACT_POINT_IN_ENABLE, FALSE);
                VarAdrSetInt(p_PP_OBJ_ACT_POINT_OUT_ENABLE, FALSE);
            }
            else if (VarAdrToInt(COREA_FL_SPECIALCODE) == 2)
            {
                VarAdrSetInt(p_PP_OBJ_ACT_POINT_IN_ENABLE, FALSE);
            }
            else if (VarAdrToInt(COREA_FL_SPECIALCODE) == 3)
            {
                VarAdrSetInt(p_PP_OBJ_ACT_POINT_OUT_ENABLE, FALSE);
            }
            else if (VarAdrToInt(COREA_FL_SPECIALCODE) == 4)
            {
                VarAdrSetInt(p_PP_OBJ_ACT_POINT_IN_ENABLE, FALSE);
                VarAdrSetInt(p_PP_OBJ_ACT_POINT_OUT_ENABLE, FALSE);
            }
            else if (VarAdrToInt(COREA_FL_SPECIALCODE) == 5)
            {
                VarAdrSetInt(p_PP_OBJ_ACT_POINT_IN_ENABLE, FALSE);
                VarAdrSetInt(p_PP_OBJ_ACT_POINT_OUT_ENABLE, FALSE);
            }
            else if (VarAdrToInt(COREA_FL_SPECIALCODE) == 6)
            {
                VarAdrSetInt(p_PP_OBJ_ACT_POINT_OUT_ENABLE, FALSE);
            }
            else if (VarAdrToInt(COREA_FL_SPECIALCODE) == 7)
            {
                VarAdrSetInt(p_PP_OBJ_ACT_POINT_IN_ENABLE, FALSE);
                VarAdrSetInt(p_PP_OBJ_ACT_POINT_OUT_ENABLE, FALSE);
            }
        }

    }

}

/*****************************************
*Object   :中子动作步背景色
******************************************/
static void CoreActBkColor()
{
    //20200302
    UI16 i;
    UI16 stIndex = 0xFF;
    static UI16 stIndexOld = 0xFF;
    static BOOL init_bkcolor = TRUE;
    UI32 addrstep[MAX_ACT] = {VarAdrToInt(ACT_ACTNAME1),VarAdrToInt(ACT_ACTNAME2),VarAdrToInt(ACT_ACTNAME3),VarAdrToInt(ACT_ACTNAME4)};
    UI32 stGroup[] = {p_PP_OBJ_CoreAIn,p_PP_OBJ_CoreAOut,p_PP_OBJ_CoreBIn,p_PP_OBJ_CoreBOut
                      ,p_PP_OBJ_CoreCIn,p_PP_OBJ_CoreCOut,p_PP_OBJ_CoreDIn,p_PP_OBJ_CoreDOut,
                      p_PP_OBJ_CoreEIn,p_PP_OBJ_CoreEOut,p_PP_OBJ_CoreFIn,p_PP_OBJ_CoreFOut};


    if(init_bkcolor)
    {
        init_bkcolor = FALSE;
        for (i = 0;  i< ARRAY_NUM(stGroup); i++)
        {
            VarAdrSetInt(stGroup[i],COLOR_TRANSPARENT);
        }
    }

    if(TRUE)//20201028 背景色变化做标准
    {
        for(i = 0; i < MAX_ACT; i++)//20210220
        {
            switch(addrstep[i]){
            case 0x22110100:
            case 0x22111000:
            case 0x22112000:
            case 0x22113000:
            case 0x22114000:
                stIndex = 0;    //Core A In
                break;
            case 0x22120100:
            case 0x22121000:
            case 0x2212FE00:
                stIndex = 1;    //Core A Out
                break;
            case 0x22210100:
            case 0x22211000:
                stIndex = 2;    //Core B In
                break;
            case 0x22221000:
            case 0x22220100:
            case 0x2222FE00:
                stIndex = 3;    //Core B Out
                break;
            case 0x22310100:
            case 0x22311000:
                stIndex = 4;    //Core C In
                break;
            case 0x22321000:
            case 0x22320100:
            case 0x2232FE00:
                stIndex = 5;    //Core C Out
                break;
            case 0x22410100:
            case 0x22411000:
                stIndex = 6;    //Core D In
                break;
            case 0x22421000:
            case 0x22420100:
            case 0x2242FE00:
            case 0x22441000:
                stIndex = 7;    //Core D Out
                break;
            case 0x22510100:
            case 0x22511000:
            case 0x22531000:
                stIndex = 8;    //Core E In
                break;
            case 0x22521000:
            case 0x22520100:
            case 0x2252FE00:
            case 0x22541000:
                stIndex = 9;    //Core E Out
                break;
            case 0x22610100:
            case 0x22611000:
            case 0x22631000:
                stIndex = 10;    //Core F In
                break;
            case 0x22621000:
            case 0x22620100:
            case 0x2262FE00:
            case 0x22641000:
                stIndex = 11;    //Core F Out
                break;
            default:
                stIndex = 0xFF;
            }

            if(stIndex != 0xFF)
            {
                break;
            }
        }

        if(stIndex != stIndexOld)
        {

            if((stIndex != 0xFF)&&(stIndexOld != 0xFF)){
                VarAdrSetInt(stGroup[stIndexOld],COLOR_TRANSPARENT);
                VarAdrSetInt(stGroup[stIndex],COLOR_ORANGRED);//0033FF
            }
            else if(stIndexOld == 0xFF){
                VarAdrSetInt(stGroup[stIndex],COLOR_ORANGRED);
            }
            else if(stIndex == 0xFF){
                VarAdrSetInt(stGroup[stIndexOld],COLOR_TRANSPARENT);
            }
            stIndexOld = stIndex;
        }
    }
}

#define CORE_GROUPNUM 3
#define CORE_POINTNUM 4
static void CorePointSignalBkColor()//20211221 dyl 中子点位信号背景色
{
    UI16 i,j;
    UI16 stIndex = 0xFF;
    static BOOL init_bkcolor = TRUE;
    UI32 FirstAddr_pointsignal[CORE_GROUPNUM] = {d_coreA_STATE_COREFWD_STATUS,d_coreC_STATE_COREFWD_STATUS,d_coreE_STATE_COREFWD_STATUS};
    UI16 st_PointValue[CORE_POINTNUM]={0xFF,0xFF,0xFF,0xFF};
    static UI16 st_PointValue_Old[CORE_POINTNUM]={0xFF,0xFF,0xFF,0xFF};
    UI32 stGroup[CORE_POINTNUM] = {p_PP_OBJ_COREACE_POINT_IN,p_PP_OBJ_COREACE_POINT_OUT,p_PP_OBJ_COREBDF_POINT_IN,p_PP_OBJ_COREBDF_POINT_OUT};


    if(init_bkcolor)
    {
        init_bkcolor = FALSE;
        for (i = 0;  i< CORE_POINTNUM; i++)
        {
            VarAdrSetInt(stGroup[i],COLOR_GRAY);
        }
    }

    if(TRUE)
    {
        stIndex = VarAdrToInt(p_PP_MACHSET_CORE_SELECT);
        st_PointValue[0] = VarAdrToInt(FirstAddr_pointsignal[stIndex]);
        st_PointValue[1] = VarAdrToInt(FirstAddr_pointsignal[stIndex]+1);
        st_PointValue[2] = VarAdrToInt(FirstAddr_pointsignal[stIndex]+0x100000);
        st_PointValue[3] = VarAdrToInt(FirstAddr_pointsignal[stIndex]+0x100000+1);
        for (j=0;j<CORE_POINTNUM;j++)
        {
            if(st_PointValue[j] != st_PointValue_Old[j])
            {
                if(st_PointValue[j] != 0)
                {
                    VarAdrSetInt(stGroup[j],COLOR_RED);
                }
                else
                {
                    VarAdrSetInt(stGroup[j],COLOR_GRAY);
                }
                st_PointValue_Old[j] = st_PointValue[j];
            }
        }
    }
}
/****************************中子功能界面逻辑模块-CORE****************************/

/*****************************************
*Subject  :
*Condition:
*Object   :特殊中子1-6功能是否使用
*Result   :
******************************************/

static void SpecialCoreFuncVISIBLE()
{
	UI16 i;
    const UI16 core_num = 6;

	for (i = 0; i < core_num; ++i)
	{
		if (VarAdrToInt(COREA_FL_SPECIALCODE) == i + 1)
		{
			VarAdrSetInt(SPECIAL_CORE_1_FUNC_VISIBLE + i, TRUE);
		} 
		else
		{
			VarAdrSetInt(SPECIAL_CORE_1_FUNC_VISIBLE + i, FALSE);
		}
	}

    if (VarAdrToInt(COREA_FL_SPECIALCODE) == 7)
    {
        VarAdrSetInt(SPECIAL_CORE_7_FUNC_VISIBLE, TRUE);
    }
    else
    {
        VarAdrSetInt(SPECIAL_CORE_7_FUNC_VISIBLE, FALSE);
    }

	if (VarAdrToInt(COREA_FL_SPECIALCODE))
	{
		VarAdrSetInt(SPECIAL_CORE_FUNC_VISIBLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(SPECIAL_CORE_FUNC_VISIBLE, FALSE);
	}
}

/*****************************************
*Subject  :
*Condition:
*Object   :途中中子退下限位置最大值
*Result   :
******************************************/

static void WayCoreOutMinPosMax()
{
	if (VarAdrToInt(CLAMP_MACHSET_EFFCPR) < 10)
	{
		VarAdrSetInt(WAY_CORE_OUT_MIN_POS_MAX, VarAdrToInt(ADPOSI_OPEN5));
	} 
	else
	{
        if((VarAdrToInt(ADPOSI_OPEN5) + 10) < VarAdrToInt(CLAMP_MACHSET_EFFCPR))
        {
            VarAdrSetInt(WAY_CORE_OUT_MIN_POS_MAX, VarAdrToInt(ADPOSI_OPEN5));
        }
        else
        {
            VarAdrSetInt(WAY_CORE_OUT_MIN_POS_MAX, VarAdrToInt(ADPOSI_OPEN5) + 10 - VarAdrToInt(CLAMP_MACHSET_EFFCPR));
        }
	}
}

/****************************中子参数界面逻辑模块-CORE****************************/

/*****************************************
*Subject  :
*Condition:
*Object   :中子选择是否使用
*Result   :
******************************************/

static void CoreSelectEnable()
{
	if ((VarAdrToInt(SYSTEM_MACH_CODE44) & 0x20) != 0)
	{
		VarAdrSetInt(CORE_SELECT_ENABLE, FALSE);
	} 
	else
	{
		VarAdrSetInt(CORE_SELECT_ENABLE, TRUE);
	}
}

/****************************温度界面变量模块-TEMP****************************/

/*****************************************
*Subject  :
*Condition:
*Object   :手动温度是否可用
*Result   :
******************************************/

static void ManualTempEnable()
{
	if (VarAdrToInt(ADDR_TEMP_RAMP1) == 0)
	{
		VarAdrSetInt(MANUAL_TEMP_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(MANUAL_TEMP_ENABLE, FALSE);
	}
}

/*****************************************
*Subject  :
*Condition:
*Object   :温度设定值最大值
*Result   :
******************************************/

static void TempSetValMax()
{
    if ((VarAdrToInt(d_machine1_MACHSET_FL_SERVOMODE) & 0x04) != 0)//温度控制+2改为code16+4 20250310 jhh
    {
		VarAdrSetInt(TEMP_SET_VAL_MAX, MAX_TEMP_CODE_TEMP_2);
	} 
	else
	{
		VarAdrSetInt(TEMP_SET_VAL_MAX, MAX_TEMP_DEFAULT);
	}
}

/*****************************************
*Subject  :
*Condition:
*Object   :温度1-14段图片索引
*Result   :
******************************************/

static void TempImgVal()
{
	UI16 i;
	const UI16 temp_num = 6;
	const UI16 temp8to14_num = 7;

	for (i = 0; i < temp_num; ++i)
	{
		if (VarAdrToInt(ADDR_TEMP_RAMP1 + i) == 0)
		{
			VarAdrSetInt(TEMP_1_IMG_VAL + i, ONE_IMG);
		} 
		else
		{	
			VarAdrSetInt(TEMP_1_IMG_VAL + i, VarAdrToInt(ADDR_TEMP_STATUS1 + i) + TWO_IMG);
		}
	}

	if (VarAdrToInt(SYS_FL_CTRLMODEL) == 0xA920)
	{
		if ((VarAdrToInt(SYS_FL_MACH_CODE4) & 0x0010) == 0)
		{
			if (VarAdrToInt(ADDR_TEMP_RAMP7) != 0)
			{
				VarAdrSetInt(TEMP_7_IMG_VAL, VarAdrToInt(ADDR_TEMP_STATUS7) + TWO_IMG);
			} 
			else
			{
				VarAdrSetInt(TEMP_7_IMG_VAL, ONE_IMG);
			}
		}
	}

	if ((VarAdrToInt(SYS_FL_MACH_CODE54) & 0x0400) != 0)
	{
		for (i = 0; i < temp8to14_num; ++i)
		{
			if (VarAdrToInt(ADDR_TEMP_RAMP8 + i) == 0)
			{
				VarAdrSetInt(TEMP_8_IMG_VAL + i, ONE_IMG);
			} 
			else
			{
				VarAdrSetInt(TEMP_8_IMG_VAL + i, VarAdrToInt(ADDR_TEMP_STATUS8 + i) + TWO_IMG);
			}
		}
	}
}

/*****************************************
*Subject  :
*Condition:
*Object   :温度4-7段是否可见
*Result   :
******************************************/

static void TempVisible()
{
	if (VarAdrToInt(SYS_FL_CTRLMODEL) == 0xA320)
	{
		if ((VarAdrToInt(SYS_FL_MACH_CODE4) & 0x0010) != 0)
		{
			VarAdrSetInt(TEMP_4_VISIBLE, FALSE);
		} 
		else
		{
			VarAdrSetInt(TEMP_4_VISIBLE, TRUE);
		}
		VarAdrSetInt(TEMP_5_VISIBLE, FALSE);
		VarAdrSetInt(TEMP_6_VISIBLE, FALSE);
		VarAdrSetInt(TEMP_7_VISIBLE, FALSE);
	} 
	else if (VarAdrToInt(SYS_FL_CTRLMODEL) ==  0xA620)
	{
		VarAdrSetInt(TEMP_4_VISIBLE, TRUE);
		VarAdrSetInt(TEMP_5_VISIBLE, TRUE);
		if ((VarAdrToInt(SYS_FL_MACH_CODE4) & 0x0010) != 0)
		{
			VarAdrSetInt(TEMP_6_VISIBLE, FALSE);
		} 
		else
		{
			VarAdrSetInt(TEMP_6_VISIBLE, TRUE);
		}
		VarAdrSetInt(TEMP_7_VISIBLE, FALSE);
	}
	else if (VarAdrToInt(SYS_FL_CTRLMODEL) == 0xA920)
	{
		VarAdrSetInt(TEMP_4_VISIBLE, TRUE);
		VarAdrSetInt(TEMP_5_VISIBLE, TRUE);
		VarAdrSetInt(TEMP_6_VISIBLE, TRUE);
		if ((VarAdrToInt(SYS_FL_MACH_CODE4) & 0x0010) != 0)
		{
			VarAdrSetInt(TEMP_7_VISIBLE, FALSE);
		} 
		else
		{
			VarAdrSetInt(TEMP_7_VISIBLE, TRUE);
		}
	}
	else
	{
		VarAdrSetInt(TEMP_4_VISIBLE, TRUE);
		VarAdrSetInt(TEMP_5_VISIBLE, TRUE);
		VarAdrSetInt(TEMP_6_VISIBLE, TRUE);
		VarAdrSetInt(TEMP_7_VISIBLE, TRUE);
	}
}

/*****************************************
*Subject  :
*Condition:
*Object   :温度一段设定是否可用
*Result   :
******************************************/

static void Temp1SetEnable()
{
	static UI32 temp_DACENT_HEATERON = 0;
	static UI32 temp_RESPONSELESSCYCLE = 0;

	if (VarAdrToInt(ADDR_TEMP_RAMP1) == 0)
	{
		VarAdrSetInt(TEMP_1_SET_ENABLE, FALSE);
		if ((temp_DACENT_HEATERON != VarAdrToInt(TEMPCTRL_DACENT_HEATERON)) || (temp_RESPONSELESSCYCLE) != VarAdrToInt(TEMPCTRL_RESPONSELESSCYCLE))
		{
			temp_DACENT_HEATERON = VarAdrToInt(TEMPCTRL_DACENT_HEATERON);
			temp_RESPONSELESSCYCLE = VarAdrToInt(TEMPCTRL_RESPONSELESSCYCLE);
			if ((VarAdrToInt(TEMPCTRL_DACENT_HEATERON) == 0) && (VarAdrToInt(TEMPCTRL_RESPONSELESSCYCLE) == 0))
			{
				VarAdrSetInt(TEMP_1_SET_ENABLE, TRUE);
			}
			else
			{
				VarAdrSetInt(TEMP_1_SET_ENABLE, FALSE);
			}
		}
	} 
	else
	{
		VarAdrSetInt(TEMP_1_SET_ENABLE, TRUE);
	}
}

/*****************************************
*Subject  :
*Condition:
*Object   :温度画面中自动转保温显示与否判断
*Result   :
******************************************/
static void Temp_AutoToWarmVisible()//20211201 dyl 自动转保温
{
    if(VarAdrToUI16(d_TempCtrlComm1_MACHSET_FL_FREETIME_WARMOFF) == 2)
    {
        VarAdrSetInt(p_PP_OBJ_TOKEEPWARM_VISIBLE,1);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_TOKEEPWARM_VISIBLE,0);
    }
}
/*****************************************
*Object   :电热连续加温检查度数

* //20211228 dyl 电热连续加温检查度数
******************************************/
static void HeaterCheckDegreeVisible()
{
    if (VarAdrToInt(d_TempCtrlComm1_MACHSET_FL_TEMPWORDUPCHECK) != 0)
    {
        VarAdrSetInt(p_PP_OBJ_HEATER_WARMCHECK_VISIBLE, TRUE);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_HEATER_WARMCHECK_VISIBLE, FALSE);
    }
}

/****************************温度参数一界面逻辑模块-TEMP****************************/

/*****************************************
*Subject  :
*Condition:
*Object   :额外温度是否使用
*Result   :
******************************************/

static void ExtraTempVisible()
{
	if ((VarAdrToInt(SYS_FL_MACH_CODE54) & 0x0400) != 0)
	{
		VarAdrSetInt(EXTRA_TEMP_VISIBLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(EXTRA_TEMP_VISIBLE, FALSE);
	}
}

/*****************************************
*Object   :温控系数一到七段最小值

*Object   :温控系数一到七段最大值
******************************************/
static void OneToSevenVal()
{
	if ((VarAdrToInt(SYS_FL_TEMPER) & 0x4000) != 0)
	{
		VarAdrSetInt(ONE_TO_SEVEN_MIN, 0);
		VarAdrSetInt(ONE_TO_SEVEN_MAX, 700);
	}
	else
	{
		VarAdrSetInt(ONE_TO_SEVEN_MIN, 165);
		VarAdrSetInt(ONE_TO_SEVEN_MAX, 195);
	}
}

/****************************其它界面逻辑模块-OTHER****************************/

/*****************************************
*Subject  :
*Condition:
*Object   :每次点动步数是否可用
*Result   :
******************************************/

static void EveryInchingStepEnable()
{
	if ((VarAdrToInt(ADJUST_ADPOSI_ADJMD) > 0) && (VarAdrToInt(ADJUST_CN_ADJFIRST) == 0))
	{
		VarAdrSetInt(EVERY_INCHING_STEP_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(EVERY_INCHING_STEP_ENABLE, FALSE);
	}
}

/*****************************************
*Subject  :
*Condition:
*Object   :震雄调模是否可用
*Result   :
******************************************/

static void PosAdjustEnable()
{
	if ((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x08) != 0)
	{
		VarAdrSetInt(POS_ADJUST_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(POS_ADJUST_ENABLE, FALSE);
	}
}

/*****************************************
*Subject  :
*Condition:
*Object   :座台使用位置是否使用
*Result   :
******************************************/

static void NozPosEnable()
{
	UI16 useMeter = ((VarAdrToInt(SYS_FL_METER) & 0x04) != 0);
	
	if (useMeter && ((SYS_SERVO3 && (A620 || A320)) == 0))
	{
		VarAdrSetInt(NOZ_POS_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(NOZ_POS_ENABLE, FALSE);
	}
}

/*****************************************
*Subject  :
*Condition:
*Object   :座台位置是否可见
*Result   :
******************************************/

static void NozMeterVisible()
{
	if ((VarAdrToInt(SYS_FL_CTRLMODEL) == 0xA920) || (SYS_SERVO3 == 0))
	{
		VarAdrSetInt(NOZ_METER_VISIBLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(NOZ_METER_VISIBLE, FALSE);
	}
}

/*****************************************
*Subject  :
*Condition:
*Object   :传感器是否可见
*Result   :
******************************************/

static void PressorVisible()
{
	if (SYS_SERVO3 && A620)
	{
		VarAdrSetInt(PRESSOR_VISIBLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(PRESSOR_VISIBLE, FALSE);
	}
}

/*****************************************
*Object   :伺服5是否可见

*Object   :伺服3是否可见
******************************************/
static void ZeroVisible()
{
	UI16 useMeter = ((VarAdrToInt(SYS_FL_METER) & 0x04) != 0);

	if (SYS_SERVO5 && (A620 || A320) && (! useMeter))
	{
		VarAdrSetInt(ZERO_VISIBLE, TRUE);
		VarAdrSetInt(AD4_VISIBLE, FALSE);
	} 
	else
	{
		VarAdrSetInt(ZERO_VISIBLE, FALSE);
		VarAdrSetInt(AD4_VISIBLE, TRUE);
	}
}

/*****************************************
*Subject  :
*Condition:
*Object   :座退阀图片索引
*Result   :
******************************************/

static void NozPoBwdImgVal()
{
	VarAdrSetInt(NOZ_PO_BWD_IMG_VAL, VarAdrToInt(NOZZLE_PO_O_NOZ_BWD) + 1);
}

/*****************************************
*Subject  :
*Condition:
*Object   :QDC功能是否可用与可见
*
*Object   :通用系统参数页面的密码输入是否可见
*
*Result   :
******************************************/
static void QDCFunctionEnable()//20200226
{
    if(OperateModeIndex()==4)
    {
        VarAdrSetInt(p_PP_OBJ_QDCFUNC_ENABLE,TRUE);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_QDCFUNC_ENABLE,FALSE);
    }

    //可见
    if(CUSTOMID_8B00)
    {
       VarAdrSetInt(p_PP_OBJ_QDCFUNC_VISIBLE,TRUE);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_QDCFUNC_VISIBLE,FALSE);
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :座台调模动作背景色
*Result   :
******************************************/
static void NozAdjActBkColor()
{
    //20200302
    UI16 i=0;
    UI16 stIndex = 0xFF;
    static UI16 stIndexOld = 0xFF;
    static BOOL init_bkcolor = TRUE;
    UI32 addrstep[MAX_ACT] = {VarAdrToInt(ACT_ACTNAME1),VarAdrToInt(ACT_ACTNAME2),VarAdrToInt(ACT_ACTNAME3),VarAdrToInt(ACT_ACTNAME4)};
    UI32 stGroup[] = {p_PP_OBJ_NOZADVFAST,p_PP_OBJ_NOZADVSLOW,p_PP_OBJ_NOZRET
                      ,p_PP_OBJ_ADJFAST,p_PP_OBJ_ADJSLOW,p_PP_OBJ_ADJRET};


    if(init_bkcolor)
    {
        init_bkcolor = FALSE;
        for (i = 0;  i< ARRAY_NUM(stGroup); i++)
        {
            VarAdrSetInt(stGroup[i],COLOR_TRANSPARENT);
        }
    }

    if(TRUE)//20201028 背景色变化做标准
    {
        for(i = 0; i < MAX_ACT; i++)//20210220
        {
            switch(addrstep[i]){
            case 0x11110000:
            case 0x11110001:
            case 0x11111000:
            case 0x11130000:
            case 0x11131000:
                stIndex = 0;    //Nozzle Adv Fast
                break;
            case 0x11111100:
                stIndex = 1;    //Nozzle Adv Slow
                break;
            case 0x11120000:
            case 0x11120001:
            case 0x11121000:
            case 0x11121100:
            case 0x1112FE00:
            case 0x11140000:
            case 0x11141000:
                stIndex = 2;    //Nozzle Ret
                break;
            case 0x24111100:
            case 0x24131100:
            case 0x24161100:
                stIndex = 3;    //Adj Adv Fast
                break;
            case 0x24110000:
            case 0x24111000:
            case 0x24130000:
            case 0x24131000:
            case 0x24160000:
            case 0x24161000:
                stIndex = 4;    //Adj Adv Slow
                break;
            case 0x24120000:
            case 0x24121000:
            case 0x24140000:
            case 0x24141000:
            case 0x24170000:
            case 0x24171000:
                stIndex = 5;    //Adj Ret
                break;
            default:
                stIndex = 0xFF;
            }

            if(stIndex != 0xFF)
            {
                break;
            }
        }

        if(stIndex != stIndexOld)
        {
            if((stIndex != 0xFF)&&(stIndexOld != 0xFF)){
                VarAdrSetInt(stGroup[stIndexOld],COLOR_TRANSPARENT);
                VarAdrSetInt(stGroup[stIndex],COLOR_ORANGRED);//0x0033ff
            }
            else if(stIndexOld == 0xFF){
                VarAdrSetInt(stGroup[stIndex],COLOR_ORANGRED);
            }
            else if(stIndex == 0xFF){
                VarAdrSetInt(stGroup[stIndexOld],COLOR_TRANSPARENT);
            }
            stIndexOld = stIndex;
        }
    }
}

/****************************其它功能界面逻辑模块-OTHER****************************/

/*****************************************
*Subject  :
*Condition:
*Object   :自动安全门是否使用
*Result   :
******************************************/

static void AutoSafeDoorEnable()
{
	if (((VarAdrToInt(SYS_FL_MACH_CODE4) & 0x80) != 0) && (VarAdrToInt(FL_PWRDOORCTRL)))
	{
		VarAdrSetInt(AUTO_SAFE_DOOR_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(AUTO_SAFE_DOOR_ENABLE, FALSE);
	}
}

/*****************************************
*Subject  :
*Condition:
*Object   :途中机械手是否使用
*Result   :
******************************************/

static void WayRobotFuncEnable()
{
    if ((VarAdrToInt(FL_ROBOTCTRL) >0) && (VarAdrToInt(CLAMP_MACHSET_FL_ADPOSROBOT)>0))
	{
		VarAdrSetInt(WAY_ROBOT_FUNC_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(WAY_ROBOT_FUNC_ENABLE, FALSE);
	}
}

/*****************************************
*Subject  :
*Condition:
*Object   :中子信号1连接是否可用
*Result   :
******************************************/

static void RobCore1LinkEnable()
{
	if (VarAdrToInt(FL_ROBOTCTRL) >= 2)
	{
		VarAdrSetInt(ROB_CORE_1_LINK_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(ROB_CORE_1_LINK_ENABLE, FALSE);
	}
}

/*****************************************
*Subject  :
*Condition:
*Object   :中子信号2连接是否可用
*Result   :
******************************************/

static void RobCore2LinkEnable()
{
	if (VarAdrToInt(FL_ROBOTCTRL) == 3)
	{
		VarAdrSetInt(ROB_CORE_2_LINK_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(ROB_CORE_2_LINK_ENABLE, FALSE);
	}
}

/*****************************************
*Subject  :
*Condition:
*Object   :特殊联动是否可用
*Result   :
******************************************/

static void SpecialLinkEnable()
{
    if((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0xB800)
    {
        VarAdrSetInt(SPECIAL_LINK_ENABLE, TRUE);
    }
    else
    {
        VarAdrSetInt(SPECIAL_LINK_ENABLE, FALSE);
    }
}

/****************************其它参数一界面逻辑模块-Other****************************/

/*****************************************
*Subject  :
*Condition:
*Object   :松模初始化是否可用
*Result   :
******************************************/

static void ReleasemdInitialEnable()
{
	if ((VarAdrToInt(SYS_FL_MACH_CODE5) & 0x0200) != 0)
	{
		VarAdrSetInt(RELEASEMD_INITIAL_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(RELEASEMD_INITIAL_ENABLE, FALSE);
	}
}

/*****************************************
*Subject  :
*Condition:
*Object   :锁模力最大有效区最大值
*Result   :
******************************************/

static void LockPrsEffareaMax()
{
    VarAdrSetInt(LOCKPRS_EFFAREA_MAX, VarAdrToInt(SYS_MACH_DAPRES_SYSLMT) * 10);
}

/****************************其它参数二界面逻辑模块-Other****************************/

/*****************************************
*Subject  :
*Condition:
*Object   :模厚调模是否使用
*Result   :
******************************************/

static void MoldThickAdjustEnable()
{
	if ((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x08) != 0)
	{
		VarAdrSetInt(MOLD_THICK_ADJUST_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(MOLD_THICK_ADJUST_ENABLE, FALSE);
	}
}

/*****************************************
*Subject  :
*Condition:
*Object   :每次移动距离小数位和最大值 20190531
*Result   :
******************************************/

static void MoldThickAdjust()
{
	if ((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x08) != 0)
	{
		if((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x0080) != 0)
		{
			VarAdrSetInt(CNPOSI_ADJPROPERTIONAL_DOT,3);
			VarAdrSetInt(CNPOSI_ADJPROPERTIONAL_MAX,5000);
		}
		else
		{
			VarAdrSetInt(CNPOSI_ADJPROPERTIONAL_DOT,1);
			VarAdrSetInt(CNPOSI_ADJPROPERTIONAL_MAX,1000);
		}
	}
}

/****************************主界面逻辑模块-Main****************************/

/*****************************************
*Object   :安全门图片索引

*Object   :安全门2图片索引

*Object   :后安全门图片索引

*Object   :螺杆检知图片索引

*Object   :开模终图片索引

*Object   :润滑检知1图片索引

*Object   :润滑检知2图片索引

*Object   :润滑检知3图片索引

*Object   :检出电眼图片索引

*Object   :座进终图片索引

*Object   :座退终图片索引

*Object   :dtAD6是否可见

*Object   :dtnozzle是否可见

*Object   :伺服相关是否可见

*Object   :座台电子尺是否可见

*Object   :蓄能器是否可见

*OBject   :螺杆是否可见
******************************************/

static void MainVal()
{
    UI32 heater_state;

#ifdef M788_PANEL
    if ((VarAdrToInt(SYS_FL_METER) & 0x0004) != 0)
    {
        VarAdrSetInt(NOZZLE_AD_VISIBLE, TRUE);
    }
    else
    {
        VarAdrSetInt(NOZZLE_AD_VISIBLE, FALSE);
    }
    return;
#endif

    if ((VarAdrToInt(SYS_FL_MACH_CODE4) & 0x0010) != 0)
    {
        VarAdrSetInt(MOTORTEMP_VISIBLE, VISIBLE);
    } 
    else
    {
        VarAdrSetInt(MOTORTEMP_VISIBLE, INVISIBLE);
    }

	if (VarAdrToInt(ADDR_I_SDR_CLS) == 0)
	{
		VarAdrSetInt(SAFEDOOR_IMG_VAL, ONE_IMG);
	} 
	else
	{
		VarAdrSetInt(SAFEDOOR_IMG_VAL, TWO_IMG);
	}

	if (VarAdrToInt(ADDR_I_SDR_CLS2) == 0)
	{
		VarAdrSetInt(SAFEDOOR_2_IMG_VAL, ONE_IMG);
	} 
	else
	{
		VarAdrSetInt(SAFEDOOR_2_IMG_VAL, TWO_IMG);
	}

	if (VarAdrToInt(ADDR_I_RSD_CLS) == 0)
	{
		VarAdrSetInt(BACK_SAFEDOOR_IMG_VAL, ONE_IMG);
	} 
	else
	{
		VarAdrSetInt(BACK_SAFEDOOR_IMG_VAL, TWO_IMG);
	}

	if (VarAdrToInt(ADDR_I_RPM_CHK) == 0)
	{
		VarAdrSetInt(SCREW_CHECK_IMG_VAL, ONE_IMG);
	} 
	else
	{
		VarAdrSetInt(SCREW_CHECK_IMG_VAL, TWO_IMG);
	}

	if (VarAdrToInt(ADDR_I_OPN_END) == 0)
	{
		VarAdrSetInt(OPN_END_IMG_VAL, ONE_IMG);
	} 
	else
	{
		VarAdrSetInt(OPN_END_IMG_VAL, TWO_IMG);
	}

	if (VarAdrToInt(ADDR_I_LUB_CHK1) == 0)
	{
		VarAdrSetInt(LUB_CHK1_IMG_VAL, ONE_IMG);
	} 
	else
	{
		VarAdrSetInt(LUB_CHK1_IMG_VAL, TWO_IMG);
	}

	if (VarAdrToInt(ADDR_I_LUB_CHK2) == 0)
	{
		VarAdrSetInt(LUB_CHK2_IMG_VAL, ONE_IMG);
	} 
	else
	{
		VarAdrSetInt(LUB_CHK2_IMG_VAL, TWO_IMG);
	}

	if (VarAdrToInt(ADDR_I_LUB_CHK3) == 0)
	{
		VarAdrSetInt(LUB_CHK3_IMG_VAL, ONE_IMG);
	} 
	else
	{
		VarAdrSetInt(LUB_CHK3_IMG_VAL, TWO_IMG);
	}

	if (VarAdrToInt(ADDR_I_PRD_CHK) == 0)
	{
		VarAdrSetInt(PRD_CHK_IMG_VAL, ONE_IMG);
	} 
	else
	{
		VarAdrSetInt(PRD_CHK_IMG_VAL, TWO_IMG);
	}

	if (VarAdrToInt(ADDR_I_NOZ_FWD) == 0)
	{
		VarAdrSetInt(NOZ_FWD_IMG_VAL, ONE_IMG);
	}
	else
	{
		VarAdrSetInt(NOZ_FWD_IMG_VAL, TWO_IMG);
	}

	if (VarAdrToInt(ADDR_I_NOZ_BWD) == 0)
	{
		VarAdrSetInt(NOZ_BWD_IMG_VAL, ONE_IMG);
	} 
	else
	{
		VarAdrSetInt(NOZ_BWD_IMG_VAL, TWO_IMG);
	}

	if ((VarAdrToInt(SYS_FL_MACH_CODE54) & 0x0020) != 0)
	{
		if (A920)
		{
			VarAdrSetInt(AD6_VISIBLE, TRUE);
		} 
		else
		{
			VarAdrSetInt(AD6_VISIBLE, FALSE);
		}
	} 
	else
	{
		VarAdrSetInt(AD6_VISIBLE, FALSE);
	}

	if ((VarAdrToInt(SYS_FL_MACH_CODE54) & 0x0020) != 0)
	{
		if (A920)
		{
			VarAdrSetInt(NOZZLE_VISIBLE, FALSE);
		} 
		else
		{
			VarAdrSetInt(NOZZLE_VISIBLE, TRUE);
		}
	} 
	else
	{
		VarAdrSetInt(NOZZLE_VISIBLE, FALSE);
	}

	if ((VarAdrToInt(SYS_FL_MACH_CODE54) & 0x0020) != 0)
	{
		VarAdrSetInt(SERVO_VISIBLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(SERVO_VISIBLE, FALSE);
	}

	if ((VarAdrToInt(SYS_FL_METER) & 0x0004) != 0)
	{
		VarAdrSetInt(NOZZLE_AD_VISIBLE, TRUE);
        VarAdrSetInt(NOZZLE_IMG_VAL, TWO_IMG);
	} 
	else
	{
		VarAdrSetInt(NOZZLE_AD_VISIBLE, FALSE);
        VarAdrSetInt(NOZZLE_IMG_VAL, ONE_IMG);
	}

	if (VarAdrToInt(ADDR_INCACC) == 0)
	{
		VarAdrSetInt(ACC_VISIBLE, FALSE);
	} 
	else
	{
		VarAdrSetInt(ACC_VISIBLE, TRUE);
	}

    if(VarAdrToInt(SYS_CN_CHGRPMHOLE))
    {
        VarAdrSetInt(PIXSCREWSPD_VISIBLE, VISIBLE);
    }
    else
    {
        VarAdrSetInt(PIXSCREWSPD_VISIBLE, INVISIBLE);
    }

    //20200323 电热状态地址切换
    if(IV3200 || IV5200 || IV3300 || IV5300)
    {
        heater_state = VarAdrToInt(d_TempCtrlComm1_STATE_FL_HEATERSTATUS);//0E160038 E502  E506
    }
    else
    {
        heater_state = VarAdrToInt(d_tempctrl1_STATE_FL_HEATERSTATUS); //0516002B
    }
    if (heater_state)
    {
        VarAdrSetInt(HEATER_VISIBLE, VISIBLE);
    }
    else
    {
        VarAdrSetInt(HEATER_VISIBLE, INVISIBLE);
    }

    if (VarAdrToInt(ADDR_ROBOTCTRL))
    {
        VarAdrSetInt(ROBOT_VISIBLE, VISIBLE);
    } 
    else
    {
        VarAdrSetInt(ROBOT_VISIBLE, INVISIBLE);
    }

    if (VarAdrToInt(ADDR_MOLDLUBRIC) || VarAdrToInt(ADDR_MOLDLUBRIC2) || VarAdrToInt(ADDR_MOLDLUBRIC3))
    {
        VarAdrSetInt(LUBR_VISIBLE, VISIBLE);
    } 
    else
    {
        VarAdrSetInt(LUBR_VISIBLE, INVISIBLE);
    }

}


/*****************************************

*Object   :伺服状态信息是否可见

******************************************/

static void ServoStVisible()
{
	UI16 normal;
	UI16 i;
	UI32 servo_state_err;

	if (SYS_SERVO5)
	{
		normal = TRUE;
        for (i = 0; i < (ADDR_SERVO_ERR8-SERVO_STATE_ERR1); i++)
		{
			servo_state_err = VarAdrToInt(SERVO_STATE_ERR1 + i);
			if (servo_state_err > 0)
			{
				VarAdrSetInt(SERVO_ST_VISIBLE, TRUE);
				normal = FALSE;
				break;
			}
		}

		if (normal)
		{
			VarAdrSetInt(SERVO_ST_VISIBLE, FALSE);
		}
	}
	else if (((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x0004) != 0) && ((VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020) != 0))
	{
		normal = TRUE;
        for (i = 1; i < (ADDR_SERVO_ERR8-SERVO_STATE_ERR1 + 1); i++)
		{
			servo_state_err = dbsvox[i].wData[ID_8000];
			if (servo_state_err > 0)
			{
				VarAdrSetInt(SERVO_ST_VISIBLE, TRUE);
				normal = FALSE;
				break;
			}
		}

		if (normal)
		{
			VarAdrSetInt(SERVO_ST_VISIBLE, FALSE);
		}
	}
	else
	{
		VarAdrSetInt(SERVO_ST_VISIBLE, FALSE);
	}
}

/*****************************************
*Subject  :
*Condition:
*Object   :电机图标是否可见
*Result   :
******************************************/

static void MotorVisible()
{
	UI32 cyctime = VarAdrToInt(PROG_CYCTIME);
	static UI32 motorTime;
	UI32 nMs;
	BOOL bTimer = FALSE;
	UI16 motorstate;
	static BOOL Motor; 

	nMs = cyctime - motorTime;
	if(nMs >= 500)
	{
		motorTime = cyctime;
		bTimer = TRUE;
	}

    motorstate = VarAdrToUI16(MOTOR_STATE_STATE);
	if(motorstate == MOTOR_START)
	{
		if(bTimer) 
		{
			Motor = ! Motor;
		}
	}
	else if(motorstate == MOTOR_RUNNING) 
	{
		Motor = TRUE;
	}
	else
	{
		Motor=FALSE;
	}

	VarAdrSetInt(MOTOR_VISIBLE, Motor);
}



/*****************************************
*Subject  :
*Condition:
*Object   :伺服通信状态是否可见
*Result   :
******************************************/

static void ServoStOneToSevenVisible()
{
	UI32 cyctime = VarAdrToInt(PROG_CYCTIME);
	static UI32 servoStTime;
	UI16 i;
	UI32 value;

	if((cyctime - servoStTime) > 300)
	{
		servoStTime = cyctime;
		if(SYS_SERVO5)
		{
            for(i=0; i < (ADDR_SERVO_CANST8 - SERVO_STATE_CANST1); ++i)
			{
                value = VarAdrToInt(SERVO_STATE_CANST1 + i);
				if(value & 0x80)//判断联机脱机
				{
					VarAdrSetInt(SERVO_ST1_VISIBLE + i, TRUE);
				}
				else
				{
					VarAdrSetInt(SERVO_ST1_VISIBLE + i, FALSE);
				}
			}
		}
		else if(((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x0004) != 0) && ((VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020) != 0))
		{
			for(i = 1; i < 8; ++i)
			{
				if(dbsvox[i].nCount != 0)
				{
					VarAdrSetInt(SERVO_ST1_VISIBLE + i - 1, TRUE);
				}
				else
				{
					VarAdrSetInt(SERVO_ST1_VISIBLE + i - 1, FALSE);
				}
			}
		}
	}
}

/*****************************************
*Subject  :
*Condition:
*Object   :伺服通信颜色切换
*Result   :
******************************************/

static void ServoColorOneToSevenVisible()
{
	UI32 cyctime = VarAdrToInt(PROG_CYCTIME);
	static UI32 colorStTime;
	UI16 i;
	UI32 value;

	if((cyctime - colorStTime) > 300)
	{
		colorStTime = cyctime;
		if(SYS_SERVO5)
		{
            for(i=0; i < (ADDR_SERVO_CANST8 - SERVO_STATE_CANST1); i++)
			{
                value = VarAdrToInt(SERVO_STATE_CANST1 + i);
				if(value & 0x80)//判断联机脱机
				{
                    if(((value & 0x07) == 7) || VarAdrToInt(SERVO_STATE_ERR1 + i))
					{
						VarAdrSetInt(SERVO_COLOR1_VAL + i, COLOR_RED);
					}
					else if(((value & 0x07)==1) || ((value & 0x07)==2))
					{
						VarAdrSetInt(SERVO_COLOR1_VAL + i, COLOR_GREEN);
					}
					else
					{
						VarAdrSetInt(SERVO_COLOR1_VAL + i, COLOR_GRAY);
					}
				}
				
			}
		}
		else if((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x0004) && (VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020))
		{
			for(i = 1; i < 8; i++)
			{
				if(dbsvox[i].nCount != 0)
				{
					if(dbsvox[i].wData[ID_8000])
					{
						VarAdrSetInt(SERVO_COLOR1_VAL + i - 1, COLOR_RED);
					}
					else if((dbsvox[i].wData[ID_3000] == 1) || (dbsvox[i].wData[ID_3000] == 2))
					{
						VarAdrSetInt(SERVO_COLOR1_VAL + i - 1, COLOR_GREEN);
					}
					else
					{
						VarAdrSetInt(SERVO_COLOR1_VAL + i - 1, COLOR_GRAY);
					}
				}
				dbsvox[i].nCount=0;
			}
		}
	}
}

/****************************电子尺调整逻辑界面-AD_Adjust****************************/

/*****************************************
*Subject  :
*Condition:
*Object   :A920和伺服3是否选用
*Result   :
******************************************/

static void A920Ser3ModleEnable()
{
	if (A920)
	{
		VarAdrSetInt(A920_MODEL_ENABLE, TRUE);
        VarAdrSetInt(SENSOR_VISIBLE, FALSE);
        VarAdrSetInt(NOZ_VISIBLE, TRUE);
        VarAdrSetStr(SENSOR_NOZ_UNIT, "mm");
        VarAdrSetStr(SENSOR_NOZ_UNIT2, "(mm)");
		if (SYS_SERVO3)
		{
			VarAdrSetStr(SENSOR_AD6_UNIT, "bar");
            VarAdrSetStr(SENSOR_AD6_UNIT2, "(bar)");
			VarAdrSetInt(SENSOR2_VISIBLE, TRUE);
			VarAdrSetInt(CFG_AD6_VISIBLE, FALSE);
		} 
		else
		{
			VarAdrSetStr(SENSOR_AD6_UNIT, "mm");
            VarAdrSetStr(SENSOR_AD6_UNIT2, "(mm)");
			VarAdrSetInt(SENSOR2_VISIBLE, FALSE);
			VarAdrSetInt(CFG_AD6_VISIBLE, TRUE);
		}

	} 
	else
	{
		VarAdrSetInt(A920_MODEL_ENABLE, FALSE);
        VarAdrSetInt(SENSOR2_VISIBLE, FALSE);
        VarAdrSetInt(CFG_AD6_VISIBLE, FALSE);
		if (SYS_SERVO3)
		{
			VarAdrSetInt(SENSOR_VISIBLE, TRUE);
			VarAdrSetInt(NOZ_VISIBLE, FALSE);
			VarAdrSetStr(SENSOR_NOZ_UNIT, "bar");
            VarAdrSetStr(SENSOR_NOZ_UNIT2, "(bar)");
		} 
		else
		{
			VarAdrSetInt(SENSOR_VISIBLE, FALSE);
			VarAdrSetInt(NOZ_VISIBLE, TRUE);
			VarAdrSetStr(SENSOR_NOZ_UNIT, "mm");
            VarAdrSetStr(SENSOR_NOZ_UNIT2, "(mm)");
		}
	}
}

/****************************DA调整界面-DA_Adjust****************************/

/*****************************************
*Subject  :
*Condition:
*Object   :厂商是否为威亚
*Result   :
******************************************/

static void WeiYaEnable()
{
	if (CUSTOMID_BA00)
	{
		VarAdrSetInt(WEIYA_ENABLE, FALSE);
	} 
	else
	{
		VarAdrSetInt(WEIYA_ENABLE, TRUE);
	}
}


/****************************数据检测界面-Data_Diag****************************/

/*****************************************
*Subject  :
*Condition:
*Object   :code51打2是否使用
*Result   :
******************************************/

static void Code3Fill2Enable()
{
	if ((VarAdrToInt(SYS_FL_MACH_CODE51) & 0x0002) != 0)
	{
		VarAdrSetInt(CODE3_2_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(CODE3_2_ENABLE, FALSE);
	}
}

/****************************主测试界面-Main_Adjust****************************/

/*****************************************
*Subject  :
*Condition:
*Object   :加号是否可见
*Result   :
******************************************/

static void AddSignVisible()
{
	UI16 i;
	const UI16 add_num1to4 = 4;

	for (i = 0; i < add_num1to4; ++i)
	{
		if (VarAdrToInt(ADMETER_ABSOLUTE1 + i) >= VarAdrToInt(ADMETER_ZSET1 + i))
		{
			VarAdrSetInt(ADD_SIGN1_VISIBLE + i, TRUE);
			VarAdrSetInt(SUB_SIGN1_VISIBLE + i, FALSE);
		} 
		else
		{
			VarAdrSetInt(ADD_SIGN1_VISIBLE + i, FALSE);
			VarAdrSetInt(SUB_SIGN1_VISIBLE + i, TRUE);
		}
	}

	if ((VarAdrToInt(ADMETER_ABSOLUTE5) >= VarAdrToInt(ADMETER_ZSET5)) && A920)
	{
		VarAdrSetInt(ADD_SIGN5_VISIBLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(ADD_SIGN5_VISIBLE, FALSE);
	}

	if (!(VarAdrToInt(ADMETER_ABSOLUTE5) >= VarAdrToInt(ADMETER_ZSET5)) && A920)
	{
		VarAdrSetInt(SUB_SIGN5_VISIBLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(SUB_SIGN5_VISIBLE, FALSE);
	}

	if ((VarAdrToInt(ADMETER_ABSOLUTE6) >= VarAdrToInt(ADMETER_ZSET6)) && A920)
	{
		VarAdrSetInt(ADD_SIGN6_VISIBLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(ADD_SIGN6_VISIBLE, FALSE);
	}

	if (!(VarAdrToInt(ADMETER_ABSOLUTE6) >= VarAdrToInt(ADMETER_ZSET6)) && A920)
	{
		VarAdrSetInt(SUB_SIGN6_VISIBLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(SUB_SIGN6_VISIBLE, FALSE);
	}

	if (VarAdrToInt(TEMP_STATE_LM73) <= 0x8000)
	{
		VarAdrSetInt(ROOM_ADD_SIGN_VISIBLE, TRUE);
		VarAdrSetInt(ROOM_SUB_SIGN_VISIBLE, FALSE);
	} 
	else
	{
		VarAdrSetInt(ROOM_ADD_SIGN_VISIBLE, FALSE);
		VarAdrSetInt(ROOM_SUB_SIGN_VISIBLE, TRUE);
	}
}



/****************************主框架界面-Mainframe****************************/

/*****************************************
*Subject  :
*Condition:
*Object   :伺服是否使用
*Result   :
******************************************/

static void UseServoVisible()
{
	if ((VarAdrToInt(SYS_FL_MACH_CODE54) & 0x20) != 0)
	{
		VarAdrSetInt(USE_SERVO_VISIBLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(USE_SERVO_VISIBLE, FALSE);
	}
}
/****************************泵浦配置界面-Pump_cfg****************************/

/*****************************************
*Subject  :
*Condition:
*Object   :泵浦调整是否可用
*Result   :
******************************************/

static void PumpAdjustEnable()
{
	if (VarAdrToInt(HYDR_FL_PWRMATCHTBL) == 1)
	{
		VarAdrSetInt(PUMP_ADJUST_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(PUMP_ADJUST_ENABLE, FALSE);
	}
}

/****************************泵浦配置额外界面-Pump_cfg_x****************************/

/*****************************************
*Subject  :
*Condition:
*Object   :单泵多泵是否使用
*Result   :
******************************************/

static void PumpVisible()
{
	if ((VarAdrToInt(SYS_FL_HYDRAUIC) & 0x800) != 0)
	{
		VarAdrSetInt(MULT_PUMP_VISIBLE, TRUE);
		VarAdrSetInt(SINGLE_PUMP_VISIBLE, FALSE);
	} 
	else
	{
		VarAdrSetInt(MULT_PUMP_VISIBLE, FALSE);
		VarAdrSetInt(SINGLE_PUMP_VISIBLE, TRUE);
	}
}

/*****************************************
*Subject  :
*Condition:
*Object   :泵启停速度是否可用
*Result   :
******************************************/

static void ViceStrStopEnable()
{
	if (((VarAdrToInt(SYS_FL_HYDRAUIC) & 0x800) != 0) && (VarAdrToInt(HYDR_FL_PWRMATCHTBL) == 1) && SYS_SERVO5)
	{
		VarAdrSetInt(VICE_STR_STOP_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(VICE_STR_STOP_ENABLE, FALSE);
	}
}

/*****************************************
*Subject  :
*Condition:
*Object   :泵浦2-7是否使用
*Result   :
******************************************/

static void PumbAdj2to7Enable()
{
	if (((VarAdrToInt(SYS_FL_HYDRAUIC) & 0x800) != 0) && (VarAdrToInt(HYDR_FL_PWRMATCHTBL) == 1))
	{
		VarAdrSetInt(PUMP_ADJ_2TO7_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(PUMP_ADJ_2TO7_ENABLE, FALSE);
	}
}

/****************************顺序阀界面-Seq_Vlv****************************/

/*****************************************
*Subject  :
*Condition:
*Object   :顺序阀方式
*Result   :
******************************************/

static void SetSequenceVlvMode()
{	
	const UI16 gap = 4;
	const UI16 seq_vlv_num = 14;
	UI16 i;

	for (i = 0; i < seq_vlv_num; ++i)
	{
		switch(VarAdrToInt(AIRBLOW1_FL_SEQ_USE + i * 0x00100000))
		{
		case 0:
			VarAdrSetInt(DLY_TIME_1_ENABLE + i * gap, FALSE);
			VarAdrSetInt(ACT_TIME_1_ENABLE + i * gap, FALSE);
			VarAdrSetInt(START_POS_1_ENABLE + i * gap, FALSE);
			VarAdrSetInt(END_POS_1_ENABLE + i * gap, FALSE);
			break;
		case 1:
			VarAdrSetInt(DLY_TIME_1_ENABLE + i * gap, TRUE);
			VarAdrSetInt(ACT_TIME_1_ENABLE + i * gap, TRUE);
			VarAdrSetInt(START_POS_1_ENABLE + i * gap, FALSE);
			VarAdrSetInt(END_POS_1_ENABLE + i * gap, FALSE);
			break;
		case 2:
			VarAdrSetInt(DLY_TIME_1_ENABLE + i * gap, FALSE);
			VarAdrSetInt(ACT_TIME_1_ENABLE + i * gap, FALSE);
			VarAdrSetInt(START_POS_1_ENABLE + i * gap, TRUE);
			VarAdrSetInt(END_POS_1_ENABLE + i * gap, TRUE );
			break;
		case 3:
			VarAdrSetInt(DLY_TIME_1_ENABLE + i * gap, TRUE);
			VarAdrSetInt(ACT_TIME_1_ENABLE + i * gap, FALSE);
			VarAdrSetInt(START_POS_1_ENABLE + i * gap, FALSE);
			VarAdrSetInt(END_POS_1_ENABLE + i * gap, TRUE);
			break;
		case 4:
			VarAdrSetInt(DLY_TIME_1_ENABLE + i * gap, FALSE);
			VarAdrSetInt(ACT_TIME_1_ENABLE + i * gap, TRUE);
			VarAdrSetInt(START_POS_1_ENABLE + i * gap, TRUE);
			VarAdrSetInt(END_POS_1_ENABLE + i * gap, FALSE);
			break;
		default:
			break;
		}
	}	
}

/*****************************************
*Subject  :
*Condition:
*Object   :顺序阀图片
*Result   :
******************************************/

static void SeqVlvImgVal()
{
	UI16 i;
	const UI16 seq_vlv_num = 14;
	for (i = 0; i < seq_vlv_num; ++i)
	{
		if (VarAdrToInt(AIRBLOW1_PO_AIR_SEQVLV + i * 0x00100000) == 0)
		{
			VarAdrSetInt(SEQ_VLV_1_IMG_VAL + i, 1);
		} 
		else
		{
			VarAdrSetInt(SEQ_VLV_1_IMG_VAL + i, 2);
		}
	}
}

/****************************伺服曲线界面-Servo_Curve****************************/

/*****************************************
*Subject  :
*Condition:
*Object   :母线电压是否可用
*Result   :
******************************************/

static void Servo3Enable()
{
	if (SYS_SERVO3)
	{
		VarAdrSetInt(SERVO_3_ENABLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(SERVO_3_ENABLE, FALSE);
	}
}

/****************************伺服测试3界面-Servo_Test3****************************/

/*****************************************
*Subject  :
*Condition:
*Object   :系统压力是否可见
*Result   :
******************************************/

static void SysPrsVisible()
{
	if (A320 || A620 || (VarAdrToInt(SYS_FL_CTRLMODEL) == 0))
	{
		VarAdrSetInt(SYS_PRS_VISIBLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(SYS_PRS_VISIBLE, FALSE);
	}
}

/****************************系统参数界面-Sys_Para****************************/
/*****************************************
*Subject  :
*Condition:
*Object   :通用开关模压力上限密码控制
*Result   :
******************************************/
static void ClpPrsMaxCtrl()//20200302
{
    //20241017 chj标准存在通用厂商代码，注释掉
    /*
    if(CUSTOMID_8B00)
    {
        if (VarAdrToInt(p_PP_TMP_TMPVAL58) == 5656)
        {
            VarAdrSetInt(p_PP_OBJ_CLAMPPRS_TEXTCOLOR,0x000000);
            VarAdrSetInt(p_PP_OBJ_CLAMPPRS_BKCOLOR,0xFFFFFF);
            VarAdrSetInt(p_PP_OBJ_CLAMPPRS_READABLE,FALSE);
        }
        else
        {
            VarAdrSetInt(p_PP_OBJ_CLAMPPRS_TEXTCOLOR,0x000000);//0xFFFFFF
            if(PanelCfgStyle()==P_TYPE)
            {
                VarAdrSetInt(p_PP_OBJ_CLAMPPRS_BKCOLOR,0x508000);//0x0068a800
            }
            else
            {
                VarAdrSetInt(p_PP_OBJ_CLAMPPRS_BKCOLOR,0xdedede);
            }
            VarAdrSetInt(p_PP_OBJ_CLAMPPRS_READABLE,TRUE);
        }
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_CLAMPPRS_TEXTCOLOR,0x000000);
        VarAdrSetInt(p_PP_OBJ_CLAMPPRS_BKCOLOR,0xFFFFFF);
        VarAdrSetInt(p_PP_OBJ_CLAMPPRS_READABLE,FALSE);
    }*/
}

/****************************系统参数3界面-Sys_Para3****************************/


/*****************************************
*Subject  :
*Condition:
*Object   :特殊数据设置是否可见
*Result   :
******************************************/

static void SpecialDataSetupVisible()
{
    if (((VarAdrToInt(SYS_FL_MACH_CODE51) & 0x40) != 0) || CUSTOMID_8B00)
	{
		VarAdrSetInt(SPECIAL_DATA_SETUP_VISIBLE, TRUE);
	} 
	else
	{
		VarAdrSetInt(SPECIAL_DATA_SETUP_VISIBLE, FALSE);
	}
}

/****************************系统参数4界面-Sys_Para5****************************/


/*****************************************
*Subject  :
*Condition:
*Object   :射出/射退慢速启动参数设置是否可用
*Result   :
******************************************/

static void InjSlowStartDataSetupEnable()
{
    if ((VarAdrToInt(SYS_FL_MACH_CODE3) & 0x400) != 0)
    {
        VarAdrSetInt(INJSLOW_START_ENABLE, TRUE);
    }
    else
    {
        VarAdrSetInt(INJSLOW_START_ENABLE, FALSE);
    }
}


/****************************版本界面-Version****************************/


/*****************************************
*Subject  :
*Condition:
*Object   :扩展板是否可用
*Result   :
******************************************/

static void ExBoardEnable()
{
	UI16 i;
	const UI16 state_num = 7;
	for (i = 0; i < state_num; ++i)
	{
		if ((VarAdrToInt(EXTIOST) & (1 << i)) != 0)
		{
			VarAdrSetInt(IO_EXTEND_1_ENABLE + i, TRUE);
		} 
		else
		{
			VarAdrSetInt(IO_EXTEND_1_ENABLE + i, FALSE);
		}
	}

	for (i = 0; i < state_num; ++i)
	{
		if ((VarAdrToInt(EXTDAST) & (1 << i)) != 0)
		{
			VarAdrSetInt(DA_EXTEND_1_ENABLE + i, TRUE);
		} 
		else
		{
			VarAdrSetInt(DA_EXTEND_1_ENABLE + i, FALSE);
		}
	}

	for (i = 0; i < state_num; ++i)
	{
		if ((VarAdrToInt(EXTTEMPST) & (1 << i)) != 0)
		{
			VarAdrSetInt(TEMP_EXTEND_1_ENABLE + i, TRUE);
		} 
		else
		{
			VarAdrSetInt(TEMP_EXTEND_1_ENABLE + i, FALSE);
		}
	}

	for (i = 0; i < state_num; ++i)
	{
		if (VarAdrToInt(MDTEMP_STATEX1 + i) == 1)
		{
			VarAdrSetInt(MDTP_EXTEND_1_ENABLE + i, TRUE);
		} 
		else
		{
			VarAdrSetInt(MDTP_EXTEND_1_ENABLE + i, FALSE);
		}
	}
}

/*****************************************
*Subject  :
*Condition:
*Object   :8路电子尺可见
*Result   :
******************************************/

static void AD78Visible()
{
//    if(IV5200 || IV5300)
//    {
//        VarAdrSetInt(p_PP_OBJ_AD5AD6_SHOW,TRUE);
//        VarAdrSetInt(p_PP_OBJ_MODEL5200_SHOW,TRUE);
//    }
//    else
//    {
//        if(IV3100 && ((VarAdrToUI16(d_machine1_VERSION_model_and_cat)&0x1FFF)==3101))
//        {
//            VarAdrSetInt(p_PP_OBJ_AD5AD6_SHOW,TRUE);
//        }
//        else
//        {
//            VarAdrSetInt(p_PP_OBJ_AD5AD6_SHOW,FALSE);
//        }
//        VarAdrSetInt(p_PP_OBJ_MODEL5200_SHOW,FALSE);
//    }
    if(g_mainresource_current.nADNum>4 && g_mainresource_current.nADNum<8)
    {
        VarAdrSetInt(p_PP_OBJ_AD5AD6_SHOW, TRUE);
        VarAdrSetInt(p_PP_OBJ_MODEL5200_SHOW,FALSE);
    }
    else if(g_mainresource_current.nADNum==8)
    {
        VarAdrSetInt(p_PP_OBJ_AD5AD6_SHOW, TRUE);
        VarAdrSetInt(p_PP_OBJ_MODEL5200_SHOW,TRUE);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_AD5AD6_SHOW, FALSE);
        VarAdrSetInt(p_PP_OBJ_MODEL5200_SHOW,FALSE);
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :AD电子尺根据通道用作压力传感器
*Result   :
******************************************/
static void ADNameByPressChanel()
{
    if((VarAdrToUI16(d_machine1_VERSION_model_and_cat)&0x1FFF)==3101)//6AD+8EX_AD
    {
        if (VarAdrToUI16(d_Pump1_MACHSET_PRESSAD_CH) == 4)
        {
            VarAdrSetInt(p_PP_OBJ_INJECT_LINE1_VISIBLE,TRUE);
        }
    }
}

/****************************系统自学习界面-Servo_Cfg****************************/

/*****************************************
*Object   :驱动器警报提示

*Object   :伺服学习步骤提示

******************************************/

static void ServoSelfLearnOrAlarm()
{
	if (VarAdrToInt(SERVO_SELFLEARNPROMPT) == 1)
	{
		VarAdrSetInt(SERVO_ALARM_VISIBLE, TRUE);
		VarAdrSetInt(SERVO_SELF_LEARN_VISIBLE, FALSE);
	} 
	else
	{
		VarAdrSetInt(SERVO_ALARM_VISIBLE, FALSE);
		VarAdrSetInt(SERVO_SELF_LEARN_VISIBLE, TRUE);
	}
}

/****************************数据保护界面-Servo_Cfg****************************/
/*****************************************
*Object   :模具冷却计时是否只读
*Object   :模具冷却计时背景色
*Object   :模具冷却计时文本色

******************************************/
static void CoolTmProtect()
{
	if(VarAdrToInt(FLCOOLTIMEPROT) ==1)
	{
		VarAdrSetInt(COOLTM_READ_PROT,TRUE);
        VarAdrSetInt(COOLTM_BKCOLOR_VAL,0xdedede);//BGR
        VarAdrSetInt(COOLTM_TXCOLOR_VAL,0x000000);
	}
	else
	{
		VarAdrSetInt(COOLTM_READ_PROT,FALSE);
        VarAdrSetInt(COOLTM_BKCOLOR_VAL,0xFFFFFF);//白色
        VarAdrSetInt(COOLTM_TXCOLOR_VAL,0x000000);//黑色
	}
    if(VarAdrToInt(FLCLAMPSPDPROT) == 1)
    {
        VarAdrSetInt(CLAMP_SPD_PORT,TRUE);
        VarAdrSetInt(CLAMP_SPD_BKCOLOR_VAL,0xdedede);//BGR
        VarAdrSetInt(CLAMP_SPD_TXCOLOR_VAL,0x000000);
    }
    else
    {
        VarAdrSetInt(CLAMP_SPD_PORT,FALSE);
        VarAdrSetInt(CLAMP_SPD_BKCOLOR_VAL,0xFFFFFF);//白色
        VarAdrSetInt(CLAMP_SPD_TXCOLOR_VAL,0x000000);//黑色
    }
    if(VarAdrToInt(FLHIPRSPRSPROT) == 1)
    {
        VarAdrSetInt(HIPRS_PRS_PORT,TRUE);
        VarAdrSetInt(HIPRS_PRS_BKCOLOR_VAL,0xdedede);//BGR
        VarAdrSetInt(HIPRS_PRS_TXCOLOR_VAL,0x000000);//白色
    }
    else
    {
        VarAdrSetInt(HIPRS_PRS_PORT,FALSE);
        VarAdrSetInt(HIPRS_PRS_BKCOLOR_VAL,0xFFFFFF);//BGR
        VarAdrSetInt(HIPRS_PRS_TXCOLOR_VAL,0x000000);//白色
    }
    if(VarAdrToInt(FLHIPRSSPDPROT) == 1)
    {
        VarAdrSetInt(HIPRS_SPD_PORT,TRUE);
        VarAdrSetInt(HIPRS_SPD_BKCOLOR_VAL,0xdedede);//BGR
        VarAdrSetInt(HIPRS_SPD_TXCOLOR_VAL,0x000000);
    }
    else
    {
        VarAdrSetInt(HIPRS_SPD_PORT,FALSE);
        VarAdrSetInt(HIPRS_SPD_BKCOLOR_VAL,0xFFFFFF);//BGR
        VarAdrSetInt(HIPRS_SPD_TXCOLOR_VAL,0x000000);//白色
    }
}

/*****************************以下为10竖屏开发新增**************************************/

/*****************************主界面变量模块-Main*********************************************/
/*****************************************
*Object   :主画面开关模动作图片

******************************************/
static void MainClampImgVal()
{
    static UI16 cClampPicNo = 1;
    static UI16 wAdClamp = 0;
    static UI16 wAdEject = 0;
    int num_close = 4;
    int iClampTemp = (num_close -1)*2;
    int  i=1;
    unsigned uDevision1 = (unsigned)VarAdrToUI16(ADPOSI_OPEN5)/iClampTemp;
    unsigned uDevision2 = (unsigned)VarAdrToUI16(ADPOSI_EJTADVFASTEND)/4;
    if((abs(VarAdrToUI16(ADDR_ADPOSI_REAL2) -wAdClamp) >20) ||(abs(VarAdrToUI16(ADDR_ADPOSI_REAL3) - wAdEject) >20)||(VarAdrToUI16(ACT_STATE) ==1)) //20141104, >20mm
    {
        if(VarAdrToUI16(ADDR_ADPOSI_REAL2) <= uDevision1)
        {
            if((VarAdrToUI16(ACT_STATE) ==1)||(VarAdrToUI16(ACT_STATE) ==2))
                i =5;
            else
                i =4;
        }
        else  if((VarAdrToUI16(ADDR_ADPOSI_REAL2) > uDevision1) &&(VarAdrToUI16(ADDR_ADPOSI_REAL2) <= uDevision1*3))
        {
            if(VarAdrToUI16(ACT_STATE) ==2)
                i =6;
            else
                i =3;
        }
        else  if((VarAdrToUI16(ADDR_ADPOSI_REAL2) > uDevision1*3) &&(VarAdrToUI16(ADDR_ADPOSI_REAL2) <= uDevision1*5))
        {
            if(VarAdrToUI16(ACT_STATE) ==2)
                i =7;
            else
                i =2;
        }
        else  if(VarAdrToUI16(ADDR_ADPOSI_REAL2) > uDevision1*5)
        {
            if(VarAdrToUI16(ADDR_ADPOSI_REAL3) <= uDevision2)
            {
                if(VarAdrToUI16(ACT_STATE) ==2)
                    i =8;
                else
                    i =1;
            }
            else  if((VarAdrToUI16(ADDR_ADPOSI_REAL3) > uDevision2) &&(VarAdrToUI16(ADDR_ADPOSI_REAL3) <= uDevision2*3))
            {
                if(VarAdrToUI16(ACT_STATE) ==2)
                    i =9;
                else
                    i =11;
            }
            else  if(VarAdrToUI16(ADDR_ADPOSI_REAL3) > uDevision2*3)
                i =10;
        }
        if(i != cClampPicNo)
        {
            cClampPicNo = i;
            wAdClamp =VarAdrToUI16(ADDR_ADPOSI_REAL2);
            wAdEject =VarAdrToUI16(ADDR_ADPOSI_REAL3);
        }
    }
    VarAdrSetInt(CLAMP_IMG_VAL, cClampPicNo);
}

/*****************************************
*Object   :主画面动作值

******************************************/
static void MainActPng()
{
    UI32 dwAct1 = VarAdrToInt(ACT_ACTNAME1) >> 8;
    UI32 dwAct2 = VarAdrToInt(ACT_ACTNAME2) >> 8;
    UI32 dwAct3 = VarAdrToInt(ACT_ACTNAME3) >> 8;//20210220
    UI32 dwAct4 = VarAdrToInt(ACT_ACTNAME4) >> 8;

    //开模
    if((dwAct1 >= ACT_OPEN_BEGIN && dwAct1 <= ACT_OPEN_END)
            ||(dwAct2 >= ACT_OPEN_BEGIN && dwAct2 <= ACT_OPEN_END)
            ||(dwAct3 >= ACT_OPEN_BEGIN && dwAct3 <= ACT_OPEN_END)
            ||(dwAct4 >= ACT_OPEN_BEGIN && dwAct4 <= ACT_OPEN_END)){
        VarAdrSetInt(OPEN_BAK_IMG_VAL, TWO_IMG);
    }
    else{
        VarAdrSetInt(OPEN_BAK_IMG_VAL, ONE_IMG);
    }
    //关模
    if((dwAct1 >= ACT_CLOSE_BEGIN && dwAct1 <= ACT_CLOSE_END)
            ||(dwAct2 >= ACT_CLOSE_BEGIN && dwAct2 <= ACT_CLOSE_END)
            ||(dwAct3 >= ACT_CLOSE_BEGIN && dwAct3 <= ACT_CLOSE_END)
            ||(dwAct4 >= ACT_CLOSE_BEGIN && dwAct4 <= ACT_CLOSE_END)){
        VarAdrSetInt(CLOSE_BAK_IMG_VAL, TWO_IMG);
    }
    else{
        VarAdrSetInt(CLOSE_BAK_IMG_VAL, ONE_IMG);
    }
    //射出,保压
    if((dwAct1 >= ACT_INJECT_BEGIN && dwAct1 <= ACT_INJECT_END)
            ||(dwAct2 >= ACT_INJECT_BEGIN && dwAct2 <= ACT_INJECT_END)
            ||(dwAct3 >= ACT_INJECT_BEGIN && dwAct3 <= ACT_INJECT_END)
            ||(dwAct4 >= ACT_INJECT_BEGIN && dwAct4 <= ACT_INJECT_END)
            ||(dwAct1 >= ACT_HOLD_BEGIN && dwAct1 <= ACT_HOLD_END)
            ||(dwAct2 >= ACT_HOLD_BEGIN && dwAct2 <= ACT_HOLD_END)
            ||(dwAct3 >= ACT_HOLD_BEGIN && dwAct3 <= ACT_HOLD_END)
            ||(dwAct4 >= ACT_HOLD_BEGIN && dwAct4 <= ACT_HOLD_END)){
        VarAdrSetInt(INJECT_BAK_IMG_VAL, TWO_IMG);
    }
    else{
        VarAdrSetInt(INJECT_BAK_IMG_VAL, ONE_IMG);
    }
    //储料
    if(((dwAct1 >= ACT_CHARGE_BEGIN && dwAct1 <= ACT_CHARGE_END) || (dwAct1 >= 0x101600 && dwAct1 <= 0x101810))
            ||((dwAct2 >= ACT_CHARGE_BEGIN && dwAct2 <= ACT_CHARGE_END) || (dwAct2 >= 0x101600 && dwAct2 <= 0x101810))
            ||((dwAct3 >= ACT_CHARGE_BEGIN && dwAct3 <= ACT_CHARGE_END) || (dwAct3 >= 0x101600 && dwAct3 <= 0x101810))
            ||((dwAct4 >= ACT_CHARGE_BEGIN && dwAct4 <= ACT_CHARGE_END) || (dwAct4 >= 0x101600 && dwAct4 <= 0x101810))){
        VarAdrSetInt(CHARGE_BAK_IMG_VAL, TWO_IMG);
    }
    else{
        VarAdrSetInt(CHARGE_BAK_IMG_VAL, ONE_IMG);
    }

    //射退
    if((dwAct1 >= ACT_SKBACK_BEGIN && dwAct1 <= ACT_SKBACK_END)
            ||(dwAct2 >= ACT_SKBACK_BEGIN && dwAct2 <= ACT_SKBACK_END)
            ||(dwAct3 >= ACT_SKBACK_BEGIN && dwAct3 <= ACT_SKBACK_END)
            ||(dwAct4 >= ACT_SKBACK_BEGIN && dwAct4 <= ACT_SKBACK_END)){
        VarAdrSetInt(SUCK_BAK_IMG_VAL, TWO_IMG);
    }
    else{
        VarAdrSetInt(SUCK_BAK_IMG_VAL, ONE_IMG);
    }
    //座台
    if((dwAct1 >= ACT_NOZADV_BEGIN && dwAct1 <= ACT_NOZADV_END)
            ||(dwAct2 >= ACT_NOZADV_BEGIN && dwAct2 <= ACT_NOZADV_END)
            ||(dwAct3 >= ACT_NOZADV_BEGIN && dwAct3 <= ACT_NOZADV_END)
            ||(dwAct4 >= ACT_NOZADV_BEGIN && dwAct4 <= ACT_NOZADV_END)
            ||(dwAct1 >= ACT_NOZRET_BEGIN && dwAct1 <= ACT_NOZRET_END)
            ||(dwAct2 >= ACT_NOZRET_BEGIN && dwAct2 <= ACT_NOZRET_END)
            ||(dwAct3 >= ACT_NOZRET_BEGIN && dwAct3 <= ACT_NOZRET_END)
            ||(dwAct4 >= ACT_NOZRET_BEGIN && dwAct4 <= ACT_NOZRET_END)){
        VarAdrSetInt(NOZZLE_BAK_IMG_VAL, TWO_IMG);
    }
    else{
        VarAdrSetInt(NOZZLE_BAK_IMG_VAL, ONE_IMG);
    }
    //托模
    if(((dwAct1 >= ACT_LEJECTADV_BEGIN && dwAct1 <= ACT_LEJECTADV_END) ||(dwAct1 >= ACT_LEJECTRET_BEGIN && dwAct1 <= ACT_LEJECTRET_END)
        || (dwAct1 >= ACT_REJECTADV_BEGIN && dwAct1 <= ACT_REJECTADV_END) ||(dwAct1 >= ACT_REJECTRET_BEGIN && dwAct1 <= ACT_REJECTRET_END))
            ||((dwAct2 >= ACT_LEJECTADV_BEGIN && dwAct2 <= ACT_LEJECTADV_END) ||(dwAct2 >= ACT_LEJECTRET_BEGIN && dwAct2 <= ACT_LEJECTRET_END)
               || (dwAct2 >= ACT_REJECTADV_BEGIN && dwAct2 <= ACT_REJECTADV_END) ||(dwAct2 >= ACT_REJECTRET_BEGIN && dwAct2 <= ACT_REJECTRET_END))
            ||((dwAct3 >= ACT_LEJECTADV_BEGIN && dwAct3 <= ACT_LEJECTADV_END) ||(dwAct3 >= ACT_LEJECTRET_BEGIN && dwAct3 <= ACT_LEJECTRET_END)
               || (dwAct3 >= ACT_REJECTADV_BEGIN && dwAct3 <= ACT_REJECTADV_END) ||(dwAct3 >= ACT_REJECTRET_BEGIN && dwAct3 <= ACT_REJECTRET_END))
            ||((dwAct4 >= ACT_LEJECTADV_BEGIN && dwAct4 <= ACT_LEJECTADV_END) ||(dwAct4 >= ACT_LEJECTRET_BEGIN && dwAct4 <= ACT_LEJECTRET_END)
               || (dwAct4 >= ACT_REJECTADV_BEGIN && dwAct4 <= ACT_REJECTADV_END) ||(dwAct4 >= ACT_REJECTRET_BEGIN && dwAct4 <= ACT_REJECTRET_END)))
    {
        VarAdrSetInt(EJECT_BAK_IMG_VAL, TWO_IMG);
    }
    else{
        VarAdrSetInt(EJECT_BAK_IMG_VAL, ONE_IMG);
    }
    //中子
    if((dwAct1 >= ACT_CORE_BEGIN && dwAct1 <= ACT_CORE_END)
            ||(dwAct2 >= ACT_CORE_BEGIN && dwAct2 <= ACT_CORE_END)
            ||(dwAct3 >= ACT_CORE_BEGIN && dwAct3 <= ACT_CORE_END)
            ||(dwAct4 >= ACT_CORE_BEGIN && dwAct4 <= ACT_CORE_END)){
        VarAdrSetInt(CORE_BAK_IMG_VAL, TWO_IMG);
    }
    else{
        VarAdrSetInt(CORE_BAK_IMG_VAL, ONE_IMG);
    }
    //吹气
    if((dwAct1 >= ACT_AIR_BEGIN && dwAct1 <= ACT_AIR_END)
            ||(dwAct2 >= ACT_AIR_BEGIN && dwAct2 <= ACT_AIR_END)
            ||(dwAct3 >= ACT_AIR_BEGIN && dwAct3 <= ACT_AIR_END)
            ||(dwAct4 >= ACT_AIR_BEGIN && dwAct4 <= ACT_AIR_END)){
        VarAdrSetInt(AIR_BAK_IMG_VAL, TWO_IMG);
    }
    else{
        VarAdrSetInt(AIR_BAK_IMG_VAL, ONE_IMG);
    }
    //冷却
    if((dwAct1 >= ACT_COOL_BEGIN && dwAct1 <= ACT_COOL_END)
            ||(dwAct2 >= ACT_COOL_BEGIN && dwAct2 <= ACT_COOL_END)
            ||(dwAct3 >= ACT_COOL_BEGIN && dwAct3 <= ACT_COOL_END)
            ||(dwAct4 >= ACT_COOL_BEGIN && dwAct4 <= ACT_COOL_END)){
        VarAdrSetInt(COOL_BAK_IMG_VAL, TWO_IMG);
    }
    else{
        VarAdrSetInt(COOL_BAK_IMG_VAL, ONE_IMG);
    }
}

/*************************************温度界面-区别标准液压7-8-10寸*****************************************/

/*****************************************
*Subject  :
*Condition:
*Object   :温度1-16段图片索引
*Result   :
******************************************/

static void Temp16ImgVal()
{
    UI16 i;
    UI16 temp_num = 9;
    const UI16 temp_extra_num = 7;


    for (i = 0; i < temp_num; ++i)
    {
        if (VarAdrToInt(ADDR_TEMP_RAMP1 + i) == 0)
        {
            VarAdrSetInt(TEMP1_IMG_VAL + i, ONE_IMG);
        }
        else
        {
            VarAdrSetInt(TEMP1_IMG_VAL + i, VarAdrToInt(ADDR_TEMP_STATUS1 + i) + ONE_IMG);
        }
    }

    for (i = 0; i < temp_extra_num; ++i)
    {
        if((VarAdrToInt(SYS_FL_CTRLMODEL) == 0x3100) || (VarAdrToInt(SYS_FL_CTRLMODEL) == 0x2100))
        {
            if (VarAdrToInt(TEMPCTRL2_MACH_DATEMP_RAMP1 + i) == 0)
            {
                VarAdrSetInt(TEMP_EXTRA1_IMG_VAL + i, ONE_IMG);
            }
            else
            {
                VarAdrSetInt(TEMP_EXTRA1_IMG_VAL + i, VarAdrToInt(TEMPCTRL2_STATE_FL_TEMPSTATUS1 + i) + ONE_IMG);
            }

        }
        else if((VarAdrToInt(SYS_FL_CTRLMODEL) == 0xA620) || (VarAdrToInt(SYS_FL_CTRLMODEL) == 0xA920))
        {
            if (VarAdrToInt(ADDR_TEMP_RAMP8 + i) == 0)
            {
                VarAdrSetInt(TEMP_EXTRA1_IMG_VAL + i, ONE_IMG);
            }
            else
            {
                VarAdrSetInt(TEMP_EXTRA1_IMG_VAL + i, VarAdrToInt(ADDR_TEMP_STATUS8 + i) + ONE_IMG);
            }
        }
        else
        {
            if (VarAdrToInt(ADDR_TEMP_RAMP8 + i) == 0)
            {
                VarAdrSetInt(TEMP_EXTRA1_IMG_VAL + i, ONE_IMG);
            }
            else
            {
                VarAdrSetInt(TEMP_EXTRA1_IMG_VAL + i, VarAdrToInt(ADDR_TEMP_STATUS8 + i) + ONE_IMG);
            }
        }
    }

    if((VarAdrToInt(SYS_FL_CTRLMODEL) == 0x3100))
    {
        VarAdrSetInt(TEMP_MAIN_IMG_VISIBLE, FALSE);
    }
    else
    {
        VarAdrSetInt(TEMP_MAIN_IMG_VISIBLE, TRUE);
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :温度6-9段是否可见/可用: bit 0-3
*Result   :
******************************************/

static void Temp6to9Ctl()
{
    int i;
    UI16 bit_for_set;
    UI16 bit_visible = 0;
    UI16 bit_enable = 0;
    UI16 bit_setenable = 0;

    bit_visible = VarAdrToUI16(TEMP6_9_VISIBLE);
    bit_enable = VarAdrToUI16(TEMP6_9_ENABLE);
    bit_setenable = VarAdrToUI16(TEMP_SET6_9_ENABLE);

    //6-9 visible
    if (VarAdrToInt(SYS_FL_CTRLMODEL) == 0xA620)
    {
        if ((VarAdrToInt(SYS_FL_MACH_CODE4) & 0x0010) != 0)
        {
            VarAdrSetInt(TEMP6_9_VISIBLE, setzero(bit_visible, 0));
        }
        else
        {
            VarAdrSetInt(TEMP6_9_VISIBLE, setone(bit_visible, 0));
        }
        VarAdrSetInt(TEMP6_9_VISIBLE, setzero(bit_visible, 1));
        VarAdrSetInt(TEMP6_9_VISIBLE, setzero(bit_visible, 2));
        VarAdrSetInt(TEMP6_9_VISIBLE, setzero(bit_visible, 3));
    }
    else if ((VarAdrToInt(SYS_FL_CTRLMODEL) ==  0xA920)||(VarAdrToInt(SYS_FL_CTRLMODEL) ==  0x2100))
    {
        VarAdrSetInt(TEMP6_9_VISIBLE, setone(bit_visible, 0));
        VarAdrSetInt(TEMP6_9_VISIBLE, setzero(bit_visible, 2));
        VarAdrSetInt(TEMP6_9_VISIBLE, setzero(bit_visible, 3));
        if ((VarAdrToInt(SYS_FL_MACH_CODE4) & 0x0010) != 0)
        {
            VarAdrSetInt(TEMP6_9_VISIBLE, setzero(bit_visible, 1));
        }
        else
        {
            VarAdrSetInt(TEMP6_9_VISIBLE, setone(bit_visible, 1));
        }
    }
    else if (VarAdrToInt(SYS_FL_CTRLMODEL) == 0x3100)
    {
        VarAdrSetInt(TEMP6_9_VISIBLE, setone(bit_visible, 0));
        VarAdrSetInt(TEMP6_9_VISIBLE, setone(bit_visible, 1));
        VarAdrSetInt(TEMP6_9_VISIBLE, setone(bit_visible, 2));
        if ((VarAdrToInt(SYS_FL_MACH_CODE4) & 0x0010) != 0)
        {
            VarAdrSetInt(TEMP6_9_VISIBLE, setzero(bit_visible, 3));
        }
        else
        {
            VarAdrSetInt(TEMP6_9_VISIBLE, setone(bit_visible, 3));
        }
    }
    else
    {
        VarAdrSetInt(TEMP6_9_VISIBLE, setone(bit_visible, 0));
        VarAdrSetInt(TEMP6_9_VISIBLE, setone(bit_visible, 1));
        VarAdrSetInt(TEMP6_9_VISIBLE, setzero(bit_visible, 2));
        VarAdrSetInt(TEMP6_9_VISIBLE, setzero(bit_visible, 3));
    }

    //6-9 enable
    if (VarAdrToInt(SYS_FL_CTRLMODEL) == 0xA620)
    {
        if ((VarAdrToInt(SYS_FL_MACH_CODE4) & 0x0010) != 0)
        {
            VarAdrSetInt(TEMP6_9_ENABLE, setzero(bit_enable, 0));
        }
        else
        {
            VarAdrSetInt(TEMP6_9_ENABLE, setone(bit_enable, 0));
        }
        VarAdrSetInt(TEMP6_9_ENABLE, setzero(bit_enable, 1));
    }
    else if ((VarAdrToInt(SYS_FL_CTRLMODEL) ==  0xA920)||(VarAdrToInt(SYS_FL_CTRLMODEL) ==  0x2100))
    {
        VarAdrSetInt(TEMP6_9_ENABLE, setone(bit_enable, 0));
        if ((VarAdrToInt(SYS_FL_MACH_CODE4) & 0x0010) != 0)
        {
            VarAdrSetInt(TEMP6_9_ENABLE, setzero(bit_enable, 1));
        }
        else
        {
            VarAdrSetInt(TEMP6_9_ENABLE, setone(bit_enable, 1));
        }
    }
    else if (VarAdrToInt(SYS_FL_CTRLMODEL) == 0x3100)
    {
        VarAdrSetInt(TEMP6_9_ENABLE, setone(bit_enable, 0));
        VarAdrSetInt(TEMP6_9_ENABLE, setone(bit_enable, 1));
        VarAdrSetInt(TEMP6_9_ENABLE, setone(bit_enable, 2));
        if ((VarAdrToInt(SYS_FL_MACH_CODE4) & 0x0010) != 0)
        {
            VarAdrSetInt(TEMP6_9_ENABLE, setzero(bit_enable, 3));
        }
        else
        {
            VarAdrSetInt(TEMP6_9_ENABLE, setone(bit_enable, 3));
        }
    }
    else
    {
        VarAdrSetInt(TEMP6_9_ENABLE, setone(bit_enable, 0));
        VarAdrSetInt(TEMP6_9_ENABLE, setone(bit_enable, 1));
        VarAdrSetInt(TEMP6_9_ENABLE, setone(bit_enable, 2));
        VarAdrSetInt(TEMP6_9_ENABLE, setone(bit_enable, 3));
    }

    //6-9 set enable
    bit_for_set = VarAdrToUI16(TEMP6_9_ENABLE);
    for(i = 0; i < 4; ++i)
    {
        if(getbit(bit_for_set, i) && VarAdrToInt(ADDR_TEMP_RAMP6 + i))
        {
            VarAdrSetInt(TEMP_SET6_9_ENABLE, setone(bit_setenable, i));
        }
        else
        {
            VarAdrSetInt(TEMP_SET6_9_ENABLE, setzero(bit_setenable, i));
        }
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :扩展温度1-7段
*Result   :
******************************************/

static void TempExtraCtl()
{
    if((VarAdrToInt(SYS_FL_MACH_CODE54) & 0x0400) != 0)
    {
        if((VarAdrToInt(SYS_FL_CTRLMODEL) == 0x3100) || (VarAdrToInt(SYS_FL_CTRLMODEL) == 0x2100))
        {
            VarAdrSetInt(TEMP_PART1_ENABLE, FALSE);
            VarAdrSetInt(TEMP_PART2_ENABLE, TRUE);
        }
        else if((VarAdrToInt(SYS_FL_CTRLMODEL) == 0xA620) || (VarAdrToInt(SYS_FL_CTRLMODEL) == 0xA920))
        {
            VarAdrSetInt(TEMP_PART1_ENABLE, TRUE);
            VarAdrSetInt(TEMP_PART2_ENABLE, FALSE);
        }
        else
        {
            VarAdrSetInt(TEMP_PART1_ENABLE, TRUE);
            VarAdrSetInt(TEMP_PART2_ENABLE, FALSE);
        }

    }
    else
    {
        VarAdrSetInt(TEMP_PART1_ENABLE, FALSE);
        VarAdrSetInt(TEMP_PART2_ENABLE, FALSE);
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :温度参数一画面切换
*Result   :
******************************************/

static void TempParaExchange()
{
    UI16 bit = 0;
    VarAdrSetInt(TEMP_PARAM_EXCHANGE, 0);

    if(VarAdrToUI16(SYS_FL_MACH_CODE54)&0x0400)
    {
        if(VarAdrToUI16(SYS_FL_CTRLMODEL) == 0xA620)
        {
            VarAdrSetInt(TEMP_PARAM_EXCHANGE, setone(bit, 3));
        }
        else if(VarAdrToUI16(SYS_FL_CTRLMODEL) == 0xA920)
        {
            VarAdrSetInt(TEMP_PARAM_EXCHANGE, setone(bit, 4));
        }
        else if(VarAdrToUI16(SYS_FL_CTRLMODEL) == 0x2100)
        {
            VarAdrSetInt(TEMP_PARAM_EXCHANGE, setone(bit, 1));
        }
        else if(VarAdrToUI16(SYS_FL_CTRLMODEL) == 0x3100)
        {
            VarAdrSetInt(TEMP_PARAM_EXCHANGE, setone(bit, 2));
        }
        else
        {
            VarAdrSetInt(TEMP_PARAM_EXCHANGE, setone(bit, 0));
        }
    }
}

/*****************************托模界面-Eject*********************************************/
/*****************************************
*Subject  :
*Condition:
*Object   :吹气多次是否使用
*Result   :
******************************************/

static void BlowAgainCtl()
{
    int i =0;
    const int blow_num = 14;
    UI16 bit = 0;

    if((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x8400)
    {
        VarAdrSetInt(MULTIPLE_ENABLE, TRUE);
        for(i = 0; i < blow_num; ++i)
        {
            bit = VarAdrToUI16(BLOW_AGAIN_1_14_ENABLE);
            if(((VarAdrToInt(AIRBLOW1_14AGAIN) >> i)& 0x01) != 0)
            {
                VarAdrSetInt(BLOW_AGAIN_1_14_ENABLE, setone(bit, i));
            }
            else
            {
                VarAdrSetInt(BLOW_AGAIN_1_14_ENABLE, setzero(bit, i));
            }
        }
    }
    else
    {
        VarAdrSetInt(MULTIPLE_ENABLE, FALSE);
        for(i = 0; i < blow_num; ++i)
        {
            VarAdrSetInt(BLOW_AGAIN_1_14_ENABLE, FALSE);
        }
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :1-14延迟计时和动作时间是否使用
*Result   :
******************************************/

static void DlyTmActTmEnable()
{
    int i =0;
    const int blow_num = 14;
    UI16 bit = 0;

    UI16 tmpbit=0;//20241205 chj 如果胶口针阀使用 且 针阀吹气同步输出未选择不使用
    int tempval = 0,tempval2 = 0;
    tempval = VarAdrToInt(d_inject1_MACHSET_FL_INJSPC_AIR);
    tempval2 = VarAdrToInt(d_inject1_MACHSET_FL_INJSPC_AIR1);
    tmpbit = VarAdrToUI16(p_PP_MACHSET_INJSPC_AIRBLOWENABLE);
    UI32 wAddr = FL_BLAST1;//20241210 chj 28组吹气,进入到吹气2画面，复用吹气1画面的控制变灰的变量。
    if(b_in_ariblowpage2)//20241210 chj 28组吹气,进入到吹气2画面，复用吹气1画面的控制变灰的变量。
    {
        wAddr = d_airblow1_MOLDSET_FL_BLAST2;
        tempval = 0;//吹气15-28，没有 胶口针阀使用 且 针阀吹气同步输出未选择不使用
        tempval2 = 0;
    }

    if(VarAdrToInt(SYS_FL_MACH_CODE50) & 0x2000)//20230518 chj 1000->2000
    {
        for(i = 0; i < blow_num; ++i)
        {
            bit = VarAdrToUI16(DLYTM_ACTTM_1_14_ENABLE);
            if(VarAdrToInt(wAddr + i*0x100000) == 5)
            {
                VarAdrSetInt(DLYTM_ACTTM_1_14_ENABLE, setzero(bit, i));
            }
            else
            {
                if(VarAdrToInt(d_inject1_MACHSET_FL_INj_SPC) == 1 && ( (tempval ==(i+1)) || (tempval2 ==(i+1)) ))//20241205 chj 如果胶口针阀使用 且 针阀吹气同步输出未选择不使用
                {
                    VarAdrSetInt(DLYTM_ACTTM_1_14_ENABLE, setzero(bit,i));
                    VarAdrSetInt(p_PP_MACHSET_INJSPC_AIRBLOWENABLE, setzero(tmpbit, i));
                }
                else
                {
                    VarAdrSetInt(DLYTM_ACTTM_1_14_ENABLE, setone(bit,i));
                    VarAdrSetInt(p_PP_MACHSET_INJSPC_AIRBLOWENABLE, setone(tmpbit, i));
                }
            }
        }
    }
    else
    {
        for(i = 0; i < blow_num; ++i)
        {
            bit = VarAdrToUI16(DLYTM_ACTTM_1_14_ENABLE);

            if(VarAdrToInt(d_inject1_MACHSET_FL_INj_SPC) == 1 && ( (tempval ==(i+1)) || (tempval2 ==(i+1)) ))//20241205 chj 如果胶口针阀使用 且 针阀吹气同步输出未选择不使用
            {
                VarAdrSetInt(DLYTM_ACTTM_1_14_ENABLE, setzero(bit,i));
                VarAdrSetInt(p_PP_MACHSET_INJSPC_AIRBLOWENABLE, setzero(tmpbit, i));
            }
            else
            {
                VarAdrSetInt(DLYTM_ACTTM_1_14_ENABLE, setone(bit,i));
                VarAdrSetInt(p_PP_MACHSET_INJSPC_AIRBLOWENABLE, setone(tmpbit, i));
            }
        }
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :页面中手动吹气按键选择和中子按键选择是否可见  目前用于芯动
*Result   :
******************************************/
static void BlowCoreKeyVisibl()
{
    if(PanelCfgStyle() == TIANJIAN_TYPE && GetPanelType() == PANEL_H10)
    {
        VarAdrSetInt(p_PP_OBJ_EJ_CORE_KEY_VISABLE,FALSE);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_EJ_CORE_KEY_VISABLE,TRUE);
    }

}

/*****************************射出界面-Inject*********************************************/
/*****************************************
*Subject  :
*Condition:
*Object   :阶梯曲线
*Result   :
******************************************/
static void HoldCurve()
{
    UI16 t1, t2, t3, t4, t5, total;
    t1 = VarAdrToUI16(INJECT_TM_HOLD1)/10;
    t2 = VarAdrToUI16(INJECT_TM_HOLD2)/10;
    t3 = VarAdrToUI16(INJECT_TM_HOLD3)/10;
    t4 = VarAdrToUI16(INJECT_TM_HOLD4)/10;
    t5 = VarAdrToUI16(INJECT_TM_HOLD5)/10;
    total = t1 + t2 + t3 + t4 + t5;

    VarAdrSetInt(HOLD_TM_TOTAL_VAL, total);
    VarAdrSetInt(HOLD_TM1_VAL, t1);
    VarAdrSetInt(HOLD_TM2_VAL, t1+t2);
    VarAdrSetInt(HOLD_TM3_VAL, t1+t2+t3);
    VarAdrSetInt(HOLD_TM4_VAL, t1+t2+t3+t4);
    VarAdrSetInt(HOLD_TM5_VAL, total);
}

/*****************************日月年产量*********************************************/
static void DatePowerVisible()
{
    if(VarAdrToInt(SYS_FL_TEMPER) & 0x2000)
    {
        VarAdrSetInt(DATE_POWER_VISIBLE, TRUE);
    }
    else {
        VarAdrSetInt(DATE_POWER_VISIBLE, FALSE);
    }
}

/**************************托膜参数界面变量模块-EJ*****************************/
/*****************************************
*Subject  :
*Condition:
*Object   :托进保持功能是否可用
*Result   :
******************************************/

static void EjAdvHoldEnable()
{
    if(VarAdrToInt(SYS_FL_MACH_CODE52) & 0x400)
    {
        VarAdrSetInt(EJ_ADV_HOLD_ENABLE, TRUE);
    }
    else
    {
        VarAdrSetInt(EJ_ADV_HOLD_ENABLE, FALSE);
    }
}

/**************************储料功能界面变量模块-CHG*****************************/
/*****************************************
*Subject  :
*Condition:
*Object   :漏油检测是否可用、真空发射器是否可用--仅限P10EHS2_M791--兼容威力仕特殊机
*Result   :
******************************************/

static void WeilishiEnable()
{
    if((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x7A00)
    {
        VarAdrSetInt(VACUUM_ENABLE, TRUE);
        VarAdrSetInt(OIL_CHECK_ENABLE, TRUE);
    }
    else
    {
        VarAdrSetInt(VACUUM_ENABLE, FALSE);
        VarAdrSetInt(OIL_CHECK_ENABLE, FALSE);
    }
}

/*****************************以下为G10竖屏开发新增**************************************/

/**************************中子界面变量模块-CORE*****************************/
/*****************************************
*Subject  :
*Condition:
*Object   :特殊中子功能是否可用
*Result   :
******************************************/
static void SpecialCoreFuncEnable()
{
    if (VarAdrToInt(d_coreA_MOLDSET_FL_CORE_CTRL) != 0)
    {
        VarAdrSetInt(SPECIAL_CORE_FUNC_ENABLE, TRUE);
    }
    else
    {
        VarAdrSetInt(SPECIAL_CORE_FUNC_ENABLE, FALSE);
    }
}

/**************************机械手界面变量模块-ROBOT*****************************/
/*****************************************
*Subject  :
*Condition:
*Object   :途中机械手输出时间小数位和最大值 途中机械手启动位置最小值
*Result   :
******************************************/
static void WayRobotOutputTmVal()
{
    //20211231 dyl 途中机械手输出时间是否可见
    if((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x8B00)
    {
        VarAdrSetInt(p_PP_OBJ_RBT_OUTPUT_TM_VISIBLE,1);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_RBT_OUTPUT_TM_VISIBLE,0);
    }
    if(VarAdrToInt(SYS_FL_MACH_CODE52) & 0x0004)
    {
        VarAdrSetInt(WAY_ROBOT_OUTPUT_TM_DOT,2);
        VarAdrSetInt(WAY_ROBOT_OUTPUT_TM_MAX,9999);
    }
    else
    {
        VarAdrSetInt(WAY_ROBOT_OUTPUT_TM_DOT,1);
        VarAdrSetInt(WAY_ROBOT_OUTPUT_TM_MAX,999);
    }

    if(PushtableType())
    {
        VarAdrSetInt(WAYROBOTPOS_MIN,VarAdrToInt(ADPOSI_CLOSELOW)/10);
    }
    else
    {
        VarAdrSetInt(WAYROBOTPOS_MIN,VarAdrToInt(ADPOSI_CLOSELOW));
    }
}


/**************************扩展温度界面变量模块-Tempxx*****************************/
/*****************************************
*Subject  :
*Condition:
*Object   :扩展温度的状态显示
*Result   :
******************************************/

static void TempCtlStatusVal()
{
    UI16 tempseg;
    UI16 i, j;

    //tempctl_1
    tempseg = VarAdrToInt(d_TempCtrlComm1_MACHSET_DATEMP_CH_NUM);
    for(i = 0; i < tempseg; ++i)
    {
        if((VarAdrToInt(d_TempCtrlComm1_STATE_FL_TEMPSTATUS_2 + i) & 0x00ff) == 0)
        {
            if(VarAdrToInt(d_TempCtrlComm1_MACHSET_DATEMP_FUNC_2 + i) == 0)
                VarAdrSetInt(TEMPCTRL1_STATUS1_VAL+i, 6);
            else
                VarAdrSetInt(TEMPCTRL1_STATUS1_VAL+i, 1);
        }
        else
        {
            if(VarAdrToInt(d_TempCtrlComm1_MACHSET_DATEMP_FUNC_2 + i) == 0)
                VarAdrSetInt(TEMPCTRL1_STATUS1_VAL+i, 6);
            for(j = 0; j < 5; ++j)
            {
                if((VarAdrToInt(d_TempCtrlComm1_STATE_FL_TEMPSTATUS_2 + i) >> j) & 0x1)
                    VarAdrSetInt(TEMPCTRL1_STATUS1_VAL+i, j+1);
            }
        }
    }

    //tempctl_2
    tempseg = VarAdrToInt(d_TempCtrlComm2_MACHSET_DATEMP_CH_NUM);
    for(i = 0; i < tempseg; ++i)
    {
        if((VarAdrToInt(d_TempCtrlComm2_STATE_FL_TEMPSTATUS_2 + i) & 0x00ff) == 0)
        {
            if(VarAdrToInt(d_TempCtrlComm2_MACHSET_DATEMP_FUNC_2 + i) == 0)
                VarAdrSetInt(TEMPCTRL2_STATUS1_VAL+i, 6);
            else
                VarAdrSetInt(TEMPCTRL2_STATUS1_VAL+i, 1);
        }
        else
        {
            if(VarAdrToInt(d_TempCtrlComm2_MACHSET_DATEMP_FUNC_2 + i) == 0)
                VarAdrSetInt(TEMPCTRL2_STATUS1_VAL+i, 6);
            for(j = 0; j < 5; ++j)
            {
                if((VarAdrToInt(d_TempCtrlComm2_STATE_FL_TEMPSTATUS_2 + i) >> j) & 0x1)
                    VarAdrSetInt(TEMPCTRL2_STATUS1_VAL+i, j+1);
            }
        }
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :落料口温度是否可见
*Result   :
******************************************/
static void MouthVisible()
{
    int i;
    int choose = 0;
    BOOL mouth_show = FALSE;

    for(i = 0; i < 9; ++i)
    {
        choose = VarAdrToInt(d_TempCtrlComm1_MACHSET_DATEMP_FUNC_2 + i);
        if(choose == 6)
        {
            mouth_show = TRUE;
        }
    }

    if(mouth_show)
    {
        VarAdrSetInt(MOUTH_VISIBLE, TRUE);
    }
    else
    {
        VarAdrSetInt(MOUTH_VISIBLE, FALSE);
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :温度的状态显示
*Result   :
******************************************/
static void NormalTempImgVal()
{
    UI16 i;
    for(i = 0; i < 9; ++i)
    {
        if(i == 0)
        {
            if(VarAdrToInt(d_tempctrl1_STATE_FL_HEATERSTATUS) == 0 || (VarAdrToInt(d_tempctrl1_MACHSET_DATEMP_RAMP_1) == 0
                                                                       || VarAdrToInt(d_tempctrl1_MOLDSET_TM_RESPONSELESSCYCLE) == 0
                                                                       || VarAdrToInt(d_tempctrl1_MOLDSET_DACENT_HEATERON) == 0))
            {
                VarAdrSetInt(NORMAL_TEMP1_IMG_VAL, 2);
            }
            else
            {
                VarAdrSetInt(NORMAL_TEMP1_IMG_VAL, VarAdrToInt(d_tempctrl1_STATE_FL_TEMPSTATUS_1));
            }
        }
        else
        {
            if(VarAdrToInt(d_tempctrl1_STATE_FL_HEATERSTATUS) == 0)
            {
                VarAdrSetInt(NORMAL_TEMP1_IMG_VAL+i, 2);
            }
            else
            {
                if(VarAdrToInt(d_tempctrl1_MACHSET_DATEMP_RAMP_1+i) == 0)
                {
                    VarAdrSetInt(NORMAL_TEMP1_IMG_VAL+i, 2);
                }
                else
                {
                    VarAdrSetInt(NORMAL_TEMP1_IMG_VAL+i, VarAdrToInt(d_tempctrl1_STATE_FL_TEMPSTATUS_1+i));
                }
            }
        }
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :料管温度偏差可用
*Result   :
******************************************/
static void BarrelCoolBiasEnable()
{
    if(VarAdrToInt(p_PP_MACHSET_E502_TEMP_AB)) //料管B
    {
        VarAdrSetInt(BARRELCOOLBIAS_ENABLE,VarAdrToInt(d_TempCtrlComm2_MOLDSET_FL_TEMPCOOL));
    }
    else //料管A
    {
        VarAdrSetInt(BARRELCOOLBIAS_ENABLE,VarAdrToInt(d_TempCtrlComm1_MOLDSET_FL_TEMPCOOL));
    }
}


/**************************系统参数4界面-Sys_Para5*****************************/
/*****************************************
*Subject  :
*Condition:
*Object   :射出慢速启动速度小数位和最大值
*Result   :
******************************************/
static void InjSlowStartSpdVal()
{
    if(VarAdrToInt(SYS_FL_MACH_CODE5) & 0x0100)
    {
        VarAdrSetInt(INJSLOW_STARTSPD_DOT,1);
        VarAdrSetInt(INJSLOW_STARTSPD_MAX,VarAdrToInt(d_autoctrl1_MOLDSET_DAFLOW_INJECTLIMIT)*10+9);
    }
    else
    {
        VarAdrSetInt(INJSLOW_STARTSPD_DOT,0);
        VarAdrSetInt(INJSLOW_STARTSPD_MAX,VarAdrToInt(d_autoctrl1_MOLDSET_DAFLOW_INJECTLIMIT));
    }
}

/**************************开关模界面-Clamp_h*****************************/
/*****************************************
*Subject  :
*Condition:
*Object   :关模段数切换
*Result   :
******************************************/
static void ClsSegChg()
{
    if(PushtableType() && VarAdrToInt(d_clamp1_MOLDSET_FL_CLS3))
    {
        VarAdrSetInt(p_PP_OBJ_CLS3_PUSH_SHOW, TRUE);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_CLS3_PUSH_SHOW, FALSE);
    }

    if(PushtableType() && VarAdrToInt(p_PP_MACHSET_CLAMPVIEW_HIGH2_VISIBLE))
    {
        VarAdrSetInt(p_PP_OBJ_CLSH1_PUSH_SHOW, TRUE);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_CLSH1_PUSH_SHOW, FALSE);
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :开模2-5段切换
*Result   :
******************************************/
static void OpenSegChg()
{
    UI16 openseg = VarAdrToUI16(d_clamp1_MOLDSET_OPN_NUM);
    if(openseg == 5)
    {
        VarAdrSetInt(OPEN_2_ENABLE,TRUE);
        VarAdrSetInt(OPEN_3_ENABLE,TRUE);
        VarAdrSetInt(OPEN_4_ENABLE,TRUE);

        VarAdrSetInt(OPEN_END_POS_MIN, VarAdrToInt(CLAMP_ACTPARA_OPN4));
        VarAdrSetInt(OPEN4_POS_MIN, VarAdrToInt(CLAMP_ACTPARA_OPN3));
        VarAdrSetInt(OPEN3_POS_MAX, VarAdrToInt(CLAMP_ACTPARA_OPN4));
        VarAdrSetInt(OPEN3_POS_MIN, VarAdrToInt(ADPOSI_OPEN2));
        VarAdrSetInt(OPEN2_POS_MAX, VarAdrToInt(CLAMP_ACTPARA_OPN3));
        if (PushtableType())
        {
            VarAdrSetInt(OPEN1_POS_MAX, VarAdrToInt(ADPOSI_OPEN2) * 10);
            if(VarAdrToInt(ADPOSI_OPEN2) * 10 > 65000)//20241026 chj 修复开关模位置最大值超过65535.00问题一段位置最大值
            {
                VarAdrSetInt(p_PP_OBJ_OPEN1_POS_MAX_UI32, 65000);
            }
            else
            {
                VarAdrSetInt(p_PP_OBJ_OPEN1_POS_MAX_UI32, VarAdrToInt(ADPOSI_OPEN2) * 10);
            }
            VarAdrSetInt(OPEN2_POS_MIN, VarAdrToInt(ADPOSI_OPEN1) / 10);
        }
        else
        {
            VarAdrSetInt(OPEN1_POS_MAX, VarAdrToInt(ADPOSI_OPEN2));
            VarAdrSetInt(p_PP_OBJ_OPEN1_POS_MAX_UI32, VarAdrToInt(ADPOSI_OPEN2));//20241026 chj 修复开关模位置最大值超过65535.00问题一段位置最大值
            VarAdrSetInt(OPEN2_POS_MIN, VarAdrToInt(ADPOSI_OPEN1));
        }

        VarAdrSetStr(p_PP_OBJ_OPN4_TEXT, GetTextTran(TEXT_CLAMP_4));
        VarAdrSetStr(p_PP_OBJ_OPN3_TEXT, GetTextTran(TEXT_CLAMP_3));
    }
    else if(openseg == 4)
    {
        VarAdrSetInt(OPEN_2_ENABLE,FALSE);
        VarAdrSetInt(OPEN_3_ENABLE,TRUE);
        VarAdrSetInt(OPEN_4_ENABLE,TRUE);

        VarAdrSetInt(OPEN_END_POS_MIN, VarAdrToInt(CLAMP_ACTPARA_OPN4));
        VarAdrSetInt(OPEN4_POS_MIN, VarAdrToInt(CLAMP_ACTPARA_OPN3));
        VarAdrSetInt(OPEN3_POS_MAX, VarAdrToInt(CLAMP_ACTPARA_OPN4));
        if (PushtableType())
        {
            VarAdrSetInt(OPEN1_POS_MAX, VarAdrToInt(CLAMP_ACTPARA_OPN3) * 10);
            if(VarAdrToInt(CLAMP_ACTPARA_OPN3) * 10 > 65000)//20241026 chj 修复开关模位置最大值超过65535.00问题一段位置最大值一段位置最大值
            {
                VarAdrSetInt(p_PP_OBJ_OPEN1_POS_MAX_UI32, 65000);
            }
            else
            {
                VarAdrSetInt(p_PP_OBJ_OPEN1_POS_MAX_UI32, VarAdrToInt(CLAMP_ACTPARA_OPN3) * 10);
            }
            VarAdrSetInt(OPEN3_POS_MIN, VarAdrToInt(ADPOSI_OPEN1) / 10);
        }
        else
        {
            VarAdrSetInt(OPEN1_POS_MAX, VarAdrToInt(CLAMP_ACTPARA_OPN3));
            VarAdrSetInt(p_PP_OBJ_OPEN1_POS_MAX_UI32, VarAdrToInt(CLAMP_ACTPARA_OPN3));//20241026 chj 修复开关模位置最大值超过65535.00问题一段位置最大值一段位置最大值
            VarAdrSetInt(OPEN3_POS_MIN, VarAdrToInt(ADPOSI_OPEN1));
        }

        VarAdrSetStr(p_PP_OBJ_OPN4_TEXT, GetTextTran(TEXT_CLAMP_3));
        VarAdrSetStr(p_PP_OBJ_OPN3_TEXT, GetTextTran(TEXT_CLAMP_CLS2));
    }
    else if(openseg == 3)
    {
        VarAdrSetInt(OPEN_2_ENABLE,FALSE);
        VarAdrSetInt(OPEN_3_ENABLE,FALSE);
        VarAdrSetInt(OPEN_4_ENABLE,TRUE);

        VarAdrSetInt(OPEN_END_POS_MIN, VarAdrToInt(CLAMP_ACTPARA_OPN4));
        if (PushtableType())
        {
            VarAdrSetInt(OPEN1_POS_MAX, VarAdrToInt(CLAMP_ACTPARA_OPN4) * 10);
            if(VarAdrToInt(CLAMP_ACTPARA_OPN4) * 10 > 65000)//20241026 chj 修复开关模位置最大值超过65535.00问题一段位置最大值一段位置最大值
            {
                VarAdrSetInt(p_PP_OBJ_OPEN1_POS_MAX_UI32, 65000);
            }
            else
            {
                VarAdrSetInt(p_PP_OBJ_OPEN1_POS_MAX_UI32, VarAdrToInt(CLAMP_ACTPARA_OPN4) * 10);
            }
            VarAdrSetInt(OPEN4_POS_MIN, VarAdrToInt(ADPOSI_OPEN1) / 10);
        }
        else
        {
            VarAdrSetInt(OPEN1_POS_MAX, VarAdrToInt(CLAMP_ACTPARA_OPN4));
            VarAdrSetInt(p_PP_OBJ_OPEN1_POS_MAX_UI32, VarAdrToInt(CLAMP_ACTPARA_OPN4));//20241026 chj 修复开关模位置最大值超过65535.00问题一段位置最大值一段位置最大值
            VarAdrSetInt(OPEN4_POS_MIN, VarAdrToInt(ADPOSI_OPEN1));
        }

        VarAdrSetStr(p_PP_OBJ_OPN4_TEXT, GetTextTran(TEXT_CLAMP_CLS2));
    }
    else if(openseg == 2)
    {
        VarAdrSetInt(OPEN_2_ENABLE,FALSE);
        VarAdrSetInt(OPEN_3_ENABLE,FALSE);
        VarAdrSetInt(OPEN_4_ENABLE,FALSE);

        if (PushtableType())
        {
            VarAdrSetInt(OPEN1_POS_MAX, VarAdrToInt(ADPOSI_OPEN5) * 10);

            if(VarAdrToInt(ADPOSI_OPEN5) * 10 > 65000)//20241026 chj 修复开关模位置最大值超过65535.00问题一段位置最大值
            {
                VarAdrSetInt(p_PP_OBJ_OPEN1_POS_MAX_UI32, 65000);
            }
            else
            {
                VarAdrSetInt(p_PP_OBJ_OPEN1_POS_MAX_UI32, VarAdrToInt(ADPOSI_OPEN5) * 10);
            }
            VarAdrSetInt(OPEN_END_POS_MIN, VarAdrToInt(ADPOSI_OPEN1) / 10);
        }
        else
        {
            VarAdrSetInt(OPEN1_POS_MAX, VarAdrToInt(ADPOSI_OPEN5));
            VarAdrSetInt(p_PP_OBJ_OPEN1_POS_MAX_UI32, VarAdrToInt(ADPOSI_OPEN5));//20241026 chj 修复开关模位置最大值超过65535.00问题一段位置最大值
            VarAdrSetInt(OPEN_END_POS_MIN, VarAdrToInt(ADPOSI_OPEN1));
        }
    }

    if(PushtableType() && VarAdrToInt(OPEN_4_ENABLE))
    {
        VarAdrSetInt(p_PP_OBJ_OPN4_PUSH_SHOW, TRUE);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_OPN4_PUSH_SHOW, FALSE);
    }

    if(PushtableType() && VarAdrToInt(OPEN_3_ENABLE))
    {
        VarAdrSetInt(p_PP_OBJ_OPN3_PUSH_SHOW, TRUE);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_OPN3_PUSH_SHOW, FALSE);
    }

    if(PushtableType() && VarAdrToInt(OPEN_2_ENABLE))
    {
        VarAdrSetInt(p_PP_OBJ_OPN2_PUSH_SHOW, TRUE);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_OPN2_PUSH_SHOW, FALSE);
    }
}

/**************************生产警报界面-Product_Alarm*****************************/
/*****************************************
*Subject  :
*Condition:
*Object   :料管温度偏差可用
*Result   :
******************************************/
static void BarrelTempBiasEnable()
{
    if(VarAdrToInt(TEMP_CTRL_PART))
    {
        VarAdrSetInt(BARRELTEMPBIAS_ENABLE,FALSE);
    }
    else
    {
        VarAdrSetInt(BARRELTEMPBIAS_ENABLE,TRUE);
    }
}

/**************************IO重定义界面-IO_Redefine*****************************/
/*****************************************
*Subject  :
*Condition:
*Object   :温度部件状态改变切换部件下拉框
*Result   :
******************************************/
static void TempCtrlPartChg()
{
    if(VarAdrToInt(TEMP_CTRL_PART) == 2)
    {
        VarAdrSetInt(TEMPCTRLPART0_SHOW,FALSE);
        VarAdrSetInt(TEMPCTRLPART1_SHOW,FALSE);
        VarAdrSetInt(TEMPCTRLPART2_SHOW,TRUE);
    }
    else if(VarAdrToInt(TEMP_CTRL_PART) == 1)
    {
        VarAdrSetInt(TEMPCTRLPART0_SHOW,FALSE);
        VarAdrSetInt(TEMPCTRLPART1_SHOW,TRUE);
        VarAdrSetInt(TEMPCTRLPART2_SHOW,FALSE);
    }
    else
    {
        VarAdrSetInt(TEMPCTRLPART0_SHOW,TRUE);
        VarAdrSetInt(TEMPCTRLPART1_SHOW,FALSE);
        VarAdrSetInt(TEMPCTRLPART2_SHOW,FALSE);
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :输入点最大值和输出点最大值
*Result   :
******************************************/
static void IoRdInputOrOutputMax()
{
    if (VarAdrToInt(SYS_FL_MACH_CODE54) & 0x0200)//第一块使用
    {
        if (VarAdrToInt(SYS_FL_MACH_CODE54) & 0x2000)//第二块使用
        {
            if (IV5000)//主机型号：5000
            {
                 VarAdrSetInt(p_PP_OBJ_IORD_INPUT_MAX,80);
                 VarAdrSetInt(p_PP_OBJ_IORD_OUTPUT_MAX,88);
            }
            else if (IV5200 || IV5300)//主机型号：5200
            {
                 VarAdrSetInt(p_PP_OBJ_IORD_INPUT_MAX,80);
                 VarAdrSetInt(p_PP_OBJ_IORD_OUTPUT_MAX,96);
            }
            else  //主机型号：3100
            {
                VarAdrSetInt(p_PP_OBJ_IORD_INPUT_MAX,64);
                VarAdrSetInt(p_PP_OBJ_IORD_OUTPUT_MAX,80);
            }
        }
        else
        {
            if (IV5000)//主机型号：5000
            {
                VarAdrSetInt(p_PP_OBJ_IORD_INPUT_MAX,64);
                VarAdrSetInt(p_PP_OBJ_IORD_OUTPUT_MAX,72);
            }
            else if (IV5200 || IV5300)//主机型号：5200
            {
                 VarAdrSetInt(p_PP_OBJ_IORD_INPUT_MAX,64);
                 VarAdrSetInt(p_PP_OBJ_IORD_OUTPUT_MAX,80);
            }
            else
            {
                VarAdrSetInt(p_PP_OBJ_IORD_INPUT_MAX,48);
                VarAdrSetInt(p_PP_OBJ_IORD_OUTPUT_MAX,64);
            }
        }
    }
    else
    {
        if (IV5000)
        {
            VarAdrSetInt(p_PP_OBJ_IORD_INPUT_MAX,48);
            VarAdrSetInt(p_PP_OBJ_IORD_OUTPUT_MAX,56);
        }
        else if (IV5200 || IV5300)
        {
            VarAdrSetInt(p_PP_OBJ_IORD_INPUT_MAX,48);
            VarAdrSetInt(p_PP_OBJ_IORD_OUTPUT_MAX,64);
        }
        else
        {
            VarAdrSetInt(p_PP_OBJ_IORD_INPUT_MAX,32);
            VarAdrSetInt(p_PP_OBJ_IORD_OUTPUT_MAX,48);
        }
    }
}

/**************************润滑界面-Lubr*****************************/
/*****************************************
*Subject  :
*Condition:
*Object   :实现不同厂家对润滑模数上限的要求
*Result   :
******************************************/
static void LubModCntMax()
{
    static UI16 lubcnt_defmax =10000; //默认上限10000
    UI16 lubcnt_max;

    lubcnt_max = VarAdrToUI16(d_machine1_MACHSET_FL_reserve_2);
    if(lubcnt_max >0) //通用通过配置工具实现润滑模数上限
    {
        VarAdrSetInt(p_PP_OBJ_LUB_MAX,lubcnt_max);
    }
    else
    {
//        VarAdrSetInt(p_PP_OBJ_LUB_MAX,lubcnt_defmax); //默认上限10000
        VarAdrSetInt(p_PP_OBJ_LUB_MAX, VarAdrToUI16(p_PP_MACHSET_wLUBCNTMAX));//20211206 dyl 润滑上限设定值
    }
}

/**************************生产界面-ProdManage1*****************************/
/*****************************************
*Subject  :
*Condition:
*Object   :自动初期不良品数是否可见
*Result   :
******************************************/
static void ForcedRejectVisible() //20200303 自动初期不良品数是否可见
{
    if(CUSTOMID_8B00)
    {
        if(g_warm1==0x20100204 || g_warm2==0x20100204 || g_warm3==0x20100204 || g_warm4==0x20100204 || g_warm5==0x20100204
                 || g_warm6==0x20100204 || g_warm7==0x20100204 || g_warm8==0x20100204 || g_warm9==0x20100204 || g_warm10==0x20100204)//产品不良警报
        {
            VarAdrSetInt(p_PP_OBJ_AUTOBADCNT_VISBLE,FALSE);
            VarAdrSetInt(p_PP_OBJ_AUTOBADCNT_ENABLE,FALSE);
        }
        else
        {
            VarAdrSetInt(p_PP_OBJ_AUTOBADCNT_VISBLE,TRUE);
            if(VarAdrToInt(d_clamp1_MOLDSET_FL_USE_PRU))
            {
                VarAdrSetInt(p_PP_OBJ_AUTOBADCNT_ENABLE,TRUE);
            }
            else
            {
                VarAdrSetInt(p_PP_OBJ_AUTOBADCNT_ENABLE,FALSE);
            }
        }
    }
    else  //20200323
    {
        VarAdrSetInt(p_PP_OBJ_AUTOBADCNT_VISBLE,FALSE);
        VarAdrSetInt(p_PP_OBJ_AUTOBADCNT_ENABLE,FALSE);
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :座台位置最大值转换
*Result   :
******************************************/
static void SetNozzlePosMax() //20200325
{
    UI16 wNozzleValue = VarAdrToInt(d_nozzle1_MACHSET_NZL_METERCH);
    UI16 wValue = 0;

    if (IV5200 || IV5300)
    {
        if((wNozzleValue>=4)&&(wNozzleValue<=7))  //4-7路做座台切换
        {
            wValue = VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_4+(wNozzleValue-4));
        }
    }
    else if (IV3100 && ((VarAdrToUI16(d_machine1_VERSION_model_and_cat)&0x1FFF)==3101))
    {
        if(wNozzleValue == 7)
        {
            wValue = VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_EXT1);
        }
        else if((wNozzleValue>=4)&&(wNozzleValue<=6))
        {
            wValue = VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_4+(wNozzleValue-4));
        }
    }
    else
    {
        if(wNozzleValue == 4)
        {
            wValue = VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_4);
        }
        else if((wNozzleValue>=5)&&(wNozzleValue<=7))  //4-7路做座台切换
        {
            wValue = VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_EXT1+(wNozzleValue-5));
        }
    }

    VarAdrSetInt(p_PP_OBJ_ADPOSI_NOZ_MAX, wValue);
}

/*****************************************
*Subject  :
*Condition:
*Object   :PID开关模画面
*Result   :
******************************************/
static void ClampxPID()
{
    if(PushtableType())
    {
        VarAdrSetInt(p_PP_OBJ_PID_OPEN_START_DOT, TWO_DOT);
        if(VarAdrToInt(d_clamp1_ACTPARA_ADPOS_OPENSPD3) * 10 > 65000)//20241026 chj 修复开关模位置最大值超过65535.00问题--破模位置最大值
        {
            VarAdrSetInt(p_PP_OBJ_PID_OPEN_START_MAX, 65000);
        }
        else
        {
            VarAdrSetInt(p_PP_OBJ_PID_OPEN_START_MAX, VarAdrToInt(d_clamp1_ACTPARA_ADPOS_OPENSPD3) * 10);
        }
        VarAdrSetInt(p_PP_OBJ_PID_OPEN_END_MIN, VarAdrToInt(d_clamp1_ACTPARA_ADPOS_OPENSPD1)/10);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_PID_OPEN_START_DOT, ONE_DOT);
        VarAdrSetInt(p_PP_OBJ_PID_OPEN_START_MAX, VarAdrToInt(d_clamp1_ACTPARA_ADPOS_OPENSPD3));
        VarAdrSetInt(p_PP_OBJ_PID_OPEN_END_MIN, VarAdrToInt(d_clamp1_ACTPARA_ADPOS_OPENSPD1));
    }
}

/**************************智能开关模界面-Clamp_Smart*****************************/
/*****************************************
*Subject  :
*Condition:
*Object   :外设慢速距离可见
*Result   :
******************************************/
static void PerSlowDistVisible()
{
    if(GetCurrentUserId() > USER_EVERYONE)
    {
        VarAdrSetInt(p_PP_OBJ_PER_SLOWDIST_SHOW, TRUE);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_PER_SLOWDIST_SHOW, FALSE);
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :温度周期最小值
*Result   :
******************************************/
static void OutputCycleMin()
{
    if(VarAdrToInt(d_machine1_MACHSET_CD_TYPE) == 0)
    {
        VarAdrSetInt(p_PP_OBJ_OUTPUTCYCLE_MIN, 6);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_OUTPUTCYCLE_MIN, 2);
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :驱动器FUNC曲线显示可见
*Result   :
******************************************/
static void ServoFuncCurveShow()
{
    if(GetCurrentUserId() > USER_MANUFACTURER)
    {
        VarAdrSetInt(p_PP_OBJ_SERVO_FUNC_CURVE_SHOW, TRUE);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_SERVO_FUNC_CURVE_SHOW, FALSE);
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :油泵压力传感器AD通道显示
*Result   :
******************************************/
static void PumpADPrsSensorChnlShow()
{
    char str[120];
    char buff[10];
    UI16 i;
    snprintf(str,sizeof(str)-1,"%d ",VarAdrToInt(d_Pump1_MACHSET_PRESSAD_CH));

    for(i = 1; i < VarAdrToInt(SERVO_CURRENT_NUM); i++)
    {
        snprintf(buff,sizeof(buff)-1,"%d ",VarAdrToInt(d_Pump1_MACHSET_PRESSAD_CH+0x100000*i));
        strcat(str,buff);
    }

    VarAdrSetStr(p_PP_OBJ_PUMPCHANNEL_TEXT, str);

    if(((VarAdrToInt(SYS_FL_METER) & 0x0004) != 0) && (VarAdrToInt(d_inject1_MOLDSET_FL_INCACC) == 2))
    {
        VarAdrSetInt(p_PP_OBJ_INJ_NOZCHNL_SHOW, TRUE);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_INJ_NOZCHNL_SHOW, FALSE);
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :最大反向速度最小值
*Result   :
******************************************/
static void MaxReverseSpdMin()
{
    if(VarAdrToInt(SYS_FL_HYDRAUIC) & 0x1)
    {
        VarAdrSetInt(p_PP_OBJ_MAX_REV_SPD_MIN, 0);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_MAX_REV_SPD_MIN, 10);
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :开模到位减速计算斜率可见
*Result   :
******************************************/
static void AIClampOpenRampShow()
{
    if((VarAdrToInt(SYS_FL_MACH_CODE50) & 0x4000) && (GetCurrentUserId() > USER_PROFESSIONAL))//20211122 dyl
    {
        VarAdrSetInt(p_PP_OBJ_AI_CLAMP_OPEN_RAMP_SHOW, TRUE);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_AI_CLAMP_OPEN_RAMP_SHOW, FALSE);
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :射出压力和保压速度模式设置
*Result   :
******************************************/
static void InjPrsAndHoldSpdModeSet()
{
    if(VarAdrToInt(d_inject1_MOLDSET_INJPRES_SETMODE))
    {
        VarAdrSetInt(p_PP_OBJ_INJ_PRS_2TO6_SHOW, FALSE);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_INJ_PRS_2TO6_SHOW, TRUE);
    }

    if(VarAdrToInt(d_inject1_MOLDSET_HOLDSPD_SETMODE))
    {
        VarAdrSetInt(p_PP_OBJ_HOLD_SPD_2TO5_SHOW, FALSE);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_HOLD_SPD_2TO5_SHOW, TRUE);
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :开环压力超调可见
*Result   :
******************************************/
static void OpenLoopOverAdjShow()
{
    if(CUSTOMID_8700)
    {
        VarAdrSetInt(p_PP_OBJ_OPENLOOP_OVERADJ_SHOW, TRUE);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_OPENLOOP_OVERADJ_SHOW, FALSE);
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :射出压力小数位和最大值
*Result   :
******************************************/
static void InjPrsVal()
{
    if(VarAdrToInt(d_machine1_MACHSET_FL_SERVOMODE) & 0x2)//油路控制+2改为code16+2 20250310 jhh
    {
        VarAdrSetInt(p_PP_OBJ_INJ_PRS_DOT, 1);
        VarAdrSetInt(p_PP_OBJ_INJ_PRS_MAX, 10*VarAdrToInt(d_machine1_MACHSET_DAPRES_SYSLMT));
        VarAdrSetInt(p_PP_OBJ_HLD_PRS_MAX, 10*VarAdrToInt(d_autoctrl1_MOLDSET_DAPRES_LIMIT));
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_INJ_PRS_DOT, 0);
        VarAdrSetInt(p_PP_OBJ_INJ_PRS_MAX, VarAdrToInt(d_machine1_MACHSET_DAPRES_SYSLMT));
        VarAdrSetInt(p_PP_OBJ_HLD_PRS_MAX, VarAdrToInt(d_autoctrl1_MOLDSET_DAPRES_LIMIT));
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :托模二段位置最大值
*Result   :
******************************************/
static void EjtRet2PosMax()
{
    if(VarAdrToInt(d_eject1_MACHSET_ADPOSI_EJTRETMAX) > VarAdrToInt(d_eject1_ACTPARA_ADPOSI_EJTRETFIRST))
    {
        VarAdrSetInt(p_PP_OBJ_EJTRET2ENDPOS_MAX, VarAdrToInt(d_eject1_ACTPARA_ADPOSI_EJTRETFIRST));
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_EJTRET2ENDPOS_MAX, VarAdrToInt(d_eject1_MACHSET_ADPOSI_EJTRETMAX));
    }

    if(VarAdrToInt(d_eject1_MACHSET_ADPOSI_EJTRETMAX) > VarAdrToInt(d_eject1_ACTPARA_ADPOSI_EJTBFIRSTEND1))
    {
        VarAdrSetInt(p_PP_OBJ_EJTRET2FIRSTPOS_MAX, VarAdrToInt(d_eject1_ACTPARA_ADPOSI_EJTBFIRSTEND1));
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_EJTRET2FIRSTPOS_MAX, VarAdrToInt(d_eject1_MACHSET_ADPOSI_EJTRETMAX));
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :开模联动位置最小值
*Result   :
******************************************/
static void OpenLinkPosMin()
{
    if(VarAdrToInt(FL_CLSOPEN_MODE) == 2)//PID
    {
        if (PushtableType())
        {
            VarAdrSetInt(p_PP_OBJ_OPENLINKPOS_MIN,VarAdrToInt(ADPOS_OPENSPD1) / 10);
        }
        else
        {
            VarAdrSetInt(p_PP_OBJ_OPENLINKPOS_MIN,VarAdrToInt(ADPOS_OPENSPD1));
        }
    }
    else
    {
        if (PushtableType())
        {
            VarAdrSetInt(p_PP_OBJ_OPENLINKPOS_MIN,VarAdrToInt(ADPOSI_OPEN1) / 10);
        }
        else
        {
            VarAdrSetInt(p_PP_OBJ_OPENLINKPOS_MIN,VarAdrToInt(ADPOSI_OPEN1));
        }
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :射出一段时间可用
*Result   :
******************************************/
static void Inj1TimeEnable()
{
    if(VarAdrToInt(d_inject1_MOLDSET_FL_TURNTOHOLD) != 4)
    {
        VarAdrSetInt(p_PP_OBJ_INJ1TIME_ENABLE, TRUE);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_INJ1TIME_ENABLE, FALSE);
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :托进终止位置最大值
*Result   :
******************************************/
static void EjtAdvEndPosMax()
{
    if(VarAdrToInt(d_clamp1_ACTPARA_ADPOSI_OPENENDEND) > VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_3))
    {
        VarAdrSetInt(p_PP_OBJ_EJTADVENDPOS_MAX, VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_3));
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_EJTADVENDPOS_MAX, VarAdrToInt(d_clamp1_ACTPARA_ADPOSI_OPENENDEND));
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :开模差动结束位置可见
*Result   :
******************************************/
static void OpenDiffEndPosShow()
{
    if(CUSTOMID_6700)
    {
        VarAdrSetInt(p_PP_OBJ_OPENDIFFENDPOS_SHOW, TRUE);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_OPENDIFFENDPOS_SHOW, FALSE);
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :锁图标可见
*Result   :
******************************************/
static void LockShow()
{
    //show lock
    if(GetLockState())
    {
        if(VarAdrToInt(HARDWARE_LOCK))//20200409.cyx 图标分为硬件锁和软件锁
        {
            VarAdrSetInt(p_PP_OBJ_HARDWARE_LOCK_SHOW, TRUE);
        }
        else
        {
            VarAdrSetInt(p_PP_OBJ_HARDWARE_LOCK_SHOW, FALSE);
        }

        if(VarAdrToInt(USER_KEYLOCK) || (PageFramGetProtect()&&((OperateModeIndex()!=MODE_MANUAL)||JudgeChargeActStep() ||JudgeSuckBackActStep())))//20230518 chj增加射退动作判断
        {
            VarAdrSetInt(p_PP_OBJ_SOFTWARE_LOCK_SHOW, TRUE);
        }
        else
        {
            VarAdrSetInt(p_PP_OBJ_SOFTWARE_LOCK_SHOW, FALSE);
        }
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_HARDWARE_LOCK_SHOW, FALSE);
        VarAdrSetInt(p_PP_OBJ_SOFTWARE_LOCK_SHOW, FALSE);
    }
}

/*****************************************
*Subject  :
*Condition:
*Object   :伺服5隐藏
*Result   :
******************************************/
static void Servo5Hide()
{
    if((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x0004) == 0 && (VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020))
    {
        VarAdrSetInt(p_PP_OBJ_SERVO5_HIDE, FALSE);
        VarAdrSetInt(p_PP_OBJ_SPDRATE_MAX, 100);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_SERVO5_HIDE, TRUE);
        VarAdrSetInt(p_PP_OBJ_SPDRATE_MAX, 150);
    }
}

/**************************更新画面-ProgUpdate*****************************/
/*****************************************
*Subject  :
*Condition:
*Object   :IAP功能显示与否
*Result   :
******************************************/
static void UpdateIAPShow()//20211229 dyl IAP
{
    if(GetCurrentUserId() >= USER_SUPERVISOR)
    {
        VarAdrSetInt(p_PP_OBJ_IAP_FUNC_VISIBLE, TRUE);
    }
    else
    {
        VarAdrSetInt(p_PP_OBJ_IAP_FUNC_VISIBLE, FALSE);
    }
}

//20220818 dyl 显示/隐藏TAB的提示信息
#define TABLE_TIPS  "stTips"
static void SetTabTipsEnable()
{
    static PPAGE_FRAM pf = NULL;
    pf = PanelCurPage();
    if(pf !=NULL)
    {
        PLABEL_WGT plblname_tabtips = (PLABEL_WGT)PageFramFindWgtByName(pf, TABLE_TIPS);
        if(plblname_tabtips && (!PanelTopDialog() || PanelTopDialog() == pf))
        {
            if (IsHaveKeyBoard())
            {
                Show((PWGT)plblname_tabtips);
            }
            else
            {
                Hide((PWGT)plblname_tabtips);
            }
        }
    }
}
