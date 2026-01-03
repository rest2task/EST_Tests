#include "WMain_Adjustx.h"
#include "typedef.h"
#include "protcmd.h"
#include "command.h"
#include "canmod.h"
#include "servo.h"
#include "texttbl.h"
#include "language.h"
#include "database.h"
#include "pushtab.h"
#include "buttonwgt.h"
#include "privdef.h"
#include "ctrlcmd.h"
#include "information.h"
#include "labelwgt.h"
#include "tabpagewgt.h"
#include "keymod.h"
#include "key.h"
#include "usermod.h"//20210914 dyl touch

DEFINE_MAIN_ADJUSTX_MAP

CONNECT_MAIN_ADJUSTX_EVENT

#define TMP_TEST_DASELECT	 TMPVAL(100)  //DA 组数选择
#define TMP_TEST_ROOMTEMPA	 TMPVAL(50)   //料筒A室温显示
#define TMP_TEST_ROOMTEMPB	 TMPVAL(51)   //料筒B室温显示
#define TMP_TEST_ROOMTEMPBD	 TMPVAL(52)   //板温室温显示
#define TMP_TEST_CANTX       TMPVAL(60)   //CAN 发送
#define TMP_TEST_CANRX	     TMPVAL(70)   //CAN 接收

#define TMP_INJECTPOS	TMPVAL(0)  //射出电子尺调整位置 临时变量0
#define TMP_CLAMPPOS	TMPVAL(1)  //开关模电子尺调整位置 临时变量1
#define TMP_EJECTPOS	TMPVAL(2)  //托模电子尺调整位置 临时变量2
#define TMP_NOZZDEPOS	TMPVAL(3)  //座台电子尺调整位置 临时变量3
#define TMP_AD5POS   	TMPVAL(4)  //AD5电子尺调整位置 临时变量4
#define TMP_AD6POS	    TMPVAL(5)  //AD6电子尺调整位置 临时变量5
#define TMP_AD7POS	    TMPVAL(6)  //AD7电子尺调整位置 临时变量6
#define TMP_AD8POS	    TMPVAL(7)  //AD8电子尺调整位置 临时变量7

#define DA_STATE_WDA_STAT    0x08160032   //DA测试状态  0-不使用;1-最小值 2-最大值
#define DA_STATE_WDA_CHANNEL 0x08160033   //DA通道值    0-压力1;  1-流量1;	2-压力2;  3-流量2
#define DA_STATE_WDA_SETDATA 0x08160034   //DA设置值

//
#define DA_MACH_WDA_ZERO1    0x08120010  //输出归零值(最小值)
#define DA_MACH_WDA_ZERO2    0x08120011  //输出归零值(最小值)
#define DA_MACH_WDA_ZERO3    0x08120012  //输出归零值(最小值)
#define DA_MACH_WDA_ZERO4    0x08120013  //输出归零值(最小值)
#define DA_MACH_WDA_ZERO5    0x08120014  //输出归零值(最小值)
#define DA_MACH_WDA_ZERO6    0x08120015  //输出归零值(最小值)
#define DA_MACH_WDA_ZERO7    0x08120016  //输出归零值(最小值)
#define DA_MACH_WDA_ZERO8    0x08120017  //输出归零值(最小值)

#define DA_MACH_WDA_MAX1     0x08120000  //输出最大值
#define DA_MACH_WDA_MAX2     0x08120001  //输出最大值
#define DA_MACH_WDA_MAX3     0x08120002  //输出最大值
#define DA_MACH_WDA_MAX4     0x08120003  //输出最大值
#define DA_MACH_WDA_MAX5     0x08120004  //输出最大值
#define DA_MACH_WDA_MAX6     0x08120005  //输出最大值
#define DA_MACH_WDA_MAX7     0x08120006  //输出最大值
#define DA_MACH_WDA_MAX8     0x08120007  //输出最大值

#define STAD_NAME(NUM,pbuff,size)		joinStrAndNum("stAD",NUM,pbuff,size)

static BOOL btest=FALSE;

static void NameChangeAndShow()  //20200325
{
    PWGT pwgt;
    char buff[10];
    UI16 wNozzleValue = VarAdrToInt(d_nozzle1_MACHSET_NZL_METERCH);

//    LabelSetText(main_adjustx_pg.stAD4,GetTextTran(TEXT_AD_ADHYDR1));
//    LabelSetText(main_adjustx_pg.stAD5,"AD5");
//    LabelSetText(main_adjustx_pg.stAD6,"AD6");
//    LabelSetText(main_adjustx_pg.stAD7,"AD7");

//    if (VarAdrToInt(SYS_FL_METER) & 0x0004)
//    {
//        if (IV5200 || IV5300)
//        {
//            if((wNozzleValue>=4)&&(wNozzleValue<=7))  //4-7路做座台切换
//            {
//                pwgt = PageFramCurWgtByName(STAD_NAME(wNozzleValue,buff,10));
//                LabelSetText(pwgt,GetTextTran(TEXT_AD_NOZZLE));
//            }
//        }
//        else
//        {
//            if(wNozzleValue == 4)  //4做座台切换
//            {
//                LabelSetText(main_adjustx_pg.stAD4,GetTextTran(TEXT_AD_NOZZLE));
//            }
//        }
//    }

    //20211210 dyl 根据液压通道号、座台通道号设置AD名称
    LabelSetText(main_adjustx_pg.stAD4,"AD4");
    LabelSetText(main_adjustx_pg.stAD5,"AD5");
    LabelSetText(main_adjustx_pg.stAD6,"AD6");
    LabelSetText(main_adjustx_pg.stAD7,"AD7");
    LabelSetText(main_adjustx_pg.stAD8,"AD8");
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
        }
        if ((wChanel_Sensor2>=4)&&(wChanel_Sensor2 <= g_mainresource_current.nADNum)
                &&(wChanel_Sensor2 != wChanel_Sensor1))
        {
            pwgt = PageFramCurWgtByName(STAD_NAME(wChanel_Sensor2,buff,10));
            LabelSetText(pwgt,GetTextTran(TEXT_AD_SENSOR2));
        }
    }
}

/************************************************************************/
/* 主机调试页面进入                                                     */
/************************************************************************/
static void MainAdjPageIn(Main_Adjustx_PG* ppg)
{
    UI8 i;

    //ad meter adjust pos
    for(i=0;i<8;i++)
    {
        VarAdrSetInt(TMP_INJECTPOS+i,0);
    }

    VarAdrSetInt(DA_STATE_WDA_STAT,0);
    VarSendByAdr(DA_STATE_WDA_STAT);
    VarAdrSetInt(DA_STATE_WDA_CHANNEL,0);
    VarSendByAdr(DA_STATE_WDA_CHANNEL);

    g_dbsvosx.wToControl=0;
    g_dbsvosx.wFromControl=0;

    VarAdrSetInt(TMP_TEST_DASELECT,0);//默认第一组

    //data request
    ProtReqPartSubData(PART1_ID(DA_ID),MACHSET,0,GetSubLenByID(PART1_SUB_ID(DA_ID,MACHSET)));//DA部件
    ProtSysCtrl(ENTER_ADJ_CMD,0,NULL);//进入调机状态

    btest=FALSE;

    ButtonSetText(ppg->canTest,GetTextTran(TEXT_TESTBEGIN));

    if(VarAdrToInt(p_PP_STATE_TEMP_CTRL_PART) > 1)//20200409
    {
        TabPageSetSubVisble(main_adjustx_pg.tabpage1, 1, TRUE);
    }
    else
    {
        TabPageSetSubVisble(main_adjustx_pg.tabpage1, 1, FALSE);
    }

    NameChangeAndShow();//20200325
    FkeyTab(ppg->tabpage1, GUI_KEY_FUN1);

    ShowMsg(VW_MSG_ADJUSTMODE);
}
/************************************************************************/
/* 主机调试页面退出                                                     */
/************************************************************************/
static void MainAdjPageOut()
{
    UI16 pdata[2]={0,0};

    VarAdrSetInt(DA_STATE_WDA_STAT,0);
    VarSendByAdr(DA_STATE_WDA_STAT);
    VarAdrSetInt(d_TempCtrlComm1_STATE_WTEMP_ADJAMB,0);
    VarSendByAdr(d_TempCtrlComm1_STATE_WTEMP_ADJAMB);
    VarAdrSetInt(d_TempCtrlComm2_STATE_WTEMP_ADJAMB,0);
    VarSendByAdr(d_TempCtrlComm2_STATE_WTEMP_ADJAMB);
    if(btest)
    {
        ProtSysCtrl(CAN_TEST_CMD,2,pdata);
    }
    ProtSysCtrl(EXIT_ADJ_CMD,0,NULL);//退出调机状态

    VarAdrSetInt(d_tempctrl1_STATE_ADJ_EXT_TEMP,0);
    VarSendByAdr(d_tempctrl1_STATE_ADJ_EXT_TEMP);
}
/************************************************************************/
/* 主机调试页面刷新                                                     */
/************************************************************************/
static void MainAdjUpdate()
{
    UI16 wValue;
    static UI32 time_100ms = 0;
    //料管A
    wValue = VarAdrToUI16(d_TempCtrlComm1_STATE_WTEMP_LM73);
    if(wValue>0x8000)
    {
        VarAdrSetInt(TMP_TEST_ROOMTEMPA,wValue-0x8000);
        LabelSetText(main_adjustx_pg.tabpage1_tab1_sign,"-");
    }
    else
    {
        VarAdrSetInt(TMP_TEST_ROOMTEMPA,wValue);
        LabelSetText(main_adjustx_pg.tabpage1_tab1_sign,"+");
    }
    //料管B
    wValue = VarAdrToUI16(d_TempCtrlComm2_STATE_WTEMP_LM73);
    if(wValue>0x8000)
    {
        VarAdrSetInt(TMP_TEST_ROOMTEMPB,wValue-0x8000);
        LabelSetText(main_adjustx_pg.tabpage1_tab2_sign,"-");
    }
    else
    {
        VarAdrSetInt(TMP_TEST_ROOMTEMPB,wValue);
        LabelSetText(main_adjustx_pg.tabpage1_tab2_sign,"+");
    }
    //板温
    wValue = VarAdrToUI16(d_tempctrl1_STATE_DATEMP_REAL_1);
    if(wValue>0x8000)
    {
        VarAdrSetInt(TMP_TEST_ROOMTEMPBD,wValue-0x8000);
        LabelSetText(main_adjustx_pg.tabpage1_tab3_sign,"-");
    }
    else
    {
        VarAdrSetInt(TMP_TEST_ROOMTEMPBD,wValue);
        LabelSetText(main_adjustx_pg.tabpage1_tab3_sign,"+");
    }

    VarAdrSetInt(TMP_TEST_CANTX, g_dbsvosx.wToControl);
    VarAdrSetInt(TMP_TEST_CANRX,g_dbsvosx.wFromControl);

    if(btest)
    {
        if((GetTick()-time_100ms)>100)
        {
            time_100ms = GetTick();
            Send_TaskCan(CAN_TX_MAINTEST,0,0,1);
            ++g_dbsvosx.wToControl;
        }
    }

    LabelSetText(main_adjustx_pg.stAdd1,(VarAdrToUI16(ADMETER_ABSOLUTE1) >= VarAdrToUI16(ADMETER_ZSET1)) ? "+":"-");
    LabelSetText(main_adjustx_pg.stAdd2,(VarAdrToUI16(ADMETER_ABSOLUTE2) >= VarAdrToUI16(ADMETER_ZSET2)) ? "+":"-");
    LabelSetText(main_adjustx_pg.stAdd3,(VarAdrToUI16(ADMETER_ABSOLUTE3) >= VarAdrToUI16(ADMETER_ZSET3)) ? "+":"-");
    LabelSetText(main_adjustx_pg.stAdd4,(VarAdrToUI16(ADMETER_ABSOLUTE4) >= VarAdrToUI16(ADMETER_ZSET4)) ? "+":"-");
    LabelSetText(main_adjustx_pg.stAdd5,(VarAdrToUI16(ADMETER_ABSOLUTE5) >= VarAdrToUI16(ADMETER_ZSET5)) ? "+":"-");
    LabelSetText(main_adjustx_pg.stAdd6,(VarAdrToUI16(ADMETER_ABSOLUTE6) >= VarAdrToUI16(ADMETER_ZSET6)) ? "+":"-");
    LabelSetText(main_adjustx_pg.stAdd7,(VarAdrToUI16(ADMETER_ABSOLUTE7) >= VarAdrToUI16(ADMETER_ZSET7)) ? "+":"-");
    LabelSetText(main_adjustx_pg.stAdd8,(VarAdrToUI16(ADMETER_ABSOLUTE8) >= VarAdrToUI16(ADMETER_ZSET8)) ? "+":"-");
}
/************************************************************************/
/* DA测试选择（DA选择，DA测试，通道）                                   */
/************************************************************************/
static void DaSelect()
{
    UI16 wValue,wValue1,wValue2;

    wValue2 = VarAdrToUI16(TMP_TEST_DASELECT);
    wValue = VarAdrToUI16(DA_STATE_WDA_STAT);
    wValue1 = VarAdrToUI16(DA_STATE_WDA_CHANNEL);
    if(wValue2==0){
        if((wValue==1) && (wValue1==0))      VarAdrSetInt(DA_STATE_WDA_SETDATA,VarAdrToUI16(DA_MACH_WDA_ZERO1)); //4A7 第一组最小压力
        else if((wValue==1) && (wValue1==1)) VarAdrSetInt(DA_STATE_WDA_SETDATA,VarAdrToUI16(DA_MACH_WDA_ZERO2)); //4A9第一组最小流量
        else if((wValue==2) && (wValue1==0)) VarAdrSetInt(DA_STATE_WDA_SETDATA,VarAdrToUI16(DA_MACH_WDA_MAX1)); //4AB第一组最大压力
        else if((wValue==2) && (wValue1==1)) VarAdrSetInt(DA_STATE_WDA_SETDATA,VarAdrToUI16(DA_MACH_WDA_MAX2)); //4AD第一组最大流量
        else   VarAdrSetInt(DA_STATE_WDA_SETDATA,0);
    }

    else if(wValue2==1){
        if((wValue==1) && (wValue1==0))      VarAdrSetInt(DA_STATE_WDA_SETDATA,VarAdrToUI16(DA_MACH_WDA_ZERO3)); //4A7 第二组最小压力
        else if((wValue==1) && (wValue1==1)) VarAdrSetInt(DA_STATE_WDA_SETDATA,VarAdrToUI16(DA_MACH_WDA_ZERO4)); //4A9第二组最小流量
        else if((wValue==2) && (wValue1==0)) VarAdrSetInt(DA_STATE_WDA_SETDATA,VarAdrToUI16(DA_MACH_WDA_MAX3)); //4AB第二组最大压力
        else if((wValue==2) && (wValue1==1)) VarAdrSetInt(DA_STATE_WDA_SETDATA,VarAdrToUI16(DA_MACH_WDA_MAX4)); //4AD第二组最大流量
        else VarAdrSetInt(DA_STATE_WDA_SETDATA,0);
    }
    else if(wValue2==2){
        if((wValue==1) && (wValue1==0))      VarAdrSetInt(DA_STATE_WDA_SETDATA,VarAdrToUI16(DA_MACH_WDA_ZERO5)); //4A7 第三组最小压力
        else if((wValue==1) && (wValue1==1)) VarAdrSetInt(DA_STATE_WDA_SETDATA,VarAdrToUI16(DA_MACH_WDA_ZERO6)); //4A9第三组最小流量
        else if((wValue==2) && (wValue1==0)) VarAdrSetInt(DA_STATE_WDA_SETDATA,VarAdrToUI16(DA_MACH_WDA_MAX5)); //4AB第三组最大压力
        else if((wValue==2) && (wValue1==1)) VarAdrSetInt(DA_STATE_WDA_SETDATA,VarAdrToUI16(DA_MACH_WDA_MAX6)); //4AD第三组最大流量
        else VarAdrSetInt(DA_STATE_WDA_SETDATA,0);
    }
    else if(wValue2==3){
        if((wValue==1) && (wValue1==0))      VarAdrSetInt(DA_STATE_WDA_SETDATA,VarAdrToUI16(DA_MACH_WDA_ZERO7)); //4A7 第四组最小压力
        else if((wValue==1) && (wValue1==1)) VarAdrSetInt(DA_STATE_WDA_SETDATA,VarAdrToUI16(DA_MACH_WDA_ZERO8)); //4A9第四组最小流量
        else if((wValue==2) && (wValue1==0)) VarAdrSetInt(DA_STATE_WDA_SETDATA,VarAdrToUI16(DA_MACH_WDA_MAX7)); //4AB第四组最大压力
        else if((wValue==2) && (wValue1==1)) VarAdrSetInt(DA_STATE_WDA_SETDATA,VarAdrToUI16(DA_MACH_WDA_MAX8)); //4AD第四组最大流量
        else VarAdrSetInt(DA_STATE_WDA_SETDATA,0);
    }
}
/************************************************************************/
/* DA测试发送（确定）                                                   */
/************************************************************************/
static void DaTestSend()
{
    UI16 wValue,wValue1,wValue2,wValue3;
    UI16 pdata[3];

    wValue1 = VarAdrToUI16(DA_STATE_WDA_CHANNEL); // 通道
    wValue = VarAdrToUI16(DA_STATE_WDA_STAT);     // DA测试
    wValue2= wValue1+(VarAdrToUI16(TMP_TEST_DASELECT)<<1); // DA选择
    wValue3= VarAdrToUI16(DA_STATE_WDA_SETDATA); //DA值
    pdata[0] = wValue;
    pdata[1] = wValue2;
    pdata[2] = wValue3;
    ProtSysCtrl(DA_ADJ_CMD,3,pdata);
    ProtReqPartSubData(PART1_ID(DA_ID),MACHSET,0,GetSubLenByID(PART1_SUB_ID(DA_ID,MACHSET)));//DA部件

    if(wValue==1){
        if(wValue2==0){
            VarAdrSetInt(DA_STATE_TEMPMIN1,wValue3);
            VarSendByAdr(DA_STATE_TEMPMIN1);
        }
        else if(wValue2==1){
            VarAdrSetInt(DA_STATE_TEMPMIN1+1,wValue3);
            VarSendByAdr(DA_STATE_TEMPMIN1+1);
        }
        else if(wValue2==2){
            VarAdrSetInt(DA_STATE_TEMPMIN1+2,wValue3);
            VarSendByAdr(DA_STATE_TEMPMIN1+2);
        }
        else if(wValue2==3){
            VarAdrSetInt(DA_STATE_TEMPMIN1+3,wValue3);
            VarSendByAdr(DA_STATE_TEMPMIN1+3);
        }
        else if(wValue2==4){
            VarAdrSetInt(DA_STATE_TEMPMIN1+4,wValue3);
            VarSendByAdr(DA_STATE_TEMPMIN1+4);
        }
        else if(wValue2==5){
            VarAdrSetInt(DA_STATE_TEMPMIN1+5,wValue3);
            VarSendByAdr(DA_STATE_TEMPMIN1+5);
        }
        else if(wValue2==6){
            VarAdrSetInt(DA_STATE_TEMPMIN1+6,wValue3);
            VarSendByAdr(DA_STATE_TEMPMIN1+6);
        }
        else if(wValue2==7){
            VarAdrSetInt(DA_STATE_TEMPMIN1+7,wValue3);
            VarSendByAdr(DA_STATE_TEMPMIN1+7);
        }
    }
    else if(wValue==2){
        if(wValue2==0){
            VarAdrSetInt(DA_STATE_TEMPMAX1,wValue3);
            VarSendByAdr(DA_STATE_TEMPMAX1);
        }
        else if(wValue2==1){
            VarAdrSetInt(DA_STATE_TEMPMAX1+1,wValue3);
            VarSendByAdr(DA_STATE_TEMPMAX1+1);
        }
        else if(wValue2==2){
            VarAdrSetInt(DA_STATE_TEMPMAX1+2,wValue3);
            VarSendByAdr(DA_STATE_TEMPMAX1+2);
        }
        else if(wValue2==3){
            VarAdrSetInt(DA_STATE_TEMPMAX1+3,wValue3);
            VarSendByAdr(DA_STATE_TEMPMAX1+3);
        }
        else if(wValue2==4){
            VarAdrSetInt(DA_STATE_TEMPMAX1+4,wValue3);
            VarSendByAdr(DA_STATE_TEMPMAX1+4);
        }
        else if(wValue2==5){
            VarAdrSetInt(DA_STATE_TEMPMAX1+5,wValue3);
            VarSendByAdr(DA_STATE_TEMPMAX1+5);
        }
        else if(wValue2==6){
            VarAdrSetInt(DA_STATE_TEMPMAX1+6,wValue3);
            VarSendByAdr(DA_STATE_TEMPMAX1+6);
        }
        else if(wValue2==7){
            VarAdrSetInt(DA_STATE_TEMPMAX1+7,wValue3);
            VarSendByAdr(DA_STATE_TEMPMAX1+7);
        }
    }
}
/************************************************************************/
/* CAN测试（开始或停止）                                                */
/************************************************************************/
static void CanTestCmd(PWGT pwgt)
{
    btest =!btest;

    if(btest)
    {
        g_dbsvosx.wToControl=0;
        g_dbsvosx.wFromControl=0;
        ButtonSetText(pwgt,GetTextTran(TEXT_TESTEND));
    }
    else
    {
        ButtonSetText(pwgt,GetTextTran(TEXT_TESTBEGIN));
    }
}

/************************************************************************/
/* 发送AD归零位置给下位机，然后发归零命令                               */
/************************************************************************/
static BOOL AdMeterZero_MainAdjx(UI32 zero_addr,UI32 abs_addr,UI16 index)
{
    PVAR pdata;
    UI16 data[1];

    VarAdrSetInt(zero_addr,VarAdrToUI16(abs_addr));
    VarSendByAdr(zero_addr);
    if(pdata = VarAdrToVar(zero_addr))
        VarSaveInt(pdata);

    VarAdrSetInt(ADDR_ADPOSI_REAL1+index, 0);
    data[0] = 0;//0-标准 1-扩展
    ProtSysCtrl(METER_ZERO_ADJ_CMD,1,data);

    return TRUE;
}

static int daTestOkOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int injZeroOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int clampZeroOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int ejtZeroOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int ad4ZeroOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int ad5ZeroOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int ad6ZeroOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int ad7ZeroOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int ad8ZeroOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int actualZeroOnClick1(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int tempProofOkOnClick1(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int roomTempOkOnClick1(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int actualZeroOnClick2(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int tempProofOkOnClick2(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int roomTempOkOnClick2(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int roomTempOkOnClick3(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int canTestOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static void CtlConnect()
{
    WgtMsgConnect(main_adjustx_pg.canTest, WGT_CLICK_MSG, canTestOnClick);
    WgtMsgConnect(main_adjustx_pg.tabpage1_tab1_roomTempOk, WGT_CLICK_MSG, roomTempOkOnClick1);
    WgtMsgConnect(main_adjustx_pg.tabpage1_tab1_tempProofOk, WGT_CLICK_MSG, tempProofOkOnClick1);
    WgtMsgConnect(main_adjustx_pg.tabpage1_tab1_actualZero, WGT_CLICK_MSG, actualZeroOnClick1);
    WgtMsgConnect(main_adjustx_pg.tabpage1_tab2_roomTempOk, WGT_CLICK_MSG, roomTempOkOnClick2);
    WgtMsgConnect(main_adjustx_pg.tabpage1_tab2_tempProofOk, WGT_CLICK_MSG, tempProofOkOnClick2);
    WgtMsgConnect(main_adjustx_pg.tabpage1_tab2_actualZero, WGT_CLICK_MSG, actualZeroOnClick2);
    WgtMsgConnect(main_adjustx_pg.tabpage1_tab3_roomTempOk, WGT_CLICK_MSG, roomTempOkOnClick3);
    WgtMsgConnect(main_adjustx_pg.ad8Zero, WGT_CLICK_MSG, ad8ZeroOnClick);
    WgtMsgConnect(main_adjustx_pg.ad7Zero, WGT_CLICK_MSG, ad7ZeroOnClick);
    WgtMsgConnect(main_adjustx_pg.ad6Zero, WGT_CLICK_MSG, ad6ZeroOnClick);
    WgtMsgConnect(main_adjustx_pg.ad5Zero, WGT_CLICK_MSG, ad5ZeroOnClick);
    WgtMsgConnect(main_adjustx_pg.ad4Zero, WGT_CLICK_MSG, ad4ZeroOnClick);
    WgtMsgConnect(main_adjustx_pg.ejtZero, WGT_CLICK_MSG, ejtZeroOnClick);
    WgtMsgConnect(main_adjustx_pg.clampZero, WGT_CLICK_MSG, clampZeroOnClick);
    WgtMsgConnect(main_adjustx_pg.injZero, WGT_CLICK_MSG, injZeroOnClick);
    WgtMsgConnect(main_adjustx_pg.daTestOk, WGT_CLICK_MSG, daTestOkOnClick);
}
static int Main_AdjustxOnInit(Main_Adjustx_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Main_AdjustxOnShow(Main_Adjustx_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_MODELOCK);//20210914 dyl touch

    MainAdjPageIn(ppg);

    return 0;
}
static int Main_AdjustxOnHide(Main_Adjustx_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch

    MainAdjPageOut();

    return 0;
}
static int Main_AdjustxOnUpdate(Main_Adjustx_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageProtect();//20210914 dyl touch //PAGE_PROTECT
    MainAdjUpdate();

    return 0;
}
static int Main_AdjustxOnChange(Main_Adjustx_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 value;
    UI16 index;
    UI16 pdata[3];
    UI32 addr;
    PATTR pattr;

    pattr = (PATTR)msg_para;

    addr = OBJAttrGetAdr(pattr);

    switch(addr)
    {
    case TMP_TEST_DASELECT:
    case DA_STATE_WDA_STAT:
    case DA_STATE_WDA_CHANNEL:
        DaSelect();
        break;
    default:
        if(addr>= TMP_INJECTPOS && addr <=TMP_AD8POS)
        {
            index =(UI16)(addr - TMP_INJECTPOS);
            value = VarAdrToUI16(TMP_INJECTPOS+index);

            if(PushtableType() && addr ==TMP_CLAMPPOS)
            {
                value = UnTransMeter(value,FALSE);
            }
            pdata[0] = index;
            pdata[1] = value;
            pdata[2] = 0; //0-标准 1-扩展
            ProtSysCtrl(METER_LENGTH_ADJ_CMD,3,pdata);
        }
        break;
    }

    return 0;
}
static int daTestOkOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    DaTestSend();

    return 0;
}
static int injZeroOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    AdMeterZero_MainAdjx(ADMETER_ZSET1,ADMETER_ABSOLUTE1,0);

    return 0;
}
static int clampZeroOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    AdMeterZero_MainAdjx(ADMETER_ZSET2,ADMETER_ABSOLUTE2,1);

    return 0;
}
static int ejtZeroOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    AdMeterZero_MainAdjx(ADMETER_ZSET3,ADMETER_ABSOLUTE3,2);

    return 0;
}
static int ad4ZeroOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    AdMeterZero_MainAdjx(ADMETER_ZSET4,ADMETER_ABSOLUTE4,3);

    return 0;
}
static int ad5ZeroOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    AdMeterZero_MainAdjx(ADMETER_ZSET5,ADMETER_ABSOLUTE5,4);

    return 0;
}
static int ad6ZeroOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    AdMeterZero_MainAdjx(ADMETER_ZSET6,ADMETER_ABSOLUTE6,5);

    return 0;
}
static int ad7ZeroOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    AdMeterZero_MainAdjx(ADMETER_ZSET7,ADMETER_ABSOLUTE7,6);

    return 0;
}
static int ad8ZeroOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    AdMeterZero_MainAdjx(ADMETER_ZSET8,ADMETER_ABSOLUTE8,7);

    return 0;
}
static int actualZeroOnClick1(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 data[1];
    data[0] = 1;

    ProtSysCtrl(TEMP_ZERO_CMD,1,data);

    return 0;
}
static int actualZeroOnClick2(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 data[1];
    data[0] = 2;

    ProtSysCtrl(TEMP_ZERO_CMD,1,data);

    return 0;
}
static int tempProofOkOnClick1(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 data[1];
    data[0] = 1;

    ProtSysCtrl(TEMP_ADJ_CMD,1,data);

    return 0;
}
static int tempProofOkOnClick2(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 data[1];
    data[0] = 2;

    ProtSysCtrl(TEMP_ADJ_CMD,1,data);

    return 0;
}
static int roomTempOkOnClick1(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 data[1];
    data[0] = 1;

    ProtSysCtrl(AMB_TEMP_ADJ_CMD,1,data);

    return 0;
}
static int roomTempOkOnClick2(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 data[1];
    data[0] = 2;

    ProtSysCtrl(AMB_TEMP_ADJ_CMD,1,data);

    return 0;
}
static int roomTempOkOnClick3(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 data[1];
    data[0] = 0;

    ProtSysCtrl(AMB_TEMP_ADJ_CMD,1,data);

    return 0;
}
static int canTestOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    CanTestCmd(pwgt);

    return 0;
}
