#ifndef CMDREC_H
#define CMDREC_H
#include "typedef.h"
#include "obj.h"
#include "mod.h"
#include "prot.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define MAX_CMD_SIZE (2000+1)

typedef struct tyCMDITEM{
    UI32 cmd;
    char data[40];
    char time[20];
    UI32 open_cnt;
    UI8  state;   //0 手动 1 半自动 2 全自动
    UI32 rev[4];
}CMDITEM;

typedef struct tyCMDHEAD
{
    UI16        front;
    UI16        rear;
}CMDHEAD;

typedef struct tyCMDRECORD
{
    CMDHEAD     cmdhead;
    CMDITEM     item[MAX_CMD_SIZE];
} CMDRECORD,*PCMDRECORD;


void ClearCmdRec();

void CmdRecInit();
BOOL ReadCmdRec(CMDITEM* item,UI32 index);
BOOL CmdRecIsChg();
void CmdSendRecInit(PPROT_HD phd, UI8 prot_id);
#define CMD_REC_PATH SAVE_PATH"cmdrec.dat"

#ifdef __cplusplus
}
#endif
#endif // CMDREC_H
