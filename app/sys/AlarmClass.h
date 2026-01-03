#include "acttype.h"

#define ALARM_CLASS_OPR_NULL 0

#define ALARM_CACHE_NUM	10


typedef void (*ACT_OP_FUNC1)(void* pp);

typedef enum enALARM_CLASS
{
	ALARM_CLASS_1st = 0x00,		//1级警报	最严重的	警报 关马达 切手动
	ALARM_CLASS_2nd = 0x01,		//2级警报	严重		警报 切手动
	ALARM_CLASS_3rd = 0x10,		//3级警报	警报
	ALARM_CLASS_4th = 0x11,		//4级提示	提示类的警报
	
	ALARM_CLASS_END				//警报等级最大值
}EN_ALARM_CLASS;

typedef struct tyALARM_CLASS
{
	UID	part_id;	//警报部件ID
	UI32	alarm_id;	//警报ID
	UI16	alarm_class;	//警报分级
	ACT_OP_FUNC1	alarm_operate;	//警报操作
	UI16	alarm_updata;	//警报上传
}ALARM_CLASS, *PALARM_CLASS;


void InitAlarmClassIndex(void);
PALARM_CLASS GetAlarmClassInfo(UID partid,UI32 alarmid);

void InsAlarmBuf(UID partid,UI32 alarmid);
void InsAlarmInAlarmCache(void);
void DelAlarmInAlarmCache(UID partid,UI32 alarmid);
BOOL ChkAlarmIsEmpty(void);




