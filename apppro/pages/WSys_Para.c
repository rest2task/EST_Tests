#include "WSys_Para.h"
#include "typedef.h"
#include "pagefram.h"
#include "dbenum.h"
#include "vardb.h"
#include "information.h"
#include "database.h"
#include "questionDlg.h"
#include "oprintf.h"

DEFINE_SYS_PARA_MAP

CONNECT_SYS_PARA_EVENT

static QUESTION g_question;

static void CtlConnect()
{
}
static int Sys_ParaOnInit(Sys_Para_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Sys_ParaOnShow(Sys_Para_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    g_question.pOkfunc = PanelMainRestart;
    g_question.pCancelfunc = NULL;
    return 0;
}
static int Sys_ParaOnHide(Sys_Para_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Sys_ParaOnUpdate(Sys_Para_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Sys_ParaOnChange(Sys_Para_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 addr;
    PATTR pattr;
    pattr = (PATTR)msg_para;
    addr = OBJAttrGetAdr(pattr);

    if(addr == p_PP_MACHSET_FL_USEOPCUA)
    {
        if(VarAdrToInt(OPMODE_INDEX) != MODE_MANUAL)
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//原始值
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }
        else
        {
            ShowQuestion(VW_MSG_RESTART,g_question);
        }

    }
    return 0;
}
