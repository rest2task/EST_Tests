#ifndef _SERVO_DIC_H_
#define _SERVO_DIC_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"

#pragma pack(1)

#define MAX_SERVO_NUM	8

typedef struct EN_HYDRSERVO_PO
{
	UI16  O_SV_REL ;						//0A100000 伺服泄压
	UI16  O_SV_ERR ;						//0A100001 伺服复位
	UI16  O_SV_CLR ;                        //0A100002 驱动器重置
}SERVO_PO, *PSERVO_PO;

typedef struct EN_HYDRSERVO_PI
{
	UI16  I_SV_ERR ;						// 0A110000 伺服故障
	UI16  I_XX_CHK;                         // 0A110001 相序检知
	UI16  I_PR_ERR;                         //0A110002 外控压力不足
}SERVO_PI, *PSERVO_PI;

typedef struct EN_HYDRSERVO_MACHSET
{
	UI16  KK_PRESSRAMP;                    /* 0A120000 压力RAMP                 范围0~500   200        */
    UI16  KP_DEFAULT;	                   /* 0A120001 缺省KP                   范围0-2500  默认210    */
    UI16  KI_DEFAULT;    		           /* 0A120002 缺省KI                   范围0-200   默认10     */
	UI16  REVERSEFLOWMAX;                  /* 0A120003 最大反向速度             范围1-99    默认10     */
    UI16  DECELERATIONSETP;                /* 0A120004 减速步幅                 范围1-50    默认8      */
    UI16  KP_TRANSITION;                   /* 0A120005 PID转折KP值              范围0-2500  默认210    */
    UI16  KI_TRANSITION;                   /* 0A120006 PID转折KI值              范围0-200   默认10     */
    UI16  INJECTFLOWMIN;                   /* 0A120007 注射时最低转速			*/
    UI16  INJECTPRESBYDMAX;                /* 0A120008 注射时最大超压范围	*/
    UI16  EJECTFLOWMIN;                    /* 0A120009 脱模时最低转速			*/
    UI16  EJECTPRESBYDMAX;                 /* 0A12000A 脱模时最大超压范围	*/

	UI16  KP_INJECTTRANS;                  /* 0A12000B 注射转换KP                */
    UI16  KI_INJECTTRANS;                  /* 0A12000C 注射转换KI                */
    UI16  WSERVO_TC;                       /* 0A12000D 伺服采样周期07-4-10        */
    UI16  WSERVO_KD;                       /* 0A12000E 伺服D系数07-4-10         */
    UI16  WSERVO_KK;                       /* 0A12000F 伺服保留    07-4-10         */
    UI16  WSERVO_K;                        /* 0A120010 伺服P值    07-4-10         */
    UI16  WSERVO_T;                        /* 0A120011 伺服I值    07-4-10         */
    UI16  WSERVO_TT;                       /* 0A120012 伺服D值    07-4-10         */
    UI16  WRITECNT;
    UI16  READCNT;    
    UI16  SRV_PID_TM;                      /* 0A120015 通讯时间 默认=24 范围12, 6*/
    UI16  MOTOR_MAXSPEED[MAX_SERVO_NUM];   /* 0A120016 - 0A12001D 电机最大转速 	默认1800	范围1000-9999*/
    UI16  PUMP_Q[MAX_SERVO_NUM];		   /* 0A12001E -  0A120025	油泵排量   默认60   范围0-500   单位cc/转*/

	UI16  FL_SERVO_CURVE;					/* 0A120026 驱动器FUNC曲线显示	默认0	0:不使用	1:使用	*/
}SERVO_MACHSET, *PSERVO_MACHSET;

typedef struct EN_HYDRSERVO_STATE
{
	UI16  FL_ERROR0;								//0A160000 警报1
	UI16  FL_ERROR1;								//0A160001 警报2
	
	UI16  SERVO_SPEED[MAX_SERVO_NUM];				//0A160002 -  0A160009   伺服驱动实际转速
  UI16  SERVO_FLOWSET[MAX_SERVO_NUM];				//0A16000A -  0A160011   设置到伺服的速度
	UI16  SERVO_ST[MAX_SERVO_NUM];				    //0A160012 -  0A160019   设置到伺服CAN通信状态
	UI16  SERVO_ERR[MAX_SERVO_NUM];                 //0A16001A -  0A160021   伺服警报	   128个警报
	UI16  SERVO_CURRENT[MAX_SERVO_NUM];             //0A160022 -  0A160029   伺服电流	
	UI16  SERVO_POWER[MAX_SERVO_NUM];               //0A16002A -  0A160031   伺服功率
	UI16  SERVO_ACTST[MAX_SERVO_NUM];               //0A160032 -  0A160039   伺服运动状态
	UI16  SERVO_TEMP[MAX_SERVO_NUM];                //0A16003A -  0A160041   驱动器模块温度
	UI16  SERVO_PRESS[MAX_SERVO_NUM];               //0A160042 -  0A160049   驱动器实际压力
    UI16  WRITE_CNT_E505;							//(3200未使用)0A16004A	E505通讯测试写入次数    20190410
    UI16  READ_CNT_E505;							//(3200未使用)0A16004B	E505通讯测试读入次数
    UI16  WRITE_CNT_E502_1;							//(3200未使用)0A16004C	E502_1通讯测定写入次数
    UI16  READ_CNT_E502_1;							//(3200未使用)0A16004D	E502_1通讯测试读入次数
    UI16  WRITE_CNT_E502_2;							//(3200未使用)0A16004E	E502_2通讯测定写入次数
    UI16  READ_CNT_E502_2;							//(3200未使用)0A16004F	E502_2通讯测试读入次数
}SERVO_STATE, *PSERVO_STATE;

#pragma pack()

#ifdef DIC_ENUM_NEED

#define     ER5_SVCTLERR    0x0001      //#0A100；伺服驱动器故障
#define     ER5_SVMODERR    0x0002		//#0A101；主机与驱动器模式不匹配  
#define     ER5_POWERERR    0x0004      //#0A102；总电源相序错误
#define     ER5_PRESSERR    0x0008      //#0A103；外控压力不足              


#define		ER1_NETINSTART		0x00010000	// #0A110；网络初始化启动
#define		ER1_ECATRECFG		0x00020000	// #0A111；重新配置EtherCAT
#define		ER1_ALRAM3			0x00040000	
#define		ER1_ALRAM4			0x00080000
#define		ER1_ALRAM5			0x00100000
#define		ER1_ALRAM6			0x00200000
#define		ER1_ALRAM7			0x00400000
#define		ER1_ALRAM8			0x00800000
#define		ER1_NETSTEP1		0x01000000	// #0A118	网络初始化第1 次连接
#define		ER1_NETSTEP2		0x02000000	// #0A119	网络初始化第2 次连接
#define		ER1_NETSTEP3		0x04000000	// #0A11A	网络初始化第3 次连接
#define		ER1_NETSTEP4		0x08000000	// #0A11B	网络初始化第4 次连接
#define		ER1_NETSTEP5		0x10000000	// #0A11C	网络初始化第5 次连接
#define		ER1_NETSTEP6		0x20000000	// #0A11D	网络初始化第6 次连接
#define		ER1_NETSTEP7		0x40000000	// #0A11E	网络初始化第7 次连接
#define		ER1_NETSTEP8		0x80000000	// #0A11F	网络初始化第8 次连接

#endif

#ifdef __cplusplus
}
#endif

#endif




