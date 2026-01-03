#include "WSaveExcel_Diag.h"
#include "typedef.h"
#include "pagefram.h"
#include "editwgt.h"
#include "panel.h"
#include "information.h"
#include "prodmoni.h"
#include "oprintf.h"
#include "warndatarec.h"

DEFINE_SAVEEXCEL_DIAG_MAP

CONNECT_SAVEEXCEL_DIAG_EVENT

#define SAVE_EXCEL_FILE_NAME  0x331700D6//保存成品监控数据文件名

static int btOkOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int btCancelOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static void CtlConnect()
{
    WgtMsgConnect(saveexcel_diag_pg.btCancel, WGT_CLICK_MSG, btCancelOnClick);
    WgtMsgConnect(saveexcel_diag_pg.btOk, WGT_CLICK_MSG, btOkOnClick);
}
static int SaveExcel_DiagOnInit(SaveExcel_Diag_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int SaveExcel_DiagOnShow(SaveExcel_Diag_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    VarAdrSetStr(SAVE_EXCEL_FILE_NAME,"");
    return 0;
}
static int SaveExcel_DiagOnHide(SaveExcel_Diag_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int SaveExcel_DiagOnUpdate(SaveExcel_Diag_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int SaveExcel_DiagOnChange(SaveExcel_Diag_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(pwgt == ppg->btCancel)
    {
        PanelCloseDialogByName(saveexcel_diag_pg_name);
    }
    return 0;
}
static int btOkOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(strlen(VarAdrToStr(SAVE_EXCEL_FILE_NAME)) == 0)
    {
        PanelCloseDialogByName(saveexcel_diag_pg_name);
        ShowMsg(VM_MSG_PLSINPUTFILENAME);
        return -1;
    }

    if(check_usb())
    {
       ShowMsg(VW_MSG_UDISKNOTEXIST);
    }
    else
    {
        char* pstr;
        int i,j;
        char invalid[9] = {'<','>','/','\\','|',':','"','*','?'};
        pstr  = VarAdrToStr(SAVE_EXCEL_FILE_NAME);
        for(i = 0;i < strlen(pstr);i++)
        {
            for(j = 0;j < 9;j++)
            {
                if(*(pstr+i) == invalid[j])
                {
                    ShowMsg(VW_MSG_FILENAMEERROR);
                    return 0;
                }
            }
        }

        if(GetWarnDataFlg() == TRUE)
        {
            if(SaveWarnDataExcel(VarAdrToStr(SAVE_EXCEL_FILE_NAME)))
            {
                PanelCloseDialogByName(saveexcel_diag_pg_name);
                ShowMsg(VW_MSG_UDISKSAVESUCCESSX);
            }
            else
            {
                PanelCloseDialogByName(saveexcel_diag_pg_name);
                ShowMsg(VW_MSG_UDISKSAVEFAIL);
            }
        }
    }
    return 0;
}
static int btCancelOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    PanelCloseDialogByName(saveexcel_diag_pg_name);
    return 0;
}
