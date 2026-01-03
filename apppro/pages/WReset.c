#include "WReset.h"
#include "typedef.h"
#include "database.h"
#include "protcmd.h"
#include "tablectl.h"
#include "machcfg.h"
#include "panel.h"
#include "information.h"

DEFINE_RESET_MAP

CONNECT_RESET_EVENT

static int OutportOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int ResetOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int CancelOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static void CtlConnect()
{
    WgtMsgConnect(reset_pg.Cancel, WGT_CLICK_MSG, CancelOnClick);
    WgtMsgConnect(reset_pg.Reset, WGT_CLICK_MSG, ResetOnClick);
    WgtMsgConnect(reset_pg.Outport, WGT_CLICK_MSG, OutportOnClick);
}
static int ResetOnInit(Reset_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int ResetOnShow(Reset_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int ResetOnHide(Reset_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int ResetOnUpdate(Reset_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int ResetOnChange(Reset_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(pwgt == PageGetWgtByName(reset_pg_name,"btnclose"))
    {
        PanelCloseDialogByName(reset_pg_name);//20191223.cyx
    }
    return 0;
}
static int OutportOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	MachCfgExport();

	PanelSetJumpFlag(FALSE);//跳转刷新标记
	PanelCloseDialogByName(reset_pg_name);

	if(check_usb())
	{
		ShowMsg(VW_MSG_UDISKNOTEXIST);
		return 0;
	}
	else
	{
        PanelShowDialogByName(DIALOG_WRITEUDISK);
		VarAdrSetInt(UDISK_WR_PRIVILEGE,3); //导入导出 权限
	}

    return 0;
}
static int ResetOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	PanelCloseDialogByName(reset_pg_name);
	MachCfgReset();

    return 0;
}
static int CancelOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	PanelCloseDialogByName(reset_pg_name);

    return 0;
}
