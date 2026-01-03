#include "WWarnData_Rec.h"
#include "typedef.h"
#include "pagefram.h"
#include "panel.h"
#include "vardb.h"
#include "warndatarec.h"
#include "usermod.h"
#include "stdio.h"
#include "alarmrecord.h"
#include "string.h"
#include "privdef.h"
#include "texttbl.h"
#include "information.h"
#include "oprecord.h"
#include "database.h"
#include "panelrec.h"
#include "usermod.h"//20210914 dyl touch

DEFINE_WARNDATA_REC_MAP

CONNECT_WARNDATA_REC_EVENT


#define		SHOW_RECORDS_COUNT   (30)
#define		TMPADDR_START		TMPVAL(100)
#define		TMPADDR_STR			TMPSTR(111)
#define		STR_EMPTY			""

#define		TMP_PASSWORD	    TMPVAL(199)//password   临时变量199

BOOL ShowWDRec()
{
    UI32 addr,strAddr;
    UI32 i;
    UI32 value;
    WDITEM item;
    UI32 warnId;
    char phy[100];
    char buf[30];
    const char* ptrans;

    addr = TMPADDR_START;
    strAddr = TMPADDR_STR;
    value = VarAdrToInt(addr);
    for(i=0;i<SHOW_RECORDS_COUNT;i++)
    {
        VarAdrSetInt(addr+i,value+i);
        if(ReadWDRec(&item,value+i))
        {
            warnId = (item.WarnData.wCurrent_Err1<<16) | item.WarnData.wCurrent_Err0;
            ptrans = GetMapTran(pmap_alarm,warnId);
            if(ptrans != NULL){
                strcpy(phy, ptrans);
                VarAdrSetStr(strAddr+i+SHOW_RECORDS_COUNT*0,(const char*)phy);
            }
            else
            {
                snprintf(phy, sizeof(phy)-1, "#%x", warnId);
                VarAdrSetStr(strAddr+i+SHOW_RECORDS_COUNT*0,(const char*)phy);
            }
        }
        else
        {
            VarAdrSetStr(strAddr+i+SHOW_RECORDS_COUNT*0,STR_EMPTY);
        }
    }
    return TRUE;
}
static void CtlConnect()
{
}
static int WarnData_RecOnInit(WarnData_Rec_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int WarnData_RecOnShow(WarnData_Rec_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_USERLOCK);//20210914 dyl touch

    VarAdrSetInt(TMPADDR_START,0);
    VarAdrSetInt(TMP_PASSWORD,0);//密码进入为0
    WGTSetEnable(ppg->btClear, FALSE);

    ShowWDRec();
    return 0;
}
static int WarnData_RecOnHide(WarnData_Rec_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch
    return 0;
}
static int WarnData_RecOnUpdate(WarnData_Rec_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(WDRecIsChg())
        ShowWDRec();
    return 0;
}
static int WarnData_RecOnChange(WarnData_Rec_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 addr, goff;
    PATTR pattr;

    pattr = (PATTR)msg_para;

    addr = OBJAttrGetAdr(pattr);
    goff = WGTGetGOffset(pwgt);

    if(goff >= 1 && goff <= 30)
    {
        PanelShowDialogByName(DIALOG_WARNHELP);
        SetCurWarnId(VarAdrToUI16(TMPADDR_START)+goff-1);
    }

    if(pwgt == ppg->button21)
    {
        if(VarAdrToUI16(OPMODE_INDEX)!=MODE_MANUAL)
        {
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }
        if(check_usb())
        {
           ShowMsg(VW_MSG_UDISKNOTEXIST);
           return -1;
        }
        else
        {
            SetWarnDataFlg(TRUE);
            PanelShowDialogByName(DIALOG_SAVEEXCEL);
        }
    }

    switch (addr)
    {
    case TMPADDR_START:
        ShowWDRec();
        break;
    case TMP_PASSWORD:
        if (VarAdrToInt(TMP_PASSWORD) == 9595)
        {
            WGTSetEnable(ppg->btClear, TRUE);
        }
        else
        {
            WGTSetEnable(ppg->btClear, FALSE);
        }
        break;
    default :
        if(pwgt == ppg->btClear)
        {
            PanelRecAdd(pwgt, PNL_CLEAR_ALL_REC, 0);
            ClearWDRec();
            ShowWDRec();
        }
        break;
    }

    return 0;
}
