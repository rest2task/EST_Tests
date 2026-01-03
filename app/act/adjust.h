#ifndef _ADJUST_H_
#define _ADJUST_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "acttype.h"
#include "adjust_dic.h"
#include "ioproc.h"

#define ADJCHKERRTIME    40
#define ADJCHKERRTIME_LEARN    3000
#define ADJCHKERRCN_LEARN    5


#define PRES_MEM_NUM    200

typedef struct tag_ADJMOLDSTRU
 {
    WORD       fl_AdjMold;     	  //允许调模标记 0=不允许调模 1=允许调模    
    WORD       AdjMoldDirect;     //松模时的调模方向   0=不调模 1=调模进 2=调模退    
    WORD       cnt;               //合模计数器  
    UI32       Total;             //锁模力的累加器，用于计算锁模力的平均值
    WORD       Avereg;            //锁模力平均值，用于判断是否松模的参考
    int	       PresCurrent;       //当前模锁模力检测压力
    WORD       Presmemory[PRES_MEM_NUM];   //锁模力的60模记录值
    WORD       delay;             //手动延迟不清零延时	
} ADJMOLDSTRU, *PADJMOLDSTRU;  

typedef struct tyCLAMP*  PCLAMP;
typedef struct tyADJUST
{
	PART				part;
	
	PADJUST_PO			po;
	PADJUST_PI			pi;
	PADJUST_MACHSET		mh;
	PADJUST_ACTPARA		pa;
	PADJUST_HYDR		hy;
	PADJUST_STATE		st;

	ACT					act[ADJUST_ACT_SN_END - 1];

	PCLAMP				pclmp;
	PICNT_ST			adj_chk_cnt;

	UI16				fadjposexchg;
	UI16				timeoverb;
	UI16				adj_start_10ms;
	UI16				cnt_start_100ms;
	UI16				adj_fwd_cnt;		
	UI16				adj_bwd_cnt;
	UI16				adj_next_fwd_cnt;		
	UI16				adj_next_bwd_cnt;	
	UI16                adj_direct;  //0:fwd      1:bwd
	UI16                adj_mode;    //1 首次调进 2 再次调进 3 首次调退 4 再次调退 5 首次压模 6 再次压模 8 调模完成
	UI16				adj_fwd_onecyc;
	UI16				adj_bwd_onecyc;	
	ADJMOLDSTRU         adj_mold;  
	UI16                adj_start;

	UI16				mode_adjposlearn;	//齿距学习模式	0: 首次调模退		1:首次调模进	2:二次调模退	3:完成
}ADJUST, *PADJUST;

#define ADJUST_INIT_INFO 	{{PO_ID, CAL_LEN(ADJUST_PO)}, {PI_ID, CAL_LEN(ADJUST_PI)}, {MH_ID, CAL_LEN(ADJUST_MACHSET)}, \
							 {PA_ID, CAL_LEN(ADJUST_ACTPARA)}, {HY_ID, CAL_LEN(ADJUST_HYDR)}, {ST_ID, CAL_LEN(ADJUST_STATE)}}


BOOL InitAdjustPart(PADJUST pp, UI8 sn);
UI32 AdjustCmd(PADJUST pp, UI32 cmd, UI32 para);
void	AdjMoldInit(BOOL bforce);					//	松模调模的参数初始化
void	GetClampclsStartPres(PACT pact,PADJUST pp);	        //	获取锁模力检测时的开始压力
void	GetClampclsPresMax(PACT pact,PADJUST pp);		    //	获取高压锁模的锁模力
void	GetAvgPres(PADJUST pp);					//	记录关模次数并计算锁模力参考值
void	CheckAdjMold(PADJUST pp);					//  判断是否需要调模
void	CheckAdjUsed(void);					//  判断是否在调模或自动调模状态
							 
#ifdef __cplusplus
}
#endif

#endif



