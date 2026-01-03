#include "oprecord.h"
#include "time.h"
#include "stdio.h"
#include "file.h"
#include "obj.h"
#include "common.h"
#include "language.h"
#include "key.h"
#include "database.h"

#define     MARK_USED				0xEB90
#define     MARK_INVALID			(0)

#define		FILE_INDEX(NUM)		(sizeof(OPRECORD_HEAD) + sizeof(OPRECORD_ITEM)*NUM)

typedef enum enISUSE
{
	NOUSE=0,
	USE
}ISUSE;
static OPRECRD m_opercrd;
static BOOL m_change= FALSE;

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

static	BOOL WriteRecord(UI16 itmenum)
{
	int index;

	FILEHD filehd;
	filehd = FileOpen(OP_FILE_PATH);
	index = FILE_INDEX(itmenum);
	if(CheckFileValid(filehd))
	{
		FileSeek(filehd,0);
		FileWrite(filehd,&m_opercrd,sizeof(OPRECORD_HEAD),NULL);
		FileSeek(filehd,index);
		FileWrite(filehd,(char*)(&m_opercrd) + index,sizeof(OPRECORD_ITEM),NULL);
		FileClose(filehd);
		return true;
	}

	return false;
}

OPRECORD_ITEM	OPReadRecord(I32 index)
{
	OPRECORD_ITEM item;
	
	if( index < (OP_MAX_RECORDS) && index >=0)
	{ 
		item = m_opercrd.opitems[index];//从最新的时间开始显示m_opercrd.opHead.cur_no -index
	}
	return item;
}

void	OPParamInit()
{
	FILEHD filehd;
	filehd = FileOpen(OP_FILE_PATH);
	if(CheckFileValid(filehd))
	{
		FileRead(filehd,&m_opercrd,sizeof(OPRECRD),NULL);
		FileClose(filehd);
	}
	else 
	{
		filehd = FileCreate(OP_FILE_PATH);
		FileClose(filehd);
	}
	
}

void OPWRRecord(UI16 type)
{
	UI16 curno;
    if(VarAdrToInt(p_PP_MACHSET_RECORD_SAVE) == NOUSE && type!=OP_CLEANSHOT || type > OP_AUTOSTOP)
	{
		return;
	}

	curno = m_opercrd.opHead.cur_no;
	m_opercrd.opitems[curno].flag = MARK_USED;
    m_opercrd.opitems[curno].nShot = VarAdrToInt(CLAMP_STATE_MOLDOPNNUM0)<<16 | VarAdrToInt(CLAMP_STATE_MOLDOPNNUM1);
	m_opercrd.opitems[curno].opId = type;
	m_opercrd.opitems[curno].datetime = (UI32)time(NULL);

	m_opercrd.opHead.cur_no++;
	if(m_opercrd.opHead.cur_no >= OP_MAX_RECORDS)
	{
		m_opercrd.opHead.cur_no = 0;
	}

	WriteRecord(curno);

	m_change = TRUE;
}

/************************************************************************/
/* 该函数在LCDMonitor中调用                                       */
/************************************************************************/
void OPLEDMonitorRecord(UI16 opmode)
{

	UI16 type;
	
	switch(opmode)
	{
    case MODE_AUTO:type=OP_AUTOSTART;break;
    case MODE_SEMIAUTO:type=OP_SEMIAUTO;break; //20200115.cyx
    case MODE_MACROADJ:type=OP_ADJMOLD;break;
    case MODE_AUTOADJMOLD:type=OP_AUTOADJMOLD;break;
	case MODE_MANUAL:type=OP_MANUAL;break;
	default:type=OP_DF;break;
	}
	OPWRRecord(type);

}

OPRECRD * GetOpRecord()
{
	return &m_opercrd;
}

BOOL OPHasChange()
{	
	return m_change;
}

void OPResetChange()
{
	m_change = FALSE;
}

void OPResetRecord()
{
	int i;

	FileDelete(OP_FILE_PATH);
	OPParamInit();

	m_opercrd.opHead.cur_no =0;

	for(i=0;i<OP_MAX_RECORDS;i++)
	{
		m_opercrd.opitems[i].flag = 0;
		WriteRecord(i);
	}
}

I32 OPGetCurRecordNum()
{
	return m_opercrd.opHead.cur_no;
}

/************************************TY Operate2******************************************/
static OPRECRD m_opercrd2;
static BOOL m_change2= FALSE;
static	BOOL WriteRecord2(UI16 itmenum)
{
    int index;

    FILEHD filehd;
    filehd = FileOpen(OP_FILE_PATH2);
    index = FILE_INDEX(itmenum);
    if(CheckFileValid(filehd))
    {
        FileSeek(filehd,0);
        FileWrite(filehd,&m_opercrd2,sizeof(OPRECORD_HEAD),NULL);
        FileSeek(filehd,index);
        FileWrite(filehd,(char*)(&m_opercrd2) + index,sizeof(OPRECORD_ITEM),NULL);
        FileClose(filehd);
        return true;
    }

    return false;
}
OPRECORD_ITEM	OPReadRecord2(I32 index)
{
    OPRECORD_ITEM item;

    if( index < (OP_MAX_RECORDS) && index >=0)
    {
        item = m_opercrd2.opitems[index];//从最新的时间开始显示m_opercrd.opHead.cur_no -index
    }
    return item;
}
void	OPParamInit2()
{
    FILEHD filehd;
    filehd = FileOpen(OP_FILE_PATH2);
    if(CheckFileValid(filehd))
    {
        FileRead(filehd,&m_opercrd2,sizeof(OPRECRD),NULL);
        FileClose(filehd);
    }
    else
    {
        filehd = FileCreate(OP_FILE_PATH2);
        FileClose(filehd);
    }

}
//20190813 CHZ
void OPWRRecord2(UI16 type)
{
    UI16 curno;
    if( type!=OP_CLEANSHOT )
    {
        return;
    }

    curno = m_opercrd2.opHead.cur_no;
    m_opercrd2.opitems[curno].flag = MARK_USED;
    m_opercrd2.opitems[curno].nShot = VarAdrToInt(CLAMP_STATE_MOLDOPNNUM0)<<16 | VarAdrToInt(CLAMP_STATE_MOLDOPNNUM1);
    m_opercrd2.opitems[curno].opId = type;
    m_opercrd2.opitems[curno].datetime = (UI32)time(NULL);

    m_opercrd2.opHead.cur_no++;
    if(m_opercrd2.opHead.cur_no >= OP_MAX_RECORDS)
    {
        m_opercrd2.opHead.cur_no = 0;
    }

    WriteRecord2(curno);

    m_change2 = TRUE;
}
BOOL OPHasChange2()
{
    return m_change2;
}
void OPResetChange2()
{
    m_change2 = FALSE;
}
void OPResetRecord2()
{
    int i;

    FileDelete(OP_FILE_PATH2);
    OPParamInit2();

    m_opercrd2.opHead.cur_no =0;

    for(i=0;i<OP_MAX_RECORDS;i++)
    {
        m_opercrd2.opitems[i].flag = 0;
        WriteRecord2(i);
    }
}
I32 OPGetCurRecordNum2()
{
    return m_opercrd2.opHead.cur_no;
}
