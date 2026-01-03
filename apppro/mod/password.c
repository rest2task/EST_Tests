#include "password.h"
#include "oprintf.h"
#include "panel.h"
#include "file.h"
#include "usermod.h"
#include "pagefram.h"
#include "database.h"
#include "stdio.h"
#include "time.h"

#define ADR_PASSWORD_PWD            0x33120046 //密码框密码

static CPROC_INFO PasswordProcs[] = 
{
	{"dlgIn",  PasswordIn},			//密码框进入
	{"check",		PasswordCheck},	//检查密码匹配
	{"cancel",		PasswordCancel} //隐藏密码对话框
};

USERPWD g_dbUserpwdDef = {
	{
	{0,0},
	{5858,1},
	{14321,2},
	{19110,3},
	{12995,4},
	{74,6},
	{5454,7},
	{19119,8}
	}
};

extern USERPWD dbuserpwd;

static PPAGE_FRAM   m_pPasswordDlg = NULL;

/************************************************************************/
/* private methods                                                      */
/************************************************************************/
static BOOL LoadUsePwd()
{
	FILEHD filehd;
	int readbytes;

	filehd = FileOpen(USERPWD_INIT_FILE);
	if(FILEHD_IS_OK(filehd))
	{
		FileRead(filehd,&dbuserpwd,sizeof(USERPWD),&readbytes);
		FileClose(filehd);
	}
	else {
		filehd = FileCreate(USERPWD_INIT_FILE);
		memcpy(&dbuserpwd,&g_dbUserpwdDef,sizeof(USERPWD));
		FileWrite(filehd,&dbuserpwd,sizeof(USERPWD),NULL);
		FileClose(filehd);
	}
	return TRUE;
}

static int Password_FindPwdIndex(UI32 password)
{
	int i;
	for(i = 0;i<MAX_USERPWD;++i)
	{
		if(dbuserpwd.userpwd[i].wpassword == password)
			return i;
	}
	return -1;
}

/************************************************************************/
/* MOD interface                                                        */
/************************************************************************/
void PasswordInital(PCMOD pmod)
{
	RegisterModProcs(pmod, PasswordProcs, ARRAY_NUM(PasswordProcs));
	//init user password
	LoadUsePwd();
}

BOOL PasswordIn(PCPAR para)
{
	VarAdrSetInt(ADR_PASSWORD_PWD,12345);
    m_pPasswordDlg = PanelFindPage(DIALOG_PASSWORD);

	return TRUE;
}

BOOL PasswordCheck(PCPAR para)
{
	int index;
	UI16 level;

	index = Password_FindPwdIndex(VarAdrToInt(ADR_PASSWORD_PWD));
	level = RU16(PanelCurPage()->privilege);

	if(index == level && m_pPasswordDlg != NULL && PanelCurPage() !=NULL )
	{
		PanelCloseDialog(m_pPasswordDlg);
		PanelCurPage() ->bprotect = FALSE;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


BOOL PasswordCancel(PCPAR para)
{
    PanelCurPage()->bprotect = TRUE;
	
    if(PanelCurPage() == PanelFindPage(PAGE_MAIN_ADJUSTX))
	{
		PanelShowPage(PanelMainPage());
	}

	return TRUE;
}

/*****************************************
*Subject  :
*Condition:
*Object   :开关模上限密码计算 //20241017 chj 金鹰特殊厂商密码计算
*Result   :
******************************************/
void Cal_Clamp_Prs_Max_Protect_PSW(UI32 wAddr)
{
    int Password=0;
    int tmp_year,tmp_month,tmp_day;
    struct tm* t;
    time_t tm_now;

    time(&tm_now);
    t = localtime(&tm_now);

    tmp_year = t->tm_year+1900-2000;/* 年份，其值从1900开始 */
    tmp_month = t->tm_mon+1;//月份（从一月开始，0代表一月取值区间为[0,11]
    tmp_day = t->tm_mday;

    if(tmp_year>=99)
        tmp_year=99;
    if(tmp_year<=0)
        tmp_year=0;

    if(tmp_month>=12)
        tmp_month=12;
    if(tmp_month<=1)
        tmp_month=1;

    if(tmp_day>=31)
        tmp_day=31;
    if(tmp_day<=1)
        tmp_day=1;
    //密码计算：年月各个位数相加再乘以9，再加日
    //如23.01.09  (2+3+0+1)x9 + 9
    //Password = (tmp_year%10+tmp_year/10 + tmp_month%10+tmp_month/10)*9+tmp_day;

    //密码计算：年月各个位数相加再乘以88，再乘日
    //如24.04.15  (2+4+0+4+1+5)x88x15
    Password = (tmp_year%10+tmp_year/10 + tmp_month%10+tmp_month/10+ tmp_day%10+tmp_day/10)*88*tmp_day;

    VarAdrSetInt(wAddr,Password);
}
