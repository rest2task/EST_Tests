#ifndef _AIRBLOW_H_
#define _AIRBLOW_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"
#include	"airblow_dic.h"
#include "part.h"

#define	MAX_SEQ_NUM	36 //顺序阀上限，最大可以做45个

//============顺序阀模块===================================================
typedef struct tyMODULE_SEQVLV_DICDATA
{
	UI16  FL_SEQ_USE;                   //顺序阀使用  0  不使用   1   时间开时间关  2  位置开位置关  3  时间开位置关  4  位置开时间关 默认0
    UI16  TM_SEQ_DELAY;                 //顺序阀延迟计时          默认0.00 范围0.00-600.00    
    UI16  TM_SEQ_MOVING;                //顺序阀动作计时          默认0.00 范围0.00-600.00        
    UI16  ADPOSI_SEQSTART;              //顺序阀动作开位置     默认0.0 范围 0.0-射出尺上限
    UI16  ADPOSI_SEQEND;                //顺序阀动作结束位置           默认0.0   范围 0.0-射出尺上限

	UI16  O_AIR_SEQ;                    //输出点 顺序阀	
}MODULE_SEQVLV_DICDATA,*PMODULE_SEQVLV_DICDATA;

typedef struct tyMODULE_SEQVLV_DATA
{
	MODULE_SEQVLV_DICDATA seq_dic;	//顺序阀相应设定数据库
	UI16 admeter_ch;
	
	UI16 startmode;	//开启模式	1:时间	2:位置
	UI16 endmode;	//关闭模式	1:时间	2:位置
	UI16 step;		//步骤	0:	关闭 1:静默	2:时间等待开启	3:位置等待开启	4:时间等待关闭	5:位置等待关闭 
	UI16 starttime;
}MODULE_SEQVLV_DATA,*PMODULE_SEQVLV_DATA;
//=========================================================================



typedef enum enAIRBLOW_MODE
{
	AIRBLOW_TM_MD, 			/*延迟时间*/
	AIRBLOW_G_POS_MD,		/*大于某电子尺位置*/
	AIRBLOW_L_POS_MD,		/*小于某电子尺位置*/
	AIRBLOW_MD_END
}AIRBLOW_MODE;

typedef struct tyAIRBLOW
{
	PART				part;
	
	PAIRBLOW_PO		po;
	PAIRBLOW_MOLDSET	md;
	PAIRBLOW_STATE		st;

	ACT					act[AIRBLOW_ACT_SN_END - 1];
	
	UI8					pos_ch;
	UI8					mode;					/*AIRBLOW_MODE*/

//	MODULE_SEQVLV_DICDATA	seq_vlv_data[3];
//	PMODULE_SEQVLV_DICDATA pseq_vlv_data;
}AIRBLOW, *PAIRBLOW;

#define AIRBLOW_INIT_INFO	{{PO_ID, CAL_LEN(AIRBLOW_PO)}, {MD_ID, CAL_LEN(AIRBLOW_MOLDSET)},  {ST_ID, CAL_LEN(AIRBLOW_STATE)}}

BOOL InitAirBlowPart(PAIRBLOW pp, UI8 sn);
UI32 AirBlowCmd(PAIRBLOW pp, UI32 cmd, UI32 para);
void InitSeqVlvMain(void);
BOOL ChkAirBlowComplete(void);




#ifdef __cplusplus
}
#endif

#endif

