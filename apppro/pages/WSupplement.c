#include "WSupplement.h"
#include "typedef.h"
#include "pagefram.h"
#include "plansheet.h"
#include "vardb.h"
#include "taskmoni.h"
#include "oprecord.h"
#include "information.h"
#include "plansheet.h"
DEFINE_SUPPLEMENT_MAP

CONNECT_SUPPLEMENT_EVENT

#define PANELDB_PRENAME "p.PP.NET."
static void CtlConnect()
{
}
static int SupplementOnInit(Supplement_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int SupplementOnShow(Supplement_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    VarNameSetInt(PANELDB_PRENAME"ERRCNT",0);
    if(OperateModeIndex() == MODE_MANUAL)
    {
        WU32(ppg->edit124->is_enable,1) ;
    }
    else
    {
        WU32(ppg->edit124->is_enable,0) ;
    }
    return 0;
}
static int SupplementOnHide(Supplement_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int SupplementOnUpdate(Supplement_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int SupplementOnChange(Supplement_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
#ifndef WIN32
    int addr = 0x3318003e;
    if(pwgt == ppg->bt_verity)
    {
        if(OperateModeIndex()!=MODE_MANUAL)
        {
            ShowMsg(VW_MSG_MANUAL);
        }
        else
        {
            int i = 0;
            if(!PlanCreateNewSupp())
            {
                ShowMsg(VW_MSG_PLANERR);
            }
            else
            {
                ShowMsg(VW_MSG_PLANSUCCESS);
            }
            VarNameSetInt(PANELDB_PRENAME"ERRCOUNT",0);
            for(i=0;i<12;i++)
            {
                VarAdrSetInt( addr+i,0);
            }
        }
    }
    else if(pwgt == ppg->button1)
    {
        PlanReqFile(FID_ERRORINFO,0);
    }
#endif
    return 0;
}
