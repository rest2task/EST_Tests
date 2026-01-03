#ifndef _HYDR_H_
#define _HYDR_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include	"part.h"
#include	"hydr_dic.h"
#include	"acttype.h"

#define     HYDR_FLOW_MAX		99
#define     DA_MAX              0x0FFF
#define     DA_PRESSYSLIMIT     140
#define     MAXPRESSTABLENO     21          //压力线性表最大编号
#define     MAX_PUMP            12
#define     DA_LINEMAX          15
#define     DA_FLOW_LINEMAX     11
#define     DA_LINEMAXNEW       12
#define     HYDRDELY            gethydelay(hydelay) //1ms  04-9-15      2 old
#define		DEF_HYDELAY			20				//缺省液压延迟 20ms
#define		DALINE_INTERVAL		35			//多段DA曲线间隔距离

#define     ACCURACY        1000
  
#define     FLWLMT          999
#define     FLOWACCURACY    (ACCURACY/10)


#define     MODEL_A310	 0x0A310
#define     MODEL_A610	 0x0A610
#define     MODEL_A710	 0x0A710
#define     MODEL_A900	 0x0A900

#define 	PIDCALTM        MAX_LOOP_100US*3
#define		OILPORTTB_NUM	25
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef struct tag_HYDRIC
  {
    WORD        prestbl[DA_LINEMAX]   ;        //压力线性表
    WORD        flowtbl[DA_FLOW_LINEMAX]   ;   //流量线性表
    UI32        pressettbl[DA_LINEMAX];        //压力调整设定值
	UI32        flowsettbl[DA_FLOW_LINEMAX];   //流量调整设定值
    WORD        limit                 ;        //压力上限
    int         Press                 ;        //当前压力   0通道为无效通道，为无液压部件预留，使用时将通道设为0 09-6-19
    int         Flow                  ;        //当前流量   
    WORD        rampstart             ;        //流量斜率启动时间
    WORD        prampstart          ;          //压力斜率启动时间
    BOOL        rampok              ;          //斜率完成标记
    WORD        FlDAAdjust          ;          //标记进入DA调整画面 0：未进入 1：DA调整状态 2:退出画面
 	BOOL	    F2DAMAX				;		       //1:调整最大值  0:调整曲线表

	WORD		pumplist			;			
  }  HYDRIC_STRU;  
  
typedef struct tag_PUMPLMT
  {      
    WORD        clszsp;                        //关模装模
    WORD        clssfst;                       //关模快速
    WORD        clsshi;                        //关模高压
    WORD        clsslow;                       //关模低压
    WORD        clssslow;                      //关模慢速
    WORD        opnzsp;                        //开模装模
    WORD        opns1st;                       //开模一快
    WORD        opnsfst;                       //开模二快
    WORD        opns2nd;                       //开模终止
    WORD        injzsp;                        //射出装模
    WORD        injs1st;                       //射出
    WORD        injhs1st;                      //保压
    WORD        chrsp;                         //储料
    WORD        sucsp;                         //射退
    WORD        ejfsp;                         //顶针进
    WORD        nozfsp;                        //座台
    WORD        cp1sp;                         //中子
    WORD        adjsp;                         //调模
    WORD        sdrsp;                         //安全门
    WORD        nmvsp;                         //备用
    WORD        Sclp;                          //滑模
    WORD        Loc;                           //定位
    WORD        ejfbsp;                        //顶针退
  }  PUMPLMT_STRU;
  
/* 多比例系统通道分类结构 07-5-8 */      
typedef struct tag_MULTIHYDR  
  {      
    WORD        Clamp;          //开关模通道
    WORD        ClampHi;        //关模高压
    WORD        Sluice;         //闸板
    WORD        Nozzle;         //座台
    WORD        ShutOff;        //液压喷嘴
    WORD        Inject;         //注射
    WORD        Charge;         //储料
    WORD        ChargeBack;     //储料背压
    WORD        SuckBack;       //射退 
    WORD        Release;        //泄压
    WORD        CorePull;       //中子 
    WORD        Eject;          //脱模
    WORD        Safedoor;       //安全门
    WORD        Adjust;         //调模
    WORD        SysAcc;         //系统增压
    WORD        ChgPrs;         //储料加压 
	WORD        DPump;          //二组变量泵通道    
    WORD        Sclp;	        //滑模
    WORD        Loc;	        //定位
    WORD        EjectB;         //脱模B
  }  MULTIHYDR_STRU;
  
typedef struct tag_PUMPSTRU
  {
    WORD        currpump[40];
    WORD        maxpump;  
    WORD        flowlmt;  
    WORD        flowlmtServo[8];      	
  }  PUMP_STRU; 
/***************************   模拟量输出部分PID控制  *********************************/
typedef struct tag_POSTIONPIDSTRU_DA
{
	UI16				p_da;						//	比例增益
	UI16				ki_da;						//	积分器
	UI16				kd_da;						//	微分器
	UI16				ti_da;						//	积分参数
	UI16				td_da;						//	微分参数
}POSPID_STRU_DA;


typedef struct tag_POSTIONSTU_DA
{
	UI32				pk_da;						//	PID调节器输出量
	UI32				pk0_da;					//	PID调节器上一次的输出量
	int				ek_da;						// 	位置差值	= 设定位置 - 实际位置
	int				ek0_da;					// 	上一次的位置差值
	int				ek00_da;					// 	再上一次的位置差值
	long int				ik_da;						// 	积分累加器  
}POS_STRU_DA;

typedef struct tag_SRAMP_DA
{
	UI16	  			accelerationstep_da;		// 	加速步幅限制
	UI16	  			decelerationstep_da;		// 	减速步幅限制
	UI16	  			speedmax_da;				// 	最大输出
	UI16	  			speedmin_da;				// 	最大输出	
	long int            speedreal_da;				// 	计算出的实际输出
	long int		    speedlast_da;				// 	上次的输出
	UI16				posset_da;					// 	目标位置
	UI16				posreal_da;				// 	实际位置
	
	UI16	  			caltm_da;					// 	PID计算时间周期
	UI16	  			tmflag_da;					// 	时间标记	    
	POSPID_STRU_DA       	pidpara_da;				// 	PID系数
	POS_STRU_DA          	posdata_da;				// 	位置PID计算结构体 
}   SRAMP_STRU_DA;

/*---------------------------------------------------------------------------+
|           Statistics                                                       |
+---------------------------------------------------------------------------*/
/*===========================================================================+
|           Globol                                                           |
+===========================================================================*/
extern      HYDRIC_STRU      g_hydric[MAXDAPORT];
extern      MULTIHYDR_STRU   g_MultHydr;
extern      PUMPLMT_STRU     g_pumplmt;
extern      PUMP_STRU        g_pump;
extern 		SRAMP_STRU_DA	 g_sramp_da;        //模拟量控制PID输出 

//extern      TASK_STRU        g_AutoTestPressLineTask;
//extern      TASK_STRU        g_CanServoWriteTask,g_CanServoReadTask;
/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/
void        getmmiprestbl(int);
void        getmmiflowtbl(int);
void        sendhydrlinetommi(void);
void        SendPumpTblToMmi(void);
void        setselfdefinehydrtbl(void);
void        hydraupresout(WORD portno,UI32 pres,WORD Direct);
void        hydraupresoutTY(WORD portno,UI32 pres,UI16 flow_set,WORD Direct);

void        HydrLoadDACur(void);
/*flow 放大了FLOWACCURACY倍，flow_set没有被放大 */
void        hydrauflowout(WORD portno,UI32 flow, UI32 flow_set,WORD Direct);

void        inithydrauport(void);	
void        hydroutramp(WORD portno,WORD pres,WORD flow,WORD pramp,WORD framp);
void        LoopHydrauic(void);
void        getcurrentpump(void);
void        pumpout(WORD flow);
void        InitHydrData(void);
void        setda5722out(int portno,WORD value);  
void        InitServoPIDPara(WORD portno);   //PID参数初始化
void        setdaout(int portno,WORD value,WORD Direction);
void    	SetPressData(WORD portno,UI32 pres,WORD Direction);

/*flow 放大了FLOWACCURACY倍，flow_set没有被放大 */
void    	SetFlowData(WORD portno,UI32 flow,WORD nomlmt,WORD srvlmt, UI32 flow_set,WORD Direction);

void		CloseHydrOut(WORD portno,WORD Priority,WORD pramp,WORD framp);
void		CloseDaOut_Pres(WORD portno,WORD Priority,WORD pramp,WORD dire);	//关闭模拟的P输出
void		CloseDaOut_Flow(WORD portno,WORD Priority,WORD framp,WORD dire);	//关闭模拟的F输出


void        getdefaulthydrtbl(void)   ;
BOOL 		InitHydrPart(void);
void		ClearAllHydrOut(UI16 portno);
void		ClearAllHydrOut2(UI16 portno);

void   		SetPressHydrOutData(WORD portno,WORD presad, WORD flow);
void		SetFlowHydrOutData(WORD portno,WORD pres, WORD flowad);
void		SetHydrOutData(WORD portno,WORD pres, WORD flow);
int 		SetFactoryHydrTbl(UI8* pmem);
void 		SetBackReset(void);
void 		SetHydrCurReset(BOOL inlay);
void 		HydrDaCurSave(void);
BOOL 		HydrCmd(UI32 cmd, UI32 para);
UI16        gethydelay(UI16 hydelay);


void    inithydlimtport(void);
void    AdjustHydrDisplay(WORD Adjbit);

UI16 	OpnSpcProc(UI16 targetpos,UI16 upramp,UI16 downramp,UI16 targetflow,UI16 protectflow,int admeter);//2014-07-29
void 	limttoline(void);
void 	chgbackpresset(UI16 lastportno,UI16 chgportno,UI16 chgpres);
void    inithydatacomisok(void);
WORD    getflowdavalue(UI32 flow,WORD portno);
WORD    getpresdavalue(UI32 pres,WORD preslmt, WORD portno);
void        getdefaulthydrtblLine(WORD portno,WORD Line) ;


void    HydrLoadDACur(void);

void     DA_OUTPUMP_Clear(void);

void     PospidProc_DA(UI16 pres,UI16 flow,UI16 posset,UI16 posreal,UI16 port,UI16 pramp,UI16 Dramp,UI16 pressmin,UI16 flowmin);  //PID控制模拟量输出
void  	 PosParaInit_DA(UI16 pidnum,UI16 posp,UI16 posi,UI16 posd);         //位置PID参数初始化
UI16    PospidProc_DASPC(UI16 pres,UI16 flow,UI16 posset,UI16 posreal,UI16 port,UI16 pramp);//位置控制程序
void     AdposServoRampDownUp(UI16 startpres,UI16 startflow,UI16 endpres,UI16 endflow,UI16 startpos,UI16 endpos,UI16 realpos);

//WORD        GetPressDAValueNew(float pres,WORD portno);
//void        AutoTestPressLineTaskInitail();  
//void        WritePressTableToEEprom(WORD    LineOfs);

typedef struct tyHYDR
{
	PART				part;
	
	PHYDR_PO			po;
	PHYDR_MACHSET		mh;
	PHYDR_MOLDSET		md;
	PHYDR_ZDSET			zd;
	PHYDR_STATE			st;

	ACT				    act[HYDR_ACT_SN_END - 1];

	UI32				last_da_can_100us;
	BOOL				press_start;
	UI8					max_hydr_port;
	BOOL                clopnrel;
}HYDR, *PHYDR;

typedef struct tyCUS_PORT_LIST
{
   UI16	part_id;	//部件id
   UI16	act_id;   	//动作id
   UI16	port;		//主油路
   UI16	pump;		//从站配置
   UI16	act_flag;	//射出保压两动作区分
}CUS_PORT_LIST, *PCUS_PORT_LIST;


#define HYDR_INIT_INFO	{{PO_ID, CAL_LEN(HYDR_PO)}, {MH_ID, CAL_LEN(HYDR_MACHSET)}, {MD_ID, CAL_LEN(HYDR_MOLDSET)}, {ZD_ID, CAL_LEN(HYDR_ZDSET)}, {ST_ID, CAL_LEN(HYDR_STATE)}}


#ifdef __cplusplus
}
#endif

#endif
