#ifndef _KEYREC_h
#define _KEYREC_h
#include "typedef.h"
#include "obj.h"
#include "mod.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define MAX_KEY_SIZE (2000+1)

typedef struct tyKEYITEM{
    UI32 phykey;
    UI32 sendvalue;
    UI16 state;
    char time[20];
    UI32 open_cnt;
    UI8  op_state;   //0 手动 1 半自动 2 全自动
    UI32 rev[4];
}KEYITEM;

typedef struct tyKEYHEAD
{
    UI16        front;
    UI16        rear;
}KEYHEAD;

typedef struct tyKEYRECORD
{
    KEYHEAD     keyhead;
    KEYITEM     item[MAX_KEY_SIZE];
} KEYRECORD,*PKEYRECORD;

#define LAN_KEYMAP  DATA_PATH"keycfg.map"
extern PHASH_TBL lan_keymap;

BOOL SetKeyRec(UI32 phykey, UI32 sendkey, UI16 press);
void ClearKeyRec();

void KeyRecInit();
BOOL ReadKeyRec(KEYITEM* item,UI32 index);
BOOL KeyRecIsChg();



#define		KEY_REC_PATH   SAVE_PATH"keyrec.dat"



#ifdef __cplusplus
}
#endif

#endif



