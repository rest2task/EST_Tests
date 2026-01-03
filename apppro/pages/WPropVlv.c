#include "WPropVlv.h"
#include "typedef.h"
#include "pagefram.h"
#include "dbenum.h"
#include "vardb.h"
#include "information.h"
#include "database.h"

DEFINE_PROPVLV_MAP

CONNECT_PROPVLV_EVENT

static void CtlConnect()
{
}
static int PropVlvOnInit(PropVlv_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int PropVlvOnShow(PropVlv_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int PropVlvOnHide(PropVlv_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int PropVlvOnUpdate(PropVlv_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int PropVlvOnChange(PropVlv_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 addr,value;
    PATTR pattr;

    pattr = (PATTR)msg_para;

    addr = OBJAttrGetAdr(pattr);
    value = VarAdrToInt(addr);

    //20250306 jhh 非手动情况下提示
    switch (addr)
    {
    case d_inject1_MACHSET_INJECTVLV_CH:
    case d_inject1_MACHSET_INJECTVLV_DIR:
    case d_inject1_MACHSET_SUCKBACKVLV_CH:
    case d_inject1_MACHSET_SUCKBACKVLV_DIR:
    case d_inject1_MACHSET_FL_INJECTVLV:
    case d_inject1_MACHSET_FL_SUCKBACKVLV:
    case d_inject1_MACHSET_ACTOPEN_CH:
    case d_clamp1_MACHSET_CLAMPOPNVLV_CH:
    case d_clamp1_MACHSET_CLAMPOPNVLV_DIR:
    case d_clamp1_MACHSET_CLAMPCLSVLV_CH:
    case d_clamp1_MACHSET_CLAMPCLSVLV_DIR:
        if(VarAdrToInt(OPMODE_INDEX) != MODE_MANUAL)
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//原始值
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }

        break;
    default :
        break;
    }
    return 0;
}
