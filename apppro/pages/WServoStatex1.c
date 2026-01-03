#include "WServoStatex1.h"
#include "typedef.h"
#include "pagefram.h"
#include "database.h"
#include "stdio.h"
#include "servo.h"
#include "canmod.h"
#include "texttbl.h"
#include "protcmd.h"
#include "common.h"
#include "buttonwgt.h"
#include "kjump.h"
#include "dataCfgtb.h"
#include "prot.h"
#include "privdef.h"
#include "ctrlcmd.h"
#include "keymod.h"
#include "key.h"
#include "tabpagewgt.h"
#include "imagewgt.h"
#include "editwgt.h"
#include "labelwgt.h"
#include "usermod.h"//20211117 dyl 伺服状态画面等级放低到LV2

DEFINE_SERVOSTATEX1_MAP

CONNECT_SERVOSTATEX1_EVENT

#define TMP_SERVO_SERVOID	     TMPVAL(50)   //伺服驱动ID 临时变量50
//comm state date
#define TMP_SERVO_PANELTX	     TMPVAL(60)   //面板与驱动发送   临时变量60
#define TMP_SERVO_PANELRX	     TMPVAL(61)   //面板与驱动接收   临时变量61
#define TMP_SERVO_MAINTX	     TMPVAL(63)   //主机与面板发送 临时变量63
#define TMP_SERVO_MAINRX	     TMPVAL(64)   //主机与面板接收 临时变量64

#define SERVO_PDLOSTBG	     0x331300e2  //面板与驱动丢包背景色
#define SERVO_PMLOSTBG	     0x331300e3  //面板与主机丢包背景色
#define SERVO_MDLOSTBG	     0x331300e4  //主机与驱动丢包背景色

#define TMP_SERVO_COMMTEST	     TMPVAL(400)  //COMM TEST  临时变量400

#define TMP_SERVO_PANELLOST	     TMPSTR(70)   //面板与驱动丢包率 临时字符串变量70
#define TMP_SERVO_MAINLOST 	     TMPSTR(71)   //主机与面板丢包率 临时字符串变量71
#define TMP_SERVO_DRIVELOST 	 TMPSTR(72)   //主机与驱动丢包率 临时字符串变量72

#define SERVO_MACH_WRITECNT 0x0A120013  //主机-驱动器 发送
#define SERVO_MACH_READCNT  0x0A120014  //主机-驱动器 接收

#define SERVO_RS232_RX      0x331300e0  //232主机与面板接收
#define SERVO_RS232_TX      0x331300e1  //232主机与面板发送

#define SHOW_ITEM           TMPVAL(123) //当前page显示的4个伺服是否显示

extern DB_SERVOS_EX g_dbsvosx;
extern DB_SERVOTYPT g_dbsvotype;

extern PHASH_TBL pmap_runstatetbl;//servo run state
extern PHASH_TBL pmap_tblx1;//can state

extern UI8 nServoId;
static BOOL btest=FALSE;
static int old_page;

//伺服导航栏换页键特殊处理
static BOOL nextpage_press = FALSE;
static BOOL KeyNextPageHandle(int key,int press)
{
    BOOL isservopage = FALSE;
    PPAGE_FRAM pf_servostatex1 = PanelFindPage(PAGE_SERVOSTATEX1);

    if(strcmp(VarAdrToStr(p_PP_MACH2_JKEY_PRESS_IMG1),"btnSTATx.png") == 0)
    {
        isservopage = TRUE;
    }

    if(isservopage && (VarAdrToInt(SERVO_CURRENT_NUM) > 6) && (GetCurrentUserId() >= USER_DEALER))//20211117 dyl 伺服状态画面等级放低到LV2
    {
        if(NaviGetIndex(pf_servostatex1) == 1)
        {
            NaviSetIndex(pf_servostatex1, 2);
        }
        else if(NaviGetIndex(pf_servostatex1) == 2)
        {
            NaviSetIndex(pf_servostatex1, 1);
        }
        nextpage_press = TRUE;
        PanelShowPage(pf_servostatex1);

        return TRUE;
    }

    PanelShowPageByName(PAGE_NEXTPAGE);
    return FALSE;
}

static void ServoTestPageIn(ServoStatex1_PG* ppg)
{
    g_dbsvosx.wToDriver=0;
    g_dbsvosx.wFromDriver=0;
    g_dbsvosx.wToControl=0;
    g_dbsvosx.wFromControl=0;

    btest=FALSE;

    ButtonSetText(ppg->button42,GetTextTran(TEXT_TESTBEGIN));
//    WGTSetEnable(ppg->dropdown1,TRUE);
//    VarAdrSetInt(TMP_SERVO_SERVOID,nServoId-1);//默认显示驱动器1
}

static void ServoTestPageOut()
{
    UI16 pdata[2] ={0,0};

    //close comm test
    if(btest)
    {
        ProtSysCtrl(CAN_TEST_CMD,2,pdata);
    }
    VarAdrSetInt(TMP_SERVO_COMMTEST,0);
}

static void ServoTestUpdate()
{
    char cbuf[10];
    int wLost;
    int sendCnt,receiveCnt;
    static UI32 time_100ms =0;
    static UI32 time_1s =0;

    //通讯数据包
    VarAdrSetInt(TMP_SERVO_PANELTX,g_dbsvosx.wToDriver);
    VarAdrSetInt(TMP_SERVO_PANELRX,g_dbsvosx.wFromDriver);
    VarAdrSetInt(TMP_SERVO_MAINTX,g_dbsvosx.wToControl);
    VarAdrSetInt(TMP_SERVO_MAINRX,g_dbsvosx.wFromControl);
    VarAdrSetInt(SERVO_RS232_TX,ProtTxCnt());
    VarAdrSetInt(SERVO_RS232_RX,ProtRxCnt());

    if((GetTick()-time_1s)>1000)
    {
        time_1s = GetTick();
        sendCnt    = g_dbsvosx.wToDriver;
        receiveCnt = g_dbsvosx.wFromDriver;
        if(g_dbsvosx.wToDriver == 0)
            wLost = 0;
        else
        {
            wLost = ((sendCnt-receiveCnt)*100)/sendCnt;
        }
        if(wLost>=0)
        {
            WordToStr(cbuf,wLost,0);
        }
        else
        {
            snprintf(cbuf,sizeof(cbuf)-1,"%d",wLost);
        }
        VarAdrSetStr(TMP_SERVO_PANELLOST,cbuf);
        if(wLost>10)
        {
            VarAdrSetInt(SERVO_PDLOSTBG,ORANGE_RED);
        }
        else if(wLost>=0 && wLost<=10)
        {
            VarAdrSetInt(SERVO_PDLOSTBG,LIGHT_GREEN);
        }
        else
        {
            VarAdrSetInt(SERVO_PDLOSTBG,0x000000);
        }
        //panel and main
        sendCnt    = g_dbsvosx.wToControl;
        receiveCnt = g_dbsvosx.wFromControl;
        if(g_dbsvosx.wToControl == 0)
            wLost = 0;
        else
        {
            wLost = ((sendCnt-receiveCnt)*100)/sendCnt;
        }
        if(wLost>=0)
        {
            WordToStr(cbuf,wLost,0);
        }
        else
        {
            snprintf(cbuf,sizeof(cbuf)-1,"%d",wLost);
        }
        VarAdrSetStr(TMP_SERVO_MAINLOST,cbuf);
        if(wLost>10)
        {
            VarAdrSetInt(SERVO_PMLOSTBG,ORANGE_RED);
        }
        else if(wLost>=0 && wLost<=10)
        {
            VarAdrSetInt(SERVO_PMLOSTBG,LIGHT_GREEN);
        }
        else
        {
            VarAdrSetInt(SERVO_PMLOSTBG,0x000000);
        }
        //main and drive
        sendCnt    = VarAdrToUI16(SERVO_MACH_WRITECNT);
        receiveCnt = VarAdrToUI16(SERVO_MACH_READCNT);
        if(VarAdrToUI16(SERVO_MACH_WRITECNT) == 0)
            wLost = 0;
        else
        {
            wLost = ((sendCnt-receiveCnt)*100)/sendCnt;
        }
        if(wLost>=0)
        {
            WordToStr(cbuf,wLost,0);
        }
        else
        {
            snprintf(cbuf,sizeof(cbuf)-1,"%d",wLost);
        }
        VarAdrSetStr(TMP_SERVO_DRIVELOST,cbuf);
        if(wLost>10)
        {
            VarAdrSetInt(SERVO_MDLOSTBG,ORANGE_RED);
        }
        else if(wLost>=0 && wLost<=10)
        {
            VarAdrSetInt(SERVO_MDLOSTBG,LIGHT_GREEN);
        }
        else
        {
            VarAdrSetInt(SERVO_MDLOSTBG,0x000000);
        }
    }

    if(btest)
    {
        if((GetTick()-time_100ms)>100)
        {
            time_100ms = GetTick();
            Send_TaskCan(CAN_TX_MAINTEST,0,0,nServoId);
            ++g_dbsvosx.wToControl;

//            Send_TaskCan(CAN_TX_READ,nServoId,0x8001,0);
//            g_dbsvosx.wToDriver++;
        }
    }
}

static void ServoStatexUpdate()
{
    UI8 i;
    char cbuf[10];
    static UI32 time_300ms =0;
    //const char *ptrans  = NULL;
    //const char *ptransx = NULL;

    if((GetTick()-time_300ms)>300)
    {
        time_300ms = GetTick();
         // run state
        for(i=0;i<8;i++)
        {
            //if(pmap_runstatetbl !=NULL)
            //{
            //    ptrans = GetMapTran(pmap_runstatetbl,VarAdrToInt(d_Pump1_STATE_SERVO_STATE+i*0x00100000));
            //    VarAdrSetStr(p_PP_TMP_TMPSTR30+i,ptrans);
            //}
            // comm state Online or Offline
            //if(pmap_tblx1 !=NULL)
            //{
            //    ptransx = GetMapTran(pmap_tblx1,VarAdrToInt(d_Pump1_STATE_SERVO_COMM_ST+i*0x00100000));//
            //    VarAdrSetStr((p_PP_TMP_TMPSTR50+i),ptransx);
            //}

            //err
            if(VarAdrToInt(d_Pump1_STATE_SERVO_ERR+i*0x00100000) == 0)
            {
                cbuf[0] = '0';
                cbuf[1] = '\0';
            }
            else
            {
                snprintf(cbuf,sizeof(cbuf)-1,"Er.%X",(VarAdrToInt(d_Pump1_STATE_SERVO_ERR+i*0x00100000) & 0x0FFF));//
            }
            VarAdrSetStr(p_PP_TMP_TMPSTR90+i,cbuf);
        }
    }
}


#define     CMD_NAME(NUM,pbuff,size)		joinStrAndNum("stCmd",NUM,pbuff,size)
#define     SPEED_NAME(NUM,pbuff,size)		joinStrAndNum("stSpeed",NUM,pbuff,size)
#define     OUTPUT_NAME(NUM,pbuff,size)		joinStrAndNum("stOutPutU",NUM,pbuff,size)
#define     REALPRS_NAME(NUM,pbuff,size)		joinStrAndNum("dtrealprs",NUM,pbuff,size)
#define     DRIVETMP_NAME(NUM,pbuff,size)		joinStrAndNum("dtdrivertmp",NUM,pbuff,size)
#define     MOTORTMP_NAME(NUM,pbuff,size)		joinStrAndNum("dtmotortmp",NUM,pbuff,size)
#define     RUNSTATE_NAME(NUM,pbuff,size)		joinStrAndNum("strunstate",NUM,pbuff,size)
#define     SERVECAN_NAME(NUM,pbuff,size)		joinStrAndNum("dtServeCan",NUM,pbuff,size)
#define     ALARMCODE_NAME(NUM,pbuff,size)		joinStrAndNum("dtalarmcode",NUM,pbuff,size)
static void CmdAndSpeedState(ServoStatex1_PG* ppg, UI16 tabpage_id)
{
    int i;
    PWGT pwgt;
    char buff[15];
    char text[10];
    UI16 speed_val, cmd_val;
    for(i = 0; i < 4; ++i)
    {
        pwgt = PageFramCurWgtByName(CMD_NAME(i+1,buff,15));
        cmd_val = VarAdrToUI16(d_Pump1_STATE_MOTOR_SPEED_SET + (i+tabpage_id*4)*0x00100000);
        if(cmd_val > 32768)
        {
            sprintf(text, "%d", cmd_val - 65536);
            EditSetText(pwgt, text);
        }
        else
        {
            sprintf(text, "%d", cmd_val);
            EditSetText(pwgt, text);
        }
        pwgt = PageFramCurWgtByName(SPEED_NAME(i+1,buff,15));
        speed_val = VarAdrToUI16(d_Pump1_STATE_MOTOR_SPEED_REAL + (i+tabpage_id*4)*0x00100000);
        if(speed_val > 32768)
        {
            sprintf(text, "%d", speed_val - 65536);
            EditSetText(pwgt, text);
        }
        else
        {
            sprintf(text, "%d", speed_val);
            EditSetText(pwgt, text);
        }
    }
}
static void PageChangeShow(ServoStatex1_PG* ppg, UI16 tabpage_id)
{
    int i;
    PWGT pwgt;
    char buff[15];
    ImageSetIndex(ppg->image1, tabpage_id + 1);
    for(i = 0; i < 4; ++i)
    {
        pwgt = PageFramCurWgtByName(OUTPUT_NAME(i+1,buff,15));
        WGTSetAttrByName(pwgt, "text", d_Pump1_STATE_MOTOR_CURRENT + (i+tabpage_id*4)*0x00100000);
        pwgt = PageFramCurWgtByName(REALPRS_NAME(i+1,buff,15));
        WGTSetAttrByName(pwgt, "text", d_Pump1_STATE_SERVO_PRESS + (i+tabpage_id*4)*0x00100000);
        pwgt = PageFramCurWgtByName(DRIVETMP_NAME(i+1,buff,15));
        WGTSetAttrByName(pwgt, "text", d_Pump1_STATE_SERVO_TEMP + (i+tabpage_id*4)*0x00100000);
        pwgt = PageFramCurWgtByName(MOTORTMP_NAME(i+1,buff,15));
        WGTSetAttrByName(pwgt, "text", d_Pump1_STATE_MOTOR_TEMP + (i+tabpage_id*4)*0x00100000);
        pwgt = PageFramCurWgtByName(RUNSTATE_NAME(i+1,buff,15));
        WGTSetAttrByName(pwgt, "text", d_Pump1_STATE_SERVO_STATE + (i+tabpage_id*4)*0x00100000);
        //WGTSetAttrByName(pwgt, "text", p_PP_TMP_TMPSTR30 + (i+tabpage_id*4));
        pwgt = PageFramCurWgtByName(SERVECAN_NAME(i+1,buff,15));
        WGTSetAttrByName(pwgt, "text", d_Pump1_STATE_SERVO_COMM_ST + (i+tabpage_id*4)*0x00100000);
        //WGTSetAttrByName(pwgt, "text", p_PP_TMP_TMPSTR50 + (i+tabpage_id*4));
        pwgt = PageFramCurWgtByName(ALARMCODE_NAME(i+1,buff,15));
        //WGTSetAttrByName(pwgt, "text", d_Pump1_STATE_SERVO_ERR + (i+tabpage_id*4)*0x00100000);
        WGTSetAttrByName(pwgt, "text", p_PP_TMP_TMPSTR90 + (i+tabpage_id*4));
    }
}

static void ServoNumShow(ServoStatex1_PG* ppg, UI16 tabpage_id)
{
    UI16 show_item = 0;
    UI16 bit = 0;
    if(VarAdrToInt(p_PP_SERVO_SERVO_CURRENT_NUM) < 5)
    {
        show_item = VarAdrToInt(p_PP_SERVO_SERVO_CURRENT_NUM)%4; //这里是需要修改
    }
    else if(VarAdrToInt(p_PP_SERVO_SERVO_CURRENT_NUM) < 9)
    {
        if(tabpage_id == 0)
            show_item = 0;
        else
            show_item = (VarAdrToInt(p_PP_SERVO_SERVO_CURRENT_NUM)-4)%4; //这里是需要修改
    }
    else
    {
        if(tabpage_id != 2)
            show_item = 0;
        else
            show_item = (VarAdrToInt(p_PP_SERVO_SERVO_CURRENT_NUM)-8)%4; //这里是需要修改
    }
    switch (show_item) {
    case 0:
        setone(bit,0);
        setone(bit,1);
        setone(bit,2);
        break;
    case 1:
        setzero(bit,0);
        setzero(bit,1);
        setzero(bit,2);
        break;
    case 2:
        setone(bit,0);
        setzero(bit,1);
        setzero(bit,2);
        break;
    case 3:
        setone(bit,0);
        setone(bit,1);
        setzero(bit,2);
        break;
    default:
        break;
    }
    VarAdrSetInt(SHOW_ITEM, bit);

    if(tabpage_id == 0)
    {
        LabelSetText(ppg->lbservo1,"1");
        LabelSetText(ppg->lbservo2,"2");
        LabelSetText(ppg->lbservo3,"3");
        LabelSetText(ppg->lbservo4,"4");
    }
    else if(tabpage_id == 1)
    {
        LabelSetText(ppg->lbservo1,"5");
        LabelSetText(ppg->lbservo2,"6");
        LabelSetText(ppg->lbservo3,"7");
        LabelSetText(ppg->lbservo4,"8");
    }
    else{
        LabelSetText(ppg->lbservo1,"9");
        LabelSetText(ppg->lbservo2,"10");
        LabelSetText(ppg->lbservo3,"11");
        LabelSetText(ppg->lbservo4,"12");
    }
}

static void PageShow(ServoStatex1_PG* ppg)
{
    UI16 show_item = 0;
    UI16 bit = 0;
    show_item = VarAdrToInt(p_PP_SERVO_SERVO_CURRENT_NUM)%4; //这里是需要修改
    switch (show_item) {
    case 0:
        setone(bit,0);
        setone(bit,1);
        setone(bit,2);
        break;
    case 1:
        setzero(bit,0);
        setzero(bit,1);
        setzero(bit,2);
        break;
    case 2:
        setone(bit,0);
        setzero(bit,1);
        setzero(bit,2);
        break;
    case 3:
        setone(bit,0);
        setone(bit,1);
        setzero(bit,2);
        break;
    default:
        break;
    }
    VarAdrSetInt(SHOW_ITEM, bit);
    if(VarAdrToInt(p_PP_SERVO_SERVO_CURRENT_NUM) < 5)
    {
        TabPageSetSubVisble(ppg->tabpage1, 1, FALSE);
        TabPageSetSubVisble(ppg->tabpage1, 2, FALSE);
    }
    else if(VarAdrToInt(p_PP_SERVO_SERVO_CURRENT_NUM) < 9)
    {
        TabPageSetSubVisble(ppg->tabpage1, 1, TRUE);
        TabPageSetSubVisble(ppg->tabpage1, 2, FALSE);
    }
    else{
        TabPageSetSubVisble(ppg->tabpage1, 1, TRUE);
        TabPageSetSubVisble(ppg->tabpage1, 2, TRUE);
    }

    if(VarAdrToInt(p_PP_SERVO_SERVO_CURRENT_NUM) > 4)
    {
        WGTSetVisible(ppg->stTips, TRUE);
    }
    else
    {
        WGTSetVisible(ppg->stTips, FALSE);
    }
}
static void CtlConnect()
{
}
static int ServoStatex1OnInit(ServoStatex1_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetServoStatex1CB(KeyNextPageHandle);
    return 0;
}
static int ServoStatex1OnShow(ServoStatex1_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{  
//    if(!nextpage_press)//开机后只执行一次
//    {
//        G10ServoDebug1GuideSwitch();
//    }

    old_page = 0;
    ServoTestPageIn(ppg);
    FkeyTab(ppg->tabpage1, GUI_KEY_FUN1);
    PageShow(ppg);
    TabPageSelPage(ppg->tabpage1,0);
    PageChangeShow(ppg, 0);
    ServoNumShow(ppg, 0);
    G10ServoDebug1GuideSwitch();//20211117 dyl 伺服状态画面等级放低到LV2
    return 0;
}
static int ServoStatex1OnHide(ServoStatex1_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    ServoTestPageOut();
    return 0;
}
static int ServoStatex1OnUpdate(ServoStatex1_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 cur_page = TabPageGetCurSel(ppg->tabpage1);
    if(old_page != cur_page)
    {
        PageChangeShow(ppg, cur_page);
        ServoNumShow(ppg, cur_page);
        old_page = cur_page;
    }
    CmdAndSpeedState(ppg, cur_page);
    ServoTestUpdate();
    ServoStatexUpdate();

    return 0;
}
static int ServoStatex1OnChange(ServoStatex1_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 value;
    UI32 addr;
    PATTR pattr;

    pattr = (PATTR)msg_para;

    addr = OBJAttrGetAdr(pattr);

    if(addr == TMP_SERVO_SERVOID)
    {
        value = VarAdrToUI16(TMP_SERVO_SERVOID);
        nServoId = value +1;
    }

    if(pwgt ==ppg->button42)
    {
        UI16 pdata[3];

        btest=!btest;

        if(btest)
        {
            g_dbsvosx.wToDriver=0;
            g_dbsvosx.wFromDriver=0;
            g_dbsvosx.wToControl=0;
            g_dbsvosx.wFromControl=0;
            ButtonSetText(pwgt,GetTextTran(TEXT_TESTEND));
            //WGTSetEnable(ppg->dropdown1,FALSE);
        }
        else
        {
            ButtonSetText(pwgt,GetTextTran(TEXT_TESTBEGIN));
            //WGTSetEnable(ppg->dropdown1,TRUE);

        }
        if(btest)
            pdata[0]=1;
        else
            pdata[0]=0;
        pdata[1]=nServoId;
        pdata[2]=0;
        ProtSysCtrl(CAN_TEST_CMD,3,pdata);
    }
    return 0;
}
