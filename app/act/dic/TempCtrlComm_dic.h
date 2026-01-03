#ifndef _TEMPCTRLCOMM__DIC_H_
#define _TEMPCTRLCOMM_DIC_H_


#include "typedef.h"
#include "admeter_dic.h"

#pragma pack(1)


#define	TEMP_E502_CH_NUM	10	//E502扩展板最大资源每块16个输入
#define	TEMP_E502_CH_NUM_DIC	16	//E502扩展板最大资源每块16个输入

typedef struct EN_TEMPCTRLCOMM_PO
{
	UI16 O_COOLER  ;                 	// 0x0E100000  冷却水  
	UI16 O_COOL[TEMP_E502_CH_NUM_DIC]; 		// 0x0E100001-0x0E100010  冷却器0  
	UI16 O_TEMOP[TEMP_E502_CH_NUM_DIC]  ;  	// 0x0E100011-0x0E100020  电热0  
	UI16 O_DROP_COL;					// 0x0E100021	落料口冷却水阀
	UI16 O_HEATER  ;                 	// 0x0E100022   电热开      
}TEMPCTRLCOMM_PO, *PTEMPCTRLCOMM_PO;

typedef struct EN_TEMPCTRLCOMM_PI
{ 
    UI16 I_HEATTOUCH;                   // 0x0E110000 加热接触器
}TEMPCTRLCOMM_PI, *PTEMPCTRLCOMM_PI;


typedef struct  EN_TEMPCOMM_MACHSET
{
	UI16 DATEMP_FUNC[TEMP_E502_CH_NUM_DIC]; 		/* 0x0E120000-0x0E12000F 温控功能选择(0为油温段)默认:0	0:关闭	1:只显示	2:手动 3:PID 4:PD渐近*/

	UI16 DATEMP_RAMP[TEMP_E502_CH_NUM_DIC];			/* 0x0E120010-0x0E12001F 温度加温缓冲区[16](第一段温度为0)	  默认:30				范围:0 ~ 99 */
	UI16 DAPID_KP[TEMP_E502_CH_NUM_DIC]; 			/* 0x0E120020-0x0E12002F 温度P系数				 默认8.0	范围0-65535		*/
	UI16 DAPID_TI[TEMP_E502_CH_NUM_DIC]; 			/* 0x0E120030-0x0E12003F 温度I系数				 默认348.0	范围0-65535   	*/
	UI16 DAPID_TD[TEMP_E502_CH_NUM_DIC]; 			/* 0x0E120040-0x0E12004F 温度D系数				 默认87.0	范围0-65535 	*/
	UI16 TM_RESPONSELESSCYCLE[TEMP_E502_CH_NUM_DIC];/* 0x0E120050-0x0E12005F 温度输出周期时间(0为油温段)    默认:12	F519范围:2 ~ 50 F518范围:6~50	*/

	UI16 DATEMP_SCTM_MIN; 						/* 0x0E120060 最小扫描周期			 默认:2				范围:1 ~ 10 */		

	UI16 FL_TEMPWORDUPCHECK;			  		/* 0x0E120061 电热连续加温检查  		默认:1使用				范围:0 ~ 1	*/

	UI16 DATEMP_TEMPCOOLERON;			  		/* 0x0E120062 料管冷却开启上偏差		  	默认:20				范围:0 ~ 100 */
	UI16 DATEMP_TEMPCOOLEROFF;			  		/* 0x0E120063 料管冷却关闭下偏差		  	默认:20				范围:0 ~ 100 */

	UI16 TEMPCOOLCTRL;        			 		/* 0x0E120064 落料口冷却功能 	 	 默认0	范围	0:不使用，1:使用。*/
	UI16 TEMPCOOLMAX;							/* 0x0E120065 落料口温度上限		 默认:100  	范围:0 ~ 200	*/
	UI16 TEMPCOOLCN;							/* 0x0E120066 落料口温度段 IV5000:范围是0-7  ；默认7， IV3100 :范围是0-9 ；默认9*/
	UI16 DATEMP_TEMPCOOLERONMOUTH;				/* 0x0E120067 落料口冷却开 			 默认:10				范围:0 ~ 99 */
	UI16 DATEMP_TEMPCOOLEROFFMOUTH; 			/* 0x0E120068 落料口冷却关 			 默认:10	范围:0~99		*/
			
	UI16 FL_TKTJ[TEMP_E502_CH_NUM_DIC];				/* 0x0E120069-0x0E120078 热电偶类型  默认:0:	0:k-型	1:J-型	*/
	
	UI16 DATEMP_CH_NUM;							/* 0x0E120079 温度段数			 	 默认:8				范围:5 ~ 10 */
	
    UI16 TEMP_PROTECT;                   		/* 0x0E12007A 温度保护上限  默认400   范围350-525*/  /*该参数暂时先放在20110画面*/

	
	UI16  FL_FREETIME_WARMOFF;				    /* 0x0E12007B 闲置电热关	默认:2		范围:0不使用	1:关电热	2:转保温	*/
	UI16  TM_FREETIME_WARMOFF;					/* 0x0E12007C 电热闲置时间		默认:0 min	范围:0~60min		*/
	
    UI16 DA_THERMOCOUPLE_PRT;                   /*0x0E12007D 电热连续加温检查度数 为0时，使用老逻辑，不为0时，使用新逻辑，默认：0，范围：0-99 */
}TEMPCOMM_MACHSET, *PTEMPCOMM_MACHSET;

typedef struct EN_TEMPCOMM_MOLDSET
{
	UI16  DATEMP_SETTING[TEMP_E502_CH_NUM_DIC];  	/* 0x0E130000-0x0E13000F 温度设定值[16]          默认:200			范围:0 ~ TempMax	*/
	UI16  DATEMP_MAX[TEMP_E502_CH_NUM_DIC];       	/* 0x0E130010-0x0E13001F 上偏差[16]            默认:50			范围:0 ~ 100	*/
	UI16  DATEMP_MIN[TEMP_E502_CH_NUM_DIC];       	/* 0x0E130020-0x0E13002F 下偏差[16]            默认:15			范围:0 ~ 100	*/
	UI16  DACENT_HEATERON[TEMP_E502_CH_NUM_DIC];  	/* 0x0E130030-0x0E13003F 手动比例(0为油温段)  	  默认:50.0			范围:0.0 ~ 100.0	*/
	UI16  TM_HEATERWARM;                 		/* 0x0E130040 预温计时                默认:30			范围:0 ~ 99	*/
	UI16  FL_TEMPHOLD;                   		/* 0x0E130041 保温功能选择   		  默认:0			范围:0 ~ 1	*/
	UI16  DATEMP_HOLD;                    		/* 0x0E130042 保温温度       		  默认:100			范围:50 ~ 300	*/
	UI16  FL_TEMPCOOL;                     		/* 0x0E130043 料管冷却器使用     	  默认:0			范围:0 ~ 1	*/
	UI16  CONFUNC;								/* 0x0E130044 同步升温功能			  默认:0			范围:0:关闭	1:开启*/
}TEMPCOMM_MOLDSET, *PTEMPCOMM_MOLDSET;

typedef struct tyTEMPCOMM_ZDSET
{	
	UI16 DATEMP_RAMP[TEMP_E502_CH_NUM_DIC];			/* 0x0E14000-0x0E14000F 温度加温缓冲区[16](第一段温度为0)	  默认:30				范围:0 ~ 99 */
	UI16 DAPID_KP[TEMP_E502_CH_NUM_DIC]; 			/* 0x0E140010-0x0E14001F 温度P系数				 默认8.0	范围0-65535		*/
	UI16 DAPID_TI[TEMP_E502_CH_NUM_DIC]; 			/* 0x0E140020-0x0E14002F 温度I系数				 默认348.0	范围0-65535   	*/
	UI16 DAPID_TD[TEMP_E502_CH_NUM_DIC]; 			/* 0x0E140030-0x0E14003F 温度D系数				 默认87.0	范围0-65535 	*/
	UI16 LIN_FIT_K[TEMP_E502_CH_NUM_DIC]; 		 /* 0x0E140040-0x0E14004F    温度斜率项k                                        */ 
	UI16 LIN_FIT_B[TEMP_E502_CH_NUM_DIC]; 		  /* 0x0E140050-0x0E14005F   温度常数项b                                        */ 
	UI16 IK0[TEMP_E502_CH_NUM_DIC];			  /* 0x0E140060-0x0E14006F       温度前馈积分项                                     */     
	UI16 P11[TEMP_E502_CH_NUM_DIC];			/* 0x0E140070-0x0E14007F         最小二乘法对应的迭代矩阵P(1,1)                     */ 
	UI16 P12[TEMP_E502_CH_NUM_DIC];			/* 0x0E140080-0x0E14008F         最小二乘法对应的迭代矩阵P(1,2)==P(2,1)             */  
	UI16 P22[TEMP_E502_CH_NUM_DIC];			/* 0x0E140090-0x0E14009F         最小二乘法对应的迭代矩阵P(2,2)                     */ 
	UI16 STD_WARMON_CNT[TEMP_E502_CH_NUM_DIC]; /* 0x0E1400A0-0x0E1400AF	     温度自适应次数                                     */ 
}TEMPCOMM_ZDSET, *PTEMPCOMM_ZDSET;


typedef struct EN_TEMPCTRLCOMM_STATE
{
	/*DB_TEMP_ID 560*/
	UI16  FL_ERROR0;						/*	0x0E160000	警报1*/
	UI16  FL_ERROR1;						/*	0x0E160001	警报2*/
	
	UI16  DATEMP_REAL[TEMP_E502_CH_NUM_DIC]; 	/*	0x0E160002-0x0E160011	温度实际值[16]			范围:0 ~ TempMax	*/
	UI16  FL_TEMPSTATUS[TEMP_E502_CH_NUM_DIC];	/*	0x0E160012-0x0E160021	温度状态[16]		范围:0:关闭 1:PID 2:超温停止 3:全功率 4:警报*/
	UI16  OUTPUT_PER[TEMP_E502_CH_NUM_DIC];		/*	0x0E160022-0x0E160031	温度输出实际值百分比[16](0为油温段)				范围:0.0~100.0%*/

	UI16  WTEMP_LM73;						/*	0x0E160032	室温LM73的温度*/
	UI16  WTEMP_RELAT;						/*	0x0E160033	校准后的实时温度*/
	UI16  WTEMP_ADVALUE;					/*	0x0E160034	AD采样得到的实时值*/
	UI16  WTEMP_CHANNEL;					/*	0x0E160035	调试温度通道值*/
	UI16  WTEMP_ADJALL; 					/*	0x0E160036	温度多通道联调标志*/
	UI16  WTEMP_ADJAMB; 					/*	0x0E160037	室温校准值(上位机发下来)*/

	UI16  FL_HEATERSTATUS;					/*	0x0E160038	电热状态*/
	UI16  WSTATERSV6;						/*	0x0E160039	保温倒计时*/
	UI16  DATEMP_CHARGEPOS; 				/*	0x0E16003A	落料口温度*/  
	UI16  DATEMP_YOUHUAXUQIU;				//  0x0E16003B	温度优化提示开电热的闪烁指示
	UI16  DATEMP_YOUHUAOK;					//  0x0E16003C  温度优化开启与完成标记   0   未优化   1   普通优化  2   强制优化  3 普通优化完成  4 强制优化完成
    UI16  DATEMP_E502_VER;					//  0x0E16003D	温度扩展板E502版本号	20190410
    
	UI16  WRITE_CNT_E502;					//	0x0E16003E	E502通讯测定写入次数		20190410
	UI16  READ_CNT_E502;					//	0x0E16003F	E502通讯测试读入次数		20190410
	
	UI16  GETTEMPONTM_60s[TEMP_E502_CH_NUM_DIC];    //  0x0E160040 -  0x0E16004F 60s内电热输出时间
    UI16  DATEMP_VALUE[TEMP_E502_CH_NUM_DIC];       //  0x0E160050-  0x0E16005F热电偶读取实际AD值
	
    UI16 TM_COUNTDOWNTOWARM;                // 0x0E160060 转保温倒计时
	
	UI16  RAMP_OPTIMIZE;                    //0x0E160061 缓冲区模型，优化标记      0:未自动优化1:表示已自动优化 
}TEMPCTRLCOMM_STATE, *PTEMPCTRLCOMM_STATE;

#pragma pack()

#ifdef DIC_ENUM_NEED

#define     ER0_WUPT_COMM		0x0001      // #0E100；预温计时未到  
#define     ER0_HTON_COMM     	0x0002      // #0E101；温度没有上升  
#define     ER0_TMPHLD_COMM		0x0004      // #0E102；取消保温模式
#define     ER0_HEATTOUCH_COMM  0x0008      // #0E103；加热接触器异常
#define     ER0_TEMPER_COMM     0x0010      // #0E104；料筒温度偏差
#define		ER0_COORER_COMM		0x0020		// #0E105；落料口温度偏高
#define		ER0_STDY_COMM		0x0040		// #0E106；温度优化开启
#define		ER0_STDYER_COMM		0x0080		// #0E107；温度优化条件不符合
#define		ER0_SYNCER_COMM		0x0100		// #0E108；未进行自学习，同步升温无效
#define 	ER0_E502_ERR		0x0200		// #0E109；E502扩展板通讯失败	20190404
#define 	ER0_TEMP_SET_LOW	0x0400		// #0E10A；温度设定值须大于200度
#define 	ER0_TEMP_REAL_HIGH	0x0800		// #0E10B；温度实际值太高

#define		ER5_E502_J_INVALID	0x1000		// #0E10C: E502热电偶选择失败	20190718HH  
#define		ER1_TEMP_ERRPRO		0x2000		// #0E10D; 温度异常(热电偶短线:1,2,3/热电偶反接:4,5/温度读取失败:6)
#define     ER5_WARMOFF     	0x4000      // #0E10E; 手动时长时间无操作，切电热
#define 	ER5_TEMP_TO_HOLD	0x8000		// #0E10F; 已自动转入保温状态，请按手动键取消
#define     ER5_YOUHUABREAK     0x00010000  // #0E110; 温度优化中断
#define     ER5_YOUHUAFINISH    0x00020000  // #0E111; 温度优化完成
/* 新增热电偶呆滞相关保护警报 */
#define 	ER1_THERMOCOUPLE	0x00040000  // #0E112  第x段温度未上升，请检查热电偶

#define     ER0_TEMPHIGH_COMM   0x00080000  // #0E113  温度超过保护上限


#endif

#endif


