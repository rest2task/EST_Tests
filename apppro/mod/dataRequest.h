#ifndef _DATAD_OPERATION_h
#define _DATAD_OPERATION_h
#include "monmcu.h"
#ifdef __cplusplus
extern "C"
{ 
#endif

BOOL DataRequest(UI32 addr,DATA_SET_RX_CB cb);

#ifdef __cplusplus
}
#endif

#endif



