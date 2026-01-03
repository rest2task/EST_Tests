#include "WOpen_Cal.h"
#include "typedef.h"
#include "pagefram.h"
#include "protcmd.h"
#include "tablectl.h"
#include "opencal.h"
#include "stdio.h"
#include "questionDlg.h"
#include "privdef.h"
#include "oprintf.h"

DEFINE_OPEN_CAL_MAP

CONNECT_OPEN_CAL_EVENT

#define BUTTON_ENABLE  TMPVAL(0) //按钮可用
#define TMP_FIRST_DATA TMPVAL(1) //第一个数据关联的临时变量
#define TMP_PASSWORD   TMPVAL(400) //密码
#define EDIT_SHOW      TMPVAL(401) //编辑框可见

static QUESTION g_question;
//数据保存
static void SaveData()
{
    PPROT_TBL_HD ptb = GetTableByType(TB_OPENCAL, TB_CUR);
    UI16 wTabData[MAX_OPENTABROW * (MAX_OPENROWLENTH/2)]= {0};
    int i;

    if(ptb == NULL)
    {
        return;
    }

    for(i = 0; i < MAX_OPENTABROW * (MAX_OPENROWLENTH/2); i++)
    {
        wTabData[i] = VarAdrToInt(TMP_FIRST_DATA+i);
    }

    memcpy((UI8*)(ptb+1), wTabData, MAX_OPENTABROW * MAX_OPENROWLENTH);

    SaveOpenCalRec(wTabData,MAX_OPENTABROW);
    ProtSendTbl(ptb);
}

//数据清空
static void ClearData()
{
    PPROT_TBL_HD ptb = GetTableByType(TB_OPENCAL, TB_CUR);
    UI16 wTabData[MAX_OPENTABROW * (MAX_OPENROWLENTH/2)]= {0};
    int i;

    if(ptb == NULL)
    {
        return;
    }

    for(i = 0; i < MAX_OPENTABROW * (MAX_OPENROWLENTH/2); i++)
    {
        VarAdrSetInt(TMP_FIRST_DATA+i, wTabData[i]);
    }

    memcpy((UI8*)(ptb+1), wTabData, MAX_OPENTABROW * MAX_OPENROWLENTH);

    SaveOpenCalRec(wTabData,MAX_OPENTABROW);
    ProtSendTbl(ptb);
}

static void CancelButton()
{

}

//算法数据可设
static void OpenCalDataCanSet(BOOL canset)
{
    VarAdrSetInt(BUTTON_ENABLE, canset);
    VarAdrSetInt(EDIT_SHOW, canset);
}

static void OpenCalShow()
{
    PPROT_TBL_HD ptb = GetTableByType(TB_OPENCAL, TB_CUR);
    UI16 wTabData[MAX_OPENTABROW * (MAX_OPENROWLENTH/2)]= {0};
    int i;

    if(ptb == NULL)
    {
        oprintf("ptb is NULL\n");
        ptb = (PPROT_TBL_HD)OBJMemMalloc(MAX_OPENTABROW * MAX_OPENROWLENTH+sizeof(PROT_TBL_HD));

        if(!LoadOpenCalRec(wTabData))
        {
            SaveOpenCalRec(wTabData,MAX_OPENTABROW);
        }
        ptb->row_len = MAX_OPENROWLENTH;
        ptb->row_num = MAX_OPENTABROW;
        ptb->type = TB_OPENCAL;
        ptb->sn = 1;
        memcpy((UI8*)(ptb+1), wTabData, MAX_OPENTABROW * MAX_OPENROWLENTH);
        AddOpenCalTable(ptb);
    }
    else
    {
        memcpy(wTabData, (UI8*)(ptb+1), MAX_OPENTABROW * MAX_OPENROWLENTH);
    }

    for(i = 0; i < MAX_OPENTABROW * (MAX_OPENROWLENTH/2); i++)
    {
        VarAdrSetInt(TMP_FIRST_DATA+i, wTabData[i]);
    }
    VarAdrSetInt(TMP_PASSWORD, 1234);
    OpenCalDataCanSet(FALSE);
}

static void CtlConnect()
{
}
static int Open_CalOnInit(Open_Cal_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Open_CalOnShow(Open_Cal_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    OpenCalShow();
    return 0;
}
static int Open_CalOnHide(Open_Cal_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Open_CalOnUpdate(Open_Cal_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Open_CalOnChange(Open_Cal_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(pwgt == ppg->edpassword)
    {
        if(VarAdrToInt(TMP_PASSWORD) == 9595)
        {
            OpenCalDataCanSet(TRUE);
        }
        else
        {
            OpenCalDataCanSet(FALSE);
        }
    }
    else if(pwgt == ppg->btsave)
    {
        g_question.pOkfunc = SaveData;
        g_question.pCancelfunc = CancelButton;
        ShowQuestion(VW_MSG_WHETHER_SAVE,g_question);
    }
    else if(pwgt == ppg->btclear)
    {
        g_question.pOkfunc = ClearData;
        g_question.pCancelfunc = CancelButton;
        ShowQuestion(VW_MSG_WHETHER_CLEAR,g_question);
    }

    return 0;
}
