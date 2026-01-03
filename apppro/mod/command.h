#ifndef _COMMAND_h
#define _COMMAND_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "mod.h"

#define MACHINE_ID           0x00
#define PLC_ID               0x01
#define MOTOR_ID             0x04
#define TEMPCTRL_ID          0x05
#define IO_ID                0x06
#define METER_ID             0x07
#define DA_ID                0x08
#define HYDR_ID              0x09
#define HYDRSERVO_ID         0x0A
#define ALARM_ID             0x0B
#define OIL_ID               0x0C
#define TEMPEXT_ID           0x0E
#define PUMP_ID              0x0F
#define INJECT_ID            0x10
#define NOZZLE_ID            0x11
#define INJSTOR_ID           0x12
#define CHGPRS_ID            0x13
#define CLAMP_ID             0x20
#define EJECT_ID             0x21
#define CORE_ID              0x22
#define AIRBLOW_ID           0x23
#define ADJUST_ID            0x24
#define SYSACC_ID            0x25
#define SHUT_ID              0x26
#define ROTATE_ID            0x27
#define LOCK_ID              0x28
#define SFDOOR_ID            0x29
#define MOLDTEMP_ID          0x2F //20200407HJM 模温功能
#define LUB_ID               0x30
#define ROBOT_ID             0x31
#define AUTOCTRL_ID          0x32

#define PANELDATA_ID		 0x33

#define PART1_SN    0x01
#define PART2_SN    0x02
#define PART3_SN    0x03
#define PART4_SN    0x04
#define PART5_SN    0x05
#define PART6_SN    0x06

#define PART1_ID(x)             ((((UI8)(x))<<8)|(1<<4))  
#define PART2_ID(x)             ((((UI8)(x))<<8)|(2<<4))  
#define PART1_SUB_ID(x,x1)      ((((UI8)(x))<<8)|(1<<4) | (UI8)(x1))   

#define PO       0x00
#define PI       0x01
#define MACHSET  0x02
#define MOLDSET  0x03
#define ACTPARA  0x04
#define HYDR     0x05
#define STATE    0x06

#define MACH2    0x07  //全电部分
#define NETSET   0x08  //联网部分
#define STANDV   0x09  //立式机部分

//send release mode init cmd
void ReleaseMdInit();

//send code to mcu，and then send save cmd
void CodeSendSave();

void CodeSendSaveBack();//20230518 chj 此函数调用后，告诉主机需要提示重启,内容同CodeSendSave一样

//send zero meter to mcu,and then send zero cmd
BOOL AdMeterZero(UI32 zero_addr,UI32 abs_addr);

//send real temp zero cmd to mcu
void RealTempZero();

//send temp correction cmd to mcu
void TempCorrection();

//send roomtemp adj cmd to mcu
void RoomTempSend();

#ifdef __cplusplus
}
#endif

#endif



