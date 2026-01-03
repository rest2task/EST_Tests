#ifndef _TEXTTBL_h
#define _TEXTTBL_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "mod.h"

extern	PHASH_TBL pmap_texttbl;

void TextTblInital();
const char* GetTextTran(int mapid );
#ifdef __cplusplus
}
#endif

#endif



