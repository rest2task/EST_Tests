#ifndef _LOGIN_h
#define _LOGIN_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "mod.h"
#include "panel.h"

#define GRAB_PNG_FLAG  20   //截图PNG标记
#define GRAB_BMP_FLAG  21   //截图BMP标记
#define DATA_PROT_FLAG 22   //20190118 数据保护页面标记

/************************************************************************/
/* MOD interface                                                        */
/************************************************************************/
void LoginInital(PCMOD pmod);
BOOL LoginConfigDlg(PCPAR para);
BOOL LoginConfigJump(PCPAR para);
BOOL LoginSwitch(PCPAR para);

int LoginFindPwdIndex(UI32 password);
PPAGE_FRAM LoginPage();

extern int m_pwdArray[];

#ifdef __cplusplus
}
#endif

#endif



