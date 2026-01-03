#ifndef _AUTOCTRL_DIC_H_
#define _AUTOCTRL_DIC_H_

#ifdef __cplusplus
extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"

#pragma pack(1)

#define MAX_SENSOR_PI_NUM		4

typedef struct EN_AUTOCTRL_PI
{
    /*----------电眼自动----------*/
    UI16  I_PRD_CHK [MAX_SENSOR_PI_NUM];    //0x32110000  ;检出电眼     ; CHECK UNIT POSITION, chg by rch 2012-12-24
}AUTOCTRL_PI, *PAUTOCTRL_PI;


typedef struct EN_AUTOCTRL_MOLDSET
{
    UI16   TM_SENSORDLY;                    /* 0x32130000 电眼检测延时      	默认0.00  范围0.00-99.99   20200219	 */
    UI16   TM_SENSORCHK;                    /* 0x32130001 电眼检测计时      	默认0.00  范围0.00-99.99   20200219   */
    //141107 预留给上位机地址
    UI16   DAPRES_CLPLIMIT;                 /*0x32130002  开关模压力上限             */
    UI16   DAPRES_ADJUSTLIMIT;		        /*0x32130003  调模压力上限               */
    UI16   DAPRES_LIMIT;			       	/*0x32130004  小动作压力上限             */

    UI16   DAFLOW_NOZZLELIMIT;		        /*0x32130005  座台速度上限          fb  2011-11-7     */
    UI16   DAFLOW_ADJUSTLIMIT;		        /*0x32130006  调模速度上限         fb  2011-11-7      */
    UI16   DAFLOW_EJECTLIMIT;			    /*0x32130007  托模速度上限       */
    UI16   DAFLOW_HOLDLIMIT;		 	    /*0x32130008  保压速度上限       */
    UI16   DAPRES_BACKLIMIT;			    /*0x32130009  储料背压压力上限   */
    UI16   DAPRES_CHARGELIMIT;            	/*0x3213000a  储料压力上限              */
    UI16   DAFLOW_CHARGELIMIT;            	/*0x3213000b  储料速度上限              */
    UI16   DAHIPRES_CLPLIMIT;            	/*0x3213000c  关模高压压力上限              */
    UI16   DAHIFLOW_CLPLIMIT;            	/*0x3213000d  关模高压速度上限              */
    //151230温度校正系数
    UI16	DATEMP_ADJUST1;                 /*0x3213000e 温度校正系数1              */
    UI16	DATEMP_ADJUST2;                 /*0x3213000f 温度校正系数2              */
    UI16	DATEMP_ADJUST3;                 /*0x32130010 温度校正系数3              */
    UI16	DATEMP_ADJUST4;                 /*0x32130011 温度校正系数4              */
    UI16	DATEMP_ADJUST5;                 /*0x32130012 温度校正系数5              */
    UI16	DATEMP_ADJUST6;                 /*0x32130013 温度校正系数6              */
    UI16	DATEMP_ADJUST7;                 /*0x32130014 温度校正系数7              */
    UI16	DATEMP_ADJUST8;                 /*0x32130015 温度校正系数8              */
    UI16	DATEMP_ADJUST9;                 /*0x32130016 温度校正系数9              */
    UI16	DATEMP_ADJUST10;            	/*0x32130017 温度校正系数10              */
    UI16	DATEMP_ADJUST11;            	/*0x32130018 温度校正系数11              */
    UI16	DATEMP_ADJUST12;            	/*0x32130019 温度校正系数12              */
    UI16	DATEMP_ADJUST13;            	/*0x3213001a 温度校正系数13              */
    UI16	DATEMP_ADJUST14;            	/*0x3213001b 温度校正系数14              */
    UI16	DATEMP_ADJUST15;            	/*0x3213001c 温度校正系数15              */
    UI16	DATEMP_ADJUST16;            	/*0x3213001d 温度校正系数16              */
    UI16	DATEMP_ADJUSTSET1;            	/*0x3213001e 温度校正设定1              */
    UI16	DATEMP_ADJUSTSET2;            	/*0x3213001f 温度校正设定2              */
    UI16	DATEMP_ADJUSTSET3;            	/*0x32130020 温度校正设定3              */
    UI16	DATEMP_ADJUSTSET4;            	/*0x32130021 温度校正设定4              */
    UI16	DATEMP_ADJUSTSET5;            	/*0x32130022 温度校正设定5              */
    UI16	DATEMP_ADJUSTSET6;            	/*0x32130023 温度校正设定6              */
    UI16	DATEMP_ADJUSTSET7;            	/*0x32130024 温度校正设定7              */
    UI16	DATEMP_ADJUSTSET8;            	/*0x32130025 温度校正设定8              */
    UI16	DATEMP_ADJUSTSET9;            	/*0x32130026 温度校正设定9              */
    UI16	DATEMP_ADJUSTSET10;            	/*0x32130027 温度校正设定10              */
    UI16	DATEMP_ADJUSTSET11;            	/*0x32130028 温度校正设定11              */
    UI16	DATEMP_ADJUSTSET12;            	/*0x32130029 温度校正设定12              */
    UI16	DATEMP_ADJUSTSET13;            	/*0x3213002a 温度校正设定13              */
    UI16	DATEMP_ADJUSTSET14;            	/*0x3213002b 温度校正设定14              */
    UI16	DATEMP_ADJUSTSET15;            	/*0x3213002c 温度校正设定15              */
    UI16	DATEMP_ADJUSTSET16;            	/*0x3213002d 温度校正设定16              */
    UI16	DALOWPRES_CLPLIMIT;            	/*0x3213002e 关模低压压力上限              */
    UI16	DAFLOW_CLPLIMIT;            	/*0x3213002f  开关模速度上限              */
    UI16	DAPRES_UNDERLIMIT;            	/*0x32130030 下压缸压力上限              */
    UI16	DAFLOW_UNDERLIMIT;            	/*0x32130031 下压缸速度上限              */
    UI16	DALOWFLOW_CLPLIMIT;            	/*0x32130032 关模低压速度上限              */
    UI16    DAPRES_EJECTLIMIT;			    /*0x32130033 托模压力上限       */
    UI16    DAFLOW_SUCKBACKLIMIT;          /*0x32130034 射退速度上限 默认20  20170706 LET*/
    UI16    DAPROPORTION;                    /*0x32130035 比重 范围（0-2.0） xiandai */
    UI16    SCREWDIAMETER;                   /*0x32130036 螺杆直径 范围（0-3000mm） xiandai */
    UI16    ADPOSI_ADJTARG;                  /*0x32130037 调模更换位置 */
    UI16    TM_AGINGMONITOR;                 /*0x32130038 面板老炼时间 */
    //压力上限  //20180409 LET 
    UI16   DAPRES_INJECTLIMIT;               /*0x32130039    射出压力上限    */
    UI16   DAPRES_SUCKBACKLIMIT;             /*0x3213003A    射退压力上限    */
    UI16   DAPRES_CORELIMIT;                 /*0x3213003B    中子压力上限    */
    UI16   DAPRES_NOZZLELIMIT;               /*0x3213003C    座台压力上限    */
    UI16   DAPRES_HOLDLIMIT;                 /*0x3213003D    保压压力上限    */
    //速度上限  //20180409 LET 
    UI16   DAFLOW_INJECTLIMIT;               /*0x3213003E    射出速度上限    */
    UI16   DAFLOW_CORELIMIT;                 /*0x3213003F    中子速度上限    */
    //20200221.dxl QT使用  HMI未用
    UI16   IN_PRI_CORE_AD;                  /*0x32130040 中子进优先级ABCD*/
    UI16   IN_PRI_CORE_EH;                  /*0x32130041 中子进优先级EFGH*/
    UI16   OUT_PRI_CORE_AD;                 /*0x32130042 中子退优先级ABCD*/
    UI16   OUT_PRI_CORE_EH;                 /*0x32130043 中子退优先级EFGH*/

    //20200224HJM 模温功能增加
    UI16   MOLDTEMPBOARD_COUNT;             /*0x32130044    模温板数量      *///20200224HJM 模温功能增加
    UI16   SYS_PUSHTABLE_USECAL;            /*0x32130045 推力座使用计算生成表*/
    UI16   SYS_PRS_CLAMP_CURVE;             /*0x32130046 曲线页面的系统压力根据选择关联 -开关模 */ //20200708 CHJ
    UI16   SYS_PRS_CHARGE_CURVE;            /*0x32130047 曲线页面的系统压力根据选择关联 -储料 */
    UI16   SYS_PRS_INJECT_CURVE;            /*0x32130048 曲线页面的系统压力根据选择关联 -射出 */
    UI16   SYS_COREAINPOSMODE;              /*0x32130049 中子A进位置模式*/
    UI16   SYS_COREBINPOSMODE;              /*0x3213004A 中子B进位置模式*/
    UI16   SYS_CORECINPOSMODE;              /*0x3213004B 中子C进位置模式*/
    UI16   SYS_COREDINPOSMODE;              /*0x3213004C 中子D进位置模式*/
    UI16   SYS_COREEINPOSMODE;              /*0x3213004D 中子E进位置模式*/
    UI16   SYS_COREFINPOSMODE;              /*0x3213004E 中子F进位置模式*/
    UI16   SYS_COREAOUTPOSMODE;             /*0x3213004F 中子A退位置模式*/
    UI16   SYS_COREBOUTPOSMODE;             /*0x32130050 中子B退位置模式*/
    UI16   SYS_CORECOUTPOSMODE;             /*0x32130051 中子C退位置模式*/
    UI16   SYS_COREDOUTPOSMODE;             /*0x32130052 中子D退位置模式*/
    UI16   SYS_COREEOUTPOSMODE;             /*0x32130053 中子E退位置模式*/
    UI16   SYS_COREFOUTPOSMODE;             /*0x32130054 中子F退位置模式*/
    UI16   KEYSEL_SPUREVLV;            	  /*0x32130055 胶口针阀按钮切换              */
//20200728.dxl
    UI16   SYS_SERVO_KP_INJCT_HIGH;         /*0x32130056 注射动作的PID_KP_HIGH*/
    UI16   SYS_SERVO_KI_INJCT_HIGH;         /*0x32130057 注射动作的PID_KI_HIGH*/
    UI16   SYS_SERVO_KP_INJCT_MID;          /*0x32130058 注射动作的PID_KP_MID*/
    UI16   SYS_SERVO_KI_INJCT_MID;          /*0x32130059 注射动作的PID_KI_MID*/
    UI16   SYS_SERVO_KP_INJCT_LOW;          /*0x3213005A 注射动作的PID_KP_LOW*/
    UI16   SYS_SERVO_KI_INJCT_LOW;          /*0x3213005B 注射动作的PID_KI_LOW*/
    UI16   SYS_SERVO_KP_HOLD_HIGH;         /*0x3213005C 保压动作的PID_KP_HIGH*/
    UI16   SYS_SERVO_KI_HOLD_HIGH;         /*0x3213005D 保压动作的PID_KI_HIGH*/
    UI16   SYS_SERVO_KP_HOLD_MID;          /*0x3213005E 保压动作的PID_KP_MID*/
    UI16   SYS_SERVO_KI_HOLD_MID;          /*0x3213005F 保压动作的PID_KI_MID*/
    UI16   SYS_SERVO_KP_HOLD_LOW;          /*0x32130060 保压动作的PID_KP_LOW*/
    UI16   SYS_SERVO_KI_HOLD_LOW;          /*0x32130061 保压动作的PID_KI_LOW*/
    UI16   SYS_SERVO_KP_INJCT_MODE;        /*0x32130062 注射动作的PID模式0-高 1-中 2-低*/
    UI16   SYS_SERVO_KP_HOLD_MODE;         /*0x32130063 保压动作的PID模式0-高 1-中 2-低*/
    UI16   KEY_LUBR2_INDEPEND;             /*0x32130064 润滑2按键独立*///20200921.dxl
    //20200909.dxl Ecat曲线相关设定值变量
    UI16   ECAT_CURVE_SELECT1;             /*0x32130065 曲线显示勾选1 */
    UI16   ECAT_CURVE_SELECT2;             /*0x32130066 曲线显示勾选2 */
    UI16   ECAT_CURVE_SELECT3;             /*0x32130067 曲线显示勾选3 */
    UI16   ECAT_CURVE_SELECT4;             /*0x32130068 曲线显示勾选4 */
    UI16   ECAT_CURVE_MAXSET1;             /*0x32130069 曲线1最大值 */
    UI16   ECAT_CURVE_MAXSET2;             /*0x3213006A 曲线2最大值 */
    UI16   ECAT_CURVE_MAXSET3;             /*0x3213006B 曲线3最大值 */
    UI16   ECAT_CURVE_MAXSET4;             /*0x3213006C 曲线4最大值 */
    UI16   ECAT_CURVE_MAXFLAG1;            /*0x3213006D 曲线1最大值正负标志 */
    UI16   ECAT_CURVE_MAXFLAG2;            /*0x3213006E 曲线2最大值正负标志 */
    UI16   ECAT_CURVE_MAXFLAG3;            /*0x3213006F 曲线3最大值正负标志 */
    UI16   ECAT_CURVE_MAXFLAG4;            /*0x32130070 曲线4最大值正负标志 */
    UI16   ECAT_CURVE_MINSET1;             /*0x32130071 曲线1最小值 */
    UI16   ECAT_CURVE_MINSET2;             /*0x32130072 曲线2最小值 */
    UI16   ECAT_CURVE_MINSET3;             /*0x32130073 曲线3最小值 */
    UI16   ECAT_CURVE_MINSET4;             /*0x32130074 曲线4最小值 */
    UI16   ECAT_CURVE_MINFLAG1;            /*0x32130075 曲线1最小值正负标志 */
    UI16   ECAT_CURVE_MINFLAG2;            /*0x32130076 曲线2最小值正负标志 */
    UI16   ECAT_CURVE_MINFLAG3;            /*0x32130077 曲线3最小值正负标志 */
    UI16   ECAT_CURVE_MINFLAG4;            /*0x32130078 曲线4最小值正负标志 */
    UI16   ECAT_CURVE_KEEPTM;              /*0x32130079 曲线持续时间 */
    UI16   ECAT_CURVE_PERIOD;              /*0x3213007A 曲线采样精度 */
    UI16   ECAT_CURVE_SERVOID;             /*0x3213007B 驱动器编号 */
    UI16   ECAT_CURVE_COLLECTMOD;          /*0x3213007C 曲线采集方式 */
    UI16   ECAT_CURVE_START;               /*0x3213007D 曲线采集启停 */
    UI16   TM_PREHEAT_LIMIT;               /*0x3213007E 预温计时下限设置 */
}AUTOCTRL_MOLDSET, *PAUTOCTRL_MOLDSET;
//NET_FUNC
typedef struct EN_AUTOCTRL_STATE
{
    UI16  FL_ERROR0;						//0x32160000 警报1
    UI16  FL_ERROR1;						//0x32160001 警报2
    UI16  FL_AUTOCTRL_STATE_IOT_ACT1;               //0x32160002 动作状态1
    UI16  FL_AUTOCTRL_STATE_IOT_ACT2;               //0x32160003 动作状态2
    UI16  FL_AUTOCTRL_STATE_IOT_ACT3;               //0x32160004 动作状态3
    UI16  FL_AUTOCTRL_STATE_IOT_WARN1_H;            //0x32160005 警报状态1_H
    UI16  FL_AUTOCTRL_STATE_IOT_WARN1_L;            //0x32160006 警报状态1_L
    UI16  FL_AUTOCTRL_STATE_IOT_WARN2_H;            //0x32160007 警报状态2_H
    UI16  FL_AUTOCTRL_STATE_IOT_WARN2_L;            //0x32160008 警报状态2_L
    UI16  FL_AUTOCTRL_STATE_IOT_WARN3_H;            //0x32160009 警报状态3_H
    UI16  FL_AUTOCTRL_STATE_IOT_WARN3_L;            //0x3216000A 警报状态3_L
    UI16  FL_AUTOCTRL_STATE_IOT_TOTALCNT_H;         //0x3216000B 累计模数_H
    UI16  FL_AUTOCTRL_STATE_IOT_TOTALCNT_L;         //0x3216000C 累计模数_L
    UI16  FL_AUTOCTRL_STATE_IOT_TOTAL_TIME_H;       //0x3216000D 历史累计时间 high 16   //2019.5.8 csj
    UI16  FL_AUTOCTRL_STATE_IOT_TOTAL_TIME_L;       //0x3216000E 历史累计时间 low 16
    UI16  FL_AUTOCTRL_STATE_IOT_START_TIME_H;       //0x3216000F 本次开机时间 high 16
    UI16  FL_AUTOCTRL_STATE_IOT_START_TIME_L;       //0x32160010 本次开机时间 low 16
}AUTOCTRL_STATE, *PAUTOCTRL_STATE;

#pragma pack()

#ifdef DIC_ENUM_NEED

enum EN_AUTOCTRL_ACT_SN
{
    OS_SENSORCHK = 1,			// 0   ;电眼检测
    AUTOCTRL_ACT_SN_END
};


#define     ER0_DROP      0x0001            //#32100   	落料电眼检出异常    
#define     ER0_DRFUL     0x0002            //#32101 	电眼检测物料已满

#endif

#ifdef __cplusplus
}
#endif

#endif

