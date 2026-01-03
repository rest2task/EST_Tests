#include "WAD_Ex.h"
#include "typedef.h"
#include "pagefram.h"
#include "database.h"
#include "keymod.h"
#include "key.h"
#include "privdef.h"
#include "questionDlg.h"
#include "command.h"
#include "labelwgt.h"
#include "protcmd.h"
#include "ctrlcmd.h"
#include "texttbl.h"
#include "usermod.h"//20210914 dyl touch

DEFINE_AD_EX_MAP

CONNECT_AD_EX_EVENT

#define AD1_ADJUST_POS  TMPVAL(0) //AD1调整位置
#define AD8_ADJUST_POS  TMPVAL(7) //AD8调整位置

#define STADEX_NAME(NUM,pbuff,size)		joinStrAndNum("stADEx",NUM,pbuff,size)
#define STADUNIT_NAME(NUM,pbuff,size)	joinStrAndNum("stUnit",NUM,pbuff,size)//20211210 dyl 根据液压通道号、座台通道号设置AD名称

enum ADEX_GOFFSET
{
    ID_AD1_METERMAX = 1,
    ID_AD8_METERMAX = 8,
    ID_AD1_MACHMAX = 9,
    ID_AD8_MACHMAX = 16,
    ID_AD1_CLEAR = 17,
    ID_AD8_CLEAR = 24,
    ID_AD1_ADJPOS = 25,
    ID_AD8_ADJPOS = 32,
    ID_SAVE = 33,
    ID_END
};

BOOL  bADExChane = FALSE;

static QUESTION g_question;
static QUESTION g_question_zeroadj_ex;//20250320 jhh AD画面归零和调整位置改变时弹出数据更改提示框
static int wgt_offset_ex;
static int old_value_ex;
static BOOL bShowQuestion_ex = FALSE;
int ADPageFlag_ex;
static int switch_key=0;
/************************************************************************/
/* 切页面信息提示框“保存”按钮                                         */
/************************************************************************/
static void SaveButton()
{
    if(VarAdrToUI16(OPMODE_INDEX) == MODE_MANUAL)//20200109.cyx 只有手动状态下才能保存
    {
        CodeSendSave();
    }

    VarAdrSetInt(ADEX_CHANGE_FLAG,0);//数据有修改标记还原
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

//20250320 jhh AD画面归零和调整位置改变时弹出数据更改提示框
static void ZeroAdjExSaveButton()
{
    UI32 index;
    UI16 pdata[3];

    if(VarAdrToUI16(OPMODE_INDEX) == MODE_MANUAL)//20200109.cyx 只有手动状态下才能保存
    {
        if((wgt_offset_ex >= ID_AD1_CLEAR) && (wgt_offset_ex <= ID_AD8_CLEAR))
        {
            pdata[0] = 1;//0-标准 1-扩展
            index = wgt_offset_ex - ID_AD1_CLEAR;
            pdata[1] = index;//记录使用
            pdata[2] = 0;
            VarSendSaveSetIntByAdr(d_admeter1_MACHSET_ZSET_EXT1 + index,VarAdrToInt(d_admeter1_STATE_ADPOSI_ABSOLUTE_EXT1 + index));
            VarAdrSetInt(d_admeter1_STATE_ADPOSI_RELATIVE_EXT1 + index,0);
            ProtSysCtrl(METER_ZERO_ADJ_CMD,3,pdata);
        }
        else if((wgt_offset_ex >= ID_AD1_ADJPOS) && (wgt_offset_ex <= ID_AD8_ADJPOS))
        {
            index = wgt_offset_ex - ID_AD1_ADJPOS;
            pdata[0] = index;
            pdata[1] = VarAdrToUI16(AD1_ADJUST_POS + index);
            pdata[2] = 1;//0-标准 1-扩展
            ProtSysCtrl(METER_LENGTH_ADJ_CMD,3,pdata);
        }
    }
}
/************************************************************************/
/*  切页面信息提示框“取消”按钮                                        */
/************************************************************************/
static void CancelButton()
{
    VarAdrSetInt(ADEX_CHANGE_FLAG,0);//数据有修改标记还原
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

//20250320 jhh AD画面归零和调整位置改变时弹出数据更改提示框
static void ZeroAdjExCancelButton()
{
    if((wgt_offset_ex >= ID_AD1_ADJPOS) && (wgt_offset_ex <= ID_AD8_ADJPOS))
    {
        UI32 index;
        index = wgt_offset_ex - ID_AD1_ADJPOS;
        VarAdrSetInt(AD1_ADJUST_POS + index, old_value_ex);
    }
}
/************************************************************************/
/* 配置修改回调函数                                                     */
/************************************************************************/
static void ADExChange(int key,int press)
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

static void NameChangeAndShow()  //20200325
{
    PWGT pwgt;
    char buff[10];
    UI16 wNozzleValue = VarAdrToInt(d_nozzle1_MACHSET_NZL_METERCH);

//    LabelSetText(ad_ex_pg.stADEx1,GetTextTran(TEXT_AD_EXAD1));
//    LabelSetText(ad_ex_pg.stADEx2,GetTextTran(TEXT_AD_EXAD2));
//    LabelSetText(ad_ex_pg.stADEx3,GetTextTran(TEXT_AD_EXAD3));
//    LabelSetText(ad_ex_pg.stADEx4,GetTextTran(TEXT_AD_EXAD4));
//    LabelSetText(ad_ex_pg.stUnit4,"mm");

//    if (VarAdrToInt(SYS_FL_METER) & 0x0004)
//    {
//        if (IV3200 || IV3300)
//        {
//            if((wNozzleValue>=5)&&(wNozzleValue<=7))  //4-7路做座台切换
//            {
//                pwgt = PageFramCurWgtByName(STADEX_NAME(wNozzleValue-4,buff,10));
//                LabelSetText(pwgt,GetTextTran(TEXT_AD_NOZZLE));
//            }
//            LabelSetText(ad_ex_pg.stADEx4,GetTextTran(TEXT_AD_ADHYDR2));
//            LabelSetText(ad_ex_pg.stUnit4,"bar");
//        }
//    }

    //20211210 dyl 根据液压通道号、座台通道号设置AD名称
    LabelSetText(ad_ex_pg.stADEx1,GetTextTran(TEXT_AD_EXAD1));
    LabelSetText(ad_ex_pg.stADEx2,GetTextTran(TEXT_AD_EXAD2));
    LabelSetText(ad_ex_pg.stADEx3,GetTextTran(TEXT_AD_EXAD3));
    LabelSetText(ad_ex_pg.stADEx4,GetTextTran(TEXT_AD_EXAD4));
    LabelSetText(ad_ex_pg.stADEx5,GetTextTran(TEXT_AD_EXAD5));
    LabelSetText(ad_ex_pg.stADEx6,GetTextTran(TEXT_AD_EXAD6));
    LabelSetText(ad_ex_pg.stADEx7,GetTextTran(TEXT_AD_EXAD7));
    LabelSetText(ad_ex_pg.stADEx8,GetTextTran(TEXT_AD_EXAD8));
    LabelSetText(ad_ex_pg.stUnit1,"mm");
    LabelSetText(ad_ex_pg.stUnit2,"mm");
    LabelSetText(ad_ex_pg.stUnit3,"mm");
    LabelSetText(ad_ex_pg.stUnit4,"mm");
    LabelSetText(ad_ex_pg.stUnit5,"mm");
    LabelSetText(ad_ex_pg.stUnit6,"mm");
    LabelSetText(ad_ex_pg.stUnit7,"mm");
    LabelSetText(ad_ex_pg.stUnit8,"mm");
    UI16 wChanel_Sensor1 = VarAdrToInt(d_Pump1_MACHSET_PRESSAD_CH);
    UI16 wChanel_Sensor2 = VarAdrToInt(d_Pump2_MACHSET_PRESSAD_CH);
    if (VarAdrToInt(SYS_FL_METER) & 0x0004)
    {
        if(((wNozzleValue-g_mainresource_current.nADNum)>=1)&&((wNozzleValue-g_mainresource_current.nADNum)<=8))  //4-7路做座台切换
        {
            pwgt = PageFramCurWgtByName(STADEX_NAME(wNozzleValue-g_mainresource_current.nADNum,buff,10));
            LabelSetText(pwgt,GetTextTran(TEXT_AD_NOZZLE));
        }
    }
    if(VarAdrToUI16(SYS_FL_MACH_CODE1)&0x04)//20220928 dyl ECAT模式
    {
        if (((wChanel_Sensor1-g_mainresource_current.nADNum)>=1)&&((wChanel_Sensor1-g_mainresource_current.nADNum) <= 8))
        {
            pwgt = PageFramCurWgtByName(STADEX_NAME((wChanel_Sensor1-g_mainresource_current.nADNum),buff,10));
            LabelSetText(pwgt,GetTextTran(TEXT_AD_SENSOR1));
            pwgt = PageFramCurWgtByName(STADUNIT_NAME((wChanel_Sensor1-g_mainresource_current.nADNum),buff,10));
            LabelSetText(pwgt,"bar");
        }
        if (((wChanel_Sensor2-g_mainresource_current.nADNum)>=1)&&((wChanel_Sensor2-g_mainresource_current.nADNum) <= 8)
                &&(wChanel_Sensor2 != wChanel_Sensor1))
        {
            pwgt = PageFramCurWgtByName(STADEX_NAME((wChanel_Sensor2-g_mainresource_current.nADNum),buff,10));
            LabelSetText(pwgt,GetTextTran(TEXT_AD_SENSOR2));
            pwgt = PageFramCurWgtByName(STADUNIT_NAME((wChanel_Sensor2-g_mainresource_current.nADNum),buff,10));
            LabelSetText(pwgt,"bar");
        }
    }

}

static void CtlConnect()
{
}
static int AD_ExOnInit(AD_Ex_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetADExCB(ADExChange);
    return 0;
}
static int AD_ExOnShow(AD_Ex_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    int i;
    bADExChane = FALSE;

    //20250320 jhh AD画面归零和调整位置改变时弹出数据更改提示框
    wgt_offset_ex = 0;
    old_value_ex = 0;
    bShowQuestion_ex = FALSE;
    ADPageFlag_ex = 0;
    g_question_zeroadj_ex.pOkfunc = ZeroAdjExSaveButton;
    g_question_zeroadj_ex.pCancelfunc = ZeroAdjExCancelButton;

    for(i = 0;i < 8;i++)
    {
        VarAdrSetInt(AD1_ADJUST_POS + i,0);
    }

    NameChangeAndShow();//20200325

    SetPageLockMode(MODE_MODELOCK);//20210914 dyl touch

    return 0;
}
static int AD_ExOnHide(AD_Ex_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch

    //20250320 jhh AD画面归零和调整位置改变时弹出数据更改提示框
    wgt_offset_ex = 0;
    old_value_ex = 0;
    bShowQuestion_ex = FALSE;
    ADPageFlag_ex = 0;

    if(bADExChane)
    {
        RequestCfgData();//request machcfg;
        bADExChane = FALSE;
    }
    return 0;
}
static int AD_ExOnUpdate(AD_Ex_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageProtect();//20210914 dyl touch //PAGE_PROTECT

    LabelSetText(ppg->sign1,(VarAdrToUI16(d_admeter1_STATE_ADPOSI_ABSOLUTE_EXT1) >= VarAdrToUI16(d_admeter1_MACHSET_ZSET_EXT1)) ? "+":"-");
    LabelSetText(ppg->sign2,(VarAdrToUI16(d_admeter1_STATE_ADPOSI_ABSOLUTE_EXT2) >= VarAdrToUI16(d_admeter1_MACHSET_ZSET_EXT2)) ? "+":"-");
    LabelSetText(ppg->sign3,(VarAdrToUI16(d_admeter1_STATE_ADPOSI_ABSOLUTE_EXT3) >= VarAdrToUI16(d_admeter1_MACHSET_ZSET_EXT3)) ? "+":"-");
    LabelSetText(ppg->sign4,(VarAdrToUI16(d_admeter1_STATE_ADPOSI_ABSOLUTE_EXT4) >= VarAdrToUI16(d_admeter1_MACHSET_ZSET_EXT4)) ? "+":"-");
    LabelSetText(ppg->sign5,(VarAdrToUI16(d_admeter1_STATE_ADPOSI_ABSOLUTE_EXT5) >= VarAdrToUI16(d_admeter1_MACHSET_ZSET_EXT5)) ? "+":"-");
    LabelSetText(ppg->sign6,(VarAdrToUI16(d_admeter1_STATE_ADPOSI_ABSOLUTE_EXT6) >= VarAdrToUI16(d_admeter1_MACHSET_ZSET_EXT6)) ? "+":"-");
    LabelSetText(ppg->sign7,(VarAdrToUI16(d_admeter1_STATE_ADPOSI_ABSOLUTE_EXT7) >= VarAdrToUI16(d_admeter1_MACHSET_ZSET_EXT7)) ? "+":"-");
    LabelSetText(ppg->sign8,(VarAdrToUI16(d_admeter1_STATE_ADPOSI_ABSOLUTE_EXT8) >= VarAdrToUI16(d_admeter1_MACHSET_ZSET_EXT8)) ? "+":"-");


    //20250320 jhh AD画面归零和调整位置改变时弹出数据更改提示框
    if(bShowQuestion_ex)
    {
        bShowQuestion_ex = FALSE;
        ShowQuestion(VW_MSG_SAVEDATA,g_question_zeroadj_ex);
    }
    return 0;
}
static int AD_ExOnChange(AD_Ex_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 goff,index;
    UI16 pdata[3];

    goff = WGTGetGOffset(pwgt);

    if((goff >= ID_AD1_METERMAX) && (goff <= ID_AD8_MACHMAX))
    {
        bADExChane = TRUE;
        VarAdrSetInt(ADEX_CHANGE_FLAG,1);//数据有修改用于提示框
    }
    else if((goff >= ID_AD1_CLEAR) && (goff <= ID_AD8_ADJPOS))//20250320 jhh AD画面归零和调整位置改变时弹出数据更改提示框
    {
        wgt_offset_ex = goff;
        old_value_ex = msg_value;
        ADPageFlag_ex = 1;
        bShowQuestion_ex = TRUE;
    }
    /*else if((goff >= ID_AD1_ADJPOS) && (goff <= ID_AD8_ADJPOS))
    {
        index = goff - ID_AD1_ADJPOS;
        pdata[0] = index;
        pdata[1] = VarAdrToUI16(AD1_ADJUST_POS + index);
        pdata[2] = 1;//0-标准 1-扩展
        ProtSysCtrl(METER_LENGTH_ADJ_CMD,3,pdata);
    }*/
    else if(goff == ID_SAVE)
    {
        VarAdrSetInt(ADEX_CHANGE_FLAG,0);//数据有修改标记还原
        CodeSendSave();
    }

    return 0;
}
