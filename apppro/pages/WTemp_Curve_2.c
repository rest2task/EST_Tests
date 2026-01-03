#include "WTemp_Curve_2.h"
#include "typedef.h"
#include "pagefram.h"
#include "vardb.h"
#include "mod.h"
#include "time.h"
#include "cgui.h"
#include "graphwgt.h"
#include "stdio.h"
#include "database.h"
#include "curvesctl.h"
#include "editwgt.h"
#include "oprintf.h"

DEFINE_TEMP_CURVE_2_MAP

CONNECT_TEMP_CURVE_2_EVENT

#define CURVE_NUM     9 // total curve number
#define MAX_TEMPCURVE_POINT  2880
#define TMP_SELECT    TMPVAL(88) //温度选择段数

static UI32 curve_num = 0;

static void SegSelect()
{
    UI16 seg;
    UI16 bit = 0;
    VarAdrSetInt(TMP_SELECT, 0);

    seg = VarAdrToInt(d_TempCtrlComm2_MACHSET_DATEMP_CH_NUM);

    switch (seg) {
    case 5:
        setone(bit, 0);
        VarAdrSetInt(p_PP_MACH2_TEMPCURVE_2_SELECT5, 0);
        VarAdrSetInt(p_PP_MACH2_TEMPCURVE_2_SELECT6, 0);
        VarAdrSetInt(p_PP_MACH2_TEMPCURVE_2_SELECT7, 0);
        VarAdrSetInt(p_PP_MACH2_TEMPCURVE_2_SELECT8, 0);
        VarAdrSetInt(p_PP_MACH2_TEMPCURVE_2_SELECT9, 0);
        break;
    case 6:
        setone(bit, 0);
        setone(bit, 1);
        VarAdrSetInt(p_PP_MACH2_TEMPCURVE_2_SELECT6, 0);
        VarAdrSetInt(p_PP_MACH2_TEMPCURVE_2_SELECT7, 0);
        VarAdrSetInt(p_PP_MACH2_TEMPCURVE_2_SELECT8, 0);
        VarAdrSetInt(p_PP_MACH2_TEMPCURVE_2_SELECT9, 0);
        break;
    case 7:
        setone(bit, 0);
        setone(bit, 1);
        setone(bit, 2);
        VarAdrSetInt(p_PP_MACH2_TEMPCURVE_2_SELECT7, 0);
        VarAdrSetInt(p_PP_MACH2_TEMPCURVE_2_SELECT8, 0);
        VarAdrSetInt(p_PP_MACH2_TEMPCURVE_2_SELECT9, 0);
        break;
    case 8:
        setone(bit, 0);
        setone(bit, 1);
        setone(bit, 2);
        setone(bit, 3);
        VarAdrSetInt(p_PP_MACH2_TEMPCURVE_2_SELECT8, 0);
        VarAdrSetInt(p_PP_MACH2_TEMPCURVE_2_SELECT9, 0);
        break;
    case 9:
        setone(bit, 0);
        setone(bit, 1);
        setone(bit, 2);
        setone(bit, 3);
        setone(bit, 4);
        VarAdrSetInt(p_PP_MACH2_TEMPCURVE_2_SELECT9, 0);
        break;
    case 10:
        setone(bit, 0);
        setone(bit, 1);
        setone(bit, 2);
        setone(bit, 3);
        setone(bit, 4);
        setone(bit, 5);
        break;
    default:
        VarAdrSetInt(p_PP_MACH2_TEMPCURVE_2_SELECT4, 0);
        VarAdrSetInt(p_PP_MACH2_TEMPCURVE_2_SELECT5, 0);
        VarAdrSetInt(p_PP_MACH2_TEMPCURVE_2_SELECT6, 0);
        VarAdrSetInt(p_PP_MACH2_TEMPCURVE_2_SELECT7, 0);
        VarAdrSetInt(p_PP_MACH2_TEMPCURVE_2_SELECT8, 0);
        VarAdrSetInt(p_PP_MACH2_TEMPCURVE_2_SELECT9, 0);
        break;
    }

    VarAdrSetInt(TMP_SELECT, bit);
}

static void TempCurveUpdate()
{
    PGRAPH_WGT pGraph = (PGRAPH_WGT)(temp_curve_2_pg.graph1);
    char curve[10] = "";
    UI16 i, pointnum;
    FPOINT point;

    if(GetTempCurve2Repaint())
    {
        return;
    }

    for(i = 0; i < CURVE_NUM; ++i)
    {
        for(pointnum = curve_num; pointnum < GetTempCurve2Num(); ++pointnum)
        {
            sprintf(curve, "curve%d", i+1);
            point.x = pointnum * 5 / 60.00;
            point.y = TempCurve2GetPointY(i, pointnum);
            GraphAddPointByCurveName(pGraph, curve, &point, 1);
        }
    }

    curve_num = GetTempCurve2Num();
    GraphRePaint(pGraph);
}

static void TempCurvePageIn()
{
    SegSelect();
    EditSetMax(temp_curve_2_pg.xmin,VarAdrToInt(p_PP_MACH2_TEMPCURVE_2_X_MAX)-10);
    EditSetMin(temp_curve_2_pg.xmax,VarAdrToInt(p_PP_MACH2_TEMPCURVE_2_X_MIN)+10);
    EditSetMax(temp_curve_2_pg.ymin,VarAdrToInt(p_PP_MACH2_TEMPCURVE_2_Y_MAX)-10);
    EditSetMin(temp_curve_2_pg.ymax,VarAdrToInt(p_PP_MACH2_TEMPCURVE_2_Y_MIN)+10);
    GraphSetScaleStartByName(temp_curve_2_pg.graph1,"scaleX",VarAdrToInt(p_PP_MACH2_TEMPCURVE_2_X_MIN));
    GraphSetScaleEndByName(temp_curve_2_pg.graph1,"scaleX",VarAdrToInt(p_PP_MACH2_TEMPCURVE_2_X_MAX));
    GraphSetScaleStartByName(temp_curve_2_pg.graph1,"scaleY",VarAdrToInt(p_PP_MACH2_TEMPCURVE_2_Y_MIN));
    GraphSetScaleEndByName(temp_curve_2_pg.graph1,"scaleY",VarAdrToInt(p_PP_MACH2_TEMPCURVE_2_Y_MAX));
    GraphRePaint((PGRAPH_WGT)(temp_curve_2_pg.graph1));

    //画满2880个点 曲线不刷新点 只在进入画面的时候重绘曲线
    if(GetTempCurve2Repaint())
    {
        PGRAPH_WGT pGraph = (PGRAPH_WGT)(temp_curve_2_pg.graph1);
        char curve[10] = "";
        UI16 i, pointnum;
        FPOINT point;

        GraphClear(pGraph);

        for(i = 0; i < CURVE_NUM; ++i)
        {
            for(pointnum = GetTempCurve2Num(); pointnum < MAX_TEMPCURVE_POINT; ++pointnum)
            {
                sprintf(curve, "curve%d", i+1);
                point.x = (pointnum - GetTempCurve2Num()) * 5 / 60.00;
                point.y = TempCurve2GetPointY(i, pointnum);
                GraphAddPointByCurveName(pGraph, curve, &point, 1);
            }

            for(pointnum = 0; pointnum < GetTempCurve2Num(); ++pointnum)
            {
                sprintf(curve, "curve%d", i+1);
                point.x = (MAX_TEMPCURVE_POINT - GetTempCurve2Num() + pointnum) * 5 / 60.00;
                point.y = TempCurve2GetPointY(i, pointnum);
                GraphAddPointByCurveName(pGraph, curve, &point, 1);
            }
        }
        GraphRePaint(pGraph);
    }
}

static void BackToTemp(int btnid, PNAVI_BTN pbtn) //20191211.cyx 曲线画面返回特殊处理
{
    if(btnid == 6)
    {
        PanelShowPageByName(PAGE_TEMP_XX); //画面的跳转表先清空，否则不会执行改行
    }
    else
    {
        NaviBtnJumpDefault(btnid, pbtn);
    }
}

static void CtlConnect()
{
}
static int Temp_Curve_2OnInit(Temp_Curve_2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Temp_Curve_2OnShow(Temp_Curve_2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    NaviSetBtnJumpCB(BackToTemp);//设置当前页面跳转
    TempCurvePageIn();
    return 0;
}
static int Temp_Curve_2OnHide(Temp_Curve_2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    NaviSetBtnJumpCB(NaviBtnJumpDefault);//设置默认跳转
    return 0;
}
static int Temp_Curve_2OnUpdate(Temp_Curve_2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    static UI32 time_5s = 0;

    if(GetTick() - time_5s >= 5000)
    {
        if(VarAdrToInt(d_TempCtrlComm2_STATE_FL_HEATERSTATUS))
        {
            TempCurveUpdate();
        }
        time_5s = GetTick();
    }

    return 0;
}
static int Temp_Curve_2OnChange(Temp_Curve_2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 addr,value,offset;
    PATTR pattr;

    pattr = (PATTR)msg_para;

    addr = OBJAttrGetAdr(pattr);
    value = VarAdrToInt(addr);
    offset = WGTGetGOffset(pwgt);

    if(offset > 0)//20201106 解决曲线刷新延迟问题
    {
        char curve[10];
        PWGT pwgt_curve;
        sprintf(curve, "curve%d", offset);
        pwgt_curve = GraphGetCurveByName(temp_curve_2_pg.graph1, curve);
        CurveSetShowMode((PGRAPHCURVE_WGT)pwgt_curve, value);
        GraphRePaint((PGRAPH_WGT)(temp_curve_2_pg.graph1));
    }

    switch(addr)
    {
    case p_PP_MACH2_TEMPCURVE_2_X_MAX:
        EditSetMax(ppg->xmin,value-10);
        GraphSetScaleStartByName(temp_curve_2_pg.graph1,"scaleX",VarAdrToInt(p_PP_MACH2_TEMPCURVE_2_X_MIN));
        GraphSetScaleEndByName(temp_curve_2_pg.graph1,"scaleX",VarAdrToInt(p_PP_MACH2_TEMPCURVE_2_X_MAX));
        GraphRePaint((PGRAPH_WGT)(temp_curve_2_pg.graph1));
        break;
    case p_PP_MACH2_TEMPCURVE_2_X_MIN:
        EditSetMin(ppg->xmax,value+10);
        GraphSetScaleStartByName(temp_curve_2_pg.graph1,"scaleX",VarAdrToInt(p_PP_MACH2_TEMPCURVE_2_X_MIN));
        GraphSetScaleEndByName(temp_curve_2_pg.graph1,"scaleX",VarAdrToInt(p_PP_MACH2_TEMPCURVE_2_X_MAX));
        GraphRePaint((PGRAPH_WGT)(temp_curve_2_pg.graph1));
        break;
    case p_PP_MACH2_TEMPCURVE_2_Y_MAX:
        EditSetMax(ppg->ymin,value-10);
        GraphSetScaleStartByName(temp_curve_2_pg.graph1,"scaleY",VarAdrToInt(p_PP_MACH2_TEMPCURVE_2_Y_MIN));
        GraphSetScaleEndByName(temp_curve_2_pg.graph1,"scaleY",VarAdrToInt(p_PP_MACH2_TEMPCURVE_2_Y_MAX));
        GraphRePaint((PGRAPH_WGT)(temp_curve_2_pg.graph1));
        break;
    case p_PP_MACH2_TEMPCURVE_2_Y_MIN:
        EditSetMin(ppg->ymax,value+10);
        GraphSetScaleStartByName(temp_curve_2_pg.graph1,"scaleY",VarAdrToInt(p_PP_MACH2_TEMPCURVE_2_Y_MIN));
        GraphSetScaleEndByName(temp_curve_2_pg.graph1,"scaleY",VarAdrToInt(p_PP_MACH2_TEMPCURVE_2_Y_MAX));
        GraphRePaint((PGRAPH_WGT)(temp_curve_2_pg.graph1));
        break;
    default:
        break;
    }
    return 0;
}
