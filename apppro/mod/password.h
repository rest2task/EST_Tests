#ifndef _PASSWORD_h
#define _PASSWORD_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "mod.h"


void PasswordInital(PCMOD pmod);

/* password page show */
BOOL PasswordIn(PCPAR para);
/*password page OK button */
BOOL PasswordCheck(PCPAR para);
/*password page CANCEL button*/
BOOL PasswordCancel(PCPAR para);
void Cal_Clamp_Prs_Max_Protect_PSW(UI32 wAddr);//20241017 chj 金鹰特殊厂商密码计算

#ifdef __cplusplus
}
#endif

#endif



