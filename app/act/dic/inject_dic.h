#ifndef _INJECT_DIC_H_
#define _INJECT_DIC_H_

#ifdef __cplusplus
extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"

#pragma pack(1)
#define INJECT_TEMP_CH_NUM	14
typedef struct EN_INJECT_PO
{
	UI16  O_INJ_SOL ;                 //0x10100000 射出      
	UI16  O_INJ_FST ;                 //0x10100001 射出快速
	UI16  O_IN_SETU ;                 //0x10100002 射出设定
	UI16  O_IN_HOLD ;                 //0x10100003 保压
	UI16  O_PASS_VLV;                 //0x10100004 旁路阀
	UI16  O_INJ_ACC;                  //0x10100005 射出增压
	UI16  O_INJ_SRV ;                 //0x10100006 射出备用

	UI16  O_CHR_SOL ;                 //0x10100007 储料 
	UI16  O_CHR_BAC ;                 //0x10100008 储料背压
	UI16  O_CHR_REL ;                 //0x10100009 储料泄压
	UI16  O_CHR_FST ;                 //0x1010000A 储料快速
	UI16  O_CHR_SET ;                 //0x1010000B 储料设定
	UI16  O_CHR_SRV ;                 //0x1010000C 储料备用

	UI16  O_SUC_SOL ;                 //0x1010000D 射退
	UI16  O_SUC_SET ;                 //0x1010000E 射退设定
	UI16  O_SUC_SRV ;                 //0x1010000F 射退备用

	UI16  O_CHR_RL2;                  //0x10100010 储料零背压阀
	UI16  O_INH_CHR;                  //0x10100011 射出保压储料(*使用链表关联)
	UI16  O_SYN_CHG;                  //0x10100012 储料联动阀
	UI16  O_INJ_SPC;                  //0x10100013 胶口针阀
	UI16  O_CHG_SUC;                  //0x10100014 储料射退
	UI16  O_INJ_CHGSUCK;              //0x10100015 射出储料射退(*使用链表关联)
	UI16  O_INJ_AG;                   //0x10100016 射出差动，     仅射出时打开，其他动作都不开
	UI16  O_HLD_REL;                  //0x10100017 保压解压
	UI16  O_INJ_OPCL;                 //0x10100018 射出开关模(*使用链表关联)        温州至上，在射出保压和开关模时打开
	UI16  O_INJ_OPEN;                 //0x10100019 射出开环
	UI16  O_CHG_RELOIL;               //0x1010001A  储料油马达泄油阀

	
	UI16  O_SPC_INJ;				  //0x1010001B  特殊射出阀
	UI16  O_SPC_CHG;				  //0x1010001C  特殊储料阀
	UI16  O_MOLD_SPC;                 //0x1010001D 模具封口
	
	UI16  O_INJ_ASSIST;               //0x1010001E 射出辅助阀
	UI16  O_PRE_MOLDENABLE;           //0x1010001F 预塑使能
}INJECT_PO, *PINJECT_PO;

typedef struct EN_INJECT_PI
{
	UI16  I_PUR_GUA ;                 //0x10110000 射出防护
	UI16  I_INJ_HLD ;                 //0x10110001 射出保压
	UI16  I_INJ_1ST ;                 //0x10110002 射出二段
	UI16  I_INJ_2ND;                  //0x10110003 射出三段

	UI16  I_CHR_END ;                 //0x10110004 储料终    
	UI16  I_SUC_END ;                 //0x10110005 射退终

	UI16  I_CHR_PRS ;                 //0x10110006 储压检知
	UI16  I_HOP_MAT ;                 //0x10110007 料斗检知
	UI16  I_RPM_CHK;                  //0x10110008 螺杆转数检知
	UI16  I_DRI_ALM;                  //0x10110009 驱动报警
}INJECT_PI, *PINJECT_PI;

typedef struct EN_INJECT_MACHSET
{
    /*斜率+液压动作前后延时+阀动作前后延时+其他机器参数*/
    /*所有斜率的范围都为0.00~1.99s                     */
    UI16  INJECT_TEMPCH[INJECT_TEMP_CH_NUM];  /* 0x10120000 ~ 0x1012000D   料桶温度通道编号*/
    UI16  INJ_METERCH;				          /* 0x1012000E 注射部件电子尺通道号 默认1*/

    UI16  DAPRES_INJRAMP;                  /* 0x1012000F 射出启动压力斜率          范围0.00-1.99s   默认0.50*/
    UI16  DAFLOW_INJRAMP;                  /* 0x10120010 射出启动速度斜率          范围0.00-1.99s   默认0.50*/
    UI16  DAPRES_INJRAMP2;                 /* 0x10120011 射出转换压力斜率          范围0.00-1.99s   默认0.50*/
    UI16  DAFLOW_INJRAMP2;                 /* 0x10120012 射出转换速度斜率          范围0.00-1.99s   默认0.50*/
    UI16  DAPRES_INJHOLDRAMP;              /* 0x10120013 保压启动压力斜率          范围0.00-1.99s   默认0.50*/
    UI16  DAFLOW_INJHOLDRAMP;              /* 0x10120014 保压启动速度斜率          范围0.00-1.99s   默认0.50*/
    UI16  DAPRES_INJOFFRAMP;               /* 0x10120015 射出关闭压力斜率          范围0.00-1.99s   默认0.00*/
    UI16  DAFLOW_INJOFFRAMP;               /* 0x10120016 射出关闭速度斜率          范围0.00-1.99s   默认0.00*/
    UI16  DAPRES_INJSETUP;                 /* 0x10120017 射出慢速调试压力          范围0-140    默认30*/
    UI16  DAFLOW_INJSETUP;                 /* 0x10120018 射出慢速调试速度          范围0-99     默认30*/

    UI16  DAPRES_CHGRAMP;                  /* 0x10120019 储料启动压力斜率          范围0.00-1.99s   默认0.50*/
    UI16  DAFLOW_CHGRAMP;                  /* 0x1012001A 储料启动速度斜率          范围0.00-1.99s   默认0.50*/
    UI16  DAPRES_CHGOFFRAMP;               /* 0x1012001B 储料关闭压力斜率          范围0.00-1.99s   默认0.00*/
    UI16  DAFLOW_CHGOFFRAMP;               /* 0x1012001C 储料关闭速度斜率          范围0.00-1.99s   默认0.00*/
    UI16  DAPRES_CHGSETUP;                 /* 0x1012001D 储料慢速调试压力          范围0-140    默认30*/
    UI16  DAFLOW_CHGSETUP;                 /* 0x1012001E 储料慢速调试速度          范围0-99     默认30*/

    UI16  DAPRES_SUCKBACKRAMP;             /* 0x1012001F 射退启动压力斜率          范围0.00-1.99s   默认0.50*/
    UI16  DAFLOW_SUCKBACKRAMP;             /* 0x10120020 射退启动速度斜率          范围0.00-1.99s   默认0.50*/
    UI16  DAPRES_SUCKBACKOFFRAMP;          /* 0x10120021 射退关闭压力斜率          范围0.00-1.99s   默认0.00*/
    UI16  DAFLOW_SUCKBACKOFFRAMP;          /* 0x10120022 射退关闭速度斜率          范围0.00-1.99s   默认0.00*/

    UI16  TMDLY_INJVALVEON;                /* 0x10120023 射出阀延迟开计时          范围0.00-5.00s    默认0.00*/
    UI16  TMDLY_VLV_OFFINJEND;             /* 0x10120024 射出阀延迟关计时          范围0.00-5.00s    默认0.00*/
    UI16  TMDLY_CHGUSEVALVE;               /* 0x10120025 储料阀延迟开计时          范围0.00-5.00s    默认0.00*/ //暂无作用
    UI16  TMDLY_VLVOFFCHGEND;              /* 0x10120026 储料阀延迟关计时          范围0.00-5.00s    默认0.00*/
    UI16  TMDLY_VLVOFFSUCKBACK;            /* 0x10120027 射退阀延迟开计时          范围0.00-5.00s    默认0.00*/ //暂无作用
    UI16  TMDLY_VLVOFFSUCKBACKEND;         /* 0x10120028 射退阀延迟关计时          范围0.00-5.00s    默认0.00*/

    UI16  TMDLY_INJEND;                    /* 0x10120029 射出动作结束延时          范围0.00-2.00s    默认0.00*/
    UI16  FL_CUSHIONFUNCTION;              /* 0x1012002A 射出监控功能			   范围0:不使用	1:使用 默认 1:使用         */

    UI16  INJ_HYDELAY;					   /* 0x1012002B 射出液压延迟 范围0.000-9.990 默认0.000*/
    UI16  HOLD_HYDELAY;					   /* 0x1012002C 保压液压延迟 默认0*/
    UI16  CHG_HYDELAY;					   /* 0x1012002D 储料液压延迟 范围0.000-9.990 默认0.000*/
    UI16  SUCKBACK_HYDELAY;				   /* 0x1012002E 射退液压延迟 范围0.000-9.990 默认0.000*/
    UI16  TMDLY_VLV_ONCHG;				   /* 0x1012002F 储料阀延迟开计时          范围0.00-5.00s    默认0.00*/
    UI16  TMDLY_VLV_ONSUCKBK;			   /* 0x10120030 射退阀延迟开计时          范围0.00-5.00s    默认0.00*/
    UI16  TMDLY_CHGEND;					   /* 0x10120031 储料动作结束延迟          范围0.00-9.99s    默认0.00*/
    UI16  TMDLY_SUCKBACKEND;			   /* 0x10120032 射退动作结束延迟          范围0.00-9.99s    默认0.00*/
    UI16  TMDLY_INJBEFORE;				   /* 0x10120033 射出任务前延迟           范围0.00-9.99s    默认0.02*/

    UI16  DAFLOW_VLVLIMIT;                 /* 0x10120034 合流储料速度限制 范围 0~99 默认 0*/
    UI16  FL_MANUALINJECTUSEPRES_FLOW;     /* 0x10120035 手动射出使用调模压力速度  0 : 不使用  1:使用   默认 0:不使用*/
    UI16  TM_EXINJTM;                      /* 0x10120036 排气时射出时间 范围0.00-6.00 默认0.00s   20200219*/
    UI16  POS_EXINJTM;                     /* 0x10120037 排气时射出距离 范围0.0-250.0 默认0.0mm*/
    UI16  Tm_HLD_REL;                      /* 0x10120038 (未使用)保压解压开时间  两位小数点，默认为0.00   20200219*/ 
    UI16  Inj_DvdAdj;					   /* 0x10120039 注射分体调试 0:不使用 1:使用    默认不使用ty*/
    UI16  TMDLY_SUCKBACKENDBEFORCHG;	   /* 0x1012003A 储前射退结束延迟          范围0.00-9.99s  ，默认0 hmd*/                //20170505 LET
    UI16  FL_INj_SPC;                      /* 0x1012003B 胶口针阀:0:不使用  1:使用 默认0*/                   //20170505 LET
    UI16  TM_INJSPC_OPNELR;                /* 0x1012003C 针阀提前开时间  默认0.00  范围0.00-5.00*/		   //20170505 LET
    UI16  TM_INJSPC_CLSDLY;                /* 0x1012003D 针阀延时关时间  默认0.00  范围0.00-5.00*/	 	   //20170505 LET
    UI16  TM_RELVLV;                       /* 0x1012003E 储料马达泄压阀开启时间  默认0.00  范围 0.00~99.99*/      //20190404.cyx

	//注射油缸参数
	UI16	DIA_CYLINDER_SUCK;				/* 0x1012003F 油缸直径 0.0-999.9 mm，默认60.0*/
	UI16	DIA_PISDIA_SUCK; 				/* 0x10120040 活塞杆直径 0.0-999.9 mm 默认30.0*/
	UI16	EFFICIENCY_FACTOR_SUCK;			/* 0x10120041 射退动能转换效率 默认:1.00	范围:0.10~5.00*/
	UI16	EFFICIENCY_FACTOR_INJ;			/* 0x10120042 注射动能转换效率 默认:1.00	范围:0.10~5.00*/
	UI16	DIA_LEFT_RIGHT;					/* 0x10120043 注射活塞杆 默认: 0:默认	范围:0:默认 1:反向*/
	UI16	NUM_CYLINDER_SUCK;				/* 0x10120044 注射油缸数   范围1-4   默认2*/

	UI16	LOWPRES_SUCK;					/* 0x10120045 射退底压	默认	5	范围0-20   单位%*/
	UI16	LOWFLOW_SUCK;					/* 0x10120046 射退底流	默认	2.0	范围0.0-20.0   单位%*/
	
	UI16	SMOOTH_LEVEL_SUCK;				/* 0x10120047 射退平滑	默认	0	范围0~100	*/

	UI16	LOWPRES_INJ;					/* 0x10120048 注射底压	默认	5	范围0-20   单位%*/
	UI16	LOWFLOW_INJ;					/* 0x10120049 注射底流	默认	2.0	范围0.0-20.0   单位%*/
	
	UI16	SMOOTH_LEVEL_INJ;				/* 0x1012004A 注射平滑	默认	0	范围0~100	*/
	UI16 	TM_EXINJTMH;					/* 0x1012004B 排气时射出时间高位 范围0.00-6.00 默认0.00s   20200219*/
	//20200311.dxl
	UI16 	POS_INJ_SPC;					/* 0x1012004C 胶口针阀开阀位置		范围0.0—开合模位置尺长，默认0.0*/
	UI16 	FL_INJ_SPC_FUNC;				/* 0x1012004D 胶口针阀功能选择	0:关模位置 1：射出同步 默认：0*/

	UI16	TM_CHARGE;						/* 0x1012004E 储料时间	默认	2.00s	范围0~600.00 单位s*/ 
	UI16	FL_CHARGEBYTM;					/* 0x1012004F 储料走时间 默认0	0:关闭 1:打开，该数据不保存，上电复位*/
	UI16	FL_CHR_RL2_SUK;					/* 0x10120050 射退开储料零背压	0:关闭	1:打开	默认:1打开*/
	UI16    FL_SPC_INJ;                	    /* 0x10120051 特殊射出阀 0-不使用 1-射出保压 2-射出 3-射出保压射退 4-射出保压储料 5-射出保压储料射退  默认 0-不使用*/	 
    UI16    FL_SPC_CHG;					    /* 0x10120052 特殊储料阀 0-不使用 1-储料 2-储料射退 3-射出保压储料 4-射出保压储料射退 默认 0-不使用*/
	UI16    FL_CHG_CHECKMM;					/* 0x10120053 储前电子尺检测   0   不检测  1   检测   默认0*/
	UI16	FL_CHG_NOLLEV;					/* 0x10120054 储料开座进阀   0   开   1   不开   默认0*/
	UI16	FL_INJ_PROV;					/* 0x10120055 注射射退模式   0   标准   1   双向比例阀   默认0*/
	UI16  	INJ_SPC_FUNC_CLS;				/* 0x10120056 针阀关闭方式 默认:0 范围 0:注射结束 1:储料前	威力仕非标程序地址为 0x10120055*/
	
	UI16	FL_INJCHAB_UPPRES;				/* 0x10120057 注射开环压力超调	默认0% 范围0~30%*/

    UI16    FL_ELECTRIC_CHG;                /* 0x10120058 电储料功能  0 不使用  1 使用  默认0 */
    UI16    FL_ELECTRIC_NOZZLEFWD;          /* 0x10120059 电储料同步座进  0 使用  1 不使用  默认0 */
    UI16  	MODE_CHOOSE;					/* 0x1012005A 免烘干机 0-不使用 1-使用 默认0 */
}INJECT_MACHSET, *PINJECT_MACHSET;

typedef struct EN_INJECT_MOLDSET
{
    UI16  FL_INCACC;                           /* 0x10130000 射出蓄能器 0-不使用 1-压力继电器              2-压力传感器 默认0*/
    UI16  DAPRES_INJACC;                	   /* 0x10130001 蓄能器压力        *///暂无作用
    UI16  FL_RELEASEHOLDPRES;                  /* 0x10130002 保压解压功能        0:不使用  1：使用  默认0*///暂无作用
    UI16  ADPOSI_INJPOSUPCUSHION;              /* 0x10130003 射出监控上限        范围0-999.9 默认99.9     */
    UI16  ADPOSI_INJPOSNEGCUSHION;             /* 0x10130004 射出监控下限        范围0-999.9 默认99.9 */
    UI16  TM_INJBEFORENOZADV;                  /* 0x10130005 座进前射出计时 范围0.00-99.99           默认0.00  20200219 */
    UI16  ADPOSI_INJBEFORENOZADV;              /* 0x10130006 座进前射出距离 范围0.0-250.0          默认0.0 */
    UI16  FL_INJECTCOREOFF;                    /* 0x10130007 射出时不开中子阀     默认1 范围0~1*///暂无作用
    UI16  TM_INJMOLDINTIME;                    /* 0x10130008 压塑前射出一段计时 范围0.00-99.99            默认10.00    20200219*/
    UI16  FL_TURNTOHOLD;                       /* 0x10130009 保压转换方式 0:时间/位置 1:压力 2:速度 3:时间 4:位置 5:分段时间默认0*/
    UI16  DAPRES_TURNTOHOLD;                   /* 0x1013000A 保压转换压力       范围0-140        默认0*/
    UI16  ADPOSI_TURNTOHOLD;                   /* 0x1013000B 保压转换位置       范围0.0-250.0    默认1.0MM*/
    UI16  TM_TURNTOHOLD;                       /* 0x1013000C 保压转换时间       范围0.00-600.00   默认6.00    20200219*/
    UI16  SPEED_TURNTOHOLD;                    /* 0x1013000D 保压转换速度       范围0-99         默认0*/
    UI16  FL_CHGBACK;                          /* 0x1013000E 储料背压阀选择         0:不使用  1：使用，  默认0*/
    UI16  FL_CHRGFAST;                         /* 0x1013000F 储料快速功能         0:不使用  1：使用，  默认0*/
    UI16  ADPOSI_CHGFLOWMOLD;                  /* 0x10130010 再次储料距离       范围0.0-150.0    默认0.0*/
    UI16  TM_CHGFLOWMOLDING;                   /* 0x10130011 再次储料计时       范围0.00-600.00   默认0.00  20200219 */
    UI16  FL_SUCKBACKMODE;                     /* 0x10130012 射退模式 0:储料後 1:冷却後 默认0*///暂无作用
    UI16  TM_SUCKBACKBEFORECHG;                /* 0x10130013 储料前射退时间    范围0.00-99.99      默认0.00    20200219*/
    UI16  TM_AUTOPURGE;                        /* 0x10130014 自动清料计时 范围0.00-99.99         默认5.00s    20200219*/
    UI16  CN_AUTOPURGE;                        /* 0x10130015 自动清料计数 范围0-99             默认3      */
    UI16  FL_AUTOPURGE;						   /* 0x10130016 座进前射出      0-不使用，  1-  时间，2  -位置  ，默认0*/
    UI16  SEC_INJ;                             /* 0x10130017	射出使用段数 默认4 范围3~10*/ //范围1-6 默认4
    UI16  SEC_CHG;                             /* 0x10130018  储料使用段数 默认2 范围2~10*/ //范围1-3 默认2
    UI16  SEC_HOLD; 	                       /* 0x10130019  保压使用段数 默认2 范围2~3*/            //范围2-5 默认2

    UI16  FL_INJFAST;                           /* 0x1013001A 射出快速阀         0-不使用  ，1-使用，  默认0*/
    UI16  FL_PASSVALUE;							/* 0x1013001B 射出旁路阀         0-不使用，1-射出开，2-保压开，3-射保开 默认0*/
    UI16  FL_CHGMODE;							/* 0x1013001C 储料模式: 缺省0*/

    /*+++++++++警报参数+++++++++*/
    UI16  TM_CHGDEFAULTMAX_L;                   /* 0x1013001D  储料计时警报上限内设值低16位   范围0.00 - 3000.00  默认60.00    20200219*/
    UI16  TM_SUCKBACKDEFAULTMAX;                /* 0x1013001E  射退计时警报上限内设值     范围 0.00 - 600.00  默认60.00        20200219*/

    UI16  FL_CHARGERELEASE;                     /* 0x1013001F 储料泄压功能  默认 0*/
    UI16  FL_SUCKBACKBEFORECHG;                 /* 0x10130020 储料前射退模式 0=不使用 1=距离 2=时间  默认0.0*/
    UI16  POS_SUCKBACKBEFORECHG;                /* 0x10130021 储料前射退距离         默认0.0*/
    UI16  TM_CHARGE_UP;                         /* 0x10130022 储料计时上偏差, 0.01S  范围0.00-99.99 默认5.00   20200219*/
    /*        QC监控参数                 2014-07-06*/
    UI16  Qc_DeviationSet_InjTm;                /* 0x10130023 射出时间允许偏差     默认0.00   范围0.00-9.99      20200219*/
    UI16  Qc_TmSet_InjTm;                       /* 0x10130024 射出时间设定值   默认0.00     范围0.00-9.99           20200219*/
    UI16  Qc_Mode_InjTm;                        /* 0x10130025 射出时间监控:0:不使用  1:使用   默认不使用*/

    UI16  Qc_DeviationSet_InjSpd;               /* 0x10130026 射出速度允许偏差     默认0.0*/
    UI16  Qc_TmSet_InjSpd;                      /* 0x10130027 射出速度设定值   默认0.0*/
    UI16  Qc_Mode_InjSpd;                       /* 0x10130028 射出速度监控:0:不使用  1:使用   默认不使用*/

    UI16  Qc_DeviationSet_InjEnd;               /* 0x10130029 保压转化位置允许偏差     默认0.0*/
    UI16  Qc_TmSet_InjEnd;                      /* 0x1013002A 保压转化位置设定值   默认0.0*/
    UI16  Qc_Mode_InjEnd;                       /* 0x1013002B 保压转化位置监控:0:不使用  1:使用   默认不使用*/

    UI16  Qc_DeviationSet_InjPrs;               /* 0x1013002C 射出最大压力允许偏差     默认0.0*/
    UI16  Qc_TmSet_InjPrs;                      /* 0x1013002D 射出最大压力设定值   默认0.0*/
    UI16  Qc_Mode_InjPrs;                       /* 0x1013002E 射出最大压力监控:0:不使用  1:使用   默认不使用*/

    UI16  Qc_DeviationSet_HoldEnd;              /* 0x1013002F 残料位置允许偏差     默认0.0*/
    UI16  Qc_TmSet_HoldEnd;                     /* 0x10130030 残料位置设定值   默认0.0*/
    UI16  Qc_Mode_HoldEnd;                      /* 0x10130031 残料位置监控:0:不使用  1:使用   默认不使用*/

    UI16  Qc_DeviationSet_ChgTm;                /* 0x10130032 储料时间允许偏差     默认0.00*/
    UI16  Qc_TmSet_ChgTm;                       /* 0x10130033 储料时间设定值   默认0.00*/
    UI16  Qc_Mode_ChgTm;                        /* 0x10130034 储料时间监控:0:不使用  1:使用   默认不使用*/

    UI16  Qc_DeviationSet_ChgEnd;               /* 0x10130035 储料终止位置允许偏差     默认0.0*/
    UI16  Qc_TmSet_ChgEnd;                      /* 0x10130036 储料终止位置设定值   默认0.0*/
    UI16  Qc_Mode_ChgEnd;                       /* 0x10130037 储料终止位置监控:0:不使用  1:使用   默认不使用*/

    UI16  Qc_DeviationSet_SuckBwd;              /* 0x10130038 射退终止位置允许偏差     默认0.0*/
    UI16  Qc_TmSet_SuckBwd;                     /* 0x10130039 射退终止位置设定值   默认0.0*/
    UI16  Qc_Mode_SuckBwd;                      /* 0x1013003A 射退终止位置监控:0:不使用  1:使用   默认不使用*/

    UI16  Qc_DeviationSet_SuckTm;               /* 0x1013003B 射退时间允许偏差     默认0.00*/
    UI16  Qc_TmSet_SuckTm;                      /* 0x1013003C 射退时间设定值   默认0.00*/
    UI16  Qc_Mode_SuckTm;                       /* 0x1013003D 射退时间监控:0:不使用  1:使用   默认不使用*/
    
    UI16  Inj_Open;                             /* 0x1013003E 射出开环使用  0  不使用  1-使用   默认不使用*/
    UI16  Hod_Open;                             /* 0x1013003F 保压开环使用  0  不使用  1  使用   默认不使用*/
    UI16  Chg_Open;                             /* 0x10130040 储料开环使用  0  不使用  1  使用   默认不使用*/
    UI16  Suck_BefChg;                          /* 0x10130041 储前射退保护距离*/
    UI16  FL_InjectWarmOn;                      /* 0x10130042 未开启加热不可射出储料功能*/
    UI16  FL_CHGINJCNT;                         /* 0x10130043 多次储料射出次数   默认0  *///20180509 CHJ
    UI16  POS_CHGINJCNT;                        /* 0x10130044 多次储料位置   默认0.00  范围0.00-99.99 */  //20190403 CHJ 补齐	

	UI16  SLOPE_MAX_SUCK_SPD;					/* 0x10130045 射退最大斜率(速度) 默认2000 范围100-10000%/s*/
	UI16  SLOPE_MAX_SUCK_PRES;					/* 0x10130046 射退最大斜率(压力) 默认2000	范围100-60000bar/s*/

	UI16  FL_SUCK_SOOMTH;						/* 0x10130047 射退平稳0 不使用  1  使用   默认不使用*/
	
	UI16  PUMP_DEAD_SUCK;						/* 0x10130048 射退死区时间	默认0.000	范围0.000-2.000   单位ms*/
	UI16  PUMP_K_SUCK;							/* 0x10130049 射退缓冲系数	默认1.20    范围0.20-5.00    */

	UI16  SUCK_PRES_STARTON;		   			/* 0x1013004A 射退启动斜率(压力)	默认100 	范围0-100%*/
	UI16  SUCK_PRES_HALFWAY;		   			/* 0x1013004B 射退中间斜率(压力)	默认100 	范围0-100%*/
	UI16  SUCK_PRES_STOPOFF;		   			/* 0x1013004C 射退停止斜率(压力)	默认100 	范围0-100%*/

	UI16  SUCK_SPD_STARTON; 		   			/* 0x1013004D 射退启动斜率(速度)	默认100 	范围0-100%*/
	UI16  SUCK_SPD_HALFWAY; 		   			/* 0x1013004E 射退中间斜率(速度)	默认100 	范围0-100%*/
	UI16  SUCK_SPD_STOPOFF; 		   			/* 0x1013004F 射退停止斜率(速度)	默认100 	范围0-100%*/

	
	UI16  SLOPE_MAX_INJ_SPD;					/* 0x10130050 注射最大斜率(速度) 默认2000 范围100-10000%/s*/
	UI16  SLOPE_MAX_INJ_PRES;					/* 0x10130051 注射最大斜率(压力) 默认2000	范围100-60000bar/s*/

	UI16  FL_INJ_SOOMTH;						/* 0x10130052 注射平稳0 不使用  1  使用   默认不使用*/
	
	UI16  PUMP_DEAD_INJ;						/* 0x10130053 注射死区时间	默认0.000	范围0.000-2.000   单位ms*/
	UI16  PUMP_K_INJ;							/* 0x10130054 注射缓冲系数	默认1.20    范围0.20-5.00    */

	UI16  INJ_PRES_STARTON;		   				/* 0x10130055 注射启动斜率(压力)	默认100 	范围0-100%*/
	UI16  INJ_PRES_HALFWAY;		   				/* 0x10130056 注射中间斜率(压力)	默认100 	范围0-100%*/
	UI16  INJ_PRES_STOPOFF;		   				/* 0x10130057 注射停止斜率(压力)	默认100 	范围0-100%*/

	UI16  INJ_SPD_STARTON; 		   				/* 0x10130058 注射启动斜率(速度)	默认100 	范围0-100%*/
	UI16  INJ_SPD_HALFWAY; 		   				/* 0x10130059 注射中间斜率(速度)	默认100 	范围0-100%*/
	UI16  INJ_SPD_STOPOFF; 		   				/* 0x1013005A 注射停止斜率(速度)	默认100 	范围0-100%*/

	UI16  TM_CHGDEFAULTMAX_H;                   /* 0x1013005B  储料计时警报上限内设值高16位   范围0.00 - 3000.00  默认60.00   20200219 */
	UI16  HYDY_WAY_SET;                         /* 0x1013005C  油路切换选择0=单油路，1=双油路。*/
	UI16  FL_CHGMOVE;							/* 0x1013005D  储料移动检查   0    不使用   1   使用   默认不使用*/
	UI16  TM_CHGMOVE;							/* 0x1013005E  储料移动时间  范围0.00-99.99   默认2.00*/
	UI16  POS_CHGMOVE;							/* 0x1013005F  储料移动距离  范围0.0-50.0   默认5.0*/
	UI16  SPEED_LIMIT;							/* 0x10130060  手动储料速度限制 范围0-100，默认20*/
	UI16  FL_AUTOPURGE_SPC;						/* 0x10130061  清料方式	0:喷嘴清料	1:针阀清料	默认0	    20200729*/
	UI16  POS_AUTOPURGE;                        /* 0x10130062  自动清料储料位置       	范围0-射出电子尺位置  默认50     20200729*/
	UI16  TM_AUTOINJ;							/* 0x10130063  自动清料注射时间	范围0.00-60.00 		   默认 6.00s      20200729*/

	UI16  INJPRES_SETMODE;						/* 0x10130064	注射压力设定模式	默认:多段	范围:0:多段	1:单段*/
	UI16  HOLDSPD_SETMODE;						/* 0x10130065	保压速度设定模式	默认:多段	范围:0:多段	1:单段*/
	
	UI16  INJ_SPD_UTI;							/* 0x10130066	注射速度使用率	默认:0	范围:0~150%*/
	UI16  FL_INJSPCMAUAL;						/* 0x10130067  手动射出输出胶口针阀 0-使用 1-不使用 默认 0-使用*/
	
	UI16  FL_INJECTWARMPRE;                     /* 0x10130068  不检测料筒温度 1-使用 默认 0-不使用 不做数据保存*/
    UI16  ADPOSI_INJMOLDIN_INJ1;                /* 0x10130069  压塑前射出一段终止位置 范围20.0-250.0 默认50.0*/
}INJECT_MOLDSET, *PINJECT_MOLDSET;

typedef struct EN_INJECT_ACTPARA
{
    /*PF+POS+TIME+HYDR(PORT;PUMPS)+SERVO(PORT;PUMPS;PID)*/
    /*+++++++++PF+POS+TIME+++++++++*/
    UI16  DAPRES_INJ1;                         /* 0x10140000 射出一段压力 范围0-140 默认50           */
    UI16  DAPRES_INJ2;                         /* 0x10140001 射出二段压力 范围0-140 默认60           */
    UI16  DAPRES_INJ3;                         /* 0x10140002 射出三段压力 范围0-140 默认70           */
    UI16  DAPRES_INJ4;                         /* 0x10140003 射出四段压力 范围0-140 默认80           */
    UI16  DAPRES_INJ5;                         /* 0x10140004 射出五段压力 范围0-140 默认85           */
    UI16  DAPRES_INJ6;                         /* 0x10140005 射出六段压力 范围0-140 默认90           */
    UI16  DAPRES_INJ7;                         /* 0x10140006 射出七段压力 默认0              */
    UI16  DAPRES_INJ8;                         /* 0x10140007 射出八段压力 默认0              */
    UI16  DAPRES_INJ9;                         /* 0x10140008 射出九段压力 默认0              */
    UI16  DAPRES_INJ10;                        /* 0x10140009 射出十段压力 默认0              */
    UI16  DAPRES_INJHOLD1;                     /* 0x1014000A 保压一段压力 范围0-140 默认40           */
    UI16  DAPRES_INJHOLD2;                     /* 0x1014000B 保压二段压力 范围0-140 默认40           */
    UI16  DAPRES_INJHOLD3;                     /* 0x1014000C 保压三段压力 范围0-140 默认40           */
    UI16  DAPRES_INJHOLD4;                     /* 0x1014000D 保压四段压力 范围0-140 默认40           */
    UI16  DAPRES_INJHOLD5;                     /* 0x1014000E 保压五段压力 范围0-140 默认40           */
    UI16  DAPRES_INJHOLD6;                     /* 0x1014000F 保压六段压力 默认40              */
    UI16  DAPRES_INJHOLD7;                     /* 0x10140010 保压七段压力 默认40              */
    UI16  DAPRES_INJHOLD8;                     /* 0x10140011 保压八段压力 默认40              */
    UI16  DAPRES_INJHOLD9;                     /* 0x10140012 保压九段压力 默认40              */
    UI16  DAPRES_INJHOLD10;                    /* 0x10140013 保压十段压力 默认40              */
    UI16  DAFLOW_INJ1;                         /* 0x10140014 射出一段速度 范围0-99 默认50*/
    UI16  DAFLOW_INJ2;                         /* 0x10140015 射出二段速度 范围0-99 默认50*/
    UI16  DAFLOW_INJ3;                         /* 0x10140016 射出三段速度 范围0-99 默认50*/
    UI16  DAFLOW_INJ4;                         /* 0x10140017 射出四段速度 范围0-99 默认50      */
    UI16  DAFLOW_INJ5;                         /* 0x10140018 射出五段速度 范围0-99 默认50      */
    UI16  DAFLOW_INJ6;                         /* 0x10140019 射出六段速度 范围0-99 默认50      */
    UI16  DAFLOW_INJ7;                         /* 0x1014001A 射出七段速度 默认50              */
    UI16  DAFLOW_INJ8;                         /* 0x1014001B 射出八段速度 默认50              */
    UI16  DAFLOW_INJ9;                         /* 0x1014001C 射出九段速度 默认50              */
    UI16  DAFLOW_INJ10;                        /* 0x1014001D 射出十段速度 默认50              */
    UI16  DAFLOW_INJHOLD1;                     /* 0x1014001E 保压一段速度 范围0-99 默认30*/
    UI16  DAFLOW_INJHOLD2;                     /* 0x1014001F 保压二段速度 范围0-99 默认30*/
    UI16  DAFLOW_INJHOLD3;                     /* 0x10140020 保压三段速度 范围0-99 默认30*/
    UI16  DAFLOW_INJHOLD4;                     /* 0x10140021 保压四段速度 范围0-99 默认30*/
    UI16  DAFLOW_INJHOLD5;                     /* 0x10140022 保压五段速度 范围0-99 默认30*/
    UI16  DAFLOW_INJHOLD6;                     /* 0x10140023 保压六段速度 默认30              */
    UI16  DAFLOW_INJHOLD7;                     /* 0x10140024 保压七段速度 默认30              */
    UI16  DAFLOW_INJHOLD8;                     /* 0x10140025 保压八段速度 默认30              */
    UI16  DAFLOW_INJHOLD9;                     /* 0x10140026 保压九段速度 默认30              */
    UI16  DAFLOW_INJHOLD10;                    /* 0x10140027 保压十段速度 默认30              */
    UI16  TM_INJ1;                             /* 0x10140028 射出一段计时低16位 范围0.00-3600.00 默认6.00  20200219  */
    UI16  TM_INJ2;                             /* 0x10140029 射出二段计时低16位 范围0.00-3600.00 默认0.00  20200219  */
    UI16  TM_INJ3;                             /* 0x1014002A 射出三段计时低16位 范围0.00-3600.00 默认0.00  20200219   */
    UI16  TM_INJ4;                             /* 0x1014002B 射出四段计时低16位 范围0.00-3600.00 默认0.00  20200219   */
    UI16  TM_INJ5;                             /* 0x1014002C 射出五段计时低16位 范围0.00-3600.00 默认0.00  20200219   */
    UI16  TM_INJ6;                             /* 0x1014002D 射出六段计时低16位 范围0.00-3600.00 默认0.00  20200219    */
    UI16  TM_INJ7;                             /* 0x1014002E 射出七段计时低16位 范围0.00-3600.00 默认0.00  20200219    */
    UI16  TM_INJ8;                             /* 0x1014002F 射出八段计时低16位 范围0.00-3600.00 默认0.00  20200219    */
    UI16  TM_INJ9;                             /* 0x10140030 射出九段计时低16位 范围0.00-3600.00 默认0.00  20200219    */
    UI16  TM_INJ10;                            /* 0x10140031 射出十段计时低16位 范围0.00-3600.00 默认0.00  20200219    */
    UI16  TM_INJHOLD1;                         /* 0x10140032 保压一段计时低16位 范围0.00-3600.00 默认0.00  20200219    */
    UI16  TM_INJHOLD2;                         /* 0x10140033 保压二段计时低16位 范围0.00-3600.00 默认0.00  20200219     */
    UI16  TM_INJHOLD3;                         /* 0x10140034 保压三段计时低16位 范围0.00-3600.00 默认0.00  20200219     */
    UI16  TM_INJHOLD4;                         /* 0x10140035 保压四段计时低16位 范围0.00-3600.00 默认0.00  20200219      */
    UI16  TM_INJHOLD5;                         /* 0x10140036 保压五段计时低16位 范围0.00-3600.00 默认0.00  20200219      */
    UI16  TM_INJHOLD6;                         /* 0x10140037 保压六段计时低16位 范围0.00-3600.00 默认0.00  20200219      */
    UI16  TM_INJHOLD7;                         /* 0x10140038 保压七段计时低16位 范围0.00-3600.00 默认0.00  20200219       */
    UI16  TM_INJHOLD8;                         /* 0x10140039 保压八段计时低16位 范围0.00-3600.00 默认0.00  20200219       */
    UI16  TM_INJHOLD9;                         /* 0x1014003A 保压九段计时低16位 范围0.00-3600.00 默认0.00  20200219       */
    UI16  TM_INJHOLD10;                        /* 0x1014003B 保压十段计时低16位 范围0.00-3600.00 默认0.00  20200219        */
    UI16  ADPOSI_INJ1END;                      /* 0x1014003C 射出一段终止位置 范围20.0-250.0 默认50.0*/
    UI16  ADPOSI_INJ2END;                      /* 0x1014003D 射出二段终止位置 范围15.0- 50.0 默认20.0*/
    UI16  ADPOSI_INJ3END;                      /* 0x1014003E 射出三段终止位置 范围10.0- 20.0 默认15.0        */
    UI16  ADPOSI_INJ4END;                      /* 0x1014003F 射出四段终止位置 范围 5.0- 15.0 默认10.0*/
    UI16  ADPOSI_INJ5END;                      /* 0x10140040 射出五段终止位置 范围 0.0- 10.0 默认 5.0*/
    UI16  ADPOSI_INJ6END;                      /* 0x10140041 射出六段终止位置 范围 0.0-            5.0 默认 0.0  */
    UI16  ADPOSI_INJ7END;                      /* 0x10140042 射出七段终止位置 默认0          */
    UI16  ADPOSI_INJ8END;                      /* 0x10140043 射出八段终止位置 默认0          */
    UI16  ADPOSI_INJ9END;                      /* 0x10140044 射出九段终止位置 默认0          */
    UI16  ADPOSI_INJ10END;                     /* 0x10140045 射出十段终止位置 默认0          */

    UI16  DAPRES_CHG1;                         /* 0x10140046 储料一段压力 范围0-140 默认80 */
    UI16  DAPRES_CHG2;                         /* 0x10140047 储料二段压力 范围0-140 默认80 */
    UI16  DAPRES_CHG3;                         /* 0x10140048 储料三段压力 范围0-140 默认80           */
    UI16  DAPRES_CHG4;                         /* 0x10140049 储料四段压力 默认0              */
    UI16  DAPRES_CHG5;                         /* 0x1014004A 储料五段压力 默认0              */
    UI16  DAPRES_CHGBACK1;                     /* 0x1014004B 储料一段背压压力 范围0-70 默认30             */
    UI16  DAPRES_CHGBACK2;                     /* 0x1014004C 储料二段背压压力 范围0-70 默认30             */
    UI16  DAPRES_CHGBACK3;                     /* 0x1014004D 储料三段背压压力 范围0-70 默认30             */
    UI16  DAPRES_CHGBACK4;                     /* 0x1014004E 储料四段背压压力 默认0           */
    UI16  DAPRES_CHGBACK5;                     /* 0x1014004F 储料五段背压压力 默认0           */
    UI16  DAFLOW_CHG1;                         /* 0x10140050 储料一段速度       范围0-99 默认60     */
    UI16  DAFLOW_CHG2;                         /* 0x10140051 储料二段速度       范围0-99 默认60 */
    UI16  DAFLOW_CHG3;                         /* 0x10140052 储料三段速度       范围0-99 默认60     */
    UI16  DAFLOW_CHG4;                         /* 0x10140053 储料四段速度 默认0              */
    UI16  DAFLOW_CHG5;                         /* 0x10140054 储料五段速度 默认0              */
    UI16  ADPOSI_CHG1END;                      /* 0x10140055 储料一段终止位置 范围            0.0-120.0 默认100.0*/
    UI16  ADPOSI_CHG2END;                      /* 0x10140056 储料二段终止位置 范围100.0-150.0 默认120.0        */
    UI16  ADPOSI_CHG3END;                      /* 0x10140057 储料三段终止位置 范围120.0-250.0 默认150.0*/
    UI16  ADPOSI_CHG4END;                      /* 0x10140058 储料四段终止位置 默认0           */
    UI16  ADPOSI_CHG5END;                      /* 0x10140059 储料五段终止位置 默认0           */

    UI16  DAPRES_SUCKBACK;                     /* 0x1014005A 射退压力     范围0-140 默认50     */
    UI16  DAFLOW_SUCKBACK;                     /* 0x1014005B 射退速度 范围0-99        默认50   */
    UI16  ADPOSI_SUCKBACKEND;                  /* 0x1014005C 射退终止位置 范围0.0-250.0 默认150.0      */
    UI16  ADPOSI_SUCKBACKDISTANCE;             /* 0x1014005D 射退距离                   */
    UI16  TM_SUCKBACK;                         /* 0x1014005E 射退计时    范围0.00-99.99   默认0.00        20200219       */
    UI16  DAPRES_SUCKBACKBFCHG;                /* 0x1014005F 储料前射退压力        chg by rch 2012-12-19   */
    UI16  DAFLOW_SUCKBACKBFCHG;                /* 0x10140060 储料前射退速度        chg by rch 2012-12-19   */

    UI16  DAPRES_INJRAMPSLOW;                  /* 0x10140061 射出慢速启动压力斜率 范围0.00-1.99 默认0.50*/
    UI16  DAFLOW_INJRAMPSLOW;                  /* 0x10140062 射出慢速启动速度斜率 范围0.00-1.99 默认0.50*/
    UI16  DAPRES_INJSLOW;                      /* 0x10140063 射出慢速启动压力 范围0-140 默认50*/
    UI16  DAFLOW_INJSLOW;                      /* 0x10140064 射出慢速启动速度 范围0-99            默认50*/
    UI16  ADPOSI_INJSLOW;                      /* 0x10140065 射出慢速启动距离 范围0.0-99.9 默认2.0*/
    UI16  TM_INJSLOW;                          /* 0x10140066 射出慢速启动时间 范围0.000-2.000 默认0.200*/
    UI16  DAPRES_SUCKRAMPSLOW;                 /* 0x10140067 射退慢速启动压力斜率 范围0.00-1.99 默认0.50*/
    UI16  DAFLOW_SUCKRAMPSLOW;                 /* 0x10140068 射退慢速启动速度斜率 范围0.00-1.99 默认0.50*/
    UI16  DAPRES_SUCKSLOW;                     /* 0x10140069 射退慢速启动压力 范围0-99            默认50*/
    UI16  DAFLOW_SUCKSLOW;                     /* 0x1014006A 射退慢速启动速度 范围0-99            默认50*/
    UI16  ADPOSI_SUCKSLOW;                     /* 0x1014006B 射退慢速启动距离 范围0.0-99.9 默认2.0*/
    UI16  TM_SUCKSLOW;                         /* 0x1014006C 射退慢速启动时间 范围0.000-2.000 默认0.200*/
    /*通用新加20161008 LY*/
    UI16  DAPRES_CHGPURGE;                     /* 0x1014006D 自动清料储料压力 默认80        */
    UI16  DAFLOW_CHGPURGE;                     /* 0x1014006E 自动清料储料速度 默认60            */
    UI16  DAPRES_CHGBACKPUGRE;                 /* 0x1014006F 自动清料储料背压压力 默认30          */
    UI16  DAPRES_INJPURGE;                     /* 0x10140070 自动清料射出压力         默认50   */
    UI16  DAFLOW_INJPURGE;                     /* 0x10140071 自动清料射出速度 默认50           */

	UI16  TM_INJ1_H;                           /* 0x10140072 射出一段计时高16位 范围0.00-3600.00 默认6.00  20200219 */
    UI16  TM_INJ2_H;                           /* 0x10140073 射出二段计时高16位 范围0.00-3600.00 默认0.00  20200219 */
    UI16  TM_INJ3_H;                           /* 0x10140074 射出三段计时高16位 范围0.00-3600.00 默认0.00  20200219 */
    UI16  TM_INJ4_H;                           /* 0x10140075 射出四段计时高16位 范围0.00-3600.00 默认0.00  20200219 */
    UI16  TM_INJ5_H;                           /* 0x10140076 射出五段计时高16位 范围0.00-3600.00 默认0.00  20200219 */
    UI16  TM_INJ6_H;                           /* 0x10140077 射出六段计时高16位 范围0.00-3600.00 默认0.00  20200219 */
    UI16  TM_INJ7_H;                           /* 0x10140078 射出七段计时高16位 范围0.00-3600.00 默认0.00  20200219 */
    UI16  TM_INJ8_H;                           /* 0x10140079 射出八段计时高16位 范围0.00-3600.00 默认0.00   20200219 */
    UI16  TM_INJ9_H;                           /* 0x1014007A 射出九段计时高16位 范围0.00-3600.00 默认0.00   20200219 */
    UI16  TM_INJ10_H;                          /* 0x1014007B 射出十段计时高16位 范围0.00-3600.00 默认0.00   20200219 */
    UI16  TM_INJHOLD1_H;                       /* 0x1014007C 保压一段计时高16位 范围0.00-3600.00 默认2.00   20200219 */
    UI16  TM_INJHOLD2_H;                       /* 0x1014007D 保压二段计时高16位 范围0.00-3600.00 默认2.00   20200219 */
    UI16  TM_INJHOLD3_H;                       /* 0x1014007E 保压三段计时高16位 范围0.00-3600.00 默认0.00    20200219 */
    UI16  TM_INJHOLD4_H;                       /* 0x1014007F 保压四段计时高16位 范围0.00-3600.00 默认0.00    20200219  */
    UI16  TM_INJHOLD5_H;                       /* 0x10140080 保压五段计时高16位 范围0.00-3600.00 默认0.00    20200219 */
    UI16  TM_INJHOLD6_H;                       /* 0x10140081 保压六段计时高16位 范围0.00-3600.00 默认0.00    20200219 */
    UI16  TM_INJHOLD7_H;                       /* 0x10140082 保压七段计时高16位 范围0.00-3600.00 默认0.00    20200219 */
    UI16  TM_INJHOLD8_H;                       /* 0x10140083 保压八段计时高16位 范围0.00-3600.00 默认0.00    20200219 */
    UI16  TM_INJHOLD9_H;                       /* 0x10140084 保压九段计时高16位 范围0.00-3600.00 默认0.00    20200219 */
    UI16  TM_INJHOLD10_H;                      /* 0x10140085 保压十段计时高16位  范围0.00-3600.00 默认0.00   20200219   */ 

	UI16  PER_DAFLOW_INJECT;				   /* 0x10140086 射出速度百分比  范围 0-100  默认 0 */
	UI16  PER_DAFLOW_HOLD;				   	   /* 0x10140087 保压速度百分比  范围 0-100  默认 0 */
	UI16  PER_DAFLOW_CHARGE;				   /* 0x10140088 储料速度百分比  范围 0-100  默认 0 */
	UI16  PER_DAFLOW_SUCKBACK;				   /* 0x10140089 射退速度百分比  范围 0-100  默认 0 */
	
	UI16  ASSIST_OPEN_SPEED;				   /* 0x1014008A 射出辅助阀开启速度  范围 0-100  默认 50 */
    UI16  DAPRES_CHGBACKHLP;				   /* 0x1014008B 储料辅助压力 范围0-100bar 默认50bar */
	UI16  DAFLOW_CHGBACKHLP;				   /* 0x1014008C 储料辅助流量 范围0-100% 默认50% */
}INJECT_ACTPARA, *PINJECT_ACTPARA;

typedef struct EN_INJECT_HYDR
{
    /*+++++++++HYDR(PORT;PUMPS)+++++++++*/
    UI16  HYDR_PORT_INJCT;						//0x10150000 注射的DA输出通道：位操作-->port1……port4
    UI16  HYDR_PUMPS_INJCT;						//0x10150001 注射动作的泵浦限制:位操作-->P1……P7
    UI16  HYDR_PUMPS_INJSETUP;					//0x10150002 注射装模的泵浦限制:位操作-->P1……P7

    UI16  SERVO_KP_INJCT;						//0x10150003 注射动作的PID_KP 范围0-2500 默认800
    UI16  SERVO_KI_INJCT;						//0x10150004 注射动作的PID_KI 范围0-200         默认40
    UI16  SERVO_KD_INJCT;						//0x10150005 注射动作的PID_KD 范围0-200         默认10

    UI16  HYDR_PORT_HOLD;						//0x10150006 保压的DA输出通道：位操作-->port1……port4
    UI16  HYDR_PUMPS_HOLD;						//0x10150007 保压动作的泵浦限制:位操作-->P1……P7
    UI16  SERVO_KP_HOLD;						//0x10150008 注射动作的PID_KP 范围0-2500 默认800
    UI16  SERVO_KI_HOLD;						//0x10150009 注射动作的PID_KI 范围0-200         默认40
    UI16  SERVO_KD_HOLD;						//0x1015000A 注射动作的PID_KD

    UI16  HYDR_PORT_CHARGE;						//0x1015000B 储料的DA输出通道：位操作-->port1……port4
    UI16  HYDR_PUMPS_CHARGE;					//0x1015000C 储料动作的泵浦限制:位操作-->P1……P7
    UI16  SERVO_KP_CHARGE;						//0x1015000D 储料动作的PID_KP 范围0-2500 默认800
    UI16  SERVO_KI_CHARGE;						//0x1015000E 储料动作的PID_KI 范围0-200         默认40
    UI16  SERVO_KD_CHARGE;						//0x1015000F 储料动作的PID_KD

    UI16  HYDR_PORT_SUCKBACK;					//0x10150010 射退的DA输出通道：位操作-->port1……port4
    UI16  HYDR_PUMPS_SUCKBACK;					//0x10150011 射退动作的泵浦限制:位操作-->P1……P7
    UI16  SERVO_KP_SUCKBACK;					//0x10150012 射退动作的PID_KP 范围0-2500 默认800
    UI16  SERVO_KI_SUCKBACK;					//0x10150013 射退动作的PID_KI 范围0-200         默认40
    UI16  SERVO_KD_SUCKBACK;					//0x10150014 射退动作的PID_KD

    UI16  HYDR_PORT_CHRGBACK;					//0x10150015 储料背压的DA输出通道：位操作-->port1……port4 范围0-4 默认2

}INJECT_HYDR, *PINJECT_HYDR;

typedef struct EN_INJECT_STATE
{
    UI16  FL_ERROR0;							// 0x10160000 警报1
    UI16  FL_ERROR1;							// 0x10160001 警报2
    UI16  TM_INJ_L;								// 0x10160002 射出计时实际值低16位, 小数点0.00 ，/* 20200219 */
    UI16  ADPOSI_INJSTART;                      // 0x10160003 射出起点位置
    UI16  ADPOSI_INJHOLDEND;					// 0x10160004 射出终点位置实际值
    UI16  ADPOSI_INJCUSHION;					// 0x10160005 射出转保压位置
    UI16  ADPOSI_INJCUSHION_MAX;				// 0x10160006 射出监控位置告警最大值
    UI16  DAPRES_TOHOLD;						// 0x10160007 保压转换压力
    UI16  TM_TOHOLD;							// 0x10160008 保压转换时间实际值小数点0.00 ，/* 20200219 */
    UI16  ADPOSI_TOHOLD;						// 0x10160009 保压转换位置实际值
    UI16  TM_CHARGE;							// 0x1016000A 储料计时实际值  0.00S
    UI16  TM_CHARGE_MAX;						// 0x1016000B 储料计时告警最大时 0.00S
    UI16   CN_CHRGSPD;							// 0x1016000C 储料转速
    UI16  ADPOSI_CHGEND;						// 0x1016000D 储料终点位置实际值   04-02-29
    UI16  TM_SUCKBACK1;                         // 0x1016000E 射退计时实际值	0.00S
    UI16  TM_SUCKBACK1_MAX;                     // 0x1016000F 射退计时告警最大值 0.00S
    UI16  SPEED_TOHOLD;                         // 0x10160010 转保压速度
    UI16  ADPOSI_SUCKBACK;                      // 0x10160011 射退终止位置实际值
    UI16  INJECT_SPEED;                         // 0x10160012 射出速度，mm/ms
    UI16  INJECT_AVERAGESPEED;                  // 0x10160013 射出平均速度，mm/ms
    UI16  INJECT_MAXPRESS;                      // 0x10160014 射出最大压力
    UI16  HOLD_MAXPRESS;						// 0x10160015 保压最大压力
    UI16  HOLD_MOVPOSITION;                     // 0x10160016 保压移动距离
    UI16  INJECT_STEP;							// 0x10160017 射出当前动作步
    /*        QC监控参数                 2014-07-06*/
    UI16  Qc_Avg_InjTm_L;                       /* 0x10160018 射出时间平均值低16位  0.00  20200219*/
    UI16  Qc_Deviation_Max_InjTm_L;             /* 0x10160019 射出时间最大偏差值低16位  0.00  20200219*/
    UI16  Qc_Unq_Cnt_InjTm;                     /* 0x1016001A 射出时间不合格数*/
    UI16  Qc_Unq_Rate_InjTm;                    /* 0x1016001B 射出时间不合格率*/
    UI16  Qc_Max_InjTm_L;                       /* 0x1016001C 射出时间最大值低16位   0.00   20200219*/
    UI16  Qc_Min_InjTm_L;                       /* 0x1016001D 射出时间最小值低16位   0.00   20200219*/

    UI16  Qc_Avg_InjSpd;                        /* 0x1016001E 射出速度平均值*/
    UI16  Qc_Deviation_Max_InjSpd;              /* 0x1016001F 射出速度最大偏差值*/
    UI16  Qc_Unq_Cnt_InjSpd;                    /* 0x10160020 射出速度不合格数*/
    UI16  Qc_Unq_Rate_InjSpd;                   /* 0x10160021 射出速度不合格率*/
    UI16  Qc_Max_InjSpd;                        /* 0x10160022 射出速度最大值*/
    UI16  Qc_Min_InjSpd;	                    /* 0x10160023 射出速度最小值*/

    UI16  Qc_Avg_InjEnd;                        /* 0x10160024 保压转换位置平均值*/
    UI16  Qc_Deviation_Max_InjEnd;              /* 0x10160025 保压转换位置最大偏差值*/
    UI16  Qc_Unq_Cnt_InjEnd;                    /* 0x10160026 保压转换位置不合格数*/
    UI16  Qc_Unq_Rate_InjEnd;                   /* 0x10160027 保压转换位置不合格率*/
    UI16  Qc_Max_InjEnd;                        /* 0x10160028 保压转换位置最大值*/
    UI16  Qc_Min_InjEnd;	                    /* 0x10160029 保压转换位置最小值*/

    UI16  Qc_Avg_InjPrs;                        /* 0x1016002A 射出压力平均值*/
    UI16  Qc_Deviation_Max_InjPrs;              /* 0x1016002B 射出压力最大偏差值*/
    UI16  Qc_Unq_Cnt_InjPrs;                    /* 0x1016002C 射出压力不合格数*/
    UI16  Qc_Unq_Rate_InjPrs;                   /* 0x1016002D 射出压力不合格率*/
    UI16  Qc_Max_InjPrs;                        /* 0x1016002E 射出压力最大值*/
    UI16  Qc_Min_InjPrs;	                    /* 0x1016002F 射出压力最小值*/

    UI16  Qc_Avg_HoldEnd;                       /* 0x10160030 残料位置平均值*/
    UI16  Qc_Deviation_Max_HoldEnd;             /* 0x10160031 残料位置最大偏差值*/
    UI16  Qc_Unq_Cnt_HoldEnd;                   /* 0x10160032 残料位置不合格数*/
    UI16  Qc_Unq_Rate_HoldEnd;                  /* 0x10160033 残料位置不合格率*/
    UI16  Qc_Max_HoldEnd;                       /* 0x10160034 残料位置最大值*/
    UI16  Qc_Min_HoldEnd;	                    /* 0x10160035 残料位置最小值*/

    UI16  Qc_Avg_ChgTm;                         /* 0x10160036 储料时间平均值*/
    UI16  Qc_Deviation_Max_ChgTm;               /* 0x10160037 储料时间最大偏差值*/
    UI16  Qc_Unq_Cnt_ChgTm;                     /* 0x10160038 储料时间不合格数*/
    UI16  Qc_Unq_Rate_ChgTm;                    /* 0x10160039 储料时间不合格率*/
    UI16  Qc_Max_ChgTm;                         /* 0x1016003A 储料时间最大值*/
    UI16  Qc_Min_ChgTm;                         /* 0x1016003B 储料时间最小值*/

    UI16  Qc_Avg_ChgEnd;                        /* 0x1016003C 储料终止位置平均值*/
    UI16  Qc_Deviation_Max_ChgEnd;              /* 0x1016003D 储料终止位置最大偏差值*/
    UI16  Qc_Unq_Cnt_ChgEnd;                    /* 0x1016003E 储料终止位置不合格数*/
    UI16  Qc_Unq_Rate_ChgEnd;                   /* 0x1016003F 储料终止位置不合格率*/
    UI16  Qc_Max_ChgEnd;                        /* 0x10160040 储料终止位置最大值*/
    UI16  Qc_Min_ChgEnd;                        /* 0x10160041 储料终止位置最小值*/

    UI16  Qc_Avg_SuckBwd;                       /* 0x10160042 射退终止位置平均值*/
    UI16  Qc_Deviation_Max_SuckBwd;             /* 0x10160043 射退终止位置最大偏差值*/
    UI16  Qc_Unq_Cnt_SuckBwd;                   /* 0x10160044 射退终止位置不合格数*/
    UI16  Qc_Unq_Rate_SuckBwd;                  /* 0x10160045 射退终止位置不合格率*/
    UI16  Qc_Max_SuckBwd;                       /* 0x10160046 射退终止位置最大值*/
    UI16  Qc_Min_SuckBwd;                       /* 0x10160047 射退终止位置最小值*/

    UI16  Qc_Avg_SuckTm;                        /* 0x10160048 射退时间平均值*/
    UI16  Qc_Deviation_Max_SuckTm;              /* 0x10160049 射退时间最大偏差值*/
    UI16  Qc_Unq_Cnt_SuckTm;                    /* 0x1016004A 射退时间不合格数*/
    UI16  Qc_Unq_Rate_SuckTm;                   /* 0x1016004B 射退时间不合格率*/
    UI16  Qc_Max_SuckTm;                        /* 0x1016004C 射退时间最大值*/
    UI16  Qc_Min_SuckTm;                        /* 0x1016004D 射退时间最小值*/
    UI16  TM_INJ_RT_L;                            /* 0x1016004E 实时计算的射出时间，用于面板显示, 0.00s。*/
    UI16  TM_CHARGE_RT;                         /* 0x1016004F 实时计算的储料时间，用于面板显示, 0.00s。*/

    UI16  TM_INJE1;                             /* 0x10160050 射出一段计时    用于显示射出一段结束后，射出时间           */
    UI16  TM_INJE2;                             /* 0x10160051 射出二段计时               */
    UI16  TM_INJE3;                             /* 0x10160052 射出三段计时               */
    UI16  TM_INJE4;                             /* 0x10160053 射出四段计时               */
    UI16  TM_INJE5;                             /* 0x10160054 射出五段计时               */
    UI16  TM_INJE6;                             /* 0x10160055 射出六段计时               */
    UI16  TM_INJE7;                             /* 0x10160056 射出七段计时               */
    UI16  TM_INJE8;                             /* 0x10160057 射出八段计时               */
    UI16  TM_INJE9;                             /* 0x10160058 射出九段计时               */
    UI16  TM_INJE10;                            /* 0x10160059 射出十段计时               */
    UI16  Step_Inj;                             /* 0x1016005A 射出实时动作步               成品画面显示      */
    UI16  Step_Hold;                            /* 0x1016005B 保压实时动作步               成品画面显示      */
    UI16  Step_InjProc;                         /* 0x1016005C 射出调度实时动作步             成品画面显示      */
    UI16  Step_Chg;                             /* 0x1016005D 储料实时动作步               成品画面显示      */
    UI16  Step_Suck;                            /* 0x1016005E 射退实时动作步               成品画面显示      */
    UI16  Step_InjBforeNoz;                     /* 0x1016005F 座进前射出实时动作步            成品画面显示      */
    UI16  Step_ChgBforeInj;                     /* 0x10160060 射出前储料实时动作步            成品画面显示      */
	UI16  TM_RELVLVOPN;							/* 0x10160061 储料油马达泄压阀开启时间              */
	UI16  CHARGE_SPEED;                         // 0x10160062 储料速度，mm/ms
	UI16  SUCK_SPEED;                         	// 0x10160063 射退速度，mm/ms
	
	UI16  TM_INJ_START;                         /* 0x10160064 射出计时起始时间		    单位:10ms      去除该功能，面板画线改为由0开启，否则所有相关的都得32位，数据太多 20200219*/
	UI16  TM_CHARGE_START;                      /* 0x10160065 储料计时起始时间		    单位:10ms     去除该功能，面板画线改为由0开启，否则所有相关的都得32位，数据太多 20200219 */
	UI16  TM_HOLD_START;					    /* 0x10160066 保压计时起始时间		    单位:10ms     去除该功能，面板画线改为由0开启，否则所有相关的都得32位，数据太多 20200219*/
	UI16  TM_SUCKBACK_START;					/* 0x10160067 储后射退计时起始时间			单位:10ms     去除该功能，面板画线改为由0开启，否则所有相关的都得32位，数据太多 20200219*/
	UI16  TM_HOLD_RT_L;							/* 0x10160068 实时计算的保压时间，用于面板显示，0.00s */
	UI16  TM_SUCKBK_RT; 						/* 0x10160069 实时计算的射退时间，用于面板显示，0.00s */
	UI16  TM_INJ_H;								/* 0x1016006A 射出计时实际值高16位，小数点0.00，20200219 */
	UI16  Qc_Max_InjTm_H;                       /* 0x1016006B 射出时间最大值高16位，小数点0.00，20200219 */
    UI16  Qc_Min_InjTm_H;                       /* 0x1016006C 射出时间最小值高16位，小数点0.00，20200219 */
	UI16  Qc_Avg_InjTm_H;                       /* 0x1016006D 射出时间平均值高16位，小数点0.00，20200219 */
	UI16  Qc_Deviation_Max_InjTm_H;             /* 0x1016006E 射出时间最大偏差值高16位  小数点0.00  20200219*/
	
	UI16  TM_HOLD_RT_H;							/* 0x1016006F 实时计算的保压时间，用于面板显示，高位 0.00s */
	UI16  TM_HOLD_L;							/* 0x10160070 保压时间实际值 低位 10ms*/
	UI16  TM_HOLD_H;							/* 0x10160071 保压时间实际值 高位 10ms*/
	UI16  TM_INJ_RT_H;							/* 0x10160072 实时计算的射出时间，高位 用于面板显示, 0.00s。*/

    UI16  TM_INJ2_L;                            /* 0x10160073 射出时间实际值 低位 (动作完成上传)*/
    UI16  TM_INJ2_H;                            /* 0x10160074 射出时间实际值 高位 (动作完成上传) */
}INJECT_STATE, *PINJECT_STATE;

#pragma pack()

#ifdef DIC_ENUM_NEED

/*ACT SN start from 1*/
enum EN_INJECT_ACT_SN
{
    OS_INJE = 1,			// 1  ; '射出    '
    //	OS0_HLDP,           // 2  ; '保压    '
    OS_CHRG,    			// 2  ; '储料    '
    OS_SUCK,    			// 3  ; '射退    '

    OS_AINJE,   			// 4  ; '调模射出    '
    OS_NZLINJ,              // 5  ; '座台进射出 '

    OS_ACHRG,   			// 6  ; '调模储料'
    OS_PCHRG,   			// 7  ; '自动清料储料'
    OS_CHRGBFINJ,           // 8  ;  储料(射出前储料或二次射出储料)

    OS_ASUCK,   			// 9  ; '调模射退    '
    OS_SUCKCHG,             // 10 ; '储前射退    '

    OS_APURG,               // 11    自动清料控制动作
    OS_ACHINJ,              // 12    多次储料  //20190403 CHJ 补齐
    OS_DACHRG,				// 13	DA调试储料动作
    OS_NEEDLEVALVE,			// 14	胶口针阀动作
    OS_SUCK_SYNC,           // 15	同步射退动作(免烘干机)
    
    INJECT_ACT_SN_END
};

#define HOLD_STEP_NUM		10
#define CHARGE_STEP_NUM		5
enum EN_INJECT_ACTNAME
{
    INJCT1	= 	STAGE_1_STEP,           //注射 1
    INJCT2	=	STAGE_2_STEP,           //注射 2
    INJCT3	=	STAGE_3_STEP,           //注射 3
    INJCT4	=	STAGE_4_STEP,           //注射 4
    INJCT5	=	STAGE_5_STEP,           //注射 5
    INJCT6	=	STAGE_6_STEP,           //注射 6
    INJCT7	= 	STAGE_7_STEP,           //注射 7
    INJCT8	=	STAGE_8_STEP,           //注射 8
    INJCT9	=	STAGE_9_STEP,           //注射 9
    INJCT10 =	STAGE_10_STEP,          //注射 10
    INJCTTWAIT	= STAGE_11_STEP,
    INJCTEND = STAGE_12_STEP,

    INJTWIC =	STAGE_SPEC1_STEP,
    INJREDUCE = STAGE_SPEC2_STEP,       //减速特殊步		20170505  LET

    HOLD1	=	STAGE_13_STEP,			//保压 1
    HOLD2	=	STAGE_14_STEP,			//保压 2
    HOLD3	=	STAGE_15_STEP,			//保压 3
    HOLD4	=	STAGE_16_STEP,			//保压 4
    HOLD5	=	STAGE_17_STEP,			//保压 5
    HOLD6	=	STAGE_18_STEP,			//保压 6
    HOLD7	=	STAGE_19_STEP,			//保压 7
    HOLD8	=	STAGE_20_STEP,			//保压 8
    HOLD9	=	STAGE_21_STEP,			//保压 9
    HOLD10	=	STAGE_22_STEP,          //保压 10
    HOLDEND = END_STEP,

    CHARGE1	= 	STAGE_1_STEP,           //储料 1
    CHARGE2	=	STAGE_2_STEP,           //储料 2
    CHARGE3	=	STAGE_3_STEP,           //储料 3
    CHARGE4	=	STAGE_4_STEP,           //储料 4
    CHARGE5	=	STAGE_5_STEP,           //储料 5

    SUCKBACK	=	STAGE_1_STEP,		//射退
    SUCKBACK2	=	STAGE_2_STEP		//射退2
};

#define     ER0_CUSH      0x0001      //#10100 '射出监控异常  '  
#define     ER0_PURG      0x0002      //#10101 '射出护罩未关'
#define     ER0_HPEM      0x0004      //#10102 '储料超时'
#define     ER0_INJS      0x0008      //#10103 '射出起始位置偏差'
#define     ER0_SRPM      0x0010      //#10104 '储料转速偏差  '
#define     ER0_INJT      0x0020      //#10105 '射出时间偏差  '
#define     ER0_SUCK      0x0040      //#10106 '射退终未到定位'
#define     ER0_CHRG      0x0080      //#10107 '储料终未到定位'
#define     ER0_CHRP      0x0100      //#10108 '储压过高  '
#define     ER0_HOPM      0x0200      //#10109 '料斗检知异常  '  
#define     ER0_PMAT      0x0400      //#1010A '允许储料    '
#define     ER0_TEMP      0x0800      //#1010B '料筒温度偏差      '
#define     ER0_CHGL      0x1000      //#1010C '料位过低  '
#define     ER0_CHFL      0x2000      //#1010D '储料位置已到  '           16-04-18
#define     ER0_WARM      0x4000      //#1010E '电热未开启  '         16-05-30
#define     ER0_PRTM      0x8000      //#1010F '射出保护时间到'          17-04-26  //20170505 LET
#define     ER0_CHBF      0x10000     //#10110 '请使用储前射退'     20200729  
#define     ER0_CHYX      0x20000     //#10111 '储料未在有效区内'  	20200729
#define     ER1_NOTMPCHK    0x40000     //#10112    '请注意，料筒温度检测已关闭'    20220209    zt

#endif


#ifdef __cplusplus
}
#endif

#endif

