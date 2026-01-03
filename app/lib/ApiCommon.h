#ifndef _API_COMMON_H_
#define _API_COMMON_H_


#include 	"typedef.h"


typedef struct tyPARA_FORMAT_HD
{
	WORD Id;			// id
	WORD Cmd;
	
	WORD ParaLen;		// 参数长度	
	BYTE FormatVer;		// 格式版本
	BYTE Reserve1;		// 预留
}PARA_FORMAT_HD, *PPARA_FORMAT_HD;

//typedef struct tyPARA_FORMAT_HD* PPARA_FORMAT_HD;

#define API_PKT_LEN (sizeof(PARA_FORMAT_HD))
#define API_DEF_VER	0x01	//默认版本

#define API_HD_TO_PKT(x)		((void*)(((PPARA_FORMAT_HD)(x)) + 1))
#define API_PKT_TO_HD(x)		(((PPARA_FORMAT_HD)(x)) - 1)

void* ApiNewPkt(WORD id, WORD cmd, WORD ver, WORD data_len);
BOOL ApiFreeBuf(void* pbuf);


#endif
