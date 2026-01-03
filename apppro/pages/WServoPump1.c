#include "WServoPump1.h"
#include "typedef.h"
#include "pagefram.h"
#include "protcmd.h"
#include "editwgt.h"
#include "stdio.h"
#include "keymod.h"
#include "key.h"
#include "tabpagewgt.h"
#include "imagewgt.h"
#include "privdef.h"
#include "oprintf.h"
#include "questionDlg.h"
#include "command.h"
#include "ctrlcmd.h"
#include "database.h"
#include "usermod.h"
#include "information.h"
#include "dataOperation.h"

DEFINE_SERVOPUMP1_MAP

CONNECT_SERVOPUMP1_EVENT

static UI16 old_page;
static QUESTION g_question;
static UI16 switch_key;
static BOOL bPump1Change = FALSE;

static void SavePumpData()
{
    int len;
    int i;
    UI16 part_sub_id;
    UI16 part_id;
    UI16 data[1];

    SavePumpPage(1, 6);
    for(i = 0; i < 6; ++i)
    {
        part_id = (UI8)PUMP_ID<<8 | (i+1)<<4;
        part_sub_id = part_id | MACHSET;
        len = GetSubLenByID(part_sub_id);
        ProtSendPartSub(part_id,MACHSET,len,GetSubByID(part_sub_id, NULL));
        data[0] = part_sub_id;
        ProtSysCtrl(COMM_SUBCMD_SERVOPUMP_WRITE,1,data);//save cmd
    }
}
/************************************************************************/
/* 切页面信息提示框“保存”按钮                                         */
/************************************************************************/
static void SaveButton()
{
    VarAdrSetInt(p_PP_MACHSET_ECAT_CHANGE_FLAG,0);//数据有修改标记还原
    SavePumpData();
    //返回物理按键
    if(switch_key>=GUI_KEY_FUN1 && switch_key<=GUI_KEY_FUN6)
    {
        switch_key = (switch_key-GUI_KEY_FUN1)+ KEY_FUN1;
    }
    else if(switch_key ==GUI_KEY_FUN7)
    {
        switch_key = KEY_FUN7;
    }
    else if(switch_key == GUI_KEY_MAINPAGE)
    {
        switch_key = KEY_MAIN;
    }
    else if(switch_key == GUI_KEY_FUN8)
    {
        switch_key = KEY_PC;
    }
    else if(switch_key == GUI_KEY_NEXTPAGE)//20191126.cyx
    {
        switch_key = KEY_NEXTPAGE;
    }
    SendKey(switch_key);
}
/************************************************************************/
/*  切页面信息提示框“取消”按钮                                        */
/************************************************************************/
static void CancelButton()
{
    VarAdrSetInt(p_PP_MACHSET_ECAT_CHANGE_FLAG,0);//数据有修改标记还原
    if(switch_key>=GUI_KEY_FUN1 && switch_key<=GUI_KEY_FUN6) //F1-F6
    {
        switch_key = (switch_key-GUI_KEY_FUN1)+ KEY_FUN1;
    }
    else if(switch_key ==GUI_KEY_FUN7)  //F7
    {
        switch_key = KEY_FUN7;
    }
    else if(switch_key == GUI_KEY_MAINPAGE)
    {
        switch_key = KEY_MAIN;
    }
    else if(switch_key == GUI_KEY_FUN8) //PC
    {
        switch_key = KEY_PC;
    }
    else if(switch_key == GUI_KEY_NEXTPAGE)//20191126.cyx
    {
        switch_key = KEY_NEXTPAGE;
    }
    SendKey(switch_key);
}

static void EcatKey(UI32 key,UI32 press)
{
    UI32 show_key;
    UI16 servo_num;
    servo_num = VarAdrToInt(p_PP_SERVO_SERVO_CURRENT_NUM);
    if(servo_num < 7)
    {
        show_key = GUI_KEY_FUN4;
    }
    else
    {
        show_key = GUI_KEY_FUN5;
    }
    if(key != show_key && VarAdrToInt(p_PP_MACHSET_ECAT_CHANGE_FLAG))
    {
        g_question.pOkfunc = SaveButton;
        g_question.pCancelfunc = CancelButton;
        switch_key = key;

        //20200109.cyx 非手动，自动登出时默认不保存，不弹框
        if((VarAdrToUI16(OPMODE_INDEX) != MODE_MANUAL) || VarAdrToInt(USER_STATE))
        {
            CancelButton();
        }
        else
        {
            ShowQuestion(VW_MSG_SAVEDATA,g_question);
        }
    }
}

static void PageShow(ServoPump1_PG* ppg)
{
    UI8 bit;
    int servo_num;
    BOOL isreadonly;
    UI32 bkcolor,textcolor;

    bit = 0;
    servo_num = VarAdrToInt(p_PP_SERVO_SERVO_CURRENT_NUM);
    if(servo_num < 7)
    {
        if(servo_num < 2)
        {
            WGTSetVisible(ppg->label_F4, FALSE);
        }
        else
        {
            WGTSetVisible(ppg->label_F4, TRUE);
        }
        WGTSetVisible(ppg->label_F5, FALSE);
        FkeyTab(ppg->tabpage1, GUI_KEY_FUN4);
    }
    else
    {
        WGTSetVisible(ppg->label_F4, FALSE);
        WGTSetVisible(ppg->label_F5, TRUE);
        FkeyTab(ppg->tabpage1, GUI_KEY_FUN5);
    }
    switch (servo_num) {
    case 1:
        bit = 1;
        break;
    case 2:
        bit = 3;
        break;
    case 3:
        bit = 7;
        break;
    case 4:
        bit = 15;
        break;
    case 5:
        bit = 31;
        break;
    default:
        bit = 63;
        break;
    }
    TabPageSetSubVisble(ppg->tabpage1, 0, getbit(bit, 0));
    TabPageSetSubVisble(ppg->tabpage1, 1, getbit(bit, 1));
    TabPageSetSubVisble(ppg->tabpage1, 2, getbit(bit, 2));
    TabPageSetSubVisble(ppg->tabpage1, 3, getbit(bit, 3));
    TabPageSetSubVisble(ppg->tabpage1, 4, getbit(bit, 4));
    TabPageSetSubVisble(ppg->tabpage1, 5, getbit(bit, 5));

    if(GetCurrentUserId() >= USER_MANUFACTURER)
    {
        isreadonly = FALSE;
        bkcolor = 0xFFFFFF;
        textcolor = 0x000000;
    }
    else
    {
        isreadonly = TRUE;
        bkcolor = 0xdedede;
        textcolor = 0x000000;
    }

    EditSetReadOnly(ppg->edit7, isreadonly);
    EditSetReadOnly(ppg->edit8, isreadonly);
    EditSetReadOnly(ppg->edit9, isreadonly);
    EditSetReadOnly(ppg->edit10, isreadonly);
    EditSetReadOnly(ppg->edit11, isreadonly);
    EditSetReadOnly(ppg->edit12, isreadonly);

    EditSetBKColor(ppg->edit7, bkcolor);
    EditSetBKColor(ppg->edit8, bkcolor);
    EditSetBKColor(ppg->edit9, bkcolor);
    EditSetBKColor(ppg->edit10, bkcolor);
    EditSetBKColor(ppg->edit11, bkcolor);
    EditSetBKColor(ppg->edit12, bkcolor);

    EditSetTextColor(ppg->edit7, textcolor);
    EditSetTextColor(ppg->edit8, textcolor);
    EditSetTextColor(ppg->edit9, textcolor);
    EditSetTextColor(ppg->edit10, textcolor);
    EditSetTextColor(ppg->edit11, textcolor);
    EditSetTextColor(ppg->edit12, textcolor);
}

static void PageChangeShow(ServoPump1_PG* ppg)
{
    UI16 cur_page = TabPageGetCurSel(ppg->tabpage1);

    if((VarAdrToInt(d_machine1_MACHSET_FL_HYDRAUIC)&0x80) || (cur_page==0))
    {
        WGTSetEnable(ppg->dropdown1, TRUE);
    }
    else
    {
        WGTSetEnable(ppg->dropdown1, FALSE);
    }

    WGTSetAttrByName(ppg->edit0, "text", d_Pump1_MACHSET_OIL_PUMP_DISPLACEMENT + cur_page*0x00100000);
    WGTSetAttrByName(ppg->edit1, "text", d_Pump1_MACHSET_PRESSAD_CH + cur_page*0x00100000);
    WGTSetAttrByName(ppg->edit2, "text", d_Pump1_MACHSET_MOTOR_SPEED_MAX + cur_page*0x00100000);
    WGTSetAttrByName(ppg->edit3, "text", d_Pump1_MACHSET_ENCODERESOLUTION + cur_page*0x00100000);
    WGTSetAttrByName(ppg->edit4, "text", d_Pump1_MACHSET_TORQUEMAX + cur_page*0x00100000);
    WGTSetAttrByName(ppg->edit5, "text", d_Pump1_MACHSET_BASEPRES + cur_page*0x00100000);
    WGTSetAttrByName(ppg->edit6, "text", d_Pump1_MACHSET_BASEFLOW + cur_page*0x00100000);
    WGTSetAttrByName(ppg->edit7, "text", d_Pump1_MACHSET_SPEED_ACC + cur_page*0x00100000);
    WGTSetAttrByName(ppg->edit8, "text", d_Pump1_MACHSET_SPEED_DEC + cur_page*0x00100000);
    WGTSetAttrByName(ppg->edit9, "text", d_Pump1_MACHSET_PRESS_ACC + cur_page*0x00100000);
    WGTSetAttrByName(ppg->edit10, "text", d_Pump1_MACHSET_PRESS_DEC + cur_page*0x00100000);
    WGTSetAttrByName(ppg->edit11, "text", d_Pump1_MACHSET_PRESS_ACC_FILTER + cur_page*0x00100000);
    WGTSetAttrByName(ppg->edit12, "text", d_Pump1_MACHSET_PRESS_DEC_FILTER + cur_page*0x00100000);
    WGTSetAttrByName(ppg->edit13, "text", d_Pump1_MACHSET_REVERSEFLOWMAX + cur_page*0x00100000);
    WGTSetAttrByName(ppg->edit14, "text", d_Pump1_MACHSET_TORQUEMAX_SLAVE_STOP + cur_page*0x00100000);
    WGTSetAttrByName(ppg->dropdown1, "value", d_Pump1_MACHSET_PRESSAD_TYPE + cur_page*0x00100000);
    WGTSetAttrByName(ppg->dropdown2, "value", d_Pump1_MACHSET_MODE_SLAVE_FOLLOW + cur_page*0x00100000);
}

static BOOL CheckChannelUsed(UI32 wUse)
{
   if(VarAdrToInt(SYS_FL_METER) & 0x0004)
   {
       if(VarAdrToInt(d_nozzle1_MACHSET_NZL_METERCH) == wUse)
       {
           return  TRUE;
       }
   }

   if(VarAdrToInt(d_inject1_MOLDSET_FL_INCACC) == 2)
   {
       if(VarAdrToInt(d_injstor1_MACHSET_INJSTOR_METERCH) == wUse)
       {
           return  TRUE;
       }
   }

   return  FALSE;
}

//最大反向速度 小于最小值则赋最小值
static void ChangeMaxReverseSpd()
{
    int len;
    int i;
    UI16 part_sub_id;
    UI16 part_id;
    UI16 data[1];

    if((VarAdrToInt(SYS_FL_HYDRAUIC) & 0x1) == 0)
    {
        for(i = 0; i < VarAdrToInt(p_PP_SERVO_SERVO_CURRENT_NUM); ++i)
        {
            if(VarAdrToInt(d_Pump1_MACHSET_REVERSEFLOWMAX + i*0x00100000) < 10)
            {
                VarAdrSetInt(d_Pump1_MACHSET_REVERSEFLOWMAX + i*0x00100000, 10);
                part_id = (UI8)PUMP_ID<<8 | (i+1)<<4;
                part_sub_id = part_id | MACHSET;
                len = GetSubLenByID(part_sub_id);
                ProtSendPartSub(part_id,MACHSET,len,GetSubByID(part_sub_id, NULL));
                data[0] = part_sub_id;
                ProtSysCtrl(COMM_SUBCMD_SERVOPUMP_WRITE,1,data);//save cmd
            }
        }

        SavePumpPage(1, VarAdrToInt(p_PP_SERVO_SERVO_CURRENT_NUM));
    }
}

static void CtlConnect()
{
}
static int ServoPump1OnInit(ServoPump1_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int ServoPump1OnShow(ServoPump1_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    old_page = 0;
    bPump1Change = FALSE;
    ImageSetIndex(ppg->image1, 1);
    TabPageSelPage(ppg->tabpage1, 0);
    VarAdrSetInt(p_PP_MACHSET_ECAT_CHANGE_FLAG,0);
    SetEcatCB(EcatKey);
    PageShow(ppg);
    PageChangeShow(ppg);
    ChangeMaxReverseSpd();//20210507
    return 0;
}
static int ServoPump1OnHide(ServoPump1_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 part_id, part_sub_id, len, i;
    if(bPump1Change)
    {
        bPump1Change = FALSE;
        for(i = 0; i < 8; ++i) //20191231 驱动目前最大只配8个 预留4个
        {
            part_id = (UI8)PUMP_ID<<8 | (i+1)<<4;
            part_sub_id = part_id | MACHSET;
            len = GetSubLenByID(part_sub_id);
            ProtReqPartSubData(part_id, MACHSET, 0, len);
        }
    }
    return 0;
}
static int ServoPump1OnUpdate(ServoPump1_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 cur_page = TabPageGetCurSel(ppg->tabpage1);
    if(old_page != cur_page)
    {
        ImageSetIndex(ppg->image1, cur_page + 1);
        PageChangeShow(ppg);
        old_page = cur_page;
    }
    return 0;
}
static int ServoPump1OnChange(ServoPump1_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 addr;
    PATTR pattr;

    pattr = (PATTR)msg_para;

    addr = OBJAttrGetAdr(pattr);

    if(pwgt == ppg->tabpage1)
        return 0;

    if(VarAdrToUI16(OPMODE_INDEX)!=MODE_MANUAL || VarAdrToInt(d_motor1_STATE_FL_MOTORSTATUS)!=0)
    {
        VarSendSaveSetIntByAdr(addr,msg_value);//原始值
        ShowMsg(VM_MSG_MANUAL_CUTMOTOR);
        return -1;
    }

    if(pwgt == ppg->button1)
    {
        VarAdrSetInt(p_PP_MACHSET_ECAT_CHANGE_FLAG,0);//数据有修改标记还原
        SavePumpData();
        PageWgtSetFocus(ppg->edit0);
    }
    if(addr >= d_Pump1_MACHSET_OIL_PUMP_DISPLACEMENT && addr <= d_Pump12_STATE_SERVO_COMM_ST)
    {
        bPump1Change = TRUE;
        VarAdrSetInt(p_PP_MACHSET_ECAT_CHANGE_FLAG,1);
    }

    if((addr&0xFF0FFFFF) == (d_Pump1_MACHSET_PRESSAD_CH&0xFF0FFFFF))
    {
        if(CheckChannelUsed(VarAdrToInt(addr)))
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//原始值
            ShowMsg(VW_MSG_METERCHANNELUSED);
            return -1;
        }
    }

    return 0;
}
