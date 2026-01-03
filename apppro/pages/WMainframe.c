#include "WMainframe.h"
#include "typedef.h"
#include "alarmrecord.h"
#include "database.h"
#include "mainframe.h"
#include "time.h"
#include "stdio.h"
#include "key.h"
#include "editwgt.h"
#include "servo.h"
#include "oprintf.h"
#include "moldset.h"
#include "mod.h"
#include "labelwgt.h"
#include "usermod.h"
#include "labelwgt.h"
#include "privdef.h"
#include "texttbl.h"
#include "alarmrecord_G10.h"
#include "keyprot.h"
#include "information.h"
#include "timecheck.h"
#include "verinfo.h"
#include "temp.h"
#include "drivealarmrecord.h"
#include "lubr.h"
#include "servoalarmtbl.h" //20211220 dyl
#include "keymod.h" //20210914 dyl
#include "touch.h" //20210914 dyl
#include "keyprot2.h"//20210914 dyl
#include "protmod.h"
#include "questionDlg.h"
#include "texttbl.h"
#include "information.h"
#include "dataOperation.h"
#include "panelcfg.h"

DEFINE_MAINFRAME_MAP

CONNECT_MAINFRAME_EVENT

#define   MAINFRAME_TIME       0x33120051 //系统时间
#define   MAINFRAME_ALARM      0x33120056 //警报显示
#define   MAINFRAME_ACTSETP    0x33120057 //动作步
#define   MAINFRAME_PAGETITLE  0x33120058 //页面标题

//#define	  TMP_ALARMSHOW	   TMPVAL(470)//警报是否显示


#define   SERVO_SPEED        0x331300e5 //电机转速
#define   SERVO_PRESS        0x331300e6 //反馈压力
#define   SERVO_CURRENT      0x331300e7 //输出电流

#define USER_LV_SHOW        0x331700b5//当前用户等级
#define LANGUAGE_SHOW       0x331700b8//当前语言

#define CMOD_WR_HEADSZ sizeof(MOLDSET_HEAD)
#define CMOD_WR_ITEMSZ sizeof(MOLDSET_ITEM)
#define CMOD_WR_PATH   SAVE_PATH"moldsetx.dat"
#define     SERVOWARN_HELP_URL      HMI_ROOT_PATH"servo_help/servo_ERR_cn_" //20211209 dyl
#define     SERVOWARN_HELP_URL_EN   HMI_ROOT_PATH"servo_help/servo_ERR_en_"
#define     FILE_SUFFIX     ".html"

#define USER_INITRANK	0x33170025  //用户初始等级
#define PumpNum 12 //20191205.cyx
#define BUZZER_SOUND    0x3317004e  //蜂鸣器声音
#define BTHELP_SHOW     0x3317003f  //帮助按钮显示图片

enum MOTOR_STATE
{
    MOTOR_STOP=0,			//停止状态（不亮）
    MOTOR_RUNNING,			//运行状态（常亮）
    MOTOR_START				//启用状态（闪烁）
};

static BOOL  bMotor = FALSE;

#define WARN_NUM_MAX    10 //最大警报数量
#define MAX_ALARM_CHAR_NUM  1000 //最大警报字符长度
extern UI32 g_warm1,g_warm2,g_warm3,g_warm4,g_warm5,g_warm6,g_warm7,g_warm8,g_warm9,g_warm10;
static PHASH_TBL  pmap_step =NULL;

extern BOOL b_MoniWriteFlie;//20230518 chj  开机发现写文件过多标记，闪烁警报10s

//20230518 chj 收到主机请求重启命令，弹出重启框，重启主机
extern BOOL b_RecMainNeedReStartCmd;
static BOOL bMianNeedBoot = FALSE;
static QUESTION g_question;
static void RecMainNeedReStartCmd()
{
    ProtUpgradReset(0);

    bMianNeedBoot = TRUE;
    InitMainHand();
}

/*********** Touch start ***********/ //20210914 dyl
enum CLICK_EVENT
{
    MouseButtonPress = 2,                   // mouse button pressed
    MouseButtonRelease = 3,                 // mouse button released
    MouseButtonDblClick = 4
};
static void ClickEvent(int type)
{
    if(MouseButtonRelease == type)
    {
        Reset_ActiveTime_Count();
    }
}
//画面锁住后点击控件出现的弹出框 //20191224.cyx
static void ShowLockMsg(PPAGE_FRAM ppage)
{
    if((GetPageLockMode() == MODE_MODELOCK) || (GetPageLockMode() == MODE_USER_MODELOCK))
    {
        if(VarAdrToInt(HARDWARE_LOCK))//硬件锁存在
        {
            ShowMsg(VW_MSG_LOCKED);//20200820
        }
        else if((OperateModeIndex() != MODE_MANUAL) || JudgeChargeActStep() || JudgeSuckBackActStep())//操作模式锁存在 //20230518 chj增加射退动作判断
        {
            ShowMsg(VW_MSG_DATAPROTECT);
        }
        else//生产人员锁
        {
            ShowMsg(VW_MSG_USERERR);
        }
    }
    else
    {
        if(VarAdrToInt(HARDWARE_LOCK))//硬件锁存在
        {
            ShowMsg(VW_MSG_LOCKED);//20200820
        }
        else
        {
            ShowMsg(VW_MSG_USERERR);
        }
    }
}
static void TouchMainframeInit()
{
    KeyPortSetCb(ProcessCommRxData3);//20191224.cyx 串口三
    SetRevKeyCb(ReceieveRxKey);//20191224.cyx 触摸屏校验
    PanelSetProtectedCB(ShowLockMsg);//20191224.cyx 用户权限管理:生产人员lock
    setEvnetCb(ClickEvent);//20191224.cyx 点击屏幕重置屏保时间
}
/*********** Touch end   ***********/

/*********** G15新增 start ***********/
extern UI32 dwPanelErr;

#define MODE_PICTURE 0x331700d7//图片根据模式改变
//控制模式图片刷新
static void PicModeUpdate()
{
    UI16 wMode;//Mode Picture
    static UI16 wModeRec = 255;
    wMode = VarAdrToInt(OPMODE_INDEX);
    if (wModeRec != wMode)
    {
        wModeRec = wMode;

        if(wMode == MODE_MANUAL)
        {
            VarAdrSetStr(MODE_PICTURE,"manual.png");
        }
        else if((wMode== 2) || (wMode == 3))
        {
            VarAdrSetStr(MODE_PICTURE,"auto.png");
        }
        else if((wMode == 1))
        {
            VarAdrSetStr(MODE_PICTURE,"semiauto.png");
        }
        else if((wMode == 4)|| (wMode == 6)|| (wMode == 11)|| (wMode == 12)|| (wMode == 13))
        {
            VarAdrSetStr(MODE_PICTURE,"adjust.png");
        }
//        else if((wMode == 7))
//        {
//            VarAdrSetStr(MODE_PICTURE,"purge.png");
//        }
    }
}

/*********** G15新增 end   ***********/

static void SetDataShow()//主页面数据显示
{
    char cbuf[10];
    BOOL g_bDaViewIn;
    UI32 ctrlmodel;
    UI32 speed_val;

    sprintf(cbuf,"%6.2f",VarAdrToInt(d_machine1_STATE_TM_CYCLE) / 100.);//全程计时
    LabelSetText(mainframe_pg.stcycle,cbuf);

    if(VarAdrToInt(SYS_FL_MACH_CODE52) & 0x0080)//20210302
    {
        if(PanelCurPage()==PanelFindPage(PAGE_CLAMP) || PanelCurPage()==PanelFindPage(PAGE_CLAMP_SMART)
                 || PanelCurPage()==PanelFindPage(PAGE_CLAMPX))
        {
            WGTSetVisible(mainframe_pg.subclamp, TRUE);
            WGTSetVisible(mainframe_pg.subinject, FALSE);
            WGTSetVisible(mainframe_pg.submain, FALSE);

            sprintf(cbuf,"%6.2f",VarAdrToInt(d_clamp1_STATE_TM_CLAMPCLS2) / 100.);//关模时间
            LabelSetText(mainframe_pg.subclamp_stclstm,cbuf);
            sprintf(cbuf,"%6.2f",VarAdrToInt(d_clamp1_STATE_TM_CLAMPOPN2) / 100.);//开模时间
            LabelSetText(mainframe_pg.subclamp_stopntm,cbuf);
            sprintf(cbuf,"%4.1f",VarAdrToInt(INJECT_CN_CHRGSPD) / 10.);//螺杆转速
            LabelSetText(mainframe_pg.subclamp_stchrgspd,cbuf);
            sprintf(cbuf,"%6.2f",VarAdrToInt(d_clamp1_STATE_TM_CLAMPCLSHI) / 100.);//锁模时间
            LabelSetText(mainframe_pg.subclamp_stlocktm,cbuf);
        }
        else if(PanelCurPage()==PanelFindPage(PAGE_INJECT))
        {
            WGTSetVisible(mainframe_pg.subclamp, FALSE);
            WGTSetVisible(mainframe_pg.subinject, TRUE);
            WGTSetVisible(mainframe_pg.submain, FALSE);

            if(VarAdrToInt(SYS_FL_MACH_CODE1) & 0x0008)
            {
                sprintf(cbuf,"%6.3f",VarAdrToInt(d_inject1_STATE_TM_INJ2_L) / 1000.);//射出时间
            }
            else
            {
                sprintf(cbuf,"%6.2f",VarAdrToInt(d_inject1_STATE_TM_INJ2_L) / 100.);//射出时间
            }
            LabelSetText(mainframe_pg.subinject_stinjtm,cbuf);
            sprintf(cbuf,"%4.1f",VarAdrToInt(d_inject1_STATE_INJECT_MAXPRESS) / 10.);//最大射压
            LabelSetText(mainframe_pg.subinject_stmaxinjprs,cbuf);
            sprintf(cbuf,"%4.1f",VarAdrToInt(d_inject1_STATE_ADPOSI_INJHOLDEND) / 10.);//残料位置
            LabelSetText(mainframe_pg.subinject_strespos,cbuf);
            sprintf(cbuf,"%4.1f",VarAdrToInt(d_inject1_STATE_ADPOSI_INJCUSHION) / 10.);//保压起点
            LabelSetText(mainframe_pg.subinject_stholdpos,cbuf);
        }
        else
        {
            WGTSetVisible(mainframe_pg.subclamp, FALSE);
            WGTSetVisible(mainframe_pg.subinject, FALSE);
            WGTSetVisible(mainframe_pg.submain, TRUE);

            sprintf(cbuf,"%6.2f",VarAdrToInt(ACT_ACTTIME1) / 100.);//动作时间
            LabelSetText(mainframe_pg.submain_stacttime,cbuf);

            sprintf(cbuf,"%5d",VarAdrToInt(p_PP_STATE_totalOpenCnt));//开模数
            LabelSetText(mainframe_pg.submain_stshotcount,cbuf);

            sprintf(cbuf,"%4.1f",VarAdrToInt(INJECT_CN_CHRGSPD) / 10.);//螺杆转速
            LabelSetText(mainframe_pg.submain_stchrgspd,cbuf);

            if(VarAdrToInt(SYS_FL_MACH_CODE1) & 0x04)//Servo E
            {
                speed_val = VarAdrToUI16(d_Pump1_STATE_MOTOR_SPEED_REAL);
                if(speed_val > 32768)
                {
                    sprintf(cbuf,"%d", speed_val - 65536);
                }
                else
                {
                    sprintf(cbuf,"%d", speed_val);
                }
            }
            else
            {
                if(dbsvox[1].wData[ID_100F] >= 32768)
                {
                    sprintf(cbuf,"%d",dbsvox[1].wData[ID_100F]-65536);
                }
                else
                {
                    sprintf(cbuf,"%d",dbsvox[1].wData[ID_100F]);
                }

            }
            LabelSetText(mainframe_pg.submain_stmotorspd,cbuf); //电机转速
        }
    }
    else
    {
        WGTSetVisible(mainframe_pg.subclamp, FALSE);
        WGTSetVisible(mainframe_pg.subinject, FALSE);
        WGTSetVisible(mainframe_pg.submain, TRUE);

        sprintf(cbuf,"%6.2f",VarAdrToInt(ACT_ACTTIME1) / 100.);//动作时间
        LabelSetText(mainframe_pg.submain_stacttime,cbuf);

        sprintf(cbuf,"%5d",VarAdrToInt(p_PP_STATE_totalOpenCnt));//开模数
        LabelSetText(mainframe_pg.submain_stshotcount,cbuf);

        sprintf(cbuf,"%4.1f",VarAdrToInt(INJECT_CN_CHRGSPD) / 10.);//螺杆转速
        LabelSetText(mainframe_pg.submain_stchrgspd,cbuf);

        if(VarAdrToInt(SYS_FL_MACH_CODE1) & 0x04)//Servo E
        {
            speed_val = VarAdrToUI16(d_Pump1_STATE_MOTOR_SPEED_REAL);
            if(speed_val > 32768)
            {
                sprintf(cbuf,"%d", speed_val - 65536);
            }
            else
            {
                sprintf(cbuf,"%d", speed_val);
            }
        }
        else
        {
            if(dbsvox[1].wData[ID_100F] >= 32768)
            {
                sprintf(cbuf,"%d",dbsvox[1].wData[ID_100F]-65536);
            }
            else
            {
                sprintf(cbuf,"%d",dbsvox[1].wData[ID_100F]);
            }

        }
        LabelSetText(mainframe_pg.submain_stmotorspd,cbuf); //电机转速
    }


    if(VarAdrToUI16(ADMETER_ABSOLUTE1) >= VarAdrToUI16(ADMETER_ZSET1))//20220818 dyl 电子尺正负号判断
    {
        sprintf(cbuf,"%4.1f",VarAdrToInt(ADDR_ADPOSI_REAL1) / 10.);
    }
    else
    {
        sprintf(cbuf,"-%4.1f",VarAdrToInt(ADDR_ADPOSI_REAL1) / 10.);
    }
    LabelSetText(mainframe_pg.stinject,cbuf); //射出位置


    if(VarAdrToUI16(ADMETER_ABSOLUTE2) >= VarAdrToUI16(ADMETER_ZSET2))//20220818 dyl 电子尺正负号判断
    {
        sprintf(cbuf,"%4.1f",VarAdrToInt(ADDR_ADPOSI_REAL2) / 10.);
    }
    else
    {
        sprintf(cbuf,"-%4.1f",VarAdrToInt(ADDR_ADPOSI_REAL2) / 10.);
    }
    LabelSetText(mainframe_pg.stclamp,cbuf); //模座位置

    if(VarAdrToUI16(ADMETER_ABSOLUTE3) >= VarAdrToUI16(ADMETER_ZSET3))//20220818 dyl 电子尺正负号判断
    {
        sprintf(cbuf,"%4.1f",VarAdrToInt(ADDR_ADPOSI_REAL3) / 10.);
    }
    else
    {
        sprintf(cbuf,"-%4.1f",VarAdrToInt(ADDR_ADPOSI_REAL3) / 10.);
    }
    LabelSetText(mainframe_pg.steject,cbuf); //托模位置

    ctrlmodel = VarAdrToInt(SYS_FL_CTRLMODEL);

//    if (VarAdrToInt(SYS_FL_MACH_CODE1) & 0x04)//Servo E
//    {
//        sprintf(cbuf,"%4.1f",VarAdrToInt(d_Pump1_STATE_SERVO_PRESS) / 10.);
//    }
//    else if((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x0004) == 0 && (VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020))
//    {
//        sprintf(cbuf,"%4.1f",VarAdrToInt(d_servo1_STATE_SERVO_PRESS_1) / 10.);
//    }
//    else
//    {
//        sprintf(cbuf,"%4.1f",0.);
//    }
    sprintf(cbuf,"%4.1f",VarAdrToInt(d_Pump1_STATE_SERVO_PRESS) / 10.);//20220818 dyl 系统压力使用d_Pump1_STATE_SERVO_PRESS
    LabelSetText(mainframe_pg.stsystprs,cbuf); //系统压力

    if(PanelCurPage() == PanelFindPage(PAGE_DA_ADJUST))
    {
        g_bDaViewIn = TRUE;
    }
    else
    {
        g_bDaViewIn = FALSE;
    }

    if((g_bDaViewIn) && (VarAdrToUI16(OPMODE_INDEX) == MODE_MANUAL))
    {
        sprintf(cbuf,"%3d",0);
    }
    else
    {
        sprintf(cbuf,"%3d",VarAdrToInt(d_hydr1_STATE_DA_OUTPUT0));
    }
    LabelSetText(mainframe_pg.stpress,cbuf); //P1

    if((g_bDaViewIn) && (VarAdrToUI16(OPMODE_INDEX) == MODE_MANUAL))
    {
        sprintf(cbuf,"%4.1f",0 / 10.);
    }
    else
    {
        sprintf(cbuf,"%4.1f",VarAdrToInt(d_hydr1_STATE_DA_OUTPUT1) / 10.);
    }
    LabelSetText(mainframe_pg.stspeed,cbuf); //F1

    if((g_bDaViewIn) && (VarAdrToUI16(OPMODE_INDEX) == MODE_MANUAL))
    {
        sprintf(cbuf,"%3d",0);
    }
    else
    {
        sprintf(cbuf,"%3d",VarAdrToInt(d_hydr1_STATE_DA_OUTPUT2));
    }
    LabelSetText(mainframe_pg.stpress2,cbuf); //P2

    if((g_bDaViewIn) && (VarAdrToUI16(OPMODE_INDEX) == MODE_MANUAL))
    {
        sprintf(cbuf,"%4.1f",0 / 10.);
    }
    else
    {
        sprintf(cbuf,"%4.1f",VarAdrToInt(d_hydr1_STATE_DA_OUTPUT3) / 10.);
    }
    LabelSetText(mainframe_pg.stspeed2,cbuf); //F2

    if((g_bDaViewIn) && (VarAdrToUI16(OPMODE_INDEX) == MODE_MANUAL))
    {
        sprintf(cbuf,"%3d",0);
    }
    else
    {
        sprintf(cbuf,"%3d",VarAdrToInt(d_hydr1_STATE_DA_OUTPUT4));
    }
    LabelSetText(mainframe_pg.stpress3,cbuf); //P3

    if((g_bDaViewIn) && (VarAdrToUI16(OPMODE_INDEX) == MODE_MANUAL))
    {
        sprintf(cbuf,"%4.1f",0 / 10.);
    }
    else
    {
        sprintf(cbuf,"%4.1f",VarAdrToInt(d_hydr1_STATE_DA_OUTPUT5) / 10.);
    }
    LabelSetText(mainframe_pg.stspeed3,cbuf); //F3
}

/**
 * @brief      : 开关模动作步翻译切换
 *
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200604
 */
const char* ClampStep(UI32 act)
{
//    if(VarAdrToInt(d_clamp1_MOLDSET_FL_CLSOPEN_MODE) == 1)//开关模模式 0-标准   1-平顺    2-比例阀  3-智能
//    {
//        switch (act) {
//        case 0x20111000:
//            return GetTextTran(TEXT_CLOSE_1);
//        case 0x20111100:
//            return GetTextTran(TEXT_CLOSE_2);
//        case 0x20111200:
//            return GetTextTran(TEXT_CLOSE_3);
//        case 0x20111300:
//            return GetTextTran(TEXT_CLOSE_LOWPRS);
//        case 0x20131000:
//            return GetTextTran(TEXT_OPEN_1);
//        case 0x20131100:
//            if(VarAdrToInt(d_clamp1_MOLDSET_OPN_NUM) == 5)
//            {
//                return GetTextTran(TEXT_OPEN_2);
//            }
//            else
//            {
//                return GetTextTran(TEXT_OPEN_1);
//            }
//        case 0x20131200:
//            if((VarAdrToInt(d_clamp1_MOLDSET_OPN_NUM) == 2) || (VarAdrToInt(d_clamp1_MOLDSET_OPN_NUM) == 3))
//            {
//                return GetTextTran(TEXT_OPEN_1);
//            }
//            else if(VarAdrToInt(d_clamp1_MOLDSET_OPN_NUM) == 4)
//            {
//                return GetTextTran(TEXT_OPEN_2);
//            }
//            else
//            {
//                return GetTextTran(TEXT_OPEN_3);
//            }
//        case 0x20131300:
//            if(VarAdrToInt(d_clamp1_MOLDSET_OPN_NUM) == 2)
//            {
//                return GetTextTran(TEXT_OPEN_1);
//            }
//            else if(VarAdrToInt(d_clamp1_MOLDSET_OPN_NUM) == 3)
//            {
//                return GetTextTran(TEXT_OPEN_2);
//            }
//            else if(VarAdrToInt(d_clamp1_MOLDSET_OPN_NUM) == 4)
//            {
//                return GetTextTran(TEXT_OPEN_3);
//            }
//            else
//            {
//                return GetTextTran(TEXT_OPEN_4);
//            }
//        case 0x20131400:
//            return GetTextTran(TEXT_OPEN_END);
//        default:
//            break;
//        }
//    }

    if(VarAdrToInt(d_clamp1_MOLDSET_FL_CLSOPEN_MODE) == 2)//开关模模式 0-标准   1-平顺    2-比例阀  3-智能
    {
        switch (act) {
        case 0x20111000:
            return GetTextTran(TEXT_CLOSE_FAST);
        case 0x20111100:
            return GetTextTran(TEXT_CLOSE_FAST);
        case 0x20111200:
            return GetTextTran(TEXT_CLOSE_FAST);
        case 0x20111300:
            return GetTextTran(TEXT_CLOSE_LOWPRS);
        case 0x20131000:
            return GetTextTran(TEXT_OPEN_START);
        case 0x20131100:
            return GetTextTran(TEXT_OPEN_FAST);
        case 0x20131200:
            return GetTextTran(TEXT_OPEN_FAST);
        case 0x20131300:
            return GetTextTran(TEXT_OPEN_FAST);
        case 0x20131400:
            return GetTextTran(TEXT_OPEN_END);
        default:
            break;
        }
    }
    else if((VarAdrToInt(d_clamp1_MOLDSET_FL_CLSOPEN_MODE) == 3) || (VarAdrToInt(d_clamp1_MOLDSET_FL_CLSOPEN_MODE) == 4))//开关模模式 0-标准   1-平顺    2-比例阀  3-智能
    {
        switch (act) {
        case 0x20111000:
            return GetTextTran(TEXT_CLOSE);
        case 0x20111100:
            return GetTextTran(TEXT_CLOSE);
        case 0x20111200:
            return GetTextTran(TEXT_CLOSE);
        case 0x20111300:
            return GetTextTran(TEXT_CLOSE_LOWPRS);
        case 0x20131000:
            return GetTextTran(TEXT_BREAKMOLD);
        case 0x20131100:
            return GetTextTran(TEXT_OPENMOLD);
        case 0x20131200:
            return GetTextTran(TEXT_OPENMOLD);
        case 0x20131300:
            return GetTextTran(TEXT_OPENMOLD);
        case 0x20131400:
            return GetTextTran(TEXT_OPENMOLD);
        default:
            break;
        }
    }
    else
    {
        switch (act) {
        case 0x20111000:
            return GetTextTran(TEXT_CLOSE_1);
        case 0x20111100:
            return GetTextTran(TEXT_CLOSE_2);
        case 0x20111200:
            return GetTextTran(TEXT_CLOSE_3);
        case 0x20111300:
            return GetTextTran(TEXT_CLOSE_LOWPRS);
        case 0x20131000:
            return GetTextTran(TEXT_OPEN_1);
        case 0x20131100:
            return GetTextTran(TEXT_OPEN_2);
        case 0x20131200:
            return GetTextTran(TEXT_OPEN_3);
        case 0x20131300:
            return GetTextTran(TEXT_OPEN_4);
        case 0x20131400:
            return GetTextTran(TEXT_OPEN_END);
        default:
            break;
        }
    }

    return NULL;
}


static void Alarm_elidedText(char*input, UI16 warnnum)
{
    int fontsize,insize,maxsize,value_type;
    char outbuff[MAX_ALARM_CHAR_NUM];

    if(input == NULL)
    {
        return;
    }

    OBJGetValueByAttrName((POBJ)mainframe_pg.subpage1_lblAlarm,"fontSize",&fontsize,0,NULL,&value_type);//字体大小
    insize = 1;//非0即可

    if(warnnum > 2)
    {
        maxsize = (RU32(mainframe_pg.subpage1_lblAlarm->x_size) - 36)/3;//控件宽度 需要减去 / 的宽度
    }
    else if(warnnum == 2)
    {
        maxsize = (RU32(mainframe_pg.subpage1_lblAlarm->x_size) - 18)/2;;//控件宽度
    }
    else
    {
        maxsize = RU32(mainframe_pg.subpage1_lblAlarm->x_size);//控件宽度
    }

    elidedText(fontsize, input, insize, outbuff, maxsize);

    memcpy(input, outbuff, MAX_ALARM_CHAR_NUM);
}

////20211216 dyl 伺服警报名称显示
//static char* ServoAlarm_Name(UI32 alarm)
//{
//    char filename[255];
//    char ctitle_all[1024];
//    char cnum_alarm[16];
//    char* ctitle_useful;
//    if(GetCurrentLanguage() == LANG_zh_CN)
//    {
//        snprintf(filename,sizeof(filename)-1,"%s%X%s",SERVOWARN_HELP_URL,alarm & 0x0FFF,FILE_SUFFIX);
//    }
//    else
//    {
//        snprintf(filename,sizeof(filename)-1,"%s%X%s",SERVOWARN_HELP_URL_EN,alarm & 0x0FFF,FILE_SUFFIX);
//    }

//    if(FileExist(filename))
//    {
//        strcpy(ctitle_all, HtmlParse_Title(filename));
//        oprintf("ctitle_all: %s", ctitle_all);
//        if (ctitle_all != NULL)
//        {
//            snprintf(cnum_alarm,sizeof(cnum_alarm)-1,"%X",alarm & 0x0FFF);
//            if (strstr(ctitle_all,cnum_alarm) != NULL)
//            {
//                strcpy(ctitle_useful, strstr(ctitle_all,cnum_alarm));
//                oprintf("ctitle_useful: %s", *ctitle_useful);
//                return ctitle_useful;
//            }
//        }
//    }
//    return NULL;
//}

static void ShowAlarm(Mainframe_PG* ppg)
{
    char strid[3][MAX_ALARM_CHAR_NUM];
    static UI32  time_1400ms = 0;
    static UI32  time_800ms  = 0;
    static BOOL  show_switch=FALSE;
    BOOL bTimer=FALSE;
    UI16 i;
    static UI16 wServvoErrId[PumpNum]={0};//增加伺服警报代码,若有多个警报保存多次
    UI16 motorstate;

    UI32 dwWarn[WARN_NUM_MAX]={g_warm1,g_warm2,g_warm3,g_warm4,g_warm5,g_warm6,g_warm7,g_warm8,g_warm9,g_warm10};
    UI16 warnnum = 0;
    BOOL haveservoalarm = FALSE;
    for(i = 0; i < WARN_NUM_MAX; i++)
    {
        if(dwWarn[i] != 0)
        {
            warnnum++;

            if((dwWarn[i] & 0xFF0FFFFF) == 0x0F000101)
            {
                haveservoalarm = TRUE;
            }
        }
    }

    //alarm
    if(!show_switch)
    {
        if(GetTick()-(time_1400ms+time_800ms)>=2000)//1400->2000警报显示时间加长
        {
            time_1400ms = GetTick()-time_800ms;
            bTimer = TRUE;
        }
    }
    else
    {
        if(GetTick() - (time_800ms+time_1400ms) >=800)
        {
            time_800ms =GetTick()-time_1400ms;
            bTimer=TRUE;
        }
    }
    if(bTimer)
    {
        show_switch=!show_switch;
    }

    for(i = 0; i < 3; i++)
    {
        if(dwWarn[i] != 0 && pmap_alarm != NULL)
        {
            if((dwWarn[i] & 0xFF0FFFFF) == 0x0F000101)//泵浦伺服警报 //20191205.cyx
            {
                UI16 wGroup = (dwWarn[i] & 0x00F00000) >> 20;
                UI16 wAlarmID = VarAdrToInt(d_Pump1_STATE_SERVO_ERR + (wGroup-1)*0x100000);
                if(wAlarmID)
                {
//                    snprintf(strid[i],sizeof(strid[i])-1,"#%d%s Er.%03X",wGroup,GetTextTran(TEXT_DRIVER),wAlarmID & 0x0FFF);
                    if (GetServoAlarmTran(wAlarmID & 0x0FFF) != NULL)//20211216 dyl 伺服警报名称显示
                    {
                        snprintf(strid[i],sizeof(strid[i])-1,"#%d%s:Er.%03X%s",wGroup,GetTextTran(TEXT_DRIVER),wAlarmID & 0x0FFF,GetServoAlarmTran(wAlarmID & 0x0FFF));
                    }
                    else
                    {
                        snprintf(strid[i],sizeof(strid[i])-1,"#%d%s:%03X",wGroup,GetTextTran(TEXT_DRIVER),wAlarmID & 0x0FFF);
                    }
                }
                else
                {
                    if(GetMapTran(pmap_alarm,dwWarn[i]) != NULL)
                    {
                        snprintf(strid[i],sizeof(strid[i])-1,"%s",GetMapTran(pmap_alarm,dwWarn[i]));
                    }
                    else
                    {
                        snprintf(strid[i],sizeof(strid[i])-1,"%x",dwWarn[i]);
                    }
                }
            }
            else
            {
                if(GetMapTran(pmap_alarm,dwWarn[i]) != NULL)
                {
                    //通用是冷态启动防止时间 不是预温计时
                    if((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x8B00)
                    {
                        if(dwWarn[i] == 0x0E100101)
                        {
                            snprintf(strid[i],sizeof(strid[i])-1,"%s",GetTextTran(TEXT_ALARM_E100101));
                        }
                        else if(dwWarn[i] == 0x0E200101)
                        {
                            snprintf(strid[i],sizeof(strid[i])-1,"%s",GetTextTran(TEXT_ALARM_E200101));
                        }
                        else
                        {
                            snprintf(strid[i],sizeof(strid[i])-1,"%s",GetMapTran(pmap_alarm,dwWarn[i]));
                        }
                    }
                    else
                    {
                        snprintf(strid[i],sizeof(strid[i])-1,"%s",GetMapTran(pmap_alarm,dwWarn[i]));
                    }
                }
                else
                {
                    snprintf(strid[i],sizeof(strid[i])-1,"%x",dwWarn[i]);
                }
            }
        }
    }

    if(b_MoniWriteFlie)//20230518 chj  开机发现写文件过多标记，闪烁警报10s
    {
        char alarmbuff[255];
        LabelSetText(ppg->subpage1_lblAlarmNum, "1");

        if((pmap_alarm != NULL) && GetMapTran(pmap_alarm,ALARM_WRITE_FILE_CNT_TOO_MUCH) != NULL)
        {
            snprintf(alarmbuff,sizeof(alarmbuff)-1,"%s",GetMapTran(pmap_alarm,ALARM_WRITE_FILE_CNT_TOO_MUCH));
        }
        else
        {
            snprintf(alarmbuff,sizeof(alarmbuff)-1,"%x",ALARM_WRITE_FILE_CNT_TOO_MUCH);
        }
        LabelSetText(ppg->subpage1_lblAlarm, alarmbuff);

        if(show_switch)
        {
            WGTSetVisible(ppg->subpage1,FALSE);
            VarAdrSetStr(BTHELP_SHOW,"Bthelp.png");
        }
        else
        {
            WGTSetVisible(ppg->subpage1,TRUE);
            VarAdrSetStr(BTHELP_SHOW,"Bthelpx.png");
        }
    }
    else if(dwPanelErr == 0x34100101)
    {
        char alarmbuff[255];
        LabelSetText(ppg->subpage1_lblAlarmNum, "1");

        if((pmap_alarm != NULL) && GetMapTran(pmap_alarm,dwPanelErr) != NULL)
        {
            snprintf(alarmbuff,sizeof(alarmbuff)-1,"%s",GetMapTran(pmap_alarm,dwPanelErr));
        }
        else
        {
            snprintf(alarmbuff,sizeof(alarmbuff)-1,"%x",dwPanelErr);
        }
        LabelSetText(ppg->subpage1_lblAlarm, alarmbuff);

        if(show_switch)
        {
            WGTSetVisible(ppg->subpage1,FALSE);
            VarAdrSetStr(BTHELP_SHOW,"Bthelp.png");
        }
        else
        {
            WGTSetVisible(ppg->subpage1,TRUE);
            VarAdrSetStr(BTHELP_SHOW,"Bthelpx.png");
        }
    }
    else if(warnnum > 0)
    {
        char buff[10];
        char alarmbuff[255];

        WordToStr(buff, warnnum, 0);
        LabelSetText(ppg->subpage1_lblAlarmNum, buff);

        if(warnnum > 2)
        {
            Alarm_elidedText(strid[0], warnnum);
            Alarm_elidedText(strid[1], warnnum);
            Alarm_elidedText(strid[2], warnnum);

            snprintf(alarmbuff,sizeof(alarmbuff)-1,"%s / %s / %s", strid[0], strid[1], strid[2]);
        }
        else if(warnnum == 2)
        {
            Alarm_elidedText(strid[0], warnnum);
            Alarm_elidedText(strid[1], warnnum);

            snprintf(alarmbuff,sizeof(alarmbuff)-1,"%s / %s", strid[0], strid[1]);
        }
        else
        {
            Alarm_elidedText(strid[0], warnnum);

            snprintf(alarmbuff,sizeof(alarmbuff)-1,"%s", strid[0]);
        }

        LabelSetText(ppg->subpage1_lblAlarm, alarmbuff);

        if(show_switch)
        {
            WGTSetVisible(ppg->subpage1,FALSE);
            VarAdrSetStr(BTHELP_SHOW,"Bthelp.png");
        }
        else
        {
            WGTSetVisible(ppg->subpage1,TRUE);
            VarAdrSetStr(BTHELP_SHOW,"Bthelpx.png");
        }
    }
    else
    {
        WGTSetVisible(ppg->subpage1, FALSE);
        VarAdrSetStr(BTHELP_SHOW,"Bthelp.png");
    }

    //20191205.cyx 增加伺服警报代码,若有多个警报保存多次
    if(haveservoalarm)
    {
        for(i = 0; i < PumpNum; ++i)
        {
            UI16 errid = VarAdrToInt(d_Pump1_STATE_SERVO_ERR+i*0x100000);
            UI16 wSubGroup = i + 1;
            if(errid)
            {
                if(wServvoErrId[i] != errid)
                {
                    UI16 curno = m_g10_warnrecd.warnHead.cur_no;
                    wServvoErrId[i] = errid;
                    if(G10_AlarmWRRecord(0x0F000101 | (wSubGroup << 20),errid))
                    {
                        //20200810
                        DriveAlarm_ReqServoIdData(wSubGroup, errid, curno);
                    }
                }
            }
        }
    }
    else
    {
        for(i = 0;i < PumpNum;i++)
        {
            if (wServvoErrId[i] != 0)
            {
                wServvoErrId[i] = 0;
            }
        }
    }

    motorstate = VarAdrToUI16(MOTOR_STATE_STATE);
    if(motorstate == MOTOR_START)
    {
        if(bTimer) bMotor = !bMotor;
    }
    else if(motorstate == MOTOR_RUNNING) bMotor = TRUE;
    else bMotor=FALSE;

    if(bMotor)
    {
        WGTSetVisible(ppg->pixMotoron, TRUE);
    }
    else
    {
        WGTSetVisible(ppg->pixMotoron, FALSE);
    }
}

static void MainframeUpdate(Mainframe_PG* ppg)
{
    MOLDSET_ITEM item;
    static UI16 cur_no = 65535;
    static UI16 cur_user = 255;
    static UI16 cur_language = 255;
    static UI32 datetime=0;

    const char* pstep1_trans;
    const char* pstep2_trans;
    const char* pstep3_trans;//20210220
    char step[255];
    char stepx[255];

    char buff[10];
    UI16 heater_state;//20200323

    //windows title
    if(strcmp(VarAdrToStr(MAINFRAME_PAGETITLE),WindowTitle()))
        VarAdrSetStr(MAINFRAME_PAGETITLE,WindowTitle());

    //sys time
    if(datetime != (UI32)time(NULL))
    {
        datetime =(UI32)time(NULL);
        VarAdrSetInt(MAINFRAME_TIME,datetime);
    }   

    //act step
    if(pmap_step!=NULL)
    {
        static I32 last_act1 = -1;
        static I32 last_act2 = -1;
        static I32 last_act3 = -1;//20210220
        static LANG_ID lang_id = LANG_ERR;
        static LANG_ID lang_id_last = LANG_ERR;

        lang_id = GetCurrentLanguage();

        if(last_act1 != VarAdrToInt(ACT_ACTNAME1) || last_act2 != VarAdrToInt(ACT_ACTNAME2)
                 || last_act3 != VarAdrToInt(ACT_ACTNAME3) || lang_id !=lang_id_last) //动作步显示随语言更新 20181207 hz
        {
            last_act1 = VarAdrToInt(ACT_ACTNAME1);
            last_act2 = VarAdrToInt(ACT_ACTNAME2);
            last_act3 = VarAdrToInt(ACT_ACTNAME3);
            lang_id_last = lang_id;
            if((last_act1>=0x20111000 && last_act1<=0x20111300) || (last_act1>=0x20131000 && last_act1<=0x20131400))
            {
                pstep1_trans = ClampStep(last_act1);
            }
            else
            {
                pstep1_trans = GetMapTran(pmap_step,last_act1);
            }

            if((last_act2>=0x20111000 && last_act2<=0x20111300) || (last_act2>=0x20131000 && last_act2<=0x20131400))
            {
                pstep2_trans = ClampStep(last_act2);
            }
            else
            {
                pstep2_trans = GetMapTran(pmap_step,last_act2);
            }

            if((last_act3>=0x20111000 && last_act3<=0x20111300) || (last_act3>=0x20131000 && last_act3<=0x20131400))
            {
                pstep3_trans = ClampStep(last_act3);
            }
            else
            {
                pstep3_trans = GetMapTran(pmap_step,last_act3);
            }

            //20210220
            snprintf(step,sizeof(step)-1,"%s","");
            if(last_act1 != 0)
            {
                if(pstep1_trans !=NULL && (VarAdrToInt(p_PP_MACHSET_ACT_TEXT_NUM) == 0))//20250108 chj 动作部显示数字
                {
                    snprintf(stepx,sizeof(stepx)-1,"%s",pstep1_trans);
                }
                else
                {
                    snprintf(stepx,sizeof(stepx)-1,"%x",last_act1);
                }
                strcat(step,stepx);
            }

            if(last_act2 != 0)
            {
                if(pstep2_trans !=NULL && (VarAdrToInt(p_PP_MACHSET_ACT_TEXT_NUM) == 0))//20250108 chj 动作部显示数字
                {
                    snprintf(stepx,sizeof(stepx)-1,"%s",pstep2_trans);
                }
                else
                {
                    snprintf(stepx,sizeof(stepx)-1,"%x",last_act2);
                }
                strcat(step," ");
                strcat(step,stepx);
            }

            if(last_act3 != 0)
            {
                if(pstep3_trans !=NULL && (VarAdrToInt(p_PP_MACHSET_ACT_TEXT_NUM) == 0))//20250108 chj 动作部显示数字
                {
                    snprintf(stepx,sizeof(stepx)-1,"%s",pstep3_trans);
                }
                else
                {
                    snprintf(stepx,sizeof(stepx)-1,"%x",last_act3);
                }
                strcat(step," ");
                strcat(step,stepx);

            }
            VarAdrSetStr(MAINFRAME_ACTSETP,step);
        }
    }

    SetDataShow();//主页面数据显示
    ShowAlarm(ppg);//警报显示

    // heater and motor img 20190218 hz
    if(IV3200 || IV5200 || IV3300 || IV5300 || IV3100)
    {
        heater_state = VarAdrToUI16(d_TempCtrlComm1_STATE_FL_HEATERSTATUS);//0E160038 E502  E506
    }
    else
    {
        heater_state = VarAdrToUI16(d_tempctrl1_STATE_FL_HEATERSTATUS); //0516002B
    }
    if(heater_state)
    {
        WGTSetVisible(ppg->pixHeaterx, TRUE);
    }
    else
    {
        WGTSetVisible(ppg->pixHeaterx, FALSE);
    }

    if(GetMtpTempHeatState())
    {
        WGTSetVisible(ppg->pixMtpHeatx, TRUE);
    }
    else
    {
        WGTSetVisible(ppg->pixMtpHeatx, FALSE);
    }

    //mold name 20190218 hz
    if(cur_no != g_moldset.cur_no || g_moldset_head.bchange==TRUE)
    {     
        cur_no = g_moldset.cur_no;
        g_moldset_head.bchange = FALSE;
        if(CMOD_RITEM_EX(g_moldset.cur_no, &item))
        {     
            LabelSetText(ppg->stMoldName, item.sName);
        }
        else
        {
            LabelSetText(ppg->stMoldName, "");
        }
    }

    //user level //20191112.cyx
    if(cur_user != GetCurrentUserId())
    {
        cur_user = GetCurrentUserId();
        if(GetCurrentUserId() >= USER_MANUFACTURER)
        {
            sprintf(buff,"lv4.png");
        }
        else
        {
            sprintf(buff,"lv%d.png",GetCurrentUserId()+1);
        }

        VarAdrSetStr(USER_LV_SHOW,buff);
    }

    //language //20191112.cyx
    if(cur_language != GetCurrentLanguage())
    {
        cur_language = GetCurrentLanguage();
        if(GetCurrentLanguage() == LANG_en)
        {
            sprintf(buff,"en.png");
        }
        else if(GetCurrentLanguage() == LANG_zh_CN)
        {
            sprintf(buff,"ch.png");
        }
        else if(GetCurrentLanguage() == LANG_fa)
        {
            sprintf(buff,"ps.png");
        }
        else if(GetCurrentLanguage() == LANG_ru)
        {
            sprintf(buff,"rus.png");
        }
        else if(GetCurrentLanguage() == LANG_es)
        {
            sprintf(buff,"spn.png");
        }
        else if(GetCurrentLanguage() == LANG_tr)
        {
            sprintf(buff,"tk.png");
        }
        else if(GetCurrentLanguage() == LANG_pt)
        {
            sprintf(buff,"port.png");
        }
        else if(GetCurrentLanguage() == LANG_ar)
        {
            sprintf(buff,"ar.png");
        }
        else if(GetCurrentLanguage() == LANG_fr)
        {
            sprintf(buff,"fr.png");
        }
        else if(GetCurrentLanguage() == LANG_ko)
        {
            sprintf(buff,"kor.png");
        }
        else if(GetCurrentLanguage() == LANG_vi)
        {
            sprintf(buff,"vn.png");
        }

        VarAdrSetStr(LANGUAGE_SHOW,buff);
    }

    PicModeUpdate(); //20191217.cyx
}

static void TempPump(int timerid, void* data)
{
    UI16 bit;
    bit = VarAdrToInt(PART_REQ_STATE);
    if(getbit(bit,0)==0 && getbit(bit, 1)==0)
    {
        ShowMsg(VM_MSG_TEMP_PUMP_ABNORMAL)
    }
    else if(getbit(bit, 0) == 0)
    {
        ShowMsg(VM_MSG_TEMP_ABNORMAL)
    }
    else if(getbit(bit, 1) == 0)
    {
        ShowMsg(VM_MSG_PUMP_ABNORMAL)
    }
}

#define  DIAOGE_WAKE_SCREEN  "Wake_Screen"
static void G15MainframeUpdate()
{
    static BOOL  bshowwake = FALSE;

    if(VarAdrToInt(KEYTEST_ADDR) == 0)//屏蔽按键测试画面
    {
        if(VarAdrToInt(PT15_SCREEN_PROTECT))
        {
            bshowwake = TRUE;
            PanelShowDialogByName(DIAOGE_WAKE_SCREEN);
        }
        else if(bshowwake)
        {
            bshowwake = FALSE;
            PanelCloseDialogByName(DIAOGE_WAKE_SCREEN);
        }
    }

    /*********** Touch start ***********/ //20210914 dyl
    static PPAGE_FRAM pcur;
    pcur = PanelCurPage();
    //hardware lock
    if(VarAdrToInt(HARDWARE_LOCK) && (pcur != PanelFindPage(PAGE_NEXTPAGE)))
    {
        if(!pcur->bprotect)
        {
            pcur->bprotect = TRUE;
        }
    }
    else
    {
        if(GetPageLockMode() == MODE_NOLOCK)
        {
            if(pcur->bprotect)
            {
                pcur->bprotect = FALSE;
            }
        }
        else if((GetPageLockMode() == MODE_USERLOCK)&&(!VarAdrToInt(USER_KEYLOCK)))
        {
            if(pcur->bprotect)
            {
                pcur->bprotect = FALSE;
            }
        }
        else if((GetPageLockMode() == MODE_MODELOCK)&&(!((OperateModeIndex() != MODE_MANUAL) || JudgeChargeActStep() || JudgeSuckBackActStep())))//20230518 chj增加射退动作判断
        {
            if(pcur->bprotect)
            {
                pcur->bprotect = FALSE;
            }
        }
        else if((GetPageLockMode() == MODE_USER_MODELOCK)&&(!((OperateModeIndex() != MODE_MANUAL) || JudgeChargeActStep() || JudgeSuckBackActStep()))&&(!VarAdrToInt(USER_KEYLOCK)))//20230518 chj增加射退动作判断
        {
            if(pcur->bprotect)
            {
                pcur->bprotect = FALSE;
            }
        }
    }
    //show lock
    if(pcur->bprotect)
    {
        if(VarAdrToInt(HARDWARE_LOCK))//20200409.cyx 图标分为硬件锁和软件锁
        {
            WGTSetVisible(mainframe_pg.pixlock, TRUE);
            if(((GetPageLockMode()==MODE_USERLOCK)&&VarAdrToInt(USER_KEYLOCK))
                    || ((GetPageLockMode()==MODE_MODELOCK)&&((OperateModeIndex()!=MODE_MANUAL) || JudgeChargeActStep() || JudgeSuckBackActStep()))//20230518 chj增加射退动作判断
                    || ((GetPageLockMode()==MODE_USER_MODELOCK)&&(((OperateModeIndex() != MODE_MANUAL) || JudgeChargeActStep() || JudgeSuckBackActStep()) || VarAdrToInt(USER_KEYLOCK))))//20230518 chj增加射退动作判断
            {
                WGTSetVisible(mainframe_pg.pixlock2, TRUE);
            }
            else
            {
                WGTSetVisible(mainframe_pg.pixlock2, FALSE);
            }
        }
        else
        {
            WGTSetVisible(mainframe_pg.pixlock, FALSE);
            WGTSetVisible(mainframe_pg.pixlock2, TRUE);
        }
    }
    else
    {
        WGTSetVisible(mainframe_pg.pixlock, FALSE);
        WGTSetVisible(mainframe_pg.pixlock2, FALSE);
    }
    /*********** Touch end   ***********/
}

static BOOL MainPortIsOk()
{
    if((ProtRxCnt() > 60) && MainProtComIsOK())
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//射出速度设定值小数点随CODE码取消而做保护
static void ChangeInjSpdValueByPoint()
{
    UI32 InjSpdList[]={
    d_inject1_ACTPARA_DAFLOW_INJ1, d_inject1_ACTPARA_DAFLOW_INJ2, d_inject1_ACTPARA_DAFLOW_INJ3,
    d_inject1_ACTPARA_DAFLOW_INJ4, d_inject1_ACTPARA_DAFLOW_INJ5, d_inject1_ACTPARA_DAFLOW_INJ6,
    d_inject1_ACTPARA_DAFLOW_INJHOLD1, d_inject1_ACTPARA_DAFLOW_INJHOLD2, d_inject1_ACTPARA_DAFLOW_INJHOLD3,
    d_inject1_ACTPARA_DAFLOW_INJHOLD4, d_inject1_ACTPARA_DAFLOW_INJHOLD5, d_inject1_ACTPARA_DAFLOW_INJSLOW
    };
    UI16 value = 0;
    UI16 value_Max = VarAdrToInt(d_autoctrl1_MOLDSET_DAFLOW_INJECTLIMIT);
    int i;
    UI16 dot = 0;

    if ((VarAdrToInt(SYS_FL_MACH_CODE5) & 0x0100) != 0)
    {
        dot = 1;
    }
    else
    {
        dot = 0;
    }


    for (i = 0; i < (sizeof(InjSpdList)/sizeof(UI32)); i++)
    {
        VarSetDot(VarAdrToVar(InjSpdList[i]),dot);//20210305 取值前先刷新地址关联的小数位 否则第一次VarAdrToInt得到的值是一个错误的值
        value = VarAdrToInt(InjSpdList[i]);
        if (value > value_Max)
        {
            VarSendSaveSetIntByAdr(InjSpdList[i],value_Max);
        }
    }
}

//储料背压设定值小数点随CODE码取消而做保护
static void ChangeBackPrsValueByPoint()
{
    UI32 BackPrsList[]={
    d_inject1_ACTPARA_DAPRES_CHGBACK1, d_inject1_ACTPARA_DAPRES_CHGBACK2, d_inject1_ACTPARA_DAPRES_CHGBACK3,
    d_inject1_ACTPARA_DAPRES_CHGBACK4, d_inject1_ACTPARA_DAPRES_CHGBACK5
    };
    UI16 value = 0;
    UI16 value_Max = VarAdrToInt(d_autoctrl1_MOLDSET_DAPRES_BACKLIMIT);
    int i;
    UI16 dot = 0;

    if ((VarAdrToInt(SYS_FL_MACH_CODE5) & 0x1000) != 0)
    {
        dot = 1;
    }
    else
    {
        dot = 0;
    }

    for (i = 0; i < (sizeof(BackPrsList)/sizeof(UI32)); i++)
    {
        VarSetDot(VarAdrToVar(BackPrsList[i]),dot);//20210305 取值前先刷新地址关联的小数位 否则第一次VarAdrToInt得到的值是一个错误的值
        value = VarAdrToInt(BackPrsList[i]);
        if (value > value_Max)
        {
            VarSendSaveSetIntByAdr(BackPrsList[i],value_Max);
        }
    }
}

//调模齿轮比code码防超限
static void ChangeAdjFactorByPoint()
{
    UI32 AdjFactor[]={
    d_adjust1_MACHSET_CNPOSI_ADJPROPERTIONAL
    };
    UI16 value = 0;
    UI16 value_Max = 1000;
    int i;
    UI16 dot = 0;

    if ((VarAdrToInt(SYS_FL_MACH_CODE0)&0x80) != 0)
    {
        dot = 3;
    }
    else
    {
        dot = 1;
    }

    for (i = 0; i < (sizeof(AdjFactor)/sizeof(UI32)); i++)
    {
        VarSetDot(VarAdrToVar(AdjFactor[i]),dot);//20210305 取值前先刷新地址关联的小数位 否则第一次VarAdrToInt得到的值是一个错误的值
        value = VarAdrToInt(AdjFactor[i]);
        if (value > value_Max)
        {
            VarSendSaveSetIntByAdr(AdjFactor[i],value_Max);
        }
    }
}

//code码变化导致小数位变化处理
static void CodeToPointChange()
{
    static UI16 wInjSpdPointCode = 0x100;
    static UI16 wBackPrsPointCode = 0x1000;
    static UI16 wAdjFactorPointCode = 0x80;

    if(MainPortIsOk())
    {
        if ((VarAdrToInt(SYS_FL_MACH_CODE5)&0x100) != wInjSpdPointCode)
        {
            ChangeInjSpdValueByPoint();
            wInjSpdPointCode = VarAdrToInt(SYS_FL_MACH_CODE5)&0x100;
        }
        if ((VarAdrToInt(SYS_FL_MACH_CODE5)&0x1000) != wBackPrsPointCode)
        {
            ChangeBackPrsValueByPoint();
            wBackPrsPointCode = VarAdrToInt(SYS_FL_MACH_CODE5)&0x1000;
        }
        if ((VarAdrToInt(SYS_FL_MACH_CODE0)&0x80) != wAdjFactorPointCode)
        {
            ChangeAdjFactorByPoint();
            wAdjFactorPointCode = VarAdrToInt(SYS_FL_MACH_CODE0)&0x80;
        }
    }
}

static void CtlConnect()
{
}
static int MainframeOnInit(Mainframe_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
#ifdef A40I //20220614 dyl
    LabelSetText(ppg->label1, "HD06");
#else
    LabelSetText(ppg->label1, "HB06");
#endif

    VarAdrSetStr(0x3317003d,"btnMAIN.png");//初始化主画面常态显示
    VarAdrSetStr(0x3317003e,"btnNEXT.png");//初始化换页常态显示
    VarAdrSetInt(0x331700b9,0x00000000);//初始化主画面文本色显示
    VarAdrSetInt(0x331700ba,0x00000000);//初始化换页文本色显示

    ParseCSVFile(ACTSETP_FILE,&pmap_step);

    //机器参数转成模具参数特殊处理
    //20160926解决途中机械手功能非模具数据的问题
    VarSendSaveSetIntByAdr(ROBOT_FL_ROBOTCLPMID,VarAdrToUI16(CLAMP_MACHSET_FL_ADPOSROBOT));
    //20161008解决机械手输出时间非模具数据的问题
    VarSendSaveSetIntByAdr(ROBOT_TM_ROBOTMID,VarAdrToUI16(CLAMP_MACHSET_TM_MANUALOUT));
    //20170411
    VarSendSaveSetIntByAdr(SYSTEM_MACH_ADPOSI_ADJTARG,VarAdrToUI16(ADJUST_MACH_ADPOSI_ADJTARG));

    //20200325调模调试功能每次重启赋0
    VarSendSaveSetIntByAdr(d_adjust1_MACHSET_FL_JOG_FUN,0);
    //储料走时间每次重启赋0
    VarSendSaveSetIntByAdr(d_inject1_MACHSET_FL_CHARGEBYTM,0);
    //干循环功能每次重启赋0
    VarSendSaveSetIntByAdr(d_machine1_MOLDSET_FL_WET_DRY_CYCLES,0);
    //驱动器FUNC曲线显示每次重启赋0 //20200623
    VarSendSaveSetIntByAdr(d_servo1_MACHSET_FL_SERVO_CURVE,0);
    //版本信息下传下位机
    VarSendSaveSetIntByAdr(d_machine1_MOLDSET_UPPER_CP_VERSION1,m_verinfo.wPanelViewVer1);
    VarSendSaveSetIntByAdr(d_machine1_MOLDSET_UPPER_CP_VERSION2,m_verinfo.wPanelViewVer2);
    VarSendSaveSetIntByAdr(d_machine1_MOLDSET_UPPER_CP_CTRLDATE1,m_verinfo.wPanelDate1);
    VarSendSaveSetIntByAdr(d_machine1_MOLDSET_UPPER_CP_CTRLDATE2,m_verinfo.wPanelDate2);

    //初始化开关模平稳状态 20241017 chj 增加开关模平稳
    if(VarAdrToInt(p_PP_MACHSET_SWITICHING_MODE_SMOOTH) == 0)
    {
        if(VarAdrToInt(d_clamp1_MOLDSET_FL_LOCATE_DA))
        {
            VarAdrSetInt(d_clamp1_MOLDSET_FL_LOCATE_DA, 0);
            VarAdrSaveInt(d_clamp1_MOLDSET_FL_LOCATE_DA);
        }
        if(VarAdrToInt(d_clamp1_MOLDSET_FL_OPN_LOCATE))
        {
            VarAdrSetInt(d_clamp1_MOLDSET_FL_OPN_LOCATE, 0);
            VarAdrSaveInt(d_clamp1_MOLDSET_FL_OPN_LOCATE);
        }
        if(VarAdrToInt(d_clamp1_MOLDSET_FL_CLS_LOCATE))
        {
            VarAdrSetInt(d_clamp1_MOLDSET_FL_CLS_LOCATE, 0);
            VarAdrSaveInt(d_clamp1_MOLDSET_FL_CLS_LOCATE);
        }
    }

    //开机时高压二段压力保持与高压一段(锁模）压力相同
    if(VarAdrToInt(d_clamp1_ACTPARA_DAPRES_CLSHIPRES2) != VarAdrToInt(d_clamp1_ACTPARA_DAPRES_CLSHIGH))
    {
        VarSendSaveSetIntByAdr(d_clamp1_ACTPARA_DAPRES_CLSHIPRES2, VarAdrToInt(d_clamp1_ACTPARA_DAPRES_CLSHIGH));
    }

    if (VarAdrToInt(d_clamp1_MACHSET_FL_CLSHIPRES2) == FALSE)//20211105 dyl HighPrs#2 //20230518 chj
    {
        if (VarAdrToInt(p_PP_MACHSET_CLAMPVIEW_HIGH2_VISIBLE) != FALSE)
        {
            VarSendSaveSetIntByAdr(p_PP_MACHSET_CLAMPVIEW_HIGH2_VISIBLE, FALSE);
        }
    }

    //20200731 开模终点模板位置 不经推力座转换
    if(VarAdrToInt(d_clamp1_ACTPARA_ADPOSI_OPENENDEND_MOLD) != VarAdrToInt(d_clamp1_ACTPARA_ADPOSI_OPENENDEND))
    {
        VarSendSaveSetIntByAdr(d_clamp1_ACTPARA_ADPOSI_OPENENDEND_MOLD, VarAdrToInt(d_clamp1_ACTPARA_ADPOSI_OPENENDEND));
    }

    //20200819
    if (VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE)==0)
    {
        if  (VarAdrToInt(d_inject1_HYDR_SERVO_KP_INJCT) != VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_HIGH))
        {
            VarSendSaveSetIntByAdr(d_inject1_HYDR_SERVO_KP_INJCT,VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_HIGH));
        }
        if  (VarAdrToInt(d_inject1_HYDR_SERVO_KI_INJCT) != VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KI_INJCT_HIGH))
        {
            VarSendSaveSetIntByAdr(d_inject1_HYDR_SERVO_KI_INJCT,VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KI_INJCT_HIGH));
        }

        if  (VarAdrToInt(d_inject1_HYDR_SERVO_KP_HOLD) != VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_HOLD_HIGH))
        {
            VarSendSaveSetIntByAdr(d_inject1_HYDR_SERVO_KP_HOLD,VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_HOLD_HIGH));
        }
        if  (VarAdrToInt(d_inject1_HYDR_SERVO_KI_HOLD) != VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KI_HOLD_HIGH))
        {
            VarSendSaveSetIntByAdr(d_inject1_HYDR_SERVO_KI_HOLD,VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KI_HOLD_HIGH));
        }
    }
    else if (VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE)==1)
    {
        if  (VarAdrToInt(d_inject1_HYDR_SERVO_KP_INJCT) != VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MID))
        {
            VarSendSaveSetIntByAdr(d_inject1_HYDR_SERVO_KP_INJCT,VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MID));
        }
        if  (VarAdrToInt(d_inject1_HYDR_SERVO_KI_INJCT) != VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KI_INJCT_MID))
        {
            VarSendSaveSetIntByAdr(d_inject1_HYDR_SERVO_KI_INJCT,VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KI_INJCT_MID));
        }

        if  (VarAdrToInt(d_inject1_HYDR_SERVO_KP_HOLD) != VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_HOLD_MID))
        {
            VarSendSaveSetIntByAdr(d_inject1_HYDR_SERVO_KP_HOLD,VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_HOLD_MID));
        }
        if  (VarAdrToInt(d_inject1_HYDR_SERVO_KI_HOLD) != VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KI_HOLD_MID))
        {
            VarSendSaveSetIntByAdr(d_inject1_HYDR_SERVO_KI_HOLD,VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KI_HOLD_MID));
        }
    }
    else if (VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_MODE)==2)
    {
        if  (VarAdrToInt(d_inject1_HYDR_SERVO_KP_INJCT) != VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_LOW))
        {
            VarSendSaveSetIntByAdr(d_inject1_HYDR_SERVO_KP_INJCT,VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_INJCT_LOW));
        }
        if  (VarAdrToInt(d_inject1_HYDR_SERVO_KI_INJCT) != VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KI_INJCT_LOW))
        {
            VarSendSaveSetIntByAdr(d_inject1_HYDR_SERVO_KI_INJCT,VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KI_INJCT_LOW));
        }

        if  (VarAdrToInt(d_inject1_HYDR_SERVO_KP_HOLD) != VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_HOLD_LOW))
        {
            VarSendSaveSetIntByAdr(d_inject1_HYDR_SERVO_KP_HOLD,VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KP_HOLD_LOW));
        }
        if  (VarAdrToInt(d_inject1_HYDR_SERVO_KI_HOLD) != VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KI_HOLD_LOW))
        {
            VarSendSaveSetIntByAdr(d_inject1_HYDR_SERVO_KI_HOLD,VarAdrToInt(d_autoctrl1_MOLDSET_SYS_SERVO_KI_HOLD_LOW));
        }
    }

    //20230518 chj 此处有个bug，当面板数据和结构参数一起导入的时候，如果est_cur.tag文件大小不同，
    //即中间加过参数，再结合结构参数内的dic，可能会导致数据错乱，所以改为如果导入过数据，开机读取结构参数
    if(FileExist(PANEL_MACHVALUE_FILE))
    {
        oprintf("exit MachValue file --------\n");
        char scmdMach[128];
        snprintf(scmdMach,sizeof(scmdMach)-1, "rm -f %s",PANEL_MACHVALUE_FILE);
        osystem(scmdMach);
        LoadDataValueExcel(MACHSET_PATH, TRUE);
    }

    if(VarAdrToInt(USER_INITRANK) == USER_EVERYONE)//20191120.cyx
    {
        VarAdrSetInt(USER_KEYLOCK,1);
        SetCurrentUserId(USER_EVERYONE);
    }
    else
    {
        VarAdrSetInt(USER_KEYLOCK,0);
        SetCurrentUserId(VarAdrToUI16(USER_INITRANK));
    }

    KeySetKeySoundRelAddr(BUZZER_SOUND); //20191213.cyx 蜂鸣器开关绑定数据库变量

    TouchMainframeInit();//20210914 dyl touch

    InitHMDLub();
    CheckHMDLubSave();
    CalculateHMDLub();

    //20200325 显示新页面
    VarAdrSetInt(SERVO_CURRENT_NUM,1);
    VarAdrSetInt(TEMP_CTRL_PART,1);

    return 0;
}
static int MainframeOnShow(Mainframe_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int MainframeOnHide(Mainframe_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int MainframeOnUpdate(Mainframe_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    int timer;
    static BOOL once_main = TRUE;
    static int mtcount = -1;
    static int wlastFactCode = -1;
    if(MainPortIsOk() && once_main)
    {
        timer = CreateTimer(10000, 1, TempPump, NULL);
        StartTimer(timer);
        once_main = FALSE;
    }
    else if(!MainPortIsOk())
    {
        once_main = TRUE;
    }
    MainframeUpdate(ppg);

//20220426
//#ifdef CAPACITIVESCREEN
//    if(IsTouchType())//20210914 dyl touch
//    {
        G15MainframeUpdate();
//    }
//#endif

    if(mtcount != VarAdrToInt(MT_BOARD_COUNT))
    {
        mtcount = VarAdrToInt(MT_BOARD_COUNT);
        MoldTempFuncClose();
    }

    //通用润滑模式为1
    if(wlastFactCode != (VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00))
    {
        wlastFactCode = (VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00);
        if((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x8B00)
        {
            if(VarAdrToInt(d_lub1_MACHSET_FL_LUBMODE) != 1)
            {
                VarSendSaveSetIntByAdr(d_lub1_MACHSET_FL_LUBMODE, 1);
            }

            if(VarAdrToInt(d_lub2_MACHSET_FL_LUBMODE) != 1)
            {
                VarSendSaveSetIntByAdr(d_lub2_MACHSET_FL_LUBMODE, 1);
            }

            if(VarAdrToInt(d_lub3_MACHSET_FL_LUBMODE) != 1)
            {
                VarSendSaveSetIntByAdr(d_lub3_MACHSET_FL_LUBMODE, 1);
            }
        }
    }

    CodeToPointChange();

    if(b_RecMainNeedReStartCmd)//20230518 chj 收到主机请求重启命令，弹出重启框，重启主机
    {
        b_RecMainNeedReStartCmd = FALSE;
        g_question.pOkfunc = RecMainNeedReStartCmd;
        g_question.pCancelfunc = NULL;
        ShowQuestion(VM_MSG_MAIN_CTRL_RESTER,g_question);
    }

    if(bMianNeedBoot && GetMainHand())
    {
        bMianNeedBoot = FALSE;
        ShowMsg(VW_MSG_HOSTSTARTOVER);
    }

    return 0;
}
#include "keydef.h"
static int MainframeOnChange(Mainframe_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(pwgt == ppg->button1)
    {
        SendKey(KEY_FUN1);
    }
    else if(pwgt == ppg->button2)
    {
        SendKey(KEY_FUN2);
    }
    else if(pwgt == ppg->button3)
    {
        SendKey(KEY_FUN3);
    }
    else if(pwgt == ppg->button4)
    {
        SendKey(KEY_FUN4);
    }
    else if(pwgt == ppg->button5)
    {
        SendKey(KEY_FUN5);
    }
    else if(pwgt == ppg->button6)
    {
        SendKey(KEY_FUN6);
    }
    else if(pwgt == ppg->button7)
    {
        SendKey(KEY_FUN7);
    }
    else if(pwgt == ppg->btlanguage)
    {
        SendKey(KEY_FUN8);
    }
    else if(pwgt == ppg->btuser)
    {
#ifdef WIN32
        if(GetPanelType() == PANEL_V10)
        {
            SendKey(KEY_NEXTPAGE); //10寸面板windows版本用户键当作换页键用
            return TRUE;
        }
#endif
        SendKey(_KEY_VIEW_USER);
    }
    else if(pwgt == PageGetWgtByName(mainframe_pg_name,"btnMain"))
    {
        SendKey(KEY_MAIN);
    }
    else if(pwgt == PageGetWgtByName(mainframe_pg_name,"btnNEXT"))
    {
        SendKey(KEY_NEXTPAGE);
    }
    else if(pwgt == PageGetWgtByName(mainframe_pg_name,"btcut"))
    {
        SendKey(_KEY_GRAB_PNGIMAGE);
    }
    else if(pwgt == PageGetWgtByName(mainframe_pg_name,"bthelp"))
    {
        if((VarAdrToInt(p_PP_MACHSET_SPC_KEY_USE_JY) == 1) && (PanelCfgSize()==DISPLAY_SIZE_10) && ((VarAdrToInt(d_machine1_MACHSET_FL_CUSTOMID)&0xFF00) == 0x6400))//20241101 金鹰10寸快速机特殊面膜按键修改
        {
            ;
        }
        else
        {
            SendKey(KEY_HELP);
        }

    }

    return 0;
}
