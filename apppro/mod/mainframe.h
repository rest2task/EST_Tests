#ifndef _MAINFRAME_H
#define _MAINFRAME_H

#ifdef __cplusplus
extern "C"
{
#endif
 
#include "mod.h"
#include "protcmd.h"

void ActCmdRx(PPROT_TSK_ST_MSG_PKT ppkt);
const char* WindowTitle();


#define ACTSETP_FILE			DATA_PATH"step.map"


#ifdef __cplusplus
}
#endif

#endif




