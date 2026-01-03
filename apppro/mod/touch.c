#include "touch.h"
#include "oprintf.h"
#include "file.h"
#include "panel.h"
#include "database.h"
#include "keyprot.h"
#include "keyprot2.h"
#include "key.h"
#include "information.h"

#define     COMM_TX_INQUIRELCD  0xE1 //回复背光亮度信号
#define     COMM_TX_READANSWER  0xE2 //解析单片机回复

#define BUZZER_SOUND                0x3317004e //蜂鸣器声音
#define HARDWARE_VERS1              0x3317004f //单片机版本号1
#define HARDWARE_VERS2              0x33170050 //单片机版本号2
#define PANEL_BRIGHT                0x33170051 //背光亮度

static BOOL bM792;
#ifdef IMX6
static UI16 glastKey = KEY_RELEASE;
#endif

void ProcessCommRxData3(UI8* buf, int len)
{
    UI8 type;
    type = buf[2];

    switch(type)
    {
    case KEY_KEY_RX_CMD:
    {
            UI16 wKey = buf[4]<< 8 | buf[5];
            if (IsTouchType())//20221123 dyl 触摸屏时可触发，否则无效
            {oprintf("ProcessCommRxData3");
                if((wKey&0x7FFF) == KEY_CALIBRATEON)
                {
                    PanelShowDialogByName("Message_Touch");
                }
            }

            if(GetKeyCnt()>5)
            {
                if(wKey & 0x8000)
                {
                    VarAdrSetInt(HARDWARE_LOCK,2);//按键板通讯锁，第二位置1
                    if(wKey != 0x8000)
                    {
                        SendKey(wKey);
                    }
                }
                else
                {
                    VarAdrSetInt(HARDWARE_LOCK,VarAdrToInt(HARDWARE_LOCK)&0xD);
                    if(wKey != 0)
                    {
                        SendKey(wKey);
                    }
                }
            }

#ifndef WIN32
#ifdef IMX6
            if(glastKey!=wKey)
            {
                if(wKey != KEY_RELEASE && wKey != 0 )
                {
                    KeySoundBeep();
                }
                glastKey = wKey;
            }
#endif
#endif
            break;
    }
    case COMM_TX_INQUIRELCD:
        VarAdrSetInt(PANEL_BRIGHT,(buf[4]<<8)|buf[5]);
        break;
    case COMM_TX_READANSWER:
        VarAdrSetInt(HARDWARE_VERS1,(buf[4]<<8)|buf[5]);
        VarAdrSetInt(HARDWARE_VERS2,(buf[6]<<8)|buf[7]);
        if(buf[7] >= 0xB0)
        {
            bM792 = TRUE;
        }
        else
        {
            bM792 = FALSE;
        }
        break;
    }
}

void SetKeySoundInit()
{
    KeySetKeySoundRelAddr(BUZZER_SOUND);
}

BOOL IsM792()
{
    if(bM792)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void ReceieveRxKey(UI16 key)//串口2接收到0xF0
{
    if (IsTouchType())//20221123 dyl 触摸屏时可触发，否则无效
    {
        if((key&0x7FFF) == KEY_CALIBRATEON)
        {
            PanelShowDialogByName("Message_Touch");
        }
    }

    if(key & 0x8000)
    {
        VarAdrSetInt(HARDWARE_LOCK,2);//按键板通讯锁，第二位置1
    }
    else
    {
        VarAdrSetInt(HARDWARE_LOCK,VarAdrToInt(HARDWARE_LOCK)&0xD);
    }
}

