#ifndef PLANSHEET_H
#define PLANSHEET_H
#ifndef WIN32

#include "iotnet.h"
#include "jsonctl.h"

#define PLAN_DIR        IOT_SAVE_PATH"Plan/"

enum FILE_ID
{
    FID_MOULDTX = 0x1090,   //发送模具参数文件
    FID_MOULDRX = 0x1091,   //接收模具参数文件
    FID_CURMOULD = 0x1092,  //当前模具
    FID_PARAM = 0x1100,   //参数结构文件---OPC请求文件ID

    FID_SHEETMASK = 0xEF00,  //关于派单协议的区别掩码（针对 协议0x41和0x40）
    FID_REVSHEET = 0xEF01,  //计划单
    FID_CURSHEET = 0xEF02,  //当前计划信息
    FID_PLANHISTORY = 0xEF03,  //总的生产信息（本地所有计划单的信息）
    FID_DELETESHEET = 0xEF04,  //删除表单 //2018.3.23 csj
    FID_LASTSHEET = 0xEF05,  //上一次表单

    FID_USERREQINFO = 0xEF20,  //请求员工登入信息表
    FID_USERINFO = 0xEF21,  //员工信息表

    FID_ERRORINFO = 0xEF10, //来自服务器的错误映射表
    FID_SUPPLEMENTINFO = 0xEF11,  //总的补登信息标识
    FID_SUPPLEMENTSHEET = 0xEF12,  //单个补登信息标识

    FID_END
};

#define MOULD_FILE              PLAN_DIR"moldTmpData.dat"   //模具参数的文件名
#define MOULD_RXFILE            PLAN_DIR"moldRXData.dat"   //模具参数的文件名
#define PARAM_FILE              PLAN_DIR"param.xml"   //OPC请求文件
// 生产派单部分 csj
#define SHEET_HISTORYFILE       PLAN_DIR"PlanHistoryList" //保存所有的单信息
#define SHEET_TMPFILE           PLAN_DIR"Tmpsheet"      //从服务器接收时保存到该文件里
#define SHEET_CURFILE           PLAN_DIR"Cursheet"      //发送时，从该文件发送
#define SHEET_DELFILE           PLAN_DIR"Delsheet"      //2018.3.23 csj
#define SHEET_OLDSHEETFILE      PLAN_DIR"Oldsheet"      //上次表单
//NET_VER2
#define USER_REQFILE            PLAN_DIR"ReqFile"       //请求员工登入信息表
#define USER_INFOFILE           PLAN_DIR"InfoFile"      //员工信息表

#define SHEET_ERRORFILE             PLAN_DIR"erroinfosheet"       //错误映射表文件
#define SHEET_SUPPLEMENTFILE        PLAN_DIR"supplementsheet"     //补登总信息
#define SHEET_CURSUPPLEMENTFILE     PLAN_DIR"cursupplementsheet"  //当前补登信息
#define SHEET_TMPSUPPLEMENTFILE     PLAN_DIR"tmpsupplementsheet"  //新补登信息

enum LISTKEY{//总表的一些key值，对应 m_ListKeys
    /** 信息表*/
    PSPartNumber,   //分单
    PSProductAllNumber, //总单
    PSMacID,    //机器id
    PSTecID,  //数据库id
    PSTecSn,  //工艺序号
    PSTecName,//工艺名
    PSMJName,//模具名
    PSMaterial,//材料
    PSColor,
    PSState,//状态
    PSCountPerMold,//每模产品数
    PSProductCount,//实际规划产量
    PSTotalOKCount,//计划产量
    PSCurOKCount,//当前开模数
    PSRealSTime,
    PSRealETime,
    PSStartTime,
    PSEndTime
};

typedef struct tyPLANSHEETST
{
    //派单
    PJsonFile curPlan;           //当前派单
    PJsonFile newPlan;           //新增派单
    PJsonFile historyPlan;       //历史单

    //补登
    PJsonFile errList;          //错误映射表
    cJSON * pcurErr;            //对应当前的翻译的json
    PJsonFile supp;             //补登总信息
    PJsonFile curSup;              //当前派单补登 注意，curSup中的json数据与supp中的节点json数据一致（同一指针）

}PLANSHEETST,*PPLANSHEETST;

void PlanInit();
void PlanUpdate();
void PlanSetEnable(BOOL enable);

BOOL PlanCreateNewSupp();

void PlanReqFile(int fileid,PROT_CB tx_cb);
void PlanSendFile(int fileid,PROT_CB tx_cb);

void PlanSendFileEX(UI16 type_id,UI8 * data,UI16 len,PROT_CB tx_cb);

/*****USERLOGIN******/
void UserLoginInit();
void UserLoginShow(int cur_key);

/*****USERLOGIN END*******/

#endif
#endif //PLANSHEET_H
