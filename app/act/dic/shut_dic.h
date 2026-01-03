#ifndef _SHUT_DIC_H_
#define _SHUT_DIC_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"

/*----------液压喷嘴部件(挡板进退)----------*/
#pragma pack(1)

typedef struct EN_SHUT_PI    //20170505 LET
{
	UI16  I_SHUTFWD ;                 //26100000 挡板进终     
	UI16  I_SHUTBWD ;                 //26100001 挡板退终   
}SHUT_PI, *PSHUT_PI;


typedef struct EN_SHUT_PO
{
	UI16  O_SHUTFWD ;                 //26110000 挡板进      
	UI16  O_SHUTBWD ;                 //26110001 挡板退   
}SHUT_PO, *PSHUT_PO;

typedef struct tyEN_SHUT_MACHSET     //20170505 LET
{
	UI16  FL_SHUT;                            /*26120000   挡板功能 :0 不使用 1使用     默认不使用  */
	UI16  FL_SHUT_FUN ;                       /*26120001   挡板联动选择  0 不联动  1联动   默认不联动*/
	UI16  FL_SHUT_MODE;                       /*26120002   液压挡板控制方式  0 时间    1 位置  默认时间 */
	UI16  DAPRES_SHUTFWDRAMP;                 /*26120003  挡板进压力斜率       0.00-1.99S    默认0.50*/
	UI16  DAFLOW_SHUTFWDRAMP;                 /*26120004  挡板进速度斜率       0.00-1.99S    默认0.50*/
	UI16  DAPRES_SHUTFWDOFFRAMP;              /*26120005  挡板进关闭压力斜率   0.00-1.99S    默认0.50*/
	UI16  DAFLOW_SHUTFWDOFFRAMP;              /*26120006  挡板进关闭速度斜率   0.00-1.99S    默认0.50*/
	UI16  DAPRES_SHUTBWDRAMP;                 /*26120007  挡板退压力斜率       0.00-1.99S    默认0.50*/
	UI16  DAFLOW_SHUTBWDRAMP;                 /*26120008  挡板退速度斜率       0.00-1.99S    默认0.50*/
	UI16  DAPRES_SHUTBWDOFFRAMP;              /*26120009  挡板退关闭压力斜率   0.00-1.99S    默认0.50*/
	UI16  DAFLOW_SHUTBWDOFFRAMP;              /*2612000A  挡板退关闭速度斜率   0.00-1.99S    默认0.50*/
}SHUT_MACHSET, *PSHUT_MACHSET;

typedef struct tyEN_SHUT_MOLDSET     //20170505 LET
{
	UI16  DAFLOW_SHUTBWDOFFRAMP;              /*26130000    挡板退关闭速度斜率       .01S    默认0.50*/
}SHUT_MOLDSET, *PSHUT_MOLDSET;

typedef struct EN_SHUT_ACTPARA
{
	/*----------液压喷嘴部件(挡板进退)----------*/
	UI16  DAPRES_SHUTFWD;                      /* 26140000 挡板进压力    范围0-系统压力上限 默认30   */
	UI16  DAFLOW_SHUTFWD;                      /* 26140001 挡板进速度    范围0-99           默认30   */
	UI16  TM_SHUTFWD;                          /* 26140002 挡板进计时    范围0.00-9.99        默认0.00     20200219 */
	UI16  DAPRES_SHUTBWD;                      /* 26140003 挡板退压力    范围0-系统压力上限 默认50   */
	UI16  DAFLOW_SHUTBWD;                      /* 26140004 挡板退速度    范围0-99           默认30   */
	UI16  TM_SHUTBWD;                          /* 26140005 挡板退计时    范围0.00-9.99        默认0.00   20200219 */
}SHUT_ACTPARA, *PSHUT_ACTPARA;


typedef struct EN_SHUT_HYDR         //20170505 LET
{
	UI16  HYDR_PORT_SHUT;							//26150000  挡板部件DA输出通道：位操作-->port1……port4
	UI16  HYDR_PUMPS_SHUT;							//26150001  挡板部件比例速度限制:位操作-->P1……P7
	UI16  SERVO_KP_SHUT;							//26150002  挡板动作的PID_KP
	UI16  SERVO_KI_SHUT;							//26150003  挡板动作的PID_KI
	UI16  SERVO_KD_SHUT;							//26150004  挡板动作的PID_KD
}SHUT_HYDR, *PSHUT_HYDR;


typedef struct EN_SHUT_STATE
{
	UI16  FL_ERROR0;								//0x26160000 警报1
	UI16  FL_ERROR1;								//0x26160001 警报2
    UI16  Step_ShutFwd;                             //0x26160002 挡板进实时动作步
    UI16  Step_ShutBwd;                             //0x26160003 挡板退实时动作步
}SHUT_STATE, *PSHUT_STATE;

#pragma pack()


#ifdef DIC_ENUM_NEED


enum EN_SHUT_ACT_SN
{
    OS_SHUTFWD = 1,			// 0   ; '挡板进'
    OS_SHUTBWD,				// 0   ; '挡板退'

    SHUT_ACT_SN_END
};

enum EN_SHUT_ACTNAME		//20170505 LET
{
    SHUTFWD = STAGE_1_STEP,		//挡板进
    SHUTBWD = STAGE_1_STEP,		//挡板退
};

#endif

#ifdef __cplusplus
}
#endif

#endif



