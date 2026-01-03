#ifndef _CLAMP_DIC_H_
#define _CLAMP_DIC_H_

#ifdef __cplusplus
extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"

#pragma pack(1)
typedef struct EN_CLAMP_PO
{
    //关模
    UI16  O_CLS_SOL ;              	 // 0x20100000 关模     
    UI16  O_CLS_ULT ;                // 0x20100001 差动         
    UI16  O_CLS_FST ;                // 0x20100002 关模快速阀   
    UI16  O_CL_SETU ;                // 0x20100003 关模设定 
    UI16  O_CL_HIP  ;                // 0x20100004 关模高压 
    UI16  O_RSV_CLS;                 // 0x20100005 关模备用阀   
    UI16  O_CLS_LOW;                 // 0x20100006 关模低压阀

    //开模
    UI16  O_OPN_SOL ;                // 0x20100007 开模       
    UI16  O_REL_PRS ;                // 0x20100008 泄压       
    UI16  O_GUIDE   ;                // 0x20100009 引导阀   
    UI16  O_OPN_FST ;                // 0x2010000A 开模快速     
    UI16  O_OP_SETU ;                // 0x2010000B 开模设定
    UI16  O_OP_SSLW ;                // 0x2010000C 开模慢速
    UI16  O_MEC_SAF_AIR;             // 0x2010000D 机械保险气缸
    UI16  O_RSV_OPN;                 // 0x2010000E 开模备用阀   
    UI16  O_OPN_REL;                 // 0x2010000F 开消阀
    UI16  O_OPN_CLS;                 // 0x20100010 开关模辅助     
    UI16  O_OPN_BAC;                 // 0x20100011 开模背压
    UI16  O_CLAMP_FST;               // 0x20100012 开关模快速			[宁塑]
    UI16  O_CLS_ULT2;                // 0x20100013 差动2
    UI16  O_CL_TOHI;                 // 0x20100014 关摸至高压
    UI16  O_OP_ESLW;                 // 0x20100015 开模终慢 
    UI16  O_CLS_SOL2;                // 0x20100016 关模安全  
    UI16  O_OPN_LOW;                 // 0x20100017 开模低压阀			16-03-11震宇
    UI16  O_CLS_ULT3;                // 0x20100018 差动3				16-06-24 威压程序
    UI16  O_CLS_HLP;                 // 0x20100019 差动辅助   			2016-07-13   威亚使用  对应威亚PC11
    UI16  O_OPN_HLP;                 // 0x2010001A 开模辅助   			2016-07-13   威亚使用  对应威亚PC20
	UI16  O_ERR_PRU;                 // 0x2010001B 不良产品     //20180409 LET 

	UI16  O_SPC_CLS;				  //0x2010001C  特殊关模阀
	UI16  O_SPC_OPN;				  //0x2010001D  特殊开模阀
	UI16  O_CLS_TOTAL;				  //0x2010001E  锁模总阀
	UI16  O_CLSOPN_STEND;			 // 0x2010001F  开合模始末
}CLAMP_PO, *PCLAMP_PO;

typedef struct EN_CLAMP_PI
{
	//关模
	UI16  I_CLS_SLW ;          	    // 0x20110000 关模低速 
	UI16  I_CLS_LOW ;               // 0x20110001 关模低压
	UI16  I_CLS_HIP ;               // 0x20110002 关模高压
	UI16  I_CLS_END ;               // 0x20110003 关模终 
	UI16  I_CLS_LMT ;               // 0x20110004 关模限位
	UI16  I_CLS_FST ;               // 0x20110005 关模快速
	UI16  I_RSV_CLS ;               // 0x20110006 关模备用
	
	//关模的安全检查信号
	UI16  I_HYD_SAF ;               // 0x20110007 液压安全检知
	UI16  I_MEC_SAF ;               // 0x20110008 机械安全检知

	//开模
	UI16  I_OPN_SLW ;               // 0x20110009 开模一慢
	UI16  I_OPN_FST ;               // 0x2011000A 开模二慢
	UI16  I_OPN_END ;               // 0x2011000B 开模终
	UI16  I_OPN_LMT;                // 0x2011000C 开模限位
	UI16  I_AIR_PRES;               // 0x2011000D 气压检知
	UI16  I_MOLDHEAT;               // 0x2011000E 模温警报
	UI16  I_EMGUP;					// 0x2011000F 紧急回升 
	UI16  I_CLS_LIFT;               // 0x20110010 左合模
	UI16  I_CLS_RIGHT;              // 0x20110011 右合模	
	UI16  I_CLS_UPGA;               // 0x20110012 上顶盖  
	UI16  I_CLP_CHK;                // 0x20110013 模板检知
	UI16  I_ERR_PRU;                // 0x20110014 不良品检知        //20180409 LET 
	UI16  I_CLPSAF_CHK;				// 0x20110015 液压保险检知
}CLAMP_PI, *PCLAMP_PI;

typedef struct EN_CLAMP_MACHSET
{
    UI16 CLMP_METERCH;				/* 20120000 开关模部件电子尺通道号*/
    UI16 CLMP_PRESSCH;				/* 20120001 模腔压力传感器通道 ， 原NOZZLE的压力传感器*/

    /*斜率+液压动作前后延时+阀动作前后延时+其他机器参数*/
    /*所有斜率的范围都为0.00~1.99s                     */
    /*+++++++++开关模斜率+++++++++*/
    UI16   DAPRES_CLSRAMP;                 /* 20120002 关模启动压力斜率       .01S    默认0.50	范围0.00-1.99*/
    UI16   DAFLOW_CLSRAMP;                 /* 20120003 关模启动速度斜率       .01S    默认0.50	范围0.00-1.99*/
    UI16   DAPRES_CLSRAMP2;                /* 20120004 关模快速压力斜率       .01s    默认0.20	范围0.00-1.99*/
    UI16   DAFLOW_CLSRAMP2;                /* 20120005 关模快速速度斜率       .01S    默认0.20	范围0.00-1.99*/
    UI16   DAPRES_CLSRAMPLOWPRES;          /* 20120006 关模低压压力斜率       .01S    默认0.20	范围0.00-1.99*/
    UI16   DAFLOW_CLSRAMPLOWPRES;          /* 20120007 关模低压速度斜率       .01S    默认0.20	范围0.00-1.99*/
    UI16   DAPRES_CLSRAMPHIPRES;           /* 20120008 关模高压压力斜率       .01S    默认0.20	范围0.00-1.99*/
    UI16   DAFLOW_CLSRAMPHIPRES;           /* 20120009 关模高压速度斜率       .01S    默认0.20	范围0.00-1.99*/
    UI16   DAPRES_CLSOFFRAMP;              /* 2012000A 关模关闭压力斜率       .01S    默认0.00	范围0.00-1.99*/
    UI16   DAFLOW_CLSOFFRAMP;              /* 2012000B 关模关闭速度斜率       .01S    默认0.00	范围0.00-1.99*/
    UI16   DAPRES_CLSSETUP;                /* 2012000C 关模慢速调试压力               默认 30 	范围0-开关模压力上限*/
    UI16   DAFLOW_CLSSETUP;                /* 2012000D 关模慢速调试速度               默认 30 	范围0-开关模速度上限*/

    UI16   DAPRES_OPNRAMP;                 /* 2012000E 开模启动压力斜率       .01S		默认0.50		范围0.00-1.99*/
    UI16   DAFLOW_OPNRAMP;                 /* 2012000F 开模启动速度斜率       .01S    	默认0.50		范围0.00-1.99*/
    UI16   DAPRES_OPENFASTRAMP;            /* 20120010 快速1压力斜率       .01s		默认0.20		范围0.00-1.99*/
    UI16   DAFLOW_OPENFASTRAMP;            /* 20120011 快速1速度斜率       .01s    	默认0.20		范围0.00-1.99*/
    UI16   DAPRES_OPENFASTRAMP1;           /* 20120012 快速2压力斜率      .01s    	默认0.20		范围0.00-1.99*/
    UI16   DAFLOW_OPENFASTRAMP1;           /* 20120013 快速2速度斜率      .01s    	默认0.20		范围0.00-1.99*/
    UI16   DAPRES_OPENFASTRAMP2;           /* 20120014 快速3压力斜率      .01s    	默认0.20		范围0.00-1.99*/
    UI16   DAFLOW_OPENFASTRAMP2;           /* 20120015 快速3速度斜率      .01s    	默认0.20		范围0.00-1.99*/
    UI16   DAPRES_OPNRAMP2;                /* 20120016 开模转二慢压力斜率         .01s   默认0.20		范围0.00-1.99*/
    UI16   DAFLOW_OPNRAMP2;                /* 20120017 开模转二慢速度斜率         .01s   默认0.20		范围0.00-1.99*/
    UI16   DAPRES_OPNOFFRAMP;              /* 20120018 开模关闭压力斜率       .01s		默认0.20		范围0.00-1.99*/
    UI16   DAFLOW_OPNOFFRAMP;              /* 20120019 开模关闭速度斜率       .01s    	默认0.20		范围0.00-1.99*/
    UI16   DAPRES_OPNSETUP;                /* 2012001A 开模慢速调试压力               	默认 30		范围0-开关模压力上限*/
    UI16   DAFLOW_OPNSETUP;                /* 2012001B 开模慢速调试速度               	默认 30		范围0-开关模速度上限*/
    /*+++++++++开关模动作阀开关延时+++++++++*/
    /*所有阀延迟都为0.00~9.99s*/
    UI16   TMDLY_VLV_OFFCLSULTRA;          /* 2012001C 关模差动阀延迟关计时    .01s   		默认0.00		范围0.00-5.00*/
    UI16   TMDLY_VLV_ONCLSULTRA;           /* 2012001D 关模差动阀延迟开计时    .01s  		默认0.00		范围0.00-5.00*/
    UI16   TMDLY_VLV_ONCLSLOW;             /* 2012001E 关模低压阀延迟开计时    .01s   		默认0.00		范围0.00-5.00*/
    UI16   TMDLY_VLV_OFFCLSLOW;            /* 2012001F 关模低压阀延迟关计时    .01s   默认0.00*/
    UI16   TMDLY_VLV_OFFCLSEND;            /* 20120020 关模阀延迟关计时        .01s   	默认0.00		范围0.00-5.00*/
    UI16   TMDLY_VLV_OFFOPENSLOWSTART;     /* 20120021 开模慢速阀延迟关计时    .01s   	默认0.00		范围0.00-5.00*/
    UI16   TMDLY_VLV_ONOPENSLOWSTART;      /* 20120022 开模慢速阀延迟开计时    .01s   	默认0.00		范围0.00-5.00*/
    UI16   TMDLY_VLV_OFFOPENEND;           /* 20120023 开模阀延时关计时        .01s   	默认0.00		范围0.00-5.00*/
    /*+++++++++开关模动作延时+++++++++*/
    /*所有阀延迟都为0.00~9.99s*/
    UI16   TMDLY_HYDCLSHIGH;               /* 20120024 高压延迟进计时      .01s		默认0.00		范围0.00-9.99*/
    UI16   TMDLY_CLSENDDLY;                /* 20120025 关模动作结束延时      .01s  		默认0.00		范围0.00-9.99*/
    UI16   TM_OPENSLOWVLV;                 /* 20120026 开模慢速阀动作时间      .01s   	默认0.00		范围0.00-9.99*/
    UI16   TMDLY_OPENEND;                  /* 20120027 开模动作结束延时        .01s		默认0.00		范围0.00-9.99*/

    //范围为模座电子尺行程
    UI16   ADPOSI_EFFCPR;                  /* 20120028 开模位置有效区       .0mm  		默认50.0		范围0.0-400.0*/

    UI16   CLMPCLS_HYDRDELAY;				/* 20120029 关模动作液压延迟				默认0.000		范围0.000-9.999*/
    UI16   CLMPOPN_HYDRDELAY;				/* 2012002A 开模动作液压延迟				默认0.000		范围0.000-9.999*/
    UI16   TMDLY_VLV_ONCLS;					/* 2012002B 关模阀开延迟					默认0.00		范围0.00-5.00*/
    UI16   TMDLY_VLV_ONOPN;					/* 2012002C 开模阀开延迟					默认0.00		范围0.00-5.00*/
    UI16   TMDLY_OPENBEGIN;					/* 2012002D 开模前延时, 0.01s*/
    UI16   TMDLY_CLSBEGIN;					/* 2012002E 关模前延时, 0.01s*/
    UI16   ADPOS_COREOUTEFT;                /* 2012002F 途中中子退下限位置				默认1.0		范围1.0-351.0*/
    UI16   FL_ADPOSROBOT;                   /* 20120030 开模途中机械手功能选择  0:不使用  1:使用	 		默认 0*/
    /*合模新加2014-10-09*/
    UI16   TM_CLS_PROTECT;                  /* 20120031 合模启动慢速保护时间  0.000S  默认0.100S*/
    UI16   FL_CLSHIPRES2;                   /* 20120032 关模高压二段  	0:不使用   1:使用  	默认不使用*/
    UI16   DAPRES_CLSRAMPHIPRES2;           /* 20120033 关模高压2段压力斜率       .01S 	  	默认0.20		范围0.00-1.99*/
    UI16   DAFLOW_CLSRAMPHIPRES2;           /* 20120034 关模高压2段速度斜率       .01S  	  	默认0.20		范围0.00-1.99*/
    UI16   TM_ROBOT_ACT;                    /* 20120035 途中机械手输出时间         			默认0.00S		范围0.00-99.99      20200219*/       
    UI16   DAPRES_SPDOPNRAMP1;              /* 20120036 快速开模启动压力斜率       .01S 		默认0.50		范围0.00-1.99*/
    UI16   DAFLOW_SPDOPNRAMP1;              /* 20120037 快速开模启动速度斜率       .01S 	   	默认0.50		范围0.00-1.99*/
    UI16   DAFLOW_SPDOPNRAMP2;              /* 20120038 快速开模快速速度斜率       .01s    	默认0.20		范围0.00-1.99*/
    UI16   DAFLOW_SPDOPNRAMP3;              /* 20120039 快速开模转慢速度斜率       .01s    	默认0.20		范围0.00-1.99*/
    UI16   DAFLOW_SPDOPNRAMP4;              /* 2012003A 快速开模关闭速度斜率       .01s    	默认0.20		范围0.00-1.99*/
    UI16   TM_CLSHI_DEFAULT;                /* 2012003B 关模高压锁死保护时间           通用国内机默认0分 (表示不使用) 韩国机默认5分   范围0到600分  20200219*/
    UI16   DAPRESS_OPNRAMPSTART;            /* 2012003C 途中开模启动压力斜率				默认1.99		范围0.00-1.99*/
    UI16   DAPFLOW_OPNRAMPSTART;            /* 2012003D 途中开模启动速度斜率				默认1.99		范围0.00-1.99*/
    UI16   ADPOS_OPNSTART;                  /* 2012003E 途中开模启动距离				默认5.0		范围0.0-400.0*/
    UI16   TM_OPNPROTECT;                   /* 2012003F 途中开模启动保护时间				默认0.200		范围0.200-9.999*/
    UI16   TMDLY_VLV_OFFCLSEND2;            /* 20120040 关模安全阀延迟关计时        .01s   	默认0.00		范围0.00-5.00*/
    /*海晶异常开模新加   2015-3-13*/
    UI16  	TM_SCLAMPOPN;                   /* 20120041 异常开模时间   范围 0.00 - 9.99  默认0.00   20200219 */
    UI16  	TM_SCLAPMCLS;            	    /* 20120042 异常关模时间   范围 0.00 - 9.99  默认0.00   20200219*/
    UI16   	SPCLAMP;                        /* 20120043 异常开模功能   15-3-13     */
    UI16    TM_CLAMPOPENULT2;               /* 20120044 开模差动动作时间 0.00-9.99 默认0.00   20200219 */
    UI16    CLSEND;                         /* 20120045 合模结束方式 DK2015.0608, 立式机使用*/

    UI16    FL_CLSOPERRAIRB;                /* 20120046 异常开模吹气  0使用1不使用  默认使用*/               //20170505  LET
    UI16    FL_DROPSENORCHEK;               /* 20120047 检出电眼检测  0开模时 1关模时  默认开模时*/			//20170505  LET

	
	//开关模油缸参数
	UI16	DIA_CYLINDER_M;					/* 20120048 油缸直径 默认:60.0 0.0-999.9 mm*/
	UI16	DIA_PISDIA_M; 					/* 20120049 活塞杆直径 默认:30.0	0.0-999.9 mm*/
	UI16	EFFICIENCY_FACTOR_CLS;			/* 2012004A	关模动能转换效率 默认:1.00	范围:0.10~5.00*/
	UI16	EFFICIENCY_FACTOR_OPN;			/* 2012004B 开模动能转换效率 默认:1.00	范围:0.10~5.00*/
	UI16	DIA_LEFT_RIGHT;					/* 2012004C 开合模活塞杆 默认:0:默认	范围:0:默认	1:反向*/
	UI16	DIA_OIL_NUM;					/* 2012004D 开合模油缸数 默认:1	范围:1~4*/
	
	UI16	LOWPRES_CLS;					/* 2012004E 关模底压	默认	5	范围0-20   单位bar*/	
	UI16	LOWFLOW_CLS;					/* 2012004F 关模底流	默认	2.0	范围0.0-20.0   单位%*/
	UI16	LOWPRES_OPN;					/* 20120050 开模底压 	默认	5	范围0-20   单位bar*/
	UI16	LOWFLOW_OPN;					/* 20120051 开模底流 	默认	2.0	范围0.0-20.0   单位%*/

	UI16	SMOOTH_LEVEL_CLS;				/* 20120052 关模平滑	默认	0	范围0~100	*/
	UI16	SMOOTH_LEVEL_OPN;				/* 20120053 开模平滑	默认	0	范围0~100	*/

	UI16   DAPRESS_OPNMIDSTART;            /* 20120054 途中开模启动压力*/
	UI16   DAPFLOW_OPNMIDSTART;            /* 20120055 途中开模启动流量*/   
	UI16   DAPRESS_CLSMIDSTART;            /* 20120056 途中关模启动压力*/
	UI16   DAPFLOW_CLSMIDSTART;            /* 20120057 途中关模启动流量*/	
	UI16   DAPRESS_CLSMIDRAMPSTART;        /* 20120058 途中关模启动压力斜率*/
	UI16   DAPFLOW_CLSMIDRAMPSTART;        /* 20120059 途中关模启动流量斜率*/	 
	UI16   ADPOS_CLSMIDSTART;              /* 2012005A 途中关模启动距离*/
	UI16   TM_CLSMIDPROTECT;               /* 2012005B 途中关模启动保护时间*/

	UI16   DAPRESS_CLSMIDEND_RAMPOFF;        /* 2012005C 途中关模结束压力斜率*/
	UI16   DAPFLOW_CLSMIDEND_RAMPOFF;        /* 2012005D 途中关模结束流量斜率*/	
	UI16   DAPRESS_OPNMIDEND_RAMPOFF;        /* 2012005E 途中开模结束压力斜率*/
	UI16   DAPFLOW_OPNMIDEND_RAMPOFF;        /* 2012005F 途中开模结束流量斜率*/

	UI16   DAPRESS_OPNPOSPIDEND;            /* 20120060 开模位置PID结束压力*/
	UI16   DAPFLOW_OPNPOSPIDEND;            /* 20120061 开模位置PID结束速度*/
	UI16   DAPRESS_CLSPOSPIDEND;            /* 20120062 关模位置PID结束压力*/
	UI16   DAPFLOW_CLSPOSPIDEND;            /* 20120063 关模位置PID结束速度*/

	UI16   DAFLOW_CLPOPN_VLVLIMIT;			/* 20120064 开模速度限制	默认:0 范围0~100%*/
	UI16   DAFLOW_VLV_CLS;					/* 20120065 关模模拟   默认20   范围0-99*/
	UI16   DAFLOW_LOWVLV_CLS;				/* 20120066 低压模拟   默认20   范围0-99*/
	UI16   DAFLOW_HIGVLV_CLS;				/* 20120067 高压模拟   默认20   范围0-99*/

	UI16   DABACKPRES_OPEN1RAMP;             /* 20120068 开模一段背压压力斜率       .01S		默认0.50		范围0.00-1.99*/
    UI16   DABACKPRES_OPEN2RAMP;            /* 20120069 开模二段背压压力斜率       .01s		默认0.20		范围0.00-1.99*/
    UI16   DABACKPRES_OPEN3RAMP;            /* 2012006A 开模三段背压压力斜率      .01s    	默认0.20		范围0.00-1.99*/
    UI16   DABACKPRES_OPEN4RAMP;            /* 2012006B 开模四段背压压力斜率      .01s    	默认0.20		范围0.00-1.99*/
    UI16   DABACKPRES_OPEN5RAMP;            /* 2012006C 开模五段背压压力斜率         .01s   默认0.20		范围0.00-1.99*/
    UI16   DABACKPRES_OPENOFFRAMP;          /* 2012006D 开模背压关闭压力斜率       .01s		默认0.20		范围0.00-1.99*/
	UI16   DABACKPRES_CLS1RAMP;             /* 2012006E 关模一段背压压力斜率       .01S		默认0.50		范围0.00-1.99*/
    UI16   DABACKPRES_CLS2RAMP;            	/* 2012006F 关模二段背压压力斜率       .01s		默认0.20		范围0.00-1.99*/
    UI16   DABACKPRES_CLS3RAMP;            	/* 20120070 关模三段背压压力斜率      .01s    	默认0.20		范围0.00-1.99*/
    UI16   DABACKPRES_CLSLOWPRESRAMP;      	/* 20120071 关模低压背压压力斜率      .01s    	默认0.20		范围0.00-1.99*/
    UI16   DABACKPRES_CLSHIGHPRESRAMP;      /* 20120072 关模高压背压压力斜率         .01s   默认0.20		范围0.00-1.99*/
    UI16   DABACKPRES_CLSOFFRAMP;           /* 20120073 关模背压关闭压力斜率       .01s		默认0.20		范围0.00-1.99*/

	
	UI16  FL_SPC_CLS;                		/* 20120074 特殊关模阀 0-不使用 1-关模四段 2-关模一二三段 3-关模二三段 4-关模五段 5-关模四五段 6-关模全段 7-开关模全段 默认 0-不使用*/	 
    UI16  FL_SPC_OPN;					   	/* 20120075 特殊开模阀 0-不使用 1-开模一段 2-开模一五段 3-开模五段 4-开模二三四段 5-开模二三四五段 6-开模全段 7-开关模全段 默认 0-不使用*/
	UI16  TM_OPN_BACK;          			/* 20120076 开模背压计时 .01s   	默认0.20  	范围0.00-1.99*/
	UI16   FL_ADMETER_TEST;					/* 20120077 开模电子尺检测   0   检测    1   不检测  默认0*/
	UI16   FL_FORCED_USE;					/* 20120078 高压二特殊使用 0  不使用   1  使用  默认0 */
	UI16   FL_PRESS_LIMIT;					/* 20120079 高压压力限制  0   限制   1   不限制   默认0*/
	UI16   ENERGY_FACTOR_OPN;				/* 2012007A 开模能耗系数 默认:10	范围:0~100*/
	UI16   FL_ENERGY_SAVE;					/* 2012007B 开模节能  0   不使用   1   使用   默认不使用*/
	UI16	TM_CLAMPSAFE;					/* 2012007C 安全阀检测无信号时间 默认0.0	范围0.0-200.0 单位s*/
    UI16	TM_MACHINESAFE;					/* 2012007D 安全阀检测有信号时间 默认0.0	范围0.0-200.0 单位s*/
	UI16   FL_CLAMP_FINE_TUNING;			/* 2012007E 开模微调功能 0 不使用	1 使用	默认不使用*/
    
    UI16   ADPOSI_ALLOWEJTUPLIMIT;			/* 2012007F 开模位置允许脱模上偏差  默认50.0  范围0.0-400.0mm */
	UI16   ADPOSI_ALLOWEJTDOWNLIMIT;		/* 20120080 开模位置允许脱模下偏差  默认50.0  范围0.0-400.0mm */
}CLAMP_MACHSET, *PCLAMP_MACHSET;

typedef struct EN_CLAMP_MOLDSET
{
    /*功能+警报参数*/
    /*+++++++++功能+++++++++*/
    UI16  FL_CLSFAST;                           /* 20130000 关模快速阀即差动合模   0:不使用  1：使用  默认0*/
    UI16  FL_CLSLOWPRES;		          		/* 20130001 关模低压阀   0:不使用  1：使用  默认0*/
    UI16  TM_HOLDCLSHIGH;                       /* 20130002 关模高压保持计时        默认0.00S		范围0.00-99.99  20200219 */
    UI16  FL_CLS3;                              /* 20130003 关模三段使用  0:使用  1：不使用  默认0*/

    UI16  FL_OPENDRAINFUNCTIO;                  /* 20130004 开模泄压选择 0:不使用  1: 使用  默认0*/
    UI16  TM_OPENDRAIN;                         /* 20130005 泄压计时       .00S       默认0.00S		范围0.00-9.99*/
    UI16  FL_OPENFAST;                          /* 20130006 开模快速阀   0:不使用  1：使用  默认0*/
    UI16  FL_OPENFAST2POS;                      /* 20130007 开模慢速阀   0:不使用  1: 一慢开 2：二慢开 3：一慢二慢都开  默认0*/
    UI16  ADPOSI_EJTTRAVEL;                     /* 20130008 开模联动位置     .0mm     默认100.0mm		范围0.0-400.0*/
    UI16  FL_OPN4;                              /* 20130009 开模四段使用  0:使用  1：不使用  默认0*/
    UI16  FL_MOLDREACHCHK;                     	/* 2013000A 开模停机模式 0:开模数 1:良品数  默认0*/
    UI16  CN_PRODUCTSETTING1;                  	/* 2013000B 目标数（开模目标数高位）			          */
    UI16  CN_PRODUCTSETTING2;                  	/* 2013000C 目标数（开模目标数低位）			          */
    UI16  CN_PACKAGE;                           /* 2013000D 本包开模数 2012-11-28   		 默认0	范围0-60000  */
    /*+++++++++警报参数+++++++++*/
    UI16  TM_CLSPROTECTDEFAULTMAX;              /* 2013000E 关模低压计时上限（警报画面参数）  0.00S   默认10.00S		范围0.00-99.99   20200219*/
    UI16  TM_CLSHIGHDEFAULTMAX;                 /* 2013000F 关模高压计时上限（警报画面参数）0.00S   默认10.00S		范围0.00-99.99   20200219*/
    UI16  TM_CLAMPCLSDEFAULTMAX;                /* 20130010 关模计时上限（警报画面参数）      0.00S    默认60.00S			范围1.00-99.99	  20200219*/
    UI16  TM_CLAMPOPNDEFAULTMAX;                /* 20130011 开模计时上限（警报画面参数）      0.00S    默认60.00S			范围1.00-99.99   20200219*/
    /*----------排气部件----------*/
    UI16  TMDLY_BAKEBEFOREEXHAUST;              /* 20130012 排气前延迟计时  	     默认0.50S		范围0.00-99.99   20200219 */
    UI16  TMDLY_BAKEAFTEREXHAUST;               /* 20130013 排气计时   		默认2.00S		范围0.00-99.99     20200219 */
    UI16  TM_EXHAUSTOPEN;                       /* 20130014 排气开模计时   	默认5.00S		范围0.00-99.99  20200219*/
    UI16  ADPOSI_EXHAUSTOPEN;                   /* 20130015 排气开模距离      .0MM    默认30.0MM	范围0.0-99.9*/
    UI16  FL_EXHAUSTMODE;                       /* 20130016 排气方式 0-不使用 1-射出後 2-保压一后 3-保压后 4-储料后 5-射出时间 6-射出位置 7-关模终 8-保压二后 9-多处排气 */
    UI16  FL_LUBBYPASS;                         /* 20130017 前二模不做润滑			0:不使用该功能		1：使用该功能			默认0  */
    UI16  POS_ROBOT;                            /* 20130018 机械手输出的开模位置设定*/
    UI16  DAPOSI_HIPRSCLAMPDOWN;                /* 20130019 高压锁模有效区下限               */
    UI16  DAPOSI_HIPRSCLAMPUP;                  /* 2013001A 高压锁模有效区上限               */
    UI16  TM_CLMPCLSHI;                         /* 2013001B 锁模高压计时0.00,*/

    /*        QC监控参数                 2014-07-06*/
    UI16  Qc_DeviationSet_ClsTm;                /* 2013001C 合模时间允许偏差     默认0.00*/
    UI16  Qc_TmSet_ClsTm;                       /* 2013001D 合模时间设定值   默认0.00*/
    UI16  Qc_Mode_ClsTm;                        /* 2013001E 合模时间监控:0:不使用  1:使用   默认不使用*/

    UI16  Qc_DeviationSet_ClsLow;               /* 2013001F 合模低压 时间允许偏差     默认0.00*/
    UI16  Qc_TmSet_ClsLow;                      /* 20130020 合模低压时间设定值   默认0.00*/
    UI16  Qc_Mode_ClsLow;                       /* 20130021 合模低压监控:0:不使用  1:使用   默认不使用*/

    UI16  Qc_DeviationSet_ClsHi;                /* 20130022 合模高压时间允许偏差     默认0.00*/
    UI16  Qc_TmSet_ClsHi;                       /* 20130023合模高压时间设定值   默认0.00*/
    UI16  Qc_Mode_ClsHi;                        /* 20130024 合模高压时间监控:0:不使用  1:使用   默认不使用*/

    UI16  Qc_DeviationSet_OpnEnd;               /* 20130025 开模终止位置允许偏差     默认0.0*/
    UI16  Qc_TmSet_OpnEnd;                      /* 20130026 开模终止位置设定值   默认0.0*/
    UI16  Qc_Mode_OpnEnd;                       /* 20130027 开模终止位置监控:0:不使用  1:使用   默认不使用*/

    UI16  Qc_DeviationSet_OpnTm;                /* 20130028 开模时间允许偏差     默认0.00*/
    UI16  Qc_TmSet_OpnTm;                       /* 20130029 开模时间设定值   默认0.00*/
    UI16  Qc_Mode_OpnTm;                        /* 2013002A 开模时间监控:0:不使用  1:使用   默认不使用*/

    UI16  Qc_DeviationSet_OpnSpd;               /* 2013002B开模速度允许偏差     默认0.0*/
    UI16  Qc_TmSet_OpnSpd;                      /* 2013002C 开模速度设定值   默认0.0*/
    UI16  Qc_Mode_OpnSpd;                       /* 2013002D开模速度监控:0:不使用  1:使用   默认不使用*/
    UI16  ADPOSI_EJTTRAVELCLOSE;                /* 2013002E 关模联动位置     .0mm     默认100.0mm		范围0.0-400.0*/
    UI16  FL_OPEN2;                             /* 2013002F 开模二段使用 fb   0：不使用 1：使用  默认0*/
    UI16  TM_RETARD;                            /* 20130030  开模减速时间默认0.00  范围0.00-99.99*/
    UI16  Tm_Cls_Ult;                           /* 20130031   威压开模模差动阀动作时间*/
    UI16  FL_ZEPOIT;                            /* 20130032 原点锁模功能使用  0  不使用   1   使用  默认0*/
    UI16  FL_ZEPOITPOS;                         /* 20130033 原点锁模位置 0.0-99.0mm    默认0.0*/
    UI16  FL_ZEPOITFLW;                         /* 20130034 原点锁模速度 默认20，范围0-50% */
    UI16  FL_MachType;                          /* 20130035 机型选择0 不使用   1   小型机   2   中型机   3   大型机   默认0 */
    UI16  TM_HLP_VLV_ONDelay;                   /* 20130036 差动辅助阀延迟开计时   范围0.00 -1.00    默认0.00 */
    UI16  TM_HLP_VLV_OFFDelay;                  /* 20130037 差动辅助阀延迟关计时   范围0.00 -0.50    默认0.20 */
    UI16  TM_OPN_BACK;                          /* 20130038 开模背压时间0.00-9.99  默认0.00 */
    UI16  BL_OPNCLS;                            /* 20130039 调模慢速比例   20%-100%    默认20%*/
    UI16  CLS_TWO_PUMP;                  		/* 2013003A 关摸多泵功能   0   不使用   1   使用    默认不使用 hmd*/   //20170505  LET
	UI16  FL_AUTO_PRU;                    		/* 0x2013003B 警报时不良品监控      0   不使用   1   使用    默认不使用   20170914  */
	UI16  FL_MAUL_PRU;                    		/* 0x2013003C 手动时不良品监控       0   不使用   1   使用   默认不使用  20170914    */
	UI16  FL_FISR_PRU;                    		/* 0x2013003D 自动初期不良品模数       范围0-99   默认0               20170914    */
	UI16  FL_Dely_PRU;                    		/* 0x2013003E 不良品延迟关      范围0.00-9.99  默认0.00                    20170914*/
	UI16  FL_USE_PRU;                     		/* 0x2013003F 不良品检知功能  0  不使用   1  使用  默认不使用  20170914*/
    UI16  SAFEVLVOFF;                           /* 0x20130040 安全阀延迟检测时间      范围0.000-3.000s，默认0.500*/   //20190403 CHJ 补齐

	UI16  PRESSTART_CLOSE_MID;					/* 0x20130041 关模手动启动压力斜率 默认100  范围0-100%*/
	UI16  FLOWSTART_CLOSE_MID;					/* 0x20130042 关模手动启动速度斜率 默认50  范围0-100%*/
	UI16  PRESSTOP_CLOSE_MID;					/* 0x20130043 关模手动停止压力斜率 默认100  范围0-100%*/
	UI16  FLOWSTOP_CLOSE_MID;					/* 0x20130044 关模手动停止速度斜率 默认50  范围0-100%*/
	UI16  PRESSTART_OPEN_MID;					/* 0x20130045 开模手动启动压力斜率 默认100  范围0-100%*/
	UI16  FLOWSTART_OPEN_MID;					/* 0x20130046 开模手动启动速度斜率 默认50  范围0-100%*/
	UI16  PRESSTOP_OPEN_MID;					/* 0x20130047 开模手动停止压力斜率 默认100  范围0-100%*/
	UI16  FLOWSTOP_OPEN_MID;					/* 0x20130048 开模手动停止速度斜率 默认50  范围0-100%*/

/*=====================================新油压参数==============================================*/
	UI16  FL_OPEN_SMOOTH;						/*0x20130049 开模平稳 0  不使用   1   开模平稳  默认0*/
	UI16  FL_CLSE_SMOOTH;						/*0x2013004A 关模平稳(整个模座) 0  不使用   1   关模平稳  默认0*/

	UI16  SLOPE_MAX_CLS_SPD;					/*0x2013004B 关模最大斜率(速度)	默认500	范围100-10000%/s*/
	UI16  SLOPE_MAX_OPN_SPD;					/*0x2013004C 开模最大斜率(速度)	默认500	范围100-10000%/s*/
	
	UI16  SLOPE_MAX_CLS_PRES;					/*0x2013004D 关模最大斜率(压力)	默认2000	范围100-60000bar/s*/
	UI16  SLOPE_MAX_OPN_PRES;					/*0x2013004E 开模最大斜率(压力)	默认2000	范围100-60000bar/s*/
	
	UI16  PUMP_K;								/* 2013004F 开模缓冲系数默认1.20    范围0.20-5.00    */
	UI16  PUMP_DEAD;							/* 20130050 开模死区时间默认0.100	范围0.000-2.000   单位ms*/
	UI16  PUMP_K_CLS;							/* 20130051 关模缓冲系数默认1.20    范围0.20-5.00    */
	UI16  PUMP_DEAD_CLS;						/* 20130052 关模死区时间默认0.000	范围0.000-2.000   单位ms*/
	
	UI16  OPN_SPD_STARTON;						/* 20130053 开模启动斜率(速度)	默认100 	范围0-100%*/
	UI16  OPN_SPD_HALFWAY;						/* 20130054 开模中间斜率(速度)	默认100 	范围0-100%*/
	UI16  OPN_SPD_STOPOFF;						/* 20130055 开模停止斜率(速度)	默认100 	范围0-100%*/

	UI16  CLS_SPD_STARTON;           			/* 20130056 关模开始斜率(速度)	默认100 	范围0-100%*/
	UI16  CLS_SPD_HALFWAY;           			/* 20130057 关模中间斜率(速度)	默认100 	范围0-100%*/
	UI16  CLS_SPD_STOPOFF;           			/* 20130058 关模停止斜率(速度)	默认100 	范围0-100%*/

	UI16  OPN_PRES_STARTON;						/* 20130059 开模启动斜率(压力)	默认100 	范围0-100%*/
	UI16  OPN_PRES_HALFWAY;						/* 2013005A 开模中间斜率(压力)	默认100 	范围0-100%*/
	UI16  OPN_PRES_STOPOFF;						/* 2013005B 开模停止斜率(压力)	默认100 	范围0-100%*/

	UI16  CLS_PRES_STARTON;           			/* 2013005C 关模开始斜率(压力)	默认100 	范围0-100%*/
	UI16  CLS_PRES_HALFWAY;           			/* 2013005D 关模中间斜率(压力)	默认100 	范围0-100%*/
	UI16  CLS_PRES_STOPOFF;           			/* 2013005E 关模停止斜率(压力)	默认100 	范围0-100%*/

	UI16  OPN_NUM;								/* 2013005F 开模段数	默认:3	范围:2~5段*/		
	UI16  FL_CLSOPEN_MODE;						/* 20130060 开关模模式    0   标准   1   平顺    2   比例阀 3 智能  4  智能-p*/
	UI16  AD_POSIDISS;							/* 20130061 外设减速距离   范围0-开模终止位置   默认20.0mm*/
	
	UI16  FL_MANULOPN1SPD;						/* 20130062 手动开模一段速度模式 0-不使用 1-使用 默认 0-不使用*/
	UI16  FL_CLSWAITCHG;						/* 20130063 关模前储料等待   0   不等待  1   等待  默认0*/
	UI16  CLPCLS_SPD_UTI;						/* 20130064 关模速度使用率	默认:0	范围:0~150%	*/
	UI16  CLPOPN_SPD_UTI;						/* 20130065 开模速度使用率	默认:0	范围:0~150%	*/
	
	UI16  TM_CLPOPN_ROBOT_PROTECT;				/* 20130066 允许机械手开模时间	默认:0.00	范围:0.00~5.00s	*/	
	UI16  FL_CLPPRE_NOTWAITAIRBLOW;				/* 20130067 关模前等待吹气	默认:0 等待	范围：0等待 1机械手时不等待*/
	
	UI16  CLPCLS_SPD_ULT;						/* 20130068 关模差动速度使用率	默认:60	范围:0~100%	*/
	UI16  FL_CLAMP_CORE_RAMP;					/* 20130069 途中中子开合模独立斜率	默认:0	范围:0:不使用	1:使用	*/

    UI16  FL_OPN_LOCATE;						/* 2013006A	开模平稳功能 默认:0 0:不使用 1:使用 */
	UI16  MODE_OPN_LOCATE;						/* 2013006B 开模平稳模式 默认:0 0:模式1，1:模式2 */
	UI16  OPN_POS_KP;							/* 2013006C 开模平稳系数 默认值:200	范围:0~6000 */
	UI16  OPN_OVER_SHOOT;						/* 2013006D 开模平稳补偿 默认值:0.0	范围:0~50.0 */

	UI16  FL_CLS_LOCATE;						/* 2013006E	关模平稳功能 默认:0 0:不使用 1:使用 */
	UI16  MODE_CLS_LOCATE;						/* 2013006F 关模平稳模式 默认:0 0:模式1，1:模式2 */
	UI16  CLS_POS_KP;							/* 20130070 关模平稳系数 默认值:200	范围:0~6000 */

	UI16  FL_EXTRAMP_LIMIT;						/* 20130071 开关模平稳外部斜率限制功能 默认:0 0:不使用 1:使用*/
	UI16  FL_LOCATE_DA;							/* 20130072 开关模平稳使用比例阀 默认:0 0:不使用 1:使用*/

    UI16  HIPRESS_DEVIATION;					/* 20130073 高压锁模压力偏差 默认0bar 范围：0-30bar*/
	UI16  HIPRESS_DEVIATION_FUNCTION;			/* 20130074 高压锁模压力偏差功能 0不使用；1使用 默认：不使用*/

    //上海光塑新增排气压塑动作
	UI16  TMDLY_BAKEBEFOREEXHAUST2;             /* 20130075 排气2前延迟 范围0.00-99.99s 默认0.50s */
	UI16  TMDLY_BAKEAFTEREXHAUST2;              /* 20130076 排气2计时 范围0.00-99.99s 默认2.00s */
	UI16  TM_EXHAUSTOPEN2;                      /* 20130077 排气2开模计时 范围0.00-99.99s 默认5.00s */
	UI16  ADPOSI_EXHAUSTOPEN2;                  /* 20130078 排气2开模距离 范围0.0-99.9mm 默认30.0mm */
	UI16  TMDLY_BAKEBEFOREEXHAUST3;             /* 20130079 排气3前延迟 范围0.00-99.99s 默认0.50s */
	UI16  TMDLY_BAKEAFTEREXHAUST3;              /* 2013007A 排气3计时 范围0.00-99.99s 默认2.00s */
	UI16  TM_EXHAUSTOPEN3;                      /* 2013007B 排气3开模计时 范围0.00-99.99s 默认5.00s */
	UI16  ADPOSI_EXHAUSTOPEN3;                  /* 2013007C 排气3开模距离 范围0.0-99.9mm 默认30.0mm */
	UI16  CN_EXHAUST;      					    /* 2013007D 排气次数选择 0-第一次 1-第二次 2-第三次 3-第四次 默认0 */
    UI16  ADPOSI_INJMOLDIN;					    /* 2013007E 排气时压塑起始位置 范围:关模低压终止位置-开模终止位置 默认3.0mm */
	UI16  FL_EXHAUSTAFTINJ;                     /* 2013007F 射出后排气 0-不使用 1-使用 默认 0-不使用*/
	UI16  FL_EXHAUSTAFTHOLD1;                   /* 20130080 保压一后排气 0-不使用 1-使用 默认 0-不使用*/
	UI16  FL_EXHAUSTAFTHOLD2;                   /* 20130081 保压二后排气 0-不使用 1-使用 默认 0-不使用*/
	UI16  FL_EXHAUSTAFTHOLD;                    /* 20130082 保压后排气 0-不使用 1-使用 默认 0-不使用*/
    UI16  TM_EXHAUSTOPENAFTINJ;                 /* 20130083 射出后排气开模计时 范围0.00-9.99s 默认5.00s */
	UI16  ADPOSI_EXHAUSTOPENAFTINJ;             /* 20130084 射出后排气开模距离 范围0.0-999.9mm 默认30.0mm */
	UI16  TM_EXHAUSTOPENAFTHOLD1;               /* 20130085 保压一后排气开模计时 范围0.00-9.99s 默认5.00s */
	UI16  ADPOSI_EXHAUSTOPENAFTHOLD1;           /* 20130086 保压一后排气开模距离 范围0.0-999.9mm 默认30.0mm */
	UI16  TM_EXHAUSTOPENAFTHOLD2;               /* 20130087 保压二后排气开模计时 范围0.00-9.99s 默认5.00s */
	UI16  ADPOSI_EXHAUSTOPENAFTHOLD2;           /* 20130088 保压二后排气开模距离 范围0.0-999.9mm 默认30.0mm */
	UI16  TM_EXHAUSTOPENAFTHOLD;                /* 20130089 保压后排气开模计时 范围0.00-9.99s 默认5.00s */
	UI16  ADPOSI_EXHAUSTOPENAFTHOLD;            /* 2013008A 保压后排气开模距离 范围0.0-999.9mm 默认30.0mm */
    UI16  TMDLY_BAKEBEFOREEXHAUST4;             /* 2013008B 排气4前延迟 范围0.00-99.99s 默认0.50s */
	UI16  TMDLY_BAKEAFTEREXHAUST4;              /* 2013008C 排气4计时 范围0.00-99.99s 默认2.00s */
	UI16  TM_EXHAUSTOPEN4;                      /* 2013008D 排气4开模计时 范围0.00-99.99s 默认5.00s */
	UI16  ADPOSI_EXHAUSTOPEN4;                  /* 2013008E 排气4开模距离 范围0.0-99.9mm 默认30.0mm */
}CLAMP_MOLDSET, *PCLAMP_MOLDSET;

typedef struct EN_CLAMP_ACTPARA
{
    /*PF+POS+TIME+HYDR(PORT;PUMPS)+SERVO(PORT;PUMPS;PID)*/
    /*+++++++++PF+POS+TIME+++++++++*/
    UI16  DAPRES_CLS1	;	       	          	/* 20140000 关模一段压力			默认50	范围0-开关模压力上限  */
    UI16  DAPRES_CLS2;                          /* 20140001 关模二段压力    		默认50	范围0-开关模压力上限           */
    UI16  DAPRES_CLS3;                          /* 20140002 关模三段压力    		默认50	范围0-开关模压力上限           */
    UI16  DAPRES_CLSPROTECT;                    /* 20140003 关模低压压力     		默认50	范围0-关模低压上限          */
    UI16  DAPRES_CLSHIGH;                       /* 20140004 关模高压压力     		默认50	范围0-关模高压上限          */
    UI16  DAFLOW_CLS1;                          /* 20140005 关模一段速度		    默认50	范围0-开关模速度上限          */
    UI16  DAFLOW_CLS2;                          /* 20140006 关模二段速度     		默认50	范围0-开关模速度上限          */
    UI16  DAFLOW_CLS3;                          /* 20140007 关模三段速度 			默认50	范围0-开关模速度上限              */
    UI16  DAFLOW_CLSPROTECT;                    /* 20140008 关模低压速度  		默认50	范围0-关模低压速度上限             */
    UI16  DAFLOW_CLSHIGH;                       /* 20140009 关模高压速度		    默认50	范围0-关模高压速度上限          */
    UI16  ADPOSI_CLS1END;                       /* 2014000A 关模一段终止位置			默认250.0*/
    UI16  ADPOSI_CLS2END;                       /* 2014000B 关模二段终止位置           	默认100.0*/
    UI16  ADPOSI_CLS3END;                       /* 2014000C 关模三段终止位置           	默认50.0*/
    UI16  ADPOSI_CLSPROTECTEND;                 /* 2014000D 关模低压终止位置           	默认30.0*/

    //开模
    UI16  DAPRES_OPENSLOW;                      /* 2014000E 开模一段压力     		默认70	范围0-开关模压力上限          */
    UI16  DAPRES_OPEN2;                         /* 2014000F 开模二段压力    		默认	120	范围0-开关模压力上限           */
    UI16  DAPRES_OPEN3;                         /* 20140010 开模三段压力     		默认	120	范围0-开关模压力上限          */
    UI16  DAPRES_OPEN4;                         /* 20140011 开模四段压力    		默认	120	范围0-开关模压力上限           */
    UI16  DAPRES_OPENEND;                       /* 20140012 开模终止压力    		默认	120	范围0-开关模压力上限           */
    UI16  DAFLOW_OPENSLOW;                      /* 20140013 开模一段速度    		默认	35	范围0-开关模速度上限           */
    UI16  DAFLOW_OPEN2;                         /* 20140014 开模二段速度    		默认	60	范围0-开关模速度上限           */
    UI16  DAFLOW_OPEN3;                         /* 20140015 开模三段速度		   	默认	60	范围0-开关模速度上限            */
    UI16  DAFLOW_OPEN4;                         /* 20140016 开模四段速度			默认	60	范围0-开关模速度上限            */
    UI16  DAFLOW_OPENEND;                       /* 20140017 开模五段速度		   	默认	60	范围0-开关模速度上限            */
    UI16  ADPOSI_OPENENDSLOW;                   /* 20140018 开模一段终止位置  			默认5.0  */
    UI16  ADPOSI_OPENEND2;                      /* 20140019 开模二段终止位置			默认200.0           */
    UI16  ADPOSI_OPENEND3;                      /* 2014001A 开模三段终止位置 			默认300.0          */
    UI16  ADPOSI_OPENEND4;                      /* 2014001B 开模四段终止位置			默认350.0           */
    UI16  ADPOSI_OPENENDEND;                    /* 2014001C 开模五段终止位置 			默认400.0          */
    UI16  DAPRES_OPENDRAIN;                     /* 2014001D 泄压压力   		默认30	范围0-系统压力上限        */
    UI16  DAFLOW_OPENDRAIN;                     /* 2014001E 泄压速度    	默认30	范围0-99  */
    /*合模新加2014-10-09*/
    UI16  ADPOS_CLSHIPRES2;                     /* 2014001F 关模高压1段切换2段位置			默认0*/
    UI16  ADPOS_CLSSTART;                       /* 20140020 合模启动距离 0.0 默认2.0*/
    UI16  DAPRES_CLSHIPRES2;                    /* 20140021 关模高压2段压力			默认60	范围0-开关模压力上限*/
    UI16  DAFLOW_CLSHIPRES2;                    /* 20140022 关模高压2段速度			默认40	范围0-开关模速度上限*/
    UI16  DAPRES_OPENSPD1;                      /* 20140023 快速开模起动压力*/
    UI16  DAFLOW_OPENSPD1;                      /* 20140024 快速开模起动速度*/
    UI16  ADPOS_OPENSPD1;                       /* 20140025 快速开模起动位置*/

    UI16  DAFLOW_OPENSPD2;                      /* 20140026 快速开模快速速度*/
    UI16  ADPOS_OPENSPD2;                       /* 20140027 快速开模快速位置*/

    UI16  DAFLOW_OPENSPD3;                      /* 20140028 快速开模结束速度*/
    UI16  ADPOS_OPENSPD3;                       /* 20140029 快速开模结束位置*/
    /*海晶异常开模新加   2015-3-13*/
    UI16    DAPRES_SCLAMPOPN;                   /* 2014002A 异常开模压力   15-3-13     */
    UI16    DAFLOW_SCLAMPOPN;                   /* 2014002B 异常开模速度   15-3-13     */
    UI16    DAPRES_SCLAMPCLS;                   /* 2014002C 异常关模压力   15-3-13     */
    UI16    DAFLOW_SCLAMPCLS;                   /* 2014002D 异常关模速度   15-3-13  */
    UI16    DAPRES_OPENSPD2;                    /* 2014002E 快速开模快速压力     20151010   yxh   华美达快速机新加*/
    UI16    DAPRES_OPENSPD3;                    /* 2014002F 快速开模二慢压力     20151010   yxh   华美达快速机新加*/
	UI16  DAFLOW_VLV;							/* 20140030 开模一段阀口范围0-99，默认30*/
	UI16  DAFLOW_VLVCLS;						/* 20140031 关模低压阀口范围0-99，默认30*/
	UI16  DAPRES_CLS_RAMPSLOW;                  /* 0x20140032 关模慢速启动压力斜率 范围0.00-1.99 默认0.50*/
    UI16  DAFLOW_CLS_RAMPSLOW;                  /* 0x20140033 关模慢速启动速度斜率 范围0.00-1.99 默认0.50*/
    UI16  DAPRES_CLS_SLOW;                      /* 0x20140034 关模慢速启动压力 范围0-系统压力上限            默认20*/
    UI16  DAFLOW_CLS_SLOW;                      /* 0x20140035 关模慢速启动速度 范围0-99            默认20*/
    UI16  ADPOSI_CLS_SLOW;                      /* 0x20140036 关模慢速启动距离 范围0.0-99.9 默认2.0*/
    UI16  TM_CLS_SLOW;                          /* 0x20140037 关模慢速启动时间 范围0.000-2.000 默认0.200*/

	UI16  PER_DAFLOW_CLAMPCLS;					/* 0x20140038 关模速度百分比  范围 0-100  默认 0 */
	UI16  PER_DAFLOW_CLAMPOPN;					/* 0x20140039 开模速度百分比  范围 0-100  默认 0 */
	UI16  PER_DAFLOW_LOWPRES;					/* 0x2014003A 关模低压速度百分比  范围 0-100  默认 0 */
	UI16  PER_DAFLOW_HIGHPRES;					/* 0x2014003B 关模高压速度百分比  范围 0-100  默认 0 */
	UI16  PER_DAFLOW_OPEN1;						/* 0x2014003C 开模一段速度百分比  范围 0-100  默认 0 */

	UI16  DABACKPRES_OPENSLOW;                  /* 0x2014003D 开模一段背压压力     		默认70	范围0-开关模压力上限          */
    UI16  DABACKPRES_OPEN2;                     /* 0x2014003E 开模二段背压压力    		默认	120	范围0-开关模压力上限           */
    UI16  DABACKPRES_OPEN3;                     /* 0x2014003F 开模三段背压压力     		默认	120	范围0-开关模压力上限          */
    UI16  DABACKPRES_OPEN4;                     /* 0x20140040 开模四段背压压力    		默认	120	范围0-开关模压力上限           */
    UI16  DABACKPRES_OPENEND;                   /* 0x20140041 开模终止背压压力    		默认	120	范围0-开关模压力上限           */
	UI16  DABACKPRES_CLS1;                  	/* 0x20140042 关模一段背压压力     		默认70	范围0-开关模压力上限          */
    UI16  DABACKPRES_CLS2;                     	/* 0x20140043 关模二段背压压力     		默认	120	范围0-开关模压力上限           */
    UI16  DABACKPRES_CLS3;                     	/* 0x20140044 关模三段背压压力      		默认	120	范围0-开关模压力上限          */
    UI16  DABACKPRES_CLSLOWPRES;                /* 0x20140045 关模低压背压压力     		默认	120	范围0-开关模压力上限           */
    UI16  DABACKPRES_CLSHIGHPRES;               /* 0x20140046 关模高压背压压力     		默认	120	范围0-开关模压力上限           */

	UI16  ADPOSI_OPENENDEND_MODE;				/* 0x20140047 开模五段模板位置			*/
	UI16  FL_MID_ROBOT;							/* 0x20140048 途中机械手强制   0   不使用   1   使用  默认0*/
	UI16  ADPOS_ROBOT;							/* 0x20140049 开模机械手位置   范围0-开模位置最大值  默认开模终止位置的默认值减去50mm*/

	UI16  ADPOSI_ULT_CLPOPEN;					/* 0x2014004A 开模差动结束位置 范围0-开模一段结束位置  默认2.0 mm*/
    UI16  DAFLOW_VLVCLSHI;						/* 0x2014004B 关模高压阀口范围 0-99，默认0*/

    UI16  ADPOS_FIRSSLW_OFF;					/* 0x2014004C 开模慢速阀切位置  默认2.5mm  范围:0-开模位置最大值 */
}CLAMP_ACTPARA, *PCLAMP_ACTPARA;

typedef struct EN_CLAMP_HYDR
{
    /*+++++++++HYDR(PORT;PUMPS)+++++++++*/
    UI16  HYDR_PORT_CLAMPCLS;					//20150000 关模动作的DA输出通道：位操作-->port1……port4
    UI16  HYDR_PUMPS_CLSSETUP;                  //20150001 关模装模的泵浦限制:位操作-->P1……P7
    UI16  HYDR_PUMPS_CLSSLOW;					//20150002 关模慢速的泵浦限制:位操作-->P1……P7
    UI16  HYDR_PUMPS_CLSFAST;					//20150003 关模快速的泵浦限制:位操作-->P1……P7
    UI16  HYDR_PUMPS_CLSPROTECT;				//20150004 关模低压的泵浦限制:位操作-->P1……P7
    UI16  HYDR_PUMPS_CLSHIG;					//20150005 关模高压的泵浦限制:位操作-->P1……P7
    //开模
    UI16  HYDR_PORT_CLAMPOPN;					//20150006 开模动作的DA输出通道：位操作-->port1……port4
    UI16  HYDR_PUMPS_OPNSETUP;                  //20150007 开模装模的泵浦限制:位操作-->P1……P7
    UI16  HYDR_PUMPS_OPNDRAIN;                  //20150008 开模泄压的泵浦限制:位操作-->P1……P7
    UI16  HYDR_PUMPS_OPNSLOW;					//20150009 开模一慢的泵浦限制:位操作-->P1……P7
    UI16  HYDR_PUMPS_OPNFAST;					//2015000A 开模快速的泵浦限制:位操作-->P1……P7
    UI16  HYDR_PUMPS_OPNEND;					//2015000B 开模二慢的泵浦限制:位操作-->P1……P7

    /*关模PID*/
    UI16  SERVO_KP_CLSSETUP;					//2015000C 关模调试PID_KP		范围0-2500	默认800
    UI16  SERVO_KI_CLSSETUP;					//2015000D 关模调试PID_KI		范围0-200		默认40
    UI16  SERVO_KD_CLSSETUP;					//2015000E 关模调试PID_KD
    UI16  SERVO_KP_CLS;                         //2015000F 关模PID_KP			范围0-2500	默认800
    UI16  SERVO_KI_CLS;                         //20150010 关模PID_KI			范围0-200		默认40
    UI16  SERVO_KD_CLS;                         //20150011 关模PID_KD

    /*压模*/
    UI16  SERVO_KP_HIPRS;						//20150012 压模PID_KP			范围0-2500	默认800
    UI16  SERVO_KI_HIPRS;						//20150013 压模PID_KI			范围0-200		默认40
    UI16  SERVO_KD_HIPRS;						//20150014 压模PID_KD

    /*开模PID*/
    UI16  SERVO_KP_OPNSETUP;					//20150015 开模调试PID_KP		范围0-2500	默认800
    UI16  SERVO_KI_OPNSETUP;					//20150016 开模调试PID_KI		范围0-200		默认40
    UI16  SERVO_KD_OPNSETUP;					//20150017 开模调试PID_KD
    UI16  SERVO_KP_OPN;                         //20150018 开模PID_KP			范围0-2500	默认800
    UI16  SERVO_KI_OPN;                         //20150019 开模PID_KI			范围0-200		默认40
    UI16  SERVO_KD_OPN;                         //2015001A 开模PID_KD

	/*开模位置PID*/
     UI16  POSN_KP_OPN;                         //2015001B 开模位置PID_KP
     UI16  POSN_KI_OPN;                         //2015001C 开模位置PID_KI
     UI16  POSN_KD_OPN;                         //2015001D 开模位置PID_KD

     /*关模位置PID*/
     UI16  POSN_KP_CLS;                         //2015001E 关模位置PID_KP
     UI16  POSN_KI_CLS;                         //2015001F 关模位置PID_KI
     UI16  POSN_KD_CLS;                         //20150020 关模位置PID_KD    
}CLAMP_HYDR, *PCLAMP_HYDR;

typedef struct EN_CLAMP_STATE
{
    UI16  FL_ERROR0;							//20160000 警报1
    UI16  FL_ERROR1;							//20160001 警报2
    UI16  CN_MOLDOPNNUM0;						//20160002 开模总数
    UI16  CN_MOLDOPNNUM1;						//20160003 开模总数
    UI16  CN_CURPACKMOLD;						//20160004 本包装良品模数
    UI16  CN_VALIDMOLD;                         //20160005 良品模数
    UI16  CN_CLAMPLUBRIC;						//20160006 自动润滑计数
    UI16  TM_CLAMPCLS;							//20160007 关模计时实际值            0.00
    UI16  TM_CLAMPCLS_MAX;                      //20160008 关模计时告警最大值        0.00
    UI16  TM_CLAMPCLSLO;						//20160009 关模低压计时实际值        0.00
    UI16   TM_CLAMPCLSLO_MAX;					//2016000A 关模低压计时告警最大值0.00
    UI16  TM_CLAMPCLSHI;						//2016000B 关模高压计时实际值        0.00
    UI16  TM_CLAMPCLSHI_MAX;					//2016000C 关模高压计时告警最大值0.00
    UI16  ADPOSI_OPNEND;						//2016000D 开模位置实际值
    UI16  TM_CLAMPOPN;							//2016000E 开模计时实际值            0.00
    UI16  TM_CLAMPOPN_MAX;                      //2016000F 开模计时告警最大值0.00
    UI16  CLAMP_SPEED;							//20160010 开模速度
    UI16  DAPRES_CLSHI_MAX;                     //20160011 锁模压力最大值
    UI16  DAPRES_CLSHIAVR;                      //20160012 锁模压力平均值
    UI16  TM_CLAMPMOV;							//20160013 移模计时，0.00s
    UI16  CLAMP_OPN_SPD;						//20160014 开模速度均值
    /*        QC监控参数                 2014-07-06*/
    UI16  Qc_Avg_Cls;							//20160015 合模时间平均值
    UI16  Qc_Deviation_Max_Cls;                 //20160016 合模时间最大偏差值
    UI16  Qc_Unq_Cnt_Cls;						//20160017 合模时间不合格数
    UI16  Qc_Unq_Rate_Cls;                      //20160018 合模时间不合格率
    UI16  Qc_Max_Cls;							//20160019 合模时间最大值
    UI16  Qc_Min_Cls;							//2016001A 合模时间最小值

    UI16  Qc_Avg_ClsLow;						//2016001B 合模低压时间平均值
    UI16  Qc_Deviation_Max_ClsLow;              //2016001C 合模低压时间最大偏差值
    UI16  Qc_Unq_Cnt_ClsLow;					//2016001D 合模低压时间不合格数
    UI16  Qc_Unq_Rate_ClsLow;					//2016001E 合模低压时间不合格率
    UI16  Qc_Max_ClsLow;						//2016001F 合模低压时间最大值
    UI16  Qc_Min_ClsLow;						//20160020 合模低压时间最小值

    UI16  Qc_Avg_ClsHi;                         //20160021 合模高压时间平均值
    UI16  Qc_Deviation_Max_ClsHi;               //20160022 合模高压时间最大偏差值
    UI16  Qc_Unq_Cnt_ClsHi;                     //20160023 合模高压时间不合格数
    UI16  Qc_Unq_Rate_ClsHi;					//20160024 合模高压时间不合格率
    UI16  Qc_Max_ClsHi;                         //20160025 合模高压时间最大值
    UI16  Qc_Min_ClsHi;                         //20160026 合模高压时间最小值

    UI16  Qc_Avg_OpnEnd;						//20160027 开模位置平均值
    UI16  Qc_Deviation_Max_OpnEnd;              //20160028 开模位置最大偏差值
    UI16  Qc_Unq_Cnt_OpnEnd;					//20160029 开模位置不合格数
    UI16  Qc_Unq_Rate_OpnEnd;					//2016002A 开模位置不合格率
    UI16  Qc_Max_OpnEnd;						//2016002B 开模位置最大值
    UI16  Qc_Min_OpnEnd;						//2016002C 开模位置最小值

    UI16  Qc_Avg_OpnTm;                         //2016002D 开模时间平均值
    UI16  Qc_Deviation_Max_OpnTm;               //2016002E 开模时间最大偏差值
    UI16  Qc_Unq_Cnt_OpnTm;                     //2016002F 开模时间不合格数
    UI16  Qc_Unq_Rate_OpnTm;					//20160030 开模时间不合格率
    UI16  Qc_Max_OpnTm;                         //20160031 开模时间最大值
    UI16  Qc_Min_OpnTm;                         //20160032 开模时间最小值

    UI16  Qc_Avg_OpnSpd;						//20160033 开模速度平均值
    UI16  Qc_Deviation_Max_OpnSpd;              //20160034 开模速度最大偏差值
    UI16  Qc_Unq_Cnt_OpnSpd;					//20160035 开模速度不合格数
    UI16  Qc_Unq_Rate_OpnSpd;					//20160036 开模速度不合格率
    UI16  Qc_Max_OpnSpd;						//20160037 开模速度最大值
    UI16  Qc_Min_OpnSpd;						//20160038 开模速度最小值
    UI16  TM_CLAMPCLS_RT;						//20160039 实时计算的关模时间，用于面板显示, 0.00s。
    UI16  TM_CLAMPOPN_RT;						//2016003A 实时计算的开模时间，用于面板显示, 0.00s。
    UI16  Step_Clampopn;						//2016003B   开模实时动作步   成品画面显示
    UI16  Step_Clampcls;						//2016003C   关模实时动作步   成品画面显示
	UI16  CN_PRU;                           	//2016003D 剩余模数     20170914
	UI16  TM_CLAMPOPN_START;                    //2016003E 开模计时起始时间		    单位:10ms       去除该功能，面板画线改为由0开启，否则所有相关的都得32位，数据太多 20200219
	UI16  TM_CLAMPCLS_START;                    //2016003F 关模计时起始时间		    单位:10ms     去除该功能，面板画线改为由0开启，否则所有相关的都得32位，数据太多 20200219

	UI16  AI_CLAMP_OPEN_RAMP;					//20160040 开模到位减速计算斜率		%
	UI16  AD_POSIDISSREAL;						//20160041 实际减速距离
	
    UI16  TM_CLAMPOPN1F;						//20160042 开模一段计时实际值        0.00

	UI16  CN_MOLDOPNNUM0_NCL;					//20160043 开模总数(不清零)(高)
    UI16  CN_MOLDOPNNUM1_NCL;					//20160044 开模总数(不清零)(低)

    UI16  TM_CLAMPCLS2;                         //20160045 关模实际时间2(关模完成时上传)
    UI16  TM_CLAMPOPN2;                         //20160046 开模实际时间2(开模完成时上传)
}CLAMP_STATE, *PCLAMP_STATE;
#pragma pack()

#ifdef DIC_ENUM_NEED

enum EN_EXHAST_ACT_PARA //排气动作的para值
{
    /* 排气方式为 0-不使用 或 7-关模终 */
    EXHAST_CNT_1 = 1,       // 1 第1次排气
    EXHAST_CNT_2 = 2,       // 2 第2次排气
    EXHAST_CNT_3 = 3,       // 3 第3次排气
    EXHAST_CNT_4 = 4,       // 4 第4次排气

    /* 排气方式为 9-多处排气 */
    EXHAST_INJEND = 11,     // 11 射出后排气
    EXHAST_HOLD1 = 12,      // 12 保压一后排气
    EXHAST_HOLD2 = 13,      // 13 保压二后排气
    EXHAST_HOLDEND = 14,    // 14 保压后排气
    
    EXHAST_ACT_PARA_END
};

/*ACT SN start from 1*/
enum EN_CLAMP_ACT_SN
{
    OS_CLSM	= 1,                // 1   ; '关模    '
    OS_PRSM,                    // 2   ; '压模    '
    OS_OPNM,                    // 3   ; '开模    '

    OS_ACLS,                    // 4   ; '调模时关模    '
    OS_AOPN,                    // 5   ; '调模时开模    '

    OS_AAPRS,                   // 6   ; '自动调模时压模    '
    OS_AAOPN,                   // 7   ; '自动调模时开模    '

    OS_EOPN,                    // 8   ; '排气时开模    '
    OS_EPRS,                    // 9   ; '排气时压模    '

    OS_EXHAST,                  // 10 排气任务
    OS_ERRPRS,                  // 11   ; '异常时关摸    '
    OS_ERROPN,                  // 12   ; '异常时开模   '

	OS_MODERF,					// 13	; 效能自学习

    CLAMP_ACT_SN_END
};

enum EN_CLAMP_ACTNAME
{
    /*关模*/
    CLAMPCLOSE1 = STAGE_1_STEP,			//0x20111000 关模一段
    CLAMPCLOSE2 = STAGE_2_STEP ,		//0x20111100 关模二段
    CLAMPCLOSE3 = STAGE_3_STEP,			//0x20111200 关模三段
    CLAMPCLOSE4 = STAGE_4_STEP,			//0x20111300 关模低压

    CLAMPCLSSETUP = STAGE_SPEC1_STEP,	//0x20118000 使用行程开关时起始没有压到输入点，使用装模设置
    CLAMPCLSSPC	  = STAGE_SPEC2_STEP,	//0x20118100 关模起始特殊段
    CLAMPOPNSPC   = STAGE_SPEC2_STEP,	//0x20138100 开模启动

    /*锁模*/
    CLAMPHIPRESS   = STAGE_1_STEP,		//0x20121000 高压锁模
    CLAMPPRESSHOLD = STAGE_2_STEP,		//0x20121100 高压保持
    CLAMPHIPRES2 = STAGE_3_STEP,		//0x20121200 高压二段
    CLAMPHIPRES3 = STAGE_4_STEP,		//0x20121300 高压锁模      关摸原点锁模功能使用时开放

    /*开模*/
    CLAMPOPNPREREL = INIT2_STEP,		//0x20130100 开模预泄压
    CLAMPOPNRELON = INIT3_STEP, 		//0x20130200 开模泄压开
    CLAMPOPNRELOFF = INIT4_STEP, 		//0x20130300 开模泄压关
    CLAMPOPENSLOW1 = STAGE_1_STEP,		//0x20131000 开模一慢
    CLAMPOPENFAST = STAGE_2_STEP,		//0x20131100 开模二段
    CLAMPOPENFST2 = STAGE_3_STEP,		//0x20131200 开模三段
    CLAMPOPENFST3 = STAGE_4_STEP,		//0x20131300 开模四段
    CLAMPOPENEND = STAGE_5_STEP,		//0x20131400 开模二慢
    /*自动调模*/
    CLAMPAUTOADJFIRST = STAGE_1_STEP,	//0x20161000 首次压膜
    CLAMPAUTOADJAGIN  = STAGE_2_STEP,	//0x20161100 再次压膜
    CLAMPOPNAUTOADJ   = STAGE_1_STEP	//0x20171000 调模开模
};

#define     ER0_CLSM      0x0001        // #20100  	关模未到定位
#define     ER0_HYSA      0x0002        // #20101  	液压安全异常  
#define     ER0_CLSP      0x0004        // #20102  	关模保护失败  
#define     ER0_OPNM      0x0008        // #20103  	开模未到定位  
#define     ER0_OPNPROT   0x0010        // #20104  	开模保护失败
#define     ER0_SHOT      0x0020        // #20105  	开模数已到
#define     ER0_PACK      0x0040        // #20106 	开模包装数己到
#define     ER0_OPNS      0x0080        // #20107	开模一慢未到定位
#define     ER0_MAOP      0x0100        // #20108   手动开模 
#define     ER0_CLOK      0x0200        // #20109   关模限制动作      
#define     ER0_AIRPRES   0x0400        // #2010A  	气压过低
#define     ER0_MESA      0x0800        // #2010B  	机械安全异常  
#define     ER0_HYSEN     0x1000        // #2010C   液压安全或电眼保护异常
#define     ER0_MDHEAT    0x2000        // #2010D  	模温偏差
#define     ER0_ALOK      0x4000        // #2010E	紧急回升  
#define     ER0_CLSLOCK   0x8000        // #2010F  	合模锁死保护失败
#define     ER0_UPGAERR   0x10000       // #20110   合模上顶盖异常               20150514   yxh
#define     ER0_ADMETER   0x20000       // #20111   开关模电子尺数据跳变    20170313   yxh
#define     ER0_SDRVALVE  0x40000       // #20112   安全阀检测异常
#define     ER0_ERRPRU    0x80000      	// #20113  产品不良  
#define     ER0_MCSDRVALVE  0x100000    // #20114 开合模安全阀检测异常  
#define     ER0_LGHTSAF   	0x200000    // #20115  模板检知异常
#define		ER1_OPNMPOSERR	0x400000	// #20116	开模位置异常，停止机械手
#define     ER0_HIPRESSERR  0x800000    // #20117  高压锁模压力偏差


#endif

#ifdef __cplusplus
}
#endif

#endif

