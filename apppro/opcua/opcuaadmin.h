#ifndef _OPCUAADMIN_h
#define _OPCUAADMIN_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "mod.h"
#include "open62541.h"

void initOpcua();
void addAddr();
void flashOpcua();
void closeOpcua();
void SpcAddrFlash();
UA_Boolean GetOPCUArunState();


#ifdef __cplusplus
}
#endif

#endif



