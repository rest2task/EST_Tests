/*===========================================================================+
|  System   : Move control                                                   |
|  Task     : Move control header file                                       |
|----------------------------------------------------------------------------|
|  Compile  : CC' C2000 -                                                    |
|  Link     : CC' C2000 -                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Xiaqinghua                                                     |
|  Version  : V1.00                                                          |
|  Creation : 09/13/2008                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef     D__MOVECTRL
#define     D__MOVECTRL
#include    "common.h"     
#include 	"ioproc.h"
/*===========================================================================+
|           const definition                                                 |
+===========================================================================*/
#define     PRESSDOWN       50      // 压力下降基本值
#define     PRESSUP         20      // 压力上升基本值           
#define     PRESSUPPID      50      // 压力上升基本值  
#define     MAXSPEEDSERVO   10000   // 驱动器最高速度值         
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef struct tag_MOVESTRU         // 运动控制数据结构
  {                                 // 以下速度都指电机转速，以0-99表示
    int            SpeedSet;        // 速度设定值，即可使用最大速度
    int            SpeedLast;       // 上次速度   
    int            SpeedCurrent;    // 当前速度
    WORD           SpeedRampOK;     // 1 斜率结束标记
    WORD           SpeedRamp;       // 速度上升/下降斜率  用1ms单位时间
    WORD           SpeedRampbit;       // 速度上升/下降斜率  用1ms单位时间
    int            SpeedCurrent100b;    //
    
    int            PressSet;        // 压力设定值，即须控制的额定压力
    int            PressLast;       // 上次压力                         
    int            PressCurrent;    // 当前压力
    WORD           PressRampOK;     // 1 斜率结束标记
    WORD           PressRamp;       // 压力上升/下降斜率  用1ms单位时间
    WORD           PressRampbit;       // 压力上升/下降斜率  用1ms单位时间
    int            PressCurrent100b;    //
	    
    DWORD          MoveStart100us;    // 运动启动时间
    WORD           MoveStart10ms;   // 运动启动时间
    WORD           MoveStart100ms;  // 运动启动时间 用于显示100MS单位 
    int            MoveEndPos;      // 当前运动终点位置   0.1mm
    WORD           MoveTime1ms;     // 工作时间
    
    WORD           HydrDlyTime;     // 液压延迟时间       1 ms
    WORD           HydrPortNo;      // 液压通道编号       
    WORD           ServoPumpLmt;    // 伺服系统的泵浦配置     伺服系统中作为伺服通道编号  BIT 0-6 对应 1-7号伺服
    WORD           NormalPumpLmt;   // 定量变量泵的泵浦配置     

    WORD           PIDPara_P;				//PID参数中的参数P，比例增益
    WORD           PIDPara_I;				//PID参数中的参数I，积分器
    WORD           PIDPara_D;				//PID参数中的参数D，微分器
    
    WORD           MoveStatus;      // 运动状态 0 未工作 1 液压启动
    WORD           KiClear;         // 积分清除状态  0 不清积分  1 清积分

	WORD			mode;	//控制模式
	WORD            Direction_P;	//DA的压力输出方向  0   与标准一致   1    正值    2    负值
	WORD            Direction_F;	//DA的速度输出方向  0   与标准一致   1    正值    2    负值
	
    WORD			presflag;	//压力标记	原td
 } MOVE_STRU;	

typedef struct tag_MOVE_SET_STRU         // 运动控制数据结构
  {                                 
	WORD			CtrlMode;		//控制模式
	
	// 以下速度都指电机转速，以0-99表示
    int            SpeedSet;        // 速度设定值，即可使用最大速度
    WORD           SpeedRamp;       // 速度上升/下降斜率  用1ms单位时间
    
    int            PressSet;        // 压力设定值，即须控制的额定压力
    WORD           PressRamp;       // 压力上升/下降斜率  用1ms单位时间
        
    WORD           HydrDlyTime;     // 液压延迟时间       1 ms
    WORD           HydrPortNo;      // 液压通道编号       
    WORD           ServoPumpLmt;    // 伺服系统的泵浦配置     伺服系统中作为伺服通道编号  BIT 0-6 对应 1-7号伺服
    WORD           NormalPumpLmt;   // 定量变量泵的泵浦配置     

    WORD           PIDPara_P;				//PID参数中的参数P，比例增益
    WORD           PIDPara_I;				//PID参数中的参数I，积分器
    WORD           PIDPara_D;				//PID参数中的参数D，微分器
    
    WORD           KiClear;         // 积分清除状态  0 不清积分  1 清积分
    WORD		   speed_tretching_hide;	// 速度拉伸 隐藏	单位%	1~150%	0表示不使用

	
    WORD			presflag;	//压力标记	原td
  } MOVE_SET_STRU;	
   
typedef struct tag_POSTM
  {
    int            pos;   
    WORD           tm;
  } SPD_POSTM;	

typedef struct tag_SPDSTRU
  {
    int            cncurrent;
    BOOL           fspeed;
    float          speed;   
    SPD_POSTM      postm[20]; 
  } SPD_STRU;	

 typedef struct tag_MOVEDISPATCHSTRU         // 运动控制数据结构
 {
 	MOVE_SET_STRU    List[5];         // 保存参数列表结构体 总的有5个对应于5个优先级
 	WORD             Priority;        // 优先级0(对应于端口液压关闭即“0压控制”),1,2,3,4(动作液压控制) 
 	WORD             Fuction[5];      // 动作参数列别状态  0：无参数   1：参数设定  2：参数传送到端口运算
 	WORD             PortNo;          // 液压端口号 
 }MOVEDPCH_STRU;	
  
/*===========================================================================+
|           extern                                                           |
+===========================================================================*/
extern      MOVE_STRU       g_NullUnit;
extern      MOVE_STRU       p_WorkStru[5];
extern      MOVEDPCH_STRU   g_Dispatch[5];

void   SetMoveUnitData(WORD portno,WORD Priority);
void   SetMoveUnitDataList(WORD mode,WORD pres, WORD flow,WORD pramp,WORD framp,WORD prampstart,WORD frampstart,WORD hydrdly,
												WORD hydrport,WORD ServoPumpLmt,WORD NormalPumpLmt,WORD Servo_P,WORD Servo_I,WORD PresFlag,WORD KiClear,WORD Priority,WORD Speed_tretching_hide);
void   SetMoveUnitData_Da_pres(WORD mode,WORD pres, WORD pramp,WORD prampstart,WORD hydrdly,
												WORD hydrport,WORD NormalPumpLmt,WORD PresFlag,WORD KiClear,WORD Priority,WORD Direct,WORD Speed_tretching_hide);

void   SetMoveUnitData_Da_flow(WORD mode,WORD flow,WORD framp,WORD frampstart,WORD hydrdly,
												WORD hydrport,WORD NormalPumpLmt,WORD PresFlag,WORD KiClear,WORD Priority,WORD Direct,WORD Speed_tretching_hide);

/*     运动位置定位控制函数                                                     */
WORD   MoveLocationMeterControlProc(int curposn,int endposn);
/*     运动时间定位控制函数     100us精度                                        */
WORD   MoveLocationTM100UsControlProc(DWORD worktime, DWORD start100us);
/*     运动时间定位控制函数     1ms精度                                        */
WORD   MoveLocationTMMsControlProc(DWORD worktime, DWORD start_ms);
/*     运动时间定位控制函数     10ms精度                                        */
WORD   MoveLocationTM10msControlProc(WORD worktime, WORD port);
/*     运动开关定位控制函数                                                     */
WORD   MoveLocationNMControlProc(WORD endpb);
/*     运动记数定位控制函数                                                     */
//      g_pbcnt.cnt = 0;                   need to be initial
//      g_pbcnt.cursts = testpb(endpb);
WORD   MoveLocationCntControlProc(PPINT_ST pst,WORD pbcnt);

int	Minimum(int num1,int num2);

void   SetHydrDisplay(WORD portno,WORD pres, WORD flow);
void   SetHydrDisplay_P(WORD portno,WORD pres);
void   SetHydrDisplay_F(WORD portno,WORD flow);


#define MoveLocationTMControlProc(workms)	MoveLocationTM100UsControlProc(((DWORD)workms)*10, pact->step_100us)
#define MoveLocationTM10msControlProc(workms)	MoveLocationTM100UsControlProc(((DWORD)workms)*100, pact->step_100us)
//#define MoveLocationTM100msControlProc(workms)	MoveLocationTMMsControlProc((DWORD)workms, pact->step_100us/10)

#define MoveLocationNoControlProc()			

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/
#endif
