#ifndef _MACHINE_DIC_H_
#define _MACHINE_DIC_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif
#include "typedef.h"
#include "admeter.h"


#pragma pack(1)

typedef struct tyEN_MACHINE_PI
{
    UI16  I_E_PO1;                  //0x00110000 特殊输入1
    UI16  I_E_PO2;                  //0x00110001 特殊输入2
    UI16  I_E_PO3;                  //0x00110002 特殊输入3
    UI16  I_E_PO4;                  //0x00110003 特殊输入4
    UI16  I_E_PO5;                  //0x00110004 特殊输入5
    UI16  I_E_PO6;                  //0x00110005 特殊输入6
    UI16  I_E_PO7;                  //0x00110006 特殊输入7
    UI16  I_E_PO8;                  //0x00110007 特殊输入8
    UI16  I_E_PO9;                  //0x00110008 特殊输入9
    UI16  I_E_PO10;                 //0x00110009 特殊输入10
    UI16  I_E_PO11;                 //0x0011000A 特殊输入11
    UI16  I_E_PO12;                 //0x0011000B 特殊输入12
    UI16  I_E_PO13;                 //0x0011000C 特殊输入13
    UI16  I_E_PO14;                 //0x0011000D 特殊输入14
    UI16  I_E_PO15;                 //0x0011000E 特殊输入15     //循环启动按键一
    UI16  I_E_PO16;                 //0x0011000F 特殊输入16     //循环启动按键二

}MACHINE_PI, *PMACHINE_PI;

typedef struct tyEN_MACHINE_PO
{
    UI16  HYD_VLV2;                //0x00100000 二油路阀
    UI16  HYD_COMB;                //0x00100001 合流阀
    UI16  SOEC_SLM;                //0x00100002 联动阀
    UI16  FST_SOL ;                //0x00100003 快速阀
    UI16  CP_PUMP;                 //0x00100004 特殊中子
    UI16  O_OPN_EJE_COR;           //0x00100005 开模顶出中子阀                  ; 伺服多泵，复合动作使用 chg by rch 2012-12-26 mov from clamp
    UI16  O_E_PO1;                 //0x00100006 特殊输出1
    UI16  O_E_PO2;                 //0x00100007 特殊输出2
    UI16  O_E_PO3;                 //0x00100008 特殊输出3
    UI16  O_E_PO4;                 //0x00100009 特殊输出4
    UI16  O_E_PO5;                 //0x0010000A 特殊输出5
    UI16  O_E_PO6;                 //0x0010000B 特殊输出6
    UI16  O_E_PO7;                 //0x0010000C 特殊输出7
    UI16  O_E_PO8;                 //0x0010000D 特殊输出8
    UI16  O_E_PO9;                 //0x0010000E 特殊输出9
    UI16  O_E_PO10;                //0x0010000F 特殊输出10
    UI16  O_E_PO11;                //0x00100010 特殊输出11
    UI16  O_E_PO12;                //0x00100011 特殊输出12
    UI16  O_E_PO13;                //0x00100012 特殊输出13
    UI16  O_E_PO14;                //0x00100013 特殊输出14
    UI16  O_E_PO15;                //0x00100014 特殊输出15
    UI16  O_E_PO16;                //0x00100015 特殊输出16               
}MACHINE_PO, *PMACHINE_PO;

#define MACHINE_CONF_LEN		24
typedef struct 	tyEN_MACHINE_MACHSET
{
    UI16 FL_MACHINECTRL0;       		   // 0x00120000  Code0：机器动作控制码 80H
    UI16 FL_MACHINECTRL1;                  // 0x00120001  Code1：机器动作控制码 82H
    UI16 FL_MACHINECTRL2;                  // 0x00120002  Code2：机器动作控制码 84H
    UI16 FL_MACHINECTRL3;                  // 0x00120003  Code3：机器动作控制码 86H
    UI16 FL_MACHINECTRL4;                  // 0x00120004  Code4：机器动作控制码 88H
    UI16 FL_MACHINECTRL5;                  // 0x00120005  Code5：机器动作控制码 8AH
    UI16 FL_MACHINECTRL43;                 // 0x00120006  Code43		;D6H
    UI16 FL_SERVOMODE;                     // 0x00120007  Code16		;D8H	//伺服控制模式配置
    UI16 FL_MACHINTERNAL0;                 // 0x00120008  code48
    UI16 FL_MACHINTERNAL1;                 // 0x00120009  Code49：E2
    UI16 FL_MACHINTERNAL2;                 // 0x0012000A  Code50：E4
    UI16 FL_MACHINTERNAL3;                 // 0x0012000B  Code51：E6
    UI16 FL_MACHINTERNAL4;                 // 0x0012000C  Code52：E8
    UI16 FL_MACHINTERNAL5;                 // 0x0012000D  Code53：EA
    UI16 FL_MACHINTERNAL6;                 // 0x0012000E  Code54：EC
    UI16 FL_HYDRAUIC;                      // 0x0012000F  油路控制;90H
    UI16 FL_TEMPER;                        // 0x00120010  温度控制    ;92H
    UI16 FL_METER;                         // 0x00120011  位置尺控制  ;94H
    UI16 FL_MACHTYPE;					   // 0x00120012  机器类型: 目前只使用bit 0 -3,其他位保留置0， 0 = 卧式机, 1 = 立式机
    UI16 FL_MACHINESAFE;                   // 0x00120013  机器国家安全标准  //20170505  LET
    UI16 FL_reserve[MACHINE_CONF_LEN - 20];// 0x00120014    ~   0x00120017

    UI16 FL_CUSTOMID;                      // 0x00120018  厂商代码：客户及机器代码    ;8CH	高8位表示客户ID，低8位表示客户机型序号
    UI16 WIO_DATE;                         // 0x00120019  code7：  ;8EH
    UI16 WDSR_DATE;                        // 0x0012001A  code11：;96H
    UI16 FL_PATCHS;                        // 0x0012001B  code12：;98H
    UI16 FL_CTRLMODEL;                     // 0x0012001C  系统代码：控制器型号        ;9AH 	A310/A610/A910/A620/A920
    UI16 FL_VERSION;                       // 0x0012001D  主机版本：程式版本          ;9CH 	FVMSS, F: 0 -1表示标准版本，2:表示特殊版本，3:临时程序, V:1-9主版本号，M:0-9次版本号, SS:0-99开发序号-909
    UI16 FL_CTRLDATE;                      // 0x0012001E  程序日期：程式日期          ;9EH	MMDDY格式，年只有个位数-7117
    UI16 FL_ROTLAC;                        // 0x0012001F  滑模使用选择  0：不使用  1：单滑模   2：直角机  3：双滑模              默认0

    UI16 FL_HYDRAUSEC;                     // 0x00120020  速度分段输出      ;CEH
    UI16 DAPRES_SYSLMT;                    // 0x00120021  系统压力上限      ;D0H
    UI16 DAPRES_CLSHILMT;                  // 0x00120022  设定压力上限      ;D2H-140
    UI16 CN_CHGRPMHOLE;                    // 0x00120023  储料RPM孔数       ;D4H
    UI16 WCONFIGRSV9;                      // 0x00120024  保留
    UI16 WCONFIGRSR10;                     // 0x00120025  保留
    UI16 FL_MACHINEMODEL;                  // 0x00120026  推力座型号：;DEH

    UI16 DAPRES_CLPLIMIT;                  // 0x00120027  开关模压力上限             //默认140//暂无使用
    UI16 DAPRES_ADJUSTLIMIT;               // 0x00120028  调模压力上限              //默认70//暂无使用
    UI16 DAPRES_LIMIT;					   // 0x00120029  小动作压力上限             //默认140//暂无使用

    UI16 DAFLOW_NOZZLELIMIT;			   // 0x0012002A  座出速度上限          unknowfb  2011-11-7     //默认99//暂无使用
    UI16 DAFLOW_ADJUSTLIMIT;			   // 0x0012002B  调模速度上限         fb  2011-11-7      //默认99//暂无使用
    UI16 DAFLOW_EJECTLIMIT;                // 0x0012002C  托模速度上限       //默认99//暂无使用
    UI16 DAFLOW_HOLDLIMIT;                 // 0x0012002D  保压速度上限       //默认99//暂无使用
    UI16 DAPRES_BACKLIMIT;                 // 0x0012002E  储料背压压力上限   默认70//默认70//暂无使用
    UI16 DAPRES_CHARGELIMIT;			   // 0x0012002F  储料压力上限              //默认140//暂无使用
    UI16 DAFLOW_CHARGELIMIT;			   // 0x00120030  储料速度上限           //默认99//暂无使用

    UI16 INJSTOR_MODE;                     // 0x00120031  0=不使用 1=压力继电器 2=压力传感器 ////暂无作用
    UI16 INJSTOR_HYPORT;                   // 0x00120032  蓄能器油路选择         0=系统油路 1=独立油路 默认0

    UI16 WCONFIGRSV0;                      // 0x00120033  保留
    UI16 WCONFIGRSV1;                      // 0x00120034  保留
    UI16 WCONFIGRSV2;                      // 0x00120035  保留
    UI16 WCONFIGRSV3;                      // 0x00120036  保留
    UI16 WCONFIGRSV4;                      // 0x00120037  保留
    UI16 WCONFIGRSV5;                      // 0x00120038  保留
    UI16 DA_LINE;                          // 0x00120039  保留  DA线性类型，用于上传给面板 0   直线   1 含底流    //20170505  LET
    UI16 CD_TYPE;						   // 0x0012003A  默认:0 0:F518(继电器) 1:F519(IO)	2:自动识别	3:F518(手动) 4:F519(手动)
}MACHINE_MACHSET, *PMACHINE_MACHSET;

typedef struct tyEN_MACHINE_MOLDSET
{

    UI16 FL_OPNDURINGCOOL;						/* 0x00130000   储料开模联动 0-不使用 1-使用 默认0              */
    UI16 FL_INJMOLDIN;							/* 0x00130001   压塑功能 0-不使用 1-射出後 2-射出一後 3-排气次数 4-关模途中 默认0 */
    UI16 TM_CYCLEPAUSE_L;						/* 0x00130002   再循环计时低16位  在开关模画面     范围0.00-3600.00 默认0.00s       20200219 */
    UI16 FL_OPNEJTCORE;							/* 0x00130003   开模联动      0=不使用 1-托膜 2-中子A 3-中子B 4-中子C 5-中子D 6-中子E 7-中子F 默认0*/

    //滑模功能选择  	/*chg by rch, 2012-12-27, mov from ROTATE_MOLDSET*/
    UI16  FL_SCLPCTRL;							/* 0x00130004   滑模功能选择  0：左滑模  1：右滑模   2: 双滑模  */
    UI16  CN_AUTOALARMUNIT;						/* 0x00130005   自动警报启动模数 范围0-99 默认0              */
    UI16  TMCYCLEDEFAULTMAX_L;					/* 0x00130006   全程计时上限低16位   在警报参数画面中      范围0.00-6500.00   默认300.00s  	20200219*/
    UI16  FL_HYTBLSEL;							/* 0x00130007   非联动状态是否使用油路优先级表: 0 使用，1: 不使用*/
    UI16  TMDLY_LINKVLV;						/* 0x00130008   联动阀延时开计时        .01s   默认0.00*/
    UI16  TMDLY_LINKVLVOFF;						/* 0x00130009   联动阀延时关延时        .01s   默认0.00*/
    UI16  POWER_CTRL;							/* 0x0013000A   电表功能: 0 不使用，1使用。默认不使用*/
    UI16  CURRENT_RATIO;						/* 0x0013000B   电流变比; 范围 1 - 9999 ，默认为1*/
	UI16  SRV3;									/* 0x0013000C   备用     左右滑模选择，只面板使用*/
    UI16  Qc_Fun;								/* 0x0013000D   QC监控功能  0: 使用  1: 不使用  默认不使用*/
    UI16  Qc_OpnNum;							/* 0x0013000E   QC监控模数  50~200  默认50*/
    UI16  Qc_DeviationSet_CycTm;				/* 0x0013000F   周期时间允许偏差     默认0.00  范围0.00-9.99    20200219*/
    UI16  Qc_TmSet_CycTm;						/* 0x00130010   周期时间设定值   默认0.00   范围0.00-9.99    20200219*/
    UI16  Qc_Mode_CycTm;						/* 0x00130011   周期时间监控:0:不使用  1:使用   默认不使用*/
    UI16  FL_CLOSJTCORE;						/* 0x00130012   关摸连动     0-不用   1-中子A 2-中子B     3-中子C 4-中子D 5-中子E 6-中子F 默认0*/
    UI16  FL_SpcMode;							/* 0x00130013   特殊连动功能    0   不使用    1   使用*/
    UI16  FL_ADMETER;							/* 0x00130014   电子尺数据保护   0   不使用  1   使用   默认使用*/
    //20190403 CHJ 补齐
    UI16  FL_METER_INJ;							/* 0x00130015   射出电子尺跳变  		范围1000-10000mm/s     默认1000mm/s  */
    UI16  FL_METER_CLAMP;						/* 0x00130016   开关模电子尺跳变  	范围1000-10000mm/s     默认1000mm/s  */
    UI16  FL_METER_EJE;							/* 0x00130017   托模电子尺跳变  		范围1000-10000mm/s     默认1000mm/s  */
	UI16  FL_TIME_ERR;							/* 0x00130018   全程计时警报停机方式  0   报警立停   1   开模完     默认0*/
	UI16  TM_CYCLEPAUSE_H;						/* 0x00130019   再循环计时高16位       范围0.00-3600.00 默认0.00s       20200219 */
	UI16  TMCYCLEDEFAULTMAX_H;					/* 0x0013001A   全程计时上限高16位         范围0.00-6500.00   默认300.00s  	20200219*/
	UI16  RECYCLE_AIR;                			/* 0x0013001B      循环启动键使用   0   不使用   1  使用      默认不使用  20200730*/
	UI16  FL_VALUE;								/* 0x0013001C 阀门功能 0-不使用 1-使用 默认0-不使用*/
    UI16  CUSTOM_PARA[15];						/* 0x0013001D - 0013002B 自定义参数*/        /*  20200219   */	

	UI16  UPPER_CP_VERSION1;					/* 0x0013002C 上位机版本号1*/
	UI16  UPPER_CP_VERSION2;					/* 0x0013002D 上位机版本号2*/
	UI16  UPPER_CP_CTRLDATE1;					/* 0x0013002E 上位机程序日期1*/
	UI16  UPPER_CP_CTRLDATE2;					/* 0x0013002F 上位机程序日期2*/

	UI16  FL_HYDY_WAY_SET;						/* 0x00130030  油路切换选择0=默认,1=厂家油路,2=配置油路,3=开储联动,4=开托联动,5=开中联动*/	
	UI16  FL_HYDY_WAY_SET_MODE;					/* 0x00130031  油路切换方式	0:固定	1:联动时*/
	UI16  FL_WET_DRY_CYCLES;					/* 0x00130032 干循环功能 0:关闭 1:开启	*(该数据不保存，开启后下电关闭)*/
    UI16  TM_IOFORCE_OUT;		                /* 0x00130033 输出点强制保护时间 范围1-15分 默认10分 */
}MACHINE_MOLDSET, *PMACHINE_MOLDSET;

typedef struct tyMACHINE_HYDR
{
    UI16  DAPRES_CLSHIAVR;						//锁模压力平均值
}MACHINE_HYDR, *PMACHINE_HYDR;

/****************版本信息********************************/
typedef struct tyHW_VERINFO
{
    UI16	test_tag: 1;
    UI16	ver_sn: 7;
    UI16	reserve: 4;
    UI16	cpu_type: 4;
    UI16	model: 13;
    UI16	cat: 3;
}HW_VERINFO, *PHW_VERINFO;

typedef struct tyHW_TYPEINFO
{
    UI16	drv: 4;
    UI16	mec: 4;					//0: 空， 1:卧式机， 2:立式机
    UI16	arch: 4;
    UI16	model: 4;
}HW_TYPEINFO, *PHW_TYPEINFO;

typedef struct tySW_VERINFO
{
    UI16	db_ver;
    UI16	boot_ver_1;				//basesw_ver->boot_ver_1，boot版本年，
    UI16	boot_ver_2;				//basesw_ver->boot_ver_2，boot版本月日
    UI16	bbdd_ver;				//原os_ver改为bbdd_ver，存储新版本规则中的基线和开发版本号。  //20170505  LET
    //UI8		os_type;
}SW_VERINFO, *PSW_VERINFO;

typedef struct tyCUS_CFGINFO
{
    UI16	sw_ver;			//程序版本号
    UI8		date[3];		//程序日期
    UI8		cus_sn;			//客户代码
}CUS_CFGINFO, *PCUS_CFGINFO;

/*内部保存到FLASH的数据*/   //20170505  LET
typedef struct tyINNER_PARA
{
    UI16	pa1;           	//ADDA线性上一次修改后的类型保存
    UI16	pa2;            //ADDA线性上一次保存后的类型保存

	UI16	Err_Rs232;		//RS232通讯异常报警记录
	UI16	Err_Temp;		//料筒温度偏差异常报警记录
	UI16	Err_Adinj;		//射出电子尺数据异常记录，主要用于记录速度过快的情况
	UI16	Err_Adcmp;		//开关模电子尺数据异常记录，主要用于记录速度过快的情况
	UI16	Err_Adeje;		//托模电子尺数据异常记录，主要用于记录速度过快的情况
}INNER_PARA, *PINNER_PARA;

typedef struct tyMH_TYPEINFO
{
    UI16    power_type;             //动力类型:      1=液压；2=混电；3=全电
    UI16    machine_type;           //机器类型:      1=卧式；2=立式；3=油压
    UI16    application_type;       //应用类型(根据《产品程序版本及程序文件命名规范V2.3》来填写)    
}MH_TYPEINFO,*PMH_TYPEINFO;

typedef struct tyEN_MACHINE_VERSION
{
    HW_VERINFO		hw_ver;					//0x00140000
    HW_TYPEINFO		hw_type;				//0x00140002
    SW_VERINFO		sw_ver;					//0x00140003	硬件boot信息
    CUS_CFGINFO		cus_info;				//0x00140007

    /**/
    INNER_PARA		saved_pa;    //20170505  LET
	MH_TYPEINFO   	mh_info;
}MACHINE_VERSION, *PMACHINE_VERSION;
/**********************************************************/

typedef struct 	tyEN_MACHINE_STATE
{
    UI16  FL_ERROR0;							//0x00160000 警报1
    UI16  FL_ERROR1;							//0x00160001 警报2
    UI16  ST_WOPERMODE;					 		//0x00160002 操作状态
    UI16  WTIME_AGE; 		         			//0x00160003 老练时间送上位机
    UI16  FL_AUTOALARM;							//0x00160004 自动警报状态 2012-11-28
    UI16  TM_CYCLE_L;							//0x00160005 循环计时 低16位   在主画面中显示的全程时间  20200219    
    UI16  TM_CYCLE_MAX_L;						//0x00160006 循环最大值低16位在警报画面显示的变量  20200219
    UI16  TM_OPER_LOWORD;						//0x00160007 开机时间， MS
    UI16  TM_OPER_HIGHWORD;						//0x00160008 开机时间， MS
    UI16  TM_CYCLE_LAST_L;						//0x00160009 制品时间低16位，不包括再循环时间   20200219

    UI16  ENERGY_LAST;							//0x0016000A 本次循环消耗的电能
    UI16  ENERGY_LASTALL;						//0x0016000B 开机后总消耗的电能

    UI16  POWER_CYCLE;							//0x0016000C 本次循环总能耗
    UI16  POWER_OTHERS;							//0x0016000D 本次循环其他能耗
    UI16  POWER_CLAMPCLS;						//0x0016000E 本次循环关模能耗
    UI16  POWER_CLAMPOPN;						//0x0016000F 本次循环开模能耗
    UI16  POWER_INJECT;							//0x00160010 本次循环射出能耗
    UI16  POWER_CHARGE;							//0x00160011 本次循环储料能耗
    UI16  POWER_SUCKBACK;						//0x00160012 本次循环射退能耗
    UI16  POWER_EJECT;							//0x00160013 本次循环托模能耗

    UI16  EXTIOST;								//0x00160014 IO扩展板状态
    UI16  EXTDAST;								//0x00160015 DA扩展板状态
    UI16  EXTTEMPST;							//0x00160016 温度扩展板状态
    UI16  EXTHSPDTEMPST;						//0x00160017 模温扩展板状态
    UI16  Qc_Opn_Cnt;							//0x00160018 QC监控模数
    UI16  Qc_Avg_CycTm_L;						//0x00160019 周期时间平均值低16位	默认0.00    /*20200219*/
    UI16  Qc_Deviation_Max_CycTm_L;				//0x0016001A 周期时间最大偏差值低16位	默认0.00    /*20200219*/
    UI16  Qc_Unq_Cnt_CycTm;						//0x0016001B 周期时间不合格数
    UI16  Qc_Unq_Rate_CycTm;					//0x0016001C 周期时间不合格率
    UI16  Qc_Max_CycTm_L;						//0x0016001D 周期时间最大值低16位	默认0.00    /*20200219*/
    UI16  Qc_Min_CycTm_L;						//0x0016001E 周期时间最小值 低16位  默认0.00    /*20200219*/
    UI16  TM_CYCLE_WHOLE_L;						//0x0016001F 全程计时低16位，包括再循环时间，开关安全门   /*20200219*/
    UI16  E301_SW_VER;							//0x00160020 E301扩展板软件版本
    UI16  Step_Precycle;						//0x00160021 前置再循环实时动作步
    UI16  Step_RecycleSemi;						//0x00160022 半自动再循环实时动作步
    UI16  Step_RecycleTime;						//0x00160023 全自动再循环实时动作步
    UI16  Step_RecycleSesor;					//0x00160024 电眼自动再循环实时动作步

	UI16  ERR_RS232_CNT;						//0x00160025	RS232警报次数			20190410
	UI16  ERR_TEMP_CNT;							//0x00160026	温度警报次数			20190410
	UI16  ERR_ADCMP_CNT;						//0x00160027	开关模电子尺跳变次数	20190410
	UI16  ERR_ADINJ_CNT;						//0x00160028	射出电子尺跳变次数		20190410
	UI16  ERR_ADEJE_CNT;						//0x00160029	托模电子尺跳变次数		20190410
	UI16  EXT_E505_ST;							//0x0016002A	E505扩展板状态		20190410
	UI16  EXT_E502_ST;							//0x0016002B  E502扩展板状态		20190410
    UI16  TM_CYCLE_H;					  		//0x0016002C 循环计时 高16位   /*20200219*/    
    UI16  TM_CYCLE_LAST_H;						//0x0016002D 制品时间高16位，不包括再循环时间   /*20200219*/
    UI16  TM_CYCLE_WHOLE_H;						//0x0016002E 全程计时高16位，包括再循环时间，开关安全门   /*20200219*/
    UI16  TM_CYCLE_MAX_H;						//0x0016002F 循环最大值高16位  在警报画面显示的变量 /*20200219*/
    UI16  Qc_Max_CycTm_H;						//0x00160030 周期时间最大值高16位	默认0.00    /*20200219*/
    UI16  Qc_Min_CycTm_H;						//0x00160031 周期时间最小值高16位  默认0.00    /*20200219*/
    UI16  Qc_Avg_CycTm_H;						//0x00160032 周期时间平均值高16位	默认0.00    /*20200219*/
    UI16  Qc_Deviation_Max_CycTm_H;				//0x00160033 周期时间最大偏差值高16位	默认0.00    /*20200219*/
	UI16  EXT_E501_ST;							//0x00160034 E501扩展板状态		
	UI16  MAIN_CYCLE_TMOUT;						//0x00160035 主循环超时次数
	UI16  DA_E700B_VER;							//0x00160036	E700B版本号
	UI16  WRITE_CNT_E700B;						//0x00160037	E700B通讯测定写入次数		
	UI16  READ_CNT_E700B;						//0x00160038	E700B通讯测试读入次数	
	UI16  IO_E40xB_VER[4];						//0x00160039-0x0016003C	IO扩展板版本号

	UI16  MODOPENNUM0;							//0x0016003D	开模数 高6位(用于能耗页面显示)			
	UI16  MODOPENNUM1;							//0x0016003E  开模数 低16位(用于能耗页面显示)	
	UI16  ENERGY_TOTAL_L;						//0x0016003F 总能耗 低16位		单位:度 ；2位小数
	UI16  ENERGY_TOTAL_H;						//0x00160040 总能耗 高16位			单位:度 ；2位小数
	UI16  ENERGY_AUTO_L;						//0x00160041 自动能耗 低16位 	单位:度 ；1位小数
	UI16  ENERGY_AUTO_H;						//0x00160042 自动能耗 高16位 	单位:度 ；1位小数
	UI16  POWER_L;								//0x00160043 总功率 低16位		单位:KW  ; 2位小数
	UI16  POWER_H;								//0x00160044 总功率高16位		单位:KW  ; 2位小数
	UI16  VOL_A;								//0x00160045  A相电压 ;   单位V	 ; 2位小数
	UI16  VOL_B;								//0x00160046  B相电压; 单位V	  ; 2位小数
	UI16  VOL_C;								//0x00160047  C相电压; 单位V	  ; 2位小数
	UI16  VOL_AB;								//0x00160048  AB相电压;	单位V	  ; 2位小数
	UI16  VOL_BC;								//0x00160049  BC相电压;	单位V	  ; 2位小数
	UI16  VOL_AC;								//0x0016004A  AC相电压;	单位V	  ; 2位小数			
	UI16  CURRENT_A;							//0x0016004B  A相电流 ;	单位A	; 2位小数
	UI16  CURRENT_B;							//0x0016004C  B相电流 ; 单位A	 ; 2位小数
	UI16  CURRENT_C;							//0x0016004D  C相电流 ; 单位A	 ; 2位小数
	UI16  VOL_LN;								//0x0016004E  相线-零线总电压;  单位V	 ; 2位小数
	UI16  VOL_LL;								//0x0016004F  相线-相线总电压;  单位V	 ; 2位小数
	UI16  WRITE_CNT_POWER;                      //0x00160050
    UI16  READ_CNT_POWER;                       //0x00160051
    UI16  PUMP_MAIN;                            //0x00160052  动力通道 (普通机模式下区分动力通道和比例阀通道)
	UI16  SERVO_TYPE[MAX_SERVO_NUM*2];          //0x00160053 - 0x00160062 伺服类型，1:580  2:650/680  3:750轴1 4:750轴2
}MACHINE_STATE, *PMACHINE_STATE;

#pragma pack()

#ifdef DIC_ENUM_NEED

enum EN_MACHINE_ACT_SN
{
    OS_RCYC = 1,			// 0   ;;'再循环  '
    OS_PRCYC,               // 15  ;;'前置再循环'
	OS_CALI,				// 斜率校准
    MACHINE_ACT_SN_END
};

#define		ER0_PNLS 			0x0001			//#00100 '急停键已按下   '
#define		ER0_SKEY			0x0002			//#00101 '请按运行键    '
#define		ER0_CYCL			0x0004			//#00102 '全程计时超时'
#define		ER0_MEM             0x0008	 		//#00103 '内存不足'
#define		ER0_TSKFUL          0x0010			//#00104 '任务控制列表满'
#define		ER0_TSKFORK         0x0020			//#00105 '动作插入失败'
#define		ER0_TSKCMD          0x0040			//#00106 '动作任务命令失败'
#define		ER0_HYTBL			0x0080			//#00107 '联动动作无油路优先级表'
#define		ER0_ENTM			0x0100			//#00108 '电表未连接'
#define		ER0_IOLINK          0x0200			//#00109 'IO扩展板通讯失败'
#define		ER0_DALINK          0x0400			//#0010A 'DA扩展板通讯失败'
#define		ER0_TEMPLINK		0x0800			//#0010B '温度扩展板通讯失败'
#define		ER0_QCPARACHK       0x1000			//#0010C 'QC监控模数计数已到    '//20170505  LET
#define		ER0_PARTNULL		0x2000			//#0010D '面板数据不全'
#define		ER0_IODELAYLIM      0x4000			//#0010E '请等待上一动作完成'
#define		ER0_E301LINK		0x8000			//#0010F 'E301扩展板通讯失败'
//20190403 CHJ 补齐
#define		ER0_METER_INJ		0x10000			//#00110 射出电子尺数据跳变
#define		ER0_METER_CLM		0x20000			//#00111 开关模电子尺数据跳变
#define		ER0_METER_EJE		0x40000			//#00112 托模电子尺数据跳变
#define		ER0_CFG_ERR			0x80000			//#00113 -100204  配置数据错误		20190626 yxh
#define		ER0_READ_ERR		0x100000		//#00114 -100205  配置数据读取失败		20190827 yxh
#define 	ER1_SLOPE_CAL_POS_ERR	0x200000		//#00115 '校准行程异常'
#define		ER1_VER_ERR			0x400000		//#001116	主机面板版本不匹配
#define		ER1_SRAMERR         0x800000	 	//#001117 'SRAM读写异常'
#define		ER0_PAYMENT			0x1000000		//#001118'分期付款未解除'
#define		ER1_TMOUT			0x2000000		//#001119	自动停留时间过长，转手动   20201112  yxh
#define		ER1_FORKTBLFULL		0x4000000		// #0011A - #0010020B 动作链表已满 2024-12-10 ccz
#define		ER1_LINKPORTTBLFULL 0x8000000		// #0011B - #0010020C 自定义联动油路表已满 2024-12-10 ccz
#define		ER1_PROTFULL        0x10000000		// #0011C - #0010020D 串口数据已满 2024-12-24 ccz
#define		ER1_PARTFULL        0x20000000		// #0011D - #0010020E 部件总数已满 2025-2-24 ccz
#define		ER1_DATAINTERR      0x40000000		// #0011E - #0010020F 数据交互异常 2025-3-4 ccz

#endif

#ifdef __cplusplus
}
#endif

#endif
