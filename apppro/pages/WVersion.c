#include "WVersion.h"
#include "typedef.h"
#include "pagefram.h"
#include "verinfo.h"
#include "stdio.h"
#include "oprintf.h"
#include "common.h"
#include "panel.h"
#include "panelcfg.h"
#include "keymod.h"
#include "database.h"
#include "editwgt.h"
#include "labelwgt.h"

DEFINE_VERSION_MAP

CONNECT_VERSION_EVENT

//panel
#define TMP_PANELTYPE	    TMPVAL(0)//面板类型
#define TMP_PANELCODE	    TMPVAL(1)//面板代码
#define TMP_PANELDATE	    TMPVAL(2)//版本日期

#define TMP_PANELVER1	    TMPSTR(0)//面板版本1
#define TMP_PANELVER2	    TMPSTR(1)//面板版本2

#define TMP_LIBRARYVER1	    TMPSTR(4)//面板库版本1
#define TMP_LIBRARYVER2	    TMPSTR(5)//面板库版本2
#define TMP_LIBRARYDATE     TMPVAL(10)//库版本日期

#define TMP_PROJECTVER      TMPSTR(6)//画面工程版本号
#define TMP_PROJECTDATE     TMPSTR(7)//画面工程版本日期

//main
#define TMP_MAINTYPE	    TMPVAL(3)//主机代码
#define TMP_MAINDATE	    TMPVAL(4)//版本日期

#define TMP_MAINVER1	    TMPSTR(2)//主机版本1
#define TMP_MAINVER2	    TMPSTR(3)//主机版本2

//boot
#define TMP_BOOTVER	        TMPVAL(5)//BOOT版本
#define TMP_BOOTDATE	    TMPVAL(6)//版本日期
//hardware type
#define TMP_HARDWARE	    TMPVAL(7)//硬件类型
//update code
#define TMP_PANELUPDATE	    TMPVAL(30)//面板更新代码
#define TMP_MAINCFG	        TMPVAL(31)//主机配置代码
#define TMP_MAINMACH	    TMPVAL(32)//主机厂商代码
#define TMP_MAINPROG	    TMPVAL(33)//主机程序代码

//version
#define SYS_HARDWARE        0x00140001 //硬件类型
#define SYS_BOOTVER         0x00140004 //BOOT版本
#define SYS_BOOTDATE        0x00140005 //BOOT日期
#define SYS_MAINVER1        0x00140007 //主机版本1
#define SYS_MAINVER2        0x00140006 //主机版本2
#define SYS_MAINDATE1       0x00140008 //主机日期1
#define SYS_MAINDATE2       0x00140009 //主机日期2
//
#define UPDATE_PANELCODE    0x33120041 //
#define UPDATE_CONFIGCODE   0x33120042 //
#define UPDATE_MANUFCODE    0x33120043 //
#define UPDATE_PROGRAMCODE  0x33120044 //

#define TMP_KERVER_YEAR	    TMPVAL(60)//内核版本-年
#define TMP_KERVER_MONTH	TMPVAL(61)//内核版本-月
#define TMP_KERVER_DAY	    TMPVAL(62)//内核版本-日

//20190411 hz
VERSIONREAD  g_dbversiondefault={
    1,        // 0  wPanelMacName;
    1,        // 1  wPanelMacMode;
    1,        // 2  wPanelMacProp;
    101,      // 3  wPanelDBVer;
    200,      // 4  wPanelPlatform;
    200,      // 5  wPanelSupportSys;
    0,        // 6  wPanelManuCode;
    16,       // 7  wPanelFactory;
    2025,     // 8  wPanelDate1;
    708,      // 9  wPanelDate2;
    #ifdef M788_PANEL
    207,      // A  wPanelViewVer1;
    002,        // B  wPanelViewVer2;
    #else
    103,      // A  wPanelViewVer1;
    000,        // B  wPanelViewVer2;
    #endif
    #ifdef MPANEL_TC
    10634       // C  wPanelViewType;
    #else
    10632       // C  wPanelViewType;
    #endif
};
//0101.0200  20180904   修改数据库增加子类问题,增加多国语言；
//0200.0000  20181119   修改DA调整页面强制输出无法编辑问题，修改射出曲线1位小数问题，曲线刻度问题修改，数据操作页面修改
//参数修改记录增加保护；
//0201.0000  20181128   修改面板重启数据请求问题，修改伺服电机参数，修改机器配置U盘导入推力座问题，修改吹气7组使用后再选不使用仍然
//吹气问题
//0201.0100  20181204   应用端增加支持10寸按键及画面跳转；增加按键测试新C代码，新增10寸中子功能脚本，托模脚本，托模参数脚本。
//0201.0200  20181225   修改面板更新连续按输入键更新异常问题；修改操作模式记录问题；修改动作步语言实时刷新问题；面板尺寸判别增加panelsize；机型配置页面机型名称过长及保存修改。
//0201.0300  20181229   修改程序更新时多次输入多次更新问题；
//0201.0400  20190109   修改画面保护时不能切语言；修改用户管理页面的密码修改；增加断电保护，曲线页面范围自动变化；；
//0300.0000  20190114   变量模式归档下发；
//0300.0100  20190118   增加数据保护页面，进入默认密码：20110；
//0300.0200  20190128   增加温度优化页面相关功能；变量控制数据保护功能；修改温度跳转页面的脚本函数；修改安全门弹起键时不屏保；
//0101.0100  20200316   合并增加通用和威力仕功能；


/************************************************************************/
/* 版本日期格式转换                                                     */
/************************************************************************/
const char* VerDateToStr(UI16 value)
{
	static char cbuf[10];

	if(value<10)  
		snprintf(cbuf,sizeof(cbuf)-1,"%u%u%u%u",0,0,0,value);
	else if(value<100) 
		snprintf(cbuf,sizeof(cbuf)-1,"%u%u%u",0,0,value);
	else if(value<1000) 
		snprintf(cbuf,sizeof(cbuf)-1,"%u%u",0,value);
	else 
		snprintf(cbuf,sizeof(cbuf)-1,"%u",value);

	return cbuf;
}

/************************************************************************/
/* 内核版本数据的读取                                                      */
/************************************************************************/
static void KerVerLoad()
{
    VarAdrSetInt(TMP_KERVER_YEAR,GetKernelVersion(0));
    VarAdrSetInt(TMP_KERVER_MONTH,GetKernelVersion(1));
    VarAdrSetInt(TMP_KERVER_DAY,GetKernelVersion(2));
}

/************************************************************************/
/* 版本页面进入                                                         */
/************************************************************************/
static BOOL VerinfoPageIn()
{
	char cVer1[10];
	char cVer2[10];
	UI32 pnldate,maindate,libdate;
	UI16 mainver1,mainver2;
    const char *ptrans;

	//panel ver1
	VarAdrSetStr(TMP_PANELVER1,VerDateToStr(m_verinfo.wPanelViewVer1));
	//panel ver2
	VarAdrSetStr(TMP_PANELVER2,VerDateToStr(m_verinfo.wPanelViewVer2));

	//library ver
	VarAdrSetStr(TMP_LIBRARYVER1,VerDateToStr(LibraryVer1()));
	VarAdrSetStr(TMP_LIBRARYVER2,VerDateToStr(LibraryVer2()));
	//panel date
	pnldate = m_verinfo.wPanelDate1*10000+m_verinfo.wPanelDate2;
	VarAdrSetInt(TMP_PANELDATE,pnldate);
	libdate = LibraryDate1()*10000+LibraryDate2();
	VarAdrSetInt(TMP_LIBRARYDATE,libdate);

	//20181128
	if(PanelCfgYsize() ==480)
	{
		VarAdrSetInt(TMP_PANELTYPE,10211);//7cun
	}
//    else if(GetPanelType() == PANEL_V10)
//	{
//        VarAdrSetInt(TMP_PANELTYPE,10632);//10cun V
//	}
    else if(GetPanelType() == PANEL_H10)
    {
        VarAdrSetInt(TMP_PANELTYPE,10231); //10cun H
    }
	else
	{
		VarAdrSetInt(TMP_PANELTYPE,m_verinfo.wPanelViewType);//8cun
	}
    VarAdrSetInt(TMP_PANELCODE, m_panelcode.wOutPanelFactory);//20210524

	//view project ver
	VarAdrSetStr(TMP_PROJECTVER,PanelProVer());
	VarAdrSetStr(TMP_PROJECTDATE,PanelProDate());

	//main ver1
    VarAdrSetInt(TMP_MAINTYPE,VarAdrToUI16(SYS_MACH_CUSTOMID)/256);
	mainver1 = VarAdrToUI16(SYS_MAINVER1);
	if(mainver1<1000)
		snprintf(cVer1,sizeof(cVer1)-1,"%u%u",0,mainver1);
	else 
		snprintf(cVer1,sizeof(cVer1)-1,"%u",mainver1);
	VarAdrSetStr(TMP_MAINVER1,cVer1);
	//main ver2
	mainver2 = VarAdrToUI16(SYS_MAINVER2);
	if(mainver2<10)  
		snprintf(cVer2,sizeof(cVer2)-1,"%u%u%u%u",0,0,0,mainver2);
	else if(mainver2<100) 
		snprintf(cVer2,sizeof(cVer2)-1,"%u%u%u",0,0,mainver2);
	else if(mainver2<1000) 
		snprintf(cVer2,sizeof(cVer2)-1,"%u%u",0,mainver2);
	else 
		snprintf(cVer2,sizeof(cVer2)-1,"%u",mainver2);
	VarAdrSetStr(TMP_MAINVER2,cVer2);
	//main date
	maindate =  (VarAdrToUI16(SYS_MAINDATE2) & 0xFF) *65536 + VarAdrToUI16(SYS_MAINDATE1) + 20000000;
	VarAdrSetInt(TMP_MAINDATE,maindate);

	//boot ver
	VarAdrSetInt(TMP_BOOTVER,VarAdrToUI16(SYS_BOOTVER));
	//boot date  (wVer5%100+2000)*10000+wVer6
	VarAdrSetInt(TMP_BOOTDATE,(VarAdrToUI16(SYS_BOOTVER)%100+2000)*10000+VarAdrToUI16(SYS_BOOTDATE));

	//hardware
    VarAdrSetInt(TMP_HARDWARE,(VarAdrToUI16(SYS_HARDWARE)&0x1FFF));

	//update code need to do
	VarAdrSetInt(TMP_PANELUPDATE,VarAdrToUI16(UPDATE_PANELCODE));
	VarAdrSetInt(TMP_MAINCFG,VarAdrToUI16(UPDATE_CONFIGCODE));
	VarAdrSetInt(TMP_MAINMACH,VarAdrToUI16(UPDATE_MANUFCODE));
	VarAdrSetInt(TMP_MAINPROG,VarAdrToUI16(UPDATE_PROGRAMCODE));

    //20200326 powertype machtype usetype to do
    if(pmap_powertype !=NULL)
    {
        ptrans = GetMapTran(pmap_powertype,VarAdrToUI16(d_machine1_VERSION_power_type));
        if(ptrans!=NULL)
        {
            VarAdrSetStr(p_PP_TMP_TMPSTR10,ptrans);
        }
        else
        {
            VarAdrSetStr(p_PP_TMP_TMPSTR10,"0");
        }
    }
    else
    {
        VarAdrSetStr(p_PP_TMP_TMPSTR10,"0");
    }
    if(pmap_machtype !=NULL)
    {
        ptrans = GetMapTran(pmap_machtype,VarAdrToUI16(d_machine1_VERSION_machine_type));
        if(ptrans!=NULL)
        {
            VarAdrSetStr(p_PP_TMP_TMPSTR11,ptrans);
        }
        else
        {
            VarAdrSetStr(p_PP_TMP_TMPSTR11,"0");
        }
    }
    else
    {
        VarAdrSetStr(p_PP_TMP_TMPSTR11,"0");
    }
    if(pmap_usetype !=NULL)
    {
        ptrans = GetMapTran(pmap_usetype,VarAdrToUI16(d_machine1_VERSION_application_type));
        if(ptrans!=NULL)
        {
            VarAdrSetStr(p_PP_TMP_TMPSTR12,ptrans);
        }
        else
        {
            VarAdrSetStr(p_PP_TMP_TMPSTR12,"0");
        }
    }
    else
    {
        VarAdrSetStr(p_PP_TMP_TMPSTR12,"0");
    }

    KerVerLoad();

    //不打上code码不显示版本号
    if(VarAdrToInt(SYS_FL_MACH_CODE54) & 0x800)//DA扩展板
    {
        EditSetTextColor(version_pg.edit166, GUI_WHITE);
    }
    else
    {
        EditSetTextColor(version_pg.edit166, EditGetBKColor(version_pg.edit166));
    }

    if(VarAdrToInt(SYS_FL_MACH_CODE54) & 0x200)//IO扩展板1
    {
        EditSetTextColor(version_pg.edit193, GUI_WHITE);
    }
    else
    {
        EditSetTextColor(version_pg.edit193, EditGetBKColor(version_pg.edit193));
    }

    if(VarAdrToInt(SYS_FL_MACH_CODE54) & 0x2000)//IO扩展板2
    {
        EditSetTextColor(version_pg.edit194, GUI_WHITE);
    }
    else
    {
        EditSetTextColor(version_pg.edit194, EditGetBKColor(version_pg.edit194));
    }

	return TRUE;
}

static void CtlConnect()
{
}


static int VersionOnInit(Version_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	PanelLibVerLoad();
    return 0;
}
static int VersionOnShow(Version_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	VerinfoPageIn();

    return 0;
}
static int VersionOnHide(Version_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int VersionOnUpdate(Version_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int VersionOnChange(Version_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
