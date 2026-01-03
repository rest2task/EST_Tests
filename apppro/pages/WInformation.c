#include "WInformation.h"
#include "typedef.h"
#include "panel.h"
#include "information.h"
#include "database.h"
#include "questionDlg.h"

DEFINE_INFORMATION_MAP

CONNECT_INFORMATION_EVENT

static BTFUNC* g_DoInformation = NULL;
static BOOL g_Information_button = FALSE;
void ShowInformation(int langid,BTFUNC* doInformation)
{
    g_DoInformation = doInformation;
    g_Information_button = TRUE;

    VarAdrSetInt(INFORMATION_INDEX,langid);
    PanelShowDialogByName(information_pg_name);
}

static int button8OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static void CtlConnect()
{
    WgtMsgConnect(information_pg.button8, WGT_CLICK_MSG, button8OnClick);
}
static int InformationOnInit(Information_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int InformationOnShow(Information_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    //20210914 dyl touch
    if (VarAdrToInt(INFORMATION_INDEX) == VW_MSG_AUTO_CHECKING)
    {
        SetValOfScreenCheck(TRUE);
    }
    else
    {
        SetValOfScreenCheck(FALSE);
    }

    return 0;
}
static int InformationOnHide(Information_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    g_DoInformation = NULL;
    //VarAdrSetInt(INFORMATION_INDEX,0);
    return 0;
}
static int InformationOnUpdate(Information_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int InformationOnChange(Information_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(pwgt == PageGetWgtByName(information_pg_name,"btnclose"))
    {
        PanelCloseDialogByName(information_pg_name);//20191223.cyx
        VarAdrSetInt(INFORMATION_INDEX,0);
    }
    return 0;
}
static int button8OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(g_DoInformation != NULL)
    {
        g_DoInformation();
        g_DoInformation = NULL;
    }

	PanelCloseDialogByName(information_pg_name);
    VarAdrSetInt(INFORMATION_INDEX,0);

    return 0;
}
