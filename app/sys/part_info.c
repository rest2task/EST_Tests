#include "com.h"
#include "part.h"
#include "acttype.h"
#include "basectrl.h"
#include "machine.h"


const PART_TYPE_INFO part_type_info[] = 
{
	{MACHINE_ID, PART_CAT_SYS, sizeof(MACHINE), InitMachinePart, MachineCmd, SUB_BIT(MH_ID)|SUB_BIT(HY_ID), SUB_BIT(ST_ID) | SUB_BIT(VR_ID)},
	{CLAMP_ID, PART_CAT_ACT, sizeof(CLAMP), InitClampPart, ClampCmd, 0, SUB_BIT(ST_ID)},
	{INJECT_ID, PART_CAT_ACT, sizeof(INJECT), InitInjectPart, InjectCmd, 0, SUB_BIT(ST_ID)},
	{NOZZLE_ID, PART_CAT_ACT, sizeof(NOZZLE), InitNozzlePart, NozzleCmd, 0, SUB_BIT(ST_ID)},
	{EJECT_ID, PART_CAT_ACT, sizeof(EJECT), InitEjectPart, EjectCmd, 0, SUB_BIT(ST_ID)},
	{CORE_ID, PART_CAT_ACT, sizeof(CORE), InitCorePart, CoreCmd, 0, SUB_BIT(ST_ID)},
	{ADJUST_ID, PART_CAT_ACT, sizeof(ADJUST), InitAdjustPart, AdjustCmd, 0, SUB_BIT(ST_ID)},
	{INJSTOR_ID, PART_CAT_ACT, sizeof(INJSTOR), InitInjStorPart, NULL, 0, SUB_BIT(ST_ID)},
	{SAFEDOOR_ID, PART_CAT_ACT, sizeof(SAFEDOOR), InitSafeDoorPart, SafeDoorCmd, 0, SUB_BIT(ST_ID)},
	{AIRBLOW_ID, PART_CAT_SUB_SRAM, sizeof(AIRBLOW), InitAirBlowPart, AirBlowCmd, 0, SUB_BIT(ST_ID)},
	{SHUT_ID, PART_CAT_SUB, sizeof(SHUT), InitShutPart, ShutCmd, 0, SUB_BIT(ST_ID)},
	{SYSACC_ID, PART_CAT_SUB, sizeof(SYSACC), InitSysAccPart, NULL, 0, SUB_BIT(ST_ID)},
	{ROBOT_ID, PART_CAT_SUB, sizeof(ROBOT), InitRobotPart, NULL, 0, SUB_BIT(ST_ID)},
	{LUB_ID, PART_CAT_SUB, sizeof(LUB), InitLubPart, LubCmd, 0, SUB_BIT(ST_ID)},
	{AUTOCTRL_ID, PART_CAT_SUB, sizeof(AUTOCTRL), InitAutoCtrlPart, AutoCtrlCmd, 0, SUB_BIT(ST_ID)},
	{CHGPRS_ID, PART_CAT_SUB, sizeof(CHGPRS), InitChgPressPart, NULL, 0, SUB_BIT(ST_ID)},
	{MOTOR_ID, PART_CAT_BCTRL, sizeof(MOTOR), InitMotorPart, MotorCmd, 0, SUB_BIT(ST_ID)},
	{HYDR_ID, PART_CAT_BCTRL, sizeof(HYDR), InitHydrPart, HydrCmd, SUB_BIT(ZD_ID), SUB_BIT(ST_ID)},
	{SERVO_ID, PART_CAT_BCTRL, sizeof(SERVO), NULL, NULL, 0, SUB_BIT(ST_ID)},
	{IO_ID, PART_CAT_BCTRL, sizeof(IO), InitIOPart, NULL, 0, SUB_BIT(ST_ID)},
	{ADMETER_ID, PART_CAT_BCTRL, sizeof(ADMETER), InitAdMeterPart, AdMeterCmd, SUB_BIT(MH_ID), SUB_BIT(ST_ID)},
	{ECAT_ID,PART_CAT_BCTRL, sizeof(SERVOECAT), InitServoEcatPart, ServoEcatCmd,0,SUB_BIT(ST_ID)},
	{DA_ID, PART_CAT_BCTRL, sizeof(DA), InitDAPart, DACmd, SUB_BIT(MH_ID), SUB_BIT(ST_ID)},
	{ALARM_ID, PART_CAT_BCTRL, sizeof(ALARM), InitAlarmPart, NULL, 0, SUB_BIT(ST_ID)},
	{OIL_ID, PART_CAT_BCTRL, sizeof(OIL), InitOilPart, OilCmd, 0, SUB_BIT(ST_ID)},
	{LOCK_ID, PART_CAT_SUB, sizeof(LOCK), InitLockPart, NULL, 0, SUB_BIT(ST_ID)},
	{PUMP_ID,	PART_CAT_SUB, sizeof(PUMP), InitPumpPart, PumpCmd,SUB_BIT(MH_ID),SUB_BIT(ST_ID)},
	{TEMPCTRL_ID, PART_CAT_BCTRL, sizeof(TEMPCTRL), InitTempCtrlPart, TempCtrlCmd, SUB_BIT(ZD_ID), SUB_BIT(ST_ID)},
	{TEMPCTRLCOMM_ID, PART_CAT_SUB_SRAM, sizeof(TEMPCTRLCOMM),InitTempCtrlCommPart,TempCtrlCommCmd,SUB_BIT(ZD_ID),SUB_BIT(ST_ID)},
	{TEMP_MW, PART_CAT_SUB_SRAM, sizeof(TEMPCTRLMW),InitTempMwPart,TempMwCmd,0,SUB_BIT(ST_ID)},
};

const UI16 part_type_info_num = ARRAY_NUM(part_type_info);

