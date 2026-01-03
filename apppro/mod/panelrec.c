#include "panelrec.h"
#include "time.h"
#include "stdio.h"
#include "file.h"
#include "obj.h"
#include "common.h"
#include "panel.h"
#include "verinfo.h"
#include "protcmd.h"
#include "database.h"
#include "prodmanage.h"

PANELRECORD m_panelrecord;


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
* @brief     :检查记录队列是否满
* @param     :队列
* @return    :1.满 0.未满
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
static int PanelisFull(PANELRECORD* rec)
{
    if((rec->panelhead.rear + 1) % MAX_PANEL_SIZE == rec->panelhead.front)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
* @brief     :检查队列是否为空
* @param     :队列
* @return    :1.空 0.未空
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
static int PanelisEmpty(PANELRECORD* rec)
{
    if(rec->panelhead.rear == rec->panelhead.front)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
* @brief     :删除第一个队列元素
* @param     :队列
* @return    :队列为空的情况下返回false，队列删除成功返回true
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
static int PanelDelete(PANELRECORD* rec)
{
    if(PanelisEmpty(rec))
        return FALSE;

    rec->panelhead.front = (rec->panelhead.front + 1) % MAX_PANEL_SIZE;
    return TRUE;
}

/**
* @brief     :将index元素写入文件
* @param     :元素index，元素数据
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
static void	WritePanelRec(int index,void* src)
{
    FILEHD filehd;
    filehd = FileOpen(PANEL_REC_PATH);
    if(CheckFileValid(filehd))
    {
        FileSeek(filehd,0);
        FileWrite(filehd,&m_panelrecord.panelhead, sizeof(PANELHEAD),NULL);
        FileSeek(filehd, sizeof(PANELHEAD) + index * sizeof(PANELITEM));
        FileWrite(filehd, src, sizeof(PANELITEM), NULL);
        FileClose(filehd);
    }
}

/**
* @brief     :队列尾部增加元素，并写入文件
* @param     :队列，元素
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
static int PanelAdd(PANELRECORD* rec, PANELITEM item)
{
    int index;
    if(PanelisFull(rec))
        PanelDelete(rec);
    rec->item[rec->panelhead.rear] = item;
    index = rec->panelhead.rear;
    rec->panelhead.rear = (rec->panelhead.rear + 1) % MAX_PANEL_SIZE;
    WritePanelRec(index, &item);
    return TRUE;
}

/**
* @brief     :当前队列元素数量
* @param     :
* @return    :元素数量
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
static int PanelNum(PANELRECORD* rec)
{
    return (rec->panelhead.rear - rec->panelhead.front + MAX_PANEL_SIZE) % MAX_PANEL_SIZE;
}

/**
* @brief     :增加画面操作
* @param     :画面进行的操作
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191101
*/
void PanelRecAdd(PWGT pobj, PANELRECTYPE data1, PANELRECTYPE data2)
{
    PANELITEM panelitem;
    PPAGE_FRAM ppage;

    ppage = GetPageByWgt((POBJ)pobj);

    if(ppage!=NULL)
    {
        strncpy(panelitem.idTitle, PageFramGetName(ppage), sizeof(panelitem.idTitle)-1);
    }
    TimeToStr(panelitem.time,"yyyy-MM-dd hh:mm:ss",(UI32)time(NULL));

    panelitem.data1 = data1;
    panelitem.data2 = data2;

    //20230518 chj 出厂更新的时候，删除文件，需要重新创建下
    if(FileExist(PANEL_REC_PATH))
    {
        ;//oprintf("file exist---------\n");
    }
    else {
        //oprintf("file not exist---------\n");
        PanelRecInit();
    }

    PanelAdd(&m_panelrecord, panelitem);
}
/**
* @brief     :数据初始化，文件初始化
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191101
*/
void PanelRecInit()
{
    FILEHD filehd;
    memset(&m_panelrecord, 0, sizeof(PANELRECORD));
    m_panelrecord.panelhead.front = 0;
    m_panelrecord.panelhead.rear = 0;
    filehd = FileOpen(PANEL_REC_PATH);
    if(CheckFileValid(filehd))
    {
        FileRead(filehd, &m_panelrecord, sizeof(PANELRECORD), NULL);
        FileClose(filehd);
    }
    else {
        filehd = FileCreate(PANEL_REC_PATH);
        FileClose(filehd);
    }
}

/**
* @brief     :清除队列,内存中数据擦除,文件中数据擦除
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
void ClearPanelRec()
{
    memset(&m_panelrecord, 0, sizeof(PANELRECORD));
    FILEHD filehd;
    filehd = FileOpen(PANEL_REC_PATH);
    if(CheckFileValid(filehd))
    {
        FileSeek(filehd,0);
        FileWrite(filehd, &m_panelrecord, sizeof(PANELRECORD), NULL);
        FileClose(filehd);
    }

}


/**
* @brief     :读数据
* @param     :数据，索引值
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191101
*/
BOOL ReadPanelRec(PANELITEM* item,UI32 index)
{
    PANELITEM tmpitem;
    PPAGE_FRAM ppage;
    if(index < PanelNum(&m_panelrecord) && !PanelisEmpty(&m_panelrecord))
    {
        tmpitem = m_panelrecord.item[(m_panelrecord.panelhead.rear - index - 1 + MAX_PANEL_SIZE) % MAX_PANEL_SIZE];

        ppage = PanelFindPageWithLoad((const char*)(tmpitem.idTitle));
        if(ppage!=NULL)
        {
            strcpy(item->idTitle, PageFramTitle(ppage));
        }
        strcpy(item->time, tmpitem.time);
        item->data1 = tmpitem.data1;
        item->data2 = tmpitem.data2;
        return TRUE;
    }
    return FALSE;
}

/**
* @brief     :数据是否有改变
* @param     :
* @return    :true 改变 false 没有
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191101
*/
BOOL PanelRecIsChg()
{
    static int chg = -1;
    if(chg != m_panelrecord.panelhead.rear)
    {
        chg = m_panelrecord.panelhead.rear;
        return TRUE;
    }
    else{
        return FALSE;
    }
}
