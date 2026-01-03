#include "WInstall_Ask.h"
#include "typedef.h"
#include "pay.h"
#include "panel.h"

DEFINE_INSTALL_ASK_MAP

CONNECT_INSTALL_ASK_EVENT

#define CMOD_WR_HEADSZ sizeof(PAYSET_HEAD)
#define CMOD_WR_ITEMSZ 0
#define CMOD_WR_PATH   SAVE_PATH"pay.dat"

extern BOOL g_bFirstMoni;
extern BOOL g_bInstallAsk;

static int OKOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int CancelOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static void CtlConnect()
{
    WgtMsgConnect(install_ask_pg.Cancel, WGT_CLICK_MSG, CancelOnClick);
    WgtMsgConnect(install_ask_pg.OK, WGT_CLICK_MSG, OKOnClick);
}
static int Install_AskOnInit(Install_Ask_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Install_AskOnShow(Install_Ask_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Install_AskOnHide(Install_Ask_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Install_AskOnUpdate(Install_Ask_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Install_AskOnChange(Install_Ask_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(pwgt == PageGetWgtByName(install_ask_pg_name,"btnclose"))
    {
        PanelCloseDialogByName(install_ask_pg_name);//20191223.cyx
    }
    return 0;
}
static int OKOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	PAYSET_HEAD head;

	g_bInstallAsk = FALSE;
	g_bFirstMoni = TRUE;
	m_operation.cNowNum = 1;
	memcpy(&head.m_operation,&m_operation,sizeof(OPERATION));
	CMOD_WHEAD_EX(&head);
    SaveCurTmLowVol();

	PanelCloseDialogByName(install_ask_pg_name);
	PanelShowPage(PanelMainPage());
	//PayMonitor();

    return 0;
}
static int CancelOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    m_operation.cState  = 0;
	PanelCloseDialogByName(install_ask_pg_name);

    return 0;
}
