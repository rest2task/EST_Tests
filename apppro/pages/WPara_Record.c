#include "WPara_Record.h"
#include "typedef.h"
#include "pagefram.h"
#include "paramrecord.h"
#include "file.h"
#include "kjump.h"
#include "panelrec.h"
#include "usermod.h"//20210914 dyl touch

DEFINE_PARA_RECORD_MAP

CONNECT_PARA_RECORD_EVENT

#define		SHOW_RECORDS_COUNT  15
#define		TMPADDR_STARTNAME	"p.PP.TMP.TMPVAL0"
#define		TMPADDR_START		TMPVAL(0)
#define		TMPADDR_STR			TMPSTR(11)
#define		STR_EMPTY			""

#define     TMP_PASSWORD   TMPVAL(20)

BOOL ShowRecord()
{
	UI32 addr,strAddr;
	UI32 i;
    UI32 value;
	RecordItem item;

	addr = TMPADDR_START;
	strAddr = TMPADDR_STR;
	value = VarAdrToInt(addr);
	for(i=0;i<SHOW_RECORDS_COUNT;i++)
	{
		VarAdrSetInt(addr+i,value+i);
        if(ReadRecord(&item,value+i))
		{
			VarAdrSetStr(strAddr+i+SHOW_RECORDS_COUNT*0,(const char*)item.idTitle);
			VarAdrSetStr(strAddr+i+SHOW_RECORDS_COUNT*1,(const char*)item.param);
			VarAdrSetStr(strAddr+i+SHOW_RECORDS_COUNT*2,(const char*)item.oldvalue);
			VarAdrSetStr(strAddr+i+SHOW_RECORDS_COUNT*3,(const char*)item.newvalue);
			VarAdrSetStr(strAddr+i+SHOW_RECORDS_COUNT*4,(const char*)item.time);
		}
		else
		{
			VarAdrSetStr(strAddr+i+SHOW_RECORDS_COUNT*0,STR_EMPTY);
			VarAdrSetStr(strAddr+i+SHOW_RECORDS_COUNT*1,STR_EMPTY);
			VarAdrSetStr(strAddr+i+SHOW_RECORDS_COUNT*2,STR_EMPTY);
			VarAdrSetStr(strAddr+i+SHOW_RECORDS_COUNT*3,STR_EMPTY);
			VarAdrSetStr(strAddr+i+SHOW_RECORDS_COUNT*4,STR_EMPTY);
		}
	}
	return TRUE;
}

BOOL    ClearParamRecord( )
{
	FileDelete(RECORD_FILE_PATH);
	ParamRecordInit();
	return ShowRecord();
}

static void CtlConnect()
{
}
static int Para_RecordOnInit(Para_Record_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	
	return 0;
}
static int Para_RecordOnShow(Para_Record_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_USERLOCK);//20210914 dyl touch

    G10ParaRecordGuideSwitch();

    VarAdrSetInt(TMPADDR_START,0);
	VarAdrSetInt(TMP_PASSWORD,0);
    WGTSetEnable(ppg->btClear, FALSE);
	ShowRecord();
	
	return 0;
}
static int Para_RecordOnHide(Para_Record_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch
	return 0;
}
static int Para_RecordOnUpdate(Para_Record_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	return 0;
}
static int Para_RecordOnChange(Para_Record_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	UI32 addr; 
	PATTR pattr;

	pattr = (PATTR)msg_para;

	addr = OBJAttrGetAdr(pattr);

	switch (addr)
	{
	case TMPADDR_START:
		ShowRecord();
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
		if(pwgt == para_record_pg.btClear)
		{
            PanelRecAdd(ppg->btClear, PNL_CLEAR_ALL_REC, 0);
			ClearParamRecord();
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
            ShowRecord();
        }
        else if(pwgt == ppg->btnext)
        {
            if(VarAdrToInt(TMPADDR_START) < (MAX_PARA_RECORDS-1-SHOW_RECORDS_COUNT*2))
            {
                VarAdrSetInt(TMPADDR_START, VarAdrToInt(TMPADDR_START)+SHOW_RECORDS_COUNT);
            }
            else
            {
                VarAdrSetInt(TMPADDR_START, MAX_PARA_RECORDS-1-SHOW_RECORDS_COUNT);
            }
            ShowRecord();
        }
		break;
	}
	return 0;
}
