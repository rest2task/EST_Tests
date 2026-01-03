#ifndef _QUESTION_DLG_h
#define _QUESTION_DLG_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "typedef.h"

typedef void BTFUNC();
typedef struct tyQUESTION{
	BTFUNC *pOkfunc;
	BTFUNC *pCancelfunc;
}QUESTION;

void ShowQuestion(int langid,QUESTION doQuestion);
void ShowInformation(int langid,BTFUNC* doInformation);
void ShowDialog_CoreProt(UI32 changed_Addr, UI16 old_value);//20211221 dyl

#ifdef __cplusplus
}
#endif

#endif



