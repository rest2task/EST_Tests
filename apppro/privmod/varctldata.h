#ifndef _VARCTLDATA_H
#define _VARCTLDATA_H

#ifdef __cplusplus
extern "C"
{
#endif

	/**************************托膜界面变量模块-EJ*****************************/
/*#define BLAST_KEY_STR_VAL		    0x33140000     吹气按键选择值*/
#define AIR1_POS_ENABLE		        0x33140001     /*吹气1结束位置是否使用*/
#define AIR2_POS_ENABLE		        0x33140002     /*吹气2结束位置是否使用*/
#define AIR3_POS_ENABLE		        0x33140003     /*吹气3结束位置是否使用*/
#define AIR4_POS_ENABLE		        0x33140004     /*吹气4结束位置是否使用*/
#define AIR5_POS_ENABLE		        0x33140005     /*吹气5结束位置是否使用*/
#define AIR6_POS_ENABLE		        0x33140006     /*吹气6结束位置是否使用*/
#define AIR1_POS_MAX		        0x33140007     /*吹气1结束位置最大值*/
#define AIR2_POS_MAX		        0x33140008     /*吹气2结束位置最大值*/
#define AIR3_POS_MAX		        0x33140009     /*吹气3结束位置最大值*/
#define AIR4_POS_MAX		        0x3314000a     /*吹气4结束位置最大值*/
#define AIR5_POS_MAX		        0x3314000b     /*吹气5结束位置最大值*/
#define AIR6_POS_MAX		        0x3314000c     /*吹气6结束位置最大值*/
#define BEF_DLY_MAX                 0x3314000d     /*托膜前延迟最大值*/
#define BEF_DLY_DOT                 0X3314000e     /*托膜前延迟小数位*/
#define BEF_DLY_LEN                 0x3314000f     /*托膜前延迟字符数*/
#define EJ_COUNT_ENABLE             0x33140010     /*托膜次数是否使用*/
#define VIBRATE_MOD_ENABLE          0x33140011     /*托膜震动模式是否使用*/
#define VIBRATE_TM_ENABLE           0x33140012     /*托膜震动时间/距离是否使用*/
#define FST_TRAVEL_ENABLE           0x33140013     /*首次独立行程是否使用*/
#define VIBRATE_TM_VISIBLE          0x33140014     /*托膜震动时间是否可见*/
#define VIBRATE_DIST_VISIBLE        0x33140015     /*托膜震动距离是否可见*/
#define VIBRATE_TorD_DOT            0x33140016     /*震动托膜时间/位置小数位*/
#define VIBRATE_TorD_MIN            0x33140017     /*震动托膜时间/位置最小值*/
#define VIBRATE_TorD_MAX            0x33140018     /*震动托膜时间/位置最大值*/
#define VIBRATE_TorD_UNIT           0x33140019     /*震动托膜时间/位置单位切换*/
#define VIBRATE_TorD_ADR            0x3314001a     /*震动托膜时间/位置地址切换*/
#define EJ_POS_IMG_VAL              0x3314001b     /*托膜位置图片*/
#define EJ_FWD_PO_IMG_VAL           0x3314001c     /*托进阀图片*/
#define EJ_BWD_PO_IMG_VAL           0x3314001d     /*托退阀图片*/
#define EJ_FWD_PI_IMG_VAL           0x3314001e     /*托膜进终图片*/
#define EJ_BWD_PI_IMG_VAL           0x3314001f     /*托膜退终图片*/
#define CLS_END_PI_IMG_VAL          0x33140020     /*合模到底图片*/

	/************************开关模界面变量模块-CLP*****************************/
#define OPEN1_POS_DOT               0x3314002b     /*开模一段位置小数位*/
#define CLS_LOW_PRS_POS_DOT         0x3314002c     /*关模低压位置小数位*/
#define CLS_HI_PRS_POS_DOT          0x3314002d     /*高压锁模位置小数位*/
#define CLS1_POS_MAX                0x3314002e     /*关模一段位置最大值*/
#define CLS2_POS_MAX                0x3314002f     /*关模二段位置最大值*/
#define CLS2_POS_MIN                0x33140030     /*关模二段位置最小值*/
#define CLS3_POS_MAX                0x33140031     /*关模三段位置最大值*/
#define CLS3_POS_MIN                0x33140032     /*关模三段位置最小值*/
#define CLS_LOW_PRS_POS_MAX         0x33140033     /*关模低压位置最大值*/
#define CLS_LOW_PRS_POS_MIN         0x33140034     /*关模低压位置最小值*/
#define OPEN1_POS_MAX               0x33140035     /*开模一段位置最大值*/
#define OPEN2_POS_MAX               0x33140036     /*开模二段位置最大值*/
#define OPEN2_POS_MIN               0x33140037     /*开模二段位置最小值*/
#define OPEN3_POS_MAX               0x33140038     /*开模三段位置最大值*/
#define OPEN3_POS_MIN               0x33140039     /*开模三段位置最小值*/
#define OPEN4_POS_MIN               0x3314003a     /*开模四段位置最小值*/
#define OPEN_END_POS_MIN            0x3314003b     /*开模终止位置最小值*/
#define OPEN1_STR_VAL               0x3314003c     /*开模一段文本*/
#define OPEN4_STR_VAL               0x3314003d     /*开模四段文本*/
#define OPEN_END_STR_VAL            0x3314003e     /*开模终止文本*/
#define OPEN2_PUSH_POS_ENABLE       0x3314003f     /*开模二段十字头位置是否使用*/
#define OPEN3_PUSH_POS_ENABLE       0x33140040     /*开模三段十字头位置是否使用*/
#define INTELL_OPEN_FUNC_CODE       0X33140041     /*code0-code5-code54-智能开模*/
#define CLS1_STR_VAL                0x33140042     /*关模一段文本*/
#define CLS2_STR_VAL                0x33140043     /*关模二段文本*/
#define CLS1_PUSH_POS_ENABLE        0x33140044     /*关模一段十字头位置是否使用*/
#define INTELL_CLS_FUNC_CODE        0x33140045     /*code43-code0-code54-智能关模*/
#define HI_PRS2_PUSH_POS_VISIBLE    0x33140046     /*高压锁模2十字头位置是否可见*/
#define PUSH_POS_UNIT2_VISIBLE      0x33140047     /*关模十字头单位是否可见*/
#define NOT_HI_PRS2_POS_ENABLE      0x33140048     /*反-高压锁模2是否使用*/
#define CLP_POS_IMG_VAL             0x33140049     /*开模位置图片索引*/
#define CLP_PO_CLS_SOL_IMG_VAL      0x3314004a     /*合模阀图片索引*/
#define CLP_PO_OPN_SOL_IMG_VAL      0x3314004b     /*开模阀图片索引*/
#define CLP_PI_CLS_END_IMG_VAL      0x3314004c     /*合模到底图片索引*/
#define OPEN4_PRS_ENABLE            0x3314004d     /*开模四段压力是否使用*/
#define ADPOSI_METERMAXCLAMP2       0x3314004e     /*开关模电子尺最大值*/
#define OPEN_END_STR1_VAL           0x3314004f     /*开模终止文本没有空格*/
#define OPEN_2_ENABLE               0x331402f8     /*开模二段可用*/
#define OPEN_3_ENABLE               0x331402f9     /*开模三段可用*/
#define OPEN_4_ENABLE               0x331402fa     /*开模四段可用*/

	/****************************射出界面变量模块-INJ****************************/
#define HOLD_POS_ENABLE             0x33140060     /*保压位置是否使用*/
#define INJ_AND_HOLD_TM_DOT         0x33140061     /*射出和保压时间小数位*/
#define HOLD_SPD_MAX                0x33140062     /*保压速度最大值*/
#define INJ_SPD_MAX                 0x33140063     /*射出速度最大值*/
#define INJ_AND_HOLD_SPD_DOT        0x33140064     /*射出和保压速度小数位*/
#define INJ_TOTAL_TM_VISIBLE        0x33140065     /*射出总时间是否可见*/
#define INJ_2_ENABLE                0x33140066     /*射出2段是否使用*/
#define INJ_3_ENABLE                0x33140067     /*射出3段是否使用*/
#define INJ_4_ENABLE                0x33140068     /*射出4段是否使用*/
#define INJ_5_ENABLE                0x33140069     /*射出5段是否使用*/
#define INJ_6_ENABLE                0x3314006a     /*射出6段是否使用*/
#define INJ_TM_2_ENABLE             0x3314006b     /*射出时间2段是否使用*/
#define INJ_TM_3_ENABLE             0x3314006c     /*射出时间3段是否使用*/
#define INJ_TM_4_ENABLE             0x3314006d     /*射出时间4段是否使用*/
#define INJ_TM_5_ENABLE             0x3314006e     /*射出时间5段是否使用*/
#define INJ_TM_6_ENABLE             0x3314006f     /*射出时间6段是否使用*/
#define INJ_POS_2_ENABLE			0x33140070     /*射出位置2段是否使用*/
#define INJ_POS_3_ENABLE			0x33140071     /*射出位置3段是否使用*/
#define INJ_POS_4_ENABLE			0x33140072     /*射出位置4段是否使用*/
#define INJ_POS_5_ENABLE			0x33140073     /*射出位置5段是否使用*/
#define INJ_POS_6_ENABLE			0x33140074     /*射出位置6段是否使用*/
#define INJ_POS_1_MIN               0x33140075     /*射出位置1段最小值*/
#define INJ_POS_2_MIN               0x33140076     /*射出位置2段最小值*/
#define INJ_POS_3_MIN               0x33140077     /*射出位置3段最小值*/
#define INJ_POS_4_MIN               0x33140078     /*射出位置4段最小值*/
#define INJ_POS_5_MIN               0x33140079     /*射出位置5段最小值*/
#define HOLD_2_ENABLE               0x3314007a     /*保压2段是否使用*/
#define HOLD_3_ENABLE               0x3314007b     /*保压3段是否使用*/
#define HOLD_4_ENABLE               0x3314007c     /*保压4段是否使用*/
#define HOLD_5_ENABLE               0x3314007d     /*保压5段是否使用*/
#define INJ_POS_IMG_VAL             0x3314007e     /*射出位置图片索引*/
#define INJ_PO_SOL_IMG_VAL          0x3314007f     /*射出阀图片索引*/
#define NOZ_PO_FWD_IMG_VAL          0x33140080     /*座进阀图片索引*/
#define INJ_POS_1_ENABLE            0x33140081     /*射出一段位置是否使用*/
#define INJ_TM_1_ENABLE             0x33140082     /*射出一段时间是否使用*/
#define INJ_SEG_TM_VISIBLE          0x33140083     /*射出分段时间是否可见*/
#define INJ_STATE_TM_VISIBLE        0x33140084     /*各段射出时间是否可见*/

	/*****************************储料界面变量模块-CHG***********************************/
#define BACK_PRS_DOT                0x3314008b     /*储料背压小数位*/
#define BACK_PRS_MAX                0x3314008c     /*储料背压最大值*/
#define CHG_2_POS_MAX               0x3314008d     /*储料2段终止位置最大值*/
#define BACK_PRS_ENABLE             0x3314008e     /*储料背压是否可用*/
#define BACK_PRS_3_ENABLE           0x3314008f     /*储料背压3段是否可用*/
#define CHG_3_ENABLE                0x33140090     /*储料3段是否使用*/
#define SK_BEF_CHG_DIST_ENABLE      0x33140091     /*储前射退距离是否可用*/    
#define SK_BEF_CHG_TM_ENABLE        0x33140092     /*储前射退时间是否可用*/
#define REMELT_ENABLE               0x33140093     /*再次储料是否使用*/
#define SK_END_POS_MAX              0x33140094     /*射退终止位置最大值*/
#define PLUS_VISIBLE                0x33140095     /*加号是否可见*/
#define SK_DIST_STR_VAL             0x33140096     /*射退距离文本*/
#define CHG_PO_SOL_IMG_VAL          0x33140097     /*储料阀图片索引*/
#define SK_PO_SOL_IMG_VAL           0x33140098     /*射退阀图片索引*/
#define REMELT_POS_MAX              0x33140099     /*再次储料位置最大值*/
#define CHG_3_POS_MAX               0x3314009a     /*储料3段终止位置最大值*/
#define CHG_2_ENABLE                0x3314009b     /*储料2段是否使用*/

	/***********************************吹气2界面变量模块-AIR*********************************************/

#define AIR7_POS_ENABLE		        0x331400a4     /*吹气7结束位置是否使用*/
#define AIR8_POS_ENABLE		        0x331400a5     /*吹气8结束位置是否使用*/
#define AIR9_POS_ENABLE		        0x331400a6     /*吹气9结束位置是否使用*/
#define AIR10_POS_ENABLE		    0x331400a7     /*吹气10结束位置是否使用*/
#define AIR11_POS_ENABLE		    0x331400a8     /*吹气11结束位置是否使用*/
#define AIR12_POS_ENABLE		    0x331400a9     /*吹气12结束位置是否使用*/
#define AIR13_POS_ENABLE		    0x331400aa     /*吹气13结束位置是否使用*/
#define AIR14_POS_ENABLE		    0x331400ab     /*吹气14结束位置是否使用*/
#define AIR7_POS_MAX		        0x331400ac     /*吹气7结束位置最大值*/
#define AIR8_POS_MAX		        0x331400ad     /*吹气8结束位置最大值*/
#define AIR9_POS_MAX		        0x331400ae     /*吹气9结束位置最大值*/
#define AIR10_POS_MAX		        0x331400af     /*吹气10结束位置最大值*/
#define AIR11_POS_MAX		        0x331400b0     /*吹气11结束位置最大值*/
#define AIR12_POS_MAX		        0x331400b1     /*吹气12结束位置最大值*/
#define AIR13_POS_MAX		        0x331400b2     /*吹气13结束位置最大值*/
#define AIR14_POS_MAX		        0x331400b3     /*吹气14结束位置最大值*/

	/**************************托膜功能界面变量模块-EJ*****************************/
#define STAY_EJTADV_KEEP_ENABLE     0x331400b9     /*停留时托进保持是否可用*/
#define DLY_MODE_BEF_EJTAdV_ENABLE  0x331400ba     /*托进前延迟方式是否可用*/
#define DLY_MODE_BEF_EJTRET_ENABLE  0x331400bb     /*托退前延迟方式是否可用*/
#define EJRET_END_FILTER_TM_ENABLE  0x331400bc     /*托退终滤波时间是否可用*/

	/************************开关模功能界面变量模块-CLP*****************************/
#define OPEN_LINK_ENABLE            0x331400c2     /*开模联动是否可用*/
#define CLOSE_LINK_ENABLE           0x331400c3     /*关模联动是否可用*/
#define OPEN_LINK_POS_ENABLE        0x331400c4     /*开模联动位置是否可用*/
#define LINK_VLV_DLYOFF_TM_ENABLE   0x331400c5     /*联动阀延迟关计时是否可用*/
#define CLOSE_LINK_POS_ENABLE       0x331400c6     /*关模联动位置是否可用*/
#define INTELLCLS_DIST_ENABLE       0x331400c7     /*智能关模启动距离是否可用*/
#define INTELLCLS_TM_ENABLE         0x331400c8     /*智能关模启动保护时间是否可用*/
#define CLS_3_OPTION_ENABLE         0x331400c9     /*关模三段选项是否可用*/
#define OPEN_4_OPTION_ENABLE        0x331400ca     /*开模四段选项是否可用*/
#define FLOAT_CYLIND_PRS_ENABLE     0x331400cb     /*浮动缸压力是否可用*/
#define FLOAT_CYLIND_SPD_ENABLE     0x331400cc     /*浮动缸速度是否可用*/
#define INTELL_CLS_OPTION_ENABLE    0x331400cd     /*智能关模功能是否可用*/
#define INTELL_OPEN_OPTION_ENABLE   0x331400ce     /*智能开模功能是否可用*/
#define OPEN_RETARD_TM_ENABLE       0x331400cf     /*开模减速时间是否可用*/


	/****************************射出界面功能变量辑模块-INJ****************************/
#define HOLD_PorS_ENABLE            0x331400d5     /*保压压力或速度是否可用*/
#define HOLD_PorS_MAX               0x331400d6     /*保压压力或速度最大值*/
#define HOLD_PorS_UNIT              0x331400d7     /*保压压力或速度单位*/
#define INJ_PorS_ENABLE             0x331400d8     /*射出压力或速度是否可用*/
#define INJ_PorS_UNIT               0x331400d9     /*射出压力或速度单位*/
#define HOLD_PRS_VISIBLE            0x331400da     /*保压压力是否可见*/
#define HOLD_SPD_VISIBLE            0x331400db     /*保压速度是否可见*/
#define INJ_PRS_VISIBLE             0x331400dc     /*射出压力是否可见*/
#define INJ_SPD_VISIBLE             0x331400dd     /*射出速度是否可见*/
#define ACC_HY_CIR_SELT_ENABLE      0x331400de     /*蓄能器油路选择是否可用*/
#define PRS_SENSOR_ENABLE           0x331400df     /*蓄能器压力传感器是否使用*/
#define INJ_1_TM_BEF_COMPRS_ENABLE  0x331400e0     /*压塑前射出一段计时是否可用*/
#define COM_PRS_VISIBLE             0x331400e1     /*压塑是否可见*/     
#define INJ_1_TM_BEF_COMPRS_DOT     0x331400e2     /*压塑前射出一段计时小数位*/
#define INJ_1_TM_BEF_COMPRS_MAX     0x331400e3     /*压塑前射出一段计时最大值*/
#define OPEN_LOOP_ENABLE            0x331400e4     /*开环功能是否使用*/

	/****************************射出功能2界面变量模块-INJ****************************/
#define EXHT_INJ_POS_ENABLE         0x331400ea     /*排气时射出位置是否可用*/
#define EXHT_INJ_TM_ENABLE          0x331400eb     /*排气时射出时间是否可用*/
#define INJ_DIST_BEF_NOZADV_ENABLE  0x331400ec     /*座进前射出距离是否可用*/
#define INJ_TM_BEF_NOZADV_ENABLE    0x331400ed     /*座进前射出时间是否可用*/
#define EXHT_INJ_TM_MAX             0x331400ee     /*排气时射出时间最大值*/
#define EXHT_INJ_TM_DOT             0x331400ef     /*排气时射出时间小数位*/
#define EXHT_FUNC_ENABLE            0x331400f0     /*排气功能是否使用*/
#define HYDRAULIC_NOZZLES_ENABLE    0x331400f1     /*液压喷嘴是否使用*/
#define INJ_MODE_BEF_NOZADV_ENABLE  0x331400f2     /*座进前射出方式是否可用*/

	/****************************储料功能界面变量模块-CHG****************************/
#define BMC_VISIBLE                 0x331400f8    /*BMC是否可见*/
#define CHG_OPEN_LOOP_ENABLE        0x331400f9    /*储料开环是否可用*/
#define CHG_OPEN_LINK_ENABLE        0x331400fa    /*储料开模联动是否可用*/

	/****************************中子界面变量模块-CORE****************************/
#define COUNT_TIME_A_VISIBLE         0x33140100   /*计数时间A是否可见*/
#define COUNT_TIME_B_VISIBLE         0x33140101   /*计数时间B是否可见*/
#define COUNT_TIME_C_VISIBLE         0x33140102   /*计数时间C是否可见*/
#define COUNT_TIME_D_VISIBLE         0x33140103   /*计数时间D是否可见*/
#define COUNT_TIME_E_VISIBLE         0x33140104   /*计数时间E是否可见*/
#define COUNT_TIME_F_VISIBLE         0x33140105   /*计数时间F是否可见*/

#define TRAVEL_TIME_A_VISIBLE        0x33140106   /*行程时间A是否可见*/
#define TRAVEL_TIME_B_VISIBLE        0x33140107   /*行程时间B是否可见*/
#define TRAVEL_TIME_C_VISIBLE        0x33140108   /*行程时间C是否可见*/
#define TRAVEL_TIME_D_VISIBLE        0x33140109   /*行程时间D是否可见*/
#define TRAVEL_TIME_E_VISIBLE        0x3314010a   /*行程时间E是否可见*/
#define TRAVEL_TIME_F_VISIBLE        0x3314010b   /*行程时间F是否可见*/

#define ACT_POS_A_IN_ENABLE          0x3314010c   /*动作位置A-IN是否可用*/
#define ACT_POS_B_IN_ENABLE          0x3314010d   /*动作位置B-IN是否可用*/
#define ACT_POS_C_IN_ENABLE          0x3314010e   /*动作位置C-IN是否可用*/
#define ACT_POS_D_IN_ENABLE          0x3314010f   /*动作位置D-IN是否可用*/
#define ACT_POS_E_IN_ENABLE          0x33140110   /*动作位置E-IN是否可用*/
#define ACT_POS_F_IN_ENABLE          0x33140111   /*动作位置F-IN是否可用*/

#define ACT_POS_A_OUT_ENABLE         0x33140112   /*动作位置A-OUT是否可用*/
#define ACT_POS_B_OUT_ENABLE         0x33140113   /*动作位置B-OUT是否可用*/
#define ACT_POS_C_OUT_ENABLE         0x33140114   /*动作位置C-OUT是否可用*/
#define ACT_POS_D_OUT_ENABLE         0x33140115   /*动作位置D-OUT是否可用*/
#define ACT_POS_E_OUT_ENABLE         0x33140116   /*动作位置E-OUT是否可用*/
#define ACT_POS_F_OUT_ENABLE         0x33140117   /*动作位置F-OUT是否可用*/

#define ACT_POS_IN_ENABLE            0x3314011a   /*动作位置IN是否可用*/
#define ACT_POS_OUT_ENABLE           0x3314011b   /*动作位置OUT是否可用*/
#define COUNT_TIME_VISIBLE           0x3314011c   /*计数时间是否可见*/
#define TRAVEL_TIME_VISIBLE          0x3314011d   /*行程时间是否可见*/

	/****************************中子功能界面变量模块-CORE****************************/
#define SPECIAL_CORE_FUNC_ENABLE     0x3314011f   /*特殊中子功能是否可用*/
#define SPECIAL_CORE_FUNC_VISIBLE    0x33140120   /*特殊中子功能是否使用*/
#define SPECIAL_CORE_1_FUNC_VISIBLE  0x33140121   /*特殊中子1功能是否使用*/
#define SPECIAL_CORE_2_FUNC_VISIBLE  0x33140122   /*特殊中子2功能是否使用*/
#define SPECIAL_CORE_3_FUNC_VISIBLE  0x33140123   /*特殊中子3功能是否使用*/
#define SPECIAL_CORE_4_FUNC_VISIBLE  0x33140124   /*特殊中子4功能是否使用*/
#define SPECIAL_CORE_5_FUNC_VISIBLE  0x33140125   /*特殊中子5功能是否使用*/
#define SPECIAL_CORE_6_FUNC_VISIBLE  0x33140126   /*特殊中子6功能是否使用*/
#define WAY_CORE_OUT_MIN_POS_MAX     0x33140127   /*途中中子退下限位置最大值*/
#define SPECIAL_CORE_7_FUNC_VISIBLE  0x33140128   /*特殊中子7功能是否使用*/


	/****************************温度界面变量模块-TEMP****************************/
#define MANUAL_TEMP_ENABLE           0x3314012c   /*手动温度是否可用*/
#define TEMP_SET_VAL_MAX             0x3314012d   /*温度设定值最大值*/
#define TEMP_1_IMG_VAL               0x3314012e   /*温度一段图片索引*/
#define TEMP_2_IMG_VAL               0x3314012f   /*温度二段图片索引*/
#define TEMP_3_IMG_VAL               0x33140130   /*温度三段图片索引*/
#define TEMP_4_IMG_VAL               0x33140131   /*温度四段图片索引*/
#define TEMP_5_IMG_VAL               0x33140132   /*温度五段图片索引*/
#define TEMP_6_IMG_VAL               0x33140133   /*温度六段图片索引*/
#define TEMP_7_IMG_VAL               0x33140134   /*温度七段图片索引*/
#define TEMP_4_VISIBLE               0x33140135   /*温度四段是否可见*/
#define TEMP_5_VISIBLE               0x33140136   /*温度五段是否可见*/
#define TEMP_6_VISIBLE               0x33140137   /*温度六段是否可见*/
#define TEMP_7_VISIBLE               0x33140138   /*温度七段是否可见*/
#define TEMP_1_SET_ENABLE            0x33140139   /*温度一段设定是否可用*/
	/****************************温度界面2变量模块-TEMP****************************/
#define TEMP_8_IMG_VAL               0x3314013a   /*温度八段图片索引*/
#define TEMP_9_IMG_VAL               0x3314013b   /*温度九段图片索引*/
#define TEMP_10_IMG_VAL              0x3314013c   /*温度十段图片索引*/
#define TEMP_11_IMG_VAL              0x3314013d   /*温度十一段图片索引*/
#define TEMP_12_IMG_VAL              0x3314013e   /*温度十二段图片索引*/
#define TEMP_13_IMG_VAL              0x3314013f   /*温度十三段图片索引*/
#define TEMP_14_IMG_VAL              0x33140140   /*温度十四段图片索引*/

	/****************************其它界面变量模块-OTHER****************************/
#define EVERY_INCHING_STEP_ENABLE    0x3314014a   /*每次点动步数是否可用*/
#define POS_ADJUST_ENABLE            0x3314014b   /*震雄调模是否可用*/
#define NOZ_POS_ENABLE               0x3314014c   /*座台使用位置是否使用*/
#define NOZ_METER_VISIBLE            0x3314014d   /*座台位置是否可见*/
#define PRESSOR_VISIBLE              0x3314014e   /*传感器是否可见*/
#define ZERO_VISIBLE                 0x3314014f   /*伺服5是否可见*/
#define AD4_VISIBLE                  0x33140150   /*伺服3是否可见*/
#define NOZ_PO_BWD_IMG_VAL           0x33140151   /*座退阀图片索引*/

/****************************其它参数界面变量模块-OTHER****************************/
#define CNPOSI_ADJPROPERTIONAL_DOT        0x33140152    /*每齿移动距离小数位*/
#define CNPOSI_ADJPROPERTIONAL_MAX        0x33140153    /*每齿移动距离最大值*/

	/****************************其它功能界面变量模块-OTHER****************************/
#define AUTO_SAFE_DOOR_ENABLE        0x33140158    /*自动安全门是否使用*/
#define WAY_ROBOT_FUNC_ENABLE        0x33140159    /*途中机械手是否使用*/
#define ROB_CORE_1_LINK_ENABLE       0x3314015a    /*中子信号1连接是否可用*/
#define ROB_CORE_2_LINK_ENABLE       0x3314015b    /*中子信号2连接是否可用*/
#define SPECIAL_LINK_ENABLE          0x3314015c    /*特殊联动是否可用*/
#define ROBOTACTTM_DOT               0x3314015d    /*机械手动作时间小数点*///20190806 CHZ
#define ROBOTACTTM_MAX               0x3314015e    /*机械手动作时间最大值*/
#define WAYROBOTPOS_MIN              0x3314015f    /*途中机械手启动位置最小值*/

#define WAY_ROBOT_OUTPUT_TM_DOT      0x33140163    /*途中机械手输出时间小数位*/
#define WAY_ROBOT_OUTPUT_TM_MAX      0x33140164    /*途中机械手输出时间最大值*/

	/****************************开关模参数一界面变量模块-CLP****************************/
#define CLS_HIGHPRS_KEEP_TM_DOT      0x33140161    /*关模高压计时小数位*/
#define CLS_HIGHPRS_KEEP_TM_MAX      0x33140162    /*关模高压计时最大值*/

	/****************************开关模参数二界面变量模块-CLP****************************/
#define RAMP_CHOOSED_ENABLE          0x33140165    /*部分斜率是否可用*/
#define OPN_4_SPD_RAMP_STR_VAL       0x33140166    /*开模四段速度斜率文本*/
#define OPN_DIFFVLV_ACT_TM_ENABLE    0x33140167    /*开模差动阀动作时间是否可用*/
#define PUSH_POS_VISIBLE             0x33140168    /*控制十字头是否可见*/

	/****************************射出参数界面变量模块-INJ****************************/
#define MANL_USE_DEBUG_PRSSPD_ENABLE 0x3314016a    /*手动使用调试压力速度是否使用*/

/****************************射出曲线界面变量模块-Inject_Curve****************************/
#define INJ_CURVE_POS_VISIBLE        0x3314016b    /*射出曲线模式为位置*/
/****************************射出曲线界面变量模块-Inject_Curve1****************************/
#define INJ_CURVE_TM_DOT             0x3314016c    /*射出和保压时间精确到0.001或0.1*/

	/****************************储料参数界面变量模块-CHG****************************/
#define CONFLUCHG_SPDMAX_ENABLE      0x3314016d    /*合流储料速度限制是否使用*/

/****************************射出曲线界面变量模块-Inject_Curve****************************/
#define HOLE_TM_MAX                  0x3314016e    /*保压使用时间5段最大值*/

	/****************************中子参数界面变量模块-CORE****************************/
#define CORE_SELECT_ENABLE           0x33140170    /*中子选择是否使用*/

	/****************************温度参数一界面变量模块-TEMP****************************/
#define EXTRA_TEMP_VISIBLE           0x33140173    /*额外温度是否使用*/
#define ONE_TO_SEVEN_MIN             0x33140174    /*一到七段最小值*/
#define ONE_TO_SEVEN_MAX             0x33140175    /*一到七段最大值*/

	/****************************其它参数一界面变量模块-Other****************************/
#define RELEASEMD_INITIAL_ENABLE     0x33140178    /*松模初始化是否可用*/
#define LOCKPRS_EFFAREA_MAX          0x33140179    /*锁模力最大有效区最大值*/

	/****************************其它参数二界面变量模块-Other****************************/
#define MOLD_THICK_ADJUST_ENABLE     0x3314017a    /*模厚调模是否使用*/

	/****************************主界面变量模块-Main****************************/
#define MOTORTEMP_VISIBLE            0x3314017d    /*电机温度是否可见*/
#define SAFEDOOR_IMG_VAL             0x3314017e    /*安全门图片索引*/
#define SAFEDOOR_2_IMG_VAL           0x3314017f    /*安全门2图片索引*/
#define BACK_SAFEDOOR_IMG_VAL        0x33140180    /*后安全门图片索引*/
#define SCREW_CHECK_IMG_VAL          0x33140181    /*螺杆检知图片索引*/
#define OPN_END_IMG_VAL              0x33140182    /*开模终图片索引*/
#define LUB_CHK1_IMG_VAL			 0x33140183	   /*润滑检知1图片索引*/
#define LUB_CHK2_IMG_VAL			 0x33140184	   /*润滑检知2图片索引*/
#define LUB_CHK3_IMG_VAL			 0x33140185	   /*润滑检知3图片索引*/
#define PRD_CHK_IMG_VAL				 0x33140186	   /*检出电眼图片索引*/
#define NOZ_FWD_IMG_VAL				 0x33140187	   /*座进终图片索引*/
#define NOZ_BWD_IMG_VAL				 0x33140188	   /*座退终图片索引*/
#define SERVO_ST_VISIBLE             0x33140189    /*伺服状态信息是否可见*/
#define MOTOR_VISIBLE                0x3314018a    /*电机图标是否可见*/
#define AD6_VISIBLE                  0x3314018b    /*dtAD6是否可见*/
#define NOZZLE_VISIBLE               0x3314018c    /*dtnozzle是否可见*/
#define SERVO_VISIBLE                0x3314018d    /*stbar/pixprs/dtsvo/stsvo/dtsvo2/strpm2 是否可见*/
#define SERVO_ST1_VISIBLE            0x3314018e    /*伺服通信状态是否可见1*/
#define SERVO_ST2_VISIBLE            0x3314018f    /*伺服通信状态是否可见2*/
#define SERVO_ST3_VISIBLE            0x33140190    /*伺服通信状态是否可见3*/
#define SERVO_ST4_VISIBLE            0x33140191    /*伺服通信状态是否可见4*/
#define SERVO_ST5_VISIBLE            0x33140192    /*伺服通信状态是否可见5*/
#define SERVO_ST6_VISIBLE            0x33140193    /*伺服通信状态是否可见6*/
#define SERVO_ST7_VISIBLE            0x33140194    /*伺服通信状态是否可见7*/
#define SERVO_COLOR1_VAL			 0x33140195    /*伺服通信状态颜色1*/
#define SERVO_COLOR2_VAL			 0x33140196    /*伺服通信状态颜色2*/
#define SERVO_COLOR3_VAL			 0x33140197    /*伺服通信状态颜色3*/
#define SERVO_COLOR4_VAL			 0x33140198    /*伺服通信状态颜色4*/
#define SERVO_COLOR5_VAL			 0x33140199    /*伺服通信状态颜色5*/
#define SERVO_COLOR6_VAL			 0x3314019a    /*伺服通信状态颜色6*/
#define SERVO_COLOR7_VAL			 0x3314019b    /*伺服通信状态颜色7*/
#define NOZZLE_AD_VISIBLE            0x3314019c    /*座台电子尺是否可见*/
#define ACC_VISIBLE                  0x3314019d    /*蓄能器是否可见*/
#define PIXSCREWSPD_VISIBLE          0x3314019e    /*螺杆是否可见*/
#define HEATER_VISIBLE               0x3314019f    /*加热图标是否可见*/
#define ROBOT_VISIBLE                0x331401a0    /*机械手图标是否可见*/
#define LUBR_VISIBLE                 0x331401a1    /*润滑图标是否可见*/

	/****************************电子尺调整界面-AD_Adjust****************************/
#define A920_MODEL_ENABLE                   0x331401a2    /*A920是否选用*/
#define SENSOR_NOZ_UNIT                     0x331401a3    /*传感器座台单位切换*/
#define SENSOR_AD6_UNIT                     0x331401a4    /*传感器AD6单位切换*/
#define SENSOR_VISIBLE                      0x331401a5    /*传感器是否可见*/
#define NOZ_VISIBLE                         0x331401a6    /*座台是否可见*/
#define SENSOR2_VISIBLE                     0x331401a7    /*传感器是否可见*/
#define CFG_AD6_VISIBLE                     0x331401a8    /*AD6是否可见*/

	/****************************DA调整界面-DA_Adjust****************************/
#define WEIYA_ENABLE                        0x331401a9    /*厂商是否为威亚*/

	/****************************数据检测界面-Data_Diag****************************/
#define CODE3_2_ENABLE                      0x331401ac    /*code51打2是否使用*/

	/****************************主测试界面-Main_Adjust****************************/
#define ADD_SIGN5_VISIBLE                   0x331401af    /*加号5是否可见*/
#define ADD_SIGN6_VISIBLE                   0x331401b0    /*加号6是否可见*/
#define ADD_SIGN1_VISIBLE                   0x331401b1    /*加号1是否可见*/
#define ADD_SIGN2_VISIBLE                   0x331401b2    /*加号2是否可见*/
#define ADD_SIGN3_VISIBLE                   0x331401b3    /*加号3是否可见*/
#define ADD_SIGN4_VISIBLE                   0x331401b4    /*加号4是否可见*/
#define ROOM_ADD_SIGN_VISIBLE               0x331401b5    /*室温加号是否可见*/
#define ROOM_SUB_SIGN_VISIBLE               0x331401b6    /*室温减号是否可见*/

	/****************************主框架界面-Mainframe****************************/
#define USE_SERVO_VISIBLE                   0x331401b8    /*伺服是否使用*/

	/****************************泵浦配置界面-Pump_cfg****************************/
#define PUMP_ADJUST_ENABLE                  0x331401bb    /*泵浦调整是否可用*/

	/****************************泵浦配置额外界面-Pump_cfg_x****************************/
#define MULT_PUMP_VISIBLE                   0x331401bc    /*多泵是否使用*/
#define SINGLE_PUMP_VISIBLE                 0x331401bd    /*单泵是否使用*/
#define VICE_STR_STOP_ENABLE                0x331401be    /*泵启停速度是否可用*/
#define PUMP_ADJ_2TO7_ENABLE                0x331401bf    /*泵浦2-7是否使用*/

	/****************************顺序阀界面-Seq_Vlv****************************/
#define DLY_TIME_1_ENABLE                   0x331401c3    /*延迟计时1是否可用*/
#define ACT_TIME_1_ENABLE                   0x331401c4    /*动作时间1是否可用*/
#define START_POS_1_ENABLE                  0x331401c5    /*起始位置1是否可用*/
#define END_POS_1_ENABLE                    0x331401c6    /*终止位置1是否可用*/
#define DLY_TIME_2_ENABLE                   0x331401c7    /*延迟计时2是否可用*/
#define ACT_TIME_2_ENABLE                   0x331401c8    /*动作时间2是否可用*/
#define START_POS_2_ENABLE                  0x331401c9    /*起始位置2是否可用*/
#define END_POS_2_ENABLE                    0x331401ca    /*终止位置2是否可用*/
#define DLY_TIME_3_ENABLE                   0x331401cb    /*延迟计时3是否可用*/
#define ACT_TIME_3_ENABLE                   0x331401cc    /*动作时间3是否可用*/
#define START_POS_3_ENABLE                  0x331401cd    /*起始位置3是否可用*/
#define END_POS_3_ENABLE                    0x331401ce    /*终止位置3是否可用*/
#define DLY_TIME_4_ENABLE                   0x331401cf    /*延迟计时4是否可用*/
#define ACT_TIME_4_ENABLE                   0x331401d0    /*动作时间4是否可用*/
#define START_POS_4_ENABLE                  0x331401d1    /*起始位置4是否可用*/
#define END_POS_4_ENABLE                    0x331401d2    /*终止位置4是否可用*/
#define DLY_TIME_5_ENABLE                   0x331401d3    /*延迟计时5是否可用*/
#define ACT_TIME_5_ENABLE                   0x331401d4    /*动作时间5是否可用*/
#define START_POS_5_ENABLE                  0x331401d5    /*起始位置5是否可用*/
#define END_POS_5_ENABLE                    0x331401d6    /*终止位置5是否可用*/
#define DLY_TIME_6_ENABLE                   0x331401d7    /*延迟计时6是否可用*/
#define ACT_TIME_6_ENABLE                   0x331401d8    /*动作时间6是否可用*/
#define START_POS_6_ENABLE                  0x331401d9    /*起始位置6是否可用*/
#define END_POS_6_ENABLE                    0x331401da    /*终止位置6是否可用*/
#define DLY_TIME_7_ENABLE                   0x331401db    /*延迟计时7是否可用*/
#define ACT_TIME_7_ENABLE                   0x331401dc    /*动作时间7是否可用*/
#define START_POS_7_ENABLE                  0x331401dd    /*起始位置7是否可用*/
#define END_POS_7_ENABLE                    0x331401de    /*终止位置7是否可用*/
#define DLY_TIME_8_ENABLE                   0x331401df    /*延迟计时8是否可用*/
#define ACT_TIME_8_ENABLE                   0x331401e0    /*动作时间8是否可用*/
#define START_POS_8_ENABLE                  0x331401e1    /*起始位置8是否可用*/
#define END_POS_8_ENABLE                    0x331401e2    /*终止位置8是否可用*/
#define DLY_TIME_9_ENABLE                   0x331401e3    /*延迟计时9是否可用*/
#define ACT_TIME_9_ENABLE                   0x331401e4    /*动作时间9是否可用*/
#define START_POS_9_ENABLE                  0x331401e5    /*起始位置9是否可用*/
#define END_POS_9_ENABLE                    0x331401e6    /*终止位置9是否可用*/
#define DLY_TIME_10_ENABLE                  0x331401e7    /*延迟计时10是否可用*/
#define ACT_TIME_10_ENABLE                  0x331401e8    /*动作时间10是否可用*/
#define START_POS_10_ENABLE                 0x331401e9    /*起始位置10是否可用*/
#define END_POS_10_ENABLE                   0x331401ea    /*终止位置10是否可用*/
#define DLY_TIME_11_ENABLE                  0x331401eb    /*延迟计时11是否可用*/
#define ACT_TIME_11_ENABLE                  0x331401ec    /*动作时间11是否可用*/
#define START_POS_11_ENABLE                 0x331401ed    /*起始位置11是否可用*/
#define END_POS_11_ENABLE                   0x331401ee    /*终止位置11是否可用*/
#define DLY_TIME_12_ENABLE                  0x331401ef    /*延迟计时12是否可用*/
#define ACT_TIME_12_ENABLE                  0x331401f0    /*动作时间12是否可用*/
#define START_POS_12_ENABLE                 0x331401f1    /*起始位置12是否可用*/
#define END_POS_12_ENABLE                   0x331401f2    /*终止位置12是否可用*/
#define DLY_TIME_13_ENABLE                  0x331401f3    /*延迟计时13是否可用*/
#define ACT_TIME_13_ENABLE                  0x331401f4    /*动作时间13是否可用*/
#define START_POS_13_ENABLE                 0x331401f5    /*起始位置13是否可用*/
#define END_POS_13_ENABLE                   0x331401f6    /*终止位置13是否可用*/
#define DLY_TIME_14_ENABLE                  0x331401f7    /*延迟计时14是否可用*/
#define ACT_TIME_14_ENABLE                  0x331401f8    /*动作时间14是否可用*/
#define START_POS_14_ENABLE                 0x331401f9    /*起始位置14是否可用*/
#define END_POS_14_ENABLE                   0x331401fa    /*终止位置14是否可用*/
#define SEQ_VLV_1_IMG_VAL                   0x331401fb    /*顺序阀图片1*/
#define SEQ_VLV_2_IMG_VAL                   0x331401fc    /*顺序阀图片2*/
#define SEQ_VLV_3_IMG_VAL                   0x331401fd    /*顺序阀图片3*/
#define SEQ_VLV_4_IMG_VAL                   0x331401fe    /*顺序阀图片4*/
#define SEQ_VLV_5_IMG_VAL                   0x331401ff    /*顺序阀图片5*/
#define SEQ_VLV_6_IMG_VAL                   0x33140200    /*顺序阀图片6*/
#define SEQ_VLV_7_IMG_VAL                   0x33140201    /*顺序阀图片7*/
#define SEQ_VLV_8_IMG_VAL                   0x33140202    /*顺序阀图片8*/
#define SEQ_VLV_9_IMG_VAL                   0x33140203    /*顺序阀图片9*/
#define SEQ_VLV_10_IMG_VAL                  0x33140204    /*顺序阀图片10*/
#define SEQ_VLV_11_IMG_VAL                  0x33140205    /*顺序阀图片11*/
#define SEQ_VLV_12_IMG_VAL                  0x33140206    /*顺序阀图片12*/
#define SEQ_VLV_13_IMG_VAL                  0x33140207    /*顺序阀图片13*/
#define SEQ_VLV_14_IMG_VAL                  0x33140208    /*顺序阀图片14*/

	/****************************伺服曲线界面-Servo_Curve****************************/
#define SERVO_3_ENABLE                      0x3314020f    /*伺服3是否可用*/

	/****************************伺服测试3界面-Servo_Test3****************************/
#define SYS_PRS_VISIBLE                     0x33140212    /*系统压力是否可见*/

	/****************************系统参数3界面-Sys_Para3****************************/
#define SPECIAL_DATA_SETUP_VISIBLE          0x33140215    /*特殊数据设置是否可见*/

    /****************************系统参数4界面-Sys_Para5****************************/
#define INJSLOW_STARTSPD_DOT                0x33140216    /*射出慢速启动速度小数位*/
#define INJSLOW_STARTSPD_MAX                0x33140217    /*射出慢速启动速度最大值*/
#define INJSLOW_START_ENABLE                0x33140214    /*射出射退启动参数设置是否可用*/

	/****************************版本界面-Version****************************/
#define IO_EXTEND_1_ENABLE                  0x33140218    /*IO扩展板连接状态1是否可用*/
#define IO_EXTEND_2_ENABLE                  0x33140219    /*IO扩展板连接状态2是否可用*/
#define IO_EXTEND_3_ENABLE                  0x3314021a    /*IO扩展板连接状态3是否可用*/
#define IO_EXTEND_4_ENABLE                  0x3314021b    /*IO扩展板连接状态4是否可用*/
#define IO_EXTEND_5_ENABLE                  0x3314021c    /*IO扩展板连接状态5是否可用*/
#define IO_EXTEND_6_ENABLE                  0x3314021d    /*IO扩展板连接状态6是否可用*/
#define IO_EXTEND_7_ENABLE                  0x3314021e    /*IO扩展板连接状态7是否可用*/
#define DA_EXTEND_1_ENABLE                  0x3314021f    /*DA扩展板连接状态1是否可用*/
#define DA_EXTEND_2_ENABLE                  0x33140220    /*DA扩展板连接状态2是否可用*/
#define DA_EXTEND_3_ENABLE                  0x33140221    /*DA扩展板连接状态3是否可用*/
#define DA_EXTEND_4_ENABLE                  0x33140222    /*DA扩展板连接状态4是否可用*/
#define DA_EXTEND_5_ENABLE                  0x33140223    /*DA扩展板连接状态5是否可用*/
#define DA_EXTEND_6_ENABLE                  0x33140224    /*DA扩展板连接状态6是否可用*/
#define DA_EXTEND_7_ENABLE                  0x33140225    /*DA扩展板连接状态7是否可用*/
#define TEMP_EXTEND_1_ENABLE                0x33140226    /*温度扩展板连接状态1是否可用*/
#define TEMP_EXTEND_2_ENABLE                0x33140227    /*温度扩展板连接状态2是否可用*/
#define TEMP_EXTEND_3_ENABLE                0x33140228    /*温度扩展板连接状态3是否可用*/
#define TEMP_EXTEND_4_ENABLE                0x33140229    /*温度扩展板连接状态4是否可用*/
#define TEMP_EXTEND_5_ENABLE                0x3314022a    /*温度扩展板连接状态5是否可用*/
#define TEMP_EXTEND_6_ENABLE                0x3314022b    /*温度扩展板连接状态6是否可用*/
#define TEMP_EXTEND_7_ENABLE                0x3314022c    /*温度扩展板连接状态7是否可用*/
#define MDTP_EXTEND_1_ENABLE                0x3314022d    /*模温扩展板连接状态1是否可用*/
#define MDTP_EXTEND_2_ENABLE                0x3314022e    /*模温扩展板连接状态2是否可用*/
#define MDTP_EXTEND_3_ENABLE                0x3314022f    /*模温扩展板连接状态3是否可用*/
#define MDTP_EXTEND_4_ENABLE                0x33140230    /*模温扩展板连接状态4是否可用*/
#define MDTP_EXTEND_5_ENABLE                0x33140231    /*模温扩展板连接状态5是否可用*/
#define MDTP_EXTEND_6_ENABLE                0x33140232    /*模温扩展板连接状态6是否可用*/
#define MDTP_EXTEND_7_ENABLE                0x33140233    /*模温扩展板连接状态7是否可用*/

/****************************主测试界面-Main_Adjust****************************/
#define SUB_SIGN1_VISIBLE                   0x33140238    /*减号1是否可见*/
#define SUB_SIGN2_VISIBLE                   0x33140239    /*减号2是否可见*/
#define SUB_SIGN3_VISIBLE                   0x3314023a    /*减号3是否可见*/
#define SUB_SIGN4_VISIBLE                   0x3314023b    /*减号4是否可见*/
#define SUB_SIGN5_VISIBLE                   0x3314023c    /*减号5是否可见*/
#define SUB_SIGN6_VISIBLE                   0x3314023d    /*减号6是否可见*/

/****************************系统自学习界面-Servo_Cfg****************************/
#define SERVO_SELF_LEARN_VISIBLE            0x33140240    /*伺服学习步骤提示*/
#define SERVO_ALARM_VISIBLE                 0x33140241    /*驱动器警报提示*/

/****************************数据保护界面-Data_Protect****************************/
#define COOLTM_READ_PROT                    0x33140244    /*模具冷却时间是否只读*/
#define COOLTM_BKCOLOR_VAL                  0x33140245    /*模具冷却时间背景颜色*/
#define COOLTM_TXCOLOR_VAL                  0x33140246    /*模具冷却时间文本颜色*/
#define CLAMP_SPD_PORT                      0x33140247    /*开关模速度上限是否只读*/
#define CLAMP_SPD_BKCOLOR_VAL               0x33140248    /*开关模速度背景色*/
#define CLAMP_SPD_TXCOLOR_VAL               0x33140249    /*开关模速度文本色*/
#define HIPRS_PRS_PORT                      0x3314024a    /*关模高压压力上限是否只读*/
#define HIPRS_PRS_BKCOLOR_VAL               0x3314024b    /*关模高压压力背景色*/
#define HIPRS_PRS_TXCOLOR_VAL               0x3314024c    /*关模高压压力文本色*/
#define HIPRS_SPD_PORT                      0x3314024d    /*关模高压速度上限是否只读*/
#define HIPRS_SPD_BKCOLOR_VAL               0x3314024e    /*关模高压速度背景色*/
#define HIPRS_SPD_TXCOLOR_VAL               0x3314024f    /*关模高压速度文本色*/
/*****************************以下为10竖屏开发新增**************************************/

/*****************************主界面变量模块-Main*********************************************/
#define CLAMP_IMG_VAL                       0x33140258   /*主画面开关模动画*/

#define CLOSE_BAK_IMG_VAL                   0x33140259   /*关模背景图片值*/
#define NOZZLE_BAK_IMG_VAL                  0x3314025a   /*座台背景图片值*/
#define INJECT_BAK_IMG_VAL                  0x3314025b   /*射出背景图片值*/
#define CHARGE_BAK_IMG_VAL                  0x3314025c   /*储料背景图片值*/
#define SUCK_BAK_IMG_VAL                    0x3314025d   /*射退背景图片值*/
#define COOL_BAK_IMG_VAL                    0x3314025e   /*冷却背景图片值*/
#define OPEN_BAK_IMG_VAL                    0x3314025f   /*开模背景图片值*/
#define EJECT_BAK_IMG_VAL                   0x33140260   /*托模背景图片值*/
#define CORE_BAK_IMG_VAL                    0x33140261   /*中子背景图片值*/
#define AIR_BAK_IMG_VAL                     0x33140262   /*吹气背景图片值*/

#define NOZZLE_IMG_VAL                      0x33140263   /*座台电子尺是否使用图片*/

/*****************************温度界面-Temp*********************************************/
#define TEMP6_9_VISIBLE                     0x33140269   /*温度6-9段是否可见*/
#define TEMP6_9_ENABLE                      0x3314026a   /*参数温度6-9段是否可用*/
#define TEMP_SET6_9_ENABLE                  0x3314026b   /*温度设定6-9段是否可用*/
#define TEMP_MAIN_IMG_VISIBLE               0x3314026c   /*主画面摄氏度图标是否可见*/

#define TEMP1_IMG_VAL                       0x3314026d   /*温度一段图片索引*/
#define TEMP2_IMG_VAL                       0x3314026e   /*温度二段图片索引*/
#define TEMP3_IMG_VAL                       0x3314026f   /*温度三段图片索引*/
#define TEMP4_IMG_VAL                       0x33140270   /*温度四段图片索引*/
#define TEMP5_IMG_VAL                       0x33140271   /*温度五段图片索引*/
#define TEMP6_IMG_VAL                       0x33140272   /*温度六段图片索引*/
#define TEMP7_IMG_VAL                       0x33140273   /*温度七段图片索引*/
#define TEMP8_IMG_VAL                       0x33140274   /*温度八段图片索引*/
#define TEMP9_IMG_VAL                       0x33140275   /*温度九段图片索引*/
#define TEMP_EXTRA1_IMG_VAL                 0x33140276   /*温度扩展一图片索引*/
#define TEMP_EXTRA2_IMG_VAL                 0x33140277   /*温度扩展二图片索引*/
#define TEMP_EXTRA3_IMG_VAL                 0x33140278   /*温度扩展三图片索引*/
#define TEMP_EXTRA4_IMG_VAL                 0x33140279   /*温度扩展四图片索引*/
#define TEMP_EXTRA5_IMG_VAL                 0x3314027a   /*温度扩展五图片索引*/
#define TEMP_EXTRA6_IMG_VAL                 0x3314027b   /*温度扩展六图片索引*/
#define TEMP_EXTRA7_IMG_VAL                 0x3314027c   /*温度扩展七图片索引*/
#define TEMP_PART1_ENABLE                   0x3314027d   /*扩展部件1切换*/
#define TEMP_PART2_ENABLE                   0x3314027e   /*扩展部件2切换*/
#define TEMP_PARAM_EXCHANGE                 0x3314027f   /*温度参数画面切换*/

/*****************************托模界面-Eject*********************************************/
#define MULTIPLE_ENABLE                     0x33140285    /*多次选项是否使用*/
#define BLOW_AGAIN_1_14_ENABLE              0x33140286    /*吹气1-14多次是否使用*/
#define DLYTM_ACTTM_1_14_ENABLE             0x33140287    /*1-14延迟计时和动作时间是否使用*/

/*****************************配置画面-Mach_Cfg*********************************************/
#define SENSOR_NOZ_UNIT2                     0x3314028a    /*传感器座台单位切换*/
#define SENSOR_AD6_UNIT2                     0x3314028b    /*传感器AD6单位切换*/

/*****************************射出界面-Inject*********************************************/
#define HOLD_TM_TOTAL_VAL                    0x33140290    /*阶梯曲线时间总*/
#define HOLD_TM1_VAL                         0x33140291    /*阶梯曲线时间1*/
#define HOLD_TM2_VAL                         0x33140292    /*阶梯曲线时间2*/
#define HOLD_TM3_VAL                         0x33140293    /*阶梯曲线时间3*/
#define HOLD_TM4_VAL                         0x33140294    /*阶梯曲线时间4*/
#define HOLD_TM5_VAL                         0x33140295    /*阶梯曲线时间5*/

/*****************************日月年产量*********************************************/
#define DATE_POWER_VISIBLE                   0x33140299    /*日月年功耗是否显示*/

/**************************托膜参数界面变量模块-EJ*****************************/
#define EJ_ADV_HOLD_ENABLE                   0x3314029b    /*托进保持功能是否可用*/

/**************************储料功能界面变量模块-CHG*****************************/
#define VACUUM_ENABLE                        0x3314029d    /*真空发射器是否可用*/

/**************************射出功能界面变量模块-CHG*****************************/
#define OIL_CHECK_ENABLE                     0x3314029f    /*漏油检测是否可用*/

/*****************************以下为G10开发新增**************************************/
/**************************扩展温度界面变量模块-Tempxx*****************************/
#define TEMPCTRL1_STATUS1_VAL               0x331402da    /*温度控制1温度状态1图片*/
#define TEMPCTRL1_STATUS2_VAL               0x331402db    /*温度控制1温度状态2图片*/
#define TEMPCTRL1_STATUS3_VAL               0x331402dc    /*温度控制1温度状态3图片*/
#define TEMPCTRL1_STATUS4_VAL               0x331402dd    /*温度控制1温度状态4图片*/
#define TEMPCTRL1_STATUS5_VAL               0x331402de    /*温度控制1温度状态5图片*/
#define TEMPCTRL1_STATUS6_VAL               0x331402df    /*温度控制1温度状态6图片*/
#define TEMPCTRL1_STATUS7_VAL               0x331402e0    /*温度控制1温度状态7图片*/
#define TEMPCTRL1_STATUS8_VAL               0x331402e1    /*温度控制1温度状态8图片*/
#define TEMPCTRL1_STATUS9_VAL               0x331402e2    /*温度控制1温度状态9图片*/
#define TEMPCTRL2_STATUS1_VAL               0x331402e3    /*温度控制2温度状态1图片*/
#define TEMPCTRL2_STATUS2_VAL               0x331402e4    /*温度控制2温度状态2图片*/
#define TEMPCTRL2_STATUS3_VAL               0x331402e5    /*温度控制2温度状态3图片*/
#define TEMPCTRL2_STATUS4_VAL               0x331402e6    /*温度控制2温度状态4图片*/
#define TEMPCTRL2_STATUS5_VAL               0x331402e7    /*温度控制2温度状态5图片*/
#define TEMPCTRL2_STATUS6_VAL               0x331402e8    /*温度控制2温度状态6图片*/
#define TEMPCTRL2_STATUS7_VAL               0x331402e9    /*温度控制2温度状态7图片*/
#define TEMPCTRL2_STATUS8_VAL               0x331402ea    /*温度控制2温度状态8图片*/
#define TEMPCTRL2_STATUS9_VAL               0x331402eb    /*温度控制2温度状态9图片*/

#define NORMAL_TEMP1_IMG_VAL                0x331402ec    /*普通温度状态图片1*/
#define NORMAL_TEMP2_IMG_VAL                0x331402ed    /*普通温度状态图片2*/
#define NORMAL_TEMP3_IMG_VAL                0x331402ee    /*普通温度状态图片3*/
#define NORMAL_TEMP4_IMG_VAL                0x331402ef    /*普通温度状态图片4*/
#define NORMAL_TEMP5_IMG_VAL                0x331402f0    /*普通温度状态图片5*/
#define NORMAL_TEMP6_IMG_VAL                0x331402f1    /*普通温度状态图片6*/
#define NORMAL_TEMP7_IMG_VAL                0x331402f2    /*普通温度状态图片7*/
#define NORMAL_TEMP8_IMG_VAL                0x331402f3    /*普通温度状态图片8*/
#define NORMAL_TEMP9_IMG_VAL                0x331402f4    /*普通温度状态图片9*/

#define MOUTH_VISIBLE                       0x331402f5    /*落料口是否可见*/

#define BARRELCOOLBIAS_ENABLE               0x331402fb    /*料管冷却偏差可用*/

/**************************生产警报界面-Product_Alarm*****************************/
#define BARRELTEMPBIAS_ENABLE               0x331402fc    /*料管温度偏差可用*/

/**************************IO重定义界面-IO_Redefine*****************************/
#define TEMPCTRLPART0_SHOW                  0x331402fd    /*温度部件为0时可见*/
#define TEMPCTRLPART1_SHOW                  0x331402fe    /*温度部件为1时可见*/
#define TEMPCTRLPART2_SHOW                  0x331402ff    /*温度部件为2时可见*/

#ifdef __cplusplus
}
#endif

#endif
