#include "WCharge_Curve.h"
#include "typedef.h"
#include "pagefram.h"
#include "curvesctl.h"
#include "dropwgt.h"
#include "oprintf.h"
#include "database.h"
#include "privdef.h"
#include "keymod.h"	
#include "key.h"
#include "stdio.h"
#include "servo.h"
#include "editwgt.h"

DEFINE_CHARGE_CURVE_MAP

CONNECT_CHARGE_CURVE_EVENT

#define     CURVE_CHGSYSPRS(curveid,buff,size)          joinStrAndNum("sysprs",curveid+1,buff,size)
#define     CURVE_CHGMOTORSPD(curveid,buff,size)        joinStrAndNum("motorspd",curveid+1,buff,size)
#define     CURVE_CHGSCREWRPM(curveid,buff,size)        joinStrAndNum("screwrpm",curveid+1,buff,size)
#define     CURVE_CHGSETPRS(curveid,buff,size)          joinStrAndNum("setprs",curveid+1,buff,size)

#define     HISTORY_CURVE             TMPVAL(100) //历史曲线选择
#define     CHARGE_CURVE_NUM          2           //当前曲线+历史曲线总条数

#define     A_CHGPOS_VAL              TMPSTR(1)   //A游标指示储料位置值
#define     A_SCREWRPM_VAL            TMPSTR(2)   //A游标指示螺杆转速值
#define     A_MOTORSPD_VAL            TMPSTR(4)   //A游标指示电机转速值
#define     A_SYSPRS_VAL              TMPSTR(5)   //A游标指示系统压力值
#define     B_CHGPOS_VAL              TMPSTR(6)   //B游标指示储料位置值
#define     B_SCREWRPM_VAL            TMPSTR(7)   //B游标指示螺杆转速值
#define     B_MOTORSPD_VAL            TMPSTR(9)   //B游标指示电机转速值
#define     B_SYSPRS_VAL              TMPSTR(10)  //B游标指示系统压力值


static BOOL tracer_status = FALSE;//是否在移动标线状态
//曲线采用自定义发送协议曲线请求
static PROT_DATREC_REQ_PKT gcmd1;
static DATSET_IND gcmd2[5] ;			//协议数据2
static BOOL curve_clear = TRUE;
static BOOL clean_first_curve = FALSE;//交替画曲线，画之前先清空
static UI16 charge_curveid = 0;//曲线编号，用于显示历史曲线
static UI16 current_curveid = 0;//当前曲线id，与charge_curveid差1
static BOOL is_recivedata = FALSE;//20191031.cyx 解决曲线画到一半切出画面下次画接着上次继续画的问题

#define CURVE_NUM 4 //曲线条数
#define TMP_CURVE1_SEL      TMPVAL(10) //曲线1选择
#define TMP_CURVE2_SEL      TMPVAL(11) //曲线2选择
#define TMP_CURVE3_SEL      TMPVAL(12) //曲线3选择
#define TMP_CURVE4_SEL      TMPVAL(13) //曲线4选择
static UI16 cb_sel[CURVE_NUM] = {1,1,1,1};//曲线选择

#define TMP_CURVE_X_MIN     TMPVAL(20) //曲线x轴最小值
#define TMP_CURVE_X_MAX     TMPVAL(21) //曲线x轴最大值
static double curve_x_min = 0.0;
static double curve_x_max = 0.0;

static void SetCurveColor(PWGT pwgt, int value_int)
{
    OBJSetValueByAttrName((POBJ)pwgt,"color",value_int,0,NULL,ATTR_UINT);
}

//曲线游标值显示
static void TraceDataShow(PLIGHTGRAPH_WGT plot ,int tracerid)
{
    double chgpos, sysprs, motorspd, screwrpm;
    PWGT pwgt;
    LDPoint point;
    char buff[20];
    int curve_id;

//    if(VarAdrToInt(HISTORY_CURVE))
//    {
//        curve_id = charge_curveid;
//    }
//    else
    {
        curve_id = current_curveid;
    }

    if(tracerid == 1)//A游标
    {
        pwgt = LightGraphGetCurve((PWGT)plot,CURVE_CHGSYSPRS(curve_id,buff,20));
        point = LCurveTracer(pwgt,tracerid);
        chgpos = point.key;
        sysprs = point.value;
        snprintf(buff,sizeof(buff)-1,"%.1f",chgpos);
        VarAdrSetStr(A_CHGPOS_VAL, buff);
        snprintf(buff,sizeof(buff)-1,"%.1f",sysprs);
        VarAdrSetStr(A_SYSPRS_VAL, buff);

        pwgt = LightGraphGetCurve((PWGT)plot,CURVE_CHGMOTORSPD(curve_id,buff,20));
        point = LCurveTracer(pwgt,tracerid);
        motorspd = point.value;
        snprintf(buff,sizeof(buff)-1,"%.f",motorspd);
        VarAdrSetStr(A_MOTORSPD_VAL, buff);

        pwgt = LightGraphGetCurve((PWGT)plot,CURVE_CHGSCREWRPM(curve_id,buff,20));
        point = LCurveTracer(pwgt,tracerid);
        screwrpm = point.value;
        snprintf(buff,sizeof(buff)-1,"%.1f",screwrpm);
        VarAdrSetStr(A_SCREWRPM_VAL, buff);
    }
    else if(tracerid == 2)//B游标
    {
        pwgt = LightGraphGetCurve((PWGT)plot,CURVE_CHGSYSPRS(curve_id,buff,20));
        point = LCurveTracer(pwgt,tracerid);
        chgpos = point.key;
        sysprs = point.value;
        snprintf(buff,sizeof(buff)-1,"%.1f",chgpos);
        VarAdrSetStr(B_CHGPOS_VAL, buff);
        snprintf(buff,sizeof(buff)-1,"%.1f",sysprs);
        VarAdrSetStr(B_SYSPRS_VAL, buff);

        pwgt = LightGraphGetCurve((PWGT)plot,CURVE_CHGMOTORSPD(curve_id,buff,20));
        point = LCurveTracer(pwgt,tracerid);
        motorspd = point.value;
        snprintf(buff,sizeof(buff)-1,"%.f",motorspd);
        VarAdrSetStr(B_MOTORSPD_VAL, buff);

        pwgt = LightGraphGetCurve((PWGT)plot,CURVE_CHGSCREWRPM(curve_id,buff,20));
        point = LCurveTracer(pwgt,tracerid);
        screwrpm = point.value;
        snprintf(buff,sizeof(buff)-1,"%.1f",screwrpm);
        VarAdrSetStr(B_SCREWRPM_VAL, buff);
    }
}

//清除要画的曲线
static void ClearChargeDrawCurve()
{
    PWGT pwgt;
    char buff[20];

    if(clean_first_curve)
    {
        pwgt = LightGraphGetCurve(charge_curve_pg.lgraph1,CURVE_CHGSYSPRS(charge_curveid,buff,20));
        LCurveClear(pwgt);
        pwgt = LightGraphGetCurve(charge_curve_pg.lgraph1,CURVE_CHGMOTORSPD(charge_curveid,buff,20));
        LCurveClear(pwgt);
        pwgt = LightGraphGetCurve(charge_curve_pg.lgraph1,CURVE_CHGSCREWRPM(charge_curveid,buff,20));
        LCurveClear(pwgt);
        pwgt = LightGraphGetCurve(charge_curve_pg.lgraph1,CURVE_CHGSETPRS(charge_curveid,buff,20));
        LCurveClear(pwgt);
    }

}

//显示历史曲线
static void ShowChargeHistoryCurve()
{
    PWGT pwgt[CURVE_NUM];
    char buff[20];
    int i,j;

    for(i = 0;i < CHARGE_CURVE_NUM;i++)
    {
        pwgt[0] = LightGraphGetCurve(charge_curve_pg.lgraph1,CURVE_CHGSYSPRS(i,buff,20));
        pwgt[1] = LightGraphGetCurve(charge_curve_pg.lgraph1,CURVE_CHGMOTORSPD(i,buff,20));
        pwgt[2] = LightGraphGetCurve(charge_curve_pg.lgraph1,CURVE_CHGSCREWRPM(i,buff,20));
        pwgt[3] = LightGraphGetCurve(charge_curve_pg.lgraph1,CURVE_CHGSETPRS(i,buff,20));
        if(VarAdrToInt(HISTORY_CURVE))
        {
            if(current_curveid == i)
            {
                SetCurveColor(pwgt[0],0x00ffff);
                SetCurveColor(pwgt[1],0x0000ff);
                SetCurveColor(pwgt[2],0x00ff00);
                SetCurveColor(pwgt[3],0xff0000);

                for(j = 0; j < CURVE_NUM; j++)
                {
                    if(cb_sel[j])
                    {
                        LCurveSetVis(pwgt[j],TRUE);
                    }
                    else
                    {
                        LCurveSetVis(pwgt[j],FALSE);
                    }
                }
            }
            else
            {
                SetCurveColor(pwgt[0],0xd2fafa);
                SetCurveColor(pwgt[1],0xc1c1ff);
                SetCurveColor(pwgt[2],0x808080);
                SetCurveColor(pwgt[3],0xeec081);

                for(j = 0; j < CURVE_NUM; j++)
                {
                    if(cb_sel[j])
                    {
                        LCurveSetVis(pwgt[j],TRUE);
                    }
                    else
                    {
                        LCurveSetVis(pwgt[j],FALSE);
                    }
                }
            }
        }
        else
        {
            if(current_curveid != i)
            {
                for(j = 0; j < CURVE_NUM; j++)
                {
                    LCurveSetVis(pwgt[j],FALSE);
                }
            }
            else
            {
                SetCurveColor(pwgt[0],0x00ffff);
                SetCurveColor(pwgt[1],0x0000ff);
                SetCurveColor(pwgt[2],0x00ff00);
                SetCurveColor(pwgt[3],0xff0000);

                for(j = 0; j < CURVE_NUM; j++)
                {
                    if(cb_sel[j])
                    {
                        LCurveSetVis(pwgt[j],TRUE);
                    }
                    else
                    {
                        LCurveSetVis(pwgt[j],FALSE);
                    }
                }
            }
        }
    }
}

//初始化范围
static void InitRange()
{
    PWGT pwgt;
    pwgt = LightGraphGetCurve(charge_curve_pg.lgraph1,"scaleX");
    if(VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_1))
    {
        LAxisSetRange(pwgt,0,VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_1)/10.0);
    }
    else
    {
        LAxisSetRange(pwgt,0,350);
    }
    pwgt = LightGraphGetCurve(charge_curve_pg.lgraph1,"scaleY1");
    LAxisSetRange(pwgt,0,140);//初始0-140
    pwgt = LightGraphGetCurve(charge_curve_pg.lgraph1,"scaleY2");
    LAxisSetRange(pwgt,0,400);//初始0-400
    pwgt = LightGraphGetCurve(charge_curve_pg.lgraph1,"scaleY3");
    LAxisSetRange(pwgt,0,4000);//初始0-4000
    LightGraphSaveRange(charge_curve_pg.lgraph1);
}

//清除曲线
static void ClearLigthGraph()
{
    CurvesClear_New(G_CHARGE);

    LightGraphClear(charge_curve_pg.lgraph1);
    charge_curveid = 0;//曲线编号，用于显示历史曲线
    current_curveid = 0;
    clean_first_curve = FALSE;
    LightGraphSetTouchMode(charge_curve_pg.lgraph1,MODE_FIXED);

    VarAdrSetInt(HISTORY_CURVE,0);
    ShowChargeHistoryCurve();//20230516 chj 修复在旋转历史曲线后，再选择复位，再做曲线，曲线颜色不正确问题，
    curve_x_min = 0.0;
    curve_x_max = 0.0;
    VarAdrSetInt(TMP_CURVE_X_MIN, 0);
    VarAdrSetInt(TMP_CURVE_X_MAX, 0);
    InitRange();

    //20230518 chj 曲线范围修改后，是否固定
    LightGraphSetAutoAdapt(charge_curve_pg.lgraph1,TRUE);

}

static void BackToCharge(int btnid, PNAVI_BTN pbtn) //20191211.cyx 曲线画面返回特殊处理
{
    if(btnid == 6)
    {
        PanelShowPageByName(PAGE_CHARGE); //画面的跳转表先清空，否则不会执行改行
    }
    else
    {
        NaviBtnJumpDefault(btnid, pbtn);
    }
}

//设置游标状态
static void SetTracerState(BOOL status)
{
    tracer_status = status;

    if(status)
    {
        WGTSetEnable(charge_curve_pg.lgraph1, TRUE);
        WGTSetVisible(charge_curve_pg.btmoving, TRUE);
        WGTSetVisible(charge_curve_pg.subpage1, TRUE);
//20220805 dyl 非全键盘不显示游标移动提示信息
#ifdef PANEL_NOKEYBOARD//G15:No_KeyBoard

#else
        WGTSetVisible(charge_curve_pg.stinfo, TRUE);
#endif
        LightGraphSetTouchMode(charge_curve_pg.lgraph1,MODE_TRACER);
        PageWgtSetFocus(charge_curve_pg.lgraph1);//模拟按下移动标线开始移动
        SendKey(_ENTER);
    }
    else
    {
        WGTSetEnable(charge_curve_pg.lgraph1, FALSE);
        WGTSetVisible(charge_curve_pg.btmoving, FALSE);
        WGTSetVisible(charge_curve_pg.subpage1, FALSE);
//20220805 dyl 非全键盘不显示游标移动提示信息
#ifdef PANEL_NOKEYBOARD//G15:No_KeyBoard

#else
        WGTSetVisible(charge_curve_pg.stinfo, FALSE);
#endif
        LightGraphSetTouchMode(charge_curve_pg.lgraph1,MODE_FIXED);
    }
}

static void ReciveData(PGRAPH_WGT pwgtx,PPROT_DATREC_RSP_PKT ppkt)
{
    if(ppkt == NULL)
        return;
    {
        UI16 rec_len = ppkt->rec_len;//记录的数据个数，以word为单位。如果rec_len = 0，表示由trigger_id触发的记录结束，为最后一个包。
        FPOINT tmppoint;
        I16* pdata = (I16*)(ppkt+1);
        unsigned dataCount = ARRAY_NUM(gcmd2);//协议中的数据请求的个数
        unsigned i=0;
        PWGT pwgt;
        char buff[20];
        if(dataCount == 0 || (PanelCurPage() != PanelFindPage(charge_curve_pg_name)))//20191031.cyx 只在本页面绘制
            return;

        is_recivedata = TRUE;//20191031.cyx
        if(tracer_status)
        {
            SetTracerState(FALSE);
            PageWgtSetFocus(charge_curve_pg.btmove);//模拟按下移动标线开始移动
        }

        if(curve_clear)
        {
            curve_clear = FALSE;
            ClearChargeDrawCurve();
        }

//        if(VarAdrToInt(HISTORY_CURVE))
//        {
//            VarAdrSetInt(HISTORY_CURVE,0);
//        }

        if(current_curveid != charge_curveid)
        {
            current_curveid = charge_curveid;
            ShowChargeHistoryCurve();
        }

        for(i=0;i<rec_len/dataCount;i++)
        {
            tmppoint.x = pdata[i*dataCount]/10.0;//0x07160002 射出位置

            tmppoint.y = pdata[i*dataCount+1]/10.;//系统压力 1位小数
            pwgt = LightGraphGetCurve(charge_curve_pg.lgraph1,CURVE_CHGSYSPRS(charge_curveid,buff,20));
            LCurveAddPoint(pwgt,tmppoint.x,tmppoint.y);

            tmppoint.y = pdata[i*dataCount+2];//0x0A160002 电机转速
            pwgt = LightGraphGetCurve(charge_curve_pg.lgraph1,CURVE_CHGMOTORSPD(charge_curveid,buff,20));
            LCurveAddPoint(pwgt,tmppoint.x,tmppoint.y);

            tmppoint.y = pdata[i*dataCount+3];//0x09160002 设定压力
            pwgt = LightGraphGetCurve(charge_curve_pg.lgraph1,CURVE_CHGSETPRS(charge_curveid,buff,20));
            LCurveAddPoint(pwgt,tmppoint.x,tmppoint.y);

            tmppoint.y = pdata[i*dataCount+4]/10.;//0x1016000C 螺杆转速
            pwgt = LightGraphGetCurve(charge_curve_pg.lgraph1,CURVE_CHGSCREWRPM(charge_curveid,buff,20));
            LCurveAddPoint(pwgt,tmppoint.x,tmppoint.y);
        }

        if(rec_len == CURVE_TRANSEND)//结束
        {
            is_recivedata = FALSE;//20191031.cyx
            curve_clear = TRUE;
            charge_curveid++;
            if(charge_curveid >= CHARGE_CURVE_NUM)
            {
                charge_curveid = 0;
                clean_first_curve = TRUE;//开始清除曲线
            }
        }
    }
}

static void DataRequestOrder_Chg()
{
    gcmd2[0].part_id = 0x0710;
    gcmd2[0].sub_ind = 0x06;
    gcmd2[0].offset = 0x0002;//0x07160002 射出位置
//    if(VarAdrToInt(SYS_FL_MACH_CODE1) & 0x04)//Servo E
//    {
//        gcmd2[1].part_id = 0x0F10;
//        gcmd2[1].sub_ind = 0x06;
//        gcmd2[1].offset = 0x000C;//0x0F16000C 实际油箱压力
//    }
//    else if(((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x0004)==0) && (VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020))
//    {
//            gcmd2[1].part_id = 0x0A10;
//            gcmd2[1].sub_ind = 0x06;
//            gcmd2[1].offset = 0x0042;//0x0A160042 反馈压力
//    }
//    else
//    {
//        gcmd2[1].part_id = 0x0710;
//        gcmd2[1].sub_ind = 0x06;
//        if((VarAdrToInt(SYS_FL_CTRLMODEL) == 0xA620) ||(VarAdrToInt(SYS_FL_CTRLMODEL) == 0x2100)||(VarAdrToInt(SYS_FL_CTRLMODEL) == 0x3100))
//        {    //判断机型，系统压力
//            gcmd2[1].offset = 0x0005;  //0x07160005 压力
//        }
//        else if(VarAdrToInt(SYS_FL_CTRLMODEL)==0xA920 || VarAdrToInt(SYS_FL_CTRLMODEL)==0x5000)
//        {
//            gcmd2[1].offset = 0x0007;  //0x07160007 压力
//        }
//    }
    //20220818 dyl 系统压力使用d_Pump1_STATE_SERVO_PRESS
    gcmd2[1].part_id = 0x0F10;
    gcmd2[1].sub_ind = 0x06;
    gcmd2[1].offset = 0x000C;//0x0F16000C 实际油箱压力

    gcmd2[2].part_id = 0x0A10;
    gcmd2[2].sub_ind = 0x06;
    gcmd2[2].offset = 0x0002;//0x0A160002 电机转速
    gcmd2[3].part_id = 0x0910;
    gcmd2[3].sub_ind = 0x06;
    gcmd2[3].offset = 0x0002;//0x09160002 设定压力
    gcmd2[4].part_id = 0x1010;
    gcmd2[4].sub_ind = 0x06;
    gcmd2[4].offset = 0x000C;//0x1016000C 螺杆转速
}

static void CtlConnect()
{
}
static int Charge_CurveOnInit(Charge_Curve_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    BindGraphCB(G_CHARGE,ReciveData);
    LightGraphSetTracerCB(ppg->lgraph1,TraceDataShow);

    gcmd1.rec_id = G_CHARGE;
    gcmd1.ind_num = ARRAY_NUM(gcmd2);
    gcmd1.rec_period = VarAdrToInt(CURVE_CHECKTIME);//ms
    gcmd1.rec_total_ms = 0;
    gcmd1.trigger_id = 0x1012;//触发条件

    LightGraphSetTracerVertical(ppg->lgraph1,TRUE);//设置游标为竖直
    InitRange();

    return 0;
}
static int Charge_CurveOnShow(Charge_Curve_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    int i;

    NaviSetBtnJumpCB(BackToCharge);//设置当前页面跳转

    DataRequestOrder_Chg();//请求数据排序
    CurvesStart_New(&gcmd1,gcmd2);//自定义协议数据请求

    for(i = 0; i < CURVE_NUM; i++)
    {
        VarAdrSetInt(TMP_CURVE1_SEL + i, cb_sel[i]);
    }
    VarAdrSetInt(TMP_CURVE_X_MIN, curve_x_min);
    VarAdrSetInt(TMP_CURVE_X_MAX, curve_x_max);
    if((curve_x_max == 0) && VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_1))
    {
        LAxisSetRange(LightGraphGetCurve(charge_curve_pg.lgraph1,"scaleX"),0,VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_1)/10.0);
    }

    VarAdrSetInt(HISTORY_CURVE,0);
    ShowChargeHistoryCurve();

    SetTracerState(FALSE);

    //20230518 chj 曲线范围修改后，是否固定
    if(VarAdrToInt(TMP_CURVE_X_MAX))
    {
        LightGraphSetAutoAdapt(charge_curve_pg.lgraph1,FALSE);
    }
    else {
        LightGraphSetAutoAdapt(charge_curve_pg.lgraph1,TRUE);
    }

//    if((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x0004) == 0 && (VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020))
//    {
//        WGTSetAttrByName(ppg->syspress,EDIT_TEXT_NAME,d_servo1_STATE_SERVO_PRESS_1);
//    }
//    else
//    {
//        WGTSetAttrByName(ppg->syspress,EDIT_TEXT_NAME,d_Pump1_STATE_SERVO_PRESS);
//    }
    //20220818 dyl 系统压力使用d_Pump1_STATE_SERVO_PRESS、d_Pump2_STATE_SERVO_PRESS
    WGTSetAttrByName(ppg->syspress,EDIT_TEXT_NAME,d_Pump1_STATE_SERVO_PRESS); //反馈压力

    //20220805 dyl 非全键盘不显示游标移动提示信息
#ifdef PANEL_NOKEYBOARD//G15:No_KeyBoard
        WGTSetVisible(charge_curve_pg.stinfo, FALSE);
#endif

    return 0;
}
static int Charge_CurveOnHide(Charge_Curve_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    NaviSetBtnJumpCB(NaviBtnJumpDefault);//设置默认跳转

    CurvesStopEx(G_CHARGE);//关闭自定义的协议请求
    if(is_recivedata)//20191031.cyx
    {
        is_recivedata = FALSE;
        curve_clear = TRUE;
        charge_curveid++;
        if(charge_curveid >= CHARGE_CURVE_NUM)
        {
            charge_curveid = 0;
            clean_first_curve = TRUE;//开始清除曲线
        }
    }

    return 0;
}
static int Charge_CurveOnUpdate(Charge_Curve_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 speed_val;
    char cbuf[10];
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
    EditSetText(ppg->edit183,cbuf); //电机转速

    static int lightgraph_select = -1;
    if(lightgraph_select != LightGraphIsSelect(charge_curve_pg.lgraph1))
    {
        lightgraph_select = LightGraphIsSelect(charge_curve_pg.lgraph1);

        if(!lightgraph_select && tracer_status)
        {
            SetTracerState(FALSE);
            PageWgtSetFocus(charge_curve_pg.btmove);//模拟按下移动标线开始移动
        }
    }

    return 0;
}
static int Charge_CurveOnChange(Charge_Curve_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    int i;

    if(pwgt == ppg->dropdown1)//曲线显示
    {
        ShowChargeHistoryCurve();
    }
    else if(pwgt == ppg->btmove)
    {
        SetTracerState(TRUE);
    }
    else if(pwgt == ppg->btclear)
    {
        ClearLigthGraph();
    }
    else if(pwgt == ppg->checkbox1 || pwgt == ppg->checkbox2 || pwgt == ppg->checkbox3 || pwgt == ppg->checkbox4)//曲线选择
    {
        for(i = 0; i < CURVE_NUM; i++)
        {
            cb_sel[i] = VarAdrToInt(TMP_CURVE1_SEL + i);
        }
        ShowChargeHistoryCurve();
    }
    else if(pwgt == ppg->edxmin || pwgt == ppg->edxmax)
    {
        curve_x_min = VarAdrToInt(TMP_CURVE_X_MIN);
        curve_x_max = VarAdrToInt(TMP_CURVE_X_MAX);
        if(curve_x_max == 0)
        {
            //20230518 chj 曲线范围修改后，是否固定
            LightGraphSetAutoAdapt(charge_curve_pg.lgraph1,TRUE);

            if(VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_1))
            {
                LAxisSetRange(LightGraphGetCurve(charge_curve_pg.lgraph1,"scaleX"),0,VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_1)/10.0);
            }
            else
            {
                LAxisSetRange(LightGraphGetCurve(charge_curve_pg.lgraph1,"scaleX"),0,350);
            }
        }
        else
        {
            //20230518 chj 曲线范围修改后，是否固定
            LightGraphSetAutoAdapt(charge_curve_pg.lgraph1,FALSE);

            LAxisSetRange(LightGraphGetCurve(charge_curve_pg.lgraph1,"scaleX"),VarAdrToInt(TMP_CURVE_X_MIN)/10.0,VarAdrToInt(TMP_CURVE_X_MAX)/10.0);
        }
    } 
    return 0;
}
