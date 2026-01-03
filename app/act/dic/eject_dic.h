#ifndef _EJECT_DIC_H_
#define _EJECT_DIC_H_
#include "typedef.h"
#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#pragma pack(1)

typedef struct EN_EJECT_PO
{
    UI16  O_EJT_FWD ;                   //0x21100000 托进         
    UI16  O_EJT_FSET;                   //0x21100001 托进设定
    UI16  O_EJT_FRSV;                   //0x21100002 托进备用
    UI16  O_EJT_BWD ;                   //0x21100003 托退
    UI16  O_EJT_BSET;                   //0x21100004 托退设定
    UI16  O_EJT_BRSV;                   //0x21100005 托退备用
}EJECT_PO, *PEJECT_PO;

typedef struct EN_EJECT_PI
{
    UI16  I_EJT_FWD ;                   //0x21110000  ;托进终
    UI16  I_EJT_BWD ;                   //0x21110001  ;托退终      ; EJECT BWD SENSOR
	
    UI16  I_EJT_ACT ;                   //0x21110002  ;启动托模     ; EJECT ACTIVE (8727)
    UI16  I_EJT_FWS ;                   //0x21110003  ;托进慢速     ; EJECT FWD SLOW  ;;(0115)
    UI16  I_EJT_PROT;                   //0x21110004  ;顶针保护 ;	  07-01-29
}EJECT_PI, *PEJECT_PI;

typedef struct EN_EJECT_MACHSET
{
	/*斜率+液压动作前后延时+阀动作前后延时+其他机器参数*/

    UI16  EJT_METERCH;                          /* 21120000 托模部件电子尺通道号*/
    UI16  DAPRES_EJTRAMP;                       /* 21120001 托模启动压力斜率       .01S    默认0.20		范围0.00-1.99*/
    UI16  DAFLOW_EJTRAMP;                       /* 21120002 托模启动速度斜率       .01S    默认0.20		范围0.00-1.99*/
    UI16  DAPRES_EJTOFFRAMP;                    /* 21120003 托模关闭压力斜率       .01S    默认0.00		范围0.00-1.99*/
    UI16  DAFLOW_EJTOFFRAMP;                    /* 21120004 托模关闭速度斜率       .01S    默认0.00		范围0.00-1.99*/

    UI16  DAPRES_EJTSETUP;                      /* 21120005 托模慢速调试压力               默认30		范围0-托模压力上限*/
    UI16  DAFLOW_EJTSETUP;                      /* 21120006 托模慢速调试速度               默认30		范围0-托模速度上限*/

    UI16  ADPOSI_EJTRETEFF;                     /* 21120007 托模位置有效区                 默认1.0  范围0~9.9*/
    UI16  ADPOSI_EJTRETMAX;                     /* 21120008 托模退位置允许最大值              默认2.0  范围0~9.9*/
    UI16  TM_EJEENDCHK;                         /* 21120009 托模终滤波时间         .01S    默认0.50		范围0.00-9.99*/

    UI16  EJTF_HYDELAY;                         /* 2112000A 托进液压延迟			默认0.000		范围0.000-9.990*/
    UI16  EJTB_HYDELAY;                         /* 2112000B 托退液压延迟			默认0.000		范围0.000-9.990*/

    UI16   TMDLY_VLV_ONEJTF;                    /* 2112000C 托模进阀开延迟			默认0.00		范围0.00-5.00*/
    UI16   TMDLY_VLV_OFFEJTF;                   /* 2112000D 托模进阀关延迟			默认0.00		范围0.00-5.00*/
    UI16   TMDLY_VLV_ONEJTB;                    /* 2112000E 托模退阀开延迟			默认0.00		范围0.00-5.00*/
    UI16   TMDLY_VLV_OFFEJTB;                   /* 2112000F 托模退阀关延迟			默认0.00		范围0.00-5.00*/
    UI16   TMDLY_EJT_FWDEND;                    /* 21120010 托模进结束延迟, 0.01s			默认0.00		范围0.00-9.99*/
    UI16   TMDLY_EJT_BWDEND;                    /* 21120011 托模退结束延迟, 0.01s			默认0.00		范围0.00-9.99*/
    UI16   DAPRES_EJTHOLD;                      /* 21120012 托模进保持压力               默认30		范围0-托模压力上限		20190409*/
    UI16   DAFLOW_EJTHOLD;                      /* 21120013 托模进保持流量              默认30		范围0-托模速度上限		20190409*/
    UI16   TM_EJTHOLD;                      	/* 21120014 托模进保持时间             默认0.00		范围0.00-99.99					20190409*/
    //托模模油缸
    UI16	DIA_CYLINDER_E;						/* 21120015 油缸直径 0.0-999.9 mm   默认60.0*/
    UI16	DIA_PISDIA_E; 						/* 21120016 活塞杆直径 0.0-999.9 mm  默认30.0*/
    UI16	EFFICIENCY_FACTOR_EJF;				/* 21120017 托进动能转换效率 默认:1.00	范围:0.10~5.00*/
    UI16	EFFICIENCY_FACTOR_EJB;				/* 21120018 托退动能转换效率 默认:1.00	范围:0.10~5.00*/
	UI16	DIA_LEFT_RIGHT;						/* 21120019 托模活塞杆 默认: 0:默认	范围:0:默认 1:反向*/
	UI16	DIA_OIL_NUM;						/* 2112001A 托模油缸数 默认: 1	范围:1~4*/

	UI16  EJEFWD_PRESS;							/* 2112001B 托进最小压力范围0-20  默认5*/
	UI16  EJEFWD_FLOWS;						  	/* 2112001C 托进最小速度范围0.0-99.9  默认5.0*/
	UI16  EJEBWD_PRESS;							/* 2112001D 托退最小压力范围0-20  默认5*/
	UI16  EJEBWD_FLOWS;						  	/* 2112001E 托退最小速度范围0.0-99.9  默认5.0*/

	UI16	SMOOTH_LEVEL_EJF;					/* 2112001F 托进平滑	默认	0	范围0~100	*/
	UI16	SMOOTH_LEVEL_EJB;					/* 21120020 托退平滑	默认	0	范围0~100	*/

	UI16	EJT_OPEN;							/* 21120021	托模开环	默认	0不使用	范围0:不使用	1:使用*/
}EJECT_MACHSET, *PEJECT_MACHSET;

typedef struct EN_EJECT_MOLDSET
{
	/*功能+警报参数*/
    UI16  TM_DLYEJTADV;                         /* 21130000 前延时（托模进）          .01S    默认0.50	范围0.00-99.99 20200219 */
    UI16  TM_DLYEJTRET;                         /* 21130001 前延时（托模退）          .01S    默认0.50	范围0.00-99.99 20200219 */
    UI16  FL_EJTCTRL;                           /* 21130002 托模功能选择 0=停留 1=定次 2=震动       3 递增  默认1*/
    UI16  CN_EJT;                               /* 21130003 托模次数                                默认2*/
	
    UI16  FL_EJTVIBRATEMODE;                    /* 21130004 震动托模方式 0 时间 1 位置  默认0*/
    UI16  TM_EJTVIBRATE;                        /* 21130005 震动托模计时   		默认0.00		范围0.00-9.99            */
    UI16  ADPOSI_EJTVIBRATEDISTANCE;            /* 21130006 震动托模距离    		默认10.0		  */
    UI16  DAPRES_EJTVIBRATE;                    /* 21130007 震动托模压力               */
    UI16  FL_EJTSPECIAL;                        /* 21130008 停留时托进保持	0：不使用	1：使用				默认0*/
	
    UI16  TM_EJTDEFAULTMAX;                     /* 21130009 托模计时警报上限内设值    0.00-99.99 默认20.00    20200219 */
    UI16  FL_EJTFIRSTSET;                       /* 2113000A 托模首次独立行程功能 0 不使用 1 使用 					默认不使用*/
    UI16  FL_CTRLMODE;                          /* 2113000B 托模控制方式   0：时间  1：位置 默认时间*/
	
    UI16  Qc_DeviationSet_EjtTm;                /* 2113000C 顶针时间允许偏差     默认0.00*/
    UI16  Qc_TmSet_EjtTm;                       /* 2113000D 顶针时间设定值   默认0.00*/
    UI16  Qc_Mode_EjtTm;                        /* 2113000E 顶针时间监控:0:不使用  1:使用   默认不使用*/
	
    UI16  FL_ejectCnt;                          /* 2113000F 手动托进次数   0  多次  1   单次    默认多次*/
    UI16  FL_EjtAdvDlyMd;                     	/* 21130010 托进前延迟方式   0  首次  1   末次    2 每次		默认0*/
    UI16  FL_EjtRetDlyMd;                     	/* 21130011 托退前延迟方式   0  首次  1   末次    2 每次		默认1*/
    UI16  TM_EJETFWD_ALARM;                     /* 21130012 托模进计时警报上限  范围0.00-99.99   默认30.00*///20180509 CHJ
    UI16  TM_EJETBWD_ALARM;                     /* 21130013 托模退计时警报上限  范围0.00-99.99   默认30.00*/

    //20190403 CHJ 补齐
    UI16  FL_EJETFWD_DOWN;					    /* 21130014 托进缓冲	 0   不使用   1   使用   默认不使用*/
    UI16  FL_EJETBWD_DOWN;					    /* 21130015 托退缓冲	 0   不使用   1   使用   默认不使用*/
    UI16  MODULUS_EJETFWD;					    /* 21130016 托进缓冲系数 范围0-2500，默认210*/
    UI16  MODULUS_EJETBWD; 					    /* 21130017 托退缓冲系数 范围0-2500，默认210*/
    UI16  EJECTBUFFUSED;					    /* 21130018 托模缓冲 0  不使用1 使用，默认不使用，放在系统参数画面*/

	UI16  FL_INJEJECT;	  						/* 21130019 进料口切断: 0不使用/1使用  默认不使用*/
	UI16  TM_INJEJECTDLY;						/* 2113001A 切割进延时: 范围0.00-99.99s  默认0.00s */
	UI16  TM_INJEJECT;							/* 2113001B 切断脱模进保持计时: 范围0.00-9.99  默认0.00*/
	UI16  FL_INJEJECTBWD;						/* 2113001C 切断后脱模退: 0不使用/1使用 默认不使用*/

	/*================================新油路算法============================================*/
	UI16  EJEF_MAX_SPD;							/* 2113001D 托模进最大斜率(速度)	默认500	范围100-10000%/s*/
	UI16  EJEF_MAX_PRES;						/* 2113001E 托模进最大斜率(压力)	默认2000	范围100-60000bar/s*/
	UI16  EJEB_MAX_SPD;							/* 2113001F 托模退最大斜率(速度)	默认500	范围100-10000%/s*/
	UI16  EJEB_MAX_PRES;						/* 21130020 托模退最大斜率(压力)	默认2000	范围100-60000bar/s*/
	
	UI16  FL_EJEF_SMOOTH;						/* 21130021 托进平稳(整个托模)  0   不使用   1   托进平稳   默认不使用*/
	UI16  FL_EJEB_SMOOTH;						/* 21130022 托退平稳  0   不使用   1   托退平稳   默认不使用*/
	
	UI16  PUMP_K_EJEFWD;						/* 21130023 托模进缓冲系数默认1.20    范围0.20-5.00    */
	UI16  PUMP_DEAD_EJEFWD;						/* 21130024 托模进死区时间默认0.000	范围0.000-2.000   单位ms*/
	UI16  PUMP_K_EJEBWD;						/* 21130025 托模退缓冲系数默认1.20    范围0.20-5.00    */
	UI16  PUMP_DEAD_EJEBWD;						/* 21130026 托模退死区时间默认0.000	范围0.000-2.000   单位ms*/

	UI16  EJTF_SPD_STARTON;           			/* 21130027 托进开始速度斜率 默认100，范围0-100%*/   
	UI16  EJTF_SPD_HALFWAY;           			/* 21130028 托进中间速度斜率 默认100，范围0-100%*/    
	UI16  EJTF_SPD_STOPOFF;           			/* 21130029 托进停止速度斜率 默认100，范围0-100%*/

	UI16  EJTF_PRES_STARTON;           			/* 2113002A 托进开始压力斜率 默认100，范围0-100%*/   
	UI16  EJTF_PRES_HALFWAY;           			/* 2113002B 托进中间压力斜率 默认100，范围0-100%*/    
	UI16  EJTF_PRES_STOPOFF;           			/* 2113002C 托进停止压力斜率 默认100，范围0-100%*/
	
	UI16  EJTB_SPD_STARTON;           			/* 2113002D 托退开始速度斜率 默认100，范围0-100%*/  
	UI16  EJTB_SPD_HALFWAY;           			/* 2113002E 托退中间速度斜率 默认100，范围0-100%*/    
	UI16  EJTB_SPD_STOPOFF;           			/* 2113002F 托退停止速度斜率 默认100，范围0-100%*/
	
	UI16  EJTB_PRES_STARTON;           			/* 21130030 托退开始压力斜率 默认100，范围0-100%*/  
	UI16  EJTB_PRES_HALFWAY;           			/* 21130031 托退中间压力斜率 默认100，范围0-100%*/    
	UI16  EJTB_PRES_STOPOFF;           			/* 21130032 托退停止压力斜率 默认100，范围0-100%*/  

    UI16  FL_INJEJTBWDBEFCLAMPOPN;				/* 21130033 开模前托退 0-不使用 1-使用 默认0 */
}EJECT_MOLDSET, *PEJECT_MOLDSET;

typedef struct EN_EJECT_ACTPARA
{
	/*PF+POS+TIME+HYDR(PORT;PUMPS)+SERVO(PORT;PUMPS;PID)*/
    UI16  DAPRES_EJTADVSTARTFIRST;              /* 21140000 托模进第一次启动压力				默认40	范围0-托模压力上限       */
    UI16  DAPRES_EJTADV;                        /* 21140001 托模进压力  					默认50	范围0-托模压力上限                      */
    UI16  DAPRES_EJTRETFIRST;                   /* 21140002 托模退第一次启动压力				默认50	范围0-托模压力上限              */
    UI16  DAPRES_EJTRET;                        /* 21140003 托模退压力 					默认60	范围0-托模压力上限                       */
    UI16  DAFLOW_EJTADVSTARTFIRST;              /* 21140004 托模进第一次启动速度       		默认50	范围0-托模速度上限*/
    UI16  DAFLOW_EJTADV;                        /* 21140005 托模进速度              		默认50	范围0-托模速度上限      */
    UI16  DAFLOW_EJTRETFIRST;                   /* 21140006 托模退第一次启动速度   			默认50	范围0-托模速度上限       */
    UI16  DAFLOW_EJTRET;                        /* 21140007 托模退速度               	默认50	范围0-托模速度上限     */
	
    UI16  ADPOSI_EJTADVSTARTFIRSTEND;          /* 21140008 托模进第一次启动终止位置				默认0.0   */
    UI16  ADPOSI_EJTADVFASTEND;                /* 21140009 托模进终止位置     				默认100.0        */
    UI16  ADPOSI_EJTRETFIRST;                  /* 2114000A 托模退第一次启动终止位置   			默认20.0*/
    UI16  ADPOSI_EJTRETEND;                    /* 2114000B 托模退终止位置             		默认0.0*/
	
    UI16  TM_EJT1STROUND;                      /* 2114000C 托模进计时         范围0.00-99.99   默认2.00    20200219        */
    UI16  TM_EJT2NDROUND;                      /* 2114000D 托模退计时          范围0.00-99.99   默认2.00    20200219         */

    UI16  ADPOSI_EJTFFIRSTEND1;                /* 2114000E 托模进一段首次终止位置   			默认0.0	*/
    UI16  ADPOSI_EJTFFIRSTEND2;                /* 2114000F 托模进二段首次终止位置  			默认0.0   */
    UI16  ADPOSI_EJTBFIRSTEND1;                /* 21140010 托模退一段首次终止位置				默认0.0   */
    UI16  ADPOSI_EJTBFIRSTEND2;                /* 21140011 托模退二段首次终止位置 				默认0.0   */

	UI16  DAPRES_INJEJT;						/* 21140012  切断进压力	  默认50   */
	UI16  DAFLOW_INJEJT;						/* 21140013  切断进速度	 默认50   */
	UI16  ADPOSI_INJEJT;						/* 21140014  切断脱模进位置   默认5.0mm	  */

	UI16  PER_DAFLOW_EJECT;				   	    /* 21140015 托模速度百分比  范围 0-100  默认 0 */

	UI16  DAPRES_INJEJTBWD;						/* 21140016 切断退压力	范围0-140bar 默认50bar */
	UI16  DAFLOW_INJEJTBWD;						/* 21140017 切断退速度 范围0-99% 默认50% */
    UI16  TM_CUTFWD;							/* 21140018 切割进时间 范围0.00-99.99s 默认5.00s */
	UI16  TM_CUTBWD;							/* 21140019 切割退时间 范围0.00-99.99s 默认5.00s */
	UI16  TM_INJEJECTBWDDLY;					/* 2114001A 切割退动作延时 范围0.00-99.99s 默认0.00s */
}EJECT_ACTPARA, *PEJECT_ACTPARA;

typedef struct EN_EJECT_HYDR
{
    UI16  HYDR_PORT_EJECT;						//21150000 托模的DA输出通道：位操作-->port1……port4
    UI16  HYDR_PUMPS_EJECTSETUP;				//21150001 托模慢速调试的泵浦限制:位操作-->P1……P7
    UI16  HYDR_PUMPS_EJECT;						//21150002 托模动作的泵浦限制:位操作-->P1……P7
    UI16  SERVO_KP_EJECT;						//21150003 托模动作的PID_KP			默认800	范围0-2500
    UI16  SERVO_KI_EJECT;						//21150004 托模动作的PID_KI			默认40	范围0-200
    UI16  SERVO_KD_EJECT;						//21150005 托模动作的PID_KD
}EJECT_HYDR, *PEJECT_HYDR;
typedef struct EN_EJECT_STATE
{
	UI16 FL_ERROR0;								//21160000 警报1
	UI16 FL_ERROR1;								//21160001 警报2
    UI16 TM_EJECT;                         		//21160002 托模计时实际值 0.01s
    UI16 TM_EJTCTFWD;                           //21160003 托模进计时        0.00
    UI16 TM_EJTCTBWD;                           //21160004 托模退计时        0.00
    /*        QC监控参数          20140706*/
    UI16  Qc_Avg_EjtTm;                         //21160005 顶针时间平均值
    UI16  Qc_Deviation_Max_EjtTm;               //21160006 顶针时间最大偏差值
	UI16  Qc_Unq_Cnt_EjtTm;                     //21160007 顶针时间不合格数
	UI16  Qc_Unq_Rate_EjtTm;                    //21160008 顶针时间不合格率
	UI16  Qc_Max_EjtTm;                         //21160009 顶针时间最大值
	UI16  Qc_Min_EjtTm;	                        //2116000A 顶针时间最小值		
    UI16  Step_EjectFwd;                        //2116000B 托模进实时动作步         成品画面显示
    UI16  Step_EjectBwd;                        //2116000C 托模退实时动作步         成品画面显示
    UI16  Step_EjectPro;                        //2116000D 托模调度实时动作步       成品画面显示
    UI16  EJEFWD_SPEED;							//2116000E	托模进速度  mm/ms
	UI16  EJEBWD_SPEED;							//2116000F	托模退速度  mm/ms	
	UI16  TM_EJECT_START;						//21160010 托模计时起始时间		    单位:10ms     去除该功能，面板画线改为由0开启，否则所有相关的都得32位，数据太多 20200219
    UI16  TM_EJECT_RT;							//21160011 托模计时实时值用于面板显示		    单位:10ms							

	UI16  POS_EJEFWDMAX;						//21160012 托进终点	单位0.0mm		成品画面显示				
}EJECT_STATE, *PEJECT_STATE;

#pragma pack()

#ifdef DIC_ENUM_NEED
/*ACT SN start from 1*/

enum EN_EJECT_ACT_SN
{
    OS_EJT = 1,				// 2111	 	托模调度动作
    OS_EJTF,				// 2112     托模进
    OS_EJTB, 				// 2113     托模退

    OS_AEJTF,				// 2114    调模托模进
    OS_AEJTB,				// 2115    调模托模退

    OS_VEJTF,				// 2116    托模进
    OS_VEJTB, 				// 2117    托模退

	OS_CUTHEADF,			// 2118		切料头进		
	OS_CUTHEADB,		    // 2119 	切料头退

	EJECT_ACT_SN_END
};

enum EN_EJECT_ACTNAME
{
        EJECTFWD	= 	STAGE_1_STEP,	//0x21121000 托模进1段
        EJECTFWD2	=	STAGE_2_STEP,	//0x21121100 托模进2段
		
        EJECTBWD	=	STAGE_1_STEP,	//0x21131000 托模退1段
        EJECTBWD2	=	STAGE_2_STEP,	//0x21131100 托模退2段
        EJECTBWD3	=	STAGE_3_STEP,	//0x21131200 托模退3段
};

#define     ER0_EJTP      0x0001      //  #21100	托模未到定位  
#define     ER0_EJTB      0x0002      //  #21101	托退未到定位
#define     ER0_EJTM      0x0004      //  #21102  	托模方式错误
#define     ER0_EJTPM     0x0008      //  #21103    模具顶针保护
#endif

#ifdef __cplusplus
}
#endif

#endif




