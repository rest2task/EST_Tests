#ifndef _IOPROC_DIC_H_
#define _IOPROC_DIC_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"

#pragma pack(1)

#define	MAX_PI_NUM	128
#define	MAX_PO_NUM	128
#define MAX_PO_LINK_NUM	10

#define PI_STATUS_NUM		(MAX_PI_NUM%16 > 0 ? MAX_PI_NUM/16 + 1 : MAX_PI_NUM/16)
#define PO_STATUS_NUM		(MAX_PO_NUM%16 > 0 ? MAX_PO_NUM/16 + 1 : MAX_PO_NUM/16)

typedef struct EN_IO_STATE
{
	UI16	FL_ERROR0;								    //0x06160000 警报1
	UI16	FL_ERROR1;								    //0x06160001 警报2

	UI16	pi_input[PI_STATUS_NUM];					//0x06160002-0x0616009chg by rch	2012-12-13
	UI16	po_output[PO_STATUS_NUM];					//0x0616000A-0x0616011chg by rch	2012-12-13
	UI16	PI_STAT[MAX_PI_NUM];						//0x06160012
	UI16	PO_STAT[MAX_PO_NUM];						//0x06160091
}IO_STATE, *PIO_STATE;

#pragma pack()

#ifdef DIC_ENUM_NEED
#define     ER0_H24V      0x0001      //#06100 'H24V电压异常'
#define     ER0_IOV       0x0002      //#06101 '输出点短路'
//20190403 CHJ 补齐
#define		ER0_H24_1	  0x0004	  //#06102 第一块IO电压异常
#define		ER0_H24_2	  0x0008	  //#06103 第二块IO电压异常
#define		ER0_H24_3	  0x0010	  //#06104 第三块IO电压异常
#define		ER0_H24_4	  0x0020	  //#06105 第四块IO电压异常
#define     ER0_IOV_1     0x0040      //#06106 第一块IO输出短路
#define     ER0_IOV_2     0x0080      //#06107 第二块IO输出短路
#define     ER0_IOV_3     0x0100      //#06108 第三块IO输出短路
#define     ER0_IOV_4     0x0200      //#06109 第四块IO输出短路

#define     ER0_DAOVER		0x0400      //#0610A '电流输出过大,请检查线路'
#define 	ER0_IO_FORCE	0x0800	  	//#0610B	输入点强制激活中
#define		ER0_POLINK		0x1000		//#0610C	'IO输出链表失败'
#define		ER0_FORCE_TM_OUT    0x2000  // #0610D - #0610010E IO强制输出超时
#define		ER0_FORCE_TEMP  0x4000      // #0610E - #0610010F 温度过高，退出IO强制输出

#endif

#ifdef __cplusplus
}
#endif

#endif





