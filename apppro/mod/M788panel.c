#ifdef M788_PANEL
#include "M788panel.h"
#include "keyprot.h"
#include "dbenum.h"
#include "database.h"
#include <fcntl.h>
#include "stdio.h"
#include "oprintf.h"
#ifndef WIN32
#include <sys/ioctl.h>
#endif
#include "keymod.h"

static int fdLCD = -1;
static int fdLed;//蜂鸣器
int OpenLCD_M788()
{
#ifndef WIN32
#ifdef IMX6
    if (IsG15Type())//20221122 dyl G15使用M789B：背光使能ARM，调光pwm2
    {
        OpenLCD_G15M789B();
        SetLCD(TRUE);//20230518 chj 优化关屏调用频率，开机赋值一次
    }
    else
    {
        fdLCD = open("/dev/pwm2", O_RDWR);//背光
        SetLCD(TRUE);
        fdLed = open("/dev/cled", O_RDWR);//蜂鸣器
    }
#endif
#endif
    return fdLCD;
}

//20231230 chj 合并10寸15寸背光和调光控制
void SetLCD_M788(BOOL bstate)
{
#ifndef WIN32
#ifdef IMX6

#ifdef A40I //20220614 dyl
    UI16  m788a_bright =0;
    //char scmd[128];//20250319 chj 优化点屏幕功能,改为write模式

    m788a_bright = VarAdrToInt(p_PP_MACH2_Bright)/10;
    if(m788a_bright ==9) m788a_bright =1;
    else if(m788a_bright ==8)  m788a_bright =2;
    else if(m788a_bright ==7)  m788a_bright =3;
    else if(m788a_bright ==6)  m788a_bright =4;
    else if(m788a_bright ==5)  m788a_bright =5;
    else if(m788a_bright ==4)  m788a_bright =6;
    else if(m788a_bright ==3)  m788a_bright =7;
    else if(m788a_bright ==2)  m788a_bright =8;
    else if(m788a_bright ==1)  m788a_bright =8;//9->8 20230518 chj 如果p_PP_MACH2_Bright为21-29，此情况下会黑屏
    else  m788a_bright=1;
    if(bstate)
    {
        oprintf("---m788a_bright=%d\n",m788a_bright);
        //20250319 chj 优化点屏幕功能,改为write模式
        set_brightness(m788a_bright);
        //snprintf(scmd,sizeof(scmd)-1, "echo %d >/sys/class/backlight/backlight_lvds/brightness", m788a_bright);
        //osystem(scmd);
    }
    else
    {
        //20250319 chj 优化点屏幕功能,改为write模式
        set_brightness(9);
        //osystem("echo 9 >/sys/class/backlight/backlight_lvds/brightness");
    }

    //20231230 chj 合并10寸15寸背光和调光控制：15寸程序需要iO控制
    if(bstate)
    {
        ioctl(fdLed, 8, 1);
    }
    else
    {
        ioctl(fdLed, 8, 0);
    }
#else
    if(bstate)
    {
        ioctl(fdLCD, 1, VarAdrToInt(p_PP_MACH2_Bright));
    }
    else
    {
        ioctl(fdLCD, 1, 0);
    }
#endif

#endif
#endif
}
void KeyBeep_M788()
{
#ifndef WIN32
#ifdef IMX6
    ioctl(fdLed, 6, 1);
#endif
#endif
}

/*******
 * //20221122 dyl G15使用M789B：背光使能ARM，调光pwm2
 * *****/
int OpenLCD_G15M789B()
{
#ifndef WIN32
#ifdef IMX6
    fdLed = open("/dev/cled", O_RDWR);//背光
#endif
#endif
    return fdLed;
}
//20231230 chj 合并10寸15寸背光和调光控制
/*
void SetLCD_G15M789B(BOOL bstate)
{
#ifndef WIN32
#ifdef IMX6
    UI16  m788a_bright =0;
    char scmd[128];

    m788a_bright = VarAdrToInt(p_PP_MACH2_Bright)/10;
    if(m788a_bright ==9) m788a_bright =1;
    else if(m788a_bright ==8)  m788a_bright =2;
    else if(m788a_bright ==7)  m788a_bright =3;
    else if(m788a_bright ==6)  m788a_bright =4;
    else if(m788a_bright ==5)  m788a_bright =5;
    else if(m788a_bright ==4)  m788a_bright =6;
    else if(m788a_bright ==3)  m788a_bright =7;
    else if(m788a_bright ==2)  m788a_bright =8;
    else if(m788a_bright ==1)  m788a_bright =8;//9->8 20230518 chj 如果p_PP_MACH2_Bright为21-29，此情况下会黑屏
    else  m788a_bright=1;
    if(bstate)
    {
        oprintf("---m788a_bright=%d\n",m788a_bright);
        snprintf(scmd,sizeof(scmd)-1, "echo %d >/sys/class/backlight/backlight_lvds/brightness", m788a_bright);
        osystem(scmd);
    }
    else
    {
        osystem("echo 9 >/sys/class/backlight/backlight_lvds/brightness");
    }

    if(bstate)
    {
        ioctl(fdLed, 8, 1);
    }
    else
    {
        ioctl(fdLed, 8, 0);
    }

#endif
#endif
}
*/
#endif

