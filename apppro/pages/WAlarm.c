#include "WAlarm.h"
#include "typedef.h"
#include "pagefram.h"
#include "buttonwgt.h"
#include "file.h"
#include "stdio.h"
#include "alarmrecord.h"
#include "alarmrecord_G10.h"
#include "texttbl.h"
#include "privdef.h"
#include "database.h"
#include "labelwgt.h"
#include "oprintf.h"
#include "warndatarec.h"
#include "drivealarmrecord.h"
#include "panelrec.h"
#include "servoalarmtbl.h"//20211216 dyl 伺服警报名称显示
#include "usermod.h"//20210914 dyl touch

DEFINE_ALARM_MAP

CONNECT_ALARM_EVENT

#define     SHOW_RECORDS_COUNT    15      //页面显示的记录数
//20200810 变量关联修改 防止与WarnHelp画面关联变量冲突
#define		TMP_STARTNUM	    TMPVAL(300) //start No.  临时变量300
#define		TMP_PASSWORD	    TMPVAL(320) //password   临时变量320
#define		TMP_ALARMSTR	    TMPSTR(60)  //alarm description 临时字符串60
#define		TMP_TEMPALARMSTR	TMPSTR(120) //警报详细记录(主要用于显示温度警报)
#define		TMP_ALARMDES_BKCOLOR	TMPVAL(360) //警报说明背景色
//20200810
#define     TMP_ALARMEXP_SHOW   TMPVAL(380) //警报详细说明可见

#define		STR_EMPTY			""

I16  nwarm=-1;

static BOOL AlarmShowRecord()
{
    UI16 i, j;
    UI32 value,nId;
    DB_G10_WARNRECORD item;
    char strtmp[255];
    char strid[255];
	const char *ptrans;
    UI16 nrec;

    if(GetAlarmExpState())//20200810 详细信息改变时刷新
    {
        nwarm = -1;
        InitAlarmExpState();
    }

    if(nwarm == m_g10_warnrecd.warnHead.cur_no)
		return FALSE;
    nwarm = m_g10_warnrecd.warnHead.cur_no;

    value = VarAdrToInt(TMP_STARTNUM);//20200810
    nrec = (G10_MAX_RECORDS+nwarm-value)%G10_MAX_RECORDS;

	for(i=0;i<SHOW_RECORDS_COUNT;i++)
	{
        if(nrec==0) nrec=G10_MAX_RECORDS;
		nrec--;

        VarAdrSetInt(TMP_STARTNUM+i,value+i);
        item = G10_AlarmReadRecord(nrec);//从最新的时间开始显示
        if(item.flag == MARK_USED /*&& (value+i)<=m_g10_warnrecd.warnHead.cur_no*/)
		{
			if(pmap_alarm!=NULL)
			{
				ptrans = GetMapTran(pmap_alarm,item.nId);
				if(ptrans!=NULL)
				{
                    //通用是冷态启动防止时间 不是预温计时
                    if((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x8B00)
                    {
                        if(item.nId == 0x0E100101)
                        {
                            ptrans = GetTextTran(TEXT_ALARM_E100101);
                        }
                        else if(item.nId == 0x0E200101)
                        {
                            ptrans = GetTextTran(TEXT_ALARM_E200101);
                        }
                    }

                    //alarm num
                    nId = item.nId - 0x101;
                    snprintf(strid,sizeof(strid)-1,"#%05X-",ALARM_NUMBER(nId));//(((nId&0xFFF00000)|((nId&0xF00)<<8)|((nId&0x0F)<<12))>>12)

                    if((item.nId & 0xFF0FFFFF) == 0x0F000101) //伺服警报
                    {
                        char buff[100];
                        UI16 wGroup = (item.nId & 0x00F00000) >> 20;
//                        snprintf(buff,sizeof(buff)-1,"#%d%s Er.%03X",wGroup,GetTextTran(TEXT_DRIVER),item.subId & 0x0FFF);
                        if (GetServoAlarmTran(item.subId & 0x0FFF) != NULL)//20211216 dyl 伺服警报名称显示
                        {
                            snprintf(buff,sizeof(buff)-1,"#%d%s:Er.%03X%s",wGroup,GetTextTran(TEXT_DRIVER),item.subId & 0x0FFF,GetServoAlarmTran(item.subId & 0x0FFF));
                        }
                        else
                        {
                            snprintf(buff,sizeof(buff)-1,"#%d%s:Er.%03X",wGroup,GetTextTran(TEXT_DRIVER),item.subId & 0x0FFF);
                        }
                        strcat(strid,buff);
                        strcpy(strtmp,strid);
                    }
                    else if(((item.nId & 0xFF0FFFFF)==0x0E00010E) || ((item.nId & 0xFF0FFFFF)==0x2F00010A)) //温度部件警报+模温部件警报
                    {
                        char buff[255];
                        char num[10];
                        UI16 wTempRec = 0;
                        //显示的警报
                        strcat(strid,ptrans);
                        strcpy(strtmp,strid);
                        strcat(strtmp,"...");

                        //详细警报
                        //热电偶断线
                        if(item.res1[0] != 0)
                        {
                            strcat(strid,"\n");
                            snprintf(buff,sizeof(buff)-1,"%s",GetTextTran(TEXT_HEATOFF));
                            wTempRec = item.res1[0];
                            for(j = 0;j < 16;j++)
                            {
                                if(wTempRec & (1 << j))
                                {
                                    snprintf(num,sizeof(num)-1,"#%d ",j+1);
                                    strcat(buff,num);
                                }
                            }
                            strcat(strid,buff);
                        }

                        //热电偶接反
                        if(item.res1[1] != 0)
                        {
                            strcat(strid,"\n");
                            snprintf(buff,sizeof(buff)-1,"%s",GetTextTran(TEXT_HEATREVERSE));
                            wTempRec = item.res1[1];
                            for(j = 0;j < 16;j++)
                            {
                                if(wTempRec & (1 << j))
                                {
                                    snprintf(num,sizeof(num)-1,"#%d ",j+1);
                                    strcat(buff,num);
                                }
                            }
                            strcat(strid,buff);
                        }

                        //温度读取失败
                        if(item.res1[2] != 0)
                        {
                            strcat(strid,"\n");
                            snprintf(buff,sizeof(buff)-1,"%s",GetTextTran(TEXT_HEATREADFAIL));
                            wTempRec = item.res1[2];
                            for(j = 0;j < 16;j++)
                            {
                                if(wTempRec & (1 << j))
                                {
                                    snprintf(num,sizeof(num)-1,"#%d ",j+1);
                                    strcat(buff,num);
                                }
                            }
                            strcat(strid,buff);
                        }
                        VarAdrSetStr(TMP_TEMPALARMSTR+i,strid);
                    }
                    else
                    {
                        strcat(strid,ptrans);
                        strcpy(strtmp,strid);
                    }
				}
                else //未翻译警报处理
                {
                    snprintf(strtmp,sizeof(strtmp)-1,"%x",item.nId);
                }
				VarAdrSetStr(TMP_ALARMSTR+i,strtmp);
                if(((item.nId & 0xFF0FFFFF)!=0x0E00010E) && ((item.nId & 0xFF0FFFFF)!=0x2F00010A))
                {
                    VarAdrSetStr(TMP_TEMPALARMSTR+i,strtmp);
                }
			}
			WordToStr(strtmp,item.wShotCount,0);
			VarAdrSetStr(TMP_ALARMSTR+SHOW_RECORDS_COUNT+i,strtmp);
			TimeToStr(strtmp,"yyyy-MM-dd hh:mm:ss",item.datetime);
			VarAdrSetStr(TMP_ALARMSTR+SHOW_RECORDS_COUNT*2+i,strtmp);
		}
		else
		{
			VarAdrSetStr(TMP_ALARMSTR+i,STR_EMPTY);
            VarAdrSetStr(TMP_TEMPALARMSTR+i,STR_EMPTY);
			VarAdrSetStr(TMP_ALARMSTR+SHOW_RECORDS_COUNT+i,STR_EMPTY);
			VarAdrSetStr(TMP_ALARMSTR+SHOW_RECORDS_COUNT*2+i,STR_EMPTY);
		}

        //20200810
        if(DriveAlarm_HaveRecord(nrec,item.subId) || HostAlarm_HaveRecord(nrec,item.nId,item.datetime)) //驱动器警报记录 主机警报记录
        {
            VarAdrSetInt(TMP_ALARMEXP_SHOW+i, TRUE);
        }
        else
        {
            VarAdrSetInt(TMP_ALARMEXP_SHOW+i, FALSE);
        }
	}
	return TRUE;
}

static BOOL AlarmClearButton()
{
	nwarm =-1;//update
    G10_AlarmClearRecord();
    DriveAlarm_ClearRecord();//20200810
    HostAlarm_ClearRecord();//20200810

    VarAdrSetInt(TMP_STARTNUM,0);

	AlarmShowRecord();

	return TRUE;
}

//刷新详细警报信息
static void DisplayAlarmInfo()
{
    UI32 index;
    UI32 i;
    PWGT pwgt;
    pwgt = PageFrameCurFocusWgt();
    index = WGTGetGOffset(pwgt);
    if(index)//20200810
    {
        LabelSetText(alarm_pg.lblinfo,VarAdrToStr(TMP_TEMPALARMSTR+index-1));

        if(VarAdrToInt(TMP_ALARMEXP_SHOW+index-1))
        {
            WGTSetVisible(alarm_pg.lblexpinfo, TRUE);
        }
        else
        {
            WGTSetVisible(alarm_pg.lblexpinfo, FALSE);
        }
    }
    else
    {
        WGTSetVisible(alarm_pg.lblexpinfo, FALSE);
        LabelSetText(alarm_pg.lblinfo,"");
    }

    for(i = 0;i < SHOW_RECORDS_COUNT;i++)
    {
        if((i+1) == index)
        {
            VarAdrSetInt(TMP_ALARMDES_BKCOLOR+i,ConvToQtColor(0xe1d700));
        }
        else
        {
            VarAdrSetInt(TMP_ALARMDES_BKCOLOR+i,0xffffff);
        }
    }
}

static int button8OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static void CtlConnect()
{
    WgtMsgConnect(alarm_pg.button8, WGT_CLICK_MSG, button8OnClick);
}
static int AlarmOnInit(Alarm_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int AlarmOnShow(Alarm_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	nwarm=-1;
    VarAdrSetInt(TMP_STARTNUM,0);
	VarAdrSetInt(TMP_PASSWORD,0);//密码进入为0
	AlarmShowRecord();
	WGTSetEnable(ppg->button8, FALSE);

    SetPageLockMode(MODE_USERLOCK);//20210914 dyl touch

    return 0;
}
static int AlarmOnHide(Alarm_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch
    return 0;
}
static int AlarmOnUpdate(Alarm_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	AlarmShowRecord();
    DisplayAlarmInfo();//刷新详细警报信息

    return 0;
}
static int AlarmOnChange(Alarm_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 addr;
    UI32 index;
	PATTR pattr;
    UI16 nrec,i;

	pattr = (PATTR)msg_para;

	addr = OBJAttrGetAdr(pattr);
    index = WGTGetGOffset(pwgt);

    if(index)
    {
        PageWgtSetFocus(pwgt);

        //20200810
        if(VarAdrToInt(TMP_ALARMEXP_SHOW+index-1))
        {
            nrec = (G10_MAX_RECORDS+m_g10_warnrecd.warnHead.cur_no-VarAdrToInt(TMP_STARTNUM))%G10_MAX_RECORDS;
            for(i = 0; i < index; i++)
            {
                if(nrec==0) nrec=G10_MAX_RECORDS;
                nrec--;
            }
            SetCurWarnId(nrec);
            PanelShowDialogByName(DIALOG_WARNHELP);
        }
    }

	switch(addr)
	{
    case TMP_STARTNUM://20200810
		nwarm =-1;
		break;
	case TMP_PASSWORD:    //20181019hz
		if (VarAdrToInt(TMP_PASSWORD) == 9595)
		{
			WGTSetEnable(ppg->button8, TRUE);
		} 
		else
		{
			WGTSetEnable(ppg->button8, FALSE);
		}
		break;
	default :
		break;
	}

    //20200810
    if(pwgt == ppg->btlast)
    {
        if(VarAdrToInt(TMP_STARTNUM) > SHOW_RECORDS_COUNT)
        {
            VarAdrSetInt(TMP_STARTNUM, VarAdrToInt(TMP_STARTNUM)-SHOW_RECORDS_COUNT);
        }
        else
        {
            VarAdrSetInt(TMP_STARTNUM, 0);
        }
        nwarm =-1;
    }
    else if(pwgt == ppg->btnext)
    {
        if(VarAdrToInt(TMP_STARTNUM) < (G10_MAX_RECORDS-SHOW_RECORDS_COUNT*2))
        {
            VarAdrSetInt(TMP_STARTNUM, VarAdrToInt(TMP_STARTNUM)+SHOW_RECORDS_COUNT);
        }
        else
        {
            VarAdrSetInt(TMP_STARTNUM, G10_MAX_RECORDS-SHOW_RECORDS_COUNT);
        }
        nwarm =-1;
    }

    return 0;
}
static int button8OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	PBUTTON_WGT ped;

	ped = WgtToButton(pwgt);

	if(ped == NULL)
		return FALSE;

    PanelRecAdd(alarm_pg.button8, PNL_CLEAR_ALL_REC, 0);
	AlarmClearButton();

    return 0;
}
