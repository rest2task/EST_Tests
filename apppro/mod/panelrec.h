#ifndef PANELREC_H
#define PANELREC_H
#include "typedef.h"
#include "obj.h"
#include "wgt.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define MAX_PANEL_SIZE (500+1)

typedef enum enPANELRECTYPE
{
    PNL_CLEAR_ALL_REC = 1,
    PNL_READ_U,
    PNL_WRITE_U,
    PNL_READ,
    PNL_SAVE,
    PNL_DELETE,
    PNL_CLEAR_CUR_DAY,
    PNL_CLEAR_CUR_MON,
    PNL_EXP_EXCEL,
    PNL_RESET,
    PNL_ALL,
    PNL_HOST,
    PNL_PANEL,
    PNL_MOLDSET,
    PNL_SERVO,
    PNL_EXPORT,
    PNL_INPUT,
    PNL_RESTORETODELIVERY,
    PNL_RESTORETODEFAULT,
    PNL_MACHSET,//20201210
    PNL_UPDATE,
    PNL_UPDATE_PACK_FAIL,
    PNL_CHANGE_PANEL_TM, //20250306 chj 记录修改时间操作
}PANELRECTYPE;

typedef struct tyPANELITEM{
    char idTitle[32];
    UI16 data1;
    UI16 data2;
    char time[20];
    UI32 rev[4];
}PANELITEM;

typedef struct tyPANELHEAD
{
    UI16        front;
    UI16        rear;
}PANELHEAD;

typedef struct tyPANELRECORD
{
    PANELHEAD     panelhead;
    PANELITEM     item[MAX_PANEL_SIZE];
} PANELRECORD,*PPANELRECORD;


void ClearPanelRec();

void PanelRecInit();
BOOL ReadPanelRec(PANELITEM* item,UI32 index);
BOOL PanelRecIsChg();
void PanelRecAdd(PWGT pobj, PANELRECTYPE data1, PANELRECTYPE data2);
#define PANEL_REC_PATH SAVE_PATH"panelrec.dat"

#ifdef __cplusplus
}
#endif
#endif // PANELREC_H
