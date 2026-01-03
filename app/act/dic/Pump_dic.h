#ifndef _PUMP_DIC_H_
#define _PUMP_DIC_H_


#include "typedef.h"

#pragma pack(1)
typedef struct EN_PUMP_PO
{
    UI16  HYD_SHUNT_PUMP;               //0x0F110000 P分流阀1
    UI16  HYD_COMB_PUMP;                //0x0F110001 P合流阀1
}PUMP_PO, *PPUMP_PO;


typedef struct  EN_PUMP_MACHSET
{
	UI16	OIL_PUMP_DISPLACEMENT;	/*	0x0F120000	油泵排量	cc/R		*/
	UI16	EFFICIENCY_FACTOR;		/*	0x0F120001	效率系数	*/
	UI16	MOTOR_SPEED_MAX;		/*	0x0F120002	电机最大转速	默认:1700	RPM	范围:0 ~ 65535	*/
	UI16	ENCODERESOLUTION;		/*	0x0F120003	编码器分辨率	默认:20		位	范围:0 ~ 64		*/
	
	UI16	TORQUEMAX;				/*	0x0F120004	扭矩上限				默认:200.0%			范围100.0%~500.0%*/
	UI16	PRESSAD_CH;				/*	0x0F120005	压力传感器AD编号		默认:IV3200:4	范围:1~8*/
	UI16	PRESSAD_TYPE;			/*	0x0F120006	压力传感器类型			默认:0:1~5V	范围:0:1~5V	1:0~10V*/

	UI16	BASEPRES;				/*	0x0F120007	底压	默认:0.5	范围0.0~20.0bar*/
	UI16	BASEFLOW;				/*	0x0F120008	底流	默认:0.5	范围0.0~20.0%*/

	UI16	PRESS_ACC;				/*	0x0F120009	最小压力上升时间 默认:100ms	0~1000ms*/
	UI16	PRESS_DEC;				/*	0x0F12000A	最小压力下降时间 默认:100ms	0~1000ms*/
	UI16	PRESS_ACC_FILTER;		/*	0x0F12000B	压力上升S滤波时间 默认:100ms 0~1000ms*/
	UI16	PRESS_DEC_FILTER;		/*	0x0F12000C	压力下降S滤波时间 默认:100ms 0~1000ms*/
	UI16	SPEED_ACC;				/*	0x0F12000D	最小速度上升时间 默认:100ms	0~1000ms*/
	UI16	SPEED_DEC;				/*	0x0F12000E	最小速度下降时间 默认:100ms	0~1000ms*/
	
	UI16	ECAT_CODE_CHK_H;		/*	0x0F12000F 	ECAT编码校对高位*/
	UI16	ECAT_CODE_CHK_L;		/*	0x0F120010 	ECAT编码校对低位*/
	UI16	REVERSEFLOWMAX;			/*	0x0F120011	最大反向速度	默认:10%	0%~50%*/

	UI16	TORQUEMAX_SLAVE_STOP;	/*	0x0F120012	从站停转扭矩	默认:50.0%	10.0%~250.0%*/

    UI16	MODE_SLAVE_FOLLOW;		/*	0x0F120013	从站跟随模式	默认:0:启停控制	范围：0:启停控制(有单向阀) 1：跟随至下一个动作(无单向阀)*/
	UI16	BIGROTARY_D;            /*	0x0F120014  大齿轮齿数[动作转盘]				默认:1				范围:0   ~ 65535		*/
	UI16	SMALLROTARY_D;          /*	0x0F120015  小齿轮齿数[驱动转盘]				默认:1				范围:0   ~ 65535		*/
	/*===========================================================================================*/	
}PUMP_MACHSET, *PPUMP_MACHSET;

typedef struct  EN_PUMP_MOLDSET
{
	UI16	PUMP_SLAVE_START;	/*	0x0F130000	从站跟随启动速度 默认:10%	范围:0~99%		*/
	UI16	PUMP_SLAVE_STOP;	/*	0x0F130001	从站跟随停止速度 默认:10%	范围:0~启动速度%		*/
}PUMP_MOLDSET, *PPUMP_MOLDSET;

typedef struct EN_PUMP_STATE
{
    UI16	FL_ERROR0;				//  0x0F160000 警报1
    UI16	FL_ERROR1;				//  0x0F160001 警报2

	UI16  	MOTOR_CURRENT;          /*	0x0F160002	电机电流		0.1A	*/
	UI16  	MOTOR_VOLTAGE;			/*	0x0F160003	电机电压			*/
	UI16  	MOTOR_POWER;			/*	0x0F160004	电机功率			*/
	UI16  	MOTOR_TEMP;				/*	0x0F160005	电机温度		1	℃	*/
	UI16  	MOTOR_SPEED_REAL;		/*	0x0F160006	电机实际转速	1	RPM	*/
    UI16  	MOTOR_SPEED_SET;		/*	0x0F160007	电机指令速度	1	RPM	*/	
	UI16	MOTOR_TORQUE_REAL;		/*	0x0F160008	电机实际转矩	0.1	%	*/
	
	UI16  	SERVO_STATE;			/*	0x0F160009	伺服状态		无小数位	驱动地址H6041	*/
	UI16  	SERVO_ERR;              /*	0x0F16000A	伺服警报		无小数位	*/
	UI16  	SERVO_TEMP;             /*	0x0F16000B	驱动器模块温度	1	℃	*/
	UI16  	SERVO_PRESS;            /*	0x0F16000C  实际油箱压力		bar	*/
	UI16	ENCODE_POS;				/*	0x0F16000D  编码器位置		puls	*/	
	UI16  	SERVO_COMM_STL;          /*	0x0F16000E 	伺服通讯状态	无小数位	*/
	UI16  	SERVO_COMM_STH;          /*	0x0F16000F 	伺服通讯状态	无小数位	*/
}PUMP_STATE, *PPUMP_STATE;

#pragma pack()

#ifdef DIC_ENUM_NEED


#define     ER0_SERVO_ERR			0x0001      // #0F100；伺服警报

#endif

#endif

