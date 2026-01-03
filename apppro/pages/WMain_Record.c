#include "WMain_Record.h"
#include "editwgt.h"
#include "buttonwgt.h"
#include "typedef.h"
#include "pagefram.h"
#include "cmdrec.h"
#include "vardb.h"
#include "stdio.h"
#include "labelwgt.h"
#include "panelrec.h"

DEFINE_MAIN_RECORD_MAP

CONNECT_MAIN_RECORD_EVENT

#define		SHOW_RECORDS_COUNT  15
#define		TMPADDR_START		TMPVAL(0)
#define     OPEN_CNT            TMPSTR(56)
#define		OPERATION_STR		TMPSTR(11)
#define		CMD_STR             TMPSTR(26)
#define		STATE_STR			TMPVAL(161)
#define		TIME_STR			TMPSTR(41)
#define		DATA_STR			TMPSTR(90)
#define		STR_EMPTY			""

#define     TMP_PASSWORD   TMPVAL(30)
#define     MAIN_CMD_FILE  DATA_PATH"main_cmd.map"
PHASH_TBL   ptbl_main_cmd = NULL;

BOOL ShowCmdRec()
{
    UI32 addr;
    UI16 i;
    UI32 value;
    char cmd_str[11];

    CMDITEM item;

    addr = TMPADDR_START;
    value = VarAdrToInt(addr);
    for(i=0;i<SHOW_RECORDS_COUNT;i++)
    {
        VarAdrSetInt(addr+i,value+i);
        if(ReadCmdRec(&item,value+i))
        {
            snprintf(cmd_str, sizeof(cmd_str), "%d", item.open_cnt);
            VarAdrSetStr(OPEN_CNT+i, cmd_str);
            snprintf(cmd_str, sizeof(cmd_str), "0x%08x", item.cmd);
            VarAdrSetStr(CMD_STR+i, (const char*)cmd_str);
            VarAdrSetStr(TIME_STR+i,(const char*)item.time);
            if(GetMapTran(ptbl_main_cmd, item.cmd) != NULL)
            {
                VarAdrSetStr(OPERATION_STR+i, (const char*)GetMapTran(ptbl_main_cmd, item.cmd));
            }
            else
            {
                VarAdrSetStr(OPERATION_STR+i, (const char*)cmd_str);
            }
            VarAdrSetInt(STATE_STR+i, item.state);
            VarAdrSetStr(DATA_STR+i, (const char*)item.data);
        }
        else
        {
            VarAdrSetStr(OPEN_CNT+i, STR_EMPTY);
            VarAdrSetStr(OPERATION_STR+i,STR_EMPTY);
            VarAdrSetStr(CMD_STR+i,STR_EMPTY);
            VarAdrSetInt(STATE_STR+i,20);
            VarAdrSetStr(TIME_STR+i,STR_EMPTY);
            VarAdrSetStr(DATA_STR+i,STR_EMPTY);
        }
    }
    return TRUE;
}

static void DisplayDataInfo()
{
    UI32 index;
    UI32 i;
    PWGT pwgt;
    pwgt = PageFrameCurFocusWgt();
    index = WGTGetGOffset(pwgt);
    if(index > SHOW_RECORDS_COUNT || index < 1)
    {
        for(i = 0; i < SHOW_RECORDS_COUNT; ++i)
        {
            pwgt = PageFramCurWgtByIndex(i+1+SHOW_RECORDS_COUNT);
            EditSetBKColor(pwgt, 0xffffff);
        }
        LabelSetText(main_record_pg.lblinfo,STR_EMPTY);
        LabelSetText(main_record_pg.data_info,STR_EMPTY);
        return;
    }

    LabelSetText(main_record_pg.lblinfo,VarAdrToStr(OPERATION_STR+index-1));
    LabelSetText(main_record_pg.data_info,VarAdrToStr(DATA_STR+index-1));

    for(i = 0;i < SHOW_RECORDS_COUNT;i++)
    {
        if(index == i+1)
        {
            pwgt = PageFramCurWgtByIndex(index + SHOW_RECORDS_COUNT);
            EditSetBKColor(pwgt, ConvToQtColor(0xe1d700));
        }
        else
        {
            pwgt = PageFramCurWgtByIndex(i+1+SHOW_RECORDS_COUNT);
            EditSetBKColor(pwgt, 0xffffff);
        }
    }
}

static void CtlConnect()
{
}
static int Main_RecordOnInit(Main_Record_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    ParseCSVFile(MAIN_CMD_FILE, &ptbl_main_cmd);
    return 0;
}
static int Main_RecordOnShow(Main_Record_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    VarAdrSetInt(TMPADDR_START,0);
    VarAdrSetInt(TMP_PASSWORD,0);
    LabelSetText(ppg->data_info, "");
    WGTSetEnable(ppg->btClear, FALSE);
    ShowCmdRec();
    return 0;
}
static int Main_RecordOnHide(Main_Record_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Main_RecordOnUpdate(Main_Record_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    DisplayDataInfo();
    if(CmdRecIsChg())
        ShowCmdRec();
    return 0;
}
static int Main_RecordOnChange(Main_Record_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 addr;
    PATTR pattr;

    pattr = (PATTR)msg_para;

    addr = OBJAttrGetAdr(pattr);

    switch (addr)
    {
    case TMPADDR_START:
        ShowCmdRec();
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
        if(pwgt == main_record_pg.btClear)
        {
            PanelRecAdd(ppg->btClear, PNL_CLEAR_ALL_REC, 0);
            ClearCmdRec();
            ShowCmdRec();
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
            ShowCmdRec();
        }
        else if(pwgt == ppg->btnext)
        {
            if(VarAdrToInt(TMPADDR_START) < (MAX_CMD_SIZE-1-SHOW_RECORDS_COUNT*2))
            {
                VarAdrSetInt(TMPADDR_START, VarAdrToInt(TMPADDR_START)+SHOW_RECORDS_COUNT);
            }
            else
            {
                VarAdrSetInt(TMPADDR_START, MAX_CMD_SIZE-1-SHOW_RECORDS_COUNT);
            }
            ShowCmdRec();
        }
        break;
    }
    return 0;
}
