#include "WPart_Table.h"
#include "typedef.h"
#include "pagefram.h"
#include "tablectl.h"
#include "editwgt.h"
#include "command.h"

DEFINE_PART_TABLE_MAP

CONNECT_PART_TABLE_EVENT

static void CtlConnect()
{
}
static int Part_TableOnInit(Part_Table_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Part_TableOnShow(Part_Table_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    EditSetNum(ppg->edmachine,      GetSubPartNum(MACHINE_ID)>1?GetSubPartNum(MACHINE_ID):1,  0);
    EditSetNum(ppg->edmotor,        GetSubPartNum(MOTOR_ID)>1?GetSubPartNum(MOTOR_ID):1,    0);
    EditSetNum(ppg->edhydr,         GetSubPartNum(HYDR_ID)>1?GetSubPartNum(HYDR_ID):1,     0);
    EditSetNum(ppg->edservo,        GetSubPartNum(HYDRSERVO_ID)>1?GetSubPartNum(HYDRSERVO_ID):1,0);
    EditSetNum(ppg->edalarm,        GetSubPartNum(ALARM_ID)>1?GetSubPartNum(ALARM_ID):1,    0);
    EditSetNum(ppg->edoil,          GetSubPartNum(OIL_ID)>1?GetSubPartNum(OIL_ID):1,      0);
    EditSetNum(ppg->edtctemp,       GetSubPartNum(TEMPEXT_ID),  0);
    EditSetNum(ppg->edpump,         GetSubPartNum(PUMP_ID),     0);
    EditSetNum(ppg->edclamp,        GetSubPartNum(CLAMP_ID),    0);
    EditSetNum(ppg->edinject,       GetSubPartNum(INJECT_ID),   0);
    EditSetNum(ppg->ednozzle,       GetSubPartNum(NOZZLE_ID),   0);
    EditSetNum(ppg->edinjstor,      GetSubPartNum(INJSTOR_ID),  0);
    EditSetNum(ppg->edchargepress,  GetSubPartNum(CHGPRS_ID),   0);
    EditSetNum(ppg->edeject,        GetSubPartNum(EJECT_ID),    0);
    EditSetNum(ppg->edcore,         GetSubPartNum(CORE_ID),     0);
    EditSetNum(ppg->edairblow,      GetSubPartNum(AIRBLOW_ID),  0);
    EditSetNum(ppg->edadjust,       GetSubPartNum(ADJUST_ID),   0);
    EditSetNum(ppg->edsysacc,       GetSubPartNum(SYSACC_ID),   0);
    EditSetNum(ppg->edshut,         GetSubPartNum(SHUT_ID),     0);
    EditSetNum(ppg->edrotate,       GetSubPartNum(ROTATE_ID),   0);
    EditSetNum(ppg->edlock,         GetSubPartNum(LOCK_ID),     0);
    EditSetNum(ppg->edsafedoor,     GetSubPartNum(SFDOOR_ID),   0);
    EditSetNum(ppg->edmoldtemp,     GetSubPartNum(MOLDTEMP_ID), 0);
    EditSetNum(ppg->edlub,          GetSubPartNum(LUB_ID),      0);
    EditSetNum(ppg->edrobot,        GetSubPartNum(ROBOT_ID),    0);
    EditSetNum(ppg->edsensor,       GetSubPartNum(AUTOCTRL_ID), 0);

    return 0;
}
static int Part_TableOnHide(Part_Table_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Part_TableOnUpdate(Part_Table_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Part_TableOnChange(Part_Table_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
