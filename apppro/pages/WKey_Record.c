#include "WKey_Record.h"
#include "typedef.h"
#include "pagefram.h"
#include "vardb.h"
#include "keyrec.h"
#include "stdio.h"
#include "texttbl.h"
#include "panelrec.h"

DEFINE_KEY_RECORD_MAP

CONNECT_KEY_RECORD_EVENT


#define		SHOW_RECORDS_COUNT  15
#define		TMPADDR_START		TMPVAL(0)
#define		TMPADDR_STR			TMPSTR(11)
#define     OP_STATE            TMPVAL(161)
#define     OPEN_CNT            TMPSTR(71)
#define		STR_EMPTY			""

#define     TMP_PASSWORD   TMPVAL(20)

BOOL ShowKeyRec()
{
    UI32 addr,strAddr;
    UI32 i;
    UI32 value;
    UI32 phyvalue;
    KEYITEM item;
    char phy[25];
    char send[20];
    const char* ptr;
    char cmd_str[11];

    addr = TMPADDR_START;
    strAddr = TMPADDR_STR;
    value = VarAdrToInt(addr);
    for(i=0;i<SHOW_RECORDS_COUNT;i++)
    {
        VarAdrSetInt(addr+i,value+i);
        if(ReadKeyRec(&item,value+i))
        {
            snprintf(send, sizeof(send)-1, "%x", item.sendvalue);
            snprintf(phy, sizeof(phy)-1, "%x", item.phykey);
            if(lan_keymap != NULL)
            {
                sscanf(phy,"%X",&phyvalue);
                ptr = GetMapTran(lan_keymap, phyvalue);
                if(ptr != NULL)
                {
                    strcat(phy, "-");
                    strcat(phy, ptr);
                }
            }
            //oprintf("1:%x, 2:%x, 3:%s, 4:%s\n", item.key, item.sendvalue, item.state, item.time);

            VarAdrSetStr(strAddr+i+SHOW_RECORDS_COUNT*0,(const char*)phy);
            VarAdrSetStr(strAddr+i+SHOW_RECORDS_COUNT*1,(const char*)send);
            VarAdrSetStr(strAddr+i+SHOW_RECORDS_COUNT*2,GetTextTran(item.state));
            VarAdrSetStr(strAddr+i+SHOW_RECORDS_COUNT*3,(const char*)item.time);
            snprintf(cmd_str, sizeof(cmd_str), "%d", item.open_cnt);
            VarAdrSetStr(OPEN_CNT+i, cmd_str);
            VarAdrSetInt(OP_STATE+i, item.op_state);
        }
        else
        {
            VarAdrSetStr(strAddr+i+SHOW_RECORDS_COUNT*0,STR_EMPTY);
            VarAdrSetStr(strAddr+i+SHOW_RECORDS_COUNT*1,STR_EMPTY);
            VarAdrSetStr(strAddr+i+SHOW_RECORDS_COUNT*2,STR_EMPTY);
            VarAdrSetStr(strAddr+i+SHOW_RECORDS_COUNT*3,STR_EMPTY);
            VarAdrSetStr(OPEN_CNT+i, STR_EMPTY);
            VarAdrSetInt(OP_STATE+i, 20);
        }
    }
    return TRUE;
}
static void CtlConnect()
{
}
static int Key_RecordOnInit(Key_Record_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Key_RecordOnShow(Key_Record_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    VarAdrSetInt(TMPADDR_START,0);
    VarAdrSetInt(TMP_PASSWORD,0);
    WGTSetEnable(ppg->btClear, FALSE);
    ShowKeyRec();
    return 0;
}
static int Key_RecordOnHide(Key_Record_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Key_RecordOnUpdate(Key_Record_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(KeyRecIsChg())
        ShowKeyRec();
    return 0;
}
static int Key_RecordOnChange(Key_Record_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 addr;
    PATTR pattr;

    pattr = (PATTR)msg_para;

    addr = OBJAttrGetAdr(pattr);

    switch (addr)
    {
    case TMPADDR_START:
        ShowKeyRec();
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
        if(pwgt == key_record_pg.btClear)
        {
            PanelRecAdd(ppg->btClear, PNL_CLEAR_ALL_REC, 0);
            ClearKeyRec();
            ShowKeyRec();
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
            ShowKeyRec();
        }
        else if(pwgt == ppg->btnext)
        {
            if(VarAdrToInt(TMPADDR_START) < (MAX_KEY_SIZE-1-SHOW_RECORDS_COUNT*2))
            {
                VarAdrSetInt(TMPADDR_START, VarAdrToInt(TMPADDR_START)+SHOW_RECORDS_COUNT);
            }
            else
            {
                VarAdrSetInt(TMPADDR_START, MAX_KEY_SIZE-1-SHOW_RECORDS_COUNT);
            }
            ShowKeyRec();
        }
        break;
    }
    return 0;
}
