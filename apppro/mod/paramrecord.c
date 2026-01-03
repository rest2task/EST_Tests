#include "paramrecord.h"
#include "time.h"
#include "stdio.h"
#include "file.h"
#include "obj.h"
#include "pagefram.h"
#include "common.h"
#include "panel.h"
#include "editwgt.h"
#include "dropwgt.h"
#include "verinfo.h"
#include "panelcfg.h"

static PHASH_TBL pmap_tbl_para=NULL;
static DB_PARAMRECORD m_paramrecord;

static BOOL CheckFileValid(FILEHD filehd)
{
	if(FILEHD_IS_OK(filehd))
	{
		return true;
	}
	else
	{
		FileClose(filehd);
		return false;
	}
}

static int ParamisFull(DB_PARAMRECORD* rec)
{
    if((rec->head.rear + 1) % MAX_PARA_RECORDS == rec->head.front)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static int ParamisEmpty(DB_PARAMRECORD* rec)
{
    if(rec->head.rear == rec->head.front)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static int ParamDelete(DB_PARAMRECORD* rec)
{
    if(ParamisEmpty(rec))
        return FALSE;

    rec->head.front = (rec->head.front + 1) % MAX_PARA_RECORDS;
    return TRUE;
}

static void	WriteParamRec(int index,void* src)
{
    FILEHD filehd;
    filehd = FileOpen(RECORD_FILE_PATH);
    if(CheckFileValid(filehd))
    {
        FileSeek(filehd,0);
        FileWrite(filehd,&m_paramrecord.head, sizeof(PARAMHEAD),NULL);
        FileSeek(filehd, sizeof(PARAMHEAD) + index * sizeof(PARAMITEM));
        FileWrite(filehd, src, sizeof(PARAMITEM), NULL);
        FileClose(filehd);
    }
}

static int ParamAdd(DB_PARAMRECORD* rec, PARAMITEM item)
{
    int index;
    if(ParamisFull(rec))
        ParamDelete(rec);
    rec->item[rec->head.rear] = item;
    index = rec->head.rear;
    rec->head.rear = (rec->head.rear + 1) % MAX_PARA_RECORDS;
    WriteParamRec(index, &item);
    return TRUE;
}

static int ParamNum(DB_PARAMRECORD* rec)
{
    return (rec->head.rear - rec->head.front + MAX_PARA_RECORDS) % MAX_PARA_RECORDS;
}

#include "iotnet.h"
static BOOL   SetRecord(DB_PARAMRECORD* rec,char *title,UI32 addr,UI8 npoint,UI8 base,UI32 newValue,UI32 oldValue)
{
    PARAMITEM item;
	if(rec!=NULL)
	{
        item.addr = addr;
        item.nPoint = npoint;
        item.base = base;
        item.wValue = newValue;
        item.wOldValue = oldValue;
        strncpy(item.idTitle, title, sizeof(item.idTitle) - 1);
        item.datetime = (UI32)time(NULL);

        ParamAdd(&m_paramrecord, item);

#ifndef WIN32
		IOT_SendParamRecord( addr,newValue,oldValue);
#endif
		return true;
	}
	return false;
}
void HasSaveFile();
void	ParamRecordInit()
{
	FILEHD filehd;
	HasSaveFile();
	filehd = FileOpen(RECORD_FILE_PATH);
    memset(&m_paramrecord, 0, sizeof(DB_PARAMRECORD));
    m_paramrecord.head.front = 0;
    m_paramrecord.head.rear = 0;
	if(CheckFileValid(filehd))
	{
        FileRead(filehd, &m_paramrecord, sizeof(DB_PARAMRECORD), NULL);
        FileClose(filehd);
	}
	else {
		filehd = FileCreate(RECORD_FILE_PATH);
		FileClose(filehd);
	}
	ParseCSVFile(MAP_FILE,&pmap_tbl_para);
	
}

BOOL ReadRecord(RecordItem* item,UI32 index)
{
    PARAMITEM tmpitem;
    PPAGE_FRAM ppage;
    char addrbuf[12];
    char number[12];
    if(index < ParamNum(&m_paramrecord) && !ParamisEmpty(&m_paramrecord))
    {
        tmpitem = m_paramrecord.item[(m_paramrecord.head.rear - index - 1 + MAX_PARA_RECORDS) % MAX_PARA_RECORDS];
        ppage = PanelFindPageWithLoad((const char*)(tmpitem.idTitle));
        if(ppage!=NULL)
        {
            //PVAR pvar;
            item->idTitle = PageFramTitle(ppage);//GetLanTran(ppage->langHash,ppage->text.pstr);//tmpTitle;

            if(pmap_tbl_para==NULL || (item->param = GetMapTran(pmap_tbl_para,tmpitem.addr))==NULL )
            {
                //pvar = VarAdrToVar(record.addr);
                item->param = VarAdrToVarName(tmpitem.addr);//(char const*)pvar->name;数据库查找
            }
            //当数据库文件不同时进行保护
            if(item->param !=NULL)
            {
                if(strcmp(item->param,"")==0)
                {
                    snprintf(addrbuf,sizeof(addrbuf)-1,"0x%08x",tmpitem.addr);
                    strcpy((char*)item->param, addrbuf);
                }
            }

            if(tmpitem.base == INPUT_DEC){
                WordToStr(item->oldvalue,tmpitem.wOldValue,tmpitem.nPoint);
                WordToStr(item->newvalue,tmpitem.wValue,tmpitem.nPoint);
            }
            else if(tmpitem.base == INPUT_HEX){
                snprintf(number, sizeof(number)-1, "%X", tmpitem.wOldValue);
                strcpy(item->oldvalue, number);
                snprintf(number, sizeof(number)-1, "%X", tmpitem.wValue);
                strcpy(item->newvalue, number);
            }
            else if(tmpitem.base == INPUT_TEXT){

            }
            else {
                WordToStr(item->oldvalue,tmpitem.wOldValue,tmpitem.nPoint);
                WordToStr(item->newvalue,tmpitem.wValue,tmpitem.nPoint);
            }

            TimeToStr(item->time,"yyyy-MM-dd hh:mm:ss",tmpitem.datetime);
            return TRUE;
        }
    }
    return FALSE;
}

void RecordValue(POBJ pobj,PATTR pattr,UI32 oldvalue,UI8 npoint, UI8 base)
{
	PVAR pdat;
	UI32 newvalue;

	if(pobj!=NULL && pattr != NULL && OBJAttrIsVar(pattr) && (pdat = OBJAttrGetVar(pattr)) != NULL)
	{
		if(OBJAttrGetType(pattr) != T_STR && VarChkAttrBit(pdat,VAR_IS_RECORD_BIT))
		{
			newvalue = VarToInt(pdat);
			if(newvalue != oldvalue)
			{
				PPAGE_FRAM ppage;
				ppage = GetPageByWgt(pobj);
				if(ppage!=NULL)
				{
                    SetRecord(&m_paramrecord,(char*)PageFramGetName(ppage),OBJAttrGetAdr(pattr),npoint,base,newvalue,oldvalue);
				}
			}
		}
	}
	return;
}

//手动添加参数记录
BOOL SetRecordByManual(char *title,UI32 addr,UI8 npoint,UI32 newValue,UI32 oldValue,UI8 base)
{
    SetRecord(&m_paramrecord,title,addr,npoint,base,newValue,oldValue);
}

int RecordValueEX(POBJ pobj, int oldvalue, PATTR pattr)
{
	UI8 npoint=0;
    UI8 base=0;
	if( pobj == NULL)
		return -1;
	
	switch(pobj->type)
	{
	case OBJ_EDIT:
		{
			//PEDIT_WGT pedit = WgtToEdit((PWGT)pobj);
			//npoint = RU32(pedit->dot_num);
			npoint =(UI8)EditGetDot((PWGT)pobj);
            base = (UI8)EditGetInputMode((PWGT)pobj);
		}
		break;
	case OBJ_DROPDOWN:
		{
			//PDROP_WGT pdrop = WgtToDrop((PWGT)pobj);
			npoint = 0;
		}
		break;
	default:
		break;
	}
    RecordValue(pobj, pattr, oldvalue, npoint, base);
	return 0;
}

void HasSaveFile()
{
	char scmd[64];
	if(!FileExist(SAVE_PATH))
	{
#ifdef USE_QT
		snprintf(scmd,sizeof(scmd)-1, "mkdir %s", SAVE_PATH);
#else
		snprintf(scmd,sizeof(scmd)-1, "md %s", SAVE_PATH);
#endif
		osystem(scmd);
	}
}
