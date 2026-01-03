#include "keyrec.h"
#include "time.h"
#include "stdio.h"
#include "file.h"
#include "obj.h"
#include "pagefram.h"
#include "common.h"
#include "panel.h"
#include "editwgt.h"
#include "dropwgt.h"
#include "verinfo.h"
#include "keymod.h"
#include "texttbl.h"
#include "privdef.h"
#include "database.h"
#include "prodmanage.h"
#include "keydef.h"

KEYRECORD m_keyrecord;
extern KEYINFO m_keyinfo;


PHASH_TBL lan_keymap = NULL;
static FILEHD g_key_filehd;

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

static int KeyisFull(KEYRECORD* rec)
{
    if((rec->keyhead.rear + 1) % MAX_KEY_SIZE == rec->keyhead.front)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static int KeyisEmpty(KEYRECORD* rec)
{
    if(rec->keyhead.rear == rec->keyhead.front)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static int KeyDelete(KEYRECORD* rec)
{
    if(KeyisEmpty(rec))
        return FALSE;

    rec->keyhead.front = (rec->keyhead.front + 1) % MAX_KEY_SIZE;
    return TRUE;
}

static void	WriteKeyRec(int index,void* src)
{
//    FILEHD filehd;
//    filehd = FileOpen(KEY_REC_PATH);
    if(CheckFileValid(g_key_filehd))
    {
        FileSeek(g_key_filehd,0);
        FileWrite(g_key_filehd,&m_keyrecord.keyhead, sizeof(KEYHEAD),NULL);
        FileSeek(g_key_filehd, sizeof(KEYHEAD) + index * sizeof(KEYITEM));
        FileWrite(g_key_filehd, src, sizeof(KEYITEM), NULL);
//        FileClose(filehd);
    }
}

static int KeyAdd(KEYRECORD* rec, KEYITEM item)
{
    int index;
    if(KeyisFull(rec))
        KeyDelete(rec);
    rec->item[rec->keyhead.rear] = item;
    index = rec->keyhead.rear;
    rec->keyhead.rear = (rec->keyhead.rear + 1) % MAX_KEY_SIZE;
    WriteKeyRec(index, &item);
    return TRUE;
}

static int KeyNum(KEYRECORD* rec)
{
    return (rec->keyhead.rear - rec->keyhead.front + MAX_KEY_SIZE) % MAX_KEY_SIZE;
}

/**
* @brief     :接受键值
* @param     :物理键，发送值，按压
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191101
*/
BOOL SetKeyRec(UI32 phykey, UI32 sendkey, UI16 press)
{
    KEYITEM keyitem;
    static int tmpprs = -1;

    if(tmpprs != press)
    {
        tmpprs = press;
        keyitem.sendvalue =  sendkey;
        TimeToStr(keyitem.time,"yyyy-MM-dd hh:mm:ss",(UI32)time(NULL));
        if(press == 1)
        {
            keyitem.state = TEXT_DOWN;
        }
        else
        {
            keyitem.state = TEXT_UP;
        }

        keyitem.phykey = phykey;
        if(phykey == _KEY_PANEL_MANUAL)
        {
            tmpprs = -1;//手动键只记录按下
        }
        //oprintf("1:%x, 2:%d, 3:%s, 4:%s\n", keyitem.phykey, keyitem.sendvalue, keyitem.state, keyitem.time);
        keyitem.op_state = VarAdrToInt(OPMODE_INDEX);
        keyitem.open_cnt = VarAdrToInt(PROD_TOTAL_SHOTCNT);

        KeyAdd(&m_keyrecord, keyitem);
    }
    return FALSE;
}


//void KeyRecTest(int t, void* b)
//{
//    static int press = 1;
//    SetKeyRec(0x4036, 1234, press);
//    press = ~press;
//}
//#include "timecheck.h"
void KeyRecInit()
{
//    int id;
//    FILEHD filehd;
    memset(&m_keyrecord, 0, sizeof(KEYRECORD));
    m_keyrecord.keyhead.front = 0;
    m_keyrecord.keyhead.rear = 0;
    g_key_filehd = FileOpen(KEY_REC_PATH);
    ParseCSVFile(LAN_KEYMAP,&lan_keymap);
    if(CheckFileValid(g_key_filehd))
    {
        FileRead(g_key_filehd, &m_keyrecord, sizeof(KEYRECORD), NULL);
//        FileClose(filehd);
    }
    else {
        g_key_filehd = FileCreate(KEY_REC_PATH);
//        FileClose(filehd);
    }
//    id = CreateTimer(1000, 0, KeyRecTest, NULL);
//    StartTimer(id);
}

void ClearKeyRec()
{
    memset(&m_keyrecord, 0, sizeof(KEYRECORD));
//    FILEHD filehd;
//    filehd = FileOpen(KEY_REC_PATH);
    if(CheckFileValid(g_key_filehd))
    {
        FileSeek(g_key_filehd,0);
        FileWrite(g_key_filehd, &m_keyrecord, sizeof(KEYRECORD), NULL);
//        FileClose(filehd);
    }

}


BOOL ReadKeyRec(KEYITEM* item,UI32 index)
{
    KEYITEM tmpitem;
    if(index < KeyNum(&m_keyrecord) && !KeyisEmpty(&m_keyrecord))
    {
        tmpitem = m_keyrecord.item[(m_keyrecord.keyhead.rear - index - 1 + MAX_KEY_SIZE) % MAX_KEY_SIZE];
        item->phykey = tmpitem.phykey;
        item->sendvalue = tmpitem.sendvalue;
        item->state = tmpitem.state;
        strcpy(item->time, tmpitem.time);
        item->open_cnt = tmpitem.open_cnt;
        item->op_state = tmpitem.op_state;
        //oprintf("1:%x, 2:%d, 3:%s, 4:%s\n", item->phykey, item->sendvalue, item->state, item->time);
        return TRUE;
    }
    return FALSE;
}
/**
* @brief     :键值接受是否有改变
* @param     :
* @return    :true 有改变 false 无改变
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191101
*/
BOOL KeyRecIsChg()
{
    static int chg = -1;
    if(chg != m_keyrecord.keyhead.rear)
    {
        chg = m_keyrecord.keyhead.rear;
        return TRUE;
    }
    else{
        return FALSE;
    }
}



