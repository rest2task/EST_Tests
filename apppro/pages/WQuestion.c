#include "WQuestion.h"
#include "typedef.h"
#include "questionDlg.h"
#include "panel.h"
#include "vardb.h"
#include "database.h"
#include "buttonwgt.h"
#include "texttbl.h"
#include "oprintf.h"
#include "key.h"
#include "privdef.h"

#define ADR_QUESTION	0x33120097


#define LANMAP_FILE			DATA_PATH"information.map"

static QUESTION g_DoQuestion;
extern int PUMPFUNC_CHANGE_FLAG; //pump表数据是否有改动的标志位
extern int ADPageFlag;//20250320 jhh AD画面归零和调整位置改变时弹出数据更改提示框
extern int ADPageFlag_ex;
DEFINE_QUESTION_MAP

CONNECT_QUESTION_EVENT

BOOL QuestionSure()
{
	if(g_DoQuestion.pOkfunc != NULL)
	{
		g_DoQuestion.pOkfunc();
	}
    if(PanelCurPage() !=PanelMainPage())
    {
        PanelSetJumpFlag(FALSE);//跳转刷新标记20190108 hz
    }
	PanelCloseDialogByName(question_pg_name);
	return TRUE;
}

BOOL QuestionCancel()
{
	if(g_DoQuestion.pCancelfunc != NULL)
	{
		g_DoQuestion.pCancelfunc();
	}
	PanelCloseDialogByName(question_pg_name);
	return TRUE;
}

static int local_langid = -1;
void ShowQuestion(int langid,QUESTION doQuestion)
{
	g_DoQuestion = doQuestion;
    local_langid = langid;
    if(local_langid == VW_MSG_RESTART)
    {
        VarAdrSetInt(ADR_QUESTION,VM_MSG_DATA_PREPARATION);
    }
    else
    {
        VarAdrSetInt(ADR_QUESTION,langid);
    }
    PanelShowDialogByName(question_pg_name);
}

static void CtlConnect()
{
}
static int QuestionOnInit(Question_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int bTime = FALSE;
static int QuestionOnShow(Question_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(VarAdrToUI16(DAADJ_CHANGE_FLAG) || VarAdrToUI16(MACHCFG5200_CHANGE_FLAG) || PUMPFUNC_CHANGE_FLAG || VarAdrToUI16(p_PP_MACHSET_ECAT_CHANGE_FLAG)
            || VarAdrToUI16(ADEX_CHANGE_FLAG) || VarAdrToUI16(ADX_CHANGE_FLAG) || ADPageFlag || ADPageFlag_ex || (local_langid == VW_MSG_WHETHER_SAVE)
            || VarAdrToUI16(p_PP_MACHSET_SPECFUNC_SELECT_CHANGE_FLAG))//20250320 jhh AD画面归零和调整位置改变时弹出数据更改提示框
	{
		ButtonSetText(ppg->button8,GetTextTran(TEXT_SAVE));
	}
	else
	{
		ButtonSetText(ppg->button8,GetTextTran(TEXT_OK));
	}
    bTime = FALSE;

    //20250320 jhh AD画面归零和调整位置改变时弹出数据更改提示框
    if(ADPageFlag)
    {
        ADPageFlag = 0;
        SendKey(_LARROW);
        SendKey(KEY_RELEASE);
    }
    else if(ADPageFlag_ex)
    {
        ADPageFlag_ex = 0;
        SendKey(_LARROW);
        SendKey(KEY_RELEASE);
    }
    return 0;
}
static int QuestionOnHide(Question_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int QuestionOnUpdate(Question_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    static UI32 second = 0;
    static UI32 time_100ms = 0;
    if(GetTick() - time_100ms >= 100)
    {
        second ++;
        time_100ms = GetTick();
    }

    if(local_langid == VW_MSG_RESTART)
    {
        if(second > 20)
        {
            VarAdrSetInt(ADR_QUESTION,VW_MSG_RESTART);
            bTime = TRUE;
            second = 0;
        }
    }
    else
    {
        if(second > 6)
        {
            bTime = TRUE;
            second = 0;
        }
    }

    return 0;
}
static int QuestionOnChange(Question_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	UI32 addr; 
	PATTR pattr;

	pattr = (PATTR)msg_para;

	addr = OBJAttrGetAdr(pattr);
    if(!bTime) return 0;

	switch (addr)
	{

	default :
		if(pwgt == ppg->button8)
		{
			QuestionSure();
		}
		else if(pwgt == ppg->button9)
		{
			QuestionCancel();
		}
		break;
	}
    return 0;
}
