#ifndef _PAY_h
#define _PAY_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "mod.h"
#include "wgt.h"

#define  WARNING_DAYS    5*24*3600
#define  INTERVAL_DAYS   10*24*3600

typedef enum enPAY_PERIODS
{
	PAYPERIODS_FIRST = 1,
	PAYPERIODS_SECOND,
	PAYPERIODS_THIRD,
	PAYPERIODS_FOURTH,
	PAYPERIODS_FIFTH,
	PAYPERIODS_SIXTH
}PAY_PERIODS;

typedef struct tyOPERATION
{
	UI16 cState;
	UI16 cPeriods;
	UI16 tHour;
	UI16 tMinute;
	UI16 doYear;
	UI16 doMonth;
	UI16 doDay;
	UI16 dsYear;
	UI16 dsMonth;
	UI16 dsDay;
	UI16 wInterval;
	UI16 wIdentifyCode;
	UI16 wIdentifyConfirm;
	UI16 cNowNum;
	UI16 bTimeOut;
	UI16 cMode;
	UI16 ds2Year;
	UI16 ds2Month;
	UI16 ds2Day;
	UI16 ds3Year;
	UI16 ds3Month;
	UI16 ds3Day;
	UI16 ds4Year;
	UI16 ds4Month;
	UI16 ds4Day;
	UI16 ds5Year;
	UI16 ds5Month;
	UI16 ds5Day;
	UI16 ds6Year;
	UI16 ds6Month;
	UI16 ds6Day;
	UI16 ds7Year;
	UI16 ds7Month;
	UI16 ds7Day;
	UI16 ds8Year;
    UI16 ds8Month;   //新模式下用做 剩余天数
    UI16 ds8Day;     //新模式下用做 总天数
    UI16 ds9Year;    //新模式下用做签发日期 年
    UI16 ds9Month;   //新模式下用做签发日期 月
    UI16 ds9Day;     //新模式下用做签发日期 日
    UI16 ds10Year;   //新模式下用做签发日期 时
    UI16 ds10Month;  //新模式下用做签发日期 分
    UI16 ds10Day;    //新模式下用做签发日期 秒
	char machine2[8+1];  
}OPERATION,*POPERATION;

typedef struct tyPAYSET_HEAD
{
	UI16    flag;
	OPERATION m_operation;
}PAYSET_HEAD;

typedef struct tyONLY_ID{
    char only_id[8+1];
}ONLY_ID;

extern OPERATION m_operation;
extern ONLY_ID onlyid;

void PaySetInital(PCMOD pmod);
BOOL IsPayChecking();
UI16 IsPayTimeOut();
BOOL IsInstallAsk(); 
UI16 PayIdentifyCode();

int Pay(UI32 dwpassword);
int Payx(UI64 dwpassword);
void PayMonitor();
void SavePayData();
void CheckPaySet();
void SetDayMax(PWGT pwgt,UI16 year,UI16 month);
UI16 ControlPass(UI8 year,UI8 month,UI8 day,UI16 random);

/*************************低电压***************************************/
#define OPENINFO_FILE SAVE_PATH"openinfo.txt"  //保存开机信息
#define LOWVOL_FILE SAVE_PATH"lowvol.dat"      //保存低电压时间

void SaveCurTmLowVol();

#ifdef __cplusplus
}
#endif

#endif



