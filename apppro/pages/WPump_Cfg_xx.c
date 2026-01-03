#include "WPump_Cfg_xx.h"
#include "typedef.h"
#include "pagefram.h"
#include "database.h"
#include "usermod.h"//20210914 dyl touch

DEFINE_PUMP_CFG_XX_MAP

CONNECT_PUMP_CFG_XX_EVENT

static void CtlConnect()
{
}
static int Pump_Cfg_xxOnInit(Pump_Cfg_xx_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Pump_Cfg_xxOnShow(Pump_Cfg_xx_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_MODELOCK);//20210914 dyl touch

    return 0;
}
static int Pump_Cfg_xxOnHide(Pump_Cfg_xx_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch
    return 0;
}
static int Pump_Cfg_xxOnUpdate(Pump_Cfg_xx_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageProtect();//20210914 dyl touch //PAGE_PROTECT

    return 0;
}
static int Pump_Cfg_xxOnChange(Pump_Cfg_xx_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 value,index;
    UI32 addr;
    PATTR pattr;

    pattr = (PATTR)msg_para;

    addr = OBJAttrGetAdr(pattr);

    if((addr&0xFF0FFFFF) == (d_Pump1_MOLDSET_PUMP_SLAVE_START&0xFF0FFFFF))
    {
        value = VarAdrToInt(addr);
        index = ((addr >> 20)&0xF) - 1;
        if(VarAdrToInt(d_Pump1_MOLDSET_PUMP_SLAVE_STOP+index*0x100000) > value)
        {
            VarSendSaveSetIntByAdr(d_Pump1_MOLDSET_PUMP_SLAVE_STOP+index*0x100000, value);
        }
    }

    return 0;
}
