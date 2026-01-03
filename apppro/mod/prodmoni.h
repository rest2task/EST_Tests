#ifndef _PRODMONI_H
#define _PRODMONI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "mod.h"
#include "file.h"

#define  PRODMONI_MAX_CNT 10000
#define  PRODMONI_VIEW_COL 6
#define DATA_MAX    26
#define MAXCOUNT_COMBOXID 27
#define MAXLENGH_STR 20

typedef struct tyProdMoniItem
{
	UI16        tmCycle;                //制品时间
	UI16        tmInject;               //射出时间
	UI16        adPosi_InjectStart;     //射出起点
	UI16        adPosi_InjectCushion;   //保压起点
	UI16        adPosi_InjectHoldEnd;   //残料位置
	UI16        adPosi_Charge;          //储料位置

	UI16        tmCharge;               //储料时间
	UI16        tmCloseMold;            //关模计时
	UI16        tmCloseLow;             //关模低压计时
	UI16        tmCloseHigh;            //关模高压计时
	UI16        tmOpenMold;             //开模计时
	UI16        adPosi_OpenMold;        //开模终点
	UI16        Pres_TurnToHold;        //转保压压力
	UI16        Flow_TurnToHold;        //转保压速度
	UI16        tmInjectBack;           //射退计时
	UI16        adPosi_Injectback;      //射退终点
	UI16        tmEject;                //托模计时
	UI16        tmNozzleAdv;            //座台进计时
	UI16        tmNozzleBack;           //座台退计时

	UI16        Moni_AvgInjSpd;         //射出平均速度
	UI16        Moni_MaxInjPrs;         //射出最大压力
	UI16        Moni_MaxHoldPrs;        //保压最大压力
	UI16        Moni_MaxHoldMoveD;      //保压移动距离

	UI16        Moni_ClampMov;			//全程计时
	UI16        Moni_EjectAdvTime;      //托模进计时
	UI16        Moni_EjectRetTime;      //托模退计时

	UI16        Moni_InjSpd;			//射出速度
        UI16        Mod_cnt1;               //开模数1
        UI16        Mod_cnt2;               //开模数2
        UI32        Moni_datetime;          //当前模时间 //20211224 dyl 增加当前时间信息
        UI32        rev[3];
}ProdMoniItem, *PProdMoniItem;

//20200324
typedef struct   tyProdMoniItem32
{
    UI32        tmCycle;                //制品时间
    UI32        tmInject;               //射出时间
    UI32        adPosi_InjectStart;     //射出起点
    UI32        adPosi_InjectCushion;   //保压起点
    UI32        adPosi_InjectHoldEnd;   //残料位置
    UI32        adPosi_Charge;          //储料位置

    UI32        tmCharge;               //储料时间
    UI32        tmCloseMold;            //关模计时
    UI32        tmCloseLow;             //关模低压计时
    UI32        tmCloseHigh;            //关模高压计时
    UI32        tmOpenMold;             //开模计时
    UI32        adPosi_OpenMold;        //开模终点
    UI32        Pres_TurnToHold;        //转保压压力
    UI32        Flow_TurnToHold;        //转保压速度
    UI32        tmInjectBack;           //射退计时
    UI32        adPosi_Injectback;      //射退终点
    UI32        tmEject;                //托模计时
    UI32        tmNozzleAdv;            //座台进计时
    UI32        tmNozzleBack;           //座台退计时

    UI32        Moni_AvgInjSpd;         //射出平均速度
    UI32        Moni_MaxInjPrs;         //射出最大压力
    UI32        Moni_MaxHoldPrs;        //保压最大压力
    UI32        Moni_MaxHoldMoveD;      //保压移动距离

    UI32        Moni_ClampMov;          //全程计时
    UI32        Moni_EjectAdvTime;      //托模进计时
    UI32        Moni_EjectRetTime;      //托模退计时

    UI32        Moni_InjSpd;            //射出速度
    UI16        Mod_cnt1;               //开模数1
    UI16        Mod_cnt2;               //开模数2

    UI32        Moni_Open1Time;         //开模一段计时 //20201217
    UI32        Moni_MaxLockPrs;        //锁模最大压力
    UI32        Moni_datetime;          //当前模时间 //20211224 dyl 增加当前时间信息
    UI32        Moni_Bak[7];
}ProdMoniItem32, *PProdMoniItem32;

typedef struct tyProdMoniHead
{
	UI16 flag;
	UI16 index;
    UI16 version; // 1: add view_cols
    UI8  view_cols[PRODMONI_VIEW_COL];
    UI16 reserve[2];
}ProdMoniHead;

typedef struct tyProdMoniDB
{
	ProdMoniHead head;
	ProdMoniItem datas[PRODMONI_MAX_CNT];
}ProdMoniDB, *PProdMoniDB;

extern ProdMoniDB g_dbProdMoni;
#define g_dbProdIndex g_dbProdMoni.head.index

//20200324
typedef struct tyProdMoniDB32
{
    ProdMoniHead head;
    ProdMoniItem32 datas[PRODMONI_MAX_CNT];
}ProdMoniDB32, *PProdMoniDB32;

extern ProdMoniDB32 g_dbProdMoni32;
#define g_dbProdIndex32 g_dbProdMoni32.head.index

void ProdMoniInital();
void ProdMoniChgNo();
void ProdMoniPageIn(int pageid);
void ProdMoniChgType(int index);
void ProdMoniClearAll();

void ProductCntAvg(int dataIndex, int*Prodmoni_AMM);

/**
 * 成品数据监控
 */
void ProdMonitor();

/**
 * 保存所有成品数据
 */
void ProdMoniSave();

BOOL ExProdmoniExcelHead(FILEHD* stream, char* tem_str);
BOOL ExProdmoniExcelBody(FILEHD* stream, UI16 start, UI16 step);


#ifdef __cplusplus
}
#endif

#endif
