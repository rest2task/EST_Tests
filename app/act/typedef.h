#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif


#define DEF_ALIGN		__align(4)

typedef signed char			I8;
typedef unsigned char 		UI8;
typedef signed short 		I16;
typedef unsigned short 		UI16;
typedef signed long			I32;
typedef signed long long	I64;
typedef unsigned long		UI32;
typedef int					HANDLE;
typedef unsigned long long	UI64;

typedef unsigned char       BYTE;
typedef unsigned short int	WORD;
typedef long                LONG;
typedef unsigned long       DWORD;
typedef unsigned char       BOOL;

typedef unsigned char 		uint8;         //  无符号8位整型变量         
typedef signed   char  		int8;          //  有符号8位整型变量           
typedef unsigned short	 	uint16;        //  无符号16位整型变量         
typedef signed   short 	    int16;         //  有符号16位整型变量         
typedef unsigned int   	    uint32;        //  无符号32位整型变量         
typedef signed   int  		int32;         //  有符号32位整型变量          
typedef float          		fp32;          //  单精度浮点数（32位长度）    
typedef double        		fp64;          //  双精度浮点数（64位长度）    

typedef unsigned short		UID;			
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#ifndef 	true
#define	 	true  	1
#endif                                                                  /*  TRUE                        */

#ifndef 	false
#define 	false 	0
#endif

#ifndef 	TRUE
#define	 	TRUE  	1
#endif                                                                  /*  TRUE                        */

#ifndef 	FALSE
#define 	FALSE 	0
#endif                   

#define     NO                  FALSE
#define     YES                 TRUE

#ifndef NULL
#define     NULL                ((void*)0)
#endif

/*must be 2^n*/
#define 	RING_BUF_SIZE	0x80
#define 	RING_IND_SIZE	0x10

#define R_BUF_INC(x)	(((x) + 1) & (RING_BUF_SIZE - 1))								/*头或尾索引加一*/
#define R_BUF_DLEN(x)	(((x).hd + RING_BUF_SIZE - (x).tl) & (RING_BUF_SIZE - 1))			/*计算数据长度*/

typedef struct tyR_BUF
{
	UI8	buf[RING_BUF_SIZE];
	UI8	tl;
	UI8	hd;
}R_BUF, *PR_BUF;


#include "dlink.h"

#ifndef OFFSETOF 
#define OFFSETOF(s, m)	 	((int)&(((s)0)->m))
#define GET_IND(s,m)		(OFFSETOF(s, m)/sizeof(UI16))
#define CAL_LEN(x)			((sizeof(x) + 1)/sizeof(UI16))		
#endif

#define ARRAY_NUM(x)		(sizeof((x))/sizeof((x)[0]))
#define DIC_ENUM_NEED

#define GET_UI16(p)			(((UI16)*((UI8*)(p) + 1) << 8) + *(UI8*)(p))
//#define PLC_ENABLE

#include "dic\typelst_dic.h"

#include "customer.h" 

#ifdef __cplusplus
}
#endif

#endif

