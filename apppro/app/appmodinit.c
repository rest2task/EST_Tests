#include "pay.h"
#include "paramrecord.h"
#include "mod.h"
#include "strevent.h"
#include "database.h"
#include "warm.h"
#include "temp.h"
#include "texttbl.h"
#include "taskmoni.h"
#include "keymod.h"
#include "panel.h"
#include "key.h"
#include "pagesinit.h"
#include "navigate.h"
#include "protmod.h"
#include "protcmd.h" 
#include "power.h"
#include "paramrecord.h"
#include "oprecord.h"
#include "prodmanage.h"
#include "moldset.h"
#include "curvesctl.h"
#include "prodmoni.h"
#include "usermod.h"
#include "ioredef.h"
#include "pushtab.h"
#include "verinfo.h"
#include "varctl.h"
#include "oprintf.h"
#include "prodpower.h"
#include "poweroff.h"
#include "servoalarmrec.h"
#include "plansheet.h"
#include "keyrec.h"
#include "cmdrec.h"
#include "wgt.h"
#include "panelcfg.h"
#include "warndatarec.h"
#include "keyprot.h"
#include "panelrec.h"
#include "udisk.h"
#include "pagefram.h"
#include "servoalarmtbl.h"//20211216 dyl 伺服警报名称显示
#include "iapadmin.h"//20211229 dyl IAP
#include "pumpfunc.h"
#include "opcuaadmin.h"

#if HMILIB_VER <= 4000200
#include "winmsgctl.h"
#endif

void AppOnUpdate(void)
{
    UI32 cur_time;

	cur_time = GetTick();
	VarAdrSetInt(PROG_CYCTIME,cur_time);

	PayMonitor();
	WarmMonitor();
	PreHeatMonitor();
    UpdateTimeCount();//20211112 dyl 开机总时间、运行总计时统计
    Monitor_Iap();//20211229 dyl IAP
    //TempOptimizeTm(); //20190522 hz
	OtherTaskMoni();
    //ServoWarnMoni(); //20190514 hz
    TempCurveGetData(); //20190627 hz
    TempCurveGetData2();//20250312 jhh 增加温度曲线2画面
    AutoUdiskUpdate();
#ifndef WIN32
#ifdef IMX6
    PowerOffMonitor(); //20190505 hz
#endif
#endif
#if HMILIB_VER <= 4000200
#ifdef WIN32
	MsgCtlLoop();
#endif
#endif
#ifndef WIN32
    //PlanUpdate(); //20210611.cyx 联网功能暂时不开放
#endif

#ifndef WIN32
    if(VarAdrToInt(p_PP_MACHSET_FL_USEOPCUA))
    {
        flashOpcua();
    }

#endif
}

int AppOnChange(const char* pf_name,PWGT pwgt, MSG_ID msg_id, int old_value, PATTR attr)
{
	RecordValueEX((POBJ)pwgt,old_value,attr);
	return 0;
}

//20230518 chj 开机进入FACE页面，显示数据加载中
void ShowFace()
{
    PPAGE_FRAM pf = PanelFindPage(PAGE_FACE);
    if(pf)
    {
        if(m_operation.bTimeOut == 0) //20241225 chj  修复开机的时候，提前3天不跳转到分期页面
        {
            PanelShowPage(pf);
        }
    }
}

/*call from panel framwork when initialze done*/
void AppModInit(void)
{

    //应用层设定系统的单个文件是否需要主动保存（去除ch_sync）20240605 chj
    //应用层默认单个文件是主动保存； TI：不主动保存,需要设置为false;A40I/T113需要主动保存，设置为true
#ifdef A40I
    SetFileActiveSave(TRUE);
#else
    SetFileActiveSave(FALSE);
#endif
/*********Initialize modules used by pages***********************************/
#ifndef WIN32
#ifdef IMX6
    LoadManualSaveData();//20210714.cyx 先加载手动保存数据再加载断电保护数据
    InitProdPower();//日月年产量能耗 //20230518 chj 优化断电保护，将函数移到外面
    PowerOffInint(); //20190505 hz
#else
	SetPowerDownData_cb(GetProtectData);
	SetReadPowerData_cb(SetProtectData);
#endif
#endif
    PanelCfgLoad();
    SetFirstPageShow(ShowFace);//20230518 chj 开机进入FACE页面，显示数据加载中
	LanguageSetInit();
	DataBaseInit();
	InitPushTable();
    NaviInit();
    CModInital();
    ServoAlarmTblInital();//20211216 dyl 伺服警报名称显示
	TextTblInital();
	ProcStrEventByFile(DATA_PATH"boot.event");
	PanelSetUpdateCb(AppOnUpdate);
	KeySetTranCb(KeyGetCallBack);
	PanelKeyProcCb(PanelKeyProcCallBack);
	PanelSetOnChgCb(AppOnChange);
	PortPktRecvCbSet(ModPktRecv);
	InitUserPwd();
	NaviSetPasswordCB(NaviPasswordUser);
	PowerParamInit();
	ParamRecordInit();
	OPParamInit();
	ProdInit();
	MoldSetInital();
	CurvesInital();
	ProdMoniInital();
	IoredefInital();
    PumpInital(); //20241223 fqh Pump表初始化
	XmlToMapKey();
    //KeyRecInit();//20220822 dyl 去掉按键记录
    PortPktSendCbSet(CmdSendRecInit);
    CmdRecInit();
    WDRecInit();
    PanelRecInit();
    LoadMoniWriteData();//20230518 chj 写文件次数监控
    TimeCountInit();//20211112 dyl 开机总时间、运行总计时统计

    //InitProdPower();  //年月日产量功能初始化 20190403 hz  移动到断电保护 20190626 hz
    ServoWarnInit();  //20190514 hz
    InitIOMap(); //20191203.cyx
    InitRedefData();//20190514.cyx 初始化IO反逻辑、强制输出
    LoadWarmSet();

    if(PanelCfgStyle() == G_TYPE || PanelCfgStyle() == P_TYPE)
    {
        WGTSetCancelFocusCB(SetCancelFocus);//20191214 取消指定类别聚焦
        WGTSetTabPageFocusCB(DownSetTabPageFocus);
    }
    PanelVerLoad();
    ResourceLoad();//20211210 dyl 资源分配：IO输入输出点，AD电子尺，DA输出
    KernelVersionRead();
    SetRevAckCb(Reset_KeyCnt);//20191224.cyx 按键板通讯

    PanelSetVarCtrlCb(VarCtlMoni);
/*********Initialize modules ends***********************************/

	/*load page special modules*/
	AppPagesInit();
#ifndef WIN32
    PlanInit();
#endif

#ifndef WIN32
    if(VarAdrToInt(p_PP_MACHSET_FL_USEOPCUA))
    {
        initOpcua();
    }
#endif
}


