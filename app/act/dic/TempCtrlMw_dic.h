#ifndef _TEMPCTRLMW__DIC_H_
#define _TEMPCTRLMW_DIC_H_


#include "typedef.h"
#include "admeter_dic.h"

#pragma pack(1)


#define	TEMP_MW_CH_NUM	16	//E501A模温扩展板最大资源每块16个采集

typedef struct EN_TEMPCTRLMW_PO
{
	UI16 O_COOL_MW  ;         // 0x2F100000  	冷却水     
}TEMPCTRLMW_PO, *PTEMPCTRLMW_PO;

typedef struct EN_TEMPCTRLMW_PI
{ 
    UI16 I_HEAT_MW;        // 0x2F110000 	加热接触器
}TEMPCTRLMW_PI, *PTEMPCTRLMW_PI;


typedef struct  EN_TEMPMW_MACHSET
{
    UI16 TEMP_PROTECT_MW;                   		/* 0x2F120000 温度保护上限  默认400   范围350-525*/  /*该参数暂时先放在20110画面*/
	UI16 TEMP_CONTROL_MW;							/* 0x2F120001 第1,2..9块温度控制   0   主板  1  E501扩展板    默认 0   */
	UI16 TEMP_CANLINK;								/* 0x2F120002 模温板can接线   0   can1     1   can2*/
}TEMPMW_MACHSET, *PTEMPMW_MACHSET;

typedef struct EN_TEMPMW_MOLDSET
{
	UI16 DATEMP_SETTING_MW[TEMP_MW_CH_NUM]; 	/* 0x2F130000-0x2F13000F 温度设定值[16]          默认:200			范围:0 ~ TempMax*/
	UI16 DATEMP_FUNC_MW[TEMP_MW_CH_NUM];  		/* 0x2F130010-0x2F13001F 温控功能选择(0为油温段)默认:0	0:关闭	1:只显示	2:手动 3:PID 4:PD渐近*/
	UI16 DAPID_KP_MW[TEMP_MW_CH_NUM]; 			/* 0x2F130020-0x2F13002F 温度P系数				 默认8.0	范围0-6553.5		*/
	UI16 DAPID_TI_MW[TEMP_MW_CH_NUM]; 			/* 0x2F130030-0x2F13003F 温度I系数				 默认348.0	范围0-6553.5   	*/
	UI16 DAPID_TD_MW[TEMP_MW_CH_NUM]; 			/* 0x2F130040-0x2F13004F 温度D系数				 默认87.0	范围0-6553.5 	*/
	UI16 DATEMP_RAMP_MW[TEMP_MW_CH_NUM];		/* 0x2F130050-0x2F13005F 温度加温缓冲区[16](第一段温度为0)	  默认:30				范围:0 ~ 99 */
	UI16 TM_RESPONSELESS_MW[TEMP_MW_CH_NUM];	/* 0x2F130060-0x2F13006F 温度输出周期时间(0为油温段)	默认:6    范围:2 ~ 12 	*/
	UI16 DATEMP_MAX_MW[TEMP_MW_CH_NUM];       	/* 0x2F130070-0x2F13007F 上偏差[16]            默认:50			范围:0 ~ 100	*/
	UI16 DATEMP_MIN_MW[TEMP_MW_CH_NUM];       	/* 0x2F130080-0x2F13008F 下偏差[16]            默认:15			范围:0 ~ 100	*/
 	UI16 FL_TKTJ_MW[TEMP_MW_CH_NUM];			/* 0x2F130090-0x2F13009F 热电偶类型  默认:0:	0:k-型	1:J-型	*/
 	UI16 DACENT_HEATERON_MW[TEMP_MW_CH_NUM];	/* 0x2F1300A0-0x2F1300AF 手动比例(0为油温段)  	  默认:50.0			范围:0.0 ~ 100.0	*/
	UI16 FL_TEMPHOLD_MW;                   		/* 0x2F1300B0 保温功能  	 	默认:0		范围:0 ~ 1	*/
	UI16 DATEMP_HOLD_MW;                    	/* 0x2F1300B1 保温温度       	默认:100		范围:50 ~ 300	*/
	UI16 DATEMP_SCTM_MIN_MW; 					/* 0x2F1300B2 最小扫描周期	默认:2		范围:1 ~ 10 */		
	UI16 FL_TEMPUPCHECK_MW;			  			/* 0x2F1300B3 电热连续加温检查  	0	不使用  1  使用			默认1	*/
	UI16 DATEMP_NUM_MW;							/* 0x2F1300B4 温度段数	 	默认:8		范围:5 ~ 10 */
	UI16 TM_HEATERWARM_MW;                 		/* 0x2F1300B5 预温计时     	默认:0		范围:0 ~ 99	*/
	UI16 FL_USE_MW;								/* 0x2F1300B6 模温功能   0   关闭   1   开启   默认关闭*/
	UI16 FL_TEMP_YH;							/* 0x2F1300B7 温度优化命令*/
	UI16 FL_TEMP_YHQZ;							/* 0x2F1300B8 温度强制优化命令*/
	UI16 CONFUNC_MW;							/* 0x2F1300B9 同步升温功能			  默认:0			范围:0:关闭	1:开启*/
	UI16 FL_INJ_ALLOW;							/* 0x2F1300BA 射出检测模温温度   0   检测   1  不 检测  默认0 检测 该参数不需要发送给模温板,第一个部件控制所有*/
}TEMPMW_MOLDSET, *PTEMPMW_MOLDSET;

typedef struct EN_TEMPCTRLMW_STATE
{
	UI16  FL_ERROR0;						// 0x2F160000	警报1		用于模温上传过来的警报
	UI16  FL_ERROR1;						// 0x2F160001	警报2		用于自己本身判断时出现的警报
	
	UI16  DATEMP_REAL_MW[TEMP_MW_CH_NUM]; 	// 0x2F160002-0x2F160011	温度实际值[16]	范围:0 ~ TempMax	
	UI16  FL_STATUS_MW[TEMP_MW_CH_NUM];		// 0x2F160012-0x2F160021	温度状态[16]		范围:0:关闭 1:PID 2:超温停止 3:全功率 4:警报
	UI16  OUTPUT_PER_MW[TEMP_MW_CH_NUM];		// 0x2F160022-0x2F160031	温度输出实际值百分比[16](0为油温段)	范围:0.0~100.0%

	UI16  WTEMP_LM73_MW;					// 0x2F160032	室温的温度
	UI16  FL_HEATERSTATUS_MW;				// 0x2F160033	电热状态
    UI16  DATEMP_VER_MW;					// 0x2F160034	温度扩展板E501版本号
    UI16  DATEMP_ERR_MW;					//0x2F160035  模温扩展板错误状态
	UI16  WRITE_CNT_E501;					// 0x2F160036	E501通讯测试写入次数		20190410
	UI16  READ_CNT_E501;					// 0x2F160037	E501通讯测试读入次数		20190410
	UI16  MW_YOUHUAXUQIU;					// 0x2F160038	温度优化提示开电热的闪烁指示
	UI16  MW_YOUHUAOK;						// 0x2F160039  温度优化开启与完成标记   0   未优化   1   普通优化  2   强制优化  3 普通优化完成  4 强制优化完成
	UI16  WSTATERSV6;						// 0x2F16003A	预温倒计时*/
}TEMPCTRLMW_STATE, *PTEMPCTRLMW_STATE;

#pragma pack()

#ifdef DIC_ENUM_NEED

#define     ER0_WUPT_MW			0x0001      // #2F100；模温预温计时未到  
#define     ER0_HTON_MW     	0x0002      // #2F101；模温温度没有上升  
#define     ER0_TMPHLD_MW		0x0004      // #2F102；模温取消保温模式
#define     ER0_TEMPER_MW     	0x0008      // #2F103；模温温度偏差
#define		ER0_STDY_MW			0x0010		// #2F104；模温温度优化开启
#define		ER0_STDYER_MW		0x0020		// #2F105；模温温度优化条件不符合
#define 	ER0_SET_LOW_MW		0x0040		// #2F106；模温温度设定值须大于150度
#define 	ER0_REAL_HIGH_MW	0x0080		// #2F107；模温温度实际值太高
#define		ER0_J_INVALID_MW	0x0100		// #2F108:    模温热电偶选择失败
#define		ER0_TEMP_ERRMW		0x0200		// #2F109;    模温温度异常(热电偶短线:1,2,3/热电偶反接:4,5/温度读取失败:6)
#define		ER0_DATA_FAILED		0x0400		// #2F10A;    模温(1-7)参数发送失败
#define 	ER0_E501_ERR		0x0800		// #2F10B；E501扩展板通讯失败	
#define		ER0_WTEMP_LM73		0x1000		// #2F10C:    室温偏差
#define		ER0_SYNCER_MW		0x2000		// #2F10D: 未进行自学习，同步升温无效

#define     ER1_TEMPER_MW     	0x10000      //#2F110；模温温度偏差

#endif

#endif

