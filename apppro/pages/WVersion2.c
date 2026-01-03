#include "WVersion2.h"
#include "typedef.h"
#include "pagefram.h"
#include "database.h"
#include "stdio.h"
#include "labelwgt.h"
#include "verinfo.h"

DEFINE_VERSION2_MAP

CONNECT_VERSION2_EVENT

#define PUSH_TEMP_ADDR    TMPSTR(34) //十字头型号对应得临时地址

//20200408.cyx
static void SetPushWndTxt()
{
    char cpushbak[8] = "";
    UI16 wValue1 = VarAdrToInt(d_machine1_MACHSET_FL_reserve_3)/100;
    UI16 wValue2 = VarAdrToInt(d_machine1_MACHSET_FL_reserve_3)%100;
    UI16 g_pushcode = VarAdrToInt(d_machine1_MACHSET_FL_MACHINEMODEL);

    if(VarAdrToInt(d_machine1_MACHSET_FL_reserve_3) > 100)
    {
        sprintf(cpushbak, "%d%c%c", g_pushcode, wValue1, wValue2);
    }
    else
    {
        if(VarAdrToInt(d_machine1_MACHSET_FL_reserve_3) == 0)
        {
            sprintf(cpushbak, "%d", g_pushcode);
        }
        else
        {
            sprintf(cpushbak, "%d%c", g_pushcode, wValue2);
        }
    }

    VarAdrSetStr(PUSH_TEMP_ADDR, cpushbak);
}

static void CtlConnect()
{
}
static int Version2OnInit(Version2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Version2OnShow(Version2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPushWndTxt();

    return 0;
}
static int Version2OnHide(Version2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Version2OnUpdate(Version2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Version2OnChange(Version2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
