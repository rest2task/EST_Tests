#include "alarmrecord_G10.h"
#include "alarmrecord.h"
#include "time.h"
#include "file.h"
#include "common.h"
#include "language.h"
#include "monmcu.h"
#include "prot.h"
#include "oprintf.h"
#include "database.h"
#include "command.h"

#define     MARK_USED				0xEB90
//
#define		FILE_INDEX(NUM)		(sizeof(DB_G10_WARNRECOD_HEAD) + sizeof(DB_G10_WARNRECORD)*NUM)

#define     SUBPAR(ADDR,SUBID) (((ADDR) & 0xFF0FFFFF) | ((SUBID) << 20)) //20200413

G10_WARNRECD m_g10_warnrecd;
extern BOOL  g_bwarn;

static UI32   filter_alarm[]={
        0x31100103,
        0x23100101,0x23200101,0x23300101,0x23400101,0x23500101,0x23600101,0x23700101,
        0x23800101,0x23900101,0x23a00101,0x23b00101,0x23c00101,0x23d00101,0x23e00101,

        //20241218 chj 新增警报过滤 顺序阀
        0x23100102,0x23200102,0x23300102,0x23400102,0x23500102,0x23600102,0x23700102,
        0x23800102,0x23900102,0x23a00102,0x23b00102,0x23c00102,0x23d00102,0x23e00102,

        //20241218 chj 新增警报过滤 胶口针阀
        0x23100103,0x23200103,0x23300103,0x23400103,0x23500103,0x23600103,0x23700103,
        0x23800103,0x23900103,0x23a00103,0x23b00103,0x23c00103,0x23d00103,0x23e00103,

        //20241218 chj 新增警报过滤 吹气15-28
        0x23100104,0x23200104,0x23300104,0x23400104,0x23500104,0x23600104,0x23700104,
        0x23800104,0x23900104,0x23a00104,0x23b00104,0x23c00104,0x23d00104,0x23e00104,

        0x20100107,0x21100101,0x29100101,0x29100102,

        //20241218 chj 新增警报过滤
        0x22100103,0x22100104,0x22200103,0x22200104,0x22300103,0x22300104,0x22400103,0x22400104,
        0x22500103,0x22500104,0x22600103,0x22600104,0x31100105,0x31100106,0x31100107,0x31100108,
        0x31100109,0x3110010A,0x3110010B,0x31100209,
        //20241218 chj 新增警报过滤
        0x2010020A
};

static UI32   filter_alarmsave[]={
    0x23100101,0x23200101,0x23300101,0x23400101,0x23500101,0x23600101,0x23700101,
    0x23800101,0x23900101,0x23a00101,0x23b00101,0x23c00101,0x23d00101,0x23e00101,

    //20241218 chj 新增警报过滤 顺序阀
    0x23100102,0x23200102,0x23300102,0x23400102,0x23500102,0x23600102,0x23700102,
    0x23800102,0x23900102,0x23a00102,0x23b00102,0x23c00102,0x23d00102,0x23e00102,

    //20241218 chj 新增警报过滤 胶口针阀
    0x23100103,0x23200103,0x23300103,0x23400103,0x23500103,0x23600103,0x23700103,
    0x23800103,0x23900103,0x23a00103,0x23b00103,0x23c00103,0x23d00103,0x23e00103,

    //20241218 chj 新增警报过滤 吹气15-28
    0x23100104,0x23200104,0x23300104,0x23400104,0x23500104,0x23600104,0x23700104,
    0x23800104,0x23900104,0x23a00104,0x23b00104,0x23c00104,0x23d00104,0x23e00104,

    0x31100103,0x29100101,0x29100102,0x21100101,
    //20201027.cyx
    0x31100105,0x31100106,0x31100107,0x31100108,0x31100109,0x3110010A,0x3110010B,
    0x22100103,0x22100104,0x22200103,0x22200104,0x22300103,0x22300104,0x22400103,
    0x22400104,0x22500103,0x22500104,0x22600103,0x22600104,
    //20241218 chj 新增警报过滤
    0x31100209,0x2010020A
};

static BOOL CheckFileValid(FILEHD filehd)
{
	if(FILEHD_IS_OK(filehd))
	{
		return TRUE;
	}
	else
	{
		FileClose(filehd);
		return FALSE;
	}
}

static BOOL G10_WriteRecord(UI16 itmenum)
{
	int index;

	FILEHD filehd;
    filehd = FileOpen(G10_ALARM_WR_PATH);
	index = FILE_INDEX(itmenum);
	if(CheckFileValid(filehd))
	{
		FileSeek(filehd,0);
        FileWrite(filehd,&m_g10_warnrecd,sizeof(DB_G10_WARNRECOD_HEAD),NULL);
		FileSeek(filehd,index);
        FileWrite(filehd,(char*)(&m_g10_warnrecd) + index,sizeof(DB_G10_WARNRECORD),NULL);
		FileClose(filehd);
		return TRUE;
	}
	return FALSE;
}

DB_G10_WARNRECORD G10_AlarmReadRecord(UI32 index)
{
    DB_G10_WARNRECORD item;
    if( index < G10_MAX_RECORDS)
	{
        item = m_g10_warnrecd.warnitems[index];
	}
	return item;
}

void G10_LoadAlarmRecord()
{
	FILEHD filehd;

    filehd = FileOpen(G10_ALARM_WR_PATH);

	if(CheckFileValid(filehd))
	{
        FileRead(filehd,&m_g10_warnrecd,sizeof(G10_WARNRECD),NULL);
		FileClose(filehd);
	}
	else 
	{
        filehd = FileCreate(G10_ALARM_WR_PATH);
		FileClose(filehd);
	}
}

#define MAX_FIND_NUM    10 //最大查找数量
G10_WARNHELPRECD m_g10_warnhelprecd[MAX_FIND_NUM];//20201118
static UI32 dwPanelErr_time = 0;
extern UI32 g_warm1,g_warm2,g_warm3,g_warm4,g_warm5,g_warm6,g_warm7,g_warm8,g_warm9,g_warm10;
BOOL G10_AlarmWRRecord(UI32 cIndex,UI32 wErrId)
{
    UI16 curno,findno;
	UI16 i;
    UI16 j;
    UI16 nTempSeg = 0;
    UI16 nPart = 0;//20200413
    UI16 warn_num = 0;
    UI32 dwWarn[MAX_FIND_NUM]={g_warm1,g_warm2,g_warm3,g_warm4,g_warm5,g_warm6,g_warm7,g_warm8,g_warm9,g_warm10};

    warn_num = FindWarnId(cIndex);
    if(cIndex == 0x34100101)//20201118 面板警报另外记录时间
    {
        dwPanelErr_time = (UI32)time(NULL);
    }
    else if(warn_num != 0xFF)
    {
        m_g10_warnhelprecd[warn_num].nId = cIndex;
        m_g10_warnhelprecd[warn_num].subId = wErrId;
        m_g10_warnhelprecd[warn_num].datetime = (UI32)time(NULL);
    }
    else
    {
        BOOL isupdate = TRUE;
        for(i = 0; i < MAX_FIND_NUM; i++)
        {
            if(m_g10_warnhelprecd[i].nId == 0)
            {
                warn_num = i;
                break;
            }

            isupdate = TRUE;
            for(j = 0; j < MAX_FIND_NUM; j++)
            {
                if(dwWarn[j] != 0)
                {
                    if(m_g10_warnhelprecd[i].nId == dwWarn[j])
                    {
                        isupdate = FALSE;
                        break;
                    }
                }
            }

            if(isupdate)
            {
                warn_num = i;
                break;
            }
        }

        m_g10_warnhelprecd[warn_num].nId = cIndex;
        m_g10_warnhelprecd[warn_num].subId = wErrId;
        m_g10_warnhelprecd[warn_num].datetime = (UI32)time(NULL);
    }

	for(i=0;i<ARRAY_NUM(filter_alarm);i++)
	{
        if(cIndex == filter_alarm[i] )
        {
            g_bwarn = FALSE;
            break;
        }
        else
        {
            g_bwarn = TRUE;
        }
	}
	for(i=0;i<ARRAY_NUM(filter_alarmsave);i++)
	{
		if(cIndex == filter_alarmsave[i])
            return FALSE;
	}   

    //20200810
    for(i = 0; i < MAX_FIND_NUM; i++)
    {
        if(m_g10_warnrecd.warnHead.cur_no >= (i + 1))
        {
            findno = m_g10_warnrecd.warnHead.cur_no - 1 - i;
        }
        else
        {
            findno = G10_MAX_RECORDS + m_g10_warnrecd.warnHead.cur_no - 1 - i;
        }

        if(m_g10_warnrecd.warnitems[findno].flag == MARK_USED)
        {
            if((m_g10_warnrecd.warnitems[findno].nId == cIndex) && (m_g10_warnrecd.warnitems[findno].subId == wErrId))
            {
                UI32 timeinterval = (UI32)time(NULL) - m_g10_warnrecd.warnitems[findno].datetime;
                if(timeinterval < (VarAdrToInt(p_PP_MACHSET_ALARM_WRITETM)*60 + 2))//1s内收到同一个警报则不记录 //20241218 chj 相同警报不记录 2s改为10分钟
                {
                    return FALSE;
                }
                else
                {
                    break;
                }
            }
        }
    }

    curno = m_g10_warnrecd.warnHead.cur_no;
    m_g10_warnrecd.warnitems[curno].flag = MARK_USED;
    m_g10_warnrecd.warnitems[curno].wShotCount = VarAdrToInt(CLAMP_STATE_MOLDOPNNUM0)<<16 | VarAdrToInt(CLAMP_STATE_MOLDOPNNUM1);
    m_g10_warnrecd.warnitems[curno].nId = cIndex;
    m_g10_warnrecd.warnitems[curno].datetime = (UI32)time(NULL);

    m_g10_warnrecd.warnitems[curno].subId = wErrId;
    if ((cIndex & 0xFF0FFFFF) == 0x0E00010E)//温度部件#1 //20200413
    {
        nPart = (cIndex >> 20) & 0x000F;//20200413
        if ((nPart > VarAdrToInt(TEMP_CTRL_PART)) || (nPart < 1 ))
            return FALSE;
        nTempSeg = VarAdrToInt(SUBPAR(d_TempCtrlComm1_MACHSET_DATEMP_CH_NUM,nPart)) - 1;
        for (i = 0;i < nTempSeg;i++)
        {
            for(j = 11;j < 14;j++)//bit11：热电偶断线 bit12：温度反接 bit13：温度读取失败
            {
                if (((VarAdrToInt(SUBPAR(d_TempCtrlComm1_STATE_FL_TEMPSTATUS_2,nPart) + i)) >> j)&0x1)
                {
                    if (j == 11)
                    {
                        m_g10_warnrecd.warnitems[curno].res1[0] = m_g10_warnrecd.warnitems[curno].res1[0] | (1 << i);
                    }
                    else if (j == 12)
                    {
                        m_g10_warnrecd.warnitems[curno].res1[1] = m_g10_warnrecd.warnitems[curno].res1[1] | (1 << i);
                    }
                    else if (j == 13)
                    {
                        m_g10_warnrecd.warnitems[curno].res1[2] = m_g10_warnrecd.warnitems[curno].res1[2] | (1 << i);
                    }
                }
            }
        }

        //20201118
        m_g10_warnhelprecd[warn_num].res1[0] = m_g10_warnrecd.warnitems[curno].res1[0];
        m_g10_warnhelprecd[warn_num].res1[1] = m_g10_warnrecd.warnitems[curno].res1[1];
        m_g10_warnhelprecd[warn_num].res1[2] = m_g10_warnrecd.warnitems[curno].res1[2];
    }
    else if ((cIndex & 0xFF0FFFFF) == 0x2F00010A)//20200407HJM 模温功能
    {
        nPart = (cIndex >> 20) & 0x000F;//20200413
        if ((nPart > VarAdrToInt(MT_CTRL_PART)) || (nPart < 1 ))
            return FALSE;
        nTempSeg = VarAdrToInt(SUBPAR(d_TempCtrlMW1_MOLDSET_DATEMP_NUM_MW,nPart)) - 1;
        for (i = 0;i < nTempSeg;i++)
        {
            for(j = 11;j < 14;j++)//bit11：热电偶断线 bit12：温度反接 bit13：温度读取失败
            {
                if (((VarAdrToInt(SUBPAR(d_TempCtrlMW1_STATE_FL_STATUS_MW0,nPart) + i)) >> j)&0x1)
                {
                    if (j == 11)
                    {
                        m_g10_warnrecd.warnitems[curno].res1[0] = m_g10_warnrecd.warnitems[curno].res1[0] | (1 << i);
                    }
                    else if (j == 12)
                    {
                        m_g10_warnrecd.warnitems[curno].res1[1] = m_g10_warnrecd.warnitems[curno].res1[1] | (1 << i);
                    }
                    else if (j == 13)
                    {
                        m_g10_warnrecd.warnitems[curno].res1[2] = m_g10_warnrecd.warnitems[curno].res1[2] | (1 << i);
                    }
                }
            }
        }

        //20201118
        m_g10_warnhelprecd[warn_num].res1[0] = m_g10_warnrecd.warnitems[curno].res1[0];
        m_g10_warnhelprecd[warn_num].res1[1] = m_g10_warnrecd.warnitems[curno].res1[1];
        m_g10_warnhelprecd[warn_num].res1[2] = m_g10_warnrecd.warnitems[curno].res1[2];
    }

    m_g10_warnrecd.warnHead.cur_no++;
    if(m_g10_warnrecd.warnHead.cur_no >= G10_MAX_RECORDS)
	{
        m_g10_warnrecd.warnHead.cur_no = 0;
	}

    G10_WriteRecord(curno);

    return TRUE;
}

void G10_AlarmClearRecord()
{
	int i;

    FileDelete(G10_ALARM_WR_PATH);
    G10_LoadAlarmRecord();

    m_g10_warnrecd.warnHead.cur_no =0;
    for(i=0;i<G10_MAX_RECORDS;i++)
	{
        m_g10_warnrecd.warnitems[i].flag = 0;
        //G10_WriteRecord(i);
	}	
}

/**
 * @brief      : 查找警报
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20201118
 */
UI16 FindWarnId(UI32 cIndex)
{
    UI16 i;

    for(i = 0; i < MAX_FIND_NUM; i++)
    {
        if(cIndex == m_g10_warnhelprecd[i].nId)
        {
            return i;
        }
    }

    return 0xFF;
}

UI32 GetPanelErrTime()
{
    return dwPanelErr_time;
}
