#include "power.h"
#include "time.h"
#include "stdio.h"
#include "file.h"
#include "obj.h"
#include "common.h"
#include "language.h"
#include "key.h"
#include "pagefram.h"
#include "panel.h"
#include "piewgt.h"
#include "oprintf.h"

#define POWER_CYCLE         0x0016000C //本次循环总能耗
#define POWER_OTHERS        0x0016000D //本次循环其它能耗
#define POWER_CLAMPCLS      0x0016000E //本次循环关模能耗
#define POWER_CLAMPOPN      0x0016000F //本次循环开模能耗
#define POWER_INJECT        0x00160010 //本次循环射出能耗
#define POWER_CHARGE        0x00160011 //本次循环储料能耗
#define POWER_SUCKBACK      0x00160012 //本次循环射退能耗
#define POWER_EJECT         0x00160013 //本次循环托模能耗

#define POWER_FILE			SAVE_PATH"power.dat"

#define		FILE_INDEX(NUM)		(sizeof(POWER_HEAD) + sizeof(POWER_ITEM)*NUM)
static POWERRECRD m_powerrecd;

typedef enum enISUSE
{
	NOUSE=0,
	USE
}ISUSE;

//static	BOOL WriteRecord(UI16 itmenum)
//{
//	int index;
//	if(m_powerrecd.Head.cur_no>=itmenum)
//	{
//		FILEHD filehd;
//		filehd = FileOpen(POWER_FILE);
//		index = FILE_INDEX(itmenum);
//		if(FILEHD_IS_OK(filehd))
//		{
//			FileSeek(filehd,0);
//			FileWrite(filehd,&m_powerrecd,sizeof(POWER_HEAD),NULL);
//			FileSeek(filehd,index);
//			FileWrite(filehd,(char*)(&m_powerrecd) + index,sizeof(POWER_ITEM),NULL);
//			FileClose(filehd);
//			return true;
//		}
//	}
//	return false;
//}

/************************************************************************/
/* 能耗监控                                                     */
/************************************************************************/
void ProdEnergy()
{
	UI16 curno;
	m_powerrecd.Head.cur_no++;
	if(m_powerrecd.Head.cur_no >= (MAX_ITEMS + 1))
	{
		m_powerrecd.Head.cur_no = 1;
	}
	curno = m_powerrecd.Head.cur_no;
	m_powerrecd.items[curno].flag = MARK_USED;
	m_powerrecd.items[curno].total = (UI16)VarAdrToInt(POWER_CYCLE);
	m_powerrecd.items[curno].cls = (UI16)VarAdrToInt(POWER_CLAMPCLS);//POWER_OTHERS
	m_powerrecd.items[curno].open = (UI16)VarAdrToInt(POWER_CLAMPOPN);//POWER_CLAMPCLS
	m_powerrecd.items[curno].inject = (UI16)VarAdrToInt(POWER_INJECT);//POWER_CLAMPOPN
	m_powerrecd.items[curno].charge = (UI16)VarAdrToInt(POWER_CHARGE);//POWER_INJECT
	m_powerrecd.items[curno].suckB = (UI16)VarAdrToInt(POWER_SUCKBACK);//POWER_CHARGE
	m_powerrecd.items[curno].eject = (UI16)VarAdrToInt(POWER_EJECT);//POWER_SUCKBACK
	m_powerrecd.items[curno].other = (UI16)VarAdrToInt(POWER_OTHERS);//POWER_EJECT

    //WriteRecord(curno);
}

void	PowerParamInit()
{
	FILEHD filehd;
	//int index;
	filehd = FileOpen(POWER_FILE);

	if(FILEHD_IS_OK(filehd))
	{
		//index = FileGetSize(filehd,POWER_FILE);
		FileRead(filehd,&m_powerrecd,sizeof(POWERRECRD),NULL);
		FileClose(filehd);
	}
	else 
	{
		filehd = FileCreate(POWER_FILE);
		FileClose(filehd);
	}
	
}


POWER_ITEM	PowerReadRecord(UI32 index)
{
	POWER_ITEM item;
	if( index < (MAX_ITEMS + 1 ) )
	{ 
		item = m_powerrecd.items[m_powerrecd.Head.cur_no-index];//从最新开始显示
	}
	return item;
}

POWERRECRD* PowerGetRecord()
{
	return &m_powerrecd;
}

UI32 PowerGetRecordCnt()
{
    return m_powerrecd.Head.cur_no;
}
void ProdEnergySave()
{
    FILEHD filehd;
	static UI16 cur_no =0;


	if(cur_no == m_powerrecd.Head.cur_no) return;

	//oprintf("ProdEnergySave*******\r\n");
	cur_no = m_powerrecd.Head.cur_no;
    filehd = FileOpen(POWER_FILE);

    if(FILEHD_IS_OK(filehd))
    {
        FileSeek(filehd,0);
        FileWrite(filehd,&m_powerrecd,sizeof(POWERRECRD),NULL);
        FileClose(filehd);
    }
}
