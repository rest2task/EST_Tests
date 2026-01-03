#include "WPanel_DetailInfo.h"
#include "typedef.h"
#include "pagefram.h"
#include "file.h"
#include "vardb.h"
#include "mod.h"
#include "prodpower.h"
#include "udisk.h"
#include "labelwgt.h"
#include "stdio.h"
#ifndef WIN32
#include <sys/sysinfo.h>
#endif

DEFINE_PANEL_DETAILINFO_MAP

CONNECT_PANEL_DETAILINFO_EVENT

#define	TMPADDR_WRITETOTAL_ALL      TMPVAL(1) //写文件总次数
#define	TMPADDR_WRITETIME_24        TMPVAL(2) //24小时写次数上限-计时
#define	TMPADDR_WRITECNT_24         TMPVAL(3) //24小时写次数上限-写文件次数
#define	TMPADDR_WRITEMOLDCNT_CTI    TMPVAL(4) //连续多少模写文件-连续模数
#define	TMPADDR_WRITETIME_24_MIN    TMPVAL(5) //24小时写次数上限-计分
//20240414 jhh 优化面板内存使用率，增加面板内存使用情况显示
#ifndef WIN32
#define TMPADDR_MEMUSED             TMPSTR(1) //面板内存使用情况
    struct sysinfo info;
#endif

static void CtlConnect()
{
}
static int Panel_DetailInfoOnInit(Panel_DetailInfo_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Panel_DetailInfoOnShow(Panel_DetailInfo_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    VarAdrSetInt(TMPADDR_WRITETOTAL_ALL,0);
    VarAdrSetInt(TMPADDR_WRITETIME_24,0);
    VarAdrSetInt(TMPADDR_WRITETIME_24_MIN,0);
    VarAdrSetInt(TMPADDR_WRITECNT_24,0);
    VarAdrSetInt(TMPADDR_WRITEMOLDCNT_CTI,0);

//20240414 jhh 优化面板内存使用率，增加面板内存使用情况显示
#ifndef WIN32
    WGTSetVisible(ppg->edit114, TRUE);
    WGTSetVisible(ppg->labelmem, TRUE);

    if (sysinfo(&info) != 0)
    {
        VarAdrSetStr(TMPADDR_MEMUSED, "");
    }
    else
    {
        char MemUsed[128];
        snprintf(MemUsed, sizeof(MemUsed) - 1, "%.2f/%.2f", (info.totalram - info.freeram) / 1024.0 / 1024, info.totalram / 1024.0 / 1024);
        VarAdrSetStr(TMPADDR_MEMUSED, MemUsed);
    }
#else
    WGTSetVisible(ppg->edit114, FALSE);
    WGTSetVisible(ppg->labelmem, FALSE);
#endif

    /*
    char text[30];
    snprintf(text, sizeof(text)-1, "%s", FILE_PATH);
    LabelSetText(ppg->stcode, text);
    */
    return 0;
}
static int Panel_DetailInfoOnHide(Panel_DetailInfo_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Panel_DetailInfoOnUpdate(Panel_DetailInfo_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    VarAdrSetInt(TMPADDR_WRITETOTAL_ALL,g_moniwritecntdata.WriteTotalCnt);
    VarAdrSetInt(TMPADDR_WRITETIME_24,g_moniwritecntdata.Min24/60);
    VarAdrSetInt(TMPADDR_WRITETIME_24_MIN,g_moniwritecntdata.Min24%60);
    VarAdrSetInt(TMPADDR_WRITECNT_24,g_moniwritecntdata.WriteCnt24);
    VarAdrSetInt(TMPADDR_WRITEMOLDCNT_CTI,g_moniwritecntdata.ContinueWriteCnt);

    return 0;
}
static int Panel_DetailInfoOnChange(Panel_DetailInfo_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
