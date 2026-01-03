#include "WFace.h"
#include "typedef.h"
#include "pagefram.h"
#include "common.h"
#include "panel.h"
#include "labelwgt.h"
#include "stdio.h"

BOOL nFaceIn = FALSE;

DEFINE_FACE_MAP

CONNECT_FACE_EVENT

#define TickCnt 8000

static UI32 TimeStart = 0;

static void CtlConnect()
{
}
static int FaceOnInit(Face_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int FaceOnShow(Face_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    nFaceIn = TRUE;
    TimeStart = GetTick();
    return 0;
}
static int FaceOnHide(Face_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    nFaceIn = FALSE;
    return 0;
}
static int FaceOnUpdate(Face_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 TimeStop = GetTick();
    if ((TimeStop-TimeStart) > (TickCnt))
    {
        nFaceIn = FALSE;
        PanelShowPage(PanelMainPage());
    }
    else
    {
        char text[255];
        UI16 cnt = (TickCnt - (TimeStop-TimeStart))/1000;
        sprintf(text, "%d", cnt);
        LabelSetText(face_pg.labelcnt, text);
    }

    return 0;
}
static int FaceOnChange(Face_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
