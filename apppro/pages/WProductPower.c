#include "WProductPower.h"
#include "typedef.h"
#include "pagefram.h"
#include "prodpower.h"
#include "time.h"
#include "oprintf.h"
#include "panelrec.h"
#include "usermod.h"//20210914 dyl touch
#include "information.h"

DEFINE_PRODUCTPOWER_MAP

CONNECT_PRODUCTPOWER_EVENT

#define TMP_RESET_DATE          TMPSTR(1) //重置日期
#define TMP_PROD_POWER          TMPVAL(10) //生产能耗
#define TMP_PROD_CNT            TMPVAL(11) //产品数
#define TMP_OPENCNT_POWER       TMPVAL(12) //开模数单位产品能耗
#define TMP_PRODCNT_POWER       TMPVAL(13) //产品数单位产品能耗
#define TMP_UNITWEIGHT_POWER    TMPVAL(14) //单位重量能耗
#define TMP_OTHER_POWER         TMPVAL(15) //其他能耗总计
#define TMP_TOTAL_POWER         TMPVAL(16) //总能耗

//20230518 chj 优化能耗表，采用电表数据减去生产能耗数据
static long dwLastTotal = -1;

static void CtlConnect()
{
}
static int ProductPowerOnInit(ProductPower_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int ProductPowerOnShow(ProductPower_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_USERLOCK);//20210914 dyl touch

    char date[20];
    TimeToStr(date,"yy-MM-dd/hh:mm",g_dbPower.tmModify);
    VarAdrSetStr(TMP_RESET_DATE, date);

    //20230518 chj 优化能耗表，采用电表数据减去生产能耗数据
    dwLastTotal = -1;

    return 0;
}
static int ProductPowerOnHide(ProductPower_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch
    return 0;
}
static int ProductPowerOnUpdate(ProductPower_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 open_cnt, prod_cnt;

    if(GetCurrentCheck())
    {
        WGTSetVisible(ppg->lbldataerr, FALSE);
    }
    else
    {
        WGTSetVisible(ppg->lbldataerr, TRUE);
    }

    if(VarAdrToInt(d_machine1_STATE_VOL_A) == 0)
    {
        WGTSetVisible(ppg->lbldisconnect, TRUE);
    }
    else
    {
        WGTSetVisible(ppg->lbldisconnect, FALSE);
    }

    open_cnt = VarAdrToInt(d_machine1_STATE_MODOPENNUM1);
    prod_cnt = open_cnt * VarAdrToUI16(PROD_PERCNT);
    VarAdrSetInt(TMP_PROD_CNT, prod_cnt);

    VarAdrSetInt(TMP_PROD_POWER, g_dbPower.dwAutoPower);
    if((open_cnt == 0) || (prod_cnt == 0) || (VarAdrToInt(p_PP_MACHSET_PERPRODWEIGHT) == 0))
    {
        VarAdrSetInt(TMP_OPENCNT_POWER, 0);
        VarAdrSetInt(TMP_PRODCNT_POWER, 0);
        VarAdrSetInt(TMP_UNITWEIGHT_POWER, 0);
    }
    else
    {
        VarAdrSetInt(TMP_OPENCNT_POWER, g_dbPower.dwAutoPower*1.0/open_cnt+0.5);
        VarAdrSetInt(TMP_PRODCNT_POWER, g_dbPower.dwAutoPower*1.0/prod_cnt+0.5);
        VarAdrSetInt(TMP_UNITWEIGHT_POWER, (g_dbPower.dwAutoPower*100000.0/open_cnt)/VarAdrToInt(p_PP_MACHSET_PERPRODWEIGHT)+0.5);
    }

    //20230518 chj 优化能耗表，采用电表数据减去生产能耗数据
    if(dwLastTotal != g_dbPower.dwTotalPower)//增加这个是为了保证生产能耗和其他能耗数据刷新顺序不出现先后问题
    {
        dwLastTotal = g_dbPower.dwTotalPower;
        if(VarAdrToInt(d_machine1_STATE_ENERGY_TOTAL_L) > g_dbPower.dwAutoPower)
        {
            VarAdrSetInt(TMP_OTHER_POWER, VarAdrToInt(d_machine1_STATE_ENERGY_TOTAL_L)-g_dbPower.dwAutoPower);
        }
        else
        {
            VarAdrSetInt(TMP_OTHER_POWER,0);
        }
    }

    //VarAdrSetInt(TMP_TOTAL_POWER, g_dbPower.dwTotalPower);//20230518 chj 优化能耗表，此处改为画面关联变量

    return 0;
}
static int ProductPowerOnChange(ProductPower_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    char date[20];

    if(pwgt == ppg->btok)
    {
        g_dbPower.tmModify = (UI32)time(NULL);
        TimeToStr(date,"yy-MM-dd/hh:mm",g_dbPower.tmModify);
        VarAdrSetStr(TMP_RESET_DATE, date);

        VarSendSaveSetIntByAdr(d_machine1_STATE_MODOPENNUM1, 0);

        g_dbPower.dwOpenCnt = 0;
        g_dbPower.dwAutoPower = 0;
        g_dbPower.dwOtherPower = 0;
        g_dbPower.dwTotalPower = 0;
        g_dbPower.dwCurrentPower = VarAdrToInt(d_machine1_STATE_ENERGY_TOTAL_L);
        SavePowerMeter();//20230518 chj 假如断电保护文件有问题，需要面板定时保存下电表能耗数据

        PanelRecAdd(pwgt, PNL_RESET, 0);
        ShowMsg(VW_MSG_RESTORE_ELEC_METER_TODEFAULT);//20241230 chj 重置电能表需要提示
    }

    return 0;
}
