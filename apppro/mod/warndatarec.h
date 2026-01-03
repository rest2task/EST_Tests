#ifndef WARNDATAREC_H
#define WARNDATAREC_H

#include "typedef.h"
#include "obj.h"
#include "mod.h"
#include "prot.h"
#include "database.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define MAX_WARNDATA_SIZE (500+1)

#define MAX_WARNRED         100
#define	MAX_PI_NUM	128
#define	MAX_PO_NUM	128
#define ADMETER_CH_NUM	8
#define ENCODE_CH_NUM	6
#define MAX_SERVO_NUM	8
#define	MAX_DA_NUM	16
#define MAX_TEMP_NUM    16
#define RESERVE1_NUM    10
#define RESERVE2_NUM    16
#define PI_STATUS_NUM		(MAX_PI_NUM%16 > 0 ? MAX_PI_NUM/16 + 1 : MAX_PI_NUM/16)
#define PO_STATUS_NUM		(MAX_PO_NUM%16 > 0 ? MAX_PO_NUM/16 + 1 : MAX_PO_NUM/16)

#define WARNDATA_PAGE PAGE_WARNDATA_REC
#define WARNHELP_PAGE DIALOG_WARNHELP

// 警报时上传数据
enum _OUTPUTTYPE {
    TYPE_NULL,
    TYPE_PRODUCT,
    TYPE_WARN
};
//警报时当前动作信息
typedef struct tagACTPAR
{
    UI16 wAct_Id;			        //当前动作的id
    UI16 wStep;			                //当前动作步
    UI32 wStart_ms;      		        //当前动作运行的时间
    UI16 wReserve[RESERVE1_NUM];                //预留10个数据地址
}ACTPAR;
//版本日期
typedef struct tagVERINFO
{
    //Panel
    UI16 wPanelType;                            //面板画面类别
    UI16 wPanelFactory;                         //面板厂商
    UI16 wPanelVer1;                            //面板画面版本1
    UI16 wPanelVer2;                            //面板画面版本2
    UI16 wPanelDate1;                           //面板日期1
    UI16 wPanelDate2;                           //面板日期2
    UI16 wReservePanel[RESERVE2_NUM];           //预留16个数据地址
    //Host
    UI16 wHostFactory;                          //主机厂商
    UI16 wHostVer1;                             //主机版本1
    UI16 wHostVer2;                             //主机版本2
    UI16 wHostDate1;                            //主机日期1
    UI16 wHostDate2;                            //主机日期2
    UI16 wBoot1;                                //BOOT1
    UI16 wBoot2;                                //BOOT2
    UI16 wHardware;                             //硬件类型
    UI16 wReserveHost[RESERVE2_NUM];            //预留16个数据地址
}VERINFO;
//机器配置参数
typedef struct tagMACHCFG
{
    UI16 wver;					//Ver
    UI16 wCode0;	                        //Code0
    UI16 wCode1;			        //Code1
    UI16 wCode2;			        //Code2
    UI16 wCode3;		                //Code3
    UI16 wCode4;               		        //Code4
    UI16 wCode5;		              //Code5
    UI16 wFactory;			     //厂商代码
    UI16 wCode7;	         	        //Code7
    UI16 wHydraulic;			        //油路控制

    UI16 wVerInfo;                              //版本信息
    UI16 wTemp;			                //温度控制
    UI16 wMeter;			      //位置尺控制
    UI16 wCode11;       		            //Code11
    UI16 wCode12;			            //Code12
    UI16 wSystem;			           //系统代码
    UI16 wSysPrs;			            //系统压力上限
    UI16 wCode16;			        //Code16
    UI16 wRpmCnt;			        //储料RPM孔数
    UI16 wPushCode;			            //推力座型号

    UI16 wCode43;						        //Code43
    UI16 wCode48;						        //Code48
    UI16 wCode49;						        //Code49
    UI16 wCode50;						        //Code50
    UI16 wCode51;						        //Code51
    UI16 wCode52;						        //Code52
    UI16 wCode53;						        //Code53
    UI16 wCode54;						        //Code54
    UI16 wQVR;			               //国标

    UI16 wReserve[16];                //预留16个数据地
}MACHCFG;
typedef struct tagWARNDATARECORD
{
    UI16 wPart;                                 //部件
    UI16 wCurrent_Err0;				//当前警报1
    UI16 wCurrent_Err1;				//当前警报2
    UI16 wServoErr[MAX_SERVO_NUM];              //伺服警报名
    UI16 wPI_Input[PI_STATUS_NUM];	        //输入点状态
    UI16 wPO_Output[PO_STATUS_NUM];		//输出点状态
    UI16 wADPosi_Relative[ADMETER_CH_NUM];	//AD位置相对值
    UI16 wAd_Relative[ADMETER_CH_NUM];	       //实际AD值

    UI16 wEle_SpdOut_RPM[MAX_SERVO_NUM];		//电机输出转速
    UI16 wEle_Real_Spd[MAX_SERVO_NUM];			//电机反馈速度
    UI16 wEle_Real_Torque[MAX_SERVO_NUM]; 		//电机反馈扭矩

    UI16 wHydr_Out_Pre[MAX_SERVO_NUM];			//液压输出压力
    UI16 wHydr_Out_Spd[MAX_SERVO_NUM];			//液压输出流量
    UI16 wHydr_Real_Pre[MAX_SERVO_NUM];			//液压反馈压力
    UI16 wDA_Value[MAX_DA_NUM];			//DA输出值

    UI16 wCn_MoldNum_L;				//开模数低16位
    UI16 wCn_MoldNum_H;				//开模数高16位
    UI16 Num;					//当前的执行的任务数量
    UI16 Item_len;				//每个任务状态的长度，以UI16 计数3个UI16

    UI16 wPort_Adtemp;                          //记录温度扫描的当前通道
    UI16 wAd_value;                             //记录温度的ad值
    UI16 wTemp_lm37;                            //记录当前温度模块的室温
    UI16 wTemp_real[MAX_TEMP_NUM];              //记录温度实际值
    UI16 wReserve1[RESERVE1_NUM];               //预留10个数据的位置
}WARNDATARECORD;

typedef struct tyWDITEM{
    char time[20];
    WARNDATARECORD WarnData;                    //警报io、速度压力等信息
    ACTPAR  Act_Data[4];                        //警报时当前动作信息
    UI16 wADPosi_Symbol[ADMETER_CH_NUM];        //AD位置正负符号：1=正，0=负
    VERINFO VersionInfo;                        //版本信息
    MACHCFG MachCfg;                            //机器配置参数
    UI16 rev[16];
}WDITEM;

typedef struct tyWDHEAD
{
    UI16        front;
    UI16        rear;
}WDHEAD;

typedef struct tyWDRECORD
{
    WDHEAD     wdhead;
    WDITEM     item[MAX_WARNDATA_SIZE];
} WDRECORD,*PWDRECORD;

void ClearWDRec();

void WDRecInit();
BOOL ReadWDRec(WDITEM* item,UI32 index);
BOOL WDRecIsChg();
int RxCmd_WarnData(UI16* pdata);
int RxCmd_MoniDat(UI16* pdata);
UI16 GetRealData();
void SetCurWarnId(UI16 id);
UI16 GetCurWarnId();
BOOL SaveWarnDataExcel(char* tem_str);
void SetWarnDataFlg(BOOL flg);
BOOL GetWarnDataFlg();
#define WD_REC_PATH SAVE_PATH"warndatarec.dat"

#ifdef __cplusplus
}
#endif

#endif // WARNDATAREC_H
