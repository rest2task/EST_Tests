#include "WAD_Adjustx.h"
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
#include "pushtab.h"
#include "texttbl.h"
#include "oprintf.h"//20211228 dyl
#include "usermod.h"//20210914 dyl touch

DEFINE_AD_ADJUSTX_MAP

CONNECT_AD_ADJUSTX_EVENT

#define AD1_ADJUST_POS  TMPVAL(0) //AD1调整位置
#define AD8_ADJUST_POS  TMPVAL(7) //AD8调整位置

#define STAD_NAME(NUM,pbuff,size)		joinStrAndNum("stAD",NUM,pbuff,size)
#define STADUNIT_NAME(NUM,pbuff,size)	joinStrAndNum("stUnit",NUM,pbuff,size)//20211210 dyl 根据液压通道号、座台通道号设置AD名称

enum ADX_GOFFSET
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

static BOOL  bADxChane = FALSE;

static QUESTION g_question;
static QUESTION g_question_zeroadj;//20250320 jhh AD画面归零和调整位置改变时弹出数据更改提示框
static int wgt_offset;
static int old_value;
static BOOL bShowQuestion = FALSE;
int ADPageFlag;
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

    VarAdrSetInt(ADX_CHANGE_FLAG,0);//数据有修改标记还原
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
static void ZeroAdjSaveButton()
{
    UI32 index;
    UI16 pdata[3];

    if(VarAdrToUI16(OPMODE_INDEX) == MODE_MANUAL)//20200109.cyx 只有手动状态下才能保存
    {
        if((wgt_offset >= ID_AD1_CLEAR) && (wgt_offset <= ID_AD8_CLEAR))
        {
            pdata[0] = 0;//0-标准 1-扩展
            index = wgt_offset - ID_AD1_CLEAR;
            pdata[1] = index;//记录使用
            pdata[2] = 0;
            VarSendSaveSetIntByAdr(ADMETER_ZSET1 + index,VarAdrToInt(ADMETER_ABSOLUTE1 + index));
            VarAdrSetInt(ADDR_ADPOSI_REAL1 + index,0);
            ProtSysCtrl(METER_ZERO_ADJ_CMD,3,pdata);
        }
        else if((wgt_offset >= ID_AD1_ADJPOS) && (wgt_offset <= ID_AD8_ADJPOS))
        {
            index = wgt_offset - ID_AD1_ADJPOS;
            pdata[0] = index;
            pdata[1] = VarAdrToUI16(AD1_ADJUST_POS + index);
            if(PushtableType() && index == 1)
            {
                pdata[1] = UnTransMeter(pdata[1],FALSE);
            }
            pdata[2] = 0;//0-标准 1-扩展
            ProtSysCtrl(METER_LENGTH_ADJ_CMD,3,pdata);
        }
    }
}
/************************************************************************/
/*  切页面信息提示框“取消”按钮                                        */
/************************************************************************/
static void CancelButton()
{
    VarAdrSetInt(ADX_CHANGE_FLAG,0);//数据有修改标记还原
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
static void ZeroAdjCancelButton()
{
    if((wgt_offset >= ID_AD1_ADJPOS) && (wgt_offset <= ID_AD8_ADJPOS))
    {
        UI32 index;
        index = wgt_offset - ID_AD1_ADJPOS;
        VarAdrSetInt(AD1_ADJUST_POS + index, old_value);
    }
}
/************************************************************************/
/* 配置修改回调函数                                                     */
/************************************************************************/
static void ADxChange(int key,int press)
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

//    LabelSetText(ad_adjustx_pg.stAD4,GetTextTran(TEXT_AD_ADHYDR1));
//    LabelSetText(ad_adjustx_pg.stUnit4,"bar");
//    LabelSetText(ad_adjustx_pg.stAD5,"AD5");
//    LabelSetText(ad_adjustx_pg.stAD6,"AD6");
//    LabelSetText(ad_adjustx_pg.stAD7,"AD7");

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
//                    LabelSetText(ad_adjustx_pg.stUnit4,"mm");
//                }
//            }
//        }
//        else
//        {
//            if(wNozzleValue == 4)  //4做座台切换
//            {
//                LabelSetText(ad_adjustx_pg.stAD4,GetTextTran(TEXT_AD_NOZZLE));
//                LabelSetText(ad_adjustx_pg.stUnit4,"mm");
//            }
//        }
//    }

    //20211210 dyl 根据液压通道号、座台通道号设置AD名称
    LabelSetText(ad_adjustx_pg.stAD4,"AD4");
    LabelSetText(ad_adjustx_pg.stAD5,"AD5");
    LabelSetText(ad_adjustx_pg.stAD6,"AD6");
    LabelSetText(ad_adjustx_pg.stAD7,"AD7");
    LabelSetText(ad_adjustx_pg.stAD8,"AD8");
    LabelSetText(ad_adjustx_pg.stUnit4,"mm");
    LabelSetText(ad_adjustx_pg.stUnit5,"mm");
    LabelSetText(ad_adjustx_pg.stUnit6,"mm");
    LabelSetText(ad_adjustx_pg.stUnit7,"mm");
    LabelSetText(ad_adjustx_pg.stUnit8,"mm");
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
            LabelSetText(pwgt,"bar");
        }
        if ((wChanel_Sensor2>=4)&&(wChanel_Sensor2 <= g_mainresource_current.nADNum)
                &&(wChanel_Sensor2 != wChanel_Sensor1))
        {
            pwgt = PageFramCurWgtByName(STAD_NAME(wChanel_Sensor2,buff,10));
            LabelSetText(pwgt,GetTextTran(TEXT_AD_SENSOR2));
            pwgt = PageFramCurWgtByName(STADUNIT_NAME(wChanel_Sensor2,buff,10));
            LabelSetText(pwgt,"bar");
        }
    }

}

static void CtlConnect()
{
}
static int AD_AdjustxOnInit(AD_Adjustx_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetADxCB(ADxChange);
    return 0;
}
static int AD_AdjustxOnShow(AD_Adjustx_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    int i;
    bADxChane = FALSE;

    //20250320 jhh AD画面归零和调整位置改变时弹出数据更改提示框
    wgt_offset = 0;
    old_value = 0;
    bShowQuestion = FALSE;
    ADPageFlag = 0;
    g_question_zeroadj.pOkfunc = ZeroAdjSaveButton;
    g_question_zeroadj.pCancelfunc = ZeroAdjCancelButton;

    for(i = 0;i < 8;i++)
    {
        VarAdrSetInt(AD1_ADJUST_POS + i,0);
    }

    NameChangeAndShow();//20200325

    SetPageLockMode(MODE_MODELOCK);//20210914 dyl touch

    return 0;
}
static int AD_AdjustxOnHide(AD_Adjustx_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch

    //20250320 jhh AD画面归零和调整位置改变时弹出数据更改提示框
    wgt_offset = 0;
    old_value = 0;
    bShowQuestion = FALSE;
    ADPageFlag = 0;

    if(bADxChane)
    {
        RequestCfgData();//request machcfg;
        bADxChane = FALSE;
    }

    return 0;
}
static int AD_AdjustxOnUpdate(AD_Adjustx_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageProtect();//20210914 dyl touch //PAGE_PROTECT

    LabelSetText(ppg->sign1,(VarAdrToUI16(ADMETER_ABSOLUTE1) >= VarAdrToUI16(ADMETER_ZSET1)) ? "+":"-");
    LabelSetText(ppg->sign2,(VarAdrToUI16(ADMETER_ABSOLUTE2) >= VarAdrToUI16(ADMETER_ZSET2)) ? "+":"-");
    LabelSetText(ppg->sign3,(VarAdrToUI16(ADMETER_ABSOLUTE3) >= VarAdrToUI16(ADMETER_ZSET3)) ? "+":"-");
    LabelSetText(ppg->sign4,(VarAdrToUI16(ADMETER_ABSOLUTE4) >= VarAdrToUI16(ADMETER_ZSET4)) ? "+":"-");
    LabelSetText(ppg->sign5,(VarAdrToUI16(ADMETER_ABSOLUTE5) >= VarAdrToUI16(ADMETER_ZSET5)) ? "+":"-");
    LabelSetText(ppg->sign6,(VarAdrToUI16(ADMETER_ABSOLUTE6) >= VarAdrToUI16(ADMETER_ZSET6)) ? "+":"-");
    LabelSetText(ppg->sign7,(VarAdrToUI16(ADMETER_ABSOLUTE7) >= VarAdrToUI16(ADMETER_ZSET7)) ? "+":"-");
    LabelSetText(ppg->sign8,(VarAdrToUI16(ADMETER_ABSOLUTE8) >= VarAdrToUI16(ADMETER_ZSET8)) ? "+":"-");

    //20250320 jhh AD画面归零和调整位置改变时弹出数据更改提示框
    if(bShowQuestion)
    {
        bShowQuestion = FALSE;
        ShowQuestion(VW_MSG_SAVEDATA,g_question_zeroadj);
    }

    return 0;
}
static int AD_AdjustxOnChange(AD_Adjustx_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 goff,index;
    UI16 pdata[3];

    UI32 addr;
    PATTR pattr;
    pattr = (PATTR)msg_para;
    addr = OBJAttrGetAdr(pattr);

    goff = WGTGetGOffset(pwgt);

    if((goff >= ID_AD1_METERMAX) && (goff <= ID_AD8_MACHMAX))
    {
        bADxChane = TRUE;
        VarAdrSetInt(ADX_CHANGE_FLAG,1);//数据有修改用于提示框
    }
    else if((goff >= ID_AD1_CLEAR) && (goff <= ID_AD8_ADJPOS))//20250320 jhh AD画面归零和调整位置改变时弹出数据更改提示框
    {
        wgt_offset = goff;
        old_value = msg_value;
        ADPageFlag = 1;
        bShowQuestion = TRUE;
    }
    /*else if((goff >= ID_AD1_ADJPOS) && (goff <= ID_AD8_ADJPOS))
    {
        index = goff - ID_AD1_ADJPOS;
        pdata[0] = index;
        pdata[1] = VarAdrToUI16(AD1_ADJUST_POS + index);
        if(PushtableType() && index == 1)
        {
            pdata[1] = UnTransMeter(pdata[1],FALSE);
        }
        pdata[2] = 0;//0-标准 1-扩展
        ProtSysCtrl(METER_LENGTH_ADJ_CMD,3,pdata);
    }*/
    else if(goff == ID_SAVE)
    {
        VarAdrSetInt(ADX_CHANGE_FLAG,0);//数据有修改标记还原
        CodeSendSave();
    }

    return 0;
}
