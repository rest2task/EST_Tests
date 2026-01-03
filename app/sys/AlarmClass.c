#include "string.h"
#include "math.h"
#include "typedef.h"
#include "typelst_dic.h"
#include "machine.h"
#include "AlarmClass.h"
#include "oil.h"
#include "alarm.h"

void StandAlarm(BOOL bforce);    //标准警报
void TempAlarm(BOOL bforce);	//临时警报
void BSafeDoorAlarm(PSAFEDOOR pp);	//后安全门警报

void SpecialAlarm1(PPART pp)
{
	if(CUS_ID==CS_YUYAOLOG)
		StandAlarm(TRUE);	  //朗格在手动情况下，警报叫 ，2015.509
	else
		StandAlarm(FALSE);	  //手动情况下，警报不叫 ，2014.1107
}

void SpecialAlarm2(PPART pp)
{
	StandAlarm(ChkSysCfg(CO1_LUBW));
}

void SpecialAlarm3(PROBOT pp)
{
	if(pp->md->FL_ROBOTWAIT != 1)	//等待机械手是否警报
	{
		StandAlarm(TRUE);
	}
}
	
void SpecialTempAlarm1()
{
	TempAlarm(TRUE); 
}

const ALARM_CLASS alarmclass_tb[]=
{
	//机器部件
	{0			,	0,				ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,		TRUE},		//  默认警报等级3级，警报时数据默认上传
	{MACHINE_ID,	ER0_PNLS,		ALARM_CLASS_1st,(ACT_OP_FUNC1)SpecialAlarm1,TRUE},		//	#00100	急停键已按下
	{MACHINE_ID,	ER0_SKEY,		ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#00101	请按运行键
	{MACHINE_ID,	ER0_CYCL,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,		TRUE}, 		//	#00102	全程计时超时

	{MACHINE_ID,	ER0_MEM,		ALARM_CLASS_3rd,(ACT_OP_FUNC1)SpecialAlarm1,FALSE},		//	#00103	内存不足
	{MACHINE_ID,	ER0_TSKFUL,		ALARM_CLASS_2nd,(ACT_OP_FUNC1)SpecialAlarm1,FALSE},		//	#00104	任务控制列表满
	{MACHINE_ID,	ER0_TSKFORK,	ALARM_CLASS_2nd,(ACT_OP_FUNC1)SpecialAlarm1,FALSE},		//	#00105	动作插入失败
	{MACHINE_ID,	ER0_HYTBL,		ALARM_CLASS_3rd,(ACT_OP_FUNC1)SpecialAlarm1,FALSE},		//	#00107	联动动作无油路优先级表
	{MACHINE_ID,	ER0_IOLINK,		ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#00109	IO扩展板通讯失败
	{MACHINE_ID,	ER0_DALINK,		ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#0010A	DA扩展板通讯失败
	{MACHINE_ID,	ER0_TEMPLINK,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#0010B	温度扩展板通讯失败
	{MACHINE_ID,	ER0_QCPARACHK,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#0010C	QC监控模数计数已到 
	{MACHINE_ID,	ER0_PARTNULL,	ALARM_CLASS_3rd,(ACT_OP_FUNC1)SpecialAlarm1,FALSE},		//	#0010D	面板数据不全
	{MACHINE_ID,	ER0_IODELAYLIM,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#0010E	请等待上一动作完成
	{MACHINE_ID,	ER0_CFG_ERR,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#00113  配置数据错误
	{MACHINE_ID,	ER0_READ_ERR,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#00114  配置数据读取失败
	{MACHINE_ID,	ER1_SLOPE_CAL_POS_ERR, ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,TRUE},		//	#00115	校准行程异常
	{MACHINE_ID,	ER1_TMOUT,		ALARM_CLASS_2nd,ALARM_CLASS_OPR_NULL,		TRUE}, 		//	#001119 自动停留时间过长，转手动   20201112  yxh
    {MACHINE_ID,    ER0_METER_CLM,  ALARM_CLASS_2nd,ALARM_CLASS_OPR_NULL,       TRUE},      //  #00111 开关模电子尺数据跳变

    

	//马达部件
	{MOTOR_ID,		ER0_MTRS,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,		TRUE},		//	#04100	马达启动失败
	{MOTOR_ID,		ER0_MOVL,		ALARM_CLASS_1st,ALARM_CLASS_OPR_NULL,		TRUE},		//	#04101	马达过载 
	{MOTOR_ID,		ER0_HTMT,		ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#04102	马达未启动  
	{MOTOR_ID,		ER0_MTTMP,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,		TRUE},		//	#04103	电机温度过高
	{MOTOR_ID,		ER0_MOTORERR,	ALARM_CLASS_1st,ALARM_CLASS_OPR_NULL,		TRUE},	 	//	#04104	马达启停异常
	{MOTOR_ID,		ER0_ESDO,		ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#04105	ECAT_SDO发送超时，使能启动失败 
	{MOTOR_ID,		ER0_ESDO2,		ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#04106	ECAT_SDO发送超时，使能关闭失败 
	{MOTOR_ID,		ER0_SALVNUM,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#04109	从站个数不匹配
	{MOTOR_ID,		ER0_NETERR, 	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,		FALSE},		//	#0410A	ECAT网络配置错误 
	{MOTOR_ID,		ER0_SALVERR,	ALARM_CLASS_1st,ALARM_CLASS_OPR_NULL,		FALSE},		//	#0410B	从站配置错误
	{MOTOR_ID,		ER0_SLVLOFF,	ALARM_CLASS_1st,ALARM_CLASS_OPR_NULL,		TRUE},		//	#0410F	从站断线
	{MOTOR_ID,		ER1_MOTOREOUTTM,	ALARM_CLASS_1st,ALARM_CLASS_OPR_NULL,		TRUE},		// #04200 "电机反转超时"
	{MOTOR_ID,		ER1_MOTOREOUTRPM,	ALARM_CLASS_1st,ALARM_CLASS_OPR_NULL,		TRUE},		// #04201 "电机反转超速"
	{MOTOR_ID,		ER1_MOTORFREETIME,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		// #04202 "电机闲置关停"
	

	//IO部件
	{IO_ID,			ER0_H24V,		ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		// #06100	H24V电压异常
	{IO_ID,			ER0_IOV,		ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		// #06101	输出点短路
	{IO_ID, 		ER0_IO_FORCE,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE}, 	// #0610B	输入点强制激活中
	{IO_ID, 		ER0_POLINK,		ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE}, 	// #0610C	IO输出链表失败

	//电子尺部件
	{ADMETER_ID,	ER0_ADMETER1,	ALARM_CLASS_2nd,ALARM_CLASS_OPR_NULL,		TRUE},
	{ADMETER_ID,	ER0_ADMETER2,	ALARM_CLASS_2nd,ALARM_CLASS_OPR_NULL,		TRUE},
	{ADMETER_ID,	ER0_ADMETER3,	ALARM_CLASS_2nd,ALARM_CLASS_OPR_NULL,		TRUE},
	{ADMETER_ID,	ER0_ADMETER4,	ALARM_CLASS_2nd,ALARM_CLASS_OPR_NULL,		TRUE},
	{ADMETER_ID,	ER0_ADMETER5,	ALARM_CLASS_2nd,ALARM_CLASS_OPR_NULL,		TRUE},
	{ADMETER_ID,	ER0_ADMETER6,	ALARM_CLASS_2nd,ALARM_CLASS_OPR_NULL,		TRUE},
	{ADMETER_ID,	ER0_ADMETER7,	ALARM_CLASS_2nd,ALARM_CLASS_OPR_NULL,		TRUE},
	{ADMETER_ID,	ER0_ADMETER8,	ALARM_CLASS_2nd,ALARM_CLASS_OPR_NULL,		TRUE},
	{ADMETER_ID,	ER0_ADMETER_EXT1,	ALARM_CLASS_2nd,ALARM_CLASS_OPR_NULL,		TRUE},
	{ADMETER_ID,	ER0_ADMETER_EXT2,	ALARM_CLASS_2nd,ALARM_CLASS_OPR_NULL,		TRUE},
	{ADMETER_ID,	ER0_ADMETER_EXT3,	ALARM_CLASS_2nd,ALARM_CLASS_OPR_NULL,		TRUE},
	{ADMETER_ID,	ER0_ADMETER_EXT4,	ALARM_CLASS_2nd,ALARM_CLASS_OPR_NULL,		TRUE},
	{ADMETER_ID,	ER0_ADMETER_EXT5,	ALARM_CLASS_2nd,ALARM_CLASS_OPR_NULL,		TRUE},
	{ADMETER_ID,	ER0_ADMETER_EXT6,	ALARM_CLASS_2nd,ALARM_CLASS_OPR_NULL,		TRUE},
	{ADMETER_ID,	ER0_ADMETER_EXT7,	ALARM_CLASS_2nd,ALARM_CLASS_OPR_NULL,		TRUE},
	{ADMETER_ID,	ER0_ADMETER_EXT8,	ALARM_CLASS_2nd,ALARM_CLASS_OPR_NULL,		TRUE},

	//液压部件
	{HYDR_ID,		ER0_PRSZSET,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#09101	压力传感器未归零

	//伺服部件
	{SERVO_ID,		ER5_SVCTLERR,	ALARM_CLASS_1st,ALARM_CLASS_OPR_NULL,		TRUE},		//	#0A100	伺服驱动器故障
	{SERVO_ID,		ER5_POWERERR,	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,		FALSE},		//	#0A102	总电源相序错误
	{SERVO_ID,		ER5_PRESSERR,	ALARM_CLASS_1st,ALARM_CLASS_OPR_NULL,		TRUE},		//	#0A103	外控压力不足 
	{SERVO_ID,		ER1_ECATRECFG,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#0A111	重新配置EtherCAT
	{SERVO_ID,		ER1_NETSTEP1,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#0A118	网络初始化第1 次连接
	{SERVO_ID,		ER1_NETSTEP2,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#0A119	网络初始化第2 次连接
	{SERVO_ID,		ER1_NETSTEP3,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#0A11A	网络初始化第3 次连接
	{SERVO_ID,		ER1_NETSTEP4,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#0A11B	网络初始化第4 次连接
	{SERVO_ID,		ER1_NETSTEP5,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#0A11C	网络初始化第5 次连接
	{SERVO_ID,		ER1_NETSTEP6,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#0A11D	网络初始化第6 次连接
	{SERVO_ID,		ER1_NETSTEP7,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#0A11E	网络初始化第7 次连接
	{SERVO_ID,		ER1_NETSTEP8,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#0A11F	网络初始化第8 次连接
	
    //警报部件
    {ALARM_ID,		ER0_PNLS_EXT,   ALARM_CLASS_1st,ALARM_CLASS_OPR_NULL,		TRUE},		// #0B100  外部急停键已按下

	//Oil部件
	{OIL_ID,		ER1_FILT,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,		TRUE},		// #0C100  液压油滤网阻塞
	{OIL_ID,		ER1_OILV,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,		TRUE},		// #0C101  液压油位低 
	{OIL_ID,		ER0_OILT,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,		TRUE},		// #0C102  油温偏差 
	{OIL_ID,		ER0_OILLEAK,	ALARM_CLASS_1st,ALARM_CLASS_OPR_NULL,		TRUE},		// #0C103  漏油 
	
	//Ecat部件
	{ECAT_ID,		ER0_SDOSENDERR,ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		// #0D100	sdo发数据失败
	{ECAT_ID,		ER0_SDOREADERR,ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		// #0D100	sdo读数据失败
	{ECAT_ID,		ER0_ECAT_CTOUT,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		// #0D104	程序周期超时

	//新温度部件
	{TEMPCTRLCOMM_ID,	ER0_WUPT_COMM,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,FALSE},	// #0E100；预温计时未到  
	{TEMPCTRLCOMM_ID,	ER0_HTON_COMM,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,FALSE},	// #0E101；温度没有上升 
	{TEMPCTRLCOMM_ID,	ER0_TMPHLD_COMM,	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,FALSE},	// #0E102；取消保温模式
	{TEMPCTRLCOMM_ID,	ER0_HEATTOUCH_COMM,	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,FALSE},	// #0E103；加热接触器异常
	{TEMPCTRLCOMM_ID,	ER0_TEMPER_COMM, 	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #0E104；料筒温度偏差
	{TEMPCTRLCOMM_ID,	ER0_COORER_COMM,	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #0E105；落料口温度偏高
	{TEMPCTRLCOMM_ID,	ER0_STDY_COMM,		ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,FALSE},	// #0E106；温度优化开启
	{TEMPCTRLCOMM_ID,	ER0_STDYER_COMM,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,FALSE},	// #0E107；温度优化条件不符合
	{TEMPCTRLCOMM_ID,	ER0_SYNCER_COMM,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,FALSE},	// #0E108；未进行自学习，同步升温无效
	{TEMPCTRLCOMM_ID,	ER0_E502_ERR,		ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,FALSE},	// #0E109；E502扩展板通讯失败
	{TEMPCTRLCOMM_ID,	ER0_TEMP_SET_LOW,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,FALSE},	// #0E10A；温度设定值须大于150度
	{TEMPCTRLCOMM_ID,	ER0_TEMP_REAL_HIGH,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,FALSE},	// #0E10B；温度实际值太高
	{TEMPCTRLCOMM_ID,	ER5_E502_J_INVALID,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,FALSE},	// #0E10C: E502热电偶选择失败
	{TEMPCTRLCOMM_ID,	ER1_TEMP_ERRPRO, 	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #0E10D; 温度异常(热电偶短线:1,2,3/热电偶反接:4,5/温度读取失败:6)

	//pump部件
	{PUMP_ID,		ER0_SERVO_ERR,	ALARM_CLASS_1st,ALARM_CLASS_OPR_NULL,TRUE},		// #0F100	伺服警报

	//注射部件
	{INJECT_ID,		ER0_CUSH,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #10100	射出监控异常
	{INJECT_ID,		ER0_PURG,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #10101	射出护罩未关
	{INJECT_ID, 	ER0_HPEM,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE}, 	// #10102	储料超时
	{INJECT_ID, 	ER0_SUCK,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE}, 	// #10106	射退终未到定位
	{INJECT_ID,		ER0_CHGL,		ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,TRUE},		// #1010C	料位过低
	{INJECT_ID,		ER0_CHFL,		ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,FALSE},		// #1010D	储料位置已到 
	{INJECT_ID,		ER0_WARM,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #1010E	电热未开启 
	{INJECT_ID,		ER0_PRTM,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #1010F	射出保护时间到
	{INJECT_ID,		ER1_NOTMPCHK,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,FALSE},   //   #10112    '请注意，料筒温度检测已关闭'

    //储料下压缸部件
    {CHGPRS_ID, 	ER0_CHGPRSBWD,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,TRUE},		// #13100	压料缸退到位

	//模座部件
	{CLAMP_ID,		ER0_CLSM,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #20100	关模未到定位
	{CLAMP_ID,		ER0_CLSP,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #20102  	关模保护失败  
	{CLAMP_ID,		ER0_OPNM,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #20103	开模未到定位  
	{CLAMP_ID,		ER0_SHOT,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,FALSE},	// #20105  	开模数已到
	{CLAMP_ID,		ER0_PACK,		ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,FALSE},	// #20106 	开模包装数己到
	{CLAMP_ID,		ER0_OPNS,		ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,FALSE},	// #20107	开模一慢未到定位
	{CLAMP_ID,		ER0_AIRPRES,	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #2010A  	气压过低
	{CLAMP_ID,		ER0_MESA,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE}, 	// #2010B	机械安全异常  
	{CLAMP_ID,		ER0_HYSEN,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE}, 	// #2010C	液压安全或电眼保护异常
	{CLAMP_ID,		ER0_MDHEAT,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE}, 	// #2010D	模温偏差
	{CLAMP_ID,		ER0_CLSLOCK,	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,FALSE},	// #2010F  	合模锁死保护失败
	{CLAMP_ID,		ER0_UPGAERR,	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #20110	合模上顶盖异常
	{CLAMP_ID,		ER0_ADMETER,	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #20111	开关模电子尺数据跳变
	{CLAMP_ID,		ER0_SDRVALVE,	ALARM_CLASS_1st,ALARM_CLASS_OPR_NULL,TRUE},		// #20112   安全阀检测异常
	{CLAMP_ID,		ER0_ERRPRU,		ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,FALSE},	// #20113  产品不良
	{CLAMP_ID,		ER1_OPNMPOSERR,	ALARM_CLASS_2nd,ALARM_CLASS_OPR_NULL,TRUE},		// #20116	开模位置异常，停止机械手

	//托膜部件
	{EJECT_ID,		ER0_EJTP,		ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,FALSE},	// #21100	托模未到定位  
	{EJECT_ID,		ER0_EJTB,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},	    // #21100	托模未到定位  
	{EJECT_ID,		ER0_EJTM,		ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,FALSE},	// #21102  	托模方式错误
	{EJECT_ID,		ER0_EJTPM,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #21103   模具顶针保护

	//座台部件
	{NOZZLE_ID,		ER0_NZLIMT,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #11100	座进终未到位
	
	//润滑部件
	{LUB_ID,		ER1_LUBL,		ALARM_CLASS_3rd,(ACT_OP_FUNC1)SpecialAlarm2,FALSE},	//	#30100	润滑1 油位低
	{LUB_ID,		ER1_LUBR,		ALARM_CLASS_3rd,(ACT_OP_FUNC1)SpecialAlarm2,FALSE},	//	#30101	润滑1 压力未到 
	{LUB_ID,		ER1_LUBERR,		ALARM_CLASS_1st,ALARM_CLASS_OPR_NULL,		FALSE},	//	#30102 	润滑1 压力开关故障

	//中子部件
	{CORE_ID,		ER0_CORE,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #22100-#22600  ; 中子A~F未到定位
	{CORE_ID,		ER0_CEFF,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #22101-#22601  ; 中子A~F动作位置偏差
	{CORE_ID,		ER0_ROB2_CPRF,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,FALSE},	// #22102-#22602 等待机械手    -中子A-F 进安全   	
	{CORE_ID,		ER0_ROB2_CPRB,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,FALSE},	// #22103-#22603 等待机械手    -中子A-F 退安全 
	{CORE_ID,		ER0_SITE,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #22106-#22606 ; 中子A~F进退限位器同时有信号
	{CORE_ID,		ER0_EJTF_CRF,	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #22107-#22607 ; 托模进时，中子A~F进未到定位
	{CORE_ID,		ER0_EJTF_CRB,	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #22108-#22608 ; 托模进时，中子A~F退未到定位
	{CORE_ID,		ER0_EJTB_CRF,	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #22109-#22609 ; 托模退时，中子A~F进未到定位
	{CORE_ID,		ER0_EJTB_CRB,	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #2210A-#2260A ; 托模退时，中子A~F退未到定位
	{CORE_ID,		ER0_CRF_EJTB,	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #2210B-#2260B ; 中子A~F进时，托模退未到定位
	{CORE_ID,		ER0_CLS_CRF,	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #2210C-#2260C ; 关模时，中子A~F进未到定位
	{CORE_ID,		ER0_CLS_CRB,	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #2210D-#2260D ; 关模时，中子A~F退未到定位
	{CORE_ID,		ER0_OPN_CRF,	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #2210E-#2260E ; 开模时，中子A~F进未到定位
	{CORE_ID,		ER0_OPN_CRB,	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #2210F-#2260F ; 开模时，中子A~F退未到定位
	{CORE_ID,		ER0_SEQ_CRF,	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #22110-#22610 ; 顺序检查:中子A~F进未到定位
	{CORE_ID,		ER0_SEQ_CRB,	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #22111-#22611 ; 顺序检查:中子A~F退未到定位
	{CORE_ID,		ER0_CRF_POS,	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #22112-#22612 ; 中子A~F进动作位置偏差
	{CORE_ID,		ER0_CRB_POS,	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		// #22113-#22613 ; 中子A~F退动作位置偏差

	//吹气部件
	{AIRBLOW_ID,	ER0_AIRM,		ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,FALSE},	// #23100-23E00	等待吹气1~14完成  	
	{AIRBLOW_ID,	ER0_SEQ,		ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,FALSE},	// #23101-23E01	等待顺序阀1~14完成

	//循环部件
	{AUTOCTRL_ID,	ER0_DROP,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,FALSE},	// #32100   落料电眼检出异常    
	{AUTOCTRL_ID,	ER0_DRFUL,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,FALSE},	// #32101	电眼检测物料已满
	
	//调模部件
	{ADJUST_ID,		ER0_FAAD,		ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,FALSE},	// #24100	完成自动调模
	{ADJUST_ID,		ER0_ADJL,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,FALSE},	// #24101	已到达调模终点
	{ADJUST_ID,		ER0_ADJS,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,FALSE},	// #24102	调模电眼失败
	{ADJUST_ID, 	ER0_ADJF,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,FALSE},	// #24103	已到达调模进终
	{ADJUST_ID,		ER0_ADJB,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,FALSE},	// #24104	已到达调模退终

	//安全门部件
	{SAFEDOOR_ID,	ER0_CSDR,		ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,FALSE},	//	#29100	安全门未关   
	{SAFEDOOR_ID,	ER0_OSDR,		ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,FALSE},	//	#29101	请开安全门
	{SAFEDOOR_ID,	ER3_RSDR,		ALARM_CLASS_1st,(ACT_OP_FUNC1)BSafeDoorAlarm,TRUE},//	#29102	背面安全门未关
	{SAFEDOOR_ID,	ER3_FSER,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		//	#29103	安全门限位开关异常
	{SAFEDOOR_ID,	ER5_PDSA,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		//	#29109	自动门安全异常
	{SAFEDOOR_ID,	ER5_SQ42,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		//	#2910A	安全阀控制开关异常
	{SAFEDOOR_ID,	ER5_SAVALVE,	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		//	#2910B	安全阀异常
	{SAFEDOOR_ID,	ER5_PRESAVALVE,	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		//	#2910C	安全阀先导阀异常
	{SAFEDOOR_ID,	ER5_SACTLERR,	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,TRUE},		//	#2910F	门机异常
	{SAFEDOOR_ID,	ER1_SAVALVEOPN,	ALARM_CLASS_2nd,ALARM_CLASS_OPR_NULL,TRUE}, 	// 	#29206	安全阀未开

	//机械手部件
	{ROBOT_ID,		ER0_ROBO,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,		TRUE},		//	#31100	机械手异常
	{ROBOT_ID,		ER0_ROBE,		ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,		TRUE},		//	#31101	机械手紧急停止  
	{ROBOT_ID,		ER0_ROB2,		ALARM_CLASS_3rd,(ACT_OP_FUNC1)SpecialAlarm3,FALSE},		//	#31102	等待机械手  	
	{ROBOT_ID,		ER0_ROB2_EJTF,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#31104	等待机械手 -ROB顶进安全
	{ROBOT_ID,		ER0_ROB2_EJTB,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#31105	等待机械手 -ROB顶退安全
	{ROBOT_ID,		ER0_ROB2_CLSM,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#31106	等待机械手 -ROB允许关模
	{ROBOT_ID,		ER0_ROB2_OPNM,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#31107	等待机械手 -ROB允许开模
	{ROBOT_ID,		ER0_ROB2_RCYC,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#31108	等待机械手 -ROB循环启动
	{ROBOT_ID,		ER0_ROB2_MAFR,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#31109	等待机械手-ROB模区安全
	{ROBOT_ID,		ER0_ROB2_IMML,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#3110A	等待机械手-ROB机械手联锁
	{ROBOT_ID,		ER0_ROB_TURN,	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,		TRUE},		//	#31115  机械手异常-输入点未变化 
	{ROBOT_ID,		ER0_Mold2,		ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#31117	等待模具保护	

	{TEMP_MW,		ER0_HTON_MW,	ALARM_CLASS_3rd,ALARM_CLASS_OPR_NULL,		FALSE}, 	// 	#2F101；模温温度没有上升  
	{TEMP_MW,		ER0_STDY_MW,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#2F104；模温温度优化开启
	{TEMP_MW,		ER0_STDYER_MW,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#2F105；模温温度优化条件不符合
	{TEMP_MW,		ER0_SET_LOW_MW,	ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE},		//	#2F106；模温温度设定值须大于150度
	{TEMP_MW,		ER0_REAL_HIGH_MW,ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE}, 	// 	#2F107；模温温度实际值太高
	{TEMP_MW,		ER0_J_INVALID_MW,ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE}, 	//	#2F108:	模温热电偶选择失败
	{TEMP_MW,		ER0_DATA_FAILED,ALARM_CLASS_4th,ALARM_CLASS_OPR_NULL,		FALSE}, 	//	#2F10A	模温(1-7)参数发送失败
};

WORD	alarmclass_index[MMI_USED_ID*32] = {0};		//用于快速索引

int	alarm_num = 0;	//插入警报个数
UI32 g_alarmdatabuf = 0;	//设定输出警报前，最近设定的警报id
UI32	alarmclass_alarmcache[ALARM_CACHE_NUM] = {0};	//发出警报的ERR记录，cache内存在ERR时，便不会停止警报

/*******************************************************************************************
 *Brief			:初始化索引表，将警报信息表各条警报的数组编号写入索引表中
 *Param			:log2()以2为底的对数，计算第几位数据
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20200220
********************************************************************************************/
void InitAlarmClassIndex()
{
	memset(alarmclass_index, 0, sizeof(alarmclass_index));	//对索引清空

	for (WORD i = 0 ;i < ARRAY_NUM(alarmclass_tb);i++)
	{
		if ((alarmclass_tb[i].part_id*32 + log2(alarmclass_tb[i].alarm_id)) < ARRAY_NUM(alarmclass_index))
		{
			alarmclass_index[(WORD)(alarmclass_tb[i].part_id*32 + log2(alarmclass_tb[i].alarm_id))] = i;
		}
	}
}

/*******************************************************************************************
 *Brief			:通过部件id及警报id从索引表中获得索引编号，然后通过编号得到对应警报信息
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20200220
********************************************************************************************/
PALARM_CLASS GetAlarmClassInfo(UID partid,UI32 alarmid)
{
	UI16 alarmid_merge = partid*32 + log2(alarmid);
	UI16 id = 0;
	
    if ((id  = alarmclass_index[alarmid_merge]) < ARRAY_NUM(alarmclass_tb))
    {
        return (PALARM_CLASS)&(alarmclass_tb[id]);
    }
	else
	{
		return (PALARM_CLASS)&(alarmclass_tb[0]);
	}
}


/*******************************************************************************************
 *Brief			:对设定警报做备案
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20210223
********************************************************************************************/
void InsAlarmBuf(UID partid,UI32 alarmid)
{
	UI16 errnum = log2(alarmid)+1;

	g_alarmdatabuf = (partid << 16) | errnum;
}

/*******************************************************************************************
 *Brief			:插入ERR进入警报缓存，警报缓存内存在ERR时，输出警报不停止
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20200215
********************************************************************************************/
void InsAlarmInAlarmCache(void)
{
	UI16 i = 0,j = 0;	
	UI32 alarmdata;
	UI16 getcache = 0;

	alarmdata = g_alarmdatabuf;	
	
	for (i = 0; i < ALARM_CACHE_NUM; i++)
	{
		//找到空缓存位置，预留
		if ((alarmclass_alarmcache[i] == 0) && (getcache == 0))
		{
			getcache = 1;
			j = i;			
		}

		//如果缓存内已经存在当前的ERR，那么退出不需要保存
		if (alarmclass_alarmcache[i] == alarmdata)
		{
			return;
		}
	}	

	//无保存当前ERR，将ERR保存在缓存内
	if (getcache > 0)
	{
		alarmclass_alarmcache[j] = alarmdata;
		alarm_num++;
	}
}

/*******************************************************************************************
 *Brief			:从警报缓存中删除ERR，当警报缓存中无数据时，输出警报停止
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20200215
********************************************************************************************/
void DelAlarmInAlarmCache(UID partid,UI32 alarmid)
{
	UI16 i = 0;	
	UI16 errnum = log2(alarmid)+1;
	UI32 alarmdata;

	alarmdata = (partid<<16) | errnum;
	for (i = 0; i < ALARM_CACHE_NUM; i++)
	{
		//找到相同ERR后，删除ERR
		if (alarmclass_alarmcache[i] == alarmdata)
		{
			alarmclass_alarmcache[i] = 0;
			alarm_num--;
		}
	}	

	if(alarm_num < 0 || alarm_num  > ALARM_CACHE_NUM)
	{
		alarm_num = 0;
	}
}

void ClrAlarmCache()
{
	UI16 i = 0;
	UI32 alarmdata = (0x0F <<16) | 1;//伺服警报
	UI16 alarmnum = 0;
	
	for (i = 0; i < ALARM_CACHE_NUM; i++)
	{
		if (alarmclass_alarmcache[i] == alarmdata)//伺服警报不清除
		{			
			alarmnum++;
		}
		else
		{
			alarmclass_alarmcache[i] = 0;
		}
	}
	alarm_num = alarmnum;
}

BOOL ChkAlarmIsEmpty()
{
	return alarm_num == 0;
}

