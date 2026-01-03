/******************************************************************************
  文 件 名   : CalibrationAggControl.c
  版 本 号   : 初稿
  作    者   : zhou tuo
  生成日期   : 2019年8月6日
  最近修改   :
  功能描述   : 液压系统各参数校准动作
  函数列表   :


******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "act.h"
#include "admeter.h"
#include "pump.h"
#include "acttype.h"
#include "ioproc.h"
#include "machine.h"
#include "typelst_dic.h"
#include "part.h"
#include "clamp.h"
#include "CalibrationAggControl.h"
#include "stdlib.h"
#include "Keycmd.h"

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
extern ADMETER	 m_ad;
extern PPUMP	g_pumpn[MAX_PUMP];
extern MACHINE	m_machine;

extern UI16 m_speed;


/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
void ActSetHydr(PACT pact,UI16 mode, UI16 press, UI16 flow, UI16 pramp, UI16 framp, UI16 prampstart, UI16 frampstart, UI16 hydrdly, UI16 hydrport, 
					UI16 ServoPumpLmt, UI16 NormalPumpLmt, UI16 Servo_P, UI16 Servo_I, UI16 PresFlag, UI16 KiClear, UI16 Priority);


/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
CalibrationSlopeData CSlopeData;

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define FASTENDVALVEDELAY	100	//阀延迟1S
#define SLOPERFNUMSUM		10	//斜率校准尝试次数总数

#define CLPSPDUP		300		//自学习时开合模速度上限	mm/s
#define EJTSPDUP		300		//自学习时托模速度上限	mm/s
#define INJSPDUP		300		//自学习时注射速度上限	mm/s


#define DIETMMOVEPOS	5	// 0.5mm
#define PI	3.14

/*******************************************************************************************
 *Brief			:通过机器参数，计算转速对应的油缸速度	rpm->mm/s
 *Param			:rpm转速	Oilpump_Q排量 ml/r	Cylinder_R油缸直径 mm	Cylinder_r柱塞杆直径 mm
 *Return Value	:mm/s的油缸速度
 *Aauthor		:ZT
 *Date			:20190809
********************************************************************************************/
int CalculateHydrocylinderSpd(int rpm,int Oilpump_Q,int Cylinder_R,int Cylinder_r,int Cylinder_num)
{
	float Cylinder_opn_s = 0;//油缸面积差 mm2
	float q = 0;//流量	mm3/min

	Cylinder_opn_s = PI * (((Cylinder_R / 2) * (Cylinder_R / 2)) - ((Cylinder_r / 2) * (Cylinder_r / 2))) * Cylinder_num;
	q = rpm * Oilpump_Q * 1000;

	return ((q / Cylinder_opn_s) / 60);
}

void InjectDataUp()
{
	PINJECT pp = (PINJECT)GetPartByID(MK_PART(INJECT_ID, 1));
	PartDataTx((PPART)pp, MD_ID, 5, GET_IND(PINJECT_MOLDSET, SLOPE_MAX_SUCK_SPD), FALSE);
	PartDataTx((PPART)pp, MD_ID, 5, GET_IND(PINJECT_MOLDSET, SLOPE_MAX_INJ_SPD), FALSE);
}

void InjectCalibration()
{	
	PINJECT pp = (PINJECT)GetPartByID(MK_PART(INJECT_ID, 1));
	PACT pact = pp->part.pact_tbl+OS_INJE-1;

	UI16 maxad = 0;

	if(CSlopeData.config.config_flag == 1)
	{
		return;
	}
	
	CSlopeData.config.admeter_ch = 1;

	//压力PI设定
	CSlopeData.config.kp_pres_fwd = 210;
	CSlopeData.config.kp_pres_bwd = 210;
	CSlopeData.config.ki_pres_fwd = 20;
	CSlopeData.config.ki_pres_bwd = 20;

	//位置设定
	if(m_ad.mh->ADPOSI_PTMSETLEN[CSlopeData.config.admeter_ch-1] < m_ad.mh->ADPOSI_MACHINELEN[CSlopeData.config.admeter_ch-1])
	{
		maxad = m_ad.mh->ADPOSI_PTMSETLEN[CSlopeData.config.admeter_ch-1];
	}
	else
	{	
		maxad = m_ad.mh->ADPOSI_MACHINELEN[CSlopeData.config.admeter_ch-1];
	}

	CSlopeData.config.pos_fwd[0] = maxad*7/10;
	CSlopeData.config.pos_fwd[1] = maxad*9/10;

	CSlopeData.config.pos_bwd[0] = maxad*3/10;
	CSlopeData.config.pos_bwd[1] = maxad*1/10;

	//第一段为快速运行学习段
	CSlopeData.config.bwdmaxspd = CalculateHydrocylinderSpd(g_pumpn[0]->mh->MOTOR_SPEED_MAX,GetMotorPump_Q(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_INJCT)),pp->hy->HYDR_PUMPS_INJCT),pp->mh->DIA_CYLINDER_SUCK/10,pp->mh->DIA_PISDIA_SUCK/10,pp->mh->NUM_CYLINDER_SUCK);
	CSlopeData.config.fwdmaxspd = CalculateHydrocylinderSpd(g_pumpn[0]->mh->MOTOR_SPEED_MAX,GetMotorPump_Q(GetPosCtrlCh(HYDR_PORT(pp->hy->HYDR_PORT_INJCT)),pp->hy->HYDR_PUMPS_INJCT),pp->mh->DIA_CYLINDER_SUCK/10,0,pp->mh->NUM_CYLINDER_SUCK);
	CSlopeData.config.spd_fwd[0] = INJSPDUP*100/CSlopeData.config.fwdmaxspd;
	CSlopeData.config.spd_bwd[0] = INJSPDUP*100/CSlopeData.config.bwdmaxspd;

	if(CSlopeData.config.spd_fwd[0] > 100)
	{
		CSlopeData.config.spd_fwd[0] = 100;
	}

	if(CSlopeData.config.spd_bwd[0] > 100)
	{
		CSlopeData.config.spd_bwd[0] = 100;
	}
	
	//第二段为慢速定位
	CSlopeData.config.spd_fwd[1] = 20;
	CSlopeData.config.spd_bwd[1] = 20;

	//压力设定
	CSlopeData.config.pres_fwd[0] = 30;
	CSlopeData.config.pres_fwd[1] = 30;
	CSlopeData.config.pres_bwd[0] = 30;
	CSlopeData.config.pres_bwd[1] = 30;

	//通道号及泵浦列表
	pact = pp->part.pact_tbl+OS_SUCK-1;
	CSlopeData.config.portlist_fwd[0] = HYDR_PORT(pp->hy->HYDR_PORT_SUCKBACK);
	CSlopeData.config.portlist_fwd[1] = HYDR_PORT(pp->hy->HYDR_PORT_SUCKBACK);

	pact = pp->part.pact_tbl+OS_INJE-1;
	CSlopeData.config.portlist_bwd[0] = HYDR_PORT(pp->hy->HYDR_PORT_INJCT);
	CSlopeData.config.portlist_bwd[1] = HYDR_PORT(pp->hy->HYDR_PORT_INJCT);

	CSlopeData.config.pumplist_fwd[0] = pp->hy->HYDR_PUMPS_SUCKBACK;
	CSlopeData.config.pumplist_fwd[1] =	pp->hy->HYDR_PUMPS_SUCKBACK;
	
	CSlopeData.config.pumplist_bwd[0] = pp->hy->HYDR_PUMPS_INJCT;
	CSlopeData.config.pumplist_bwd[1] =	pp->hy->HYDR_PUMPS_INJCT;

	//各动作的阀设定
	CSlopeData.config.valve_fwd[0][0] = pp->po->O_SUC_SOL;
	CSlopeData.config.valve_fwd[0][1] = 0;
	CSlopeData.config.valve_fwd[0][2] = 0;

	CSlopeData.config.valve_fwd[1][0] = pp->po->O_SUC_SOL;
	CSlopeData.config.valve_fwd[1][1] = 0;
	CSlopeData.config.valve_fwd[1][2] = 0;

	CSlopeData.config.valve_bwd[0][0] = pp->po->O_INJ_SOL;
	CSlopeData.config.valve_bwd[0][1] = 0;
	CSlopeData.config.valve_bwd[0][2] = 0;

	CSlopeData.config.valve_bwd[1][0] = pp->po->O_INJ_SOL;
	CSlopeData.config.valve_bwd[1][1] = 0;
	CSlopeData.config.valve_bwd[1][2] = 0;

	CSlopeData.config.chkfunc = (ACT_CB_FUNC)InjectPreChk_CBAC;

	CSlopeData.config.pdietm_fwd = (UI16*)&(pp->md->PUMP_DEAD_SUCK);
	CSlopeData.config.pdietm_bwd = (UI16*)&(pp->md->PUMP_DEAD_INJ);
	
	CSlopeData.config.pslope_max_fwd = (UI16*)&(pp->md->SLOPE_MAX_SUCK_SPD);
	CSlopeData.config.pslope_max_bwd = (UI16*)&(pp->md->SLOPE_MAX_INJ_SPD);

	CSlopeData.config.updatafunc = (ACT_CBUP_FUNC)InjectDataUp;
	//设置1激活自学习
	CSlopeData.config.config_flag = 1;

	ForkAct((&m_machine)->act + OS_CALI - 1, FIRST_FORK, NULL, 0, 0);
}

void EjectDataUp()
{
	PEJECT pp = (PEJECT)GetPartByID(MK_PART(EJECT_ID, 1));
	PartDataTx((PPART)pp, MD_ID, 10, GET_IND(PEJECT_MOLDSET, EJEF_MAX_SPD), FALSE);
}

void EjectCalibration()
{	
	PEJECT pp = (PEJECT)GetPartByID(MK_PART(EJECT_ID, 1));
	PACT pact = pp->part.pact_tbl;

	UI16 maxad = 0;

	if(CSlopeData.config.config_flag == 1)
	{
		return;
	}
	
	CSlopeData.config.admeter_ch = 3;

	//压力PI设定
	CSlopeData.config.kp_pres_fwd = 210;
	CSlopeData.config.kp_pres_bwd = 210;
	CSlopeData.config.ki_pres_fwd = 20;
	CSlopeData.config.ki_pres_bwd = 20;

	//位置设定
//	if(m_ad.mh->ADPOSI_PTMSETLEN[CSlopeData.config.admeter_ch-1] < m_ad.mh->ADPOSI_MACHINELEN[CSlopeData.config.admeter_ch-1])
//	{
//		maxad = m_ad.mh->ADPOSI_PTMSETLEN[CSlopeData.config.admeter_ch-1];
//	}
//	else
//	{	
//		maxad = m_ad.mh->ADPOSI_MACHINELEN[CSlopeData.config.admeter_ch-1];
//	}
	maxad = pp->pa->ADPOSI_EJTADVFASTEND;	//托模动作

	CSlopeData.config.pos_fwd[0] = maxad*7/10;
	CSlopeData.config.pos_fwd[1] = maxad*9/10;

	CSlopeData.config.pos_bwd[0] = maxad*3/10;
	CSlopeData.config.pos_bwd[1] = maxad*1/10;

	//第一段为快速运行学习段
	CSlopeData.config.fwdmaxspd = CalculateHydrocylinderSpd(g_pumpn[0]->mh->MOTOR_SPEED_MAX,g_pumpn[0]->mh->OIL_PUMP_DISPLACEMENT/10,pp->mh->DIA_CYLINDER_E/10,pp->mh->DIA_PISDIA_E/10,pp->mh->DIA_OIL_NUM);
	CSlopeData.config.bwdmaxspd = CalculateHydrocylinderSpd(g_pumpn[0]->mh->MOTOR_SPEED_MAX,g_pumpn[0]->mh->OIL_PUMP_DISPLACEMENT/10,pp->mh->DIA_CYLINDER_E/10,0,pp->mh->DIA_OIL_NUM);
	CSlopeData.config.spd_fwd[0] = EJTSPDUP*100/CSlopeData.config.fwdmaxspd;
	CSlopeData.config.spd_bwd[0] = EJTSPDUP*100/CSlopeData.config.bwdmaxspd;

	if(CSlopeData.config.spd_fwd[0] > 100)
	{
		CSlopeData.config.spd_fwd[0] = 100;
	}

	if(CSlopeData.config.spd_bwd[0] > 100)
	{
		CSlopeData.config.spd_bwd[0] = 100;
	}
	
	//第二段为慢速定位
	CSlopeData.config.spd_fwd[1] = 20;
	CSlopeData.config.spd_bwd[1] = 20;

	//压力设定
	CSlopeData.config.pres_fwd[0] = 30;
	CSlopeData.config.pres_fwd[1] = 30;
	CSlopeData.config.pres_bwd[0] = 30;
	CSlopeData.config.pres_bwd[1] = 30;

	//通道号及泵浦列表
	pact = pp->part.pact_tbl+OS_EJTF-1;
	CSlopeData.config.portlist_fwd[0] = HYDR_PORT(pp->hy->HYDR_PORT_EJECT);
	CSlopeData.config.portlist_fwd[1] = HYDR_PORT(pp->hy->HYDR_PORT_EJECT);

	pact = pp->part.pact_tbl+OS_EJTB-1;
	CSlopeData.config.portlist_bwd[0] = HYDR_PORT(pp->hy->HYDR_PORT_EJECT);
	CSlopeData.config.portlist_bwd[1] = HYDR_PORT(pp->hy->HYDR_PORT_EJECT);

	CSlopeData.config.pumplist_fwd[0] = pp->hy->HYDR_PUMPS_EJECT;
	CSlopeData.config.pumplist_fwd[1] =	pp->hy->HYDR_PUMPS_EJECT;
	
	CSlopeData.config.pumplist_bwd[0] = pp->hy->HYDR_PUMPS_EJECT;
	CSlopeData.config.pumplist_bwd[1] =	pp->hy->HYDR_PUMPS_EJECT;

	//各动作的阀设定
	CSlopeData.config.valve_fwd[0][0] = pp->po->O_EJT_FWD;
	CSlopeData.config.valve_fwd[0][1] = 0;
	CSlopeData.config.valve_fwd[0][2] = 0;

	CSlopeData.config.valve_fwd[1][0] = pp->po->O_EJT_FWD;
	CSlopeData.config.valve_fwd[1][1] = 0;
	CSlopeData.config.valve_fwd[1][2] = 0;

	CSlopeData.config.valve_bwd[0][0] = pp->po->O_EJT_BWD;
	CSlopeData.config.valve_bwd[0][1] = 0;
	CSlopeData.config.valve_bwd[0][2] = 0;

	CSlopeData.config.valve_bwd[1][0] = pp->po->O_EJT_BWD;
	CSlopeData.config.valve_bwd[1][1] = 0;
	CSlopeData.config.valve_bwd[1][2] = 0;

	CSlopeData.config.chkfunc = (ACT_CB_FUNC)EjfFwdPreChk_CBAC;

	CSlopeData.config.pdietm_fwd = (UI16*)&(pp->md->PUMP_DEAD_EJEFWD);
	CSlopeData.config.pdietm_bwd = (UI16*)&(pp->md->PUMP_DEAD_EJEBWD);
	
	CSlopeData.config.pslope_max_fwd = (UI16*)&(pp->md->EJEF_MAX_SPD);
	CSlopeData.config.pslope_max_bwd = (UI16*)&(pp->md->EJEB_MAX_SPD);

	CSlopeData.config.updatafunc = (ACT_CBUP_FUNC)EjectDataUp;
	//设置1激活自学习
	CSlopeData.config.config_flag = 1;

	ForkAct((&m_machine)->act + OS_CALI - 1, FIRST_FORK, NULL, 0, 0);
}


void ClampDataUp()
{
	PCLAMP pp = (PCLAMP)GetPartByID(MK_PART(CLAMP_ID, 1));
	PartDataTx((PPART)pp, MD_ID, 2, GET_IND(PCLAMP_MOLDSET, SLOPE_MAX_CLS_SPD), FALSE);
	PartDataTx((PPART)pp, MD_ID, 4, GET_IND(PCLAMP_MOLDSET, PUMP_K), FALSE);
}

void ClampCalibration()
{	
	PCLAMP pp = (PCLAMP)GetPartByID(MK_PART(CLAMP_ID, 1));
	PACT pact = pp->part.pact_tbl;

	UI16 maxad = 0;

	if(CSlopeData.config.config_flag == 1)
	{
		return;
	}
	
	CSlopeData.config.admeter_ch = 2;

	//压力PI设定
	CSlopeData.config.kp_pres_fwd = 210;
	CSlopeData.config.kp_pres_bwd = 210;
	CSlopeData.config.ki_pres_fwd = 20;
	CSlopeData.config.ki_pres_bwd = 20;

	//位置设定
	if(m_ad.mh->ADPOSI_PTMSETLEN[1] < m_ad.mh->ADPOSI_MACHINELEN[1])
	{
		maxad = m_ad.mh->ADPOSI_PTMSETLEN[1];
	}
	else
	{	
		maxad = m_ad.mh->ADPOSI_MACHINELEN[1];
	}

	CSlopeData.config.pos_fwd[0] = maxad*7/10;
	CSlopeData.config.pos_fwd[1] = maxad*9/10;

	CSlopeData.config.pos_bwd[0] = maxad*5/10;
	CSlopeData.config.pos_bwd[1] = maxad*1/10;

	//第一段为快速运行学习段
	CSlopeData.config.fwdmaxspd = CalculateHydrocylinderSpd(g_pumpn[0]->mh->MOTOR_SPEED_MAX,g_pumpn[0]->mh->OIL_PUMP_DISPLACEMENT/10,pp->mh->DIA_CYLINDER_M/10,pp->mh->DIA_PISDIA_M/10,pp->mh->DIA_OIL_NUM);
	CSlopeData.config.bwdmaxspd = CalculateHydrocylinderSpd(g_pumpn[0]->mh->MOTOR_SPEED_MAX,g_pumpn[0]->mh->OIL_PUMP_DISPLACEMENT/10,pp->mh->DIA_CYLINDER_M/10,0,pp->mh->DIA_OIL_NUM);
	CSlopeData.config.spd_fwd[0] = CLPSPDUP*100/CSlopeData.config.fwdmaxspd;
	CSlopeData.config.spd_bwd[0] = CLPSPDUP*100/CSlopeData.config.bwdmaxspd;

	if(CSlopeData.config.spd_fwd[0] > 100)
	{
		CSlopeData.config.spd_fwd[0] = 100;
	}

	if(CSlopeData.config.spd_bwd[0] > 100)
	{
		CSlopeData.config.spd_bwd[0] = 100;
	}
	
	//第二段为慢速定位
	CSlopeData.config.spd_fwd[1] = 20;
	CSlopeData.config.spd_bwd[1] = 20;

	//压力设定
	CSlopeData.config.pres_fwd[0] = 30;
	CSlopeData.config.pres_fwd[1] = 30;
	CSlopeData.config.pres_bwd[0] = 30;
	CSlopeData.config.pres_bwd[1] = 30;

	//通道号及泵浦列表
	pact = pp->part.pact_tbl+OS_OPNM-1;
	CSlopeData.config.portlist_fwd[0] = HYDR_PORT(pp->hy->HYDR_PORT_CLAMPOPN);
	CSlopeData.config.portlist_fwd[1] = HYDR_PORT(pp->hy->HYDR_PORT_CLAMPOPN);

	pact = pp->part.pact_tbl+OS_CLSM-1;
	CSlopeData.config.portlist_bwd[0] = HYDR_PORT(pp->hy->HYDR_PORT_CLAMPCLS);
	CSlopeData.config.portlist_bwd[1] = HYDR_PORT(pp->hy->HYDR_PORT_CLAMPCLS);

	CSlopeData.config.pumplist_fwd[0] = pp->hy->HYDR_PUMPS_OPNFAST;
	CSlopeData.config.pumplist_fwd[1] =	pp->hy->HYDR_PUMPS_OPNFAST;
	
	CSlopeData.config.pumplist_bwd[0] = pp->hy->HYDR_PUMPS_CLSFAST;
	CSlopeData.config.pumplist_bwd[1] =	pp->hy->HYDR_PUMPS_CLSFAST;

	//各动作的阀设定
	CSlopeData.config.valve_fwd[0][0] = pp->po->O_OPN_SOL;
	CSlopeData.config.valve_fwd[0][1] = pp->po->O_OPN_FST;
	CSlopeData.config.valve_fwd[0][2] = pp->po->O_CLAMP_FST;

	CSlopeData.config.valve_fwd[1][0] = pp->po->O_OPN_SOL;
	CSlopeData.config.valve_fwd[1][1] = 0;
	CSlopeData.config.valve_fwd[1][2] = 0;

	CSlopeData.config.valve_bwd[0][0] = pp->po->O_CLS_SOL;
	CSlopeData.config.valve_bwd[0][1] = pp->po->O_CLS_ULT;
	CSlopeData.config.valve_bwd[0][2] = pp->po->O_CLS_FST;

	CSlopeData.config.valve_bwd[1][0] = pp->po->O_CLS_SOL;
	CSlopeData.config.valve_bwd[1][1] = 0;
	CSlopeData.config.valve_bwd[1][2] = 0;

	CSlopeData.config.chkfunc = (ACT_CB_FUNC)ClampClsPreChk_CBAC;

	CSlopeData.config.pdietm_fwd = (UI16*)&(pp->md->PUMP_DEAD_CLS);
	CSlopeData.config.pdietm_bwd = (UI16*)&(pp->md->PUMP_DEAD);
	
	CSlopeData.config.pslope_max_fwd = (UI16*)&(pp->md->SLOPE_MAX_CLS_SPD);
	CSlopeData.config.pslope_max_bwd = (UI16*)&(pp->md->SLOPE_MAX_OPN_SPD);

	CSlopeData.config.updatafunc = (ACT_CBUP_FUNC)ClampDataUp;
	//设置1激活自学习
	CSlopeData.config.config_flag = 1;

	ForkAct((&m_machine)->act + OS_CALI - 1, FIRST_FORK, NULL, 0, 0);
}

void SetCAggCFastFwd(PMACHINE pp, PACT pact)
{
    SetPOBit(CSlopeData.config.valve_fwd[0][0], PC_ON);
    SetPOBit(CSlopeData.config.valve_fwd[0][1], PC_ON);
    SetPOBit(CSlopeData.config.valve_fwd[0][2], PC_ON);


    ActSetHydr(pact, MC_CTR_MODE_SPEED | MC_CTR_MODE_PRES, CSlopeData.config.pres_fwd[0], CSlopeData.config.spd_fwd[0], 0, 0,
               0, 0, 0, CSlopeData.config.portlist_fwd[0], CSlopeData.config.pumplist_fwd[0], CSlopeData.config.pumplist_fwd[0],
               CSlopeData.config.kp_pres_fwd, CSlopeData.config.ki_pres_fwd, 0, !(pact->ftaskstat & INTF_CEC), 0);
}

void CAggCFastFwdEnd(PMACHINE pp, PACT pact)
{
    HYDR_CLS(0, 0);

    SetPOBitDelay(CSlopeData.config.valve_fwd[0][0], PC_OFF, FASTENDVALVEDELAY);
    SetPOBitDelay(CSlopeData.config.valve_fwd[0][1], PC_OFF, FASTENDVALVEDELAY);
    SetPOBitDelay(CSlopeData.config.valve_fwd[0][2], PC_OFF, FASTENDVALVEDELAY);
}

void SetCAggCFastBwd(PMACHINE pp, PACT pact)
{
    SetPOBit(CSlopeData.config.valve_bwd[0][0], PC_ON);
    SetPOBit(CSlopeData.config.valve_bwd[0][1], PC_ON);
    SetPOBit(CSlopeData.config.valve_bwd[0][2], PC_ON);


    ActSetHydr(pact, MC_CTR_MODE_SPEED | MC_CTR_MODE_PRES, CSlopeData.config.pres_bwd[0], CSlopeData.config.spd_bwd[0], 0, 0,
               0, 0, 0, CSlopeData.config.portlist_bwd[0], CSlopeData.config.pumplist_bwd[0], CSlopeData.config.pumplist_bwd[0],
               CSlopeData.config.kp_pres_bwd, CSlopeData.config.ki_pres_bwd, 0, !(pact->ftaskstat & INTF_CEC), 0);
}

void CAggCFastBwdEnd(PMACHINE pp, PACT pact)
{
    HYDR_CLS(0, 0);

    SetPOBitDelay(CSlopeData.config.valve_bwd[0][0], PC_OFF, FASTENDVALVEDELAY);
    SetPOBitDelay(CSlopeData.config.valve_bwd[0][1], PC_OFF, FASTENDVALVEDELAY);
    SetPOBitDelay(CSlopeData.config.valve_bwd[0][2], PC_OFF, FASTENDVALVEDELAY);
}


void SetCAggCSlowFwd(PMACHINE pp, PACT pact)
{
    SetPOBit(CSlopeData.config.valve_fwd[1][0], PC_ON);
    SetPOBit(CSlopeData.config.valve_fwd[1][1], PC_ON);
    SetPOBit(CSlopeData.config.valve_fwd[1][2], PC_ON);


    ActSetHydr(pact, MC_CTR_MODE_SPEED | MC_CTR_MODE_PRES, CSlopeData.config.pres_fwd[1], CSlopeData.config.spd_fwd[1], 0, 0,
               0, 0, 0, CSlopeData.config.portlist_fwd[1], CSlopeData.config.pumplist_fwd[1], CSlopeData.config.pumplist_fwd[1],
               CSlopeData.config.kp_pres_fwd, CSlopeData.config.ki_pres_fwd, 0, !(pact->ftaskstat & INTF_CEC), 0);
}

void CAggCSlowFwdEnd(PMACHINE pp, PACT pact)
{
    HYDR_CLS(0, 0);

    SetPOBitDelay(CSlopeData.config.valve_fwd[1][0], PC_OFF, FASTENDVALVEDELAY);
    SetPOBitDelay(CSlopeData.config.valve_fwd[1][1], PC_OFF, FASTENDVALVEDELAY);
    SetPOBitDelay(CSlopeData.config.valve_fwd[1][2], PC_OFF, FASTENDVALVEDELAY);
}

void SetCAggCSlowBwd(PMACHINE pp, PACT pact)
{
    SetPOBit(CSlopeData.config.valve_bwd[1][0], PC_ON);
    SetPOBit(CSlopeData.config.valve_bwd[1][1], PC_ON);
    SetPOBit(CSlopeData.config.valve_bwd[1][2], PC_ON);


    ActSetHydr(pact, MC_CTR_MODE_SPEED | MC_CTR_MODE_PRES, CSlopeData.config.pres_bwd[1], CSlopeData.config.spd_bwd[1], 0, 0,
               0, 0, 0, CSlopeData.config.portlist_bwd[1], CSlopeData.config.pumplist_bwd[1], CSlopeData.config.pumplist_bwd[1],
               CSlopeData.config.kp_pres_bwd, CSlopeData.config.ki_pres_bwd, 0, !(pact->ftaskstat & INTF_CEC), 0);
}

void CAggCSlowBwdEnd(PMACHINE pp, PACT pact)
{
    HYDR_CLS(0, 0);

    SetPOBitDelay(CSlopeData.config.valve_bwd[1][0], PC_OFF, FASTENDVALVEDELAY);
    SetPOBitDelay(CSlopeData.config.valve_bwd[1][1], PC_OFF, FASTENDVALVEDELAY);
    SetPOBitDelay(CSlopeData.config.valve_bwd[1][2], PC_OFF, FASTENDVALVEDELAY);
}

void CAggCActOff(PMACHINE pp, PACT pact)
{
    HYDR_CLS(0, 0);

    SetPOBit(CSlopeData.config.valve_bwd[1][0], PC_OFF);
    SetPOBit(CSlopeData.config.valve_bwd[1][1], PC_OFF);
    SetPOBit(CSlopeData.config.valve_bwd[1][2], PC_OFF);
	
	SetPOBit(CSlopeData.config.valve_fwd[1][0], PC_OFF);
    SetPOBit(CSlopeData.config.valve_fwd[1][1], PC_OFF);
    SetPOBit(CSlopeData.config.valve_fwd[1][2], PC_OFF);

	CSlopeData.config.updatafunc = NULL;
	CSlopeData.config.chkfunc = NULL;
	CSlopeData.config.config_flag = 0;

	SetOperMode(OM_MANUAL);
}


ACT_RET CalibrationSlopeActBack(PMACHINE pp, PACT pact)
{
    ACT_RET ret = ACT_OK;

    CSlopeData.processdata.admeter = GetMeterPosByCh(CSlopeData.config.admeter_ch);
    CSlopeData.processdata.ad_speed = AdMeterSpeedLoop(&(CSlopeData.processdata.speed_cal));
	m_speed = CSlopeData.processdata.ad_speed/10;

    CSlopeData.processdata.ad_add = abs(CSlopeData.processdata.ad_speed - CSlopeData.processdata.ad_speed_last[CSlopeData.processdata.CalibrationSlope_tm % ADDINTERVALTIME])*1000/(ADDINTERVALTIME/2);
    CSlopeData.processdata.ad_speed_last[CSlopeData.processdata.CalibrationSlope_tm % ADDINTERVALTIME] = CSlopeData.processdata.ad_speed;
    return ret;
}

/*******************************************************************************************
 *Brief			:确认斜率校准行程是否有问题，如果行程不够，则警报 停止校准动作
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20190812
********************************************************************************************/
ACT_RET CalibrationSlopePosLenChk(PMACHINE pp, PACT pact)
{
	static UI16	zerospdcn = 0;

	if(ACT_STEP_100MS > 2)
	{
		if(CSlopeData.processdata.ad_speed < 500)
		{
			zerospdcn++;
		}

		if(zerospdcn > 400)
		{
			SET_PART_ERR(pp,ER1_SLOPE_CAL_POS_ERR);
			return ACT_ERR_ABORT;
		}
	}
	else
	{
		zerospdcn = 0;
	}

	return ACT_OK;
}

ACT_RET CalibrationSlopeAct(PACT pact, UI32 para)
{
    PMACHINE pp = (PMACHINE)(pact->part);
    ACT_RET ret = ACT_OK;
		UI16 i = 0;
		UI32 slope_max_buf = 0;

		CSlopeData.processdata.CalibrationSlope_tm++;

    switch (pact->step)
    {
        case CAGGC_INIT:
			if(CSlopeData.config.config_flag == 1)
			{
				if(CSlopeData.config.chkfunc != NULL)
				{
					if((ACT_RET)(*CSlopeData.config.chkfunc)() == ACT_ERR_ABORT)
					{
						return ACT_ERR_ABORT;
					}
				}				
				
				CSlopeData.processdata.slope_process_step = SLOPE_PRO_STEP_SCOPE_CONFIRM;
				CSlopeData.processdata.slope_rf_num = 0;
				CSlopeData.processdata.CalibrationSlope_tm = 0;
				AdMeterSpeedInit(CSlopeData.config.admeter_ch, &(CSlopeData.processdata.speed_cal),1);

				if(ChkSysCfg(CO4_CKEY))
				{
					SET_PART_ERR(GetSys(),ER0_SKEY);
					ACT_STEP_NEW(CAGGC_INIT + 1);
				}
				else
					ACT_STEP_NEW(CAGGC_SLOW_BWD_START);
			}
			else
			{
				ACT_STEP_NEW(CAGGC_END);
			}
			break;	
		case CAGGC_INIT + 1:  //等待运行键
			if(GetRunKey())
			{
				CLR_PART_ERR(GetSys(), ER0_SKEY);
				ClearRunKey();
				ACT_STEP_NEW(CAGGC_SLOW_BWD_START);
			}			
			break;
        case CAGGC_FAST_FWD_START:
            CSlopeData.processdata.dietm_startpos = CSlopeData.processdata.admeter;
            CSlopeData.processdata.dietm_actstarttm_ms = GetMsTick();
			CSlopeData.processdata.dietm_fwd_ms[CSlopeData.processdata.slope_rf_num] = 0;
        case CAGGC_FAST_FWD_START + 1:
            if (MoveLocationMeterControlProc(CSlopeData.processdata.admeter, CSlopeData.config.pos_fwd[0]))
            {
                ACT_STEP_NEW(CAGGC_FAST_FWD_END);
            }

			if(CSlopeData.processdata.dietm_fwd_ms[CSlopeData.processdata.slope_rf_num] == 0)
			{
				if (MoveLocationMeterControlProc(CSlopeData.processdata.admeter, (CSlopeData.processdata.dietm_startpos + DIETMMOVEPOS)))
	            {
	                CSlopeData.processdata.dietm_fwd_ms[CSlopeData.processdata.slope_rf_num] = DIFF_MS(CSlopeData.processdata.dietm_actstarttm_ms);
	            }
			}
            
            CHK_SET_MOV(SetCAggCFastFwd);
            break;
        case CAGGC_FAST_FWD_END:
            CSlopeData.processdata.slope_max_fwd[CSlopeData.processdata.slope_rf_num] = 0;
        case CAGGC_FAST_FWD_END + 1:
            if (ACT_STEP_100MS > 12)
            {
                ACT_STEP_NEW(CAGGC_SLOW_FWD_START);
            }
            if (CSlopeData.processdata.slope_max_fwd[CSlopeData.processdata.slope_rf_num] < CSlopeData.processdata.ad_add)
            {
                CSlopeData.processdata.slope_max_fwd[CSlopeData.processdata.slope_rf_num] = CSlopeData.processdata.ad_add;
            }
            CHK_SET_MOV(CAggCFastFwdEnd);
            break;
        case CAGGC_SLOW_FWD_START:
        case CAGGC_SLOW_FWD_START + 1:
            if (MoveLocationMeterControlProc(CSlopeData.processdata.admeter, CSlopeData.config.pos_fwd[1]))
            {
                ACT_STEP_NEW(CAGGC_SLOW_FWD_END);
            }
			
			if(CSlopeData.processdata.slope_process_step == SLOPE_PRO_STEP_SCOPE_CONFIRM)
			{
				if(CalibrationSlopePosLenChk(pp, pact) == ACT_ERR_ABORT)
				{
					return ACT_ERR_ABORT;
				}
			}
            CHK_SET_MOV(SetCAggCSlowFwd);
            break;
        case CAGGC_SLOW_FWD_END:
        case CAGGC_SLOW_FWD_END + 1:
            if (ACT_STEP_100MS > 12)
            {
                if (CSlopeData.processdata.slope_process_step == SLOPE_PRO_STEP_SCOPE_CONFIRM)
                {
                    CSlopeData.processdata.slope_process_step = SLOPE_PRO_STEP_CYCLE_RF;
                    ACT_STEP_NEW(CAGGC_FAST_BWD_START);
                }
                else
                {
                    CSlopeData.processdata.slope_rf_num++;
                    if (CSlopeData.processdata.slope_rf_num >= SLOPERFNUMSUM)
                    {
                        ACT_STEP_NEW(CAGGC_END);
                    }
                    else
                    {
                        ACT_STEP_NEW(CAGGC_FAST_BWD_START);
                    }
                }
            }

            CHK_SET_MOV(CAggCSlowFwdEnd);
            break;
        case CAGGC_FAST_BWD_START:
            CSlopeData.processdata.dietm_startpos = CSlopeData.processdata.admeter;
            CSlopeData.processdata.dietm_actstarttm_ms = GetMsTick();
			CSlopeData.processdata.dietm_bwd_ms[CSlopeData.processdata.slope_rf_num] = 0;
        case CAGGC_FAST_BWD_START + 1:
            if (MoveLocationMeterControlProc(CSlopeData.config.pos_bwd[0], CSlopeData.processdata.admeter))
            {
                ACT_STEP_NEW(CAGGC_FAST_BWD_END);
            }

			if(CSlopeData.processdata.dietm_bwd_ms[CSlopeData.processdata.slope_rf_num] == 0)
			{
				if (MoveLocationMeterControlProc((CSlopeData.processdata.dietm_startpos - DIETMMOVEPOS), CSlopeData.processdata.admeter))
	            {
	                CSlopeData.processdata.dietm_bwd_ms[CSlopeData.processdata.slope_rf_num] = DIFF_MS(CSlopeData.processdata.dietm_actstarttm_ms);
	            }
			}
            
            CHK_SET_MOV(SetCAggCFastBwd);
            break;
        case CAGGC_FAST_BWD_END:
            CSlopeData.processdata.slope_max_bwd[CSlopeData.processdata.slope_rf_num] = 0;
        case CAGGC_FAST_BWD_END + 1:
            if (ACT_STEP_100MS > 12)
            {
                ACT_STEP_NEW(CAGGC_SLOW_BWD_START);
            }
            if (CSlopeData.processdata.slope_max_bwd[CSlopeData.processdata.slope_rf_num] < CSlopeData.processdata.ad_add)
            {
                CSlopeData.processdata.slope_max_bwd[CSlopeData.processdata.slope_rf_num] = CSlopeData.processdata.ad_add;
            }

            CHK_SET_MOV(CAggCFastBwdEnd);
            break;
        case CAGGC_SLOW_BWD_START:
        case CAGGC_SLOW_BWD_START + 1:
            if (MoveLocationMeterControlProc(CSlopeData.config.pos_bwd[1], CSlopeData.processdata.admeter))
            {
                ACT_STEP_NEW(CAGGC_SLOW_BWD_END);
            }

			if(CSlopeData.processdata.slope_process_step == SLOPE_PRO_STEP_SCOPE_CONFIRM)
			{
				if(CalibrationSlopePosLenChk(pp, pact) == ACT_ERR_ABORT)
				{
					return ACT_ERR_ABORT;
				}
			}
            CHK_SET_MOV(SetCAggCSlowBwd);
            break;
        case CAGGC_SLOW_BWD_END:
        case CAGGC_SLOW_BWD_END + 1:
            if (ACT_STEP_100MS > 12)
            {
            	if (CSlopeData.processdata.slope_process_step == SLOPE_PRO_STEP_SCOPE_CONFIRM)
            	{
                	ACT_STEP_NEW(CAGGC_SLOW_FWD_START);
            	}
				else
				{
					ACT_STEP_NEW(CAGGC_FAST_FWD_START);
				}
            }

            CHK_SET_MOV(CAggCSlowBwdEnd);
            break;
        case CAGGC_END:   
			//学习斜率-取后四组数据的平均值
            for (i = 0, slope_max_buf = 0; i < 4; i++)
            {
                slope_max_buf = slope_max_buf + CSlopeData.processdata.slope_max_fwd[i + 6];
            }
            CSlopeData.processdata.slope_max_fwd_result = slope_max_buf / 4;

            for (i = 0, slope_max_buf = 0; i < 4; i++)
            {
                slope_max_buf = slope_max_buf + CSlopeData.processdata.slope_max_bwd[i + 6];
            }
            CSlopeData.processdata.slope_max_bwd_result = slope_max_buf / 4;


			//学习死区时间-取死区时间最小值
			for (i = 1,CSlopeData.processdata.dietm_fwd_ms_result = CSlopeData.processdata.dietm_fwd_ms[0]; i < 10; i++)
            {
				if(CSlopeData.processdata.dietm_fwd_ms_result > CSlopeData.processdata.dietm_fwd_ms[i])
				{
					if(CSlopeData.processdata.dietm_fwd_ms[i] != 0)
					{
						CSlopeData.processdata.dietm_fwd_ms_result = CSlopeData.processdata.dietm_fwd_ms[i];
					}
				}
            }

			for (i = 1,CSlopeData.processdata.dietm_bwd_ms_result = CSlopeData.processdata.dietm_bwd_ms[0]; i < 10; i++)
            {
				if(CSlopeData.processdata.dietm_bwd_ms_result > CSlopeData.processdata.dietm_bwd_ms[i])
				{
					if(CSlopeData.processdata.dietm_bwd_ms[i] != 0)
					{
						CSlopeData.processdata.dietm_bwd_ms_result = CSlopeData.processdata.dietm_bwd_ms[i];
					}
				}
            }
			if(CSlopeData.config.pdietm_fwd != NULL)
			{
				*(CSlopeData.config.pdietm_fwd) = CSlopeData.processdata.dietm_fwd_ms_result;
			}
			if(CSlopeData.config.pdietm_bwd != NULL)
			{
				*(CSlopeData.config.pdietm_bwd) = CSlopeData.processdata.dietm_bwd_ms_result;
			}
			if(CSlopeData.config.pslope_max_fwd != NULL)
			{
				*(CSlopeData.config.pslope_max_fwd) = CSlopeData.processdata.slope_max_fwd_result/(UI32)CSlopeData.config.fwdmaxspd;
			}
			if(CSlopeData.config.pslope_max_bwd != NULL)
			{
				*(CSlopeData.config.pslope_max_bwd) = CSlopeData.processdata.slope_max_bwd_result/(UI32)CSlopeData.config.bwdmaxspd;
			}

			(*CSlopeData.config.updatafunc)();
			CSlopeData.config.updatafunc = NULL;
			CSlopeData.config.chkfunc = NULL;
			CSlopeData.config.config_flag = 0;
			
			SetOperMode(OM_MANUAL);
            ret = ACT_END;
            break;
        default:
			CSlopeData.config.updatafunc = NULL;
			CSlopeData.config.chkfunc = NULL;
			CSlopeData.config.config_flag = 0;
			
			SetOperMode(OM_MANUAL);
            ret = ACT_END;
            break;
    }


    if (ret == ACT_END || ret == ACT_ERR_ABORT)
    {
        return ret;
    }
    else
    {
        return CalibrationSlopeActBack(pp, pact);
    }
}
