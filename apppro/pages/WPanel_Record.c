#include "WPanel_Record.h"
#include "typedef.h"
#include "pagefram.h"
#include "panelrec.h"
#include "editwgt.h"
#include "buttonwgt.h"
#include "vardb.h"
#include "stdio.h"
#include "labelwgt.h"
#include "mod.h"

DEFINE_PANEL_RECORD_MAP

CONNECT_PANEL_RECORD_EVENT

#define		SHOW_RECORDS_COUNT  15
#define		TMPADDR_START		TMPVAL(0)
#define		TITLE_STR			TMPSTR(11)
#define		DATA_STR			TMPSTR(31)
#define		TIME_STR			TMPSTR(51)
#define		STR_EMPTY			""

#define     TMP_PASSWORD   TMPVAL(30)
#define     PANEL_REC_FILE  DATA_PATH"panel_rec.map"
PHASH_TBL   ptbl_panel_cmd = NULL;

BOOL ShowPanelRec()
{
    UI32 addr;
    UI16 i;
    UI32 value;
    char str[100];
    const char *ptrans1;
    const char *ptrans2;

    PANELITEM item;

    addr = TMPADDR_START;
    value = VarAdrToInt(addr);
    for(i=0;i<SHOW_RECORDS_COUNT;i++)
    {
        VarAdrSetInt(addr+i,value+i);
        if(ReadPanelRec(&item,value+i))
        {
            VarAdrSetStr(TITLE_STR+i, (const char*)item.idTitle);
            if(item.data2 == 0){
                VarAdrSetStr(DATA_STR+i, (const char*)GetMapTran(ptbl_panel_cmd, item.data1));
            }
            else{
                ptrans1 = GetMapTran(ptbl_panel_cmd, item.data1);
                ptrans2 = GetMapTran(ptbl_panel_cmd, item.data2);
                if(ptrans1 != NULL && ptrans2 != NULL){
                    strncpy(str, ptrans1, sizeof(str)-1);
                    strncat(str, ptrans2, sizeof(str)-strlen(str)-1);
                    VarAdrSetStr(DATA_STR+i, str);
                }
                else{
                    VarAdrSetStr(DATA_STR+i, STR_EMPTY);
                }
            }
            VarAdrSetStr(TIME_STR+i,(const char*)item.time);
        }
        else
        {
            VarAdrSetStr(TIME_STR+i,STR_EMPTY);
            VarAdrSetStr(DATA_STR+i,STR_EMPTY);
            VarAdrSetStr(TITLE_STR+i,STR_EMPTY);
        }
    }
    return TRUE;
}

static void CtlConnect()
{
}
static int Panel_RecordOnInit(Panel_Record_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    ParseCSVFile(PANEL_REC_FILE, &ptbl_panel_cmd);
    return 0;
}
static int Panel_RecordOnShow(Panel_Record_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    VarAdrSetInt(TMPADDR_START,0);
    VarAdrSetInt(TMP_PASSWORD,0);
    WGTSetEnable(ppg->btClear, FALSE);
    ShowPanelRec();
    return 0;
}
static int Panel_RecordOnHide(Panel_Record_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Panel_RecordOnUpdate(Panel_Record_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(PanelRecIsChg())
        ShowPanelRec();
    return 0;
}
static int Panel_RecordOnChange(Panel_Record_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 addr;
    PATTR pattr;

    pattr = (PATTR)msg_para;

    addr = OBJAttrGetAdr(pattr);

    switch (addr)
    {
    case TMPADDR_START:
        ShowPanelRec();
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
            ClearPanelRec();
            ShowPanelRec();
            PanelRecAdd(ppg->btClear, PNL_CLEAR_ALL_REC, 0);//20230518 chj
        }
        else if(pwgt == ppg->btlast)
        {
            if(VarAdrToInt(TMPADDR_START) > SHOW_RECORDS_COUNT)
            {
                VarAdrSetInt(TMPADDR_START, VarAdrToInt(TMPADDR_START)-SHOW_RECORDS_COUNT);
            }
            else
            {
                VarAdrSetInt(TMPADDR_START, 0);
            }
            ShowPanelRec();
        }
        else if(pwgt == ppg->btnext)
        {
            if(VarAdrToInt(TMPADDR_START) < (MAX_PANEL_SIZE-1-SHOW_RECORDS_COUNT*2))
            {
                VarAdrSetInt(TMPADDR_START, VarAdrToInt(TMPADDR_START)+SHOW_RECORDS_COUNT);
            }
            else
            {
                VarAdrSetInt(TMPADDR_START, MAX_PANEL_SIZE-1-SHOW_RECORDS_COUNT);
            }
            ShowPanelRec();
        }
        break;
    }
    return 0;
}
