#ifndef _DRIVEALARMRECORD_h
#define _DRIVEALARMRECORD_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "mod.h"
#include "alarmrecord_G10.h"
#include "warndatarec.h"

typedef struct tyDRIVEALARMHEAD
{
    UI16 flag;
    UI16 cur_no;
    UI16 reserve[6];
}DRIVEALARMHEAD;

typedef struct tyDRIVEALARMITEM
{
    UI16 flag;
    UI16 alarmservoid;  //警报驱动编号
    UI16 alarmcode;     //警报代码
    UI16 H0100;         //MCU 版本
    UI16 H0101;         //FPGA 版本
    UI16 H0B37;          //故障时电机转速(有可能是负的)
    UI16 H0B38;          //故障时电机U相电流(有可能是负的)
    UI16 H0B39;          //故障时电机V相电流(有可能是负的)
    UI16 H0B40;         //故障时母线电压
    UI16 H0B41;         //故障时端子输入状态
    UI16 H0B42;         //故障时端子输出状态
    UI16 H0B43;         //参数异常的功能码组
    UI16 H0B44;         //参数异常的功能码组内偏置
    UI16 H0B46;         //故障时FPGA给出的绝对编码器故障信息
    UI16 H0B47;         //故障时FPGA给出的系统状态信息
    UI16 H0B48;         //故障时FPGA给出的系统故障信息
    UI32 H0B35;         //故障时间戳
    UI16 rev[14];       //预留
}DRIVEALARMITEM;

typedef struct tyDRIVEALARMRECORD
{
    DRIVEALARMHEAD head;
    DRIVEALARMITEM item[G10_MAX_RECORDS];
} DRIVEALARMRECORD,*PDRIVEALARMRECORD;

//下位机警报上传
typedef struct tyHOSTALARMHEAD
{
    UI16 flag;
    UI16 cur_no;
    UI16 reserve[6];
}HOSTALARMHEAD;

typedef struct tyHOSTALARMRECORD
{
    HOSTALARMHEAD head;
    WDITEM item[G10_MAX_RECORDS];
} HOSTALARMRECORD,*PHOSTALARMRECORD;

DRIVEALARMITEM DriveAlarm_ReadRecord(UI32 index);
void DriveAlarm_LoadRecord();
void DriveAlarm_ClearRecord();
BOOL DriveAlarm_HaveRecord(UI32 index, UI16 subId);
void DriveAlarm_ReqServoIdData(UI16 servo_id, UI32 alarmcode, UI16 curno);

WDITEM HostAlarm_ReadRecord(UI32 index);
void HostAlarm_LoadRecord();
void HostAlarm_ClearRecord();
BOOL HostAlarm_HaveRecord(UI32 index, UI32 warnid, UI32 datetime);
void AddHostAlarm(WDITEM item);

void InitAlarmExpState();
BOOL GetAlarmExpState();

#define DRIVEALARM_SAVE_PATH   SAVE_PATH"drivealarmrec.dat"
#define HOSTALARM_SAVE_PATH    SAVE_PATH"hostalarmrec.dat"


#ifdef __cplusplus
}
#endif

#endif



