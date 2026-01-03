#include "lubr.h"
#include "time.h"
#include "stdio.h"
#include "file.h"
#include "obj.h"
#include "database.h"
#include "ctrlcmd.h"
#include "protcmd.h"
#include "oprintf.h"

UI32 HMD_LUBHOUR1 = 0;
UI32 HMD_LeftLUBHOUR1 = 0;
UI32 HMD_LUBHOUR2 = 0;
UI32 HMD_LeftLUBHOUR2 = 0;
UI32 HMD_LUBHOUR3 = 0;
UI32 HMD_LeftLUBHOUR3 = 0;
DATETIME HMD_LubStrTm1;
DATETIME HMD_LubStrTm2;
DATETIME HMD_LubStrTm3;

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

void SaveHMDLub(int dst,void* src,UI16 wCount)
{
    FILEHD filehd;

    if(FileExist(HOUR_LUB_PATH))
    {
        filehd = FileOpen(HOUR_LUB_PATH);
    }
    else
    {
        filehd = FileCreate(HOUR_LUB_PATH);
    }

    if(CheckFileValid(filehd))
    {
        FileSeek(filehd, dst);
        FileWrite(filehd, src, wCount, NULL);
        FileClose(filehd);
    }
}

BOOL LoadHMDLub(void* dst,int src,UI16 wCount)
{
    FILEHD filehd;
    filehd = FileOpen(HOUR_LUB_PATH);
    if(CheckFileValid(filehd))
    {
        FileSeek(filehd, src);
        FileRead(filehd, dst, wCount, NULL);
        FileClose(filehd);
        return TRUE;
    }
    else
    {
        filehd = FileCreate(HOUR_LUB_PATH);
        FileClose(filehd);
        return FALSE;
    }
}

void CheckHMDLubSave()
{
    BOOL bsave = false;
    struct tm* t;
    struct tm t_save;
    time_t time_now;
    time_t time_save;

    time(&time_now);
    t = localtime(&time_now);

    //check lub 1
    if((HMD_LubStrTm1.year == 0)/* || (HMD_LubStrTm1.month == 0)*/ || (HMD_LubStrTm1.day == 0))
    {
        bsave = true;
    }
    else
    {
        t_save.tm_year = HMD_LubStrTm1.year;
        t_save.tm_mon = HMD_LubStrTm1.month;
        t_save.tm_mday = HMD_LubStrTm1.day;
        t_save.tm_hour = HMD_LubStrTm1.hour;
        t_save.tm_min = HMD_LubStrTm1.minute;
        t_save.tm_sec = HMD_LubStrTm1.second;
        time_save = mktime(&t_save);
        if(time_save > time_now) //文件保存的时间，大于今天，则为错误时间，重新保存日期
        {
            bsave = true;
        }
    }

    if(bsave)
    {
        HMD_LubStrTm1.year = t->tm_year;
        HMD_LubStrTm1.month = t->tm_mon;
        HMD_LubStrTm1.day = t->tm_mday;
        HMD_LubStrTm1.hour = t->tm_hour;
        HMD_LubStrTm1.minute = t->tm_min;
        HMD_LubStrTm1.second = t->tm_sec;
        SaveHMDLub(0,&HMD_LubStrTm1,sizeof(DATETIME));
        bsave = false;
    }

    //check lub ２
    if((HMD_LubStrTm2.year == 0)/* || (HMD_LubStrTm2.month == 0)*/ || (HMD_LubStrTm2.day == 0))
    {
        bsave = true;
    }
    else
    {
        t_save.tm_year = HMD_LubStrTm2.year;
        t_save.tm_mon = HMD_LubStrTm2.month;
        t_save.tm_mday = HMD_LubStrTm2.day;
        t_save.tm_hour = HMD_LubStrTm2.hour;
        t_save.tm_min = HMD_LubStrTm2.minute;
        t_save.tm_sec = HMD_LubStrTm2.second;
        time_save = mktime(&t_save);
        if(time_save > time_now) //文件保存的时间，大于今天，则为错误时间，重新保存日期
        {
            bsave = true;
        }
    }

    if(bsave)
    {
        HMD_LubStrTm2.year = t->tm_year;
        HMD_LubStrTm2.month = t->tm_mon;
        HMD_LubStrTm2.day = t->tm_mday;
        HMD_LubStrTm2.hour = t->tm_hour;
        HMD_LubStrTm2.minute = t->tm_min;
        HMD_LubStrTm2.second = t->tm_sec;
        SaveHMDLub(sizeof(DATETIME),&HMD_LubStrTm2,sizeof(DATETIME));
        bsave = false;
    }

    //check lub ３
    if((HMD_LubStrTm3.year == 0)/* || (HMD_LubStrTm3.month == 0)*/ || (HMD_LubStrTm3.day == 0))
    {
        bsave = true;
    }
    else
    {
        t_save.tm_year = HMD_LubStrTm3.year;
        t_save.tm_mon = HMD_LubStrTm3.month;
        t_save.tm_mday = HMD_LubStrTm3.day;
        t_save.tm_hour = HMD_LubStrTm3.hour;
        t_save.tm_min = HMD_LubStrTm3.minute;
        t_save.tm_sec = HMD_LubStrTm3.second;
        time_save = mktime(&t_save);
        if(time_save > time_now) //文件保存的时间，大于今天，则为错误时间，重新保存日期
        {
            bsave = true;
        }
    }

    if(bsave)
    {
        HMD_LubStrTm3.year = t->tm_year;
        HMD_LubStrTm3.month = t->tm_mon;
        HMD_LubStrTm3.day = t->tm_mday;
        HMD_LubStrTm3.hour = t->tm_hour;
        HMD_LubStrTm3.minute = t->tm_min;
        HMD_LubStrTm3.second = t->tm_sec;
        SaveHMDLub(2*sizeof(DATETIME),&HMD_LubStrTm3,sizeof(DATETIME));
        bsave = false;
    }
}

void CalculateHMDLub()
{
    struct tm* t;
    struct tm t_save;
    time_t time_now;
    time_t time_save;;

    time(&time_now);
    t = localtime(&time_now);

    //lub 1
    t_save.tm_year = HMD_LubStrTm1.year;
    t_save.tm_mon = HMD_LubStrTm1.month;
    t_save.tm_mday = HMD_LubStrTm1.day;
    t_save.tm_hour = HMD_LubStrTm1.hour;
    t_save.tm_min = HMD_LubStrTm1.minute;
    t_save.tm_sec = HMD_LubStrTm1.second;
    time_save = mktime(&t_save);

    HMD_LUBHOUR1 = (time_now - time_save)/60;
    if(VarAdrToInt(p_PP_MACHSET_wUse15DaysLub1) == 0)
    {
        HMD_LeftLUBHOUR1 = 0;
    }
    else
    {
        if(HMD_LUBHOUR1 > (VarAdrToInt(p_PP_MACHSET_wUse15DaysLub1)*60))
        {
            HMD_LeftLUBHOUR1 = 0;
        }
        else
        {
            HMD_LeftLUBHOUR1 = VarAdrToInt(p_PP_MACHSET_wUse15DaysLub1)*60 - HMD_LUBHOUR1;
        }
    }

    //lub 2
    t_save.tm_year = HMD_LubStrTm2.year;
    t_save.tm_mon = HMD_LubStrTm2.month;
    t_save.tm_mday = HMD_LubStrTm2.day;
    t_save.tm_hour = HMD_LubStrTm2.hour;
    t_save.tm_min = HMD_LubStrTm2.minute;
    t_save.tm_sec = HMD_LubStrTm2.second;
    time_save = mktime(&t_save);

    HMD_LUBHOUR2 = (time_now - time_save)/60;
    if(VarAdrToInt(p_PP_MACHSET_wUse15DaysLub2) == 0)
    {
        HMD_LeftLUBHOUR2 = 0;
    }
    else
    {
        if(HMD_LUBHOUR2 > (VarAdrToInt(p_PP_MACHSET_wUse15DaysLub2)*60))
        {
            HMD_LeftLUBHOUR2 = 0;
        }
        else
        {
            HMD_LeftLUBHOUR2 = VarAdrToInt(p_PP_MACHSET_wUse15DaysLub2)*60 - HMD_LUBHOUR2;
        }
    }

    //lub 3
    t_save.tm_year = HMD_LubStrTm3.year;
    t_save.tm_mon = HMD_LubStrTm3.month;
    t_save.tm_mday = HMD_LubStrTm3.day;
    t_save.tm_hour = HMD_LubStrTm3.hour;
    t_save.tm_min = HMD_LubStrTm3.minute;
    t_save.tm_sec = HMD_LubStrTm3.second;
    time_save = mktime(&t_save);

    HMD_LUBHOUR3 = (time_now - time_save)/60;
    if(VarAdrToInt(p_PP_MACHSET_wUse15DaysLub3) == 0)
    {
        HMD_LeftLUBHOUR3 = 0;
    }
    else
    {
        if(HMD_LUBHOUR3 > (VarAdrToInt(p_PP_MACHSET_wUse15DaysLub3)*60))
        {
            HMD_LeftLUBHOUR3 = 0;
        }
        else
        {
            HMD_LeftLUBHOUR3 = VarAdrToInt(p_PP_MACHSET_wUse15DaysLub3)*60 - HMD_LUBHOUR3;
        }
    }
}


void UpdateHMDLub()
{
    if(VarAdrToInt(d_lub1_MACHSET_FL_LUBMODE) == 3)
    {
        HMD_LUBHOUR1 = HMD_LUBHOUR1 + 5;
        CheckHMDLubArrive(1);
    }
    if(VarAdrToInt(d_lub2_MACHSET_FL_LUBMODE) == 3)
    {
        HMD_LUBHOUR2 = HMD_LUBHOUR2 + 5;
        CheckHMDLubArrive(2);
    }
    if(VarAdrToInt(d_lub3_MACHSET_FL_LUBMODE) == 3)
    {
        HMD_LUBHOUR3 = HMD_LUBHOUR3 + 5;
        CheckHMDLubArrive(3);
    }

}
void CheckHMDLubArrive(UI16 lubno)
{
    struct tm* t;
    time_t time_now;
    UI16 pdata[1];

    time(&time_now);
    t = localtime(&time_now);

    switch(lubno)
    {
        case 1:
            if(VarAdrToInt(p_PP_MACHSET_wUse15DaysLub1) != 0)
            {
                if(HMD_LUBHOUR1 >= (VarAdrToInt(p_PP_MACHSET_wUse15DaysLub1) * 60))//分
                {
                    pdata[0] = 1;
                    ProtSysCtrl(COMM_SUBCMD_NEEDLUB,1,pdata);

                    HMD_LUBHOUR1 = 0;
                    HMD_LubStrTm1.year = t->tm_year;
                    HMD_LubStrTm1.month = t->tm_mon;
                    HMD_LubStrTm1.day = t->tm_mday;
                    HMD_LubStrTm1.hour = t->tm_hour;
                    HMD_LubStrTm1.minute = t->tm_min;
                    HMD_LubStrTm1.second = t->tm_sec;
                    SaveHMDLub(0,&HMD_LubStrTm1,sizeof(DATETIME));
                }

                HMD_LeftLUBHOUR1 = VarAdrToInt(p_PP_MACHSET_wUse15DaysLub1)*60 - HMD_LUBHOUR1;
            }
            break;
        case 2:
            if(VarAdrToInt(p_PP_MACHSET_wUse15DaysLub2) != 0)
            {
                if(HMD_LUBHOUR2 >= (VarAdrToInt(p_PP_MACHSET_wUse15DaysLub2) * 60))//分
                {
                    pdata[0] = 2;
                    ProtSysCtrl(COMM_SUBCMD_NEEDLUB,1,pdata);

                    HMD_LUBHOUR2 = 0;
                    HMD_LubStrTm2.year = t->tm_year;
                    HMD_LubStrTm2.month = t->tm_mon;
                    HMD_LubStrTm2.day = t->tm_mday;
                    HMD_LubStrTm2.hour = t->tm_hour;
                    HMD_LubStrTm2.minute = t->tm_min;
                    HMD_LubStrTm2.second = t->tm_sec;
                    SaveHMDLub(sizeof(DATETIME),&HMD_LubStrTm2,sizeof(DATETIME));
                }

                HMD_LeftLUBHOUR2 = VarAdrToInt(p_PP_MACHSET_wUse15DaysLub2)*60 - HMD_LUBHOUR2;
            }
            break;
        case 3:
            if(VarAdrToInt(p_PP_MACHSET_wUse15DaysLub3) != 0)
            {
                if(HMD_LUBHOUR3 >= (VarAdrToInt(p_PP_MACHSET_wUse15DaysLub3) * 60))//分
                {
                    pdata[0] = 3;
                    ProtSysCtrl(COMM_SUBCMD_NEEDLUB,1,pdata);

                    HMD_LUBHOUR3 = 0;
                    HMD_LubStrTm3.year = t->tm_year;
                    HMD_LubStrTm3.month = t->tm_mon;
                    HMD_LubStrTm3.day = t->tm_mday;
                    HMD_LubStrTm3.hour = t->tm_hour;
                    HMD_LubStrTm3.minute = t->tm_min;
                    HMD_LubStrTm3.second = t->tm_sec;
                    SaveHMDLub(2*sizeof(DATETIME),&HMD_LubStrTm3,sizeof(DATETIME));
                }

                HMD_LeftLUBHOUR3 = VarAdrToInt(p_PP_MACHSET_wUse15DaysLub3)*60 - HMD_LUBHOUR3;
            }
            break;
        default:
            break;
    }
}

void CheckHMDLubCnt()//当润滑倒计数等于１时，润滑计时清零
{
    struct tm* t;
    time_t time_now;

    time(&time_now);
    t = localtime(&time_now);

    if(VarAdrToInt(p_PP_MACHSET_wUse15DaysLub1) != 0)
    {
        if(VarAdrToInt(d_lub1_STATE_LUB_CLMP_LEFT) == 0) //当润滑倒计数等于0时，润滑计时清零；
        {
            HMD_LUBHOUR1 = 0;
            HMD_LubStrTm1.year = t->tm_year;
            HMD_LubStrTm1.month = t->tm_mon;
            HMD_LubStrTm1.day = t->tm_mday;
            HMD_LubStrTm1.hour = t->tm_hour;
            HMD_LubStrTm1.minute = t->tm_min;
            HMD_LubStrTm1.second = t->tm_sec;
            oprintf("HMD_LubStrTm111111111111\n");
            SaveHMDLub(0,&HMD_LubStrTm1,sizeof(DATETIME));
            HMD_LeftLUBHOUR1 = VarAdrToInt(p_PP_MACHSET_wUse15DaysLub1)*60 - HMD_LUBHOUR1;
        }
    }
    if(VarAdrToInt(p_PP_MACHSET_wUse15DaysLub2) != 0)
    {
        if(VarAdrToInt(d_lub2_STATE_LUB_CLMP_LEFT) == 0)
        {
            HMD_LUBHOUR2 = 0;
            HMD_LubStrTm2.year = t->tm_year;
            HMD_LubStrTm2.month = t->tm_mon;
            HMD_LubStrTm2.day = t->tm_mday;
            HMD_LubStrTm2.hour = t->tm_hour;
            HMD_LubStrTm2.minute = t->tm_min;
            HMD_LubStrTm2.second = t->tm_sec;
            oprintf("HMD_LubStrTm222222222222\n");
            SaveHMDLub(sizeof(DATETIME),&HMD_LubStrTm2,sizeof(DATETIME));
            HMD_LeftLUBHOUR2 = VarAdrToInt(p_PP_MACHSET_wUse15DaysLub2)*60 - HMD_LUBHOUR2;
        }

    }
    if(VarAdrToInt(p_PP_MACHSET_wUse15DaysLub3) != 0)
    {
        if(VarAdrToInt(d_lub3_STATE_LUB_CLMP_LEFT) == 0)
        {
            HMD_LUBHOUR3 = 0;
            HMD_LubStrTm3.year = t->tm_year;
            HMD_LubStrTm3.month = t->tm_mon;
            HMD_LubStrTm3.day = t->tm_mday;
            HMD_LubStrTm3.hour = t->tm_hour;
            HMD_LubStrTm3.minute = t->tm_min;
            HMD_LubStrTm3.second = t->tm_sec;
            oprintf("HMD_LubStrTm33333333333\n");
            SaveHMDLub(2*sizeof(DATETIME),&HMD_LubStrTm3,sizeof(DATETIME));
            HMD_LeftLUBHOUR3 = VarAdrToInt(p_PP_MACHSET_wUse15DaysLub3)*60 - HMD_LUBHOUR3;
        }
    }
}

void InitHMDLub()
{
    memset(&HMD_LubStrTm1,0,sizeof(DATETIME));
    memset(&HMD_LubStrTm2,0,sizeof(DATETIME));
    memset(&HMD_LubStrTm3,0,sizeof(DATETIME));

    LoadHMDLub(&HMD_LubStrTm1,0,sizeof(DATETIME));
    LoadHMDLub(&HMD_LubStrTm2,1*sizeof(DATETIME),sizeof(DATETIME));
    LoadHMDLub(&HMD_LubStrTm3,2*sizeof(DATETIME),sizeof(DATETIME));
}

void SaveLubStartTm(UI16 lubno)
{
    struct tm* t;
    time_t time_now;

    time(&time_now);
    t = localtime(&time_now);

    switch(lubno)
    {
    case 1:
        HMD_LubStrTm1.year = t->tm_year;
        HMD_LubStrTm1.month = t->tm_mon;
        HMD_LubStrTm1.day = t->tm_mday;
        HMD_LubStrTm1.hour = t->tm_hour;
        HMD_LubStrTm1.minute = t->tm_min;
        HMD_LubStrTm1.second = t->tm_sec;
        SaveHMDLub(0,&HMD_LubStrTm1,sizeof(DATETIME));

        HMD_LUBHOUR1 = 0;
        HMD_LeftLUBHOUR1 = VarAdrToInt(p_PP_MACHSET_wUse15DaysLub1)*60 - HMD_LUBHOUR1;
        break;
    case 2:
        HMD_LubStrTm2.year = t->tm_year;
        HMD_LubStrTm2.month = t->tm_mon;
        HMD_LubStrTm2.day = t->tm_mday;
        HMD_LubStrTm2.hour = t->tm_hour;
        HMD_LubStrTm2.minute = t->tm_min;
        HMD_LubStrTm2.second = t->tm_sec;
        SaveHMDLub(sizeof(DATETIME),&HMD_LubStrTm2,sizeof(DATETIME));

        HMD_LUBHOUR2 = 0;
        HMD_LeftLUBHOUR2 = VarAdrToInt(p_PP_MACHSET_wUse15DaysLub2)*60 - HMD_LUBHOUR2;
        break;
    case 3:
        HMD_LubStrTm3.year = t->tm_year;
        HMD_LubStrTm3.month = t->tm_mon;
        HMD_LubStrTm3.day = t->tm_mday;
        HMD_LubStrTm3.hour = t->tm_hour;
        HMD_LubStrTm3.minute = t->tm_min;
        HMD_LubStrTm3.second = t->tm_sec;
        SaveHMDLub(2*sizeof(DATETIME),&HMD_LubStrTm3,sizeof(DATETIME));

        HMD_LUBHOUR3 = 0;
        HMD_LeftLUBHOUR3 = VarAdrToInt(p_PP_MACHSET_wUse15DaysLub3)*60 - HMD_LUBHOUR3;
        break;
    default:
        break;
    }
}

UI32 GetLubHour(UI16 lubno)
{
    switch(lubno)
    {
    case 1:
        return HMD_LUBHOUR1;
    case 2:
        return HMD_LUBHOUR2;
    case 3:
        return HMD_LUBHOUR3;
    default:
        break;
    }

    return 0;
}

void SetLubHour(UI16 lubno, UI32 value)
{
    switch(lubno)
    {
    case 1:
        HMD_LUBHOUR1 = value;
        break;
    case 2:
        HMD_LUBHOUR2 = value;
        break;
    case 3:
        HMD_LUBHOUR3 = value;
        break;
    default:
        break;
    }
}

UI32 GetLeftLubHour(UI16 lubno)
{
    switch(lubno)
    {
    case 1:
        return HMD_LeftLUBHOUR1;
    case 2:
        return HMD_LeftLUBHOUR2;
    case 3:
        return HMD_LeftLUBHOUR3;
    default:
        break;
    }

    return 0;
}

void SetLeftLubHour(UI16 lubno, UI32 value)
{
    switch(lubno)
    {
    case 1:
        HMD_LeftLUBHOUR1 = value;
        break;
    case 2:
        HMD_LeftLUBHOUR2 = value;
        break;
    case 3:
        HMD_LeftLUBHOUR3 = value;
        break;
    default:
        break;
    }
}
