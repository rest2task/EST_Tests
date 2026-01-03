#include "WServo_Curve.h"
#include "typedef.h"
#include "pagefram.h"
#include "graphwgt.h"
#include "editwgt.h"
#include "database.h"
#include "oprintf.h"
#include "keymod.h"
#include "buttonwgt.h"
#include "privdef.h"
#include "texttbl.h"
#include "stdio.h"
#include "lightgraphwgt.h"
#include "curvesctl.h"
#include "math.h"

DEFINE_SERVO_CURVE_MAP

CONNECT_SERVO_CURVE_EVENT

#define TMP_TIME_MAX    TMPVAL(20)//时间最大值
#define TMP_TIME_MIN    TMPVAL(21)//时间最小值
#define A_TIME_VAL      TMPSTR(0) //A游标指示时间值
#define A_FDBKPRS_VAL   TMPSTR(1) //A游标指示反馈压力值
#define A_CYLISPD_VAL   TMPSTR(2) //A游标指示油缸速度值
#define A_CMDSPD_VAL    TMPSTR(3) //A游标指示指令速度值
#define A_FDBKSPD_VAL   TMPSTR(4) //A游标指示反馈速度值
#define B_TIME_VAL      TMPSTR(10) //B游标指示时间值
#define B_FDBKPRS_VAL   TMPSTR(11) //B游标指示反馈压力值
#define B_CYLISPD_VAL   TMPSTR(12) //B游标指示油缸速度值
#define B_CMDSPD_VAL    TMPSTR(13) //B游标指示指令速度值
#define B_FDBKSPD_VAL   TMPSTR(14) //B游标指示反馈速度值
#define TMP_CURVE1_CUR  TMPVAL(30) //曲线1当前值
#define TMP_CURVE2_CUR  TMPVAL(31) //曲线2当前值
#define TMP_CURVE3_CUR  TMPVAL(32) //曲线3当前值
#define TMP_CURVE4_CUR  TMPVAL(33) //曲线4当前值

static BOOL isstart = FALSE;
static BOOL trace_update = FALSE;//游标刷新
static UI16 x_time_max = 20;
static UI16 x_time_min = 0;

//曲线采用自定义发送协议曲线请求
static PROT_DATREC_REQ_PKT gcmdecat;
static DATSET_IND gcmd2[4] ;			//协议数据2
static double ecatcurve_num = 0;

//曲线游标值显示
static void VerTraceDataShow(PLIGHTGRAPH_WGT plot ,int tracerid)
{
    double time, fdbkprs, cylispd, cmdspd, fdbkspd;
    PWGT pwgt;
    LDPoint point;
    char buff[20];

    if(tracerid == 1)//A游标
    {
        pwgt = LightGraphGetCurve((PWGT)plot,"curve1");
        point = LCurveTracer(pwgt,tracerid);
        time = point.key;
        snprintf(buff,sizeof(buff)-1,"%.2f",time);
        VarAdrSetStr(A_TIME_VAL, buff);
        fdbkprs = point.value;
        snprintf(buff,sizeof(buff)-1,"%.1f",fdbkprs);
        VarAdrSetStr(A_FDBKPRS_VAL, buff);

        pwgt = LightGraphGetCurve((PWGT)plot,"curve2");
        point = LCurveTracer(pwgt,tracerid);
        cylispd = point.value;
        snprintf(buff,sizeof(buff)-1,"%.1f",cylispd);
        VarAdrSetStr(A_CYLISPD_VAL, buff);

        pwgt = LightGraphGetCurve((PWGT)plot,"curve3");
        point = LCurveTracer(pwgt,tracerid);
        cmdspd = point.value;
        snprintf(buff,sizeof(buff)-1,"%.f",cmdspd);
        VarAdrSetStr(A_CMDSPD_VAL, buff);

        pwgt = LightGraphGetCurve((PWGT)plot,"curve4");
        point = LCurveTracer(pwgt,tracerid);
        fdbkspd = point.value;
        snprintf(buff,sizeof(buff)-1,"%.f",fdbkspd);
        VarAdrSetStr(A_FDBKSPD_VAL, buff);
    }
    else if(tracerid == 2)//B游标
    {
        pwgt = LightGraphGetCurve((PWGT)plot,"curve1");
        point = LCurveTracer(pwgt,tracerid);
        time = point.key;
        snprintf(buff,sizeof(buff)-1,"%.2f",time);
        VarAdrSetStr(B_TIME_VAL, buff);
        fdbkprs = point.value;
        snprintf(buff,sizeof(buff)-1,"%.1f",fdbkprs);
        VarAdrSetStr(B_FDBKPRS_VAL, buff);

        pwgt = LightGraphGetCurve((PWGT)plot,"curve2");
        point = LCurveTracer(pwgt,tracerid);
        cylispd = point.value;
        snprintf(buff,sizeof(buff)-1,"%.1f",cylispd);
        VarAdrSetStr(B_CYLISPD_VAL, buff);

        pwgt = LightGraphGetCurve((PWGT)plot,"curve3");
        point = LCurveTracer(pwgt,tracerid);
        cmdspd = point.value;
        snprintf(buff,sizeof(buff)-1,"%.f",cmdspd);
        VarAdrSetStr(B_CMDSPD_VAL, buff);

        pwgt = LightGraphGetCurve((PWGT)plot,"curve4");
        point = LCurveTracer(pwgt,tracerid);
        fdbkspd = point.value;
        snprintf(buff,sizeof(buff)-1,"%.f",fdbkspd);
        VarAdrSetStr(B_FDBKSPD_VAL, buff);
    }
}

static void ServoCurvePageIn()
{
    PWGT pwgt;
    double y_min,y_max;

    VarAdrSetInt(TMP_TIME_MIN, x_time_min);
    VarAdrSetInt(TMP_TIME_MAX, x_time_max);
    pwgt = LightGraphGetCurve(servo_curve_pg.lgraph1,"scaleX");
    LAxisSetRange(pwgt, x_time_min, x_time_max);

    y_min = pow(-1,VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MINFLAG1)) * VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MINSET1);
    y_max = pow(-1,VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MAXFLAG1)) * VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MAXSET1);
    pwgt = LightGraphGetCurve(servo_curve_pg.lgraph1,"scaleY1");
    LAxisSetRange(pwgt, y_min, y_max);

    y_min = pow(-1,VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MINFLAG2)) * VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MINSET2);
    y_max = pow(-1,VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MAXFLAG2)) * VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MAXSET2);
    pwgt = LightGraphGetCurve(servo_curve_pg.lgraph1,"scaleY2");
    LAxisSetRange(pwgt, y_min, y_max);

    y_min = pow(-1,VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MINFLAG3)) * VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MINSET3);
    y_max = pow(-1,VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MAXFLAG3)) * VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MAXSET3);
    pwgt = LightGraphGetCurve(servo_curve_pg.lgraph1,"scaleY3");
    LAxisSetRange(pwgt, y_min, y_max);

    y_min = pow(-1,VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MINFLAG4)) * VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MINSET4);
    y_max = pow(-1,VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MAXFLAG4)) * VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MAXSET4);
    pwgt = LightGraphGetCurve(servo_curve_pg.lgraph1,"scaleY4");
    LAxisSetRange(pwgt, y_min, y_max);

    isstart = FALSE;
    ButtonSetText(servo_curve_pg.btstart,GetTextTran(TEXT_CLAMP_START));

    LightGraphSetTouchMode(servo_curve_pg.lgraph1,MODE_TRACER);
    trace_update = TRUE;
}

static void ServoCurveUpdate()
{
    UI16 value;
    UI16 servo_id;

    if(LightGraphGetTouchMode(servo_curve_pg.lgraph1) == MODE_TRACER)
    {
        if(trace_update)
        {
            trace_update = FALSE;
            VerTraceDataShow(WgtToLightGraph(servo_curve_pg.lgraph1),1);
            VerTraceDataShow(WgtToLightGraph(servo_curve_pg.lgraph1),2);
        }
    }

    servo_id = VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_SERVOID);
    //曲线1 反馈压力
    value = VarAdrToInt(d_Pump1_STATE_SERVO_PRESS + (servo_id - 1)*0x100000);
    VarAdrSetInt(TMP_CURVE1_CUR, value);
    //曲线2 油缸速度
    //value = VarAdrToInt(d_Pump1_STATE_SERVO_PRESS + (servo_id - 1)*0x100000);
    if(value >= 32768)
    {
        VarAdrSetInt(TMP_CURVE2_CUR, value - 65535);
    }
    else
    {
        VarAdrSetInt(TMP_CURVE2_CUR, value);
    }
    //曲线3 指令速度
    value = VarAdrToInt(d_Pump1_STATE_MOTOR_SPEED_SET + (servo_id - 1)*0x100000);
    if(value >= 32768)
    {
        VarAdrSetInt(TMP_CURVE3_CUR, value - 65535);
    }
    else
    {
        VarAdrSetInt(TMP_CURVE3_CUR, value);
    }
    //曲线4 反馈速度
    value = VarAdrToInt(d_Pump1_STATE_MOTOR_SPEED_REAL + (servo_id - 1)*0x100000);
    if(value >= 32768)
    {
        VarAdrSetInt(TMP_CURVE4_CUR, value - 65535);
    }
    else
    {
        VarAdrSetInt(TMP_CURVE4_CUR, value);
    }
}

static void DataRequestOrder_ECAT()
{
    UI16 servo_id;

    gcmdecat.rec_id = G_ECAT;
    gcmdecat.ind_num = ARRAY_NUM(gcmd2);
    gcmdecat.rec_period = VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_PERIOD);//ms
    gcmdecat.rec_total_ms = 0;
    if(VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_COLLECTMOD) == 0)
    {
        gcmdecat.trigger_id = 0x1011;//触发条件 射出
    }
    else if(VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_COLLECTMOD) == 1)
    {
        gcmdecat.trigger_id = 0x2013;//触发条件 开模
    }
    else if(VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_COLLECTMOD) == 2)
    {
        gcmdecat.trigger_id = 0x2011;//触发条件 关模
    }
    else if(VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_COLLECTMOD) == 3)
    {
        gcmdecat.trigger_id = 0x2111;//触发条件 托模
    }
    else
    {
        gcmdecat.trigger_id = 0;//持续
    }

    servo_id = VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_SERVOID);
    gcmd2[0].part_id = 0x0F10 + (servo_id - 1)*0x10;
    gcmd2[0].sub_ind = 0x06;
    gcmd2[0].offset = 0x000C;//0x0f16000c 实际油箱压力
    gcmd2[1].part_id = 0x0F10 + (servo_id - 1)*0x10;
    gcmd2[1].sub_ind = 0x06;
    gcmd2[1].offset = 0x0;//油缸速度
    gcmd2[2].part_id = 0x0F10 + (servo_id - 1)*0x10;
    gcmd2[2].sub_ind = 0x06;
    gcmd2[2].offset = 0x0007;//0x0f160007 电机指令速度
    gcmd2[3].part_id = 0x0F10 + (servo_id - 1)*0x10;
    gcmd2[3].sub_ind = 0x06;
    gcmd2[3].offset = 0x0006;//0x0f160006 电机实际转速
}

static void ReciveData(PGRAPH_WGT pwgtx,PPROT_DATREC_RSP_PKT ppkt)
{
    if(ppkt == NULL)
        return;
    {
        UI16 rec_len = ppkt->rec_len;//记录的数据个数，以word为单位。如果rec_len = 0，表示由trigger_id触发的记录结束，为最后一个包。
        //UI16 rec_id = ppkt->rec_id;
        FPOINT tmppoint;
        I16* pdata = (I16*)(ppkt+1);
        unsigned dataCount = ARRAY_NUM(gcmd2);//协议中的数据请求的个数
        unsigned i=0;
        PWGT pwgt;
        if(dataCount == 0 || (PanelCurPage() != PanelFindPage(servo_curve_pg_name)))//20191031.cyx 只在本页面绘制
            return;

        for(i=0;i<rec_len/dataCount;i++)
        {
            ecatcurve_num+=1;
            tmppoint.x = ecatcurve_num * VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_PERIOD) / 100.;

            tmppoint.y = pdata[i*dataCount]/10.0;//0x0f16000c 实际油箱压力 1位小数
            pwgt = LightGraphGetCurve(servo_curve_pg.lgraph1,"curve1");
            LCurveAddPoint(pwgt, tmppoint.x, tmppoint.y);

            tmppoint.y = pdata[i*dataCount+1]/10.0;//油缸速度 1位小数
            pwgt = LightGraphGetCurve(servo_curve_pg.lgraph1,"curve2");
            LCurveAddPoint(pwgt, tmppoint.x, tmppoint.y);

            tmppoint.y = pdata[i*dataCount+2];//0x0f160007 电机指令速度 0位小数
            pwgt = LightGraphGetCurve(servo_curve_pg.lgraph1,"curve3");
            LCurveAddPoint(pwgt, tmppoint.x, tmppoint.y);

            tmppoint.y = pdata[i*dataCount+3];//0x0f160006 电机实际转速 0位小数
            pwgt = LightGraphGetCurve(servo_curve_pg.lgraph1,"curve4");
            LCurveAddPoint(pwgt, tmppoint.x, tmppoint.y);
        }

        if(rec_len == CURVE_TRANSEND)//结束
        {
            ecatcurve_num = 0;
        }
    }

}

static void CtlConnect()
{
}
static int Servo_CurveOnInit(Servo_Curve_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    BindGraphCB(G_ECAT,ReciveData);
    LightGraphSetTracerCB(ppg->lgraph1,VerTraceDataShow);
    return 0;
}
static int Servo_CurveOnShow(Servo_Curve_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    ServoCurvePageIn();

    return 0;
}
static int Servo_CurveOnHide(Servo_Curve_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    CurvesStopEx(G_ECAT);//关闭自定义的协议请求

    return 0;
}
static int Servo_CurveOnUpdate(Servo_Curve_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    ServoCurveUpdate();

    return 0;
}
static int Servo_CurveOnChange(Servo_Curve_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    PWGT pwgtx;
    UI32 addr,value;
    PATTR pattr;
    double y_min,y_max;

    pattr = (PATTR)msg_para;

    addr = OBJAttrGetAdr(pattr);
    value = VarAdrToInt(addr);

    trace_update = TRUE;

    switch(addr)
    {
    case TMP_TIME_MIN:
        x_time_min = value;
        pwgtx = LightGraphGetCurve(servo_curve_pg.lgraph1,"scaleX");
        LAxisSetRange(pwgtx, x_time_min, x_time_max);
        break;
    case TMP_TIME_MAX:
        x_time_max = value;
        pwgtx = LightGraphGetCurve(servo_curve_pg.lgraph1,"scaleX");
        LAxisSetRange(pwgtx, x_time_min, x_time_max);
        break;
    case d_autoctrl1_MOLDSET_ECAT_CURVE_MINFLAG1:
    case d_autoctrl1_MOLDSET_ECAT_CURVE_MINSET1:
    case d_autoctrl1_MOLDSET_ECAT_CURVE_MAXFLAG1:
    case d_autoctrl1_MOLDSET_ECAT_CURVE_MAXSET1:
        y_min = pow(-1,VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MINFLAG1)) * VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MINSET1);
        y_max = pow(-1,VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MAXFLAG1)) * VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MAXSET1);
        pwgtx = LightGraphGetCurve(servo_curve_pg.lgraph1,"scaleY1");
        LAxisSetRange(pwgtx, y_min, y_max);
        break;
    case d_autoctrl1_MOLDSET_ECAT_CURVE_MINFLAG2:
    case d_autoctrl1_MOLDSET_ECAT_CURVE_MINSET2:
    case d_autoctrl1_MOLDSET_ECAT_CURVE_MAXFLAG2:
    case d_autoctrl1_MOLDSET_ECAT_CURVE_MAXSET2:
        y_min = pow(-1,VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MINFLAG2)) * VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MINSET2);
        y_max = pow(-1,VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MAXFLAG2)) * VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MAXSET2);
        pwgtx = LightGraphGetCurve(servo_curve_pg.lgraph1,"scaleY2");
        LAxisSetRange(pwgtx, y_min, y_max);
        break;
    case d_autoctrl1_MOLDSET_ECAT_CURVE_MINFLAG3:
    case d_autoctrl1_MOLDSET_ECAT_CURVE_MINSET3:
    case d_autoctrl1_MOLDSET_ECAT_CURVE_MAXFLAG3:
    case d_autoctrl1_MOLDSET_ECAT_CURVE_MAXSET3:
        y_min = pow(-1,VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MINFLAG3)) * VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MINSET3);
        y_max = pow(-1,VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MAXFLAG3)) * VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MAXSET3);
        pwgtx = LightGraphGetCurve(servo_curve_pg.lgraph1,"scaleY3");
        LAxisSetRange(pwgtx, y_min, y_max);
        break;
    case d_autoctrl1_MOLDSET_ECAT_CURVE_MINFLAG4:
    case d_autoctrl1_MOLDSET_ECAT_CURVE_MINSET4:
    case d_autoctrl1_MOLDSET_ECAT_CURVE_MAXFLAG4:
    case d_autoctrl1_MOLDSET_ECAT_CURVE_MAXSET4:
        y_min = pow(-1,VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MINFLAG4)) * VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MINSET4);
        y_max = pow(-1,VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MAXFLAG4)) * VarAdrToInt(d_autoctrl1_MOLDSET_ECAT_CURVE_MAXSET4);
        pwgtx = LightGraphGetCurve(servo_curve_pg.lgraph1,"scaleY4");
        LAxisSetRange(pwgtx, y_min, y_max);
        break;
    default:
        break;
    }

    if(pwgt == ppg->btstart)
    {
        isstart = !isstart;

        if(isstart)
        {
            ecatcurve_num = 0;
            ButtonSetText(pwgt,GetTextTran(TEXT_STOP));
            LightGraphClear(servo_curve_pg.lgraph1);

            DataRequestOrder_ECAT();
            CurvesStart_New(&gcmdecat,gcmd2);//自定义协议数据请求
        }
        else
        {
            ButtonSetText(pwgt,GetTextTran(TEXT_CLAMP_START));

            CurvesStopEx(G_ECAT);//关闭自定义的协议请求
        }
    }

    if(pwgt == ppg->btleftA)
    {
        LightGraphSetTracerMove(servo_curve_pg.lgraph1,"A",-1);
    }
    else if(pwgt == ppg->btrightA)
    {
        LightGraphSetTracerMove(servo_curve_pg.lgraph1,"A",1);
    }
    else if(pwgt == ppg->btleftB)
    {
        LightGraphSetTracerMove(servo_curve_pg.lgraph1,"B",-1);
    }
    else if(pwgt == ppg->btrightB)
    {
        LightGraphSetTracerMove(servo_curve_pg.lgraph1,"B",1);
    }

    return 0;
}
