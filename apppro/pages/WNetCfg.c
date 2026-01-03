#include "WNetCfg.h"
#include "typedef.h"
#include "pagefram.h"
#include "iotnet.h"
#include "oprintf.h"
#include "editwgt.h"
#include "checkboxwgt.h"
DEFINE_NETCFG_MAP

CONNECT_NETCFG_EVENT

static void CtlConnect()
{
}
static int NetCfgOnInit(NetCfg_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int NetCfgOnShow(NetCfg_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int NetCfgOnHide(NetCfg_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int NetCfgOnUpdate(NetCfg_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int NetCfgOnChange(NetCfg_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
