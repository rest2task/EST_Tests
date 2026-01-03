#include "WBlow.h"
#include "typedef.h"
#include "pagefram.h"
#include "common.h"
#include "oprintf.h"
#include "database.h"
#include "information.h"

DEFINE_BLOW_MAP

CONNECT_BLOW_EVENT

extern BOOL b_in_ariblowpage2;

static void CtlConnect()
{
}
static int BlowOnInit(Blow_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int BlowOnShow(Blow_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    b_in_ariblowpage2 = FALSE;
    int i = 0;
    for(i = 0;i<14;i++)
    {
        UI16 blow_mode = VarAdrToInt(d_airblow1_MOLDSET_FL_BLAST+0x100000*i);
        if(blow_mode == 256)
        {
            VarAdrSetInt(p_PP_MACHSET_FL_BLAST1+i, 0);
        }
        else
        {
            VarAdrSetInt(p_PP_MACHSET_FL_BLAST1+i, blow_mode+1);
        }
    }
    return 0;
}
static int BlowOnHide(Blow_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    b_in_ariblowpage2 = FALSE;
    return 0;
}
static int BlowOnUpdate(Blow_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int BlowOnChange(Blow_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 addr;
    PATTR pattr;
    pattr = (PATTR)msg_para;
    addr = OBJAttrGetAdr(pattr);
    UI16 index = 0;
    UI16 value;
    value = VarAdrToUI16(addr);
    if((addr >= p_PP_MACHSET_FL_BLAST1) && (addr <= p_PP_MACHSET_FL_BLAST14))
    {
        if(OperateModeIndex() == MODE_MANUAL)
        {
            index = addr-p_PP_MACHSET_FL_BLAST1;

            if(value == 0)
            {
                VarSendSaveSetIntByAdr(d_airblow1_MOLDSET_FL_BLAST+0x100000*index, 256);
            }
            else if((value == 8) && (VarAdrToInt(p_PP_MACHSET_EJECTFUNC_SENSOR) == 0))
            {
                VarSendSaveSetIntByAdr(addr,msg_value);
                ShowMsg(VW_EJECT_SENSER_USE);
                return -1;
            }
            else
            {
                VarSendSaveSetIntByAdr(d_airblow1_MOLDSET_FL_BLAST+0x100000*index, value-1);
            }

            VarSendByAdr(d_airblow1_MOLDSET_ADPOSI_MOVINGBLASTSTART+0x100000*index);//20241210 chj 28组吹气:下拉框改变，对应的位置也要再发送下
        }
        else
        {
            VarSendSaveSetIntByAdr(addr,msg_value);
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }
    }

    return 0;
}
