#ifndef _SERVFIVE_H_
#define _SERVFIVE_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include	"part.h"
#include	"Servo_dic.h"
#include	"acttype.h"


/*伺服模式5新增加*/

#define		SERVO_PID_TM		24			// 3ms, 100us单位

#define     RUNSTATE            0x000     //运行监视状态，读取数据字典5021的运行状态值
#define     PARAREAD            0x001     //驱动器数据字典读，通过字典索引读取驱动器的数据字典值
#define     PARAWRITE           0x002     //驱动器运行参数写，写入驱动器运行时可修改的运行参数
#define     PARAPISET           0x003     //动作PI值设定。P和I参数各占10位bit，范围0-1023
#define     CHGCOMPARA          0x004     //驱动器通知。驱动器重要状态变化时通知主机
#define     CMDSRV1             0x005     //保留1
#define     CMDSRV2             0x006     //保留2
#define     CMDSRV3             0x007     //保留3


typedef struct tag_CAN_TxPDOTaskStru
{
    WORD        wLength;                  //
    int         wData[4]; 
}CAN_TxPDOTaskStru;

typedef struct tag_POWERSTU
{
	UI16	powerstart[8];  //单位千焦   动作开始功耗
	UI16	powerend[8];    //单位千焦   动作结束功耗
//	UI16    actid;          //动作编号  高八位为动作ID 低八位为动作开始或者动作结束  动作开始0X01  动作结束0X10
	UI16    readflag[8];    //读取驱动器功耗状态: 0x01表示动作开始功耗读取中,0x04表示动作开始功耗读取完成,0x02表示动作结束功耗读取中,0x08表示动作结束功耗读取完成
}POWER;

typedef struct tag_PUMPSTRU_5
 {
 	UI16                ServoNum;
	UI16                writestep;
 	UI16				write1ms;
	UI16                readstep;
 	UI16				pressshow[2];
 	UI16                cantestmode; //0:正常控制发送模式  1:can通信测试模式   2:主机开启主动请求发送模式
 	CAN_TxPDOTaskStru   ServoPDORead;
	CAN_TxPDOTaskStru	ServoPDOWrite;
	
    WORD        flowlmt_5;  
    WORD        flowlmtServo_5[8]; 
	
	UI32			    F_3msclock;
	UI16        		svrsendcnt[8];
	UI32			  	pres500ms[8];

	UI16                sverrfiltime;
	UI16                sverroutbit;

	UI16                svrflag;      //伺服主机标记位操作 相应位为1表示该号驱动器为主机接收发送的压力流量指令 反之为从机不能发送压力流量指令
	POWER               serpower[8];  //伺服功耗回复   对应8个动作
	UI16                resendtime;

	UI16                getmodetime;
	UI16                getmodecount;
	UI16                getmodebuf[8];

	UI16                servoalarmbuf;
	UI16                alarmclrtime;
	UI32                statetcnt[8];//相应未置1表示对应的状态数据已经请求过了
	UI16                servocanmode[8];//0 : 发送状态请求   1 : 发送数据读取   2 : 发送数据写入  3 : 发送PI设定   后面保留
	UI16                servocomst100us[8];  // 驱动器于主机的通信时间计数  大于200MS没收到数据表示驱动器脱机   
	UI16                servostcoldread;//低八位位操作相应位置1表示开机从机状态读取完成，高八位表示需要去读取的从机号位操作

	UI16				ServoPower;
 }PUMP_STRU_5,*PPUMP_STRU_5;


typedef struct tag_MOVESTRU_5         	// 伺服5模式下的运动控制数据结构
  {                                 	// 以下速度都指电机转速，以0-99表示
    WORD           PIDPara_P_5;			//PID参数中的参数P，比例增益
    WORD           PIDPara_I_5;			//PID参数中的参数I，积分器
    WORD           PIDPara_D_5;			//PID参数中的参数D，微分器
 } MOVE_STRU_5;

extern      PUMP_STRU_5        g_pump_5;
extern 		MOVE_STRU_5		 p_Work_5[4];


void 	DataMovetoRamp(WORD portno,WORD Priority);
void    CanServoReadTask(int i);
void    CanServoWriteTask(WORD portno);
void    CanServoWriteTask_TransCheck(int portno);
void	MoveHydrRampOut_5(void);
void    ServoPIDCtrlProc(void);
BOOL 	ServoModeVerify(void);
void	InitServoCanComm(void);
void     SetPressData_5(WORD portno,UI32 pres);
void    SetFlowData_5(WORD portno,UI32 flow, WORD srvlmt);
void    InitServo_PISet(WORD portno);

#ifdef __cplusplus
}
#endif

#endif
