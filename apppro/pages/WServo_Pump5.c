#include "WServo_Pump5.h"
#include "typedef.h"
#include "pagefram.h"
#include "command.h"
#include "ctrlcmd.h"
#include "protcmd.h"
#include "questionDlg.h"
#include "key.h"
#include "database.h"
#include "privdef.h"
#include "keymod.h"
#include "information.h"

DEFINE_SERVO_PUMP5_MAP

CONNECT_SERVO_PUMP5_EVENT

#define PUMP_VIEWNUM 6//画面泵浦数量 20241017 chj 泵浦增加搭配6组
static QUESTION g_question;
static UI16 switch_key;
static BOOL bPump5Change = FALSE;

static void SavePumpData()
{
    int len;
    int i;
    UI16 part_sub_id;
    UI16 part_id;
    UI16 data[1];

    for(i = 0; i < PUMP_VIEWNUM; ++i)
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
    if(key != GUI_KEY_FUN7 && VarAdrToInt(p_PP_MACHSET_ECAT_CHANGE_FLAG))
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


static void CtlConnect()
{
}
static int Servo_Pump5OnInit(Servo_Pump5_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Servo_Pump5OnShow(Servo_Pump5_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    bPump5Change = FALSE;
    VarAdrSetInt(p_PP_MACHSET_ECAT_CHANGE_FLAG,0);
    //20220923 dyl 根据伺服当前个数显示油泵数量
    int servo_num;
    servo_num = VarAdrToInt(p_PP_SERVO_SERVO_CURRENT_NUM);
    switch (servo_num) {
    case 1:
        WGTSetVisible(ppg->label51, FALSE);//油泵6转速 20241017 chj 泵浦增加搭配6组
        WGTSetVisible(ppg->edit11, FALSE);
        WGTSetVisible(ppg->label53, FALSE);
        WGTSetVisible(ppg->label55, FALSE);//油泵6排量
        WGTSetVisible(ppg->edit13, FALSE);
        WGTSetVisible(ppg->label57, FALSE);
        WGTSetVisible(ppg->label50, FALSE);//油泵5转速
        WGTSetVisible(ppg->edit10, FALSE);
        WGTSetVisible(ppg->label52, FALSE);
        WGTSetVisible(ppg->label54, FALSE);//油泵5排量
        WGTSetVisible(ppg->edit12, FALSE);
        WGTSetVisible(ppg->label56, FALSE);
        WGTSetVisible(ppg->label40, FALSE);//油泵4转速
        WGTSetVisible(ppg->edit5, FALSE);
        WGTSetVisible(ppg->label41, FALSE);
        WGTSetVisible(ppg->label45, FALSE);//油泵4排量
        WGTSetVisible(ppg->edit9, FALSE);
        WGTSetVisible(ppg->label49, FALSE);
        WGTSetVisible(ppg->label38, FALSE);//油泵3转速
        WGTSetVisible(ppg->edit4, FALSE);
        WGTSetVisible(ppg->label39, FALSE);
        WGTSetVisible(ppg->label44, FALSE);//油泵3排量
        WGTSetVisible(ppg->edit8, FALSE);
        WGTSetVisible(ppg->label48, FALSE);
        WGTSetVisible(ppg->label36, FALSE);//油泵2转速
        WGTSetVisible(ppg->edit3, FALSE);
        WGTSetVisible(ppg->label37, FALSE);
        WGTSetVisible(ppg->label43, FALSE);//油泵2排量
        WGTSetVisible(ppg->edit7, FALSE);
        WGTSetVisible(ppg->label47, FALSE);
        WGTSetVisible(ppg->label20, TRUE);//油泵1转速
        WGTSetVisible(ppg->edit2, TRUE);
        WGTSetVisible(ppg->label31, TRUE);
        WGTSetVisible(ppg->label42, TRUE);//油泵1排量
        WGTSetVisible(ppg->edit6, TRUE);
        WGTSetVisible(ppg->label46, TRUE);
        break;
    case 2:
        WGTSetVisible(ppg->label51, FALSE);//油泵6转速 20241017 chj 泵浦增加搭配6组
        WGTSetVisible(ppg->edit11, FALSE);
        WGTSetVisible(ppg->label53, FALSE);
        WGTSetVisible(ppg->label55, FALSE);//油泵6排量
        WGTSetVisible(ppg->edit13, FALSE);
        WGTSetVisible(ppg->label57, FALSE);
        WGTSetVisible(ppg->label50, FALSE);//油泵5转速
        WGTSetVisible(ppg->edit10, FALSE);
        WGTSetVisible(ppg->label52, FALSE);
        WGTSetVisible(ppg->label54, FALSE);//油泵5排量
        WGTSetVisible(ppg->edit12, FALSE);
        WGTSetVisible(ppg->label56, FALSE);
        WGTSetVisible(ppg->label40, FALSE);//油泵4转速
        WGTSetVisible(ppg->edit5, FALSE);
        WGTSetVisible(ppg->label41, FALSE);
        WGTSetVisible(ppg->label45, FALSE);//油泵4排量
        WGTSetVisible(ppg->edit9, FALSE);
        WGTSetVisible(ppg->label49, FALSE);
        WGTSetVisible(ppg->label38, FALSE);//油泵3转速
        WGTSetVisible(ppg->edit4, FALSE);
        WGTSetVisible(ppg->label39, FALSE);
        WGTSetVisible(ppg->label44, FALSE);//油泵3排量
        WGTSetVisible(ppg->edit8, FALSE);
        WGTSetVisible(ppg->label48, FALSE);
        WGTSetVisible(ppg->label36, TRUE);//油泵2转速
        WGTSetVisible(ppg->edit3, TRUE);
        WGTSetVisible(ppg->label37, TRUE);
        WGTSetVisible(ppg->label43, TRUE);//油泵2排量
        WGTSetVisible(ppg->edit7, TRUE);
        WGTSetVisible(ppg->label47, TRUE);
        WGTSetVisible(ppg->label20, TRUE);//油泵1转速
        WGTSetVisible(ppg->edit2, TRUE);
        WGTSetVisible(ppg->label31, TRUE);
        WGTSetVisible(ppg->label42, TRUE);//油泵1排量
        WGTSetVisible(ppg->edit6, TRUE);
        WGTSetVisible(ppg->label46, TRUE);
        break;
    case 3:
        WGTSetVisible(ppg->label51, FALSE);//油泵6转速 20241017 chj 泵浦增加搭配6组
        WGTSetVisible(ppg->edit11, FALSE);
        WGTSetVisible(ppg->label53, FALSE);
        WGTSetVisible(ppg->label55, FALSE);//油泵6排量
        WGTSetVisible(ppg->edit13, FALSE);
        WGTSetVisible(ppg->label57, FALSE);
        WGTSetVisible(ppg->label50, FALSE);//油泵5转速
        WGTSetVisible(ppg->edit10, FALSE);
        WGTSetVisible(ppg->label52, FALSE);
        WGTSetVisible(ppg->label54, FALSE);//油泵5排量
        WGTSetVisible(ppg->edit12, FALSE);
        WGTSetVisible(ppg->label56, FALSE);
        WGTSetVisible(ppg->label40, FALSE);//油泵4转速
        WGTSetVisible(ppg->edit5, FALSE);
        WGTSetVisible(ppg->label41, FALSE);
        WGTSetVisible(ppg->label45, FALSE);//油泵4排量
        WGTSetVisible(ppg->edit9, FALSE);
        WGTSetVisible(ppg->label49, FALSE);
        WGTSetVisible(ppg->label38, TRUE);//油泵3转速
        WGTSetVisible(ppg->edit4, TRUE);
        WGTSetVisible(ppg->label39, TRUE);
        WGTSetVisible(ppg->label44, TRUE);//油泵3排量
        WGTSetVisible(ppg->edit8, TRUE);
        WGTSetVisible(ppg->label48, TRUE);
        WGTSetVisible(ppg->label36, TRUE);//油泵2转速
        WGTSetVisible(ppg->edit3, TRUE);
        WGTSetVisible(ppg->label37, TRUE);
        WGTSetVisible(ppg->label43, TRUE);//油泵2排量
        WGTSetVisible(ppg->edit7, TRUE);
        WGTSetVisible(ppg->label47, TRUE);
        WGTSetVisible(ppg->label20, TRUE);//油泵1转速
        WGTSetVisible(ppg->edit2, TRUE);
        WGTSetVisible(ppg->label31, TRUE);
        WGTSetVisible(ppg->label42, TRUE);//油泵1排量
        WGTSetVisible(ppg->edit6, TRUE);
        WGTSetVisible(ppg->label46, TRUE);
        break;
    case 4:
        WGTSetVisible(ppg->label51, FALSE);//油泵6转速 20241017 chj 泵浦增加搭配6组
        WGTSetVisible(ppg->edit11, FALSE);
        WGTSetVisible(ppg->label53, FALSE);
        WGTSetVisible(ppg->label55, FALSE);//油泵6排量
        WGTSetVisible(ppg->edit13, FALSE);
        WGTSetVisible(ppg->label57, FALSE);
        WGTSetVisible(ppg->label50, FALSE);//油泵5转速
        WGTSetVisible(ppg->edit10, FALSE);
        WGTSetVisible(ppg->label52, FALSE);
        WGTSetVisible(ppg->label54, FALSE);//油泵5排量
        WGTSetVisible(ppg->edit12, FALSE);
        WGTSetVisible(ppg->label56, FALSE);
        WGTSetVisible(ppg->label40, TRUE);//油泵4转速
        WGTSetVisible(ppg->edit5, TRUE);
        WGTSetVisible(ppg->label41, TRUE);
        WGTSetVisible(ppg->label45, TRUE);//油泵4排量
        WGTSetVisible(ppg->edit9, TRUE);
        WGTSetVisible(ppg->label49, TRUE);
        WGTSetVisible(ppg->label38, TRUE);//油泵3转速
        WGTSetVisible(ppg->edit4, TRUE);
        WGTSetVisible(ppg->label39, TRUE);
        WGTSetVisible(ppg->label44, TRUE);//油泵3排量
        WGTSetVisible(ppg->edit8, TRUE);
        WGTSetVisible(ppg->label48, TRUE);
        WGTSetVisible(ppg->label36, TRUE);//油泵2转速
        WGTSetVisible(ppg->edit3, TRUE);
        WGTSetVisible(ppg->label37, TRUE);
        WGTSetVisible(ppg->label43, TRUE);//油泵2排量
        WGTSetVisible(ppg->edit7, TRUE);
        WGTSetVisible(ppg->label47, TRUE);
        WGTSetVisible(ppg->label20, TRUE);//油泵1转速
        WGTSetVisible(ppg->edit2, TRUE);
        WGTSetVisible(ppg->label31, TRUE);
        WGTSetVisible(ppg->label42, TRUE);//油泵1排量
        WGTSetVisible(ppg->edit6, TRUE);
        WGTSetVisible(ppg->label46, TRUE);
        break;
    case 5:
        WGTSetVisible(ppg->label51, FALSE);//油泵6转速 20241017 chj 泵浦增加搭配6组
        WGTSetVisible(ppg->edit11, FALSE);
        WGTSetVisible(ppg->label53, FALSE);
        WGTSetVisible(ppg->label55, FALSE);//油泵6排量
        WGTSetVisible(ppg->edit13, FALSE);
        WGTSetVisible(ppg->label57, FALSE);
        WGTSetVisible(ppg->label50, TRUE);//油泵5转速
        WGTSetVisible(ppg->edit10, TRUE);
        WGTSetVisible(ppg->label52, TRUE);
        WGTSetVisible(ppg->label54, TRUE);//油泵5排量
        WGTSetVisible(ppg->edit12, TRUE);
        WGTSetVisible(ppg->label56, TRUE);
        WGTSetVisible(ppg->label40, TRUE);//油泵4转速
        WGTSetVisible(ppg->edit5, TRUE);
        WGTSetVisible(ppg->label41, TRUE);
        WGTSetVisible(ppg->label45, TRUE);//油泵4排量
        WGTSetVisible(ppg->edit9, TRUE);
        WGTSetVisible(ppg->label49, TRUE);
        WGTSetVisible(ppg->label38, TRUE);//油泵3转速
        WGTSetVisible(ppg->edit4, TRUE);
        WGTSetVisible(ppg->label39, TRUE);
        WGTSetVisible(ppg->label44, TRUE);//油泵3排量
        WGTSetVisible(ppg->edit8, TRUE);
        WGTSetVisible(ppg->label48, TRUE);
        WGTSetVisible(ppg->label36, TRUE);//油泵2转速
        WGTSetVisible(ppg->edit3, TRUE);
        WGTSetVisible(ppg->label37, TRUE);
        WGTSetVisible(ppg->label43, TRUE);//油泵2排量
        WGTSetVisible(ppg->edit7, TRUE);
        WGTSetVisible(ppg->label47, TRUE);
        WGTSetVisible(ppg->label20, TRUE);//油泵1转速
        WGTSetVisible(ppg->edit2, TRUE);
        WGTSetVisible(ppg->label31, TRUE);
        WGTSetVisible(ppg->label42, TRUE);//油泵1排量
        WGTSetVisible(ppg->edit6, TRUE);
        WGTSetVisible(ppg->label46, TRUE);
        break;
    case 6:
        WGTSetVisible(ppg->label51, TRUE);//油泵6转速 20241017 chj 泵浦增加搭配6组
        WGTSetVisible(ppg->edit11, TRUE);
        WGTSetVisible(ppg->label53, TRUE);
        WGTSetVisible(ppg->label55, TRUE);//油泵6排量
        WGTSetVisible(ppg->edit13, TRUE);
        WGTSetVisible(ppg->label57, TRUE);
        WGTSetVisible(ppg->label50, TRUE);//油泵5转速
        WGTSetVisible(ppg->edit10, TRUE);
        WGTSetVisible(ppg->label52, TRUE);
        WGTSetVisible(ppg->label54, TRUE);//油泵5排量
        WGTSetVisible(ppg->edit12, TRUE);
        WGTSetVisible(ppg->label56, TRUE);
        WGTSetVisible(ppg->label40, TRUE);//油泵4转速
        WGTSetVisible(ppg->edit5, TRUE);
        WGTSetVisible(ppg->label41, TRUE);
        WGTSetVisible(ppg->label45, TRUE);//油泵4排量
        WGTSetVisible(ppg->edit9, TRUE);
        WGTSetVisible(ppg->label49, TRUE);
        WGTSetVisible(ppg->label38, TRUE);//油泵3转速
        WGTSetVisible(ppg->edit4, TRUE);
        WGTSetVisible(ppg->label39, TRUE);
        WGTSetVisible(ppg->label44, TRUE);//油泵3排量
        WGTSetVisible(ppg->edit8, TRUE);
        WGTSetVisible(ppg->label48, TRUE);
        WGTSetVisible(ppg->label36, TRUE);//油泵2转速
        WGTSetVisible(ppg->edit3, TRUE);
        WGTSetVisible(ppg->label37, TRUE);
        WGTSetVisible(ppg->label43, TRUE);//油泵2排量
        WGTSetVisible(ppg->edit7, TRUE);
        WGTSetVisible(ppg->label47, TRUE);
        WGTSetVisible(ppg->label20, TRUE);//油泵1转速
        WGTSetVisible(ppg->edit2, TRUE);
        WGTSetVisible(ppg->label31, TRUE);
        WGTSetVisible(ppg->label42, TRUE);//油泵1排量
        WGTSetVisible(ppg->edit6, TRUE);
        WGTSetVisible(ppg->label46, TRUE);
        break;
    default:
        WGTSetVisible(ppg->label51, FALSE);//油泵6转速 20241017 chj 泵浦增加搭配6组
        WGTSetVisible(ppg->edit11, FALSE);
        WGTSetVisible(ppg->label53, FALSE);
        WGTSetVisible(ppg->label55, FALSE);//油泵6排量
        WGTSetVisible(ppg->edit13, FALSE);
        WGTSetVisible(ppg->label57, FALSE);
        WGTSetVisible(ppg->label50, FALSE);//油泵5转速
        WGTSetVisible(ppg->edit10, FALSE);
        WGTSetVisible(ppg->label52, FALSE);
        WGTSetVisible(ppg->label54, FALSE);//油泵5排量
        WGTSetVisible(ppg->edit12, FALSE);
        WGTSetVisible(ppg->label56, FALSE);
        WGTSetVisible(ppg->label40, FALSE);//油泵4转速
        WGTSetVisible(ppg->edit5, FALSE);
        WGTSetVisible(ppg->label41, FALSE);
        WGTSetVisible(ppg->label45, FALSE);//油泵4排量
        WGTSetVisible(ppg->edit9, FALSE);
        WGTSetVisible(ppg->label49, FALSE);
        WGTSetVisible(ppg->label38, FALSE);//油泵3转速
        WGTSetVisible(ppg->edit4, FALSE);
        WGTSetVisible(ppg->label39, FALSE);
        WGTSetVisible(ppg->label44, FALSE);//油泵3排量
        WGTSetVisible(ppg->edit8, FALSE);
        WGTSetVisible(ppg->label48, FALSE);
        WGTSetVisible(ppg->label36, FALSE);//油泵2转速
        WGTSetVisible(ppg->edit3, FALSE);
        WGTSetVisible(ppg->label37, FALSE);
        WGTSetVisible(ppg->label43, FALSE);//油泵2排量
        WGTSetVisible(ppg->edit7, FALSE);
        WGTSetVisible(ppg->label47, FALSE);
        WGTSetVisible(ppg->label20, FALSE);//油泵1转速
        WGTSetVisible(ppg->edit2, FALSE);
        WGTSetVisible(ppg->label31, FALSE);
        WGTSetVisible(ppg->label42, FALSE);//油泵1排量
        WGTSetVisible(ppg->edit6, FALSE);
        WGTSetVisible(ppg->label46, FALSE);
        break;
    }

    SetEcatCB(EcatKey);

    return 0;
}
static int Servo_Pump5OnHide(Servo_Pump5_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 part_id, part_sub_id, len, i;
    if(bPump5Change)
    {
        bPump5Change = FALSE;
        for(i = 0; i < PUMP_VIEWNUM; ++i)
        {
            part_id = (UI8)PUMP_ID<<8 | (i+1)<<4;
            part_sub_id = part_id | MACHSET;
            len = GetSubLenByID(part_sub_id);
            ProtReqPartSubData(part_id, MACHSET, 0, len);
        }
    }
    return 0;
}
static int Servo_Pump5OnUpdate(Servo_Pump5_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Servo_Pump5OnChange(Servo_Pump5_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 addr;
    PATTR pattr;

    pattr = (PATTR)msg_para;

    addr = OBJAttrGetAdr(pattr);

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
        PageWgtSetFocus(ppg->edit2);
    }
    if(addr >= d_Pump1_MACHSET_OIL_PUMP_DISPLACEMENT && addr <= d_Pump12_STATE_SERVO_COMM_ST)
    {
        bPump5Change = TRUE;
        VarAdrSetInt(p_PP_MACHSET_ECAT_CHANGE_FLAG,1);
    }

    return 0;
}
