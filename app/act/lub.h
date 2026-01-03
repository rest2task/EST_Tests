#ifndef _LUB_H_
#define _LUB_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include	"part.h"
#include	"lub_dic.h"

enum enLUB_MODE
{
	LUB_ONE,					 //时间模式，单次润滑
	LUB_TM_AND_PRESS,			 //继电器多次润滑 时间到 压力到
	LUB_TM_OR_PRESS,			//继电器多次润滑 时间到或压力到即转换
	LUB_TM_OR_PRESS2,			//继电器多次润滑 时间到或压力到即转换
	LUB_MODE_END
};

typedef struct tyLUB
{
	PART				part;
	
	PLUB_PO				po;
	PLUB_PI				pi;
	PLUB_MACHSET		mh;
	PLUB_STATE			st;

	WORD        		lubdelay  ;   // lubricate start time
	WORD        		lublast   ;   // lubricate lasting time

	ACT				act[LUB_ACT_SN_END - 1];

	WORD        		luberrtwice; 
	UI16                lubcmp;//润滑完成标记  0 未完成   1 完成
	UI16        		alarmlubfirst;	//自动开始首模判断是否上次润滑出错

	UI16				lub_on;	//1:表示润滑开启
}LUB, *PLUB;

#define LUB_INIT_INFO	{{PO_ID, CAL_LEN(LUB_PO)}, {PI_ID, CAL_LEN(LUB_PI)}, {MH_ID, CAL_LEN(LUB_MACHSET)}, {ST_ID, CAL_LEN(LUB_STATE)}}

/*接口函数*/
BOOL LubLevelErr(PLUB pp);
BOOL InitLubPart(PLUB pp, UI8 sn);
PI_STATUS LubChkPIChk(PLUB pp);
UI32 LubCmd(PLUB pp, UI32 cmd, UI32 para);
void LubNeedChk(PLUB pp,UI16 lubcnt);
void LubClmpLeftClear(PLUB pp);


#ifdef __cplusplus
}
#endif

#endif



