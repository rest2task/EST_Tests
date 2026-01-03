#include "login.h"
#include "oprintf.h"
#include "navigate.h"
#include "database.h"
#include "time.h"

static CPROC_INFO LoginProcs[] = 
{
	{"config_dlg",  LoginConfigDlg,	CPAR_STR},			//配置密码对话框名称
	{"config_jump", LoginConfigJump,CPAR_INT,CPAR_STR},	//配置跳转页面
	{"switch",		LoginSwitch}					    //弹出/隐藏密码对话框
};

int m_pwdArray[] =
{
	5959,
	5858,
	4545,
	14321,
	12995,
	19110,
	7474,
	5656,
	5757,
	15963,
    14753,
    14520,
    54321
};
PPAGE_FRAM m_pwdJumpPages[ARRAY_NUM(m_pwdArray)];
PPAGE_FRAM   m_pLoginDlg = NULL;

/************************************************************************/
/*methods                                                               */
/************************************************************************/
int LoginFindPwdIndex(UI32 password)
{
	int i;
	//20190118
	UI32 wpassword;
	struct tm* t;
	time_t m_tmNow;

	time(&m_tmNow);
	t = localtime(&m_tmNow);

	wpassword=(t->tm_year+1900-2000)*10+(t->tm_mon+1)*100+t->tm_mday*13+20000;//数据保护画面密码

	for(i = 0; i < ARRAY_NUM(m_pwdArray); ++i)
	{
		if(m_pwdArray[i] == password)
			return i;
		else if (password == 619119)
			return 4; //operator
		else if(password == 31415)
			return 8;
		else if (password ==1)
			return GRAB_PNG_FLAG;
		else if (password ==2)
			return GRAB_BMP_FLAG;
		else if ((password == VarAdrToInt(DATA_PROTECT_PASSWORD)) || (password == wpassword)) //20190118
			return DATA_PROT_FLAG;
	}
	return -1;
}

static void NaviLoginUser()
{
	if(m_pLoginDlg)
	{
		if(PanelTopDialog() == m_pLoginDlg)
		{
			PanelCloseDialog(m_pLoginDlg);
		}
		else
		{	
			PanelShowDialog(m_pLoginDlg);
		}
	}
	else
	{
		PanelShowDialog(m_pLoginDlg);
	}
}

/************************************************************************/
/* MOD interface                                                        */
/************************************************************************/
void LoginInital(PCMOD pmod)
{
	RegisterModProcs(pmod, LoginProcs, ARRAY_NUM(LoginProcs));
	NaviSetLoginCB(NaviLoginUser);
}

BOOL LoginConfigDlg(PCPAR para)
{
	m_pLoginDlg = PanelFindPageWithLoad(para[0].pstr);
	return m_pLoginDlg != NULL;
}

BOOL LoginConfigJump(PCPAR para)
{
	int index = para[0].v;
	const char* pagename = para[1].pstr;

	if(index < ARRAY_NUM(m_pwdArray))
	{
		m_pwdJumpPages[index] = PanelFindPage(pagename);
	}
	return TRUE;
}

BOOL LoginSwitch(PCPAR para)
{
	NaviLogin();
	return TRUE;
}

PPAGE_FRAM LoginPage()
{
	if(m_pLoginDlg !=NULL)
		return m_pLoginDlg;

	return NULL;
}
