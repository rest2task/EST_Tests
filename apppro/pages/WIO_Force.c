#include "WIO_Force.h"
#include "typedef.h"
#include "pagefram.h"
#include "panel.h"
#include "vardb.h"
#include "checkboxwgt.h"
#include "ioredef.h"
#include "oprintf.h"

DEFINE_IO_FORCE_MAP
CONNECT_IO_FORCE_EVENT

#define     IO_STATE_pi_input1  0x6160002
#define     MASK_BIT0           0x01

BOOL bforce1 = FALSE,bforce2 = FALSE;
UI16 Num = 0;

static void CtlConnect()
{
}
static int IO_ForceOnInit(IO_Force_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int IO_ForceOnShow(IO_Force_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    //20190325 YX
    Num = GetCurrentIoNum();//得到是由哪个点点击的输入点序号
    bforce1 = FALSE;//是否使用强制输入复选框标志位
    bforce2 = FALSE;//是否强制设置复选框标志位
    UI8 bAddId=Num/16;//得到该序号是否在16位中
    UI8 bOffset=Num%16;//该16位中的第几位
    UI16 wIOData=VarAdrToInt(IO_STATE_pi_input1+bAddId)>>bOffset & MASK_BIT0; //得到该位是否置1
    CheckboxSetCheck(io_force_pg.checkbox1,wIOData);//得到该位所对应的复选框是否勾上

    //force input
    if(GetUseForceRec(Num))//是否使用强制输入的记录
    {
        CheckboxSetCheck(io_force_pg.checkbox2,TRUE);//显示使用强制输入对应的复选框是否勾上
    }
    else
    {
        CheckboxSetCheck(io_force_pg.checkbox2,FALSE);
    }

    if(GetForceInputRec(Num))//是否强制设置记录
    {
        CheckboxSetCheck(io_force_pg.checkbox3,FALSE);//显示强制设置对应的复选框是否勾上
    }
    else
    {
        CheckboxSetCheck(io_force_pg.checkbox3,TRUE);
    }

    return 0;
}
static int IO_ForceOnHide(IO_Force_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int IO_ForceOnUpdate(IO_Force_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI8 bAddId=Num/16;//得到该序号是否在16位中
    UI8 bOffset=Num%16;//该16位中的第几位
    UI16 wIOData=VarAdrToInt(IO_STATE_pi_input1+bAddId)>>bOffset & MASK_BIT0; //得到该位是否置1
    CheckboxSetCheck(io_force_pg.checkbox1,wIOData);//得到该位所对应的复选框是否勾上
    return 0;
}
static int IO_ForceOnChange(IO_Force_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    //20190325 YX
    BOOL bflagUseForce = FALSE,bflagForceInput = FALSE;
    if(pwgt == ppg->btok)
    {
        if(bforce1 || bforce2)
        {
            if(CheckboxGetCheck(ppg->checkbox2))
            {
                bflagUseForce = TRUE;
            }
            else
            {
                bflagUseForce = FALSE;
            }
            if(CheckboxGetCheck(ppg->checkbox3))
            {
                bflagForceInput = TRUE;
            }
            else
            {
                bflagForceInput = FALSE;
            }
            RedefForcePoint(Num,bforce1,bforce2,bflagUseForce,bflagForceInput);
        }
        PanelCloseDialogByName(io_force_pg_name);
    }
    else if(pwgt == ppg->btcancel)
    {
        PanelCloseDialogByName(io_force_pg_name);
    }
    else if(pwgt == ppg->checkbox2)
    {
        bforce1 = TRUE;
    }
    else if(pwgt == ppg->checkbox3)
    {
        bforce2 = TRUE;
    }
    else if(pwgt == PageGetWgtByName(io_force_pg_name,"btnclose"))
    {
        PanelCloseDialogByName(io_force_pg_name);//20191223.cyx
    }
    return 0;
}
