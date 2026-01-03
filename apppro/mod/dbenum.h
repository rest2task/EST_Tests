#ifndef _DBENUM_H
#define _DBENUM_H

enum db_machine1_PO{
  d_machine1_PO_HYD_VLV2 = 0x00100000, // 0x00100000, 二油路阀
  d_machine1_PO_HYD_COMB,              // 0x00100001, 合流阀
  d_machine1_PO_SOEC_SLM,              // 0x00100002, 联动阀
  d_machine1_PO_FST_SOL,               // 0x00100003, 快速阀
  d_machine1_PO_CP_PUMP,               // 0x00100004, 特殊中子
  d_machine1_PO_O_OPN_EJE_COR,         // 0x00100005, 开模顶出中子阀
  d_machine1_PO_O_E_PO1,               // 0x00100006, 特殊输出1
  d_machine1_PO_O_E_PO2,               // 0x00100007, 特殊输出2
  d_machine1_PO_O_E_PO3,               // 0x00100008, 特殊输出3
  d_machine1_PO_O_E_PO4,               // 0x00100009, 特殊输出4
  d_machine1_PO_O_E_PO5,               // 0x0010000a, 特殊输出5
  d_machine1_PO_O_E_PO6,               // 0x0010000b, 特殊输出6
  d_machine1_PO_O_E_PO7,               // 0x0010000c, 特殊输出7
  d_machine1_PO_O_E_PO8,               // 0x0010000d, 特殊输出8
  d_machine1_PO_O_E_PO9,               // 0x0010000e, 特殊输出9
  d_machine1_PO_O_E_PO10,              // 0x0010000f, 特殊输出10
  d_machine1_PO_O_E_PO11,              // 0x00100010, 特殊输出11
  d_machine1_PO_O_E_PO12,              // 0x00100011, 特殊输出12
  d_machine1_PO_O_E_PO13,              // 0x00100012, 特殊输出13
  d_machine1_PO_O_E_PO14,              // 0x00100013, 特殊输出14
  d_machine1_PO_O_E_PO15,              // 0x00100014, 特殊输出15
  d_machine1_PO_O_E_PO16               // 0x00100015, 特殊输出16
};

enum db_machine1_PI{
  d_machine1_PI_I_E_PO1 = 0x00110000, // 0x00110000, 特殊输入1
  d_machine1_PI_I_E_PO2,              // 0x00110001, 特殊输入2
  d_machine1_PI_I_E_PO3,              // 0x00110002, 特殊输入3
  d_machine1_PI_I_E_PO4,              // 0x00110003, 特殊输入4
  d_machine1_PI_I_E_PO5,              // 0x00110004, 特殊输入5
  d_machine1_PI_I_E_PO6,              // 0x00110005, 特殊输入6
  d_machine1_PI_I_E_PO7,              // 0x00110006, 特殊输入7
  d_machine1_PI_I_E_PO8,              // 0x00110007, 特殊输入8
  d_machine1_PI_I_E_PO9,              // 0x00110008, 特殊输入9
  d_machine1_PI_I_E_PO10,             // 0x00110009, 特殊输入10
  d_machine1_PI_I_E_PO11,             // 0x0011000a, 特殊输入11
  d_machine1_PI_I_E_PO12,             // 0x0011000b, 特殊输入12
  d_machine1_PI_I_E_PO13,             // 0x0011000c, 特殊输入13
  d_machine1_PI_I_E_PO14,             // 0x0011000d, 特殊输入14
  d_machine1_PI_I_E_PO15,             // 0x0011000e, 特殊输入15
  d_machine1_PI_I_E_PO16              // 0x0011000f, 特殊输入16
};

enum db_machine1_MACHSET{
  d_machine1_MACHSET_FL_MACHINECTRL0 = 0x00120000, // 0x00120000, Code0
  d_machine1_MACHSET_FL_MACHINECTRL1,              // 0x00120001, Code1
  d_machine1_MACHSET_FL_MACHINECTRL2,              // 0x00120002, Code2
  d_machine1_MACHSET_FL_MACHINECTRL3,              // 0x00120003, Code3
  d_machine1_MACHSET_FL_MACHINECTRL4,              // 0x00120004, Code4
  d_machine1_MACHSET_FL_MACHINECTRL5,              // 0x00120005, Code5
  d_machine1_MACHSET_FL_MACHINECTRL43,             // 0x00120006, Code43
  d_machine1_MACHSET_FL_SERVOMODE,                 // 0x00120007, Code16
  d_machine1_MACHSET_FL_MACHINTERNAL0,             // 0x00120008, Code48
  d_machine1_MACHSET_FL_MACHINTERNAL1,             // 0x00120009, Code49
  d_machine1_MACHSET_FL_MACHINTERNAL2,             // 0x0012000a, Code50
  d_machine1_MACHSET_FL_MACHINTERNAL3,             // 0x0012000b, Code51
  d_machine1_MACHSET_FL_MACHINTERNAL4,             // 0x0012000c, Code52
  d_machine1_MACHSET_FL_MACHINTERNAL5,             // 0x0012000d, Code53
  d_machine1_MACHSET_FL_MACHINTERNAL6,             // 0x0012000e, Code54
  d_machine1_MACHSET_FL_HYDRAUIC,                  // 0x0012000f, Code8
  d_machine1_MACHSET_FL_TEMPER,                    // 0x00120010, Code9
  d_machine1_MACHSET_FL_METER,                     // 0x00120011, Code10
  d_machine1_MACHSET_FL_MACHTYPE,                  // 0x00120012, 机器类型
  d_machine1_MACHSET_FL_reserve_1,                 // 0x00120013, 机器国家安全标准
  d_machine1_MACHSET_FL_reserve_2,                 // 0x00120014, 保留
  d_machine1_MACHSET_FL_reserve_3,                 // 0x00120015, 保留
  d_machine1_MACHSET_FL_reserve_4,                 // 0x00120016, 保留
  d_machine1_MACHSET_FL_reserve_5,                 // 0x00120017, 保留
  d_machine1_MACHSET_FL_CUSTOMID,                  // 0x00120018, Code6
  d_machine1_MACHSET_WIO_DATE,                     // 0x00120019, Code7
  d_machine1_MACHSET_WDSR_DATE,                    // 0x0012001a, Code11
  d_machine1_MACHSET_FL_PATCHS,                    // 0x0012001b, Code12
  d_machine1_MACHSET_FL_CTRLMODEL,                 // 0x0012001c, 系统代码
  d_machine1_MACHSET_FL_VERSION,                   // 0x0012001d, Code14
  d_machine1_MACHSET_FL_CTRLDATE,                  // 0x0012001e, Code15
  d_machine1_MACHSET_FL_ROTLAC,                    // 0x0012001f, 滑模功能
  d_machine1_MACHSET_FL_HYDRAUSEC,                 // 0x00120020, 速度选择
  d_machine1_MACHSET_DAPRES_SYSLMT,                // 0x00120021, 系统压力上限
  d_machine1_MACHSET_DAPRES_CLSHILMT,              // 0x00120022, 当前系统压力上限
  d_machine1_MACHSET_CN_CHGRPMHOLE,                // 0x00120023, 储料RPM孔数
  d_machine1_MACHSET_WCONFIGRSV9,                  // 0x00120024, 保留
  d_machine1_MACHSET_WCONFIGRSR10,                 // 0x00120025, 保留
  d_machine1_MACHSET_FL_MACHINEMODEL,              // 0x00120026, 十字头型号
  d_machine1_MACHSET_DAPRES_CLPLIMIT,              // 0x00120027, 开关模压力上限
  d_machine1_MACHSET_DAPRES_ADJUSTLIMIT,           // 0x00120028, 调模压力上限
  d_machine1_MACHSET_DAPRES_LIMIT,                 // 0x00120029, 小动作压力上限
  d_machine1_MACHSET_DAFLOW_NOZZLELIMIT,           // 0x0012002a, 座台速度上限
  d_machine1_MACHSET_DAFLOW_ADJUSTLIMIT,           // 0x0012002b, 调模速度上限
  d_machine1_MACHSET_DAFLOW_EJECTLIMIT,            // 0x0012002c, 托模速度上限
  d_machine1_MACHSET_DAFLOW_HOLDLIMIT,             // 0x0012002d, 保压速度上限
  d_machine1_MACHSET_DAPRES_BACKLIMIT,             // 0x0012002e, 储料背压压力上限
  d_machine1_MACHSET_DAPRES_CHARGELIMIT,           // 0x0012002f, 储料压力上限
  d_machine1_MACHSET_DAFLOW_CHARGELIMIT,           // 0x00120030, 储料速度上限
  d_machine1_MACHSET_INJSTOR_MODE,                 // 0x00120031, 
  d_machine1_MACHSET_INJSTOR_HYPORT,               // 0x00120032, 蓄能器油路选择
  d_machine1_MACHSET_WCONFIGRSV0,                  // 0x00120033, 保留
  d_machine1_MACHSET_WCONFIGRSV1,                  // 0x00120034, 保留
  d_machine1_MACHSET_WCONFIGRSV2,                  // 0x00120035, 保留
  d_machine1_MACHSET_WCONFIGRSV3,                  // 0x00120036, 保留
  d_machine1_MACHSET_WCONFIGRSV4,                  // 0x00120037, 保留
  d_machine1_MACHSET_WCONFIGRSV5,                  // 0x00120038, 保留
  d_machine1_MACHSET_WCONFIGRSV6,                  // 0x00120039, DA线性类型
  d_machine1_MACHSET_CD_TYPE                       // 0x0012003a, 默认
};

enum db_machine1_MOLDSET{
  d_machine1_MOLDSET_FL_OPNDURINGCOOL = 0x00130000, // 0x00130000, 储料开模联动
  d_machine1_MOLDSET_FL_INJMOLDIN,                  // 0x00130001, 压塑功能
  d_machine1_MOLDSET_TM_CYCLEPAUSE,                 // 0x00130002, 再循环计时
  d_machine1_MOLDSET_FL_OPNEJTCORE,                 // 0x00130003, 开模联动选择
  d_machine1_MOLDSET_FL_SCLPCTRL,                   // 0x00130004, 滑模使用选择
  d_machine1_MOLDSET_CN_AUTOALARMUNIT,              // 0x00130005, 自动警报启动模数
  d_machine1_MOLDSET_TMCYCLEDEFAULTMAX,             // 0x00130006, 全程计时上限
  d_machine1_MOLDSET_FL_HYTBLSEL,                   // 0x00130007, 使用油路优先级表
  d_machine1_MOLDSET_TMDLY_LINKVLV,                 // 0x00130008, 联动阀延迟开计时
  d_machine1_MOLDSET_TMDLY_LINKVLVOFF,              // 0x00130009, 联动阀延迟关计时
  d_machine1_MOLDSET_SRV1,                          // 0x0013000a, 电表功能
  d_machine1_MOLDSET_SRV2,                          // 0x0013000b, 电表数值
  d_machine1_MOLDSET_SRV3,                          // 0x0013000c, 滑模左右选择
  d_machine1_MOLDSET_Qc_Fun,                        // 0x0013000d, QC监控功能
  d_machine1_MOLDSET_Qc_OpnNum,                     // 0x0013000e, QC监控模数
  d_machine1_MOLDSET_Qc_DeviationSet_CycTm,         // 0x0013000f, 周期时间允许偏差
  d_machine1_MOLDSET_Qc_TmSet_CycTm,                // 0x00130010, 周期时间设定值
  d_machine1_MOLDSET_Qc_Mode_CycTm,                 // 0x00130011, 周期时间监控
  d_machine1_MOLDSET_FL_CLOSJTCORE,                 // 0x00130012, 关摸连动
  d_machine1_MOLDSET_FL_SpcMode,                    // 0x00130013, 特殊联动
  d_machine1_MOLDSET_FL_ADMETER,                    // 0x00130014, 电子尺数据保护
  d_machine1_MOLDSET_FL_METER_INJ,                  // 0x00130015, 射出电子尺跳变
  d_machine1_MOLDSET_FL_METER_CLAMP,                // 0x00130016, 开关模电子尺跳变
  d_machine1_MOLDSET_FL_METER_EJE,                  // 0x00130017, 托模电子尺跳变
  d_machine1_MOLDSET_FL_TIME_ERR,                   // 0x00130018, 全程计时警报停机方式
  d_machine1_MOLDSET_TM_CYCLEPAUSE_H,               // 0x00130019, 再循环计时
  d_machine1_MOLDSET_TMCYCLEDEFAULTMAX_H,           // 0x0013001a, 全程计时上限
  d_machine1_MOLDSET_RECYCLE_AIR,                   // 0x0013001b, 循环启动键使用
  d_machine1_MOLDSET_FL_VALUE,                      // 0x0013001c, 阀门功能
  d_machine1_MOLDSET_CUSTOM_PARA3,                  // 0x0013001d, 
  d_machine1_MOLDSET_CUSTOM_PARA4,                  // 0x0013001e, 
  d_machine1_MOLDSET_CUSTOM_PARA5,                  // 0x0013001f, 
  d_machine1_MOLDSET_CUSTOM_PARA6,                  // 0x00130020, 
  d_machine1_MOLDSET_CUSTOM_PARA7,                  // 0x00130021, 
  d_machine1_MOLDSET_CUSTOM_PARA8,                  // 0x00130022, 
  d_machine1_MOLDSET_CUSTOM_PARA9,                  // 0x00130023, 
  d_machine1_MOLDSET_CUSTOM_PARA10,                 // 0x00130024, 
  d_machine1_MOLDSET_CUSTOM_PARA11,                 // 0x00130025, 
  d_machine1_MOLDSET_CUSTOM_PARA12,                 // 0x00130026, 
  d_machine1_MOLDSET_CUSTOM_PARA13,                 // 0x00130027, 
  d_machine1_MOLDSET_CUSTOM_PARA14,                 // 0x00130028, 
  d_machine1_MOLDSET_CUSTOM_PARA15,                 // 0x00130029, 
  d_machine1_MOLDSET_CUSTOM_PARA16,                 // 0x0013002a, 
  d_machine1_MOLDSET_CUSTOM_PARA17,                 // 0x0013002b, 
  d_machine1_MOLDSET_UPPER_CP_VERSION1,             // 0x0013002c, 上位机版本号1
  d_machine1_MOLDSET_UPPER_CP_VERSION2,             // 0x0013002d, 上位机版本号2
  d_machine1_MOLDSET_UPPER_CP_CTRLDATE1,            // 0x0013002e, 上位机程序日期1
  d_machine1_MOLDSET_UPPER_CP_CTRLDATE2,            // 0x0013002f, 上位机程序日期2
  d_machine1_MOLDSET_FL_HYDY_WAY_SET,               // 0x00130030, 油路切换选择
  d_machine1_MOLDSET_FL_HYDY_WAY_SET_MODE,          // 0x00130031, 油路切换方式
  d_machine1_MOLDSET_FL_WET_DRY_CYCLES,             // 0x00130032, 干循环功能
  d_machine1_MOLDSET_FL_FREETIMEOUT,                // 0x00130033, 自动停留转手动时间
  d_machine1_MOLDSET_FL_OPNEJTCORE_FWD,             // 0x00130034, 开模联动中子退后中子进
  d_machine1_MOLDSET_FL_HYDR_LINK_OPNCHG,           // 0x00130035, 开模储料联动油路
  d_machine1_MOLDSET_FL_HYDR_LINK_OPNEJT,           // 0x00130036, 开模托模联动油路
  d_machine1_MOLDSET_FL_HYDR_LINK_OPNCORE,          // 0x00130037, 开模中子联动油路
  d_machine1_MOLDSET_FL_POWER_TYPE,                 // 0x00130038, 电表型号
  d_machine1_MOLDSET_TM_IOFORCE_OUT,                // 0x00130039, 输出点强制保护时间
  d_machine1_MOLDSET_FL_AIR_EXTEND28                // 0x0013003a, 吹气28组
};

enum db_machine1_VERSION{
  d_machine1_VERSION_test_tag_and_ver_sn_and_reserve_and_cpu_type = 0x00140000, // 0x00140000, 
  d_machine1_VERSION_model_and_cat,                                             // 0x00140001, 
  d_machine1_VERSION_drv_and_mec_and_arch_and_model,                            // 0x00140002, 
  d_machine1_VERSION_db_ver,                                                    // 0x00140003, 
  d_machine1_VERSION_boot_ver_1,                                                // 0x00140004, 
  d_machine1_VERSION_boot_ver_2,                                                // 0x00140005, 
  d_machine1_VERSION_os_ver_and_os_type,                                        // 0x00140006, 
  d_machine1_VERSION_sw_ver,                                                    // 0x00140007, 
  d_machine1_VERSION_date_1_and_date_2,                                         // 0x00140008, 
  d_machine1_VERSION_date_3_and_cus_sn,                                         // 0x00140009, 
  d_machine1_VERSION_pa1,                                                       // 0x0014000a, 
  d_machine1_VERSION_pa2,                                                       // 0x0014000b, 
  d_machine1_VERSION_Err_Rs232,                                                 // 0x0014000c, 
  d_machine1_VERSION_Err_Temp,                                                  // 0x0014000d, 
  d_machine1_VERSION_Err_Adinj,                                                 // 0x0014000e, 
  d_machine1_VERSION_Err_Adcmp,                                                 // 0x0014000f, 
  d_machine1_VERSION_Err_Adeje,                                                 // 0x00140010, 
  d_machine1_VERSION_power_type,                                                // 0x00140011, 动力类型
  d_machine1_VERSION_machine_type,                                              // 0x00140012, 机器类型
  d_machine1_VERSION_application_type                                           // 0x00140013, 应用类型
};

enum db_machine1_HYDR{
  d_machine1_HYDR_DAPRES_CLSHIAVR = 0x00150000  // 0x00150000, 锁模压力平均值
};

enum db_machine1_STATE{
  d_machine1_STATE_FL_ERROR0 = 0x00160000, // 0x00160000, 警报1
  d_machine1_STATE_FL_ERROR1,              // 0x00160001, 警报2
  d_machine1_STATE_ST_WOPERMODE,           // 0x00160002, 
  d_machine1_STATE_WTIME_AGE,              // 0x00160003, 老练时间
  d_machine1_STATE_FL_AUTOALARM,           // 0x00160004, 自动警报状态
  d_machine1_STATE_TM_CYCLE,               // 0x00160005, 循环计时
  d_machine1_STATE_TM_CYCLE_MAX,           // 0x00160006, 全程计时最大值
  d_machine1_STATE_TM_OPER_LOWORD,         // 0x00160007, 开机时间
  d_machine1_STATE_TM_OPER_HIGHWORD,       // 0x00160008, 开机时间
  d_machine1_STATE_TM_CYCLE_LAST,          // 0x00160009, 电眼自动再循环实时动作步
  d_machine1_STATE_ENERGY_LAST,            // 0x0016000a, 本次循环消耗的电能
  d_machine1_STATE_ENERGY_LASTALL,         // 0x0016000b, 开机后总消耗的电能
  d_machine1_STATE_POWER_CYCLE,            // 0x0016000c, 本次循环总能耗
  d_machine1_STATE_POWER_OTHERS,           // 0x0016000d, 本次循环其它能耗
  d_machine1_STATE_POWER_CLAMPCLS,         // 0x0016000e, 本次循环关模能耗
  d_machine1_STATE_POWER_CLAMPOPN,         // 0x0016000f, 本次循环开模能耗
  d_machine1_STATE_POWER_INJECT,           // 0x00160010, 本次循环射出能耗
  d_machine1_STATE_POWER_CHARGE,           // 0x00160011, 本次循环储料能耗
  d_machine1_STATE_POWER_SUCKBACK,         // 0x00160012, 本次循环射退能耗
  d_machine1_STATE_POWER_EJECT,            // 0x00160013, 本次循环托模能耗
  d_machine1_STATE_EXTIOST,                // 0x00160014, IO扩展板状态
  d_machine1_STATE_EXTDAST,                // 0x00160015, DA扩展板状态
  d_machine1_STATE_EXTTEMPST,              // 0x00160016, 温度扩展板状态
  d_machine1_STATE_EXTHSPDTEMPST,          // 0x00160017, 模温扩展板状态
  d_machine1_STATE_QC_OPN_CNT,             // 0x00160018, QC监控模数
  d_machine1_STATE_QC_AVG_CYCTM,           // 0x00160019, 周期时间平均值
  d_machine1_STATE_QC_DEVIATION_MAX_CYCTM, // 0x0016001a, 周期时间最大偏差值
  d_machine1_STATE_QC_UNQ_CNT_CYCTM,       // 0x0016001b, 周期时间不合格数
  d_machine1_STATE_QC_UNQ_RATE_CYCTM,      // 0x0016001c, 周期时间不合格率
  d_machine1_STATE_QC_MAX_CYCTM,           // 0x0016001d, 周期时间最大值
  d_machine1_STATE_QC_MIN_CYCTM,           // 0x0016001e, 周期时间最小值
  d_machine1_STATE_TM_CYCLE_WHOLE,         // 0x0016001f, 全程计时，包括再循环时间，开关安全门
  d_machine1_STATE_E301_SW_VER,            // 0x00160020, E301扩展板软件版本
  d_machine1_STATE_STEP_PRECYCLE,          // 0x00160021, 前置再循环实时动作步
  d_machine1_STATE_STEP_RECYCLESEMI,       // 0x00160022, 半自动再循环实时动作步
  d_machine1_STATE_STEP_RECYCJLETIME,      // 0x00160023, 全自动再循环实时动作步
  d_machine1_STATE_STEP_RECYCLESESOR,      // 0x00160024, 电眼自动再循环实时动作步
  d_machine1_STATE_ERR_RS232_CNT,          // 0x00160025, RS232警报次数
  d_machine1_STATE_ERR_TEMP_CNT,           // 0x00160026, 温度警报次数
  d_machine1_STATE_ERR_ADCMP_CNT,          // 0x00160027, 开关模电子尺跳变次数
  d_machine1_STATE_ERR_ADINJ_CNT,          // 0x00160028, 射出电子尺跳变次数
  d_machine1_STATE_ERR_ADEJE_CNT,          // 0x00160029, 托模电子尺跳变次数
  d_machine1_STATE_EXT_E505_ST,            // 0x0016002a, E505扩展板状态
  d_machine1_STATE_EXT_E502_ST,            // 0x0016002b, E502扩展板状态
  d_machine1_STATE_TM_CYCLE_H,             // 0x0016002c, 循环计时
  d_machine1_STATE_TM_CYCLE_LAST_H,        // 0x0016002d, 制品时间
  d_machine1_STATE_TM_CYCLE_WHOLE_H,       // 0x0016002e, 全程计时
  d_machine1_STATE_TM_CYCLE_MAX_H,         // 0x0016002f, 循环最大值
  d_machine1_STATE_Qc_Max_CycTm_H,         // 0x00160030, 周期时间最大值
  d_machine1_STATE_Qc_Min_CycTm_H,         // 0x00160031, 周期时间最小值
  d_machine1_STATE_Qc_Avg_CycTm_H,         // 0x00160032, 周期时间平均值
  d_machine1_STATE_Qc_Deviation_Max_CycTm_H, // 0x00160033, 周期时间最大偏差值
  d_machine1_STATE_EXT_E501_ST,            // 0x00160034, E501扩展板状态
  d_machine1_STATE_MAIN_CYCLE_TMOUT,       // 0x00160035, 主循环超时次数
  d_machine1_STATE_DA_E700B_VER,           // 0x00160036, E700B版本号
  d_machine1_STATE_WRITE_CNT_E700B,        // 0x00160037, 通讯测试写入次数
  d_machine1_STATE_READ_CNT_E700B,         // 0x00160038, 通讯测试读入次数
  d_machine1_STATE_IO_E40XB_VER1,          // 0x00160039, IO扩展板1版本号
  d_machine1_STATE_IO_E40XB_VER2,          // 0x0016003a, IO扩展板2版本号
  d_machine1_STATE_IO_E40XB_VER3,          // 0x0016003b, IO扩展板3版本号
  d_machine1_STATE_IO_E40XB_VER4,          // 0x0016003c, IO扩展板4版本号
  d_machine1_STATE_MODOPENNUM0,            // 0x0016003d, 开模数
  d_machine1_STATE_MODOPENNUM1,            // 0x0016003e, 开模数
  d_machine1_STATE_ENERGY_TOTAL_L,         // 0x0016003f, 总能耗
  d_machine1_STATE_ENERGY_TOTAL_H,         // 0x00160040, 总能耗
  d_machine1_STATE_ENERGY_AUTO_L,          // 0x00160041, 自动能耗
  d_machine1_STATE_ENERGY_AUTO_H,          // 0x00160042, 自动能耗
  d_machine1_STATE_POWER_L,                // 0x00160043, 总功率
  d_machine1_STATE_POWER_H,                // 0x00160044, 总功率
  d_machine1_STATE_VOL_A,                  // 0x00160045, A相电压
  d_machine1_STATE_VOL_B,                  // 0x00160046, B相电压
  d_machine1_STATE_VOL_C,                  // 0x00160047, C相电压
  d_machine1_STATE_VOL_AB,                 // 0x00160048, AB相电压
  d_machine1_STATE_VOL_BC,                 // 0x00160049, BC相电压
  d_machine1_STATE_VOL_AC,                 // 0x0016004a, AC相电压
  d_machine1_STATE_CURRENT_A,              // 0x0016004b, A相电流
  d_machine1_STATE_CURRENT_B,              // 0x0016004c, B相电流
  d_machine1_STATE_CURRENT_C,              // 0x0016004d, C相电流
  d_machine1_STATE_VOL_LN,                 // 0x0016004e, 相线-零线总电压
  d_machine1_STATE_VOL_LL,                 // 0x0016004f, 相线-相线总电压
  d_machine1_STATE_WRITE_CNT_POWER,        // 0x00160050, 
  d_machine1_STATE_READ_CNT_POWER,         // 0x00160051, 
  d_machine1_STATE_PUMP_MAIN,              // 0x00160052, 动力通道
  d_machine1_STATE_PROTFULL_CMD,           // 0x00160053, 串口数据已满警报-cmd
  d_machine1_STATE_PROTFULL_ID,            // 0x00160054, 串口数据已满警报-id
  d_machine1_STATE_MAIN_CYCLE_TWAVG,       // 0x00160055, (主循环)平均周期
  d_machine1_STATE_TM_MAIN_STARTUP         // 0x00160056, 主机本次开机时间
};

enum db_motor1_PO{
  d_motor1_PO_O_MTR_STP = 0x04100000, // 0x04100000, 马达停
  d_motor1_PO_O_MTR_STR,              // 0x04100001, 马达开
  d_motor1_PO_O_MTR_Y_D,              // 0x04100002, Y 转 DELTA
  d_motor1_PO_O_MTR_STP2,             // 0x04100003, 马达2停
  d_motor1_PO_O_MTR_STR2,             // 0x04100004, 马达2开
  d_motor1_PO_O_MTR_STP3,             // 0x04100005, 马达3停
  d_motor1_PO_O_MTR_STR3,             // 0x04100006, 马达3开
  d_motor1_PO_O_MTR_STP4,             // 0x04100007, 马达4停
  d_motor1_PO_O_MTR_STR4,             // 0x04100008, 马达4开
  d_motor1_PO_O_MOTO_SRV,             // 0x04100009, 马达保留
  d_motor1_PO_O_SERVO_EN              // 0x0410000a, 伺服使能
};

enum db_motor1_PI{
  d_motor1_PI_I_MTROVL = 0x04110000, // 0x04110000, 马达过载
  d_motor1_PI_I_MTRCMPT,             // 0x04110001, 马达启动完成
  d_motor1_PI_I_MTRCMPT2,            // 0x04110002, 马达2启动完成
  d_motor1_PI_I_MTRCMPT3,            // 0x04110003, 马达3启动完成
  d_motor1_PI_I_MTRCMPT4,            // 0x04110004, 马达4启动完成
  d_motor1_PI_I_MOTO_SRV,            // 0x04110005, 马达启动备用
  d_motor1_PI_I_MOTO_OK              // 0x04110006, 马达启动
};

enum db_motor1_MACHSET{
  d_motor1_MACHSET_MOTOR_TEMPCH = 0x04120000, // 0x04120000, 马达温度通道
  d_motor1_MACHSET_TM_MOTORYSTART,            // 0x04120001, 马达启动保持时间
  d_motor1_MACHSET_TM_MOTORDELTA,             // 0x04120002, Y->Δ 转换时间
  d_motor1_MACHSET_TM_HOLDMOTORON,            // 0x04120003, Δ 启动保持时间
  d_motor1_MACHSET_FL_MOTORCNT,               // 0x04120004, 马达数量
  d_motor1_MACHSET_TM_WARNMOTORON,            // 0x04120005, 多马达启动超时检测
  d_motor1_MACHSET_MOTORTEMP_ERRORMAX,        // 0x04120006, 电机温度上限
  d_motor1_MACHSET_PUMP_REVERSE_LIMITTM,      // 0x04120007, 电机反转保护时间
  d_motor1_MACHSET_FL_FREETIME_MOTOROFF,      // 0x04120008, 马达闲置关停
  d_motor1_MACHSET_TM_FREETIME_MOTOROFF       // 0x04120009, 马达闲置时间
};

enum db_motor1_STATE{
  d_motor1_STATE_FL_ERROR0 = 0x04160000, // 0x04160000, 警报1
  d_motor1_STATE_FL_ERROR1,              // 0x04160001, 警报2
  d_motor1_STATE_FL_MOTORSTATUS,         // 0x04160002, 马达状态
  d_motor1_STATE_DATEMP_REALMOTOR,       // 0x04160003, 电机温度
  d_motor1_STATE_TM_COUNTDOWNMOTOROFF    // 0x04160004, 闲置马达倒计时
};

enum db_tempctrl1_PO{
  d_tempctrl1_PO_O_MLD_COL = 0x05100000, // 0x05100000, 模具冷却
  d_tempctrl1_PO_O_HEATER,               // 0x05100001, 电热开
  d_tempctrl1_PO_O_OILCOOL,              // 0x05100002, 油温冷却
  d_tempctrl1_PO_O_COOLER,               // 0x05100003, 冷却
  d_tempctrl1_PO_O_COOL_1,               // 0x05100004, 冷却器 1
  d_tempctrl1_PO_O_COOL_2,               // 0x05100005, 冷却器 2
  d_tempctrl1_PO_O_COOL_3,               // 0x05100006, 冷却器 3
  d_tempctrl1_PO_O_COOL_4,               // 0x05100007, 冷却器 4
  d_tempctrl1_PO_O_COOL_5,               // 0x05100008, 冷却器 5
  d_tempctrl1_PO_O_COOL_6,               // 0x05100009, 冷却器 6
  d_tempctrl1_PO_O_COOL_7,               // 0x0510000a, 冷却器 7
  d_tempctrl1_PO_O_COOL_8,               // 0x0510000b, 冷却器 8
  d_tempctrl1_PO_O_COOL_9,               // 0x0510000c, 冷却器 9
  d_tempctrl1_PO_O_COOL_10,              // 0x0510000d, 冷却器 10
  d_tempctrl1_PO_O_COOL_11,              // 0x0510000e, 冷却器 11
  d_tempctrl1_PO_O_COOL_12,              // 0x0510000f, 冷却器 12
  d_tempctrl1_PO_O_COOL_13,              // 0x05100010, 冷却器 13
  d_tempctrl1_PO_O_COOL_14,              // 0x05100011, 冷却器 14
  d_tempctrl1_PO_O_COOL_15,              // 0x05100012, 冷却器 15
  d_tempctrl1_PO_O_TEMOP_1,              // 0x05100013, 电热一
  d_tempctrl1_PO_O_TEMOP_2,              // 0x05100014, 电热二
  d_tempctrl1_PO_O_TEMOP_3,              // 0x05100015, 电热三
  d_tempctrl1_PO_O_TEMOP_4,              // 0x05100016, 电热四
  d_tempctrl1_PO_O_TEMOP_5,              // 0x05100017, 电热五
  d_tempctrl1_PO_O_TEMOP_6,              // 0x05100018, 电热六
  d_tempctrl1_PO_O_TEMOP_7,              // 0x05100019, 电热七
  d_tempctrl1_PO_O_TEMOP_8,              // 0x0510001a, 电热八
  d_tempctrl1_PO_O_TEMOP_9,              // 0x0510001b, 电热九
  d_tempctrl1_PO_O_TEMOP_10,             // 0x0510001c, 电热十
  d_tempctrl1_PO_O_TEMOP_11,             // 0x0510001d, 电热十一
  d_tempctrl1_PO_O_TEMOP_12,             // 0x0510001e, 电热十二
  d_tempctrl1_PO_O_TEMOP_13,             // 0x0510001f, 电热十三
  d_tempctrl1_PO_O_TEMOP_14,             // 0x05100020, 电热十四
  d_tempctrl1_PO_O_TEMOP_15,             // 0x05100021, 电热十五
  d_tempctrl1_PO_O_TEMOPR1,              // 0x05100022, 射嘴加温
  d_tempctrl1_PO_O_DROP_COL              // 0x05100023, 落料口冷却水阀
};

enum db_tempctrl1_PI{
  d_tempctrl1_PI_I_TEM_IND = 0x05110000, // 0x05110000, 温度偏差
  d_tempctrl1_PI_I_OIL_TEM,              // 0x05110001, 油温偏差
  d_tempctrl1_PI_I_COLPSR,               // 0x05110002, 冷却水压力
  d_tempctrl1_PI_I_HEATTOUCH             // 0x05110003, 加热接触器
};

enum db_tempctrl1_MACHSET{
  d_tempctrl1_MACHSET_DATEMP_ERRORMIN = 0x05120000, // 0x05120000, 料管温度下限偏差
  d_tempctrl1_MACHSET_DATEMP_ERRORMAX,              // 0x05120001, 料管温度上限偏差
  d_tempctrl1_MACHSET_DATEMP_OILMIN,                // 0x05120002, 油箱油温下限
  d_tempctrl1_MACHSET_DATEMP_OILMAX,                // 0x05120003, 油箱油温上限
  d_tempctrl1_MACHSET_FL_TEMPWORDUPCHECK,           // 0x05120004, 电热连续加温不检查
  d_tempctrl1_MACHSET_DATEMP_COOLINGDIFFERENCE,     // 0x05120005, 冷却水温差
  d_tempctrl1_MACHSET_DATEMP_COOLERONFOROIL,        // 0x05120006, 油温冷却开启温度
  d_tempctrl1_MACHSET_DATEMP_COOLEROFFFOROIL,       // 0x05120007, 油温冷却关闭温度
  d_tempctrl1_MACHSET_DATEMP_PWRONHEATERON,         // 0x05120008, 自动加温
  d_tempctrl1_MACHSET_WTEMPFREEUP,                  // 0x05120009, 自由升温度数
  d_tempctrl1_MACHSET_DATEMP_TEMPCOOLERON,          // 0x0512000a, 料管冷却开启上偏差
  d_tempctrl1_MACHSET_DATEMP_TEMPCOOLEROFF,         // 0x0512000b, 料管冷却关闭下偏差
  d_tempctrl1_MACHSET_DATEMP_RAMP_1,                // 0x0512000c, 温控#1段缓冲区
  d_tempctrl1_MACHSET_DATEMP_RAMP_2,                // 0x0512000d, 温控#2段缓冲区
  d_tempctrl1_MACHSET_DATEMP_RAMP_3,                // 0x0512000e, 温控#3段缓冲区
  d_tempctrl1_MACHSET_DATEMP_RAMP_4,                // 0x0512000f, 温控#4段缓冲区
  d_tempctrl1_MACHSET_DATEMP_RAMP_5,                // 0x05120010, 温控#5段缓冲区
  d_tempctrl1_MACHSET_DATEMP_RAMP_6,                // 0x05120011, 温控#6段缓冲区
  d_tempctrl1_MACHSET_DATEMP_RAMP_7,                // 0x05120012, 温控#7段缓冲区
  d_tempctrl1_MACHSET_DATEMP_RAMP_8,                // 0x05120013, 温控#8段缓冲区
  d_tempctrl1_MACHSET_DATEMP_RAMP_9,                // 0x05120014, 温控#9段缓冲区
  d_tempctrl1_MACHSET_DATEMP_RAMP_10,               // 0x05120015, 温控#10段缓冲区
  d_tempctrl1_MACHSET_DATEMP_RAMP_11,               // 0x05120016, 温控#11段缓冲区
  d_tempctrl1_MACHSET_DATEMP_RAMP_12,               // 0x05120017, 温控#12段缓冲区
  d_tempctrl1_MACHSET_DATEMP_RAMP_13,               // 0x05120018, 温控#13段缓冲区
  d_tempctrl1_MACHSET_DATEMP_RAMP_14,               // 0x05120019, 温控#14段缓冲区
  d_tempctrl1_MACHSET_DATEMP_RAMP_15,               // 0x0512001a, 温控#15段缓冲区
  d_tempctrl1_MACHSET_DATEMP_RAMP_16,               // 0x0512001b, 温控#16段缓冲区
  d_tempctrl1_MACHSET_DAPID_T_1,                    // 0x0512001c, 温控1段系数
  d_tempctrl1_MACHSET_DAPID_T_2,                    // 0x0512001d, 温控2段系数
  d_tempctrl1_MACHSET_DAPID_T_3,                    // 0x0512001e, 温控3段系数
  d_tempctrl1_MACHSET_DAPID_T_4,                    // 0x0512001f, 温控4段系数
  d_tempctrl1_MACHSET_DAPID_T_5,                    // 0x05120020, 温控5段系数
  d_tempctrl1_MACHSET_DAPID_T_6,                    // 0x05120021, 温控6段系数
  d_tempctrl1_MACHSET_DAPID_T_7,                    // 0x05120022, 温控7段系数
  d_tempctrl1_MACHSET_DAPID_T_8,                    // 0x05120023, 温控8段系数
  d_tempctrl1_MACHSET_DAPID_T_9,                    // 0x05120024, 温控9段系数
  d_tempctrl1_MACHSET_DAPID_T_10,                   // 0x05120025, 温控10段系数
  d_tempctrl1_MACHSET_DAPID_T_11,                   // 0x05120026, 温控11段系数
  d_tempctrl1_MACHSET_DAPID_T_12,                   // 0x05120027, 温控12段系数
  d_tempctrl1_MACHSET_DAPID_T_13,                   // 0x05120028, 温控13段系数
  d_tempctrl1_MACHSET_DAPID_T_14,                   // 0x05120029, 温控14段系数
  d_tempctrl1_MACHSET_DAPID_T_15,                   // 0x0512002a, 温控15段系数
  d_tempctrl1_MACHSET_DAPID_T_16,                   // 0x0512002b, 温控16段系数
  d_tempctrl1_MACHSET_TEMP_MTIN_OPT,                // 0x0512002c, 落料口冷却
  d_tempctrl1_MACHSET_TEMP_MTIN_COOL,               // 0x0512002d, 落料口冷却开温度
  d_tempctrl1_MACHSET_TEMP_MTIN_BUFF,               // 0x0512002e, 落料口冷却关缓冲
  d_tempctrl1_MACHSET_DATEMP_OILHEAT,               // 0x0512002f, 预热温度
  d_tempctrl1_MACHSET_DAPID_KP_1,                   // 0x05120030, 温控#1段优化 KP
  d_tempctrl1_MACHSET_DAPID_KP_2,                   // 0x05120031, 温控#2段优化 KP
  d_tempctrl1_MACHSET_DAPID_KP_3,                   // 0x05120032, 温控#3段优化 KP
  d_tempctrl1_MACHSET_DAPID_KP_4,                   // 0x05120033, 温控#4段优化 KP
  d_tempctrl1_MACHSET_DAPID_KP_5,                   // 0x05120034, 温控#5段优化 KP
  d_tempctrl1_MACHSET_DAPID_KP_6,                   // 0x05120035, 温控#6段优化 KP
  d_tempctrl1_MACHSET_DAPID_KP_7,                   // 0x05120036, 温控#7段优化 KP
  d_tempctrl1_MACHSET_DAPID_KP_8,                   // 0x05120037, 温控#8段优化 KP
  d_tempctrl1_MACHSET_DAPID_KP_9,                   // 0x05120038, 温控#9段优化 KP
  d_tempctrl1_MACHSET_DAPID_KP_10,                  // 0x05120039, 温控#10段优化 KP
  d_tempctrl1_MACHSET_DAPID_KP_11,                  // 0x0512003a, 温控#11段优化 KP
  d_tempctrl1_MACHSET_DAPID_KP_12,                  // 0x0512003b, 温控#12段优化 KP
  d_tempctrl1_MACHSET_DAPID_KP_13,                  // 0x0512003c, 温控#13段优化 KP
  d_tempctrl1_MACHSET_DAPID_KP_14,                  // 0x0512003d, 温控#14段优化 KP
  d_tempctrl1_MACHSET_DAPID_KP_15,                  // 0x0512003e, 温控#15段优化 KP
  d_tempctrl1_MACHSET_DAPID_KP_16,                  // 0x0512003f, 温控#16段优化 KP
  d_tempctrl1_MACHSET_DAPID_TI_1,                   // 0x05120040, 温控#1段优化 Ti
  d_tempctrl1_MACHSET_DAPID_TI_2,                   // 0x05120041, 温控#2段优化 Ti
  d_tempctrl1_MACHSET_DAPID_TI_3,                   // 0x05120042, 温控#3段优化 Ti
  d_tempctrl1_MACHSET_DAPID_TI_4,                   // 0x05120043, 温控#4段优化 Ti
  d_tempctrl1_MACHSET_DAPID_TI_5,                   // 0x05120044, 温控#5段优化 Ti
  d_tempctrl1_MACHSET_DAPID_TI_6,                   // 0x05120045, 温控#6段优化 Ti
  d_tempctrl1_MACHSET_DAPID_TI_7,                   // 0x05120046, 温控#7段优化 Ti
  d_tempctrl1_MACHSET_DAPID_TI_8,                   // 0x05120047, 温控#8段优化 Ti
  d_tempctrl1_MACHSET_DAPID_TI_9,                   // 0x05120048, 温控#9段优化 Ti
  d_tempctrl1_MACHSET_DAPID_TI_10,                  // 0x05120049, 温控#10段优化 Ti
  d_tempctrl1_MACHSET_DAPID_TI_11,                  // 0x0512004a, 温控#11段优化 Ti
  d_tempctrl1_MACHSET_DAPID_TI_12,                  // 0x0512004b, 温控#12段优化 Ti
  d_tempctrl1_MACHSET_DAPID_TI_13,                  // 0x0512004c, 温控#13段优化 Ti
  d_tempctrl1_MACHSET_DAPID_TI_14,                  // 0x0512004d, 温控#14段优化 Ti
  d_tempctrl1_MACHSET_DAPID_TI_15,                  // 0x0512004e, 温控#15段优化 Ti
  d_tempctrl1_MACHSET_DAPID_TI_16,                  // 0x0512004f, 温控#16段优化 Ti
  d_tempctrl1_MACHSET_DAPID_TD_1,                   // 0x05120050, 温控#1段优化 Td
  d_tempctrl1_MACHSET_DAPID_TD_2,                   // 0x05120051, 温控#2段优化 Td
  d_tempctrl1_MACHSET_DAPID_TD_3,                   // 0x05120052, 温控#3段优化 Td
  d_tempctrl1_MACHSET_DAPID_TD_4,                   // 0x05120053, 温控#4段优化 Td
  d_tempctrl1_MACHSET_DAPID_TD_5,                   // 0x05120054, 温控#5段优化 Td
  d_tempctrl1_MACHSET_DAPID_TD_6,                   // 0x05120055, 温控#6段优化 Td
  d_tempctrl1_MACHSET_DAPID_TD_7,                   // 0x05120056, 温控#7段优化 Td
  d_tempctrl1_MACHSET_DAPID_TD_8,                   // 0x05120057, 温控#8段优化 Td
  d_tempctrl1_MACHSET_DAPID_TD_9,                   // 0x05120058, 温控#9段优化 Td
  d_tempctrl1_MACHSET_DAPID_TD_10,                  // 0x05120059, 温控#10段优化 Td
  d_tempctrl1_MACHSET_DAPID_TD_11,                  // 0x0512005a, 温控#11段优化 Td
  d_tempctrl1_MACHSET_DAPID_TD_12,                  // 0x0512005b, 温控#12段优化 Td
  d_tempctrl1_MACHSET_DAPID_TD_13,                  // 0x0512005c, 温控#13段优化 Td
  d_tempctrl1_MACHSET_DAPID_TD_14,                  // 0x0512005d, 温控#14段优化 Td
  d_tempctrl1_MACHSET_DAPID_TD_15,                  // 0x0512005e, 温控#15段优化 Td
  d_tempctrl1_MACHSET_DAPID_TD_16,                  // 0x0512005f, 温控#16段优化 Td
  d_tempctrl1_MACHSET_DAPID_TC_1,                   // 0x05120060, 温控#1段优化 Tc
  d_tempctrl1_MACHSET_DAPID_TC_2,                   // 0x05120061, 温控#2段优化 Tc
  d_tempctrl1_MACHSET_DAPID_TC_3,                   // 0x05120062, 温控#3段优化 Tc
  d_tempctrl1_MACHSET_DAPID_TC_4,                   // 0x05120063, 温控#4段优化 Tc
  d_tempctrl1_MACHSET_DAPID_TC_5,                   // 0x05120064, 温控#5段优化 Tc
  d_tempctrl1_MACHSET_DAPID_TC_6,                   // 0x05120065, 温控#6段优化 Tc
  d_tempctrl1_MACHSET_DAPID_TC_7,                   // 0x05120066, 温控#7段优化 Tc
  d_tempctrl1_MACHSET_DAPID_TC_8,                   // 0x05120067, 温控#8段优化 Tc
  d_tempctrl1_MACHSET_DAPID_TC_9,                   // 0x05120068, 温控#9段优化 Tc
  d_tempctrl1_MACHSET_DAPID_TC_10,                  // 0x05120069, 温控#10段优化 Tc
  d_tempctrl1_MACHSET_DAPID_TC_11,                  // 0x0512006a, 温控#11段优化 Tc
  d_tempctrl1_MACHSET_DAPID_TC_12,                  // 0x0512006b, 温控#12段优化 Tc
  d_tempctrl1_MACHSET_DAPID_TC_13,                  // 0x0512006c, 温控#13段优化 Tc
  d_tempctrl1_MACHSET_DAPID_TC_14,                  // 0x0512006d, 温控#14段优化 Tc
  d_tempctrl1_MACHSET_DAPID_TC_15,                  // 0x0512006e, 温控#15段优化 Tc
  d_tempctrl1_MACHSET_DAPID_TC_16,                  // 0x0512006f, 温控#16段优化 Tc
  d_tempctrl1_MACHSET_TEMP_PROTECT,                 // 0x05120070, 温度保护上限
  d_tempctrl1_MACHSET_TEMPCOOLCTRL,                 // 0x05120071, 落料口冷却功能
  d_tempctrl1_MACHSET_TEMPCOOLMAX                   // 0x05120072, 落料口温度上限
};

enum db_tempctrl1_MOLDSET{
  d_tempctrl1_MOLDSET_DATEMP_SETTING_1 = 0x05130000, // 0x05130000, 温度设定值1
  d_tempctrl1_MOLDSET_DATEMP_SETTING_2,              // 0x05130001, 温度设定值2
  d_tempctrl1_MOLDSET_DATEMP_SETTING_3,              // 0x05130002, 温度设定值3
  d_tempctrl1_MOLDSET_DATEMP_SETTING_4,              // 0x05130003, 温度设定值4
  d_tempctrl1_MOLDSET_DATEMP_SETTING_5,              // 0x05130004, 温度设定值5
  d_tempctrl1_MOLDSET_DATEMP_SETTING_6,              // 0x05130005, 温度设定值6
  d_tempctrl1_MOLDSET_DATEMP_SETTING_7,              // 0x05130006, 温度设定值7
  d_tempctrl1_MOLDSET_DATEMP_SETTING_8,              // 0x05130007, 温度设定值8
  d_tempctrl1_MOLDSET_DATEMP_SETTING_9,              // 0x05130008, 温度设定值9
  d_tempctrl1_MOLDSET_DATEMP_SETTING_10,             // 0x05130009, 温度设定值10
  d_tempctrl1_MOLDSET_DATEMP_SETTING_11,             // 0x0513000a, 温度设定值11
  d_tempctrl1_MOLDSET_DATEMP_SETTING_12,             // 0x0513000b, 温度设定值12
  d_tempctrl1_MOLDSET_DATEMP_SETTING_13,             // 0x0513000c, 温度设定值13
  d_tempctrl1_MOLDSET_DATEMP_SETTING_14,             // 0x0513000d, 温度设定值14
  d_tempctrl1_MOLDSET_DATEMP_SETTING_15,             // 0x0513000e, 温度设定值15
  d_tempctrl1_MOLDSET_DATEMP_SETTING_16,             // 0x0513000f, 温度设定值16
  d_tempctrl1_MOLDSET_DATEMP_MAX_1,                  // 0x05130010, 温度上限1
  d_tempctrl1_MOLDSET_DATEMP_MAX_2,                  // 0x05130011, 温度上限2
  d_tempctrl1_MOLDSET_DATEMP_MAX_3,                  // 0x05130012, 温度上限3
  d_tempctrl1_MOLDSET_DATEMP_MAX_4,                  // 0x05130013, 温度上限4
  d_tempctrl1_MOLDSET_DATEMP_MAX_5,                  // 0x05130014, 温度上限5
  d_tempctrl1_MOLDSET_DATEMP_MAX_6,                  // 0x05130015, 温度上限5
  d_tempctrl1_MOLDSET_DATEMP_MAX_7,                  // 0x05130016, 温度上限7
  d_tempctrl1_MOLDSET_DATEMP_MAX_8,                  // 0x05130017, 温度上限8
  d_tempctrl1_MOLDSET_DATEMP_MAX_9,                  // 0x05130018, 温度上限9
  d_tempctrl1_MOLDSET_DATEMP_MAX_10,                 // 0x05130019, 温度上限10
  d_tempctrl1_MOLDSET_DATEMP_MAX_11,                 // 0x0513001a, 温度上限11
  d_tempctrl1_MOLDSET_DATEMP_MAX_12,                 // 0x0513001b, 温度上限12
  d_tempctrl1_MOLDSET_DATEMP_MAX_13,                 // 0x0513001c, 温度上限13
  d_tempctrl1_MOLDSET_DATEMP_MAX_14,                 // 0x0513001d, 温度上限14
  d_tempctrl1_MOLDSET_DATEMP_MAX_15,                 // 0x0513001e, 温度上限15
  d_tempctrl1_MOLDSET_DATEMP_MAX_16,                 // 0x0513001f, 温度上限16
  d_tempctrl1_MOLDSET_DATEMP_MIN_1,                  // 0x05130020, 温度下限1
  d_tempctrl1_MOLDSET_DATEMP_MIN_2,                  // 0x05130021, 温度下限2
  d_tempctrl1_MOLDSET_DATEMP_MIN_3,                  // 0x05130022, 温度下限3
  d_tempctrl1_MOLDSET_DATEMP_MIN_4,                  // 0x05130023, 温度下限4
  d_tempctrl1_MOLDSET_DATEMP_MIN_5,                  // 0x05130024, 温度下限5
  d_tempctrl1_MOLDSET_DATEMP_MIN_6,                  // 0x05130025, 温度下限6
  d_tempctrl1_MOLDSET_DATEMP_MIN_7,                  // 0x05130026, 温度下限7
  d_tempctrl1_MOLDSET_DATEMP_MIN_8,                  // 0x05130027, 温度下限8
  d_tempctrl1_MOLDSET_DATEMP_MIN_9,                  // 0x05130028, 温度下限9
  d_tempctrl1_MOLDSET_DATEMP_MIN_10,                 // 0x05130029, 温度下限10
  d_tempctrl1_MOLDSET_DATEMP_MIN_11,                 // 0x0513002a, 温度下限11
  d_tempctrl1_MOLDSET_DATEMP_MIN_12,                 // 0x0513002b, 温度下限12
  d_tempctrl1_MOLDSET_DATEMP_MIN_13,                 // 0x0513002c, 温度下限13
  d_tempctrl1_MOLDSET_DATEMP_MIN_14,                 // 0x0513002d, 温度下限14
  d_tempctrl1_MOLDSET_DATEMP_MIN_15,                 // 0x0513002e, 温度下限15
  d_tempctrl1_MOLDSET_DATEMP_MIN_16,                 // 0x0513002f, 温度下限16
  d_tempctrl1_MOLDSET_TM_HEATERWARM,                 // 0x05130030, 预温计时
  d_tempctrl1_MOLDSET_FL_HEATEROFFIFALARM,           // 0x05130031, 警报时间到关电热
  d_tempctrl1_MOLDSET_FL_TEMPHOLD,                   // 0x05130032, 保温功能选择
  d_tempctrl1_MOLDSET_DATEMP_HOLD,                   // 0x05130033, 保温温度
  d_tempctrl1_MOLDSET_TM_RESPONSELESSCYCLE,          // 0x05130034, 手动温控周期
  d_tempctrl1_MOLDSET_DACENT_HEATERON,               // 0x05130035, 手动加热百分比
  d_tempctrl1_MOLDSET_FL_TEMPCOOL,                   // 0x05130036, 料管冷却器使用
  d_tempctrl1_MOLDSET_MAX_INFLECT_CNT,               // 0x05130037, 拐点数
  d_tempctrl1_MOLDSET_FL_DATEMPINJM,                 // 0x05130038, 射嘴比例温控功能
  d_tempctrl1_MOLDSET_FL_TEMPLIVE,                   // 0x05130039, 温度优化
  d_tempctrl1_MOLDSET_TEMP_PORT,                     // 0x0513003a, 温度特殊通道
  d_tempctrl1_MOLDSET_TEMP_PORT_BL,                  // 0x0513003b, 温度比例
  d_tempctrl1_MOLDSET_FL_TOGETHER,                   // 0x0513003c, 同步升温
  d_tempctrl1_MOLDSET_FL_TEMPYIZI                    // 0x0513003d, 超温抑制
};

enum db_tempctrl1_ZDSET{
  d_tempctrl1_ZDSET_WTEMP_CHZERO_1 = 0x05140000, // 0x05140000, 
  d_tempctrl1_ZDSET_WTEMP_CHZERO_2,              // 0x05140001, 
  d_tempctrl1_ZDSET_WTEMP_CHZERO_3,              // 0x05140002, 
  d_tempctrl1_ZDSET_WTEMP_CHZERO_4,              // 0x05140003, 
  d_tempctrl1_ZDSET_WTEMP_CHZERO_5,              // 0x05140004, 
  d_tempctrl1_ZDSET_WTEMP_CHZERO_6,              // 0x05140005, 
  d_tempctrl1_ZDSET_WTEMP_CHZERO_7,              // 0x05140006, 
  d_tempctrl1_ZDSET_WTEMP_CHZERO_8,              // 0x05140007, 
  d_tempctrl1_ZDSET_WTEMP_CHZERO_9,              // 0x05140008, 
  d_tempctrl1_ZDSET_WTEMP_CHZERO_10,             // 0x05140009, 
  d_tempctrl1_ZDSET_WTEMP_CHZERO_11,             // 0x0514000a, 
  d_tempctrl1_ZDSET_WTEMP_CHZERO_12,             // 0x0514000b, 
  d_tempctrl1_ZDSET_WTEMP_CHZERO_13,             // 0x0514000c, 
  d_tempctrl1_ZDSET_WTEMP_CHZERO_14,             // 0x0514000d, 
  d_tempctrl1_ZDSET_WTEMP_CHZERO_15,             // 0x0514000e, 
  d_tempctrl1_ZDSET_WTEMP_CHZERO_16,             // 0x0514000f, 
  d_tempctrl1_ZDSET_WTEMP_CHB_1,                 // 0x05140010, 
  d_tempctrl1_ZDSET_WTEMP_CHB_2,                 // 0x05140011, 
  d_tempctrl1_ZDSET_WTEMP_CHB_3,                 // 0x05140012, 
  d_tempctrl1_ZDSET_WTEMP_CHB_4,                 // 0x05140013, 
  d_tempctrl1_ZDSET_WTEMP_CHB_5,                 // 0x05140014, 
  d_tempctrl1_ZDSET_WTEMP_CHB_6,                 // 0x05140015, 
  d_tempctrl1_ZDSET_WTEMP_CHB_7,                 // 0x05140016, 
  d_tempctrl1_ZDSET_WTEMP_CHB_8,                 // 0x05140017, 
  d_tempctrl1_ZDSET_WTEMP_CHB_9,                 // 0x05140018, 
  d_tempctrl1_ZDSET_WTEMP_CHB_10,                // 0x05140019, 
  d_tempctrl1_ZDSET_WTEMP_CHB_11,                // 0x0514001a, 
  d_tempctrl1_ZDSET_WTEMP_CHB_12,                // 0x0514001b, 
  d_tempctrl1_ZDSET_WTEMP_CHB_13,                // 0x0514001c, 
  d_tempctrl1_ZDSET_WTEMP_CHB_14,                // 0x0514001d, 
  d_tempctrl1_ZDSET_WTEMP_CHB_15,                // 0x0514001e, 
  d_tempctrl1_ZDSET_WTEMP_CHB_16,                // 0x0514001f, 
  d_tempctrl1_ZDSET_WTEMP_OFFSET,                // 0x05140020, 室温校准
  d_tempctrl1_ZDSET_HEAT_RATE_1,                 // 0x05140021, 
  d_tempctrl1_ZDSET_HEAT_RATE_2,                 // 0x05140022, 
  d_tempctrl1_ZDSET_HEAT_RATE_3,                 // 0x05140023, 
  d_tempctrl1_ZDSET_HEAT_RATE_4,                 // 0x05140024, 
  d_tempctrl1_ZDSET_HEAT_RATE_5,                 // 0x05140025, 
  d_tempctrl1_ZDSET_HEAT_RATE_6,                 // 0x05140026, 
  d_tempctrl1_ZDSET_HEAT_RATE_7,                 // 0x05140027, 
  d_tempctrl1_ZDSET_HEAT_RATE_8,                 // 0x05140028, 
  d_tempctrl1_ZDSET_HEAT_RATE_9,                 // 0x05140029, 
  d_tempctrl1_ZDSET_HEAT_RATE_10,                // 0x0514002a, 
  d_tempctrl1_ZDSET_HEAT_RATE_11,                // 0x0514002b, 
  d_tempctrl1_ZDSET_HEAT_RATE_12,                // 0x0514002c, 
  d_tempctrl1_ZDSET_HEAT_RATE_13,                // 0x0514002d, 
  d_tempctrl1_ZDSET_HEAT_RATE_14,                // 0x0514002e, 
  d_tempctrl1_ZDSET_HEAT_RATE_15,                // 0x0514002f, 
  d_tempctrl1_ZDSET_HEAT_RATE_16                 // 0x05140030, 
};

enum db_tempctrl1_STATE{
  d_tempctrl1_STATE_FL_ERROR0 = 0x05160000, // 0x05160000, 
  d_tempctrl1_STATE_FL_ERROR1,              // 0x05160001, 
  d_tempctrl1_STATE_DATEMP_REAL_1,          // 0x05160002, 温度实际值0
  d_tempctrl1_STATE_DATEMP_REAL_2,          // 0x05160003, 温度实际值1
  d_tempctrl1_STATE_DATEMP_REAL_3,          // 0x05160004, 温度实际值2
  d_tempctrl1_STATE_DATEMP_REAL_4,          // 0x05160005, 温度实际值3
  d_tempctrl1_STATE_DATEMP_REAL_5,          // 0x05160006, 温度实际值4
  d_tempctrl1_STATE_DATEMP_REAL_6,          // 0x05160007, 温度实际值5
  d_tempctrl1_STATE_DATEMP_REAL_7,          // 0x05160008, 温度实际值6
  d_tempctrl1_STATE_DATEMP_REAL_8,          // 0x05160009, 温度实际值7
  d_tempctrl1_STATE_DATEMP_REAL_9,          // 0x0516000a, 温度实际值8
  d_tempctrl1_STATE_DATEMP_REAL_10,         // 0x0516000b, 温度实际值9
  d_tempctrl1_STATE_DATEMP_REAL_11,         // 0x0516000c, 温度实际值10
  d_tempctrl1_STATE_DATEMP_REAL_12,         // 0x0516000d, 温度实际值11
  d_tempctrl1_STATE_DATEMP_REAL_13,         // 0x0516000e, 温度实际值12
  d_tempctrl1_STATE_DATEMP_REAL_14,         // 0x0516000f, 温度实际值13
  d_tempctrl1_STATE_DATEMP_REAL_15,         // 0x05160010, 温度实际值14
  d_tempctrl1_STATE_DATEMP_REAL_16,         // 0x05160011, 温度实际值15
  d_tempctrl1_STATE_FL_TEMPSTATUS_1,        // 0x05160012, 温度加温状态1
  d_tempctrl1_STATE_FL_TEMPSTATUS_2,        // 0x05160013, 温度加温状态2
  d_tempctrl1_STATE_FL_TEMPSTATUS_3,        // 0x05160014, 温度加温状态3
  d_tempctrl1_STATE_FL_TEMPSTATUS_4,        // 0x05160015, 温度加温状态4
  d_tempctrl1_STATE_FL_TEMPSTATUS_5,        // 0x05160016, 温度加温状态5
  d_tempctrl1_STATE_FL_TEMPSTATUS_6,        // 0x05160017, 温度加温状态6
  d_tempctrl1_STATE_FL_TEMPSTATUS_7,        // 0x05160018, 温度加温状态7
  d_tempctrl1_STATE_FL_TEMPSTATUS_8,        // 0x05160019, 温度加温状态8
  d_tempctrl1_STATE_FL_TEMPSTATUS_9,        // 0x0516001a, 温度加温状态9
  d_tempctrl1_STATE_FL_TEMPSTATUS_10,       // 0x0516001b, 温度加温状态10
  d_tempctrl1_STATE_FL_TEMPSTATUS_11,       // 0x0516001c, 温度实际值11
  d_tempctrl1_STATE_FL_TEMPSTATUS_12,       // 0x0516001d, 温度实际值12
  d_tempctrl1_STATE_FL_TEMPSTATUS_13,       // 0x0516001e, 温度实际值13
  d_tempctrl1_STATE_FL_TEMPSTATUS_14,       // 0x0516001f, 温度实际值14
  d_tempctrl1_STATE_FL_TEMPSTATUS_15,       // 0x05160020, 温度实际值14
  d_tempctrl1_STATE_FL_TEMPSTATUS_16,       // 0x05160021, 温度加温状态
  d_tempctrl1_STATE_WTEMP_LM73,             // 0x05160022, 室温LM73的温度
  d_tempctrl1_STATE_WTEMP_RELAT,            // 0x05160023, 校准后的实时温度
  d_tempctrl1_STATE_WTEMP_ADJ,              // 0x05160024, 温度系数 
  d_tempctrl1_STATE_WTEMP_LM73UV,           // 0x05160025, 室温对应uV值
  d_tempctrl1_STATE_WTEMP_ADVALUE,          // 0x05160026, AD采样得到的实时值
  d_tempctrl1_STATE_WTEMP_REAL,             // 0x05160027, 实际温度(不叠加室温值)
  d_tempctrl1_STATE_WTEMP_CHANNEL,          // 0x05160028, 温度通道
  d_tempctrl1_STATE_WTEMP_ADJALL,           // 0x05160029, 测试画面标志  
  d_tempctrl1_STATE_WTEMP_ADJAMB,           // 0x0516002a, 室温校对  
  d_tempctrl1_STATE_FL_HEATERSTATUS,        // 0x0516002b, ST_电热
  d_tempctrl1_STATE_WSTATERSV6,             // 0x0516002c, 保温倒计时
  d_tempctrl1_STATE_DATEMP_REALMOTOR,       // 0x0516002d, 电机温度
  d_tempctrl1_STATE_MOVE_STEP_1,            // 0x0516002e, 
  d_tempctrl1_STATE_MOVE_STEP_2,            // 0x0516002f, 
  d_tempctrl1_STATE_MOVE_STEP_3,            // 0x05160030, 
  d_tempctrl1_STATE_MOVE_STEP_4,            // 0x05160031, 
  d_tempctrl1_STATE_MOVE_STEP_5,            // 0x05160032, 
  d_tempctrl1_STATE_MOVE_STEP_6,            // 0x05160033, 
  d_tempctrl1_STATE_MOVE_STEP_7,            // 0x05160034, 
  d_tempctrl1_STATE_MOVE_STEP_8,            // 0x05160035, 
  d_tempctrl1_STATE_MOVE_STEP_9,            // 0x05160036, 
  d_tempctrl1_STATE_MOVE_STEP_10,           // 0x05160037, 
  d_tempctrl1_STATE_MOVE_STEP_11,           // 0x05160038, 
  d_tempctrl1_STATE_MOVE_STEP_12,           // 0x05160039, 
  d_tempctrl1_STATE_MOVE_STEP_13,           // 0x0516003a, 
  d_tempctrl1_STATE_MOVE_STEP_14,           // 0x0516003b, 
  d_tempctrl1_STATE_MOVE_STEP_15,           // 0x0516003c, 
  d_tempctrl1_STATE_MOVE_STEP_16,           // 0x0516003d, 
  d_tempctrl1_STATE_ADJ_EXT_TEMP,           // 0x0516003e, 
  d_tempctrl1_STATE_DATEMP_CNVALID_1,       // 0x0516003f, 
  d_tempctrl1_STATE_DATEMP_CNVALID_2,       // 0x05160040, 
  d_tempctrl1_STATE_DATEMP_CNVALID_3,       // 0x05160041, 
  d_tempctrl1_STATE_DATEMP_CNVALID_4,       // 0x05160042, 
  d_tempctrl1_STATE_DATEMP_CNVALID_5,       // 0x05160043, 
  d_tempctrl1_STATE_DATEMP_CNVALID_6,       // 0x05160044, 
  d_tempctrl1_STATE_DATEMP_CNVALID_7,       // 0x05160045, 
  d_tempctrl1_STATE_DATEMP_CNVALID_8,       // 0x05160046, 
  d_tempctrl1_STATE_DATEMP_CNVALID_9,       // 0x05160047, 
  d_tempctrl1_STATE_DATEMP_CNVALID_10,      // 0x05160048, 
  d_tempctrl1_STATE_DATEMP_CNVALID_11,      // 0x05160049, 
  d_tempctrl1_STATE_DATEMP_CNVALID_12,      // 0x0516004a, 
  d_tempctrl1_STATE_DATEMP_CNVALID_13,      // 0x0516004b, 
  d_tempctrl1_STATE_DATEMP_CNVALID_14,      // 0x0516004c, 
  d_tempctrl1_STATE_DATEMP_CNVALID_15,      // 0x0516004d, 
  d_tempctrl1_STATE_DATEMP_CNVALID_16,      // 0x0516004e, 
  d_tempctrl1_STATE_DATEMP_CNINVALID_1,     // 0x0516004f, 
  d_tempctrl1_STATE_DATEMP_CNINVALID_2,     // 0x05160050, 
  d_tempctrl1_STATE_DATEMP_CNINVALID_3,     // 0x05160051, 
  d_tempctrl1_STATE_DATEMP_CNINVALID_4,     // 0x05160052, 
  d_tempctrl1_STATE_DATEMP_CNINVALID_5,     // 0x05160053, 
  d_tempctrl1_STATE_DATEMP_CNINVALID_6,     // 0x05160054, 
  d_tempctrl1_STATE_DATEMP_CNINVALID_7,     // 0x05160055, 
  d_tempctrl1_STATE_DATEMP_CNINVALID_8,     // 0x05160056, 
  d_tempctrl1_STATE_DATEMP_CNINVALID_9,     // 0x05160057, 
  d_tempctrl1_STATE_DATEMP_CNINVALID_10,    // 0x05160058, 
  d_tempctrl1_STATE_DATEMP_CNINVALID_11,    // 0x05160059, 
  d_tempctrl1_STATE_DATEMP_CNINVALID_12,    // 0x0516005a, 
  d_tempctrl1_STATE_DATEMP_CNINVALID_13,    // 0x0516005b, 
  d_tempctrl1_STATE_DATEMP_CNINVALID_14,    // 0x0516005c, 
  d_tempctrl1_STATE_DATEMP_CNINVALID_15,    // 0x0516005d, 
  d_tempctrl1_STATE_DATEMP_CNINVALID_16,    // 0x0516005e, 
  d_tempctrl1_STATE_DATEMP_CNINREAD_1,      // 0x0516005f, 
  d_tempctrl1_STATE_DATEMP_CNINREAD_2,      // 0x05160060, 
  d_tempctrl1_STATE_DATEMP_CNINREAD_3,      // 0x05160061, 
  d_tempctrl1_STATE_DATEMP_CNINREAD_4,      // 0x05160062, 
  d_tempctrl1_STATE_DATEMP_CNINREAD_5,      // 0x05160063, 
  d_tempctrl1_STATE_DATEMP_CNINREAD_6,      // 0x05160064, 
  d_tempctrl1_STATE_DATEMP_CNINREAD_7,      // 0x05160065, 
  d_tempctrl1_STATE_DATEMP_CNINREAD_8,      // 0x05160066, 
  d_tempctrl1_STATE_DATEMP_CNINREAD_9,      // 0x05160067, 
  d_tempctrl1_STATE_DATEMP_CNINREAD_10,     // 0x05160068, 
  d_tempctrl1_STATE_DATEMP_CNINREAD_11,     // 0x05160069, 
  d_tempctrl1_STATE_DATEMP_CNINREAD_12,     // 0x0516006a, 
  d_tempctrl1_STATE_DATEMP_CNINREAD_13,     // 0x0516006b, 
  d_tempctrl1_STATE_DATEMP_CNINREAD_14,     // 0x0516006c, 
  d_tempctrl1_STATE_DATEMP_CNINREAD_15,     // 0x0516006d, 
  d_tempctrl1_STATE_DATEMP_CNINREAD_16,     // 0x0516006e, 
  d_tempctrl1_STATE_DATEMP_VALUE_1,         // 0x0516006f, 
  d_tempctrl1_STATE_DATEMP_VALUE_2,         // 0x05160070, 
  d_tempctrl1_STATE_DATEMP_VALUE_3,         // 0x05160071, 
  d_tempctrl1_STATE_DATEMP_VALUE_4,         // 0x05160072, 
  d_tempctrl1_STATE_DATEMP_VALUE_5,         // 0x05160073, 
  d_tempctrl1_STATE_DATEMP_VALUE_6,         // 0x05160074, 
  d_tempctrl1_STATE_DATEMP_VALUE_7,         // 0x05160075, 
  d_tempctrl1_STATE_DATEMP_VALUE_8,         // 0x05160076, 
  d_tempctrl1_STATE_DATEMP_VALUE_9,         // 0x05160077, 
  d_tempctrl1_STATE_DATEMP_VALUE_10,        // 0x05160078, 
  d_tempctrl1_STATE_DATEMP_VALUE_11,        // 0x05160079, 
  d_tempctrl1_STATE_DATEMP_VALUE_12,        // 0x0516007a, 
  d_tempctrl1_STATE_DATEMP_VALUE_13,        // 0x0516007b, 
  d_tempctrl1_STATE_DATEMP_VALUE_14,        // 0x0516007c, 
  d_tempctrl1_STATE_DATEMP_VALUE_15,        // 0x0516007d, 
  d_tempctrl1_STATE_DATEMP_VALUE_16,        // 0x0516007e, 
  d_tempctrl1_STATE_DROP_COL_STATE,         // 0x0516007f, 落料口冷却是否开启
  d_tempctrl1_STATE_DATEMP_CHARGEPOS,       // 0x05160080, 落料口温度
  d_tempctrl1_STATE_DATEMP_WORKTM1,         // 0x05160081, 60s内电热输出时间1
  d_tempctrl1_STATE_DATEMP_WORKTM2,         // 0x05160082, 60s内电热输出时间2
  d_tempctrl1_STATE_DATEMP_WORKTM3,         // 0x05160083, 60s内电热输出时间3
  d_tempctrl1_STATE_DATEMP_WORKTM4,         // 0x05160084, 60s内电热输出时间4
  d_tempctrl1_STATE_DATEMP_WORKTM5,         // 0x05160085, 60s内电热输出时间5
  d_tempctrl1_STATE_DATEMP_WORKTM6,         // 0x05160086, 60s内电热输出时间6
  d_tempctrl1_STATE_DATEMP_WORKTM7,         // 0x05160087, 60s内电热输出时间7
  d_tempctrl1_STATE_DATEMP_WORKTM8,         // 0x05160088, 60s内电热输出时间8
  d_tempctrl1_STATE_DATEMP_WORKTM9,         // 0x05160089, 60s内电热输出时间9
  d_tempctrl1_STATE_DATEMP_WORKTM10,        // 0x0516008a, 60s内电热输出时间10
  d_tempctrl1_STATE_DATEMP_WORKTM11,        // 0x0516008b, 60s内电热输出时间11
  d_tempctrl1_STATE_DATEMP_WORKTM12,        // 0x0516008c, 60s内电热输出时间12
  d_tempctrl1_STATE_DATEMP_WORKTM13,        // 0x0516008d, 60s内电热输出时间13
  d_tempctrl1_STATE_DATEMP_WORKTM14,        // 0x0516008e, 60s内电热输出时间14
  d_tempctrl1_STATE_DATEMP_WORKTM15,        // 0x0516008f, 60s内电热输出时间15
  d_tempctrl1_STATE_DATEMP_WORKTM16,        // 0x05160090, 60s内电热输出时间16
  d_tempctrl1_STATE_DATEMP_E505_VER1,       // 0x05160091, 室温扩展板版本号1
  d_tempctrl1_STATE_DATEMP_E505_VER2,       // 0x05160092, 室温扩展板版本号2
  d_tempctrl1_STATE_DATEMP_E502_VER1,       // 0x05160093, 温度扩展板E502版本号1
  d_tempctrl1_STATE_DATEMP_E502_VER2,       // 0x05160094, 温度扩展板E502版本号
  d_tempctrl1_STATE_DATEMP_YOUHUAXUQIU,     // 0x05160095, 温度优化提示开马达的闪烁指示
  d_tempctrl1_STATE_DATEMP_YOUHUAOK         // 0x05160096, 温度优化开启与完成标记
};

enum db_tempctrl2_PO{
  d_tempctrl2_PO_O_MLD_COL = 0x05200000, // 0x05200000, 模具冷却
  d_tempctrl2_PO_O_HEATER,               // 0x05200001, 电热开
  d_tempctrl2_PO_O_OILCOOL,              // 0x05200002, 油温冷却
  d_tempctrl2_PO_O_COOLER,               // 0x05200003, 冷却
  d_tempctrl2_PO_O_COOL_1,               // 0x05200004, 冷却器 1
  d_tempctrl2_PO_O_COOL_2,               // 0x05200005, 冷却器 2
  d_tempctrl2_PO_O_COOL_3,               // 0x05200006, 冷却器 3
  d_tempctrl2_PO_O_COOL_4,               // 0x05200007, 冷却器 4
  d_tempctrl2_PO_O_COOL_5,               // 0x05200008, 冷却器 5
  d_tempctrl2_PO_O_COOL_6,               // 0x05200009, 冷却器 6
  d_tempctrl2_PO_O_COOL_7,               // 0x0520000a, 冷却器 7
  d_tempctrl2_PO_O_COOL_8,               // 0x0520000b, 冷却器 8
  d_tempctrl2_PO_O_COOL_9,               // 0x0520000c, 冷却器 9
  d_tempctrl2_PO_O_COOL_10,              // 0x0520000d, 冷却器 10
  d_tempctrl2_PO_O_COOL_11,              // 0x0520000e, 冷却器 11
  d_tempctrl2_PO_O_COOL_12,              // 0x0520000f, 冷却器 12
  d_tempctrl2_PO_O_COOL_13,              // 0x05200010, 冷却器 13
  d_tempctrl2_PO_O_COOL_14,              // 0x05200011, 冷却器 14
  d_tempctrl2_PO_O_COOL_15,              // 0x05200012, 冷却器 15
  d_tempctrl2_PO_O_TEMOP_1,              // 0x05200013, 电热一
  d_tempctrl2_PO_O_TEMOP_2,              // 0x05200014, 电热二
  d_tempctrl2_PO_O_TEMOP_3,              // 0x05200015, 电热三
  d_tempctrl2_PO_O_TEMOP_4,              // 0x05200016, 电热四
  d_tempctrl2_PO_O_TEMOP_5,              // 0x05200017, 电热五
  d_tempctrl2_PO_O_TEMOP_6,              // 0x05200018, 电热六
  d_tempctrl2_PO_O_TEMOP_7,              // 0x05200019, 电热七
  d_tempctrl2_PO_O_TEMOP_8,              // 0x0520001a, 电热八
  d_tempctrl2_PO_O_TEMOP_9,              // 0x0520001b, 电热九
  d_tempctrl2_PO_O_TEMOP_10,             // 0x0520001c, 电热十
  d_tempctrl2_PO_O_TEMOP_11,             // 0x0520001d, 电热十一
  d_tempctrl2_PO_O_TEMOP_12,             // 0x0520001e, 电热十二
  d_tempctrl2_PO_O_TEMOP_13,             // 0x0520001f, 电热十三
  d_tempctrl2_PO_O_TEMOP_14,             // 0x05200020, 电热十四
  d_tempctrl2_PO_O_TEMOP_15,             // 0x05200021, 电热十五
  d_tempctrl2_PO_O_TEMOPR1,              // 0x05200022, 射嘴加温
  d_tempctrl2_PO_O_DROP_COL              // 0x05200023, 落料口冷却水阀
};

enum db_tempctrl2_PI{
  d_tempctrl2_PI_I_TEM_IND = 0x05210000, // 0x05210000, 温度偏差
  d_tempctrl2_PI_I_OIL_TEM,              // 0x05210001, 油温偏差
  d_tempctrl2_PI_I_COLPSR,               // 0x05210002, 冷却水压力
  d_tempctrl2_PI_I_HEATTOUCH             // 0x05210003, 加热接触器
};

enum db_tempctrl2_MACHSET{
  d_tempctrl2_MACHSET_DATEMP_ERRORMIN = 0x05220000, // 0x05220000, 料管温度下限偏差
  d_tempctrl2_MACHSET_DATEMP_ERRORMAX,              // 0x05220001, 料管温度上限偏差
  d_tempctrl2_MACHSET_DATEMP_OILMIN,                // 0x05220002, 油箱油温下限
  d_tempctrl2_MACHSET_DATEMP_OILMAX,                // 0x05220003, 油箱油温上限
  d_tempctrl2_MACHSET_FL_TEMPWORDUPCHECK,           // 0x05220004, 电热连续加温不检查
  d_tempctrl2_MACHSET_DATEMP_COOLINGDIFFERENCE,     // 0x05220005, 冷却水温差
  d_tempctrl2_MACHSET_DATEMP_COOLERONFOROIL,        // 0x05220006, 油温冷却开启温度
  d_tempctrl2_MACHSET_DATEMP_COOLEROFFFOROIL,       // 0x05220007, 油温冷却关闭温度
  d_tempctrl2_MACHSET_DATEMP_PWRONHEATERON,         // 0x05220008, 自动加温
  d_tempctrl2_MACHSET_WTEMPFREEUP,                  // 0x05220009, 自由升温度数
  d_tempctrl2_MACHSET_DATEMP_TEMPCOOLERON,          // 0x0522000a, 料管冷却开启上偏差
  d_tempctrl2_MACHSET_DATEMP_TEMPCOOLEROFF,         // 0x0522000b, 料管冷却关闭下偏差
  d_tempctrl2_MACHSET_DATEMP_RAMP_1,                // 0x0522000c, 温控#1段缓冲区
  d_tempctrl2_MACHSET_DATEMP_RAMP_2,                // 0x0522000d, 温控#2段缓冲区
  d_tempctrl2_MACHSET_DATEMP_RAMP_3,                // 0x0522000e, 温控#3段缓冲区
  d_tempctrl2_MACHSET_DATEMP_RAMP_4,                // 0x0522000f, 温控#4段缓冲区
  d_tempctrl2_MACHSET_DATEMP_RAMP_5,                // 0x05220010, 温控#5段缓冲区
  d_tempctrl2_MACHSET_DATEMP_RAMP_6,                // 0x05220011, 温控#6段缓冲区
  d_tempctrl2_MACHSET_DATEMP_RAMP_7,                // 0x05220012, 温控#7段缓冲区
  d_tempctrl2_MACHSET_DATEMP_RAMP_8,                // 0x05220013, 温控#8段缓冲区
  d_tempctrl2_MACHSET_DATEMP_RAMP_9,                // 0x05220014, 温控#9段缓冲区
  d_tempctrl2_MACHSET_DATEMP_RAMP_10,               // 0x05220015, 温控#10段缓冲区
  d_tempctrl2_MACHSET_DATEMP_RAMP_11,               // 0x05220016, 温控#11段缓冲区
  d_tempctrl2_MACHSET_DATEMP_RAMP_12,               // 0x05220017, 温控#12段缓冲区
  d_tempctrl2_MACHSET_DATEMP_RAMP_13,               // 0x05220018, 温控#13段缓冲区
  d_tempctrl2_MACHSET_DATEMP_RAMP_14,               // 0x05220019, 温控#14段缓冲区
  d_tempctrl2_MACHSET_DATEMP_RAMP_15,               // 0x0522001a, 温控#15段缓冲区
  d_tempctrl2_MACHSET_DATEMP_RAMP_16,               // 0x0522001b, 温控#16段缓冲区
  d_tempctrl2_MACHSET_DAPID_T_1,                    // 0x0522001c, 温控1段系数
  d_tempctrl2_MACHSET_DAPID_T_2,                    // 0x0522001d, 温控2段系数
  d_tempctrl2_MACHSET_DAPID_T_3,                    // 0x0522001e, 温控3段系数
  d_tempctrl2_MACHSET_DAPID_T_4,                    // 0x0522001f, 温控4段系数
  d_tempctrl2_MACHSET_DAPID_T_5,                    // 0x05220020, 温控5段系数
  d_tempctrl2_MACHSET_DAPID_T_6,                    // 0x05220021, 温控6段系数
  d_tempctrl2_MACHSET_DAPID_T_7,                    // 0x05220022, 温控7段系数
  d_tempctrl2_MACHSET_DAPID_T_8,                    // 0x05220023, 温控8段系数
  d_tempctrl2_MACHSET_DAPID_T_9,                    // 0x05220024, 温控9段系数
  d_tempctrl2_MACHSET_DAPID_T_10,                   // 0x05220025, 温控10段系数
  d_tempctrl2_MACHSET_DAPID_T_11,                   // 0x05220026, 温控11段系数
  d_tempctrl2_MACHSET_DAPID_T_12,                   // 0x05220027, 温控12段系数
  d_tempctrl2_MACHSET_DAPID_T_13,                   // 0x05220028, 温控13段系数
  d_tempctrl2_MACHSET_DAPID_T_14,                   // 0x05220029, 温控14段系数
  d_tempctrl2_MACHSET_DAPID_T_15,                   // 0x0522002a, 温控15段系数
  d_tempctrl2_MACHSET_DAPID_T_16,                   // 0x0522002b, 温控16段系数
  d_tempctrl2_MACHSET_TEMP_MTIN_OPT,                // 0x0522002c, 落料口冷却
  d_tempctrl2_MACHSET_TEMP_MTIN_COOL,               // 0x0522002d, 落料口冷却开温度
  d_tempctrl2_MACHSET_TEMP_MTIN_BUFF,               // 0x0522002e, 落料口冷却关缓冲
  d_tempctrl2_MACHSET_DATEMP_OILHEAT,               // 0x0522002f, 预热温度
  d_tempctrl2_MACHSET_DAPID_KP_1,                   // 0x05220030, 温控#1段优化 KP
  d_tempctrl2_MACHSET_DAPID_KP_2,                   // 0x05220031, 温控#2段优化 KP
  d_tempctrl2_MACHSET_DAPID_KP_3,                   // 0x05220032, 温控#3段优化 KP
  d_tempctrl2_MACHSET_DAPID_KP_4,                   // 0x05220033, 温控#4段优化 KP
  d_tempctrl2_MACHSET_DAPID_KP_5,                   // 0x05220034, 温控#5段优化 KP
  d_tempctrl2_MACHSET_DAPID_KP_6,                   // 0x05220035, 温控#6段优化 KP
  d_tempctrl2_MACHSET_DAPID_KP_7,                   // 0x05220036, 温控#7段优化 KP
  d_tempctrl2_MACHSET_DAPID_KP_8,                   // 0x05220037, 温控#8段优化 KP
  d_tempctrl2_MACHSET_DAPID_KP_9,                   // 0x05220038, 温控#9段优化 KP
  d_tempctrl2_MACHSET_DAPID_KP_10,                  // 0x05220039, 温控#10段优化 KP
  d_tempctrl2_MACHSET_DAPID_KP_11,                  // 0x0522003a, 温控#11段优化 KP
  d_tempctrl2_MACHSET_DAPID_KP_12,                  // 0x0522003b, 温控#12段优化 KP
  d_tempctrl2_MACHSET_DAPID_KP_13,                  // 0x0522003c, 温控#13段优化 KP
  d_tempctrl2_MACHSET_DAPID_KP_14,                  // 0x0522003d, 温控#14段优化 KP
  d_tempctrl2_MACHSET_DAPID_KP_15,                  // 0x0522003e, 温控#15段优化 KP
  d_tempctrl2_MACHSET_DAPID_KP_16,                  // 0x0522003f, 温控#16段优化 KP
  d_tempctrl2_MACHSET_DAPID_TI_1,                   // 0x05220040, 温控#1段优化 Ti
  d_tempctrl2_MACHSET_DAPID_TI_2,                   // 0x05220041, 温控#2段优化 Ti
  d_tempctrl2_MACHSET_DAPID_TI_3,                   // 0x05220042, 温控#3段优化 Ti
  d_tempctrl2_MACHSET_DAPID_TI_4,                   // 0x05220043, 温控#4段优化 Ti
  d_tempctrl2_MACHSET_DAPID_TI_5,                   // 0x05220044, 温控#5段优化 Ti
  d_tempctrl2_MACHSET_DAPID_TI_6,                   // 0x05220045, 温控#6段优化 Ti
  d_tempctrl2_MACHSET_DAPID_TI_7,                   // 0x05220046, 温控#7段优化 Ti
  d_tempctrl2_MACHSET_DAPID_TI_8,                   // 0x05220047, 温控#8段优化 Ti
  d_tempctrl2_MACHSET_DAPID_TI_9,                   // 0x05220048, 温控#9段优化 Ti
  d_tempctrl2_MACHSET_DAPID_TI_10,                  // 0x05220049, 温控#10段优化 Ti
  d_tempctrl2_MACHSET_DAPID_TI_11,                  // 0x0522004a, 温控#11段优化 Ti
  d_tempctrl2_MACHSET_DAPID_TI_12,                  // 0x0522004b, 温控#12段优化 Ti
  d_tempctrl2_MACHSET_DAPID_TI_13,                  // 0x0522004c, 温控#13段优化 Ti
  d_tempctrl2_MACHSET_DAPID_TI_14,                  // 0x0522004d, 温控#14段优化 Ti
  d_tempctrl2_MACHSET_DAPID_TI_15,                  // 0x0522004e, 温控#15段优化 Ti
  d_tempctrl2_MACHSET_DAPID_TI_16,                  // 0x0522004f, 温控#16段优化 Ti
  d_tempctrl2_MACHSET_DAPID_TD_1,                   // 0x05220050, 温控#1段优化 Td
  d_tempctrl2_MACHSET_DAPID_TD_2,                   // 0x05220051, 温控#2段优化 Td
  d_tempctrl2_MACHSET_DAPID_TD_3,                   // 0x05220052, 温控#3段优化 Td
  d_tempctrl2_MACHSET_DAPID_TD_4,                   // 0x05220053, 温控#4段优化 Td
  d_tempctrl2_MACHSET_DAPID_TD_5,                   // 0x05220054, 温控#5段优化 Td
  d_tempctrl2_MACHSET_DAPID_TD_6,                   // 0x05220055, 温控#6段优化 Td
  d_tempctrl2_MACHSET_DAPID_TD_7,                   // 0x05220056, 温控#7段优化 Td
  d_tempctrl2_MACHSET_DAPID_TD_8,                   // 0x05220057, 温控#8段优化 Td
  d_tempctrl2_MACHSET_DAPID_TD_9,                   // 0x05220058, 温控#9段优化 Td
  d_tempctrl2_MACHSET_DAPID_TD_10,                  // 0x05220059, 温控#10段优化 Td
  d_tempctrl2_MACHSET_DAPID_TD_11,                  // 0x0522005a, 温控#11段优化 Td
  d_tempctrl2_MACHSET_DAPID_TD_12,                  // 0x0522005b, 温控#12段优化 Td
  d_tempctrl2_MACHSET_DAPID_TD_13,                  // 0x0522005c, 温控#13段优化 Td
  d_tempctrl2_MACHSET_DAPID_TD_14,                  // 0x0522005d, 温控#14段优化 Td
  d_tempctrl2_MACHSET_DAPID_TD_15,                  // 0x0522005e, 温控#15段优化 Td
  d_tempctrl2_MACHSET_DAPID_TD_16,                  // 0x0522005f, 温控#16段优化 Td
  d_tempctrl2_MACHSET_DAPID_TC_1,                   // 0x05220060, 温控#1段优化 Tc
  d_tempctrl2_MACHSET_DAPID_TC_2,                   // 0x05220061, 温控#2段优化 Tc
  d_tempctrl2_MACHSET_DAPID_TC_3,                   // 0x05220062, 温控#3段优化 Tc
  d_tempctrl2_MACHSET_DAPID_TC_4,                   // 0x05220063, 温控#4段优化 Tc
  d_tempctrl2_MACHSET_DAPID_TC_5,                   // 0x05220064, 温控#5段优化 Tc
  d_tempctrl2_MACHSET_DAPID_TC_6,                   // 0x05220065, 温控#6段优化 Tc
  d_tempctrl2_MACHSET_DAPID_TC_7,                   // 0x05220066, 温控#7段优化 Tc
  d_tempctrl2_MACHSET_DAPID_TC_8,                   // 0x05220067, 温控#8段优化 Tc
  d_tempctrl2_MACHSET_DAPID_TC_9,                   // 0x05220068, 温控#9段优化 Tc
  d_tempctrl2_MACHSET_DAPID_TC_10,                  // 0x05220069, 温控#10段优化 Tc
  d_tempctrl2_MACHSET_DAPID_TC_11,                  // 0x0522006a, 温控#11段优化 Tc
  d_tempctrl2_MACHSET_DAPID_TC_12,                  // 0x0522006b, 温控#12段优化 Tc
  d_tempctrl2_MACHSET_DAPID_TC_13,                  // 0x0522006c, 温控#13段优化 Tc
  d_tempctrl2_MACHSET_DAPID_TC_14,                  // 0x0522006d, 温控#14段优化 Tc
  d_tempctrl2_MACHSET_DAPID_TC_15,                  // 0x0522006e, 温控#15段优化 Tc
  d_tempctrl2_MACHSET_DAPID_TC_16,                  // 0x0522006f, 温控#16段优化 Tc
  d_tempctrl2_MACHSET_TEMP_PROTECT,                 // 0x05220070, 温度保护上限
  d_tempctrl2_MACHSET_TEMPCOOLCTRL,                 // 0x05220071, 落料口冷却功能
  d_tempctrl2_MACHSET_TEMPCOOLMAX                   // 0x05220072, 落料口温度上限
};

enum db_tempctrl2_MOLDSET{
  d_tempctrl2_MOLDSET_DATEMP_SETTING_1 = 0x05230000, // 0x05230000, 温度设定值1
  d_tempctrl2_MOLDSET_DATEMP_SETTING_2,              // 0x05230001, 温度设定值2
  d_tempctrl2_MOLDSET_DATEMP_SETTING_3,              // 0x05230002, 温度设定值3
  d_tempctrl2_MOLDSET_DATEMP_SETTING_4,              // 0x05230003, 温度设定值4
  d_tempctrl2_MOLDSET_DATEMP_SETTING_5,              // 0x05230004, 温度设定值5
  d_tempctrl2_MOLDSET_DATEMP_SETTING_6,              // 0x05230005, 温度设定值6
  d_tempctrl2_MOLDSET_DATEMP_SETTING_7,              // 0x05230006, 温度设定值7
  d_tempctrl2_MOLDSET_DATEMP_SETTING_8,              // 0x05230007, 温度设定值8
  d_tempctrl2_MOLDSET_DATEMP_SETTING_9,              // 0x05230008, 温度设定值9
  d_tempctrl2_MOLDSET_DATEMP_SETTING_10,             // 0x05230009, 温度设定值10
  d_tempctrl2_MOLDSET_DATEMP_SETTING_11,             // 0x0523000a, 温度设定值11
  d_tempctrl2_MOLDSET_DATEMP_SETTING_12,             // 0x0523000b, 温度设定值12
  d_tempctrl2_MOLDSET_DATEMP_SETTING_13,             // 0x0523000c, 温度设定值13
  d_tempctrl2_MOLDSET_DATEMP_SETTING_14,             // 0x0523000d, 温度设定值14
  d_tempctrl2_MOLDSET_DATEMP_SETTING_15,             // 0x0523000e, 温度设定值15
  d_tempctrl2_MOLDSET_DATEMP_SETTING_16,             // 0x0523000f, 温度设定值16
  d_tempctrl2_MOLDSET_DATEMP_MAX_1,                  // 0x05230010, 温度上限1
  d_tempctrl2_MOLDSET_DATEMP_MAX_2,                  // 0x05230011, 温度上限2
  d_tempctrl2_MOLDSET_DATEMP_MAX_3,                  // 0x05230012, 温度上限3
  d_tempctrl2_MOLDSET_DATEMP_MAX_4,                  // 0x05230013, 温度上限4
  d_tempctrl2_MOLDSET_DATEMP_MAX_5,                  // 0x05230014, 温度上限5
  d_tempctrl2_MOLDSET_DATEMP_MAX_6,                  // 0x05230015, 温度上限5
  d_tempctrl2_MOLDSET_DATEMP_MAX_7,                  // 0x05230016, 温度上限7
  d_tempctrl2_MOLDSET_DATEMP_MAX_8,                  // 0x05230017, 温度上限8
  d_tempctrl2_MOLDSET_DATEMP_MAX_9,                  // 0x05230018, 温度上限9
  d_tempctrl2_MOLDSET_DATEMP_MAX_10,                 // 0x05230019, 温度上限10
  d_tempctrl2_MOLDSET_DATEMP_MAX_11,                 // 0x0523001a, 温度上限11
  d_tempctrl2_MOLDSET_DATEMP_MAX_12,                 // 0x0523001b, 温度上限12
  d_tempctrl2_MOLDSET_DATEMP_MAX_13,                 // 0x0523001c, 温度上限13
  d_tempctrl2_MOLDSET_DATEMP_MAX_14,                 // 0x0523001d, 温度上限14
  d_tempctrl2_MOLDSET_DATEMP_MAX_15,                 // 0x0523001e, 温度上限15
  d_tempctrl2_MOLDSET_DATEMP_MAX_16,                 // 0x0523001f, 温度上限16
  d_tempctrl2_MOLDSET_DATEMP_MIN_1,                  // 0x05230020, 温度下限1
  d_tempctrl2_MOLDSET_DATEMP_MIN_2,                  // 0x05230021, 温度下限2
  d_tempctrl2_MOLDSET_DATEMP_MIN_3,                  // 0x05230022, 温度下限3
  d_tempctrl2_MOLDSET_DATEMP_MIN_4,                  // 0x05230023, 温度下限4
  d_tempctrl2_MOLDSET_DATEMP_MIN_5,                  // 0x05230024, 温度下限5
  d_tempctrl2_MOLDSET_DATEMP_MIN_6,                  // 0x05230025, 温度下限6
  d_tempctrl2_MOLDSET_DATEMP_MIN_7,                  // 0x05230026, 温度下限7
  d_tempctrl2_MOLDSET_DATEMP_MIN_8,                  // 0x05230027, 温度下限8
  d_tempctrl2_MOLDSET_DATEMP_MIN_9,                  // 0x05230028, 温度下限9
  d_tempctrl2_MOLDSET_DATEMP_MIN_10,                 // 0x05230029, 温度下限10
  d_tempctrl2_MOLDSET_DATEMP_MIN_11,                 // 0x0523002a, 温度下限11
  d_tempctrl2_MOLDSET_DATEMP_MIN_12,                 // 0x0523002b, 温度下限12
  d_tempctrl2_MOLDSET_DATEMP_MIN_13,                 // 0x0523002c, 温度下限13
  d_tempctrl2_MOLDSET_DATEMP_MIN_14,                 // 0x0523002d, 温度下限14
  d_tempctrl2_MOLDSET_DATEMP_MIN_15,                 // 0x0523002e, 温度下限15
  d_tempctrl2_MOLDSET_DATEMP_MIN_16,                 // 0x0523002f, 温度下限16
  d_tempctrl2_MOLDSET_TM_HEATERWARM,                 // 0x05230030, 预温计时
  d_tempctrl2_MOLDSET_FL_HEATEROFFIFALARM,           // 0x05230031, 警报时间到关电热
  d_tempctrl2_MOLDSET_FL_TEMPHOLD,                   // 0x05230032, 保温功能选择
  d_tempctrl2_MOLDSET_DATEMP_HOLD,                   // 0x05230033, 保温温度
  d_tempctrl2_MOLDSET_TM_RESPONSELESSCYCLE,          // 0x05230034, 手动温控周期
  d_tempctrl2_MOLDSET_DACENT_HEATERON,               // 0x05230035, 手动加热百分比
  d_tempctrl2_MOLDSET_FL_TEMPCOOL,                   // 0x05230036, 料管冷却器使用
  d_tempctrl2_MOLDSET_MAX_INFLECT_CNT,               // 0x05230037, 拐点数
  d_tempctrl2_MOLDSET_FL_DATEMPINJM,                 // 0x05230038, 射嘴比例温控功能
  d_tempctrl2_MOLDSET_FL_TEMPLIVE,                   // 0x05230039, 温度优化
  d_tempctrl2_MOLDSET_TEMP_PORT,                     // 0x0523003a, 温度特殊通道
  d_tempctrl2_MOLDSET_TEMP_PORT_BL,                  // 0x0523003b, 温度比例
  d_tempctrl2_MOLDSET_FL_TOGETHER,                   // 0x0523003c, 同步升温
  d_tempctrl2_MOLDSET_FL_TEMPYIZI                    // 0x0523003d, 超温抑制
};

enum db_tempctrl2_ZDSET{
  d_tempctrl2_ZDSET_WTEMP_CHZERO_1 = 0x05240000, // 0x05240000, 
  d_tempctrl2_ZDSET_WTEMP_CHZERO_2,              // 0x05240001, 
  d_tempctrl2_ZDSET_WTEMP_CHZERO_3,              // 0x05240002, 
  d_tempctrl2_ZDSET_WTEMP_CHZERO_4,              // 0x05240003, 
  d_tempctrl2_ZDSET_WTEMP_CHZERO_5,              // 0x05240004, 
  d_tempctrl2_ZDSET_WTEMP_CHZERO_6,              // 0x05240005, 
  d_tempctrl2_ZDSET_WTEMP_CHZERO_7,              // 0x05240006, 
  d_tempctrl2_ZDSET_WTEMP_CHZERO_8,              // 0x05240007, 
  d_tempctrl2_ZDSET_WTEMP_CHZERO_9,              // 0x05240008, 
  d_tempctrl2_ZDSET_WTEMP_CHZERO_10,             // 0x05240009, 
  d_tempctrl2_ZDSET_WTEMP_CHZERO_11,             // 0x0524000a, 
  d_tempctrl2_ZDSET_WTEMP_CHZERO_12,             // 0x0524000b, 
  d_tempctrl2_ZDSET_WTEMP_CHZERO_13,             // 0x0524000c, 
  d_tempctrl2_ZDSET_WTEMP_CHZERO_14,             // 0x0524000d, 
  d_tempctrl2_ZDSET_WTEMP_CHZERO_15,             // 0x0524000e, 
  d_tempctrl2_ZDSET_WTEMP_CHZERO_16,             // 0x0524000f, 
  d_tempctrl2_ZDSET_WTEMP_CHB_1,                 // 0x05240010, 
  d_tempctrl2_ZDSET_WTEMP_CHB_2,                 // 0x05240011, 
  d_tempctrl2_ZDSET_WTEMP_CHB_3,                 // 0x05240012, 
  d_tempctrl2_ZDSET_WTEMP_CHB_4,                 // 0x05240013, 
  d_tempctrl2_ZDSET_WTEMP_CHB_5,                 // 0x05240014, 
  d_tempctrl2_ZDSET_WTEMP_CHB_6,                 // 0x05240015, 
  d_tempctrl2_ZDSET_WTEMP_CHB_7,                 // 0x05240016, 
  d_tempctrl2_ZDSET_WTEMP_CHB_8,                 // 0x05240017, 
  d_tempctrl2_ZDSET_WTEMP_CHB_9,                 // 0x05240018, 
  d_tempctrl2_ZDSET_WTEMP_CHB_10,                // 0x05240019, 
  d_tempctrl2_ZDSET_WTEMP_CHB_11,                // 0x0524001a, 
  d_tempctrl2_ZDSET_WTEMP_CHB_12,                // 0x0524001b, 
  d_tempctrl2_ZDSET_WTEMP_CHB_13,                // 0x0524001c, 
  d_tempctrl2_ZDSET_WTEMP_CHB_14,                // 0x0524001d, 
  d_tempctrl2_ZDSET_WTEMP_CHB_15,                // 0x0524001e, 
  d_tempctrl2_ZDSET_WTEMP_CHB_16,                // 0x0524001f, 
  d_tempctrl2_ZDSET_WTEMP_OFFSET,                // 0x05240020, 室温校准
  d_tempctrl2_ZDSET_HEAT_RATE_1,                 // 0x05240021, 
  d_tempctrl2_ZDSET_HEAT_RATE_2,                 // 0x05240022, 
  d_tempctrl2_ZDSET_HEAT_RATE_3,                 // 0x05240023, 
  d_tempctrl2_ZDSET_HEAT_RATE_4,                 // 0x05240024, 
  d_tempctrl2_ZDSET_HEAT_RATE_5,                 // 0x05240025, 
  d_tempctrl2_ZDSET_HEAT_RATE_6,                 // 0x05240026, 
  d_tempctrl2_ZDSET_HEAT_RATE_7,                 // 0x05240027, 
  d_tempctrl2_ZDSET_HEAT_RATE_8,                 // 0x05240028, 
  d_tempctrl2_ZDSET_HEAT_RATE_9,                 // 0x05240029, 
  d_tempctrl2_ZDSET_HEAT_RATE_10,                // 0x0524002a, 
  d_tempctrl2_ZDSET_HEAT_RATE_11,                // 0x0524002b, 
  d_tempctrl2_ZDSET_HEAT_RATE_12,                // 0x0524002c, 
  d_tempctrl2_ZDSET_HEAT_RATE_13,                // 0x0524002d, 
  d_tempctrl2_ZDSET_HEAT_RATE_14,                // 0x0524002e, 
  d_tempctrl2_ZDSET_HEAT_RATE_15,                // 0x0524002f, 
  d_tempctrl2_ZDSET_HEAT_RATE_16                 // 0x05240030, 
};

enum db_tempctrl2_STATE{
  d_tempctrl2_STATE_FL_ERROR0 = 0x05260000, // 0x05260000, 
  d_tempctrl2_STATE_FL_ERROR1,              // 0x05260001, 
  d_tempctrl2_STATE_DATEMP_REAL_1,          // 0x05260002, 温度实际值0
  d_tempctrl2_STATE_DATEMP_REAL_2,          // 0x05260003, 温度实际值1
  d_tempctrl2_STATE_DATEMP_REAL_3,          // 0x05260004, 温度实际值2
  d_tempctrl2_STATE_DATEMP_REAL_4,          // 0x05260005, 温度实际值3
  d_tempctrl2_STATE_DATEMP_REAL_5,          // 0x05260006, 温度实际值4
  d_tempctrl2_STATE_DATEMP_REAL_6,          // 0x05260007, 温度实际值5
  d_tempctrl2_STATE_DATEMP_REAL_7,          // 0x05260008, 温度实际值6
  d_tempctrl2_STATE_DATEMP_REAL_8,          // 0x05260009, 温度实际值7
  d_tempctrl2_STATE_DATEMP_REAL_9,          // 0x0526000a, 温度实际值8
  d_tempctrl2_STATE_DATEMP_REAL_10,         // 0x0526000b, 温度实际值9
  d_tempctrl2_STATE_DATEMP_REAL_11,         // 0x0526000c, 温度实际值10
  d_tempctrl2_STATE_DATEMP_REAL_12,         // 0x0526000d, 温度实际值11
  d_tempctrl2_STATE_DATEMP_REAL_13,         // 0x0526000e, 温度实际值12
  d_tempctrl2_STATE_DATEMP_REAL_14,         // 0x0526000f, 温度实际值13
  d_tempctrl2_STATE_DATEMP_REAL_15,         // 0x05260010, 温度实际值14
  d_tempctrl2_STATE_DATEMP_REAL_16,         // 0x05260011, 温度实际值15
  d_tempctrl2_STATE_FL_TEMPSTATUS_1,        // 0x05260012, 温度加温状态1
  d_tempctrl2_STATE_FL_TEMPSTATUS_2,        // 0x05260013, 温度加温状态2
  d_tempctrl2_STATE_FL_TEMPSTATUS_3,        // 0x05260014, 温度加温状态3
  d_tempctrl2_STATE_FL_TEMPSTATUS_4,        // 0x05260015, 温度加温状态4
  d_tempctrl2_STATE_FL_TEMPSTATUS_5,        // 0x05260016, 温度加温状态5
  d_tempctrl2_STATE_FL_TEMPSTATUS_6,        // 0x05260017, 温度加温状态6
  d_tempctrl2_STATE_FL_TEMPSTATUS_7,        // 0x05260018, 温度加温状态7
  d_tempctrl2_STATE_FL_TEMPSTATUS_8,        // 0x05260019, 温度加温状态8
  d_tempctrl2_STATE_FL_TEMPSTATUS_9,        // 0x0526001a, 温度加温状态9
  d_tempctrl2_STATE_FL_TEMPSTATUS_10,       // 0x0526001b, 温度加温状态10
  d_tempctrl2_STATE_FL_TEMPSTATUS_11,       // 0x0526001c, 温度实际值11
  d_tempctrl2_STATE_FL_TEMPSTATUS_12,       // 0x0526001d, 温度实际值12
  d_tempctrl2_STATE_FL_TEMPSTATUS_13,       // 0x0526001e, 温度实际值13
  d_tempctrl2_STATE_FL_TEMPSTATUS_14,       // 0x0526001f, 温度实际值14
  d_tempctrl2_STATE_FL_TEMPSTATUS_15,       // 0x05260020, 温度实际值14
  d_tempctrl2_STATE_FL_TEMPSTATUS_16,       // 0x05260021, 温度加温状态
  d_tempctrl2_STATE_WTEMP_LM73,             // 0x05260022, 室温LM73的温度
  d_tempctrl2_STATE_WTEMP_RELAT,            // 0x05260023, 校准后的实时温度
  d_tempctrl2_STATE_WTEMP_ADJ,              // 0x05260024, 温度系数 
  d_tempctrl2_STATE_WTEMP_LM73UV,           // 0x05260025, 室温对应uV值
  d_tempctrl2_STATE_WTEMP_ADVALUE,          // 0x05260026, AD采样得到的实时值
  d_tempctrl2_STATE_WTEMP_REAL,             // 0x05260027, 实际温度(不叠加室温值)
  d_tempctrl2_STATE_WTEMP_CHANNEL,          // 0x05260028, 温度通道
  d_tempctrl2_STATE_WTEMP_ADJALL,           // 0x05260029, 测试画面标志  
  d_tempctrl2_STATE_WTEMP_ADJAMB,           // 0x0526002a, 室温校对  
  d_tempctrl2_STATE_FL_HEATERSTATUS,        // 0x0526002b, ST_电热
  d_tempctrl2_STATE_WSTATERSV6,             // 0x0526002c, 保温倒计时
  d_tempctrl2_STATE_DATEMP_REALMOTOR,       // 0x0526002d, 电机温度
  d_tempctrl2_STATE_MOVE_STEP_1,            // 0x0526002e, 
  d_tempctrl2_STATE_MOVE_STEP_2,            // 0x0526002f, 
  d_tempctrl2_STATE_MOVE_STEP_3,            // 0x05260030, 
  d_tempctrl2_STATE_MOVE_STEP_4,            // 0x05260031, 
  d_tempctrl2_STATE_MOVE_STEP_5,            // 0x05260032, 
  d_tempctrl2_STATE_MOVE_STEP_6,            // 0x05260033, 
  d_tempctrl2_STATE_MOVE_STEP_7,            // 0x05260034, 
  d_tempctrl2_STATE_MOVE_STEP_8,            // 0x05260035, 
  d_tempctrl2_STATE_MOVE_STEP_9,            // 0x05260036, 
  d_tempctrl2_STATE_MOVE_STEP_10,           // 0x05260037, 
  d_tempctrl2_STATE_MOVE_STEP_11,           // 0x05260038, 
  d_tempctrl2_STATE_MOVE_STEP_12,           // 0x05260039, 
  d_tempctrl2_STATE_MOVE_STEP_13,           // 0x0526003a, 
  d_tempctrl2_STATE_MOVE_STEP_14,           // 0x0526003b, 
  d_tempctrl2_STATE_MOVE_STEP_15,           // 0x0526003c, 
  d_tempctrl2_STATE_MOVE_STEP_16,           // 0x0526003d, 
  d_tempctrl2_STATE_ADJ_EXT_TEMP,           // 0x0526003e, 
  d_tempctrl2_STATE_DATEMP_CNVALID_1,       // 0x0526003f, 
  d_tempctrl2_STATE_DATEMP_CNVALID_2,       // 0x05260040, 
  d_tempctrl2_STATE_DATEMP_CNVALID_3,       // 0x05260041, 
  d_tempctrl2_STATE_DATEMP_CNVALID_4,       // 0x05260042, 
  d_tempctrl2_STATE_DATEMP_CNVALID_5,       // 0x05260043, 
  d_tempctrl2_STATE_DATEMP_CNVALID_6,       // 0x05260044, 
  d_tempctrl2_STATE_DATEMP_CNVALID_7,       // 0x05260045, 
  d_tempctrl2_STATE_DATEMP_CNVALID_8,       // 0x05260046, 
  d_tempctrl2_STATE_DATEMP_CNVALID_9,       // 0x05260047, 
  d_tempctrl2_STATE_DATEMP_CNVALID_10,      // 0x05260048, 
  d_tempctrl2_STATE_DATEMP_CNVALID_11,      // 0x05260049, 
  d_tempctrl2_STATE_DATEMP_CNVALID_12,      // 0x0526004a, 
  d_tempctrl2_STATE_DATEMP_CNVALID_13,      // 0x0526004b, 
  d_tempctrl2_STATE_DATEMP_CNVALID_14,      // 0x0526004c, 
  d_tempctrl2_STATE_DATEMP_CNVALID_15,      // 0x0526004d, 
  d_tempctrl2_STATE_DATEMP_CNVALID_16,      // 0x0526004e, 
  d_tempctrl2_STATE_DATEMP_CNINVALID_1,     // 0x0526004f, 
  d_tempctrl2_STATE_DATEMP_CNINVALID_2,     // 0x05260050, 
  d_tempctrl2_STATE_DATEMP_CNINVALID_3,     // 0x05260051, 
  d_tempctrl2_STATE_DATEMP_CNINVALID_4,     // 0x05260052, 
  d_tempctrl2_STATE_DATEMP_CNINVALID_5,     // 0x05260053, 
  d_tempctrl2_STATE_DATEMP_CNINVALID_6,     // 0x05260054, 
  d_tempctrl2_STATE_DATEMP_CNINVALID_7,     // 0x05260055, 
  d_tempctrl2_STATE_DATEMP_CNINVALID_8,     // 0x05260056, 
  d_tempctrl2_STATE_DATEMP_CNINVALID_9,     // 0x05260057, 
  d_tempctrl2_STATE_DATEMP_CNINVALID_10,    // 0x05260058, 
  d_tempctrl2_STATE_DATEMP_CNINVALID_11,    // 0x05260059, 
  d_tempctrl2_STATE_DATEMP_CNINVALID_12,    // 0x0526005a, 
  d_tempctrl2_STATE_DATEMP_CNINVALID_13,    // 0x0526005b, 
  d_tempctrl2_STATE_DATEMP_CNINVALID_14,    // 0x0526005c, 
  d_tempctrl2_STATE_DATEMP_CNINVALID_15,    // 0x0526005d, 
  d_tempctrl2_STATE_DATEMP_CNINVALID_16,    // 0x0526005e, 
  d_tempctrl2_STATE_DATEMP_CNINREAD_1,      // 0x0526005f, 
  d_tempctrl2_STATE_DATEMP_CNINREAD_2,      // 0x05260060, 
  d_tempctrl2_STATE_DATEMP_CNINREAD_3,      // 0x05260061, 
  d_tempctrl2_STATE_DATEMP_CNINREAD_4,      // 0x05260062, 
  d_tempctrl2_STATE_DATEMP_CNINREAD_5,      // 0x05260063, 
  d_tempctrl2_STATE_DATEMP_CNINREAD_6,      // 0x05260064, 
  d_tempctrl2_STATE_DATEMP_CNINREAD_7,      // 0x05260065, 
  d_tempctrl2_STATE_DATEMP_CNINREAD_8,      // 0x05260066, 
  d_tempctrl2_STATE_DATEMP_CNINREAD_9,      // 0x05260067, 
  d_tempctrl2_STATE_DATEMP_CNINREAD_10,     // 0x05260068, 
  d_tempctrl2_STATE_DATEMP_CNINREAD_11,     // 0x05260069, 
  d_tempctrl2_STATE_DATEMP_CNINREAD_12,     // 0x0526006a, 
  d_tempctrl2_STATE_DATEMP_CNINREAD_13,     // 0x0526006b, 
  d_tempctrl2_STATE_DATEMP_CNINREAD_14,     // 0x0526006c, 
  d_tempctrl2_STATE_DATEMP_CNINREAD_15,     // 0x0526006d, 
  d_tempctrl2_STATE_DATEMP_CNINREAD_16,     // 0x0526006e, 
  d_tempctrl2_STATE_DATEMP_VALUE_1,         // 0x0526006f, 
  d_tempctrl2_STATE_DATEMP_VALUE_2,         // 0x05260070, 
  d_tempctrl2_STATE_DATEMP_VALUE_3,         // 0x05260071, 
  d_tempctrl2_STATE_DATEMP_VALUE_4,         // 0x05260072, 
  d_tempctrl2_STATE_DATEMP_VALUE_5,         // 0x05260073, 
  d_tempctrl2_STATE_DATEMP_VALUE_6,         // 0x05260074, 
  d_tempctrl2_STATE_DATEMP_VALUE_7,         // 0x05260075, 
  d_tempctrl2_STATE_DATEMP_VALUE_8,         // 0x05260076, 
  d_tempctrl2_STATE_DATEMP_VALUE_9,         // 0x05260077, 
  d_tempctrl2_STATE_DATEMP_VALUE_10,        // 0x05260078, 
  d_tempctrl2_STATE_DATEMP_VALUE_11,        // 0x05260079, 
  d_tempctrl2_STATE_DATEMP_VALUE_12,        // 0x0526007a, 
  d_tempctrl2_STATE_DATEMP_VALUE_13,        // 0x0526007b, 
  d_tempctrl2_STATE_DATEMP_VALUE_14,        // 0x0526007c, 
  d_tempctrl2_STATE_DATEMP_VALUE_15,        // 0x0526007d, 
  d_tempctrl2_STATE_DATEMP_VALUE_16,        // 0x0526007e, 
  d_tempctrl2_STATE_DROP_COL_STATE,         // 0x0526007f, 落料口冷却是否开启
  d_tempctrl2_STATE_DATEMP_CHARGEPOS,       // 0x05260080, 落料口温度
  d_tempctrl2_STATE_DATEMP_WORKTM1,         // 0x05260081, 60s内电热输出时间1
  d_tempctrl2_STATE_DATEMP_WORKTM2,         // 0x05260082, 60s内电热输出时间2
  d_tempctrl2_STATE_DATEMP_WORKTM3,         // 0x05260083, 60s内电热输出时间3
  d_tempctrl2_STATE_DATEMP_WORKTM4,         // 0x05260084, 60s内电热输出时间4
  d_tempctrl2_STATE_DATEMP_WORKTM5,         // 0x05260085, 60s内电热输出时间5
  d_tempctrl2_STATE_DATEMP_WORKTM6,         // 0x05260086, 60s内电热输出时间6
  d_tempctrl2_STATE_DATEMP_WORKTM7,         // 0x05260087, 60s内电热输出时间7
  d_tempctrl2_STATE_DATEMP_WORKTM8,         // 0x05260088, 60s内电热输出时间8
  d_tempctrl2_STATE_DATEMP_WORKTM9,         // 0x05260089, 60s内电热输出时间9
  d_tempctrl2_STATE_DATEMP_WORKTM10,        // 0x0526008a, 60s内电热输出时间10
  d_tempctrl2_STATE_DATEMP_WORKTM11,        // 0x0526008b, 60s内电热输出时间11
  d_tempctrl2_STATE_DATEMP_WORKTM12,        // 0x0526008c, 60s内电热输出时间12
  d_tempctrl2_STATE_DATEMP_WORKTM13,        // 0x0526008d, 60s内电热输出时间13
  d_tempctrl2_STATE_DATEMP_WORKTM14,        // 0x0526008e, 60s内电热输出时间14
  d_tempctrl2_STATE_DATEMP_WORKTM15,        // 0x0526008f, 60s内电热输出时间15
  d_tempctrl2_STATE_DATEMP_WORKTM16,        // 0x05260090, 60s内电热输出时间16
  d_tempctrl2_STATE_DATEMP_E505_VER1,       // 0x05260091, 室温扩展板版本号1
  d_tempctrl2_STATE_DATEMP_E505_VER2,       // 0x05260092, 室温扩展板版本号2
  d_tempctrl2_STATE_DATEMP_E502_VER1,       // 0x05260093, 温度扩展板E502版本号1
  d_tempctrl2_STATE_DATEMP_E502_VER2,       // 0x05260094, 温度扩展板E502版本号
  d_tempctrl2_STATE_DATEMP_YOUHUAXUQIU,     // 0x05260095, 温度优化提示开马达的闪烁指示
  d_tempctrl2_STATE_DATEMP_YOUHUAOK         // 0x05260096, 温度优化开启与完成标记
};

enum db_io1_STATE{
  d_io1_STATE_FL_ERROR0 = 0x06160000, // 0x06160000, 
  d_io1_STATE_FL_ERROR1,              // 0x06160001, 
  d_io1_STATE_pi_input_1,             // 0x06160002, 
  d_io1_STATE_pi_input_2,             // 0x06160003, 
  d_io1_STATE_pi_input_3,             // 0x06160004, 
  d_io1_STATE_pi_input_4,             // 0x06160005, 
  d_io1_STATE_pi_input_5,             // 0x06160006, 
  d_io1_STATE_pi_input_6,             // 0x06160007, 
  d_io1_STATE_pi_input_7,             // 0x06160008, 
  d_io1_STATE_pi_input_8,             // 0x06160009, 
  d_io1_STATE_po_output_1,            // 0x0616000a, 
  d_io1_STATE_po_output_2,            // 0x0616000b, 
  d_io1_STATE_po_output_3,            // 0x0616000c, 
  d_io1_STATE_po_output_4,            // 0x0616000d, 
  d_io1_STATE_po_output_5,            // 0x0616000e, 
  d_io1_STATE_po_output_6,            // 0x0616000f, 
  d_io1_STATE_po_output_7,            // 0x06160010, 
  d_io1_STATE_po_output_8,            // 0x06160011, 
  d_io1_STATE_PI_STAT_1,              // 0x06160012, 
  d_io1_STATE_PI_STAT_2,              // 0x06160013, 
  d_io1_STATE_PI_STAT_3,              // 0x06160014, 
  d_io1_STATE_PI_STAT_4,              // 0x06160015, 
  d_io1_STATE_PI_STAT_5,              // 0x06160016, 
  d_io1_STATE_PI_STAT_6,              // 0x06160017, 
  d_io1_STATE_PI_STAT_7,              // 0x06160018, 
  d_io1_STATE_PI_STAT_8,              // 0x06160019, 
  d_io1_STATE_PI_STAT_9,              // 0x0616001a, 
  d_io1_STATE_PI_STAT_10,             // 0x0616001b, 
  d_io1_STATE_PI_STAT_11,             // 0x0616001c, 
  d_io1_STATE_PI_STAT_12,             // 0x0616001d, 
  d_io1_STATE_PI_STAT_13,             // 0x0616001e, 
  d_io1_STATE_PI_STAT_14,             // 0x0616001f, 
  d_io1_STATE_PI_STAT_15,             // 0x06160020, 
  d_io1_STATE_PI_STAT_16,             // 0x06160021, 
  d_io1_STATE_PI_STAT_17,             // 0x06160022, 
  d_io1_STATE_PI_STAT_18,             // 0x06160023, 
  d_io1_STATE_PI_STAT_19,             // 0x06160024, 
  d_io1_STATE_PI_STAT_20,             // 0x06160025, 
  d_io1_STATE_PI_STAT_21,             // 0x06160026, 
  d_io1_STATE_PI_STAT_22,             // 0x06160027, 
  d_io1_STATE_PI_STAT_23,             // 0x06160028, 
  d_io1_STATE_PI_STAT_24,             // 0x06160029, 
  d_io1_STATE_PI_STAT_25,             // 0x0616002a, 
  d_io1_STATE_PI_STAT_26,             // 0x0616002b, 
  d_io1_STATE_PI_STAT_27,             // 0x0616002c, 
  d_io1_STATE_PI_STAT_28,             // 0x0616002d, 
  d_io1_STATE_PI_STAT_29,             // 0x0616002e, 
  d_io1_STATE_PI_STAT_30,             // 0x0616002f, 
  d_io1_STATE_PI_STAT_31,             // 0x06160030, 
  d_io1_STATE_PI_STAT_32,             // 0x06160031, 
  d_io1_STATE_PI_STAT_33,             // 0x06160032, 
  d_io1_STATE_PI_STAT_34,             // 0x06160033, 
  d_io1_STATE_PI_STAT_35,             // 0x06160034, 
  d_io1_STATE_PI_STAT_36,             // 0x06160035, 
  d_io1_STATE_PI_STAT_37,             // 0x06160036, 
  d_io1_STATE_PI_STAT_38,             // 0x06160037, 
  d_io1_STATE_PI_STAT_39,             // 0x06160038, 
  d_io1_STATE_PI_STAT_40,             // 0x06160039, 
  d_io1_STATE_PI_STAT_41,             // 0x0616003a, 
  d_io1_STATE_PI_STAT_42,             // 0x0616003b, 
  d_io1_STATE_PI_STAT_43,             // 0x0616003c, 
  d_io1_STATE_PI_STAT_44,             // 0x0616003d, 
  d_io1_STATE_PI_STAT_45,             // 0x0616003e, 
  d_io1_STATE_PI_STAT_46,             // 0x0616003f, 
  d_io1_STATE_PI_STAT_47,             // 0x06160040, 
  d_io1_STATE_PI_STAT_48,             // 0x06160041, 
  d_io1_STATE_PI_STAT_49,             // 0x06160042, 
  d_io1_STATE_PI_STAT_50,             // 0x06160043, 
  d_io1_STATE_PI_STAT_51,             // 0x06160044, 
  d_io1_STATE_PI_STAT_52,             // 0x06160045, 
  d_io1_STATE_PI_STAT_53,             // 0x06160046, 
  d_io1_STATE_PI_STAT_54,             // 0x06160047, 
  d_io1_STATE_PI_STAT_55,             // 0x06160048, 
  d_io1_STATE_PI_STAT_56,             // 0x06160049, 
  d_io1_STATE_PI_STAT_57,             // 0x0616004a, 
  d_io1_STATE_PI_STAT_58,             // 0x0616004b, 
  d_io1_STATE_PI_STAT_59,             // 0x0616004c, 
  d_io1_STATE_PI_STAT_60,             // 0x0616004d, 
  d_io1_STATE_PI_STAT_61,             // 0x0616004e, 
  d_io1_STATE_PI_STAT_62,             // 0x0616004f, 
  d_io1_STATE_PI_STAT_63,             // 0x06160050, 
  d_io1_STATE_PI_STAT_64,             // 0x06160051, 
  d_io1_STATE_PI_STAT_65,             // 0x06160052, 
  d_io1_STATE_PI_STAT_66,             // 0x06160053, 
  d_io1_STATE_PI_STAT_67,             // 0x06160054, 
  d_io1_STATE_PI_STAT_68,             // 0x06160055, 
  d_io1_STATE_PI_STAT_69,             // 0x06160056, 
  d_io1_STATE_PI_STAT_70,             // 0x06160057, 
  d_io1_STATE_PI_STAT_71,             // 0x06160058, 
  d_io1_STATE_PI_STAT_72,             // 0x06160059, 
  d_io1_STATE_PI_STAT_73,             // 0x0616005a, 
  d_io1_STATE_PI_STAT_74,             // 0x0616005b, 
  d_io1_STATE_PI_STAT_75,             // 0x0616005c, 
  d_io1_STATE_PI_STAT_76,             // 0x0616005d, 
  d_io1_STATE_PI_STAT_77,             // 0x0616005e, 
  d_io1_STATE_PI_STAT_78,             // 0x0616005f, 
  d_io1_STATE_PI_STAT_79,             // 0x06160060, 
  d_io1_STATE_PI_STAT_80,             // 0x06160061, 
  d_io1_STATE_PI_STAT_81,             // 0x06160062, 
  d_io1_STATE_PI_STAT_82,             // 0x06160063, 
  d_io1_STATE_PI_STAT_83,             // 0x06160064, 
  d_io1_STATE_PI_STAT_84,             // 0x06160065, 
  d_io1_STATE_PI_STAT_85,             // 0x06160066, 
  d_io1_STATE_PI_STAT_86,             // 0x06160067, 
  d_io1_STATE_PI_STAT_87,             // 0x06160068, 
  d_io1_STATE_PI_STAT_88,             // 0x06160069, 
  d_io1_STATE_PI_STAT_89,             // 0x0616006a, 
  d_io1_STATE_PI_STAT_90,             // 0x0616006b, 
  d_io1_STATE_PI_STAT_91,             // 0x0616006c, 
  d_io1_STATE_PI_STAT_92,             // 0x0616006d, 
  d_io1_STATE_PI_STAT_93,             // 0x0616006e, 
  d_io1_STATE_PI_STAT_94,             // 0x0616006f, 
  d_io1_STATE_PI_STAT_95,             // 0x06160070, 
  d_io1_STATE_PI_STAT_96,             // 0x06160071, 
  d_io1_STATE_PI_STAT_97,             // 0x06160072, 
  d_io1_STATE_PI_STAT_98,             // 0x06160073, 
  d_io1_STATE_PI_STAT_99,             // 0x06160074, 
  d_io1_STATE_PI_STAT_100,            // 0x06160075, 
  d_io1_STATE_PI_STAT_101,            // 0x06160076, 
  d_io1_STATE_PI_STAT_102,            // 0x06160077, 
  d_io1_STATE_PI_STAT_103,            // 0x06160078, 
  d_io1_STATE_PI_STAT_104,            // 0x06160079, 
  d_io1_STATE_PI_STAT_105,            // 0x0616007a, 
  d_io1_STATE_PI_STAT_106,            // 0x0616007b, 
  d_io1_STATE_PI_STAT_107,            // 0x0616007c, 
  d_io1_STATE_PI_STAT_108,            // 0x0616007d, 
  d_io1_STATE_PI_STAT_109,            // 0x0616007e, 
  d_io1_STATE_PI_STAT_110,            // 0x0616007f, 
  d_io1_STATE_PI_STAT_111,            // 0x06160080, 
  d_io1_STATE_PI_STAT_112,            // 0x06160081, 
  d_io1_STATE_PI_STAT_113,            // 0x06160082, 
  d_io1_STATE_PI_STAT_114,            // 0x06160083, 
  d_io1_STATE_PI_STAT_115,            // 0x06160084, 
  d_io1_STATE_PI_STAT_116,            // 0x06160085, 
  d_io1_STATE_PI_STAT_117,            // 0x06160086, 
  d_io1_STATE_PI_STAT_118,            // 0x06160087, 
  d_io1_STATE_PI_STAT_119,            // 0x06160088, 
  d_io1_STATE_PI_STAT_120,            // 0x06160089, 
  d_io1_STATE_PI_STAT_121,            // 0x0616008a, 
  d_io1_STATE_PI_STAT_122,            // 0x0616008b, 
  d_io1_STATE_PI_STAT_123,            // 0x0616008c, 
  d_io1_STATE_PI_STAT_124,            // 0x0616008d, 
  d_io1_STATE_PI_STAT_125,            // 0x0616008e, 
  d_io1_STATE_PI_STAT_126,            // 0x0616008f, 
  d_io1_STATE_PI_STAT_127,            // 0x06160090, 
  d_io1_STATE_PI_STAT_128,            // 0x06160091, 
  d_io1_STATE_PO_STAT_1,              // 0x06160092, 
  d_io1_STATE_PO_STAT_2,              // 0x06160093, 
  d_io1_STATE_PO_STAT_3,              // 0x06160094, 
  d_io1_STATE_PO_STAT_4,              // 0x06160095, 
  d_io1_STATE_PO_STAT_5,              // 0x06160096, 
  d_io1_STATE_PO_STAT_6,              // 0x06160097, 
  d_io1_STATE_PO_STAT_7,              // 0x06160098, 
  d_io1_STATE_PO_STAT_8,              // 0x06160099, 
  d_io1_STATE_PO_STAT_9,              // 0x0616009a, 
  d_io1_STATE_PO_STAT_10,             // 0x0616009b, 
  d_io1_STATE_PO_STAT_11,             // 0x0616009c, 
  d_io1_STATE_PO_STAT_12,             // 0x0616009d, 
  d_io1_STATE_PO_STAT_13,             // 0x0616009e, 
  d_io1_STATE_PO_STAT_14,             // 0x0616009f, 
  d_io1_STATE_PO_STAT_15,             // 0x061600a0, 
  d_io1_STATE_PO_STAT_16,             // 0x061600a1, 
  d_io1_STATE_PO_STAT_17,             // 0x061600a2, 
  d_io1_STATE_PO_STAT_18,             // 0x061600a3, 
  d_io1_STATE_PO_STAT_19,             // 0x061600a4, 
  d_io1_STATE_PO_STAT_20,             // 0x061600a5, 
  d_io1_STATE_PO_STAT_21,             // 0x061600a6, 
  d_io1_STATE_PO_STAT_22,             // 0x061600a7, 
  d_io1_STATE_PO_STAT_23,             // 0x061600a8, 
  d_io1_STATE_PO_STAT_24,             // 0x061600a9, 
  d_io1_STATE_PO_STAT_25,             // 0x061600aa, 
  d_io1_STATE_PO_STAT_26,             // 0x061600ab, 
  d_io1_STATE_PO_STAT_27,             // 0x061600ac, 
  d_io1_STATE_PO_STAT_28,             // 0x061600ad, 
  d_io1_STATE_PO_STAT_29,             // 0x061600ae, 
  d_io1_STATE_PO_STAT_30,             // 0x061600af, 
  d_io1_STATE_PO_STAT_31,             // 0x061600b0, 
  d_io1_STATE_PO_STAT_32,             // 0x061600b1, 
  d_io1_STATE_PO_STAT_33,             // 0x061600b2, 
  d_io1_STATE_PO_STAT_34,             // 0x061600b3, 
  d_io1_STATE_PO_STAT_35,             // 0x061600b4, 
  d_io1_STATE_PO_STAT_36,             // 0x061600b5, 
  d_io1_STATE_PO_STAT_37,             // 0x061600b6, 
  d_io1_STATE_PO_STAT_38,             // 0x061600b7, 
  d_io1_STATE_PO_STAT_39,             // 0x061600b8, 
  d_io1_STATE_PO_STAT_40,             // 0x061600b9, 
  d_io1_STATE_PO_STAT_41,             // 0x061600ba, 
  d_io1_STATE_PO_STAT_42,             // 0x061600bb, 
  d_io1_STATE_PO_STAT_43,             // 0x061600bc, 
  d_io1_STATE_PO_STAT_44,             // 0x061600bd, 
  d_io1_STATE_PO_STAT_45,             // 0x061600be, 
  d_io1_STATE_PO_STAT_46,             // 0x061600bf, 
  d_io1_STATE_PO_STAT_47,             // 0x061600c0, 
  d_io1_STATE_PO_STAT_48,             // 0x061600c1, 
  d_io1_STATE_PO_STAT_49,             // 0x061600c2, 
  d_io1_STATE_PO_STAT_50,             // 0x061600c3, 
  d_io1_STATE_PO_STAT_51,             // 0x061600c4, 
  d_io1_STATE_PO_STAT_52,             // 0x061600c5, 
  d_io1_STATE_PO_STAT_53,             // 0x061600c6, 
  d_io1_STATE_PO_STAT_54,             // 0x061600c7, 
  d_io1_STATE_PO_STAT_55,             // 0x061600c8, 
  d_io1_STATE_PO_STAT_56,             // 0x061600c9, 
  d_io1_STATE_PO_STAT_57,             // 0x061600ca, 
  d_io1_STATE_PO_STAT_58,             // 0x061600cb, 
  d_io1_STATE_PO_STAT_59,             // 0x061600cc, 
  d_io1_STATE_PO_STAT_60,             // 0x061600cd, 
  d_io1_STATE_PO_STAT_61,             // 0x061600ce, 
  d_io1_STATE_PO_STAT_62,             // 0x061600cf, 
  d_io1_STATE_PO_STAT_63,             // 0x061600d0, 
  d_io1_STATE_PO_STAT_64,             // 0x061600d1, 
  d_io1_STATE_PO_STAT_65,             // 0x061600d2, 
  d_io1_STATE_PO_STAT_66,             // 0x061600d3, 
  d_io1_STATE_PO_STAT_67,             // 0x061600d4, 
  d_io1_STATE_PO_STAT_68,             // 0x061600d5, 
  d_io1_STATE_PO_STAT_69,             // 0x061600d6, 
  d_io1_STATE_PO_STAT_70,             // 0x061600d7, 
  d_io1_STATE_PO_STAT_71,             // 0x061600d8, 
  d_io1_STATE_PO_STAT_72,             // 0x061600d9, 
  d_io1_STATE_PO_STAT_73,             // 0x061600da, 
  d_io1_STATE_PO_STAT_74,             // 0x061600db, 
  d_io1_STATE_PO_STAT_75,             // 0x061600dc, 
  d_io1_STATE_PO_STAT_76,             // 0x061600dd, 
  d_io1_STATE_PO_STAT_77,             // 0x061600de, 
  d_io1_STATE_PO_STAT_78,             // 0x061600df, 
  d_io1_STATE_PO_STAT_79,             // 0x061600e0, 
  d_io1_STATE_PO_STAT_80,             // 0x061600e1, 
  d_io1_STATE_PO_STAT_81,             // 0x061600e2, 
  d_io1_STATE_PO_STAT_82,             // 0x061600e3, 
  d_io1_STATE_PO_STAT_83,             // 0x061600e4, 
  d_io1_STATE_PO_STAT_84,             // 0x061600e5, 
  d_io1_STATE_PO_STAT_85,             // 0x061600e6, 
  d_io1_STATE_PO_STAT_86,             // 0x061600e7, 
  d_io1_STATE_PO_STAT_87,             // 0x061600e8, 
  d_io1_STATE_PO_STAT_88,             // 0x061600e9, 
  d_io1_STATE_PO_STAT_89,             // 0x061600ea, 
  d_io1_STATE_PO_STAT_90,             // 0x061600eb, 
  d_io1_STATE_PO_STAT_91,             // 0x061600ec, 
  d_io1_STATE_PO_STAT_92,             // 0x061600ed, 
  d_io1_STATE_PO_STAT_93,             // 0x061600ee, 
  d_io1_STATE_PO_STAT_94,             // 0x061600ef, 
  d_io1_STATE_PO_STAT_95,             // 0x061600f0, 
  d_io1_STATE_PO_STAT_96,             // 0x061600f1, 
  d_io1_STATE_PO_STAT_97,             // 0x061600f2, 
  d_io1_STATE_PO_STAT_98,             // 0x061600f3, 
  d_io1_STATE_PO_STAT_99,             // 0x061600f4, 
  d_io1_STATE_PO_STAT_100,            // 0x061600f5, 
  d_io1_STATE_PO_STAT_101,            // 0x061600f6, 
  d_io1_STATE_PO_STAT_102,            // 0x061600f7, 
  d_io1_STATE_PO_STAT_103,            // 0x061600f8, 
  d_io1_STATE_PO_STAT_104,            // 0x061600f9, 
  d_io1_STATE_PO_STAT_105,            // 0x061600fa, 
  d_io1_STATE_PO_STAT_106,            // 0x061600fb, 
  d_io1_STATE_PO_STAT_107,            // 0x061600fc, 
  d_io1_STATE_PO_STAT_108,            // 0x061600fd, 
  d_io1_STATE_PO_STAT_109,            // 0x061600fe, 
  d_io1_STATE_PO_STAT_110,            // 0x061600ff, 
  d_io1_STATE_PO_STAT_111,            // 0x06160100, 
  d_io1_STATE_PO_STAT_112,            // 0x06160101, 
  d_io1_STATE_PO_STAT_113,            // 0x06160102, 
  d_io1_STATE_PO_STAT_114,            // 0x06160103, 
  d_io1_STATE_PO_STAT_115,            // 0x06160104, 
  d_io1_STATE_PO_STAT_116,            // 0x06160105, 
  d_io1_STATE_PO_STAT_117,            // 0x06160106, 
  d_io1_STATE_PO_STAT_118,            // 0x06160107, 
  d_io1_STATE_PO_STAT_119,            // 0x06160108, 
  d_io1_STATE_PO_STAT_120,            // 0x06160109, 
  d_io1_STATE_PO_STAT_121,            // 0x0616010a, 
  d_io1_STATE_PO_STAT_122,            // 0x0616010b, 
  d_io1_STATE_PO_STAT_123,            // 0x0616010c, 
  d_io1_STATE_PO_STAT_124,            // 0x0616010d, 
  d_io1_STATE_PO_STAT_125,            // 0x0616010e, 
  d_io1_STATE_PO_STAT_126,            // 0x0616010f, 
  d_io1_STATE_PO_STAT_127,            // 0x06160110, 
  d_io1_STATE_PO_STAT_128             // 0x06160111, 
};

enum db_admeter1_MACHSET{
  d_admeter1_MACHSET_ADPOSI_PTMSETLEN_1 = 0x07120000, // 0x07120000, 射出电子尺行程
  d_admeter1_MACHSET_ADPOSI_PTMSETLEN_2,              // 0x07120001, 开关模电子尺行程
  d_admeter1_MACHSET_ADPOSI_PTMSETLEN_3,              // 0x07120002, 托模电子尺行程
  d_admeter1_MACHSET_ADPOSI_PTMSETLEN_4,              // 0x07120003, AD4电子尺行程
  d_admeter1_MACHSET_ADPOSI_PTMSETLEN_5,              // 0x07120004, AD5电子尺行程
  d_admeter1_MACHSET_ADPOSI_PTMSETLEN_6,              // 0x07120005, AD6电子尺行程
  d_admeter1_MACHSET_ADPOSI_PTMSETLEN_7,              // 0x07120006, AD7电子尺行程
  d_admeter1_MACHSET_ADPOSI_PTMSETLEN_8,              // 0x07120007, AD8电子尺行程
  d_admeter1_MACHSET_ADPOSI_MACHINELEN_1,             // 0x07120008, 射出机械行程
  d_admeter1_MACHSET_ADPOSI_MACHINELEN_2,             // 0x07120009, 开关模机械行程
  d_admeter1_MACHSET_ADPOSI_MACHINELEN_3,             // 0x0712000a, 托模机械行程
  d_admeter1_MACHSET_ADPOSI_MACHINELEN_4,             // 0x0712000b, AD4机械行程
  d_admeter1_MACHSET_ADPOSI_MACHINELEN_5,             // 0x0712000c, AD5机械行程
  d_admeter1_MACHSET_ADPOSI_MACHINELEN_6,             // 0x0712000d, AD6机械行程
  d_admeter1_MACHSET_ADPOSI_MACHINELEN_7,             // 0x0712000e, AD7机械行程
  d_admeter1_MACHSET_ADPOSI_MACHINELEN_8,             // 0x0712000f, AD8机械行程
  d_admeter1_MACHSET_ZSET_1,                          // 0x07120010, 位置尺归零台1
  d_admeter1_MACHSET_ZSET_2,                          // 0x07120011, 位置尺归零台2
  d_admeter1_MACHSET_ZSET_3,                          // 0x07120012, 位置尺归零台3
  d_admeter1_MACHSET_ZSET_4,                          // 0x07120013, 位置尺归零台4
  d_admeter1_MACHSET_ZSET_5,                          // 0x07120014, 位置尺归零台5
  d_admeter1_MACHSET_ZSET_6,                          // 0x07120015, 位置尺归零台6
  d_admeter1_MACHSET_ZSET_7,                          // 0x07120016, 位置尺归零台7
  d_admeter1_MACHSET_ZSET_8,                          // 0x07120017, 位置尺归零台8
  d_admeter1_MACHSET_ADPOSI_PTMSETLEN_EXT1,           // 0x07120018, 扩展板位置尺长1
  d_admeter1_MACHSET_ADPOSI_PTMSETLEN_EXT2,           // 0x07120019, 扩展板位置尺长2
  d_admeter1_MACHSET_ADPOSI_PTMSETLEN_EXT3,           // 0x0712001a, 扩展板位置尺长3
  d_admeter1_MACHSET_ADPOSI_PTMSETLEN_EXT4,           // 0x0712001b, 扩展板位置尺长4
  d_admeter1_MACHSET_ADPOSI_PTMSETLEN_EXT5,           // 0x0712001c, 扩展板位置尺长5
  d_admeter1_MACHSET_ADPOSI_PTMSETLEN_EXT6,           // 0x0712001d, 扩展板位置尺长6
  d_admeter1_MACHSET_ADPOSI_PTMSETLEN_EXT7,           // 0x0712001e, 扩展板位置尺长7
  d_admeter1_MACHSET_ADPOSI_PTMSETLEN_EXT8,           // 0x0712001f, 扩展板位置尺长8
  d_admeter1_MACHSET_ADPOSI_MACHINELEN_EXT1,          // 0x07120020, 扩展板机械行程1
  d_admeter1_MACHSET_ADPOSI_MACHINELEN_EXT2,          // 0x07120021, 扩展板机械行程2
  d_admeter1_MACHSET_ADPOSI_MACHINELEN_EXT3,          // 0x07120022, 扩展板机械行程3
  d_admeter1_MACHSET_ADPOSI_MACHINELEN_EXT4,          // 0x07120023, 扩展板机械行程4
  d_admeter1_MACHSET_ADPOSI_MACHINELEN_EXT5,          // 0x07120024, 扩展板机械行程5
  d_admeter1_MACHSET_ADPOSI_MACHINELEN_EXT6,          // 0x07120025, 扩展板机械行程6
  d_admeter1_MACHSET_ADPOSI_MACHINELEN_EXT7,          // 0x07120026, 扩展板机械行程7
  d_admeter1_MACHSET_ADPOSI_MACHINELEN_EXT8,          // 0x07120027, 扩展板机械行程8
  d_admeter1_MACHSET_ZSET_EXT1,                       // 0x07120028, 外扩电子尺位置尺归零台1
  d_admeter1_MACHSET_ZSET_EXT2,                       // 0x07120029, 外扩电子尺位置尺归零台2
  d_admeter1_MACHSET_ZSET_EXT3,                       // 0x0712002a, 外扩电子尺位置尺归零台3
  d_admeter1_MACHSET_ZSET_EXT4,                       // 0x0712002b, 外扩电子尺位置尺归零台4
  d_admeter1_MACHSET_ZSET_EXT5,                       // 0x0712002c, 外扩电子尺位置尺归零台5
  d_admeter1_MACHSET_ZSET_EXT6,                       // 0x0712002d, 外扩电子尺位置尺归零台6
  d_admeter1_MACHSET_ZSET_EXT7,                       // 0x0712002e, 外扩电子尺位置尺归零台7
  d_admeter1_MACHSET_ZSET_EXT8                        // 0x0712002f, 外扩电子尺位置尺归零台8
};

enum db_admeter1_STATE{
  d_admeter1_STATE_FL_ERROR0 = 0x07160000, // 0x07160000, 
  d_admeter1_STATE_FL_ERROR1,              // 0x07160001, 
  d_admeter1_STATE_ADPOSI_RELATIVE_1,      // 0x07160002, 位置尺相对位置0
  d_admeter1_STATE_ADPOSI_RELATIVE_2,      // 0x07160003, 
  d_admeter1_STATE_ADPOSI_RELATIVE_3,      // 0x07160004, 
  d_admeter1_STATE_ADPOSI_RELATIVE_4,      // 0x07160005, 
  d_admeter1_STATE_ADPOSI_RELATIVE_5,      // 0x07160006, 
  d_admeter1_STATE_ADPOSI_RELATIVE_6,      // 0x07160007, 
  d_admeter1_STATE_ADPOSI_RELATIVE_7,      // 0x07160008, 
  d_admeter1_STATE_ADPOSI_RELATIVE_8,      // 0x07160009, 
  d_admeter1_STATE_ADPOSI_ABSOLUTE_1,      // 0x0716000a, 位置尺绝对位置0
  d_admeter1_STATE_ADPOSI_ABSOLUTE_2,      // 0x0716000b, 
  d_admeter1_STATE_ADPOSI_ABSOLUTE_3,      // 0x0716000c, 
  d_admeter1_STATE_ADPOSI_ABSOLUTE_4,      // 0x0716000d, 
  d_admeter1_STATE_ADPOSI_ABSOLUTE_5,      // 0x0716000e, 
  d_admeter1_STATE_ADPOSI_ABSOLUTE_6,      // 0x0716000f, 
  d_admeter1_STATE_ADPOSI_ABSOLUTE_7,      // 0x07160010, 
  d_admeter1_STATE_ADPOSI_ABSOLUTE_8,      // 0x07160011, 
  d_admeter1_STATE_ADPOSI_METERMAX_1,      // 0x07160012, 位置尺使用最大值
  d_admeter1_STATE_ADPOSI_METERMAX_2,      // 0x07160013, 
  d_admeter1_STATE_ADPOSI_METERMAX_3,      // 0x07160014, 
  d_admeter1_STATE_ADPOSI_METERMAX_4,      // 0x07160015, 
  d_admeter1_STATE_ADPOSI_METERMAX_5,      // 0x07160016, 
  d_admeter1_STATE_ADPOSI_METERMAX_6,      // 0x07160017, 
  d_admeter1_STATE_ADPOSI_METERMAX_7,      // 0x07160018, 
  d_admeter1_STATE_ADPOSI_METERMAX_8,      // 0x07160019, 
  d_admeter1_STATE_ADPOSI_METER_1,         // 0x0716001a, 位置尺AD实际采集值
  d_admeter1_STATE_ADPOSI_METER_2,         // 0x0716001b, 
  d_admeter1_STATE_ADPOSI_METER_3,         // 0x0716001c, 
  d_admeter1_STATE_ADPOSI_METER_4,         // 0x0716001d, 
  d_admeter1_STATE_ADPOSI_METER_5,         // 0x0716001e, 
  d_admeter1_STATE_ADPOSI_METER_6,         // 0x0716001f, 
  d_admeter1_STATE_ADPOSI_METER_7,         // 0x07160020, 
  d_admeter1_STATE_ADPOSI_METER_8,         // 0x07160021, 
  d_admeter1_STATE_ADPOSI_RELATIVE_EXT1,   // 0x07160022, 扩展电子尺 位置尺相对位置1
  d_admeter1_STATE_ADPOSI_RELATIVE_EXT2,   // 0x07160023, 扩展电子尺 位置尺相对位置2
  d_admeter1_STATE_ADPOSI_RELATIVE_EXT3,   // 0x07160024, 扩展电子尺 位置尺相对位置3
  d_admeter1_STATE_ADPOSI_RELATIVE_EXT4,   // 0x07160025, 扩展电子尺 位置尺相对位置4
  d_admeter1_STATE_ADPOSI_RELATIVE_EXT5,   // 0x07160026, 扩展电子尺 位置尺相对位置5
  d_admeter1_STATE_ADPOSI_RELATIVE_EXT6,   // 0x07160027, 扩展电子尺 位置尺相对位置6
  d_admeter1_STATE_ADPOSI_RELATIVE_EXT7,   // 0x07160028, 扩展电子尺 位置尺相对位置7
  d_admeter1_STATE_ADPOSI_RELATIVE_EXT8,   // 0x07160029, 扩展电子尺 位置尺相对位置8
  d_admeter1_STATE_ADPOSI_ABSOLUTE_EXT1,   // 0x0716002a, 扩展电子尺位置尺绝对位置1
  d_admeter1_STATE_ADPOSI_ABSOLUTE_EXT2,   // 0x0716002b, 扩展电子尺位置尺绝对位置2
  d_admeter1_STATE_ADPOSI_ABSOLUTE_EXT3,   // 0x0716002c, 扩展电子尺位置尺绝对位置3
  d_admeter1_STATE_ADPOSI_ABSOLUTE_EXT4,   // 0x0716002d, 扩展电子尺位置尺绝对位置4
  d_admeter1_STATE_ADPOSI_ABSOLUTE_EXT5,   // 0x0716002e, 扩展电子尺位置尺绝对位置5
  d_admeter1_STATE_ADPOSI_ABSOLUTE_EXT6,   // 0x0716002f, 扩展电子尺位置尺绝对位置6
  d_admeter1_STATE_ADPOSI_ABSOLUTE_EXT7,   // 0x07160030, 扩展电子尺位置尺绝对位置7
  d_admeter1_STATE_ADPOSI_ABSOLUTE_EXT8,   // 0x07160031, 扩展电子尺位置尺绝对位置8
  d_admeter1_STATE_ADPOSI_METERMAX_EXT1,   // 0x07160032, 扩展电子尺位置尺使用最大值1
  d_admeter1_STATE_ADPOSI_METERMAX_EXT2,   // 0x07160033, 扩展电子尺位置尺使用最大值2
  d_admeter1_STATE_ADPOSI_METERMAX_EXT3,   // 0x07160034, 扩展电子尺位置尺使用最大值3
  d_admeter1_STATE_ADPOSI_METERMAX_EXT4,   // 0x07160035, 扩展电子尺位置尺使用最大值4
  d_admeter1_STATE_ADPOSI_METERMAX_EXT5,   // 0x07160036, 扩展电子尺位置尺使用最大值5
  d_admeter1_STATE_ADPOSI_METERMAX_EXT6,   // 0x07160037, 扩展电子尺位置尺使用最大值6
  d_admeter1_STATE_ADPOSI_METERMAX_EXT7,   // 0x07160038, 扩展电子尺位置尺使用最大值7
  d_admeter1_STATE_ADPOSI_METERMAX_EXT8    // 0x07160039, 扩展电子尺位置尺使用最大值8
};

enum db_da1_MACHSET{
  d_da1_MACHSET_WDA_MAX_1 = 0x08120000, // 0x08120000, 输出最大值0
  d_da1_MACHSET_WDA_MAX_2,              // 0x08120001, 输出最大值1
  d_da1_MACHSET_WDA_MAX_3,              // 0x08120002, 输出最大值2
  d_da1_MACHSET_WDA_MAX_4,              // 0x08120003, 输出最大值3
  d_da1_MACHSET_WDA_MAX_5,              // 0x08120004, 输出最大值4
  d_da1_MACHSET_WDA_MAX_6,              // 0x08120005, 输出最大值5
  d_da1_MACHSET_WDA_MAX_7,              // 0x08120006, 输出最大值6
  d_da1_MACHSET_WDA_MAX_8,              // 0x08120007, 输出最大值7
  d_da1_MACHSET_WDA_MAX_9,              // 0x08120008, 输出最大值8
  d_da1_MACHSET_WDA_MAX_10,             // 0x08120009, 输出最大值9
  d_da1_MACHSET_WDA_MAX_11,             // 0x0812000a, 输出最大值10
  d_da1_MACHSET_WDA_MAX_12,             // 0x0812000b, 输出最大值11
  d_da1_MACHSET_WDA_MAX_13,             // 0x0812000c, 输出最大值12
  d_da1_MACHSET_WDA_MAX_14,             // 0x0812000d, 输出最大值13
  d_da1_MACHSET_WDA_MAX_15,             // 0x0812000e, 输出最大值14
  d_da1_MACHSET_WDA_MAX_16,             // 0x0812000f, 输出最大值15
  d_da1_MACHSET_WDA_ZERO_1,             // 0x08120010, 输出最小值0
  d_da1_MACHSET_WDA_ZERO_2,             // 0x08120011, 输出最小值1
  d_da1_MACHSET_WDA_ZERO_3,             // 0x08120012, 输出最小值2
  d_da1_MACHSET_WDA_ZERO_4,             // 0x08120013, 输出最小值3
  d_da1_MACHSET_WDA_ZERO_5,             // 0x08120014, 输出最小值4
  d_da1_MACHSET_WDA_ZERO_6,             // 0x08120015, 输出最小值5
  d_da1_MACHSET_WDA_ZERO_7,             // 0x08120016, 输出最小值6
  d_da1_MACHSET_WDA_ZERO_8,             // 0x08120017, 输出最小值7
  d_da1_MACHSET_WDA_ZERO_9,             // 0x08120018, 输出最小值8
  d_da1_MACHSET_WDA_ZERO_10,            // 0x08120019, 输出最小值9
  d_da1_MACHSET_WDA_ZERO_11,            // 0x0812001a, 输出最小值10
  d_da1_MACHSET_WDA_ZERO_12,            // 0x0812001b, 输出最小值11
  d_da1_MACHSET_WDA_ZERO_13,            // 0x0812001c, 输出最小值12
  d_da1_MACHSET_WDA_ZERO_14,            // 0x0812001d, 输出最小值13
  d_da1_MACHSET_WDA_ZERO_15,            // 0x0812001e, 输出最小值14
  d_da1_MACHSET_WDA_ZERO_16             // 0x0812001f, 输出最小值15
};

enum db_da1_STATE{
  d_da1_STATE_FL_ERROR0 = 0x08160000, // 0x08160000, 警报1
  d_da1_STATE_FL_ERROR1,              // 0x08160001, 警报2
  d_da1_STATE_TEMPMAX_1,              // 0x08160002, 
  d_da1_STATE_TEMPMAX_2,              // 0x08160003, 
  d_da1_STATE_TEMPMAX_3,              // 0x08160004, 
  d_da1_STATE_TEMPMAX_4,              // 0x08160005, 
  d_da1_STATE_TEMPMAX_5,              // 0x08160006, 
  d_da1_STATE_TEMPMAX_6,              // 0x08160007, 
  d_da1_STATE_TEMPMAX_7,              // 0x08160008, 
  d_da1_STATE_TEMPMAX_8,              // 0x08160009, 
  d_da1_STATE_TEMPMAX_9,              // 0x0816000a, 
  d_da1_STATE_TEMPMAX_10,             // 0x0816000b, 
  d_da1_STATE_TEMPMAX_11,             // 0x0816000c, 
  d_da1_STATE_TEMPMAX_12,             // 0x0816000d, 
  d_da1_STATE_TEMPMAX_13,             // 0x0816000e, 
  d_da1_STATE_TEMPMAX_14,             // 0x0816000f, 
  d_da1_STATE_TEMPMAX_15,             // 0x08160010, 
  d_da1_STATE_TEMPMAX_16,             // 0x08160011, 
  d_da1_STATE_TEMPMIN_1,              // 0x08160012, 
  d_da1_STATE_TEMPMIN_2,              // 0x08160013, 
  d_da1_STATE_TEMPMIN_3,              // 0x08160014, 
  d_da1_STATE_TEMPMIN_4,              // 0x08160015, 
  d_da1_STATE_TEMPMIN_5,              // 0x08160016, 
  d_da1_STATE_TEMPMIN_6,              // 0x08160017, 
  d_da1_STATE_TEMPMIN_7,              // 0x08160018, 
  d_da1_STATE_TEMPMIN_8,              // 0x08160019, 
  d_da1_STATE_TEMPMIN_9,              // 0x0816001a, 
  d_da1_STATE_TEMPMIN_10,             // 0x0816001b, 
  d_da1_STATE_TEMPMIN_11,             // 0x0816001c, 
  d_da1_STATE_TEMPMIN_12,             // 0x0816001d, 
  d_da1_STATE_TEMPMIN_13,             // 0x0816001e, 
  d_da1_STATE_TEMPMIN_14,             // 0x0816001f, 
  d_da1_STATE_TEMPMIN_15,             // 0x08160020, 
  d_da1_STATE_TEMPMIN_16,             // 0x08160021, 
  d_da1_STATE_DA_VAL_1,               // 0x08160022, 
  d_da1_STATE_DA_VAL_2,               // 0x08160023, 
  d_da1_STATE_DA_VAL_3,               // 0x08160024, 
  d_da1_STATE_DA_VAL_4,               // 0x08160025, 
  d_da1_STATE_DA_VAL_5,               // 0x08160026, 
  d_da1_STATE_DA_VAL_6,               // 0x08160027, 
  d_da1_STATE_DA_VAL_7,               // 0x08160028, 
  d_da1_STATE_DA_VAL_8,               // 0x08160029, 
  d_da1_STATE_DA_VAL_9,               // 0x0816002a, 
  d_da1_STATE_DA_VAL_10,              // 0x0816002b, 
  d_da1_STATE_DA_VAL_11,              // 0x0816002c, 
  d_da1_STATE_DA_VAL_12,              // 0x0816002d, 
  d_da1_STATE_DA_VAL_13,              // 0x0816002e, 
  d_da1_STATE_DA_VAL_14,              // 0x0816002f, 
  d_da1_STATE_DA_VAL_15,              // 0x08160030, 
  d_da1_STATE_DA_VAL_16,              // 0x08160031, 
  d_da1_STATE_WDA_STATE,              // 0x08160032, 
  d_da1_STATE_WDA_CHANNEL,            // 0x08160033, 
  d_da1_STATE_WDA_SETDATA,            // 0x08160034, 
  d_da1_STATE_DA_E700B_VAL1,          // 0x08160035, E700B压力流量指令 0-P1
  d_da1_STATE_DA_E700B_VAL2,          // 0x08160036, E700B压力流量指令 1-F1
  d_da1_STATE_DA_E700B_VAL3,          // 0x08160037, E700B压力流量指令 2-P2
  d_da1_STATE_DA_E700B_VAL4,          // 0x08160038, E700B压力流量指令 3-F2
  d_da1_STATE_DA_E700B_DIR1,          // 0x08160039, E700B压力流量方向1
  d_da1_STATE_DA_E700B_DIR2,          // 0x0816003a, E700B压力流量方向2
  d_da1_STATE_DA_E700B_DIR3,          // 0x0816003b, E700B压力流量方向3
  d_da1_STATE_DA_E700B_DIR4,          // 0x0816003c, E700B压力流量方向4
  d_da1_STATE_DA_VOL_VAL1,            // 0x0816003d, 
  d_da1_STATE_DA_VOL_VAL2,            // 0x0816003e, 
  d_da1_STATE_DA_VOL_VAL3,            // 0x0816003f, 
  d_da1_STATE_DA_VOL_VAL4,            // 0x08160040, 
  d_da1_STATE_DA_VOL_VAL5,            // 0x08160041, 
  d_da1_STATE_DA_VOL_VAL6,            // 0x08160042, 
  d_da1_STATE_DA_VOL_VAL7,            // 0x08160043, 
  d_da1_STATE_DA_VOL_VAL8,            // 0x08160044, 
  d_da1_STATE_DA_VOL_VAL9,            // 0x08160045, 
  d_da1_STATE_DA_VOL_VAL10,           // 0x08160046, 
  d_da1_STATE_DA_VOL_VAL11,           // 0x08160047, 
  d_da1_STATE_DA_VOL_VAL12,           // 0x08160048, 
  d_da1_STATE_DA_VOL_VAL13,           // 0x08160049, 
  d_da1_STATE_DA_VOL_VAL14,           // 0x0816004a, 
  d_da1_STATE_DA_VOL_VAL15,           // 0x0816004b, 
  d_da1_STATE_DA_VOL_VAL16            // 0x0816004c, 
};

enum db_hydr1_PO{
  d_hydr1_PO_O_PQVLV = 0x09100000, // 0x09100000, 比例阀开关
  d_hydr1_PO_O_PWRM_P1,            // 0x09100001, 泵1
  d_hydr1_PO_O_PWRM_P2,            // 0x09100002, 泵2
  d_hydr1_PO_O_PWRM_P3,            // 0x09100003, 泵3
  d_hydr1_PO_O_PWRM_P4,            // 0x09100004, 泵4
  d_hydr1_PO_O_PWRM_P5,            // 0x09100005, 泵5
  d_hydr1_PO_O_PWRM_P6,            // 0x09100006, 泵6
  d_hydr1_PO_O_PWRM_P7,            // 0x09100007, 泵7
  d_hydr1_PO_O_PWRM_P8             // 0x09100008, 泵8
};

enum db_hydr1_MACHSET{
  d_hydr1_MACHSET_PRESSENSORCH = 0x09120000, // 0x09120000, 油路液压
  d_hydr1_MACHSET_FL_PWRMATCHTBL,            // 0x09120001, 使用面板PUMP配置表
  d_hydr1_MACHSET_TBL_PWRMATCH1,             // 0x09120002, 速度比例开配置1
  d_hydr1_MACHSET_TBL_PWRMATCH2,             // 0x09120003, 速度比例开配置2
  d_hydr1_MACHSET_TBL_PWRMATCH3,             // 0x09120004, 速度比例开配置3
  d_hydr1_MACHSET_TBL_PWRMATCH4,             // 0x09120005, 速度比例开配置4
  d_hydr1_MACHSET_TBL_PWRMATCH5,             // 0x09120006, 速度比例开配置5
  d_hydr1_MACHSET_TBL_PWRMATCH6,             // 0x09120007, 速度比例开配置6
  d_hydr1_MACHSET_TBL_PWRMATCH7,             // 0x09120008, 速度比例开配置7
  d_hydr1_MACHSET_TBL_PWRMATCH8,             // 0x09120009, 速度比例开配置8
  d_hydr1_MACHSET_TBL_PWRMATCH9,             // 0x0912000a, 速度比例开配置9
  d_hydr1_MACHSET_TBL_PWRMATCH10,            // 0x0912000b, 速度比例开配置10
  d_hydr1_MACHSET_DAFLOW_PWRMATCH1,          // 0x0912000c, 速度比例开1
  d_hydr1_MACHSET_DAFLOW_PWRMATCH2,          // 0x0912000d, 速度比例开2   
  d_hydr1_MACHSET_DAFLOW_PWRMATCH3,          // 0x0912000e, 速度比例开3 
  d_hydr1_MACHSET_DAFLOW_PWRMATCH4,          // 0x0912000f, 速度比例开4
  d_hydr1_MACHSET_DAFLOW_PWRMATCH5,          // 0x09120010, 速度比例开5
  d_hydr1_MACHSET_DAFLOW_PWRMATCH6,          // 0x09120011, 速度比例开6
  d_hydr1_MACHSET_DAFLOW_PWRMATCH7,          // 0x09120012, 速度比例开7
  d_hydr1_MACHSET_DAFLOW_PWRMATCH8,          // 0x09120013, 速度比例开8
  d_hydr1_MACHSET_DAFLOW_PWRMATCH9,          // 0x09120014, 速度比例开9
  d_hydr1_MACHSET_DAFLOW_PWRMATCH10,         // 0x09120015, 速度比例开10
  d_hydr1_MACHSET_TBL_PUMPLMT1,              // 0x09120016, 速度比例开限制1
  d_hydr1_MACHSET_TBL_PUMPLMT2,              // 0x09120017, 速度比例开限制2
  d_hydr1_MACHSET_TBL_PUMPLMT3,              // 0x09120018, 速度比例开限制3
  d_hydr1_MACHSET_TBL_PUMPLMT4,              // 0x09120019, 速度比例开限制4
  d_hydr1_MACHSET_TBL_PUMPLMT5,              // 0x0912001a, 速度比例开限制5
  d_hydr1_MACHSET_TBL_PUMPLMT6,              // 0x0912001b, 速度比例开限制6
  d_hydr1_MACHSET_TBL_PUMPLMT7,              // 0x0912001c, 速度比例开限制7
  d_hydr1_MACHSET_TBL_PUMPLMT8,              // 0x0912001d, 速度比例开限制8
  d_hydr1_MACHSET_TBL_PUMPLMT9,              // 0x0912001e, 速度比例开限制9
  d_hydr1_MACHSET_TBL_PUMPLMT10,             // 0x0912001f, 速度比例开限制10
  d_hydr1_MACHSET_TBL_PUMPLMT11,             // 0x09120020, 速度比例开限制11
  d_hydr1_MACHSET_TBL_PUMPLMT12,             // 0x09120021, 速度比例开限制12
  d_hydr1_MACHSET_TBL_PUMPLMT13,             // 0x09120022, 速度比例开限制13
  d_hydr1_MACHSET_TBL_PUMPLMT14,             // 0x09120023, 速度比例开限制14
  d_hydr1_MACHSET_TBL_PUMPLMT15,             // 0x09120024, 速度比例开限制15
  d_hydr1_MACHSET_TBL_PUMPLMT16,             // 0x09120025, 速度比例开限制16
  d_hydr1_MACHSET_TBL_PUMPLMT17,             // 0x09120026, 速度比例开限制17
  d_hydr1_MACHSET_TBL_PUMPLMT18,             // 0x09120027, 速度比例开限制18
  d_hydr1_MACHSET_TBL_PUMPLMT19,             // 0x09120028, 速度比例开限制19
  d_hydr1_MACHSET_TBL_PUMPLMT20,             // 0x09120029, 速度比例开限制20
  d_hydr1_MACHSET_HY_DELAY,                  // 0x0912002a, 液压延迟
  d_hydr1_MACHSET_MINFLOW_LOWPRES,           // 0x0912002b, 低压最小转速
  d_hydr1_MACHSET_MINFLOW_MEDPRES,           // 0x0912002c, 中压最小转速
  d_hydr1_MACHSET_MINFLOW_HIPRES,            // 0x0912002d, 高压最小转速
  d_hydr1_MACHSET_MAXFLOW_SYSSET,            // 0x0912002e, 设定最大转速
  d_hydr1_MACHSET_CHGFLOW_TESTSET,           // 0x0912002f, 电机转速波动最大值
  d_hydr1_MACHSET_HIFLOW_ADJPARA,            // 0x09120030, 高速射出开环校正系数
  d_hydr1_MACHSET_LOWFLOW_ADJPARA,           // 0x09120031, 低速射出开环校正系数
  d_hydr1_MACHSET_DA_TBL_PUMPOUT,            // 0x09120032, DA校正画面泵浦输出选择
  d_hydr1_MACHSET_PRESSELF_PORT,             // 0x09120033, 自学习通道
  d_hydr1_MACHSET_PRESSELF_PUMP,             // 0x09120034, 自学习Pump表
  d_hydr1_MACHSET_PRESSELF_PRES,             // 0x09120035, 自学习压力
  d_hydr1_MACHSET_PRESSELF_FLOW,             // 0x09120036, 自学习速度
  d_hydr1_MACHSET_PRESSELF_P,                // 0x09120037, 自学习P
  d_hydr1_MACHSET_PRESSELF_I                 // 0x09120038, 自学习I
};

enum db_hydr1_MOLDSET{
  d_hydr1_MOLDSET_FL_FLOWSHOW = 0x09130000, // 0x09130000, 
  d_hydr1_MOLDSET_DACHK_CHG                 // 0x09130001, 速度调整时，输出储料动作
};

enum db_hydr1_ZDSET{
  d_hydr1_ZDSET_DA_PRSOUT0_1 = 0x09140000, // 0x09140000, DA压力速度曲线
  d_hydr1_ZDSET_DA_PRSOUT1_1,              // 0x09140001, 
  d_hydr1_ZDSET_DA_PRSOUT2_1,              // 0x09140002, 
  d_hydr1_ZDSET_DA_PRSOUT3_1,              // 0x09140003, 
  d_hydr1_ZDSET_DA_PRSOUT4_1,              // 0x09140004, 
  d_hydr1_ZDSET_DA_PRSOUT5_1,              // 0x09140005, 
  d_hydr1_ZDSET_DA_PRSOUT6_1,              // 0x09140006, 
  d_hydr1_ZDSET_DA_PRSOUT7_1,              // 0x09140007, 
  d_hydr1_ZDSET_DA_PRSOUT8_1,              // 0x09140008, 
  d_hydr1_ZDSET_DA_PRSOUT9_1,              // 0x09140009, 
  d_hydr1_ZDSET_DA_PRSOUT10_1,             // 0x0914000a, 
  d_hydr1_ZDSET_DA_PRSOUT11_1,             // 0x0914000b, 
  d_hydr1_ZDSET_DA_PRSOUT12_1,             // 0x0914000c, 
  d_hydr1_ZDSET_DA_PRSOUT13_1,             // 0x0914000d, 
  d_hydr1_ZDSET_DA_PRSOUT14_1,             // 0x0914000e, 
  d_hydr1_ZDSET_DA_PRSOUT15_1,             // 0x0914000f, 
  d_hydr1_ZDSET_DA_PRSOUT16_1,             // 0x09140010, 
  d_hydr1_ZDSET_DA_PRSOUT17_1,             // 0x09140011, 
  d_hydr1_ZDSET_DA_PRSOUT18_1,             // 0x09140012, 
  d_hydr1_ZDSET_DA_PRSOUT19_1,             // 0x09140013, 
  d_hydr1_ZDSET_DA_PRSOUT20_1,             // 0x09140014, 
  d_hydr1_ZDSET_DA_PRSOUT21_1,             // 0x09140015, 
  d_hydr1_ZDSET_DA_PRSOUT22_1,             // 0x09140016, 
  d_hydr1_ZDSET_DA_PRSOUT23_1,             // 0x09140017, 
  d_hydr1_ZDSET_DA_PRSOUT24_1,             // 0x09140018, 
  d_hydr1_ZDSET_DA_FLOWOUT0_1,             // 0x09140019, 
  d_hydr1_ZDSET_DA_FLOWOUT1_1,             // 0x0914001a, 
  d_hydr1_ZDSET_DA_FLOWOUT2_1,             // 0x0914001b, 
  d_hydr1_ZDSET_DA_FLOWOUT3_1,             // 0x0914001c, 
  d_hydr1_ZDSET_DA_FLOWOUT4_1,             // 0x0914001d, 
  d_hydr1_ZDSET_DA_FLOWOUT5_1,             // 0x0914001e, 
  d_hydr1_ZDSET_DA_FLOWOUT6_1,             // 0x0914001f, 
  d_hydr1_ZDSET_DA_FLOWOUT7_1,             // 0x09140020, 
  d_hydr1_ZDSET_DA_FLOWOUT8_1,             // 0x09140021, 
  d_hydr1_ZDSET_DA_FLOWOUT9_1,             // 0x09140022, 
  d_hydr1_ZDSET_DA_FLOWOUT10_1,            // 0x09140023, 
  d_hydr1_ZDSET_DA_PRSOUT0_2,              // 0x09140024, 
  d_hydr1_ZDSET_DA_PRSOUT1_2,              // 0x09140025, 
  d_hydr1_ZDSET_DA_PRSOUT2_2,              // 0x09140026, 
  d_hydr1_ZDSET_DA_PRSOUT3_2,              // 0x09140027, 
  d_hydr1_ZDSET_DA_PRSOUT4_2,              // 0x09140028, 
  d_hydr1_ZDSET_DA_PRSOUT5_2,              // 0x09140029, 
  d_hydr1_ZDSET_DA_PRSOUT6_2,              // 0x0914002a, 
  d_hydr1_ZDSET_DA_PRSOUT7_2,              // 0x0914002b, 
  d_hydr1_ZDSET_DA_PRSOUT8_2,              // 0x0914002c, 
  d_hydr1_ZDSET_DA_PRSOUT9_2,              // 0x0914002d, 
  d_hydr1_ZDSET_DA_PRSOUT10_2,             // 0x0914002e, 
  d_hydr1_ZDSET_DA_PRSOUT11_2,             // 0x0914002f, 
  d_hydr1_ZDSET_DA_PRSOUT12_2,             // 0x09140030, 
  d_hydr1_ZDSET_DA_PRSOUT13_2,             // 0x09140031, 
  d_hydr1_ZDSET_DA_PRSOUT14_2,             // 0x09140032, 
  d_hydr1_ZDSET_DA_PRSOUT15_2,             // 0x09140033, 
  d_hydr1_ZDSET_DA_PRSOUT16_2,             // 0x09140034, 
  d_hydr1_ZDSET_DA_PRSOUT17_2,             // 0x09140035, 
  d_hydr1_ZDSET_DA_PRSOUT18_2,             // 0x09140036, 
  d_hydr1_ZDSET_DA_PRSOUT19_2,             // 0x09140037, 
  d_hydr1_ZDSET_DA_PRSOUT20_2,             // 0x09140038, 
  d_hydr1_ZDSET_DA_PRSOUT21_2,             // 0x09140039, 
  d_hydr1_ZDSET_DA_PRSOUT22_2,             // 0x0914003a, 
  d_hydr1_ZDSET_DA_PRSOUT23_2,             // 0x0914003b, 
  d_hydr1_ZDSET_DA_PRSOUT24_2,             // 0x0914003c, 
  d_hydr1_ZDSET_DA_FLOWOUT0_2,             // 0x0914003d, 
  d_hydr1_ZDSET_DA_FLOWOUT1_2,             // 0x0914003e, 
  d_hydr1_ZDSET_DA_FLOWOUT2_2,             // 0x0914003f, 
  d_hydr1_ZDSET_DA_FLOWOUT3_2,             // 0x09140040, 
  d_hydr1_ZDSET_DA_FLOWOUT4_2,             // 0x09140041, 
  d_hydr1_ZDSET_DA_FLOWOUT5_2,             // 0x09140042, 
  d_hydr1_ZDSET_DA_FLOWOUT6_2,             // 0x09140043, 
  d_hydr1_ZDSET_DA_FLOWOUT7_2,             // 0x09140044, 
  d_hydr1_ZDSET_DA_FLOWOUT8_2,             // 0x09140045, 
  d_hydr1_ZDSET_DA_FLOWOUT9_2,             // 0x09140046, 
  d_hydr1_ZDSET_DA_FLOWOUT10_2,            // 0x09140047, 
  d_hydr1_ZDSET_DA_PRSOUT0_3,              // 0x09140048, 
  d_hydr1_ZDSET_DA_PRSOUT1_3,              // 0x09140049, 
  d_hydr1_ZDSET_DA_PRSOUT2_3,              // 0x0914004a, 
  d_hydr1_ZDSET_DA_PRSOUT3_3,              // 0x0914004b, 
  d_hydr1_ZDSET_DA_PRSOUT4_3,              // 0x0914004c, 
  d_hydr1_ZDSET_DA_PRSOUT5_3,              // 0x0914004d, 
  d_hydr1_ZDSET_DA_PRSOUT6_3,              // 0x0914004e, 
  d_hydr1_ZDSET_DA_PRSOUT7_3,              // 0x0914004f, 
  d_hydr1_ZDSET_DA_PRSOUT8_3,              // 0x09140050, 
  d_hydr1_ZDSET_DA_PRSOUT9_3,              // 0x09140051, 
  d_hydr1_ZDSET_DA_PRSOUT10_3,             // 0x09140052, 
  d_hydr1_ZDSET_DA_PRSOUT11_3,             // 0x09140053, 
  d_hydr1_ZDSET_DA_PRSOUT12_3,             // 0x09140054, 
  d_hydr1_ZDSET_DA_PRSOUT13_3,             // 0x09140055, 
  d_hydr1_ZDSET_DA_PRSOUT14_3,             // 0x09140056, 
  d_hydr1_ZDSET_DA_PRSOUT15_3,             // 0x09140057, 
  d_hydr1_ZDSET_DA_PRSOUT16_3,             // 0x09140058, 
  d_hydr1_ZDSET_DA_PRSOUT17_3,             // 0x09140059, 
  d_hydr1_ZDSET_DA_PRSOUT18_3,             // 0x0914005a, 
  d_hydr1_ZDSET_DA_PRSOUT19_3,             // 0x0914005b, 
  d_hydr1_ZDSET_DA_PRSOUT20_3,             // 0x0914005c, 
  d_hydr1_ZDSET_DA_PRSOUT21_3,             // 0x0914005d, 
  d_hydr1_ZDSET_DA_PRSOUT22_3,             // 0x0914005e, 
  d_hydr1_ZDSET_DA_PRSOUT23_3,             // 0x0914005f, 
  d_hydr1_ZDSET_DA_PRSOUT24_3,             // 0x09140060, 
  d_hydr1_ZDSET_DA_FLOWOUT0_3,             // 0x09140061, 
  d_hydr1_ZDSET_DA_FLOWOUT1_3,             // 0x09140062, 
  d_hydr1_ZDSET_DA_FLOWOUT2_3,             // 0x09140063, 
  d_hydr1_ZDSET_DA_FLOWOUT3_3,             // 0x09140064, 
  d_hydr1_ZDSET_DA_FLOWOUT4_3,             // 0x09140065, 
  d_hydr1_ZDSET_DA_FLOWOUT5_3,             // 0x09140066, 
  d_hydr1_ZDSET_DA_FLOWOUT6_3,             // 0x09140067, 
  d_hydr1_ZDSET_DA_FLOWOUT7_3,             // 0x09140068, 
  d_hydr1_ZDSET_DA_FLOWOUT8_3,             // 0x09140069, 
  d_hydr1_ZDSET_DA_FLOWOUT9_3,             // 0x0914006a, 
  d_hydr1_ZDSET_DA_FLOWOUT10_3,            // 0x0914006b, 
  d_hydr1_ZDSET_DA_PRSOUT0_4,              // 0x0914006c, 
  d_hydr1_ZDSET_DA_PRSOUT1_4,              // 0x0914006d, 
  d_hydr1_ZDSET_DA_PRSOUT2_4,              // 0x0914006e, 
  d_hydr1_ZDSET_DA_PRSOUT3_4,              // 0x0914006f, 
  d_hydr1_ZDSET_DA_PRSOUT4_4,              // 0x09140070, 
  d_hydr1_ZDSET_DA_PRSOUT5_4,              // 0x09140071, 
  d_hydr1_ZDSET_DA_PRSOUT6_4,              // 0x09140072, 
  d_hydr1_ZDSET_DA_PRSOUT7_4,              // 0x09140073, 
  d_hydr1_ZDSET_DA_PRSOUT8_4,              // 0x09140074, 
  d_hydr1_ZDSET_DA_PRSOUT9_4,              // 0x09140075, 
  d_hydr1_ZDSET_DA_PRSOUT10_4,             // 0x09140076, 
  d_hydr1_ZDSET_DA_PRSOUT11_4,             // 0x09140077, 
  d_hydr1_ZDSET_DA_PRSOUT12_4,             // 0x09140078, 
  d_hydr1_ZDSET_DA_PRSOUT13_4,             // 0x09140079, 
  d_hydr1_ZDSET_DA_PRSOUT14_4,             // 0x0914007a, 
  d_hydr1_ZDSET_DA_PRSOUT15_4,             // 0x0914007b, 
  d_hydr1_ZDSET_DA_PRSOUT16_4,             // 0x0914007c, 
  d_hydr1_ZDSET_DA_PRSOUT17_4,             // 0x0914007d, 
  d_hydr1_ZDSET_DA_PRSOUT18_4,             // 0x0914007e, 
  d_hydr1_ZDSET_DA_PRSOUT19_4,             // 0x0914007f, 
  d_hydr1_ZDSET_DA_PRSOUT20_4,             // 0x09140080, 
  d_hydr1_ZDSET_DA_PRSOUT21_4,             // 0x09140081, 
  d_hydr1_ZDSET_DA_PRSOUT22_4,             // 0x09140082, 
  d_hydr1_ZDSET_DA_PRSOUT23_4,             // 0x09140083, 
  d_hydr1_ZDSET_DA_PRSOUT24_4,             // 0x09140084, 
  d_hydr1_ZDSET_DA_FLOWOUT0_4,             // 0x09140085, 
  d_hydr1_ZDSET_DA_FLOWOUT1_4,             // 0x09140086, 
  d_hydr1_ZDSET_DA_FLOWOUT2_4,             // 0x09140087, 
  d_hydr1_ZDSET_DA_FLOWOUT3_4,             // 0x09140088, 
  d_hydr1_ZDSET_DA_FLOWOUT4_4,             // 0x09140089, 
  d_hydr1_ZDSET_DA_FLOWOUT5_4,             // 0x0914008a, 
  d_hydr1_ZDSET_DA_FLOWOUT6_4,             // 0x0914008b, 
  d_hydr1_ZDSET_DA_FLOWOUT7_4,             // 0x0914008c, 
  d_hydr1_ZDSET_DA_FLOWOUT8_4,             // 0x0914008d, 
  d_hydr1_ZDSET_DA_FLOWOUT9_4,             // 0x0914008e, 
  d_hydr1_ZDSET_DA_FLOWOUT10_4,            // 0x0914008f, 
  d_hydr1_ZDSET_DASET_PRSOUT0_1,           // 0x09140090, 
  d_hydr1_ZDSET_DASET_PRSOUT1_1,           // 0x09140091, 
  d_hydr1_ZDSET_DASET_PRSOUT2_1,           // 0x09140092, 
  d_hydr1_ZDSET_DASET_PRSOUT3_1,           // 0x09140093, 
  d_hydr1_ZDSET_DASET_PRSOUT4_1,           // 0x09140094, 
  d_hydr1_ZDSET_DASET_PRSOUT5_1,           // 0x09140095, 
  d_hydr1_ZDSET_DASET_PRSOUT6_1,           // 0x09140096, 
  d_hydr1_ZDSET_DASET_PRSOUT7_1,           // 0x09140097, 
  d_hydr1_ZDSET_DASET_PRSOUT8_1,           // 0x09140098, 
  d_hydr1_ZDSET_DASET_PRSOUT9_1,           // 0x09140099, 
  d_hydr1_ZDSET_DASET_PRSOUT10_1,          // 0x0914009a, 
  d_hydr1_ZDSET_DASET_PRSOUT11_1,          // 0x0914009b, 
  d_hydr1_ZDSET_DASET_PRSOUT12_1,          // 0x0914009c, 
  d_hydr1_ZDSET_DASET_PRSOUT13_1,          // 0x0914009d, 
  d_hydr1_ZDSET_DASET_PRSOUT14_1,          // 0x0914009e, 
  d_hydr1_ZDSET_DASET_PRSOUT15_1,          // 0x0914009f, 
  d_hydr1_ZDSET_DASET_PRSOUT16_1,          // 0x091400a0, 
  d_hydr1_ZDSET_DASET_PRSOUT17_1,          // 0x091400a1, 
  d_hydr1_ZDSET_DASET_PRSOUT18_1,          // 0x091400a2, 
  d_hydr1_ZDSET_DASET_PRSOUT19_1,          // 0x091400a3, 
  d_hydr1_ZDSET_DASET_PRSOUT20_1,          // 0x091400a4, 
  d_hydr1_ZDSET_DASET_PRSOUT21_1,          // 0x091400a5, 
  d_hydr1_ZDSET_DASET_PRSOUT22_1,          // 0x091400a6, 
  d_hydr1_ZDSET_DASET_PRSOUT23_1,          // 0x091400a7, 
  d_hydr1_ZDSET_DASET_PRSOUT24_1,          // 0x091400a8, 
  d_hydr1_ZDSET_DASET_FLOWOUT0_1,          // 0x091400a9, 
  d_hydr1_ZDSET_DASET_FLOWOUT1_1,          // 0x091400aa, 
  d_hydr1_ZDSET_DASET_FLOWOUT2_1,          // 0x091400ab, 
  d_hydr1_ZDSET_DASET_FLOWOUT3_1,          // 0x091400ac, 
  d_hydr1_ZDSET_DASET_FLOWOUT4_1,          // 0x091400ad, 
  d_hydr1_ZDSET_DASET_FLOWOUT5_1,          // 0x091400ae, 
  d_hydr1_ZDSET_DASET_FLOWOUT6_1,          // 0x091400af, 
  d_hydr1_ZDSET_DASET_FLOWOUT7_1,          // 0x091400b0, 
  d_hydr1_ZDSET_DASET_FLOWOUT8_1,          // 0x091400b1, 
  d_hydr1_ZDSET_DASET_FLOWOUT9_1,          // 0x091400b2, 
  d_hydr1_ZDSET_DASET_FLOWOUT10_1,         // 0x091400b3, 
  d_hydr1_ZDSET_DASET_PRSOUT0_2,           // 0x091400b4, 
  d_hydr1_ZDSET_DASET_PRSOUT1_2,           // 0x091400b5, 
  d_hydr1_ZDSET_DASET_PRSOUT2_2,           // 0x091400b6, 
  d_hydr1_ZDSET_DASET_PRSOUT3_2,           // 0x091400b7, 
  d_hydr1_ZDSET_DASET_PRSOUT4_2,           // 0x091400b8, 
  d_hydr1_ZDSET_DASET_PRSOUT5_2,           // 0x091400b9, 
  d_hydr1_ZDSET_DASET_PRSOUT6_2,           // 0x091400ba, 
  d_hydr1_ZDSET_DASET_PRSOUT7_2,           // 0x091400bb, 
  d_hydr1_ZDSET_DASET_PRSOUT8_2,           // 0x091400bc, 
  d_hydr1_ZDSET_DASET_PRSOUT9_2,           // 0x091400bd, 
  d_hydr1_ZDSET_DASET_PRSOUT10_2,          // 0x091400be, 
  d_hydr1_ZDSET_DASET_PRSOUT11_2,          // 0x091400bf, 
  d_hydr1_ZDSET_DASET_PRSOUT12_2,          // 0x091400c0, 
  d_hydr1_ZDSET_DASET_PRSOUT13_2,          // 0x091400c1, 
  d_hydr1_ZDSET_DASET_PRSOUT14_2,          // 0x091400c2, 
  d_hydr1_ZDSET_DASET_PRSOUT15_2,          // 0x091400c3, 
  d_hydr1_ZDSET_DASET_PRSOUT16_2,          // 0x091400c4, 
  d_hydr1_ZDSET_DASET_PRSOUT17_2,          // 0x091400c5, 
  d_hydr1_ZDSET_DASET_PRSOUT18_2,          // 0x091400c6, 
  d_hydr1_ZDSET_DASET_PRSOUT19_2,          // 0x091400c7, 
  d_hydr1_ZDSET_DASET_PRSOUT20_2,          // 0x091400c8, 
  d_hydr1_ZDSET_DASET_PRSOUT21_2,          // 0x091400c9, 
  d_hydr1_ZDSET_DASET_PRSOUT22_2,          // 0x091400ca, 
  d_hydr1_ZDSET_DASET_PRSOUT23_2,          // 0x091400cb, 
  d_hydr1_ZDSET_DASET_PRSOUT24_2,          // 0x091400cc, 
  d_hydr1_ZDSET_DASET_FLOWOUT0_2,          // 0x091400cd, 
  d_hydr1_ZDSET_DASET_FLOWOUT1_2,          // 0x091400ce, 
  d_hydr1_ZDSET_DASET_FLOWOUT2_2,          // 0x091400cf, 
  d_hydr1_ZDSET_DASET_FLOWOUT3_2,          // 0x091400d0, 
  d_hydr1_ZDSET_DASET_FLOWOUT4_2,          // 0x091400d1, 
  d_hydr1_ZDSET_DASET_FLOWOUT5_2,          // 0x091400d2, 
  d_hydr1_ZDSET_DASET_FLOWOUT6_2,          // 0x091400d3, 
  d_hydr1_ZDSET_DASET_FLOWOUT7_2,          // 0x091400d4, 
  d_hydr1_ZDSET_DASET_FLOWOUT8_2,          // 0x091400d5, 
  d_hydr1_ZDSET_DASET_FLOWOUT9_2,          // 0x091400d6, 
  d_hydr1_ZDSET_DASET_FLOWOUT10_2,         // 0x091400d7, 
  d_hydr1_ZDSET_DASET_PRSOUT0_3,           // 0x091400d8, 
  d_hydr1_ZDSET_DASET_PRSOUT1_3,           // 0x091400d9, 
  d_hydr1_ZDSET_DASET_PRSOUT2_3,           // 0x091400da, 
  d_hydr1_ZDSET_DASET_PRSOUT3_3,           // 0x091400db, 
  d_hydr1_ZDSET_DASET_PRSOUT4_3,           // 0x091400dc, 
  d_hydr1_ZDSET_DASET_PRSOUT5_3,           // 0x091400dd, 
  d_hydr1_ZDSET_DASET_PRSOUT6_3,           // 0x091400de, 
  d_hydr1_ZDSET_DASET_PRSOUT7_3,           // 0x091400df, 
  d_hydr1_ZDSET_DASET_PRSOUT8_3,           // 0x091400e0, 
  d_hydr1_ZDSET_DASET_PRSOUT9_3,           // 0x091400e1, 
  d_hydr1_ZDSET_DASET_PRSOUT10_3,          // 0x091400e2, 
  d_hydr1_ZDSET_DASET_PRSOUT11_3,          // 0x091400e3, 
  d_hydr1_ZDSET_DASET_PRSOUT12_3,          // 0x091400e4, 
  d_hydr1_ZDSET_DASET_PRSOUT13_3,          // 0x091400e5, 
  d_hydr1_ZDSET_DASET_PRSOUT14_3,          // 0x091400e6, 
  d_hydr1_ZDSET_DASET_PRSOUT15_3,          // 0x091400e7, 
  d_hydr1_ZDSET_DASET_PRSOUT16_3,          // 0x091400e8, 
  d_hydr1_ZDSET_DASET_PRSOUT17_3,          // 0x091400e9, 
  d_hydr1_ZDSET_DASET_PRSOUT18_3,          // 0x091400ea, 
  d_hydr1_ZDSET_DASET_PRSOUT19_3,          // 0x091400eb, 
  d_hydr1_ZDSET_DASET_PRSOUT20_3,          // 0x091400ec, 
  d_hydr1_ZDSET_DASET_PRSOUT21_3,          // 0x091400ed, 
  d_hydr1_ZDSET_DASET_PRSOUT22_3,          // 0x091400ee, 
  d_hydr1_ZDSET_DASET_PRSOUT23_3,          // 0x091400ef, 
  d_hydr1_ZDSET_DASET_PRSOUT24_3,          // 0x091400f0, 
  d_hydr1_ZDSET_DASET_FLOWOUT0_3,          // 0x091400f1, 
  d_hydr1_ZDSET_DASET_FLOWOUT1_3,          // 0x091400f2, 
  d_hydr1_ZDSET_DASET_FLOWOUT2_3,          // 0x091400f3, 
  d_hydr1_ZDSET_DASET_FLOWOUT3_3,          // 0x091400f4, 
  d_hydr1_ZDSET_DASET_FLOWOUT4_3,          // 0x091400f5, 
  d_hydr1_ZDSET_DASET_FLOWOUT5_3,          // 0x091400f6, 
  d_hydr1_ZDSET_DASET_FLOWOUT6_3,          // 0x091400f7, 
  d_hydr1_ZDSET_DASET_FLOWOUT7_3,          // 0x091400f8, 
  d_hydr1_ZDSET_DASET_FLOWOUT8_3,          // 0x091400f9, 
  d_hydr1_ZDSET_DASET_FLOWOUT9_3,          // 0x091400fa, 
  d_hydr1_ZDSET_DASET_FLOWOUT10_3,         // 0x091400fb, 
  d_hydr1_ZDSET_DASET_PRSOUT0_4,           // 0x091400fc, 
  d_hydr1_ZDSET_DASET_PRSOUT1_4,           // 0x091400fd, 
  d_hydr1_ZDSET_DASET_PRSOUT2_4,           // 0x091400fe, 
  d_hydr1_ZDSET_DASET_PRSOUT3_4,           // 0x091400ff, 
  d_hydr1_ZDSET_DASET_PRSOUT4_4,           // 0x09140100, 
  d_hydr1_ZDSET_DASET_PRSOUT5_4,           // 0x09140101, 
  d_hydr1_ZDSET_DASET_PRSOUT6_4,           // 0x09140102, 
  d_hydr1_ZDSET_DASET_PRSOUT7_4,           // 0x09140103, 
  d_hydr1_ZDSET_DASET_PRSOUT8_4,           // 0x09140104, 
  d_hydr1_ZDSET_DASET_PRSOUT9_4,           // 0x09140105, 
  d_hydr1_ZDSET_DASET_PRSOUT10_4,          // 0x09140106, 
  d_hydr1_ZDSET_DASET_PRSOUT11_4,          // 0x09140107, 
  d_hydr1_ZDSET_DASET_PRSOUT12_4,          // 0x09140108, 
  d_hydr1_ZDSET_DASET_PRSOUT13_4,          // 0x09140109, 
  d_hydr1_ZDSET_DASET_PRSOUT14_4,          // 0x0914010a, 
  d_hydr1_ZDSET_DASET_PRSOUT15_4,          // 0x0914010b, 
  d_hydr1_ZDSET_DASET_PRSOUT16_4,          // 0x0914010c, 
  d_hydr1_ZDSET_DASET_PRSOUT17_4,          // 0x0914010d, 
  d_hydr1_ZDSET_DASET_PRSOUT18_4,          // 0x0914010e, 
  d_hydr1_ZDSET_DASET_PRSOUT19_4,          // 0x0914010f, 
  d_hydr1_ZDSET_DASET_PRSOUT20_4,          // 0x09140110, 
  d_hydr1_ZDSET_DASET_PRSOUT21_4,          // 0x09140111, 
  d_hydr1_ZDSET_DASET_PRSOUT22_4,          // 0x09140112, 
  d_hydr1_ZDSET_DASET_PRSOUT23_4,          // 0x09140113, 
  d_hydr1_ZDSET_DASET_PRSOUT24_4,          // 0x09140114, 
  d_hydr1_ZDSET_DASET_FLOWOUT0_4,          // 0x09140115, 
  d_hydr1_ZDSET_DASET_FLOWOUT1_4,          // 0x09140116, 
  d_hydr1_ZDSET_DASET_FLOWOUT2_4,          // 0x09140117, 
  d_hydr1_ZDSET_DASET_FLOWOUT3_4,          // 0x09140118, 
  d_hydr1_ZDSET_DASET_FLOWOUT4_4,          // 0x09140119, 
  d_hydr1_ZDSET_DASET_FLOWOUT5_4,          // 0x0914011a, 
  d_hydr1_ZDSET_DASET_FLOWOUT6_4,          // 0x0914011b, 
  d_hydr1_ZDSET_DASET_FLOWOUT7_4,          // 0x0914011c, 
  d_hydr1_ZDSET_DASET_FLOWOUT8_4,          // 0x0914011d, 
  d_hydr1_ZDSET_DASET_FLOWOUT9_4,          // 0x0914011e, 
  d_hydr1_ZDSET_DASET_FLOWOUT10_4,         // 0x0914011f, 
  d_hydr1_ZDSET_WPRESSLINE0,               // 0x09140120, 第一路压力曲线
  d_hydr1_ZDSET_WPRESSLINE1,               // 0x09140121, 
  d_hydr1_ZDSET_WPRESSLINE2,               // 0x09140122, 
  d_hydr1_ZDSET_WPRESSLINE3,               // 0x09140123, 
  d_hydr1_ZDSET_WPRESSLINE4,               // 0x09140124, 
  d_hydr1_ZDSET_WPRESSLINE5,               // 0x09140125, 
  d_hydr1_ZDSET_WPRESSLINE6,               // 0x09140126, 
  d_hydr1_ZDSET_WPRESSLINE7,               // 0x09140127, 
  d_hydr1_ZDSET_WPRESSLINE8,               // 0x09140128, 
  d_hydr1_ZDSET_WPRESSLINE9,               // 0x09140129, 
  d_hydr1_ZDSET_WPRESSLINE10,              // 0x0914012a, 
  d_hydr1_ZDSET_WPRESSLINE11,              // 0x0914012b, 
  d_hydr1_ZDSET_WPRESSLINE12,              // 0x0914012c, 
  d_hydr1_ZDSET_WPRESSLINE13,              // 0x0914012d, 
  d_hydr1_ZDSET_WPRESSLINE14,              // 0x0914012e, 
  d_hydr1_ZDSET_WPRESSLINE15,              // 0x0914012f, 
  d_hydr1_ZDSET_WPRESSLINE16,              // 0x09140130, 
  d_hydr1_ZDSET_WPRESSLINE17,              // 0x09140131, 
  d_hydr1_ZDSET_WPRESSLINE18,              // 0x09140132, 
  d_hydr1_ZDSET_WPRESSLINE19,              // 0x09140133, 
  d_hydr1_ZDSET_WPRESSLINE20               // 0x09140134, 
};

enum db_hydr1_STATE{
  d_hydr1_STATE_FL_ERROR0 = 0x09160000, // 0x09160000, 警报1
  d_hydr1_STATE_FL_ERROR1,              // 0x09160001, 警报2
  d_hydr1_STATE_DA_OUTPUT0,             // 0x09160002, DA压力输出1
  d_hydr1_STATE_DA_OUTPUT1,             // 0x09160003, DA速度输出1
  d_hydr1_STATE_DA_OUTPUT2,             // 0x09160004, DA压力输出2
  d_hydr1_STATE_DA_OUTPUT3,             // 0x09160005, DA速度输出2
  d_hydr1_STATE_DA_OUTPUT4,             // 0x09160006, DA压力输出3
  d_hydr1_STATE_DA_OUTPUT5,             // 0x09160007, DA速度输出3
  d_hydr1_STATE_DA_OUTPUT6,             // 0x09160008, DA压力输出4
  d_hydr1_STATE_DA_OUTPUT7,             // 0x09160009, DA速度输出4
  d_hydr1_STATE_DA_HYDRAUMAX0,          // 0x0916000a, 比例阀最大值  
  d_hydr1_STATE_DA_HYDRAUMAX1,          // 0x0916000b, 
  d_hydr1_STATE_DA_HYDRAUMAX2,          // 0x0916000c, 
  d_hydr1_STATE_DA_HYDRAUMAX3,          // 0x0916000d, 
  d_hydr1_STATE_DA_HYDRAUMAX4,          // 0x0916000e, 
  d_hydr1_STATE_DA_HYDRAUMAX5,          // 0x0916000f, 
  d_hydr1_STATE_DA_HYDRAUMAX6,          // 0x09160010, 
  d_hydr1_STATE_DA_HYDRAUMAX7,          // 0x09160011, 
  d_hydr1_STATE_DA_TEMPERMAX0           // 0x09160012, 温度最大值 
};

enum db_servo1_PO{
  d_servo1_PO_O_SV_REL = 0x0a100000, // 0x0a100000, 伺服泄压
  d_servo1_PO_O_SV_ERR               // 0x0a100001, 伺服复位
};

enum db_servo1_PI{
  d_servo1_PI_I_SV_ERR = 0x0a110000, // 0x0a110000, 伺服故障
  d_servo1_PI_I_XX_CHK,              // 0x0a110001, 相序检知
  d_servo1_PI_I_PR_ERR,              // 0x0a110002, 外控压力不足
  d_servo1_PI_I_CHG_SV_ERR           // 0x0a110003, 储料伺服故障
};

enum db_servo1_MACHSET{
  d_servo1_MACHSET_KK_PRESSRAMP = 0x0a120000, // 0x0a120000, 压力斜率
  d_servo1_MACHSET_KP_DEFAULT,                // 0x0a120001, 缺省P
  d_servo1_MACHSET_KI_DEFAULT,                // 0x0a120002, 缺省I
  d_servo1_MACHSET_REVERSEFLOWMAX,            // 0x0a120003, 最大反向速度
  d_servo1_MACHSET_DECELERATIONSETP,          // 0x0a120004, 压力上限
  d_servo1_MACHSET_KP_TRANSITION,             // 0x0a120005, PID转折P值
  d_servo1_MACHSET_KI_TRANSITION,             // 0x0a120006, PID转折I值
  d_servo1_MACHSET_INJECTFLOWMIN,             // 0x0a120007, 射出速度下限
  d_servo1_MACHSET_INJECTPRESBYDMAX,          // 0x0a120008, 射出时最大超压范围
  d_servo1_MACHSET_EJECTFLOWMIN,              // 0x0a120009, 托模速度下限
  d_servo1_MACHSET_EJECTPRESBYDMAX,           // 0x0a12000a, 托模压力上限
  d_servo1_MACHSET_KP_INJECTTRANS,            // 0x0a12000b, 射出转换P
  d_servo1_MACHSET_KI_INJECTTRANS,            // 0x0a12000c, 注射转换I
  d_servo1_MACHSET_WSERVO_TC,                 // 0x0a12000d, 伺服采样周期
  d_servo1_MACHSET_WSERVO_KD,                 // 0x0a12000e, 伺服D系数
  d_servo1_MACHSET_WSERVO_KK,                 // 0x0a12000f, 伺服KK值
  d_servo1_MACHSET_WSERVO_K,                  // 0x0a120010, 伺服P
  d_servo1_MACHSET_WSERVO_T,                  // 0x0a120011, 伺服I
  d_servo1_MACHSET_WSERVO_TT,                 // 0x0a120012, 伺服D
  d_servo1_MACHSET_WRITECNT,                  // 0x0a120013, 写次数
  d_servo1_MACHSET_READCNT,                   // 0x0a120014, 读次数
  d_servo1_MACHSET_SRV_PID_TM,                // 0x0a120015, 通讯时间
  d_servo1_MACHSET_MOTOR_MAXSPEED1,           // 0x0a120016, 电机1最大转速
  d_servo1_MACHSET_MOTOR_MAXSPEED2,           // 0x0a120017, 电机2最大转速
  d_servo1_MACHSET_MOTOR_MAXSPEED3,           // 0x0a120018, 电机3最大转速
  d_servo1_MACHSET_MOTOR_MAXSPEED4,           // 0x0a120019, 电机4最大转速
  d_servo1_MACHSET_MOTOR_MAXSPEED5,           // 0x0a12001a, 电机5最大转速
  d_servo1_MACHSET_MOTOR_MAXSPEED6,           // 0x0a12001b, 电机6最大转速
  d_servo1_MACHSET_MOTOR_MAXSPEED7,           // 0x0a12001c, 电机7最大转速
  d_servo1_MACHSET_MOTOR_MAXSPEED8,           // 0x0a12001d, 电机8最大转速
  d_servo1_MACHSET_PUMP_Q1,                   // 0x0a12001e, 油泵1排量
  d_servo1_MACHSET_PUMP_Q2,                   // 0x0a12001f, 油泵2排量
  d_servo1_MACHSET_PUMP_Q3,                   // 0x0a120020, 油泵3排量
  d_servo1_MACHSET_PUMP_Q4,                   // 0x0a120021, 油泵4排量
  d_servo1_MACHSET_PUMP_Q5,                   // 0x0a120022, 油泵5排量
  d_servo1_MACHSET_PUMP_Q6,                   // 0x0a120023, 油泵6排量
  d_servo1_MACHSET_PUMP_Q7,                   // 0x0a120024, 油泵7排量
  d_servo1_MACHSET_PUMP_Q8,                   // 0x0a120025, 油泵8排量
  d_servo1_MACHSET_FL_SERVO_CURVE,            // 0x0a120026, 驱动器FUNC曲线显示
  d_servo1_MACHSET_FL_MGMT_SERVOERR,          // 0x0a120027, 麦格米特伺服故障检测
  d_servo1_MACHSET_PRES_ALPHA_INJECT,         // 0x0a120028, 注射动作超压抑制系数
  d_servo1_MACHSET_PRES_ALPHA_OTHER           // 0x0a120029, 其它动作超压抑制系数
};

enum db_servo1_STATE{
  d_servo1_STATE_FL_ERROR0 = 0x0a160000, // 0x0a160000, 
  d_servo1_STATE_FL_ERROR1,              // 0x0a160001, 
  d_servo1_STATE_SERVO_SPEED_1,          // 0x0a160002, 伺服驱动实际转速
  d_servo1_STATE_SERVO_SPEED_2,          // 0x0a160003, 
  d_servo1_STATE_SERVO_SPEED_3,          // 0x0a160004, 
  d_servo1_STATE_SERVO_SPEED_4,          // 0x0a160005, 
  d_servo1_STATE_SERVO_SPEED_5,          // 0x0a160006, 
  d_servo1_STATE_SERVO_SPEED_6,          // 0x0a160007, 
  d_servo1_STATE_SERVO_SPEED_7,          // 0x0a160008, 
  d_servo1_STATE_SERVO_SPEED_8,          // 0x0a160009, 
  d_servo1_STATE_SERVO_FLOWSET_1,        // 0x0a16000a, 设置到伺服的速度
  d_servo1_STATE_SERVO_FLOWSET_2,        // 0x0a16000b, 
  d_servo1_STATE_SERVO_FLOWSET_3,        // 0x0a16000c, 
  d_servo1_STATE_SERVO_FLOWSET_4,        // 0x0a16000d, 
  d_servo1_STATE_SERVO_FLOWSET_5,        // 0x0a16000e, 
  d_servo1_STATE_SERVO_FLOWSET_6,        // 0x0a16000f, 
  d_servo1_STATE_SERVO_FLOWSET_7,        // 0x0a160010, 
  d_servo1_STATE_SERVO_FLOWSET_8,        // 0x0a160011, 
  d_servo1_STATE_SERVO_CANST_1,          // 0x0a160012, 伺服CAN通信状态
  d_servo1_STATE_SERVO_CANST_2,          // 0x0a160013, 
  d_servo1_STATE_SERVO_CANST_3,          // 0x0a160014, 
  d_servo1_STATE_SERVO_CANST_4,          // 0x0a160015, 
  d_servo1_STATE_SERVO_CANST_5,          // 0x0a160016, 
  d_servo1_STATE_SERVO_CANST_6,          // 0x0a160017, 
  d_servo1_STATE_SERVO_CANST_7,          // 0x0a160018, 
  d_servo1_STATE_SERVO_CANST_8,          // 0x0a160019, 
  d_servo1_STATE_SERVO_ERR_1,            // 0x0a16001a, 伺服警报
  d_servo1_STATE_SERVO_ERR_2,            // 0x0a16001b, 
  d_servo1_STATE_SERVO_ERR_3,            // 0x0a16001c, 
  d_servo1_STATE_SERVO_ERR_4,            // 0x0a16001d, 
  d_servo1_STATE_SERVO_ERR_5,            // 0x0a16001e, 
  d_servo1_STATE_SERVO_ERR_6,            // 0x0a16001f, 
  d_servo1_STATE_SERVO_ERR_7,            // 0x0a160020, 
  d_servo1_STATE_SERVO_ERR_8,            // 0x0a160021, 
  d_servo1_STATE_SERVO_CURRENT_1,        // 0x0a160022, 伺服电流
  d_servo1_STATE_SERVO_CURRENT_2,        // 0x0a160023, 
  d_servo1_STATE_SERVO_CURRENT_3,        // 0x0a160024, 
  d_servo1_STATE_SERVO_CURRENT_4,        // 0x0a160025, 
  d_servo1_STATE_SERVO_CURRENT_5,        // 0x0a160026, 
  d_servo1_STATE_SERVO_CURRENT_6,        // 0x0a160027, 
  d_servo1_STATE_SERVO_CURRENT_7,        // 0x0a160028, 
  d_servo1_STATE_SERVO_CURRENT_8,        // 0x0a160029, 
  d_servo1_STATE_SERVO_POWER_1,          // 0x0a16002a, 伺服功率
  d_servo1_STATE_SERVO_POWER_2,          // 0x0a16002b, 
  d_servo1_STATE_SERVO_POWER_3,          // 0x0a16002c, 
  d_servo1_STATE_SERVO_POWER_4,          // 0x0a16002d, 
  d_servo1_STATE_SERVO_POWER_5,          // 0x0a16002e, 
  d_servo1_STATE_SERVO_POWER_6,          // 0x0a16002f, 
  d_servo1_STATE_SERVO_POWER_7,          // 0x0a160030, 
  d_servo1_STATE_SERVO_POWER_8,          // 0x0a160031, 
  d_servo1_STATE_SERVO_ACTST_1,          // 0x0a160032, 伺服运动状态
  d_servo1_STATE_SERVO_ACTST_2,          // 0x0a160033, 
  d_servo1_STATE_SERVO_ACTST_3,          // 0x0a160034, 
  d_servo1_STATE_SERVO_ACTST_4,          // 0x0a160035, 
  d_servo1_STATE_SERVO_ACTST_5,          // 0x0a160036, 
  d_servo1_STATE_SERVO_ACTST_6,          // 0x0a160037, 
  d_servo1_STATE_SERVO_ACTST_7,          // 0x0a160038, 
  d_servo1_STATE_SERVO_ACTST_8,          // 0x0a160039, 
  d_servo1_STATE_SERVO_TEMP_1,           // 0x0a16003a, 驱动器模块温度
  d_servo1_STATE_SERVO_TEMP_2,           // 0x0a16003b, 
  d_servo1_STATE_SERVO_TEMP_3,           // 0x0a16003c, 
  d_servo1_STATE_SERVO_TEMP_4,           // 0x0a16003d, 
  d_servo1_STATE_SERVO_TEMP_5,           // 0x0a16003e, 
  d_servo1_STATE_SERVO_TEMP_6,           // 0x0a16003f, 
  d_servo1_STATE_SERVO_TEMP_7,           // 0x0a160040, 
  d_servo1_STATE_SERVO_TEMP_8,           // 0x0a160041, 
  d_servo1_STATE_SERVO_PRESS_1,          // 0x0a160042, 驱动器实际压力
  d_servo1_STATE_SERVO_PRESS_2,          // 0x0a160043, 
  d_servo1_STATE_SERVO_PRESS_3,          // 0x0a160044, 
  d_servo1_STATE_SERVO_PRESS_4,          // 0x0a160045, 
  d_servo1_STATE_SERVO_PRESS_5,          // 0x0a160046, 
  d_servo1_STATE_SERVO_PRESS_6,          // 0x0a160047, 
  d_servo1_STATE_SERVO_PRESS_7,          // 0x0a160048, 
  d_servo1_STATE_SERVO_PRESS_8,          // 0x0a160049, 
  d_servo1_STATE_WRITE_CNT_E505,         // 0x0a16004a, E505通讯测试写入次数
  d_servo1_STATE_READ_CNT_E505,          // 0x0a16004b, E505通讯测试读入次数
  d_servo1_STATE_WRITE_CNT_E502_1,       // 0x0a16004c, E502通讯测定写入次数
  d_servo1_STATE_READ_CNT_E502_1,        // 0x0a16004d, E502通讯测试读入次数
  d_servo1_STATE_WRITE_CNT_E502_2,       // 0x0a16004e, E502通讯测定写入次数
  d_servo1_STATE_READ_CNT_E502_2,        // 0x0a16004f, E502通讯测试读入次数
  d_servo1_STATE_MGMT_SRV_SPEED_PERCENT_1, // 0x0a160050, 麦米驱动1转速
  d_servo1_STATE_MGMT_SRV_SPEED_PERCENT_2, // 0x0a160051, 麦米驱动2转速(RPM)
  d_servo1_STATE_MGMT_SRV_SPEED_PERCENT_3, // 0x0a160052, 
  d_servo1_STATE_MGMT_SRV_SPEED_PERCENT_4, // 0x0a160053, 
  d_servo1_STATE_MGMT_SRV_SPEED_PERCENT_5, // 0x0a160054, 
  d_servo1_STATE_MGMT_SRV_SPEED_PERCENT_6, // 0x0a160055, 
  d_servo1_STATE_MGMT_SRV_SPEED_PERCENT_7, // 0x0a160056, 
  d_servo1_STATE_MGMT_SRV_SPEED_PERCENT_8, // 0x0a160057, 
  d_servo1_STATE_MGMT_SRV_ST_1,          // 0x0a160058, 麦米驱动1状态
  d_servo1_STATE_MGMT_SRV_ST_2,          // 0x0a160059, 麦米驱动2状态 0-离线 1-在线
  d_servo1_STATE_MGMT_SRV_ST_3,          // 0x0a16005a, 
  d_servo1_STATE_MGMT_SRV_ST_4,          // 0x0a16005b, 
  d_servo1_STATE_MGMT_SRV_ST_5,          // 0x0a16005c, 
  d_servo1_STATE_MGMT_SRV_ST_6,          // 0x0a16005d, 
  d_servo1_STATE_MGMT_SRV_ST_7,          // 0x0a16005e, 
  d_servo1_STATE_MGMT_SRV_ST_8,          // 0x0a16005f, 
  d_servo1_STATE_MGMT_SRV_PRESS_SET_1,   // 0x0a160060, 麦米驱动1设定压力
  d_servo1_STATE_MGMT_SRV_PRESS_SET_2,   // 0x0a160061, 麦米驱动2设定压力 0.1bar
  d_servo1_STATE_MGMT_SRV_PRESS_SET_3,   // 0x0a160062, 
  d_servo1_STATE_MGMT_SRV_PRESS_SET_4,   // 0x0a160063, 
  d_servo1_STATE_MGMT_SRV_PRESS_SET_5,   // 0x0a160064, 
  d_servo1_STATE_MGMT_SRV_PRESS_SET_6,   // 0x0a160065, 
  d_servo1_STATE_MGMT_SRV_PRESS_SET_7,   // 0x0a160066, 
  d_servo1_STATE_MGMT_SRV_PRESS_SET_8,   // 0x0a160067, 
  d_servo1_STATE_MGMT_SRV_PRESS_REAL_1,  // 0x0a160068, 麦米驱动1反馈压力
  d_servo1_STATE_MGMT_SRV_PRESS_REAL_2,  // 0x0a160069, 麦米驱动2反馈压力 0.1bar
  d_servo1_STATE_MGMT_SRV_PRESS_REAL_3,  // 0x0a16006a, 
  d_servo1_STATE_MGMT_SRV_PRESS_REAL_4,  // 0x0a16006b, 
  d_servo1_STATE_MGMT_SRV_PRESS_REAL_5,  // 0x0a16006c, 
  d_servo1_STATE_MGMT_SRV_PRESS_REAL_6,  // 0x0a16006d, 
  d_servo1_STATE_MGMT_SRV_PRESS_REAL_7,  // 0x0a16006e, 
  d_servo1_STATE_MGMT_SRV_PRESS_REAL_8,  // 0x0a16006f, 
  d_servo1_STATE_MGMT_SRV_SPEED_SET_LOW_1, // 0x0a160070, 麦米驱动1设定转速
  d_servo1_STATE_MGMT_SRV_SPEED_SET_LOW_2, // 0x0a160071, 麦米驱动2设定转速(主机计算值)rpm
  d_servo1_STATE_MGMT_SRV_SPEED_SET_LOW_3, // 0x0a160072, 
  d_servo1_STATE_MGMT_SRV_SPEED_SET_LOW_4, // 0x0a160073, 
  d_servo1_STATE_MGMT_SRV_SPEED_SET_LOW_5, // 0x0a160074, 
  d_servo1_STATE_MGMT_SRV_SPEED_SET_LOW_6, // 0x0a160075, 
  d_servo1_STATE_MGMT_SRV_SPEED_SET_LOW_7, // 0x0a160076, 
  d_servo1_STATE_MGMT_SRV_SPEED_SET_LOW_8, // 0x0a160077, 
  d_servo1_STATE_MGMT_SRV_SPEED_SET_HI_1, // 0x0a160078, 麦米驱动1暂未使用
  d_servo1_STATE_MGMT_SRV_SPEED_SET_HI_2, // 0x0a160079, 麦米驱动2暂未使用
  d_servo1_STATE_MGMT_SRV_SPEED_SET_HI_3, // 0x0a16007a, 
  d_servo1_STATE_MGMT_SRV_SPEED_SET_HI_4, // 0x0a16007b, 
  d_servo1_STATE_MGMT_SRV_SPEED_SET_HI_5, // 0x0a16007c, 
  d_servo1_STATE_MGMT_SRV_SPEED_SET_HI_6, // 0x0a16007d, 
  d_servo1_STATE_MGMT_SRV_SPEED_SET_HI_7, // 0x0a16007e, 
  d_servo1_STATE_MGMT_SRV_SPEED_SET_HI_8, // 0x0a16007f, 
  d_servo1_STATE_MGMT_SRV_ERR_ST_1,      // 0x0a160080, 麦米驱动1故障码
  d_servo1_STATE_MGMT_SRV_ERR_ST_2,      // 0x0a160081, 麦米驱动2故障码
  d_servo1_STATE_MGMT_SRV_ERR_ST_3,      // 0x0a160082, 
  d_servo1_STATE_MGMT_SRV_ERR_ST_4,      // 0x0a160083, 
  d_servo1_STATE_MGMT_SRV_ERR_ST_5,      // 0x0a160084, 
  d_servo1_STATE_MGMT_SRV_ERR_ST_6,      // 0x0a160085, 
  d_servo1_STATE_MGMT_SRV_ERR_ST_7,      // 0x0a160086, 
  d_servo1_STATE_MGMT_SRV_ERR_ST_8,      // 0x0a160087, 
  d_servo1_STATE_MGMT_SRV_SPEED_DIRECT_1, // 0x0a160088, 麦米驱动1转速方向
  d_servo1_STATE_MGMT_SRV_SPEED_DIRECT_2, // 0x0a160089, 麦米驱动2转速方向 0-正 1-负
  d_servo1_STATE_MGMT_SRV_SPEED_DIRECT_3, // 0x0a16008a, 
  d_servo1_STATE_MGMT_SRV_SPEED_DIRECT_4, // 0x0a16008b, 
  d_servo1_STATE_MGMT_SRV_SPEED_DIRECT_5, // 0x0a16008c, 
  d_servo1_STATE_MGMT_SRV_SPEED_DIRECT_6, // 0x0a16008d, 
  d_servo1_STATE_MGMT_SRV_SPEED_DIRECT_7, // 0x0a16008e, 
  d_servo1_STATE_MGMT_SRV_SPEED_DIRECT_8, // 0x0a16008f, 
  d_servo1_STATE_MGMT_SRV_SPEED_MAX_1,   // 0x0a160090, 麦米驱动1最大转速
  d_servo1_STATE_MGMT_SRV_SPEED_MAX_2,   // 0x0a160091, 麦米驱动2最大转速
  d_servo1_STATE_MGMT_SRV_SPEED_MAX_3,   // 0x0a160092, 
  d_servo1_STATE_MGMT_SRV_SPEED_MAX_4,   // 0x0a160093, 
  d_servo1_STATE_MGMT_SRV_SPEED_MAX_5,   // 0x0a160094, 
  d_servo1_STATE_MGMT_SRV_SPEED_MAX_6,   // 0x0a160095, 
  d_servo1_STATE_MGMT_SRV_SPEED_MAX_7,   // 0x0a160096, 
  d_servo1_STATE_MGMT_SRV_SPEED_MAX_8    // 0x0a160097, 
};

enum db_alarm1_PO{
  d_alarm1_PO_O_ALARM = 0x0b100000, // 0x0b100000, 警报
  d_alarm1_PO_O_FLASH,              // 0x0b100001, 闪光
  d_alarm1_PO_O_OK_LIT,             // 0x0b100002, 正常灯
  d_alarm1_PO_O_ERR_LIT,            // 0x0b100003, 不正常灯
  d_alarm1_PO_O_EMG_LIT,            // 0x0b100004, 紧急停止灯
  d_alarm1_PO_O_AUT_LIT,            // 0x0b100005, 自动灯
  d_alarm1_PO_O_MAN_LIT,            // 0x0b100006, 手动灯
  d_alarm1_PO_O_ALM_SRV             // 0x0b100007, 报警备用
};

enum db_alarm1_PI{
  d_alarm1_PI_I_EMG_EXT = 0x0b110000, // 0x0b110000, 外部急停键
  d_alarm1_PI_I_KEY_RUN,              // 0x0b110001, 外部运行键
  d_alarm1_PI_I_MANUAL1,              // 0x0b110002, 外部手动键1
  d_alarm1_PI_I_MANUAL2,              // 0x0b110003, 外部手动键2
  d_alarm1_PI_I_MANUAL3,              // 0x0b110004, 外部手动键3
  d_alarm1_PI_I_MANUAL4               // 0x0b110005, 外部手动键4
};

enum db_alarm1_MACHSET{
  d_alarm1_MACHSET_CN_BUZZERACT = 0x0b120000, // 0x0b120000, 警报次数到关马达
  d_alarm1_MACHSET_TM_HEATEROFFIFALARM,       // 0x0b120001, 警报时间到关电热
  d_alarm1_MACHSET_FL_ALARM,                  // 0x0b120002, 警报功能
  d_alarm1_MACHSET_FL_AlARMONMANL,            // 0x0b120003, 手动状态下开蜂鸣器
  d_alarm1_MACHSET_FL_ALARMCNT                // 0x0b120004, 临时警报次数
};

enum db_alarm1_STATE{
  d_alarm1_STATE_FL_ERROR0 = 0x0b160000, // 0x0b160000, 警报1
  d_alarm1_STATE_FL_ERROR1,              // 0x0b160001, 警报2
  d_alarm1_STATE_AUTOALARM_SHORTNUM      // 0x0b160002, 自动警报开启剩余模数
};

enum db_oil1_PO{
  d_oil1_PO_O_OILHEAT1 = 0x0c100000, // 0x0c100000, 油温1
  d_oil1_PO_O_OILCOOL1,              // 0x0c100001, 油温2
  d_oil1_PO_O_OIL_RSV                // 0x0c100002, 油温保留
};

enum db_oil1_PI{
  d_oil1_PI_I_FIL_BLK = 0x0c110000, // 0x0c110000, 滤油网检知
  d_oil1_PI_I_OIL_LVL,              // 0x0c110001, 油压油位检知
  d_oil1_PI_I_OIL_RSV               // 0x0c110002, 油温输入保留
};

enum db_oil1_MACHSET{
  d_oil1_MACHSET_OIL_TEMPCH = 0x0c120000, // 0x0c120000, 油温温度通道
  d_oil1_MACHSET_FL_OILWARM,              // 0x0c120001, 油温预热控制
  d_oil1_MACHSET_DATEMP_OILMIN,           // 0x0c120002, 油温下限
  d_oil1_MACHSET_DATEMP_OILMAX,           // 0x0c120003, 油温上限
  d_oil1_MACHSET_MIN_PRESS,               // 0x0c120004, 最小系统压力
  d_oil1_MACHSET_TM_PROTECT,              // 0x0c120005, 系统压力检测时间
  d_oil1_MACHSET_FL_CHKOIL,               // 0x0c120006, 漏油检测功能
  d_oil1_MACHSET_FL_TEMP_OIL,             // 0x0c120007, 油温功能
  d_oil1_MACHSET_DATEMP_COOLERONFOROIL,   // 0x0c120008, 油温冷却开启温度
  d_oil1_MACHSET_DATEMP_COOLEROFFFOROIL   // 0x0c120009, 油温冷却关闭温度
};

enum db_oil1_STATE{
  d_oil1_STATE_FL_ERROR0 = 0x0c160000, // 0x0c160000, 警报1
  d_oil1_STATE_FL_ERROR1,              // 0x0c160001, 警报2
  d_oil1_STATE_DATEMP_OIL              // 0x0c160002, 
};

enum db_ECat1_STATE{
  d_ECat1_STATE_FL_ERROR0 = 0x0d160000, // 0x0d160000, 警报1
  d_ECat1_STATE_FL_ERROR1,              // 0x0d160001, 警报2
  d_ECat1_STATE_ETHERCATCOMDATASUM,     // 0x0d160002, 驱动器传输数据总数
  d_ECat1_STATE_ETHERCATCOMDATANUM      // 0x0d160003, 驱动器传输当前量
};

enum db_TempCtrlComm1_PO{
  d_TempCtrlComm1_PO_O_COOLER = 0x0e100000, // 0x0e100000, 冷却水
  d_TempCtrlComm1_PO_O_COOL_1,              // 0x0e100001, 冷却器1
  d_TempCtrlComm1_PO_O_COOL_2,              // 0x0e100002, 冷却器2
  d_TempCtrlComm1_PO_O_COOL_3,              // 0x0e100003, 冷却器3
  d_TempCtrlComm1_PO_O_COOL_4,              // 0x0e100004, 冷却器4
  d_TempCtrlComm1_PO_O_COOL_5,              // 0x0e100005, 冷却器5
  d_TempCtrlComm1_PO_O_COOL_6,              // 0x0e100006, 冷却器6
  d_TempCtrlComm1_PO_O_COOL_7,              // 0x0e100007, 冷却器7
  d_TempCtrlComm1_PO_O_COOL_8,              // 0x0e100008, 冷却器8
  d_TempCtrlComm1_PO_O_COOL_9,              // 0x0e100009, 冷却器9
  d_TempCtrlComm1_PO_O_COOL_10,             // 0x0e10000a, 冷却器10
  d_TempCtrlComm1_PO_O_COOL_11,             // 0x0e10000b, 冷却器11
  d_TempCtrlComm1_PO_O_COOL_12,             // 0x0e10000c, 冷却器12
  d_TempCtrlComm1_PO_O_COOL_13,             // 0x0e10000d, 冷却器13
  d_TempCtrlComm1_PO_O_COOL_14,             // 0x0e10000e, 冷却器14
  d_TempCtrlComm1_PO_O_COOL_15,             // 0x0e10000f, 冷却器15
  d_TempCtrlComm1_PO_O_COOL_16,             // 0x0e100010, 冷却器16
  d_TempCtrlComm1_PO_O_TEMOP_1,             // 0x0e100011, 电热1
  d_TempCtrlComm1_PO_O_TEMOP_2,             // 0x0e100012, 电热2
  d_TempCtrlComm1_PO_O_TEMOP_3,             // 0x0e100013, 电热3
  d_TempCtrlComm1_PO_O_TEMOP_4,             // 0x0e100014, 电热4
  d_TempCtrlComm1_PO_O_TEMOP_5,             // 0x0e100015, 电热5
  d_TempCtrlComm1_PO_O_TEMOP_6,             // 0x0e100016, 电热6
  d_TempCtrlComm1_PO_O_TEMOP_7,             // 0x0e100017, 电热7
  d_TempCtrlComm1_PO_O_TEMOP_8,             // 0x0e100018, 电热8
  d_TempCtrlComm1_PO_O_TEMOP_9,             // 0x0e100019, 电热9
  d_TempCtrlComm1_PO_O_TEMOP_10,            // 0x0e10001a, 电热10
  d_TempCtrlComm1_PO_O_TEMOP_11,            // 0x0e10001b, 电热11
  d_TempCtrlComm1_PO_O_TEMOP_12,            // 0x0e10001c, 电热12
  d_TempCtrlComm1_PO_O_TEMOP_13,            // 0x0e10001d, 电热13
  d_TempCtrlComm1_PO_O_TEMOP_14,            // 0x0e10001e, 电热14
  d_TempCtrlComm1_PO_O_TEMOP_15,            // 0x0e10001f, 电热15
  d_TempCtrlComm1_PO_O_TEMOP_16,            // 0x0e100020, 电热16
  d_TempCtrlComm1_PO_O_DROP_COL,            // 0x0e100021, 落料口冷却水阀
  d_TempCtrlComm1_PO_O_HEATER               // 0x0e100022, 电热开
};

enum db_TempCtrlComm1_PI{
  d_TempCtrlComm1_PI_I_HEATTOUCH = 0x0e110000  // 0x0e110000, 加热接触器
};

enum db_TempCtrlComm1_MACHSET{
  d_TempCtrlComm1_MACHSET_DATEMP_FUNC_1 = 0x0e120000, // 0x0e120000, 温度#1温控功能
  d_TempCtrlComm1_MACHSET_DATEMP_FUNC_2,              // 0x0e120001, 温度#2温控功能
  d_TempCtrlComm1_MACHSET_DATEMP_FUNC_3,              // 0x0e120002, 温度#3温控功能
  d_TempCtrlComm1_MACHSET_DATEMP_FUNC_4,              // 0x0e120003, 温度#4温控功能
  d_TempCtrlComm1_MACHSET_DATEMP_FUNC_5,              // 0x0e120004, 温度#5温控功能
  d_TempCtrlComm1_MACHSET_DATEMP_FUNC_6,              // 0x0e120005, 温度#6温控功能
  d_TempCtrlComm1_MACHSET_DATEMP_FUNC_7,              // 0x0e120006, 温度#7温控功能
  d_TempCtrlComm1_MACHSET_DATEMP_FUNC_8,              // 0x0e120007, 温度#8温控功能
  d_TempCtrlComm1_MACHSET_DATEMP_FUNC_9,              // 0x0e120008, 温度#9温控功能
  d_TempCtrlComm1_MACHSET_DATEMP_FUNC_10,             // 0x0e120009, 温度#10温控功能
  d_TempCtrlComm1_MACHSET_DATEMP_FUNC_11,             // 0x0e12000a, 温度#11温控功能
  d_TempCtrlComm1_MACHSET_DATEMP_FUNC_12,             // 0x0e12000b, 温度#12温控功能
  d_TempCtrlComm1_MACHSET_DATEMP_FUNC_13,             // 0x0e12000c, 温度#13温控功能
  d_TempCtrlComm1_MACHSET_DATEMP_FUNC_14,             // 0x0e12000d, 温度#14温控功能
  d_TempCtrlComm1_MACHSET_DATEMP_FUNC_15,             // 0x0e12000e, 温度#15温控功能
  d_TempCtrlComm1_MACHSET_DATEMP_FUNC_16,             // 0x0e12000f, 温度#16温控功能
  d_TempCtrlComm1_MACHSET_DATEMP_RAMP_1,              // 0x0e120010, 温控#1段缓冲区
  d_TempCtrlComm1_MACHSET_DATEMP_RAMP_2,              // 0x0e120011, 温控#2段缓冲区
  d_TempCtrlComm1_MACHSET_DATEMP_RAMP_3,              // 0x0e120012, 温控#3段缓冲区
  d_TempCtrlComm1_MACHSET_DATEMP_RAMP_4,              // 0x0e120013, 温控#4段缓冲区
  d_TempCtrlComm1_MACHSET_DATEMP_RAMP_5,              // 0x0e120014, 温控#5段缓冲区
  d_TempCtrlComm1_MACHSET_DATEMP_RAMP_6,              // 0x0e120015, 温控#6段缓冲区
  d_TempCtrlComm1_MACHSET_DATEMP_RAMP_7,              // 0x0e120016, 温控#7段缓冲区
  d_TempCtrlComm1_MACHSET_DATEMP_RAMP_8,              // 0x0e120017, 温控#8段缓冲区
  d_TempCtrlComm1_MACHSET_DATEMP_RAMP_9,              // 0x0e120018, 温控#9段缓冲区
  d_TempCtrlComm1_MACHSET_DATEMP_RAMP_10,             // 0x0e120019, 温控#10段缓冲区
  d_TempCtrlComm1_MACHSET_DATEMP_RAMP_11,             // 0x0e12001a, 温控#11段缓冲区
  d_TempCtrlComm1_MACHSET_DATEMP_RAMP_12,             // 0x0e12001b, 温控#12段缓冲区
  d_TempCtrlComm1_MACHSET_DATEMP_RAMP_13,             // 0x0e12001c, 温控#13段缓冲区
  d_TempCtrlComm1_MACHSET_DATEMP_RAMP_14,             // 0x0e12001d, 温控#14段缓冲区
  d_TempCtrlComm1_MACHSET_DATEMP_RAMP_15,             // 0x0e12001e, 温控#15段缓冲区
  d_TempCtrlComm1_MACHSET_DATEMP_RAMP_16,             // 0x0e12001f, 温控#16段缓冲区
  d_TempCtrlComm1_MACHSET_DAPID_KP_1,                 // 0x0e120020, 温度P系数#1
  d_TempCtrlComm1_MACHSET_DAPID_KP_2,                 // 0x0e120021, 温度P系数#2
  d_TempCtrlComm1_MACHSET_DAPID_KP_3,                 // 0x0e120022, 温度P系数#3
  d_TempCtrlComm1_MACHSET_DAPID_KP_4,                 // 0x0e120023, 温度P系数#4
  d_TempCtrlComm1_MACHSET_DAPID_KP_5,                 // 0x0e120024, 温度P系数#5
  d_TempCtrlComm1_MACHSET_DAPID_KP_6,                 // 0x0e120025, 温度P系数#6
  d_TempCtrlComm1_MACHSET_DAPID_KP_7,                 // 0x0e120026, 温度P系数#7
  d_TempCtrlComm1_MACHSET_DAPID_KP_8,                 // 0x0e120027, 温度P系数#8
  d_TempCtrlComm1_MACHSET_DAPID_KP_9,                 // 0x0e120028, 温度P系数#9
  d_TempCtrlComm1_MACHSET_DAPID_KP_10,                // 0x0e120029, 温度P系数#10
  d_TempCtrlComm1_MACHSET_DAPID_KP_11,                // 0x0e12002a, 温度P系数#11
  d_TempCtrlComm1_MACHSET_DAPID_KP_12,                // 0x0e12002b, 温度P系数#12
  d_TempCtrlComm1_MACHSET_DAPID_KP_13,                // 0x0e12002c, 温度P系数#13
  d_TempCtrlComm1_MACHSET_DAPID_KP_14,                // 0x0e12002d, 温度P系数#14
  d_TempCtrlComm1_MACHSET_DAPID_KP_15,                // 0x0e12002e, 温度P系数#15
  d_TempCtrlComm1_MACHSET_DAPID_KP_16,                // 0x0e12002f, 温度P系数#16
  d_TempCtrlComm1_MACHSET_DAPID_TI_1,                 // 0x0e120030, 温度I系数#1
  d_TempCtrlComm1_MACHSET_DAPID_TI_2,                 // 0x0e120031, 温度I系数#2
  d_TempCtrlComm1_MACHSET_DAPID_TI_3,                 // 0x0e120032, 温度I系数#3
  d_TempCtrlComm1_MACHSET_DAPID_TI_4,                 // 0x0e120033, 温度I系数#4
  d_TempCtrlComm1_MACHSET_DAPID_TI_5,                 // 0x0e120034, 温度I系数#5
  d_TempCtrlComm1_MACHSET_DAPID_TI_6,                 // 0x0e120035, 温度I系数#6
  d_TempCtrlComm1_MACHSET_DAPID_TI_7,                 // 0x0e120036, 温度I系数#7
  d_TempCtrlComm1_MACHSET_DAPID_TI_8,                 // 0x0e120037, 温度I系数#8
  d_TempCtrlComm1_MACHSET_DAPID_TI_9,                 // 0x0e120038, 温度I系数#9
  d_TempCtrlComm1_MACHSET_DAPID_TI_10,                // 0x0e120039, 温度I系数#10
  d_TempCtrlComm1_MACHSET_DAPID_TI_11,                // 0x0e12003a, 温度I系数#11
  d_TempCtrlComm1_MACHSET_DAPID_TI_12,                // 0x0e12003b, 温度I系数#12
  d_TempCtrlComm1_MACHSET_DAPID_TI_13,                // 0x0e12003c, 温度I系数#13
  d_TempCtrlComm1_MACHSET_DAPID_TI_14,                // 0x0e12003d, 温度I系数#14
  d_TempCtrlComm1_MACHSET_DAPID_TI_15,                // 0x0e12003e, 温度I系数#15
  d_TempCtrlComm1_MACHSET_DAPID_TI_16,                // 0x0e12003f, 温度I系数#16
  d_TempCtrlComm1_MACHSET_DAPID_TD_1,                 // 0x0e120040, 温度D系数#1
  d_TempCtrlComm1_MACHSET_DAPID_TD_2,                 // 0x0e120041, 温度D系数#2
  d_TempCtrlComm1_MACHSET_DAPID_TD_3,                 // 0x0e120042, 温度D系数#3
  d_TempCtrlComm1_MACHSET_DAPID_TD_4,                 // 0x0e120043, 温度D系数#4
  d_TempCtrlComm1_MACHSET_DAPID_TD_5,                 // 0x0e120044, 温度D系数#5
  d_TempCtrlComm1_MACHSET_DAPID_TD_6,                 // 0x0e120045, 温度D系数#6
  d_TempCtrlComm1_MACHSET_DAPID_TD_7,                 // 0x0e120046, 温度D系数#7
  d_TempCtrlComm1_MACHSET_DAPID_TD_8,                 // 0x0e120047, 温度D系数#8
  d_TempCtrlComm1_MACHSET_DAPID_TD_9,                 // 0x0e120048, 温度D系数#9
  d_TempCtrlComm1_MACHSET_DAPID_TD_10,                // 0x0e120049, 温度D系数#10
  d_TempCtrlComm1_MACHSET_DAPID_TD_11,                // 0x0e12004a, 温度D系数#11
  d_TempCtrlComm1_MACHSET_DAPID_TD_12,                // 0x0e12004b, 温度D系数#12
  d_TempCtrlComm1_MACHSET_DAPID_TD_13,                // 0x0e12004c, 温度D系数#13
  d_TempCtrlComm1_MACHSET_DAPID_TD_14,                // 0x0e12004d, 温度D系数#14
  d_TempCtrlComm1_MACHSET_DAPID_TD_15,                // 0x0e12004e, 温度D系数#15
  d_TempCtrlComm1_MACHSET_DAPID_TD_16,                // 0x0e12004f, 温度D系数#16
  d_TempCtrlComm1_MACHSET_TM_RESPONSELESSCYCLE_1,     // 0x0e120050, 温度#1输出周期时间
  d_TempCtrlComm1_MACHSET_TM_RESPONSELESSCYCLE_2,     // 0x0e120051, 温度#2输出周期时间
  d_TempCtrlComm1_MACHSET_TM_RESPONSELESSCYCLE_3,     // 0x0e120052, 温度#3输出周期时间
  d_TempCtrlComm1_MACHSET_TM_RESPONSELESSCYCLE_4,     // 0x0e120053, 温度#4输出周期时间
  d_TempCtrlComm1_MACHSET_TM_RESPONSELESSCYCLE_5,     // 0x0e120054, 温度#5输出周期时间
  d_TempCtrlComm1_MACHSET_TM_RESPONSELESSCYCLE_6,     // 0x0e120055, 温度#6输出周期时间
  d_TempCtrlComm1_MACHSET_TM_RESPONSELESSCYCLE_7,     // 0x0e120056, 温度#7输出周期时间
  d_TempCtrlComm1_MACHSET_TM_RESPONSELESSCYCLE_8,     // 0x0e120057, 温度#8输出周期时间
  d_TempCtrlComm1_MACHSET_TM_RESPONSELESSCYCLE_9,     // 0x0e120058, 温度#9输出周期时间
  d_TempCtrlComm1_MACHSET_TM_RESPONSELESSCYCLE_10,    // 0x0e120059, 温度#10输出周期时间
  d_TempCtrlComm1_MACHSET_TM_RESPONSELESSCYCLE_11,    // 0x0e12005a, 温度#11输出周期时间
  d_TempCtrlComm1_MACHSET_TM_RESPONSELESSCYCLE_12,    // 0x0e12005b, 温度#12输出周期时间
  d_TempCtrlComm1_MACHSET_TM_RESPONSELESSCYCLE_13,    // 0x0e12005c, 温度#13输出周期时间
  d_TempCtrlComm1_MACHSET_TM_RESPONSELESSCYCLE_14,    // 0x0e12005d, 温度#14输出周期时间
  d_TempCtrlComm1_MACHSET_TM_RESPONSELESSCYCLE_15,    // 0x0e12005e, 温度#15输出周期时间
  d_TempCtrlComm1_MACHSET_TM_RESPONSELESSCYCLE_16,    // 0x0e12005f, 温度#16输出周期时间
  d_TempCtrlComm1_MACHSET_DATEMP_SCTM_MIN,            // 0x0e120060, 最小扫描周期
  d_TempCtrlComm1_MACHSET_FL_TEMPWORDUPCHECK,         // 0x0e120061, 电热连续加温检查
  d_TempCtrlComm1_MACHSET_DATEMP_TEMPCOOLERON,        // 0x0e120062, 料管冷却开启上偏差
  d_TempCtrlComm1_MACHSET_DATEMP_TEMPCOOLEROFF,       // 0x0e120063, 料管冷却关闭下偏差
  d_TempCtrlComm1_MACHSET_TEMPCOOLCTRL,               // 0x0e120064, 落料口冷却功能
  d_TempCtrlComm1_MACHSET_TEMPCOOLMAX,                // 0x0e120065, 落料口温度上限
  d_TempCtrlComm1_MACHSET_TEMPCOOLCN,                 // 0x0e120066, 落料口温度段
  d_TempCtrlComm1_MACHSET_DATEMP_TEMPCOOLERONMOUTH,   // 0x0e120067, 落料口冷却开
  d_TempCtrlComm1_MACHSET_DATEMP_TEMPCOOLEROFFMOUTH,  // 0x0e120068, 落料口冷却关
  d_TempCtrlComm1_MACHSET_FL_TKTJ_1,                  // 0x0e120069, 热电偶类型1
  d_TempCtrlComm1_MACHSET_FL_TKTJ_2,                  // 0x0e12006a, 热电偶类型2
  d_TempCtrlComm1_MACHSET_FL_TKTJ_3,                  // 0x0e12006b, 热电偶类型3
  d_TempCtrlComm1_MACHSET_FL_TKTJ_4,                  // 0x0e12006c, 热电偶类型4
  d_TempCtrlComm1_MACHSET_FL_TKTJ_5,                  // 0x0e12006d, 热电偶类型5
  d_TempCtrlComm1_MACHSET_FL_TKTJ_6,                  // 0x0e12006e, 热电偶类型6
  d_TempCtrlComm1_MACHSET_FL_TKTJ_7,                  // 0x0e12006f, 热电偶类型7
  d_TempCtrlComm1_MACHSET_FL_TKTJ_8,                  // 0x0e120070, 热电偶类型8
  d_TempCtrlComm1_MACHSET_FL_TKTJ_9,                  // 0x0e120071, 热电偶类型9
  d_TempCtrlComm1_MACHSET_FL_TKTJ_10,                 // 0x0e120072, 热电偶类型10
  d_TempCtrlComm1_MACHSET_FL_TKTJ_11,                 // 0x0e120073, 热电偶类型11
  d_TempCtrlComm1_MACHSET_FL_TKTJ_12,                 // 0x0e120074, 热电偶类型12
  d_TempCtrlComm1_MACHSET_FL_TKTJ_13,                 // 0x0e120075, 热电偶类型13
  d_TempCtrlComm1_MACHSET_FL_TKTJ_14,                 // 0x0e120076, 热电偶类型14
  d_TempCtrlComm1_MACHSET_FL_TKTJ_15,                 // 0x0e120077, 热电偶类型15
  d_TempCtrlComm1_MACHSET_FL_TKTJ_16,                 // 0x0e120078, 热电偶类型16
  d_TempCtrlComm1_MACHSET_DATEMP_CH_NUM,              // 0x0e120079, 温度段数
  d_TempCtrlComm1_MACHSET_TEMP_PROTECT,               // 0x0e12007a, 温度保护上限
  d_TempCtrlComm1_MACHSET_FL_FREETIME_WARMOFF,        // 0x0e12007b, 动作闲置电热处理
  d_TempCtrlComm1_MACHSET_TM_FREETIME_WARMOFF,        // 0x0e12007c, 电热闲置时间
  d_TempCtrlComm1_MACHSET_DA_THERMOCOUPLE_PRT,        // 0x0e12007d, 电热连续加温检查度数
  d_TempCtrlComm1_MACHSET_TM_TEMPWORDUPCHECK          // 0x0e12007e, 电热连续加温检查时间
};

enum db_TempCtrlComm1_MOLDSET{
  d_TempCtrlComm1_MOLDSET_DATEMP_SETTING_1 = 0x0e130000, // 0x0e130000, 温度1段设定值
  d_TempCtrlComm1_MOLDSET_DATEMP_SETTING_2,              // 0x0e130001, 温度2段设定值
  d_TempCtrlComm1_MOLDSET_DATEMP_SETTING_3,              // 0x0e130002, 温度3段设定值
  d_TempCtrlComm1_MOLDSET_DATEMP_SETTING_4,              // 0x0e130003, 温度4段设定值
  d_TempCtrlComm1_MOLDSET_DATEMP_SETTING_5,              // 0x0e130004, 温度5段设定值
  d_TempCtrlComm1_MOLDSET_DATEMP_SETTING_6,              // 0x0e130005, 温度6段设定值
  d_TempCtrlComm1_MOLDSET_DATEMP_SETTING_7,              // 0x0e130006, 温度7段设定值
  d_TempCtrlComm1_MOLDSET_DATEMP_SETTING_8,              // 0x0e130007, 温度8段设定值
  d_TempCtrlComm1_MOLDSET_DATEMP_SETTING_9,              // 0x0e130008, 温度9段设定值
  d_TempCtrlComm1_MOLDSET_DATEMP_SETTING_10,             // 0x0e130009, 温度10段设定值
  d_TempCtrlComm1_MOLDSET_DATEMP_SETTING_11,             // 0x0e13000a, 温度11段设定值
  d_TempCtrlComm1_MOLDSET_DATEMP_SETTING_12,             // 0x0e13000b, 温度12段设定值
  d_TempCtrlComm1_MOLDSET_DATEMP_SETTING_13,             // 0x0e13000c, 温度13段设定值
  d_TempCtrlComm1_MOLDSET_DATEMP_SETTING_14,             // 0x0e13000d, 温度14段设定值
  d_TempCtrlComm1_MOLDSET_DATEMP_SETTING_15,             // 0x0e13000e, 温度15段设定值
  d_TempCtrlComm1_MOLDSET_DATEMP_SETTING_16,             // 0x0e13000f, 温度16段设定值
  d_TempCtrlComm1_MOLDSET_DATEMP_MAX_1,                  // 0x0e130010, 温控#1段上偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MAX_2,                  // 0x0e130011, 温控#2段上偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MAX_3,                  // 0x0e130012, 温控#3段上偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MAX_4,                  // 0x0e130013, 温控#4段上偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MAX_5,                  // 0x0e130014, 温控#5段上偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MAX_6,                  // 0x0e130015, 温控#6段上偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MAX_7,                  // 0x0e130016, 温控#7段上偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MAX_8,                  // 0x0e130017, 温控#8段上偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MAX_9,                  // 0x0e130018, 温控#9段上偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MAX_10,                 // 0x0e130019, 温控#10段上偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MAX_11,                 // 0x0e13001a, 温控#11段上偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MAX_12,                 // 0x0e13001b, 温控#12段上偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MAX_13,                 // 0x0e13001c, 温控#13段上偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MAX_14,                 // 0x0e13001d, 温控#14段上偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MAX_15,                 // 0x0e13001e, 温控#15段上偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MAX_16,                 // 0x0e13001f, 温控#16段上偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MIN_1,                  // 0x0e130020, 温控#1段下偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MIN_2,                  // 0x0e130021, 温控#2段下偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MIN_3,                  // 0x0e130022, 温控#3段下偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MIN_4,                  // 0x0e130023, 温控#4段下偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MIN_5,                  // 0x0e130024, 温控#5段下偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MIN_6,                  // 0x0e130025, 温控#6段下偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MIN_7,                  // 0x0e130026, 温控#7段下偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MIN_8,                  // 0x0e130027, 温控#8段下偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MIN_9,                  // 0x0e130028, 温控#9段下偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MIN_10,                 // 0x0e130029, 温控#10段下偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MIN_11,                 // 0x0e13002a, 温控#11段下偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MIN_12,                 // 0x0e13002b, 温控#12段下偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MIN_13,                 // 0x0e13002c, 温控#13段下偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MIN_14,                 // 0x0e13002d, 温控#14段下偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MIN_15,                 // 0x0e13002e, 温控#15段下偏差
  d_TempCtrlComm1_MOLDSET_DATEMP_MIN_16,                 // 0x0e13002f, 温控#16段下偏差
  d_TempCtrlComm1_MOLDSET_DACENT_HEATERON_1,             // 0x0e130030, 温度#1手动加热百分比
  d_TempCtrlComm1_MOLDSET_DACENT_HEATERON_2,             // 0x0e130031, 温度#2手动加热百分比
  d_TempCtrlComm1_MOLDSET_DACENT_HEATERON_3,             // 0x0e130032, 温度#3手动加热百分比
  d_TempCtrlComm1_MOLDSET_DACENT_HEATERON_4,             // 0x0e130033, 温度#4手动加热百分比
  d_TempCtrlComm1_MOLDSET_DACENT_HEATERON_5,             // 0x0e130034, 温度#5手动加热百分比
  d_TempCtrlComm1_MOLDSET_DACENT_HEATERON_6,             // 0x0e130035, 温度#6手动加热百分比
  d_TempCtrlComm1_MOLDSET_DACENT_HEATERON_7,             // 0x0e130036, 温度#7手动加热百分比
  d_TempCtrlComm1_MOLDSET_DACENT_HEATERON_8,             // 0x0e130037, 温度#8手动加热百分比
  d_TempCtrlComm1_MOLDSET_DACENT_HEATERON_9,             // 0x0e130038, 温度#9手动加热百分比
  d_TempCtrlComm1_MOLDSET_DACENT_HEATERON_10,            // 0x0e130039, 温度#10手动加热百分比
  d_TempCtrlComm1_MOLDSET_DACENT_HEATERON_11,            // 0x0e13003a, 温度#11手动加热百分比
  d_TempCtrlComm1_MOLDSET_DACENT_HEATERON_12,            // 0x0e13003b, 温度#12手动加热百分比
  d_TempCtrlComm1_MOLDSET_DACENT_HEATERON_13,            // 0x0e13003c, 温度#13手动加热百分比
  d_TempCtrlComm1_MOLDSET_DACENT_HEATERON_14,            // 0x0e13003d, 温度#14手动加热百分比
  d_TempCtrlComm1_MOLDSET_DACENT_HEATERON_15,            // 0x0e13003e, 温度#15手动加热百分比
  d_TempCtrlComm1_MOLDSET_DACENT_HEATERON_16,            // 0x0e13003f, 温度#16手动加热百分比
  d_TempCtrlComm1_MOLDSET_TM_HEATERWARM,                 // 0x0e130040, 预温计时
  d_TempCtrlComm1_MOLDSET_FL_TEMPHOLD,                   // 0x0e130041, 保温功能选择
  d_TempCtrlComm1_MOLDSET_DATEMP_HOLD,                   // 0x0e130042, 保温温度
  d_TempCtrlComm1_MOLDSET_FL_TEMPCOOL,                   // 0x0e130043, 料管冷却器使用
  d_TempCtrlComm1_MOLDSET_CONFUNC                        // 0x0e130044, 同步升温功能
};

enum db_TempCtrlComm1_ZDSET{
  d_TempCtrlComm1_ZDSET_DATEMP_RAMP1 = 0x0e140000, // 0x0e140000, 温度加温缓冲区
  d_TempCtrlComm1_ZDSET_DATEMP_RAMP2,              // 0x0e140001, 
  d_TempCtrlComm1_ZDSET_DATEMP_RAMP3,              // 0x0e140002, 
  d_TempCtrlComm1_ZDSET_DATEMP_RAMP4,              // 0x0e140003, 
  d_TempCtrlComm1_ZDSET_DATEMP_RAMP5,              // 0x0e140004, 
  d_TempCtrlComm1_ZDSET_DATEMP_RAMP6,              // 0x0e140005, 
  d_TempCtrlComm1_ZDSET_DATEMP_RAMP7,              // 0x0e140006, 
  d_TempCtrlComm1_ZDSET_DATEMP_RAMP8,              // 0x0e140007, 
  d_TempCtrlComm1_ZDSET_DATEMP_RAMP9,              // 0x0e140008, 
  d_TempCtrlComm1_ZDSET_DATEMP_RAMP10,             // 0x0e140009, 
  d_TempCtrlComm1_ZDSET_DATEMP_RAMP11,             // 0x0e14000a, 
  d_TempCtrlComm1_ZDSET_DATEMP_RAMP12,             // 0x0e14000b, 
  d_TempCtrlComm1_ZDSET_DATEMP_RAMP13,             // 0x0e14000c, 
  d_TempCtrlComm1_ZDSET_DATEMP_RAMP14,             // 0x0e14000d, 
  d_TempCtrlComm1_ZDSET_DATEMP_RAMP15,             // 0x0e14000e, 
  d_TempCtrlComm1_ZDSET_DATEMP_RAMP16,             // 0x0e14000f, 
  d_TempCtrlComm1_ZDSET_DAPID_KP1,                 // 0x0e140010, 温度P系数
  d_TempCtrlComm1_ZDSET_DAPID_KP2,                 // 0x0e140011, 
  d_TempCtrlComm1_ZDSET_DAPID_KP3,                 // 0x0e140012, 
  d_TempCtrlComm1_ZDSET_DAPID_KP4,                 // 0x0e140013, 
  d_TempCtrlComm1_ZDSET_DAPID_KP5,                 // 0x0e140014, 
  d_TempCtrlComm1_ZDSET_DAPID_KP6,                 // 0x0e140015, 
  d_TempCtrlComm1_ZDSET_DAPID_KP7,                 // 0x0e140016, 
  d_TempCtrlComm1_ZDSET_DAPID_KP8,                 // 0x0e140017, 
  d_TempCtrlComm1_ZDSET_DAPID_KP9,                 // 0x0e140018, 
  d_TempCtrlComm1_ZDSET_DAPID_KP10,                // 0x0e140019, 
  d_TempCtrlComm1_ZDSET_DAPID_KP11,                // 0x0e14001a, 
  d_TempCtrlComm1_ZDSET_DAPID_KP12,                // 0x0e14001b, 
  d_TempCtrlComm1_ZDSET_DAPID_KP13,                // 0x0e14001c, 
  d_TempCtrlComm1_ZDSET_DAPID_KP14,                // 0x0e14001d, 
  d_TempCtrlComm1_ZDSET_DAPID_KP15,                // 0x0e14001e, 
  d_TempCtrlComm1_ZDSET_DAPID_KP16,                // 0x0e14001f, 
  d_TempCtrlComm1_ZDSET_DAPID_TI1,                 // 0x0e140020, 温度I系数
  d_TempCtrlComm1_ZDSET_DAPID_TI2,                 // 0x0e140021, 
  d_TempCtrlComm1_ZDSET_DAPID_TI3,                 // 0x0e140022, 
  d_TempCtrlComm1_ZDSET_DAPID_TI4,                 // 0x0e140023, 
  d_TempCtrlComm1_ZDSET_DAPID_TI5,                 // 0x0e140024, 
  d_TempCtrlComm1_ZDSET_DAPID_TI6,                 // 0x0e140025, 
  d_TempCtrlComm1_ZDSET_DAPID_TI7,                 // 0x0e140026, 
  d_TempCtrlComm1_ZDSET_DAPID_TI8,                 // 0x0e140027, 
  d_TempCtrlComm1_ZDSET_DAPID_TI9,                 // 0x0e140028, 
  d_TempCtrlComm1_ZDSET_DAPID_TI10,                // 0x0e140029, 
  d_TempCtrlComm1_ZDSET_DAPID_TI11,                // 0x0e14002a, 
  d_TempCtrlComm1_ZDSET_DAPID_TI12,                // 0x0e14002b, 
  d_TempCtrlComm1_ZDSET_DAPID_TI13,                // 0x0e14002c, 
  d_TempCtrlComm1_ZDSET_DAPID_TI14,                // 0x0e14002d, 
  d_TempCtrlComm1_ZDSET_DAPID_TI15,                // 0x0e14002e, 
  d_TempCtrlComm1_ZDSET_DAPID_TI16,                // 0x0e14002f, 
  d_TempCtrlComm1_ZDSET_DAPID_TD1,                 // 0x0e140030, 温度D系数
  d_TempCtrlComm1_ZDSET_DAPID_TD2,                 // 0x0e140031, 
  d_TempCtrlComm1_ZDSET_DAPID_TD3,                 // 0x0e140032, 
  d_TempCtrlComm1_ZDSET_DAPID_TD4,                 // 0x0e140033, 
  d_TempCtrlComm1_ZDSET_DAPID_TD5,                 // 0x0e140034, 
  d_TempCtrlComm1_ZDSET_DAPID_TD6,                 // 0x0e140035, 
  d_TempCtrlComm1_ZDSET_DAPID_TD7,                 // 0x0e140036, 
  d_TempCtrlComm1_ZDSET_DAPID_TD8,                 // 0x0e140037, 
  d_TempCtrlComm1_ZDSET_DAPID_TD9,                 // 0x0e140038, 
  d_TempCtrlComm1_ZDSET_DAPID_TD10,                // 0x0e140039, 
  d_TempCtrlComm1_ZDSET_DAPID_TD11,                // 0x0e14003a, 
  d_TempCtrlComm1_ZDSET_DAPID_TD12,                // 0x0e14003b, 
  d_TempCtrlComm1_ZDSET_DAPID_TD13,                // 0x0e14003c, 
  d_TempCtrlComm1_ZDSET_DAPID_TD14,                // 0x0e14003d, 
  d_TempCtrlComm1_ZDSET_DAPID_TD15,                // 0x0e14003e, 
  d_TempCtrlComm1_ZDSET_DAPID_TD16                 // 0x0e14003f, 
};

enum db_TempCtrlComm1_STATE{
  d_TempCtrlComm1_STATE_FL_ERROR0 = 0x0e160000, // 0x0e160000, 警报1
  d_TempCtrlComm1_STATE_FL_ERROR1,              // 0x0e160001, 警报2
  d_TempCtrlComm1_STATE_DATEMP_REAL_1,          // 0x0e160002, 温度实际值1
  d_TempCtrlComm1_STATE_DATEMP_REAL_2,          // 0x0e160003, 温度实际值2
  d_TempCtrlComm1_STATE_DATEMP_REAL_3,          // 0x0e160004, 温度实际值3
  d_TempCtrlComm1_STATE_DATEMP_REAL_4,          // 0x0e160005, 温度实际值4
  d_TempCtrlComm1_STATE_DATEMP_REAL_5,          // 0x0e160006, 温度实际值5
  d_TempCtrlComm1_STATE_DATEMP_REAL_6,          // 0x0e160007, 温度实际值6
  d_TempCtrlComm1_STATE_DATEMP_REAL_7,          // 0x0e160008, 温度实际值7
  d_TempCtrlComm1_STATE_DATEMP_REAL_8,          // 0x0e160009, 温度实际值8
  d_TempCtrlComm1_STATE_DATEMP_REAL_9,          // 0x0e16000a, 温度实际值9
  d_TempCtrlComm1_STATE_DATEMP_REAL_10,         // 0x0e16000b, 温度实际值10
  d_TempCtrlComm1_STATE_DATEMP_REAL_11,         // 0x0e16000c, 温度实际值11
  d_TempCtrlComm1_STATE_DATEMP_REAL_12,         // 0x0e16000d, 温度实际值12
  d_TempCtrlComm1_STATE_DATEMP_REAL_13,         // 0x0e16000e, 温度实际值13
  d_TempCtrlComm1_STATE_DATEMP_REAL_14,         // 0x0e16000f, 温度实际值14
  d_TempCtrlComm1_STATE_DATEMP_REAL_15,         // 0x0e160010, 温度实际值15
  d_TempCtrlComm1_STATE_DATEMP_REAL_16,         // 0x0e160011, 温度实际值16
  d_TempCtrlComm1_STATE_FL_TEMPSTATUS_1,        // 0x0e160012, 温度加温状态1
  d_TempCtrlComm1_STATE_FL_TEMPSTATUS_2,        // 0x0e160013, 温度加温状态2
  d_TempCtrlComm1_STATE_FL_TEMPSTATUS_3,        // 0x0e160014, 温度加温状态3
  d_TempCtrlComm1_STATE_FL_TEMPSTATUS_4,        // 0x0e160015, 温度加温状态4
  d_TempCtrlComm1_STATE_FL_TEMPSTATUS_5,        // 0x0e160016, 温度加温状态5
  d_TempCtrlComm1_STATE_FL_TEMPSTATUS_6,        // 0x0e160017, 温度加温状态6
  d_TempCtrlComm1_STATE_FL_TEMPSTATUS_7,        // 0x0e160018, 温度加温状态7
  d_TempCtrlComm1_STATE_FL_TEMPSTATUS_8,        // 0x0e160019, 温度加温状态8
  d_TempCtrlComm1_STATE_FL_TEMPSTATUS_9,        // 0x0e16001a, 温度加温状态9
  d_TempCtrlComm1_STATE_FL_TEMPSTATUS_10,       // 0x0e16001b, 温度加温状态10
  d_TempCtrlComm1_STATE_FL_TEMPSTATUS_11,       // 0x0e16001c, 温度加温状态11
  d_TempCtrlComm1_STATE_FL_TEMPSTATUS_12,       // 0x0e16001d, 温度加温状态12
  d_TempCtrlComm1_STATE_FL_TEMPSTATUS_13,       // 0x0e16001e, 温度加温状态13
  d_TempCtrlComm1_STATE_FL_TEMPSTATUS_14,       // 0x0e16001f, 温度加温状态14
  d_TempCtrlComm1_STATE_FL_TEMPSTATUS_15,       // 0x0e160020, 温度加温状态15
  d_TempCtrlComm1_STATE_FL_TEMPSTATUS_16,       // 0x0e160021, 温度加温状态16
  d_TempCtrlComm1_STATE_OUTPUT_PER_1,           // 0x0e160022, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm1_STATE_OUTPUT_PER_2,           // 0x0e160023, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm1_STATE_OUTPUT_PER_3,           // 0x0e160024, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm1_STATE_OUTPUT_PER_4,           // 0x0e160025, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm1_STATE_OUTPUT_PER_5,           // 0x0e160026, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm1_STATE_OUTPUT_PER_6,           // 0x0e160027, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm1_STATE_OUTPUT_PER_7,           // 0x0e160028, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm1_STATE_OUTPUT_PER_8,           // 0x0e160029, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm1_STATE_OUTPUT_PER_9,           // 0x0e16002a, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm1_STATE_OUTPUT_PER_10,          // 0x0e16002b, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm1_STATE_OUTPUT_PER_11,          // 0x0e16002c, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm1_STATE_OUTPUT_PER_12,          // 0x0e16002d, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm1_STATE_OUTPUT_PER_13,          // 0x0e16002e, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm1_STATE_OUTPUT_PER_14,          // 0x0e16002f, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm1_STATE_OUTPUT_PER_15,          // 0x0e160030, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm1_STATE_OUTPUT_PER_16,          // 0x0e160031, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm1_STATE_WTEMP_LM73,             // 0x0e160032, 室温LM73的温度
  d_TempCtrlComm1_STATE_WTEMP_RELAT,            // 0x0e160033, 校准后的实时温度
  d_TempCtrlComm1_STATE_WTEMP_ADVALUE,          // 0x0e160034, 采样得到的实时值
  d_TempCtrlComm1_STATE_WTEMP_CHANNEL,          // 0x0e160035, 调试温度通道值
  d_TempCtrlComm1_STATE_WTEMP_ADJALL,           // 0x0e160036, 温度多通道联调标志
  d_TempCtrlComm1_STATE_WTEMP_ADJAMB,           // 0x0e160037, 室温校正
  d_TempCtrlComm1_STATE_FL_HEATERSTATUS,        // 0x0e160038, 电热状态
  d_TempCtrlComm1_STATE_WSTATERSV6,             // 0x0e160039, 保温倒计时
  d_TempCtrlComm1_STATE_DATEMP_CHARGEPOS,       // 0x0e16003a, 落料口温度
  d_TempCtrlComm1_STATE_DATEMP_YOUHUAXUQIU,     // 0x0e16003b, 温度优化提示开电热的闪烁指示
  d_TempCtrlComm1_STATE_DATEMP_YOUHUAOK,        // 0x0e16003c, 温度优化开启与完成标记
  d_TempCtrlComm1_STATE_DATEMP_E502_VER,        // 0x0e16003d, 温度扩展板E502版本号	20190410
  d_TempCtrlComm1_STATE_WRITE_CNT_E502,         // 0x0e16003e, 通讯测定写入次数
  d_TempCtrlComm1_STATE_READ_CNT_E502,          // 0x0e16003f, 通讯测试读入次数
  d_TempCtrlComm1_STATE_GETTEMPONTM_60s_1,      // 0x0e160040, 60s内电热输出时间
  d_TempCtrlComm1_STATE_GETTEMPONTM_60s_2,      // 0x0e160041, 60s内电热输出时间
  d_TempCtrlComm1_STATE_GETTEMPONTM_60s_3,      // 0x0e160042, 60s内电热输出时间
  d_TempCtrlComm1_STATE_GETTEMPONTM_60s_4,      // 0x0e160043, 60s内电热输出时间
  d_TempCtrlComm1_STATE_GETTEMPONTM_60s_5,      // 0x0e160044, 60s内电热输出时间
  d_TempCtrlComm1_STATE_GETTEMPONTM_60s_6,      // 0x0e160045, 60s内电热输出时间
  d_TempCtrlComm1_STATE_GETTEMPONTM_60s_7,      // 0x0e160046, 60s内电热输出时间
  d_TempCtrlComm1_STATE_GETTEMPONTM_60s_8,      // 0x0e160047, 60s内电热输出时间
  d_TempCtrlComm1_STATE_GETTEMPONTM_60s_9,      // 0x0e160048, 60s内电热输出时间
  d_TempCtrlComm1_STATE_GETTEMPONTM_60s_10,     // 0x0e160049, 60s内电热输出时间
  d_TempCtrlComm1_STATE_GETTEMPONTM_60s_11,     // 0x0e16004a, 60s内电热输出时间
  d_TempCtrlComm1_STATE_GETTEMPONTM_60s_12,     // 0x0e16004b, 60s内电热输出时间
  d_TempCtrlComm1_STATE_GETTEMPONTM_60s_13,     // 0x0e16004c, 60s内电热输出时间
  d_TempCtrlComm1_STATE_GETTEMPONTM_60s_14,     // 0x0e16004d, 60s内电热输出时间
  d_TempCtrlComm1_STATE_GETTEMPONTM_60s_15,     // 0x0e16004e, 60s内电热输出时间
  d_TempCtrlComm1_STATE_GETTEMPONTM_60s_16,     // 0x0e16004f, 60s内电热输出时间
  d_TempCtrlComm1_STATE_DATEMP_VALUE_1,         // 0x0e160050, 热电偶读取实际AD值
  d_TempCtrlComm1_STATE_DATEMP_VALUE_2,         // 0x0e160051, 热电偶读取实际AD值
  d_TempCtrlComm1_STATE_DATEMP_VALUE_3,         // 0x0e160052, 热电偶读取实际AD值
  d_TempCtrlComm1_STATE_DATEMP_VALUE_4,         // 0x0e160053, 热电偶读取实际AD值
  d_TempCtrlComm1_STATE_DATEMP_VALUE_5,         // 0x0e160054, 热电偶读取实际AD值
  d_TempCtrlComm1_STATE_DATEMP_VALUE_6,         // 0x0e160055, 热电偶读取实际AD值
  d_TempCtrlComm1_STATE_DATEMP_VALUE_7,         // 0x0e160056, 热电偶读取实际AD值
  d_TempCtrlComm1_STATE_DATEMP_VALUE_8,         // 0x0e160057, 热电偶读取实际AD值
  d_TempCtrlComm1_STATE_DATEMP_VALUE_9,         // 0x0e160058, 热电偶读取实际AD值
  d_TempCtrlComm1_STATE_DATEMP_VALUE_10,        // 0x0e160059, 热电偶读取实际AD值
  d_TempCtrlComm1_STATE_DATEMP_VALUE_11,        // 0x0e16005a, 热电偶读取实际AD值
  d_TempCtrlComm1_STATE_DATEMP_VALUE_12,        // 0x0e16005b, 热电偶读取实际AD值
  d_TempCtrlComm1_STATE_DATEMP_VALUE_13,        // 0x0e16005c, 热电偶读取实际AD值
  d_TempCtrlComm1_STATE_DATEMP_VALUE_14,        // 0x0e16005d, 热电偶读取实际AD值
  d_TempCtrlComm1_STATE_DATEMP_VALUE_15,        // 0x0e16005e, 热电偶读取实际AD值
  d_TempCtrlComm1_STATE_DATEMP_VALUE_16,        // 0x0e16005f, 热电偶读取实际AD值
  d_TempCtrlComm1_STATE_TM_COUNTDOWNTOWARM,     // 0x0e160060, 转保温倒计时
  d_TempCtrlComm1_STATE_RAMP_OPTIMIZE           // 0x0e160061, 缓冲区模型，优化标记      0:未自动优化1:表示已自动优化
};

enum db_TempCtrlComm2_PO{
  d_TempCtrlComm2_PO_O_COOLER = 0x0e200000, // 0x0e200000, 冷却水
  d_TempCtrlComm2_PO_O_COOL_1,              // 0x0e200001, 冷却器1
  d_TempCtrlComm2_PO_O_COOL_2,              // 0x0e200002, 冷却器2
  d_TempCtrlComm2_PO_O_COOL_3,              // 0x0e200003, 冷却器3
  d_TempCtrlComm2_PO_O_COOL_4,              // 0x0e200004, 冷却器4
  d_TempCtrlComm2_PO_O_COOL_5,              // 0x0e200005, 冷却器5
  d_TempCtrlComm2_PO_O_COOL_6,              // 0x0e200006, 冷却器6
  d_TempCtrlComm2_PO_O_COOL_7,              // 0x0e200007, 冷却器7
  d_TempCtrlComm2_PO_O_COOL_8,              // 0x0e200008, 冷却器8
  d_TempCtrlComm2_PO_O_COOL_9,              // 0x0e200009, 冷却器9
  d_TempCtrlComm2_PO_O_COOL_10,             // 0x0e20000a, 冷却器10
  d_TempCtrlComm2_PO_O_COOL_11,             // 0x0e20000b, 冷却器11
  d_TempCtrlComm2_PO_O_COOL_12,             // 0x0e20000c, 冷却器12
  d_TempCtrlComm2_PO_O_COOL_13,             // 0x0e20000d, 冷却器13
  d_TempCtrlComm2_PO_O_COOL_14,             // 0x0e20000e, 冷却器14
  d_TempCtrlComm2_PO_O_COOL_15,             // 0x0e20000f, 冷却器15
  d_TempCtrlComm2_PO_O_COOL_16,             // 0x0e200010, 冷却器16
  d_TempCtrlComm2_PO_O_TEMOP_1,             // 0x0e200011, 电热1
  d_TempCtrlComm2_PO_O_TEMOP_2,             // 0x0e200012, 电热2
  d_TempCtrlComm2_PO_O_TEMOP_3,             // 0x0e200013, 电热3
  d_TempCtrlComm2_PO_O_TEMOP_4,             // 0x0e200014, 电热4
  d_TempCtrlComm2_PO_O_TEMOP_5,             // 0x0e200015, 电热5
  d_TempCtrlComm2_PO_O_TEMOP_6,             // 0x0e200016, 电热6
  d_TempCtrlComm2_PO_O_TEMOP_7,             // 0x0e200017, 电热7
  d_TempCtrlComm2_PO_O_TEMOP_8,             // 0x0e200018, 电热8
  d_TempCtrlComm2_PO_O_TEMOP_9,             // 0x0e200019, 电热9
  d_TempCtrlComm2_PO_O_TEMOP_10,            // 0x0e20001a, 电热10
  d_TempCtrlComm2_PO_O_TEMOP_11,            // 0x0e20001b, 电热11
  d_TempCtrlComm2_PO_O_TEMOP_12,            // 0x0e20001c, 电热12
  d_TempCtrlComm2_PO_O_TEMOP_13,            // 0x0e20001d, 电热13
  d_TempCtrlComm2_PO_O_TEMOP_14,            // 0x0e20001e, 电热14
  d_TempCtrlComm2_PO_O_TEMOP_15,            // 0x0e20001f, 电热15
  d_TempCtrlComm2_PO_O_TEMOP_16,            // 0x0e200020, 电热16
  d_TempCtrlComm2_PO_O_DROP_COL,            // 0x0e200021, 落料口冷却水阀
  d_TempCtrlComm2_PO_O_HEATER               // 0x0e200022, 电热开
};

enum db_TempCtrlComm2_PI{
  d_TempCtrlComm2_PI_I_HEATTOUCH = 0x0e210000  // 0x0e210000, 加热接触器
};

enum db_TempCtrlComm2_MACHSET{
  d_TempCtrlComm2_MACHSET_DATEMP_FUNC_1 = 0x0e220000, // 0x0e220000, TC2 温度#1温控功能
  d_TempCtrlComm2_MACHSET_DATEMP_FUNC_2,              // 0x0e220001, TC2 温度#2温控功能
  d_TempCtrlComm2_MACHSET_DATEMP_FUNC_3,              // 0x0e220002, TC2 温度#3温控功能
  d_TempCtrlComm2_MACHSET_DATEMP_FUNC_4,              // 0x0e220003, TC2 温度#4温控功能
  d_TempCtrlComm2_MACHSET_DATEMP_FUNC_5,              // 0x0e220004, TC2 温度#5温控功能
  d_TempCtrlComm2_MACHSET_DATEMP_FUNC_6,              // 0x0e220005, TC2 温度#6温控功能
  d_TempCtrlComm2_MACHSET_DATEMP_FUNC_7,              // 0x0e220006, TC2 温度#7温控功能
  d_TempCtrlComm2_MACHSET_DATEMP_FUNC_8,              // 0x0e220007, TC2 温度#8温控功能
  d_TempCtrlComm2_MACHSET_DATEMP_FUNC_9,              // 0x0e220008, TC2 温度#9温控功能
  d_TempCtrlComm2_MACHSET_DATEMP_FUNC_10,             // 0x0e220009, TC2 温度#10温控功能
  d_TempCtrlComm2_MACHSET_DATEMP_FUNC_11,             // 0x0e22000a, TC2 温度#11温控功能
  d_TempCtrlComm2_MACHSET_DATEMP_FUNC_12,             // 0x0e22000b, TC2 温度#12温控功能
  d_TempCtrlComm2_MACHSET_DATEMP_FUNC_13,             // 0x0e22000c, TC2 温度#13温控功能
  d_TempCtrlComm2_MACHSET_DATEMP_FUNC_14,             // 0x0e22000d, TC2 温度#14温控功能
  d_TempCtrlComm2_MACHSET_DATEMP_FUNC_15,             // 0x0e22000e, TC2 温度#15温控功能
  d_TempCtrlComm2_MACHSET_DATEMP_FUNC_16,             // 0x0e22000f, TC2 温度#16温控功能
  d_TempCtrlComm2_MACHSET_DATEMP_RAMP_1,              // 0x0e220010, TC2 温控#1段缓冲区
  d_TempCtrlComm2_MACHSET_DATEMP_RAMP_2,              // 0x0e220011, TC2 温控#2段缓冲区
  d_TempCtrlComm2_MACHSET_DATEMP_RAMP_3,              // 0x0e220012, TC2 温控#3段缓冲区
  d_TempCtrlComm2_MACHSET_DATEMP_RAMP_4,              // 0x0e220013, TC2 温控#4段缓冲区
  d_TempCtrlComm2_MACHSET_DATEMP_RAMP_5,              // 0x0e220014, TC2 温控#5段缓冲区
  d_TempCtrlComm2_MACHSET_DATEMP_RAMP_6,              // 0x0e220015, TC2 温控#6段缓冲区
  d_TempCtrlComm2_MACHSET_DATEMP_RAMP_7,              // 0x0e220016, TC2 温控#7段缓冲区
  d_TempCtrlComm2_MACHSET_DATEMP_RAMP_8,              // 0x0e220017, TC2 温控#8段缓冲区
  d_TempCtrlComm2_MACHSET_DATEMP_RAMP_9,              // 0x0e220018, TC2 温控#9段缓冲区
  d_TempCtrlComm2_MACHSET_DATEMP_RAMP_10,             // 0x0e220019, TC2 温控#10段缓冲区
  d_TempCtrlComm2_MACHSET_DATEMP_RAMP_11,             // 0x0e22001a, TC2 温控#11段缓冲区
  d_TempCtrlComm2_MACHSET_DATEMP_RAMP_12,             // 0x0e22001b, TC2 温控#12段缓冲区
  d_TempCtrlComm2_MACHSET_DATEMP_RAMP_13,             // 0x0e22001c, TC2 温控#13段缓冲区
  d_TempCtrlComm2_MACHSET_DATEMP_RAMP_14,             // 0x0e22001d, TC2 温控#14段缓冲区
  d_TempCtrlComm2_MACHSET_DATEMP_RAMP_15,             // 0x0e22001e, TC2 温控#15段缓冲区
  d_TempCtrlComm2_MACHSET_DATEMP_RAMP_16,             // 0x0e22001f, TC2 温控#16段缓冲区
  d_TempCtrlComm2_MACHSET_DAPID_KP_1,                 // 0x0e220020, TC2 温度P系数#1
  d_TempCtrlComm2_MACHSET_DAPID_KP_2,                 // 0x0e220021, TC2 温度P系数#2
  d_TempCtrlComm2_MACHSET_DAPID_KP_3,                 // 0x0e220022, TC2 温度P系数#3
  d_TempCtrlComm2_MACHSET_DAPID_KP_4,                 // 0x0e220023, TC2 温度P系数#4
  d_TempCtrlComm2_MACHSET_DAPID_KP_5,                 // 0x0e220024, TC2 温度P系数#5
  d_TempCtrlComm2_MACHSET_DAPID_KP_6,                 // 0x0e220025, TC2 温度P系数#6
  d_TempCtrlComm2_MACHSET_DAPID_KP_7,                 // 0x0e220026, TC2 温度P系数#7
  d_TempCtrlComm2_MACHSET_DAPID_KP_8,                 // 0x0e220027, TC2 温度P系数#8
  d_TempCtrlComm2_MACHSET_DAPID_KP_9,                 // 0x0e220028, TC2 温度P系数#9
  d_TempCtrlComm2_MACHSET_DAPID_KP_10,                // 0x0e220029, TC2 温度P系数#10
  d_TempCtrlComm2_MACHSET_DAPID_KP_11,                // 0x0e22002a, TC2 温度P系数#11
  d_TempCtrlComm2_MACHSET_DAPID_KP_12,                // 0x0e22002b, TC2 温度P系数#12
  d_TempCtrlComm2_MACHSET_DAPID_KP_13,                // 0x0e22002c, TC2 温度P系数#13
  d_TempCtrlComm2_MACHSET_DAPID_KP_14,                // 0x0e22002d, TC2 温度P系数#14
  d_TempCtrlComm2_MACHSET_DAPID_KP_15,                // 0x0e22002e, TC2 温度P系数#15
  d_TempCtrlComm2_MACHSET_DAPID_KP_16,                // 0x0e22002f, TC2 温度P系数#16
  d_TempCtrlComm2_MACHSET_DAPID_TI_1,                 // 0x0e220030, TC2 温度I系数#1
  d_TempCtrlComm2_MACHSET_DAPID_TI_2,                 // 0x0e220031, TC2 温度I系数#2
  d_TempCtrlComm2_MACHSET_DAPID_TI_3,                 // 0x0e220032, TC2 温度I系数#3
  d_TempCtrlComm2_MACHSET_DAPID_TI_4,                 // 0x0e220033, TC2 温度I系数#4
  d_TempCtrlComm2_MACHSET_DAPID_TI_5,                 // 0x0e220034, TC2 温度I系数#5
  d_TempCtrlComm2_MACHSET_DAPID_TI_6,                 // 0x0e220035, TC2 温度I系数#6
  d_TempCtrlComm2_MACHSET_DAPID_TI_7,                 // 0x0e220036, TC2 温度I系数#7
  d_TempCtrlComm2_MACHSET_DAPID_TI_8,                 // 0x0e220037, TC2 温度I系数#8
  d_TempCtrlComm2_MACHSET_DAPID_TI_9,                 // 0x0e220038, TC2 温度I系数#9
  d_TempCtrlComm2_MACHSET_DAPID_TI_10,                // 0x0e220039, TC2 温度I系数#10
  d_TempCtrlComm2_MACHSET_DAPID_TI_11,                // 0x0e22003a, TC2 温度I系数#11
  d_TempCtrlComm2_MACHSET_DAPID_TI_12,                // 0x0e22003b, TC2 温度I系数#12
  d_TempCtrlComm2_MACHSET_DAPID_TI_13,                // 0x0e22003c, TC2 温度I系数#13
  d_TempCtrlComm2_MACHSET_DAPID_TI_14,                // 0x0e22003d, TC2 温度I系数#14
  d_TempCtrlComm2_MACHSET_DAPID_TI_15,                // 0x0e22003e, TC2 温度I系数#15
  d_TempCtrlComm2_MACHSET_DAPID_TI_16,                // 0x0e22003f, TC2 温度I系数#16
  d_TempCtrlComm2_MACHSET_DAPID_TD_1,                 // 0x0e220040, TC2 温度D系数#1
  d_TempCtrlComm2_MACHSET_DAPID_TD_2,                 // 0x0e220041, TC2 温度D系数#2
  d_TempCtrlComm2_MACHSET_DAPID_TD_3,                 // 0x0e220042, TC2 温度D系数#3
  d_TempCtrlComm2_MACHSET_DAPID_TD_4,                 // 0x0e220043, TC2 温度D系数#4
  d_TempCtrlComm2_MACHSET_DAPID_TD_5,                 // 0x0e220044, TC2 温度D系数#5
  d_TempCtrlComm2_MACHSET_DAPID_TD_6,                 // 0x0e220045, TC2 温度D系数#6
  d_TempCtrlComm2_MACHSET_DAPID_TD_7,                 // 0x0e220046, TC2 温度D系数#7
  d_TempCtrlComm2_MACHSET_DAPID_TD_8,                 // 0x0e220047, TC2 温度D系数#8
  d_TempCtrlComm2_MACHSET_DAPID_TD_9,                 // 0x0e220048, TC2 温度D系数#9
  d_TempCtrlComm2_MACHSET_DAPID_TD_10,                // 0x0e220049, TC2 温度D系数#10
  d_TempCtrlComm2_MACHSET_DAPID_TD_11,                // 0x0e22004a, TC2 温度D系数#11
  d_TempCtrlComm2_MACHSET_DAPID_TD_12,                // 0x0e22004b, TC2 温度D系数#12
  d_TempCtrlComm2_MACHSET_DAPID_TD_13,                // 0x0e22004c, TC2 温度D系数#13
  d_TempCtrlComm2_MACHSET_DAPID_TD_14,                // 0x0e22004d, TC2 温度D系数#14
  d_TempCtrlComm2_MACHSET_DAPID_TD_15,                // 0x0e22004e, TC2 温度D系数#15
  d_TempCtrlComm2_MACHSET_DAPID_TD_16,                // 0x0e22004f, TC2 温度D系数#16
  d_TempCtrlComm2_MACHSET_TM_RESPONSELESSCYCLE_1,     // 0x0e220050, TC2 温度#1输出周期时间
  d_TempCtrlComm2_MACHSET_TM_RESPONSELESSCYCLE_2,     // 0x0e220051, TC2 温度#2输出周期时间
  d_TempCtrlComm2_MACHSET_TM_RESPONSELESSCYCLE_3,     // 0x0e220052, TC2 温度#3输出周期时间
  d_TempCtrlComm2_MACHSET_TM_RESPONSELESSCYCLE_4,     // 0x0e220053, TC2 温度#4输出周期时间
  d_TempCtrlComm2_MACHSET_TM_RESPONSELESSCYCLE_5,     // 0x0e220054, TC2 温度#5输出周期时间
  d_TempCtrlComm2_MACHSET_TM_RESPONSELESSCYCLE_6,     // 0x0e220055, TC2 温度#6输出周期时间
  d_TempCtrlComm2_MACHSET_TM_RESPONSELESSCYCLE_7,     // 0x0e220056, TC2 温度#7输出周期时间
  d_TempCtrlComm2_MACHSET_TM_RESPONSELESSCYCLE_8,     // 0x0e220057, TC2 温度#8输出周期时间
  d_TempCtrlComm2_MACHSET_TM_RESPONSELESSCYCLE_9,     // 0x0e220058, TC2 温度#9输出周期时间
  d_TempCtrlComm2_MACHSET_TM_RESPONSELESSCYCLE_10,    // 0x0e220059, TC2 温度#10输出周期时间
  d_TempCtrlComm2_MACHSET_TM_RESPONSELESSCYCLE_11,    // 0x0e22005a, TC2 温度#11输出周期时间
  d_TempCtrlComm2_MACHSET_TM_RESPONSELESSCYCLE_12,    // 0x0e22005b, TC2 温度#12输出周期时间
  d_TempCtrlComm2_MACHSET_TM_RESPONSELESSCYCLE_13,    // 0x0e22005c, TC2 温度#13输出周期时间
  d_TempCtrlComm2_MACHSET_TM_RESPONSELESSCYCLE_14,    // 0x0e22005d, TC2 温度#14输出周期时间
  d_TempCtrlComm2_MACHSET_TM_RESPONSELESSCYCLE_15,    // 0x0e22005e, TC2 温度#15输出周期时间
  d_TempCtrlComm2_MACHSET_TM_RESPONSELESSCYCLE_16,    // 0x0e22005f, TC2 温度#16输出周期时间
  d_TempCtrlComm2_MACHSET_DATEMP_SCTM_MIN,            // 0x0e220060, TC2 最小扫描周期
  d_TempCtrlComm2_MACHSET_FL_TEMPWORDUPCHECK,         // 0x0e220061, TC2 电热连续加温检查
  d_TempCtrlComm2_MACHSET_DATEMP_TEMPCOOLERON,        // 0x0e220062, TC2 料管冷却开启上偏差
  d_TempCtrlComm2_MACHSET_DATEMP_TEMPCOOLEROFF,       // 0x0e220063, TC2 料管冷却关闭下偏差
  d_TempCtrlComm2_MACHSET_TEMPCOOLCTRL,               // 0x0e220064, TC2 落料口冷却功能
  d_TempCtrlComm2_MACHSET_TEMPCOOLMAX,                // 0x0e220065, TC2 落料口温度上限
  d_TempCtrlComm2_MACHSET_TEMPCOOLCN,                 // 0x0e220066, TC2 落料口温度段
  d_TempCtrlComm2_MACHSET_DATEMP_TEMPCOOLERONMOUTH,   // 0x0e220067, TC2 落料口冷却开
  d_TempCtrlComm2_MACHSET_DATEMP_TEMPCOOLEROFFMOUTH,  // 0x0e220068, TC2 落料口冷却关
  d_TempCtrlComm2_MACHSET_FL_TKTJ_1,                  // 0x0e220069, TC2 热电偶类型1
  d_TempCtrlComm2_MACHSET_FL_TKTJ_2,                  // 0x0e22006a, TC2 热电偶类型2
  d_TempCtrlComm2_MACHSET_FL_TKTJ_3,                  // 0x0e22006b, TC2 热电偶类型3
  d_TempCtrlComm2_MACHSET_FL_TKTJ_4,                  // 0x0e22006c, TC2 热电偶类型4
  d_TempCtrlComm2_MACHSET_FL_TKTJ_5,                  // 0x0e22006d, TC2 热电偶类型5
  d_TempCtrlComm2_MACHSET_FL_TKTJ_6,                  // 0x0e22006e, TC2 热电偶类型6
  d_TempCtrlComm2_MACHSET_FL_TKTJ_7,                  // 0x0e22006f, TC2 热电偶类型7
  d_TempCtrlComm2_MACHSET_FL_TKTJ_8,                  // 0x0e220070, TC2 热电偶类型8
  d_TempCtrlComm2_MACHSET_FL_TKTJ_9,                  // 0x0e220071, TC2 热电偶类型9
  d_TempCtrlComm2_MACHSET_FL_TKTJ_10,                 // 0x0e220072, TC2 热电偶类型10
  d_TempCtrlComm2_MACHSET_FL_TKTJ_11,                 // 0x0e220073, TC2 热电偶类型11
  d_TempCtrlComm2_MACHSET_FL_TKTJ_12,                 // 0x0e220074, TC2 热电偶类型12
  d_TempCtrlComm2_MACHSET_FL_TKTJ_13,                 // 0x0e220075, TC2 热电偶类型13
  d_TempCtrlComm2_MACHSET_FL_TKTJ_14,                 // 0x0e220076, TC2 热电偶类型14
  d_TempCtrlComm2_MACHSET_FL_TKTJ_15,                 // 0x0e220077, TC2 热电偶类型15
  d_TempCtrlComm2_MACHSET_FL_TKTJ_16,                 // 0x0e220078, TC2 热电偶类型16
  d_TempCtrlComm2_MACHSET_DATEMP_CH_NUM,              // 0x0e220079, TC2 温度段数
  d_TempCtrlComm2_MACHSET_TEMP_PROTECT,               // 0x0e22007a, TC2 温度保护上限
  d_TempCtrlComm2_MACHSET_FL_FREETIME_WARMOFF,        // 0x0e22007b, TC2 动作闲置电热处理
  d_TempCtrlComm2_MACHSET_TM_FREETIME_WARMOFF,        // 0x0e22007c, TC2 电热闲置时间
  d_TempCtrlComm2_MACHSET_DA_THERMOCOUPLE_PRT,        // 0x0e22007d, TC2 电热连续加温检查度数
  d_TempCtrlComm2_MACHSET_TM_TEMPWORDUPCHECK          // 0x0e22007e, 电热连续加温检查时间
};

enum db_TempCtrlComm2_MOLDSET{
  d_TempCtrlComm2_MOLDSET_DATEMP_SETTING_1 = 0x0e230000, // 0x0e230000, TC2 温度1段设定值
  d_TempCtrlComm2_MOLDSET_DATEMP_SETTING_2,              // 0x0e230001, TC2 温度2段设定值
  d_TempCtrlComm2_MOLDSET_DATEMP_SETTING_3,              // 0x0e230002, TC2 温度3段设定值
  d_TempCtrlComm2_MOLDSET_DATEMP_SETTING_4,              // 0x0e230003, TC2 温度4段设定值
  d_TempCtrlComm2_MOLDSET_DATEMP_SETTING_5,              // 0x0e230004, TC2 温度5段设定值
  d_TempCtrlComm2_MOLDSET_DATEMP_SETTING_6,              // 0x0e230005, TC2 温度6段设定值
  d_TempCtrlComm2_MOLDSET_DATEMP_SETTING_7,              // 0x0e230006, TC2 温度7段设定值
  d_TempCtrlComm2_MOLDSET_DATEMP_SETTING_8,              // 0x0e230007, TC2 温度8段设定值
  d_TempCtrlComm2_MOLDSET_DATEMP_SETTING_9,              // 0x0e230008, TC2 温度9段设定值
  d_TempCtrlComm2_MOLDSET_DATEMP_SETTING_10,             // 0x0e230009, TC2 温度10段设定值
  d_TempCtrlComm2_MOLDSET_DATEMP_SETTING_11,             // 0x0e23000a, TC2 温度11段设定值
  d_TempCtrlComm2_MOLDSET_DATEMP_SETTING_12,             // 0x0e23000b, TC2 温度12段设定值
  d_TempCtrlComm2_MOLDSET_DATEMP_SETTING_13,             // 0x0e23000c, TC2 温度13段设定值
  d_TempCtrlComm2_MOLDSET_DATEMP_SETTING_14,             // 0x0e23000d, TC2 温度14段设定值
  d_TempCtrlComm2_MOLDSET_DATEMP_SETTING_15,             // 0x0e23000e, TC2 温度15段设定值
  d_TempCtrlComm2_MOLDSET_DATEMP_SETTING_16,             // 0x0e23000f, TC2 温度16段设定值
  d_TempCtrlComm2_MOLDSET_DATEMP_MAX_1,                  // 0x0e230010, TC2 温控#1段上偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MAX_2,                  // 0x0e230011, TC2 温控#2段上偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MAX_3,                  // 0x0e230012, TC2 温控#3段上偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MAX_4,                  // 0x0e230013, TC2 温控#4段上偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MAX_5,                  // 0x0e230014, TC2 温控#5段上偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MAX_6,                  // 0x0e230015, TC2 温控#6段上偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MAX_7,                  // 0x0e230016, TC2 温控#7段上偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MAX_8,                  // 0x0e230017, TC2 温控#8段上偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MAX_9,                  // 0x0e230018, TC2 温控#9段上偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MAX_10,                 // 0x0e230019, TC2 温控#10段上偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MAX_11,                 // 0x0e23001a, TC2 温控#11段上偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MAX_12,                 // 0x0e23001b, TC2 温控#12段上偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MAX_13,                 // 0x0e23001c, TC2 温控#13段上偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MAX_14,                 // 0x0e23001d, TC2 温控#14段上偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MAX_15,                 // 0x0e23001e, TC2 温控#15段上偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MAX_16,                 // 0x0e23001f, TC2 温控#16段上偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MIN_1,                  // 0x0e230020, TC2 温控#1段下偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MIN_2,                  // 0x0e230021, TC2 温控#2段下偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MIN_3,                  // 0x0e230022, TC2 温控#3段下偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MIN_4,                  // 0x0e230023, TC2 温控#4段下偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MIN_5,                  // 0x0e230024, TC2 温控#5段下偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MIN_6,                  // 0x0e230025, TC2 温控#6段下偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MIN_7,                  // 0x0e230026, TC2 温控#7段下偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MIN_8,                  // 0x0e230027, TC2 温控#8段下偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MIN_9,                  // 0x0e230028, TC2 温控#9段下偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MIN_10,                 // 0x0e230029, TC2 温控#10段下偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MIN_11,                 // 0x0e23002a, TC2 温控#11段下偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MIN_12,                 // 0x0e23002b, TC2 温控#12段下偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MIN_13,                 // 0x0e23002c, TC2 温控#13段下偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MIN_14,                 // 0x0e23002d, TC2 温控#14段下偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MIN_15,                 // 0x0e23002e, TC2 温控#15段下偏差
  d_TempCtrlComm2_MOLDSET_DATEMP_MIN_16,                 // 0x0e23002f, TC2 温控#16段下偏差
  d_TempCtrlComm2_MOLDSET_DACENT_HEATERON_1,             // 0x0e230030, TC2 温度#1手动加热百分比
  d_TempCtrlComm2_MOLDSET_DACENT_HEATERON_2,             // 0x0e230031, TC2 温度#2手动加热百分比
  d_TempCtrlComm2_MOLDSET_DACENT_HEATERON_3,             // 0x0e230032, TC2 温度#3手动加热百分比
  d_TempCtrlComm2_MOLDSET_DACENT_HEATERON_4,             // 0x0e230033, TC2 温度#4手动加热百分比
  d_TempCtrlComm2_MOLDSET_DACENT_HEATERON_5,             // 0x0e230034, TC2 温度#5手动加热百分比
  d_TempCtrlComm2_MOLDSET_DACENT_HEATERON_6,             // 0x0e230035, TC2 温度#6手动加热百分比
  d_TempCtrlComm2_MOLDSET_DACENT_HEATERON_7,             // 0x0e230036, TC2 温度#7手动加热百分比
  d_TempCtrlComm2_MOLDSET_DACENT_HEATERON_8,             // 0x0e230037, TC2 温度#8手动加热百分比
  d_TempCtrlComm2_MOLDSET_DACENT_HEATERON_9,             // 0x0e230038, TC2 温度#9手动加热百分比
  d_TempCtrlComm2_MOLDSET_DACENT_HEATERON_10,            // 0x0e230039, TC2 温度#10手动加热百分比
  d_TempCtrlComm2_MOLDSET_DACENT_HEATERON_11,            // 0x0e23003a, TC2 温度#11手动加热百分比
  d_TempCtrlComm2_MOLDSET_DACENT_HEATERON_12,            // 0x0e23003b, TC2 温度#12手动加热百分比
  d_TempCtrlComm2_MOLDSET_DACENT_HEATERON_13,            // 0x0e23003c, TC2 温度#13手动加热百分比
  d_TempCtrlComm2_MOLDSET_DACENT_HEATERON_14,            // 0x0e23003d, TC2 温度#14手动加热百分比
  d_TempCtrlComm2_MOLDSET_DACENT_HEATERON_15,            // 0x0e23003e, TC2 温度#15手动加热百分比
  d_TempCtrlComm2_MOLDSET_DACENT_HEATERON_16,            // 0x0e23003f, TC2 温度#16手动加热百分比
  d_TempCtrlComm2_MOLDSET_TM_HEATERWARM,                 // 0x0e230040, TC2 预温计时
  d_TempCtrlComm2_MOLDSET_FL_TEMPHOLD,                   // 0x0e230041, TC2 保温功能选择
  d_TempCtrlComm2_MOLDSET_DATEMP_HOLD,                   // 0x0e230042, TC2 保温温度
  d_TempCtrlComm2_MOLDSET_FL_TEMPCOOL,                   // 0x0e230043, TC2 料管冷却器使用
  d_TempCtrlComm2_MOLDSET_CONFUNC                        // 0x0e230044, TC2 同步升温功能
};

enum db_TempCtrlComm2_ZDSET{
  d_TempCtrlComm2_ZDSET_DATEMP_RAMP1 = 0x0e240000, // 0x0e240000, 温度加温缓冲区
  d_TempCtrlComm2_ZDSET_DATEMP_RAMP2,              // 0x0e240001, 
  d_TempCtrlComm2_ZDSET_DATEMP_RAMP3,              // 0x0e240002, 
  d_TempCtrlComm2_ZDSET_DATEMP_RAMP4,              // 0x0e240003, 
  d_TempCtrlComm2_ZDSET_DATEMP_RAMP5,              // 0x0e240004, 
  d_TempCtrlComm2_ZDSET_DATEMP_RAMP6,              // 0x0e240005, 
  d_TempCtrlComm2_ZDSET_DATEMP_RAMP7,              // 0x0e240006, 
  d_TempCtrlComm2_ZDSET_DATEMP_RAMP8,              // 0x0e240007, 
  d_TempCtrlComm2_ZDSET_DATEMP_RAMP9,              // 0x0e240008, 
  d_TempCtrlComm2_ZDSET_DATEMP_RAMP10,             // 0x0e240009, 
  d_TempCtrlComm2_ZDSET_DATEMP_RAMP11,             // 0x0e24000a, 
  d_TempCtrlComm2_ZDSET_DATEMP_RAMP12,             // 0x0e24000b, 
  d_TempCtrlComm2_ZDSET_DATEMP_RAMP13,             // 0x0e24000c, 
  d_TempCtrlComm2_ZDSET_DATEMP_RAMP14,             // 0x0e24000d, 
  d_TempCtrlComm2_ZDSET_DATEMP_RAMP15,             // 0x0e24000e, 
  d_TempCtrlComm2_ZDSET_DATEMP_RAMP16,             // 0x0e24000f, 
  d_TempCtrlComm2_ZDSET_DAPID_KP1,                 // 0x0e240010, 温度P系数
  d_TempCtrlComm2_ZDSET_DAPID_KP2,                 // 0x0e240011, 
  d_TempCtrlComm2_ZDSET_DAPID_KP3,                 // 0x0e240012, 
  d_TempCtrlComm2_ZDSET_DAPID_KP4,                 // 0x0e240013, 
  d_TempCtrlComm2_ZDSET_DAPID_KP5,                 // 0x0e240014, 
  d_TempCtrlComm2_ZDSET_DAPID_KP6,                 // 0x0e240015, 
  d_TempCtrlComm2_ZDSET_DAPID_KP7,                 // 0x0e240016, 
  d_TempCtrlComm2_ZDSET_DAPID_KP8,                 // 0x0e240017, 
  d_TempCtrlComm2_ZDSET_DAPID_KP9,                 // 0x0e240018, 
  d_TempCtrlComm2_ZDSET_DAPID_KP10,                // 0x0e240019, 
  d_TempCtrlComm2_ZDSET_DAPID_KP11,                // 0x0e24001a, 
  d_TempCtrlComm2_ZDSET_DAPID_KP12,                // 0x0e24001b, 
  d_TempCtrlComm2_ZDSET_DAPID_KP13,                // 0x0e24001c, 
  d_TempCtrlComm2_ZDSET_DAPID_KP14,                // 0x0e24001d, 
  d_TempCtrlComm2_ZDSET_DAPID_KP15,                // 0x0e24001e, 
  d_TempCtrlComm2_ZDSET_DAPID_KP16,                // 0x0e24001f, 
  d_TempCtrlComm2_ZDSET_DAPID_TI1,                 // 0x0e240020, 温度I系数
  d_TempCtrlComm2_ZDSET_DAPID_TI2,                 // 0x0e240021, 
  d_TempCtrlComm2_ZDSET_DAPID_TI3,                 // 0x0e240022, 
  d_TempCtrlComm2_ZDSET_DAPID_TI4,                 // 0x0e240023, 
  d_TempCtrlComm2_ZDSET_DAPID_TI5,                 // 0x0e240024, 
  d_TempCtrlComm2_ZDSET_DAPID_TI6,                 // 0x0e240025, 
  d_TempCtrlComm2_ZDSET_DAPID_TI7,                 // 0x0e240026, 
  d_TempCtrlComm2_ZDSET_DAPID_TI8,                 // 0x0e240027, 
  d_TempCtrlComm2_ZDSET_DAPID_TI9,                 // 0x0e240028, 
  d_TempCtrlComm2_ZDSET_DAPID_TI10,                // 0x0e240029, 
  d_TempCtrlComm2_ZDSET_DAPID_TI11,                // 0x0e24002a, 
  d_TempCtrlComm2_ZDSET_DAPID_TI12,                // 0x0e24002b, 
  d_TempCtrlComm2_ZDSET_DAPID_TI13,                // 0x0e24002c, 
  d_TempCtrlComm2_ZDSET_DAPID_TI14,                // 0x0e24002d, 
  d_TempCtrlComm2_ZDSET_DAPID_TI15,                // 0x0e24002e, 
  d_TempCtrlComm2_ZDSET_DAPID_TI16,                // 0x0e24002f, 
  d_TempCtrlComm2_ZDSET_DAPID_TD1,                 // 0x0e240030, 温度D系数
  d_TempCtrlComm2_ZDSET_DAPID_TD2,                 // 0x0e240031, 
  d_TempCtrlComm2_ZDSET_DAPID_TD3,                 // 0x0e240032, 
  d_TempCtrlComm2_ZDSET_DAPID_TD4,                 // 0x0e240033, 
  d_TempCtrlComm2_ZDSET_DAPID_TD5,                 // 0x0e240034, 
  d_TempCtrlComm2_ZDSET_DAPID_TD6,                 // 0x0e240035, 
  d_TempCtrlComm2_ZDSET_DAPID_TD7,                 // 0x0e240036, 
  d_TempCtrlComm2_ZDSET_DAPID_TD8,                 // 0x0e240037, 
  d_TempCtrlComm2_ZDSET_DAPID_TD9,                 // 0x0e240038, 
  d_TempCtrlComm2_ZDSET_DAPID_TD10,                // 0x0e240039, 
  d_TempCtrlComm2_ZDSET_DAPID_TD11,                // 0x0e24003a, 
  d_TempCtrlComm2_ZDSET_DAPID_TD12,                // 0x0e24003b, 
  d_TempCtrlComm2_ZDSET_DAPID_TD13,                // 0x0e24003c, 
  d_TempCtrlComm2_ZDSET_DAPID_TD14,                // 0x0e24003d, 
  d_TempCtrlComm2_ZDSET_DAPID_TD15,                // 0x0e24003e, 
  d_TempCtrlComm2_ZDSET_DAPID_TD16                 // 0x0e24003f, 
};

enum db_TempCtrlComm2_STATE{
  d_TempCtrlComm2_STATE_FL_ERROR0 = 0x0e260000, // 0x0e260000, 警报1
  d_TempCtrlComm2_STATE_FL_ERROR1,              // 0x0e260001, 警报2
  d_TempCtrlComm2_STATE_DATEMP_REAL_1,          // 0x0e260002, 温度实际值1
  d_TempCtrlComm2_STATE_DATEMP_REAL_2,          // 0x0e260003, 温度实际值2
  d_TempCtrlComm2_STATE_DATEMP_REAL_3,          // 0x0e260004, 温度实际值3
  d_TempCtrlComm2_STATE_DATEMP_REAL_4,          // 0x0e260005, 温度实际值4
  d_TempCtrlComm2_STATE_DATEMP_REAL_5,          // 0x0e260006, 温度实际值5
  d_TempCtrlComm2_STATE_DATEMP_REAL_6,          // 0x0e260007, 温度实际值6
  d_TempCtrlComm2_STATE_DATEMP_REAL_7,          // 0x0e260008, 温度实际值7
  d_TempCtrlComm2_STATE_DATEMP_REAL_8,          // 0x0e260009, 温度实际值8
  d_TempCtrlComm2_STATE_DATEMP_REAL_9,          // 0x0e26000a, 温度实际值9
  d_TempCtrlComm2_STATE_DATEMP_REAL_10,         // 0x0e26000b, 温度实际值10
  d_TempCtrlComm2_STATE_DATEMP_REAL_11,         // 0x0e26000c, 温度实际值11
  d_TempCtrlComm2_STATE_DATEMP_REAL_12,         // 0x0e26000d, 温度实际值12
  d_TempCtrlComm2_STATE_DATEMP_REAL_13,         // 0x0e26000e, 温度实际值13
  d_TempCtrlComm2_STATE_DATEMP_REAL_14,         // 0x0e26000f, 温度实际值14
  d_TempCtrlComm2_STATE_DATEMP_REAL_15,         // 0x0e260010, 温度实际值15
  d_TempCtrlComm2_STATE_DATEMP_REAL_16,         // 0x0e260011, 温度实际值16
  d_TempCtrlComm2_STATE_FL_TEMPSTATUS_1,        // 0x0e260012, 温度加温状态1
  d_TempCtrlComm2_STATE_FL_TEMPSTATUS_2,        // 0x0e260013, 温度加温状态2
  d_TempCtrlComm2_STATE_FL_TEMPSTATUS_3,        // 0x0e260014, 温度加温状态3
  d_TempCtrlComm2_STATE_FL_TEMPSTATUS_4,        // 0x0e260015, 温度加温状态4
  d_TempCtrlComm2_STATE_FL_TEMPSTATUS_5,        // 0x0e260016, 温度加温状态5
  d_TempCtrlComm2_STATE_FL_TEMPSTATUS_6,        // 0x0e260017, 温度加温状态6
  d_TempCtrlComm2_STATE_FL_TEMPSTATUS_7,        // 0x0e260018, 温度加温状态7
  d_TempCtrlComm2_STATE_FL_TEMPSTATUS_8,        // 0x0e260019, 温度加温状态8
  d_TempCtrlComm2_STATE_FL_TEMPSTATUS_9,        // 0x0e26001a, 温度加温状态9
  d_TempCtrlComm2_STATE_FL_TEMPSTATUS_10,       // 0x0e26001b, 温度加温状态10
  d_TempCtrlComm2_STATE_FL_TEMPSTATUS_11,       // 0x0e26001c, 温度加温状态11
  d_TempCtrlComm2_STATE_FL_TEMPSTATUS_12,       // 0x0e26001d, 温度加温状态12
  d_TempCtrlComm2_STATE_FL_TEMPSTATUS_13,       // 0x0e26001e, 温度加温状态13
  d_TempCtrlComm2_STATE_FL_TEMPSTATUS_14,       // 0x0e26001f, 温度加温状态14
  d_TempCtrlComm2_STATE_FL_TEMPSTATUS_15,       // 0x0e260020, 温度加温状态15
  d_TempCtrlComm2_STATE_FL_TEMPSTATUS_16,       // 0x0e260021, 温度加温状态16
  d_TempCtrlComm2_STATE_OUTPUT_PER_1,           // 0x0e260022, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm2_STATE_OUTPUT_PER_2,           // 0x0e260023, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm2_STATE_OUTPUT_PER_3,           // 0x0e260024, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm2_STATE_OUTPUT_PER_4,           // 0x0e260025, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm2_STATE_OUTPUT_PER_5,           // 0x0e260026, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm2_STATE_OUTPUT_PER_6,           // 0x0e260027, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm2_STATE_OUTPUT_PER_7,           // 0x0e260028, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm2_STATE_OUTPUT_PER_8,           // 0x0e260029, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm2_STATE_OUTPUT_PER_9,           // 0x0e26002a, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm2_STATE_OUTPUT_PER_10,          // 0x0e26002b, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm2_STATE_OUTPUT_PER_11,          // 0x0e26002c, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm2_STATE_OUTPUT_PER_12,          // 0x0e26002d, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm2_STATE_OUTPUT_PER_13,          // 0x0e26002e, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm2_STATE_OUTPUT_PER_14,          // 0x0e26002f, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm2_STATE_OUTPUT_PER_15,          // 0x0e260030, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm2_STATE_OUTPUT_PER_16,          // 0x0e260031, 温度输出实际值百分比[16](0为油温段)
  d_TempCtrlComm2_STATE_WTEMP_LM73,             // 0x0e260032, 室温LM73的温度
  d_TempCtrlComm2_STATE_WTEMP_RELAT,            // 0x0e260033, 校准后的实时温度
  d_TempCtrlComm2_STATE_WTEMP_ADVALUE,          // 0x0e260034, 采样得到的实时值
  d_TempCtrlComm2_STATE_WTEMP_CHANNEL,          // 0x0e260035, 调试温度通道值
  d_TempCtrlComm2_STATE_WTEMP_ADJALL,           // 0x0e260036, 温度多通道联调标志
  d_TempCtrlComm2_STATE_WTEMP_ADJAMB,           // 0x0e260037, 室温校正
  d_TempCtrlComm2_STATE_FL_HEATERSTATUS,        // 0x0e260038, 电热状态
  d_TempCtrlComm2_STATE_WSTATERSV6,             // 0x0e260039, 保温倒计时
  d_TempCtrlComm2_STATE_DATEMP_CHARGEPOS,       // 0x0e26003a, 落料口温度
  d_TempCtrlComm2_STATE_DATEMP_YOUHUAXUQIU,     // 0x0e26003b, 温度优化提示开电热的闪烁指示
  d_TempCtrlComm2_STATE_DATEMP_YOUHUAOK,        // 0x0e26003c, 温度优化开启与完成标记
  d_TempCtrlComm2_STATE_DATEMP_E502_VER,        // 0x0e26003d, 温度扩展板E502版本号	20190410
  d_TempCtrlComm2_STATE_WRITE_CNT_E502,         // 0x0e26003e, 通讯测定写入次数
  d_TempCtrlComm2_STATE_READ_CNT_E502,          // 0x0e26003f, 通讯测试读入次数
  d_TempCtrlComm2_STATE_GETTEMPONTM_60s_1,      // 0x0e260040, 60s内电热输出时间
  d_TempCtrlComm2_STATE_GETTEMPONTM_60s_2,      // 0x0e260041, 60s内电热输出时间
  d_TempCtrlComm2_STATE_GETTEMPONTM_60s_3,      // 0x0e260042, 60s内电热输出时间
  d_TempCtrlComm2_STATE_GETTEMPONTM_60s_4,      // 0x0e260043, 60s内电热输出时间
  d_TempCtrlComm2_STATE_GETTEMPONTM_60s_5,      // 0x0e260044, 60s内电热输出时间
  d_TempCtrlComm2_STATE_GETTEMPONTM_60s_6,      // 0x0e260045, 60s内电热输出时间
  d_TempCtrlComm2_STATE_GETTEMPONTM_60s_7,      // 0x0e260046, 60s内电热输出时间
  d_TempCtrlComm2_STATE_GETTEMPONTM_60s_8,      // 0x0e260047, 60s内电热输出时间
  d_TempCtrlComm2_STATE_GETTEMPONTM_60s_9,      // 0x0e260048, 60s内电热输出时间
  d_TempCtrlComm2_STATE_GETTEMPONTM_60s_10,     // 0x0e260049, 60s内电热输出时间
  d_TempCtrlComm2_STATE_GETTEMPONTM_60s_11,     // 0x0e26004a, 60s内电热输出时间
  d_TempCtrlComm2_STATE_GETTEMPONTM_60s_12,     // 0x0e26004b, 60s内电热输出时间
  d_TempCtrlComm2_STATE_GETTEMPONTM_60s_13,     // 0x0e26004c, 60s内电热输出时间
  d_TempCtrlComm2_STATE_GETTEMPONTM_60s_14,     // 0x0e26004d, 60s内电热输出时间
  d_TempCtrlComm2_STATE_GETTEMPONTM_60s_15,     // 0x0e26004e, 60s内电热输出时间
  d_TempCtrlComm2_STATE_GETTEMPONTM_60s_16,     // 0x0e26004f, 60s内电热输出时间
  d_TempCtrlComm2_STATE_DATEMP_VALUE_1,         // 0x0e260050, 热电偶读取实际AD值
  d_TempCtrlComm2_STATE_DATEMP_VALUE_2,         // 0x0e260051, 热电偶读取实际AD值
  d_TempCtrlComm2_STATE_DATEMP_VALUE_3,         // 0x0e260052, 热电偶读取实际AD值
  d_TempCtrlComm2_STATE_DATEMP_VALUE_4,         // 0x0e260053, 热电偶读取实际AD值
  d_TempCtrlComm2_STATE_DATEMP_VALUE_5,         // 0x0e260054, 热电偶读取实际AD值
  d_TempCtrlComm2_STATE_DATEMP_VALUE_6,         // 0x0e260055, 热电偶读取实际AD值
  d_TempCtrlComm2_STATE_DATEMP_VALUE_7,         // 0x0e260056, 热电偶读取实际AD值
  d_TempCtrlComm2_STATE_DATEMP_VALUE_8,         // 0x0e260057, 热电偶读取实际AD值
  d_TempCtrlComm2_STATE_DATEMP_VALUE_9,         // 0x0e260058, 热电偶读取实际AD值
  d_TempCtrlComm2_STATE_DATEMP_VALUE_10,        // 0x0e260059, 热电偶读取实际AD值
  d_TempCtrlComm2_STATE_DATEMP_VALUE_11,        // 0x0e26005a, 热电偶读取实际AD值
  d_TempCtrlComm2_STATE_DATEMP_VALUE_12,        // 0x0e26005b, 热电偶读取实际AD值
  d_TempCtrlComm2_STATE_DATEMP_VALUE_13,        // 0x0e26005c, 热电偶读取实际AD值
  d_TempCtrlComm2_STATE_DATEMP_VALUE_14,        // 0x0e26005d, 热电偶读取实际AD值
  d_TempCtrlComm2_STATE_DATEMP_VALUE_15,        // 0x0e26005e, 热电偶读取实际AD值
  d_TempCtrlComm2_STATE_DATEMP_VALUE_16,        // 0x0e26005f, 热电偶读取实际AD值
  d_TempCtrlComm2_STATE_TM_COUNTDOWNTOWARM,     // 0x0e260060, 转保温倒计时
  d_TempCtrlComm2_STATE_RAMP_OPTIMIZE           // 0x0e260061, 缓冲区模型，优化标记      0:未自动优化1:表示已自动优化
};

enum db_Pump1_PO{
  d_Pump1_PO_HYD_SHUNT_PUMP = 0x0f100000, // 0x0f100000, P分流阀
  d_Pump1_PO_HYD_COMB_PUMP                // 0x0f100001, P合流阀
};

enum db_Pump1_MACHSET{
  d_Pump1_MACHSET_OIL_PUMP_DISPLACEMENT = 0x0f120000, // 0x0f120000, 油泵1排量
  d_Pump1_MACHSET_EFFICIENCY_FACTOR,                  // 0x0f120001, 油泵1效率系数
  d_Pump1_MACHSET_MOTOR_SPEED_MAX,                    // 0x0f120002, 油泵1电机最大转速
  d_Pump1_MACHSET_ENCODERESOLUTION,                   // 0x0f120003, 油泵1编码器分辨率
  d_Pump1_MACHSET_TORQUEMAX,                          // 0x0f120004, 油泵1转矩上限
  d_Pump1_MACHSET_PRESSAD_CH,                         // 0x0f120005, 油泵1压力传感器AD通道
  d_Pump1_MACHSET_PRESSAD_TYPE,                       // 0x0f120006, 油泵1压力传感器量程
  d_Pump1_MACHSET_BASEPRES,                           // 0x0f120007, 油泵1底压
  d_Pump1_MACHSET_BASEFLOW,                           // 0x0f120008, 油泵1底流
  d_Pump1_MACHSET_PRESS_ACC,                          // 0x0f120009, 油泵1最小压力上升时间
  d_Pump1_MACHSET_PRESS_DEC,                          // 0x0f12000a, 油泵1最小压力下降时间
  d_Pump1_MACHSET_PRESS_ACC_FILTER,                   // 0x0f12000b, 油泵1压力上升滤波时间
  d_Pump1_MACHSET_PRESS_DEC_FILTER,                   // 0x0f12000c, 油泵1压力下降滤波时间
  d_Pump1_MACHSET_SPEED_ACC,                          // 0x0f12000d, 油泵1最小速度上升时间
  d_Pump1_MACHSET_SPEED_DEC,                          // 0x0f12000e, 油泵1最小速度下降时间
  d_Pump1_MACHSET_ECAT_CODE_CHK_H,                    // 0x0f12000f, 油泵1编码校对高位
  d_Pump1_MACHSET_ECAT_CODE_CHK_L,                    // 0x0f120010, 油泵1编码校对低位
  d_Pump1_MACHSET_REVERSEFLOWMAX,                     // 0x0f120011, 油泵1最大反向速度
  d_Pump1_MACHSET_TORQUEMAX_SLAVE_STOP,               // 0x0f120012, 油泵1从泵停转扭矩
  d_Pump1_MACHSET_MODE_SLAVE_FOLLOW                   // 0x0f120013, 油泵1从站跟随模式
};

enum db_Pump1_MOLDSET{
  d_Pump1_MOLDSET_PUMP_SLAVE_START = 0x0f130000, // 0x0f130000, 油泵1从站跟随启动速度
  d_Pump1_MOLDSET_PUMP_SLAVE_STOP                // 0x0f130001, 油泵1从站跟随停止速度
};

enum db_Pump1_STATE{
  d_Pump1_STATE_FL_ERROR0 = 0x0f160000, // 0x0f160000, 警报1
  d_Pump1_STATE_FL_ERROR1,              // 0x0f160001, 警报2
  d_Pump1_STATE_MOTOR_CURRENT,          // 0x0f160002, 电机电流
  d_Pump1_STATE_MOTOR_VOLTAGE,          // 0x0f160003, 电机电压
  d_Pump1_STATE_MOTOR_POWER,            // 0x0f160004, 电机功率
  d_Pump1_STATE_MOTOR_TEMP,             // 0x0f160005, 电机温度
  d_Pump1_STATE_MOTOR_SPEED_REAL,       // 0x0f160006, 电机实际转速
  d_Pump1_STATE_MOTOR_SPEED_SET,        // 0x0f160007, 电机指令速度
  d_Pump1_STATE_MOTOR_TORQUE_REAL,      // 0x0f160008, 电机实际转矩
  d_Pump1_STATE_SERVO_STATE,            // 0x0f160009, 伺服状态
  d_Pump1_STATE_SERVO_ERR,              // 0x0f16000a, 伺服警报
  d_Pump1_STATE_SERVO_TEMP,             // 0x0f16000b, 驱动器模块温度
  d_Pump1_STATE_SERVO_PRESS,            // 0x0f16000c, 实际油箱压力
  d_Pump1_STATE_ENCODE_POS,             // 0x0f16000d, 编码器位置
  d_Pump1_STATE_SERVO_COMM_ST,          // 0x0f16000e, 伺服通讯状态
  d_Pump1_STATE_SERVO_COMM_STH          // 0x0f16000f, 伺服通讯状态
};

enum db_Pump2_PO{
  d_Pump2_PO_HYD_SHUNT_PUMP = 0x0f200000, // 0x0f200000, P分流阀
  d_Pump2_PO_HYD_COMB_PUMP                // 0x0f200001, P合流阀
};

enum db_Pump2_MACHSET{
  d_Pump2_MACHSET_OIL_PUMP_DISPLACEMENT = 0x0f220000, // 0x0f220000, 油泵2排量
  d_Pump2_MACHSET_EFFICIENCY_FACTOR,                  // 0x0f220001, 油泵2效率系数
  d_Pump2_MACHSET_MOTOR_SPEED_MAX,                    // 0x0f220002, 油泵2电机最大转速
  d_Pump2_MACHSET_ENCODERESOLUTION,                   // 0x0f220003, 油泵2编码器分辨率
  d_Pump2_MACHSET_TORQUEMAX,                          // 0x0f220004, 油泵2转矩上限
  d_Pump2_MACHSET_PRESSAD_CH,                         // 0x0f220005, 油泵2压力传感器AD通道
  d_Pump2_MACHSET_PRESSAD_TYPE,                       // 0x0f220006, 油泵2压力传感器量程
  d_Pump2_MACHSET_BASEPRES,                           // 0x0f220007, 油泵2底压
  d_Pump2_MACHSET_BASEFLOW,                           // 0x0f220008, 油泵2底流
  d_Pump2_MACHSET_PRESS_ACC,                          // 0x0f220009, 油泵2最小压力上升时间
  d_Pump2_MACHSET_PRESS_DEC,                          // 0x0f22000a, 油泵2最小压力下降时间
  d_Pump2_MACHSET_PRESS_ACC_FILTER,                   // 0x0f22000b, 油泵2压力上升滤波时间
  d_Pump2_MACHSET_PRESS_DEC_FILTER,                   // 0x0f22000c, 油泵2压力下降滤波时间
  d_Pump2_MACHSET_SPEED_ACC,                          // 0x0f22000d, 油泵2最小速度上升时间
  d_Pump2_MACHSET_SPEED_DEC,                          // 0x0f22000e, 油泵2最小速度下降时间
  d_Pump2_MACHSET_ECAT_CODE_CHK_H,                    // 0x0f22000f, 油泵2编码校对高位
  d_Pump2_MACHSET_ECAT_CODE_CHK_L,                    // 0x0f220010, 油泵2编码校对低位
  d_Pump2_MACHSET_REVERSEFLOWMAX,                     // 0x0f220011, 油泵2最大反向速度
  d_Pump2_MACHSET_TORQUEMAX_SLAVE_STOP,               // 0x0f220012, 油泵2从泵停转扭矩
  d_Pump2_MACHSET_MODE_SLAVE_FOLLOW                   // 0x0f220013, 油泵2从站跟随模式
};

enum db_Pump2_MOLDSET{
  d_Pump2_MOLDSET_PUMP_SLAVE_START = 0x0f230000, // 0x0f230000, 油泵2从站跟随启动速度
  d_Pump2_MOLDSET_PUMP_SLAVE_STOP                // 0x0f230001, 油泵2从站跟随停止速度
};

enum db_Pump2_STATE{
  d_Pump2_STATE_FL_ERROR0 = 0x0f260000, // 0x0f260000, 警报1
  d_Pump2_STATE_FL_ERROR1,              // 0x0f260001, 警报2
  d_Pump2_STATE_MOTOR_CURRENT,          // 0x0f260002, 电机电流
  d_Pump2_STATE_MOTOR_VOLTAGE,          // 0x0f260003, 电机电压
  d_Pump2_STATE_MOTOR_POWER,            // 0x0f260004, 电机功率
  d_Pump2_STATE_MOTOR_TEMP,             // 0x0f260005, 电机温度
  d_Pump2_STATE_MOTOR_SPEED_REAL,       // 0x0f260006, 电机实际转速
  d_Pump2_STATE_MOTOR_SPEED_SET,        // 0x0f260007, 电机指令速度
  d_Pump2_STATE_MOTOR_TORQUE_REAL,      // 0x0f260008, 电机实际转矩
  d_Pump2_STATE_SERVO_STATE,            // 0x0f260009, 伺服状态
  d_Pump2_STATE_SERVO_ERR,              // 0x0f26000a, 伺服警报
  d_Pump2_STATE_SERVO_TEMP,             // 0x0f26000b, 驱动器模块温度
  d_Pump2_STATE_SERVO_PRESS,            // 0x0f26000c, 实际油箱压力
  d_Pump2_STATE_ENCODE_POS,             // 0x0f26000d, 编码器位置
  d_Pump2_STATE_SERVO_COMM_ST,          // 0x0f26000e, 伺服通讯状态
  d_Pump2_STATE_SERVO_COMM_STH          // 0x0f26000f, 伺服通讯状态
};

enum db_Pump3_PO{
  d_Pump3_PO_HYD_SHUNT_PUMP = 0x0f300000, // 0x0f300000, P分流阀
  d_Pump3_PO_HYD_COMB_PUMP                // 0x0f300001, P合流阀
};

enum db_Pump3_MACHSET{
  d_Pump3_MACHSET_OIL_PUMP_DISPLACEMENT = 0x0f320000, // 0x0f320000, 油泵3排量
  d_Pump3_MACHSET_EFFICIENCY_FACTOR,                  // 0x0f320001, 油泵3效率系数
  d_Pump3_MACHSET_MOTOR_SPEED_MAX,                    // 0x0f320002, 油泵3电机最大转速
  d_Pump3_MACHSET_ENCODERESOLUTION,                   // 0x0f320003, 油泵3编码器分辨率
  d_Pump3_MACHSET_TORQUEMAX,                          // 0x0f320004, 油泵3转矩上限
  d_Pump3_MACHSET_PRESSAD_CH,                         // 0x0f320005, 油泵3压力传感器AD通道
  d_Pump3_MACHSET_PRESSAD_TYPE,                       // 0x0f320006, 油泵3压力传感器量程
  d_Pump3_MACHSET_BASEPRES,                           // 0x0f320007, 油泵3底压
  d_Pump3_MACHSET_BASEFLOW,                           // 0x0f320008, 油泵3底流
  d_Pump3_MACHSET_PRESS_ACC,                          // 0x0f320009, 油泵3最小压力上升时间
  d_Pump3_MACHSET_PRESS_DEC,                          // 0x0f32000a, 油泵3最小压力下降时间
  d_Pump3_MACHSET_PRESS_ACC_FILTER,                   // 0x0f32000b, 油泵3压力上升滤波时间
  d_Pump3_MACHSET_PRESS_DEC_FILTER,                   // 0x0f32000c, 油泵3压力下降滤波时间
  d_Pump3_MACHSET_SPEED_ACC,                          // 0x0f32000d, 油泵3最小速度上升时间
  d_Pump3_MACHSET_SPEED_DEC,                          // 0x0f32000e, 油泵3最小速度下降时间
  d_Pump3_MACHSET_ECAT_CODE_CHK_H,                    // 0x0f32000f, 油泵3编码校对高位
  d_Pump3_MACHSET_ECAT_CODE_CHK_L,                    // 0x0f320010, 油泵3编码校对低位
  d_Pump3_MACHSET_REVERSEFLOWMAX,                     // 0x0f320011, 油泵3最大反向速度
  d_Pump3_MACHSET_TORQUEMAX_SLAVE_STOP,               // 0x0f320012, 油泵3从泵停转扭矩
  d_Pump3_MACHSET_MODE_SLAVE_FOLLOW                   // 0x0f320013, 油泵3从站跟随模式
};

enum db_Pump3_MOLDSET{
  d_Pump3_MOLDSET_PUMP_SLAVE_START = 0x0f330000, // 0x0f330000, 油泵3从站跟随启动速度
  d_Pump3_MOLDSET_PUMP_SLAVE_STOP                // 0x0f330001, 油泵3从站跟随停止速度
};

enum db_Pump3_STATE{
  d_Pump3_STATE_FL_ERROR0 = 0x0f360000, // 0x0f360000, 警报1
  d_Pump3_STATE_FL_ERROR1,              // 0x0f360001, 警报2
  d_Pump3_STATE_MOTOR_CURRENT,          // 0x0f360002, 电机电流
  d_Pump3_STATE_MOTOR_VOLTAGE,          // 0x0f360003, 电机电压
  d_Pump3_STATE_MOTOR_POWER,            // 0x0f360004, 电机功率
  d_Pump3_STATE_MOTOR_TEMP,             // 0x0f360005, 电机温度
  d_Pump3_STATE_MOTOR_SPEED_REAL,       // 0x0f360006, 电机实际转速
  d_Pump3_STATE_MOTOR_SPEED_SET,        // 0x0f360007, 电机指令速度
  d_Pump3_STATE_MOTOR_TORQUE_REAL,      // 0x0f360008, 电机实际转矩
  d_Pump3_STATE_SERVO_STATE,            // 0x0f360009, 伺服状态
  d_Pump3_STATE_SERVO_ERR,              // 0x0f36000a, 伺服警报
  d_Pump3_STATE_SERVO_TEMP,             // 0x0f36000b, 驱动器模块温度
  d_Pump3_STATE_SERVO_PRESS,            // 0x0f36000c, 实际油箱压力
  d_Pump3_STATE_ENCODE_POS,             // 0x0f36000d, 编码器位置
  d_Pump3_STATE_SERVO_COMM_ST,          // 0x0f36000e, 伺服通讯状态
  d_Pump3_STATE_SERVO_COMM_STH          // 0x0f36000f, 伺服通讯状态
};

enum db_Pump4_PO{
  d_Pump4_PO_HYD_SHUNT_PUMP = 0x0f400000, // 0x0f400000, P分流阀
  d_Pump4_PO_HYD_COMB_PUMP                // 0x0f400001, P合流阀
};

enum db_Pump4_MACHSET{
  d_Pump4_MACHSET_OIL_PUMP_DISPLACEMENT = 0x0f420000, // 0x0f420000, 油泵4排量
  d_Pump4_MACHSET_EFFICIENCY_FACTOR,                  // 0x0f420001, 油泵4效率系数
  d_Pump4_MACHSET_MOTOR_SPEED_MAX,                    // 0x0f420002, 油泵4电机最大转速
  d_Pump4_MACHSET_ENCODERESOLUTION,                   // 0x0f420003, 油泵4编码器分辨率
  d_Pump4_MACHSET_TORQUEMAX,                          // 0x0f420004, 油泵4转矩上限
  d_Pump4_MACHSET_PRESSAD_CH,                         // 0x0f420005, 油泵4压力传感器AD通道
  d_Pump4_MACHSET_PRESSAD_TYPE,                       // 0x0f420006, 油泵4压力传感器量程
  d_Pump4_MACHSET_BASEPRES,                           // 0x0f420007, 油泵4底压
  d_Pump4_MACHSET_BASEFLOW,                           // 0x0f420008, 油泵4底流
  d_Pump4_MACHSET_PRESS_ACC,                          // 0x0f420009, 油泵4最小压力上升时间
  d_Pump4_MACHSET_PRESS_DEC,                          // 0x0f42000a, 油泵4最小压力下降时间
  d_Pump4_MACHSET_PRESS_ACC_FILTER,                   // 0x0f42000b, 油泵4压力上升滤波时间
  d_Pump4_MACHSET_PRESS_DEC_FILTER,                   // 0x0f42000c, 油泵4压力下降滤波时间
  d_Pump4_MACHSET_SPEED_ACC,                          // 0x0f42000d, 油泵4最小速度上升时间
  d_Pump4_MACHSET_SPEED_DEC,                          // 0x0f42000e, 油泵4最小速度下降时间
  d_Pump4_MACHSET_ECAT_CODE_CHK_H,                    // 0x0f42000f, 油泵4编码校对高位
  d_Pump4_MACHSET_ECAT_CODE_CHK_L,                    // 0x0f420010, 油泵4编码校对低位
  d_Pump4_MACHSET_REVERSEFLOWMAX,                     // 0x0f420011, 油泵4最大反向速度
  d_Pump4_MACHSET_TORQUEMAX_SLAVE_STOP,               // 0x0f420012, 油泵4从泵停转扭矩
  d_Pump4_MACHSET_MODE_SLAVE_FOLLOW                   // 0x0f420013, 油泵4从站跟随模式
};

enum db_Pump4_MOLDSET{
  d_Pump4_MOLDSET_PUMP_SLAVE_START = 0x0f430000, // 0x0f430000, 油泵4从站跟随启动速度
  d_Pump4_MOLDSET_PUMP_SLAVE_STOP                // 0x0f430001, 油泵4从站跟随停止速度
};

enum db_Pump4_STATE{
  d_Pump4_STATE_FL_ERROR0 = 0x0f460000, // 0x0f460000, 警报1
  d_Pump4_STATE_FL_ERROR1,              // 0x0f460001, 警报2
  d_Pump4_STATE_MOTOR_CURRENT,          // 0x0f460002, 电机电流
  d_Pump4_STATE_MOTOR_VOLTAGE,          // 0x0f460003, 电机电压
  d_Pump4_STATE_MOTOR_POWER,            // 0x0f460004, 电机功率
  d_Pump4_STATE_MOTOR_TEMP,             // 0x0f460005, 电机温度
  d_Pump4_STATE_MOTOR_SPEED_REAL,       // 0x0f460006, 电机实际转速
  d_Pump4_STATE_MOTOR_SPEED_SET,        // 0x0f460007, 电机指令速度
  d_Pump4_STATE_MOTOR_TORQUE_REAL,      // 0x0f460008, 电机实际转矩
  d_Pump4_STATE_SERVO_STATE,            // 0x0f460009, 伺服状态
  d_Pump4_STATE_SERVO_ERR,              // 0x0f46000a, 伺服警报
  d_Pump4_STATE_SERVO_TEMP,             // 0x0f46000b, 驱动器模块温度
  d_Pump4_STATE_SERVO_PRESS,            // 0x0f46000c, 实际油箱压力
  d_Pump4_STATE_ENCODE_POS,             // 0x0f46000d, 编码器位置
  d_Pump4_STATE_SERVO_COMM_ST,          // 0x0f46000e, 伺服通讯状态
  d_Pump4_STATE_SERVO_COMM_STH          // 0x0f46000f, 伺服通讯状态
};

enum db_Pump5_PO{
  d_Pump5_PO_HYD_SHUNT_PUMP = 0x0f500000, // 0x0f500000, P分流阀
  d_Pump5_PO_HYD_COMB_PUMP                // 0x0f500001, P合流阀
};

enum db_Pump5_MACHSET{
  d_Pump5_MACHSET_OIL_PUMP_DISPLACEMENT = 0x0f520000, // 0x0f520000, 油泵5排量
  d_Pump5_MACHSET_EFFICIENCY_FACTOR,                  // 0x0f520001, 油泵5效率系数
  d_Pump5_MACHSET_MOTOR_SPEED_MAX,                    // 0x0f520002, 油泵5电机最大转速
  d_Pump5_MACHSET_ENCODERESOLUTION,                   // 0x0f520003, 油泵5编码器分辨率
  d_Pump5_MACHSET_TORQUEMAX,                          // 0x0f520004, 油泵5转矩上限
  d_Pump5_MACHSET_PRESSAD_CH,                         // 0x0f520005, 油泵5压力传感器AD通道
  d_Pump5_MACHSET_PRESSAD_TYPE,                       // 0x0f520006, 油泵5压力传感器量程
  d_Pump5_MACHSET_BASEPRES,                           // 0x0f520007, 油泵5底压
  d_Pump5_MACHSET_BASEFLOW,                           // 0x0f520008, 油泵5底流
  d_Pump5_MACHSET_PRESS_ACC,                          // 0x0f520009, 油泵5最小压力上升时间
  d_Pump5_MACHSET_PRESS_DEC,                          // 0x0f52000a, 油泵5最小压力下降时间
  d_Pump5_MACHSET_PRESS_ACC_FILTER,                   // 0x0f52000b, 油泵5压力上升滤波时间
  d_Pump5_MACHSET_PRESS_DEC_FILTER,                   // 0x0f52000c, 油泵5压力下降滤波时间
  d_Pump5_MACHSET_SPEED_ACC,                          // 0x0f52000d, 油泵5最小速度上升时间
  d_Pump5_MACHSET_SPEED_DEC,                          // 0x0f52000e, 油泵5最小速度下降时间
  d_Pump5_MACHSET_ECAT_CODE_CHK_H,                    // 0x0f52000f, 油泵5编码校对高位
  d_Pump5_MACHSET_ECAT_CODE_CHK_L,                    // 0x0f520010, 油泵5编码校对低位
  d_Pump5_MACHSET_REVERSEFLOWMAX,                     // 0x0f520011, 油泵5最大反向速度
  d_Pump5_MACHSET_TORQUEMAX_SLAVE_STOP,               // 0x0f520012, 油泵5从泵停转扭矩
  d_Pump5_MACHSET_MODE_SLAVE_FOLLOW                   // 0x0f520013, 油泵5从站跟随模式
};

enum db_Pump5_MOLDSET{
  d_Pump5_MOLDSET_PUMP_SLAVE_START = 0x0f530000, // 0x0f530000, 油泵5从站跟随启动速度
  d_Pump5_MOLDSET_PUMP_SLAVE_STOP                // 0x0f530001, 油泵5从站跟随停止速度
};

enum db_Pump5_STATE{
  d_Pump5_STATE_FL_ERROR0 = 0x0f560000, // 0x0f560000, 警报1
  d_Pump5_STATE_FL_ERROR1,              // 0x0f560001, 警报2
  d_Pump5_STATE_MOTOR_CURRENT,          // 0x0f560002, 电机电流
  d_Pump5_STATE_MOTOR_VOLTAGE,          // 0x0f560003, 电机电压
  d_Pump5_STATE_MOTOR_POWER,            // 0x0f560004, 电机功率
  d_Pump5_STATE_MOTOR_TEMP,             // 0x0f560005, 电机温度
  d_Pump5_STATE_MOTOR_SPEED_REAL,       // 0x0f560006, 电机实际转速
  d_Pump5_STATE_MOTOR_SPEED_SET,        // 0x0f560007, 电机指令速度
  d_Pump5_STATE_MOTOR_TORQUE_REAL,      // 0x0f560008, 电机实际转矩
  d_Pump5_STATE_SERVO_STATE,            // 0x0f560009, 伺服状态
  d_Pump5_STATE_SERVO_ERR,              // 0x0f56000a, 伺服警报
  d_Pump5_STATE_SERVO_TEMP,             // 0x0f56000b, 驱动器模块温度
  d_Pump5_STATE_SERVO_PRESS,            // 0x0f56000c, 实际油箱压力
  d_Pump5_STATE_ENCODE_POS,             // 0x0f56000d, 编码器位置
  d_Pump5_STATE_SERVO_COMM_ST,          // 0x0f56000e, 伺服通讯状态
  d_Pump5_STATE_SERVO_COMM_STH          // 0x0f56000f, 伺服通讯状态
};

enum db_Pump6_PO{
  d_Pump6_PO_HYD_SHUNT_PUMP = 0x0f600000, // 0x0f600000, P分流阀
  d_Pump6_PO_HYD_COMB_PUMP                // 0x0f600001, P合流阀
};

enum db_Pump6_MACHSET{
  d_Pump6_MACHSET_OIL_PUMP_DISPLACEMENT = 0x0f620000, // 0x0f620000, 油泵6排量
  d_Pump6_MACHSET_EFFICIENCY_FACTOR,                  // 0x0f620001, 油泵6效率系数
  d_Pump6_MACHSET_MOTOR_SPEED_MAX,                    // 0x0f620002, 油泵6电机最大转速
  d_Pump6_MACHSET_ENCODERESOLUTION,                   // 0x0f620003, 油泵6编码器分辨率
  d_Pump6_MACHSET_TORQUEMAX,                          // 0x0f620004, 油泵6转矩上限
  d_Pump6_MACHSET_PRESSAD_CH,                         // 0x0f620005, 油泵6压力传感器AD通道
  d_Pump6_MACHSET_PRESSAD_TYPE,                       // 0x0f620006, 油泵6压力传感器量程
  d_Pump6_MACHSET_BASEPRES,                           // 0x0f620007, 油泵6底压
  d_Pump6_MACHSET_BASEFLOW,                           // 0x0f620008, 油泵6底流
  d_Pump6_MACHSET_PRESS_ACC,                          // 0x0f620009, 油泵6最小压力上升时间
  d_Pump6_MACHSET_PRESS_DEC,                          // 0x0f62000a, 油泵6最小压力下降时间
  d_Pump6_MACHSET_PRESS_ACC_FILTER,                   // 0x0f62000b, 油泵6压力上升滤波时间
  d_Pump6_MACHSET_PRESS_DEC_FILTER,                   // 0x0f62000c, 油泵6压力下降滤波时间
  d_Pump6_MACHSET_SPEED_ACC,                          // 0x0f62000d, 油泵6最小速度上升时间
  d_Pump6_MACHSET_SPEED_DEC,                          // 0x0f62000e, 油泵6最小速度下降时间
  d_Pump6_MACHSET_ECAT_CODE_CHK_H,                    // 0x0f62000f, 油泵6编码校对高位
  d_Pump6_MACHSET_ECAT_CODE_CHK_L,                    // 0x0f620010, 油泵6编码校对低位
  d_Pump6_MACHSET_REVERSEFLOWMAX,                     // 0x0f620011, 油泵6最大反向速度
  d_Pump6_MACHSET_TORQUEMAX_SLAVE_STOP,               // 0x0f620012, 油泵6从泵停转扭矩
  d_Pump6_MACHSET_MODE_SLAVE_FOLLOW                   // 0x0f620013, 油泵6从站跟随模式
};

enum db_Pump6_MOLDSET{
  d_Pump6_MOLDSET_PUMP_SLAVE_START = 0x0f630000, // 0x0f630000, 油泵6从站跟随启动速度
  d_Pump6_MOLDSET_PUMP_SLAVE_STOP                // 0x0f630001, 油泵6从站跟随停止速度
};

enum db_Pump6_STATE{
  d_Pump6_STATE_FL_ERROR0 = 0x0f660000, // 0x0f660000, 警报1
  d_Pump6_STATE_FL_ERROR1,              // 0x0f660001, 警报2
  d_Pump6_STATE_MOTOR_CURRENT,          // 0x0f660002, 电机电流
  d_Pump6_STATE_MOTOR_VOLTAGE,          // 0x0f660003, 电机电压
  d_Pump6_STATE_MOTOR_POWER,            // 0x0f660004, 电机功率
  d_Pump6_STATE_MOTOR_TEMP,             // 0x0f660005, 电机温度
  d_Pump6_STATE_MOTOR_SPEED_REAL,       // 0x0f660006, 电机实际转速
  d_Pump6_STATE_MOTOR_SPEED_SET,        // 0x0f660007, 电机指令速度
  d_Pump6_STATE_MOTOR_TORQUE_REAL,      // 0x0f660008, 电机实际转矩
  d_Pump6_STATE_SERVO_STATE,            // 0x0f660009, 伺服状态
  d_Pump6_STATE_SERVO_ERR,              // 0x0f66000a, 伺服警报
  d_Pump6_STATE_SERVO_TEMP,             // 0x0f66000b, 驱动器模块温度
  d_Pump6_STATE_SERVO_PRESS,            // 0x0f66000c, 实际油箱压力
  d_Pump6_STATE_ENCODE_POS,             // 0x0f66000d, 编码器位置
  d_Pump6_STATE_SERVO_COMM_ST,          // 0x0f66000e, 伺服通讯状态
  d_Pump6_STATE_SERVO_COMM_STH          // 0x0f66000f, 伺服通讯状态
};

enum db_Pump7_PO{
  d_Pump7_PO_HYD_SHUNT_PUMP = 0x0f700000, // 0x0f700000, P分流阀
  d_Pump7_PO_HYD_COMB_PUMP                // 0x0f700001, P合流阀
};

enum db_Pump7_MACHSET{
  d_Pump7_MACHSET_OIL_PUMP_DISPLACEMENT = 0x0f720000, // 0x0f720000, 油泵7排量
  d_Pump7_MACHSET_EFFICIENCY_FACTOR,                  // 0x0f720001, 油泵7效率系数
  d_Pump7_MACHSET_MOTOR_SPEED_MAX,                    // 0x0f720002, 油泵7电机最大转速
  d_Pump7_MACHSET_ENCODERESOLUTION,                   // 0x0f720003, 油泵7编码器分辨率
  d_Pump7_MACHSET_TORQUEMAX,                          // 0x0f720004, 油泵7转矩上限
  d_Pump7_MACHSET_PRESSAD_CH,                         // 0x0f720005, 油泵7压力传感器AD通道
  d_Pump7_MACHSET_PRESSAD_TYPE,                       // 0x0f720006, 油泵7压力传感器量程
  d_Pump7_MACHSET_BASEPRES,                           // 0x0f720007, 油泵7底压
  d_Pump7_MACHSET_BASEFLOW,                           // 0x0f720008, 油泵7底流
  d_Pump7_MACHSET_PRESS_ACC,                          // 0x0f720009, 油泵7最小压力上升时间
  d_Pump7_MACHSET_PRESS_DEC,                          // 0x0f72000a, 油泵7最小压力下降时间
  d_Pump7_MACHSET_PRESS_ACC_FILTER,                   // 0x0f72000b, 油泵7压力上升滤波时间
  d_Pump7_MACHSET_PRESS_DEC_FILTER,                   // 0x0f72000c, 油泵7压力下降滤波时间
  d_Pump7_MACHSET_SPEED_ACC,                          // 0x0f72000d, 油泵7最小速度上升时间
  d_Pump7_MACHSET_SPEED_DEC,                          // 0x0f72000e, 油泵7最小速度下降时间
  d_Pump7_MACHSET_ECAT_CODE_CHK_H,                    // 0x0f72000f, 油泵7编码校对高位
  d_Pump7_MACHSET_ECAT_CODE_CHK_L,                    // 0x0f720010, 油泵7编码校对低位
  d_Pump7_MACHSET_REVERSEFLOWMAX,                     // 0x0f720011, 油泵7最大反向速度
  d_Pump7_MACHSET_TORQUEMAX_SLAVE_STOP,               // 0x0f720012, 油泵7从泵停转扭矩
  d_Pump7_MACHSET_MODE_SLAVE_FOLLOW                   // 0x0f720013, 油泵7从站跟随模式
};

enum db_Pump7_MOLDSET{
  d_Pump7_MOLDSET_PUMP_SLAVE_START = 0x0f730000, // 0x0f730000, 油泵7从站跟随启动速度
  d_Pump7_MOLDSET_PUMP_SLAVE_STOP                // 0x0f730001, 油泵7从站跟随停止速度
};

enum db_Pump7_STATE{
  d_Pump7_STATE_FL_ERROR0 = 0x0f760000, // 0x0f760000, 警报1
  d_Pump7_STATE_FL_ERROR1,              // 0x0f760001, 警报2
  d_Pump7_STATE_MOTOR_CURRENT,          // 0x0f760002, 电机电流
  d_Pump7_STATE_MOTOR_VOLTAGE,          // 0x0f760003, 电机电压
  d_Pump7_STATE_MOTOR_POWER,            // 0x0f760004, 电机功率
  d_Pump7_STATE_MOTOR_TEMP,             // 0x0f760005, 电机温度
  d_Pump7_STATE_MOTOR_SPEED_REAL,       // 0x0f760006, 电机实际转速
  d_Pump7_STATE_MOTOR_SPEED_SET,        // 0x0f760007, 电机指令速度
  d_Pump7_STATE_MOTOR_TORQUE_REAL,      // 0x0f760008, 电机实际转矩
  d_Pump7_STATE_SERVO_STATE,            // 0x0f760009, 伺服状态
  d_Pump7_STATE_SERVO_ERR,              // 0x0f76000a, 伺服警报
  d_Pump7_STATE_SERVO_TEMP,             // 0x0f76000b, 驱动器模块温度
  d_Pump7_STATE_SERVO_PRESS,            // 0x0f76000c, 实际油箱压力
  d_Pump7_STATE_ENCODE_POS,             // 0x0f76000d, 编码器位置
  d_Pump7_STATE_SERVO_COMM_ST,          // 0x0f76000e, 伺服通讯状态
  d_Pump7_STATE_SERVO_COMM_STH          // 0x0f76000f, 伺服通讯状态
};

enum db_Pump8_PO{
  d_Pump8_PO_HYD_SHUNT_PUMP = 0x0f800000, // 0x0f800000, P分流阀
  d_Pump8_PO_HYD_COMB_PUMP                // 0x0f800001, P合流阀
};

enum db_Pump8_MACHSET{
  d_Pump8_MACHSET_OIL_PUMP_DISPLACEMENT = 0x0f820000, // 0x0f820000, 油泵8排量
  d_Pump8_MACHSET_EFFICIENCY_FACTOR,                  // 0x0f820001, 油泵8效率系数
  d_Pump8_MACHSET_MOTOR_SPEED_MAX,                    // 0x0f820002, 油泵8电机最大转速
  d_Pump8_MACHSET_ENCODERESOLUTION,                   // 0x0f820003, 油泵8编码器分辨率
  d_Pump8_MACHSET_TORQUEMAX,                          // 0x0f820004, 油泵8转矩上限
  d_Pump8_MACHSET_PRESSAD_CH,                         // 0x0f820005, 油泵8压力传感器AD通道
  d_Pump8_MACHSET_PRESSAD_TYPE,                       // 0x0f820006, 油泵8压力传感器量程
  d_Pump8_MACHSET_BASEPRES,                           // 0x0f820007, 油泵8底压
  d_Pump8_MACHSET_BASEFLOW,                           // 0x0f820008, 油泵8底流
  d_Pump8_MACHSET_PRESS_ACC,                          // 0x0f820009, 油泵8最小压力上升时间
  d_Pump8_MACHSET_PRESS_DEC,                          // 0x0f82000a, 油泵8最小压力下降时间
  d_Pump8_MACHSET_PRESS_ACC_FILTER,                   // 0x0f82000b, 油泵8压力上升滤波时间
  d_Pump8_MACHSET_PRESS_DEC_FILTER,                   // 0x0f82000c, 油泵8压力下降滤波时间
  d_Pump8_MACHSET_SPEED_ACC,                          // 0x0f82000d, 油泵8最小速度上升时间
  d_Pump8_MACHSET_SPEED_DEC,                          // 0x0f82000e, 油泵8最小速度下降时间
  d_Pump8_MACHSET_ECAT_CODE_CHK_H,                    // 0x0f82000f, 油泵8编码校对高位
  d_Pump8_MACHSET_ECAT_CODE_CHK_L,                    // 0x0f820010, 油泵8编码校对低位
  d_Pump8_MACHSET_REVERSEFLOWMAX,                     // 0x0f820011, 油泵8最大反向速度
  d_Pump8_MACHSET_TORQUEMAX_SLAVE_STOP,               // 0x0f820012, 油泵8从泵停转扭矩
  d_Pump8_MACHSET_MODE_SLAVE_FOLLOW                   // 0x0f820013, 油泵8从站跟随模式
};

enum db_Pump8_MOLDSET{
  d_Pump8_MOLDSET_PUMP_SLAVE_START = 0x0f830000, // 0x0f830000, 油泵8从站跟随启动速度
  d_Pump8_MOLDSET_PUMP_SLAVE_STOP                // 0x0f830001, 油泵8从站跟随停止速度
};

enum db_Pump8_STATE{
  d_Pump8_STATE_FL_ERROR0 = 0x0f860000, // 0x0f860000, 警报1
  d_Pump8_STATE_FL_ERROR1,              // 0x0f860001, 警报2
  d_Pump8_STATE_MOTOR_CURRENT,          // 0x0f860002, 电机电流
  d_Pump8_STATE_MOTOR_VOLTAGE,          // 0x0f860003, 电机电压
  d_Pump8_STATE_MOTOR_POWER,            // 0x0f860004, 电机功率
  d_Pump8_STATE_MOTOR_TEMP,             // 0x0f860005, 电机温度
  d_Pump8_STATE_MOTOR_SPEED_REAL,       // 0x0f860006, 电机实际转速
  d_Pump8_STATE_MOTOR_SPEED_SET,        // 0x0f860007, 电机指令速度
  d_Pump8_STATE_MOTOR_TORQUE_REAL,      // 0x0f860008, 电机实际转矩
  d_Pump8_STATE_SERVO_STATE,            // 0x0f860009, 伺服状态
  d_Pump8_STATE_SERVO_ERR,              // 0x0f86000a, 伺服警报
  d_Pump8_STATE_SERVO_TEMP,             // 0x0f86000b, 驱动器模块温度
  d_Pump8_STATE_SERVO_PRESS,            // 0x0f86000c, 实际油箱压力
  d_Pump8_STATE_ENCODE_POS,             // 0x0f86000d, 编码器位置
  d_Pump8_STATE_SERVO_COMM_ST,          // 0x0f86000e, 伺服通讯状态
  d_Pump8_STATE_SERVO_COMM_STH          // 0x0f86000f, 伺服通讯状态
};

enum db_Pump9_PO{
  d_Pump9_PO_HYD_SHUNT_PUMP = 0x0f900000, // 0x0f900000, P分流阀
  d_Pump9_PO_HYD_COMB_PUMP                // 0x0f900001, P合流阀
};

enum db_Pump9_MACHSET{
  d_Pump9_MACHSET_OIL_PUMP_DISPLACEMENT = 0x0f920000, // 0x0f920000, 油泵9排量
  d_Pump9_MACHSET_EFFICIENCY_FACTOR,                  // 0x0f920001, 油泵9效率系数
  d_Pump9_MACHSET_MOTOR_SPEED_MAX,                    // 0x0f920002, 油泵9电机最大转速
  d_Pump9_MACHSET_ENCODERESOLUTION,                   // 0x0f920003, 油泵9编码器分辨率
  d_Pump9_MACHSET_TORQUEMAX,                          // 0x0f920004, 油泵9转矩上限
  d_Pump9_MACHSET_PRESSAD_CH,                         // 0x0f920005, 油泵9压力传感器AD通道
  d_Pump9_MACHSET_PRESSAD_TYPE,                       // 0x0f920006, 油泵9压力传感器量程
  d_Pump9_MACHSET_BASEPRES,                           // 0x0f920007, 油泵9底压
  d_Pump9_MACHSET_BASEFLOW,                           // 0x0f920008, 油泵9底流
  d_Pump9_MACHSET_PRESS_ACC,                          // 0x0f920009, 油泵9最小压力上升时间
  d_Pump9_MACHSET_PRESS_DEC,                          // 0x0f92000a, 油泵9最小压力下降时间
  d_Pump9_MACHSET_PRESS_ACC_FILTER,                   // 0x0f92000b, 油泵9压力上升滤波时间
  d_Pump9_MACHSET_PRESS_DEC_FILTER,                   // 0x0f92000c, 油泵9压力下降滤波时间
  d_Pump9_MACHSET_SPEED_ACC,                          // 0x0f92000d, 油泵9最小速度上升时间
  d_Pump9_MACHSET_SPEED_DEC,                          // 0x0f92000e, 油泵9最小速度下降时间
  d_Pump9_MACHSET_ECAT_CODE_CHK_H,                    // 0x0f92000f, 油泵9编码校对高位
  d_Pump9_MACHSET_ECAT_CODE_CHK_L,                    // 0x0f920010, 油泵9编码校对低位
  d_Pump9_MACHSET_REVERSEFLOWMAX,                     // 0x0f920011, 油泵9最大反向速度
  d_Pump9_MACHSET_TORQUEMAX_SLAVE_STOP,               // 0x0f920012, 油泵9从泵停转扭矩
  d_Pump9_MACHSET_MODE_SLAVE_FOLLOW                   // 0x0f920013, 油泵9从站跟随模式
};

enum db_Pump9_MOLDSET{
  d_Pump9_MOLDSET_PUMP_SLAVE_START = 0x0f930000, // 0x0f930000, 油泵9从站跟随启动速度
  d_Pump9_MOLDSET_PUMP_SLAVE_STOP                // 0x0f930001, 油泵9从站跟随停止速度
};

enum db_Pump9_STATE{
  d_Pump9_STATE_FL_ERROR0 = 0x0f960000, // 0x0f960000, 警报1
  d_Pump9_STATE_FL_ERROR1,              // 0x0f960001, 警报2
  d_Pump9_STATE_MOTOR_CURRENT,          // 0x0f960002, 电机电流
  d_Pump9_STATE_MOTOR_VOLTAGE,          // 0x0f960003, 电机电压
  d_Pump9_STATE_MOTOR_POWER,            // 0x0f960004, 电机功率
  d_Pump9_STATE_MOTOR_TEMP,             // 0x0f960005, 电机温度
  d_Pump9_STATE_MOTOR_SPEED_REAL,       // 0x0f960006, 电机实际转速
  d_Pump9_STATE_MOTOR_SPEED_SET,        // 0x0f960007, 电机指令速度
  d_Pump9_STATE_MOTOR_TORQUE_REAL,      // 0x0f960008, 电机实际转矩
  d_Pump9_STATE_SERVO_STATE,            // 0x0f960009, 伺服状态
  d_Pump9_STATE_SERVO_ERR,              // 0x0f96000a, 伺服警报
  d_Pump9_STATE_SERVO_TEMP,             // 0x0f96000b, 驱动器模块温度
  d_Pump9_STATE_SERVO_PRESS,            // 0x0f96000c, 实际油箱压力
  d_Pump9_STATE_ENCODE_POS,             // 0x0f96000d, 编码器位置
  d_Pump9_STATE_SERVO_COMM_ST,          // 0x0f96000e, 伺服通讯状态
  d_Pump9_STATE_SERVO_COMM_STH          // 0x0f96000f, 伺服通讯状态
};

enum db_Pump10_PO{
  d_Pump10_PO_HYD_SHUNT_PUMP = 0x0fa00000, // 0x0fa00000, P分流阀
  d_Pump10_PO_HYD_COMB_PUMP                // 0x0fa00001, P合流阀
};

enum db_Pump10_MACHSET{
  d_Pump10_MACHSET_OIL_PUMP_DISPLACEMENT = 0x0fa20000, // 0x0fa20000, 油泵10排量
  d_Pump10_MACHSET_EFFICIENCY_FACTOR,                  // 0x0fa20001, 油泵10效率系数
  d_Pump10_MACHSET_MOTOR_SPEED_MAX,                    // 0x0fa20002, 油泵10电机最大转速
  d_Pump10_MACHSET_ENCODERESOLUTION,                   // 0x0fa20003, 油泵10编码器分辨率
  d_Pump10_MACHSET_TORQUEMAX,                          // 0x0fa20004, 油泵10转矩上限
  d_Pump10_MACHSET_PRESSAD_CH,                         // 0x0fa20005, 油泵10压力传感器AD通道
  d_Pump10_MACHSET_PRESSAD_TYPE,                       // 0x0fa20006, 油泵10压力传感器量程
  d_Pump10_MACHSET_BASEPRES,                           // 0x0fa20007, 油泵10底压
  d_Pump10_MACHSET_BASEFLOW,                           // 0x0fa20008, 油泵10底流
  d_Pump10_MACHSET_PRESS_ACC,                          // 0x0fa20009, 油泵10最小压力上升时间
  d_Pump10_MACHSET_PRESS_DEC,                          // 0x0fa2000a, 油泵10最小压力下降时间
  d_Pump10_MACHSET_PRESS_ACC_FILTER,                   // 0x0fa2000b, 油泵10压力上升滤波时间
  d_Pump10_MACHSET_PRESS_DEC_FILTER,                   // 0x0fa2000c, 油泵10压力下降滤波时间
  d_Pump10_MACHSET_SPEED_ACC,                          // 0x0fa2000d, 油泵10最小速度上升时间
  d_Pump10_MACHSET_SPEED_DEC,                          // 0x0fa2000e, 油泵10最小速度下降时间
  d_Pump10_MACHSET_ECAT_CODE_CHK_H,                    // 0x0fa2000f, 油泵10编码校对高位
  d_Pump10_MACHSET_ECAT_CODE_CHK_L,                    // 0x0fa20010, 油泵10编码校对低位
  d_Pump10_MACHSET_REVERSEFLOWMAX,                     // 0x0fa20011, 油泵10最大反向速度
  d_Pump10_MACHSET_TORQUEMAX_SLAVE_STOP,               // 0x0fa20012, 油泵10从泵停转扭矩
  d_Pump10_MACHSET_MODE_SLAVE_FOLLOW                   // 0x0fa20013, 油泵10从站跟随模式
};

enum db_Pump10_MOLDSET{
  d_Pump10_MOLDSET_PUMP_SLAVE_START = 0x0fa30000, // 0x0fa30000, 油泵10从站跟随启动速度
  d_Pump10_MOLDSET_PUMP_SLAVE_STOP                // 0x0fa30001, 油泵10从站跟随停止速度
};

enum db_Pump10_STATE{
  d_Pump10_STATE_FL_ERROR0 = 0x0fa60000, // 0x0fa60000, 警报1
  d_Pump10_STATE_FL_ERROR1,              // 0x0fa60001, 警报2
  d_Pump10_STATE_MOTOR_CURRENT,          // 0x0fa60002, 电机电流
  d_Pump10_STATE_MOTOR_VOLTAGE,          // 0x0fa60003, 电机电压
  d_Pump10_STATE_MOTOR_POWER,            // 0x0fa60004, 电机功率
  d_Pump10_STATE_MOTOR_TEMP,             // 0x0fa60005, 电机温度
  d_Pump10_STATE_MOTOR_SPEED_REAL,       // 0x0fa60006, 电机实际转速
  d_Pump10_STATE_MOTOR_SPEED_SET,        // 0x0fa60007, 电机指令速度
  d_Pump10_STATE_MOTOR_TORQUE_REAL,      // 0x0fa60008, 电机实际转矩
  d_Pump10_STATE_SERVO_STATE,            // 0x0fa60009, 伺服状态
  d_Pump10_STATE_SERVO_ERR,              // 0x0fa6000a, 伺服警报
  d_Pump10_STATE_SERVO_TEMP,             // 0x0fa6000b, 驱动器模块温度
  d_Pump10_STATE_SERVO_PRESS,            // 0x0fa6000c, 实际油箱压力
  d_Pump10_STATE_ENCODE_POS,             // 0x0fa6000d, 编码器位置
  d_Pump10_STATE_SERVO_COMM_ST,          // 0x0fa6000e, 伺服通讯状态
  d_Pump10_STATE_SERVO_COMM_STH          // 0x0fa6000f, 伺服通讯状态
};

enum db_Pump11_PO{
  d_Pump11_PO_HYD_SHUNT_PUMP = 0x0fb00000, // 0x0fb00000, P分流阀
  d_Pump11_PO_HYD_COMB_PUMP                // 0x0fb00001, P合流阀
};

enum db_Pump11_MACHSET{
  d_Pump11_MACHSET_OIL_PUMP_DISPLACEMENT = 0x0fb20000, // 0x0fb20000, 油泵11排量
  d_Pump11_MACHSET_EFFICIENCY_FACTOR,                  // 0x0fb20001, 油泵11效率系数
  d_Pump11_MACHSET_MOTOR_SPEED_MAX,                    // 0x0fb20002, 油泵11电机最大转速
  d_Pump11_MACHSET_ENCODERESOLUTION,                   // 0x0fb20003, 油泵11编码器分辨率
  d_Pump11_MACHSET_TORQUEMAX,                          // 0x0fb20004, 油泵11转矩上限
  d_Pump11_MACHSET_PRESSAD_CH,                         // 0x0fb20005, 油泵11压力传感器AD通道
  d_Pump11_MACHSET_PRESSAD_TYPE,                       // 0x0fb20006, 油泵11压力传感器量程
  d_Pump11_MACHSET_BASEPRES,                           // 0x0fb20007, 油泵11底压
  d_Pump11_MACHSET_BASEFLOW,                           // 0x0fb20008, 油泵11底流
  d_Pump11_MACHSET_PRESS_ACC,                          // 0x0fb20009, 油泵11最小压力上升时间
  d_Pump11_MACHSET_PRESS_DEC,                          // 0x0fb2000a, 油泵11最小压力下降时间
  d_Pump11_MACHSET_PRESS_ACC_FILTER,                   // 0x0fb2000b, 油泵11压力上升滤波时间
  d_Pump11_MACHSET_PRESS_DEC_FILTER,                   // 0x0fb2000c, 油泵11压力下降滤波时间
  d_Pump11_MACHSET_SPEED_ACC,                          // 0x0fb2000d, 油泵11最小速度上升时间
  d_Pump11_MACHSET_SPEED_DEC,                          // 0x0fb2000e, 油泵11最小速度下降时间
  d_Pump11_MACHSET_ECAT_CODE_CHK_H,                    // 0x0fb2000f, 油泵11编码校对高位
  d_Pump11_MACHSET_ECAT_CODE_CHK_L,                    // 0x0fb20010, 油泵11编码校对低位
  d_Pump11_MACHSET_REVERSEFLOWMAX,                     // 0x0fb20011, 油泵11最大反向速度
  d_Pump11_MACHSET_TORQUEMAX_SLAVE_STOP,               // 0x0fb20012, 油泵11从泵停转扭矩
  d_Pump11_MACHSET_MODE_SLAVE_FOLLOW                   // 0x0fb20013, 油泵11从站跟随模式
};

enum db_Pump11_MOLDSET{
  d_Pump11_MOLDSET_PUMP_SLAVE_START = 0x0fb30000, // 0x0fb30000, 油泵11从站跟随启动速度
  d_Pump11_MOLDSET_PUMP_SLAVE_STOP                // 0x0fb30001, 油泵11从站跟随停止速度
};

enum db_Pump11_STATE{
  d_Pump11_STATE_FL_ERROR0 = 0x0fb60000, // 0x0fb60000, 警报1
  d_Pump11_STATE_FL_ERROR1,              // 0x0fb60001, 警报2
  d_Pump11_STATE_MOTOR_CURRENT,          // 0x0fb60002, 电机电流
  d_Pump11_STATE_MOTOR_VOLTAGE,          // 0x0fb60003, 电机电压
  d_Pump11_STATE_MOTOR_POWER,            // 0x0fb60004, 电机功率
  d_Pump11_STATE_MOTOR_TEMP,             // 0x0fb60005, 电机温度
  d_Pump11_STATE_MOTOR_SPEED_REAL,       // 0x0fb60006, 电机实际转速
  d_Pump11_STATE_MOTOR_SPEED_SET,        // 0x0fb60007, 电机指令速度
  d_Pump11_STATE_MOTOR_TORQUE_REAL,      // 0x0fb60008, 电机实际转矩
  d_Pump11_STATE_SERVO_STATE,            // 0x0fb60009, 伺服状态
  d_Pump11_STATE_SERVO_ERR,              // 0x0fb6000a, 伺服警报
  d_Pump11_STATE_SERVO_TEMP,             // 0x0fb6000b, 驱动器模块温度
  d_Pump11_STATE_SERVO_PRESS,            // 0x0fb6000c, 实际油箱压力
  d_Pump11_STATE_ENCODE_POS,             // 0x0fb6000d, 编码器位置
  d_Pump11_STATE_SERVO_COMM_ST,          // 0x0fb6000e, 伺服通讯状态
  d_Pump11_STATE_SERVO_COMM_STH          // 0x0fb6000f, 伺服通讯状态
};

enum db_Pump12_PO{
  d_Pump12_PO_HYD_SHUNT_PUMP = 0x0fc00000, // 0x0fc00000, P分流阀
  d_Pump12_PO_HYD_COMB_PUMP                // 0x0fc00001, P合流阀
};

enum db_Pump12_MACHSET{
  d_Pump12_MACHSET_OIL_PUMP_DISPLACEMENT = 0x0fc20000, // 0x0fc20000, 油泵12排量
  d_Pump12_MACHSET_EFFICIENCY_FACTOR,                  // 0x0fc20001, 油泵12效率系数
  d_Pump12_MACHSET_MOTOR_SPEED_MAX,                    // 0x0fc20002, 油泵12电机最大转速
  d_Pump12_MACHSET_ENCODERESOLUTION,                   // 0x0fc20003, 油泵12编码器分辨率
  d_Pump12_MACHSET_TORQUEMAX,                          // 0x0fc20004, 油泵12转矩上限
  d_Pump12_MACHSET_PRESSAD_CH,                         // 0x0fc20005, 油泵12压力传感器AD通道
  d_Pump12_MACHSET_PRESSAD_TYPE,                       // 0x0fc20006, 油泵12压力传感器量程
  d_Pump12_MACHSET_BASEPRES,                           // 0x0fc20007, 油泵12底压
  d_Pump12_MACHSET_BASEFLOW,                           // 0x0fc20008, 油泵12底流
  d_Pump12_MACHSET_PRESS_ACC,                          // 0x0fc20009, 油泵12最小压力上升时间
  d_Pump12_MACHSET_PRESS_DEC,                          // 0x0fc2000a, 油泵12最小压力下降时间
  d_Pump12_MACHSET_PRESS_ACC_FILTER,                   // 0x0fc2000b, 油泵12压力上升滤波时间
  d_Pump12_MACHSET_PRESS_DEC_FILTER,                   // 0x0fc2000c, 油泵12压力下降滤波时间
  d_Pump12_MACHSET_SPEED_ACC,                          // 0x0fc2000d, 油泵12最小速度上升时间
  d_Pump12_MACHSET_SPEED_DEC,                          // 0x0fc2000e, 油泵12最小速度下降时间
  d_Pump12_MACHSET_ECAT_CODE_CHK_H,                    // 0x0fc2000f, 油泵12编码校对高位
  d_Pump12_MACHSET_ECAT_CODE_CHK_L,                    // 0x0fc20010, 油泵12编码校对低位
  d_Pump12_MACHSET_REVERSEFLOWMAX,                     // 0x0fc20011, 油泵12最大反向速度
  d_Pump12_MACHSET_TORQUEMAX_SLAVE_STOP,               // 0x0fc20012, 油泵12从泵停转扭矩
  d_Pump12_MACHSET_MODE_SLAVE_FOLLOW                   // 0x0fc20013, 油泵12从站跟随模式
};

enum db_Pump12_MOLDSET{
  d_Pump12_MOLDSET_PUMP_SLAVE_START = 0x0fc30000, // 0x0fc30000, 油泵12从站跟随启动速度
  d_Pump12_MOLDSET_PUMP_SLAVE_STOP                // 0x0fc30001, 油泵12从站跟随停止速度
};

enum db_Pump12_STATE{
  d_Pump12_STATE_FL_ERROR0 = 0x0fc60000, // 0x0fc60000, 警报1
  d_Pump12_STATE_FL_ERROR1,              // 0x0fc60001, 警报2
  d_Pump12_STATE_MOTOR_CURRENT,          // 0x0fc60002, 电机电流
  d_Pump12_STATE_MOTOR_VOLTAGE,          // 0x0fc60003, 电机电压
  d_Pump12_STATE_MOTOR_POWER,            // 0x0fc60004, 电机功率
  d_Pump12_STATE_MOTOR_TEMP,             // 0x0fc60005, 电机温度
  d_Pump12_STATE_MOTOR_SPEED_REAL,       // 0x0fc60006, 电机实际转速
  d_Pump12_STATE_MOTOR_SPEED_SET,        // 0x0fc60007, 电机指令速度
  d_Pump12_STATE_MOTOR_TORQUE_REAL,      // 0x0fc60008, 电机实际转矩
  d_Pump12_STATE_SERVO_STATE,            // 0x0fc60009, 伺服状态
  d_Pump12_STATE_SERVO_ERR,              // 0x0fc6000a, 伺服警报
  d_Pump12_STATE_SERVO_TEMP,             // 0x0fc6000b, 驱动器模块温度
  d_Pump12_STATE_SERVO_PRESS,            // 0x0fc6000c, 实际油箱压力
  d_Pump12_STATE_ENCODE_POS,             // 0x0fc6000d, 编码器位置
  d_Pump12_STATE_SERVO_COMM_ST,          // 0x0fc6000e, 伺服通讯状态
  d_Pump12_STATE_SERVO_COMM_STH          // 0x0fc6000f, 伺服通讯状态
};

enum db_inject1_PO{
  d_inject1_PO_O_INJ_SOL = 0x10100000, // 0x10100000, 射出
  d_inject1_PO_O_INJ_FST,              // 0x10100001, 射出快速
  d_inject1_PO_O_IN_SETU,              // 0x10100002, 射出设定
  d_inject1_PO_O_IN_HOLD,              // 0x10100003, 保压
  d_inject1_PO_O_PASS_VLV,             // 0x10100004, 旁路阀
  d_inject1_PO_O_INJ_ACC,              // 0x10100005, 射出增压
  d_inject1_PO_O_INJ_SRV,              // 0x10100006, 射出备用
  d_inject1_PO_O_CHR_SOL,              // 0x10100007, 储料
  d_inject1_PO_O_CHR_BAC,              // 0x10100008, 储料背压
  d_inject1_PO_O_CHR_REL,              // 0x10100009, 储料泄压
  d_inject1_PO_O_CHR_FST,              // 0x1010000a, 储料快速
  d_inject1_PO_O_CHR_SET,              // 0x1010000b, 储料设定
  d_inject1_PO_O_CHR_SRV,              // 0x1010000c, 储料备用
  d_inject1_PO_O_SUC_SOL,              // 0x1010000d, 射退
  d_inject1_PO_O_SUC_SET,              // 0x1010000e, 射退设定
  d_inject1_PO_O_SUC_SRV,              // 0x1010000f, 射退备用
  d_inject1_PO_O_CHR_RL2,              // 0x10100010, 储料0背压
  d_inject1_PO_O_INH_CHR,              // 0x10100011, 射出储料
  d_inject1_PO_O_SYN_CHG,              // 0x10100012, 储料联动
  d_inject1_PO_O_INJ_SPC,              // 0x10100013, 胶口针阀
  d_inject1_PO_O_CHG_SUC,              // 0x10100014, 储料射退
  d_inject1_PO_O_INJ_CHGSUCK,          // 0x10100015, 射出储料射退
  d_inject1_PO_O_INJ_AG,               // 0x10100016, 射出差动
  d_inject1_PO_O_HLD_REL,              // 0x10100017, 保压解压
  d_inject1_PO_O_INJ_OPCL,             // 0x10100018, 射出开关模
  d_inject1_PO_O_INJ_OPEN,             // 0x10100019, 射出开环
  d_inject1_PO_O_CHG_RELOIL,           // 0x1010001a, 储料油马达泄油阀
  d_inject1_PO_O_SPC_INJ,              // 0x1010001b, 特殊射出阀
  d_inject1_PO_O_SPC_CHG,              // 0x1010001c, 特殊储料阀
  d_inject1_PO_O_MOLD_SPC,             // 0x1010001d, 模具封口
  d_inject1_PO_O_INJ_ASSIST,           // 0x1010001e, 射出辅助阀
  d_inject1_PO_O_INJ_SUCK,             // 0x1010001f, 射出射退
  d_inject1_PO_O_INJ_SOL2,             // 0x10100020, 射出2
  d_inject1_PO_O_INJ_SOL3,             // 0x10100021, 射出3
  d_inject1_PO_O_CHR_SOL2,             // 0x10100022, 储料2
  d_inject1_PO_O_SUC_SOL2,             // 0x10100023, 射退2
  d_inject1_PO_0_OPEN_CHG,             // 0x10100024, 开储联动
  d_inject1_PO_0_INJ_SPC2              // 0x10100025, 胶口针阀2
};

enum db_inject1_PI{
  d_inject1_PI_I_PUR_GUA = 0x10110000, // 0x10110000, 射出防护
  d_inject1_PI_I_INJ_HLD,              // 0x10110001, 射出保压
  d_inject1_PI_I_INJ_1ST,              // 0x10110002, 射出二段
  d_inject1_PI_I_INJ_2ND,              // 0x10110003, 射出三段
  d_inject1_PI_I_CHR_END,              // 0x10110004, 储料终
  d_inject1_PI_I_SUC_END,              // 0x10110005, 射退终
  d_inject1_PI_I_CHR_PRS,              // 0x10110006, 储压检知
  d_inject1_PI_I_HOP_MAT,              // 0x10110007, 料斗检知
  d_inject1_PI_I_RPM_CHK               // 0x10110008, 螺杆转速检知
};

enum db_inject1_MACHSET{
  d_inject1_MACHSET_INJECT_TEMPCH_1 = 0x10120000, // 0x10120000, 料筒温度通道编号1
  d_inject1_MACHSET_INJECT_TEMPCH_2,              // 0x10120001, 料筒温度通道编号2
  d_inject1_MACHSET_INJECT_TEMPCH_3,              // 0x10120002, 料筒温度通道编号3
  d_inject1_MACHSET_INJECT_TEMPCH_4,              // 0x10120003, 料筒温度通道编号4
  d_inject1_MACHSET_INJECT_TEMPCH_5,              // 0x10120004, 料筒温度通道编号5
  d_inject1_MACHSET_INJECT_TEMPCH_6,              // 0x10120005, 料筒温度通道编号6
  d_inject1_MACHSET_INJECT_TEMPCH_7,              // 0x10120006, 料筒温度通道编号7
  d_inject1_MACHSET_INJECT_TEMPCH_8,              // 0x10120007, 料筒温度通道编号8
  d_inject1_MACHSET_INJECT_TEMPCH_9,              // 0x10120008, 料筒温度通道编号9
  d_inject1_MACHSET_INJECT_TEMPCH_10,             // 0x10120009, 料筒温度通道编号10
  d_inject1_MACHSET_INJECT_TEMPCH_11,             // 0x1012000a, 料筒温度通道编号11
  d_inject1_MACHSET_INJECT_TEMPCH_12,             // 0x1012000b, 料筒温度通道编号12
  d_inject1_MACHSET_INJECT_TEMPCH_13,             // 0x1012000c, 料筒温度通道编号13
  d_inject1_MACHSET_INJECT_TEMPCH_14,             // 0x1012000d, 料筒温度通道编号14
  d_inject1_MACHSET_INJ_METERCH,                  // 0x1012000e, 料筒温度通道编号15
  d_inject1_MACHSET_DAPRES_INJRAMP,               // 0x1012000f, 射出启动压力斜率
  d_inject1_MACHSET_DAFLOW_INJRAMP,               // 0x10120010, 射出启动速度斜率
  d_inject1_MACHSET_DAPRES_INJRAMP2,              // 0x10120011, 射出转换压力斜率 
  d_inject1_MACHSET_DAFLOW_INJRAMP2,              // 0x10120012, 射出转换速度斜率
  d_inject1_MACHSET_DAPRES_INJHOLDRAMP,           // 0x10120013, 保压转换压力斜率
  d_inject1_MACHSET_DAFLOW_INJHOLDRAMP,           // 0x10120014, 保压转换速度斜率
  d_inject1_MACHSET_DAPRES_INJOFFRAMP,            // 0x10120015, 射出关闭压力斜率
  d_inject1_MACHSET_DAFLOW_INJOFFRAMP,            // 0x10120016, 射出关闭速度斜率
  d_inject1_MACHSET_DAPRES_INJSETUP,              // 0x10120017, 射出慢速调试压力
  d_inject1_MACHSET_DAFLOW_INJSETUP,              // 0x10120018, 射出慢速调试速度
  d_inject1_MACHSET_DAPRES_CHGRAMP,               // 0x10120019, 储料启动压力斜率
  d_inject1_MACHSET_DAFLOW_CHGRAMP,               // 0x1012001a, 储料启动速度斜率
  d_inject1_MACHSET_DAPRES_CHGOFFRAMP,            // 0x1012001b, 储料关闭压力斜率
  d_inject1_MACHSET_DAFLOW_CHGOFFRAMP,            // 0x1012001c, 储料关闭速度斜率
  d_inject1_MACHSET_DAPRES_CHGSETUP,              // 0x1012001d, 储料射退慢速调试压力
  d_inject1_MACHSET_DAFLOW_CHGSETUP,              // 0x1012001e, 储料射退慢速调试速度
  d_inject1_MACHSET_DAPRES_SUCKBACKRAMP,          // 0x1012001f, 射退启动压力斜率
  d_inject1_MACHSET_DAFLOW_SUCKBACKRAMP,          // 0x10120020, 射退启动速度斜率
  d_inject1_MACHSET_DAPRES_SUCKBACKOFFRAMP,       // 0x10120021, 射退关闭压力斜率
  d_inject1_MACHSET_DAFLOW_SUCKBACKOFFRAMP,       // 0x10120022, 射退关闭速度斜率
  d_inject1_MACHSET_TMDLY_INJVALVEON,             // 0x10120023, 射出阀延迟开计时
  d_inject1_MACHSET_TMDLY_VLV_OFFINJEND,          // 0x10120024, 射出阀延迟关计时
  d_inject1_MACHSET_TMDLY_CHGUSEVALVE,            // 0x10120025, 储料阀延迟开计时
  d_inject1_MACHSET_TMDLY_VLVOFFCHGEND,           // 0x10120026, 储料阀延迟关计时
  d_inject1_MACHSET_TMDLY_VLVOFFSUCKBACK,         // 0x10120027, 射退阀延迟开计时
  d_inject1_MACHSET_TMDLY_VLVOFFSUCKBACKEND,      // 0x10120028, 射退阀延迟关计时
  d_inject1_MACHSET_TMDLY_INJEND,                 // 0x10120029, 射出动作结束延时
  d_inject1_MACHSET_FL_CUSHIONFUNCTION,           // 0x1012002a, 射出监控
  d_inject1_MACHSET_INJ_HYDELAY,                  // 0x1012002b, 射出液压延迟开计时
  d_inject1_MACHSET_HOLD_HYDELAY,                 // 0x1012002c, 保压液压延迟开计时
  d_inject1_MACHSET_CHG_HYDELAY,                  // 0x1012002d, 储料液压延迟开计时
  d_inject1_MACHSET_SUCKBACK_HYDELAY,             // 0x1012002e, 射退液压延迟开计时
  d_inject1_MACHSET_TMDLY_VLV_ONCHG,              // 0x1012002f, 储料阀延迟开计时
  d_inject1_MACHSET_TMDLY_VLV_ONSUCKBK,           // 0x10120030, 射退阀延迟开计时
  d_inject1_MACHSET_TMDLY_CHGEND,                 // 0x10120031, 储料动作结束延时
  d_inject1_MACHSET_TMDLY_SUCKBACKEND,            // 0x10120032, 射退动作结束延时
  d_inject1_MACHSET_TMDLY_INJBEFORE,              // 0x10120033, 射出任务前延时
  d_inject1_MACHSET_DAFLOW_VLVLIMIT,              // 0x10120034, 合流储料速度限制
  d_inject1_MACHSET_FL_MANUALINJECTUSEPRES_FLOW,  // 0x10120035, 手动使用调模压力流量
  d_inject1_MACHSET_TM_EXINJTM,                   // 0x10120036, 排气时射出时间
  d_inject1_MACHSET_POS_EXINJTM,                  // 0x10120037, 排气时射出位置
  d_inject1_MACHSET_Tm_HLD_REL,                   // 0x10120038, 保压解压开时间
  d_inject1_MACHSET_Inj_DvdAdj,                   // 0x10120039, 注射分体调试
  d_inject1_MACHSET_TMDLY_SUCKBACKENDBEFORCHG,    // 0x1012003a, 储前射退结束延迟
  d_inject1_MACHSET_FL_INj_SPC,                   // 0x1012003b, 胶口针阀
  d_inject1_MACHSET_TM_INJSPC_OPNELR,             // 0x1012003c, 针阀提前开时间
  d_inject1_MACHSET_TM_INJSPC_CLSDLY,             // 0x1012003d, 针阀延迟关时间
  d_inject1_MACHSET_TM_RELVLV,                    // 0x1012003e, 工作时间
  d_inject1_MACHSET_DIA_CYLINDER_SUCK,            // 0x1012003f, 射退油缸直径
  d_inject1_MACHSET_DIA_PISDIA_SUCK,              // 0x10120040, 射退活塞杆直径
  d_inject1_MACHSET_EFFICIENCY_FACTOR_SUCK,       // 0x10120041, 射退效率系数
  d_inject1_MACHSET_EFFICIENCY_FACTOR_INJ,        // 0x10120042, 射出效率系数
  d_inject1_MACHSET_DIA_LEFT_RIGHT,               // 0x10120043, 射退活塞杆
  d_inject1_MACHSET_NUM_CYLINDER_SUCK,            // 0x10120044, 射退油缸个数
  d_inject1_MACHSET_LOWPRES_SUCK,                 // 0x10120045, 射退底压
  d_inject1_MACHSET_LOWFLOW_SUCK,                 // 0x10120046, 射退底流
  d_inject1_MACHSET_SMOOTH_LEVEL_SUCK,            // 0x10120047, 射退平滑
  d_inject1_MACHSET_LOWPRES_INJ,                  // 0x10120048, 射出底压
  d_inject1_MACHSET_LOWFLOW_INJ,                  // 0x10120049, 射出底流
  d_inject1_MACHSET_SMOOTH_LEVEL_INJ,             // 0x1012004a, 射出平稳
  d_inject1_MACHSET_TM_EXINJTM_Hi,                // 0x1012004b, 排气时射出时间
  d_inject1_MACHSET_POS_INJ_SPC,                  // 0x1012004c, 针阀开阀位置
  d_inject1_MACHSET_FL_INJ_SPC_FUNC,              // 0x1012004d, 针阀控制方式
  d_inject1_MACHSET_TM_CHARGE,                    // 0x1012004e, 储料时间
  d_inject1_MACHSET_FL_CHARGEBYTM,                // 0x1012004f, 储料走时间
  d_inject1_MACHSET_FL_CHR_RL2_SUK,               // 0x10120050, 射退开储料零背压阀
  d_inject1_MACHSET_FL_SPC_INJ,                   // 0x10120051, 特殊射出阀
  d_inject1_MACHSET_FL_SPC_CHG,                   // 0x10120052, 特殊储料阀
  d_inject1_MACHSET_FL_CHG_CHECKMM,               // 0x10120053, 储前电子尺检测
  d_inject1_MACHSET_FL_CHG_NOLLEV,                // 0x10120054, 储料开座进阀
  d_inject1_MACHSET_FL_INJ_PROV,                  // 0x10120055, 射出射退模式
  d_inject1_MACHSET_INJ_SPC_FUNC_CLS,             // 0x10120056, 针阀关闭方式
  d_inject1_MACHSET_FL_INJCHAB_UPPRS,             // 0x10120057, 射出开环压力超调
  d_inject1_MACHSET_FL_ELECTRIC_CHG,              // 0x10120058, 电储料功能
  d_inject1_MACHSET_FL_ELECTRIC_NOZFWD,           // 0x10120059, 电储料同步座进
  d_inject1_MACHSET_FL_INJPRESS,                  // 0x1012005a, 射出压力独立检测
  d_inject1_MACHSET_INJECT_PRESSCH,               // 0x1012005b, 射出压力传感器通道
  d_inject1_MACHSET_FL_INJSPC_AIR,                // 0x1012005c, 针阀同步吹气点1
  d_inject1_MACHSET_FL_INJSPC_AIR1,               // 0x1012005d, 针阀同步吹气点2
  d_inject1_MACHSET_TM_INJSPC_CLSBEF,             // 0x1012005e, 针阀提前关时间
  d_inject1_MACHSET_INJECTVLV_CH,                 // 0x1012005f, 射出比例阀硬件通道
  d_inject1_MACHSET_INJECTVLV_DIR,                // 0x10120060, 射出比例阀输出电压
  d_inject1_MACHSET_SUCKBACKVLV_CH,               // 0x10120061, 射退比例阀硬件通道
  d_inject1_MACHSET_SUCKBACKVLV_DIR,              // 0x10120062, 退出比例阀输出电压
  d_inject1_MACHSET_FL_INJECTVLV,                 // 0x10120063, 射出比例阀功能
  d_inject1_MACHSET_FL_SUCKBACKVLV,               // 0x10120064, 射退比例阀功能
  d_inject1_MACHSET_ACTOPEN_CH,                   // 0x10120065, 动作开环通道
  d_inject1_MACHSET_TM_WARN_PURGE,                // 0x10120066, 清料提醒时间
  d_inject1_MACHSET_FL_ELECTRIC_SUCK,             // 0x10120067, 电储料开模联动射退
  d_inject1_MACHSET_FL_ELECCHG_HYDR,              // 0x10120068, 电储料液压
  d_inject1_MACHSET_DAPRES_ELECCHG_HYDR,          // 0x10120069, 电储料液压压力
  d_inject1_MACHSET_DAFLOW_ELECCHG_HYDR,          // 0x1012006a, 电储料液压速度
  d_inject1_MACHSET_PORT_ELECCHG_HYDR,            // 0x1012006b, 电储料液压通道
  d_inject1_MACHSET_PORT_ELECCHG_DA,              // 0x1012006c, 电储料通道
  d_inject1_MACHSET_FL_ELECTRIC_CHG_MODE,         // 0x1012006d, 电储料输出方式
  d_inject1_MACHSET_TM_INJSPC_OPNDLY,             // 0x1012006e, 针阀延迟开时间
  d_inject1_MACHSET_FL_INj_SPC2,                  // 0x1012006f, 胶口针阀2
  d_inject1_MACHSET_FL_INJ_SPC_FUNC2,             // 0x10120070, 针阀2控制方式
  d_inject1_MACHSET_POS_INJ_SPC2,                 // 0x10120071, 胶口针阀2开阀位置
  d_inject1_MACHSET_TM_INJSPC_OPNELR2,            // 0x10120072, 针阀2提前开时间
  d_inject1_MACHSET_TM_INJSPC_OPNDLY2,            // 0x10120073, 针阀2延迟开时间
  d_inject1_MACHSET_INJ_SPC_FUNC_CLS2,            // 0x10120074, 针阀2关闭方式
  d_inject1_MACHSET_TM_INJSPC_CLSBEF2,            // 0x10120075, 针阀2提前关时间
  d_inject1_MACHSET_TM_INJSPC_CLSDLY2             // 0x10120076, 针阀2延时关时间
};

enum db_inject1_MOLDSET{
  d_inject1_MOLDSET_FL_INCACC = 0x10130000, // 0x10130000, 射出增压功能
  d_inject1_MOLDSET_DAPRES_INJACC,          // 0x10130001, 蓄能器压力
  d_inject1_MOLDSET_FL_RELEASEHOLDPRES,     // 0x10130002, 泄压功能
  d_inject1_MOLDSET_ADPOSI_INJPOSUPCUSHION, // 0x10130003, 射出监控上限
  d_inject1_MOLDSET_ADPOSI_INJPOSNEGCUSHION, // 0x10130004, 射出监控下限
  d_inject1_MOLDSET_TM_INJBEFORENOZADV,     // 0x10130005, 座进前射出计时
  d_inject1_MOLDSET_ADPOSI_INJBEFORENOZADV, // 0x10130006, 座进前射出距离
  d_inject1_MOLDSET_FL_INJECTCOREOFF,       // 0x10130007, 射出时不开中子阀
  d_inject1_MOLDSET_TM_INJMOLDINTIME,       // 0x10130008, 压塑前射出一段计时
  d_inject1_MOLDSET_FL_TURNTOHOLD,          // 0x10130009, 保压转换方式
  d_inject1_MOLDSET_DAPRES_TURNTOHOLD,      // 0x1013000a, 保压转换压力
  d_inject1_MOLDSET_ADPOSI_TURNTOHOLD,      // 0x1013000b, 保压转换位置
  d_inject1_MOLDSET_TM_TURNTOHOLD,          // 0x1013000c, 保压转换时间
  d_inject1_MOLDSET_SPEED_TURNTOHOLD,       // 0x1013000d, 保压转换速度
  d_inject1_MOLDSET_FL_CHGBACK,             // 0x1013000e, 储料背压阀
  d_inject1_MOLDSET_FL_CHRGFAST,            // 0x1013000f, 储料快速阀
  d_inject1_MOLDSET_ADPOSI_CHGFLOWMOLD,     // 0x10130010, 再次储料位置
  d_inject1_MOLDSET_TM_CHGFLOWMOLDING,      // 0x10130011, 再次储料时间
  d_inject1_MOLDSET_FL_SUCKBACKMODE,        // 0x10130012, 射退模式
  d_inject1_MOLDSET_TM_SUCKBACKBEFORECHG,   // 0x10130013, 储前射退时间
  d_inject1_MOLDSET_TM_AUTOPURGE,           // 0x10130014, 清料时间
  d_inject1_MOLDSET_CN_AUTOPURGE,           // 0x10130015, 清料次数
  d_inject1_MOLDSET_FL_AUTOPURGE,           // 0x10130016, 自动清料功能
  d_inject1_MOLDSET_SEC_INJ,                // 0x10130017, 射出段数
  d_inject1_MOLDSET_SEC_CHG,                // 0x10130018, 储料段数
  d_inject1_MOLDSET_SEC_HOLD,               // 0x10130019, 保压段数
  d_inject1_MOLDSET_FL_INJFAST,             // 0x1013001a, 射出快速阀
  d_inject1_MOLDSET_FL_PASSVALUE,           // 0x1013001b, 射出盘路阀
  d_inject1_MOLDSET_FL_CHGMODE,             // 0x1013001c, 储料模式
  d_inject1_MOLDSET_TM_CHGDEFAULTMAX,       // 0x1013001d, 储料计时上限
  d_inject1_MOLDSET_TM_SUCKBACKDEFAULTMAX,  // 0x1013001e, 射退计时上限
  d_inject1_MOLDSET_FL_CHARGERELEASE,       // 0x1013001f, 储料泄压功能
  d_inject1_MOLDSET_FL_SUCKBACKBEFORECHG,   // 0x10130020, 储料前射退模式
  d_inject1_MOLDSET_POS_SUCKBACKBEFORECHG,  // 0x10130021, 储料前射退距离
  d_inject1_MOLDSET_TM_CHARGE_UP,           // 0x10130022, 储料计时上偏差
  d_inject1_MOLDSET_Qc_DeviationSet_InjTm,  // 0x10130023, 
  d_inject1_MOLDSET_Qc_TmSet_InjTm,         // 0x10130024, 
  d_inject1_MOLDSET_Qc_Mode_InjTm,          // 0x10130025, 
  d_inject1_MOLDSET_Qc_DeviationSet_InjSpd, // 0x10130026, 
  d_inject1_MOLDSET_Qc_TmSet_InjSpd,        // 0x10130027, 
  d_inject1_MOLDSET_Qc_Mode_InjSpd,         // 0x10130028, 
  d_inject1_MOLDSET_Qc_DeviationSet_InjEnd, // 0x10130029, 
  d_inject1_MOLDSET_Qc_TmSet_InjEnd,        // 0x1013002a, 
  d_inject1_MOLDSET_Qc_Mode_InjEnd,         // 0x1013002b, 
  d_inject1_MOLDSET_Qc_DeviationSet_InjPrs, // 0x1013002c, 
  d_inject1_MOLDSET_Qc_TmSet_InjPrs,        // 0x1013002d, 
  d_inject1_MOLDSET_Qc_Mode_InjPrs,         // 0x1013002e, 
  d_inject1_MOLDSET_Qc_DeviationSet_HoldEnd, // 0x1013002f, 
  d_inject1_MOLDSET_Qc_TmSet_HoldEnd,       // 0x10130030, 
  d_inject1_MOLDSET_Qc_Mode_HoldEnd,        // 0x10130031, 
  d_inject1_MOLDSET_Qc_DeviationSet_ChgTm,  // 0x10130032, 
  d_inject1_MOLDSET_Qc_TmSet_ChgTm,         // 0x10130033, 
  d_inject1_MOLDSET_Qc_Mode_ChgTm,          // 0x10130034, 
  d_inject1_MOLDSET_Qc_DeviationSet_ChgEnd, // 0x10130035, 
  d_inject1_MOLDSET_Qc_TmSet_ChgEnd,        // 0x10130036, 
  d_inject1_MOLDSET_Qc_Mode_ChgEnd,         // 0x10130037, 
  d_inject1_MOLDSET_Qc_DeviationSet_SuckBwd, // 0x10130038, 
  d_inject1_MOLDSET_Qc_TmSet_SuckBwd,       // 0x10130039, 
  d_inject1_MOLDSET_Qc_Mode_SuckBwd,        // 0x1013003a, 
  d_inject1_MOLDSET_Qc_DeviationSet_SuckTm, // 0x1013003b, 
  d_inject1_MOLDSET_Qc_TmSet_SuckTm,        // 0x1013003c, 
  d_inject1_MOLDSET_Qc_Mode_SuckTm,         // 0x1013003d, 
  d_inject1_MOLDSET_Inj_Open,               // 0x1013003e, 射出开环
  d_inject1_MOLDSET_Hold_Open,              // 0x1013003f, 保压开环
  d_inject1_MOLDSET_Chg_Open,               // 0x10130040, 储料开环
  d_inject1_MOLDSET_Suck_BefChg,            // 0x10130041, 储前射退保护距离
  d_inject1_MOLDSET_FL_InjectWarmOn,        // 0x10130042, 电热关闭时螺杆动作
  d_inject1_MOLDSET_FL_CHGINJCNT,           // 0x10130043, 多次储料射出次数
  d_inject1_MOLDSET_POS_CHGINJCNT,          // 0x10130044, 多次储料位置
  d_inject1_MOLDSET_SLOPE_MAX_SUCK_SPD,     // 0x10130045, 射退最大速度斜率
  d_inject1_MOLDSET_SLOPE_MAX_SUCK_PRES,    // 0x10130046, 射退最大压力斜率
  d_inject1_MOLDSET_FL_SUCK_SOOMTH,         // 0x10130047, 射退平稳
  d_inject1_MOLDSET_PUMP_DEAD_SUCK,         // 0x10130048, 射退死区时间
  d_inject1_MOLDSET_PUMP_K_SUCK,            // 0x10130049, 射退减速调节系数
  d_inject1_MOLDSET_SUCK_PRES_STARTON,      // 0x1013004a, 射退启动压力斜率
  d_inject1_MOLDSET_SUCK_PRES_HALFWAY,      // 0x1013004b, 射退中间压力斜率
  d_inject1_MOLDSET_SUCK_PRES_STOPOFF,      // 0x1013004c, 射退终止压力斜率
  d_inject1_MOLDSET_SUCK_SPD_STARTON,       // 0x1013004d, 射退启动速度斜率
  d_inject1_MOLDSET_SUCK_SPD_HALFWAY,       // 0x1013004e, 射退中间速度斜率
  d_inject1_MOLDSET_SUCK_SPD_STOPOFF,       // 0x1013004f, 射退终止速度斜率
  d_inject1_MOLDSET_SLOPE_MAX_INJ_SPD,      // 0x10130050, 射出最大速度斜率
  d_inject1_MOLDSET_SLOPE_MAX_INJ_PRES,     // 0x10130051, 射出最大压力斜率
  d_inject1_MOLDSET_FL_INJ_SOOMTH,          // 0x10130052, 射出平稳
  d_inject1_MOLDSET_PUMP_DEAD_INJ,          // 0x10130053, 射出死区时间
  d_inject1_MOLDSET_PUMP_K_INJ,             // 0x10130054, 射出减速调节系数
  d_inject1_MOLDSET_INJ_PRES_STARTON,       // 0x10130055, 射出启动压力斜率
  d_inject1_MOLDSET_INJ_PRES_HALFWAY,       // 0x10130056, 射出中间压力斜率
  d_inject1_MOLDSET_INJ_PRES_STOPOFF,       // 0x10130057, 射出终止压力斜率
  d_inject1_MOLDSET_INJ_SPD_STARTON,        // 0x10130058, 射出启动速度斜率
  d_inject1_MOLDSET_INJ_SPD_HALFWAY,        // 0x10130059, 射出中间速度斜率
  d_inject1_MOLDSET_INJ_SPD_STOPOFF,        // 0x1013005a, 射出终止速度斜率
  d_inject1_MOLDSET_TM_CHGDEFAULTMAX_Hi,    // 0x1013005b, 储料计时警报上限
  d_inject1_MOLDSET_HYDY_WAY_SET,           // 0x1013005c, 油路切换选择
  d_inject1_MOLDSET_FL_CHGMOVE,             // 0x1013005d, 储料移动检查
  d_inject1_MOLDSET_TM_CHGMOVE,             // 0x1013005e, 储料移动时间
  d_inject1_MOLDSET_POS_CHGMOVE,            // 0x1013005f, 储料移动距离
  d_inject1_MOLDSET_SPEED_LIMIT,            // 0x10130060, 手动储料速度限制
  d_inject1_MOLDSET_FL_AUTOPURGE_SPC,       // 0x10130061, 清料方式
  d_inject1_MOLDSET_POS_AUTOPURGE,          // 0x10130062, 自动清料储料位置
  d_inject1_MOLDSET_TM_AUTOINJ,             // 0x10130063, 自动清料注射时间
  d_inject1_MOLDSET_INJPRES_SETMODE,        // 0x10130064, 射出压力模式
  d_inject1_MOLDSET_HOLDSPD_SETMODE,        // 0x10130065, 保压速度模式
  d_inject1_MOLDSET_INJ_SPD_UTI,            // 0x10130066, 射出速度使用率
  d_inject1_MOLDSET_FL_INJSPCMAUAL,         // 0x10130067, 手动射出输出针阀
  d_inject1_MOLDSET_FL_INJECTWARMPRE,       // 0x10130068, 不检测料筒温度
  d_inject1_MOLDSET_FL_AUTOPURGESUCK,       // 0x10130069, 清料储料前射退
  d_inject1_MOLDSET_POS_SUTOPURGEINJE,      // 0x1013006a, 清料射出终止位置
  d_inject1_MOLDSET_FL_PET,                 // 0x1013006b, 机器材料选择
  d_inject1_MOLDSET_FL_PREVENTCOOL,         // 0x1013006c, 防冷料功能
  d_inject1_MOLDSET_FL_INJSPCADJUST,        // 0x1013006d, 调模射出输出针阀
  d_inject1_MOLDSET_FL_INJSPCPURG           // 0x1013006e, 清料射出输出针阀
};

enum db_inject1_ACTPARA{
  d_inject1_ACTPARA_DAPRES_INJ1 = 0x10140000, // 0x10140000, 射出一段压力
  d_inject1_ACTPARA_DAPRES_INJ2,              // 0x10140001, 射出二段压力 
  d_inject1_ACTPARA_DAPRES_INJ3,              // 0x10140002, 射出三段压力 
  d_inject1_ACTPARA_DAPRES_INJ4,              // 0x10140003, 射出四段压力 
  d_inject1_ACTPARA_DAPRES_INJ5,              // 0x10140004, 射出五段压力 
  d_inject1_ACTPARA_DAPRES_INJ6,              // 0x10140005, 射出六段压力 
  d_inject1_ACTPARA_DAPRES_INJ7,              // 0x10140006, 射出七段压力 
  d_inject1_ACTPARA_DAPRES_INJ8,              // 0x10140007, 射出八段压力 
  d_inject1_ACTPARA_DAPRES_INJ9,              // 0x10140008, 射出九段压力 
  d_inject1_ACTPARA_DAPRES_INJ10,             // 0x10140009, 射出十段压力
  d_inject1_ACTPARA_DAPRES_INJHOLD1,          // 0x1014000a, 保压一段压力
  d_inject1_ACTPARA_DAPRES_INJHOLD2,          // 0x1014000b, 保压二段压力
  d_inject1_ACTPARA_DAPRES_INJHOLD3,          // 0x1014000c, 保压三段压力
  d_inject1_ACTPARA_DAPRES_INJHOLD4,          // 0x1014000d, 保压四段压力
  d_inject1_ACTPARA_DAPRES_INJHOLD5,          // 0x1014000e, 保压五段压力
  d_inject1_ACTPARA_DAPRES_INJHOLD6,          // 0x1014000f, 保压六段压力
  d_inject1_ACTPARA_DAPRES_INJHOLD7,          // 0x10140010, 保压七段压力
  d_inject1_ACTPARA_DAPRES_INJHOLD8,          // 0x10140011, 保压八段压力
  d_inject1_ACTPARA_DAPRES_INJHOLD9,          // 0x10140012, 保压九段压力
  d_inject1_ACTPARA_DAPRES_INJHOLD10,         // 0x10140013, 保压十段压力
  d_inject1_ACTPARA_DAFLOW_INJ1,              // 0x10140014, 射出一段速度
  d_inject1_ACTPARA_DAFLOW_INJ2,              // 0x10140015, 射出二段速度
  d_inject1_ACTPARA_DAFLOW_INJ3,              // 0x10140016, 射出三段速度
  d_inject1_ACTPARA_DAFLOW_INJ4,              // 0x10140017, 射出四段速度
  d_inject1_ACTPARA_DAFLOW_INJ5,              // 0x10140018, 射出五段速度
  d_inject1_ACTPARA_DAFLOW_INJ6,              // 0x10140019, 射出六段速度
  d_inject1_ACTPARA_DAFLOW_INJ7,              // 0x1014001a, 射出七段速度
  d_inject1_ACTPARA_DAFLOW_INJ8,              // 0x1014001b, 射出八段速度
  d_inject1_ACTPARA_DAFLOW_INJ9,              // 0x1014001c, 射出九段速度
  d_inject1_ACTPARA_DAFLOW_INJ10,             // 0x1014001d, 射出十段速度
  d_inject1_ACTPARA_DAFLOW_INJHOLD1,          // 0x1014001e, 保压一段速度
  d_inject1_ACTPARA_DAFLOW_INJHOLD2,          // 0x1014001f, 保压二段速度
  d_inject1_ACTPARA_DAFLOW_INJHOLD3,          // 0x10140020, 保压三段速度
  d_inject1_ACTPARA_DAFLOW_INJHOLD4,          // 0x10140021, 保压四段速度
  d_inject1_ACTPARA_DAFLOW_INJHOLD5,          // 0x10140022, 保压五段速度
  d_inject1_ACTPARA_DAFLOW_INJHOLD6,          // 0x10140023, 保压六段速度
  d_inject1_ACTPARA_DAFLOW_INJHOLD7,          // 0x10140024, 保压七段速度
  d_inject1_ACTPARA_DAFLOW_INJHOLD8,          // 0x10140025, 保压八段速度
  d_inject1_ACTPARA_DAFLOW_INJHOLD9,          // 0x10140026, 保压九段速度
  d_inject1_ACTPARA_DAFLOW_INJHOLD10,         // 0x10140027, 保压十段速度
  d_inject1_ACTPARA_TM_INJ1,                  // 0x10140028, 射出一段时间
  d_inject1_ACTPARA_TM_INJ2,                  // 0x10140029, 射出二段时间
  d_inject1_ACTPARA_TM_INJ3,                  // 0x1014002a, 射出三段时间
  d_inject1_ACTPARA_TM_INJ4,                  // 0x1014002b, 射出四段时间
  d_inject1_ACTPARA_TM_INJ5,                  // 0x1014002c, 射出五段时间
  d_inject1_ACTPARA_TM_INJ6,                  // 0x1014002d, 射出六段时间
  d_inject1_ACTPARA_TM_INJ7,                  // 0x1014002e, 射出七段时间
  d_inject1_ACTPARA_TM_INJ8,                  // 0x1014002f, 射出八段时间
  d_inject1_ACTPARA_TM_INJ9,                  // 0x10140030, 射出九段时间
  d_inject1_ACTPARA_TM_INJ10,                 // 0x10140031, 射出十段时间
  d_inject1_ACTPARA_TM_INJHOLD1,              // 0x10140032, 保压一段时间
  d_inject1_ACTPARA_TM_INJHOLD2,              // 0x10140033, 保压二段时间
  d_inject1_ACTPARA_TM_INJHOLD3,              // 0x10140034, 保压三段时间
  d_inject1_ACTPARA_TM_INJHOLD4,              // 0x10140035, 保压四段时间
  d_inject1_ACTPARA_TM_INJHOLD5,              // 0x10140036, 保压五段时间
  d_inject1_ACTPARA_TM_INJHOLD6,              // 0x10140037, 保压六段时间
  d_inject1_ACTPARA_TM_INJHOLD7,              // 0x10140038, 保压七段时间
  d_inject1_ACTPARA_TM_INJHOLD8,              // 0x10140039, 保压八段时间
  d_inject1_ACTPARA_TM_INJHOLD9,              // 0x1014003a, 保压九段时间
  d_inject1_ACTPARA_TM_INJHOLD10,             // 0x1014003b, 保压十段时间
  d_inject1_ACTPARA_ADPOSI_INJ1END,           // 0x1014003c, 射出一段终止位置
  d_inject1_ACTPARA_ADPOSI_INJ2END,           // 0x1014003d, 射出二段终止位置
  d_inject1_ACTPARA_ADPOSI_INJ3END,           // 0x1014003e, 射出三段终止位置
  d_inject1_ACTPARA_ADPOSI_INJ4END,           // 0x1014003f, 射出四段终止位置
  d_inject1_ACTPARA_ADPOSI_INJ5END,           // 0x10140040, 射出五段终止位置
  d_inject1_ACTPARA_ADPOSI_INJ6END,           // 0x10140041, 射出六段终止位置
  d_inject1_ACTPARA_ADPOSI_INJ7END,           // 0x10140042, 射出七段终止位置
  d_inject1_ACTPARA_ADPOSI_INJ8END,           // 0x10140043, 射出八段终止位置
  d_inject1_ACTPARA_ADPOSI_INJ9END,           // 0x10140044, 射出九段终止位置
  d_inject1_ACTPARA_ADPOSI_INJ10END,          // 0x10140045, 射出十段终止位置
  d_inject1_ACTPARA_DAPRES_CHG1,              // 0x10140046, 储料一段压力
  d_inject1_ACTPARA_DAPRES_CHG2,              // 0x10140047, 储料二段压力  
  d_inject1_ACTPARA_DAPRES_CHG3,              // 0x10140048, 储料三段压力  
  d_inject1_ACTPARA_DAPRES_CHG4,              // 0x10140049, 储料四段压力  
  d_inject1_ACTPARA_DAPRES_CHG5,              // 0x1014004a, 储料五段压力  
  d_inject1_ACTPARA_DAPRES_CHGBACK1,          // 0x1014004b, 储料一段背压
  d_inject1_ACTPARA_DAPRES_CHGBACK2,          // 0x1014004c, 储料二段背压
  d_inject1_ACTPARA_DAPRES_CHGBACK3,          // 0x1014004d, 储料三段背压
  d_inject1_ACTPARA_DAPRES_CHGBACK4,          // 0x1014004e, 储料四段背压
  d_inject1_ACTPARA_DAPRES_CHGBACK5,          // 0x1014004f, 储料五段背压
  d_inject1_ACTPARA_DAFLOW_CHG1,              // 0x10140050, 储料一段速度
  d_inject1_ACTPARA_DAFLOW_CHG2,              // 0x10140051, 储料二段速度
  d_inject1_ACTPARA_DAFLOW_CHG3,              // 0x10140052, 储料三段速度
  d_inject1_ACTPARA_DAFLOW_CHG4,              // 0x10140053, 储料四段速度
  d_inject1_ACTPARA_DAFLOW_CHG5,              // 0x10140054, 储料五段速度
  d_inject1_ACTPARA_ADPOSI_CHG1END,           // 0x10140055, 储料一段终止位置
  d_inject1_ACTPARA_ADPOSI_CHG2END,           // 0x10140056, 储料二段终止位置
  d_inject1_ACTPARA_ADPOSI_CHG3END,           // 0x10140057, 储料三段终止位置
  d_inject1_ACTPARA_ADPOSI_CHG4END,           // 0x10140058, 储料四段终止位置
  d_inject1_ACTPARA_ADPOSI_CHG5END,           // 0x10140059, 储料五段终止位置
  d_inject1_ACTPARA_DAPRES_SUCKBACK,          // 0x1014005a, 射退压力
  d_inject1_ACTPARA_DAFLOW_SUCKBACK,          // 0x1014005b, 射退速度
  d_inject1_ACTPARA_ADPOSI_SUCKBACKEND,       // 0x1014005c, 射退位置
  d_inject1_ACTPARA_ADPOSI_SUCKBACKDISTANCE,  // 0x1014005d, 射退距离
  d_inject1_ACTPARA_TM_SUCKBACK,              // 0x1014005e, 射退计时
  d_inject1_ACTPARA_DAPRES_SUCKBACKBFCHG,     // 0x1014005f, 储料前射退压力
  d_inject1_ACTPARA_DAFLOW_SUCKBACKBFCHG,     // 0x10140060, 储料前射退速度
  d_inject1_ACTPARA_DAPRES_INJRAMPSLOW,       // 0x10140061, 射出慢速启动压力斜率
  d_inject1_ACTPARA_DAFLOW_INJRAMPSLOW,       // 0x10140062, 射出慢速启动速度斜率
  d_inject1_ACTPARA_DAPRES_INJSLOW,           // 0x10140063, 射出慢速启动压力
  d_inject1_ACTPARA_DAFLOW_INJSLOW,           // 0x10140064, 射出慢速启动速度
  d_inject1_ACTPARA_ADPOSI_INJSLOW,           // 0x10140065, 射出慢速启动距离
  d_inject1_ACTPARA_TM_INJSLOW,               // 0x10140066, 射出慢速启动时间
  d_inject1_ACTPARA_DAPRES_SUCKRAMPSLOW,      // 0x10140067, 射退慢速启动压力斜率
  d_inject1_ACTPARA_DAFLOW_SUCKRAMPSLOW,      // 0x10140068, 射退慢速启动速度斜率
  d_inject1_ACTPARA_DAPRES_SUCKSLOW,          // 0x10140069, 射退慢速启动压力
  d_inject1_ACTPARA_DAFLOW_SUCKSLOW,          // 0x1014006a, 射退慢速启动速度
  d_inject1_ACTPARA_ADPOSI_SUCKSLOW,          // 0x1014006b, 射退慢速启动距离
  d_inject1_ACTPARA_TM_SUCKSLOW,              // 0x1014006c, 射退慢速启动时间
  d_inject1_ACTPARA_DAPRES_CHGPURGE,          // 0x1014006d, 自动清料储料压力
  d_inject1_ACTPARA_DAFLOW_CHGPURGE,          // 0x1014006e, 自动清料储料速度
  d_inject1_ACTPARA_DAPRES_CHGBACKPUGRE,      // 0x1014006f, 自动清料储料背压
  d_inject1_ACTPARA_DAPRES_INJPURGE,          // 0x10140070, 自动清料射出压力
  d_inject1_ACTPARA_DAFLOW_INJPURGE,          // 0x10140071, 自动清料射出速度
  d_inject1_ACTPARA_TM_INJ1_H,                // 0x10140072, 射出一段时间
  d_inject1_ACTPARA_TM_INJ2_H,                // 0x10140073, 射出二段时间
  d_inject1_ACTPARA_TM_INJ3_H,                // 0x10140074, 射出三段时间
  d_inject1_ACTPARA_TM_INJ4_H,                // 0x10140075, 射出四段时间
  d_inject1_ACTPARA_TM_INJ5_H,                // 0x10140076, 射出五段时间
  d_inject1_ACTPARA_TM_INJ6_H,                // 0x10140077, 射出六段时间
  d_inject1_ACTPARA_TM_INJ7_H,                // 0x10140078, 射出七段时间
  d_inject1_ACTPARA_TM_INJ8_H,                // 0x10140079, 射出八段时间
  d_inject1_ACTPARA_TM_INJ9_H,                // 0x1014007a, 射出九段时间
  d_inject1_ACTPARA_TM_INJ10_H,               // 0x1014007b, 射出十段时间
  d_inject1_ACTPARA_TM_INJHOLD1_H,            // 0x1014007c, 保压一段时间
  d_inject1_ACTPARA_TM_INJHOLD2_H,            // 0x1014007d, 保压二段时间
  d_inject1_ACTPARA_TM_INJHOLD3_H,            // 0x1014007e, 保压三段时间
  d_inject1_ACTPARA_TM_INJHOLD4_H,            // 0x1014007f, 保压四段时间
  d_inject1_ACTPARA_TM_INJHOLD5_H,            // 0x10140080, 保压五段时间
  d_inject1_ACTPARA_TM_INJHOLD6_H,            // 0x10140081, 保压六段时间
  d_inject1_ACTPARA_TM_INJHOLD7_H,            // 0x10140082, 保压七段时间
  d_inject1_ACTPARA_TM_INJHOLD8_H,            // 0x10140083, 保压八段时间
  d_inject1_ACTPARA_TM_INJHOLD9_H,            // 0x10140084, 保压九段时间
  d_inject1_ACTPARA_TM_INJHOLD10_H,           // 0x10140085, 保压十段时间
  d_inject1_ACTPARA_PER_DAFLOW_INJECT,        // 0x10140086, 射出速度使用率
  d_inject1_ACTPARA_PER_DAFLOW_HOLD,          // 0x10140087, 保压速度使用率
  d_inject1_ACTPARA_PER_DAFLOW_CHARGE,        // 0x10140088, 储料速度使用率
  d_inject1_ACTPARA_PER_DAFLOW_SUCKBACK,      // 0x10140089, 射退速度使用率
  d_inject1_ACTPARA_ASSIST_OPEN_SPEED         // 0x1014008a, 射出辅助阀开启速度
};

enum db_inject1_HYDR{
  d_inject1_HYDR_HYDR_PORT_INJCT = 0x10150000, // 0x10150000, 
  d_inject1_HYDR_HYDR_PUMPS_INJCT,             // 0x10150001, 
  d_inject1_HYDR_HYDR_PUMPS_INJSETUP,          // 0x10150002, 
  d_inject1_HYDR_SERVO_KP_INJCT,               // 0x10150003, 射出 P
  d_inject1_HYDR_SERVO_KI_INJCT,               // 0x10150004, 射出 I
  d_inject1_HYDR_SERVO_KD_INJCT,               // 0x10150005, 射出 D
  d_inject1_HYDR_HYDR_PORT_HOLD,               // 0x10150006, 
  d_inject1_HYDR_HYDR_PUMPS_HOLD,              // 0x10150007, 
  d_inject1_HYDR_SERVO_KP_HOLD,                // 0x10150008, 保压 P
  d_inject1_HYDR_SERVO_KI_HOLD,                // 0x10150009, 保压 I
  d_inject1_HYDR_SERVO_KD_HOLD,                // 0x1015000a, 保压 D
  d_inject1_HYDR_HYDR_PORT_CHARGE,             // 0x1015000b, 
  d_inject1_HYDR_HYDR_PUMPS_CHARGE,            // 0x1015000c, 
  d_inject1_HYDR_SERVO_KP_CHARGE,              // 0x1015000d, 储料 P
  d_inject1_HYDR_SERVO_KI_CHARGE,              // 0x1015000e, 储料 I
  d_inject1_HYDR_SERVO_KD_CHARGE,              // 0x1015000f, 储料 D
  d_inject1_HYDR_HYDR_PORT_SUCKBACK,           // 0x10150010, 
  d_inject1_HYDR_HYDR_PUMPS_SUCKBACK,          // 0x10150011, 
  d_inject1_HYDR_SERVO_KP_SUCKBACK,            // 0x10150012, 射退 P
  d_inject1_HYDR_SERVO_KI_SUCKBACK,            // 0x10150013, 射退 I
  d_inject1_HYDR_SERVO_KD_SUCKBACK,            // 0x10150014, 射退 D
  d_inject1_HYDR_HYDR_PORT_CHRGBACK            // 0x10150015, 储料背压使用通道
};

enum db_inject1_STATE{
  d_inject1_STATE_FL_ERROR0 = 0x10160000, // 0x10160000, 警报1
  d_inject1_STATE_FL_ERROR1,              // 0x10160001, 警报2
  d_inject1_STATE_TM_INJ,                 // 0x10160002, 射出计时实际值
  d_inject1_STATE_ADPOSI_INJSTART,        // 0x10160003, 射出起点位置
  d_inject1_STATE_ADPOSI_INJHOLDEND,      // 0x10160004, 射出终点位置实际值
  d_inject1_STATE_ADPOSI_INJCUSHION,      // 0x10160005, 射出转保压位置
  d_inject1_STATE_ADPOSI_INJCUSHION_MAX,  // 0x10160006, 射出监控位置最大值
  d_inject1_STATE_DAPRES_TOHOLD,          // 0x10160007, 保压转换压力
  d_inject1_STATE_TM_TOHOLD,              // 0x10160008, 保压转换时间实际值
  d_inject1_STATE_ADPOSI_TOHOLD,          // 0x10160009, 保压转换位置实际值
  d_inject1_STATE_TM_CHARGE,              // 0x1016000a, 储料计时实际值
  d_inject1_STATE_TM_CHARGE_MAX,          // 0x1016000b, 储料计时最大值
  d_inject1_STATE_CN_CHRGSPD,             // 0x1016000c, 储料转速
  d_inject1_STATE_ADPOSI_CHGEND,          // 0x1016000d, 储料终点位置实际值
  d_inject1_STATE_TM_SUCKBACK1,           // 0x1016000e, 射退计时实际值
  d_inject1_STATE_TM_SUCKBACK1_MAX,       // 0x1016000f, 射退计时最大值
  d_inject1_STATE_SPEED_TOHOLD,           // 0x10160010, 转保压速度
  d_inject1_STATE_ADPOSI_SUCKBACK,        // 0x10160011, 射退终止位置实际值
  d_inject1_STATE_INJECT_SPEED,           // 0x10160012, 射出速度
  d_inject1_STATE_INJECT_AVERAGESPEED,    // 0x10160013, 射出平均速度
  d_inject1_STATE_INJECT_MAXPRESS,        // 0x10160014, 射出最大压力
  d_inject1_STATE_HOLD_MAXPRESS,          // 0x10160015, 保压最大压力
  d_inject1_STATE_HOLD_MOVPOSITION,       // 0x10160016, 保压移动距离
  d_inject1_STATE_INJECT_STEP,            // 0x10160017, 射出当前动作步
  d_inject1_STATE_Qc_Avg_InjTm,           // 0x10160018, 射出时间平均值
  d_inject1_STATE_Qc_Deviation_Max_InjTm, // 0x10160019, 射出时间最大偏差值
  d_inject1_STATE_Qc_Unq_Cnt_InjTm,       // 0x1016001a, 射出时间不合格数
  d_inject1_STATE_Qc_Unq_Rate_InjTm,      // 0x1016001b, 射出时间不合格率
  d_inject1_STATE_Qc_Max_InjTm,           // 0x1016001c, 射出时间最大值
  d_inject1_STATE_Qc_Min_InjTm,           // 0x1016001d, 射出时间最小值
  d_inject1_STATE_Qc_Avg_InjSpd,          // 0x1016001e, 射出速度平均值
  d_inject1_STATE_Qc_Deviation_Max_InjSpd, // 0x1016001f, 射出速度最大偏差值
  d_inject1_STATE_Qc_Unq_Cnt_InjSpd,      // 0x10160020, 射出速度不合格数
  d_inject1_STATE_Qc_Unq_Rate_InjSpd,     // 0x10160021, 射出速度不合格率
  d_inject1_STATE_Qc_Max_InjSpd,          // 0x10160022, 射出速度最大值
  d_inject1_STATE_Qc_Min_InjSpd,          // 0x10160023, 射出速度最小值
  d_inject1_STATE_Qc_Avg_InjEnd,          // 0x10160024, 保压转换位置平均值
  d_inject1_STATE_Qc_Deviation_Max_InjEnd, // 0x10160025, 保压转换位置最大偏差值
  d_inject1_STATE_Qc_Unq_Cnt_InjEnd,      // 0x10160026, 保压转换位置不合格数
  d_inject1_STATE_Qc_Unq_Rate_InjEnd,     // 0x10160027, 保压转换位置不合格率
  d_inject1_STATE_Qc_Max_InjEnd,          // 0x10160028, 保压转换位置最大值
  d_inject1_STATE_Qc_Min_InjEnd,          // 0x10160029, 保压转换位置最小值
  d_inject1_STATE_Qc_Avg_InjPrs,          // 0x1016002a, 射出压力平均值
  d_inject1_STATE_Qc_Deviation_Max_InjPrs, // 0x1016002b, 射出压力最大偏差值
  d_inject1_STATE_Qc_Unq_Cnt_InjPrs,      // 0x1016002c, 射出压力不合格数
  d_inject1_STATE_Qc_Unq_Rate_InjPrs,     // 0x1016002d, 射出压力不合格率
  d_inject1_STATE_Qc_Max_InjPrs,          // 0x1016002e, 射出压力最大值
  d_inject1_STATE_Qc_Min_InjPrs,          // 0x1016002f, 射出压力最小值
  d_inject1_STATE_Qc_Avg_HoldEnd,         // 0x10160030, 残料位置平均值
  d_inject1_STATE_Qc_Deviation_Max_HoldEnd, // 0x10160031, 残料位置最大偏差值
  d_inject1_STATE_Qc_Unq_Cnt_HoldEnd,     // 0x10160032, 残料位置不合格数
  d_inject1_STATE_Qc_Unq_Rate_HoldEnd,    // 0x10160033, 残料位置不合格率
  d_inject1_STATE_Qc_Max_HoldEnd,         // 0x10160034, 残料位置最大值
  d_inject1_STATE_Qc_Min_HoldEnd,         // 0x10160035, 残料位置最小值
  d_inject1_STATE_Qc_Avg_ChgTm,           // 0x10160036, 储料时间平均值
  d_inject1_STATE_Qc_Deviation_Max_ChgTm, // 0x10160037, 储料时间最大偏差值
  d_inject1_STATE_Qc_Unq_Cnt_ChgTm,       // 0x10160038, 储料时间不合格数
  d_inject1_STATE_Qc_Unq_Rate_ChgTm,      // 0x10160039, 储料时间不合格率
  d_inject1_STATE_Qc_Max_ChgTm,           // 0x1016003a, 储料时间最大值
  d_inject1_STATE_Qc_Min_ChgTm,           // 0x1016003b, 储料时间最小值
  d_inject1_STATE_Qc_Avg_ChgEnd,          // 0x1016003c, 储料终止位置平均值
  d_inject1_STATE_Qc_Deviation_Max_ChgEnd, // 0x1016003d, 储料终止位置最大偏差值
  d_inject1_STATE_Qc_Unq_Cnt_ChgEnd,      // 0x1016003e, 储料终止位置不合格数
  d_inject1_STATE_Qc_Unq_Rate_ChgEnd,     // 0x1016003f, 储料终止位置不合格率
  d_inject1_STATE_Qc_Max_ChgEnd,          // 0x10160040, 储料终止位置最大值
  d_inject1_STATE_Qc_Min_ChgEnd,          // 0x10160041, 储料终止位置最小值
  d_inject1_STATE_Qc_Avg_SuckBwd,         // 0x10160042, 射退终止位置平均值
  d_inject1_STATE_Qc_Deviation_Max_SuckBwd, // 0x10160043, 射退终止位置最大偏差值
  d_inject1_STATE_Qc_Unq_Cnt_SuckBwd,     // 0x10160044, 射退终止位置不合格数
  d_inject1_STATE_Qc_Unq_Rate_SuckBwd,    // 0x10160045, 射退终止位置不合格率
  d_inject1_STATE_Qc_Max_SuckBwd,         // 0x10160046, 射退终止位置最大值
  d_inject1_STATE_Qc_Min_SuckBwd,         // 0x10160047, 射退终止位置最小值
  d_inject1_STATE_Qc_Avg_SuckTm,          // 0x10160048, 射退时间平均值
  d_inject1_STATE_Qc_Deviation_Max_SuckTm, // 0x10160049, 射退时间最大偏差值
  d_inject1_STATE_Qc_Unq_Cnt_SuckTm,      // 0x1016004a, 射退时间不合格数
  d_inject1_STATE_Qc_Unq_Rate_SuckTm,     // 0x1016004b, 射退时间不合格率
  d_inject1_STATE_Qc_Max_SuckTm,          // 0x1016004c, 射退时间最大值
  d_inject1_STATE_Qc_Min_SuckTm,          // 0x1016004d, 射退时间最小值
  d_inject1_STATE_TM_INJ_RT,              // 0x1016004e, 实时计算的射出时间
  d_inject1_STATE_TM_CHARGE_RT,           // 0x1016004f, 实时计算的储料时间
  d_inject1_STATE_TIME_INJ1,              // 0x10160050, 射出一段计时
  d_inject1_STATE_TIME_INJ2,              // 0x10160051, 射出二段计时
  d_inject1_STATE_TIME_INJ3,              // 0x10160052, 射出三段计时
  d_inject1_STATE_TIME_INJ4,              // 0x10160053, 射出四段计时
  d_inject1_STATE_TIME_INJ5,              // 0x10160054, 射出五段计时
  d_inject1_STATE_TIME_INJ6,              // 0x10160055, 射出六段计时
  d_inject1_STATE_TIME_INJ7,              // 0x10160056, 射出七段计时
  d_inject1_STATE_TIME_INJ8,              // 0x10160057, 射出八段计时
  d_inject1_STATE_TIME_INJ9,              // 0x10160058, 射出九段计时
  d_inject1_STATE_TIME_INJ10,             // 0x10160059, 射出十段计时
  d_inject1_STATE_Step_Inj,               // 0x1016005a, 射出实时动作步
  d_inject1_STATE_Step_Hold,              // 0x1016005b, 保压实时动作步
  d_inject1_STATE_Step_InjProc,           // 0x1016005c, 射出调度实时动作步
  d_inject1_STATE_Step_Chg,               // 0x1016005d, 储料实时动作步
  d_inject1_STATE_Step_Suck,              // 0x1016005e, 射退实时动作步
  d_inject1_STATE_Step_InjBforeNoz,       // 0x1016005f, 座进前射出实时动作步
  d_inject1_STATE_Step_ChgBforeInj,       // 0x10160060, 射出前储料实时动作步
  d_inject1_STATE_TM_RELVLVOPN,           // 0x10160061, 储料油马达泄压阀开启时间
  d_inject1_STATE_CHARGE_SPEED,           // 0x10160062, 射出计时起始时间
  d_inject1_STATE_SUCK_SPEED,             // 0x10160063, 储料计时起始时间
  d_inject1_STATE_HOLD_START,             // 0x10160064, 保压计时起始时间
  d_inject1_STATE_SUCKBACK_START,         // 0x10160065, 储后射退计时起始时间
  d_inject1_STATE_HOLD_RT,                // 0x10160066, 实时计算的保压时间，用于面板显示
  d_inject1_STATE_SUCKBK_RT,              // 0x10160067, 实时计算的射退时间，用于面板显示
  d_inject1_STATE_TM_HOLD_RT,             // 0x10160068, 实时计算的保压时间
  d_inject1_STATE_TM_SUCKBK_RT,           // 0x10160069, 实时计算的射退时间
  d_inject1_STATE_TM_INJ_H,               // 0x1016006a, 射出计时实际值
  d_inject1_STATE_Qc_Max_InjTm_H,         // 0x1016006b, 射出时间最大值
  d_inject1_STATE_Qc_Min_InjTm_H,         // 0x1016006c, 射出时间最小值
  d_inject1_STATE_Qc_Avg_InjTm_H,         // 0x1016006d, 射出时间平均值
  d_inject1_STATE_Qc_Deviation_Max_InjTm_H, // 0x1016006e, 射出时间最大偏差值
  d_inject1_STATE_TM_HOLD_RT_H,           // 0x1016006f, 实时计算的保压时间
  d_inject1_STATE_TM_HOLD_L,              // 0x10160070, 保压时间实际值
  d_inject1_STATE_TM_HOLD_H,              // 0x10160071, 保压时间实际值
  d_inject1_STATE_TM_INJ_RT_H,            // 0x10160072, 实时计算的射出时间
  d_inject1_STATE_TM_INJ2_L,              // 0x10160073, 射出时间实际值 低位（动作完成上传）
  d_inject1_STATE_TM_INJ2_H,              // 0x10160074, 射出时间实际值 高位（动作完成上传）
  d_inject1_STATE_INJ_HLD_PRES,           // 0x10160075, 射出压力传感器实际压力
  d_inject1_STATE_FLAG_PURGE_FINISH       // 0x10160076, 多次清料完成标记
};

enum db_nozzle1_PO{
  d_nozzle1_PO_O_NOZ_FWD = 0x11100000, // 0x11100000, 座进
  d_nozzle1_PO_O_SRV_NOZF,             // 0x11100001, 座进备用
  d_nozzle1_PO_O_NOZ_BWD,              // 0x11100002, 座退
  d_nozzle1_PO_O_SRV_NOZB,             // 0x11100003, 座退备用
  d_nozzle1_PO_O_NOZF_NOZB,            // 0x11100004, 座进座退
  d_nozzle1_PO_O_NOZ_FWD2,             // 0x11100005, 座进2
  d_nozzle1_PO_O_NOZ_BWD2              // 0x11100006, 座退2
};

enum db_nozzle1_PI{
  d_nozzle1_PI_I_NOZ_FWD = 0x11110000, // 0x11110000, 座进终
  d_nozzle1_PI_I_NOZ_FWS,              // 0x11110001, 座进慢速
  d_nozzle1_PI_I_NOZ_BWD,              // 0x11110002, 座退终
  d_nozzle1_PI_I_NOZ_BWS               // 0x11110003, 座退慢速
};

enum db_nozzle1_MACHSET{
  d_nozzle1_MACHSET_NZL_METERCH = 0x11120000, // 0x11120000, 座台电子尺通道
  d_nozzle1_MACHSET_DAPRES_NOZRAMP,           // 0x11120001, 座台启动压力斜率
  d_nozzle1_MACHSET_DAFLOW_NOZRAMP,           // 0x11120002, 座台启动速度斜率
  d_nozzle1_MACHSET_DAPRES_OFFNOZRAMP,        // 0x11120003, 座台关闭压力斜率
  d_nozzle1_MACHSET_DAFLOW_OFFNOZRAMP,        // 0x11120004, 座台关闭速度斜率
  d_nozzle1_MACHSET_DAPRES_NOZSETUP,          // 0x11120005, 座台慢速调试压力
  d_nozzle1_MACHSET_DAFLOW_NOZSETUP,          // 0x11120006, 座台慢速调试速度
  d_nozzle1_MACHSET_TM_DLYNOZADV,             // 0x11120007, 座进延迟
  d_nozzle1_MACHSET_TM_DLYNOZCOM,             // 0x11120008, 座进动作结束延时
  d_nozzle1_MACHSET_TM_DLYNOZRET,             // 0x11120009, 座退延迟
  d_nozzle1_MACHSET_TM_DLYNOZBCK,             // 0x1112000a, 座退动作结束延时
  d_nozzle1_MACHSET_TM_NOZADVSLOW,            // 0x1112000b, 座台进慢速时间
  d_nozzle1_MACHSET_TMDLY_VLVOFFNOZBWD,       // 0x1112000c, 座退阀延迟关计时
  d_nozzle1_MACHSET_NZLF_HYDELAY,             // 0x1112000d, 座进液压延迟开计时
  d_nozzle1_MACHSET_NZLB_HYDELAY,             // 0x1112000e, 座退液压延迟开计时
  d_nozzle1_MACHSET_TMDLY_VLV_ONNZLF,         // 0x1112000f, 座进阀延迟开计时
  d_nozzle1_MACHSET_TMDLY_VLV_OFFNZLF,        // 0x11120010, 座进阀延迟关计时
  d_nozzle1_MACHSET_TMDLY_VLV_ONNZLB,         // 0x11120011, 座退阀延迟开计时
  d_nozzle1_MACHSET_NOZEND                    // 0x11120012, 座台使用方式
};

enum db_nozzle1_MOLDSET{
  d_nozzle1_MOLDSET_TM_COOLINGBEFORECHG = 0x11130000, // 0x11130000, 储前冷却计时
  d_nozzle1_MOLDSET_TM_COOLING,                       // 0x11130001, 模具冷却时间
  d_nozzle1_MOLDSET_FL_NOZRET,                        // 0x11130002, 座退方式
  d_nozzle1_MOLDSET_TM_NOZDEFAULTMAX,                 // 0x11130003, 座台计时上限
  d_nozzle1_MOLDSET_FL_COOLCHG,                       // 0x11130004, 储前冷却功能
  d_nozzle1_MOLDSET_TM_COOLINGBEFORECHG_H,            // 0x11130005, 储前冷却计时
  d_nozzle1_MOLDSET_TM_COOLING_H,                     // 0x11130006, 模具冷却时间
  d_nozzle1_MOLDSET_FL_AUTONOZFWD                     // 0x11130007, 自动全程输出座进阀
};

enum db_nozzle1_ACTPARA{
  d_nozzle1_ACTPARA_DAPRES_NOZADV = 0x11140000, // 0x11140000, 座台进压力
  d_nozzle1_ACTPARA_DAFLOW_NOZADV,              // 0x11140001, 座台进速度
  d_nozzle1_ACTPARA_DAPRES_NOZADVSLOW,          // 0x11140002, 座台进慢速压力
  d_nozzle1_ACTPARA_DAFLOW_NOZADVSLOW,          // 0x11140003, 座台进慢速速度
  d_nozzle1_ACTPARA_DAPRES_NOZRET,              // 0x11140004, 座台退压力
  d_nozzle1_ACTPARA_DAFLOW_NOZRET,              // 0x11140005, 座台退速度
  d_nozzle1_ACTPARA_DAPRES_NOZRETSLOW,          // 0x11140006, 座台退慢速压力
  d_nozzle1_ACTPARA_DAFLOW_NOZRETSLOW,          // 0x11140007, 座台退慢速速度
  d_nozzle1_ACTPARA_ADPOSI_NOZADVSLOWEND,       // 0x11140008, 座台进慢速终止位置
  d_nozzle1_ACTPARA_ADPOSI_NOZRETSLOWEND,       // 0x11140009, 座台退慢速终止位置
  d_nozzle1_ACTPARA_ADPOSI_NOZRETSLOWDIST,      // 0x1114000a, 座台退慢速距离
  d_nozzle1_ACTPARA_ADPOSI_NOZADVEND,           // 0x1114000b, 座台进终止位置
  d_nozzle1_ACTPARA_ADPOSI_NOZRETEND,           // 0x1114000c, 座台退终止位置
  d_nozzle1_ACTPARA_TM_NOZRETACT,               // 0x1114000d, 座台退时间
  d_nozzle1_ACTPARA_TM_NOZADVACT,               // 0x1114000e, 座台进快速时间
  d_nozzle1_ACTPARA_FL_OILHEAT,                 // 0x1114000f, 油温预热功能
  d_nozzle1_ACTPARA_DAPRES_OILHEAT,             // 0x11140010, 油温预热压力
  d_nozzle1_ACTPARA_DAFLOW_OILHEAT,             // 0x11140011, 油温预热速度
  d_nozzle1_ACTPARA_TM_OILHEAT,                 // 0x11140012, 油温预热时间
  d_nozzle1_ACTPARA_PER_DAFLOW_NOZZLE           // 0x11140013, 座台速度使用率
};

enum db_nozzle1_HYDR{
  d_nozzle1_HYDR_HYDR_PORT_NOZZLE = 0x11150000, // 0x11150000, 
  d_nozzle1_HYDR_HYDR_PUMPS_NOZZLE,             // 0x11150001, 
  d_nozzle1_HYDR_SERVO_KP_NOZZLE,               // 0x11150002, 座台 P
  d_nozzle1_HYDR_SERVO_KI_NOZZLE,               // 0x11150003, 座台 I
  d_nozzle1_HYDR_SERVO_KD_NOZZLE                // 0x11150004, 座台 D
};

enum db_nozzle1_STATE{
  d_nozzle1_STATE_FL_ERROR0 = 0x11160000, // 0x11160000, 警报1
  d_nozzle1_STATE_FL_ERROR1,              // 0x11160001, 警报2
  d_nozzle1_STATE_TM_NOZZLEFWD,           // 0x11160002, 座进计时实际值
  d_nozzle1_STATE_TM_NOZZLEBWD,           // 0x11160003, 座退计时实际值
  d_nozzle1_STATE_TM_COOL_RT,             // 0x11160004, 实时计算的冷却计时时间
  d_nozzle1_STATE_Step_NozzleFwd,         // 0x11160005, 
  d_nozzle1_STATE_Step_NozzleBwd,         // 0x11160006, 
  d_nozzle1_STATE_TM_COOL_START,          // 0x11160007, 冷却计时起始时间
  d_nozzle1_STATE_TM_NOZZLEF_START,       // 0x11160008, 座进计时起始值
  d_nozzle1_STATE_TM_NOZZLEB_START,       // 0x11160009, 座退计时起始值
  d_nozzle1_STATE_TM_NOZZLEF_RT,          // 0x1116000a, 座进计时实时值
  d_nozzle1_STATE_TM_NOZZLEB_RT,          // 0x1116000b, 座退计时实时值
  d_nozzle1_STATE_TM_COOL_RTH,            // 0x1116000c, 实时计算的冷却计时时间
  d_nozzle1_STATE_TM_COOL_L,              // 0x1116000d, 冷却计时实际值
  d_nozzle1_STATE_TM_COOL_H               // 0x1116000e, 冷却计时实际值
};

enum db_injstor1_PO{
  d_injstor1_PO_O_INJ_STO = 0x12100000, // 0x12100000, 储能
  d_injstor1_PO_O_STO_REL,              // 0x12100001, 储能泄压
  d_injstor1_PO_O_INJ_STO_SRV           // 0x12100002, 储能备用
};

enum db_injstor1_PI{
  d_injstor1_PI_I_INJ_STO = 0x12110000, // 0x12110000, 射出储能
  d_injstor1_PI_I_INJ_STO_SRV           // 0x12110001, 储能备用
};

enum db_injstor1_MACHSET{
  d_injstor1_MACHSET_INJSTOR_METERCH = 0x12120000  // 0x12120000, 蓄能器压力检测通道
};

enum db_injstor1_MOLDSET{
  d_injstor1_MOLDSET_DAPRES_INJSTOR = 0x12130000, // 0x12130000, 射出蓄能器当前压力
  d_injstor1_MOLDSET_DAPRES_INJSMAX,              // 0x12130001, 蓄能器压力上限
  d_injstor1_MOLDSET_DAPRES_INJSMIN               // 0x12130002, 蓄能器压力下限
};

enum db_injstor1_ACTPARA{
  d_injstor1_ACTPARA_DAPRES_INJSTORX = 0x12140000, // 0x12140000, 蓄能器充压压力
  d_injstor1_ACTPARA_DAFLOW_INJSTOR                // 0x12140001, 蓄能器充压速度
};

enum db_injstor1_HYDR{
  d_injstor1_HYDR_HYDR_PORT_INJSTOR = 0x12150000, // 0x12150000, 
  d_injstor1_HYDR_HYDR_PUMPS_INJSTOR              // 0x12150001, 
};

enum db_injstor1_STATE{
  d_injstor1_STATE_FL_ERROR0 = 0x12160000, // 0x12160000, 警报1
  d_injstor1_STATE_FL_ERROR1,              // 0x12160001, 警报2
  d_injstor1_STATE_Step_Injor              // 0x12160002, 
};

enum db_chargpress1_PO{
  d_chargpress1_PO_O_PRS_FWD = 0x13100000, // 0x13100000, 下压缸进
  d_chargpress1_PO_O_PRS_BWD,              // 0x13100001, 下压缸退
  d_chargpress1_PO_O_PRS_SRV               // 0x13100002, 下压缸备用
};

enum db_chargpress1_PI{
  d_chargpress1_PI_I_PRS_FWD = 0x13110000, // 0x13110000, 下压缸进终
  d_chargpress1_PI_I_PRS_BWD,              // 0x13110001, 下压缸退终
  d_chargpress1_PI_I_PRS_LVL,              // 0x13110002, 下压缸料位低
  d_chargpress1_PI_I_PRS_SRV               // 0x13110003, 下压缸备用
};

enum db_chargpress1_ACTPARA{
  d_chargpress1_ACTPARA_DAPRES_CHGPRSFWD = 0x13140000, // 0x13140000, 下压缸进压力
  d_chargpress1_ACTPARA_DAFLOW_CHGPRSFWD,              // 0x13140001, 下压缸进速度
  d_chargpress1_ACTPARA_DAPRES_CHGPRSBWD,              // 0x13140002, 下压缸退压力
  d_chargpress1_ACTPARA_DAFLOW_CHGPRSBWD,              // 0x13140003, 下压缸退速度
  d_chargpress1_ACTPARA_DAPRES_CHGPRS_SETUP,           // 0x13140004, 下压缸动作慢速调试压力
  d_chargpress1_ACTPARA_DAFLOW_CHGPRS_SETUP,           // 0x13140005, 下压缸动作慢速调试速度
  d_chargpress1_ACTPARA_DAPRES_CHGPRSRAMP,             // 0x13140006, 下压缸启动压力斜率
  d_chargpress1_ACTPARA_DAFLOW_CHGPRSRAMP,             // 0x13140007, 下压缸启动速度斜率
  d_chargpress1_ACTPARA_DAPRES_CHGPRSOFFRAMP,          // 0x13140008, 下压缸关闭压力斜率
  d_chargpress1_ACTPARA_DAFLOW_CHGPRSOFFRAMP           // 0x13140009, 下压缸关闭速度斜率
};

enum db_chargpress1_HYDR{
  d_chargpress1_HYDR_HYDR_PORT_CHGPRS = 0x13150000, // 0x13150000, 
  d_chargpress1_HYDR_HYDR_PUMPS_CHGPRS,             // 0x13150001, 
  d_chargpress1_HYDR_SERVO_KP_CHGPRS,               // 0x13150002, 
  d_chargpress1_HYDR_SERVO_KI_CHGPRS,               // 0x13150003, 
  d_chargpress1_HYDR_SERVO_KD_CHGPRS                // 0x13150004, 
};

enum db_chargpress1_STATE{
  d_chargpress1_STATE_FL_ERROR0 = 0x13160000, // 0x13160000, 
  d_chargpress1_STATE_FL_ERROR1,              // 0x13160001, 
  d_chargpress1_STATE_Step_ChargeUp,          // 0x13160002, 
  d_chargpress1_STATE_Step_ChargeDown         // 0x13160003, 
};

enum db_QcMonitor1_MOLDSET{
  d_QcMonitor1_MOLDSET_Qc_Fun = 0x1b130000, // 0x1b130000, 监控功能
  d_QcMonitor1_MOLDSET_Qc_OpnNum,           // 0x1b130001, 监控模数
  d_QcMonitor1_MOLDSET_Qc_DeviationSet_CycTm, // 0x1b130002, 周期时间允许偏差
  d_QcMonitor1_MOLDSET_Qc_TmSet_CycTm,      // 0x1b130003, 周期时间设定值
  d_QcMonitor1_MOLDSET_Qc_Mode_CycTm,       // 0x1b130004, 周期时间监控
  d_QcMonitor1_MOLDSET_Qc_DeviationSet_InjTm, // 0x1b130005, 射出时间允许偏差
  d_QcMonitor1_MOLDSET_Qc_TmSet_InjTm,      // 0x1b130006, 射出时间设定值
  d_QcMonitor1_MOLDSET_Qc_Mode_InjTm,       // 0x1b130007, 射出时间监控
  d_QcMonitor1_MOLDSET_Qc_DeviationSet_InjSpd, // 0x1b130008, 射出速度允许偏差
  d_QcMonitor1_MOLDSET_Qc_TmSet_InjSpd,     // 0x1b130009, 射出速度设定值
  d_QcMonitor1_MOLDSET_Qc_Mode_InjSpd,      // 0x1b13000a, 射出速度监控
  d_QcMonitor1_MOLDSET_Qc_DeviationSet_InjEnd, // 0x1b13000b, 保压转化位置允许偏差
  d_QcMonitor1_MOLDSET_Qc_TmSet_InjEnd,     // 0x1b13000c, 保压转化位置设定值
  d_QcMonitor1_MOLDSET_Qc_Mode_InjEnd,      // 0x1b13000d, 保压转化位置监控
  d_QcMonitor1_MOLDSET_Qc_DeviationSet_InjPrs, // 0x1b13000e, 射出最大压力允许偏差
  d_QcMonitor1_MOLDSET_Qc_TmSet_InjPrs,     // 0x1b13000f, 射出最大压力设定值
  d_QcMonitor1_MOLDSET_Qc_Mode_InjPrs,      // 0x1b130010, 射出最大压力监控
  d_QcMonitor1_MOLDSET_Qc_DeviationSet_HoldEnd, // 0x1b130011, 残料位置允许偏差
  d_QcMonitor1_MOLDSET_Qc_TmSet_HoldEnd,    // 0x1b130012, 残料位置设定值
  d_QcMonitor1_MOLDSET_Qc_Mode_HoldEnd,     // 0x1b130013, 残料位置监控
  d_QcMonitor1_MOLDSET_Qc_DeviationSet_ChgTm, // 0x1b130014, 储料时间允许偏差
  d_QcMonitor1_MOLDSET_Qc_TmSet_ChgTm,      // 0x1b130015, 储料时间设定值
  d_QcMonitor1_MOLDSET_Qc_Mode_ChgTm,       // 0x1b130016, 储料时间监控
  d_QcMonitor1_MOLDSET_Qc_DeviationSet_ChgEnd, // 0x1b130017, 储料终止位置允许偏差
  d_QcMonitor1_MOLDSET_Qc_TmSet_ChgEnd,     // 0x1b130018, 储料终止位置设定值
  d_QcMonitor1_MOLDSET_Qc_Mode_ChgEnd,      // 0x1b130019, 储料终止位置监控
  d_QcMonitor1_MOLDSET_Qc_DeviationSet_SuckBwd, // 0x1b13001a, 射退终止位置允许偏差
  d_QcMonitor1_MOLDSET_Qc_TmSet_SuckBwd,    // 0x1b13001b, 射退终止位置设定值
  d_QcMonitor1_MOLDSET_Qc_Mode_SuckBwd,     // 0x1b13001c, 射退终止位置监控
  d_QcMonitor1_MOLDSET_Qc_DeviationSet_SuckTm, // 0x1b13001d, 射退时间允许偏差
  d_QcMonitor1_MOLDSET_Qc_TmSet_SuckTm,     // 0x1b13001e, 射退时间设定值
  d_QcMonitor1_MOLDSET_Qc_Mode_SuckTm,      // 0x1b13001f, 射退时间监控
  d_QcMonitor1_MOLDSET_Qc_DeviationSet_ClsTm, // 0x1b130020, 合模时间允许偏差
  d_QcMonitor1_MOLDSET_Qc_TmSet_ClsTm,      // 0x1b130021, 合模时间设定值
  d_QcMonitor1_MOLDSET_Qc_Mode_ClsTm,       // 0x1b130022, 合模时间监控
  d_QcMonitor1_MOLDSET_Qc_DeviationSet_ClsLow, // 0x1b130023, 合模低压时间允许偏差
  d_QcMonitor1_MOLDSET_Qc_TmSet_ClsLow,     // 0x1b130024, 合模低压时间设定值
  d_QcMonitor1_MOLDSET_Qc_Mode_ClsLow,      // 0x1b130025, 合模低压监控
  d_QcMonitor1_MOLDSET_Qc_DeviationSet_ClsHi, // 0x1b130026, 合模高压时间允许偏差
  d_QcMonitor1_MOLDSET_Qc_TmSet_ClsHi,      // 0x1b130027, 合模高压时间设定值
  d_QcMonitor1_MOLDSET_Qc_Mode_ClsHi,       // 0x1b130028, 合模高压时间监控
  d_QcMonitor1_MOLDSET_Qc_DeviationSet_OpnEnd, // 0x1b130029, 开模终止位置允许偏差
  d_QcMonitor1_MOLDSET_Qc_TmSet_OpnEnd,     // 0x1b13002a, 开模终止位置设定值
  d_QcMonitor1_MOLDSET_Qc_Mode_OpnEnd,      // 0x1b13002b, 开模终止位置监控
  d_QcMonitor1_MOLDSET_Qc_DeviationSet_OpnTm, // 0x1b13002c, 开模时间允许偏差
  d_QcMonitor1_MOLDSET_Qc_TmSet_OpnTm,      // 0x1b13002d, 开模时间设定值
  d_QcMonitor1_MOLDSET_Qc_Mode_OpnTm,       // 0x1b13002e, 开模时间监控
  d_QcMonitor1_MOLDSET_Qc_DeviationSet_OpnSpd, // 0x1b13002f, 开模速度允许偏差
  d_QcMonitor1_MOLDSET_Qc_TmSet_OpnSpd,     // 0x1b130030, 开模速度设定值
  d_QcMonitor1_MOLDSET_Qc_Mode_OpnSpd,      // 0x1b130031, 开模速度监控
  d_QcMonitor1_MOLDSET_Qc_DeviationSet_EjtTm, // 0x1b130032, 顶针时间允许偏差
  d_QcMonitor1_MOLDSET_Qc_TmSet_EjtTm,      // 0x1b130033, 顶针时间设定值
  d_QcMonitor1_MOLDSET_Qc_Mode_EjtTm        // 0x1b130034, 顶针时间监控
};

enum db_QcMonitor1_STATE{
  d_QcMonitor1_STATE_FL_ERROR0 = 0x1b160000, // 0x1b160000, 警报1
  d_QcMonitor1_STATE_FL_ERROR1,              // 0x1b160001, 警报2
  d_QcMonitor1_STATE_Qc_Opn_Cnt,             // 0x1b160002, 监控模数
  d_QcMonitor1_STATE_Qc_Avg_CycTm,           // 0x1b160003, 周期时间平均值
  d_QcMonitor1_STATE_Qc_Deviation_Max_CycTm, // 0x1b160004, 周期时间最大偏差值
  d_QcMonitor1_STATE_Qc_Unq_Cnt_CycTm,       // 0x1b160005, 周期时间不合格数
  d_QcMonitor1_STATE_Qc_Unq_Rate_CycTm,      // 0x1b160006, 周期时间不合格率
  d_QcMonitor1_STATE_Qc_Max_CycTm,           // 0x1b160007, 周期时间最大值
  d_QcMonitor1_STATE_Qc_Min_CycTm,           // 0x1b160008, 周期时间最小值
  d_QcMonitor1_STATE_Qc_Avg_InjTm,           // 0x1b160009, 射出时间平均值
  d_QcMonitor1_STATE_Qc_Deviation_Max_InjTm, // 0x1b16000a, 射出时间最大偏差值
  d_QcMonitor1_STATE_Qc_Unq_Cnt_InjTm,       // 0x1b16000b, 射出时间不合格数
  d_QcMonitor1_STATE_Qc_Unq_Rate_InjTm,      // 0x1b16000c, 射出时间不合格率
  d_QcMonitor1_STATE_Qc_Max_InjTm,           // 0x1b16000d, 射出时间最大值
  d_QcMonitor1_STATE_Qc_Min_InjTm,           // 0x1b16000e, 射出时间最小值
  d_QcMonitor1_STATE_Qc_Avg_InjSpd,          // 0x1b16000f, 射出速度平均值
  d_QcMonitor1_STATE_Qc_Deviation_Max_InjSpd, // 0x1b160010, 射出速度最大偏差值
  d_QcMonitor1_STATE_Qc_Unq_Cnt_InjSpd,      // 0x1b160011, 射出速度不合格数
  d_QcMonitor1_STATE_Qc_Unq_Rate_InjSpd,     // 0x1b160012, 射出速度不合格率
  d_QcMonitor1_STATE_Qc_Max_InjSpd,          // 0x1b160013, 射出速度最大值
  d_QcMonitor1_STATE_Qc_Min_InjSpd,          // 0x1b160014, 射出速度最小值
  d_QcMonitor1_STATE_Qc_Avg_InjEnd,          // 0x1b160015, 保压转换位置平均值
  d_QcMonitor1_STATE_Qc_Deviation_Max_InjEnd, // 0x1b160016, 保压转换位置最大偏差值
  d_QcMonitor1_STATE_Qc_Unq_Cnt_InjEnd,      // 0x1b160017, 保压转换位置不合格数
  d_QcMonitor1_STATE_Qc_Unq_Rate_InjEnd,     // 0x1b160018, 保压转换位置不合格率
  d_QcMonitor1_STATE_Qc_Max_InjEnd,          // 0x1b160019, 保压转换位置最大值
  d_QcMonitor1_STATE_Qc_Min_InjEnd,          // 0x1b16001a, 保压转换位置最小值
  d_QcMonitor1_STATE_Qc_Avg_InjPrs,          // 0x1b16001b, 射出压力平均值
  d_QcMonitor1_STATE_Qc_Deviation_Max_InjPrs, // 0x1b16001c, 射出压力最大偏差值
  d_QcMonitor1_STATE_Qc_Unq_Cnt_InjPrs,      // 0x1b16001d, 射出压力不合格数
  d_QcMonitor1_STATE_Qc_Unq_Rate_InjPrs,     // 0x1b16001e, 射出压力不合格率
  d_QcMonitor1_STATE_Qc_Max_InjPrs,          // 0x1b16001f, 射出压力最大值
  d_QcMonitor1_STATE_Qc_Min_InjPrs,          // 0x1b160020, 射出压力最小值
  d_QcMonitor1_STATE_Qc_Avg_HoldEnd,         // 0x1b160021, 残料位置平均值
  d_QcMonitor1_STATE_Qc_Deviation_Max_HoldEnd, // 0x1b160022, 残料位置最大偏差值
  d_QcMonitor1_STATE_Qc_Unq_Cnt_HoldEnd,     // 0x1b160023, 残料位置不合格数
  d_QcMonitor1_STATE_Qc_Unq_Rate_HoldEnd,    // 0x1b160024, 残料位置不合格率
  d_QcMonitor1_STATE_Qc_Max_HoldEnd,         // 0x1b160025, 残料位置最大值
  d_QcMonitor1_STATE_Qc_Min_HoldEnd,         // 0x1b160026, 残料位置最小值
  d_QcMonitor1_STATE_Qc_Avg_ChgTm,           // 0x1b160027, 储料时间平均值
  d_QcMonitor1_STATE_Qc_Deviation_Max_ChgTm, // 0x1b160028, 储料时间最大偏差值
  d_QcMonitor1_STATE_Qc_Unq_Cnt_ChgTm,       // 0x1b160029, 储料时间不合格数
  d_QcMonitor1_STATE_Qc_Unq_Rate_ChgTm,      // 0x1b16002a, 储料时间不合格率
  d_QcMonitor1_STATE_Qc_Max_ChgTm,           // 0x1b16002b, 储料时间最大值
  d_QcMonitor1_STATE_Qc_Min_ChgTm,           // 0x1b16002c, 储料时间最小值
  d_QcMonitor1_STATE_Qc_Avg_ChgEnd,          // 0x1b16002d, 储料终止位置平均值
  d_QcMonitor1_STATE_Qc_Deviation_Max_ChgEnd, // 0x1b16002e, 储料终止位置最大偏差值
  d_QcMonitor1_STATE_Qc_Unq_Cnt_ChgEnd,      // 0x1b16002f, 储料终止位置不合格数
  d_QcMonitor1_STATE_Qc_Unq_Rate_ChgEnd,     // 0x1b160030, 储料终止位置不合格率
  d_QcMonitor1_STATE_Qc_Max_ChgEnd,          // 0x1b160031, 储料终止位置最大值
  d_QcMonitor1_STATE_Qc_Min_ChgEnd,          // 0x1b160032, 储料终止位置最小值
  d_QcMonitor1_STATE_Qc_Avg_SuckBwd,         // 0x1b160033, 射退终止位置平均值
  d_QcMonitor1_STATE_Qc_Deviation_Max_SuckBwd, // 0x1b160034, 射退终止位置最大偏差值
  d_QcMonitor1_STATE_Qc_Unq_Cnt_SuckBwd,     // 0x1b160035, 射退终止位置不合格数
  d_QcMonitor1_STATE_Qc_Unq_Rate_SuckBwd,    // 0x1b160036, 射退终止位置不合格率
  d_QcMonitor1_STATE_Qc_Max_SuckBwd,         // 0x1b160037, 射退终止位置最大值
  d_QcMonitor1_STATE_Qc_Min_SuckBwd,         // 0x1b160038, 射退终止位置最小值
  d_QcMonitor1_STATE_Qc_Avg_SuckTm,          // 0x1b160039, 射退时间平均值
  d_QcMonitor1_STATE_Qc_Deviation_Max_SuckTm, // 0x1b16003a, 射退时间最大偏差值
  d_QcMonitor1_STATE_Qc_Unq_Cnt_SuckTm,      // 0x1b16003b, 射退时间不合格数
  d_QcMonitor1_STATE_Qc_Unq_Rate_SuckTm,     // 0x1b16003c, 射退时间不合格率
  d_QcMonitor1_STATE_Qc_Max_SuckTm,          // 0x1b16003d, 射退时间最大值
  d_QcMonitor1_STATE_Qc_Min_SuckTm,          // 0x1b16003e, 射退时间最小值
  d_QcMonitor1_STATE_Qc_Avg_Cls,             // 0x1b16003f, 合模时间平均值
  d_QcMonitor1_STATE_Qc_Deviation_Max_Cls,   // 0x1b160040, 合模时间最大偏差值
  d_QcMonitor1_STATE_Qc_Unq_Cnt_Cls,         // 0x1b160041, 合模时间不合格数
  d_QcMonitor1_STATE_Qc_Unq_Rate_Cls,        // 0x1b160042, 合模时间不合格率
  d_QcMonitor1_STATE_Qc_Max_Cls,             // 0x1b160043, 合模时间最大值
  d_QcMonitor1_STATE_Qc_Min_Cls,             // 0x1b160044, 合模时间最小值
  d_QcMonitor1_STATE_Qc_Avg_ClsLow,          // 0x1b160045, 合模低压时间平均值
  d_QcMonitor1_STATE_Qc_Deviation_Max_ClsLow, // 0x1b160046, 合模低压时间最大偏差值
  d_QcMonitor1_STATE_Qc_Unq_Cnt_ClsLow,      // 0x1b160047, 合模低压时间不合格数
  d_QcMonitor1_STATE_Qc_Unq_Rate_ClsLow,     // 0x1b160048, 合模低压时间不合格率
  d_QcMonitor1_STATE_Qc_Max_ClsLow,          // 0x1b160049, 合模低压时间最大值
  d_QcMonitor1_STATE_Qc_Min_ClsLow,          // 0x1b16004a, 合模低压时间最小值
  d_QcMonitor1_STATE_Qc_Avg_ClsHi,           // 0x1b16004b, 合模高压时间平均值
  d_QcMonitor1_STATE_Qc_Deviation_Max_ClsHi, // 0x1b16004c, 合模高压时间最大偏差值
  d_QcMonitor1_STATE_Qc_Unq_Cnt_ClsHi,       // 0x1b16004d, 合模高压时间不合格数
  d_QcMonitor1_STATE_Qc_Unq_Rate_ClsHi,      // 0x1b16004e, 合模高压时间不合格率
  d_QcMonitor1_STATE_Qc_Max_ClsHi,           // 0x1b16004f, 合模高压时间最大值
  d_QcMonitor1_STATE_Qc_Min_ClsHi,           // 0x1b160050, 合模高压时间最小值
  d_QcMonitor1_STATE_Qc_Avg_OpnEnd,          // 0x1b160051, 开模位置平均值
  d_QcMonitor1_STATE_Qc_Deviation_Max_OpnEnd, // 0x1b160052, 开模位置最大偏差值
  d_QcMonitor1_STATE_Qc_Unq_Cnt_OpnEnd,      // 0x1b160053, 开模位置不合格数
  d_QcMonitor1_STATE_Qc_Unq_Rate_OpnEnd,     // 0x1b160054, 开模位置不合格率
  d_QcMonitor1_STATE_Qc_Max_OpnEnd,          // 0x1b160055, 开模位置最大值
  d_QcMonitor1_STATE_Qc_Min_OpnEnd,          // 0x1b160056, 开模位置最小值
  d_QcMonitor1_STATE_Qc_Avg_OpnTm,           // 0x1b160057, 开模时间平均值
  d_QcMonitor1_STATE_Qc_Deviation_Max_OpnTm, // 0x1b160058, 开模时间最大偏差值
  d_QcMonitor1_STATE_Qc_Unq_Cnt_OpnTm,       // 0x1b160059, 开模时间不合格数
  d_QcMonitor1_STATE_Qc_Unq_Rate_OpnTm,      // 0x1b16005a, 开模时间不合格率
  d_QcMonitor1_STATE_Qc_Max_OpnTm,           // 0x1b16005b, 开模时间最大值
  d_QcMonitor1_STATE_Qc_Min_OpnTm,           // 0x1b16005c, 开模时间最小值
  d_QcMonitor1_STATE_Qc_Avg_OpnSpd,          // 0x1b16005d, 开模速度平均值
  d_QcMonitor1_STATE_Qc_Deviation_Max_OpnSpd, // 0x1b16005e, 开模速度最大偏差值
  d_QcMonitor1_STATE_Qc_Unq_Cnt_OpnSpd,      // 0x1b16005f, 开模速度不合格数
  d_QcMonitor1_STATE_Qc_Unq_Rate_OpnSpd,     // 0x1b160060, 开模速度不合格率
  d_QcMonitor1_STATE_Qc_Max_OpnSpd,          // 0x1b160061, 开模速度最大值
  d_QcMonitor1_STATE_Qc_Min_OpnSpd,          // 0x1b160062, 开模速度最小值
  d_QcMonitor1_STATE_Qc_Avg_EjtTm,           // 0x1b160063, 顶针时间平均值
  d_QcMonitor1_STATE_Qc_Deviation_Max_EjtTm, // 0x1b160064, 顶针时间最大偏差值
  d_QcMonitor1_STATE_Qc_Unq_Cnt_EjtTm,       // 0x1b160065, 顶针时间不合格数
  d_QcMonitor1_STATE_Qc_Unq_Rate_EjtTm,      // 0x1b160066, 顶针时间不合格率
  d_QcMonitor1_STATE_Qc_Max_EjtTm,           // 0x1b160067, 顶针时间最大值
  d_QcMonitor1_STATE_Qc_Min_EjtTm            // 0x1b160068, 顶针时间最小值
};

enum db_clamp1_PO{
  d_clamp1_PO_O_CLS_SOL = 0x20100000, // 0x20100000, 关模
  d_clamp1_PO_O_CLS_ULT,              // 0x20100001, 差动
  d_clamp1_PO_O_CLS_FST,              // 0x20100002, 关模快速阀
  d_clamp1_PO_O_CL_SETU,              // 0x20100003, 关模设定
  d_clamp1_PO_O_CL_HIP,               // 0x20100004, 关模高压
  d_clamp1_PO_O_RSV_CLS,              // 0x20100005, 关模备用阀
  d_clamp1_PO_O_CLS_LOW,              // 0x20100006, 关模低压阀头
  d_clamp1_PO_O_OPN_SOL,              // 0x20100007, 开模
  d_clamp1_PO_O_REL_PRS,              // 0x20100008, 泄压
  d_clamp1_PO_O_GUIDE,                // 0x20100009, 引导阀
  d_clamp1_PO_O_OPN_FST,              // 0x2010000a, 开模快速
  d_clamp1_PO_O_OP_SETU,              // 0x2010000b, 开模设定
  d_clamp1_PO_O_OP_SSLW,              // 0x2010000c, 开模慢速
  d_clamp1_PO_O_MEC_SAF_AIR,          // 0x2010000d, 机械保险气缸
  d_clamp1_PO_O_RSV_OPN,              // 0x2010000e, 开模备用阀
  d_clamp1_PO_O_OPN_REL,              // 0x2010000f, 开消阀
  d_clamp1_PO_O_CLAMP_AUX,            // 0x20100010, 开关模辅助
  d_clamp1_PO_O_OPN_BAC,              // 0x20100011, 开模背压
  d_clamp1_PO_O_CLAMP_FST,            // 0x20100012, 开关模快速
  d_clamp1_PO_O_CLS_ULT2,             // 0x20100013, 差动2
  d_clamp1_PO_O_CL_TOHI,              // 0x20100014, 关摸至高压
  d_clamp1_PO_O_OP_ESLW,              // 0x20100015, 开模终慢
  d_clamp1_PO_O_CLS_SOL2,             // 0x20100016, 关模安全
  d_clamp1_PO_O_OPN_LOW,              // 0x20100017, 开模低压阀
  d_clamp1_PO_O_CLS_ULT3,             // 0x20100018, 差动3
  d_clamp1_PO_O_CLS_HLP,              // 0x20100019, 差动辅助
  d_clamp1_PO_O_OPN_HLP,              // 0x2010001a, 开模辅助
  d_clamp1_PO_O_ERR_PRU,              // 0x2010001b, 不良产品
  d_clamp1_PO_O_SPC_CLS,              // 0x2010001c, 特殊关模阀
  d_clamp1_PO_O_SPC_OPN,              // 0x2010001d, 特殊开模阀
  d_clamp1_PO_O_CLS_TOTAL,            // 0x2010001e, 锁模总阀
  d_clamp1_PO_O_CLSOPN_STEND,         // 0x2010001f, 开合模始末
  d_clamp1_PO_O_CLS_OPN,              // 0x20100020, 开关模
  d_clamp1_PO_O_CLS_SOL2_2,           // 0x20100021, 关模2
  d_clamp1_PO_O_OPN_SOL2              // 0x20100022, 开模2
};

enum db_clamp1_PI{
  d_clamp1_PI_I_CLS_SLW = 0x20110000, // 0x20110000, 关模低速
  d_clamp1_PI_I_CLS_LOW,              // 0x20110001, 关模低压
  d_clamp1_PI_I_CLS_HIP,              // 0x20110002, 关模高压
  d_clamp1_PI_I_CLS_END,              // 0x20110003, 关模终
  d_clamp1_PI_I_CLS_LMT,              // 0x20110004, 关模限位
  d_clamp1_PI_I_CLS_FST,              // 0x20110005, 关模快速
  d_clamp1_PI_I_RSV_CLS,              // 0x20110006, 关模备用
  d_clamp1_PI_I_HYD_SAF,              // 0x20110007, 液压安全检知
  d_clamp1_PI_I_MEC_SAF,              // 0x20110008, 机械安全检知
  d_clamp1_PI_I_OPN_SLW,              // 0x20110009, 开模一慢
  d_clamp1_PI_I_OPN_FST,              // 0x2011000a, 开模二慢
  d_clamp1_PI_I_OPN_END,              // 0x2011000b, 开模终
  d_clamp1_PI_I_OPN_LMT,              // 0x2011000c, 开模限位
  d_clamp1_PI_I_AIR_PRES,             // 0x2011000d, 气压检知
  d_clamp1_PI_I_MOLDHEAT,             // 0x2011000e, 模温警报
  d_clamp1_PI_I_EMGUP,                // 0x2011000f, 紧急回升
  d_clamp1_PI_I_CLS_LIFT,             // 0x20110010, 左合模
  d_clamp1_PI_I_CLS_RIGHT,            // 0x20110011, 右合模
  d_clamp1_PI_I_CLS_UPGA,             // 0x20110012, 上顶盖
  d_clamp1_PI_I_CLP_CHK,              // 0x20110013, 模板检知
  d_clamp1_PI_I_ERR_PRU,              // 0x20110014, 不良品检知
  d_clamp1_PI_I_CLPSAF_CHK,           // 0x20110015, 液压保险检知
  d_clamp1_PI_I_MOLDSAFE,             // 0x20110016, 液压夹具安全
  d_clamp1_PI_I_MEC_PROTECT,          // 0x20110017, 机械保护
  d_clamp1_PI_I_CLP_SWITCH,           // 0x20110018, 脚踏开关
  d_clamp1_PI_I_CLP_LIGHT             // 0x20110019, 光幕安全检知
};

enum db_clamp1_MACHSET{
  d_clamp1_MACHSET_CLMP_METERCH = 0x20120000, // 0x20120000, 开关模电子尺通道号
  d_clamp1_MACHSET_CLMP_PRESSCH,              // 0x20120001, 模腔压力传感器通道
  d_clamp1_MACHSET_DAPRES_CLSRAMP,            // 0x20120002, 关模启动压力斜率
  d_clamp1_MACHSET_DAFLOW_CLSRAMP,            // 0x20120003, 关模启动速度斜率
  d_clamp1_MACHSET_DAPRES_CLSRAMP2,           // 0x20120004, 关模快速压力斜率
  d_clamp1_MACHSET_DAFLOW_CLSRAMP2,           // 0x20120005, 关模快速速度斜率
  d_clamp1_MACHSET_DAPRES_CLSRAMPLOWPRES,     // 0x20120006, 关模低压压力斜率
  d_clamp1_MACHSET_DAFLOW_CLSRAMPLOWPRES,     // 0x20120007, 关模低压速度斜率
  d_clamp1_MACHSET_DAPRES_CLSRAMPHIPRES,      // 0x20120008, 关模高压压力斜率
  d_clamp1_MACHSET_DAFLOW_CLSRAMPHIPRES,      // 0x20120009, 关模高压速度斜率
  d_clamp1_MACHSET_DAPRES_CLSOFFRAMP,         // 0x2012000a, 关模关闭压力斜率
  d_clamp1_MACHSET_DAFLOW_CLSOFFRAMP,         // 0x2012000b, 关模关闭速度斜率
  d_clamp1_MACHSET_DAPRES_CLSSETUP,           // 0x2012000c, 关模慢速调试压力
  d_clamp1_MACHSET_DAFLOW_CLSSETUP,           // 0x2012000d, 关模慢速调试速度
  d_clamp1_MACHSET_DAPRES_OPNRAMP,            // 0x2012000e, 开模启动压力斜率
  d_clamp1_MACHSET_DAFLOW_OPNRAMP,            // 0x2012000f, 开模启动速度斜率
  d_clamp1_MACHSET_DAPRES_OPENFASTRAMP,       // 0x20120010, 开模二段压力斜率
  d_clamp1_MACHSET_DAFLOW_OPENFASTRAMP,       // 0x20120011, 开模二段速度斜率
  d_clamp1_MACHSET_DAPRES_OPENFASTRAMP1,      // 0x20120012, 开模三段压力斜率
  d_clamp1_MACHSET_DAFLOW_OPENFASTRAMP1,      // 0x20120013, 开模三段速度斜率
  d_clamp1_MACHSET_DAPRES_OPENFASTRAMP2,      // 0x20120014, 开模四段压力斜率
  d_clamp1_MACHSET_DAFLOW_OPENFASTRAMP2,      // 0x20120015, 开模四段速度斜率
  d_clamp1_MACHSET_DAPRES_OPNRAMP2,           // 0x20120016, 开模终止压力斜率
  d_clamp1_MACHSET_DAFLOW_OPNRAMP2,           // 0x20120017, 开模终止速度斜率
  d_clamp1_MACHSET_DAPRES_OPNOFFRAMP,         // 0x20120018, 开模关闭压力斜率
  d_clamp1_MACHSET_DAFLOW_OPNOFFRAMP,         // 0x20120019, 开模关闭速度斜率
  d_clamp1_MACHSET_DAPRES_OPNSETUP,           // 0x2012001a, 开模慢速调试压力
  d_clamp1_MACHSET_DAFLOW_OPNSETUP,           // 0x2012001b, 开模慢速调试速度
  d_clamp1_MACHSET_TMDLY_VLV_OFFCLSULTRA,     // 0x2012001c, 关模差动阀延迟关计时
  d_clamp1_MACHSET_TMDLY_VLV_ONCLSULTRA,      // 0x2012001d, 关模差动阀延迟开计时
  d_clamp1_MACHSET_TMDLY_VLV_ONCLSLOW,        // 0x2012001e, 关模低压阀延迟开计时
  d_clamp1_MACHSET_TMDLY_VLV_OFFCLSLOW,       // 0x2012001f, 关模低压阀延迟关计时
  d_clamp1_MACHSET_TMDLY_VLV_OFFCLSEND,       // 0x20120020, 关模阀延迟关计时
  d_clamp1_MACHSET_TMDLY_VLV_OFFOPENSLOWSTART, // 0x20120021, 开模慢速阀延迟关计时
  d_clamp1_MACHSET_TMDLY_VLV_ONOPENSLOWSTART, // 0x20120022, 开模慢速阀延迟开计时
  d_clamp1_MACHSET_TMDLY_VLV_OFFOPENEND,      // 0x20120023, 开模阀延迟关计时
  d_clamp1_MACHSET_TMDLY_HYDCLSHIGH,          // 0x20120024, 关模高压延迟进计时
  d_clamp1_MACHSET_TMDLY_CLSENDDLY,           // 0x20120025, 关模动作结束延时
  d_clamp1_MACHSET_TM_OPENSLOWVLV,            // 0x20120026, 开模慢速阀动作时间
  d_clamp1_MACHSET_TMDLY_OPENEND,             // 0x20120027, 开模动作结束延时
  d_clamp1_MACHSET_ADPOSI_EFFCPR,             // 0x20120028, 开模位置有效区
  d_clamp1_MACHSET_CLMPCLS_HYDRDELAY,         // 0x20120029, 关模液压延迟开计时
  d_clamp1_MACHSET_CLMPOPN_HYDRDELAY,         // 0x2012002a, 开模液压延迟开计时
  d_clamp1_MACHSET_TMDLY_VLV_ONCLS,           // 0x2012002b, 关模阀延迟开计时
  d_clamp1_MACHSET_TMDLY_VLV_ONOPN,           // 0x2012002c, 开模阀延迟开计时
  d_clamp1_MACHSET_TMDLY_OPENBEGIN,           // 0x2012002d, 开模前延时
  d_clamp1_MACHSET_TMDLY_CLSBEGIN,            // 0x2012002e, 关模前延时
  d_clamp1_MACHSET_ADPOS_COREOUTEFT,          // 0x2012002f, 途中中子退下限位置
  d_clamp1_MACHSET_FL_ADPOSROBOT,             // 0x20120030, 途中机械手功能
  d_clamp1_MACHSET_TM_CLS_PROTECT,            // 0x20120031, 关模启动慢速保护时间
  d_clamp1_MACHSET_FL_CLSHIPRES2,             // 0x20120032, 关模高压二段
  d_clamp1_MACHSET_DAPRES_CLSRAMPHIPRES2,     // 0x20120033, 关模高压二段压力斜率
  d_clamp1_MACHSET_DAFLOW_CLSRAMPHIPRES2,     // 0x20120034, 关模高压二段速度斜率
  d_clamp1_MACHSET_TM_MANUALOUT,              // 0x20120035, 途中机械手输出时间
  d_clamp1_MACHSET_DAPRES_SPDOPNRAMP1,        // 0x20120036, 快速开模启动压力斜率
  d_clamp1_MACHSET_DAFLOW_SPDOPNRAMP1,        // 0x20120037, 快速开模启动速度斜率
  d_clamp1_MACHSET_DAFLOW_SPDOPNRAMP2,        // 0x20120038, 快速开模快速速度斜率
  d_clamp1_MACHSET_DAFLOW_SPDOPNRAMP3,        // 0x20120039, 快速开模慢速速度斜率
  d_clamp1_MACHSET_DAFLOW_SPDOPNRAMP4,        // 0x2012003a, 快速开模关闭速度斜率
  d_clamp1_MACHSET_TM_CLSHI_DEFAULT,          // 0x2012003b, 关模高压锁死保护时间
  d_clamp1_MACHSET_DAPRESS_OPNRAMPSTART,      // 0x2012003c, 途中开模启动压力斜率
  d_clamp1_MACHSET_DAPFLOW_OPNRAMPSTART,      // 0x2012003d, 途中开模启动速度斜率
  d_clamp1_MACHSET_ADPOS_OPNSTART,            // 0x2012003e, 途中开模启动距离
  d_clamp1_MACHSET_TM_OPNPROTECT,             // 0x2012003f, 途中开模启动保护时间
  d_clamp1_MACHSET_TMDLY_VLV_OFFCLSEND2,      // 0x20120040, 关模安全阀延迟关计时
  d_clamp1_MACHSET_TM_SCLAMPOPN,              // 0x20120041, 异常开模时间
  d_clamp1_MACHSET_TM_SCLAPMCLS,              // 0x20120042, 异常关模时间
  d_clamp1_MACHSET_SPCLAMP,                   // 0x20120043, 异常开模功能
  d_clamp1_MACHSET_TM_CLAMPOPENULT2,          // 0x20120044, 开模差动动作时间
  d_clamp1_MACHSET_CLSEND,                    // 0x20120045, 合模结束方式
  d_clamp1_MACHSET_FL_CLSOPERRAIRB,           // 0x20120046, 异常开模吹气
  d_clamp1_MACHSET_FL_DROPSENORCHEK,          // 0x20120047, 检出电眼检测
  d_clamp1_MACHSET_DIA_CYLINDER_M,            // 0x20120048, 开关模油缸直径
  d_clamp1_MACHSET_DIA_PISDIA_M,              // 0x20120049, 开关模活塞杆直径
  d_clamp1_MACHSET_EFFICIENCY_FACTOR_CLS,     // 0x2012004a, 关模效率系数
  d_clamp1_MACHSET_EFFICIENCY_FACTOR_OPN,     // 0x2012004b, 开模效率系数
  d_clamp1_MACHSET_DIA_LEFT_RIGHT,            // 0x2012004c, 开关模活塞杆
  d_clamp1_MACHSET_DIA_OIL_NUM,               // 0x2012004d, 开关模油缸个数
  d_clamp1_MACHSET_LOWPRES_CLS,               // 0x2012004e, 关模底压
  d_clamp1_MACHSET_LOWFLOW_CLS,               // 0x2012004f, 关模底流
  d_clamp1_MACHSET_LOWPRES_OPN,               // 0x20120050, 开模底压
  d_clamp1_MACHSET_LOWFLOW_OPN,               // 0x20120051, 开模底流
  d_clamp1_MACHSET_SMOOTH_LEVEL_CLS,          // 0x20120052, 关模平滑
  d_clamp1_MACHSET_SMOOTH_LEVEL_OPN,          // 0x20120053, 开模平滑
  d_clamp1_MACHSET_DAPRESS_OPNMIDSTART,       // 0x20120054, 途中开模启动压力
  d_clamp1_MACHSET_DAPFLOW_OPNMIDSTART,       // 0x20120055, 途中开模启动速度
  d_clamp1_MACHSET_DAPRESS_CLSMIDSTART,       // 0x20120056, 途中关模启动压力
  d_clamp1_MACHSET_DAPFLOW_CLSMIDSTART,       // 0x20120057, 途中关模启动速度
  d_clamp1_MACHSET_DAPRESS_CLSMIDRAMPSTART,   // 0x20120058, 途中关模启动压力斜率
  d_clamp1_MACHSET_DAPFLOW_CLSMIDRAMPSTART,   // 0x20120059, 途中关模启动速度斜率
  d_clamp1_MACHSET_ADPOS_CLSMIDSTART,         // 0x2012005a, 途中关模启动距离
  d_clamp1_MACHSET_TM_CLSMIDPROTECT,          // 0x2012005b, 途中关模启动保护时间
  d_clamp1_MACHSET_DAPRESS_CLSMIDEND_RAMPSTART, // 0x2012005c, 途中关模结束压力斜率
  d_clamp1_MACHSET_DAPFLOW_CLSMIDEND_RAMPSTART, // 0x2012005d, 途中关模结束速度斜率
  d_clamp1_MACHSET_DAPRESS_OPNMIDEND_RAMPSTART, // 0x2012005e, 途中开模结束压力斜率
  d_clamp1_MACHSET_DAPFLOW_OPNMIDEND_RAMPSTART, // 0x2012005f, 途中开模结束速度斜率
  d_clamp1_MACHSET_DAPRESS_OPNPOSPIDEND,      // 0x20120060, 开模位置结束压力
  d_clamp1_MACHSET_DAPFLOW_OPNPOSPIDEND,      // 0x20120061, 开模位置结束速度
  d_clamp1_MACHSET_DAPRESS_CLSPOSPIDEND,      // 0x20120062, 关模位置结束压力
  d_clamp1_MACHSET_DAPFLOW_CLSPOSPIDEND,      // 0x20120063, 关模位置结束速度
  d_clamp1_MACHSET_DAPFLOW_CLPOPN_VLVLIMIT,   // 0x20120064, 开模速度限制
  d_clamp1_MACHSET_DAPFLOW_VLV_CLS,           // 0x20120065, 关模速度模拟
  d_clamp1_MACHSET_DAPFLOW_LOWVLV_CLS,        // 0x20120066, 低压速度模拟
  d_clamp1_MACHSET_DAPFLOW_HIGVLV_CLS,        // 0x20120067, 高压速度模拟
  d_clamp1_MACHSET_DABACKPRES_OPEN1RAMP,      // 0x20120068, 开模一段背压压力斜率
  d_clamp1_MACHSET_DABACKPRES_OPEN2RAMP,      // 0x20120069, 开模二段背压压力斜率
  d_clamp1_MACHSET_DABACKPRES_OPEN3RAMP,      // 0x2012006a, 开模三段背压压力斜率
  d_clamp1_MACHSET_DABACKPRES_OPEN4RAMP,      // 0x2012006b, 开模四段背压压力斜率
  d_clamp1_MACHSET_DABACKPRES_OPEN5RAMP,      // 0x2012006c, 开模五段背压压力斜率
  d_clamp1_MACHSET_DABACKPRES_OPENOFFRAMP,    // 0x2012006d, 开模背压关闭压力斜率
  d_clamp1_MACHSET_DABACKPRES_CLS1RAMP,       // 0x2012006e, 关模一段背压压力斜率
  d_clamp1_MACHSET_DABACKPRES_CLS2RAMP,       // 0x2012006f, 关模二段背压压力斜率
  d_clamp1_MACHSET_DABACKPRES_CLS3RAMP,       // 0x20120070, 关模三段背压压力斜率
  d_clamp1_MACHSET_DABACKPRES_CLSLOWPRESRAMP, // 0x20120071, 关模低压背压压力斜率
  d_clamp1_MACHSET_DABACKPRES_CLSHIGHPRESRAMP, // 0x20120072, 关模高压背压压力斜率
  d_clamp1_MACHSET_DABACKPRES_CLSOFFRAMP,     // 0x20120073, 关模背压关闭压力斜率
  d_clamp1_MACHSET_FL_SPC_CLS,                // 0x20120074, 特殊关模阀
  d_clamp1_MACHSET_FL_SPC_OPN,                // 0x20120075, 特殊开模阀
  d_clamp1_MACHSET_TM_OPN_BACK,               // 0x20120076, 开模背压计时
  d_clamp1_MACHSET_FL_ADMETER_TEST,           // 0x20120077, 开模电子尺检测
  d_clamp1_MACHSET_FL_FORCED_USE,             // 0x20120078, 高压二特殊使用
  d_clamp1_MACHSET_FL_PRESS_LIMIT,            // 0x20120079, 高压压力限制
  d_clamp1_MACHSET_ENERGY_FACTOR_OPN,         // 0x2012007a, 开模能耗系数
  d_clamp1_MACHSET_FL_ENERGY_SAVE,            // 0x2012007b, 开模节能
  d_clamp1_MACHSET_TM_CLAMPSAFE,              // 0x2012007c, 安全阀检测无信号时间
  d_clamp1_MACHSET_TM_MACHINESAFE,            // 0x2012007d, 安全阀检测有信号时间
  d_clamp1_MACHSET_FL_CLAMP_FINE_TUNING,      // 0x2012007e, 开模微调功能
  d_clamp1_MACHSET_CLAMPOPNVLV_CH,            // 0x2012007f, 开模比例阀硬件通道
  d_clamp1_MACHSET_CLAMPOPNVLV_DIR,           // 0x20120080, 开模比例阀输出电压
  d_clamp1_MACHSET_CLAMPCLSVLV_CH,            // 0x20120081, 关模比例阀硬件通道
  d_clamp1_MACHSET_CLAMPCLSVLV_DIR            // 0x20120082, 关模比例阀输出电压
};

enum db_clamp1_MOLDSET{
  d_clamp1_MOLDSET_FL_CLSFAST = 0x20130000, // 0x20130000, 关模快速阀
  d_clamp1_MOLDSET_FL_CLSLOWPRES,           // 0x20130001, 关模低压阀
  d_clamp1_MOLDSET_TM_HOLDCLSHIGH,          // 0x20130002, 关模高压保持计时
  d_clamp1_MOLDSET_FL_CLS3,                 // 0x20130003, 关模三段使用
  d_clamp1_MOLDSET_FL_OPENDRAINFUNCTIO,     // 0x20130004, 开模泄压选择
  d_clamp1_MOLDSET_TM_OPENDRAIN,            // 0x20130005, 开模泄压计时
  d_clamp1_MOLDSET_FL_OPENFAST,             // 0x20130006, 开模快速阀
  d_clamp1_MOLDSET_FL_OPENFAST2POS,         // 0x20130007, 开模慢速阀
  d_clamp1_MOLDSET_ADPOSI_EJTTRAVEL,        // 0x20130008, 开模联动位置
  d_clamp1_MOLDSET_FL_OPN4,                 // 0x20130009, 开模四段
  d_clamp1_MOLDSET_FL_MOLDREACHCHK,         // 0x2013000a, 开模停机模式
  d_clamp1_MOLDSET_CN_PRODUCTSETTING1,      // 0x2013000b, 目标数1
  d_clamp1_MOLDSET_CN_PRODUCTSETTING2,      // 0x2013000c, 目标数2
  d_clamp1_MOLDSET_CN_PACKAGE,              // 0x2013000d, 每包开模数
  d_clamp1_MOLDSET_TM_CLSPROTECTDEFAULTMAX, // 0x2013000e, 关模低压计时上限
  d_clamp1_MOLDSET_TM_CLSHIGHDEFAULTMAX,    // 0x2013000f, 关模高压计时上限
  d_clamp1_MOLDSET_TM_CLAMPCLSDEFAULTMAX,   // 0x20130010, 关模计时上限
  d_clamp1_MOLDSET_TM_CLAMPOPNDEFAULTMAX,   // 0x20130011, 开模计时上限
  d_clamp1_MOLDSET_TMDLY_BAKEBEFOREEXHAUST, // 0x20130012, 排气前延迟
  d_clamp1_MOLDSET_TMDLY_BAKEAFTEREXHAUST,  // 0x20130013, 排气计时
  d_clamp1_MOLDSET_TM_EXHAUSTOPEN,          // 0x20130014, 排气开模计时
  d_clamp1_MOLDSET_ADPOSI_EXHAUSTOPEN,      // 0x20130015, 排气开模距离
  d_clamp1_MOLDSET_FL_EXHAUSTMODE,          // 0x20130016, 排气方式
  d_clamp1_MOLDSET_FL_LUBBYPASS,            // 0x20130017, 前二模不做润滑
  d_clamp1_MOLDSET_POS_ROBOT,               // 0x20130018, 途中机械手启动位置
  d_clamp1_MOLDSET_DAPOSI_HIPRSCLAMPDOWN,   // 0x20130019, 锁模压力最小有效区
  d_clamp1_MOLDSET_DAPOSI_HIPRSCLAMPUP,     // 0x2013001a, 锁模压力最大有效区
  d_clamp1_MOLDSET_TM_CLMPCLSHI,            // 0x2013001b, 锁模高压计时
  d_clamp1_MOLDSET_Qc_DeviationSet_ClsTm,   // 0x2013001c, 合模时间允许偏差
  d_clamp1_MOLDSET_Qc_TmSet_ClsTm,          // 0x2013001d, 合模时间设定值
  d_clamp1_MOLDSET_Qc_Mode_ClsTm,           // 0x2013001e, 合模时间监控
  d_clamp1_MOLDSET_Qc_DeviationSet_ClsLow,  // 0x2013001f, 合模低压时间允许偏差
  d_clamp1_MOLDSET_Qc_TmSet_ClsLow,         // 0x20130020, 合模低压时间设定值
  d_clamp1_MOLDSET_Qc_Mode_ClsLow,          // 0x20130021, 合模低压监控
  d_clamp1_MOLDSET_Qc_DeviationSet_ClsHi,   // 0x20130022, 合模高压时间允许偏差
  d_clamp1_MOLDSET_Qc_TmSet_ClsHi,          // 0x20130023, 合模高压时间设定值
  d_clamp1_MOLDSET_Qc_Mode_ClsHi,           // 0x20130024, 合模高压时间监控
  d_clamp1_MOLDSET_Qc_DeviationSet_OpnEnd,  // 0x20130025, 开模终止位置允许偏差
  d_clamp1_MOLDSET_Qc_TmSet_OpnEnd,         // 0x20130026, 开模终止位置设定值
  d_clamp1_MOLDSET_Qc_Mode_OpnEnd,          // 0x20130027, 开模终止位置监控
  d_clamp1_MOLDSET_Qc_DeviationSet_OpnTm,   // 0x20130028, 开模时间允许偏差
  d_clamp1_MOLDSET_Qc_TmSet_OpnTm,          // 0x20130029, 开模时间设定值
  d_clamp1_MOLDSET_Qc_Mode_OpnTm,           // 0x2013002a, 开模时间监控
  d_clamp1_MOLDSET_Qc_DeviationSet_OpnSpd,  // 0x2013002b, 开模速度允许偏差
  d_clamp1_MOLDSET_Qc_TmSet_OpnSpd,         // 0x2013002c, 开模速度设定值
  d_clamp1_MOLDSET_Qc_Mode_OpnSpd,          // 0x2013002d, 开模速度监控
  d_clamp1_MOLDSET_ADPOSI_EJTTRAVELCLOSE,   // 0x2013002e, 关模联动位置
  d_clamp1_MOLDSET_FL_OPEN2,                // 0x2013002f, 开模二段使用
  d_clamp1_MOLDSET_TM_RETARD,               // 0x20130030, 开模减速时间
  d_clamp1_MOLDSET_Tm_Cls_Ult,              // 0x20130031, 开模模差动阀动作时间
  d_clamp1_MOLDSET_FL_ZEPOIT,               // 0x20130032, 原点锁模功能
  d_clamp1_MOLDSET_FL_ZEPOITPOS,            // 0x20130033, 原点锁模位置
  d_clamp1_MOLDSET_FL_ZEPOITFLW,            // 0x20130034, 原点锁模速度
  d_clamp1_MOLDSET_FL_MachType,             // 0x20130035, 机型选择
  d_clamp1_MOLDSET_TM_HLP_VLV_ONDelay,      // 0x20130036, 差动辅助阀延迟开计时
  d_clamp1_MOLDSET_TM_HLP_VLV_OFFDelay,     // 0x20130037, 差动辅助阀延迟关计时
  d_clamp1_MOLDSET_TM_OPN_BACK,             // 0x20130038, 开模背压时间
  d_clamp1_MOLDSET_BL_OPNCLS,               // 0x20130039, 调模慢速比例
  d_clamp1_MOLDSET_CLS_TWO_PUMP,            // 0x2013003a, 关摸多泵功能
  d_clamp1_MOLDSET_FL_AUTO_PRU,             // 0x2013003b, 警报时不良品监控
  d_clamp1_MOLDSET_FL_MAUL_PRU,             // 0x2013003c, 手动时不良品监控
  d_clamp1_MOLDSET_FL_FISR_PRU,             // 0x2013003d, 自动初期不良品模数
  d_clamp1_MOLDSET_FL_Dely_PRU,             // 0x2013003e, 不良品延迟关
  d_clamp1_MOLDSET_FL_USE_PRU,              // 0x2013003f, 不良品检测功能
  d_clamp1_MOLDSET_SAFEVLVOFF,              // 0x20130040, 安全阀延迟检测时间
  d_clamp1_MOLDSET_PRESSTART_CLOSE_MID,     // 0x20130041, 关模手动启动压力斜率
  d_clamp1_MOLDSET_FLOWSTART_CLOSE_MID,     // 0x20130042, 关模手动启动速度斜率
  d_clamp1_MOLDSET_PRESSTOP_CLOSE_MID,      // 0x20130043, 关模手动停止压力斜率
  d_clamp1_MOLDSET_FLOWSTOP_CLOSE_MID,      // 0x20130044, 关模手动停止速度斜率
  d_clamp1_MOLDSET_PRESSTART_OPEN_MID,      // 0x20130045, 开模手动启动压力斜率
  d_clamp1_MOLDSET_FLOWSTART_OPEN_MID,      // 0x20130046, 开模手动启动速度斜率
  d_clamp1_MOLDSET_PRESSTOP_OPEN_MID,       // 0x20130047, 开模手动停止压力斜率
  d_clamp1_MOLDSET_FLOWSTOP_OPEN_MID,       // 0x20130048, 开模手动停止速度斜率
  d_clamp1_MOLDSET_FL_OPEN_SMOOTH,          // 0x20130049, 开模平稳
  d_clamp1_MOLDSET_FL_CLSE_SMOOTH,          // 0x2013004a, 关模平稳
  d_clamp1_MOLDSET_SLOPE_MAX_CLS_SPD,       // 0x2013004b, 关模最大速度斜率
  d_clamp1_MOLDSET_SLOPE_MAX_OPN_SPD,       // 0x2013004c, 开模最大速度斜率
  d_clamp1_MOLDSET_SLOPE_MAX_CLS_PRES,      // 0x2013004d, 关模最大压力斜率
  d_clamp1_MOLDSET_SLOPE_MAX_OPN_PRES,      // 0x2013004e, 开模最大压力斜率
  d_clamp1_MOLDSET_PUMP_K,                  // 0x2013004f, 开模减速调节系数
  d_clamp1_MOLDSET_PUMP_DEAD,               // 0x20130050, 开模死区时间
  d_clamp1_MOLDSET_PUMP_K_CLS,              // 0x20130051, 关模减速调节系数
  d_clamp1_MOLDSET_PUMP_DEAD_CLS,           // 0x20130052, 关模死区时间
  d_clamp1_MOLDSET_OPN_SPD_STARTON,         // 0x20130053, 开模启动速度斜率
  d_clamp1_MOLDSET_OPN_SPD_HALFWAY,         // 0x20130054, 开模中间速度斜率
  d_clamp1_MOLDSET_OPN_SPD_STOPOFF,         // 0x20130055, 开模终止速度斜率
  d_clamp1_MOLDSET_CLS_SPD_STARTON,         // 0x20130056, 关模启动速度斜率
  d_clamp1_MOLDSET_CLS_SPD_HALFWAY,         // 0x20130057, 关模中间速度斜率
  d_clamp1_MOLDSET_CLS_SPD_STOPOFF,         // 0x20130058, 关模终止速度斜率
  d_clamp1_MOLDSET_OPN_PRES_STARTON,        // 0x20130059, 开模启动压力斜率
  d_clamp1_MOLDSET_OPN_PRES_HALFWAY,        // 0x2013005a, 开模中间压力斜率
  d_clamp1_MOLDSET_OPN_PRES_STOPOFF,        // 0x2013005b, 开模终止压力斜率
  d_clamp1_MOLDSET_CLS_PRES_STARTON,        // 0x2013005c, 关模启动压力斜率
  d_clamp1_MOLDSET_CLS_PRES_HALFWAY,        // 0x2013005d, 关模中间压力斜率
  d_clamp1_MOLDSET_CLS_PRES_STOPOFF,        // 0x2013005e, 关模终止压力斜率
  d_clamp1_MOLDSET_OPN_NUM,                 // 0x2013005f, 开模段数
  d_clamp1_MOLDSET_FL_CLSOPEN_MODE,         // 0x20130060, 开关模控制方式
  d_clamp1_MOLDSET_AD_POSIDISS,             // 0x20130061, 外设慢速距离
  d_clamp1_MOLDSET_FL_MANULOPN1SPD,         // 0x20130062, 手动开模一段速度模式
  d_clamp1_MOLDSET_FL_CLSWAITCHG,           // 0x20130063, 关模前储料等待
  d_clamp1_MOLDSET_CLPCLS_SPD_UTI,          // 0x20130064, 关模速度使用率
  d_clamp1_MOLDSET_CLPOPN_SPD_UTI,          // 0x20130065, 开模速度使用率
  d_clamp1_MOLDSET_TM_CLPOPN_ROBOT_PROTECT, // 0x20130066, 允许机械手开模时间
  d_clamp1_MOLDSET_FL_CLPPRE_NOTWAITAIRBLOW, // 0x20130067, 关模前等待吹气
  d_clamp1_MOLDSET_CLPCLS_SPD_ULT,          // 0x20130068, 关模差动速度使用率
  d_clamp1_MOLDSET_FL_CLAMP_CORE_RAMP,      // 0x20130069, 特殊中子斜率
  d_clamp1_MOLDSET_FL_OPN_LOCATE,           // 0x2013006a, 开模智能辅助功能
  d_clamp1_MOLDSET_MODE_OPN_LOCATE,         // 0x2013006b, 开模智能辅助模式
  d_clamp1_MOLDSET_OPN_POS_KP,              // 0x2013006c, 开模智能辅助系数
  d_clamp1_MOLDSET_OPN_OVER_SHOOT,          // 0x2013006d, 开模智能辅助补偿
  d_clamp1_MOLDSET_FL_CLS_LOCATE,           // 0x2013006e, 关模智能辅助功能
  d_clamp1_MOLDSET_MODE_CLS_LOCATE,         // 0x2013006f, 关模智能辅助模式
  d_clamp1_MOLDSET_CLS_POS_KP,              // 0x20130070, 关模智能辅助系数
  d_clamp1_MOLDSET_FL_EXTRAMP_LIMIT,        // 0x20130071, 开关模智能辅助外部斜率限制功能
  d_clamp1_MOLDSET_FL_LOCATE_DA,            // 0x20130072, 开关模智能辅助使用比例阀
  d_clamp1_MOLDSET_OPN_OVER_SHOOT_SYM,      // 0x20130073, 开模智能辅助补偿符号
  d_clamp1_MOLDSET_PRES_CLSMIDHOLD_RAMPOFF_LOCATE, // 0x20130074, 关模途中结束压力斜率
  d_clamp1_MOLDSET_FLOW_CLSMIDHOLD_RAMPOFF_LOCATE, // 0x20130075, 关模途中结束速度斜率
  d_clamp1_MOLDSET_PRES_OPNMIDHOLD_RAMPOFF_LOCATE, // 0x20130076, 开模途中结束压力斜率
  d_clamp1_MOLDSET_FLOW_OPNMIDHOLD_RAMPOFF_LOCATE, // 0x20130077, 开模途中结束速度斜率
  d_clamp1_MOLDSET_FLOW_CLSMIDERR_RAMPOFF_LOCATE, // 0x20130078, 关模手动放开速度斜率
  d_clamp1_MOLDSET_FLOW_OPNMIDERR_RAMPOFF_LOCATE, // 0x20130079, 开模手动放开速度斜率
  d_clamp1_MOLDSET_FL_LOCATE_DA_COEF_OPN,   // 0x2013007a, 开模比例阀放大系数
  d_clamp1_MOLDSET_FL_LOCATE_DA_COEF_CLS,   // 0x2013007b, 关模比例阀放大系数
  d_clamp1_MOLDSET_TM_CLPCLS_ROBOT_FILT,    // 0x2013007c, 关模机械手滤波时间
  d_clamp1_MOLDSET_FL_EXHAUST_CLAMPCLS,     // 0x2013007d, 排气后关模
  d_clamp1_MOLDSET_DAPRES_INJCLAMPOPN,      // 0x2013007e, 排气开模压力
  d_clamp1_MOLDSET_DAFLOW_INJCLAMPOPN,      // 0x2013007f, 排气开模速度
  d_clamp1_MOLDSET_FL_OPENSLOW_USE_ULT      // 0x20130080, 开模慢速阀使用差动阀
};

enum db_clamp1_ACTPARA{
  d_clamp1_ACTPARA_DAPRES_CLS1 = 0x20140000, // 0x20140000, 关模一段压力
  d_clamp1_ACTPARA_DAPRES_CLS2,              // 0x20140001, 关模二段压力
  d_clamp1_ACTPARA_DAPRES_CLS3,              // 0x20140002, 关模三段压力
  d_clamp1_ACTPARA_DAPRES_CLSPROTECT,        // 0x20140003, 关模低压模保压力
  d_clamp1_ACTPARA_DAPRES_CLSHIGH,           // 0x20140004, 关模高压锁模压力
  d_clamp1_ACTPARA_DAFLOW_CLS1,              // 0x20140005, 关模一段速度
  d_clamp1_ACTPARA_DAFLOW_CLS2,              // 0x20140006, 关模二段速度
  d_clamp1_ACTPARA_DAFLOW_CLS3,              // 0x20140007, 关模三段速度
  d_clamp1_ACTPARA_DAFLOW_CLSPROTECT,        // 0x20140008, 关模低压模保速度
  d_clamp1_ACTPARA_DAFLOW_CLSHIGH,           // 0x20140009, 关模高压锁模速度
  d_clamp1_ACTPARA_ADPOSI_CLS1END,           // 0x2014000a, 关模一段模板位置
  d_clamp1_ACTPARA_ADPOSI_CLS2END,           // 0x2014000b, 关模二段模板位置
  d_clamp1_ACTPARA_ADPOSI_CLS3END,           // 0x2014000c, 关模三段模板位置
  d_clamp1_ACTPARA_ADPOSI_CLSPROTECTEND,     // 0x2014000d, 关模低压模保模板位置
  d_clamp1_ACTPARA_DAPRES_OPENSLOW,          // 0x2014000e, 开模一段压力
  d_clamp1_ACTPARA_DAPRES_OPEN2,             // 0x2014000f, 开模二段压力
  d_clamp1_ACTPARA_DAPRES_OPEN3,             // 0x20140010, 开模三段压力
  d_clamp1_ACTPARA_DAPRES_OPEN4,             // 0x20140011, 开模四段压力
  d_clamp1_ACTPARA_DAPRES_OPENEND,           // 0x20140012, 开模五段压力
  d_clamp1_ACTPARA_DAFLOW_OPENSLOW,          // 0x20140013, 开模一段速度
  d_clamp1_ACTPARA_DAFLOW_OPEN2,             // 0x20140014, 开模二段速度
  d_clamp1_ACTPARA_DAFLOW_OPEN3,             // 0x20140015, 开模三段速度
  d_clamp1_ACTPARA_DAFLOW_OPEN4,             // 0x20140016, 开模四段速度
  d_clamp1_ACTPARA_DAFLOW_OPENEND,           // 0x20140017, 开模五段速度
  d_clamp1_ACTPARA_ADPOSI_OPENENDSLOW,       // 0x20140018, 开模一段模板位置
  d_clamp1_ACTPARA_ADPOSI_OPENEND2,          // 0x20140019, 开模二段模板位置
  d_clamp1_ACTPARA_ADPOSI_OPENEND3,          // 0x2014001a, 开模三段模板位置
  d_clamp1_ACTPARA_ADPOSI_OPENEND4,          // 0x2014001b, 开模四段模板位置
  d_clamp1_ACTPARA_ADPOSI_OPENENDEND,        // 0x2014001c, 开模五段模板位置
  d_clamp1_ACTPARA_DAPRES_OPENDRAIN,         // 0x2014001d, 开模泄压压力
  d_clamp1_ACTPARA_DAFLOW_OPENDRAIN,         // 0x2014001e, 开模泄压速度
  d_clamp1_ACTPARA_ADPOS_CLSHIPRES2,         // 0x2014001f, 关模高压锁模模板位置
  d_clamp1_ACTPARA_ADPOS_CLSSTART,           // 0x20140020, 关模启动距离
  d_clamp1_ACTPARA_DAPRES_CLSHIPRES2,        // 0x20140021, 关模高压2压力
  d_clamp1_ACTPARA_DAFLOW_CLSHIPRES2,        // 0x20140022, 关模高压2速度
  d_clamp1_ACTPARA_DAPRES_OPENSPD1,          // 0x20140023, 快速开模启动压力
  d_clamp1_ACTPARA_DAFLOW_OPENSPD1,          // 0x20140024, 快速开模启动速度
  d_clamp1_ACTPARA_ADPOS_OPENSPD1,           // 0x20140025, 快速开模启动位置
  d_clamp1_ACTPARA_DAFLOW_OPENSPD2,          // 0x20140026, 快速开模快速速度
  d_clamp1_ACTPARA_ADPOS_OPENSPD2,           // 0x20140027, 快速开模快速位置
  d_clamp1_ACTPARA_DAFLOW_OPENSPD3,          // 0x20140028, 快速开模结束速度
  d_clamp1_ACTPARA_ADPOS_OPENSPD3,           // 0x20140029, 快速开模结束位置
  d_clamp1_ACTPARA_DAPRES_SCLAMPOPN,         // 0x2014002a, 异常开模压力
  d_clamp1_ACTPARA_DAFLOW_SCLAMPOPN,         // 0x2014002b, 异常开模速度
  d_clamp1_ACTPARA_DAPRES_SCLAMPCLS,         // 0x2014002c, 异常关模压力
  d_clamp1_ACTPARA_DAFLOW_SCLAMPCLS,         // 0x2014002d, 异常关模速度
  d_clamp1_ACTPARA_DAPRES_OPENSPD2,          // 0x2014002e, 快速开模快速压力
  d_clamp1_ACTPARA_DAPRES_OPENSPD3,          // 0x2014002f, 快速开模二慢压力
  d_clamp1_ACTPARA_DAFLOW_VLV,               // 0x20140030, 开模一段阀口
  d_clamp1_ACTPARA_DAFLOW_VLVCLS,            // 0x20140031, 关模低压阀口
  d_clamp1_ACTPARA_DAPRES_CLS_RAMPSLOW,      // 0x20140032, 关模慢速启动压力斜率
  d_clamp1_ACTPARA_DAFLOW_CLS_RAMPSLOW,      // 0x20140033, 关模慢速启动速度斜率
  d_clamp1_ACTPARA_DAPRES_CLS_SLOW,          // 0x20140034, 关模慢速启动压力
  d_clamp1_ACTPARA_DAFLOW_CLS_SLOW,          // 0x20140035, 关模慢速启动速度
  d_clamp1_ACTPARA_ADPOSI_CLS_SLOW,          // 0x20140036, 关模慢速启动距离
  d_clamp1_ACTPARA_TM_CLS_SLOW,              // 0x20140037, 关模慢速启动时间
  d_clamp1_ACTPARA_PER_DAFLOW_CLAMPCLS,      // 0x20140038, 关模速度百分比
  d_clamp1_ACTPARA_PER_DAFLOW_CLAMPOPN,      // 0x20140039, 开模速度百分比
  d_clamp1_ACTPARA_PER_DAFLOW_LOWPRES,       // 0x2014003a, 关模低压速度使用率
  d_clamp1_ACTPARA_PER_DAFLOW_HIGHPRES,      // 0x2014003b, 关模高压速度使用率
  d_clamp1_ACTPARA_PER_DAFLOW_OPEN1,         // 0x2014003c, 开模一段速度使用率
  d_clamp1_ACTPARA_DABACKPRES_OPENSLOW,      // 0x2014003d, 开模一段背压压力
  d_clamp1_ACTPARA_DABACKPRES_OPEN2,         // 0x2014003e, 开模二段背压压力
  d_clamp1_ACTPARA_DABACKPRES_OPEN3,         // 0x2014003f, 开模三段背压压力
  d_clamp1_ACTPARA_DABACKPRES_OPEN4,         // 0x20140040, 开模四段背压压力
  d_clamp1_ACTPARA_DABACKPRES_OPENEND,       // 0x20140041, 开模终止背压压力
  d_clamp1_ACTPARA_DABACKPRES_CLS1,          // 0x20140042, 关模一段背压压力
  d_clamp1_ACTPARA_DABACKPRES_CLS2,          // 0x20140043, 关模二段背压压力
  d_clamp1_ACTPARA_DABACKPRES_CLS3,          // 0x20140044, 关模三段背压压力
  d_clamp1_ACTPARA_DABACKPRES_CLSLOWPRES,    // 0x20140045, 关模低压背压压力
  d_clamp1_ACTPARA_DABACKPRES_CLSHIGHPRES,   // 0x20140046, 关模高压背压压力
  d_clamp1_ACTPARA_ADPOSI_OPENENDEND_MOLD,   // 0x20140047, 开模五段模板位置
  d_clamp1_ACTPARA_FL_MID_ROBOT,             // 0x20140048, 途中机械手强制
  d_clamp1_ACTPARA_ADPOS_ROBOT,              // 0x20140049, 开模机械手位置
  d_clamp1_ACTPARA_ADPOS_ULT_CLPOPEN,        // 0x2014004a, 开模差动结束位置
  d_clamp1_ACTPARA_DAFLOW_HI_VLVCLS          // 0x2014004b, 关模高压阀口
};

enum db_clamp1_HYDR{
  d_clamp1_HYDR_HYDR_PORT_CLAMPCLS = 0x20150000, // 0x20150000, 
  d_clamp1_HYDR_HYDR_PUMPS_CLSSETUP,             // 0x20150001, 
  d_clamp1_HYDR_HYDR_PUMPS_CLSSLOW,              // 0x20150002, 
  d_clamp1_HYDR_HYDR_PUMPS_CLSFAST,              // 0x20150003, 
  d_clamp1_HYDR_HYDR_PUMPS_CLSPROTECT,           // 0x20150004, 
  d_clamp1_HYDR_HYDR_PUMPS_CLSHIG,               // 0x20150005, 
  d_clamp1_HYDR_HYDR_PORT_CLAMPOPN,              // 0x20150006, 
  d_clamp1_HYDR_HYDR_PUMPS_OPNSETUP,             // 0x20150007, 
  d_clamp1_HYDR_HYDR_PUMPS_OPNDRAIN,             // 0x20150008, 
  d_clamp1_HYDR_HYDR_PUMPS_OPNSLOW,              // 0x20150009, 
  d_clamp1_HYDR_HYDR_PUMPS_OPNFAST,              // 0x2015000a, 
  d_clamp1_HYDR_HYDR_PUMPS_OPNEND,               // 0x2015000b, 
  d_clamp1_HYDR_SERVO_KP_CLSSETUP,               // 0x2015000c, 
  d_clamp1_HYDR_SERVO_KI_CLSSETUP,               // 0x2015000d, 
  d_clamp1_HYDR_SERVO_KD_CLSSETUP,               // 0x2015000e, 
  d_clamp1_HYDR_SERVO_KP_CLS,                    // 0x2015000f, 关模 P
  d_clamp1_HYDR_SERVO_KI_CLS,                    // 0x20150010, 关模 I
  d_clamp1_HYDR_SERVO_KD_CLS,                    // 0x20150011, 关模 D
  d_clamp1_HYDR_SERVO_KP_HIPRS,                  // 0x20150012, 压模 P
  d_clamp1_HYDR_SERVO_KI_HIPRS,                  // 0x20150013, 压模 I
  d_clamp1_HYDR_SERVO_KD_HIPRS,                  // 0x20150014, 压模 D
  d_clamp1_HYDR_SERVO_KP_OPNSETUP,               // 0x20150015, 
  d_clamp1_HYDR_SERVO_KI_OPNSETUP,               // 0x20150016, 
  d_clamp1_HYDR_SERVO_KD_OPNSETUP,               // 0x20150017, 
  d_clamp1_HYDR_SERVO_KP_OPN,                    // 0x20150018, 开模 P
  d_clamp1_HYDR_SERVO_KI_OPN,                    // 0x20150019, 开模 I
  d_clamp1_HYDR_SERVO_KD_OPN,                    // 0x2015001a, 开模 D
  d_clamp1_HYDR_POSN_KP_OPN,                     // 0x2015001b, 开模位置 P
  d_clamp1_HYDR_POSN_KI_OPN,                     // 0x2015001c, 开模位置 I
  d_clamp1_HYDR_POSN_KD_OPN,                     // 0x2015001d, 开模位置 D
  d_clamp1_HYDR_POSN_KP_CLS,                     // 0x2015001e, 关模位置 P
  d_clamp1_HYDR_POSN_KI_CLS,                     // 0x2015001f, 关模位置 I
  d_clamp1_HYDR_POSN_KD_CLS                      // 0x20150020, 关模位置 D
};

enum db_clamp1_STATE{
  d_clamp1_STATE_FL_ERROR0 = 0x20160000, // 0x20160000, 
  d_clamp1_STATE_FL_ERROR1,              // 0x20160001, 
  d_clamp1_STATE_CN_MOLDOPNNUM0,         // 0x20160002, 
  d_clamp1_STATE_CN_MOLDOPNNUM1,         // 0x20160003, 
  d_clamp1_STATE_CN_CURPACKMOLD,         // 0x20160004, 良品总模数
  d_clamp1_STATE_CN_VALIDMOLD,           // 0x20160005, 
  d_clamp1_STATE_CN_CLAMPLUBRIC,         // 0x20160006, 润滑计数
  d_clamp1_STATE_TM_CLAMPCLS,            // 0x20160007, 关模计时实际值
  d_clamp1_STATE_TM_CLAMPCLS_MAX,        // 0x20160008, 关模计时最大值
  d_clamp1_STATE_TM_CLAMPCLSLO,          // 0x20160009, 关模低压计时实际值
  d_clamp1_STATE_TM_CLAMPCLSLO_MAX,      // 0x2016000a, 关模低压计时最大值
  d_clamp1_STATE_TM_CLAMPCLSHI,          // 0x2016000b, 
  d_clamp1_STATE_TM_CLAMPCLSHI_MAX,      // 0x2016000c, 关模高压计时最大值
  d_clamp1_STATE_ADPOSI_OPNEND,          // 0x2016000d, 开模位置实际值
  d_clamp1_STATE_TM_CLAMPOPN,            // 0x2016000e, 开模计时实际值
  d_clamp1_STATE_TM_CLAMPOPN_MAX,        // 0x2016000f, 开模计时最大值
  d_clamp1_STATE_CLAMP_SPEED,            // 0x20160010, 开模速度
  d_clamp1_STATE_DAPRES_CLSHI_MAX,       // 0x20160011, 锁模压力最大值
  d_clamp1_STATE_DAPRES_CLSHIAVR,        // 0x20160012, 锁模压力平均值
  d_clamp1_STATE_TM_CLAMPMOV,            // 0x20160013, 移模计时
  d_clamp1_STATE_CLAMP_OPN_SPD,          // 0x20160014, 开模速度均值
  d_clamp1_STATE_Qc_Avg_Cls,             // 0x20160015, 合模时间平均值
  d_clamp1_STATE_Qc_Deviation_Max_Cls,   // 0x20160016, 合模时间最大偏差值
  d_clamp1_STATE_Qc_Unq_Cnt_Cls,         // 0x20160017, 合模时间不合格数
  d_clamp1_STATE_Qc_Unq_Rate_Cls,        // 0x20160018, 合模时间不合格率
  d_clamp1_STATE_Qc_Max_Cls,             // 0x20160019, 合模时间最大值
  d_clamp1_STATE_Qc_Min_Cls,             // 0x2016001a, 合模时间最小值
  d_clamp1_STATE_Qc_Avg_ClsLow,          // 0x2016001b, 合模低压时间平均值
  d_clamp1_STATE_Qc_Deviation_Max_ClsLow, // 0x2016001c, 合模低压时间最大偏差值
  d_clamp1_STATE_Qc_Unq_Cnt_ClsLow,      // 0x2016001d, 合模低压时间不合格数
  d_clamp1_STATE_Qc_Unq_Rate_ClsLow,     // 0x2016001e, 合模低压时间不合格率
  d_clamp1_STATE_Qc_Max_ClsLow,          // 0x2016001f, 合模低压时间最大值
  d_clamp1_STATE_Qc_Min_ClsLow,          // 0x20160020, 合模低压时间最小值
  d_clamp1_STATE_Qc_Avg_ClsHi,           // 0x20160021, 合模高压时间平均值
  d_clamp1_STATE_Qc_Deviation_Max_ClsHi, // 0x20160022, 合模高压时间最大偏差值
  d_clamp1_STATE_Qc_Unq_Cnt_ClsHi,       // 0x20160023, 合模高压时间不合格数
  d_clamp1_STATE_Qc_Unq_Rate_ClsHi,      // 0x20160024, 合模高压时间不合格率
  d_clamp1_STATE_Qc_Max_ClsHi,           // 0x20160025, 合模高压时间最大值
  d_clamp1_STATE_Qc_Min_ClsHi,           // 0x20160026, 合模高压时间最小值
  d_clamp1_STATE_Qc_Avg_OpnEnd,          // 0x20160027, 开模位置平均值
  d_clamp1_STATE_Qc_Deviation_Max_OpnEnd, // 0x20160028, 开模位置最大偏差值
  d_clamp1_STATE_Qc_Unq_Cnt_OpnEnd,      // 0x20160029, 开模位置不合格数
  d_clamp1_STATE_Qc_Unq_Rate_OpnEnd,     // 0x2016002a, 开模位置不合格率
  d_clamp1_STATE_Qc_Max_OpnEnd,          // 0x2016002b, 开模位置最大值
  d_clamp1_STATE_Qc_Min_OpnEnd,          // 0x2016002c, 开模位置最小值
  d_clamp1_STATE_Qc_Avg_OpnTm,           // 0x2016002d, 开模时间平均值
  d_clamp1_STATE_Qc_Deviation_Max_OpnTm, // 0x2016002e, 开模时间最大偏差值
  d_clamp1_STATE_Qc_Unq_Cnt_OpnTm,       // 0x2016002f, 开模时间不合格数
  d_clamp1_STATE_Qc_Unq_Rate_OpnTm,      // 0x20160030, 开模时间不合格率
  d_clamp1_STATE_Qc_Max_OpnTm,           // 0x20160031, 开模时间最大值
  d_clamp1_STATE_Qc_Min_OpnTm,           // 0x20160032, 开模时间最小值
  d_clamp1_STATE_Qc_Avg_OpnSpd,          // 0x20160033, 开模速度平均值
  d_clamp1_STATE_Qc_Deviation_Max_OpnSpd, // 0x20160034, 开模速度最大偏差值
  d_clamp1_STATE_Qc_Unq_Cnt_OpnSpd,      // 0x20160035, 开模速度不合格数
  d_clamp1_STATE_Qc_Unq_Rate_OpnSpd,     // 0x20160036, 开模速度不合格率
  d_clamp1_STATE_Qc_Max_OpnSpd,          // 0x20160037, 开模速度最大值
  d_clamp1_STATE_Qc_Min_OpnSpd,          // 0x20160038, 开模速度最小值
  d_clamp1_STATE_TM_CLAMPCLS_RT,         // 0x20160039, 实时计算的关模时间
  d_clamp1_STATE_TM_CLAMPOPN_RT,         // 0x2016003a, 实时计算的开模时间
  d_clamp1_STATE_Step_Clampopn,          // 0x2016003b, 开模实时动作步
  d_clamp1_STATE_Step_Clampcls,          // 0x2016003c, 关模实时动作步
  d_clamp1_STATE_CN_PRU,                 // 0x2016003d, 剩余模数
  d_clamp1_STATE_TM_CLAMPOPN_START,      // 0x2016003e, 开模计时起始时间
  d_clamp1_STATE_TM_CLAMPCLS_START,      // 0x2016003f, 关模计时起始时间
  d_clamp1_STATE_AI_CLAMP_OPEN_RAMP,     // 0x20160040, 开模到位减速计算斜率
  d_clamp1_STATE_AD_POSIDISSREAL,        // 0x20160041, 实际减速距离
  d_clamp1_STATE_TM_CLAMPOPN1F,          // 0x20160042, 开模一段计时实际值
  d_clamp1_STATE_CN_MOLDOPNNUM0_NCL,     // 0x20160043, 开模总数（不清零）（高）
  d_clamp1_STATE_CN_MOLDOPNNUM1_NCL,     // 0x20160044, 开模总数（不清零）（低）
  d_clamp1_STATE_TM_CLAMPCLS2,           // 0x20160045, 关模实际时间2（关模完成时上传）
  d_clamp1_STATE_TM_CLAMPOPN2            // 0x20160046, 开模实际时间2（开模完成时上传）
};

enum db_eject1_PO{
  d_eject1_PO_O_EJT_FWD = 0x21100000, // 0x21100000, 托进
  d_eject1_PO_O_EJT_FSET,             // 0x21100001, 托进设定
  d_eject1_PO_O_EJT_FRSV,             // 0x21100002, 托进备用
  d_eject1_PO_O_EJT_BWD,              // 0x21100003, 托退
  d_eject1_PO_O_EJT_BSET,             // 0x21100004, 托退设定
  d_eject1_PO_O_EJT_BRSV,             // 0x21100005, 托退备用
  d_eject1_PO_O_EJT_FBWD,             // 0x21100006, 托进托退
  d_eject1_PO_O_EJT_FWD2,             // 0x21100007, 托进2
  d_eject1_PO_O_EJT_BWD2              // 0x21100008, 托退2
};

enum db_eject1_PI{
  d_eject1_PI_I_EJT_FWD = 0x21110000, // 0x21110000, 托进终
  d_eject1_PI_I_EJT_BWD,              // 0x21110001, 托退终
  d_eject1_PI_I_EJT_ACT,              // 0x21110002, 启动托模
  d_eject1_PI_I_EJT_FWS,              // 0x21110003, 托进慢速
  d_eject1_PI_I_EJT_PROT,             // 0x21110004, 托模保护
  d_eject1_PI_I_MOLDBCKEND            // 0x21110005, 模具退终
};

enum db_eject1_MACHSET{
  d_eject1_MACHSET_EJT_METERCH = 0x21120000, // 0x21120000, 托模电子尺通道
  d_eject1_MACHSET_DAPRES_EJTRAMP,           // 0x21120001, 托模启动压力斜率
  d_eject1_MACHSET_DAFLOW_EJTRAMP,           // 0x21120002, 托模启动速度斜率
  d_eject1_MACHSET_DAPRES_EJTOFFRAMP,        // 0x21120003, 托模关闭压力斜率
  d_eject1_MACHSET_DAFLOW_EJTOFFRAMP,        // 0x21120004, 托模关闭速度斜率
  d_eject1_MACHSET_DAPRES_EJTSETUP,          // 0x21120005, 托模慢速调试压力
  d_eject1_MACHSET_DAFLOW_EJTSETUP,          // 0x21120006, 托模慢速调试速度
  d_eject1_MACHSET_ADPOSI_EJTRETEFF,         // 0x21120007, 托模退位置有效区
  d_eject1_MACHSET_ADPOSI_EJTRETMAX,         // 0x21120008, 托模退位置允许最大值
  d_eject1_MACHSET_TM_EJEENDCHK,             // 0x21120009, 托模终检测时间
  d_eject1_MACHSET_EJTF_HYDELAY,             // 0x2112000a, 托进液压延迟开计时
  d_eject1_MACHSET_EJTB_HYDELAY,             // 0x2112000b, 托退液压延迟开计时
  d_eject1_MACHSET_TMDLY_VLV_ONEJTF,         // 0x2112000c, 托进阀延迟开计时
  d_eject1_MACHSET_TMDLY_VLV_OFFEJTF,        // 0x2112000d, 托进阀延迟关计时
  d_eject1_MACHSET_TMDLY_VLV_ONEJTB,         // 0x2112000e, 托退阀延迟开计时
  d_eject1_MACHSET_TMDLY_VLV_OFFEJTB,        // 0x2112000f, 托退阀延迟关计时
  d_eject1_MACHSET_TMDLY_EJT_FWDEND,         // 0x21120010, 托进动作结束延时
  d_eject1_MACHSET_TMDLY_EJT_BWDEND,         // 0x21120011, 托退动作结束延时
  d_eject1_MACHSET_DAPRES_EJTHOLD,           // 0x21120012, 托模进保持压力
  d_eject1_MACHSET_DAFLOW_EJTHOLD,           // 0x21120013, 托模进保持速度
  d_eject1_MACHSET_TM_EJTHOLD,               // 0x21120014, 托模进保持时间
  d_eject1_MACHSET_DIA_CYLINDER_E,           // 0x21120015, 托模油缸直径
  d_eject1_MACHSET_DIA_PISDIA_E,             // 0x21120016, 托模活塞杆直径
  d_eject1_MACHSET_EFFICIENCY_FACTOR_EJF,    // 0x21120017, 托进效率系数
  d_eject1_MACHSET_EFFICIENCY_FACTOR_EJB,    // 0x21120018, 托退效率系数
  d_eject1_MACHSET_DIA_LEFT_RIGHT,           // 0x21120019, 托模活塞杆
  d_eject1_MACHSET_DIA_OIL_NUM,              // 0x2112001a, 托模油缸个数
  d_eject1_MACHSET_EJEFWD_PRESS,             // 0x2112001b, 托进底压
  d_eject1_MACHSET_EJEFWD_FLOWS,             // 0x2112001c, 托进底流
  d_eject1_MACHSET_EJEBWD_PRESS,             // 0x2112001d, 托退底压
  d_eject1_MACHSET_EJEBWD_FLOWS,             // 0x2112001e, 托退底流
  d_eject1_MACHSET_SMOOTH_LEVEL_EJF,         // 0x2112001f, 托进平滑
  d_eject1_MACHSET_SMOOTH_LEVEL_EJB,         // 0x21120020, 托退平滑
  d_eject1_MACHSET_EJT_OPEN                  // 0x21120021, 托模开环
};

enum db_eject1_MOLDSET{
  d_eject1_MOLDSET_TM_DLYEJTADV = 0x21130000, // 0x21130000, 托模进前延迟
  d_eject1_MOLDSET_TM_DLYEJTRET,              // 0x21130001, 托模退前延迟
  d_eject1_MOLDSET_FL_EJTCTRL,                // 0x21130002, 托模方式
  d_eject1_MOLDSET_CN_EJT,                    // 0x21130003, 托模次数
  d_eject1_MOLDSET_FL_EJTVIBRATEMODE,         // 0x21130004, 震动托模方式
  d_eject1_MOLDSET_TM_EJTVIBRATE,             // 0x21130005, 震动托模计时
  d_eject1_MOLDSET_ADPOSI_EJTVIBRATEDISTANCE, // 0x21130006, 震动托模距离
  d_eject1_MOLDSET_DAPRES_EJTVIBRATE,         // 0x21130007, 震动托模压力
  d_eject1_MOLDSET_FL_EJTSPECIAL,             // 0x21130008, 半托时托进保持
  d_eject1_MOLDSET_TM_EJTDEFAULTMAX,          // 0x21130009, 托模计时警报上限
  d_eject1_MOLDSET_FL_EJTFIRSTSET,            // 0x2113000a, 托模首次独立行程功能
  d_eject1_MOLDSET_FL_CTRLMODE,               // 0x2113000b, 托模控制方式
  d_eject1_MOLDSET_Qc_DeviationSet_EjtTm,     // 0x2113000c, 顶针时间允许偏差
  d_eject1_MOLDSET_Qc_TmSet_EjtTm,            // 0x2113000d, 顶针时间设定值
  d_eject1_MOLDSET_Qc_Mode_EjtTm,             // 0x2113000e, 顶针时间监控
  d_eject1_MOLDSET_FL_ejectCnt,               // 0x2113000f, 手动托进次数
  d_eject1_MOLDSET_FL_EjtAdvDlyMd,            // 0x21130010, 托模进延迟方式
  d_eject1_MOLDSET_FL_EjtRetDlyMd,            // 0x21130011, 托模退延迟方式
  d_eject1_MOLDSET_TM_EJETFWD_ALARM,          // 0x21130012, 托模进计时警报上限
  d_eject1_MOLDSET_TM_EJETBWD_ALARM,          // 0x21130013, 托模退计时警报上限
  d_eject1_MOLDSET_FL_EJETFWD_DOWN,           // 0x21130014, 托进缓冲
  d_eject1_MOLDSET_FL_EJETBWD_DOWN,           // 0x21130015, 托退缓冲
  d_eject1_MOLDSET_MODULUS_EJETFWD,           // 0x21130016, 托进缓冲系数
  d_eject1_MOLDSET_MODULUS_EJETBWD,           // 0x21130017, 托退缓冲系数
  d_eject1_MOLDSET_EJECTBUFFUSED,             // 0x21130018, 托模缓冲
  d_eject1_MOLDSET_FL_INJEJECT,               // 0x21130019, 进料口切断
  d_eject1_MOLDSET_TM_INJEJECTDLY,            // 0x2113001a, 切料托模进延时
  d_eject1_MOLDSET_TM_INJEJECT,               // 0x2113001b, 切料托模进保持时间
  d_eject1_MOLDSET_FL_INJEJECTBWD,            // 0x2113001c, 切料后托模退
  d_eject1_MOLDSET_EJEF_MAX_SPD,              // 0x2113001d, 托进最大速度斜率
  d_eject1_MOLDSET_EJEF_MAX_PRES,             // 0x2113001e, 托进最大压力斜率
  d_eject1_MOLDSET_EJEB_MAX_SPD,              // 0x2113001f, 托退最大速度斜率
  d_eject1_MOLDSET_EJEB_MAX_PRES,             // 0x21130020, 托退最大压力斜率
  d_eject1_MOLDSET_FL_EJEF_SMOOTH,            // 0x21130021, 托进平稳
  d_eject1_MOLDSET_FL_EJEB_SMOOTH,            // 0x21130022, 托退平稳
  d_eject1_MOLDSET_PUMP_K_EJEFWD,             // 0x21130023, 托进减速调节系数
  d_eject1_MOLDSET_PUMP_DEAD_EJEFWD,          // 0x21130024, 托进死区时间
  d_eject1_MOLDSET_PUMP_K_EJEBWD,             // 0x21130025, 托退减速调节系数
  d_eject1_MOLDSET_PUMP_DEAD_EJEBWD,          // 0x21130026, 托退死区时间
  d_eject1_MOLDSET_EJTF_SPD_STARTON,          // 0x21130027, 托进启动速度斜率
  d_eject1_MOLDSET_EJTF_SPD_HALFWAY,          // 0x21130028, 托进中间速度斜率
  d_eject1_MOLDSET_EJTF_SPD_STOPOFF,          // 0x21130029, 托进终止速度斜率
  d_eject1_MOLDSET_EJTF_PRES_STARTON,         // 0x2113002a, 托进启动压力斜率
  d_eject1_MOLDSET_EJTF_PRES_HALFWAY,         // 0x2113002b, 托进中间压力斜率
  d_eject1_MOLDSET_EJTF_PRES_STOPOFF,         // 0x2113002c, 托进终止压力斜率
  d_eject1_MOLDSET_EJTB_SPD_STARTON,          // 0x2113002d, 托退启动速度斜率
  d_eject1_MOLDSET_EJTB_SPD_HALFWAY,          // 0x2113002e, 托退中间速度斜率
  d_eject1_MOLDSET_EJTB_SPD_STOPOFF,          // 0x2113002f, 托退终止速度斜率
  d_eject1_MOLDSET_EJTB_PRES_STARTON,         // 0x21130030, 托退启动压力斜率
  d_eject1_MOLDSET_EJTB_PRES_HALFWAY,         // 0x21130031, 托退中间压力斜率
  d_eject1_MOLDSET_EJTB_PRES_STOPOFF          // 0x21130032, 托退终止压力斜率
};

enum db_eject1_ACTPARA{
  d_eject1_ACTPARA_DAPRES_EJTADVSTARTFIRST = 0x21140000, // 0x21140000, 托进一段压力
  d_eject1_ACTPARA_DAPRES_EJTADV,                        // 0x21140001, 托进二段压力
  d_eject1_ACTPARA_DAPRES_EJTRETFIRST,                   // 0x21140002, 托退一段压力
  d_eject1_ACTPARA_DAPRES_EJTRET,                        // 0x21140003, 托退二段压力
  d_eject1_ACTPARA_DAFLOW_EJTADVSTARTFIRST,              // 0x21140004, 托进一段速度
  d_eject1_ACTPARA_DAFLOW_EJTADV,                        // 0x21140005, 托进二段速度
  d_eject1_ACTPARA_DAFLOW_EJTRETFIRST,                   // 0x21140006, 托退一段速度
  d_eject1_ACTPARA_DAFLOW_EJTRET,                        // 0x21140007, 托退二段速度
  d_eject1_ACTPARA_ADPOSI_EJTADVSTARTFIRSTEND,           // 0x21140008, 托进一段终止位置
  d_eject1_ACTPARA_ADPOSI_EJTADVFASTEND,                 // 0x21140009, 托进二段终止位置
  d_eject1_ACTPARA_ADPOSI_EJTRETFIRST,                   // 0x2114000a, 托退一段终止位置
  d_eject1_ACTPARA_ADPOSI_EJTRETEND,                     // 0x2114000b, 托退二段终止位置
  d_eject1_ACTPARA_TM_EJT1STROUND,                       // 0x2114000c, 托模进计时
  d_eject1_ACTPARA_TM_EJT2NDROUND,                       // 0x2114000d, 托模退计时
  d_eject1_ACTPARA_ADPOSI_EJTFFIRSTEND1,                 // 0x2114000e, 托模进一段首次位置
  d_eject1_ACTPARA_ADPOSI_EJTFFIRSTEND2,                 // 0x2114000f, 托模进二段首次位置
  d_eject1_ACTPARA_ADPOSI_EJTBFIRSTEND1,                 // 0x21140010, 托模退一段首次位置
  d_eject1_ACTPARA_ADPOSI_EJTBFIRSTEND2,                 // 0x21140011, 托模退二段首次位置
  d_eject1_ACTPARA_DAPRES_INJEJT,                        // 0x21140012, 切料压力
  d_eject1_ACTPARA_DAFLOW_INJEJT,                        // 0x21140013, 切料速度
  d_eject1_ACTPARA_ADPOSI_INJEJT,                        // 0x21140014, 切料托进位置
  d_eject1_ACTPARA_PER_DAFLOW_EJECT,                     // 0x21140015, 托模速度使用率
  d_eject1_ACTPARA_DAPRES_INJEJTBWD,                     // 0x21140016, 切断退压力
  d_eject1_ACTPARA_DAFLOW_INJEJTBWD                      // 0x21140017, 切断退速度
};

enum db_eject1_HYDR{
  d_eject1_HYDR_HYDR_PORT_EJECT = 0x21150000, // 0x21150000, 
  d_eject1_HYDR_HYDR_PUMPS_EJECTSETUP,        // 0x21150001, 
  d_eject1_HYDR_HYDR_PUMPS_EJECT,             // 0x21150002, 
  d_eject1_HYDR_SERVO_KP_EJECT,               // 0x21150003, 托模 P
  d_eject1_HYDR_SERVO_KI_EJECT,               // 0x21150004, 托模 I
  d_eject1_HYDR_SERVO_KD_EJECT                // 0x21150005, 托模 D
};

enum db_eject1_STATE{
  d_eject1_STATE_FL_ERROR0 = 0x21160000, // 0x21160000, 警报1
  d_eject1_STATE_FL_ERROR1,              // 0x21160001, 警报2
  d_eject1_STATE_TM_EJECT,               // 0x21160002, 托模计时实际值
  d_eject1_STATE_TM_EJTCTFWD,            // 0x21160003, 托模进计时
  d_eject1_STATE_TM_EJTCTBWD,            // 0x21160004, 托模退计时
  d_eject1_STATE_Qc_Avg_EjtTm,           // 0x21160005, 顶针时间平均值
  d_eject1_STATE_Qc_Deviation_Max_EjtTm, // 0x21160006, 顶针时间最大偏差值
  d_eject1_STATE_Qc_Unq_Cnt_EjtTm,       // 0x21160007, 顶针时间不合格数
  d_eject1_STATE_Qc_Unq_Rate_EjtTm,      // 0x21160008, 顶针时间不合格率
  d_eject1_STATE_Qc_Max_EjtTm,           // 0x21160009, 顶针时间最大值
  d_eject1_STATE_Qc_Min_EjtTm,           // 0x2116000a, 顶针时间最小值
  d_eject1_STATE_Step_EjectFwd,          // 0x2116000b, 托模进实时动作步
  d_eject1_STATE_Step_EjectBwd,          // 0x2116000c, 托模退实时动作步
  d_eject1_STATE_Step_EjectPro,          // 0x2116000d, 托模调度实时动作步
  d_eject1_STATE_EJEFWD_SPEED,           // 0x2116000e, 托模进速度
  d_eject1_STATE_EJEBWD_SPEED,           // 0x2116000f, 托模退速度
  d_eject1_STATE_TM_EJECT_START,         // 0x21160010, 托模计时起始时间
  d_eject1_STATE_TM_EJECT_RT             // 0x21160011, 托模计时实时值用于面板显示
};

enum db_eject2_PO{
  d_eject2_PO_O_EJT_FWD = 0x21200000, // 0x21200000, 托2进
  d_eject2_PO_O_EJT_FSET,             // 0x21200001, 托2进设定
  d_eject2_PO_O_EJT_FRSV,             // 0x21200002, 托2进备用
  d_eject2_PO_O_EJT_BWD,              // 0x21200003, 托2退
  d_eject2_PO_O_EJT_BSET,             // 0x21200004, 托2退设定
  d_eject2_PO_O_EJT_BRSV,             // 0x21200005, 托2退备用
  d_eject2_PO_O_EJT_FBWD,             // 0x21200006, 托2进托退
  d_eject2_PO_O_EJT_FWD2,             // 0x21200007, 托2进2
  d_eject2_PO_O_EJT_BWD2              // 0x21200008, 托2退2
};

enum db_eject2_PI{
  d_eject2_PI_I_EJT_FWD = 0x21210000, // 0x21210000, 托2进终
  d_eject2_PI_I_EJT_BWD,              // 0x21210001, 托2退终
  d_eject2_PI_I_EJT_ACT,              // 0x21210002, 启动托模2
  d_eject2_PI_I_EJT_FWS,              // 0x21210003, 托2进慢速
  d_eject2_PI_I_EJT_PROT,             // 0x21210004, 托模2保护
  d_eject2_PI_I_MOLDBCKEND            // 0x21210005, 模具退终
};

enum db_eject2_MACHSET{
  d_eject2_MACHSET_EJT_METERCH = 0x21220000, // 0x21220000, 托模2电子尺通道
  d_eject2_MACHSET_DAPRES_EJTRAMP,           // 0x21220001, 托模2启动压力斜率
  d_eject2_MACHSET_DAFLOW_EJTRAMP,           // 0x21220002, 托模2启动速度斜率
  d_eject2_MACHSET_DAPRES_EJTOFFRAMP,        // 0x21220003, 托模2关闭压力斜率
  d_eject2_MACHSET_DAFLOW_EJTOFFRAMP,        // 0x21220004, 托模2关闭速度斜率
  d_eject2_MACHSET_DAPRES_EJTSETUP,          // 0x21220005, 托模2慢速调试压力
  d_eject2_MACHSET_DAFLOW_EJTSETUP,          // 0x21220006, 托模2慢速调试速度
  d_eject2_MACHSET_ADPOSI_EJTRETEFF,         // 0x21220007, 托模2退位置有效区
  d_eject2_MACHSET_ADPOSI_EJTRETMAX,         // 0x21220008, 托模2退位置允许最大值
  d_eject2_MACHSET_TM_EJEENDCHK,             // 0x21220009, 托模2终检测时间
  d_eject2_MACHSET_EJTF_HYDELAY,             // 0x2122000a, 托2进液压延时开计时
  d_eject2_MACHSET_EJTB_HYDELAY,             // 0x2122000b, 托2退液压延时开计时
  d_eject2_MACHSET_TMDLY_VLV_ONEJTF,         // 0x2122000c, 托2进阀延迟开计时
  d_eject2_MACHSET_TMDLY_VLV_OFFEJTF,        // 0x2122000d, 托2进阀延迟关计时
  d_eject2_MACHSET_TMDLY_VLV_ONEJTB,         // 0x2122000e, 托2退阀延迟开计时
  d_eject2_MACHSET_TMDLY_VLV_OFFEJTB,        // 0x2122000f, 托2退阀延迟关计时
  d_eject2_MACHSET_TMDLY_EJT_FWDEND,         // 0x21220010, 托模2进动作结束延时
  d_eject2_MACHSET_TMDLY_EJT_BWDEND,         // 0x21220011, 托模2退动作结束延时
  d_eject2_MACHSET_DAPRES_EJTHOLD,           // 0x21220012, 托模2进保持压力
  d_eject2_MACHSET_DAFLOW_EJTHOLD,           // 0x21220013, 托模2进保持速度
  d_eject2_MACHSET_TM_EJTHOLD,               // 0x21220014, 托模2进保持时间
  d_eject2_MACHSET_DIA_CYLINDER_E,           // 0x21220015, 托模2油缸直径
  d_eject2_MACHSET_DIA_PISDIA_E,             // 0x21220016, 托模2柱塞杆直径
  d_eject2_MACHSET_EFFICIENCY_FACTOR_EJF,    // 0x21220017, 托2进效率系数
  d_eject2_MACHSET_EFFICIENCY_FACTOR_EJB,    // 0x21220018, 托2退效率系数
  d_eject2_MACHSET_DIA_LEFT_RIGHT,           // 0x21220019, 托模2活塞杆
  d_eject2_MACHSET_DIA_OIL_NUM,              // 0x2122001a, 托模2油缸个数
  d_eject2_MACHSET_EJEFWD_PRESS,             // 0x2122001b, 托2进最小压力范围
  d_eject2_MACHSET_EJEFWD_FLOWS,             // 0x2122001c, 托2进最小速度范围
  d_eject2_MACHSET_EJEBWD_PRESS,             // 0x2122001d, 托2退最小压力范围
  d_eject2_MACHSET_EJEBWD_FLOWS,             // 0x2122001e, 托2退最小速度范围
  d_eject2_MACHSET_SMOOTH_LEVEL_EJF,         // 0x2122001f, 托2进平滑
  d_eject2_MACHSET_SMOOTH_LEVEL_EJB,         // 0x21220020, 托2退平滑
  d_eject2_MACHSET_EJT_OPEN                  // 0x21220021, 托模2开环
};

enum db_eject2_MOLDSET{
  d_eject2_MOLDSET_TM_DLYEJTADV = 0x21230000, // 0x21230000, 托模2进延迟计时
  d_eject2_MOLDSET_TM_DLYEJTRET,              // 0x21230001, 托模2退延迟计时
  d_eject2_MOLDSET_FL_EJTCTRL,                // 0x21230002, 托模2方式
  d_eject2_MOLDSET_CN_EJT,                    // 0x21230003, 托模2次数
  d_eject2_MOLDSET_FL_EJTVIBRATEMODE,         // 0x21230004, 震动托模方式
  d_eject2_MOLDSET_TM_EJTVIBRATE,             // 0x21230005, 震动托模计时
  d_eject2_MOLDSET_ADPOSI_EJTVIBRATEDISTANCE, // 0x21230006, 震动托模距离
  d_eject2_MOLDSET_DAPRES_EJTVIBRATE,         // 0x21230007, 震动托模压力
  d_eject2_MOLDSET_FL_EJTSPECIAL,             // 0x21230008, 半托时托2进保持
  d_eject2_MOLDSET_TM_EJTDEFAULTMAX,          // 0x21230009, 托模2计时警报上限
  d_eject2_MOLDSET_FL_EJTFIRSTSET,            // 0x2123000a, 托模2首次独立行程功能
  d_eject2_MOLDSET_FL_CTRLMODE,               // 0x2123000b, 托模2控制方式
  d_eject2_MOLDSET_Qc_DeviationSet_EjtTm,     // 0x2123000c, 顶针2时间允许偏差
  d_eject2_MOLDSET_Qc_TmSet_EjtTm,            // 0x2123000d, 顶针2时间设定值
  d_eject2_MOLDSET_Qc_Mode_EjtTm,             // 0x2123000e, 顶针2时间监控
  d_eject2_MOLDSET_FL_ejectCnt,               // 0x2123000f, 手动托2进次数
  d_eject2_MOLDSET_FL_EjtAdvDlyMd,            // 0x21230010, 托模2进延迟方式
  d_eject2_MOLDSET_FL_EjtRetDlyMd,            // 0x21230011, 托模2退延迟方式
  d_eject2_MOLDSET_TM_EJETFWD_ALARM,          // 0x21230012, 托模2进计时警报上限
  d_eject2_MOLDSET_TM_EJETBWD_ALARM,          // 0x21230013, 托模2退计时警报上限
  d_eject2_MOLDSET_FL_EJETFWD_DOWN,           // 0x21230014, 托2进缓冲
  d_eject2_MOLDSET_FL_EJETBWD_DOWN,           // 0x21230015, 托2退缓冲
  d_eject2_MOLDSET_MODULUS_EJETFWD,           // 0x21230016, 托2进缓冲系数
  d_eject2_MOLDSET_MODULUS_EJETBWD,           // 0x21230017, 托2退缓冲系数
  d_eject2_MOLDSET_EJECTBUFFUSED,             // 0x21230018, 托模2缓冲
  d_eject2_MOLDSET_FL_INJEJECT,               // 0x21230019, 进料口切断
  d_eject2_MOLDSET_TM_INJEJECTDLY,            // 0x2123001a, 切断托模2进延时
  d_eject2_MOLDSET_TM_INJEJECT,               // 0x2123001b, 切断托模2进保持计时
  d_eject2_MOLDSET_FL_INJEJECTBWD,            // 0x2123001c, 切料后托模退
  d_eject2_MOLDSET_EJEF_MAX_SPD,              // 0x2123001d, 托模进最大斜率(速度)
  d_eject2_MOLDSET_EJEF_MAX_PRES,             // 0x2123001e, 托模进最大斜率(压力)
  d_eject2_MOLDSET_EJEB_MAX_SPD,              // 0x2123001f, 托模退最大斜率(速度)
  d_eject2_MOLDSET_EJEB_MAX_PRES,             // 0x21230020, 托模退最大斜率(压力)
  d_eject2_MOLDSET_FL_EJEF_SMOOTH,            // 0x21230021, 托进平稳
  d_eject2_MOLDSET_FL_EJEB_SMOOTH,            // 0x21230022, 托退平稳
  d_eject2_MOLDSET_PUMP_K_EJEFWD,             // 0x21230023, 托模进缓冲系数
  d_eject2_MOLDSET_PUMP_DEAD_EJEFWD,          // 0x21230024, 托模进死区时间
  d_eject2_MOLDSET_PUMP_K_EJEBWD,             // 0x21230025, 托模退缓冲系数
  d_eject2_MOLDSET_PUMP_DEAD_EJEBWD,          // 0x21230026, 托模退死区时间
  d_eject2_MOLDSET_EJTF_SPD_STARTON,          // 0x21230027, 托进开始速度斜率
  d_eject2_MOLDSET_EJTF_SPD_HALFWAY,          // 0x21230028, 托进中间速度斜率
  d_eject2_MOLDSET_EJTF_SPD_STOPOFF,          // 0x21230029, 托进停止速度斜率
  d_eject2_MOLDSET_EJTF_PRES_STARTON,         // 0x2123002a, 托进开始压力斜率
  d_eject2_MOLDSET_EJTF_PRES_HALFWAY,         // 0x2123002b, 托进中间压力斜率
  d_eject2_MOLDSET_EJTF_PRES_STOPOFF,         // 0x2123002c, 托进停止压力斜率
  d_eject2_MOLDSET_EJTB_SPD_STARTON,          // 0x2123002d, 托退开始速度斜率
  d_eject2_MOLDSET_EJTB_SPD_HALFWAY,          // 0x2123002e, 托退中间速度斜率
  d_eject2_MOLDSET_EJTB_SPD_STOPOFF,          // 0x2123002f, 托退停止速度斜率
  d_eject2_MOLDSET_EJTB_PRES_STARTON,         // 0x21230030, 托退开始压力斜率
  d_eject2_MOLDSET_EJTB_PRES_HALFWAY,         // 0x21230031, 托退中间压力斜率
  d_eject2_MOLDSET_EJTB_PRES_STOPOFF          // 0x21230032, 托退停止压力斜率
};

enum db_eject2_ACTPARA{
  d_eject2_ACTPARA_DAPRES_EJTADVSTARTFIRST = 0x21240000, // 0x21240000, 托2进一段压力
  d_eject2_ACTPARA_DAPRES_EJTADV,                        // 0x21240001, 托2进二段压力
  d_eject2_ACTPARA_DAPRES_EJTRETFIRST,                   // 0x21240002, 托2退一段压力
  d_eject2_ACTPARA_DAPRES_EJTRET,                        // 0x21240003, 托2退二段压力
  d_eject2_ACTPARA_DAFLOW_EJTADVSTARTFIRST,              // 0x21240004, 托2进一段速度
  d_eject2_ACTPARA_DAFLOW_EJTADV,                        // 0x21240005, 托2进二段速度
  d_eject2_ACTPARA_DAFLOW_EJTRETFIRST,                   // 0x21240006, 托2退一段速度
  d_eject2_ACTPARA_DAFLOW_EJTRET,                        // 0x21240007, 托2退二段速度
  d_eject2_ACTPARA_ADPOSI_EJTADVSTARTFIRSTEND,           // 0x21240008, 托2进一段终止位置
  d_eject2_ACTPARA_ADPOSI_EJTADVFASTEND,                 // 0x21240009, 托2进二段终止位置
  d_eject2_ACTPARA_ADPOSI_EJTRETFIRST,                   // 0x2124000a, 托2退一段终止位置
  d_eject2_ACTPARA_ADPOSI_EJTRETEND,                     // 0x2124000b, 托2退二段终止位置
  d_eject2_ACTPARA_TM_EJT1STROUND,                       // 0x2124000c, 托模2进计时
  d_eject2_ACTPARA_TM_EJT2NDROUND,                       // 0x2124000d, 托模2退计时
  d_eject2_ACTPARA_ADPOSI_EJTFFIRSTEND1,                 // 0x2124000e, 托模2进一段首次终止位置
  d_eject2_ACTPARA_ADPOSI_EJTFFIRSTEND2,                 // 0x2124000f, 托模2进二段首次终止位置
  d_eject2_ACTPARA_ADPOSI_EJTBFIRSTEND1,                 // 0x21240010, 托模2退一段首次终止位置
  d_eject2_ACTPARA_ADPOSI_EJTBFIRSTEND2,                 // 0x21240011, 托模2退二段首次终止位置
  d_eject2_ACTPARA_DAPRES_INJEJT,                        // 0x21240012, 切断2压力
  d_eject2_ACTPARA_DAFLOW_INJEJT,                        // 0x21240013, 切断2速度
  d_eject2_ACTPARA_ADPOSI_INJEJT,                        // 0x21240014, 切断2托进位置
  d_eject2_ACTPARA_PER_DAFLOW_EJECT                      // 0x21240015, 托模2速度百分比
};

enum db_eject2_HYDR{
  d_eject2_HYDR_HYDR_PORT_EJECT = 0x21250000, // 0x21250000, 
  d_eject2_HYDR_HYDR_PUMPS_EJECTSETUP,        // 0x21250001, 
  d_eject2_HYDR_HYDR_PUMPS_EJECT,             // 0x21250002, 
  d_eject2_HYDR_SERVO_KP_EJECT,               // 0x21250003, 托模2 P
  d_eject2_HYDR_SERVO_KI_EJECT,               // 0x21250004, 托模2 I
  d_eject2_HYDR_SERVO_KD_EJECT                // 0x21250005, 托模2 D
};

enum db_eject2_STATE{
  d_eject2_STATE_FL_ERROR0 = 0x21260000, // 0x21260000, 警报1
  d_eject2_STATE_FL_ERROR1,              // 0x21260001, 警报2
  d_eject2_STATE_TM_EJECT,               // 0x21260002, 托模计时实际值
  d_eject2_STATE_TM_EJTCTFWD,            // 0x21260003, 托模进计时
  d_eject2_STATE_TM_EJTCTBWD,            // 0x21260004, 托模退计时
  d_eject2_STATE_Qc_Avg_EjtTm,           // 0x21260005, 顶针时间平均值
  d_eject2_STATE_Qc_Deviation_Max_EjtTm, // 0x21260006, 顶针时间最大偏差值
  d_eject2_STATE_Qc_Unq_Cnt_EjtTm,       // 0x21260007, 顶针时间不合格数
  d_eject2_STATE_Qc_Unq_Rate_EjtTm,      // 0x21260008, 顶针时间不合格率
  d_eject2_STATE_Qc_Max_EjtTm,           // 0x21260009, 顶针时间最大值
  d_eject2_STATE_Qc_Min_EjtTm,           // 0x2126000a, 顶针时间最小值
  d_eject2_STATE_Step_EjectFwd,          // 0x2126000b, 托模进实时动作步
  d_eject2_STATE_Step_EjectBwd,          // 0x2126000c, 托模退实时动作步
  d_eject2_STATE_Step_EjectPro,          // 0x2126000d, 托模调度实时动作步
  d_eject2_STATE_EJEFWD_SPEED,           // 0x2126000e, 托模进速度
  d_eject2_STATE_EJEBWD_SPEED,           // 0x2126000f, 托模退速度
  d_eject2_STATE_TM_EJECT_START,         // 0x21260010, 托模计时起始时间
  d_eject2_STATE_TM_EJECT_RT             // 0x21260011, 托模计时实时值用于面板显示
};

enum db_coreA_PO{
  d_coreA_PO_O_CP_FWD = 0x22100000, // 0x22100000, 中子A进
  d_coreA_PO_O_CP_FSET,             // 0x22100001, 中子A进装模
  d_coreA_PO_O_CP_FRSV,             // 0x22100002, 中子A进备用
  d_coreA_PO_O_CP_BWD,              // 0x22100003, 中子A退
  d_coreA_PO_O_CP_BSET,             // 0x22100004, 中子A退装模
  d_coreA_PO_O_CP_BRSV              // 0x22100005, 中子A退备用
};

enum db_coreA_PI{
  d_coreA_PI_I_CORE_STA = 0x22110000, // 0x22110000, 中子A启动
  d_coreA_PI_I_CP_FWD,                // 0x22110001, 中子A进终
  d_coreA_PI_I_CP_BWD,                // 0x22110002, 中子A退终
  d_coreA_PI_I_CP_ACT,                // 0x22110003, 中子A
  d_coreA_PI_I_SC_CHK,                // 0x22110004, 绞牙A电眼
  d_coreA_PI_I_CP_FWD2                // 0x22110005, 中子A进终2
};

enum db_coreA_MACHSET{
  d_coreA_MACHSET_DAPRES_CORERAMP = 0x22120000, // 0x22120000, 中子A启动压力斜率
  d_coreA_MACHSET_DAFLOW_CORERAMP,              // 0x22120001, 中子A启动速度斜率
  d_coreA_MACHSET_DAPRES_COREOFFRAMP,           // 0x22120002, 中子A关闭压力斜率
  d_coreA_MACHSET_DAFLOW_COREOFFRAMP,           // 0x22120003, 中子A关闭速度斜率
  d_coreA_MACHSET_DAPRES_CORESETUP,             // 0x22120004, 中子A慢速调试压力
  d_coreA_MACHSET_DAFLOW_CORESETUP,             // 0x22120005, 中子A慢速调试速度
  d_coreA_MACHSET_TM_COREINCTDLY,               // 0x22120006, 中子A进延迟计时
  d_coreA_MACHSET_TM_COREOUTCTDLY,              // 0x22120007, 中子A退延迟计时
  d_coreA_MACHSET_TM_COREENDDLY,                // 0x22120008, 中子A动作结束延时
  d_coreA_MACHSET_ADPOSI_EFFCPR2,               // 0x22120009, 途中中子A退下限位置
  d_coreA_MACHSET_TM_MIDDLECOREDLY,             // 0x2212000a, 途中中子A动作延迟
  d_coreA_MACHSET_TM_CPPUMP_DLY,                // 0x2212000b, 特殊中子A阀迟关计时
  d_coreA_MACHSET_COREF_HYDELAY,                // 0x2212000c, 中子A进液压延时
  d_coreA_MACHSET_COREB_HYDELAY,                // 0x2212000d, 中子A退液压延时
  d_coreA_MACHSET_TMDLY_VLV_ONCOREF,            // 0x2212000e, 中子A进阀延迟开计时
  d_coreA_MACHSET_TMDLY_VLV_OFFCOREF,           // 0x2212000f, 中子A进阀延迟关计时
  d_coreA_MACHSET_TMDLY_VLV_ONCOREB,            // 0x22120010, 中子A退阀延迟开计时
  d_coreA_MACHSET_TMDLY_VLV_OFFCOREB,           // 0x22120011, 中子A退阀延迟关计时
  d_coreA_MACHSET_TM_COREINENDDLY,              // 0x22120012, 中子A进动作结束延时
  d_coreA_MACHSET_TM_COREOUTENDDLY,             // 0x22120013, 中子A退动作结束延时
  d_coreA_MACHSET_ADPOSI_EFFCOREINPLUS,         // 0x22120014, 中子A进位置有效区(+)
  d_coreA_MACHSET_ADPOSI_EFFCOREINMINUS,        // 0x22120015, 中子A进位置有效区(-)
  d_coreA_MACHSET_ADPOSI_EFFCOREOUTPLUS,        // 0x22120016, 中子A退位置有效区(+)
  d_coreA_MACHSET_ADPOSI_EFFCOREOUTMINUS        // 0x22120017, 中子A退位置有效区(-)
};

enum db_coreA_MOLDSET{
  d_coreA_MOLDSET_FL_COREPROTECT = 0x22130000, // 0x22130000, 中子A保护功能
  d_coreA_MOLDSET_FL_CORE_CTRL,                // 0x22130001, 中子A功能
  d_coreA_MOLDSET_FL_CORE_TRAVEL,              // 0x22130002, 中子A进控制方式
  d_coreA_MOLDSET_FL_CORE_OUTTRAVEL,           // 0x22130003, 中子A退控制方式
  d_coreA_MOLDSET_FL_SPECIALCODE,              // 0x22130004, 特殊中子A功能
  d_coreA_MOLDSET_FL_INJ_FWDOFF,               // 0x22130005, 射出时中子A进保持
  d_coreA_MOLDSET_ADPOSI_CORE_FWD,             // 0x22130006, 中子A进动作位置
  d_coreA_MOLDSET_ADPOSI_CORE_BWD,             // 0x22130007, 中子A退动作位置
  d_coreA_MOLDSET_PRI_CORE_FWD,                // 0x22130008, 中子A进优先级
  d_coreA_MOLDSET_PRI_CORE_BWD,                // 0x22130009, 中子A退优先级
  d_coreA_MOLDSET_CORE_LIMIT_CHECK,            // 0x2213000a, 进退限位器检查
  d_coreA_MOLDSET_CLAMP_CORE_CHECK,            // 0x2213000b, 开关模检测中子
  d_coreA_MOLDSET_FL_HOLD_CORE_FWD,            // 0x2213000c, 中子A进保持计时
  d_coreA_MOLDSET_FL_HOLD_CORE_BWD             // 0x2213000d, 中子A退保持计时
};

enum db_coreA_ACTPARA{
  d_coreA_ACTPARA_DAPRES_COREIN = 0x22140000, // 0x22140000, 中子A进压力
  d_coreA_ACTPARA_DAPRES_COREOUT,             // 0x22140001, 中子A退压力
  d_coreA_ACTPARA_DAFLOW_COREIN,              // 0x22140002, 中子A进速度
  d_coreA_ACTPARA_DAFLOW_COREOUT,             // 0x22140003, 中子A退速度
  d_coreA_ACTPARA_TM_COREIN,                  // 0x22140004, 中子A进动作时间
  d_coreA_ACTPARA_TM_COREOUT,                 // 0x22140005, 中子A退动作时间
  d_coreA_ACTPARA_CN_SCREWIN,                 // 0x22140006, 绞牙A进绞牙计数
  d_coreA_ACTPARA_CN_SCREWOUT,                // 0x22140007, 绞牙A退绞牙计数
  d_coreA_ACTPARA_ADPOSI_COREINACT,           // 0x22140008, 中子A进动作位置
  d_coreA_ACTPARA_ADPOSI_COREOUTACT,          // 0x22140009, 中子A退动作位置
  d_coreA_ACTPARA_DAPRES_CORESLOW,            // 0x2214000a, 中子A慢速压力
  d_coreA_ACTPARA_DAFLOW_CORESLOW,            // 0x2214000b, 中子A慢速速度
  d_coreA_ACTPARA_PER_DAFLOW_CORE,            // 0x2214000c, 中子速度使用率
  d_coreA_ACTPARA_DAFLOW_CLSSTARTRAMP_CORE,   // 0x2214000d, 中子A后关模启动斜率
  d_coreA_ACTPARA_DAFLOW_CLSENDRAMP_CORE,     // 0x2214000e, 中子A前关模停止斜率
  d_coreA_ACTPARA_DAFLOW_OPNSTARTRAMP_CORE,   // 0x2214000f, 中子A后开模启动斜率
  d_coreA_ACTPARA_DAFLOW_OPNENDRAMP_CORE      // 0x22140010, 中子A前开模停止斜率
};

enum db_coreA_HYDR{
  d_coreA_HYDR_HYDR_PORT_CORE = 0x22150000, // 0x22150000, 
  d_coreA_HYDR_HYDR_PUMPS_CORE,             // 0x22150001, 
  d_coreA_HYDR_SERVO_KP_CORE,               // 0x22150002, 中子A P
  d_coreA_HYDR_SERVO_KI_CORE,               // 0x22150003, 中子A I
  d_coreA_HYDR_SERVO_KD_CORE                // 0x22150004, 中子A D
};

enum db_coreA_STATE{
  d_coreA_STATE_FL_ERROR0 = 0x22160000, // 0x22160000, 警报1
  d_coreA_STATE_FL_ERROR1,              // 0x22160001, 警报2
  d_coreA_STATE_Step_CoreFwd,           // 0x22160002, 
  d_coreA_STATE_Step_CoreBwd,           // 0x22160003, 
  d_coreA_STATE_COREFWD_STATUS,         // 0x22160004, 中子A进终信号
  d_coreA_STATE_COREBWD_STATUS          // 0x22160005, 中子A退终信号
};

enum db_coreB_PO{
  d_coreB_PO_O_CP_FWD = 0x22200000, // 0x22200000, 中子B进
  d_coreB_PO_O_CP_FSET,             // 0x22200001, 中子B进装模
  d_coreB_PO_O_CP_FRSV,             // 0x22200002, 中子B进备用
  d_coreB_PO_O_CP_BWD,              // 0x22200003, 中子B退
  d_coreB_PO_O_CP_BSET,             // 0x22200004, 中子B退装模
  d_coreB_PO_O_CP_BRSV              // 0x22200005, 中子B退备用
};

enum db_coreB_PI{
  d_coreB_PI_I_CORE_STA = 0x22210000, // 0x22210000, 中子B启动
  d_coreB_PI_I_CP_FWD,                // 0x22210001, 中子B进终
  d_coreB_PI_I_CP_BWD,                // 0x22210002, 中子B退终
  d_coreB_PI_I_CP_ACT,                // 0x22210003, 中子B
  d_coreB_PI_I_SC_CHK,                // 0x22210004, 绞牙B电眼
  d_coreB_PI_I_CP_FWD2                // 0x22210005, 中子B进终2
};

enum db_coreB_MACHSET{
  d_coreB_MACHSET_DAPRES_CORERAMP = 0x22220000, // 0x22220000, 中子B启动压力斜率
  d_coreB_MACHSET_DAFLOW_CORERAMP,              // 0x22220001, 中子B启动速度斜率
  d_coreB_MACHSET_DAPRES_COREOFFRAMP,           // 0x22220002, 中子B关闭压力斜率
  d_coreB_MACHSET_DAFLOW_COREOFFRAMP,           // 0x22220003, 中子B关闭速度斜率
  d_coreB_MACHSET_DAPRES_CORESETUP,             // 0x22220004, 中子B慢速调试压力
  d_coreB_MACHSET_DAFLOW_CORESETUP,             // 0x22220005, 中子B慢速调试速度
  d_coreB_MACHSET_TM_COREINCTDLY,               // 0x22220006, 中子B进延迟计时
  d_coreB_MACHSET_TM_COREOUTCTDLY,              // 0x22220007, 中子B退延迟计时
  d_coreB_MACHSET_TM_COREENDDLY,                // 0x22220008, 中子B动作结束延时
  d_coreB_MACHSET_ADPOSI_EFFCPR2,               // 0x22220009, 途中中子B退下限位置
  d_coreB_MACHSET_TM_MIDDLECOREDLY,             // 0x2222000a, 途中中子B动作延迟
  d_coreB_MACHSET_TM_CPPUMP_DLY,                // 0x2222000b, 特殊中子B阀迟关计时
  d_coreB_MACHSET_COREF_HYDELAY,                // 0x2222000c, 中子B进液压延时
  d_coreB_MACHSET_COREB_HYDELAY,                // 0x2222000d, 中子B退液压延时
  d_coreB_MACHSET_TMDLY_VLV_ONCOREF,            // 0x2222000e, 中子B进阀延迟开计时
  d_coreB_MACHSET_TMDLY_VLV_OFFCOREF,           // 0x2222000f, 中子B进阀延迟关计时
  d_coreB_MACHSET_TMDLY_VLV_ONCOREB,            // 0x22220010, 中子B退阀延迟开计时
  d_coreB_MACHSET_TMDLY_VLV_OFFCOREB,           // 0x22220011, 中子B退阀延迟关计时
  d_coreB_MACHSET_TM_COREINENDDLY,              // 0x22220012, 中子B进动作结束延时
  d_coreB_MACHSET_TM_COREOUTENDDLY,             // 0x22220013, 中子B退动作结束延时
  d_coreB_MACHSET_ADPOSI_EFFCOREINPLUS,         // 0x22220014, 中子B进位置有效区(+)
  d_coreB_MACHSET_ADPOSI_EFFCOREINMINUS,        // 0x22220015, 中子B进位置有效区(-)
  d_coreB_MACHSET_ADPOSI_EFFCOREOUTPLUS,        // 0x22220016, 中子B退位置有效区(+)
  d_coreB_MACHSET_ADPOSI_EFFCOREOUTMINUS        // 0x22220017, 中子B退位置有效区(-)
};

enum db_coreB_MOLDSET{
  d_coreB_MOLDSET_FL_COREPROTECT = 0x22230000, // 0x22230000, 中子B保护功能
  d_coreB_MOLDSET_FL_CORE_CTRL,                // 0x22230001, 中子B功能
  d_coreB_MOLDSET_FL_CORE_TRAVEL,              // 0x22230002, 中子B进控制方式
  d_coreB_MOLDSET_FL_CORE_OUTTRAVEL,           // 0x22230003, 中子B退控制方式
  d_coreB_MOLDSET_FL_SPECIALCODE,              // 0x22230004, 特殊中子B功能
  d_coreB_MOLDSET_FL_INJ_FWDOFF,               // 0x22230005, 射出时中子B进保持
  d_coreB_MOLDSET_ADPOSI_CORE_FWD,             // 0x22230006, 中子B进动作位置
  d_coreB_MOLDSET_ADPOSI_CORE_BWD,             // 0x22230007, 中子B退动作位置
  d_coreB_MOLDSET_PRI_CORE_FWD,                // 0x22230008, 中子B进优先级
  d_coreB_MOLDSET_PRI_CORE_BWD,                // 0x22230009, 中子B退优先级
  d_coreB_MOLDSET_CORE_LIMIT_CHECK,            // 0x2223000a, 进退限位器检查
  d_coreB_MOLDSET_CLAMP_CORE_CHECK,            // 0x2223000b, 开关模检测中子B
  d_coreB_MOLDSET_FL_HOLD_CORE_FWD,            // 0x2223000c, 中子B进保持计时
  d_coreB_MOLDSET_FL_HOLD_CORE_BWD             // 0x2223000d, 中子B退保持计时
};

enum db_coreB_ACTPARA{
  d_coreB_ACTPARA_DAPRES_COREIN = 0x22240000, // 0x22240000, 中子B进压力
  d_coreB_ACTPARA_DAPRES_COREOUT,             // 0x22240001, 中子B退压力
  d_coreB_ACTPARA_DAFLOW_COREIN,              // 0x22240002, 中子B进速度
  d_coreB_ACTPARA_DAFLOW_COREOUT,             // 0x22240003, 中子B退速度
  d_coreB_ACTPARA_TM_COREIN,                  // 0x22240004, 中子B进动作时间
  d_coreB_ACTPARA_TM_COREOUT,                 // 0x22240005, 中子B退动作时间
  d_coreB_ACTPARA_CN_SCREWIN,                 // 0x22240006, 绞牙B进绞牙计数
  d_coreB_ACTPARA_CN_SCREWOUT,                // 0x22240007, 绞牙B退绞牙计数
  d_coreB_ACTPARA_ADPOSI_COREINACT,           // 0x22240008, 中子B进动作位置
  d_coreB_ACTPARA_ADPOSI_COREOUTACT,          // 0x22240009, 中子B退动作位置
  d_coreB_ACTPARA_DAPRES_CORESLOW,            // 0x2224000a, 中子B慢速压力
  d_coreB_ACTPARA_DAFLOW_CORESLOW,            // 0x2224000b, 中子B慢速速度
  d_coreB_ACTPARA_PER_DAFLOW_CORE,            // 0x2224000c, 中子速度使用率
  d_coreB_ACTPARA_DAFLOW_CLSSTARTRAMP_CORE,   // 0x2224000d, 中子B后关模启动斜率
  d_coreB_ACTPARA_DAFLOW_CLSENDRAMP_CORE,     // 0x2224000e, 中子B前关模停止斜率
  d_coreB_ACTPARA_DAFLOW_OPNSTARTRAMP_CORE,   // 0x2224000f, 中子B后开模启动斜率
  d_coreB_ACTPARA_DAFLOW_OPNENDRAMP_CORE      // 0x22240010, 中子B前开模停止斜率
};

enum db_coreB_HYDR{
  d_coreB_HYDR_HYDR_PORT_CORE = 0x22250000, // 0x22250000, 
  d_coreB_HYDR_HYDR_PUMPS_CORE,             // 0x22250001, 
  d_coreB_HYDR_SERVO_KP_CORE,               // 0x22250002, 中子B P
  d_coreB_HYDR_SERVO_KI_CORE,               // 0x22250003, 中子B I
  d_coreB_HYDR_SERVO_KD_CORE                // 0x22250004, 中子B D
};

enum db_coreB_STATE{
  d_coreB_STATE_FL_ERROR0 = 0x22260000, // 0x22260000, 
  d_coreB_STATE_FL_ERROR1,              // 0x22260001, 
  d_coreB_STATE_Step_CoreFwd,           // 0x22260002, 
  d_coreB_STATE_Step_CoreBwd,           // 0x22260003, 
  d_coreB_STATE_COREFWD_STATUS,         // 0x22260004, 中子B进终信号
  d_coreB_STATE_COREBWD_STATUS          // 0x22260005, 中子B退终信号
};

enum db_coreC_PO{
  d_coreC_PO_O_CP_FWD = 0x22300000, // 0x22300000, 中子C进
  d_coreC_PO_O_CP_FSET,             // 0x22300001, 中子C进装模
  d_coreC_PO_O_CP_FRSV,             // 0x22300002, 中子C进备用
  d_coreC_PO_O_CP_BWD,              // 0x22300003, 中子C退
  d_coreC_PO_O_CP_BSET,             // 0x22300004, 中子C退装模
  d_coreC_PO_O_CP_BRSV              // 0x22300005, 中子C退备用
};

enum db_coreC_PI{
  d_coreC_PI_I_CORE_STA = 0x22310000, // 0x22310000, 中子C启动
  d_coreC_PI_I_CP_FWD,                // 0x22310001, 中子C进终
  d_coreC_PI_I_CP_BWD,                // 0x22310002, 中子C退终
  d_coreC_PI_I_CP_ACT,                // 0x22310003, 中子C
  d_coreC_PI_I_SC_CHK,                // 0x22310004, 绞牙C电眼
  d_coreC_PI_I_CP_FWD2                // 0x22310005, 中子C进终2
};

enum db_coreC_MACHSET{
  d_coreC_MACHSET_DAPRES_CORERAMP = 0x22320000, // 0x22320000, 中子C启动压力斜率
  d_coreC_MACHSET_DAFLOW_CORERAMP,              // 0x22320001, 中子C启动速度斜率
  d_coreC_MACHSET_DAPRES_COREOFFRAMP,           // 0x22320002, 中子C关闭压力斜率
  d_coreC_MACHSET_DAFLOW_COREOFFRAMP,           // 0x22320003, 中子C关闭速度斜率
  d_coreC_MACHSET_DAPRES_CORESETUP,             // 0x22320004, 中子C慢速调试压力
  d_coreC_MACHSET_DAFLOW_CORESETUP,             // 0x22320005, 中子C慢速调试速度
  d_coreC_MACHSET_TM_COREINCTDLY,               // 0x22320006, 中子C进延迟计时
  d_coreC_MACHSET_TM_COREOUTCTDLY,              // 0x22320007, 中子C退延迟计时
  d_coreC_MACHSET_TM_COREENDDLY,                // 0x22320008, 中子C动作结束延时
  d_coreC_MACHSET_ADPOSI_EFFCPR2,               // 0x22320009, 途中中子C退下限位置
  d_coreC_MACHSET_TM_MIDDLECOREDLY,             // 0x2232000a, 途中中子C动作延迟
  d_coreC_MACHSET_TM_CPPUMP_DLY,                // 0x2232000b, 特殊中子C阀迟关计时
  d_coreC_MACHSET_COREF_HYDELAY,                // 0x2232000c, 中子C进液压延时
  d_coreC_MACHSET_COREB_HYDELAY,                // 0x2232000d, 中子C退液压延时
  d_coreC_MACHSET_TMDLY_VLV_ONCOREF,            // 0x2232000e, 中子C进阀延迟开计时
  d_coreC_MACHSET_TMDLY_VLV_OFFCOREF,           // 0x2232000f, 中子C进阀延迟关计时
  d_coreC_MACHSET_TMDLY_VLV_ONCOREB,            // 0x22320010, 中子C退阀延迟开计时
  d_coreC_MACHSET_TMDLY_VLV_OFFCOREB,           // 0x22320011, 中子C退阀延迟关计时
  d_coreC_MACHSET_TM_COREINENDDLY,              // 0x22320012, 中子C进动作结束延时
  d_coreC_MACHSET_TM_COREOUTENDDLY,             // 0x22320013, 中子C退动作结束延时
  d_coreC_MACHSET_ADPOSI_EFFCOREINPLUS,         // 0x22320014, 中子C进位置有效区(+)
  d_coreC_MACHSET_ADPOSI_EFFCOREINMINUS,        // 0x22320015, 中子C进位置有效区(-)
  d_coreC_MACHSET_ADPOSI_EFFCOREOUTPLUS,        // 0x22320016, 中子C退位置有效区(+)
  d_coreC_MACHSET_ADPOSI_EFFCOREOUTMINUS        // 0x22320017, 中子C退位置有效区(-)
};

enum db_coreC_MOLDSET{
  d_coreC_MOLDSET_FL_COREPROTECT = 0x22330000, // 0x22330000, 中子C保护功能
  d_coreC_MOLDSET_FL_CORE_CTRL,                // 0x22330001, 中子C功能
  d_coreC_MOLDSET_FL_CORE_TRAVEL,              // 0x22330002, 中子C进控制方式
  d_coreC_MOLDSET_FL_CORE_OUTTRAVEL,           // 0x22330003, 中子C退控制方式
  d_coreC_MOLDSET_FL_SPECIALCODE,              // 0x22330004, 特殊中子C功能
  d_coreC_MOLDSET_FL_INJ_FWDOFF,               // 0x22330005, 射出时中子C进保持
  d_coreC_MOLDSET_ADPOSI_CORE_FWD,             // 0x22330006, 中子C进动作位置
  d_coreC_MOLDSET_ADPOSI_CORE_BWD,             // 0x22330007, 中子C退动作位置
  d_coreC_MOLDSET_PRI_CORE_FWD,                // 0x22330008, 中子C进优先级
  d_coreC_MOLDSET_PRI_CORE_BWD,                // 0x22330009, 中子C退优先级
  d_coreC_MOLDSET_CORE_LIMIT_CHECK,            // 0x2233000a, 进退限位器检查
  d_coreC_MOLDSET_CLAMP_CORE_CHECK,            // 0x2233000b, 开关模检测中子C
  d_coreC_MOLDSET_FL_HOLD_CORE_FWD,            // 0x2233000c, 中子C进保持计时
  d_coreC_MOLDSET_FL_HOLD_CORE_BWD             // 0x2233000d, 中子C退保持计时
};

enum db_coreC_ACTPARA{
  d_coreC_ACTPARA_DAPRES_COREIN = 0x22340000, // 0x22340000, 中子C进压力
  d_coreC_ACTPARA_DAPRES_COREOUT,             // 0x22340001, 中子C退压力
  d_coreC_ACTPARA_DAFLOW_COREIN,              // 0x22340002, 中子C进速度
  d_coreC_ACTPARA_DAFLOW_COREOUT,             // 0x22340003, 中子C退速度
  d_coreC_ACTPARA_TM_COREIN,                  // 0x22340004, 中子C进动作时间
  d_coreC_ACTPARA_TM_COREOUT,                 // 0x22340005, 中子C退动作时间
  d_coreC_ACTPARA_CN_SCREWIN,                 // 0x22340006, 绞牙C进绞牙计数
  d_coreC_ACTPARA_CN_SCREWOUT,                // 0x22340007, 绞牙C退绞牙计数
  d_coreC_ACTPARA_ADPOSI_COREINACT,           // 0x22340008, 中子C进动作位置
  d_coreC_ACTPARA_ADPOSI_COREOUTACT,          // 0x22340009, 中子C退动作位置
  d_coreC_ACTPARA_DAPRES_CORESLOW,            // 0x2234000a, 中子C慢速压力
  d_coreC_ACTPARA_DAFLOW_CORESLOW,            // 0x2234000b, 中子C慢速速度
  d_coreC_ACTPARA_PER_DAFLOW_CORE,            // 0x2234000c, 中子速度使用率
  d_coreC_ACTPARA_DAFLOW_CLSSTARTRAMP_CORE,   // 0x2234000d, 中子C后关模启动斜率
  d_coreC_ACTPARA_DAFLOW_CLSENDRAMP_CORE,     // 0x2234000e, 中子C前关模停止斜率
  d_coreC_ACTPARA_DAFLOW_OPNSTARTRAMP_CORE,   // 0x2234000f, 中子C后开模启动斜率
  d_coreC_ACTPARA_DAFLOW_OPNENDRAMP_CORE      // 0x22340010, 中子C前开模停止斜率
};

enum db_coreC_HYDR{
  d_coreC_HYDR_HYDR_PORT_CORE = 0x22350000, // 0x22350000, 
  d_coreC_HYDR_HYDR_PUMPS_CORE,             // 0x22350001, 
  d_coreC_HYDR_SERVO_KP_CORE,               // 0x22350002, 中子C P
  d_coreC_HYDR_SERVO_KI_CORE,               // 0x22350003, 中子C I
  d_coreC_HYDR_SERVO_KD_CORE                // 0x22350004, 中子C D
};

enum db_coreC_STATE{
  d_coreC_STATE_FL_ERROR0 = 0x22360000, // 0x22360000, 
  d_coreC_STATE_FL_ERROR1,              // 0x22360001, 
  d_coreC_STATE_Step_CoreFwd,           // 0x22360002, 
  d_coreC_STATE_Step_CoreBwd,           // 0x22360003, 
  d_coreC_STATE_COREFWD_STATUS,         // 0x22360004, 中子C进终信号
  d_coreC_STATE_COREBWD_STATUS          // 0x22360005, 中子C退终信号
};

enum db_coreD_PO{
  d_coreD_PO_O_CP_FWD = 0x22400000, // 0x22400000, 中子D进
  d_coreD_PO_O_CP_FSET,             // 0x22400001, 中子D进装模
  d_coreD_PO_O_CP_FRSV,             // 0x22400002, 中子D进备用
  d_coreD_PO_O_CP_BWD,              // 0x22400003, 中子D退
  d_coreD_PO_O_CP_BSET,             // 0x22400004, 中子D退装模
  d_coreD_PO_O_CP_BRSV              // 0x22400005, 中子D退备用
};

enum db_coreD_PI{
  d_coreD_PI_I_CORE_STA = 0x22410000, // 0x22410000, 中子D启动
  d_coreD_PI_I_CP_FWD,                // 0x22410001, 中子D进终
  d_coreD_PI_I_CP_BWD,                // 0x22410002, 中子D退终
  d_coreD_PI_I_CP_ACT,                // 0x22410003, 中子D
  d_coreD_PI_I_SC_CHK,                // 0x22410004, 绞牙D电眼
  d_coreD_PI_I_CP_FWD2                // 0x22410005, 中子D进终2
};

enum db_coreD_MACHSET{
  d_coreD_MACHSET_DAPRES_CORERAMP = 0x22420000, // 0x22420000, 中子D启动压力斜率
  d_coreD_MACHSET_DAFLOW_CORERAMP,              // 0x22420001, 中子D启动速度斜率
  d_coreD_MACHSET_DAPRES_COREOFFRAMP,           // 0x22420002, 中子D关闭压力斜率
  d_coreD_MACHSET_DAFLOW_COREOFFRAMP,           // 0x22420003, 中子D关闭速度斜率
  d_coreD_MACHSET_DAPRES_CORESETUP,             // 0x22420004, 中子D慢速调试压力
  d_coreD_MACHSET_DAFLOW_CORESETUP,             // 0x22420005, 中子D慢速调试速度
  d_coreD_MACHSET_TM_COREINCTDLY,               // 0x22420006, 中子D进延迟计时
  d_coreD_MACHSET_TM_COREOUTCTDLY,              // 0x22420007, 中子D退延迟计时
  d_coreD_MACHSET_TM_COREENDDLY,                // 0x22420008, 中子D动作结束延时
  d_coreD_MACHSET_ADPOSI_EFFCPR2,               // 0x22420009, 途中中子D退下限位置
  d_coreD_MACHSET_TM_MIDDLECOREDLY,             // 0x2242000a, 途中中子D动作延迟
  d_coreD_MACHSET_TM_CPPUMP_DLY,                // 0x2242000b, 特殊中子D阀迟关计时
  d_coreD_MACHSET_COREF_HYDELAY,                // 0x2242000c, 中子D进液压延时
  d_coreD_MACHSET_COREB_HYDELAY,                // 0x2242000d, 中子D退液压延时
  d_coreD_MACHSET_TMDLY_VLV_ONCOREF,            // 0x2242000e, 中子D进阀延迟开计时
  d_coreD_MACHSET_TMDLY_VLV_OFFCOREF,           // 0x2242000f, 中子D进阀延迟关计时
  d_coreD_MACHSET_TMDLY_VLV_ONCOREB,            // 0x22420010, 中子D退阀延迟开计时
  d_coreD_MACHSET_TMDLY_VLV_OFFCOREB,           // 0x22420011, 中子D退阀延迟关计时
  d_coreD_MACHSET_TM_COREINENDDLY,              // 0x22420012, 中子D进动作结束延时
  d_coreD_MACHSET_TM_COREOUTENDDLY,             // 0x22420013, 中子D退动作结束延时
  d_coreD_MACHSET_ADPOSI_EFFCOREINPLUS,         // 0x22420014, 中子D进位置有效区(+)
  d_coreD_MACHSET_ADPOSI_EFFCOREINMINUS,        // 0x22420015, 中子D进位置有效区(-)
  d_coreD_MACHSET_ADPOSI_EFFCOREOUTPLUS,        // 0x22420016, 中子D退位置有效区(+)
  d_coreD_MACHSET_ADPOSI_EFFCOREOUTMINUS        // 0x22420017, 中子D退位置有效区(-)
};

enum db_coreD_MOLDSET{
  d_coreD_MOLDSET_FL_COREPROTECT = 0x22430000, // 0x22430000, 中子D保护功能
  d_coreD_MOLDSET_FL_CORE_CTRL,                // 0x22430001, 中子D功能
  d_coreD_MOLDSET_FL_CORE_TRAVEL,              // 0x22430002, 中子D进控制方式
  d_coreD_MOLDSET_FL_CORE_OUTTRAVEL,           // 0x22430003, 中子D退控制方式
  d_coreD_MOLDSET_FL_SPECIALCODE,              // 0x22430004, 特殊中子D功能
  d_coreD_MOLDSET_FL_INJ_FWDOFF,               // 0x22430005, 射出时中子D进保持
  d_coreD_MOLDSET_ADPOSI_CORE_FWD,             // 0x22430006, 中子D进动作位置
  d_coreD_MOLDSET_ADPOSI_CORE_BWD,             // 0x22430007, 中子D退动作位置
  d_coreD_MOLDSET_PRI_CORE_FWD,                // 0x22430008, 中子D进优先级
  d_coreD_MOLDSET_PRI_CORE_BWD,                // 0x22430009, 中子D退优先级
  d_coreD_MOLDSET_CORE_LIMIT_CHECK,            // 0x2243000a, 进退限位器检查
  d_coreD_MOLDSET_CLAMP_CORE_CHECK,            // 0x2243000b, 开关模检测中子D
  d_coreD_MOLDSET_FL_HOLD_CORE_FWD,            // 0x2243000c, 中子D进保持计时
  d_coreD_MOLDSET_FL_HOLD_CORE_BWD             // 0x2243000d, 中子D退保持计时
};

enum db_coreD_ACTPARA{
  d_coreD_ACTPARA_DAPRES_COREIN = 0x22440000, // 0x22440000, 中子D进压力
  d_coreD_ACTPARA_DAPRES_COREOUT,             // 0x22440001, 中子D退压力
  d_coreD_ACTPARA_DAFLOW_COREIN,              // 0x22440002, 中子D进速度
  d_coreD_ACTPARA_DAFLOW_COREOUT,             // 0x22440003, 中子D退速度
  d_coreD_ACTPARA_TM_COREIN,                  // 0x22440004, 中子D进动作时间
  d_coreD_ACTPARA_TM_COREOUT,                 // 0x22440005, 中子D退动作时间
  d_coreD_ACTPARA_CN_SCREWIN,                 // 0x22440006, 绞牙D进绞牙计数
  d_coreD_ACTPARA_CN_SCREWOUT,                // 0x22440007, 绞牙D退绞牙计数
  d_coreD_ACTPARA_ADPOSI_COREINACT,           // 0x22440008, 中子D进动作位置
  d_coreD_ACTPARA_ADPOSI_COREOUTACT,          // 0x22440009, 中子D退动作位置
  d_coreD_ACTPARA_DAPRES_CORESLOW,            // 0x2244000a, 中子D慢速压力
  d_coreD_ACTPARA_DAFLOW_CORESLOW,            // 0x2244000b, 中子D慢速速度
  d_coreD_ACTPARA_PER_DAFLOW_CORE,            // 0x2244000c, 中子速度使用率
  d_coreD_ACTPARA_DAFLOW_CLSSTARTRAMP_CORE,   // 0x2244000d, 中子D后关模启动斜率
  d_coreD_ACTPARA_DAFLOW_CLSENDRAMP_CORE,     // 0x2244000e, 中子D前关模停止斜率
  d_coreD_ACTPARA_DAFLOW_OPNSTARTRAMP_CORE,   // 0x2244000f, 中子D后开模启动斜率
  d_coreD_ACTPARA_DAFLOW_OPNENDRAMP_CORE      // 0x22440010, 中子D前开模停止斜率
};

enum db_coreD_HYDR{
  d_coreD_HYDR_HYDR_PORT_CORE = 0x22450000, // 0x22450000, 
  d_coreD_HYDR_HYDR_PUMPS_CORE,             // 0x22450001, 
  d_coreD_HYDR_SERVO_KP_CORE,               // 0x22450002, 中子D P
  d_coreD_HYDR_SERVO_KI_CORE,               // 0x22450003, 中子D I
  d_coreD_HYDR_SERVO_KD_CORE                // 0x22450004, 中子D D
};

enum db_coreD_STATE{
  d_coreD_STATE_FL_ERROR0 = 0x22460000, // 0x22460000, 
  d_coreD_STATE_FL_ERROR1,              // 0x22460001, 
  d_coreD_STATE_Step_CoreFwd,           // 0x22460002, 
  d_coreD_STATE_Step_CoreBwd,           // 0x22460003, 
  d_coreD_STATE_COREFWD_STATUS,         // 0x22460004, 中子D进终信号
  d_coreD_STATE_COREBWD_STATUS          // 0x22460005, 中子D退终信号
};

enum db_coreE_PO{
  d_coreE_PO_O_CP_FWD = 0x22500000, // 0x22500000, 中子E进
  d_coreE_PO_O_CP_FSET,             // 0x22500001, 中子E进装模
  d_coreE_PO_O_CP_FRSV,             // 0x22500002, 中子E进备用
  d_coreE_PO_O_CP_BWD,              // 0x22500003, 中子E退
  d_coreE_PO_O_CP_BSET,             // 0x22500004, 中子E退装模
  d_coreE_PO_O_CP_BRSV              // 0x22500005, 中子E退备用
};

enum db_coreE_PI{
  d_coreE_PI_I_CORE_STA = 0x22510000, // 0x22510000, 中子E启动
  d_coreE_PI_I_CP_FWD,                // 0x22510001, 中子E进终
  d_coreE_PI_I_CP_BWD,                // 0x22510002, 中子E退终
  d_coreE_PI_I_CP_ACT,                // 0x22510003, 中子E
  d_coreE_PI_I_SC_CHK,                // 0x22510004, 绞牙E电眼
  d_coreE_PI_I_CP_FWD2                // 0x22510005, 中子E进终2
};

enum db_coreE_MACHSET{
  d_coreE_MACHSET_DAPRES_CORERAMP = 0x22520000, // 0x22520000, 中子E启动压力斜率
  d_coreE_MACHSET_DAFLOW_CORERAMP,              // 0x22520001, 中子E启动速度斜率
  d_coreE_MACHSET_DAPRES_COREOFFRAMP,           // 0x22520002, 中子E关闭压力斜率
  d_coreE_MACHSET_DAFLOW_COREOFFRAMP,           // 0x22520003, 中子E关闭速度斜率
  d_coreE_MACHSET_DAPRES_CORESETUP,             // 0x22520004, 中子E慢速调试压力
  d_coreE_MACHSET_DAFLOW_CORESETUP,             // 0x22520005, 中子E慢速调试速度
  d_coreE_MACHSET_TM_COREINCTDLY,               // 0x22520006, 中子E进延迟计时
  d_coreE_MACHSET_TM_COREOUTCTDLY,              // 0x22520007, 中子E退延迟计时
  d_coreE_MACHSET_TM_COREENDDLY,                // 0x22520008, 中子E动作结束延时
  d_coreE_MACHSET_ADPOSI_EFFCPR2,               // 0x22520009, 途中中子E退下限位置
  d_coreE_MACHSET_TM_MIDDLECOREDLY,             // 0x2252000a, 途中中子E动作延迟
  d_coreE_MACHSET_TM_CPPUMP_DLY,                // 0x2252000b, 特殊中子E阀迟关计时
  d_coreE_MACHSET_COREF_HYDELAY,                // 0x2252000c, 中子E进液压延时
  d_coreE_MACHSET_COREB_HYDELAY,                // 0x2252000d, 中子E退液压延时
  d_coreE_MACHSET_TMDLY_VLV_ONCOREF,            // 0x2252000e, 中子E进阀延迟开计时
  d_coreE_MACHSET_TMDLY_VLV_OFFCOREF,           // 0x2252000f, 中子E进阀延迟关计时
  d_coreE_MACHSET_TMDLY_VLV_ONCOREB,            // 0x22520010, 中子E退阀延迟开计时
  d_coreE_MACHSET_TMDLY_VLV_OFFCOREB,           // 0x22520011, 中子E退阀延迟关计时
  d_coreE_MACHSET_TM_COREINENDDLY,              // 0x22520012, 中子E进动作结束延时
  d_coreE_MACHSET_TM_COREOUTENDDLY,             // 0x22520013, 中子E退动作结束延时
  d_coreE_MACHSET_ADPOSI_EFFCOREINPLUS,         // 0x22520014, 中子E进位置有效区(+)
  d_coreE_MACHSET_ADPOSI_EFFCOREINMINUS,        // 0x22520015, 中子E进位置有效区(-)
  d_coreE_MACHSET_ADPOSI_EFFCOREOUTPLUS,        // 0x22520016, 中子E退位置有效区(+)
  d_coreE_MACHSET_ADPOSI_EFFCOREOUTMINUS        // 0x22520017, 中子E退位置有效区(-)
};

enum db_coreE_MOLDSET{
  d_coreE_MOLDSET_FL_COREPROTECT = 0x22530000, // 0x22530000, 中子E保护功能
  d_coreE_MOLDSET_FL_CORE_CTRL,                // 0x22530001, 中子E功能
  d_coreE_MOLDSET_FL_CORE_TRAVEL,              // 0x22530002, 中子E进控制方式
  d_coreE_MOLDSET_FL_CORE_OUTTRAVEL,           // 0x22530003, 中子E退控制方式
  d_coreE_MOLDSET_FL_SPECIALCODE,              // 0x22530004, 特殊中子E功能
  d_coreE_MOLDSET_FL_INJ_FWDOFF,               // 0x22530005, 射出时中子E进保持
  d_coreE_MOLDSET_ADPOSI_CORE_FWD,             // 0x22530006, 中子E进动作位置
  d_coreE_MOLDSET_ADPOSI_CORE_BWD,             // 0x22530007, 中子E退动作位置
  d_coreE_MOLDSET_PRI_CORE_FWD,                // 0x22530008, 中子E进优先级
  d_coreE_MOLDSET_PRI_CORE_BWD,                // 0x22530009, 中子E退优先级
  d_coreE_MOLDSET_CORE_LIMIT_CHECK,            // 0x2253000a, 进退限位器检查
  d_coreE_MOLDSET_CLAMP_CORE_CHECK,            // 0x2253000b, 开关模检测中子E
  d_coreE_MOLDSET_FL_HOLD_CORE_FWD,            // 0x2253000c, 中子E进保持计时
  d_coreE_MOLDSET_FL_HOLD_CORE_BWD             // 0x2253000d, 中子E退保持计时
};

enum db_coreE_ACTPARA{
  d_coreE_ACTPARA_DAPRES_COREIN = 0x22540000, // 0x22540000, 中子E进压力
  d_coreE_ACTPARA_DAPRES_COREOUT,             // 0x22540001, 中子E退压力
  d_coreE_ACTPARA_DAFLOW_COREIN,              // 0x22540002, 中子E进速度
  d_coreE_ACTPARA_DAFLOW_COREOUT,             // 0x22540003, 中子E退速度
  d_coreE_ACTPARA_TM_COREIN,                  // 0x22540004, 中子E进动作时间
  d_coreE_ACTPARA_TM_COREOUT,                 // 0x22540005, 中子E退动作时间
  d_coreE_ACTPARA_CN_SCREWIN,                 // 0x22540006, 绞牙E进绞牙计数
  d_coreE_ACTPARA_CN_SCREWOUT,                // 0x22540007, 绞牙E退绞牙计数
  d_coreE_ACTPARA_ADPOSI_COREINACT,           // 0x22540008, 中子E进动作位置
  d_coreE_ACTPARA_ADPOSI_COREOUTACT,          // 0x22540009, 中子E退动作位置
  d_coreE_ACTPARA_DAPRES_CORESLOW,            // 0x2254000a, 中子E慢速压力
  d_coreE_ACTPARA_DAFLOW_CORESLOW,            // 0x2254000b, 中子E慢速速度
  d_coreE_ACTPARA_PER_DAFLOW_CORE,            // 0x2254000c, 中子速度使用率
  d_coreE_ACTPARA_DAFLOW_CLSSTARTRAMP_CORE,   // 0x2254000d, 中子E后关模启动斜率
  d_coreE_ACTPARA_DAFLOW_CLSENDRAMP_CORE,     // 0x2254000e, 中子E前关模停止斜率
  d_coreE_ACTPARA_DAFLOW_OPNSTARTRAMP_CORE,   // 0x2254000f, 中子E后开模启动斜率
  d_coreE_ACTPARA_DAFLOW_OPNENDRAMP_CORE      // 0x22540010, 中子E前开模停止斜率
};

enum db_coreE_HYDR{
  d_coreE_HYDR_HYDR_PORT_CORE = 0x22550000, // 0x22550000, 
  d_coreE_HYDR_HYDR_PUMPS_CORE,             // 0x22550001, 
  d_coreE_HYDR_SERVO_KP_CORE,               // 0x22550002, 中子E P
  d_coreE_HYDR_SERVO_KI_CORE,               // 0x22550003, 中子E I
  d_coreE_HYDR_SERVO_KD_CORE                // 0x22550004, 中子E D
};

enum db_coreE_STATE{
  d_coreE_STATE_FL_ERROR0 = 0x22560000, // 0x22560000, 
  d_coreE_STATE_FL_ERROR1,              // 0x22560001, 
  d_coreE_STATE_Step_CoreFwd,           // 0x22560002, 
  d_coreE_STATE_Step_CoreBwd,           // 0x22560003, 
  d_coreE_STATE_COREFWD_STATUS,         // 0x22560004, 中子E进终信号
  d_coreE_STATE_COREBWD_STATUS          // 0x22560005, 中子E退终信号
};

enum db_coreF_PO{
  d_coreF_PO_O_CP_FWD = 0x22600000, // 0x22600000, 中子F进
  d_coreF_PO_O_CP_FSET,             // 0x22600001, 中子F进装模
  d_coreF_PO_O_CP_FRSV,             // 0x22600002, 中子F进备用
  d_coreF_PO_O_CP_BWD,              // 0x22600003, 中子F退
  d_coreF_PO_O_CP_BSET,             // 0x22600004, 中子F退装模
  d_coreF_PO_O_CP_BRSV              // 0x22600005, 中子F退备用
};

enum db_coreF_PI{
  d_coreF_PI_I_CORE_STA = 0x22610000, // 0x22610000, 中子F启动
  d_coreF_PI_I_CP_FWD,                // 0x22610001, 中子F进终
  d_coreF_PI_I_CP_BWD,                // 0x22610002, 中子F退终
  d_coreF_PI_I_CP_ACT,                // 0x22610003, 中子F
  d_coreF_PI_I_SC_CHK,                // 0x22610004, 绞牙F电眼
  d_coreF_PI_I_CP_FWD2                // 0x22610005, 中子F进终2
};

enum db_coreF_MACHSET{
  d_coreF_MACHSET_DAPRES_CORERAMP = 0x22620000, // 0x22620000, 中子F启动压力斜率
  d_coreF_MACHSET_DAFLOW_CORERAMP,              // 0x22620001, 中子F启动速度斜率
  d_coreF_MACHSET_DAPRES_COREOFFRAMP,           // 0x22620002, 中子F关闭压力斜率
  d_coreF_MACHSET_DAFLOW_COREOFFRAMP,           // 0x22620003, 中子F关闭速度斜率
  d_coreF_MACHSET_DAPRES_CORESETUP,             // 0x22620004, 中子F慢速调试压力
  d_coreF_MACHSET_DAFLOW_CORESETUP,             // 0x22620005, 中子F慢速调试速度
  d_coreF_MACHSET_TM_COREINCTDLY,               // 0x22620006, 中子F进延迟计时
  d_coreF_MACHSET_TM_COREOUTCTDLY,              // 0x22620007, 中子F退延迟计时
  d_coreF_MACHSET_TM_COREENDDLY,                // 0x22620008, 中子F动作结束延时
  d_coreF_MACHSET_ADPOSI_EFFCPR2,               // 0x22620009, 途中中子F退下限位置
  d_coreF_MACHSET_TM_MIDDLECOREDLY,             // 0x2262000a, 途中中子F动作延迟
  d_coreF_MACHSET_TM_CPPUMP_DLY,                // 0x2262000b, 特殊中子F阀迟关计时
  d_coreF_MACHSET_COREF_HYDELAY,                // 0x2262000c, 中子F进液压延时
  d_coreF_MACHSET_COREB_HYDELAY,                // 0x2262000d, 中子F退液压延时
  d_coreF_MACHSET_TMDLY_VLV_ONCOREF,            // 0x2262000e, 中子F进阀延迟开计时
  d_coreF_MACHSET_TMDLY_VLV_OFFCOREF,           // 0x2262000f, 中子F进阀延迟关计时
  d_coreF_MACHSET_TMDLY_VLV_ONCOREB,            // 0x22620010, 中子F退阀延迟开计时
  d_coreF_MACHSET_TMDLY_VLV_OFFCOREB,           // 0x22620011, 中子F退阀延迟关计时
  d_coreF_MACHSET_TM_COREINENDDLY,              // 0x22620012, 中子F进动作结束延时
  d_coreF_MACHSET_TM_COREOUTENDDLY,             // 0x22620013, 中子F退动作结束延时
  d_coreF_MACHSET_ADPOSI_EFFCOREINPLUS,         // 0x22620014, 中子F进位置有效区(+)
  d_coreF_MACHSET_ADPOSI_EFFCOREINMINUS,        // 0x22620015, 中子F进位置有效区(-)
  d_coreF_MACHSET_ADPOSI_EFFCOREOUTPLUS,        // 0x22620016, 中子F退位置有效区(+)
  d_coreF_MACHSET_ADPOSI_EFFCOREOUTMINUS        // 0x22620017, 中子F退位置有效区(-)
};

enum db_coreF_MOLDSET{
  d_coreF_MOLDSET_FL_COREPROTECT = 0x22630000, // 0x22630000, 中子F保护功能
  d_coreF_MOLDSET_FL_CORE_CTRL,                // 0x22630001, 中子F功能
  d_coreF_MOLDSET_FL_CORE_TRAVEL,              // 0x22630002, 中子F进控制方式
  d_coreF_MOLDSET_FL_CORE_OUTTRAVEL,           // 0x22630003, 中子F退控制方式
  d_coreF_MOLDSET_FL_SPECIALCODE,              // 0x22630004, 特殊中子F功能
  d_coreF_MOLDSET_FL_INJ_FWDOFF,               // 0x22630005, 射出时中子F进保持
  d_coreF_MOLDSET_ADPOSI_CORE_FWD,             // 0x22630006, 中子F进动作位置
  d_coreF_MOLDSET_ADPOSI_CORE_BWD,             // 0x22630007, 中子F退动作位置
  d_coreF_MOLDSET_PRI_CORE_FWD,                // 0x22630008, 中子F进优先级
  d_coreF_MOLDSET_PRI_CORE_BWD,                // 0x22630009, 中子F退优先级
  d_coreF_MOLDSET_CORE_LIMIT_CHECK,            // 0x2263000a, 进退限位器检查
  d_coreF_MOLDSET_CLAMP_CORE_CHECK,            // 0x2263000b, 开关模检测中子F
  d_coreF_MOLDSET_FL_HOLD_CORE_FWD,            // 0x2263000c, 中子F进保持计时
  d_coreF_MOLDSET_FL_HOLD_CORE_BWD             // 0x2263000d, 中子F退保持计时
};

enum db_coreF_ACTPARA{
  d_coreF_ACTPARA_DAPRES_COREIN = 0x22640000, // 0x22640000, 中子F进压力
  d_coreF_ACTPARA_DAPRES_COREOUT,             // 0x22640001, 中子F退压力
  d_coreF_ACTPARA_DAFLOW_COREIN,              // 0x22640002, 中子F进速度
  d_coreF_ACTPARA_DAFLOW_COREOUT,             // 0x22640003, 中子F退速度
  d_coreF_ACTPARA_TM_COREIN,                  // 0x22640004, 中子F进动作时间
  d_coreF_ACTPARA_TM_COREOUT,                 // 0x22640005, 中子F退动作时间
  d_coreF_ACTPARA_CN_SCREWIN,                 // 0x22640006, 绞牙F进绞牙计数
  d_coreF_ACTPARA_CN_SCREWOUT,                // 0x22640007, 绞牙F退绞牙计数
  d_coreF_ACTPARA_ADPOSI_COREINACT,           // 0x22640008, 中子F进动作位置
  d_coreF_ACTPARA_ADPOSI_COREOUTACT,          // 0x22640009, 中子F退动作位置
  d_coreF_ACTPARA_DAPRES_CORESLOW,            // 0x2264000a, 中子F慢速压力
  d_coreF_ACTPARA_DAFLOW_CORESLOW,            // 0x2264000b, 中子F慢速速度
  d_coreF_ACTPARA_PER_DAFLOW_CORE,            // 0x2264000c, 中子速度使用率
  d_coreF_ACTPARA_DAFLOW_CLSSTARTRAMP_CORE,   // 0x2264000d, 中子F后关模启动斜率
  d_coreF_ACTPARA_DAFLOW_CLSENDRAMP_CORE,     // 0x2264000e, 中子F前关模停止斜率
  d_coreF_ACTPARA_DAFLOW_OPNSTARTRAMP_CORE,   // 0x2264000f, 中子F后开模启动斜率
  d_coreF_ACTPARA_DAFLOW_OPNENDRAMP_CORE      // 0x22640010, 中子F前开模停止斜率
};

enum db_coreF_HYDR{
  d_coreF_HYDR_HYDR_PORT_CORE = 0x22650000, // 0x22650000, 
  d_coreF_HYDR_HYDR_PUMPS_CORE,             // 0x22650001, 
  d_coreF_HYDR_SERVO_KP_CORE,               // 0x22650002, 中子F P
  d_coreF_HYDR_SERVO_KI_CORE,               // 0x22650003, 中子F I
  d_coreF_HYDR_SERVO_KD_CORE                // 0x22650004, 中子F D
};

enum db_coreF_STATE{
  d_coreF_STATE_FL_ERROR0 = 0x22660000, // 0x22660000, 
  d_coreF_STATE_FL_ERROR1,              // 0x22660001, 
  d_coreF_STATE_Step_CoreFwd,           // 0x22660002, 中子进动作步显示
  d_coreF_STATE_Step_CoreBwd,           // 0x22660003, 中子退动作步显示
  d_coreF_STATE_COREFWD_STATUS,         // 0x22660004, 中子F进终信号
  d_coreF_STATE_COREBWD_STATUS          // 0x22660005, 中子F退终信号
};

enum db_airblow1_PO{
  d_airblow1_PO_O_AIR_BLT = 0x23100000, // 0x23100000, 吹气1
  d_airblow1_PO_O_AIR_SRV,              // 0x23100001, 吹气1备用
  d_airblow1_PO_O_AIR_SEQVLV1,          // 0x23100002, 顺序阀1
  d_airblow1_PO_O_AIR_SEQVLV2,          // 0x23100003, 顺序阀2
  d_airblow1_PO_O_AIR_SEQVLV3,          // 0x23100004, 顺序阀3
  d_airblow1_PO_O_AIR_JKF1,             // 0x23100005, 胶口针阀1-1
  d_airblow1_PO_O_AIR_JKF2,             // 0x23100006, 胶口针阀1-2
  d_airblow1_PO_O_AIR_BLT2              // 0x23100007, 吹气15
};

enum db_airblow1_MOLDSET{
  d_airblow1_MOLDSET_FL_BLAST = 0x23130000, // 0x23130000, 吹气1方式
  d_airblow1_MOLDSET_ADPOSI_MOVINGBLASTSTART, // 0x23130001, 吹气1动作位置
  d_airblow1_MOLDSET_TM_MOVINGPLATENDLY,    // 0x23130002, 吹气1延迟计时
  d_airblow1_MOLDSET_TM_MOVINGPLATENACT,    // 0x23130003, 吹气1动作时间
  d_airblow1_MOLDSET_TM_AIRDLY,             // 0x23130004, 吹气1间歇时间
  d_airblow1_MOLDSET_TM_AIRALL,             // 0x23130005, 吹气1总时间
  d_airblow1_MOLDSET_FL_SEQ_USE,            // 0x23130006, 顺序阀1功能
  d_airblow1_MOLDSET_TM_SEQ_DELAY,          // 0x23130007, 顺序阀1延时
  d_airblow1_MOLDSET_TM_SEQ_MOVING,         // 0x23130008, 顺序阀1动作时间
  d_airblow1_MOLDSET_ADPOSI_SEQSTART,       // 0x23130009, 顺序阀1动作起始位置
  d_airblow1_MOLDSET_ADPOSI_SEQEND,         // 0x2313000a, 顺序阀1动作结束位置
  d_airblow1_MOLDSET_FL_SEQ_USE2,           // 0x2313000b, 顺序阀2功能
  d_airblow1_MOLDSET_TM_SEQ_DELAY2,         // 0x2313000c, 顺序阀2延时
  d_airblow1_MOLDSET_TM_SEQ_MOVING2,        // 0x2313000d, 顺序阀2动作时间
  d_airblow1_MOLDSET_ADPOSI_SEQSTART2,      // 0x2313000e, 顺序阀2动作起始位置
  d_airblow1_MOLDSET_ADPOSI_SEQEND2,        // 0x2313000f, 顺序阀2动作结束位置
  d_airblow1_MOLDSET_FL_SEQ_USE3,           // 0x23130010, 顺序阀3功能
  d_airblow1_MOLDSET_TM_SEQ_DELAY3,         // 0x23130011, 顺序阀3延时
  d_airblow1_MOLDSET_TM_SEQ_MOVING3,        // 0x23130012, 顺序阀3动作时间
  d_airblow1_MOLDSET_ADPOSI_SEQSTART3,      // 0x23130013, 顺序阀3动作起始位置
  d_airblow1_MOLDSET_ADPOSI_SEQEND3,        // 0x23130014, 顺序阀3动作结束位置
  d_airblow1_MOLDSET_FL_BLAST2,             // 0x23130015, 吹气15方式
  d_airblow1_MOLDSET_ADPOSI_MOVINGBLASTSTART2, // 0x23130016, 吹气15动作位置
  d_airblow1_MOLDSET_TM_MOVINGPLATENDLY2,   // 0x23130017, 吹气15延迟计时
  d_airblow1_MOLDSET_TM_MOVINGPLATENACT2,   // 0x23130018, 吹气15动作时间
  d_airblow1_MOLDSET_TM_AIRDLY2,            // 0x23130019, 吹气15间歇时间
  d_airblow1_MOLDSET_TM_AIRALL2,            // 0x2313001a, 吹气15总时间
  d_airblow1_MOLDSET_FL_SEQ_HOLD_USE1,      // 0x2313001b, 顺序阀1保压使用
  d_airblow1_MOLDSET_TM_SEQ_HOLD_DELAY1,    // 0x2313001c, 顺序阀1保压延迟计时
  d_airblow1_MOLDSET_TM_SEQ_HOLD_MOVING1,   // 0x2313001d, 顺序阀1保压动作时间
  d_airblow1_MOLDSET_FL_SEQ_HOLD_USE2,      // 0x2313001e, 顺序阀2保压使用
  d_airblow1_MOLDSET_TM_SEQ_HOLD_DELAY2,    // 0x2313001f, 顺序阀2保压延迟计时
  d_airblow1_MOLDSET_TM_SEQ_HOLD_MOVING2,   // 0x23130020, 顺序阀2保压动作时间
  d_airblow1_MOLDSET_FL_SEQ_HOLD_USE3,      // 0x23130021, 顺序阀3保压使用
  d_airblow1_MOLDSET_TM_SEQ_HOLD_DELAY3,    // 0x23130022, 顺序阀3保压延迟计时
  d_airblow1_MOLDSET_TM_SEQ_HOLD_MOVING3    // 0x23130023, 顺序阀3保压动作时间
};

enum db_airblow1_STATE{
  d_airblow1_STATE_FL_ERROR0 = 0x23160000, // 0x23160000, 警报1
  d_airblow1_STATE_FL_ERROR1,              // 0x23160001, 警报2
  d_airblow1_STATE_Step_AirBlow,           // 0x23160002, 吹气实时动作步
  d_airblow1_STATE_Step_SEQ                // 0x23160003, 顺序阀实时动作步
};

enum db_airblow2_PO{
  d_airblow2_PO_O_AIR_BLT = 0x23200000, // 0x23200000, 吹气2
  d_airblow2_PO_O_AIR_SRV,              // 0x23200001, 吹气2备用
  d_airblow2_PO_O_AIR_SEQVLV1,          // 0x23200002, 顺序阀4
  d_airblow2_PO_O_AIR_SEQVLV2,          // 0x23200003, 顺序阀5
  d_airblow2_PO_O_AIR_SEQVLV3,          // 0x23200004, 顺序阀6
  d_airblow2_PO_O_AIR_JKF1,             // 0x23200005, 胶口针阀2-1
  d_airblow2_PO_O_AIR_JKF2,             // 0x23200006, 胶口针阀2-2
  d_airblow2_PO_O_AIR_BLT2              // 0x23200007, 吹气16
};

enum db_airblow2_MOLDSET{
  d_airblow2_MOLDSET_FL_BLAST = 0x23230000, // 0x23230000, 吹气2方式
  d_airblow2_MOLDSET_ADPOSI_MOVINGBLASTSTART, // 0x23230001, 吹气2动作位置
  d_airblow2_MOLDSET_TM_MOVINGPLATENDLY,    // 0x23230002, 吹气2延迟计时
  d_airblow2_MOLDSET_TM_MOVINGPLATENACT,    // 0x23230003, 吹气2动作时间
  d_airblow2_MOLDSET_TM_AIRDLY,             // 0x23230004, 吹气2间歇时间
  d_airblow2_MOLDSET_TM_AIRALL,             // 0x23230005, 吹气2总时间
  d_airblow2_MOLDSET_FL_SEQ_USE,            // 0x23230006, 顺序阀4功能
  d_airblow2_MOLDSET_TM_SEQ_DELAY,          // 0x23230007, 顺序阀4延时
  d_airblow2_MOLDSET_TM_SEQ_MOVING,         // 0x23230008, 顺序阀4动作时间
  d_airblow2_MOLDSET_ADPOSI_SEQSTART,       // 0x23230009, 顺序阀4动作起始位置
  d_airblow2_MOLDSET_ADPOSI_SEQEND,         // 0x2323000a, 顺序阀4动作结束位置
  d_airblow2_MOLDSET_FL_SEQ_USE2,           // 0x2323000b, 顺序阀5功能
  d_airblow2_MOLDSET_TM_SEQ_DELAY2,         // 0x2323000c, 顺序阀5延时
  d_airblow2_MOLDSET_TM_SEQ_MOVING2,        // 0x2323000d, 顺序阀5动作时间
  d_airblow2_MOLDSET_ADPOSI_SEQSTART2,      // 0x2323000e, 顺序阀5动作起始位置
  d_airblow2_MOLDSET_ADPOSI_SEQEND2,        // 0x2323000f, 顺序阀5动作结束位置
  d_airblow2_MOLDSET_FL_SEQ_USE3,           // 0x23230010, 顺序阀6功能
  d_airblow2_MOLDSET_TM_SEQ_DELAY3,         // 0x23230011, 顺序阀6延时
  d_airblow2_MOLDSET_TM_SEQ_MOVING3,        // 0x23230012, 顺序阀6动作时间
  d_airblow2_MOLDSET_ADPOSI_SEQSTART3,      // 0x23230013, 顺序阀6动作起始位置
  d_airblow2_MOLDSET_ADPOSI_SEQEND3,        // 0x23230014, 顺序阀6动作结束位置
  d_airblow2_MOLDSET_FL_BLAST2,             // 0x23230015, 吹气16方式
  d_airblow2_MOLDSET_ADPOSI_MOVINGBLASTSTART2, // 0x23230016, 吹气16动作位置
  d_airblow2_MOLDSET_TM_MOVINGPLATENDLY2,   // 0x23230017, 吹气16延迟计时
  d_airblow2_MOLDSET_TM_MOVINGPLATENACT2,   // 0x23230018, 吹气16动作时间
  d_airblow2_MOLDSET_TM_AIRDLY2,            // 0x23230019, 吹气16间歇时间
  d_airblow2_MOLDSET_TM_AIRALL2,            // 0x2323001a, 吹气16总时间
  d_airblow2_MOLDSET_FL_SEQ_HOLD_USE4,      // 0x2323001b, 顺序阀4保压使用
  d_airblow2_MOLDSET_TM_SEQ_HOLD_DELAY4,    // 0x2323001c, 顺序阀4保压延迟计时
  d_airblow2_MOLDSET_TM_SEQ_HOLD_MOVING4,   // 0x2323001d, 顺序阀4保压动作时间
  d_airblow2_MOLDSET_FL_SEQ_HOLD_USE5,      // 0x2323001e, 顺序阀5保压使用
  d_airblow2_MOLDSET_TM_SEQ_HOLD_DELAY5,    // 0x2323001f, 顺序阀5保压延迟计时
  d_airblow2_MOLDSET_TM_SEQ_HOLD_MOVING5,   // 0x23230020, 顺序阀5保压动作时间
  d_airblow2_MOLDSET_FL_SEQ_HOLD_USE6,      // 0x23230021, 顺序阀6保压使用
  d_airblow2_MOLDSET_TM_SEQ_HOLD_DELAY6,    // 0x23230022, 顺序阀6保压延迟计时
  d_airblow2_MOLDSET_TM_SEQ_HOLD_MOVING6    // 0x23230023, 顺序阀6保压动作时间
};

enum db_airblow2_STATE{
  d_airblow2_STATE_FL_ERROR0 = 0x23260000, // 0x23260000, 警报1
  d_airblow2_STATE_FL_ERROR1,              // 0x23260001, 警报2
  d_airblow2_STATE_Step_AirBlow,           // 0x23260002, 
  d_airblow2_STATE_Step_SEQ                // 0x23260003, 
};

enum db_airblow3_PO{
  d_airblow3_PO_O_AIR_BLT = 0x23300000, // 0x23300000, 吹气3
  d_airblow3_PO_O_AIR_SRV,              // 0x23300001, 吹气3备用
  d_airblow3_PO_O_AIR_SEQVLV1,          // 0x23300002, 顺序阀7
  d_airblow3_PO_O_AIR_SEQVLV2,          // 0x23300003, 顺序阀8
  d_airblow3_PO_O_AIR_SEQVLV3,          // 0x23300004, 顺序阀9
  d_airblow3_PO_O_AIR_JKF1,             // 0x23300005, 胶口针阀3-1
  d_airblow3_PO_O_AIR_JKF2,             // 0x23300006, 胶口针阀3-2
  d_airblow3_PO_O_AIR_BLT2              // 0x23300007, 吹气17
};

enum db_airblow3_MOLDSET{
  d_airblow3_MOLDSET_FL_BLAST = 0x23330000, // 0x23330000, 吹气3方式
  d_airblow3_MOLDSET_ADPOSI_MOVINGBLASTSTART, // 0x23330001, 吹气3动作位置
  d_airblow3_MOLDSET_TM_MOVINGPLATENDLY,    // 0x23330002, 吹气3延迟计时
  d_airblow3_MOLDSET_TM_MOVINGPLATENACT,    // 0x23330003, 吹气3动作时间
  d_airblow3_MOLDSET_TM_AIRDLY,             // 0x23330004, 吹气3间歇时间
  d_airblow3_MOLDSET_TM_AIRALL,             // 0x23330005, 吹气3总时间
  d_airblow3_MOLDSET_FL_SEQ_USE,            // 0x23330006, 顺序阀7功能
  d_airblow3_MOLDSET_TM_SEQ_DELAY,          // 0x23330007, 顺序阀7延时
  d_airblow3_MOLDSET_TM_SEQ_MOVING,         // 0x23330008, 顺序阀7动作时间
  d_airblow3_MOLDSET_ADPOSI_SEQSTART,       // 0x23330009, 顺序阀7动作起始位置
  d_airblow3_MOLDSET_ADPOSI_SEQEND,         // 0x2333000a, 顺序阀7动作结束位置
  d_airblow3_MOLDSET_FL_SEQ_USE2,           // 0x2333000b, 顺序阀8功能
  d_airblow3_MOLDSET_TM_SEQ_DELAY2,         // 0x2333000c, 顺序阀8延时
  d_airblow3_MOLDSET_TM_SEQ_MOVING2,        // 0x2333000d, 顺序阀8动作时间
  d_airblow3_MOLDSET_ADPOSI_SEQSTART2,      // 0x2333000e, 顺序阀8动作起始位置
  d_airblow3_MOLDSET_ADPOSI_SEQEND2,        // 0x2333000f, 顺序阀8动作结束位置
  d_airblow3_MOLDSET_FL_SEQ_USE3,           // 0x23330010, 顺序阀9功能
  d_airblow3_MOLDSET_TM_SEQ_DELAY3,         // 0x23330011, 顺序阀9延时
  d_airblow3_MOLDSET_TM_SEQ_MOVING3,        // 0x23330012, 顺序阀9动作时间
  d_airblow3_MOLDSET_ADPOSI_SEQSTART3,      // 0x23330013, 顺序阀9动作起始位置
  d_airblow3_MOLDSET_ADPOSI_SEQEND3,        // 0x23330014, 顺序阀9动作结束位置
  d_airblow3_MOLDSET_FL_BLAST2,             // 0x23330015, 吹气17方式
  d_airblow3_MOLDSET_ADPOSI_MOVINGBLASTSTART2, // 0x23330016, 吹气17动作位置
  d_airblow3_MOLDSET_TM_MOVINGPLATENDLY2,   // 0x23330017, 吹气17延迟计时
  d_airblow3_MOLDSET_TM_MOVINGPLATENACT2,   // 0x23330018, 吹气17动作时间
  d_airblow3_MOLDSET_TM_AIRDLY2,            // 0x23330019, 吹气17间歇时间
  d_airblow3_MOLDSET_TM_AIRALL2,            // 0x2333001a, 吹气17总时间
  d_airblow3_MOLDSET_FL_SEQ_HOLD_USE7,      // 0x2333001b, 顺序阀7保压使用
  d_airblow3_MOLDSET_TM_SEQ_HOLD_DELAY7,    // 0x2333001c, 顺序阀7保压延迟计时
  d_airblow3_MOLDSET_TM_SEQ_HOLD_MOVING7,   // 0x2333001d, 顺序阀7保压动作时间
  d_airblow3_MOLDSET_FL_SEQ_HOLD_USE8,      // 0x2333001e, 顺序阀8保压使用
  d_airblow3_MOLDSET_TM_SEQ_HOLD_DELAY8,    // 0x2333001f, 顺序阀8保压延迟计时
  d_airblow3_MOLDSET_TM_SEQ_HOLD_MOVING8,   // 0x23330020, 顺序阀8保压动作时间
  d_airblow3_MOLDSET_FL_SEQ_HOLD_USE9,      // 0x23330021, 顺序阀9保压使用
  d_airblow3_MOLDSET_TM_SEQ_HOLD_DELAY9,    // 0x23330022, 顺序阀9保压延迟计时
  d_airblow3_MOLDSET_TM_SEQ_HOLD_MOVING9    // 0x23330023, 顺序阀9保压动作时间
};

enum db_airblow3_STATE{
  d_airblow3_STATE_FL_ERROR0 = 0x23360000, // 0x23360000, 
  d_airblow3_STATE_FL_ERROR1,              // 0x23360001, 
  d_airblow3_STATE_Step_AirBlow,           // 0x23360002, 
  d_airblow3_STATE_Step_SEQ                // 0x23360003, 
};

enum db_airblow4_PO{
  d_airblow4_PO_O_AIR_BLT = 0x23400000, // 0x23400000, 吹气4
  d_airblow4_PO_O_AIR_SRV,              // 0x23400001, 吹气4备用
  d_airblow4_PO_O_AIR_SEQVLV1,          // 0x23400002, 顺序阀10
  d_airblow4_PO_O_AIR_SEQVLV2,          // 0x23400003, 顺序阀11
  d_airblow4_PO_O_AIR_SEQVLV3,          // 0x23400004, 顺序阀12
  d_airblow4_PO_O_AIR_JKF1,             // 0x23400005, 胶口针阀4-1
  d_airblow4_PO_O_AIR_JKF2,             // 0x23400006, 胶口针阀4-2
  d_airblow4_PO_O_AIR_BLT2              // 0x23400007, 吹气18
};

enum db_airblow4_MOLDSET{
  d_airblow4_MOLDSET_FL_BLAST = 0x23430000, // 0x23430000, 吹气4方式
  d_airblow4_MOLDSET_ADPOSI_MOVINGBLASTSTART, // 0x23430001, 吹气4动作位置
  d_airblow4_MOLDSET_TM_MOVINGPLATENDLY,    // 0x23430002, 吹气4延迟计时
  d_airblow4_MOLDSET_TM_MOVINGPLATENACT,    // 0x23430003, 吹气4动作时间
  d_airblow4_MOLDSET_TM_AIRDLY,             // 0x23430004, 吹气4间歇时间
  d_airblow4_MOLDSET_TM_AIRALL,             // 0x23430005, 吹气4总时间
  d_airblow4_MOLDSET_FL_SEQ_USE,            // 0x23430006, 顺序阀10功能
  d_airblow4_MOLDSET_TM_SEQ_DELAY,          // 0x23430007, 顺序阀10延时
  d_airblow4_MOLDSET_TM_SEQ_MOVING,         // 0x23430008, 顺序阀10动作时间
  d_airblow4_MOLDSET_ADPOSI_SEQSTART,       // 0x23430009, 顺序阀10动作起始位置
  d_airblow4_MOLDSET_ADPOSI_SEQEND,         // 0x2343000a, 顺序阀10动作结束位置
  d_airblow4_MOLDSET_FL_SEQ_USE2,           // 0x2343000b, 顺序阀11功能
  d_airblow4_MOLDSET_TM_SEQ_DELAY2,         // 0x2343000c, 顺序阀11延时
  d_airblow4_MOLDSET_TM_SEQ_MOVING2,        // 0x2343000d, 顺序阀11动作时间
  d_airblow4_MOLDSET_ADPOSI_SEQSTART2,      // 0x2343000e, 顺序阀11动作起始位置
  d_airblow4_MOLDSET_ADPOSI_SEQEND2,        // 0x2343000f, 顺序阀11动作结束位置
  d_airblow4_MOLDSET_FL_SEQ_USE3,           // 0x23430010, 顺序阀12功能
  d_airblow4_MOLDSET_TM_SEQ_DELAY3,         // 0x23430011, 顺序阀12延时
  d_airblow4_MOLDSET_TM_SEQ_MOVING3,        // 0x23430012, 顺序阀12动作时间
  d_airblow4_MOLDSET_ADPOSI_SEQSTART3,      // 0x23430013, 顺序阀12动作起始位置
  d_airblow4_MOLDSET_ADPOSI_SEQEND3,        // 0x23430014, 顺序阀12动作结束位置
  d_airblow4_MOLDSET_FL_BLAST2,             // 0x23430015, 吹气18方式
  d_airblow4_MOLDSET_ADPOSI_MOVINGBLASTSTART2, // 0x23430016, 吹气18动作位置
  d_airblow4_MOLDSET_TM_MOVINGPLATENDLY2,   // 0x23430017, 吹气18延迟计时
  d_airblow4_MOLDSET_TM_MOVINGPLATENACT2,   // 0x23430018, 吹气18动作时间
  d_airblow4_MOLDSET_TM_AIRDLY2,            // 0x23430019, 吹气18间歇时间
  d_airblow4_MOLDSET_TM_AIRALL2,            // 0x2343001a, 吹气18总时间
  d_airblow4_MOLDSET_FL_SEQ_HOLD_USE10,     // 0x2343001b, 顺序阀10保压使用
  d_airblow4_MOLDSET_TM_SEQ_HOLD_DELAY10,   // 0x2343001c, 顺序阀10保压延迟计时
  d_airblow4_MOLDSET_TM_SEQ_HOLD_MOVING10,  // 0x2343001d, 顺序阀10保压动作时间
  d_airblow4_MOLDSET_FL_SEQ_HOLD_USE11,     // 0x2343001e, 顺序阀11保压使用
  d_airblow4_MOLDSET_TM_SEQ_HOLD_DELAY11,   // 0x2343001f, 顺序阀11保压延迟计时
  d_airblow4_MOLDSET_TM_SEQ_HOLD_MOVING11,  // 0x23430020, 顺序阀11保压动作时间
  d_airblow4_MOLDSET_FL_SEQ_HOLD_USE12,     // 0x23430021, 顺序阀12保压使用
  d_airblow4_MOLDSET_TM_SEQ_HOLD_DELAY12,   // 0x23430022, 顺序阀12保压延迟计时
  d_airblow4_MOLDSET_TM_SEQ_HOLD_MOVING12   // 0x23430023, 顺序阀12保压动作时间
};

enum db_airblow4_STATE{
  d_airblow4_STATE_FL_ERROR0 = 0x23460000, // 0x23460000, 
  d_airblow4_STATE_FL_ERROR1,              // 0x23460001, 
  d_airblow4_STATE_Step_AirBlow,           // 0x23460002, 
  d_airblow4_STATE_Step_SEQ                // 0x23460003, 
};

enum db_airblow5_PO{
  d_airblow5_PO_O_AIR_BLT = 0x23500000, // 0x23500000, 吹气5
  d_airblow5_PO_O_AIR_SRV,              // 0x23500001, 吹气5备用
  d_airblow5_PO_O_AIR_SEQVLV1,          // 0x23500002, 顺序阀13
  d_airblow5_PO_O_AIR_SEQVLV2,          // 0x23500003, 顺序阀14
  d_airblow5_PO_O_AIR_SEQVLV3,          // 0x23500004, 顺序阀15
  d_airblow5_PO_O_AIR_JKF1,             // 0x23500005, 胶口针阀5-1
  d_airblow5_PO_O_AIR_JKF2,             // 0x23500006, 胶口针阀5-2
  d_airblow5_PO_O_AIR_BLT2              // 0x23500007, 吹气19
};

enum db_airblow5_MOLDSET{
  d_airblow5_MOLDSET_FL_BLAST = 0x23530000, // 0x23530000, 吹气5方式
  d_airblow5_MOLDSET_ADPOSI_MOVINGBLASTSTART, // 0x23530001, 吹气5动作位置
  d_airblow5_MOLDSET_TM_MOVINGPLATENDLY,    // 0x23530002, 吹气5延迟计时
  d_airblow5_MOLDSET_TM_MOVINGPLATENACT,    // 0x23530003, 吹气5动作时间
  d_airblow5_MOLDSET_TM_AIRDLY,             // 0x23530004, 吹气5间歇时间
  d_airblow5_MOLDSET_TM_AIRALL,             // 0x23530005, 吹气5总时间
  d_airblow5_MOLDSET_FL_SEQ_USE,            // 0x23530006, 顺序阀13功能
  d_airblow5_MOLDSET_TM_SEQ_DELAY,          // 0x23530007, 顺序阀13延时
  d_airblow5_MOLDSET_TM_SEQ_MOVING,         // 0x23530008, 顺序阀13动作时间
  d_airblow5_MOLDSET_ADPOSI_SEQSTART,       // 0x23530009, 顺序阀13动作起始位置
  d_airblow5_MOLDSET_ADPOSI_SEQEND,         // 0x2353000a, 顺序阀13动作结束位置
  d_airblow5_MOLDSET_FL_SEQ_USE2,           // 0x2353000b, 顺序阀14功能
  d_airblow5_MOLDSET_TM_SEQ_DELAY2,         // 0x2353000c, 顺序阀14延时
  d_airblow5_MOLDSET_TM_SEQ_MOVING2,        // 0x2353000d, 顺序阀14动作时间
  d_airblow5_MOLDSET_ADPOSI_SEQSTART2,      // 0x2353000e, 顺序阀14动作起始位置
  d_airblow5_MOLDSET_ADPOSI_SEQEND2,        // 0x2353000f, 顺序阀14动作结束位置
  d_airblow5_MOLDSET_FL_SEQ_USE3,           // 0x23530010, 顺序阀15功能
  d_airblow5_MOLDSET_TM_SEQ_DELAY3,         // 0x23530011, 顺序阀15延时
  d_airblow5_MOLDSET_TM_SEQ_MOVING3,        // 0x23530012, 顺序阀15动作时间
  d_airblow5_MOLDSET_ADPOSI_SEQSTART3,      // 0x23530013, 顺序阀15动作起始位置
  d_airblow5_MOLDSET_ADPOSI_SEQEND3,        // 0x23530014, 顺序阀15动作结束位置
  d_airblow5_MOLDSET_FL_BLAST2,             // 0x23530015, 吹气19方式
  d_airblow5_MOLDSET_ADPOSI_MOVINGBLASTSTART2, // 0x23530016, 吹气19动作位置
  d_airblow5_MOLDSET_TM_MOVINGPLATENDLY2,   // 0x23530017, 吹气19延迟计时
  d_airblow5_MOLDSET_TM_MOVINGPLATENACT2,   // 0x23530018, 吹气19动作时间
  d_airblow5_MOLDSET_TM_AIRDLY2,            // 0x23530019, 吹气19间歇时间
  d_airblow5_MOLDSET_TM_AIRALL2,            // 0x2353001a, 吹气19总时间
  d_airblow5_MOLDSET_FL_SEQ_HOLD_USE13,     // 0x2353001b, 顺序阀13保压使用
  d_airblow5_MOLDSET_TM_SEQ_HOLD_DELAY13,   // 0x2353001c, 顺序阀13保压延迟计时
  d_airblow5_MOLDSET_TM_SEQ_HOLD_MOVING13,  // 0x2353001d, 顺序阀13保压动作时间
  d_airblow5_MOLDSET_FL_SEQ_HOLD_USE14,     // 0x2353001e, 顺序阀14保压使用
  d_airblow5_MOLDSET_TM_SEQ_HOLD_DELAY14,   // 0x2353001f, 顺序阀14保压延迟计时
  d_airblow5_MOLDSET_TM_SEQ_HOLD_MOVING14,  // 0x23530020, 顺序阀14保压动作时间
  d_airblow5_MOLDSET_FL_SEQ_HOLD_USE15,     // 0x23530021, 顺序阀15保压使用
  d_airblow5_MOLDSET_TM_SEQ_HOLD_DELAY15,   // 0x23530022, 顺序阀15保压延迟计时
  d_airblow5_MOLDSET_TM_SEQ_HOLD_MOVING15   // 0x23530023, 顺序阀15保压动作时间
};

enum db_airblow5_STATE{
  d_airblow5_STATE_FL_ERROR0 = 0x23560000, // 0x23560000, 
  d_airblow5_STATE_FL_ERROR1,              // 0x23560001, 
  d_airblow5_STATE_Step_AirBlow,           // 0x23560002, 
  d_airblow5_STATE_Step_SEQ                // 0x23560003, 
};

enum db_airblow6_PO{
  d_airblow6_PO_O_AIR_BLT = 0x23600000, // 0x23600000, 吹气6
  d_airblow6_PO_O_AIR_SRV,              // 0x23600001, 吹气6备用
  d_airblow6_PO_O_AIR_SEQVLV1,          // 0x23600002, 顺序阀16
  d_airblow6_PO_O_AIR_SEQVLV2,          // 0x23600003, 顺序阀17
  d_airblow6_PO_O_AIR_SEQVLV3,          // 0x23600004, 顺序阀18
  d_airblow6_PO_O_AIR_JKF1,             // 0x23600005, 胶口针阀6-1
  d_airblow6_PO_O_AIR_JKF2,             // 0x23600006, 胶口针阀6-2
  d_airblow6_PO_O_AIR_BLT2              // 0x23600007, 吹气20
};

enum db_airblow6_MOLDSET{
  d_airblow6_MOLDSET_FL_BLAST = 0x23630000, // 0x23630000, 吹气6方式
  d_airblow6_MOLDSET_ADPOSI_MOVINGBLASTSTART, // 0x23630001, 吹气6动作位置
  d_airblow6_MOLDSET_TM_MOVINGPLATENDLY,    // 0x23630002, 吹气6延迟计时
  d_airblow6_MOLDSET_TM_MOVINGPLATENACT,    // 0x23630003, 吹气6动作时间
  d_airblow6_MOLDSET_TM_AIRDLY,             // 0x23630004, 吹气6间歇时间
  d_airblow6_MOLDSET_TM_AIRALL,             // 0x23630005, 吹气6总时间
  d_airblow6_MOLDSET_FL_SEQ_USE,            // 0x23630006, 顺序阀16功能
  d_airblow6_MOLDSET_TM_SEQ_DELAY,          // 0x23630007, 顺序阀16延时
  d_airblow6_MOLDSET_TM_SEQ_MOVING,         // 0x23630008, 顺序阀16动作时间
  d_airblow6_MOLDSET_ADPOSI_SEQSTART,       // 0x23630009, 顺序阀16动作起始位置
  d_airblow6_MOLDSET_ADPOSI_SEQEND,         // 0x2363000a, 顺序阀16动作结束位置
  d_airblow6_MOLDSET_FL_SEQ_USE2,           // 0x2363000b, 顺序阀17功能
  d_airblow6_MOLDSET_TM_SEQ_DELAY2,         // 0x2363000c, 顺序阀17延时
  d_airblow6_MOLDSET_TM_SEQ_MOVING2,        // 0x2363000d, 顺序阀17动作时间
  d_airblow6_MOLDSET_ADPOSI_SEQSTART2,      // 0x2363000e, 顺序阀17动作起始位置
  d_airblow6_MOLDSET_ADPOSI_SEQEND2,        // 0x2363000f, 顺序阀17动作结束位置
  d_airblow6_MOLDSET_FL_SEQ_USE3,           // 0x23630010, 顺序阀18功能
  d_airblow6_MOLDSET_TM_SEQ_DELAY3,         // 0x23630011, 顺序阀18延时
  d_airblow6_MOLDSET_TM_SEQ_MOVING3,        // 0x23630012, 顺序阀18动作时间
  d_airblow6_MOLDSET_ADPOSI_SEQSTART3,      // 0x23630013, 顺序阀18动作起始位置
  d_airblow6_MOLDSET_ADPOSI_SEQEND3,        // 0x23630014, 顺序阀18动作结束位置
  d_airblow6_MOLDSET_FL_BLAST2,             // 0x23630015, 吹气20方式
  d_airblow6_MOLDSET_ADPOSI_MOVINGBLASTSTART2, // 0x23630016, 吹气20动作位置
  d_airblow6_MOLDSET_TM_MOVINGPLATENDLY2,   // 0x23630017, 吹气20延迟计时
  d_airblow6_MOLDSET_TM_MOVINGPLATENACT2,   // 0x23630018, 吹气20动作时间
  d_airblow6_MOLDSET_TM_AIRDLY2,            // 0x23630019, 吹气20间歇时间
  d_airblow6_MOLDSET_TM_AIRALL2,            // 0x2363001a, 吹气20总时间
  d_airblow6_MOLDSET_FL_SEQ_HOLD_USE16,     // 0x2363001b, 顺序阀16保压使用
  d_airblow6_MOLDSET_TM_SEQ_HOLD_DELAY16,   // 0x2363001c, 顺序阀16保压延迟计时
  d_airblow6_MOLDSET_TM_SEQ_HOLD_MOVING16,  // 0x2363001d, 顺序阀16保压动作时间
  d_airblow6_MOLDSET_FL_SEQ_HOLD_USE17,     // 0x2363001e, 顺序阀17保压使用
  d_airblow6_MOLDSET_TM_SEQ_HOLD_DELAY17,   // 0x2363001f, 顺序阀17保压延迟计时
  d_airblow6_MOLDSET_TM_SEQ_HOLD_MOVING17,  // 0x23630020, 顺序阀17保压动作时间
  d_airblow6_MOLDSET_FL_SEQ_HOLD_USE18,     // 0x23630021, 顺序阀18保压使用
  d_airblow6_MOLDSET_TM_SEQ_HOLD_DELAY18,   // 0x23630022, 顺序阀18保压延迟计时
  d_airblow6_MOLDSET_TM_SEQ_HOLD_MOVING18   // 0x23630023, 顺序阀18保压动作时间
};

enum db_airblow6_STATE{
  d_airblow6_STATE_FL_ERROR0 = 0x23660000, // 0x23660000, 
  d_airblow6_STATE_FL_ERROR1,              // 0x23660001, 
  d_airblow6_STATE_Step_AirBlow,           // 0x23660002, 
  d_airblow6_STATE_Step_SEQ                // 0x23660003, 
};

enum db_airblow7_PO{
  d_airblow7_PO_O_AIR_BLT = 0x23700000, // 0x23700000, 吹气7
  d_airblow7_PO_O_AIR_SRV,              // 0x23700001, 吹气7备用
  d_airblow7_PO_O_AIR_SEQVLV1,          // 0x23700002, 顺序阀19
  d_airblow7_PO_O_AIR_SEQVLV2,          // 0x23700003, 顺序阀20
  d_airblow7_PO_O_AIR_SEQVLV3,          // 0x23700004, 顺序阀21
  d_airblow7_PO_O_AIR_JKF1,             // 0x23700005, 胶口针阀7-1
  d_airblow7_PO_O_AIR_JKF2,             // 0x23700006, 胶口针阀7-2
  d_airblow7_PO_O_AIR_BLT2              // 0x23700007, 吹气21
};

enum db_airblow7_MOLDSET{
  d_airblow7_MOLDSET_FL_BLAST = 0x23730000, // 0x23730000, 吹气7方式
  d_airblow7_MOLDSET_ADPOSI_MOVINGBLASTSTART, // 0x23730001, 吹气7动作位置
  d_airblow7_MOLDSET_TM_MOVINGPLATENDLY,    // 0x23730002, 吹气7延迟计时
  d_airblow7_MOLDSET_TM_MOVINGPLATENACT,    // 0x23730003, 吹气7动作时间
  d_airblow7_MOLDSET_TM_AIRDLY,             // 0x23730004, 吹气7间歇时间
  d_airblow7_MOLDSET_TM_AIRALL,             // 0x23730005, 吹气7总时间
  d_airblow7_MOLDSET_FL_SEQ_USE,            // 0x23730006, 顺序阀19功能
  d_airblow7_MOLDSET_TM_SEQ_DELAY,          // 0x23730007, 顺序阀19延时
  d_airblow7_MOLDSET_TM_SEQ_MOVING,         // 0x23730008, 顺序阀19动作时间
  d_airblow7_MOLDSET_ADPOSI_SEQSTART,       // 0x23730009, 顺序阀19动作起始位置
  d_airblow7_MOLDSET_ADPOSI_SEQEND,         // 0x2373000a, 顺序阀19动作结束位置
  d_airblow7_MOLDSET_FL_SEQ_USE2,           // 0x2373000b, 顺序阀20功能
  d_airblow7_MOLDSET_TM_SEQ_DELAY2,         // 0x2373000c, 顺序阀20延时
  d_airblow7_MOLDSET_TM_SEQ_MOVING2,        // 0x2373000d, 顺序阀20动作时间
  d_airblow7_MOLDSET_ADPOSI_SEQSTART2,      // 0x2373000e, 顺序阀20动作起始位置
  d_airblow7_MOLDSET_ADPOSI_SEQEND2,        // 0x2373000f, 顺序阀20动作结束位置
  d_airblow7_MOLDSET_FL_SEQ_USE3,           // 0x23730010, 顺序阀21功能
  d_airblow7_MOLDSET_TM_SEQ_DELAY3,         // 0x23730011, 顺序阀21延时
  d_airblow7_MOLDSET_TM_SEQ_MOVING3,        // 0x23730012, 顺序阀21动作时间
  d_airblow7_MOLDSET_ADPOSI_SEQSTART3,      // 0x23730013, 顺序阀21动作起始位置
  d_airblow7_MOLDSET_ADPOSI_SEQEND3,        // 0x23730014, 顺序阀21动作结束位置
  d_airblow7_MOLDSET_FL_BLAST2,             // 0x23730015, 吹气21方式
  d_airblow7_MOLDSET_ADPOSI_MOVINGBLASTSTART2, // 0x23730016, 吹气21动作位置
  d_airblow7_MOLDSET_TM_MOVINGPLATENDLY2,   // 0x23730017, 吹气21延迟计时
  d_airblow7_MOLDSET_TM_MOVINGPLATENACT2,   // 0x23730018, 吹气21动作时间
  d_airblow7_MOLDSET_TM_AIRDLY2,            // 0x23730019, 吹气21间歇时间
  d_airblow7_MOLDSET_TM_AIRALL2,            // 0x2373001a, 吹气21总时间
  d_airblow7_MOLDSET_FL_SEQ_HOLD_USE19,     // 0x2373001b, 顺序阀19保压使用
  d_airblow7_MOLDSET_TM_SEQ_HOLD_DELAY19,   // 0x2373001c, 顺序阀19保压延迟计时
  d_airblow7_MOLDSET_TM_SEQ_HOLD_MOVING19,  // 0x2373001d, 顺序阀19保压动作时间
  d_airblow7_MOLDSET_FL_SEQ_HOLD_USE20,     // 0x2373001e, 顺序阀20保压使用
  d_airblow7_MOLDSET_TM_SEQ_HOLD_DELAY20,   // 0x2373001f, 顺序阀20保压延迟计时
  d_airblow7_MOLDSET_TM_SEQ_HOLD_MOVING20,  // 0x23730020, 顺序阀20保压动作时间
  d_airblow7_MOLDSET_FL_SEQ_HOLD_USE21,     // 0x23730021, 顺序阀21保压使用
  d_airblow7_MOLDSET_TM_SEQ_HOLD_DELAY21,   // 0x23730022, 顺序阀21保压延迟计时
  d_airblow7_MOLDSET_TM_SEQ_HOLD_MOVING21   // 0x23730023, 顺序阀21保压动作时间
};

enum db_airblow7_STATE{
  d_airblow7_STATE_FL_ERROR0 = 0x23760000, // 0x23760000, 
  d_airblow7_STATE_FL_ERROR1,              // 0x23760001, 
  d_airblow7_STATE_Step_AirBlow,           // 0x23760002, 
  d_airblow7_STATE_Step_SEQ                // 0x23760003, 
};

enum db_airblow8_PO{
  d_airblow8_PO_O_AIR_BLT = 0x23800000, // 0x23800000, 吹气8
  d_airblow8_PO_O_AIR_SRV,              // 0x23800001, 吹气8备用
  d_airblow8_PO_O_AIR_SEQVLV1,          // 0x23800002, 顺序阀22
  d_airblow8_PO_O_AIR_SEQVLV2,          // 0x23800003, 顺序阀23
  d_airblow8_PO_O_AIR_SEQVLV3,          // 0x23800004, 顺序阀24
  d_airblow8_PO_O_AIR_JKF1,             // 0x23800005, 胶口针阀8-1
  d_airblow8_PO_O_AIR_JKF2,             // 0x23800006, 胶口针阀8-2
  d_airblow8_PO_O_AIR_BLT2              // 0x23800007, 吹气22
};

enum db_airblow8_MOLDSET{
  d_airblow8_MOLDSET_FL_BLAST = 0x23830000, // 0x23830000, 吹气8方式
  d_airblow8_MOLDSET_ADPOSI_MOVINGBLASTSTART, // 0x23830001, 吹气8动作位置
  d_airblow8_MOLDSET_TM_MOVINGPLATENDLY,    // 0x23830002, 吹气8延迟计时
  d_airblow8_MOLDSET_TM_MOVINGPLATENACT,    // 0x23830003, 吹气8动作时间
  d_airblow8_MOLDSET_TM_AIRDLY,             // 0x23830004, 吹气8间歇时间
  d_airblow8_MOLDSET_TM_AIRALL,             // 0x23830005, 吹气8总时间
  d_airblow8_MOLDSET_FL_SEQ_USE,            // 0x23830006, 顺序阀22功能
  d_airblow8_MOLDSET_TM_SEQ_DELAY,          // 0x23830007, 顺序阀22延时
  d_airblow8_MOLDSET_TM_SEQ_MOVING,         // 0x23830008, 顺序阀22动作时间
  d_airblow8_MOLDSET_ADPOSI_SEQSTART,       // 0x23830009, 顺序阀22动作起始位置
  d_airblow8_MOLDSET_ADPOSI_SEQEND,         // 0x2383000a, 顺序阀22动作结束位置
  d_airblow8_MOLDSET_FL_SEQ_USE2,           // 0x2383000b, 顺序阀23功能
  d_airblow8_MOLDSET_TM_SEQ_DELAY2,         // 0x2383000c, 顺序阀23延时
  d_airblow8_MOLDSET_TM_SEQ_MOVING2,        // 0x2383000d, 顺序阀23动作时间
  d_airblow8_MOLDSET_ADPOSI_SEQSTART2,      // 0x2383000e, 顺序阀23动作起始位置
  d_airblow8_MOLDSET_ADPOSI_SEQEND2,        // 0x2383000f, 顺序阀23动作结束位置
  d_airblow8_MOLDSET_FL_SEQ_USE3,           // 0x23830010, 顺序阀24功能
  d_airblow8_MOLDSET_TM_SEQ_DELAY3,         // 0x23830011, 顺序阀24延时
  d_airblow8_MOLDSET_TM_SEQ_MOVING3,        // 0x23830012, 顺序阀24动作时间
  d_airblow8_MOLDSET_ADPOSI_SEQSTART3,      // 0x23830013, 顺序阀24动作起始位置
  d_airblow8_MOLDSET_ADPOSI_SEQEND3,        // 0x23830014, 顺序阀24动作结束位置
  d_airblow8_MOLDSET_FL_BLAST2,             // 0x23830015, 吹气22方式
  d_airblow8_MOLDSET_ADPOSI_MOVINGBLASTSTART2, // 0x23830016, 吹气22动作位置
  d_airblow8_MOLDSET_TM_MOVINGPLATENDLY2,   // 0x23830017, 吹气22延迟计时
  d_airblow8_MOLDSET_TM_MOVINGPLATENACT2,   // 0x23830018, 吹气22动作时间
  d_airblow8_MOLDSET_TM_AIRDLY2,            // 0x23830019, 吹气22间歇时间
  d_airblow8_MOLDSET_TM_AIRALL2,            // 0x2383001a, 吹气22总时间
  d_airblow8_MOLDSET_FL_SEQ_HOLD_USE22,     // 0x2383001b, 顺序阀22保压使用
  d_airblow8_MOLDSET_TM_SEQ_HOLD_DELAY22,   // 0x2383001c, 顺序阀22保压延迟计时
  d_airblow8_MOLDSET_TM_SEQ_HOLD_MOVING22,  // 0x2383001d, 顺序阀22保压动作时间
  d_airblow8_MOLDSET_FL_SEQ_HOLD_USE23,     // 0x2383001e, 顺序阀23保压使用
  d_airblow8_MOLDSET_TM_SEQ_HOLD_DELAY23,   // 0x2383001f, 顺序阀23保压延迟计时
  d_airblow8_MOLDSET_TM_SEQ_HOLD_MOVING23,  // 0x23830020, 顺序阀23保压动作时间
  d_airblow8_MOLDSET_FL_SEQ_HOLD_USE24,     // 0x23830021, 顺序阀24保压使用
  d_airblow8_MOLDSET_TM_SEQ_HOLD_DELAY24,   // 0x23830022, 顺序阀24保压延迟计时
  d_airblow8_MOLDSET_TM_SEQ_HOLD_MOVING24   // 0x23830023, 顺序阀24保压动作时间
};

enum db_airblow8_STATE{
  d_airblow8_STATE_FL_ERROR0 = 0x23860000, // 0x23860000, 
  d_airblow8_STATE_FL_ERROR1,              // 0x23860001, 
  d_airblow8_STATE_Step_AirBlow,           // 0x23860002, 
  d_airblow8_STATE_Step_SEQ                // 0x23860003, 
};

enum db_airblow9_PO{
  d_airblow9_PO_O_AIR_BLT = 0x23900000, // 0x23900000, 吹气9
  d_airblow9_PO_O_AIR_SRV,              // 0x23900001, 吹气9备用
  d_airblow9_PO_O_AIR_SEQVLV1,          // 0x23900002, 顺序阀25
  d_airblow9_PO_O_AIR_SEQVLV2,          // 0x23900003, 顺序阀26
  d_airblow9_PO_O_AIR_SEQVLV3,          // 0x23900004, 顺序阀27
  d_airblow9_PO_O_AIR_JKF1,             // 0x23900005, 胶口针阀9-1
  d_airblow9_PO_O_AIR_JKF2,             // 0x23900006, 胶口针阀9-2
  d_airblow9_PO_O_AIR_BLT2              // 0x23900007, 吹气23
};

enum db_airblow9_MOLDSET{
  d_airblow9_MOLDSET_FL_BLAST = 0x23930000, // 0x23930000, 吹气9方式
  d_airblow9_MOLDSET_ADPOSI_MOVINGBLASTSTART, // 0x23930001, 吹气9动作位置
  d_airblow9_MOLDSET_TM_MOVINGPLATENDLY,    // 0x23930002, 吹气9延迟计时
  d_airblow9_MOLDSET_TM_MOVINGPLATENACT,    // 0x23930003, 吹气9动作时间
  d_airblow9_MOLDSET_TM_AIRDLY,             // 0x23930004, 吹气9间歇时间
  d_airblow9_MOLDSET_TM_AIRALL,             // 0x23930005, 吹气9总时间
  d_airblow9_MOLDSET_FL_SEQ_USE,            // 0x23930006, 顺序阀25功能
  d_airblow9_MOLDSET_TM_SEQ_DELAY,          // 0x23930007, 顺序阀25延时
  d_airblow9_MOLDSET_TM_SEQ_MOVING,         // 0x23930008, 顺序阀25动作时间
  d_airblow9_MOLDSET_ADPOSI_SEQSTART,       // 0x23930009, 顺序阀25动作起始位置
  d_airblow9_MOLDSET_ADPOSI_SEQEND,         // 0x2393000a, 顺序阀25动作结束位置
  d_airblow9_MOLDSET_FL_SEQ_USE2,           // 0x2393000b, 顺序阀26功能
  d_airblow9_MOLDSET_TM_SEQ_DELAY2,         // 0x2393000c, 顺序阀26延时
  d_airblow9_MOLDSET_TM_SEQ_MOVING2,        // 0x2393000d, 顺序阀26动作时间
  d_airblow9_MOLDSET_ADPOSI_SEQSTART2,      // 0x2393000e, 顺序阀26动作起始位置
  d_airblow9_MOLDSET_ADPOSI_SEQEND2,        // 0x2393000f, 顺序阀26动作结束位置
  d_airblow9_MOLDSET_FL_SEQ_USE3,           // 0x23930010, 顺序阀27功能
  d_airblow9_MOLDSET_TM_SEQ_DELAY3,         // 0x23930011, 顺序阀27延时
  d_airblow9_MOLDSET_TM_SEQ_MOVING3,        // 0x23930012, 顺序阀27动作时间
  d_airblow9_MOLDSET_ADPOSI_SEQSTART3,      // 0x23930013, 顺序阀27动作起始位置
  d_airblow9_MOLDSET_ADPOSI_SEQEND3,        // 0x23930014, 顺序阀27动作结束位置
  d_airblow9_MOLDSET_FL_BLAST2,             // 0x23930015, 吹气23方式
  d_airblow9_MOLDSET_ADPOSI_MOVINGBLASTSTART2, // 0x23930016, 吹气23动作位置
  d_airblow9_MOLDSET_TM_MOVINGPLATENDLY2,   // 0x23930017, 吹气23延迟计时
  d_airblow9_MOLDSET_TM_MOVINGPLATENACT2,   // 0x23930018, 吹气23动作时间
  d_airblow9_MOLDSET_TM_AIRDLY2,            // 0x23930019, 吹气23间歇时间
  d_airblow9_MOLDSET_TM_AIRALL2,            // 0x2393001a, 吹气23总时间
  d_airblow9_MOLDSET_FL_SEQ_HOLD_USE25,     // 0x2393001b, 顺序阀25保压使用
  d_airblow9_MOLDSET_TM_SEQ_HOLD_DELAY25,   // 0x2393001c, 顺序阀25保压延迟计时
  d_airblow9_MOLDSET_TM_SEQ_HOLD_MOVING25,  // 0x2393001d, 顺序阀25保压动作时间
  d_airblow9_MOLDSET_FL_SEQ_HOLD_USE26,     // 0x2393001e, 顺序阀26保压使用
  d_airblow9_MOLDSET_TM_SEQ_HOLD_DELAY26,   // 0x2393001f, 顺序阀26保压延迟计时
  d_airblow9_MOLDSET_TM_SEQ_HOLD_MOVING26,  // 0x23930020, 顺序阀26保压动作时间
  d_airblow9_MOLDSET_FL_SEQ_HOLD_USE27,     // 0x23930021, 顺序阀27保压使用
  d_airblow9_MOLDSET_TM_SEQ_HOLD_DELAY27,   // 0x23930022, 顺序阀27保压延迟计时
  d_airblow9_MOLDSET_TM_SEQ_HOLD_MOVING27   // 0x23930023, 顺序阀27保压动作时间
};

enum db_airblow9_STATE{
  d_airblow9_STATE_FL_ERROR0 = 0x23960000, // 0x23960000, 
  d_airblow9_STATE_FL_ERROR1,              // 0x23960001, 
  d_airblow9_STATE_Step_AirBlow,           // 0x23960002, 
  d_airblow9_STATE_Step_SEQ                // 0x23960003, 
};

enum db_airblow10_PO{
  d_airblow10_PO_O_AIR_BLT = 0x23a00000, // 0x23a00000, 吹气10
  d_airblow10_PO_O_AIR_SRV,              // 0x23a00001, 吹气10备用
  d_airblow10_PO_O_AIR_SEQVLV1,          // 0x23a00002, 顺序阀28
  d_airblow10_PO_O_AIR_SEQVLV2,          // 0x23a00003, 顺序阀29
  d_airblow10_PO_O_AIR_SEQVLV3,          // 0x23a00004, 顺序阀30
  d_airblow10_PO_O_AIR_JKF1,             // 0x23a00005, 胶口针阀10-1
  d_airblow10_PO_O_AIR_JKF2,             // 0x23a00006, 胶口针阀10-2
  d_airblow10_PO_O_AIR_BLT2              // 0x23a00007, 吹气24
};

enum db_airblow10_MOLDSET{
  d_airblow10_MOLDSET_FL_BLAST = 0x23a30000, // 0x23a30000, 吹气10方式
  d_airblow10_MOLDSET_ADPOSI_MOVINGBLASTSTART, // 0x23a30001, 吹气10动作位置
  d_airblow10_MOLDSET_TM_MOVINGPLATENDLY,    // 0x23a30002, 吹气10延迟计时
  d_airblow10_MOLDSET_TM_MOVINGPLATENACT,    // 0x23a30003, 吹气10动作时间
  d_airblow10_MOLDSET_TM_AIRDLY,             // 0x23a30004, 吹气10间歇时间
  d_airblow10_MOLDSET_TM_AIRALL,             // 0x23a30005, 吹气10总时间
  d_airblow10_MOLDSET_FL_SEQ_USE,            // 0x23a30006, 顺序阀28功能
  d_airblow10_MOLDSET_TM_SEQ_DELAY,          // 0x23a30007, 顺序阀28延时
  d_airblow10_MOLDSET_TM_SEQ_MOVING,         // 0x23a30008, 顺序阀28动作时间
  d_airblow10_MOLDSET_ADPOSI_SEQSTART,       // 0x23a30009, 顺序阀28动作起始位置
  d_airblow10_MOLDSET_ADPOSI_SEQEND,         // 0x23a3000a, 顺序阀28动作结束位置
  d_airblow10_MOLDSET_FL_SEQ_USE2,           // 0x23a3000b, 顺序阀29功能
  d_airblow10_MOLDSET_TM_SEQ_DELAY2,         // 0x23a3000c, 顺序阀29延时
  d_airblow10_MOLDSET_TM_SEQ_MOVING2,        // 0x23a3000d, 顺序阀29动作时间
  d_airblow10_MOLDSET_ADPOSI_SEQSTART2,      // 0x23a3000e, 顺序阀29动作起始位置
  d_airblow10_MOLDSET_ADPOSI_SEQEND2,        // 0x23a3000f, 顺序阀29动作结束位置
  d_airblow10_MOLDSET_FL_SEQ_USE3,           // 0x23a30010, 顺序阀30功能
  d_airblow10_MOLDSET_TM_SEQ_DELAY3,         // 0x23a30011, 顺序阀30延时
  d_airblow10_MOLDSET_TM_SEQ_MOVING3,        // 0x23a30012, 顺序阀30动作时间
  d_airblow10_MOLDSET_ADPOSI_SEQSTART3,      // 0x23a30013, 顺序阀30动作起始位置
  d_airblow10_MOLDSET_ADPOSI_SEQEND3,        // 0x23a30014, 顺序阀30动作结束位置
  d_airblow10_MOLDSET_FL_BLAST2,             // 0x23a30015, 吹气24方式
  d_airblow10_MOLDSET_ADPOSI_MOVINGBLASTSTART2, // 0x23a30016, 吹气24动作位置
  d_airblow10_MOLDSET_TM_MOVINGPLATENDLY2,   // 0x23a30017, 吹气24延迟计时
  d_airblow10_MOLDSET_TM_MOVINGPLATENACT2,   // 0x23a30018, 吹气24动作时间
  d_airblow10_MOLDSET_TM_AIRDLY2,            // 0x23a30019, 吹气24间歇时间
  d_airblow10_MOLDSET_TM_AIRALL2,            // 0x23a3001a, 吹气24总时间
  d_airblow10_MOLDSET_FL_SEQ_HOLD_USE28,     // 0x23a3001b, 顺序阀28保压使用
  d_airblow10_MOLDSET_TM_SEQ_HOLD_DELAY28,   // 0x23a3001c, 顺序阀28保压延迟计时
  d_airblow10_MOLDSET_TM_SEQ_HOLD_MOVING28,  // 0x23a3001d, 顺序阀28保压动作时间
  d_airblow10_MOLDSET_FL_SEQ_HOLD_USE29,     // 0x23a3001e, 顺序阀29保压使用
  d_airblow10_MOLDSET_TM_SEQ_HOLD_DELAY29,   // 0x23a3001f, 顺序阀29保压延迟计时
  d_airblow10_MOLDSET_TM_SEQ_HOLD_MOVING29,  // 0x23a30020, 顺序阀29保压动作时间
  d_airblow10_MOLDSET_FL_SEQ_HOLD_USE30,     // 0x23a30021, 顺序阀30保压使用
  d_airblow10_MOLDSET_TM_SEQ_HOLD_DELAY30,   // 0x23a30022, 顺序阀30保压延迟计时
  d_airblow10_MOLDSET_TM_SEQ_HOLD_MOVING30   // 0x23a30023, 顺序阀30保压动作时间
};

enum db_airblow10_STATE{
  d_airblow10_STATE_FL_ERROR0 = 0x23a60000, // 0x23a60000, 
  d_airblow10_STATE_FL_ERROR1,              // 0x23a60001, 
  d_airblow10_STATE_Step_AirBlow,           // 0x23a60002, 
  d_airblow10_STATE_Step_SEQ                // 0x23a60003, 
};

enum db_airblow11_PO{
  d_airblow11_PO_O_AIR_BLT = 0x23b00000, // 0x23b00000, 吹气11
  d_airblow11_PO_O_AIR_SRV,              // 0x23b00001, 吹气1备用
  d_airblow11_PO_O_AIR_SEQVLV1,          // 0x23b00002, 顺序阀31
  d_airblow11_PO_O_AIR_SEQVLV2,          // 0x23b00003, 顺序阀32
  d_airblow11_PO_O_AIR_SEQVLV3,          // 0x23b00004, 顺序阀33
  d_airblow11_PO_O_AIR_JKF1,             // 0x23b00005, 胶口针阀11-1
  d_airblow11_PO_O_AIR_JKF2,             // 0x23b00006, 胶口针阀11-2
  d_airblow11_PO_O_AIR_BLT2              // 0x23b00007, 吹气25
};

enum db_airblow11_MOLDSET{
  d_airblow11_MOLDSET_FL_BLAST = 0x23b30000, // 0x23b30000, 吹气11方式
  d_airblow11_MOLDSET_ADPOSI_MOVINGBLASTSTART, // 0x23b30001, 吹气11动作位置
  d_airblow11_MOLDSET_TM_MOVINGPLATENDLY,    // 0x23b30002, 吹气11延迟计时
  d_airblow11_MOLDSET_TM_MOVINGPLATENACT,    // 0x23b30003, 吹气11动作时间
  d_airblow11_MOLDSET_TM_AIRDLY,             // 0x23b30004, 吹气11间歇时间
  d_airblow11_MOLDSET_TM_AIRALL,             // 0x23b30005, 吹气11总时间
  d_airblow11_MOLDSET_FL_SEQ_USE,            // 0x23b30006, 顺序阀31功能
  d_airblow11_MOLDSET_TM_SEQ_DELAY,          // 0x23b30007, 顺序阀31延时
  d_airblow11_MOLDSET_TM_SEQ_MOVING,         // 0x23b30008, 顺序阀31动作时间
  d_airblow11_MOLDSET_ADPOSI_SEQSTART,       // 0x23b30009, 顺序阀31动作起始位置
  d_airblow11_MOLDSET_ADPOSI_SEQEND,         // 0x23b3000a, 顺序阀31动作结束位置
  d_airblow11_MOLDSET_FL_SEQ_USE2,           // 0x23b3000b, 顺序阀32功能
  d_airblow11_MOLDSET_TM_SEQ_DELAY2,         // 0x23b3000c, 顺序阀32延时
  d_airblow11_MOLDSET_TM_SEQ_MOVING2,        // 0x23b3000d, 顺序阀32动作时间
  d_airblow11_MOLDSET_ADPOSI_SEQSTART2,      // 0x23b3000e, 顺序阀32动作起始位置
  d_airblow11_MOLDSET_ADPOSI_SEQEND2,        // 0x23b3000f, 顺序阀32动作结束位置
  d_airblow11_MOLDSET_FL_SEQ_USE3,           // 0x23b30010, 顺序阀33功能
  d_airblow11_MOLDSET_TM_SEQ_DELAY3,         // 0x23b30011, 顺序阀33延时
  d_airblow11_MOLDSET_TM_SEQ_MOVING3,        // 0x23b30012, 顺序阀33动作时间
  d_airblow11_MOLDSET_ADPOSI_SEQSTART3,      // 0x23b30013, 顺序阀33动作起始位置
  d_airblow11_MOLDSET_ADPOSI_SEQEND3,        // 0x23b30014, 顺序阀33动作结束位置
  d_airblow11_MOLDSET_FL_BLAST2,             // 0x23b30015, 吹气25方式
  d_airblow11_MOLDSET_ADPOSI_MOVINGBLASTSTART2, // 0x23b30016, 吹气25动作位置
  d_airblow11_MOLDSET_TM_MOVINGPLATENDLY2,   // 0x23b30017, 吹气25延迟计时
  d_airblow11_MOLDSET_TM_MOVINGPLATENACT2,   // 0x23b30018, 吹气25动作时间
  d_airblow11_MOLDSET_TM_AIRDLY2,            // 0x23b30019, 吹气25间歇时间
  d_airblow11_MOLDSET_TM_AIRALL2,            // 0x23b3001a, 吹气25总时间
  d_airblow11_MOLDSET_FL_SEQ_HOLD_USE31,     // 0x23b3001b, 顺序阀31保压使用
  d_airblow11_MOLDSET_TM_SEQ_HOLD_DELAY31,   // 0x23b3001c, 顺序阀31保压延迟计时
  d_airblow11_MOLDSET_TM_SEQ_HOLD_MOVING31,  // 0x23b3001d, 顺序阀31保压动作时间
  d_airblow11_MOLDSET_FL_SEQ_HOLD_USE32,     // 0x23b3001e, 顺序阀32保压使用
  d_airblow11_MOLDSET_TM_SEQ_HOLD_DELAY32,   // 0x23b3001f, 顺序阀32保压延迟计时
  d_airblow11_MOLDSET_TM_SEQ_HOLD_MOVING32,  // 0x23b30020, 顺序阀32保压动作时间
  d_airblow11_MOLDSET_FL_SEQ_HOLD_USE33,     // 0x23b30021, 顺序阀33保压使用
  d_airblow11_MOLDSET_TM_SEQ_HOLD_DELAY33,   // 0x23b30022, 顺序阀33保压延迟计时
  d_airblow11_MOLDSET_TM_SEQ_HOLD_MOVING33   // 0x23b30023, 顺序阀33保压动作时间
};

enum db_airblow11_STATE{
  d_airblow11_STATE_FL_ERROR0 = 0x23b60000, // 0x23b60000, 
  d_airblow11_STATE_FL_ERROR1,              // 0x23b60001, 
  d_airblow11_STATE_Step_AirBlow,           // 0x23b60002, 
  d_airblow11_STATE_Step_SEQ                // 0x23b60003, 
};

enum db_airblow12_PO{
  d_airblow12_PO_O_AIR_BLT = 0x23c00000, // 0x23c00000, 吹气12
  d_airblow12_PO_O_AIR_SRV,              // 0x23c00001, 吹气12备用
  d_airblow12_PO_O_AIR_SEQVLV1,          // 0x23c00002, 顺序阀34
  d_airblow12_PO_O_AIR_SEQVLV2,          // 0x23c00003, 顺序阀35
  d_airblow12_PO_O_AIR_SEQVLV3,          // 0x23c00004, 顺序阀36
  d_airblow12_PO_O_AIR_JKF1,             // 0x23c00005, 胶口针阀12-1
  d_airblow12_PO_O_AIR_JKF2,             // 0x23c00006, 胶口针阀12-2
  d_airblow12_PO_O_AIR_BLT2              // 0x23c00007, 吹气26
};

enum db_airblow12_MOLDSET{
  d_airblow12_MOLDSET_FL_BLAST = 0x23c30000, // 0x23c30000, 吹气12方式
  d_airblow12_MOLDSET_ADPOSI_MOVINGBLASTSTART, // 0x23c30001, 吹气12动作位置
  d_airblow12_MOLDSET_TM_MOVINGPLATENDLY,    // 0x23c30002, 吹气12延迟计时
  d_airblow12_MOLDSET_TM_MOVINGPLATENACT,    // 0x23c30003, 吹气12动作时间
  d_airblow12_MOLDSET_TM_AIRDLY,             // 0x23c30004, 吹气12间歇时间
  d_airblow12_MOLDSET_TM_AIRALL,             // 0x23c30005, 吹气12总时间
  d_airblow12_MOLDSET_FL_SEQ_USE,            // 0x23c30006, 顺序阀34功能
  d_airblow12_MOLDSET_TM_SEQ_DELAY,          // 0x23c30007, 顺序阀34延时
  d_airblow12_MOLDSET_TM_SEQ_MOVING,         // 0x23c30008, 顺序阀34动作时间
  d_airblow12_MOLDSET_ADPOSI_SEQSTART,       // 0x23c30009, 顺序阀34动作起始位置
  d_airblow12_MOLDSET_ADPOSI_SEQEND,         // 0x23c3000a, 顺序阀34动作结束位置
  d_airblow12_MOLDSET_FL_SEQ_USE2,           // 0x23c3000b, 顺序阀35功能
  d_airblow12_MOLDSET_TM_SEQ_DELAY2,         // 0x23c3000c, 顺序阀35延时
  d_airblow12_MOLDSET_TM_SEQ_MOVING2,        // 0x23c3000d, 顺序阀35动作时间
  d_airblow12_MOLDSET_ADPOSI_SEQSTART2,      // 0x23c3000e, 顺序阀35动作起始位置
  d_airblow12_MOLDSET_ADPOSI_SEQEND2,        // 0x23c3000f, 顺序阀35动作结束位置
  d_airblow12_MOLDSET_FL_SEQ_USE3,           // 0x23c30010, 顺序阀36功能
  d_airblow12_MOLDSET_TM_SEQ_DELAY3,         // 0x23c30011, 顺序阀36延时
  d_airblow12_MOLDSET_TM_SEQ_MOVING3,        // 0x23c30012, 顺序阀36动作时间
  d_airblow12_MOLDSET_ADPOSI_SEQSTART3,      // 0x23c30013, 顺序阀36动作起始位置
  d_airblow12_MOLDSET_ADPOSI_SEQEND3,        // 0x23c30014, 顺序阀36动作结束位置
  d_airblow12_MOLDSET_FL_BLAST2,             // 0x23c30015, 吹气26方式
  d_airblow12_MOLDSET_ADPOSI_MOVINGBLASTSTART2, // 0x23c30016, 吹气26动作位置
  d_airblow12_MOLDSET_TM_MOVINGPLATENDLY2,   // 0x23c30017, 吹气26延迟计时
  d_airblow12_MOLDSET_TM_MOVINGPLATENACT2,   // 0x23c30018, 吹气26动作时间
  d_airblow12_MOLDSET_TM_AIRDLY2,            // 0x23c30019, 吹气26间歇时间
  d_airblow12_MOLDSET_TM_AIRALL2,            // 0x23c3001a, 吹气26总时间
  d_airblow12_MOLDSET_FL_SEQ_HOLD_USE34,     // 0x23c3001b, 顺序阀34保压使用
  d_airblow12_MOLDSET_TM_SEQ_HOLD_DELAY34,   // 0x23c3001c, 顺序阀34保压延迟计时
  d_airblow12_MOLDSET_TM_SEQ_HOLD_MOVING34,  // 0x23c3001d, 顺序阀34保压动作时间
  d_airblow12_MOLDSET_FL_SEQ_HOLD_USE35,     // 0x23c3001e, 顺序阀35保压使用
  d_airblow12_MOLDSET_TM_SEQ_HOLD_DELAY35,   // 0x23c3001f, 顺序阀35保压延迟计时
  d_airblow12_MOLDSET_TM_SEQ_HOLD_MOVING35,  // 0x23c30020, 顺序阀35保压动作时间
  d_airblow12_MOLDSET_FL_SEQ_HOLD_USE36,     // 0x23c30021, 顺序阀36保压使用
  d_airblow12_MOLDSET_TM_SEQ_HOLD_DELAY36,   // 0x23c30022, 顺序阀36保压延迟计时
  d_airblow12_MOLDSET_TM_SEQ_HOLD_MOVING36   // 0x23c30023, 顺序阀36保压动作时间
};

enum db_airblow12_STATE{
  d_airblow12_STATE_FL_ERROR0 = 0x23c60000, // 0x23c60000, 
  d_airblow12_STATE_FL_ERROR1,              // 0x23c60001, 
  d_airblow12_STATE_Step_AirBlow,           // 0x23c60002, 
  d_airblow12_STATE_Step_SEQ                // 0x23c60003, 
};

enum db_airblow13_PO{
  d_airblow13_PO_O_AIR_BLT = 0x23d00000, // 0x23d00000, 吹气13
  d_airblow13_PO_O_AIR_SRV,              // 0x23d00001, 吹气13备用
  d_airblow13_PO_O_AIR_SEQVLV1,          // 0x23d00002, 顺序阀37
  d_airblow13_PO_O_AIR_SEQVLV2,          // 0x23d00003, 顺序阀38
  d_airblow13_PO_O_AIR_SEQVLV3,          // 0x23d00004, 顺序阀39
  d_airblow13_PO_O_AIR_JKF1,             // 0x23d00005, 胶口针阀13-1
  d_airblow13_PO_O_AIR_JKF2,             // 0x23d00006, 胶口针阀13-2
  d_airblow13_PO_O_AIR_BLT2              // 0x23d00007, 吹气27
};

enum db_airblow13_MOLDSET{
  d_airblow13_MOLDSET_FL_BLAST = 0x23d30000, // 0x23d30000, 吹气13方式
  d_airblow13_MOLDSET_ADPOSI_MOVINGBLASTSTART, // 0x23d30001, 吹气13动作位置
  d_airblow13_MOLDSET_TM_MOVINGPLATENDLY,    // 0x23d30002, 吹气13延迟计时
  d_airblow13_MOLDSET_TM_MOVINGPLATENACT,    // 0x23d30003, 吹气13动作时间
  d_airblow13_MOLDSET_TM_AIRDLY,             // 0x23d30004, 吹气13间歇时间
  d_airblow13_MOLDSET_TM_AIRALL,             // 0x23d30005, 吹气13总时间
  d_airblow13_MOLDSET_FL_SEQ_USE,            // 0x23d30006, 顺序阀37功能
  d_airblow13_MOLDSET_TM_SEQ_DELAY,          // 0x23d30007, 顺序阀37延时
  d_airblow13_MOLDSET_TM_SEQ_MOVING,         // 0x23d30008, 顺序阀37动作时间
  d_airblow13_MOLDSET_ADPOSI_SEQSTART,       // 0x23d30009, 顺序阀37动作起始位置
  d_airblow13_MOLDSET_ADPOSI_SEQEND,         // 0x23d3000a, 顺序阀37动作结束位置
  d_airblow13_MOLDSET_FL_SEQ_USE2,           // 0x23d3000b, 顺序阀38功能
  d_airblow13_MOLDSET_TM_SEQ_DELAY2,         // 0x23d3000c, 顺序阀38延时
  d_airblow13_MOLDSET_TM_SEQ_MOVING2,        // 0x23d3000d, 顺序阀38动作时间
  d_airblow13_MOLDSET_ADPOSI_SEQSTART2,      // 0x23d3000e, 顺序阀38动作起始位置
  d_airblow13_MOLDSET_ADPOSI_SEQEND2,        // 0x23d3000f, 顺序阀38动作结束位置
  d_airblow13_MOLDSET_FL_SEQ_USE3,           // 0x23d30010, 顺序阀39功能
  d_airblow13_MOLDSET_TM_SEQ_DELAY3,         // 0x23d30011, 顺序阀39延时
  d_airblow13_MOLDSET_TM_SEQ_MOVING3,        // 0x23d30012, 顺序阀39动作时间
  d_airblow13_MOLDSET_ADPOSI_SEQSTART3,      // 0x23d30013, 顺序阀39动作起始位置
  d_airblow13_MOLDSET_ADPOSI_SEQEND3,        // 0x23d30014, 顺序阀39动作结束位置
  d_airblow13_MOLDSET_FL_BLAST2,             // 0x23d30015, 吹气27方式
  d_airblow13_MOLDSET_ADPOSI_MOVINGBLASTSTART2, // 0x23d30016, 吹气27动作位置
  d_airblow13_MOLDSET_TM_MOVINGPLATENDLY2,   // 0x23d30017, 吹气27延迟计时
  d_airblow13_MOLDSET_TM_MOVINGPLATENACT2,   // 0x23d30018, 吹气27动作时间
  d_airblow13_MOLDSET_TM_AIRDLY2,            // 0x23d30019, 吹气27间歇时间
  d_airblow13_MOLDSET_TM_AIRALL2,            // 0x23d3001a, 吹气27总时间
  d_airblow13_MOLDSET_FL_SEQ_HOLD_USE37,     // 0x23d3001b, 顺序阀37保压使用
  d_airblow13_MOLDSET_TM_SEQ_HOLD_DELAY37,   // 0x23d3001c, 顺序阀37保压延迟计时
  d_airblow13_MOLDSET_TM_SEQ_HOLD_MOVING37,  // 0x23d3001d, 顺序阀37保压动作时间
  d_airblow13_MOLDSET_FL_SEQ_HOLD_USE38,     // 0x23d3001e, 顺序阀38保压使用
  d_airblow13_MOLDSET_TM_SEQ_HOLD_DELAY38,   // 0x23d3001f, 顺序阀38保压延迟计时
  d_airblow13_MOLDSET_TM_SEQ_HOLD_MOVING38,  // 0x23d30020, 顺序阀38保压动作时间
  d_airblow13_MOLDSET_FL_SEQ_HOLD_USE39,     // 0x23d30021, 顺序阀39保压使用
  d_airblow13_MOLDSET_TM_SEQ_HOLD_DELAY39,   // 0x23d30022, 顺序阀39保压延迟计时
  d_airblow13_MOLDSET_TM_SEQ_HOLD_MOVING39   // 0x23d30023, 顺序阀39保压动作时间
};

enum db_airblow13_STATE{
  d_airblow13_STATE_FL_ERROR0 = 0x23d60000, // 0x23d60000, 
  d_airblow13_STATE_FL_ERROR1,              // 0x23d60001, 
  d_airblow13_STATE_Step_AirBlow,           // 0x23d60002, 
  d_airblow13_STATE_Step_SEQ                // 0x23d60003, 
};

enum db_airblow14_PO{
  d_airblow14_PO_O_AIR_BLT = 0x23e00000, // 0x23e00000, 吹气14
  d_airblow14_PO_O_AIR_SRV,              // 0x23e00001, 吹气14备用
  d_airblow14_PO_O_AIR_SEQVLV1,          // 0x23e00002, 顺序阀40
  d_airblow14_PO_O_AIR_SEQVLV2,          // 0x23e00003, 顺序阀41
  d_airblow14_PO_O_AIR_SEQVLV3,          // 0x23e00004, 顺序阀42
  d_airblow14_PO_O_AIR_JKF1,             // 0x23e00005, 胶口针阀14-1
  d_airblow14_PO_O_AIR_JKF2,             // 0x23e00006, 胶口针阀14-2
  d_airblow14_PO_O_AIR_BLT2              // 0x23e00007, 吹气28
};

enum db_airblow14_MOLDSET{
  d_airblow14_MOLDSET_FL_BLAST = 0x23e30000, // 0x23e30000, 吹气14方式
  d_airblow14_MOLDSET_ADPOSI_MOVINGBLASTSTART, // 0x23e30001, 吹气14动作位置
  d_airblow14_MOLDSET_TM_MOVINGPLATENDLY,    // 0x23e30002, 吹气14延迟计时
  d_airblow14_MOLDSET_TM_MOVINGPLATENACT,    // 0x23e30003, 吹气14动作时间
  d_airblow14_MOLDSET_TM_AIRDLY,             // 0x23e30004, 吹气14间歇时间
  d_airblow14_MOLDSET_TM_AIRALL,             // 0x23e30005, 吹气14总时间
  d_airblow14_MOLDSET_FL_SEQ_USE,            // 0x23e30006, 顺序阀40功能
  d_airblow14_MOLDSET_TM_SEQ_DELAY,          // 0x23e30007, 顺序阀40延时
  d_airblow14_MOLDSET_TM_SEQ_MOVING,         // 0x23e30008, 顺序阀40动作时间
  d_airblow14_MOLDSET_ADPOSI_SEQSTART,       // 0x23e30009, 顺序阀40动作起始位置
  d_airblow14_MOLDSET_ADPOSI_SEQEND,         // 0x23e3000a, 顺序阀40动作结束位置
  d_airblow14_MOLDSET_FL_SEQ_USE2,           // 0x23e3000b, 顺序阀41功能
  d_airblow14_MOLDSET_TM_SEQ_DELAY2,         // 0x23e3000c, 顺序阀41延时
  d_airblow14_MOLDSET_TM_SEQ_MOVING2,        // 0x23e3000d, 顺序阀41动作时间
  d_airblow14_MOLDSET_ADPOSI_SEQSTART2,      // 0x23e3000e, 顺序阀41动作起始位置
  d_airblow14_MOLDSET_ADPOSI_SEQEND2,        // 0x23e3000f, 顺序阀41动作结束位置
  d_airblow14_MOLDSET_FL_SEQ_USE3,           // 0x23e30010, 顺序阀42功能
  d_airblow14_MOLDSET_TM_SEQ_DELAY3,         // 0x23e30011, 顺序阀42延时
  d_airblow14_MOLDSET_TM_SEQ_MOVING3,        // 0x23e30012, 顺序阀42动作时间
  d_airblow14_MOLDSET_ADPOSI_SEQSTART3,      // 0x23e30013, 顺序阀42动作起始位置
  d_airblow14_MOLDSET_ADPOSI_SEQEND3,        // 0x23e30014, 顺序阀42动作结束位置
  d_airblow14_MOLDSET_FL_BLAST2,             // 0x23e30015, 吹气28方式
  d_airblow14_MOLDSET_ADPOSI_MOVINGBLASTSTART2, // 0x23e30016, 吹气28动作位置
  d_airblow14_MOLDSET_TM_MOVINGPLATENDLY2,   // 0x23e30017, 吹气28延迟计时
  d_airblow14_MOLDSET_TM_MOVINGPLATENACT2,   // 0x23e30018, 吹气28动作时间
  d_airblow14_MOLDSET_TM_AIRDLY2,            // 0x23e30019, 吹气28间歇时间
  d_airblow14_MOLDSET_TM_AIRALL2,            // 0x23e3001a, 吹气28总时间
  d_airblow14_MOLDSET_FL_SEQ_HOLD_USE40,     // 0x23e3001b, 顺序阀40保压使用
  d_airblow14_MOLDSET_TM_SEQ_HOLD_DELAY40,   // 0x23e3001c, 顺序阀40保压延迟计时
  d_airblow14_MOLDSET_TM_SEQ_HOLD_MOVING40,  // 0x23e3001d, 顺序阀40保压动作时间
  d_airblow14_MOLDSET_FL_SEQ_HOLD_USE41,     // 0x23e3001e, 顺序阀41保压使用
  d_airblow14_MOLDSET_TM_SEQ_HOLD_DELAY41,   // 0x23e3001f, 顺序阀41保压延迟计时
  d_airblow14_MOLDSET_TM_SEQ_HOLD_MOVING41,  // 0x23e30020, 顺序阀41保压动作时间
  d_airblow14_MOLDSET_FL_SEQ_HOLD_USE42,     // 0x23e30021, 顺序阀42保压使用
  d_airblow14_MOLDSET_TM_SEQ_HOLD_DELAY42,   // 0x23e30022, 顺序阀42保压延迟计时
  d_airblow14_MOLDSET_TM_SEQ_HOLD_MOVING42   // 0x23e30023, 顺序阀42保压动作时间
};

enum db_airblow14_STATE{
  d_airblow14_STATE_FL_ERROR0 = 0x23e60000, // 0x23e60000, 
  d_airblow14_STATE_FL_ERROR1,              // 0x23e60001, 
  d_airblow14_STATE_Step_AirBlow,           // 0x23e60002, 
  d_airblow14_STATE_Step_SEQ                // 0x23e60003, 
};

enum db_adjust1_PO{
  d_adjust1_PO_O_ADJ_FWD = 0x24100000, // 0x24100000, 调模进
  d_adjust1_PO_O_ADJ_BSRV,             // 0x24100001, 调模进备用
  d_adjust1_PO_O_ADJ_BWD,              // 0x24100002, 调模退
  d_adjust1_PO_O_ADJ_BSRV2,            // 0x24100003, 调模退备用
  d_adjust1_PO_O_ADJ_LUB,              // 0x24100004, 调模润滑
  d_adjust1_PO_O_ADJ_USE               // 0x24100005, 调模使用
};

enum db_adjust1_PI{
  d_adjust1_PI_I_ADJ_FWD = 0x24110000, // 0x24110000, 调模进终
  d_adjust1_PI_I_ADJ_BWD,              // 0x24110001, 调模退终
  d_adjust1_PI_I_ADJ_CHK,              // 0x24110002, 调模电眼
  d_adjust1_PI_I_ADJ_LOK               // 0x24110003, 调模锁定
};

enum db_adjust1_MACHSET{
  d_adjust1_MACHSET_DAFLOW_RAMP = 0x24120000, // 0x24120000, 调模启动速度斜率
  d_adjust1_MACHSET_DAPRES_RAMP,              // 0x24120001, 调模启动压力斜率
  d_adjust1_MACHSET_DAFLOW_OFFRAMP,           // 0x24120002, 调模关闭速度斜率
  d_adjust1_MACHSET_DAPRES_OFFRAMP,           // 0x24120003, 调模关闭压力斜率
  d_adjust1_MACHSET_TM_AUTOADJCLSPROTECTACT,  // 0x24120004, 自调模低压计时
  d_adjust1_MACHSET_CNPOSI_ADJPROPERTIONAL,   // 0x24120005, 调模每齿移动距离
  d_adjust1_MACHSET_ADPOSI_ADJCUR,            // 0x24120006, 模板校正厚度
  d_adjust1_MACHSET_ADPOSI_ADJMIN,            // 0x24120007, 机器最小模厚
  d_adjust1_MACHSET_ADPOSI_ADJMAX,            // 0x24120008, 机器最大模厚
  d_adjust1_MACHSET_ADJF_HYDELAY,             // 0x24120009, 调模进液压延迟
  d_adjust1_MACHSET_ADJB_HYDELAY,             // 0x2412000a, 调模退液压延迟
  d_adjust1_MACHSET_ADPOSI_ADJTARG,           // 0x2412000b, 调模更换位置
  d_adjust1_MACHSET_FL_QDC,                   // 0x2412000c, QDC功能
  d_adjust1_MACHSET_FL_JOG_FUN,               // 0x2412000d, 调模调试功能
  d_adjust1_MACHSET_FL_ADJ_LUB                // 0x2412000e, 调模输出润滑
};

enum db_adjust1_ACTPARA{
  d_adjust1_ACTPARA_DAPRES_ADJFFAST = 0x24140000, // 0x24140000, 调模快速压力
  d_adjust1_ACTPARA_DAFLOW_ADJFFAST,              // 0x24140001, 调模快速速度
  d_adjust1_ACTPARA_DAPRES_ADJSLOW,               // 0x24140002, 调模慢速压力
  d_adjust1_ACTPARA_DAFLOW_ADJSLOW,               // 0x24140003, 调模慢速速度
  d_adjust1_ACTPARA_DAPRES_ADJBWD,                // 0x24140004, 调模退压力
  d_adjust1_ACTPARA_DAFLOW_ADJBWD,                // 0x24140005, 调模退速度
  d_adjust1_ACTPARA_TM_ADJUSTSLOW,                // 0x24140006, 松模单次调模计时
  d_adjust1_ACTPARA_CN_ADJADV,                    // 0x24140007, 调模每次点动步数
  d_adjust1_ACTPARA_CN_ADJFIRST,                  // 0x24140008, 调模每次点动计时
  d_adjust1_ACTPARA_ADPOSI_ADJ,                   // 0x24140009, 调模位置
  d_adjust1_ACTPARA_ADPOSI_ADJMD,                 // 0x2414000a, 粗调模模式
  d_adjust1_ACTPARA_DAPRES_QDC,                   // 0x2414000b, QDC压力
  d_adjust1_ACTPARA_DAFLOW_QDC,                   // 0x2414000c, QDC速度
  d_adjust1_ACTPARA_PER_DAFLOW_ADJUST             // 0x2414000d, 调模速度使用率
};

enum db_adjust1_HYDR{
  d_adjust1_HYDR_HYDR_PORT_ADJUST = 0x24150000, // 0x24150000, 
  d_adjust1_HYDR_HYDR_PUMPS_ADJUST,             // 0x24150001, 
  d_adjust1_HYDR_SERVO_KP_ADJUST,               // 0x24150002, 调模 P
  d_adjust1_HYDR_SERVO_KI_ADJUST,               // 0x24150003, 调模 I
  d_adjust1_HYDR_SERVO_KD_ADJUST                // 0x24150004, 调模 D
};

enum db_adjust1_STATE{
  d_adjust1_STATE_FL_ERROR0 = 0x24160000, // 0x24160000, 警报1
  d_adjust1_STATE_FL_ERROR1,              // 0x24160001, 警报2
  d_adjust1_STATE_ADPOSI_ADJREAL,         // 0x24160002, 调模中的实时位置
  d_adjust1_STATE_Step_ManualAdjFwd,      // 0x24160003, 手动调模进实时动作步
  d_adjust1_STATE_Step_ManualAdjBwd,      // 0x24160004, 手动调模退实时动作步
  d_adjust1_STATE_Step_AUTOAdj            // 0x24160005, 自动调模实时动作步
};

enum db_sysacc1_PO{
  d_sysacc1_PO_O_SYS_ACC = 0x25100000, // 0x25100000, 系统蓄压
  d_sysacc1_PO_O_SYS_FDG,              // 0x25100001, 浮动缸
  d_sysacc1_PO_O_SYS_CHN,              // 0x25100002, 浮动缸储能
  d_sysacc1_PO_O_SYS_XY                // 0x25100003, 浮动缸泄压
};

enum db_sysacc1_PI{
  d_sysacc1_PI_I_SYS_ACC = 0x25110000, // 0x25110000, 浮动缸
  d_sysacc1_PI_I_SYS_FDG               // 0x25110001, 浮动缸检知
};

enum db_sysacc1_ACTPARA{
  d_sysacc1_ACTPARA_DAPRES_SYSACC = 0x25140000, // 0x25140000, 浮动缸压力
  d_sysacc1_ACTPARA_DAFLOW_SYSACC               // 0x25140001, 浮动缸速度
};

enum db_sysacc1_HYDR{
  d_sysacc1_HYDR_HYDR_PORT_SYSACC = 0x25150000, // 0x25150000, 
  d_sysacc1_HYDR_HYDR_PUMPS_SYSACC              // 0x25150001, 
};

enum db_sysacc1_STATE{
  d_sysacc1_STATE_FL_ERROR0 = 0x25160000, // 0x25160000, 
  d_sysacc1_STATE_FL_ERROR1               // 0x25160001, 
};

enum db_shut1_PO{
  d_shut1_PO_O_SHUTFWD = 0x26100000, // 0x26100000, 挡板进
  d_shut1_PO_O_SHUTBWD               // 0x26100001, 挡板退
};

enum db_shut1_PI{
  d_shut1_PI_INPUT_SHUTFWD = 0x26110000, // 0x26110000, 
  d_shut1_PI_INPUT_SHUTBWD               // 0x26110001, 
};

enum db_shut1_MACHSET{
  d_shut1_MACHSET_FL_SHUT = 0x26120000, // 0x26120000, 挡板功能
  d_shut1_MACHSET_FL_SHUT_FUN,          // 0x26120001, 挡板联动选择
  d_shut1_MACHSET_FL_SHUT_MODE,         // 0x26120002, 液压挡板控制方式
  d_shut1_MACHSET_DAPRES_SHUTFWDRAMP,   // 0x26120003, 挡板进压力斜率
  d_shut1_MACHSET_DAFLOW_SHUTFWDRAMP,   // 0x26120004, 挡板进速度斜率
  d_shut1_MACHSET_DAPRES_SHUTFWDOFFRAMP, // 0x26120005, 挡板进关闭压力斜率
  d_shut1_MACHSET_DAFLOW_SHUTFWDOFFRAMP, // 0x26120006, 挡板进关闭速度斜率
  d_shut1_MACHSET_DAPRES_SHUTBWDRAMP,   // 0x26120007, 挡板退压力斜率
  d_shut1_MACHSET_DAFLOW_SHUTBWDRAMP,   // 0x26120008, 挡板退速度斜率
  d_shut1_MACHSET_DAPRES_SHUTBWDOFFRAMP, // 0x26120009, 挡板退关闭压力斜率
  d_shut1_MACHSET_DAFLOW_SHUTBWDOFFRAMP // 0x2612000a, 挡板退关闭速度斜率
};

enum db_shut1_MOLDSET{
  d_shut1_MOLDSET_SHUTBWDOFFRAM = 0x26130000  // 0x26130000, 挡板退关闭速度斜率
};

enum db_shut1_ACTPARA{
  d_shut1_ACTPARA_DAPRES_SHUTFWD = 0x26140000, // 0x26140000, 挡板进压力
  d_shut1_ACTPARA_DAFLOW_SHUTFWD,              // 0x26140001, 挡板进速度
  d_shut1_ACTPARA_TM_SHUTFWD,                  // 0x26140002, 挡板进计时
  d_shut1_ACTPARA_DAPRES_SHUTBWD,              // 0x26140003, 挡板退压力
  d_shut1_ACTPARA_DAFLOW_SHUTBWD,              // 0x26140004, 挡板退速度
  d_shut1_ACTPARA_TM_SHUTBWD                   // 0x26140005, 挡板退计时
};

enum db_shut1_HYDR{
  d_shut1_HYDR_HYDR_PORT_SHU = 0x26150000, // 0x26150000, 
  d_shut1_HYDR_HYDR_PUMPS_SH,              // 0x26150001, 
  d_shut1_HYDR_SERVO_KP_SHUT,              // 0x26150002, 挡板 P
  d_shut1_HYDR_SERVO_KI_SHUT,              // 0x26150003, 挡板 I
  d_shut1_HYDR_SERVO_KD_SHUT               // 0x26150004, 挡板 D
};

enum db_shut1_STATE{
  d_shut1_STATE_FL_ERROR0 = 0x26160000, // 0x26160000, 警报1
  d_shut1_STATE_FL_ERROR1,              // 0x26160001, 警报2
  d_shut1_STATE_Step_ShutFwd,           // 0x26160002, 
  d_shut1_STATE_Step_ShutBwd            // 0x26160003, 
};

enum db_rotate1_PO{
  d_rotate1_PO_O_ROTSLOW = 0x27100000, // 0x27100000, 滑模慢速
  d_rotate1_PO_O_ROT_FWD,              // 0x27100001, 滑模进
  d_rotate1_PO_O_ROT_BWD,              // 0x27100002, 滑模退
  d_rotate1_PO_O_ROT_FWDS,             // 0x27100003, 左滑模快速
  d_rotate1_PO_O_ROT_BWDS,             // 0x27100004, 右滑模快速
  d_rotate1_PO_O_ROT_RSV               // 0x27100005, 滑模备用输出
};

enum db_rotate1_PI{
  d_rotate1_PI_I_ROT_FWD = 0x27110000, // 0x27110000, 滑模进终
  d_rotate1_PI_I_ROT_BWD,              // 0x27110001, 滑模退终
  d_rotate1_PI_I_ROT_FWS,              // 0x27110002, 滑模进慢速
  d_rotate1_PI_I_ROT_BWS,              // 0x27110003, 滑模退慢速
  d_rotate1_PI_I_ROT_RSV               // 0x27110004, 滑模备用输入
};

enum db_rotate1_MACHSET{
  d_rotate1_MACHSET_ROTATE_METERCH = 0x27120000, // 0x27120000, 
  d_rotate1_MACHSET_DAFLOW_SCLPRAMP,             // 0x27120001, 滑模启动速度斜率
  d_rotate1_MACHSET_DAPRES_SCLPRAMP,             // 0x27120002, 滑模启动压力斜率
  d_rotate1_MACHSET_DAFLOW_SCLPOFFRAMP,          // 0x27120003, 滑模结束速度斜率
  d_rotate1_MACHSET_DAPRES_SCLPOFFRAMP,          // 0x27120004, 滑模结束压力斜率
  d_rotate1_MACHSET_TM_SCLPBDLY,                 // 0x27120005, 左滑模延迟
  d_rotate1_MACHSET_TM_SCLPFDLY,                 // 0x27120006, 右滑模延迟
  d_rotate1_MACHSET_TM_ROTATEFAST,               // 0x27120007, 左滑模快速保护计时
  d_rotate1_MACHSET_TM_ROTLOC,                   // 0x27120008, 左滑模保护计时
  d_rotate1_MACHSET_SCLB_HYDELAY,                // 0x27120009, 滑模退液压延迟
  d_rotate1_MACHSET_SCLF_HYDELAY                 // 0x2712000a, 滑模进液压延迟
};

enum db_rotate1_ACTPARA{
  d_rotate1_ACTPARA_ADPOSI_SCLPBFAST = 0x27140000, // 0x27140000, 左滑退快速位置
  d_rotate1_ACTPARA_ADPOSI_SCLPBLOW,               // 0x27140001, 左滑退慢速位置
  d_rotate1_ACTPARA_ADPOSI_SCLPFFAST,              // 0x27140002, 左滑进快速位置
  d_rotate1_ACTPARA_ADPOSI_SCLPFLOW,               // 0x27140003, 左滑进慢速位置
  d_rotate1_ACTPARA_DAFLOW_SCLPBLOW,               // 0x27140004, 左滑模退慢速速度
  d_rotate1_ACTPARA_DAFLOW_SCLPBFAST,              // 0x27140005, 左滑模退快速速度
  d_rotate1_ACTPARA_DAFLOW_SCLPFLOW,               // 0x27140006, 左滑模进慢速速度
  d_rotate1_ACTPARA_DAFLOW_SCLPFFAST,              // 0x27140007, 左滑模进快速速度
  d_rotate1_ACTPARA_DAPRES_SCLPBLOW,               // 0x27140008, 左滑模退慢速压力
  d_rotate1_ACTPARA_DAPRES_SCLPBFAST,              // 0x27140009, 左滑模退快速压力
  d_rotate1_ACTPARA_DAPRES_SCLPFLOW,               // 0x2714000a, 左滑模进慢速压力
  d_rotate1_ACTPARA_DAPRES_SCLPFFAST               // 0x2714000b, 左滑模进快速压力
};

enum db_rotate1_HYDR{
  d_rotate1_HYDR_HYDR_PORT_ROTATEFWD = 0x27150000, // 0x27150000, 
  d_rotate1_HYDR_HYDR_PUMPS_ROTATEFWD,             // 0x27150001, 
  d_rotate1_HYDR_HYDR_PORT_ROTATEBWD,              // 0x27150002, 
  d_rotate1_HYDR_HYDR_PUMPS_ROTATEBWD,             // 0x27150003, 
  d_rotate1_HYDR_SERVO_KP_ROTATEFWD,               // 0x27150004, 
  d_rotate1_HYDR_SERVO_KI_ROTATEFWD,               // 0x27150005, 
  d_rotate1_HYDR_SERVO_KD_ROTATEFWD,               // 0x27150006, 
  d_rotate1_HYDR_SERVO_KP_ROTATEBWD,               // 0x27150007, 
  d_rotate1_HYDR_SERVO_KI_ROTATEBWD,               // 0x27150008, 
  d_rotate1_HYDR_SERVO_KD_ROTATEBWD                // 0x27150009, 
};

enum db_rotate1_STATE{
  d_rotate1_STATE_FL_ERROR0 = 0x27160000, // 0x27160000, 
  d_rotate1_STATE_FL_ERROR1,              // 0x27160001, 
  d_rotate1_STATE_Step_RotateFwd,         // 0x27160002, 
  d_rotate1_STATE_Step_RotateBwd,         // 0x27160003, 
  d_rotate1_STATE_GRADE_ERR0,             // 0x27160004, 
  d_rotate1_STATE_GRADE_ERR1,             // 0x27160005, 
  d_rotate1_STATE_GRADE_ERR2,             // 0x27160006, 
  d_rotate1_STATE_GRADE_ERR3              // 0x27160007, 
};

enum db_rotate2_PO{
  d_rotate2_PO_O_ROTSLOW = 0x27200000, // 0x27200000, 滑模慢速
  d_rotate2_PO_O_ROT_FWD,              // 0x27200001, 滑模进
  d_rotate2_PO_O_ROT_BWD,              // 0x27200002, 滑模退
  d_rotate2_PO_O_ROT_FWDS,             // 0x27200003, 左滑模快速
  d_rotate2_PO_O_ROT_BWDS,             // 0x27200004, 右滑模快速
  d_rotate2_PO_O_ROT_RSV               // 0x27200005, 滑模备用输出
};

enum db_rotate2_PI{
  d_rotate2_PI_I_ROT_FWD = 0x27210000, // 0x27210000, 滑模进终
  d_rotate2_PI_I_ROT_BWD,              // 0x27210001, 滑模退终
  d_rotate2_PI_I_ROT_FWS,              // 0x27210002, 滑模进慢速
  d_rotate2_PI_I_ROT_BWS,              // 0x27210003, 滑模退慢速
  d_rotate2_PI_I_ROT_RSV               // 0x27210004, 滑模备用输入
};

enum db_rotate2_MACHSET{
  d_rotate2_MACHSET_ROTATE_METERCH = 0x27220000, // 0x27220000, 
  d_rotate2_MACHSET_DAFLOW_SCLPRAMP,             // 0x27220001, 滑模启动速度斜率
  d_rotate2_MACHSET_DAPRES_SCLPRAMP,             // 0x27220002, 滑模启动压力斜率
  d_rotate2_MACHSET_DAFLOW_SCLPOFFRAMP,          // 0x27220003, 滑模结束速度斜率
  d_rotate2_MACHSET_DAPRES_SCLPOFFRAMP,          // 0x27220004, 滑模结束压力斜率
  d_rotate2_MACHSET_TM_SCLPBDLY,                 // 0x27220005, 左滑模延迟
  d_rotate2_MACHSET_TM_SCLPFDLY,                 // 0x27220006, 右滑模延迟
  d_rotate2_MACHSET_TM_ROTATEFAST,               // 0x27220007, 左滑模快速保护计时
  d_rotate2_MACHSET_TM_ROTLOC,                   // 0x27220008, 左滑模保护计时
  d_rotate2_MACHSET_SCLB_HYDELAY,                // 0x27220009, 滑模退液压延迟
  d_rotate2_MACHSET_SCLF_HYDELAY                 // 0x2722000a, 滑模进液压延迟
};

enum db_rotate2_ACTPARA{
  d_rotate2_ACTPARA_ADPOSI_SCLPBFAST = 0x27240000, // 0x27240000, 左滑退快速位置
  d_rotate2_ACTPARA_ADPOSI_SCLPBLOW,               // 0x27240001, 左滑退慢速位置
  d_rotate2_ACTPARA_ADPOSI_SCLPFFAST,              // 0x27240002, 左滑进快速位置
  d_rotate2_ACTPARA_ADPOSI_SCLPFLOW,               // 0x27240003, 左滑进慢速位置
  d_rotate2_ACTPARA_DAFLOW_SCLPBLOW,               // 0x27240004, 左滑模退慢速速度
  d_rotate2_ACTPARA_DAFLOW_SCLPBFAST,              // 0x27240005, 左滑模退快速速度
  d_rotate2_ACTPARA_DAFLOW_SCLPFLOW,               // 0x27240006, 左滑模进慢速速度
  d_rotate2_ACTPARA_DAFLOW_SCLPFFAST,              // 0x27240007, 左滑模进快速速度
  d_rotate2_ACTPARA_DAPRES_SCLPBLOW,               // 0x27240008, 左滑模退慢速压力
  d_rotate2_ACTPARA_DAPRES_SCLPBFAST,              // 0x27240009, 左滑模退快速压力
  d_rotate2_ACTPARA_DAPRES_SCLPFLOW,               // 0x2724000a, 左滑模进慢速压力
  d_rotate2_ACTPARA_DAPRES_SCLPFFAST               // 0x2724000b, 左滑模进快速压力
};

enum db_rotate2_HYDR{
  d_rotate2_HYDR_HYDR_PORT_ROTATEFWD = 0x27250000, // 0x27250000, 
  d_rotate2_HYDR_HYDR_PUMPS_ROTATEFWD,             // 0x27250001, 
  d_rotate2_HYDR_HYDR_PORT_ROTATEBWD,              // 0x27250002, 
  d_rotate2_HYDR_HYDR_PUMPS_ROTATEBWD,             // 0x27250003, 
  d_rotate2_HYDR_SERVO_KP_ROTATEFWD,               // 0x27250004, 
  d_rotate2_HYDR_SERVO_KI_ROTATEFWD,               // 0x27250005, 
  d_rotate2_HYDR_SERVO_KD_ROTATEFWD,               // 0x27250006, 
  d_rotate2_HYDR_SERVO_KP_ROTATEBWD,               // 0x27250007, 
  d_rotate2_HYDR_SERVO_KI_ROTATEBWD,               // 0x27250008, 
  d_rotate2_HYDR_SERVO_KD_ROTATEBWD                // 0x27250009, 
};

enum db_rotate2_STATE{
  d_rotate2_STATE_FL_ERROR0 = 0x27260000, // 0x27260000, 
  d_rotate2_STATE_FL_ERROR1,              // 0x27260001, 
  d_rotate2_STATE_Step_RotateFwd,         // 0x27260002, 
  d_rotate2_STATE_Step_RotateBwd,         // 0x27260003, 
  d_rotate2_STATE_GRADE_ERR0,             // 0x27260004, 
  d_rotate2_STATE_GRADE_ERR1,             // 0x27260005, 
  d_rotate2_STATE_GRADE_ERR2,             // 0x27260006, 
  d_rotate2_STATE_GRADE_ERR3              // 0x27260007, 
};

enum db_lock1_PO{
  d_lock1_PO_O_LOC_FWD = 0x28100000, // 0x28100000, 定位1进
  d_lock1_PO_O_LOC_BWD,              // 0x28100001, 定位1退
  d_lock1_PO_O_LOC_SRV               // 0x28100002, 定位1备用
};

enum db_lock1_PI{
  d_lock1_PI_I_LOC_FWD = 0x28110000, // 0x28110000, 定位1进终
  d_lock1_PI_I_LOC_BWD,              // 0x28110001, 定位1退终
  d_lock1_PI_I_LOC_SRV               // 0x28110002, 定位1备用
};

enum db_lock1_MACHSET{
  d_lock1_MACHSET_DAPRES_LOCRAMP = 0x28120000, // 0x28120000, 左定位启动压力斜率
  d_lock1_MACHSET_DAFLOW_LOCRAMP,              // 0x28120001, 左定位启动流量斜率
  d_lock1_MACHSET_DAPRES_LOCOFFRAMP,           // 0x28120002, 左定位结束压力斜率
  d_lock1_MACHSET_DAFLOW_LOCOFFRAMP,           // 0x28120003, 左定位结束流量斜率
  d_lock1_MACHSET_DAFLOW_LOCFWD_SETUP,         // 0x28120004, 左定位慢速调试流量
  d_lock1_MACHSET_DAPRES_LOCBWD_SETUP,         // 0x28120005, 左定位慢速调试压力
  d_lock1_MACHSET_LOCKF_HYDELAY,               // 0x28120006, 定位进液压延迟
  d_lock1_MACHSET_LOCKB_HYDELAY                // 0x28120007, 定位退液压延迟
};

enum db_lock1_MOLDSET{
  d_lock1_MOLDSET_FL_LOC = 0x28130000, // 0x28130000, 左定位使用选择
  d_lock1_MOLDSET_TM_LOCFWDDLY,        // 0x28130001, 左定位进延迟
  d_lock1_MOLDSET_TM_LOCBWDDLY         // 0x28130002, 左定位退延迟
};

enum db_lock1_ACTPARA{
  d_lock1_ACTPARA_DAPRES_LCKFWD = 0x28140000, // 0x28140000, 左定位进压力
  d_lock1_ACTPARA_DAPRES_LCKBWD,              // 0x28140001, 左定位退压力
  d_lock1_ACTPARA_DAFLOW_LCKFWD,              // 0x28140002, 左定位进速度
  d_lock1_ACTPARA_DAFLOW_LCKBWD               // 0x28140003, 左定位退速度
};

enum db_lock1_HYDR{
  d_lock1_HYDR_HYDR_PORT_LOCK = 0x28150000, // 0x28150000, 
  d_lock1_HYDR_HYDR_PUMPS_LOCK,             // 0x28150001, 
  d_lock1_HYDR_SERVO_KP_LOCK,               // 0x28150002, 
  d_lock1_HYDR_SERVO_KI_LOCK,               // 0x28150003, 
  d_lock1_HYDR_SERVO_KD_LOCK                // 0x28150004, 
};

enum db_lock1_STATE{
  d_lock1_STATE_FL_ERROR1 = 0x28160000, // 0x28160000, 
  d_lock1_STATE_FL_ERROR2,              // 0x28160001, 
  d_lock1_STATE_Step_LockFwd,           // 0x28160002, 
  d_lock1_STATE_Step_LockBwd            // 0x28160003, 
};

enum db_lock2_PO{
  d_lock2_PO_O_LOC_FWD = 0x28200000, // 0x28200000, 定位2进
  d_lock2_PO_O_LOC_BWD,              // 0x28200001, 定位2退
  d_lock2_PO_O_LOC_SRV               // 0x28200002, 定位2备用
};

enum db_lock2_PI{
  d_lock2_PI_I_LOC_FWD = 0x28210000, // 0x28210000, 定位2进终
  d_lock2_PI_I_LOC_BWD,              // 0x28210001, 定位2退终
  d_lock2_PI_I_LOC_SRV               // 0x28210002, 定位2备用
};

enum db_lock2_MACHSET{
  d_lock2_MACHSET_DAPRES_LOCRAMP = 0x28220000, // 0x28220000, 右定位启动压力斜率
  d_lock2_MACHSET_DAFLOW_LOCRAMP,              // 0x28220001, 右定位启动速度斜率
  d_lock2_MACHSET_DAPRES_LOCOFFRAMP,           // 0x28220002, 右定位结束压力斜率
  d_lock2_MACHSET_DAFLOW_LOCOFFRAMP,           // 0x28220003, 右定位结束速度斜率
  d_lock2_MACHSET_DAFLOW_LOCFWD_SETUP,         // 0x28220004, 右定位慢速调试速度
  d_lock2_MACHSET_DAPRES_LOCBWD_SETUP,         // 0x28220005, 右定位慢速调试压力
  d_lock2_MACHSET_LOCKF_HYDELAY,               // 0x28220006, 定位进液压延迟
  d_lock2_MACHSET_LOCKB_HYDELAY                // 0x28220007, 定位退液压延迟
};

enum db_lock2_MOLDSET{
  d_lock2_MOLDSET_FL_LOC = 0x28230000, // 0x28230000, 右定位使用选择
  d_lock2_MOLDSET_TM_LOCFWDDLY,        // 0x28230001, 右定位进延迟
  d_lock2_MOLDSET_TM_LOCBWDDLY         // 0x28230002, 右定位退延迟
};

enum db_lock2_ACTPARA{
  d_lock2_ACTPARA_DAPRES_LCKFWD = 0x28240000, // 0x28240000, 右定位进压力
  d_lock2_ACTPARA_DAPRES_LCKBWD,              // 0x28240001, 右定位退压力
  d_lock2_ACTPARA_DAFLOW_LCKFWD,              // 0x28240002, 右定位进速度
  d_lock2_ACTPARA_DAFLOW_LCKBWD               // 0x28240003, 右定位退速度
};

enum db_lock2_HYDR{
  d_lock2_HYDR_HYDR_PORT_LOCK = 0x28250000, // 0x28250000, 
  d_lock2_HYDR_HYDR_PUMPS_LOCK,             // 0x28250001, 
  d_lock2_HYDR_SERVO_KP_LOCK,               // 0x28250002, 
  d_lock2_HYDR_SERVO_KI_LOCK,               // 0x28250003, 
  d_lock2_HYDR_SERVO_KD_LOCK                // 0x28250004, 
};

enum db_lock2_STATE{
  d_lock2_STATE_FL_ERROR1 = 0x28260000, // 0x28260000, 
  d_lock2_STATE_FL_ERROR2,              // 0x28260001, 
  d_lock2_STATE_Step_LockFwd,           // 0x28260002, 
  d_lock2_STATE_Step_LockBwd            // 0x28260003, 
};

enum db_safedoor1_PO{
  d_safedoor1_PO_O_SDR_OPN = 0x29100000, // 0x29100000, 安全门开
  d_safedoor1_PO_O_SDR_CLS,              // 0x29100001, 安全门关
  d_safedoor1_PO_O_SDR_SLW,              // 0x29100002, 安全门慢速
  d_safedoor1_PO_O_SDR_FST,              // 0x29100003, 安全门快速
  d_safedoor1_PO_O_RSDR_OP,              // 0x29100004, 后安全门开
  d_safedoor1_PO_O_RSDR_CL,              // 0x29100005, 后安全门关
  d_safedoor1_PO_O_SDR_RSV,              // 0x29100006, 安全门留
  d_safedoor1_PO_O_SDR_ERR,              // 0x29100007, 门机复位
  d_safedoor1_PO_O_SDR_VALVE             // 0x29100008, 安全阀
};

enum db_safedoor1_PI{
  d_safedoor1_PI_I_SDR_CLS = 0x29110000, // 0x29110000, 安全门关
  d_safedoor1_PI_I_SDR_CL2,              // 0x29110001, 安全门关二
  d_safedoor1_PI_I_SDR_OPN,              // 0x29110002, 安全门开终
  d_safedoor1_PI_I_SDR_SLW,              // 0x29110003, 安全门关慢
  d_safedoor1_PI_I_SDR_OSL,              // 0x29110004, 安全门开慢
  d_safedoor1_PI_I_SDR_SAF,              // 0x29110005, 安全门防碰
  d_safedoor1_PI_I_RSD_CLS,              // 0x29110006, 后安全门关终
  d_safedoor1_PI_I_FOOTSW,               // 0x29110007, 脚踏开关
  d_safedoor1_PI_I_FOOTEN,               // 0x29110008, 可用脚踏开关
  d_safedoor1_PI_I_SAF_PLT,              // 0x29110009, 底板安全
  d_safedoor1_PI_I_SDR_RSV,              // 0x2911000a, 安全门保留
  d_safedoor1_PI_I_SDR_SQ42,             // 0x2911000b, 安全阀控制开关
  d_safedoor1_PI_I_SDR_VALVE,            // 0x2911000c, 安全阀检测
  d_safedoor1_PI_I_SDR_PREVAVLE,         // 0x2911000d, 安全阀先导检测
  d_safedoor1_PI_I_SDR_ERR,              // 0x2911000e, 门机故障
  d_safedoor1_PI_I_SDR_SAFE_RULE,        // 0x2911000f, 安全门检知
  d_safedoor1_PI_I_RSD_CLS2,             // 0x29110010, 后安全门关2
  d_safedoor1_PI_I_SFE_LINE,             // 0x29110011, 安全杆
  d_safedoor1_PI_I_SAFE_EDGE,            // 0x29110012, 安全边沿开关
  d_safedoor1_PI_I_SAFE_REMERG,          // 0x29110013, 安全门急停
  d_safedoor1_PI_I_SAFE_CHK,             // 0x29110014, 安全继电器检知
  d_safedoor1_PI_I_SAFEBLOCK_CHK,        // 0x29110015, 安全模块检知
  d_safedoor1_PI_I_SAFE_BOARD            // 0x29110016, 安全踏板
};

enum db_safedoor1_MACHSET{
  d_safedoor1_MACHSET_TM_SAFEDOORSLOWVLV = 0x29120000, // 0x29120000, 安全门慢速阀动作时间
  d_safedoor1_MACHSET_TMDLY_PWRDOOR,                   // 0x29120001, 安全门延迟
  d_safedoor1_MACHSET_DAPRES_SAFEDRZRAMP,              // 0x29120002, 安全门启动压力斜率
  d_safedoor1_MACHSET_DAFLOW_SAFEDRZRAMP,              // 0x29120003, 安全门启动速度斜率
  d_safedoor1_MACHSET_DAPRES_SAFEDROFFRAMP,            // 0x29120004, 安全门关闭压力斜率
  d_safedoor1_MACHSET_DAFLOW_SAFEDROFFRAMP,            // 0x29120005, 安全门关闭速度斜率
  d_safedoor1_MACHSET_DAPRES_SAFETYDOOR_SETUP,         // 0x29120006, 安全门慢速调试压力
  d_safedoor1_MACHSET_DAFLOW_SAFETYDOOR_SETUP,         // 0x29120007, 安全门慢速调试速度
  d_safedoor1_MACHSET_SFDROPN_HYDELAY,                 // 0x29120008, 安全门开液压延迟
  d_safedoor1_MACHSET_SFDRCLS_HYDELAY,                 // 0x29120009, 安全门关液压延迟
  d_safedoor1_MACHSET_SFDRCLS_TMCHECK,                 // 0x2912000a, 安全门开检测时间
  d_safedoor1_MACHSET_SDR_VALVE_CLSTMCHECK,            // 0x2912000b, 安全阀关检测计时
  d_safedoor1_MACHSET_SDR_VALVE_OPNTMCHECK,            // 0x2912000c, 安全阀开检测计时
  d_safedoor1_MACHSET_TM_SFBLOCKCHECK,                 // 0x2912000d, 安全模块检测时间
  d_safedoor1_MACHSET_FL_SFBLOCKCHECK,                 // 0x2912000e, 安全模块使用
  d_safedoor1_MACHSET_FL_SFRELAYCHECK,                 // 0x2912000f, 安全继电器使用
  d_safedoor1_MACHSET_TM_SFRELAYCHECK                  // 0x29120010, 安全继电器检测时间
};

enum db_safedoor1_MOLDSET{
  d_safedoor1_MOLDSET_FL_PWRDOORCTRL = 0x29130000, // 0x29130000, 自动安全门功能
  d_safedoor1_MOLDSET_FL_PWRDOOROPEN,              // 0x29130001, 安全门开模式
  d_safedoor1_MOLDSET_FL_MANULTEST,                // 0x29130002, 安全门调试功能
  d_safedoor1_MOLDSET_FL_TESTTIMES,                // 0x29130003, 安全门测试次数
  d_safedoor1_MOLDSET_POS_SDROPEN                  // 0x29130004, 安全门开位置
};

enum db_safedoor1_ACTPARA{
  d_safedoor1_ACTPARA_DAPRES_SAFETYDOOR = 0x29140000, // 0x29140000, 安全门压力
  d_safedoor1_ACTPARA_DAFLOW_SAFETYDOOR,              // 0x29140001, 安全门速度
  d_safedoor1_ACTPARA_DAPRES_SAFETYDOORSLOW,          // 0x29140002, 安全门慢速压力
  d_safedoor1_ACTPARA_DAFLOW_SAFETYDOORSLOW,          // 0x29140003, 安全门慢速速度
  d_safedoor1_ACTPARA_TM_PWRDOOROPEN,                 // 0x29140004, 安全门开计时
  d_safedoor1_ACTPARA_PER_DAFLOW_SAFEDOOR             // 0x29140005, 安全门速度百分比
};

enum db_safedoor1_HYDR{
  d_safedoor1_HYDR_HYDR_PORT_SAFETYDOOR = 0x29150000, // 0x29150000, 
  d_safedoor1_HYDR_HYDR_PUMPS_SAFETYDOOR,             // 0x29150001, 
  d_safedoor1_HYDR_SERVO_KP_SAFETYDOOR,               // 0x29150002, 安全门 P
  d_safedoor1_HYDR_SERVO_KI_SAFETYDOOR,               // 0x29150003, 安全门 I
  d_safedoor1_HYDR_SERVO_KD_SAFETYDOOR                // 0x29150004, 安全门 D
};

enum db_safedoor1_STATE{
  d_safedoor1_STATE_FL_ERROR0 = 0x29160000, // 0x29160000, 警报1
  d_safedoor1_STATE_FL_ERROR1,              // 0x29160001, 警报2
  d_safedoor1_STATE_Step_DoorOpen,          // 0x29160002, 
  d_safedoor1_STATE_Step_DoorClos           // 0x29160003, 
};

enum db_ModeSpc1_PO{
  d_ModeSpc1_PO_O_OPN_END_SPC = 0x2a100000, // 0x2a100000, 特殊开模结束
  d_ModeSpc1_PO_O_SFD_CLS_SPC,              // 0x2a100001, 特殊安全门关
  d_ModeSpc1_PO_O_M_OPN,                    // 0x2a100002, 模具开模完成
  d_ModeSpc1_PO_O_M_SFD,                    // 0x2a100003, 模具安全门关
  d_ModeSpc1_PO_O_M_EJEF,                   // 0x2a100004, 模具顶进完成
  d_ModeSpc1_PO_O_M_EJEB,                   // 0x2a100005, 模具顶退完成
  d_ModeSpc1_PO_O_M_MYALARM                 // 0x2a100006, 模压警报解除
};

enum db_ModeSpc1_PI{
  d_ModeSpc1_PI_I_M_MYALARM = 0x2a110000, // 0x2a110000, 模压警报
  d_ModeSpc1_PI_I_M_CLS,                  // 0x2a110001, 模具关摸安全
  d_ModeSpc1_PI_I_M_EJEF,                 // 0x2a110002, 模具顶进安全
  d_ModeSpc1_PI_I_M_EJEB,                 // 0x2a110003, 模具顶退安全
  d_ModeSpc1_PI_I_M_ROB,                  // 0x2a110004, 模具允许机械手
  d_ModeSpc1_PI_I_M_ALAR                  // 0x2a110005, 模具保护警报
};

enum db_ModeSpc1_MOLDSET{
  d_ModeSpc1_MOLDSET_FL_DETPRE_ALARM = 0x2a130000, // 0x2a130000, 模腔压力偏差是否出警报
  d_ModeSpc1_MOLDSET_FL_MOJUPOTE,                  // 0x2a130001, 模具保护使用
  d_ModeSpc1_MOLDSET_FL_MODE_MODUL_1,              // 0x2a130002, 模块检测1
  d_ModeSpc1_MOLDSET_FL_MODE_MODUL_2,              // 0x2a130003, 模块检测1
  d_ModeSpc1_MOLDSET_FL_MODE_MODUL_3,              // 0x2a130004, 模块检测1
  d_ModeSpc1_MOLDSET_FL_MODE_MODUL_4,              // 0x2a130005, 模块检测1
  d_ModeSpc1_MOLDSET_FL_MODE_MODUL_5,              // 0x2a130006, 模块检测1
  d_ModeSpc1_MOLDSET_FL_MODE_MODUL_6,              // 0x2a130007, 模块检测1
  d_ModeSpc1_MOLDSET_FL_MODE_MODUL_7,              // 0x2a130008, 模块检测1
  d_ModeSpc1_MOLDSET_FL_MODE_MODUL_8,              // 0x2a130009, 模块检测1
  d_ModeSpc1_MOLDSET_FL_MODE_MODUL_9,              // 0x2a13000a, 模块检测1
  d_ModeSpc1_MOLDSET_FL_MODE_MODUL_10,             // 0x2a13000b, 模块检测1
  d_ModeSpc1_MOLDSET_FL_MODE_MODUL_11,             // 0x2a13000c, 模块检测1
  d_ModeSpc1_MOLDSET_FL_MODE_MODUL_12,             // 0x2a13000d, 模块检测1
  d_ModeSpc1_MOLDSET_FL_MODE_MODUL_13,             // 0x2a13000e, 模块检测1
  d_ModeSpc1_MOLDSET_FL_MODE_MODUL_14,             // 0x2a13000f, 模块检测1
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_BEG_1,          // 0x2a130010, 模块检测1起始时间
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_BEG_2,          // 0x2a130011, 模块检测1起始时间
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_BEG_3,          // 0x2a130012, 模块检测1起始时间
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_BEG_4,          // 0x2a130013, 模块检测1起始时间
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_BEG_5,          // 0x2a130014, 模块检测1起始时间
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_BEG_6,          // 0x2a130015, 模块检测1起始时间
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_BEG_7,          // 0x2a130016, 模块检测1起始时间
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_BEG_8,          // 0x2a130017, 模块检测1起始时间
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_BEG_9,          // 0x2a130018, 模块检测1起始时间
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_BEG_10,         // 0x2a130019, 模块检测1起始时间
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_BEG_11,         // 0x2a13001a, 模块检测1起始时间
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_BEG_12,         // 0x2a13001b, 模块检测1起始时间
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_BEG_13,         // 0x2a13001c, 模块检测1起始时间
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_BEG_14,         // 0x2a13001d, 模块检测1起始时间
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_END_1,          // 0x2a13001e, 模块检测1结束时间
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_END_2,          // 0x2a13001f, 模块检测1结束时间
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_END_3,          // 0x2a130020, 模块检测1结束时间
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_END_4,          // 0x2a130021, 模块检测1结束时间
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_END_5,          // 0x2a130022, 模块检测1结束时间
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_END_6,          // 0x2a130023, 模块检测1结束时间
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_END_7,          // 0x2a130024, 模块检测1结束时间
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_END_8,          // 0x2a130025, 模块检测1结束时间
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_END_9,          // 0x2a130026, 模块检测1结束时间
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_END_10,         // 0x2a130027, 模块检测1结束时间
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_END_11,         // 0x2a130028, 模块检测1结束时间
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_END_12,         // 0x2a130029, 模块检测1结束时间
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_END_13,         // 0x2a13002a, 模块检测1结束时间
  d_ModeSpc1_MOLDSET_TM_MODE_MODUL_END_14,         // 0x2a13002b, 模块检测1结束时间
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_UP_1,           // 0x2a13002c, 模块检测1上限值
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_UP_2,           // 0x2a13002d, 模块检测1上限值
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_UP_3,           // 0x2a13002e, 模块检测1上限值
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_UP_4,           // 0x2a13002f, 模块检测1上限值
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_UP_5,           // 0x2a130030, 模块检测1上限值
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_UP_6,           // 0x2a130031, 模块检测1上限值
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_UP_7,           // 0x2a130032, 模块检测1上限值
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_UP_8,           // 0x2a130033, 模块检测1上限值
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_UP_9,           // 0x2a130034, 模块检测1上限值
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_UP_10,          // 0x2a130035, 模块检测1上限值
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_UP_11,          // 0x2a130036, 模块检测1上限值
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_UP_12,          // 0x2a130037, 模块检测1上限值
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_UP_13,          // 0x2a130038, 模块检测1上限值
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_UP_14,          // 0x2a130039, 模块检测1上限值
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_DOW_1,          // 0x2a13003a, 模块检测1下限值
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_DOW_2,          // 0x2a13003b, 模块检测1下限值
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_DOW_3,          // 0x2a13003c, 模块检测1下限值
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_DOW_4,          // 0x2a13003d, 模块检测1下限值
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_DOW_5,          // 0x2a13003e, 模块检测1下限值
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_DOW_6,          // 0x2a13003f, 模块检测1下限值
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_DOW_7,          // 0x2a130040, 模块检测1下限值
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_DOW_8,          // 0x2a130041, 模块检测1下限值
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_DOW_9,          // 0x2a130042, 模块检测1下限值
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_DOW_10,         // 0x2a130043, 模块检测1下限值
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_DOW_11,         // 0x2a130044, 模块检测1下限值
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_DOW_12,         // 0x2a130045, 模块检测1下限值
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_DOW_13,         // 0x2a130046, 模块检测1下限值
  d_ModeSpc1_MOLDSET_LM_MODE_MODUL_DOW_14,         // 0x2a130047, 模块检测1下限值
  d_ModeSpc1_MOLDSET_FL_PROTECT_MANUAL,            // 0x2a130048, 手动模具保护功能
  d_ModeSpc1_MOLDSET_FL_PROTECT_AUTO,              // 0x2a130049, 自动模具保护功能
  d_ModeSpc1_MOLDSET_TM_PROTECT_FILTER,            // 0x2a13004a, 模具保护检知时间
  d_ModeSpc1_MOLDSET_CN_AUTOTORQUE,                // 0x2a13004b, 自动模具保护启动模数
  d_ModeSpc1_MOLDSET_TORQUEOFFSET1,                // 0x2a13004c, 自动峰值扭矩上偏差 1
  d_ModeSpc1_MOLDSET_TORQUEOFFSET2,                // 0x2a13004d, 自动峰值扭矩上偏差 2
  d_ModeSpc1_MOLDSET_TORQUEOFFSET3                 // 0x2a13004e, 自动峰值扭矩上偏差 3
};

enum db_ModeSpc1_ACTPARA{
  d_ModeSpc1_ACTPARA_ADPOS_PROTECTSTART = 0x2a140000, // 0x2a140000, 全电]模具保护开始位置
  d_ModeSpc1_ACTPARA_ADPOS_PROTECTEND1,               // 0x2a140001, 全电]模具保护一段终止位置
  d_ModeSpc1_ACTPARA_ADPOS_PROTECTEND2,               // 0x2a140002, 全电]模具保护二段终止位置
  d_ModeSpc1_ACTPARA_ADPOS_PROTECTEND3,               // 0x2a140003, 全电]模具保护三段终止位置
  d_ModeSpc1_ACTPARA_TORQUE_PROTECT1,                 // 0x2a140004, 全电]模具保护一段保护扭矩
  d_ModeSpc1_ACTPARA_TORQUE_PROTECT2,                 // 0x2a140005, 全电]模具保护二段保护扭矩
  d_ModeSpc1_ACTPARA_TORQUE_PROTECT3                  // 0x2a140006, 全电]模具保护三段保护扭矩
};

enum db_ModeSpc1_STATE{
  d_ModeSpc1_STATE_FL_ERROR0 = 0x2a160000, // 0x2a160000, 警报1
  d_ModeSpc1_STATE_FL_ERROR1,              // 0x2a160001, 警报2
  d_ModeSpc1_STATE_TORQUE_MAX_CLPPRO1,     // 0x2a160002, 全电]模具保护一段峰值扭矩
  d_ModeSpc1_STATE_TORQUE_MAX_CLPPRO2,     // 0x2a160003, 全电]模具保护二段峰值扭矩
  d_ModeSpc1_STATE_TORQUE_MAX_CLPPRO3,     // 0x2a160004, 全电]模具保护三段峰值扭矩
  d_ModeSpc1_STATE_ADPOSI_MAX_CLPPRO1,     // 0x2a160005, 全电]模具保护一段峰值T位置
  d_ModeSpc1_STATE_ADPOSI_MAX_CLPPRO2,     // 0x2a160006, 全电]模具保护二段峰值T位置
  d_ModeSpc1_STATE_ADPOSI_MAX_CLPPRO3,     // 0x2a160007, 全电]模具保护三段峰值T位置
  d_ModeSpc1_STATE_TORQUE_AVG_CLPPRO1,     // 0x2a160008, 全电]模具保护一段平均峰值扭矩
  d_ModeSpc1_STATE_TORQUE_AVG_CLPPRO2,     // 0x2a160009, 全电]模具保护二段平均峰值扭矩
  d_ModeSpc1_STATE_TORQUE_AVG_CLPPRO3      // 0x2a16000a, 全电]模具保护三段平均峰值扭矩
};

enum db_TempCtrlMW1_PO{
  d_TempCtrlMW1_PO_O_COOL_MW = 0x2f100000  // 0x2f100000, 冷却水
};

enum db_TempCtrlMW1_PI{
  d_TempCtrlMW1_PI_I_HEAT_MW = 0x2f110000  // 0x2f110000, 加热接触器
};

enum db_TempCtrlMW1_MACHSET{
  d_TempCtrlMW1_MACHSET_TEMP_PROTECT_MW = 0x2f120000, // 0x2f120000, 温度保护上限
  d_TempCtrlMW1_MACHSET_TEMP_CONTROL_MW,              // 0x2f120001, 温度控制
  d_TempCtrlMW1_MACHSET_TEMP_CANLINK                  // 0x2f120002, 模温板接线
};

enum db_TempCtrlMW1_MOLDSET{
  d_TempCtrlMW1_MOLDSET_DATEMP_SETTING_MW0 = 0x2f130000, // 0x2f130000, 温度1段设定值
  d_TempCtrlMW1_MOLDSET_DATEMP_SETTING_MW1,              // 0x2f130001, 温度2段设定值
  d_TempCtrlMW1_MOLDSET_DATEMP_SETTING_MW2,              // 0x2f130002, 温度3段设定值
  d_TempCtrlMW1_MOLDSET_DATEMP_SETTING_MW3,              // 0x2f130003, 温度4段设定值
  d_TempCtrlMW1_MOLDSET_DATEMP_SETTING_MW4,              // 0x2f130004, 温度5段设定值
  d_TempCtrlMW1_MOLDSET_DATEMP_SETTING_MW5,              // 0x2f130005, 温度6段设定值
  d_TempCtrlMW1_MOLDSET_DATEMP_SETTING_MW6,              // 0x2f130006, 温度7段设定值
  d_TempCtrlMW1_MOLDSET_DATEMP_SETTING_MW7,              // 0x2f130007, 温度8段设定值
  d_TempCtrlMW1_MOLDSET_DATEMP_SETTING_MW8,              // 0x2f130008, 温度9段设定值
  d_TempCtrlMW1_MOLDSET_DATEMP_SETTING_MW9,              // 0x2f130009, 温度10段设定值
  d_TempCtrlMW1_MOLDSET_DATEMP_SETTING_MW10,             // 0x2f13000a, 温度11段设定值
  d_TempCtrlMW1_MOLDSET_DATEMP_SETTING_MW11,             // 0x2f13000b, 温度12段设定值
  d_TempCtrlMW1_MOLDSET_DATEMP_SETTING_MW12,             // 0x2f13000c, 温度13段设定值
  d_TempCtrlMW1_MOLDSET_DATEMP_SETTING_MW13,             // 0x2f13000d, 温度14段设定值
  d_TempCtrlMW1_MOLDSET_DATEMP_SETTING_MW14,             // 0x2f13000e, 温度15段设定值
  d_TempCtrlMW1_MOLDSET_DATEMP_SETTING_MW15,             // 0x2f13000f, 温度16段设定值
  d_TempCtrlMW1_MOLDSET_DATEMP_FUNC_MW0,                 // 0x2f130010, 温控1段功能选择
  d_TempCtrlMW1_MOLDSET_DATEMP_FUNC_MW1,                 // 0x2f130011, 温控2段功能选择
  d_TempCtrlMW1_MOLDSET_DATEMP_FUNC_MW2,                 // 0x2f130012, 温控3段功能选择
  d_TempCtrlMW1_MOLDSET_DATEMP_FUNC_MW3,                 // 0x2f130013, 温控4段功能选择
  d_TempCtrlMW1_MOLDSET_DATEMP_FUNC_MW4,                 // 0x2f130014, 温控5段功能选择
  d_TempCtrlMW1_MOLDSET_DATEMP_FUNC_MW5,                 // 0x2f130015, 温控6段功能选择
  d_TempCtrlMW1_MOLDSET_DATEMP_FUNC_MW6,                 // 0x2f130016, 温控7段功能选择
  d_TempCtrlMW1_MOLDSET_DATEMP_FUNC_MW7,                 // 0x2f130017, 温控8段功能选择
  d_TempCtrlMW1_MOLDSET_DATEMP_FUNC_MW8,                 // 0x2f130018, 温控9段功能选择
  d_TempCtrlMW1_MOLDSET_DATEMP_FUNC_MW9,                 // 0x2f130019, 温控10段功能选择
  d_TempCtrlMW1_MOLDSET_DATEMP_FUNC_MW10,                // 0x2f13001a, 温控11段功能选择
  d_TempCtrlMW1_MOLDSET_DATEMP_FUNC_MW11,                // 0x2f13001b, 温控12段功能选择
  d_TempCtrlMW1_MOLDSET_DATEMP_FUNC_MW12,                // 0x2f13001c, 温控13段功能选择
  d_TempCtrlMW1_MOLDSET_DATEMP_FUNC_MW13,                // 0x2f13001d, 温控14段功能选择
  d_TempCtrlMW1_MOLDSET_DATEMP_FUNC_MW14,                // 0x2f13001e, 温控15段功能选择
  d_TempCtrlMW1_MOLDSET_DATEMP_FUNC_MW15,                // 0x2f13001f, 温控16段功能选择
  d_TempCtrlMW1_MOLDSET_DAPID_KP_MW0,                    // 0x2f130020, 温度1段P系数
  d_TempCtrlMW1_MOLDSET_DAPID_KP_MW1,                    // 0x2f130021, 温度2段P系数
  d_TempCtrlMW1_MOLDSET_DAPID_KP_MW2,                    // 0x2f130022, 温度3段P系数
  d_TempCtrlMW1_MOLDSET_DAPID_KP_MW3,                    // 0x2f130023, 温度4段P系数
  d_TempCtrlMW1_MOLDSET_DAPID_KP_MW4,                    // 0x2f130024, 温度5段P系数
  d_TempCtrlMW1_MOLDSET_DAPID_KP_MW5,                    // 0x2f130025, 温度6段P系数
  d_TempCtrlMW1_MOLDSET_DAPID_KP_MW6,                    // 0x2f130026, 温度7段P系数
  d_TempCtrlMW1_MOLDSET_DAPID_KP_MW7,                    // 0x2f130027, 温度8段P系数
  d_TempCtrlMW1_MOLDSET_DAPID_KP_MW8,                    // 0x2f130028, 温度9段P系数
  d_TempCtrlMW1_MOLDSET_DAPID_KP_MW9,                    // 0x2f130029, 温度10段P系数
  d_TempCtrlMW1_MOLDSET_DAPID_KP_MW10,                   // 0x2f13002a, 温度11段P系数
  d_TempCtrlMW1_MOLDSET_DAPID_KP_MW11,                   // 0x2f13002b, 温度12段P系数
  d_TempCtrlMW1_MOLDSET_DAPID_KP_MW12,                   // 0x2f13002c, 温度13段P系数
  d_TempCtrlMW1_MOLDSET_DAPID_KP_MW13,                   // 0x2f13002d, 温度14段P系数
  d_TempCtrlMW1_MOLDSET_DAPID_KP_MW14,                   // 0x2f13002e, 温度15段P系数
  d_TempCtrlMW1_MOLDSET_DAPID_KP_MW15,                   // 0x2f13002f, 温度16段P系数
  d_TempCtrlMW1_MOLDSET_DAPID_TI_MW0,                    // 0x2f130030, 温度1段I系数
  d_TempCtrlMW1_MOLDSET_DAPID_TI_MW1,                    // 0x2f130031, 温度2段I系数
  d_TempCtrlMW1_MOLDSET_DAPID_TI_MW2,                    // 0x2f130032, 温度3段I系数
  d_TempCtrlMW1_MOLDSET_DAPID_TI_MW3,                    // 0x2f130033, 温度4段I系数
  d_TempCtrlMW1_MOLDSET_DAPID_TI_MW4,                    // 0x2f130034, 温度5段I系数
  d_TempCtrlMW1_MOLDSET_DAPID_TI_MW5,                    // 0x2f130035, 温度6段I系数
  d_TempCtrlMW1_MOLDSET_DAPID_TI_MW6,                    // 0x2f130036, 温度7段I系数
  d_TempCtrlMW1_MOLDSET_DAPID_TI_MW7,                    // 0x2f130037, 温度8段I系数
  d_TempCtrlMW1_MOLDSET_DAPID_TI_MW8,                    // 0x2f130038, 温度9段I系数
  d_TempCtrlMW1_MOLDSET_DAPID_TI_MW9,                    // 0x2f130039, 温度10段I系数
  d_TempCtrlMW1_MOLDSET_DAPID_TI_MW10,                   // 0x2f13003a, 温度11段I系数
  d_TempCtrlMW1_MOLDSET_DAPID_TI_MW11,                   // 0x2f13003b, 温度12段I系数
  d_TempCtrlMW1_MOLDSET_DAPID_TI_MW12,                   // 0x2f13003c, 温度13段I系数
  d_TempCtrlMW1_MOLDSET_DAPID_TI_MW13,                   // 0x2f13003d, 温度14段I系数
  d_TempCtrlMW1_MOLDSET_DAPID_TI_MW14,                   // 0x2f13003e, 温度15段I系数
  d_TempCtrlMW1_MOLDSET_DAPID_TI_MW15,                   // 0x2f13003f, 温度16段I系数
  d_TempCtrlMW1_MOLDSET_DAPID_TD_MW0,                    // 0x2f130040, 温度1段D系数
  d_TempCtrlMW1_MOLDSET_DAPID_TD_MW1,                    // 0x2f130041, 温度2段D系数
  d_TempCtrlMW1_MOLDSET_DAPID_TD_MW2,                    // 0x2f130042, 温度3段D系数
  d_TempCtrlMW1_MOLDSET_DAPID_TD_MW3,                    // 0x2f130043, 温度4段D系数
  d_TempCtrlMW1_MOLDSET_DAPID_TD_MW4,                    // 0x2f130044, 温度5段D系数
  d_TempCtrlMW1_MOLDSET_DAPID_TD_MW5,                    // 0x2f130045, 温度6段D系数
  d_TempCtrlMW1_MOLDSET_DAPID_TD_MW6,                    // 0x2f130046, 温度7段D系数
  d_TempCtrlMW1_MOLDSET_DAPID_TD_MW7,                    // 0x2f130047, 温度8段D系数
  d_TempCtrlMW1_MOLDSET_DAPID_TD_MW8,                    // 0x2f130048, 温度9段D系数
  d_TempCtrlMW1_MOLDSET_DAPID_TD_MW9,                    // 0x2f130049, 温度10段D系数
  d_TempCtrlMW1_MOLDSET_DAPID_TD_MW10,                   // 0x2f13004a, 温度11段D系数
  d_TempCtrlMW1_MOLDSET_DAPID_TD_MW11,                   // 0x2f13004b, 温度12段D系数
  d_TempCtrlMW1_MOLDSET_DAPID_TD_MW12,                   // 0x2f13004c, 温度13段D系数
  d_TempCtrlMW1_MOLDSET_DAPID_TD_MW13,                   // 0x2f13004d, 温度14段D系数
  d_TempCtrlMW1_MOLDSET_DAPID_TD_MW14,                   // 0x2f13004e, 温度15段D系数
  d_TempCtrlMW1_MOLDSET_DAPID_TD_MW15,                   // 0x2f13004f, 温度16段D系数
  d_TempCtrlMW1_MOLDSET_DATEMP_RAMP_MW0,                 // 0x2f130050, 温度1段加温缓冲区
  d_TempCtrlMW1_MOLDSET_DATEMP_RAMP_MW1,                 // 0x2f130051, 温度2段加温缓冲区
  d_TempCtrlMW1_MOLDSET_DATEMP_RAMP_MW2,                 // 0x2f130052, 温度3段加温缓冲区
  d_TempCtrlMW1_MOLDSET_DATEMP_RAMP_MW3,                 // 0x2f130053, 温度4段加温缓冲区
  d_TempCtrlMW1_MOLDSET_DATEMP_RAMP_MW4,                 // 0x2f130054, 温度5段加温缓冲区
  d_TempCtrlMW1_MOLDSET_DATEMP_RAMP_MW5,                 // 0x2f130055, 温度6段加温缓冲区
  d_TempCtrlMW1_MOLDSET_DATEMP_RAMP_MW6,                 // 0x2f130056, 温度7段加温缓冲区
  d_TempCtrlMW1_MOLDSET_DATEMP_RAMP_MW7,                 // 0x2f130057, 温度8段加温缓冲区
  d_TempCtrlMW1_MOLDSET_DATEMP_RAMP_MW8,                 // 0x2f130058, 温度9段加温缓冲区
  d_TempCtrlMW1_MOLDSET_DATEMP_RAMP_MW9,                 // 0x2f130059, 温度10段加温缓冲区
  d_TempCtrlMW1_MOLDSET_DATEMP_RAMP_MW10,                // 0x2f13005a, 温度11段加温缓冲区
  d_TempCtrlMW1_MOLDSET_DATEMP_RAMP_MW11,                // 0x2f13005b, 温度12段加温缓冲区
  d_TempCtrlMW1_MOLDSET_DATEMP_RAMP_MW12,                // 0x2f13005c, 温度13段加温缓冲区
  d_TempCtrlMW1_MOLDSET_DATEMP_RAMP_MW13,                // 0x2f13005d, 温度14段加温缓冲区
  d_TempCtrlMW1_MOLDSET_DATEMP_RAMP_MW14,                // 0x2f13005e, 温度15段加温缓冲区
  d_TempCtrlMW1_MOLDSET_DATEMP_RAMP_MW15,                // 0x2f13005f, 温度16段加温缓冲区
  d_TempCtrlMW1_MOLDSET_TM_RESPONSELESS_MW0,             // 0x2f130060, 温度1段输出周期时间
  d_TempCtrlMW1_MOLDSET_TM_RESPONSELESS_MW1,             // 0x2f130061, 温度2段输出周期时间
  d_TempCtrlMW1_MOLDSET_TM_RESPONSELESS_MW2,             // 0x2f130062, 温度3段输出周期时间
  d_TempCtrlMW1_MOLDSET_TM_RESPONSELESS_MW3,             // 0x2f130063, 温度4段输出周期时间
  d_TempCtrlMW1_MOLDSET_TM_RESPONSELESS_MW4,             // 0x2f130064, 温度5段输出周期时间
  d_TempCtrlMW1_MOLDSET_TM_RESPONSELESS_MW5,             // 0x2f130065, 温度6段输出周期时间
  d_TempCtrlMW1_MOLDSET_TM_RESPONSELESS_MW6,             // 0x2f130066, 温度7段输出周期时间
  d_TempCtrlMW1_MOLDSET_TM_RESPONSELESS_MW7,             // 0x2f130067, 温度8段输出周期时间
  d_TempCtrlMW1_MOLDSET_TM_RESPONSELESS_MW8,             // 0x2f130068, 温度9段输出周期时间
  d_TempCtrlMW1_MOLDSET_TM_RESPONSELESS_MW9,             // 0x2f130069, 温度10段输出周期时间
  d_TempCtrlMW1_MOLDSET_TM_RESPONSELESS_MW10,            // 0x2f13006a, 温度11段输出周期时间
  d_TempCtrlMW1_MOLDSET_TM_RESPONSELESS_MW11,            // 0x2f13006b, 温度12段输出周期时间
  d_TempCtrlMW1_MOLDSET_TM_RESPONSELESS_MW12,            // 0x2f13006c, 温度13段输出周期时间
  d_TempCtrlMW1_MOLDSET_TM_RESPONSELESS_MW13,            // 0x2f13006d, 温度14段输出周期时间
  d_TempCtrlMW1_MOLDSET_TM_RESPONSELESS_MW14,            // 0x2f13006e, 温度15段输出周期时间
  d_TempCtrlMW1_MOLDSET_TM_RESPONSELESS_MW15,            // 0x2f13006f, 温度16段输出周期时间
  d_TempCtrlMW1_MOLDSET_DATEMP_MAX_MW0,                  // 0x2f130070, 温度1段上偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MAX_MW1,                  // 0x2f130071, 温度2段上偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MAX_MW2,                  // 0x2f130072, 温度3段上偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MAX_MW3,                  // 0x2f130073, 温度4段上偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MAX_MW4,                  // 0x2f130074, 温度5段上偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MAX_MW5,                  // 0x2f130075, 温度6段上偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MAX_MW6,                  // 0x2f130076, 温度7段上偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MAX_MW7,                  // 0x2f130077, 温度8段上偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MAX_MW8,                  // 0x2f130078, 温度9段上偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MAX_MW9,                  // 0x2f130079, 温度10段上偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MAX_MW10,                 // 0x2f13007a, 温度11段上偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MAX_MW11,                 // 0x2f13007b, 温度12段上偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MAX_MW12,                 // 0x2f13007c, 温度13段上偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MAX_MW13,                 // 0x2f13007d, 温度14段上偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MAX_MW14,                 // 0x2f13007e, 温度15段上偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MAX_MW15,                 // 0x2f13007f, 温度16段上偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MIN_MW0,                  // 0x2f130080, 温度1段下偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MIN_MW1,                  // 0x2f130081, 温度2段下偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MIN_MW2,                  // 0x2f130082, 温度3段下偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MIN_MW3,                  // 0x2f130083, 温度4段下偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MIN_MW4,                  // 0x2f130084, 温度5段下偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MIN_MW5,                  // 0x2f130085, 温度6段下偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MIN_MW6,                  // 0x2f130086, 温度7段下偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MIN_MW7,                  // 0x2f130087, 温度8段下偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MIN_MW8,                  // 0x2f130088, 温度9段下偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MIN_MW9,                  // 0x2f130089, 温度10段下偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MIN_MW10,                 // 0x2f13008a, 温度11段下偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MIN_MW11,                 // 0x2f13008b, 温度12段下偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MIN_MW12,                 // 0x2f13008c, 温度13段下偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MIN_MW13,                 // 0x2f13008d, 温度14段下偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MIN_MW14,                 // 0x2f13008e, 温度15段下偏差
  d_TempCtrlMW1_MOLDSET_DATEMP_MIN_MW15,                 // 0x2f13008f, 温度16段下偏差
  d_TempCtrlMW1_MOLDSET_FL_TKTJ_MW0,                     // 0x2f130090, 温度1段热电偶类型
  d_TempCtrlMW1_MOLDSET_FL_TKTJ_MW1,                     // 0x2f130091, 温度2段热电偶类型
  d_TempCtrlMW1_MOLDSET_FL_TKTJ_MW2,                     // 0x2f130092, 温度3段热电偶类型
  d_TempCtrlMW1_MOLDSET_FL_TKTJ_MW3,                     // 0x2f130093, 温度4段热电偶类型
  d_TempCtrlMW1_MOLDSET_FL_TKTJ_MW4,                     // 0x2f130094, 温度5段热电偶类型
  d_TempCtrlMW1_MOLDSET_FL_TKTJ_MW5,                     // 0x2f130095, 温度6段热电偶类型
  d_TempCtrlMW1_MOLDSET_FL_TKTJ_MW6,                     // 0x2f130096, 温度7段热电偶类型
  d_TempCtrlMW1_MOLDSET_FL_TKTJ_MW7,                     // 0x2f130097, 温度8段热电偶类型
  d_TempCtrlMW1_MOLDSET_FL_TKTJ_MW8,                     // 0x2f130098, 温度9段热电偶类型
  d_TempCtrlMW1_MOLDSET_FL_TKTJ_MW9,                     // 0x2f130099, 温度10段热电偶类型
  d_TempCtrlMW1_MOLDSET_FL_TKTJ_MW10,                    // 0x2f13009a, 温度11段热电偶类型
  d_TempCtrlMW1_MOLDSET_FL_TKTJ_MW11,                    // 0x2f13009b, 温度12段热电偶类型
  d_TempCtrlMW1_MOLDSET_FL_TKTJ_MW12,                    // 0x2f13009c, 温度13段热电偶类型
  d_TempCtrlMW1_MOLDSET_FL_TKTJ_MW13,                    // 0x2f13009d, 温度14段热电偶类型
  d_TempCtrlMW1_MOLDSET_FL_TKTJ_MW14,                    // 0x2f13009e, 温度15段热电偶类型
  d_TempCtrlMW1_MOLDSET_FL_TKTJ_MW15,                    // 0x2f13009f, 温度16段热电偶类型
  d_TempCtrlMW1_MOLDSET_DACENT_HEATERON_MW0,             // 0x2f1300a0, 温度1段手动比例
  d_TempCtrlMW1_MOLDSET_DACENT_HEATERON_MW1,             // 0x2f1300a1, 温度2段手动比例
  d_TempCtrlMW1_MOLDSET_DACENT_HEATERON_MW2,             // 0x2f1300a2, 温度3段手动比例
  d_TempCtrlMW1_MOLDSET_DACENT_HEATERON_MW3,             // 0x2f1300a3, 温度4段手动比例
  d_TempCtrlMW1_MOLDSET_DACENT_HEATERON_MW4,             // 0x2f1300a4, 温度5段手动比例
  d_TempCtrlMW1_MOLDSET_DACENT_HEATERON_MW5,             // 0x2f1300a5, 温度6段手动比例
  d_TempCtrlMW1_MOLDSET_DACENT_HEATERON_MW6,             // 0x2f1300a6, 温度7段手动比例
  d_TempCtrlMW1_MOLDSET_DACENT_HEATERON_MW7,             // 0x2f1300a7, 温度8段手动比例
  d_TempCtrlMW1_MOLDSET_DACENT_HEATERON_MW8,             // 0x2f1300a8, 温度9段手动比例
  d_TempCtrlMW1_MOLDSET_DACENT_HEATERON_MW9,             // 0x2f1300a9, 温度10段手动比例
  d_TempCtrlMW1_MOLDSET_DACENT_HEATERON_MW10,            // 0x2f1300aa, 温度11段手动比例
  d_TempCtrlMW1_MOLDSET_DACENT_HEATERON_MW11,            // 0x2f1300ab, 温度12段手动比例
  d_TempCtrlMW1_MOLDSET_DACENT_HEATERON_MW12,            // 0x2f1300ac, 温度13段手动比例
  d_TempCtrlMW1_MOLDSET_DACENT_HEATERON_MW13,            // 0x2f1300ad, 温度14段手动比例
  d_TempCtrlMW1_MOLDSET_DACENT_HEATERON_MW14,            // 0x2f1300ae, 温度15段手动比例
  d_TempCtrlMW1_MOLDSET_DACENT_HEATERON_MW15,            // 0x2f1300af, 温度16段手动比例
  d_TempCtrlMW1_MOLDSET_FL_TEMPHOLD_MW,                  // 0x2f1300b0, 保温功能
  d_TempCtrlMW1_MOLDSET_DATEMP_HOLD_MW,                  // 0x2f1300b1, 保温温度
  d_TempCtrlMW1_MOLDSET_DATEMP_SCTM_MIN_MW0,             // 0x2f1300b2, 最小扫描周期
  d_TempCtrlMW1_MOLDSET_FL_TEMPUPCHECK_MW,               // 0x2f1300b3, 模温连续加温检查
  d_TempCtrlMW1_MOLDSET_DATEMP_NUM_MW,                   // 0x2f1300b4, 温度段数
  d_TempCtrlMW1_MOLDSET_TM_HEATERWARM_MW,                // 0x2f1300b5, 预温计时
  d_TempCtrlMW1_MOLDSET_FL_USE_MW,                       // 0x2f1300b6, 模温功能
  d_TempCtrlMW1_MOLDSET_FL_TEMP_YH,                      // 0x2f1300b7, 温度优化命令
  d_TempCtrlMW1_MOLDSET_FL_TEMP_YHQZ,                    // 0x2f1300b8, 温度强制优化命令
  d_TempCtrlMW1_MOLDSET_CONFUNC_MW,                      // 0x2f1300b9, 同步升温功能
  d_TempCtrlMW1_MOLDSET_FL_INJ_ALLOW                     // 0x2f1300ba, 射出检测模温温度
};

enum db_TempCtrlMW1_STATE{
  d_TempCtrlMW1_STATE_FL_ERROR0 = 0x2f160000, // 0x2f160000, 警报0
  d_TempCtrlMW1_STATE_FL_ERROR1,              // 0x2f160001, 警报1
  d_TempCtrlMW1_STATE_DATEMP_REAL_MW0,        // 0x2f160002, 温度1段实际值
  d_TempCtrlMW1_STATE_DATEMP_REAL_MW1,        // 0x2f160003, 温度2段实际值
  d_TempCtrlMW1_STATE_DATEMP_REAL_MW2,        // 0x2f160004, 温度3段实际值
  d_TempCtrlMW1_STATE_DATEMP_REAL_MW3,        // 0x2f160005, 温度4段实际值
  d_TempCtrlMW1_STATE_DATEMP_REAL_MW4,        // 0x2f160006, 温度5段实际值
  d_TempCtrlMW1_STATE_DATEMP_REAL_MW5,        // 0x2f160007, 温度6段实际值
  d_TempCtrlMW1_STATE_DATEMP_REAL_MW6,        // 0x2f160008, 温度7段实际值
  d_TempCtrlMW1_STATE_DATEMP_REAL_MW7,        // 0x2f160009, 温度8段实际值
  d_TempCtrlMW1_STATE_DATEMP_REAL_MW8,        // 0x2f16000a, 温度9段实际值
  d_TempCtrlMW1_STATE_DATEMP_REAL_MW9,        // 0x2f16000b, 温度10段实际值
  d_TempCtrlMW1_STATE_DATEMP_REAL_MW10,       // 0x2f16000c, 温度11段实际值
  d_TempCtrlMW1_STATE_DATEMP_REAL_MW11,       // 0x2f16000d, 温度12段实际值
  d_TempCtrlMW1_STATE_DATEMP_REAL_MW12,       // 0x2f16000e, 温度13段实际值
  d_TempCtrlMW1_STATE_DATEMP_REAL_MW13,       // 0x2f16000f, 温度14段实际值
  d_TempCtrlMW1_STATE_DATEMP_REAL_MW14,       // 0x2f160010, 温度15段实际值
  d_TempCtrlMW1_STATE_DATEMP_REAL_MW15,       // 0x2f160011, 温度16段实际值
  d_TempCtrlMW1_STATE_FL_STATUS_MW0,          // 0x2f160012, 温度1段状态
  d_TempCtrlMW1_STATE_FL_STATUS_MW1,          // 0x2f160013, 温度2段状态
  d_TempCtrlMW1_STATE_FL_STATUS_MW2,          // 0x2f160014, 温度3段状态
  d_TempCtrlMW1_STATE_FL_STATUS_MW3,          // 0x2f160015, 温度4段状态
  d_TempCtrlMW1_STATE_FL_STATUS_MW4,          // 0x2f160016, 温度5段状态
  d_TempCtrlMW1_STATE_FL_STATUS_MW5,          // 0x2f160017, 温度6段状态
  d_TempCtrlMW1_STATE_FL_STATUS_MW6,          // 0x2f160018, 温度7段状态
  d_TempCtrlMW1_STATE_FL_STATUS_MW7,          // 0x2f160019, 温度8段状态
  d_TempCtrlMW1_STATE_FL_STATUS_MW8,          // 0x2f16001a, 温度9段状态
  d_TempCtrlMW1_STATE_FL_STATUS_MW9,          // 0x2f16001b, 温度10段状态
  d_TempCtrlMW1_STATE_FL_STATUS_MW10,         // 0x2f16001c, 温度11段状态
  d_TempCtrlMW1_STATE_FL_STATUS_MW11,         // 0x2f16001d, 温度12段状态
  d_TempCtrlMW1_STATE_FL_STATUS_MW12,         // 0x2f16001e, 温度13段状态
  d_TempCtrlMW1_STATE_FL_STATUS_MW13,         // 0x2f16001f, 温度14段状态
  d_TempCtrlMW1_STATE_FL_STATUS_MW14,         // 0x2f160020, 温度15段状态
  d_TempCtrlMW1_STATE_FL_STATUS_MW15,         // 0x2f160021, 温度16段状态
  d_TempCtrlMW1_STATE_OUTPUT_PER_MW0,         // 0x2f160022, 温度1段输出实际值百分比
  d_TempCtrlMW1_STATE_OUTPUT_PER_MW1,         // 0x2f160023, 温度2段输出实际值百分比
  d_TempCtrlMW1_STATE_OUTPUT_PER_MW2,         // 0x2f160024, 温度3段输出实际值百分比
  d_TempCtrlMW1_STATE_OUTPUT_PER_MW3,         // 0x2f160025, 温度4段输出实际值百分比
  d_TempCtrlMW1_STATE_OUTPUT_PER_MW4,         // 0x2f160026, 温度5段输出实际值百分比
  d_TempCtrlMW1_STATE_OUTPUT_PER_MW5,         // 0x2f160027, 温度6段输出实际值百分比
  d_TempCtrlMW1_STATE_OUTPUT_PER_MW6,         // 0x2f160028, 温度7段输出实际值百分比
  d_TempCtrlMW1_STATE_OUTPUT_PER_MW7,         // 0x2f160029, 温度8段输出实际值百分比
  d_TempCtrlMW1_STATE_OUTPUT_PER_MW8,         // 0x2f16002a, 温度9段输出实际值百分比
  d_TempCtrlMW1_STATE_OUTPUT_PER_MW9,         // 0x2f16002b, 温度10段输出实际值百分比
  d_TempCtrlMW1_STATE_OUTPUT_PER_MW10,        // 0x2f16002c, 温度11段输出实际值百分比
  d_TempCtrlMW1_STATE_OUTPUT_PER_MW11,        // 0x2f16002d, 温度12段输出实际值百分比
  d_TempCtrlMW1_STATE_OUTPUT_PER_MW12,        // 0x2f16002e, 温度13段输出实际值百分比
  d_TempCtrlMW1_STATE_OUTPUT_PER_MW13,        // 0x2f16002f, 温度14段输出实际值百分比
  d_TempCtrlMW1_STATE_OUTPUT_PER_MW14,        // 0x2f160030, 温度15段输出实际值百分比
  d_TempCtrlMW1_STATE_OUTPUT_PER_MW15,        // 0x2f160031, 温度16段输出实际值百分比
  d_TempCtrlMW1_STATE_WTEMP_LM73_MW,          // 0x2f160032, 室温的温度
  d_TempCtrlMW1_STATE_FL_HEATERSTATUS_MW,     // 0x2f160033, 电热状态
  d_TempCtrlMW1_STATE_DATEMP_VER_MW,          // 0x2f160034, 温度扩展板E501版本号
  d_TempCtrlMW1_STATE_DATEMP_ERR_MW,          // 0x2f160035, 模温扩展板错误状态
  d_TempCtrlMW1_STATE_WRITE_CNT_E501,         // 0x2f160036, E501通讯测定写入次数
  d_TempCtrlMW1_STATE_READ_CNT_E501,          // 0x2f160037, E501通讯测试读入次数
  d_TempCtrlMW1_STATE_DATEMP_YOUHUAXUQIU,     // 0x2f160038, 温度优化提示开电热的闪烁指示
  d_TempCtrlMW1_STATE_DATEMP_YOUHUAOK,        // 0x2f160039, 温度优化开启与完成标记
  d_TempCtrlMW1_STATE_WSTATERSV6              // 0x2f16003a, 预温倒计时
};

enum db_TempCtrlMW2_PO{
  d_TempCtrlMW2_PO_O_COOL_MW = 0x2f200000  // 0x2f200000, 冷却水
};

enum db_TempCtrlMW2_PI{
  d_TempCtrlMW2_PI_I_HEAT_MW = 0x2f210000  // 0x2f210000, 加热接触器
};

enum db_TempCtrlMW2_MACHSET{
  d_TempCtrlMW2_MACHSET_TEMP_PROTECT_MW = 0x2f220000, // 0x2f220000, 温度保护上限
  d_TempCtrlMW2_MACHSET_TEMP_CONTROL_MW,              // 0x2f220001, 温度控制
  d_TempCtrlMW2_MACHSET_TEMP_CANLINK                  // 0x2f220002, 模温板接线
};

enum db_TempCtrlMW2_MOLDSET{
  d_TempCtrlMW2_MOLDSET_DATEMP_SETTING_MW0 = 0x2f230000, // 0x2f230000, 温度1段设定值
  d_TempCtrlMW2_MOLDSET_DATEMP_SETTING_MW1,              // 0x2f230001, 温度2段设定值
  d_TempCtrlMW2_MOLDSET_DATEMP_SETTING_MW2,              // 0x2f230002, 温度3段设定值
  d_TempCtrlMW2_MOLDSET_DATEMP_SETTING_MW3,              // 0x2f230003, 温度4段设定值
  d_TempCtrlMW2_MOLDSET_DATEMP_SETTING_MW4,              // 0x2f230004, 温度5段设定值
  d_TempCtrlMW2_MOLDSET_DATEMP_SETTING_MW5,              // 0x2f230005, 温度6段设定值
  d_TempCtrlMW2_MOLDSET_DATEMP_SETTING_MW6,              // 0x2f230006, 温度7段设定值
  d_TempCtrlMW2_MOLDSET_DATEMP_SETTING_MW7,              // 0x2f230007, 温度8段设定值
  d_TempCtrlMW2_MOLDSET_DATEMP_SETTING_MW8,              // 0x2f230008, 温度9段设定值
  d_TempCtrlMW2_MOLDSET_DATEMP_SETTING_MW9,              // 0x2f230009, 温度10段设定值
  d_TempCtrlMW2_MOLDSET_DATEMP_SETTING_MW10,             // 0x2f23000a, 温度11段设定值
  d_TempCtrlMW2_MOLDSET_DATEMP_SETTING_MW11,             // 0x2f23000b, 温度12段设定值
  d_TempCtrlMW2_MOLDSET_DATEMP_SETTING_MW12,             // 0x2f23000c, 温度13段设定值
  d_TempCtrlMW2_MOLDSET_DATEMP_SETTING_MW13,             // 0x2f23000d, 温度14段设定值
  d_TempCtrlMW2_MOLDSET_DATEMP_SETTING_MW14,             // 0x2f23000e, 温度15段设定值
  d_TempCtrlMW2_MOLDSET_DATEMP_SETTING_MW15,             // 0x2f23000f, 温度16段设定值
  d_TempCtrlMW2_MOLDSET_DATEMP_FUNC_MW0,                 // 0x2f230010, 温控1段功能选择
  d_TempCtrlMW2_MOLDSET_DATEMP_FUNC_MW1,                 // 0x2f230011, 温控2段功能选择
  d_TempCtrlMW2_MOLDSET_DATEMP_FUNC_MW2,                 // 0x2f230012, 温控3段功能选择
  d_TempCtrlMW2_MOLDSET_DATEMP_FUNC_MW3,                 // 0x2f230013, 温控4段功能选择
  d_TempCtrlMW2_MOLDSET_DATEMP_FUNC_MW4,                 // 0x2f230014, 温控5段功能选择
  d_TempCtrlMW2_MOLDSET_DATEMP_FUNC_MW5,                 // 0x2f230015, 温控6段功能选择
  d_TempCtrlMW2_MOLDSET_DATEMP_FUNC_MW6,                 // 0x2f230016, 温控7段功能选择
  d_TempCtrlMW2_MOLDSET_DATEMP_FUNC_MW7,                 // 0x2f230017, 温控8段功能选择
  d_TempCtrlMW2_MOLDSET_DATEMP_FUNC_MW8,                 // 0x2f230018, 温控9段功能选择
  d_TempCtrlMW2_MOLDSET_DATEMP_FUNC_MW9,                 // 0x2f230019, 温控10段功能选择
  d_TempCtrlMW2_MOLDSET_DATEMP_FUNC_MW10,                // 0x2f23001a, 温控11段功能选择
  d_TempCtrlMW2_MOLDSET_DATEMP_FUNC_MW11,                // 0x2f23001b, 温控12段功能选择
  d_TempCtrlMW2_MOLDSET_DATEMP_FUNC_MW12,                // 0x2f23001c, 温控13段功能选择
  d_TempCtrlMW2_MOLDSET_DATEMP_FUNC_MW13,                // 0x2f23001d, 温控14段功能选择
  d_TempCtrlMW2_MOLDSET_DATEMP_FUNC_MW14,                // 0x2f23001e, 温控15段功能选择
  d_TempCtrlMW2_MOLDSET_DATEMP_FUNC_MW15,                // 0x2f23001f, 温控16段功能选择
  d_TempCtrlMW2_MOLDSET_DAPID_KP_MW0,                    // 0x2f230020, 温度1段P系数
  d_TempCtrlMW2_MOLDSET_DAPID_KP_MW1,                    // 0x2f230021, 温度2段P系数
  d_TempCtrlMW2_MOLDSET_DAPID_KP_MW2,                    // 0x2f230022, 温度3段P系数
  d_TempCtrlMW2_MOLDSET_DAPID_KP_MW3,                    // 0x2f230023, 温度4段P系数
  d_TempCtrlMW2_MOLDSET_DAPID_KP_MW4,                    // 0x2f230024, 温度5段P系数
  d_TempCtrlMW2_MOLDSET_DAPID_KP_MW5,                    // 0x2f230025, 温度6段P系数
  d_TempCtrlMW2_MOLDSET_DAPID_KP_MW6,                    // 0x2f230026, 温度7段P系数
  d_TempCtrlMW2_MOLDSET_DAPID_KP_MW7,                    // 0x2f230027, 温度8段P系数
  d_TempCtrlMW2_MOLDSET_DAPID_KP_MW8,                    // 0x2f230028, 温度9段P系数
  d_TempCtrlMW2_MOLDSET_DAPID_KP_MW9,                    // 0x2f230029, 温度10段P系数
  d_TempCtrlMW2_MOLDSET_DAPID_KP_MW10,                   // 0x2f23002a, 温度11段P系数
  d_TempCtrlMW2_MOLDSET_DAPID_KP_MW11,                   // 0x2f23002b, 温度12段P系数
  d_TempCtrlMW2_MOLDSET_DAPID_KP_MW12,                   // 0x2f23002c, 温度13段P系数
  d_TempCtrlMW2_MOLDSET_DAPID_KP_MW13,                   // 0x2f23002d, 温度14段P系数
  d_TempCtrlMW2_MOLDSET_DAPID_KP_MW14,                   // 0x2f23002e, 温度15段P系数
  d_TempCtrlMW2_MOLDSET_DAPID_KP_MW15,                   // 0x2f23002f, 温度16段P系数
  d_TempCtrlMW2_MOLDSET_DAPID_TI_MW0,                    // 0x2f230030, 温度1段I系数
  d_TempCtrlMW2_MOLDSET_DAPID_TI_MW1,                    // 0x2f230031, 温度2段I系数
  d_TempCtrlMW2_MOLDSET_DAPID_TI_MW2,                    // 0x2f230032, 温度3段I系数
  d_TempCtrlMW2_MOLDSET_DAPID_TI_MW3,                    // 0x2f230033, 温度4段I系数
  d_TempCtrlMW2_MOLDSET_DAPID_TI_MW4,                    // 0x2f230034, 温度5段I系数
  d_TempCtrlMW2_MOLDSET_DAPID_TI_MW5,                    // 0x2f230035, 温度6段I系数
  d_TempCtrlMW2_MOLDSET_DAPID_TI_MW6,                    // 0x2f230036, 温度7段I系数
  d_TempCtrlMW2_MOLDSET_DAPID_TI_MW7,                    // 0x2f230037, 温度8段I系数
  d_TempCtrlMW2_MOLDSET_DAPID_TI_MW8,                    // 0x2f230038, 温度9段I系数
  d_TempCtrlMW2_MOLDSET_DAPID_TI_MW9,                    // 0x2f230039, 温度10段I系数
  d_TempCtrlMW2_MOLDSET_DAPID_TI_MW10,                   // 0x2f23003a, 温度11段I系数
  d_TempCtrlMW2_MOLDSET_DAPID_TI_MW11,                   // 0x2f23003b, 温度12段I系数
  d_TempCtrlMW2_MOLDSET_DAPID_TI_MW12,                   // 0x2f23003c, 温度13段I系数
  d_TempCtrlMW2_MOLDSET_DAPID_TI_MW13,                   // 0x2f23003d, 温度14段I系数
  d_TempCtrlMW2_MOLDSET_DAPID_TI_MW14,                   // 0x2f23003e, 温度15段I系数
  d_TempCtrlMW2_MOLDSET_DAPID_TI_MW15,                   // 0x2f23003f, 温度16段I系数
  d_TempCtrlMW2_MOLDSET_DAPID_TD_MW0,                    // 0x2f230040, 温度1段D系数
  d_TempCtrlMW2_MOLDSET_DAPID_TD_MW1,                    // 0x2f230041, 温度2段D系数
  d_TempCtrlMW2_MOLDSET_DAPID_TD_MW2,                    // 0x2f230042, 温度3段D系数
  d_TempCtrlMW2_MOLDSET_DAPID_TD_MW3,                    // 0x2f230043, 温度4段D系数
  d_TempCtrlMW2_MOLDSET_DAPID_TD_MW4,                    // 0x2f230044, 温度5段D系数
  d_TempCtrlMW2_MOLDSET_DAPID_TD_MW5,                    // 0x2f230045, 温度6段D系数
  d_TempCtrlMW2_MOLDSET_DAPID_TD_MW6,                    // 0x2f230046, 温度7段D系数
  d_TempCtrlMW2_MOLDSET_DAPID_TD_MW7,                    // 0x2f230047, 温度8段D系数
  d_TempCtrlMW2_MOLDSET_DAPID_TD_MW8,                    // 0x2f230048, 温度9段D系数
  d_TempCtrlMW2_MOLDSET_DAPID_TD_MW9,                    // 0x2f230049, 温度10段D系数
  d_TempCtrlMW2_MOLDSET_DAPID_TD_MW10,                   // 0x2f23004a, 温度11段D系数
  d_TempCtrlMW2_MOLDSET_DAPID_TD_MW11,                   // 0x2f23004b, 温度12段D系数
  d_TempCtrlMW2_MOLDSET_DAPID_TD_MW12,                   // 0x2f23004c, 温度13段D系数
  d_TempCtrlMW2_MOLDSET_DAPID_TD_MW13,                   // 0x2f23004d, 温度14段D系数
  d_TempCtrlMW2_MOLDSET_DAPID_TD_MW14,                   // 0x2f23004e, 温度15段D系数
  d_TempCtrlMW2_MOLDSET_DAPID_TD_MW15,                   // 0x2f23004f, 温度16段D系数
  d_TempCtrlMW2_MOLDSET_DATEMP_RAMP_MW0,                 // 0x2f230050, 温度1段加温缓冲区
  d_TempCtrlMW2_MOLDSET_DATEMP_RAMP_MW1,                 // 0x2f230051, 温度2段加温缓冲区
  d_TempCtrlMW2_MOLDSET_DATEMP_RAMP_MW2,                 // 0x2f230052, 温度3段加温缓冲区
  d_TempCtrlMW2_MOLDSET_DATEMP_RAMP_MW3,                 // 0x2f230053, 温度4段加温缓冲区
  d_TempCtrlMW2_MOLDSET_DATEMP_RAMP_MW4,                 // 0x2f230054, 温度5段加温缓冲区
  d_TempCtrlMW2_MOLDSET_DATEMP_RAMP_MW5,                 // 0x2f230055, 温度6段加温缓冲区
  d_TempCtrlMW2_MOLDSET_DATEMP_RAMP_MW6,                 // 0x2f230056, 温度7段加温缓冲区
  d_TempCtrlMW2_MOLDSET_DATEMP_RAMP_MW7,                 // 0x2f230057, 温度8段加温缓冲区
  d_TempCtrlMW2_MOLDSET_DATEMP_RAMP_MW8,                 // 0x2f230058, 温度9段加温缓冲区
  d_TempCtrlMW2_MOLDSET_DATEMP_RAMP_MW9,                 // 0x2f230059, 温度10段加温缓冲区
  d_TempCtrlMW2_MOLDSET_DATEMP_RAMP_MW10,                // 0x2f23005a, 温度11段加温缓冲区
  d_TempCtrlMW2_MOLDSET_DATEMP_RAMP_MW11,                // 0x2f23005b, 温度12段加温缓冲区
  d_TempCtrlMW2_MOLDSET_DATEMP_RAMP_MW12,                // 0x2f23005c, 温度13段加温缓冲区
  d_TempCtrlMW2_MOLDSET_DATEMP_RAMP_MW13,                // 0x2f23005d, 温度14段加温缓冲区
  d_TempCtrlMW2_MOLDSET_DATEMP_RAMP_MW14,                // 0x2f23005e, 温度15段加温缓冲区
  d_TempCtrlMW2_MOLDSET_DATEMP_RAMP_MW15,                // 0x2f23005f, 温度16段加温缓冲区
  d_TempCtrlMW2_MOLDSET_TM_RESPONSELESS_MW0,             // 0x2f230060, 温度1段输出周期时间
  d_TempCtrlMW2_MOLDSET_TM_RESPONSELESS_MW1,             // 0x2f230061, 温度2段输出周期时间
  d_TempCtrlMW2_MOLDSET_TM_RESPONSELESS_MW2,             // 0x2f230062, 温度3段输出周期时间
  d_TempCtrlMW2_MOLDSET_TM_RESPONSELESS_MW3,             // 0x2f230063, 温度4段输出周期时间
  d_TempCtrlMW2_MOLDSET_TM_RESPONSELESS_MW4,             // 0x2f230064, 温度5段输出周期时间
  d_TempCtrlMW2_MOLDSET_TM_RESPONSELESS_MW5,             // 0x2f230065, 温度6段输出周期时间
  d_TempCtrlMW2_MOLDSET_TM_RESPONSELESS_MW6,             // 0x2f230066, 温度7段输出周期时间
  d_TempCtrlMW2_MOLDSET_TM_RESPONSELESS_MW7,             // 0x2f230067, 温度8段输出周期时间
  d_TempCtrlMW2_MOLDSET_TM_RESPONSELESS_MW8,             // 0x2f230068, 温度9段输出周期时间
  d_TempCtrlMW2_MOLDSET_TM_RESPONSELESS_MW9,             // 0x2f230069, 温度10段输出周期时间
  d_TempCtrlMW2_MOLDSET_TM_RESPONSELESS_MW10,            // 0x2f23006a, 温度11段输出周期时间
  d_TempCtrlMW2_MOLDSET_TM_RESPONSELESS_MW11,            // 0x2f23006b, 温度12段输出周期时间
  d_TempCtrlMW2_MOLDSET_TM_RESPONSELESS_MW12,            // 0x2f23006c, 温度13段输出周期时间
  d_TempCtrlMW2_MOLDSET_TM_RESPONSELESS_MW13,            // 0x2f23006d, 温度14段输出周期时间
  d_TempCtrlMW2_MOLDSET_TM_RESPONSELESS_MW14,            // 0x2f23006e, 温度15段输出周期时间
  d_TempCtrlMW2_MOLDSET_TM_RESPONSELESS_MW15,            // 0x2f23006f, 温度16段输出周期时间
  d_TempCtrlMW2_MOLDSET_DATEMP_MAX_MW0,                  // 0x2f230070, 温度1段上偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MAX_MW1,                  // 0x2f230071, 温度2段上偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MAX_MW2,                  // 0x2f230072, 温度3段上偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MAX_MW3,                  // 0x2f230073, 温度4段上偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MAX_MW4,                  // 0x2f230074, 温度5段上偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MAX_MW5,                  // 0x2f230075, 温度6段上偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MAX_MW6,                  // 0x2f230076, 温度7段上偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MAX_MW7,                  // 0x2f230077, 温度8段上偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MAX_MW8,                  // 0x2f230078, 温度9段上偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MAX_MW9,                  // 0x2f230079, 温度10段上偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MAX_MW10,                 // 0x2f23007a, 温度11段上偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MAX_MW11,                 // 0x2f23007b, 温度12段上偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MAX_MW12,                 // 0x2f23007c, 温度13段上偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MAX_MW13,                 // 0x2f23007d, 温度14段上偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MAX_MW14,                 // 0x2f23007e, 温度15段上偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MAX_MW15,                 // 0x2f23007f, 温度16段上偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MIN_MW0,                  // 0x2f230080, 温度1段下偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MIN_MW1,                  // 0x2f230081, 温度2段下偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MIN_MW2,                  // 0x2f230082, 温度3段下偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MIN_MW3,                  // 0x2f230083, 温度4段下偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MIN_MW4,                  // 0x2f230084, 温度5段下偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MIN_MW5,                  // 0x2f230085, 温度6段下偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MIN_MW6,                  // 0x2f230086, 温度7段下偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MIN_MW7,                  // 0x2f230087, 温度8段下偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MIN_MW8,                  // 0x2f230088, 温度9段下偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MIN_MW9,                  // 0x2f230089, 温度10段下偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MIN_MW10,                 // 0x2f23008a, 温度11段下偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MIN_MW11,                 // 0x2f23008b, 温度12段下偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MIN_MW12,                 // 0x2f23008c, 温度13段下偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MIN_MW13,                 // 0x2f23008d, 温度14段下偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MIN_MW14,                 // 0x2f23008e, 温度15段下偏差
  d_TempCtrlMW2_MOLDSET_DATEMP_MIN_MW15,                 // 0x2f23008f, 温度16段下偏差
  d_TempCtrlMW2_MOLDSET_FL_TKTJ_MW0,                     // 0x2f230090, 温度1段热电偶类型
  d_TempCtrlMW2_MOLDSET_FL_TKTJ_MW1,                     // 0x2f230091, 温度2段热电偶类型
  d_TempCtrlMW2_MOLDSET_FL_TKTJ_MW2,                     // 0x2f230092, 温度3段热电偶类型
  d_TempCtrlMW2_MOLDSET_FL_TKTJ_MW3,                     // 0x2f230093, 温度4段热电偶类型
  d_TempCtrlMW2_MOLDSET_FL_TKTJ_MW4,                     // 0x2f230094, 温度5段热电偶类型
  d_TempCtrlMW2_MOLDSET_FL_TKTJ_MW5,                     // 0x2f230095, 温度6段热电偶类型
  d_TempCtrlMW2_MOLDSET_FL_TKTJ_MW6,                     // 0x2f230096, 温度7段热电偶类型
  d_TempCtrlMW2_MOLDSET_FL_TKTJ_MW7,                     // 0x2f230097, 温度8段热电偶类型
  d_TempCtrlMW2_MOLDSET_FL_TKTJ_MW8,                     // 0x2f230098, 温度9段热电偶类型
  d_TempCtrlMW2_MOLDSET_FL_TKTJ_MW9,                     // 0x2f230099, 温度10段热电偶类型
  d_TempCtrlMW2_MOLDSET_FL_TKTJ_MW10,                    // 0x2f23009a, 温度11段热电偶类型
  d_TempCtrlMW2_MOLDSET_FL_TKTJ_MW11,                    // 0x2f23009b, 温度12段热电偶类型
  d_TempCtrlMW2_MOLDSET_FL_TKTJ_MW12,                    // 0x2f23009c, 温度13段热电偶类型
  d_TempCtrlMW2_MOLDSET_FL_TKTJ_MW13,                    // 0x2f23009d, 温度14段热电偶类型
  d_TempCtrlMW2_MOLDSET_FL_TKTJ_MW14,                    // 0x2f23009e, 温度15段热电偶类型
  d_TempCtrlMW2_MOLDSET_FL_TKTJ_MW15,                    // 0x2f23009f, 温度16段热电偶类型
  d_TempCtrlMW2_MOLDSET_DACENT_HEATERON_MW0,             // 0x2f2300a0, 温度1段手动比例
  d_TempCtrlMW2_MOLDSET_DACENT_HEATERON_MW1,             // 0x2f2300a1, 温度2段手动比例
  d_TempCtrlMW2_MOLDSET_DACENT_HEATERON_MW2,             // 0x2f2300a2, 温度3段手动比例
  d_TempCtrlMW2_MOLDSET_DACENT_HEATERON_MW3,             // 0x2f2300a3, 温度4段手动比例
  d_TempCtrlMW2_MOLDSET_DACENT_HEATERON_MW4,             // 0x2f2300a4, 温度5段手动比例
  d_TempCtrlMW2_MOLDSET_DACENT_HEATERON_MW5,             // 0x2f2300a5, 温度6段手动比例
  d_TempCtrlMW2_MOLDSET_DACENT_HEATERON_MW6,             // 0x2f2300a6, 温度7段手动比例
  d_TempCtrlMW2_MOLDSET_DACENT_HEATERON_MW7,             // 0x2f2300a7, 温度8段手动比例
  d_TempCtrlMW2_MOLDSET_DACENT_HEATERON_MW8,             // 0x2f2300a8, 温度9段手动比例
  d_TempCtrlMW2_MOLDSET_DACENT_HEATERON_MW9,             // 0x2f2300a9, 温度10段手动比例
  d_TempCtrlMW2_MOLDSET_DACENT_HEATERON_MW10,            // 0x2f2300aa, 温度11段手动比例
  d_TempCtrlMW2_MOLDSET_DACENT_HEATERON_MW11,            // 0x2f2300ab, 温度12段手动比例
  d_TempCtrlMW2_MOLDSET_DACENT_HEATERON_MW12,            // 0x2f2300ac, 温度13段手动比例
  d_TempCtrlMW2_MOLDSET_DACENT_HEATERON_MW13,            // 0x2f2300ad, 温度14段手动比例
  d_TempCtrlMW2_MOLDSET_DACENT_HEATERON_MW14,            // 0x2f2300ae, 温度15段手动比例
  d_TempCtrlMW2_MOLDSET_DACENT_HEATERON_MW15,            // 0x2f2300af, 温度16段手动比例
  d_TempCtrlMW2_MOLDSET_FL_TEMPHOLD_MW,                  // 0x2f2300b0, 保温功能
  d_TempCtrlMW2_MOLDSET_DATEMP_HOLD_MW,                  // 0x2f2300b1, 保温温度
  d_TempCtrlMW2_MOLDSET_DATEMP_SCTM_MIN_MW0,             // 0x2f2300b2, 最小扫描周期
  d_TempCtrlMW2_MOLDSET_FL_TEMPUPCHECK_MW,               // 0x2f2300b3, 模温连续加温检查
  d_TempCtrlMW2_MOLDSET_DATEMP_NUM_MW,                   // 0x2f2300b4, 温度段数
  d_TempCtrlMW2_MOLDSET_TM_HEATERWARM_MW,                // 0x2f2300b5, 预温计时
  d_TempCtrlMW2_MOLDSET_FL_USE_MW,                       // 0x2f2300b6, 模温功能
  d_TempCtrlMW2_MOLDSET_FL_TEMP_YH,                      // 0x2f2300b7, 温度优化命令
  d_TempCtrlMW2_MOLDSET_FL_TEMP_YHQZ,                    // 0x2f2300b8, 温度强制优化命令
  d_TempCtrlMW2_MOLDSET_CONFUNC_MW,                      // 0x2f2300b9, 同步升温功能
  d_TempCtrlMW2_MOLDSET_FL_INJ_ALLOW                     // 0x2f2300ba, 射出检测模温温度
};

enum db_TempCtrlMW2_STATE{
  d_TempCtrlMW2_STATE_FL_ERROR0 = 0x2f260000, // 0x2f260000, 警报0
  d_TempCtrlMW2_STATE_FL_ERROR1,              // 0x2f260001, 警报1
  d_TempCtrlMW2_STATE_DATEMP_REAL_MW0,        // 0x2f260002, 温度1段实际值
  d_TempCtrlMW2_STATE_DATEMP_REAL_MW1,        // 0x2f260003, 温度2段实际值
  d_TempCtrlMW2_STATE_DATEMP_REAL_MW2,        // 0x2f260004, 温度3段实际值
  d_TempCtrlMW2_STATE_DATEMP_REAL_MW3,        // 0x2f260005, 温度4段实际值
  d_TempCtrlMW2_STATE_DATEMP_REAL_MW4,        // 0x2f260006, 温度5段实际值
  d_TempCtrlMW2_STATE_DATEMP_REAL_MW5,        // 0x2f260007, 温度6段实际值
  d_TempCtrlMW2_STATE_DATEMP_REAL_MW6,        // 0x2f260008, 温度7段实际值
  d_TempCtrlMW2_STATE_DATEMP_REAL_MW7,        // 0x2f260009, 温度8段实际值
  d_TempCtrlMW2_STATE_DATEMP_REAL_MW8,        // 0x2f26000a, 温度9段实际值
  d_TempCtrlMW2_STATE_DATEMP_REAL_MW9,        // 0x2f26000b, 温度10段实际值
  d_TempCtrlMW2_STATE_DATEMP_REAL_MW10,       // 0x2f26000c, 温度11段实际值
  d_TempCtrlMW2_STATE_DATEMP_REAL_MW11,       // 0x2f26000d, 温度12段实际值
  d_TempCtrlMW2_STATE_DATEMP_REAL_MW12,       // 0x2f26000e, 温度13段实际值
  d_TempCtrlMW2_STATE_DATEMP_REAL_MW13,       // 0x2f26000f, 温度14段实际值
  d_TempCtrlMW2_STATE_DATEMP_REAL_MW14,       // 0x2f260010, 温度15段实际值
  d_TempCtrlMW2_STATE_DATEMP_REAL_MW15,       // 0x2f260011, 温度16段实际值
  d_TempCtrlMW2_STATE_FL_STATUS_MW0,          // 0x2f260012, 温度1段状态
  d_TempCtrlMW2_STATE_FL_STATUS_MW1,          // 0x2f260013, 温度2段状态
  d_TempCtrlMW2_STATE_FL_STATUS_MW2,          // 0x2f260014, 温度3段状态
  d_TempCtrlMW2_STATE_FL_STATUS_MW3,          // 0x2f260015, 温度4段状态
  d_TempCtrlMW2_STATE_FL_STATUS_MW4,          // 0x2f260016, 温度5段状态
  d_TempCtrlMW2_STATE_FL_STATUS_MW5,          // 0x2f260017, 温度6段状态
  d_TempCtrlMW2_STATE_FL_STATUS_MW6,          // 0x2f260018, 温度7段状态
  d_TempCtrlMW2_STATE_FL_STATUS_MW7,          // 0x2f260019, 温度8段状态
  d_TempCtrlMW2_STATE_FL_STATUS_MW8,          // 0x2f26001a, 温度9段状态
  d_TempCtrlMW2_STATE_FL_STATUS_MW9,          // 0x2f26001b, 温度10段状态
  d_TempCtrlMW2_STATE_FL_STATUS_MW10,         // 0x2f26001c, 温度11段状态
  d_TempCtrlMW2_STATE_FL_STATUS_MW11,         // 0x2f26001d, 温度12段状态
  d_TempCtrlMW2_STATE_FL_STATUS_MW12,         // 0x2f26001e, 温度13段状态
  d_TempCtrlMW2_STATE_FL_STATUS_MW13,         // 0x2f26001f, 温度14段状态
  d_TempCtrlMW2_STATE_FL_STATUS_MW14,         // 0x2f260020, 温度15段状态
  d_TempCtrlMW2_STATE_FL_STATUS_MW15,         // 0x2f260021, 温度16段状态
  d_TempCtrlMW2_STATE_OUTPUT_PER_MW0,         // 0x2f260022, 温度1段输出实际值百分比
  d_TempCtrlMW2_STATE_OUTPUT_PER_MW1,         // 0x2f260023, 温度2段输出实际值百分比
  d_TempCtrlMW2_STATE_OUTPUT_PER_MW2,         // 0x2f260024, 温度3段输出实际值百分比
  d_TempCtrlMW2_STATE_OUTPUT_PER_MW3,         // 0x2f260025, 温度4段输出实际值百分比
  d_TempCtrlMW2_STATE_OUTPUT_PER_MW4,         // 0x2f260026, 温度5段输出实际值百分比
  d_TempCtrlMW2_STATE_OUTPUT_PER_MW5,         // 0x2f260027, 温度6段输出实际值百分比
  d_TempCtrlMW2_STATE_OUTPUT_PER_MW6,         // 0x2f260028, 温度7段输出实际值百分比
  d_TempCtrlMW2_STATE_OUTPUT_PER_MW7,         // 0x2f260029, 温度8段输出实际值百分比
  d_TempCtrlMW2_STATE_OUTPUT_PER_MW8,         // 0x2f26002a, 温度9段输出实际值百分比
  d_TempCtrlMW2_STATE_OUTPUT_PER_MW9,         // 0x2f26002b, 温度10段输出实际值百分比
  d_TempCtrlMW2_STATE_OUTPUT_PER_MW10,        // 0x2f26002c, 温度11段输出实际值百分比
  d_TempCtrlMW2_STATE_OUTPUT_PER_MW11,        // 0x2f26002d, 温度12段输出实际值百分比
  d_TempCtrlMW2_STATE_OUTPUT_PER_MW12,        // 0x2f26002e, 温度13段输出实际值百分比
  d_TempCtrlMW2_STATE_OUTPUT_PER_MW13,        // 0x2f26002f, 温度14段输出实际值百分比
  d_TempCtrlMW2_STATE_OUTPUT_PER_MW14,        // 0x2f260030, 温度15段输出实际值百分比
  d_TempCtrlMW2_STATE_OUTPUT_PER_MW15,        // 0x2f260031, 温度16段输出实际值百分比
  d_TempCtrlMW2_STATE_WTEMP_LM73_MW,          // 0x2f260032, 室温的温度
  d_TempCtrlMW2_STATE_FL_HEATERSTATUS_MW,     // 0x2f260033, 电热状态
  d_TempCtrlMW2_STATE_DATEMP_VER_MW,          // 0x2f260034, 温度扩展板E501版本号
  d_TempCtrlMW2_STATE_DATEMP_ERR_MW,          // 0x2f260035, 模温扩展板错误状态
  d_TempCtrlMW2_STATE_WRITE_CNT_E501,         // 0x2f260036, E501通讯测定写入次数
  d_TempCtrlMW2_STATE_READ_CNT_E501,          // 0x2f260037, E501通讯测试读入次数
  d_TempCtrlMW2_STATE_DATEMP_YOUHUAXUQIU,     // 0x2f260038, 温度优化提示开电热的闪烁指示
  d_TempCtrlMW2_STATE_DATEMP_YOUHUAOK,        // 0x2f260039, 温度优化开启与完成标记
  d_TempCtrlMW2_STATE_WSTATERSV6              // 0x2f26003a, 预温倒计时
};

enum db_TempCtrlMW3_PO{
  d_TempCtrlMW3_PO_O_COOL_MW = 0x2f300000  // 0x2f300000, 冷却水
};

enum db_TempCtrlMW3_PI{
  d_TempCtrlMW3_PI_I_HEAT_MW = 0x2f310000  // 0x2f310000, 加热接触器
};

enum db_TempCtrlMW3_MACHSET{
  d_TempCtrlMW3_MACHSET_TEMP_PROTECT_MW = 0x2f320000, // 0x2f320000, 温度保护上限
  d_TempCtrlMW3_MACHSET_TEMP_CONTROL_MW,              // 0x2f320001, 温度控制
  d_TempCtrlMW3_MACHSET_TEMP_CANLINK                  // 0x2f320002, 模温板接线
};

enum db_TempCtrlMW3_MOLDSET{
  d_TempCtrlMW3_MOLDSET_DATEMP_SETTING_MW0 = 0x2f330000, // 0x2f330000, 温度1段设定值
  d_TempCtrlMW3_MOLDSET_DATEMP_SETTING_MW1,              // 0x2f330001, 温度2段设定值
  d_TempCtrlMW3_MOLDSET_DATEMP_SETTING_MW2,              // 0x2f330002, 温度3段设定值
  d_TempCtrlMW3_MOLDSET_DATEMP_SETTING_MW3,              // 0x2f330003, 温度4段设定值
  d_TempCtrlMW3_MOLDSET_DATEMP_SETTING_MW4,              // 0x2f330004, 温度5段设定值
  d_TempCtrlMW3_MOLDSET_DATEMP_SETTING_MW5,              // 0x2f330005, 温度6段设定值
  d_TempCtrlMW3_MOLDSET_DATEMP_SETTING_MW6,              // 0x2f330006, 温度7段设定值
  d_TempCtrlMW3_MOLDSET_DATEMP_SETTING_MW7,              // 0x2f330007, 温度8段设定值
  d_TempCtrlMW3_MOLDSET_DATEMP_SETTING_MW8,              // 0x2f330008, 温度9段设定值
  d_TempCtrlMW3_MOLDSET_DATEMP_SETTING_MW9,              // 0x2f330009, 温度10段设定值
  d_TempCtrlMW3_MOLDSET_DATEMP_SETTING_MW10,             // 0x2f33000a, 温度11段设定值
  d_TempCtrlMW3_MOLDSET_DATEMP_SETTING_MW11,             // 0x2f33000b, 温度12段设定值
  d_TempCtrlMW3_MOLDSET_DATEMP_SETTING_MW12,             // 0x2f33000c, 温度13段设定值
  d_TempCtrlMW3_MOLDSET_DATEMP_SETTING_MW13,             // 0x2f33000d, 温度14段设定值
  d_TempCtrlMW3_MOLDSET_DATEMP_SETTING_MW14,             // 0x2f33000e, 温度15段设定值
  d_TempCtrlMW3_MOLDSET_DATEMP_SETTING_MW15,             // 0x2f33000f, 温度16段设定值
  d_TempCtrlMW3_MOLDSET_DATEMP_FUNC_MW0,                 // 0x2f330010, 温控1段功能选择
  d_TempCtrlMW3_MOLDSET_DATEMP_FUNC_MW1,                 // 0x2f330011, 温控2段功能选择
  d_TempCtrlMW3_MOLDSET_DATEMP_FUNC_MW2,                 // 0x2f330012, 温控3段功能选择
  d_TempCtrlMW3_MOLDSET_DATEMP_FUNC_MW3,                 // 0x2f330013, 温控4段功能选择
  d_TempCtrlMW3_MOLDSET_DATEMP_FUNC_MW4,                 // 0x2f330014, 温控5段功能选择
  d_TempCtrlMW3_MOLDSET_DATEMP_FUNC_MW5,                 // 0x2f330015, 温控6段功能选择
  d_TempCtrlMW3_MOLDSET_DATEMP_FUNC_MW6,                 // 0x2f330016, 温控7段功能选择
  d_TempCtrlMW3_MOLDSET_DATEMP_FUNC_MW7,                 // 0x2f330017, 温控8段功能选择
  d_TempCtrlMW3_MOLDSET_DATEMP_FUNC_MW8,                 // 0x2f330018, 温控9段功能选择
  d_TempCtrlMW3_MOLDSET_DATEMP_FUNC_MW9,                 // 0x2f330019, 温控10段功能选择
  d_TempCtrlMW3_MOLDSET_DATEMP_FUNC_MW10,                // 0x2f33001a, 温控11段功能选择
  d_TempCtrlMW3_MOLDSET_DATEMP_FUNC_MW11,                // 0x2f33001b, 温控12段功能选择
  d_TempCtrlMW3_MOLDSET_DATEMP_FUNC_MW12,                // 0x2f33001c, 温控13段功能选择
  d_TempCtrlMW3_MOLDSET_DATEMP_FUNC_MW13,                // 0x2f33001d, 温控14段功能选择
  d_TempCtrlMW3_MOLDSET_DATEMP_FUNC_MW14,                // 0x2f33001e, 温控15段功能选择
  d_TempCtrlMW3_MOLDSET_DATEMP_FUNC_MW15,                // 0x2f33001f, 温控16段功能选择
  d_TempCtrlMW3_MOLDSET_DAPID_KP_MW0,                    // 0x2f330020, 温度1段P系数
  d_TempCtrlMW3_MOLDSET_DAPID_KP_MW1,                    // 0x2f330021, 温度2段P系数
  d_TempCtrlMW3_MOLDSET_DAPID_KP_MW2,                    // 0x2f330022, 温度3段P系数
  d_TempCtrlMW3_MOLDSET_DAPID_KP_MW3,                    // 0x2f330023, 温度4段P系数
  d_TempCtrlMW3_MOLDSET_DAPID_KP_MW4,                    // 0x2f330024, 温度5段P系数
  d_TempCtrlMW3_MOLDSET_DAPID_KP_MW5,                    // 0x2f330025, 温度6段P系数
  d_TempCtrlMW3_MOLDSET_DAPID_KP_MW6,                    // 0x2f330026, 温度7段P系数
  d_TempCtrlMW3_MOLDSET_DAPID_KP_MW7,                    // 0x2f330027, 温度8段P系数
  d_TempCtrlMW3_MOLDSET_DAPID_KP_MW8,                    // 0x2f330028, 温度9段P系数
  d_TempCtrlMW3_MOLDSET_DAPID_KP_MW9,                    // 0x2f330029, 温度10段P系数
  d_TempCtrlMW3_MOLDSET_DAPID_KP_MW10,                   // 0x2f33002a, 温度11段P系数
  d_TempCtrlMW3_MOLDSET_DAPID_KP_MW11,                   // 0x2f33002b, 温度12段P系数
  d_TempCtrlMW3_MOLDSET_DAPID_KP_MW12,                   // 0x2f33002c, 温度13段P系数
  d_TempCtrlMW3_MOLDSET_DAPID_KP_MW13,                   // 0x2f33002d, 温度14段P系数
  d_TempCtrlMW3_MOLDSET_DAPID_KP_MW14,                   // 0x2f33002e, 温度15段P系数
  d_TempCtrlMW3_MOLDSET_DAPID_KP_MW15,                   // 0x2f33002f, 温度16段P系数
  d_TempCtrlMW3_MOLDSET_DAPID_TI_MW0,                    // 0x2f330030, 温度1段I系数
  d_TempCtrlMW3_MOLDSET_DAPID_TI_MW1,                    // 0x2f330031, 温度2段I系数
  d_TempCtrlMW3_MOLDSET_DAPID_TI_MW2,                    // 0x2f330032, 温度3段I系数
  d_TempCtrlMW3_MOLDSET_DAPID_TI_MW3,                    // 0x2f330033, 温度4段I系数
  d_TempCtrlMW3_MOLDSET_DAPID_TI_MW4,                    // 0x2f330034, 温度5段I系数
  d_TempCtrlMW3_MOLDSET_DAPID_TI_MW5,                    // 0x2f330035, 温度6段I系数
  d_TempCtrlMW3_MOLDSET_DAPID_TI_MW6,                    // 0x2f330036, 温度7段I系数
  d_TempCtrlMW3_MOLDSET_DAPID_TI_MW7,                    // 0x2f330037, 温度8段I系数
  d_TempCtrlMW3_MOLDSET_DAPID_TI_MW8,                    // 0x2f330038, 温度9段I系数
  d_TempCtrlMW3_MOLDSET_DAPID_TI_MW9,                    // 0x2f330039, 温度10段I系数
  d_TempCtrlMW3_MOLDSET_DAPID_TI_MW10,                   // 0x2f33003a, 温度11段I系数
  d_TempCtrlMW3_MOLDSET_DAPID_TI_MW11,                   // 0x2f33003b, 温度12段I系数
  d_TempCtrlMW3_MOLDSET_DAPID_TI_MW12,                   // 0x2f33003c, 温度13段I系数
  d_TempCtrlMW3_MOLDSET_DAPID_TI_MW13,                   // 0x2f33003d, 温度14段I系数
  d_TempCtrlMW3_MOLDSET_DAPID_TI_MW14,                   // 0x2f33003e, 温度15段I系数
  d_TempCtrlMW3_MOLDSET_DAPID_TI_MW15,                   // 0x2f33003f, 温度16段I系数
  d_TempCtrlMW3_MOLDSET_DAPID_TD_MW0,                    // 0x2f330040, 温度1段D系数
  d_TempCtrlMW3_MOLDSET_DAPID_TD_MW1,                    // 0x2f330041, 温度2段D系数
  d_TempCtrlMW3_MOLDSET_DAPID_TD_MW2,                    // 0x2f330042, 温度3段D系数
  d_TempCtrlMW3_MOLDSET_DAPID_TD_MW3,                    // 0x2f330043, 温度4段D系数
  d_TempCtrlMW3_MOLDSET_DAPID_TD_MW4,                    // 0x2f330044, 温度5段D系数
  d_TempCtrlMW3_MOLDSET_DAPID_TD_MW5,                    // 0x2f330045, 温度6段D系数
  d_TempCtrlMW3_MOLDSET_DAPID_TD_MW6,                    // 0x2f330046, 温度7段D系数
  d_TempCtrlMW3_MOLDSET_DAPID_TD_MW7,                    // 0x2f330047, 温度8段D系数
  d_TempCtrlMW3_MOLDSET_DAPID_TD_MW8,                    // 0x2f330048, 温度9段D系数
  d_TempCtrlMW3_MOLDSET_DAPID_TD_MW9,                    // 0x2f330049, 温度10段D系数
  d_TempCtrlMW3_MOLDSET_DAPID_TD_MW10,                   // 0x2f33004a, 温度11段D系数
  d_TempCtrlMW3_MOLDSET_DAPID_TD_MW11,                   // 0x2f33004b, 温度12段D系数
  d_TempCtrlMW3_MOLDSET_DAPID_TD_MW12,                   // 0x2f33004c, 温度13段D系数
  d_TempCtrlMW3_MOLDSET_DAPID_TD_MW13,                   // 0x2f33004d, 温度14段D系数
  d_TempCtrlMW3_MOLDSET_DAPID_TD_MW14,                   // 0x2f33004e, 温度15段D系数
  d_TempCtrlMW3_MOLDSET_DAPID_TD_MW15,                   // 0x2f33004f, 温度16段D系数
  d_TempCtrlMW3_MOLDSET_DATEMP_RAMP_MW0,                 // 0x2f330050, 温度1段加温缓冲区
  d_TempCtrlMW3_MOLDSET_DATEMP_RAMP_MW1,                 // 0x2f330051, 温度2段加温缓冲区
  d_TempCtrlMW3_MOLDSET_DATEMP_RAMP_MW2,                 // 0x2f330052, 温度3段加温缓冲区
  d_TempCtrlMW3_MOLDSET_DATEMP_RAMP_MW3,                 // 0x2f330053, 温度4段加温缓冲区
  d_TempCtrlMW3_MOLDSET_DATEMP_RAMP_MW4,                 // 0x2f330054, 温度5段加温缓冲区
  d_TempCtrlMW3_MOLDSET_DATEMP_RAMP_MW5,                 // 0x2f330055, 温度6段加温缓冲区
  d_TempCtrlMW3_MOLDSET_DATEMP_RAMP_MW6,                 // 0x2f330056, 温度7段加温缓冲区
  d_TempCtrlMW3_MOLDSET_DATEMP_RAMP_MW7,                 // 0x2f330057, 温度8段加温缓冲区
  d_TempCtrlMW3_MOLDSET_DATEMP_RAMP_MW8,                 // 0x2f330058, 温度9段加温缓冲区
  d_TempCtrlMW3_MOLDSET_DATEMP_RAMP_MW9,                 // 0x2f330059, 温度10段加温缓冲区
  d_TempCtrlMW3_MOLDSET_DATEMP_RAMP_MW10,                // 0x2f33005a, 温度11段加温缓冲区
  d_TempCtrlMW3_MOLDSET_DATEMP_RAMP_MW11,                // 0x2f33005b, 温度12段加温缓冲区
  d_TempCtrlMW3_MOLDSET_DATEMP_RAMP_MW12,                // 0x2f33005c, 温度13段加温缓冲区
  d_TempCtrlMW3_MOLDSET_DATEMP_RAMP_MW13,                // 0x2f33005d, 温度14段加温缓冲区
  d_TempCtrlMW3_MOLDSET_DATEMP_RAMP_MW14,                // 0x2f33005e, 温度15段加温缓冲区
  d_TempCtrlMW3_MOLDSET_DATEMP_RAMP_MW15,                // 0x2f33005f, 温度16段加温缓冲区
  d_TempCtrlMW3_MOLDSET_TM_RESPONSELESS_MW0,             // 0x2f330060, 温度1段输出周期时间
  d_TempCtrlMW3_MOLDSET_TM_RESPONSELESS_MW1,             // 0x2f330061, 温度2段输出周期时间
  d_TempCtrlMW3_MOLDSET_TM_RESPONSELESS_MW2,             // 0x2f330062, 温度3段输出周期时间
  d_TempCtrlMW3_MOLDSET_TM_RESPONSELESS_MW3,             // 0x2f330063, 温度4段输出周期时间
  d_TempCtrlMW3_MOLDSET_TM_RESPONSELESS_MW4,             // 0x2f330064, 温度5段输出周期时间
  d_TempCtrlMW3_MOLDSET_TM_RESPONSELESS_MW5,             // 0x2f330065, 温度6段输出周期时间
  d_TempCtrlMW3_MOLDSET_TM_RESPONSELESS_MW6,             // 0x2f330066, 温度7段输出周期时间
  d_TempCtrlMW3_MOLDSET_TM_RESPONSELESS_MW7,             // 0x2f330067, 温度8段输出周期时间
  d_TempCtrlMW3_MOLDSET_TM_RESPONSELESS_MW8,             // 0x2f330068, 温度9段输出周期时间
  d_TempCtrlMW3_MOLDSET_TM_RESPONSELESS_MW9,             // 0x2f330069, 温度10段输出周期时间
  d_TempCtrlMW3_MOLDSET_TM_RESPONSELESS_MW10,            // 0x2f33006a, 温度11段输出周期时间
  d_TempCtrlMW3_MOLDSET_TM_RESPONSELESS_MW11,            // 0x2f33006b, 温度12段输出周期时间
  d_TempCtrlMW3_MOLDSET_TM_RESPONSELESS_MW12,            // 0x2f33006c, 温度13段输出周期时间
  d_TempCtrlMW3_MOLDSET_TM_RESPONSELESS_MW13,            // 0x2f33006d, 温度14段输出周期时间
  d_TempCtrlMW3_MOLDSET_TM_RESPONSELESS_MW14,            // 0x2f33006e, 温度15段输出周期时间
  d_TempCtrlMW3_MOLDSET_TM_RESPONSELESS_MW15,            // 0x2f33006f, 温度16段输出周期时间
  d_TempCtrlMW3_MOLDSET_DATEMP_MAX_MW0,                  // 0x2f330070, 温度1段上偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MAX_MW1,                  // 0x2f330071, 温度2段上偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MAX_MW2,                  // 0x2f330072, 温度3段上偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MAX_MW3,                  // 0x2f330073, 温度4段上偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MAX_MW4,                  // 0x2f330074, 温度5段上偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MAX_MW5,                  // 0x2f330075, 温度6段上偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MAX_MW6,                  // 0x2f330076, 温度7段上偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MAX_MW7,                  // 0x2f330077, 温度8段上偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MAX_MW8,                  // 0x2f330078, 温度9段上偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MAX_MW9,                  // 0x2f330079, 温度10段上偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MAX_MW10,                 // 0x2f33007a, 温度11段上偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MAX_MW11,                 // 0x2f33007b, 温度12段上偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MAX_MW12,                 // 0x2f33007c, 温度13段上偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MAX_MW13,                 // 0x2f33007d, 温度14段上偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MAX_MW14,                 // 0x2f33007e, 温度15段上偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MAX_MW15,                 // 0x2f33007f, 温度16段上偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MIN_MW0,                  // 0x2f330080, 温度1段下偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MIN_MW1,                  // 0x2f330081, 温度2段下偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MIN_MW2,                  // 0x2f330082, 温度3段下偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MIN_MW3,                  // 0x2f330083, 温度4段下偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MIN_MW4,                  // 0x2f330084, 温度5段下偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MIN_MW5,                  // 0x2f330085, 温度6段下偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MIN_MW6,                  // 0x2f330086, 温度7段下偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MIN_MW7,                  // 0x2f330087, 温度8段下偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MIN_MW8,                  // 0x2f330088, 温度9段下偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MIN_MW9,                  // 0x2f330089, 温度10段下偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MIN_MW10,                 // 0x2f33008a, 温度11段下偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MIN_MW11,                 // 0x2f33008b, 温度12段下偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MIN_MW12,                 // 0x2f33008c, 温度13段下偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MIN_MW13,                 // 0x2f33008d, 温度14段下偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MIN_MW14,                 // 0x2f33008e, 温度15段下偏差
  d_TempCtrlMW3_MOLDSET_DATEMP_MIN_MW15,                 // 0x2f33008f, 温度16段下偏差
  d_TempCtrlMW3_MOLDSET_FL_TKTJ_MW0,                     // 0x2f330090, 温度1段热电偶类型
  d_TempCtrlMW3_MOLDSET_FL_TKTJ_MW1,                     // 0x2f330091, 温度2段热电偶类型
  d_TempCtrlMW3_MOLDSET_FL_TKTJ_MW2,                     // 0x2f330092, 温度3段热电偶类型
  d_TempCtrlMW3_MOLDSET_FL_TKTJ_MW3,                     // 0x2f330093, 温度4段热电偶类型
  d_TempCtrlMW3_MOLDSET_FL_TKTJ_MW4,                     // 0x2f330094, 温度5段热电偶类型
  d_TempCtrlMW3_MOLDSET_FL_TKTJ_MW5,                     // 0x2f330095, 温度6段热电偶类型
  d_TempCtrlMW3_MOLDSET_FL_TKTJ_MW6,                     // 0x2f330096, 温度7段热电偶类型
  d_TempCtrlMW3_MOLDSET_FL_TKTJ_MW7,                     // 0x2f330097, 温度8段热电偶类型
  d_TempCtrlMW3_MOLDSET_FL_TKTJ_MW8,                     // 0x2f330098, 温度9段热电偶类型
  d_TempCtrlMW3_MOLDSET_FL_TKTJ_MW9,                     // 0x2f330099, 温度10段热电偶类型
  d_TempCtrlMW3_MOLDSET_FL_TKTJ_MW10,                    // 0x2f33009a, 温度11段热电偶类型
  d_TempCtrlMW3_MOLDSET_FL_TKTJ_MW11,                    // 0x2f33009b, 温度12段热电偶类型
  d_TempCtrlMW3_MOLDSET_FL_TKTJ_MW12,                    // 0x2f33009c, 温度13段热电偶类型
  d_TempCtrlMW3_MOLDSET_FL_TKTJ_MW13,                    // 0x2f33009d, 温度14段热电偶类型
  d_TempCtrlMW3_MOLDSET_FL_TKTJ_MW14,                    // 0x2f33009e, 温度15段热电偶类型
  d_TempCtrlMW3_MOLDSET_FL_TKTJ_MW15,                    // 0x2f33009f, 温度16段热电偶类型
  d_TempCtrlMW3_MOLDSET_DACENT_HEATERON_MW0,             // 0x2f3300a0, 温度1段手动比例
  d_TempCtrlMW3_MOLDSET_DACENT_HEATERON_MW1,             // 0x2f3300a1, 温度2段手动比例
  d_TempCtrlMW3_MOLDSET_DACENT_HEATERON_MW2,             // 0x2f3300a2, 温度3段手动比例
  d_TempCtrlMW3_MOLDSET_DACENT_HEATERON_MW3,             // 0x2f3300a3, 温度4段手动比例
  d_TempCtrlMW3_MOLDSET_DACENT_HEATERON_MW4,             // 0x2f3300a4, 温度5段手动比例
  d_TempCtrlMW3_MOLDSET_DACENT_HEATERON_MW5,             // 0x2f3300a5, 温度6段手动比例
  d_TempCtrlMW3_MOLDSET_DACENT_HEATERON_MW6,             // 0x2f3300a6, 温度7段手动比例
  d_TempCtrlMW3_MOLDSET_DACENT_HEATERON_MW7,             // 0x2f3300a7, 温度8段手动比例
  d_TempCtrlMW3_MOLDSET_DACENT_HEATERON_MW8,             // 0x2f3300a8, 温度9段手动比例
  d_TempCtrlMW3_MOLDSET_DACENT_HEATERON_MW9,             // 0x2f3300a9, 温度10段手动比例
  d_TempCtrlMW3_MOLDSET_DACENT_HEATERON_MW10,            // 0x2f3300aa, 温度11段手动比例
  d_TempCtrlMW3_MOLDSET_DACENT_HEATERON_MW11,            // 0x2f3300ab, 温度12段手动比例
  d_TempCtrlMW3_MOLDSET_DACENT_HEATERON_MW12,            // 0x2f3300ac, 温度13段手动比例
  d_TempCtrlMW3_MOLDSET_DACENT_HEATERON_MW13,            // 0x2f3300ad, 温度14段手动比例
  d_TempCtrlMW3_MOLDSET_DACENT_HEATERON_MW14,            // 0x2f3300ae, 温度15段手动比例
  d_TempCtrlMW3_MOLDSET_DACENT_HEATERON_MW15,            // 0x2f3300af, 温度16段手动比例
  d_TempCtrlMW3_MOLDSET_FL_TEMPHOLD_MW,                  // 0x2f3300b0, 保温功能
  d_TempCtrlMW3_MOLDSET_DATEMP_HOLD_MW,                  // 0x2f3300b1, 保温温度
  d_TempCtrlMW3_MOLDSET_DATEMP_SCTM_MIN_MW0,             // 0x2f3300b2, 最小扫描周期
  d_TempCtrlMW3_MOLDSET_FL_TEMPUPCHECK_MW,               // 0x2f3300b3, 模温连续加温检查
  d_TempCtrlMW3_MOLDSET_DATEMP_NUM_MW,                   // 0x2f3300b4, 温度段数
  d_TempCtrlMW3_MOLDSET_TM_HEATERWARM_MW,                // 0x2f3300b5, 预温计时
  d_TempCtrlMW3_MOLDSET_FL_USE_MW,                       // 0x2f3300b6, 模温功能
  d_TempCtrlMW3_MOLDSET_FL_TEMP_YH,                      // 0x2f3300b7, 温度优化命令
  d_TempCtrlMW3_MOLDSET_FL_TEMP_YHQZ,                    // 0x2f3300b8, 温度强制优化命令
  d_TempCtrlMW3_MOLDSET_CONFUNC_MW,                      // 0x2f3300b9, 同步升温功能
  d_TempCtrlMW3_MOLDSET_FL_INJ_ALLOW                     // 0x2f3300ba, 射出检测模温温度
};

enum db_TempCtrlMW3_STATE{
  d_TempCtrlMW3_STATE_FL_ERROR0 = 0x2f360000, // 0x2f360000, 警报0
  d_TempCtrlMW3_STATE_FL_ERROR1,              // 0x2f360001, 警报1
  d_TempCtrlMW3_STATE_DATEMP_REAL_MW0,        // 0x2f360002, 温度1段实际值
  d_TempCtrlMW3_STATE_DATEMP_REAL_MW1,        // 0x2f360003, 温度2段实际值
  d_TempCtrlMW3_STATE_DATEMP_REAL_MW2,        // 0x2f360004, 温度3段实际值
  d_TempCtrlMW3_STATE_DATEMP_REAL_MW3,        // 0x2f360005, 温度4段实际值
  d_TempCtrlMW3_STATE_DATEMP_REAL_MW4,        // 0x2f360006, 温度5段实际值
  d_TempCtrlMW3_STATE_DATEMP_REAL_MW5,        // 0x2f360007, 温度6段实际值
  d_TempCtrlMW3_STATE_DATEMP_REAL_MW6,        // 0x2f360008, 温度7段实际值
  d_TempCtrlMW3_STATE_DATEMP_REAL_MW7,        // 0x2f360009, 温度8段实际值
  d_TempCtrlMW3_STATE_DATEMP_REAL_MW8,        // 0x2f36000a, 温度9段实际值
  d_TempCtrlMW3_STATE_DATEMP_REAL_MW9,        // 0x2f36000b, 温度10段实际值
  d_TempCtrlMW3_STATE_DATEMP_REAL_MW10,       // 0x2f36000c, 温度11段实际值
  d_TempCtrlMW3_STATE_DATEMP_REAL_MW11,       // 0x2f36000d, 温度12段实际值
  d_TempCtrlMW3_STATE_DATEMP_REAL_MW12,       // 0x2f36000e, 温度13段实际值
  d_TempCtrlMW3_STATE_DATEMP_REAL_MW13,       // 0x2f36000f, 温度14段实际值
  d_TempCtrlMW3_STATE_DATEMP_REAL_MW14,       // 0x2f360010, 温度15段实际值
  d_TempCtrlMW3_STATE_DATEMP_REAL_MW15,       // 0x2f360011, 温度16段实际值
  d_TempCtrlMW3_STATE_FL_STATUS_MW0,          // 0x2f360012, 温度1段状态
  d_TempCtrlMW3_STATE_FL_STATUS_MW1,          // 0x2f360013, 温度2段状态
  d_TempCtrlMW3_STATE_FL_STATUS_MW2,          // 0x2f360014, 温度3段状态
  d_TempCtrlMW3_STATE_FL_STATUS_MW3,          // 0x2f360015, 温度4段状态
  d_TempCtrlMW3_STATE_FL_STATUS_MW4,          // 0x2f360016, 温度5段状态
  d_TempCtrlMW3_STATE_FL_STATUS_MW5,          // 0x2f360017, 温度6段状态
  d_TempCtrlMW3_STATE_FL_STATUS_MW6,          // 0x2f360018, 温度7段状态
  d_TempCtrlMW3_STATE_FL_STATUS_MW7,          // 0x2f360019, 温度8段状态
  d_TempCtrlMW3_STATE_FL_STATUS_MW8,          // 0x2f36001a, 温度9段状态
  d_TempCtrlMW3_STATE_FL_STATUS_MW9,          // 0x2f36001b, 温度10段状态
  d_TempCtrlMW3_STATE_FL_STATUS_MW10,         // 0x2f36001c, 温度11段状态
  d_TempCtrlMW3_STATE_FL_STATUS_MW11,         // 0x2f36001d, 温度12段状态
  d_TempCtrlMW3_STATE_FL_STATUS_MW12,         // 0x2f36001e, 温度13段状态
  d_TempCtrlMW3_STATE_FL_STATUS_MW13,         // 0x2f36001f, 温度14段状态
  d_TempCtrlMW3_STATE_FL_STATUS_MW14,         // 0x2f360020, 温度15段状态
  d_TempCtrlMW3_STATE_FL_STATUS_MW15,         // 0x2f360021, 温度16段状态
  d_TempCtrlMW3_STATE_OUTPUT_PER_MW0,         // 0x2f360022, 温度1段输出实际值百分比
  d_TempCtrlMW3_STATE_OUTPUT_PER_MW1,         // 0x2f360023, 温度2段输出实际值百分比
  d_TempCtrlMW3_STATE_OUTPUT_PER_MW2,         // 0x2f360024, 温度3段输出实际值百分比
  d_TempCtrlMW3_STATE_OUTPUT_PER_MW3,         // 0x2f360025, 温度4段输出实际值百分比
  d_TempCtrlMW3_STATE_OUTPUT_PER_MW4,         // 0x2f360026, 温度5段输出实际值百分比
  d_TempCtrlMW3_STATE_OUTPUT_PER_MW5,         // 0x2f360027, 温度6段输出实际值百分比
  d_TempCtrlMW3_STATE_OUTPUT_PER_MW6,         // 0x2f360028, 温度7段输出实际值百分比
  d_TempCtrlMW3_STATE_OUTPUT_PER_MW7,         // 0x2f360029, 温度8段输出实际值百分比
  d_TempCtrlMW3_STATE_OUTPUT_PER_MW8,         // 0x2f36002a, 温度9段输出实际值百分比
  d_TempCtrlMW3_STATE_OUTPUT_PER_MW9,         // 0x2f36002b, 温度10段输出实际值百分比
  d_TempCtrlMW3_STATE_OUTPUT_PER_MW10,        // 0x2f36002c, 温度11段输出实际值百分比
  d_TempCtrlMW3_STATE_OUTPUT_PER_MW11,        // 0x2f36002d, 温度12段输出实际值百分比
  d_TempCtrlMW3_STATE_OUTPUT_PER_MW12,        // 0x2f36002e, 温度13段输出实际值百分比
  d_TempCtrlMW3_STATE_OUTPUT_PER_MW13,        // 0x2f36002f, 温度14段输出实际值百分比
  d_TempCtrlMW3_STATE_OUTPUT_PER_MW14,        // 0x2f360030, 温度15段输出实际值百分比
  d_TempCtrlMW3_STATE_OUTPUT_PER_MW15,        // 0x2f360031, 温度16段输出实际值百分比
  d_TempCtrlMW3_STATE_WTEMP_LM73_MW,          // 0x2f360032, 室温的温度
  d_TempCtrlMW3_STATE_FL_HEATERSTATUS_MW,     // 0x2f360033, 电热状态
  d_TempCtrlMW3_STATE_DATEMP_VER_MW,          // 0x2f360034, 温度扩展板E501版本号
  d_TempCtrlMW3_STATE_DATEMP_ERR_MW,          // 0x2f360035, 模温扩展板错误状态
  d_TempCtrlMW3_STATE_WRITE_CNT_E501,         // 0x2f360036, E501通讯测定写入次数
  d_TempCtrlMW3_STATE_READ_CNT_E501,          // 0x2f360037, E501通讯测试读入次数
  d_TempCtrlMW3_STATE_DATEMP_YOUHUAXUQIU,     // 0x2f360038, 温度优化提示开电热的闪烁指示
  d_TempCtrlMW3_STATE_DATEMP_YOUHUAOK,        // 0x2f360039, 温度优化开启与完成标记
  d_TempCtrlMW3_STATE_WSTATERSV6              // 0x2f36003a, 预温倒计时
};

enum db_TempCtrlMW4_PO{
  d_TempCtrlMW4_PO_O_COOL_MW = 0x2f400000  // 0x2f400000, 冷却水
};

enum db_TempCtrlMW4_PI{
  d_TempCtrlMW4_PI_I_HEAT_MW = 0x2f410000  // 0x2f410000, 加热接触器
};

enum db_TempCtrlMW4_MACHSET{
  d_TempCtrlMW4_MACHSET_TEMP_PROTECT_MW = 0x2f420000, // 0x2f420000, 温度保护上限
  d_TempCtrlMW4_MACHSET_TEMP_CONTROL_MW,              // 0x2f420001, 温度控制
  d_TempCtrlMW4_MACHSET_TEMP_CANLINK                  // 0x2f420002, 模温板接线
};

enum db_TempCtrlMW4_MOLDSET{
  d_TempCtrlMW4_MOLDSET_DATEMP_SETTING_MW0 = 0x2f430000, // 0x2f430000, 温度1段设定值
  d_TempCtrlMW4_MOLDSET_DATEMP_SETTING_MW1,              // 0x2f430001, 温度2段设定值
  d_TempCtrlMW4_MOLDSET_DATEMP_SETTING_MW2,              // 0x2f430002, 温度3段设定值
  d_TempCtrlMW4_MOLDSET_DATEMP_SETTING_MW3,              // 0x2f430003, 温度4段设定值
  d_TempCtrlMW4_MOLDSET_DATEMP_SETTING_MW4,              // 0x2f430004, 温度5段设定值
  d_TempCtrlMW4_MOLDSET_DATEMP_SETTING_MW5,              // 0x2f430005, 温度6段设定值
  d_TempCtrlMW4_MOLDSET_DATEMP_SETTING_MW6,              // 0x2f430006, 温度7段设定值
  d_TempCtrlMW4_MOLDSET_DATEMP_SETTING_MW7,              // 0x2f430007, 温度8段设定值
  d_TempCtrlMW4_MOLDSET_DATEMP_SETTING_MW8,              // 0x2f430008, 温度9段设定值
  d_TempCtrlMW4_MOLDSET_DATEMP_SETTING_MW9,              // 0x2f430009, 温度10段设定值
  d_TempCtrlMW4_MOLDSET_DATEMP_SETTING_MW10,             // 0x2f43000a, 温度11段设定值
  d_TempCtrlMW4_MOLDSET_DATEMP_SETTING_MW11,             // 0x2f43000b, 温度12段设定值
  d_TempCtrlMW4_MOLDSET_DATEMP_SETTING_MW12,             // 0x2f43000c, 温度13段设定值
  d_TempCtrlMW4_MOLDSET_DATEMP_SETTING_MW13,             // 0x2f43000d, 温度14段设定值
  d_TempCtrlMW4_MOLDSET_DATEMP_SETTING_MW14,             // 0x2f43000e, 温度15段设定值
  d_TempCtrlMW4_MOLDSET_DATEMP_SETTING_MW15,             // 0x2f43000f, 温度16段设定值
  d_TempCtrlMW4_MOLDSET_DATEMP_FUNC_MW0,                 // 0x2f430010, 温控1段功能选择
  d_TempCtrlMW4_MOLDSET_DATEMP_FUNC_MW1,                 // 0x2f430011, 温控2段功能选择
  d_TempCtrlMW4_MOLDSET_DATEMP_FUNC_MW2,                 // 0x2f430012, 温控3段功能选择
  d_TempCtrlMW4_MOLDSET_DATEMP_FUNC_MW3,                 // 0x2f430013, 温控4段功能选择
  d_TempCtrlMW4_MOLDSET_DATEMP_FUNC_MW4,                 // 0x2f430014, 温控5段功能选择
  d_TempCtrlMW4_MOLDSET_DATEMP_FUNC_MW5,                 // 0x2f430015, 温控6段功能选择
  d_TempCtrlMW4_MOLDSET_DATEMP_FUNC_MW6,                 // 0x2f430016, 温控7段功能选择
  d_TempCtrlMW4_MOLDSET_DATEMP_FUNC_MW7,                 // 0x2f430017, 温控8段功能选择
  d_TempCtrlMW4_MOLDSET_DATEMP_FUNC_MW8,                 // 0x2f430018, 温控9段功能选择
  d_TempCtrlMW4_MOLDSET_DATEMP_FUNC_MW9,                 // 0x2f430019, 温控10段功能选择
  d_TempCtrlMW4_MOLDSET_DATEMP_FUNC_MW10,                // 0x2f43001a, 温控11段功能选择
  d_TempCtrlMW4_MOLDSET_DATEMP_FUNC_MW11,                // 0x2f43001b, 温控12段功能选择
  d_TempCtrlMW4_MOLDSET_DATEMP_FUNC_MW12,                // 0x2f43001c, 温控13段功能选择
  d_TempCtrlMW4_MOLDSET_DATEMP_FUNC_MW13,                // 0x2f43001d, 温控14段功能选择
  d_TempCtrlMW4_MOLDSET_DATEMP_FUNC_MW14,                // 0x2f43001e, 温控15段功能选择
  d_TempCtrlMW4_MOLDSET_DATEMP_FUNC_MW15,                // 0x2f43001f, 温控16段功能选择
  d_TempCtrlMW4_MOLDSET_DAPID_KP_MW0,                    // 0x2f430020, 温度1段P系数
  d_TempCtrlMW4_MOLDSET_DAPID_KP_MW1,                    // 0x2f430021, 温度2段P系数
  d_TempCtrlMW4_MOLDSET_DAPID_KP_MW2,                    // 0x2f430022, 温度3段P系数
  d_TempCtrlMW4_MOLDSET_DAPID_KP_MW3,                    // 0x2f430023, 温度4段P系数
  d_TempCtrlMW4_MOLDSET_DAPID_KP_MW4,                    // 0x2f430024, 温度5段P系数
  d_TempCtrlMW4_MOLDSET_DAPID_KP_MW5,                    // 0x2f430025, 温度6段P系数
  d_TempCtrlMW4_MOLDSET_DAPID_KP_MW6,                    // 0x2f430026, 温度7段P系数
  d_TempCtrlMW4_MOLDSET_DAPID_KP_MW7,                    // 0x2f430027, 温度8段P系数
  d_TempCtrlMW4_MOLDSET_DAPID_KP_MW8,                    // 0x2f430028, 温度9段P系数
  d_TempCtrlMW4_MOLDSET_DAPID_KP_MW9,                    // 0x2f430029, 温度10段P系数
  d_TempCtrlMW4_MOLDSET_DAPID_KP_MW10,                   // 0x2f43002a, 温度11段P系数
  d_TempCtrlMW4_MOLDSET_DAPID_KP_MW11,                   // 0x2f43002b, 温度12段P系数
  d_TempCtrlMW4_MOLDSET_DAPID_KP_MW12,                   // 0x2f43002c, 温度13段P系数
  d_TempCtrlMW4_MOLDSET_DAPID_KP_MW13,                   // 0x2f43002d, 温度14段P系数
  d_TempCtrlMW4_MOLDSET_DAPID_KP_MW14,                   // 0x2f43002e, 温度15段P系数
  d_TempCtrlMW4_MOLDSET_DAPID_KP_MW15,                   // 0x2f43002f, 温度16段P系数
  d_TempCtrlMW4_MOLDSET_DAPID_TI_MW0,                    // 0x2f430030, 温度1段I系数
  d_TempCtrlMW4_MOLDSET_DAPID_TI_MW1,                    // 0x2f430031, 温度2段I系数
  d_TempCtrlMW4_MOLDSET_DAPID_TI_MW2,                    // 0x2f430032, 温度3段I系数
  d_TempCtrlMW4_MOLDSET_DAPID_TI_MW3,                    // 0x2f430033, 温度4段I系数
  d_TempCtrlMW4_MOLDSET_DAPID_TI_MW4,                    // 0x2f430034, 温度5段I系数
  d_TempCtrlMW4_MOLDSET_DAPID_TI_MW5,                    // 0x2f430035, 温度6段I系数
  d_TempCtrlMW4_MOLDSET_DAPID_TI_MW6,                    // 0x2f430036, 温度7段I系数
  d_TempCtrlMW4_MOLDSET_DAPID_TI_MW7,                    // 0x2f430037, 温度8段I系数
  d_TempCtrlMW4_MOLDSET_DAPID_TI_MW8,                    // 0x2f430038, 温度9段I系数
  d_TempCtrlMW4_MOLDSET_DAPID_TI_MW9,                    // 0x2f430039, 温度10段I系数
  d_TempCtrlMW4_MOLDSET_DAPID_TI_MW10,                   // 0x2f43003a, 温度11段I系数
  d_TempCtrlMW4_MOLDSET_DAPID_TI_MW11,                   // 0x2f43003b, 温度12段I系数
  d_TempCtrlMW4_MOLDSET_DAPID_TI_MW12,                   // 0x2f43003c, 温度13段I系数
  d_TempCtrlMW4_MOLDSET_DAPID_TI_MW13,                   // 0x2f43003d, 温度14段I系数
  d_TempCtrlMW4_MOLDSET_DAPID_TI_MW14,                   // 0x2f43003e, 温度15段I系数
  d_TempCtrlMW4_MOLDSET_DAPID_TI_MW15,                   // 0x2f43003f, 温度16段I系数
  d_TempCtrlMW4_MOLDSET_DAPID_TD_MW0,                    // 0x2f430040, 温度1段D系数
  d_TempCtrlMW4_MOLDSET_DAPID_TD_MW1,                    // 0x2f430041, 温度2段D系数
  d_TempCtrlMW4_MOLDSET_DAPID_TD_MW2,                    // 0x2f430042, 温度3段D系数
  d_TempCtrlMW4_MOLDSET_DAPID_TD_MW3,                    // 0x2f430043, 温度4段D系数
  d_TempCtrlMW4_MOLDSET_DAPID_TD_MW4,                    // 0x2f430044, 温度5段D系数
  d_TempCtrlMW4_MOLDSET_DAPID_TD_MW5,                    // 0x2f430045, 温度6段D系数
  d_TempCtrlMW4_MOLDSET_DAPID_TD_MW6,                    // 0x2f430046, 温度7段D系数
  d_TempCtrlMW4_MOLDSET_DAPID_TD_MW7,                    // 0x2f430047, 温度8段D系数
  d_TempCtrlMW4_MOLDSET_DAPID_TD_MW8,                    // 0x2f430048, 温度9段D系数
  d_TempCtrlMW4_MOLDSET_DAPID_TD_MW9,                    // 0x2f430049, 温度10段D系数
  d_TempCtrlMW4_MOLDSET_DAPID_TD_MW10,                   // 0x2f43004a, 温度11段D系数
  d_TempCtrlMW4_MOLDSET_DAPID_TD_MW11,                   // 0x2f43004b, 温度12段D系数
  d_TempCtrlMW4_MOLDSET_DAPID_TD_MW12,                   // 0x2f43004c, 温度13段D系数
  d_TempCtrlMW4_MOLDSET_DAPID_TD_MW13,                   // 0x2f43004d, 温度14段D系数
  d_TempCtrlMW4_MOLDSET_DAPID_TD_MW14,                   // 0x2f43004e, 温度15段D系数
  d_TempCtrlMW4_MOLDSET_DAPID_TD_MW15,                   // 0x2f43004f, 温度16段D系数
  d_TempCtrlMW4_MOLDSET_DATEMP_RAMP_MW0,                 // 0x2f430050, 温度1段加温缓冲区
  d_TempCtrlMW4_MOLDSET_DATEMP_RAMP_MW1,                 // 0x2f430051, 温度2段加温缓冲区
  d_TempCtrlMW4_MOLDSET_DATEMP_RAMP_MW2,                 // 0x2f430052, 温度3段加温缓冲区
  d_TempCtrlMW4_MOLDSET_DATEMP_RAMP_MW3,                 // 0x2f430053, 温度4段加温缓冲区
  d_TempCtrlMW4_MOLDSET_DATEMP_RAMP_MW4,                 // 0x2f430054, 温度5段加温缓冲区
  d_TempCtrlMW4_MOLDSET_DATEMP_RAMP_MW5,                 // 0x2f430055, 温度6段加温缓冲区
  d_TempCtrlMW4_MOLDSET_DATEMP_RAMP_MW6,                 // 0x2f430056, 温度7段加温缓冲区
  d_TempCtrlMW4_MOLDSET_DATEMP_RAMP_MW7,                 // 0x2f430057, 温度8段加温缓冲区
  d_TempCtrlMW4_MOLDSET_DATEMP_RAMP_MW8,                 // 0x2f430058, 温度9段加温缓冲区
  d_TempCtrlMW4_MOLDSET_DATEMP_RAMP_MW9,                 // 0x2f430059, 温度10段加温缓冲区
  d_TempCtrlMW4_MOLDSET_DATEMP_RAMP_MW10,                // 0x2f43005a, 温度11段加温缓冲区
  d_TempCtrlMW4_MOLDSET_DATEMP_RAMP_MW11,                // 0x2f43005b, 温度12段加温缓冲区
  d_TempCtrlMW4_MOLDSET_DATEMP_RAMP_MW12,                // 0x2f43005c, 温度13段加温缓冲区
  d_TempCtrlMW4_MOLDSET_DATEMP_RAMP_MW13,                // 0x2f43005d, 温度14段加温缓冲区
  d_TempCtrlMW4_MOLDSET_DATEMP_RAMP_MW14,                // 0x2f43005e, 温度15段加温缓冲区
  d_TempCtrlMW4_MOLDSET_DATEMP_RAMP_MW15,                // 0x2f43005f, 温度16段加温缓冲区
  d_TempCtrlMW4_MOLDSET_TM_RESPONSELESS_MW0,             // 0x2f430060, 温度1段输出周期时间
  d_TempCtrlMW4_MOLDSET_TM_RESPONSELESS_MW1,             // 0x2f430061, 温度2段输出周期时间
  d_TempCtrlMW4_MOLDSET_TM_RESPONSELESS_MW2,             // 0x2f430062, 温度3段输出周期时间
  d_TempCtrlMW4_MOLDSET_TM_RESPONSELESS_MW3,             // 0x2f430063, 温度4段输出周期时间
  d_TempCtrlMW4_MOLDSET_TM_RESPONSELESS_MW4,             // 0x2f430064, 温度5段输出周期时间
  d_TempCtrlMW4_MOLDSET_TM_RESPONSELESS_MW5,             // 0x2f430065, 温度6段输出周期时间
  d_TempCtrlMW4_MOLDSET_TM_RESPONSELESS_MW6,             // 0x2f430066, 温度7段输出周期时间
  d_TempCtrlMW4_MOLDSET_TM_RESPONSELESS_MW7,             // 0x2f430067, 温度8段输出周期时间
  d_TempCtrlMW4_MOLDSET_TM_RESPONSELESS_MW8,             // 0x2f430068, 温度9段输出周期时间
  d_TempCtrlMW4_MOLDSET_TM_RESPONSELESS_MW9,             // 0x2f430069, 温度10段输出周期时间
  d_TempCtrlMW4_MOLDSET_TM_RESPONSELESS_MW10,            // 0x2f43006a, 温度11段输出周期时间
  d_TempCtrlMW4_MOLDSET_TM_RESPONSELESS_MW11,            // 0x2f43006b, 温度12段输出周期时间
  d_TempCtrlMW4_MOLDSET_TM_RESPONSELESS_MW12,            // 0x2f43006c, 温度13段输出周期时间
  d_TempCtrlMW4_MOLDSET_TM_RESPONSELESS_MW13,            // 0x2f43006d, 温度14段输出周期时间
  d_TempCtrlMW4_MOLDSET_TM_RESPONSELESS_MW14,            // 0x2f43006e, 温度15段输出周期时间
  d_TempCtrlMW4_MOLDSET_TM_RESPONSELESS_MW15,            // 0x2f43006f, 温度16段输出周期时间
  d_TempCtrlMW4_MOLDSET_DATEMP_MAX_MW0,                  // 0x2f430070, 温度1段上偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MAX_MW1,                  // 0x2f430071, 温度2段上偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MAX_MW2,                  // 0x2f430072, 温度3段上偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MAX_MW3,                  // 0x2f430073, 温度4段上偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MAX_MW4,                  // 0x2f430074, 温度5段上偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MAX_MW5,                  // 0x2f430075, 温度6段上偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MAX_MW6,                  // 0x2f430076, 温度7段上偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MAX_MW7,                  // 0x2f430077, 温度8段上偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MAX_MW8,                  // 0x2f430078, 温度9段上偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MAX_MW9,                  // 0x2f430079, 温度10段上偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MAX_MW10,                 // 0x2f43007a, 温度11段上偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MAX_MW11,                 // 0x2f43007b, 温度12段上偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MAX_MW12,                 // 0x2f43007c, 温度13段上偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MAX_MW13,                 // 0x2f43007d, 温度14段上偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MAX_MW14,                 // 0x2f43007e, 温度15段上偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MAX_MW15,                 // 0x2f43007f, 温度16段上偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MIN_MW0,                  // 0x2f430080, 温度1段下偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MIN_MW1,                  // 0x2f430081, 温度2段下偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MIN_MW2,                  // 0x2f430082, 温度3段下偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MIN_MW3,                  // 0x2f430083, 温度4段下偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MIN_MW4,                  // 0x2f430084, 温度5段下偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MIN_MW5,                  // 0x2f430085, 温度6段下偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MIN_MW6,                  // 0x2f430086, 温度7段下偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MIN_MW7,                  // 0x2f430087, 温度8段下偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MIN_MW8,                  // 0x2f430088, 温度9段下偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MIN_MW9,                  // 0x2f430089, 温度10段下偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MIN_MW10,                 // 0x2f43008a, 温度11段下偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MIN_MW11,                 // 0x2f43008b, 温度12段下偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MIN_MW12,                 // 0x2f43008c, 温度13段下偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MIN_MW13,                 // 0x2f43008d, 温度14段下偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MIN_MW14,                 // 0x2f43008e, 温度15段下偏差
  d_TempCtrlMW4_MOLDSET_DATEMP_MIN_MW15,                 // 0x2f43008f, 温度16段下偏差
  d_TempCtrlMW4_MOLDSET_FL_TKTJ_MW0,                     // 0x2f430090, 温度1段热电偶类型
  d_TempCtrlMW4_MOLDSET_FL_TKTJ_MW1,                     // 0x2f430091, 温度2段热电偶类型
  d_TempCtrlMW4_MOLDSET_FL_TKTJ_MW2,                     // 0x2f430092, 温度3段热电偶类型
  d_TempCtrlMW4_MOLDSET_FL_TKTJ_MW3,                     // 0x2f430093, 温度4段热电偶类型
  d_TempCtrlMW4_MOLDSET_FL_TKTJ_MW4,                     // 0x2f430094, 温度5段热电偶类型
  d_TempCtrlMW4_MOLDSET_FL_TKTJ_MW5,                     // 0x2f430095, 温度6段热电偶类型
  d_TempCtrlMW4_MOLDSET_FL_TKTJ_MW6,                     // 0x2f430096, 温度7段热电偶类型
  d_TempCtrlMW4_MOLDSET_FL_TKTJ_MW7,                     // 0x2f430097, 温度8段热电偶类型
  d_TempCtrlMW4_MOLDSET_FL_TKTJ_MW8,                     // 0x2f430098, 温度9段热电偶类型
  d_TempCtrlMW4_MOLDSET_FL_TKTJ_MW9,                     // 0x2f430099, 温度10段热电偶类型
  d_TempCtrlMW4_MOLDSET_FL_TKTJ_MW10,                    // 0x2f43009a, 温度11段热电偶类型
  d_TempCtrlMW4_MOLDSET_FL_TKTJ_MW11,                    // 0x2f43009b, 温度12段热电偶类型
  d_TempCtrlMW4_MOLDSET_FL_TKTJ_MW12,                    // 0x2f43009c, 温度13段热电偶类型
  d_TempCtrlMW4_MOLDSET_FL_TKTJ_MW13,                    // 0x2f43009d, 温度14段热电偶类型
  d_TempCtrlMW4_MOLDSET_FL_TKTJ_MW14,                    // 0x2f43009e, 温度15段热电偶类型
  d_TempCtrlMW4_MOLDSET_FL_TKTJ_MW15,                    // 0x2f43009f, 温度16段热电偶类型
  d_TempCtrlMW4_MOLDSET_DACENT_HEATERON_MW0,             // 0x2f4300a0, 温度1段手动比例
  d_TempCtrlMW4_MOLDSET_DACENT_HEATERON_MW1,             // 0x2f4300a1, 温度2段手动比例
  d_TempCtrlMW4_MOLDSET_DACENT_HEATERON_MW2,             // 0x2f4300a2, 温度3段手动比例
  d_TempCtrlMW4_MOLDSET_DACENT_HEATERON_MW3,             // 0x2f4300a3, 温度4段手动比例
  d_TempCtrlMW4_MOLDSET_DACENT_HEATERON_MW4,             // 0x2f4300a4, 温度5段手动比例
  d_TempCtrlMW4_MOLDSET_DACENT_HEATERON_MW5,             // 0x2f4300a5, 温度6段手动比例
  d_TempCtrlMW4_MOLDSET_DACENT_HEATERON_MW6,             // 0x2f4300a6, 温度7段手动比例
  d_TempCtrlMW4_MOLDSET_DACENT_HEATERON_MW7,             // 0x2f4300a7, 温度8段手动比例
  d_TempCtrlMW4_MOLDSET_DACENT_HEATERON_MW8,             // 0x2f4300a8, 温度9段手动比例
  d_TempCtrlMW4_MOLDSET_DACENT_HEATERON_MW9,             // 0x2f4300a9, 温度10段手动比例
  d_TempCtrlMW4_MOLDSET_DACENT_HEATERON_MW10,            // 0x2f4300aa, 温度11段手动比例
  d_TempCtrlMW4_MOLDSET_DACENT_HEATERON_MW11,            // 0x2f4300ab, 温度12段手动比例
  d_TempCtrlMW4_MOLDSET_DACENT_HEATERON_MW12,            // 0x2f4300ac, 温度13段手动比例
  d_TempCtrlMW4_MOLDSET_DACENT_HEATERON_MW13,            // 0x2f4300ad, 温度14段手动比例
  d_TempCtrlMW4_MOLDSET_DACENT_HEATERON_MW14,            // 0x2f4300ae, 温度15段手动比例
  d_TempCtrlMW4_MOLDSET_DACENT_HEATERON_MW15,            // 0x2f4300af, 温度16段手动比例
  d_TempCtrlMW4_MOLDSET_FL_TEMPHOLD_MW,                  // 0x2f4300b0, 保温功能
  d_TempCtrlMW4_MOLDSET_DATEMP_HOLD_MW,                  // 0x2f4300b1, 保温温度
  d_TempCtrlMW4_MOLDSET_DATEMP_SCTM_MIN_MW0,             // 0x2f4300b2, 最小扫描周期
  d_TempCtrlMW4_MOLDSET_FL_TEMPUPCHECK_MW,               // 0x2f4300b3, 模温连续加温检查
  d_TempCtrlMW4_MOLDSET_DATEMP_NUM_MW,                   // 0x2f4300b4, 温度段数
  d_TempCtrlMW4_MOLDSET_TM_HEATERWARM_MW,                // 0x2f4300b5, 预温计时
  d_TempCtrlMW4_MOLDSET_FL_USE_MW,                       // 0x2f4300b6, 模温功能
  d_TempCtrlMW4_MOLDSET_FL_TEMP_YH,                      // 0x2f4300b7, 温度优化命令
  d_TempCtrlMW4_MOLDSET_FL_TEMP_YHQZ,                    // 0x2f4300b8, 温度强制优化命令
  d_TempCtrlMW4_MOLDSET_CONFUNC_MW,                      // 0x2f4300b9, 同步升温功能
  d_TempCtrlMW4_MOLDSET_FL_INJ_ALLOW                     // 0x2f4300ba, 射出检测模温温度
};

enum db_TempCtrlMW4_STATE{
  d_TempCtrlMW4_STATE_FL_ERROR0 = 0x2f460000, // 0x2f460000, 警报0
  d_TempCtrlMW4_STATE_FL_ERROR1,              // 0x2f460001, 警报1
  d_TempCtrlMW4_STATE_DATEMP_REAL_MW0,        // 0x2f460002, 温度1段实际值
  d_TempCtrlMW4_STATE_DATEMP_REAL_MW1,        // 0x2f460003, 温度2段实际值
  d_TempCtrlMW4_STATE_DATEMP_REAL_MW2,        // 0x2f460004, 温度3段实际值
  d_TempCtrlMW4_STATE_DATEMP_REAL_MW3,        // 0x2f460005, 温度4段实际值
  d_TempCtrlMW4_STATE_DATEMP_REAL_MW4,        // 0x2f460006, 温度5段实际值
  d_TempCtrlMW4_STATE_DATEMP_REAL_MW5,        // 0x2f460007, 温度6段实际值
  d_TempCtrlMW4_STATE_DATEMP_REAL_MW6,        // 0x2f460008, 温度7段实际值
  d_TempCtrlMW4_STATE_DATEMP_REAL_MW7,        // 0x2f460009, 温度8段实际值
  d_TempCtrlMW4_STATE_DATEMP_REAL_MW8,        // 0x2f46000a, 温度9段实际值
  d_TempCtrlMW4_STATE_DATEMP_REAL_MW9,        // 0x2f46000b, 温度10段实际值
  d_TempCtrlMW4_STATE_DATEMP_REAL_MW10,       // 0x2f46000c, 温度11段实际值
  d_TempCtrlMW4_STATE_DATEMP_REAL_MW11,       // 0x2f46000d, 温度12段实际值
  d_TempCtrlMW4_STATE_DATEMP_REAL_MW12,       // 0x2f46000e, 温度13段实际值
  d_TempCtrlMW4_STATE_DATEMP_REAL_MW13,       // 0x2f46000f, 温度14段实际值
  d_TempCtrlMW4_STATE_DATEMP_REAL_MW14,       // 0x2f460010, 温度15段实际值
  d_TempCtrlMW4_STATE_DATEMP_REAL_MW15,       // 0x2f460011, 温度16段实际值
  d_TempCtrlMW4_STATE_FL_STATUS_MW0,          // 0x2f460012, 温度1段状态
  d_TempCtrlMW4_STATE_FL_STATUS_MW1,          // 0x2f460013, 温度2段状态
  d_TempCtrlMW4_STATE_FL_STATUS_MW2,          // 0x2f460014, 温度3段状态
  d_TempCtrlMW4_STATE_FL_STATUS_MW3,          // 0x2f460015, 温度4段状态
  d_TempCtrlMW4_STATE_FL_STATUS_MW4,          // 0x2f460016, 温度5段状态
  d_TempCtrlMW4_STATE_FL_STATUS_MW5,          // 0x2f460017, 温度6段状态
  d_TempCtrlMW4_STATE_FL_STATUS_MW6,          // 0x2f460018, 温度7段状态
  d_TempCtrlMW4_STATE_FL_STATUS_MW7,          // 0x2f460019, 温度8段状态
  d_TempCtrlMW4_STATE_FL_STATUS_MW8,          // 0x2f46001a, 温度9段状态
  d_TempCtrlMW4_STATE_FL_STATUS_MW9,          // 0x2f46001b, 温度10段状态
  d_TempCtrlMW4_STATE_FL_STATUS_MW10,         // 0x2f46001c, 温度11段状态
  d_TempCtrlMW4_STATE_FL_STATUS_MW11,         // 0x2f46001d, 温度12段状态
  d_TempCtrlMW4_STATE_FL_STATUS_MW12,         // 0x2f46001e, 温度13段状态
  d_TempCtrlMW4_STATE_FL_STATUS_MW13,         // 0x2f46001f, 温度14段状态
  d_TempCtrlMW4_STATE_FL_STATUS_MW14,         // 0x2f460020, 温度15段状态
  d_TempCtrlMW4_STATE_FL_STATUS_MW15,         // 0x2f460021, 温度16段状态
  d_TempCtrlMW4_STATE_OUTPUT_PER_MW0,         // 0x2f460022, 温度1段输出实际值百分比
  d_TempCtrlMW4_STATE_OUTPUT_PER_MW1,         // 0x2f460023, 温度2段输出实际值百分比
  d_TempCtrlMW4_STATE_OUTPUT_PER_MW2,         // 0x2f460024, 温度3段输出实际值百分比
  d_TempCtrlMW4_STATE_OUTPUT_PER_MW3,         // 0x2f460025, 温度4段输出实际值百分比
  d_TempCtrlMW4_STATE_OUTPUT_PER_MW4,         // 0x2f460026, 温度5段输出实际值百分比
  d_TempCtrlMW4_STATE_OUTPUT_PER_MW5,         // 0x2f460027, 温度6段输出实际值百分比
  d_TempCtrlMW4_STATE_OUTPUT_PER_MW6,         // 0x2f460028, 温度7段输出实际值百分比
  d_TempCtrlMW4_STATE_OUTPUT_PER_MW7,         // 0x2f460029, 温度8段输出实际值百分比
  d_TempCtrlMW4_STATE_OUTPUT_PER_MW8,         // 0x2f46002a, 温度9段输出实际值百分比
  d_TempCtrlMW4_STATE_OUTPUT_PER_MW9,         // 0x2f46002b, 温度10段输出实际值百分比
  d_TempCtrlMW4_STATE_OUTPUT_PER_MW10,        // 0x2f46002c, 温度11段输出实际值百分比
  d_TempCtrlMW4_STATE_OUTPUT_PER_MW11,        // 0x2f46002d, 温度12段输出实际值百分比
  d_TempCtrlMW4_STATE_OUTPUT_PER_MW12,        // 0x2f46002e, 温度13段输出实际值百分比
  d_TempCtrlMW4_STATE_OUTPUT_PER_MW13,        // 0x2f46002f, 温度14段输出实际值百分比
  d_TempCtrlMW4_STATE_OUTPUT_PER_MW14,        // 0x2f460030, 温度15段输出实际值百分比
  d_TempCtrlMW4_STATE_OUTPUT_PER_MW15,        // 0x2f460031, 温度16段输出实际值百分比
  d_TempCtrlMW4_STATE_WTEMP_LM73_MW,          // 0x2f460032, 室温的温度
  d_TempCtrlMW4_STATE_FL_HEATERSTATUS_MW,     // 0x2f460033, 电热状态
  d_TempCtrlMW4_STATE_DATEMP_VER_MW,          // 0x2f460034, 温度扩展板E501版本号
  d_TempCtrlMW4_STATE_DATEMP_ERR_MW,          // 0x2f460035, 模温扩展板错误状态
  d_TempCtrlMW4_STATE_WRITE_CNT_E501,         // 0x2f460036, E501通讯测定写入次数
  d_TempCtrlMW4_STATE_READ_CNT_E501,          // 0x2f460037, E501通讯测试读入次数
  d_TempCtrlMW4_STATE_DATEMP_YOUHUAXUQIU,     // 0x2f460038, 温度优化提示开电热的闪烁指示
  d_TempCtrlMW4_STATE_DATEMP_YOUHUAOK,        // 0x2f460039, 温度优化开启与完成标记
  d_TempCtrlMW4_STATE_WSTATERSV6              // 0x2f46003a, 预温倒计时
};

enum db_TempCtrlMW5_PO{
  d_TempCtrlMW5_PO_O_COOL_MW = 0x2f500000  // 0x2f500000, 冷却水
};

enum db_TempCtrlMW5_PI{
  d_TempCtrlMW5_PI_I_HEAT_MW = 0x2f510000  // 0x2f510000, 加热接触器
};

enum db_TempCtrlMW5_MACHSET{
  d_TempCtrlMW5_MACHSET_TEMP_PROTECT_MW = 0x2f520000, // 0x2f520000, 温度保护上限
  d_TempCtrlMW5_MACHSET_TEMP_CONTROL_MW,              // 0x2f520001, 温度控制
  d_TempCtrlMW5_MACHSET_TEMP_CANLINK                  // 0x2f520002, 模温板接线
};

enum db_TempCtrlMW5_MOLDSET{
  d_TempCtrlMW5_MOLDSET_DATEMP_SETTING_MW0 = 0x2f530000, // 0x2f530000, 温度1段设定值
  d_TempCtrlMW5_MOLDSET_DATEMP_SETTING_MW1,              // 0x2f530001, 温度2段设定值
  d_TempCtrlMW5_MOLDSET_DATEMP_SETTING_MW2,              // 0x2f530002, 温度3段设定值
  d_TempCtrlMW5_MOLDSET_DATEMP_SETTING_MW3,              // 0x2f530003, 温度4段设定值
  d_TempCtrlMW5_MOLDSET_DATEMP_SETTING_MW4,              // 0x2f530004, 温度5段设定值
  d_TempCtrlMW5_MOLDSET_DATEMP_SETTING_MW5,              // 0x2f530005, 温度6段设定值
  d_TempCtrlMW5_MOLDSET_DATEMP_SETTING_MW6,              // 0x2f530006, 温度7段设定值
  d_TempCtrlMW5_MOLDSET_DATEMP_SETTING_MW7,              // 0x2f530007, 温度8段设定值
  d_TempCtrlMW5_MOLDSET_DATEMP_SETTING_MW8,              // 0x2f530008, 温度9段设定值
  d_TempCtrlMW5_MOLDSET_DATEMP_SETTING_MW9,              // 0x2f530009, 温度10段设定值
  d_TempCtrlMW5_MOLDSET_DATEMP_SETTING_MW10,             // 0x2f53000a, 温度11段设定值
  d_TempCtrlMW5_MOLDSET_DATEMP_SETTING_MW11,             // 0x2f53000b, 温度12段设定值
  d_TempCtrlMW5_MOLDSET_DATEMP_SETTING_MW12,             // 0x2f53000c, 温度13段设定值
  d_TempCtrlMW5_MOLDSET_DATEMP_SETTING_MW13,             // 0x2f53000d, 温度14段设定值
  d_TempCtrlMW5_MOLDSET_DATEMP_SETTING_MW14,             // 0x2f53000e, 温度15段设定值
  d_TempCtrlMW5_MOLDSET_DATEMP_SETTING_MW15,             // 0x2f53000f, 温度16段设定值
  d_TempCtrlMW5_MOLDSET_DATEMP_FUNC_MW0,                 // 0x2f530010, 温控1段功能选择
  d_TempCtrlMW5_MOLDSET_DATEMP_FUNC_MW1,                 // 0x2f530011, 温控2段功能选择
  d_TempCtrlMW5_MOLDSET_DATEMP_FUNC_MW2,                 // 0x2f530012, 温控3段功能选择
  d_TempCtrlMW5_MOLDSET_DATEMP_FUNC_MW3,                 // 0x2f530013, 温控4段功能选择
  d_TempCtrlMW5_MOLDSET_DATEMP_FUNC_MW4,                 // 0x2f530014, 温控5段功能选择
  d_TempCtrlMW5_MOLDSET_DATEMP_FUNC_MW5,                 // 0x2f530015, 温控6段功能选择
  d_TempCtrlMW5_MOLDSET_DATEMP_FUNC_MW6,                 // 0x2f530016, 温控7段功能选择
  d_TempCtrlMW5_MOLDSET_DATEMP_FUNC_MW7,                 // 0x2f530017, 温控8段功能选择
  d_TempCtrlMW5_MOLDSET_DATEMP_FUNC_MW8,                 // 0x2f530018, 温控9段功能选择
  d_TempCtrlMW5_MOLDSET_DATEMP_FUNC_MW9,                 // 0x2f530019, 温控10段功能选择
  d_TempCtrlMW5_MOLDSET_DATEMP_FUNC_MW10,                // 0x2f53001a, 温控11段功能选择
  d_TempCtrlMW5_MOLDSET_DATEMP_FUNC_MW11,                // 0x2f53001b, 温控12段功能选择
  d_TempCtrlMW5_MOLDSET_DATEMP_FUNC_MW12,                // 0x2f53001c, 温控13段功能选择
  d_TempCtrlMW5_MOLDSET_DATEMP_FUNC_MW13,                // 0x2f53001d, 温控14段功能选择
  d_TempCtrlMW5_MOLDSET_DATEMP_FUNC_MW14,                // 0x2f53001e, 温控15段功能选择
  d_TempCtrlMW5_MOLDSET_DATEMP_FUNC_MW15,                // 0x2f53001f, 温控16段功能选择
  d_TempCtrlMW5_MOLDSET_DAPID_KP_MW0,                    // 0x2f530020, 温度1段P系数
  d_TempCtrlMW5_MOLDSET_DAPID_KP_MW1,                    // 0x2f530021, 温度2段P系数
  d_TempCtrlMW5_MOLDSET_DAPID_KP_MW2,                    // 0x2f530022, 温度3段P系数
  d_TempCtrlMW5_MOLDSET_DAPID_KP_MW3,                    // 0x2f530023, 温度4段P系数
  d_TempCtrlMW5_MOLDSET_DAPID_KP_MW4,                    // 0x2f530024, 温度5段P系数
  d_TempCtrlMW5_MOLDSET_DAPID_KP_MW5,                    // 0x2f530025, 温度6段P系数
  d_TempCtrlMW5_MOLDSET_DAPID_KP_MW6,                    // 0x2f530026, 温度7段P系数
  d_TempCtrlMW5_MOLDSET_DAPID_KP_MW7,                    // 0x2f530027, 温度8段P系数
  d_TempCtrlMW5_MOLDSET_DAPID_KP_MW8,                    // 0x2f530028, 温度9段P系数
  d_TempCtrlMW5_MOLDSET_DAPID_KP_MW9,                    // 0x2f530029, 温度10段P系数
  d_TempCtrlMW5_MOLDSET_DAPID_KP_MW10,                   // 0x2f53002a, 温度11段P系数
  d_TempCtrlMW5_MOLDSET_DAPID_KP_MW11,                   // 0x2f53002b, 温度12段P系数
  d_TempCtrlMW5_MOLDSET_DAPID_KP_MW12,                   // 0x2f53002c, 温度13段P系数
  d_TempCtrlMW5_MOLDSET_DAPID_KP_MW13,                   // 0x2f53002d, 温度14段P系数
  d_TempCtrlMW5_MOLDSET_DAPID_KP_MW14,                   // 0x2f53002e, 温度15段P系数
  d_TempCtrlMW5_MOLDSET_DAPID_KP_MW15,                   // 0x2f53002f, 温度16段P系数
  d_TempCtrlMW5_MOLDSET_DAPID_TI_MW0,                    // 0x2f530030, 温度1段I系数
  d_TempCtrlMW5_MOLDSET_DAPID_TI_MW1,                    // 0x2f530031, 温度2段I系数
  d_TempCtrlMW5_MOLDSET_DAPID_TI_MW2,                    // 0x2f530032, 温度3段I系数
  d_TempCtrlMW5_MOLDSET_DAPID_TI_MW3,                    // 0x2f530033, 温度4段I系数
  d_TempCtrlMW5_MOLDSET_DAPID_TI_MW4,                    // 0x2f530034, 温度5段I系数
  d_TempCtrlMW5_MOLDSET_DAPID_TI_MW5,                    // 0x2f530035, 温度6段I系数
  d_TempCtrlMW5_MOLDSET_DAPID_TI_MW6,                    // 0x2f530036, 温度7段I系数
  d_TempCtrlMW5_MOLDSET_DAPID_TI_MW7,                    // 0x2f530037, 温度8段I系数
  d_TempCtrlMW5_MOLDSET_DAPID_TI_MW8,                    // 0x2f530038, 温度9段I系数
  d_TempCtrlMW5_MOLDSET_DAPID_TI_MW9,                    // 0x2f530039, 温度10段I系数
  d_TempCtrlMW5_MOLDSET_DAPID_TI_MW10,                   // 0x2f53003a, 温度11段I系数
  d_TempCtrlMW5_MOLDSET_DAPID_TI_MW11,                   // 0x2f53003b, 温度12段I系数
  d_TempCtrlMW5_MOLDSET_DAPID_TI_MW12,                   // 0x2f53003c, 温度13段I系数
  d_TempCtrlMW5_MOLDSET_DAPID_TI_MW13,                   // 0x2f53003d, 温度14段I系数
  d_TempCtrlMW5_MOLDSET_DAPID_TI_MW14,                   // 0x2f53003e, 温度15段I系数
  d_TempCtrlMW5_MOLDSET_DAPID_TI_MW15,                   // 0x2f53003f, 温度16段I系数
  d_TempCtrlMW5_MOLDSET_DAPID_TD_MW0,                    // 0x2f530040, 温度1段D系数
  d_TempCtrlMW5_MOLDSET_DAPID_TD_MW1,                    // 0x2f530041, 温度2段D系数
  d_TempCtrlMW5_MOLDSET_DAPID_TD_MW2,                    // 0x2f530042, 温度3段D系数
  d_TempCtrlMW5_MOLDSET_DAPID_TD_MW3,                    // 0x2f530043, 温度4段D系数
  d_TempCtrlMW5_MOLDSET_DAPID_TD_MW4,                    // 0x2f530044, 温度5段D系数
  d_TempCtrlMW5_MOLDSET_DAPID_TD_MW5,                    // 0x2f530045, 温度6段D系数
  d_TempCtrlMW5_MOLDSET_DAPID_TD_MW6,                    // 0x2f530046, 温度7段D系数
  d_TempCtrlMW5_MOLDSET_DAPID_TD_MW7,                    // 0x2f530047, 温度8段D系数
  d_TempCtrlMW5_MOLDSET_DAPID_TD_MW8,                    // 0x2f530048, 温度9段D系数
  d_TempCtrlMW5_MOLDSET_DAPID_TD_MW9,                    // 0x2f530049, 温度10段D系数
  d_TempCtrlMW5_MOLDSET_DAPID_TD_MW10,                   // 0x2f53004a, 温度11段D系数
  d_TempCtrlMW5_MOLDSET_DAPID_TD_MW11,                   // 0x2f53004b, 温度12段D系数
  d_TempCtrlMW5_MOLDSET_DAPID_TD_MW12,                   // 0x2f53004c, 温度13段D系数
  d_TempCtrlMW5_MOLDSET_DAPID_TD_MW13,                   // 0x2f53004d, 温度14段D系数
  d_TempCtrlMW5_MOLDSET_DAPID_TD_MW14,                   // 0x2f53004e, 温度15段D系数
  d_TempCtrlMW5_MOLDSET_DAPID_TD_MW15,                   // 0x2f53004f, 温度16段D系数
  d_TempCtrlMW5_MOLDSET_DATEMP_RAMP_MW0,                 // 0x2f530050, 温度1段加温缓冲区
  d_TempCtrlMW5_MOLDSET_DATEMP_RAMP_MW1,                 // 0x2f530051, 温度2段加温缓冲区
  d_TempCtrlMW5_MOLDSET_DATEMP_RAMP_MW2,                 // 0x2f530052, 温度3段加温缓冲区
  d_TempCtrlMW5_MOLDSET_DATEMP_RAMP_MW3,                 // 0x2f530053, 温度4段加温缓冲区
  d_TempCtrlMW5_MOLDSET_DATEMP_RAMP_MW4,                 // 0x2f530054, 温度5段加温缓冲区
  d_TempCtrlMW5_MOLDSET_DATEMP_RAMP_MW5,                 // 0x2f530055, 温度6段加温缓冲区
  d_TempCtrlMW5_MOLDSET_DATEMP_RAMP_MW6,                 // 0x2f530056, 温度7段加温缓冲区
  d_TempCtrlMW5_MOLDSET_DATEMP_RAMP_MW7,                 // 0x2f530057, 温度8段加温缓冲区
  d_TempCtrlMW5_MOLDSET_DATEMP_RAMP_MW8,                 // 0x2f530058, 温度9段加温缓冲区
  d_TempCtrlMW5_MOLDSET_DATEMP_RAMP_MW9,                 // 0x2f530059, 温度10段加温缓冲区
  d_TempCtrlMW5_MOLDSET_DATEMP_RAMP_MW10,                // 0x2f53005a, 温度11段加温缓冲区
  d_TempCtrlMW5_MOLDSET_DATEMP_RAMP_MW11,                // 0x2f53005b, 温度12段加温缓冲区
  d_TempCtrlMW5_MOLDSET_DATEMP_RAMP_MW12,                // 0x2f53005c, 温度13段加温缓冲区
  d_TempCtrlMW5_MOLDSET_DATEMP_RAMP_MW13,                // 0x2f53005d, 温度14段加温缓冲区
  d_TempCtrlMW5_MOLDSET_DATEMP_RAMP_MW14,                // 0x2f53005e, 温度15段加温缓冲区
  d_TempCtrlMW5_MOLDSET_DATEMP_RAMP_MW15,                // 0x2f53005f, 温度16段加温缓冲区
  d_TempCtrlMW5_MOLDSET_TM_RESPONSELESS_MW0,             // 0x2f530060, 温度1段输出周期时间
  d_TempCtrlMW5_MOLDSET_TM_RESPONSELESS_MW1,             // 0x2f530061, 温度2段输出周期时间
  d_TempCtrlMW5_MOLDSET_TM_RESPONSELESS_MW2,             // 0x2f530062, 温度3段输出周期时间
  d_TempCtrlMW5_MOLDSET_TM_RESPONSELESS_MW3,             // 0x2f530063, 温度4段输出周期时间
  d_TempCtrlMW5_MOLDSET_TM_RESPONSELESS_MW4,             // 0x2f530064, 温度5段输出周期时间
  d_TempCtrlMW5_MOLDSET_TM_RESPONSELESS_MW5,             // 0x2f530065, 温度6段输出周期时间
  d_TempCtrlMW5_MOLDSET_TM_RESPONSELESS_MW6,             // 0x2f530066, 温度7段输出周期时间
  d_TempCtrlMW5_MOLDSET_TM_RESPONSELESS_MW7,             // 0x2f530067, 温度8段输出周期时间
  d_TempCtrlMW5_MOLDSET_TM_RESPONSELESS_MW8,             // 0x2f530068, 温度9段输出周期时间
  d_TempCtrlMW5_MOLDSET_TM_RESPONSELESS_MW9,             // 0x2f530069, 温度10段输出周期时间
  d_TempCtrlMW5_MOLDSET_TM_RESPONSELESS_MW10,            // 0x2f53006a, 温度11段输出周期时间
  d_TempCtrlMW5_MOLDSET_TM_RESPONSELESS_MW11,            // 0x2f53006b, 温度12段输出周期时间
  d_TempCtrlMW5_MOLDSET_TM_RESPONSELESS_MW12,            // 0x2f53006c, 温度13段输出周期时间
  d_TempCtrlMW5_MOLDSET_TM_RESPONSELESS_MW13,            // 0x2f53006d, 温度14段输出周期时间
  d_TempCtrlMW5_MOLDSET_TM_RESPONSELESS_MW14,            // 0x2f53006e, 温度15段输出周期时间
  d_TempCtrlMW5_MOLDSET_TM_RESPONSELESS_MW15,            // 0x2f53006f, 温度16段输出周期时间
  d_TempCtrlMW5_MOLDSET_DATEMP_MAX_MW0,                  // 0x2f530070, 温度1段上偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MAX_MW1,                  // 0x2f530071, 温度2段上偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MAX_MW2,                  // 0x2f530072, 温度3段上偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MAX_MW3,                  // 0x2f530073, 温度4段上偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MAX_MW4,                  // 0x2f530074, 温度5段上偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MAX_MW5,                  // 0x2f530075, 温度6段上偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MAX_MW6,                  // 0x2f530076, 温度7段上偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MAX_MW7,                  // 0x2f530077, 温度8段上偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MAX_MW8,                  // 0x2f530078, 温度9段上偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MAX_MW9,                  // 0x2f530079, 温度10段上偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MAX_MW10,                 // 0x2f53007a, 温度11段上偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MAX_MW11,                 // 0x2f53007b, 温度12段上偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MAX_MW12,                 // 0x2f53007c, 温度13段上偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MAX_MW13,                 // 0x2f53007d, 温度14段上偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MAX_MW14,                 // 0x2f53007e, 温度15段上偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MAX_MW15,                 // 0x2f53007f, 温度16段上偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MIN_MW0,                  // 0x2f530080, 温度1段下偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MIN_MW1,                  // 0x2f530081, 温度2段下偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MIN_MW2,                  // 0x2f530082, 温度3段下偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MIN_MW3,                  // 0x2f530083, 温度4段下偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MIN_MW4,                  // 0x2f530084, 温度5段下偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MIN_MW5,                  // 0x2f530085, 温度6段下偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MIN_MW6,                  // 0x2f530086, 温度7段下偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MIN_MW7,                  // 0x2f530087, 温度8段下偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MIN_MW8,                  // 0x2f530088, 温度9段下偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MIN_MW9,                  // 0x2f530089, 温度10段下偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MIN_MW10,                 // 0x2f53008a, 温度11段下偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MIN_MW11,                 // 0x2f53008b, 温度12段下偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MIN_MW12,                 // 0x2f53008c, 温度13段下偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MIN_MW13,                 // 0x2f53008d, 温度14段下偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MIN_MW14,                 // 0x2f53008e, 温度15段下偏差
  d_TempCtrlMW5_MOLDSET_DATEMP_MIN_MW15,                 // 0x2f53008f, 温度16段下偏差
  d_TempCtrlMW5_MOLDSET_FL_TKTJ_MW0,                     // 0x2f530090, 温度1段热电偶类型
  d_TempCtrlMW5_MOLDSET_FL_TKTJ_MW1,                     // 0x2f530091, 温度2段热电偶类型
  d_TempCtrlMW5_MOLDSET_FL_TKTJ_MW2,                     // 0x2f530092, 温度3段热电偶类型
  d_TempCtrlMW5_MOLDSET_FL_TKTJ_MW3,                     // 0x2f530093, 温度4段热电偶类型
  d_TempCtrlMW5_MOLDSET_FL_TKTJ_MW4,                     // 0x2f530094, 温度5段热电偶类型
  d_TempCtrlMW5_MOLDSET_FL_TKTJ_MW5,                     // 0x2f530095, 温度6段热电偶类型
  d_TempCtrlMW5_MOLDSET_FL_TKTJ_MW6,                     // 0x2f530096, 温度7段热电偶类型
  d_TempCtrlMW5_MOLDSET_FL_TKTJ_MW7,                     // 0x2f530097, 温度8段热电偶类型
  d_TempCtrlMW5_MOLDSET_FL_TKTJ_MW8,                     // 0x2f530098, 温度9段热电偶类型
  d_TempCtrlMW5_MOLDSET_FL_TKTJ_MW9,                     // 0x2f530099, 温度10段热电偶类型
  d_TempCtrlMW5_MOLDSET_FL_TKTJ_MW10,                    // 0x2f53009a, 温度11段热电偶类型
  d_TempCtrlMW5_MOLDSET_FL_TKTJ_MW11,                    // 0x2f53009b, 温度12段热电偶类型
  d_TempCtrlMW5_MOLDSET_FL_TKTJ_MW12,                    // 0x2f53009c, 温度13段热电偶类型
  d_TempCtrlMW5_MOLDSET_FL_TKTJ_MW13,                    // 0x2f53009d, 温度14段热电偶类型
  d_TempCtrlMW5_MOLDSET_FL_TKTJ_MW14,                    // 0x2f53009e, 温度15段热电偶类型
  d_TempCtrlMW5_MOLDSET_FL_TKTJ_MW15,                    // 0x2f53009f, 温度16段热电偶类型
  d_TempCtrlMW5_MOLDSET_DACENT_HEATERON_MW0,             // 0x2f5300a0, 温度1段手动比例
  d_TempCtrlMW5_MOLDSET_DACENT_HEATERON_MW1,             // 0x2f5300a1, 温度2段手动比例
  d_TempCtrlMW5_MOLDSET_DACENT_HEATERON_MW2,             // 0x2f5300a2, 温度3段手动比例
  d_TempCtrlMW5_MOLDSET_DACENT_HEATERON_MW3,             // 0x2f5300a3, 温度4段手动比例
  d_TempCtrlMW5_MOLDSET_DACENT_HEATERON_MW4,             // 0x2f5300a4, 温度5段手动比例
  d_TempCtrlMW5_MOLDSET_DACENT_HEATERON_MW5,             // 0x2f5300a5, 温度6段手动比例
  d_TempCtrlMW5_MOLDSET_DACENT_HEATERON_MW6,             // 0x2f5300a6, 温度7段手动比例
  d_TempCtrlMW5_MOLDSET_DACENT_HEATERON_MW7,             // 0x2f5300a7, 温度8段手动比例
  d_TempCtrlMW5_MOLDSET_DACENT_HEATERON_MW8,             // 0x2f5300a8, 温度9段手动比例
  d_TempCtrlMW5_MOLDSET_DACENT_HEATERON_MW9,             // 0x2f5300a9, 温度10段手动比例
  d_TempCtrlMW5_MOLDSET_DACENT_HEATERON_MW10,            // 0x2f5300aa, 温度11段手动比例
  d_TempCtrlMW5_MOLDSET_DACENT_HEATERON_MW11,            // 0x2f5300ab, 温度12段手动比例
  d_TempCtrlMW5_MOLDSET_DACENT_HEATERON_MW12,            // 0x2f5300ac, 温度13段手动比例
  d_TempCtrlMW5_MOLDSET_DACENT_HEATERON_MW13,            // 0x2f5300ad, 温度14段手动比例
  d_TempCtrlMW5_MOLDSET_DACENT_HEATERON_MW14,            // 0x2f5300ae, 温度15段手动比例
  d_TempCtrlMW5_MOLDSET_DACENT_HEATERON_MW15,            // 0x2f5300af, 温度16段手动比例
  d_TempCtrlMW5_MOLDSET_FL_TEMPHOLD_MW,                  // 0x2f5300b0, 保温功能
  d_TempCtrlMW5_MOLDSET_DATEMP_HOLD_MW,                  // 0x2f5300b1, 保温温度
  d_TempCtrlMW5_MOLDSET_DATEMP_SCTM_MIN_MW0,             // 0x2f5300b2, 最小扫描周期
  d_TempCtrlMW5_MOLDSET_FL_TEMPUPCHECK_MW,               // 0x2f5300b3, 模温连续加温检查
  d_TempCtrlMW5_MOLDSET_DATEMP_NUM_MW,                   // 0x2f5300b4, 温度段数
  d_TempCtrlMW5_MOLDSET_TM_HEATERWARM_MW,                // 0x2f5300b5, 预温计时
  d_TempCtrlMW5_MOLDSET_FL_USE_MW,                       // 0x2f5300b6, 模温功能
  d_TempCtrlMW5_MOLDSET_FL_TEMP_YH,                      // 0x2f5300b7, 温度优化命令
  d_TempCtrlMW5_MOLDSET_FL_TEMP_YHQZ,                    // 0x2f5300b8, 温度强制优化命令
  d_TempCtrlMW5_MOLDSET_CONFUNC_MW,                      // 0x2f5300b9, 同步升温功能
  d_TempCtrlMW5_MOLDSET_FL_INJ_ALLOW                     // 0x2f5300ba, 射出检测模温温度
};

enum db_TempCtrlMW5_STATE{
  d_TempCtrlMW5_STATE_FL_ERROR0 = 0x2f560000, // 0x2f560000, 警报0
  d_TempCtrlMW5_STATE_FL_ERROR1,              // 0x2f560001, 警报1
  d_TempCtrlMW5_STATE_DATEMP_REAL_MW0,        // 0x2f560002, 温度1段实际值
  d_TempCtrlMW5_STATE_DATEMP_REAL_MW1,        // 0x2f560003, 温度2段实际值
  d_TempCtrlMW5_STATE_DATEMP_REAL_MW2,        // 0x2f560004, 温度3段实际值
  d_TempCtrlMW5_STATE_DATEMP_REAL_MW3,        // 0x2f560005, 温度4段实际值
  d_TempCtrlMW5_STATE_DATEMP_REAL_MW4,        // 0x2f560006, 温度5段实际值
  d_TempCtrlMW5_STATE_DATEMP_REAL_MW5,        // 0x2f560007, 温度6段实际值
  d_TempCtrlMW5_STATE_DATEMP_REAL_MW6,        // 0x2f560008, 温度7段实际值
  d_TempCtrlMW5_STATE_DATEMP_REAL_MW7,        // 0x2f560009, 温度8段实际值
  d_TempCtrlMW5_STATE_DATEMP_REAL_MW8,        // 0x2f56000a, 温度9段实际值
  d_TempCtrlMW5_STATE_DATEMP_REAL_MW9,        // 0x2f56000b, 温度10段实际值
  d_TempCtrlMW5_STATE_DATEMP_REAL_MW10,       // 0x2f56000c, 温度11段实际值
  d_TempCtrlMW5_STATE_DATEMP_REAL_MW11,       // 0x2f56000d, 温度12段实际值
  d_TempCtrlMW5_STATE_DATEMP_REAL_MW12,       // 0x2f56000e, 温度13段实际值
  d_TempCtrlMW5_STATE_DATEMP_REAL_MW13,       // 0x2f56000f, 温度14段实际值
  d_TempCtrlMW5_STATE_DATEMP_REAL_MW14,       // 0x2f560010, 温度15段实际值
  d_TempCtrlMW5_STATE_DATEMP_REAL_MW15,       // 0x2f560011, 温度16段实际值
  d_TempCtrlMW5_STATE_FL_STATUS_MW0,          // 0x2f560012, 温度1段状态
  d_TempCtrlMW5_STATE_FL_STATUS_MW1,          // 0x2f560013, 温度2段状态
  d_TempCtrlMW5_STATE_FL_STATUS_MW2,          // 0x2f560014, 温度3段状态
  d_TempCtrlMW5_STATE_FL_STATUS_MW3,          // 0x2f560015, 温度4段状态
  d_TempCtrlMW5_STATE_FL_STATUS_MW4,          // 0x2f560016, 温度5段状态
  d_TempCtrlMW5_STATE_FL_STATUS_MW5,          // 0x2f560017, 温度6段状态
  d_TempCtrlMW5_STATE_FL_STATUS_MW6,          // 0x2f560018, 温度7段状态
  d_TempCtrlMW5_STATE_FL_STATUS_MW7,          // 0x2f560019, 温度8段状态
  d_TempCtrlMW5_STATE_FL_STATUS_MW8,          // 0x2f56001a, 温度9段状态
  d_TempCtrlMW5_STATE_FL_STATUS_MW9,          // 0x2f56001b, 温度10段状态
  d_TempCtrlMW5_STATE_FL_STATUS_MW10,         // 0x2f56001c, 温度11段状态
  d_TempCtrlMW5_STATE_FL_STATUS_MW11,         // 0x2f56001d, 温度12段状态
  d_TempCtrlMW5_STATE_FL_STATUS_MW12,         // 0x2f56001e, 温度13段状态
  d_TempCtrlMW5_STATE_FL_STATUS_MW13,         // 0x2f56001f, 温度14段状态
  d_TempCtrlMW5_STATE_FL_STATUS_MW14,         // 0x2f560020, 温度15段状态
  d_TempCtrlMW5_STATE_FL_STATUS_MW15,         // 0x2f560021, 温度16段状态
  d_TempCtrlMW5_STATE_OUTPUT_PER_MW0,         // 0x2f560022, 温度1段输出实际值百分比
  d_TempCtrlMW5_STATE_OUTPUT_PER_MW1,         // 0x2f560023, 温度2段输出实际值百分比
  d_TempCtrlMW5_STATE_OUTPUT_PER_MW2,         // 0x2f560024, 温度3段输出实际值百分比
  d_TempCtrlMW5_STATE_OUTPUT_PER_MW3,         // 0x2f560025, 温度4段输出实际值百分比
  d_TempCtrlMW5_STATE_OUTPUT_PER_MW4,         // 0x2f560026, 温度5段输出实际值百分比
  d_TempCtrlMW5_STATE_OUTPUT_PER_MW5,         // 0x2f560027, 温度6段输出实际值百分比
  d_TempCtrlMW5_STATE_OUTPUT_PER_MW6,         // 0x2f560028, 温度7段输出实际值百分比
  d_TempCtrlMW5_STATE_OUTPUT_PER_MW7,         // 0x2f560029, 温度8段输出实际值百分比
  d_TempCtrlMW5_STATE_OUTPUT_PER_MW8,         // 0x2f56002a, 温度9段输出实际值百分比
  d_TempCtrlMW5_STATE_OUTPUT_PER_MW9,         // 0x2f56002b, 温度10段输出实际值百分比
  d_TempCtrlMW5_STATE_OUTPUT_PER_MW10,        // 0x2f56002c, 温度11段输出实际值百分比
  d_TempCtrlMW5_STATE_OUTPUT_PER_MW11,        // 0x2f56002d, 温度12段输出实际值百分比
  d_TempCtrlMW5_STATE_OUTPUT_PER_MW12,        // 0x2f56002e, 温度13段输出实际值百分比
  d_TempCtrlMW5_STATE_OUTPUT_PER_MW13,        // 0x2f56002f, 温度14段输出实际值百分比
  d_TempCtrlMW5_STATE_OUTPUT_PER_MW14,        // 0x2f560030, 温度15段输出实际值百分比
  d_TempCtrlMW5_STATE_OUTPUT_PER_MW15,        // 0x2f560031, 温度16段输出实际值百分比
  d_TempCtrlMW5_STATE_WTEMP_LM73_MW,          // 0x2f560032, 室温的温度
  d_TempCtrlMW5_STATE_FL_HEATERSTATUS_MW,     // 0x2f560033, 电热状态
  d_TempCtrlMW5_STATE_DATEMP_VER_MW,          // 0x2f560034, 温度扩展板E501版本号
  d_TempCtrlMW5_STATE_DATEMP_ERR_MW,          // 0x2f560035, 模温扩展板错误状态
  d_TempCtrlMW5_STATE_WRITE_CNT_E501,         // 0x2f560036, E501通讯测定写入次数
  d_TempCtrlMW5_STATE_READ_CNT_E501,          // 0x2f560037, E501通讯测试读入次数
  d_TempCtrlMW5_STATE_DATEMP_YOUHUAXUQIU,     // 0x2f560038, 温度优化提示开电热的闪烁指示
  d_TempCtrlMW5_STATE_DATEMP_YOUHUAOK,        // 0x2f560039, 温度优化开启与完成标记
  d_TempCtrlMW5_STATE_WSTATERSV6              // 0x2f56003a, 预温倒计时
};

enum db_TempCtrlMW6_PO{
  d_TempCtrlMW6_PO_O_COOL_MW = 0x2f600000  // 0x2f600000, 冷却水
};

enum db_TempCtrlMW6_PI{
  d_TempCtrlMW6_PI_I_HEAT_MW = 0x2f610000  // 0x2f610000, 加热接触器
};

enum db_TempCtrlMW6_MACHSET{
  d_TempCtrlMW6_MACHSET_TEMP_PROTECT_MW = 0x2f620000, // 0x2f620000, 温度保护上限
  d_TempCtrlMW6_MACHSET_TEMP_CONTROL_MW,              // 0x2f620001, 温度控制
  d_TempCtrlMW6_MACHSET_TEMP_CANLINK                  // 0x2f620002, 模温板接线
};

enum db_TempCtrlMW6_MOLDSET{
  d_TempCtrlMW6_MOLDSET_DATEMP_SETTING_MW0 = 0x2f630000, // 0x2f630000, 温度1段设定值
  d_TempCtrlMW6_MOLDSET_DATEMP_SETTING_MW1,              // 0x2f630001, 温度2段设定值
  d_TempCtrlMW6_MOLDSET_DATEMP_SETTING_MW2,              // 0x2f630002, 温度3段设定值
  d_TempCtrlMW6_MOLDSET_DATEMP_SETTING_MW3,              // 0x2f630003, 温度4段设定值
  d_TempCtrlMW6_MOLDSET_DATEMP_SETTING_MW4,              // 0x2f630004, 温度5段设定值
  d_TempCtrlMW6_MOLDSET_DATEMP_SETTING_MW5,              // 0x2f630005, 温度6段设定值
  d_TempCtrlMW6_MOLDSET_DATEMP_SETTING_MW6,              // 0x2f630006, 温度7段设定值
  d_TempCtrlMW6_MOLDSET_DATEMP_SETTING_MW7,              // 0x2f630007, 温度8段设定值
  d_TempCtrlMW6_MOLDSET_DATEMP_SETTING_MW8,              // 0x2f630008, 温度9段设定值
  d_TempCtrlMW6_MOLDSET_DATEMP_SETTING_MW9,              // 0x2f630009, 温度10段设定值
  d_TempCtrlMW6_MOLDSET_DATEMP_SETTING_MW10,             // 0x2f63000a, 温度11段设定值
  d_TempCtrlMW6_MOLDSET_DATEMP_SETTING_MW11,             // 0x2f63000b, 温度12段设定值
  d_TempCtrlMW6_MOLDSET_DATEMP_SETTING_MW12,             // 0x2f63000c, 温度13段设定值
  d_TempCtrlMW6_MOLDSET_DATEMP_SETTING_MW13,             // 0x2f63000d, 温度14段设定值
  d_TempCtrlMW6_MOLDSET_DATEMP_SETTING_MW14,             // 0x2f63000e, 温度15段设定值
  d_TempCtrlMW6_MOLDSET_DATEMP_SETTING_MW15,             // 0x2f63000f, 温度16段设定值
  d_TempCtrlMW6_MOLDSET_DATEMP_FUNC_MW0,                 // 0x2f630010, 温控1段功能选择
  d_TempCtrlMW6_MOLDSET_DATEMP_FUNC_MW1,                 // 0x2f630011, 温控2段功能选择
  d_TempCtrlMW6_MOLDSET_DATEMP_FUNC_MW2,                 // 0x2f630012, 温控3段功能选择
  d_TempCtrlMW6_MOLDSET_DATEMP_FUNC_MW3,                 // 0x2f630013, 温控4段功能选择
  d_TempCtrlMW6_MOLDSET_DATEMP_FUNC_MW4,                 // 0x2f630014, 温控5段功能选择
  d_TempCtrlMW6_MOLDSET_DATEMP_FUNC_MW5,                 // 0x2f630015, 温控6段功能选择
  d_TempCtrlMW6_MOLDSET_DATEMP_FUNC_MW6,                 // 0x2f630016, 温控7段功能选择
  d_TempCtrlMW6_MOLDSET_DATEMP_FUNC_MW7,                 // 0x2f630017, 温控8段功能选择
  d_TempCtrlMW6_MOLDSET_DATEMP_FUNC_MW8,                 // 0x2f630018, 温控9段功能选择
  d_TempCtrlMW6_MOLDSET_DATEMP_FUNC_MW9,                 // 0x2f630019, 温控10段功能选择
  d_TempCtrlMW6_MOLDSET_DATEMP_FUNC_MW10,                // 0x2f63001a, 温控11段功能选择
  d_TempCtrlMW6_MOLDSET_DATEMP_FUNC_MW11,                // 0x2f63001b, 温控12段功能选择
  d_TempCtrlMW6_MOLDSET_DATEMP_FUNC_MW12,                // 0x2f63001c, 温控13段功能选择
  d_TempCtrlMW6_MOLDSET_DATEMP_FUNC_MW13,                // 0x2f63001d, 温控14段功能选择
  d_TempCtrlMW6_MOLDSET_DATEMP_FUNC_MW14,                // 0x2f63001e, 温控15段功能选择
  d_TempCtrlMW6_MOLDSET_DATEMP_FUNC_MW15,                // 0x2f63001f, 温控16段功能选择
  d_TempCtrlMW6_MOLDSET_DAPID_KP_MW0,                    // 0x2f630020, 温度1段P系数
  d_TempCtrlMW6_MOLDSET_DAPID_KP_MW1,                    // 0x2f630021, 温度2段P系数
  d_TempCtrlMW6_MOLDSET_DAPID_KP_MW2,                    // 0x2f630022, 温度3段P系数
  d_TempCtrlMW6_MOLDSET_DAPID_KP_MW3,                    // 0x2f630023, 温度4段P系数
  d_TempCtrlMW6_MOLDSET_DAPID_KP_MW4,                    // 0x2f630024, 温度5段P系数
  d_TempCtrlMW6_MOLDSET_DAPID_KP_MW5,                    // 0x2f630025, 温度6段P系数
  d_TempCtrlMW6_MOLDSET_DAPID_KP_MW6,                    // 0x2f630026, 温度7段P系数
  d_TempCtrlMW6_MOLDSET_DAPID_KP_MW7,                    // 0x2f630027, 温度8段P系数
  d_TempCtrlMW6_MOLDSET_DAPID_KP_MW8,                    // 0x2f630028, 温度9段P系数
  d_TempCtrlMW6_MOLDSET_DAPID_KP_MW9,                    // 0x2f630029, 温度10段P系数
  d_TempCtrlMW6_MOLDSET_DAPID_KP_MW10,                   // 0x2f63002a, 温度11段P系数
  d_TempCtrlMW6_MOLDSET_DAPID_KP_MW11,                   // 0x2f63002b, 温度12段P系数
  d_TempCtrlMW6_MOLDSET_DAPID_KP_MW12,                   // 0x2f63002c, 温度13段P系数
  d_TempCtrlMW6_MOLDSET_DAPID_KP_MW13,                   // 0x2f63002d, 温度14段P系数
  d_TempCtrlMW6_MOLDSET_DAPID_KP_MW14,                   // 0x2f63002e, 温度15段P系数
  d_TempCtrlMW6_MOLDSET_DAPID_KP_MW15,                   // 0x2f63002f, 温度16段P系数
  d_TempCtrlMW6_MOLDSET_DAPID_TI_MW0,                    // 0x2f630030, 温度1段I系数
  d_TempCtrlMW6_MOLDSET_DAPID_TI_MW1,                    // 0x2f630031, 温度2段I系数
  d_TempCtrlMW6_MOLDSET_DAPID_TI_MW2,                    // 0x2f630032, 温度3段I系数
  d_TempCtrlMW6_MOLDSET_DAPID_TI_MW3,                    // 0x2f630033, 温度4段I系数
  d_TempCtrlMW6_MOLDSET_DAPID_TI_MW4,                    // 0x2f630034, 温度5段I系数
  d_TempCtrlMW6_MOLDSET_DAPID_TI_MW5,                    // 0x2f630035, 温度6段I系数
  d_TempCtrlMW6_MOLDSET_DAPID_TI_MW6,                    // 0x2f630036, 温度7段I系数
  d_TempCtrlMW6_MOLDSET_DAPID_TI_MW7,                    // 0x2f630037, 温度8段I系数
  d_TempCtrlMW6_MOLDSET_DAPID_TI_MW8,                    // 0x2f630038, 温度9段I系数
  d_TempCtrlMW6_MOLDSET_DAPID_TI_MW9,                    // 0x2f630039, 温度10段I系数
  d_TempCtrlMW6_MOLDSET_DAPID_TI_MW10,                   // 0x2f63003a, 温度11段I系数
  d_TempCtrlMW6_MOLDSET_DAPID_TI_MW11,                   // 0x2f63003b, 温度12段I系数
  d_TempCtrlMW6_MOLDSET_DAPID_TI_MW12,                   // 0x2f63003c, 温度13段I系数
  d_TempCtrlMW6_MOLDSET_DAPID_TI_MW13,                   // 0x2f63003d, 温度14段I系数
  d_TempCtrlMW6_MOLDSET_DAPID_TI_MW14,                   // 0x2f63003e, 温度15段I系数
  d_TempCtrlMW6_MOLDSET_DAPID_TI_MW15,                   // 0x2f63003f, 温度16段I系数
  d_TempCtrlMW6_MOLDSET_DAPID_TD_MW0,                    // 0x2f630040, 温度1段D系数
  d_TempCtrlMW6_MOLDSET_DAPID_TD_MW1,                    // 0x2f630041, 温度2段D系数
  d_TempCtrlMW6_MOLDSET_DAPID_TD_MW2,                    // 0x2f630042, 温度3段D系数
  d_TempCtrlMW6_MOLDSET_DAPID_TD_MW3,                    // 0x2f630043, 温度4段D系数
  d_TempCtrlMW6_MOLDSET_DAPID_TD_MW4,                    // 0x2f630044, 温度5段D系数
  d_TempCtrlMW6_MOLDSET_DAPID_TD_MW5,                    // 0x2f630045, 温度6段D系数
  d_TempCtrlMW6_MOLDSET_DAPID_TD_MW6,                    // 0x2f630046, 温度7段D系数
  d_TempCtrlMW6_MOLDSET_DAPID_TD_MW7,                    // 0x2f630047, 温度8段D系数
  d_TempCtrlMW6_MOLDSET_DAPID_TD_MW8,                    // 0x2f630048, 温度9段D系数
  d_TempCtrlMW6_MOLDSET_DAPID_TD_MW9,                    // 0x2f630049, 温度10段D系数
  d_TempCtrlMW6_MOLDSET_DAPID_TD_MW10,                   // 0x2f63004a, 温度11段D系数
  d_TempCtrlMW6_MOLDSET_DAPID_TD_MW11,                   // 0x2f63004b, 温度12段D系数
  d_TempCtrlMW6_MOLDSET_DAPID_TD_MW12,                   // 0x2f63004c, 温度13段D系数
  d_TempCtrlMW6_MOLDSET_DAPID_TD_MW13,                   // 0x2f63004d, 温度14段D系数
  d_TempCtrlMW6_MOLDSET_DAPID_TD_MW14,                   // 0x2f63004e, 温度15段D系数
  d_TempCtrlMW6_MOLDSET_DAPID_TD_MW15,                   // 0x2f63004f, 温度16段D系数
  d_TempCtrlMW6_MOLDSET_DATEMP_RAMP_MW0,                 // 0x2f630050, 温度1段加温缓冲区
  d_TempCtrlMW6_MOLDSET_DATEMP_RAMP_MW1,                 // 0x2f630051, 温度2段加温缓冲区
  d_TempCtrlMW6_MOLDSET_DATEMP_RAMP_MW2,                 // 0x2f630052, 温度3段加温缓冲区
  d_TempCtrlMW6_MOLDSET_DATEMP_RAMP_MW3,                 // 0x2f630053, 温度4段加温缓冲区
  d_TempCtrlMW6_MOLDSET_DATEMP_RAMP_MW4,                 // 0x2f630054, 温度5段加温缓冲区
  d_TempCtrlMW6_MOLDSET_DATEMP_RAMP_MW5,                 // 0x2f630055, 温度6段加温缓冲区
  d_TempCtrlMW6_MOLDSET_DATEMP_RAMP_MW6,                 // 0x2f630056, 温度7段加温缓冲区
  d_TempCtrlMW6_MOLDSET_DATEMP_RAMP_MW7,                 // 0x2f630057, 温度8段加温缓冲区
  d_TempCtrlMW6_MOLDSET_DATEMP_RAMP_MW8,                 // 0x2f630058, 温度9段加温缓冲区
  d_TempCtrlMW6_MOLDSET_DATEMP_RAMP_MW9,                 // 0x2f630059, 温度10段加温缓冲区
  d_TempCtrlMW6_MOLDSET_DATEMP_RAMP_MW10,                // 0x2f63005a, 温度11段加温缓冲区
  d_TempCtrlMW6_MOLDSET_DATEMP_RAMP_MW11,                // 0x2f63005b, 温度12段加温缓冲区
  d_TempCtrlMW6_MOLDSET_DATEMP_RAMP_MW12,                // 0x2f63005c, 温度13段加温缓冲区
  d_TempCtrlMW6_MOLDSET_DATEMP_RAMP_MW13,                // 0x2f63005d, 温度14段加温缓冲区
  d_TempCtrlMW6_MOLDSET_DATEMP_RAMP_MW14,                // 0x2f63005e, 温度15段加温缓冲区
  d_TempCtrlMW6_MOLDSET_DATEMP_RAMP_MW15,                // 0x2f63005f, 温度16段加温缓冲区
  d_TempCtrlMW6_MOLDSET_TM_RESPONSELESS_MW0,             // 0x2f630060, 温度1段输出周期时间
  d_TempCtrlMW6_MOLDSET_TM_RESPONSELESS_MW1,             // 0x2f630061, 温度2段输出周期时间
  d_TempCtrlMW6_MOLDSET_TM_RESPONSELESS_MW2,             // 0x2f630062, 温度3段输出周期时间
  d_TempCtrlMW6_MOLDSET_TM_RESPONSELESS_MW3,             // 0x2f630063, 温度4段输出周期时间
  d_TempCtrlMW6_MOLDSET_TM_RESPONSELESS_MW4,             // 0x2f630064, 温度5段输出周期时间
  d_TempCtrlMW6_MOLDSET_TM_RESPONSELESS_MW5,             // 0x2f630065, 温度6段输出周期时间
  d_TempCtrlMW6_MOLDSET_TM_RESPONSELESS_MW6,             // 0x2f630066, 温度7段输出周期时间
  d_TempCtrlMW6_MOLDSET_TM_RESPONSELESS_MW7,             // 0x2f630067, 温度8段输出周期时间
  d_TempCtrlMW6_MOLDSET_TM_RESPONSELESS_MW8,             // 0x2f630068, 温度9段输出周期时间
  d_TempCtrlMW6_MOLDSET_TM_RESPONSELESS_MW9,             // 0x2f630069, 温度10段输出周期时间
  d_TempCtrlMW6_MOLDSET_TM_RESPONSELESS_MW10,            // 0x2f63006a, 温度11段输出周期时间
  d_TempCtrlMW6_MOLDSET_TM_RESPONSELESS_MW11,            // 0x2f63006b, 温度12段输出周期时间
  d_TempCtrlMW6_MOLDSET_TM_RESPONSELESS_MW12,            // 0x2f63006c, 温度13段输出周期时间
  d_TempCtrlMW6_MOLDSET_TM_RESPONSELESS_MW13,            // 0x2f63006d, 温度14段输出周期时间
  d_TempCtrlMW6_MOLDSET_TM_RESPONSELESS_MW14,            // 0x2f63006e, 温度15段输出周期时间
  d_TempCtrlMW6_MOLDSET_TM_RESPONSELESS_MW15,            // 0x2f63006f, 温度16段输出周期时间
  d_TempCtrlMW6_MOLDSET_DATEMP_MAX_MW0,                  // 0x2f630070, 温度1段上偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MAX_MW1,                  // 0x2f630071, 温度2段上偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MAX_MW2,                  // 0x2f630072, 温度3段上偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MAX_MW3,                  // 0x2f630073, 温度4段上偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MAX_MW4,                  // 0x2f630074, 温度5段上偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MAX_MW5,                  // 0x2f630075, 温度6段上偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MAX_MW6,                  // 0x2f630076, 温度7段上偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MAX_MW7,                  // 0x2f630077, 温度8段上偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MAX_MW8,                  // 0x2f630078, 温度9段上偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MAX_MW9,                  // 0x2f630079, 温度10段上偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MAX_MW10,                 // 0x2f63007a, 温度11段上偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MAX_MW11,                 // 0x2f63007b, 温度12段上偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MAX_MW12,                 // 0x2f63007c, 温度13段上偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MAX_MW13,                 // 0x2f63007d, 温度14段上偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MAX_MW14,                 // 0x2f63007e, 温度15段上偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MAX_MW15,                 // 0x2f63007f, 温度16段上偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MIN_MW0,                  // 0x2f630080, 温度1段下偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MIN_MW1,                  // 0x2f630081, 温度2段下偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MIN_MW2,                  // 0x2f630082, 温度3段下偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MIN_MW3,                  // 0x2f630083, 温度4段下偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MIN_MW4,                  // 0x2f630084, 温度5段下偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MIN_MW5,                  // 0x2f630085, 温度6段下偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MIN_MW6,                  // 0x2f630086, 温度7段下偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MIN_MW7,                  // 0x2f630087, 温度8段下偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MIN_MW8,                  // 0x2f630088, 温度9段下偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MIN_MW9,                  // 0x2f630089, 温度10段下偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MIN_MW10,                 // 0x2f63008a, 温度11段下偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MIN_MW11,                 // 0x2f63008b, 温度12段下偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MIN_MW12,                 // 0x2f63008c, 温度13段下偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MIN_MW13,                 // 0x2f63008d, 温度14段下偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MIN_MW14,                 // 0x2f63008e, 温度15段下偏差
  d_TempCtrlMW6_MOLDSET_DATEMP_MIN_MW15,                 // 0x2f63008f, 温度16段下偏差
  d_TempCtrlMW6_MOLDSET_FL_TKTJ_MW0,                     // 0x2f630090, 温度1段热电偶类型
  d_TempCtrlMW6_MOLDSET_FL_TKTJ_MW1,                     // 0x2f630091, 温度2段热电偶类型
  d_TempCtrlMW6_MOLDSET_FL_TKTJ_MW2,                     // 0x2f630092, 温度3段热电偶类型
  d_TempCtrlMW6_MOLDSET_FL_TKTJ_MW3,                     // 0x2f630093, 温度4段热电偶类型
  d_TempCtrlMW6_MOLDSET_FL_TKTJ_MW4,                     // 0x2f630094, 温度5段热电偶类型
  d_TempCtrlMW6_MOLDSET_FL_TKTJ_MW5,                     // 0x2f630095, 温度6段热电偶类型
  d_TempCtrlMW6_MOLDSET_FL_TKTJ_MW6,                     // 0x2f630096, 温度7段热电偶类型
  d_TempCtrlMW6_MOLDSET_FL_TKTJ_MW7,                     // 0x2f630097, 温度8段热电偶类型
  d_TempCtrlMW6_MOLDSET_FL_TKTJ_MW8,                     // 0x2f630098, 温度9段热电偶类型
  d_TempCtrlMW6_MOLDSET_FL_TKTJ_MW9,                     // 0x2f630099, 温度10段热电偶类型
  d_TempCtrlMW6_MOLDSET_FL_TKTJ_MW10,                    // 0x2f63009a, 温度11段热电偶类型
  d_TempCtrlMW6_MOLDSET_FL_TKTJ_MW11,                    // 0x2f63009b, 温度12段热电偶类型
  d_TempCtrlMW6_MOLDSET_FL_TKTJ_MW12,                    // 0x2f63009c, 温度13段热电偶类型
  d_TempCtrlMW6_MOLDSET_FL_TKTJ_MW13,                    // 0x2f63009d, 温度14段热电偶类型
  d_TempCtrlMW6_MOLDSET_FL_TKTJ_MW14,                    // 0x2f63009e, 温度15段热电偶类型
  d_TempCtrlMW6_MOLDSET_FL_TKTJ_MW15,                    // 0x2f63009f, 温度16段热电偶类型
  d_TempCtrlMW6_MOLDSET_DACENT_HEATERON_MW0,             // 0x2f6300a0, 温度1段手动比例
  d_TempCtrlMW6_MOLDSET_DACENT_HEATERON_MW1,             // 0x2f6300a1, 温度2段手动比例
  d_TempCtrlMW6_MOLDSET_DACENT_HEATERON_MW2,             // 0x2f6300a2, 温度3段手动比例
  d_TempCtrlMW6_MOLDSET_DACENT_HEATERON_MW3,             // 0x2f6300a3, 温度4段手动比例
  d_TempCtrlMW6_MOLDSET_DACENT_HEATERON_MW4,             // 0x2f6300a4, 温度5段手动比例
  d_TempCtrlMW6_MOLDSET_DACENT_HEATERON_MW5,             // 0x2f6300a5, 温度6段手动比例
  d_TempCtrlMW6_MOLDSET_DACENT_HEATERON_MW6,             // 0x2f6300a6, 温度7段手动比例
  d_TempCtrlMW6_MOLDSET_DACENT_HEATERON_MW7,             // 0x2f6300a7, 温度8段手动比例
  d_TempCtrlMW6_MOLDSET_DACENT_HEATERON_MW8,             // 0x2f6300a8, 温度9段手动比例
  d_TempCtrlMW6_MOLDSET_DACENT_HEATERON_MW9,             // 0x2f6300a9, 温度10段手动比例
  d_TempCtrlMW6_MOLDSET_DACENT_HEATERON_MW10,            // 0x2f6300aa, 温度11段手动比例
  d_TempCtrlMW6_MOLDSET_DACENT_HEATERON_MW11,            // 0x2f6300ab, 温度12段手动比例
  d_TempCtrlMW6_MOLDSET_DACENT_HEATERON_MW12,            // 0x2f6300ac, 温度13段手动比例
  d_TempCtrlMW6_MOLDSET_DACENT_HEATERON_MW13,            // 0x2f6300ad, 温度14段手动比例
  d_TempCtrlMW6_MOLDSET_DACENT_HEATERON_MW14,            // 0x2f6300ae, 温度15段手动比例
  d_TempCtrlMW6_MOLDSET_DACENT_HEATERON_MW15,            // 0x2f6300af, 温度16段手动比例
  d_TempCtrlMW6_MOLDSET_FL_TEMPHOLD_MW,                  // 0x2f6300b0, 保温功能
  d_TempCtrlMW6_MOLDSET_DATEMP_HOLD_MW,                  // 0x2f6300b1, 保温温度
  d_TempCtrlMW6_MOLDSET_DATEMP_SCTM_MIN_MW0,             // 0x2f6300b2, 最小扫描周期
  d_TempCtrlMW6_MOLDSET_FL_TEMPUPCHECK_MW,               // 0x2f6300b3, 模温连续加温检查
  d_TempCtrlMW6_MOLDSET_DATEMP_NUM_MW,                   // 0x2f6300b4, 温度段数
  d_TempCtrlMW6_MOLDSET_TM_HEATERWARM_MW,                // 0x2f6300b5, 预温计时
  d_TempCtrlMW6_MOLDSET_FL_USE_MW,                       // 0x2f6300b6, 模温功能
  d_TempCtrlMW6_MOLDSET_FL_TEMP_YH,                      // 0x2f6300b7, 温度优化命令
  d_TempCtrlMW6_MOLDSET_FL_TEMP_YHQZ,                    // 0x2f6300b8, 温度强制优化命令
  d_TempCtrlMW6_MOLDSET_CONFUNC_MW,                      // 0x2f6300b9, 同步升温功能
  d_TempCtrlMW6_MOLDSET_FL_INJ_ALLOW                     // 0x2f6300ba, 射出检测模温温度
};

enum db_TempCtrlMW6_STATE{
  d_TempCtrlMW6_STATE_FL_ERROR0 = 0x2f660000, // 0x2f660000, 警报0
  d_TempCtrlMW6_STATE_FL_ERROR1,              // 0x2f660001, 警报1
  d_TempCtrlMW6_STATE_DATEMP_REAL_MW0,        // 0x2f660002, 温度1段实际值
  d_TempCtrlMW6_STATE_DATEMP_REAL_MW1,        // 0x2f660003, 温度2段实际值
  d_TempCtrlMW6_STATE_DATEMP_REAL_MW2,        // 0x2f660004, 温度3段实际值
  d_TempCtrlMW6_STATE_DATEMP_REAL_MW3,        // 0x2f660005, 温度4段实际值
  d_TempCtrlMW6_STATE_DATEMP_REAL_MW4,        // 0x2f660006, 温度5段实际值
  d_TempCtrlMW6_STATE_DATEMP_REAL_MW5,        // 0x2f660007, 温度6段实际值
  d_TempCtrlMW6_STATE_DATEMP_REAL_MW6,        // 0x2f660008, 温度7段实际值
  d_TempCtrlMW6_STATE_DATEMP_REAL_MW7,        // 0x2f660009, 温度8段实际值
  d_TempCtrlMW6_STATE_DATEMP_REAL_MW8,        // 0x2f66000a, 温度9段实际值
  d_TempCtrlMW6_STATE_DATEMP_REAL_MW9,        // 0x2f66000b, 温度10段实际值
  d_TempCtrlMW6_STATE_DATEMP_REAL_MW10,       // 0x2f66000c, 温度11段实际值
  d_TempCtrlMW6_STATE_DATEMP_REAL_MW11,       // 0x2f66000d, 温度12段实际值
  d_TempCtrlMW6_STATE_DATEMP_REAL_MW12,       // 0x2f66000e, 温度13段实际值
  d_TempCtrlMW6_STATE_DATEMP_REAL_MW13,       // 0x2f66000f, 温度14段实际值
  d_TempCtrlMW6_STATE_DATEMP_REAL_MW14,       // 0x2f660010, 温度15段实际值
  d_TempCtrlMW6_STATE_DATEMP_REAL_MW15,       // 0x2f660011, 温度16段实际值
  d_TempCtrlMW6_STATE_FL_STATUS_MW0,          // 0x2f660012, 温度1段状态
  d_TempCtrlMW6_STATE_FL_STATUS_MW1,          // 0x2f660013, 温度2段状态
  d_TempCtrlMW6_STATE_FL_STATUS_MW2,          // 0x2f660014, 温度3段状态
  d_TempCtrlMW6_STATE_FL_STATUS_MW3,          // 0x2f660015, 温度4段状态
  d_TempCtrlMW6_STATE_FL_STATUS_MW4,          // 0x2f660016, 温度5段状态
  d_TempCtrlMW6_STATE_FL_STATUS_MW5,          // 0x2f660017, 温度6段状态
  d_TempCtrlMW6_STATE_FL_STATUS_MW6,          // 0x2f660018, 温度7段状态
  d_TempCtrlMW6_STATE_FL_STATUS_MW7,          // 0x2f660019, 温度8段状态
  d_TempCtrlMW6_STATE_FL_STATUS_MW8,          // 0x2f66001a, 温度9段状态
  d_TempCtrlMW6_STATE_FL_STATUS_MW9,          // 0x2f66001b, 温度10段状态
  d_TempCtrlMW6_STATE_FL_STATUS_MW10,         // 0x2f66001c, 温度11段状态
  d_TempCtrlMW6_STATE_FL_STATUS_MW11,         // 0x2f66001d, 温度12段状态
  d_TempCtrlMW6_STATE_FL_STATUS_MW12,         // 0x2f66001e, 温度13段状态
  d_TempCtrlMW6_STATE_FL_STATUS_MW13,         // 0x2f66001f, 温度14段状态
  d_TempCtrlMW6_STATE_FL_STATUS_MW14,         // 0x2f660020, 温度15段状态
  d_TempCtrlMW6_STATE_FL_STATUS_MW15,         // 0x2f660021, 温度16段状态
  d_TempCtrlMW6_STATE_OUTPUT_PER_MW0,         // 0x2f660022, 温度1段输出实际值百分比
  d_TempCtrlMW6_STATE_OUTPUT_PER_MW1,         // 0x2f660023, 温度2段输出实际值百分比
  d_TempCtrlMW6_STATE_OUTPUT_PER_MW2,         // 0x2f660024, 温度3段输出实际值百分比
  d_TempCtrlMW6_STATE_OUTPUT_PER_MW3,         // 0x2f660025, 温度4段输出实际值百分比
  d_TempCtrlMW6_STATE_OUTPUT_PER_MW4,         // 0x2f660026, 温度5段输出实际值百分比
  d_TempCtrlMW6_STATE_OUTPUT_PER_MW5,         // 0x2f660027, 温度6段输出实际值百分比
  d_TempCtrlMW6_STATE_OUTPUT_PER_MW6,         // 0x2f660028, 温度7段输出实际值百分比
  d_TempCtrlMW6_STATE_OUTPUT_PER_MW7,         // 0x2f660029, 温度8段输出实际值百分比
  d_TempCtrlMW6_STATE_OUTPUT_PER_MW8,         // 0x2f66002a, 温度9段输出实际值百分比
  d_TempCtrlMW6_STATE_OUTPUT_PER_MW9,         // 0x2f66002b, 温度10段输出实际值百分比
  d_TempCtrlMW6_STATE_OUTPUT_PER_MW10,        // 0x2f66002c, 温度11段输出实际值百分比
  d_TempCtrlMW6_STATE_OUTPUT_PER_MW11,        // 0x2f66002d, 温度12段输出实际值百分比
  d_TempCtrlMW6_STATE_OUTPUT_PER_MW12,        // 0x2f66002e, 温度13段输出实际值百分比
  d_TempCtrlMW6_STATE_OUTPUT_PER_MW13,        // 0x2f66002f, 温度14段输出实际值百分比
  d_TempCtrlMW6_STATE_OUTPUT_PER_MW14,        // 0x2f660030, 温度15段输出实际值百分比
  d_TempCtrlMW6_STATE_OUTPUT_PER_MW15,        // 0x2f660031, 温度16段输出实际值百分比
  d_TempCtrlMW6_STATE_WTEMP_LM73_MW,          // 0x2f660032, 室温的温度
  d_TempCtrlMW6_STATE_FL_HEATERSTATUS_MW,     // 0x2f660033, 电热状态
  d_TempCtrlMW6_STATE_DATEMP_VER_MW,          // 0x2f660034, 温度扩展板E501版本号
  d_TempCtrlMW6_STATE_DATEMP_ERR_MW,          // 0x2f660035, 模温扩展板错误状态
  d_TempCtrlMW6_STATE_WRITE_CNT_E501,         // 0x2f660036, E501通讯测定写入次数
  d_TempCtrlMW6_STATE_READ_CNT_E501,          // 0x2f660037, E501通讯测试读入次数
  d_TempCtrlMW6_STATE_DATEMP_YOUHUAXUQIU,     // 0x2f660038, 温度优化提示开电热的闪烁指示
  d_TempCtrlMW6_STATE_DATEMP_YOUHUAOK,        // 0x2f660039, 温度优化开启与完成标记
  d_TempCtrlMW6_STATE_WSTATERSV6              // 0x2f66003a, 预温倒计时
};

enum db_TempCtrlMW7_PO{
  d_TempCtrlMW7_PO_O_COOL_MW = 0x2f700000  // 0x2f700000, 冷却水
};

enum db_TempCtrlMW7_PI{
  d_TempCtrlMW7_PI_I_HEAT_MW = 0x2f710000  // 0x2f710000, 加热接触器
};

enum db_TempCtrlMW7_MACHSET{
  d_TempCtrlMW7_MACHSET_TEMP_PROTECT_MW = 0x2f720000, // 0x2f720000, 温度保护上限
  d_TempCtrlMW7_MACHSET_TEMP_CONTROL_MW,              // 0x2f720001, 温度控制
  d_TempCtrlMW7_MACHSET_TEMP_CANLINK                  // 0x2f720002, 模温板接线
};

enum db_TempCtrlMW7_MOLDSET{
  d_TempCtrlMW7_MOLDSET_DATEMP_SETTING_MW0 = 0x2f730000, // 0x2f730000, 温度1段设定值
  d_TempCtrlMW7_MOLDSET_DATEMP_SETTING_MW1,              // 0x2f730001, 温度2段设定值
  d_TempCtrlMW7_MOLDSET_DATEMP_SETTING_MW2,              // 0x2f730002, 温度3段设定值
  d_TempCtrlMW7_MOLDSET_DATEMP_SETTING_MW3,              // 0x2f730003, 温度4段设定值
  d_TempCtrlMW7_MOLDSET_DATEMP_SETTING_MW4,              // 0x2f730004, 温度5段设定值
  d_TempCtrlMW7_MOLDSET_DATEMP_SETTING_MW5,              // 0x2f730005, 温度6段设定值
  d_TempCtrlMW7_MOLDSET_DATEMP_SETTING_MW6,              // 0x2f730006, 温度7段设定值
  d_TempCtrlMW7_MOLDSET_DATEMP_SETTING_MW7,              // 0x2f730007, 温度8段设定值
  d_TempCtrlMW7_MOLDSET_DATEMP_SETTING_MW8,              // 0x2f730008, 温度9段设定值
  d_TempCtrlMW7_MOLDSET_DATEMP_SETTING_MW9,              // 0x2f730009, 温度10段设定值
  d_TempCtrlMW7_MOLDSET_DATEMP_SETTING_MW10,             // 0x2f73000a, 温度11段设定值
  d_TempCtrlMW7_MOLDSET_DATEMP_SETTING_MW11,             // 0x2f73000b, 温度12段设定值
  d_TempCtrlMW7_MOLDSET_DATEMP_SETTING_MW12,             // 0x2f73000c, 温度13段设定值
  d_TempCtrlMW7_MOLDSET_DATEMP_SETTING_MW13,             // 0x2f73000d, 温度14段设定值
  d_TempCtrlMW7_MOLDSET_DATEMP_SETTING_MW14,             // 0x2f73000e, 温度15段设定值
  d_TempCtrlMW7_MOLDSET_DATEMP_SETTING_MW15,             // 0x2f73000f, 温度16段设定值
  d_TempCtrlMW7_MOLDSET_DATEMP_FUNC_MW0,                 // 0x2f730010, 温控1段功能选择
  d_TempCtrlMW7_MOLDSET_DATEMP_FUNC_MW1,                 // 0x2f730011, 温控2段功能选择
  d_TempCtrlMW7_MOLDSET_DATEMP_FUNC_MW2,                 // 0x2f730012, 温控3段功能选择
  d_TempCtrlMW7_MOLDSET_DATEMP_FUNC_MW3,                 // 0x2f730013, 温控4段功能选择
  d_TempCtrlMW7_MOLDSET_DATEMP_FUNC_MW4,                 // 0x2f730014, 温控5段功能选择
  d_TempCtrlMW7_MOLDSET_DATEMP_FUNC_MW5,                 // 0x2f730015, 温控6段功能选择
  d_TempCtrlMW7_MOLDSET_DATEMP_FUNC_MW6,                 // 0x2f730016, 温控7段功能选择
  d_TempCtrlMW7_MOLDSET_DATEMP_FUNC_MW7,                 // 0x2f730017, 温控8段功能选择
  d_TempCtrlMW7_MOLDSET_DATEMP_FUNC_MW8,                 // 0x2f730018, 温控9段功能选择
  d_TempCtrlMW7_MOLDSET_DATEMP_FUNC_MW9,                 // 0x2f730019, 温控10段功能选择
  d_TempCtrlMW7_MOLDSET_DATEMP_FUNC_MW10,                // 0x2f73001a, 温控11段功能选择
  d_TempCtrlMW7_MOLDSET_DATEMP_FUNC_MW11,                // 0x2f73001b, 温控12段功能选择
  d_TempCtrlMW7_MOLDSET_DATEMP_FUNC_MW12,                // 0x2f73001c, 温控13段功能选择
  d_TempCtrlMW7_MOLDSET_DATEMP_FUNC_MW13,                // 0x2f73001d, 温控14段功能选择
  d_TempCtrlMW7_MOLDSET_DATEMP_FUNC_MW14,                // 0x2f73001e, 温控15段功能选择
  d_TempCtrlMW7_MOLDSET_DATEMP_FUNC_MW15,                // 0x2f73001f, 温控16段功能选择
  d_TempCtrlMW7_MOLDSET_DAPID_KP_MW0,                    // 0x2f730020, 温度1段P系数
  d_TempCtrlMW7_MOLDSET_DAPID_KP_MW1,                    // 0x2f730021, 温度2段P系数
  d_TempCtrlMW7_MOLDSET_DAPID_KP_MW2,                    // 0x2f730022, 温度3段P系数
  d_TempCtrlMW7_MOLDSET_DAPID_KP_MW3,                    // 0x2f730023, 温度4段P系数
  d_TempCtrlMW7_MOLDSET_DAPID_KP_MW4,                    // 0x2f730024, 温度5段P系数
  d_TempCtrlMW7_MOLDSET_DAPID_KP_MW5,                    // 0x2f730025, 温度6段P系数
  d_TempCtrlMW7_MOLDSET_DAPID_KP_MW6,                    // 0x2f730026, 温度7段P系数
  d_TempCtrlMW7_MOLDSET_DAPID_KP_MW7,                    // 0x2f730027, 温度8段P系数
  d_TempCtrlMW7_MOLDSET_DAPID_KP_MW8,                    // 0x2f730028, 温度9段P系数
  d_TempCtrlMW7_MOLDSET_DAPID_KP_MW9,                    // 0x2f730029, 温度10段P系数
  d_TempCtrlMW7_MOLDSET_DAPID_KP_MW10,                   // 0x2f73002a, 温度11段P系数
  d_TempCtrlMW7_MOLDSET_DAPID_KP_MW11,                   // 0x2f73002b, 温度12段P系数
  d_TempCtrlMW7_MOLDSET_DAPID_KP_MW12,                   // 0x2f73002c, 温度13段P系数
  d_TempCtrlMW7_MOLDSET_DAPID_KP_MW13,                   // 0x2f73002d, 温度14段P系数
  d_TempCtrlMW7_MOLDSET_DAPID_KP_MW14,                   // 0x2f73002e, 温度15段P系数
  d_TempCtrlMW7_MOLDSET_DAPID_KP_MW15,                   // 0x2f73002f, 温度16段P系数
  d_TempCtrlMW7_MOLDSET_DAPID_TI_MW0,                    // 0x2f730030, 温度1段I系数
  d_TempCtrlMW7_MOLDSET_DAPID_TI_MW1,                    // 0x2f730031, 温度2段I系数
  d_TempCtrlMW7_MOLDSET_DAPID_TI_MW2,                    // 0x2f730032, 温度3段I系数
  d_TempCtrlMW7_MOLDSET_DAPID_TI_MW3,                    // 0x2f730033, 温度4段I系数
  d_TempCtrlMW7_MOLDSET_DAPID_TI_MW4,                    // 0x2f730034, 温度5段I系数
  d_TempCtrlMW7_MOLDSET_DAPID_TI_MW5,                    // 0x2f730035, 温度6段I系数
  d_TempCtrlMW7_MOLDSET_DAPID_TI_MW6,                    // 0x2f730036, 温度7段I系数
  d_TempCtrlMW7_MOLDSET_DAPID_TI_MW7,                    // 0x2f730037, 温度8段I系数
  d_TempCtrlMW7_MOLDSET_DAPID_TI_MW8,                    // 0x2f730038, 温度9段I系数
  d_TempCtrlMW7_MOLDSET_DAPID_TI_MW9,                    // 0x2f730039, 温度10段I系数
  d_TempCtrlMW7_MOLDSET_DAPID_TI_MW10,                   // 0x2f73003a, 温度11段I系数
  d_TempCtrlMW7_MOLDSET_DAPID_TI_MW11,                   // 0x2f73003b, 温度12段I系数
  d_TempCtrlMW7_MOLDSET_DAPID_TI_MW12,                   // 0x2f73003c, 温度13段I系数
  d_TempCtrlMW7_MOLDSET_DAPID_TI_MW13,                   // 0x2f73003d, 温度14段I系数
  d_TempCtrlMW7_MOLDSET_DAPID_TI_MW14,                   // 0x2f73003e, 温度15段I系数
  d_TempCtrlMW7_MOLDSET_DAPID_TI_MW15,                   // 0x2f73003f, 温度16段I系数
  d_TempCtrlMW7_MOLDSET_DAPID_TD_MW0,                    // 0x2f730040, 温度1段D系数
  d_TempCtrlMW7_MOLDSET_DAPID_TD_MW1,                    // 0x2f730041, 温度2段D系数
  d_TempCtrlMW7_MOLDSET_DAPID_TD_MW2,                    // 0x2f730042, 温度3段D系数
  d_TempCtrlMW7_MOLDSET_DAPID_TD_MW3,                    // 0x2f730043, 温度4段D系数
  d_TempCtrlMW7_MOLDSET_DAPID_TD_MW4,                    // 0x2f730044, 温度5段D系数
  d_TempCtrlMW7_MOLDSET_DAPID_TD_MW5,                    // 0x2f730045, 温度6段D系数
  d_TempCtrlMW7_MOLDSET_DAPID_TD_MW6,                    // 0x2f730046, 温度7段D系数
  d_TempCtrlMW7_MOLDSET_DAPID_TD_MW7,                    // 0x2f730047, 温度8段D系数
  d_TempCtrlMW7_MOLDSET_DAPID_TD_MW8,                    // 0x2f730048, 温度9段D系数
  d_TempCtrlMW7_MOLDSET_DAPID_TD_MW9,                    // 0x2f730049, 温度10段D系数
  d_TempCtrlMW7_MOLDSET_DAPID_TD_MW10,                   // 0x2f73004a, 温度11段D系数
  d_TempCtrlMW7_MOLDSET_DAPID_TD_MW11,                   // 0x2f73004b, 温度12段D系数
  d_TempCtrlMW7_MOLDSET_DAPID_TD_MW12,                   // 0x2f73004c, 温度13段D系数
  d_TempCtrlMW7_MOLDSET_DAPID_TD_MW13,                   // 0x2f73004d, 温度14段D系数
  d_TempCtrlMW7_MOLDSET_DAPID_TD_MW14,                   // 0x2f73004e, 温度15段D系数
  d_TempCtrlMW7_MOLDSET_DAPID_TD_MW15,                   // 0x2f73004f, 温度16段D系数
  d_TempCtrlMW7_MOLDSET_DATEMP_RAMP_MW0,                 // 0x2f730050, 温度1段加温缓冲区
  d_TempCtrlMW7_MOLDSET_DATEMP_RAMP_MW1,                 // 0x2f730051, 温度2段加温缓冲区
  d_TempCtrlMW7_MOLDSET_DATEMP_RAMP_MW2,                 // 0x2f730052, 温度3段加温缓冲区
  d_TempCtrlMW7_MOLDSET_DATEMP_RAMP_MW3,                 // 0x2f730053, 温度4段加温缓冲区
  d_TempCtrlMW7_MOLDSET_DATEMP_RAMP_MW4,                 // 0x2f730054, 温度5段加温缓冲区
  d_TempCtrlMW7_MOLDSET_DATEMP_RAMP_MW5,                 // 0x2f730055, 温度6段加温缓冲区
  d_TempCtrlMW7_MOLDSET_DATEMP_RAMP_MW6,                 // 0x2f730056, 温度7段加温缓冲区
  d_TempCtrlMW7_MOLDSET_DATEMP_RAMP_MW7,                 // 0x2f730057, 温度8段加温缓冲区
  d_TempCtrlMW7_MOLDSET_DATEMP_RAMP_MW8,                 // 0x2f730058, 温度9段加温缓冲区
  d_TempCtrlMW7_MOLDSET_DATEMP_RAMP_MW9,                 // 0x2f730059, 温度10段加温缓冲区
  d_TempCtrlMW7_MOLDSET_DATEMP_RAMP_MW10,                // 0x2f73005a, 温度11段加温缓冲区
  d_TempCtrlMW7_MOLDSET_DATEMP_RAMP_MW11,                // 0x2f73005b, 温度12段加温缓冲区
  d_TempCtrlMW7_MOLDSET_DATEMP_RAMP_MW12,                // 0x2f73005c, 温度13段加温缓冲区
  d_TempCtrlMW7_MOLDSET_DATEMP_RAMP_MW13,                // 0x2f73005d, 温度14段加温缓冲区
  d_TempCtrlMW7_MOLDSET_DATEMP_RAMP_MW14,                // 0x2f73005e, 温度15段加温缓冲区
  d_TempCtrlMW7_MOLDSET_DATEMP_RAMP_MW15,                // 0x2f73005f, 温度16段加温缓冲区
  d_TempCtrlMW7_MOLDSET_TM_RESPONSELESS_MW0,             // 0x2f730060, 温度1段输出周期时间
  d_TempCtrlMW7_MOLDSET_TM_RESPONSELESS_MW1,             // 0x2f730061, 温度2段输出周期时间
  d_TempCtrlMW7_MOLDSET_TM_RESPONSELESS_MW2,             // 0x2f730062, 温度3段输出周期时间
  d_TempCtrlMW7_MOLDSET_TM_RESPONSELESS_MW3,             // 0x2f730063, 温度4段输出周期时间
  d_TempCtrlMW7_MOLDSET_TM_RESPONSELESS_MW4,             // 0x2f730064, 温度5段输出周期时间
  d_TempCtrlMW7_MOLDSET_TM_RESPONSELESS_MW5,             // 0x2f730065, 温度6段输出周期时间
  d_TempCtrlMW7_MOLDSET_TM_RESPONSELESS_MW6,             // 0x2f730066, 温度7段输出周期时间
  d_TempCtrlMW7_MOLDSET_TM_RESPONSELESS_MW7,             // 0x2f730067, 温度8段输出周期时间
  d_TempCtrlMW7_MOLDSET_TM_RESPONSELESS_MW8,             // 0x2f730068, 温度9段输出周期时间
  d_TempCtrlMW7_MOLDSET_TM_RESPONSELESS_MW9,             // 0x2f730069, 温度10段输出周期时间
  d_TempCtrlMW7_MOLDSET_TM_RESPONSELESS_MW10,            // 0x2f73006a, 温度11段输出周期时间
  d_TempCtrlMW7_MOLDSET_TM_RESPONSELESS_MW11,            // 0x2f73006b, 温度12段输出周期时间
  d_TempCtrlMW7_MOLDSET_TM_RESPONSELESS_MW12,            // 0x2f73006c, 温度13段输出周期时间
  d_TempCtrlMW7_MOLDSET_TM_RESPONSELESS_MW13,            // 0x2f73006d, 温度14段输出周期时间
  d_TempCtrlMW7_MOLDSET_TM_RESPONSELESS_MW14,            // 0x2f73006e, 温度15段输出周期时间
  d_TempCtrlMW7_MOLDSET_TM_RESPONSELESS_MW15,            // 0x2f73006f, 温度16段输出周期时间
  d_TempCtrlMW7_MOLDSET_DATEMP_MAX_MW0,                  // 0x2f730070, 温度1段上偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MAX_MW1,                  // 0x2f730071, 温度2段上偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MAX_MW2,                  // 0x2f730072, 温度3段上偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MAX_MW3,                  // 0x2f730073, 温度4段上偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MAX_MW4,                  // 0x2f730074, 温度5段上偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MAX_MW5,                  // 0x2f730075, 温度6段上偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MAX_MW6,                  // 0x2f730076, 温度7段上偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MAX_MW7,                  // 0x2f730077, 温度8段上偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MAX_MW8,                  // 0x2f730078, 温度9段上偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MAX_MW9,                  // 0x2f730079, 温度10段上偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MAX_MW10,                 // 0x2f73007a, 温度11段上偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MAX_MW11,                 // 0x2f73007b, 温度12段上偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MAX_MW12,                 // 0x2f73007c, 温度13段上偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MAX_MW13,                 // 0x2f73007d, 温度14段上偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MAX_MW14,                 // 0x2f73007e, 温度15段上偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MAX_MW15,                 // 0x2f73007f, 温度16段上偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MIN_MW0,                  // 0x2f730080, 温度1段下偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MIN_MW1,                  // 0x2f730081, 温度2段下偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MIN_MW2,                  // 0x2f730082, 温度3段下偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MIN_MW3,                  // 0x2f730083, 温度4段下偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MIN_MW4,                  // 0x2f730084, 温度5段下偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MIN_MW5,                  // 0x2f730085, 温度6段下偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MIN_MW6,                  // 0x2f730086, 温度7段下偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MIN_MW7,                  // 0x2f730087, 温度8段下偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MIN_MW8,                  // 0x2f730088, 温度9段下偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MIN_MW9,                  // 0x2f730089, 温度10段下偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MIN_MW10,                 // 0x2f73008a, 温度11段下偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MIN_MW11,                 // 0x2f73008b, 温度12段下偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MIN_MW12,                 // 0x2f73008c, 温度13段下偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MIN_MW13,                 // 0x2f73008d, 温度14段下偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MIN_MW14,                 // 0x2f73008e, 温度15段下偏差
  d_TempCtrlMW7_MOLDSET_DATEMP_MIN_MW15,                 // 0x2f73008f, 温度16段下偏差
  d_TempCtrlMW7_MOLDSET_FL_TKTJ_MW0,                     // 0x2f730090, 温度1段热电偶类型
  d_TempCtrlMW7_MOLDSET_FL_TKTJ_MW1,                     // 0x2f730091, 温度2段热电偶类型
  d_TempCtrlMW7_MOLDSET_FL_TKTJ_MW2,                     // 0x2f730092, 温度3段热电偶类型
  d_TempCtrlMW7_MOLDSET_FL_TKTJ_MW3,                     // 0x2f730093, 温度4段热电偶类型
  d_TempCtrlMW7_MOLDSET_FL_TKTJ_MW4,                     // 0x2f730094, 温度5段热电偶类型
  d_TempCtrlMW7_MOLDSET_FL_TKTJ_MW5,                     // 0x2f730095, 温度6段热电偶类型
  d_TempCtrlMW7_MOLDSET_FL_TKTJ_MW6,                     // 0x2f730096, 温度7段热电偶类型
  d_TempCtrlMW7_MOLDSET_FL_TKTJ_MW7,                     // 0x2f730097, 温度8段热电偶类型
  d_TempCtrlMW7_MOLDSET_FL_TKTJ_MW8,                     // 0x2f730098, 温度9段热电偶类型
  d_TempCtrlMW7_MOLDSET_FL_TKTJ_MW9,                     // 0x2f730099, 温度10段热电偶类型
  d_TempCtrlMW7_MOLDSET_FL_TKTJ_MW10,                    // 0x2f73009a, 温度11段热电偶类型
  d_TempCtrlMW7_MOLDSET_FL_TKTJ_MW11,                    // 0x2f73009b, 温度12段热电偶类型
  d_TempCtrlMW7_MOLDSET_FL_TKTJ_MW12,                    // 0x2f73009c, 温度13段热电偶类型
  d_TempCtrlMW7_MOLDSET_FL_TKTJ_MW13,                    // 0x2f73009d, 温度14段热电偶类型
  d_TempCtrlMW7_MOLDSET_FL_TKTJ_MW14,                    // 0x2f73009e, 温度15段热电偶类型
  d_TempCtrlMW7_MOLDSET_FL_TKTJ_MW15,                    // 0x2f73009f, 温度16段热电偶类型
  d_TempCtrlMW7_MOLDSET_DACENT_HEATERON_MW0,             // 0x2f7300a0, 温度1段手动比例
  d_TempCtrlMW7_MOLDSET_DACENT_HEATERON_MW1,             // 0x2f7300a1, 温度2段手动比例
  d_TempCtrlMW7_MOLDSET_DACENT_HEATERON_MW2,             // 0x2f7300a2, 温度3段手动比例
  d_TempCtrlMW7_MOLDSET_DACENT_HEATERON_MW3,             // 0x2f7300a3, 温度4段手动比例
  d_TempCtrlMW7_MOLDSET_DACENT_HEATERON_MW4,             // 0x2f7300a4, 温度5段手动比例
  d_TempCtrlMW7_MOLDSET_DACENT_HEATERON_MW5,             // 0x2f7300a5, 温度6段手动比例
  d_TempCtrlMW7_MOLDSET_DACENT_HEATERON_MW6,             // 0x2f7300a6, 温度7段手动比例
  d_TempCtrlMW7_MOLDSET_DACENT_HEATERON_MW7,             // 0x2f7300a7, 温度8段手动比例
  d_TempCtrlMW7_MOLDSET_DACENT_HEATERON_MW8,             // 0x2f7300a8, 温度9段手动比例
  d_TempCtrlMW7_MOLDSET_DACENT_HEATERON_MW9,             // 0x2f7300a9, 温度10段手动比例
  d_TempCtrlMW7_MOLDSET_DACENT_HEATERON_MW10,            // 0x2f7300aa, 温度11段手动比例
  d_TempCtrlMW7_MOLDSET_DACENT_HEATERON_MW11,            // 0x2f7300ab, 温度12段手动比例
  d_TempCtrlMW7_MOLDSET_DACENT_HEATERON_MW12,            // 0x2f7300ac, 温度13段手动比例
  d_TempCtrlMW7_MOLDSET_DACENT_HEATERON_MW13,            // 0x2f7300ad, 温度14段手动比例
  d_TempCtrlMW7_MOLDSET_DACENT_HEATERON_MW14,            // 0x2f7300ae, 温度15段手动比例
  d_TempCtrlMW7_MOLDSET_DACENT_HEATERON_MW15,            // 0x2f7300af, 温度16段手动比例
  d_TempCtrlMW7_MOLDSET_FL_TEMPHOLD_MW,                  // 0x2f7300b0, 保温功能
  d_TempCtrlMW7_MOLDSET_DATEMP_HOLD_MW,                  // 0x2f7300b1, 保温温度
  d_TempCtrlMW7_MOLDSET_DATEMP_SCTM_MIN_MW0,             // 0x2f7300b2, 最小扫描周期
  d_TempCtrlMW7_MOLDSET_FL_TEMPUPCHECK_MW,               // 0x2f7300b3, 模温连续加温检查
  d_TempCtrlMW7_MOLDSET_DATEMP_NUM_MW,                   // 0x2f7300b4, 温度段数
  d_TempCtrlMW7_MOLDSET_TM_HEATERWARM_MW,                // 0x2f7300b5, 预温计时
  d_TempCtrlMW7_MOLDSET_FL_USE_MW,                       // 0x2f7300b6, 模温功能
  d_TempCtrlMW7_MOLDSET_FL_TEMP_YH,                      // 0x2f7300b7, 温度优化命令
  d_TempCtrlMW7_MOLDSET_FL_TEMP_YHQZ,                    // 0x2f7300b8, 温度强制优化命令
  d_TempCtrlMW7_MOLDSET_CONFUNC_MW,                      // 0x2f7300b9, 同步升温功能
  d_TempCtrlMW7_MOLDSET_FL_INJ_ALLOW                     // 0x2f7300ba, 射出检测模温温度
};

enum db_TempCtrlMW7_STATE{
  d_TempCtrlMW7_STATE_FL_ERROR0 = 0x2f760000, // 0x2f760000, 警报0
  d_TempCtrlMW7_STATE_FL_ERROR1,              // 0x2f760001, 警报1
  d_TempCtrlMW7_STATE_DATEMP_REAL_MW0,        // 0x2f760002, 温度1段实际值
  d_TempCtrlMW7_STATE_DATEMP_REAL_MW1,        // 0x2f760003, 温度2段实际值
  d_TempCtrlMW7_STATE_DATEMP_REAL_MW2,        // 0x2f760004, 温度3段实际值
  d_TempCtrlMW7_STATE_DATEMP_REAL_MW3,        // 0x2f760005, 温度4段实际值
  d_TempCtrlMW7_STATE_DATEMP_REAL_MW4,        // 0x2f760006, 温度5段实际值
  d_TempCtrlMW7_STATE_DATEMP_REAL_MW5,        // 0x2f760007, 温度6段实际值
  d_TempCtrlMW7_STATE_DATEMP_REAL_MW6,        // 0x2f760008, 温度7段实际值
  d_TempCtrlMW7_STATE_DATEMP_REAL_MW7,        // 0x2f760009, 温度8段实际值
  d_TempCtrlMW7_STATE_DATEMP_REAL_MW8,        // 0x2f76000a, 温度9段实际值
  d_TempCtrlMW7_STATE_DATEMP_REAL_MW9,        // 0x2f76000b, 温度10段实际值
  d_TempCtrlMW7_STATE_DATEMP_REAL_MW10,       // 0x2f76000c, 温度11段实际值
  d_TempCtrlMW7_STATE_DATEMP_REAL_MW11,       // 0x2f76000d, 温度12段实际值
  d_TempCtrlMW7_STATE_DATEMP_REAL_MW12,       // 0x2f76000e, 温度13段实际值
  d_TempCtrlMW7_STATE_DATEMP_REAL_MW13,       // 0x2f76000f, 温度14段实际值
  d_TempCtrlMW7_STATE_DATEMP_REAL_MW14,       // 0x2f760010, 温度15段实际值
  d_TempCtrlMW7_STATE_DATEMP_REAL_MW15,       // 0x2f760011, 温度16段实际值
  d_TempCtrlMW7_STATE_FL_STATUS_MW0,          // 0x2f760012, 温度1段状态
  d_TempCtrlMW7_STATE_FL_STATUS_MW1,          // 0x2f760013, 温度2段状态
  d_TempCtrlMW7_STATE_FL_STATUS_MW2,          // 0x2f760014, 温度3段状态
  d_TempCtrlMW7_STATE_FL_STATUS_MW3,          // 0x2f760015, 温度4段状态
  d_TempCtrlMW7_STATE_FL_STATUS_MW4,          // 0x2f760016, 温度5段状态
  d_TempCtrlMW7_STATE_FL_STATUS_MW5,          // 0x2f760017, 温度6段状态
  d_TempCtrlMW7_STATE_FL_STATUS_MW6,          // 0x2f760018, 温度7段状态
  d_TempCtrlMW7_STATE_FL_STATUS_MW7,          // 0x2f760019, 温度8段状态
  d_TempCtrlMW7_STATE_FL_STATUS_MW8,          // 0x2f76001a, 温度9段状态
  d_TempCtrlMW7_STATE_FL_STATUS_MW9,          // 0x2f76001b, 温度10段状态
  d_TempCtrlMW7_STATE_FL_STATUS_MW10,         // 0x2f76001c, 温度11段状态
  d_TempCtrlMW7_STATE_FL_STATUS_MW11,         // 0x2f76001d, 温度12段状态
  d_TempCtrlMW7_STATE_FL_STATUS_MW12,         // 0x2f76001e, 温度13段状态
  d_TempCtrlMW7_STATE_FL_STATUS_MW13,         // 0x2f76001f, 温度14段状态
  d_TempCtrlMW7_STATE_FL_STATUS_MW14,         // 0x2f760020, 温度15段状态
  d_TempCtrlMW7_STATE_FL_STATUS_MW15,         // 0x2f760021, 温度16段状态
  d_TempCtrlMW7_STATE_OUTPUT_PER_MW0,         // 0x2f760022, 温度1段输出实际值百分比
  d_TempCtrlMW7_STATE_OUTPUT_PER_MW1,         // 0x2f760023, 温度2段输出实际值百分比
  d_TempCtrlMW7_STATE_OUTPUT_PER_MW2,         // 0x2f760024, 温度3段输出实际值百分比
  d_TempCtrlMW7_STATE_OUTPUT_PER_MW3,         // 0x2f760025, 温度4段输出实际值百分比
  d_TempCtrlMW7_STATE_OUTPUT_PER_MW4,         // 0x2f760026, 温度5段输出实际值百分比
  d_TempCtrlMW7_STATE_OUTPUT_PER_MW5,         // 0x2f760027, 温度6段输出实际值百分比
  d_TempCtrlMW7_STATE_OUTPUT_PER_MW6,         // 0x2f760028, 温度7段输出实际值百分比
  d_TempCtrlMW7_STATE_OUTPUT_PER_MW7,         // 0x2f760029, 温度8段输出实际值百分比
  d_TempCtrlMW7_STATE_OUTPUT_PER_MW8,         // 0x2f76002a, 温度9段输出实际值百分比
  d_TempCtrlMW7_STATE_OUTPUT_PER_MW9,         // 0x2f76002b, 温度10段输出实际值百分比
  d_TempCtrlMW7_STATE_OUTPUT_PER_MW10,        // 0x2f76002c, 温度11段输出实际值百分比
  d_TempCtrlMW7_STATE_OUTPUT_PER_MW11,        // 0x2f76002d, 温度12段输出实际值百分比
  d_TempCtrlMW7_STATE_OUTPUT_PER_MW12,        // 0x2f76002e, 温度13段输出实际值百分比
  d_TempCtrlMW7_STATE_OUTPUT_PER_MW13,        // 0x2f76002f, 温度14段输出实际值百分比
  d_TempCtrlMW7_STATE_OUTPUT_PER_MW14,        // 0x2f760030, 温度15段输出实际值百分比
  d_TempCtrlMW7_STATE_OUTPUT_PER_MW15,        // 0x2f760031, 温度16段输出实际值百分比
  d_TempCtrlMW7_STATE_WTEMP_LM73_MW,          // 0x2f760032, 室温的温度
  d_TempCtrlMW7_STATE_FL_HEATERSTATUS_MW,     // 0x2f760033, 电热状态
  d_TempCtrlMW7_STATE_DATEMP_VER_MW,          // 0x2f760034, 温度扩展板E501版本号
  d_TempCtrlMW7_STATE_DATEMP_ERR_MW,          // 0x2f760035, 模温扩展板错误状态
  d_TempCtrlMW7_STATE_WRITE_CNT_E501,         // 0x2f760036, E501通讯测定写入次数
  d_TempCtrlMW7_STATE_READ_CNT_E501,          // 0x2f760037, E501通讯测试读入次数
  d_TempCtrlMW7_STATE_DATEMP_YOUHUAXUQIU,     // 0x2f760038, 温度优化提示开电热的闪烁指示
  d_TempCtrlMW7_STATE_DATEMP_YOUHUAOK,        // 0x2f760039, 温度优化开启与完成标记
  d_TempCtrlMW7_STATE_WSTATERSV6              // 0x2f76003a, 预温倒计时
};

enum db_lub1_PO{
  d_lub1_PO_O_LUBRCTR = 0x30100000, // 0x30100000, 润滑1
  d_lub1_PO_O_LUBSRV                // 0x30100001, 润滑1备用
};

enum db_lub1_PI{
  d_lub1_PI_I_LUB_LVL = 0x30110000, // 0x30110000, 润滑1油位检知
  d_lub1_PI_I_LUB_CHK,              // 0x30110001, 润滑1检知
  d_lub1_PI_I_LUB_SRV               // 0x30110002, 润滑1备用
};

enum db_lub1_MACHSET{
  d_lub1_MACHSET_FL_LUBMODE = 0x30120000, // 0x30120000, 润滑1模式
  d_lub1_MACHSET_TM_LUBDLY,               // 0x30120001, 自动润滑1延迟
  d_lub1_MACHSET_TM_LUBTOTAL,             // 0x30120002, 自动润滑1总计时
  d_lub1_MACHSET_TM_LUBACT,               // 0x30120003, 自动润滑1计时
  d_lub1_MACHSET_CN_LUBUNIT,              // 0x30120004, 自动润滑1模数
  d_lub1_MACHSET_TM_LUBFIRST,             // 0x30120005, 首次润滑1计时
  d_lub1_MACHSET_TM_SWITCHCHECK,          // 0x30120006, 开关检测时间
  d_lub1_MACHSET_CN_LUB3,                 // 0x30120007, 润滑1次数
  d_lub1_MACHSET_TM_LUB3,                 // 0x30120008, 润滑1时间
  d_lub1_MACHSET_TM_LUBDELAY3,            // 0x30120009, 润滑1间歇
  d_lub1_MACHSET_TM_LUBFORCE,             // 0x3012000a, 润滑1延迟断开时间
  d_lub1_MACHSET_FL_LUBERR_AGAIN,         // 0x3012000b, 润滑异常立即重新润滑
  d_lub1_MACHSET_FL_LUBERR_AUTOMODE,      // 0x3012000c, 润滑异常后自动前润滑模式 
  d_lub1_MACHSET_FL_LUBERR_AUTOKEY        // 0x3012000d, 自动使用润滑按键
};

enum db_lub1_STATE{
  d_lub1_STATE_FL_ERROR0 = 0x30160000, // 0x30160000, 警报1
  d_lub1_STATE_FL_ERROR1,              // 0x30160001, 警报2
  d_lub1_STATE_FL_MOLDLUBRIC,          // 0x30160002, 润滑状态
  d_lub1_STATE_AUTOLUB_NEED,           // 0x30160003, 本循环是否需要启动润滑
  d_lub1_STATE_Step_Lub,               // 0x30160004, 
  d_lub1_STATE_LUB_CLMP_LEFT,          // 0x30160005, 开润滑剩余模数
  d_lub1_STATE_CN_CLAMPLUBRIC          // 0x30160006, 自动润滑模数
};

enum db_lub2_PO{
  d_lub2_PO_O_LUBRCTR = 0x30200000, // 0x30200000, 润滑2
  d_lub2_PO_O_LUBSRV                // 0x30200001, 润滑2备用
};

enum db_lub2_PI{
  d_lub2_PI_I_LUB_LVL = 0x30210000, // 0x30210000, 润滑2油位检知
  d_lub2_PI_I_LUB_CHK,              // 0x30210001, 润滑2检知
  d_lub2_PI_I_LUB_SRV               // 0x30210002, 润滑2备用
};

enum db_lub2_MACHSET{
  d_lub2_MACHSET_FL_LUBMODE = 0x30220000, // 0x30220000, 润滑2模式
  d_lub2_MACHSET_TM_LUBDLY,               // 0x30220001, 自动润滑2延迟
  d_lub2_MACHSET_TM_LUBTOTAL,             // 0x30220002, 自动润滑2总计时
  d_lub2_MACHSET_TM_LUBACT,               // 0x30220003, 自动润滑2计时
  d_lub2_MACHSET_CN_LUBUNIT,              // 0x30220004, 自动润滑2模数
  d_lub2_MACHSET_TM_LUBFIRST,             // 0x30220005, 首次润滑2计时
  d_lub2_MACHSET_TM_SWITCHCHECK,          // 0x30220006, 开关检测时间2
  d_lub2_MACHSET_CN_LUB3,                 // 0x30220007, 润滑2次数
  d_lub2_MACHSET_TM_LUB3,                 // 0x30220008, 润滑2时间
  d_lub2_MACHSET_TM_LUBDELAY3,            // 0x30220009, 润滑2间歇
  d_lub2_MACHSET_TM_LUBFORCE,             // 0x3022000a, 润滑2延迟断开时间
  d_lub2_MACHSET_FL_LUBERR_AGAIN,         // 0x3022000b, 润滑异常立即重新润滑
  d_lub2_MACHSET_FL_LUBERR_AUTOMODE,      // 0x3022000c, 润滑异常后自动前润滑模式 
  d_lub2_MACHSET_FL_LUBERR_AUTOKEY        // 0x3022000d, 自动使用润滑按键
};

enum db_lub2_STATE{
  d_lub2_STATE_FL_ERROR0 = 0x30260000, // 0x30260000, 警报1
  d_lub2_STATE_FL_ERROR1,              // 0x30260001, 警报2
  d_lub2_STATE_FL_MOLDLUBRIC,          // 0x30260002, 润滑状态
  d_lub2_STATE_AUTOLUB_NEED,           // 0x30260003, 本循环是否需要启动润滑
  d_lub2_STATE_Step_Lub,               // 0x30260004, 
  d_lub2_STATE_LUB_CLMP_LEFT,          // 0x30260005, 开润滑剩余模数
  d_lub2_STATE_CN_CLAMPLUBRIC          // 0x30260006, 自动润滑模数
};

enum db_lub3_PO{
  d_lub3_PO_O_LUBRCTR = 0x30300000, // 0x30300000, 润滑3
  d_lub3_PO_O_LUBSRV                // 0x30300001, 润滑3备用
};

enum db_lub3_PI{
  d_lub3_PI_I_LUB_LVL = 0x30310000, // 0x30310000, 润滑3油位检知
  d_lub3_PI_I_LUB_CHK,              // 0x30310001, 润滑3检知
  d_lub3_PI_I_LUB_SRV               // 0x30310002, 润滑3备用
};

enum db_lub3_MACHSET{
  d_lub3_MACHSET_FL_LUBMODE = 0x30320000, // 0x30320000, 润滑3模式
  d_lub3_MACHSET_TM_LUBDLY,               // 0x30320001, 自动润滑3延迟
  d_lub3_MACHSET_TM_LUBTOTAL,             // 0x30320002, 自动润滑3总计时
  d_lub3_MACHSET_TM_LUBACT,               // 0x30320003, 自动润滑3计时
  d_lub3_MACHSET_CN_LUBUNIT,              // 0x30320004, 自动润滑3模数
  d_lub3_MACHSET_TM_LUBFIRST,             // 0x30320005, 首次润滑3计时
  d_lub3_MACHSET_TM_SWITCHCHECK,          // 0x30320006, 开关检测时间3
  d_lub3_MACHSET_CN_LUB3,                 // 0x30320007, 润滑3次数
  d_lub3_MACHSET_TM_LUB3,                 // 0x30320008, 润滑3时间
  d_lub3_MACHSET_TM_LUBDELAY3,            // 0x30320009, 润滑3间歇
  d_lub3_MACHSET_TM_LUBFORCE,             // 0x3032000a, 润滑3延迟断开时间
  d_lub3_MACHSET_FL_LUBERR_AGAIN,         // 0x3032000b, 润滑异常立即重新润滑
  d_lub3_MACHSET_FL_LUBERR_AUTOMODE,      // 0x3032000c, 润滑异常后自动前润滑模式 
  d_lub3_MACHSET_FL_LUBERR_AUTOKEY        // 0x3032000d, 自动使用润滑按键
};

enum db_lub3_STATE{
  d_lub3_STATE_FL_ERROR0 = 0x30360000, // 0x30360000, 警报1
  d_lub3_STATE_FL_ERROR1,              // 0x30360001, 警报2
  d_lub3_STATE_FL_MOLDLUBRIC,          // 0x30360002, 润滑状态
  d_lub3_STATE_AUTOLUB_NEED,           // 0x30360003, 本循环是否需要启动润滑
  d_lub3_STATE_Step_Lub,               // 0x30360004, 
  d_lub3_STATE_LUB_CLMP_LEFT,          // 0x30360005, 开润滑剩余模数
  d_lub3_STATE_CN_CLAMPLUBRIC          // 0x30360006, 自动润滑模数
};

enum db_robot1_PO{
  d_robot1_PO_O_ROBOT = 0x31100000, // 0x31100000, 机械手
  d_robot1_PO_O_R_EJTF,             // 0x31100001, 机械手顶进
  d_robot1_PO_O_R_EJTB,             // 0x31100002, 机械手顶退
  d_robot1_PO_O_R_SDRC,             // 0x31100003, 机械手安全门关
  d_robot1_PO_O_R_OPNM,             // 0x31100004, 开模结束
  d_robot1_PO_O_R_CLSM,             // 0x31100005, 关模结束
  d_robot1_PO_O_R_INJE,             // 0x31100006, 射出中
  d_robot1_PO_O_R_AUTO,             // 0x31100007, 机械手自动
  d_robot1_PO_O_R_CPRF,             // 0x31100008, 机械手中子进
  d_robot1_PO_O_R_CPRB,             // 0x31100009, 机械手中子退
  d_robot1_PO_O_R_INJERR,           // 0x3110000a, 射出监控失败
  d_robot1_PO_O_R_EMERG,            // 0x3110000b, 机械手急停
  d_robot1_PO_O_R_C2PRF,            // 0x3110000c, 机械手中子2进
  d_robot1_PO_O_R_C2PRB,            // 0x3110000d, 机械手中子2退
  d_robot1_PO_O_R_SDRC2,            // 0x3110000e, 机械手安全门关2
  d_robot1_PO_O_R_EMERG2,           // 0x3110000f, 注塑机急停2
  d_robot1_PO_O_R_SFMA1,            // 0x31100010, 安全装置1
  d_robot1_PO_O_R_SFMA2,            // 0x31100011, 安全装置2
  d_robot1_PO_O_R_OPMD              // 0x31100012, 开模中途
};

enum db_robot1_PI{
  d_robot1_PI_I_ROBOT = 0x31110000, // 0x31110000, 机械手
  d_robot1_PI_I_OPN_ENA,            // 0x31110001, 空气模致能1
  d_robot1_PI_I_INJ_ENA,            // 0x31110002, 空气模致能2
  d_robot1_PI_I_R_EJTF,             // 0x31110003, 顶进安全
  d_robot1_PI_I_R_CLSM,             // 0x31110004, 关模安全
  d_robot1_PI_I_R_OPNM,             // 0x31110005, 开模安全
  d_robot1_PI_I_R_EJTB,             // 0x31110006, 顶退指令
  d_robot1_PI_I_R_RCYC,             // 0x31110007, 循环启动
  d_robot1_PI_I_R_MAFR,             // 0x31110008, 模区安全
  d_robot1_PI_I_R_EMEG,             // 0x31110009, 机械手急停
  d_robot1_PI_I_R_CPRF,             // 0x3111000a, 中子进安全
  d_robot1_PI_I_R_CPRB,             // 0x3111000b, 中子退安全
  d_robot1_PI_I_R_MANUAL,           // 0x3111000c, 机械手手动
  d_robot1_PI_I_R_C2PRF,            // 0x3111000d, 中子2进安全
  d_robot1_PI_I_R_C2PRB,            // 0x3111000e, 中子2退安全
  d_robot1_PI_I_R_EMEG2,            // 0x3111000f, 机械手急停2
  d_robot1_PI_I_R_OPMD              // 0x31110010, 允许完全开模
};

enum db_robot1_MOLDSET{
  d_robot1_MOLDSET_FL_ROBOTCTRL = 0x31130000, // 0x31130000, 机械手功能
  d_robot1_MOLDSET_FL_MANUALOPN,              // 0x31130001, 手动状态使用机械手
  d_robot1_MOLDSET_FL_CORESEC1,               // 0x31130002, 机械手中子信号1连接
  d_robot1_MOLDSET_FL_CORESEC2,               // 0x31130003, 机械手中子信号2连接
  d_robot1_MOLDSET_FL_ROBOTCLPMID,            // 0x31130004, 途中机械手功能
  d_robot1_MOLDSET_TM_ROBOTMID,               // 0x31130005, 途中机械手输出时间
  d_robot1_MOLDSET_FL_ROBOTWAIT,              // 0x31130006, 等待机械手警报
  d_robot1_MOLDSET_FL_MOJUPOTE,               // 0x31130007, 模具保护使用
  d_robot1_MOLDSET_ROBOTACT_POSITION          // 0x31130008, 途中机械手动作位置下限
};

enum db_robot1_STATE{
  d_robot1_STATE_FL_ERROR0 = 0x31160000, // 0x31160000, 警报1
  d_robot1_STATE_FL_ERROR1               // 0x31160001, 警报2
};

enum db_autoctrl1_PI{
  d_autoctrl1_PI_I_PRD_CHK_1 = 0x32110000, // 0x32110000, 检出电眼1
  d_autoctrl1_PI_I_PRD_CHK_2,              // 0x32110001, 检出电眼2
  d_autoctrl1_PI_I_PRD_CHK_3,              // 0x32110002, 检出电眼3
  d_autoctrl1_PI_I_PRD_CHK_4               // 0x32110003, 检出电眼4
};

enum db_autoctrl1_MOLDSET{
  d_autoctrl1_MOLDSET_TM_SENSORDLY = 0x32130000, // 0x32130000, 电眼检测延时
  d_autoctrl1_MOLDSET_TM_SENSORCHK,              // 0x32130001, 电眼检测计时
  d_autoctrl1_MOLDSET_DAPRES_CLPLIMIT,           // 0x32130002, 开关模压力上限
  d_autoctrl1_MOLDSET_DAPRES_ADJUSTLIMIT,        // 0x32130003, 调模压力上限
  d_autoctrl1_MOLDSET_DAPRES_LIMIT,              // 0x32130004, 小动作压力上限
  d_autoctrl1_MOLDSET_DAFLOW_NOZZLELIMIT,        // 0x32130005, 座台速度上限
  d_autoctrl1_MOLDSET_DAFLOW_ADJUSTLIMIT,        // 0x32130006, 调模速度上限
  d_autoctrl1_MOLDSET_DAFLOW_EJECTLIMIT,         // 0x32130007, 托模速度上限
  d_autoctrl1_MOLDSET_DAFLOW_HOLDLIMIT,          // 0x32130008, 保压速度上限
  d_autoctrl1_MOLDSET_DAPRES_BACKLIMIT,          // 0x32130009, 储料背压压力上限
  d_autoctrl1_MOLDSET_DAPRES_CHARGELIMIT,        // 0x3213000a, 储料压力上限
  d_autoctrl1_MOLDSET_DAFLOW_CHARGELIMIT,        // 0x3213000b, 储料速度上限
  d_autoctrl1_MOLDSET_DAHIPRES_CLPLIMIT,         // 0x3213000c, 关模高压压力上限
  d_autoctrl1_MOLDSET_DAHIFLOW_CLPLIMIT,         // 0x3213000d, 关模高压速度上限
  d_autoctrl1_MOLDSET_DATEMP_ADJUST1,            // 0x3213000e, 温度校正系数1
  d_autoctrl1_MOLDSET_DATEMP_ADJUST2,            // 0x3213000f, 
  d_autoctrl1_MOLDSET_DATEMP_ADJUST3,            // 0x32130010, 
  d_autoctrl1_MOLDSET_DATEMP_ADJUST4,            // 0x32130011, 
  d_autoctrl1_MOLDSET_DATEMP_ADJUST5,            // 0x32130012, 
  d_autoctrl1_MOLDSET_DATEMP_ADJUST6,            // 0x32130013, 
  d_autoctrl1_MOLDSET_DATEMP_ADJUST7,            // 0x32130014, 
  d_autoctrl1_MOLDSET_DATEMP_ADJUST8,            // 0x32130015, 
  d_autoctrl1_MOLDSET_DATEMP_ADJUST9,            // 0x32130016, 
  d_autoctrl1_MOLDSET_DATEMP_ADJUST10,           // 0x32130017, 
  d_autoctrl1_MOLDSET_DATEMP_ADJUST11,           // 0x32130018, 
  d_autoctrl1_MOLDSET_DATEMP_ADJUST12,           // 0x32130019, 
  d_autoctrl1_MOLDSET_DATEMP_ADJUST13,           // 0x3213001a, 
  d_autoctrl1_MOLDSET_DATEMP_ADJUST14,           // 0x3213001b, 
  d_autoctrl1_MOLDSET_DATEMP_ADJUST15,           // 0x3213001c, 
  d_autoctrl1_MOLDSET_DATEMP_ADJUST16,           // 0x3213001d, 
  d_autoctrl1_MOLDSET_DATEMP_ADJUSTSET1,         // 0x3213001e, 温度1段设定值
  d_autoctrl1_MOLDSET_DATEMP_ADJUSTSET2,         // 0x3213001f, 温度2段设定值
  d_autoctrl1_MOLDSET_DATEMP_ADJUSTSET3,         // 0x32130020, 温度3段设定值
  d_autoctrl1_MOLDSET_DATEMP_ADJUSTSET4,         // 0x32130021, 温度4段设定值
  d_autoctrl1_MOLDSET_DATEMP_ADJUSTSET5,         // 0x32130022, 温度5段设定值
  d_autoctrl1_MOLDSET_DATEMP_ADJUSTSET6,         // 0x32130023, 温度6段设定值
  d_autoctrl1_MOLDSET_DATEMP_ADJUSTSET7,         // 0x32130024, 温度7段设定值
  d_autoctrl1_MOLDSET_DATEMP_ADJUSTSET8,         // 0x32130025, 温度8段设定值
  d_autoctrl1_MOLDSET_DATEMP_ADJUSTSET9,         // 0x32130026, 温度9段设定值
  d_autoctrl1_MOLDSET_DATEMP_ADJUSTSET10,        // 0x32130027, 温度10段设定值
  d_autoctrl1_MOLDSET_DATEMP_ADJUSTSET11,        // 0x32130028, 温度11段设定值
  d_autoctrl1_MOLDSET_DATEMP_ADJUSTSET12,        // 0x32130029, 温度12段设定值
  d_autoctrl1_MOLDSET_DATEMP_ADJUSTSET13,        // 0x3213002a, 温度13段设定值
  d_autoctrl1_MOLDSET_DATEMP_ADJUSTSET14,        // 0x3213002b, 温度14段设定值
  d_autoctrl1_MOLDSET_DATEMP_ADJUSTSET15,        // 0x3213002c, 温度15段设定值
  d_autoctrl1_MOLDSET_DATEMP_ADJUSTSET16,        // 0x3213002d, 温度16段设定值
  d_autoctrl1_MOLDSET_DALOWPRES_CLPLIMIT,        // 0x3213002e, 关模低压压力上限
  d_autoctrl1_MOLDSET_DAFLOW_CLPLIMIT,           // 0x3213002f, 开关模速度上限
  d_autoctrl1_MOLDSET_DAPRES_UNDERLIMIT,         // 0x32130030, 下压缸压力上限
  d_autoctrl1_MOLDSET_DAFLOW_UNDERLIMIT,         // 0x32130031, 下压缸速度上限
  d_autoctrl1_MOLDSET_DALOWFLOW_CLPLIMIT,        // 0x32130032, 关模低压速度上限
  d_autoctrl1_MOLDSET_DAPRES_EJECTLIMIT,         // 0x32130033, 托模压力上限
  d_autoctrl1_MOLDSET_DAHIFLOW_SUCKBACKLIMIT,    // 0x32130034, 射退速度上限
  d_autoctrl1_MOLDSET_DAPROPORTION,              // 0x32130035, 比重
  d_autoctrl1_MOLDSET_SCREWDIAMETER,             // 0x32130036, 螺杆直径
  d_autoctrl1_MOLDSET_ADPOS_ADJTARG,             // 0x32130037, 模具厚度
  d_autoctrl1_MOLDSET_TM_AGINGMONITOR,           // 0x32130038, 
  d_autoctrl1_MOLDSET_DAPRES_INJECTLIMIT,        // 0x32130039, 射出压力上限
  d_autoctrl1_MOLDSET_DAPRES_SUCKBACKLIMIT,      // 0x3213003a, 射退压力上限
  d_autoctrl1_MOLDSET_DAPRES_CORELIMIT,          // 0x3213003b, 中子压力上限
  d_autoctrl1_MOLDSET_DAPRES_NOZZLELIMIT,        // 0x3213003c, 座台压力上限
  d_autoctrl1_MOLDSET_DAPRES_HOLDLIMIT,          // 0x3213003d, 保压压力上限
  d_autoctrl1_MOLDSET_DAFLOW_INJECTLIMIT,        // 0x3213003e, 射出速度上限
  d_autoctrl1_MOLDSET_DAFLOW_CORELIMIT,          // 0x3213003f, 中子速度上限
  d_autoctrl1_MOLDSET_IN_PRI_CORE_AD,            // 0x32130040, 中子进优先级ABCD
  d_autoctrl1_MOLDSET_IN_PRI_CORE_EH,            // 0x32130041, 中子进优先级EFGH
  d_autoctrl1_MOLDSET_OUT_PRI_CORE_AD,           // 0x32130042, 中子退优先级ABCD
  d_autoctrl1_MOLDSET_OUT_PRI_CORE_EH,           // 0x32130043, 中子退优先级EFGH
  d_autoctrl1_MOLDSET_MOLDTEMPBOARD_COUNT,       // 0x32130044, 模温板数量
  d_autoctrl1_MOLDSET_SYS_PUSHTABLE_USECAL,      // 0x32130045, 计算十字头功能
  d_autoctrl1_MOLDSET_SYS_PRS_CLAMP_CURVE,       // 0x32130046, 曲线页面的系统压力根据选择关联 -开关模
  d_autoctrl1_MOLDSET_SYS_PRS_CHARGE_CURVE,      // 0x32130047, 曲线页面的系统压力根据选择关联 -储料
  d_autoctrl1_MOLDSET_SYS_PRS_INJECT_CURVE,      // 0x32130048, 曲线页面的系统压力根据选择关联 -射出
  d_autoctrl1_MOLDSET_SYS_COREAINPOSMODE,        // 0x32130049, 中子A进位置模式
  d_autoctrl1_MOLDSET_SYS_COREBINPOSMODE,        // 0x3213004a, 中子B进位置模式
  d_autoctrl1_MOLDSET_SYS_CORECINPOSMODE,        // 0x3213004b, 中子C进位置模式
  d_autoctrl1_MOLDSET_SYS_COREDINPOSMODE,        // 0x3213004c, 中子D进位置模式
  d_autoctrl1_MOLDSET_SYS_COREEINPOSMODE,        // 0x3213004d, 中子E进位置模式
  d_autoctrl1_MOLDSET_SYS_COREFINPOSMODE,        // 0x3213004e, 中子F进位置模式
  d_autoctrl1_MOLDSET_SYS_COREAOUTPOSMODE,       // 0x3213004f, 中子A退位置模式
  d_autoctrl1_MOLDSET_SYS_COREBOUTPOSMODE,       // 0x32130050, 中子B退位置模式
  d_autoctrl1_MOLDSET_SYS_CORECOUTPOSMODE,       // 0x32130051, 中子C退位置模式
  d_autoctrl1_MOLDSET_SYS_COREDOUTPOSMODE,       // 0x32130052, 中子D退位置模式
  d_autoctrl1_MOLDSET_SYS_COREEOUTPOSMODE,       // 0x32130053, 中子E退位置模式
  d_autoctrl1_MOLDSET_SYS_COREFOUTPOSMODE,       // 0x32130054, 中子F退位置模式
  d_autoctrl1_MOLDSET_KEYSEL_SPUREVLV,           // 0x32130055, 胶口针阀按钮切换
  d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_HIGH,   // 0x32130056, 射出高流动性KP
  d_autoctrl1_MOLDSET_SYS_SERVO_KI_INJCT_HIGH,   // 0x32130057, 射出高流动性KI
  d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MID,    // 0x32130058, 射出中流动性KP
  d_autoctrl1_MOLDSET_SYS_SERVO_KI_INJCT_MID,    // 0x32130059, 射出中流动性KI
  d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_LOW,    // 0x3213005a, 射出低流动性KP
  d_autoctrl1_MOLDSET_SYS_SERVO_KI_INJCT_LOW,    // 0x3213005b, 射出低流动性KI
  d_autoctrl1_MOLDSET_SYS_SERVO_KP_HOLD_HIGH,    // 0x3213005c, 保压高流动性KP
  d_autoctrl1_MOLDSET_SYS_SERVO_KI_HOLD_HIGH,    // 0x3213005d, 保压高流动性KI
  d_autoctrl1_MOLDSET_SYS_SERVO_KP_HOLD_MID,     // 0x3213005e, 保压中流动性KP
  d_autoctrl1_MOLDSET_SYS_SERVO_KI_HOLD_MID,     // 0x3213005f, 保压中流动性KI
  d_autoctrl1_MOLDSET_SYS_SERVO_KP_HOLD_LOW,     // 0x32130060, 保压低流动性KP
  d_autoctrl1_MOLDSET_SYS_SERVO_KI_HOLD_LOW,     // 0x32130061, 保压低流动性KI
  d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE,   // 0x32130062, 原料流动性模式
  d_autoctrl1_MOLDSET_SYS_SERVO_KP_HOLD_MODE,    // 0x32130063, 保压流动性模式
  d_autoctrl1_MOLDSET_KEY_LUBR2_INDEPEND,        // 0x32130064, 润滑二按键独立
  d_autoctrl1_MOLDSET_ECAT_CURVE_SELECT1,        // 0x32130065, 曲线显示勾选1
  d_autoctrl1_MOLDSET_ECAT_CURVE_SELECT2,        // 0x32130066, 曲线显示勾选2
  d_autoctrl1_MOLDSET_ECAT_CURVE_SELECT3,        // 0x32130067, 曲线显示勾选3
  d_autoctrl1_MOLDSET_ECAT_CURVE_SELECT4,        // 0x32130068, 曲线显示勾选4
  d_autoctrl1_MOLDSET_ECAT_CURVE_MAXSET1,        // 0x32130069, 曲线1最大值
  d_autoctrl1_MOLDSET_ECAT_CURVE_MAXSET2,        // 0x3213006a, 曲线2最大值
  d_autoctrl1_MOLDSET_ECAT_CURVE_MAXSET3,        // 0x3213006b, 曲线3最大值
  d_autoctrl1_MOLDSET_ECAT_CURVE_MAXSET4,        // 0x3213006c, 曲线4最大值
  d_autoctrl1_MOLDSET_ECAT_CURVE_MAXFLAG1,       // 0x3213006d, 曲线1最大值正负标志
  d_autoctrl1_MOLDSET_ECAT_CURVE_MAXFLAG2,       // 0x3213006e, 曲线2最大值正负标志
  d_autoctrl1_MOLDSET_ECAT_CURVE_MAXFLAG3,       // 0x3213006f, 曲线3最大值正负标志
  d_autoctrl1_MOLDSET_ECAT_CURVE_MAXFLAG4,       // 0x32130070, 曲线4最大值正负标志
  d_autoctrl1_MOLDSET_ECAT_CURVE_MINSET1,        // 0x32130071, 曲线1最小值
  d_autoctrl1_MOLDSET_ECAT_CURVE_MINSET2,        // 0x32130072, 曲线2最小值
  d_autoctrl1_MOLDSET_ECAT_CURVE_MINSET3,        // 0x32130073, 曲线3最小值
  d_autoctrl1_MOLDSET_ECAT_CURVE_MINSET4,        // 0x32130074, 曲线4最小值
  d_autoctrl1_MOLDSET_ECAT_CURVE_MINFLAG1,       // 0x32130075, 曲线1最小值正负标志
  d_autoctrl1_MOLDSET_ECAT_CURVE_MINFLAG2,       // 0x32130076, 曲线2最小值正负标志
  d_autoctrl1_MOLDSET_ECAT_CURVE_MINFLAG3,       // 0x32130077, 曲线3最小值正负标志
  d_autoctrl1_MOLDSET_ECAT_CURVE_MINFLAG4,       // 0x32130078, 曲线4最小值正负标志
  d_autoctrl1_MOLDSET_ECAT_CURVE_KEEPTM,         // 0x32130079, 曲线持续时间
  d_autoctrl1_MOLDSET_ECAT_CURVE_PERIOD,         // 0x3213007a, 曲线采样精度
  d_autoctrl1_MOLDSET_ECAT_CURVE_SERVOID,        // 0x3213007b, 驱动器编号
  d_autoctrl1_MOLDSET_ECAT_CURVE_COLLECTMOD,     // 0x3213007c, 曲线采集方式
  d_autoctrl1_MOLDSET_ECAT_CURVE_START,          // 0x3213007d, 曲线采集启停
  d_autoctrl1_MOLDSET_TM_PREHEAT_LIMIT,          // 0x3213007e, 预温计时下限
  d_autoctrl1_MOLDSET_FL_EJTCTRL,                // 0x3213007f, 托模方式
  d_autoctrl1_MOLDSET_SYS_COREPOSEFF,            // 0x32130080, 中子有效区位置
  d_autoctrl1_MOLDSET_DAHIFLOW_SUCKBACKLIMIT_NEW // 0x32130081, 射退速度上限
};

enum db_autoctrl1_STATE{
  d_autoctrl1_STATE_FL_ERROR0 = 0x32160000, // 0x32160000, 
  d_autoctrl1_STATE_FL_ERROR1,              // 0x32160001, 
  d_autoctrl1_STATE_FL_AUTOCTRL_STATE_IOT_ACT1, // 0x32160002, 动作状态1
  d_autoctrl1_STATE_FL_AUTOCTRL_STATE_IOT_ACT2, // 0x32160003, 动作状态2
  d_autoctrl1_STATE_FL_AUTOCTRL_STATE_IOT_ACT3, // 0x32160004, 动作状态3
  d_autoctrl1_STATE_FL_AUTOCTRL_STATE_IOT_WARN1_H, // 0x32160005, 警报状态1_H
  d_autoctrl1_STATE_FL_AUTOCTRL_STATE_IOT_WARN1_L, // 0x32160006, 警报状态1_L
  d_autoctrl1_STATE_FL_AUTOCTRL_STATE_IOT_WARN2_H, // 0x32160007, 警报状态2_H
  d_autoctrl1_STATE_FL_AUTOCTRL_STATE_IOT_WARN2_L, // 0x32160008, 警报状态2_L
  d_autoctrl1_STATE_FL_AUTOCTRL_STATE_IOT_WARN3_H, // 0x32160009, 警报状态3_H
  d_autoctrl1_STATE_FL_AUTOCTRL_STATE_IOT_WARN3_L, // 0x3216000a, 警报状态3_L
  d_autoctrl1_STATE_FL_AUTOCTRL_STATE_IOT_TOTALCNT_H, // 0x3216000b, 累计模数_H
  d_autoctrl1_STATE_FL_AUTOCTRL_STATE_IOT_TOTALCNT_L, // 0x3216000c, 累计模数_L
  d_autoctrl1_STATE_FL_AUTOCTRL_STATE_IOT_TOTAL_TIME_H, // 0x3216000d, 历史累计时间
  d_autoctrl1_STATE_FL_AUTOCTRL_STATE_IOT_TOTAL_TIME_L, // 0x3216000e, 历史累计时间
  d_autoctrl1_STATE_FL_AUTOCTRL_STATE_IOT_START_TIME_H, // 0x3216000f, 本次开机时间
  d_autoctrl1_STATE_FL_AUTOCTRL_STATE_IOT_START_TIME_L // 0x32160010, 本次开机时间
};

enum db_PP_MACHSET{
  p_PP_MACHSET_JKEY_COLOR1 = 0x33120000, // 0x33120000, 
  p_PP_MACHSET_JKEY_COLOR2,              // 0x33120001, 
  p_PP_MACHSET_JKEY_COLOR3,              // 0x33120002, 
  p_PP_MACHSET_JKEY_COLOR4,              // 0x33120003, 
  p_PP_MACHSET_JKEY_COLOR5,              // 0x33120004, 
  p_PP_MACHSET_JKEY_COLOR6,              // 0x33120005, 
  p_PP_MACHSET_JKEY_COLOR7,              // 0x33120006, 
  p_PP_MACHSET_JKEY_COLOR8,              // 0x33120007, 
  p_PP_MACHSET_JKEY_COLOR9,              // 0x33120008, 
  p_PP_MACHSET_JKEY_COLOR10,             // 0x33120009, 
  p_PP_MACHSET_JKEY_STR1,                // 0x3312000a, 
  p_PP_MACHSET_JKEY_STR2,                // 0x3312000b, 
  p_PP_MACHSET_JKEY_STR3,                // 0x3312000c, 
  p_PP_MACHSET_JKEY_STR4,                // 0x3312000d, 
  p_PP_MACHSET_JKEY_STR5,                // 0x3312000e, 
  p_PP_MACHSET_JKEY_STR6,                // 0x3312000f, 
  p_PP_MACHSET_JKEY_STR7,                // 0x33120010, 
  p_PP_MACHSET_JKEY_STR8,                // 0x33120011, 
  p_PP_MACHSET_JKEY_STR9,                // 0x33120012, 
  p_PP_MACHSET_JKEY_STR10,               // 0x33120013, 
  p_PP_MACHSET_TEMP_LEFTMIN,             // 0x33120014, 预温剩余分钟
  p_PP_MACHSET_TEMP_LEFTSEC,             // 0x33120015, 预温剩余秒钟
  p_PP_MACHSET_CORE_SELECT,              // 0x33120016, 中子参数选择
  p_PP_MACHSET_PROD_PERCNT,              // 0x33120017, 每模产品数
  p_PP_MACHSET_PROD_SHOTCNT,             // 0x33120018, 开模数记录
  p_PP_MACHSET_PROD_TARGETCNT,           // 0x33120019, 目标数
  p_PP_MACHSET_PROD_PKGCNT,              // 0x3312001a, 本包开模数
  p_PP_MACHSET_PROD_TRANSTIME,           // 0x3312001b, 包装传输时间
  p_PP_MACHSET_PROD_TRANSDELAY,          // 0x3312001c, 包装延迟时间
  p_PP_MACHSET_RECORD_SAVE,              // 0x3312001d, 操作记录保存
  p_PP_MACHSET_INJECT_CUREMODE,          // 0x3312001e, 射出曲线模式选择
  p_PP_MACHSET_LOGIN_PWD,                // 0x3312001f, 登录框密码
  p_PP_MACHSET_SYSPAR_AIR7GROUP,         // 0x33120020, 吹气7组功能选择
  p_PP_MACHSET_SYSPAR_MDTEMP,            // 0x33120021, 模温功能选择
  p_PP_MACHSET_LANG_SELECT,              // 0x33120022, 面板语言选择保存
  p_PP_MACHSET_ALARM_STATE,              // 0x33120023, 自动警报状态
  p_PP_MACHSET_PROD_GOODCNT,             // 0x33120024, 良品数
  p_PP_MACHSET_PROD_CLAMPLUBRIC,         // 0x33120025, 自动润滑模数
  p_PP_MACHSET_MDTEMP_STATE1,            // 0x33120026, 模温加热状态1
  p_PP_MACHSET_MDTEMP_STATE2,            // 0x33120027, 模温加热状态2
  p_PP_MACHSET_MDTEMP_STATE3,            // 0x33120028, 模温加热状态3
  p_PP_MACHSET_MDTEMP_STATE4,            // 0x33120029, 模温加热状态4
  p_PP_MACHSET_MDTEMP_STATE5,            // 0x3312002a, 模温加热状态5
  p_PP_MACHSET_MDTEMP_STATE6,            // 0x3312002b, 模温加热状态6
  p_PP_MACHSET_MDTEMP_STATE7,            // 0x3312002c, 模温加热状态7
  p_PP_MACHSET_MDTEMP_VERSION1,          // 0x3312002d, 模温版本号1
  p_PP_MACHSET_MDTEMP_VERSION2,          // 0x3312002e, 模温版本号2
  p_PP_MACHSET_MDTEMP_VERSION3,          // 0x3312002f, 模温版本号3
  p_PP_MACHSET_MDTEMP_VERSION4,          // 0x33120030, 模温版本号4
  p_PP_MACHSET_MDTEMP_VERSION5,          // 0x33120031, 模温版本号5
  p_PP_MACHSET_MDTEMP_VERSION6,          // 0x33120032, 模温版本号6
  p_PP_MACHSET_MDTEMP_VERSION7,          // 0x33120033, 模温版本号7
  p_PP_MACHSET_MDTEMP_STATEX1,           // 0x33120034, 模温在线状态1
  p_PP_MACHSET_MDTEMP_STATEX2,           // 0x33120035, 模温在线状态2
  p_PP_MACHSET_MDTEMP_STATEX3,           // 0x33120036, 模温在线状态3
  p_PP_MACHSET_MDTEMP_STATEX4,           // 0x33120037, 模温在线状态4
  p_PP_MACHSET_MDTEMP_STATEX5,           // 0x33120038, 模温在线状态5
  p_PP_MACHSET_MDTEMP_STATEX6,           // 0x33120039, 模温在线状态6
  p_PP_MACHSET_MDTEMP_STATEX7,           // 0x3312003a, 模温在线状态7
  p_PP_MACHSET_SETUP_SERVO_EXPORT,       // 0x3312003b, 伺服数据导出
  p_PP_MACHSET_SETUP_SERVO_IMPORT,       // 0x3312003c, 伺服数据导入
  p_PP_MACHSET_SETUP_SERVO_SAVEDEF,      // 0x3312003d, 保存位出厂设置标志
  p_PP_MACHSET_SETUP_SERVO_READDEF,      // 0x3312003e, 读取位出厂设置标志
  p_PP_MACHSET_SETUP_SERVO_MSG,          // 0x3312003f, 伺服地址信息
  p_PP_MACHSET_SETUP_SERVO_MSGDEF,       // 0x33120040, 伺服地址默认信息
  p_PP_MACHSET_UPDATE_PANELCODE,         // 0x33120041, 面板更新代码记录
  p_PP_MACHSET_UPDATE_CONFIGCODE,        // 0x33120042, 配置代码
  p_PP_MACHSET_UPDATE_MANUFCODE,         // 0x33120043, 厂商代码
  p_PP_MACHSET_UPDATE_PROGRAMCODE,       // 0x33120044, 程序代码
  p_PP_MACHSET_INFORMATION_INDEX,        // 0x33120045, 消息框表索引值
  p_PP_MACHSET_PASSWORD_PWD,             // 0x33120046, 密码框密码
  p_PP_MACHSET_PASSWORD_USER,            // 0x33120047, 密码等级用户
  p_PP_MACHSET_SAFETY_DATAPROTECT,       // 0x33120048, 数据保护
  p_PP_MACHSET_SAFETY_VIEWPROTECT,       // 0x33120049, 画面显示保护
  p_PP_MACHSET_SAFETY_PARAMHIDE,         // 0x3312004a, 参数画面隐藏
  p_PP_MACHSET_SAFETY_PWDPROMPT,         // 0x3312004b, 密码提示框功能
  p_PP_MACHSET_MACHTAB_SYSTEMNAMEINDEX,  // 0x3312004c, 机器名称对应索引
  p_PP_MACHSET_MACHTAB_SYSTEMNAME,       // 0x3312004d, 机器类型名称
  p_PP_MACHSET_DAADJUST_MAXSPEED,        // 0x3312004e, DA最大速度
  p_PP_MACHSET_DAADJUST_DACHANEL,        // 0x3312004f, DA通道号
  p_PP_MACHSET_DAADJUST_ADJMOD,          // 0x33120050, DA调整模式：调整or调整中
  p_PP_MACHSET_MAINFRAME_TIME,           // 0x33120051, 系统时钟
  p_PP_MACHSET_EJECTFUNC_SENSOR,         // 0x33120052, 电眼使用
  p_PP_MACHSET_SYSCFG_ACTIVETIME,        // 0x33120053, 屏保时间
  p_PP_MACHSET_SYSCFG_DISMODE,           // 0x33120054, 显示方式
  p_PP_MACHSET_SYSCFG_BACKGROUND,        // 0x33120055, 背景选择
  p_PP_MACHSET_MAINFRAME_ALARM,          // 0x33120056, 警报显示
  p_PP_MACHSET_MAINFRAME_ACTSETP,        // 0x33120057, 动作步显示
  p_PP_MACHSET_MAINFRAME_PAGETITLE,      // 0x33120058, 页面标题名称
  p_PP_MACHSET_DATA_SOFTWARE_LOCK,       // 0x33120059, 资料锁软件锁变量
  p_PP_MACHSET_USB_VISIBLE,              // 0x3312005a, USB是否使用
  p_PP_MACHSET_CORE_SWITCH,              // 0x3312005b, 中子切换显示
  p_PP_MACHSET_BLOW_SWITCH,              // 0x3312005c, 吹气切换显示
  p_PP_MACHSET_CORE_SWITCHBK,            // 0x3312005d, 中子切换背景色
  p_PP_MACHSET_CORE_SWITCHTEXT,          // 0x3312005e, 中子切换文本色
  p_PP_MACHSET_BLOW_SWITCHBK,            // 0x3312005f, 吹气切换背景色
  p_PP_MACHSET_BLOW_SWITCHTEXT,          // 0x33120060, 吹气切换文本色
  p_PP_MACHSET_AD_CLAMP_ABSOUTE,         // 0x33120061, 十字头位置显示值
  p_PP_MACHSET_PANEL_TYPE_FLAG,          // 0x33120062, 面板类型标记
  p_PP_MACHSET_UDISK_WR_PRIVILEGE,       // 0x33120063, U盘导入导出等级
  p_PP_MACHSET_UDIKS_READ_FLAG,          // 0x33120064, U盘读成功标记
  p_PP_MACHSET_DAADJ_CHANGE_FLAG,        // 0x33120065, DA调整数据改变标记
  p_PP_MACHSET_MACHCFG_CHANGE_FLAG,      // 0x33120066, 机器配置数据改变标记
  p_PP_MACHSET_MACHCFGX_CHANGE_FLAG,     // 0x33120067, 机型选择数据改变标记
  p_PP_MACHSET_SYSPAR_BLOW14_FUNC,       // 0x33120068, 是否使用7组吹气功能
  p_PP_MACHSET_DATA_PROT_PASW,           // 0x33120069, 数据保护页面密码
  p_PP_MACHSET_FLCOOLTIMEPROT,           // 0x3312006a, 模具冷却时间保护
  p_PP_MACHSET_FLCLAMPSPDPROT,           // 0x3312006b, 开关模速度上限保护
  p_PP_MACHSET_FLHIPRSPRSPROT,           // 0x3312006c, 关模高压压力上限保护
  p_PP_MACHSET_FLHIPRSSPDPROT,           // 0x3312006d, 关模高压速度上限保护
  p_PP_MACHSET_SWPUSHPOSOPT,             // 0x3312006e, 推力座是否显示
  p_PP_MACHSET_AIRBLOW1_14AGAIN,         // 0x3312006f, 吹气1-14是否多次
  p_PP_MACHSET_SCREEN_SHOT,              // 0x33120070, 天剑风格截屏功能
  p_PP_MACHSET_TEMP_CURVE,               // 0x33120071, 是否使用温度曲线
  p_PP_MACHSET_SWCYCLEMOLDCNT,           // 0x33120072, 周期计数模数
  p_PP_MACHSET_SWCYCLEMOLDCURRENTCNT,    // 0x33120073, 周期计数当前模数
  p_PP_MACHSET_POWER_ON_MOLD,            // 0x33120074, 每次上电开始记录开模数
  p_PP_MACHSET_PUSHPARA_L1,              // 0x33120075, 五支点参数L1
  p_PP_MACHSET_PUSHPARA_L2,              // 0x33120076, 五支点参数L2
  p_PP_MACHSET_PUSHPARA_L3,              // 0x33120077, 五支点参数L3
  p_PP_MACHSET_PUSHPARA_L4,              // 0x33120078, 五支点参数L4
  p_PP_MACHSET_PUSHPARA_L5,              // 0x33120079, 五支点参数L5
  p_PP_MACHSET_PUSHPARA_H,               // 0x3312007a, 五支点参数H
  p_PP_MACHSET_PUSHPARA_h,               // 0x3312007b, 五支点参数h
  p_PP_MACHSET_PUSHPARA_Hm,              // 0x3312007c, 五支点参数Hm
  p_PP_MACHSET_PUSHPARA_LMTAG,           // 0x3312007d, 五支点参数极限夹角
  p_PP_MACHSET_SHOW_LMTAG_ERROR,         // 0x3312007e, 五支点参数极限夹角输入有误
  p_PP_MACHSET_USE_PUSHPARA,             // 0x3312007f, 是否使用五支点
  p_PP_MACHSET_PRS_PROTECT,              // 0x33120080, 温度上限保护
  p_PP_MACHSET_TAB_PAGE_SHOW,            // 0x33120081, 有TAB控件的画面进入标记
  p_PP_MACHSET_ECAT_CHANGE_FLAG,         // 0x33120082, ECAT数据改变标记
  p_PP_MACHSET_FLOPMODE_USEPSW,          // 0x33120083, 操作模式切换使用密码
  p_PP_MACHSET_G_KEYVALUE,               // 0x33120084, 全局按键值
  p_PP_MACHSET_ADEX_CHANGE_FLAG,         // 0x33120085, AD扩展数据改变标记
  p_PP_MACHSET_ADX_CHANGE_FLAG,          // 0x33120086, ADX页面数据改变标记
  p_PP_MACHSET_MACHCFG5200_CHANGE_FLAG,  // 0x33120087, MACHCFGX页面数据改变标记
  p_PP_MACHSET_KEY_LAST_FOCUS,           // 0x33120088, 换页画面聚焦控件索引值
  p_PP_MACHSET_E502_TEMP_AB,             // 0x33120089, E502料管切换
  p_PP_MACHSET_SYSPARA_BACK,             // 0x3312008a, 系统参数返回上一层标记
  p_PP_MACHSET_SELF_TURNING_FLAG,        // 0x3312008b, 自整定完成标记
  p_PP_MACHSET_INJ_CURVE_SELECT,         // 0x3312008c, 射出曲线选择
  p_PP_MACHSET_INJPOS_CURVE_SELECT,      // 0x3312008d, 射出位置曲线选择
  p_PP_MACHSET_HOLD_CURVE_SELECT,        // 0x3312008e, 保压曲线选择
  p_PP_MACHSET_OPN_CURVE_SELECT,         // 0x3312008f, 开模曲线选择
  p_PP_MACHSET_CLS_CURVE_SELECT,         // 0x33120090, 关模曲线选择
  p_PP_MACHSET_CHG_CURVE_SELECT,         // 0x33120091, 储料曲线选择
  p_PP_MACHSET_MT_BOARD_COUNT,           // 0x33120092, 模温板数量
  p_PP_MACHSET_MT_OPT_SELECT,            // 0x33120093, 模温优化选择
  p_PP_MACHSET_PROD_CLAMPLUBRIC2,        // 0x33120094, 自动润滑模数2
  p_PP_MACHSET_PROD_CLAMPLUBRIC3,        // 0x33120095, 自动润滑模数3
  p_PP_MACHSET_BACK_IMG_STR,             // 0x33120096, 背景图片
  p_PP_MACHSET_QUESTION_INDEX,           // 0x33120097, 消息框表索引值
  p_PP_MACHSET_STUDY_STEP,               // 0x33120098, 自学习动作步骤
  p_PP_MACHSET_STUDY_PROCESS,            // 0x33120099, 自学习动作进度条
  p_PP_MACHSET_STUDY_ENERGY,             // 0x3312009a, 自学习动能转换效率
  p_PP_MACHSET_STUDY_DEADTM,             // 0x3312009b, 自学习死区时间
  p_PP_MACHSET_STUDY_RAMP,               // 0x3312009c, 自学习最大斜率
  p_PP_MACHSET_SELF_STUDY_FLAG,          // 0x3312009d, 自学习标志
  p_PP_MACHSET_SELF_STUDY_PARA,          // 0x3312009e, 自学习参数
  p_PP_MACHSET_STUDY_ENERGY2,            // 0x3312009f, 自学习动能转换效率2
  p_PP_MACHSET_STUDY_DEADTM2,            // 0x331200a0, 自学习死区时间2
  p_PP_MACHSET_STUDY_RAMP2,              // 0x331200a1, 自学习最大斜率2
  p_PP_MACHSET_STUDY_ID,                 // 0x331200a2, 自学习索引值
  p_PP_MACHSET_SPECFUNC_SELECT_CHANGE_FLAG, // 0x331200a3, 功能选择页面改变标记
  p_PP_MACHSET_PUSHTAB_MODE,             // 0x331200a4, 十字头模式
  p_PP_MACHSET_PERPRODWEIGHT,            // 0x331200a5, 每模产品重量
  p_PP_MACHSET_SPARE1_KEY,               // 0x331200a6, 备用1按键定义
  p_PP_MACHSET_SPARE2_KEY,               // 0x331200a7, 备用2按键定义
  p_PP_MACHSET_SPARE3_KEY,               // 0x331200a8, 备用3按键定义
  p_PP_MACHSET_SPARE4_KEY,               // 0x331200a9, 备用4按键定义
  p_PP_MACHSET_SPARE5_KEY,               // 0x331200aa, 备用5按键定义
  p_PP_MACHSET_SPARE6_KEY,               // 0x331200ab, 备用6按键定义
  p_PP_MACHSET_SPARE7_KEY,               // 0x331200ac, 备用7按键定义
  p_PP_MACHSET_SPARE8_KEY,               // 0x331200ad, 备用8按键定义
  p_PP_MACHSET_SPARE9_KEY,               // 0x331200ae, 备用9按键定义
  p_PP_MACHSET_SPARE10_KEY,              // 0x331200af, 备用10按键定义
  p_PP_MACHSET_SPARE1_VALUE,             // 0x331200b0, 备用1按键值
  p_PP_MACHSET_SPARE2_VALUE,             // 0x331200b1, 备用2按键值
  p_PP_MACHSET_SPARE3_VALUE,             // 0x331200b2, 备用3按键值
  p_PP_MACHSET_SPARE4_VALUE,             // 0x331200b3, 备用4按键值
  p_PP_MACHSET_SPARE5_VALUE,             // 0x331200b4, 备用5按键值
  p_PP_MACHSET_SPARE6_VALUE,             // 0x331200b5, 备用6按键值
  p_PP_MACHSET_SPARE7_VALUE,             // 0x331200b6, 备用7按键值
  p_PP_MACHSET_SPARE8_VALUE,             // 0x331200b7, 备用8按键值
  p_PP_MACHSET_SPARE9_VALUE,             // 0x331200b8, 备用9按键值
  p_PP_MACHSET_SPARE10_VALUE,            // 0x331200b9, 备用10按键值
  p_PP_MACHSET_wUse15DaysLub1,           // 0x331200ba, 润滑1间歇时间
  p_PP_MACHSET_wUse15DaysLub2,           // 0x331200bb, 润滑2间歇时间
  p_PP_MACHSET_wUse15DaysLub3,           // 0x331200bc, 润滑3间歇时间
  p_PP_MACHSET_CLAMPVIEW_HIGH2_VISIBLE,  // 0x331200bd, 开合模页面高压二段可见
  p_PP_MACHSET_FL_BLAST1,                // 0x331200be, 吹气1方式
  p_PP_MACHSET_FL_BLAST2,                // 0x331200bf, 吹气2方式
  p_PP_MACHSET_FL_BLAST3,                // 0x331200c0, 吹气3方式
  p_PP_MACHSET_FL_BLAST4,                // 0x331200c1, 吹气4方式
  p_PP_MACHSET_FL_BLAST5,                // 0x331200c2, 吹气5方式
  p_PP_MACHSET_FL_BLAST6,                // 0x331200c3, 吹气6方式
  p_PP_MACHSET_FL_BLAST7,                // 0x331200c4, 吹气7方式
  p_PP_MACHSET_FL_BLAST8,                // 0x331200c5, 吹气8方式
  p_PP_MACHSET_FL_BLAST9,                // 0x331200c6, 吹气9方式
  p_PP_MACHSET_FL_BLAST10,               // 0x331200c7, 吹气10方式
  p_PP_MACHSET_FL_BLAST11,               // 0x331200c8, 吹气11方式
  p_PP_MACHSET_FL_BLAST12,               // 0x331200c9, 吹气12方式
  p_PP_MACHSET_FL_BLAST13,               // 0x331200ca, 吹气13方式
  p_PP_MACHSET_FL_BLAST14,               // 0x331200cb, 吹气14方式
  p_PP_MACHSET_wLUBCNTMAX,               // 0x331200cc, 自动润滑模数上限值
  p_PP_MACHSET_wLUBTMMIN,                // 0x331200cd, 自动润滑计时下限值
  p_PP_MACHSET_wLUBDLYMAX,               // 0x331200ce, 自动润滑延迟上限值
  p_PP_MACHSET_wLUBDLYMIN,               // 0x331200cf, 自动润滑总时间下限值
  p_PP_MACHSET_SHOTCNT_TOTAL,            // 0x331200d0, 开模数记录（不清零）
  p_PP_MACHSET_CORE_SELECT_SYSPAR,       // 0x331200d1, 中子参数选择_系统参数画面
  p_PP_MACHSET_COREPROTECT_ASK_ADDR,     // 0x331200d2, 中子保护功能不使用对应地址临时存储，用于弹出框判断
  p_PP_MACHSET_REMELT_FUNC,              // 0x331200d3, 再次储料模式
  p_PP_MACHSET_IAP_BOARD_CODE,           // 0x331200d4, 扩展板代码
  p_PP_MACHSET_IAP_NUMBER,               // 0x331200d5, 扩展板号
  p_PP_MACHSET_IAP_TYPE,                 // 0x331200d6, 扩展板类型
  p_PP_MACHSET_24HOURWRITEFILEMAX,       // 0x331200d7, 24小时写次数上限
  p_PP_MACHSET_ContinueWriteFilw,        // 0x331200d8, 连续多少模写文件
  p_PP_MACHSET_MAINFRAME_ALARM_Self,     // 0x331200d9, 警报显示面板自己使用
  p_PP_MACHSET_SWITICHING_MODE_SMOOTH,   // 0x331200da, 开关模智能辅助功能
  p_PP_MACHSET_wLUBR1_CNT_MIN,           // 0x331200db, 自动润滑一模数下限
  p_PP_MACHSET_wLUBR2_CNT_MIN,           // 0x331200dc, 自动润滑二模数下限
  p_PP_MACHSET_wLUBR3_CNT_MIN,           // 0x331200dd, 自动润滑三模数下限
  p_PP_MACHSET_SYS_CLAMP_PROTECT,        // 0x331200de, 开关模压力上限密码保护
  p_PP_MACHSET_Prod_MONI_SENIOR,         // 0x331200df, 监测页面使用平均值显示
  p_PP_MACHSET_DAFLOW_OPENSPD1_LIMIT,    // 0x331200e0, 快速开模启动速度上限
  p_PP_MACHSET_HIPRS_LIMIT_MAX,          // 0x331200e1, 关模高压压力上限限制
  p_PP_MACHSET_CHGBKPRS_LIMIT_MAX,       // 0x331200e2, 储料背压上限限制
  p_PP_MACHSET_CHGSPD_LIMIT_MAX,         // 0x331200e3, 储料速度上限限制
  p_PP_MACHSET_CHGBKPRS_LIMIT_MAX_REAL,  // 0x331200e4, 储料背压上限最大值
  p_PP_MACHSET_LubrCnt,                  // 0x331200e5, 润滑组数
  p_PP_MACHSET_MOLD_NAME,                // 0x331200e6, 模具颜色
  p_PP_MACHSET_LOCK_SCALE,               // 0x331200e7, 锁模力比例系数
  p_PP_MACHSET_SPC_KEY_USE_JY,           // 0x331200e8, 金鹰按键排布特殊修改
  p_PP_MACHSET_CORE_KEY_SELECT,          // 0x331200e9, 中子按键选择
  p_PP_MACHSET_LUBR_BTN_FUNC,            // 0x331200ea, 润滑按键功能选择
  p_PP_MACHSET_INJSPC_AIRBLOWENABLE,     // 0x331200eb, 针阀吹气同步输出时是否变灰
  p_PP_MACHSET_FL_BLAST15,               // 0x331200ec, 吹气15方式
  p_PP_MACHSET_FL_BLAST16,               // 0x331200ed, 吹气16方式
  p_PP_MACHSET_FL_BLAST17,               // 0x331200ee, 吹气17方式
  p_PP_MACHSET_FL_BLAST18,               // 0x331200ef, 吹气18方式
  p_PP_MACHSET_FL_BLAST19,               // 0x331200f0, 吹气19方式
  p_PP_MACHSET_FL_BLAST20,               // 0x331200f1, 吹气20方式
  p_PP_MACHSET_FL_BLAST21,               // 0x331200f2, 吹气21方式
  p_PP_MACHSET_FL_BLAST22,               // 0x331200f3, 吹气22方式
  p_PP_MACHSET_FL_BLAST23,               // 0x331200f4, 吹气23方式
  p_PP_MACHSET_FL_BLAST24,               // 0x331200f5, 吹气24方式
  p_PP_MACHSET_FL_BLAST25,               // 0x331200f6, 吹气25方式
  p_PP_MACHSET_FL_BLAST26,               // 0x331200f7, 吹气26方式
  p_PP_MACHSET_FL_BLAST27,               // 0x331200f8, 吹气27方式
  p_PP_MACHSET_FL_BLAST28,               // 0x331200f9, 吹气28方式
  p_PP_MACHSET_SYSPAR_BLOW28_FUNC_BAKE,  // 0x331200fa, 是否使用28组吹气功能备用
  p_PP_MACHSET_ALARM_WRITETM,            // 0x331200fb, 保存警报间隔时间
  p_PP_MACHSET_WLSMACHINE_TYPE,          // 0x331200fc, 机器类型
  p_PP_MACHSET_FL_USEOPCUA,              // 0x331200fd, OPCUA功能
  p_PP_MACHSET_ACT_TEXT_NUM,             // 0x331200fe, 不翻译动作步
  p_PP_MACHSET_CLSHIPRS_LIMIT_MAX_REAL,  // 0x331200ff, 关模高压压力上限最大值
  p_PP_MACHSET_CHARGE_SPD_LIMIT_MAX_REAL, // 0x33120100, 储料速度上限最大值
  p_PP_MACHSET_PANEL_STARTUP_TM,         // 0x33120101, 面板本次运行时间
  p_PP_MACHSET_PUMP_FUNC_TABLE_NUM       // 0x33120102, 联动油路表标记
};

enum db_PP_SERVO{
  p_PP_SERVO_ID_A300 = 0x33130000, // 0x33130000, 驱动器控制模式 0:调试,3:EST控制,5:伺服5 新增
  p_PP_SERVO_ID_1F02,              // 0x33130001, 电机打包代码  新增
  p_PP_SERVO_ID_F101,              // 0x33130002, 0额定功率  0.4---1000.0 kW
  p_PP_SERVO_ID_F102,              // 0x33130003, 1额定电压  0---440 V
  p_PP_SERVO_ID_F103,              // 0x33130004, 2额定电流  0.01---655.35 A
  p_PP_SERVO_ID_F104,              // 0x33130005, 3额定频率  0.00---最大频率 Hz
  p_PP_SERVO_ID_F105,              // 0x33130006, 4额定转速  0---30000 rmp
  p_PP_SERVO_ID_F00A,              // 0x33130007, 5最大频率  50.00---300.00 Hz
  p_PP_SERVO_ID_F012,              // 0x33130008, 8减速时间  0.0---6500.0 s
  p_PP_SERVO_ID_A301,              // 0x33130009, 最大转速  0---30000rmp 新增
  p_PP_SERVO_ID_F00F,              // 0x3313000a, 6载波频率  0.5---16.0 Hz
  p_PP_SERVO_ID_F011,              // 0x3313000b, 7加速时间  0.0---6500.0 s
  p_PP_SERVO_ID_F200,              // 0x3313000c, 9速度环比例增益  0---100
  p_PP_SERVO_ID_F201,              // 0x3313000d, 10速度环积分时间  0.01---10.00 s
  p_PP_SERVO_ID_F20A,              // 0x3313000e, 扭矩上限  0.0---250.0 %  新增
  p_PP_SERVO_ID_F10B,              // 0x3313000f, 33D轴电感
  p_PP_SERVO_ID_F10C,              // 0x33130010, 34Q轴电感
  p_PP_SERVO_ID_F10D,              // 0x33130011, 35定子电阻
  p_PP_SERVO_ID_F10E,              // 0x33130012, 36单位
  p_PP_SERVO_ID_F10F,              // 0x33130013, 37反电动势
  p_PP_SERVO_ID_F20D,              // 0x33130014, 39DP
  p_PP_SERVO_ID_F20E,              // 0x33130015, 40DI
  p_PP_SERVO_ID_F20F,              // 0x33130016, 41QP
  p_PP_SERVO_ID_F210,              // 0x33130017, 42QI
  p_PP_SERVO_ID_A102,              // 0x33130018, 44初始角
  p_PP_SERVO_ID_A103,              // 0x33130019, 45方向
  p_PP_SERVO_ID_F21B,              // 0x3313001a, 自动相序调整  新增
  p_PP_SERVO_ID_F009,              // 0x3313001b, 31旋转方向
  p_PP_SERVO_ID_F00C,              // 0x3313001c, 32频率上限
  p_PP_SERVO_ID_F202,              // 0x3313001d, 切换频率1 0.00 ～F2-05 新增
  p_PP_SERVO_ID_F203,              // 0x3313001e, 38速度环比例增益2  0 ～ 400
  p_PP_SERVO_ID_F204,              // 0x3313001f, 速度环积分时间2  0.01s ～10.00s 新增
  p_PP_SERVO_ID_F205,              // 0x33130020, 切换频率2  F2-02 ～最大频率 新增
  p_PP_SERVO_ID_F819,              // 0x33130021, 制动单元允许开启时间 0.1s ～ 3600.0s 新增
  p_PP_SERVO_ID_F911,              // 0x33130022, 电机超温保护方式  新增
  p_PP_SERVO_ID_A308,              // 0x33130023, 最大反向速 0.0% ～ 100.0% 新增
  p_PP_SERVO_ID_A303,              // 0x33130024, 22最大油压  0.0---500.0 kg/cm2
  p_PP_SERVO_ID_A30A,              // 0x33130025, 23底压  0.0---50.0 kg/cm2
  p_PP_SERVO_ID_A322,              // 0x33130026, 从机中间点输入 A3-34 ～ A3-36   新增
  p_PP_SERVO_ID_A323,              // 0x33130027, 从机中间点输入对应 -100.0% ～ 100.0% 新增
  p_PP_SERVO_ID_A320,              // 0x33130028, 从机最小输入 0.0% ～ A3-34       新增
  p_PP_SERVO_ID_A321,              // 0x33130029, 从机最小输入对应 -100.0% ～ 100.0%   新增
  p_PP_SERVO_ID_A324,              // 0x3313002a, 从机最大输入 A3-34 ～ 100.0%    新增
  p_PP_SERVO_ID_A325,              // 0x3313002b, 从机最大输入对应 -100.0% ～ 100.0%   新增
  p_PP_SERVO_ID_A304,              // 0x3313002c, 油压指令斜坡时间 0.000s ～ 2.000s 新增
  p_PP_SERVO_ID_A305,              // 0x3313002d, 油压控制KP1   0.0 ～ 800.0 新增
  p_PP_SERVO_ID_A306,              // 0x3313002e, 油压控制ti1   0.001s ～ 10.000s 新增
  p_PP_SERVO_ID_A307,              // 0x3313002f, 油压控制td1   0.000s ～ 1.000s 新增
  p_PP_SERVO_ID_A309,              // 0x33130030, 底流   0.0% ～ 50.0% 新增
  p_PP_SERVO_ID_A315,              // 0x33130031, 油压传感器故障检测时间    0.000s：检测无效 0.001s ～ 60.000 新增
  p_PP_SERVO_ID_A319,              // 0x33130032, 给定油压上升S 滤波时间 0.000s ～ 1.000s 新增
  p_PP_SERVO_ID_A31A,              // 0x33130033, 给定油压下降S 滤波时间 0.000s ～ 1.000s 新增
  p_PP_SERVO_ID_A31B,              // 0x33130034, 超调抑制检测等级 0~2000 新增
  p_PP_SERVO_ID_A31E,              // 0x33130035, 压力模式切换速度模式转矩上限 50.0%~250.0% 新增
  p_PP_SERVO_ID_A326,              // 0x33130036, 多泵主机判断是否发送从机转速使能 0：禁止从机速度使能 1：允许从机速度使能 新增
  p_PP_SERVO_ID_A327,              // 0x33130037, 多泵合流保压控制增益 20~800 新增
  p_PP_SERVO_ID_A403,              // 0x33130038, 流量上升滤波时间 0~1.000s   新增
  p_PP_SERVO_ID_A404,              // 0x33130039, 流量下降滤波时间 0~1.000s   新增
  p_PP_SERVO_ID_A40A,              // 0x3313003a, 注射动作曲线S曲线上升时间 0.001s ～ 1.000s 新增
  p_PP_SERVO_ID_A40B,              // 0x3313003b, 注射动作曲线S曲线下降时间 0.001s ～ 1.000s 新增
  p_PP_SERVO_ID_A40C,              // 0x3313003c, 注射动作流量上升斜率 0~5.000s 新增
  p_PP_SERVO_ID_A40D,              // 0x3313003d, 注射动作流量下降斜率 0~5.000s 新增
  p_PP_SERVO_ID_A40E,              // 0x3313003e, 注射给定油压上升时间 0~2.000s 新增
  p_PP_SERVO_ID_A40F,              // 0x3313003f, 注射给定油压下降时间 0~2.000s 新增
  p_PP_SERVO_ID_A41A,              // 0x33130040, 油压 PID 算法选择   0~2     新增
  p_PP_SERVO_ID_A41B,              // 0x33130041, 保留  新增
  p_PP_SERVO_ID_F400,              // 0x33130042, DI1端子功能选择  0：无功能 1：正转使能 2：反转使能 3：三线式运行控制 4：正向点动使能 5：反向点动使能 6 ～ 7：保留 8：自由停车 9：故障复位 10：保留 11：外部故障常开输入 12 ～ 32：保留 33：外部故障常闭输入 34 ～ 47：保留 48：伺服油泵PID选择端子1 49：伺服油泵PID选择端子2 50：CAN 通信使能 51：从机作主机使能 52：压力切换到速度模式端子 53：从泵地址选择端子1 54：从泵地址选择端子2 55：射胶转保压端子 56：故障复位端子2过流故障无法复位） 新增
  p_PP_SERVO_ID_F401,              // 0x33130043, DI1端子功能选择  0：无功能 1：正转使能 2：反转使能 3：三线式运行控制 4：正向点动使能 5：反向点动使能 6 ～ 7：保留 8：自由停车 9：故障复位 10：保留 11：外部故障常开输入 12 ～ 32：保留 33：外部故障常闭输入 34 ～ 47：保留 48：伺服油泵PID选择端子1 49：伺服油泵PID选择端子2 50：CAN 通信使能 51：从机作主机使能 52：压力切换到速度模式端子 53：从泵地址选择端子1 54：从泵地址选择端子2 55：射胶转保压端子 56：故障复位端子2过流故障无法复位） 新增
  p_PP_SERVO_ID_F402,              // 0x33130044, DI1端子功能选择  0：无功能 1：正转使能 2：反转使能 3：三线式运行控制 4：正向点动使能 5：反向点动使能 6 ～ 7：保留 8：自由停车 9：故障复位 10：保留 11：外部故障常开输入 12 ～ 32：保留 33：外部故障常闭输入 34 ～ 47：保留 48：伺服油泵PID选择端子1 49：伺服油泵PID选择端子2 50：CAN 通信使能 51：从机作主机使能 52：压力切换到速度模式端子 53：从泵地址选择端子1 54：从泵地址选择端子2 55：射胶转保压端子 56：故障复位端子2过流故障无法复位） 新增
  p_PP_SERVO_ID_F403,              // 0x33130045, DI1端子功能选择  0：无功能 1：正转使能 2：反转使能 3：三线式运行控制 4：正向点动使能 5：反向点动使能 6 ～ 7：保留 8：自由停车 9：故障复位 10：保留 11：外部故障常开输入 12 ～ 32：保留 33：外部故障常闭输入 34 ～ 47：保留 48：伺服油泵PID选择端子1 49：伺服油泵PID选择端子2 50：CAN 通信使能 51：从机作主机使能 52：压力切换到速度模式端子 53：从泵地址选择端子1 54：从泵地址选择端子2 55：射胶转保压端子 56：故障复位端子2过流故障无法复位） 新增
  p_PP_SERVO_ID_F404,              // 0x33130046, DI1端子功能选择  0：无功能 1：正转使能 2：反转使能 3：三线式运行控制 4：正向点动使能 5：反向点动使能 6 ～ 7：保留 8：自由停车 9：故障复位 10：保留 11：外部故障常开输入 12 ～ 32：保留 33：外部故障常闭输入 34 ～ 47：保留 48：伺服油泵PID选择端子1 49：伺服油泵PID选择端子2 50：CAN 通信使能 51：从机作主机使能 52：压力切换到速度模式端子 53：从泵地址选择端子1 54：从泵地址选择端子2 55：射胶转保压端子 56：故障复位端子2过流故障无法复位） 新增
  p_PP_SERVO_ID_F412,              // 0x33130047, AI1最小输入  -11.00V ～ 11.00V    新增
  p_PP_SERVO_ID_F413,              // 0x33130048, AI1最小输入对应设定 -100.0% ～ 100.0% 新增
  p_PP_SERVO_ID_F414,              // 0x33130049, AI1最大输入  -11.00V ～ 11.00V    新增
  p_PP_SERVO_ID_F415,              // 0x3313004a, AI1最大输入对应设定 -100.0% ～ 100.0% 新增
  p_PP_SERVO_ID_F416,              // 0x3313004b, AI1输入滤波时间  0.000s ～ 10.000s 新增
  p_PP_SERVO_ID_F417,              // 0x3313004c, AI2最小输入  -11.00V ～ 11.00V    新增
  p_PP_SERVO_ID_F418,              // 0x3313004d, AI2最小输入对应设定 -100.0% ～ 100.0% 新增
  p_PP_SERVO_ID_F419,              // 0x3313004e, AI2最大输入  -11.00V ～ 11.00V    新增
  p_PP_SERVO_ID_F41A,              // 0x3313004f, AI2最大输入对应设定 -100.0% ～ 100.0% 新增
  p_PP_SERVO_ID_F41B,              // 0x33130050, AI2输入滤波时间  0.000s ～ 10.000s 新增
  p_PP_SERVO_ID_F41D,              // 0x33130051, AI3最小输入对应设定 -100.0% ～ 100.0% 新增
  p_PP_SERVO_ID_F41E,              // 0x33130052, AI3最大输入  -11.00V ～ 11.00V    新增
  p_PP_SERVO_ID_F41F,              // 0x33130053, AI3最大输入对应设定 -100.0% ～ 100.0% 新增
  p_PP_SERVO_ID_F420,              // 0x33130054, AI3输入滤波时间  0.000s ～ 10.000s 新增
  p_PP_SERVO_ID_F501,              // 0x33130055, 控制板继电器（T/A1-T/B1-T/C1）输出选择 0：无输出 1：伺服驱动器运行中 2：故障输出 3 ～ 5：保留 6：电机过载预报警 7：驱动器过载预报警 8 ～ 11：保留 12：运行时间到达 13 ～ 14：保留 15：运行准备就绪 16 ～ 19：保留 20: 通讯设定 21 ～ 22:保留 23：双排量柱塞泵斜盘切换1 24：压力控制状态输出 25：从泵报警输出 26：双排量柱塞泵斜盘切换2 27：母线电压建立 28：商务运行时间到达 29：商务运行时间不足24小时 30：最大反向转速DO输出 新增
  p_PP_SERVO_ID_F502,              // 0x33130056, 控制板继电器（T/A2-T/C2）输出选择 0：无输出 1：伺服驱动器运行中 2：故障输出 3 ～ 5：保留 6：电机过载预报警 7：驱动器过载预报警 8 ～ 11：保留 12：运行时间到达 13 ～ 14：保留 15：运行准备就绪 16 ～ 19：保留 20: 通讯设定 21 ～ 22:保留 23：双排量柱塞泵斜盘切换1 24：压力控制状态输出 25：从泵报警输出 26：双排量柱塞泵斜盘切换2 27：母线电压建立 28：商务运行时间到达 29：商务运行时间不足24小时 30：最大反向转速DO输出 新增
  p_PP_SERVO_ID_F503,              // 0x33130057, 控制板继电器（T/A3-T/C3）输出选择 0：无输出 1：伺服驱动器运行中 2：故障输出 3 ～ 5：保留 6：电机过载预报警 7：驱动器过载预报警 8 ～ 11：保留 12：运行时间到达 13 ～ 14：保留 15：运行准备就绪 16 ～ 19：保留 20: 通讯设定 21 ～ 22:保留 23：双排量柱塞泵斜盘切换1 24：压力控制状态输出 25：从泵报警输出 26：双排量柱塞泵斜盘切换2 27：母线电压建立 28：商务运行时间到达 29：商务运行时间不足24小时 30：最大反向转速DO输出 新增
  p_PP_SERVO_ID_F50A,              // 0x33130058, AO1输出选择  0：运行频率 1：设定频率 2：输出电流 3：输出转矩 4：输出功率 5：输出电压 6：保留 7：AI1 8：AI2 9：AI3 10：反馈转速（油压模式） 11：反馈压力（油压模式） 12 ～ 16：保留  新增
  p_PP_SERVO_ID_F50B,              // 0x33130059, AO1输出选择  0：运行频率 1：设定频率 2：输出电流 3：输出转矩 4：输出功率 5：输出电压 6：保留 7：AI1 8：AI2 9：AI3 10：反馈转速（油压模式） 11：反馈压力（油压模式） 12 ～ 16：保留  新增
  p_PP_SERVO_ID_F50E,              // 0x3313005a, AO1零偏系数  -100.0% ～ 100.0%  新增
  p_PP_SERVO_ID_F50F,              // 0x3313005b, AO1增益      -10.00 ～ 10.00    新增
  p_PP_SERVO_ID_F510,              // 0x3313005c, AO2零偏系数  -100.0% ～ 100.0%  新增
  p_PP_SERVO_ID_F511,              // 0x3313005d, AO2增益      -10.00 ～ 10.00    新增
  p_PP_SERVO_ID_F100,              // 0x3313005e, 电机类型选择  0：普通异步电机 1：变频异步电机 2：永磁同步伺服电机  新增
  p_PP_SERVO_ID_F206,              // 0x3313005f, 转差补偿系数  50% ～200%   新增
  p_PP_SERVO_ID_F207,              // 0x33130060, 速度反馈滤波时间 0.5ms ～ 10.0ms 新增
  p_PP_SERVO_ID_F208,              // 0x33130061, 转矩控制  0：无效 1：有效  新增
  p_PP_SERVO_ID_F209,              // 0x33130062, 转矩上限源  0：F2-10 1：AI1 2：AI2 3：AI3 4：保留 5：通信给定模拟输入量程对应F2-10  新增
  p_PP_SERVO_ID_F20B,              // 0x33130063, 转矩滤波带宽  0Hz~1500Hz 新增
  p_PP_SERVO_ID_F20C,              // 0x33130064, 保留   新增
  p_PP_SERVO_ID_F211,              // 0x33130065, 1
  p_PP_SERVO_ID_F212,              // 0x33130066, 1
  p_PP_SERVO_ID_F213,              // 0x33130067, 1
  p_PP_SERVO_ID_F214,              // 0x33130068, 1
  p_PP_SERVO_ID_F215,              // 0x33130069, 1
  p_PP_SERVO_ID_F216,              // 0x3313006a, 1
  p_PP_SERVO_ID_F217,              // 0x3313006b, 1
  p_PP_SERVO_ID_F218,              // 0x3313006c, 1
  p_PP_SERVO_ID_F219,              // 0x3313006d, 过电压调制系数  100%~120%  新增
  p_PP_SERVO_ID_F21A,              // 0x3313006e, 母线电压滤波   0.000~0.100  新增
  p_PP_SERVO_ID_F21C,              // 0x3313006f, 1
  p_PP_SERVO_ID_F90C,              // 0x33130070,  输入缺相保护选择  0：禁止 1：允许  新增
  p_PP_SERVO_ID_F90D,              // 0x33130071, 输出缺相保护选择  0：禁止 1：允许  新增
  p_PP_SERVO_ID_F90E,              // 0x33130072, 飞车速度偏差设定  0.50Hz~50.00Hz 新增
  p_PP_SERVO_ID_F90F,              // 0x33130073, 飞车故障判断时间  0.1s~20.0S     新增
  p_PP_SERVO_ID_F910,              // 0x33130074, 电机温度保护使能  0：禁止 1：允许  新增
  p_PP_SERVO_ID_A000,              // 0x33130075, 弱磁控制方式  0：直接计算 1：自动调整 2：自动调整 + 计算  新增
  p_PP_SERVO_ID_A001,              // 0x33130076, 弱磁电流系数  0~500    新增
  p_PP_SERVO_ID_A002,              // 0x33130077, 同步机弱磁深度  0~50%  新增
  p_PP_SERVO_ID_A003,              // 0x33130078, 同步机最大出力调整增益     20%~300%  新增
  p_PP_SERVO_ID_A004,              // 0x33130079, 同步机计算励磁电流调整增益 40%~200%  新增
  p_PP_SERVO_ID_A100,              // 0x3313007a, PG卡类型  0：旋转变压器 1：保留 2：普通 ABZ 编码器  新增
  p_PP_SERVO_ID_A104,              // 0x3313007b, 旋转变压器极对数     1 ～ 50  新增
  p_PP_SERVO_ID_A105,              // 0x3313007c, 旋变断线故障检测时间  0.000：检测无效 0.001s ～ 60.000s 新增
  p_PP_SERVO_ID_A106,              // 0x3313007d, 编码器线数  0 ～ 65535  新增
  p_PP_SERVO_ID_A200,              // 0x3313007e, 波特率选择  0：20k 1：50k 2：125k 3：250k 4：500k 5：1M  新增
  p_PP_SERVO_ID_A202,              // 0x3313007f, CAN 连续通讯时间  0.0S（无效）0.1S～ 600.0s  新增
  p_PP_SERVO_ID_A203,              // 0x33130080, CAN 多泵模式  0（广播模式）1（多主模式） 新增
  p_PP_SERVO_ID_A204,              // 0x33130081, CAN 从机地址1  0~65535  新增
  p_PP_SERVO_ID_A205,              // 0x33130082, CAN 从机地址2  0~65535  新增
  p_PP_SERVO_ID_A206,              // 0x33130083, CAN 从机地址3  0~65535  新增
  p_PP_SERVO_ID_A207,              // 0x33130084, CAN 从机地址4  0~65535  新增
  p_PP_SERVO_ID_A208,              // 0x33130085, 1
  p_PP_SERVO_ID_A209,              // 0x33130086, 1
  p_PP_SERVO_ID_A302,              // 0x33130087, 系统油压  0.0kg/cm2 ～最大油压（A3-03）  新增
  p_PP_SERVO_ID_A30B,              // 0x33130088, 油压控制 Kp2  0.0 ～ 800.0      新增
  p_PP_SERVO_ID_A30C,              // 0x33130089, 油压控制 Ti2  0.001s ～ 10.000s 新增
  p_PP_SERVO_ID_A30D,              // 0x3313008a, 油压控制 Td2  0.000s ～ 1.000s  新增
  p_PP_SERVO_ID_A30E,              // 0x3313008b, 油压控制 Kp3  0.0 ～ 800.0      新增
  p_PP_SERVO_ID_A30F,              // 0x3313008c, 油压控制 Ti3  0.001s ～ 10.000s 新增
  p_PP_SERVO_ID_A310,              // 0x3313008d, 油压控制 Td3  0.000s ～ 1.000s  新增
  p_PP_SERVO_ID_A311,              // 0x3313008e, 油压控制 Kp4  0.0 ～ 800.0      新增
  p_PP_SERVO_ID_A312,              // 0x3313008f, 油压控制 Ti4  0.001s ～ 10.000s 新增
  p_PP_SERVO_ID_A313,              // 0x33130090, 油压控制 Td4  0.000s ～ 1.000s  新增
  p_PP_SERVO_ID_A316,              // 0x33130091, 压力控制状态输出最高转速设定  0.0% ～ 100.0%  新增
  p_PP_SERVO_ID_A317,              // 0x33130092, 压力控制状态输出最低油压设定  0.0% ～ 100.0%  新增
  p_PP_SERVO_ID_A318,              // 0x33130093, 压力控制状态输出延迟时间 0.000s ～ 10.000s 新增
  p_PP_SERVO_ID_A31C,              // 0x33130094, 超调抵制系数   0~3.000  新增
  p_PP_SERVO_ID_A31D,              // 0x33130095, 油压环增益系数  0.20~5.00  新增
  p_PP_SERVO_ID_A31F,              // 0x33130096, 注射阀门开通延迟时间  0.020s ～ 0.500s  新增
  p_PP_SERVO_ID_A328,              // 0x33130097, 多泵注射降 PI去抖压力偏差 0.0~50.0kg  新增
  p_PP_SERVO_ID_A329,              // 0x33130098, 多泵注射降 PI去抖流量下限 0~30000rpm 新增
  p_PP_SERVO_ID_A32A,              // 0x33130099, 多泵注射降 PI去抖流量检测时间  0.200~2.000s 新增
  p_PP_SERVO_ID_A32B,              // 0x3313009a, 多泵 CAN 通讯模式下从泵不工作的压力偏差  0~50.0kg 新增
  p_PP_SERVO_ID_A32C,              // 0x3313009b, 多泵 CAN 通讯模式下从泵不工作的流量下限 -100.0%~100.0% 新增
  p_PP_SERVO_ID_A32D,              // 0x3313009c, 从泵无转速指令停机判断时间 0.100~5.000s 新增
  p_PP_SERVO_ID_A32E,              // 0x3313009d, 从泵无转速指令停机减速时间 0.001~5.000s 新增
  p_PP_SERVO_ID_A32F,              // 0x3313009e, 启动阀门卸压延迟  0.001~5.000s  新增
  p_PP_SERVO_ID_A330,              // 0x3313009f, 退出阀门卸压延迟  0.001~5.000s  新增
  p_PP_SERVO_ID_A331,              // 0x331300a0, 启动阀门卸压压力偏差下限  0.0~A3-02( 系统油压） 新增
  p_PP_SERVO_ID_A332,              // 0x331300a1, 启动阀门卸压压力设定下限  0.0~A3-02( 系统油压） 新增
  p_PP_SERVO_ID_A333,              // 0x331300a2, 压力传感器故障检测电流下限 0%~300%（电机额定电流F1-03） 新增
  p_PP_SERVO_ID_A334,              // 0x331300a3, 压力传感器故障检测速度上限 0%~100%（最大转速 A3-01） 新增
  p_PP_SERVO_ID_A400,              // 0x331300a4, 转速滤波时间  0~5.000S 新增
  p_PP_SERVO_ID_A401,              // 0x331300a5, 电流滤波时间  0~5.000S 新增
  p_PP_SERVO_ID_A402,              // 0x331300a6, 保留 新增
  p_PP_SERVO_ID_A405,              // 0x331300a7, 保留 新增
  p_PP_SERVO_ID_A406,              // 0x331300a8, 流量泄漏补偿值 0.0%~50.0% 新增
  p_PP_SERVO_ID_A407,              // 0x331300a9, 保留 新增
  p_PP_SERVO_ID_A408,              // 0x331300aa, 反转泄压最小压力 0.0kg/cm2 ～ A3-02  新增
  p_PP_SERVO_ID_A409,              // 0x331300ab, 反转泄压长时间运行保护时间 0.001s ～ 5.000s 新增
  p_PP_SERVO_ID_A410,              // 0x331300ac, 注射超调抑制检测等级  0~2000 新增
  p_PP_SERVO_ID_A411,              // 0x331300ad, 注射超调抑制系数  0~3.000s  新增
  p_PP_SERVO_ID_A412,              // 0x331300ae, 保留 新增
  p_PP_SERVO_ID_A413,              // 0x331300af, 保留 新增
  p_PP_SERVO_ID_A414,              // 0x331300b0, 保留 新增
  p_PP_SERVO_ID_A415,              // 0x331300b1, 保留 新增
  p_PP_SERVO_ID_A416,              // 0x331300b2, 油压抑制取消油压偏差  0.0kg/cm2 ～ A3-02  新增
  p_PP_SERVO_ID_A417,              // 0x331300b3, 积分限制偏差最大值   0.0kg/cm2 ～ A3-02  新增
  p_PP_SERVO_ID_A418,              // 0x331300b4, 积分限幅模式选择  0~1  新增
  p_PP_SERVO_ID_A419,              // 0x331300b5, 压力环输出上限增幅 0~50.0 新增
  p_PP_SERVO_ID_A41C,              // 0x331300b6, 1
  p_PP_SERVO_ID_A41D,              // 0x331300b7, 1
  p_PP_SERVO_ID_A41E,              // 0x331300b8, 1
  p_PP_SERVO_ID_A41F,              // 0x331300b9, 1
  p_PP_SERVO_ID_A420,              // 0x331300ba, 1
  p_PP_SERVO_ID_E000,              // 0x331300bb, 驱动器控制模式 0:调试
  p_PP_SERVO_ID_E002,              // 0x331300bc, 11转矩上限  0.0---250.0 %
  p_PP_SERVO_ID_1001,              // 0x331300bd, 12运行频率  0.00---最大频率 Hz  状态量
  p_PP_SERVO_ID_1003,              // 0x331300be, 13输出电压  0.00---电机额定电压 V  状态量
  p_PP_SERVO_ID_1004,              // 0x331300bf, 14输出电流  0.01---655.35 A   状态量
  p_PP_SERVO_ID_1006,              // 0x331300c0, 15输出转矩  0.0%---扭矩上限 %  状态量
  p_PP_SERVO_ID_100F,              // 0x331300c1, 16电机转速  -9999---30000 rmp  状态量
  p_PP_SERVO_ID_3000,              // 0x331300c2, 17驱动器运行状态   状态量
  p_PP_SERVO_ID_8000,              // 0x331300c3, 18驱动器状态   状态量
  p_PP_SERVO_ID_8001,              // 0x331300c4, 19通讯状态  状态量
  p_PP_SERVO_ID_F70A,              // 0x331300c5, 20版本号1  状态量
  p_PP_SERVO_ID_F70B,              // 0x331300c6, 21版本号2  状态量
  p_PP_SERVO_ID_100B,              // 0x331300c7, 检测前零漂电压  0.00---5.00 V
  p_PP_SERVO_ID_F41C,              // 0x331300c8, 检测后零漂电压  0.00---5.00 V
  p_PP_SERVO_ID_1002,              // 0x331300c9, 母线电压
  p_PP_SERVO_ID_1005,              // 0x331300ca, 功率
  p_PP_SERVO_ID_1013,              // 0x331300cb, 实时角度
  p_PP_SERVO_ID_1707,              // 0x331300cc, 模块温度
  p_PP_SERVO_ID_A005,              // 0x331300cd, 缺相检测T
  p_PP_SERVO_ID_1019,              // 0x331300ce, 过载累计
  p_PP_SERVO_ID_1015,              // 0x331300cf, 反馈油压
  p_PP_SERVO_ID_1017,              // 0x331300d0, 给定流量
  p_PP_SERVO_SERVO_MOTORBRAND,     // 0x331300d1, 电机品牌
  p_PP_SERVO_SERVO_PARRESETOK,     // 0x331300d2, 驱动参数对话框确定
  p_PP_SERVO_SERVO_SELFLEARN,      // 0x331300d3, 伺服自学习确认
  p_PP_SERVO_SERVO_SELFLEANSWITCH, // 0x331300d4, 伺服自学习开关
  p_PP_SERVO_SERVO_MINROTATINGSPD, // 0x331300d5, 最小转速
  p_PP_SERVO_SERVO_MAXROTATINGSPD, // 0x331300d6, 最大转速
  p_PP_SERVO_SERVO_EVERYSELECT,    // 0x331300d7, 自学习画面各选项
  p_PP_SERVO_SERVO_RATEDSET,       // 0x331300d8, 额定数据是否只读
  p_PP_SERVO_SERVO_MAXROTATIONGSET, // 0x331300d9, 最大转速是否允许修改
  p_PP_SERVO_SERVO_SELFLEARNPROMPT, // 0x331300da, 自学习步奏或警报提示
  p_PP_SERVO_SERVO_DEBUGZERO,      // 0x331300db, 伺服模式和传感器归零是否允许使用
  p_PP_SERVO_SERVO_ROTATEDDIR,     // 0x331300dc, 旋转方向是否变灰
  p_PP_SERVO_SERVO_MAINID,         // 0x331300dd, 伺服主泵个数
  p_PP_SERVO_SERVO_CURVE,          // 0x331300de, 伺服曲线
  p_PP_SERVO_SERVO_MDTEMP,         // 0x331300df, 实时角度和模块温度是否可用
  p_PP_SERVO_SERVO_RS232_RX,       // 0x331300e0, 测试RS232接收数据
  p_PP_SERVO_SERVO_RS232_TX,       // 0x331300e1, 测试RS232发送数据
  p_PP_SERVO_SERVO_PDLOSTBG,       // 0x331300e2, 面板与驱动丢包背景色
  p_PP_SERVO_SERVO_PMLOSTBG,       // 0x331300e3, 面板与主机丢包背景色
  p_PP_SERVO_SERVO_MDLOSTBG,       // 0x331300e4, 主机与驱动丢包背景色
  p_PP_SERVO_SERVO_SPEED,          // 0x331300e5, 电机转速
  p_PP_SERVO_SERVO_PRESS,          // 0x331300e6, 系统反馈压力
  p_PP_SERVO_SERVO_CURRENT,        // 0x331300e7, 输出电流
  p_PP_SERVO_SERVO_CURRENT_NUM     // 0x331300e8, 伺服当前个数
};

enum db_PP_STATE{
  p_PP_STATE_ActName1 = 0x33160000, // 0x33160000, 动作名称1
  p_PP_STATE_ActSt1,                // 0x33160001, 动作状态1
  p_PP_STATE_ActT1,                 // 0x33160002, 动作时间1
  p_PP_STATE_ActName2,              // 0x33160003, 动作名称2
  p_PP_STATE_ActSt2,                // 0x33160004, 动作状态2
  p_PP_STATE_ActT2,                 // 0x33160005, 动作时间2
  p_PP_STATE_ActName3,              // 0x33160006, 动作名称3
  p_PP_STATE_ActSt3,                // 0x33160007, 动作状态3
  p_PP_STATE_ActT3,                 // 0x33160008, 动作时间2
  p_PP_STATE_Warn1,                 // 0x33160009, 警报状态1
  p_PP_STATE_Warn2,                 // 0x3316000a, 警报状态2
  p_PP_STATE_Warn3,                 // 0x3316000b, 警报状态3
  p_PP_STATE_totalOpenCnt,          // 0x3316000c, 开模总数
  p_PP_STATE_totalProdCnt,          // 0x3316000d, 产品总数
  p_PP_STATE_shotCntPerHour,        // 0x3316000e, 每小时开模数
  p_PP_STATE_remainTimeH,           // 0x3316000f, 剩余时间小时
  p_PP_STATE_remainTimeM,           // 0x33160010, 剩余时间分钟
  p_PP_STATE_badShot,               // 0x33160011, 当前不良品数
  p_PP_STATE_goodCntTotal,          // 0x33160012, 良品总数
  p_PP_STATE_opMode,                // 0x33160013, 操作状态
  p_PP_STATE_isRemainTimeShow,      // 0x33160014, 预温剩余时间显示
  p_PP_STATE_remainMin,             // 0x33160015, 预温剩余时间分
  p_PP_STATE_remainSec,             // 0x33160016, 预温剩余时间秒
  p_PP_STATE_ADClampPos,            // 0x33160017, 开关模电子尺位置，只用于有推力座点情况下
  p_PP_STATE_TESTKEY_FLAG,          // 0x33160018, 测试按键标记
  p_PP_STATE_PROG_CYCTIME,          // 0x33160019, 程序运行循环时间
  p_PP_STATE_REBOOT_PANEL,          // 0x3316001a, 面板重启标志
  p_PP_STATE_PROGUPDATE_FLAG,       // 0x3316001b, 程序更新标记
  p_PP_STATE_ActState,              // 0x3316001c, 全自动过程中的动作状态，用于开关模部分的动画显示
  p_PP_STATE_InjectTimeLast,        // 0x3316001d, 上一模射出计时
  p_PP_STATE_CoolTimeLast,          // 0x3316001e, 上一模冷却计时
  p_PP_STATE_BAR_CLOSE_VAL,         // 0x3316001f, 关模时间进度条
  p_PP_STATE_BAR_INJECT_VAL,        // 0x33160020, 射出时间进度条
  p_PP_STATE_BAR_CHARGE_VAL,        // 0x33160021, 储料时间进度条
  p_PP_STATE_BAR_COOL_VAL,          // 0x33160022, 冷却时间进度条
  p_PP_STATE_BAR_OPEN_VAL,          // 0x33160023, 开模时间进度条
  p_PP_STATE_BAR_CYCLE_VAL,         // 0x33160024, 循环时间进度条
  p_PP_STATE_BAR_CLOSE_LAST_VAL,    // 0x33160025, 上一模关模时间进度条
  p_PP_STATE_BAR_INJECT_LAST_VAL,   // 0x33160026, 上一模射出时间进度条
  p_PP_STATE_BAR_CHARGE_LAST_VAL,   // 0x33160027, 上一模储料时间
  p_PP_STATE_BAR_COOL_LAST_VAL,     // 0x33160028, 上一模冷却时间
  p_PP_STATE_BAR_OPEN_LAST_VAL,     // 0x33160029, 上一模开模时间进度条
  p_PP_STATE_BAR_CYCLE_LAST_VAL,    // 0x3316002a, 上一模循环时间进度条
  p_PP_STATE_OPTIMIZE_HOUR_LAST,    // 0x3316002b, 上一次温度优化小时
  p_PP_STATE_OPTIMIZE_MIN_LAST,     // 0x3316002c, 上一次温度优化分钟
  p_PP_STATE_OPTIMIZE_HOUR_CUR,     // 0x3316002d, 当前温度优化小时
  p_PP_STATE_OPTIMIZE_MIN_CUR,      // 0x3316002e, 当前温度优化分钟
  p_PP_STATE_SHOT_TOTAL_CNT,        // 0x3316002f, 总开模数累计
  p_PP_STATE_isRemain2TimeShow,     // 0x33160030, 预温2剩余时间显示
  p_PP_STATE_remain2Min,            // 0x33160031, 预温2剩余时间分
  p_PP_STATE_remain2Sec,            // 0x33160032, 预温2剩余时间秒
  p_PP_STATE_TEMP_CTRL_PART,        // 0x33160033, 温度控制部件选择标记
  p_PP_STATE_ActName4,              // 0x33160034, 动作名称4
  p_PP_STATE_ActSt4,                // 0x33160035, 动作状态4
  p_PP_STATE_ActT4,                 // 0x33160036, 动作时间4
  p_PP_STATE_MT_CTRL_PART,          // 0x33160037, 模具温度控制部件选择标记
  p_PP_STATE_MT_CUR_SELECT,         // 0x33160038, 模温当前选择的项
  p_PP_STATE_PART_REQ_STATE,        // 0x33160039, 部件（温度、伺服、模温...）请求是否存在，按位判断
  p_PP_STATE_SYSPRS1_CUR,           // 0x3316003a, 系统压力1当前值
  p_PP_STATE_SYSPRS2_CUR            // 0x3316003b, 系统压力2当前值
};

enum db_PP_TMP{
  p_PP_TMP_TMPVAL0 = 0x33150000, // 0x33150000, 
  p_PP_TMP_TMPVAL1,              // 0x33150001, 
  p_PP_TMP_TMPVAL2,              // 0x33150002, 
  p_PP_TMP_TMPVAL3,              // 0x33150003, 
  p_PP_TMP_TMPVAL4,              // 0x33150004, 
  p_PP_TMP_TMPVAL5,              // 0x33150005, 
  p_PP_TMP_TMPVAL6,              // 0x33150006, 
  p_PP_TMP_TMPVAL7,              // 0x33150007, 
  p_PP_TMP_TMPVAL8,              // 0x33150008, 
  p_PP_TMP_TMPVAL9,              // 0x33150009, 
  p_PP_TMP_TMPVAL10,             // 0x3315000a, 
  p_PP_TMP_TMPVAL11,             // 0x3315000b, 
  p_PP_TMP_TMPVAL12,             // 0x3315000c, 
  p_PP_TMP_TMPVAL13,             // 0x3315000d, 
  p_PP_TMP_TMPVAL14,             // 0x3315000e, 
  p_PP_TMP_TMPVAL15,             // 0x3315000f, 
  p_PP_TMP_TMPVAL16,             // 0x33150010, 
  p_PP_TMP_TMPVAL17,             // 0x33150011, 
  p_PP_TMP_TMPVAL18,             // 0x33150012, 
  p_PP_TMP_TMPVAL19,             // 0x33150013, 
  p_PP_TMP_TMPVAL20,             // 0x33150014, 
  p_PP_TMP_TMPVAL21,             // 0x33150015, 
  p_PP_TMP_TMPVAL22,             // 0x33150016, 
  p_PP_TMP_TMPVAL23,             // 0x33150017, 
  p_PP_TMP_TMPVAL24,             // 0x33150018, 
  p_PP_TMP_TMPVAL25,             // 0x33150019, 
  p_PP_TMP_TMPVAL26,             // 0x3315001a, 
  p_PP_TMP_TMPVAL27,             // 0x3315001b, 
  p_PP_TMP_TMPVAL28,             // 0x3315001c, 
  p_PP_TMP_TMPVAL29,             // 0x3315001d, 
  p_PP_TMP_TMPVAL30,             // 0x3315001e, 
  p_PP_TMP_TMPVAL31,             // 0x3315001f, 
  p_PP_TMP_TMPVAL32,             // 0x33150020, 
  p_PP_TMP_TMPVAL33,             // 0x33150021, 
  p_PP_TMP_TMPVAL34,             // 0x33150022, 
  p_PP_TMP_TMPVAL35,             // 0x33150023, 
  p_PP_TMP_TMPVAL36,             // 0x33150024, 
  p_PP_TMP_TMPVAL37,             // 0x33150025, 
  p_PP_TMP_TMPVAL38,             // 0x33150026, 
  p_PP_TMP_TMPVAL39,             // 0x33150027, 
  p_PP_TMP_TMPVAL40,             // 0x33150028, 
  p_PP_TMP_TMPVAL41,             // 0x33150029, 
  p_PP_TMP_TMPVAL42,             // 0x3315002a, 
  p_PP_TMP_TMPVAL43,             // 0x3315002b, 
  p_PP_TMP_TMPVAL44,             // 0x3315002c, 
  p_PP_TMP_TMPVAL45,             // 0x3315002d, 
  p_PP_TMP_TMPVAL46,             // 0x3315002e, 
  p_PP_TMP_TMPVAL47,             // 0x3315002f, 
  p_PP_TMP_TMPVAL48,             // 0x33150030, 
  p_PP_TMP_TMPVAL49,             // 0x33150031, 
  p_PP_TMP_TMPVAL50,             // 0x33150032, 
  p_PP_TMP_TMPVAL51,             // 0x33150033, 
  p_PP_TMP_TMPVAL52,             // 0x33150034, 
  p_PP_TMP_TMPVAL53,             // 0x33150035, 
  p_PP_TMP_TMPVAL54,             // 0x33150036, 
  p_PP_TMP_TMPVAL55,             // 0x33150037, 
  p_PP_TMP_TMPVAL56,             // 0x33150038, 
  p_PP_TMP_TMPVAL57,             // 0x33150039, 
  p_PP_TMP_TMPVAL58,             // 0x3315003a, 
  p_PP_TMP_TMPVAL59,             // 0x3315003b, 
  p_PP_TMP_TMPVAL60,             // 0x3315003c, 
  p_PP_TMP_TMPVAL61,             // 0x3315003d, 
  p_PP_TMP_TMPVAL62,             // 0x3315003e, 
  p_PP_TMP_TMPVAL63,             // 0x3315003f, 
  p_PP_TMP_TMPVAL64,             // 0x33150040, 
  p_PP_TMP_TMPVAL65,             // 0x33150041, 
  p_PP_TMP_TMPVAL66,             // 0x33150042, 
  p_PP_TMP_TMPVAL67,             // 0x33150043, 
  p_PP_TMP_TMPVAL68,             // 0x33150044, 
  p_PP_TMP_TMPVAL69,             // 0x33150045, 
  p_PP_TMP_TMPVAL70,             // 0x33150046, 
  p_PP_TMP_TMPVAL71,             // 0x33150047, 
  p_PP_TMP_TMPVAL72,             // 0x33150048, 
  p_PP_TMP_TMPVAL73,             // 0x33150049, 
  p_PP_TMP_TMPVAL74,             // 0x3315004a, 
  p_PP_TMP_TMPVAL75,             // 0x3315004b, 
  p_PP_TMP_TMPVAL76,             // 0x3315004c, 
  p_PP_TMP_TMPVAL77,             // 0x3315004d, 
  p_PP_TMP_TMPVAL78,             // 0x3315004e, 
  p_PP_TMP_TMPVAL79,             // 0x3315004f, 
  p_PP_TMP_TMPVAL80,             // 0x33150050, 
  p_PP_TMP_TMPVAL81,             // 0x33150051, 
  p_PP_TMP_TMPVAL82,             // 0x33150052, 
  p_PP_TMP_TMPVAL83,             // 0x33150053, 
  p_PP_TMP_TMPVAL84,             // 0x33150054, 
  p_PP_TMP_TMPVAL85,             // 0x33150055, 
  p_PP_TMP_TMPVAL86,             // 0x33150056, 
  p_PP_TMP_TMPVAL87,             // 0x33150057, 
  p_PP_TMP_TMPVAL88,             // 0x33150058, 
  p_PP_TMP_TMPVAL89,             // 0x33150059, 
  p_PP_TMP_TMPVAL90,             // 0x3315005a, 
  p_PP_TMP_TMPVAL91,             // 0x3315005b, 
  p_PP_TMP_TMPVAL92,             // 0x3315005c, 
  p_PP_TMP_TMPVAL93,             // 0x3315005d, 
  p_PP_TMP_TMPVAL94,             // 0x3315005e, 
  p_PP_TMP_TMPVAL95,             // 0x3315005f, 
  p_PP_TMP_TMPVAL96,             // 0x33150060, 
  p_PP_TMP_TMPVAL97,             // 0x33150061, 
  p_PP_TMP_TMPVAL98,             // 0x33150062, 
  p_PP_TMP_TMPVAL99,             // 0x33150063, 
  p_PP_TMP_TMPVAL100,            // 0x33150064, 
  p_PP_TMP_TMPVAL101,            // 0x33150065, 
  p_PP_TMP_TMPVAL102,            // 0x33150066, 
  p_PP_TMP_TMPVAL103,            // 0x33150067, 
  p_PP_TMP_TMPVAL104,            // 0x33150068, 
  p_PP_TMP_TMPVAL105,            // 0x33150069, 
  p_PP_TMP_TMPVAL106,            // 0x3315006a, 
  p_PP_TMP_TMPVAL107,            // 0x3315006b, 
  p_PP_TMP_TMPVAL108,            // 0x3315006c, 
  p_PP_TMP_TMPVAL109,            // 0x3315006d, 
  p_PP_TMP_TMPVAL110,            // 0x3315006e, 
  p_PP_TMP_TMPVAL111,            // 0x3315006f, 
  p_PP_TMP_TMPVAL112,            // 0x33150070, 
  p_PP_TMP_TMPVAL113,            // 0x33150071, 
  p_PP_TMP_TMPVAL114,            // 0x33150072, 
  p_PP_TMP_TMPVAL115,            // 0x33150073, 
  p_PP_TMP_TMPVAL116,            // 0x33150074, 
  p_PP_TMP_TMPVAL117,            // 0x33150075, 
  p_PP_TMP_TMPVAL118,            // 0x33150076, 
  p_PP_TMP_TMPVAL119,            // 0x33150077, 
  p_PP_TMP_TMPVAL120,            // 0x33150078, 
  p_PP_TMP_TMPVAL121,            // 0x33150079, 
  p_PP_TMP_TMPVAL122,            // 0x3315007a, 
  p_PP_TMP_TMPVAL123,            // 0x3315007b, 
  p_PP_TMP_TMPVAL124,            // 0x3315007c, 
  p_PP_TMP_TMPVAL125,            // 0x3315007d, 
  p_PP_TMP_TMPVAL126,            // 0x3315007e, 
  p_PP_TMP_TMPVAL127,            // 0x3315007f, 
  p_PP_TMP_TMPVAL128,            // 0x33150080, 
  p_PP_TMP_TMPVAL129,            // 0x33150081, 
  p_PP_TMP_TMPVAL130,            // 0x33150082, 
  p_PP_TMP_TMPVAL131,            // 0x33150083, 
  p_PP_TMP_TMPVAL132,            // 0x33150084, 
  p_PP_TMP_TMPVAL133,            // 0x33150085, 
  p_PP_TMP_TMPVAL134,            // 0x33150086, 
  p_PP_TMP_TMPVAL135,            // 0x33150087, 
  p_PP_TMP_TMPVAL136,            // 0x33150088, 
  p_PP_TMP_TMPVAL137,            // 0x33150089, 
  p_PP_TMP_TMPVAL138,            // 0x3315008a, 
  p_PP_TMP_TMPVAL139,            // 0x3315008b, 
  p_PP_TMP_TMPVAL140,            // 0x3315008c, 
  p_PP_TMP_TMPVAL141,            // 0x3315008d, 
  p_PP_TMP_TMPVAL142,            // 0x3315008e, 
  p_PP_TMP_TMPVAL143,            // 0x3315008f, 
  p_PP_TMP_TMPVAL144,            // 0x33150090, 
  p_PP_TMP_TMPVAL145,            // 0x33150091, 
  p_PP_TMP_TMPVAL146,            // 0x33150092, 
  p_PP_TMP_TMPVAL147,            // 0x33150093, 
  p_PP_TMP_TMPVAL148,            // 0x33150094, 
  p_PP_TMP_TMPVAL149,            // 0x33150095, 
  p_PP_TMP_TMPVAL150,            // 0x33150096, 
  p_PP_TMP_TMPVAL151,            // 0x33150097, 
  p_PP_TMP_TMPVAL152,            // 0x33150098, 
  p_PP_TMP_TMPVAL153,            // 0x33150099, 
  p_PP_TMP_TMPVAL154,            // 0x3315009a, 
  p_PP_TMP_TMPVAL155,            // 0x3315009b, 
  p_PP_TMP_TMPVAL156,            // 0x3315009c, 
  p_PP_TMP_TMPVAL157,            // 0x3315009d, 
  p_PP_TMP_TMPVAL158,            // 0x3315009e, 
  p_PP_TMP_TMPVAL159,            // 0x3315009f, 
  p_PP_TMP_TMPVAL160,            // 0x331500a0, 
  p_PP_TMP_TMPVAL161,            // 0x331500a1, 
  p_PP_TMP_TMPVAL162,            // 0x331500a2, 
  p_PP_TMP_TMPVAL163,            // 0x331500a3, 
  p_PP_TMP_TMPVAL164,            // 0x331500a4, 
  p_PP_TMP_TMPVAL165,            // 0x331500a5, 
  p_PP_TMP_TMPVAL166,            // 0x331500a6, 
  p_PP_TMP_TMPVAL167,            // 0x331500a7, 
  p_PP_TMP_TMPVAL168,            // 0x331500a8, 
  p_PP_TMP_TMPVAL169,            // 0x331500a9, 
  p_PP_TMP_TMPVAL170,            // 0x331500aa, 
  p_PP_TMP_TMPVAL171,            // 0x331500ab, 
  p_PP_TMP_TMPVAL172,            // 0x331500ac, 
  p_PP_TMP_TMPVAL173,            // 0x331500ad, 
  p_PP_TMP_TMPVAL174,            // 0x331500ae, 
  p_PP_TMP_TMPVAL175,            // 0x331500af, 
  p_PP_TMP_TMPVAL176,            // 0x331500b0, 
  p_PP_TMP_TMPVAL177,            // 0x331500b1, 
  p_PP_TMP_TMPVAL178,            // 0x331500b2, 
  p_PP_TMP_TMPVAL179,            // 0x331500b3, 
  p_PP_TMP_TMPVAL180,            // 0x331500b4, 
  p_PP_TMP_TMPVAL181,            // 0x331500b5, 
  p_PP_TMP_TMPVAL182,            // 0x331500b6, 
  p_PP_TMP_TMPVAL183,            // 0x331500b7, 
  p_PP_TMP_TMPVAL184,            // 0x331500b8, 
  p_PP_TMP_TMPVAL185,            // 0x331500b9, 
  p_PP_TMP_TMPVAL186,            // 0x331500ba, 
  p_PP_TMP_TMPVAL187,            // 0x331500bb, 
  p_PP_TMP_TMPVAL188,            // 0x331500bc, 
  p_PP_TMP_TMPVAL189,            // 0x331500bd, 
  p_PP_TMP_TMPVAL190,            // 0x331500be, 
  p_PP_TMP_TMPVAL191,            // 0x331500bf, 
  p_PP_TMP_TMPVAL192,            // 0x331500c0, 
  p_PP_TMP_TMPVAL193,            // 0x331500c1, 
  p_PP_TMP_TMPVAL194,            // 0x331500c2, 
  p_PP_TMP_TMPVAL195,            // 0x331500c3, 
  p_PP_TMP_TMPVAL196,            // 0x331500c4, 
  p_PP_TMP_TMPVAL197,            // 0x331500c5, 
  p_PP_TMP_TMPVAL198,            // 0x331500c6, 
  p_PP_TMP_TMPVAL199,            // 0x331500c7, 
  p_PP_TMP_TMPVAL200,            // 0x331500c8, 
  p_PP_TMP_TMPVAL201,            // 0x331500c9, 
  p_PP_TMP_TMPVAL202,            // 0x331500ca, 
  p_PP_TMP_TMPVAL203,            // 0x331500cb, 
  p_PP_TMP_TMPVAL204,            // 0x331500cc, 
  p_PP_TMP_TMPVAL205,            // 0x331500cd, 
  p_PP_TMP_TMPVAL206,            // 0x331500ce, 
  p_PP_TMP_TMPVAL207,            // 0x331500cf, 
  p_PP_TMP_TMPVAL208,            // 0x331500d0, 
  p_PP_TMP_TMPVAL209,            // 0x331500d1, 
  p_PP_TMP_TMPVAL210,            // 0x331500d2, 
  p_PP_TMP_TMPVAL211,            // 0x331500d3, 
  p_PP_TMP_TMPVAL212,            // 0x331500d4, 
  p_PP_TMP_TMPVAL213,            // 0x331500d5, 
  p_PP_TMP_TMPVAL214,            // 0x331500d6, 
  p_PP_TMP_TMPVAL215,            // 0x331500d7, 
  p_PP_TMP_TMPVAL216,            // 0x331500d8, 
  p_PP_TMP_TMPVAL217,            // 0x331500d9, 
  p_PP_TMP_TMPVAL218,            // 0x331500da, 
  p_PP_TMP_TMPVAL219,            // 0x331500db, 
  p_PP_TMP_TMPVAL220,            // 0x331500dc, 
  p_PP_TMP_TMPVAL221,            // 0x331500dd, 
  p_PP_TMP_TMPVAL222,            // 0x331500de, 
  p_PP_TMP_TMPVAL223,            // 0x331500df, 
  p_PP_TMP_TMPVAL224,            // 0x331500e0, 
  p_PP_TMP_TMPVAL225,            // 0x331500e1, 
  p_PP_TMP_TMPVAL226,            // 0x331500e2, 
  p_PP_TMP_TMPVAL227,            // 0x331500e3, 
  p_PP_TMP_TMPVAL228,            // 0x331500e4, 
  p_PP_TMP_TMPVAL229,            // 0x331500e5, 
  p_PP_TMP_TMPVAL230,            // 0x331500e6, 
  p_PP_TMP_TMPVAL231,            // 0x331500e7, 
  p_PP_TMP_TMPVAL232,            // 0x331500e8, 
  p_PP_TMP_TMPVAL233,            // 0x331500e9, 
  p_PP_TMP_TMPVAL234,            // 0x331500ea, 
  p_PP_TMP_TMPVAL235,            // 0x331500eb, 
  p_PP_TMP_TMPVAL236,            // 0x331500ec, 
  p_PP_TMP_TMPVAL237,            // 0x331500ed, 
  p_PP_TMP_TMPVAL238,            // 0x331500ee, 
  p_PP_TMP_TMPVAL239,            // 0x331500ef, 
  p_PP_TMP_TMPVAL240,            // 0x331500f0, 
  p_PP_TMP_TMPVAL241,            // 0x331500f1, 
  p_PP_TMP_TMPVAL242,            // 0x331500f2, 
  p_PP_TMP_TMPVAL243,            // 0x331500f3, 
  p_PP_TMP_TMPVAL244,            // 0x331500f4, 
  p_PP_TMP_TMPVAL245,            // 0x331500f5, 
  p_PP_TMP_TMPVAL246,            // 0x331500f6, 
  p_PP_TMP_TMPVAL247,            // 0x331500f7, 
  p_PP_TMP_TMPVAL248,            // 0x331500f8, 
  p_PP_TMP_TMPVAL249,            // 0x331500f9, 
  p_PP_TMP_TMPVAL250,            // 0x331500fa, 
  p_PP_TMP_TMPVAL251,            // 0x331500fb, 
  p_PP_TMP_TMPVAL252,            // 0x331500fc, 
  p_PP_TMP_TMPVAL253,            // 0x331500fd, 
  p_PP_TMP_TMPVAL254,            // 0x331500fe, 
  p_PP_TMP_TMPVAL255,            // 0x331500ff, 
  p_PP_TMP_TMPVAL256,            // 0x33150100, 
  p_PP_TMP_TMPVAL257,            // 0x33150101, 
  p_PP_TMP_TMPVAL258,            // 0x33150102, 
  p_PP_TMP_TMPVAL259,            // 0x33150103, 
  p_PP_TMP_TMPVAL260,            // 0x33150104, 
  p_PP_TMP_TMPVAL261,            // 0x33150105, 
  p_PP_TMP_TMPVAL262,            // 0x33150106, 
  p_PP_TMP_TMPVAL263,            // 0x33150107, 
  p_PP_TMP_TMPVAL264,            // 0x33150108, 
  p_PP_TMP_TMPVAL265,            // 0x33150109, 
  p_PP_TMP_TMPVAL266,            // 0x3315010a, 
  p_PP_TMP_TMPVAL267,            // 0x3315010b, 
  p_PP_TMP_TMPVAL268,            // 0x3315010c, 
  p_PP_TMP_TMPVAL269,            // 0x3315010d, 
  p_PP_TMP_TMPVAL270,            // 0x3315010e, 
  p_PP_TMP_TMPVAL271,            // 0x3315010f, 
  p_PP_TMP_TMPVAL272,            // 0x33150110, 
  p_PP_TMP_TMPVAL273,            // 0x33150111, 
  p_PP_TMP_TMPVAL274,            // 0x33150112, 
  p_PP_TMP_TMPVAL275,            // 0x33150113, 
  p_PP_TMP_TMPVAL276,            // 0x33150114, 
  p_PP_TMP_TMPVAL277,            // 0x33150115, 
  p_PP_TMP_TMPVAL278,            // 0x33150116, 
  p_PP_TMP_TMPVAL279,            // 0x33150117, 
  p_PP_TMP_TMPVAL280,            // 0x33150118, 
  p_PP_TMP_TMPVAL281,            // 0x33150119, 
  p_PP_TMP_TMPVAL282,            // 0x3315011a, 
  p_PP_TMP_TMPVAL283,            // 0x3315011b, 
  p_PP_TMP_TMPVAL284,            // 0x3315011c, 
  p_PP_TMP_TMPVAL285,            // 0x3315011d, 
  p_PP_TMP_TMPVAL286,            // 0x3315011e, 
  p_PP_TMP_TMPVAL287,            // 0x3315011f, 
  p_PP_TMP_TMPVAL288,            // 0x33150120, 
  p_PP_TMP_TMPVAL289,            // 0x33150121, 
  p_PP_TMP_TMPVAL290,            // 0x33150122, 
  p_PP_TMP_TMPVAL291,            // 0x33150123, 
  p_PP_TMP_TMPVAL292,            // 0x33150124, 
  p_PP_TMP_TMPVAL293,            // 0x33150125, 
  p_PP_TMP_TMPVAL294,            // 0x33150126, 
  p_PP_TMP_TMPVAL295,            // 0x33150127, 
  p_PP_TMP_TMPVAL296,            // 0x33150128, 
  p_PP_TMP_TMPVAL297,            // 0x33150129, 
  p_PP_TMP_TMPVAL298,            // 0x3315012a, 
  p_PP_TMP_TMPVAL299,            // 0x3315012b, 
  p_PP_TMP_TMPVAL300,            // 0x3315012c, 
  p_PP_TMP_TMPVAL301,            // 0x3315012d, 
  p_PP_TMP_TMPVAL302,            // 0x3315012e, 
  p_PP_TMP_TMPVAL303,            // 0x3315012f, 
  p_PP_TMP_TMPVAL304,            // 0x33150130, 
  p_PP_TMP_TMPVAL305,            // 0x33150131, 
  p_PP_TMP_TMPVAL306,            // 0x33150132, 
  p_PP_TMP_TMPVAL307,            // 0x33150133, 
  p_PP_TMP_TMPVAL308,            // 0x33150134, 
  p_PP_TMP_TMPVAL309,            // 0x33150135, 
  p_PP_TMP_TMPVAL310,            // 0x33150136, 
  p_PP_TMP_TMPVAL311,            // 0x33150137, 
  p_PP_TMP_TMPVAL312,            // 0x33150138, 
  p_PP_TMP_TMPVAL313,            // 0x33150139, 
  p_PP_TMP_TMPVAL314,            // 0x3315013a, 
  p_PP_TMP_TMPVAL315,            // 0x3315013b, 
  p_PP_TMP_TMPVAL316,            // 0x3315013c, 
  p_PP_TMP_TMPVAL317,            // 0x3315013d, 
  p_PP_TMP_TMPVAL318,            // 0x3315013e, 
  p_PP_TMP_TMPVAL319,            // 0x3315013f, 
  p_PP_TMP_TMPVAL320,            // 0x33150140, 
  p_PP_TMP_TMPVAL321,            // 0x33150141, 
  p_PP_TMP_TMPVAL322,            // 0x33150142, 
  p_PP_TMP_TMPVAL323,            // 0x33150143, 
  p_PP_TMP_TMPVAL324,            // 0x33150144, 
  p_PP_TMP_TMPVAL325,            // 0x33150145, 
  p_PP_TMP_TMPVAL326,            // 0x33150146, 
  p_PP_TMP_TMPVAL327,            // 0x33150147, 
  p_PP_TMP_TMPVAL328,            // 0x33150148, 
  p_PP_TMP_TMPVAL329,            // 0x33150149, 
  p_PP_TMP_TMPVAL330,            // 0x3315014a, 
  p_PP_TMP_TMPVAL331,            // 0x3315014b, 
  p_PP_TMP_TMPVAL332,            // 0x3315014c, 
  p_PP_TMP_TMPVAL333,            // 0x3315014d, 
  p_PP_TMP_TMPVAL334,            // 0x3315014e, 
  p_PP_TMP_TMPVAL335,            // 0x3315014f, 
  p_PP_TMP_TMPVAL336,            // 0x33150150, 
  p_PP_TMP_TMPVAL337,            // 0x33150151, 
  p_PP_TMP_TMPVAL338,            // 0x33150152, 
  p_PP_TMP_TMPVAL339,            // 0x33150153, 
  p_PP_TMP_TMPVAL340,            // 0x33150154, 
  p_PP_TMP_TMPVAL341,            // 0x33150155, 
  p_PP_TMP_TMPVAL342,            // 0x33150156, 
  p_PP_TMP_TMPVAL343,            // 0x33150157, 
  p_PP_TMP_TMPVAL344,            // 0x33150158, 
  p_PP_TMP_TMPVAL345,            // 0x33150159, 
  p_PP_TMP_TMPVAL346,            // 0x3315015a, 
  p_PP_TMP_TMPVAL347,            // 0x3315015b, 
  p_PP_TMP_TMPVAL348,            // 0x3315015c, 
  p_PP_TMP_TMPVAL349,            // 0x3315015d, 
  p_PP_TMP_TMPVAL350,            // 0x3315015e, 
  p_PP_TMP_TMPVAL351,            // 0x3315015f, 
  p_PP_TMP_TMPVAL352,            // 0x33150160, 
  p_PP_TMP_TMPVAL353,            // 0x33150161, 
  p_PP_TMP_TMPVAL354,            // 0x33150162, 
  p_PP_TMP_TMPVAL355,            // 0x33150163, 
  p_PP_TMP_TMPVAL356,            // 0x33150164, 
  p_PP_TMP_TMPVAL357,            // 0x33150165, 
  p_PP_TMP_TMPVAL358,            // 0x33150166, 
  p_PP_TMP_TMPVAL359,            // 0x33150167, 
  p_PP_TMP_TMPVAL360,            // 0x33150168, 
  p_PP_TMP_TMPVAL361,            // 0x33150169, 
  p_PP_TMP_TMPVAL362,            // 0x3315016a, 
  p_PP_TMP_TMPVAL363,            // 0x3315016b, 
  p_PP_TMP_TMPVAL364,            // 0x3315016c, 
  p_PP_TMP_TMPVAL365,            // 0x3315016d, 
  p_PP_TMP_TMPVAL366,            // 0x3315016e, 
  p_PP_TMP_TMPVAL367,            // 0x3315016f, 
  p_PP_TMP_TMPVAL368,            // 0x33150170, 
  p_PP_TMP_TMPVAL369,            // 0x33150171, 
  p_PP_TMP_TMPVAL370,            // 0x33150172, 
  p_PP_TMP_TMPVAL371,            // 0x33150173, 
  p_PP_TMP_TMPVAL372,            // 0x33150174, 
  p_PP_TMP_TMPVAL373,            // 0x33150175, 
  p_PP_TMP_TMPVAL374,            // 0x33150176, 
  p_PP_TMP_TMPVAL375,            // 0x33150177, 
  p_PP_TMP_TMPVAL376,            // 0x33150178, 
  p_PP_TMP_TMPVAL377,            // 0x33150179, 
  p_PP_TMP_TMPVAL378,            // 0x3315017a, 
  p_PP_TMP_TMPVAL379,            // 0x3315017b, 
  p_PP_TMP_TMPVAL380,            // 0x3315017c, 
  p_PP_TMP_TMPVAL381,            // 0x3315017d, 
  p_PP_TMP_TMPVAL382,            // 0x3315017e, 
  p_PP_TMP_TMPVAL383,            // 0x3315017f, 
  p_PP_TMP_TMPVAL384,            // 0x33150180, 
  p_PP_TMP_TMPVAL385,            // 0x33150181, 
  p_PP_TMP_TMPVAL386,            // 0x33150182, 
  p_PP_TMP_TMPVAL387,            // 0x33150183, 
  p_PP_TMP_TMPVAL388,            // 0x33150184, 
  p_PP_TMP_TMPVAL389,            // 0x33150185, 
  p_PP_TMP_TMPVAL390,            // 0x33150186, 
  p_PP_TMP_TMPVAL391,            // 0x33150187, 
  p_PP_TMP_TMPVAL392,            // 0x33150188, 
  p_PP_TMP_TMPVAL393,            // 0x33150189, 
  p_PP_TMP_TMPVAL394,            // 0x3315018a, 
  p_PP_TMP_TMPVAL395,            // 0x3315018b, 
  p_PP_TMP_TMPVAL396,            // 0x3315018c, 
  p_PP_TMP_TMPVAL397,            // 0x3315018d, 
  p_PP_TMP_TMPVAL398,            // 0x3315018e, 
  p_PP_TMP_TMPVAL399,            // 0x3315018f, 
  p_PP_TMP_TMPVAL400,            // 0x33150190, 
  p_PP_TMP_TMPVAL401,            // 0x33150191, 
  p_PP_TMP_TMPVAL402,            // 0x33150192, 
  p_PP_TMP_TMPVAL403,            // 0x33150193, 
  p_PP_TMP_TMPVAL404,            // 0x33150194, 
  p_PP_TMP_TMPVAL405,            // 0x33150195, 
  p_PP_TMP_TMPVAL406,            // 0x33150196, 
  p_PP_TMP_TMPVAL407,            // 0x33150197, 
  p_PP_TMP_TMPVAL408,            // 0x33150198, 
  p_PP_TMP_TMPVAL409,            // 0x33150199, 
  p_PP_TMP_TMPVAL410,            // 0x3315019a, 
  p_PP_TMP_TMPVAL411,            // 0x3315019b, 
  p_PP_TMP_TMPVAL412,            // 0x3315019c, 
  p_PP_TMP_TMPVAL413,            // 0x3315019d, 
  p_PP_TMP_TMPVAL414,            // 0x3315019e, 
  p_PP_TMP_TMPVAL415,            // 0x3315019f, 
  p_PP_TMP_TMPVAL416,            // 0x331501a0, 
  p_PP_TMP_TMPVAL417,            // 0x331501a1, 
  p_PP_TMP_TMPVAL418,            // 0x331501a2, 
  p_PP_TMP_TMPVAL419,            // 0x331501a3, 
  p_PP_TMP_TMPVAL420,            // 0x331501a4, 
  p_PP_TMP_TMPVAL421,            // 0x331501a5, 
  p_PP_TMP_TMPVAL422,            // 0x331501a6, 
  p_PP_TMP_TMPVAL423,            // 0x331501a7, 
  p_PP_TMP_TMPVAL424,            // 0x331501a8, 
  p_PP_TMP_TMPVAL425,            // 0x331501a9, 
  p_PP_TMP_TMPVAL426,            // 0x331501aa, 
  p_PP_TMP_TMPVAL427,            // 0x331501ab, 
  p_PP_TMP_TMPVAL428,            // 0x331501ac, 
  p_PP_TMP_TMPVAL429,            // 0x331501ad, 
  p_PP_TMP_TMPVAL430,            // 0x331501ae, 
  p_PP_TMP_TMPVAL431,            // 0x331501af, 
  p_PP_TMP_TMPVAL432,            // 0x331501b0, 
  p_PP_TMP_TMPVAL433,            // 0x331501b1, 
  p_PP_TMP_TMPVAL434,            // 0x331501b2, 
  p_PP_TMP_TMPVAL435,            // 0x331501b3, 
  p_PP_TMP_TMPVAL436,            // 0x331501b4, 
  p_PP_TMP_TMPVAL437,            // 0x331501b5, 
  p_PP_TMP_TMPVAL438,            // 0x331501b6, 
  p_PP_TMP_TMPVAL439,            // 0x331501b7, 
  p_PP_TMP_TMPVAL440,            // 0x331501b8, 
  p_PP_TMP_TMPVAL441,            // 0x331501b9, 
  p_PP_TMP_TMPVAL442,            // 0x331501ba, 
  p_PP_TMP_TMPVAL443,            // 0x331501bb, 
  p_PP_TMP_TMPVAL444,            // 0x331501bc, 
  p_PP_TMP_TMPVAL445,            // 0x331501bd, 
  p_PP_TMP_TMPVAL446,            // 0x331501be, 
  p_PP_TMP_TMPVAL447,            // 0x331501bf, 
  p_PP_TMP_TMPVAL448,            // 0x331501c0, 
  p_PP_TMP_TMPVAL449,            // 0x331501c1, 
  p_PP_TMP_TMPVAL450,            // 0x331501c2, 
  p_PP_TMP_TMPVAL451,            // 0x331501c3, 
  p_PP_TMP_TMPVAL452,            // 0x331501c4, 
  p_PP_TMP_TMPVAL453,            // 0x331501c5, 
  p_PP_TMP_TMPVAL454,            // 0x331501c6, 
  p_PP_TMP_TMPVAL455,            // 0x331501c7, 
  p_PP_TMP_TMPVAL456,            // 0x331501c8, 
  p_PP_TMP_TMPVAL457,            // 0x331501c9, 
  p_PP_TMP_TMPVAL458,            // 0x331501ca, 
  p_PP_TMP_TMPVAL459,            // 0x331501cb, 
  p_PP_TMP_TMPVAL460,            // 0x331501cc, 
  p_PP_TMP_TMPVAL461,            // 0x331501cd, 
  p_PP_TMP_TMPVAL462,            // 0x331501ce, 
  p_PP_TMP_TMPVAL463,            // 0x331501cf, 
  p_PP_TMP_TMPVAL464,            // 0x331501d0, 
  p_PP_TMP_TMPVAL465,            // 0x331501d1, 
  p_PP_TMP_TMPVAL466,            // 0x331501d2, 
  p_PP_TMP_TMPVAL467,            // 0x331501d3, 
  p_PP_TMP_TMPVAL468,            // 0x331501d4, 
  p_PP_TMP_TMPVAL469,            // 0x331501d5, 
  p_PP_TMP_TMPVAL470,            // 0x331501d6, 
  p_PP_TMP_TMPVAL471,            // 0x331501d7, 
  p_PP_TMP_TMPVAL472,            // 0x331501d8, 
  p_PP_TMP_TMPVAL473,            // 0x331501d9, 
  p_PP_TMP_TMPVAL474,            // 0x331501da, 
  p_PP_TMP_TMPVAL475,            // 0x331501db, 
  p_PP_TMP_TMPVAL476,            // 0x331501dc, 
  p_PP_TMP_TMPVAL477,            // 0x331501dd, 
  p_PP_TMP_TMPVAL478,            // 0x331501de, 
  p_PP_TMP_TMPVAL479,            // 0x331501df, 
  p_PP_TMP_TMPVAL480,            // 0x331501e0, 
  p_PP_TMP_TMPVAL481,            // 0x331501e1, 
  p_PP_TMP_TMPVAL482,            // 0x331501e2, 
  p_PP_TMP_TMPVAL483,            // 0x331501e3, 
  p_PP_TMP_TMPVAL484,            // 0x331501e4, 
  p_PP_TMP_TMPVAL485,            // 0x331501e5, 
  p_PP_TMP_TMPVAL486,            // 0x331501e6, 
  p_PP_TMP_TMPVAL487,            // 0x331501e7, 
  p_PP_TMP_TMPVAL488,            // 0x331501e8, 
  p_PP_TMP_TMPVAL489,            // 0x331501e9, 
  p_PP_TMP_TMPVAL490,            // 0x331501ea, 
  p_PP_TMP_TMPVAL491,            // 0x331501eb, 
  p_PP_TMP_TMPVAL492,            // 0x331501ec, 
  p_PP_TMP_TMPVAL493,            // 0x331501ed, 
  p_PP_TMP_TMPVAL494,            // 0x331501ee, 
  p_PP_TMP_TMPVAL495,            // 0x331501ef, 
  p_PP_TMP_TMPVAL496,            // 0x331501f0, 
  p_PP_TMP_TMPVAL497,            // 0x331501f1, 
  p_PP_TMP_TMPVAL498,            // 0x331501f2, 
  p_PP_TMP_TMPVAL499,            // 0x331501f3, 
  p_PP_TMP_TMPSTR0,              // 0x331501f4, 
  p_PP_TMP_TMPSTR1,              // 0x331501f5, 
  p_PP_TMP_TMPSTR2,              // 0x331501f6, 
  p_PP_TMP_TMPSTR3,              // 0x331501f7, 
  p_PP_TMP_TMPSTR4,              // 0x331501f8, 
  p_PP_TMP_TMPSTR5,              // 0x331501f9, 
  p_PP_TMP_TMPSTR6,              // 0x331501fa, 
  p_PP_TMP_TMPSTR7,              // 0x331501fb, 
  p_PP_TMP_TMPSTR8,              // 0x331501fc, 
  p_PP_TMP_TMPSTR9,              // 0x331501fd, 
  p_PP_TMP_TMPSTR10,             // 0x331501fe, 
  p_PP_TMP_TMPSTR11,             // 0x331501ff, 
  p_PP_TMP_TMPSTR12,             // 0x33150200, 
  p_PP_TMP_TMPSTR13,             // 0x33150201, 
  p_PP_TMP_TMPSTR14,             // 0x33150202, 
  p_PP_TMP_TMPSTR15,             // 0x33150203, 
  p_PP_TMP_TMPSTR16,             // 0x33150204, 
  p_PP_TMP_TMPSTR17,             // 0x33150205, 
  p_PP_TMP_TMPSTR18,             // 0x33150206, 
  p_PP_TMP_TMPSTR19,             // 0x33150207, 
  p_PP_TMP_TMPSTR20,             // 0x33150208, 
  p_PP_TMP_TMPSTR21,             // 0x33150209, 
  p_PP_TMP_TMPSTR22,             // 0x3315020a, 
  p_PP_TMP_TMPSTR23,             // 0x3315020b, 
  p_PP_TMP_TMPSTR24,             // 0x3315020c, 
  p_PP_TMP_TMPSTR25,             // 0x3315020d, 
  p_PP_TMP_TMPSTR26,             // 0x3315020e, 
  p_PP_TMP_TMPSTR27,             // 0x3315020f, 
  p_PP_TMP_TMPSTR28,             // 0x33150210, 
  p_PP_TMP_TMPSTR29,             // 0x33150211, 
  p_PP_TMP_TMPSTR30,             // 0x33150212, 
  p_PP_TMP_TMPSTR31,             // 0x33150213, 
  p_PP_TMP_TMPSTR32,             // 0x33150214, 
  p_PP_TMP_TMPSTR33,             // 0x33150215, 
  p_PP_TMP_TMPSTR34,             // 0x33150216, 
  p_PP_TMP_TMPSTR35,             // 0x33150217, 
  p_PP_TMP_TMPSTR36,             // 0x33150218, 
  p_PP_TMP_TMPSTR37,             // 0x33150219, 
  p_PP_TMP_TMPSTR38,             // 0x3315021a, 
  p_PP_TMP_TMPSTR39,             // 0x3315021b, 
  p_PP_TMP_TMPSTR40,             // 0x3315021c, 
  p_PP_TMP_TMPSTR41,             // 0x3315021d, 
  p_PP_TMP_TMPSTR42,             // 0x3315021e, 
  p_PP_TMP_TMPSTR43,             // 0x3315021f, 
  p_PP_TMP_TMPSTR44,             // 0x33150220, 
  p_PP_TMP_TMPSTR45,             // 0x33150221, 
  p_PP_TMP_TMPSTR46,             // 0x33150222, 
  p_PP_TMP_TMPSTR47,             // 0x33150223, 
  p_PP_TMP_TMPSTR48,             // 0x33150224, 
  p_PP_TMP_TMPSTR49,             // 0x33150225, 
  p_PP_TMP_TMPSTR50,             // 0x33150226, 
  p_PP_TMP_TMPSTR51,             // 0x33150227, 
  p_PP_TMP_TMPSTR52,             // 0x33150228, 
  p_PP_TMP_TMPSTR53,             // 0x33150229, 
  p_PP_TMP_TMPSTR54,             // 0x3315022a, 
  p_PP_TMP_TMPSTR55,             // 0x3315022b, 
  p_PP_TMP_TMPSTR56,             // 0x3315022c, 
  p_PP_TMP_TMPSTR57,             // 0x3315022d, 
  p_PP_TMP_TMPSTR58,             // 0x3315022e, 
  p_PP_TMP_TMPSTR59,             // 0x3315022f, 
  p_PP_TMP_TMPSTR60,             // 0x33150230, 
  p_PP_TMP_TMPSTR61,             // 0x33150231, 
  p_PP_TMP_TMPSTR62,             // 0x33150232, 
  p_PP_TMP_TMPSTR63,             // 0x33150233, 
  p_PP_TMP_TMPSTR64,             // 0x33150234, 
  p_PP_TMP_TMPSTR65,             // 0x33150235, 
  p_PP_TMP_TMPSTR66,             // 0x33150236, 
  p_PP_TMP_TMPSTR67,             // 0x33150237, 
  p_PP_TMP_TMPSTR68,             // 0x33150238, 
  p_PP_TMP_TMPSTR69,             // 0x33150239, 
  p_PP_TMP_TMPSTR70,             // 0x3315023a, 
  p_PP_TMP_TMPSTR71,             // 0x3315023b, 
  p_PP_TMP_TMPSTR72,             // 0x3315023c, 
  p_PP_TMP_TMPSTR73,             // 0x3315023d, 
  p_PP_TMP_TMPSTR74,             // 0x3315023e, 
  p_PP_TMP_TMPSTR75,             // 0x3315023f, 
  p_PP_TMP_TMPSTR76,             // 0x33150240, 
  p_PP_TMP_TMPSTR77,             // 0x33150241, 
  p_PP_TMP_TMPSTR78,             // 0x33150242, 
  p_PP_TMP_TMPSTR79,             // 0x33150243, 
  p_PP_TMP_TMPSTR80,             // 0x33150244, 
  p_PP_TMP_TMPSTR81,             // 0x33150245, 
  p_PP_TMP_TMPSTR82,             // 0x33150246, 
  p_PP_TMP_TMPSTR83,             // 0x33150247, 
  p_PP_TMP_TMPSTR84,             // 0x33150248, 
  p_PP_TMP_TMPSTR85,             // 0x33150249, 
  p_PP_TMP_TMPSTR86,             // 0x3315024a, 
  p_PP_TMP_TMPSTR87,             // 0x3315024b, 
  p_PP_TMP_TMPSTR88,             // 0x3315024c, 
  p_PP_TMP_TMPSTR89,             // 0x3315024d, 
  p_PP_TMP_TMPSTR90,             // 0x3315024e, 
  p_PP_TMP_TMPSTR91,             // 0x3315024f, 
  p_PP_TMP_TMPSTR92,             // 0x33150250, 
  p_PP_TMP_TMPSTR93,             // 0x33150251, 
  p_PP_TMP_TMPSTR94,             // 0x33150252, 
  p_PP_TMP_TMPSTR95,             // 0x33150253, 
  p_PP_TMP_TMPSTR96,             // 0x33150254, 
  p_PP_TMP_TMPSTR97,             // 0x33150255, 
  p_PP_TMP_TMPSTR98,             // 0x33150256, 
  p_PP_TMP_TMPSTR99,             // 0x33150257, 
  p_PP_TMP_TMPSTR100,            // 0x33150258, 
  p_PP_TMP_TMPSTR101,            // 0x33150259, 
  p_PP_TMP_TMPSTR102,            // 0x3315025a, 
  p_PP_TMP_TMPSTR103,            // 0x3315025b, 
  p_PP_TMP_TMPSTR104,            // 0x3315025c, 
  p_PP_TMP_TMPSTR105,            // 0x3315025d, 
  p_PP_TMP_TMPSTR106,            // 0x3315025e, 
  p_PP_TMP_TMPSTR107,            // 0x3315025f, 
  p_PP_TMP_TMPSTR108,            // 0x33150260, 
  p_PP_TMP_TMPSTR109,            // 0x33150261, 
  p_PP_TMP_TMPSTR110,            // 0x33150262, 
  p_PP_TMP_TMPSTR111,            // 0x33150263, 
  p_PP_TMP_TMPSTR112,            // 0x33150264, 
  p_PP_TMP_TMPSTR113,            // 0x33150265, 
  p_PP_TMP_TMPSTR114,            // 0x33150266, 
  p_PP_TMP_TMPSTR115,            // 0x33150267, 
  p_PP_TMP_TMPSTR116,            // 0x33150268, 
  p_PP_TMP_TMPSTR117,            // 0x33150269, 
  p_PP_TMP_TMPSTR118,            // 0x3315026a, 
  p_PP_TMP_TMPSTR119,            // 0x3315026b, 
  p_PP_TMP_TMPSTR120,            // 0x3315026c, 
  p_PP_TMP_TMPSTR121,            // 0x3315026d, 
  p_PP_TMP_TMPSTR122,            // 0x3315026e, 
  p_PP_TMP_TMPSTR123,            // 0x3315026f, 
  p_PP_TMP_TMPSTR124,            // 0x33150270, 
  p_PP_TMP_TMPSTR125,            // 0x33150271, 
  p_PP_TMP_TMPSTR126,            // 0x33150272, 
  p_PP_TMP_TMPSTR127,            // 0x33150273, 
  p_PP_TMP_TMPSTR128,            // 0x33150274, 
  p_PP_TMP_TMPSTR129,            // 0x33150275, 
  p_PP_TMP_TMPSTR130,            // 0x33150276, 
  p_PP_TMP_TMPSTR131,            // 0x33150277, 
  p_PP_TMP_TMPSTR132,            // 0x33150278, 
  p_PP_TMP_TMPSTR133,            // 0x33150279, 
  p_PP_TMP_TMPSTR134,            // 0x3315027a, 
  p_PP_TMP_TMPSTR135,            // 0x3315027b, 
  p_PP_TMP_TMPSTR136,            // 0x3315027c, 
  p_PP_TMP_TMPSTR137,            // 0x3315027d, 
  p_PP_TMP_TMPSTR138,            // 0x3315027e, 
  p_PP_TMP_TMPSTR139,            // 0x3315027f, 
  p_PP_TMP_TMPSTR140,            // 0x33150280, 
  p_PP_TMP_TMPSTR141,            // 0x33150281, 
  p_PP_TMP_TMPSTR142,            // 0x33150282, 
  p_PP_TMP_TMPSTR143,            // 0x33150283, 
  p_PP_TMP_TMPSTR144,            // 0x33150284, 
  p_PP_TMP_TMPSTR145,            // 0x33150285, 
  p_PP_TMP_TMPSTR146,            // 0x33150286, 
  p_PP_TMP_TMPSTR147,            // 0x33150287, 
  p_PP_TMP_TMPSTR148,            // 0x33150288, 
  p_PP_TMP_TMPSTR149             // 0x33150289, 
};

enum db_PP_NET{
  p_PP_NET_MOD_ST = 0x33180000, // 0x33180000, 模块状态
  p_PP_NET_MOD_ID,              // 0x33180001, 模块ID
  p_PP_NET_MOD_SIGNAL,          // 0x33180002, 模块信号强度
  p_PP_NET_MOD_CNT,             // 0x33180003, 模块指令数
  p_PP_NET_MOD_VER,             // 0x33180004, 模块版本
  p_PP_NET_MOD_MSG,             // 0x33180005, 模块自定义数据
  p_PP_NET_ABOXID,              // 0x33180006, ABOX指定id
  p_PP_NET_ABOXPASSWD,          // 0x33180007, 指定ABOX的密码
  p_PP_NET_REMOTEPASSWD,        // 0x33180008, 远程登入密码
  p_PP_NET_REMOTE_ST,           // 0x33180009, 远程登入状态
  p_PP_NET_CFG_START,           // 0x3318000a, 配置起始位置
  p_PP_NET_CFG_NET,             // 0x3318000b, 网络功能开关
  p_PP_NET_CFG_IOT,             // 0x3318000c, iLink配置开关
  p_PP_NET_CFG_MODBUS,          // 0x3318000d, MODBUS配置开关
  p_PP_NET_CFG_QRSN,            // 0x3318000e, 序列号
  p_PP_NET_CFG_QRVCODE,         // 0x3318000f, 校验码
  p_PP_NET_CFG_REGISTER,        // 0x33180010, 注册密码
  p_PP_NET_CFG_USERLOGIN,       // 0x33180011, 用户登入功能更
  p_PP_NET_CFG_USERNO,          // 0x33180012, 用户编号
  p_PP_NET_CFG_USERLT,          // 0x33180013, 用户登入时间
  p_PP_NET_CFG_PLAN,            // 0x33180014, 派单功能
  p_PP_NET_CFG_MACHTYPE,        // 0x33180015, 联网机型 0：普通机，1：全电3：油压机
  p_PP_NET_CFG_ADDUPSHOTCNT,    // 0x33180016, 开模数累计
  p_PP_NET_CFG_SHOTCNT,         // 0x33180017, 开模数
  p_PP_NET_CFG_OP,              // 0x33180018, 操作状态
  p_PP_NET_CFG_TOTALTIME,       // 0x33180019, 开机累计时间
  p_PP_NET_CFG_FACTORY,         // 0x3318001a, 厂商代码
  p_PP_NET_CFG_END,             // 0x3318001b, 配置结束位置
  p_PP_NET_PS_START,            // 0x3318001c, 分单起始，以下地址的名字取名与json一致
  p_PP_NET_partId,              // 0x3318001d, 分单号
  p_PP_NET_productAllId,        // 0x3318001e, 总单号
  p_PP_NET_devId,               // 0x3318001f, 机器ID
  p_PP_NET_tecId,               // 0x33180020, 工艺ID
  p_PP_NET_tecSn,               // 0x33180021, 工艺序号
  p_PP_NET_tecName,             // 0x33180022, 工艺名
  p_PP_NET_mouldName,           // 0x33180023, 模具名
  p_PP_NET_material,            // 0x33180024, 材料
  p_PP_NET_color,               // 0x33180025, 颜色
  p_PP_NET_state,               // 0x33180026, 状态
  p_PP_NET_countPerMold,        // 0x33180027, 每模产品数
  p_PP_NET_productCount,        // 0x33180028, 实际规划产量
  p_PP_NET_totalOKCount,        // 0x33180029, 计划产量
  p_PP_NET_curOKCount,          // 0x3318002a, 当前开模数
  p_PP_NET_realSTime,           // 0x3318002b, 实际开始时间
  p_PP_NET_realETime,           // 0x3318002c, 实际结束时间
  p_PP_NET_sTime,               // 0x3318002d, 计划开始时间
  p_PP_NET_eTime,               // 0x3318002e, 计划结束时间
  p_PP_NET_PS_END,              // 0x3318002f, 分单结束
  p_PP_NET_SUP_START,           // 0x33180030, 补登开始
  p_PP_NET_ERRCOUNT,            // 0x33180031, 次品总个数
  p_PP_NET_ERR1,                // 0x33180032, 次品原因1
  p_PP_NET_ERR2,                // 0x33180033, 次品原因2
  p_PP_NET_ERR3,                // 0x33180034, 次品原因3
  p_PP_NET_ERR4,                // 0x33180035, 次品原因4
  p_PP_NET_ERR5,                // 0x33180036, 次品原因5
  p_PP_NET_ERR6,                // 0x33180037, 次品原因6
  p_PP_NET_ERR7,                // 0x33180038, 次品原因7
  p_PP_NET_ERR8,                // 0x33180039, 次品原因8
  p_PP_NET_ERR9,                // 0x3318003a, 次品原因9
  p_PP_NET_ERR10,               // 0x3318003b, 次品原因10
  p_PP_NET_ERR11,               // 0x3318003c, 次品原因11
  p_PP_NET_ERR12,               // 0x3318003d, 次品原因12
  p_PP_NET_ERR1CNT,             // 0x3318003e, 次品原因1个数
  p_PP_NET_ERR2CNT,             // 0x3318003f, 次品原因2个数
  p_PP_NET_ERR3CNT,             // 0x33180040, 次品原因3个数
  p_PP_NET_ERR4CNT,             // 0x33180041, 次品原因4个数
  p_PP_NET_ERR5CNT,             // 0x33180042, 次品原因5个数
  p_PP_NET_ERR6CNT,             // 0x33180043, 次品原因6个数
  p_PP_NET_ERR7CNT,             // 0x33180044, 次品原因7个数
  p_PP_NET_ERR8CNT,             // 0x33180045, 次品原因8个数
  p_PP_NET_ERR9CNT,             // 0x33180046, 次品原因9个数
  p_PP_NET_ERR10CNT,            // 0x33180047, 次品原因10个数
  p_PP_NET_ERR11CNT,            // 0x33180048, 次品原因11个数
  p_PP_NET_ERR12CNT,            // 0x33180049, 次品原因12个数
  p_PP_NET_ERRVIS1,             // 0x3318004a, 次品原因1是否可见
  p_PP_NET_ERRVIS2,             // 0x3318004b, 次品原因2是否可见
  p_PP_NET_ERRVIS3,             // 0x3318004c, 次品原因3是否可见
  p_PP_NET_ERRVIS4,             // 0x3318004d, 次品原因4是否可见
  p_PP_NET_ERRVIS5,             // 0x3318004e, 次品原因5是否可见
  p_PP_NET_ERRVIS6,             // 0x3318004f, 次品原因6是否可见
  p_PP_NET_ERRVIS7,             // 0x33180050, 次品原因7是否可见
  p_PP_NET_ERRVIS8,             // 0x33180051, 次品原因8是否可见
  p_PP_NET_ERRVIS9,             // 0x33180052, 次品原因9是否可见
  p_PP_NET_ERRVIS10,            // 0x33180053, 次品原因10是否可见
  p_PP_NET_ERRVIS11,            // 0x33180054, 次品原因11是否可见
  p_PP_NET_ERRVIS12,            // 0x33180055, 次品原因12是否可见
  p_PP_NET_SUP_END,             // 0x33180056, 补登结束
  p_PP_NET_HISNAME1,            // 0x33180057, 历史派单1
  p_PP_NET_HISNAME2,            // 0x33180058, 历史派单2
  p_PP_NET_HISNAME3,            // 0x33180059, 历史派单3
  p_PP_NET_HISNAME4,            // 0x3318005a, 历史派单4
  p_PP_NET_HISNAME5,            // 0x3318005b, 历史派单5
  p_PP_NET_HISSTATE1,           // 0x3318005c, 历史派单状态1
  p_PP_NET_HISSTATE2,           // 0x3318005d, 历史派单状态2
  p_PP_NET_HISSTATE3,           // 0x3318005e, 历史派单状态3
  p_PP_NET_HISSTATE4,           // 0x3318005f, 历史派单状态4
  p_PP_NET_HISSTATE5,           // 0x33180060, 历史派单状态5
  p_PP_NET_HISCUR1,             // 0x33180061, 历史派单当前值1
  p_PP_NET_HISCUR2,             // 0x33180062, 历史派单当前值2
  p_PP_NET_HISCUR3,             // 0x33180063, 历史派单当前值3
  p_PP_NET_HISCUR4,             // 0x33180064, 历史派单当前值4
  p_PP_NET_HISCUR5,             // 0x33180065, 历史派单当前值5
  p_PP_NET_HISTOTAL1,           // 0x33180066, 历史派单总计划产量1
  p_PP_NET_HISTOTAL2,           // 0x33180067, 历史派单总计划产量2
  p_PP_NET_HISTOTAL3,           // 0x33180068, 历史派单总计划产量3
  p_PP_NET_HISTOTAL4,           // 0x33180069, 历史派单总计划产量4
  p_PP_NET_HISTOTAL5,           // 0x3318006a, 历史派单总计划产量5
  p_PP_NET_LOGINST,             // 0x3318006b, 登入状态
  p_PP_NET_USERLOGINNAME        // 0x3318006c, 用户登入名
};

enum db_PP_OBJ{
  p_PP_OBJ_BLAST_KEY_STR_VAL = 0x33140000, // 0x33140000, 1
  p_PP_OBJ_AIR1_POS_ENABLE,                // 0x33140001, 吹气1结束位置
  p_PP_OBJ_AIR2_POS_ENABLE,                // 0x33140002, 吹气2结束位置
  p_PP_OBJ_AIR3_POS_ENABLE,                // 0x33140003, 吹气3结束位置
  p_PP_OBJ_AIR4_POS_ENABLE,                // 0x33140004, 吹气4结束位置
  p_PP_OBJ_AIR5_POS_ENABLE,                // 0x33140005, 吹气5结束位置
  p_PP_OBJ_AIR6_POS_ENABLE,                // 0x33140006, 吹气6结束位置
  p_PP_OBJ_AIR1_POS_MAX,                   // 0x33140007, 吹气1结束位置最大值
  p_PP_OBJ_AIR2_POS_MAX,                   // 0x33140008, 吹气2结束位置最大值
  p_PP_OBJ_AIR3_POS_MAX,                   // 0x33140009, 吹气3结束位置最大值
  p_PP_OBJ_AIR4_POS_MAX,                   // 0x3314000a, 吹气4结束位置最大值
  p_PP_OBJ_AIR5_POS_MAX,                   // 0x3314000b, 吹气5结束位置最大值
  p_PP_OBJ_AIR6_POS_MAX,                   // 0x3314000c, 吹气6结束位置最大值
  p_PP_OBJ_BEF_DLY_MAX,                    // 0x3314000d, 托膜前延迟最大值
  p_PP_OBJ_BEF_DLY_DOT,                    // 0x3314000e, 托膜前延迟小数位
  p_PP_OBJ_d,                              // 0x3314000f, delete托膜前延迟字符数
  p_PP_OBJ_EJ_COUNT_ENABLE,                // 0x33140010, 托膜次数是否使用
  p_PP_OBJ_VIBRATE_MOD_ENABLE,             // 0x33140011, 托膜震动模式是否使用
  p_PP_OBJ_VIBRATE_TM_ENABLE,              // 0x33140012, 托膜震动时间/距离是否使用
  p_PP_OBJ_FST_TRAVEL_ENABLE,              // 0x33140013, 首次独立行程是否使用
  p_PP_OBJ_VIBRATE_TM_VISABLE,             // 0x33140014, 托膜震动时间是否可见
  p_PP_OBJ_VIBRATE_DIST_VISABLE,           // 0x33140015, 托膜震动距离是否可见
  p_PP_OBJ_VIBRATE_TorD_DOT,               // 0x33140016, 震动托膜时间/位置小数位
  p_PP_OBJ_VIBRATE_TorD_MIN,               // 0x33140017, 震动托膜时间/位置最小值
  p_PP_OBJ_VIBRATE_TorD_MAX,               // 0x33140018, 震动托膜时间/位置最大值
  p_PP_OBJ_VIBRATE_TorD_UNIT,              // 0x33140019, 震动托膜时间/位置单位切换
  p_PP_OBJ_dd,                             // 0x3314001a, delete震动托膜时间/位置地址切换
  p_PP_OBJ_EJ_POS_IMG_VAL,                 // 0x3314001b, 托膜位置图片
  p_PP_OBJ_EJ_FWD_PO_IMG_VAL,              // 0x3314001c, 托进阀图片
  p_PP_OBJ_EJ_BWD_PO_IMG_VAL,              // 0x3314001d, 托退阀图片
  p_PP_OBJ_EJ_FWD_PI_IMG_VAL,              // 0x3314001e, 托膜进终图片
  p_PP_OBJ_EJ_BWD_PI_IMG_VAL,              // 0x3314001f, 托膜退终图片
  p_PP_OBJ_CLS_END_PI_IMG_VAL,             // 0x33140020, 合模到底图片
  p_PP_OBJ_EJ_CORE_KEY_VISABLE,            // 0x33140021, 中子吹气按键选择是否可见
  p_PP_OBJ_obj34,                          // 0x33140022, 1
  p_PP_OBJ_obj35,                          // 0x33140023, 1
  p_PP_OBJ_obj36,                          // 0x33140024, 1
  p_PP_OBJ_obj37,                          // 0x33140025, 1
  p_PP_OBJ_obj38,                          // 0x33140026, 1
  p_PP_OBJ_obj39,                          // 0x33140027, 1
  p_PP_OBJ_obj40,                          // 0x33140028, 1
  p_PP_OBJ_obj41,                          // 0x33140029, 1
  p_PP_OBJ_obj42,                          // 0x3314002a, 1
  p_PP_OBJ_OPEN1_POS_DOT,                  // 0x3314002b, 开模一段位置小数位
  p_PP_OBJ_CLS_LOW_PRS_POS_DOT,            // 0x3314002c, 关模低压位置小数位
  p_PP_OBJ_CLS_HI_PRS_POS_DOT,             // 0x3314002d, 高压锁模位置小数位
  p_PP_OBJ_CLS1_POS_MAX,                   // 0x3314002e, 关模一段位置最大值
  p_PP_OBJ_CLS2_POS_MAX,                   // 0x3314002f, 关模二段位置最大值
  p_PP_OBJ_CLS2_POS_MIN,                   // 0x33140030, 关模二段位置最小值
  p_PP_OBJ_CLS3_POS_MAX,                   // 0x33140031, 关模三段位置最大值
  p_PP_OBJ_CLS3_POS_MIN,                   // 0x33140032, 关模三段位置最小值
  p_PP_OBJ_CLS_LOW_PRS_POS_MAX,            // 0x33140033, 关模低压位置最大值
  p_PP_OBJ_CLS_LOW_PRS_POS_MIN,            // 0x33140034, 关模低压位置最小值
  p_PP_OBJ_OPEN1_POS_MAX,                  // 0x33140035, 开模一段位置最大值
  p_PP_OBJ_OPEN2_POS_MAX,                  // 0x33140036, 开模二段位置最大值
  p_PP_OBJ_OPEN2_POS_MIN,                  // 0x33140037, 开模二段位置最小值
  p_PP_OBJ_OPEN3_POS_MAX,                  // 0x33140038, 开模三段位置最大值
  p_PP_OBJ_OPEN3_POS_MIN,                  // 0x33140039, 开模三段位置最小值
  p_PP_OBJ_OPEN4_POS_MIN,                  // 0x3314003a, 开模四段位置最小值
  p_PP_OBJ_OPEN_END_POS_MIN,               // 0x3314003b, 开模终止位置最小值
  p_PP_OBJ_OPEN1_STR_VAL,                  // 0x3314003c, 开模一段文本
  p_PP_OBJ_OPEN4_STR_VAL,                  // 0x3314003d, 开模四段文本
  p_PP_OBJ_OPEN_END_STR_VAL,               // 0x3314003e, 开模终止文本
  p_PP_OBJ_OPEN2_PUSH_POS_ENABLE,          // 0x3314003f, 开模二段十字头位置是否使用
  p_PP_OBJ_OPEN3_PUSH_POS_ENABLE,          // 0x33140040, 开模三段十字头位置是否使用
  p_PP_OBJ_INTELL_OPEN_FUNC_CODE,          // 0x33140041, code0-code5-code54-智能开模
  p_PP_OBJ_CLS1_STR_VAL,                   // 0x33140042, 关模一段文本
  p_PP_OBJ_CLS2_STR_VAL,                   // 0x33140043, 关模二段文本
  p_PP_OBJ_CLS1_PUSH_POS_ENABLE,           // 0x33140044, 关模一段十字头位置是否使用
  p_PP_OBJ_INTELL_CLS_FUNC_CODE,           // 0x33140045, code43-code0-code54-智能关模
  p_PP_OBJ_HI_PRS2_PUSH_POS_ENABLE,        // 0x33140046, 高压锁模2十字头位置是否可见
  p_PP_OBJ_PUSH_POS_UNIT2_ENABLE,          // 0x33140047, 关模十字头单位是否可见
  p_PP_OBJ_NOT_HI_PRS2_POS_ENABLE,         // 0x33140048, 反-高压锁模2是否使用
  p_PP_OBJ_CLP_POS_IMG_VAL,                // 0x33140049, 开模位置图片索引
  p_PP_OBJ_CLP_PO_CLS_SOL_IMG_VAL,         // 0x3314004a, 合模阀图片索引
  p_PP_OBJ_CLP_PO_OPN_SOL_IMG_VAL,         // 0x3314004b, 开模阀图片索引
  p_PP_OBJ_K,                              // 0x3314004c, delete合模到底图片索引
  p_PP_OBJ_OPEN4_PRS_ENABLE,               // 0x3314004d, 开模四段压力是否使用
  p_PP_OBJ_ADPOSI_METERMAXCLAMP2,          // 0x3314004e, 开关模电子尺最大值
  p_PP_OBJ_OPEN_END_STR1_VAL,              // 0x3314004f, 开模终止文本没有空格
  p_PP_OBJ_COOLLEFTTIME,                   // 0x33140050, 1
  p_PP_OBJ_CODE52EQUEL1,                   // 0x33140051, CODE52为1小数点切换
  p_PP_OBJ_CODE52EQUEL1MAX,                // 0x33140052, CODE52为1最大值切换
  p_PP_OBJ_CLS1PIC,                        // 0x33140053, 关模一段背景色
  p_PP_OBJ_CLS2PIC,                        // 0x33140054, 关模二段背景色
  p_PP_OBJ_CLS3PIC,                        // 0x33140055, 关模三段背景色
  p_PP_OBJ_CLSLPIC,                        // 0x33140056, 关模低压背景色
  p_PP_OBJ_CLSH1PIC,                       // 0x33140057, 关模高1背景色
  p_PP_OBJ_CLSH2PIC,                       // 0x33140058, 关模高2背景色
  p_PP_OBJ_OPN1PIC,                        // 0x33140059, 开模一段背景色
  p_PP_OBJ_OPN2PIC,                        // 0x3314005a, 开模二段背景色
  p_PP_OBJ_OPN3PIC,                        // 0x3314005b, 开模三段背景色
  p_PP_OBJ_OPN4PIC,                        // 0x3314005c, 开模四段背景色
  p_PP_OBJ_OPN5PIC,                        // 0x3314005d, 开模五段背景色
  p_PP_OBJ_LOWPRSTMMAX_DOT,                // 0x3314005e, 1
  p_PP_OBJ_LOWPRSTMMAX_MAX,                // 0x3314005f, 1
  p_PP_OBJ_HOLD_POS_ENABLE,                // 0x33140060, 保压位置是否使用
  p_PP_OBJ_INJ_AND_HOLD_TM_DOT,            // 0x33140061, 射出和保压时间小数位
  p_PP_OBJ_HOLD_SPD_MAX,                   // 0x33140062, 保压速度最大值
  p_PP_OBJ_INJ_SPD_MAX,                    // 0x33140063, 射出速度最大值
  p_PP_OBJ_INJ_AND_HOLD_SPD_DOT,           // 0x33140064, 射出和保压速度小数位
  p_PP_OBJ_INJ_TOTAL_TM_VISABLE,           // 0x33140065, 射出总时间是否可见
  p_PP_OBJ_INJ_2_ENABLE,                   // 0x33140066, 射出2段是否使用
  p_PP_OBJ_INJ_3_ENABLE,                   // 0x33140067, 射出3段是否使用
  p_PP_OBJ_INJ_4_ENABLE,                   // 0x33140068, 射出4段是否使用
  p_PP_OBJ_INJ_5_ENABLE,                   // 0x33140069, 射出5段是否使用
  p_PP_OBJ_INJ_6_ENABLE,                   // 0x3314006a, 射出6段是否使用
  p_PP_OBJ_INJ_TM_2_ENABLE,                // 0x3314006b, 射出时间2段是否使用
  p_PP_OBJ_INJ_TM_3_ENABLE,                // 0x3314006c, 射出时间3段是否使用
  p_PP_OBJ_INJ_TM_4_ENABLE,                // 0x3314006d, 射出时间4段是否使用
  p_PP_OBJ_INJ_TM_5_ENABLE,                // 0x3314006e, 射出时间5段是否使用
  p_PP_OBJ_INJ_TM_6_ENABLE,                // 0x3314006f, 射出时间6段是否使用
  p_PP_OBJ_INJ_POS_2_ENABLE,               // 0x33140070, 射出位置2段是否使用
  p_PP_OBJ_INJ_POS_3_ENABLE,               // 0x33140071, 射出位置3段是否使用
  p_PP_OBJ_INJ_POS_4_ENABLE,               // 0x33140072, 射出位置4段是否使用
  p_PP_OBJ_INJ_POS_5_ENABLE,               // 0x33140073, 射出位置5段是否使用
  p_PP_OBJ_INJ_POS_6_ENABLE,               // 0x33140074, 射出位置6段是否使用
  p_PP_OBJ_INJ_POS_1_MIN,                  // 0x33140075, 射出位置1段最小值
  p_PP_OBJ_INJ_POS_2_MIN,                  // 0x33140076, 射出位置2段最小值
  p_PP_OBJ_INJ_POS_3_MIN,                  // 0x33140077, 射出位置3段最小值
  p_PP_OBJ_INJ_POS_4_MIN,                  // 0x33140078, 射出位置4段最小值
  p_PP_OBJ_INJ_POS_5_MIN,                  // 0x33140079, 射出位置5段最小值
  p_PP_OBJ_HOLD_2_ENABLE,                  // 0x3314007a, 保压2段是否使用
  p_PP_OBJ_HOLD_3_ENABLE,                  // 0x3314007b, 保压3段是否使用
  p_PP_OBJ_HOLD_4_ENABLE,                  // 0x3314007c, 保压4段是否使用
  p_PP_OBJ_HOLD_5_ENABLE,                  // 0x3314007d, 保压5段是否使用
  p_PP_OBJ_INJ_POS_IMG_VAL,                // 0x3314007e, 射出位置图片索引
  p_PP_OBJ_INJ_PO_SOL_IMG_VAL,             // 0x3314007f, 射出阀图片索引
  p_PP_OBJ_NOZ_PO_FWD_IMG_VAL,             // 0x33140080, 座进阀图片索引
  p_PP_OBJ_INJ_POS_1_ENABLE,               // 0x33140081, 射出一段位置是否使用
  p_PP_OBJ_INJ_TM_1_ENABLE,                // 0x33140082, 射出一段时间是否使用
  p_PP_OBJ_INJ_SEG_TM_VISIBLE,             // 0x33140083, 射出分段时间是否可见
  p_PP_OBJ_INJ_STATE_TM_VISIBLE,           // 0x33140084, 各段射出时间是否可见
  p_PP_OBJ_CLAMPPRS_TEXTCOLOR,             // 0x33140085, 开关模压力上限文本色
  p_PP_OBJ_CLAMPPRS_BKCOLOR,               // 0x33140086, 开关模压力上限背景色
  p_PP_OBJ_CLAMPPRS_READABLE,              // 0x33140087, 开关模压力上限只读
  p_PP_OBJ_obj136,                         // 0x33140088, 1
  p_PP_OBJ_obj137,                         // 0x33140089, 1
  p_PP_OBJ_obj138,                         // 0x3314008a, 1
  p_PP_OBJ_BACK_PRS_DOT,                   // 0x3314008b, 储料背压小数位
  p_PP_OBJ_BACK_PRS_MAX,                   // 0x3314008c, 储料背压最大值
  p_PP_OBJ_CHG_2_POS_MAX,                  // 0x3314008d, 储料2段终止位置最大值
  p_PP_OBJ_BACK_PRS_ENABLE,                // 0x3314008e, 储料背压是否可用
  p_PP_OBJ_BACK_PRS_3_ENABLE,              // 0x3314008f, 储料背压3段是否可用
  p_PP_OBJ_CHG_3_ENABLE,                   // 0x33140090, 储料3段是否使用
  p_PP_OBJ_SK_BEF_CHG_DIST_ENABLE,         // 0x33140091, 储前射退距离是否可用
  p_PP_OBJ_SK_BEF_CHG_TM_ENABLE,           // 0x33140092, 储前射退时间是否可用
  p_PP_OBJ_REMELT_ENABLE,                  // 0x33140093, 再次储料时间是否使用
  p_PP_OBJ_SK_END_POS_MAX,                 // 0x33140094, 射退终止位置最大值
  p_PP_OBJ_PLUS_VISABLE,                   // 0x33140095, 加号是否可见
  p_PP_OBJ_SK_DIST_STR_VAL,                // 0x33140096, 射退距离文本
  p_PP_OBJ_CHG_PO_SOL_IMG_VAL,             // 0x33140097, 储料阀图片索引
  p_PP_OBJ_SK_PO_SOL_IMG_VAL,              // 0x33140098, 射退阀图片索引
  p_PP_OBJ_REMELT_POS_MAX,                 // 0x33140099, 再次储料位置最大值
  p_PP_OBJ_CHG_3_POS_MAX,                  // 0x3314009a, 储料3段终止位置最大值
  p_PP_OBJ_CHG_2_ENABLE,                   // 0x3314009b, 储料2段是否是当前选择段数-DA画面使用
  p_PP_OBJ_CHG_4_ENABLE,                   // 0x3314009c, 1
  p_PP_OBJ_BACK_PRS_4_ENABLE,              // 0x3314009d, 1
  p_PP_OBJ_CHG_4_POS_MAX,                  // 0x3314009e, 1
  p_PP_OBJ_MULTICHG_VISIBLE,               // 0x3314009f, 1
  p_PP_OBJ_PURGE_ALONE_ENABLE,             // 0x331400a0, 自动清料单独压力流量是否可用
  p_PP_OBJ_obj161,                         // 0x331400a1, 1
  p_PP_OBJ_obj162,                         // 0x331400a2, 1
  p_PP_OBJ_obj163,                         // 0x331400a3, 1
  p_PP_OBJ_AIR7_POS_ENABLE,                // 0x331400a4, 吹气7结束位置是否使用
  p_PP_OBJ_AIR8_POS_ENABLE,                // 0x331400a5, 吹气8结束位置是否使用
  p_PP_OBJ_AIR9_POS_ENABLE,                // 0x331400a6, 吹气9结束位置是否使用
  p_PP_OBJ_AIR10_POS_ENABLE,               // 0x331400a7, 吹气10结束位置是否使用
  p_PP_OBJ_AIR11_POS_ENABLE,               // 0x331400a8, 吹气11结束位置是否使用
  p_PP_OBJ_AIR12_POS_ENABLE,               // 0x331400a9, 吹气12结束位置是否使用
  p_PP_OBJ_AIR13_POS_ENABLE,               // 0x331400aa, 吹气13结束位置是否使用
  p_PP_OBJ_AIR14_POS_ENABLE,               // 0x331400ab, 吹气14结束位置是否使用
  p_PP_OBJ_AIR7_POS_MAX,                   // 0x331400ac, 吹气7结束位置最大值
  p_PP_OBJ_AIR8_POS_MAX,                   // 0x331400ad, 吹气8结束位置最大值
  p_PP_OBJ_AIR9_POS_MAX,                   // 0x331400ae, 吹气9结束位置最大值
  p_PP_OBJ_AIR10_POS_MAX,                  // 0x331400af, 吹气10结束位置最大值
  p_PP_OBJ_AIR11_POS_MAX,                  // 0x331400b0, 吹气11结束位置最大值
  p_PP_OBJ_AIR12_POS_MAX,                  // 0x331400b1, 吹气12结束位置最大值
  p_PP_OBJ_AIR13_POS_MAX,                  // 0x331400b2, 吹气13结束位置最大值
  p_PP_OBJ_AIR14_POS_MAX,                  // 0x331400b3, 吹气14结束位置最大值
  p_PP_OBJ_obj180,                         // 0x331400b4, 1
  p_PP_OBJ_obj181,                         // 0x331400b5, 1
  p_PP_OBJ_obj182,                         // 0x331400b6, 1
  p_PP_OBJ_obj183,                         // 0x331400b7, 1
  p_PP_OBJ_obj184,                         // 0x331400b8, 1
  p_PP_OBJ_STAY_EJTADV_KEEP_ENABLE,        // 0x331400b9, 停留时托进保持是否可用
  p_PP_OBJ_DLY_MODE_BEF_EJTAdV_ENABLE,     // 0x331400ba, 托进前延迟方式是否可用
  p_PP_OBJ_DLY_MODE_BEF_EJTRET_ENABLE,     // 0x331400bb, 托退前延迟方式是否可用
  p_PP_OBJ_EJRET_END_FILTER_TM_ENABLE,     // 0x331400bc, 托退终滤波时间是否可用
  p_PP_OBJ_obj189,                         // 0x331400bd, 1
  p_PP_OBJ_obj190,                         // 0x331400be, 1
  p_PP_OBJ_obj191,                         // 0x331400bf, 1
  p_PP_OBJ_obj192,                         // 0x331400c0, 1
  p_PP_OBJ_obj193,                         // 0x331400c1, 1
  p_PP_OBJ_OPEN_LINK_ENABLE,               // 0x331400c2, 开模联动是否可用
  p_PP_OBJ_CLOSE_LINK_ENABLE,              // 0x331400c3, 关模联动是否可用
  p_PP_OBJ_OPEN_LINK_POS_ENABLE,           // 0x331400c4, 开模联动位置是否可用
  p_PP_OBJ_LINK_VLV_DLYOFF_TM_ENABLE,      // 0x331400c5, 联动阀延迟关计时是否可用
  p_PP_OBJ_CLOSE_LINK_POS_ENABLE,          // 0x331400c6, 关模联动位置是否可用
  p_PP_OBJ_INTELLCLS_DIST_ENABLE,          // 0x331400c7, 智能关模启动距离是否可用
  p_PP_OBJ_INTELLCLS_TM_ENABLE,            // 0x331400c8, 智能关模启动保护时间是否可用
  p_PP_OBJ_CLS_3_OPTION_ENABLE,            // 0x331400c9, 关模三段选项是否可用
  p_PP_OBJ_OPEN_4_OPTION_ENABLE,           // 0x331400ca, 开模四段选项是否可用
  p_PP_OBJ_FLOAT_CYLIND_PRS_ENABLE,        // 0x331400cb, 浮动缸压力是否可用
  p_PP_OBJ_FLOAT_CYLIND_SPD_ENABLE,        // 0x331400cc, 浮动缸速度是否可用
  p_PP_OBJ_INTELL_CLS_OPTION_ENABLE,       // 0x331400cd, 智能关模功能是否可用
  p_PP_OBJ_INTELL_OPEN_OPTION_ENABLE,      // 0x331400ce, 智能开模功能是否可用
  p_PP_OBJ_OPEN_RETARD_TM_ENABLE,          // 0x331400cf, 开模减速时间是否可用
  p_PP_OBJ_a,                              // 0x331400d0, 1
  p_PP_OBJ_b,                              // 0x331400d1, 1
  p_PP_OBJ_c,                              // 0x331400d2, 1
  p_PP_OBJ_obj211,                         // 0x331400d3, 1
  p_PP_OBJ_obj212,                         // 0x331400d4, 1
  p_PP_OBJ_HOLD_PorS_ENABLE,               // 0x331400d5, 保压压力或速度是否可用
  p_PP_OBJ_HOLD_PorS_MAX,                  // 0x331400d6, 保压压力或速度最大值
  p_PP_OBJ_HOLD_PorS_UNIT,                 // 0x331400d7, 保压压力或速度单位
  p_PP_OBJ_INJ_PorS_ENABLE,                // 0x331400d8, 射出压力或速度是否可用
  p_PP_OBJ_INJ_PorS_UNIT,                  // 0x331400d9, 射出压力或速度单位
  p_PP_OBJ_HOLD_PRS_VISABLE,               // 0x331400da, 保压压力是否可见
  p_PP_OBJ_HOLD_SPD_VISABLE,               // 0x331400db, 保压速度是否可见
  p_PP_OBJ_INJ_PRS_VISABLE,                // 0x331400dc, 射出压力是否可见
  p_PP_OBJ_INJ_SPD_VISABLE,                // 0x331400dd, 射出速度是否可见
  p_PP_OBJ_ACC_HY_CIR_SELT_ENABLE,         // 0x331400de, 蓄能器油路选择是否可用
  p_PP_OBJ_PRS_SENSOR_ENABLE,              // 0x331400df, 蓄能器压力传感器是否使用
  p_PP_OBJ_INJ_1_TM_BEF_COMPRS_ENABLE,     // 0x331400e0, 压塑前射出一段计时是否可用
  p_PP_OBJ_COM_PRS_VISABLE,                // 0x331400e1, 压塑是否可见
  p_PP_OBJ_INJ_1_TM_BEF_COMPRS_DOT,        // 0x331400e2, 压塑前射出一段计时小数位
  p_PP_OBJ_INJ_1_TM_BEF_COMPRS_MAX,        // 0x331400e3, 压塑前射出一段计时最大值
  p_PP_OBJ_OPEN_LOOP_ENABLE,               // 0x331400e4, 开环功能是否使用
  p_PP_OBJ_OIL_LEAK_VISABLE,               // 0x331400e5, 漏油功能是否可见（威力士）
  p_PP_OBJ_obj230,                         // 0x331400e6, 1
  p_PP_OBJ_obj231,                         // 0x331400e7, 1
  p_PP_OBJ_obj232,                         // 0x331400e8, 1
  p_PP_OBJ_obj233,                         // 0x331400e9, 1
  p_PP_OBJ_EXHT_INJ_POS_ENABLE,            // 0x331400ea, 排气时射出位置是否可用
  p_PP_OBJ_EXHT_INJ_TM_ENABLE,             // 0x331400eb, 排气时射出时间是否可用
  p_PP_OBJ_INJ_DIST_BEF_NOZADV_ENABLE,     // 0x331400ec, 座进前射出距离是否可用
  p_PP_OBJ_INJ_TM_BEF_NOZADV_ENABLE,       // 0x331400ed, 座进前射出时间是否可用
  p_PP_OBJ_EXHT_INJ_TM_MAX,                // 0x331400ee, 排气时射出时间最大值
  p_PP_OBJ_EXHT_INJ_TM_DOT,                // 0x331400ef, 排气时射出时间小数位
  p_PP_OBJ_EXHT_FUNC_ENABLE,               // 0x331400f0, 排气功能是否使用
  p_PP_OBJ_HYDRAULIC_NOZZLES_ENABLE,       // 0x331400f1, 液压喷嘴是否使用
  p_PP_OBJ_INJ_MODE_BEF_NOZADV_ENABLE,     // 0x331400f2, 座进前射出方式是否可用
  p_PP_OBJ_EXHT_TM_DOT,                    // 0x331400f3, 1
  p_PP_OBJ_EXHT_TM_MAX4,                   // 0x331400f4, 1
  p_PP_OBJ_EXHT_TM_MAX3,                   // 0x331400f5, 1
  p_PP_OBJ_INJ_TM_BEF_NOZADV_MAX,          // 0x331400f6, 1
  p_PP_OBJ_obj247,                         // 0x331400f7, 1
  p_PP_OBJ_BMC_VISABLE,                    // 0x331400f8, BMC是否可见
  p_PP_OBJ_CHG_OPEN_LOOP_ENABLE,           // 0x331400f9, 储料开环是否可用
  p_PP_OBJ_CHG_OPEN_LINK_ENABLE,           // 0x331400fa, 储料开模联动是否可用
  p_PP_OBJ_obj251,                         // 0x331400fb, 1
  p_PP_OBJ_obj252,                         // 0x331400fc, 1
  p_PP_OBJ_obj253,                         // 0x331400fd, 1
  p_PP_OBJ_obj254,                         // 0x331400fe, 1
  p_PP_OBJ_obj255,                         // 0x331400ff, 1
  p_PP_OBJ_COUNT_TIME_A_VISABLE,           // 0x33140100, 计数时间A是否可见
  p_PP_OBJ_COUNT_TIME_B_VISIBLE,           // 0x33140101, 计数时间B是否可见
  p_PP_OBJ_COUNT_TIME_C_VISIBLE,           // 0x33140102, 计数时间C是否可见
  p_PP_OBJ_COUNT_TIME_D_VISIBLE,           // 0x33140103, 计数时间D是否可见
  p_PP_OBJ_COUNT_TIME_E_VISIBLE,           // 0x33140104, 计数时间E是否可见
  p_PP_OBJ_COUNT_TIME_F_VISIBLE,           // 0x33140105, 计数时间F是否可见
  p_PP_OBJ_TRAVEL_TIME_A_VISIBLE,          // 0x33140106, 行程时间A是否可见
  p_PP_OBJ_TRAVEL_TIME_B_VISIBLE,          // 0x33140107, 行程时间B是否可见
  p_PP_OBJ_TRAVEL_TIME_C_VISIBLE,          // 0x33140108, 行程时间C是否可见
  p_PP_OBJ_TRAVEL_TIME_D_VISIBLE,          // 0x33140109, 行程时间D是否可见
  p_PP_OBJ_TRAVEL_TIME_E_VISIBLE,          // 0x3314010a, 行程时间E是否可见
  p_PP_OBJ_TRAVEL_TIME_F_VISIBLE,          // 0x3314010b, 行程时间F是否可见
  p_PP_OBJ_ACT_POS_A_IN_ENABLE,            // 0x3314010c, 动作位置A-IN是否可用
  p_PP_OBJ_ACT_POS_B_IN_ENABLE,            // 0x3314010d, 动作位置B-IN是否可用
  p_PP_OBJ_ACT_POS_C_IN_ENABLE,            // 0x3314010e, 动作位置C-IN是否可用
  p_PP_OBJ_ACT_POS_D_IN_ENABLE,            // 0x3314010f, 动作位置D-IN是否可用
  p_PP_OBJ_ACT_POS_E_IN_ENABLE,            // 0x33140110, 动作位置E-IN是否可用
  p_PP_OBJ_ACT_POS_F_IN_ENABLE,            // 0x33140111, 动作位置F-IN是否可用
  p_PP_OBJ_ACT_POS_A_OUT_ENABLE,           // 0x33140112, 动作位置A-OUT是否可用
  p_PP_OBJ_ACT_POS_B_OUT_ENABLE,           // 0x33140113, 动作位置B-OUT是否可用
  p_PP_OBJ_ACT_POS_C_OUT_ENABLE,           // 0x33140114, 动作位置C-OUT是否可用
  p_PP_OBJ_ACT_POS_D_OUT_ENABLE,           // 0x33140115, 动作位置D-OUT是否可用
  p_PP_OBJ_ACT_POS_E_OUT_ENABLE,           // 0x33140116, 动作位置E-OUT是否可用
  p_PP_OBJ_ACT_POS_F_OUT_ENABLE,           // 0x33140117, 动作位置F-OUT是否可用
  p_PP_OBJ_COREACTTM_DOT,                  // 0x33140118, 中子动作时间小数位
  p_PP_OBJ_COREACTTM_MAX,                  // 0x33140119, 中子动作时间最大值
  p_PP_OBJ_ACT_POS_IN_ENABLE,              // 0x3314011a, 动作位置IN是否可用
  p_PP_OBJ_ACT_POS_OUT_ENABLE,             // 0x3314011b, 动作位置OUT是否可用
  p_PP_OBJ_COUNT_TIME_VISABLE,             // 0x3314011c, 计数时间是否可见
  p_PP_OBJ_TRAVEL_TIME_VISIBLE,            // 0x3314011d, 动作位置IN是否可用
  p_PP_OBJ_obj286,                         // 0x3314011e, 1
  p_PP_OBJ_SPECIAL_CORE_FUNC_ENABLE,       // 0x3314011f, 特殊中子功能是否可用
  p_PP_OBJ_SPECIAL_CORE_FUNC_VISABLE,      // 0x33140120, 特殊中子功能是否使用
  p_PP_OBJ_SPECIAL_CORE_1_FUNC_VISABLE,    // 0x33140121, 特殊中子1功能是否使用
  p_PP_OBJ_SPECIAL_CORE_2_FUNC_VISABLE,    // 0x33140122, 特殊中子2功能是否使用
  p_PP_OBJ_SPECIAL_CORE_3_FUNC_VISABLE,    // 0x33140123, 特殊中子3功能是否使用
  p_PP_OBJ_SPECIAL_CORE_4_FUNC_VISABLE,    // 0x33140124, 特殊中子4功能是否使用
  p_PP_OBJ_SPECIAL_CORE_5_FUNC_VISABLE,    // 0x33140125, 特殊中子5功能是否使用
  p_PP_OBJ_SPECIAL_CORE_6_FUNC_VISABLE,    // 0x33140126, 特殊中子6功能是否使用
  p_PP_OBJ_WAY_CORE_OUT_MIN_POS_MAX,       // 0x33140127, 途中中子退下限位置最大值
  p_PP_OBJ_SPECIAL_CORE_7_FUNC_VISABLE,    // 0x33140128, 特殊中子7功能是否使用
  p_PP_OBJ_obj297,                         // 0x33140129, 1
  p_PP_OBJ_obj298,                         // 0x3314012a, 1
  p_PP_OBJ_obj299,                         // 0x3314012b, 1
  p_PP_OBJ_MANUAL_TEMP_ENABLE,             // 0x3314012c, 手动温度是否可用
  p_PP_OBJ_TEMP_SET_VAL_MAX,               // 0x3314012d, 温度设定值最大值
  p_PP_OBJ_TEMP_1_IMG_VAL,                 // 0x3314012e, 温度一段图片索引
  p_PP_OBJ_TEMP_2_IMG_VAL,                 // 0x3314012f, 温度二段图片索引
  p_PP_OBJ_TEMP_3_IMG_VAL,                 // 0x33140130, 温度三段图片索引
  p_PP_OBJ_TEMP_4_IMG_VAL,                 // 0x33140131, 温度四段图片索引
  p_PP_OBJ_TEMP_5_IMG_VAL,                 // 0x33140132, 温度五段图片索引
  p_PP_OBJ_TEMP_6_IMG_VAL,                 // 0x33140133, 温度六段图片索引
  p_PP_OBJ_TEMP_7_IMG_VAL,                 // 0x33140134, 温度七段图片索引
  p_PP_OBJ_TEMP_4_VISABLE,                 // 0x33140135, 温度四段是否可见
  p_PP_OBJ_TEMP_5_VISABLE,                 // 0x33140136, 温度五段是否可见
  p_PP_OBJ_TEMP_6_VISABLE,                 // 0x33140137, 温度六段是否可见
  p_PP_OBJ_TEMP_7_VISABLE,                 // 0x33140138, 温度七段是否可见
  p_PP_OBJ_TEMP_1_SET_ENABLE,              // 0x33140139, 温度一段设定是否可用
  p_PP_OBJ_TEMP_8_IMG_VAL,                 // 0x3314013a, 温度八段图片索引
  p_PP_OBJ_TEMP_9_IMG_VAL,                 // 0x3314013b, 温度九段图片索引
  p_PP_OBJ_TEMP_10_IMG_VAL,                // 0x3314013c, 温度十段图片索引
  p_PP_OBJ_TEMP_11_IMG_VAL,                // 0x3314013d, 温度十一段图片索引
  p_PP_OBJ_TEMP_12_IMG_VAL,                // 0x3314013e, 温度十二段图片索引
  p_PP_OBJ_TEMP_13_IMG_VAL,                // 0x3314013f, 温度十三段图片索引
  p_PP_OBJ_TEMP_14_IMG_VAL,                // 0x33140140, 温度十四段图片索引
  p_PP_OBJ_TEMP_8_VISABLE,                 // 0x33140141, 1
  p_PP_OBJ_COLDSTRPREVTMMIN,               // 0x33140142, 1
  p_PP_OBJ_SWISSETDEFAULT,                 // 0x33140143, 1
  p_PP_OBJ_DROPMAX,                        // 0x33140144, 1
  p_PP_OBJ_obj325,                         // 0x33140145, 1
  p_PP_OBJ_obj326,                         // 0x33140146, 1
  p_PP_OBJ_obj327,                         // 0x33140147, 1
  p_PP_OBJ_obj328,                         // 0x33140148, 1
  p_PP_OBJ_obj329,                         // 0x33140149, 1
  p_PP_OBJ_EVERY_INCHING_STEP_ENABLE,      // 0x3314014a, 每次点动步数是否可用
  p_PP_OBJ_POS_ADJUST_ENABLE,              // 0x3314014b, 震雄调模是否可用
  p_PP_OBJ_NOZ_POS_ENABLE,                 // 0x3314014c, 座台使用位置是否使用
  p_PP_OBJ_NOZ_METER_VISIBLE,              // 0x3314014d, 座台位置是否可见
  p_PP_OBJ_PRESSOR_VISIBLE,                // 0x3314014e, 传感器是否可见
  p_PP_OBJ_ZERO_VISIBLE,                   // 0x3314014f, 伺服5是否可见
  p_PP_OBJ_AD4_VISIBLE,                    // 0x33140150, 伺服3是否可见
  p_PP_OBJ_NOZ_PO_BWD_IMG_VAL,             // 0x33140151, 座退阀图片索引
  p_PP_OBJ_CNPOSI_ADJPROPERTIONAL_DOT,     // 0x33140152, 每齿移动距离小数位
  p_PP_OBJ_CNPOSI_ADJPROPERTIONAL_MAX,     // 0x33140153, 每齿移动距离最大值
  p_PP_OBJ_AUTOBADCNT_VISBLE,              // 0x33140154, 自动初期不良品模数可见
  p_PP_OBJ_QDCFUNC_ENABLE,                 // 0x33140155, QDF功能是否可用
  p_PP_OBJ_QDCFUNC_VISIBLE,                // 0x33140156, QDF功能是否可见
  p_PP_OBJ_AUTOBADCNT_ENABLE,              // 0x33140157, 自动初期不良品模数可用
  p_PP_OBJ_AUTO_SAFE_DOOR_ENABLE,          // 0x33140158, 自动安全门是否使用
  p_PP_OBJ_WAY_ROBOT_FUNC_ENABLE,          // 0x33140159, 途中机械手是否使用
  p_PP_OBJ_ROB_CORE_1_LINK_ENABLE,         // 0x3314015a, 中子信号1连接是否可用
  p_PP_OBJ_ROB_CORE_2_LINK_ENABLE,         // 0x3314015b, 中子信号2连接是否可用
  p_PP_OBJ_SPECIAL_LINK_ENABLE,            // 0x3314015c, 特殊联动是否可用
  p_PP_OBJ_ROBOTACTTM_DOT,                 // 0x3314015d, 机械手动作时间小数位
  p_PP_OBJ_ROBOTACTTM_MAX,                 // 0x3314015e, 机械手动作时间最大值
  p_PP_OBJ_WAYROBOTPOS_MIN,                // 0x3314015f, 途中机械手启动位置最小值
  p_PP_OBJ_obj352,                         // 0x33140160, 1
  p_PP_OBJ_CLS_HIGHPRS_KEEP_TM_DOT,        // 0x33140161, 关模高压计时小数位
  p_PP_OBJ_CLS_HIGHPRS_KEEP_TM_MAX,        // 0x33140162, 关模高压计时最大值
  p_PP_OBJ_WAY_ROBOT_OUTPUT_TM_DOT,        // 0x33140163, 途中机械手输出时间小数位
  p_PP_OBJ_WAY_ROBOT_OUTPUT_TM_MAX,        // 0x33140164, 途中机械手输出时间最大值
  p_PP_OBJ_RAMP_CHOOSED_ENABLE,            // 0x33140165, 部分斜率是否可用
  p_PP_OBJ_OPN_4_SPD_RAMP_STR_VAL,         // 0x33140166, 开模四段速度斜率文本
  p_PP_OBJ_OPN_DIFFVLV_ACT_TM_ENABLE,      // 0x33140167, 开模差动阀动作时间是否可用
  p_PP_OBJ_PUSH_POS_VISIBLE,               // 0x33140168, 控制十字头是否可见
  p_PP_OBJ_obj361,                         // 0x33140169, 1
  p_PP_OBJ_MANL_USE_DEBUG_PRSSPD_ENABLE,   // 0x3314016a, 手动使用调试压力速度是否使用
  p_PP_OBJ_INJ_CURVE_POS_VISIBLE,          // 0x3314016b, 射出曲线模式为位置
  p_PP_OBJ_INJ_CURVE_TM_DOT,               // 0x3314016c, 射出和保压时间精确到0.001或0.1
  p_PP_OBJ_CONFLUCHG_SPDMAX_ENABLE,        // 0x3314016d, 合流储料速度限制是否使用
  p_PP_OBJ_obj366,                         // 0x3314016e, 1
  p_PP_OBJ_obj367,                         // 0x3314016f, 1
  p_PP_OBJ_CORE_SELECT_ENABLE,             // 0x33140170, 中子选择是否使用
  p_PP_OBJ_obj369,                         // 0x33140171, 1
  p_PP_OBJ_obj370,                         // 0x33140172, 1
  p_PP_OBJ_EXTRA_TEMP_VISIBLE,             // 0x33140173, 额外温度是否使用
  p_PP_OBJ_ONE_TO_SEVEN_MIN,               // 0x33140174, 一到七段最小值
  p_PP_OBJ_ONE_TO_SEVEN_MAX,               // 0x33140175, 一到七段最大值
  p_PP_OBJ_o,                              // 0x33140176, 1
  p_PP_OBJ_obj375,                         // 0x33140177, 1
  p_PP_OBJ_RELEASEMD_INITIAL_ENABLE,       // 0x33140178, 松模初始化是否可用
  p_PP_OBJ_LOCKPRS_EFFAREA_MAX,            // 0x33140179, 锁模力最大有效区最大值
  p_PP_OBJ_MOLD_THICK_ADJUST_ENABLE,       // 0x3314017a, 模厚调模是否使用
  p_PP_OBJ_obj379,                         // 0x3314017b, 1
  p_PP_OBJ_obj380,                         // 0x3314017c, 润滑模数上限
  p_PP_OBJ_MOTORTEMP_VISIBLE,              // 0x3314017d, 电机温度是否可见
  p_PP_OBJ_SAFEDOOR_IMG_VAL,               // 0x3314017e, 安全门图片索引
  p_PP_OBJ_SAFEDOOR_2_IMG_VAL,             // 0x3314017f, 安全门2图片索引
  p_PP_OBJ_BACK_SAFEDOOR_IMG_VAL,          // 0x33140180, 后安全门图片索引
  p_PP_OBJ_SCREW_CHECK_IMG_VAL,            // 0x33140181, 螺杆检知图片索引
  p_PP_OBJ_OPN_END_IMG_VAL,                // 0x33140182, 开模终图片索引
  p_PP_OBJ_LUB_CHK1_IMG_VAL,               // 0x33140183, 润滑检知1图片索引
  p_PP_OBJ_LUB_CHK2_IMG_VAL,               // 0x33140184, 润滑检知2图片索引
  p_PP_OBJ_LUB_CHK3_IMG_VAL,               // 0x33140185, 润滑检知3图片索引
  p_PP_OBJ_PRD_CHK_IMG_VAL,                // 0x33140186, 检出电眼图片索引
  p_PP_OBJ_NOZ_FWD_IMG_VAL,                // 0x33140187, 座进终图片索引
  p_PP_OBJ_NOZ_BWD_IMG_VAL,                // 0x33140188, 座退终图片索引
  p_PP_OBJ_SERVO_ST_VISIBLE,               // 0x33140189, 伺服状态信息是否可见
  p_PP_OBJ_MOTOR_VISIBLE,                  // 0x3314018a, 电机图标是否可见
  p_PP_OBJ_AD6_VISIBLE,                    // 0x3314018b, dtAD6是否可见
  p_PP_OBJ_NOZZLE_VISIBLE,                 // 0x3314018c, dtnozzle是否可见
  p_PP_OBJ_SERVO_VISIBLE,                  // 0x3314018d, stbar/pixprs/dtsvo/stsvo/dtsvo2/strpm2 是否可见
  p_PP_OBJ_SERVO_ST1_VISIBLE,              // 0x3314018e, 伺服通信状态是否可见1
  p_PP_OBJ_SERVO_ST2_VISIBLE,              // 0x3314018f, 伺服通信状态是否可见2
  p_PP_OBJ_SERVO_ST3_VISIBLE,              // 0x33140190, 伺服通信状态是否可见3
  p_PP_OBJ_SERVO_ST4_VISIBLE,              // 0x33140191, 伺服通信状态是否可见4
  p_PP_OBJ_SERVO_ST5_VISIBLE,              // 0x33140192, 伺服通信状态是否可见5
  p_PP_OBJ_SERVO_ST6_VISIBLE,              // 0x33140193, 伺服通信状态是否可见6
  p_PP_OBJ_SERVO_ST7_VISIBLE,              // 0x33140194, 伺服通信状态是否可见7
  p_PP_OBJ_SERVO_COLOR1_VAL,               // 0x33140195, 伺服通信状态颜色1
  p_PP_OBJ_SERVO_COLOR2_VAL,               // 0x33140196, 伺服通信状态颜色2
  p_PP_OBJ_SERVO_COLOR3_VAL,               // 0x33140197, 伺服通信状态颜色3
  p_PP_OBJ_SERVO_COLOR4_VAL,               // 0x33140198, 伺服通信状态颜色4
  p_PP_OBJ_SERVO_COLOR5_VAL,               // 0x33140199, 伺服通信状态颜色5
  p_PP_OBJ_SERVO_COLOR6_VAL,               // 0x3314019a, 伺服通信状态颜色6
  p_PP_OBJ_SERVO_COLOR7_VAL,               // 0x3314019b, 伺服通信状态颜色7
  p_PP_OBJ_NOZZLE_AD_VISIBLE,              // 0x3314019c, 座台电子尺是否可见
  p_PP_OBJ_ACC_VISIBLE,                    // 0x3314019d, 蓄能器是否可见
  p_PP_OBJ_PIXSCREWSPD_VISIBLE,            // 0x3314019e, 螺杆是否可见
  p_PP_OBJ_HEATER_VISIBLE,                 // 0x3314019f, 加热图标是否可见
  p_PP_OBJ_ROBOT_VISIBLE,                  // 0x331401a0, 机械手图标是否可见
  p_PP_OBJ_LUBR_VISIBLE,                   // 0x331401a1, 润滑图标是否可见
  p_PP_OBJ_A920_MODEL_ENABLE,              // 0x331401a2, A920是否选用
  p_PP_OBJ_SENSOR_NOZ_UNIT,                // 0x331401a3, 传感器座台单位切换
  p_PP_OBJ_SENSOR_AD6_UNIT,                // 0x331401a4, 传感器AD6单位切换
  p_PP_OBJ_SENSOR_VISIBLE,                 // 0x331401a5, 传感器是否可见
  p_PP_OBJ_NOZ_VISIBLE,                    // 0x331401a6, 座台是否可见
  p_PP_OBJ_SENSOR2_VISIBLE,                // 0x331401a7, AD6传感器是否可见
  p_PP_OBJ_CFG_AD6_VISIBLE,                // 0x331401a8, AD6是否可见
  p_PP_OBJ_WEIYA_ENABLE,                   // 0x331401a9, 厂商是否为威亚
  p_PP_OBJ_LUB_MAX,                        // 0x331401aa, 自动润滑模数最大值
  p_PP_OBJ_obj427,                         // 0x331401ab, 1
  p_PP_OBJ_CODE3_2_ENABLE,                 // 0x331401ac, code3打2是否使用
  p_PP_OBJ_obj429,                         // 0x331401ad, 1
  p_PP_OBJ_obj430,                         // 0x331401ae, 1
  p_PP_OBJ_ADD_SIGN5_VISIBLE,              // 0x331401af, 加号5是否可见
  p_PP_OBJ_ADD_SIGN6_VISIBLE,              // 0x331401b0, 加号6是否可见
  p_PP_OBJ_ADD_SIGN1_VISIBLE,              // 0x331401b1, 加号1是否可见
  p_PP_OBJ_ADD_SIGN2_VISIBLE,              // 0x331401b2, 加号2是否可见
  p_PP_OBJ_ADD_SIGN3_VISIBLE,              // 0x331401b3, 加号3是否可见
  p_PP_OBJ_ADD_SIGN4_VISIBLE,              // 0x331401b4, 加号4是否可见
  p_PP_OBJ_ROOM_ADD_SIGN_VISIBLE,          // 0x331401b5, 室温加号是否可见
  p_PP_OBJ_ROOM_SUB_SIGN_VISIBLE,          // 0x331401b6, 室温减号是否可见
  p_PP_OBJ_obj439,                         // 0x331401b7, 1
  p_PP_OBJ_USE_SERVO_VISIBLE,              // 0x331401b8, 伺服是否使用
  p_PP_OBJ_obj441,                         // 0x331401b9, 1
  p_PP_OBJ_obj442,                         // 0x331401ba, 1
  p_PP_OBJ_PUMP_ADJUST_ENABLE,             // 0x331401bb, 泵浦调整是否可用
  p_PP_OBJ_MULT_PUMP_VISIBLE,              // 0x331401bc, 多泵是否使用
  p_PP_OBJ_SINGLE_PUMP_VISIBLE,            // 0x331401bd, 单泵是否使用
  p_PP_OBJ_VICE_STR_STOP_ENABLE,           // 0x331401be, 泵启停速度是否可用
  p_PP_OBJ_PUMP_ADJ_2TO7_ENABLE,           // 0x331401bf, 泵浦2-7是否使用
  p_PP_OBJ_obj448,                         // 0x331401c0, 1
  p_PP_OBJ_obj449,                         // 0x331401c1, 1
  p_PP_OBJ_obj450,                         // 0x331401c2, 1
  p_PP_OBJ_DLY_TIME_1_ENABLE,              // 0x331401c3, 延迟计时1是否可用
  p_PP_OBJ_ACT_TIME_1_ENABLE,              // 0x331401c4, 动作时间1是否可用
  p_PP_OBJ_START_POS_1_ENABLE,             // 0x331401c5, 起始位置1是否可用
  p_PP_OBJ_END_POS_1_ENABLE,               // 0x331401c6, 终止位置1是否可用
  p_PP_OBJ_DLY_TIME_2_ENABLE,              // 0x331401c7, 延迟计时2是否可用
  p_PP_OBJ_ACT_TIME_2_ENABLE,              // 0x331401c8, 动作时间2是否可用
  p_PP_OBJ_START_POS_2_ENABLE,             // 0x331401c9, 起始位置2是否可用
  p_PP_OBJ_END_POS_2_ENABLE,               // 0x331401ca, 终止位置2是否可用
  p_PP_OBJ_DLY_TIME_3_ENABLE,              // 0x331401cb, 延迟计时3是否可用
  p_PP_OBJ_ACT_TIME_3_ENABLE,              // 0x331401cc, 动作时间3是否可用
  p_PP_OBJ_START_POS_3_ENABLE,             // 0x331401cd, 起始位置3是否可用
  p_PP_OBJ_END_POS_3_ENABLE,               // 0x331401ce, 终止位置3是否可用
  p_PP_OBJ_DLY_TIME_4_ENABLE,              // 0x331401cf, 延迟计时4是否可用
  p_PP_OBJ_ACT_TIME_4_ENABLE,              // 0x331401d0, 动作时间4是否可用
  p_PP_OBJ_START_POS_4_ENABLE,             // 0x331401d1, 起始位置4是否可用
  p_PP_OBJ_END_POS_4_ENABLE,               // 0x331401d2, 终止位置4是否可用
  p_PP_OBJ_DLY_TIME_5_ENABLE,              // 0x331401d3, 延迟计时5是否可用
  p_PP_OBJ_ACT_TIME_5_ENABLE,              // 0x331401d4, 动作时间5是否可用
  p_PP_OBJ_START_POS_5_ENABLE,             // 0x331401d5, 起始位置5是否可用
  p_PP_OBJ_END_POS_5_ENABLE,               // 0x331401d6, 终止位置5是否可用
  p_PP_OBJ_DLY_TIME_6_ENABLE,              // 0x331401d7, 延迟计时6是否可用
  p_PP_OBJ_ACT_TIME_6_ENABLE,              // 0x331401d8, 动作时间6是否可用
  p_PP_OBJ_START_POS_6_ENABLE,             // 0x331401d9, 起始位置6是否可用
  p_PP_OBJ_END_POS_6_ENABLE,               // 0x331401da, 终止位置6是否可用
  p_PP_OBJ_DLY_TIME_7_ENABLE,              // 0x331401db, 延迟计时7是否可用
  p_PP_OBJ_ACT_TIME_7_ENABLE,              // 0x331401dc, 动作时间7是否可用
  p_PP_OBJ_START_POS_7_ENABLE,             // 0x331401dd, 起始位置7是否可用
  p_PP_OBJ_END_POS_7_ENABLE,               // 0x331401de, 终止位置7是否可用
  p_PP_OBJ_DLY_TIME_8_ENABLE,              // 0x331401df, 延迟计时8是否可用
  p_PP_OBJ_ACT_TIME_8_ENABLE,              // 0x331401e0, 动作时间8是否可用
  p_PP_OBJ_START_POS_8_ENABLE,             // 0x331401e1, 起始位置8是否可用
  p_PP_OBJ_END_POS_8_ENABLE,               // 0x331401e2, 终止位置8是否可用
  p_PP_OBJ_DLY_TIME_9_ENABLE,              // 0x331401e3, 延迟计时9是否可用
  p_PP_OBJ_ACT_TIME_9_ENABLE,              // 0x331401e4, 动作时间9是否可用
  p_PP_OBJ_START_POS_9_ENABLE,             // 0x331401e5, 起始位置9是否可用
  p_PP_OBJ_END_POS_9_ENABLE,               // 0x331401e6, 终止位置9是否可用
  p_PP_OBJ_DLY_TIME_10_ENABLE,             // 0x331401e7, 延迟计时10是否可用
  p_PP_OBJ_ACT_TIME_10_ENABLE,             // 0x331401e8, 动作时间10是否可用
  p_PP_OBJ_START_POS_10_ENABLE,            // 0x331401e9, 起始位置10是否可用
  p_PP_OBJ_END_POS_10_ENABLE,              // 0x331401ea, 终止位置10是否可用
  p_PP_OBJ_DLY_TIME_11_ENABLE,             // 0x331401eb, 延迟计时11是否可用
  p_PP_OBJ_ACT_TIME_11_ENABLE,             // 0x331401ec, 动作时间11是否可用
  p_PP_OBJ_START_POS_11_ENABLE,            // 0x331401ed, 起始位置11是否可用
  p_PP_OBJ_END_POS_11_ENABLE,              // 0x331401ee, 终止位置11是否可用
  p_PP_OBJ_DLY_TIME_12_ENABLE,             // 0x331401ef, 延迟计时12是否可用
  p_PP_OBJ_ACT_TIME_12_ENABLE,             // 0x331401f0, 动作时间12是否可用
  p_PP_OBJ_START_POS_12_ENABLE,            // 0x331401f1, 起始位置12是否可用
  p_PP_OBJ_END_POS_12_ENABLE,              // 0x331401f2, 终止位置12是否可用
  p_PP_OBJ_DLY_TIME_13_ENABLE,             // 0x331401f3, 延迟计时13是否可用
  p_PP_OBJ_ACT_TIME_13_ENABLE,             // 0x331401f4, 动作时间13是否可用
  p_PP_OBJ_START_POS_13_ENABLE,            // 0x331401f5, 起始位置13是否可用
  p_PP_OBJ_END_POS_13_ENABLE,              // 0x331401f6, 终止位置13是否可用
  p_PP_OBJ_DLY_TIME_14_ENABLE,             // 0x331401f7, 延迟计时14是否可用
  p_PP_OBJ_ACT_TIME_14_ENABLE,             // 0x331401f8, 动作时间14是否可用
  p_PP_OBJ_START_POS_14_ENABLE,            // 0x331401f9, 起始位置14是否可用
  p_PP_OBJ_END_POS_14_ENABLE,              // 0x331401fa, 终止位置14是否可用
  p_PP_OBJ_SEQ_VLV_1_IMG_VAL,              // 0x331401fb, 顺序阀图片1
  p_PP_OBJ_SEQ_VLV_2_IMG_VAL,              // 0x331401fc, 顺序阀图片2
  p_PP_OBJ_SEQ_VLV_3_IMG_VAL,              // 0x331401fd, 顺序阀图片3
  p_PP_OBJ_SEQ_VLV_4_IMG_VAL,              // 0x331401fe, 顺序阀图片4
  p_PP_OBJ_SEQ_VLV_5_IMG_VAL,              // 0x331401ff, 顺序阀图片5
  p_PP_OBJ_SEQ_VLV_6_IMG_VAL,              // 0x33140200, 顺序阀图片6
  p_PP_OBJ_SEQ_VLV_7_IMG_VAL,              // 0x33140201, 顺序阀图片7
  p_PP_OBJ_SEQ_VLV_8_IMG_VAL,              // 0x33140202, 顺序阀图片8
  p_PP_OBJ_SEQ_VLV_9_IMG_VAL,              // 0x33140203, 顺序阀图片9
  p_PP_OBJ_SEQ_VLV_10_IMG_VAL,             // 0x33140204, 顺序阀图片10
  p_PP_OBJ_SEQ_VLV_11_IMG_VAL,             // 0x33140205, 顺序阀图片11
  p_PP_OBJ_SEQ_VLV_12_IMG_VAL,             // 0x33140206, 顺序阀图片12
  p_PP_OBJ_SEQ_VLV_13_IMG_VAL,             // 0x33140207, 顺序阀图片13
  p_PP_OBJ_SEQ_VLV_14_IMG_VAL,             // 0x33140208, 顺序阀图片14
  p_PP_OBJ_obj521,                         // 0x33140209, 1
  p_PP_OBJ_obj522,                         // 0x3314020a, 1
  p_PP_OBJ_obj523,                         // 0x3314020b, 1
  p_PP_OBJ_1,                              // 0x3314020c, 1
  p_PP_OBJ_2,                              // 0x3314020d, 2
  p_PP_OBJ_3,                              // 0x3314020e, 3
  p_PP_OBJ_SERVO_3_ENABLE,                 // 0x3314020f, 伺服3是否可用
  p_PP_OBJ_obj528,                         // 0x33140210, 1
  p_PP_OBJ_obj529,                         // 0x33140211, 1
  p_PP_OBJ_SYS_PRS_VISIBLE,                // 0x33140212, 系统压力是否可见
  p_PP_OBJ_obj531,                         // 0x33140213, 1
  p_PP_OBJ_INJSLOW_START_ENABLE,           // 0x33140214, 射出射退慢速参数设置是否可以用
  p_PP_OBJ_SPECIAL_DATA_SETUP_VISIBLE,     // 0x33140215, 特殊数据设置是否可见
  p_PP_OBJ_INJSLOW_STARTSPD_DOT,           // 0x33140216, 射出慢速启动速度小数位
  p_PP_OBJ_INJSLOW_STARTSPD_MAX,           // 0x33140217, 射出慢速启动速度最大值
  p_PP_OBJ_IO_EXTEND_1_ENABLE,             // 0x33140218, IO扩展板连接状态1是否可用
  p_PP_OBJ_IO_EXTEND_2_ENABLE,             // 0x33140219, IO扩展板连接状态2是否可用
  p_PP_OBJ_IO_EXTEND_3_ENABLE,             // 0x3314021a, IO扩展板连接状态3是否可用
  p_PP_OBJ_IO_EXTEND_4_ENABLE,             // 0x3314021b, IO扩展板连接状态4是否可用
  p_PP_OBJ_IO_EXTEND_5_ENABLE,             // 0x3314021c, IO扩展板连接状态5是否可用
  p_PP_OBJ_IO_EXTEND_6_ENABLE,             // 0x3314021d, IO扩展板连接状态6是否可用
  p_PP_OBJ_IO_EXTEND_7_ENABLE,             // 0x3314021e, IO扩展板连接状态7是否可用
  p_PP_OBJ_DA_EXTEND_1_ENABLE,             // 0x3314021f, DA扩展板连接状态1是否可用
  p_PP_OBJ_DA_EXTEND_2_ENABLE,             // 0x33140220, DA扩展板连接状态2是否可用
  p_PP_OBJ_DA_EXTEND_3_ENABLE,             // 0x33140221, DA扩展板连接状态3是否可用
  p_PP_OBJ_DA_EXTEND_4_ENABLE,             // 0x33140222, DA扩展板连接状态4是否可用
  p_PP_OBJ_DA_EXTEND_5_ENABLE,             // 0x33140223, DA扩展板连接状态5是否可用
  p_PP_OBJ_DA_EXTEND_6_ENABLE,             // 0x33140224, DA扩展板连接状态6是否可用
  p_PP_OBJ_DA_EXTEND_7_ENABLE,             // 0x33140225, DA扩展板连接状态7是否可用
  p_PP_OBJ_TEMP_EXTEND_1_ENABLE,           // 0x33140226, 温度扩展板连接状态1是否可用
  p_PP_OBJ_TEMP_EXTEND_2_ENABLE,           // 0x33140227, 温度扩展板连接状态2是否可用
  p_PP_OBJ_TEMP_EXTEND_3_ENABLE,           // 0x33140228, 温度扩展板连接状态3是否可用
  p_PP_OBJ_TEMP_EXTEND_4_ENABLE,           // 0x33140229, 温度扩展板连接状态4是否可用
  p_PP_OBJ_TEMP_EXTEND_5_ENABLE,           // 0x3314022a, 温度扩展板连接状态5是否可用
  p_PP_OBJ_TEMP_EXTEND_6_ENABLE,           // 0x3314022b, 温度扩展板连接状态6是否可用
  p_PP_OBJ_TEMP_EXTEND_7_ENABLE,           // 0x3314022c, 温度扩展板连接状态7是否可用
  p_PP_OBJ_MDTP_EXTEND_1_ENABLE,           // 0x3314022d, 模温扩展板连接状态1是否可用
  p_PP_OBJ_MDTP_EXTEND_2_ENABLE,           // 0x3314022e, 模温扩展板连接状态2是否可用
  p_PP_OBJ_MDTP_EXTEND_3_ENABLE,           // 0x3314022f, 模温扩展板连接状态3是否可用
  p_PP_OBJ_MDTP_EXTEND_4_ENABLE,           // 0x33140230, 模温扩展板连接状态4是否可用
  p_PP_OBJ_MDTP_EXTEND_5_ENABLE,           // 0x33140231, 模温扩展板连接状态5是否可用
  p_PP_OBJ_MDTP_EXTEND_6_ENABLE,           // 0x33140232, 模温扩展板连接状态6是否可用
  p_PP_OBJ_MDTP_EXTEND_7_ENABLE,           // 0x33140233, 模温扩展板连接状态7是否可用
  p_PP_OBJ_obj564,                         // 0x33140234, 1
  p_PP_OBJ_obj565,                         // 0x33140235, 1
  p_PP_OBJ_obj566,                         // 0x33140236, 1
  p_PP_OBJ_obj567,                         // 0x33140237, 1
  p_PP_OBJ_SUB_SIGN1_VISIBLE,              // 0x33140238, 减号1是否可见
  p_PP_OBJ_SUB_SIGN2_VISIBLE,              // 0x33140239, 减号2是否可见
  p_PP_OBJ_SUB_SIGN3_VISIBLE,              // 0x3314023a, 减号3是否可见
  p_PP_OBJ_SUB_SIGN4_VISIBLE,              // 0x3314023b, 减号4是否可见
  p_PP_OBJ_SUB_SIGN5_VISIBLE,              // 0x3314023c, 减号5是否可见
  p_PP_OBJ_SUB_SIGN6_VISIBLE,              // 0x3314023d, 减号6是否可见
  p_PP_OBJ_obj574,                         // 0x3314023e, 1
  p_PP_OBJ_obj575,                         // 0x3314023f, 1
  p_PP_OBJ_SERVO_SELF_LEARN_VISIBLE,       // 0x33140240, 伺服学习步骤提示
  p_PP_OBJ_SERVO_ALARM_VISIBLE,            // 0x33140241, 驱动器警报提示
  p_PP_OBJ_obj578,                         // 0x33140242, 1
  p_PP_OBJ_obj579,                         // 0x33140243, 1
  p_PP_OBJ_COOLTM_READ_PROT,               // 0x33140244, 模具冷却时间是否保护
  p_PP_OBJ_COOLTM_BKCOLOR_VAL,             // 0x33140245, 模具冷却时间背景色
  p_PP_OBJ_COOLTM_TXCOLOR_VAL,             // 0x33140246, 模具冷却时间文本色
  p_PP_OBJ_CLAMP_SPD_PORT,                 // 0x33140247, 开关模速度上限是否只读
  p_PP_OBJ_CLAMP_SPD_BKCOLOR_VAL,          // 0x33140248, 开关模速度背景色
  p_PP_OBJ_CLAMP_SPD_TXCOLOR_VAL,          // 0x33140249, 开关模速度文本色
  p_PP_OBJ_HIPRS_PRS_PORT,                 // 0x3314024a, 关模高压压力上限是否只读
  p_PP_OBJ_HIPRS_PRS_BKCOLOR_VAL,          // 0x3314024b, 关模高压压力背景色
  p_PP_OBJ_HIPRS_PRS_TXCOLOR_VAL,          // 0x3314024c, 关模高压压力文本色
  p_PP_OBJ_HIPRS_SPD_PORT,                 // 0x3314024d, 关模高压速度上限是否只读
  p_PP_OBJ_HIPRS_SPD_BKCOLOR_VAL,          // 0x3314024e, 关模高压速度背景色
  p_PP_OBJ_HIPRS_SPD_TXCOLOR_VAL,          // 0x3314024f, 关模高压速度文本色
  p_PP_OBJ_qw,                             // 0x33140250, 1
  p_PP_OBJ_wqe,                            // 0x33140251, 1
  p_PP_OBJ_wq,                             // 0x33140252, 1
  p_PP_OBJ_qwww,                           // 0x33140253, 1
  p_PP_OBJ_sa,                             // 0x33140254, 1
  p_PP_OBJ_sss,                            // 0x33140255, 1
  p_PP_OBJ_sd,                             // 0x33140256, 1
  p_PP_OBJ_asd,                            // 0x33140257, 1
  p_PP_OBJ_CLAMP_IMG_VAL,                  // 0x33140258, 主画面开关模动画
  p_PP_OBJ_CLOSE_BAK_IMG_VAL,              // 0x33140259, 关模背景图片值
  p_PP_OBJ_NOZZLE_BAK_IMG_VAL,             // 0x3314025a, 座台背景图片值
  p_PP_OBJ_INJECT_BAK_IMG_VAL,             // 0x3314025b, 射出背景图片值
  p_PP_OBJ_CHARGE_BAK_IMG_VAL,             // 0x3314025c, 储料背景图片值
  p_PP_OBJ_SUCK_BAK_IMG_VAL,               // 0x3314025d, 射退背景图片值
  p_PP_OBJ_COOL_BAK_IMG_VAL,               // 0x3314025e, 冷却背景图片值
  p_PP_OBJ_OPEN_BAK_IMG_VAL,               // 0x3314025f, 开模背景图片值
  p_PP_OBJ_EJECT_BAK_IMG_VAL,              // 0x33140260, 托模背景图片值
  p_PP_OBJ_CORE_BAK_IMG_VAL,               // 0x33140261, 中子背景图片值
  p_PP_OBJ_AIR_BAK_IMG_VAL,                // 0x33140262, 吹气背景图片值
  p_PP_OBJ_NOZZLE_IMG_VAL,                 // 0x33140263, 座台电子尺是否使用图片
  p_PP_OBJ_obj612,                         // 0x33140264, 1
  p_PP_OBJ_obj613,                         // 0x33140265, 1
  p_PP_OBJ_obj614,                         // 0x33140266, 1
  p_PP_OBJ_obj615,                         // 0x33140267, 1
  p_PP_OBJ_obj616,                         // 0x33140268, 1
  p_PP_OBJ_TEMP6_9_VISIBLE,                // 0x33140269, 温度6-9段是否可见
  p_PP_OBJ_TEMP6_9_ENABLE,                 // 0x3314026a, 参数温度6-9段是否可用
  p_PP_OBJ_TEMP_SET6_9_ENABLE,             // 0x3314026b, 温度设定6-9段是否可用
  p_PP_OBJ_TEMP_MAIN_IMG_VISIBLE,          // 0x3314026c, 主画面摄氏度图标是否可见
  p_PP_OBJ_TEMP1_IMG_VAL,                  // 0x3314026d, 温度一段图片索引
  p_PP_OBJ_TEMP2_IMG_VAL,                  // 0x3314026e, 温度二段图片索引
  p_PP_OBJ_TEMP3_IMG_VAL,                  // 0x3314026f, 温度三段图片索引
  p_PP_OBJ_TEMP4_IMG_VAL,                  // 0x33140270, 温度四段图片索引
  p_PP_OBJ_TEMP5_IMG_VAL,                  // 0x33140271, 温度五段图片索引
  p_PP_OBJ_TEMP6_IMG_VAL,                  // 0x33140272, 温度六段图片索引
  p_PP_OBJ_TEMP7_IMG_VAL,                  // 0x33140273, 温度七段图片索引
  p_PP_OBJ_TEMP8_IMG_VAL,                  // 0x33140274, 温度八段图片索引
  p_PP_OBJ_TEMP9_IMG_VAL,                  // 0x33140275, 温度九段图片索引
  p_PP_OBJ_TEMP_EXTRA1_IMG_VAL,            // 0x33140276, 温度扩展一图片索引
  p_PP_OBJ_TEMP_EXTRA2_IMG_VAL,            // 0x33140277, 温度扩展二图片索引
  p_PP_OBJ_TEMP_EXTRA3_IMG_VAL,            // 0x33140278, 温度扩展三图片索引
  p_PP_OBJ_TEMP_EXTRA4_IMG_VAL,            // 0x33140279, 温度扩展四图片索引
  p_PP_OBJ_TEMP_EXTRA5_IMG_VAL,            // 0x3314027a, 温度扩展五图片索引
  p_PP_OBJ_TEMP_EXTRA6_IMG_VAL,            // 0x3314027b, 温度扩展六图片索引
  p_PP_OBJ_TEMP_EXTRA7_IMG_VAL,            // 0x3314027c, 温度扩展七图片索引
  p_PP_OBJ_TEMP_PART1_ENABLE,              // 0x3314027d, 扩展部件1切换
  p_PP_OBJ_TEMP_PART2_ENABLE,              // 0x3314027e, 扩展部件2切换
  p_PP_OBJ_TEMP_PARAM_EXCHANGE,            // 0x3314027f, 温度参数画面切换
  p_PP_OBJ_a4,                             // 0x33140280, 1
  p_PP_OBJ_a5,                             // 0x33140281, 1
  p_PP_OBJ_a6,                             // 0x33140282, 1
  p_PP_OBJ_aa7,                            // 0x33140283, 1
  p_PP_OBJ_aw,                             // 0x33140284, 1
  p_PP_OBJ_MULTIPLE_ENABLE,                // 0x33140285, 多次选项是否使用
  p_PP_OBJ_BLOW_AGAIN_1_14_ENABLE,         // 0x33140286, 吹气1-14多次是否使用
  p_PP_OBJ_DLYTM_ACTTM_1_14_ENABLE,        // 0x33140287, 1-14延迟计时和动作时间是否使用
  p_PP_OBJ_obj661,                         // 0x33140288, 1
  p_PP_OBJ_obj662,                         // 0x33140289, 1
  p_PP_OBJ_SENSOR_NOZ_UNIT2,               // 0x3314028a, 传感器座台单位切换
  p_PP_OBJ_SENSOR_AD6_UNIT2,               // 0x3314028b, 传感器AD6单位切换
  p_PP_OBJ_obj665,                         // 0x3314028c, 1
  p_PP_OBJ_obj666,                         // 0x3314028d, 1
  p_PP_OBJ_obj667,                         // 0x3314028e, 1
  p_PP_OBJ_obj668,                         // 0x3314028f, 1
  p_PP_OBJ_HOLD_TM_TOTAL_VAL,              // 0x33140290, 阶梯曲线时间总
  p_PP_OBJ_HOLD_TM1_VAL,                   // 0x33140291, 阶梯曲线时间1
  p_PP_OBJ_HOLD_TM2_VAL,                   // 0x33140292, 阶梯曲线时间2
  p_PP_OBJ_HOLD_TM3_VAL,                   // 0x33140293, 阶梯曲线时间3
  p_PP_OBJ_HOLD_TM4_VAL,                   // 0x33140294, 阶梯曲线时间4
  p_PP_OBJ_HOLD_TM5_VAL,                   // 0x33140295, 阶梯曲线时间5
  p_PP_OBJ_obj675,                         // 0x33140296, 1
  p_PP_OBJ_obj676,                         // 0x33140297, 1
  p_PP_OBJ_obj677,                         // 0x33140298, 1
  p_PP_OBJ_DATE_POWER_VISIBLE,             // 0x33140299, 日月年功耗是否显示
  p_PP_OBJ_obj679,                         // 0x3314029a, 1
  p_PP_OBJ_EJ_ADV_HOLD_ENABLE,             // 0x3314029b, 托进保持功能是否可用
  p_PP_OBJ_obj681,                         // 0x3314029c, 1
  p_PP_OBJ_VACUUM_ENABLE,                  // 0x3314029d, 真空发射器是否可用
  p_PP_OBJ_obj683,                         // 0x3314029e, 1
  p_PP_OBJ_OIL_CHECK_ENABLE,               // 0x3314029f, 漏油检测是否可用
  p_PP_OBJ_EjAdv1,                         // 0x331402a0, 托进1背景色
  p_PP_OBJ_EjAdv2,                         // 0x331402a1, 托进2背景色
  p_PP_OBJ_EjRet1,                         // 0x331402a2, 托退1背景色
  p_PP_OBJ_EjRet2,                         // 0x331402a3, 托退2背景色
  p_PP_OBJ_Air1,                           // 0x331402a4, 吹气1背景色
  p_PP_OBJ_Air2,                           // 0x331402a5, 吹气2背景色
  p_PP_OBJ_Air3,                           // 0x331402a6, 吹气3背景色
  p_PP_OBJ_Air4,                           // 0x331402a7, 吹气4背景色
  p_PP_OBJ_Air5,                           // 0x331402a8, 吹气5背景色
  p_PP_OBJ_Air6,                           // 0x331402a9, 吹气6背景色
  p_PP_OBJ_Air7,                           // 0x331402aa, 吹气7背景色
  p_PP_OBJ_Air8,                           // 0x331402ab, 吹气8背景色
  p_PP_OBJ_Air9,                           // 0x331402ac, 吹气9背景色
  p_PP_OBJ_Air10,                          // 0x331402ad, 吹气10背景色
  p_PP_OBJ_Air11,                          // 0x331402ae, 吹气11背景色
  p_PP_OBJ_Air12,                          // 0x331402af, 吹气12背景色
  p_PP_OBJ_Air13,                          // 0x331402b0, 吹气13背景色
  p_PP_OBJ_Air14,                          // 0x331402b1, 吹气14背景色
  p_PP_OBJ_Inj1,                           // 0x331402b2, 射出1段背景
  p_PP_OBJ_Inj2,                           // 0x331402b3, 射出2段背景
  p_PP_OBJ_Inj3,                           // 0x331402b4, 射出3段背景
  p_PP_OBJ_Inj4,                           // 0x331402b5, 射出4段背景
  p_PP_OBJ_Inj5,                           // 0x331402b6, 射出5段背景
  p_PP_OBJ_Inj6,                           // 0x331402b7, 射出6段背景
  p_PP_OBJ_Hold1,                          // 0x331402b8, 保压1段背景
  p_PP_OBJ_Hold2,                          // 0x331402b9, 保压2段背景
  p_PP_OBJ_Hold3,                          // 0x331402ba, 保压3段背景
  p_PP_OBJ_Hold4,                          // 0x331402bb, 保压4段背景
  p_PP_OBJ_Hold5,                          // 0x331402bc, 保压5段背景
  p_PP_OBJ_Charge1,                        // 0x331402bd, 储料1段背景
  p_PP_OBJ_Charge2,                        // 0x331402be, 储料2段背景
  p_PP_OBJ_Charge3,                        // 0x331402bf, 储料3段背景
  p_PP_OBJ_Charge4,                        // 0x331402c0, 储料4段背景
  p_PP_OBJ_SuckBack,                       // 0x331402c1, 射退背景
  p_PP_OBJ_CoreAIn,                        // 0x331402c2, 中子A进背景
  p_PP_OBJ_CoreAOut,                       // 0x331402c3, 中子A退背景
  p_PP_OBJ_CoreBIn,                        // 0x331402c4, 中子B进背景
  p_PP_OBJ_CoreBOut,                       // 0x331402c5, 中子B退背景
  p_PP_OBJ_CoreCIn,                        // 0x331402c6, 中子C进背景
  p_PP_OBJ_CoreCOut,                       // 0x331402c7, 中子C退背景
  p_PP_OBJ_CoreDIn,                        // 0x331402c8, 中子D进背景
  p_PP_OBJ_CoreDOut,                       // 0x331402c9, 中子D退背景
  p_PP_OBJ_CoreEIn,                        // 0x331402ca, 中子E进背景
  p_PP_OBJ_CoreEOut,                       // 0x331402cb, 中子E退背景
  p_PP_OBJ_CoreFIn,                        // 0x331402cc, 中子F进背景
  p_PP_OBJ_CoreFOut,                       // 0x331402cd, 中子F退背景
  p_PP_OBJ_NOZADVFAST,                     // 0x331402ce, 座进快速背景
  p_PP_OBJ_NOZADVSLOW,                     // 0x331402cf, 座进慢速背景
  p_PP_OBJ_NOZRET,                         // 0x331402d0, 座退背景
  p_PP_OBJ_ADJFAST,                        // 0x331402d1, 调模快速背景
  p_PP_OBJ_ADJSLOW,                        // 0x331402d2, 调模慢速背景
  p_PP_OBJ_ADJRET,                         // 0x331402d3, 调模退背景
  p_PP_OBJ_PRS_175,                        // 0x331402d4, 1
  p_PP_OBJ_PRS_190,                        // 0x331402d5, 1
  p_PP_OBJ_INJFENTIENABLE,                 // 0x331402d6, 1
  p_PP_OBJ_obj740,                         // 0x331402d7, 1
  p_PP_OBJ_obj741,                         // 0x331402d8, 1
  p_PP_OBJ_obj742,                         // 0x331402d9, 1
  p_PP_OBJ_TEMPCTRL1_STATUS1_VAL,          // 0x331402da, 温度控制1温度状态1
  p_PP_OBJ_TEMPCTRL1_STATUS2_VAL,          // 0x331402db, 温度控制1温度状态2
  p_PP_OBJ_TEMPCTRL1_STATUS3_VAL,          // 0x331402dc, 温度控制1温度状态3
  p_PP_OBJ_TEMPCTRL1_STATUS4_VAL,          // 0x331402dd, 温度控制1温度状态4
  p_PP_OBJ_TEMPCTRL1_STATUS5_VAL,          // 0x331402de, 温度控制1温度状态5
  p_PP_OBJ_TEMPCTRL1_STATUS6_VAL,          // 0x331402df, 温度控制1温度状态6
  p_PP_OBJ_TEMPCTRL1_STATUS7_VAL,          // 0x331402e0, 温度控制1温度状态7
  p_PP_OBJ_TEMPCTRL1_STATUS8_VAL,          // 0x331402e1, 温度控制1温度状态8
  p_PP_OBJ_TEMPCTRL1_STATUS9_VAL,          // 0x331402e2, 温度控制1温度状态9
  p_PP_OBJ_TEMPCTRL2_STATUS1_VAL,          // 0x331402e3, 温度控制2温度状态1
  p_PP_OBJ_TEMPCTRL2_STATUS2_VAL,          // 0x331402e4, 温度控制2温度状态2
  p_PP_OBJ_TEMPCTRL2_STATUS3_VAL,          // 0x331402e5, 温度控制2温度状态3
  p_PP_OBJ_TEMPCTRL2_STATUS4_VAL,          // 0x331402e6, 温度控制2温度状态4
  p_PP_OBJ_TEMPCTRL2_STATUS5_VAL,          // 0x331402e7, 温度控制2温度状态5
  p_PP_OBJ_TEMPCTRL2_STATUS6_VAL,          // 0x331402e8, 温度控制2温度状态6
  p_PP_OBJ_TEMPCTRL2_STATUS7_VAL,          // 0x331402e9, 温度控制2温度状态7
  p_PP_OBJ_TEMPCTRL2_STATUS8_VAL,          // 0x331402ea, 温度控制2温度状态8
  p_PP_OBJ_TEMPCTRL2_STATUS9_VAL,          // 0x331402eb, 温度控制2温度状态9
  p_PP_OBJ_NORMAL_TEMP1_IMG_VAL,           // 0x331402ec, 普通温度状态图片1
  p_PP_OBJ_NORMAL_TEMP2_IMG_VAL,           // 0x331402ed, 普通温度状态图片2
  p_PP_OBJ_NORMAL_TEMP3_IMG_VAL,           // 0x331402ee, 普通温度状态图片3
  p_PP_OBJ_NORMAL_TEMP4_IMG_VAL,           // 0x331402ef, 普通温度状态图片4
  p_PP_OBJ_NORMAL_TEMP5_IMG_VAL,           // 0x331402f0, 普通温度状态图片5
  p_PP_OBJ_NORMAL_TEMP6_IMG_VAL,           // 0x331402f1, 普通温度状态图片6
  p_PP_OBJ_NORMAL_TEMP7_IMG_VAL,           // 0x331402f2, 普通温度状态图片7
  p_PP_OBJ_NORMAL_TEMP8_IMG_VAL,           // 0x331402f3, 普通温度状态图片8
  p_PP_OBJ_NORMAL_TEMP9_IMG_VAL,           // 0x331402f4, 普通温度状态图片9
  p_PP_OBJ_MOUTH_VISIBLE,                  // 0x331402f5, 落料口是否可见
  p_PP_OBJ_obj771,                         // 0x331402f6, 1
  p_PP_OBJ_obj772,                         // 0x331402f7, 1
  p_PP_OBJ_OPEN_2_ENABLE,                  // 0x331402f8, 开模二段可用
  p_PP_OBJ_OPEN_3_ENABLE,                  // 0x331402f9, 开模三段可用
  p_PP_OBJ_OPEN_4_ENABLE,                  // 0x331402fa, 开模四段可用
  p_PP_OBJ_BARRELCOOLBIAS_ENABLE,          // 0x331402fb, 料管冷却偏差可用
  p_PP_OBJ_BARRELTEMPBIAS_ENABLE,          // 0x331402fc, 料管温度偏差可用
  p_PP_OBJ_TEMPCTRLPART0_SHOW,             // 0x331402fd, 温度部件为0时可见
  p_PP_OBJ_TEMPCTRLPART1_SHOW,             // 0x331402fe, 温度部件为1时可见
  p_PP_OBJ_TEMPCTRLPART2_SHOW,             // 0x331402ff, 温度部件为2时可见
  p_PP_OBJ_IORD_INPUT_MAX,                 // 0x33140300, 输入点最大值
  p_PP_OBJ_IORD_OUTPUT_MAX,                // 0x33140301, 输出点最大值
  p_PP_OBJ_MODEL5200_SHOW,                 // 0x33140302, 5200可见
  p_PP_OBJ_INJ_AND_HOLD_TM_MAX,            // 0x33140303, 射出和保压时间最大值
  p_PP_OBJ_ADPOSI_NOZ_MAX,                 // 0x33140304, 座台位置最大值
  p_PP_OBJ_NEEDLEVLV_VISIBLE,              // 0x33140305, 胶口针阀是否可见
  p_PP_OBJ_NEEDLEVLV_ONPOS_ENABLE,         // 0x33140306, 胶阀开发位置是否可用
  p_PP_OBJ_obj788,                         // 0x33140307, 1
  p_PP_OBJ_PID_OPEN_START_MAX,             // 0x33140308, 开关模启动位置最大值
  p_PP_OBJ_PID_OPEN_START_DOT,             // 0x33140309, 开关模启动位置小数位
  p_PP_OBJ_PID_OPEN_END_MIN,               // 0x3314030a, 开模终止位置最小值
  p_PP_OBJ_CLS3_PUSH_SHOW,                 // 0x3314030b, 关模三段十字头可见
  p_PP_OBJ_CLSH1_PUSH_SHOW,                // 0x3314030c, 高压锁模十字头可见
  p_PP_OBJ_OPN4_PUSH_SHOW,                 // 0x3314030d, 开模四段十字头可见
  p_PP_OBJ_OPN3_PUSH_SHOW,                 // 0x3314030e, 开模三段十字头可见
  p_PP_OBJ_OPN2_PUSH_SHOW,                 // 0x3314030f, 开模二段十字头可见
  p_PP_OBJ_OPNMD_MIN_POS,                  // 0x33140310, 开模位置最小值
  p_PP_OBJ_BREAKMD_MAX_POS,                // 0x33140311, 破模位置最大值
  p_PP_OBJ_PER_SLOWDIST_SHOW,              // 0x33140312, 外设慢速距离可见
  p_PP_OBJ_CLSMDPIC,                       // 0x33140313, 智能关模背景色
  p_PP_OBJ_OPN4_TEXT,                      // 0x33140314, 开模四段文本
  p_PP_OBJ_OPN3_TEXT,                      // 0x33140315, 开模三段文本
  p_PP_OBJ_OPNFASTPIC,                     // 0x33140316, 开模快速背景色
  p_PP_OBJ_OPN1XPIC,                       // 0x33140317, 开模一段文本色
  p_PP_OBJ_OUTPUTCYCLE_MIN,                // 0x33140318, 输出周期最小值
  p_PP_OBJ_SERVO_FUNC_CURVE_SHOW,          // 0x33140319, 驱动器FUNC曲线显示可见
  p_PP_OBJ_PUMPCHANNEL_TEXT,               // 0x3314031a, 油泵压力传感器AD通道显示文本
  p_PP_OBJ_INJ_NOZCHNL_SHOW,               // 0x3314031b, 射出页面座台通道号可见
  p_PP_OBJ_CLSHIPRS2_MAX,                  // 0x3314031c, 关模高压2段最大值
  p_PP_OBJ_AI_CLAMP_OPEN_RAMP_SHOW,        // 0x3314031d, 开模到位减速计算斜率可见
  p_PP_OBJ_MAX_REV_SPD_MIN,                // 0x3314031e, 最大反向速度最小值
  p_PP_OBJ_INJ_PRS_2TO6_SHOW,              // 0x3314031f, 射出压力2-6段可见
  p_PP_OBJ_HOLD_SPD_2TO5_SHOW,             // 0x33140320, 保压速度2-5段可见
  p_PP_OBJ_OPENLOOP_OVERADJ_SHOW,          // 0x33140321, 开环压力超调可见
  p_PP_OBJ_ACT_POS_IN2_ENABLE,             // 0x33140322, 动作位置IN2是否可用
  p_PP_OBJ_ACT_POS_OUT2_ENABLE,            // 0x33140323, 动作位置OUT2是否可用
  p_PP_OBJ_COUNT_TIME2_VISABLE,            // 0x33140324, 计数时间2是否可见
  p_PP_OBJ_TRAVEL_TIME2_VISIBLE,           // 0x33140325, 动作位置IN2是否可用
  p_PP_OBJ_INJ_PRS_DOT,                    // 0x33140326, 射出压力小数位
  p_PP_OBJ_INJ_PRS_MAX,                    // 0x33140327, 射出压力最大值
  p_PP_OBJ_HLD_PRS_MAX,                    // 0x33140328, 保压压力最大值
  p_PP_OBJ_EJTRET2ENDPOS_MAX,              // 0x33140329, 托模二段终止位置最大值
  p_PP_OBJ_EJTRET2FIRSTPOS_MAX,            // 0x3314032a, 托模二段首次位置最大值
  p_PP_OBJ_OPENLINKPOS_MIN,                // 0x3314032b, 开模联动位置最小值
  p_PP_OBJ_INJ1TIME_ENABLE,                // 0x3314032c, 射出一段时间可用
  p_PP_OBJ_EJTADVENDPOS_MAX,               // 0x3314032d, 托进终止位置最大值
  p_PP_OBJ_OPENDIFFENDPOS_SHOW,            // 0x3314032e, 开模差动结束位置
  p_PP_OBJ_HARDWARE_LOCK_SHOW,             // 0x3314032f, 硬件锁图标可见
  p_PP_OBJ_SOFTWARE_LOCK_SHOW,             // 0x33140330, 软件锁图标可见
  p_PP_OBJ_SERVO5_HIDE,                    // 0x33140331, 伺服5隐藏
  p_PP_OBJ_SPDRATE_MAX,                    // 0x33140332, 开模关模射出速度使用率最大值
  p_PP_OBJ_AD5AD6_SHOW,                    // 0x33140333, AD5AD6可见
  p_PP_OBJ_CHG_2_ENABLE_SHOW,              // 0x33140334, 储料2段是否使用-储料画面使用
  p_PP_OBJ_CHG_1_ENABLE,                   // 0x33140335, 储料1段是否是当前选择段数-DA画面使用
  p_PP_OBJ_CHG_1_POS_MAX,                  // 0x33140336, 储料1段终止位置最大值
  p_PP_OBJ_INJECT_REALTM_2_ENABLE,         // 0x33140337, 射出2段实际时间是否可用
  p_PP_OBJ_INJECT_REALTM_3_ENABLE,         // 0x33140338, 射出3段实际时间是否可用
  p_PP_OBJ_INJECT_REALTM_4_ENABLE,         // 0x33140339, 射出4段实际时间是否可用
  p_PP_OBJ_INJECT_REALTM_5_ENABLE,         // 0x3314033a, 射出5段实际时间是否可用
  p_PP_OBJ_INJECT_REALTM_6_ENABLE,         // 0x3314033b, 射出6段实际时间是否可用
  p_PP_OBJ_INJECT_LINE1_VISIBLE,           // 0x3314033c, 射出总时间设定值对应直线1是否可见
  p_PP_OBJ_INJECT_LINE2_VISIBLE,           // 0x3314033d, 射出总时间设定值对应直线2是否可见
  p_PP_OBJ_TOKEEPWARM_VISIBLE,             // 0x3314033e, 自动转保温时间是否显示
  p_PP_OBJ_HOLD_PARAM_VISABLE,             // 0x3314033f, 保压参数（压力、速度、位置）是否可见
  p_PP_OBJ_COREACE_POINT_IN,               // 0x33140340, 中子A/C/E进点位信号背景色
  p_PP_OBJ_COREACE_POINT_OUT,              // 0x33140341, 中子A/C/E退点位信号背景色
  p_PP_OBJ_COREBDF_POINT_IN,               // 0x33140342, 中子B/D/F进点位信号背景色
  p_PP_OBJ_COREBDF_POINT_OUT,              // 0x33140343, 中子B/D/F退点位信号背景色
  p_PP_OBJ_ACT_POINT_IN_ENABLE,            // 0x33140344, 动作点IN是否可用
  p_PP_OBJ_ACT_POINT_OUT_ENABLE,           // 0x33140345, 动作点OUT是否可用
  p_PP_OBJ_ACT_POINT_IN_ENABLE2,           // 0x33140346, 动作点2IN是否可用
  p_PP_OBJ_ACT_POINT_OUT_ENABLE2,          // 0x33140347, 动作点2OUT是否可用
  p_PP_OBJ_COREIN1_POS_READONLY,           // 0x33140348, 中子进位置是否可读-开模连动选择该中子时可读
  p_PP_OBJ_COREOUT1_POS_READONLY,          // 0x33140349, 中子退位置是否可读-开模连动选择该中子时可读
  p_PP_OBJ_COREIN2_POS_READONLY,           // 0x3314034a, 中子进2位置是否可读-开模连动选择该中子时可读
  p_PP_OBJ_COREOUT2_POS_READONLY,          // 0x3314034b, 中子退2位置是否可读-开模连动选择该中子时可读
  p_PP_OBJ_COREIN1_POS_BKCOLOR,            // 0x3314034c, 中子进位置背景色-开模连动选择该中子时可读
  p_PP_OBJ_COREOUT1_POS_BKCOLOR,           // 0x3314034d, 中子退位置背景色-开模连动选择该中子时可读
  p_PP_OBJ_COREIN2_POS_BKCOLOR,            // 0x3314034e, 中子进2位置背景色-开模连动选择该中子时可读
  p_PP_OBJ_COREOUT2_POS_BKCOLOR,           // 0x3314034f, 中子退2位置背景色-开模连动选择该中子时可读
  p_PP_OBJ_REMELT_POS_ENABLE,              // 0x33140350, 再次储料位置是否可见
  p_PP_OBJ_HEATER_WARMCHECK_VISIBLE,       // 0x33140351, 电热连续加温检查度数是否可见
  p_PP_OBJ_RBT_OUTPUT_TM_VISIBLE,          // 0x33140352, 途中机械手输出时间是否可见-通用厂家可见
  p_PP_OBJ_IAP_FUNC_VISIBLE,               // 0x33140353, IAP功能是否显示
  p_PP_OBJ_OPEN1_POS_MAX_UI32,             // 0x33140354, 开模一段位置最大值
  p_PP_OBJ_BREAKMD_MAX_POS_UI32,           // 0x33140355, 破模位置最大值
  p_PP_OBJ_CLS_LOW_PRS_POS_MAX_UI32,       // 0x33140356, 关模低压位置最大值
  p_PP_OBJ_CLS_HI2_SPD_MAX,                // 0x33140357, 关模高压二段速度最大值
  p_PP_OBJ_CLS_HI2_POS_MIN                 // 0x33140358, 关模高压二段位置最小值
};

enum db_PP_MACH2{
  p_PP_MACH2_AlarmState = 0x33170000, // 0x33170000, 自动警报状态
  p_PP_MACH2_RampClsSpd,              // 0x33170001, 关模速度斜率选择
  p_PP_MACH2_RampOpenSpd,             // 0x33170002, 开模速度斜率选择
  p_PP_MACH2_RampInjPrs,              // 0x33170003, 射出压力斜率选择
  p_PP_MACH2_RampInjSpd,              // 0x33170004, 射出速度斜率选择
  p_PP_MACH2_RampHoldPrs,             // 0x33170005, 保压压力斜率选择
  p_PP_MACH2_RampHoldSpd,             // 0x33170006, 保压速度斜率选择
  p_PP_MACH2_RampChgPrs,              // 0x33170007, 储料背压斜率选择
  p_PP_MACH2_RampChgSpd,              // 0x33170008, 储料速度斜率选择
  p_PP_MACH2_SWCOREAINCLOSE,          // 0x33170009, 中子A进位置模式
  p_PP_MACH2_SWCOREAOUTOPEN,          // 0x3317000a, 中子A退位置模式
  p_PP_MACH2_SWCOREBINCLOSE,          // 0x3317000b, 中子B进位置模式
  p_PP_MACH2_SWCOREBOUTOPEN,          // 0x3317000c, 中子B退位置模式
  p_PP_MACH2_SWCORECINCLOSE,          // 0x3317000d, 中子C进位置模式
  p_PP_MACH2_SWCORECOUTOPEN,          // 0x3317000e, 中子C退位置模式
  p_PP_MACH2_SWCOREDINCLOSE,          // 0x3317000f, 中子D进位置模式
  p_PP_MACH2_SWCOREDOUTOPEN,          // 0x33170010, 中子D退位置模式
  p_PP_MACH2_SWCOREEINCLOSE,          // 0x33170011, 中子E进位置模式
  p_PP_MACH2_SWCOREEOUTOPEN,          // 0x33170012, 中子E退位置模式
  p_PP_MACH2_SWCOREFINCLOSE,          // 0x33170013, 中子F进位置模式
  p_PP_MACH2_SWCOREFOUTOPEN,          // 0x33170014, 中子F退位置模式
  p_PP_MACH2_PositionSel,             // 0x33170015, 距离位置单位选择
  p_PP_MACH2_TempSel,                 // 0x33170016, 温度单位选择
  p_PP_MACH2_InjPrsSel,               // 0x33170017, 射出压力单位选择
  p_PP_MACH2_ClsPrsSel,               // 0x33170018, 锁模力单位选择
  p_PP_MACH2_InjSpdSel,               // 0x33170019, 射出速度单位选择
  p_PP_MACH2_VoltageSel,              // 0x3317001a, 电压单位选择
  p_PP_MACH2_UserPassword1,           // 0x3317001b, 登录密码
  p_PP_MACH2_UserPassword2,           // 0x3317001c, 登录密码
  p_PP_MACH2_UserPassword3,           // 0x3317001d, 登录密码
  p_PP_MACH2_UserPassword4,           // 0x3317001e, 登录密码
  p_PP_MACH2_UserPassword5,           // 0x3317001f, 登录密码
  p_PP_MACH2_UserPassword6,           // 0x33170020, 登录密码
  p_PP_MACH2_UserPassword7,           // 0x33170021, 登录密码
  p_PP_MACH2_UserPassword8,           // 0x33170022, 登录密码
  p_PP_MACH2_UserPassword9,           // 0x33170023, 登录密码
  p_PP_MACH2_UserPassword10,          // 0x33170024, 登录密码
  p_PP_MACH2_UserInitialLv,           // 0x33170025, 开机初始等级选择
  p_PP_MACH2_TempMaxLimit,            // 0x33170026, 温度上限值限制
  p_PP_MACH2_ConnectAttention,        // 0x33170027, 连接块提示
  p_PP_MACH2_UserPwdMode,             // 0x33170028, 用户登录密码方式选择:0:固定密码,1:动态密码
  p_PP_MACH2_JKEY_PRESS_IMG1,         // 0x33170029, 
  p_PP_MACH2_JKEY_PRESS_IMG2,         // 0x3317002a, 
  p_PP_MACH2_JKEY_PRESS_IMG3,         // 0x3317002b, 
  p_PP_MACH2_JKEY_PRESS_IMG4,         // 0x3317002c, 
  p_PP_MACH2_JKEY_PRESS_IMG5,         // 0x3317002d, 
  p_PP_MACH2_JKEY_PRESS_IMG6,         // 0x3317002e, 
  p_PP_MACH2_JKEY_PRESS_IMG7,         // 0x3317002f, 
  p_PP_MACH2_JKEY_PRESS_IMG8,         // 0x33170030, 
  p_PP_MACH2_JKEY_PRESS_IMG9,         // 0x33170031, 
  p_PP_MACH2_JKEY_PRESS_IMG10,        // 0x33170032, 
  p_PP_MACH2_JKEY_RELEASE_IMG1,       // 0x33170033, 
  p_PP_MACH2_JKEY_RELEASE_IMG2,       // 0x33170034, 
  p_PP_MACH2_JKEY_RELEASE_IMG3,       // 0x33170035, 
  p_PP_MACH2_JKEY_RELEASE_IMG4,       // 0x33170036, 
  p_PP_MACH2_JKEY_RELEASE_IMG5,       // 0x33170037, 
  p_PP_MACH2_JKEY_RELEASE_IMG6,       // 0x33170038, 
  p_PP_MACH2_JKEY_RELEASE_IMG7,       // 0x33170039, 
  p_PP_MACH2_JKEY_RELEASE_IMG8,       // 0x3317003a, 
  p_PP_MACH2_JKEY_RELEASE_IMG9,       // 0x3317003b, 
  p_PP_MACH2_JKEY_RELEASE_IMG10,      // 0x3317003c, 
  p_PP_MACH2_JKEY_RELEASE_MAIN,       // 0x3317003d, 
  p_PP_MACH2_JKEY_RELEASE_PAGE,       // 0x3317003e, 
  p_PP_MACH2_JKEY_RELEASE_HELP,       // 0x3317003f, 
  p_PP_MACH2_PARLIMIT_CHANGE_FLAG,    // 0x33170040, 参数上限页面数据修改标记
  p_PP_MACH2_MACHCFG2_CHANGE_FLAG,    // 0x33170041, 机器配置2页面数据修改标记
  p_PP_MACH2_CLSPRSTB_CHANGE_FLAG,    // 0x33170042, 锁模力表设置页面数据修改标记
  p_PP_MACH2_AD_PAGE_FLAGE,           // 0x33170043, ad画面切换出去提示
  p_PP_MACH2_OptionalShow,            // 0x33170044, 选购设置显示
  p_PP_MACH2_ToatlOpenCountNoClear,   // 0x33170045, 开模总数不清零
  p_PP_MACH2_ST_wShotCnt_NoClear,     // 0x33170046, ST_开模数1不清零
  p_PP_MACH2_ST_wShotCnt2_NoClear,    // 0x33170047, ST_开模数不清零
  p_PP_MACH2_Resever,                 // 0x33170048, 预留
  p_PP_MACH2_SCREEN_PT15_SHOT,        // 0x33170049, 截屏功能
  p_PP_MACH2_InjectTimeLast,          // 0x3317004a, 上一模射出计时
  p_PP_MACH2_CoolTimeLst,             // 0x3317004b, 上一模冷却计时
  p_PP_MACH2_USER_KEYLOCK,            // 0x3317004c, 生产人员等级锁
  p_PP_MACH2_USER_STATE,              // 0x3317004d, 自动登出刷新标志
  p_PP_MACH2_BUZZER_SOUND,            // 0x3317004e, 蜂鸣器声音
  p_PP_MACH2_HardWareVers1,           // 0x3317004f, 单片机版本号1
  p_PP_MACH2_HardWareVers2,           // 0x33170050, 单片机版本号2
  p_PP_MACH2_Bright,                  // 0x33170051, 背关亮度
  p_PP_MACH2_BrightnessAuto,          // 0x33170052, 背关亮度自动调整
  p_PP_MACH2_MOLD2_USE,               // 0x33170053, 模温2是否使用
  p_PP_MACH2_PROD_MONI_COMBO_1,       // 0x33170054, 成品监控下拉框1值
  p_PP_MACH2_PROD_MONI_COMBO_2,       // 0x33170055, 成品监控下拉框2值
  p_PP_MACH2_PROD_MONI_COMBO_3,       // 0x33170056, 成品监控下拉框3值
  p_PP_MACH2_PROD_MONI_COMBO_4,       // 0x33170057, 成品监控下拉框4值
  p_PP_MACH2_PROD_MONI_COMBO_5,       // 0x33170058, 成品监控下拉框5值
  p_PP_MACH2_PROD_MONI_COMBO_6,       // 0x33170059, 成品监控下拉框6值
  p_PP_MACH2_PROD_MONI_COMBO_7,       // 0x3317005a, 成品监控下拉框7值
  p_PP_MACH2_PROD_MONI_COMBO_8,       // 0x3317005b, 成品监控下拉框8值
  p_PP_MACH2_PROD_MONI_COMBO_9,       // 0x3317005c, 成品监控下拉框9值
  p_PP_MACH2_PROD_MONI_COMBO_10,      // 0x3317005d, 成品监控下拉框10值
  p_PP_MACH2_CURVE_CHECKTIME,         // 0x3317005e, 检测时间
  p_PP_MACH2_INJ_SPEED_MIN,           // 0x3317005f, 射出曲线速度下限
  p_PP_MACH2_INJ_SPEED_MAX,           // 0x33170060, 射出曲线速度上限
  p_PP_MACH2_INJ_TORQUE_MIN,          // 0x33170061, 射出曲线扭矩下限
  p_PP_MACH2_INJ_TORQUE_MAX,          // 0x33170062, 射出曲线扭矩上限
  p_PP_MACH2_INJ_PRESS_MIN,           // 0x33170063, 射出曲线压力下限
  p_PP_MACH2_INJ_PRESS_MAX,           // 0x33170064, 射出曲线压力上限
  p_PP_MACH2_INJ_POS_MIN,             // 0x33170065, 射出曲线位置下限
  p_PP_MACH2_INJ_POS_MAX,             // 0x33170066, 射出曲线位置上限
  p_PP_MACH2_INJ_TIME_MIN,            // 0x33170067, 射出曲线计时下限
  p_PP_MACH2_INJ_TIME_MAX,            // 0x33170068, 射出曲线计时上限
  p_PP_MACH2_INJCURVE_UPDATE_FLAG,    // 0x33170069, 射出曲线刷新标记
  p_PP_MACH2_CHG_SPEED_MIN,           // 0x3317006a, 储料曲线速度下限
  p_PP_MACH2_CHG_SPEED_MAX,           // 0x3317006b, 储料曲线速度上限
  p_PP_MACH2_CHG_TORQUE_MIN,          // 0x3317006c, 储料曲线扭矩下限
  p_PP_MACH2_CHG_TORQUE_MAX,          // 0x3317006d, 储料曲线扭矩上限
  p_PP_MACH2_CHG_SETPRS_MIN,          // 0x3317006e, 储料曲线设定压力下限
  p_PP_MACH2_CHG_SETPRS_MAX,          // 0x3317006f, 储料曲线设定压力上限
  p_PP_MACH2_CHG_PRS_MIN,             // 0x33170070, 储料曲线实际压力下限
  p_PP_MACH2_CHG_PRS_MAX,             // 0x33170071, 储料曲线实际压力上限
  p_PP_MACH2_CHG_TIME_MIN,            // 0x33170072, 储料曲线计时下限
  p_PP_MACH2_CHG_TIME_MAX,            // 0x33170073, 储料曲线计时上限
  p_PP_MACH2_CHGCURVE_UPDATE_FLAG,    // 0x33170074, 储料曲线刷新标记
  p_PP_MACH2_CLAMP_SPEED_MIN,         // 0x33170075, 开关模曲线速度下限
  p_PP_MACH2_CLAMP_SPEED_MAX,         // 0x33170076, 开关模曲线速度上限
  p_PP_MACH2_CLAMP_CMDSPD_MIN,        // 0x33170077, 开关模曲线指令速度下限
  p_PP_MACH2_CLAMP_CMDSPD_MAX,        // 0x33170078, 开关模曲线指令速度上限
  p_PP_MACH2_CLAMP_TORQUE_MIN,        // 0x33170079, 开关模曲线扭矩下限
  p_PP_MACH2_CLAMP_TORQUE_MAX,        // 0x3317007a, 开关模曲线扭矩上限
  p_PP_MACH2_CLAMP_POS_MIN,           // 0x3317007b, 开关模曲线位置下限
  p_PP_MACH2_CLAMP_POS_MAX,           // 0x3317007c, 开关模曲线位置上限
  p_PP_MACH2_CLAMP_TIME_MIN,          // 0x3317007d, 开关模曲线计时下限
  p_PP_MACH2_CLAMP_TIME_MAX,          // 0x3317007e, 开关模曲线计时上限
  p_PP_MACH2_CLAMPCURVE_UPDATE_FLAG,  // 0x3317007f, 开关模曲线刷新标记
  p_PP_MACH2_EJECT_SPEED_MIN,         // 0x33170080, 托模曲线速度下限
  p_PP_MACH2_EJECT_SPEED_MAX,         // 0x33170081, 托模曲线速度上限
  p_PP_MACH2_EJECT_CMDSPD_MIN,        // 0x33170082, 托模曲线指令速度下限
  p_PP_MACH2_EJECT_CMDSPD_MAX,        // 0x33170083, 托模曲线指令速度上限
  p_PP_MACH2_EJECT_TORQUE_MIN,        // 0x33170084, 托模曲线扭矩下限
  p_PP_MACH2_EJECT_TORQUE_MAX,        // 0x33170085, 托模曲线扭矩上限
  p_PP_MACH2_EJECT_POS_MIN,           // 0x33170086, 托模曲线位置下限
  p_PP_MACH2_EJECT_POS_MAX,           // 0x33170087, 托模曲线位置上限
  p_PP_MACH2_EJECT_TIME_MIN,          // 0x33170088, 托模曲线计时下限
  p_PP_MACH2_EJECT_TIME_MAX,          // 0x33170089, 托模曲线计时上限
  p_PP_MACH2_EJECTCURVE_UPDATE_FLAG,  // 0x3317008a, 托模曲线刷新标记
  p_PP_MACH2_TEMPCURVE_SELECT1,       // 0x3317008b, 温度曲线选择1
  p_PP_MACH2_TEMPCURVE_SELECT2,       // 0x3317008c, 温度曲线选择2
  p_PP_MACH2_TEMPCURVE_SELECT3,       // 0x3317008d, 温度曲线选择3
  p_PP_MACH2_TEMPCURVE_SELECT4,       // 0x3317008e, 温度曲线选择4
  p_PP_MACH2_TEMPCURVE_SELECT5,       // 0x3317008f, 温度曲线选择5
  p_PP_MACH2_TEMPCURVE_SELECT6,       // 0x33170090, 温度曲线选择6
  p_PP_MACH2_TEMPCURVE_SELECT7,       // 0x33170091, 温度曲线选择7
  p_PP_MACH2_TEMPCURVE_SELECT8,       // 0x33170092, 温度曲线选择8
  p_PP_MACH2_TEMPCURVE_SELECT9,       // 0x33170093, 温度曲线选择9
  p_PP_MACH2_TEMPCURVE_X_MIN,         // 0x33170094, 温度曲线x轴最小值
  p_PP_MACH2_TEMPCURVE_X_MAX,         // 0x33170095, 温度曲线x轴最大值
  p_PP_MACH2_TEMPCURVE_Y_MIN,         // 0x33170096, 温度曲线y轴最小值
  p_PP_MACH2_TEMPCURVE_Y_MAX,         // 0x33170097, 温度曲线y轴最大值
  p_PP_MACH2_DRIVECURVE_SELECT1,      // 0x33170098, 驱动器温度曲线选择1
  p_PP_MACH2_DRIVECURVE_SELECT2,      // 0x33170099, 驱动器温度曲线选择2
  p_PP_MACH2_DRIVECURVE_SELECT3,      // 0x3317009a, 驱动器温度曲线选择3
  p_PP_MACH2_DRIVECURVE_SELECT4,      // 0x3317009b, 驱动器温度曲线选择4
  p_PP_MACH2_DRIVECURVE_X_MIN,        // 0x3317009c, 驱动器温度曲线x轴最小值
  p_PP_MACH2_DRIVECURVE_X_MAX,        // 0x3317009d, 驱动器温度曲线x轴最大值
  p_PP_MACH2_DRIVECURVE_Y_MIN,        // 0x3317009e, 驱动器温度曲线y轴最小值
  p_PP_MACH2_DRIVECURVE_Y_MAX,        // 0x3317009f, 驱动器温度曲线y轴最大值
  p_PP_MACH2_CAVITYCURVE_KEEPTIME,    // 0x331700a0, 保持时间
  p_PP_MACH2_CAVITYCURVE_X_MIN,       // 0x331700a1, 模腔曲线x轴最小值
  p_PP_MACH2_CAVITYCURVE_X_MAX,       // 0x331700a2, 模腔曲线x轴最大值
  p_PP_MACH2_CAVITYCURVE_Y_MIN,       // 0x331700a3, 模腔曲线y轴最小值
  p_PP_MACH2_CAVITYCURVE_Y_MAX,       // 0x331700a4, 模腔曲线y轴最大值
  p_PP_MACH2_CAVITYCURVE_CURVESELECT1, // 0x331700a5, 模腔曲线曲线选择1
  p_PP_MACH2_CAVITYCURVE_CURVESELECT2, // 0x331700a6, 模腔曲线曲线选择2
  p_PP_MACH2_CAVITYCURVE_CURVESELECT3, // 0x331700a7, 模腔曲线曲线选择3
  p_PP_MACH2_CAVITYCURVE_CURVESELECT4, // 0x331700a8, 模腔曲线曲线选择4
  p_PP_MACH2_CAVITYCURVE_CURVESELECT5, // 0x331700a9, 模腔曲线曲线选择5
  p_PP_MACH2_CAVITYCURVE_HISTORYENABLE, // 0x331700aa, 模腔曲线历史使能
  p_PP_MACH2_AD_ZERO_FLAGE,           // 0x331700ab, ad调零画面切出标志位
  p_PP_MACH2_LOCK_MSG_SHOW,           // 0x331700ac, 锁住后弹出框显示
  p_PP_MACH2_SCREEN_PROTECT,          // 0x331700ad, 是否处于屏保状态
  p_PP_MACH2_WAKE_SCREEN_SHOW,        // 0x331700ae, 唤醒屏保显示
  p_PP_MACH2_ALARM_EXPLAIN,           // 0x331700af, 警报说明记录
  p_PP_MACH2_SERVO_ERR_EXP,           // 0x331700b0, 伺服警报说明记录
  p_PP_MACH2_MACHTYPE_SELECT,         // 0x331700b1, 机型选择选择项
  p_PP_MACH2_HARDWARE_LOCK,           // 0x331700b2, 硬件锁
  p_PP_MACH2_COLDSTRPREVTMMIN,        // 0x331700b3, 冷态启动防止时间最小值
  p_PP_MACH2_CURRENT_PAGE,            // 0x331700b4, 当前页面值
  p_PP_MACH2_USER_LV_SHOW,            // 0x331700b5, 当前用户等级显示
  p_PP_MACH2_ZXUSER_CURID,            // 0x331700b6, 用来记录当前所选的用户号
  p_PP_MACH2_ZXUSER_TOTAL,            // 0x331700b7, 用来记录当前总共的用户号
  p_PP_MACH2_LANGUAGE_SHOW,           // 0x331700b8, 当前语言显示
  p_PP_MACH2_JKEY_COLOR_MAIN,         // 0x331700b9, 主画面文本色
  p_PP_MACH2_JKEY_COLOR_PAGE,         // 0x331700ba, 换页文本色
  p_PP_MACH2_REVIEWTIME_ERR,          // 0x331700bb, 保养维护警报
  p_PP_MACH2_UPDATE_DRIVEPARA,        // 0x331700bc, 标记-用于刷新驱动器参数请求
  p_PP_MACH2_READ_PACK,               // 0x331700bd, 读当前驱动器参数总包数
  p_PP_MACH2_WRITE_PACK,              // 0x331700be, 写当前驱动器参数总包数
  p_PP_MACH2_CUR_PACKTYPE,            // 0x331700bf, 包号类型-用于区别当前请求的包属于哪个画面
  p_PP_MACH2_CUR_OPERATETYPE,         // 0x331700c0, 增加本地导入导出功能 0:无导入导出操作；1:U盘导入导出；2:本地面板导入导出
  p_PP_MACH2_SWSTARTTOTALPRODTIME,    // 0x331700c1, 历史生产累计时间
  p_PP_MACH2_SERVOALARM_EXPLAIN,      // 0x331700c2, 伺服警报说明记录（帮助画面）
  p_PP_MACH2_PRODMONI_COUNT,          // 0x331700c3, 成品监控数据累计10万模,用于监控画面显示序号
  p_PP_MACH2_ROBOTIO_INPUT_NUM,       // 0x331700c4, 机械手输入点数量
  p_PP_MACH2_ROBOTIO_OUTPUT_NUM,      // 0x331700c5, 机械手输出点数量
  p_PP_MACH2_ROBOTIO_INPUT_SN_NUM,    // 0x331700c6, 机械手输入点sn数量
  p_PP_MACH2_ROBOTIO_OUTPUT_SN_NUM,   // 0x331700c7, 机械手输出点sn数量
  p_PP_MACH2_ROBOTIO_INPUT_SN_BAK_NUM, // 0x331700c8, 机械手输入点sn_bak数量
  p_PP_MACH2_ROBOTIO_OUTPUT_SN_BAK_NUM, // 0x331700c9, 机械手输出点sn_bak数量
  p_PP_MACH2_POWER_UNIT_TRANS,        // 0x331700ca, 功耗单位转换
  p_PP_MACH2_FRESH_ROBOTIO,           // 0x331700cb, 刷新机械手IO界面
  p_PP_MACH2_ROBOTIO_CHANGE_FLAG,     // 0x331700cc, 机械手IO页面数据修改标记
  p_PP_MACH2_LISTPHYSN_IN_NUM,        // 0x331700cd, listPhysn_in表个数
  p_PP_MACH2_LISTPHYSN_OUT_NUM,       // 0x331700ce, listPhysn_out表个数
  p_PP_MACH2_InjPosSel,               // 0x331700cf, 射位置单位选择
  p_PP_MACH2_cm3tomm,                 // 0x331700d0, 单位转换cm3tomm
  p_PP_MACH2_in3tomm,                 // 0x331700d1, 单位转换in3tomm
  p_PP_MACH2_MTOA_KEY_USE,            // 0x331700d2, 是否已经按下了自动键
  p_PP_MACH2_MTOA_KEYX,               // 0x331700d3, 记录按下的自动键
  p_PP_MACH2_MTOA_PASSWORD,           // 0x331700d4, 手动切全自动半自动密码
  p_PP_MACH2_SEARCH_PAGE_NAME,        // 0x331700d5, 跳转页面名
  p_PP_MACH2_SAVE_EXCEL_FILE_NAME,    // 0x331700d6, 保存成品监控数据文件名
  p_PP_MACH2_MODE_PICTURE,            // 0x331700d7, 模式图片刷新
  p_PP_MACH2_SYSDATA,                 // 0x331700d8, 主画面年月日
  p_PP_MACH2_MAIN_PICTRUE_1,          // 0x331700d9, 主画面动作图1
  p_PP_MACH2_MAIN_PICTRUE_2,          // 0x331700da, 主画面动作图2
  p_PP_MACH2_MAIN_PICTRUE_3,          // 0x331700db, 主画面动作图3
  p_PP_MACH2_MAIN_PICTRUE_4,          // 0x331700dc, 主画面动作图4
  p_PP_MACH2_MAIN_PICTRUE_5,          // 0x331700dd, 主画面动作图5
  p_PP_MACH2_MAIN_PICTRUE_6,          // 0x331700de, 主画面动作图6
  p_PP_MACH2_MAIN_PICTRUE_7,          // 0x331700df, 主画面动作图7
  p_PP_MACH2_MAIN_PICTRUE_8,          // 0x331700e0, 主画面动作图8
  p_PP_MACH2_MAIN_PICTRUE_9,          // 0x331700e1, 主画面动作图9
  p_PP_MACH2_MAIN_PICTRUE_10,         // 0x331700e2, 主画面动作图10
  p_PP_MACH2_DRIVEOUT_INPUT,          // 0x331700e3, 伺服画面导入导出切换页面标志
  p_PP_MACH2_TEMPCURVE_2_SELECT1,     // 0x331700e4, 温度2曲线选择1
  p_PP_MACH2_TEMPCURVE_2_SELECT2,     // 0x331700e5, 温度2曲线选择2
  p_PP_MACH2_TEMPCURVE_2_SELECT3,     // 0x331700e6, 温度2曲线选择3
  p_PP_MACH2_TEMPCURVE_2_SELECT4,     // 0x331700e7, 温度2曲线选择4
  p_PP_MACH2_TEMPCURVE_2_SELECT5,     // 0x331700e8, 温度2曲线选择5
  p_PP_MACH2_TEMPCURVE_2_SELECT6,     // 0x331700e9, 温度2曲线选择6
  p_PP_MACH2_TEMPCURVE_2_SELECT7,     // 0x331700ea, 温度2曲线选择7
  p_PP_MACH2_TEMPCURVE_2_SELECT8,     // 0x331700eb, 温度2曲线选择8
  p_PP_MACH2_TEMPCURVE_2_SELECT9,     // 0x331700ec, 温度2曲线选择9
  p_PP_MACH2_TEMPCURVE_2_X_MIN,       // 0x331700ed, 温度2曲线x轴最小值
  p_PP_MACH2_TEMPCURVE_2_X_MAX,       // 0x331700ee, 温度2曲线x轴最大值
  p_PP_MACH2_TEMPCURVE_2_Y_MIN,       // 0x331700ef, 温度2曲线y轴最小值
  p_PP_MACH2_TEMPCURVE_2_Y_MAX        // 0x331700f0, 温度2曲线y轴最大值
};

#endif  //_DBENUM_H
