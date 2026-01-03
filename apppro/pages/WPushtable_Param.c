#include "WPushtable_Param.h"
#include "typedef.h"
#include "pagefram.h"
#include "database.h"
#include "pushtab.h"
#include "questionDlg.h"
#include "privdef.h"

DEFINE_PUSHTABLE_PARAM_MAP

CONNECT_PUSHTABLE_PARAM_EVENT

#define PUSHPARA_L1         0x33120075
#define PUSHPARA_L2         0x33120076
#define PUSHPARA_L3         0x33120077
#define PUSHPARA_L4         0x33120078
#define PUSHPARA_L5         0x33120079
#define PUSHPARA_H          0x3312007A
#define PUSHPARA_h          0x3312007B
#define PUSHPARA_Hm         0x3312007C
#define PUSHPARA_LMTAG      0x3312007D
#define SHOW_LMTAG_ERROR    0x3312007E

static QUESTION g_question;

static void CtlConnect()
{
}
static int Pushtable_ParamOnInit(Pushtable_Param_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Pushtable_ParamOnShow(Pushtable_Param_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    WGTSetVisible(ppg->lblinfo,FALSE);
    g_question.pOkfunc = PanelMainRestart;
    g_question.pCancelfunc = NULL;

    if(VarAdrToInt(p_PP_MACHSET_PUSHTAB_MODE) == 1)//20200811
    {
        WGTSetVisible(ppg->subpage1, FALSE);
        WGTSetVisible(ppg->subpage2, TRUE);
    }
    else
    {
        WGTSetVisible(ppg->subpage1, TRUE);
        WGTSetVisible(ppg->subpage2, FALSE);
    }

    return 0;
}
static int Pushtable_ParamOnHide(Pushtable_Param_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Pushtable_ParamOnUpdate(Pushtable_Param_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Pushtable_ParamOnChange(Pushtable_Param_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    double L1,L2,L3,L4,L5,h,H,Hm,limitangle;

    if(WGTGetVisible(ppg->lblinfo))//20200811
    {
        WGTSetVisible(ppg->lblinfo,FALSE);
    }

    if(pwgt == ppg->subpage1_btcalcu || pwgt == ppg->subpage2_btcalcu)
    {
        L1 = ((double)(VarAdrToInt(PUSHPARA_L1)))/10000;
        L2 = ((double)(VarAdrToInt(PUSHPARA_L2)))/10000;
        L3 = ((double)(VarAdrToInt(PUSHPARA_L3)))/10000;
        L4 = ((double)(VarAdrToInt(PUSHPARA_L4)))/10000;
        L5 = ((double)(VarAdrToInt(PUSHPARA_L5)))/10000;
        H =  ((double)(VarAdrToInt(PUSHPARA_H)))/10000;
        h =  ((double)(VarAdrToInt(PUSHPARA_h)))/10000;
        Hm = ((double)(VarAdrToInt(PUSHPARA_Hm)))/10000;
        limitangle = ((double)(VarAdrToInt(PUSHPARA_LMTAG)))/10000;

        CalculateData(L1,L2,L3,L4,L5,H,h,Hm,limitangle);

        if(VarAdrToInt(SHOW_LMTAG_ERROR))
        {
            VarAdrSetInt(SHOW_LMTAG_ERROR,0);
            WGTSetVisible(ppg->lblinfo,TRUE);
        }

        SetMachineType(PushtableType());
        ShowQuestion(VW_MSG_RESTART,g_question);
    }
    else if(pwgt == ppg->cbmodesel)
    {
        if(VarAdrToInt(p_PP_MACHSET_PUSHTAB_MODE) == 1)//20200811
        {
            WGTSetVisible(ppg->subpage1, FALSE);
            WGTSetVisible(ppg->subpage2, TRUE);
        }
        else
        {
            WGTSetVisible(ppg->subpage1, TRUE);
            WGTSetVisible(ppg->subpage2, FALSE);
        }
    }

    return 0;
}
