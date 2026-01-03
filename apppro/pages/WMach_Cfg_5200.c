#include "WMach_Cfg_5200.h"
#include "typedef.h"
#include "pagefram.h"
#include "database.h"
#include "machcfg.h"
#include "information.h"
#include "command.h"
#include "kjump.h"
#include "questionDlg.h"
#include "keymod.h"
#include "key.h"
#include "oprintf.h"
#include "editwgt.h"
#include "pushtab.h"
#include "ctrlcmd.h"
#include "labelwgt.h"
#include "texttbl.h"
#include "stdio.h"
#include "panelrec.h"
#include "usermod.h"//20210914 dyl touch

DEFINE_MACH_CFG_5200_MAP

CONNECT_MACH_CFG_5200_EVENT

#define TMP_CODEPASSWORD    TMPVAL(100) //code码修改密码
#define TMP_MACHINETYPE     TMPVAL(101) //机器类型选择
#define TMP_MACHINETYPEMG   TMPVAL(201) //机器类型选择//20241205 chj 金鹰增加麦格米特模式
#define TMP_MAINRESETPASW   TMPVAL(102) //主机重置密码

#define STAD_NAME(NUM,pbuff,size)		joinStrAndNum("stAD",NUM,pbuff,size)
#define STADUNIT_NAME(NUM,pbuff,size)	joinStrAndNum("stUnit",NUM,pbuff,size)//20211210 dyl 根据液压通道号、座台通道号设置AD名称


static QUESTION g_question;

static BOOL bMachCfg5200Change=FALSE;
extern BOOL g_bMachCfgReset;//重置标记
extern BOOL bret;

#define PUSH_TEMP_ADDR    TMPSTR(34) //十字头型号对应得临时地址
static char cpushbak[8] = "";

//20200408.cyx
static void SetPushWndTxt()
{
    UI16 wValue1 = VarAdrToInt(d_machine1_MACHSET_FL_reserve_3)/100;
    UI16 wValue2 = VarAdrToInt(d_machine1_MACHSET_FL_reserve_3)%100;
    UI16 g_pushcode = VarAdrToInt(d_machine1_MACHSET_FL_MACHINEMODEL);

    if(VarAdrToInt(d_machine1_MACHSET_FL_reserve_3) > 100)
    {
        sprintf(cpushbak, "%d%c%c", g_pushcode, wValue1, wValue2);
    }
    else
    {
        if(VarAdrToInt(d_machine1_MACHSET_FL_reserve_3) == 0)
        {
            sprintf(cpushbak, "%d", g_pushcode);
        }
        else
        {
            sprintf(cpushbak, "%d%c", g_pushcode, wValue2);
        }
    }

    VarAdrSetStr(PUSH_TEMP_ADDR, cpushbak);
}

static BOOL CheckLegal()
{
    int i;
    char list[8];

    UI16 nlen = strlen(VarAdrToStr(PUSH_TEMP_ADDR));
    UI16 wNum = 0;
    UI32 wPushNum = 0;

    if(nlen > 8) return false;

    strcpy(list, VarAdrToStr(PUSH_TEMP_ADDR));

    for(i = 0; i < nlen; i++)
    {
        if(list[i]>='A'&&list[i]<='z')
        {
            break;
        }
        wNum = i+1;
        wPushNum = wPushNum*10+list[i]-'0';
    }

    if((nlen-wNum)>2)
    {
        oprintf("too much letter\n");
        return false;
    }
    if(wPushNum>65000)
    {
        oprintf("push code >65000\n");
        return false;
    }
    if(((nlen-wNum)!=0)&&(wPushNum==0))
    {
        oprintf("letter num >0&wPushNum=0\n");
        return false;
    }
    if(nlen==0)
    {
        oprintf("len=0\n");
        return false;
    }

    for(i = 0; i < nlen; i++)
    {
        if(list[i] >= '0' && list[i] <= 'Z')
        {

        }
        else
        {
            return false;
        }
    }
    strcpy(cpushbak, list);
    return true;
}

static void GetPushSPNum()
{
    int i;
    char list[8];
    UI16 nlen = strlen(VarAdrToStr(PUSH_TEMP_ADDR));
    UI16 wNum = 0;
    UI16 wPushNum = 0;
    UI32 wPushNumSp = 0;
    BOOL needtrans = false;

    if(nlen > 8) return;

    strcpy(list, VarAdrToStr(PUSH_TEMP_ADDR));
    for(i = 0; i < nlen; i++)
    {
        if(list[i]>='A'&&list[i]<='z')
        {
            needtrans=true;
            break;
        }
        wNum = i+1;
        wPushNum = wPushNum*10+list[i]-'0';
    }
    if(needtrans)
    {
        for(i = wNum; i < nlen; i++)
        {
            wPushNumSp = wPushNumSp*100+list[i];
        }
    }

    VarAdrSetInt(d_machine1_MACHSET_FL_MACHINEMODEL,wPushNum);
    VarAdrSetInt(d_machine1_MACHSET_FL_reserve_3,wPushNumSp);
}

/************************************************************************/
/* code码只读       20181019hz                                           */
/************************************************************************/
static void CodeReadOnly(Mach_Cfg_5200_PG* ppg)
{
    BOOL bRead = FALSE;
    UI32 bkcolor = 0xffffff;
    UI32 textcolor = 0xffffff;

    if (VarAdrToInt(TMP_CODEPASSWORD) == 4321)
    {
        bRead = FALSE;
        bkcolor = 0xffffff;
        textcolor = GUI_BLACK;
    }
    else
    {
        bRead = TRUE;
        bkcolor = LIGHT_GREEN;
        textcolor = GUI_WHITE;
    }

    WGTSetEnable(ppg->button44, !bRead);//读U盘操作需要通过密码4321才能操作

    EditSetReadOnly(ppg->edit18, bRead);
    EditSetReadOnly(ppg->edit22, bRead);
    EditSetReadOnly(ppg->edit26, bRead);
    EditSetReadOnly(ppg->edit30, bRead);
    EditSetReadOnly(ppg->edit34, bRead);
    EditSetReadOnly(ppg->edit38, bRead);
    EditSetReadOnly(ppg->edit42, bRead);
    EditSetReadOnly(ppg->edit46, bRead);
    EditSetReadOnly(ppg->edit19, bRead);
    EditSetReadOnly(ppg->edit23, bRead);
    EditSetReadOnly(ppg->edit27, bRead);
    EditSetReadOnly(ppg->edit31, bRead);
    EditSetReadOnly(ppg->edit35, bRead);
    EditSetReadOnly(ppg->edit39, bRead);
    EditSetReadOnly(ppg->edit20, bRead);
    EditSetReadOnly(ppg->edit24, bRead);
    EditSetReadOnly(ppg->edit28, bRead);
    EditSetReadOnly(ppg->edit32, bRead);
    EditSetReadOnly(ppg->edit36, bRead);
    EditSetReadOnly(ppg->edit40, bRead);
    EditSetReadOnly(ppg->edit44, bRead);
    EditSetReadOnly(ppg->edit48, bRead);
    EditSetReadOnly(ppg->edit21, bRead);
    EditSetReadOnly(ppg->edit25, bRead);
    EditSetReadOnly(ppg->edit29, bRead);
    EditSetReadOnly(ppg->edit33, bRead);
    EditSetReadOnly(ppg->edit70, bRead);

    EditSetBKColor(ppg->edit18, bkcolor);
    EditSetBKColor(ppg->edit22, bkcolor);
    EditSetBKColor(ppg->edit26, bkcolor);
    EditSetBKColor(ppg->edit30, bkcolor);
    EditSetBKColor(ppg->edit34, bkcolor);
    EditSetBKColor(ppg->edit38, bkcolor);
    EditSetBKColor(ppg->edit42, bkcolor);
    EditSetBKColor(ppg->edit46, bkcolor);
    EditSetBKColor(ppg->edit19, bkcolor);
    EditSetBKColor(ppg->edit23, bkcolor);
    EditSetBKColor(ppg->edit27, bkcolor);
    EditSetBKColor(ppg->edit31, bkcolor);
    EditSetBKColor(ppg->edit35, bkcolor);
    EditSetBKColor(ppg->edit39, bkcolor);
    EditSetBKColor(ppg->edit20, bkcolor);
    EditSetBKColor(ppg->edit24, bkcolor);
    EditSetBKColor(ppg->edit28, bkcolor);
    EditSetBKColor(ppg->edit32, bkcolor);
    EditSetBKColor(ppg->edit36, bkcolor);
    EditSetBKColor(ppg->edit40, bkcolor);
    EditSetBKColor(ppg->edit44, bkcolor);
    EditSetBKColor(ppg->edit48, bkcolor);
    EditSetBKColor(ppg->edit21, bkcolor);
    EditSetBKColor(ppg->edit25, bkcolor);
    EditSetBKColor(ppg->edit29, bkcolor);
    EditSetBKColor(ppg->edit33, bkcolor);
    EditSetBKColor(ppg->edit70, bkcolor);

    EditSetTextColor(ppg->edit18, textcolor);
    EditSetTextColor(ppg->edit22, textcolor);
    EditSetTextColor(ppg->edit26, textcolor);
    EditSetTextColor(ppg->edit30, textcolor);
    EditSetTextColor(ppg->edit34, textcolor);
    EditSetTextColor(ppg->edit38, textcolor);
    EditSetTextColor(ppg->edit42, textcolor);
    EditSetTextColor(ppg->edit46, textcolor);
    EditSetTextColor(ppg->edit19, textcolor);
    EditSetTextColor(ppg->edit23, textcolor);
    EditSetTextColor(ppg->edit27, textcolor);
    EditSetTextColor(ppg->edit31, textcolor);
    EditSetTextColor(ppg->edit35, textcolor);
    EditSetTextColor(ppg->edit39, textcolor);
    EditSetTextColor(ppg->edit20, textcolor);
    EditSetTextColor(ppg->edit24, textcolor);
    EditSetTextColor(ppg->edit28, textcolor);
    EditSetTextColor(ppg->edit32, textcolor);
    EditSetTextColor(ppg->edit36, textcolor);
    EditSetTextColor(ppg->edit40, textcolor);
    EditSetTextColor(ppg->edit44, textcolor);
    EditSetTextColor(ppg->edit48, textcolor);
    EditSetTextColor(ppg->edit21, textcolor);
    EditSetTextColor(ppg->edit25, textcolor);
    EditSetTextColor(ppg->edit29, textcolor);
    EditSetTextColor(ppg->edit33, textcolor);
    EditSetTextColor(ppg->edit70, textcolor);
}

static void NameChangeAndShow()  //20200325
{
    PWGT pwgt;
    char buff[10];
    UI16 wNozzleValue = VarAdrToInt(d_nozzle1_MACHSET_NZL_METERCH);

//    LabelSetText(mach_cfg_5200_pg.stAD4,GetTextTran(TEXT_AD_ADHYDR1));
//    LabelSetText(mach_cfg_5200_pg.stUnit4,"(bar)");
//    LabelSetText(mach_cfg_5200_pg.stAD5,"AD5");
//    LabelSetText(mach_cfg_5200_pg.stAD6,"AD6");
//    LabelSetText(mach_cfg_5200_pg.stAD7,"AD7");

//    if (VarAdrToInt(SYS_FL_METER) & 0x0004)
//    {
//        if (IV5200 || IV5300)
//        {
//            if((wNozzleValue>=4)&&(wNozzleValue<=7))  //4-7路做座台切换
//            {
//                pwgt = PageFramCurWgtByName(STAD_NAME(wNozzleValue,buff,10));
//                LabelSetText(pwgt,GetTextTran(TEXT_AD_NOZZLE));
//                if(wNozzleValue == 4)
//                {
//                    LabelSetText(mach_cfg_5200_pg.stUnit4,"(mm)");
//                }
//            }
//        }
//        else
//        {
//            if(wNozzleValue == 4)  //4做座台切换
//            {
//                LabelSetText(mach_cfg_5200_pg.stAD4,GetTextTran(TEXT_AD_NOZZLE));
//                LabelSetText(mach_cfg_5200_pg.stUnit4,"(mm)");
//            }
//        }
//    }

    //20211210 dyl 根据液压通道号、座台通道号设置AD名称
    LabelSetText(mach_cfg_5200_pg.stAD4,"AD4");
    LabelSetText(mach_cfg_5200_pg.stAD5,"AD5");
    LabelSetText(mach_cfg_5200_pg.stAD6,"AD6");
    LabelSetText(mach_cfg_5200_pg.stAD7,"AD7");
    LabelSetText(mach_cfg_5200_pg.stAD8,"AD8");
    LabelSetText(mach_cfg_5200_pg.stUnit4,"(mm)");
    LabelSetText(mach_cfg_5200_pg.stUnit5,"(mm)");
    LabelSetText(mach_cfg_5200_pg.stUnit6,"(mm)");
    LabelSetText(mach_cfg_5200_pg.stUnit7,"(mm)");
    LabelSetText(mach_cfg_5200_pg.stUnit8,"(mm)");
    UI16 wChanel_Sensor1 = VarAdrToInt(d_Pump1_MACHSET_PRESSAD_CH);
    UI16 wChanel_Sensor2 = VarAdrToInt(d_Pump2_MACHSET_PRESSAD_CH);
    if (VarAdrToInt(SYS_FL_METER) & 0x0004)
    {
        if((wNozzleValue>=4)&&(wNozzleValue<=g_mainresource_current.nADNum))  //4-7路做座台切换
        {
            pwgt = PageFramCurWgtByName(STAD_NAME(wNozzleValue,buff,10));
            LabelSetText(pwgt,GetTextTran(TEXT_AD_NOZZLE));
        }
    }
    if(VarAdrToUI16(SYS_FL_MACH_CODE1)&0x04)//20220928 dyl ECAT模式
    {
        if ((wChanel_Sensor1>=4)&&(wChanel_Sensor1 <= g_mainresource_current.nADNum))
        {
            pwgt = PageFramCurWgtByName(STAD_NAME(wChanel_Sensor1,buff,10));
            LabelSetText(pwgt,GetTextTran(TEXT_AD_SENSOR1));
            pwgt = PageFramCurWgtByName(STADUNIT_NAME(wChanel_Sensor1,buff,10));
            LabelSetText(pwgt,"(bar)");
        }
        if ((wChanel_Sensor2>=4)&&(wChanel_Sensor2 <= g_mainresource_current.nADNum)
                &&(wChanel_Sensor2 != wChanel_Sensor1))
        {
            pwgt = PageFramCurWgtByName(STAD_NAME(wChanel_Sensor2,buff,10));
            LabelSetText(pwgt,GetTextTran(TEXT_AD_SENSOR2));
            pwgt = PageFramCurWgtByName(STADUNIT_NAME(wChanel_Sensor2,buff,10));
            LabelSetText(pwgt,"(bar)");
        }
    }

}

/************************************************************************/
/* 机器类型选择                                                         */
/************************************************************************/
static void MachTypeSel()
{
    UI16 value;

    //20241205 chj 金鹰增加麦格米特模式
    if((VarAdrToInt(d_machine1_MACHSET_FL_CUSTOMID)&0xFF00) == 0x6400)
    {
        value = VarAdrToUI16(TMP_MACHINETYPEMG);
        if(value == 0)
        {
            VarAdrSetInt(SYS_FL_MACH_CODE0,VarAdrToUI16(SYS_FL_MACH_CODE0)&(~0x0004));
            VarAdrSetInt(SYS_FL_MACH_CODE1,VarAdrToUI16(SYS_FL_MACH_CODE1)&(~0x0004));
            VarAdrSetInt(SYS_FL_MACH_CODE54,VarAdrToUI16(SYS_FL_MACH_CODE54)&(~0x1020));
            VarAdrSetInt(SYS_FL_MACH_CODE54,VarAdrToUI16(SYS_FL_MACH_CODE54)&(~0x100));
        }
        else if(value == 1)
        {
            VarAdrSetInt(SYS_FL_MACH_CODE0,VarAdrToUI16(SYS_FL_MACH_CODE0)|(0x0004));
            VarAdrSetInt(SYS_FL_MACH_CODE1,VarAdrToUI16(SYS_FL_MACH_CODE1)|(0x0004));
            VarAdrSetInt(SYS_FL_MACH_CODE54,VarAdrToUI16(SYS_FL_MACH_CODE54)&(~0x1020));
            VarAdrSetInt(SYS_FL_MACH_CODE54,VarAdrToUI16(SYS_FL_MACH_CODE54)&(~0x100));
        }
        else if(value == 2)
        {
            VarAdrSetInt(SYS_FL_MACH_CODE0,VarAdrToUI16(SYS_FL_MACH_CODE0)&(~0x0004));
            VarAdrSetInt(SYS_FL_MACH_CODE1,VarAdrToUI16(SYS_FL_MACH_CODE1)&(~0x0004));
            VarAdrSetInt(SYS_FL_MACH_CODE54,VarAdrToUI16(SYS_FL_MACH_CODE54)|(0x1020));
            VarAdrSetInt(SYS_FL_MACH_CODE54,VarAdrToUI16(SYS_FL_MACH_CODE54)&(~0x100));
        }
        else if(value == 3)
        {
            VarAdrSetInt(SYS_FL_MACH_CODE0,VarAdrToUI16(SYS_FL_MACH_CODE0)&(~0x0004));
            VarAdrSetInt(SYS_FL_MACH_CODE1,VarAdrToUI16(SYS_FL_MACH_CODE1)&(~0x0004));
            VarAdrSetInt(SYS_FL_MACH_CODE54,VarAdrToUI16(SYS_FL_MACH_CODE54)|(0x1020));
            VarAdrSetInt(SYS_FL_MACH_CODE54,VarAdrToUI16(SYS_FL_MACH_CODE54)|(0x100));
        }
    }
    else
    {
        value = VarAdrToUI16(TMP_MACHINETYPE);
        if(value == 0)
        {
            VarAdrSetInt(SYS_FL_MACH_CODE0,VarAdrToUI16(SYS_FL_MACH_CODE0)&(~0x0004));
            VarAdrSetInt(SYS_FL_MACH_CODE1,VarAdrToUI16(SYS_FL_MACH_CODE1)&(~0x0004));
            VarAdrSetInt(SYS_FL_MACH_CODE54,VarAdrToUI16(SYS_FL_MACH_CODE54)&(~0x1020));
        }
        else if(value == 1)
        {
            VarAdrSetInt(SYS_FL_MACH_CODE0,VarAdrToUI16(SYS_FL_MACH_CODE0)|(0x0004));
            VarAdrSetInt(SYS_FL_MACH_CODE1,VarAdrToUI16(SYS_FL_MACH_CODE1)|(0x0004));
            VarAdrSetInt(SYS_FL_MACH_CODE54,VarAdrToUI16(SYS_FL_MACH_CODE54)&(~0x1020));
        }
        else if(value == 2)
        {
            VarAdrSetInt(SYS_FL_MACH_CODE0,VarAdrToUI16(SYS_FL_MACH_CODE0)&(~0x0004));
            VarAdrSetInt(SYS_FL_MACH_CODE1,VarAdrToUI16(SYS_FL_MACH_CODE1)&(~0x0004));
            VarAdrSetInt(SYS_FL_MACH_CODE54,VarAdrToUI16(SYS_FL_MACH_CODE54)|(0x1020));
        }
    }

}
/************************************************************************/
/* 机器类型设定                                                         */
/************************************************************************/
static void MachType()
{
    UI16 value;

    if(VarAdrToUI16(SYS_FL_MACH_CODE1)&0x04)
    {
        value = 1;
    }
    else if(((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x0004)==0) && (VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020))
    {
        value = 2;
    }
    else
    {
        value = 0;
    }

    VarAdrSetInt(TMP_MACHINETYPE,value);

    //20241205 chj 金鹰增加麦格米特模式
    if(VarAdrToUI16(SYS_FL_MACH_CODE1)&0x04)
    {
        value = 1;
    }
    else if(((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x0004)==0) && (VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020) && (VarAdrToInt(SYS_FL_MACH_CODE54) & 0x100) )//20241205 chj 金鹰增加麦格米特模式
    {
        value = 3;
    }
    else if(((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x0004)==0) && (VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020))
    {
        value = 2;
    }
    else
    {
        value = 0;
    }

    VarAdrSetInt(TMP_MACHINETYPEMG,value);

    if((VarAdrToInt(d_machine1_MACHSET_FL_CUSTOMID)&0xFF00) == 0x6400)
    {
        WGTSetVisible(mach_cfg_5200_pg.dropdown1,FALSE);
        WGTSetVisible(mach_cfg_5200_pg.dropdownMG,TRUE);
    }
    else
    {
        WGTSetVisible(mach_cfg_5200_pg.dropdown1,TRUE);
        WGTSetVisible(mach_cfg_5200_pg.dropdownMG,FALSE);
    }
}

static int switch_key=0;
/************************************************************************/
/* 切页面信息提示框“保存”按钮                                         */
/************************************************************************/
static void SaveButton()
{
    if(VarAdrToUI16(OPMODE_INDEX) == MODE_MANUAL)//20200109.cyx 只有手动状态下才能保存
    {
        NameChangeAndShow();//20200325
        GetPushSPNum();
        WGTSetVisible(mach_cfg_5200_pg.lblpushtable, FALSE);
        CodeSendSaveBack();//20230518 chj 此函数调用后，告诉主机需要提示重启,内容同CodeSendSave一样
    }

    VarAdrSetInt(MACHCFG5200_CHANGE_FLAG,0);//数据有修改标记还原
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
    //oprintf("switch_key==%x\r\n",switch_key);
}
/************************************************************************/
/*  切页面信息提示框“取消”按钮                                        */
/************************************************************************/
static void CancelButton()
{
    VarAdrSetInt(MACHCFG5200_CHANGE_FLAG,0);//数据有修改标记还原
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
/************************************************************************/
/* 配置修改回调函数                                                     */
/************************************************************************/
static void MachCfg5200Change(int key,int press)
{
    if(press)
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

static int button42OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int button43OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int button44OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int button45OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static void CtlConnect()
{
    WgtMsgConnect(mach_cfg_5200_pg.button45, WGT_CLICK_MSG, button45OnClick);
    WgtMsgConnect(mach_cfg_5200_pg.button44, WGT_CLICK_MSG, button44OnClick);
    WgtMsgConnect(mach_cfg_5200_pg.button43, WGT_CLICK_MSG, button43OnClick);
    WgtMsgConnect(mach_cfg_5200_pg.button42, WGT_CLICK_MSG, button42OnClick);
}
static int Mach_Cfg_5200OnInit(Mach_Cfg_5200_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetMachcfg5200CB(MachCfg5200Change);

    return 0;
}
static int Mach_Cfg_5200OnShow(Mach_Cfg_5200_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_MODELOCK);//20210914 dyl touch

    bMachCfg5200Change=FALSE;
    g_bMachCfgReset=FALSE;
    VarAdrSetInt(TMP_CODEPASSWORD,0);//CODE密码为0
    VarAdrSetInt(TMP_MAINRESETPASW,0);//RESET密码为0
    MachType();
    CodeReadOnly(ppg);
    WGTSetEnable(ppg->button43, FALSE);

    NameChangeAndShow();//20200325
    WGTSetVisible(mach_cfg_5200_pg.lblpushtable, FALSE);
    SetPushWndTxt();

    return 0;
}
static int Mach_Cfg_5200OnHide(Mach_Cfg_5200_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch

    if(bMachCfg5200Change)
    {
        RequestCfgData();//request machcfg;
        bMachCfg5200Change = FALSE;
    }

    SetPushtableType(VarAdrToUI16(SYS_FL_MACHINEMODEL));  //hz 20181128  test

    return 0;
}
static int Mach_Cfg_5200OnUpdate(Mach_Cfg_5200_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageProtect();//20210914 dyl touch //PAGE_PROTECT

    if(g_bMachCfgReset && VarAdrToUI16(SYS_FL_CTRLDATE)){
        g_bMachCfgReset = FALSE;
        ShowMsg(VW_MSG_HOST_RESTAROK);//主机重置成功

        MachType();
        WGTSetVisible(mach_cfg_5200_pg.lblpushtable, FALSE);
        SetPushWndTxt();
    }

    if(VarAdrToUI16(UDIKS_READ_FLAG))
    {
        VarAdrSetInt(UDIKS_READ_FLAG,0);
        //bret =  FALSE;
        LoadAllMachcfg();
        MachType();
        CodeSendSaveBack();//20230518 chj 此函数调用后，告诉主机需要提示重启,内容同CodeSendSave一样
        WGTSetVisible(mach_cfg_5200_pg.lblpushtable, FALSE);
        SetPushWndTxt();
    }

    return 0;
}

static int Mach_Cfg_5200OnChange(Mach_Cfg_5200_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 addr;
    PATTR pattr;

    pattr = (PATTR)msg_para;

    addr = OBJAttrGetAdr(pattr);

    //20241205 chj 金鹰增加麦格米特模式
    if(addr !=TMP_CODEPASSWORD && addr !=TMP_MACHINETYPE && addr !=TMP_MACHINETYPEMG && addr !=TMP_MAINRESETPASW && pwgt!=ppg->button42
        && pwgt!=ppg->button43 && pwgt!=ppg->button44 && pwgt!=ppg->button45)
    {
        bMachCfg5200Change = TRUE;
        VarAdrSetInt(MACHCFG5200_CHANGE_FLAG,1);//数据有修改用于提示框
    }

    switch(addr)
    {
    case TMP_MACHINETYPE:
    case TMP_MACHINETYPEMG://20241205 chj 金鹰增加麦格米特模式
        MachTypeSel();
        CodeSendSaveBack();//20230518 chj 此函数调用后，告诉主机需要提示重启,内容同CodeSendSave一样
        break;
    case TMP_MAINRESETPASW:
        if (VarAdrToInt(TMP_MAINRESETPASW) == 95)
        {
            WGTSetEnable(ppg->button43, TRUE);
        }
        else
        {
            WGTSetEnable(ppg->button43, FALSE);
        }
        break;
    case TMP_CODEPASSWORD:
        CodeReadOnly(ppg);
        break;
    case SYS_FL_MACH_CODE4:      // 20181127 hz  当code4从打10到不打10过程转变中，会把电机温度值赋给对应的缓冲区的温度实际值，即使原先缓冲区为0不使用状态。
        if (VarAdrToInt(ADDR_TEMP_RAMP4) == 0)
        {
            VarAdrSetInt(ADDR_TEMP_RAMP4, 0);
            VarAdrSaveInt(ADDR_TEMP_RAMP4);
            VarSendByAdr(ADDR_TEMP_RAMP4);
        }
        if (VarAdrToInt(ADDR_TEMP_RAMP6) == 0)
        {
            VarAdrSetInt(ADDR_TEMP_RAMP6, 0);
            VarAdrSaveInt(ADDR_TEMP_RAMP6);
            VarSendByAdr(ADDR_TEMP_RAMP6);
        }
        if (VarAdrToInt(ADDR_TEMP_RAMP7) == 0)
        {
            VarAdrSetInt(ADDR_TEMP_RAMP7, 0);
            VarAdrSaveInt(ADDR_TEMP_RAMP7);
            VarSendByAdr(ADDR_TEMP_RAMP7);
        }
        break;
    case PUSH_TEMP_ADDR:
        if(CheckLegal())
        {
            WGTSetVisible(mach_cfg_5200_pg.lblpushtable, FALSE);
        }
        else
        {
            WGTSetVisible(mach_cfg_5200_pg.lblpushtable, TRUE);
            VarAdrSetStr(PUSH_TEMP_ADDR, cpushbak);
            return FALSE;
        }
        break;
    default:
        break;
    }


    return 0;
}
static int button42OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    VarAdrSetInt(MACHCFG5200_CHANGE_FLAG,0);//数据有修改标记还原
    MachType();
    NameChangeAndShow();//20200325
    GetPushSPNum();
    WGTSetVisible(mach_cfg_5200_pg.lblpushtable, FALSE);
    CodeSendSaveBack();//20230518 chj 此函数调用后，告诉主机需要提示重启,内容同CodeSendSave一样
    PanelRecAdd(mach_cfg_5200_pg.button42, PNL_SAVE, 0);
    return 0;
}
static int button43OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    PanelShowDialogByName(DIALOG_RESET);
    PanelRecAdd(mach_cfg_5200_pg.button43, PNL_RESET, 0);

    return 0;
}
static int button44OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(check_usb())
    {
        ShowMsg(VW_MSG_UDISKNOTEXIST);
        return 0;
    }
    else
    {
        PanelShowDialogByName(DIALOG_READUDISK);
        VarAdrSetInt(UDISK_WR_PRIVILEGE,3); //导入导出 权限
        PanelRecAdd(mach_cfg_5200_pg.button44, PNL_READ_U, 0);
    }

    return 0;
}
static int button45OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    MachCfgExport();

    if(check_usb())
    {
        ShowMsg(VW_MSG_UDISKNOTEXIST);
        return 0;
    }
    else
    {
        PanelShowDialogByName(DIALOG_WRITEUDISK);
        VarAdrSetInt(UDISK_WR_PRIVILEGE,3); //导入导出 权限
        PanelRecAdd(mach_cfg_5200_pg.button45, PNL_WRITE_U, 0);
    }

    return 0;
}
