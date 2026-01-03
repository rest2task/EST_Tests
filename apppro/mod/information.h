#ifndef _INFORMATION_h
#define _INFORMATION_h

#include "panel.h"
#include "privdef.h"
//信息提示框
#define INFORMATION_INDEX     0x33120045 //信息提示框ID地址
#define PAGE_INFORMATION      "Information"

#define ShowMsg(index)		\
	VarAdrSetInt(INFORMATION_INDEX,(UI32)index);	\
	PanelShowDialogByName(PAGE_INFORMATION);




#endif
