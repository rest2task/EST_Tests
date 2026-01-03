#include "warndatarec.h"
#include "time.h"
#include "stdio.h"
#include "file.h"
#include "obj.h"
#include "common.h"
#include "panel.h"
#include "verinfo.h"
#include "protcmd.h"
#include "math.h"
#include "verinfo.h"
#include "database.h"
#include "alarmrecord.h"
#include "drivealarmrecord.h"
#include "tstring.h"
#include "oprintf.h"
#define MIN_ALLOC_SIZE 512

#define SYS_HARDWARE                0x00140001 //硬件类型
#define SYS_BOOTVER                 0x00140004 //BOOT版本
#define SYS_BOOTDATE                0x00140005 //BOOT日期
#define SYS_MAINVER2                0x00140006 //主机版本2
#define SYS_MAINVER1                0x00140007 //主机版本1
#define SYS_MAINDATE1               0x00140008 //主机日期1
#define SYS_MAINDATE2               0x00140009 //主机日期2

WDRECORD m_wdrecord;

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

static int WDisFull(WDRECORD* rec)
{
    if((rec->wdhead.rear + 1) % MAX_WARNDATA_SIZE == rec->wdhead.front)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static int WDisEmpty(WDRECORD* rec)
{
    if(rec->wdhead.rear == rec->wdhead.front)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static int WDDelete(WDRECORD* rec)
{
    if(WDisEmpty(rec))
        return FALSE;

    rec->wdhead.front = (rec->wdhead.front + 1) % MAX_WARNDATA_SIZE;
    return TRUE;
}

static void	WriteWDRec(int index,void* src)
{
    FILEHD filehd;
    filehd = FileOpen(WD_REC_PATH);
    if(CheckFileValid(filehd))
    {
        FileSeek(filehd,0);
        FileWrite(filehd,&m_wdrecord.wdhead, sizeof(WDHEAD),NULL);
        FileSeek(filehd, sizeof(WDHEAD) + index * sizeof(WDITEM));
        FileWrite(filehd, src, sizeof(WDITEM), NULL);
        FileClose(filehd);
    }
}

static int WDAdd(WDRECORD* rec, WDITEM item)
{
    int index;
    if(WDisFull(rec))
        WDDelete(rec);
    rec->item[rec->wdhead.rear] = item;
    index = rec->wdhead.rear;
    rec->wdhead.rear = (rec->wdhead.rear + 1) % MAX_WARNDATA_SIZE;
    WriteWDRec(index, &item);
    return TRUE;
}

static int WDNum(WDRECORD* rec)
{
    return (rec->wdhead.rear - rec->wdhead.front + MAX_WARNDATA_SIZE) % MAX_WARNDATA_SIZE;
}

void WDRecInit()
{
    FILEHD filehd;
    memset(&m_wdrecord, 0, sizeof(WDRECORD));
    m_wdrecord.wdhead.front = 0;
    m_wdrecord.wdhead.rear = 0;
    filehd = FileOpen(WD_REC_PATH);
    if(CheckFileValid(filehd))
    {
        FileRead(filehd, &m_wdrecord, sizeof(WDRECORD), NULL);
        FileClose(filehd);
    }
    else {
        filehd = FileCreate(WD_REC_PATH);
        FileClose(filehd);
    }
}

//
void ClearWDRec()
{
    memset(&m_wdrecord, 0, sizeof(WDRECORD));
    FILEHD filehd;
    filehd = FileOpen(WD_REC_PATH);
    if(CheckFileValid(filehd))
    {
        FileSeek(filehd,0);
        FileWrite(filehd, &m_wdrecord, sizeof(WDRECORD), NULL);
        FileClose(filehd);
    }
}

BOOL ReadWDRec(WDITEM* item,UI32 index)
{
    if(index < WDNum(&m_wdrecord) && !WDisEmpty(&m_wdrecord))
    {
        if(item == NULL)
            return FALSE;
        memcpy(item, &m_wdrecord.item[(m_wdrecord.wdhead.rear - index - 1 + MAX_WARNDATA_SIZE) % MAX_WARNDATA_SIZE], sizeof(WDITEM));

        return TRUE;
    }
    return FALSE;
}

BOOL WDRecIsChg()
{
    static int chg = -1;
    if(chg != m_wdrecord.wdhead.rear)
    {
        chg = m_wdrecord.wdhead.rear;
        return TRUE;
    }
    else{
        return FALSE;
    }
}

static WDITEM WarnRec;
int RxCmd_WarnData(UI16* pdata)
{
    //当前警报
    int warnvalue = 0;
    int i, j;
    UI16* actdata = pdata;
    memcpy(&WarnRec.WarnData,pdata,sizeof(WARNDATARECORD));
    WarnRec.WarnData.wCurrent_Err1 = pdata[0] & 0xFFF0;//部件
    if ((pdata[1] != 0) && (pdata[2] == 0))//Err0
    {
        warnvalue = log2(pdata[1]);
        WarnRec.WarnData.wCurrent_Err0 = (1<<8) + warnvalue + 1;
    }
    else if (pdata[2] != 0)//Err1
    {
        warnvalue = log2(pdata[2]);
        WarnRec.WarnData.wCurrent_Err0 = (2<<8) + warnvalue + 1;
    }

    //警报时间
    TimeToStr(WarnRec.time,"yyyy-MM-dd hh:mm:ss",(UI32)time(NULL));

    if(WarnRec.WarnData.Num !=0)
    {
        actdata = pdata+sizeof(WARNDATARECORD)/2;
    }
    //警报时当前动作信息
    if(WarnRec.WarnData.Num < 4)
    {
        for(i=0; i<WarnRec.WarnData.Num; i++)
        {
            WarnRec.Act_Data[i].wAct_Id = *actdata;
            actdata++;
            WarnRec.Act_Data[i].wStep = *actdata;
            actdata++;
            WarnRec.Act_Data[i].wStart_ms = *actdata;
            actdata++;
            WarnRec.Act_Data[i].wStart_ms += (*actdata) << 16;
            actdata++;
        }
        for(j=WarnRec.WarnData.Num; j<4; j++)
        {
            WarnRec.Act_Data[j].wAct_Id = 0;
            WarnRec.Act_Data[j].wStep = 0;
            WarnRec.Act_Data[j].wStart_ms = 0;
        }
    }
    else
    {
        for(i=0; i<4; i++)
        {
            WarnRec.Act_Data[i].wAct_Id = *actdata;
            actdata++;
            WarnRec.Act_Data[i].wStep = *actdata;
            actdata++;
            WarnRec.Act_Data[i].wStart_ms = *actdata;
            actdata++;
            WarnRec.Act_Data[i].wStart_ms += (*actdata) << 16;
            actdata++;
        }
    }

    //AD位置相对值
    for(i=0; i<ADMETER_CH_NUM; i++)
    {
        if(VarAdrToInt(ADMETER_ABSOLUTE1+i) >= VarAdrToInt(ADMETER_ZSET1+i))
        {
            WarnRec.wADPosi_Symbol[i] = 1;
        }
        else
        {
            WarnRec.wADPosi_Symbol[i] = 0;
        }
    }


    //版本信息
    WarnRec.VersionInfo.wPanelType = m_verinfo.wPanelViewType;
    WarnRec.VersionInfo.wPanelFactory = VarAdrToUI16(SYS_MACH_CUSTOMID)/256;//面板代码和主机代码一样
    WarnRec.VersionInfo.wPanelVer1 = m_verinfo.wPanelViewVer1;
    WarnRec.VersionInfo.wPanelVer2 = m_verinfo.wPanelViewVer2;
    WarnRec.VersionInfo.wPanelDate1 = m_verinfo.wPanelDate1;
    WarnRec.VersionInfo.wPanelDate2 = m_verinfo.wPanelDate2;
    WarnRec.VersionInfo.wHostFactory = VarAdrToUI16(SYS_MACH_CUSTOMID)/256;
    WarnRec.VersionInfo.wHostVer1 = VarAdrToInt(SYS_MAINVER1);
    WarnRec.VersionInfo.wHostVer2 = VarAdrToInt(SYS_MAINVER2);
    WarnRec.VersionInfo.wHostDate1 = VarAdrToInt(SYS_MAINDATE1);
    WarnRec.VersionInfo.wHostDate2 = VarAdrToInt(SYS_MAINDATE2);
    WarnRec.VersionInfo.wBoot1 = VarAdrToInt(SYS_BOOTVER);
    WarnRec.VersionInfo.wBoot2 = VarAdrToUI16(SYS_BOOTDATE);
    WarnRec.VersionInfo.wHardware = VarAdrToUI16(SYS_HARDWARE);


    //机器配置参数
    WarnRec.MachCfg.wver = VarAdrToInt(SYS_FL_VERSION);
    WarnRec.MachCfg.wCode0 = VarAdrToInt(SYS_FL_MACH_CODE0);
    WarnRec.MachCfg.wCode1 = VarAdrToInt(SYS_FL_MACH_CODE1);
    WarnRec.MachCfg.wCode2 = VarAdrToInt(SYS_FL_MACH_CODE2);
    WarnRec.MachCfg.wCode3 = VarAdrToInt(SYS_FL_MACH_CODE3);
    WarnRec.MachCfg.wCode4 = VarAdrToInt(SYS_FL_MACH_CODE4);
    WarnRec.MachCfg.wCode5 = VarAdrToInt(SYS_FL_MACH_CODE5);
    WarnRec.MachCfg.wFactory = VarAdrToInt(SYS_MACH_CUSTOMID);
    WarnRec.MachCfg.wCode7 = VarAdrToInt(SYS_MACH_WIO_DATE);
    WarnRec.MachCfg.wHydraulic = VarAdrToInt(SYS_FL_HYDRAUIC);

    WarnRec.MachCfg.wVerInfo = VarAdrToInt(SYS_FL_CTRLDATE);
    WarnRec.MachCfg.wTemp = VarAdrToInt(SYS_FL_TEMPER);
    WarnRec.MachCfg.wMeter = VarAdrToInt(SYS_FL_METER);
    WarnRec.MachCfg.wCode11 = VarAdrToInt(SYS_MACH_WDSR_DATE);
    WarnRec.MachCfg.wCode12 = VarAdrToInt(SYS_MACH_FL_PATCHS);
    WarnRec.MachCfg.wSystem = VarAdrToInt(SYS_FL_CTRLMODEL);
    WarnRec.MachCfg.wSysPrs = VarAdrToInt(SYS_MACH_DAPRES_SYSLMT);
    WarnRec.MachCfg.wCode16 = VarAdrToInt(SYSTEM_MACH_CODE44);
    WarnRec.MachCfg.wRpmCnt = VarAdrToInt(SYS_CN_CHGRPMHOLE);
    WarnRec.MachCfg.wPushCode = VarAdrToInt(SYS_FL_MACHINEMODEL);

    WarnRec.MachCfg.wCode43 = VarAdrToInt(SYSTEM_MACH_CODE43);
    WarnRec.MachCfg.wCode48 = VarAdrToInt(SYS_FL_MACH_CODE48);
    WarnRec.MachCfg.wCode49 = VarAdrToInt(SYS_FL_MACH_CODE49);
    WarnRec.MachCfg.wCode50 = VarAdrToInt(SYS_FL_MACH_CODE50);
    WarnRec.MachCfg.wCode51 = VarAdrToInt(SYS_FL_MACH_CODE51);
    WarnRec.MachCfg.wCode52 = VarAdrToInt(SYS_FL_MACH_CODE52);
    WarnRec.MachCfg.wCode53 = VarAdrToInt(SYS_FL_MACH_CODE53);
    WarnRec.MachCfg.wCode54 = VarAdrToInt(SYS_FL_MACH_CODE54);
    WarnRec.MachCfg.wQVR    = VarAdrToInt(SYS_MACH_FL_MACHINESAFE);

    WarnRec.MachCfg.wReserve[0] = VarAdrToInt(d_machine1_MACHSET_FL_reserve_3);//推力座

    //警报存储
    //WDAdd(&m_wdrecord, WarnRec); //20241218 chj 相同警报不记录 警报详情没在使用了，注释掉
    AddHostAlarm(WarnRec);//20200810

    return 1;
}

static UI16 cur_id = 0;
void SetCurWarnId(UI16 id)
{
    cur_id = id;
    return;
}

UI16 GetCurWarnId()
{
    return cur_id;
}

static UI16 wMoniData[4];
int RxCmd_MoniDat(UI16* pdata)
{
    wMoniData[0] = pdata[2];

    return 1;
}

UI16 GetRealData()
{
    return wMoniData[0];
}


static const char* GetPageLangStr(const char* page_name, const char* lang)
{
    char path[50];
    PHASH_TBL lang_tbl;
    strcpy(path, PAGE_PATH);
    strcat(path, page_name);
    strcat(path, ".lan");
    ParseCSVFile(path, &lang_tbl);
    return GetLanTran(lang_tbl, lang);
}

/*//20230518 chj 原先的fileprintf方式有问题，每次调用都会写文件
BOOL SaveWarnDataExcel(char* tem_str)
{
    FILEHD stream;
    char buf[255];
    char * p_str;
    char cbuf[20];
    char cbuf2[20];
    int i, j;
    UI8 bs[3] = {(UI8)0xEF, (UI8)0xBB, (UI8)0xBF};

    if(WDisEmpty(&m_wdrecord))
        return FALSE;

    sprintf(buf,"%s%s%s",USB_BOOT_PATH,tem_str,".csv");

    p_str = ConvToGBK_Dup(buf);

    if(!FileExist(p_str))
    {
       stream = FileCreate(p_str);
    }
    else
    {
       stream = FileOpen(p_str);
    }

    OBJMemFree(p_str);

    if(!FILEHD_IS_OK(stream))
    {
        return FALSE;
    }

    FileWrite(stream, bs, sizeof(bs), NULL);

    FilePrintf(stream, "%s, ", GetPageLangStr(WARNDATA_PAGE, "Alarm Description"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Alarm Time"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Open Count"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Input #1"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Input #2"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Input #3"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Input #4"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Input #5"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Input #6"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Output #1"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Output #2"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Output #3"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Output #4"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Output #5"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Output #6"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "AD1"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "AD2"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "AD3"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "AD4"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "AD5"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "AD6"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "AD7"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "AD8"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "AD Collect 1"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "AD Collect 2"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "AD Collect 3"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "AD Collect 4"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "AD Collect 5"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "AD Collect 6"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "AD Collect 7"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "AD Collect 8"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Temp Scan Channel"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Temp AD"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Temp Room"));
    FilePrintf(stream, "%s-1, ", GetPageLangStr(WARNHELP_PAGE, "Temp Real"));
    FilePrintf(stream, "%s-2, ", GetPageLangStr(WARNHELP_PAGE, "Temp Real"));
    FilePrintf(stream, "%s-3, ", GetPageLangStr(WARNHELP_PAGE, "Temp Real"));
    FilePrintf(stream, "%s-4, ", GetPageLangStr(WARNHELP_PAGE, "Temp Real"));
    FilePrintf(stream, "%s-5, ", GetPageLangStr(WARNHELP_PAGE, "Temp Real"));
    FilePrintf(stream, "%s-6, ", GetPageLangStr(WARNHELP_PAGE, "Temp Real"));
    FilePrintf(stream, "%s-7, ", GetPageLangStr(WARNHELP_PAGE, "Temp Real"));
    FilePrintf(stream, "%s-8, ", GetPageLangStr(WARNHELP_PAGE, "Temp Real"));
    FilePrintf(stream, "%s-9, ", GetPageLangStr(WARNHELP_PAGE, "Temp Real"));
    FilePrintf(stream, "%s-10, ", GetPageLangStr(WARNHELP_PAGE, "Temp Real"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Act #1 ID"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Act #1 Step"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Act #1 Time"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Act #2 ID"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Act #2 Step"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Act #2 Time"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Act #3 ID"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Act #3 Step"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Act #3 Time"));
    FilePrintf(stream, "%s-1, ", GetPageLangStr(WARNHELP_PAGE, "DA OutPut"));
    FilePrintf(stream, "%s-2, ", GetPageLangStr(WARNHELP_PAGE, "DA OutPut"));
    FilePrintf(stream, "%s-3, ", GetPageLangStr(WARNHELP_PAGE, "DA OutPut"));
    FilePrintf(stream, "%s-4, ", GetPageLangStr(WARNHELP_PAGE, "DA OutPut"));
    FilePrintf(stream, "%s-5, ", GetPageLangStr(WARNHELP_PAGE, "DA OutPut"));
    FilePrintf(stream, "%s-6, ", GetPageLangStr(WARNHELP_PAGE, "DA OutPut"));
    FilePrintf(stream, "%s-7, ", GetPageLangStr(WARNHELP_PAGE, "DA OutPut"));
    FilePrintf(stream, "%s-8, ", GetPageLangStr(WARNHELP_PAGE, "DA OutPut"));
    FilePrintf(stream, "%s-1, ", GetPageLangStr(WARNHELP_PAGE, "Alarm Code"));
    FilePrintf(stream, "%s-2, ", GetPageLangStr(WARNHELP_PAGE, "Alarm Code"));
    FilePrintf(stream, "%s-3, ", GetPageLangStr(WARNHELP_PAGE, "Alarm Code"));
    FilePrintf(stream, "%s-4, ", GetPageLangStr(WARNHELP_PAGE, "Alarm Code"));
    FilePrintf(stream, "%s-5, ", GetPageLangStr(WARNHELP_PAGE, "Alarm Code"));
    FilePrintf(stream, "%s-6, ", GetPageLangStr(WARNHELP_PAGE, "Alarm Code"));
    FilePrintf(stream, "%s-1, ", GetPageLangStr(WARNHELP_PAGE, "Cmd Spd"));
    FilePrintf(stream, "%s-2, ", GetPageLangStr(WARNHELP_PAGE, "Cmd Spd"));
    FilePrintf(stream, "%s-3, ", GetPageLangStr(WARNHELP_PAGE, "Cmd Spd"));
    FilePrintf(stream, "%s-4, ", GetPageLangStr(WARNHELP_PAGE, "Cmd Spd"));
    FilePrintf(stream, "%s-5, ", GetPageLangStr(WARNHELP_PAGE, "Cmd Spd"));
    FilePrintf(stream, "%s-6, ", GetPageLangStr(WARNHELP_PAGE, "Cmd Spd"));
    FilePrintf(stream, "%s-1, ", GetPageLangStr(WARNHELP_PAGE, "Motor Spd"));
    FilePrintf(stream, "%s-2, ", GetPageLangStr(WARNHELP_PAGE, "Motor Spd"));
    FilePrintf(stream, "%s-3, ", GetPageLangStr(WARNHELP_PAGE, "Motor Spd"));
    FilePrintf(stream, "%s-4, ", GetPageLangStr(WARNHELP_PAGE, "Motor Spd"));
    FilePrintf(stream, "%s-5, ", GetPageLangStr(WARNHELP_PAGE, "Motor Spd"));
    FilePrintf(stream, "%s-6, ", GetPageLangStr(WARNHELP_PAGE, "Motor Spd"));
    FilePrintf(stream, "%s-1, ", GetPageLangStr(WARNHELP_PAGE, "Current Torque"));
    FilePrintf(stream, "%s-2, ", GetPageLangStr(WARNHELP_PAGE, "Current Torque"));
    FilePrintf(stream, "%s-3, ", GetPageLangStr(WARNHELP_PAGE, "Current Torque"));
    FilePrintf(stream, "%s-4, ", GetPageLangStr(WARNHELP_PAGE, "Current Torque"));
    FilePrintf(stream, "%s-5, ", GetPageLangStr(WARNHELP_PAGE, "Current Torque"));
    FilePrintf(stream, "%s-6, ", GetPageLangStr(WARNHELP_PAGE, "Current Torque"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Panel View Type"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Panel Factory"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Panel Version"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Panel Date"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Host Factory"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Host Version"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Host Date"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Boot Version"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Hardware"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Machine Config Version"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Code 0"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Code 1"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Code 2"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Code 3"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Code 4"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Code 5"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Manufact"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Code 7"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Hydr Ctrl"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Machine Config Ver Date"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Temp Ctrl"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Pos MT Ctrl"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Code 11"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Code 12"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "System Code"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "SysPrs Max"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Code 16"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Rpm Hole Cnt"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Push Code"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Code 43"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Code 48"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Code 49"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Code 50"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Code 51"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Code 52"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Code 53"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "Code 54"));
    FilePrintf(stream, "%s, ", GetPageLangStr(WARNHELP_PAGE, "QVR"));
    FilePrintf(stream, "\n");

    for(j = 0; j<WDNum(&m_wdrecord); ++j)
    {
        WarnRec = m_wdrecord.item[(m_wdrecord.wdhead.rear - j - 1 + MAX_WARNDATA_SIZE) % MAX_WARNDATA_SIZE], sizeof(WDITEM);

        //warn name
        FilePrintf(stream, "%s, ", GetMapTran(pmap_alarm,((WarnRec.WarnData.wCurrent_Err1<<16) | WarnRec.WarnData.wCurrent_Err0)));
        //time
        FilePrintf(stream, "%s, ", (const char*)WarnRec.time);
        //open cnt
        FilePrintf(stream, "%ld, ", WarnRec.WarnData.wCn_MoldNum_H<<16 | WarnRec.WarnData.wCn_MoldNum_L);
        //io
        for(i = 0; i < 6; ++i)
        {
            FilePrintf(stream, "%d, ", WarnRec.WarnData.wPI_Input[i]);
        }
        for(i = 0; i < 6; ++i)
        {
            FilePrintf(stream, "%d, ", WarnRec.WarnData.wPO_Output[i]);
        }
        //AD
        for(i = 0; i < ADMETER_CH_NUM; ++i)
        {
            if(WarnRec.wADPosi_Symbol[i] == 1)
            {
                snprintf(cbuf, sizeof(cbuf)-1, "%d", WarnRec.WarnData.wADPosi_Relative[i]);
            }
            else
            {
                snprintf(cbuf, sizeof(cbuf)-1, "-%d", WarnRec.WarnData.wADPosi_Relative[i]);
            }
            FilePrintf(stream, "%s, " ,(const char*)cbuf);
        }
        for(i = 0; i < ADMETER_CH_NUM; ++i)
        {
            FilePrintf(stream, "%d, ", WarnRec.WarnData.wAd_Relative[i]);
        }

        FilePrintf(stream, "%d, " , WarnRec.WarnData.wPort_Adtemp);
        FilePrintf(stream, "%d, " , WarnRec.WarnData.wAd_value);
        FilePrintf(stream, "%d, " , WarnRec.WarnData.wTemp_lm37);
        for(i = 0; i < 10; ++i)
        {
            FilePrintf(stream, "%d, ", WarnRec.WarnData.wTemp_real[i]);
        }

        //act
        FilePrintf(stream, "%X, ",WarnRec.Act_Data[0].wAct_Id);
        FilePrintf(stream, "%X, ",WarnRec.Act_Data[0].wStep);
        FilePrintf(stream, "%.3f, ",WarnRec.Act_Data[0].wStart_ms/1000.00);
        FilePrintf(stream, "%X, ",WarnRec.Act_Data[1].wAct_Id);
        FilePrintf(stream, "%X, ",WarnRec.Act_Data[1].wStep);
        FilePrintf(stream, "%.3f, ",WarnRec.Act_Data[1].wStart_ms/1000.00);
        FilePrintf(stream, "%X, ",WarnRec.Act_Data[2].wAct_Id);
        FilePrintf(stream, "%X, ",WarnRec.Act_Data[2].wStep);
        FilePrintf(stream, "%.3f, ",WarnRec.Act_Data[2].wStart_ms/1000.00);

        //daoutput
        for(i=0; i<8; ++i)
        {
            FilePrintf(stream, "%d, ", WarnRec.WarnData.wDA_Value[i]);
        }

        //driver
        for(i=0; i<6; ++i)
        {
            FilePrintf(stream, "%X, ", WarnRec.WarnData.wServoErr[i]);
        }
        for(i=0; i<6; ++i)
        {
            if (WarnRec.WarnData.wEle_SpdOut_RPM[i] >= 32768)
                snprintf(cbuf, sizeof(cbuf)-1, "-%d", 65536-WarnRec.WarnData.wEle_SpdOut_RPM[i]);
            else
                snprintf(cbuf, sizeof(cbuf)-1, "%d", WarnRec.WarnData.wEle_SpdOut_RPM[i]);
            FilePrintf(stream, "%s, ", (const char*)cbuf);
        }
        for(i=0; i<6; ++i)
        {
            if (WarnRec.WarnData.wEle_Real_Spd[i] >= 32768)
                snprintf(cbuf, sizeof(cbuf)-1, "-%d", 65536-WarnRec.WarnData.wEle_Real_Spd[i]);
            else
                snprintf(cbuf, sizeof(cbuf)-1, "%d", WarnRec.WarnData.wEle_Real_Spd[i]);
            FilePrintf(stream, "%s, ", (const char*)cbuf);
        }
        for(i=0; i<6; ++i)
        {
            if (WarnRec.WarnData.wEle_Real_Torque[i] >= 32768)
                snprintf(cbuf, sizeof(cbuf)-1, "-%d", 65536-WarnRec.WarnData.wEle_Real_Torque[i]);
            else
                snprintf(cbuf, sizeof(cbuf)-1, "%d", WarnRec.WarnData.wEle_Real_Torque[i]);
            FilePrintf(stream, "%s, ", (const char*)cbuf);
        }
//--------record 1 end-----------

//--------record 2 start---------
        //panel
        FilePrintf(stream, "%d, ", WarnRec.VersionInfo.wPanelType);
        FilePrintf(stream, "%d, ", WarnRec.VersionInfo.wPanelFactory);
        if(WarnRec.VersionInfo.wPanelVer1 < 10){
            snprintf(cbuf, sizeof(cbuf)-1, "%u%u%u%u", 0,0,0,WarnRec.VersionInfo.wPanelVer1);
        }
        else if(WarnRec.VersionInfo.wPanelVer1 < 100){
            snprintf(cbuf, sizeof(cbuf)-1, "%u%u%u", 0,0,WarnRec.VersionInfo.wPanelVer1);
        }
        else if(WarnRec.VersionInfo.wPanelVer1 < 1000){
            snprintf(cbuf, sizeof(cbuf)-1, "%u%u", 0,WarnRec.VersionInfo.wPanelVer1);
        }
        else{
            snprintf(cbuf, sizeof(cbuf)-1, "%u", WarnRec.VersionInfo.wPanelVer1);
        }
        if(WarnRec.VersionInfo.wPanelVer2 < 10){
            snprintf(cbuf2, sizeof(cbuf2)-1, "%u%u%u%u", 0,0,0,WarnRec.VersionInfo.wPanelVer2);
        }
        else if(WarnRec.VersionInfo.wPanelVer2 < 100){
            snprintf(cbuf2, sizeof(cbuf2)-1, "%u%u%u", 0,0,WarnRec.VersionInfo.wPanelVer2);
        }
        else if(WarnRec.VersionInfo.wPanelVer2 < 1000){
            snprintf(cbuf2, sizeof(cbuf2)-1, "%u%u", 0,WarnRec.VersionInfo.wPanelVer2);
        }
        else{
            snprintf(cbuf2, sizeof(cbuf2)-1, "%u", WarnRec.VersionInfo.wPanelVer2);
        }
        FilePrintf(stream, "%s %s, ", cbuf, cbuf2);

        FilePrintf(stream, "%d, ", WarnRec.VersionInfo.wPanelDate1*10000+WarnRec.VersionInfo.wPanelDate2);

        //host
        FilePrintf(stream, "%d, ", WarnRec.VersionInfo.wHostFactory);

        if(WarnRec.VersionInfo.wHostVer1 < 1000){
            snprintf(cbuf, sizeof(cbuf)-1, "%u%u", 0, WarnRec.VersionInfo.wHostVer1);
        }
        else{
            snprintf(cbuf, sizeof(cbuf)-1, "%u", WarnRec.VersionInfo.wHostVer1);
        }
//        FilePrintf(stream, "%s, ", cbuf);

        if(WarnRec.VersionInfo.wHostVer2 < 10){
            snprintf(cbuf2, sizeof(cbuf2)-1, "%u%u%u%u", 0,0,0, WarnRec.VersionInfo.wHostVer2);
        }
        else if(WarnRec.VersionInfo.wHostVer2 < 100){
            snprintf(cbuf2, sizeof(cbuf2)-1, "%u%u%u", 0,0,WarnRec.VersionInfo.wHostVer2);
        }
        else if(WarnRec.VersionInfo.wHostVer2 < 1000){
            snprintf(cbuf2, sizeof(cbuf2)-1, "%u%u", 0,WarnRec.VersionInfo.wHostVer2);
        }
        else{
            snprintf(cbuf2, sizeof(cbuf2)-1, "%u", WarnRec.VersionInfo.wHostVer2);
        }
        FilePrintf(stream, "%s %s, ", cbuf, cbuf2);
        FilePrintf(stream, "%d, ", WarnRec.VersionInfo.wHostDate1+(WarnRec.VersionInfo.wHostDate2&255)*65536 + 20000000);
//        FilePrintf(stream, "%s, ", WarnRec.VersionInfo.wBoot1);
        snprintf(cbuf, sizeof(cbuf)-1, "%u", (WarnRec.VersionInfo.wBoot1%100+2000)*10000+WarnRec.VersionInfo.wBoot2);
        FilePrintf(stream, "%d %s, ", WarnRec.VersionInfo.wBoot1, cbuf);
        snprintf(cbuf, sizeof(cbuf)-1, "%u", WarnRec.VersionInfo.wHardware&0x1FFF);
        FilePrintf(stream, "%s, ", cbuf);

        //code
        FilePrintf(stream, "%d, ", WarnRec.MachCfg.wver);
        FilePrintf(stream, "%X, ", WarnRec.MachCfg.wCode0);
        FilePrintf(stream, "%X, ", WarnRec.MachCfg.wCode1);
        FilePrintf(stream, "%X, ", WarnRec.MachCfg.wCode2);
        FilePrintf(stream, "%X, ", WarnRec.MachCfg.wCode3);
        FilePrintf(stream, "%X, ", WarnRec.MachCfg.wCode4);
        FilePrintf(stream, "%X, ", WarnRec.MachCfg.wCode5);
        FilePrintf(stream, "%X, ", WarnRec.MachCfg.wFactory);
        FilePrintf(stream, "%X, ", WarnRec.MachCfg.wCode7);
        FilePrintf(stream, "%X, ", WarnRec.MachCfg.wHydraulic);

        FilePrintf(stream, "%d, ", WarnRec.MachCfg.wVerInfo);
        FilePrintf(stream, "%X, ", WarnRec.MachCfg.wTemp);
        FilePrintf(stream, "%X, ", WarnRec.MachCfg.wMeter);
        FilePrintf(stream, "%X, ", WarnRec.MachCfg.wCode11);
        FilePrintf(stream, "%X, ", WarnRec.MachCfg.wCode12);
        FilePrintf(stream, "%X, ", WarnRec.MachCfg.wSystem);
        FilePrintf(stream, "%d, ", WarnRec.MachCfg.wSysPrs);
        FilePrintf(stream, "%X, ", WarnRec.MachCfg.wCode16);
        FilePrintf(stream, "%d, ", WarnRec.MachCfg.wRpmCnt);

        if((WarnRec.MachCfg.wReserve[0]) > 100)
        {
            FilePrintf(stream, "%d%c%c, ", WarnRec.MachCfg.wPushCode,(WarnRec.MachCfg.wReserve[0])/100,(WarnRec.MachCfg.wReserve[0])%100);
        }
        else
        {
            if((WarnRec.MachCfg.wReserve[0]) == 0)
            {
                FilePrintf(stream, "%d, ", WarnRec.MachCfg.wPushCode);
            }
            else
            {
                FilePrintf(stream, "%d%c, ", WarnRec.MachCfg.wPushCode,(WarnRec.MachCfg.wReserve[0])%100);
            }
        }

        FilePrintf(stream, "%X, ", WarnRec.MachCfg.wCode43);
        FilePrintf(stream, "%X, ", WarnRec.MachCfg.wCode48);
        FilePrintf(stream, "%X, ", WarnRec.MachCfg.wCode49);
        FilePrintf(stream, "%X, ", WarnRec.MachCfg.wCode50);
        FilePrintf(stream, "%X, ", WarnRec.MachCfg.wCode51);
        FilePrintf(stream, "%X, ", WarnRec.MachCfg.wCode52);
        FilePrintf(stream, "%X, ", WarnRec.MachCfg.wCode53);
        FilePrintf(stream, "%X, ", WarnRec.MachCfg.wCode54);
        FilePrintf(stream, "%X, ", WarnRec.MachCfg.wQVR);

        FilePrintf( stream, "\n");
    }
        FileClose(stream);
        return TRUE;
}*/

//20230518 chj 原先的fileprintf方式有问题，每次调用都会写文件
BOOL SaveWarnDataExcel(char* tem_str)
{
    TSTRING_CREATE(str_tran);

    FILEHD stream;
    char buf[255];
    char * p_str;
    char cbuf[20];
    char cbuf2[20];
    int i, j;
    UI8 bs[3] = {(UI8)0xEF, (UI8)0xBB, (UI8)0xBF};

    if(WDisEmpty(&m_wdrecord))
    {
        TSTRING_DESTROY(str_tran);
        return FALSE;
    }

    sprintf(buf,"%s%s%s",USB_BOOT_PATH,tem_str,".csv");

    p_str = ConvToGBK_Dup(buf);

    if(!FileExist(p_str))
    {
       stream = FileCreate(p_str);
    }
    else
    {
       stream = FileOpen(p_str);
    }

    OBJMemFree(p_str);

    if(!FILEHD_IS_OK(stream))
    {
        TSTRING_DESTROY(str_tran);
        return FALSE;
    }

    FileWriteUdisk(stream, bs, sizeof(bs), NULL);//20240923 chj FileWrite-》替换为 FileWriteUdisk,调用后，底层过100ms，主动sync

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNDATA_PAGE, "Alarm Description"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Alarm Time"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Open Count"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Input #1"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Input #2"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Input #3"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Input #4"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Input #5"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Input #6"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Output #1"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Output #2"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Output #3"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Output #4"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Output #5"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Output #6"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "AD1"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "AD2"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "AD3"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "AD4"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "AD5"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "AD6"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "AD7"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "AD8"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "AD Collect1"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "AD Collect2"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "AD Collect3"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "AD Collect4"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "AD Collect5"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "AD Collect6"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "AD Collect7"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "AD Collect8"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Temp Scan Chnl"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Temp AD"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Temp Room"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Temp Real"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-1", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Temp Real"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-2", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Temp Real"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-3", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Temp Real"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-4", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Temp Real"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-5", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Temp Real"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-6", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Temp Real"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-7", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Temp Real"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-8", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Temp Real"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-9", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Temp Real"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-10", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Act #1 ID"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Act #1 Step"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Act #1 Time"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Act #2 ID"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Act #2 Step"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Act #2 Time"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Act #3 ID"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Act #3 Step"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Act #3 Time"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "DA Output"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-1", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "DA Output"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-2", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "DA Output"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-3", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "DA Output"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-4", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "DA Output"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-5", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "DA Output"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-6", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "DA Output"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-7", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "DA Output"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-8", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Alarm Code"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-1", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Alarm Code"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-2", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Alarm Code"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-3", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Alarm Code"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-4", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Alarm Code"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-5", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Alarm Code"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-6", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Cmd Spd"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-1", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Cmd Spd"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-2", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Cmd Spd"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-3", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Cmd Spd"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-4", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Cmd Spd"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-5", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Cmd Spd"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-6", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Motor Spd"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-1", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Motor Spd"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-2", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Motor Spd"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-3", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Motor Spd"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-4", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Motor Spd"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-5", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Motor Spd"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-6", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Current Torque"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-1", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Current Torque"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-2", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Current Torque"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-3", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Current Torque"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-4", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Current Torque"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-5", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Current Torque"), MIN_ALLOC_SIZE);
    tstring_appends_fix(&str_tran, "-6", MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Panel View Type"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Panel Factory"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Panel Version"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Panel Date"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Host Factory"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Host Version"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Host Date"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Boot Version"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Hardware"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Machine Config Version"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Code 0"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Code 1"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Code 2"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Code 3"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Code 4"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Code 5"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Manufact"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Code 7"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Hydr Ctrl"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Machine Config Ver Date"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Temp Ctrl"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Pos MT Ctrl"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Code 11"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Code 12"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "System Code"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "SysPrs Max"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Code 16"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "RPM Hole Cnt"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Push Code"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Code 43"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Code 48"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Code 49"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Code 50"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Code 51"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Code 52"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Code 53"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "Code 54"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_appends_fix(&str_tran, GetPageLangStr(WARNHELP_PAGE, "QVR"), MIN_ALLOC_SIZE);
    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

    tstring_append_fix(&str_tran, '\n', MIN_ALLOC_SIZE);

    char tmp[20];
    for(j = 0; j<WDNum(&m_wdrecord); ++j)
    {
        WarnRec = m_wdrecord.item[(m_wdrecord.wdhead.rear - j - 1 + MAX_WARNDATA_SIZE) % MAX_WARNDATA_SIZE], sizeof(WDITEM);

        //warn name
        tstring_appends_fix(&str_tran, GetMapTran(pmap_alarm,((WarnRec.WarnData.wCurrent_Err1<<16) | WarnRec.WarnData.wCurrent_Err0)), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);
        //time
        tstring_appends_fix(&str_tran, WarnRec.time, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        //open cnt
        WordToStr(tmp,WarnRec.WarnData.wCn_MoldNum_H<<16 | WarnRec.WarnData.wCn_MoldNum_L,0);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        //io
        for(i = 0; i < 6; ++i)
        {
            WordToStr(tmp,WarnRec.WarnData.wPI_Input[i],0);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);
        }
        for(i = 0; i < 6; ++i)
        {
            WordToStr(tmp,WarnRec.WarnData.wPO_Output[i],0);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);
        }
        //AD
        for(i = 0; i < ADMETER_CH_NUM; ++i)
        {
            if(WarnRec.wADPosi_Symbol[i] == 1)
            {
                snprintf(cbuf, sizeof(cbuf)-1, "%d", WarnRec.WarnData.wADPosi_Relative[i]);
            }
            else
            {
                snprintf(cbuf, sizeof(cbuf)-1, "-%d", WarnRec.WarnData.wADPosi_Relative[i]);
            }

            tstring_appends_fix(&str_tran, cbuf, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);
        }
        for(i = 0; i < ADMETER_CH_NUM; ++i)
        {
            WordToStr(tmp,WarnRec.WarnData.wAd_Relative[i],0);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);
        }

        WordToStr(tmp,WarnRec.WarnData.wPort_Adtemp,0);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.WarnData.wAd_value,0);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.WarnData.wTemp_lm37,0);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        for(i = 0; i < 10; ++i)
        {
            WordToStr(tmp,WarnRec.WarnData.wTemp_real[i],0);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);
        }

        //act
        WordToStr(tmp,WarnRec.Act_Data[0].wAct_Id,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.Act_Data[0].wStep,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.Act_Data[0].wStart_ms,3);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.Act_Data[1].wAct_Id,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.Act_Data[1].wStep,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.Act_Data[1].wStart_ms,3);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.Act_Data[2].wAct_Id,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.Act_Data[2].wStep,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.Act_Data[2].wStart_ms,3);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        //daoutput
        for(i=0; i<8; ++i)
        {
            WordToStr(tmp, WarnRec.WarnData.wDA_Value[i],0);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);
        }

        //driver
        for(i=0; i<6; ++i)
        {
            WordToStr(tmp,WarnRec.WarnData.wServoErr[i],HEXFLAG);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);
        }
        for(i=0; i<6; ++i)
        {
            if (WarnRec.WarnData.wEle_SpdOut_RPM[i] >= 32768)
                snprintf(cbuf, sizeof(cbuf)-1, "-%d", 65536-WarnRec.WarnData.wEle_SpdOut_RPM[i]);
            else
                snprintf(cbuf, sizeof(cbuf)-1, "%d", WarnRec.WarnData.wEle_SpdOut_RPM[i]);
            tstring_appends_fix(&str_tran, cbuf, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ' ', MIN_ALLOC_SIZE);

        }
        for(i=0; i<6; ++i)
        {
            if (WarnRec.WarnData.wEle_Real_Spd[i] >= 32768)
                snprintf(cbuf, sizeof(cbuf)-1, "-%d", 65536-WarnRec.WarnData.wEle_Real_Spd[i]);
            else
                snprintf(cbuf, sizeof(cbuf)-1, "%d", WarnRec.WarnData.wEle_Real_Spd[i]);
            tstring_appends_fix(&str_tran, cbuf, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ' ', MIN_ALLOC_SIZE);

        }
        for(i=0; i<6; ++i)
        {
            if (WarnRec.WarnData.wEle_Real_Torque[i] >= 32768)
                snprintf(cbuf, sizeof(cbuf)-1, "-%d", 65536-WarnRec.WarnData.wEle_Real_Torque[i]);
            else
                snprintf(cbuf, sizeof(cbuf)-1, "%d", WarnRec.WarnData.wEle_Real_Torque[i]);
            tstring_appends_fix(&str_tran, cbuf, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ' ', MIN_ALLOC_SIZE);

        }
//--------record 1 end-----------

//--------record 2 start---------
        //panel
        WordToStr(tmp,WarnRec.VersionInfo.wPanelType,0);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.VersionInfo.wPanelFactory,0);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        if(WarnRec.VersionInfo.wPanelVer1 < 10){
            snprintf(cbuf, sizeof(cbuf)-1, "%u%u%u%u", 0,0,0,WarnRec.VersionInfo.wPanelVer1);
        }
        else if(WarnRec.VersionInfo.wPanelVer1 < 100){
            snprintf(cbuf, sizeof(cbuf)-1, "%u%u%u", 0,0,WarnRec.VersionInfo.wPanelVer1);
        }
        else if(WarnRec.VersionInfo.wPanelVer1 < 1000){
            snprintf(cbuf, sizeof(cbuf)-1, "%u%u", 0,WarnRec.VersionInfo.wPanelVer1);
        }
        else{
            snprintf(cbuf, sizeof(cbuf)-1, "%u", WarnRec.VersionInfo.wPanelVer1);
        }
        if(WarnRec.VersionInfo.wPanelVer2 < 10){
            snprintf(cbuf2, sizeof(cbuf2)-1, "%u%u%u%u", 0,0,0,WarnRec.VersionInfo.wPanelVer2);
        }
        else if(WarnRec.VersionInfo.wPanelVer2 < 100){
            snprintf(cbuf2, sizeof(cbuf2)-1, "%u%u%u", 0,0,WarnRec.VersionInfo.wPanelVer2);
        }
        else if(WarnRec.VersionInfo.wPanelVer2 < 1000){
            snprintf(cbuf2, sizeof(cbuf2)-1, "%u%u", 0,WarnRec.VersionInfo.wPanelVer2);
        }
        else{
            snprintf(cbuf2, sizeof(cbuf2)-1, "%u", WarnRec.VersionInfo.wPanelVer2);
        }
        tstring_appends_fix(&str_tran, cbuf, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ' ', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, cbuf2, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.VersionInfo.wPanelDate1*10000+WarnRec.VersionInfo.wPanelDate2,0);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        //host
        WordToStr(tmp, WarnRec.VersionInfo.wHostFactory,0);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        if(WarnRec.VersionInfo.wHostVer1 < 1000){
            snprintf(cbuf, sizeof(cbuf)-1, "%u%u", 0, WarnRec.VersionInfo.wHostVer1);
        }
        else{
            snprintf(cbuf, sizeof(cbuf)-1, "%u", WarnRec.VersionInfo.wHostVer1);
        }

        if(WarnRec.VersionInfo.wHostVer2 < 10){
            snprintf(cbuf2, sizeof(cbuf2)-1, "%u%u%u%u", 0,0,0, WarnRec.VersionInfo.wHostVer2);
        }
        else if(WarnRec.VersionInfo.wHostVer2 < 100){
            snprintf(cbuf2, sizeof(cbuf2)-1, "%u%u%u", 0,0,WarnRec.VersionInfo.wHostVer2);
        }
        else if(WarnRec.VersionInfo.wHostVer2 < 1000){
            snprintf(cbuf2, sizeof(cbuf2)-1, "%u%u", 0,WarnRec.VersionInfo.wHostVer2);
        }
        else{
            snprintf(cbuf2, sizeof(cbuf2)-1, "%u", WarnRec.VersionInfo.wHostVer2);
        }
        tstring_appends_fix(&str_tran, cbuf, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ' ', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, cbuf2, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);


        WordToStr(tmp,WarnRec.VersionInfo.wHostDate1+(WarnRec.VersionInfo.wHostDate2&255)*65536 + 20000000,0);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        snprintf(cbuf, sizeof(cbuf)-1, "%u", (WarnRec.VersionInfo.wBoot1%100+2000)*10000+WarnRec.VersionInfo.wBoot2);
        tstring_appends_fix(&str_tran, cbuf, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        snprintf(cbuf, sizeof(cbuf)-1, "%u", WarnRec.VersionInfo.wHardware&0x1FFF);
        tstring_appends_fix(&str_tran, cbuf, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        //code
        WordToStr(tmp,WarnRec.MachCfg.wver,0);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.MachCfg.wCode0,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.MachCfg.wCode1,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.MachCfg.wCode2,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.MachCfg.wCode3,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.MachCfg.wCode4,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.MachCfg.wCode5,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.MachCfg.wFactory,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.MachCfg.wCode7,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.MachCfg.wHydraulic,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.MachCfg.wVerInfo,0);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.MachCfg.wTemp,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.MachCfg.wMeter,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.MachCfg.wCode11,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.MachCfg.wCode12,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.MachCfg.wSystem,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.MachCfg.wSysPrs,0);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.MachCfg.wCode16,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.MachCfg.wRpmCnt,0);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);


        if((WarnRec.MachCfg.wReserve[0]) > 100)
        {
            WordToStr(tmp,WarnRec.MachCfg.wPushCode,0);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, (WarnRec.MachCfg.wReserve[0])/100, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, (WarnRec.MachCfg.wReserve[0])%100, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);
        }
        else
        {
            if((WarnRec.MachCfg.wReserve[0]) == 0)
            {
                WordToStr(tmp, WarnRec.MachCfg.wPushCode,0);
                tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
                tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);
            }
            else
            {
                WordToStr(tmp,WarnRec.MachCfg.wPushCode,0);
                tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
                tstring_append_fix(&str_tran, (WarnRec.MachCfg.wReserve[0])%100, MIN_ALLOC_SIZE);
                tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);
            }
        }


        WordToStr(tmp,WarnRec.MachCfg.wCode43,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.MachCfg.wCode48,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.MachCfg.wCode49,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.MachCfg.wCode50,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.MachCfg.wCode51,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.MachCfg.wCode52,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.MachCfg.wCode53,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.MachCfg.wCode54,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        WordToStr(tmp,WarnRec.MachCfg.wQVR,HEXFLAG);
        tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_append_fix(&str_tran, '\n', MIN_ALLOC_SIZE);
    }
    oprintf("str_tran.len:%ld\n",str_tran.len);
    FileWriteUdisk(stream, str_tran.str, str_tran.len, NULL);//20240923 chj FileWrite-》替换为 FileWriteUdisk,调用后，底层过100ms，主动sync
    TSTRING_DESTROY(str_tran);
    FileClose(stream);
    return TRUE;
}


static BOOL WarnDataFlg = FALSE;
void SetWarnDataFlg(BOOL flg)
{
    WarnDataFlg = flg;
}

BOOL GetWarnDataFlg()
{
    return WarnDataFlg;
}
