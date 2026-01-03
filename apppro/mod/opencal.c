#include "opencal.h"
#include "file.h"
#include "tablectl.h"
#include "stdio.h"
#include "oprintf.h"

#define CMOD_WR_HEADSZ sizeof(MOLDSET_HEAD)
#define CMOD_WR_ITEMSZ sizeof(MOLDSET_ITEM)
#define CMOD_WR_PATH   SAVE_PATH"opentabcalx.dat"
#define CMOD_TEMP_PATH	SAVE_PATH"opentabcalxtemp.dat"

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

/**
 * @brief      : 保存AI记忆开模算法数据
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200727
 */
void  SaveOpenCalRec(void* src,int nCount)
{
    FILEHD filehd;

    if(FileExist(OPEN_CAL_PATH))
    {
        filehd = FileOpen(OPEN_CAL_PATH);
    }
    else
    {
        filehd = FileCreate(OPEN_CAL_PATH);
    }

    if(CheckFileValid(filehd))
    {
        FileSeek(filehd,0);
        FileWrite(filehd, src, nCount*MAX_OPENROWLENTH, NULL);
        FileClose(filehd);
    }
}

/**
 * @brief      : 加载AI记忆开模算法数据
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200727
 */
BOOL  LoadOpenCalRec(void* dst)
{
    FILEHD filehd;
    filehd = FileOpen(OPEN_CAL_PATH);
    if(CheckFileValid(filehd))
    {
        FileRead(filehd, dst, MAX_OPENTABROW*MAX_OPENROWLENTH, NULL);
        FileClose(filehd);
        return TRUE;
    }
    else
    {
        filehd = FileCreate(OPEN_CAL_PATH);
        FileClose(filehd);
        return FALSE;
    }
}

/*******AI记忆开模算法数据 可跟随模具参数导入导出*******/
static void OprintfTabData(UI16* data)
{
    UI16 wTabData[MAX_OPENTABROW * (MAX_OPENROWLENTH/2)]= {0};
    int m,n;

    memcpy(wTabData, data, sizeof(wTabData));

    for(m=0;m<MAX_OPENTABROW;m++)
    {
        oprintf("wTabData %d:",m);
        for(n=0;n<MAX_OPENROWLENTH/2;n++)
        {
            oprintf("%d,",wTabData[m*(MAX_OPENROWLENTH/2)+n]);
        }
        oprintf("\n");
    }
}

//保存AI 记忆开模数据
BOOL SaveOpenCalData_Mold(UI16 cMoldSet)
{
    UI16 wTabData[MAX_OPENTABROW * (MAX_OPENROWLENTH/2)]= {0};
    PPROT_TBL_HD ptb = GetTableByType(TB_OPENCAL, TB_CUR);
    UI32 dataPos;

    if(ptb == NULL)
    {
        if(!LoadOpenCalRec(wTabData))
        {
            SaveOpenCalRec(wTabData,MAX_OPENTABROW);
        }
    }
    else
    {
        memcpy(wTabData, (UI8*)(ptb+1), MAX_OPENTABROW * MAX_OPENROWLENTH);
    }

    dataPos = CMOD_WR_HEADSZ + MAX_MOLDSET*CMOD_WR_ITEMSZ + (UI16)cMoldSet*sizeof(wTabData);
    return CModWriteRaw(dataPos, wTabData, sizeof(wTabData), CMOD_WR_PATH);
}

//读入AI 记忆开模数据
BOOL ReadOpenCalData_Mold(UI16 cMoldSet)
{
    UI16 wTabData[MAX_OPENTABROW * (MAX_OPENROWLENTH/2)]= {0};
    PPROT_TBL_HD ptb = GetTableByType(TB_OPENCAL, TB_CUR);
    UI32 dataPos;

    dataPos = CMOD_WR_HEADSZ + MAX_MOLDSET*CMOD_WR_ITEMSZ + (UI16)cMoldSet*sizeof(wTabData);
    if(!CModReadRaw(wTabData, dataPos, sizeof(wTabData), CMOD_WR_PATH))
    {
        return FALSE;
    }

    if(ptb == NULL)
    {
        ptb = (PPROT_TBL_HD)OBJMemMalloc(MAX_OPENTABROW * MAX_OPENROWLENTH+sizeof(PROT_TBL_HD));
        ptb->row_len = MAX_OPENROWLENTH;
        ptb->row_num = MAX_OPENTABROW;
        ptb->type = TB_OPENCAL;
        ptb->sn = 1;
        memcpy((UI8*)(ptb+1), wTabData, MAX_OPENTABROW * MAX_OPENROWLENTH);
        AddOpenCalTable(ptb);
    }
    else
    {
        memcpy((UI8*)(ptb+1), wTabData, MAX_OPENTABROW * MAX_OPENROWLENTH);
    }
    SaveOpenCalRec(wTabData,MAX_OPENTABROW);
    ProtSendTbl(ptb);

    return TRUE;
}

static void ReadBKRAM_OpenCal(void* dst,UI16 cMoldSet)
{
    if(dst != NULL && cMoldSet<MAX_MOLDSET)
    {
        CMOD_RITEM_EX(cMoldSet,dst);
    }
}

static BOOL ReadOpenCal_MoldInfo(UI16 cMoldSet,MOLDSET_ITEM* info)
{
    info->flag=0;
    ReadBKRAM_OpenCal(info,cMoldSet-1);

    if (info->flag == MARK_USED)  return TRUE;
    else 	return FALSE;
}

void WriteBKRAM_OpenCal(UI32 dst,void* src,UI16 wCount)
{
    CModWriteRaw(dst, src, wCount, CMOD_WR_PATH);
}

void SaveOpenCal_MoldInfo(UI16 cMoldSet,MOLDSET_ITEM* info)
{
    UI32    dwAddr;
    dwAddr = CMOD_WR_HEADSZ+(UI16)cMoldSet*sizeof(MOLDSET_ITEM);

    info->flag = MARK_USED;
    WriteBKRAM_OpenCal(dwAddr,info,sizeof(MOLDSET_ITEM));
}

void DeleteOpenCal_Mold(UI16 cMoldSet)
{
    CMOD_DITEM(cMoldSet);
}

/***读写U盘相关函数***/

static void	ReadBKRAM_OpenCalTemp(void*item,UI32 index,UI16 size)
{
    UI32 dataPos =CMOD_WR_ITEMSZ*index;//临时文件不包含头信息
    CModReadRaw(item, dataPos, size, CMOD_TEMP_PATH);
}

//读取临时AI数据
BOOL ReadOpenCalInfoTemp(UI16 cMoldSet,MOLDSET_ITEM* info)
{
    ReadBKRAM_OpenCalTemp(info,cMoldSet-1,sizeof(MOLDSET_ITEM));

    if (info->flag== MARK_USED)  return TRUE;
    else 	return FALSE;
}

BOOL ReadOpenCalTempEx(UI16 cMoldSet,void* dst)
{
    UI16 wTabData[MAX_OPENTABROW * (MAX_OPENROWLENTH/2)]= {0};
    MOLDSET_ITEM  info;
    UI32 dataPos;

    if (ReadOpenCalInfoTemp(cMoldSet, &info))
    {
        dataPos = MAX_MOLDSET*CMOD_WR_ITEMSZ + (UI16)(cMoldSet-1)*sizeof(wTabData);
        CModReadRaw(dst, dataPos, sizeof(wTabData), CMOD_TEMP_PATH);
    }
    else
    {
        return FALSE;
    }

    return TRUE;

}

BOOL SaveOpenCalEx(UI16 cMoldSet,void* dst)
{
    UI16 wTabData[MAX_OPENTABROW * (MAX_OPENROWLENTH/2)]= {0};

    UI32 dataPos = CMOD_WR_HEADSZ + MAX_MOLDSET*CMOD_WR_ITEMSZ + (UI16)cMoldSet*sizeof(wTabData);

    WriteBKRAM_OpenCal(dataPos,dst,sizeof(wTabData));

    return TRUE;
}

//读U盘
BOOL OpenCalInput()
{
    MOLDSET_ITEM info;
    UI16 wTabData[MAX_OPENTABROW * (MAX_OPENROWLENTH/2)]= {0};
    int k=0;
    int i=1,j=0;
    UI16 nIdAddMoldset[MAX_MOLDSET+1]={0};
    for(i=1;i<=MAX_MOLDSET;i++){
        if(ReadOpenCalInfoTemp(i, &info)){
            nIdAddMoldset[k++]=i;
        }
    }

    i=1;
    j=0;
    while(j<k && i<=MAX_MOLDSET){
        if(!ReadOpenCal_MoldInfo(i, &info)){

            ReadOpenCalInfoTemp(nIdAddMoldset[j],&info);
            ReadOpenCalTempEx(nIdAddMoldset[j],wTabData);

            SaveOpenCal_MoldInfo(i-1,&info);
            SaveOpenCalEx(i-1,wTabData);
            j++;
        }

        i++;
    }
    osystem(CMD_DEL CMOD_TEMP_PATH);
    return true;
}

static void WriteBKRAM_OpenCalTemp(UI32 dst,void* src,UI16 wCount)
{
    CModWriteRaw(dst, src, wCount, CMOD_TEMP_PATH);
}

void DeleteOpenCalTemp(UI16 cMoldSet)
{
    UI32 dwAddr;
    MOLDSET_ITEM info;
    memset(&info,0,sizeof(MOLDSET_ITEM));

    dwAddr=(UI16)(cMoldSet-1)*sizeof(MOLDSET_ITEM);

    info.flag = 0;
    WriteBKRAM_OpenCalTemp(dwAddr,&info,sizeof(MOLDSET_ITEM));
}

//写U盘
BOOL OpenCalExportTo(const char* cmd,const char * filepath,UI16 maxsize)
{
    if(cmd != NULL && filepath!=NULL)
    {
        int i=0;
        int j=0;
        UI32 dwAddr;
        MOLDSET_ITEM info;
        UI16 wTabData[MAX_OPENTABROW * (MAX_OPENROWLENTH/2)]= {0};

        for( i=1;i<=MAX_MOLDSET;i++)
        {
            DeleteOpenCalTemp(i);
            ReadBKRAM_OpenCal(&info,i-1);
            if(g_moldset.sel_state[i-1]  && info.flag==MARK_USED)
            {
                dwAddr=(UI16)(j)*CMOD_WR_ITEMSZ;
                WriteBKRAM_OpenCalTemp(dwAddr,&info,CMOD_WR_ITEMSZ);
                dwAddr = CMOD_WR_HEADSZ + MAX_MOLDSET*CMOD_WR_ITEMSZ + (UI16)(i-1)*sizeof(wTabData);
                if(!CModReadRaw(wTabData, dwAddr, sizeof(wTabData), CMOD_WR_PATH))
                {
                    return FALSE;
                }

                dwAddr = MAX_MOLDSET*CMOD_WR_ITEMSZ + (UI16)j*sizeof(wTabData);
                WriteBKRAM_OpenCalTemp(dwAddr,wTabData,sizeof(wTabData));
                j++;
            }
        }

        snprintf((char*)cmd,maxsize-1,CMD_COPY" %s %s",CMOD_TEMP_PATH,filepath);

        return TRUE;
    }

    return FALSE;
}

//校验文件长度是否符合HMI数据
BOOL CheckOpenCalFile(const char* filename, BOOL includehead)
{
    FILEHD filehd;
    UI16 wTabData[MAX_OPENTABROW * (MAX_OPENROWLENTH/2)]= {0};
    int file_size = FileGetSize(filehd, filename);
    //oprintf("--CheckOpenCalpFile--%s--file_size = %d\n",filename,file_size);

    if(includehead)
    {
        file_size = file_size - sizeof(MOLDSET_HEAD);
    }

    if(file_size > MAX_MOLDSET*sizeof(MOLDSET_ITEM))
    {
        int data_num = (file_size-MAX_MOLDSET*sizeof(MOLDSET_ITEM))%sizeof(wTabData);

        if(data_num == 0)
        {
            return TRUE;
        }
    }

    return FALSE;
}
