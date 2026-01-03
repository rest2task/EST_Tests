#ifndef _HYDRSERVO_H_
#define _HYDRSERVO_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include	"part.h"
#include    "servo_dic.h"
#include    "hydr.h"

#define	SERVO_ECAT 1


#define     MAXPRESSSETNUM      8
#define     MAXPRESSREALNUM     2
#define     MAXFLOWSETNUM       8
#define     MAXFLOWREALNUM      2

#define		DECELERATIONSTEP	200	//25ms
#define		ACCELERATESTEP		200	//25ms

#define     ZEROHYDRD           0x0A          //1ms  泄压标记
#define     ZEROHYDRD1          0xAA          //1ms  开模结束泄压处理
#define     ZEROFLAG            0x0B          //泄压后零扭矩输出	09-08-10
#define     INJDFLAG            0x08           //PID D信号 作为射出标记用于伺服
#define     HOLDFLAG            0x10          //PID D信号 作为保压标记用于伺服
#define     COREFLAG            0x11          //PID D信号 作为中子标记用于伺服
#define     NOCHANGELAG         0x60          //PID 参数不变化标记	作为脱模标记用于伺服
#define     BACKPFLAG           0x30          //PID D信号 作为储料背压标记


#define Ecat_PCPID	1

#ifndef Ecat_PCPID
#define	PID_KI_ACC	100		//积分放大倍数
#else
#define	PID_KI_ACC	10		//积分放大倍数
#endif

typedef struct tag_SERVOPIDSTRU
{
  int   p;    //比例增益
  int   ki;   //积分器
  int   kd;   //微分器
  int	ti;   //积分参数
  int	td;   //微分参数
}SERVOPID_STRU;


typedef struct tag_PRESSSTU
{
  long int		pk;		                 // PID调节器输出量
  long int		pk0;                   // PID调节器上一次的输出量
  int		ek;		                       // 压力差值	= 设定压力 - 实际压力
  int		ek0;	                       // 上一次的压力差值
  int		ek00;                        // 再上一次的压力差值
  long int		ik;                    // 积分累加器
  long int		ik0;
}PRESSPWR_STRU;

typedef struct tag_DATARECORD
{
  int           wData;               // 记录数据
  WORD          tmRecord;            // 记录时间
}DATARECORD_STRU;

typedef struct tyPUMP *PPUMP;

typedef struct tag_PRESSPOWER
{
  WORD				runstate;		//运行状态	0:不使用，1:被占用
  	
  SERVOPID_STRU	   pidpara;
  PRESSPWR_STRU	   presspwr;

  WORD             CycleTime;        //	PID控制周期
  int              LastPress;        // 上级动作设定压力
  int              FlowOutMax;       // 最大输出速度  
  int              FlowOut;          // 实际输出速度 
  int              PressOut;         // 实际输出扭距 
  int              CntZeroSpeed;     // 0输入速度数
  DATARECORD_STRU  PressSet[MAXPRESSSETNUM];     // 输入压力记录表
  DATARECORD_STRU  PressReal[MAXPRESSREALNUM];   // 实际压力记录表
  DATARECORD_STRU  FlowSet[MAXFLOWSETNUM];       // 输入速度记录表
  DATARECORD_STRU  FlowReal[MAXFLOWREALNUM];     // 实际速度记录表                
  WORD			   ControlMode;									 //	控制模式	1：单PID控制模式	2：强PID->超调，并回调->弱PID	3：弱PID->超调->强PID->回调->弱PID;弱PID->速度到->强PID->超调，并回调->弱PID
  int			   ReverseFlowMax;							 // 最大反向速度 0~99
  int			   DecelerationStep;						 //	减速步幅 1~50
  int			   AccelerateSetp;						 	//	加速步幅 1~1000

  u32				PressSetF;
  
  UI16		  		PresCtrlValid;  //压力控制生效  压力速度1 指令速度0
  UI16				PresOverCtrl;	//超压控制 0:全过程压力控制	1:开启超压控制 2:超调控制进入压力控制

  UI16				PresSpdLimit;	// 0 不走s 1走s
  
  PPUMP				ppump;
  UI16				presflag;		//压力控制标记	原td
}PRESS_STRU,*PPRESS_STRU;

extern	PRESS_STRU       	g_press[MAXDAPORT];

typedef struct	tySERVO
{
	PART				part;

	PSERVO_PO		    po;
	PSERVO_PI		    pi;
	PSERVO_MACHSET   	mh;
	PSERVO_STATE		st;
	
	UI16                cantestmode; //0:正常控制发送模式  1:can通信测试模式   2:主机开启主动请求发送模式
}SERVO, *PSERVO;


extern  SERVO            g_hydrservo; 

#define SERVO_INIT_INFO	{{PO_ID, CAL_LEN(SERVO_PO)}, {PI_ID, CAL_LEN(SERVO_PI)}, {MH_ID, CAL_LEN(SERVO_MACHSET)}, {ST_ID, CAL_LEN(SERVO_STATE)}}

void	InitServoPIDParaCold(WORD portno);   //冷启动PID参数初始化
void    SetServoPIDPara(WORD p,WORD i,WORD d,WORD portno);
void    SetServoPIDPara_NoInit(WORD p,WORD i,WORD d,WORD portno);
void    SetHydrDisplay(WORD portno,WORD pres, WORD flow);
void    InitCtrlTableCnt(int lastpres,WORD portno);
WORD    GetkpDefault(void);
WORD    GetkiDefault(void);
BOOL 	InitServoPart(void);
UI32 	GetServo3MsTick(void);
void    ServoErr(void);
BOOL    ServoTest(void);
WORD GetkpDefault(void);
WORD GetkiDefault(void);
void ClrNetErr(void);


int GetSpdRPMBySpdPulse(WORD ServoNo,int ActSpdPulse);


#ifdef __cplusplus
}
#endif

#endif



