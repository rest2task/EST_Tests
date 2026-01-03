#ifndef _QCMONITOR_DIC_H_
#define _QCMONITOR_DIC_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"

#pragma pack(1)


typedef struct EN_QCMONITOR_MOLDSET
{
	/*		  周期QC监控参数	*/
	UI16   Qc_Fun;                    /* 1B130000   QC监控功能  0: 不使用  1: 使用  默认不使用*/
    UI16   Qc_OpnNum;                 /* 1B130001   QC监控模数  50~200  默认50*/
    UI16  Qc_DeviationSet_CycTm;      /* 1B130002   周期时间允许偏差     默认0.0*/
    UI16  Qc_TmSet_CycTm;             /* 1B130003   周期时间设定值   默认0.0*/
	UI16  Qc_Mode_CycTm;              /* 1B130004   周期时间监控:0:不使用  1:使用   默认不使用*/	


	/*		  注射部件QC监控参数	*/
	 UI16  Qc_DeviationSet_InjTm;				/* 1B130005 射出时间允许偏差	 默认0.00*/
	 UI16  Qc_TmSet_InjTm;						/* 1B130006 射出时间设定值	 默认0.00*/
	 UI16  Qc_Mode_InjTm;						/* 1B130007 射出时间监控:0:不使用  1:使用	默认不使用*/ 
	
	 UI16  Qc_DeviationSet_InjSpd;				  /* 1B130008 射出速度允许偏差	   默认0.00*/
	 UI16  Qc_TmSet_InjSpd; 					  /* 1B130009 射出速度设定值   默认0.00*/
	 UI16  Qc_Mode_InjSpd;						  /* 1B13000A 射出速度监控:0:不使用  1:使用   默认不使用*/		 
	
	 UI16  Qc_DeviationSet_InjEnd;				  /* 1B13000B 保压转化位置允许偏差	   默认0.00*/
	 UI16  Qc_TmSet_InjEnd; 					  /* 1B13000C 保压转化位置设定值   默认0.00*/
	 UI16  Qc_Mode_InjEnd;						  /* 1B13000D 保压转化位置监控:0:不使用  1:使用   默认不使用*/		 
	
	 UI16  Qc_DeviationSet_InjPrs;				  /* 1B13000E 射出最大压力允许偏差	   默认0.0*/
	 UI16  Qc_TmSet_InjPrs; 					  /* 1B13000F 射出最大压力设定值   默认0.0*/
	 UI16  Qc_Mode_InjPrs;						  /* 1B130010 射出最大压力监控:0:不使用  1:使用   默认不使用*/		 
	
	 UI16  Qc_DeviationSet_HoldEnd; 			  /* 1B130011 残料位置允许偏差	   默认0.00*/
	 UI16  Qc_TmSet_HoldEnd;					  /* 1B130012 残料位置设定值   默认0.00*/
	 UI16  Qc_Mode_HoldEnd; 					  /* 1B130013 残料位置监控:0:不使用  1:使用   默认不使用*/		 
	
	 UI16  Qc_DeviationSet_ChgTm;				  /* 1B130014 储料时间允许偏差	   默认0.00*/
	 UI16  Qc_TmSet_ChgTm;						  /* 1B130015 储料时间设定值   默认0.00*/
	 UI16  Qc_Mode_ChgTm;						  /* 1B130016 储料时间监控:0:不使用  1:使用   默认不使用*/		 
	
	 UI16  Qc_DeviationSet_ChgEnd;				  /* 1B130017 储料终止位置允许偏差	   默认0.00*/
	 UI16  Qc_TmSet_ChgEnd; 					  /* 1B130018 储料终止位置设定值   默认0.00*/
	 UI16  Qc_Mode_ChgEnd;						  /* 1B130019 储料终止位置监控:0:不使用  1:使用   默认不使用*/		 
	
	 UI16  Qc_DeviationSet_SuckBwd; 			  /* 1B13001A 射退终止位置允许偏差	   默认0.00*/
	 UI16  Qc_TmSet_SuckBwd;					  /* 1B13001B 射退终止位置设定值   默认0.00*/
	 UI16  Qc_Mode_SuckBwd; 					  /* 1B13001C 射退终止位置监控:0:不使用  1:使用   默认不使用*/		 
	
	 UI16  Qc_DeviationSet_SuckTm;				  /* 1B13001D 射退时间允许偏差	   默认0.00*/
	 UI16  Qc_TmSet_SuckTm; 					  /* 1B13001E 射退时间设定值   默认0.00*/
	 UI16  Qc_Mode_SuckTm;						  /* 1B13001F 射退时间监控:0:不使用  1:使用   默认不使用*/			 


	/*		  开合模部件QC监控参数				 */
	 UI16  Qc_DeviationSet_ClsTm;			   /* 1B130020 合模时间允许偏差 	默认0.00*/
	 UI16  Qc_TmSet_ClsTm;					   /* 1B130021 合模时间设定值	默认0.00*/
	 UI16  Qc_Mode_ClsTm;					   /* 1B130022 合模时间监控:0:不使用  1:使用   默认不使用*/
	
	 UI16  Qc_DeviationSet_ClsLow;			   /* 1B130023 合模低压时间允许偏差	 默认0.000*/
	 UI16  Qc_TmSet_ClsLow; 				   /* 1B130024 合模低压时间设定值	默认0.000*/
	 UI16  Qc_Mode_ClsLow;					   /* 1B130025 合模低压监控:0:不使用  1:使用   默认不使用*/

	 UI16  Qc_DeviationSet_ClsHi;			   /* 1B130026 合模高压时间允许偏差 	默认0.000*/
	 UI16  Qc_TmSet_ClsHi;					   /* 1B130027合模高压时间设定值   默认0.000*/
	 UI16  Qc_Mode_ClsHi;					   /* 1B130028 合模高压时间监控:0:不使用  1:使用   默认不使用*/  

	 UI16  Qc_DeviationSet_OpnEnd;			   /* 1B130029 开模终止位置允许偏差 	默认0.00*/
	 UI16  Qc_TmSet_OpnEnd; 				   /* 1B13002A 开模终止位置设定值	默认0.00*/
	 UI16  Qc_Mode_OpnEnd;					   /* 1B13002B 开模终止位置监控:0:不使用  1:使用   默认不使用*/  

	 UI16  Qc_DeviationSet_OpnTm;			   /* 1B13002C 开模时间允许偏差 	默认0.00*/
	 UI16  Qc_TmSet_OpnTm;					   /* 1B13002D 开模时间设定值	默认0.00*/
	 UI16  Qc_Mode_OpnTm;					   /* 1B13002E 开模时间监控:0:不使用  1:使用   默认不使用*/  

	 UI16  Qc_DeviationSet_OpnSpd;			   /* 1B13002F开模速度允许偏差	   默认0.00*/
	 UI16  Qc_TmSet_OpnSpd; 				   /* 1B130030 开模速度设定值	默认0.00*/
	 UI16  Qc_Mode_OpnSpd;					   /* 1B130031开模速度监控:0:不使用  1:使用   默认不使用*/	 

	/*		  脱模QC监控参数				 */
	 UI16  Qc_DeviationSet_EjtTm;			/*1B130032 顶针时间允许偏差 	默认0.00*/
	 UI16  Qc_TmSet_EjtTm;					/*1B130033 顶针时间设定值	默认0.00*/
	 UI16  Qc_Mode_EjtTm;					/*1B130034 顶针时间监控:0:不使用  1:使用   默认不使用*/

}QCMONITOR_MOLDSET, *PQCMONITOR_MOLDSET;

typedef struct EN_QCMONITOR_STATE
{
	UI16  FL_ERROR0;								/*1B160000 警报1*/
	UI16  FL_ERROR1;								/*1B160001 警报2*/

	UI16  Qc_Opn_Cnt;                          /*1B160002 QC监控模数*/
    UI16  Qc_Avg_CycTm;                        /*1B160003 周期时间平均值*/
    UI16  Qc_Deviation_Max_CycTm;              /*1B160004 周期时间最大偏差值*/
	UI16  Qc_Unq_Cnt_CycTm;                    /*1B160005 周期时间不合格数*/
	UI16  Qc_Unq_Rate_CycTm;                   /*1B160006 周期时间不合格率*/
	UI16  Qc_Max_CycTm;                        /*1B160007 周期时间最大值*/
	UI16  Qc_Min_CycTm;	                       /*1B160008 周期时间最小值*/		


	/*        QC监控参数                 2014-07-06*/
    UI16  Qc_Avg_InjTm;                       /*1B160009 射出时间平均值*/
    UI16  Qc_Deviation_Max_InjTm;             /*1B16000A 射出时间最大偏差值*/
	UI16  Qc_Unq_Cnt_InjTm;                   /*1B16000B 射出时间不合格数*/
	UI16  Qc_Unq_Rate_InjTm;                  /*1B16000C 射出时间不合格率*/
	UI16  Qc_Max_InjTm;                       /*1B16000D 射出时间最大值*/
	UI16  Qc_Min_InjTm;	                      /*1B16000E 射出时间最小值*/	

    UI16  Qc_Avg_InjSpd;                      /*1B16000F 射出速度平均值*/
    UI16  Qc_Deviation_Max_InjSpd;            /*1B160010 射出速度最大偏差值*/
	UI16  Qc_Unq_Cnt_InjSpd;                  /*1B160011 射出速度不合格数*/
	UI16  Qc_Unq_Rate_InjSpd;                 /*1B160012 射出速度不合格率*/
	UI16  Qc_Max_InjSpd;                      /*1B160013 射出速度最大值*/
	UI16  Qc_Min_InjSpd;	                  /*1B160014 射出速度最小值*/	

    UI16  Qc_Avg_InjEnd;                      /*1B160015 保压转换位置平均值*/
    UI16  Qc_Deviation_Max_InjEnd;            /*1B160016 保压转换位置最大偏差值*/
	UI16  Qc_Unq_Cnt_InjEnd;                  /*1B160017 保压转换位置不合格数*/
	UI16  Qc_Unq_Rate_InjEnd;                 /*1B160018 保压转换位置不合格率*/
	UI16  Qc_Max_InjEnd;                      /*1B160019 保压转换位置最大值*/
	UI16  Qc_Min_InjEnd;	                  /*1B16001A 保压转换位置最小值*/	

    UI16  Qc_Avg_InjPrs;                      /*1B16001B 射出压力平均值*/
    UI16  Qc_Deviation_Max_InjPrs;            /*1B16001C 射出压力最大偏差值*/
	UI16  Qc_Unq_Cnt_InjPrs;                  /*1B16001D 射出压力不合格数*/
	UI16  Qc_Unq_Rate_InjPrs;                 /*1B16001E 射出压力不合格率*/
	UI16  Qc_Max_InjPrs;                      /*1B16001F 射出压力最大值*/
	UI16  Qc_Min_InjPrs;	                  /*1B160020 射出压力最小值*/	

    UI16  Qc_Avg_HoldEnd;                      /*1B160021 残料位置平均值*/
    UI16  Qc_Deviation_Max_HoldEnd;            /*1B160022 残料位置最大偏差值*/
	UI16  Qc_Unq_Cnt_HoldEnd;                  /*1B160023 残料位置不合格数*/
	UI16  Qc_Unq_Rate_HoldEnd;                 /*1B160024 残料位置不合格率*/
	UI16  Qc_Max_HoldEnd;                      /*1B160025 残料位置最大值*/
	UI16  Qc_Min_HoldEnd;	                   /*1B160026 残料位置最小值*/		

    UI16  Qc_Avg_ChgTm;                        /*1B160027 储料时间平均值*/
    UI16  Qc_Deviation_Max_ChgTm;              /*1B160028 储料时间最大偏差值*/
	UI16  Qc_Unq_Cnt_ChgTm;                    /*1B160029 储料时间不合格数*/
	UI16  Qc_Unq_Rate_ChgTm;                   /*1B16002A 储料时间不合格率*/
	UI16  Qc_Max_ChgTm;                        /*1B16002B 储料时间最大值*/
	UI16  Qc_Min_ChgTm;	                       /*1B16002C 储料时间最小值*/		

    UI16  Qc_Avg_ChgEnd;                        /*1B16002D 储料终止位置平均值*/
    UI16  Qc_Deviation_Max_ChgEnd;              /*1B16002E 储料终止位置最大偏差值*/
	UI16  Qc_Unq_Cnt_ChgEnd;                    /*1B16002F 储料终止位置不合格数*/
	UI16  Qc_Unq_Rate_ChgEnd;                   /*1B160030 储料终止位置不合格率*/
	UI16  Qc_Max_ChgEnd;                        /*1B160031 储料终止位置最大值*/
	UI16  Qc_Min_ChgEnd;	                    /*1B160032 储料终止位置最小值*/		

    UI16  Qc_Avg_SuckBwd;                       /*1B160033 射退终止位置平均值*/
    UI16  Qc_Deviation_Max_SuckBwd;             /*1B160034 射退终止位置最大偏差值*/
	UI16  Qc_Unq_Cnt_SuckBwd;                   /*1B160035 射退终止位置不合格数*/
	UI16  Qc_Unq_Rate_SuckBwd;                  /*1B160036 射退终止位置不合格率*/
	UI16  Qc_Max_SuckBwd;                       /*1B160037 射退终止位置最大值*/
	UI16  Qc_Min_SuckBwd;	                    /*1B160038 射退终止位置最小值*/			

    UI16  Qc_Avg_SuckTm;                        /*1B160039 射退时间平均值*/
    UI16  Qc_Deviation_Max_SuckTm;              /*1B16003A 射退时间最大偏差值*/
	UI16  Qc_Unq_Cnt_SuckTm;                    /*1B16003B 射退时间不合格数*/
	UI16  Qc_Unq_Rate_SuckTm;                   /*1B16003C 射退时间不合格率*/
	UI16  Qc_Max_SuckTm;                        /*1B16003D 射退时间最大值*/
	UI16  Qc_Min_SuckTm;	                    /*1B16003E 射退时间最小值*/	

	 /*        QC监控参数                 2014-07-06*/
    UI16  Qc_Avg_Cls;                           /*1B16003F 合模时间平均值*/
    UI16  Qc_Deviation_Max_Cls;                 /*1B160041 合模时间最大偏差值*/
	UI16  Qc_Unq_Cnt_Cls;                   	/*1B160042 合模时间不合格数*/
	UI16  Qc_Unq_Rate_Cls;                   	/*1B160043 合模时间不合格率*/
	UI16  Qc_Max_Cls;                           /*1B160044 合模时间最大值*/
	UI16  Qc_Min_Cls;	                        /*1B160045 合模时间最小值*/

    UI16  Qc_Avg_ClsLow;                        /*1B160046 合模低压时间平均值*/
    UI16  Qc_Deviation_Max_ClsLow;              /*1B160047 合模低压时间最大偏差值*/
	UI16  Qc_Unq_Cnt_ClsLow;                   	/*1B160048 合模低压时间不合格数*/
	UI16  Qc_Unq_Rate_ClsLow;                   /*1B160049 合模低压时间不合格率*/
	UI16  Qc_Max_ClsLow;                        /*1B16004A 合模低压时间最大值*/
	UI16  Qc_Min_ClsLow;	                    /*1B16004B 合模低压时间最小值*/	

    UI16  Qc_Avg_ClsHi;                         /*1B16004C 合模高压时间平均值*/
    UI16  Qc_Deviation_Max_ClsHi;               /*1B16004D 合模高压时间最大偏差值*/
	UI16  Qc_Unq_Cnt_ClsHi;                   	/*1B16004E 合模高压时间不合格数*/
	UI16  Qc_Unq_Rate_ClsHi;                   	/*1B16004F 合模高压时间不合格率*/
	UI16  Qc_Max_ClsHi;                         /*1B160050 合模高压时间最大值*/
	UI16  Qc_Min_ClsHi;	                        /*1B160051 合模高压时间最小值*/	

    UI16  Qc_Avg_OpnEnd;                        /*1B160052 开模位置平均值*/
    UI16  Qc_Deviation_Max_OpnEnd;              /*1B160053 开模位置最大偏差值*/
	UI16  Qc_Unq_Cnt_OpnEnd;                   	/*1B160054 开模位置不合格数*/
	UI16  Qc_Unq_Rate_OpnEnd;                   /*1B160055 开模位置不合格率*/
	UI16  Qc_Max_OpnEnd;                        /*1B160056 开模位置最大值*/
	UI16  Qc_Min_OpnEnd;	                    /*1B160057 开模位置最小值*/		

    UI16  Qc_Avg_OpnTm;                         /*1B160058 开模时间平均值*/
    UI16  Qc_Deviation_Max_OpnTm;               /*1B160059 开模时间最大偏差值*/
	UI16  Qc_Unq_Cnt_OpnTm;                   	/*1B16005A 开模时间不合格数*/
	UI16  Qc_Unq_Rate_OpnTm;                    /*1B16005B 开模时间不合格率*/
	UI16  Qc_Max_OpnTm;                         /*1B16005C开模时间最大值*/
	UI16  Qc_Min_OpnTm;	                        /*1B16005D开模时间最小值*/			

    UI16  Qc_Avg_OpnSpd;                        /*1B16005E开模速度平均值*/
    UI16  Qc_Deviation_Max_OpnSpd;              /*1B16005F开模速度最大偏差值*/
	UI16  Qc_Unq_Cnt_OpnSpd;                   	/*1B160060开模速度不合格数*/
	UI16  Qc_Unq_Rate_OpnSpd;                   /*1B160061开模速度不合格率*/
	UI16  Qc_Max_OpnSpd;                        /*1B160062开模速度最大值*/
	UI16  Qc_Min_OpnSpd;	                    /*1B160063开模速度最小值*/	

	   /*        QC监控参数           2014-07-06*/
    UI16  Qc_Avg_EjtTm;                        /*1B160064 顶针时间平均值*/
    UI16  Qc_Deviation_Max_EjtTm;              /*1B160065 顶针时间最大偏差值*/
	UI16  Qc_Unq_Cnt_EjtTm;                    /*1B160066 顶针时间不合格数*/
	UI16  Qc_Unq_Rate_EjtTm;                   /*1B160067 顶针时间不合格率*/
	UI16  Qc_Max_EjtTm;                        /*1B160068 顶针时间最大值*/
	UI16  Qc_Min_EjtTm;	                       /*1B160069 顶针时间最小值*/
}QCMONITOR_STATE, *PQCMONITOR_STATE;

#pragma pack()

#ifdef DIC_ENUM_NEED

#define		ER0_QCPARACHK	0x0001		//		;检测模数已到参数统计中


#endif

#ifdef __cplusplus
}
#endif

#endif






