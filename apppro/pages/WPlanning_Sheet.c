#include "WPlanning_Sheet.h"
#include "typedef.h"
#include "pagefram.h"
#include "plansheet.h"
#include "dropwgt.h"
DEFINE_PLANNING_SHEET_MAP

CONNECT_PLANNING_SHEET_EVENT

static void CtlConnect()
{
}
static int Planning_SheetOnInit(Planning_Sheet_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Planning_SheetOnShow(Planning_Sheet_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Planning_SheetOnHide(Planning_Sheet_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Planning_SheetOnUpdate(Planning_Sheet_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Planning_SheetOnChange(Planning_Sheet_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
#ifndef WIN32
//    if(pwgt == ppg->dropdown1)
//    {
//        int index = DropGetCurIndex(ppg->dropdown1);
//        PlanSetEnable(index);
//    }
#endif
    return 0;
}
