#ifndef _CALIBRATION_AGG_CONTROL_H_
#define _CALIBRATION_AGG_CONTROL_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#define ADDINTERVALTIME		12	//加速度采集间隔时间6ms

typedef ACT_RET (*ACT_CB_FUNC)();
typedef void (*ACT_CBUP_FUNC)();


typedef enum enSLOPE_PRO_STEP
{
	SLOPE_PRO_STEP_SCOPE_CONFIRM = 0x01,
	SLOPE_PRO_STEP_CYCLE_RF = 0x02,
}SLOPE_PRO_STEP;


typedef struct tyCalibrationSlopeConfig
{
	UI16	config_flag;	//斜率校准配置标记	0:未配置	1:已配置，可进行相应学习动作
	UI16	admeter_ch;		//电子尺编号	1开始

	//		前进动作的速度、压力、位置设定	1为快速学习段	2为慢速定位段
	UI16	spd_fwd[2];		//%
	UI16	pres_fwd[2];	//bar
	UI16	pos_fwd[2];		//0.1mm

	//		后退动作的速度、压力、位置设定	1为快速学习段	2为慢速定位段
	UI16	spd_bwd[2];	//%
	UI16	pres_bwd[2];	//bar
	UI16	pos_bwd[2];		//0.1mm

	UI16	kp_pres_fwd;
	UI16	kp_pres_bwd;

	UI16	ki_pres_fwd;
	UI16	ki_pres_bwd;

	//		对应动作泵浦及油路配置	1为快速学习段	2为慢速定位段
	UI16	portlist_fwd[2];
	UI16	portlist_bwd[2];
	UI16	pumplist_fwd[2];
	UI16	pumplist_bwd[2];

	//		相应动作的开关阀动作 	行:1为快速学习段	2为慢速定位段 列:3个阀启动点
	UI16	valve_fwd[2][3];
	UI16	valve_bwd[2][3];	

	//		检测函数
	ACT_CB_FUNC	chkfunc;

	UI16*	pdietm_fwd;
	UI16*	pdietm_bwd;
	UI16*	pslope_max_fwd;
	UI16*	pslope_max_bwd;

	ACT_CBUP_FUNC	updatafunc;

	UI16	fwdmaxspd;	//mm/s
	UI16	bwdmaxspd;	//mm/s
}CalibrationSlopeConfig, *PCalibrationSlopeConfig;

typedef struct tyCalibrationSlopeProcessData
{
	UI16	slope_process_step;
	UI16	slope_rf_num;

	SPEED_CAL	speed_cal;
	
	UI32	admeter;		//对应电子尺位置	0.01mm
	UI32	ad_speed;		//对应电子尺速度	0.01mm/s
	UI32	ad_speed_last[ADDINTERVALTIME];	//上次电子尺速度	0.01mm/s
	UI32	ad_add;			//对应电子尺加速度	0.01mm/s2

	UI32	CalibrationSlope_tm;		//学习起始时间

	UI16	dietm_startpos;					//死区开始位置
	
	//10次动作的死区学习相关
	UI16	dietm_actstarttm_ms;	//指令开始发送时计时
	UI16	dietm_cylinderstarttm_ms;	//油缸开始运行时计时

	UI16	dietm_fwd_ms[10];	//计算的死区时间
	UI16	dietm_bwd_ms[10];

	UI16	dietm_fwd_ms_result;	//最终死区时间
	UI16	dietm_bwd_ms_result;
	
	//10次动作的斜率学习相关
	UI32	slope_max_fwd[10];	//计算的最大加速度
	UI32	slope_max_bwd[10];

	UI32	slope_max_fwd_result;	//最终最大斜率
	UI32	slope_max_bwd_result;
}CalibrationSlopeProcessData, *PCalibrationSlopeProcessData;


typedef struct tyCalibrationSlopeData
{
	CalibrationSlopeConfig config;	
	CalibrationSlopeProcessData processdata;
}CalibrationSlopeData, *PCalibrationSlopeData;



enum EN_CAGGCONTROL_ACTNAME
{
    CAGGC_INIT = INIT1_STEP,			
    CAGGC_FAST_FWD_START = STAGE_1_STEP,	//快速前进开始
    CAGGC_FAST_FWD_END = STAGE_2_STEP,		//快速前进停止
    CAGGC_SLOW_FWD_START = STAGE_3_STEP,	//慢速前进开始
    CAGGC_SLOW_FWD_END = STAGE_4_STEP,		//慢速前进停止
    CAGGC_FAST_BWD_START = STAGE_5_STEP,	//快速后退开始
    CAGGC_FAST_BWD_END = STAGE_6_STEP,		//快速后退停止
    CAGGC_SLOW_BWD_START = STAGE_7_STEP,	//慢速后退开始
    CAGGC_SLOW_BWD_END = STAGE_8_STEP,		//慢速后退停止
    CAGGC_END = END_STEP
};

ACT_RET CalibrationSlopeAct(PACT pact, UI32 para);
void CAggCActOff(PMACHINE pp, PACT pact);

void ClampCalibration(void);
void EjectCalibration(void);
void InjectCalibration(void);


#ifdef __cplusplus
}
#endif

#endif

