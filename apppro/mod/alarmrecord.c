#include "alarmrecord.h"
#include "time.h"
#include "file.h"
#include "common.h"
#include "language.h"
#include "monmcu.h"
#include "prot.h"
#include "oprintf.h"
#include "panelcfg.h"
#include "alarmrecord_G10.h"
#include "database.h"
#include "drivealarmrecord.h"

#define     MARK_USED				0xEB90
//
#define		FILE_INDEX(NUM)		(sizeof(DB_WARNRECOD_HEAD) + sizeof(DB_WARNRECORD)*NUM)


WARNRECD m_warnrecd;
PHASH_TBL pmap_alarm=NULL;
UI32   g_warm1,g_warm2,g_warm3;
//20200407HJM 模温功能
UI32   g_warm4,g_warm5,g_warm6,g_warm7,g_warm8,g_warm9,g_warm10;
BOOL   g_bwarn=FALSE;

UI32   filter_alarm[]={
		0x31100103,0x23100101,0x23200101,0x23300101,0x23400101,0x23500101,0x23600101,0x23700101,
		0x23800101,0x23900101,0x23a00101,0x23b00101,0x23c00101,0x23d00101,0x23e00101,
		0x20100107,0x21100101,0x29100101,0x29100102
};

UI32   filter_alarmsave[]={
	0x23100101,0x23200101,0x23300101,0x23400101,0x23500101,0x23600101,0x23700101,
	0x23800101,0x23900101,0x23a00101,0x23b00101,0x23c00101,0x23d00101,0x23e00101,
	0x31100103,0x29100101,0x29100102,0x21100101
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

/**
* @brief     :写警报,只会写传入的itmenum这条警报 及 头部数据
* @param     :警报索引
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
static BOOL WriteRecord(UI16 itmenum)
{
	int index;

	FILEHD filehd;
	filehd = FileOpen(ALARM_WR_PATH);
	index = FILE_INDEX(itmenum);
	if(CheckFileValid(filehd))
	{
		FileSeek(filehd,0);
		FileWrite(filehd,&m_warnrecd,sizeof(DB_WARNRECOD_HEAD),NULL);
		FileSeek(filehd,index);
		FileWrite(filehd,(char*)(&m_warnrecd) + index,sizeof(DB_WARNRECORD),NULL);
		FileClose(filehd);
		return TRUE;
	}
	return FALSE;
}

/**
* @brief     :从内存中读警报
* @param     :警报索引
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
DB_WARNRECORD AlarmReadRecord(UI32 index)
{
	DB_WARNRECORD item;
	if( index <MAX_RECORDS /*&& index >=0*/)
	{
		item = m_warnrecd.warnitems[index];//m_warnrecd.warnHead.cur_no-index
	}
	return item;
}

/**
* @brief     :加载已有的警报数据，G系列警报加载也会在这里被调用
* @param     :
* @return    :
* @retval    :
* @note      :TODO 把G系列调用分出来
* @attention :
* @author    :hz
* @date      :20200306
*/
void LoadAlarmRecord()
{
	FILEHD filehd;

    if((PanelCfgStyle()==G_TYPE) || (PanelCfgStyle()==P_TYPE))
    {
        G10_LoadAlarmRecord();
        DriveAlarm_LoadRecord();//20200810
        HostAlarm_LoadRecord();//20200810
        return;
    }

	filehd = FileOpen(ALARM_WR_PATH);

	if(CheckFileValid(filehd))
	{
		FileRead(filehd,&m_warnrecd,sizeof(WARNRECD),NULL);
		FileClose(filehd);
	}
	else 
	{
		filehd = FileCreate(ALARM_WR_PATH);
		FileClose(filehd);
	}
}

/**
* @brief     :具体的警报处理，及将警报写入文件
* @param     :警报索引
* @return    :
* @retval    :
* @note      :TODO 要把G系列分出来
* @attention :
* @author    :hz
* @date      :20200306
*/
void AlarmWRRecord(UI32 cIndex)
{
	UI16 curno;
	UI16 i;

    if((PanelCfgStyle()==G_TYPE) || (PanelCfgStyle()==P_TYPE))
    {
        G10_AlarmWRRecord(cIndex,0);
        return;
    }

	for(i=0;i<ARRAY_NUM(filter_alarm);i++)
	{
		if(cIndex == filter_alarm[i] )
        {
			g_bwarn = FALSE;
            break;
        }
		else 
			g_bwarn = TRUE;
	}

	for(i=0;i<ARRAY_NUM(filter_alarmsave);i++)
	{
		if(cIndex == filter_alarmsave[i])
			return;
	}


	curno = m_warnrecd.warnHead.cur_no;
	m_warnrecd.warnitems[curno].flag = MARK_USED;
    m_warnrecd.warnitems[curno].wShotCount = VarAdrToInt(CLAMP_STATE_MOLDOPNNUM0)<<16 | VarAdrToInt(CLAMP_STATE_MOLDOPNNUM1);
	m_warnrecd.warnitems[curno].nId = cIndex;
	m_warnrecd.warnitems[curno].datetime = (UI32)time(NULL);

	m_warnrecd.warnHead.cur_no++;
	if(m_warnrecd.warnHead.cur_no >= MAX_RECORDS)
	{
		m_warnrecd.warnHead.cur_no = 0;
	}

	WriteRecord(curno);
}

/**
* @brief     :警报清除，并重建文件
* @param     :
* @return    :
* @retval    :
* @note      :TODO G系列分出来
* @attention :
* @author    :hz
* @date      :20200306
*/
void AlarmClearRecord()
{
	int i;

    if((PanelCfgStyle()==G_TYPE) || (PanelCfgStyle()==P_TYPE))
    {
        G10_AlarmClearRecord();
        return;
    }

	FileDelete(ALARM_WR_PATH);
	LoadAlarmRecord();

	m_warnrecd.warnHead.cur_no =0;
	for(i=0;i<MAX_RECORDS;i++)
	{
		m_warnrecd.warnitems[i].flag = 0;
		WriteRecord(i);
	}	
}

/************************************************************************/
/* 警报监控函数                                                         */
/************************************************************************/
/**
* @brief     :警报监控，分离出警报
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
static void ReadWarmMonitor(int adr_num, PDATA_SET_ADR pvar_adr)
{
	UI16 warnnum=0;
    UI16 i,j,k,m,warnvalue,num;
	UI16 len;
	PVAR pvar;
    //20200407HJM 模温功能
    static UI32 wOldValue[10]={0};//3 -modify 10 20200407HJM 模温功能
    UI32 *dwWarn[10]={&g_warm1,&g_warm2,&g_warm3,&g_warm4,&g_warm5,&g_warm6,&g_warm7,&g_warm8,&g_warm9,&g_warm10};
    UI16 totalwarnnum=0;
    BOOL bwriterecord = FALSE;
    UI16 writenum = 0;//20201118 需要写的警报序号

    if(isUseMoldFunc())
    {
        totalwarnnum = 10;
    }
    else
    {
        totalwarnnum = 3;
    }

	if(pvar_adr == NULL)
		return;

	for(i=0;i<adr_num;i++)
	{
		len = pvar_adr[i].len;
		pvar = pvar_adr[i].pvar;
		for(m=0;m<len;m++)
		{
			if(*(UI16*)((pvar+m)->pvalue) == 0)
				continue;

			warnvalue = *(UI16*)((pvar+m)->pvalue);

			for(k=0;k<16;k++)
			{
				if(warnvalue&0x8000){
					*dwWarn[warnnum] = (((pvar+m)->address&0xFFF00000)|(((pvar+m)->address&0xF)+1)<<8)+15-k+1;
                    //与原来的当前警报值进行比较，有变化的保存警报
                    bwriterecord = TRUE;//20200407HJM 模温功能
                    for(num = 0; num < totalwarnnum; num++)
                    {
                        if(*dwWarn[warnnum] == wOldValue[num])
                        {
                            bwriterecord = FALSE;
                            break;
                        }
                    }

                    if(bwriterecord && (*dwWarn[warnnum]!=0)){
                        if(((PanelCfgStyle()==G_TYPE) || (PanelCfgStyle()==P_TYPE)) && ((*dwWarn[warnnum] & 0xFF0FFFFF)== 0x0F000101))//增加伺服警报代码,若有多个警报保存多次
                        {
                            ;
                        }
                        else
                        {
                            writenum |= (1<<warnnum);//20201118 由单个写文件改为统一写文件 等待警报全部赋值完成
                            //AlarmWRRecord(*dwWarn[warnnum]);
                        }
                        //wOldValue[warnnum]=*dwWarn[warnnum];
					}
					warnnum++;
				}
				warnvalue<<=1;
                if(warnnum>=totalwarnnum)
                {
                    for(j=0;j<totalwarnnum;j++)
                    {
                        wOldValue[j]=*dwWarn[j];

                        if((writenum >> j) & 0x1)//20201118
                        {
                            AlarmWRRecord(*dwWarn[j]);
                        }
                    }
                    return;
                }
			}
		}
	}
    for(j=0;j<totalwarnnum;j++)
    {
        wOldValue[j]=*dwWarn[j];

        if((writenum >> j) & 0x1)//20201118
        {
            AlarmWRRecord(*dwWarn[j]);
        }
    }

    return;
}

/**
* @brief     :警报监控, 被调函数
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
static void AlarmRecvData(UI16 set_id, int dat_num, UI16* pdata, int adr_num, PDATA_SET_ADR pvar_adr)
{
	if(pvar_adr == NULL)
		return;

	if(set_id == 130 + 0x100)//alarm data
	{
		g_warm1 =0;
		g_warm2 =0;
		g_warm3 =0;
        //20200407HJM 模温功能
        g_warm4 =0;
        g_warm5 =0;
        g_warm6 =0;
        g_warm7 =0;
        g_warm8 =0;
        g_warm9 =0;
        g_warm10 =0;
		ReadWarmMonitor(adr_num,pvar_adr);
	}
}

/**
* @brief     :警报初始化，注册警报记录处理函数
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
void AlarmRecdInital(PCMOD pmod)
{
	LoadAlarmRecord();
	ParseCSVFile(ALARM_FILE,&pmap_alarm);
	DataSetCbSet(130+0x100,AlarmRecvData);//alarm data 设置回调函数
}
