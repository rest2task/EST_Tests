#ifndef _DATA_CFGTB_h
#define _DATA_CFGTB_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "mod.h"

	typedef struct tyVERSION
	{
		UI16    datecode;
		UI16    ver;
		char    system1[21+1];
		char    machine1[8+1];
		char    machine2[8+1];
		UI16    reserve[20];
	} VERSION;


#define FILE_NAME_LENTH		(128)

extern VERSION g_version;

char * ConvertDirInfoToStr(FILEDIR * dirinfo);
VERSION ReadVersion();
void WriteVersion(VERSION version);
#ifdef __cplusplus
}
#endif

#endif



