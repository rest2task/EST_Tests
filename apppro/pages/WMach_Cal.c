#include "WMach_Cal.h"
#include "typedef.h"
#include "pagefram.h"
#include "database.h"
#include "ctrlcmd.h"
#include "protcmd.h"
#include "privdef.h"
#include "buttonwgt.h"
#include "texttbl.h"

DEFINE_MACH_CAL_MAP

CONNECT_MACH_CAL_EVENT

#define CLAMP_PART       0x2010
#define EJECT_PART       0x2110
#define INJ_PART         0x1010

static BOOL badjust = FALSE;
static UI16 wSend1 = 0;
static UI16 wSend2 = 0;

static void CtlConnect()
{
}
static int Mach_CalOnInit(Mach_Cal_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Mach_CalOnShow(Mach_Cal_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Mach_CalOnHide(Mach_Cal_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Mach_CalOnUpdate(Mach_Cal_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if((VarAdrToInt(d_machine1_STATE_ST_WOPERMODE)&0xFFFE) == MODE_RAMPADJUST)
    {
        badjust = true;
        if((wSend1 != 0xFF))
        {
            if(wSend2 == CLAMP_PART)
            {
                ButtonSetText(ppg->btStartClamp,GetTextTran(TEXT_ADJUSTING));
                WGTSetEnable(ppg->btStartClamp,TRUE);
                WGTSetEnable(ppg->btStartEject,FALSE);
                WGTSetEnable(ppg->btStartSKBK,FALSE);
            }
            else if(wSend2 == EJECT_PART)
            {
                ButtonSetText(ppg->btStartEject,GetTextTran(TEXT_ADJUSTING));
                WGTSetEnable(ppg->btStartClamp,FALSE);
                WGTSetEnable(ppg->btStartEject,TRUE);
                WGTSetEnable(ppg->btStartSKBK,FALSE);
            }
            else if(wSend2 == INJ_PART)
            {
                ButtonSetText(ppg->btStartSKBK,GetTextTran(TEXT_ADJUSTING));
                WGTSetEnable(ppg->btStartClamp,FALSE);
                WGTSetEnable(ppg->btStartEject,FALSE);
                WGTSetEnable(ppg->btStartSKBK,TRUE);
            }
            else
            {
                ;
            }
        }
    }
    else
    {
        badjust = false;
        ButtonSetText(ppg->btStartClamp,GetTextTran(TEXT_START));
        ButtonSetText(ppg->btStartEject,GetTextTran(TEXT_START));
        ButtonSetText(ppg->btStartSKBK,GetTextTran(TEXT_START));
        WGTSetEnable(ppg->btStartClamp,TRUE);
        WGTSetEnable(ppg->btStartEject,TRUE);
        WGTSetEnable(ppg->btStartSKBK,TRUE);
    }

    return 0;
}
static int Mach_CalOnChange(Mach_Cal_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 data[2];
    if(pwgt == ppg->btStartClamp)
    {
        if(badjust)
        {
            wSend1 = 0xFF;
        }
        else
        {
            wSend1 = 0x01;
        }
        wSend2 = CLAMP_PART;

        data[0] = wSend1;
        data[1] = wSend2;
        ProtSysCtrl(COMM_SUBCMD_SLOPE_CALIBRATION,2,data);
    }
    else if(pwgt == ppg->btStartEject)
    {
        if(badjust)
        {
            wSend1 = 0xFF;
        }
        else
        {
            wSend1 = 0x01;
        }
        wSend2 = EJECT_PART;

        data[0] = wSend1;
        data[1] = wSend2;
        ProtSysCtrl(COMM_SUBCMD_SLOPE_CALIBRATION,2,data);
    }
    else if(pwgt == ppg->btStartSKBK)
    {
        if(badjust)
        {
            wSend1 = 0xFF;
        }
        else
        {
            wSend1 = 0x01;
        }
        wSend2 = INJ_PART;

        data[0] = wSend1;
        data[1] = wSend2;
        ProtSysCtrl(COMM_SUBCMD_SLOPE_CALIBRATION,2,data);
    }

    return 0;
}
