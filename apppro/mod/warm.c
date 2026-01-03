#include "warm.h"
#include "file.h"
#include "oprintf.h" 
#include "common.h"
#include "time.h"
#include "database.h"
#include "keydef.h"

DB_WARMX  dbwarmx;

DB_WARMX   g_dbWarmxDefault = {
	0xEB90,
	{
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	{0,0,0,0,0,0}
	}
};

/************************************************************************/
/* 加热预设读取函数                                                     */
/************************************************************************/
BOOL LoadWarmSet()
{
	FILEHD filehd;
	int readbytes;

	filehd = FileOpen(WARMDB_INIT_FILE);
	if(FILEHD_IS_OK(filehd))
	{
		FileRead(filehd,&dbwarmx,sizeof(DB_WARMX),&readbytes);
		FileClose(filehd);
	}
	else {
		filehd = FileCreate(WARMDB_INIT_FILE);
		memcpy(&dbwarmx,&g_dbWarmxDefault,sizeof(DB_WARMX));
		FileWrite(filehd,&dbwarmx,sizeof(DB_WARMX),NULL);
		FileClose(filehd);
	}
	return TRUE;
}
/************************************************************************/
/* 加热预设保存函数                                                     */
/************************************************************************/
void SaveWarmSet(UI32 dst, void* src, UI16 wCount)
{
	FILEHD filehd;
	int writebytes;

	filehd = FileOpen(WARMDB_INIT_FILE);
	if(FILEHD_IS_OK(filehd))
	{
		FileSeek(filehd, dst);
		FileWrite(filehd, src, wCount, &writebytes);
	}
	FileClose(filehd);
}

/************************************************************************/
/* 加热预设监控函数                                                     */
/************************************************************************/
void WarmMonitor()
{
	static BOOL bWarm=FALSE;   //定时加热on
	static BOOL bWarmCls=FALSE;//定时加热off
	UI16 nWeek;
    UI16 heater_state;
	struct tm* t;
	time_t m_tmNow;

	time(&m_tmNow);
	t = localtime(&m_tmNow);

	if(t == NULL)
		return;

	nWeek=t->tm_wday;
	if(nWeek==7) nWeek=0;

    //20200323 电热状态地址切换
    if(IV3200 || IV5200 || IV3300 || IV5300 || IV3100)
    {
        heater_state = VarAdrToUI16(d_TempCtrlComm1_STATE_FL_HEATERSTATUS);//0E160038 E502  E506
    }
    else
    {
        heater_state = VarAdrToUI16(d_tempctrl1_STATE_FL_HEATERSTATUS); //0516002B
    }

    if(heater_state == 0)
	{
		if(dbwarmx.dbwarmconfig[nWeek].bWarm == 1 && dbwarmx.dbwarmconfig[nWeek].nHour == t->tm_hour && dbwarmx.dbwarmconfig[nWeek].nMinute == t->tm_min)
		{
			if(!bWarm)
			{
				SendKey(_KEY_PANEL_HEATER);
				SendKey(_KEY_NULL);
				bWarm = TRUE;
			}
		}
		else
		{
			bWarm = FALSE;
		}
	}
	else
	{
		if(dbwarmx.dbwarmconfig[nWeek].bWarmCls == 1 && dbwarmx.dbwarmconfig[nWeek].nClsHour == t->tm_hour && dbwarmx.dbwarmconfig[nWeek].nClsMin == t->tm_min)
		{
			if(!bWarmCls)
			{
				SendKey(_KEY_PANEL_HEATER);
				SendKey(_KEY_NULL);
				bWarmCls = TRUE;
			}
		}
		else
		{
			bWarmCls = FALSE;
		}
	}
}


