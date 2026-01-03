#include "WInject_Curve_1.h"
#include "typedef.h"
#include "curvesctl.h"
#include "database.h"
#include "dropwgt.h"
#include "oprintf.h"
#include "privdef.h"
#include "keymod.h"
#include "key.h"
#include "stdio.h"
#include "checkboxwgt.h"
#include "texttbl.h"
#include "labelwgt.h"

DEFINE_INJECT_CURVE_1_MAP

CONNECT_INJECT_CURVE_1_EVENT

#define     CURVE_SYSPRS(curveid,buff,size)         joinStrAndNum("sysprs",curveid+1,buff,size)
#define     CURVE_INJSPD(curveid,buff,size)         joinStrAndNum("injspd",curveid+1,buff,size)
#define     CURVE_INJPOS(curveid,buff,size)         joinStrAndNum("injpos",curveid+1,buff,size)
#define     CURVE_SETPRS(curveid,buff,size)         joinStrAndNum("setprs",curveid+1,buff,size)

#define     HISTORY_CURVE             TMPVAL(100) //历史曲线选择
#define     INJECT_CURVE_NUM          2           //当前曲线+历史曲线总条数

#define     A_TIME_VAL                TMPSTR(1)   //A游标指示时间值
#define     A_INJPOS_VAL              TMPSTR(2)   //A游标指示射出位置值
#define     A_SYSPRS_VAL              TMPSTR(4)   //A游标指示系统压力值
#define     A_INJSPD_VAL              TMPSTR(5)   //A游标指示射出速度值
#define     B_TIME_VAL                TMPSTR(6)   //B游标指示时间值
#define     B_INJPOS_VAL              TMPSTR(7)  //B游标指示射出位置值
#define     B_SYSPRS_VAL              TMPSTR(9)   //B游标指示系统压力值
#define     B_INJSPD_VAL              TMPSTR(10)   //B游标指示射出速度值

static BOOL tracer_status = FALSE;//是否在移动标线状态
//曲线采用自定义发送协议曲线请求
static PROT_DATREC_REQ_PKT gcmd1;
static DATSET_IND gcmd2[5] ;			//协议数据2
static BOOL curve_clear = TRUE;
static BOOL clean_first_curve = FALSE;//交替画曲线，画之前先清空
static UI16 inject_curveid = 0;//曲线编号，用于显示历史曲线
static UI16 current_curveid = 0;//当前曲线id，与inject_curveid差1
static double injcurve_num = 0;
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

static void TraceDataShow(PLIGHTGRAPH_WGT plot ,int tracerid)
{
    double injtime,sysprs, injspd, injpos;
    PWGT pwgt;
    LDPoint point;
    char buff[20];
    int curve_id;

//    if(VarAdrToInt(HISTORY_CURVE))
//    {
//        curve_id = inject_curveid;
//    }
//    else
    {
        curve_id = current_curveid;
    }

    if(tracerid == 1)//A游标
    {
        pwgt = LightGraphGetCurve((PWGT)plot,CURVE_SYSPRS(curve_id,buff,20));
        point = LCurveTracer(pwgt,tracerid);
        injtime = point.key;
        snprintf(buff,sizeof(buff)-1,"%.2f",injtime);
        VarAdrSetStr(A_TIME_VAL, buff);
        sysprs = point.value;
        snprintf(buff,sizeof(buff)-1,"%.1f",sysprs);
        VarAdrSetStr(A_SYSPRS_VAL, buff);

        pwgt = LightGraphGetCurve((PWGT)plot,CURVE_INJSPD(curve_id,buff,20));
        point = LCurveTracer(pwgt,tracerid);
        injspd = point.value;
        snprintf(buff,sizeof(buff)-1,"%.1f",injspd);
        VarAdrSetStr(A_INJSPD_VAL, buff);

        pwgt = LightGraphGetCurve((PWGT)plot,CURVE_INJPOS(curve_id,buff,20));
        point = LCurveTracer(pwgt,tracerid);
        injpos = point.value;
        snprintf(buff,sizeof(buff)-1,"%.1f",injpos);
        VarAdrSetStr(A_INJPOS_VAL, buff);
    }
    else if(tracerid == 2)//B游标
    {
        pwgt = LightGraphGetCurve((PWGT)plot,CURVE_SYSPRS(curve_id,buff,20));
        point = LCurveTracer(pwgt,tracerid);
        injtime = point.key;
        snprintf(buff,sizeof(buff)-1,"%.2f",injtime);
        VarAdrSetStr(B_TIME_VAL, buff);
        sysprs = point.value;
        snprintf(buff,sizeof(buff)-1,"%.1f",sysprs);
        VarAdrSetStr(B_SYSPRS_VAL, buff);

        pwgt = LightGraphGetCurve((PWGT)plot,CURVE_INJSPD(curve_id,buff,20));
        point = LCurveTracer(pwgt,tracerid);
        injspd = point.value;
        snprintf(buff,sizeof(buff)-1,"%.1f",injspd);
        VarAdrSetStr(B_INJSPD_VAL, buff);

        pwgt = LightGraphGetCurve((PWGT)plot,CURVE_INJPOS(curve_id,buff,20));
        point = LCurveTracer(pwgt,tracerid);
        injpos = point.value;
        snprintf(buff,sizeof(buff)-1,"%.1f",injpos);
        VarAdrSetStr(B_INJPOS_VAL, buff);
    }
}

//清除要画的曲线
static void ClearInjectDrawCurve()
{
    PWGT pwgt;
    char buff[20];

    if(clean_first_curve)
    {
        pwgt = LightGraphGetCurve(inject_curve_1_pg.lgraph1,CURVE_SYSPRS(inject_curveid,buff,20));
        LCurveClear(pwgt);
        pwgt = LightGraphGetCurve(inject_curve_1_pg.lgraph1,CURVE_INJSPD(inject_curveid,buff,20));
        LCurveClear(pwgt);
        pwgt = LightGraphGetCurve(inject_curve_1_pg.lgraph1,CURVE_INJPOS(inject_curveid,buff,20));
        LCurveClear(pwgt);
        pwgt = LightGraphGetCurve(inject_curve_1_pg.lgraph1,CURVE_SETPRS(inject_curveid,buff,20));
        LCurveClear(pwgt);
    }
}

//显示历史曲线
static void ShowInjectHistoryCurve()
{
    PWGT pwgt[CURVE_NUM];
    char buff[20];
    int i,j;

    for(i = 0;i < INJECT_CURVE_NUM;i++)
    {
        pwgt[0] = LightGraphGetCurve(inject_curve_1_pg.lgraph1,CURVE_SYSPRS(i,buff,20));
        pwgt[1] = LightGraphGetCurve(inject_curve_1_pg.lgraph1,CURVE_INJSPD(i,buff,20));
        pwgt[2] = LightGraphGetCurve(inject_curve_1_pg.lgraph1,CURVE_INJPOS(i,buff,20));
        pwgt[3] = LightGraphGetCurve(inject_curve_1_pg.lgraph1,CURVE_SETPRS(i,buff,20));
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
    pwgt = LightGraphGetCurve(inject_curve_1_pg.lgraph1,"scaleX");
    LAxisSetRange(pwgt,0,3);//初始0-3s
    pwgt = LightGraphGetCurve(inject_curve_1_pg.lgraph1,"scaleY1");
    LAxisSetRange(pwgt,0,140);//初始0-140
    pwgt = LightGraphGetCurve(inject_curve_1_pg.lgraph1,"scaleY2");
    LAxisSetRange(pwgt,0,200);//初始0-200
    pwgt = LightGraphGetCurve(inject_curve_1_pg.lgraph1,"scaleY3");
    if(VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_1))
    {
        LAxisSetRange(pwgt,0,VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_1)/10.0);
    }
    else
    {
        LAxisSetRange(pwgt,0,350);
    }
    LightGraphSaveRange(inject_curve_1_pg.lgraph1);
}

//清除曲线
static void ClearLigthGraph()
{
    CurvesClear_New(G_INJECT);

    LightGraphClear(inject_curve_1_pg.lgraph1);
    inject_curveid = 0;//曲线编号，用于显示历史曲线
    current_curveid = 0;
    clean_first_curve = FALSE;
    LightGraphSetTouchMode(inject_curve_1_pg.lgraph1,MODE_FIXED);

    VarAdrSetInt(HISTORY_CURVE,0);
    ShowInjectHistoryCurve();//20230516 chj 修复在旋转历史曲线后，再选择复位，再做曲线，曲线颜色不正确问题，
    curve_x_min = 0.0;
    curve_x_max = 0.0;
    VarAdrSetInt(TMP_CURVE_X_MIN, 0);
    VarAdrSetInt(TMP_CURVE_X_MAX, 0);
    InitRange();
    //20230518 chj 曲线范围修改后，是否固定
    LightGraphSetAutoAdapt(inject_curve_1_pg.lgraph1,TRUE);

}

//设置游标状态
static void SetTracerState(BOOL status)
{
    tracer_status = status;

    if(status)
    {
        WGTSetEnable(inject_curve_1_pg.lgraph1, TRUE);
        WGTSetVisible(inject_curve_1_pg.btmoving, TRUE);
        WGTSetVisible(inject_curve_1_pg.subpage1, TRUE);
//20220805 dyl 非全键盘不显示游标移动提示信息
#ifdef PANEL_NOKEYBOARD//G15:No_KeyBoard

#else
        WGTSetVisible(inject_curve_1_pg.stinfo, TRUE);
#endif
        LightGraphSetTouchMode(inject_curve_1_pg.lgraph1,MODE_TRACER);
        PageWgtSetFocus(inject_curve_1_pg.lgraph1);//模拟按下移动标线开始移动
        SendKey(_ENTER);
    }
    else
    {
        WGTSetEnable(inject_curve_1_pg.lgraph1, FALSE);
        WGTSetVisible(inject_curve_1_pg.btmoving, FALSE);
        WGTSetVisible(inject_curve_1_pg.subpage1, FALSE);
//20220805 dyl 非全键盘不显示游标移动提示信息
#ifdef PANEL_NOKEYBOARD//G15:No_KeyBoard

#else
        WGTSetVisible(inject_curve_1_pg.stinfo, FALSE);
#endif
        LightGraphSetTouchMode(inject_curve_1_pg.lgraph1,MODE_FIXED);
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
        double wPeriod =1.037847;//20210305.dxl  根据上传的周期，面板自己累加时间 + //20220831 dyl
        if(dataCount == 0 || (PanelCurPage() != PanelFindPage(inject_curve_1_pg_name)))//20191031.cyx 只在本页面绘制
            return;

        is_recivedata = TRUE;//20191031.cyx
        if(tracer_status)
        {
            SetTracerState(FALSE);
            PageWgtSetFocus(inject_curve_1_pg.btmove);//模拟按下移动标线开始移动
        }

        if(curve_clear)
        {
            curve_clear = FALSE;
            ClearInjectDrawCurve();
        }

//        if(VarAdrToInt(HISTORY_CURVE))
//        {
//            VarAdrSetInt(HISTORY_CURVE,0);
//        }

        if(current_curveid != inject_curveid)
        {
            current_curveid = inject_curveid;
            ShowInjectHistoryCurve();
        }

        for(i=0;i<rec_len/dataCount;i++)
        {
            injcurve_num+=1;
            tmppoint.x = injcurve_num * VarAdrToInt(CURVE_CHECKTIME)*wPeriod / 1000.;//20220831 dyl

            tmppoint.y = pdata[i*dataCount]/10.0;//0x07160002 射出位置
            pwgt = LightGraphGetCurve(inject_curve_1_pg.lgraph1,CURVE_INJPOS(inject_curveid,buff,20));
            LCurveAddPoint(pwgt,tmppoint.x,tmppoint.y);

            tmppoint.y = pdata[i*dataCount+1]/10.;//系统压力 1位小数
            pwgt = LightGraphGetCurve(inject_curve_1_pg.lgraph1,CURVE_SYSPRS(inject_curveid,buff,20));
            LCurveAddPoint(pwgt,tmppoint.x,tmppoint.y);

            tmppoint.y = pdata[i*dataCount+2]/10.0;//0x10160012 射出速度
            pwgt = LightGraphGetCurve(inject_curve_1_pg.lgraph1,CURVE_INJSPD(inject_curveid,buff,20));
            LCurveAddPoint(pwgt,tmppoint.x,tmppoint.y);

            tmppoint.y = pdata[i*dataCount+3];//0x09160002 设定压力
            pwgt = LightGraphGetCurve(inject_curve_1_pg.lgraph1,CURVE_SETPRS(inject_curveid,buff,20));
            LCurveAddPoint(pwgt,tmppoint.x,tmppoint.y);
        }

        if(rec_len == CURVE_TRANSEND)//结束
        {
            is_recivedata = FALSE;//20191031.cyx
            injcurve_num = 0;
            curve_clear = TRUE;
            inject_curveid++;
            if(inject_curveid >= INJECT_CURVE_NUM)
            {
                inject_curveid = 0;
                clean_first_curve = TRUE;//开始清除曲线
            }
            LightGraphSaveRange(inject_curve_1_pg.lgraph1);//时间范围保存
        }
    }
}

static void DataRequestOrder_Inj()
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
    //20241023 chj 射出压力独立检测功能选择使用时，射出曲线画面的系统压力文字改为射出压力，dic替换为10160075
    if(VarAdrToInt(d_inject1_MACHSET_FL_INJPRESS))
    {
        gcmd2[1].part_id = 0x1010;
        gcmd2[1].sub_ind = 0x06;
        gcmd2[1].offset = 0x0075;//0x10160075 射出压力
        CheckboxSetText(inject_curve_1_pg.checkbox1,GetTextTran(TEXT_INJPRS_BAR));
        LabelSetText(inject_curve_1_pg.labelsysprs,GetTextTran(TEXT_INJPRS));
        WGTSetAttrByName(inject_curve_1_pg.edSysPrs,EDIT_TEXT_NAME,d_inject1_STATE_INJ_HLD_PRES);
    }
    else
    {
        //20220818 dyl 系统压力使用d_Pump1_STATE_SERVO_PRESS
        gcmd2[1].part_id = 0x0F10;
        gcmd2[1].sub_ind = 0x06;
        gcmd2[1].offset = 0x000C;//0x0F16000C 实际油箱压力
        CheckboxSetText(inject_curve_1_pg.checkbox1,GetTextTran(TEXT_SYSPRS_BAR));
        LabelSetText(inject_curve_1_pg.labelsysprs,GetTextTran(TEXT_SYSPRS));
        WGTSetAttrByName(inject_curve_1_pg.edSysPrs,EDIT_TEXT_NAME,d_Pump1_STATE_SERVO_PRESS);

    }
    gcmd2[2].part_id = 0x1010;
    gcmd2[2].sub_ind = 0x06;
    gcmd2[2].offset = 0x0012;  //0x10160012 射出速度
    gcmd2[3].part_id = 0x0910;
    gcmd2[3].sub_ind = 0x06;
    gcmd2[3].offset = 0x0002;  //0x09160002 设定压力
    gcmd2[4].part_id = 0x1010;
    gcmd2[4].sub_ind = 0x06;
    gcmd2[4].offset = 0x0017;  //0x10160017 射出子操作步骤，新增数据库资料，旧主机返回0
}

static void BackToInject(int btnid, PNAVI_BTN pbtn) //20191211.cyx 曲线画面返回特殊处理
{
    if(btnid == 6)
    {
        PanelShowPageByName(PAGE_INJECT); //画面的跳转表先清空，否则不会执行改行
    }
    else
    {
        NaviBtnJumpDefault(btnid, pbtn);
    }
}

static void CtlConnect()
{
}
static int Inject_Curve_1OnInit(Inject_Curve_1_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    BindGraphCB(G_INJECT,ReciveData);
    LightGraphSetTracerCB(ppg->lgraph1,TraceDataShow);

    gcmd1.rec_id = G_INJECT;
    gcmd1.ind_num = ARRAY_NUM(gcmd2);
    gcmd1.rec_period = VarAdrToInt(CURVE_CHECKTIME);//ms
    gcmd1.rec_total_ms = 0;
    gcmd1.trigger_id = 0x1011;//触发条件

    LightGraphSetTracerVertical(ppg->lgraph1,TRUE);//设置游标为竖直
    InitRange();

    return 0;
}
static int Inject_Curve_1OnShow(Inject_Curve_1_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    int i;

    NaviSetBtnJumpCB(BackToInject);//设置当前页面跳转

    DataRequestOrder_Inj();//请求数据排序
    CurvesStart_New(&gcmd1,gcmd2);//自定义协议数据请求

    for(i = 0; i < CURVE_NUM; i++)
    {
        VarAdrSetInt(TMP_CURVE1_SEL + i, cb_sel[i]);
    }
    VarAdrSetInt(TMP_CURVE_X_MIN, curve_x_min);
    VarAdrSetInt(TMP_CURVE_X_MAX, curve_x_max);
    if(VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_1))
    {
        LAxisSetRange(LightGraphGetCurve(inject_curve_1_pg.lgraph1,"scaleY3"),0,VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_1)/10.0);
    }

    VarAdrSetInt(HISTORY_CURVE,0);
    ShowInjectHistoryCurve();

    SetTracerState(FALSE);

    //20230518 chj 曲线范围修改后，是否固定
    if(VarAdrToInt(TMP_CURVE_X_MAX))
    {
        LightGraphSetAutoAdapt(inject_curve_1_pg.lgraph1,FALSE);
    }
    else {
        LightGraphSetAutoAdapt(inject_curve_1_pg.lgraph1,TRUE);
    }
//    if((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x0004) == 0 && (VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020))
//    {
//        WGTSetAttrByName(ppg->edSysPrs,EDIT_TEXT_NAME,d_servo1_STATE_SERVO_PRESS_1);
//    }
//    else
//    {
//        WGTSetAttrByName(ppg->edSysPrs,EDIT_TEXT_NAME,d_Pump1_STATE_SERVO_PRESS);
//    }
    //20220818 dyl 系统压力使用d_Pump1_STATE_SERVO_PRESS、d_Pump2_STATE_SERVO_PRESS //20241023 chj 射出压力独立检测功能选择使用时，射出曲线画面的系统压力文字改为射出压力，dic替换为10160075
    //WGTSetAttrByName(ppg->edSysPrs,EDIT_TEXT_NAME,d_Pump1_STATE_SERVO_PRESS); //反馈压力

    //20220805 dyl 非全键盘不显示游标移动提示信息
#ifdef PANEL_NOKEYBOARD//G15:No_KeyBoard
    WGTSetVisible(inject_curve_1_pg.stinfo, FALSE);
#endif

    return 0;
}
static int Inject_Curve_1OnHide(Inject_Curve_1_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    NaviSetBtnJumpCB(NaviBtnJumpDefault);//设置默认跳转

    CurvesStopEx(G_INJECT);//关闭自定义的协议请求
    if(is_recivedata)//20191031.cyx
    {
        is_recivedata = FALSE;
        injcurve_num = 0;
        curve_clear = TRUE;
        inject_curveid++;
        if(inject_curveid >= INJECT_CURVE_NUM)
        {
            inject_curveid = 0;
            clean_first_curve = TRUE;//开始清除曲线
        }
    }
    return 0;
}
static int Inject_Curve_1OnUpdate(Inject_Curve_1_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    static int lightgraph_select = -1;
    if(lightgraph_select != LightGraphIsSelect(inject_curve_1_pg.lgraph1))
    {
        lightgraph_select = LightGraphIsSelect(inject_curve_1_pg.lgraph1);

        if(!lightgraph_select && tracer_status)
        {
            SetTracerState(FALSE);
            PageWgtSetFocus(inject_curve_1_pg.btmove);//模拟按下移动标线开始移动
        }
    }

    return 0;
}
static int Inject_Curve_1OnChange(Inject_Curve_1_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    int i;

    if(pwgt == ppg->dropdown1)//曲线显示
    {
        ShowInjectHistoryCurve();
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
        ShowInjectHistoryCurve();
    }
    else if(pwgt == ppg->edxmin || pwgt == ppg->edxmax)
    {
        curve_x_min = VarAdrToInt(TMP_CURVE_X_MIN);
        curve_x_max = VarAdrToInt(TMP_CURVE_X_MAX);
        if(curve_x_max == 0)
        {
            //20230518 chj 曲线范围修改后，是否固定
            LightGraphSetAutoAdapt(inject_curve_1_pg.lgraph1,TRUE);
            LightGraphRecovery(inject_curve_1_pg.lgraph1);
        }
        else
        {
            //20230518 chj 曲线范围修改后，是否固定
            LightGraphSetAutoAdapt(inject_curve_1_pg.lgraph1,FALSE);
            LAxisSetRange(LightGraphGetCurve(inject_curve_1_pg.lgraph1,"scaleX"),VarAdrToInt(TMP_CURVE_X_MIN)/100.0,VarAdrToInt(TMP_CURVE_X_MAX)/100.0);
        }
    }

    return 0;
}


