#include "WPump_Cfg.h"
#include "typedef.h"
#include "pagefram.h"
#include "usermod.h"

DEFINE_PUMP_CFG_MAP

CONNECT_PUMP_CFG_EVENT

static void CtlConnect()
{
}
static int Pump_CfgOnInit(Pump_Cfg_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Pump_CfgOnShow(Pump_Cfg_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_MODELOCK);//20250401 jhh 增加模式锁
    return 0;
}
static int Pump_CfgOnHide(Pump_Cfg_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20250401 jhh 增加模式锁
    return 0;
}
static int Pump_CfgOnUpdate(Pump_Cfg_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageProtect();//20250401 jhh 增加模式锁
    return 0;
}
static int Pump_CfgOnChange(Pump_Cfg_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
