#ifndef _MOTOR_DIC_H_
#define _MOTOR_DIC_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"

#pragma pack(1)

typedef struct EN_MOTOR_PO
{
	UI16  O_MTR_STP;                  //0x04100000 马达停
	UI16  O_MTR_STR;                  //0x04100001 马达开
	UI16  O_MTR_Y_D;                  //0x04100002 Y转DELTA
	UI16  O_MTR_STP2;                 //0x04100003 马达2停
	UI16  O_MTR_STR2;                 //0x04100004 马达2开
	UI16  O_MTR_STP3;                 //0x04100005 马达3停
	UI16  O_MTR_STR3;                 //0x04100006 马达3开
	UI16  O_MTR_STP4;                 //0x04100007 马达4停
	UI16  O_MTR_STR4;                 //0x04100008 马达4开
	UI16  O_MOTO_SRV;				  //0x04100009 马达备用			  
	UI16  O_SERVO_EN;				  //0x0410000A 伺服使能
}MOTOR_PO, *PMOTOR_PO;

typedef struct EN_MOTOR_PI
{
	UI16  I_MTROVL;                   //0x04110000 马达过载
	UI16  I_MTRCMPT;                  //0x04110001 马达启动完成
	UI16  I_MTRCMPT2;                 //0x04110002 马达2启动完成
	UI16  I_MTRCMPT3;                 //0x04110003 马达3启动完成
	UI16  I_MTRCMPT4;                 //0x04110004 马达4启动完成
	UI16  I_MOTO_SRV;                 //0x04110005 马达异常
	UI16  I_MOTO_OK;                  //0x04110006 马达启动           20150514           yxh
}MOTOR_PI, *PMOTOR_PI;

typedef struct EN_MOTOR_MACHSET
{
	UI16  MOTOR_TEMPCH;						/* 0x04120000  马达温度通道, 从0开始*/
	UI16  TM_MOTORYSTART;					/* 0x04120001  马达启动保持时间,              范围2.00-10.00     默认3.00s   20200219 */
	UI16  TM_MOTORDELTA;                   	/* 0x04120002  Y->△转换时间,              范围0.00-0.60      默认0.20s    20200219*/
	UI16  TM_HOLDMOTORON;					/* 0x04120003  △启动保持时间,               范围2.00-180.00    默认3.00s    20200219 */
	UI16  FL_MOTORCNT;                    	/* 0x04120004  马达数量,                  范围1-4          默认1      */
	UI16  TM_WARNMOTORON;                	/* 0x04120005  多马达启动超时检测,             范围5.00-50.00     默认10.00s  20200219 */
	UI16  MOTORTEMP_ERRORMAX;				/* 0x04120006  电机温度上限		,           范围50-120       默认70℃ */

	UI16  PUMP_REVERSE_LIMITTM;				/* 0x04120007 电机反转保护时间	默认:5.00s	范围:0.00~20.00s		*/

	UI16  FL_FREETIME_MOTOROFF;				/* 0x04120008 闲置马达关	默认:0		范围:0不使用	1使用		*/
	UI16  TM_FREETIME_MOTOROFF;				/* 0x04120009 闲置时间		默认:0 min	范围:0~60min		*/
    UI16  CURRENT_ALARM_MAX;				/* 0x0412000A 电机电流报警最大值  默认0  范围0-100 */
}MOTOR_MACHSET, *PMOTOR_MACHSET;

typedef struct EN_MOTOR_STATE
{
	UI16  FL_ERROR0;						//0x04160000 警报1
	UI16  FL_ERROR1;						//0x04160001 警报2
	UI16  FL_MOTORSTATUS;                  	//0x04160002 马达状态
	UI16  DATEMP_REALMOTOR;					//0x04160003 电机温度
	UI16  TM_COUNTDOWNMOTOROFF;				//0x04160004 闲置马达倒计时
}MOTOR_STATE, *PMOTOR_STATE;

#pragma pack()

#ifdef DIC_ENUM_NEED


#define     ER0_MTRS		0x0001      //#04100 '马达启动失败   '  
#define     ER0_MOVL		0x0002      //#04101 '马达过载    '
#define     ER0_HTMT		0x0004      //#04102 '马达未启动'  
#define     ER0_MTTMP		0x0008      //#04103 '电机温度过高'  
#define     ER0_MOTORERR	0x0010      //#04104 '马达启停异常'               //20150514           yxh
#define		ER0_ESDO		0X0020		// 106	ECAT_SDO发送超时，使能启动失败 
#define		ER0_ESDO2		0X0040		// 107	ECAT_SDO发送超时，使能关闭失败 
#define		ER0_PS			0x0080		// 108	总电源相序错误
#define		ER1_UNRE		0x0100		// 109	无反馈数据
#define		ER0_SALVNUM		0x0200		// 10A	从站个数不匹配
#define		ER0_NETERR		0x0400		// 10B	ECAT网络配置错误
#define		ER0_SALVERR		0x0800		// 10C	从站配置错误
#define		ER0_TIMEERR		0x1000		// 10D	从站时钟配置未完成
#define		ER0_TIMECFG		0x2000		// 10E	正在配置从站时钟
#define		ER0_WAITMOTOR 	0x4000		// 10F	等待伺服启动
#define		ER0_SLVLOFF 	0x8000		// 110	从站断线
#define		ER1_SERVOERR	0x10000		// 201	伺服警报,请查看
#define 	ER1_MOTOREOUTTM	0x40000		// #04200 "电机反转超时"
#define 	ER1_MOTOREOUTRPM	0x80000		// #04201 "电机反转超速"
#define		ER1_MOTORFREETIME  0x100000		// #04202 "电机闲置关停"
#define		ER1_CURRENTERR  0x200000		// #04203 "电机电流过大"



enum EN_MOTOR_ACT_SN
{
	OS_MOTON = 1,			// 0	; '马达开    '   
    OS_MOTOFF,				// 1	; '马达关    '    //20170505   LET
	OS_MOTOENABLEON,		// 2	; '开使能'
	MOTOR_ACT_SN_END
};

#endif

#ifdef __cplusplus
}
#endif

#endif


