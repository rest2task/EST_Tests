#include "WOperate_Record.h"
#include "typedef.h"
#include "pagefram.h"
#include "oprecord.h"
#include "file.h"
#include "editwgt.h"
#include "panelrec.h"
#include "usermod.h"//20210914 dyl touch

DEFINE_OPERATE_RECORD_MAP

CONNECT_OPERATE_RECORD_EVENT

#define		SHOW_RECORDS_COUNT	15
#define		TMP_PASSWORD	    TMPVAL(20)//password
#define		TMPADDR_START		TMPVAL(0)
#define		TMPADDR_STR			TMPSTR(0)


#define		STR_EMPTY			""
static PHASH_TBL pmap_tbl=NULL;

#define MAP_FILE			DATA_PATH"operatmoni.map"

BOOL	OPShowRecord( );
void    OPClearRecord( );

BOOL OPShowRecord()
{
    UI32 addr;
	I32 i;
    I32 value;
	OPRECORD_ITEM item;
	char strtmp[20];
	const char *ptrans;
	I32 curnum = OPGetCurRecordNum();
    UI16 nrec;
	
	addr = TMPADDR_START;
	value = VarAdrToInt(addr);
    nrec = (OP_MAX_RECORDS+curnum-value)%OP_MAX_RECORDS;

	for(i=0;i<SHOW_RECORDS_COUNT;i++)
	{
        if(nrec==0) nrec=OP_MAX_RECORDS-1;
		else nrec--;

		VarAdrSetInt(addr+i,value+i);
		item = OPReadRecord(nrec);//从最新的时间开始显示
		if(item.flag == MARK_USED /*&& (value+i)<=curnum*/)
		{
			if(pmap_tbl!=NULL)
			{
				ptrans = GetMapTran(pmap_tbl,item.opId);
                VarAdrSetStr(TMPADDR_STR+i,ptrans);
			}
			WordToStr(strtmp,item.nShot,0);
            VarAdrSetStr(TMPADDR_STR+SHOW_RECORDS_COUNT+i,strtmp);
			TimeToStr(strtmp,"yyyy-MM-dd hh:mm:ss",item.datetime);
            VarAdrSetStr(TMPADDR_STR+SHOW_RECORDS_COUNT*2+i,strtmp);
		}
		else
		{
            VarAdrSetStr(TMPADDR_STR+i,STR_EMPTY);
            VarAdrSetStr(TMPADDR_STR+SHOW_RECORDS_COUNT+i,STR_EMPTY);
            VarAdrSetStr(TMPADDR_STR+SHOW_RECORDS_COUNT*2+i,STR_EMPTY);
		}
	}
	return TRUE;

}

void    OPClearRecord( )
{
	OPResetRecord();
    VarAdrSetInt(TMPADDR_START,0);
	OPShowRecord();
}


static void CtlConnect()
{
}
static int Operate_RecordOnInit(Operate_Record_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	ParseCSVFile(MAP_FILE,&pmap_tbl);
	
    return 0;
}
static int Operate_RecordOnShow(Operate_Record_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_USERLOCK);//20210914 dyl touch

	VarAdrSetInt(TMP_PASSWORD,0);
    VarAdrSetInt(TMPADDR_START,0);
    WGTSetEnable(operate_record_pg.btClear,FALSE);
	OPShowRecord();
	
    return 0;
}
static int Operate_RecordOnHide(Operate_Record_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch
    return 0;
}
static int Operate_RecordOnUpdate(Operate_Record_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	if(OPHasChange())
	{
		OPResetChange();
		OPShowRecord();
	}
    return 0;
}
static int Operate_RecordOnChange(Operate_Record_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	UI32 addr; 
	PATTR pattr;
	UI16 value;

	pattr = (PATTR)msg_para;

	addr = OBJAttrGetAdr(pattr);

	switch (addr)
	{
	case TMPADDR_START:
		OPShowRecord();
		break;
	case TMP_PASSWORD:
		value = VarAdrToUI16(TMP_PASSWORD);
		if(value==9595)
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
            PanelRecAdd(ppg->btClear, PNL_CLEAR_ALL_REC, 0);
			OPClearRecord();
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
            OPShowRecord();
        }
        else if(pwgt == ppg->btnext)
        {
            if(VarAdrToInt(TMPADDR_START) < (OP_MAX_RECORDS-SHOW_RECORDS_COUNT*2))
            {
                VarAdrSetInt(TMPADDR_START, VarAdrToInt(TMPADDR_START)+SHOW_RECORDS_COUNT);
            }
            else
            {
                VarAdrSetInt(TMPADDR_START, OP_MAX_RECORDS-SHOW_RECORDS_COUNT);
            }
            OPShowRecord();
        }
		break;
	}
    return 0;
}
