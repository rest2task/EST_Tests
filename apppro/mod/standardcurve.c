#include "standardcurve.h"
#include "database.h"
#include "oprintf.h"
#include "lightgraphwgt.h"
#include "file.h"
#include "curvesctl.h"
#include "stdio.h"
#include "privdef.h"
#include "time.h"

#ifdef USE_QT
#define CURVE_NUM_MAX 4

BOOL SaveStandardCurve(UI32 datalength[],LDPoint * src1,LDPoint * src2,LDPoint * src3,LDPoint * src4,int curve_id)
{
    FILEHD filehd;
    char filename[255];

    if(curve_id == G_CLAMPOPEN)
    {
        snprintf(filename,sizeof(filename)-1,"%s",OPN_STANDARDCURVE_SAVEPATH);
    }
    else if(curve_id == G_CLAMPCLOSE)
    {
        snprintf(filename,sizeof(filename)-1,"%s",CLS_STANDARDCURVE_SAVEPATH);
    }
    else if(curve_id == G_CHARGE)
    {
        snprintf(filename,sizeof(filename)-1,"%s",CHG_STANDARDCURVE_SAVEPATH);
    }
    else if(curve_id == G_INJECT)
    {
        snprintf(filename,sizeof(filename)-1,"%s",INJ_STANDARDCURVE_SAVEPATH);
    }
    else if(curve_id == G_EJECT)
    {
        snprintf(filename,sizeof(filename)-1,"%s",EJE_STANDARDCURVE_SAVEPATH);
    }
    else
    {
        return FALSE;
    }

    if(FileExist(filename))
    {
        FileDelete(filename);
    }

    filehd = FileCreate(filename);
    if(FILEHD_IS_OK(filehd))
    {
        FileSeek(filehd,0);
        FileWrite(filehd,datalength, sizeof(UI32)*4,NULL);
        FileSeek(filehd, sizeof(UI32)*4);
        FileWrite(filehd, src1, sizeof(LDPoint)*datalength[0], NULL);
        FileSeek(filehd, sizeof(UI32)*4+sizeof(LDPoint)*datalength[0]);
        FileWrite(filehd, src2, sizeof(LDPoint)*datalength[1], NULL);
        FileSeek(filehd, sizeof(UI32)*4+sizeof(LDPoint)*(datalength[0]+datalength[1]));
        FileWrite(filehd, src3, sizeof(LDPoint)*datalength[2], NULL);
        FileSeek(filehd, sizeof(UI32)*4+sizeof(LDPoint)*(datalength[0]+datalength[1]+datalength[2]));
        FileWrite(filehd, src4, sizeof(LDPoint)*datalength[3], NULL);
        FileClose(filehd);
        return TRUE;
    }
    return FALSE;
}

void LoadStandardCurve(PWGT pwgt1,PWGT pwgt2,PWGT pwgt3,PWGT pwgt4,int curve_id)
{
    UI32 datalength[4] = {0};
    FILEHD filehd;
    char filename[255];

    if(curve_id == G_CLAMPOPEN)
    {
        snprintf(filename,sizeof(filename)-1,"%s",OPN_STANDARDCURVE_SAVEPATH);
    }
    else if(curve_id == G_CLAMPCLOSE)
    {
        snprintf(filename,sizeof(filename)-1,"%s",CLS_STANDARDCURVE_SAVEPATH);
    }
    else if(curve_id == G_CHARGE)
    {
        snprintf(filename,sizeof(filename)-1,"%s",CHG_STANDARDCURVE_SAVEPATH);
    }
    else if(curve_id == G_INJECT)
    {
        snprintf(filename,sizeof(filename)-1,"%s",INJ_STANDARDCURVE_SAVEPATH);
    }
    else if(curve_id == G_EJECT)
    {
        snprintf(filename,sizeof(filename)-1,"%s",EJE_STANDARDCURVE_SAVEPATH);
    }

    filehd = FileOpen(filename);
    if(FILEHD_IS_OK(filehd))
    {
        PLDPoint points1 = NULL,points2 = NULL,points3 = NULL,points4 = NULL;
        FileSeek(filehd,0);
        FileRead(filehd,datalength,sizeof(UI32)*4,NULL);
        points1 = (PLDPoint)OBJMemMalloc(datalength[0]*sizeof(LDPoint));
        points2 = (PLDPoint)OBJMemMalloc(datalength[1]*sizeof(LDPoint));
        points3 = (PLDPoint)OBJMemMalloc(datalength[2]*sizeof(LDPoint));
        points4 = (PLDPoint)OBJMemMalloc(datalength[3]*sizeof(LDPoint));
        FileSeek(filehd,sizeof(UI32)*4);
        FileRead(filehd,points1,datalength[0]*sizeof(LDPoint),NULL);
        FileSeek(filehd,sizeof(UI32)*4+datalength[0]*sizeof(LDPoint));
        FileRead(filehd,points2,datalength[1]*sizeof(LDPoint),NULL);
        FileSeek(filehd,sizeof(UI32)*4+(datalength[0]+datalength[1])*sizeof(LDPoint));
        FileRead(filehd,points3,datalength[2]*sizeof(LDPoint),NULL);
        FileSeek(filehd,sizeof(UI32)*4+(datalength[0]+datalength[1]+datalength[2])*sizeof(LDPoint));
        FileRead(filehd,points4,datalength[3]*sizeof(LDPoint),NULL);
        FileClose(filehd);
        LCurveClear(pwgt1);
        LCurveClear(pwgt2);
        LCurveClear(pwgt3);
        LCurveClear(pwgt4);
        LCurveAddPointsEx(pwgt1,points1,datalength[0]);
        LCurveAddPointsEx(pwgt2,points2,datalength[1]);
        LCurveAddPointsEx(pwgt3,points3,datalength[2]);
        LCurveAddPointsEx(pwgt4,points4,datalength[3]);
        OBJMemFree(points1);
        OBJMemFree(points2);
        OBJMemFree(points3);
        OBJMemFree(points4);
    }
}

BOOL SaveSpecStandardCurve(UI32 datalength[],LDPoint * src[],int curve_num,int curve_id)
{
    FILEHD filehd;
    char filename[255];

    if(curve_id == G_CLAMPOPEN)
    {
        snprintf(filename,sizeof(filename)-1,"%s",OPN_STANDARDCURVE_SAVEPATH);
    }
    else if(curve_id == G_CLAMPCLOSE)
    {
        snprintf(filename,sizeof(filename)-1,"%s",CLS_STANDARDCURVE_SAVEPATH);
    }
    else if(curve_id == G_CHARGE)
    {
        snprintf(filename,sizeof(filename)-1,"%s",CHG_STANDARDCURVE_SAVEPATH);
    }
    else if(curve_id == G_INJECT)
    {
        snprintf(filename,sizeof(filename)-1,"%s",INJ_STANDARDCURVE_SAVEPATH);
    }
    else if(curve_id == G_INJECT_HOLD_TM)
    {
        snprintf(filename,sizeof(filename)-1,"%s",INJ_HOLD_TM_STANDARDCURVE_SAVEPATH);
    }
    else if(curve_id == G_INJECT_INJ_POS)
    {
        snprintf(filename,sizeof(filename)-1,"%s",INJ_INJ_POS_STANDARDCURVE_SAVEPATH);
    }
    else if(curve_id == G_EJECT)
    {
        snprintf(filename,sizeof(filename)-1,"%s",EJE_STANDARDCURVE_SAVEPATH);
    }
    else
    {
        return FALSE;
    }

    if(FileExist(filename))
    {
        FileDelete(filename);
    }

    filehd = FileCreate(filename);
    if(FILEHD_IS_OK(filehd))
    {
        int i,j,length;
        FileSeek(filehd,0);
        FileWrite(filehd,datalength, sizeof(UI32)*curve_num,NULL);

        for(i = 0; i < curve_num; i++)
        {
            length = 0;
            for(j = 0; j < i; j++)
            {
                length = datalength[j] + length;
            }
            FileSeek(filehd, sizeof(UI32)*curve_num+sizeof(LDPoint)*length);
            FileWrite(filehd, src[i], sizeof(LDPoint)*datalength[i], NULL);
        }
        FileClose(filehd);
        return TRUE;
    }
    return FALSE;
}

void LoadSpecStandardCurve(PWGT pwgt[],int curve_num,int curve_id)
{
    FILEHD filehd;
    char filename[255];

    if(curve_num > CURVE_NUM_MAX)
    {
        return;
    }

    if(curve_id == G_CLAMPOPEN)
    {
        snprintf(filename,sizeof(filename)-1,"%s",OPN_STANDARDCURVE_SAVEPATH);
    }
    else if(curve_id == G_CLAMPCLOSE)
    {
        snprintf(filename,sizeof(filename)-1,"%s",CLS_STANDARDCURVE_SAVEPATH);
    }
    else if(curve_id == G_CHARGE)
    {
        snprintf(filename,sizeof(filename)-1,"%s",CHG_STANDARDCURVE_SAVEPATH);
    }
    else if(curve_id == G_INJECT)
    {
        snprintf(filename,sizeof(filename)-1,"%s",INJ_STANDARDCURVE_SAVEPATH);
    }
    else if(curve_id == G_INJECT_HOLD_TM)
    {
        snprintf(filename,sizeof(filename)-1,"%s",INJ_HOLD_TM_STANDARDCURVE_SAVEPATH);
    }
    else if(curve_id == G_INJECT_INJ_POS)
    {
        snprintf(filename,sizeof(filename)-1,"%s",INJ_INJ_POS_STANDARDCURVE_SAVEPATH);
    }
    else if(curve_id == G_EJECT)
    {
        snprintf(filename,sizeof(filename)-1,"%s",EJE_STANDARDCURVE_SAVEPATH);
    }

    filehd = FileOpen(filename);
    if(FILEHD_IS_OK(filehd))
    {
        int i,j,length;
        PLDPoint points[CURVE_NUM_MAX];
        UI32 datalength[CURVE_NUM_MAX];

        for(i = 0; i < CURVE_NUM_MAX; i++)
        {
            points[i] = NULL;
            datalength[i] = 0;
        }

        FileSeek(filehd,0);
        FileRead(filehd,datalength,sizeof(UI32)*curve_num,NULL);

        for(i = 0; i < curve_num; i++)
        {
            points[i] = (PLDPoint)OBJMemMalloc(datalength[i]*sizeof(LDPoint));

            length = 0;
            for(j = 0; j < i; j++)
            {
                length = datalength[j] + length;
            }

            FileSeek(filehd, sizeof(UI32)*curve_num+sizeof(LDPoint)*length);
            FileRead(filehd,points[i],datalength[i]*sizeof(LDPoint),NULL);
        }
        FileClose(filehd);

        for(i = 0; i < curve_num; i++)
        {
            LCurveClear(pwgt[i]);
            LCurveAddPointsEx(pwgt[i],points[i],datalength[i]);
            OBJMemFree(points[i]);
        }

    }
}

/******************************** 导出标准曲线 *************************************/

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

/**
 * @brief      : 导出标准射出曲线到U盘
 *
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200120
 */
BOOL ExportInjectStandardCurve()
{
    //20230518 chj 原先的fileprintf方式有问题，每次调用都会写文件,开放使用，请参照成品导出excel进行修改
    /*
    FILEHD stream;
    FILEHD filehd;
    char buf[255];
    char cbuf[20];
    char *p_str;
    UI8 bs[3] = {(UI8)0xEF, (UI8)0xBB, (UI8)0xBF};
    UI32 filetime;
    UI32 datalength[4] = {0};
    PLDPoint points1 = NULL,points2 = NULL,points3 = NULL,points4 = NULL;
    int i;
    UI32 length;

    filetime = (UI32)time(NULL);
    TimeToStr(cbuf,"yyyy-MM-dd_hh-mm-ss", filetime);

    sprintf(buf,"%sInjCurve-%s.csv",CURVEEXCEL_PATH,cbuf);

    p_str = ConvToGBK_Dup(buf);

    if(!FileExist(CURVEEXCEL_PATH))
    {
		if(!FileExist(ESTDATAEXCEL_PATH))
        {
            MkDir(ESTDATAEXCEL_PATH);
        }

        MkDir(CURVEEXCEL_PATH);
    }

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

    FilePrintf(stream, "%s, ", GetPageLangStr(PAGE_INJCURVE_1, "Time(s)"));
    FilePrintf(stream, "%s, ", GetPageLangStr(PAGE_INJCURVE_1, "SysPress(bar)"));
    FilePrintf(stream, "%s, ", GetPageLangStr(PAGE_INJCURVE_1, "Setting Prs(bar)"));
    FilePrintf(stream, "%s, ", GetPageLangStr(PAGE_INJCURVE_1, "Inject Spd(mm/s)"));
    FilePrintf(stream, "%s, ", GetPageLangStr(PAGE_INJCURVE_1, "Inject Pos(mm)"));
    FilePrintf(stream, "\n");

    //加载保存的数据
    filehd = FileOpen(INJ_STANDARDCURVE_SAVEPATH);

    if(!FILEHD_IS_OK(filehd))
    {
        FileClose(stream);
        return FALSE;
    }

    FileSeek(filehd,0);
    FileRead(filehd,datalength,sizeof(UI32)*4,NULL);
    points1 = (PLDPoint)OBJMemMalloc(datalength[0]*sizeof(LDPoint));
    points2 = (PLDPoint)OBJMemMalloc(datalength[1]*sizeof(LDPoint));
    points3 = (PLDPoint)OBJMemMalloc(datalength[2]*sizeof(LDPoint));
    points4 = (PLDPoint)OBJMemMalloc(datalength[3]*sizeof(LDPoint));
    FileSeek(filehd,sizeof(UI32)*4);
    FileRead(filehd,points1,datalength[0]*sizeof(LDPoint),NULL);
    FileSeek(filehd,sizeof(UI32)*4+datalength[0]*sizeof(LDPoint));
    FileRead(filehd,points2,datalength[1]*sizeof(LDPoint),NULL);
    FileSeek(filehd,sizeof(UI32)*4+(datalength[0]+datalength[1])*sizeof(LDPoint));
    FileRead(filehd,points3,datalength[2]*sizeof(LDPoint),NULL);
    FileSeek(filehd,sizeof(UI32)*4+(datalength[0]+datalength[1]+datalength[2])*sizeof(LDPoint));
    FileRead(filehd,points4,datalength[3]*sizeof(LDPoint),NULL);
    FileClose(filehd);

    //一般情况下datalength各值相等
    length = datalength[0];
    for(i = 0; i < 4; i++)
    {
        if(datalength[i] < length)
        {
            length = datalength[i];
        }
    }

    for(i = 0; i < length; i++)
    {
        if(VarAdrToInt(p_PP_OBJ_INJ_CURVE_TM_DOT) == 3)
        {
            FilePrintf(stream, "%.3f, ", (points1 + i)->key);
        }
        else
        {
            FilePrintf(stream, "%.2f, ", (points1 + i)->key);
        }

        FilePrintf(stream, "%.f, ", (points1 + i)->value);
        FilePrintf(stream, "%.f, ", (points2 + i)->value);
        FilePrintf(stream, "%.1f, ", (points3 + i)->value);
        FilePrintf(stream, "%.1f, ", (points4 + i)->value);
        FilePrintf(stream, "\n");
    }

    FileClose(stream);
    OBJMemFree(points1);
    OBJMemFree(points2);
    OBJMemFree(points3);
    OBJMemFree(points4);*/
    return TRUE;
}

/**
 * @brief      : 导出标准储料曲线到U盘
 *
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200218
 */
BOOL ExportChargeStandardCurve()
{
    //20230518 chj 原先的fileprintf方式有问题，每次调用都会写文件,开放使用，请参照成品导出excel进行修改
    /*
    FILEHD stream;
    FILEHD filehd;
    char buf[255];
    char cbuf[20];
    char *p_str;
    UI8 bs[3] = {(UI8)0xEF, (UI8)0xBB, (UI8)0xBF};
    UI32 filetime;
    UI32 datalength[4] = {0};
    PLDPoint points1 = NULL,points2 = NULL,points3 = NULL,points4 = NULL;
    int i;
    UI32 length;

    filetime = (UI32)time(NULL);
    TimeToStr(cbuf,"yyyy-MM-dd_hh-mm-ss", filetime);

    sprintf(buf,"%sChgCurve-%s.csv",CURVEEXCEL_PATH,cbuf);

    p_str = ConvToGBK_Dup(buf);

    if(!FileExist(CURVEEXCEL_PATH))
    {
		if(!FileExist(ESTDATAEXCEL_PATH))
        {
            MkDir(ESTDATAEXCEL_PATH);
        }

        MkDir(CURVEEXCEL_PATH);
    }

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

    FilePrintf(stream, "%s, ", GetPageLangStr(PAGE_CHARGE_CURVE, "Charge Pos(mm)"));
    FilePrintf(stream, "%s, ", GetPageLangStr(PAGE_CHARGE_CURVE, "SysPress(bar)"));
    FilePrintf(stream, "%s, ", GetPageLangStr(PAGE_CHARGE_CURVE, "Setting Prs(bar)"));
    FilePrintf(stream, "%s, ", GetPageLangStr(PAGE_CHARGE_CURVE, "Motor Spd(RPM)"));
    FilePrintf(stream, "%s, ", GetPageLangStr(PAGE_CHARGE_CURVE, "Screw RPM(RPM)"));
    FilePrintf(stream, "\n");

    //加载保存的数据
    filehd = FileOpen(CHG_STANDARDCURVE_SAVEPATH);

    if(!FILEHD_IS_OK(filehd))
    {
        FileClose(stream);
        return FALSE;
    }

    FileSeek(filehd,0);
    FileRead(filehd,datalength,sizeof(UI32)*4,NULL);
    points1 = (PLDPoint)OBJMemMalloc(datalength[0]*sizeof(LDPoint));
    points2 = (PLDPoint)OBJMemMalloc(datalength[1]*sizeof(LDPoint));
    points3 = (PLDPoint)OBJMemMalloc(datalength[2]*sizeof(LDPoint));
    points4 = (PLDPoint)OBJMemMalloc(datalength[3]*sizeof(LDPoint));
    FileSeek(filehd,sizeof(UI32)*4);
    FileRead(filehd,points1,datalength[0]*sizeof(LDPoint),NULL);
    FileSeek(filehd,sizeof(UI32)*4+datalength[0]*sizeof(LDPoint));
    FileRead(filehd,points2,datalength[1]*sizeof(LDPoint),NULL);
    FileSeek(filehd,sizeof(UI32)*4+(datalength[0]+datalength[1])*sizeof(LDPoint));
    FileRead(filehd,points3,datalength[2]*sizeof(LDPoint),NULL);
    FileSeek(filehd,sizeof(UI32)*4+(datalength[0]+datalength[1]+datalength[2])*sizeof(LDPoint));
    FileRead(filehd,points4,datalength[3]*sizeof(LDPoint),NULL);
    FileClose(filehd);

    //一般情况下datalength各值相等
    length = datalength[0];
    for(i = 0; i < 4; i++)
    {
        if(datalength[i] < length)
        {
            length = datalength[i];
        }
    }

    for(i = 0; i < length; i++)
    {
        FilePrintf(stream, "%.1f, ", (points1 + i)->key);
        FilePrintf(stream, "%.f, ", (points1 + i)->value);
        FilePrintf(stream, "%.f, ", (points2 + i)->value);
        FilePrintf(stream, "%.f, ", (points3 + i)->value);
        FilePrintf(stream, "%.1f, ", (points4 + i)->value);
        FilePrintf(stream, "\n");
    }

    FileClose(stream);
    OBJMemFree(points1);
    OBJMemFree(points2);
    OBJMemFree(points3);
    OBJMemFree(points4);*/
    return TRUE;
}

/**
 * @brief      : 导出标准开模曲线到U盘
 *
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200218
 */
BOOL ExportOpenStandardCurve()
{
    //20230518 chj 原先的fileprintf方式有问题，每次调用都会写文件,开放使用，请参照成品导出excel进行修改
    /*
    FILEHD stream;
    FILEHD filehd;
    char buf[255];
    char cbuf[20];
    char *p_str;
    UI8 bs[3] = {(UI8)0xEF, (UI8)0xBB, (UI8)0xBF};
    UI32 filetime;
    UI32 datalength[2] = {0};
    PLDPoint points1 = NULL,points2 = NULL;
    int i;
    UI32 length;

    filetime = (UI32)time(NULL);
    TimeToStr(cbuf,"yyyy-MM-dd_hh-mm-ss", filetime);

    sprintf(buf,"%sOpenCurve-%s.csv",CURVEEXCEL_PATH,cbuf);

    p_str = ConvToGBK_Dup(buf);

    if(!FileExist(CURVEEXCEL_PATH))
    {
        if(!FileExist(ESTDATAEXCEL_PATH))
        {
            MkDir(ESTDATAEXCEL_PATH);
        }

        MkDir(CURVEEXCEL_PATH);
    }

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

    FilePrintf(stream, "%s, ", GetPageLangStr(PAGE_CLAMP_CURVE, "Open Pos(mm)"));
    FilePrintf(stream, "%s, ", GetPageLangStr(PAGE_CLAMP_CURVE, "SysPress(bar)"));
    FilePrintf(stream, "%s, ", GetPageLangStr(PAGE_CLAMP_CURVE, "Open Spd(mm/s)"));
    FilePrintf(stream, "\n");

    //加载保存的数据
    filehd = FileOpen(OPN_STANDARDCURVE_SAVEPATH);

    if(!FILEHD_IS_OK(filehd))
    {
        FileClose(stream);
        return FALSE;
    }

    FileSeek(filehd,0);
    FileRead(filehd,datalength,sizeof(UI32)*2,NULL);
    points1 = (PLDPoint)OBJMemMalloc(datalength[0]*sizeof(LDPoint));
    points2 = (PLDPoint)OBJMemMalloc(datalength[1]*sizeof(LDPoint));
    FileSeek(filehd,sizeof(UI32)*2);
    FileRead(filehd,points1,datalength[0]*sizeof(LDPoint),NULL);
    FileSeek(filehd,sizeof(UI32)*2+datalength[0]*sizeof(LDPoint));
    FileRead(filehd,points2,datalength[1]*sizeof(LDPoint),NULL);
    FileClose(filehd);

    //一般情况下datalength各值相等
    length = datalength[0];
    for(i = 0; i < 2; i++)
    {
        if(datalength[i] < length)
        {
            length = datalength[i];
        }
    }

    for(i = 0; i < length; i++)
    {
        FilePrintf(stream, "%.1f, ", (points1 + i)->key);
        FilePrintf(stream, "%.f, ", (points1 + i)->value);
        FilePrintf(stream, "%.1f, ", (points2 + i)->value);
        FilePrintf(stream, "\n");
    }

    FileClose(stream);
    OBJMemFree(points1);
    OBJMemFree(points2);*/
    return TRUE;
}

/**
 * @brief      : 导出标准关模曲线到U盘
 *
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200218
 */
BOOL ExportCloseStandardCurve()
{
    //20230518 chj 原先的fileprintf方式有问题，每次调用都会写文件,开放使用，请参照成品导出excel进行修改
    /*
    FILEHD stream;
    FILEHD filehd;
    char buf[255];
    char cbuf[20];
    char *p_str;
    UI8 bs[3] = {(UI8)0xEF, (UI8)0xBB, (UI8)0xBF};
    UI32 filetime;
    UI32 datalength[2] = {0};
    PLDPoint points1 = NULL,points2 = NULL;
    int i;
    UI32 length;

    filetime = (UI32)time(NULL);
    TimeToStr(cbuf,"yyyy-MM-dd_hh-mm-ss", filetime);

    sprintf(buf,"%sCloseCurve-%s.csv",CURVEEXCEL_PATH,cbuf);

    p_str = ConvToGBK_Dup(buf);

    if(!FileExist(CURVEEXCEL_PATH))
    {
        if(!FileExist(ESTDATAEXCEL_PATH))
        {
            MkDir(ESTDATAEXCEL_PATH);
        }

        MkDir(CURVEEXCEL_PATH);
    }

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

    FilePrintf(stream, "%s, ", GetPageLangStr(PAGE_CLOSE_CURVE, "Close Pos(mm)"));
    FilePrintf(stream, "%s, ", GetPageLangStr(PAGE_CLOSE_CURVE, "SysPress(bar)"));
    FilePrintf(stream, "%s, ", GetPageLangStr(PAGE_CLOSE_CURVE, "Close Spd(mm/s)"));
    FilePrintf(stream, "\n");

    //加载保存的数据
    filehd = FileOpen(CLS_STANDARDCURVE_SAVEPATH);

    if(!FILEHD_IS_OK(filehd))
    {
        FileClose(stream);
        return FALSE;
    }

    FileSeek(filehd,0);
    FileRead(filehd,datalength,sizeof(UI32)*2,NULL);
    points1 = (PLDPoint)OBJMemMalloc(datalength[0]*sizeof(LDPoint));
    points2 = (PLDPoint)OBJMemMalloc(datalength[1]*sizeof(LDPoint));
    FileSeek(filehd,sizeof(UI32)*2);
    FileRead(filehd,points1,datalength[0]*sizeof(LDPoint),NULL);
    FileSeek(filehd,sizeof(UI32)*2+datalength[0]*sizeof(LDPoint));
    FileRead(filehd,points2,datalength[1]*sizeof(LDPoint),NULL);
    FileClose(filehd);

    //一般情况下datalength各值相等
    length = datalength[0];
    for(i = 0; i < 2; i++)
    {
        if(datalength[i] < length)
        {
            length = datalength[i];
        }
    }

    for(i = 0; i < length; i++)
    {
        FilePrintf(stream, "%.1f, ", (points1 + i)->key);
        FilePrintf(stream, "%.f, ", (points1 + i)->value);
        FilePrintf(stream, "%.1f, ", (points2 + i)->value);
        FilePrintf(stream, "\n");
    }

    FileClose(stream);
    OBJMemFree(points1);
    OBJMemFree(points2);*/
    return TRUE;
}

/**
 * @brief      : 导出标准射出位置曲线到U盘
 *
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200218
 */
BOOL ExportInjectPosStandardCurve()
{
    //20230518 chj 原先的fileprintf方式有问题，每次调用都会写文件,开放使用，请参照成品导出excel进行修改
    /*
    FILEHD stream;
    FILEHD filehd;
    char buf[255];
    char cbuf[20];
    char *p_str;
    UI8 bs[3] = {(UI8)0xEF, (UI8)0xBB, (UI8)0xBF};
    UI32 filetime;
    UI32 datalength[3] = {0};
    PLDPoint points1 = NULL,points2 = NULL,points3 = NULL;
    int i;
    UI32 length;

    filetime = (UI32)time(NULL);
    TimeToStr(cbuf,"yyyy-MM-dd_hh-mm-ss", filetime);

    sprintf(buf,"%sInjPosCurve-%s.csv",CURVEEXCEL_PATH,cbuf);

    p_str = ConvToGBK_Dup(buf);

    if(!FileExist(CURVEEXCEL_PATH))
    {
        if(!FileExist(ESTDATAEXCEL_PATH))
        {
            MkDir(ESTDATAEXCEL_PATH);
        }

        MkDir(CURVEEXCEL_PATH);
    }

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

    FilePrintf(stream, "%s, ", GetPageLangStr(PAGE_INJCURVE, "Inject Pos(mm)"));
    FilePrintf(stream, "%s, ", GetPageLangStr(PAGE_INJCURVE, "SysPress(bar)"));
    FilePrintf(stream, "%s, ", GetPageLangStr(PAGE_INJCURVE, "Setting Prs(bar)"));
    FilePrintf(stream, "%s, ", GetPageLangStr(PAGE_INJCURVE, "Inject Spd(mm/s)"));
    FilePrintf(stream, "\n");

    //加载保存的数据
    filehd = FileOpen(INJ_INJ_POS_STANDARDCURVE_SAVEPATH);

    if(!FILEHD_IS_OK(filehd))
    {
        FileClose(stream);
        return FALSE;
    }

    FileSeek(filehd,0);
    FileRead(filehd,datalength,sizeof(UI32)*3,NULL);
    points1 = (PLDPoint)OBJMemMalloc(datalength[0]*sizeof(LDPoint));
    points2 = (PLDPoint)OBJMemMalloc(datalength[1]*sizeof(LDPoint));
    points3 = (PLDPoint)OBJMemMalloc(datalength[2]*sizeof(LDPoint));
    FileSeek(filehd,sizeof(UI32)*3);
    FileRead(filehd,points1,datalength[0]*sizeof(LDPoint),NULL);
    FileSeek(filehd,sizeof(UI32)*3+datalength[0]*sizeof(LDPoint));
    FileRead(filehd,points2,datalength[1]*sizeof(LDPoint),NULL);
    FileSeek(filehd,sizeof(UI32)*3+(datalength[0]+datalength[1])*sizeof(LDPoint));
    FileRead(filehd,points3,datalength[2]*sizeof(LDPoint),NULL);
    FileClose(filehd);

    //一般情况下datalength各值相等
    length = datalength[0];
    for(i = 0; i < 3; i++)
    {
        if(datalength[i] < length)
        {
            length = datalength[i];
        }
    }

    for(i = 0; i < length; i++)
    {
        FilePrintf(stream, "%.1f, ", (points1 + i)->key);
        FilePrintf(stream, "%.f, ", (points1 + i)->value);
        FilePrintf(stream, "%.f, ", (points2 + i)->value);
        FilePrintf(stream, "%.1f, ", (points3 + i)->value);
        FilePrintf(stream, "\n");
    }

    FileClose(stream);
    OBJMemFree(points1);
    OBJMemFree(points2);
    OBJMemFree(points3);*/
    return TRUE;
}

/**
 * @brief      : 导出标准保压曲线到U盘
 *
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200218
 */
BOOL ExportHoldStandardCurve()
{
    //20230518 chj 原先的fileprintf方式有问题，每次调用都会写文件,开放使用，请参照成品导出excel进行修改
    /*
    FILEHD stream;
    FILEHD filehd;
    char buf[255];
    char cbuf[20];
    char *p_str;
    UI8 bs[3] = {(UI8)0xEF, (UI8)0xBB, (UI8)0xBF};
    UI32 filetime;
    UI32 datalength[3] = {0};
    PLDPoint points1 = NULL,points2 = NULL,points3 = NULL;
    int i;
    UI32 length;

    filetime = (UI32)time(NULL);
    TimeToStr(cbuf,"yyyy-MM-dd_hh-mm-ss", filetime);

    sprintf(buf,"%sHoldCurve-%s.csv",CURVEEXCEL_PATH,cbuf);

    p_str = ConvToGBK_Dup(buf);

    if(!FileExist(CURVEEXCEL_PATH))
    {
        if(!FileExist(ESTDATAEXCEL_PATH))
        {
            MkDir(ESTDATAEXCEL_PATH);
        }

        MkDir(CURVEEXCEL_PATH);
    }

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

    FilePrintf(stream, "%s, ", GetPageLangStr(PAGE_HOLDCURVE, "Hold Time(s)"));
    FilePrintf(stream, "%s, ", GetPageLangStr(PAGE_HOLDCURVE, "SysPress(bar)"));
    FilePrintf(stream, "%s, ", GetPageLangStr(PAGE_HOLDCURVE, "Setting Prs(bar)"));
    FilePrintf(stream, "%s, ", GetPageLangStr(PAGE_HOLDCURVE, "Hold Spd(mm/s)"));
    FilePrintf(stream, "\n");

    //加载保存的数据
    filehd = FileOpen(INJ_HOLD_TM_STANDARDCURVE_SAVEPATH);

    if(!FILEHD_IS_OK(filehd))
    {
        FileClose(stream);
        return FALSE;
    }

    FileSeek(filehd,0);
    FileRead(filehd,datalength,sizeof(UI32)*3,NULL);
    points1 = (PLDPoint)OBJMemMalloc(datalength[0]*sizeof(LDPoint));
    points2 = (PLDPoint)OBJMemMalloc(datalength[1]*sizeof(LDPoint));
    points3 = (PLDPoint)OBJMemMalloc(datalength[2]*sizeof(LDPoint));
    FileSeek(filehd,sizeof(UI32)*3);
    FileRead(filehd,points1,datalength[0]*sizeof(LDPoint),NULL);
    FileSeek(filehd,sizeof(UI32)*3+datalength[0]*sizeof(LDPoint));
    FileRead(filehd,points2,datalength[1]*sizeof(LDPoint),NULL);
    FileSeek(filehd,sizeof(UI32)*3+(datalength[0]+datalength[1])*sizeof(LDPoint));
    FileRead(filehd,points3,datalength[2]*sizeof(LDPoint),NULL);
    FileClose(filehd);

    //一般情况下datalength各值相等
    length = datalength[0];
    for(i = 0; i < 3; i++)
    {
        if(datalength[i] < length)
        {
            length = datalength[i];
        }
    }

    for(i = 0; i < length; i++)
    {
        FilePrintf(stream, "%.1f, ", (points1 + i)->key);
        FilePrintf(stream, "%.f, ", (points1 + i)->value);
        FilePrintf(stream, "%.f, ", (points2 + i)->value);
        FilePrintf(stream, "%.1f, ", (points3 + i)->value);
        FilePrintf(stream, "\n");
    }

    FileClose(stream);
    OBJMemFree(points1);
    OBJMemFree(points2);
    OBJMemFree(points3);*/
    return TRUE;
}

/**
 * @brief      : 将曲线游标显示值转换成整型
 *
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200309
 */
int StrAdrToInt(UI32 adr)
{
    char buff[20];
    int i;
    snprintf(buff,sizeof(buff)-1,"%s",VarAdrToStr(adr));

    if(buff[0] == '-') //转换数为负数
    {
        for(i = 0; i < strlen(buff); i++)
        {
            buff[i] = buff[i+1];
        }
        return (-(StrToWord(buff, 0)));
    }
    else
    {
        return (StrToWord(buff, 0));
    }
}

#endif
