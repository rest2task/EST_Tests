#ifndef _CURVESCTL_h
#define _CURVESCTL_h
#include "typedef.h"
#include "graphwgt.h"
#include "protcmd.h"
#include "mod.h"
#include "math.h"
#include "lightgraphwgt.h"
#ifdef __cplusplus
extern "C"
{ 
#endif
#define CURVE_TRANSEND	0

	void DataRecRspRx(PPROT_DATREC_RSP_PKT ppkt);
	void AttachGraph(PGRAPH_WGT pwgt);

	void CurvesInital();
	BOOL CurvesStart(PGRAPH_WGT pwgt);//(WM_HWIN hwin,PGRAPH_WGT pwgt);
	BOOL CurvesStartEx(PPROT_DATREC_REQ_PKT cmd1,PDATSET_IND cmd2);
	BOOL CurvesStop(PGRAPH_WGT pwgt);
	BOOL CurvesStopEx(UI16 rec_id);
	BOOL CurvesClose(PGRAPH_WGT pwgt);
	BOOL CurvesClear(PGRAPH_WGT pwgt);
	BOOL GraphClear(PGRAPH_WGT pwgt);
	BOOL CurvesMonitor(PGRAPH_WGT pwgt);

    void TempCurveGetData();//20190618.cyx 温度曲线取数据
    void TempCurveGetData2();//20250312 jhh 增加温度曲线2画面 温度曲线2取数据
    double TempCurveGetPointX(int curvenum,int pointnum);
    double TempCurveGetPointY(int curvenum,int pointnum);
    double TempCurve2GetPointX(int curvenum,int pointnum);//20250312 jhh 增加温度曲线2画面
    double TempCurve2GetPointY(int curvenum,int pointnum);
    void SetTempCurveRepaint(BOOL isRepaint);//设置温度曲线重绘
    BOOL GetTempCurveRepaint();//获得温度曲线重绘状态
    void SetTempCurve2Repaint(BOOL isRepaint);//设置温度曲线重绘//20250312 jhh 增加温度曲线2画面
    BOOL GetTempCurve2Repaint();//获得温度曲线重绘状态
    void SetTempCurveNum(UI32 num);//设置温度曲线点数个数
    UI32 GetTempCurveNum();//获得温度曲线点数个数
    void SetTempCurve2Num(UI32 num);//设置温度曲线2点数个数//20250312 jhh 增加温度曲线2画面
    UI32 GetTempCurve2Num();//获得温度曲线2点数个数
    void DriveTempCurveGetData();//20190618.cyx 驱动器温度曲线取数据
    double DriveTempCurveGetPointX(int curvenum,int pointnum);
    double DriveTempCurveGetPointY(int curvenum,int pointnum);
    void SetDriveTempCurveRepaint(BOOL isRepaint);//设置驱动器曲线重绘
    BOOL GetDriveTempCurveRepaint();//获得驱动器曲线重绘状态
    void SetDriveTempCurveNum(UI32 num);//设置驱动器曲线点数个数
    UI32 GetDriveTempCurveNum();//获得驱动器曲线点数个数

	typedef void (*CURVE_DATA_RX_CB)(PGRAPH_WGT pwgt,PPROT_DATREC_RSP_PKT ppkt);
	CURVE_DATA_RX_CB CurvesSetCbSet(UI16 set_id, CURVE_DATA_RX_CB rx_cb);//针对协议数据的回调函数设置

    void BindGraphCB(int id,CURVE_DATA_RX_CB rx_cb);
    //全电新曲线控件新增
    #define     G_CAVITYCURVES  24 //模腔曲线
    #define     G_CLAMPOPEN     25 //开模曲线
    #define     G_CLAMPCLOSE    26 //关模曲线
    #define     G_CHARGE        27 //储料曲线
    #define     G_INJECT        28 //射出曲线时间模式
    #define     G_EJECT         29 //托模曲线
    #define     G_INJECT_POS    30 //射出曲线位置模式
    #define     G_ECAT          31 //伺服曲线

    //曲线的触摸方式 FIXED:0, ZOOMIN:1, ZOOMOUT:2, MOVE:3, TRACER:4, DRAW:5
    typedef enum enLIGHTGRAPH_TOUCH_MODE{
        MODE_FIXED = 0, //固定
        MODE_ZOOMIN,    //放大
        MODE_ZOOMOUT,   //缩小
        MODE_MOVE,      //移动
        MODE_TRACER,    //游标
        MODE_DRAW       //绘制
    }LIGHTGRAPH_TOUCH_MODE;

    BOOL CurvesStart_New(PPROT_DATREC_REQ_PKT cmd1,PDATSET_IND cmd2);
    BOOL CurvesClear_New(UI16 rec_id);


#ifdef __cplusplus
}
#endif

#endif



