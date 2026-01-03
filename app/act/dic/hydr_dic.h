#ifndef _HYDR_DIC_H_
#define _HYDR_DIC_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"

#pragma pack(1)

#define     MAXDAPORT           4

typedef struct EN_HYDR_PO
{
	UI16  O_PQVLV;                    //0x09100000 比例阀开关  
	UI16  O_PWRM_P1 ;                 //0x09100001 泵1    
	UI16  O_PWRM_P2 ;                 //0x09100002 泵2      
	UI16  O_PWRM_P3 ;                 //0x09100003 泵3      
	UI16  O_PWRM_P4 ;                 //0x09100004 泵4        
	UI16  O_PWRM_P5 ;                 //0x09100005 泵5      
	UI16  O_PWRM_P6 ;                 //0x09100006 泵6      
	UI16  O_PWRM_P7 ;                 //0x09100007 泵7        
	UI16  O_PWRM_P8 ;                 //0x09100008 泵8
}HYDR_PO, *PHYDR_PO;

typedef struct EN_HYDR_MACHSET
{
  	UI16  PRESSENSORCH; 				   /*0x09120000 油路液压 chg by dky 2013-1-11*/
     /*第一组*/
	UI16  FL_PWRMATCHTBL;                  /*0x09120001 使用面板PUMP配置表         */

    UI16  TBL_PWRMATCH1;                   /* 0x09120002 速度比例开配置1            */
    UI16  TBL_PWRMATCH2;                   /* 0x09120003 速度比例开配置2            */
    UI16  TBL_PWRMATCH3;                   /* 0x09120004 速度比例开配置3            */
    UI16  TBL_PWRMATCH4;                   /* 0x09120005 速度比例开配置4            */
    UI16  TBL_PWRMATCH5;                   /* 0x09120006 速度比例开配置5            */
    UI16  TBL_PWRMATCH6;                   /* 0x09120007 速度比例开配置6            */
    UI16  TBL_PWRMATCH7;                   /* 0x09120008 速度比例开配置7            */
    UI16  TBL_PWRMATCH8;                   /* 0x09120009 速度比例开配置8            */
    UI16  TBL_PWRMATCH9;                   /* 0x0912000A 速度比例开配置9            */
    UI16  TBL_PWRMATCH10;                  /* 0x0912000B 速度比例开配置10         */
	
    UI16  DAFLOW_PWRMATCH1;                /* 0x0912000C 速度比例开1                */
    UI16  DAFLOW_PWRMATCH2;                /* 0x0912000D 速度比例开2                */
    UI16  DAFLOW_PWRMATCH3;                /* 0x0912000E 速度比例开3                */
    UI16  DAFLOW_PWRMATCH4;                /* 0x0912000F 速度比例开4                */
    UI16  DAFLOW_PWRMATCH5;                /* 0x09120010 速度比例开5                */
    UI16  DAFLOW_PWRMATCH6;                /* 0x09120011 速度比例开6                */
    UI16  DAFLOW_PWRMATCH7;                /* 0x09120012 速度比例开7                */
    UI16  DAFLOW_PWRMATCH8;                /* 0x09120013 速度比例开8                */
    UI16  DAFLOW_PWRMATCH9;                /* 0x09120014 速度比例开9                */
    UI16  DAFLOW_PWRMATCH10;               /* 0x09120015 速度比例开10               */

    UI16  TBL_PUMPLMT1;                    /* 0x09120016 速度比例开限制1            */
    UI16  TBL_PUMPLMT2;                    /* 0x09120017 速度比例开限制2            */
    UI16  TBL_PUMPLMT3;                    /* 0x09120018 速度比例开限制3            */
    UI16  TBL_PUMPLMT4;                    /* 0x09120019 速度比例开限制4            */
    UI16  TBL_PUMPLMT5;                    /* 0x0912001A 速度比例开限制5            */
    UI16  TBL_PUMPLMT6;                    /* 0x0912001B 速度比例开限制6            */
    UI16  TBL_PUMPLMT7;                    /* 0x0912001C 速度比例开限制7            */
    UI16  TBL_PUMPLMT8;                    /* 0x0912001D 速度比例开限制8            */
    UI16  TBL_PUMPLMT9;                    /* 0x0912001E 速度比例开限制9            */
    UI16  TBL_PUMPLMT10;                   /* 0x0912001F 速度比例开限制10           */
    UI16  TBL_PUMPLMT11;                   /* 0x09120020 速度比例开限制11           */
    UI16  TBL_PUMPLMT12;                   /* 0x09120021 速度比例开限制12            */
    UI16  TBL_PUMPLMT13;                   /* 0x09120022 速度比例开限制13            */
    UI16  TBL_PUMPLMT14;                   /* 0x09120023 速度比例开限制14            */
    UI16  TBL_PUMPLMT15;                   /* 0x09120024 速度比例开限制15            */
    UI16  TBL_PUMPLMT16;                   /* 0x09120025 速度比例开限制16            */
    UI16  TBL_PUMPLMT17;                   /* 0x09120026 速度比例开限制17            */
    UI16  TBL_PUMPLMT18;                   /* 0x09120027 速度比例开限制18            */
    UI16  TBL_PUMPLMT19;                   /* 0x09120028 速度比例开限制19            */
    UI16  TBL_PUMPLMT20;                   /* 0x09120029 速度比例开限制20            */
	UI16  HY_DELAY;                        /* 0x0912002A 液压延迟*/

    UI16 MINFLOW_LOWPRES;                  /* 0x0912002B 低压最低转速 0--3000        */
    UI16 MINFLOW_MEDPRES;                  /* 0x0912002C 中压最低转速 0--3000        */
    UI16 MINFLOW_HIPRES;                   /* 0x0912002D 高压最低转速 0--3000        */
    UI16 MAXFLOW_SYSSET;                   /* 0x0912002E 设定最大转速 1500-3000      */
    UI16 CHGFLOW_TESTSET;                  /* 0x0912002F 电机转速波动最大值 0-100    */
    
    UI16 HIFLOW_ADJPARA;                    /*0x09120030 高速度射出开环校正系数*/
    UI16 LOWFLOW_ADJPARA;                   /*0x09120031 低速度射出开环校正系数*/
	UI16 DA_TBL_PUMPOUT;                    /*0x09120032 DA校正画面泵浦输出选择默认0 不输出  0-0xffff,按位与*/                 

	/*油路压力PI自学习*/
	UI16 PRESSELF_PORT;						/*0x09120033	学习通道 默认1: 按位操作位-->P1 P2 P3*/
	UI16 PRESSELF_PUMP;						/*0x09120034	学习pump表 默认P1 按位操作位-->P1……P8*/
	UI16 PRESSELF_PRES;						/*0x09120035	自学习压力	默认50bar	20~100bar*/
	UI16 PRESSELF_FLOW;						/*0x09120036	自学习流量	默认30%		20~99%*/
	UI16 PRESSELF_P;						/*0x09120037	自学习得P	范围100~2000*/
	UI16 PRESSELF_I;						/*0x09120038	自学习得I	范围5~100*/
	
}HYDR_MACHSET, *PHYDR_MACHSET;

typedef struct tyHYDR_DA_PRESS_CUR
{
    UI16  DA_PRSOUT0;                       /* 0x9140000 -0x9150023压力DA 0%    */
    UI16  DA_PRSOUT1;                       /* 0x9140001 压力DA 10%                 */
    UI16  DA_PRSOUT2;                       /* 0x9140002 压力DA 20%                 */
    UI16  DA_PRSOUT3;                       /* 0x9140003 压力DA 30%                 */
    UI16  DA_PRSOUT4;                       /* 0x9140004 压力DA 40%                 */
    UI16  DA_PRSOUT5;                       /* 0x9140005 压力DA 50%                 */
    UI16  DA_PRSOUT6;                       /* 0x9140006 压力DA 60%                 */
    UI16  DA_PRSOUT7;                       /* 0x9140007 压力DA 70%                 */
    UI16  DA_PRSOUT8;                       /* 0x9140008 压力DA 80%                 */
    UI16  DA_PRSOUT9;                       /* 0x9140009 压力DA 90%                  */
	UI16  DA_PRSOUT10;                      /* 0x914000A 压力DA 100%                 */
	UI16  DA_PRSOUT11;                      /* 0x914000B 压力DA 110%                 */
	UI16  DA_PRSOUT12;                      /* 0x914000C 压力DA 120%                 */
	UI16  DA_PRSOUT13;                      /* 0x914000D 压力DA 130%                 */
	UI16  DA_PRSOUT14;                      /* 0x914000E 压力DA 140%                 */
	UI16  DA_PRSOUT15;                      /* 0x914000F 压力DA 150%                 */
	UI16  DA_PRSOUT16;                      /* 0x9140010 压力DA 160%                 */
	UI16  DA_PRSOUT17;                      /* 0x9140011 压力DA 170%                 */
	UI16  DA_PRSOUT18;                      /* 0x9140012 压力DA 180%                 */
	UI16  DA_PRSOUT19;                      /* 0x9140013 压力DA 190%                 */
	UI16  DA_PRSOUT20;                      /* 0x9140014 压力DA 200%                 */
	UI16  DA_PRSOUT21;                      /* 0x9140015 压力DA 210%                 */
	UI16  DA_PRSOUT22;                      /* 0x9140016 压力DA 220%                 */
	UI16  DA_PRSOUT23;                      /* 0x9140017 压力DA 230%                 */
	UI16  DA_PRSOUT24;                      /* 0x9140018 压力DA 240%                 */

    UI16  DA_FLOWOUT0;                      /* 0x9140019 速度 DA 0%                 */
    UI16  DA_FLOWOUT1;                      /* 0x914001A 速度 DA 10%                */
    UI16  DA_FLOWOUT2;                      /* 0x914001B 速度 DA 20%                */
    UI16  DA_FLOWOUT3;                      /* 0x914001C 速度 DA 30%                */
    UI16  DA_FLOWOUT4;                      /* 0x914001D 速度 DA 40%                */
    UI16  DA_FLOWOUT5;                      /* 0x914001E 速度 DA 50%                */
    UI16  DA_FLOWOUT6;                      /* 0x914001F 速度 DA 60%                */
    UI16  DA_FLOWOUT7;                      /* 0x9140020 速度 DA 70%                */
    UI16  DA_FLOWOUT8;                      /* 0x9140021 速度 DA 80%                */
    UI16  DA_FLOWOUT9;                      /* 0x9140022 速度 DA 90%                */
    UI16  DA_FLOWOUT10;                     /* 0x9140023 速度 DA 100%               */
}HYDR_DA_CUR, *PHYDR_DA_CUR;

typedef struct tyHYDR_DA_PRESS_SETVAL
{
    UI16  DASET_PRSOUT0;                        /* 0x9140090 压力DA 0%                  */
    UI16  DASET_PRSOUT1;                        /* 0x9140091 压力DA 10%                 */
    UI16  DASET_PRSOUT2;                        /* 0x9140092 压力DA 20%                 */
    UI16  DASET_PRSOUT3;                        /* 0x9140093 压力DA 30%                 */
    UI16  DASET_PRSOUT4;                        /* 0x9140094 压力DA 40%                 */
    UI16  DASET_PRSOUT5;                        /* 0x9140095 压力DA 50%                 */
    UI16  DASET_PRSOUT6;                        /* 0x9140096 压力DA 60%                 */
    UI16  DASET_PRSOUT7;                        /* 0x9140097 压力DA 70%                 */
    UI16  DASET_PRSOUT8;                        /* 0x9140098 压力DA 80%                 */
    UI16  DASET_PRSOUT9;                        /* 0x9140099 压力DA 90%                   */
    UI16  DASET_PRSOUT10;                       /* 0x914009A 压力DA 100%                 */
    UI16  DASET_PRSOUT11;                       /* 0x914009B 压力DA 110%                 */
    UI16  DASET_PRSOUT12;                       /* 0x914009C 压力DA 120%                 */
    UI16  DASET_PRSOUT13;                       /* 0x914009D 压力DA 130%                 */
    UI16  DASET_PRSOUT14;                       /* 0x914009E 压力DA 140%                 */
    UI16  DASET_PRSOUT15;                       /* 0x914009F 压力DA 150%                 */
    UI16  DASET_PRSOUT16;                       /* 0x91400A0 压力DA 160%                 */
    UI16  DASET_PRSOUT17;                       /* 0x91400A1 压力DA 170%                 */
    UI16  DASET_PRSOUT18;                       /* 0x91400A2 压力DA 180%                 */
    UI16  DASET_PRSOUT19;                       /* 0x91400A3 压力DA 190%                 */
    UI16  DASET_PRSOUT20;                       /* 0x91400A4 压力DA 200%                 */
    UI16  DASET_PRSOUT21;                       /* 0x91400A5 压力DA 210%                 */
    UI16  DASET_PRSOUT22;                       /* 0x91400A6 压力DA 220%                 */
    UI16  DASET_PRSOUT23;                       /* 0x91400A7 压力DA 230%                 */
    UI16  DASET_PRSOUT24;                       /* 0x91400A8 压力DA 240%                 */

    UI16  DASET_FLOWOUT0;                       /* 0x91400A9 速度 DA 0%                 */
    UI16  DASET_FLOWOUT1;                       /* 0x91400AA 速度 DA 10%                 */
    UI16  DASET_FLOWOUT2;                       /* 0x91400AB 速度 DA 20%                */
    UI16  DASET_FLOWOUT3;                       /* 0x91400AC 速度 DA 30%                */
    UI16  DASET_FLOWOUT4;                       /* 0x91400AD 速度 DA 40%                */
    UI16  DASET_FLOWOUT5;                       /* 0x91400AE 速度 DA 50%                */
    UI16  DASET_FLOWOUT6;                       /* 0x91400AF 速度 DA 60%                */
    UI16  DASET_FLOWOUT7;                       /* 0x91400B0 速度 DA 70%                */
    UI16  DASET_FLOWOUT8;                       /* 0x91400B1 速度 DA 80%                */
    UI16  DASET_FLOWOUT9;                       /* 0x91400B2 速度 DA 90%                */
    UI16  DASET_FLOWOUT10;                      /* 0x91400B3 速度 DA 100%                */
}HYDR_DA_SETVAL, *PHYDR_DA_SETVAL;

#define DA_PRS_NUM		25
#define DA_FLOW_NUM		11

typedef struct tyHYDR_ZDSET
{
    HYDR_DA_CUR      DA_CUR[MAXDAPORT];		//0x09140000-0x09140023  0x09140000-0x0914008F 共0x90个 DA压力速度曲线
    HYDR_DA_SETVAL   DA_SET[MAXDAPORT];		//0x09140090-0x091400B3  0x09140090-0x0914011F 共0x90个 DA压力速度曲线设定值
    UI16 WPRESSLINE0;                       // 0x09140120    ;压力测试曲线写入标记,伺服比例阀曲线   
    UI16 WPRESSLINE1;                       // 0x09140121        1-21 第一路压力曲线
    UI16 WPRESSLINE2;                       // 0x09140122    
    UI16 WPRESSLINE3;                       // 0x09140123    
    UI16 WPRESSLINE4;                       // 0x09140124    
    UI16 WPRESSLINE5;                       // 0x09140125    
    UI16 WPRESSLINE6;                       // 0x09140126    
    UI16 WPRESSLINE7;                       // 0x09140127    
    UI16 WPRESSLINE8;                       // 0x09140128    
    UI16 WPRESSLINE9;                       // 0x09140129    
    UI16 WPRESSLINE10;                      // 0x0914012A    
    UI16 WPRESSLINE11;                      // 0x0914012B    
    UI16 WPRESSLINE12;                      // 0x0914012C    
    UI16 WPRESSLINE13;                      // 0x0914012D    
    UI16 WPRESSLINE14;                      // 0x0914012E    
    UI16 WPRESSLINE15;                      // 0x0914012F    
    UI16 WPRESSLINE16;                      // 0x09140130    
    UI16 WPRESSLINE17;                      // 0x09140131    
    UI16 WPRESSLINE18;                      // 0x09140132    
    UI16 WPRESSLINE19;                      // 0x09140133    
    UI16 WPRESSLINE20;                      // 0x09140134  
}HYDR_ZDSET, *PHYDR_ZDSET;

typedef struct EN_HYDR_MOLDSET
{
	/*功能*/
	UI16  FL_FLOWSHOW;      //0x09130000
    UI16  DACHK_CHG;        //0x09130001 DA调整速度时输出储料
}HYDR_MOLDSET, *PHYDR_MOLDSET;


typedef struct EN_HYDR_STATE
{
	UI16  FL_ERROR0;						//0x09160000  警报1
	UI16  FL_ERROR1;						//0x09160001  警报2
		
	UI16  DA_OUTPUT0;      					// 0x09160002 DA输出1
	UI16  DA_OUTPUT1;                       // 0x09160003 DA输出2
	UI16  DA_OUTPUT2;                       // 0x09160004 DA输出3
	UI16  DA_OUTPUT3;                       // 0x09160005 DA输出4
	UI16  DA_OUTPUT4;                       // 0x09160006 DA输出5
	UI16  DA_OUTPUT5;                       // 0x09160007 DA输出6
	UI16  DA_OUTPUT6;                       // 0x09160008 DA输出7
	UI16  DA_OUTPUT7;                       // 0x09160009 DA输出8

	UI16  DA_HYDRAUMAX0;                    // 0x0916000A 比例阀最大值  
	UI16  DA_HYDRAUMAX1;                    // 0x0916000B  
	UI16  DA_HYDRAUMAX2;                    // 0x0916000C  
	UI16  DA_HYDRAUMAX3;                    // 0x0916000D  
	UI16  DA_HYDRAUMAX4;                    // 0x0916000E  
	UI16  DA_HYDRAUMAX5;                    // 0x0916000F  
	UI16  DA_HYDRAUMAX6;                    // 0x09160010  
	UI16  DA_HYDRAUMAX7;                    // 0x09160011  
	UI16  DA_TEMPERMAX0;                    // 0x09160012 温度最大值    
}HYDR_STATE, *PHYDR_STATE;

#pragma pack()

enum EN_HYDR_ACT_SN
{
	OS_PRESSELF = 1,

	HYDR_ACT_SN_END
};

#define     ER0_PRSSENOR    0x0001      //#09100 '压力传感器无效'  
#define 	ER0_PRSZSET		0x0002		//#09101 '压力传感器未归零'  
#define 	ER0_SERVOCOM	0x0004		//#09102 '伺服驱动器异常
#define     ER0_OILH        0x0008      //#09103 '油温过高    '
#define     ER0_LENNOW      0x0010      //#09104 '自整定中'
#define     ER0_LENSAF      0x0020      //#09105 '保存完毕，请按手动键'
#define     ER0_LENRPM      0x0040      //#09106 '转速波动过大  '	            //自整定时，15s后未通过
#define     ER0_SAFING      0x0080      //#09107 '自整定结束,正在保存中'
#define     ER0_PRESET      0x0100      //#09108 '未到设定压力'
#define     ER0_MODE      	0x0200      //#09109 '模式错误,请切换至手动状态'
#define     ER0_PRESERR     0x0400      //#0910A '系统压力>5公斤，无法转为从机  '                 2015.1110


#ifdef DIC_ENUM_NEED

#endif

#ifdef __cplusplus
}
#endif

#endif




