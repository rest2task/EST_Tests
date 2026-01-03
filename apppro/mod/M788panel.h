#ifndef _M788PANEL_h
#define _M788PANEL_h

#ifdef __cplusplus
extern "C"
{
#endif
#include "typedef.h"

#ifdef M788_PANEL
int OpenLCD_M788();
void SetLCD_M788(BOOL bstate);
void KeyBeep_M788();
/*******
 * //20221122 dyl G15使用M789B：背光使能ARM，调光pwm2
 * *****/
int OpenLCD_G15M789B();
void SetLCD_G15M789B(BOOL bstate);
#endif

#ifdef __cplusplus
}
#endif

#endif



