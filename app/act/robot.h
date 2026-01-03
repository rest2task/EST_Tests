#ifndef _ROBOT_H_
#define _ROBOT_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "part.h"
#include "robot_dic.h"
#include "clamp.h"

typedef enum enROBOTTYPE
{
	ROBTYPE_NONE = 0,
	ROBTYPE_NORMAL,
	ROBTYPE_EU12,
	ROBTYPE_EU67,
	ROBTYPE_END
}ROBOTTYPE;


typedef struct tyROBOT
{
	PART				part;
	
	PROBOT_PO			po;
	PROBOT_PI			pi;
	PROBOT_MOLDSET		md;
	PROBOT_STATE		st;

	PCLAMP				pclmp;

	UI16		flag_Robot_Turn;		//机械手信号翻转起始标记
	UI16		Tm_Robot_Turn;			//机械手翻转初始时间
	UI16                AlarmTime;
}ROBOT, *PROBOT;

#define ROBOT_INIT_INFO		{{PO_ID, CAL_LEN(ROBOT_PO)}, {PI_ID, CAL_LEN(ROBOT_PI)}, {MD_ID, CAL_LEN(ROBOT_MOLDSET)}, {ST_ID, CAL_LEN(ROBOT_STATE)}}

void RobotAutoON(PROBOT pp);
BOOL RobotRecycleErrChk(PROBOT pp);
void RobotClampCloseStart(PROBOT pp);
void RobotClampCloseEnd(PROBOT pp, BOOL bset);
void RobotClampOpenEnd(PROBOT pp);
 void RobotInjectOnOff(PROBOT pp, BOOL bon);
 void RobotInjectErr(PROBOT pp);
BOOL RobotClampClsErrChk(PROBOT pp);

BOOL RobotClampPreErrChk(PROBOT pp);
BOOL RobotClampOpnErrChk(PROBOT pp);

BOOL RobotClampClsProtectErrChk(PROBOT pp);
void RobotEmergeStop(PROBOT pp, BOOL bemerge);
void RobotEmergeStop2(PROBOT pp, BOOL bemerge);
void RobotClampOpenMid(PROBOT pp);	  //开模中途，当有开模中途输出点时使用	20170711

BOOL InitRobotPart(PROBOT pp, UI8 sn);
void EuroRobotChkLoop(PROBOT pp);					//欧规机械手动作

BOOL RobotEjectFwdErrChk(PROBOT pp);  //等待机械手，托模进前判断  20200922
BOOL RobotEjectFwdChk(PROBOT pp);		//机械手失败，托模进过程中判断  20200922
BOOL RobotEjectBwdErrChk(PROBOT pp);  //等待机械手，托模退前判断  20200922
BOOL RobotEjectBwdChk(PROBOT pp) ;	//机械手失败，托模退过程中判断  20200922

BOOL RobotCoreFwdErrChk(PROBOT pp,int i);  //等待机械手，中子进前判断 20200922
BOOL RobotCoreFwdChk(PROBOT pp,int i);  //机械手失败   中子进过程判断20200922 
BOOL RobotCoreBwdErrChk(PROBOT pp,int i);  //等待机械手，中子退前判断 20200922
BOOL RobotCoreBwdChk(PROBOT pp,int i);  //机械手失败   中子退过程判断20200922 

void RobotCoreFwd(PROBOT pp,int i);	
void RobotCoreFwdEnd(PROBOT pp,int i);
void RobotCoreBwd(PROBOT pp,int i);
void RobotCoreBwdEnd(PROBOT pp,int i);

void Robot_Turn(void);
void MoldClampCloseStart(PROBOT pp);
void MoldClampOpenEnd(PROBOT pp);
void MoldChkLoop(PROBOT pp);    
BOOL MoldClampClsErrChk(PROBOT pp); 
BOOL MoldClampClsProtectErrChk(PROBOT pp); 
BOOL MoldClampOpnProtectErrChk(PROBOT pp);
BOOL MoldEjectFwdErrChk(PROBOT pp);
BOOL MoldEjectBwdErrChk(PROBOT pp);
void MoldAlarmClear(void);
BOOL RobotConfigErrChk(PROBOT pp);

#ifdef __cplusplus
}
#endif

#endif

