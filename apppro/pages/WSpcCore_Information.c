#include "WSpcCore_Information.h"
#include "typedef.h"
#include "pagefram.h"
#include "panel.h"

DEFINE_SPCCORE_INFORMATION_MAP

CONNECT_SPCCORE_INFORMATION_EVENT

static int btOKOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static void CtlConnect()
{
    WgtMsgConnect(spccore_information_pg.btOk, WGT_CLICK_MSG, btOKOnClick);
}
static int SpcCore_InformationOnInit(SpcCore_Information_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int SpcCore_InformationOnShow(SpcCore_Information_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int SpcCore_InformationOnHide(SpcCore_Information_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int SpcCore_InformationOnUpdate(SpcCore_Information_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int SpcCore_InformationOnChange(SpcCore_Information_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(pwgt == PageGetWgtByName(spccore_information_pg_name,"btOK"))
    {
        PanelCloseDialogByName(spccore_information_pg_name);//20191223.cyx
    }
    return 0;
}
static int btOKOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    PanelCloseDialogByName(spccore_information_pg_name);

    return 0;
}
