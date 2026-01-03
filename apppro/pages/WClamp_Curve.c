#include "WClamp_Curve.h"
#include "typedef.h"
#include "pagefram.h"
#include "curvesctl.h"
#include "dropwgt.h"
#include "database.h"
#include "oprintf.h"		
#include "privdef.h"
#include "stdio.h"
#include "servo.h"
#include "editwgt.h"
#include "keymod.h"
#include "key.h"

DEFINE_CLAMP_CURVE_MAP

CONNECT_CLAMP_CURVE_EVENT

#define     CURVE_SYSPRS(curveid,buff,size)        joinStrAndNum("sysprs",curveid+1,buff,size)
#define     CURVE_SPD(curveid,buff,size)           joinStrAndNum("spd",curveid+1,buff,size)

#define     HISTORY_CURVE             TMPVAL(100) //历史曲线选择
#define     CLAMP_CURVE_NUM           2           //当前曲线+历史曲线总条数

#define     A_CLOSEPOS_VAL             TMPSTR(1)   //A游标指示关模位置值
#define     A_CLSSYSPRS_VAL            TMPSTR(2)   //A游标指示系统压力值
#define     A_CLOSESPD_VAL             TMPSTR(4)   //A游标指示关模速度值
#define     B_CLOSEPOS_VAL             TMPSTR(6)   //B游标指示关模位置值
#define     B_CLSSYSPRS_VAL            TMPSTR(7)   //B游标指示系统压力值
#define     B_CLOSESPD_VAL             TMPSTR(9)   //B游标指示关模速度值
#define     A_OPENPOS_VAL             TMPSTR(11)   //A游标指示开模位置值
#define     A_OPNSYSPRS_VAL           TMPSTR(12)   //A游标指示系统压力值
#define     A_OPENSPD_VAL             TMPSTR(14)   //A游标指示开模速度值
#define     B_OPENPOS_VAL             TMPSTR(16)   //B游标指示开模位置值
#define     B_OPNSYSPRS_VAL           TMPSTR(17)   //B游标指示系统压力值
#define     B_OPENSPD_VAL             TMPSTR(19)   //B游标指示开模速度值

static BOOL tracer_status = FALSE;//是否在移动标线状态
//曲线采用自定义发送协议曲线请求
static PROT_DATREC_REQ_PKT gcmdclose;
static DATSET_IND gcmd2[4] ;			//协议数据2
static BOOL clscurve_clear = TRUE;
static BOOL clean_first_clscurve = FALSE;//交替画曲线，画之前先清空
static UI16 close_curveid = 0;//曲线编号，用于显示历史曲线
static UI16 current_closecurveid = 0;//当前曲线id，与close_curveid差1
static BOOL is_cls_recivedata = FALSE;//20191031.cyx 解决曲线画到一半切出画面下次画接着上次继续画的问题

static PROT_DATREC_REQ_PKT gcmdopen;
static BOOL opencurve_clear = TRUE;
static BOOL clean_first_opncurve = FALSE;//交替画曲线，画之前先清空
static UI16 open_curveid = 0;//曲线编号，用于显示历史曲线
static UI16 current_opencurveid = 0;//当前曲线id，与open_curveid差1
static BOOL is_opn_recivedata = FALSE;//20191031.cyx 解决曲线画到一半切出画面下次画接着上次继续画的问题

#define CURVE_NUM 2 //曲线条数
#define TMP_CURVE1_SEL      TMPVAL(10) //曲线1选择
#define TMP_CURVE2_SEL      TMPVAL(11) //曲线2选择
static UI16 cb_sel[CURVE_NUM] = {1,1};//曲线选择

#define TMP_CURVE_X_MIN     TMPVAL(20) //曲线x轴最小值
#define TMP_CURVE_X_MAX     TMPVAL(21) //曲线x轴最大值
static double curve_x_min = 0.0;
static double curve_x_max = 0.0;

static void SetCurveColor(PWGT pwgt, int value_int)
{
    OBJSetValueByAttrName((POBJ)pwgt,"color",value_int,0,NULL,ATTR_UINT);
}

static void TraceDataShow_Close(PLIGHTGRAPH_WGT plot ,int tracerid)
{
    double closepos,sysprs, closespd;
    PWGT pwgt;
    LDPoint point;
    char buff[20];
    int curve_id;

//    if(VarAdrToInt(HISTORY_CURVE))
//    {
//        curve_id = close_curveid;
//    }
//    else
    {
        curve_id = current_closecurveid;
    }

    if(tracerid == 1)//A游标
    {
        pwgt = LightGraphGetCurve((PWGT)plot,CURVE_SYSPRS(curve_id,buff,20));
        point = LCurveTracer(pwgt,tracerid);
        closepos = point.key;
        snprintf(buff,sizeof(buff)-1,"%.1f",closepos);
        VarAdrSetStr(A_CLOSEPOS_VAL, buff);
        sysprs = point.value;
        snprintf(buff,sizeof(buff)-1,"%.1f",sysprs);
        VarAdrSetStr(A_CLSSYSPRS_VAL, buff);

        pwgt = LightGraphGetCurve((PWGT)plot,CURVE_SPD(curve_id,buff,20));
        point = LCurveTracer(pwgt,tracerid);
        closespd = point.value;
        snprintf(buff,sizeof(buff)-1,"%.1f",closespd);
        VarAdrSetStr(A_CLOSESPD_VAL, buff);
    }
    else if(tracerid == 2)//B游标
    {
        pwgt = LightGraphGetCurve((PWGT)plot,CURVE_SYSPRS(curve_id,buff,20));
        point = LCurveTracer(pwgt,tracerid);
        closepos = point.key;
        snprintf(buff,sizeof(buff)-1,"%.1f",closepos);
        VarAdrSetStr(B_CLOSEPOS_VAL, buff);
        sysprs = point.value;
        snprintf(buff,sizeof(buff)-1,"%.1f",sysprs);
        VarAdrSetStr(B_CLSSYSPRS_VAL, buff);

        pwgt = LightGraphGetCurve((PWGT)plot,CURVE_SPD(curve_id,buff,20));
        point = LCurveTracer(pwgt,tracerid);
        closespd = point.value;
        snprintf(buff,sizeof(buff)-1,"%.1f",closespd);
        VarAdrSetStr(B_CLOSESPD_VAL, buff);
    }
}

static void TraceDataShow_Open(PLIGHTGRAPH_WGT plot ,int tracerid)
{
    double openpos,sysprs, openspd;
    PWGT pwgt;
    LDPoint point;
    char buff[20];
    int curve_id;

//    if(VarAdrToInt(HISTORY_CURVE))
//    {
//        curve_id = open_curveid;
//    }
//    else
    {
        curve_id = current_opencurveid;
    }

    if(tracerid == 1)//A游标
    {
        pwgt = LightGraphGetCurve((PWGT)plot,CURVE_SYSPRS(curve_id,buff,20));
        point = LCurveTracer(pwgt,tracerid);
        openpos = point.key;
        snprintf(buff,sizeof(buff)-1,"%.1f",openpos);
        VarAdrSetStr(A_OPENPOS_VAL, buff);
        sysprs = point.value;
        snprintf(buff,sizeof(buff)-1,"%.1f",sysprs);
        VarAdrSetStr(A_OPNSYSPRS_VAL, buff);

        pwgt = LightGraphGetCurve((PWGT)plot,CURVE_SPD(curve_id,buff,20));
        point = LCurveTracer(pwgt,tracerid);
        openspd = point.value;
        snprintf(buff,sizeof(buff)-1,"%.1f",openspd);
        VarAdrSetStr(A_OPENSPD_VAL, buff);
    }
    else if(tracerid == 2)//B游标
    {
        pwgt = LightGraphGetCurve((PWGT)plot,CURVE_SYSPRS(curve_id,buff,20));
        point = LCurveTracer(pwgt,tracerid);
        openpos = point.key;
        snprintf(buff,sizeof(buff)-1,"%.1f",openpos);
        VarAdrSetStr(B_OPENPOS_VAL, buff);
        sysprs = point.value;
        snprintf(buff,sizeof(buff)-1,"%.1f",sysprs);
        VarAdrSetStr(B_OPNSYSPRS_VAL, buff);

        pwgt = LightGraphGetCurve((PWGT)plot,CURVE_SPD(curve_id,buff,20));
        point = LCurveTracer(pwgt,tracerid);
        openspd = point.value;
        snprintf(buff,sizeof(buff)-1,"%.1f",openspd);
        VarAdrSetStr(B_OPENSPD_VAL, buff);
    }
}

//清除要画的曲线
static void ClearCloseDrawCurve()
{
    PWGT pwgt;
    char buff[20];

    if(clean_first_clscurve)
    {
        pwgt = LightGraphGetCurve(clamp_curve_pg.lgraph1,CURVE_SYSPRS(close_curveid,buff,20));
        LCurveClear(pwgt);
        pwgt = LightGraphGetCurve(clamp_curve_pg.lgraph1,CURVE_SPD(close_curveid,buff,20));
        LCurveClear(pwgt);
    }
}

//清除要画的曲线
static void ClearOpenDrawCurve()
{
    PWGT pwgt;
    char buff[20];

    if(clean_first_opncurve)
    {
        pwgt = LightGraphGetCurve(clamp_curve_pg.lgraph2,CURVE_SYSPRS(open_curveid,buff,20));
        LCurveClear(pwgt);
        pwgt = LightGraphGetCurve(clamp_curve_pg.lgraph2,CURVE_SPD(open_curveid,buff,20));
        LCurveClear(pwgt);
    }
}

//显示历史曲线
static void ShowCloseHistoryCurve()
{
    PWGT pwgt[CURVE_NUM];
    char buff[20];
    int i,j;

    for(i = 0;i < CLAMP_CURVE_NUM;i++)
    {
        pwgt[0] = LightGraphGetCurve(clamp_curve_pg.lgraph1,CURVE_SYSPRS(i,buff,20));
        pwgt[1] = LightGraphGetCurve(clamp_curve_pg.lgraph1,CURVE_SPD(i,buff,20));
        if(VarAdrToInt(HISTORY_CURVE))
        {
            if(current_closecurveid == i)
            {
                SetCurveColor(pwgt[0],0x00ffff);
                SetCurveColor(pwgt[1],0x0000ff);
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
            if(current_closecurveid != i)
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

//显示历史曲线
static void ShowOpenHistoryCurve()
{
    PWGT pwgt[CURVE_NUM];
    char buff[20];
    int i,j;

    for(i = 0;i < CLAMP_CURVE_NUM;i++)
    {
        pwgt[0] = LightGraphGetCurve(clamp_curve_pg.lgraph2,CURVE_SYSPRS(i,buff,20));
        pwgt[1] = LightGraphGetCurve(clamp_curve_pg.lgraph2,CURVE_SPD(i,buff,20));
        if(VarAdrToInt(HISTORY_CURVE))
        {
            if(current_opencurveid == i)
            {
                SetCurveColor(pwgt[0],0x00ffff);
                SetCurveColor(pwgt[1],0x0000ff);
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
            if(current_opencurveid != i)
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
    pwgt = LightGraphGetCurve(clamp_curve_pg.lgraph1,"scaleX");
    if(VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_2))
    {
        LAxisSetRange(pwgt,0,VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_2)/10.0);
    }
    else
    {
        LAxisSetRange(pwgt,0,500);
    }
    pwgt = LightGraphGetCurve(clamp_curve_pg.lgraph1,"scaleY1");
    LAxisSetRange(pwgt,0,140);//初始0-140
    pwgt = LightGraphGetCurve(clamp_curve_pg.lgraph1,"scaleY2");
    LAxisSetRange(pwgt,0,100);//初始0-100
    LightGraphSaveRange(clamp_curve_pg.lgraph1);

    pwgt = LightGraphGetCurve(clamp_curve_pg.lgraph2,"scaleX");
    if(VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_2))
    {
        LAxisSetRange(pwgt,0,VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_2)/10.0);
    }
    else
    {
        LAxisSetRange(pwgt,0,500);
    }
    pwgt = LightGraphGetCurve(clamp_curve_pg.lgraph2,"scaleY1");
    LAxisSetRange(pwgt,0,140);//初始0-140
    pwgt = LightGraphGetCurve(clamp_curve_pg.lgraph2,"scaleY2");
    LAxisSetRange(pwgt,0,100);//初始0-100
    LightGraphSaveRange(clamp_curve_pg.lgraph2);
}

//清除曲线
static void ClearLigthGraph()
{
    CurvesClear_New(G_CLAMPCLOSE);
    LightGraphClear(clamp_curve_pg.lgraph1);
    close_curveid = 0;//曲线编号，用于显示历史曲线
    current_closecurveid = 0;
    clean_first_clscurve = FALSE;
    LightGraphSetTouchMode(clamp_curve_pg.lgraph1,MODE_FIXED);

    CurvesClear_New(G_CLAMPOPEN);
    LightGraphClear(clamp_curve_pg.lgraph2);
    open_curveid = 0;//曲线编号，用于显示历史曲线
    current_opencurveid = 0;
    clean_first_opncurve = FALSE;
    LightGraphSetTouchMode(clamp_curve_pg.lgraph2,MODE_FIXED);

    VarAdrSetInt(HISTORY_CURVE,0);
    ShowCloseHistoryCurve();//20230516 chj 修复在旋转历史曲线后，再选择复位，再做曲线，曲线颜色不正确问题，
    ShowOpenHistoryCurve();
    curve_x_min = 0.0;
    curve_x_max = 0.0;
    VarAdrSetInt(TMP_CURVE_X_MIN, 0);
    VarAdrSetInt(TMP_CURVE_X_MAX, 0);
    InitRange();

    //20230518 chj 曲线范围修改后，是否固定
    LightGraphSetAutoAdapt(clamp_curve_pg.lgraph1,TRUE);
    LightGraphSetAutoAdapt(clamp_curve_pg.lgraph2,TRUE);

}

//设置游标状态
static void SetTracerState(BOOL status)
{
    tracer_status = status;

    if(status)
    {
        WGTSetEnable(clamp_curve_pg.lgraph1, TRUE);
        WGTSetEnable(clamp_curve_pg.lgraph2, TRUE);
        WGTSetVisible(clamp_curve_pg.btmoving1, TRUE);
        WGTSetVisible(clamp_curve_pg.btmoving2, FALSE);
        WGTSetVisible(clamp_curve_pg.subpage1, TRUE);
        WGTSetVisible(clamp_curve_pg.subpage2, TRUE);
//20220805 dyl 非全键盘不显示游标移动提示信息
#ifdef PANEL_NOKEYBOARD//G15:No_KeyBoard

#else
        WGTSetVisible(clamp_curve_pg.stinfo, TRUE);
#endif
        LightGraphSetTouchMode(clamp_curve_pg.lgraph1,MODE_TRACER);
        LightGraphSetTouchMode(clamp_curve_pg.lgraph2,MODE_TRACER);
        PageWgtSetFocus(clamp_curve_pg.lgraph1);//模拟按下移动标线开始移动
        SendKey(_ENTER);
    }
    else
    {
        WGTSetEnable(clamp_curve_pg.lgraph1, FALSE);
        WGTSetEnable(clamp_curve_pg.lgraph2, FALSE);
        WGTSetVisible(clamp_curve_pg.btmoving1, FALSE);
        WGTSetVisible(clamp_curve_pg.btmoving2, FALSE);
        WGTSetVisible(clamp_curve_pg.subpage1, FALSE);
        WGTSetVisible(clamp_curve_pg.subpage2, FALSE);
//20220805 dyl 非全键盘不显示游标移动提示信息
#ifdef PANEL_NOKEYBOARD//G15:No_KeyBoard

#else
        WGTSetVisible(clamp_curve_pg.stinfo, FALSE);
#endif
        LightGraphSetTouchMode(clamp_curve_pg.lgraph1,MODE_FIXED);
        LightGraphSetTouchMode(clamp_curve_pg.lgraph2,MODE_FIXED);
    }
}

static void ReciveData_Close(PGRAPH_WGT pwgtx,PPROT_DATREC_RSP_PKT ppkt)
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
        char buff[20];
        if(dataCount == 0 || (PanelCurPage() != PanelFindPage(clamp_curve_pg_name)))//20191031.cyx 只在本页面绘制
            return;

        is_cls_recivedata = TRUE;//20191031.cyx
        if(tracer_status)
        {
            SetTracerState(FALSE);
            PageWgtSetFocus(clamp_curve_pg.btmove);//模拟按下移动标线开始移动
        }

        if(clscurve_clear)
        {
            clscurve_clear = FALSE;
            ClearCloseDrawCurve();
        }

//        if(VarAdrToInt(HISTORY_CURVE))
//        {
//            VarAdrSetInt(HISTORY_CURVE,0);
//        }

        if(current_closecurveid != close_curveid)
        {
            current_closecurveid = close_curveid;
            ShowCloseHistoryCurve();
        }

        for(i=0;i<rec_len/dataCount;i++)
        {
            tmppoint.x = pdata[i*dataCount]/10.0;//0x07160003 开模位置

            tmppoint.y = pdata[i*dataCount+1]/10.;//系统压力 1位小数
            pwgt = LightGraphGetCurve(clamp_curve_pg.lgraph1,CURVE_SYSPRS(close_curveid,buff,20));
            LCurveAddPoint(pwgt,tmppoint.x,tmppoint.y);

            tmppoint.y = pdata[i*dataCount+3]/10.0;//0x20160010 开合模速度
            pwgt = LightGraphGetCurve(clamp_curve_pg.lgraph1,CURVE_SPD(close_curveid,buff,20));
            LCurveAddPoint(pwgt,tmppoint.x,tmppoint.y);
        }

        if(rec_len == CURVE_TRANSEND)//结束
        {
            is_cls_recivedata = FALSE;
            clscurve_clear = TRUE;
            close_curveid++;
            if(close_curveid >= CLAMP_CURVE_NUM)
            {
                close_curveid = 0;
                clean_first_clscurve = TRUE;//开始清除曲线
            }
        }
    }
}

static void ReciveData_Open(PGRAPH_WGT pwgtx,PPROT_DATREC_RSP_PKT ppkt)
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
        char buff[20];
        if(dataCount == 0 || (PanelCurPage() != PanelFindPage(clamp_curve_pg_name)))//20191031.cyx 只在本页面绘制
            return;

        is_opn_recivedata = TRUE;//20191031.cyx
        if(tracer_status)
        {
            SetTracerState(FALSE);
            PageWgtSetFocus(clamp_curve_pg.btmove);//模拟按下移动标线开始移动
        }

        if(opencurve_clear)
        {
            opencurve_clear = FALSE;
            ClearOpenDrawCurve();
        }

//        if(VarAdrToInt(HISTORY_CURVE))
//        {
//            VarAdrSetInt(HISTORY_CURVE,0);
//        }

        if(current_opencurveid != open_curveid)
        {
            current_opencurveid = open_curveid;
            ShowOpenHistoryCurve();
        }

        for(i=0;i<rec_len/dataCount;i++)
        {
            tmppoint.x = pdata[i*dataCount]/10.0;//0x07160003 开模位置

            tmppoint.y = pdata[i*dataCount+1]/10.;//系统压力 1位小数
            pwgt = LightGraphGetCurve(clamp_curve_pg.lgraph2,CURVE_SYSPRS(open_curveid,buff,20));
            LCurveAddPoint(pwgt,tmppoint.x,tmppoint.y);

            tmppoint.y = pdata[i*dataCount+3]/10.0;//0x20160010 开合模速度
            pwgt = LightGraphGetCurve(clamp_curve_pg.lgraph2,CURVE_SPD(open_curveid,buff,20));
            LCurveAddPoint(pwgt,tmppoint.x,tmppoint.y);
        }

        if(rec_len == CURVE_TRANSEND)//结束
        {
            is_opn_recivedata = FALSE;
            opencurve_clear = TRUE;
            open_curveid++;
            if(open_curveid >= CLAMP_CURVE_NUM)
            {
                open_curveid = 0;
                clean_first_opncurve = TRUE;//开始清除曲线
            }
        }
    }
}

static void DataRequestOrder_Clamp()
{
    gcmd2[0].part_id = 0x0710;
    gcmd2[0].sub_ind = 0x06;
    gcmd2[0].offset = 0x0003;//0x07160003 开模位置
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
    gcmd2[3].part_id = 0x2010;
    gcmd2[3].sub_ind = 0x06;
    gcmd2[3].offset = 0x0010;//0x20160010 开合模速度
}

static void CtlConnect()
{
}
static int Clamp_CurveOnInit(Clamp_Curve_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    BindGraphCB(G_CLAMPCLOSE,ReciveData_Close);
    LightGraphSetTracerCB(ppg->lgraph1,TraceDataShow_Close);
    gcmdclose.rec_id = G_CLAMPCLOSE;
    gcmdclose.ind_num = ARRAY_NUM(gcmd2);
    gcmdclose.rec_period = VarAdrToInt(CURVE_CHECKTIME);//ms
    gcmdclose.rec_total_ms = 0;
    gcmdclose.trigger_id = 0x2011;//触发条件 关模
    LightGraphSetTracerVertical(ppg->lgraph1,TRUE);//设置游标为竖直

    BindGraphCB(G_CLAMPOPEN,ReciveData_Open);
    LightGraphSetTracerCB(ppg->lgraph2,TraceDataShow_Open);
    gcmdopen.rec_id = G_CLAMPOPEN;
    gcmdopen.ind_num = ARRAY_NUM(gcmd2);
    gcmdopen.rec_period = VarAdrToInt(CURVE_CHECKTIME);//ms
    gcmdopen.rec_total_ms = 0;
    gcmdopen.trigger_id = 0x2013;//触发条件 开模
    LightGraphSetTracerVertical(ppg->lgraph2,TRUE);//设置游标为竖直

    InitRange();

    return 0;
}
static int Clamp_CurveOnShow(Clamp_Curve_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    int i;

    DataRequestOrder_Clamp();//请求数据排序
    CurvesStart_New(&gcmdclose,gcmd2);//自定义协议数据请求
    CurvesStart_New(&gcmdopen,gcmd2);//自定义协议数据请求

    for(i = 0; i < CURVE_NUM; i++)
    {
        VarAdrSetInt(TMP_CURVE1_SEL + i, cb_sel[i]);
    }
    VarAdrSetInt(TMP_CURVE_X_MIN, curve_x_min);
    VarAdrSetInt(TMP_CURVE_X_MAX, curve_x_max);
    if((curve_x_max == 0) && VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_2))
    {
        LAxisSetRange(LightGraphGetCurve(clamp_curve_pg.lgraph1,"scaleX"),0,VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_2)/10.0);
        LAxisSetRange(LightGraphGetCurve(clamp_curve_pg.lgraph2,"scaleX"),0,VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_2)/10.0);
    }

    VarAdrSetInt(HISTORY_CURVE,0);
    ShowCloseHistoryCurve();
    ShowOpenHistoryCurve();

    SetTracerState(FALSE);

    //20230518 chj 曲线范围修改后，是否固定
    if(VarAdrToInt(TMP_CURVE_X_MAX))
    {
        LightGraphSetAutoAdapt(clamp_curve_pg.lgraph1,FALSE);
        LightGraphSetAutoAdapt(clamp_curve_pg.lgraph2,FALSE);
    }
    else {
        LightGraphSetAutoAdapt(clamp_curve_pg.lgraph1,TRUE);
        LightGraphSetAutoAdapt(clamp_curve_pg.lgraph2,TRUE);
    }

    if(VarAdrToInt(FL_CLSOPEN_MODE) == 2)
        NaviSetIndex(PanelCurPage(), 1);
    else if((VarAdrToInt(FL_CLSOPEN_MODE) == 3) || (VarAdrToInt(FL_CLSOPEN_MODE) == 4))
        NaviSetIndex(PanelCurPage(), 2);
    else
        NaviSetIndex(PanelCurPage(), 0);

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
    WGTSetVisible(clamp_curve_pg.stinfo, FALSE);
#endif

    return 0;
}
static int Clamp_CurveOnHide(Clamp_Curve_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    CurvesStopEx(G_CLAMPCLOSE);//关闭自定义的协议请求
    if(is_cls_recivedata)//20191031.cyx
    {
        is_cls_recivedata = FALSE;
        clscurve_clear = TRUE;
        close_curveid++;
        if(close_curveid >= CLAMP_CURVE_NUM)
        {
            close_curveid = 0;
            clean_first_clscurve = TRUE;//开始清除曲线
        }
    }

    CurvesStopEx(G_CLAMPOPEN);//关闭自定义的协议请求
    if(is_opn_recivedata)//20191031.cyx
    {
        is_opn_recivedata = FALSE;
        opencurve_clear = TRUE;
        open_curveid++;
        if(open_curveid >= CLAMP_CURVE_NUM)
        {
            open_curveid = 0;
            clean_first_opncurve = TRUE;//开始清除曲线
        }
    }

    return 0;
}
static int Clamp_CurveOnUpdate(Clamp_Curve_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
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

    static int clslightgraph_select = -1;
    static int opnlightgraph_select = -1;
    if(clslightgraph_select != LightGraphIsSelect(clamp_curve_pg.lgraph1))
    {
        clslightgraph_select = LightGraphIsSelect(clamp_curve_pg.lgraph1);

        if(!clslightgraph_select && tracer_status)
        {
            WGTSetVisible(clamp_curve_pg.btmoving1, FALSE);
            WGTSetVisible(clamp_curve_pg.btmoving2, TRUE);
            PageWgtSetFocus(clamp_curve_pg.lgraph2);//模拟按下移动标线开始移动
            SendKey(_ENTER);
        }
    }

    if(opnlightgraph_select != LightGraphIsSelect(clamp_curve_pg.lgraph2))
    {
        opnlightgraph_select = LightGraphIsSelect(clamp_curve_pg.lgraph2);

        if(!opnlightgraph_select && tracer_status)
        {
            SetTracerState(FALSE);
            PageWgtSetFocus(clamp_curve_pg.btmove);//模拟按下移动标线开始移动
        }
    }

    return 0;
}
static int Clamp_CurveOnChange(Clamp_Curve_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    int i;

    if(pwgt == ppg->dropdown1)//曲线显示
    {
        ShowCloseHistoryCurve();
        ShowOpenHistoryCurve();
        SetTracerState(FALSE);//20220727 dyl
    }
    else if(pwgt == ppg->btmove)
    {
        SetTracerState(TRUE);
    }
    else if(pwgt == ppg->btclear)
    {
        ClearLigthGraph();
        SetTracerState(FALSE);//20220727 dyl
    }
    else if(pwgt == ppg->checkbox1 || pwgt == ppg->checkbox2)//曲线选择
    {
        for(i = 0; i < CURVE_NUM; i++)
        {
            cb_sel[i] = VarAdrToInt(TMP_CURVE1_SEL + i);
        }
        ShowCloseHistoryCurve();
        ShowOpenHistoryCurve();
        SetTracerState(FALSE);//20220727 dyl
    }
    else if(pwgt == ppg->edxmin || pwgt == ppg->edxmax)
    {
        curve_x_min = VarAdrToInt(TMP_CURVE_X_MIN);
        curve_x_max = VarAdrToInt(TMP_CURVE_X_MAX);
        if(curve_x_max == 0)
        {
            //20230518 chj 曲线范围修改后，是否固定
            LightGraphSetAutoAdapt(clamp_curve_pg.lgraph1,TRUE);
            LightGraphSetAutoAdapt(clamp_curve_pg.lgraph2,TRUE);

            if(VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_2))
            {
                LAxisSetRange(LightGraphGetCurve(clamp_curve_pg.lgraph1,"scaleX"),0,VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_2)/10.0);
                LAxisSetRange(LightGraphGetCurve(clamp_curve_pg.lgraph2,"scaleX"),0,VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_2)/10.0);
            }
            else
            {
                LAxisSetRange(LightGraphGetCurve(clamp_curve_pg.lgraph1,"scaleX"),0,500);
                LAxisSetRange(LightGraphGetCurve(clamp_curve_pg.lgraph2,"scaleX"),0,500);
            }
        }
        else
        {
            //20230518 chj 曲线范围修改后，是否固定
            LightGraphSetAutoAdapt(clamp_curve_pg.lgraph1,FALSE);
            LightGraphSetAutoAdapt(clamp_curve_pg.lgraph2,FALSE);

            LAxisSetRange(LightGraphGetCurve(clamp_curve_pg.lgraph1,"scaleX"),VarAdrToInt(TMP_CURVE_X_MIN)/10.0,VarAdrToInt(TMP_CURVE_X_MAX)/10.0);
            LAxisSetRange(LightGraphGetCurve(clamp_curve_pg.lgraph2,"scaleX"),VarAdrToInt(TMP_CURVE_X_MIN)/10.0,VarAdrToInt(TMP_CURVE_X_MAX)/10.0);
        }
        SetTracerState(FALSE);//20220727 dyl
    }

	return 0;
}
