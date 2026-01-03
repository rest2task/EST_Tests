#include "cmdrec.h"
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
#include "ctrlcmd.h"

CMDRECORD m_cmdrecord;


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
static int CmdisFull(CMDRECORD* rec)
{
    if((rec->cmdhead.rear + 1) % MAX_CMD_SIZE == rec->cmdhead.front)
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
static int CmdisEmpty(CMDRECORD* rec)
{
    if(rec->cmdhead.rear == rec->cmdhead.front)
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
static int CmdDelete(CMDRECORD* rec)
{
    if(CmdisEmpty(rec))
        return FALSE;

    rec->cmdhead.front = (rec->cmdhead.front + 1) % MAX_CMD_SIZE;
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
static void	WriteCmdRec(int index,void* src)
{
    FILEHD filehd;
    filehd = FileOpen(CMD_REC_PATH);
    if(CheckFileValid(filehd))
    {
        FileSeek(filehd,0);
        FileWrite(filehd,&m_cmdrecord.cmdhead, sizeof(CMDHEAD),NULL);
        FileSeek(filehd, sizeof(CMDHEAD) + index * sizeof(CMDITEM));
        FileWrite(filehd, src, sizeof(CMDITEM), NULL);
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
static int CmdAdd(CMDRECORD* rec, CMDITEM item)
{
    int index;
    if(CmdisFull(rec))
        CmdDelete(rec);
    rec->item[rec->cmdhead.rear] = item;
    index = rec->cmdhead.rear;
    rec->cmdhead.rear = (rec->cmdhead.rear + 1) % MAX_CMD_SIZE;
    WriteCmdRec(index, &item);
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
static int CmdNum(CMDRECORD* rec)
{
    return (rec->cmdhead.rear - rec->cmdhead.front + MAX_CMD_SIZE) % MAX_CMD_SIZE;
}

/**
* @brief     :接受数据初始化
* @param     :包头 id号
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191101
*/
void CmdSendRecInit(PPROT_HD phd, UI8 prot_id)
{
    CMDITEM cmditem;
    PPROT_CTRL_CMD_PKT ppkt;
    int i, len;
    UI32 allcmd;
    char data[40] = "\0";
    char tmpdata[20] = "\0";

    TimeToStr(cmditem.time,"yyyy-MM-dd hh:mm:ss",(UI32)time(NULL));
    cmditem.state = VarAdrToInt(OPMODE_INDEX);
    cmditem.open_cnt = VarAdrToInt(PROD_TOTAL_SHOTCNT);

    if((phd->cmd & CMD_BITS) == SYS_CTRL_CMD)
    {
        ppkt = (PPROT_CTRL_CMD_PKT)(phd+1);
        allcmd = phd->cmd & CMD_BITS;
        allcmd = allcmd << 16;
        allcmd = allcmd | ppkt->ctrl_cmd;
        if(ppkt->ctrl_cmd == OPENRELVLV_CMD)//20201102 频繁写的指令过滤
        {
            return;
        }
        cmditem.cmd = allcmd;
        for(i = 0; i < ppkt->data_len; ++i)
        {
            len = strlen(data);
            snprintf(tmpdata, sizeof(tmpdata)-1, "%d:%d ", i, *((UI16*)(ppkt+1)+i));
            strncat(data, tmpdata, sizeof(data)-len-2);
        }
        strcpy(cmditem.data, data);
        CmdAdd(&m_cmdrecord, cmditem);
    }
    else if((phd->cmd & CMD_BITS) == UPGRADE_RESET_CMD)
    {
        ppkt = (PPROT_CTRL_CMD_PKT)(phd+1);
        allcmd = phd->cmd & CMD_BITS;
        allcmd = allcmd << 16;
        allcmd = allcmd | ppkt->ctrl_cmd;
        cmditem.data[0] = '\0';
        cmditem.cmd = allcmd;
        CmdAdd(&m_cmdrecord, cmditem);
    }
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
void CmdRecInit()
{
    FILEHD filehd;
    memset(&m_cmdrecord, 0, sizeof(CMDRECORD));
    m_cmdrecord.cmdhead.front = 0;
    m_cmdrecord.cmdhead.rear = 0;
    filehd = FileOpen(CMD_REC_PATH);
    if(CheckFileValid(filehd))
    {
        FileRead(filehd, &m_cmdrecord, sizeof(CMDRECORD), NULL);
        FileClose(filehd);
    }
    else {
        filehd = FileCreate(CMD_REC_PATH);
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
void ClearCmdRec()
{
    memset(&m_cmdrecord, 0, sizeof(CMDRECORD));
    FILEHD filehd;
    filehd = FileOpen(CMD_REC_PATH);
    if(CheckFileValid(filehd))
    {
        FileSeek(filehd,0);
        FileWrite(filehd, &m_cmdrecord, sizeof(CMDRECORD), NULL);
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
BOOL ReadCmdRec(CMDITEM* item,UI32 index)
{
    CMDITEM tmpitem;
    if(index < CmdNum(&m_cmdrecord) && !CmdisEmpty(&m_cmdrecord))
    {
        tmpitem = m_cmdrecord.item[(m_cmdrecord.cmdhead.rear - index - 1 + MAX_CMD_SIZE) % MAX_CMD_SIZE];
//        strcpy(item->cmd, tmpitem.cmd);
        item->cmd = tmpitem.cmd;
        strcpy(item->time, tmpitem.time);
        strcpy(item->data, tmpitem.data);
        item->open_cnt = tmpitem.open_cnt;
        item->state = tmpitem.state;
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
BOOL CmdRecIsChg()
{
    static int chg = -1;
    if(chg != m_cmdrecord.cmdhead.rear)
    {
        chg = m_cmdrecord.cmdhead.rear;
        return TRUE;
    }
    else{
        return FALSE;
    }
}
